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
#include "DgElementGripPoints.h"
#include "DgElement.h"
#include "RxObjectImpl.h"
#include "Ge/GeExtents3d.h"

OdResult OdDgElementGripPointsPE::getGripPoints( const OdDgElement* pEnt, OdGePoint3dArray& gripPoints )const
{
  OdGeExtents3d exts;
  OdResult res = pEnt->getGeomExtents(exts);
  if (res == eOk)
  {
    unsigned int size = gripPoints.size();
    gripPoints.resize( size + 1 );
    gripPoints[size] =
        exts.minPoint() + (exts.maxPoint() - exts.minPoint()) / 2.;
  }
  return res;
}

OdResult OdDgElementGripPointsPE::moveGripPointsAt( OdDgElement* pEnt, const OdIntArray& indices, const OdGeVector3d& offset )
{
  if ( indices.empty() )
    return eOk;
  return pEnt->transformBy( OdGeMatrix3d::translation(offset) );
}

OdResult OdDgElementGripPointsPE::getStretchPoints( const OdDgElement* pEnt, OdGePoint3dArray& stretchPoints ) const
{
  return getGripPoints( pEnt, stretchPoints );
}


OdResult OdDgElementGripPointsPE::moveStretchPointsAt( OdDgElement* pEnt, const OdIntArray& indices, const OdGeVector3d& offset )
{
  return moveGripPointsAt( pEnt, indices, offset );
}


OdResult OdDgElementGripPointsPE::getOsnapPoints(const OdDgElement* pEnt, 
                                                OdDgElement::OsnapMode osnapMode, 
                                                OdGsMarker gsSelectionMark, 
                                                const OdGePoint3d& pickPoint,
                                                const OdGePoint3d& lastPoint, 
                                                const OdGeMatrix3d& xWorldToEye, 
                                                OdGePoint3dArray& snapPoints) const
{
  return eOk;
}
