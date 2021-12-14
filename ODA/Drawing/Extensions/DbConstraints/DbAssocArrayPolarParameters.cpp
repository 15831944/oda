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
#include "DbAssocArrayPolarParameters.h"
#include "DbImpAssocArrayParameters.h"

ODRX_DEFINE_MEMBERS_EX(OdDbAssocArrayPolarParameters,    // ClassName
                       OdDbAssocArrayCommonParameters,   // ParentClass
                       RXIMPL_CONSTR,                    // DOCREATE (RXIMPL_CONSTR DBOBJECT_CONSTR)
                       OdDb::kDHL_1024,                  // DwgVer
                       OdDb::kMRelease6,                 // MaintVer
                       1025,                             // nProxyFlags (kEraseAllowed | kDisableProxyWarning)
                       L"AcDbAssocArrayPolarParameters", // DWG class name
                       L"ACDBASSOCARRAYPOLARPARAMETERS", // DxfName
                       L"ObjectDBX Classes",             // AppName
                       OdRx::kMTLoading | OdRx::kHistoryAware);

OdDbAssocArrayPolarParameters::OdDbAssocArrayPolarParameters(double angle, 
                                                             double rowSpacing,
                                                             double levelSpacing, 
                                                             int itemCount, 
                                                             int rowCount, 
                                                             int levelCount, 
                                                             double rowElevation) 
 : OdDbAssocArrayCommonParameters(new OdDbImpAssocArrayPolarParameters(OdaToRadian(angle), 
                                                                       rowSpacing, levelSpacing, itemCount,
                                                                       rowCount, levelCount, rowElevation))
{
}                                                      

//protected:
OdDbAssocArrayPolarParameters::OdDbAssocArrayPolarParameters(OdDbImpAssocArrayPolarParameters* pImpl)
  : OdDbAssocArrayCommonParameters(pImpl)
{
}                                                      

OdDbAssocArrayPolarParameters::~OdDbAssocArrayPolarParameters()
{
}                                                      

int OdDbAssocArrayPolarParameters::itemCount(OdString& expression, OdString& evaluatorId) const
{
  OdDbImpAssocArrayPolarParameters* pImpl = static_cast<OdDbImpAssocArrayPolarParameters*>(m_pImpl);
  return pImpl->itemCount(expression, evaluatorId);
}                                                      

int OdDbAssocArrayPolarParameters::itemCount() const
{
  OdString expression, evaluatorId;
  return itemCount(expression, evaluatorId);
}

int OdDbAssocArrayPolarParameters::itemCount(OdString& expression) const
{
  OdString evaluatorId;
  return itemCount(expression, evaluatorId);
}

double OdDbAssocArrayPolarParameters::angleBetweenItems(OdString& expression,
                                                        OdString& evaluatorId) const
{
  OdDbImpAssocArrayPolarParameters* pImpl = static_cast<OdDbImpAssocArrayPolarParameters*>(m_pImpl);
  return OdaToDegree(pImpl->angleBetweenItems(expression, evaluatorId));
}                                                      

double OdDbAssocArrayPolarParameters::angleBetweenItems() const
{
  OdString expression, evaluatorId;
  return angleBetweenItems(expression, evaluatorId);
}

double OdDbAssocArrayPolarParameters::angleBetweenItems(OdString& expression) const
{
  OdString evaluatorId;
  return angleBetweenItems(expression, evaluatorId);
}

double OdDbAssocArrayPolarParameters::fillAngle(OdString& expression, OdString& evaluatorId) const
{
  OdDbImpAssocArrayPolarParameters* pImpl = static_cast<OdDbImpAssocArrayPolarParameters*>(m_pImpl);
  return OdaToDegree(pImpl->fillAngle(expression, evaluatorId));
}                                                      

double OdDbAssocArrayPolarParameters::fillAngle() const
{
  OdString expression, evaluatorId;
  return fillAngle(expression, evaluatorId);
}

double OdDbAssocArrayPolarParameters::fillAngle(OdString& expression) const
{
  OdString evaluatorId;
  return fillAngle(expression, evaluatorId);
}

double OdDbAssocArrayPolarParameters::startAngle(OdString& expression, 
                                                 OdString& evaluatorId) const
{
  OdDbImpAssocArrayPolarParameters* pImpl = static_cast<OdDbImpAssocArrayPolarParameters*>(m_pImpl);
  return OdaToDegree(pImpl->startAngle(expression, evaluatorId));
}                                                      

OdDbAssocArrayPolarParameters::Direction OdDbAssocArrayPolarParameters::direction() const
{
  OdDbImpAssocArrayPolarParameters* pImpl = static_cast<OdDbImpAssocArrayPolarParameters*>(m_pImpl);
  return static_cast<OdDbAssocArrayPolarParameters::Direction>(pImpl->direction());
}                                                      

bool OdDbAssocArrayPolarParameters::rotateItems() const
{
  OdDbImpAssocArrayPolarParameters* pImpl = static_cast<OdDbImpAssocArrayPolarParameters*>(m_pImpl);
  return pImpl->rotateItems();
}                                                      

double OdDbAssocArrayPolarParameters::radius(OdString& expression, OdString& evaluatorId) const
{
  OdDbImpAssocArrayPolarParameters* pImpl = static_cast<OdDbImpAssocArrayPolarParameters*>(m_pImpl);
  return pImpl->radius(expression, evaluatorId);
}                                                      

double OdDbAssocArrayPolarParameters::radius() const
{
  OdString expression, evaluatorId;
  return radius(expression, evaluatorId);
}

double OdDbAssocArrayPolarParameters::radius(OdString& expression) const
{
  OdString evaluatorId;
  return radius(expression, evaluatorId);
}

OdResult OdDbAssocArrayPolarParameters::setItemCount(int nItems, 
                                                     const OdString& expression, // = OdString::kEmpty
                                                     const OdString& evaluatorId, // = OdString::kEmpty
                                                     OdString& pErrorMessage) // = NULL
{
  OdDbImpAssocArrayPolarParameters* pImpl = static_cast<OdDbImpAssocArrayPolarParameters*>(m_pImpl);
  return pImpl->setItemCount(nItems, expression, evaluatorId, pErrorMessage);
}                                                      

OdResult OdDbAssocArrayPolarParameters::setAngleBetweenItems(double angle, 
                                                             const OdString& expression, // = OdString::kEmpty
                                                             const OdString& evaluatorId, // = OdString::kEmpty
                                                             OdString& pErrorMessage) // = NULL
{
  OdDbImpAssocArrayPolarParameters* pImpl = static_cast<OdDbImpAssocArrayPolarParameters*>(m_pImpl);
  return pImpl->setAngleBetweenItems(OdaToRadian(angle), expression, evaluatorId, pErrorMessage);
}                                                      

OdResult OdDbAssocArrayPolarParameters::setFillAngle(double fillAngle, 
                                                     const OdString& expression, // = OdString::kEmpty
                                                     const OdString& evaluatorId, // = OdString::kEmpty
                                                     OdString& pErrorMessage) // = NULL
{
  OdDbImpAssocArrayPolarParameters* pImpl = static_cast<OdDbImpAssocArrayPolarParameters*>(m_pImpl);
  return pImpl->setFillAngle(OdaToRadian(fillAngle), expression, evaluatorId, pErrorMessage);
}                                                      

OdResult OdDbAssocArrayPolarParameters::setStartAngle(double angle, 
                                                      const OdString& expression, // = OdString::kEmpty
                                                      const OdString& evaluatorId, // = OdString::kEmpty
                                                      OdString& pErrorMessage) // = NULL
{
  OdDbImpAssocArrayPolarParameters* pImpl = static_cast<OdDbImpAssocArrayPolarParameters*>(m_pImpl);
  return pImpl->setStartAngle(OdaToRadian(angle), expression, evaluatorId, pErrorMessage);
}                                                      

OdResult OdDbAssocArrayPolarParameters::setDirection(OdDbAssocArrayPolarParameters::Direction direction)
{
  OdDbImpAssocArrayPolarParameters* pImpl = static_cast<OdDbImpAssocArrayPolarParameters*>(m_pImpl);
  return pImpl->setDirection(direction);
}                                                      

OdResult OdDbAssocArrayPolarParameters::setRotateItems(bool bRotateItems)
{
  OdDbImpAssocArrayPolarParameters* pImpl = static_cast<OdDbImpAssocArrayPolarParameters*>(m_pImpl);
  return pImpl->setRotateItems(bRotateItems);
}                                                      

OdResult OdDbAssocArrayPolarParameters::setRadius(double radius, 
                                                  const OdString& expression, // = OdString::kEmpty
                                                  const OdString& evaluatorId, // = OdString::kEmpty
                                                  OdString& pErrorMessage) // = NULL
{
  OdDbImpAssocArrayPolarParameters* pImpl = static_cast<OdDbImpAssocArrayPolarParameters*>(m_pImpl);
  return pImpl->setRadius(radius, expression, evaluatorId, pErrorMessage);
}                                                      

OdResult OdDbAssocArrayPolarParameters::getItems(OdArray<OdDbAssocArrayItemPtr>& items) const
{
  return m_pImpl->getItems( items );
}                                                      

OdResult OdDbAssocArrayPolarParameters::getItemPosition(const OdDbItemLocator& locator,
                                                        OdGePoint3d& position, 
                                                        OdGeMatrix3d& xform) const
{
  return m_pImpl->getItemPosition( locator, position, xform );
}
