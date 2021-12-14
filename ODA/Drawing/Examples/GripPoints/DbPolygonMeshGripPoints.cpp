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
/* OdDbPolygonMesh entities.                                            */
/************************************************************************/
#include "StdAfx.h"
#include "DbPolygonMeshGripPoints.h"
#include "DbPolygonMesh.h"
#include "RxObjectImpl.h"
#include "DbPolygonMeshVertex.h"
#include "DbLine.h"

OdResult OdDbPolygonMeshGripPointsPE::getGripPoints( const OdDbEntity* ent, OdGePoint3dArray& gripPoints )const
{
  OdDbPolygonMesh* pMesh = OdDbPolygonMesh::cast(ent);
  OdDbObjectIteratorPtr pIter = pMesh->vertexIterator();

  while(!pIter->done())
  {
    OdDbPolygonMeshVertexPtr pVertex = pIter->entity();
    gripPoints.append( pVertex->position() );
    pIter->step();
  }

  return eOk;
}

OdResult OdDbPolygonMeshGripPointsPE::moveGripPointsAt( OdDbEntity* pEnt, const OdIntArray& indices, const OdGeVector3d& offset )
{
  OdDbPolygonMesh* pMesh = OdDbPolygonMesh::cast(pEnt);
  OdDbObjectIteratorPtr pIter;

  for( unsigned iPt = 0; iPt < indices.size(); iPt++ )
  {
    int iIndex = indices[iPt];
    pIter = pMesh->vertexIterator();

    int iCurIndex = 0;

    while(!pIter->done())
    {
      if( iCurIndex == iIndex )
      {
        OdDbPolygonMeshVertexPtr pVertex = pIter->entity();
        pMesh->openVertex( pVertex->id(), OdDb::kForWrite );
        pVertex->setPosition( pVertex->position() + offset );
        pMesh->subClose();
        break;
      }
      else
        iCurIndex++;

      pIter->step();
    }
  }
  return eOk;
}

OdResult OdDbPolygonMeshGripPointsPE::getStretchPoints( const OdDbEntity* ent, OdGePoint3dArray& stretchPoints ) const
{
  return getGripPoints( ent, stretchPoints );
}

OdResult OdDbPolygonMeshGripPointsPE::moveStretchPointsAt( OdDbEntity* ent, const OdIntArray& indices, const OdGeVector3d& offset )
{
  return moveGripPointsAt( ent, indices, offset );
}

OdResult OdDbPolygonMeshGripPointsPE::getOsnapPoints(const OdDbEntity* ent, 
                         OdDb::OsnapMode osnapMode, 
                         OdGsMarker gsSelectionMark, 
                         const OdGePoint3d& pickPoint,
                         const OdGePoint3d& lastPoint, 
                         const OdGeMatrix3d& xWorldToEye, 
                         OdGePoint3dArray& snapPoints) const
{
  OdDbPolygonMesh* pMesh = OdDbPolygonMesh::cast(ent);
  OdDbObjectIteratorPtr pIter = pMesh->vertexIterator();
  OdGePoint3dArray pPosArr;

  while(!pIter->done())
  {
    OdDbPolygonMeshVertexPtr pVertex = pIter->entity();
    pPosArr.append( pVertex->position() );
    pIter->step();
  }

  OdInt32 nCols = pMesh->nSize();
  OdInt32 nRows = pMesh->mSize();
  bool    bColsClosed = pMesh->isNClosed();
  bool    bRowsClosed = pMesh->isMClosed();

  for( int i = 0; i < nRows; i++ )
    for( int j = 0; j < nCols; j++ )
    {
      if( j < nCols - 1 )
      {
        OdDbLinePtr pLine = OdDbLine::createObject();
        pLine->setStartPoint( pPosArr[ i*nCols + j] );
        pLine->setEndPoint( pPosArr[ i*nCols + j + 1] );
        pLine->getOsnapPoints( osnapMode, 0, pickPoint, lastPoint, xWorldToEye, snapPoints);
      }
      else if( j == nCols - 1 && bColsClosed )
      {
        OdDbLinePtr pLine = OdDbLine::createObject();
        pLine->setStartPoint( pPosArr[ i*nCols + j] );
        pLine->setEndPoint( pPosArr[ i*nCols ] );
        pLine->getOsnapPoints( osnapMode, 0, pickPoint, lastPoint, xWorldToEye, snapPoints);
      }

      if( i < nRows - 1 )
      {
        OdDbLinePtr pLine = OdDbLine::createObject();
        pLine->setStartPoint( pPosArr[ i*nCols + j] );
        pLine->setEndPoint( pPosArr[ (i+1)*nCols + j] );
        pLine->getOsnapPoints( osnapMode, 0, pickPoint, lastPoint, xWorldToEye, snapPoints);
      }
      else if( i == nRows - 1 && bRowsClosed )
      {
        OdDbLinePtr pLine = OdDbLine::createObject();
        pLine->setStartPoint( pPosArr[ i*nCols + j] );
        pLine->setEndPoint( pPosArr[j] );
        pLine->getOsnapPoints( osnapMode, 0, pickPoint, lastPoint, xWorldToEye, snapPoints);
      }
    }

  return eOk;
}

OdResult OdDbPolygonMeshGripPointsPE::getGripPointsAtSubentPath(  const OdDbEntity* pEntity, 
                                const OdDbFullSubentPath& path, 
                                OdDbGripDataPtrArray& grips,
                                const double curViewUnitSize, 
                                const int gripSize,
                                const OdGeVector3d& curViewDir, 
                                const OdUInt32 bitflags) const
{
  return eNotApplicable;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

OdResult OdDbPolygonMeshGripPointsPE::moveGripPointsAtSubentPaths( OdDbEntity* pEntity,
                                 const OdDbFullSubentPathArray& paths, 
                                 const OdDbVoidPtrArray& gripAppData,
                                 const OdGeVector3d& offset, 
                                 const OdUInt32 bitflags)
{
  return eNotApplicable;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
