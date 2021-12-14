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
#ifndef _OD_DB_GEOMAPFIELDS_PE_
#define _OD_DB_GEOMAPFIELDS_PE_

#include "RxObject.h"
#include "SmartPtr.h"
#include "DbGeolocationObjExports.h"

#include "Ge/GePoint3d.h"
#include "Gi/GiRasterImage.h"
#include "DbGeoMap.h"
#include "DbGripPoints.h"
#include "TD_PackPush.h"

class DBGEOLOCATIONOBJ_EXPORT OdDbGeoMapFieldsPE : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdDbGeoMapFieldsPE);
  virtual ~OdDbGeoMapFieldsPE() {};

  virtual OdInt8 getLOD(OdDbGeoMap * pGeoMap) const;
  virtual bool getIsOutOfDate(OdDbGeoMap * pGeoMap) const;
  virtual const OdBinaryData & getPixelData(OdDbGeoMap * pGeoMap) const;
  virtual const OdGePoint2dArray & getPtClipBnd(OdDbGeoMap * pGeoMap) const;
  virtual const OdStringArray & getStrings(OdDbGeoMap * pGeoMap) const;
  virtual const OdGeVector3d & getvU(OdDbGeoMap * pGeoMap) const;
  virtual const OdGeVector3d & getvV(OdDbGeoMap * pGeoMap) const;
  virtual double getTextHeight(OdDbGeoMap * pGeoMap) const;
  virtual OdCmColor getTextColor(OdDbGeoMap * pGeoMap) const;
  virtual OdInt32 getHeight(OdDbGeoMap * pGeoMap) const;
  virtual OdInt32 getWidth(OdDbGeoMap * pGeoMap) const;
  virtual double getImageWidth(OdDbGeoMap * pGeoMap) const;
  virtual double getImageHeight(OdDbGeoMap * pGeoMap) const;
  virtual const OdGePoint3d & getPtTextPosition(OdDbGeoMap * pGeoMap) const;
  virtual const OdGePoint3d & getPtImageBottomLeft(OdDbGeoMap * pGeoMap) const;
  virtual OdInt8 getResolution(OdDbGeoMap * pGeoMap) const;

  virtual OdResult setLOD(OdDbGeoMap * pGeoMap, OdInt8 LOD) const;
  virtual OdResult setIsOutOfDate(OdDbGeoMap * pGeoMap, bool bIsOutOfDate) const;
  virtual OdResult setPixelData(OdDbGeoMap * pGeoMap, const OdBinaryData & pixelData) const;
  virtual OdResult setPtClipBnd(OdDbGeoMap * pGeoMap, const OdGePoint2dArray & ptClipBnd) const;
  virtual OdResult setStrings(OdDbGeoMap * pGeoMap, const OdStringArray & strings) const;
  virtual OdResult setvU(OdDbGeoMap * pGeoMap,const OdGeVector3d & vU) const;
  virtual OdResult setvV(OdDbGeoMap * pGeoMap, const OdGeVector3d & vV) const;
  virtual OdResult setTextHeight(OdDbGeoMap * pGeoMap, double dTextHeight) const;
  virtual OdResult setTextColor(OdDbGeoMap * pGeoMap, const OdCmColor& cColor) const;
  virtual OdResult setHeight(OdDbGeoMap * pGeoMap, OdInt32 height) const;
  virtual OdResult setWidth(OdDbGeoMap * pGeoMap, OdInt32 width) const;
  virtual OdResult setImageWidth(OdDbGeoMap * pGeoMap, double dImageWidth) const;
  virtual OdResult setImageHeight(OdDbGeoMap * pGeoMap, double dImageHeight) const;
  virtual OdResult setPtTextPosition(OdDbGeoMap * pGeoMap, const OdGePoint3d & ptTextPosition) const;
  virtual OdResult setPtImageBottomLeft(OdDbGeoMap * pGeoMap, const OdGePoint3d & ptImageBottomLeft) const;
  virtual OdResult setResolution(OdDbGeoMap * pGeoMap, OdInt8 Resolution) const;
};

typedef OdSmartPtr<OdDbGeoMapFieldsPE> OdDbGeoMapFieldsPEPtr;

class OdDbGeoMapGripPointsPE : public OdDbGripPointsPE
{
public:
  virtual OdResult getGripPoints(const OdDbEntity* pEntity, OdGePoint3dArray& gripPoints) const;
  virtual OdResult moveGripPointsAt(OdDbEntity* pEntity, const OdIntArray& indices, const OdGeVector3d& offset);
  virtual OdResult getStretchPoints(const OdDbEntity* pEntity, OdGePoint3dArray& stretchPoints) const;
  virtual OdResult moveStretchPointsAt(OdDbEntity* pEntity, const OdIntArray& indices, const OdGeVector3d& offset);
  virtual OdResult getOsnapPoints(const OdDbEntity* ent, OdDb::OsnapMode osnapMode, OdGsMarker gsSelectionMark,
    const OdGePoint3d& pickPoint, const OdGePoint3d& lastPoint, const OdGeMatrix3d& viewXform,
    OdGePoint3dArray& snapPoints) const;
};

#include "TD_PackPop.h"

#endif //_OD_DB_GEOMAPFIELDS_PE_
