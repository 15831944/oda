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

#ifndef _ODDBGEOMAP_INCLUDED_
#define _ODDBGEOMAP_INCLUDED_ /** {Secret} */

#include "TD_PackPush.h"

#include "DbGeolocationObjExports.h"
#include "DbRasterImage.h"

#include "OdBinaryData.h"
#include "StringArray.h"
#include "Gi/GiRasterWrappers.h"

enum OdGeoMapType
{
  kNoMap = 0,
  kAerial = 1,
  kRoad = 2,
  kHybrid = 3
};


enum OdGeoMapResolution
{
  kCoarse = -1,
  kOptimal = 0,
  kFine = 1,
  kFiner = 2
};



class DBGEOLOCATIONOBJ_EXPORT OdDbGeoMap : public OdDbRasterImage
{
public:
  ODDB_DECLARE_MEMBERS(OdDbGeoMap);

  OdDbGeoMap();
  virtual ~OdDbGeoMap();

  OdGePoint3d         bottomLeftPt() const;
  double              height() const;
  double              width() const;
  OdGePoint3d         imageBottomLeftPt() const;
  double              imageHeight() const;
  double              imageWidth() const;
  OdGeoMapResolution  resolution() const;
  OdResult            setResolution(OdGeoMapResolution resolution);
  OdInt8              LOD() const;
  OdGeoMapType        mapType() const;
  OdResult            setMapType(OdGeoMapType mapType);
  bool                isOutOfDate() const;
  OdResult            updateMapImage(bool bReset = false);

  OdResult    dwgInFields(OdDbDwgFiler* pFiler);
  void        dwgOutFields(OdDbDwgFiler* pFiler) const;
  OdResult    dxfInFields(OdDbDxfFiler* pFiler);
  void        dxfOutFields(OdDbDxfFiler* pFiler) const;

  OdDbObjectPtr decomposeForSave(OdDb::DwgVersion ver, OdDbObjectId& replaceId, bool& exchangeXData);
  void composeForLoad(OdDb::SaveType format, OdDb::DwgVersion version, OdDbAuditInfo* pAuditInfo);
  OdResult subErase(bool bErasing);
  void subClose();

//  OdResult applyPartialUndo(OdDbDwgFiler* undoFiler, OdRxClass*  classObj);

  OdResult  subTransformBy(const OdGeMatrix3d& xform);

  // OdDbRasterImage
  OdDbObjectId imageDefId() const;

  OdResult getVertices(OdGePoint3dArray& vertices) const;
  OdResult getImageVertices(OdGePoint3dArray& vertices) const;
  OdGeVector2d imageSize(bool bGetCachedValue = false) const;
  const OdGePoint2dArray& clipBoundary() const;

  OdResult  setBrightness(OdInt8 value);
  OdInt8    brightness() const;

  OdResult  setContrast(OdInt8 value);
  OdInt8    contrast() const;

  OdResult  setFade(OdInt8 value);
  OdInt8    fade() const;

  virtual OdGiRasterImagePtr image(bool load = true) const;

  void getOrientation(OdGePoint3d& origin, OdGeVector3d& u, OdGeVector3d& v) const;

  bool setOrientation(const OdGePoint3d & ptOrigin, const OdGeVector3d & u, const OdGeVector3d & v);

  void dragStatus(const OdDb::DragStat);
protected:
  bool subWorldDraw(OdGiWorldDraw* pWorldDraw) const;
  void subViewportDraw(OdGiViewportDraw* pViewportDraw) const;
  
  OdResult subGetGeomExtents(OdGeExtents3d& extents) const;

  void updateMapImageIfNeeded(bool bReset = false) const;

private:
  OdDbObjectId m_UnkId;
  OdGePoint3d m_ptImageBottomLeft;
  OdGePoint3d m_ptTextPosition;
  double m_dImageWidth;
  double m_dImageHeight;
  OdInt8 m_LOD;
  OdInt8 m_Resolution;
  OdInt8 m_MapType;
  OdGeVector3d m_vU;
  OdGeVector3d m_vV;

  OdInt8 m_myBrightness;
  OdInt8 m_myContrast;
  OdInt8 m_myFade;

  bool m_bOutOfDate;
  OdInt32 m_nHeight;
  OdInt32 m_nWidth;
  OdBinaryData m_PixelData;

  
  OdGePoint3dArray m_arrVertices;//m_ptBottomLeft, m_ptBottomRight, m_ptUpperRight, m_ptUpperLeft

  OdStringArray m_strings;

  OdCmColor m_textColor;
  double  m_dTextHeight;

  OdGePoint2dArray m_ptClipBnd;

  OdDbObjectId m_idGeoMapDef;

  OdDb::DragStat  m_dragStatus;
  OdGiImageBGRA32 m_image;
  friend class OdDbGeoMapFieldsPE;
};

typedef OdSmartPtr<OdDbGeoMap> OdDbGeoMapPtr;

///////////////////////////////////////////////////////////////////

class DBGEOLOCATIONOBJ_EXPORT OdDbGeoMapDef : public OdDbRasterImageDef
{
public:
  ODDB_DECLARE_MEMBERS(OdDbGeoMapDef);

  OdDbGeoMapDef();
  virtual ~OdDbGeoMapDef();

  virtual OdGiRasterImagePtr image(bool load = true);

  OdResult    dwgInFields(OdDbDwgFiler* pFiler);
  void        dwgOutFields(OdDbDwgFiler* pFiler) const;
};

typedef OdSmartPtr<OdDbGeoMapDef> OdDbGeoMapDefPtr;

#include "TD_PackPop.h"

#endif
