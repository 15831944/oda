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
#include "DbAssocActionBody.h"
#include "AssocActionPE.h"
#include "DbAssoc2dConstraintGroup.h"
#include "DbImpAssoc2dConstraintGroup.h"
#include "DbAssocGeomDependency.h"
#include "DbAssocValueDependency.h"
#include "DbAssocDimDependencyBodyBase.h"
#include "DbAssocNetCloneCtx.h"
#include "DbAssocVariable.h"
#include "DbAssocNetwork.h"
#include "OdConstraintGroupNodeImpl.h"
#include "OdCompositeConstraintImpl.h"
#include "OdDistanceConstraintImpl.h"
#include "OdImplicitConstr.h"
#include "OdExplicitConstr.h"
#include "DbFiler.h"

#include "DbIdMapping.h"
#include "OdConstrainedConstructionLineImpl.h"
#include "OdConstrainedRigidSetImpl.h"
#include "OdParallelConstraintImpl.h"
#include "OdDbAssocCurvePersSubentIdPE.h"

#include "DbBlockReference.h"
#include "DbSpline.h"
#include "StringArray.h"

#define STL_USING_ALGORITHM
#define STL_USING_SET
#include "OdaSTL.h"

#include "DbAudit.h"
#include "DbHostAppServices.h"
#include "RxEvent.h"
#include "Ge/GeScale3d.h"
#include "OdConstrainedSplineImpl.h"
#include "DbDictionary.h"

OdDbImpAssoc2dConstraintGroup::OdDbImpAssoc2dConstraintGroup()
  : OdDbImpAssocAction()
  , m_plane()
  , m_seedId(0)
  , m_Unk_Bool(false)
{
  m_isBase = false;
}

OdDbImpAssoc2dConstraintGroup::~OdDbImpAssoc2dConstraintGroup()
{
  m_mapNodes.clear();
}

OdResult OdDbImpAssoc2dConstraintGroup::setWorkPlane(const OdGePlane& plane)
{
  m_plane = plane;
  return eOk;
}

const OdGePlane& OdDbImpAssoc2dConstraintGroup::getWorkPlane() const
{
  return m_plane;
}

OdResult OdDbImpAssoc2dConstraintGroup::transformActionBy(OdDbAssocAction* thisAction, const OdGeMatrix3d& xform) {
  OdResult res = eOk;
  OdGeMatrix3d x = xform;
  {
    OdGeScale3d scale;
    bool hasScale = !scale.removeScale(x).isEqualTo(OdGeScale3d::kIdentity);
    bool isNotOrtho = !x.isUniScaledOrtho();
    if (isNotOrtho || hasScale) { // remove constraints nonsupporting this transform if any
      OdArray<OdGeomConstraint*> constraints;
      res = getConstraints(constraints);
      if (res != eOk)
        return res;
      for (OdArray<OdGeomConstraint*>::size_type i = 0; i < constraints.size(); ++i) {
        OdGeomConstraint* constraint = constraints[i];
        if (isNotOrtho || (hasScale && constraint->isKindOf(OdExplicitConstraint::desc())))
          deleteConstraint(constraint);
      }
    }
  }

  m_plane.transformBy(x);
  thisAction->setStatus(kChangedDirectlyAssocStatus, true, false);

  return res;
}

#if 0
int OdDbImpAssoc2dConstraintGroup::getDOF() const
{
  throw OdError(eNotImplemened);
  return 1;
}
#endif

OdDbAssoc2dConstraintGroup::SolutionStatus
OdDbImpAssoc2dConstraintGroup::solutionStatus(const OdDbAssoc2dConstraintGroup* pGroup,
  bool bAlsoCheckForConstraints) const
{
  OdSmartPtr<AssocActionPE> pPE = pGroup->isA()->getX(AssocActionPE::desc());
  if (!pPE.isNull())
  {
    pPE->runDiagnostics(pGroup);
  }

  return m_groupState;
}

OdDbAssoc2dConstraintGroup::SolutionStatus
OdDbImpAssoc2dConstraintGroup::geometryStatus(const OdDbAssoc2dConstraintGroup* pGroup, 
  const OdConstrainedGeometry* pConsGeom) const
{
  OdSmartPtr<AssocActionPE> pPE = pGroup->isA()->getX(AssocActionPE::desc());
  if (!pPE.isNull())
  {
    pPE->runDiagnostics(pGroup);
  }
  if (m_statusNodes.find(pConsGeom->nodeId()) != m_statusNodes.end())
  {
    std::map<OdConstraintGroupNodeId, OdDbAssoc2dConstraintGroup::SolutionStatus>::const_iterator st = m_statusNodes.find(pConsGeom->nodeId());
    if (st != m_statusNodes.end())
    {
      return st->second;
    }
  }

  return OdDbAssoc2dConstraintGroup::kNotAvailable;
}

OdDbAssoc2dConstraintGroup::SolutionStatus
OdDbImpAssoc2dConstraintGroup::constraintStatus(const OdDbAssoc2dConstraintGroup* pGroup, 
  const OdGeomConstraint* pConstraint) const
{
  OdSmartPtr<AssocActionPE> pPE = pGroup->isA()->getX(AssocActionPE::desc());
  if (!pPE.isNull())
  {
    pPE->runDiagnostics(pGroup);
  }
  if (m_statusNodes.find(pConstraint->nodeId()) != m_statusNodes.end())
  {
    std::map<OdConstraintGroupNodeId, OdDbAssoc2dConstraintGroup::SolutionStatus>::const_iterator st = m_statusNodes.find(pConstraint->nodeId());
    if (st != m_statusNodes.end())
    {
      return st->second;
    }
  }

  return OdDbAssoc2dConstraintGroup::kNotAvailable;
}

void OdDbImpAssoc2dConstraintGroup::onAddConstrainedGeometry(const OdDbFullSubentPath& subentPath,
  OdConstrainedGeometry* pConsGeom)
{
  // check if edge of polyline was added
  OdDbObjectId objId = subentPath.objectIds().last();
  OdDbObjectPtr pObj = objId.openObject();
  if ( pObj->isKindOf(OdDbPolyline::desc()) && subentPath.subentId().type() == OdDb::kEdgeSubentType ) {
    // look for other adjacent edges of polyline and add coincidence constraint between corresponding vertexes
    // collect constrained implicit points of new created edge
    OdConstrainedCurve* pConsGeomCurve = OdConstrainedCurve::cast(pConsGeom);
    OdArray<OdConstrainedImplicitPoint*> aConsGeomPts;
    if(pConsGeomCurve)
      pConsGeomCurve->getConstrainedImplicitPoints(aConsGeomPts);
    // consider all constrained geometries in the constraint group
    OdArray<OdConstrainedGeometry*> aConstGeoms;
    getConstrainedGeometries(aConstGeoms);
    for ( unsigned int i = 0; i < aConstGeoms.length(); ++i ) {
      OdConstrainedCurve* pCurve = OdConstrainedCurve::cast(aConstGeoms[i]);
      if (!pCurve || pCurve == pConsGeomCurve)
        continue;
      OdDbFullSubentPathArray aFullSubentPaths;
      pCurve->getFullSubentPaths(aFullSubentPaths);
      if (aFullSubentPaths.empty())
        continue;
      OdDbFullSubentPath path = aFullSubentPaths[0];
      if (path.objectIds().last() == objId) {
        // edge from the same polyline was found, obtain its constrained implicit points
        OdArray<OdConstrainedImplicitPoint*> aPts;
        pCurve->getConstrainedImplicitPoints(aPts);
        // find constrained implicit points that have the same FullSubentPath
        // and create coincidence constraint between them
        for ( unsigned int j = 0; j < aPts.length(); ++j ) {
          aPts[j]->getFullSubentPaths(aFullSubentPaths);
          OdDbFullSubentPath ptPath = aFullSubentPaths[0];
          for ( unsigned int k = 0; k < aConsGeomPts.length(); ++k ) {
            aConsGeomPts[k]->getFullSubentPaths(aFullSubentPaths);
            if (aFullSubentPaths[0] == ptPath) {
              OdSmartPtr<OdPointCoincidenceConstraint> pCo = OdPointCoincidenceConstraint::createObject();
              tieNode(pCo, pCurve->owningConstraintGroupId());
              tieGeomsAndCnstr(aPts[j], aConsGeomPts[k], pCo);
            }
          }
        }
      }
    }
  }
}

OdResult OdDbImpAssoc2dConstraintGroup::moveConstrainedGeometry(
  const OdDbFullSubentPath& subentPath,
  OdDbAssocGeomDependency * pExistingDep, OdDbAssoc2dConstraintGroup *pGroup)
{
  OdConstrainedGeometry* pConsGeom = 0;
  if ( getConstrainedGeometry(subentPath, pConsGeom, true) == eOk )
    return eOk;

  addDependency(pExistingDep->objectId(), true, pGroup);

  addConstrainedGeometry(pExistingDep);

  onAddConstrainedGeometry(subentPath, pConsGeom);

  return eOk;
}

OdResult OdDbImpAssoc2dConstraintGroup::addConstrainedGeometry(const OdDbFullSubentPath& subentPath, OdConstrainedGeometry*& pConsGeom,
                                                               OdDbAssoc2dConstraintGroup *pGroup)
{
  if ( getConstrainedGeometry(subentPath, pConsGeom, true) == eOk )
    return eOk;

  OdDbCompoundObjectId cmpObj;
  cmpObj.setFullPath(subentPath.objectIds());

  OdDbAssocGeomDependencyPtr pGeomDep = OdDbAssocGeomDependency::createObject();
  pGeomDep->setOrder(10000);
  
  OdDbObjectId geomDepId = pGroup->database()->addOdDbObject(pGeomDep);

  if ( pGeomDep->attachToObject(cmpObj) != eOk )
  {
    pGeomDep->erase();
    return eBadObjType;
  }
  
  if ( pGeomDep->setSubentity(subentPath.subentId()) != eOk )
  {
    pGeomDep->erase();
    return eBadObjType;
  }

  if ( pGeomDep->isWriteEnabled() )
    pGeomDep->downgradeOpen();
  addDependency(geomDepId, true, pGroup);
  pConsGeom = addConstrainedGeometry(pGeomDep);
  onAddConstrainedGeometry(subentPath, pConsGeom);

  return eOk;
}

OdResult OdDbImpAssoc2dConstraintGroup::deleteConstrainedGeometry(const OdDbObjectIdArray& aDestDepToRemove)
{
  std::map<OdDbObjectId, OdConstrainedGeometry*> aConstGeom;
  for (OdConstraintGroupNodeMap::const_iterator pNode = m_mapNodes.begin(); pNode != m_mapNodes.end(); ++pNode)
  {
    if (pNode->second->isKindOf(OdConstrainedGeometry::desc()))
      aConstGeom[((OdConstrainedGeometry*)pNode->second.get())->geomDependencyId()] = (OdConstrainedGeometry*)(pNode->second.get());
  }
  OdArray<OdGeomConstraint*> aConstraints;
  OdArray<OdConstrainedGeometry*> aGeometries;
  for (OdDbObjectIdArray::const_iterator i = aDestDepToRemove.begin(); i != aDestDepToRemove.end(); ++i)
  {
    std::map<OdDbObjectId, OdConstrainedGeometry*>::iterator pConstrainedGeometry = aConstGeom.find(*i);
    if (pConstrainedGeometry != aConstGeom.end())
      aGeometries.append(pConstrainedGeometry->second);
  }
  return deleteNodes(aConstraints, aGeometries);
}

OdResult OdDbImpAssoc2dConstraintGroup::deleteConstrainedGeometry(const OdDbObjectId& geomDependencyId)
{
  OdDbAssocGeomDependencyPtr geomDependency = OdDbAssocGeomDependency::cast(geomDependencyId.openObject());
  OdConstrainedGeometry* pConstrainedGeometry = NULL;

  if (!geomDependency.isNull())
    getConstrainedGeometry(geomDependency, pConstrainedGeometry);

  geomDependency.release();

  if (pConstrainedGeometry == NULL)
    return eNullObjectPointer;

  OdArray<OdGeomConstraint*> aConstraints;
  OdArray<OdConstrainedGeometry*> aGeometries;
  aGeometries.append(pConstrainedGeometry);

  return deleteNodes(aConstraints, aGeometries);
}

OdResult OdDbImpAssoc2dConstraintGroup::getConstrainedGeometry(const OdDbFullSubentPath& fullSubentPath,
                                  OdConstrainedGeometry*&   pConsGeom,
                                  bool bCreateArcLineMid)
{
  OdDbObjectIdArray arrObjId = fullSubentPath.objectIds();

  OdDbObjectId obj = arrObjId[arrObjId.length() - 1];
  OdDbObjectPtr pObj = obj.openObject();

  OdDbObjectIdArray arrDepId;
  {
    OdDbObjectPtr pDependentOnObject = arrObjId.first().openObject();
    OdDbAssocDependency::getDependenciesOnObject(pDependentOnObject, true, true, arrDepId);
  }

  if ( arrDepId.length() == 0 )
    return eBadObjType;

  OdDbObjectPtr pDepObj;
  OdDbAssocGeomDependencyPtr pGeomDep;
  OdConstrainedGeometry* pConsGeomTmp;
  OdDbFullSubentPathArray arrFullSubentPath;
  OdConstrainedImplicitPoint::ImplicitPointType* pPtType = NULL;
  OdConstrainedImplicitPoint::ImplicitPointType ptType;

  const OdDbSubentId subentId = fullSubentPath.subentId();

  for ( unsigned int i = 0; i < arrDepId.length(); i++ )
  {
    pDepObj = arrDepId[i].openObject();
    if ( !pDepObj->isKindOf(OdDbAssocGeomDependency::desc()) )
      continue;

    pGeomDep = OdDbAssocGeomDependency::cast(pDepObj);
    if ( getConstrainedGeometry(pGeomDep, pConsGeomTmp) != eOk )
      continue;

    arrFullSubentPath.clear();
    if ( pConsGeomTmp->getFullSubentPaths(arrFullSubentPath) != eOk )
      continue;

    if ( fullSubentPath == arrFullSubentPath[0] )
    {
      pConsGeom = pConsGeomTmp;
      return eOk;
    }
    if ( fullSubentPath.objectIds().size() != arrFullSubentPath[0].objectIds().size() )
      continue;

    if ( subentId.type() == OdDb::kVertexSubentType )//find point
    {
      if ( subentId.index() < 0 && !pObj->isKindOf(OdDbSpline::desc()) ) // simple circle, line, ellipse, arc
      {
        if ( subentId.index() == OdDbAssocCurvePersSubentIdPE::kMiddle )
        {
          ptType = OdConstrainedImplicitPoint::kMidImplicit;
        }
        else if ( subentId.index() == OdDbAssocCurvePersSubentIdPE::kCenter )
        {
          ptType = OdConstrainedImplicitPoint::kCenterImplicit;
        }
        else if ( subentId.index() == OdDbAssocCurvePersSubentIdPE::kEnd )
        {
          ptType = OdConstrainedImplicitPoint::kEndImplicit;
        }
        else if ( subentId.index() == OdDbAssocCurvePersSubentIdPE::kStart )
        {
          ptType = OdConstrainedImplicitPoint::kStartImplicit;
        }
        else
          ODA_FAIL();

        pPtType = &ptType;

        if ( getConstrainedGeometry(pGeomDep, pConsGeom, pPtType, -1, bCreateArcLineMid) == eOk )
          return eOk;
      }
      else//polyline points or define points
      {
        if ( pObj->isKindOf(OdDbSpline::desc()) )
        {
          int idx = (int)subentId.index();
          switch (idx) {
          case OdDbAssocCurvePersSubentIdPE::kStart:
            idx = 0;
            break;
          case OdDbAssocCurvePersSubentIdPE::kEnd:
            idx = OdDbSpline::cast(pObj)->numControlPoints() - 1;
            break;
          }
          ptType = OdConstrainedImplicitPoint::kDefineImplicit;
          pPtType = &ptType;
          if ( getConstrainedGeometry(pGeomDep,
                                pConsGeom,
                                pPtType,
                                idx,
                                bCreateArcLineMid) == eOk )
            return eOk;
        }
        else //polyline
        {
          if ( subentId.index() < 1000000 )//start, end
          {
            ptType = OdConstrainedImplicitPoint::kStartImplicit;
            pPtType = &ptType;
            if ( getConstrainedGeometry(pGeomDep, pConsGeom, pPtType, -1, bCreateArcLineMid) == eOk )
            {
              if ( pConsGeom->getFullSubentPaths(arrFullSubentPath) == eOk )
                if ( fullSubentPath == arrFullSubentPath[0] )
                  return eOk;

            }

           ptType = OdConstrainedImplicitPoint::kEndImplicit;
           pPtType = &ptType;
           if ( getConstrainedGeometry(pGeomDep, pConsGeom, pPtType, -1, bCreateArcLineMid) == eOk )
           {
             if ( pConsGeom->getFullSubentPaths(arrFullSubentPath) == eOk )
                if ( fullSubentPath == arrFullSubentPath[0] )
                  return eOk;
           }
           continue;
         }
          
          if ( subentId.index() > OdDbAssocCurvePersSubentIdPE::kMiddlePLinePrefix ) //middle point
          {
            ptType = OdConstrainedImplicitPoint::kMidImplicit;
            pPtType = &ptType;
          }
          else if ( subentId.index() > OdDbAssocCurvePersSubentIdPE::kCenterPLinePrefix )//center
          {
            ptType = OdConstrainedImplicitPoint::kCenterImplicit;
            pPtType = &ptType;
          }
          
          if ( getConstrainedGeometry(pGeomDep, pConsGeom, pPtType, -1, bCreateArcLineMid) == eOk )
            return eOk;
        }
      }
    }
  }
  
  pConsGeom = NULL;
  return eBadObjType;
}

OdResult OdDbImpAssoc2dConstraintGroup::getConstrainedGeometry(const OdDbAssocGeomDependency* pGeomDependency,
                                  OdConstrainedGeometry*&   pConsGeom,
                                  OdConstrainedImplicitPoint::ImplicitPointType* pPtType,
                                  int defPtIndex,
                                  bool bCreateArcLineMid)
{
  const OdDbObjectId searchId = pGeomDependency->objectId();
  OdResult res = eBadObjType;
  if (searchId.isNull())
  {
    pConsGeom = NULL;
    return res;
  }
  for (OdConstraintGroupNodeMap::const_iterator i = m_mapNodes.begin(); i != m_mapNodes.end(); ++i)
  {
    if (i->second->isKindOf(OdConstrainedGeometry::desc()))
    {
      if (((OdConstrainedGeometry*)(i->second.get()))->geomDependencyId() == searchId)
      {
        pConsGeom = (OdConstrainedGeometry*)(i->second.get());
        res = eOk;
        break;
      }
    }
  }

  if ( res == eOk )
  {
    if ( pPtType != NULL && pConsGeom->isKindOf(OdConstrainedCurve::desc()) )
    {
      res = eBadObjType;
      OdConstrainedCurve* pCurve = OdConstrainedCurve::cast(pConsGeom);
      OdArray<OdConstrainedImplicitPoint*> aPoints;
      pCurve->getConstrainedImplicitPoints(aPoints);
      for ( unsigned int i = 0; i < aPoints.length(); i++ )
      {
        if ( aPoints[i]->pointType() == *pPtType )
        {
          if ( defPtIndex != -1 )
          {
            if ( aPoints[i]->pointIndex() == defPtIndex )
            {
              pConsGeom = aPoints[i];
              res = eOk;
              break;
            }
          }
          else
          {
            pConsGeom = aPoints[i];
            res = eOk;
            break;
          }
        }
      }
      
      if ( res != eOk && *pPtType == OdConstrainedImplicitPoint::kMidImplicit && bCreateArcLineMid )
      {
        pConsGeom = addImplicitPoint(pCurve, OdConstrainedImplicitPoint::kMidImplicit);
        res = eOk;
      }
    }
    else if ( pPtType != NULL )
    {
      pConsGeom = NULL;
      res = eBadObjType;
    }
  }
  else
    pConsGeom = NULL;

  return res;
}

OdResult OdDbImpAssoc2dConstraintGroup::getConstrainedGeometries(OdArray<OdConstrainedGeometry*>& aConstGeom) const
{
  for (OdConstraintGroupNodeMap::const_iterator pNode = m_mapNodes.begin(); pNode != m_mapNodes.end(); ++pNode)
  {
    if (pNode->second.get()->isKindOf(OdConstrainedGeometry::desc()) )
      aConstGeom.append((OdConstrainedGeometry*)(pNode->second.get()));
  }
  return eOk;
}

OdResult OdDbImpAssoc2dConstraintGroup::addGeometricalConstraint(OdDbObjectId actionId, 
                                                                 OdGeomConstraint::GeomConstraintType type, 
                                                                 const OdDbFullSubentPathArray& aPaths,
                                                                 OdGeomConstraint** ppNewConstraint,
                                                                 const OdGeMatrix3d* pUCSCoord)
{
  OdArray<OdConstrainedGeometry*> apConsGeoms;
  OdArray<OdConstrainedGeometry*> apAllConsGeoms;
  if ( getConstrainedGeometries(apAllConsGeoms) != eOk )
    return eBadObjType;

  OdDbFullSubentPathArray arrGeomPath;
  unsigned int i, j;
  for ( j = 0; j < aPaths.length(); j++ ) {
    for ( i = 0; i < apAllConsGeoms.length(); i++ ) {
      arrGeomPath.clear();
      if ( apAllConsGeoms[i]->getFullSubentPaths(arrGeomPath) != eOk || arrGeomPath.length() == 0 )
        continue;
      if ( aPaths[j] == arrGeomPath[0] )
      {
        apConsGeoms.append(apAllConsGeoms[i]);
        break;
      }
    }
  }

  return addGeometricalConstraint(actionId, type, apConsGeoms, ppNewConstraint, pUCSCoord);
}

OdResult OdDbImpAssoc2dConstraintGroup::addGeometricalConstraint(OdDbObjectId actionId,
                                                                 OdGeomConstraint::GeomConstraintType type,
                                                                 const OdArray<OdConstrainedGeometry*>& apConsGeoms,
                                                                 OdGeomConstraint** ppNewConstraint,
                                                                 const OdGeMatrix3d* pUCSCoord)
{
  *ppNewConstraint = NULL;

  switch ( type )
  {
    case OdGeomConstraint::kHorizontal:
    {
      OdGeVector3d xAxis = OdGeVector3d::kXAxis;
      if (pUCSCoord)
      {
        OdGeVector3d localXAxis = pUCSCoord->getCsXAxis();
        localXAxis.transformBy(OdGeMatrix3d::worldToPlane(m_plane));
        localXAxis.normalize();
        if (!localXAxis.isParallelTo(xAxis))
          xAxis = localXAxis;
      }
      if (apConsGeoms.length() == 1)
        *ppNewConstraint = addHorizontalConstraint(apConsGeoms[0], actionId, xAxis);
      else if (apConsGeoms.length() == 2)
        *ppNewConstraint = addHorizontalConstraint(apConsGeoms[0], apConsGeoms[1], actionId, xAxis);
      else
        *ppNewConstraint = NULL;
    }
    break;
    case OdGeomConstraint::kVertical:
    {
      OdGeVector3d yAxis = OdGeVector3d::kYAxis;
      if (pUCSCoord)
      {
        OdGeVector3d localYAxis = pUCSCoord->getCsYAxis();
        localYAxis.transformBy(OdGeMatrix3d::worldToPlane(m_plane));
        localYAxis.normalize();
        if (!localYAxis.isParallelTo(yAxis))
          yAxis = localYAxis;
      }
      if (apConsGeoms.length() == 1)
        *ppNewConstraint = addVerticalConstraint(apConsGeoms[0], actionId, yAxis);
      else if (apConsGeoms.length() == 2)
        *ppNewConstraint = addVerticalConstraint(apConsGeoms[0], apConsGeoms[1], actionId, yAxis);
      else
        *ppNewConstraint = NULL;
    }
    break;
    case OdGeomConstraint::kParallel:
      if ( apConsGeoms.length() == 2 )
        *ppNewConstraint = addParallelConstraint(apConsGeoms[0], apConsGeoms[1], actionId);
      else
        *ppNewConstraint = NULL;
    break;
    case OdGeomConstraint::kPerpendicular:
      if ( apConsGeoms.length() == 2 )
        *ppNewConstraint = addPerpendicularConstraint(apConsGeoms[0], apConsGeoms[1], actionId);
      else
        *ppNewConstraint = NULL;
    break;
    case OdGeomConstraint::kNormal:
        if ( apConsGeoms.length() == 2 )
        *ppNewConstraint = addNormalConstraint(apConsGeoms[0], apConsGeoms[1], actionId);
      else
        *ppNewConstraint = NULL;
    break;
    case OdGeomConstraint::kColinear:
      if ( apConsGeoms.length() == 2 )
        *ppNewConstraint = addColinearConstraint(apConsGeoms[0], apConsGeoms[1], actionId);
      else
        *ppNewConstraint = NULL;
    break;
    case OdGeomConstraint::kCoincident:
      if ( apConsGeoms.length() == 2 )
        *ppNewConstraint = addCoincidentConstraint(apConsGeoms[0], apConsGeoms[1], actionId);
      else
        *ppNewConstraint = NULL;
    break;
    case OdGeomConstraint::kConcentric:
      if ( apConsGeoms.length() == 2 )
        *ppNewConstraint = addConcentricConstraint(apConsGeoms[0], apConsGeoms[1], actionId);
      else
        *ppNewConstraint = NULL;
    break;
    case OdGeomConstraint::kTangent:
      if ( apConsGeoms.length() == 2 )
        *ppNewConstraint = addTangentConstraint(apConsGeoms[0], apConsGeoms[1], NULL, NULL, actionId);
      else
        *ppNewConstraint = NULL;
    break;
    case OdGeomConstraint::kEqualRadius:
      if ( apConsGeoms.length() == 2 )
        *ppNewConstraint = addEqualRadiusConstraint(apConsGeoms[0], apConsGeoms[1], actionId);
      else
        *ppNewConstraint = NULL;
    break;
    case OdGeomConstraint::kEqualLength:
      if ( apConsGeoms.length() == 2 )
        *ppNewConstraint = addEqualLengthConstraint(apConsGeoms[0], apConsGeoms[1], actionId);
      else
        *ppNewConstraint = NULL;
    break;
    case OdGeomConstraint::kSymmetric:
      if ( apConsGeoms.length() == 3 )
        *ppNewConstraint = addSymmetricConstraint(apConsGeoms[0], apConsGeoms[1], apConsGeoms[2], actionId);
      else
        *ppNewConstraint = NULL;
    break;
    case OdGeomConstraint::kG2Smooth:
      if ( apConsGeoms.length() == 2 )
        *ppNewConstraint = addG2SmoothConstraint(apConsGeoms[0], apConsGeoms[1], actionId);
      else
        *ppNewConstraint = NULL;
    break;
    case OdGeomConstraint::kFix:
      if ( apConsGeoms.length() == 1 )
        *ppNewConstraint = addFixConstraint(apConsGeoms[0], actionId);
      else
        *ppNewConstraint = NULL;
    break;
  }

  if ( *ppNewConstraint != NULL ) {
    OdGeomConstraintImpl *pGeomCnstrImpl = (OdGeomConstraintImpl*)getConstraintGroupNodeImpl(*ppNewConstraint);
    pGeomCnstrImpl->setImplied(false);
    return eOk;
  }
  else
    return eBadObjType;
}

bool OdDbImpAssoc2dConstraintGroup::isEmpty() const
{
  return m_mapNodes.empty();
}

OdResult OdDbImpAssoc2dConstraintGroup::addDistanceConstraint(OdDbAssoc2dConstraintGroup *pGroup,
                                                              OdConstrainedGeometry* pConsGeom1,
                                                              OdConstrainedGeometry* pConsGeom2,
                                                              OdDistanceConstraint::DirectionType directionType,
                                                              OdDbObjectId valueDependencyId,
                                                              OdDbObjectId dimDependencyId, // May be null
                                                              const OdGeVector3d* pFixedDirection/* = NULL*/,
                                                              const OdConstrainedLine* pDirectionLine/* = NULL*/,
                                                              OdDistanceConstraint** ppNewDisConstraint/* = NULL*/)
{
  OdDistanceConstraint* res = NULL;

  OdArray<OdGeomConstraint*> aConstr;
  pConsGeom1->getCommonConstraints(pConsGeom2, aConstr);
  OdGeomConstraint* tmp;
  if (isConstraintOfThisTypeExist(aConstr, OdDistanceConstraint::desc(), &tmp))
    return eBadObjType;

  OdSmartPtr<OdDistanceConstraint> spDistCnstr = ::odrxCreateObject(OdDistanceConstraint::desc()->name());
  tieNode(spDistCnstr, pGroup->objectId());

  res = spDistCnstr.get();

  tieDimAndCnstr(res, valueDependencyId, dimDependencyId, pGroup);
  tieGeomsAndCnstr3(res, pConsGeom1, pConsGeom2);

  if (pFixedDirection)
    res->setDirection(*pFixedDirection);
  else if (pDirectionLine)
    res->setDirection(pDirectionLine->nodeId(), directionType);

  *ppNewDisConstraint = res;
  return eOk;
}

OdResult OdDbImpAssoc2dConstraintGroup::addAngleConstraint(OdDbAssoc2dConstraintGroup *pGroup,
                                                           OdConstrainedLine* pConsLine1,
                                                           OdConstrainedLine* pConsLine2,
                                                           OdAngleConstraint::SectorType sectorType,
                                                           OdDbObjectId valueDependencyId,
                                                           OdDbObjectId dimDependencyId, // May be null
                                                           OdAngleConstraint** ppNewAngConstraint/* = NULL*/)
{
  OdAngleConstraint* res = NULL;

  OdArray<OdGeomConstraint*> aConstr;
  pConsLine1->getCommonConstraints(pConsLine2, aConstr);
  OdGeomConstraint* tmp;
  if (isConstraintOfThisTypeExist(aConstr, OdAngleConstraint::desc(), &tmp))
    return eBadObjType;

  OdSmartPtr<OdAngleConstraint> spAngCnstr = ::odrxCreateObject(OdAngleConstraint::desc()->name());
  tieNode(spAngCnstr, pGroup->objectId());

  res = spAngCnstr.get();

  tieDimAndCnstr(res, valueDependencyId, dimDependencyId, pGroup);
  tieGeomsAndCnstr3(res, pConsLine1, pConsLine2);

  res->setSectorType(sectorType);

  *ppNewAngConstraint = res;
  return eOk;
}

OdResult OdDbImpAssoc2dConstraintGroup::add3PointAngleConstraint(OdDbAssoc2dConstraintGroup *pGroup,
                                                                 OdConstrainedPoint* pConsPoint1,
                                                                 OdConstrainedPoint* pConsPoint2,
                                                                 OdConstrainedPoint* pConsPoint3,
                                                                 OdAngleConstraint::SectorType sectorType,
                                                                 OdDbObjectId valueDependencyId,
                                                                 OdDbObjectId dimDependencyId, // May be null
                                                                 Od3PointAngleConstraint** ppNewAngConstraint/* = NULL*/)
{
  Od3PointAngleConstraint* res = NULL;

  OdSmartPtr<Od3PointAngleConstraint> sp3PtAngCnstr = ::odrxCreateObject(Od3PointAngleConstraint::desc()->name());
  tieNode(sp3PtAngCnstr, pGroup->objectId());

  res = sp3PtAngCnstr.get();

  tieDimAndCnstr(res, valueDependencyId, dimDependencyId, pGroup);
  tieGeomsAndCnstr3(res, pConsPoint1, pConsPoint2, pConsPoint3);

  res->setSectorType(sectorType);

  *ppNewAngConstraint = res;
  return eOk;
}

OdResult OdDbImpAssoc2dConstraintGroup::addRadiusDiameterConstraint(OdDbAssoc2dConstraintGroup *pGroup,
                                                                    OdConstrainedGeometry* pConsGeom,
                                                                    OdRadiusDiameterConstraint::RadiusDiameterConstrType type,
                                                                    OdDbObjectId valueDependencyId,
                                                                    OdDbObjectId dimDependencyId, // May be null
                                                                    OdRadiusDiameterConstraint** ppNewRadDiaConstraint/* = NULL*/)
{
  OdRadiusDiameterConstraint* res = NULL;

  OdArray<OdGeomConstraint*> aConstr;
  pConsGeom->getConnectedConstraints(aConstr);
  OdGeomConstraint* tmp;
  if (isConstraintOfThisTypeExist(aConstr, OdRadiusDiameterConstraint::desc(), &tmp))
    return eBadObjType;

  OdSmartPtr<OdRadiusDiameterConstraint> spRadDiaCnstr = ::odrxCreateObject(OdRadiusDiameterConstraint::desc()->name());
  tieNode(spRadDiaCnstr, pGroup->objectId());

  res = spRadDiaCnstr.get();

  tieDimAndCnstr(res, valueDependencyId, dimDependencyId, pGroup);
  tieGeomsAndCnstr3(res, pConsGeom, NULL);

  res->setConstrType(type);

  *ppNewRadDiaConstraint = res;
  return eOk;
}

OdResult OdDbImpAssoc2dConstraintGroup::deleteNodes(OdArray<OdGeomConstraint*> &iaConstraints,
                                                    OdArray<OdConstrainedGeometry*> &iaGeometries)
{
  OdResult res = eOk;
  unsigned int i, j;
  OdConstrainedGeometry* pConstrainedGeometry = 0;

  if (iaConstraints.size() > 0) {
    if ((res = deleteConstraints(iaConstraints, iaGeometries)) != eOk)
      return res;
  } else if (iaGeometries.size() == 1) {
    pConstrainedGeometry = iaGeometries[0];
    pConstrainedGeometry->getConnectedConstraints(iaConstraints);
    for (i = 0; i < iaConstraints.size(); i++) {
      OdGeomConstraintPtr geomConstraint = iaConstraints[i];
      if (!geomConstraint->isImplied())
        continue;
      OdArray<OdConstrainedGeometry*> aConnectedGeometries;
      ((OdGeomConstraintImpl*)geomConstraint->m_pImpl)->getConnectedGeometries(aConnectedGeometries);
      for (j = 0; j < aConnectedGeometries.size(); j++) {
        OdConstrainedGeometry* connectedGeometry = aConnectedGeometries[j];
        bool isImplicit = connectedGeometry->isKindOf(OdConstrainedImplicitPoint::desc());
        if (isImplicit && !iaGeometries.contains(connectedGeometry))
          iaGeometries.append(connectedGeometry);
      }
    }
  }

  do {
    for (i = 0; i < iaGeometries.size(); i++) {
      pConstrainedGeometry = iaGeometries[i];
      iaConstraints.clear();
      pConstrainedGeometry->getConnectedConstraints(iaConstraints);
      if ((res = deleteConstraints(iaConstraints, iaGeometries)) != eOk)
        return res;
    }
    if ((res = deleteGeometries(iaGeometries)) != eOk)
      return res;
    if (!iaGeometries.isEmpty())
      continue;

    std::set<OdConstraintGroupNodeId> aIsExternal;
    for (OdConstraintGroupNodeMap::const_iterator pNode = m_mapNodes.begin(); pNode != m_mapNodes.end(); ++pNode){
      OdGeomConstraint* geomConstraint = OdGeomConstraint::cast(pNode->second);
      if (geomConstraint && (!geomConstraint->isInternal() || geomConstraint->getOwningCompositeConstraint())) {
        OdArray<OdConstrainedGeometry*> aConnectedGeometries;
        ((OdGeomConstraintImpl*)geomConstraint->m_pImpl)->getConnectedGeometries(aConnectedGeometries);
        for (j = 0; j < aConnectedGeometries.size(); j++) {
            aIsExternal.insert(aConnectedGeometries[j]->nodeId());
        }
      }
    }
    for (OdConstraintGroupNodeMap::const_iterator pNode = m_mapNodes.begin(); pNode != m_mapNodes.end(); ++pNode){
      OdConstrainedImplicitPoint* pPoint = OdConstrainedImplicitPoint::cast(pNode->second);
      if (pPoint && aIsExternal.find(pPoint->nodeId()) != aIsExternal.end())
        aIsExternal.insert(pPoint->constrainedCurveId());
    }
    for (OdConstraintGroupNodeMap::const_iterator pNode = m_mapNodes.begin(); pNode != m_mapNodes.end(); ++pNode){
      OdConstrainedGeometry* pConstrainedGeometry = OdConstrainedGeometry::cast(pNode->second);
      if (!pConstrainedGeometry || pConstrainedGeometry->geomDependencyId().isNull() || aIsExternal.find(pConstrainedGeometry->nodeId()) != aIsExternal.end())
        continue;
      OdConstrainedImplicitPoint* point = OdConstrainedImplicitPoint::cast(pConstrainedGeometry);
      if (point && aIsExternal.find(point->constrainedCurveId()) != aIsExternal.end())
        continue;
      OdArray<OdConstrainedGeometry*> aConnectedGeometries;
      ((OdConstrainedGeometryImpl*)pConstrainedGeometry->m_pImpl)->getConnectedGeometries(aConnectedGeometries);
      bool hasNoExternal = true;
      for (j = 0; j < aConnectedGeometries.size() && hasNoExternal; j++) {
        hasNoExternal = aIsExternal.find(aConnectedGeometries[j]->nodeId()) == aIsExternal.end();
      }
      if (hasNoExternal) {
        iaGeometries.append(pConstrainedGeometry);
        break;
      }
    }
  } while (!iaGeometries.isEmpty());
  return eOk;
}

OdResult OdDbImpAssoc2dConstraintGroup::deleteGeometries(OdArray<OdConstrainedGeometry*> &ioaGeometries)
{
  int i = ioaGeometries.size();
  while (i-- > 0) {
    OdConstrainedGeometry *pConstrainedGeometry = ioaGeometries[i];
    OdDbObjectId depId = pConstrainedGeometry->geomDependencyId();
    if (!depId.isNull())
      removeDependency(depId, true);

    if (OdConstrainedRigidSet* pRigidSet = pConstrainedGeometry->getOwningRigidSet()) {
      OdConstrainedRigidSetImpl* pRigidSetImpl = static_cast<OdConstrainedRigidSetImpl*>(pRigidSet->m_pImpl);
      ODA_VERIFY(pRigidSetImpl->removeConstrainedGeom(pConstrainedGeometry->nodeId()) == eOk);
    }

    removeNode(pConstrainedGeometry);
  }
  ioaGeometries.clear();

  for (OdConstraintGroupNodeMap::reverse_iterator ri = m_mapNodes.rbegin(); ri != m_mapNodes.rend(); ++ri) {
    OdConstraintGroupNodePtr node = ri->second;
    if (!node->isKindOf(OdConstrainedGeometry::desc()))
      continue;
    OdConstrainedGeometry *pConstrainedGeometry = OdConstrainedGeometry::cast(node);
    if (node->isKindOf(OdConstrainedImplicitPoint::desc())) {
      OdSmartPtr<OdConstrainedImplicitPoint> point = OdConstrainedImplicitPoint::cast(node);
      bool isOrhpan = getGroupNodePtr(point->constrainedCurveId()) == NULL;
      if (isOrhpan && !ioaGeometries.contains(pConstrainedGeometry))
        ioaGeometries.append(pConstrainedGeometry);
    } else if (node->isKindOf(OdConstrainedDatumLine::desc())) {
      bool found = false;
      for (OdConstraintGroupNodeMap::reverse_iterator rj = m_mapNodes.rbegin(); rj != m_mapNodes.rend() && !found; ++rj) {
        OdConstraintGroupNodePtr constr = rj->second;
        if (constr->isKindOf(OdParallelConstraint::desc())) {
          OdSmartPtr<OdParallelConstraint> parallelConstraint = OdParallelConstraint::cast(constr);
          OdConstraintGroupNodeImpl *pImpl = OdDbImpAssoc2dConstraintGroup::getConstraintGroupNodeImpl(parallelConstraint);
          found = (((OdParallelConstraintImpl*)pImpl)->getDatumLineIdx() == node->nodeId());
        }
      }
      if (!found && !ioaGeometries.contains(pConstrainedGeometry))
        ioaGeometries.append(pConstrainedGeometry);
    } else if (OdConstrainedRigidSet* pRigidSet = OdConstrainedRigidSet::cast(node).get()) {
      if (pRigidSet->numOfConstrainedGeoms() == 0)
        ioaGeometries.append(pRigidSet);
    }

  }
  return eOk;
}

OdResult OdDbImpAssoc2dConstraintGroup::deleteHelpParameters(OdArray<OdHelpParameter*> &ioaHelpParameters,
                                                             OdConstraintGroupNodeId id)
{
  int i = ioaHelpParameters.size();
  while (i-- > 0) {
    OdHelpParameter* helpParameter = ioaHelpParameters[i];
    bool isNoConnection = false;
    OdConstraintGroupNodeImpl *pImpl = OdDbImpAssoc2dConstraintGroup::getConstraintGroupNodeImpl(helpParameter);
    pImpl->removeConnection(id, isNoConnection);
    if (isNoConnection) {
      removeNode(helpParameter);
      ioaHelpParameters.remove(helpParameter);
    }
  }
  return eOk;
}

OdResult OdDbImpAssoc2dConstraintGroup::deleteConstraints(OdArray<OdGeomConstraint*> &iaConstraints,
                                                          OdArray<OdConstrainedGeometry*> &ioaGeometries)
{
  std::set<void*> deletedPointers;
  OdResult res = eOk;
  for (unsigned i = 0; i < iaConstraints.size(); i++) {
    if (deletedPointers.count(iaConstraints[i]))
      continue;

    OdGeomConstraintPtr geomConstraint = iaConstraints[i];

    OdCompositeConstraint *pComposite = geomConstraint->getOwningCompositeConstraint();
    if (pComposite != NULL) {
      if (iaConstraints.contains(pComposite)) {
        iaConstraints[i] = NULL;
        continue;
      } else {
        iaConstraints[i] = pComposite;
        geomConstraint = pComposite;
      }
    }

    if (!deletedPointers.insert(geomConstraint).second)
      continue;

    if (geomConstraint->isKindOf(OdExplicitConstraint::desc())) {
      OdExplicitConstraint* explicitConstraint = static_cast<OdExplicitConstraint*>(geomConstraint.get());
      OdDbObjectId dimDepId = explicitConstraint->dimDependencyId();
      if (!dimDepId.isNull()) {
        if (OdDbAssocDimDependencyBodyBase::getEraseDimensionIfDependencyIsErased()) {
          OdDbAssocDependencyPtr dimDep = dimDepId.openObject();
          if (!dimDep.isNull()) {
            OdDbObjectId dimId = dimDep->dependentOnObject();
            OdDbObjectPtr dimension = dimId.openObject(OdDb::kForWrite);
            if (!dimension.isNull()) {
              dimension->removePersistentReactor(dimDepId);
              dimension->erase(true);
            }
          }
        }
        removeDependency(dimDepId, true);
      }
      OdDbObjectId valDepId = explicitConstraint->valueDependencyId();
      OdDbAssocValueDependencyPtr valDep = valDepId.openObject();
      if (!valDep.isNull()) {
        OdDbObjectId varId = valDep->dependentOnObject();
        valDep = 0;
        removeDependency(valDepId, true);
        OdDbAssocNetworkPtr pNetwork = OdDbAssocNetwork::cast(owningNetwork().openObject(OdDb::kForWrite));
        if (!pNetwork.isNull())
          pNetwork->removeAction(varId, true);
      }
    }
    else if (geomConstraint->isKindOf(OdCompositeConstraint::desc())) {
      OdCompositeConstraint* compositeConstraint = static_cast<OdCompositeConstraint*>(geomConstraint.get());
      OdArray<OdGeomConstraint*> aConstr;
      compositeConstraint->getOwnedConstraints(aConstr);
      for (unsigned j = 0; j < aConstr.size(); j++) {
        OdGeomConstraintImpl* pImpl = (OdGeomConstraintImpl*)OdDbImpAssoc2dConstraintGroup::getConstraintGroupNodeImpl(aConstr[j]);
        pImpl->setOwningCompositeConstraintId(OdConstraintGroupNode::kNullGroupNodeId);
        if (!iaConstraints.contains(aConstr[j]))
          iaConstraints.append(aConstr[j]);
      }
    }

    OdArray<OdHelpParameter*> aHelpParameters;
    if ((res = geomConstraint->getConnectedHelpParameters(aHelpParameters)) != eOk)
    {
      ODA_ASSERT(false && "Failed to get contraint's help parameters");
    }

    OdArray<OdConstrainedGeometry*> aConnectedGeometries;
    if ((res = ((OdGeomConstraintImpl*)geomConstraint->m_pImpl)->getConnectedGeometries(aConnectedGeometries)) != eOk)
    {
      ODA_ASSERT(false && "Failed to get constraint's connected geometry");
    }

    for (unsigned j = 0; j < aConnectedGeometries.size(); j++) {
      OdConstrainedGeometry* connectedGeometry = aConnectedGeometries[j];
      OdConstrainedImplicitPoint* implicitPoint = OdConstrainedImplicitPoint::cast(connectedGeometry);
      bool isNoConnection = false,
           isImplicit = geomConstraint->isImplied() &&
                        connectedGeometry->isKindOf(OdConstrainedConstructionLine::desc()),
           isDefinePt = (implicitPoint && implicitPoint->constrainedCurveId() != OdConstraintGroupNode::kNullGroupNodeId);

      OdConstraintGroupNodeImpl *pImpl = OdDbImpAssoc2dConstraintGroup::getConstraintGroupNodeImpl(connectedGeometry);
      pImpl->removeConnection(geomConstraint->nodeId(), isNoConnection);

      for (unsigned k = 0; k < aHelpParameters.size(); k++) {
        pImpl->removeConnection(aHelpParameters[k]->nodeId(), isNoConnection);
      }

      if ((res = deleteHelpParameters(aHelpParameters, connectedGeometry->nodeId())) != eOk)
      {
        ODA_ASSERT(false && "Failed to delete constrained geometry's help parameters");
        continue;
      }

      if ((isNoConnection || isImplicit) && !isDefinePt && !ioaGeometries.contains(connectedGeometry))
        ioaGeometries.append(connectedGeometry);
    }

    if ((res = deleteHelpParameters(aHelpParameters, geomConstraint->nodeId())) != eOk)
    {
      ODA_ASSERT(false && "Failed to delete constraint's help parameters");
    }

    removeNode(geomConstraint);
  }
  return res;
}

OdResult OdDbImpAssoc2dConstraintGroup::deleteConstraint(OdGeomConstraint* pGeomConst)
{
  OdResult res = eInvalidInput;

  if (!pGeomConst || pGeomConst->isInternal())
    return res;

  OdArray<OdGeomConstraint*> aConstraints;
  OdArray<OdConstrainedGeometry*> aGeometries;

  aConstraints.append(pGeomConst);

  return deleteNodes(aConstraints, aGeometries);
}

OdResult OdDbImpAssoc2dConstraintGroup::getConstraints(OdArray<OdGeomConstraint*>& apConstraints) const
{
  for (OdConstraintGroupNodeMap::const_iterator i = m_mapNodes.begin(); i != m_mapNodes.end() ; ++i) {
    if (i->second->isKindOf(OdGeomConstraint::desc()) )
      apConstraints.append(const_cast<OdGeomConstraint*>(static_cast<const OdGeomConstraint*>(i->second.get())));
  }
  return eOk;
}

#if 0
OdResult OdDbImpAssoc2dConstraintGroup::autoConstrain(const OdDbFullSubentPathArray& aPaths,
                         const OdGeTol& tol,
                         OdAutoConstrainEvaluationCallback* pCallback)
#endif
OdResult OdDbImpAssoc2dConstraintGroup::getAllConnectedGeomDependencies(const OdDbObjectIdArray& srcGeomDependencyIds,
                                           OdDbObjectIdArray& allConnectedGeomDependencyIds) const
{

  return eOk;
}

OdConstraintGroupNode* OdDbImpAssoc2dConstraintGroup::getGroupNodePtr(const OdConstraintGroupNodeId nodeId) const
{
  OdConstraintGroupNodeMap::const_iterator it = m_mapNodes.find(nodeId);
  if (it != m_mapNodes.end())
    return const_cast<OdConstraintGroupNode*>(it->second.get());
  return 0;
}

#if 0
void OdDbImpAssoc2dConstraintGroup::regenDimensionSystem()

OdResult OdDbImpAssoc2dConstraintGroup::geometryMirrored(OdDbAssocGeomDependency* pGeomDependency)
  
void OdDbImpAssoc2dConstraintGroup::addGlobalCallback(class OdDbAssoc2dConstraintCallback* pCallback)

void OdDbImpAssoc2dConstraintGroup::removeGlobalCallback(class OdDbAssoc2dConstraintCallback* pCallback)

class OdDbAssoc2dConstraintCallback* OdDbImpAssoc2dConstraintGroup::globalCallback()

#endif

static bool isDependencyAlive(const OdDbAssocDependency* pDep)
{
  if (!pDep || pDep->status() == kErasedAssocStatus)
    return false;

  OdDbCompoundObjectId compId;
  pDep->getDependentOnCompoundObject(compId);
  if (compId.isEmpty())
    return false;

  OdDbObjectIdArray objIds;
  compId.getFullPath(objIds);
  for (unsigned objIdIdx = 0; objIdIdx < objIds.size(); ++objIdIdx)
  {
    if (objIds[objIdIdx].isEffectivelyErased() || objIds[objIdIdx].openObject().isNull())
      return false;
  }

  if (const OdDbAssocGeomDependency* pGeomDep = OdDbAssocGeomDependency::cast(pDep).get())
  {
    OdArray<OdDbSubentId> aSubentIds;
    if (pGeomDep->getTransientSubentIds(aSubentIds) != eOk)
      return false;
  }

  return true;
}

void OdDbImpAssoc2dConstraintGroup::evaluate(OdDbAssocAction *pAction, OdDbAssocEvaluationCallback* pEvaluationCallback)
{
  OdDbObjectIdArray dependencies(m_arrDependencies);
  OdUInt32 n = dependencies.size();
  const OdDbObjectId *pId = dependencies.asArrayPtr();
  bool worldToPlaneReady = false;
  OdGeMatrix3d worldToPlane;
  while (n--) {
    OdDbAssocDependencyPtr pDep = pId->openObject(OdDb::kForWrite);
    if (pDep.get()) {
      if (!isDependencyAlive(pDep)) { // do cleanup erased dependencies
        if (pDep->isKindOf(OdDbAssocGeomDependency::desc())) {
          deleteConstrainedGeometry(*pId);
        }
        else {
          OdArray<OdGeomConstraint*> apConstraints;
          getConstraints(apConstraints);
          for (unsigned constrIdx = 0; constrIdx < apConstraints.size(); ++constrIdx) {
            OdGeomConstraint* pConstr = apConstraints[constrIdx];
            if (pConstr && pConstr->isA()->isDerivedFrom(OdExplicitConstraint::desc())) {
              OdExplicitConstraint* pDim = static_cast<OdExplicitConstraint*>(pConstr);
              if (pDim->dimDependencyId() == *pId || pDim->valueDependencyId() == *pId)
                deleteConstraint(pConstr);
            }
          }
        }
      }
      else if(isEvaluationRequest(pDep->status())) { // do update geometry from modified dependencies
        if (pDep->isA()->isDerivedFrom(OdDbAssocGeomDependency::desc())) {
          OdDbAssocGeomDependency* geomDep = static_cast<OdDbAssocGeomDependency*>(pDep.get());
          OdConstrainedGeometry *consGeom;
          getConstrainedGeometry(geomDep, consGeom);
          if (consGeom) {
            if(!worldToPlaneReady) {
              worldToPlane.setToWorldToPlane(getWorkPlane());
              worldToPlaneReady = true;
            }
            consGeom->setDataFromGeomDependency(pDep->objectId(), worldToPlane);
          }
        }
      }
    }
    ++pId;
  }
  if (m_arrDependencies.isEmpty()) {
    setStatus(pAction, kErasedAssocStatus);
    return;
  }

  if(status()==kErasedAssocStatus)
    return;
  
  ODA_ASSERT(isEvaluationRequest(status()));

  OdSmartPtr<AssocActionPE> pPE = pAction->isA()->getX(AssocActionPE::desc());
  if ( !pPE.isNull() )
  {
    pPE->evaluate(pAction);
    updateDependentOnObjects(pAction);
    pAction->evaluateDependencies();
    setStatus(pAction, kIsUpToDateAssocStatus);
  }
}

void moveDepFromSrcGroup(OdDbAssoc2dConstraintGroup *pSrcGroup, OdDbAssoc2dConstraintGroup *pDestGroup);

struct ScanDependencies {
  OdDbObjectIdArray* toErase;
  OdDbObjectPtr* geometry;
  ScanDependencies(OdDbObjectPtr& geometry, OdDbObjectIdArray& toErase) {
    this->toErase = &toErase;
    this->geometry = &geometry;
  }
  void operator() (const OdDbObjectId& id) {
    OdDbAssocDependencyPtr clonedDependency = OdDbAssocDependency::cast(id.openObject(OdDb::kForWrite));
    if (clonedDependency.get()) {
      OdDbObjectPtr object = clonedDependency->dependentOnObject().openObject();
      if (object.get()) {
        if (geometry->isNull() && clonedDependency->isKindOf(OdDbAssocGeomDependency::desc()))
          *geometry = object;
        clonedDependency->setStatus(kChangedNoDifferenceAssocStatus);
      }
      else {
        clonedDependency->setStatus(kErasedAssocStatus);
        toErase->append(id);
      }
    }
  }
};


OdResult OdDbImpAssoc2dConstraintGroup::postProcessAfterDeepClone(OdDbAssocAction *pAction, OdDbIdMapping& idMap)
{
  OdDbAssoc2dConstraintGroupPtr groupClone =
    OdDbAssoc2dConstraintGroup::cast(::oddbTranslate(pAction->objectId(), idMap).openObject(OdDb::kForWrite));
  if (groupClone.isNull())
    return eOk;

  OdDbObjectIdArray clonedDependencies;
  groupClone->getDependencies(true, true, clonedDependencies);
  OdDbObjectPtr geometry;
  OdDbObjectIdArray toErase;
  std::for_each(clonedDependencies.begin(), clonedDependencies.end(), ScanDependencies(geometry, toErase));

  if (geometry.isNull()) {
    groupClone->erase();
    return eOk;
  }
  if (toErase.size()) {
    toErase.erase(std::unique(toErase.begin(), toErase.end()), toErase.end());
    groupClone->deleteConstrainedGeometry(toErase);
    if (groupClone->status() == kErasedAssocStatus) {
      groupClone->erase();
      return eOk;
    }
  }

  {
    const OdDb::DeepCloneType dcType = idMap.deepCloneContext();
    if (OdDb::kDcCopy == dcType || OdDb::kDcExplode == dcType) {
      OdArray<OdGeomConstraint*> constraints;
      if (groupClone->getConstraints(constraints) == eOk) {
        unsigned int i, gcLength = constraints.length();
        for (i = 0; i < gcLength; i++) {
          OdGeomConstraint * constraint = constraints.getAt(i);
          if (constraint->isKindOf(OdFixedConstraint::desc()))
            groupClone->deleteConstraint(constraint);
        }
        if (groupClone->status() == kErasedAssocStatus) {
          groupClone->erase();
          return eOk;
        }
      }
    }
  }

  // If after clearing cloned constraint group has no dependencies it will be erased
  if (clonedDependencies.size() - toErase.size() != 0)
  {
    // Check whether destination network already has group of same kind
    // Then actions have to be reassigned and newly created group has to be removed

    OdDbAssocNetworkPtr pDestParentNetwork = groupClone->owningNetwork().openObject(OdDb::kForRead);
    ODA_ASSERT(!pDestParentNetwork.isNull());
    if(pDestParentNetwork.isNull())
      return eOk;

    OdGePlane destWorkPlane = groupClone->getWorkPlane();

    const OdDbObjectIdArray actions = pDestParentNetwork->getActions();
    for (unsigned int i = 0, n = actions.size(); i < n; i++) {
      OdDbObjectId actionId = actions[i];
      if (actionId==groupClone->objectId())
        continue;
      OdDbAssoc2dConstraintGroupPtr checkGroup = OdDbAssoc2dConstraintGroup::cast(actionId.openObject());
      if (checkGroup.get() && destWorkPlane.isCoplanarTo(checkGroup->getWorkPlane())) {
        checkGroup->upgradeOpen();
        ::moveDepFromSrcGroup(groupClone, checkGroup);
        break;
      }
    }
  }
  return eOk;
}

class AddMoreObjectsFromConnectedConstraints {
  const OdDbImpAssoc2dConstraintGroup* groupImpl;
  OdDbObjectId actionId;
  OdDbIdMapping& idMap;
  OdDbObjectIdArray& objectsToClone;
  OdDbObjectIdArray objectsToCloneBuff;
  OdDbStubPtrHashSet objectsToCloneHash;
public:
  AddMoreObjectsFromConnectedConstraints(OdDbObjectId groupId, const OdDbImpAssoc2dConstraintGroup* group,
    OdDbIdMapping& idM, OdDbObjectIdArray& additionalObjects)
    : groupImpl(group)
    , actionId(groupId)
    , objectsToClone(additionalObjects)
    , idMap(idM)
  { }

  // predicate for each geometry dependency
  void operator ()(const OdDbObjectId& geomDepId) {
    OdDbAssocGeomDependencyPtr geomDep = OdDbAssocGeomDependency::cast(geomDepId.openObject());
    if (geomDep.get()) {
      OdDbIdPair idPair(geomDep->dependentOnObject());
      if (idMap.compute(idPair) && idPair.isCloned()) {
        OdConstrainedGeometry* geometry;
        const_cast<OdDbImpAssoc2dConstraintGroup*>(groupImpl)->getConstrainedGeometry(geomDep, geometry);
        if (geometry) {
          OdArray<OdConstrainedGeometry*> connectedGeometries;
          geometry->getConnectedGeometries(connectedGeometries);
          if (connectedGeometries.empty()) {
            if (geometry->isKindOf(OdConstrainedCurve::desc())) {
              OdConstraintGroupNodeImpl* imp =
                OdDbImpAssoc2dConstraintGroup::getConstraintGroupNodeImpl(geometry);
              static_cast<OdConstrainedCurveImpl*>(imp)->getConstrainedImplicitPoints(
                reinterpret_cast<OdArray<OdConstrainedImplicitPoint*>&>(connectedGeometries));
            }
            /*/  dna: not sure if including back link from implicit point to curve is necessary here
            else if (geometry->isKindOf(OdConstrainedImplicitPoint::desc())) {
              OdConstrainedImplicitPointImpl* pointImp = static_cast<OdConstrainedImplicitPointImpl*>(
                OdDbImpAssoc2dConstraintGroup::getConstraintGroupNodeImpl(geometry) );
              OdConstrainedGeometry* curveGeom =
                static_cast<OdConstrainedCurve*>(groupImpl->getGroupNodePtr(pointImp->constrainedCurveId()));
              if(curveGeom)
                connectedGeometries.append(curveGeom);
            }
            /*/
          }
          std::for_each(connectedGeometries.begin(), connectedGeometries.end(), *this);
          (*this)(geometry);
        }
      }
    }
  }

  // predicate for each geometry
  void operator ()(const OdConstrainedGeometry* geometry) {
    OdArray<OdGeomConstraint*> constraints;
    geometry->getConnectedConstraints(constraints);
    std::for_each(constraints.begin(), constraints.end(), *this);
  }

  // predicate for each constraint
  void operator ()(const OdGeomConstraint* constraint) {
    objectsToCloneBuff.clear();
    constraint->addMoreObjectsToDeepClone(idMap, objectsToCloneBuff);
    objectsToCloneHash.insert(objectsToCloneBuff.begin(), objectsToCloneBuff.end());
  }

  // finalize
  ~AddMoreObjectsFromConnectedConstraints() {
    OdDbAssocNetCloneCtxPtr ctx = OdDbAssocNetCloneCtx::internal_fromDb(idMap.destDb());
    OdDbStubPtrHashSet::iterator it = objectsToCloneHash.begin();
    OdDbStubPtrHashSet::iterator end = objectsToCloneHash.end();
    for(; it != end; ++it) {
      OdDbObjectId id(*it);
      if(OdDbAssocNetCloneCtx::isOwnedByEvalGraph(id)) {
        // objectId is a parameter of OdDbEvalGraph (which is ACAD_ENHANCEDBLOCK record of extension dictionary).
        // ACAD_ENHANCEDBLOCK should not be cloned. Only one copy of ACAD_ENHANCEDBLOCK is 
        // present in the "master" dynamic block definition.
        ctx->trackEvalGraphParam(actionId, id);
      }
      else
        objectsToClone.append(id);
    }
  }
};


inline bool needClone(const OdDbObject* obj, OdDbIdMapping& idMap) {
  return
    obj->database()->appServices()->getPARAMETERCOPYMODE()!=0
    || idMap.deepCloneContext()==OdDb::kDcExplode
    ;
}

OdResult OdDbImpAssoc2dConstraintGroup::addMoreObjectsToDeepClone(
  const OdDbAssocAction* pThisAction, OdDbIdMapping& idMap,
  OdDbObjectIdArray& additionalObjectsToClone) const
{
  if (::needClone(pThisAction, idMap)) {
    OdDbObjectIdArray dependencies;
    OdResult res = pThisAction->getDependencies(true, false, dependencies);
    if (res == eOk && dependencies.size())
      std::for_each(
        dependencies.begin(), dependencies.end(),
        AddMoreObjectsFromConnectedConstraints(pThisAction->objectId(), this, idMap, additionalObjectsToClone)
      );
  }
  return eOk;
}

OdResult OdDbImpAssoc2dConstraintGroup::dwgInFields(OdDbAssocAction *pSelf, OdDbDwgFiler* pFiler)
{
  OdResult res = OdDbImpAssocAction::dwgInFields(pSelf, pFiler);
  if ( res != eOk )
    return res;
  OdUInt32 ver = pFiler->rdInt16();
  ODA_ASSERT(!ver || ver == 2);

  m_Unk_Bool = pFiler->rdBool();
  ODA_ASSERT_ONCE(!m_Unk_Bool);
  OdGePoint3d ptOrigin = pFiler->rdPoint3d();
  OdGeVector3d vU = pFiler->rdVector3d();
  OdGeVector3d vV = pFiler->rdVector3d();
  m_plane.set(ptOrigin, vU, vV);
  m_Unk_ObjId = pFiler->rdHardOwnershipId();
  OdUInt32 nCnt = pFiler->rdInt32();
  m_arrDependencies.resize(nCnt);
  m_isOwnedDependency.resize(nCnt, true);
  OdDbObjectId* pId = m_arrDependencies.asArrayPtr();
  while (nCnt--)
  {
    *pId++ = pFiler->rdHardOwnershipId();
  }
  m_seedId = pFiler->rdInt32();

  if (pFiler->dwgVersion() <= OdDb::vAC24)
    res = dwgInNodes2010(pFiler, pSelf->objectId(), ver);
  else
    res = dwgInNodes2013(pFiler, pSelf->objectId());
  if ( res != eOk )
    return res;

  return eOk;
}

OdResult OdDbImpAssoc2dConstraintGroup::dwgInNodes2010(OdDbDwgFiler* pFiler, 
                                                       OdDbObjectId objectId,
                                                       OdUInt32 ver)
{
  ODA_ASSERT(!ver || ver == 2);
  OdUInt32 sizeNodes = pFiler->rdInt32();
  m_mapNodes.clear();
  for (OdUInt32 idx = 0; idx < sizeNodes; idx++)
  {
    OdString strName = pFiler->rdString();
    OdConstraintGroupNodePtr pNode = ::odrxCreateObject(strName);
    if (pNode.isNull())
    {
      ODA_FAIL(); // Class not registered
      throw OdError(eDwgObjectImproperlyRead);
    }
    if (!ver)
    {
      // intermediate R24 version (thre is no way to save data in this format)
      // dataFiles/dwg/ACADSamples/2010/architectural_example-imperial.dwg
      //
      // set marker for OdConstraintGroupNodeImpl::dwgInFields
      OdConstraintGroupNodeId id = pFiler->rdInt32();
      if (!id || !pNode->m_pImpl)
      {
        ODA_FAIL_ONCE(); // TODO
        return eMakeMeProxy;
      }
      pNode->m_pImpl->setNodeId(id);
    }

    OdResult res = pNode->dwgInFields(pFiler);
    if (res != eOk)
      return res;
    if (!m_mapNodes.insert(std::make_pair(pNode->m_pImpl->nodeId(), pNode)).second)
    {
      ODA_FAIL(); // nodeId duplicated
      m_mapNodes[UINT_MAX] = 0;
    }
    pNode->m_pImpl->setOwnerAction(objectId);
  }
  return eOk;
}

OdResult OdDbImpAssoc2dConstraintGroup::dwgInNodes2013(OdDbDwgFiler* pFiler, OdDbObjectId objectId)
{
  if (pFiler->rdInt16())
  { // TODO AC2013
    ODA_FAIL_ONCE();
    return eMakeMeProxy;
  }
  if (pFiler->rdInt32())
  { // TODO AC2013
    ODA_FAIL_ONCE();
    return eMakeMeProxy;
  }
  if (pFiler->rdBool())
  { // TODO AC2013
    ODA_FAIL_ONCE();
    return eMakeMeProxy;
  }

  OdUInt32 sizeDictClassNames  = pFiler->rdInt32();
  OdStringArray asDictClassNames;
  asDictClassNames.resize(sizeDictClassNames);
  OdUInt32 index = 0;
  for (; index < sizeDictClassNames; index++)
  {
    OdString strName = pFiler->rdString();
    ODA_ASSERT_ONCE(!strName.isEmpty());

    asDictClassNames[index] = strName;
  }

  OdUInt32 sizeNodes = pFiler->rdInt32();
  OdArray<OdConstraintGroupNodePtr> arrNodes;
  arrNodes.resize(sizeNodes);
  for (index = 0; index < sizeNodes; index++)
  {
    if (pFiler->rdBool())
    {
      ODA_FAIL_ONCE(); // TODO
      return eMakeMeProxy;
    }

    OdUInt32 idx = pFiler->rdInt32();
    idx--;
    ODA_ASSERT_ONCE(idx < sizeDictClassNames);

    OdString strName = asDictClassNames[idx];
    ODA_ASSERT_ONCE(!strName.isEmpty());

    OdConstraintGroupNodePtr pNode = ::odrxCreateObject(strName);
    if (pNode.isNull())
    {
      ODA_FAIL(); // Class not registered
      throw OdError(eDwgObjectImproperlyRead);
    }
    ODA_ASSERT_ONCE(strName == pNode->isA()->name());

    OdConstraintGroupNodeId idNode = pFiler->rdInt32();
    ODA_ASSERT_ONCE(idNode && idNode <= m_seedId); // test

    OdConstraintGroupNodeImpl* pImpl = pNode->m_pImpl;
    pImpl->setNodeId(idNode);
    arrNodes[index] = pNode;
  }
  m_mapNodes.clear();
  for (index = 0; index < arrNodes.size(); index++)
  {
    OdConstraintGroupNodePtr pNode = arrNodes[index];
    OdConstraintGroupNodeImpl* pImpl = pNode->m_pImpl;
    ODA_ASSERT_ONCE(pImpl->nodeId());
    OdResult res = pNode->dwgInFields(pFiler);
    if (res != eOk)
      return res;
    if (!m_mapNodes.insert(std::make_pair(pImpl->nodeId(), pNode)).second)
    {
      ODA_FAIL(); // nodeId duplicated
      m_mapNodes[UINT_MAX] = 0;
    }

    pImpl->setOwnerAction(objectId);
  }
  return eOk;
}

void OdDbImpAssoc2dConstraintGroup::dwgOutFields(OdDbDwgFiler* pFiler, OdDbObjectId objectId) const
{
  OdDbImpAssocAction::dwgOutFields(pFiler, objectId);

  pFiler->wrInt16(2);   // Version?
  ODA_ASSERT_ONCE(!m_Unk_Bool);
  pFiler->wrBool(m_Unk_Bool);

  OdGePoint3d ptOrigin;
  OdGeVector3d vU, vV;
  m_plane.get(ptOrigin, vU, vV);
  pFiler->wrPoint3d(ptOrigin);
  pFiler->wrVector3d(vU);
  pFiler->wrVector3d(vV);

  pFiler->wrHardOwnershipId(m_Unk_ObjId);
  OdUInt32 nCnt = m_arrDependencies.size();
  pFiler->wrInt32(nCnt);
  const OdDbObjectId* pId = m_arrDependencies.asArrayPtr();
  while (nCnt--)
  {
    pFiler->wrHardOwnershipId(*pId++);
  }
  pFiler->wrInt32(m_seedId);

  if (pFiler->dwgVersion() <= OdDb::vAC24)
    dwgOutNodes2010(pFiler, objectId);
  else
    dwgOutNodes2013(pFiler, objectId);
}

void OdDbImpAssoc2dConstraintGroup::dwgOutNodes2010(OdDbDwgFiler* pFiler, OdDbObjectId objectId) const
{
  pFiler->wrInt32(OdInt32(m_mapNodes.size()));
  for (OdConstraintGroupNodeMap::const_iterator pNode = m_mapNodes.begin(); pNode != m_mapNodes.end(); ++pNode)
  {
    pFiler->wrString(pNode->second->isA()->name());
    pNode->second->dwgOutFields(pFiler);
  }
}

void OdDbImpAssoc2dConstraintGroup::dwgOutNodes2013(OdDbDwgFiler* pFiler, OdDbObjectId objectId) const
{
  pFiler->wrInt16(0);
  pFiler->wrInt32(0);
  pFiler->wrBool(false);

  // collect dictionary of class names
  OdStringArray asDictClassNames;
  for (OdConstraintGroupNodeMap::const_iterator pNode = m_mapNodes.begin(); pNode != m_mapNodes.end(); ++pNode)
  {
    OdString strName = pNode->second->isA()->name();
    if (asDictClassNames.contains(strName))
      continue;
    asDictClassNames.push_back(strName);
  }

  // write dictionary
  OdUInt32 sizeDictClassNames = asDictClassNames.size();
  pFiler->wrInt32(sizeDictClassNames);
  for (OdUInt32 index = 0; index < sizeDictClassNames; index++)
  {
    pFiler->wrString(asDictClassNames[index]);
  }

  // write section to create node instances (with indexes to dictionary)
  pFiler->wrInt32(OdInt32(m_mapNodes.size()));
  for (OdConstraintGroupNodeMap::const_iterator pNode = m_mapNodes.begin(); pNode != m_mapNodes.end(); ++pNode)
  {
    pFiler->wrBool(false);

    OdString strName = pNode->second->isA()->name();
    unsigned int idx = (unsigned int)(-1);
    if (!asDictClassNames.find(strName, idx))
    {
      ODA_FAIL_ONCE();
      throw OdError(eFileInternalErr);
    }
    idx++; // 1 base index
    ODA_ASSERT_ONCE(idx && idx <= sizeDictClassNames);

    pFiler->wrInt32(idx);

    const OdConstraintGroupNodeImpl* pImpl = pNode->second->m_pImpl;
    OdConstraintGroupNodeId idNode = pImpl->nodeId();
    ODA_ASSERT_ONCE(idNode && idNode <= m_seedId);

    pFiler->wrInt32(idNode);
  }

  for (OdConstraintGroupNodeMap::const_iterator pNode = m_mapNodes.begin(); pNode != m_mapNodes.end(); ++pNode)
  {
    pNode->second->dwgOutFields(pFiler);
  }
}

#define NEXT_CODE(code) \
if (pFiler->nextItem() != code) \
{                               \
  ODA_FAIL();                   \
  return eBadDxfSequence;       \
}


OdResult OdDbImpAssoc2dConstraintGroup::dxfInFields(OdDbDxfFiler* pFiler, OdDbObjectId objectId)
{
  OdResult res = OdDbImpAssocAction::dxfInFields(pFiler, objectId);
  if ( res != eOk )
    return res;

  NEXT_CODE(90)
#ifdef ODA_DIAGNOSTICS
  OdUInt32 nUnk = 
#endif // ODA_DIAGNOSTICS
  pFiler->rdUInt32();
  ODA_ASSERT(nUnk == 2);  // Version?

  NEXT_CODE(70)
  m_Unk_Bool = (pFiler->rdInt16() != 0);
  ODA_ASSERT_ONCE(!m_Unk_Bool);
  NEXT_CODE(10)
  OdGePoint3d ptOrigin;
  pFiler->rdPoint3d(ptOrigin);
  NEXT_CODE(10)
  OdGeVector3d vU;
  pFiler->rdVector3d(vU);
  NEXT_CODE(10)
  OdGeVector3d vV;
  pFiler->rdVector3d(vV);
  m_plane.set(ptOrigin, vU, vV);
  NEXT_CODE(360)
  m_Unk_ObjId = pFiler->rdObjectId();
  NEXT_CODE(90)
  OdUInt32 nCnt = pFiler->rdUInt32();
  m_arrDependencies.resize(nCnt);
  m_isOwnedDependency.resize(nCnt, true);
  OdDbObjectId* pId = m_arrDependencies.asArrayPtr();
  while (nCnt--)
  {
    NEXT_CODE(360)
    *pId++ = pFiler->rdObjectId();
  }
  NEXT_CODE(90)
  m_seedId = pFiler->rdUInt32();

  if (pFiler->dwgVersion() <= OdDb::vAC24)
    res = dxfInNodes2010(pFiler, objectId);
  else
    res = dxfInNodes2013(pFiler, objectId);
  if (res != eOk)
    return res;

  ODA_ASSERT_ONCE(pFiler->atEOF());  // TODO
  return eOk;
}

OdResult OdDbImpAssoc2dConstraintGroup::dxfInNodes2010(OdDbDxfFiler* pFiler, OdDbObjectId objectId)
{
  OdResult res;
  NEXT_CODE(90)
  unsigned int nCnt = pFiler->rdUInt32();
  m_mapNodes.clear();
  while (nCnt--)
  {
    NEXT_CODE(1)
    OdString strName = pFiler->rdString();
    OdConstraintGroupNodePtr pNode = ::odrxCreateObject(strName);
    if (pNode.isNull())
    {
      ODA_FAIL(); // Class not registered
      throw OdError(eDwgObjectImproperlyRead);
    }
    res = pNode->dxfInFields(pFiler);
    if (res != eOk)
      return res;
    pNode->m_pImpl->setOwnerAction(objectId);
    if (pNode->nodeId() == 0)
      return eBadDxfSequence;
    if (!m_mapNodes.insert(std::make_pair(pNode->nodeId(), pNode)).second)
    {
      ODA_FAIL(); // nodeId duplicated
      m_mapNodes[UINT_MAX] = 0;
    }
  }
  return eOk;
}

OdResult OdDbImpAssoc2dConstraintGroup::dxfInNodes2013(OdDbDxfFiler* pFiler, OdDbObjectId objectId)
{
  OdResult res;
  NEXT_CODE(90)
  unsigned int nCnt = pFiler->rdUInt32();
  if (nCnt > 0)
  {
    ODA_FAIL_ONCE();
    return eMakeMeProxy;
  }
  NEXT_CODE(90)
  nCnt = pFiler->rdUInt32();
  if (nCnt > 0)
  {
    ODA_FAIL_ONCE();
    return eMakeMeProxy;
  }
  NEXT_CODE(290)
  if (pFiler->rdBool())
  {
    ODA_FAIL_ONCE();
    return eMakeMeProxy;
  }

  NEXT_CODE(90)
  nCnt = pFiler->rdUInt32();
  OdArray<OdConstraintGroupNodePtr> arrNodes;
  unsigned int nCnt1 = nCnt;
  while (nCnt--)
  {
    NEXT_CODE(1)
    OdString strName = pFiler->rdString();
    OdConstraintGroupNodePtr pNode = ::odrxCreateObject(strName);
    if (pNode.isNull())
    {
      ODA_FAIL(); // Class not registered
      throw OdError(eDwgObjectImproperlyRead);
    }
    OdConstraintGroupNodeImpl *pImpl = pNode->m_pImpl;

    NEXT_CODE(90)
    pImpl->setNodeId(pFiler->rdUInt32());
    arrNodes.push_back(pNode);
  }
  m_mapNodes.clear();
  for (unsigned int index = 0; index < arrNodes.size(); index++)
  {
    OdConstraintGroupNodePtr pNode = arrNodes[index];
    res = pNode->dxfInFields(pFiler);
    if (res != eOk)
      return res;
    OdConstraintGroupNodeImpl *pImpl = pNode->m_pImpl;
    if (!m_mapNodes.insert(std::make_pair(pImpl->nodeId(), pNode)).second)
    {
      ODA_FAIL(); // nodeId duplicated
      m_mapNodes[UINT_MAX] = 0;
    }
    pImpl->setOwnerAction(objectId);
  }

  return eOk;
}

void OdDbImpAssoc2dConstraintGroup::dxfOutFields(OdDbDxfFiler* pFiler, OdDbObjectId objectId) const
{
  OdDbImpAssocAction::dxfOutFields(pFiler, objectId);
  pFiler->wrUInt32(90, 2);  // Version?

  ODA_ASSERT_ONCE(!m_Unk_Bool);
  pFiler->wrInt16(70, m_Unk_Bool ? 1 : 0);

  OdGePoint3d ptOrigin;
  OdGeVector3d vU, vV;
  m_plane.get(ptOrigin, vU, vV);
  pFiler->wrPoint3d(10, ptOrigin);
  pFiler->wrVector3d(10, vU);
  pFiler->wrVector3d(10, vV);

  pFiler->wrObjectId(360, m_Unk_ObjId);
  OdUInt32 nCnt = m_arrDependencies.size();
  pFiler->wrUInt32(90, nCnt);
  const OdDbObjectId* pId = m_arrDependencies.asArrayPtr();
  while (nCnt--)
  {
    pFiler->wrObjectId(360, *pId++);
  }
  pFiler->wrUInt32(90, m_seedId);

  if (pFiler->dwgVersion() <= OdDb::vAC24)
    dxfOutNodes2010(pFiler, objectId);
  else
    dxfOutNodes2013(pFiler, objectId);
}

void OdDbImpAssoc2dConstraintGroup::dxfOutNodes2010(OdDbDxfFiler* pFiler, OdDbObjectId objectId) const
{
  pFiler->wrUInt32(90, OdUInt32(m_mapNodes.size()));
  for (OdConstraintGroupNodeMap::const_iterator pNode = m_mapNodes.begin(); pNode != m_mapNodes.end(); ++pNode)
  {
    OdString strName = pNode->second->isA()->name();
    pFiler->wrString(1, strName);
    pNode->second->dxfOutFields(pFiler);
  }
}

void OdDbImpAssoc2dConstraintGroup::dxfOutNodes2013(OdDbDxfFiler* pFiler, OdDbObjectId objectId) const
{
  pFiler->wrUInt32(90, 0);
  pFiler->wrUInt32(90, 0);
  pFiler->wrBool(290, false);
  pFiler->wrUInt32(90, OdUInt32(m_mapNodes.size()));
  for (OdConstraintGroupNodeMap::const_iterator pNode = m_mapNodes.begin(); pNode != m_mapNodes.end(); ++pNode)
  {
    pFiler->wrString(1, pNode->second->isA()->name());
    pFiler->wrUInt32(90, pNode->first);
  }
  for (OdConstraintGroupNodeMap::const_iterator pNode = m_mapNodes.begin(); pNode != m_mapNodes.end(); ++pNode)
    pNode->second->dxfOutFields(pFiler);
}

//add constraints functions
bool OdDbImpAssoc2dConstraintGroup::isConstraintOfThisTypeExist(const OdArray<OdGeomConstraint*> aConstr,
                                                                const OdRxClass* pObjType,
                                                                OdGeomConstraint** ppConstraint)
{
  unsigned int size = aConstr.size();

  for ( unsigned int i = 0; i < size; i++ )
  {
    if ( aConstr[i]->isA() == pObjType )
    {
      *ppConstraint = aConstr[i];
      return true;
    }
  }

  return false;
}

OdResult OdDbImpAssoc2dConstraintGroup::getConstraintGroupNodeObjects(OdArray<OdConstraintGroupNodePtr>& aResObj, OdRxClass* pObjType)
{
  for (OdConstraintGroupNodeMap::const_iterator pNode = m_mapNodes.begin(); pNode != m_mapNodes.end(); ++pNode)
  {
    if (pNode->second->isA() == pObjType)
      aResObj.push_back(pNode->second);
  }
  if ( aResObj.length() > 0 )
    return eOk;
  else
    return eBadObjType;
}

bool OdDbImpAssoc2dConstraintGroup::findDatumLine(OdConstrainedDatumLine** ppDatumLine, const OdGeVector3d& dir)
{

  OdArray<OdConstraintGroupNodePtr> aDatumObj;
  OdResult res = getConstraintGroupNodeObjects(aDatumObj, OdConstrainedDatumLine::desc());
  if ( res != eOk )
  {
    *ppDatumLine = NULL;
    return false;
  }

  unsigned int size = aDatumObj.size();
  OdConstrainedDatumLine * pLine;
  OdGeVector3d curDir;
  for ( unsigned int i = 0; i < size; i++ )
  {
    pLine = OdConstrainedDatumLine::cast(aDatumObj[i]);
    curDir = curDir = pLine->direction();
    if ( curDir == dir )
    {
      *ppDatumLine = pLine;
      return true;
    }
  }

  *ppDatumLine = NULL;
  return false;
}

OdResult OdDbImpAssoc2dConstraintGroup::tieNode(OdConstraintGroupNodePtr node, OdDbObjectId actionId)
{
  OdConstraintGroupNodeImpl* nodeImpl = OdDbImpAssoc2dConstraintGroup::getConstraintGroupNodeImpl(node);
  nodeImpl->setNodeId(++m_seedId);
  if (!actionId.isNull())
    nodeImpl->setOwnerAction(actionId);
  if (!m_mapNodes.insert(std::make_pair(node->nodeId(), node)).second)
  {
    ODA_FAIL_ONCE();
    throw OdError(eDuplicateKey);
  }
  return eOk;
}

void OdDbImpAssoc2dConstraintGroup::tieGeomsAndCnstr3(OdGeomConstraint *pCnstr,
                                                      OdConstrainedGeometry* pGeomF,
                                                      OdConstrainedGeometry* pGeomS,
                                                      OdConstrainedGeometry* pGeomT)
{
  OdConstraintGroupNodeImpl *pGrNodeImpl = OdDbImpAssoc2dConstraintGroup::getConstraintGroupNodeImpl(pGeomF);
  pGrNodeImpl->addConnection(pCnstr->nodeId());

  if (pGeomS) {
    pGrNodeImpl = OdDbImpAssoc2dConstraintGroup::getConstraintGroupNodeImpl(pGeomS);
    pGrNodeImpl->addConnection(pCnstr->nodeId());
  }

  if (pGeomT) {
    pGrNodeImpl = OdDbImpAssoc2dConstraintGroup::getConstraintGroupNodeImpl(pGeomT);
    pGrNodeImpl->addConnection(pCnstr->nodeId());
  }

  pGrNodeImpl = OdDbImpAssoc2dConstraintGroup::getConstraintGroupNodeImpl(pCnstr);
  pGrNodeImpl->addConnection(pGeomF->nodeId());
  if (pGeomS)
    pGrNodeImpl->addConnection(pGeomS->nodeId());
  if (pGeomT)
    pGrNodeImpl->addConnection(pGeomT->nodeId());
}

void OdDbImpAssoc2dConstraintGroup::tieDimAndCnstr(OdExplicitConstraint *pCnstr,
                                                   OdDbObjectId valueDependencyId,
                                                   OdDbObjectId dimDependencyId,
                                                   OdDbAssoc2dConstraintGroup *pGroup)
{
  addDependency(valueDependencyId, true, pGroup);
  pCnstr->setValueDependencyId(valueDependencyId);
  if (!dimDependencyId.isNull()) {
    addDependency(dimDependencyId, true, pGroup);
    pCnstr->setDimDependencyId(dimDependencyId);
  }
}

void OdDbImpAssoc2dConstraintGroup::tieGeomsAndCnstr(OdConstrainedGeometry* pGeomF,
                                                     OdConstrainedGeometry* pGeomS,
                                                     OdGeomConstraint *pCnstr)
{
  OdConstraintGroupNodeImpl *pGrNodeImpl = OdDbImpAssoc2dConstraintGroup::getConstraintGroupNodeImpl(pGeomF);
  pGrNodeImpl->addConnection(pCnstr->nodeId());

  pGrNodeImpl = OdDbImpAssoc2dConstraintGroup::getConstraintGroupNodeImpl(pGeomS);
  pGrNodeImpl->addConnection(pCnstr->nodeId());

  pGrNodeImpl = OdDbImpAssoc2dConstraintGroup::getConstraintGroupNodeImpl(pCnstr);
  pGrNodeImpl->addConnection(pGeomF->nodeId());
  pGrNodeImpl->addConnection(pGeomS->nodeId());
}

void OdDbImpAssoc2dConstraintGroup::tieGeomsAndCnstrWithHeplPrm(OdConstrainedGeometry* pGeomF,
                                                                OdConstrainedGeometry* pGeomS,
                                                                OdGeomConstraint *pCnstr,
                                                                OdHelpParameter *pHelpPrmF,
                                                                OdHelpParameter *pHelpPrmS)
{
  OdConstraintGroupNodeImpl *pGrNodeImpl = OdDbImpAssoc2dConstraintGroup::getConstraintGroupNodeImpl(pGeomF);

  pGrNodeImpl = OdDbImpAssoc2dConstraintGroup::getConstraintGroupNodeImpl(pGeomF);
  pGrNodeImpl->addConnection(pCnstr->nodeId());
  if ( pHelpPrmF != NULL )
    pGrNodeImpl->addConnection(pHelpPrmF->nodeId());

  pGrNodeImpl = OdDbImpAssoc2dConstraintGroup::getConstraintGroupNodeImpl(pGeomS);
  pGrNodeImpl->addConnection(pCnstr->nodeId());
  if ( pHelpPrmS != NULL )
    pGrNodeImpl->addConnection(pHelpPrmS->nodeId());

  pGrNodeImpl = OdDbImpAssoc2dConstraintGroup::getConstraintGroupNodeImpl(pCnstr);
  pGrNodeImpl->addConnection(pGeomF->nodeId());
  pGrNodeImpl->addConnection(pGeomS->nodeId());
  if ( pHelpPrmF != NULL )
    pGrNodeImpl->addConnection(pHelpPrmF->nodeId());

  if ( pHelpPrmS != NULL )
    pGrNodeImpl->addConnection(pHelpPrmS->nodeId());

  if ( pHelpPrmF != NULL )
  {
    pGrNodeImpl = OdDbImpAssoc2dConstraintGroup::getConstraintGroupNodeImpl(pHelpPrmF);
    pGrNodeImpl->addConnection(pGeomF->nodeId());
    pGrNodeImpl->addConnection(pCnstr->nodeId());
  }

  if ( pHelpPrmS != NULL )
  {
    pGrNodeImpl = OdDbImpAssoc2dConstraintGroup::getConstraintGroupNodeImpl(pHelpPrmS);
    pGrNodeImpl->addConnection(pGeomS->nodeId());
    pGrNodeImpl->addConnection(pCnstr->nodeId());
  }
}

OdGeomConstraint* OdDbImpAssoc2dConstraintGroup::addHorizontalConstraint(OdConstrainedGeometry* pGeomF,
                                                                         OdDbObjectId actionId,
                                                                         const OdGeVector3d& axis)
{
  return addVertHorzConstraint(pGeomF, actionId, OdHorizontalConstraint::desc(), axis,
                               OdHorizontalConstraint::desc()->name());
}

OdGeomConstraint* OdDbImpAssoc2dConstraintGroup::addHorizontalConstraint(OdConstrainedGeometry* pGeomF,
                                                                         OdConstrainedGeometry* pGeomS,
                                                                         OdDbObjectId actionId,
                                                                         const OdGeVector3d& axis)
{
  return addVertHorzConstraint(pGeomF, pGeomS, actionId, OdHorizontalConstraint::desc(), axis,
                               OdHorizontalConstraint::desc()->name());
}

OdGeomConstraint* OdDbImpAssoc2dConstraintGroup::addVerticalConstraint(OdConstrainedGeometry* pGeomF,
                                                                       OdDbObjectId actionId,
                                                                       const OdGeVector3d& axis)
{
  return addVertHorzConstraint(pGeomF, actionId, OdVerticalConstraint::desc(), axis,
                               OdVerticalConstraint::desc()->name());
}

OdGeomConstraint* OdDbImpAssoc2dConstraintGroup::addVerticalConstraint(OdConstrainedGeometry* pGeomF,
                                                                       OdConstrainedGeometry* pGeomS,
                                                                       OdDbObjectId actionId,
                                                                       const OdGeVector3d& axis)
{
  return addVertHorzConstraint(pGeomF, pGeomS, actionId, OdVerticalConstraint::desc(), axis,
                               OdVerticalConstraint::desc()->name());
}

OdGeomConstraint* OdDbImpAssoc2dConstraintGroup::addParallelConstraint(OdConstrainedGeometry* pGeomF,
                                                                       OdConstrainedGeometry* pGeomS,
                                                                       OdDbObjectId actionId)
{
  OdGeomConstraint* res = NULL;

  if ( !pGeomF->isKindOf(OdConstrainedLine::desc()) || !pGeomS->isKindOf(OdConstrainedLine::desc()) )
    return res;

  if ( pGeomF->nodeId() == pGeomS->nodeId() )
    return res;

  OdArray<OdGeomConstraint*> aConstr;
  pGeomF->getCommonConstraints(pGeomS, aConstr);

  if ( isConstraintOfThisTypeExist(aConstr, OdParallelConstraint::desc(), &res) )
    return NULL;

  OdSmartPtr<OdParallelConstraint> spPrlCnstr = ::odrxCreateObject(OdParallelConstraint::desc()->name());
  tieNode(spPrlCnstr , actionId);

  res = spPrlCnstr.get();
  tieGeomsAndCnstr(pGeomF, pGeomS, res);

  return res;
}

OdGeomConstraint* OdDbImpAssoc2dConstraintGroup::addPerpendicularConstraint(OdConstrainedGeometry* pGeomF,
                                                                            OdConstrainedGeometry* pGeomS,
                                                                            OdDbObjectId actionId)
{
  OdGeomConstraint* res = NULL;

  if ( !pGeomF->isKindOf(OdConstrainedLine::desc()) || !pGeomS->isKindOf(OdConstrainedLine::desc()) )
    return res;

  if ( pGeomF->nodeId() == pGeomS->nodeId() )
    return res;

  OdArray<OdGeomConstraint*> aConstr;
  pGeomF->getCommonConstraints(pGeomS, aConstr);

  if ( isConstraintOfThisTypeExist(aConstr, OdPerpendicularConstraint::desc(), &res) )
    return NULL;

  OdSmartPtr<OdPerpendicularConstraint> spPrpCnstr = ::odrxCreateObject(OdPerpendicularConstraint::desc()->name());
  tieNode(spPrpCnstr, actionId);

  res = spPrpCnstr.get();
  tieGeomsAndCnstr(pGeomF, pGeomS, res);

  return res;
}

OdGeomConstraint* OdDbImpAssoc2dConstraintGroup::addNormalConstraint(OdConstrainedGeometry* pGeomF,
                                                                     OdConstrainedGeometry* pGeomS,
                                                                     OdDbObjectId actionId)
{
  OdGeomConstraint* res = NULL;

  if ( (!pGeomF->isKindOf(OdConstrainedLine::desc()) || !pGeomS->isKindOf(OdConstrainedCircle::desc())) &&
       (!pGeomF->isKindOf(OdConstrainedCircle::desc()) || !pGeomS->isKindOf(OdConstrainedLine::desc())) )
    return res;

  if ( pGeomF->nodeId() == pGeomS->nodeId() )
    return res;

  OdArray<OdGeomConstraint*> aConstr;
  pGeomF->getCommonConstraints(pGeomS, aConstr);

  if ( isConstraintOfThisTypeExist(aConstr, OdNormalConstraint::desc(), &res) )
    return NULL;

  OdSmartPtr<OdNormalConstraint> spNrmCnstr = ::odrxCreateObject(OdNormalConstraint::desc()->name());
  tieNode(spNrmCnstr, actionId);

  res = spNrmCnstr.get();
  tieGeomsAndCnstr(pGeomF, pGeomS, res);

  return res;
}

OdGeomConstraint* OdDbImpAssoc2dConstraintGroup::addColinearConstraint(OdConstrainedGeometry* pGeomF,
                                                                       OdConstrainedGeometry* pGeomS,
                                                                       OdDbObjectId actionId)
{
  OdGeomConstraint* res = NULL;

  if ( !pGeomF->isKindOf(OdConstrainedLine::desc()) || !pGeomS->isKindOf(OdConstrainedLine::desc()) )
    return res;

  if ( pGeomF->nodeId() == pGeomS->nodeId() )
    return res;

  OdArray<OdGeomConstraint*> aConstr;
  pGeomF->getCommonConstraints(pGeomS, aConstr);

  if ( isConstraintOfThisTypeExist(aConstr, OdColinearConstraint::desc(), &res) )
    return NULL;

  OdSmartPtr<OdColinearConstraint> spClnCnstr = ::odrxCreateObject(OdColinearConstraint::desc()->name());
  tieNode(spClnCnstr, actionId);

  res = spClnCnstr.get();
  tieGeomsAndCnstr(pGeomF, pGeomS, res);

  return res;
}

OdGeomConstraint* OdDbImpAssoc2dConstraintGroup::addCoincidentConstraint(OdConstrainedGeometry* pGeomF,
                                                                         OdConstrainedGeometry* pGeomS,
                                                                         OdDbObjectId actionId)
{
  OdGeomConstraint* res = NULL;

  if ( !(pGeomF->isKindOf(OdConstrainedPoint::desc()) || pGeomS->isKindOf(OdConstrainedPoint::desc())) )
    return res;

  if ( pGeomF->nodeId() == pGeomS->nodeId() )
    return res;

  if ( pGeomF->isKindOf(OdConstrainedImplicitPoint::desc()) && pGeomS->isKindOf(OdConstrainedImplicitPoint::desc()) )
  {
    OdConstrainedImplicitPoint *pImplPtF = OdConstrainedImplicitPoint::cast(pGeomF);
    OdConstrainedImplicitPoint *pImplPtS = OdConstrainedImplicitPoint::cast(pGeomS);
    if ( pImplPtF->constrainedCurveId() == pImplPtS->constrainedCurveId() )
      return res;
  }

  OdArray<OdGeomConstraint*> aConstr;
  pGeomF->getCommonConstraints(pGeomS, aConstr);

  if ( isConstraintOfThisTypeExist(aConstr, OdPointCoincidenceConstraint::desc(), &res) )
    return NULL;

  OdSmartPtr<OdPointCoincidenceConstraint> spCoiCnstr = ::odrxCreateObject(OdPointCoincidenceConstraint::desc()->name());
  tieNode(spCoiCnstr, actionId);

  res = spCoiCnstr.get();
  tieGeomsAndCnstr(pGeomF, pGeomS, res);

  return res;
}


OdGeomConstraint* OdDbImpAssoc2dConstraintGroup::addConcentricConstraint(OdConstrainedGeometry* pGeomF,
                                                                         OdConstrainedGeometry* pGeomS,
                                                                         OdDbObjectId actionId)
{
  OdGeomConstraint* res = NULL;

  if ( (!pGeomF->isKindOf(OdConstrainedCircle::desc()) && !pGeomF->isKindOf(OdConstrainedEllipse::desc())) ||
       (!pGeomS->isKindOf(OdConstrainedCircle::desc()) && !pGeomS->isKindOf(OdConstrainedEllipse::desc())) )
    return res;

  if ( pGeomF->nodeId() == pGeomS->nodeId() )
    return res;

  OdArray<OdGeomConstraint*> aConstr;
  pGeomF->getCommonConstraints(pGeomS, aConstr);

  if ( isConstraintOfThisTypeExist(aConstr, OdConcentricConstraint::desc(), &res) )
    return NULL;

  OdSmartPtr<OdConcentricConstraint> spClnCnstr = ::odrxCreateObject(OdConcentricConstraint::desc()->name());
  tieNode(spClnCnstr, actionId);

  res = spClnCnstr.get();
  tieGeomsAndCnstr(pGeomF, pGeomS, res);

  return res;
}

OdGeomConstraint* OdDbImpAssoc2dConstraintGroup::addTangentConstraint(OdConstrainedGeometry* pGeomF,
                                                                      OdConstrainedGeometry* pGeomS,
                                                                      OdHelpParameter* pHelpParamF,
                                                                      OdHelpParameter* pHelpParamS,
                                                                      OdDbObjectId actionId)
{
  OdGeomConstraint* res = NULL;

  if ( pHelpParamF == NULL && pHelpParamS == NULL )
  {
    bool bLineF = pGeomF->isKindOf(OdConstrainedLine::desc());
    if ( bLineF )
    {
      if ( !pGeomS->isKindOf(OdConstrainedCircle::desc()) && !pGeomS->isKindOf(OdConstrainedEllipse::desc()) )
        return res;

    }
    else if ( !pGeomF->isKindOf(OdConstrainedCircle::desc()) && !pGeomF->isKindOf(OdConstrainedEllipse::desc()) )
      return res;
    else if ( !pGeomS->isKindOf(OdConstrainedCircle::desc()) && !pGeomS->isKindOf(OdConstrainedEllipse::desc()) && 
              !pGeomS->isKindOf(OdConstrainedLine::desc()))
      return res;
  }
  else
  {
    if ( pHelpParamF != NULL && pHelpParamS != NULL )
    {
      if ( !pGeomF->isKindOf(OdConstrainedSpline::desc()) && !pGeomS->isKindOf(OdConstrainedSpline::desc()) )
        return res;
    }
    else 
		{
    OdConstrainedGeometry *pGeom = pHelpParamF == NULL ? pGeomF : pGeomS;
    if ( !pGeom->isKindOf(OdConstrainedArc::desc()) && !pGeom->isKindOf(OdConstrainedBoundedEllipse::desc()) &&
          !pGeom->isKindOf(OdConstrainedBoundedLine::desc()))
          return res;
		}
  }

  if ( pGeomF->nodeId() == pGeomS->nodeId() )
    return res;

  OdArray<OdGeomConstraint*> aConstr;
  pGeomF->getCommonConstraints(pGeomS, aConstr);

  OdSmartPtr<OdTangentConstraint> spTanCnstr = ::odrxCreateObject(OdTangentConstraint::desc()->name());
  tieNode(spTanCnstr, actionId);

  res = spTanCnstr.get();
  tieGeomsAndCnstrWithHeplPrm(pGeomF, pGeomS, res, pHelpParamF, pHelpParamS);

  return res;
}

OdGeomConstraint* OdDbImpAssoc2dConstraintGroup::addEqualRadiusConstraint(OdConstrainedGeometry* pGeomF,
                                                                          OdConstrainedGeometry* pGeomS,
                                                                          OdDbObjectId actionId)
{
  OdGeomConstraint* res = NULL;

  if ( !pGeomF->isKindOf(OdConstrainedCircle::desc()) || !pGeomS->isKindOf(OdConstrainedCircle::desc()))
    return res;

  if ( pGeomF->nodeId() == pGeomS->nodeId() )
    return res;

  OdArray<OdGeomConstraint*> aConstr;
  pGeomF->getCommonConstraints(pGeomS, aConstr);

  if ( isConstraintOfThisTypeExist(aConstr, OdEqualRadiusConstraint::desc(), &res) )
    return NULL;

  OdSmartPtr<OdEqualRadiusConstraint> spEqrCnstr = ::odrxCreateObject(OdEqualRadiusConstraint::desc()->name());
  tieNode(spEqrCnstr, actionId);

  res = spEqrCnstr.get();
  tieGeomsAndCnstr(pGeomF, pGeomS, res);

  return res;
}

OdGeomConstraint* OdDbImpAssoc2dConstraintGroup::addEqualLengthConstraint(OdConstrainedGeometry* pGeomF,
                                                                          OdConstrainedGeometry* pGeomS,
                                                                          OdDbObjectId actionId)
{
  OdGeomConstraint* res = NULL;

  if ( !pGeomF->isKindOf(OdConstrainedBoundedLine::desc()) || !pGeomS->isKindOf(OdConstrainedBoundedLine::desc()))
    return res;

  if ( pGeomF->nodeId() == pGeomS->nodeId() )
    return res;

  OdArray<OdGeomConstraint*> aConstr;
  pGeomF->getCommonConstraints(pGeomS, aConstr);

  if ( isConstraintOfThisTypeExist(aConstr, OdEqualLengthConstraint::desc(), &res) )
    return NULL;

  OdSmartPtr<OdEqualLengthConstraint> spEqlCnstr = ::odrxCreateObject(OdEqualLengthConstraint::desc()->name());
  tieNode(spEqlCnstr, actionId);

  res = spEqlCnstr.get();
  tieGeomsAndCnstr(pGeomF, pGeomS, res);

  return res;
}

OdGeomConstraint* OdDbImpAssoc2dConstraintGroup::addSymmetricConstraint(OdConstrainedGeometry* pGeomF,
                                                                        OdConstrainedGeometry* pGeomS,
                                                                        OdConstrainedGeometry* pGeomLine,
                                                                        OdDbObjectId actionId)
{
  OdGeomConstraint* res = NULL;

  if ( !pGeomLine->isKindOf(OdConstrainedLine::desc()) )
    return res;

  if ( pGeomF->isA() != pGeomS->isA() )
  {
    
    if ( !pGeomF->isKindOf(OdConstrainedPoint::desc()) || !pGeomS->isKindOf(OdConstrainedPoint::desc()) )
      return res;
  }

  if ( pGeomF->isKindOf(OdConstrainedSpline::desc()) || pGeomS->isKindOf(OdConstrainedSpline::desc()) )
    return res;

  OdArray<OdGeomConstraint*> aConstr0;
  pGeomF->getCommonConstraints(pGeomS, aConstr0);

  OdArray<OdGeomConstraint*> aConstr1;
  pGeomF->getCommonConstraints(pGeomLine, aConstr1);

  unsigned int i, j, size0 = aConstr0.length(), size1 = aConstr1.length();

  for ( i = 0; i < size0; i++ )
  {
    for ( j = 0; j < size1; j++ )
    {
      if ( aConstr0[i]->nodeId() == aConstr1[j]->nodeId() )
      {
        if ( aConstr0[i]->isA() == OdSymmetricConstraint::desc() )
          return res;
      }
    }
  }

  OdSmartPtr<OdSymmetricConstraint> spSymCnstr = ::odrxCreateObject(OdSymmetricConstraint::desc()->name());
  tieNode(spSymCnstr, actionId);

  res = spSymCnstr.get();
  tieGeomsAndCnstr(pGeomF, pGeomS, res);

  OdConstraintGroupNodeImpl *pGrNodeImpl = OdDbImpAssoc2dConstraintGroup::getConstraintGroupNodeImpl(pGeomLine);
  pGrNodeImpl->addConnection(res ->nodeId());

  pGrNodeImpl = OdDbImpAssoc2dConstraintGroup::getConstraintGroupNodeImpl(res);
  pGrNodeImpl->addConnection(pGeomLine->nodeId());

  return res;
}

OdGeomConstraint* OdDbImpAssoc2dConstraintGroup::addG2SmoothConstraint(OdConstrainedGeometry* pGeomF,
                                                                       OdConstrainedGeometry* pGeomS,
                                                                       OdDbObjectId actionId)
{
  OdGeomConstraint* res = NULL;

  OdArray<OdGeomConstraint*> aConstr;
  pGeomF->getCommonConstraints(pGeomS, aConstr);
  OdGeomConstraint *pCoiCnstr = NULL;
  if ( !isConstraintOfThisTypeExist(aConstr, OdPointCoincidenceConstraint::desc(), &pCoiCnstr) )
  {
    pCoiCnstr = addCoincidentConstraint(pGeomF, pGeomS, actionId);
    if (!pCoiCnstr)
      return res;
	}

  if ( pCoiCnstr->getOwningCompositeConstraint() != NULL )
    return res;

  if ( pGeomF->nodeId() == pGeomS->nodeId() )
    return res;

  OdConstrainedImplicitPoint *pPointF;
  OdConstrainedImplicitPoint *pPointS;

  if ( pGeomF->isKindOf(OdConstrainedImplicitPoint::desc()) )
    pPointF = OdConstrainedImplicitPoint::cast(pGeomF);
  else
    return res;

  if ( pGeomS->isKindOf(OdConstrainedImplicitPoint::desc()) )
    pPointS = OdConstrainedImplicitPoint::cast(pGeomS);
  else
    return res;

  OdConstrainedGeometry *pCnstrGeomF = OdConstrainedGeometry::cast(getGroupNodePtr(pPointF->constrainedCurveId()));
  OdConstrainedGeometry *pCnstrGeomS = OdConstrainedGeometry::cast(getGroupNodePtr(pPointS->constrainedCurveId()));

  //add tangency
  OdHelpParameter* pHlpPrmF = OdDbImpAssoc2dConstraintGroup::createHelpParameter(pPointF, actionId);
  OdHelpParameter* pHlpPrmS = OdDbImpAssoc2dConstraintGroup::createHelpParameter(pPointS, actionId);

  if ( pHlpPrmF == NULL && pHlpPrmS == NULL )
    return res;

  OdGeomConstraint *pTngCnstr = addTangentConstraint(pCnstrGeomF, pCnstrGeomS, pHlpPrmF, pHlpPrmS, actionId);
  if ( pTngCnstr == NULL )
    return res;

  //add EqualCurvature
  pHlpPrmF = OdDbImpAssoc2dConstraintGroup::createHelpParameter(pPointF, actionId);
  pHlpPrmS = OdDbImpAssoc2dConstraintGroup::createHelpParameter(pPointS, actionId);

  OdGeomConstraint *pEqCurvCnstr = addEqualCurvatureConstraint(pCnstrGeomF, pCnstrGeomS, pHlpPrmF, pHlpPrmS, actionId);
  if ( pEqCurvCnstr == NULL )
    return res;

  OdSmartPtr<OdG2SmoothConstraint> spG2Cnstr = ::odrxCreateObject(OdG2SmoothConstraint::desc()->name());
  tieNode(spG2Cnstr, actionId);

  res = spG2Cnstr.get();
  OdCompositeConstraintImpl* nodeImpl = (OdCompositeConstraintImpl*)OdDbImpAssoc2dConstraintGroup::getConstraintGroupNodeImpl(res);
  nodeImpl->addOwnedConstraint(pCoiCnstr);
  nodeImpl->addOwnedConstraint(pTngCnstr);
  nodeImpl->addOwnedConstraint(pEqCurvCnstr);

  return res;
}

OdGeomConstraint* OdDbImpAssoc2dConstraintGroup::addFixConstraint(OdConstrainedGeometry* pGeom, OdDbObjectId actionId)
{
  OdGeomConstraint* res = NULL;

  OdArray<OdGeomConstraint*> aConstr;
  pGeom->getConnectedConstraints(aConstr);

  if ( isConstraintOfThisTypeExist(aConstr, OdFixedConstraint::desc(), &res) )
    return NULL;

  OdSmartPtr<OdFixedConstraint> spFixCnstr = ::odrxCreateObject(OdFixedConstraint::desc()->name());
  tieNode(spFixCnstr, actionId);

  OdConstraintGroupNodeImpl *pGrNodeImpl = OdDbImpAssoc2dConstraintGroup::getConstraintGroupNodeImpl(pGeom);
  pGrNodeImpl->addConnection(spFixCnstr->nodeId());

  res = spFixCnstr.get();
  pGrNodeImpl = OdDbImpAssoc2dConstraintGroup::getConstraintGroupNodeImpl(res);
  pGrNodeImpl->addConnection(pGeom->nodeId());
  
  return res;
}

OdGeomConstraint* OdDbImpAssoc2dConstraintGroup::addEqualCurvatureConstraint(OdConstrainedGeometry* pGeomF,
                                                                             OdConstrainedGeometry* pGeomS,
                                                                             OdHelpParameter* pHelpParamF,
                                                                             OdHelpParameter* pHelpParamS,
                                                                             OdDbObjectId actionId)
{
  OdGeomConstraint* res = NULL;

  if ( pHelpParamF != NULL && pHelpParamS != NULL )
  {
    if ( !pGeomF->isKindOf(OdConstrainedSpline::desc()) && !pGeomS->isKindOf(OdConstrainedSpline::desc()) )
      return res;
  }
  else
  {
    OdConstrainedGeometry *pGeom = pHelpParamF == NULL ? pGeomF : pGeomS;
	  if ( !pGeom->isKindOf(OdConstrainedArc::desc()) && !pGeom->isKindOf(OdConstrainedBoundedEllipse::desc()) &&
        !pGeom->isKindOf(OdConstrainedBoundedLine::desc()))
        return res;
	}

  if ( pGeomF->nodeId() == pGeomS->nodeId() )
    return res;

  OdSmartPtr<OdEqualCurvatureConstraint> spTanCnstr = ::odrxCreateObject(OdEqualCurvatureConstraint::desc()->name());
  tieNode(spTanCnstr, actionId);

  res = spTanCnstr.get();
  tieGeomsAndCnstrWithHeplPrm(pGeomF, pGeomS, res, pHelpParamF, pHelpParamS);

  return res;
}

OdHelpParameter* OdDbImpAssoc2dConstraintGroup::createHelpParameter(OdConstrainedImplicitPoint *pPoint, OdDbObjectId actionId)
{
  if ( pPoint->pointType() != OdConstrainedImplicitPoint::kDefineImplicit )
    return NULL;

  OdConstraintGroupNode *node = getGroupNodePtr(pPoint->constrainedCurveId());
  if ( node == NULL )
    return NULL;

  if ( !node->isKindOf(OdConstrainedSpline::desc()) )
    return NULL;

  OdConstrainedSpline *spline = OdConstrainedSpline::cast(node);
  OdGeNurbCurve3d nurb = spline->nurbSpline();

  double val = pPoint->pointIndex() == 0 ? nurb.startParam() : nurb.endParam();
  
  OdSmartPtr<OdHelpParameter> spHelpParam = ::odrxCreateObject(OdHelpParameter::desc()->name());
  spHelpParam->setValue(val);

  tieNode(spHelpParam, actionId);
  return spHelpParam.get();
}

OdGeomConstraint* OdDbImpAssoc2dConstraintGroup::addVertHorzConstraint(OdConstrainedGeometry* pGeomF,
                                                                       OdDbObjectId actionId,
                                                                       const OdRxClass *desc,
                                                                       const OdGeVector3d dir,
                                                                       const OdString name)
{
  OdGeomConstraint* res = NULL;

  if ( !pGeomF->isKindOf(OdConstrainedLine::desc()) )
    return res;
  
  OdArray<OdGeomConstraint*> aConstr;
  pGeomF->getConnectedConstraints(aConstr);

  if ( isConstraintOfThisTypeExist(aConstr, desc, &res) )
    return NULL;

  OdConstrainedDatumLine *pDatumLine = NULL;
  if ( !findDatumLine(&pDatumLine, dir) )
  {
    OdSmartPtr<OdConstrainedDatumLine> spDatumLine =
        ::odrxCreateObject(OdConstrainedDatumLine::desc()->name());

    tieNode(spDatumLine, actionId);
    pDatumLine = spDatumLine.get();

    OdConstrainedDatumLineImpl *pDatumImpl = (OdConstrainedDatumLineImpl*)OdDbImpAssoc2dConstraintGroup::getConstraintGroupNodeImpl(pDatumLine);

    pDatumImpl->init(OdGePoint3d::kOrigin, dir);
  }

  OdSmartPtr<OdParallelConstraint> spVHCnstr = ::odrxCreateObject(name);
  tieNode(spVHCnstr, actionId);

  OdConstraintGroupNodeImpl *pGrNodeImpl = OdDbImpAssoc2dConstraintGroup::getConstraintGroupNodeImpl(pGeomF);
  pGrNodeImpl->addConnection(spVHCnstr->nodeId());

  OdParallelConstraintImpl *pConstrImpl = (OdParallelConstraintImpl*)OdDbImpAssoc2dConstraintGroup::getConstraintGroupNodeImpl(spVHCnstr);
  pConstrImpl->addConnection(pGrNodeImpl->nodeId());
  pConstrImpl->setDatumLineIdx(pDatumLine->nodeId());

  res = spVHCnstr.get();
  return res;
}

OdGeomConstraint* OdDbImpAssoc2dConstraintGroup::addVertHorzConstraint(OdConstrainedGeometry* pGeomF,
                                                                       OdConstrainedGeometry* pGeomS,
                                                                       OdDbObjectId actionId,
                                                                       const OdRxClass *desc,
                                                                       const OdGeVector3d dir,
                                                                       const OdString name)
{
  OdGeomConstraint* res = NULL;

  if ( !pGeomF->isKindOf(OdConstrainedPoint::desc()) || !pGeomS->isKindOf(OdConstrainedPoint::desc()) )
    return res;

  OdArray<OdGeomConstraint*> aConstr;
  pGeomF->getCommonConstraints(pGeomS, aConstr);

  if ( isConstraintOfThisTypeExist(aConstr, desc, &res) )
    return NULL;

  OdConstrainedDatumLine *pDatumLine = NULL;
  if ( !findDatumLine(&pDatumLine, dir) )
  {
    OdSmartPtr<OdConstrainedDatumLine> spDatumLine = ::odrxCreateObject(OdConstrainedDatumLine::desc()->name());
    tieNode(spDatumLine, actionId);
    pDatumLine = spDatumLine.get();

    OdConstrainedDatumLineImpl *pDatumImpl = (OdConstrainedDatumLineImpl*)OdDbImpAssoc2dConstraintGroup::getConstraintGroupNodeImpl(pDatumLine);
    pDatumImpl->init(OdGePoint3d::kOrigin, dir);
  }

  OdConstrainedLine *p2PointsLine = NULL;
  if ( pGeomF->isKindOf(OdConstrainedImplicitPoint::desc()) && pGeomS->isKindOf(OdConstrainedImplicitPoint::desc()) )
  {
    OdConstrainedImplicitPoint *pImplPointF = OdConstrainedImplicitPoint::cast(pGeomF);
    OdConstrainedImplicitPoint *pImplPointS = OdConstrainedImplicitPoint::cast(pGeomS);
  
    if ( pImplPointF->constrainedCurveId() == pImplPointS->constrainedCurveId() )
    {
      OdConstraintGroupNode *pCurve = getGroupNodePtr(pImplPointF->constrainedCurveId());
      ODA_ASSERT_ONCE(pCurve);
      if (pCurve && pCurve->isKindOf(OdConstrainedBoundedLine::desc()))
        p2PointsLine = OdConstrainedBoundedLine::cast(pCurve);
    }
  }

  if ( p2PointsLine == NULL )
  {
    OdSmartPtr<OdConstrained2PointsConstructionLine> sp2PointsLine =
      ::odrxCreateObject(OdConstrained2PointsConstructionLine::desc()->name());

    tieNode(sp2PointsLine, actionId);

    OdSmartPtr<OdPointCurveConstraint> spPointCurveCnstrF =
      ::odrxCreateObject(OdPointCurveConstraint::desc()->name());

    tieNode(spPointCurveCnstrF, actionId);

    tieGeomsAndCnstr(pGeomF, sp2PointsLine, spPointCurveCnstrF);

    OdSmartPtr<OdPointCurveConstraint> spPointCurveCnstrS =
      ::odrxCreateObject(OdPointCurveConstraint::desc()->name());

    tieNode(spPointCurveCnstrS, actionId);

    tieGeomsAndCnstr(pGeomS, sp2PointsLine, spPointCurveCnstrS);
  }

  OdSmartPtr<OdParallelConstraint> spVHCnstr = ::odrxCreateObject(name);
  tieNode(spVHCnstr, actionId);

  res = spVHCnstr.get();

  OdParallelConstraintImpl *pConstrImpl = (OdParallelConstraintImpl*)OdDbImpAssoc2dConstraintGroup::getConstraintGroupNodeImpl(res);
  pConstrImpl->setDatumLineIdx(pDatumLine->nodeId());

  tieGeomsAndCnstr(pGeomF, pGeomS, res);

  return res;
}

void OdDbImpAssoc2dConstraintGroup::postReadWork()
{
  for (OdConstraintGroupNodeMap::const_iterator pNode = m_mapNodes.begin(); pNode != m_mapNodes.end(); ++pNode)
  {
    if ( pNode->second->isKindOf(OdDistanceConstraint::desc()) )
    {
      OdDistanceConstraintImpl *pDstCnstrImpl = (OdDistanceConstraintImpl*)getConstraintGroupNodeImpl(pNode->second);
      pDstCnstrImpl->fillCurrentStateObj();
    }
  }
}

OdConstrainedGeometry* OdDbImpAssoc2dConstraintGroup::addConstrainedGeometry(OdDbAssocGeomDependencyPtr pGeomDep)
{
  OdConstrainedGeometry *res = NULL;
  if ( pGeomDep->subentType() == OdDb::kVertexSubentType )
  {

    res = addPoint(pGeomDep->objectId());
  }
  else if ( pGeomDep->subentType() == OdDb::kEdgeSubentType )
  {
    OdArray<OdGeCurve3d*> arrCurve;
    if ( pGeomDep->getEdgeSubentityGeometry(arrCurve) != eOk )
      return NULL;

    if ( arrCurve[0]->isKindOf(OdGe::kCircArc3d) )
      if ( arrCurve[0]->isClosed() )
        res = addCircle(pGeomDep->objectId());
      else
        res = addArc(pGeomDep->objectId());
    else if ( arrCurve[0]->isKindOf(OdGe::kLineSeg3d) )
      res = addBoundedLine(pGeomDep->objectId());
    else if ( arrCurve[0]->isKindOf(OdGe::kRay3d) )
      res = addRay(pGeomDep->objectId());
    else if ( arrCurve[0]->isKindOf(OdGe::kLine3d) )
      res = addXLine(pGeomDep->objectId());
    else if ( arrCurve[0]->isKindOf(OdGe::kEllipArc3d) )
      if ( arrCurve[0]->isClosed() )
        res = addEllipse(pGeomDep->objectId());
      else
        res = addEllipseArc(pGeomDep->objectId());
    else if ( arrCurve[0]->isKindOf(OdGe::kNurbCurve3d) )
        res = addSpline(pGeomDep->objectId(), *(OdGeNurbCurve3d*)arrCurve[0]);

    delete arrCurve[0];
    arrCurve.clear();
  }

  OdConstrainedRigidSet* pRigidSet = getRigidSet(pGeomDep->objectId(), true);
  if (pRigidSet && res)
  {
    addGeometryToRigidSet(*pRigidSet, *res);
    if (OdConstrainedCurve* pCurve = OdConstrainedCurve::cast(res).get())
    {
      OdArray<OdConstrainedImplicitPoint*> aImplicitPts;
      pCurve->getConstrainedImplicitPoints(aImplicitPts);
      for (unsigned ptIdx = 0; ptIdx < aImplicitPts.size(); ++ptIdx)
      {
        if (OdConstrainedImplicitPoint* implicitPt = aImplicitPts[ptIdx])
          addGeometryToRigidSet(*pRigidSet, *implicitPt);
      }
    }
  }

  return res;
}

OdConstrainedGeometry* OdDbImpAssoc2dConstraintGroup::addPoint(OdDbObjectId geomDepId)
{
  OdSmartPtr<OdConstrainedPoint> pPt = OdConstrainedPoint::createObject();
  pPt->init(geomDepId);
  tieNode(pPt, OdDbObjectId::kNull);
  return pPt;
}

OdConstrainedGeometry* OdDbImpAssoc2dConstraintGroup::addCircle(OdDbObjectId geomDepId)
{
  OdSmartPtr<OdConstrainedCircle> pCircle = OdConstrainedCircle::createObject();
  pCircle->init(geomDepId);
  tieNode(pCircle, OdDbObjectId::kNull);

  addImplicitPoint(pCircle, OdConstrainedImplicitPoint::kCenterImplicit, -1);

  return pCircle;
}

OdConstrainedGeometry* OdDbImpAssoc2dConstraintGroup::addArc(OdDbObjectId geomDepId)
{
  OdSmartPtr<OdConstrainedArc> pArc = OdConstrainedArc::createObject();
  pArc->init(geomDepId);
  tieNode(pArc, OdDbObjectId::kNull);

  addImplicitPoint(pArc, OdConstrainedImplicitPoint::kCenterImplicit, -1);

  addImplicitPoint(pArc, OdConstrainedImplicitPoint::kStartImplicit, -1);

  addImplicitPoint(pArc, OdConstrainedImplicitPoint::kEndImplicit, -1);

  return pArc;
}

OdConstrainedGeometry* OdDbImpAssoc2dConstraintGroup::addBoundedLine(OdDbObjectId geomDepId)
{
  OdSmartPtr<OdConstrainedBoundedLine> pLine = OdConstrainedBoundedLine::createObject();
  pLine->init(geomDepId, false);
  tieNode(pLine, OdDbObjectId::kNull);

  addImplicitPoint(pLine, OdConstrainedImplicitPoint::kStartImplicit, -1);

  addImplicitPoint(pLine, OdConstrainedImplicitPoint::kEndImplicit, -1);

  return pLine;
}

OdConstrainedGeometry* OdDbImpAssoc2dConstraintGroup::addRay(OdDbObjectId geomDepId)
{
  OdSmartPtr<OdConstrainedBoundedLine> pLine = OdConstrainedBoundedLine::createObject();
  pLine->init(geomDepId, true);
  tieNode(pLine, OdDbObjectId::kNull);

  addImplicitPoint(pLine, OdConstrainedImplicitPoint::kStartImplicit, -1);

  return pLine;
}

OdConstrainedGeometry* OdDbImpAssoc2dConstraintGroup::addXLine(OdDbObjectId geomDepId)
{
  OdSmartPtr<OdConstrainedLine> pLine = OdConstrainedLine::createObject();
  pLine->init(geomDepId);
  tieNode(pLine, OdDbObjectId::kNull);

  return pLine;
}

OdConstrainedGeometry* OdDbImpAssoc2dConstraintGroup::addEllipse(OdDbObjectId geomDepId)
{
  OdSmartPtr<OdConstrainedEllipse> pEllipse = OdConstrainedEllipse::createObject();
  pEllipse->init(geomDepId);
  tieNode(pEllipse, OdDbObjectId::kNull);

  addImplicitPoint(pEllipse, OdConstrainedImplicitPoint::kCenterImplicit, -1);

  return pEllipse;
}

OdConstrainedGeometry* OdDbImpAssoc2dConstraintGroup::addEllipseArc(OdDbObjectId geomDepId)
{
  OdSmartPtr<OdConstrainedBoundedEllipse> pArc = OdConstrainedBoundedEllipse::createObject();
  pArc->init(geomDepId);
  tieNode(pArc, OdDbObjectId::kNull);

  addImplicitPoint(pArc, OdConstrainedImplicitPoint::kCenterImplicit, -1);

  addImplicitPoint(pArc, OdConstrainedImplicitPoint::kStartImplicit, -1);

  addImplicitPoint(pArc, OdConstrainedImplicitPoint::kEndImplicit, -1);

  return pArc;
}

OdConstrainedGeometry* OdDbImpAssoc2dConstraintGroup::addSpline(OdDbObjectId geomDepId, const OdGeNurbCurve3d& spline)
{
  OdSmartPtr<OdConstrainedSpline> pSpline = OdConstrainedSpline::createObject();
  pSpline->init(geomDepId, spline);
  tieNode(pSpline, OdDbObjectId::kNull);

  int ptNum = spline.numControlPoints();
  for ( int i = 0; i < ptNum; i++ )
    addImplicitPoint(pSpline, OdConstrainedImplicitPoint::kDefineImplicit, i);

  return pSpline;
}

static OdDbObjectId getDependentOnObject(OdDbObjectId depId)
{
  OdDbAssocDependencyPtr pDep = OdDbAssocDependency::cast(depId.openObject());
  return !pDep.isNull() ? pDep->dependentOnObject() : OdDbObjectId::kNull;
}

static OdConstrainedRigidSet* findRigidSetIn(OdDbImpAssoc2dConstraintGroup& pGroupImp, OdDbObjectId brefId)
{
  OdArray<OdConstrainedGeometry*> geometries;
  pGroupImp.getConstrainedGeometries(geometries);
  for (unsigned geomIdx = 0; geomIdx < geometries.size(); ++geomIdx)
  {
    if (OdConstrainedGeometry* pGeometry = geometries[geomIdx])
    {
      OdConstrainedRigidSet* pRigidSet = pGeometry->getOwningRigidSet();
      if (pRigidSet && getDependentOnObject(pGeometry->geomDependencyId()) == brefId)
        return pRigidSet;
    }
  }
  return NULL;
}

// creates rigid set on demand, if there is already one - return it
OdConstrainedRigidSet* OdDbImpAssoc2dConstraintGroup::getRigidSet(OdDbObjectId geomDepId, bool createIfNotExists)
{
  const OdDbObjectId brefId = getDependentOnObject(geomDepId);
  if (!brefId || !brefId.objectClass()->isDerivedFrom(OdDbBlockReference::desc()))
    return NULL;

  OdConstrainedRigidSet* pSearchedRigidSet = findRigidSetIn(*this, brefId);
  if (!pSearchedRigidSet && createIfNotExists)
  {
    OdSmartPtr<OdConstrainedRigidSet> pNewRigidSet = OdConstrainedRigidSet::createObject();
    pNewRigidSet->init(geomDepId);
    tieNode(pNewRigidSet, OdDbObjectId::kNull);
    pSearchedRigidSet = pNewRigidSet.get();
  }
  return pSearchedRigidSet;
}

bool OdDbImpAssoc2dConstraintGroup::addGeometryToRigidSet(OdConstrainedRigidSet& rigidSet, OdConstrainedGeometry& geom)
{
  OdConstrainedGeometryImpl* pResGeomImpl = static_cast<OdConstrainedGeometryImpl*>(geom.m_pImpl);
  OdConstrainedRigidSetImpl* pRigidSetImpl = static_cast<OdConstrainedRigidSetImpl*>(rigidSet.m_pImpl);

  return pRigidSetImpl->addConstrainedGeom(geom.nodeId()) == eOk &&
         pResGeomImpl->setOwningRigidSet(rigidSet.nodeId()) == eOk;
}

OdConstrainedImplicitPoint* OdDbImpAssoc2dConstraintGroup::addImplicitPoint(OdConstrainedCurve *pCurve, OdConstrainedImplicitPoint::ImplicitPointType type, int index)
{
  OdSmartPtr<OdConstrainedImplicitPoint> pPoint = OdConstrainedImplicitPoint::createObject();
  pPoint->init(pCurve->nodeId(), type, index);
  tieNode(pPoint, pCurve->owningConstraintGroupId());

  if ( type == OdConstrainedImplicitPoint::kDefineImplicit )
    return pPoint;

  OdSmartPtr<OdGeomConstraint> pCnstr;
  if ( type == OdConstrainedImplicitPoint::kCenterImplicit )
    pCnstr = OdCenterPointConstraint::createObject();
  else if ( type == OdConstrainedImplicitPoint::kMidImplicit)
    pCnstr = OdMidPointConstraint::createObject();
  else
    pCnstr = OdPointCurveConstraint::createObject();

  tieNode(pCnstr, pCurve->owningConstraintGroupId());

  if (type == OdConstrainedImplicitPoint::kMidImplicit) {
    tieGeomsAndCnstr(pPoint, pCurve, pCnstr);
    //additional p-c constraint to keep point on line
    pCnstr = OdPointCurveConstraint::createObject();
    tieNode(pCnstr, pCurve->owningConstraintGroupId());
  }

  tieGeomsAndCnstr(pCurve, pPoint, pCnstr);

  return pPoint;
}

enum ConstraintDependencyType {
  eUnknownDependency,
  eGeomDependency,
  eValueDependency,
  eDimDependency
};

static ConstraintDependencyType getDependencyType(const OdDbAssocDependency* pDep) {
  if (!pDep) {
    return eUnknownDependency;
  }
  else if (pDep->isA() == OdDbAssocGeomDependency::desc()) {
    return eGeomDependency;
  }
  else if (pDep->isA() == OdDbAssocValueDependency::desc()) {
    return eValueDependency;
  }
  else if (pDep->dependencyBody().objectClass()->isDerivedFrom(OdDbAssocDimDependencyBodyBase::desc())) {
    return eDimDependency;
  }
  return eUnknownDependency;
}

void OdDbImpAssoc2dConstraintGroup::updateDependentOnObjects(OdDbAssocAction *pAction)
{
  OdDbAssoc2dConstraintGroupPtr p2dConstraintGroup(pAction);
  OdDbObjectIdArray ids; pAction->getDependencies(false, true, ids);

  OdArray<OdDbAssocDependencyPtr> apDepsToUpdate;
  for (unsigned int i = 0; i < ids.size(); ++i) {
    OdDbAssocDependencyPtr pDep = OdDbAssocDependency::cast(ids[i].openObject());
    if (pDep.isNull())
      continue;

    bool updateDependency = false;
    const ConstraintDependencyType depType = getDependencyType(pDep.get());
    switch (depType) {
      case eGeomDependency: {
        OdConstrainedGeometry *pConsGeom;
        OdDbAssocGeomDependency* pGeomDep = static_cast<OdDbAssocGeomDependency*>(pDep.get());
        updateDependency = p2dConstraintGroup->getConstrainedGeometry(pGeomDep, pConsGeom) == eOk &&
                           pConsGeom && pConsGeom->isModified();
        break;
      }
      case eDimDependency: {
        updateDependency = true;
        break;
      }
      default: {
        updateDependency = pDep->status() != kIsUpToDateAssocStatus;
      }
    }

    if (updateDependency)
      apDepsToUpdate.push_back(pDep);
  }

  OdGeMatrix3d planeToWorld = OdGeMatrix3d::planeToWorld(p2dConstraintGroup->getWorkPlane());
  for (unsigned int i = 0; i < apDepsToUpdate.size(); i++) {
    OdDbAssocDependencyPtr dep = apDepsToUpdate[i];
    {
      dep->updateDependentOnObject();

      OdDbAssocGeomDependencyPtr geoDep = OdDbAssocGeomDependency::cast(dep);
      if (geoDep.get()) {
        OdDbObjectPtr dependentOnObject = geoDep->dependentOnObject().openObject(OdDb::kForWrite);
        OdConstrainedGeometry *proxyGeom;
        if (dependentOnObject.get() && p2dConstraintGroup->getConstrainedGeometry(geoDep, proxyGeom) == eOk) {
          OdConstrainedGeometryImpl *proxyGeomImpl =
            (OdConstrainedGeometryImpl*)OdDbImpAssoc2dConstraintGroup::getConstraintGroupNodeImpl(proxyGeom);
          proxyGeomImpl->updateDbObject(dependentOnObject, geoDep, planeToWorld);
        }
      }
    }
  }
}

bool OdDbImpAssoc2dConstraintGroup::isRelevantDependencyChange(const OdDbAssocDependency* pDep) const {
  const ConstraintDependencyType depType = getDependencyType(pDep);
  switch (depType) {
    case eGeomDependency: {
      const OdDbAssocGeomDependency* pGeomDep = static_cast<const OdDbAssocGeomDependency*>(pDep);
      OdConstrainedGeometry *pConsGeom;
      const_cast<OdDbImpAssoc2dConstraintGroup*>(this)->getConstrainedGeometry(pGeomDep, pConsGeom);
      if (pConsGeom) {
        OdSmartPtr<OdConstrainedGeometry> sourceGeometry = pConsGeom->isA()->create();
        if (sourceGeometry->setDataFromGeomDependency(pDep->objectId(), OdGeMatrix3d::worldToPlane(getWorkPlane())) == eOk)
          return *pConsGeom != *(sourceGeometry.get());
      }
      break;
    }
    case eValueDependency: {
      const OdDbAssocValueDependency* pValDep = static_cast<const OdDbAssocValueDependency*>(pDep);
      OdDbEvalVariantPtr pCached = OdDbEvalVariant::createObject(),
                         pActual = OdDbEvalVariant::createObject();

      if (pValDep->getDependentOnObjectValue(pActual) == eOk &&
          pValDep->getDependentOnObjectCachedValue(pCached) == eOk &&
          pActual.get() && pCached.get()) {
        return *pActual != *pCached;
      }
      break;
    }
    case eDimDependency: {
      return false;
    }
    default: break;
  }
  return true;
}

void OdDbImpAssoc2dConstraintGroup::evaluateDependency(OdDbAssocDependency* pDependency) {
  if (pDependency && pDependency->isA() == OdDbAssocValueDependency::desc()) {
    const bool prevIsDelegating = pDependency->isDelegatingToOwningAction();
    pDependency->setIsDelegatingToOwningAction(false);
    pDependency->evaluate();
    pDependency->setIsDelegatingToOwningAction(prevIsDelegating);
  }
  OdDbImpAssocAction::evaluateDependency(pDependency);
}

void OdDbImpAssoc2dConstraintGroup::composeForLoad( OdDbObject* pObj, 
                                         OdDb::SaveType format,
                                         OdDb::DwgVersion version,
                                         OdDbAuditInfo* pAuditInfo )
{
  OdDbImpAssocAction::composeForLoad(pObj, format, version, pAuditInfo );
  if (m_mapNodes.find(UINT_MAX) != m_mapNodes.end())
  {
    ODA_FAIL_ONCE(); // nodeId were duplicated
    OdDbHostAppServices* pHostApp = pObj->database()->appServices();
    pHostApp->warning(pHostApp->formatMessage(sidRecvObjInvalidData, odDbGetObjectName(pObj).c_str()));
    OdDbAssocAction* pAction = (OdDbAssocAction*)pObj;
    pAction->removeAllDependencies(false);
    m_mapNodes.clear();
    OdDbAssocNetworkPtr pOwner = OdDbAssocNetwork::cast(pAction->owningNetwork().openObject(OdDb::kForWrite));
    if (!pOwner.isNull())
      pOwner->removeAction(pObj->objectId(), false);
    return;
  }
  postReadWork();
}

OdResult OdDbImpAssoc2dConstraintGroup::removeNode(OdConstraintGroupNode* pNode)
{
  OdConstraintGroupNodeMap::iterator it = m_mapNodes.find(pNode->nodeId());
  if (it != m_mapNodes.end())
  {
    m_mapNodes.erase(it);
    return eOk;
  }
  else
    return eInvalidKey;
}

void OdDbImpAssoc2dConstraintGroup::audit(OdDbAssoc2dConstraintGroup* pGroup, OdDbAuditInfo* pAuditInfo)
{
  int numErrorsFound = pAuditInfo->numErrors();
  for (OdConstraintGroupNodeMap::iterator nodeIt = m_mapNodes.begin(); nodeIt != m_mapNodes.end(); ++nodeIt)
  {
    if (OdConstraintGroupNode* pNode = nodeIt->second)
      pNode->m_pImpl->audit(pNode, pAuditInfo);
  }

  if (numErrorsFound != pAuditInfo->numErrors())
  {
    OdDbHostAppServices* pHostApp = pGroup->database()->appServices();
    pAuditInfo->printError(pGroup,
                           pHostApp->formatMessage(sidInvalidConstraintGeometry),
                           pHostApp->formatMessage(sidVarValidInvalid),
                           pHostApp->formatMessage(sidVarDefRepair));
  }
}
