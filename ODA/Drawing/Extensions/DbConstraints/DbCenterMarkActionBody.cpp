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

#include "OdaCommon.h"
#include "DbCenterMarkActionBody.h"
#include "DbImpCenterMarkActionBody.h"
#include "DbSmartCenterObjectsParameters.h"
#include "Ge/GeCircArc3d.h"

ODRX_DEFINE_MEMBERS_EX(OdDbCenterMarkActionBody,      // ClassName
  OdDbSmartCenterActionBody,                          // ParentClass
  DBOBJECT_CONSTR,                                    // DOCREATE
  OdDb::kDHL_1027,                                    // DwgVer
  OdDb::kMRelease125,                                 // MaintVer
  1153,                                               // nProxyFlags (kEraseAllowed | kDisableProxyWarning)
  L"AcDbCenterMarkActionBody",                        // DWG class name
  L"AcDbCenterMarkActionBody",                        // DxfName
  L"AcDbSmartCenter",                                 // AppName
  OdRx::kMTLoading | OdRx::kHistoryAware);

OdDbCenterMarkActionBody::OdDbCenterMarkActionBody(bool bCreateImpl)
{
  if (bCreateImpl)
    m_pImpObj = static_cast<OdDbImpAssocActionBody*>(new OdDbImpCenterMarkActionBody());
  else
    m_pImpObj = NULL;
}

OdDbCenterMarkActionBody::~OdDbCenterMarkActionBody()
{
  // via base class // delete m_pImpObj;
}

//const OdDbSmartCenterMarkParametersPtr OdDbCenterMarkActionBody::parameters() const
//{
//  return static_cast<OdDbImpCenterMarkActionBody *>(m_pImpObj)->parameters();
//}
//
//OdDbSmartCenterMarkParametersPtr& OdDbCenterMarkActionBody::parameters()
//{
//  return static_cast<OdDbImpCenterMarkActionBody *>(m_pImpObj)->parameters();
//}
//
void OdDbCenterMarkActionBody::composeForLoad(OdDb::SaveType format, OdDb::DwgVersion version, OdDbAuditInfo* pAuditInfo)
{
  static_cast<OdDbImpCenterMarkActionBody *>(m_pImpObj)->composeForLoad(this, format, version, pAuditInfo);
}

enum OdResult OdDbCenterMarkActionBody::setHorizontalEndOvershoot(double newOvershoot)
{
  return setGeometryParam(newOvershoot, OD_T("HorizontalEndOvershoot"));
}

enum OdResult OdDbCenterMarkActionBody::setHorizontalStartOvershoot(double newOvershoot)
{
  return setGeometryParam(newOvershoot, OD_T("HorizontalStartOvershoot"));
}

enum OdResult OdDbCenterMarkActionBody::setVerticalEndOvershoot(double newOvershoot)
{
  return setGeometryParam(newOvershoot, OD_T("VerticalEndOvershoot"));
}

enum OdResult OdDbCenterMarkActionBody::setVerticalStartOvershoot(double newOvershoot)
{
  return setGeometryParam(newOvershoot, OD_T("VerticalStartOvershoot"));
}

double OdDbCenterMarkActionBody::horizontalEndOvershoot() const
{
  double ret = -1.0;

  if (getGeometryParam(ret, OD_T("HorizontalEndOvershoot")) != eOk)
    ret = -1.0;

  return ret;
}

double OdDbCenterMarkActionBody::horizontalStartOvershoot() const
{
  double ret = -1.0;

  if (getGeometryParam(ret, OD_T("HorizontalStartOvershoot")) != eOk)
    ret = -1.0;

  return ret;
}

double OdDbCenterMarkActionBody::verticalEndOvershoot() const
{
  double ret = -1.0;

  if (getGeometryParam(ret, OD_T("VerticalEndOvershoot")) != eOk)
    ret = -1.0;

  return ret;
}

double OdDbCenterMarkActionBody::verticalStartOvershoot() const
{
  double ret = -1.0;

  if (getGeometryParam(ret, OD_T("VerticalStartOvershoot")) != eOk)
    ret = -1.0;

  return ret;
}

OdString OdDbCenterMarkActionBody::crossHairGapExpression(void)const
{
  OdString ret = OdString("-1");

  if (getStringGeometryParam(ret, OD_T("CrossHairGapExpression")) != eOk)
    ret = OdString("-1");;

  return ret;
}

OdString OdDbCenterMarkActionBody::crossHairLengthExpression(void)const
{
  OdString ret = OdString("-1");

  if (getStringGeometryParam(ret, OD_T("CrossHairLengthExpression")) != eOk)
    ret = OdString("-1");;

  return ret;
}

enum OdResult OdDbCenterMarkActionBody::setCrossHairGapExpression(OdString const & str)
{
  return setStringGeometryParam(str, OD_T("CrossHairGapExpression"));
}
enum OdResult OdDbCenterMarkActionBody::setCrossHairLengthExpression(OdString const & str)
{
  return setStringGeometryParam(str, OD_T("CrossHairLengthExpression"));
}

enum OdResult OdDbCenterMarkActionBody::setExtensionLinesVisibility(OdDb::Visibility visible)
{
  double dVis = visible == OdDb::kVisible ? 1.0 : 0.0;

  OdResult ret = setGeometryParam(dVis, OD_T("eLeftLine_visibility"));
  OdResult ret1 = setGeometryParam(dVis, OD_T("eRightLine_visibility"), false);
  OdResult ret2 = setGeometryParam(dVis, OD_T("eBottomLine_visibility"), false);
  OdResult ret3 = setGeometryParam(dVis, OD_T("eTopLine_visibility"), false);
  ODA_ASSERT(ret == ret1 && ret == ret2 && ret == ret3);
  return ret;
}

//OdDb::Visibility OdDbCenterMarkActionBody::getExtensionLinesVisibility() const// default return value is true
//{
//  double ret = -1.0;
//  OdDb::Visibility bRet = OdDb::kVisible;
//
//  if (getGeometryParam(ret, OD_T("eLeftLine_visibility")) != eOk)
//  {
//    ret = -1.0;
//    return OdDb::kVisible;
//  }
//
//  if (getGeometryParam(ret, OD_T("eRightLine_visibility")) != eOk)
//  {
//    ret = -1.0;
//    return OdDb::kVisible;
//  }
//
//  if (getGeometryParam(ret, OD_T("eBottomLine_visibility")) != eOk)
//  {
//    ret = -1.0;
//    return OdDb::kVisible;
//  }
//
//  if (getGeometryParam(ret, OD_T("eTopLine_visibility")) != eOk)
//  {
//    ret = -1.0;
//    return OdDb::kVisible;
//  }
//
//  if (ret = 0.0)
//    bRet = OdDb::kInvisible;
//
//  return bRet;
//}

enum OdResult OdDbCenterMarkActionBody::setRotation(double rotation)// Radian
{
  return static_cast<OdDbImpCenterMarkActionBody *>
    (m_pImpObj)->setRotation(rotation, parentAction());
}

double OdDbCenterMarkActionBody::rotation(void)	const// Radian
{
  return static_cast<OdDbImpCenterMarkActionBody *>
    (m_pImpObj)->rotation(parentAction());
}

OdResult OdDbCenterMarkActionBody::setAllOvershoots(double dNewOvershoots)
{
  OdResult ret = setGeometryParam(dNewOvershoots, OD_T("HorizontalEndOvershoot"));
  OdResult ret1 = setGeometryParam(dNewOvershoots, OD_T("HorizontalStartOvershoot"));
  OdResult ret2 = setGeometryParam(dNewOvershoots, OD_T("VerticalEndOvershoot"));
  OdResult ret3 = setGeometryParam(dNewOvershoots, OD_T("VerticalStartOvershoot"));
  ODA_ASSERT(ret == ret1 && ret == ret2 && ret == ret3);
  return ret;
}

OdResult OdDbCenterMarkActionBody::setCenterPoint(const OdGePoint3d& ptCenter)
{
  return static_cast<OdDbImpCenterMarkActionBody *>
    (m_pImpObj)->setCenterPoint(ptCenter, parentAction());
}

OdResult OdDbCenterMarkActionBody::createInstance(
  const OdDbFullSubentPath subentPath,
  OdDbObjectId& centerMarkId,
  OdDbObjectId& actionBodyId)
{
  return OdDbImpCenterMarkActionBody::createInstance(subentPath, centerMarkId, actionBodyId);
}

OdResult OdDbCenterMarkActionBody::getRefCircle(OdGeCircArc3d & arc)const
{
  OdResult res = static_cast<OdDbImpCenterMarkActionBody *>
    (m_pImpObj)->getRefCircle(arc, parentAction());

  if (res == eOk)
    return res;

  // if there is no associated circle, creating new circle using BlockReference params

  OdDbImpAssocActionBody *pPtr = static_cast<OdDbImpAssocActionBody *>(m_pImpObj);
 
  OdDbBlockReferencePtr pBlkRef = OdDbBlockReference::cast(pPtr->getArrayEntity(parentAction()).safeOpenObject(OdDb::kForRead));
  
  double dRadius = 0;
  res = getGeometryParam(dRadius, L"CachedCircleRadius");
  
  OdGeCircArc3d Circle(pBlkRef->position(), pBlkRef->normal(), dRadius);
  arc = Circle;

  return res;
}

OdResult OdDbCenterMarkActionBody::getAllExtensionPoints(OdGePoint3dArray& arrPoints)
{
  OdGePoint3d pt1, pt2, pt3, pt4;
  OdResult res = getAllExtensionPoints(pt1, pt2, pt3, pt4);
  if (res != eOk)
    return res;
  arrPoints.append(pt1);
  arrPoints.append(pt2);
  arrPoints.append(pt3);
  arrPoints.append(pt4);
  return eOk;
}

OdResult OdDbCenterMarkActionBody::getAllExtensionPoints(OdGePoint3d& pt1, OdGePoint3d& pt2, OdGePoint3d& pt3, OdGePoint3d& pt4)
{
  OdGeCircArc3d refCircle;
  OdResult res = getRefCircle(refCircle);
  if (res != eOk)
    return res;

  double dRotation = rotation();
  if (res != eOk)
    dRotation = 0.;

  double dExtension;
  res = getGeometryParam(dExtension, L"HorizontalStartExtension");
  if (res != eOk || dExtension >= 1.e60)
    dExtension = refCircle.radius();

  OdGeVector3d vOffset(-dExtension, 0., 0.);
  vOffset.rotateBy(dRotation, refCircle.normal());

  pt1 = refCircle.center() + vOffset;

  res = getGeometryParam(dExtension, L"HorizontalEndExtension");
  if (res != eOk || dExtension >= 1.e60)
    dExtension = refCircle.radius();

  vOffset.set(dExtension, 0, 0.);
  vOffset.rotateBy(dRotation, refCircle.normal());

  pt2 = refCircle.center() + vOffset;

  res = getGeometryParam(dExtension, L"VerticalStartExtension");
  if (res != eOk || dExtension >= 1.e60)
    dExtension = refCircle.radius();

  vOffset.set(0., -dExtension, 0.);
  vOffset.rotateBy(dRotation, refCircle.normal());

  pt3 = refCircle.center() + vOffset;

  res = getGeometryParam(dExtension, L"VerticalEndExtension");
  if (res != eOk || dExtension >= 1.e60)
    dExtension = refCircle.radius();

  vOffset.set(0., dExtension, 0.);
  vOffset.rotateBy(dRotation, refCircle.normal());

  pt4 = refCircle.center() + vOffset;

  return eOk;
}

OdResult OdDbCenterMarkActionBody::getAllOvershootPoints(OdGePoint3dArray& arrPoints)
{
  OdGePoint3d pt1, pt2, pt3, pt4;
  OdResult res = getAllOvershootPoints(pt1, pt2, pt3, pt4);
  if (res != eOk)
    return res;
  arrPoints.append(pt1);
  arrPoints.append(pt2);
  arrPoints.append(pt3);
  arrPoints.append(pt4);
  return eOk;
}

OdResult OdDbCenterMarkActionBody::getAllOvershootPoints(OdGePoint3d& pt1, OdGePoint3d& pt2, OdGePoint3d& pt3, OdGePoint3d& pt4)
{
  OdGeCircArc3d refCircle;
  OdResult res = getRefCircle(refCircle);
  if (res != eOk)
    return res;

  double dRotation = rotation();
  if (res != eOk)
    dRotation = 0.;

  double dExtension, dOvershoot;
  res = getGeometryParam(dExtension, L"HorizontalStartExtension");
  if (res != eOk || dExtension >= 1.e60)
    dExtension = refCircle.radius();
  res = getGeometryParam(dOvershoot, L"HorizontalStartOvershoot");
  if (res != eOk)
    dOvershoot = 0;

  OdGeVector3d vOffset(-dExtension - dOvershoot, 0., 0.);
  vOffset.rotateBy(dRotation, refCircle.normal());

  pt1 = refCircle.center() + vOffset;

  res = getGeometryParam(dExtension, L"HorizontalEndExtension");
  if (res != eOk || dExtension >= 1.e60)
    dExtension = refCircle.radius();
  res = getGeometryParam(dOvershoot, L"HorizontalEndOvershoot");
  if (res != eOk)
    dOvershoot = 0;

  vOffset.set(dExtension + dOvershoot, 0, 0.);
  vOffset.rotateBy(dRotation, refCircle.normal());

  pt2 = refCircle.center() + vOffset;

  res = getGeometryParam(dExtension, L"VerticalStartExtension");
  if (res != eOk || dExtension >= 1.e60)
    dExtension = refCircle.radius();
  res = getGeometryParam(dOvershoot, L"VerticalStartOvershoot");
  if (res != eOk)
    dOvershoot = 0;

  vOffset.set(0., -dExtension - dOvershoot, 0.);
  vOffset.rotateBy(dRotation, refCircle.normal());

  pt3 = refCircle.center() + vOffset;

  res = getGeometryParam(dExtension, L"VerticalEndExtension");
  if (res != eOk || dExtension >= 1.e60)
    dExtension = refCircle.radius();
  res = getGeometryParam(dOvershoot, L"VerticalEndOvershoot");
  if (res != eOk)
    dOvershoot = 0;

  vOffset.set(0., dExtension + dOvershoot, 0.);
  vOffset.rotateBy(dRotation, refCircle.normal());

  pt4 = refCircle.center() + vOffset;

  return eOk;
}

OdResult OdDbCenterMarkActionBody::setHorizontalEndExtension(double newExtension)
{
  return setGeometryParam(newExtension, OD_T("HorizontalEndExtension"));
}

OdResult OdDbCenterMarkActionBody::setHorizontalStartExtension(double newExtension)
{
  return setGeometryParam(newExtension, OD_T("HorizontalStartExtension"));
}

OdResult OdDbCenterMarkActionBody::setVerticalEndExtension(double newExtension)
{
  return setGeometryParam(newExtension, OD_T("VerticalEndExtension"));
}

OdResult OdDbCenterMarkActionBody::setVerticalStartExtension(double newExtension)
{
  return setGeometryParam(newExtension, OD_T("VerticalStartExtension"));
}

double OdDbCenterMarkActionBody::horizontalEndExtension(void) const
{
  double dExtension = -1.0;
  OdGeCircArc3d refCircle;
  OdResult res = getRefCircle(refCircle);
  if (res != eOk)
    return dExtension;

  double dRotation = rotation();
  if (res != eOk)
    dRotation = 0.;

  res = getGeometryParam(dExtension, L"HorizontalEndExtension");
  if (res != eOk || dExtension >= 1.e60)
    dExtension = refCircle.radius();

  return dExtension;
}

double OdDbCenterMarkActionBody::horizontalStartExtension(void) const
{
  double dExtension = -1.0;
  OdGeCircArc3d refCircle;
  OdResult res = getRefCircle(refCircle);
  if (res != eOk)
    return dExtension;

  double dRotation = rotation();
  if (res != eOk)
    dRotation = 0.;

  res = getGeometryParam(dExtension, L"HorizontalStartExtension");
  if (res != eOk || dExtension >= 1.e60)
    dExtension = refCircle.radius();

  return dExtension;
}

double OdDbCenterMarkActionBody::verticalEndExtension(void) const
{
  double dExtension = -1.0;
  OdGeCircArc3d refCircle;
  OdResult res = getRefCircle(refCircle);
  if (res != eOk)
    return dExtension;

  double dRotation = rotation();
  if (res != eOk)
    dRotation = 0.;

  res = getGeometryParam(dExtension, L"VerticalEndExtension");
  if (res != eOk || dExtension >= 1.e60)
    dExtension = refCircle.radius();

  return dExtension;
}

double OdDbCenterMarkActionBody::verticalStartExtension(void) const
{
  double dExtension = -1.0;
  OdGeCircArc3d refCircle;
  OdResult res = getRefCircle(refCircle);
  if (res != eOk)
    return dExtension;

  double dRotation = rotation();
  if (res != eOk)
    dRotation = 0.;

  res = getGeometryParam(dExtension, L"VerticalStartExtension");
  if (res != eOk || dExtension >= 1.e60)
    dExtension = refCircle.radius();

  return dExtension;
}

OdResult OdDbCenterMarkActionBody::getCenterPoint(OdGePoint3d &pt)
{
  OdGeCircArc3d refCircle;
  OdResult res = getRefCircle(refCircle);
  if (res != eOk)
    return res;

  pt = refCircle.center();
  return eOk;
}
