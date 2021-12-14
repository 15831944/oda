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


///////////////////////////////////////////////////////////////////////////////
//
// PdfExportImplBase.cpp : implementation of the CPdfExportImplBase class
//
///////////////////////////////////////////////////////////////////////////////

#include "OdaCommon.h"
//#ifdef _WIN32
#define STL_USING_IOSTREAM
#include "OdaSTL.h"
//#endif

#include "PdfExportCommon.h"
#include "PdfExportImplBase.h"
#include "PdfExport.h"
#include "PdfExportPE.h"

#include "Ge/GeExtents3d.h"
#include "OdTimeStamp.h"
#include "ColorMapping.h"
#include "Pdf2dExportDevice.h"
#include "Pdf2dExportView.h"
#include "PdfPageNodeDictionary.h"
#include "PdfOCManager.h"
#include "Gs/GsPageParams.h"
#include "PdfAux.h"
#include "PdfLinearizer.h"
#include "Gi/GiDrawablePtrArray.h"

#define STL_USING_MAP
#define STL_USING_VECTOR
#include "OdaSTL.h"

#include "RxObject.h"
#include "AbstractViewPE.h"
#include "MemoryStream.h"

namespace TD_PDF_2D_EXPORT {

//***************************************************************************
// 
//***************************************************************************
CPdfExportImplBase::CPdfExportImplBase()
{
}

CPdfExportImplBase::~CPdfExportImplBase()
{
  //Force releasing a device at first.
  //Order of data releasing is important.
  m_pDevice.release();
}

void CPdfExportImplBase::applyLineweightToDcScale(const PDFExportParams& prms)
{
  OdRxObjectPtr pLayout = OdDbBaseDatabasePEPtr(prms.database())->currentLayout(prms.database());
  OdDbBaseLayoutPEPtr pLayoutPE(pLayout);

  if (pLayoutPE->printLineweights(pLayout))
  {
    OdGsView* pTo = m_pDevice->viewAt(0);
    pTo->setLineweightToDcScale(prms.getGeomDPI() / kMmPerInch * 0.01);
  }
}

static void setLineweightToDcScale(OdGsDevice& device, double scale)
{
  for(int j = 0; j < device.numViews(); ++j)
  {
    OdGsView* pView = device.viewAt(j);
    if(pView)
      pView->setLineweightToDcScale(scale);
  }
}

void CPdfExportImplBase::setupPdfRenderDevices(const PDFExportParams& prms,
  OdGiDefaultContext* pCtx, PDF2dExportDevice& pdfDevice,
  OdDbStub* objectId, OdGsDCRect& clipBox,
  const OdGeBoundBlock3d& extents)
{
  OdUInt32 extentsFlag = 0;
  SETBIT(extentsFlag, OdDbBaseDatabasePE::kExactExtents, !GETBIT(prms.m_reserved1, 1));
  SETBIT(extentsFlag, OdDbBaseDatabasePE::kUseViewExtents, prms.useViewExtents());
  const bool bZ2E = (GETBIT(prms.exportFlags(), PDFExportParams::kZoomToExtentsMode) != 0);
  OdDbBaseDatabasePtr pDb = prms.database();
  OdDbBaseDatabasePEPtr pDbPE(pDb);

#ifdef BS_CHANGES_ENABLED
  OdGiDrawablePtr pDbDrw = OdGiDrawable::cast(prms.database());
  if(!pDbDrw.get() || !pdfDevice.numRenderDevices())
    return;
  AutoSavedGsModelData asgm(pDbDrw);
  ODA_ASSERT(!pDbDrw.get() || !pDbDrw->gsNode()); //gs model was detached
#endif //BS_CHANGES_ENABLED

  for (int nDevice = 0; nDevice < pdfDevice.numRenderDevices(); nDevice++)
  {
    OdGsDevice *pRenderDevice = pdfDevice.renderDeviceAt(nDevice);
    OdGsDevicePtr pDevice = setupPdfRenderDevice(prms, pDbPE, pCtx, *pRenderDevice,
      bZ2E, extentsFlag, objectId, clipBox, extents);
    pdfDevice.setRenderLayoutHelperAt(nDevice, pDevice);
#ifdef BS_CHANGES_ENABLED
    SavedGsModelData& gsModelData = pdfDevice.renderDeviceGsModelAt(nDevice);
    gsModelData.save(pDbDrw);
#endif //BS_CHANGES_ENABLED
  }
}

OdGsDevicePtr CPdfExportImplBase::setupPdfRenderDevice(const PDFExportParams& prms,
  OdDbBaseDatabasePE* pDbPE,
  OdGiDefaultContext* pCtx, OdGsDevice& renderDevice,
  bool bZ2E, OdUInt32 extentsFlag,
  OdDbStub* objectId, OdGsDCRect& clipBox,
  const OdGeBoundBlock3d& extents)
{
  renderDevice.setBackgroundColor(prms.background());
  renderDevice.setLogicalPalette(prms.palette(), 256);

  OdGsDevicePtr pDevice;
  if(objectId)
    pDevice = pDbPE->setupLayoutView(&renderDevice, pCtx, objectId);
  else
    pDevice = pDbPE->setupActiveLayoutViews(&renderDevice, pCtx);

  setLineweightToDcScale(*pDevice, prms.getGeomDPI() / kMmPerInch * 0.01);

  if(!objectId && !bZ2E)
  {
    pDbPE->applyLayoutSettings(clipBox, pDevice, prms.database(), extentsFlag, prms.getGeomDPI());
  }
  else if(!objectId && bZ2E)
  {
    extentsFlag |= OdDbBaseDatabasePE::kUseGivenExtents;
    OdGeBoundBlock3d ext(extents);
    pDbPE->zoomToExtents(clipBox, pDevice, prms.database(), ext, extentsFlag, 0);//Zero in first bit means the ExactExtents is true
  }
  return pDevice;
}

void CPdfExportImplBase::DatabasesHolder::startDbTransaction(OdDbBaseDatabase* pDb, bool bSaveGsModel)
{
  _mmap::iterator it = m_data.find(pDb);
  if(it == m_data.end())
  {
    OdDbBaseDatabasePEPtr(pDb)->startTransaction(pDb);
    m_data[pDb] = SavedGsModelData(bSaveGsModel ? OdGiDrawable::cast(pDb) : OdGiDrawablePtr());
  }
}

void CPdfExportImplBase::DatabasesHolder::saveGsModel(OdGiDrawable* pDbAsDrawable)
{
  _mmap::iterator it = m_data.find(pDbAsDrawable);
  if(it == m_data.end())
    throw OdError(eInvalidKey);
  if(!it->second.isValid())
    it->second = SavedGsModelData(pDbAsDrawable);
}

void CPdfExportImplBase::DatabasesHolder::abortTransactions()
{
  for(; !m_data.empty();)
  {
    _mmap::iterator it = m_data.begin();
    OdDbBaseDatabasePEPtr(it->first)->abortTransaction(it->first);
    if(it->second.isValid())
      it->second.restore((OdGiDrawable*)it->first);
    m_data.erase(it);
  }
}

void CPdfExportImplBase::setupPdfLayout(PdfExportParamsHolder& paramsHolder,
  OdGsPageParams& pPageParams, OdGiDefaultContextPtr pCtx,
  PDF2dExportDevice* pDevicePdf, OdDbStub* objectId /*= 0*/)
{
  const PDFExportParams& prms = paramsHolder.getParams();
  OdDbBaseDatabasePtr pDb = prms.database();
  OdDbBaseDatabasePEPtr pDbPE(pDb);
  OdGiDrawablePtr pDbDrw = OdGiDrawable::cast(prms.database());

  OdUInt32 extentsFlag = 0;
  SETBIT(extentsFlag, OdDbBaseDatabasePE::kExactExtents, !GETBIT(prms.m_reserved1, 1));
  SETBIT(extentsFlag, OdDbBaseDatabasePE::kUseViewExtents, prms.useViewExtents());
  //////////////////////////////////////////////////////////////////////////
  SETBIT(extentsFlag, OdDbBaseDatabasePE::kUseGivenView, true); //use PdfExportView to calculate extents
  //////////////////////////////////////////////////////////////////////////
  const bool bIncludeOffLayers = GETBIT(prms.exportFlags(), PDFExportParams::kIncludeOffLayers);

  pPageParams.scale((double)prms.getGeomDPI() / 72.); 
  pPageParams.set(OdRoundToLong(pPageParams.getPaperWidth()), OdRoundToLong(pPageParams.getPaperHeight()),
    int(pPageParams.getLeftMargin()), int(pPageParams.getRightMargin()),
    int(pPageParams.getTopMargin()), int(pPageParams.getBottomMargin()));

  OdGsDCRect clipBox(0, pPageParams.getPaperWidth(), 0, pPageParams.getPaperHeight());
  pDevicePdf->setPaperBox(clipBox);

  OdGsModel* pModel = NULL;
  OdGeBoundBlock3d plotExtents; //plot or view extents depending on extents calculation mode
  OdPdfExportPEPtr pdfExportPE = OdPdfExport::desc()->getX(OdPdfExportPE::desc());

  if (GETBIT(prms.exportFlags(), PDFExportParams::kZoomToExtentsMode) == 0) //!Z2E mode
  {
    m_pDevice->onSize(clipBox);
    pDbPE->applyLayoutSettings(clipBox, m_pDevice, pDb, extentsFlag, prms.getGeomDPI());
    pPageParams.set(pPageParams.getPaperWidth(), pPageParams.getPaperHeight(), clipBox.m_min.x,
      pPageParams.getPaperWidth() - clipBox.m_max.x, pPageParams.getPaperHeight() - clipBox.m_max.y, clipBox.m_min.y);

    // Apply clip region to screen
    OdGePoint2dArray clipPoints;
    clipPoints.push_back(OdGePoint2d(clipBox.m_min.x, clipBox.m_max.y));
    clipPoints.push_back(OdGePoint2d(clipBox.m_min.x, clipBox.m_min.y));
    clipPoints.push_back(OdGePoint2d(clipBox.m_max.x, clipBox.m_min.y));
    clipPoints.push_back(OdGePoint2d(clipBox.m_max.x, clipBox.m_max.y));

    pDevicePdf->m_clipPoints = clipPoints;

    if (!pdfExportPE.isNull())
    {
      OdGiDrawablePtrArray aDrw;
      pdfExportPE->createAuxDrawables(*m_pDevice, aDrw);
      pdfExportPE->addDrawables(aDrw, m_pDevice, pModel);
    }

    if GETBIT(prms.exportFlags(), PDFExportParams::kMeasuring)
    {
      setMeasuringViewport(paramsHolder, clipBox);
    }
  }
  else //Z2E mode
  {
    // temporary enable Frozen layers to calculate valid extents (#16918)
    if (bIncludeOffLayers)
      paramsHolder.frozenLayers(true);

    clipBox = OdGsDCRect(pPageParams.getLeftMargin(), pPageParams.getPaperWidth() - pPageParams.getRightMargin(),
      pPageParams.getBottomMargin(), pPageParams.getPaperHeight() - pPageParams.getTopMargin());

    pDbPE->zoomToExtents(clipBox, m_pDevice, pDb, plotExtents, extentsFlag, objectId);//Zero in first bit means the ExactExtents is true

    OdRxObjectPtr pLayout = pDbPE->currentLayout(pDb);
    OdDbBaseLayoutPEPtr pLayoutPE(pLayout);

    if (pLayoutPE->printLineweights(pLayout))
      pDevicePdf->setLwToDcScale((double)prms.getGeomDPI() / kMmPerInch * 0.01);
    //////////////////////////////////////////////////////////////////////////

    if (!pdfExportPE.isNull())
    {
      OdGiDrawablePtrArray aDrw;
      pdfExportPE->createAuxDrawables(*m_pDevice, aDrw);
      pdfExportPE->addDrawables(aDrw, m_pDevice, pModel);
    }

    if GETBIT(prms.exportFlags(), PDFExportParams::kMeasuring)
      setMeasuringViewport(paramsHolder, clipBox, &plotExtents, &pPageParams);

    if (bIncludeOffLayers)
      paramsHolder.frozenLayers(false);
  }
  setupPdfRenderDevices(prms, pCtx, *pDevicePdf, objectId, clipBox, plotExtents);
}

PagesetupToLayout::PagesetupToLayout(OdDbBaseDatabase* db, OdUInt32 pageIdx): m_db(db)
{
  if (!m_db)
    return;
  OdPdfExportPEPtr pdfExportPE = OdPdfExport::desc()->getX(OdPdfExportPE::desc());
  if (pdfExportPE.isNull())
    m_db = 0;
  else
  {
    OdDbBaseDatabasePEPtr(m_db)->startTransaction(m_db);
    pdfExportPE->applyPagesetupToLayout(pageIdx);
  }
}

PagesetupToLayout::~PagesetupToLayout()
{
  if(m_db)
  {
    if(m_savedGsModel.isValid())
      m_savedGsModel.restore(OdGiDrawable::cast(m_db));
    OdDbBaseDatabasePEPtr(m_db)->abortTransaction(m_db);
  }
}

bool canUseGsCache(const PDFExportParams &pParams);

PDFResultEx CPdfExportImplBase::base_run(PdfExportParamsHolder& paramsHolder, bool isExport2XObj)
{
  PDFExportParams& prms = paramsHolder.getParams();
  OdDbBaseDatabasePtr pDb = prms.database();
  OdDbBaseDatabasePEPtr pDbPE(pDb);
  OdGiDrawablePtr pDbDrw = OdGiDrawable::cast(prms.database());

  const bool bIncludeOffLayers = GETBIT(prms.exportFlags(), PDFExportParams::kIncludeOffLayers);
  OdUInt32 extentsFlag = 0;
  SETBIT(extentsFlag, OdDbBaseDatabasePE::kExactExtents, !GETBIT(prms.m_reserved1, 1));
  SETBIT(extentsFlag, OdDbBaseDatabasePE::kUseViewExtents, prms.useViewExtents());

  OdUInt32 nLayoutCount = prms.layouts().size();
  PDFPageNodeDictionaryPtr pPageTree(paramsHolder.document().Root()->getPages());
  PDFDictionaryPtr pResDict = pPageTree->getResources();

  OdDbStub* objectId = 0;
  bool bDwgPaletteNeeded = false;
  OdArray<ODCOLORREF> palette_array;
  DbLayout2PagePtr pDbLayout2Page;
  if(GETBIT(prms.exportFlags(), PDFExportParams::kExportHyperlinks))
    pDbLayout2Page = new DbLayout2Page(prms);

  for (OdUInt32 nPageIndx = 0; nPageIndx < nLayoutCount; ++nPageIndx)
  {
    OdGiDefaultContextPtr pCtx;
    OdDbBaseDatabasePtr pDb;
    if(prms.databases().size())
    {
      pDb = prms.databases()[nPageIndx];
      prms.setDatabase(pDb);
    }
    else
      pDb = prms.database();

    OdDbBaseDatabasePEPtr pDbPE(pDb);
    OdGiDrawablePtr pDbAsDrawable = OdGiDrawable::cast(pDb);
    ODA_ASSERT(pDb);
    PagesetupToLayout pagesetupToLayout(pDb, nPageIndx);
    //////////////////////////////////////////////////////////////////////////
    OdPdfExportPEPtr pdfExportPE = OdPdfExport::desc()->getX(OdPdfExportPE::desc());
    if (!pdfExportPE.isNull())
      pCtx = pdfExportPE->createGiContext(pDb);
    //////////////////////////////////////////////////////////////////////////
    if (!pCtx.get())
      pCtx = pDbPE->createGiContext(pDb);

    pDbPE->setCurrentLayout(pDb, prms.layouts()[nPageIndx]);
    if (!pdfExportPE.isNull())
      pdfExportPE->evaluateFields(pDb);
    else
      pDbPE->evaluateFields(pDb, 0x04/*OdDbField::kPlot*/);

    if (!isExport2XObj)
    {
      PDFArrayPtr pPages(pPageTree->Find(("Kids")));
      PDFPageDictionaryPtr pCurPage = pPages->getAt(nPageIndx);
      paramsHolder.setCurrentPage(pCurPage);
      paramsHolder.setContentCommands(PDFContentStreamPtr(pCurPage->getContents()->last()));
    }
    OdGsPageParams& pPageParams = prms.pageParams()[nPageIndx];

    OdGsDevicePtr pDevice = PDF2dExportDevice::createObject();
    PDF2dExportDevice* pDevicePdf = (PDF2dExportDevice*)pDevice.get();
    pDevicePdf->eraseAllViews();
    pDevicePdf->setParams(&paramsHolder); //set special context for pdf device
    pDevicePdf->setDbLayoutToPage(pDbLayout2Page);

    pCtx->setPlotGeneration(true);
    pDbPE->loadPlotstyleTableForActiveLayout(pCtx, pDb);

    if (canUseGsCache(prms))
    {
      pCtx->enableGsModel(true);
      pDevicePdf->enableBlockCachSupport(true);
      pDevicePdf->setSupportParallelVect(paramsHolder.getParams().isParallelVectorization());
    }

    if(pCtx->useGsModel())
      pagesetupToLayout.saveGsModel();

    if (nPageIndx > 0)
    {
      if (!prms.layouts()[nPageIndx].compare(prms.layouts()[nPageIndx - 1])) //the same layout as previous
        objectId = pDbPE->getNextViewForActiveLayout(pCtx, objectId);
      else
        objectId = 0;
    }
    if ((NULL == prms.palette()) && (0 == nPageIndx))
    {
      // set palette if it is not init.
      ODCOLORREF Bgr = ODRGBA(ODGETRED(prms.background()), ODGETGREEN(prms.background()), ODGETBLUE(prms.background()), 255);
      pDbPE->setupPalette(pDevicePdf, pCtx, objectId, Bgr);
      int num_color;
      const ODCOLORREF* refColors = pDevicePdf->getLogicalPalette(num_color);
      if (256 != num_color)
        throw OdError(::eBadColorIndex);
      palette_array.insert(palette_array.begin(), refColors, refColors + num_color);
      prms.setPalette(palette_array.asArrayPtr());
   }
    else
    {
      pCtx->setPaletteBackground((ODCOLORREF)(prms.background()));
      pDevicePdf->setLogicalPalette(prms.palette(), 256);
    }
    pDevicePdf->setBackgroundColor(prms.background());

    if (!objectId)
    {
      m_pDevice = pDbPE->setupActiveLayoutViews(pDevicePdf, pCtx);
      objectId = pDbPE->getNextViewForActiveLayout(pCtx, objectId);
    }
    else
    {
      m_pDevice = pDbPE->setupLayoutView(pDevicePdf, pCtx, objectId);
    }

    if (!pdfExportPE.isNull())
      pdfExportPE->filterDrawables(*m_pDevice);
    // !prms.enablePRC will disable rendering of shaded viewports as bitmap
    pDevicePdf->setRenderDevice(prms.getPRCMode() == PDFExportParams::kDisabled);//!m_Params.enablePRC);
    applyLineweightToDcScale(prms);

    OdGsDCRect pageSize(0, pPageParams.getPaperWidth(), 0, pPageParams.getPaperHeight());
    try
    {
      setupPdfLayout(paramsHolder, pPageParams, pCtx, pDevicePdf, objectId);
    }
    catch (OdError&)
    {
      return exCannotOpenOverallVport;
    }

    m_pDevice->update(0);
    if (pDevicePdf->isDWGPaletteNeeded())
    {
      bDwgPaletteNeeded = true;
    }

    if (!isExport2XObj && prms.watermarks().length())
    {
      for (OdUInt16 i = 0; i < prms.watermarks().length(); i++)
        PDFAUX::CreateWatermark(i, pageSize, paramsHolder.document(), paramsHolder.CurrentPage(), prms);
    }
    pDevicePdf->setLwToDcScale(0.);
  }//for(OdUInt32 nPageIndx = 0; nPageIndx < nLayoutCount; ++nPageIndx)

   //put indexed Dwg palette
  if (bDwgPaletteNeeded)
    PDFAUX::createIndexedDWGPalette(pResDict, prms.palette(), 256, paramsHolder.document());

  if (0 != palette_array.size())
    prms.setPalette(NULL);
  return ::exOk;
}

void CPdfExportImplBase::setMeasuringViewport(PdfExportParamsHolder& paramsHolder,
  const OdGsDCRect& rect, const OdGeBoundBlock3d* ext, const OdGsPageParams* pPageParams)
{
  const PDFExportParams& prms = paramsHolder.getParams();
  OdDbBaseDatabasePtr pDb = prms.database();
  OdDbBaseDatabasePEPtr pDbPE(pDb);
  OdGiDrawablePtr pDbDrw = OdGiDrawable::cast(prms.database());

  OdRxObjectPtr pLayoutPtr = OdDbBaseDatabasePEPtr(pDb)->currentLayout(prms.database());
  OdDbBaseLayoutPEPtr pLayout(pLayoutPtr);
  if (!pLayout.isNull() && !pLayoutPtr.isNull())
  {
    double measure_scale = 0.;
    if (GETBIT(prms.exportFlags(), PDFExportParams::kZoomToExtentsMode) && ext && pPageParams)
    {
      OdGsPageParams tmpParams(*pPageParams);
      tmpParams.scale(kMmPerInch / (double)prms.getGeomDPI());
      OdGePoint3d pMin = ext->minPoint();
      OdGePoint3d pMax = ext->maxPoint();
      double m_dRealWorldUnits = 1.;
      double m_dDrawingUnits = odmax(fabs(pMax.x - pMin.x) / OdRoundToLong(tmpParams.getPaperWidth()), fabs(pMax.y - pMin.y) / OdRoundToLong(tmpParams.getPaperHeight()));
      measure_scale = m_dRealWorldUnits / m_dDrawingUnits;
    }
    else
    {
      if (pLayout->useStandardScale(pLayoutPtr))
        pLayout->getStdScale(pLayoutPtr, measure_scale);
      else
      {
        double numerator = 0., denominator = 0.;
        pLayout->getCustomPrintScale(pLayoutPtr, numerator, denominator);
        if (denominator)
          measure_scale = numerator / denominator;
      }
    }
    if (measure_scale)
    {
      double plotUnitPerInch = 25.4;
      if (pLayout->plotPaperUnits(pLayoutPtr) == 0 /*kInches*/)
        plotUnitPerInch = 1.;
      measure_scale = (plotUnitPerInch / 72./*plot unit per dot*/) / measure_scale;

      OdGsDCRect clipBox = rect;
      clipBox.m_min.x *= (72. / (double)prms.getGeomDPI());
      clipBox.m_min.y *= (72. / (double)prms.getGeomDPI());
      clipBox.m_max.x *= (72. / (double)prms.getGeomDPI());
      clipBox.m_max.y *= (72. / (double)prms.getGeomDPI());
      PDFAUX::createMeasuringViewPort(clipBox, paramsHolder.document(), paramsHolder.CurrentPage(), measure_scale);
    }
  }
}

PDFResultEx CPdfExportImplBase::startTransactions(const PDFExportParams &pParams)
{
#ifdef BS_CHANGES_ENABLED
  const bool bSaveGsModel = canUseGsCache(pParams);
#else
  const bool bSaveGsModel = false;
#endif //BS_CHANGES_ENABLED
  OdRxObjectPtrArray paramDBs = pParams.databases();
  for(OdUInt32 i = 0; i < paramDBs.size(); i++)
  {
    if(paramDBs[i].isNull())
      return exNullDatabase;
    m_dbHolder.startDbTransaction(paramDBs[i], bSaveGsModel);
  }
  if(!paramDBs.size())
    m_dbHolder.startDbTransaction(pParams.database(), bSaveGsModel);
  return ::exOk;
}

}
