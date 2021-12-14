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
/* OdDbUnderlay entities                                                     */
/************************************************************************/

#include "StdAfx.h"
#include "DbUnderlayGripPoints.h"
#include "DbUnderlayDefinition.h"
#include "Ge/GeLineSeg2d.h"

#define STL_USING_ALGORITHM
#define STL_USING_UTILITY
#include "OdaSTL.h"

OdResult OdDbUnderlayGripPointsPE::checkBorder(const OdDbUnderlayReferencePtr& pRef,
                                               OdDb::OsnapMode osnapMode,
                                               const OdGePoint3d& pickPoint,
                                               OdGePoint3dArray& snapPoints) const
{
  if (pRef.isNull() || osnapMode != OdDb::kOsModeEnd)
    return eNotImplemented;

  OdGePoint2dArray pnts2d;
  if (pRef->isClipped() && pRef->clipBoundary().size())
  { //fill from clip boundary
    pnts2d.insert(pnts2d.begin(), pRef->clipBoundary().asArrayPtr(), pRef->clipBoundary().asArrayPtr() + pRef->clipBoundary().size());
  }
  else
  { //fill from extents
    OdDbUnderlayDefinitionPtr pDef = pRef->definitionId().openObject();
    if (pDef.isNull())
      return eNullEntityPointer;
    OdDbUnderlayItemPtr pItem = pDef->getUnderlayItem();
    if (pItem.isNull())
      return eNullEntityPointer;
    pnts2d.resize(2);
    pItem->getExtents(pnts2d[0], pnts2d[1]);
  }

  if (pnts2d.size() == 2)
  {
    if (pnts2d[0].x > pnts2d[1].x)
      std::swap(pnts2d[0].x, pnts2d[1].x);
    if (pnts2d[0].y > pnts2d[1].y)
      std::swap(pnts2d[0].y, pnts2d[1].y);

    OdGePoint2d pt0 = pnts2d[0], pt1 = pnts2d[1];
    pnts2d.resize(4);
    pnts2d[0].set(pt0.x, pt0.y);
    pnts2d[1].set(pt0.x, pt1.y);
    pnts2d[2].set(pt1.x, pt1.y);
    pnts2d[3].set(pt1.x, pt0.y);
  }
  pnts2d.append(pnts2d[0]);

  for (OdUInt32 f = 0; f < pnts2d.size() - 1; f++)
  {
    OdGeLineSeg2d lSeg(pnts2d[f], pnts2d[f+1]);

    OdGeTol tol(lSeg.length()*0.05);
    if (lSeg.isOn(pickPoint.convert2d(), tol))
    {
      snapPoints.append(OdGePoint3d(lSeg.startPoint().x, lSeg.startPoint().y, 0.));
      snapPoints.append(OdGePoint3d(lSeg.endPoint().x, lSeg.endPoint().y, 0.));
      return eOk;
    }
  }
  return eOk;
}

OdResult OdDbUnderlayGripPointsPE::getOsnapPoints(const OdDbEntity* ent, 
                                             OdDb::OsnapMode osnapMode, 
                                             OdGsMarker gsSelectionMark, 
                                             const OdGePoint3d& pickPoint,
                                             const OdGePoint3d& lastPoint, 
                                             const OdGeMatrix3d& xWorldToEye, 
                                             OdGePoint3dArray& snapPoints) const
{
  OdDbUnderlayReferencePtr pRef = OdDbUnderlayReference::cast(ent);
  if (pRef.isNull())
    return eNullObjectPointer;
  
  OdDbUnderlayDefinitionPtr pDef = pRef->definitionId().openObject(OdDb::kForWrite);
  if (pDef.isNull())
    return eNullObjectPointer;
  if (!pDef->isLoaded())
    return eFileNotFound; //nothing can be rendered

  checkBorder(pRef, osnapMode, pickPoint, snapPoints);
  return eOk;
}
