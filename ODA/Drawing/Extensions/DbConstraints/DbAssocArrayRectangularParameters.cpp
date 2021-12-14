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
#include "DbAssocArrayRectangularParameters.h"
#include "DbImpAssocArrayParameters.h"
#include "DbAssocArrayActionBody.h"

ODRX_DEFINE_MEMBERS_EX(OdDbAssocArrayRectangularParameters,    // ClassName
                       OdDbAssocArrayCommonParameters,         // ParentClass
                       RXIMPL_CONSTR,                          // DOCREATE (RXIMPL_CONSTR DBOBJECT_CONSTR)
                       OdDb::kDHL_1024,                        // DwgVer
                       OdDb::kMRelease6,                       // MaintVer
                       1025,                                   // nProxyFlags (kEraseAllowed | kDisableProxyWarning)
                       L"AcDbAssocArrayRectangularParameters", // DWG class name
                       L"ACDBASSOCARRAYRECTANGULARPARAMETERS", // DxfName
                       L"ObjectDBX Classes",                   // AppName
                       OdRx::kMTLoading | OdRx::kHistoryAware);

OdDbAssocArrayRectangularParameters::OdDbAssocArrayRectangularParameters(double columnSpacing,
                                                                         double rowSpacing,
                                                                         double levelSpacing,
                                                                         int columnCount,
                                                                         int rowCount,
                                                                         int levelCount,
                                                                         double rowElevation,
                                                                         double axesAngle)
  : OdDbAssocArrayCommonParameters(new OdDbImpAssocArrayRectangularParameters(columnSpacing, rowSpacing, levelSpacing,
                                                                              columnCount, rowCount, levelCount,
                                                                              rowElevation, OdaToRadian(axesAngle)))
{
}


OdDbAssocArrayRectangularParameters::OdDbAssocArrayRectangularParameters(OdDbImpAssocArrayRectangularParameters* pImpl)
  : OdDbAssocArrayCommonParameters(pImpl)
{
}                                                      

OdDbAssocArrayRectangularParameters::~OdDbAssocArrayRectangularParameters()
{
}

int OdDbAssocArrayRectangularParameters::columnCount(OdString& expression, 
                                                     OdString& evaluatorId) const
{
  OdDbImpAssocArrayRectangularParameters* pImpl = static_cast<OdDbImpAssocArrayRectangularParameters*>(m_pImpl);
  return pImpl->ColumnCount(expression, evaluatorId);
}

int OdDbAssocArrayRectangularParameters::columnCount() const
{
  OdString expression, evaluatorId;
  return columnCount(expression, evaluatorId);
}

int OdDbAssocArrayRectangularParameters::columnCount(OdString& expression) const
{
  OdString evaluatorId;
  return columnCount(expression, evaluatorId);
}

double OdDbAssocArrayRectangularParameters::columnSpacing(OdString& expression, 
                                                          OdString& evaluatorId) const
{
  OdDbImpAssocArrayRectangularParameters* pImpl = static_cast<OdDbImpAssocArrayRectangularParameters*>(m_pImpl);
  return pImpl->ColumnSpacing(expression, evaluatorId);
}

double OdDbAssocArrayRectangularParameters::columnSpacing() const
{
  OdString expression, evaluatorId;
  return columnSpacing(expression, evaluatorId);
}

double OdDbAssocArrayRectangularParameters::columnSpacing(OdString& expression) const
{
  OdString evaluatorId;
  return columnSpacing(expression, evaluatorId);
}

double OdDbAssocArrayRectangularParameters::axesAngle(OdString& expression, 
                                                      OdString& evaluatorId) const
{
  OdDbImpAssocArrayRectangularParameters* pImpl = static_cast<OdDbImpAssocArrayRectangularParameters*>(m_pImpl);
  return OdaToDegree(pImpl->AxesAngle(expression, evaluatorId));
  
}

double OdDbAssocArrayRectangularParameters::axesAngle() const
{
  OdString expression, evaluatorId;
  return axesAngle(expression, evaluatorId);
}

double OdDbAssocArrayRectangularParameters::axesAngle(OdString& expression) const
{
  OdString evaluatorId;
  return axesAngle(expression, evaluatorId);
}

OdGeVector3d OdDbAssocArrayRectangularParameters::axisDirection(OdGeVector3d* pYAxis) const // = NULL
{
  OdDbImpAssocArrayRectangularParameters* pImpl = static_cast<OdDbImpAssocArrayRectangularParameters*>(m_pImpl);
  return pImpl->axisDirection(pYAxis);
}

OdResult OdDbAssocArrayRectangularParameters::setColumnCount(int nColumns, 
                                                             const OdString& expression, // = OdString::kEmpty
                                                             const OdString& evaluatorId, // = OdString::kEmpty
                                                             OdString& pErrorMessage) // = NULL
{
  OdDbImpAssocArrayRectangularParameters* pImpl = static_cast<OdDbImpAssocArrayRectangularParameters*>(m_pImpl);
  return pImpl->setColumnCount(nColumns, expression, evaluatorId, pErrorMessage);

}

OdResult OdDbAssocArrayRectangularParameters::setColumnSpacing(double offset, 
                                                               const OdString& expression, // = OdString::kEmpty
                                                               const OdString& evaluatorId, // = OdString::kEmpty
                                                               OdString& pErrorMessage) // = NULL
{
  OdDbImpAssocArrayRectangularParameters* pImpl = static_cast<OdDbImpAssocArrayRectangularParameters*>(m_pImpl);
  return pImpl->setColumnSpacing(offset, expression, evaluatorId, pErrorMessage);
}

OdResult OdDbAssocArrayRectangularParameters::setAxesAngle(double axesAngle, 
                                                           const OdString& expression, // = OdString::kEmpty
                                                           const OdString& evaluatorId, // = OdString::kEmpty
                                                           OdString& pErrorMessage) // = NULL
{
  OdDbImpAssocArrayRectangularParameters* pImpl = static_cast<OdDbImpAssocArrayRectangularParameters*>(m_pImpl);
  return pImpl->setAxesAngle(OdaToRadian(axesAngle), expression, evaluatorId, pErrorMessage);
}

OdResult OdDbAssocArrayRectangularParameters::setAxisDirection(const OdGeVector3d& xAxis)
{
  OdDbImpAssocArrayRectangularParameters* pImpl = static_cast<OdDbImpAssocArrayRectangularParameters*>(m_pImpl);
  return pImpl->setAxisDirection(xAxis);
}

OdResult OdDbAssocArrayRectangularParameters::getItemPosition( const OdDbItemLocator& locator,
                                                               OdGePoint3d& position,
                                                               OdGeMatrix3d& xform) const
{
  return m_pImpl->getItemPosition( locator, position, xform );
}

OdResult OdDbAssocArrayRectangularParameters::getItems(OdArray<OdDbAssocArrayItemPtr>& items) const
{  
  return m_pImpl->getItems( items );
}                                                      

