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


#ifndef _EX_DBCURVE_INTERSECTIONPE_
#define _EX_DBCURVE_INTERSECTIONPE_


#include "DbEntityIntersectionPE.h"

/** Description:
    This class defines the interface for the Entity Intersection Protocol Extension classes.

    Library:
    TD_Db

    <group OdDb_Classes>
*/
class ExDbCurveIntersectionPE : public OdDbEntityIntersectionPE
{
public:
  /**
    Description:
    Intersects this entity with other entity if possible.

    Arguments:
    pEnt   (I) Pointer to other entity
    inType (I) Determines type of intersection to be calculated
  */
  virtual OdResult intersectWith(
    const OdDbEntity* pThisEnt, const OdDbEntity* pEnt,
    OdDb::Intersect intType, OdGePoint3dArray& points,
    OdGsMarker thisGsMarker = 0, OdGsMarker otherGsMarker = 0) const;

  virtual OdResult intersectWith(
    const OdDbEntity* pThisEnt, const OdDbEntity* pEnt,
    OdDb::Intersect intType, const OdGePlane& projPlane, OdGePoint3dArray& points,
    OdGsMarker thisGsMarker = 0, OdGsMarker otherGsMarker = 0) const;

  /**
    Description:
    Uses the bounding box of this object to determine an intersection array of points.
  */
  virtual OdResult boundingBoxIntersectWith(
    const OdDbEntity* pThisEnt, const OdDbEntity* pEnt,
    OdDb::Intersect intType, OdGePoint3dArray& points,
    OdGsMarker thisGsMarker, OdGsMarker otherGsMarker) const;

  virtual OdResult boundingBoxIntersectWith(
    const OdDbEntity* pThisEnt, const OdDbEntity* pEnt,
    OdDb::Intersect intType, const OdGePlane& projPlane, OdGePoint3dArray& points,
    OdGsMarker thisGsMarker, OdGsMarker otherGsMarker) const;
};

/** Description:
  This template class is a specialization of the OdSmartPtr class
  for OdDbEntityIntersectionPE object pointers.
*/
typedef OdSmartPtr<ExDbCurveIntersectionPE> ExDbCurveIntersectionPEPtr;

#endif //_OD_DB_INTERSECTIONPE_
