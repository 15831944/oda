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
/* OdDb2dPolyline entities.                                             */
/************************************************************************/
#include "StdAfx.h"
#include "Db2dPolylineGripPoints.h"
#include "Db2dPolyline.h"
#include "Ge/GeMatrix3d.h"
#include "Ge/GeCircArc2d.h"
#include "Gi/GiViewport.h"

OdResult OdDb2dPolylineGripPointsPE::getGripPoints( const OdDbEntity* pEnt, OdGePoint3dArray& gripPoints )const
{
  unsigned int size = gripPoints.size();
  OdDb2dPolylinePtr pPoly = pEnt;
  OdDbObjectIteratorPtr pIt = pPoly->vertexIterator();
  while (!pIt->done())
  {
    OdDb2dVertexPtr pVertex = OdDb2dVertex::cast(pIt->entity());
    ODA_ASSERT_ONCE(pVertex->vertexType() == OdDb::k2dVertex); //TODO AlexeyTyurin 06.03.2017 
    if (pVertex->vertexType() == OdDb::k2dVertex)
      gripPoints.append(pVertex->position());
    pIt->step();
  }
  return eOk;
}

OdResult OdDb2dPolylineGripPointsPE::moveGripPointsAt( OdDbEntity* pEnt, const OdIntArray& indices, const OdGeVector3d& offset )
{
  unsigned size = indices.size();
  if (size == 0)
    return eOk;

  OdDb2dPolylinePtr pPoly = pEnt;

  OdDb::Poly2dType prevType = pPoly->polyType();

  OdDbObjectIteratorPtr pIt = pPoly->vertexIterator();
  int counter = 0;
  while (!pIt->done())
  {
    OdDb2dVertexPtr pVertex = OdDb2dVertex::cast(pIt->entity());
    if (pVertex->vertexType() == OdDb::k2dVertex)
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

  if (prevType != OdDb::k2dSimplePoly)
    pPoly->convertToPolyType(prevType); // Force recompution of spline curve

  return eOk;
}

OdResult OdDb2dPolylineGripPointsPE::getStretchPoints( const OdDbEntity* ent, OdGePoint3dArray& stretchPoints ) const
{
  OdDb2dPolylinePtr pPoly = ent;
  for (OdDbObjectIteratorPtr i = pPoly->vertexIterator() ; !i->done(); i->step())
  {
    OdDb2dVertexPtr v = OdDb2dVertex::cast(i->entity());
    if (!v.isNull() && v->vertexType() == OdDb::k2dVertex)
      stretchPoints.append(v->position());
  }
  return eOk;
}


OdResult OdDb2dPolylineGripPointsPE::moveStretchPointsAt( OdDbEntity* ent, const OdIntArray& indices, const OdGeVector3d& offset)
{
  OdDb2dPolylinePtr pPoly = ent;
  int index = 0;
  for (OdDbObjectIteratorPtr i = pPoly->vertexIterator() ; !i->done(); i->step(), ++index)
  {
    OdDb2dVertexPtr v = OdDb2dVertex::cast(i->entity());
    if (!v.isNull() && v->vertexType() == OdDb::k2dVertex)
    {
      if (indices.contains(index))
      {
        v->upgradeOpen();
        v->setPosition(v->position() + offset);
      }
    }
  }
  return eOk;
}

OdResult OdDb2dPolylineGripPointsPE::getOsnapPoints(const OdDbEntity* ent, 
                                                  OdDb::OsnapMode osnapMode, 
                                                  OdGsMarker gsSelectionMark, 
                                                  const OdGePoint3d& pickPoint,
                                                  const OdGePoint3d& lastPoint, 
                                                  const OdGeMatrix3d& xWorldToEye, 
                                                  OdGePoint3dArray& snapPoints) const
{
  ///return eNotImplementedYet;
  OdDb2dPolylinePtr pPoly = ent;
  int size = snapPoints.size();
  switch (osnapMode)
  {
  case OdDb::kOsModeEnd:
  case OdDb::kOsModeCen:
  case OdDb::kOsModeQuad:
  case OdDb::kOsModePerp:
  case OdDb::kOsModeTan:
  {
    OdDbObjectIteratorPtr pIt = pPoly->vertexIterator();
    while (!pIt->done())
    {
      OdDb2dVertexPtr pVertex = OdDb2dVertex::cast(pIt->entity());
      if (pVertex->vertexType() == OdDb::k2dVertex || pVertex->vertexType() == OdDb::k2dCurveFitVertex)
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
