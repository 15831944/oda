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

#include "DbAssocNetCloneCtx.h"
#include "DbAssocNetworkDeepCloneReactor.h"
#include "DbAssocNetworkLongTransactionReactor.h"

#include "DbAssoc2dConstraintGroup.h"
#include "DbAssocNetwork.h"
#include "DbAssocValueDependency.h"
#include "DbImpAssocDependency.h"
#include "OdExpression.h"
#include "OdUnitsFormatter.h"
#include "DbAssocVariable.h"
#include "DbImpAssoc2dConstraintGroup.h"
#include "DbDictionary.h"
#include "OdCharConverter.h"
#include "OdDbAssocPersSubentIdPE.h"
#include "OdImplicitConstr.h"
#define STL_USING_ALGORITHM
#include "OdaSTL.h"
#include "DbHostAppServices.h"
#include "DbImpAssocVariable.h"
#include "DbDictionaryVar.h"
#include "OdHashSet.h"
#include "DbPlaceHolder.h"
#include "DbBlockTableRecord.h"
#include "DbAssocManager.h"
#include "DebugStuff.h"
#include "DbDynBlockReference.h"
#include "DynamicBlocks/DbBlockParameter.h"


ODRX_CONS_DEFINE_MEMBERS(OdDbAssocNetCloneCtx, OdDbObjectReactor, RXIMPL_CONSTR);

static bool isInLongTransaction(OdDbIdMapping& idMap)
{
  if (OdApLongTransactionManager* pTM = odapLongTransactionManager())
  {
    const OdDbObjectId origTransId = pTM->currentLongTransactionFor(idMap.origDb());
    const OdDbObjectId destTransId = pTM->currentLongTransactionFor(idMap.destDb());
    if (origTransId || destTransId)
      return true;
  }
  return false;
}

void OdDbAssocNetCloneCtx::attach(OdDbIdMapping& idMap) {
  ODA_ASSERT(OdDbAssocNetCloneCtx::internal_fromDb(idMap.destDb()).isNull());
  OdSmartPtr<OdDbAssocNetCloneCtx> res = OdRxObjectImpl<OdDbAssocNetCloneCtx>::createObject();
  res->idMap = &idMap;
  idMap.destDb()->OdDbObject::addReactor( res );
}

bool OdDbAssocNetCloneCtx::detach(OdDbDatabase* fromDestDb) {
  OdDbAssocNetCloneCtxPtr ctx = OdDbAssocNetCloneCtx::internal_fromDb(fromDestDb);
  if(ctx.get()) {
    fromDestDb->OdDbObject::removeReactor(ctx);
    return true;
  }
  return false;
}


class DimConstraintData : public OdRxObjectImpl<OdRxObject>
{
public:
  ODRX_DECLARE_MEMBERS(DimConstraintData);
  virtual ~DimConstraintData() {};
  virtual bool init(OdGeomConstraint* pConstraint) = 0;
  virtual bool fill(OdDbAssoc2dConstraintGroup * pDestGroup) = 0;
protected:
  bool init(OdExplicitConstraint* pConstraint, unsigned int reqGeomsLength)
  {
    OdArray<OdConstrainedGeometry*> apConsGeoms;
    pConstraint->getConnectedGeometries(apConsGeoms);
    unsigned int consGeomsLength = apConsGeoms.length();
    if (consGeomsLength != reqGeomsLength)
      return false;
    ODA_ASSERT(consGeomsLength <= 3);    
    m_valueDependencyId = pConstraint->valueDependencyId();
    m_dimDependencyId   = pConstraint->dimDependencyId();
    for (unsigned int i=0; i<consGeomsLength; i++)
    {
      OdResult res = apConsGeoms[i]->getFullSubentPaths(m_aSubentPath[i]);
      if (res != eOk)
        return false;
    }
    return true;
  }
  bool getGeometries(OdDbAssoc2dConstraintGroupPtr pGroup, OdConstrainedGeometry * apConsGeom[3], const unsigned int reqGeomsLength)
  {
    ODA_ASSERT(reqGeomsLength <= 3);
    bool bCreateArcLineMid = true;
    for (unsigned int i = 0; i<reqGeomsLength; i++)
    {
      apConsGeom[i] = 0;
      if (eOk != pGroup->getConstrainedGeometry(m_aSubentPath[i][0], apConsGeom[i], bCreateArcLineMid))
        return false;
    }
    return true;
  }
  OdDbObjectId m_valueDependencyId;
  OdDbObjectId m_dimDependencyId;
private:
  OdDbFullSubentPathArray m_aSubentPath[3];
};

ODRX_NO_CONS_DEFINE_MEMBERS(DimConstraintData, OdRxObject);

typedef OdSmartPtr<DimConstraintData> DimConstraintDataPtr;

class DistanceConstraintData : public DimConstraintData
{
public:
  ODRX_DECLARE_MEMBERS(DistanceConstraintData);
  ~DistanceConstraintData() {};
  bool init(OdGeomConstraint* pConstraint)
  {
    OdDistanceConstraint * pDC = (OdDistanceConstraint *) pConstraint;
    if (!DimConstraintData::init(pDC, 2))
      return false;
    m_directionType = pDC->directionType();
    m_fixedDirection = pDC->direction();
    return true;
  }
  bool fill(OdDbAssoc2dConstraintGroup * pDestGroup)
  {
    OdConstrainedGeometry * apConsGeom[2];
    if (!getGeometries(pDestGroup, apConsGeom, 2))
      return false;
    OdGeVector3d * pFixedDirection = (m_directionType == OdDistanceConstraint::kFixedDirection) ?
                                     &m_fixedDirection : NULL;
    OdConstrainedLine* pDirectionLine = NULL;
    OdDistanceConstraint * pConstr = 0;
    pDestGroup->addDistanceConstraint(apConsGeom[0], apConsGeom[1], m_directionType,
                                      m_valueDependencyId, m_dimDependencyId,
                                      pFixedDirection, pDirectionLine, &pConstr);
    return true;
  }
private:
  OdDistanceConstraint::DirectionType m_directionType;
  OdGeVector3d m_fixedDirection;
};

ODRX_CONS_DEFINE_MEMBERS(DistanceConstraintData, DimConstraintData, RXIMPL_CONSTR);

class AngleConstraintData : public DimConstraintData
{
public:
  ODRX_DECLARE_MEMBERS(AngleConstraintData);
  ~AngleConstraintData() {}
  bool init(OdGeomConstraint* pConstraint)
  {
    OdAngleConstraint * pAC = (OdAngleConstraint *) pConstraint;
    if (!DimConstraintData::init(pAC, 2))
      return false;
    m_sectorType = pAC->sectorType();
    return true;
  }
  bool fill(OdDbAssoc2dConstraintGroup * pDestGroup)
  {
    OdConstrainedGeometry * apConsGeom[2];
    if (!getGeometries(pDestGroup, apConsGeom, 2))
      return false;
    OdAngleConstraint * pConstr = 0;
    pDestGroup->addAngleConstraint((OdConstrainedLine*) apConsGeom[0], (OdConstrainedLine*) apConsGeom[1],
                                   m_sectorType,
                                   m_valueDependencyId, m_dimDependencyId,
                                   &pConstr);
    return true;
  }
private:
  OdAngleConstraint::SectorType m_sectorType;
};

ODRX_CONS_DEFINE_MEMBERS(AngleConstraintData, DimConstraintData, RXIMPL_CONSTR);

class ThreePointAngleConstraintData : public DimConstraintData
{
public:
  ODRX_DECLARE_MEMBERS(ThreePointAngleConstraintData);
  ~ThreePointAngleConstraintData() {}
  bool init(OdGeomConstraint* pConstraint)
  {
    Od3PointAngleConstraint * pAC = (Od3PointAngleConstraint *) pConstraint;
    if (!DimConstraintData::init(pAC, 3))
      return false;
    return true;
  }
  bool fill(OdDbAssoc2dConstraintGroup * pDestGroup)
  {
    OdConstrainedGeometry * apConsGeom[3];
    if (!getGeometries(pDestGroup, apConsGeom, 3))
      return false;
    Od3PointAngleConstraint * pConstr = 0;  
    pDestGroup->add3PointAngleConstraint((OdConstrainedPoint *) apConsGeom[0], (OdConstrainedPoint *) apConsGeom[1], (OdConstrainedPoint *) apConsGeom[2],
                                         m_sectorType, m_valueDependencyId, m_dimDependencyId,
                                         &pConstr);
    return true;
  }
private:
  OdAngleConstraint::SectorType m_sectorType;
};

ODRX_CONS_DEFINE_MEMBERS(ThreePointAngleConstraintData, DimConstraintData, RXIMPL_CONSTR);

class RadiusDiameterConstraintData : public DimConstraintData
{
public:
  ODRX_DECLARE_MEMBERS(RadiusDiameterConstraintData);
  ~RadiusDiameterConstraintData() {}
  bool init(OdGeomConstraint* pConstraint)
  {
    OdRadiusDiameterConstraint * pRC = (OdRadiusDiameterConstraint *) pConstraint;
    if (!DimConstraintData::init(pRC, 1))
      return false;
    m_rdType = pRC->constrType();
    return true;
  }
  bool fill(OdDbAssoc2dConstraintGroup * pDestGroup)
  {
    OdConstrainedGeometry * apConsGeom[1];
    if (!getGeometries(pDestGroup, apConsGeom, 1))
      return false;
    OdRadiusDiameterConstraint * pConstr = 0;
    pDestGroup->addRadiusDiameterConstraint(apConsGeom[0], m_rdType,
                                            m_valueDependencyId, m_dimDependencyId,
                                            &pConstr);
    return true;
  }
private:
  OdRadiusDiameterConstraint::RadiusDiameterConstrType m_rdType;
};

ODRX_CONS_DEFINE_MEMBERS(RadiusDiameterConstraintData, DimConstraintData, RXIMPL_CONSTR);

struct ConstraintCloneData
{
  ConstraintCloneData(OdGeomConstraint::GeomConstraintType type,
                      const OdDbFullSubentPathArray& aPaths,
                      const OdGeMatrix3d& ucs)
    : m_type(type), m_aPaths(aPaths), m_ucs(ucs)
  {}

  OdGeomConstraint::GeomConstraintType m_type;
  OdDbFullSubentPathArray m_aPaths;
  OdGeMatrix3d m_ucs;
};

static OdGeMatrix3d getConstraintUCS(const OdGeomConstraint& constr, const OdDbAssoc2dConstraintGroup& group)
{
  if (!constr.isKindOf(OdHorizontalConstraint::desc()) && !constr.isKindOf(OdVerticalConstraint::desc()))
    return OdGeMatrix3d::kIdentity;

  const OdParallelConstraint& pParallel = static_cast<const OdParallelConstraint&>(constr);
  const OdConstraintGroupNodeId datumLineId = pParallel.getDatumLineId();
  const OdConstrainedDatumLine* pDatumLine =
      static_cast<const OdConstrainedDatumLine*>(group.getGroupNodePtr(datumLineId));

  if (!pDatumLine)
    return OdGeMatrix3d::kIdentity;

  const OdGePlane groupPlane = group.getWorkPlane();
  const OdGeMatrix3d gcs2wcs = OdGeMatrix3d::planeToWorld(groupPlane);
  const OdGeVector3d constrDir = pDatumLine->direction().transformBy(gcs2wcs).normalize();
  if (constr.isKindOf(OdHorizontalConstraint::desc()))
  {
    const OdGeVector3d zAxis = groupPlane.normal();
    const OdGeVector3d xAxis = constrDir;
    const OdGeVector3d yAxis = zAxis.crossProduct(xAxis).normalize();
    return OdGeMatrix3d().setCoordSystem(groupPlane.pointOnPlane(), xAxis, yAxis, zAxis);
  }
  else if (constr.isKindOf(OdVerticalConstraint::desc()))
  {
    const OdGeVector3d yAxis = constrDir;
    const OdGeVector3d zAxis = groupPlane.normal();
    const OdGeVector3d xAxis = yAxis.crossProduct(zAxis).normalize();
    return OdGeMatrix3d().setCoordSystem(groupPlane.pointOnPlane(), xAxis, yAxis, zAxis);
  }
  return OdGeMatrix3d::kIdentity;
}

void moveDepFromSrcGroup(OdDbAssoc2dConstraintGroup * pSrcGroup, OdDbAssoc2dConstraintGroup * pDestGroup)
{
  // collect constraints
  OdArray<ConstraintCloneData> aExistingGeomConstraints;
  OdArray<DimConstraintDataPtr> aExistingDimConstraints;

  OdArray<OdGeomConstraint*> apSrcConstraints;
  pSrcGroup->getConstraints(apSrcConstraints);
  unsigned int constrLength = apSrcConstraints.length();
  for (unsigned int k=0; k<constrLength; k++) {

    OdGeomConstraint* pConstraint = apSrcConstraints[k];
    if (!pConstraint)
      continue;

    // Internal constraints are not to be explicitly created if they are not composite
    if (pConstraint->isInternal())
    {
      continue;
    }

    // Note that order constraints are checked is important due to some constraints derive from others!

    DimConstraintDataPtr pDimData;
    OdGeomConstraint::GeomConstraintType type;

    OdGeMatrix3d constraintUcs = getConstraintUCS(*pConstraint, *pSrcGroup);
    if (pConstraint->isKindOf(OdHorizontalConstraint::desc())) // Derived from OdParallelConstraint
      type = OdGeomConstraint::kHorizontal;
    else if (pConstraint->isKindOf(OdVerticalConstraint::desc()))
      type = OdGeomConstraint::kVertical;

    else if (pConstraint->isKindOf(OdG2SmoothConstraint::desc())) // Derived from OdCompositeConstraint
      type = OdGeomConstraint::kG2Smooth;

    else if (pConstraint->isKindOf(OdColinearConstraint::desc()))
      type = OdGeomConstraint::kColinear;
    else if (pConstraint->isKindOf(OdConcentricConstraint::desc()))
      type = OdGeomConstraint::kConcentric;
    else if (pConstraint->isKindOf(OdEqualLengthConstraint::desc()))
      type = OdGeomConstraint::kEqualLength;
    else if (pConstraint->isKindOf(OdEqualRadiusConstraint::desc()))
      type = OdGeomConstraint::kEqualRadius;
    else if (pConstraint->isKindOf(OdFixedConstraint::desc()))
      type = OdGeomConstraint::kFix;
    else if (pConstraint->isKindOf(OdNormalConstraint::desc()))
      type = OdGeomConstraint::kNormal;
    else if (pConstraint->isKindOf(OdParallelConstraint::desc())) // Derived from OdGeomConstraint
      type = OdGeomConstraint::kParallel;
    else if (pConstraint->isKindOf(OdPerpendicularConstraint::desc()))
      type = OdGeomConstraint::kPerpendicular;
    else if (pConstraint->isKindOf(OdPointCoincidenceConstraint::desc()))
      type = OdGeomConstraint::kCoincident;  
    else if (pConstraint->isKindOf(OdSymmetricConstraint::desc()))
      type = OdGeomConstraint::kSymmetric;
    else if (pConstraint->isKindOf(OdTangentConstraint::desc()))
      type = OdGeomConstraint::kTangent;    

    else if (pConstraint->isKindOf(Od3PointAngleConstraint::desc()))
      pDimData = ThreePointAngleConstraintData::createObject();
    else  if (pConstraint->isKindOf(OdAngleConstraint::desc()))
      pDimData = AngleConstraintData::createObject();
    else if (pConstraint->isKindOf(OdDistanceConstraint::desc())) // Derived from OdExplicitConstraint
      pDimData = DistanceConstraintData::createObject();
    else if (pConstraint->isKindOf(OdRadiusDiameterConstraint::desc()))
      pDimData = RadiusDiameterConstraintData::createObject();

    if (!pDimData.isNull())
    {
      if (!pDimData->init(pConstraint))
        continue;
      aExistingDimConstraints.append(pDimData);
      continue;
    }

    OdArray<OdConstrainedGeometry*> apConsGeoms;
    pConstraint->getConnectedGeometries(apConsGeoms);

    OdDbFullSubentPathArray subentPaths;
    unsigned int consGeomLength = apConsGeoms.length();
    if (consGeomLength)
    {
      for (unsigned int l=0; l<consGeomLength; l++) {
        OdDbFullSubentPathArray subentPath;
        OdConstrainedGeometry * pGeom = apConsGeoms[l];
        pGeom->getFullSubentPaths(subentPath);
        if (subentPath.length() > 0)
          subentPaths.append(subentPath[0]);    
      }
    }
    else
    {
      if (pConstraint->isKindOf(OdCompositeConstraint::desc()))
      {
        OdCompositeConstraint * pCG = OdCompositeConstraint::cast(pConstraint);
        OdArray<OdGeomConstraint*> apSubConstraints;
        pCG->getOwnedConstraints(apSubConstraints);
        for (unsigned int k=0; k<apSubConstraints.length(); k++)
        {
          OdGeomConstraint * pSubConstr = apSubConstraints[k];
          OdArray<OdConstrainedGeometry*> apSubCompConsGeoms;
          pSubConstr->getConnectedGeometries(apSubCompConsGeoms);

          bool metOnlyImplicitGeometry = false;
          for (unsigned int l=0; l<apSubCompConsGeoms.length(); l++)
          {
            OdConstrainedGeometry *pCG = apSubCompConsGeoms[l];
            if (!pCG->isKindOf(OdConstrainedImplicitPoint::desc()))
            {
              metOnlyImplicitGeometry = false;
              break;
            }
            metOnlyImplicitGeometry = true;
            OdDbFullSubentPathArray subentPath;
            pCG->getFullSubentPaths(subentPath);
            if (subentPath.length() > 0)
              subentPaths.append(subentPath[0]);
          }
          if (!metOnlyImplicitGeometry)
            subentPaths.clear();
          else if (subentPaths.length())
            break;
        }
      }
    }

    aExistingGeomConstraints.append(ConstraintCloneData(type, subentPaths, constraintUcs));
  }

  // move dependencies
  unsigned int i;
  OdDbObjectIdArray aSrcDependencyIds;
  pSrcGroup->getDependencies(true, true, aSrcDependencyIds);
  unsigned int srcDepLength = aSrcDependencyIds.length();
  for (i=0; i<srcDepLength; i++) {

    OdDbObjectId depId = aSrcDependencyIds[i];
    OdDbAssocDependencyPtr pDep = OdDbAssocDependency::cast(depId.openObject());
    ODA_ASSERT(!pDep.isNull());
    if (pDep.isNull())
      continue;

    OdDbAssocGeomDependencyPtr pGeomDep = OdDbAssocGeomDependency::cast(pDep);
    if (!pGeomDep.isNull())
    {
      OdDbCompoundObjectId compId;
      pDep->getDependentOnCompoundObject(compId);
      OdDbObjectId objectId = compId.leafId();

      OdDbEntityPtr pObject = OdDbEntity::cast(objectId.openObject(OdDb::kForRead));
      OdSmartPtr<OdDbAssocPersSubentIdPE> persSubentIdPE = OdDbAssocPersSubentIdPE::cast(pObject);
      if (persSubentIdPE.isNull())
        continue;

      OdArray<OdDbSubentId> aSubentIds;
      persSubentIdPE->getTransientSubentIds(pObject, pGeomDep->persistentSubentId(), aSubentIds);
      if (aSubentIds.size() < 1)
      {
        const OdDb::SubentType subentType = (pGeomDep->persistentSubentId().isNull())
          ? OdDb::kEdgeSubentType
          : pGeomDep->persistentSubentId()->subentType(pObject);
        persSubentIdPE->getAllSubentities(pObject, subentType, aSubentIds);
        if (aSubentIds.size() < 1)
          continue;
      }
      persSubentIdPE.release();

      OdDbObjectIdArray fullPath;
      compId.getFullPath(fullPath);

      OdDbFullSubentPath fsPath(fullPath, aSubentIds[0]); 

      pSrcGroup->removeDependency(depId, false);

      pGeomDep->upgradeOpen();
      pDestGroup->assertWriteEnabled();
      OdDbImpAssoc2dConstraintGroup *pDestImp = (OdDbImpAssoc2dConstraintGroup*)OdDbImpAssocAction::getImpObject(pDestGroup);
      pDestImp->moveConstrainedGeometry(fsPath, pGeomDep, pDestGroup);
    }
    else {
      pSrcGroup->removeDependency(depId, false);
      pDestGroup->addDependency(depId, true);
    }
  }

  OdArray<ConstraintCloneData>::iterator it = aExistingGeomConstraints.begin();
  for (; it != aExistingGeomConstraints.end(); ++it)
  {
    for (unsigned idx = 0; idx < it->m_aPaths.size(); idx++)
    {
      OdConstrainedGeometry* pG = 0;
      pDestGroup->addConstrainedGeometry(it->m_aPaths[idx], pG);
    }

    OdGeomConstraint* ppNewConstraint = 0;
    const OdGeMatrix3d* pUCSCoord = it->m_ucs.isEqualTo(OdGeMatrix3d::kIdentity) ? NULL : &it->m_ucs;
    ODA_VERIFY(eOk == pDestGroup->addGeometricalConstraint(it->m_type, it->m_aPaths, &ppNewConstraint, pUCSCoord));
  }

  OdArray<DimConstraintDataPtr>::iterator dimIt = aExistingDimConstraints.begin();
  bool res;
  for (; dimIt != aExistingDimConstraints.end(); ++dimIt)
  {
    res = (*dimIt)->fill(pDestGroup);
    ODA_ASSERT(res);
  }
  pSrcGroup->erase();
}



void OdDbAssocVarCloneCtx::addDestVariables(OdDbAssocNetwork* network)
{
  const OdDbObjectIdArray aActionIds = network->getActions();
  for (unsigned i = 0; i < aActionIds.size(); ++i) {
    const OdDbObjectId actionId = aActionIds[i];
    if (actionId.objectClass()->isDerivedFrom(OdDbAssocVariable::desc())) {
      OdDbAssocVariablePtr pVar = OdDbAssocVariable::cast(actionId.openObject());
      if (pVar.get() && !pVar->isAnonymous())
        addDestVariable(pVar->name());
    }
  }
}

void OdDbAssocVarCloneCtx::addSourceVariables(OdDbAssocVariable* variable)
{
  if (variable && !variable->isAnonymous() && addSourceVariable(variable->name())) {
    OdDbObjectIdArray aDeps;
    variable->getDependencies(true, true, aDeps);
    for (unsigned i = 0; i < aDeps.size(); ++i) {
      const OdDbObjectId depId = aDeps[i];
      if (depId.objectClass() == OdDbAssocValueDependency::desc()) {
        OdDbAssocValueDependencyPtr valDep = OdDbAssocValueDependency::cast(depId.openObject());
        if (valDep.get()) {
          OdDbAssocVariablePtr dependentVar = OdDbAssocVariable::cast(valDep->dependentOnObject().openObject());
          addSourceVariables(dependentVar);
        }
      }
    }
  }
}

bool OdDbAssocVarCloneCtx::addSourceVariable(const OdString& srcName)
{
  OdString2IdxMap::value_type record(OdString(srcName).makeUpper(), 0);
  std::pair<OdString2IdxMap::iterator, bool> insertRes = m_forwardIndex.insert(record);
  if (insertRes.second) {
    DataEntry entry;
    entry.m_srcName = srcName;
    entry.m_isCloned = false;

    m_data.append(entry);
    insertRes.first->second = m_data.size() - 1;
  }
  return insertRes.second;
}

bool OdDbAssocVarCloneCtx::addDestVariable(const OdString& dstName)
{
  OdString2IdxMap::value_type record(OdString(dstName).makeUpper(), 0);
  std::pair<OdString2IdxMap::iterator, bool> insertRes = m_invertedIndex.insert(record);
  if (insertRes.second) {
    DataEntry entry;
    entry.m_dstName = dstName;
    entry.m_isCloned = false;

    m_data.append(entry);
    insertRes.first->second = m_data.size() - 1;
  }
  return insertRes.second;
}

void OdDbAssocVarCloneCtx::recordVariableClone(const OdString& srcName, const OdString& dstName)
{
  const OdString srcKey = OdString(srcName).makeUpper();
  const OdString dstKey = OdString(dstName).makeUpper();

  ODA_ASSERT(m_invertedIndex.find(OdString(dstName).makeUpper()) == m_invertedIndex.end() &&
             "A variable with name matching distination name already exists");

  OdString2IdxMap::value_type forwardIndexRecord(srcKey, 0);
  std::pair<OdString2IdxMap::iterator, bool> forwardInsertRes = m_forwardIndex.insert(forwardIndexRecord);
  if (forwardInsertRes.second) {
    DataEntry entry;
    entry.m_srcName = srcName;
    entry.m_dstName = dstName;
    entry.m_isCloned = true;

    m_data.append(entry);
    forwardInsertRes.first->second = m_data.size() - 1;
  }
  else {
    const unsigned idx = forwardInsertRes.first->second;
    DataEntry& entry = m_data[idx];
    entry.m_dstName = dstName;
    entry.m_isCloned = true;
  }
  m_invertedIndex[dstKey] = forwardInsertRes.first->second;
}

const OdDbAssocVarCloneCtx::DataEntry* OdDbAssocVarCloneCtx::getEntryBySourceName(const OdString& srcName) const
{
  OdString2IdxMap::const_iterator it = m_forwardIndex.find(OdString(srcName).makeUpper());
  return it != m_forwardIndex.end() ? &m_data[it->second] : NULL;
}

const OdDbAssocVarCloneCtx::DataEntry* OdDbAssocVarCloneCtx::getEntryByDestName(const OdString& dstName) const
{
  OdString2IdxMap::const_iterator it = m_invertedIndex.find(OdString(dstName).makeUpper());
  return it != m_invertedIndex.end() ? &m_data[it->second] : NULL;
}

int OdDbAssocVarCloneCtx::destVariableCount() const
{
  return int(m_invertedIndex.size());
}

int OdDbAssocVarCloneCtx::clonedVariableCount() const
{
  return int(m_forwardIndex.size());
}

class RecordSourceVarName
{
  OdDbIdMapping& m_idMap;
  OdDbAssocVarCloneCtx& m_varCtx;
  OdDbAssocNetwork& m_net;
public:
  RecordSourceVarName(OdDbIdMapping& idMap, OdDbAssocVarCloneCtx& varCtx, OdDbAssocNetwork& net)
    : m_idMap(idMap), m_varCtx(varCtx), m_net(net)
  {}
  void operator()(const OdDbObjectId& actionId) {
    if (actionId.objectClass()->isDerivedFrom(OdDbAssocVariable::desc())) {
      OdDbIdPair cloneSearch(actionId);
      if (m_idMap.compute(cloneSearch) && cloneSearch.isCloned()) {
        OdDbObjectPtr clone = cloneSearch.value().openObject();
        if (clone->ownerId() == m_net.objectId() || clone->ownerId() == m_net.objectThatOwnsNetworkInstance()) {
          OdDbAssocVariablePtr origVar = OdDbAssocVariable::cast(actionId.openObject());
          m_varCtx.addSourceVariables(origVar);
        }
      }
    }
  }
};

OdDbAssocVarCloneCtx& OdDbAssocNetCloneCtx::varCloneContext(OdDbAssocNetwork* destNetwork) {
  NetworkVariableIndex::iterator it = variableIndices.find(destNetwork);
  if (it == variableIndices.end()) {
    OdDbAssocVarCloneCtx varCtx;
    varCtx.addDestVariables(destNetwork);
    std::for_each(actionsToAppend.begin(), actionsToAppend.end(),
                  RecordSourceVarName(*idMap, varCtx, *destNetwork));

    it = variableIndices.insert(NetworkVariableIndex::value_type(destNetwork, varCtx)).first;
  }
  return it->second;
}

struct ReactorKeeper
{
  OdStaticRxObject<OdDbAssocNetworkDeepCloneReactor> m_assocNetworkDeepCloneReactor;
  OdStaticRxObject<OdDbAssocNetworkLongTransactionReactor> m_assocNetworkLongTransactionReactor;
};

ReactorKeeper * gpReactorKeeper = 0;
void OdDbAssocNetCloneCtx::createAssocNetworkCloneReactors()
{
  DimConstraintData::rxInit();
  DistanceConstraintData::rxInit();
  AngleConstraintData::rxInit();
  ThreePointAngleConstraintData::rxInit();
  RadiusDiameterConstraintData::rxInit();
  OdDbAssocNetworkObjectModificationReactor::rxInit();
  OdDbAssocNetCloneOverrule::rxInit();
  gpReactorKeeper = new ReactorKeeper;
  gpReactorKeeper->m_assocNetworkDeepCloneReactor.addRefReactor();
  gpReactorKeeper->m_assocNetworkLongTransactionReactor.addRefReactor();
}

void OdDbAssocNetCloneCtx::deleteAssocNetworkCloneReactors()
{
  gpReactorKeeper->m_assocNetworkDeepCloneReactor.releaseReactor();
  gpReactorKeeper->m_assocNetworkLongTransactionReactor.releaseReactor();
  delete gpReactorKeeper;
  gpReactorKeeper = 0;
  OdDbAssocNetCloneOverrule::rxUninit();
  OdDbAssocNetworkObjectModificationReactor::rxUninit();
  DistanceConstraintData::rxUninit();
  AngleConstraintData::rxUninit();
  ThreePointAngleConstraintData::rxUninit();
  RadiusDiameterConstraintData::rxUninit();
  DimConstraintData::rxUninit();
}



void OdDbAssocNetCloneCtx::onBlockCloned(const OdDbBlockTableRecord& originalBlock)
{
  OdDbIdPair ownerMapping(originalBlock.objectId());
  idMap->compute(ownerMapping);
  OdDbObjectPtr destOwner = ownerMapping.value().openObject();

  const OdDbObjectId networkId = OdDbAssocNetwork::getInstanceFromObject(originalBlock.objectId(), false);
  OdDbAssocNetworkPtr network = OdDbAssocNetwork::cast(networkId.openObject());
  if (network.get() && destOwner.get()) {
    const OdDbObjectIdArray actionIds = network->getActions();
    for (unsigned int i = 0; i < actionIds.size(); ++i) {
      OdDbObjectPtr action = actionIds[i].openObject();
      if (action.get()) {
        OdDbAssocNetCloneCtx::cloneObject(*idMap, action, destOwner);
      }
    }
  }
}

void OdDbAssocNetCloneCtx::onDependentObjectCloned(const OdDbObject& originalObject)
{
  dependentOnObjectsCloned.insert(originalObject.objectId());
}

// Returns true only if id or its owner belongs to OdDbEvalGraph.
// Only 'master' dynamic block definition has OdDbEvalGraph in the extension dictionary (ACAD_ENHANCEDBLOCK)
bool OdDbAssocNetCloneCtx::isOwnedByEvalGraph(OdDbObjectId id)
{
  OdDbObjectPtr obj = id.openObject();
  if (obj.isNull())
    return false;
  do
  {
    obj = obj->ownerId().openObject();
  } while (!obj.isNull() && !obj->isA()->isDerivedFrom(OdDbEvalGraph::desc()));

  ODA_ASSERT(!obj.isNull() ? obj->ownerId().objectClass()->isDerivedFrom(OdDbDictionary::desc()) : true);
  return !obj.isNull();
}

OdDbObjectId cloneOwner(OdDbIdMapping& idMap, const OdDbObject* action);

bool cloneObjectOwnerDeduced(OdDbObjectId objectId, OdDbAssocAction* pAction,
                             OdDbBlockTableRecordPtr& pDestBlock, OdDbIdMapping& idMap) {
  OdDbIdPair mapping(objectId);
  if (idMap.compute(mapping) && mapping.isCloned())
    return false;

  OdDbObjectPtr object = objectId.openObject();
  if (!object.get())
    return false;

  OdDbObjectPtr owner;
  if (object->isKindOf(OdDbBlockTableRecord::desc())) {
    owner = idMap.destDb()->getBlockTableId().openObject();
  }
  else if (object.get() == pAction) {
    OdDbAssocNetCloneCtx::lookupDestinationBlock(idMap, pAction, pDestBlock);
    owner = pDestBlock; // see OdDbAssocAction::appendToOwner()
  }
  else if (object->isKindOf(OdDbAssocAction::desc())) {
    OdDbObjectPtr cloneOwner = ::cloneOwner(idMap, pAction).openObject();
    if (cloneOwner.get())
      owner = cloneOwner;
    else {
      OdDbAssocNetCloneCtx::lookupDestinationBlock(idMap, pAction, pDestBlock);
      owner = pDestBlock; // see OdDbAssocAction::appendToOwner()
    }
  }
  else if (object->isKindOf(OdDbEntity::desc())) {
    OdDbAssocNetCloneCtx::lookupDestinationBlock(idMap, pAction, pDestBlock);
    owner = pDestBlock;
  }
  ODA_ASSERT(!owner.isNull() && "can't automatiacally obtain cloned action destination owner.\n"
    "Action was not cloned.\n"
    "Action should clone itself inside addMoreObjectsToDeepClone() providing destination owner explicitly");

  return OdDbAssocNetCloneCtx::cloneObject(idMap, object, owner);
}

class AddMoreObjectsProcessor
{
public:
  AddMoreObjectsProcessor(OdDbAssocNetCloneCtx *assocNetCloneCtx)
    : m_idMap(assocNetCloneCtx->idMapping())
    , m_assocNetCloneCtx(assocNetCloneCtx)
  {}

  bool init() {
    const bool collectFromDependentObjects = isInLongTransaction(m_idMap);

    bool result = false;
    OdDbIdMappingIterPtr pIt = m_idMap.newIterator();
    for (; !pIt->done(); pIt->next()) {
      OdDbIdPair mapping;
      pIt->getMap(mapping);
      if (!mapping.isCloned()) {
        continue;
      }

      result |= appendIfAction(mapping.key());
      result |= !collectFromDependentObjects && appendObjectActions(mapping.key());
    }
    return result;
  }

  void process() {
    OdDbObjectIdArray aActionsToProcess;
    OdDbObjectIdArray additionalObjectsToClone;
    while (!m_aActionIds.isEmpty()) {
      m_alreadyAdded.clear();
      aActionsToProcess.clear();
      std::swap(m_aActionIds, aActionsToProcess);

      OdDbObjectIdArray::iterator actionToProcessIt = aActionsToProcess.begin();
      OdDbObjectIdArray::iterator actionToProcessEnd = aActionsToProcess.end();
      for (; actionToProcessIt != actionToProcessEnd; ++actionToProcessIt) {
        OdDbAssocActionPtr pAction = OdDbAssocAction::cast(actionToProcessIt->openObject());
        if (pAction.isNull()) {
          continue;
        }

        additionalObjectsToClone.clear();
        pAction->addMoreObjectsToDeepClone(m_idMap, additionalObjectsToClone);

        OdDbBlockTableRecordPtr pDestBlock;
        OdDbObjectIdArray::iterator toCloneIt = additionalObjectsToClone.begin();
        OdDbObjectIdArray::iterator toCloneEnd = additionalObjectsToClone.end();
        for (; toCloneIt != toCloneEnd; ++toCloneIt) {
          if (!cloneObjectOwnerDeduced(*toCloneIt, pAction, pDestBlock, m_idMap)) {
            continue;
          }

          appendIfAction(*toCloneIt);
          appendOwnedActions(*toCloneIt);
          appendObjectActions(*toCloneIt);
        }
      }
    }
  }

private:

  // return true if an object is watched by assoc actions
  bool appendObjectActions(OdDbObjectId objectId) {
    OdDbObjectPtr pObject = objectId.openObject();
    if (pObject.isNull()) {
      return false;
    }

    bool result = false;
    OdDbObjectId depId = OdDbAssocDependency::getFirstDependencyOnObject(pObject);
    OdDbAssocDependencyPtr pDep = OdDbAssocDependency::cast(depId.openObject());
    while (!pDep.isNull()) {
      if (m_alreadyAdded.insert(pDep->owningAction()).second) {
        m_aActionIds.append(pDep->owningAction());
      }
      depId = pDep->nextDependencyOnObject();
      pDep = OdDbAssocDependency::cast(depId.openObject());
      result = true;
    }

    return result;
  }

  // return true if action
  bool appendIfAction(OdDbObjectId objectId) {
    bool isAction = objectId.objectClass()->isDerivedFrom(OdDbAssocAction::desc());
    if (isAction && m_alreadyAdded.insert(objectId).second) {
      m_aActionIds.append(objectId);
    }
    return isAction;
  }

  // return true if block
  bool appendOwnedActions(OdDbObjectId objectId) {
    bool isBlock = objectId.objectClass()->isDerivedFrom(OdDbBlockTableRecord::desc());
    if (!isBlock) {
      return false;
    }

    OdDbObjectId netId = OdDbAssocNetwork::getInstanceFromObject(objectId, false);
    OdDbAssocNetworkPtr pNet = OdDbAssocNetwork::cast(netId.openObject());
    if (pNet.isNull()) {
      return true;
    }

    OdDbObjectIdArray aNetActionIds = pNet->getActions();
    OdDbObjectIdArray::iterator it = aNetActionIds.begin();
    OdDbObjectIdArray::iterator end = aNetActionIds.end();
    for (; it != end; ++it) {
      OdDbIdPair actionMapping(*it);
      if (m_idMap.compute(actionMapping) && actionMapping.isCloned() &&
          m_alreadyAdded.insert(*it).second) {
        m_aActionIds.append(*it);
      }
    }

    return true;
  }

private:
  OdDbIdMapping& m_idMap;
  OdDbObjectIdArray m_aActionIds;
  OdDbStubPtrHashSet m_alreadyAdded;
  OdDbAssocNetCloneCtxPtr m_assocNetCloneCtx;
};

static void copyModelSpaceActions(OdDbIdMapping& idMap)
{
  OdDbObjectId msid = idMap.origDb()->getModelSpaceId();
  OdDbAssocNetworkPtr network = OdDbAssocNetwork::cast(OdDbAssocNetwork::getInstanceFromObject(msid, false).openObject());
  if (network.get()) {
    OdDbObjectPtr owner = idMap.destDb()->getModelSpaceId().safeOpenObject();

    const OdDbObjectIdArray actionIds = network->getActions();
    for (unsigned int i = 0; i < actionIds.size(); ++i) {
      OdDbObjectPtr object = actionIds[i].openObject();
      if (object.get()) {
        OdDbAssocNetCloneCtx::cloneObject(idMap, object, owner);
      }
    }
  }
}

void OdDbAssocNetCloneCtx::onActionCloned(const OdDbAssocAction* action) {
  OdDbIdPair idPair(action->objectId());
  if (idMap->compute(idPair)) {
    if (!idPair.isOwnerXlated()) {
      idPair.setOwnerXlated(true); // suppress default appending to owner
      idMap->assign(idPair);
      actionsToAppend.append(idPair.key());
    }
    actionsToClone.insert(idPair.key());
    clonedActions().append(idPair.key());
  }
}

OdDbObjectId cloneOwner(OdDbIdMapping& idMap, const OdDbObject* action) {
  OdDbObjectPtr clone = ::oddbTranslateCloned(action->objectId(), idMap).openObject();
  if(clone.get())
    return clone->ownerId();
  return OdDbObjectId::kNull;
}

OdDbObjectId getDependentOnObjectId_or_topIdOfCompound_from(OdDbAssocDependency* dependency) {
  if (dependency->isDependentOnCompoundObject()) {
    OdDbCompoundObjectId compound;
    dependency->getDependentOnCompoundObject(compound);
    return compound.topId();
  }
  return dependency->dependentOnObject();
}

void OdDbAssocNetCloneCtx::lookupDestinationBlock(OdDbIdMapping& idMap, OdDbAssocAction* action, OdDbBlockTableRecordPtr& destBlock) {
  if (destBlock.get()) return;

  OdDbObjectIdArray dependencies;
  action->getDependencies(true, false, dependencies);
  OdDbObjectId actionOwningBlock = action->objectThatOwnsNetworkInstance();
  for (unsigned int i = 0; i < dependencies.length(); i++)
  {
    OdDbAssocDependencyPtr dependency = dependencies[i].openObject();
    if (dependency.get())
    {
      OdDbIdPair pair( getDependentOnObjectId_or_topIdOfCompound_from(dependency) );
      if (idMap.compute(pair) && pair.isCloned())
      {
        OdDbObjectPtr original = pair.key().openObject();
        OdDbObjectPtr clone = pair.value().openObject();
        if ( original.get() && original->ownerId()==actionOwningBlock &&
             clone.get() && clone->isKindOf(OdDbEntity::desc()) )
        {
          // found dependent dbentity that was cloned from the same block that owns the action
          OdDbObjectId cloneOwnerId = clone->ownerId();
          if (clone->isOdDbObjectIdsInFlux()) {
            OdDbObjectId ownerIdToTranslate = cloneOwnerId ? cloneOwnerId : actionOwningBlock;
            OdDbObjectId translatedCloneOwnerId = ::oddbTranslate(ownerIdToTranslate, idMap);
            if(!translatedCloneOwnerId.isNull()) // may be not cloned
              cloneOwnerId = translatedCloneOwnerId;
          }
          destBlock = OdDbBlockTableRecord::cast(cloneOwnerId.openObject());
          if(destBlock.get())
            return;
        }
      }
    }
  }
  ODA_ASSERT(!destBlock.isNull() && "can't automatiacally obtain cloned action destination owner\n"
    "action should clone itself inside addMoreObjectsToDeepClone() providing destination owner explicitly");
}

OdDbObjectPtr OdDbAssocNetCloneCtx::lookupActionCloneOwner(OdDbIdMapping& idMap, OdDbBlockTableRecord* destBlock) {
  return destBlock;
}

OdDbObjectPtr OdDbAssocNetCloneCtx::lookupActionCloneOwner(OdDbIdMapping& idMap, OdDbAssocAction* action) {
  OdDbBlockTableRecordPtr destBlock;
  OdDbAssocNetCloneCtx::lookupDestinationBlock(idMap, action, destBlock);
  return OdDbAssocNetCloneCtx::lookupActionCloneOwner(idMap, destBlock);
}

OdDbObjectPtr OdDbAssocNetCloneCtx::lookupActionCloneOwner(OdDbIdMapping& idMap, OdDbEntity* destActionEntity) {
  OdDbBlockTableRecordPtr destBlock = OdDbBlockTableRecord::cast(destActionEntity->ownerId().openObject());
  return destBlock;
}

void OdDbAssocNetCloneCtx::trackEvalGraphParam(OdDbObjectId action, OdDbObjectId dependentOnEvalGraphParamId) {
  evalGraphParams[action].insert(dependentOnEvalGraphParamId);
}

void OdDbAssocNetCloneCtx::addMoreObjectsToDeepClone() {
  const OdDb::DeepCloneType dct = idMap->deepCloneContext();
  switch (dct) {
    case OdDb::kDcInsert:
    case OdDb::kDcInsertCopy:
    case OdDb::kDcXrefBind:
    case OdDb::kDcXrefInsert:
      ::copyModelSpaceActions(*idMap);
    break;
    default: break;
  }

  AddMoreObjectsProcessor amoProcessor(this);
  if (amoProcessor.init()) {
    amoProcessor.process();
  }
}

// some cloned objects may have NULL owner (e.g. OdDbDictionary when 
// cloned "master" dynamic block definition already exist in the destDB)
// It checks that object or one of its owner belong to a BTR of destDB 
static bool isOwner_OdDbBTR(const OdDbObjectId& id, OdDbObjectId& btrId) {
  btrId.setNull();

  OdDbObjectPtr obj = id.openObject();
  if (obj.isNull())
    return false;
  do {
    obj = obj->ownerId().openObject();
  }
  while (!obj.isNull() && !obj->isKindOf(OdDbBlockTableRecord::desc()));

  if (!obj.isNull()) {
    btrId = obj->objectId();
    return true;
  }
  return false;
}

static OdDbBlockParameterPtr findParameterInDynBlock(const OdDbObjectId& inBtrId, const OdDbObjectId& sameAsParamId) {
  OdDbBlockParameterPtr emptyParam;

  if (!OdDbDynBlockReference::isDynamicBlock(inBtrId))
    return emptyParam;

  OdDbBlockTableRecordPtr btr = OdDbBlockTableRecord::cast(inBtrId.openObject());
  OdDbBlockParameterPtr sameAsParam = OdDbBlockParameter::cast(sameAsParamId.openObject());
  if (btr.isNull() || sameAsParam.isNull())
    return emptyParam;

  OdDbEvalGraphPtr gr = OdDbEvalGraph::getGraph(btr, L"ACAD_ENHANCEDBLOCK");
  if (gr.isNull())
    return emptyParam;

  OdDbEvalNodeIdArray nodes;
  gr->getAllNodes(nodes);
  for(OdUInt32 i=0,n=nodes.size(); i<n; ++i)
  {
    OdDbEvalNodeId nodeId = nodes[i];
    OdDbBlockParameterPtr p = OdDbBlockParameter::cast(gr->getNode(nodeId));
    if (!p.isNull() && p->isA() == sameAsParam->isA() && p->name() == sameAsParam->name())
      return p;  // maybe also compare p->getPropertyDescription(ar) ?
  }
  return emptyParam;
}

class RelinkDependecies {
  OdDbIdMapping& idMap;
public:
  RelinkDependecies(OdDbIdMapping& idM) : idMap(idM) { }
  void operator()(const OdDbObjectId& id) {
    OdDbIdPair idPair(id);
    if (idMap.compute(idPair) && idPair.isCloned())
      OdDbImpAssocDependency::relinkDependenciesOn(idPair.value().openObject());
  }
};

void OdDbAssocNetCloneCtx::relinkDependenciesOnObjects() {
  std::for_each(dependentOnObjectsCloned.begin(), dependentOnObjectsCloned.end(), RelinkDependecies(*idMap));
}

class ReattachEvalGraphDependecies {
  OdDbIdMapping& idMap;

  static void reattachDependency(OdDbAssocDependency* dependency, OdDbObjectId toObject) {
    dependency->detachFromObject();
    OdDbCompoundObjectId compoundId;
    compoundId.set(toObject);
    dependency->attachToObject(compoundId);
  }
public:
  ReattachEvalGraphDependecies(OdDbIdMapping& idM) : idMap(idM) { }

  void operator()(const std::pair<OdDbStub*, OdDbStubPtrHashSet>& actionDependentOnEvalParams) {
    OdDbIdPair actionIdPair(actionDependentOnEvalParams.first);
    if (!idMap.compute(actionIdPair) || !actionIdPair.isCloned())
      return;
    OdDbAssocActionPtr origAction = OdDbObjectId(actionIdPair.key()).openObject();

    OdDbAssocActionPtr cloneAction = actionIdPair.value().openObject();
    if (origAction.isNull() || cloneAction.isNull())
      return;

    if(OdDbBlockTableRecord::cast(cloneAction->objectThatOwnsNetworkInstance().openObject()).isNull())
      return; // orphaned clone (owning block record was erased)

    OdDbObjectId btrId;
    const OdDbStubPtrHashSet& evalParams = actionDependentOnEvalParams.second;
    OdDbObjectIdArray origDependencies;
    origAction->getDependencies(true, true, origDependencies);
    bool sameDatabase = idMap.origDb() == idMap.destDb();
    for (OdUInt32 i=0, n=origDependencies.size(); i<n; ++i) {
      OdDbIdPair depIdPair(origDependencies[i]);
      if(!idMap.compute(depIdPair) || !depIdPair.isCloned())
        continue;

      OdDbAssocDependencyPtr originalDependency = OdDbAssocDependency::cast(depIdPair.key().openObject());
      if (originalDependency.isNull())
        continue;

      OdDbObjectId originalDependentOnObjectId = originalDependency->dependentOnObject();
      if (originalDependentOnObjectId.isNull() || evalParams.find(originalDependentOnObjectId)==evalParams.end())
        continue;

      OdDbAssocDependencyPtr clonedDependency = OdDbAssocDependency::cast(depIdPair.value().openObject(OdDb::kForWrite));
      if (clonedDependency.isNull())
        continue;

      if(sameDatabase) {
        // Fix1: dynamic block ref is being copied (or modified) in same DB. "Master" or anonymous BTR 
        // is being copied too. But only "master" dynamic BTR can store ACAD_ENHANCEDBLOCK dictionary record.
        // Note, when "Master" BTR is being copied, ACAD_ENHANCEDBLOCK dictionary record is deleted after operation. 
        // Cloned OdDbAssocDependency mast refer to one "Master" BTR. 
        // Note2: clonedDependentOnObjectId can be NULL when block reference has anonymous dynamic BTR.

        reattachDependency(clonedDependency, originalDependentOnObjectId);
      }
      else {
        OdDbObjectId clonedDependentOnObjectId = clonedDependency->dependentOnObject();

        if(!clonedDependentOnObjectId.isNull() &&
           !isOwner_OdDbBTR(clonedDependentOnObjectId,btrId) && btrId.isNull())
        {
          // Fix2: dynamic block ref is being copied from another DB. So "master" and anonymous dynamic BTR 
          // are copied too. But destination DB already has BTR with such name (like "master" BTR).   
          // Therefore we try to detect appropriate object (dependentOnObjectId) in already existing 
          // (in destDB) BTR and use it as dependentOnObject in cloned OdDbAssocDependency object.

          OdDbObjectId originalBtrId;
          if(!isOwner_OdDbBTR(originalDependentOnObjectId,originalBtrId))
            continue;

          OdDbIdPair btrIdPair(originalBtrId);
          if(!idMap.compute(btrIdPair))
            continue;

          ODA_ASSERT(!btrIdPair.isCloned()); // btrIdPair.value() is a BTR that already exist in the destDB
          OdDbBlockParameterPtr paramInExistingBlock = findParameterInDynBlock(btrIdPair.value(),originalDependentOnObjectId);
          if(paramInExistingBlock.isNull())
            continue;

          reattachDependency(clonedDependency,paramInExistingBlock->objectId());
        }
      }
    }
  }
};

class AppendActions {
  OdDbIdMapping& idMap;
public:
  AppendActions(OdDbIdMapping& idM) : idMap(idM) { }
  void operator()(const OdDbObjectId& id) {
    OdDbAssocActionPtr pSourceAction = id.openObject();
    if (pSourceAction.get()) {
      OdDbIdPair idPair(id);
      if (idMap.compute(idPair) && idPair.isCloned()) {
        OdDbAssocActionPtr clone = OdDbAssocAction::cast(idPair.value().openObject(OdDb::kForWrite));
        if (clone.get()) {
          OdDbObjectPtr owner = clone->ownerId().openObject(OdDb::kForWrite);
          if (owner.get()) {
            idPair.setOwnerXlated(false);
            clone->appendToOwner(idPair, owner, idMap);
            idPair.setOwnerXlated(true);
            idMap.assign(idPair);
          }
        }
      }
    }
  }
};


void attachToOwniningNetwork(OdDbAssocAction* clone, OdDbObjectId owner, bool setAsDbOwner) {
  OdDbAssocStatus status = clone->status();
  clone->setStatus(kIsUpToDateAssocStatus);
  clone->setOwningNetwork(owner, setAsDbOwner);
  clone->setStatus(status);
}

void OdDbAssocNetCloneCtx::appendActionsToOwners() {
  std::for_each(actionsToAppend.begin(), actionsToAppend.end(), AppendActions(*idMap));

  OdDbObjectIdLinkedArray& actions = clonedActions();
  OdDbObjectId global;
  for(OdDbObjectIdLinkedArray::const_iterator it = actions.begin(), end = actions.end(); it!=end; ++it) {
    OdDbAssocActionPtr pSourceAction = it->openObject();
    if (pSourceAction.get()) {
      OdDbIdPair idPair(*it);
      if (idMap->compute(idPair) && idPair.isCloned()) {
        OdDbAssocActionPtr clone = OdDbAssocAction::cast(idPair.value().openObject(OdDb::kForWrite));
        if (clone.get() && clone->owningNetwork().isNull()) {
          OdDbObjectPtr owner = clone->ownerId().openObject(OdDb::kForWrite);
          if (owner.get()) {
            if(owner->isKindOf(OdDbAssocNetwork::desc())) {
              ::attachToOwniningNetwork(clone, owner->objectId(), true);
            }
            else if(owner->isKindOf(OdDbDictionary::desc())) {
              if(global.isNull())
                global = OdDbAssocNetwork::getInstanceFromDatabase(idMap->destDb(), true);
              ::attachToOwniningNetwork(clone, global, false);
            }
          }
        }
      }
    }
  }

  std::for_each(evalGraphParams.begin(), evalGraphParams.end(), ReattachEvalGraphDependecies(*idMap));
}

OdDbObjectPtr OdDbAssocNetCloneOverrule::deepClone(const OdDbObject* subject, OdDbIdMapping& idMap, OdDbObject* owner, bool primary /*= true*/) {
  OdDbObjectPtr clone = OdDbObjectOverrule::deepClone(subject, idMap, owner, primary);
  if(clone.get()) {
    OdDbAssocNetCloneCtxPtr ctx = OdDbAssocNetCloneCtx::internal_fromDb(idMap.destDb());
    if (ctx.get() && subject->isA()->isDerivedFrom(OdDbBlockTableRecord::desc())) {
      ctx->onBlockCloned(static_cast<const OdDbBlockTableRecord&>(*subject));
    }
  }

  return clone;
}

OdDbObjectPtr OdDbAssocNetCloneOverrule::wblockClone(const OdDbObject* subject, OdDbIdMapping& idMap, OdDbObject* owner, bool primary /*= true*/) {
  OdDbObjectPtr clone = OdDbObjectOverrule::wblockClone(subject, idMap, owner, primary);
  if (clone.get()) {
    OdDbAssocNetCloneCtxPtr ctx = OdDbAssocNetCloneCtx::internal_fromDb(idMap.destDb());
    if (ctx.get() && subject->isA()->isDerivedFrom(OdDbBlockTableRecord::desc())) {
      ctx->onBlockCloned(static_cast<const OdDbBlockTableRecord&>(*subject));
    }
  }

  return clone;
}

bool OdDbAssocNetCloneOverrule::isApplicable(const OdRxObject* pOverruledSubject) const {
  return pOverruledSubject && pOverruledSubject->isA()->isDerivedFrom(OdDbBlockTableRecord::desc());
}

OdDbAssocNetCloneOverrule::OdDbAssocNetCloneOverrule()
  : m_clients(0),
    m_WasOverruling(false)
{}


void OdDbAssocNetCloneOverrule::addOverruleClient()
{
  if (!(m_clients++))
  {
    m_WasOverruling = OdRxOverrule::isOverruling();
    OdDbObjectOverrule::addOverrule(OdDbBlockTableRecord::desc(), this);
    OdRxOverrule::setIsOverruling(true);
  }
  ODA_ASSERT(m_clients > 0);
}

void OdDbAssocNetCloneOverrule::removeOverruleClient()
{
  if (!(--m_clients))
  {
    if(OdRxOverrule::removeOverrule(OdDbBlockTableRecord::desc(), this))
      OdRxOverrule::setIsOverruling(m_WasOverruling);
  }
  ODA_ASSERT(m_clients >= 0);
}
