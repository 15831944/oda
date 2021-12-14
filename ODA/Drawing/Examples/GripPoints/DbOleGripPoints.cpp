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
#include "DbOleGripPoints.h"

//----------------------------------------------------------------------------------------
#include <Ge/GeMatrix3d.h>
#include <Ge/GeScale3d.h>
#include <Ge/GeExtents3d.h>

//########################################################################################
//###############                   OdDbOleGripPointsPE                     ##############
//########################################################################################

OdResult OdDbOleGripPointsPE::getGripPoints( const OdDbEntity* pEnt, OdGePoint3dArray& gripPoints )const
{
  OdGeExtents3d exts;
  unsigned int size = gripPoints.size();
  gripPoints.resize( size + 5 );
  OdRectangle3d rect;

  OdDbOle2FramePtr ole2Frame = pEnt;

  ole2Frame->position(rect);
  gripPoints[size + 0] = rect.lowLeft;
  gripPoints[size + 1] = rect.lowRight;
  gripPoints[size + 2] = rect.upLeft;
  gripPoints[size + 3] = rect.upRight;

  OdResult res = pEnt->getGeomExtents(exts);
  if (res == eOk)
  {
    gripPoints[size + 4] = exts.minPoint() + (exts.maxPoint() - exts.minPoint()) / 2.;
  }

  return eOk;
}

OdResult OdDbOleGripPointsPE::moveGripPointsAt( OdDbEntity* pEnt, const OdIntArray& indices, const OdGeVector3d& offset )
{
  OdGePoint3dArray point3dArray;
  OdRectangle3d rect;
  OdDbOle2FramePtr ole2Frame = pEnt;

  if ( indices.empty() )
    return eOk;

  OdResult res = getGripPoints( pEnt, point3dArray );
  if(res != eOk)
  {
    return eOk;
  }

  OdGePoint3dArray gripPoints(point3dArray);

//  OdGeMatrix3d x = OdGeMatrix3d::worldToPlane(ole2Frame->normal());
  for ( unsigned i = 0; i < indices.size(); ++i )
  {
    gripPoints[indices[i]] += offset;
//    gripPoints[indices[i]] = (x * gripPoints[indices[i]]);
  }
  ole2Frame->position(rect);

  switch(indices[0])
  {
    case 0:/*lowLeft*/
    {
      if ((point3dArray[0].x - point3dArray[3].x) != 0 && (point3dArray[0].y - point3dArray[3].y) !=0)
      {
        double scalex = fabs( (gripPoints[0].x - gripPoints[3].x)/(point3dArray[0].x - point3dArray[3].x) ) ;
        double scaley = fabs( (gripPoints[0].y - gripPoints[3].y)/(point3dArray[0].y - point3dArray[3].y) ) ;
        OdGeScale3d odGeScale3d(scalex,scaley,1);

        res = pEnt->transformBy(OdGeMatrix3d::scaling(odGeScale3d,gripPoints[3]));
      }
    }
    break;
    case 1:/*lowRight*/
    {
      if ((point3dArray[1].x - point3dArray[2].x) != 0 && (point3dArray[1].y - point3dArray[2].y) !=0)
      {
        double scalex = fabs( (gripPoints[1].x - gripPoints[2].x)/(point3dArray[1].x - point3dArray[2].x) ) ;
        double scaley = fabs( (gripPoints[1].y - gripPoints[2].y)/(point3dArray[1].y - point3dArray[2].y) ) ;
        OdGeScale3d odGeScale3d(scalex,scaley,1);

        res = pEnt->transformBy(OdGeMatrix3d::scaling(odGeScale3d,gripPoints[2]));
      }
    }
    break;
    case 2:/*upLeft*/
    {
      if ((point3dArray[0].x - point3dArray[3].x) != 0 && (point3dArray[0].y - point3dArray[3].y) !=0)
      {
        double scalex = fabs( (gripPoints[1].x - gripPoints[2].x)/(point3dArray[1].x - point3dArray[2].x) ) ;
        double scaley = fabs( (gripPoints[1].y - gripPoints[2].y)/(point3dArray[1].y - point3dArray[2].y) ) ;
        OdGeScale3d odGeScale3d(scalex,scaley,1);

        res = pEnt->transformBy(OdGeMatrix3d::scaling(odGeScale3d,gripPoints[1]));
      }
    }
    break;
    case 3:/*upRight*/
    {
      if ((point3dArray[1].x - point3dArray[2].x) != 0 && (point3dArray[1].y - point3dArray[2].y) !=0)
      {
        double scalex = fabs( (gripPoints[0].x - gripPoints[3].x)/(point3dArray[0].x - point3dArray[3].x) ) ;
        double scaley = fabs( (gripPoints[0].y - gripPoints[3].y)/(point3dArray[0].y - point3dArray[3].y) ) ;
        OdGeScale3d odGeScale3d(scalex,scaley,1);

        res = pEnt->transformBy(OdGeMatrix3d::scaling(odGeScale3d,gripPoints[0]));
      }
    }
    break;
    case 4: /* center */
    {
      OdGeVector3d offset = gripPoints[4] - point3dArray[4];
      res = pEnt->transformBy(OdGeMatrix3d::translation(offset));
    }
    default:
    break;
  }
  return eOk;
}

OdResult OdDbOleGripPointsPE::getStretchPoints( const OdDbEntity* ent, OdGePoint3dArray& stretchPoints ) const
{
  OdResult res = getGripPoints( ent, stretchPoints );
  return res;
}

OdResult OdDbOleGripPointsPE::moveStretchPointsAt( OdDbEntity* ent, const OdIntArray& indices, const OdGeVector3d& offset )
{
  return moveGripPointsAt( ent, indices, offset);
}

OdResult OdDbOleGripPointsPE::getOsnapPoints( const OdDbEntity* pEnt, OdDb::OsnapMode osnapMode, OdGsMarker /*gsSelectionMark*/,
  const OdGePoint3d& pickPoint, const OdGePoint3d& lastPoint,
  const OdGeMatrix3d& xWorldToEye,
  OdGePoint3dArray& snapPoints ) const
{
  return eOk;
}
