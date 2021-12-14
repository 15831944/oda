/////////////////////////////////////////////////////////////////////////////// 
// Copyright (C) 2002-2021, Open Design Alliance (the "Alliance"). 
// All rights reserved. 
// 
// This software and its documentation and related materials are owned by 
// the Alliance. The software may only be incorporated into application 
// programs owned by members of the Alliance, subject to a signed 
// Membership Agreement and Supplemental Software License Agreement with the
// Alliance. The structure and organization of this software are the valuable  
// trade secrets of the Alliance and its suppliers. The software is also 
// protected by copyright law and international treaty provisions. Application  
// programs incorporating this software must include the following statement 
// with their copyright notices:
//   
//   This application incorporates Open Design Alliance software pursuant to a license 
//   agreement with Open Design Alliance.
//   Open Design Alliance Copyright (C) 2002-2021 by Open Design Alliance. 
//   All rights reserved.
//
// By use of this software, its documentation or related materials, you 
// acknowledge and accept the above terms.
///////////////////////////////////////////////////////////////////////////////


#include "PdfExportCommon.h"

#include "2dExportDeviceForPdfExport.h"

#include "DbBaseDatabase.h"

#include "Pdf2dExportDevice.h"
#include "Pdf2dExportView.h"
#include "PdfAux.h"

#include "PdfFontMatrix.h"
#include "PdfPageNodeDictionary.h"

#include "OdCharMapper.h"
#include "OdPlatformStreamer.h"

#include "Ge/GeKnotVector.h"
#include "Ge/GeNurbCurve3d.h"
#include "Ge/GeNurbCurve2d.h"
#include "Ge/GeEllipArc3d.h"

#include "RxRasterServices.h"
#include "MemoryStream.h"
#include "Gi/GiRasterWrappers.h"

#include "DynamicLinker.h"
#include "PdfExportService.h"
#include "PdfExportPE.h"
#include "PdfExport.h"
#include "DbBaseHostAppServices.h"


//////////////////////////////////////////////////////////////////////////////////
// PDF2dExportDevice
//////////////////////////////////////////////////////////////////////////////////
namespace TD_PDF_2D_EXPORT
{

  PDF2dExportGeometry::PDF2dExportGeometry(const OdGsBaseVectorizeDevice& refDevice)
    : m_refDevice(refDevice)
    , m_bGraphStateFixed(false)
    , m_bOnePointPoly(false)
    , m_num(0)
    , m_LayerOpen(false)
    , m_lineWeight(-HUGE_VAL)
    , m_CapStyle(TD_PDF::kLineCapNotSet)
    , m_JoinStyle(TD_PDF::kLineJoinNotSet)
    , m_hatchDPI(72)
    , m_bwBitmapDPI(400)
    , m_colorBitmapDPI(400)
    , m_stateDrawingContour(0)
    , m_bHatchDrawingMode(false)
    , m_currentTransparencyState("")
    , m_bDwgPaletteNeeded(false)
    , m_bRasterClipping(true)
    , m_prc_context()
    , m_paperBox(OdGsDCRect::Null)
    , m_LwToDcScale(0)
    , m_pParamsHolder(NULL)
    , m_bDarkenGSSet(false)
    , m_bExportHiddenText(false)
    , m_curRGB(0xFFFFFFFF)
    , m_curColorIndex(0)
    , m_bRGBActive(true) // don't change it to false
    , m_fillRule(defaultFillRule())
{
  m_LastPoint.x = m_LastPoint.y = -HUGE_VAL;

  resetTraits();

  // Recording is disabled by default. Usually zoomExtents process is before main update();
  // So you should enable Recording before main update().
  enableRecording(false);
}

PDFDummyContentStream PDF2dExportGeometry::m_DummyCCStream = PDFDummyContentStream();

void PDF2dExportGeometry::enableRecording(bool bEnable /* = true */, bool bZ2E /* = true */)
{
  m_bRecordingEnabled = bEnable;
  if (bEnable)
  {
    m_OutputCCStream = m_pParamsHolder->ContentCommands();
    m_currentPdfPage = m_pParamsHolder->CurrentPage();
  }
  else
  {
    m_OutputCCStream = &m_DummyCCStream;
    if (bZ2E)
      m_currentPdfPage = 0;
  }
}

bool PDF2dExportGeometry::recordingEnabled()
{
  return m_bRecordingEnabled;
}

void PDF2dExportGeometry::setPaperBox(const OdGsDCRect& box)
{
  m_paperBox = box;
}

class CHatchAsPolygonState
{
public:
  CHatchAsPolygonState(OdGiDefaultContext& ctx,
    OdGiDefaultContext::SolidHatchAsPolygonMode eMode) :
    m_ctx(ctx), m_eSaved(ctx.hatchAsPolygon())
  {
    m_ctx.setHatchAsPolygon(eMode);
  }
  ~CHatchAsPolygonState()
  {
    m_ctx.setHatchAsPolygon(m_eSaved);
  }
protected:
  OdGiDefaultContext& m_ctx;
  OdGiDefaultContext::SolidHatchAsPolygonMode m_eSaved;
};

void savePalleteFromDeviceToParams(PDF2PRCExportParams &params, const OdGsBaseVectorizeDevice &device);

struct PDF2PRCViewportParams
{
  OdGeVector3d x, y, z;
  double distanceToTarget;
  double scale;
  OdGeExtents2d rect;
  OdGeMatrix3d mxWTE;
  OdGeVector3d cameraVector;

  PDF2PRCViewportParams(const OdGeExtents3d &extentsAll,
    const OdGeMatrix3d &objectToDeviceMatr,
    const OdGeMatrix3d &worldToEyeTransformMatr,
    const OdGeMatrix3d &worldToCameraMatr,
    const OdGeVector3d &viewDir,
    OdUInt16 geomDPI
    )

    : x(1, 0, 0)
    , y(0, 1, 0)
    , z(0, 0, -1)
    , distanceToTarget(extentsAll.maxPoint().distanceTo(extentsAll.minPoint()))
  {
    // camera vector (position)
    cameraVector = viewDir * distanceToTarget;

    // scale
    OdGePoint2d pnt1(extentsAll.minPoint().x, extentsAll.minPoint().y);
    OdGePoint2d pnt2(extentsAll.maxPoint().x, extentsAll.maxPoint().y);
    OdGePoint2d pnt3(extentsAll.maxPoint().x, extentsAll.minPoint().y);
    scale = odmax(pnt1.distanceTo(pnt3), pnt2.distanceTo(pnt3));
    scale = 1. / scale;

    // rect
    OdGePoint3d minPoint = extentsAll.minPoint();
    minPoint.transformBy(worldToCameraMatr);
    minPoint.transformBy(objectToDeviceMatr);
    minPoint.scaleBy(/*0.12*/ 72. / geomDPI);
    OdGePoint3d maxPoint = extentsAll.maxPoint();
    maxPoint.transformBy(worldToCameraMatr);
    maxPoint.transformBy(objectToDeviceMatr);
    maxPoint.scaleBy(/*0.12*/ 72. / geomDPI);
    rect.set((const OdGePoint2d&)minPoint, (const OdGePoint2d&)maxPoint);

    // Camera 2 World matrix
    mxWTE = worldToEyeTransformMatr; // World to Eye matrix
    mxWTE.invert(); // pdf file requires Camera 2 World matrix!
    mxWTE.setTranslation(OdGeVector3d(0, 0, 0)); // camera translation (camera target) by default is is the target of the acad camera. clear it.
    OdGeVector3d extentsCenter = extentsAll.center().asVector();
    extentsCenter.transformBy(worldToCameraMatr);
    mxWTE.setTranslation(extentsCenter);         // set camera translation to our desired target (that is the center of the extents, independent of acad camera)
  }
};

void PDF2dExportDevice::finishPRCVectorization()
{
  // get service
  PdfExportServiceInterfacePtr pExportIFace = getPdfExportService();

  for (std::map< OdUInt32, std::pair<OdRxObjectPtr, ViewExtents> > ::iterator it = geom().m_prc_context.begin(); it != geom().m_prc_context.end(); ++it)
  {
    OdGeExtents3d &extentsAll = it->second.second.m_extents;
    if(!extentsAll.isValidExtents())
      continue;

    // set export params
    PDF2PRCExportParams prcParams;
    prcParams.m_viewportIdx = it->first;
    // - compression
    PDF3D_ENUMS::PRCCompressionLevel compressLev;
    bool hasBrepCompression = exportParams().hasPrcBrepCompression(compressLev);
    prcParams.setPRCCompression(compressLev, hasBrepCompression, exportParams().hasPrcTessellationCompression());

    // export prc file from context to stream
    //NS, TODO: why to write pallete for each viewport??
    savePalleteFromDeviceToParams(prcParams, *this);

    OdStreamBufPtr pStream = OdMemoryStream::createNew();
    OdResult result = pExportIFace->serialize(pStream, it->second.first, &prcParams);
    if (result != ::eOk)
      throw OdError(result); // should not fail at this point; if it does - something bad has happened

    // write stream into pdf file
    if (pStream->length() == 0)
      return;
    OdPrcContextForPdfExportWrapper* pPrcContext = OdPrcContextForPdfExportWrapper::cast(geom().exportParams().getPRCContext());
    if(pPrcContext)
      pPrcContext->getOutputPRC()[it->first] = pStream;
    OdGsView *pView = viewAt(it->second.second.m_viewIdx);
    if(!pView)
    {
      ODA_ASSERT(false);
      continue;
    }
    OdGeVector3d cameraVector = pView->position() - pView->target();
    cameraVector.normalize();
    OdGeVector3d normalToCamera = pView->upVector();
    OdGeVector3d third = normalToCamera.crossProduct(cameraVector);
    OdGeMatrix3d cameraView;
    cameraView.setCoordSystem(OdGePoint3d(0.0f ,0.0f ,0.0f), third, normalToCamera, cameraVector);
    PDF2PRCViewportParams viewportParams(extentsAll, pView->objectToDeviceMatrix(), pView->viewingMatrix(), cameraView, cameraVector, exportParams().getGeomDPI());
    result = geom().dc_prc(pStream, 
      viewportParams.rect, 
      viewportParams.cameraVector, 
      viewportParams.x, 
      viewportParams.y, 
      viewportParams.z, 
      viewportParams.distanceToTarget,
      viewportParams.scale,
      &viewportParams.mxWTE);
    ODA_ASSERT(result == ::eOk);
  }
}

TD_PDF::PDFResourceDictionaryPtr PDF2dExportGeometry::getResourceDictionary()
{
  PDFResourceDictionaryPtr pResDict;
  if (m_currentPdfPage)
  {
    if (m_pParamsHolder->getParams().export2XObject())
    {
      PDFXObjectForm* pXobj = dynamic_cast<PDFXObjectForm*>(m_pParamsHolder->ContentCommands());
      if (NULL != pXobj)
        pResDict = pXobj->dictionary()->getResources();
    }
    else
    {
      if (m_currentPdfPage->HasItem("Resources"))
        pResDict = m_currentPdfPage->getResources();
    }
  }
  return pResDict;
}

void PDF2dExportGeometry::beginOutput(ODCOLORREF bg)
{
  if(isLayersSupported() && m_LayerOpen)
  {
    CloseLayerBracket();
    m_curLayerName = "";
  }

  // try to use PdfExport parameter background color.
  if(m_currentPdfPage && bg != ODRGB(255, 255, 255) && !m_paperBox.is_null())
  {
    set_Color(bg);
    double x = m_paperBox.m_max.x - m_paperBox.m_min.x;
    double y = m_paperBox.m_max.y - m_paperBox.m_min.y;
    OdGePoint2d rect[4];
    rect[0].set(0, 0);
    rect[1].set(x, 0);
    rect[2].set(x, y);
    rect[3].set(0, y);
    dc_polygon(4, rect);
  }
}

void PDF2dExportGeometry::endOutput()
{
  if(isLayersSupported() && m_LayerOpen)
  {
    CloseLayerBracket();
    m_curLayerName = "";
  }
}

void PDF2dExportDevice::doUpdateGeometry()
{
  //NS: there should be the same initialization as in PDF2dExportDevice::update()
  //Begin
  OdGiDefaultContext* pCtx = OdGiDefaultContext::cast(userGiContext());
  ODA_ASSERT(pCtx);
  CHatchAsPolygonState ss(*pCtx, //hatches will be handled as polygons on rendering stage
    (exportParams().solidHatchesExportType() == PDFExportParams::kPolygons) ?
    OdGiDefaultContext::kHatchPolygon : pCtx->hatchAsPolygon());

  OdGsBaseVectorizeDevice::update(NULL);
}

void invalidateViewModelChanges(OdGsViewImpl& view);
void invalidateViewModelChanges(OdGsBaseVectorizeDevice& device)
{
  for(int nView = 0; nView < device.numViews(); nView++)
  {
    OdGsViewImpl *pView = device.viewImplAt(nView);
    if(pView && pView->isVisible())
      invalidateViewModelChanges(*pView);
  }
}

void PDF2dExportDevice::update(OdGsDCRect* pUpdatedRect)
{
  geom().enableRecording();
  OdGiDefaultContext* pCtx = OdGiDefaultContext::cast(userGiContext());
  ODA_ASSERT(pCtx);
  CHatchAsPolygonState ss(*pCtx, //hatches will be handled as polygons on rendering stage
  (exportParams().solidHatchesExportType() == PDFExportParams::kPolygons) ?
    OdGiDefaultContext::kHatchPolygon : pCtx->hatchAsPolygon());

  // #CORE-16421 : this part is required to update pre-built caches in case if them influenced by view changes.
  invalidateViewModelChanges(*this);
  for(int nDevice = 0; nDevice < numRenderDevices(); ++nDevice)
  {
    OdGsBaseVectorizeDevice *pRenderDevice = OdGsBaseVectorizeDevice::cast(renderDeviceAt(nDevice));
    if(pRenderDevice)
      invalidateViewModelChanges(*pRenderDevice);
  }
  geom().beginOutput(getBackgroundColor());
  Od2dExportDevice::update(pUpdatedRect);
  if(exportParams().getPRCMode() != PDFExportParams::kDisabled)
    finishPRCVectorization();
  geom().endOutput();
  geom().enableRecording(false);
}

double PDF2dExportDevice::renderAreaMultiplier(OdUInt32 width, OdUInt32 height, ShadedViewportExportMode& mode) const
{
  OdUInt32 dimMax = odmax(width, height);
  double koef = 150 / 72.;
  PDFExportParams::PDFShadedViewportExportMode pdfMode = geom().exportParams().shadedVpExportMode();

  if (pdfMode != PDFExportParams::kExportOnePiece)
  {
    //there is a constant because we already have the correctly calculated resolution for the viewport
    //we just need to downscale it because of for example:
    //dpi 72 for 10 inch x 10 inch paper it is 360x360 picture but not the 720x720 (at least Autocad calculates it this way)
    koef = .5;

    //We can create larger pictures but in practice it is very painful to view such PDFs in viewers,
    //so the image size is restricted
    if (dimMax * koef > 20000.)
      koef = 20000. / dimMax;

    mode = (pdfMode == PDFExportParams::kExportAsTiles) ? kAsTiles : kAsStrips;

    return koef;
  } 
  if (dimMax * koef > 4096.) // 3d renderer limitation
    koef = 4096. / dimMax;
  mode = kAsStrips;
  return koef;
}

bool PDF2dExportDevice::isRenderDeviceRequired(const OdGsView& view) const
{
  if(exportParams().forceDisableGsDevice())
    return false;
  return super::isRenderDeviceRequired(view);
}

OdResult PDF2dExportGeometry::makeBgTransparent(OdUInt8Array &buffer, OdInt32 width, OdInt32 height, ODCOLORREF backgroundColor, OdGiRasterImagePtr &pRast) const
{
  int bytesPerLine = width * 4;
  OdStreamBufPtr pMem = OdMemoryStream::createNew();

  OdPlatformStreamer::wrInt16(*pMem, 0x4d42);
  OdInt32 scanLinesPos = 54;
  OdInt32 size = scanLinesPos + bytesPerLine * height;
  OdPlatformStreamer::wrInt32(*pMem, size);
  OdPlatformStreamer::wrInt32(*pMem, 0); // reserved
  OdPlatformStreamer::wrInt32(*pMem, scanLinesPos); // offBits
  OdPlatformStreamer::wrInt32(*pMem, 40);
  // save BITMAPINFOHEADER
  OdPlatformStreamer::wrInt32(*pMem, width);
  OdPlatformStreamer::wrInt32(*pMem, height);
  OdPlatformStreamer::wrInt16(*pMem, 8);
  OdPlatformStreamer::wrInt16(*pMem, 32);
  OdPlatformStreamer::wrInt32(*pMem, 0);
  OdPlatformStreamer::wrInt32(*pMem, 0);
  OdPlatformStreamer::wrInt32(*pMem, 0);
  OdPlatformStreamer::wrInt32(*pMem, 0);
  OdPlatformStreamer::wrInt32(*pMem, 0);
  OdPlatformStreamer::wrInt32(*pMem, 0);

  // Number of additional bytes at line end in the original RGB bitmap.
  int bytesAtEnd = (4 - ((width * 3) % 4)) % 4;

  OdUInt8Array::const_iterator pE = buffer.begin();
  OdUInt8Array::const_iterator pB = pE + (width * 3 + bytesAtEnd) * height - bytesAtEnd - 3;

  OdUInt8 *pCurBGRA = buffer.begin() + width * height * 4 - 4;

  // convert RGB to BGRA format.
  for (; pB >= pE; pB -= bytesAtEnd) // bitmap lines are 32 bits aligned. Skip it.
  {
    for (OdUInt32 p = width; p > 0; --p, pB -= 3)
    {
      ODA_ASSERT(pCurBGRA >= pE);
      // alfa component
      {
        pCurBGRA[2] = pB[2];
        pCurBGRA[1] = pB[1];
        pCurBGRA[0] = pB[0];
        pCurBGRA[3] = 255; // not transparent

        if (pB[0] == ODGETRED(backgroundColor) && pB[1] == ODGETGREEN(backgroundColor) && pB[2] == ODGETBLUE(backgroundColor))
        {
          pCurBGRA[3] = 0; // 100% transparent
        }
      }
      pCurBGRA -= 4;
    }
  }

  ODA_ASSERT(pCurBGRA + 4 == pE);
  ODA_ASSERT(pB + bytesAtEnd + 3 == pE);

  pMem->putBytes(buffer.getPtr(), buffer.size());

  OdRxRasterServicesPtr pRasSvcs = odrxDynamicLinker()->loadApp(RX_RASTER_SERVICES_APPNAME);
  if (pRasSvcs.isNull())
    return eExtendedError;

  pMem->rewind();
  //OdStreamBufPtr pF = odrxSystemServices()->createFile("f:\\1.bmp", Oda::kFileWrite, Oda::kShareDenyNo, Oda::kCreateAlways);
  //pMem->copyDataTo(pF);
  //pMem->rewind();
  pRast = pRasSvcs->loadRasterImage(pMem);
  return ::eOk;
}

void PDF2dExportDevice::calculateRenderDeviceClipping(OdGsDevice *pRenderDevice, OdGeExtents3d& ext2d, OdGeExtents3d& extDevice,
                                                      OdIntArray& nrcCounts, OdGsDCPointArray& nrcPoints, int nDevice, bool& bDrawPicture) const
{
  bool bUseClipping = true;
  bDrawPicture = false;
  OdGeMatrix3d xfm;

  for (int i = 0; i < pRenderDevice->numViews(); ++i)
  {
    const OdGsView* pView = viewAt(i);

    OdGsView* pRenderedView = pRenderDevice->viewAt(i);
    if(!pRenderedView->isVisible()) //there is no point to process hidden views
      continue;

    OdGsClientViewInfo vinfo;
    pView->clientViewInfo(vinfo);
    std::map<OdDbStub *, OdUInt32>::const_iterator pRes = m_arrViewports.find(vinfo.viewportObjectId);
    if (pRes != m_arrViewports.end() && pRes->second == nDevice)
    {
      bDrawPicture = true;
      OdGePoint3d ll, ur;

      pView->getViewport((OdGePoint2d&)ll, (OdGePoint2d&)ur);
      xfm = pView->screenMatrix();
      ll.transformBy(xfm);
      ur.transformBy(xfm);
      extDevice.addPoint(ll);
      extDevice.addPoint(ur);

      if (bUseClipping)
      {
        const Od2dExportView* p2dExpView = static_cast<const Od2dExportView*>(pView);
        OdIntArray nrcCountsNew;
        OdGsDCPointArray nrcPointsNew;
        p2dExpView->viewportClipRegion(nrcCountsNew, nrcPointsNew);
        if (nrcCountsNew.empty())
        {
          bUseClipping = false;
          nrcCounts.clear();
          nrcPoints.clear();
        }
        else
        {
          // create union clipping area of two clippings
          unionNrcClips(nrcCounts.size(), nrcCounts.asArrayPtr(), nrcPoints, nrcCountsNew.size(), nrcCountsNew.asArrayPtr(), nrcPointsNew, nrcCounts, nrcPoints);
        }
      }
    }
  }

  //recalculate viewport for GsDevice cause we already have it in PdfDevice, it's just turned upside down
  //so we won't need to call onSize() and update() for GsDevice and create the huge raster
  //we also need to take into account that for GsDevice viewport coordinates start paper corner (see onSize(OdGsDCRect(0, wholeWidth, wholeHeight, 0)))
  //so for PdfDevice, viewport coordinates will be paper corner + m_outputRect.m_min
  OdUInt32 wholeWidth = Od_abs((int)(m_outputRect.m_max.x - m_outputRect.m_min.x));
  OdUInt32 wholeHeight = Od_abs((int)(m_outputRect.m_max.y - m_outputRect.m_min.y));
  ShadedViewportExportMode exMode;
  double koef = renderAreaMultiplier(wholeWidth, wholeHeight, exMode);
  OdGePoint3d pt((extDevice.minPoint().x - m_outputRect.m_min.x)*koef, ((double)wholeHeight - (extDevice.maxPoint().y - m_outputRect.m_min.y))*koef, 0.);
  ext2d.addPoint(pt);
  pt = OdGePoint3d((extDevice.maxPoint().x - m_outputRect.m_min.x)*koef, ((double)wholeHeight - (extDevice.minPoint().y - m_outputRect.m_min.y))*koef, 0.);
  ext2d.addPoint(pt);
}

bool PDF2dExportGeometry::transformImage(OdGiRasterImagePtr pRastIn, OdGiRasterImagePtr& pRastOut, ODCOLORREF backgroundColor) const
{
  if (pRastIn->colorDepth() != 24)
    return false; //transform only from RGB to RGBA images
  const PDFExportParams& params = m_pParamsHolder->getParams();
  if (!m_pParamsHolder->getParams().transparentShadedVpBg())
    return false;
  else
  {
    OdUInt8Array buffer;
    buffer.resize(pRastIn->pixelWidth()*pRastIn->pixelHeight() * 4, 255);

    pRastIn->scanLines(buffer.asArrayPtr(), 0, pRastIn->pixelHeight());
    OdResult res = makeBgTransparent(buffer, pRastIn->pixelWidth(), pRastIn->pixelHeight(), backgroundColor, pRastOut);
    if (res != ::eOk || pRastOut.isNull())
      return false;
  }
  return true;
}

//====================================================================
//
OdGsDevicePtr PDF2dExportDevice::createObject()
{
  OdGsDevicePtr pRes = OdRxObjectImpl<PDF2dExportDevice, OdGsDevice>::createObject();
  return pRes;
}


//====================================================================
//
OdGsViewPtr PDF2dExportDevice::createView(const OdGsClientViewInfo* pViewInfo, bool bEnableLayVisPerView)
{
  OdGsViewPtr pView = PDF2dExportView::createObject();
  PDF2dExportView* pMyView = static_cast<PDF2dExportView*>(pView.get());

  pMyView->init(this, pViewInfo, bEnableLayVisPerView);

  pMyView->setAnalyticLinetypingCircles(true);
  pMyView->setAnalyticLinetypingComplexCurves(true);

  pMyView->screenOutput().setDestGeometry(geom());

  return pMyView;
}


//====================================================================
//
static OdDbBaseHostAppServices* getHostApp(OdDbBaseDatabase* pDb)
{
  OdDbBaseDatabasePEPtr pDbPE = OdDbBaseDatabasePE::cast(pDb);
  if(pDbPE.get())
    return pDbPE->appServices(pDb);
  return NULL;
}

DbLayout2Page::DbLayout2Page(const PDFExportParams& prms): m_prms(prms)
{
  m_hostApp = getHostApp(m_prms.database());
  if(!m_hostApp.get())
  {
    const OdRxObjectPtrArray& dbs = m_prms.databases();
    for(OdUInt32 i = 0; (!m_hostApp.get()) && (i < dbs.size()); ++i)
      m_hostApp = getHostApp(m_prms.database());
  }
}

void DbLayout2Page::init()
{
  if(m_bInitialized)
    return;
  TD_AUTOLOCK(m_mt);
  if(m_bInitialized)
    return;
  OdUInt32 nLayoutCount = m_prms.layouts().size();
  for(OdUInt32 nPageIndx = 0; nPageIndx < nLayoutCount; ++nPageIndx)
  {
    OdDbBaseDatabasePtr pDb;
    if(m_prms.databases().size())
      pDb = m_prms.databases()[nPageIndx];
    else
      pDb = m_prms.database();

    OdDbBaseDatabasePEPtr pDbPE = OdDbBaseDatabasePE::cast(pDb);
    if(!pDbPE.get())
      continue;
    OdString fName = pDbPE->getFilename(pDb);
    if(fName.isEmpty())
      continue;
    fName = getFullPath(fName, pDb);
    const OdString layout = m_prms.layouts()[nPageIndx];
    m_map[DwgLayoutKey(fName, layout)] = nPageIndx;
  }
  m_bInitialized = true;
}

OdString DbLayout2Page::getFullPath(const OdString& dbName, OdDbBaseDatabase* pCurDb)
{
  OdString path(dbName);
  OdDbBaseDatabasePEPtr pDbPE = OdDbBaseDatabasePE::cast(pCurDb);
  OdDbBaseHostAppServices* pApp = pDbPE.isNull() ? NULL : pDbPE->appServices(pCurDb);
  if(!pApp)
    pApp = m_hostApp;
  if(path.isEmpty() && pDbPE.get())
    path = pDbPE->getFilename(pCurDb);

  return pApp ? pApp->findFile(path, pCurDb) : dbName;
}

void PDF2dExportDevice::setParams(PdfExportParamsHolderPointer params_holder)
{
  geom().setParams(params_holder);
  setUseHLR(GETBIT(exportParams().exportFlags(), PDFExportParams::kUseHLR));
  setSupportParallelVect(exportParams().isParallelVectorization());
  setExportSearchableTextInRenderedViews(exportParams().searchableTextInRenderedViews());
  setExportSearchableTextAsHiddenText(exportParams().searchableTextAsHiddenText());
}

void PDF2dExportGeometry::setParams(PdfExportParamsHolderPointer params_holder)
{
  m_pParamsHolder = params_holder;
  DWG_PALETTE_NAME = PDFName::createObject(m_pParamsHolder->document(), ("DWGPalette"));
  PDF_OC_NAME = PDFName::createObject(m_pParamsHolder->document(), ("OC"));
  setHatchDPI(m_pParamsHolder->getParams().hatchDPI());
  setBwBitmapDPI(m_pParamsHolder->getParams().bwImagesDPI());
  setColorBitmapDPI(m_pParamsHolder->getParams().colorImagesDPI());
  setFrozenLayers(m_pParamsHolder->getFrozenLayers());
}

bool PDF2dExportGeometry::isFrozenLayer(const OdString &layerName) const
{
  return m_frozenLayers.find(layerName) != m_frozenLayers.end();
}

void PDF2dExportGeometry::setText(const OdGePoint3d& position,
  const OdGeVector3d& u,      // normal
  const OdGeVector3d& v,      // direction
  const OdChar* /*msg*/, OdInt32 /*length*/, bool raw,
  const OdGiTextStyle* pTextStyle,
  bool  /*bBox*/,
  const Od2dFont* /*pFont*/,
  const OdArray<OdUInt16> *pUnicode)
{
  bool isTtfFont = pTextStyle->isTtfFont();
  ODA_ASSERT(isTtfFont);
  if(!isTtfFont)
    return;

  bool bASCII = false/*true*/;
  bool bSkip(false);

  PDFFontPtr pNewFont;
  OdAnsiString fontName;

  fontName = dc_truetype_font(*pTextStyle, &pNewFont);
  if(fontName.isEmpty())
  {
    bSkip = true; ODA_ASSERT(0); // Should Pdf support all TTF ??
  }
  else
  {
    ODA_ASSERT(!pNewFont.isNull());
    bool bConvertText2Unicode = pNewFont->getSubtype()->str() == ("Type0");
    bASCII = !bConvertText2Unicode;
  }

  if(!bSkip)
  {
    /*
        if (bASCII)
        {
          dc_drawtext(fontName, position.convert2d(), u.convert2d(), v.convert2d(), *msg, length, *pTextStyle);
          getFontOptimizer().addText(pNewFont, msg);
        }
        else
    */
    {
      // Note: pUnicode canbe modified inside dc_drawunicodetext (for Arabic fonts).
      OdArray<OdUInt16> pUnicodeTmp = *pUnicode;
      dc_drawunicodetext(fontName, position.convert2d(), u.convert2d(), v.convert2d(), &pUnicodeTmp, *pTextStyle, raw);
      getFontOptimizer().addUnicodeText(pNewFont, pUnicodeTmp);
    }
  }
  else
  {
    bSkip = true;
  }
}

//====================================================================
//

//====================================================================
//
void PDF2dExportGeometry::set_Color(ODCOLORREF rgb)
{
  if (exportParams().colorPolicy() == PDFExportParams::kGrayscale)
    convertColorToGrayscale(rgb);
  else if (exportParams().colorPolicy() == PDFExportParams::kMono && (ODGETRED(rgb) != 255 || ODGETGREEN(rgb) != 255 || ODGETBLUE(rgb) != 255))
    rgb = ODRGBA(0, 0, 0, ODGETALPHA(rgb));

  if (!m_bRGBActive || m_curRGB != rgb)
  {
    //m_bRGBActive = true;
    m_curRGB = rgb; // If the color was changed
    traitsChanged(kColorModified);
  }
}

//====================================================================
//
void PDF2dExportGeometry::set_SecondaryColor(ODCOLORREF rgb)
{
  if (exportParams().colorPolicy() == PDFExportParams::kGrayscale)
    convertColorToGrayscale(rgb);
  else if (exportParams().colorPolicy() == PDFExportParams::kMono && (ODGETRED(rgb) != 255 || ODGETGREEN(rgb) != 255 || ODGETBLUE(rgb) != 255))
    rgb = ODRGBA(0, 0, 0, ODGETALPHA(rgb));

  if (!m_bRGBActive || m_curSecondaryColorRGB != rgb)
  {
    //m_bRGBActive = true;
    m_curSecondaryColorRGB = rgb; // If the color was changed
  }
}

//====================================================================
//
void PDF2dExportGeometry::set_Layer(OdDbStub* layer, OdGiContext* pCtx)
{
  if(!layer || !isLayersSupported())
    return;
  OdRxObjectPtr pLayer(pCtx->openDrawable(layer));
  if (pLayer.isNull())
    return;
  OdString strLayerName = OdDbBaseLayerPEPtr(pLayer)->name(pLayer);
  if (m_curLayerName != strLayerName)
  {
    m_curLayerName = strLayerName;
    traitsChanged(kLayerModified);
  }
}

void PDF2dExportGeometry::convertColorToGrayscale(ODCOLORREF& rgb)
{
  OdUInt8 gray = (OdUInt8)((30 * ODGETRED(rgb) + 59 * ODGETGREEN(rgb) + 11 * ODGETBLUE(rgb)) / 100);
  rgb = ODRGBA(gray, gray, gray, ODGETALPHA(rgb));
}

//====================================================================
//
void PDF2dExportGeometry::set_ColorIndex(OdUInt16 colorIndex)
{
  if(!(m_bRGBActive || m_curColorIndex != colorIndex))
    return;

  ODCOLORREF rgb = m_refDevice.getColor(colorIndex);
  if (exportParams().colorPolicy() == PDFExportParams::kGrayscale || exportParams().colorPolicy() == PDFExportParams::kMono)
  {
    if (exportParams().colorPolicy() == PDFExportParams::kGrayscale)
      convertColorToGrayscale(rgb);
    else if (exportParams().colorPolicy() == PDFExportParams::kMono && (ODGETRED(rgb) != 255 || ODGETGREEN(rgb) != 255 || ODGETBLUE(rgb) != 255))
      rgb = ODRGBA(0, 0, 0, ODGETALPHA(rgb));

    m_curRGB = rgb;
    traitsChanged(kColorModified);
  }
  else
  {
    //m_bRGBActive = false;
    m_curColorIndex = colorIndex; // If the color was changed
    traitsChanged(kColorIndexModified);
  }
}

//====================================================================
//
void PDF2dExportGeometry::set_SecondaryColorIndex(OdUInt16 colorIndex)
{
  if(!(m_bRGBActive || m_curSecondaryColorIndex != colorIndex))
    return;

  ODCOLORREF rgb = m_refDevice.getColor(colorIndex);
  if (exportParams().colorPolicy() == PDFExportParams::kGrayscale)
    convertColorToGrayscale(rgb);
  else if (exportParams().colorPolicy() == PDFExportParams::kMono && (ODGETRED(rgb) != 255 || ODGETGREEN(rgb) != 255 || ODGETBLUE(rgb) != 255))
    rgb = ODRGBA(0, 0, 0, ODGETALPHA(rgb));

  m_curSecondaryColorRGB = rgb;
  //m_bRGBActive = false;
  m_curSecondaryColorIndex = colorIndex; // If the color was changed
}

//==========================================================================================
//***************************************************************************
// getLineWeightInMM()
//
//  To translate the OdDb lineweight (in 100ths of a millimeter) to WT_Line_Weight code
//***************************************************************************
//int GsPdfDevice::getLineWeightInMM(const OdDb::LineWeight lineWeight)
double getLineWeightInMM(const OdDb::LineWeight lineWeight)
{
  double wtLineWeight = 0;

  switch (lineWeight)
  {
  case OdDb::kLnWt000:      wtLineWeight = 0;       break;
  case OdDb::kLnWt005:      wtLineWeight = 0.05;    break;
  case OdDb::kLnWt009:      wtLineWeight = 0.09;    break;
  case OdDb::kLnWt013:      wtLineWeight = 0.13;    break;
  case OdDb::kLnWt015:      wtLineWeight = 0.15;    break;
  case OdDb::kLnWt018:      wtLineWeight = 0.18;    break;
  case OdDb::kLnWt020:      wtLineWeight = 0.20;    break;
  case OdDb::kLnWt025:      wtLineWeight = 0.25;    break;
  case OdDb::kLnWt030:      wtLineWeight = 0.30;    break;
  case OdDb::kLnWt035:      wtLineWeight = 0.35;    break;
  case OdDb::kLnWt040:      wtLineWeight = 0.40;    break;
  case OdDb::kLnWt050:      wtLineWeight = 0.50;    break;
  case OdDb::kLnWt053:      wtLineWeight = 0.53;    break;
  case OdDb::kLnWt060:      wtLineWeight = 0.60;    break;
  case OdDb::kLnWt070:      wtLineWeight = 0.70;    break;
  case OdDb::kLnWt080:      wtLineWeight = 0.80;    break;
  case OdDb::kLnWt090:      wtLineWeight = 0.90;    break;
  case OdDb::kLnWt100:      wtLineWeight = 1.00;    break;
  case OdDb::kLnWt106:      wtLineWeight = 1.06;    break;
  case OdDb::kLnWt120:      wtLineWeight = 1.20;    break;
  case OdDb::kLnWt140:      wtLineWeight = 1.40;    break;
  case OdDb::kLnWt158:      wtLineWeight = 1.58;    break;
  case OdDb::kLnWt200:      wtLineWeight = 2.00;    break;
  case OdDb::kLnWt211:      wtLineWeight = 2.11;    break;

  case OdDb::kLnWtByLayer:      // -1,
  case OdDb::kLnWtByBlock:      // -2,
  case OdDb::kLnWtByLwDefault:  // -3
    ODA_ASSERT(false);
  default:
    wtLineWeight = 0;
    break;
  };

  return wtLineWeight;

}

void PDF2dExportGeometry::set_LineWeight(double lineWeight)
{
  if (m_lineWeight != lineWeight)
  {
    // If the lineWeight was changed
    m_lineWeight = lineWeight;
    traitsChanged(kLineWeightModified);
  }
}

class PdfBug16745Wrap : public OdGiRasterImage
{
public:
  PdfBug16745Wrap() {};

  void setData(OdGiRasterImagePtr pImage, OdGiRasterImage::PixelFormatInfo pixelFormat)
  {
    m_pImage = pImage;
    m_PixelFormat = pixelFormat;
  };

  virtual OdUInt32 pixelWidth() const { return m_pImage->pixelWidth(); }
  virtual OdUInt32 pixelHeight() const { return m_pImage->pixelHeight(); }
  virtual OdUInt32 colorDepth() const { return m_pImage->colorDepth(); }
  virtual OdUInt32 numColors() const { return m_pImage->numColors(); }
  virtual ODCOLORREF color(OdUInt32 colorIndex) const { return m_pImage->color(colorIndex); }
  virtual OdUInt32 paletteDataSize() const { return m_pImage->paletteDataSize(); }
  virtual void paletteData(OdUInt8* bytes) const { m_pImage->paletteData(bytes); }
  virtual PixelFormatInfo pixelFormat() const
  {
    return m_PixelFormat;
  }
  virtual Units defaultResolution(double& xPelsPerUnit, double& yPelsPerUnit) const { return m_pImage->defaultResolution(xPelsPerUnit, yPelsPerUnit); }
  virtual int transparentColor() const { return m_pImage->transparentColor(); }
  virtual OdUInt32 scanLineSize() const { return m_pImage->scanLineSize(); }
  virtual ImageSource imageSource() const { return m_pImage->imageSource(); }
  virtual const OdString &sourceFileName() const { return m_pImage->sourceFileName(); }
  virtual TransparencyMode transparencyMode() const { return m_pImage->transparencyMode(); }

  virtual const OdUInt8* scanLines() const { return m_pImage->scanLines(); }
  virtual void scanLines(OdUInt8* pBytes, OdUInt32 index, OdUInt32 numLines = 1) const { m_pImage->scanLines(pBytes, index, numLines); }

  virtual OdUInt32 scanLinesAlignment() const { return m_pImage->scanLinesAlignment(); }

private:
  OdGiRasterImagePtr m_pImage;
  OdGiRasterImage::PixelFormatInfo m_PixelFormat;

};

void PDF2dExportGeometry::rasterImageProc(const OdGePoint3d& origin,
                                        const OdGeVector3d& u,
                                        const OdGeVector3d& v,
                                        const OdGiRasterImage* pImg, // image object
                                        const OdGePoint2d* uvBoundary, // may not be null
                                        OdUInt32 numBoundPts,
                                        bool transparency,
                                        double brightness,
                                        double contrast,
                                        double fade)
{
  if (!pImg || u.isZeroLength() || v.isZeroLength())
    return;

  const OdGePoint2d* pUVBound = uvBoundary;

  OdGePoint2d tmp[4];
  if (numBoundPts == 2)
  {
    numBoundPts = 4;
    tmp[0].set((uvBoundary[0].x), (uvBoundary[0].y));
    tmp[1].set((uvBoundary[0].x), (uvBoundary[1].y));
    tmp[2].set((uvBoundary[1].x), (uvBoundary[1].y));
    tmp[3].set((uvBoundary[1].x), (uvBoundary[0].y));

    pUVBound = tmp;
  }

  OdGePoint2dArray newBoundary;
  if (numBoundPts)
  {
    OdGeMatrix2d m = createImageClipTransform(pImg, origin, u, v); // transform boundary from image coordinates to device coordinates
    for (OdUInt32 i = 0; i < numBoundPts; i++)
    {
      OdGePoint2d pt(pUVBound[i].x + .5, pUVBound[i].y + .5);
      newBoundary.push_back(pt.transformBy(m));
    }
    pUVBound = newBoundary.asArrayPtr();
  }

  // It need for bitonal images.
  ODCOLORREF entityColor(0);

  const OdCmEntityColor& color = m_effTraits.trueColor();
  if (color.isByColor())
    entityColor = ODRGB(color.red(), color.green(), color.blue());
  else
    entityColor = m_refDevice.getColor(color.colorIndex());

  if (!m_bHatchDrawingMode)
  {
    OdGiRasterImage::PixelFormatInfo pfOrig = pImg->pixelFormat();
    OdUInt32 current_dpi = OdRoundToLong((double)exportParams().getGeomDPI() / u.length());
    OdUInt32 necessary_dpi = pImg->numColors() == 2 ? m_bwBitmapDPI : m_colorBitmapDPI;
    if (OdGreater(current_dpi, necessary_dpi, .1*(double)current_dpi) ||
        (OdLess(current_dpi, necessary_dpi, .1*(double)current_dpi) && m_pParamsHolder->getParams().upscaleImages()))//set the tolerance to 10% for avoid rounding errors etc.
      //current dpi == necessary dpi for OLE images (see PDF2dExportView::ownerDrawDc)
      /*current dpi can't be less than necessary dpi for non-OLE images (because of for these images the dpi in that stage is always
      maximum that it can be, so - here is no point to increase it)*/
    {
      double iScale = (double)necessary_dpi / (double)current_dpi;
      if (iScale > 1)
      {
        OdUInt32 imageSize = pImg->pixelHeight()*pImg->pixelWidth()*pImg->colorDepth() / 8;
        while (imageSize*iScale*iScale > 80000000)
        {
          iScale -= .1*iScale;
        }
      }

      OdRxRasterServicesPtr pRasSvcs = odrxDynamicLinker()->loadApp(RX_RASTER_SERVICES_APPNAME);
      OdMemoryStreamPtr pStm = OdMemoryStream::createNew();
      OdUInt32 iFilter = iScale < 1 ? OdRxRasterServices::kRescaleBox : OdRxRasterServices::kRescaleBicubic;
      OdUInt32 flags[9] = { OdRxRasterServices::kRescale, iFilter/*OdRxRasterServices::kRescaleBox*/, OdRxRasterServices::kRescaleWidth,(OdUInt32)((double)pImg->pixelWidth()*iScale),
        OdRxRasterServices::kRescaleHeight, (OdUInt32)((double)pImg->pixelHeight()*iScale), 0, 0, 0 };
      if (pImg->numColors() == 2 && iScale < 1)
      {
        flags[6] = OdRxRasterServices::kDithering;
        flags[7] = OdRxRasterServices::kDitheringFS;
      }

      if (pRasSvcs->convertRasterImage(pImg, OdRxRasterServices::kBMP, pStm, flags))
      {
        pStm->rewind();
        OdGiRasterImagePtr pImage = pRasSvcs->loadRasterImage(pStm);

        if (!pImage.isNull())
        {
          OdGiRasterImage::PixelFormatInfo pfConverted = pImage->pixelFormat();
          if (pfOrig == pfConverted || pImg->numColors())
            dc_raster_image(origin, u / iScale, v / iScale, pImage, pUVBound, numBoundPts, transparency, brightness, contrast, fade, entityColor);
          else
          { //workaround for bug 16745
            OdSmartPtr<PdfBug16745Wrap> pWrapper = OdRxObjectImpl<PdfBug16745Wrap>::createObject();
            pWrapper->setData(pImage, pfOrig);
            dc_raster_image(origin, u / iScale, v / iScale, pWrapper, pUVBound, numBoundPts, transparency, brightness, contrast, fade, entityColor);
          }
        }
        else //if null, let's export the picture the old way at least
          dc_raster_image(origin, u, v, pImg, pUVBound, numBoundPts, transparency, brightness, contrast, fade, entityColor);
      }
      else //if false, let's export the picture the old way at least
        dc_raster_image(origin, u, v, pImg, pUVBound, numBoundPts, transparency, brightness, contrast, fade, entityColor);
    }
    else
      dc_raster_image(origin, u, v, pImg, pUVBound, numBoundPts, transparency, brightness, contrast, fade, entityColor);
  }
  else dc_raster_image(origin, u, v, pImg, pUVBound, numBoundPts, transparency, brightness, contrast, fade, entityColor);
}

void PDF2dExportGeometry::metafileProc(
          const OdGePoint3d& /*origin*/,
		      const OdGeVector3d& /*u*/,
		      const OdGeVector3d& /*v*/,
          const OdGiMetafile* /*pMetafile*/,
          bool /*bDcAligned*/,       // reserved
          bool /*bAllowClipping*/) // reserved
{
  ODA_ASSERT(0);
}

static bool isAtFont(const OdGiTextStyle* pTextStyle)
{
  if (!pTextStyle->isTtfFont())
    return false;

  const OdTtfDescriptor &descr = pTextStyle->ttfdescriptor();
  bool ret = !descr.typeface().isEmpty() && descr.typeface()[0] == '@';
  return ret;
}

void PDF2dExportGeometry::textProc(
            const OdGePoint3d& position,
            const OdGeVector3d& u, 
            const OdGeVector3d& v,
            const OdChar* msg, 
            OdInt32 length, 
            bool bRaw, 
            const OdGiTextStyle* pTextStyle,
            const OdGeVector3d* pExtrusion)
{
  OdGeVector2d u2d = u.convert2d();
  OdGeVector2d v2d = v.convert2d();
  if (OdEqual(u2d.length(), 0) || OdEqual(v2d.length(), 0)) //the text is perpendicular to eye - no necessary to draw it (BZ17053)
    return;
  PolygonFillRules currentFillRule = m_fillRule;
  m_fillRule = kFillNonzeroRule; //Bug CORE-11184 - Text renders darker/thicker during PDF Export in 4.2.2 (reason is stroking the path by default)
  if(exportHiddenText() || 
    !isTTFTextGeometry() && (pTextStyle->isTtfFont() && !isAtFont(pTextStyle)) ||
    !isSHXTextGeometry() && pTextStyle->isShxFont())
  {
    try
    {
      super::textProc(position, u, v, msg, length, bRaw, pTextStyle, pExtrusion);
    }
    catch (OdError&)
    {
      OdGiGeometrySimplifier::textProc(position, u, v, msg, length, bRaw, pTextStyle, pExtrusion);
    }
  }
  else
  {
    OdGiGeometrySimplifier::textProc( position, u, v, msg, length, bRaw, pTextStyle, pExtrusion );
  }
  m_fillRule = currentFillRule;
}

void PDF2dExportGeometry::nurbsProc(const OdGeNurbCurve3d& inputNurb)
{
  processModifiedTraits();

  OdGeNurbCurve3d nurbsCurve(inputNurb);
  unsigned int i;
  int degree;
  bool rational;
  bool periodic;
  OdGeKnotVector aKt;
  OdGePoint3dArray aCP;
  OdGeDoubleArray aWt;
  OdGeDoubleArray aWtEmpty;
  OdGeNurbCurve3d crvAux;

  try
  {
    nurbsCurve.hardTrimByParams(nurbsCurve.startParam(), nurbsCurve.endParam());
    nurbsCurve.getDefinitionData(degree, rational, periodic, aKt, aCP, aWt);
    if(degree < 2 || degree > 3 || rational)
    {
      super::nurbsProc(nurbsCurve);
      return;
    }
    crvAux = nurbsCurve;
    //crvAux.set(degree, aKt, aCP, aWtEmpty, periodic);
    if(degree == 2)
    {
      crvAux.elevateDegree(1);
      crvAux.getDefinitionData(degree, rational, periodic, aKt, aCP, aWt);
    }
  }
  catch(...) //CORE-17279 - errors in hardTrimByParams and elevateDegree
  {
    super::nurbsProc(inputNurb);
    return;
  }

  OdGeDoubleArray knots;
  aKt.getDistinctKnots(knots, NULL);
  OdGeKnotVector aKt2d;
  aKt2d.setLogicalLength(8);
  aKt2d[0] = aKt2d[1] = aKt2d[2] = aKt2d[3] = 0.;
  aKt2d[4] = aKt2d[5] = aKt2d[6] = aKt2d[7] = 1.;
  OdGePoint2dArray aCP2d;
  aCP2d.setLogicalLength(4);
  OdGeCurve3d* pCrv = &crvAux;
  PDFIContentCommands *pOut = cc();
  if(m_stateDrawingContour == 2) // first curve of contour
  {
    pOut->m(aCP[0].x, aCP[0].y);
    m_stateDrawingContour = 1;
  }
  for(i = 1; i < knots.length(); ++i)
  {
    OdGeCurve3d *crv1 = NULL;
    OdGeCurve3d *crv2 = NULL;
    if(i < knots.length() - 1)
    {
      try
      {
        pCrv->getSplitCurves(knots[i], crv1, crv2);
      }
      catch(...)
      {
        // split may fail on bad geometry, output polyline
        super::nurbsProc(*((OdGeNurbCurve3d*)pCrv));
        if(i > 1)
        {
          delete pCrv;
        }
        return;
      }
      if(i > 1)
      {
        delete pCrv;
      }
    }
    else
    {
      crv1 = pCrv;
    }

    if(crv1 == NULL)  // this can be if bad knots only (bug 8389)
    {
      return;
    }

    ODA_ASSERT(crv1->isKindOf(OdGe::kNurbCurve3d));
    OdGeNurbCurve3d* nurbs1 = (OdGeNurbCurve3d*)crv1;
    if(nurbs1->numControlPoints() != 4)  // bad geometry, knots are too close
    {
      super::nurbsProc(*nurbs1);
    }
    else
    {
      for(int j = 0; j < 4; ++j)
      {
        aCP2d[j] = nurbs1->controlPointAt(j).convert2d();
      }

      OdGeNurbCurve2d nurbs2d;
      double period;

      ODA_ASSERT(!periodic || (periodic && ((knots.length() > 2 && !nurbs1->isPeriodic(period))
        || (knots.length() == 2 && nurbs1->isPeriodic(period)))));

      nurbs2d.set(3, aKt2d, aCP2d, aWtEmpty, nurbs1->isPeriodic(period));

      if(m_stateDrawingContour > 0)
      {
        pOut->c(aCP2d[1], aCP2d[2], aCP2d[3]);
      }
      else
      {
        dc_nurb(nurbs2d);
      }
    }

    if(knots.length() > 2)
    {
      delete crv1;
    }

    pCrv = crv2;
  }
}


void PDF2dExportGeometry::shxText( const OdGePoint3d& position,
                                const OdGeVector3d& u,
                                const OdGeVector3d& v,
                                const OdChar* msg,
                                OdInt32 length,
                                bool raw,
                                const OdGiTextStyle* pStyle,
                                const OdGeVector3d* pExtrusion )
{
  ODA_ASSERT(!pExtrusion);

  bool bASCII(true);

  bool bAlwaysUnicode = GETBIT(getDeviceType(), super::kUnicodeOnly);
  if ( bAlwaysUnicode )
    bASCII = false;

  OdString sProcessedText;
  OdArray<OdUInt16>    arrayUnicode;
  OdBoolArray isInBigFont;
  OdGePoint2dArray Advances;

  if (!bASCII || !textProcessing( position, u, v, msg, length, raw, pStyle, sProcessedText, arrayUnicode, true, &isInBigFont, &Advances))
  {
    bASCII = false;
    arrayUnicode.clear();
    isInBigFont.clear();
    Advances.clear();

    textProcessing( position, u, v, msg, length, raw, pStyle, sProcessedText, arrayUnicode, false, &isInBigFont, &Advances);
  }

  setSHXText(position, u, v, msg, length, raw, pStyle, OdString(OdUS2W(arrayUnicode.asArrayPtr()), arrayUnicode.size()), isInBigFont, Advances);
}

void PDF2dExportGeometry::setSHXText(const OdGePoint3d& position,
                                   const OdGeVector3d& u,      // normal
                                   const OdGeVector3d& v,      // direction
                                   const OdChar* /*msg*/, OdInt32 /*length*/, bool raw,
                                   const OdGiTextStyle* pTextStyle,
                                   const OdString &pUnicode,
                                   const OdBoolArray &isInBigFont,
                                   const OdGePoint2dArray &Advances)
{
  bool isShxFont = pTextStyle->isShxFont();
  ODA_ASSERT(isShxFont);
  if (!isShxFont)
    return;

  //Reset possible scale (CORE-12409)///////////////////////////////////////
  OdFont *pOdFont = pTextStyle->getFont();
  OdFont *pBigFont = pTextStyle->getBigFont();
  if (TD_PDF_HELPER_FUNCS::isShx(pOdFont) || (pBigFont && TD_PDF_HELPER_FUNCS::isShx(pBigFont)))
  {
    OdTextProperties textFlags;
    textFlags.setLastChar(true);
    OdGePoint2d advance;
    OdGiEmptyGeometry emptyGeom;
    pOdFont->drawCharacter(0, advance, &emptyGeom, textFlags);
    if (pBigFont)
      pBigFont->drawCharacter(0, advance, &emptyGeom, textFlags);
    //Check if there is a scaling in the string///////////////////////////////
    if (!getFontType3Optimizer().checkTextScale(*pTextStyle, pUnicode, isInBigFont))
      throw OdError(::eInvalidInput);
  }
  //////////////////////////////////////////////////////////////////////////

  bool bVertical = pTextStyle->isVertical();

  OdGePoint2d pos2d = position.convert2d();
  OdGeVector2d u2d = u.convert2d();
  OdGeVector2d v2d = v.convert2d();

  OdUInt16Array oneUnicodeChar;
  OdDoubleArray oneCharSpace;
  //OdBoolArray oneFlag;
  OdAnsiString oneCharText;

  PDFFontPtr pT3Font, prevT3Font;
  OdAnsiString fontName, prevFontName;

  OdUInt32 nStartIndx = 0;
  OdUInt32 nLen = (OdUInt32)pUnicode.getLength();

  PDFResourceDictionaryPtr pResDict = getResourceDictionary();
  if (pResDict.isNull())
  {
    pResDict = PDFPageNodeDictionaryPtr(document().Root()->getPages())->getResources();
  }

  if (!bVertical)
  {
    for(OdUInt32 f = 0; f < nLen; ++f)
    {
      OdAnsiString tmp;
      getFontType3Optimizer().getFontForCharacter(document(), *pTextStyle, pUnicode.getAt(f), pT3Font, fontName, m_lineWeight, u2d, v2d, pResDict);
      getFontType3Optimizer().addUnicodeChar(*pTextStyle, pT3Font, pUnicode.getAt(f), tmp, isInBigFont[f]);

      // pUnicode is in MBCS for bigfont.
      OdChar realUnicode = pUnicode.getAt(f);
      if (isInBigFont[f])
      {
        OdString bigFontName = pTextStyle->bigFontFileName();
        OdCodePageId bigFontCodePage = OdCharMapper::getCpByBigFont(bigFontName);
        OdCharMapper::codepageToUnicode(realUnicode, bigFontCodePage, realUnicode);
      }

      if (prevFontName.isEmpty())
      {
        prevFontName = fontName;
        prevT3Font = pT3Font;
      }

      if (!OdEqual(pTextStyle->trackingPercent(), 1.))
      {
        //calculate char spacing
        OdGePoint3d min, max;
        OdGePoint3d pEndPos;
        drawContext()->giContext().textExtentsBox(*pTextStyle, &realUnicode, 1, raw ? kOdGiRawText : 0, min, max, &pEndPos);
        double charSpace = (pEndPos.x - pEndPos.x / pTextStyle->trackingPercent())*(-1000.) / TD_PDF_HELPER_FUNCS::textHeight(*pTextStyle);
        oneCharSpace.append(charSpace);
      }

      if (fontName != prevFontName || (f+1) == nLen)
      {
        if (f+1 == nLen)
        {
          if (fontName != prevFontName)
          {
            dc_drawtext(prevFontName, Advances[nStartIndx], u2d, v2d, *oneCharText.c_str(), oneCharText.getLength(), *pTextStyle, oneCharSpace);
            nStartIndx = f;
            oneUnicodeChar.clear();
            prevFontName = fontName;
            oneCharText = "";
          }
          oneUnicodeChar.append(realUnicode);
          //oneFlag.append(isInBigFont[f]);
          oneCharText += tmp;
        }

        // all text will be in \OOO form.
        dc_drawtext(prevFontName, Advances[nStartIndx], u2d, v2d, *oneCharText.c_str(), oneCharText.getLength(), *pTextStyle, oneCharSpace);

        nStartIndx = f;

        oneUnicodeChar.resize(1, realUnicode);
        //oneFlag.resize(1, isInBigFont[f]);
        oneCharText = tmp;
      }
      else
      {
        oneCharText += tmp;
        oneUnicodeChar.append(realUnicode);
        //oneFlag.append(isInBigFont[f]);
      }

      prevFontName = fontName;
      prevT3Font = pT3Font;
    }
  }
  else
  { // vertical text
    for(OdUInt32 f = 0; f < nLen; ++f)
    {
      OdAnsiString oneCharText;
      getFontType3Optimizer().getFontForCharacter(document(), *pTextStyle, pUnicode.getAt(f), pT3Font, fontName, m_lineWeight, u2d, v2d, pResDict);
      getFontType3Optimizer().addUnicodeChar(*pTextStyle, pT3Font, pUnicode.getAt(f), oneCharText, isInBigFont[f]);

      if (!OdEqual(pTextStyle->trackingPercent(), 1.))
      {
        //calculate char spacing
        OdGePoint3d min, max;
        OdGePoint3d pEndPos;
        OdChar realUnicode = pUnicode.getAt(f);
        drawContext()->giContext().textExtentsBox(*pTextStyle, &realUnicode, 1, raw ? kOdGiRawText : 0, min, max, &pEndPos);
        double charSpace = (pEndPos.x - pEndPos.x / pTextStyle->trackingPercent())*(-1000.) / TD_PDF_HELPER_FUNCS::textHeight(*pTextStyle);
        oneCharSpace.append(charSpace);
      }

      // all text will be in \OOO form.
      dc_drawtext(fontName, Advances[f], u2d, v2d, *oneCharText.c_str(), oneCharText.getLength(), *pTextStyle, oneCharSpace);
    } // end of vertical way
  }
}


void PDF2dExportGeometry::polygonOut( OdInt32 nbPoints, 
                                   const OdGePoint3d* pVertexList, 
                                   const OdGeVector3d* /*pNormal*/)
{
  OdGePoint2dArray pnts2d;
  pnts2d.resize(nbPoints);

  for(OdInt32 f=0; f<nbPoints; ++f)
  {
    pnts2d[f] = pVertexList[f].convert2d();
  }

  dc_polygon(nbPoints, pnts2d.asArrayPtr());
}

void PDF2dExportGeometry::shellFaceOut( OdInt32 faceListSize, const OdInt32* pFaceList, const OdGeVector3d* pNormal )
{
  OdInt32 nLoopSize = *pFaceList;
  const bool bSingleLoop = nLoopSize+1 == faceListSize;

  // gradient hatch has different vertex colors, so single loop should be triangulated
  const OdGiVertexData* vd = vertexData();
  if ((vd && vd->trueColors()) || !bSingleLoop)
  {
    super::shellFaceOut( faceListSize, pFaceList, pNormal );
    return;
  }

  OdGePoint2dArray pnts2d;
  pnts2d.resize(pFaceList[0]);

  for(OdInt32 f=1; f<=pFaceList[0]; ++f)
  {
    pnts2d[f-1] = vertexDataList()[pFaceList[f]].convert2d();
  }

  dc_polygon(pFaceList[0], pnts2d.asArrayPtr());
}

#ifdef BR_CR_10507
void PDF2dExportGeometry::shellProc(OdInt32 nbVertex,
                                  const OdGePoint3d* pVertexList,
                                  OdInt32 faceListSize,
                                  const OdInt32* pFaceList,
                                  const OdGiEdgeData* pEdgeData,
                                  const OdGiFaceData* pFaceData,
                                  const OdGiVertexData* pVertexData)
{
  bool bEdgesOn;
  if (fillMode(bEdgesOn))
  {
    // draw all edges, including internal and invisible, by face color
    // to avoid appearing of unpainted places between faces.
    OdGiSubEntityTraits& traits = m_pDrawCtx->subEntityTraits();
    const OdDb::LineWeight lw = traits.lineWeight();
    traits.setLineWeight(OdDb::kLnWt000);
    m_pDrawCtx->onTraitsModified();

    generateShellWires(faceListSize, pFaceList, NULL, pFaceData);

    traits.setLineWeight(lw);
    m_pDrawCtx->onTraitsModified();
  }
}
#endif

bool PDF2dExportGeometry::ellipArcProc(const OdGeEllipArc3d& arc, double width)
{
  OdGeEllipArc3d ellipArc(arc);
  if (OdZero(ellipArc.majorRadius()) || OdZero(ellipArc.minorRadius()))
    return false;

  OdGe::ErrorCondition ret = OdGe::kOk;
  OdGeVector3d normal = ellipArc.normal();
  normal.normalize(OdGeContext::gTol, ret);
  if(ret != OdGe::kOk)
    return false;
  if(normal.z < 0)
    ellipArc.reverseParam();
 
  double widthOld = m_lineWeight;
  setCapJoin(kButtCap, kRoundJoin);
  set_LineWeight(fabs(width));
  dc_ellipse(ellipArc);

  OdUInt32  uCapStyle   = drawContext()->giContext().lineWeightConfiguration(OdGiContext::kLineCapStyle);
  OdUInt32  uJointStyle = drawContext()->giContext().lineWeightConfiguration(OdGiContext::kLineJoinStyle);

  TD_PDF::PDFLineCap  uLineCap  = kRoundCap; 
  TD_PDF::PDFLineJoin uLineJoin = kRoundJoin;

  if( uCapStyle == 0 )
    uLineCap = kButtCap;
  else if( uCapStyle == 1 )
    uLineCap = kProjectingSquareCap;

  if( uJointStyle == 0 )
    uLineJoin = kMiterJoin;
  else if( uJointStyle == 1 )
  uLineJoin = kBevelJoin;

  setCapJoin(uLineCap, uLineJoin);

  set_LineWeight(fabs(widthOld));

  return true;
}

bool PDF2dExportGeometry::plotGeneratorEllipseOut(const OdGeEllipArc3d& arc, double width)
{
  return ellipArcProc(arc, width);
}

void PDF2dExportGeometry::startDrawingContour()
{
  m_stateDrawingContour = 2;
  ClosePolyline();
}

bool PDF2dExportGeometry::isDrawingContour()
{
  return m_stateDrawingContour != 0;
}

void PDF2dExportGeometry::endDrawingContour()
{
  if(m_stateDrawingContour == 0)
    return;
  m_stateDrawingContour = 0;
  dc_fill();
}

void PDF2dExportDevice::enableBlockCachSupport(bool bEnable)
{
  SETBIT(m_flags, kSupportBlocks, bEnable);
}

bool PDF2dExportDevice::supportPartialUpdate() const
{
  return supportBlocks();
}

bool PDF2dExportDevice::createRenderDevices(const OdRxObjectPtrArray& aView)
{
  using namespace TD_PDF_2D_EXPORT;
  OdPdfExportPEPtr pdfExportPE = OdPdfExport::desc()->getX(OdPdfExportPE::desc());
  //if(!pdfExportPE.get())
  //  return false;
  const int flags = isUseHLR() ? OdDbBaseHostAppServices::kUseSoftwareHLR : 0;
  OdDbBaseDatabasePEPtr pDbPE = OdDbBaseDatabasePE::cast(userGiContext()->database());
  OdArray<OdGsDevicePtr> aDevice;
  if(!pdfExportPE.get() || pdfExportPE->gsBitmapDevices(aView, userGiContext()->database(), flags, aDevice) != OdResult::eOk)
  {
    OdGsDevicePtr pMainDevice;
    for(unsigned i = 0; i < aView.size(); ++i)
    {
      OdGsViewPtr pView = aView[i];
      OdGsDevicePtr pDevice = createRenderDevice(pView);

      if(!pMainDevice.get() && pDevice.get())
        pMainDevice = pDevice;

      aDevice.push_back(pDevice.get() ? pMainDevice : pDevice);
    }
  }

  ODA_ASSERT(aDevice.size() == aView.size());
  std::map<OdGsDevice*, unsigned> aUniqueDevices;
  for(unsigned i = 0; i < aView.size(); ++i)
  {
    if(!aDevice[i].get())
      continue;
    OdGsViewPtr pView = aView[i];
    OdGsClientViewInfo vinfo;
    pView->clientViewInfo(vinfo);
    OdGsDevicePtr pDevice = aDevice[i];
    unsigned iDevice = 0;
    std::map<OdGsDevice*, unsigned>::const_iterator it = aUniqueDevices.find(pDevice.get());
    if(it == aUniqueDevices.end())
    {
      iDevice = m_renderDevices.size();
      aUniqueDevices[pDevice.get()] = iDevice;
      m_renderDevices.push_back(RenderDeviceInfo(aDevice[i]));
    }
    else
      iDevice = it->second;
    m_arrViewports[vinfo.viewportObjectId] = iDevice;
  }
  return true;
}

Od2dExportGeometry* PDF2dExportDevice::createGeometry() const
{
  return new PDF2dExportGeometry(*this);
}

}
