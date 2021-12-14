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
#include "DbGeoMapFieldsPE.h"
#include "DbGeoMap.h"
#include "DbWipeout.h"
ODRX_CONS_DEFINE_MEMBERS(OdDbGeoMapFieldsPE, OdRxObject, RXIMPL_CONSTR);

//getters
OdInt8 OdDbGeoMapFieldsPE::getLOD(OdDbGeoMap * pGeoMap) const
{
  if (pGeoMap == NULL)
  {
    throw OdError(eNullPtr);
  }
  pGeoMap->assertReadEnabled();
  return pGeoMap->m_LOD;
}
bool OdDbGeoMapFieldsPE::getIsOutOfDate(OdDbGeoMap * pGeoMap) const
{
  if (pGeoMap == NULL)
  {
    throw OdError(eNullPtr);
  }
  pGeoMap->assertReadEnabled();
  return pGeoMap->m_bOutOfDate;
}
const OdBinaryData & OdDbGeoMapFieldsPE::getPixelData(OdDbGeoMap * pGeoMap) const
{
  if (pGeoMap == NULL)
  {
    throw OdError(eNullPtr);
  }
  pGeoMap->assertReadEnabled();
  return pGeoMap->m_PixelData;
}
const OdGePoint2dArray & OdDbGeoMapFieldsPE::getPtClipBnd(OdDbGeoMap * pGeoMap) const
{
  if (pGeoMap == NULL)
  {
    throw OdError(eNullPtr);
  }
  pGeoMap->assertReadEnabled();
  return pGeoMap->m_ptClipBnd;
}
const OdStringArray & OdDbGeoMapFieldsPE::getStrings(OdDbGeoMap * pGeoMap) const
{
  if (pGeoMap == NULL)
  {
    throw OdError(eNullPtr);
  }
  pGeoMap->assertReadEnabled();
  return pGeoMap->m_strings;
}
const OdGeVector3d & OdDbGeoMapFieldsPE::getvU(OdDbGeoMap * pGeoMap) const
{
  if (pGeoMap == NULL)
  {
    throw OdError(eNullPtr);
  }
  pGeoMap->assertReadEnabled();
  return pGeoMap->m_vU;
}
const OdGeVector3d & OdDbGeoMapFieldsPE::getvV(OdDbGeoMap * pGeoMap) const
{
  if (pGeoMap == NULL)
  {
    throw OdError(eNullPtr);
  }
  pGeoMap->assertReadEnabled();
  return pGeoMap->m_vV;
}
double OdDbGeoMapFieldsPE::getTextHeight(OdDbGeoMap * pGeoMap) const
{
  if (pGeoMap == NULL)
  {
    throw OdError(eNullPtr);
  }
  pGeoMap->assertReadEnabled();
  return pGeoMap->m_dTextHeight;
}

OdCmColor OdDbGeoMapFieldsPE::getTextColor(OdDbGeoMap * pGeoMap) const
{
  if (pGeoMap == NULL)
  {
    throw OdError(eNullPtr);
  }
  pGeoMap->assertReadEnabled();
  return pGeoMap->m_textColor;
}

OdInt32 OdDbGeoMapFieldsPE::getHeight(OdDbGeoMap * pGeoMap) const
{
  if (pGeoMap == NULL)
  {
    throw OdError(eNullPtr);
  }
  pGeoMap->assertReadEnabled();
  return pGeoMap->m_nHeight;
}
OdInt32 OdDbGeoMapFieldsPE::getWidth(OdDbGeoMap * pGeoMap) const
{
  if (pGeoMap == NULL)
  {
    throw OdError(eNullPtr);
  }
  pGeoMap->assertReadEnabled();
  return pGeoMap->m_nWidth;
}
double OdDbGeoMapFieldsPE::getImageWidth(OdDbGeoMap * pGeoMap) const
{
  if (pGeoMap == NULL)
  {
    throw OdError(eNullPtr);
  }
  pGeoMap->assertReadEnabled();
  return pGeoMap->m_dImageWidth;
}
double OdDbGeoMapFieldsPE::getImageHeight(OdDbGeoMap * pGeoMap) const
{
  if (pGeoMap == NULL)
  {
    throw OdError(eNullPtr);
  }
  pGeoMap->assertReadEnabled();
  return pGeoMap->m_dImageHeight;
}
const OdGePoint3d & OdDbGeoMapFieldsPE::getPtTextPosition(OdDbGeoMap * pGeoMap) const
{
  if (pGeoMap == NULL)
  {
    throw OdError(eNullPtr);
  }
  pGeoMap->assertReadEnabled();
  return pGeoMap->m_ptTextPosition;
}
const OdGePoint3d & OdDbGeoMapFieldsPE::getPtImageBottomLeft(OdDbGeoMap * pGeoMap) const
{
  if (pGeoMap == NULL)
  {
    throw OdError(eNullPtr);
  }
  pGeoMap->assertReadEnabled();
  return pGeoMap->m_ptImageBottomLeft;
}
OdInt8 OdDbGeoMapFieldsPE::getResolution(OdDbGeoMap * pGeoMap) const
{
  if (pGeoMap == NULL)
  {
    throw OdError(eNullPtr);
  }
  pGeoMap->assertReadEnabled();
  return pGeoMap->m_Resolution;
}

//setters
OdResult OdDbGeoMapFieldsPE::setLOD(OdDbGeoMap * pGeoMap, OdInt8 LOD) const
{
  if (pGeoMap == NULL)
  {
    return eNullPtr;
  }
  pGeoMap->assertWriteEnabled();
  pGeoMap->m_LOD = LOD;
  return eOk;
}
OdResult OdDbGeoMapFieldsPE::setIsOutOfDate(OdDbGeoMap * pGeoMap, bool bIsOutOfDate) const
{
  if (pGeoMap == NULL)
  {
    return eNullPtr;
  }
  pGeoMap->assertWriteEnabled();
  pGeoMap->m_bOutOfDate = bIsOutOfDate;
  return eOk;
}
OdResult OdDbGeoMapFieldsPE::setPixelData(OdDbGeoMap * pGeoMap, const OdBinaryData & pixelData) const
{
  if (pGeoMap == NULL)
  {
    return eNullPtr;
  }
  pGeoMap->assertWriteEnabled();
  pGeoMap->m_PixelData = pixelData;
  return eOk;
}
OdResult OdDbGeoMapFieldsPE::setPtClipBnd(OdDbGeoMap * pGeoMap, const OdGePoint2dArray & ptClipBnd) const
{
  if (pGeoMap == NULL)
  {
    return eNullPtr;
  }
  pGeoMap->assertWriteEnabled();
  pGeoMap->m_ptClipBnd = ptClipBnd;
  return eOk;
}
OdResult OdDbGeoMapFieldsPE::setStrings(OdDbGeoMap * pGeoMap, const OdStringArray & strings) const
{
  if (pGeoMap == NULL)
  {
    return eNullPtr;
  }
  pGeoMap->assertWriteEnabled();
  pGeoMap->m_strings = strings;
  return eOk;
}
OdResult OdDbGeoMapFieldsPE::setvU(OdDbGeoMap * pGeoMap,const OdGeVector3d & vU) const
{
  if (pGeoMap == NULL)
  {
    return eNullPtr;
  }
  pGeoMap->assertWriteEnabled();
  pGeoMap->m_vU = vU;
  return eOk;
}
OdResult OdDbGeoMapFieldsPE::setvV(OdDbGeoMap * pGeoMap, const OdGeVector3d & vV) const
{
  if (pGeoMap == NULL)
  {
    return eNullPtr;
  }
  pGeoMap->assertWriteEnabled();
  pGeoMap->m_vV = vV;
  return eOk;
}
OdResult OdDbGeoMapFieldsPE::setTextHeight(OdDbGeoMap * pGeoMap, double dTextHeight) const
{
  if (pGeoMap == NULL)
  {
    return eNullPtr;
  }
  pGeoMap->assertWriteEnabled();
  pGeoMap->m_dTextHeight = dTextHeight;
  return eOk;
}
OdResult OdDbGeoMapFieldsPE::setTextColor(OdDbGeoMap * pGeoMap, const OdCmColor& cColor) const
{
  if (pGeoMap == NULL)
  {
    return eNullPtr;
  }
  pGeoMap->assertWriteEnabled();
  pGeoMap->m_textColor = cColor;
  return eOk;
}
OdResult OdDbGeoMapFieldsPE::setHeight(OdDbGeoMap * pGeoMap, OdInt32 height) const
{
  if (pGeoMap == NULL)
  {
    return eNullPtr;
  }
  pGeoMap->assertWriteEnabled();
  pGeoMap->m_nHeight = height;
  return eOk;
}
OdResult OdDbGeoMapFieldsPE::setWidth(OdDbGeoMap * pGeoMap, OdInt32 width) const
{
  if (pGeoMap == NULL)
  {
    return eNullPtr;
  }
  pGeoMap->assertWriteEnabled();
  pGeoMap->m_nWidth = width;
  return eOk;
}
OdResult OdDbGeoMapFieldsPE::setImageWidth(OdDbGeoMap * pGeoMap, double dImageWidth) const
{
  if (pGeoMap == NULL)
  {
    return eNullPtr;
  }
  pGeoMap->assertWriteEnabled();
  pGeoMap->m_dImageWidth = dImageWidth;
  return eOk;
}
OdResult OdDbGeoMapFieldsPE::setImageHeight(OdDbGeoMap * pGeoMap, double dImageHeight) const
{
  if (pGeoMap == NULL)
  {
    return eNullPtr;
  }
  pGeoMap->assertWriteEnabled();
  pGeoMap->m_dImageHeight = dImageHeight;
  return eOk;
}
OdResult OdDbGeoMapFieldsPE::setPtTextPosition(OdDbGeoMap * pGeoMap, const OdGePoint3d & ptTextPosition) const
{
  if (pGeoMap == NULL)
  {
    return eNullPtr;
  }
  pGeoMap->assertWriteEnabled();
  pGeoMap->m_ptTextPosition = ptTextPosition;
  return eOk;
}
OdResult OdDbGeoMapFieldsPE::setPtImageBottomLeft(OdDbGeoMap * pGeoMap, const OdGePoint3d & ptImageBottomLeft) const
{
  if (pGeoMap == NULL)
  {
    return eNullPtr;
  }
  pGeoMap->assertWriteEnabled();
  pGeoMap->m_ptImageBottomLeft = ptImageBottomLeft;
  return eOk;
}
OdResult OdDbGeoMapFieldsPE::setResolution(OdDbGeoMap * pGeoMap, OdInt8 Resolution) const
{
  if (pGeoMap == NULL)
  {
    return eNullPtr;
  }
  pGeoMap->assertWriteEnabled();
  pGeoMap->m_Resolution = Resolution;
  return eOk;
}

//////////////////////////////////////////
// OdDbGeoMapGripPointsPE
//////////////////////////////////////////

OdResult OdDbGeoMapGripPointsPE::getGripPoints(const OdDbEntity* ent, OdGePoint3dArray& gripPoints)const
{
  OdDbRasterImagePtr pImg = ent;
  OdGeExtents3d exts;
  OdResult res = pImg->getGeomExtents(exts);
  if (eOk == res)
  {
    OdGePoint3dArray clipPoints;
    pImg->getVertices(clipPoints);
    if (clipPoints.last() == clipPoints.first())
    {
      clipPoints.removeLast();
    }
    gripPoints.append(clipPoints);
  }
  return res;
}

OdResult OdDbGeoMapGripPointsPE::moveGripPointsAt(OdDbEntity* pEnt, const OdIntArray& indices, const OdGeVector3d& offset)
{
  OdDbGeoMapPtr pImg = pEnt;
  unsigned size = indices.size();
  if (size == 0)
  {
    return eOk;
  }
  if (indices.size() != 1 || indices[0] > 3)
  {
    return eNotApplicable;
  }

  double dWidth = pImg->width();
  double dHeight = pImg->height();
  if (OdZero(dWidth, 1.e-6) || OdZero(dHeight, 1.e-6))
  {
    return eNotApplicable;
  }
  OdGePoint3dArray arrVertices;
  pImg->getVertices(arrVertices);
  OdGeMatrix3d mat;
  mat.setCoordSystem(
    arrVertices[(indices[0] + 2) % 4],
    (arrVertices[1] - arrVertices[0]).normal(),
    (arrVertices[3] - arrVertices[0]).normal(),
    OdGeVector3d::kZAxis);

  OdGeMatrix3d matInverse = mat.inverse();
  OdGeVector3d vOffset = offset;
  vOffset.transformBy(matInverse);

  double dX = vOffset.x;
  if (indices[0] == 0 || indices[0] == 3)
  {
    dX = -vOffset.x;
  }

  double dY = vOffset.y;
  if (indices[0] <= 1)
  {
    dY = -vOffset.y;
  }
  dX = fabs((dX + dWidth) / dWidth);
  dY = fabs((dY + dHeight) / dHeight);

  if (OdZero(dX, 1.e-6) && OdZero(dY, 1.e-6))
  {
    return eNotApplicable;
  }

  OdGeMatrix3d matScale;
  matScale[0][0] = dX;
  matScale[1][1] = dY;
  pImg->transformBy((mat * matScale) * matInverse);
  return eOk;
}

OdResult OdDbGeoMapGripPointsPE::getStretchPoints(const OdDbEntity* ent, OdGePoint3dArray& stretchPoints) const
{
  return getGripPoints(ent, stretchPoints);
}

OdResult OdDbGeoMapGripPointsPE::moveStretchPointsAt(OdDbEntity* ent, const OdIntArray& indices, const OdGeVector3d& offset)
{
  return moveGripPointsAt(ent, indices, offset);
}

OdResult OdDbGeoMapGripPointsPE::getOsnapPoints(const OdDbEntity* ent, OdDb::OsnapMode osnapMode, OdGsMarker gsSelectionMark, const OdGePoint3d& pickPoint, const OdGePoint3d& lastPoint, const OdGeMatrix3d& viewXform, OdGePoint3dArray& snapPoints) const
{
  switch (osnapMode)
  {
  case OdDb::kOsModeEnd:
  case OdDb::kOsModeCen:
  {
    OdGePoint3dArray gripPoints;
    getGripPoints(ent, gripPoints);
    if (gripPoints.size() > 0)
    {
      if (osnapMode == OdDb::kOsModeEnd)
      {
        gripPoints.erase(gripPoints.begin());
      }
      else
      {
        gripPoints.erase(gripPoints.begin() + 1, gripPoints.end());
      }
      snapPoints.append(gripPoints);
    }
  }
  break;
  default:
    break;
  }
  return eOk;
}
