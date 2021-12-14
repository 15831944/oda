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
#include "DbViewport.h"
#include "DbViewportGripPoints.h"
#include "Gs/GsDefs.h" // for OdGsDCRectDouble
#include "AbstractViewPE.h"

OdResult OdDbViewportGripPointsPE::getGripPoints( const OdDbEntity* pEnt, OdGePoint3dArray& gripPoints ) const
{
  OdDbViewportPtr pVpt(pEnt);
  if (!pVpt->isNonRectClipOn())
  {
    OdGePoint3d centerPt = pVpt->centerPoint();
    double widthD = pVpt->width();
    double heightD = pVpt->height();
    OdGeVector3d xAxis = OdGeVector3d::kXAxis * (widthD * 0.5);
    OdGeVector3d yAxis = OdGeVector3d::kYAxis * (heightD * 0.5);
    gripPoints.reserve(4);
    gripPoints.append(centerPt - xAxis - yAxis);
    gripPoints.append(centerPt + xAxis - yAxis);
    gripPoints.append(centerPt + xAxis + yAxis);
    gripPoints.append(centerPt - xAxis + yAxis);
  }
  return eOk;
}

OdResult OdDbViewportGripPointsPE::moveGripPointsAt( OdDbEntity* pEnt, const OdIntArray& indices, const OdGeVector3d& offset )
{
  OdDbViewportPtr pVpt(pEnt);
  OdGsDCRectDouble rect;
  OdGePoint3d centerPt = pVpt->centerPoint();
  double widthD = pVpt->width() * 0.5;
  double heightD = pVpt->height() * 0.5;
  rect.m_min.x = centerPt.x - widthD;
  rect.m_min.y = centerPt.y - heightD;
  rect.m_max.x = centerPt.x + widthD;
  rect.m_max.y = centerPt.y + heightD;
  double *logX[4] = { &(rect.m_min.x), &(rect.m_max.x), &(rect.m_max.x), &(rect.m_min.x) };
  double *logY[4] = { &(rect.m_min.y), &(rect.m_min.y), &(rect.m_max.y), &(rect.m_max.y) };
  bool bMovement[4] = { false, false, false, false };
  bool *bMovX[4] = { bMovement + 0, bMovement + 2, bMovement + 2, bMovement + 0 };
  bool *bMovY[4] = { bMovement + 1, bMovement + 1, bMovement + 3, bMovement + 3 };
  for (OdUInt32 i = 0; i < indices.size(); i++)
  {
    int id = indices[i];
    if (!(*bMovX[id]))
    {
      *logX[id] += offset.x;
      *bMovX[id] = true;
    }
    if (!(*bMovY[id]))
    {
      *logY[id] += offset.y;
      *bMovY[id] = true;
    }
  }
  if (rect.m_min.x > rect.m_max.x) { double tmp = rect.m_min.x; rect.m_min.x = rect.m_max.x; rect.m_max.x = tmp; }
  if (rect.m_min.y > rect.m_max.y) { double tmp = rect.m_min.y; rect.m_min.y = rect.m_max.y; rect.m_max.y = tmp; }
  double newWidth = rect.m_max.x - rect.m_min.x;
  double newHeight = rect.m_max.y - rect.m_min.y;
  OdGePoint3d newCenter(rect.m_min.x + newWidth * 0.5, rect.m_min.y + newHeight * 0.5, centerPt.z);
  if (OdNonZero(newWidth) && OdNonZero(newHeight) && !pVpt->isPerspectiveOn())
  {
    widthD *= 2; heightD *= 2;
    OdAbstractViewPEPtr pViewPE(pVpt);
    OdGePoint3d viewTarget = pViewPE->target(pVpt);
    OdGeVector3d zAxis = pViewPE->direction(pVpt);
    OdGeVector3d yAxis = pViewPE->upVector(pVpt);
    OdGeVector3d xAxis = yAxis.crossProduct(zAxis).normal();
    double fieldHeight = pViewPE->fieldHeight(pVpt) / heightD * newHeight;
    double fieldWidth = fieldHeight / heightD * widthD;
    OdGeVector3d diff = (newCenter - centerPt) / newHeight * fieldHeight;
    pViewPE->setView(pVpt, pViewPE->target(pVpt) + xAxis * diff.x + yAxis * diff.y,
                     zAxis, yAxis, fieldWidth, fieldHeight, false);
  }
  pVpt->setCenterPoint(newCenter);
  pVpt->setWidth(newWidth);
  pVpt->setHeight(newHeight);
  return eOk;
}

OdResult OdDbViewportGripPointsPE::getStretchPoints( const OdDbEntity* pEnt, OdGePoint3dArray& stretchPoints ) const
{
  return getGripPoints( pEnt, stretchPoints );
}

OdResult OdDbViewportGripPointsPE::moveStretchPointsAt( OdDbEntity* pEnt, const OdIntArray& indices, const OdGeVector3d& offset )
{
  return moveGripPointsAt( pEnt, indices, offset );
}

OdResult OdDbViewportGripPointsPE::getOsnapPoints(const OdDbEntity* /*pEnt*/,
                                                  OdDb::OsnapMode /*osnapMode*/,
                                                  OdGsMarker /*gsSelectionMark*/,
                                                  const OdGePoint3d& /*pickPoint*/,
                                                  const OdGePoint3d& /*lastPoint*/,
                                                  const OdGeMatrix3d& /*xWorldToEye*/,
                                                  OdGePoint3dArray& /*snapPoints*/) const
{
  return eOk;
}
