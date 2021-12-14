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
/* OdDbCircle entities.                                                 */
/************************************************************************/
#include "StdAfx.h"
#include "RxObjectImpl.h"
#include "DbCircleGripPoints.h"
#include "OdGripPointsModule.h"

//----------------------------------------------------------------------------------------------------------------------------
// Returns 5 Points: center + 4 points on Circle

OdResult OdDbCircleGripPointsPE::getGripPoints( const OdDbEntity* ent, OdGePoint3dArray& gripPoints )const
{
  OdDbCirclePtr circle = ent;
  unsigned int size = gripPoints.size();
  gripPoints.resize(size + 5);

  gripPoints[size + 0] = circle->center();                            // 0 - center

  circle->getPointAtParam(    0,  gripPoints[size + 1] );             // 1 - right  (0)
  circle->getPointAtParam(OdaPI,  gripPoints[size + 2] );             // 2 - left   (pi)
  circle->getPointAtParam(OdaPI2, gripPoints[size + 3] );             // 3 - top    (pi/2)
  circle->getPointAtParam(OdaPI + OdaPI2, gripPoints[size + 4] );     // 4 - bottom (pi + pi/2)

  if (!OdZero(circle->thickness()))
  {
    OdGeVector3d vExtrusion = circle->normal() * circle->thickness();

    for(int i=0; i<5; i++)
      gripPoints.append(gripPoints[size + i] + vExtrusion);
  }

  return eOk;
}
//----------------------------------------------------------------------------------------------------------------------------
// Move circle or change radius

OdResult OdDbCircleGripPointsPE::moveGripPointsAt(OdDbEntity* pEnt, const OdIntArray& indices, const OdGeVector3d& vOffset)
{
  if ( indices.size() == 0 )                                          // indices[0] - defines for what point we pull: 
    return eOk;                                                       //              center or other

  OdDbCirclePtr circle = pEnt;
  bool          bCenter = true;
  OdGeVector3d  offset(vOffset);
                                                                      
  if (!projectOffset(circle->database(), circle->normal(), offset))   // Project offset on entity's plane in view direction
  { 
    circle->setCenter( circle->center() + offset );                   // View direction is perpendicular to normal
    return eOk;                                                       // Move the circle
  }

  for(unsigned i=0; i<indices.size(); i++)
  {
      if (indices[i]%5 == 0)                                          // point center - move circle
      {
        if (bCenter)                                                  // move center only one time
        {
          circle->setCenter( circle->center() + offset );
          bCenter = false;
        }
      }
      else                                                            // change radius
      {                                                               
        OdGePoint3dArray gripPoints;
        circle->getGripPoints(gripPoints);

        OdGePoint3d point = gripPoints[ indices[i]%5 ] + offset;

        circle->setRadius(circle->center().distanceTo(point));
      }
  }
  return eOk;
}
//----------------------------------------------------------------------------------------------------------------------------
// Cannot be stretched

OdResult OdDbCircleGripPointsPE::getStretchPoints( const OdDbEntity* pEnt, OdGePoint3dArray& stretchPoints ) const
{
  OdDbCirclePtr circle = pEnt;

  stretchPoints.append(circle->center());                             // center

  if (!OdZero(circle->thickness()))                                   // next center
  {
    stretchPoints.append(circle->center() + circle->normal()*circle->thickness());              
  }
  return eOk;
}

OdResult OdDbCircleGripPointsPE::moveStretchPointsAt( OdDbEntity* pEnt, const OdIntArray& indices_, const OdGeVector3d& offset )
{
  OdDbCirclePtr circle = pEnt;

  circle->setCenter(circle->center() + offset );

  return eOk;
}
//----------------------------------------------------------------------------------------------------------------------------
// Return snap Points into snapPoints, depending on type osnapMode

OdResult OdDbCircleGripPointsPE::getOsnapPoints(const OdDbEntity* ent, 
                                                OdDb::OsnapMode osnapMode,           
                                                OdGsMarker gsSelectionMark, 
                                                const OdGePoint3d& pickPoint_,        // Point, which moves
                                                const OdGePoint3d& lastPoint_,        // Point, from which draw line
                                                const OdGeMatrix3d& xWorldToEye, 
                                                OdGePoint3dArray& snapPoints) const
{
  OdGePoint3dArray gripPoints;
  OdResult res = getGripPoints( ent, gripPoints );
  if ( res != eOk  || gripPoints.size()<5) 
    return res;

  OdDbCirclePtr circle    = ent;
  OdGePoint3d   pickPoint = getPlanePoint(circle, pickPoint_),  // recalculated pickPoint and lastPoint
                lastPoint = getPlanePoint(circle, lastPoint_);  // in plane of circle

  OdGePoint3d center = circle->center();
  double      radius = circle->radius();

  OdGePoint3d ptPick = pickPoint - center.asVector();
  double      rdPick = pickPoint.distanceTo(center);

  OdGePoint3d ptLast = lastPoint - center.asVector();
  double      rdLast = lastPoint.distanceTo(center);

  bool bThickness = !OdZero(circle->thickness());
  OdGeVector3d vThickness = circle->normal()*circle->thickness();

  switch ( osnapMode )
  {
    case OdDb::kOsModeCen:                   // Center: draw cross
      for(unsigned i=0; i<gripPoints.size(); i +=5)
        snapPoints.append(gripPoints[i]);
      break;

    case OdDb::kOsModeQuad:                  // Quadrant: cursor as square turned on 45 degrees
      for(unsigned i=1; i<gripPoints.size(); i++)
      {
        if (i==5) continue;
        snapPoints.append(gripPoints[i]);
      }
      break;

    case OdDb::kOsModePerp:                  // Perpendicular: cursor as angle 90
      if (rdLast > 0)
      {
        OdGePoint3d Point = ptLast*radius/rdLast;

        snapPoints.append( center + Point.asVector() );
        snapPoints.append( center - Point.asVector() );

        if (bThickness)
        {
          snapPoints.append(center + Point.asVector() + vThickness);
          snapPoints.append(center - Point.asVector() + vThickness);
        }
      }
      break;

    case OdDb::kOsModeTan:                   // Tangent: cursor as circle with tangent
      if (rdLast <= radius)
        break;

      if (ptLast.y != 0)
      {
        double a = 1 + pow(ptLast.x, 2)/pow(ptLast.y, 2);
        double b =-2*pow(radius, 2)*ptLast.x/pow(ptLast.y, 2);
        double c = pow(radius, 4)/pow(ptLast.y, 2) - pow(radius, 2);

        if (pow(b, 2) - 4*a*c >= 0)
        {
          double x = (-b + sqrt( pow(b, 2) - 4*a*c ))/(2*a);
          double y = (pow(radius, 2) - ptLast.x*x)/ptLast.y;

          snapPoints.append(center + OdGeVector3d(x, y, 0) );
          if (bThickness)
            snapPoints.append(center + OdGeVector3d(x, y, 0) + vThickness);

          x = (-b - sqrt( pow(b, 2) - 4*a*c ))/(2*a);
          y = (pow(radius, 2) - ptLast.x*x)/ptLast.y;

          snapPoints.append(center + OdGeVector3d(x, y, 0) );
          if (bThickness)
            snapPoints.append(center + OdGeVector3d(x, y, 0) + vThickness);
        }
      }
      else
      {
        double x = pow(radius, 2)/ptLast.x;
        double y = sqrt( pow(radius, 2) - pow(x, 2) );

        snapPoints.append(center + OdGeVector3d(x,  y, 0) );
        snapPoints.append(center + OdGeVector3d(x, -y, 0) );
        if (bThickness)
        {
          snapPoints.append(center + OdGeVector3d(x,  y, 0) + vThickness);
          snapPoints.append(center + OdGeVector3d(x, -y, 0) + vThickness);
        }
      }
      break;

    case OdDb::kOsModeNear:                  // Nearest: cursor ~ hourglasses
      if (rdPick > 0)
      {
        OdGePoint3d Point = ptPick*radius/rdPick;
        snapPoints.append(center + Point.asVector());
        if (bThickness)
          snapPoints.append(center + Point.asVector() + vThickness);
      }
      break;

    case OdDb::kOsModeEnd:                   // Endpoint:     cursor as square
    case OdDb::kOsModeNode:                  // Node:         cursor as cross in a square
    case OdDb::kOsModeMid:                   // Midpoint:     cursor as triangle
    case OdDb::kOsModeIntersec:              // Intersection: cursor as intersection in a square
    case OdDb::kOsModeIns:                   // Insertion:    -/-
    case OdDb::kOsModePar:                   // Parallel:
    case OdDb::kOsModeApint:                 // Apparent intersection:
      break;                                 //               isn't necessary to do

    default:
      break;
  }

  return eOk;
}
//----------------------------------------------------------------------------------------------------------------------------
