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
#include "DbDgnUnderlayGripPoints.h"
#include "DbUnderlayReference.h"
#include "DbUnderlayDefinition.h"
#include "RxObjectImpl.h"
#include "Gi/GiViewport.h"
#include "Gi/GiWorldDraw.h"
#include "Gi/GiDummyGeometry.h"
#include "Ge/GeNurbCurve3d.h"
#include "Ge/GeCircArc3d.h"
#include "DbGrip.h"
#include "TDVersion.h"
#include "StaticRxObject.h"

OdResult OdDbDgnUnderlayGripPointsPE::getOsnapPoints(const OdDbEntity* ent, 
                                             OdDb::OsnapMode osnapMode, 
                                             OdGsMarker gsSelectionMark, 
                                             const OdGePoint3d& pickPoint,
                                             const OdGePoint3d& lastPoint, 
                                             const OdGeMatrix3d& xWorldToEye, 
                                             OdGePoint3dArray& snapPoints) const
{
  OdRxModulePtr pModule = odrxDynamicLinker()->loadModule(ExDgnGripPointsModuleName);
  if (pModule.isNull())
    return eTxError;

  OdResult res = OdDbUnderlayGripPointsPE::getOsnapPoints(ent, osnapMode, gsSelectionMark, pickPoint, lastPoint, xWorldToEye, snapPoints);
  if (eOk == res)
  {
    OdDbUnderlayReferencePtr pDgnRef = OdDbUnderlayReference::cast(ent);
    OdDbUnderlayDefinitionPtr pDef = pDgnRef->definitionId().openObject();
    OdDbUnderlayItemPtr pItem = pDef->getUnderlayItem();
    OdIntArray dummy; // NB: last parameter of this call needs to be changed to last parameter of this function
    return pItem->getOsnapPoints(pDgnRef->transform(), osnapMode, gsSelectionMark, pickPoint, lastPoint, xWorldToEye, OdGeMatrix3d::kIdentity, snapPoints, dummy);
  }
  return res;
}
