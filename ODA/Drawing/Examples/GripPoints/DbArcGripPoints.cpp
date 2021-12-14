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
/* OdDbArc entities                                                     */
/************************************************************************/
#include "StdAfx.h"
#include "DbArcGripPoints.h"
#include "DbArc.h"
#include "Ge/GeCircArc3d.h"
#include "Ge/GeMatrix3d.h"
#include "RxObjectImpl.h"
#include "DbGrip.h"
#include "OdGripPointsModule.h"


OdResult OdDbArcGripPointsPE::getGripPoints( const OdDbEntity* ent, OdGePoint3dArray& gripPoints )const
{
  unsigned int size = gripPoints.size();
  OdDbArcPtr arc = ent;
  double dThickness = arc->thickness();
  int nNumPoints = OdZero(dThickness) ? 4 : 8;
  gripPoints.resize( size + nNumPoints );
  OdGePoint3d * pStart = gripPoints.asArrayPtr() + size;
  OdGePoint3d * pCur = pStart;

  arc->getStartPoint( *pCur++ );
  arc->getEndPoint( *pCur++ );
  double start,end;
  arc->getStartParam( start );
  arc->getEndParam ( end );
  arc->getPointAtParam( (start + end)/2, *pCur++ );
  *pCur++ = arc->center();

  if (nNumPoints == 8)
  {
    OdGeVector3d vExtrusion = arc->normal() * dThickness;
    *pCur++ = *pStart++ + vExtrusion;
    *pCur++ = *pStart++ + vExtrusion;
    *pCur++ = *pStart++ + vExtrusion;
    *pCur++ = *pStart++ + vExtrusion;
  }

  return eOk;
}

OdResult OdDbArcGripPointsPE::moveGripPointsAt( OdDbEntity* pEnt, const OdIntArray& indices, const OdGeVector3d& vOffset )
{
  const unsigned indicesSize = indices.size();
  if ( indicesSize == 0 )
    return eOk;

  OdDbArcPtr pArc = pEnt;
  OdGeVector3d offset(vOffset);
  // Project offset on entity's plane in view direction
  if (!projectOffset(pArc->database(), pArc->normal(), offset))
  { // View direction is perpendicular to normal
    // Move the arc
    pArc->setCenter( pArc->center() + offset );
  }
  else
  {
    int flags = 0;
    OdGePoint3dArray pts;
    getGripPoints( pEnt, pts );
    for (unsigned i = 0; i < indicesSize; ++i)
    {
      unsigned ind = indices[i];
      if (ind < pts.size())
      {
        if (ind >= 4)
        {
          ind -= 4;
        }
        int nMask = 1 << ind;
        if (!(flags & nMask))
        {
          pts[ind] += offset;
          flags |= nMask;
        }
      }
    }

    try
    {
      if ( (flags & 8)          // Center moved
        || ((flags & 7) == 7))  // All 3 arc points moved
      {
        pArc->setCenter( pts[3] );
      }
      else
      {
        OdGePoint3d * pP1 = pts.asArrayPtr();
        OdGePoint3d * pP2 = pP1 + 2;
        OdGePoint3d * pP3 = pP1 + 1;
        OdGeCircArc3d geArc;
        // Check if arc changed direction. Normal should not be flipped.
        OdGeVector3d v1 = *pP2 - *pP1;
        OdGeVector3d v2 = *pP3 - *pP2;
        OdGeVector3d vNewNormal = v1.crossProduct(v2);
        if (vNewNormal.isCodirectionalTo(pArc->normal()))
        { // OK
          geArc.set(*pP1, *pP2, *pP3);
        }
        else
        {
          geArc.set(*pP3, *pP2, *pP1);
        }
        pArc->setFromOdGeCurve(geArc);
      }
    }
    catch(const OdError& e)
    {
      return e.code();
    }
  }
  return eOk;
}


OdResult OdDbArcGripPointsPE::getStretchPoints( const OdDbEntity* ent, OdGePoint3dArray& stretchPoints ) const
{
  int nOldSize = stretchPoints.size();
  OdResult res = getGripPoints( ent, stretchPoints );
  if ( res == eOk )
    stretchPoints.resize( nOldSize + 2 ); // remove mid and center and thickness grips
  return res;
}


OdResult OdDbArcGripPointsPE::moveStretchPointsAt( OdDbEntity* pEnt, const OdIntArray& indices, const OdGeVector3d& vOffset)
{
  const unsigned indicesSize = indices.size();
  if ( indicesSize == 0 )
    return eOk;

  OdDbArcPtr pArc = pEnt;
  OdGeVector3d offset(vOffset);
  // Project offset on entity's plane in view direction
  if (!projectOffset(pArc->database(), pArc->normal(), offset))
  { // View direction is perpendicular to normal
    // Do nothing
    return eOk;
  }

  if ( indicesSize >= 2 )
    return pEnt->transformBy(OdGeMatrix3d::translation(offset));
  try
  {
    OdGePoint3d pt;
    switch (indices[0])
    {
    case 0:
      {
        pArc->getStartPoint(pt);
        OdGeVector3d v0 = pt - pArc->center();
        pt += offset;
        OdGeVector3d v1 = pt - pArc->center();
        double angle = v0.angleTo( v1, pArc->normal() );
        pArc->setStartAngle( pArc->startAngle() + angle );
      }
      break;
    case 1:
      {
        pArc->getEndPoint(pt);
        OdGeVector3d v0 = pt - pArc->center();
        pt += offset;
        OdGeVector3d v1 = pt - pArc->center();
        double angle = v0.angleTo( v1, pArc->normal() );
        pArc->setEndAngle( pArc->endAngle() + angle );
      }
      break;
    }
  }
  catch ( const OdError& e )
  {
    return e.code();
  }
  return eOk;
}

OdResult OdDbArcGripPointsPE::getOsnapPoints(const OdDbEntity* ent, 
                                             OdDb::OsnapMode osnapMode, 
                                             OdGsMarker gsSelectionMark, 
                                             const OdGePoint3d& pickPoint,
                                             const OdGePoint3d& lastPoint, 
                                             const OdGeMatrix3d& xWorldToEye, 
                                             OdGePoint3dArray& snapPoints) const
{
  OdGePoint3dArray gripPoints;
  OdResult res = getGripPoints( ent, gripPoints );
  if ( res != eOk ) return res;
  OdDbArcPtr arc = ent;
  switch ( osnapMode )
  {
  case OdDb::kOsModeEnd:
    snapPoints.append( gripPoints[0] );
    snapPoints.append( gripPoints[1] );
    break;
  case OdDb::kOsModeMid:
    snapPoints.append( gripPoints[2] );
    break;
  case OdDb::kOsModeCen:
    snapPoints.append( gripPoints[3] );
    break;
  case OdDb::kOsModeQuad:
    {
      const OdDbDatabase* pDb = ent->database(); 
      OdGeVector3d xAxis = pDb->getUCSXDIR(); 
      OdGeVector3d yAxis = pDb->getUCSYDIR(); 
      OdGeVector3d zAxis = xAxis.crossProduct(yAxis);
      if ( !arc->normal().isParallelTo(zAxis) )
        return eInvalidAxis;
      snapPoints.append( arc->center() + xAxis * arc->radius() );
      snapPoints.append( arc->center() - xAxis * arc->radius() );
      snapPoints.append( arc->center() + yAxis * arc->radius() );
      snapPoints.append( arc->center() - yAxis * arc->radius() );
    }
    break;
  case OdDb::kOsModePerp:
    {
      OdGePlane pln( arc->center(), arc->normal() );
      OdGePoint3d pp;
      if ( !pln.project( lastPoint, pp ) )
        return eNotApplicable;
      OdGeVector3d v( pp - arc->center() );
      if ( v.isZeroLength() )
        return eNotApplicable;
      v.normalize();
      v *= arc->radius();
      snapPoints.append( arc->center() + v  );
      snapPoints.append( arc->center() - v  );
    }
    break;
  case OdDb::kOsModeTan:
    {
      OdGePlane pln( arc->center(), arc->normal() );
      OdGePoint3d pp;
      if ( !pln.project( lastPoint, pp ) )
        return eNotApplicable;
      OdGeVector3d v( pp - arc->center() );
      double dLen = v.length();
      double dRadius = arc->radius();
      if (dLen <= dRadius)
        return eNotApplicable;  // dLen may be zero
      double c = dRadius / dLen;
      double angle = OD_ACOS( c );
      OdGeVector3d v1 = v.normalize();
      v1 *= dRadius;
      OdGeVector3d v2(v1);
      v1.rotateBy( angle, arc->normal() );
      v2.rotateBy( -angle, arc->normal() );
      snapPoints.append( arc->center() + v1  );
      snapPoints.append( arc->center() + v2  );
    }
    break;
  case OdDb::kOsModeNear:
    {
      OdGePoint3d p; 
      if ( arc->getClosestPointTo( pickPoint, xWorldToEye.inverse() * OdGeVector3d::kZAxis, p ) == eOk )
        snapPoints.append( p );
    }
    break;
  default:
    break;
  }
  return eOk;
}
