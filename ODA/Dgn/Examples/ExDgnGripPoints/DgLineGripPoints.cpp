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
#include "DgLineGripPoints.h"
#include "DgLine.h"
#include "RxObjectImpl.h"
#include "Ge/GeLine3d.h"

OdResult OdDgLineGripPointsPE::getGripPoints( const OdDgElement* pEnt, OdGePoint3dArray& gripPoints )const
{
  unsigned int size = gripPoints.size();
  gripPoints.resize( size + 2 );

  if( pEnt->isKindOf( OdDgLine2d::desc() ) )
  {
    OdDgLine2dPtr pLine2d = pEnt;

    OdGePoint2d ptStart = pLine2d->getStartPoint();
    OdGePoint2d ptEnd   = pLine2d->getEndPoint();

    gripPoints[size]     = OdGePoint3d( ptStart.x, ptStart.y, 0 );
    gripPoints[size + 1] = OdGePoint3d( ptEnd.x, ptEnd.y, 0 );
  }
  else
  {
    OdDgLine3dPtr pLine3d = pEnt;

    gripPoints[size]     = pLine3d->getStartPoint();
    gripPoints[size + 1] = pLine3d->getEndPoint();
  }

  return eOk;
}

OdResult OdDgLineGripPointsPE::moveGripPointsAt( OdDgElement* pEnt, const OdIntArray& indices, const OdGeVector3d& offset )
{
  unsigned size = indices.size();

  if ( size == 0 )
    return eOk;

  if( pEnt->isKindOf( OdDgLine2d::desc() ) )
  {
    OdDgLine2dPtr pLine2d = pEnt;

    if( size > 1 || indices[0] == 2 )
    {
      OdGePoint2d ptStart = pLine2d->getStartPoint();
      OdGePoint2d ptEnd = pLine2d->getEndPoint();

      ptStart.x += offset.x;
      ptStart.y += offset.y;

      ptEnd.x += offset.x;
      ptEnd.y += offset.y;

      pLine2d->setStartPoint( ptStart );
      pLine2d->setEndPoint( ptEnd );
    }
    else if( indices[0] == 0 )
    {
      OdGePoint2d ptStart = pLine2d->getStartPoint();

      ptStart.x += offset.x;
      ptStart.y += offset.y;

      pLine2d->setStartPoint( ptStart );
    }
    else
    {
      OdGePoint2d ptEnd = pLine2d->getEndPoint();

      ptEnd.x += offset.x;
      ptEnd.y += offset.y;

      pLine2d->setEndPoint( ptEnd );
    }
  }
  else
  {
    OdDgLine3dPtr pLine3d = pEnt;

    if( size > 1 || indices[0] == 2 )
    {
      pLine3d->setStartPoint( pLine3d->getStartPoint() + offset );
      pLine3d->setEndPoint( pLine3d->getEndPoint() + offset );
    }
    else if( indices[0] == 0 )
    {
      pLine3d->setStartPoint( pLine3d->getStartPoint() + offset );
    }
    else
    {
      pLine3d->setEndPoint( pLine3d->getEndPoint() + offset );
    }
  }

  return eOk;
}

OdResult OdDgLineGripPointsPE::getStretchPoints( const OdDgElement* pEnt, OdGePoint3dArray& stretchPoints ) const
{
  OdResult res = getGripPoints( pEnt, stretchPoints );

  if( res == eOk )
  {
    stretchPoints.resize( stretchPoints.size() - 1 );
  }

  return res;
}


OdResult OdDgLineGripPointsPE::moveStretchPointsAt( OdDgElement* pEnt, const OdIntArray& indices, const OdGeVector3d& offset )
{
  return moveGripPointsAt( pEnt, indices, offset );
}


OdResult OdDgLineGripPointsPE::getOsnapPoints(const OdDgElement* pEnt, 
                                                OdDgElement::OsnapMode osnapMode, 
                                                OdGsMarker gsSelectionMark, 
                                                const OdGePoint3d& pickPoint,
                                                const OdGePoint3d& lastPoint, 
                                                const OdGeMatrix3d& xWorldToEye, 
                                                OdGePoint3dArray& snapPoints) const
{
  OdGePoint3d ptStart;
  OdGePoint3d ptEnd;

  if( pEnt->isKindOf( OdDgLine2d::desc() ) )
  {
    OdDgLine2dPtr pLine2d = pEnt;

    ptStart.x = pLine2d->getStartPoint().x;
    ptStart.y = pLine2d->getStartPoint().y;
    ptStart.z = 0;

    ptEnd.x   = pLine2d->getEndPoint().x;
    ptEnd.y   = pLine2d->getEndPoint().y;
    ptEnd.z   = 0;
  }
  else
  {
    OdDgLine3dPtr pLine3d = pEnt;

    ptStart = pLine3d->getStartPoint();
    ptEnd   = pLine3d->getEndPoint();
  }

  switch( osnapMode )
  {
    case OdDgElement::kOsModeEnd:
    {
      snapPoints.append( ptStart );
      snapPoints.append( ptEnd );
    }
    break;

    case OdDgElement::kOsModeMid:
    {
      snapPoints.append( ptStart +  (ptEnd - ptStart)/2 );
    }
    break;

    case OdDgElement::kOsModePerp:
    {
      OdGeLine3d l( ptStart, ptEnd );
      snapPoints.append( l.evalPoint( l.paramOf( lastPoint ) ) );
    }
    break;

    case OdDgElement::kOsModeNear: // TODO: project on view plane
    {
      if(!ptStart.isEqualTo(ptEnd))
      {
        OdGeLine3d l( ptStart, ptEnd );
        double p = l.paramOf( pickPoint );

        if( p > 1 )
        {
          snapPoints.append( ptEnd );
        }
        else if( p < 0 )
        {
          snapPoints.append( ptStart );
        }
        else
        {
          snapPoints.append( l.evalPoint(p) );
        }
      }
      else
      {
        snapPoints.append( ptStart );
      }
    }
    break;

    default:
    {

    }
    break;
  }

  return eOk;
}
