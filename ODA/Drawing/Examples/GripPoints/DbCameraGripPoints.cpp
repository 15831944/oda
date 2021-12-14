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
/* OdDbCamera entities.                                                 */
/************************************************************************/
#include "StdAfx.h"
#include "DbCameraGripPoints.h"
#include "DbCamera.h"
#include "DbViewTableRecord.h"
#include "AbstractViewPE.h"

OdResult OdDbCameraGripPointsPE::getGripPoints(const OdDbEntity* pEnt, OdGePoint3dArray& gripPoints) const
{
  unsigned int size = gripPoints.size();
  OdDbCameraPtr pCamera = pEnt;
  OdDbViewTableRecordPtr pView = pCamera->openView(OdDb::kForRead);
  if (!pView.isNull())
  {
    OdAbstractViewPEPtr pAvd(pView);
    OdGePoint3d target = pAvd->target(pView);
    OdGeVector3d dir = pAvd->direction(pView);
    gripPoints.resize(size + 3);
    gripPoints[size + 0] = target + dir;
    gripPoints[size + 1] = target + dir * 0.5;
    gripPoints[size + 2] = target;
  }
  return eOk;
}

OdResult OdDbCameraGripPointsPE::moveGripPointsAt(OdDbEntity* pEnt, const OdIntArray& indices, const OdGeVector3d& offset)
{
  unsigned size = indices.size();
  if (size == 0)
    return eOk;

  OdDbCameraPtr pCamera = pEnt;
  if ((size > 1) || (indices[0] == 1))
  {
    pCamera->transformBy(OdGeMatrix3d::translation(offset));
  }
  else
  {
    OdDbViewTableRecordPtr pView = pCamera->openView(OdDb::kForWrite);
    if (!pView.isNull())
    {
      OdAbstractViewPEPtr pAvd(pView);
      OdGePoint3d target = pAvd->target(pView);
      OdGeVector3d dir = pAvd->direction(pView);
      OdGePoint3d position = target + dir;
      if (indices[0] == 0)
        position += offset;
      else
        target += offset;
      pAvd->setView(pView, target, position - target, pAvd->upVector(pView), pAvd->fieldWidth(pView), pAvd->fieldHeight(pView),
        pAvd->isPerspective(pView));
      pCamera->updateView();
    }
  }

  return eOk;
}

OdResult OdDbCameraGripPointsPE::getStretchPoints(const OdDbEntity* ent, OdGePoint3dArray& stretchPoints) const
{
  return getGripPoints(ent, stretchPoints);
}

OdResult OdDbCameraGripPointsPE::moveStretchPointsAt(OdDbEntity* ent, const OdIntArray& indices, const OdGeVector3d& offset)
{
  return moveGripPointsAt(ent, indices, offset);
}

OdResult OdDbCameraGripPointsPE::getOsnapPoints(const OdDbEntity* /*ent*/,
                                                OdDb::OsnapMode /*osnapMode*/,
                                                OdGsMarker /*gsSelectionMark*/,
                                                const OdGePoint3d& /*pickPoint*/,
                                                const OdGePoint3d& /*lastPoint*/,
                                                const OdGeMatrix3d& /*xWorldToEye*/,
                                                OdGePoint3dArray& /*snapPoints*/) const
{
  return eOk;
}
