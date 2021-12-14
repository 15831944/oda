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

// PDFDirect module wrapper (used for isolate same typenames).
#include "OdaCommon.h"
#include "PdfDirectWrapper.h"

#include "MemoryStream.h"
#include "OdPlatformStreamer.h"
#include "RxRasterServices.h"

#include "Gi/GiViewportDraw.h"
#include "Ge/GeExtents2d.h"
#include "Gi/GiPathNode.h"

#include "DbHostAppServices.h"
#include "PdfUnderlayItemImpl.h"

#define PDF_UNDERLAY_GEOMETRY_CACHE
#ifdef PDF_UNDERLAY_GEOMETRY_CACHE
bool cacheUnderlayGeometrySplinesAsPolylines = true;
#else
bool cacheUnderlayGeometrySplinesAsPolylines = false;
#endif

#define PDF_UNDERLAY_CACHE
#ifdef PDF_UNDERLAY_CACHE
bool cacheUnderlay = true;
#else
bool cacheUnderlay = false;
#endif

ODRX_NO_CONS_DEFINE_MEMBERS(PdfDirectWrapper, OdRxObject);
ODRX_NO_CONS_DEFINE_MEMBERS(OdDbPdfUnderlayItemPE, OdRxObject);

OdStaticRxObject<OdDbPdfUnderlayItemPE> PdfDirectWrapper::m_PdfItemPE;

OdGiRasterImagePtr PdfDirectWrapper::convertBmpToRaster(const BITMAPINFOHEADER &bmi, const unsigned char *imagedata)  const
{
  OdStreamBufPtr pFileBuf = OdMemoryStream::createNew();

  OdPlatformStreamer::wrInt16(*pFileBuf, 0x4d42);
  OdUInt32 scanLinesPos = 14 + 40;  // Below version is correct only on Windows
  OdUInt32 size = scanLinesPos + bmi.biWidth*32*Od_abs(bmi.biHeight);

  OdPlatformStreamer::wrInt32(*pFileBuf, size);
  OdPlatformStreamer::wrInt32(*pFileBuf, 0); // reserved
  OdPlatformStreamer::wrInt32(*pFileBuf, scanLinesPos); // offBits
  OdPlatformStreamer::wrInt32(*pFileBuf, bmi.biSize);  // not portable: sizeof(BITMAPINFOHEADER));
  // save BITMAPINFOHEADER
  OdPlatformStreamer::wrInt32(*pFileBuf, bmi.biWidth);
  OdPlatformStreamer::wrInt32(*pFileBuf, bmi.biHeight);
  OdPlatformStreamer::wrInt16(*pFileBuf, bmi.biPlanes);
  OdPlatformStreamer::wrInt16(*pFileBuf, bmi.biBitCount);
  OdPlatformStreamer::wrInt32(*pFileBuf, bmi.biCompression);
  OdPlatformStreamer::wrInt32(*pFileBuf, bmi.biSizeImage);
  OdPlatformStreamer::wrInt32(*pFileBuf, bmi.biXPelsPerMeter);
  OdPlatformStreamer::wrInt32(*pFileBuf, bmi.biYPelsPerMeter);
  OdPlatformStreamer::wrInt32(*pFileBuf, bmi.biClrUsed);
  OdPlatformStreamer::wrInt32(*pFileBuf, bmi.biClrImportant);

  pFileBuf->putBytes(imagedata, bmi.biSizeImage);

  pFileBuf->rewind();

  OdGiRasterImagePtr pTmp;
  OdRxRasterServicesPtr pRasSvcs = odrxDynamicLinker()->loadApp(RX_RASTER_SERVICES_APPNAME);
  if ( !pRasSvcs.isNull() )
  {
    pTmp = pRasSvcs->loadRasterImage(pFileBuf);
  }
  return pTmp;
}

bool PdfDirectWrapper::checkVisibility(double uScale, double vScale, OdGiViewportDraw* pVd, OdGeExtents2d &resUnCS) const
{
  OdGeMatrix3d x = pVd->viewport().getModelToEyeTransform();

  OdGePoint2d vps[2];
  pVd->viewport().getViewportDcCorners(vps[0], vps[1]);  // in EYE

//#define PDF_DEBUG
#ifdef PDF_DEBUG
  OdGeMatrix3d xx = x.inverse();

  {
    double d = (vps[1].x - vps[0].x) / 10. * 2;
    vps[0].x += d;
    vps[1].x -= d;
    d = (vps[1].y - vps[0].y) / 10. * 2;
    vps[0].y += d;
    vps[1].y -= d;

    // draw new viewport
    OdGePoint3d pnts[5];
    pnts[0].set(vps[0].x, vps[0].y, 0);
    pnts[1].set(vps[1].x, vps[0].y, 0);
    pnts[2].set(vps[1].x, vps[1].y, 0);
    pnts[3].set(vps[0].x, vps[1].y, 0);
    pnts[4] = pnts[0];

    pnts[0].transformBy(xx); // now in WCS
    pnts[1].transformBy(xx);
    pnts[2].transformBy(xx); // now in WCS
    pnts[3].transformBy(xx);
    pnts[4].transformBy(xx);

    pVd->subEntityTraits().setColor(0);
    pVd->geometry().polyline(5, pnts);
  }
#endif

  OdGePoint3d origin(OdGePoint3d::kOrigin); // in WCS
  OdGeVector3d u(OdGeVector3d::kXAxis * uScale), v(OdGeVector3d::kYAxis * vScale);

  origin.transformBy(x); // now in EYE
  u.transformBy(x); // now in EYE
  v.transformBy(x); // now in EYE

  origin.z = u.z = v.z = 0;

  /// to underlay CS
  OdGeMatrix2d mx, mmx;
  mmx.setCoordSystem((const OdGePoint2d&)origin, (const OdGeVector2d&)u, (const OdGeVector2d&)v);
  mx = mmx.inverse();

  OdGeExtents2d gpcVXs, gpcVXs2;
  gpcVXs.addPoint(vps[0]);
  gpcVXs.addPoint(vps[1]);

  gpcVXs2.addPoint(OdGePoint2d(origin.x, origin.y));
  gpcVXs2.addPoint(OdGePoint2d(origin.x + u.x + v.x, origin.y + u.y + v.y));

  gpcVXs.intersectWith(gpcVXs2, &resUnCS);
  if (!resUnCS.isValidExtents())
    return false;

  resUnCS.transformBy(mx); // in underlayCS

  OdGeExtents2d uB;
  uB.addPoint(OdGePoint2d(0, 0));
  uB.addPoint(OdGePoint2d(1, 1));
  resUnCS.intersectWith(uB, &resUnCS); // in underlayCS

#ifdef PDF_DEBUG
  {
    OdGePoint3d p[5];
    (OdGePoint2d&)p[0] = resUnCS.minPoint();
    (OdGePoint2d&)p[2] = resUnCS.maxPoint();
    p[1] = p[0]; p[1].y = p[2].y;
    p[3] = p[0]; p[3].x = p[2].x;
    p[4] = p[0];

    ((OdGePoint2d&)p[0]).transformBy(mmx); // in EYE
    ((OdGePoint2d&)p[1]).transformBy(mmx);
    ((OdGePoint2d&)p[2]).transformBy(mmx);
    ((OdGePoint2d&)p[3]).transformBy(mmx);
    ((OdGePoint2d&)p[4]).transformBy(mmx);

    p[0].transformBy(xx); // in WCS
    p[1].transformBy(xx);
    p[2].transformBy(xx);
    p[3].transformBy(xx);
    p[4].transformBy(xx);

    pVd->subEntityTraits().setColor(3);
    pVd->geometry().polyline(5, p);
  }
#endif

  return true;
}

OdGeExtents2d PdfDirectWrapper::extendVisiblePart(const OdGeExtents2d& visiblePartUnCS) const
{
  OdGeVector2d visibleSize = visiblePartUnCS.maxPoint() - visiblePartUnCS.minPoint();

  // we keep a larger image than what is actually displayed.
  // extended area will be about 4 times larger than visible area
  OdGeVector2d extendBy = visibleSize * 0.5;
  OdGeExtents2d extUnCS = visiblePartUnCS;
  extUnCS.expandBy(extendBy);
  extUnCS.expandBy(-extendBy);
  OdGePoint2d minPt(extUnCS.minPoint()), maxPt(extUnCS.maxPoint());

  {
    minPt.x -= 1;
    minPt.y -= 1;
    maxPt.x = (OdUInt32)(maxPt.x + 1.5);
    maxPt.y = (OdUInt32)(maxPt.y + 1.5);
  }

  if (minPt.x < 0.)
    minPt.x = 0.;
  if (minPt.y < 0.)
    minPt.y = 0.;
  extUnCS.set(minPt, maxPt);
  return extUnCS;
}

bool PdfDirectWrapper::ImageCache::isUContextEqual(const OdDbUnderlayDrawContext& drawCtx1, const OdDbUnderlayDrawContext& drawCtx2)
{
  if (drawCtx1.contrast == drawCtx2.contrast &&
    drawCtx1.fade == drawCtx2.fade &&
    drawCtx1.monochrome == drawCtx2.monochrome &&
    drawCtx1.adjustColorForBackground == drawCtx2.adjustColorForBackground)
  {
    OdArray<OdUnderlayLayer>::const_iterator it1 = drawCtx1.layers.begin();
    for (; it1 != drawCtx1.layers.end(); it1++)
    {
      bool isExist = false;
      OdArray<OdUnderlayLayer>::const_iterator it2 = drawCtx2.layers.begin();
      for (; it2 != drawCtx2.layers.end(); it2++)
      {
        if (it1->name() == it2->name() && it1->state() == it2->state())
        {
          isExist = true;
          break;
        }
      }
      if (!isExist)
        return false;
    }
    return true;
  }
  return false;
}

PdfDirectWrapper::ImageCache& PdfDirectWrapper::ImageCacheHolder::getPdfInfo(OdUInt32 vpId, const OdDbObjectId& objId) //const
{
  TD_AUTOLOCK(m_lock);
  if (m_cachedPdfInfoArray.size() <= vpId)
    m_cachedPdfInfoArray.resize(vpId + 1);

  OdImageCacheMap::iterator iter = m_cachedPdfInfoArray[vpId].find(objId);
  if (iter == m_cachedPdfInfoArray[vpId].end())
  {
    ImageCache cache;
    m_cachedPdfInfoArray[vpId][objId] = cache;
    OdDbObjectPtr pObjSrc = objId.safeOpenObject();
    pObjSrc->addReactor(this);
  }
  return m_cachedPdfInfoArray[vpId][objId];
}

void PdfDirectWrapper::ImageCacheHolder::erased(const OdDbObject* pObj, bool bErasing)
{
  TD_AUTOLOCK(m_lock);
  OdDbObjectId objID = pObj->id();
  pObj->removeReactor(this);
  for (OdUInt32 i = 0; i < m_cachedPdfInfoArray.size(); i++)
  {
    OdImageCacheMap::iterator iter = m_cachedPdfInfoArray[i].find(objID);
    if (iter != m_cachedPdfInfoArray[i].end())
      m_cachedPdfInfoArray[i].erase(iter);
  }
}

void PdfDirectWrapper::ImageCacheHolder::goodbye(const OdDbObject* pObject)
{
  erased(pObject);
}

void PdfDirectWrapper::ImageCacheHolder::clear()
{
  TD_AUTOLOCK(m_lock);
  for (OdUInt32 i = 0; i < m_cachedPdfInfoArray.size(); i++)
  {
    for (OdImageCacheMap::iterator iter = m_cachedPdfInfoArray[i].begin(); iter != m_cachedPdfInfoArray[i].end(); iter++)
    {
      try
      {
        iter->first.safeOpenObject()->removeReactor(this);
      }
      catch (...)
      {
      }
    }
    m_cachedPdfInfoArray[i].clear();
  }
  m_cachedPdfInfoArray.clear();
};

OdResult PdfDirectWrapper::getSnapGeometry(OdDbDatabase *pDb, int nPage, OdGiGeometry *pGrabber, const OdArray<OdUnderlayLayer> &layers, const OdGePoint3d* pickPtMc) const
{
  //if (!security(getKey()))
    //return eKeyNotFound;

  if (m_path.isEmpty() || nPage < 0 || nPage > getItemsCount() || !pGrabber)
    return eInvalidInput;

  if (m_geometryCache.m_nPage != nPage)
    createGeometryCache(nPage, layers);

  OdGeExtents2d selectionBoxMc;
  {
    ODA_ASSERT_ONCE(pDb);
    if (pDb && pickPtMc)
    {
      double apertureMc = pDb->appServices()->getAPERTURE() * getMeasureDictInfo(nPage);
      OdGeVector2d ap(apertureMc, apertureMc);
      OdGePoint2d minPtMc = pickPtMc->convert2d() - ap;
      OdGePoint2d maxPtMc = pickPtMc->convert2d() + ap;
      selectionBoxMc.set(minPtMc, maxPtMc);
    }
  }

  bool bFullTrace = pickPtMc == 0;

  for (GeometryCache::Polylines::iterator i = m_geometryCache.m_polylines.begin();
    i != m_geometryCache.m_polylines.end(); ++i)
  {
    if (bFullTrace || !selectionBoxMc.isDisjoint(i->second))
      pGrabber->polyline(i->first.size(), i->first.asArrayPtr());
  }

  for (GeometryCache::Splines::iterator i = m_geometryCache.m_splines.begin();
    i != m_geometryCache.m_splines.end(); ++i)
  {
    if (bFullTrace || !selectionBoxMc.isDisjoint(i->second))
      pGrabber->nurbs(i->first);
  }

  for (GeometryCache::Ellipses::iterator i = m_geometryCache.m_ellipses.begin();
    i != m_geometryCache.m_ellipses.end(); ++i)
  {
    if (bFullTrace || !selectionBoxMc.isDisjoint(i->second))
      pGrabber->ellipArc(i->first);
  }

  // check boundary

  return eOk;
}

OdResult PdfDirectWrapper::optimizeSplines(OdGiViewportDraw* pVd) const
{
  m_geometryCache.m_splines2polylines.clear();
  for (GeometryCache::Splines::iterator i = m_geometryCache.m_splines.begin();
    i != m_geometryCache.m_splines.end(); ++i)
  {
    OdGeNurbCurve3d nurb = i->first;
    OdGeExtents2d ext = i->second;

    double dev = pVd->deviation(kOdGiMaxDevForCurve, nurb.controlPointAt(0));
    if (dev != 0.0) // appendSamplePoints uses strong compare
    {
      OdGeExtents3d ext;
      int nCp = nurb.numControlPoints();
      // OdGeCurve3d::boundBlock should be used here then it will be implemented
      for (int i = 0; i < nCp; i++)
        ext.addPoint(nurb.controlPointAt(i));
      const OdGePoint3d &ptMin = ext.minPoint(), &ptMax = ext.maxPoint();
      OdGeVector3d nrbExt = ptMax - ptMin;
      // This is correct to get deviation here with /ptMin + nrbExt * 0.5/ instead of 0 control point, but
      // sometimes deviations is null, so we could avoid extents calculation for optimization purposes.
      double dMax = OdGreater(nrbExt.x, nrbExt.y) ? nrbExt.x : nrbExt.y;
      if (OdGreater(nrbExt.z, dMax)) dMax = nrbExt.z;
      // Some magic value for keep kOdGiMaxDevForCurve near Acad range
      dev *= 0.007;
      dMax *= 1.0e-5;
      if (OdGreater(dMax, dev))
        dev = dMax;
    }

    OdGePoint3dArray points;
    nurb.appendSamplePoints(nurb.startParam(), nurb.endParam(), dev, points);
    m_geometryCache.m_splines2polylines.push_back(GeometryCache::Polyline(points, ext));
  }
  return eOk;
}

//#define PDF_DEBUG

void PdfDirectWrapper::viewportDraw(int nPage, OdGiViewportDraw* pVd, const OdDbUnderlayDrawContext& context) const
{
  if (nPage >= getItemsCount() || nPage < 0 || m_path.isEmpty())
    return;

  // it returns sizes in PDF dots
  const OdGePoint2d &pCurExtents = getPageSize(nPage);

  double measureScale = getMeasureDictInfo(nPage) * 72.;

  double maxXminX = pCurExtents.x * measureScale;
  double maxYminY = pCurExtents.y * measureScale;

  OdGePoint2d pixelDensity;
  pVd->viewport().getNumPixelsInUnitSquare(OdGePoint3d::kOrigin, pixelDensity);
  //getNumPixelsInUnitSquare is an abstract measure in common case but for pdf underlays in dwg it is always dot per inches
  //if it suddenly not there will be incorrect result
  double ds = pVd->geometry().getModelToWorldTransform().scale();

  double imageWidth = maxXminX;
  double imageHeight = maxYminY;

  // imageWidth, imageHeight are in screen dots
  imageWidth = imageWidth / 72. * pixelDensity.x * ds;
  imageHeight = imageHeight / 72. * pixelDensity.x * ds;

  //double DPI = imageWidth / (pCurExtents.x / 72.) + 0.5;
  //             ||
  //             \/
  // ((pCurExtents.x * measureScale) / 72.) * pixelDensity.x * ds
  //-------------------------------------------------------------      + 0.5
  //                    (pCurExtents.x / 72.)
  //             ||
  //             \/
  double DPI = pixelDensity.x * ds * measureScale + 0.5;

  OdGeExtents2d resUnCS;
  bool bIsVisible;

  double uScale = maxXminX / 72.;
  double vScale = maxYminY / 72.;

  try
  {
    bIsVisible = checkVisibility(uScale, vScale, pVd, resUnCS); //resUnCS is visible part of bitmap (0,0)-(1,1)
  }
  catch (OdError&)
  { // This is occurs when camera looking exactly in plane of underlay - draw nothing. 
    return;
  }

  if (!bIsVisible)
    return;

  // calculate the sizes of resulted bitmap
  int ImagePixW = (int)(imageWidth + 0.5);
  int ImagePixH = (int)(imageHeight + 0.5);

  if (ImagePixW <= 0 || ImagePixH <= 0)
    return;

  // ImagePixW/ImagePixH must be equal to W/H of clipping area
  // Calculate extents of clipping area
  // The w and H of pdf page in 1/72 units
  double dW = maxXminX;
  double dH = maxYminY;

  // calculate the visible area of bitmap (in bitmap pixels)
  // btw : 1 bitmap pixels = 1 screen pixel
  double a = resUnCS.minPoint().x * dW;
  double b = resUnCS.minPoint().y * dH;
  double c = resUnCS.maxPoint().x * dW;
  double d = resUnCS.maxPoint().y * dH;

  double clipRect_a = (OdInt32)(a / measureScale) - 1;
  double clipRect_b = (OdInt32)(b / measureScale) - 1;
  double clipRect_c = (OdInt32)((c / measureScale) + 1.5);
  double clipRect_d = (OdInt32)((d / measureScale) + 1.5);

  if (clipRect_a < 0)
  {
    clipRect_a = 0;
  }
  if (clipRect_b < 0)
  {
    clipRect_b = 0;
  }
  if (clipRect_c > maxXminX / measureScale)
  {
    clipRect_c = (OdUInt32)(maxXminX / measureScale);
  }
  if (clipRect_d > maxYminY / measureScale)
  {
    clipRect_d = (OdUInt32)(maxYminY / measureScale);
  }
  if (clipRect_c - clipRect_a <= 0 || clipRect_d - clipRect_b <= 0.)
    return;

  OdGeExtents2d clipRect(OdGePoint2d(clipRect_a, clipRect_b), OdGePoint2d(clipRect_c, clipRect_d));

  OdDbObjectId obj;
  const OdGiPathNode* curr = pVd->currentGiPath();
  if (curr)
    obj = curr->persistentDrawableId();
  if (obj.isNull())
    return;
  ImageCache& pdfInfo = m_cash.getPdfInfo(pVd->viewport().viewportId(), obj);
  TD_AUTOLOCK(m_cash.m_lock);

  OdGePoint3d clippedOrigin;
  OdGeVector3d u;
  OdGeVector3d v;

  // fade/contract depends on background color
  ODCOLORREF backgroundColor = 0;
  OdGiDefaultContextPtr pCtx = OdGiDefaultContext::cast(pVd->context());
  if (pCtx.get())
    backgroundColor = pCtx->paletteBackground();
  else
    ODA_ASSERT(0);

  if (!pdfInfo.m_ext.isValidExtents() || !pdfInfo.m_ext.contains(clipRect) || (pdfInfo.m_dpi < DPI && DPI <= 3000.) || pdfInfo.m_dpi > 2 * DPI ||
    !pdfInfo.isUContextEqual(pdfInfo.m_drawCtx, context) || pdfInfo.m_nPage != nPage || pVd->context()->isPlotGeneration() ||
    pdfInfo.m_backgroundColor != backgroundColor || !cacheUnderlay) //need update
  {
    if (!pVd->context()->isPlotGeneration() && pdfInfo.m_ext.isValidExtents()/*in first render reserving area is not necessary*/)
    {
      clipRect = extendVisiblePart(clipRect);
      clipRect.intersectWith(OdGeExtents2d(OdGePoint2d(0., 0.), pCurExtents), &clipRect); //crop result if it's bigger than whole pdf
    }

    OdResult res = getImage(pdfInfo, nPage, pVd, context, clipRect, DPI, ImagePixW, ImagePixH);
    if (res != eOk)
      return;

    clippedOrigin = OdGePoint3d::kOrigin + OdGeVector3d::kXAxis * clipRect.minPoint().x / 72. + OdGeVector3d::kYAxis * clipRect.minPoint().y / 72.;
    u = OdGeVector3d((clipRect.maxPoint().x - clipRect.minPoint().x) / 72. / pdfInfo.m_biWidth, 0, 0);
    v = OdGeVector3d(0, (clipRect.maxPoint().y - clipRect.minPoint().y) / 72. / -pdfInfo.m_biHeight, 0);
  }
  else
  {
    clippedOrigin = OdGePoint3d::kOrigin + OdGeVector3d::kXAxis * pdfInfo.m_ext.minPoint().x / 72. + OdGeVector3d::kYAxis * pdfInfo.m_ext.minPoint().y / 72.;
    u = OdGeVector3d((pdfInfo.m_ext.maxPoint().x - pdfInfo.m_ext.minPoint().x) / 72. / pdfInfo.m_biWidth, 0, 0);
    v = OdGeVector3d(0, (pdfInfo.m_ext.maxPoint().y - pdfInfo.m_ext.minPoint().y) / 72. / -pdfInfo.m_biHeight, 0);
  }

  u *= measureScale;
  v *= measureScale;
  clippedOrigin *= measureScale;

  pVd->geometry().rasterImageDc(clippedOrigin, u, v, pdfInfo.m_pImage, 0, 0, true);

#ifdef PDF_DEBUG
  {
    OdRxRasterServicesPtr pRasSvcs = odrxDynamicLinker()->loadApp(RX_RASTER_SERVICES_APPNAME);
    if (!pRasSvcs.isNull())
      pRasSvcs->saveRasterImage(pdfInfo.m_pImage, "f:\\pdf_undrly.bmp");
    OdGePoint3d pnts[2];
    pnts[0] = clippedOrigin;
    pnts[1] = clippedOrigin + u * pdfInfo.m_biWidth - v * pdfInfo.m_biHeight;
    pVd->geometry().polyline(2, pnts);
  }
#endif
}

OdResult PdfDirectWrapper::drawGeometryForSelection(int nPage, OdGiViewportDraw* pVd, const OdDbUnderlayDrawContext& context) const
{
  if (m_path.isEmpty() || nPage < 0 || nPage > getItemsCount() || !pVd)
    return eInvalidInput;
  OdGiGeometry *pGrabber = &pVd->geometry();

  if (m_geometryCache.m_nPage != nPage)
    createGeometryCache(nPage, context.layers);

  if (m_geometryCache.m_splines.size() > 0)
  {
    double dev = pVd->deviation(kOdGiMaxDevForCurve, m_geometryCache.m_splines[0].first.controlPointAt(0));
    if ((m_Deviation == 0 || dev < m_Deviation) && cacheUnderlayGeometrySplinesAsPolylines)
    {
      m_Deviation = dev;
      optimizeSplines(pVd);
    }
  }

  bool selectionGeom = pVd->subEntityTraits().selectionGeom();
  pVd->subEntityTraits().setSelectionGeom(true);

  for (GeometryCache::Polylines::iterator i = m_geometryCache.m_polylines.begin();
    i != m_geometryCache.m_polylines.end(); ++i)
  {
    pGrabber->polyline(i->first.size(), i->first.asArrayPtr());
  }

  for (GeometryCache::Polylines::iterator i = m_geometryCache.m_splines2polylines.begin();
    i != m_geometryCache.m_splines2polylines.end(); ++i)
  {
    pGrabber->polyline(i->first.size(), i->first.asArrayPtr());
  }

  if (!cacheUnderlayGeometrySplinesAsPolylines)
  {
    for (GeometryCache::Splines::iterator i = m_geometryCache.m_splines.begin();
      i != m_geometryCache.m_splines.end(); ++i)
    {
      pGrabber->nurbs(i->first);
    }
  }

  for (GeometryCache::Ellipses::iterator i = m_geometryCache.m_ellipses.begin();
    i != m_geometryCache.m_ellipses.end(); ++i)
  {
    pGrabber->ellipArc(i->first);
  }
  pVd->subEntityTraits().setSelectionGeom(selectionGeom);
  return eOk;
}
