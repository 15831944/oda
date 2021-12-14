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
#include "DbGeoMap.h"
#include "Gi/GiWorldDraw.h"
#include "Gi/GiViewportDraw.h"
#include "DbRasterVariables.h"
#include "DbFiler.h"

#include "DbProxyEntity.h"
#include "DbProxyObject.h"
#include "RxObject.h"
#include "DbText.h"

#include "DbSortentsTable.h"
#include "DbBlockTableRecord.h"

#include "DbGeoMapPE.h"

#include "Gi/GiTextStyle.h"
#include "DbHostAppServices.h"

ODRX_NO_CONS_DEFINE_MEMBERS(OdDbGeoMapPE, OdRxObject);

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

ODRX_DEFINE_MEMBERS_EX(OdDbGeoMap, OdDbRasterImage, DBOBJECT_CONSTR,
                       OdDb::vAC27, 81, OdDbProxyEntity::kEraseAllowed,
                       L"AcDbGeoMap", L"GEOMAPIMAGE", L"AcGeolocationObj",
                       OdRx::kMTLoading|OdRx::kMTRender|OdRx::kMTRenderInBlock | OdRx::kHistoryAware);


OdDbGeoMap::OdDbGeoMap()
  : OdDbRasterImage()
  , m_myBrightness(50)
  , m_myContrast(50)
  , m_myFade(0)
  , m_nWidth(0)
  , m_nHeight(0)
  , m_bOutOfDate(true)
  , m_LOD(14)
  , m_Resolution(kOptimal)
  , m_MapType(kAerial)
  , m_dImageWidth(0.)
  , m_dImageHeight(0.)
  , m_dragStatus(OdDb::kDragEnd)
{
}


OdDbGeoMap::~OdDbGeoMap()
{
}

OdResult OdDbGeoMap::dwgInFields(OdDbDwgFiler* pFiler)
{
  assertWriteEnabled();

  OdResult res = OdDbRasterImage::dwgInFields(pFiler);
  if (res != eOk)
  {
    return res;
  }

  int classVersion = pFiler->rdInt16();
  if (classVersion != 0)
  {
    ODA_FAIL_ONCE();
    return eMakeMeProxy;
  }

  m_UnkId = pFiler->rdSoftPointerId();
  ODA_ASSERT_ONCE(m_UnkId.isNull());

  m_ptImageBottomLeft = pFiler->rdPoint3d();
  m_ptTextPosition = pFiler->rdPoint3d();

  m_dImageWidth = pFiler->rdDouble();
  m_dImageHeight = pFiler->rdDouble();

  m_LOD = pFiler->rdInt8();

  m_Resolution = pFiler->rdUInt8();
  ODA_ASSERT_ONCE(m_Resolution <= kFiner);

  m_MapType = pFiler->rdUInt8();
  ODA_ASSERT_ONCE(m_MapType <= kHybrid);

  m_vU = pFiler->rdVector3d();
  m_vV = pFiler->rdVector3d();

  m_myBrightness = pFiler->rdInt8();
  m_myContrast = pFiler->rdInt8();
  m_myFade = pFiler->rdInt8();
  m_bOutOfDate = pFiler->rdBool();

  {
    //image data
    m_nWidth = pFiler->rdInt32();
    m_nHeight = pFiler->rdInt32();

    if (pFiler->filerType() != OdDbFiler::kCopyFiler)
    {
      OdUInt32 nBytes = m_nWidth * m_nHeight * 4;
      m_PixelData.resize(nBytes);
      if (nBytes)
      {
        pFiler->rdBytes(m_PixelData.asArrayPtr(), nBytes);
      }
    }
  }

  {
    //points array
    m_arrVertices.setLogicalLength(pFiler->rdInt32());
    m_arrVertices.setPhysicalLength(pFiler->rdInt32());
    m_arrVertices.setGrowLength(pFiler->rdInt32());

    for (OdUInt32 i = 0; i < m_arrVertices.size(); ++i)
    {
      m_arrVertices[i] = pFiler->rdPoint3d();
    }
  }

  OdInt32 nStrings = pFiler->rdInt32();
  m_strings.resize(nStrings);
  for (OdInt32 i = 0; i < nStrings; ++i)
  {
    m_strings[i] = pFiler->rdString();
  }

  m_textColor.dwgIn(pFiler);
  m_dTextHeight = pFiler->rdDouble();

  {
    //ClipBoundary
    OdInt16 eClipBoundaryType = pFiler->rdInt16();
    if (eClipBoundaryType == kRect)
    {
      m_ptClipBnd.resize(2);
      m_ptClipBnd[0] = pFiler->rdPoint2d();
      m_ptClipBnd[1] = pFiler->rdPoint2d();
    }
    else if (eClipBoundaryType == kPoly)
    {
      OdInt32 nClipPts = pFiler->rdInt32();
      m_ptClipBnd.resize(nClipPts + 1);
      for (OdInt32 i = 0; i < nClipPts; ++i)
      {
        m_ptClipBnd[i] = pFiler->rdPoint2d();
      }
      if (nClipPts > 0)
      {
        m_ptClipBnd.last() = m_ptClipBnd.first();
      }
    }
  }

  m_idGeoMapDef = pFiler->rdHardOwnershipId();

  if (pFiler->filerType() == OdDbFiler::kCopyFiler)
  {
    m_dragStatus = (OdDb::DragStat)pFiler->rdUInt8();
  }
  return eOk;
}


void OdDbGeoMap::dwgOutFields(OdDbDwgFiler* pFiler) const
{
  assertReadEnabled();

  OdDbRasterImage::dwgOutFields(pFiler);

  pFiler->wrInt16(0); //classVersion

  pFiler->wrSoftPointerId(m_UnkId);

  pFiler->wrPoint3d(m_ptImageBottomLeft);
  pFiler->wrPoint3d(m_ptTextPosition);

  pFiler->wrDouble(m_dImageWidth);
  pFiler->wrDouble(m_dImageHeight);

  pFiler->wrInt8(m_LOD);
  pFiler->wrUInt8(m_Resolution);
  pFiler->wrUInt8(m_MapType);
  pFiler->wrVector3d(m_vU);
  pFiler->wrVector3d(m_vV);

  pFiler->wrInt8(m_myBrightness);
  pFiler->wrInt8(m_myContrast);
  pFiler->wrInt8(m_myFade);
  pFiler->wrBool(m_bOutOfDate);

  {
    //image data
    pFiler->wrInt32(m_nWidth);
    pFiler->wrInt32(m_nHeight);

    if (pFiler->filerType() != OdDbFiler::kCopyFiler)
    {
      OdUInt32 nBytes = m_nWidth * m_nHeight * 4;
      if (nBytes)
      {
        pFiler->wrBytes(m_PixelData.asArrayPtr(), nBytes);
      }
    }
  }

  {
    //points array
    pFiler->wrInt32(m_arrVertices.logicalLength());
    pFiler->wrInt32(m_arrVertices.physicalLength());
    pFiler->wrInt32(m_arrVertices.growLength());

    for (OdUInt32 i = 0; i < m_arrVertices.logicalLength(); ++i)
    {
      pFiler->wrPoint3d(m_arrVertices[i]);
    }
  }

  OdInt32 nStrings = m_strings.size();
  pFiler->wrInt32(nStrings);
  for (OdInt32 i = 0; i < nStrings; ++i)
  {
    pFiler->wrString(m_strings[i]);
  }

  m_textColor.dwgOut(pFiler);
  pFiler->wrDouble(m_dTextHeight);

  
  {
    //ClipBoundary
    OdInt32 nClipPts = m_ptClipBnd.size();
    if (nClipPts == 2)
    {
      pFiler->wrInt16(kRect);
      pFiler->wrPoint2d(m_ptClipBnd[0]);
      pFiler->wrPoint2d(m_ptClipBnd[1]);
    }
    else if (nClipPts > 0)
    {
      pFiler->wrInt16(kPoly);
      --nClipPts;
      pFiler->wrInt32(nClipPts);
      for (OdInt32 i = 0; i < nClipPts; ++i)
      {
        pFiler->wrPoint2d(m_ptClipBnd[i]);
      }
    }
    else
    {
      pFiler->wrInt16(kInvalid);
    }
  }

  pFiler->wrHardOwnershipId(m_idGeoMapDef);

  if (pFiler->filerType() == OdDbFiler::kCopyFiler)
  {
    pFiler->wrUInt8((OdUInt8)m_dragStatus);
  }
}


OdGeoMapResolution OdDbGeoMap::resolution() const
{
  assertReadEnabled();
  return (OdGeoMapResolution)m_Resolution;
}

OdResult OdDbGeoMap::setResolution(OdGeoMapResolution resolution)
{
  assertWriteEnabled();
  // just in case (no check in acad)
  if (resolution < kCoarse)
  {
    resolution = kCoarse;
  }
  else if (resolution > kFiner)
  {
    resolution = kFiner;
  }
  
  if (m_Resolution != resolution)
  {
    m_Resolution = resolution;
    m_bOutOfDate = true;
  }

  return eOk;
}

OdInt8 OdDbGeoMap::LOD() const
{
  assertReadEnabled();
  return m_LOD;
}

OdGeoMapType OdDbGeoMap::mapType() const
{
  assertReadEnabled();
  return (OdGeoMapType)m_MapType;
}

OdResult OdDbGeoMap::setMapType(OdGeoMapType mapType)
{
  assertWriteEnabled();
  if (mapType == kNoMap)
  {
    return eInvalidInput;
  }

  if (m_MapType != mapType)
  {
    m_MapType = mapType;
    m_bOutOfDate = true;
  }

  return eOk;
}


OdGePoint3d OdDbGeoMap::imageBottomLeftPt() const
{
  assertReadEnabled();
  return m_ptImageBottomLeft;
}

double OdDbGeoMap::imageHeight() const
{
  assertReadEnabled();
  return m_dImageHeight;
}

double OdDbGeoMap::imageWidth() const
{
  assertReadEnabled();
  return m_dImageWidth;
}

OdGeVector2d OdDbGeoMap::imageSize(bool /*bGetCachedValue = false*/) const
{
  assertReadEnabled();
  return OdGeVector2d(m_nWidth, m_nHeight);
}

OdDbObjectId OdDbGeoMap::imageDefId() const
{
  assertReadEnabled();
  return m_idGeoMapDef;
}

OdResult OdDbGeoMap::getVertices(OdGePoint3dArray& vertices) const
{
  assertReadEnabled();
  if (m_arrVertices.size() != 4)
  {
    return eNotInitializedYet;
  }
  vertices = m_arrVertices;
  vertices.append(m_arrVertices.first());
  return eOk;
}

const OdGePoint2dArray& OdDbGeoMap::clipBoundary() const
{
  assertReadEnabled();
  return m_ptClipBnd;
}

bool OdDbGeoMap::isOutOfDate() const
{
  assertReadEnabled();
  return m_bOutOfDate;
}

#define NEXT_CODE(code)         \
  if (pFiler->nextItem() != code) \
{                               \
  ODA_FAIL_ONCE();              \
  return eMakeMeProxy;          \
}

OdResult OdDbGeoMap::dxfInFields(OdDbDxfFiler* pFiler)
{
  OdResult res = OdDbEntity::dxfInFields(pFiler);
  if (res != eOk)
  {
    ODA_FAIL_ONCE();
    return res;
  }

  if (!pFiler->atSubclassData(desc()->name()))
  {
    ODA_FAIL_ONCE();
    return eMakeMeProxy;
  }

  setDisplayOpt(OdDbRasterImage::kShow, true);  // Because RasterImage data is not saved to DXF

  NEXT_CODE(70);
  int classVersion = pFiler->rdInt16();
  if (classVersion != 0)
  {
    ODA_FAIL_ONCE();
    return eMakeMeProxy;
  }
  NEXT_CODE(330);
  m_UnkId = pFiler->rdObjectId();
  ODA_ASSERT_ONCE(m_UnkId.isNull());

  NEXT_CODE(10);
  pFiler->rdPoint3d(m_ptImageBottomLeft);
  NEXT_CODE(10);
  pFiler->rdPoint3d(m_ptTextPosition);

  NEXT_CODE(40);
  m_dImageWidth = pFiler->rdDouble();
  NEXT_CODE(40);
  m_dImageHeight = pFiler->rdDouble();

  NEXT_CODE(280);
  m_LOD = pFiler->rdInt8();

  NEXT_CODE(280);
  m_Resolution = pFiler->rdUInt8();
  ODA_ASSERT_ONCE(m_Resolution <= kFiner);

  NEXT_CODE(280);
  m_MapType = pFiler->rdUInt8();
  ODA_ASSERT_ONCE(m_MapType <= kHybrid);

  NEXT_CODE(10);
  pFiler->rdVector3d(m_vU);
  NEXT_CODE(10);
  pFiler->rdVector3d(m_vV);

  NEXT_CODE(280);
  m_myBrightness = pFiler->rdInt8();
  NEXT_CODE(280);
  m_myContrast = pFiler->rdInt8();
  NEXT_CODE(280);
  m_myFade = pFiler->rdInt8();
  NEXT_CODE(290);
  m_bOutOfDate = pFiler->rdBool();

  {
    //image data
    NEXT_CODE(90);
    m_nWidth = pFiler->rdInt32();
    NEXT_CODE(90);
    m_nHeight = pFiler->rdInt32();

    OdUInt32 nBytes = m_nWidth * m_nHeight * 4;

    m_PixelData.resize(0);
    if (nBytes)
    {
      m_PixelData.reserve(nBytes);
      OdBinaryData chunk;
      while (m_PixelData.size() < nBytes)
      {
        NEXT_CODE(310);
        pFiler->rdBinaryChunk(chunk);
        m_PixelData.append(chunk);
      }
    }
  }

  {
    //points array
    NEXT_CODE(90);
    m_arrVertices.setLogicalLength(pFiler->rdInt32());
    NEXT_CODE(90);
    m_arrVertices.setPhysicalLength(pFiler->rdInt32());
    NEXT_CODE(90);
    m_arrVertices.setGrowLength(pFiler->rdInt32());

    for (OdUInt32 i = 0; i < m_arrVertices.size(); ++i)
    {
      NEXT_CODE(10);
      pFiler->rdPoint3d(m_arrVertices[i]);
    }
  }

  NEXT_CODE(90);
  OdInt32 nStrings = pFiler->rdInt32();
  m_strings.resize(nStrings);
  for (OdInt32 i = 0; i < nStrings; ++i)
  {
    NEXT_CODE(1);
    m_strings[i] = pFiler->rdString();
  }
  m_textColor.dxfIn(pFiler, 1);
  NEXT_CODE(40);
  m_dTextHeight = pFiler->rdDouble();

  {
    //ClipBoundary
    NEXT_CODE(71);
    OdInt16 eClipBoundaryType = pFiler->rdInt16();

    if (eClipBoundaryType == kRect)
    {
      m_ptClipBnd.resize(2);
      NEXT_CODE(14);
      pFiler->rdPoint2d(m_ptClipBnd[0]);
      NEXT_CODE(14);
      pFiler->rdPoint2d(m_ptClipBnd[1]);
    }
    else if (eClipBoundaryType == kPoly)
    {
      NEXT_CODE(91);
      OdInt32 nClipPts = pFiler->rdInt32();
      m_ptClipBnd.resize(nClipPts);
      for (OdInt32 i = 0; i < nClipPts; ++i)
      {
        NEXT_CODE(14);
        pFiler->rdPoint2d(m_ptClipBnd[i]);
      }
      if (m_ptClipBnd.size() && m_ptClipBnd.first() != m_ptClipBnd.last())
      {
        m_ptClipBnd.append(m_ptClipBnd.first());
      }
    }
  }

  //  m_idGeoMapDef - not saved to DXF
  return eOk;
}


void OdDbGeoMap::dxfOutFields(OdDbDxfFiler* pFiler) const
{
  OdDbEntity::dxfOutFields(pFiler);

  pFiler->wrSubclassMarker(desc()->name());

  pFiler->wrInt16(70, 0); // classVersion
  pFiler->wrObjectId(330, m_UnkId);

  pFiler->wrPoint3d(10, m_ptImageBottomLeft);
  pFiler->wrPoint3d(10, m_ptTextPosition);

  pFiler->wrDouble(40, m_dImageWidth);
  pFiler->wrDouble(40, m_dImageHeight);

  pFiler->wrInt8(280, m_LOD);
  pFiler->wrUInt8(280, m_Resolution);
  pFiler->wrUInt8(280, m_MapType);

  pFiler->wrVector3d(10, m_vU);
  pFiler->wrVector3d(10, m_vV);

  pFiler->wrInt8(280, m_myBrightness);
  pFiler->wrInt8(280, m_myContrast);
  pFiler->wrInt8(280, m_myFade);
  pFiler->wrBool(290, m_bOutOfDate);

  {
    //image data
    pFiler->wrInt32(90, m_nWidth);
    pFiler->wrInt32(90, m_nHeight);

    pFiler->wrBinaryChunk(310, m_PixelData.getPtr(), m_PixelData.size());
  }

  {
    //points array
    pFiler->wrInt32(90, m_arrVertices.logicalLength());
    pFiler->wrInt32(90, m_arrVertices.physicalLength());
    pFiler->wrInt32(90, m_arrVertices.growLength());

    for (OdUInt32 i = 0; i < m_arrVertices.logicalLength(); ++i)
    {
      pFiler->wrPoint3d(10, m_arrVertices[i]);
    }
  }

  OdInt32 nStrings = m_strings.size();
  pFiler->wrInt32(90, nStrings);
  for (OdInt32 i = 0; i < nStrings; ++i)
  {
    pFiler->wrString(1, m_strings[i]);
  }

  m_textColor.dxfOut(pFiler, 1);

  pFiler->wrDouble(40, m_dTextHeight);

  {
    //ClipBoundary
    OdInt32 nClipPts = m_ptClipBnd.size();
    if (nClipPts == 2)
    {
      pFiler->wrInt16(71, kRect);
      pFiler->wrInt32(91, 2);
      pFiler->wrPoint2d(14, m_ptClipBnd[0]);
      pFiler->wrPoint2d(14, m_ptClipBnd[1]);
    }
    else if (nClipPts > 0)
    {
      pFiler->wrInt16(71, kPoly);
      pFiler->wrInt32(91, nClipPts);
      for (OdInt32 i = 0; i < nClipPts; ++i)
      {
        pFiler->wrPoint2d(14, m_ptClipBnd[i]);
      }
    }
    else
    {
      pFiler->wrInt16(71, kInvalid);
    }
  }

  //  m_idGeoMapDef - not saved to DXF
}

void OdDbGeoMap::composeForLoad(OdDb::SaveType format, OdDb::DwgVersion version, OdDbAuditInfo* pAuditInfo)
{
  // Def object is not saved to DXF and is recreated each time DXF is loaded.
  if (m_idGeoMapDef.isNull())
  {
    OdDbGeoMapDefPtr pDef = OdDbGeoMapDef::createObject();
    m_idGeoMapDef = database()->addOdDbObject(pDef, objectId());
  }
}

OdDbObjectPtr OdDbGeoMap::decomposeForSave(OdDb::DwgVersion ver, OdDbObjectId& replaceId, bool& exchangeXData)
{
  OdDbObjectPtr pObj = OdDbRasterImage::decomposeForSave(ver, replaceId, exchangeXData);
  if (m_idGeoMapDef.isNull())
  {
    OdDbGeoMapDefPtr pDef = OdDbGeoMapDef::createObject();
    m_idGeoMapDef = database()->addOdDbObject(pDef, objectId());
  }
  return pObj;
}

OdInt8 OdDbGeoMap::brightness() const
{
  assertReadEnabled();
  return m_myBrightness;
}

OdResult OdDbGeoMap::setBrightness(OdInt8 brightness)
{
  if (brightness < 0 || brightness > 100)
    return eInvalidInput;
  assertWriteEnabled();
  m_myBrightness = brightness;
  return eOk;
}

OdInt8 OdDbGeoMap::contrast() const
{
  assertReadEnabled();
  return m_myContrast;
}

OdResult OdDbGeoMap::setContrast(OdInt8 contrast)
{
  if (contrast < 0 || contrast > 100)
    return eInvalidInput;
  assertWriteEnabled();
  m_myContrast = contrast;
  return eOk;
}
OdInt8 OdDbGeoMap::fade() const
{
  assertReadEnabled();
  return m_myFade;
}

OdResult OdDbGeoMap::setFade(OdInt8 fade)
{
  if (fade < 0 || fade > 100)
    return eInvalidInput;
  assertWriteEnabled();
  m_myFade = fade;
  return eOk;
}

OdResult OdDbGeoMap::updateMapImage(bool bReset)
{
  assertWriteEnabled();

  OdDbGeoMapPEPtr ext = desc()->getX(OdDbGeoMapPE::desc());
  if (ext.isNull())
  {
    m_bOutOfDate = true;
    return eNoInterface;
  }

  return ext->updateMapImage(this, bReset);
}

OdGiRasterImagePtr OdDbGeoMap::image(bool /*load*/) const
{
  assertReadEnabled();
  updateMapImageIfNeeded();

  OdGiImageBGRA32 * pImg = (OdGiImageBGRA32 *) &m_image;
  pImg->setImage(m_nWidth, m_nHeight, (OdGiPixelBGRA32 *)m_PixelData.asArrayPtr());
  OdGiRasterImagePtr pImage = OdGiRasterImageRGBA32::createObject(pImg);

  // Flip it
  OdSmartPtr<OdGiUpsideDownRasterTransformer> pRet = OdRxObjectImpl<OdGiUpsideDownRasterTransformer>::createObject();
  pRet->setOriginal(pImage);

  return pRet;
}

bool OdDbGeoMap::subWorldDraw(OdGiWorldDraw* pWd) const
{
  OdGiRegenType regenType = pWd->regenType();
  if ( regenType == kOdGiForExtents
    || regenType == kOdGiSaveWorldDrawForProxy
    || !isSetDisplayOpt(kShow)
    || m_dragStatus == OdDb::kDragStart
    )
  {
    OdGePoint3dArray framePoints;
    getVertices(framePoints);
    pWd->geometry().polyline(framePoints.size(), framePoints.getPtr());
    return true;
  }
  return false; // go to viewportDraw, and paint image
}

bool loadFontForGeoMap(const OdString & sFont, double dTextHeight, OdDbDatabase * pDb, OdGiTextStyle & textStyle)
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

void drawProvidersStrings(OdGiViewportDraw* pVd, OdDbDatabase* pDb, const OdStringArray& arrCopyrightStrings, const OdGePoint3dArray& arrVertices, const OdGePoint3d& ptTextPosition, double dTextHeight, const OdCmColor& textColor)
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
        if (!loadFontForGeoMap(sSegoeUI, dTextHeightCorrected, pDb, textStyle))
        {
          OdString sTahoma(OD_T("Tahoma"));
          if (!loadFontForGeoMap(sTahoma, dTextHeightCorrected, pDb, textStyle))
          {
            OdDbHostAppServices* pSvc = pDb->appServices();
            if (!loadFontForGeoMap(pSvc->getAlternateFontName(), dTextHeightCorrected, pDb, textStyle))
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

void convertRectClipBoudary(const OdGePoint2dArray& arrInputClipBnd, OdGePoint2dArray& arrClipBnd)
{
  if (arrInputClipBnd.size() == 2) //kRect
  {
    const OdGePoint2d& min = arrInputClipBnd[0];
    const OdGePoint2d& max = arrInputClipBnd[1];
    arrClipBnd.resize(5);
    arrClipBnd[0] = arrClipBnd[4] = min;
    arrClipBnd[2] = max;
    arrClipBnd[1].set(max.x, min.y);
    arrClipBnd[3].set(min.x, max.y);
  }
  else
  {
    arrClipBnd = arrInputClipBnd;
  }
}

void OdDbGeoMap::subViewportDraw(OdGiViewportDraw* pVd) const
{
  assertReadEnabled();
  if (m_dragStatus != OdDb::kDragStart)
  {
    //Draw Image
    updateMapImageIfNeeded();
    if (!m_bOutOfDate)
    {
      OdGePoint2dArray arrClipBnd;
      convertRectClipBoudary(m_ptClipBnd, arrClipBnd);
      OdGiRasterImagePtr pRaster = image();      
      pVd->geometry().rasterImageDc(
        m_ptImageBottomLeft,
        m_vU,
        m_vV,
        pRaster,
        arrClipBnd.asArrayPtr(), arrClipBnd.size(),
        true,
        brightness(),
        contrast(),
        fade()
      );
    }
  }

  // Draw Frame
  {
    OdGePoint3dArray frame;
    getImageVertices(frame);
    pVd->geometry().polyline(frame.size(), frame.getPtr());
  }

  //Draw Text
  if(m_bOutOfDate)
  {
    return;
  }

  drawProvidersStrings(pVd, database(), m_strings, m_arrVertices, m_ptTextPosition, m_dTextHeight, m_textColor);
}

OdResult OdDbGeoMap::subGetGeomExtents(OdGeExtents3d& ext) const
{
  assertReadEnabled();
  ext = OdGeExtents3d();
  ext.addPoints(m_arrVertices);
  return eOk;
}

OdResult OdDbGeoMap::subTransformBy(const OdGeMatrix3d & xform)
{
  if (xform.det() < 0.)
  {
    return eNotApplicable;
  }
  if (xform.isSingular())
  {
    return eInvalidInput;
  }
  assertWriteEnabled();
  //note: z correction is needed
  m_ptImageBottomLeft.transformBy(xform);
  m_ptImageBottomLeft.z = 0.;

  m_vU.transformBy(xform);
  m_vV.transformBy(xform);

  double dLengthOld = 0.;
  if (m_arrVertices.logicalLength() == 4)
  {
    dLengthOld = (m_arrVertices[3] - m_arrVertices[0]).length();
  }

  for (OdUInt32 i = 0; i < m_arrVertices.logicalLength(); ++i)
  {
    m_arrVertices[i].transformBy(xform);
    m_arrVertices[i].z = 0.;
  }

  m_ptTextPosition.transformBy(xform);
  m_ptTextPosition.z = 0;

  if (m_arrVertices.logicalLength() == 4)
  {
    double dLengthNew = (m_arrVertices[3] - m_arrVertices[0]).length();
    if (!OdZero(dLengthOld, 1.e-6))
    {
      m_dTextHeight *= dLengthNew / dLengthOld;
    }
  }

  m_bOutOfDate = true;
  xDataTransformBy(xform);
  return eOk;
}

void OdDbGeoMap::getOrientation(OdGePoint3d& origin, OdGeVector3d& u, OdGeVector3d& v) const
{
  assertReadEnabled();
  origin = m_ptImageBottomLeft;
  u = m_vU * m_nWidth;
  v = m_vV * m_nHeight;
}

OdResult OdDbGeoMap::getImageVertices(OdGePoint3dArray& vertices) const
{
  assertReadEnabled();
  OdGePoint2dArray arrClipBnd;
  convertRectClipBoudary(m_ptClipBnd, arrClipBnd);
  OdGeMatrix3d mat = pixelToModelTransform(m_ptImageBottomLeft, m_vU, m_vV, m_nHeight);
  vertices.resize(arrClipBnd.size());
  for (OdUInt32 i = 0; i < arrClipBnd.size(); ++i)
  {
    vertices[i] = mat * OdGePoint3d(arrClipBnd[i].x, arrClipBnd[i].y, 0.);
  }
  return eOk;
}

double OdDbGeoMap::height() const
{
  assertReadEnabled();
  if (m_arrVertices.size() != 4)
  {
    return 0.;
  }
  return (m_arrVertices[3] - m_arrVertices[0]).length();
}

double OdDbGeoMap::width() const
{
  assertReadEnabled();
  if (m_arrVertices.size() != 4)
  {
    return 0.;
  }
  return (m_arrVertices[1] - m_arrVertices[0]).length();
}

OdGePoint3d OdDbGeoMap::bottomLeftPt() const
{
  assertReadEnabled();
  if (m_arrVertices.size() != 4)
  {
    return OdGePoint3d::kOrigin;
  }
  return m_arrVertices[0];
}

bool OdDbGeoMap::setOrientation(const OdGePoint3d & ptOrigin, const OdGeVector3d & u, const OdGeVector3d & v)
{
  //throw OdError("TODO: OdDbGeoMap::setOrientation MUST BE DELETED!");
  //return false;
  assertWriteEnabled();
  OdGePoint3d ptOriginXY(ptOrigin.x, ptOrigin.y, 0.);
  m_arrVertices.resize(4);
  m_arrVertices[0] = ptOriginXY;
  m_arrVertices[1] = ptOriginXY + u;
  m_arrVertices[2] = ptOriginXY + u + v;
  m_arrVertices[3] = ptOriginXY + v;
  m_bOutOfDate = true;
  return eOk;
}

OdResult OdDbGeoMap::subErase(bool bErasing)
{
  OdDbBlockTableRecordPtr pModelSpace = database()->getModelSpaceId().safeOpenObject();
  OdDbSortentsTablePtr pSortentsTable = pModelSpace->getSortentsTable(false);
  if(pSortentsTable.get())
  {
    pSortentsTable->removeField(id());
  }
  return OdDbRasterImage::subErase(bErasing);
}

void OdDbGeoMap::updateMapImageIfNeeded(bool bReset) const
{
  //trying to update image if needed
  if (m_bOutOfDate && m_dragStatus != OdDb::kDragStart)
  {
    OdDbGeoMap * pGeoMap = const_cast<OdDbGeoMap *>(this);
    pGeoMap->upgradeOpen();
    pGeoMap->updateMapImage(bReset);
    pGeoMap->downgradeOpen();
  }
}

void OdDbGeoMap::subClose()
{
  if (m_bOutOfDate
       && isModified()
       && !database()->isDatabaseLoading() // Load file as is
       && !isUndoing()
       && !isOdDbObjectIdsInFlux()) // Cloning
  {
    updateMapImage();
  }
}

void OdDbGeoMap::dragStatus( const OdDb::DragStat status )
{
  m_dragStatus = status;
}

////////////////////////////////////////////////////////////////////////////////////
//
//                  OdDbGeoMapDef
//
////////////////////////////////////////////////////////////////////////////////////
ODRX_DEFINE_MEMBERS_EX(OdDbGeoMapDef, OdDbRasterImageDef, DBOBJECT_CONSTR,
                       OdDb::vAC27, 81, OdDbProxyObject::kDisableProxyWarning,
                       L"AcDbGeoMapDef", L"AcDbGeoMapDef", L"AcGeolocationObj",
                       OdRx::kMTLoading | OdRx::kHistoryAware);


OdDbGeoMapDef::OdDbGeoMapDef()
  : OdDbRasterImageDef()
{}

OdDbGeoMapDef::~OdDbGeoMapDef()
{
}

OdResult OdDbGeoMapDef::dwgInFields(OdDbDwgFiler* pFiler)
{
  assertWriteEnabled();

  OdResult res = OdDbObject::dwgInFields(pFiler);
  if (res != eOk)
  {
    return res;
  }

  OdInt16 classVersion = pFiler->rdInt16();
  if (classVersion != 0)
  {
    ODA_FAIL_ONCE();
    return eMakeMeProxy;
  }

  OdDbObjectId id = pFiler->rdSoftPointerId();
  if (id != ownerId())
  {
    ODA_FAIL_ONCE();
    return eMakeMeProxy;
  }
  return eOk;
}


void OdDbGeoMapDef::dwgOutFields(OdDbDwgFiler* pFiler) const
{
  assertReadEnabled();
  OdDbObject::dwgOutFields(pFiler);

  pFiler->wrInt16(0); //classVersion
  pFiler->wrSoftPointerId(ownerId());
}


OdGiRasterImagePtr OdDbGeoMapDef::image(bool bReset)
{
  assertReadEnabled();
  OdDbGeoMapPtr pEnt = ownerId().safeOpenObject();
  return pEnt->image(bReset);
}
