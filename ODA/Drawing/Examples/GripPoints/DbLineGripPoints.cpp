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
/* OdDbLine entities.                                                   */
/************************************************************************/
#include "StdAfx.h"
#include "DbLineGripPoints.h"
#include "DbLine.h"
#include "RxObjectImpl.h"
#include "Ge/GeLine3d.h"

OdResult OdDbLineGripPointsPE::getGripPoints( const OdDbEntity* ent, OdGePoint3dArray& gripPoints )const
{
  unsigned int size = gripPoints.size();
  gripPoints.resize( size + 3 );
  OdDbLinePtr line = ent;
  line->getStartPoint( gripPoints[size + 0] );
  line->getEndPoint( gripPoints[size + 1] );
  gripPoints[size + 2] = gripPoints[size + 0] + (gripPoints[size + 1] - gripPoints[size + 0])/2;
  return eOk;
}

OdResult OdDbLineGripPointsPE::moveGripPointsAt( OdDbEntity* pEnt, const OdIntArray& indices, const OdGeVector3d& offset )
{
  unsigned size = indices.size();
  if ( size == 0 )
    return eOk;

  OdDbLinePtr pLine = pEnt;
  if ( size > 1 || indices[0] == 2 )
  {
    pLine->setStartPoint( pLine->startPoint() + offset );
    pLine->setEndPoint( pLine->endPoint() + offset );
  }
  else if ( indices[0] == 0 )
  {
    pLine->setStartPoint( pLine->startPoint() + offset );
  }
  else if ( indices[0] == 1 )
  {
    pLine->setEndPoint( pLine->endPoint() + offset );
  }
  return eOk;
}

OdResult OdDbLineGripPointsPE::getStretchPoints( const OdDbEntity* ent, OdGePoint3dArray& stretchPoints ) const
{
  OdResult res = getGripPoints( ent, stretchPoints );
  if ( res == eOk ) stretchPoints.resize( stretchPoints.size() - 1 );
  return res;
}

OdResult OdDbLineGripPointsPE::moveStretchPointsAt( OdDbEntity* ent, const OdIntArray& indices, const OdGeVector3d& offset )
{
  return moveGripPointsAt( ent, indices, offset );
}

OdResult OdDbLineGripPointsPE::getOsnapPoints(const OdDbEntity* ent, 
                                              OdDb::OsnapMode osnapMode, 
                                              OdGsMarker gsSelectionMark, 
                                              const OdGePoint3d& pickPoint,
                                              const OdGePoint3d& lastPoint, 
                                              const OdGeMatrix3d& xWorldToEye, 
                                              OdGePoint3dArray& snapPoints) const
{
  OdDbLinePtr line = ent;
  OdGePoint3d start,end;
  line->getStartPoint( start );
  line->getEndPoint( end );
  switch ( osnapMode )
  {
  case OdDb::kOsModeEnd:
    snapPoints.append( start );
    snapPoints.append( end );
    break;
  case OdDb::kOsModeMid:
    snapPoints.append( start +  (end - start)/2 );
    break;
  case OdDb::kOsModePerp:
    {
      OdGeLine3d l( start, end );
      snapPoints.append( l.evalPoint( l.paramOf( lastPoint ) ) );
    }
    break;
  case OdDb::kOsModeNear: // TODO: project on view plane
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
    break;
  default:
    break;
  }
  return eOk;
}
