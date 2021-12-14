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
/* OdDbPolyline entities.                                               */
/************************************************************************/
#include "StdAfx.h"
#include "DbPolylineGripPoints.h"
#include "DbPolyline.h"
#include "Ge/GeMatrix3d.h"
#include "Ge/GeCircArc2d.h"
#include "Ge/GeCircArc3d.h"
#include "Gi/GiViewport.h"

OdResult OdDbPolylineGripPointsPE::getGripPoints( const OdDbEntity* pEnt, OdGePoint3dArray& gripPoints )const
{
  unsigned int size = gripPoints.size();
  OdDbPolylinePtr pPoly = pEnt;
  
  gripPoints.resize( size + (pPoly->numVerts()*2-1) );

  if (pPoly->numVerts() > 1)
  {
    pPoly->getPointAt( 0, gripPoints[size] );
  }
  unsigned int iIndAdd = size + 1;
  for ( unsigned int i = 1; i < pPoly->numVerts(); ++i )
  {
    OdGePoint3d p; pPoly->getPointAtParam( i - 0.5, p );
    gripPoints[iIndAdd++] = p;
    pPoly->getPointAt( i, gripPoints[iIndAdd++] );
  }
  if(pPoly->isClosed())
  {
    OdGePoint3d p; pPoly->getPointAtParam( pPoly->numVerts() - 0.5, p );
    gripPoints.append(p);
  }
  return eOk;
}

OdResult OdDbPolylineGripPointsPE::moveGripPointsAt( OdDbEntity* pEnt, const OdIntArray& indices, const OdGeVector3d& offset )
{
  unsigned size = indices.size();
  if ( size == 0 )
    return eOk;

  OdDbPolylinePtr pPoly = pEnt;
  OdGeMatrix3d x = OdGeMatrix3d::worldToPlane(pPoly->normal());
  int nVerts = pPoly->numVerts();

  int iPolyIndex = 0;
  unsigned int iUStart = 0, i = 0;

  for (i = 0; i < size; ++i )
  {
    iPolyIndex = indices[i]/2;
    if ( indices[i]%2 == 0 )
    { // "Vertex. Check if near middle grip point presents. If yes skip this vertex
      int iPrev = indices[i] - 1;
      if (iPrev < 0)
      {
        if(pPoly->isClosed())
        {
          iPrev = nVerts * 2 - 1;
        }
      }
      if (iPrev >= 0 && indices.find(iPrev, iUStart))
        continue;

      int iNext = indices[i] + 1;
      if (iNext > (nVerts - 1) * 2)
      {
        iNext = pPoly->isClosed() ? 1 : -1;
      }
      if (iNext >= 0 && indices.find(iNext, iUStart))
        continue;
        
      OdGePoint3d pt;      
      pPoly->getPointAt(iPolyIndex, pt);
      pt += offset;
      pPoly->setPointAt(iPolyIndex, (x * pt).convert2d());
    }
    else
    { // Middle of segment point
      int iPolyIndex1 = (iPolyIndex != pPoly->numVerts() - 1) ? iPolyIndex + 1 : 0;

      if (!OdZero( pPoly->getBulgeAt(iPolyIndex) ) )
      { // Arc segment
        OdGePoint2d p0;
        pPoly->getPointAt( iPolyIndex, p0 );
        OdGePoint2d p1;
        pPoly->getPointAt( iPolyIndex1, p1 );
        OdGePoint3d pt;
        pPoly->getPointAtParam( iPolyIndex + 0.5, pt );
        pt += offset;
        OdGePoint2d p = (x * pt).convert2d();
        try
        {
          OdGeCircArc2d arc( p0, p, p1 );
          double bulge = tan((arc.endAng()-arc.startAng())/4);
          if ( arc.isClockWise() ) bulge = -bulge;
          pPoly->setBulgeAt( iPolyIndex, bulge );
        }
        catch( OdError& e )
        {
          return e.code();
        }
      }
      else
      { // Line segement
        OdGePoint3d pt;
        pPoly->getPointAt( iPolyIndex, pt );
        pt += offset;
        pPoly->setPointAt(iPolyIndex, (x * pt).convert2d());

        pPoly->getPointAt( iPolyIndex1, pt );
        pt += offset;
        pPoly->setPointAt(iPolyIndex1, (x * pt).convert2d());
      }
    }
  }
  return eOk;
}

OdResult OdDbPolylineGripPointsPE::getStretchPoints( const OdDbEntity* ent, OdGePoint3dArray& stretchPoints ) const
{
  unsigned int size = stretchPoints.size();
  OdDbPolylinePtr pPoly = ent;

  stretchPoints.resize( size + pPoly->numVerts() );
  unsigned int i = 0;
  for ( ; i < pPoly->numVerts(); ++i )
    pPoly->getPointAt( i, stretchPoints[size + i] );

  return eOk;
}


OdResult OdDbPolylineGripPointsPE::moveStretchPointsAt( OdDbEntity* ent, const OdIntArray& indices, const OdGeVector3d& offset)
{
  OdDbPolylinePtr pPoly = ent;
  OdGeVector3d off(offset);
  off.transformBy(OdGeMatrix3d::worldToPlane(pPoly->normal()));
  OdGeVector2d offset2d(offset.convert2d());
  
  for (unsigned int i = 0; i < indices.size(); ++i )
  {
    OdGePoint2d p;
    pPoly->getPointAt(indices[i], p);
    pPoly->setPointAt(indices[i], p + offset2d);
  }
  return eOk;
}

OdResult OdDbPolylineGripPointsPE::getOsnapPoints(const OdDbEntity* ent, 
                                                  OdDb::OsnapMode osnapMode, 
                                                  OdGsMarker gsSelectionMark, 
                                                  const OdGePoint3d& pickPoint,
                                                  const OdGePoint3d& lastPoint, 
                                                  const OdGeMatrix3d& xWorldToEye, 
                                                  OdGePoint3dArray& snapPoints) const
{
  OdDbPolylinePtr pPoly = ent;

  if (gsSelectionMark)
  {
    OdDbFullSubentPath subEntPath(OdDb::kEdgeSubentType, gsSelectionMark);
    OdDbEntityPtr pSubEnt = pPoly->subentPtr(subEntPath);
    if (!pSubEnt.isNull())
      return pSubEnt->getOsnapPoints( 
                                    osnapMode, 
                                    0,  // gsSelectionMark, 
                                    pickPoint,
                                    lastPoint, 
                                    xWorldToEye, 
                                    snapPoints);
  }

  int size = snapPoints.size();
  switch ( osnapMode )
  {
  case OdDb::kOsModeEnd:
    getStretchPoints( ent, snapPoints );
    break;
  case OdDb::kOsModeMid:
      {
        snapPoints.resize( size + pPoly->numVerts() - 1 );
        for ( unsigned i = 1; i < pPoly->numVerts(); ++i )
          pPoly->getPointAtParam( i - 0.5, snapPoints[ size + i - 1 ] );
        break;
      }
  
  case OdDb::kOsModeCen:

    for ( unsigned i = 0; i < pPoly->numVerts(); i++ )
    {
      if ( pPoly->segType(i) == OdDbPolyline::kArc )
      {  
        OdGeCircArc3d arc;
        pPoly->getArcSegAt(i, arc);

        snapPoints.append( arc.center() );
      }
    }

    break;

  case OdDb::kOsModeQuad: 
    {
  
    for ( unsigned i = 0; i < pPoly->numVerts(); i++ )
    {
      if ( pPoly->segType(i) == OdDbPolyline::kArc )
      {  
        OdGeCircArc3d arc;
        pPoly->getArcSegAt(i, arc);

       
        const OdDbDatabase* pDb = ent->database(); 
        OdGeVector3d xAxis = pDb->getUCSXDIR(); 
        OdGeVector3d yAxis = pDb->getUCSYDIR(); 
        OdGeVector3d zAxis = xAxis.crossProduct(yAxis);
        if ( !arc.normal().isParallelTo(zAxis) )
          return eInvalidAxis;

        OdGeVector3d vStart, vEnd, vQuad;

        vStart = arc.startPoint() - arc.center();
        vEnd = arc.endPoint() - arc.center();
 
        int k[5] = {0, 1, 0, -1, 0};

        for (int quad = 0; quad < 4; quad ++)
        {
          vQuad = xAxis * arc.radius() * k[quad + 1] 
                + yAxis * arc.radius() * k[quad] ;

          if ( (vQuad - vStart).crossProduct( (vEnd - vQuad ) ).isCodirectionalTo( arc.normal() ) )
          {
            snapPoints.append( arc.center() 
                + xAxis * arc.radius() * k[quad + 1] 
                + yAxis * arc.radius() * k[quad] );
          }
        }        
      }
    }

    }   
    break;
  case OdDb::kOsModeNear:
    {
      OdGePoint3d p; 
      if ( pPoly->getClosestPointTo( pickPoint, xWorldToEye.inverse() * OdGeVector3d::kZAxis, p ) == eOk )
        snapPoints.append( p );
    }
    break;
  default:
    break;
  }
  return eOk;
}
