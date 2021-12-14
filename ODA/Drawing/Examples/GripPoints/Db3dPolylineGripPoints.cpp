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
/* OdDb3DPolyline entities.                                             */
/************************************************************************/
#include "StdAfx.h"
#include "Db3dPolylineGripPoints.h"
#include "Db3dPolyline.h"
#include "Db3dPolylineVertex.h"

OdResult OdDb3dPolylineGripPointsPE::getGripPoints( const OdDbEntity* pEnt, OdGePoint3dArray& gripPoints )const
{
  unsigned int size = gripPoints.size();
  OdDb3dPolylinePtr pPoly = pEnt;
  OdDbObjectIteratorPtr pIt = pPoly->vertexIterator();
  while (!pIt->done())
  {
    OdDb3dPolylineVertexPtr pVertex = OdDb3dPolylineVertex::cast(pIt->entity());
    if (pVertex->vertexType() == OdDb::k3dSimpleVertex || pVertex->vertexType() == OdDb::k3dControlVertex)
      gripPoints.append(pVertex->position());
    pIt->step();
  }
  return eOk;
}

OdResult OdDb3dPolylineGripPointsPE::moveGripPointsAt( OdDbEntity* pEnt, const OdIntArray& indices, const OdGeVector3d& offset )
{
  unsigned size = indices.size();
  if (size == 0)
    return eOk;

  OdDb3dPolylinePtr pPoly = pEnt;

  OdDb::Poly3dType prevType = pPoly->polyType();

  OdDbObjectIteratorPtr pIt = pPoly->vertexIterator();
  int counter = 0;
  while (!pIt->done())
  {
    OdDb3dPolylineVertexPtr pVertex = OdDb3dPolylineVertex::cast(pIt->entity());
    if (pVertex->vertexType() == OdDb::k3dSimpleVertex || pVertex->vertexType() == OdDb::k3dControlVertex)
    {
      for (unsigned i = 0; i < size; i++)
      {
        if (indices[i] == counter)
        {
          pVertex->upgradeOpen();
          pVertex->setPosition(pVertex->position() + offset);
          break;
        }
      }
      counter++;
    }
    pIt->step();
  }

  if (prevType != OdDb::k3dSimplePoly)
    pPoly->convertToPolyType(prevType); // Force recompution of spline curve

  return eOk;
}

OdResult OdDb3dPolylineGripPointsPE::getStretchPoints( const OdDbEntity* ent, OdGePoint3dArray& stretchPoints ) const
{
  return getGripPoints(ent, stretchPoints);
}

OdResult OdDb3dPolylineGripPointsPE::moveStretchPointsAt( OdDbEntity* ent, const OdIntArray& indices, const OdGeVector3d& offset)
{
  return moveGripPointsAt(ent, indices, offset);
}

OdResult OdDb3dPolylineGripPointsPE::getOsnapPoints(const OdDbEntity* ent,
                                                    OdDb::OsnapMode osnapMode,
                                                    OdGsMarker /*gsSelectionMark*/,
                                                    const OdGePoint3d& /*pickPoint*/,
                                                    const OdGePoint3d& /*lastPoint*/,
                                                    const OdGeMatrix3d& /*xWorldToEye*/,
                                                    OdGePoint3dArray& snapPoints) const
{
  OdDb3dPolylinePtr pPoly = ent;
  int size = snapPoints.size();
  switch ( osnapMode )
  {
  case OdDb::kOsModeEnd:
    {
      OdDbObjectIteratorPtr pIt = pPoly->vertexIterator();
      while (!pIt->done())
      {
        OdDb3dPolylineVertexPtr pVertex = OdDb3dPolylineVertex::cast(pIt->entity());
        if (pVertex->vertexType() == OdDb::k3dSimpleVertex || pVertex->vertexType() == OdDb::k3dFitVertex)
          snapPoints.append(pVertex->position());
        pIt->step();
      }
    }
    break;
  default:
    break;
  }
  return eOk;
}
