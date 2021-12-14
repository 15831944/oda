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

#include "StdAfx.h"
#include "DbGeoPositionMarkerGripPoints.h"
#include "DbGeoPositionMarker.h"
#include "Ge/GeCircArc3d.h"
#include "DbViewTableRecord.h"
#include <cfloat>

OdResult OdDbGeoPositionMarkerPE::getGripPoints(const OdDbEntity* pEntity, OdGePoint3dArray& gripPoints) const
{
  OdDbGeoPositionMarkerPtr pMarker = pEntity;
  gripPoints.reserve(2);
  gripPoints.append(pMarker->position());
  OdDbMTextPtr pMText = pMarker->mtext();
  if (!pMText.isNull())
  {
    OdGePoint3d closestPoint;
    OdGePoint3dArray framePoints;
    pMText->getActualBoundingPoints(framePoints, pMarker->landingGap(), pMarker->landingGap());
    framePoints.append(framePoints[0]);
    framePoints.swap(2, 3);

    double module = DBL_MAX;
    for(int i = 0; i < 4; ++i)
    {
      
      OdGePoint3d pntMiddle = (framePoints[i] + framePoints[i+1].asVector()) / 2.;
      if (module >= pMarker->position().distanceTo(pntMiddle))
      {
        closestPoint = pntMiddle;
        module = pMarker->position().distanceTo(pntMiddle);
      }
    }
    gripPoints.append(closestPoint);
  }
  return eOk;
}

OdResult OdDbGeoPositionMarkerPE::moveGripPointsAt(OdDbEntity* pEntity, const OdIntArray& indices, const OdGeVector3d& offset)
{
  OdDbGeoPositionMarkerPtr pMarker = pEntity;
  for(unsigned int i = 0; i < indices.length(); ++i)
  {
    switch (indices[i])
    { 
    case 0:
      {
        OdDbMTextPtr pMTextOld = pMarker->mtext();
        pMarker->setPosition(pMarker->position() + offset);
        pMarker->setMText(pMTextOld);
      }
      break;
    case 1:
      {
        OdDbMTextPtr pMText = pMarker->mtext();
        pMText->setLocation(pMText->location() + offset);
        pMarker->setMText(pMText);
      }
      break;
    default:
      return eNotImplementedYet;
      break;
    }
  }
  return eOk;
}

OdResult OdDbGeoPositionMarkerPE::getStretchPoints(const OdDbEntity* pEntity, OdGePoint3dArray& stretchPoints) const
{
  return eNotImplementedYet;
}

OdResult OdDbGeoPositionMarkerPE::moveStretchPointsAt(OdDbEntity* pEntity, const OdIntArray& indices, const OdGeVector3d& offset)
{
  return eOk;
}

OdResult OdDbGeoPositionMarkerPE::getOsnapPoints(
    const OdDbEntity* pEntity, 
    OdDb::OsnapMode osnapMode, 
    OdGsMarker gsSelectionMark, 
    const OdGePoint3d& pickPoint,
    const OdGePoint3d& lastPoint, 
    const OdGeMatrix3d& xWorldToEye, 
    OdGePoint3dArray& snapPoints) const
{
  OdDbGeoPositionMarkerPtr pMarker = pEntity;

  switch (osnapMode)
  {
  case OdDb::kOsModeEnd:
    if(!pMarker->mtext().isNull())
    {
      snapPoints.reserve(2);
      OdGePoint3dArray gripPoints;
      pMarker->getGripPoints(gripPoints);
      snapPoints.append(gripPoints[1]);
      OdGeCircArc3d circle(pMarker->position(), pMarker->normal(), pMarker->radius());
      snapPoints.append(circle.closestPointTo(gripPoints[1]));
    }
    break;
  case OdDb::kOsModeMid:
    break;
  case OdDb::kOsModeCen:
    break;
  case OdDb::kOsModeNode:
    {
      if(!pMarker->mtext().isNull())
      {
        snapPoints.reserve(5);
        pMarker->mtext()->getBoundingPoints(snapPoints);
      }
      snapPoints.append(pMarker->position());
    }
    break;
  case OdDb::kOsModeQuad:
  case OdDb::kOsModeIntersec:
  case OdDb::kOsModeIns:
  case OdDb::kOsModePerp:
  case OdDb::kOsModeTan:
  case OdDb::kOsModeNear:
  case OdDb::kOsModeApint:
  case OdDb::kOsModePar:
  case OdDb::kOsModeStart:
  default:
    break;
  }

  return eOk;
}
