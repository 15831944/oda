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

//////////////////////////////////////////////////////////////////////
// 2dExportDevice.cpp: implementation of the Od2dExportDevice, Od2dExportDevice classes.
//                    These are base classes for 2d export.
//////////////////////////////////////////////////////////////////////

#include "OdaCommon.h"
#include "2dExportDevice.h"
#include "2dSupport.h"

#include "Ge/GeCircArc3d.h"
#include "ColorMapping.h"
#include "Ge/GeEllipArc3d.h"
#include "Ge/GeEllipArc.h"
#include "Ge/GeLineSeg3d.h"
#include "OdFont.h"
#include "Ge/GeScale3d.h"
#include "OdRound.h"

#include "RxDictionary.h"
#include "RxObjectImpl.h"
#include "DbBaseHostAppServices.h"
#include "RxRasterServices.h"
#include "DynamicLinker.h"
#include "Ge/avgpc.h"

#include "../Extensions/ExRender/BmpTilesGen.h"

#define STL_USING_ALGORITHM
#include "OdaSTL.h"

#if defined(ODA_WINDOWS) && !defined(_WINRT) && !defined(_WIN32_WCE)
#include "../../../Extensions/ExRender/win/GdiBmp.h"
#else 
#define drawWithGdiBmp(pDrawable, pVect, origin, u, v, bitmapWidth, bitmapHeight, rect, bMonochrome)
#endif

namespace TD_2D_EXPORT
{

//////////////////////////////////////////////////////////////////////
//  Od2dExportDevice
//////////////////////////////////////////////////////////////////////
//====================================================================
//
Od2dExportDevice::Od2dExportDevice(OdUInt32 t) 
  : m_bUseHLR(true)
  , m_type(t)
{
  ::odgsInitialize();
}

Od2dExportDevice::~Od2dExportDevice()
{
  ::odgsUninitialize();
}

double Od2dExportDevice::renderAreaMultiplier(OdUInt32 width, OdUInt32 height, ShadedViewportExportMode& mode) const
{
  double dMax(width);
  if(height > dMax)
    dMax = height;
  double koef(1.0);
  if(dMax > 1000.0) // needn't create very DPI rasters
    koef = 1000.0 / dMax;
  mode = kOnePiece;
  return koef;
}

OdGsDevicePtr Od2dExportDevice::createRenderDevice(OdGsView* pView)
{
  OdDbBaseDatabasePEPtr pDbPE = OdDbBaseDatabasePE::cast(userGiContext()->database());
  if (pDbPE.isNull())
    return OdGsDevicePtr();
  OdDbBaseHostAppServices *pSvcs = pDbPE->appServices(userGiContext()->database());
  if (!pSvcs)
    return OdGsDevicePtr();
  return pSvcs->gsBitmapDevice(pView, userGiContext()->database(),
                               OdDbBaseHostAppServices::kFor2dExportRender |
                               ((pView->mode() == OdGsView::kHiddenLine && isUseHLR()) ? OdDbBaseHostAppServices::kFor2dExportRenderHLR : 0) |
                               (isUseHLR() ? OdDbBaseHostAppServices::kUseSoftwareHLR : 0));
}

bool Od2dExportDevice::isEnabledToAddRenderDevice(const OdArray<OdGsViewPtr>& views, int indexView) const
{
  ODA_ASSERT(indexView >= 0 && indexView < (int) views.size());
  OdGsView* pView = const_cast<OdGsView*>(views[indexView].get());
  if (pView->isVisible() && pView->mode() >= OdGsView::kHiddenLine) // moved from Od2dExportDevice::setRenderDevice
    return true;
  return false;
}

void Od2dExportDevice::setRenderDevice(OdGsDevice* pRenderDev, bool bEnable)
{
  //create Device for every Viewport that is not going to be drawn on pdf
  m_renderDevices.clear();
  m_arrViewports.clear();
  if (bEnable)
  {
    for (int nView = 0, szView = numViews(); nView < szView; ++nView)
    {
      if (isEnabledToAddRenderDevice(m_views, nView))
      {
        //do not use pdf device for current view
        OdGsClientViewInfo vinfo;
        m_views[nView]->clientViewInfo(vinfo);
        if (m_arrViewports.find(vinfo.viewportObjectId) == m_arrViewports.end())
        {
          RenderDeviceInfo rDev;
          if (pRenderDev)
          {
            if (m_renderDevices.isEmpty())
            {
              rDev.m_pDevice = pRenderDev;
              m_renderDevices.push_back(rDev);
            }
            m_arrViewports[vinfo.viewportObjectId] = 0;
          }
          else
          {
            rDev.m_pDevice = createRenderDevice(m_views[nView]);
            if (!rDev.m_pDevice.isNull())
            {
              m_renderDevices.push_back(rDev);
              m_arrViewports[vinfo.viewportObjectId] = m_renderDevices.size() - 1;
            }
          }
        }
      }
    }
  }
}

inline bool isViewportInsideOutputArea(const OdGsView *pView)
{
  return static_cast<const OdGsViewImpl*>(pView)->isViewportOnScreen();
}

static gpc_polygon getNrcClipAsGpcPolygon(int nrcContours, const int* pNrcCounts, const OdGsDCPointArray &nrcPoints)
{
  gpc_polygon result;
  result.num_contours = 0;
  result.contour = 0;
  result.hole = 0;

  if (nrcContours < 1)
    return result;

  result.num_contours = 1;
  result.hole = ::gpc_alloc<int>(1);
  result.hole[0] = 0;
  result.contour = ::gpc_alloc<gpc_vertex_list>(1);
  result.contour[0].num_vertices = pNrcCounts[0];
  result.contour[0].vertex = ::gpc_alloc<gpc_vertex>(pNrcCounts[0]);
  int i;
  for (i = 0; i < pNrcCounts[0]; i++)
  {
    result.contour[0].vertex[i].x = nrcPoints[i].x;
    result.contour[0].vertex[i].y = nrcPoints[i].y;
  } // NB: this value of i will be used in the next block

  int j;
  for (j = 1; j < nrcContours; j++)
  {
    int zero = 0;

    gpc_polygon next;
    next.num_contours = 1;
    next.hole = &zero;
    gpc_vertex_list vList;
    next.contour = &vList;
    vList.num_vertices = pNrcCounts[j];
    OdGePoint2dArray vertices;
    vertices.resize(vList.num_vertices);
    vList.vertex = (gpc_vertex*)vertices.asArrayPtr();
    int k;
    for (k = 0; k < vList.num_vertices; k++, i++)
    {
      vList.vertex[k].x = nrcPoints[i].x;
      vList.vertex[k].y = nrcPoints[i].y;
    }

    gpc_polygon res;
    gpc_polygon_clip(GPC_XOR, &result, &next, &res);
    gpc_free_polygon(&result);
    result = res;
  }

  return result;
}

static int getGpcPolygonAsNrcClip(const gpc_polygon *poly, OdIntArray &nrcCounts, OdGsDCPointArray &nrcPoints)
{
  nrcCounts.resize((OdUInt32)poly->num_contours);
  int nVerts = 0, i;
  for (i = 0; i < poly->num_contours; i++)
  {
    nVerts += poly->contour[i].num_vertices;
  }
  nrcPoints.resize((OdUInt32)nVerts);
  int *pCounts = nrcCounts.asArrayPtr();
  OdGsDCPoint *pPoints = nrcPoints.asArrayPtr();
  for (i = 0; i < poly->num_contours; i++, pCounts++)
  {
    *pCounts = poly->contour[i].num_vertices;
    for (int j = 0; j < poly->contour[i].num_vertices; j++, pPoints++)
    {
      pPoints->x = OdRoundToLong(poly->contour[i].vertex[j].x);
      pPoints->y = OdRoundToLong(poly->contour[i].vertex[j].y);
    }
  }
  return poly->num_contours;
}

int Od2dExportDevice::unionNrcClips(int nrcContours1, const int* pNrcCounts1, const OdGsDCPointArray &nrcPoints1,
                                    int nrcContours2, const int* pNrcCounts2, const OdGsDCPointArray &nrcPoints2,
                                    OdIntArray &nrcCounts, OdGsDCPointArray &nrcPoints) const
{
  gpc_polygon clip1 = getNrcClipAsGpcPolygon(nrcContours1, pNrcCounts1, nrcPoints1);
  gpc_polygon clip2 = getNrcClipAsGpcPolygon(nrcContours2, pNrcCounts2, nrcPoints2);
  gpc_polygon res;
  gpc_polygon_clip(GPC_UNION, &clip1, &clip2, &res);
  gpc_free_polygon(&clip2);
  gpc_free_polygon(&clip1);
  int nrcContours = getGpcPolygonAsNrcClip(&res, nrcCounts, nrcPoints);
  gpc_free_polygon(&res);
  return nrcContours;
}

void Od2dExportDevice::calculateRenderDeviceClipping(OdGsDevice *pRenderDevice, OdGeExtents3d& ext2d, OdGeExtents3d& extDevice,
                                                     OdIntArray& nrcCounts, OdGsDCPointArray& nrcPoints, int nDevice, bool& bDrawPicture) const
{
  bool bUseClipping = true;
  bDrawPicture = false;

  for (int i = 0; i < pRenderDevice->numViews(); ++i)
  {
    OdGsView* pRenderView = pRenderDevice->viewAt(i);
    const OdGsView* pView = viewAt(i);

    OdGsClientViewInfo vinfo;
    pView->clientViewInfo(vinfo);
    std::map<OdDbStub *, OdUInt32>::const_iterator pRes = m_arrViewports.find(vinfo.viewportObjectId);
    if (pRes != m_arrViewports.end() && pRes->second == nDevice)
    {
      bDrawPicture = true;
      OdGePoint3d ll, ur;
      pRenderView->getViewport((OdGePoint2d&)ll, (OdGePoint2d&)ur);
      OdGeMatrix3d xfm = pRenderView->screenMatrix();
      ll.transformBy(xfm);
      ur.transformBy(xfm);
      ext2d.addPoint(ll);
      ext2d.addPoint(ur);

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
          unionNrcClips(nrcCounts.size(), nrcCounts.asArrayPtr(), nrcPoints,
            nrcCountsNew.size(), nrcCountsNew.asArrayPtr(), nrcPointsNew,
            nrcCounts, nrcPoints);
        }
      }
    }
  }
}

void Od2dExportDevice::exportAsPicture(OdGsDevice *pRenderDevice, const OdGeExtents3d& ext2d, const OdGeExtents3d& extDevice, const OdIntArray& nrcCounts,
                                       const OdGsDCPointArray& nrcPoints, OdUInt32 wholeWidth, OdUInt32 wholeHeight, ShadedViewportExportMode exMode)
{
  // get client screen rect (take into account DC subdividing)
  OdGePoint3d ll, ur;
  ll = ext2d.minPoint();
  ur = ext2d.maxPoint();
  if (ll.x > ur.x)
    std::swap(ll.x, ur.x);
  if (ll.y > ur.y)
    std::swap(ll.y, ur.y);
  if (ll.x < 0.0)
    ll.x = 0.0;
  if (ll.y < 0.0)
    ll.y = 0.0;
  if (ur.x < 0.0)
    ur.x = 0.0;
  if (ur.y < 0.0)
    ur.y = 0.0;
  if (ll.x > wholeWidth)
    ll.x = wholeWidth;
  if (ll.y > wholeHeight)
    ll.y = wholeHeight;
  if (ur.x > wholeWidth)
    ur.x = wholeWidth;
  if (ur.y > wholeHeight)
    ur.y = wholeHeight;

  OdUInt32 pieceHeight = OdRoundToLong(ur.y - ll.y);
  OdUInt32 pieceWidth = OdRoundToLong(ur.x - ll.x);

  OdGePoint3d ll_stored = ll, ur_stored = ur;
  if (pieceHeight > 2 && pieceWidth > 2)
  {
    //set cropRect the same size as old pSweetPiece before
    OdGsDCRect cropRect(OdRoundToLong(ll.x) + 1, OdRoundToLong(ll.x) + 1 + pieceWidth - 2, OdRoundToLong(ll.y) + 2, OdRoundToLong(ll.y) + pieceHeight);

    //OdGiRasterImagePtr pSweetPiece = pWholeImg->crop(OdRoundToLong(ll.x) + 1, OdRoundToLong(wholeHeight - ll.y) - pieceHeight, pieceWidth - 2, pieceHeight - 2);
    // for debug only
    //OdString name;
    //name.format(L"f:\\sweet_piece.bmp");
    //OdRxRasterServicesPtr pRasSvcs = odrxDynamicLinker()->loadApp(RX_RASTER_SERVICES_APPNAME);
    //if (pRasSvcs.get())
    //  pRasSvcs->saveRasterImage(pSweetPiece, name);

    ll = extDevice.minPoint();
    ur = extDevice.maxPoint();

    if (ll.x > ur.x)
      std::swap(ll.x, ur.x);
    if (ll.y > ur.y)
      std::swap(ll.y, ur.y);

    if (ll.x < m_outputRect.m_min.x)
      ll.x = m_outputRect.m_min.x;
    if (ll.y < m_outputRect.m_min.y)
      ll.y = m_outputRect.m_min.y;
    if (ur.x < m_outputRect.m_min.x)
      ur.x = m_outputRect.m_min.x;
    if (ur.y < m_outputRect.m_min.y)
      ur.y = m_outputRect.m_min.y;
    if (ll.x > m_outputRect.m_max.x)
      ll.x = m_outputRect.m_max.x;
    if (ll.y > m_outputRect.m_max.y)
      ll.y = m_outputRect.m_max.y;
    if (ur.x > m_outputRect.m_max.x)
      ur.x = m_outputRect.m_max.x;
    if (ur.y > m_outputRect.m_max.y)
      ur.y = m_outputRect.m_max.y;

    OdGeExtents2d imgExts; // extents of image in DWF device coords
    imgExts.addPoint((OdGePoint2d&)ll);
    imgExts.addPoint((OdGePoint2d&)ur);

    bool bNrcClipped(false);
    if (nrcCounts.size())
    {
      bNrcClipped = true;
      dc_pushClip(nrcCounts.size(), nrcCounts.asArrayPtr(), nrcPoints);
    }
    bool bOutputClip(false);
    {
      // Clip by output rect
      OdGsDCPoint outMinPt(0, wholeHeight);
      OdGsDCPoint outMaxPt(wholeWidth, 0);
      OdGsDCPoint inMinPt(OdRoundToLong(ll.x), OdRoundToLong(ur.y));
      OdGsDCPoint inMaxPt(OdRoundToLong(ur.x), OdRoundToLong(ll.y));
      if (inMinPt.x < outMinPt.x || inMaxPt.x > outMaxPt.x ||
        inMaxPt.y < outMaxPt.y || inMinPt.y > outMinPt.y)
      {
        bOutputClip = true;
        const int nrcCountsOut = 4;
        OdGsDCPointArray nrcPointsOut;
        nrcPointsOut.resize(4);
        nrcPointsOut[0] = m_outputRect.m_min;
        nrcPointsOut[1].x = m_outputRect.m_max.x;
        nrcPointsOut[1].y = m_outputRect.m_min.y;
        nrcPointsOut[2] = m_outputRect.m_max;
        nrcPointsOut[3].x = m_outputRect.m_min.x;
        nrcPointsOut[3].y = m_outputRect.m_max.y;
        dc_pushClip(1, &nrcCountsOut, nrcPointsOut);
      }
    }

    //////////////////////////////////////////////////////////////////////////
    BmpTilesGen _tg(pRenderDevice, OdGsDCRect(0, wholeWidth, wholeHeight, 0));

    //calculate total picture height (not taking slicing into account)
    OdGsDCRect totalRect(0, wholeWidth, 0, wholeHeight);
    totalRect.intersectWith(cropRect);
    double resHeight = totalRect.m_max.y - totalRect.m_min.y;
    double resWidth = totalRect.m_max.x - totalRect.m_min.x;
    OdGeVector2d yVec = OdGeVector2d::kYAxis * (imgExts.maxPoint().y - imgExts.minPoint().y) / resHeight;
    OdGeVector2d xVec = OdGeVector2d::kXAxis * (imgExts.maxPoint().x - imgExts.minPoint().x) / resWidth;

    OdUInt32 startY = totalRect.m_max.y;
    OdGePoint2d newMinPt(imgExts.minPoint());

    OdUInt32 tileW = (exMode == kAsTiles) ? 2048 : resWidth;
    OdUInt32 tileH = (exMode == kAsTiles) ? 2048 : resHeight;
    if (exMode == kAsStrips) //calculate stripe height
    {
      if (tileH * tileW > 20000000)
      {
        while (tileH * tileW > 20000000)
          tileH /= 2;
      }
      else if (tileH * tileW < 10000000)
      {
        while (tileH * tileW < 10000000)
          tileH *= 2;
      }
    }

    OdGePoint2d newMaxPt = newMinPt;
    OdGeExtents2d imgExtsTmp(newMinPt, newMaxPt);

    OdInt32 nextY = startY;
    while (nextY > totalRect.m_min.y)
    {
      nextY -= tileH;
      if (nextY < totalRect.m_min.y)
        nextY = totalRect.m_min.y;

      newMaxPt += yVec * (startY - nextY);
      newMaxPt.y = OdRoundToLong(newMaxPt.y);
      imgExtsTmp.set(imgExtsTmp.minPoint(), newMaxPt);

      OdInt32 startX = totalRect.m_min.x;
      OdInt32 nextX = startX;
      while (nextX < totalRect.m_max.x)
      {
        nextX += tileW;
        if (nextX > totalRect.m_max.x)
          nextX = totalRect.m_max.x;

        OdGiRasterImagePtr pImg;
        OdGsDCRect pictRect(startX, nextX, startY, nextY);
        pImg = _tg.regenTile(pictRect);

        OdUInt32 pxWidth = pImg->pixelWidth();
        newMaxPt += xVec * pxWidth;
        newMinPt += xVec * pxWidth;

        newMaxPt.x = OdRoundToLong(newMaxPt.x);
        newMinPt.x = OdRoundToLong(newMinPt.x);

        imgExtsTmp.set(imgExtsTmp.minPoint(), newMaxPt);

        OdGiRasterImagePtr rast;
        ODCOLORREF backgroundColor = pRenderDevice->getBackgroundColor();
        if (transformImage(pImg, rast, backgroundColor))
          dc_raster_image(rast, imgExtsTmp);
        else
          dc_raster_image(pImg, imgExtsTmp);

        imgExtsTmp.set(newMinPt, newMaxPt);

        startX = nextX;
      }

      newMinPt.x = imgExts.minPoint().x;
      newMaxPt.x = imgExts.minPoint().x;
      newMinPt += yVec * (startY - nextY);
      newMinPt.y = OdRoundToLong(newMinPt.y);
      imgExtsTmp.set(newMinPt, imgExtsTmp.maxPoint());

      startY = nextY;
    }

    if (bOutputClip)
    {
      dc_popClip();
    }
    if (bNrcClipped)
    {
      dc_popClip();
    }
  }
}

//====================================================================
//
bool Od2dExportDevice::viewHasMatchingRenderDevice(const OdGsView* pView, OdUInt32 deviceIdx) const
{
  OdGsClientViewInfo vinfo;
  pView->clientViewInfo(vinfo);
  std::map<OdDbStub *, OdUInt32>::const_iterator pRes = m_arrViewports.find(vinfo.viewportObjectId);
  return (pRes != m_arrViewports.end()) && (pRes->second == deviceIdx) && isViewportInsideOutputArea(pView) &&
    (pView->mode() > OdGsView::kHiddenLine || (pView->mode() == OdGsView::kHiddenLine && !isUseHLR())); //do not render view as bitmap if we able to export it as geometry
}

//====================================================================
//
void Od2dExportDevice::setupViewsVisibility()
{
  // For every created device hide OdGsView's that
  // are not part of Viewport connected to device.
  // If current OdGsView is part of Viewport of any
  // created devices hide this view for pdf Device,
  // because there is no need to draw it twice.
  for(int nDevice = 0; nDevice < numRenderDevices(); ++nDevice)
  {
    OdGsDevice* pRenderDevice = renderLayoutHelperAt(nDevice);
    if(!pRenderDevice)
      pRenderDevice = renderDeviceAt(nDevice);

    // Render device available - should process shaded viewports
    // this device could have (m_pRenderDevice->numViews() + 1) views
    // In this case additional view is paper space view, that must be drawn last
    for(int i = 0; i < pRenderDevice->numViews(); ++i)
    {
      OdGsView* pView = viewAt(i);
      if(viewHasMatchingRenderDevice(pView, nDevice))
      {
        pView->hide();
      }
      else
      {
        OdGsView* pRenderView = pRenderDevice->viewAt(i);
        pRenderView->hide();
      }
    }
  }
}

//====================================================================
//
void Od2dExportDevice::update(OdGsDCRect* pUpdatedRect)
{
  setupViewsVisibility();

  // Draw every device, collect extents and clipping values 
  // of every OdGsView that are part of Viewport connected to device.
  // Crop image by summary extents and clipping.
  for (int nDevice = 0; nDevice < numRenderDevices(); ++nDevice)
  {
    OdGsDevice *pRenderDevice = renderLayoutHelperAt(nDevice);
    if (!pRenderDevice) 
      pRenderDevice = renderDeviceAt(nDevice);

    OdUInt32 wholeWidth = Od_abs((int)(m_outputRect.m_max.x-m_outputRect.m_min.x));
    OdUInt32 wholeHeight = Od_abs((int)(m_outputRect.m_max.y-m_outputRect.m_min.y));
    ShadedViewportExportMode exMode = kOnePiece;
    double koef = renderAreaMultiplier(wholeWidth, wholeHeight, exMode);
    wholeWidth  = OdRoundToLong(wholeWidth*koef);
    wholeHeight = OdRoundToLong(wholeHeight*koef);

    if (exMode == kOnePiece)
    {
      //call OnSize and update to calculate correct clipping
      pRenderDevice->onSize(OdGsDCRect(0, wholeWidth, wholeHeight, 0));
      pRenderDevice->update(0);

      //OdGiRasterImagePtr pWholeImg = pRenderDevice->properties()->getAt(OD_T("RasterImage"));
      //
      // for debug only
      //OdRxRasterServicesPtr pRasSvcs = odrxDynamicLinker()->loadApp(RX_RASTER_SERVICES_APPNAME);
      //if (pRasSvcs.get())
      //  pRasSvcs->saveRasterImage(pWholeImg, L"f:\\whole.bmp");
    }

    OdGeExtents3d ext2d;
    OdGeExtents3d extDevice;
    OdIntArray nrcCounts;
    OdGsDCPointArray nrcPoints;
    bool bDrawPicture;

//     Od2dExportDevice::calculateRenderDeviceClipping(pRenderDevice, ext2d, extDevice, nrcCounts, nrcPoints, nDevice, bDrawPicture);
//     ext2d = OdGeExtents3d();
//     extDevice = OdGeExtents3d();

    calculateRenderDeviceClipping(pRenderDevice, ext2d, extDevice, nrcCounts, nrcPoints, nDevice, bDrawPicture);

    if (bDrawPicture)
    {
      exportAsPicture(pRenderDevice, ext2d, extDevice, nrcCounts, nrcPoints, wholeWidth, wholeHeight, exMode);
    }
  }

  OdGsBaseVectorizeDevice::update(pUpdatedRect); //PDF Device rendering
}

bool Od2dExportDevice::transformImage(OdGiRasterImagePtr pRastIn, OdGiRasterImagePtr& pRastOut, ODCOLORREF backgroundColor) const
{
  return false;
}

//====================================================================
//
//
// getSupportedColorBits()
//
//  Returns the supported number of bits per pixel
//
int Od2dExportDevice::getSupportedColorBits(int nColorBits, bool ) const
{
  int nBitsPerPixel = orgBitonal;

  switch (nColorBits)
  {
  case 1:
    nBitsPerPixel = orgBitonal;
    break;

  case 4:
  case 8:
    nBitsPerPixel = orgMapped;
    break;

  case 16:
  case 24:
    nBitsPerPixel = orgRGB;
    break;

  case 32:
    nBitsPerPixel = orgBGRA;
    break;

  default:
    ODA_ASSERT(false);
    break;
  }
  return nBitsPerPixel;
}



//////////////////////////////////////////////////////////////////////
//  Od2dExportView
//////////////////////////////////////////////////////////////////////

Od2dExportView::Od2dExportView()
{
  m_pMfPlay = OdGiXform::createObject();
  m_pMfPlay->input().addSourceNode(m_mfPlayEntryPoint);
  m_pMetafiler = OdGiMetafiler::createObject();

  m_pViewportClip = OdGiOrthoClipperEx::createObject();
  m_pViewportClip->input().addSourceNode(output());
  m_pViewportClip->input().addSourceNode(m_pMfPlay->output());

  m_pHLRemover = OdGiHLRemover::createObject(); // by default it is disabled
  m_pHLRemover->input().addSourceNode(m_pViewportClip->output());

  m_pXYProjector = OdGiXYProjector::createObject();
  m_pXYProjector->input().addSourceNode(m_pHLRemover->output());

  // here we don't know if it is perspective view, so
  // xform is created in any case
  m_pPerspPrepClip = OdGiOrthoClipperEx::createObject();
  m_pPerspPrepClip->setDeviation(&m_pModelToEyeProc->eyeDeviation());
  m_pPerspXform = OdGiXform::createObject();
  m_pPerspPrepr = OdGiPerspectivePreprocessor::createObject();
  m_pPerspPrepr->setDeviation(&m_pModelToEyeProc->eyeDeviation());
}

void Od2dExportView::setView( const OdGePoint3d & position,
                              const OdGePoint3d& target,
                              const OdGeVector3d& upVector,
                              double fieldWidth,
                              double fieldHeight,
                              Projection projection )
{
  Projection prevProj = view().isPerspective() ? kPerspective : kParallel;
  OdGsBaseVectorizeView::setView(position, target, upVector, fieldWidth, fieldHeight, projection);
  if(projection == kPerspective)
  {
    m_pPerspPrepClip->input().removeSourceNode(output());
    m_pViewportClip->input().removeSourceNode(output());
    m_pViewportClip->input().removeSourceNode(m_pMfPlay->output());
    m_pHLRemover->input().removeSourceNode(m_pViewportClip->output());
    m_pXYProjector->input().removeSourceNode(m_pHLRemover->output());

    m_pPerspPrepClip->input().addSourceNode(output());
    m_pPerspPrepClip->input().addSourceNode(m_pMfPlay->output());
    m_pPerspPrepr->input().removeSourceNode(m_pPerspPrepClip->output());
    m_pPerspPrepr->input().addSourceNode(m_pPerspPrepClip->output());
    m_pPerspXform->input().removeSourceNode(m_pPerspPrepr->output());
    m_pPerspXform->input().addSourceNode(m_pPerspPrepr->output());
    m_pViewportClip->input().removeSourceNode(m_pPerspXform->output());
    m_pViewportClip->input().addSourceNode(m_pPerspXform->output());
    m_pHLRemover->input().addSourceNode(m_pViewportClip->output());
    m_pXYProjector->input().addSourceNode(m_pHLRemover->output());

    m_pViewportClip->setDeviation(&m_pModelToEyeProc->eyeDeviation());
    m_pPerspPrepClip->setDeviation(&m_pModelToEyeProc->eyeDeviation());
  }
  else if (prevProj != projection)
  {
    // #8806 : Previous projection is perspective, so restore conveyor for orthogonal projection
    // Detach perspective conveyor:
    // output()->perspPrepClip->perspPrepr->perspXform->viewportClip->HLRemover->XYproj
    m_pPerspPrepClip->input().removeSourceNode(output());
    m_pPerspPrepClip->input().removeSourceNode(m_pMfPlay->output());
    m_pPerspPrepr->input().removeSourceNode(m_pPerspPrepClip->output());
    m_pPerspXform->input().removeSourceNode(m_pPerspPrepr->output());
    m_pViewportClip->input().removeSourceNode(m_pPerspXform->output());
    m_pHLRemover->input().removeSourceNode(m_pViewportClip->output());
    m_pXYProjector->input().removeSourceNode(m_pHLRemover->output());

    // Attach orthogonal conveyor:
    // output()->viewportClip->HLRemover->XYproj
    m_pViewportClip->input().addSourceNode(output());
    m_pViewportClip->input().addSourceNode(m_pMfPlay->output());
    m_pHLRemover->input().addSourceNode(m_pViewportClip->output());
    m_pXYProjector->input().addSourceNode(m_pHLRemover->output());
  }
}

//////////////////////////////////////////////////////////////////////////
// caching
OdRxObjectPtr Od2dExportView::newGsMetafile()
{
  return OdGiGeometryMetafile::createObject().get();
}

void Od2dExportView::beginMetafile(OdRxObject* pMetafile)
{
  OdGsBaseVectorizer::beginMetafile(pMetafile);
  OdGiGeometryMetafile* pMFer = static_cast<OdGiGeometryMetafile*>(pMetafile);
  pMFer->clear();

  m_pMfPlay->input().removeSourceNode(output());
  m_pMetafiler->input().addSourceNode(output());

  m_pMetafiler->setMetafile(pMFer);
}

void Od2dExportView::endMetafile(OdRxObject* /*pMetafile*/)
{
  m_nMetafileSize = (OdUInt64)(-1);
  if ((m_pMetafiler->metafile() == 0) || m_pMetafiler->metafile()->isEmpty())
  {
    m_nMetafileSize = 0;
  }
  else
  {
    if (!baseDevice()->getUpdateManager().isNull())
    {
      m_nMetafileSize = m_pMetafiler->metafile()->metafileSize();
    }
  }
  m_pMetafiler->setMetafile(0);
  m_pMetafiler->input().removeSourceNode(output());
  m_pMfPlay->input().addSourceNode(output());
}

void Od2dExportView::playMetafile(const OdRxObject* pMetafile)
{
  if (metafilePlayMode() != kMfExtents) // dynamicSubEntHltEnabled
    m_nSelectionMarker = kNullSubentIndex;

  static_cast<const OdGiGeometryMetafile*>(pMetafile)->play(&(m_mfPlayEntryPoint.geometry())/*&m_GeometryDestForPlaying*/, drawContext());
}

void Od2dExportView::pushMetafileTransform(const OdGeMatrix3d& mtx, OdUInt32 uFlags)
{
  if (GETBIT(uFlags, kSharedRefUpdate))
    return;
  m_mfTransformStack.pushMetafileTransform(mtx);
  m_pMfPlay->setTransform(m_mfTransformStack.getCompositeTransform());
}

void Od2dExportView::popMetafileTransform(OdUInt32 uFlags)
{
  if (GETBIT(uFlags, kSharedRefUpdate))
    return;
  m_mfTransformStack.popMetafileTransform();
  m_pMfPlay->setTransform(m_mfTransformStack.getCompositeTransform());
}

void Od2dExportView::setTransformForMetafileGeometry(const OdGeMatrix3d& mtx)
{
  m_mfTransformStack.setOutputTransform(mtx);
  m_pMfPlay->setTransform(mtx);
}

OdGeMatrix3d Od2dExportView::getTransformForMetafileGeometry() const
{
  return m_mfTransformStack.outputTransform();
}
//////////////////////////////////////////////////////////////////////////

void Od2dExportDevice::circleProc2(const OdGePoint3d& center, double radius,
  const OdGeVector3d& normal, const OdGeVector3d& startVector,
  const OdGeVector3d* pExtrusion)
{
  if ((pExtrusion && !pExtrusion->isZeroLength()) || circleArcFillMode())
  {
    OdGiGeometrySimplifier::circleProc2(center, radius, normal, startVector, pExtrusion);
    return;
  }
  return circleProc(center, radius, normal, pExtrusion);
}

void Od2dExportDevice::circleProc(const OdGePoint3d& center,
                                  double radius,
                                  const OdGeVector3d& normal,
                                  const OdGeVector3d* pExtrusion)
{
  if ((pExtrusion && !pExtrusion->isZeroLength()) || circleArcFillMode())
  {
    OdGiGeometrySimplifier::circleProc( center, radius, normal, pExtrusion );
    return;
  }

  if (OdZero(radius))
  {
    // to transform to point at this case
    OdGePoint3d points[2];
    points[1] = points[0] = center;

    OdGeVector3d normals[2];
    normals[1] = normals[0] = normal;

    polylineProc(2, points, normals, pExtrusion, -1);
    return;
  }
  // To transform the entity firstly and then to project onto XYPlane when there is a need.

  OdGeEllipArc3d ellipse;

  OdGeVector3d tmp(OdGeVector3d::kXAxis);    
  OdGeVector3d majorAxis = tmp.transformBy( OdGeMatrix3d::planeToWorld(normal) ).normalize() * radius;
  double radiusRatio = 1.0;
  double startAngle = 0.0;
  double endAngle = Oda2PI;

  double dMajorRadius = majorAxis.length();

  ellipse.set(center, 
              majorAxis / dMajorRadius, 
              normal.crossProduct(majorAxis).normalize(),
              dMajorRadius, dMajorRadius * radiusRatio,
              OdGeEllipArc::paramFromAngle(startAngle, radiusRatio),
              OdGeEllipArc::paramFromAngle(endAngle, radiusRatio)
             );

  if(GETBIT(getDeviceType(), Od2dExportDevice::kSupport2dCircle))
  {
    dc_circle(ellipse);
  }
  else if(GETBIT(getDeviceType(), Od2dExportDevice::kSupport2dEllipse))
  {
    dc_ellipse(ellipse);
  }
  else if(GETBIT(getDeviceType(), Od2dExportDevice::kSupport2dPolyline))
  {
    OdGiGeometrySimplifier::circleProc(center, radius, normal, pExtrusion);
  }
}


//====================================================================
//
void Od2dExportDevice::circleProc(const OdGePoint3d& ptStart,
                              const OdGePoint3d& pt2,
                              const OdGePoint3d& ptEnd,
                              const OdGeVector3d* pExtrusion)
{
  OdGeCircArc3d circArc;
  OdGeError error;
  circArc.set(ptStart, pt2, ptEnd, error);

  if (error != OdGe::kOk)
  {
    OdGiGeometrySimplifier::circleProc(ptStart, pt2, ptEnd, pExtrusion);
  }
  else
  {
    circleProc(circArc.center(), circArc.radius(), circArc.normal(), pExtrusion);
  }
}

void Od2dExportDevice::circularArcProc(const OdGePoint3d& center,
                                       double radius,
                                       const OdGeVector3d& normal,
                                       const OdGeVector3d& startVector,
                                       double sweepAngle,
                                       OdGiArcType arcType,
                                       const OdGeVector3d* pExtrusion)
{
  if ((pExtrusion && !pExtrusion->isZeroLength()) || (arcType != kOdGiArcSimple && circleArcFillMode()))
  {
    OdGiGeometrySimplifier::circularArcProc(center, radius, normal, startVector, sweepAngle, arcType, pExtrusion);
    return;
  }

  if (OdZero(radius))
  {
    OdGePoint3d points[2];
    points[1] = points[0] = center;
    OdGeVector3d normals[2];
    normals[1] = normals[0] = normal;
    polylineProc(2, points, normals, pExtrusion, -1);
  }
  else if (OdZero(sweepAngle))
  {
    OdGePoint3d points[2];
    points[1] = points[0] = (center + startVector * radius);
    OdGeVector3d normals[2];
    normals[1] = normals[0] = normal;
    polylineProc(2, points, normals, pExtrusion, -1);
  }
  else
  {
    OdGeVector3d majorAxis = startVector.normal() * radius;
    double radiusRatio = 1.0;
    double dMajorRadius = majorAxis.length();
    OdGeEllipArc3d ellipse(center, 
                           majorAxis / dMajorRadius, 
                           normal.crossProduct(majorAxis).normalize(),
                           dMajorRadius, dMajorRadius * radiusRatio,
                           0,
                           OdGeEllipArc::paramFromAngle(sweepAngle, radiusRatio));
    putEllipse(ellipse);
  }
}

//====================================================================
//
void Od2dExportDevice::circularArcProc(const OdGePoint3d& start,
                                       const OdGePoint3d& point,
                                       const OdGePoint3d& end,
                                       OdGiArcType arcType,
                                       const OdGeVector3d* pExtrusion)
{
  OdGeCircArc3d circArc;
  OdGeError error;
  circArc.set(start, point, end, error);

  if (error != OdGe::kOk)
  {
    OdGiGeometrySimplifier::circularArcProc(start, point, end, arcType, pExtrusion);
  }
  else
  {
    OdGeVector3d startVec(circArc.refVec().rotateBy(circArc.startAng(), circArc.normal()));
    circularArcProc( circArc.center(), circArc.radius(), circArc.normal(), startVec,
                     circArc.endAng() - circArc.startAng(), arcType, pExtrusion );
  }
}

// AlexR : 3194,3195
bool testCorrectAxis(const OdGeEllipArc3d &ellipArc, OdGeTol dCorrectAxisTol)
{
  OdGeEllipArc3d tmpArc(ellipArc);
  OdGeExtents3d ext1, ext2;
  tmpArc.getGeomExtents(ext1);
  OdGe::ErrorCondition ret;
  tmpArc.orthogonolizeAxes( dCorrectAxisTol, ret );
  if (ret == OdGe::kOk)
  {
    tmpArc.getGeomExtents(ext2);

    double d1, d2;
    double dDiff = ext1.maxPoint().distanceTo(ext1.minPoint()) * 0.1; // 10% differences

    bool b1, b2, b3, b4;
  
    d1 = ext1.minPoint().x;
    d2 = ext2.minPoint().x;
    b1 = OdEqual(d1, d2, dDiff);

    d1 = ext1.minPoint().y;
    d2 = ext2.minPoint().y;
    b2 = OdEqual(d1, d2, dDiff);

    d1 = ext1.maxPoint().x;
    d2 = ext2.maxPoint().x;
    b3 = OdEqual(d1, d2, dDiff);

    d1 = ext1.maxPoint().y;
    d2 = ext2.maxPoint().y;
    b4 = OdEqual(d1, d2, dDiff);

    return (b1 && b2 && b3 && b4);
  }
  return true; // cannot test this way
}

//====================================================================
//
// MKU 28.10.2004   Regular dwf export recovery after after changes in Ge.
//                   It's necessary to call normalize() after getting vector now
//
void Od2dExportDevice::ellipArcProc(const OdGeEllipArc3d& arc,
                                    const OdGePoint3d* pEndPointsOverrides,
                                    OdGiArcType arcType,
                                    const OdGeVector3d* pExtrusion)
{
  if ((pExtrusion && !pExtrusion->isZeroLength()) || (arcType != kOdGiArcSimple && circleArcFillMode()))
  {
    OdGiGeometrySimplifier::ellipArcProc(arc, pEndPointsOverrides, arcType, pExtrusion);
    return;
  }

  OdGeEllipArc3d ellipArc(arc);
  double dRadius = (ellipArc.majorRadius() > ellipArc.minorRadius()) ? ellipArc.majorRadius() : ellipArc.minorRadius();

  if (OdZero(ellipArc.majorRadius()) || OdZero(ellipArc.minorRadius()))
  {
    OdGePoint3d points[2];
    if (OdZero(dRadius)) //degenerated to point
    {
      points[1] = points[0] = ellipArc.center();
    }
    else //degenerated to line
    {
      //we can't just use the extents so we have to do it the other way
      //find sample points (see OdGiGeometrySimplifier::ellipArcProc)
      OdGePoint3dArray arcPoints;
      OdGeInterval interval;
      ellipArc.getInterval(interval);
      ellipArc.appendSamplePoints(interval.lowerBound(), interval.upperBound(), deviation(kOdGiMaxDevForCurve, ellipArc.center()), arcPoints);
      if (pEndPointsOverrides)
      {
        arcPoints.first() = pEndPointsOverrides[0];
        arcPoints.last() = pEndPointsOverrides[1];
      }
      //find min and max points
      OdUInt32 size = arcPoints.size();
      OdGeExtents3d ext;
      for (OdUInt32 i = 0; i < size; i++)
        ext.addPoint(arcPoints[i]);
      if (arcPoints.contains(ext.minPoint()) && arcPoints.contains(ext.maxPoint())) //all right - just put them to device
      {
        points[0] = ext.minPoint();
        points[1] = ext.maxPoint();
      }
      else //arc has other direction
      {
        points[0] = OdGePoint3d(ext.minPoint().x, ext.maxPoint().y, 0.); //z-coord is always 0 for 2d device
        points[1] = OdGePoint3d(ext.maxPoint().x, ext.minPoint().y, 0.);
        ODA_ASSERT(arcPoints.contains(points[0]) && arcPoints.contains(arcPoints[1])); //if there are no these points, then something wrong (but I'm sure them have to be there)
      }
    }

    polylineProc(2, points, NULL /*anyway polylineProc doesn't use normals*/, pExtrusion, -1);
    return;
  }

  OdGe::ErrorCondition ret = OdGe::kOk;
  OdGeVector3d normal;
  try
  {
    normal = ellipArc.normal();
    normal.normalize(OdGeContext::gTol, ret);
  }
  catch (...)
  {
    OdGiGeometrySimplifier::ellipArcProc(arc, pEndPointsOverrides, arcType, pExtrusion);
    return;
  }

  if(ret == OdGe::kOk)
  {
    if(ellipArc.isCircular(/*OdGeTol(1.0e-6)*/))
    {
      OdGeCircArc3d circle( ellipArc.center(),
                            normal,
                            ellipArc.majorAxis(),
                            dRadius,
                            ellipArc.startAng(), 
                            ellipArc.endAng()
                          );

      if (circle.isClosed())
      {
        circleProc( circle.center(), circle.radius(), normal/*circle.normal()*/, pExtrusion );
      }
      else
      {
        circularArcProc( ellipArc.center(), 
                         dRadius, 
                         normal,
                         ellipArc.startPoint() - ellipArc.center(),
                                         ellipArc.endAng() - ellipArc.startAng(), 
                                         arcType, pExtrusion ); 
      }
    }
    else
    {
      OdGeTol dCorrectAxisTol(OdGeContext::gTol);

      if (!testCorrectAxis(ellipArc, dCorrectAxisTol)) // AlexR : test for 3195, 3194, 5123
      {
        OdGiGeometrySimplifier::ellipArcProc( arc, pEndPointsOverrides, arcType, pExtrusion );
        return;
      }

      OdGe::ErrorCondition ret;
      ellipArc.orthogonolizeAxes( dCorrectAxisTol, ret );

      OdGeError flag1, flag2;
      //OdGeVector3d vMajor = ellipArc.majorAxis( OdGeContext::gTol, flag1 );
      //OdGeVector3d vMinor = ellipArc.minorAxis( OdGeContext::gTol, flag2 );
      OdGeVector3d vMajor = ellipArc.majorAxis();
      OdGeVector3d vMinor = ellipArc.minorAxis();
      vMajor.normalize( OdGeContext::gTol, flag1 );
      vMinor.normalize( OdGeContext::gTol, flag2 );

      if ( ret != OdGe::kOk || flag1 != OdGe::kOk || flag2 != OdGe::kOk)
      {
        OdGiGeometrySimplifier::ellipArcProc( arc, pEndPointsOverrides, arcType, pExtrusion );
      }
      else
      {

        OdGe::ErrorCondition ret;
        //OdGeVector3d normalArc = ellipArc.normal(OdGeContext::gTol, ret);     // 1703 // MKU 05.03.2004
        OdGeVector3d normalArc = ellipArc.normal();     // 1703 // MKU 05.03.2004
        normalArc.normalize( OdGeContext::gTol, ret );
        if (ret != OdGe::kOk)
        {
          OdGiGeometrySimplifier::ellipArcProc( arc, pEndPointsOverrides, arcType, pExtrusion );
          return;
        }
        else
        {
          // Everything is correct. Finally outputting the ellipse.
          putEllipse(ellipArc);
        }
      }
    }
  }
  else
  {
    OdGiGeometrySimplifier::ellipArcProc( ellipArc,
                                          pEndPointsOverrides,
                                          arcType,
                                          pExtrusion );
  }
}

//
// putEllipse
//
void Od2dExportDevice::putEllipse(OdGeEllipArc3d& ellipse)
{
  // To transform the entity firstly and then to project onto XYPlane when there is a need.

  // Check Coo system and reverse it if there is a need
  if(ellipse.normal().z < 0)
  {
    ellipse.reverseParam();
  }

  if(GETBIT(getDeviceType(), Od2dExportDevice::kSupport2dEllipse))
  {
    dc_ellipse(ellipse);
  }
  else if(GETBIT(getDeviceType(), Od2dExportDevice::kSupport2dPolyline))
  {
    OdGiGeometrySimplifier::ellipArcProc(ellipse);
  }
}

void Od2dExportDevice::triangleOut(const OdInt32* vertices, const OdGeVector3d* )
{
  const OdGiVertexData* vd = vertexData();
  if (vd && vd->trueColors())
  {
    OdInt32 face[4];
    face[0] = 3;
    for(OdInt32 j = 1; j < 4; j++)
      face[j] = vertices[j-1];
    dc_gouraud_polytriangle(vertexDataList(), 4, face, vd->trueColors());
  }
  else
  {
    OdGePoint2d points2d[3];
    for(OdInt32 j = 0; j < 3; j++)
      points2d[j] = OdGePoint2d(*(OdGePoint2d*)(vertexDataList() + vertices[j]));

    dc_polygon(3, points2d);
  }
}

void Od2dExportDevice::polylineOut(OdInt32 nPoints, const OdGePoint3d* pVertexList)
{
  OdGePoint2dArray points2d;
  points2d.resize(nPoints);

  for(OdInt32 j = 0; j < nPoints; j++)
    points2d[j] = OdGePoint2d(pVertexList[j].x, pVertexList[j].y);
 
  if(nPoints)
    dc_polyline(nPoints, points2d.asArrayPtr());
}

//====================================================================
//
void Od2dExportDevice::polygonProc(OdInt32 nPoints,
                                   const OdGePoint3d* pVertexList,
                                   const OdGeVector3d* pNormal,
                                   const OdGeVector3d* pExtrusion)
{
  if(pExtrusion && !pExtrusion->isZeroLength())
  {
    OdGiGeometrySimplifier::polygonProc(nPoints, pVertexList, NULL, pExtrusion);
    return;
  }

  if(fillMode())
  {
    if (GETBIT(m_effTraits.drawFlags(), OdGiSubEntityTraits::kDrawContourFill) &&
        !GETBIT(getDeviceType(), Od2dExportDevice::kSupportContourFill) && drawContext())
    {
      OdGePoint2dArray points;
      points.resize(nPoints + 1);
      for(OdInt32 j = 0; j < nPoints; j++)
        points[j] = OdGePoint2d(pVertexList[j].x, pVertexList[j].y);
      points[nPoints] = points.first();
      OdGiSubEntityTraits &traits = drawContext()->subEntityTraits();
      OdCmEntityColor prevColor = traits.trueColor();
      traits.setTrueColor(traits.secondaryTrueColor());
      drawContext()->onTraitsModified();
      dc_polygon(nPoints, points.getPtr());
      traits.setTrueColor(prevColor);
      drawContext()->onTraitsModified();
      dc_polyline(nPoints + 1, points.getPtr());
    }
    else
    {
      OdGePoint2dArray points2d;
      points2d.resize(nPoints);

      for(OdInt32 j = 0; j < nPoints; j++)
        points2d[j] = OdGePoint2d(pVertexList[j].x, pVertexList[j].y);

      dc_polygon(nPoints, points2d.asArrayPtr());
    }
  }
  else
  {
    // draw closed polyline
    OdGePoint3dArray points(nPoints + 1);
    points.insert(points.end(), pVertexList, pVertexList + nPoints);
    points.push_back(pVertexList[0]);
    polylineProc(++nPoints, points.getPtr(), pNormal);
  }
}


void Od2dExportDevice::textProc(const OdGePoint3d& position,
                                const OdGeVector3d& u, // normal
                                const OdGeVector3d& v,  // direction
                                const OdChar* msg,
                                OdInt32 length,
                                bool raw,                       // informing whether to interpret escape codes
                                const OdGiTextStyle* pStyle,
                                const OdGeVector3d* pExtrusion)
{
  if(length == 0)  
    return;               // Text must be not empty

  if(!pStyle->isTtfFont())
  {
    // for SHX font
    //
    ODA_ASSERT(pStyle->isShxFont());

    if ( pExtrusion && !pExtrusion->isZeroLength() )
    {
      OdGiGeometrySimplifier::textProc( position, u, v, msg, length, raw, pStyle, pExtrusion );
      return;
    }
    else
    {
      shxText(position, u, v, msg, length, raw, pStyle, pExtrusion);
    }
  }
  else
  {
    if (u.isZeroLength() || v.isZeroLength()) // check via default tolerance // #6094
      return;

    // for True Type Font only

    // Prepare Font
    //
    bool bBold;
    bool bItalic;
    int  nCharset;
    int  nPitchAndFamily;
    pStyle->ttfdescriptor().getTtfFlags(bBold, bItalic, nCharset, nPitchAndFamily);


    OdGeVector2d yDir;
    yDir = (const OdGeVector2d&)v;

    // To set font

    OdTtfDescriptor desc;
    pStyle->getFont()->getDescriptor(desc);
    Od2dFont odFont;
    odFont.m_font_name = desc.typeface();
    if ( odFont.m_font_name.isEmpty() )
      odFont.m_font_name = desc.fileName();
    odFont.m_charset = nCharset;
    odFont.m_pitch   = desc.getPitch();
    odFont.m_family  = desc.getFamily();

    odFont.m_bold = bBold;
    odFont.m_italic = bItalic;
    odFont.m_underline = pStyle->isUnderlined();

                                        // describes how the entire string is rotated from horizontal
    double dRotation = OdGeVector3d::kXAxis.angleTo( u, OdGeVector3d::kZAxis);
    odFont.m_rotation = OdaToDegree(dRotation);
    odFont.m_width_scale = u.length()/v.length(); //pStyle->xScale();
                                        // The desired rotation of the individual characters within the font
    odFont.m_oblique = OdaPI2 - u.angleTo(v, OdGeVector3d::kZAxis);

    odFont.m_height = v.length();

    if (pStyle->isVertical())
    {
      odFont.m_flags |= Od2dFont::VERTICAL;
    }
    if (pStyle->isUpsideDown())
    {
      odFont.m_flags |= Od2dFont::MIRROR_Y;
    }
    if (pStyle->isBackward())
    {
      odFont.m_flags |= Od2dFont::MIRROR_X;
    }
    if (pStyle->isUnderlined())
    {
      odFont.m_flags |= Od2dFont::UNDERSCORE;
    }
    if (pStyle->isOverlined())
    {
      odFont.m_flags |= Od2dFont::OVERSCORE;
    }

    {
      bool bASCII(true);

      bool bAlwaysUnicode = GETBIT(getDeviceType(), Od2dExportDevice::kUnicodeOnly);
      if ( bAlwaysUnicode )
        bASCII = false;

      OdString sProcessedText;
      OdArray<OdUInt16>    arrayUnicode;
      if (!bASCII || !textProcessing( position, u, v, msg, length, raw, pStyle, sProcessedText,
                           /*giEntryPoint, pFontXform,*/ arrayUnicode, /*arrayOverlines, arrayUnderlines,*/
                           true)
                         )
      {
        bASCII = false;
        arrayUnicode.clear();

        textProcessing( position, u, v, msg, length, raw, pStyle, sProcessedText,
                        /*giEntryPoint, pFontXform,*/ arrayUnicode, /*arrayOverlines, arrayUnderlines,*/
                        false);
      }

      Od2dExportView* p2dExpView = static_cast<Od2dExportView*>(viewAt(0));
      p2dExpView->setText( position, u, v, 
               (bASCII) ? sProcessedText.c_str() : msg, 
               (bASCII) ? sProcessedText.getLength() : length, 
               true, pStyle, true, &odFont, 
               (bASCII) ? NULL : &arrayUnicode
             );
    } 
  }
}

bool Od2dExportDevice::textProcessing(const OdGePoint3d& position,
                                      const OdGeVector3d& u,
                                      const OdGeVector3d& v,
                                      const OdChar* msg,
                                      OdInt32 length,
                                      bool raw,
                                      const OdGiTextStyle* pStyle,
                                      OdString& sProcessedText,
                                      OdArray<OdUInt16>& arrayUnicode,
                                      bool bASCII,
                                      OdBoolArray *isInBigFont,
                                      OdGePoint2dArray *pAdvances
                                      )
{
  OdFont* pFont = pStyle->getFont();
  OdFont* pBigFont  = pStyle->getBigFont();

  OdGiConveyorEntryPoint  giEntryPoint;
  OdGiXformPtr            pFontXform;
  pFontXform = OdGiXform::createObject();

  pFontXform->input().addSourceNode( giEntryPoint );
  pFontXform->output().setDestGeometry( *this );

  OdGeMatrix3d charXForm, charXFormBig;
  OdGePoint2d advance;
  charXForm.setCoordSystem(position, u, v, u.crossProduct( v ));
  charXFormBig = charXForm;

  OdGeScale3d scale(1. / pFont->fontAbove());
  charXForm.setToProduct(OdGeMatrix3d::scaling( scale, position), charXForm );

  OdGeScale3d scaleBig;
  if (pBigFont)
  {
    double scBig(1.0 / pBigFont->fontAbove());  // Text size is took into account in u, v vectors
    scaleBig.set(scBig, scBig, scBig);
    charXFormBig.setToProduct(OdGeMatrix3d::scaling(scaleBig, position), charXFormBig);
  }

  OdDbBaseDatabase* pDb = drawContext()->giContext().database();
  OdBaseTextIteratorPtr itText = OdDbBaseDatabasePEPtr(pDb)->createTextIterator( pDb, msg, length, raw, pStyle );
  OdCharacterProperties chProperties;
  OdTextProperties textFlags;
  //bool bOverlined(false);
  //bool bUnderlined(false);

  textFlags.setVerticalText(pStyle->isVertical());
  textFlags.setTrackingPercent(pStyle->trackingPercent());
  textFlags.setIncludePenups(false);

  OdGePoint3d pointsUnder[2];
  OdGePoint3d pointsOver[2];

  bool bUnicode(false);
  for( bool bLast = false; !bLast && !bUnicode; bLast = itText->currProperties().bLastChar )
  {
    OdUInt16 ch = itText->nextChar();
    chProperties = itText->currProperties();
    if ( ch == 0 ) break;

    if (isInBigFont)
    {
      isInBigFont->push_back(chProperties.bInBigFont);
    }

    if (bASCII)
    { 
      // ASCII case
      if ( ch < 128 ) 
      {
        sProcessedText += (char)ch;
      }
      else 
      {
        bUnicode = true;
        break;
      }
    } 
    else 
    { 
      // Unicode case
      arrayUnicode.push_back( ch );
    }

    OdFont* pDrawFont;
    bool bigFont = false;
    if (chProperties.bInBigFont && pBigFont)
    {
      pDrawFont = pBigFont;
      bigFont = true;
      pFontXform->setTransform(charXFormBig);
    }
    else
    {
      pDrawFont = pFont;
      pFontXform->setTransform(charXForm);
    }

    if (!raw)
    {
      textFlags.setUnderlined( chProperties.bUnderlined );
      textFlags.setOverlined( chProperties.bOverlined );

      pDrawFont->getScore( ch, advance, pointsOver, pointsUnder, textFlags );

      if (textFlags.isOverlined()) 
      {
        (&giEntryPoint.geometry())->polylineProc(2, pointsOver);
      }
      if (textFlags.isUnderlined())
      {
        (&giEntryPoint.geometry())->polylineProc(2, pointsUnder);
      }
    }
    else if (pAdvances)
    {
      pDrawFont->getScore( ch, advance, pointsOver, pointsUnder, textFlags );
    }

    if (pAdvances)
    {
      if (bigFont)
        pAdvances->append()->set(charXFormBig.translation().x, charXFormBig.translation().y);
      else
        pAdvances->append()->set(charXForm.translation().x, charXForm.translation().y);
    }

    OdGeVector3d vec = OdGeVector3d(advance.x, advance.y, 0.);
    if (bigFont)
    {
      charXFormBig.setToProduct(charXFormBig, OdGeMatrix3d::translation(OdGeVector3d(advance.x, advance.y, 0.)));
      vec.setToProduct(vec, scaleBig.sx / scale.sx);
      charXForm.setToProduct(charXForm, OdGeMatrix3d::translation(vec));
    }
    else
    {
      charXForm.setToProduct(charXForm, OdGeMatrix3d::translation(OdGeVector3d(advance.x, advance.y, 0.)));
      if (pBigFont)
      {
        vec.setToProduct(vec, scale.sx / scaleBig.sx);
        charXFormBig.setToProduct(charXFormBig, OdGeMatrix3d::translation(vec));
      }
    }
  }

  return !bUnicode;
}

//====================================================================
//
void Od2dExportDevice::xlineProc(const OdGePoint3d& pt1, const OdGePoint3d& pt2)
{
  OdGePoint3d ptsLine[2];

  ptsLine[0] = pt1;
  ptsLine[1] = pt2;
  polylineProc( 2, ptsLine );
}

//====================================================================
//
void Od2dExportDevice::rayProc(const OdGePoint3d& pt1, const OdGePoint3d& pt2)
{
  OdGePoint3d ptsLine[2];

  ptsLine[0] = pt1;
  ptsLine[1] = pt2;
  polylineProc( 2, ptsLine );
}


//====================================================================
// OLE support

static void exOwnerDrawDc(const OdGePoint3d& _origin,
  const OdGeVector3d& _u,
  const OdGeVector3d& _v,
  const OdGiSelfGdiDrawable* pDrawable,
  bool /*bDcAligned*/,
  bool /*bAllowClipping*/,
  OdGsBaseVectorizer& view)
{
  OdGeMatrix3d x = view.getModelToEyeTransform();

  OdGePoint3d origin(x * _origin);
  OdGeVector3d u(x * _u), v(x * _v);

  OdGeExtents3d ext(origin, origin);
  ext.addPoint(origin + u);
  ext.addPoint(origin + v);
  ext.addPoint(origin + u + v);

  OdGePoint3d imgMin = ext.minPoint();
  OdGePoint3d imgMax = ext.maxPoint();

  OdGePoint3d origin2;
  OdGeVector3d u2;
  OdGeVector3d v2;
  origin2 = imgMin;
  v2 = u2 = imgMax - imgMin;
  u2.z = u2.y = 0.;
  v2.z = v2.x = 0.;

  x = view.eyeToOutputTransform();
  origin2.transformBy(x);
  u2.transformBy(x);
  v2.transformBy(x);

  // to increase DPI of output
  //double dCoef = 1.;
  //u2 *= dCoef;
  //v2 *= dCoef;

  OdUInt64 rasterWidth, rasterHeight;
  try
  {
    rasterWidth = OdRoundToLong(fabs(u2.x));
    rasterHeight = OdRoundToLong(fabs(v2.y));
    if (rasterWidth * rasterHeight > 48 * 1024 * 1024) // CreateDibBitmap limitation (huge rasters are scaled anyway)
    {
      double scale = sqrt((((double)(rasterWidth) / 1024.) * ((double)(rasterHeight) / 1024.)) / 48.);
      if (OdLessOrEqual(scale, 1.))
        scale = 1.1;
      while (rasterWidth * rasterHeight > 48 * 1024 * 1024)
      {
        rasterWidth = OdRoundToLong((double)rasterWidth / scale);
        rasterHeight = OdRoundToLong((double)rasterHeight / scale);
      }
    }
  }
  catch (const OdError&)
  {
    // #10225 : Can't render if arithmetic overflow thrown
    return;
  }

  //u2 /= dCoef;
  //v2 /= dCoef;

  OdGsDCRect rect; // RECT 
  rect.m_min.x = 0; // left
  rect.m_min.y = rasterHeight; // top
  rect.m_max.x = rasterWidth; // right
  rect.m_max.y = 0; // bottom 

  OdSmartPtr<OdGiSelfGiDrawablePE> pOdGiSelfGiDrawablePE = OdGiSelfGiDrawablePE::cast(pDrawable);
  if (pOdGiSelfGiDrawablePE.get()
    && pOdGiSelfGiDrawablePE->draw(const_cast<OdGiSelfGdiDrawable*>(pDrawable),
      &view.output().destGeometry(), origin2, u2, v2, rect, true))
    return;
  drawWithGdiBmp(pDrawable, &view, origin2, u2, v2, rasterWidth, rasterHeight, rect, false);
}

void Od2dExportView::ownerDrawDc(
  const OdGePoint3d& origin,
  const OdGeVector3d& u,
  const OdGeVector3d& v,
  const OdGiSelfGdiDrawable* pDrawable,
  bool bDcAligned,
  bool bAllowClipping)
{
  if (mode() != OdGsView::k2DOptimized)
    return;
  exOwnerDrawDc(origin, u, v, pDrawable, bDcAligned, bAllowClipping, *this);
}

//====================================================================
//
void Od2dExportView::onTraitsModified()
{
  OdGsBaseVectorizer::onTraitsModified();
  device()->onTraitsModified(effectiveTraits());
}

void Od2dExportDevice::onTraitsModified(const OdGiSubEntityTraitsData& traits)
{
  m_effTraits = traits;
}


//====================================================================
//
bool Od2dExportView::regenAbort() const
{
  return OdGsBaseVectorizer::regenAbort();
  // return true here to abort the rendering process
  //return false;  
}

//====================================================================
//
void Od2dExportView::beginViewVectorization()
{
  OdGsBaseVectorizer::beginViewVectorization();

  //const bool bDisplaying = GETBIT(vectorizingFlags(), kDisplaying);
  //if (bDisplaying)
  {
    device()->setDrawContext(drawContext());
    device()->setupSimplifier(&m_pModelToEyeProc->eyeDeviation());
  }
  setDrawContextFlags(kSpatialFilterSimplText | kEmbranchmentSimplText, false);

  m_pMetafiler->setDrawContext(drawContext());
  m_pMfPlay->input().removeSourceNode(output());
  if (isModelCacheEnabled())
  {
    m_pMetafiler->setDeviation(&m_pModelToEyeProc->worldDeviation());
    m_pMetafiler->setCoordinatesType(OdGiMetafiler::kWorld);

    OdGeMatrix3d mat = eyeToScreenMatrix();

    setEyeToOutputTransform(mat/*getEyeToWorldTransform()*/);
    m_pMfPlay->input().addSourceNode(output());
    m_pMfPlay->setTransform(mat*getWorldToEyeTransform());
    m_mfTransformStack.setOutputTransform(mat*getWorldToEyeTransform());
  }
}

//====================================================================
//
void Od2dExportView::endViewVectorization()
{
  OdGsBaseVectorizer::endViewVectorization();

  //const bool bDisplaying = GETBIT(vectorizingFlags(), kDisplaying);

#if !defined(_WIN32_WCE) || (_WIN32_WCE >= 0x500)
  if (!m_pHLRemover.isNull() && isVisible() && !std::uncaught_exception())
#else
  if (!m_pHLRemover.isNull() && isVisible())
#endif
    m_pHLRemover->process(); // compute hidden lines in case HLR enabled

  //if (bDisplaying)
    device()->setDrawContext(0);

  m_mfTransformStack.setOutputTransform(OdGeMatrix3d::kIdentity);
  m_mfTransformStack.checkMetafileTransform();
}

void Od2dExportView::loadViewport()
{
  OdGsDCPoint min, max;
  screenRect(min, max);
  OdGeMatrix3d xToDc = eyeToScreenMatrix();

  m_pHLRemover->setDrawContext(OdGsBaseVectorizer::drawContext());

  if(mode() == kHiddenLine && device()->isUseHLR())
  {
    m_pHLRemover->enable(true);
    m_pHLRemover->invertZ(true);
  }
  else
  {
    // only k2DOptimized mode supports plot style tables
    m_renderMode = k2DOptimized/*kWireframe*/;
    m_regenerationType = kOdGiStandardDisplay;
    m_pHLRemover->enable(false);
  }

  const OdGePoint2d pts[4] = { OdGePoint2d(min.x, min.y), 
    OdGePoint2d((double)max.x, (double)min.y),
    OdGePoint2d(max.x, max.y),
    OdGePoint2d((double)min.x, (double)max.y) };
  const int ptsCounts = 4;

  if(!view().isPerspective())
  {
    OdGeMatrix3d xToDc = eyeToScreenMatrix();

    setEyeToOutputTransform(xToDc);

    OdGeVector3d zAxis = OdGeVector3d::kZAxis;
    OdGeVector3d dcFront(0.,0.,frontClip());
    OdGeVector3d dcBack(0.,0.,backClip());
    dcFront.transformBy(xToDc);
    dcBack.transformBy(xToDc);
    zAxis.transformBy(xToDc);

    if(zAxis.z > 0.)
    {
      m_pViewportClip->set(ptsCounts, pts, isBackClipped(), dcBack.z, isFrontClipped(), dcFront.z);
    }
    else
    {
      m_pViewportClip->set(ptsCounts, pts, isFrontClipped(), dcFront.z, isBackClipped(), dcBack.z);
    }
    m_pXYProjector->enable();
  }
  else
  {
    if (isModelCacheEnabled())
    {
      m_pMfPlay->setTransform(getModelToEyeTransform());
      m_mfTransformStack.setOutputTransform(getModelToEyeTransform());
    }

    OdGeMatrix3d xToDc = screenMatrix() * projectionMatrix();
    setEyeToOutputTransform(OdGeMatrix3d::kIdentity);

    bool isFrontClp(isFrontClipped());
    bool isBackClp(isBackClipped());

    double frClp(frontClip());
    double bkClp(backClip());

    if(isFrontClp)
      frClp = odmin(frClp, focalLength() * 0.9);
    else
      frClp = focalLength() * 0.9;

    m_pPerspPrepClip->set(0, 0, isBackClp, bkClp, true, frClp);
    m_pViewportClip->pushClipStage(1, &ptsCounts, 4, pts);

    m_pXYProjector->enable();
    m_pPerspPrepClip->setDrawContext(OdGiBaseVectorizer::drawContext());

    m_pPerspPrepr->setDrawContext(OdGiBaseVectorizer::drawContext());
    m_pXYProjector->output().setDestGeometry(*device());
    m_pPerspXform->setTransform(xToDc);  
  }
  m_pPerspPrepClip->setDeviation(&m_pModelToEyeProc->eyeDeviation());

  m_pViewportClip->setDrawContext(OdGiBaseVectorizer::drawContext());
  m_pViewportClip->setDeviation(&m_pModelToEyeProc->eyeDeviation());
}

void Od2dExportView::update()
{
  if(abs((int)(dcScreenMax().x-dcScreenMin().x)) > 0 && abs((int)(dcScreenMax().y-dcScreenMin().y)) > 0)
  {
    OdGsBaseVectorizeView::update();
  }
}

//====================================================================
//
bool Od2dExportView::isTransparency(OdGeVector3d screenU, OdGeVector3d screenV)
{
  return ((screenU.isParallelTo(OdGeVector3d::kXAxis) 
           && screenV.isParallelTo(OdGeVector3d::kYAxis))
                        ? false
                        : true
         );
}

// Default implementations
void Od2dExportDevice::dc_polyline(OdUInt32, const OdGePoint2d*)
{
  throw OdError(eNotImplemented); // If called, then must be implemented in child class
}

void Od2dExportDevice::dc_polygon(OdUInt32, const OdGePoint2d*)
{
  throw OdError(eNotImplemented); // If called, then must be implemented in child class
}

void Od2dExportDevice::dc_circle(const OdGeEllipArc3d&)
{
  throw OdError(eNotImplemented); // If called, then must be implemented in child class
}

void Od2dExportDevice::dc_ellipse(const OdGeEllipArc3d&)
{
  throw OdError(eNotImplemented); // If called, then must be implemented in child class
}

void Od2dExportDevice::dc_gouraud_polytriangle(const OdGePoint3d*,
                                               OdInt32,
                                               const OdInt32*,
                                               const OdCmEntityColor*)
{
  throw OdError(eNotImplemented); // If called, then must be implemented in child class
}

void Od2dExportDevice::dc_raster_image(OdGiRasterImagePtr, const OdGeExtents2d&)
{
  throw OdError(eNotImplemented); // If called, then must be implemented in child class
}

void Od2dExportDevice::dc_color(ODCOLORREF)
{
  throw OdError(eNotImplemented); // If called, then must be implemented in child class
}

void Od2dExportDevice::dc_layer(int, const OdString&)
{
  throw OdError(eNotImplemented); // If called, then must be implemented in child class
}

void Od2dExportDevice::dc_pushClip(int, const int*, const OdGsDCPointArray&)
{
  throw OdError(eNotImplemented); // If called, then must be implemented in child class
}

void Od2dExportDevice::dc_popClip()
{
  throw OdError(eNotImplemented); // If called, then must be implemented in child class
}

} // TD_2D_EXPORT namespace
