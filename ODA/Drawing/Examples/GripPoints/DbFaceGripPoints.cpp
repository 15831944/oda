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

/************************************************************************/
/* This class is an implementation of the OdDbGripPointsPE class for    */
/* OdDbFace entities.                                                   */
/************************************************************************/
#include "StdAfx.h"
#include "RxObjectImpl.h"
#include "DbFaceGripPoints.h"
#include "OdGripPointsModule.h"
//#include "Ge/GePointOnCurve3d.h"

OdResult OdDbFaceGripPointsPE::getGripPoints( const OdDbEntity* ent, OdGePoint3dArray& gripPoints )const
{
    unsigned int nSize = gripPoints.size();
    gripPoints.resize(nSize + 4);
    OdDbFacePtr pFace = ent;
  
    for (unsigned int i = 0; i < 4; i++)
    {
      pFace->getVertexAt(i, gripPoints[nSize + i]);
    }
    return eOk;
}

OdResult OdDbFaceGripPointsPE::moveGripPointsAt(OdDbEntity* pEnt, const OdIntArray& indices, const OdGeVector3d& vOffset)
{
  unsigned int nSize = indices.size();
  if (nSize == 0)
    return eOk;

  OdGeVector3d offset(vOffset);
  OdDbFacePtr pFace = pEnt;

  OdGePlane       plane;
  OdDb::Planarity planarity;

  pFace->getPlane(plane, planarity);
  
  if (!projectOffset(pFace->database(), plane.normal(), offset)) // Project offset on entity's plane in view direction
    return eOk;

  OdGePoint3d point;
  for (unsigned int i = 0; i < nSize; i++)
  {
    pFace->getVertexAt(indices[i], point);
    pFace->setVertexAt(indices[i], point + offset);
  }
  return eOk;
}

OdResult OdDbFaceGripPointsPE::getStretchPoints( const OdDbEntity* ent, OdGePoint3dArray& stretchPoints ) const
{
  return getGripPoints( ent, stretchPoints );
}

OdResult OdDbFaceGripPointsPE::moveStretchPointsAt( OdDbEntity* ent, const OdIntArray& indices, const OdGeVector3d& offset )
{
  return moveGripPointsAt( ent, indices, offset );
}

OdResult OdDbFaceGripPointsPE::getOsnapPoints(const OdDbEntity* ent, 
                                              OdDb::OsnapMode osnapMode, 
                                              OdGsMarker gsSelectionMark, 
                                              const OdGePoint3d& pickPoint_,
                                              const OdGePoint3d& lastPoint_, 
                                              const OdGeMatrix3d& xWorldToEye, 
                                              OdGePoint3dArray& snapPoints) const
{
  OdGePoint3dArray gripPoints;
  OdResult res = getGripPoints(ent, gripPoints);
  unsigned int nSize;
  if (res != eOk || (nSize = gripPoints.size()) < 4)
    return res;

  OdDbFacePtr pFace = ent;

  OdGePoint3d pickPoint = getPlanePoint(pFace, pickPoint_),  // recalculated pickPoint and lastPoint
              lastPoint = getPlanePoint(pFace, lastPoint_);  // in plane of face

  OdGePoint3d start, end, mid;

  switch (osnapMode)
  {
  case OdDb::kOsModeEnd:
    for (unsigned int i = 0; i < nSize; i++) snapPoints.append(gripPoints[i]);
    break;

  case OdDb::kOsModeMid:
    for (unsigned int i = 0; i < nSize; i++)
    {
       unsigned int j = i + 1 - nSize * ((i + 1) / nSize);
       start = gripPoints[i];
       end = gripPoints[i + 1 - nSize * ((i + 1) / nSize)];
       if (!gripPoints[i].isEqualTo(gripPoints[j]))
       {
          mid = gripPoints[i] + (gripPoints[j] - gripPoints[i]) / 2;
          snapPoints.append(mid);
       }
    }
    break;

  case OdDb::kOsModePerp:
    for (unsigned int i = 0; i < nSize; i++)
    {
       unsigned int j = i + 1 - nSize * ((i + 1) / nSize);
       start = gripPoints[i];
       end = gripPoints[i + 1 - nSize * ((i + 1) / nSize)];
       if (!gripPoints[i].isEqualTo(gripPoints[j]))
       {
          OdGeLine3d l(start, end);
          OdGePlane perpPlane;
          l.getPerpPlane(lastPoint, perpPlane);
          if (perpPlane.intersectWith(l, mid))
          {
             snapPoints.append(mid);
          }
         
          /* using getNormalPoint() - not implemernted yet
          OdGeLine3d l(start, end);
          OdGePointOnCurve3d perpPntOnCrv(l);
          OdGePointOnCurve3d perpPntOnCrv;
          if (l.getNormalPoint(lastPoint, perpPntOnCrv))
          {
            snapPoints.append(perpPntOnCrv.point3d());
          }*/
       }
    }
    break;

  case OdDb::kOsModeNear:
    for (unsigned int i = 0; i < nSize; i++)
    {
       start = gripPoints[i];
       end = gripPoints[i + 1 - nSize * ((i + 1) / nSize)];

       if (!start.isEqualTo(end))
       {
          OdGeLine3d l(start, end);
          double p = l.paramOf(pickPoint);
          if (p > 1)
            snapPoints.append(end);
          else if (p < 0)
            snapPoints.append(start);
          else
            snapPoints.append(l.evalPoint(p));
        }
        else
        {
           snapPoints.append(start);
        }
    }
    break;

  default:
    break;
  }

  return eOk;
}
