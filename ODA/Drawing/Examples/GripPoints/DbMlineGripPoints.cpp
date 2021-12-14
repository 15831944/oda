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
/* OdDbMline entities.                                                   */
/************************************************************************/
#include "StdAfx.h"
#include "DbMlineGripPoints.h"
#include "DbMline.h"
#include "RxObjectImpl.h"

OdResult OdDbMlineGripPointsPE::getGripPoints( const OdDbEntity* ent, OdGePoint3dArray& gripPoints )const
{
  OdDbMline* pMline = OdDbMline::cast(ent);
  unsigned int size = gripPoints.size();
  unsigned int nVerts = pMline->numVertices();
  gripPoints.resize( size + nVerts );
  OdGePoint3d * pPoint = gripPoints.asArrayPtr() + size;
  for (unsigned int i = 0; i < nVerts; i++)
  {
    *pPoint++ = pMline->vertexAt(i);
  }
  return eOk;
}

OdResult OdDbMlineGripPointsPE::moveGripPointsAt( OdDbEntity* pEnt, const OdIntArray& indices, const OdGeVector3d& offset )
{
  unsigned size = indices.size();
  if ( size == 0 )
    return eOk;

  OdDbMline* pMline = OdDbMline::cast(pEnt);
  for (unsigned int i = 0; i < size; ++i)
  {
    pMline->moveVertexAt(indices[i], pMline->vertexAt(indices[i]) + offset);
  }
  return eOk;
}

OdResult OdDbMlineGripPointsPE::getStretchPoints( const OdDbEntity* ent, OdGePoint3dArray& stretchPoints ) const
{
  return getGripPoints( ent, stretchPoints );
}

OdResult OdDbMlineGripPointsPE::moveStretchPointsAt( OdDbEntity* ent, const OdIntArray& indices, const OdGeVector3d& offset )
{
  return moveGripPointsAt( ent, indices, offset );
}

OdResult OdDbMlineGripPointsPE::getOsnapPoints(const OdDbEntity* ent, 
                                              OdDb::OsnapMode osnapMode, 
                                              OdGsMarker gsSelectionMark, 
                                              const OdGePoint3d& pickPoint,
                                              const OdGePoint3d& lastPoint, 
                                              const OdGeMatrix3d& xWorldToEye, 
                                              OdGePoint3dArray& snapPoints) const
{
  OdResult res;
  OdRxObjectPtrArray arrExploded;
  res = ent->explode(arrExploded);
  if (res != eOk)
    return res;

  for (unsigned int i = 0; i < arrExploded.size(); ++i)
  {
    OdDbEntityPtr pEnt = OdDbEntity::cast(arrExploded[i]);
    if (!pEnt.isNull())
    {
      pEnt->getOsnapPoints( osnapMode, gsSelectionMark,
                                    pickPoint, lastPoint, xWorldToEye, snapPoints);
    }
  }
  return eOk;
}
