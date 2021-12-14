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

#include "OdaCommon.h"
#include "DbGeoMapPEImpl.h"
#include "OdBingMaps.h"
#include "OdBingMapsCache.h"
#include "DbGeoMapFieldsPE.h"

#include "RxRasterServices.h"
#include "DbGeoData.h"
#include "OdDbGeoCoordinateSystem.h"

#include "DbViewportTable.h"
#include "DbViewportTableRecord.h"

#include "DbDictionaryVar.h"
#include "Ge/GeRay3d.h"
#include "OdStack.h"

#include "GiContextForDbDatabase.h"
#include "DbGsManager.h"
#include "DbViewport.h"
#include "ColorMapping.h"

#include "Gi/GiClipBoundary.h"
#include "Ge/GeLineSeg2d.h"
#include "Gi/GiViewportDraw.h"
#include "Gi/GiTextStyle.h"

bool checkPointInsidePolygon(const OdGePoint2d& ptInput, const OdGePoint2dArray& arrPt);
void getChunkVertices(int nLOD, OdInt32 nTileX, OdInt32 nTileY, const OdDbGeoData* pGeoData, OdGePoint3dArray& arrChunkVertices);
void getChunkVertices2d(int nLOD, OdInt32 nTileX, OdInt32 nTileY, const OdDbGeoData* pGeoData, OdGePoint2dArray& arrChunkVertices2d);
void addTileToStack(OdStack<OdDbGeoMapTile>& stackTiles, char cMapTypeId, int nLOD, OdInt32 nTileX, OdInt32 nTileY, const OdDbGeoData* pGeoData, const OdGePoint2dArray& arrMapExtents, const OdGePoint2dArray& arrGlobalMapExtents, const std::set<OdDbGeoMapTile>& arrTiles);
OdResult getGeoMapTiles(std::set<OdDbGeoMapTile>& arrTiles, const OdDbGeoData* pGeoData, OdGeoMapType eGeoMapType, OdInt8 nLOD, const OdGePoint2dArray& arrMapExtents, const OdGePoint2dArray& arrGlobalMapExtents);
OdResult getGlobalMapExtents(const OdDbGeoData* pGeoData, OdGePoint2dArray& arrMapExtents);
OdGeoMapType getGeoMapType(OdDbObjectId viewportId);
OdResult getGeoData(OdDbDatabase* pDb, OdDbGeoDataPtr& pGeoData);
void drawTiles(OdGiViewportDraw* pVd, const OdDbGeoData* pGeoData, const OdGePoint2dArray& arrGlobalMapExtents, const std::set<OdDbGeoMapTile>& arrTiles, const OdArray<OdGiRasterImagePtr>& arrTilesImages);

//TODO: these 2 functions are copy-paste form DbGeoMap.cpp:
bool loadFontForGeoMap2(const OdString& sFont, double dTextHeight, OdDbDatabase* pDb, OdGiTextStyle& textStyle);
void drawProvidersStrings2(OdGiViewportDraw* pVd, OdDbDatabase* pDb, const OdStringArray& arrCopyrightStrings, const OdGePoint3dArray& arrVertices, const OdGePoint3d& ptTextPosition, double dTextHeight, const OdCmColor& textColor);

ODRX_CONS_DEFINE_MEMBERS(OdDbGeoDataChunksPEImpl, OdDbGeoDataChunksPE, RXIMPL_CONSTR);

OdDbGeoDataChunksPEImpl::~OdDbGeoDataChunksPEImpl()
{
}

bool loadFontForGeoMap2(const OdString& sFont, double dTextHeight, OdDbDatabase* pDb, OdGiTextStyle& textStyle)
{
  textStyle.setFont(sFont, false, false, 1, 0);
  textStyle.setBigFontFileName(OD_T(""));
  textStyle.setTextSize(dTextHeight);
  textStyle.setObliquingAngle(0.);
  textStyle.setXScale(1.);
  textStyle.setTrackingPercent(1.);
  textStyle.setVertical(false);
  textStyle.setOverlined(false);
  textStyle.setUnderlined(false);
  textStyle.setStriked(false);
  textStyle.setBackward(false);
  textStyle.setUpsideDown(false);
  textStyle.loadStyleRec(pDb);
  // arx check return value of loadStyleRec, we can't...
  // therefore compare result with default simplex.shx (default == our font not loaded):
  OdFontPtr pFont = textStyle.getFont();
  return pFont->getFileName().iCompare("simplex.shx");
}

void drawProvidersStrings2(OdGiViewportDraw* pVd, OdDbDatabase* pDb, const OdStringArray& arrCopyrightStrings, const OdGePoint3dArray& arrVertices, const OdGePoint3d& ptTextPosition, double dTextHeight, const OdCmColor& textColor)
{
  //attributionStringInSingleLine:
  OdString strText;
  for (OdUInt32 i = 0; i < arrCopyrightStrings.size(); ++i)
  {
    strText += arrCopyrightStrings[i];
    strText += L' '; // Last (terminating) space is required because of right alignment
  }

  if (!strText.isEmpty() && arrVertices.size() == 4)
  {
    // PLEASE NOTE: arx has extents() function in thier GiTextStyle.
    // Here we are using pVd->context()->textExtentsBox() hoping for simular result.

    OdGeVector3d vU = (arrVertices[1] - arrVertices[0]).normalize();
    OdGeVector3d vV = (arrVertices[2] - arrVertices[1]).normalize();
    OdGeLine3d line1(arrVertices[0], vV);
    OdGeLine3d line2(ptTextPosition, -vU);

    OdGePoint3d ptInt;
    OdGeTol gTolerance = 1.e-6; //in arx it's gTol, but we have changed it #CORE-15623
    if (line2.intersectWith(line1, ptInt, gTolerance))
    {
      OdGeVector3d vPlacing = ptTextPosition - ptInt;
      double dPlacingLength = vPlacing.length();
      if (!OdZero(dPlacingLength, gTolerance.equalPoint()) && vPlacing.isCodirectionalTo(vU))
      {
        //check dTextHeight:
        double dTextHeightCorrected = dTextHeight;
        if (OdZero(dTextHeightCorrected, gTolerance.equalPoint()))
        {
          //ODA_ASSERT(!"GeoMap TextHeight is near ZERO!");
          dTextHeightCorrected = (arrVertices[2] - arrVertices[1]).length() * 0.01;
        }

        //apply textStyle:
        OdGiTextStyle textStyle;
        OdString sSegoeUI(OD_T("Segoe UI"));
        if (!loadFontForGeoMap2(sSegoeUI, dTextHeightCorrected, pDb, textStyle))
        {
          OdString sTahoma(OD_T("Tahoma"));
          if (!loadFontForGeoMap2(sTahoma, dTextHeightCorrected, pDb, textStyle))
          {
            OdDbHostAppServices* pSvc = pDb->appServices();
            if (!loadFontForGeoMap2(pSvc->getAlternateFontName(), dTextHeightCorrected, pDb, textStyle))
            {
              return;
            }
          }
        }

        OdGePoint3d min, max;
        pVd->context()->textExtentsBox(textStyle, strText, -1, 0, min, max);
        double dTextWidth = max.x - min.x;
        if (!OdZero(dTextWidth, gTolerance.equalPoint()))
        {
          OdString sRes = strText;
          if (dTextWidth > dPlacingLength) //modify result string?
          {
            OdInt32 uFullTextLength = strText.getLength();
            sRes = strText.right(dPlacingLength / dTextWidth * uFullTextLength);

            //check if we need to remove more symbols:
            while (sRes.getLength())
            {
              pVd->context()->textExtentsBox(textStyle, sRes, -1, 0, min, max);
              if (max.x - min.x <= dPlacingLength)
              {
                break;
              }
              sRes = sRes.right(sRes.getLength() - 1);
            }

            //or check if we have removed more than needed:
            while (sRes.getLength() < uFullTextLength)
            {
              OdString sBuf = strText.right(sRes.getLength() + 1);
              OdGePoint3d minBuf, maxBuf;
              pVd->context()->textExtentsBox(textStyle, sBuf, -1, 0, minBuf, maxBuf);
              if (maxBuf.x - minBuf.x > dPlacingLength)
              {
                break;
              }
              min = minBuf;
              max = maxBuf;
              sRes = sBuf;
            }

            if (sRes.isEmpty())
            {
              return;
            }
          }

          OdGePoint3d ptResultTextPos = ptTextPosition;
          ptResultTextPos.z += 0.001;

          ptResultTextPos -= (max.x - min.x) * vU;
          ptResultTextPos -= (max.y + min.y) * vV; //yeah, "+", not sure...

          OdCmEntityColor oldColor = pVd->subEntityTraits().trueColor();
          pVd->subEntityTraits().setTrueColor(textColor.entityColor());
          pVd->geometry().text(ptResultTextPos, vU.crossProduct(vV).normal(), vU, sRes, -1, false, &textStyle);
          pVd->subEntityTraits().setTrueColor(oldColor);
        }
      }
    }
  }
}

//this class was taken from DbGeoMap.cpp
class OdGiRasterImageRGBA32 : public OdGiRasterImageBGRA32
{
public:
  OdGiRasterImage::PixelFormatInfo pixelFormat() const
  {
    OdGiRasterImage::PixelFormatInfo pf;
    pf.setRGBA();
    return pf;
  }

  static OdGiRasterImagePtr createObject(OdGiImageBGRA32 *pImage, OdGiRasterImage::TransparencyMode transparencyMode = OdGiRasterImage::kTransparency8Bit)
  {
    ODA_ASSERT(pImage);
    OdSmartPtr<OdGiRasterImageRGBA32> pIw = OdRxObjectImpl<OdGiRasterImageRGBA32>::createObject();
    pIw->m_pBGRAImage = pImage;
    pIw->m_transparencyMode = transparencyMode;
    return pIw;
  }
};


//////////////////////////////
// OdDbGeoMapHelper
//////////////////////////////

OdResult OdDbGeoMapHelper::getMap(const OdDbGeoMap * pGeoMap, OdBinaryData & imgPixelData)
{
  OdDbDatabasePtr pHostDb = pGeoMap->database();
  if (pHostDb.isNull())
  {
    return eWrongDatabase;
  }

  OdDbDatabasePtr pDb = pHostDb->appServices()->createDatabase();
  if (pDb.isNull())
  {
    return eCreateFailed;
  }

  OdGsDevicePtr pDevice;
  OdGsModulePtr pGs = ::odrxDynamicLinker()->loadModule(OdWinBitmapModuleName);
  if (!pGs.isNull())
  {
    pDevice = pGs->createBitmapDevice();
  }
  if (pDevice.isNull())
  {
    return eDeviceNotFound;
  }

  OdGiContextForDbDatabasePtr pDwgContext = OdGiContextForDbDatabase::createObject();
  pDwgContext->setDatabase(pDb);
  pDwgContext->enableGsModel(false);
  pDevice = OdDbGsManager::setupActiveLayoutViews(pDevice, pDwgContext);

  pDevice->setLogicalPalette(::odcmAcadDarkPalette(), 256);
  pDevice->setBackgroundColor(ODRGB(255, 255, 255));
  pDwgContext->setPaletteBackground(ODRGB(255, 255, 255));

  OdGsDCRect screenRect(OdGsDCPoint(0, pGeoMap->imageSize().y), OdGsDCPoint(pGeoMap->imageSize().x, 0));
  pDevice->onSize(screenRect);
  OdGsView* pView = pDevice->viewAt(0);
  
  double dWidth = pGeoMap->imageWidth();
  double dHeight = pGeoMap->imageHeight();

  OdGePoint3dArray arrVertices;
  pGeoMap->getVertices(arrVertices);
  OdGePoint3d ptCenter = OdGeExtents3d().addPoints(arrVertices).center();

  pView->setView(
    ptCenter + OdGeVector3d::kZAxis,
    ptCenter,
    OdGeVector3d::kYAxis,
    dWidth,
    dHeight);

  pView->setMode(OdGsView::k2DOptimized);

  OdDbGeoMapImageCreatorPtr pGeoMapImageCreator = OdDbGeoMapImageCreator::createObject();
  pGeoMapImageCreator->setGeoMapId(pGeoMap->id());

  pView->add(pGeoMapImageCreator, NULL);
  pDevice->update();

  OdGiRasterImagePtr pImage = OdGiRasterImage::cast(pDevice->properties()->getAt(OD_T("RasterImage")));
  OdSmartPtr<OdGiRasterImageDesc> pDesc = OdGiRasterImageDesc::createObject(pImage);
  pDesc->setColorDepth(32);
  pDesc->pixelFormat().setRGBA();
  pDesc->setScanLinesAlignment(4);
  OdGiRasterImagePtr pRaster = pImage->convert(true, 50., 50., 0.0, 0, false, true, false, pDesc, true);
  const OdUInt8* pData = pRaster->scanLines();
  imgPixelData.assign(pData, pData + 4 * pRaster->pixelWidth() * pRaster->pixelHeight());

  return eOk;
}

bool OdDbGeoMapHelper::normalizeSize(double dWidth, double dHeight, OdUInt32& nWidth, OdUInt32& nHeight)
{
  if (OdZero(dWidth, 1.e-6) || OdZero(dHeight, 1.e-6))
  {
    return false;
  }

  if (dWidth < 1. || dHeight < 1.)
  {
    double dScale = dWidth <= dHeight ? 200. / dWidth : 200. / dHeight;
    nWidth = dWidth * dScale;
    nHeight = dHeight * dScale;
  }

  nWidth = dWidth;
  nHeight = dHeight;
  return nWidth && nHeight;
}

OdResult OdDbGeoMapHelper::getImageScale(const OdDbGeoData * pGeoData, OdInt8 LOD, double& dScale)
{
  dScale = 0.1;
  
  OdGePoint2d ptLatLonMin, ptLatLonMax;
  OdBingMaps::PixelXYToLatLong(0, 0, LOD, ptLatLonMin.x, ptLatLonMin.y);
  OdBingMaps::PixelXYToLatLong(OdBingMaps::TILE_SIZE, OdBingMaps::TILE_SIZE, LOD, ptLatLonMax.x, ptLatLonMax.y);

  OdGeExtents3d extents;
  OdGePoint3d ptRes;
  pGeoData->transformFromLonLatAlt(OdGePoint3d(ptLatLonMin.y, ptLatLonMin.x, 0.), ptRes);
  extents.addPoint(ptRes);
  pGeoData->transformFromLonLatAlt(OdGePoint3d(ptLatLonMin.y, ptLatLonMax.x, 0.), ptRes);
  extents.addPoint(ptRes);
  pGeoData->transformFromLonLatAlt(OdGePoint3d(ptLatLonMax.y, ptLatLonMin.x, 0.), ptRes);
  extents.addPoint(ptRes);
  pGeoData->transformFromLonLatAlt(OdGePoint3d(ptLatLonMax.y, ptLatLonMax.x, 0.), ptRes);
  extents.addPoint(ptRes);
  dScale = OdBingMaps::TILE_SIZE / (extents.maxPoint().x - extents.minPoint().x);
  return eOk;
}

OdResult OdDbGeoMapHelper::getOptimalLOD(double dViewportDiagLengthInPixels, double dMapDiagLength, const OdDbGeoData * pGeoData, OdInt8& LOD)
{
  OdGePoint3d ptDesign = pGeoData->designPoint();
  OdGePoint3d ptDesignLLA;
  pGeoData->transformToLonLatAlt(ptDesign, ptDesignLLA);

  double dHorizontalUnitScale = pGeoData->horizontalUnitScale();

  double dPrevValue;
  double dValue = 1.e9;
  LOD = OdBingMaps::MIN_LOD;
  for (OdInt8 i = OdBingMaps::MIN_LOD; i <= OdBingMaps::MAX_LOD; ++i)
  {
    dPrevValue = dValue;
    double dCurrent = fabs((dHorizontalUnitScale * dMapDiagLength / dViewportDiagLengthInPixels) - OdBingMaps::GroundResolution(ptDesignLLA.y, i));
    if (dValue > dCurrent)
    {
      dValue = dCurrent;
    }

    if (dPrevValue > dCurrent)
    {
      LOD = i;
    }
  }
  return eOk;
}

OdResult OdDbGeoMapHelper::getImageSize(double dScale, const OdGePoint3dArray & arrVertices, OdGePoint3d & ptImageBottomLeft, OdInt32& width, OdInt32& height, double & dImageWidth, double & dImageHeight)
{
  OdGePoint3d ptImgBottomLeft;
  OdGePoint3d ptImgUpperRight;
  {
    OdGeExtents3d ext;
    ext.addPoints(arrVertices);
    ptImgBottomLeft.set(ext.minPoint().x, ext.minPoint().y, 0.);
    ptImgUpperRight.set(ext.maxPoint().x, ext.maxPoint().y, 0.);
  }

  //due to scale 1.02, we must also expand request area in future
  dImageWidth = (ptImgUpperRight.x - ptImgBottomLeft.x) * 1.02;
  dImageHeight = (ptImgUpperRight.y - ptImgBottomLeft.y) * 1.02;
  ptImageBottomLeft = (ptImgBottomLeft + ptImgUpperRight.asVector()) * 0.5 - OdGeVector3d(dImageWidth, dImageHeight, 0.) * 0.5;

  double dAspectRatio = dImageHeight / dImageWidth;

  double dWidth = (ptImgUpperRight.x - ptImgBottomLeft.x) * dScale;
  double dHeight = (ptImgUpperRight.y - ptImgBottomLeft.y) * dScale;
  OdUInt32 uWidth = 1;
  OdUInt32 uHeight = 1;
  OdDbGeoMapHelper::normalizeSize(dWidth, dHeight, uWidth, uHeight);

  // max ImageSize 2000x2000
  if (uWidth > 2000 || uHeight > 2000)
  {
    if (uWidth > uHeight)
    {
      uWidth = 2000;
      uHeight = dAspectRatio * 2000.;
    }
    else
    {
      uHeight = 2000;
      uWidth = 2000. / dAspectRatio;
    }
  }

  ////but min ImageSize 50õ50, and it's more preferable
  if (uWidth < 50 || uHeight < 50)
  {
    if (uWidth >= uHeight)
    {
      uHeight = 50;
      uWidth = 50. / dAspectRatio;
    }
    else
    {
      uWidth = 50;
      uHeight = dAspectRatio * 50.;
    }
  }

  if (uWidth & 1)
  {
    ++uWidth;
    uHeight = uWidth * dAspectRatio;
  }

  width = uWidth;
  height = uHeight;

  return eOk;
}

//////////////////////////////
// OdDbGeoMapHelper
//////////////////////////////


///////////////////////////////////////
// OdDbGeoMapPEImpl code
///////////////////////////////////////

ODRX_CONS_DEFINE_MEMBERS(OdDbGeoMapPEImpl, OdDbGeoMapPE, RXIMPL_CONSTR);


OdResult getViewportDiagonalLength(const OdDbDatabase * pDb, double & dViewportDiagonalLength)
{
  OdDbViewportTablePtr pVt = pDb->getViewportTableId().safeOpenObject();
  OdDbViewportTableRecordPtr pVp = pVt->getActiveViewportId().safeOpenObject();
  OdGsView* pView = pVp->gsView();
  if (pView == NULL)
  {
    return eNullPtr;
  }

  OdGsDCRect screenRect;
  pView->getViewport(screenRect);
  OdGeVector2d ptViewportBox(screenRect.m_max.x - screenRect.m_min.x, screenRect.m_max.y - screenRect.m_min.y);
  dViewportDiagonalLength = ptViewportBox.length();
  return eOk;
}

OdResult OdDbGeoMapPEImpl::updateMapImage(OdDbGeoMap* pGeoMap, bool bReset)
{
  if (pGeoMap == NULL)
  {
    return eNullPtr;
  }

  OdDbGeoMapFieldsPEPtr ext = OdDbGeoMap::desc()->getX(OdDbGeoMapFieldsPE::desc());
  if (ext.isNull())
  {
    return eNoInterface;
  }

  //if update failed, values must be restored
  ext->setIsOutOfDate(pGeoMap, false); // disable updateMapImage() in subClose()
  OdDbGeoMapPtr backupGeoMap = pGeoMap->clone();

  OdDbObjectId objId;
  oddbGetGeoDataObjId(pGeoMap->database(), objId);
  OdDbGeoDataPtr pGeoData = objId.openObject();
  if (pGeoData.isNull())
  {
    pGeoMap->copyFrom(backupGeoMap);
    ext->setIsOutOfDate(pGeoMap, true);
    return eNullPtr;
  }

  OdGePoint3dArray arrVertices;
  pGeoMap->getVertices(arrVertices);

  OdGeExtents3d extents;
  extents.addPoints(arrVertices);
  double dMapDiagLength = extents.diagonal().length();

  OdResult status = eOk;
  if (bReset)
  {
    OdInt8 nLOD;
    double dViewportDiagLength;
    if (eOk != (status = getViewportDiagonalLength(pGeoData->database(), dViewportDiagLength))
      || eOk != (status = OdDbGeoMapHelper::getOptimalLOD(dViewportDiagLength, dMapDiagLength, pGeoData, nLOD)))
    {
      pGeoMap->copyFrom(backupGeoMap);
      ext->setIsOutOfDate(pGeoMap, true);
      return status;
    }

    ext->setLOD(pGeoMap, nLOD);
    ext->setResolution(pGeoMap, kOptimal);
  }

  OdInt8 nLOD = odmin(odmax(pGeoMap->LOD() + pGeoMap->resolution(), OdBingMaps::MIN_LOD), OdBingMaps::MAX_LOD);

  // pre-request to check if we should reset geomap LOD
  {
    // create global map extents
    OdGePoint2dArray arrGlobalMapExtents;
    getGlobalMapExtents(pGeoData, arrGlobalMapExtents);

    // get map extents
    OdGePoint2dArray arrViewExtents;
    {
      arrViewExtents.resize(arrVertices.size());
      for (OdUInt32 i = 0; i < arrVertices.size(); ++i)
      {
        arrViewExtents[i] = arrVertices[i].convert2d();
      }
    }

    // get all needed tiles
    std::set<OdDbGeoMapTile> arrTiles;
    if (eOk != getGeoMapTiles(arrTiles, pGeoData, pGeoMap->mapType(), nLOD, arrViewExtents, arrGlobalMapExtents))
    {
      //reset
      double dViewportDiagLength;
      if (eOk != (status = getViewportDiagonalLength(pGeoData->database(), dViewportDiagLength))
        || eOk != (status = OdDbGeoMapHelper::getOptimalLOD(dViewportDiagLength, dMapDiagLength, pGeoData, nLOD))
        )
      {
        pGeoMap->copyFrom(backupGeoMap);
        ext->setIsOutOfDate(pGeoMap, true);
        return status;
      }
      ext->setLOD(pGeoMap, nLOD);
      ext->setResolution(pGeoMap, kOptimal);
    }
  }

  double dScale = 1.;
  OdDbGeoMapHelper::getImageScale(pGeoData, nLOD, dScale);

  OdGePoint3d ptImageBottomLeft;
  double dImageWidth, dImageHeight;
  OdInt32 nWidth, nHeight;
  if (eOk != (status = OdDbGeoMapHelper::getImageSize(dScale, arrVertices, ptImageBottomLeft, nWidth, nHeight, dImageWidth, dImageHeight)))
  {
    pGeoMap->copyFrom(backupGeoMap);
    ext->setIsOutOfDate(pGeoMap, true);
    return status;
  }

  ext->setPtImageBottomLeft(pGeoMap, ptImageBottomLeft);
  ext->setImageWidth(pGeoMap, dImageWidth);
  ext->setImageHeight(pGeoMap, dImageHeight);
  ext->setWidth(pGeoMap, nWidth);
  ext->setHeight(pGeoMap, nHeight);

  //if image resolution is changed UV must be updated:
  OdGeVector3d vU(dImageWidth / nWidth, 0., 0.);
  OdGeVector3d vV(0., dImageHeight / nHeight, 0.);
  ext->setvU(pGeoMap, vU);
  ext->setvV(pGeoMap, vV);
  
  OdGePoint2dArray arrImageArea;
  arrImageArea.resize(arrVertices.size());
  OdGePoint2dArray arrClipBnd;
  arrClipBnd.resize(arrVertices.size());
  OdGeMatrix3d mat = pixelToModelTransform(ptImageBottomLeft, vU, vV, nHeight).inverse();
  for (OdUInt32 i = 0; i < arrVertices.size(); ++i)
  {
    arrClipBnd[i] = (mat * arrVertices[i]).convert2d();
    arrImageArea[i] = arrVertices[i].convert2d();
  }
  ext->setPtClipBnd(pGeoMap, arrClipBnd);

  OdBinaryData PixelData;
  if (eOk != (status = OdDbGeoMapHelper::getMap(pGeoMap, PixelData)))
  {
    pGeoMap->copyFrom(backupGeoMap);
    ext->setIsOutOfDate(pGeoMap, true);
    return status;
  }
  ext->setPixelData(pGeoMap, PixelData);

  // get copyright strings
  OdStringArray arrCopyrightStrings;
  OdResult res = ProviderHelper::getProvidersString(pGeoMap->mapType(), pGeoData, nLOD, arrImageArea, arrCopyrightStrings);
  if (eOk != res)
  {
    pGeoMap->copyFrom(backupGeoMap);
    ext->setIsOutOfDate(pGeoMap, true);
    return res;
  }
  ext->setStrings(pGeoMap, arrCopyrightStrings);

  //calculate text parameters:
  {
    // loading bing logo "powered by"
    OdGiRasterImagePtr pRasterLogo;
    if (eOk != (status = ProviderHelper::getBrandLogo(pRasterLogo)))
    {
      pGeoMap->copyFrom(backupGeoMap);
      ext->setIsOutOfDate(pGeoMap, true);
      return status;
    }

    OdUInt32 nBingLogoWidth = pRasterLogo->pixelWidth();
    OdUInt32 nBingLogoHeight = pRasterLogo->pixelHeight();

    double dWidth = (arrVertices[1] - arrVertices[0]).length() * dScale;
    double dHeight = (arrVertices[2] - arrVertices[1]).length() * dScale;

    OdUInt32 nWidth, nHeight;
    OdDbGeoMapHelper::normalizeSize(dWidth, dHeight, nWidth, nHeight);

    double dResolutionScale = pow(2.0, pGeoMap->resolution());
    OdGeVector3d vVerticalShift = (arrVertices[2] - arrVertices[1]) * nBingLogoHeight * 0.75 / nHeight * dResolutionScale;
    OdGeVector3d vHozirontalShift = (arrVertices[1] - arrVertices[0]).normal() * nBingLogoWidth * vVerticalShift.length() / nBingLogoHeight;

    OdGePoint3d ptTextPosition = arrVertices[1] + vVerticalShift / 2. - vHozirontalShift;
    double dTextHeight = vVerticalShift.length() * 0.25;
    ext->setTextHeight(pGeoMap, dTextHeight);
    ext->setPtTextPosition(pGeoMap, ptTextPosition);
  }

  OdCmColor color;
  if (pGeoMap->mapType() == kRoad)
  {
    color.setRGB(0, 0, 0);
  }
  else
  {
    color.setRGB(255, 255, 255);
  }
  ext->setTextColor(pGeoMap, color);

  ext->setIsOutOfDate(pGeoMap, false);
  return eOk;
}

///////////////////////////////////////
// OdDbGeoMapPEImpl code end
///////////////////////////////////////

void OdDbGeoMapHelper::createLLAExtentsBy4Points(const OdGePoint2dArray& arrInput, OdGePoint2dArray& arrRes)
{
  const OdUInt32 nSize = arrInput.size();
  const OdUInt32 nStepsCount = 9;
  arrRes.resize(nSize * nStepsCount);
  OdUInt32 nIndex = 0;
  for (OdUInt32 i = 0; i < nSize; ++i)
  {
    OdGeVector2d vShift = (arrInput[(i + 1) % nSize] - arrInput[i]) / nStepsCount;
    for (OdUInt32 j = 0; j < nStepsCount; ++j)
    {
      arrRes[nIndex++] = arrInput[i] + vShift * j;
    }
  }
}

bool checkPointInsidePolygon(const OdGePoint2d & ptInput, const OdGePoint2dArray & arrPt)
{
  const OdUInt32 nSize = arrPt.size();
  const OdGePoint2d * pPtLast = &arrPt[nSize - 1];
  bool b4 = pPtLast->y >= ptInput.y;
  bool bInside = false;
  OdUInt32 nPointsDone = 0;
  const OdGePoint2d * pData = arrPt.asArrayPtr();

  if (nSize >= 4)
  {
    OdUInt32 nSteps = nSize / 4;
    nPointsDone = nSteps * 4;

    do
    {
      bool b0 = pData[0].y >= ptInput.y;
      bool b1 = pData[1].y >= ptInput.y;
      bool b2 = pData[2].y >= ptInput.y;
      bool b3 = pData[3].y >= ptInput.y;

      if (b4 != b0
        && b4 == (*pPtLast - pData[0]).crossProduct(ptInput - *pPtLast) >= 0.0)
      {
        bInside = !bInside;
      }

      if (b0 != b1
        && b0 == (pData[0] - pData[1]).crossProduct(ptInput - pData[0]) >= 0.0)
      {
        bInside = !bInside;
      }

      if (b1 != b2
        && b1 == (pData[1] - pData[2]).crossProduct(ptInput - pData[1]) >= 0.0)
      {
        bInside = !bInside;
      }

      if (b2 != b3
        && b2 == (pData[2] - pData[3]).crossProduct(ptInput - pData[2]) >= 0.0)
      {
        bInside = !bInside;
      }

      b4 = b3;
      pPtLast = pData + 3;
      pData += 4; //shift 4 points
      --nSteps;
    } while (nSteps);
  }

  if (nPointsDone < nSize)
  {
    OdUInt32 nPointsLeft = nSize - nPointsDone;
    do
    {
      if (b4 != pData[0].y >= ptInput.y
        && b4 == (*pPtLast - pData[0]).crossProduct(ptInput - *pPtLast) >= 0.0)
      {
        bInside = !bInside;
      }
      pPtLast = pData;
      b4 = pData[0].y >= ptInput.y;
      ++pData;
      --nPointsLeft;
    } while (nPointsLeft);
  }
  return bInside;
}

bool OdDbGeoMapHelper::getPolygonsPoint(const OdGePoint2dArray& arr1, const OdGePoint2dArray& arr2, OdGePoint2d & pt)
{
  const OdUInt32 nArr1Size = arr1.size();
  const OdUInt32 nArr2Size = arr2.size();
  for (OdUInt32 i = 0; i < nArr1Size; ++i)
  {
    if (checkPointInsidePolygon(arr1[i], arr2))
    {
      pt = arr1[i];
      return true;
    }
  }
  for (OdUInt32 i = 0; i < nArr2Size; ++i)
  {
    if (checkPointInsidePolygon(arr2[i], arr1))
    {
      pt = arr2[i];
      return true;
    }
  }

  double dTolerance = 1.e-10;
  OdGeLineSeg2d seg2;
  for (OdUInt32 i = 0; i < nArr2Size; ++i)
  {
    seg2.set(arr2[i], arr2[(i + 1) % nArr2Size]);
    if (dTolerance <= seg2.length())
    {
      OdGeLineSeg2d seg1;
      for (OdUInt32 i = 0; i < nArr1Size; ++i)
      {
        seg1.set(arr1[i], arr1[(i + 1) % nArr1Size]);
        if (dTolerance <= seg1.length() && seg1.intersectWith(seg2, pt))
        {
          return true;
        }
      }
      return false;
    }
  }

  return false;
}

void getChunkVertices(int nLOD, OdInt32 nTileX, OdInt32 nTileY, const OdDbGeoData * pGeoData, OdGePoint3dArray & arrChunkVertices)
{
  {
    //debug chunks drawing
    double latitudeMin, longitudeMin;
    double latitudeMax, longitudeMax;
    OdBingMaps::TileXYToLatLong(nTileX, nTileY, nLOD, latitudeMin, longitudeMin);
    OdBingMaps::TileXYToLatLong(nTileX + 1, nTileY + 1, nLOD, latitudeMax, longitudeMax);

    const OdUInt32 nRectSize = 4;
    arrChunkVertices.resize(nRectSize);
    arrChunkVertices[0].set(longitudeMin, latitudeMin, 0.);
    arrChunkVertices[1].set(longitudeMax, latitudeMin, 0.);
    arrChunkVertices[2].set(longitudeMax, latitudeMax, 0.);
    arrChunkVertices[3].set(longitudeMin, latitudeMax, 0.);

    for (OdUInt32 i = 0; i < nRectSize; ++i)
    {
      pGeoData->transformFromLonLatAlt(arrChunkVertices[i], arrChunkVertices[i]);
    }
  }
}

void getChunkVertices2d(int nLOD, OdInt32 nTileX, OdInt32 nTileY, const OdDbGeoData* pGeoData, OdGePoint2dArray& arrChunkVertices2d)
{
  OdGePoint3dArray arrChunkVertices;
  getChunkVertices(nLOD, nTileX, nTileY, pGeoData, arrChunkVertices);
  arrChunkVertices2d.resize(arrChunkVertices.size());
  for (OdUInt32 i = 0; i < arrChunkVertices.size(); ++i)
  {
    arrChunkVertices2d[i] = arrChunkVertices[i].convert2d();
  }
}

void addTileToStack(OdStack<OdDbGeoMapTile>& stackTiles, char cMapTypeId, int nLOD, OdInt32 nTileX, OdInt32 nTileY, const OdDbGeoData * pGeoData, const OdGePoint2dArray & arrMapExtents, const OdGePoint2dArray& arrGlobalMapExtents, const std::set<OdDbGeoMapTile> & arrTiles)
{
  if (!OdBingMaps::isTileValid(nLOD, nTileX, nTileY))
  {
    return;
  }

  OdDbGeoMapTile tile(cMapTypeId, nLOD, nTileX, nTileY);
  if (arrTiles.find(tile) != arrTiles.end())
  {
    return;
  }

  OdGePoint2dArray arrChunkVertices;
  getChunkVertices2d(nLOD, nTileX, nTileY, pGeoData, arrChunkVertices);
  OdGePoint2d ptRes;
  if (OdDbGeoMapHelper::getPolygonsPoint(arrChunkVertices, arrMapExtents, ptRes)
    && OdDbGeoMapHelper::getPolygonsPoint(arrChunkVertices, arrGlobalMapExtents, ptRes))
  {
    stackTiles.push(tile);
  }
}

OdResult getGlobalMapExtents(const OdDbGeoData * pGeoData, OdGePoint2dArray & arrMapExtents)
{
  OdDbGeoCoordinateSystemPtr pCs;
  OdResult res = OdDbGeoCoordinateSystem::create(pGeoData->coordinateSystem(), pCs);
  if (eOk != res)
  {
    return res;
  }

  OdGeExtents2d extGeo;
  pCs->getGeodeticExtents(extGeo);

  OdGePoint2d ptMin;
  OdGePoint2d ptMax;
  ptMin.x = odmin(odmax(extGeo.minPoint().x, OdBingMaps::MinLongitude), OdBingMaps::MaxLongitude);
  ptMin.y = odmin(odmax(extGeo.minPoint().y, OdBingMaps::MinLatitude), OdBingMaps::MaxLatitude);
  ptMax.x = odmin(odmax(extGeo.maxPoint().x, OdBingMaps::MinLongitude), OdBingMaps::MaxLongitude);
  ptMax.y = odmin(odmax(extGeo.maxPoint().y, OdBingMaps::MinLatitude), OdBingMaps::MaxLatitude);

  OdGeVector2d vShift(0.01, 0.01);
  ptMin += vShift;
  ptMax -= vShift;

  OdGePoint2dArray arrInput;
  arrInput.resize(4);
  arrInput[0].set(ptMin.x, ptMin.y);
  arrInput[1].set(ptMax.x, ptMin.y);
  arrInput[2].set(ptMax.x, ptMax.y);
  arrInput[3].set(ptMin.x, ptMax.y);

  OdGePoint2dArray arrResLLA;
  OdDbGeoMapHelper::createLLAExtentsBy4Points(arrInput, arrResLLA);

  arrMapExtents.resize(arrResLLA.size());
  OdGePoint3d ptBuf;
  for (OdUInt32 i = 0; i < arrResLLA.size(); ++i)
  {
    pGeoData->transformFromLonLatAlt(OdGePoint3d(arrResLLA[i].x, arrResLLA[i].y, 0.), ptBuf);
    arrMapExtents[i] = ptBuf.convert2d();
  }
  return eOk;
}

OdResult getGeoMapTiles(std::set<OdDbGeoMapTile>& arrTiles, const OdDbGeoData* pGeoData, OdGeoMapType eGeoMapType, OdInt8 nLOD, const OdGePoint2dArray& arrMapExtents, const OdGePoint2dArray& arrGlobalMapExtents)
{
  char cMapTypeId = (eGeoMapType == kAerial) ? 'a' : (eGeoMapType == kHybrid) ? 'h' : 'r';
  // get first tile
  OdDbGeoMapTile firstTile;
  {
    OdGePoint2d pt2d;
    OdDbGeoMapHelper::getPolygonsPoint(arrMapExtents, arrGlobalMapExtents, pt2d);
    OdGePoint3d pt3d(pt2d.x, pt2d.y, pGeoData->designPoint().z);
    OdGePoint3d pt3dLLA;
    pGeoData->transformToLonLatAlt(pt3d, pt3dLLA);
    OdInt32 nTileX, nTileY;
    OdBingMaps::LatLongToTileXY(pt3dLLA.y, pt3dLLA.x, nLOD, nTileX, nTileY);
    firstTile.set(cMapTypeId, nLOD, nTileX, nTileY);
  }

  OdStack<OdDbGeoMapTile> stackTiles;
  stackTiles.push(firstTile);
  while (!stackTiles.empty())
  {
    OdDbGeoMapTile* pTile = stackTiles.top();
    OdInt32 nTileX = pTile->getX();
    OdInt32 nTileY = pTile->getY();
    arrTiles.insert(*pTile);
    stackTiles.pop();
    addTileToStack(stackTiles, cMapTypeId, nLOD, nTileX - 1, nTileY, pGeoData, arrMapExtents, arrGlobalMapExtents, arrTiles);
    addTileToStack(stackTiles, cMapTypeId, nLOD, nTileX + 1, nTileY, pGeoData, arrMapExtents, arrGlobalMapExtents, arrTiles);
    addTileToStack(stackTiles, cMapTypeId, nLOD, nTileX, nTileY - 1, pGeoData, arrMapExtents, arrGlobalMapExtents, arrTiles);
    addTileToStack(stackTiles, cMapTypeId, nLOD, nTileX, nTileY + 1, pGeoData, arrMapExtents, arrGlobalMapExtents, arrTiles);
    if (arrTiles.size() > 2500)
    {
      //extents are to big
      return eInvalidExtents;
    }
  }
  return eOk;
}

OdGeoMapType getGeoMapType(OdDbObjectId viewportId)
{
  OdDbViewportTableRecordPtr pVp = viewportId.safeOpenObject();
  OdDbDictionaryPtr pExtDictionary = pVp->extensionDictionary().openObject();
  OdDbDictionaryVarPtr pDictionaryVar;
  if (pExtDictionary.get())
  {
    OdDbDictionaryPtr pExtVariableDictionary = pExtDictionary->getAt(L"AcDbVariableDictionary").openObject();
    if (pExtVariableDictionary.get())
    {
      pDictionaryVar = pExtVariableDictionary->getAt(L"GEOMAPMODE").openObject();
    }
  }
  if (pDictionaryVar.isNull())
  {
    return OdGeoMapType::kNoMap;
  }
  OdString sGeoMapType = pDictionaryVar->value();
  return (OdGeoMapType)Od_strtoint(sGeoMapType);
}

OdResult getGeoData(OdDbDatabase * pDb, OdDbGeoDataPtr & pGeoData)
{
  OdDbObjectId idGeoData;
  if (eOk != oddbGetGeoDataObjId(pDb, idGeoData)
    || idGeoData.isNull()
    || (pGeoData = idGeoData.openObject()).isNull()
    )
  {
    return eInvalidInput;
  }
  return eOk;
}

void drawTiles(OdGiViewportDraw* pVd, const OdDbGeoData * pGeoData, const OdGePoint2dArray & arrGlobalMapExtents, const std::set<OdDbGeoMapTile> & arrTiles, const OdArray<OdGiRasterImagePtr> & arrTilesImages)
{
  // clipping boundary
  OdGiClipBoundary boundary;
  boundary.m_vNormal = pGeoData->upDirection();
  boundary.m_Points = arrGlobalMapExtents;
  boundary.m_dFrontClipZ = 0.;
  boundary.m_dBackClipZ = 0.;
  boundary.m_bClippingFront = false;
  boundary.m_bClippingBack = false;
  boundary.m_bDrawBoundary = false;
  pVd->geometry().pushClipBoundary(&boundary);

  double dZCorrection = 0.1;
  double dHorizontalUnitScale = pGeoData->horizontalUnitScale();
  if (dHorizontalUnitScale > OdGeContext::gTol.equalPoint())
  {
    dZCorrection = 0.1 / dHorizontalUnitScale;
  }
  if (pVd->viewport().viewDir().z > 0)
  {
    dZCorrection = -dZCorrection;
  }

  OdUInt32 uIndex = 0;
  std::set<OdDbGeoMapTile>::const_iterator pIt = arrTiles.begin();
  std::set<OdDbGeoMapTile>::const_iterator pEnd = arrTiles.end();
  for (; pIt != pEnd; ++pIt, ++uIndex)
  {
    // get chunk clip boundary
    const OdDbGeoMapTile& tile = *pIt;
    OdGePoint3dArray arrChunkVertices;
    getChunkVertices(tile.getLOD(), tile.getX(), tile.getY(), pGeoData, arrChunkVertices);

    for (OdUInt32 i = 0; i < 4; ++i)
    {
      arrChunkVertices[i].z = dZCorrection;
    }
    arrChunkVertices.swap(1, 3); //correct order for draw func

    // draw tile
    odgiImageViewport(pVd->geometry(), arrTilesImages[uIndex], arrChunkVertices.asArrayPtr());
  }

  pVd->geometry().popClipBoundary();
}

OdResult OdDbGeoDataChunksPEImpl::drawGeoDataChunks(OdGiViewportDraw* pVd) const
{
  OdGiDrawFlagsHelper autoFlags(pVd->subEntityTraits(), OdGiSubEntityTraits::kExcludeFromViewExt);
  OdDbDatabasePtr pDb = pVd->context()->database();

  // determine OdDbGeoData:
  OdDbGeoDataPtr pGeoData;
  OdResult res = getGeoData(pDb, pGeoData);
  if (eOk != res)
  {
    return res;
  }

  // determine OdGeoMapType:
  OdGeoMapType eGeoMapType = getGeoMapType(pVd->viewportObjectId());
  if (OdGeoMapType::kNoMap == eGeoMapType)
  {
    return eOk;
  }

  // get viewport DC corners
  OdGePoint2d lowerLeft, upperRight;
  pVd->viewport().getViewportDcCorners(lowerLeft, upperRight);

  //get scale
  OdGePoint2d ptScale;
  pVd->viewport().getNumPixelsInUnitSquare(OdGePoint3d::kOrigin, ptScale, false);

  // get map extents
  OdGePoint2dArray arrMapExtents;
  OdGePoint3dArray arrMapExtents3d;
  OdGePoint3dArray arrViewPt3d;
  {
    OdGeMatrix3d mat = pVd->viewport().getEyeToWorldTransform();
    arrViewPt3d.resize(4);
    arrViewPt3d[0].set(lowerLeft.x, lowerLeft.y, 0.);
    arrViewPt3d[1].set(upperRight.x, lowerLeft.y, 0.);
    arrViewPt3d[2].set(upperRight.x, upperRight.y, 0.);
    arrViewPt3d[3].set(lowerLeft.x, upperRight.y, 0.);

    arrMapExtents3d.resize(4);
    arrMapExtents.resize(4);
    OdGePlane plane(pGeoData->designPoint(), pGeoData->upDirection());
    OdGeTol tol = 1.e-6;
    for (OdUInt32 i = 0; i < 4; ++i)
    {
      arrViewPt3d[i].transformBy(mat);
      plane.intersectWith(OdGeRay3d(arrViewPt3d[i], pVd->viewport().viewDir()), arrMapExtents3d[i], tol);
      arrMapExtents[i] = arrMapExtents3d[i].convert2d();
    }
  }

  // get level of detail for result map image (for current viewport)
  OdInt8 nLOD = OdBingMaps::MIN_LOD;
  {
    OdGeExtents2d extents2d;
    extents2d.addPoints(arrMapExtents);

    double dWidth = (upperRight.x - lowerLeft.x) * ptScale.x;
    double dHeight = (upperRight.y - lowerLeft.y) * ptScale.y;
    double dViewportDiagLengthInPixels = OdGeVector2d(dWidth, dHeight).length();
    double dMapDiagLength = extents2d.diagonal().length();
    OdResult res = OdDbGeoMapHelper::getOptimalLOD(dViewportDiagLengthInPixels, dMapDiagLength, pGeoData, nLOD);
    if (eOk != res)
    {
      return res;
    }
  }

  // create global map extents
  OdGePoint2dArray arrGlobalMapExtents;
  if (eOk != getGlobalMapExtents(pGeoData, arrGlobalMapExtents))
  {
    return eInvalidInput;
  }

  // get all needed tiles
  std::set<OdDbGeoMapTile> arrTiles;
  res = getGeoMapTiles(arrTiles, pGeoData, eGeoMapType, nLOD, arrMapExtents, arrGlobalMapExtents);
  if (eOk != res)
  {
    return res;
  }

  // get all tiles images
  OdArray<OdGiRasterImagePtr> arrTilesImages;
  res = g_OdBingMapsCache->getTiles(arrTiles, arrTilesImages);
  if (eOk != res)
  {
    return res;
  }

  //draw tiles
  drawTiles(pVd, pGeoData, arrGlobalMapExtents, arrTiles, arrTilesImages);
  return eOk;
}

OdResult OdDbGeoDataChunksPEImpl::drawGeoDataProvidersString(OdGiViewportDraw* pVd) const
{
  OdGiDrawFlagsHelper autoFlags(pVd->subEntityTraits(), OdGiSubEntityTraits::kExcludeFromViewExt);
  OdDbDatabasePtr pDb = pVd->context()->database();

  // determine OdDbGeoData:
  OdDbGeoDataPtr pGeoData;
  OdResult res = getGeoData(pDb, pGeoData);
  if (eOk != res)
  {
    return res;
  }

  // determine OdGeoMapType:
  OdGeoMapType eGeoMapType = getGeoMapType(pVd->viewportObjectId());
  if (OdGeoMapType::kNoMap == eGeoMapType)
  {
    return eOk;
  }

  // get viewport DC corners
  OdGePoint2d lowerLeft, upperRight;
  pVd->viewport().getViewportDcCorners(lowerLeft, upperRight);

  //get scale
  OdGePoint2d ptScale;
  pVd->viewport().getNumPixelsInUnitSquare(OdGePoint3d::kOrigin, ptScale, false);

  // get map extents
  OdGePoint2dArray arrMapExtents;
  OdGePoint3dArray arrMapExtents3d;
  OdGePoint3dArray arrViewPt3d;
  {
    OdGeMatrix3d mat = pVd->viewport().getEyeToWorldTransform();
    arrViewPt3d.resize(4);
    arrViewPt3d[0].set(lowerLeft.x, lowerLeft.y, 0.);
    arrViewPt3d[1].set(upperRight.x, lowerLeft.y, 0.);
    arrViewPt3d[2].set(upperRight.x, upperRight.y, 0.);
    arrViewPt3d[3].set(lowerLeft.x, upperRight.y, 0.);

    arrMapExtents3d.resize(4);
    arrMapExtents.resize(4);
    OdGePlane plane(pGeoData->designPoint(), pGeoData->upDirection());
    OdGeTol tol = 1.e-6;
    for (OdUInt32 i = 0; i < 4; ++i)
    {
      arrViewPt3d[i].transformBy(mat);
      plane.intersectWith(OdGeRay3d(arrViewPt3d[i], pVd->viewport().viewDir()), arrMapExtents3d[i], tol);
      arrMapExtents[i] = arrMapExtents3d[i].convert2d();
    }
  }

  // get level of detail for result map image (for current viewport)
  OdInt8 nLOD = OdBingMaps::MIN_LOD;
  {
    OdGeExtents2d extents2d;
    extents2d.addPoints(arrMapExtents);

    double dWidth = (upperRight.x - lowerLeft.x) * ptScale.x;
    double dHeight = (upperRight.y - lowerLeft.y) * ptScale.y;
    double dViewportDiagLengthInPixels = OdGeVector2d(dWidth, dHeight).length();
    double dMapDiagLength = extents2d.diagonal().length();
    OdResult res = OdDbGeoMapHelper::getOptimalLOD(dViewportDiagLengthInPixels, dMapDiagLength, pGeoData, nLOD);
    if (eOk != res)
    {
      return res;
    }
  }

  {
    // drawing bing logo "powered by"
    OdGiRasterImagePtr pRasterLogo;
    if (eOk == ProviderHelper::getBrandLogo(pRasterLogo))
    {
      const OdUInt32 nBingLogoWidth = pRasterLogo->pixelWidth();
      const OdUInt32 nBingLogoHeight = pRasterLogo->pixelHeight();

      OdGePoint3d ptOrigin(upperRight.x - nBingLogoWidth / ptScale.x, lowerLeft.y, 0.);
      OdGeVector3d vU(1. / ptScale.x, 0., 0.);
      OdGeVector3d vV(0., 1. / ptScale.y, 0.);

      OdGeMatrix3d mat = pVd->viewport().getEyeToWorldTransform();
      ptOrigin.transformBy(mat);
      vU.transformBy(mat);
      vV.transformBy(mat);

      OdGePoint2dArray arrLogoClipBoundary;
      arrLogoClipBoundary.resize(5);
      arrLogoClipBoundary[0].set(0., 0.);
      arrLogoClipBoundary[1].set(nBingLogoWidth, 0.);
      arrLogoClipBoundary[2].set(nBingLogoWidth, nBingLogoHeight);
      arrLogoClipBoundary[3].set(0., nBingLogoHeight);
      arrLogoClipBoundary[4] = arrLogoClipBoundary[0];
      pVd->geometry().rasterImageDc(ptOrigin, vU, vV, pRasterLogo, arrLogoClipBoundary.asArrayPtr(), arrLogoClipBoundary.size(), true);

      // drawing copyright strings
      {
        OdStringArray arrCopyrightStrings;
        ProviderHelper::getProvidersString(eGeoMapType, pGeoData, nLOD, arrMapExtents, arrCopyrightStrings);
        OdCmColor textColor;
        if (eGeoMapType == kRoad)
        {
          textColor.setRGB(0, 0, 0);
        }
        else
        {
          textColor.setRGB(255, 255, 255);
        }

        double dTextHeight = 0.25 * nBingLogoHeight / ptScale.y;
        OdGePoint3d ptTextPosition(upperRight.x - nBingLogoWidth / ptScale.x, lowerLeft.y + nBingLogoHeight / ptScale.y / 2., 0.);
        ptTextPosition.transformBy(mat);

        drawProvidersStrings2(pVd, pDb, arrCopyrightStrings, arrViewPt3d, ptTextPosition, dTextHeight, textColor);
      }
    }
  }
  return eOk;
}

////////////////////////
//OdDbGeoMapImageCreator
OdDbGeoMapImageCreator::OdDbGeoMapImageCreator()
  : m_pGsNode(0)
  , m_nRefCounter(1)
{
}

void OdDbGeoMapImageCreator::addRef()
{
  ++m_nRefCounter;
}

void OdDbGeoMapImageCreator::release()
{
  ODA_ASSERT((m_nRefCounter > 0));
  if (!(--m_nRefCounter))
  {
    delete this;
  }
}

long OdDbGeoMapImageCreator::numRefs() const
{
  return m_nRefCounter;
}

void OdDbGeoMapImageCreator::setGsNode(OdGsCache* pGsNode)
{
  m_pGsNode = pGsNode;
}

OdGsCache* OdDbGeoMapImageCreator::gsNode() const
{
  return m_pGsNode;
}

bool OdDbGeoMapImageCreator::isPersistent() const
{
  return false;
}

OdDbStub* OdDbGeoMapImageCreator::id() const
{
  return 0;
}

OdDbGeoMapImageCreator::~OdDbGeoMapImageCreator()
{
  if (m_pGsNode)
  {
    m_pGsNode->setDrawableNull();
  }
}

OdDbGeoMapImageCreatorPtr OdDbGeoMapImageCreator::createObject()
{
  return OdRxObjectImpl<OdDbGeoMapImageCreator>::createObject();
}

OdUInt32 OdDbGeoMapImageCreator::subSetAttributes(OdGiDrawableTraits* pTraits) const
{
  return kDrawableRegenDraw;
}

bool OdDbGeoMapImageCreator::subWorldDraw(OdGiWorldDraw* pWd) const
{
  return false;
}

void OdDbGeoMapImageCreator::subViewportDraw(OdGiViewportDraw* pVd) const
{
  drawGeoMap(pVd); //TODO: status
}

OdResult OdDbGeoMapImageCreator::drawGeoMap(OdGiViewportDraw* pVd) const
{
  OdGiDrawFlagsHelper autoFlags(pVd->subEntityTraits(), OdGiSubEntityTraits::kExcludeFromViewExt);
  OdDbGeoMapPtr pGeoMap = m_idGeoMap.openObject();

  // determine OdDbGeoData:
  OdDbGeoDataPtr pGeoData;
  OdResult res = getGeoData(pGeoMap->database(), pGeoData);
  if (eOk != res)
  {
    return res;
  }

  // determine OdGeoMapType:
  OdGeoMapType eGeoMapType = pGeoMap->mapType();

  // get map extents
  OdGePoint2dArray arrMapExtents;
  OdGePoint3dArray arrMapExtents3d;
  {
    pGeoMap->getVertices(arrMapExtents3d);
    arrMapExtents.resize(arrMapExtents3d.size());
    for (OdUInt32 i = 0; i < arrMapExtents3d.size(); ++i)
    {
      arrMapExtents[i] = arrMapExtents3d[i].convert2d();
    }
  }

  // get level of detail for current map image
  OdInt8 nLOD = odmin(odmax(pGeoMap->LOD() + pGeoMap->resolution(), OdBingMaps::MIN_LOD), OdBingMaps::MAX_LOD);

  // create global map extents
  OdGePoint2dArray arrGlobalMapExtents;
  if (eOk != getGlobalMapExtents(pGeoData, arrGlobalMapExtents))
  {
    return eInvalidInput;
  }

  // get all needed tiles
  std::set<OdDbGeoMapTile> arrTiles;
  res = getGeoMapTiles(arrTiles, pGeoData, eGeoMapType, nLOD, arrMapExtents, arrGlobalMapExtents);
  if (eOk != res)
  {
    return res;
  }

  // get all tiles images
  OdArray<OdGiRasterImagePtr> arrTilesImages;
  res = g_OdBingMapsCache->getTiles(arrTiles, arrTilesImages);
  if (eOk != res)
  {
    return res;
  }

  //draw tiles
  drawTiles(pVd, pGeoData, arrGlobalMapExtents, arrTiles, arrTilesImages);

  {
    // drawing bing logo "powered by"
    OdGiRasterImagePtr pRasterLogo;
    if (eOk == ProviderHelper::getBrandLogo(pRasterLogo))
    {
      double dScale = 1.;
      OdDbGeoMapHelper::getImageScale(pGeoData, nLOD, dScale);

      double dResolutionScale = pow(2.0, pGeoMap->resolution());
      double dResultScale = dResolutionScale * 0.75 / dScale;

      OdGeVector3d vU = (arrMapExtents3d[1] - arrMapExtents3d[0]).normalize() * dResultScale;
      OdGeVector3d vV = (arrMapExtents3d[2] - arrMapExtents3d[1]).normalize() * dResultScale;
      OdGePoint3d ptOrigin(arrMapExtents3d[1] - vU * pRasterLogo->pixelWidth());

      OdGePoint2dArray arrLogoClipBoundary;
      arrLogoClipBoundary.resize(5);
      arrLogoClipBoundary[0].set(0., 0.);
      arrLogoClipBoundary[1].set(pRasterLogo->pixelWidth(), 0.);
      arrLogoClipBoundary[2].set(pRasterLogo->pixelWidth(), pRasterLogo->pixelHeight());
      arrLogoClipBoundary[3].set(0., pRasterLogo->pixelHeight());
      arrLogoClipBoundary[4] = arrLogoClipBoundary[0];
      pVd->geometry().rasterImageDc(ptOrigin, vU, vV, pRasterLogo, arrLogoClipBoundary.asArrayPtr(), arrLogoClipBoundary.size(), true);
    }
  }
  return eOk;
}

void OdDbGeoMapImageCreator::setGeoMapId(OdDbObjectId id)
{
  m_idGeoMap = id;
}
