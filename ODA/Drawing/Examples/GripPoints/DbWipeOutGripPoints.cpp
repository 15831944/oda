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
#include "DbWipeOutGripPoints.h"
//----------------------------------------------------------------------------------------

#include <Ge/GeMatrix3d.h>
#include <Ge/GeCircArc2d.h>
#include <ResBuf.h>
#include <Ge/GeExtents3d.h>
#include <Ge/GeScale3d.h>
#include <Ge/GeLine2d.h>
#include <Ge/GeLineSeg3d.h>
#include <Ge/GeCircArc3d.h>
#include <DbWipeout.h>

OdResult OdDbWipeOutGripPointsPE::getGripPoints( const OdDbEntity* pEnt, OdGePoint3dArray& gripPoints )const
{
  OdGePoint3dArray odGePoint3dArray;
  OdDbWipeoutPtr(pEnt)->getVertices(odGePoint3dArray);

  // for the closed polyline boundary last coincident point is not returned as a grip
  if (odGePoint3dArray.size() > 2 && odGePoint3dArray.last().isEqualTo(odGePoint3dArray.first()))
    odGePoint3dArray.removeLast();

  gripPoints.append(odGePoint3dArray);
  return eOk;
}

OdResult OdDbWipeOutGripPointsPE::moveGripPointsAt( OdDbEntity* pEnt, const OdIntArray& indices, const OdGeVector3d& offset )
{
  unsigned size = indices.size();
  if ( size == 0 )
    return eOk;

  OdDbWipeoutPtr pWipeout = pEnt;
  OdGePoint3dArray odGePoint3dArray;
  pWipeout->getVertices(odGePoint3dArray);

  // for the closed polyline boundary last coincident point is not returned as a grip
  bool hasClosedPolylineBoundary = odGePoint3dArray.size() > 2 && odGePoint3dArray.last().isEqualTo(odGePoint3dArray.first());

  for ( unsigned i = 0; i < size; ++i )
  {
    if (indices[i] < (int)odGePoint3dArray.length() )
    {
      OdGePoint3d pt3D = odGePoint3dArray.getAt(indices[i]);
      pt3D += offset;
      odGePoint3dArray.setAt(indices[i], pt3D);
    }
  }
  if (hasClosedPolylineBoundary)
    odGePoint3dArray[odGePoint3dArray.length() - 1] = odGePoint3dArray.first();
  pWipeout->setBoundary(odGePoint3dArray);
  return eOk;
}

OdResult OdDbWipeOutGripPointsPE::getStretchPoints( const OdDbEntity* ent, OdGePoint3dArray& stretchPoints ) const
{
  return getGripPoints(ent, stretchPoints);
}

OdResult OdDbWipeOutGripPointsPE::moveStretchPointsAt( OdDbEntity* ent, const OdIntArray& indices, const OdGeVector3d& offset)
{
  return moveGripPointsAt(ent, indices, offset);
}

OdResult OdDbWipeOutGripPointsPE::getOsnapPoints(const OdDbEntity* ent,
                                                  OdDb::OsnapMode osnapMode,
                                                  OdGsMarker gsSelectionMark,
                                                  const OdGePoint3d& pickPoint,
                                                  const OdGePoint3d& lastPoint,
                                                  const OdGeMatrix3d& xWorldToEye,
                                                  OdGePoint3dArray& snapPoints) const
{

  OdDbWipeoutPtr pbWipeout = ent;
  OdGePoint3dArray odGePoint3dArray;
  pbWipeout->getVertices(odGePoint3dArray);

  int size = snapPoints.size();
  switch ( osnapMode )
  {
  case OdDb::kOsModeEnd:
     getStretchPoints( ent, snapPoints );
    break;
  case OdDb::kOsModeMid:
      {
        snapPoints.resize( size + odGePoint3dArray.length() - 1 );
        for ( unsigned i = 1; i <  odGePoint3dArray.length(); ++i )
    {
      OdGeLine3d l( odGePoint3dArray.getAt(i-1), odGePoint3dArray.getAt(i) );
      snapPoints.append( l.evalPoint( l.paramOf( lastPoint ) ) );
    }
        break;
      }
  case OdDb::kOsModeQuad: // not implemented yet
    break;
  case OdDb::kOsModeNear:
    {
    snapPoints.resize( size + odGePoint3dArray.length() - 1 );
     OdGePoint3d start,end;
    for ( unsigned i = 1; i <  odGePoint3dArray.length(); ++i )
    {
      start = odGePoint3dArray.getAt(i-1);
      end = odGePoint3dArray.getAt(i);

      if(!start.isEqualTo(end))
      {
        OdGeLine3d l( start, end );
        double p = l.paramOf( pickPoint );
        if ( p > 1 )
          snapPoints.append( end );
        else if ( p < 0 )
          snapPoints.append( start );
        else
          snapPoints.append( l.evalPoint(p) );
      }
      else
      {
        snapPoints.append( start );
      }
    }
      }
    break;
  }
  return eOk;
}
