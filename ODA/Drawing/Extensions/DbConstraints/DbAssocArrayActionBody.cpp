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
#include "DbAssocArrayActionBody.h"
#include "DbImpAssocActionBody.h"
#include "DbImpAssocDependency.h"
#include "DbCurve.h"
#include "DbGeomRef.h"
#include "DbAssocActionParam.h"

ODRX_DEFINE_MEMBERS_EX(OdDbAssocArrayActionBody,      // ClassName
                       OdDbAssocParamBasedActionBody, // ParentClass
                       DBOBJECT_CONSTR,               // DOCREATE
                       OdDb::kDHL_1021,               // DwgVer
                       OdDb::kMRelease6,              // MaintVer
                       1025,                          // nProxyFlags (kEraseAllowed | kDisableProxyWarning)
                       L"AcDbAssocArrayActionBody",   // DWG class name
                       L"ACDBASSOCARRAYACTIONBODY",   // DxfName
                       L"ObjectDBX Classes",         // AppName
                       OdRx::kMTLoading | OdRx::kHistoryAware);

OdDbAssocArrayActionBody::OdDbAssocArrayActionBody( bool bCreateImpl )
{
  if ( bCreateImpl )
    m_pImpObj = static_cast<OdDbImpAssocActionBody*>(new OdDbImpAssocArrayActionBody());
  else
    m_pImpObj = NULL;
}

OdDbAssocArrayActionBody::~OdDbAssocArrayActionBody()
{
  // via base class // delete m_pImpObj;
}

void OdDbAssocArrayActionBody::composeForLoad(OdDb::SaveType format, OdDb::DwgVersion version, OdDbAuditInfo* pAuditInfo)
{
  static_cast<OdDbImpAssocArrayActionBody *>(m_pImpObj)->composeForLoad( this, format, version, pAuditInfo );
}

const OdDbAssocArrayParametersPtr OdDbAssocArrayActionBody::parameters() const
{
  return static_cast<OdDbImpAssocArrayActionBody *>(m_pImpObj)->parameters();
}

OdDbAssocArrayParametersPtr& OdDbAssocArrayActionBody::parameters()
{
  return static_cast<OdDbImpAssocArrayActionBody *>(m_pImpObj)->parameters();
}

OdResult OdDbAssocArrayActionBody::deleteItem(const OdDbItemLocator& index, 
                                              bool bErase) // = true
{
  assertWriteEnabled();
  return static_cast<OdDbImpAssocArrayActionBody *>(m_pImpObj)->deleteItem( index, bErase );
}

OdResult OdDbAssocArrayActionBody::transformItemBy(const OdDbItemLocator& index,
                                                   const OdGeMatrix3d& xform)
{
  assertWriteEnabled();
  return static_cast<OdDbImpAssocArrayActionBody *>(m_pImpObj)->transformItemBy( index, xform );
}

void OdDbAssocArrayActionBody::getItems(OdArray<OdDbItemLocator>& indices, 
                                        bool skipErased) const // = true
{
  assertReadEnabled();
  return static_cast<OdDbImpAssocArrayActionBody *>(m_pImpObj)->getItems(indices, skipErased);
}

const OdDbAssocArrayItemPtr OdDbAssocArrayActionBody::getItemAt(const OdDbItemLocator& index,
                                                              OdDbFullSubentPath& path) const
{
  assertReadEnabled();
  return static_cast<OdDbImpAssocArrayActionBody *>(m_pImpObj)->getItemAt(index, path, getArrayEntity() );
}

const OdDbAssocArrayItemPtr OdDbAssocArrayActionBody::getItemAt(const OdDbItemLocator& index) const
{
  assertReadEnabled();
  OdDbFullSubentPath path = OdDbFullSubentPath();
  return getItemAt(index, path);
}

const OdDbAssocArrayItemPtr OdDbAssocArrayActionBody::getItemAt(const OdDbFullSubentPath& path) const
{
  assertReadEnabled();
  return static_cast<OdDbImpAssocArrayActionBody *>(m_pImpObj)->getItemAt(path);
}

void OdDbAssocArrayActionBody::evaluateOverride()
{
  assertWriteEnabled();
  static_cast<OdDbImpAssocArrayActionBody *>(m_pImpObj)->evaluateOverride( parentAction() );
}

OdDbObjectId OdDbAssocArrayActionBody::getArrayEntity() const
{
  assertReadEnabled();
  return static_cast<OdDbImpAssocArrayActionBody *>
         (m_pImpObj)->getArrayEntity( parentAction() );
}

bool OdDbAssocArrayActionBody::controlsItem(const OdDbItemLocator &index) const
{
  assertReadEnabled();
  return static_cast<OdDbImpAssocArrayActionBody *>
         (m_pImpObj)->controlsItem( index );
}

OdResult OdDbAssocArrayActionBody::transformBy(const OdGeMatrix3d& xform)
{
  assertWriteEnabled();
  return static_cast<OdDbImpAssocArrayActionBody *>
         (m_pImpObj)->transformBy( xform );
}

OdGeMatrix3d OdDbAssocArrayActionBody::getTransform() const
{
  assertReadEnabled();
  return static_cast<OdDbImpAssocArrayActionBody *>
         (m_pImpObj)->getTransform();
}

OdDbObjectIdArray OdDbAssocArrayActionBody::getSourceEntities() const
{
  assertReadEnabled();
  return static_cast<OdDbImpAssocArrayActionBody *>
         (m_pImpObj)->getSourceEntities( parentAction() );
}

OdResult OdDbAssocArrayActionBody::addSourceEntity(OdDbObjectId entity,
                                                   const OdGePoint3d& basePoint)
{
  assertWriteEnabled();
  return static_cast<OdDbImpAssocArrayActionBody *>
         (m_pImpObj)->addSourceEntity( entity, basePoint, parentAction() );
}

OdResult OdDbAssocArrayActionBody::removeSourceEntity(OdDbObjectId entity)
{
  assertWriteEnabled();
  return static_cast<OdDbImpAssocArrayActionBody *>
         (m_pImpObj)->removeSourceEntity( entity, parentAction() );
}

OdResult OdDbAssocArrayActionBody::setSourceBasePoint(const OdDbVertexRef& basePoint)
{
  assertWriteEnabled();
  return static_cast<OdDbImpAssocArrayActionBody *>
         (m_pImpObj)->setSourceBasePoint( basePoint, parentAction() );
}

OdResult OdDbAssocArrayActionBody::getSourceBasePoint(OdDbVertexRef& vertexRef,
                                                      OdGePoint3d& position) const
{
  assertReadEnabled();
  return static_cast<OdDbImpAssocArrayActionBody *>
         (m_pImpObj)->getSourceBasePoint( vertexRef, position );
}

OdDbObjectId OdDbAssocArrayActionBody::getArraySourceBTR() const
{
  assertReadEnabled();
  return static_cast<OdDbImpAssocArrayActionBody *>
         (m_pImpObj)->getArraySourceBTR( parentAction() );
}

OdResult OdDbAssocArrayActionBody::dragStatusOverride(const OdDb::DragStat status)
{
  ODA_FAIL_ONCE();
  return eNotImplementedYet;
}

inline bool profileGeomentryChangeCheck(OdDbAssocArrayParameters* params, const OdDbAssocDependency* dep, bool &isRelevantDepChange) {
  OdDbObjectId paramId;
  OdDbGeomRefPtr geomRef;
  OdResult res = params->getGeomParam("RowProfile", geomRef, paramId);
  if (res == eOk) {
    OdDbAssocEdgeActionParamPtr edgeActionParam = paramId.openObject();
    OdDbObjectIdArray depids;
    res = edgeActionParam->getDependencies(true, false, depids);
    if (res != eOk || depids.isEmpty() || depids[0] != dep->objectId())
      return false; // it's not profile dependency
    if (geomRef.isNull() || !geomRef->isKindOf(OdDbEdgeRef::desc()))
      return true; // can't check
    OdDbEdgeRefPtr edgeRef = static_cast<OdDbEdgeRef*>(geomRef.get());
    if (edgeRef.isNull())
      return true; // can't check
    OdDbCurvePtr dependentOnCurve = OdDbCurve::cast(dep->dependentOnObject().openObject());
    if (dependentOnCurve.isNull())
      return true; // can't check
    OdGeCurve3d* dependentOnGeCurve;
    res = dependentOnCurve->getOdGeCurve(dependentOnGeCurve);
    if (res != eOk || !dependentOnGeCurve)
      return true; // can't check
    const OdGeCurve3d* profileCurve = edgeRef->curve();
    if (profileCurve)
      isRelevantDepChange = !profileCurve->isEqualTo(*dependentOnGeCurve);
    delete dependentOnGeCurve;
    return true;
  }
  return false; // it's not profile-based array
}

OdResult OdDbAssocArrayActionBody::isRelevantDependencyChangeOverride(const OdDbAssocDependency* dep, bool& isRelevantDepChange) const {
  isRelevantDepChange = true;
  OdDbAssocArrayParametersPtr params = parameters();
  if(::profileGeomentryChangeCheck(params, dep, isRelevantDepChange))
    return eOk;
  // add here other dependency type change checking
  return eOk;
}

OdResult OdDbAssocArrayActionBody::createInstance( const OdDbObjectIdArray& sourceEntites, 
                                                   OdDbVertexRef& basePoint, 
                                                   const OdDbAssocArrayParametersPtr &pParameters, 
                                                   OdDbObjectId& arrayId, 
                                                   OdDbObjectId& actionBodyId )
{
  return OdDbImpAssocArrayActionBody::createInstance( sourceEntites, basePoint, pParameters, arrayId, actionBodyId );
}

OdDbObjectId OdDbAssocArrayActionBody::getControllingActionBody( const OdDbEntity* pEntity, 
                                                                 const OdDbItemLocator* pItemIndex /*= NULL*/ )
{
  return OdDbImpAssocArrayActionBody::getControllingActionBody( pEntity, pItemIndex );
}

OdResult OdDbAssocArrayActionBody::explode( OdDbEntity* pEntity, OdDbObjectIdArray& newIds )
{
  return OdDbImpAssocArrayActionBody::explode( pEntity, newIds );
}

bool OdDbAssocArrayActionBody::isAssociativeArray( const OdDbEntity* pEntity )
{
  return OdDbImpAssocArrayActionBody::isAssociativeArray( pEntity );
}

OdResult OdDbAssocArrayActionBody::resetArrayItems( OdDbObjectId arrayEntityId,
                                                    const OdArray<OdDbItemLocator>& indices, 
                                                    bool resetAll) 
{
  return OdDbImpAssocArrayActionBody::resetArrayItems( arrayEntityId, indices, resetAll);
}

OdResult OdDbAssocArrayActionBody::getArrayItemLocators(const OdDbFullSubentPathArray& subents, 
                                                        OdArray<OdDbItemLocator>& indices)
{
  return OdDbImpAssocArrayActionBody::getArrayItemLocators( subents, indices );
}

void OdDbAssocArrayActionBody::audit( OdDbAuditInfo* pAuditInfo )
{
  OdDbObject::audit( pAuditInfo );
  static_cast<OdDbImpAssocArrayActionBody *>( m_pImpObj )->audit( this, pAuditInfo );
}
