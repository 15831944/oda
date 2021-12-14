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
#include "DbAssocArrayModifyActionBody.h"
#include "DbImpAssocActionBody.h"
#include "DbImpAssocArrayParameters.h"
#include "NextCodeDefs.h"
#include "DbBlockTableRecord.h"
#include "DbBlockTable.h"
#include "DbBlockReference.h"
#include "DbAssocArrayCommonParameters.h"
#include "DbAssocArrayPathParameters.h"
#include "DbAssocArrayPolarParameters.h"
#include "DbAssocArrayRectangularParameters.h"
#include "DbAssocNetwork.h"
#include "DbAssocDependency.h"
#include "DbAssocActionParam.h"
#define STL_USING_ALGORITHM
#include <OdaSTL.h>
#include "DbCurve.h"
#include "DbAssocNetCloneCtx.h"
#include "DbDictionary.h"
#include "DbAssocManager.h"
#include "DbSortentsTable.h"
#include "Ge/GeScale3d.h"
#include "DbAudit.h"
#include "DbHostAppServices.h"
#include "OdDbAssocPersSubentIdPE.h"
#include "DbAssocGeomDependency.h"
#include "DbObjectId.h"
#include <typeinfo>
//////////////////////////////////////////////////////////////////////////

class findItemByLocator
{
  const OdDbItemLocator &m_locator;
public:
  inline findItemByLocator(const OdDbItemLocator& locator)
    : m_locator(locator)
  {
  }

  inline bool operator()( const OdDbAssocArrayItemPtr &Item) const
  {
    return Item->locator() == m_locator;
  }
};

class findItemBySubPath
{
  const OdDbFullSubentPath& m_path;
public:
  inline findItemBySubPath(const OdDbFullSubentPath& path)
    : m_path(path)
  {
  }

  inline bool operator()( const OdDbAssocArrayItemPtr &Item) const
  {
    return Item->entityProvider() == m_path.objectIds().last();
  }
};

class getIndices
{
  OdArray<OdDbItemLocator>& m_indices;
  const bool m_skipErased;
public:
  inline getIndices( OdArray<OdDbItemLocator>& indices, const bool skipErased)
    : m_indices(indices), m_skipErased(skipErased)
  {
  }

  inline void operator()( OdDbAssocArrayItemPtr &Item) const
  {
    if ( !Item->isErased() || (Item->isErased() && !m_skipErased) )
    {
      m_indices.append( Item->locator() );
    }
  }
};

OdDbImpValueParam::OdDbImpValueParam()
  : m_UnitType(OdValue::kUnitless)
{
  m_value = OdDbEvalVariant::init( int() );
  m_InputVariablesIds.push_back(OdDbObjectId::kNull);
}

OdDbImpValueParam::~OdDbImpValueParam()
{
}


void OdDbImpValueParam::setValue(const OdDbEvalVariant& Val)
{
  m_value = &Val;
  m_InputVariablesIds.resize(0);
  for (OdDbEvalVariantPtr pEV = m_value; !pEV.isNull(); pEV = pEV->next())
  {
    m_InputVariablesIds.push_back(OdDbObjectId::kNull);
  }
}

OdResult OdDbImpValueParam::dwgInFields(OdDbDwgFiler* pFiler)
{
  OdUInt32 ver = pFiler->rdInt32();
  ODA_ASSERT_ONCE(!ver); // TODO
  if (ver)
    return eMakeMeProxy;

  m_sName = pFiler->rdString();
  ODA_ASSERT_ONCE(!m_sName.isEmpty());

  m_UnitType = (OdValue::UnitType)pFiler->rdInt32();
  OdInt32 nEvalVars = pFiler->rdInt32();
  m_InputVariablesIds.resize(nEvalVars);

  m_value->setNext(NULL);
  OdResult res = m_value->dwgInFields(pFiler);
  if (res != eOk)
    return eMakeMeProxy;
  
  m_InputVariablesIds[0] = pFiler->rdSoftPointerId();

  OdDbEvalVariant* pLast = m_value;
  for (int i = 1; i < nEvalVars; ++i)
  {
    OdDbEvalVariantPtr pNewEvVar = OdDbEvalVariant::createObject();
    if (pNewEvVar->dwgInFields(pFiler) != eOk)
      return eMakeMeProxy;
    pLast->setNext(pNewEvVar);
    pLast = pNewEvVar;
    m_InputVariablesIds[i] = pFiler->rdSoftPointerId();
    ODA_ASSERT_ONCE(m_InputVariablesIds[i].isNull()); // TODO
    }
  m_ControlledObjectDepId = pFiler->rdSoftPointerId();
  ODA_ASSERT_ONCE(m_ControlledObjectDepId.isNull());
  return eOk;
}

void OdDbImpValueParam::dwgOutFields(OdDbDwgFiler* pFiler) const
{
  OdUInt32 ver = 0;
  pFiler->wrInt32(ver);

  pFiler->wrString(m_sName);
  pFiler->wrInt32(m_UnitType);
  OdInt32 nEvalVars = m_InputVariablesIds.size();
  pFiler->wrInt32(nEvalVars);
  OdDbEvalVariantPtr pEvalVar = m_value;
  for (int i = 0; i < nEvalVars; ++i)
  {
    pEvalVar->dwgOutFields(pFiler);
    pEvalVar = pEvalVar->next();
    pFiler->wrSoftPointerId(m_InputVariablesIds[i]);
  }
  pFiler->wrSoftPointerId( m_ControlledObjectDepId);
}

OdResult OdDbImpValueParam::dxfInFields(OdDbDxfFiler* pFiler)
{
  NEXT_CODE(90)
  OdUInt32 ver = pFiler->rdUInt32();
  ODA_ASSERT_ONCE(!ver); // TODO
  if (ver)
    return eMakeMeProxy;

  NEXT_CODE(1)
  pFiler->rdString(m_sName);
  ODA_ASSERT_ONCE(!m_sName.isEmpty());

  NEXT_CODE(90)
  m_UnitType = (OdValue::UnitType)pFiler->rdUInt32();

  NEXT_CODE(90)
  OdInt32 nEvalVars = pFiler->rdUInt32();
  m_InputVariablesIds.resize(nEvalVars);

  m_value->setNext(NULL);
  OdResult res = m_value->dxfInFields(pFiler);
  if (res != eOk)
    return eMakeMeProxy;
  NEXT_CODE(330)
  m_InputVariablesIds[0] = pFiler->rdObjectId();
  ODA_ASSERT_ONCE(m_InputVariablesIds[0].isNull()); // TODO
  OdDbEvalVariant* pLast = m_value;
  
  for (int i = 1; i < nEvalVars; ++i)
  {
    OdDbEvalVariantPtr pNewEvVar = OdDbEvalVariant::createObject();
    if (pNewEvVar->dxfInFields(pFiler) != eOk)
      return eMakeMeProxy;
    pLast->setNext(pNewEvVar);
    pLast = pNewEvVar;
    NEXT_CODE(330)
    m_InputVariablesIds[i] = pFiler->rdObjectId();
    ODA_ASSERT_ONCE(m_InputVariablesIds[i].isNull()); // TODO
  }

  NEXT_CODE(330)
  m_ControlledObjectDepId = pFiler->rdObjectId();
  //ODA_ASSERT_ONCE(!m_ControlledObjectDepId.isNull());
  return eOk;
}

OdStringArray OdDbImpValueParam::compareWith(const OdDbImpValueParam *paramIDToCompare, OdDb::DwgVersion filerVersion /* OdDb::kDHL_CURRENT */) const
{
	OdStringArray retArr;

	if (m_sName.compare(paramIDToCompare->m_sName) != 0)
		retArr.append(CS_PARAM_NAME);

	if (m_UnitType != paramIDToCompare->m_UnitType)
		retArr.append(CS_DIFFERENT_PARAM_CRITICAL);

	if (m_InputVariablesIds.size() != paramIDToCompare->m_InputVariablesIds.size())
		retArr.append(CS_DIFFERENT_PARAM_CRITICAL);

	//m_value TODO comparerWith
	OdUInt32 n = m_InputVariablesIds.size();

	OdUInt32 i = 0;

	for (i = 0; i < n; i++)
	{
		if (!m_InputVariablesIds[i].isNull() && !paramIDToCompare->m_InputVariablesIds[i].isNull())
		{
			if (typeid(*(m_InputVariablesIds[i]).openObject()) != typeid(*(paramIDToCompare->m_InputVariablesIds[i].openObject())))
				retArr.append(CS_DIFFERENT_OBJECTS);
		}
	}

	if (!m_ControlledObjectDepId.isNull() && !paramIDToCompare->m_ControlledObjectDepId.isNull())
	{
		if (typeid(*(m_ControlledObjectDepId).openObject()) != typeid(*(paramIDToCompare->m_ControlledObjectDepId.openObject())))
			retArr.append(CS_DIFFERENT_OBJECTS);
	}

	return retArr;
}

void OdDbImpValueParam::dxfOutFields(OdDbDxfFiler* pFiler) const
{
  OdUInt32 ver = 0;
  pFiler->wrUInt32(90, ver);

  ODA_ASSERT_ONCE(!m_sName.isEmpty());
  pFiler->wrString(1, m_sName);

  pFiler->wrUInt32(90, m_UnitType);
  OdInt32 nEvalVars = m_InputVariablesIds.size();
  pFiler->wrUInt32(90, nEvalVars);
  OdDbEvalVariantPtr pEvalVar = m_value;
  for (int i = 0; i < nEvalVars; ++i)
  {
    pEvalVar->dxfOutFields(pFiler);
    pEvalVar = pEvalVar->next();
    pFiler->wrObjectId(330, m_InputVariablesIds[i]);
  }
  pFiler->wrObjectId(330, m_ControlledObjectDepId);
}

//////////////////////////////////////////////////////////////////////////

OdDbImpAssocActionBody::OdDbImpAssocActionBody()
{
  //m_undefined1 = -1; Todo
  m_dwgVersion = 2;// version?
}

OdDbImpAssocActionBody::~OdDbImpAssocActionBody()
{
}

OdResult OdDbImpAssocActionBody::dwgInFields(OdDbDwgFiler* pFiler)
{
  m_dwgVersion = pFiler->rdInt32();
  // ver
  if (m_dwgVersion != (pFiler->dwgVersion() > OdDb::vAC24 ? 2 : 1))
  {
    ODA_FAIL_ONCE();
    return eMakeMeProxy;
  }

  return eOk;
}

void OdDbImpAssocActionBody::dwgOutFields(OdDbDwgFiler* pFiler) const
{
  pFiler->wrInt32(pFiler->dwgVersion() > OdDb::vAC24 ? 2 : 1);
}

OdResult OdDbImpAssocActionBody::dxfInFields(OdDbDxfFiler* pFiler)
{
  if (!pFiler->atSubclassData(OdDbAssocActionBody::desc()->name()))
  {
    ODA_FAIL_ONCE();
    return eMakeMeProxy;
  }

  NEXT_CODE(90)
  m_dwgVersion = pFiler->rdUInt32();

  if (m_dwgVersion != (pFiler->dwgVersion() > OdDb::vAC24 ? 2 : 1))
  {
    ODA_FAIL_ONCE();
    return eMakeMeProxy;
  }

  ODA_ASSERT_ONCE(pFiler->atEOF()); // TODO
  return eOk;
}

void OdDbImpAssocActionBody::dxfOutFields(OdDbDxfFiler* pFiler) const
{
  pFiler->wrSubclassMarker(OdDbAssocActionBody::desc()->name());
  pFiler->wrUInt32(90, pFiler->dwgVersion() > OdDb::vAC24 ? 2 : 1);
}

OdStringArray OdDbImpAssocActionBody::compareWith(OdDbImpAssocActionBody *comparedBody, OdDb::DwgVersion filerVersion) const
{
  OdStringArray retArr;

  ODA_ASSERT(comparedBody != NULL);

  if (comparedBody != NULL)
  {
    if (m_dwgVersion != comparedBody->m_dwgVersion)
      retArr.append(CS_DIFFERENT_PARAM_CRITICAL);
  }

  return retArr;
}

OdResult OdDbImpAssocActionBody::createActionAndActionBodyAndPostToDatabase( 
          OdRxClass* pActionBodyClass, 
          const OdDbObjectId& objectId, 
          OdDbObjectId& createdActionId, 
          OdDbObjectId& createdActionBodyId)
{
  OdResult res;
  OdDbAssocNetworkPtr pAssocNetwork = OdDbAssocNetwork::cast( objectId.openObject( OdDb::kForWrite  ) );
  OdDbAssocActionPtr pAction = OdDbAssocAction::createObject();

  pAssocNetwork->database()->addOdDbObject( pAction, objectId );
  createdActionId = pAction->objectId();
  res = pAssocNetwork->addAction(createdActionId, true);

  if (eOk != res)
    return res;
  //OdDbAssocArrayActionBodyPtr pActionBody = OdDbAssocArrayActionBody::cast(pActionBodyClass->create());
  OdDbAssocActionBodyPtr pActionBody = OdDbAssocActionBody::cast(pActionBodyClass->create());

  if (!pActionBody.isNull())
  {
    pAssocNetwork->database()->addOdDbObject(pActionBody, createdActionId);
    createdActionBodyId = pActionBody->objectId();
  }

  return eOk;
}

static OdDbObjectId getIdDepArrayEntity(const OdDbObjectIdArray& depIDs);

OdDbObjectId OdDbImpAssocActionBody::getArrayEntity(OdDbObjectId parentActionId) const
{
  OdDbObjectIdArray objIDs;
  OdDbAssocActionPtr pAction = parentActionId.safeOpenObject(OdDb::kForRead);
  pAction->getDependencies(false, true, objIDs);
  if(objIDs.empty())
    return OdDbObjectId::kNull;

  OdDbAssocDependencyPtr dep = objIDs.first().openObject();
  if(dep.isNull())
    return OdDbObjectId::kNull;
  return dep->dependentOnObject();
}

OdResult OdDbImpAssocActionBody::dependentObjectClonedOverride(OdDbAssocActionBody* thisBody, const OdDbAssocDependency* dependency, const OdDbObject* original, const OdDbObject* clone) {
  return eNotImplemented;
}

OdDbEntityPtr OdDbImpAssocActionBody::checkAccompanying(OdDbObjectId parentActionId, OdDbAssocAction* pAction)
{
  OdDbObjectPtr obj = getArrayEntity(parentActionId).openObject(OdDb::kForWrite);
  if(obj.isNull()) {
    pAction->setStatus(kErasedAssocStatus, false);
    return OdDbEntityPtr();
  }
  OdDbEntityPtr bRef = OdDbEntity::cast(obj);
  if (bRef.isNull())
    pAction->setStatus(kFailedToEvaluateAssocStatus, false);
  return bRef;
}

//void OdDbImpAssocActionBody::transformPositionByViewPort(OdDbViewportPtr& pViewport, OdGePoint3d& blPosition)
//{
//  if (!pViewport.isNull())
//  {
//    if (pViewport->viewCenter() != OdGePoint2d(0, 0))
//    {
//      ODA_ASSERT(!(pViewport->viewTarget() != OdGePoint3d(0, 0, 0)));
//      blPosition.x = blPosition.x - pViewport->viewCenter().x;
//      blPosition.y = blPosition.y - pViewport->viewCenter().y;
//    }
//    else
//    {
//      ODA_ASSERT(pViewport->viewTarget() != OdGePoint3d(0, 0, 0));
//      blPosition.x = blPosition.x - pViewport->viewTarget().x;
//      blPosition.y = blPosition.y - pViewport->viewTarget().y;
//    }
//
//    blPosition *= pViewport->customScale();
//    blPosition.x = blPosition.x + pViewport->centerPoint().x;
//    blPosition.y = blPosition.y + pViewport->centerPoint().y;
//  }
//}
//////////////////////////////////////////////////////////////////////////

OdDbImpAssocParamBasedActionBody::OdDbImpAssocParamBasedActionBody()
{
  m_undefined1 = 0;
  m_undefined2 = 0;
  m_undefined3 = 0;
  m_undefined4 = 0;//TODO
  m_assocDependencyID = NULL;
  m_notParent = false;
}

OdDbImpAssocParamBasedActionBody::OdDbImpAssocParamBasedActionBody(bool notParent)
{
	m_undefined1 = 0;
	m_undefined2 = 0;
	m_undefined3 = 0;
	m_undefined4 = 0;//TODO
	m_assocDependencyID = NULL;
	m_notParent = notParent;
}

OdDbImpAssocParamBasedActionBody::~OdDbImpAssocParamBasedActionBody()
{
}

OdResult OdDbImpAssocParamBasedActionBody::dwgInFields(OdDbDwgFiler* pFiler)
{
	m_arrParams.clear();
	m_arrValueParams.clear();

	//OdResult res = OdDbImpAssocActionBody::dwgInFields(pFiler);
	//if (res != eOk)
	//	return res;

  if (pFiler->dwgVersion() > OdDb::vAC24)
    return eOk;

  m_undefined1 = pFiler->rdInt32();
  ODA_ASSERT_ONCE(!m_undefined1); // TODO
  if (m_undefined1)
    return eMakeMeProxy;

  m_undefined2 = pFiler->rdInt32();
  //if (m_undefined2) .. TODO
  //{
  //  ODA_FAIL_ONCE(); // TODO
  //  return eMakeMeProxy;
  //}

  OdUInt32 cnt = pFiler->rdInt32();
  //OdDbHandle h;
  while (cnt-- > 0) 
  {
    m_arrParams.push_back(pFiler->rdHardOwnershipId());
	//h = m_arrParams[0].getHandle();
  }

  m_undefined3 = pFiler->rdInt32();
  if (m_undefined3)
  {
    ODA_FAIL_ONCE(); // TODO
    return eMakeMeProxy;
  }

  cnt = pFiler->rdInt32();

  if (cnt == 0 && m_notParent)
  {
    m_undefined4 = pFiler->rdInt32();
	m_assocDependencyID = pFiler->rdSoftPointerId();
  }

  while (cnt-- > 0) 
  {
    OdDbImpValueParamPtr pValueParam = new OdDbImpValueParam(); 
    OdResult res = pValueParam->dwgInFields(pFiler);
    if (res != eOk)
      return res;
    m_arrValueParams.push_back(pValueParam);
  }

  return eOk;
}

void OdDbImpAssocParamBasedActionBody::dwgOutFields(OdDbDwgFiler* pFiler) const
{
  //OdDbImpAssocActionBody::dwgOutFields(pFiler);

  if (pFiler->dwgVersion() > OdDb::vAC24)
    return;

  pFiler->wrInt32(m_undefined1);
  pFiler->wrInt32(m_undefined2);

  OdUInt32 cnt = m_arrParams.size();
  pFiler->wrInt32(cnt);
  OdUInt32 idx = 0;
  for (; idx < cnt; idx++)
    pFiler->wrHardOwnershipId(m_arrParams[idx]);

  pFiler->wrInt32(m_undefined3);

  cnt = m_arrValueParams.size();
  pFiler->wrInt32(cnt);

  if (cnt == 0 && m_notParent)
  {
    pFiler->wrInt32(m_undefined4);
	pFiler->wrSoftPointerId(m_assocDependencyID);
  }

  for (idx = 0; idx < cnt; idx++)
  {
    OdDbImpValueParamPtr pValueParam = m_arrValueParams[idx]; 
    pValueParam->dwgOutFields(pFiler);
  }
}

OdResult OdDbImpAssocParamBasedActionBody::dxfInFields(OdDbDxfFiler* pFiler)
{
	m_arrParams.clear();
	m_arrValueParams.clear();

	//OdResult res = OdDbImpAssocActionBody::dxfInFields(pFiler);

	//if (res != eOk)
	//	return res;

	if (pFiler->dwgVersion() > OdDb::vAC24)	
		return eOk;

  if (!pFiler->atSubclassData(OdDbAssocParamBasedActionBody::desc()->name()))
  {
    ODA_FAIL_ONCE();
    return eMakeMeProxy;
  }

  NEXT_CODE(90)
  m_undefined1 = pFiler->rdUInt32();
  ODA_ASSERT_ONCE(!m_undefined1); // TODO
  if (m_undefined1)
    return eMakeMeProxy;

  NEXT_CODE(90)
  m_undefined2 = pFiler->rdUInt32();
  if (m_undefined2)
  {
    ODA_FAIL_ONCE(); // TODO
    return eMakeMeProxy;
  }

  NEXT_CODE(90)
  OdUInt32 cnt = pFiler->rdUInt32();
  while (cnt-- > 0) 
  {
    NEXT_CODE(360)
    m_arrParams.push_back(pFiler->rdObjectId());
  }

  NEXT_CODE(90)
  m_undefined3 = pFiler->rdUInt32();
  
  if (m_undefined3)
  {
    ODA_FAIL_ONCE(); // TODO
    return eMakeMeProxy;
  }

  NEXT_CODE(90)
  cnt = pFiler->rdUInt32();
  // see also OdDbImpAssocAction::dxfInFields R27
  if (cnt == 0 && m_notParent)
  {
	NEXT_CODE(90);
	m_undefined4 = pFiler->rdUInt32();
	NEXT_CODE(330)
	m_assocDependencyID = pFiler->rdObjectId();
  }

  while (cnt-- > 0) 
  {
    OdDbImpValueParamPtr pValueParam = new OdDbImpValueParam(); 
    OdResult res = pValueParam->dxfInFields(pFiler);
    if (res != eOk)
      return res;
    m_arrValueParams.push_back(pValueParam);
  }

  ODA_ASSERT_ONCE(pFiler->atEOF()); // TODO
  return eOk;
}

void OdDbImpAssocParamBasedActionBody::dxfOutFields(OdDbDxfFiler* pFiler) const
{
  //OdDbImpAssocActionBody::dxfOutFields(pFiler);
  
  if (pFiler->dwgVersion() > OdDb::vAC24)
    return;

  pFiler->wrSubclassMarker(OdDbAssocParamBasedActionBody::desc()->name());

  pFiler->wrUInt32(90, m_undefined1);

  pFiler->wrUInt32(90, m_undefined2);

  OdUInt32 cnt = m_arrParams.size();
  pFiler->wrUInt32(90, cnt);
  OdUInt32 idx = 0;
  for (; idx < cnt; idx++)
    pFiler->wrObjectId(360, m_arrParams[idx]);

  pFiler->wrUInt32(90, m_undefined3);

  cnt = m_arrValueParams.size();
  pFiler->wrUInt32(90, cnt);

  if (cnt == 0 && m_notParent)
  {
    pFiler->wrUInt32(90, m_undefined4);
	pFiler->wrObjectId(330, m_assocDependencyID);
  }

  for (idx = 0; idx < cnt; idx++)
  {
    OdDbImpValueParamPtr pValueParam = m_arrValueParams[idx]; 
    pValueParam->dxfOutFields(pFiler);
  }
}

OdStringArray  OdDbImpAssocParamBasedActionBody::compareWith(OdDbImpAssocActionBody *bodyIDToCompare, OdDb::DwgVersion filerVersion /*OdDb::kDHL_CURRENT*/) const
{
  OdStringArray retArr;
  OdDbImpAssocParamBasedActionBody* comparedBody = static_cast<OdDbImpAssocParamBasedActionBody *>(bodyIDToCompare);
  ODA_ASSERT(comparedBody != NULL);

  if (comparedBody != NULL && m_dwgVersion <= OdDb::vAC24)
  {
    /*if (m_udef1 != comparedBody->m_udef1)
      retArr.append(CS_DIFFERENT_PARAM_CRITICAL);

    if (typeid(*(m_surfaceObjectDepId.openObject())) != typeid(*(comparedBody->m_surfaceObjectDepId.openObject())))
      retArr.append(CS_DIFFERENT_OBJECTS);

    if (m_boolUndef1 != comparedBody->m_boolUndef1)
      retArr.append(CS_DIFFERENT_PARAM_CRITICAL);

    if (m_udef2 != comparedBody->m_udef2)
      retArr.append(CS_NOT_CRITICAL);

    if (m_boolUndef2 != comparedBody->m_boolUndef2)
      retArr.append(CS_DIFFERENT_PARAM_CRITICAL);

    if (m_udefInt16_1 != comparedBody->m_udefInt16_1)
      retArr.append(CS_NOT_CRITICAL);*/
  }
  return retArr;
}
//////////////////////////////////////////////////////////////////////////

OdDbImpAssocArrayActionBody::OdDbImpAssocArrayActionBody()
{
}

OdDbImpAssocArrayActionBody::~OdDbImpAssocArrayActionBody()
{
}

OdResult OdDbImpAssocArrayActionBody::dwgInFields(OdDbDwgFiler* pFiler)
{
  OdResult res = OdDbImpAssocActionBody::dwgInFields(pFiler);
  if (res != eOk)
    return res;
  
  res = OdDbImpAssocParamBasedActionBody::dwgInFields(pFiler);
  if (res != eOk)
    return res;

  int ver = pFiler->rdInt32();
  ODA_ASSERT_ONCE(ver == 1); // TODO
  if (ver != 1)
    return eMakeMeProxy;

  OdString sClass = pFiler->rdString(); // = L"AcDbAssocArrayRectangularParameters"
  ODA_ASSERT_ONCE(!sClass.isEmpty()); // TODO
  if (sClass.isEmpty())
    return eMakeMeProxy;

  OdRxObjectPtr pObj = ::odrxCreateObject(sClass);
  m_pParams = OdDbAssocArrayParameters::cast(pObj);
  ODA_ASSERT_ONCE(m_pParams.get());
  if (!m_pParams.get())
    return eMakeMeProxy;

  res = m_pParams->dwgInFields(pFiler);
  if ( eOk != res )
    return res;

  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 4; j++)
      m_matrix.entry[i][j] = pFiler->rdDouble();
  }

  return res;
}

void OdDbImpAssocArrayActionBody::dwgOutFields(OdDbDwgFiler* pFiler) const
{
  OdDbImpAssocActionBody::dwgOutFields(pFiler);
  OdDbImpAssocParamBasedActionBody::dwgOutFields(pFiler);

  int ver = 1;
  pFiler->wrInt32(ver);

  if (m_pParams.get())
  {
    OdString sClass = m_pParams->isA()->name();
    ODA_ASSERT_ONCE(sClass == L"AcDbAssocArrayPathParameters"
      || sClass == L"AcDbAssocArrayPolarParameters"
      || sClass == L"AcDbAssocArrayRectangularParameters"
      || sClass == L"AcDbAssocArrayModifyParameters");
    pFiler->wrString(sClass);

    m_pParams->dwgOutFields(pFiler);
  }

  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 4; j++)
      pFiler->wrDouble(m_matrix.entry[i][j]);
  }
}

OdResult OdDbImpAssocArrayActionBody::dxfInFields(OdDbDxfFiler* pFiler)
{
  OdResult res = OdDbImpAssocActionBody::dxfInFields(pFiler);
  if (res != eOk)
    return res;

  res = OdDbImpAssocParamBasedActionBody::dxfInFields(pFiler);

  if (res != eOk)
    return res;

  if (!pFiler->atSubclassData(OdDbAssocArrayActionBody::desc()->name()))
  {
    ODA_FAIL_ONCE();
    return eMakeMeProxy;
  }
  
  NEXT_CODE(90)
  OdUInt32 ver = pFiler->rdUInt32();
  if (ver != 1)
  {
    ODA_FAIL_ONCE(); // TODO
    return eMakeMeProxy;
  }

  NEXT_CODE(1)
  OdString sClass = pFiler->rdString();
  if (sClass.isEmpty())
  {
    ODA_FAIL_ONCE(); // TODO
    return eMakeMeProxy;
  }
  ODA_ASSERT_ONCE(   sClass == L"AcDbAssocArrayPathParameters"
                  || sClass == L"AcDbAssocArrayPolarParameters"
                  || sClass == L"AcDbAssocArrayRectangularParameters"
                  || sClass == L"AcDbAssocArrayModifyParameters");

  OdRxObjectPtr pObj = ::odrxCreateObject(sClass);
  m_pParams = OdDbAssocArrayParameters::cast(pObj);
  if (!m_pParams.get())
  {
    ODA_FAIL_ONCE();
    return eMakeMeProxy;
  }
  
  res = m_pParams->dxfInFields(pFiler);
  if (res != eOk)
    return res;

  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      NEXT_CODE(40)
        m_matrix.entry[i][j] = pFiler->rdDouble();
    }
  }

  ODA_ASSERT_ONCE(pFiler->atEOF()); // TODO
  return eOk;
}

void OdDbImpAssocArrayActionBody::dxfOutFields(OdDbDxfFiler* pFiler) const
{
  OdDbImpAssocActionBody::dxfOutFields(pFiler);

  //if (pFiler->dwgVersion() <= OdDb::vAC24)
  //  pFiler->wrSubclassMarker(OdDbAssocParamBasedActionBody::desc()->name());

  OdDbImpAssocParamBasedActionBody::dxfOutFields(pFiler);
  
  pFiler->wrSubclassMarker(OdDbAssocArrayActionBody::desc()->name());

  OdUInt32 ver = 1;
  pFiler->wrUInt32(90, ver);

  if (!m_pParams.get())
  {
    ODA_FAIL_ONCE();
    throw OdError(eNotApplicable);
  }
  OdString sClass = m_pParams->isA()->name();
  ODA_ASSERT_ONCE(   sClass == L"AcDbAssocArrayPathParameters"
                  || sClass == L"AcDbAssocArrayPolarParameters"
                  || sClass == L"AcDbAssocArrayRectangularParameters"
                  || sClass == L"AcDbAssocArrayModifyParameters");
  pFiler->wrString(1, sClass);

  m_pParams->dxfOutFields(pFiler);

  // pFiler->wrMatrix3d(m_matrix) :
  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      pFiler->wrDouble(40, m_matrix.entry[i][j]);
    }
  }
}

void OdDbImpAssocArrayActionBody::composeForLoad(OdDbObject *pObject, 
                                                  OdDb::SaveType format, 
                                                  OdDb::DwgVersion version, 
                                                  OdDbAuditInfo* pAuditInfo )
{
  OdDbAssocArrayActionBody *Body = static_cast<OdDbAssocArrayActionBody *>(pObject);
  
  if (m_pParams->setOwner(Body) == eOk)
  {
    m_pParams->composeForLoad(format, version, pAuditInfo);
    OdDbObjectId arrayEntID = getArrayEntity(Body->parentAction());

    OdDbBlockReferencePtr arrayEnt = OdDbBlockReference::cast(arrayEntID.openObject());
    if (arrayEnt.get())
      m_matrix = arrayEnt->blockTransform();
  }
}

OdResult OdDbImpAssocArrayActionBody::deleteItem( const OdDbItemLocator& index, bool bErase )
{
  OdResult res = eOk;
  OdDbAssocArrayItemPtr pItem;

  if ( eOk != (res = getItem(pItem, index)) )
    return res;

  pItem->setErased( bErase );
  return eOk;
}

const OdDbAssocArrayParametersPtr OdDbImpAssocArrayActionBody::parameters() const
{
  return m_pParams;
}

OdDbAssocArrayParametersPtr& OdDbImpAssocArrayActionBody::parameters()
{
  return m_pParams;
}

OdResult OdDbImpAssocArrayActionBody::transformItemBy( const OdDbItemLocator& index, 
                                                       const OdGeMatrix3d& xform )
{
  OdResult res = eOk;
  OdDbAssocArrayItemPtr pItem;

  if ( eOk != (res = getItem(pItem, index)) )
    return res;

  pItem->transformBy( xform );

  return res;
}

void OdDbImpAssocArrayActionBody::getItems( OdArray<OdDbItemLocator>& indices, bool skipErased ) const
{
  OdDbImpAssocArrayParameters *pParamImpl = OdDbImpAssocArrayParameters::getImpl( m_pParams );

  std::for_each( pParamImpl->m_items.begin(), pParamImpl->m_items.end(), getIndices(indices, skipErased) );
}

OdResult OdDbImpAssocArrayActionBody::getItem( OdDbAssocArrayItemPtr &pItem, 
                                               const OdDbItemLocator& index ) const
{
  OdDbImpAssocArrayParameters *pParamImpl = OdDbImpAssocArrayParameters::getImpl( m_pParams );

  return pParamImpl->GetItemByLocator( index, pItem );
}

const OdDbAssocArrayItem* OdDbImpAssocArrayActionBody::getItemAt( const OdDbItemLocator& index, 
                                                                  OdDbFullSubentPath& path, 
                                                                  OdDbObjectId arrayId ) const
{
  OdDbAssocArrayItemPtr pItem;

  if ( eOk != getItem(pItem, index) )
    return pItem;

  path.objectIds().clear();
  
  path.objectIds().append( arrayId );

  OdDbBlockReferencePtr pArrayBlock = OdDbBlockReference::cast( arrayId.openObject( OdDb::kForRead ) );

  path.objectIds().insertAt( 0, pArrayBlock->ownerId() ); //model or papper space Id  

  path.objectIds().append( pItem->entityProvider() );

  return pItem;
}

const OdDbAssocArrayItem* OdDbImpAssocArrayActionBody::getItemAt( const OdDbFullSubentPath& path ) const
{
  OdDbImpAssocArrayParameters *pParamImpl = OdDbImpAssocArrayParameters::getImpl( m_pParams );

  OdArray<OdDbAssocArrayItemPtr>::iterator it = std::find_if( pParamImpl->m_items.begin(), 
                                                              pParamImpl->m_items.end(), 
                                                              findItemBySubPath(path) ); 
  if (pParamImpl->m_items.end() == it)
    return NULL;
  else
    return it->get();
}

static bool isArrayFillEntirePath(const OdDbAssocArrayPathParameters* pPathParam)
{
  ODA_ASSERT(pPathParam);
  OdDbEvalVariantPtr value = OdDbEvalVariant::init(double());
  OdString expression, evaluatorId;
  if (pPathParam && pPathParam->owner()->getValueParam(OD_T("FillPath"), *value.get(), expression, evaluatorId) == eOk)
    return !OdZero(value->getAsDouble());
  return false; // CORE-17203 // return true;
}

static bool recalculateCountOfItemsOnPath(OdDbAssocArrayPathParameters* pPathParam, double pathLength, bool isClosed)
{
  ODA_ASSERT(pPathParam);
  if (pPathParam->method() == OdDbAssocArrayPathParameters::kMeasure)
  {
    int maxItems = int(pathLength / pPathParam->itemSpacing()) + 1;
    if (isArrayFillEntirePath(pPathParam))
    {
      if (isClosed && maxItems > 1)
        maxItems--;
      if (pPathParam->itemCount() != maxItems && pPathParam->setItemCount(maxItems) == eOk)
        return true;
    }
    else
    {
      int n = pPathParam->itemCount();
      if (pPathParam->itemSpacing() * ((n > 1) ? n - 1 : 1) > pathLength)
      {
        if (pPathParam->setItemCount(maxItems) == eOk)
          return true;
      }
    }
  }
  else
  {
    int items = pPathParam->itemCount();
    double spacing = pathLength / ((items > 1) ? (isClosed ? items : items - 1) : 1);
    pPathParam->setItemSpacing(spacing);
    return true;
  }
  return false;
}


void OdDbImpAssocArrayActionBody::evaluateOverride( OdDbObjectId parentActionId )
{
  OdDbAssocActionPtr pAction = OdDbAssocAction::cast( parentActionId.safeOpenObject(OdDb::kForWrite) );

  m_pParams->setOwner( OdDbAssocArrayActionBodyPtr( pAction->actionBody().safeOpenObject() ) );

  //OdDbEntityPtr bEntRef = checkAccompanying(parentActionId, pAction);

  //if (bEntRef.isNull())
  //  return;

  OdDbObjectIdArray objIDs;
  
  //Array entity dependency check
  //BEGIN
  pAction->getDependencies( true, true, objIDs );

  OdDbObjectId idDepEntity = getIdDepArrayEntity(objIDs);
  if (idDepEntity.isNull())
    return;
  OdDbAssocDependencyPtr pDepArrayEntity = idDepEntity.openObject( OdDb::kForWrite );

  bool arrayIsErased = (kErasedAssocStatus == pAction->status() );
  bool arrayEntityChanged = false;
  OdGeMatrix3d prevArrayTransform = m_matrix;

  switch ( pDepArrayEntity->status() )
  {
  case kErasedAssocStatus:
    arrayIsErased = true;
    
    break;

  case kChangedDirectlyAssocStatus:
    {
      m_matrix = OdDbBlockReference::cast( getArrayEntity( parentActionId ).safeOpenObject() )
                                               ->blockTransform();
      if ( !m_matrix.isEqualTo( prevArrayTransform ) )
      {
        arrayEntityChanged = true;
      }
    }
    // No break; CORE-12336

  case kIsUpToDateAssocStatus:
    {
      OdDbBlockReferencePtr pArrayEnt = getArrayEntity( parentActionId ).safeOpenObject( OdDb::kForWrite );
      pArrayEnt->setBlockTransform( m_matrix );
      pArrayEnt->downgradeOpen();
    }
    break;
  }
  //END

  OdDbAssocArrayRectangularParametersPtr pRectParam = OdDbAssocArrayRectangularParameters::cast( m_pParams );
  OdDbAssocArrayPolarParametersPtr pPolarParam = OdDbAssocArrayPolarParameters::cast( m_pParams );
  OdDbAssocArrayPathParametersPtr pPathParam = OdDbAssocArrayPathParameters::cast( m_pParams );

  //READ dependency check
  //BEGIN
  objIDs.clear();
  pAction->getDependencies( true, false, objIDs );
  ODA_ASSERT_ONCE( objIDs.length() );

  OdDbAssocDependencyPtr pDepSrcBTR;

  if ( !pRectParam.isNull() 
    || !pPolarParam.isNull() )
  {        
    pDepSrcBTR = OdDbAssocDependency::cast( objIDs.first().openObject( OdDb::kForWrite ) );
  }

  if ( !pPathParam.isNull() )
  {
    switch ( objIDs.length() )
    {
    case 3:
      {
      pDepSrcBTR = OdDbAssocDependency::cast( objIDs[1].openObject( OdDb::kForWrite ) );      

      OdDbAssocDependencyPtr pDepPath = OdDbAssocDependency::cast( objIDs[0].openObject( OdDb::kForWrite ) );

      if (arrayIsErased)
      {
        pDepPath->detachFromObject();
        break;
      }

      switch ( pDepPath->status() )
      {

      case kIsUpToDateAssocStatus:
        // WARNING: no break if arrayEntityChanged == true
        if ( !arrayEntityChanged )
        {
          break;
        }
        else
        {
          //TODO request for termination of associativity ??
        }

      case kErasedAssocStatus:
        pAction->removeDependency( pDepPath->objectId(), true );
        {
          OdDbAssocActionParamPtr pParam = 
            pAction->paramAtName(OD_T("RowProfile")).openObject( OdDb::kForWrite );
          pParam->detachDependencies();
        }
        break;

      case kChangedDirectlyAssocStatus:
        {
        if ( arrayEntityChanged )
        {
          m_matrix = prevArrayTransform;
          OdDbBlockReferencePtr pArrayEnt = getArrayEntity( parentActionId ).safeOpenObject( OdDb::kForWrite );
          pArrayEnt->setBlockTransform( m_matrix );
          pArrayEnt->downgradeOpen();
        }

        OdString rowProfileName = OD_T("RowProfile");
        OdDbGeomRefPtr pGeomRef;
        OdDbObjectId paramId;

        OdResult res = pPathParam->getGeomParam( rowProfileName, pGeomRef, paramId );
        if ( eOk != res )
        {
          ODA_FAIL_ONCE();
          return;
        }

        OdDbCurvePtr pCurve = OdDbCurve::cast( pDepPath->dependentOnObject().openObject( OdDb::kForRead ) );
        if ( pCurve.isNull() )
        {
          ODA_FAIL_ONCE();
          return;
        }

        OdGeCurve3d *pGeCurve = NULL;
        pCurve->getOdGeCurve( pGeCurve );
        if (!pGeCurve)
        {   // curve degenerated to zero length. Erase assoc array.
            pDepPath->detachFromObject();
            
            OdDbBlockReferencePtr pArrayEnt = getArrayEntity(parentActionId).safeOpenObject(OdDb::kForWrite);
            pArrayEnt->erase();
            pArrayEnt->downgradeOpen();

            arrayIsErased = true;
            break;
        }
        OdDbAssocEdgeActionParamPtr pActionParam = 
          OdDbAssocEdgeActionParam::cast( paramId.openObject( OdDb::kForWrite ) );
        pActionParam->setEdgeSubentityGeometry( pGeCurve );

        pGeCurve->transformBy(m_matrix); // is it necessary?
        OdDbImpAssocArrayPathParameters* pImpParam = static_cast<OdDbImpAssocArrayPathParameters *>(pPathParam->m_pImpl);
        if (!pImpParam->pathDirection())
          pGeCurve->reverseParam();
        
        double pathLength = pGeCurve->length();
        bool isClosed = pGeCurve->isClosed();
      
        if (!OdZero(pathLength))
        {
          pathLength -= pPathParam->startOffset();
          if (isClosed)
            isClosed = OdZero(pPathParam->startOffset());

          if (pPathParam->method() == OdDbAssocArrayPathParameters::kDivide)
          {
            pathLength -= pPathParam->endOffset();
            if (isClosed)
              isClosed = OdZero(pPathParam->endOffset());
          }
          if(pathLength > 0.)
              recalculateCountOfItemsOnPath(pPathParam, pathLength, isClosed);
        }



        delete pGeCurve;
        }
        break;
      }

      }
      break;

    case 2:
      pDepSrcBTR = OdDbAssocDependency::cast( objIDs[0].openObject( OdDb::kForWrite ) );
      break;

    default:
      ODA_ASSERT_ONCE( !"pPathParam number of read dependency > 3" );
      return;
    }
  }    
  //END

  if ( arrayIsErased ) {
    // AutoCAD does not erase  the blocks. They will be purged after file save / load or by Purge operation
    pAction->setStatus( kErasedAssocStatus );
    return;
  }
  
  OdDbBlockReferencePtr pArrayEnt = getArrayEntity(parentActionId).openObject();
  if(pArrayEnt.isNull()) {
    pAction->setStatus(kErasedAssocStatus);
    return;
  }

  OdDbBlockTableRecordPtr sourceBTR =
    OdDbBlockTableRecord::cast(pDepSrcBTR->dependentOnObject().openObject(OdDb::kForRead));
  if (sourceBTR.isNull()) {
    pAction->setStatus(kFailedToEvaluateAssocStatus);
    return;
  }

  objIDs.clear();
  pAction->getDependencies( false, true, objIDs );
  ODA_ASSERT_ONCE( 2 == objIDs.length() );

  OdDbAssocDependencyPtr pDepDstBTR = OdDbAssocDependency::cast( objIDs[0].openObject( OdDb::kForWrite ) );
  OdDbBlockTableRecordPtr destinationBTR = 
    OdDbBlockTableRecord::cast( pDepDstBTR->dependentOnObject().openObject( OdDb::kForWrite ) );
  if(destinationBTR.isNull()) {
    pAction->setStatus(kFailedToEvaluateAssocStatus);
    return;
  }

  OdArray <OdDbAssocArrayItemPtr> pItems;
  if (m_pParams->getItems( pItems ) != eOk)
  {
    pAction->setStatus(kFailedToEvaluateAssocStatus);
    return;
  }

  OdGeMatrix3d matOrigin;
  OdDbGeomRefPtr Origin;  
  OdDbObjectId objID;
  OdResult es = m_pParams->getGeomParam(L"Origin", Origin, objID);
  if (es == eOk)
  {
    OdDbVertexRefPtr pOrigin = OdDbVertexRef::cast(Origin);
    ODA_ASSERT_ONCE_X(ASSOC, pOrigin.get());
    OdGePoint3d ptOrigin = pOrigin->point();
    if (   ptOrigin != OdGePoint3d::kOrigin 
        && pPolarParam.isNull()) // CORE-15158 problem with AssocArrayTests_AS 72
      matOrigin.setToTranslation(ptOrigin.asVector()); // CORE-15158 subtest createRectArrayAssoc_CORE15158
  }
  ODA_ASSERT_VAR(else)
    ODA_ASSERT_ONCE_X(ASSOC, es == eNotInGroup);

  OdGeMatrix3d mat;

  OdDbBlockReferencePtr pBlkRef;
 
  const OdDbObjectId& idLayer = pArrayEnt->layerId();

  OdArray<OdDbAssocArrayItemPtr>::iterator itItem;
  for (itItem = pItems.begin(); itItem != pItems.end(); itItem++)
  {   
    if ( (*itItem)->isErased() )
    {
      pBlkRef = OdDbBlockReference::cast( (*itItem)->entityProvider().openObject( OdDb::kForWrite ) );

      if (pBlkRef.isNull())
        continue;

      pBlkRef->erase( true );

      continue;
    }

    if ( (*itItem)->entityProvider().isNull() || (*itItem)->entityProvider().isErased() )
    {
      pBlkRef = OdDbBlockReference::createObject();
      pBlkRef->setColor(OdCmEntityColor::kByBlock);
      pBlkRef->setLineWeight(OdDb::kLnWtByBlock);
      pBlkRef->setPlotStyleName(OdDb::kPlotStyleNameByBlock);
      pBlkRef->setTransparency(OdCmTransparency(OdCmTransparency::kByBlock));
      pBlkRef->setLinetype(destinationBTR->database()->getLinetypeByBlockId());
      pBlkRef->setMaterial(destinationBTR->database()->byBlockMaterialId());
      pBlkRef->setBlockTableRecord( sourceBTR->objectId() );

      OdDbObjectId blkRefId = destinationBTR->appendOdDbEntity(pBlkRef);
      (*itItem)->setEntityProvider( blkRefId );
    }
    else
    {
      pBlkRef = OdDbBlockReference::cast( (*itItem)->entityProvider().safeOpenObject( OdDb::kForWrite ) );
      pBlkRef->setBlockTableRecord( sourceBTR->objectId() );
    }
    if (idLayer != pBlkRef->layerId())
      pBlkRef->setLayer(idLayer); // CORE-15709

    (*itItem)->getTransform( mat );
    ODA_ASSERT_VAR(if (matOrigin != OdGeMatrix3d::kIdentity))
      mat *= matOrigin; // CORE-15158 subtest createRectArrayAssoc_CORE15158
    pBlkRef->setBlockTransform( mat );
  }

  destinationBTR->disableUndoRecording( true );
  destinationBTR->assertWriteEnabled( false, true );
  destinationBTR->disableUndoRecording( false );
  destinationBTR->downgradeOpen();

  pAction->evaluateDependencies();
  pAction->setStatus( kIsUpToDateAssocStatus );
}

//enum AssocArraySkipOdaSpecificFlags {
//  kSkip_None = 0,
//  kSkip_TransformToZero = 1,
//  kSkip_EraseSourceEntities = 2,
//
//  kSkip_Default = kSkip_None
//  // kSkip_Default = kSkip_TransformToZero + kSkip_EraseSourceEntities // TODO
//};
static AssocArraySkipOdaSpecificFlags s_enSkipOdaSpecific = kSkip_Default;
OdUInt32 OdDbAssocArrayActionBody_setSkipNextFlags(OdUInt32 flags) // for tests only
{
  OdUInt32 prev = s_enSkipOdaSpecific;
  s_enSkipOdaSpecific = (AssocArraySkipOdaSpecificFlags) flags;
  return prev;
}

static double radiansToDegree(double rad)
{
  if (OdZero(rad))
    rad = 0.0;
  double ang = OdaToDegree(rad);
  // UI way for property palette
  while (ang < 0.0)
    ang += 360.0;
  while (ang > 360.0)
    ang -= 360.0;
  return ang;
}

static OdGeMatrix3d getMatixToZeroState(const OdDbAssocArrayParametersPtr& pParameters, OdDbObjectId blockId)
{
  OdDbAssocArrayRectangularParametersPtr pRectParam = OdDbAssocArrayRectangularParameters::cast(pParameters);
  OdDbAssocArrayPolarParametersPtr pPolarParam = OdDbAssocArrayPolarParameters::cast(pParameters);
  OdDbAssocArrayPathParametersPtr pPathParam = OdDbAssocArrayPathParameters::cast(pParameters);

  OdGeMatrix3d matExtenceToZeroState;

  if (pRectParam.get() || pPolarParam.get())
  {
    OdDbBlockTableRecordPtr pSourceBTR = OdDbBlockTableRecord::cast(blockId.openObject());
    ODA_ASSERT_ONCE_X(ASSOC, pSourceBTR.get());
    OdGeExtents3d extSourceBTR;
    pSourceBTR->getGeomExtents(extSourceBTR);

    double dItemSize = extSourceBTR.minPoint().distanceTo(extSourceBTR.maxPoint());
    OdGeVector3d vecZeroPointYoExtenceCentr(extSourceBTR.center().x, extSourceBTR.center().y, extSourceBTR.center().z);

    matExtenceToZeroState.setToTranslation(vecZeroPointYoExtenceCentr.negate());
  }
  else if (!pPathParam.isNull())
  {
    OdDbEdgeRef path = pPathParam->path();

    OdGeInterval interval;
    path.curve()->getInterval(interval);

    OdGeVector3dArray deriv;
    OdGePoint3d pointStartPath = path.curve()->evalPoint(interval.lowerBound(), 1, deriv);

    OdGeVector3d vX = blockId.database()->getUCSXDIR(),
                 vY = blockId.database()->getUCSYDIR(),
                 vZ = vX.crossProduct(vY);

    OdGeVector3d vTranslateSrcBlock(pointStartPath.x, pointStartPath.y, pointStartPath.z);
    matExtenceToZeroState = OdGeMatrix3d().setCoordSystem(OdGePoint3d::kOrigin, vX, vY, vZ).invert();

    double ang = OdGeVector3d::kYAxis.angleTo((vZ.crossProduct(deriv.first())).transformBy(matExtenceToZeroState),
      OdGeVector3d::kZAxis);
    matExtenceToZeroState *=   OdGeMatrix3d().rotation(-ang, vZ)
                             * OdGeMatrix3d().setToTranslation(vTranslateSrcBlock.negate());
  }

  return matExtenceToZeroState;
}

// initially was mistakenly integrated into OdDbImpAssocArrayActionBody::createInstance
// see OdDbImpAssocArrayActionBody::createInstance
void odrxAssocArrayAdjustLocation(OdDbObjectId arrayId, const OdDbAssocArrayParametersPtr& pParameters)
{
  OdDbObjectId blockId;
  OdDbBlockReferencePtr pArray;
  OdDbAssocArrayActionBodyPtr pBody = OdDbAssocArrayActionBody::cast(arrayId.openObject());
  if (pBody.get())
  {
    arrayId = pBody->getArrayEntity();
    blockId = pBody->getArraySourceBTR();
  }
  else
  {
    pArray = OdDbBlockReference::cast(arrayId.openObject());
    blockId = pArray->blockTableRecord();
    OdDbBlockTableRecordPtr pDestBlock = OdDbBlockTableRecord::cast(blockId.openObject()); // *U2 with items
    OdDbObjectIteratorPtr pIter = pDestBlock->newIterator();
    ODA_ASSERT_ONCE_X(ASSOC, !pIter->done());
    OdDbBlockReferencePtr pRef = OdDbBlockReference::cast(pIter->entity());
    ODA_ASSERT_ONCE_X(ASSOC, pRef.get());
    blockId = pRef->blockTableRecord(); // *U1 with cloned source entities
  }

  OdDbBlockTableRecordPtr pSourceBTR = OdDbBlockTableRecord::cast(blockId.openObject());
  ODA_ASSERT_ONCE_X(ASSOC, pSourceBTR.get());
  OdString sName = pSourceBTR->getName();
  ODA_ASSERT_ONCE_X(ASSOC, !sName.find(L"*U"));

  OdDbAssocArrayPolarParameters* pPolarParam = OdDbAssocArrayPolarParameters::cast(pParameters).get();
  if (pPolarParam)
  {
    OdResult es;
    OdDbDatabase* pDb = blockId.database();
    OdGeVector3d vX = pDb->getUCSXDIR();
    OdGeVector3d vY = pDb->getUCSYDIR();
    OdGeVector3d vZ = vX.crossProduct(vY);
    
    OdGePoint3d ptBase(0, 0, 0);
    OdDbGeomRefPtr pBasePointRef; OdDbObjectId basePointId;
    OdResult esBase = pParameters->getGeomParam(L"basePoint", pBasePointRef, basePointId);
    OdDbVertexRefPtr pBasePoint = OdDbVertexRef::cast(pBasePointRef);
    if (esBase == eOk && pBasePoint.get() && pBasePoint->impliedType() == OdDbVertexRef::kExplicitVertex)
      ptBase = pBasePoint->point();
    else
      ODA_ASSERT_ONCE_X(ASSOC, esBase == eNotInGroup);

    // center in property palette
    OdGePoint3d ptOrigin(0, 0, 0);
    OdDbGeomRefPtr pOriginRef; OdDbObjectId originId;
    OdResult esOrigin = pParameters->getGeomParam(L"Origin", pOriginRef, originId);
    OdDbVertexRefPtr pOriginPoint = OdDbVertexRef::cast(pOriginRef);
    if (esOrigin == eOk && pOriginPoint.get() && pOriginPoint->impliedType() == OdDbVertexRef::kExplicitVertex)
      ptOrigin = pOriginPoint->point();
    else
      ODA_ASSERT_ONCE_X(ASSOC, esOrigin == eNotInGroup);

    OdGeExtents3d extSourceBTR;
    pSourceBTR->getGeomExtents(extSourceBTR);
    OdGePoint3d ptSourceCenter = extSourceBTR.center();

    OdGeVector3d vCenterToFirstItem(ptSourceCenter - ptOrigin); // vecZeroPointToOrigin

    //CORE-15240
    OdGeVector3d polarNormal(vZ);
    OdDbObjectId paramId;
    OdDbGeomRefPtr pGeomRef;
    if (pPolarParam->getGeomParam(L"BaseNormal", pGeomRef, paramId) == eOk)
      polarNormal = OdDbVertexRef::cast(pGeomRef)->point().asVector();

    OdGeVector3d vXaxis, vYaxis, vZaxis;
    //static_cast<OdDbImpAssocArrayPolarParameters*>(OdDbImpAssocArrayParameters::getImpl(pPolarParam))->CalculateAxes(vXaxis, vYaxis, vZaxis, polarNormal);
    OdDbImpAssocArrayCommonParameters::CalculateAxes(vXaxis, vYaxis, vZaxis, polarNormal);

    double startAngle = vXaxis.angleTo(vCenterToFirstItem, vZaxis);
    //

    double radius = vCenterToFirstItem.length();

    OdString expression, evaluatorID;
    es = pPolarParam->setStartAngle(radiansToDegree(startAngle), expression, evaluatorID);
    ODA_ASSERT_ONCE_X(ASSOC, es == eOk);
    es = pPolarParam->setRadius(radius, expression, evaluatorID);
    ODA_ASSERT_ONCE_X(ASSOC, es == eOk);

#if 0
    if (pPolarParam.get() && !GETBIT(enSkipOdaSpecific, kSkip_TransformToZero))
    {
      OdString expression, evaluatorID;
      OdGeVector3d vCenterToFirstItem(extSourceBTR.center() - ptBasePoint); // - ptOrigin);

#ifdef BS_CHANGES_ENABLED // vladimirv #16641, reported OdaJira:CORE-15240
      OdGeVector3d polarNormal(vZ);
      OdDbObjectId paramId;
      OdDbGeomRefPtr pGeomRef;
      if (pPolarParam->getGeomParam(OD_T("BaseNormal"), pGeomRef, paramId) == eOk)
        polarNormal = OdDbVertexRef::cast(pGeomRef)->point().asVector();

      OdGeVector3d vXaxis, vYaxis, vZaxis;
      OdDbImpAssocArrayPolarParameters* pImpParam = static_cast<OdDbImpAssocArrayPolarParameters *>(pPolarParam->m_pImpl);
      pImpParam->CalculateAxes(vXaxis, vYaxis, vZaxis, polarNormal);

      double startAngle = vXaxis.angleTo(vCenterToFirstItem, vZaxis);
#else // BS_CHANGES_ENABLED
      OdGeMatrix3d mat = OdGeMatrix3d().setCoordSystem(OdGePoint3d::kOrigin, vX, vY, vZ);
      double dRotation;
      {
        OdGeVector3d xAxis, yAxis, vNormal;
        OdGePoint3d pOrigin;
        mat.getCoordSystem(pOrigin, xAxis, yAxis, vNormal);
        OdGeScale3d sScale;
        sScale.sx = xAxis.length();
        sScale.sy = yAxis.length();
        sScale.sz = vNormal.length();

        vNormal /= sScale.sz;

        // Remove planeToWorld transformation from source matrix
        OdGeMatrix3d mTmp;//(mTrans * OdGeMatrix3d::worldToPlane(vNormal));
        mTmp.setToProduct(OdGeMatrix3d::worldToPlane(vNormal), mat);
        sScale.removeScale(mTmp, true);
        // mTmp contains only rotation around Z axis and translation
        dRotation = OD_ATAN2(mTmp[1][0], mTmp[0][0]);
        if (dRotation < 0.0)
          dRotation += Oda2PI;
      }

      double startAngle = vX.angleTo(vCenterToFirstItem, vZ) + dRotation;
#endif // BS_CHANGES_ENABLED

      pPolarParam->setStartAngle(OdaToDegree(startAngle), expression, evaluatorID);
      pPolarParam->setRadius(vCenterToFirstItem.length(), expression, evaluatorID);
    }
#endif

  }

  OdGeMatrix3d mat = getMatixToZeroState(pParameters, blockId);

  //transform source BTR to zero state
  //BEGIN
  OdDbObjectIteratorPtr pEntIter;
  for (pEntIter = pSourceBTR->newIterator(); !pEntIter->done(); pEntIter->step())
  {
    OdDbEntityPtr pEnt = OdDbEntity::cast(pEntIter->entity(OdDb::kForWrite));
    pEnt->transformBy(mat);
  }
  //END

  OdDbAssocArrayRectangularParameters* pRectParam = OdDbAssocArrayRectangularParameters::cast(pParameters).get();
  if (pRectParam)
  {
    if (pArray.isNull())
      pArray = OdDbBlockReference::cast(arrayId.openObject(OdDb::kForWrite));
    else
      pArray->upgradeOpen();

    // to get result like ACAD command    ARRAY RECT
    pArray->transformBy(mat.inverse());
  }

  if (pBody.get() && (pRectParam || pPolarParam))
    pBody->evaluateOverride();
}

OdResult OdDbImpAssocArrayActionBody::createInstance( const OdDbObjectIdArray& sourceEntites, 
                                                      OdDbVertexRef& basePoint, 
                                                      const OdDbAssocArrayParametersPtr &pParameters, 
                                                      OdDbObjectId& arrayId, 
                                                      OdDbObjectId& actionBodyId )
{
  OdResult res = eOk;

  if ( sourceEntites.isEmpty() )
    return eInvalidInput;

  OdDbAssocArrayRectangularParametersPtr pRectParam = OdDbAssocArrayRectangularParameters::cast( pParameters );
  OdDbAssocArrayPolarParametersPtr pPolarParam = OdDbAssocArrayPolarParameters::cast( pParameters );
  OdDbAssocArrayPathParametersPtr pPathParam = OdDbAssocArrayPathParameters::cast( pParameters ); 

  OdDbDatabase *pDb = sourceEntites.first().database();

  OdGeVector3d vX = pDb->getUCSXDIR();
  OdGeVector3d vY = pDb->getUCSYDIR();
  OdGeVector3d vZ = vX.crossProduct( vY );

  OdDbBlockTablePtr pMainBT = pDb->getBlockTableId().safeOpenObject( OdDb::kForWrite );
  OdDbObjectId ownerBTRId;
  {
    ownerBTRId = sourceEntites.first().openObject()->ownerId();
  }
  OdDbBlockTableRecordPtr pOwnerSpace = ownerBTRId.safeOpenObject( OdDb::kForWrite );

  //create source BTR
  //BEGIN
  OdDbBlockTableRecordPtr pSourceBTR = OdDbBlockTableRecord::createObject();
  pSourceBTR->setName( L"*U" );
  OdDbObjectId blockId = pDb->addOdDbObject( pSourceBTR, pMainBT->objectId() );  

  OdDbObjectId networkId = OdDbAssocNetwork::getInstanceFromObject( pOwnerSpace->objectId(), true );
  OdDbAssocNetworkPtr pAssocNetwork = OdDbAssocNetwork::cast( networkId.openObject( OdDb::kForWrite ) );

  OdDbIdMappingPtr pMapping = OdDbIdMapping::createObject();
  pMapping->setDestDb( pDb );
  pDb->deepCloneObjects( sourceEntites, blockId, *pMapping );

  OdGeExtents3d extSourceBTR;
  pSourceBTR->getGeomExtents( extSourceBTR ); 
  double dItemSize = extSourceBTR.minPoint().distanceTo( extSourceBTR.maxPoint() );
  //END  

  AssocArraySkipOdaSpecificFlags enSkipOdaSpecific = s_enSkipOdaSpecific; s_enSkipOdaSpecific = kSkip_Default;
  if (   pRectParam.get() // CORE-15158 assoc unit subtest createRectArrayAssoc_CORE15158
      || (pPathParam.get() && !GETBIT(enSkipOdaSpecific, kSkip_TransformToZero)))
  {
    //calculate transformation matrix to Zero state
    //BEGIN
    OdGeMatrix3d matExtenceToZeroState;  

    if (pRectParam.get()) // || pPolarParam.get())
    {
      OdGeVector3d vecZeroPointToOrigin = extSourceBTR.center().asVector();

      if (pRectParam.get())
      {
        if (pRectParam->rowCount() > 1 || pRectParam->columnCount() > 1) // || sourceEntites.size() > 1)
          vecZeroPointToOrigin = OdGeVector3d::kIdentity; // CORE-16399
        else
        {
        // CORE-15863 Difference in results of bitmap AssocArrayTests_AS 63, 66 in ACAD & ODA.
        //
        // It looks like ACAD does zero origin by 1st grip point of 1st entity in *U1 block
        // (see tripod position in ACAD for ODA & ACAD result files
        OdDbEntityPtr pFirstEnt = sourceEntites.first().safeOpenObject();
        OdGePoint3dArray pts;
        OdDbGripDataPtrArray grips;
        if (eOk == pFirstEnt->getGripPoints(pts) && pts.size())
          vecZeroPointToOrigin = pts.first().asVector();
        else if (eOk == pFirstEnt->getGripPoints(grips,
                                                 1.0, //curViewUnitSize // m_pOwner->activeViewUnitSize()
                                                 5, // gripSize // m_pOwner->m_GRIPSIZE
                                                 OdGeVector3d::kZAxis, // curViewDir // m_pOwner->activeViewDirection()
                                                 0) && grips.size()) // bitFlags
        {
          OdDbGripDataPtr pData = grips.first();
          vecZeroPointToOrigin = pData->gripPoint().asVector();
        }
        else
        {
          OdGeExtents3d ext;
          pFirstEnt->getGeomExtents(ext);
          vecZeroPointToOrigin = ext.center().asVector();
        }
      }
      }

      matExtenceToZeroState.setToTranslation(vecZeroPointToOrigin.negate());
    }
    
    if (pPathParam.get())
    {
      OdDbEdgeRef path = pPathParam->path();

      OdGeInterval interval;
      path.curve()->getInterval(interval);

      OdGeVector3dArray deriv;
      OdGePoint3d pointStartPath = path.curve()->evalPoint( interval.lowerBound(), 1, deriv );

      OdGeVector3d vTranslateSrcBlock( pointStartPath.x, 
                                       pointStartPath.y, 
                                       pointStartPath.z );

      matExtenceToZeroState = OdGeMatrix3d().setCoordSystem( OdGePoint3d::kOrigin, vX, vY, vZ ).invert();

      double ang = OdGeVector3d::kYAxis.angleTo( (vZ.crossProduct( deriv.first() )).transformBy( matExtenceToZeroState ),
                                                 OdGeVector3d::kZAxis );
      matExtenceToZeroState *=
          OdGeMatrix3d().rotation( -ang, vZ )
        * OdGeMatrix3d().setToTranslation( vTranslateSrcBlock.negate() );
    }
    //END

    //transform source BTR to zero state
    //BEGIN
    OdDbObjectIteratorPtr pEntIter;
    for ( pEntIter = pSourceBTR->newIterator(); !pEntIter->done(); pEntIter->step() )
    {
      OdDbEntityPtr pEnt = OdDbEntity::cast( pEntIter->entity(OdDb::kForWrite) );
      pEnt->transformBy( matExtenceToZeroState );
    }
    //END
  }

  //create destination BTR
  OdDbBlockTableRecordPtr pDestinationBTR = OdDbBlockTableRecord::createObject();
  OdString destinationBTRname = OD_T("*U");
  pDestinationBTR->setName( destinationBTRname );
  pDb->addOdDbObject( pDestinationBTR, pMainBT->objectId() );

  //create array block reference
  OdDbBlockReferencePtr pArrayBlock = OdDbBlockReference::createObject();
  pArrayBlock->setDatabaseDefaults(pDb);
  pArrayBlock->setBlockTableRecord( pDestinationBTR->objectId() );
  pDb->addOdDbObject( pArrayBlock, ownerBTRId );

  //create action and action body
  //BEGIN
  OdRxClass* pClass;
  pClass = (OdRxClass*)odrxClassDictionary()->getAt( OD_T("AcDbAssocArrayActionBody") ).get();

  OdDbObjectId actionId;

  OdResult es = createActionAndActionBodyAndPostToDatabase(pClass, networkId, actionId, actionBodyId);
  ODA_ASSERT_ONCE_X(ASSOC, es == eOk);
  if (es != eOk)
    return eInvalidInput;
  //END

  //append BTR and block ref
  pMainBT->upgradeOpen();
  pMainBT->add( pSourceBTR );
  pMainBT->add( pDestinationBTR );
  pOwnerSpace->appendOdDbEntity( pArrayBlock );
  arrayId = pArrayBlock->objectId();  

  //set owner for array parameter 
  OdDbAssocArrayActionBodyPtr pActionBody = 
    OdDbAssocActionBody::cast( actionBodyId.openObject( OdDb::kForWrite ) );
  pActionBody->parameters() = pParameters;
  const_cast<OdDbAssocArrayParametersPtr &>(pParameters)->setOwner( pActionBody );

  //create action parameters
  //BEGIN
  OdDbAssocActionPtr pAction = OdDbAssocAction::cast( actionId.openObject( OdDb::kForWrite ) );
  pAction->setActionBody( actionBodyId );

  int paramIndex = 0;
  OdDbObjectId paramId;

  OdString paramName;
  if (   pRectParam.isNull() // CORE-15158 subtest createRectArrayAssoc_CORE15158
      && vZ != OdGeVector3d::kZAxis && !GETBIT(enSkipOdaSpecific, kSkip_TransformToZero))
  {
    paramName = OD_T("BaseNormal");
    pClass = (OdRxClass*)odrxClassDictionary()->getAt( OD_T("AcDbAssocVertexActionParam") ).get();
    OdDbAssocVertexActionParamPtr pBaseNormal;
    OdDbVertexRefPtr pNormalVertexRef;
    switch ( pAction->addParam( paramName, pClass, paramId, paramIndex )  )
    {
    case eOk:
      pBaseNormal = OdDbAssocVertexActionParam::cast( paramId.openObject( OdDb::kForWrite ) );
      pNormalVertexRef = OdDbVertexRef::createObject( OdGePoint3d( vZ.x, vZ.y, vZ.z ) );
      pBaseNormal->setVertexRef( *pNormalVertexRef.get(),false );
      break;
    case eAlreadyInDb:
      pBaseNormal = OdDbAssocVertexActionParam::cast( paramId.openObject( OdDb::kForRead ) );
      pNormalVertexRef = OdDbVertexRef::createObject();
      pBaseNormal->getVertexRef( *pNormalVertexRef.get() );
      break;
    default:
      ODA_FAIL_ONCE();
    }
  }

  OdGePoint3d ptOrigin(0, 0, 0);
  if (basePoint.impliedType() == OdDbVertexRef::kExplicitVertex)
    ptOrigin = basePoint.point();
  if (   (pPathParam.get()) // || pPolarParam.get()) // pRectParam.isNull() // CORE-15158 subtest createRectArrayAssoc_CORE15158
      && ptOrigin != OdGePoint3d::kOrigin && !GETBIT(enSkipOdaSpecific, kSkip_TransformToZero))
  {
    ODA_FAIL_ONCE_X(ASSOC);
    paramName = OD_T("Origin");
    pClass = (OdRxClass*)odrxClassDictionary()->getAt( OD_T("AcDbAssocVertexActionParam") ).get();
    OdDbAssocVertexActionParamPtr pOrigin;
    OdDbVertexRefPtr pOriginVertexRef = OdDbVertexRef::createObject(ptOrigin);
    switch ( pAction->addParam( paramName, pClass, paramId, paramIndex )  )
    {
    case eOk:
      pOrigin = OdDbAssocVertexActionParam::cast(paramId.openObject(OdDb::kForWrite));
      pOrigin->setVertexRef( *pOriginVertexRef.get(), false );
      break;
    case eAlreadyInDb:
      pOriginVertexRef = OdDbVertexRef::createObject();
      pOrigin = OdDbAssocVertexActionParam::cast(paramId.openObject(OdDb::kForRead));
      pOrigin->getVertexRef( *pOriginVertexRef.get() );
      break;
    default:
      ODA_FAIL_ONCE();
    }
  }

  OdGePoint3d ptBasePoint(0, 0, 0);
  if (   pRectParam.isNull() // CORE-15158 subtest createRectArrayAssoc_CORE15158
      && basePoint.impliedType() == OdDbVertexRef::kExplicitVertex)
    ptBasePoint = basePoint.point();
  paramName = OD_T("basePoint"); 
  pClass = (OdRxClass*)odrxClassDictionary()->getAt( OD_T("AcDbAssocVertexActionParam") ).get();
  OdDbAssocVertexActionParamPtr pBasePoint;
  OdDbVertexRefPtr pBaseVertexRef = OdDbVertexRef::createObject(ptBasePoint);
  switch ( pAction->addParam( paramName, pClass, paramId, paramIndex )  )
  {
  case eOk:
    pBasePoint = OdDbAssocVertexActionParam::cast( paramId.openObject( OdDb::kForWrite ) );
    pBasePoint->setVertexRef( *pBaseVertexRef.get(), false );
    break;
  case eAlreadyInDb:
    pBasePoint = OdDbAssocVertexActionParam::cast( paramId.openObject( OdDb::kForRead ) );
    pBasePoint->getVertexRef( *pBaseVertexRef.get() );
    break;
  default:
    ODA_FAIL_ONCE();
  }  

  // next strange solution (acadRectArray.dwg is created in ACAD but has not GeomParam "RowProfile" on assoc action - handle 202) 
  // is marked for // CORE-10186 // AS * #19363 - Associative array has wrong position if UCS != WCS.
  // but it is conflicted with bitmap AssocArrayTests_AS 63, 66 to have the same result as in ARX.
  //
  // CORE-15863 //if ( !pRectParam.isNull() && !vX.isCodirectionalTo( OdGeVector3d::kXAxis ) )
  // CORE-15863 //  pRectParam->setAxisDirection( vX ); 

  //END

  //set default valueParam 
  //BEGIN
  OdString errorMSG;
  OdString valueParamName;
  OdValue::UnitType unitType;
  
  // next commented for CORE-15158 subtest createRectArrayAssoc_CORE15158
  //if ( !pRectParam.isNull() )
  //{
  //  OdGeMatrix3d mat;
  //  mat.setToTranslation( OdGeVector3d( extSourceBTR.center().x,
  //                                      extSourceBTR.center().y,
  //                                      extSourceBTR.center().z) );
  //
  //  pActionBody->transformBy( mat );
  //  pArrayBlock->transformBy( mat );
  //}

  // moved into odrxAssocArrayAdjustLocation
  // to have common behaviour with ACAD
  //if (pPolarParam.get() && !GETBIT(enSkipOdaSpecific, kSkip_TransformToZero))
  //{
  //  OdString expression, evaluatorID;
  //
  //  OdGeVector3d vCenterToFirstItem(extSourceBTR.center() - ptBasePoint); // - ptOrigin);
  //
  //  OdGeMatrix3d mat = OdGeMatrix3d().setCoordSystem( OdGePoint3d::kOrigin, vX, vY, vZ );
  //  double dRotation;
  //  {
  //    OdGeVector3d xAxis, yAxis, vNormal;
  //    OdGePoint3d pOrigin;
  //    mat.getCoordSystem( pOrigin, xAxis, yAxis, vNormal );
  //    OdGeScale3d sScale;
  //    sScale.sx = xAxis.length();
  //    sScale.sy = yAxis.length();
  //    sScale.sz = vNormal.length();
  //
  //    vNormal /= sScale.sz;
  //
  //    // Remove planeToWorld transformation from source matrix
  //    OdGeMatrix3d mTmp;//(mTrans * OdGeMatrix3d::worldToPlane(vNormal));
  //    mTmp.setToProduct( OdGeMatrix3d::worldToPlane( vNormal ), mat );
  //    sScale.removeScale( mTmp, true );
  //    // mTmp contains only rotation around Z axis and translation
  //    dRotation = OD_ATAN2( mTmp[1][0], mTmp[0][0] );
  //    if ( dRotation < 0.0 )
  //      dRotation += Oda2PI;
  //  }
  //
  //    double startAngle = vX.angleTo( vCenterToFirstItem, vZ ) + dRotation;
  //
  //  pPolarParam->setStartAngle(OdaToDegree(startAngle), expression, evaluatorID );
  //  pPolarParam->setRadius( vCenterToFirstItem.length(), expression, evaluatorID );
  //}

  if (pPathParam.get())
  {
    OdDbEvalVariantPtr value = OdDbEvalVariant::init( dItemSize );
    unitType = OdValue::kDistance;

    int count = int( pPathParam->path().curve()->length() / dItemSize ) + (pPathParam->path().curve()->isClosed() ? 0 : 1);
    if ( count <= 0 )
      count = 1;
    pPathParam->setItemCount( count );
    pPathParam->setItemSpacing( dItemSize );
    pPathParam->setAlignItems( true );

    value = OdDbEvalVariant::init( double(1) );
    unitType = OdValue::kUnitless;

    valueParamName = OD_T("FillPath");
    pActionBody->setValueParam( 
      valueParamName, *value.get(), OdString::kEmpty, OdString::kEmpty, errorMSG, true );
    pActionBody->setValueParamUnitType( valueParamName, unitType );

    valueParamName = OD_T("MaintainZ");
    pActionBody->setValueParam( 
      valueParamName, *value.get(), OdString::kEmpty, OdString::kEmpty, errorMSG, true );
    pActionBody->setValueParamUnitType( valueParamName, unitType );

    value = OdDbEvalVariant::init( double(0) );
    valueParamName = OD_T("TangentOrientationAngle");
    pActionBody->setValueParam( 
      valueParamName, *value.get(), OdString::kEmpty, OdString::kEmpty, errorMSG, true );
    pActionBody->setValueParamUnitType( valueParamName, unitType );

    value = OdDbEvalVariant::init( int(1) );
    valueParamName = OD_T("Orientation");
    pActionBody->setValueParam( 
      valueParamName, *value.get(), OdString::kEmpty, OdString::kEmpty, errorMSG, true );
    pActionBody->setValueParamUnitType( valueParamName, unitType );
  }
  //END

  //fill destination BTR
  //BEGIN
  OdArray<OdDbAssocArrayItemPtr> Items;
  OdResult resGetItems = pParameters->getItems( Items );
  if (resGetItems != eOk)
    return resGetItems;

  OdGeMatrix3d mat;
  OdDbBlockReferencePtr pBlkRef;
  OdArray<OdDbAssocArrayItemPtr>::iterator it_ArrayItem;

  for ( it_ArrayItem = Items.begin(); it_ArrayItem != Items.end(); it_ArrayItem++ )
  {
    if ( (*it_ArrayItem)->entityProvider().isNull() )
    {
      pBlkRef = OdDbBlockReference::createObject();
      pBlkRef->setLayer(pDb->getCLAYER());
      pBlkRef->setColor(OdCmEntityColor::kByBlock);
      pBlkRef->setLineWeight(OdDb::kLnWtByBlock);
      pBlkRef->setPlotStyleName(OdDb::kPlotStyleNameByBlock);
      pBlkRef->setTransparency(OdCmTransparency(OdCmTransparency::kByBlock));
      pBlkRef->setLinetype(pDb->getLinetypeByBlockId());
      pBlkRef->setMaterial(pDb->byBlockMaterialId());
      pBlkRef->setBlockTableRecord( pSourceBTR->objectId() );

      OdDbObjectId blkRefId = pDestinationBTR->appendOdDbEntity(pBlkRef);
      (*it_ArrayItem)->setEntityProvider( blkRefId );
      
      (*it_ArrayItem)->getTransform( mat );
      pBlkRef->setBlockTransform( mat );
    }
  } 

  //END
  
  //create dependency
  //BEGIN

  //Destination block table record
  OdDbAssocDependencyPtr pDependency = OdDbAssocDependency::createObject();
  pDb->addOdDbObject( pDependency );
  pDependency->setIsReadDependency( false );
  pDependency->setIsWriteDependency( true );
  OdDbCompoundObjectId compoundObjectId;
  compoundObjectId.set( pDestinationBTR->objectId() );
  pDependency->attachToObject( compoundObjectId );
  pAction->addDependency( pDependency->objectId(), true );

  //Source block table record
  pDependency = OdDbAssocDependency::createObject();
  pDb->addOdDbObject( pDependency );
  pDependency->setIsReadDependency( true );
  pDependency->setIsWriteDependency( false );
  compoundObjectId.set( pSourceBTR->objectId() );
  pDependency->attachToObject( compoundObjectId );
  pAction->addDependency( pDependency->objectId(), true );

  //array block reference
  pDependency = OdDbAssocDependency::createObject();
  pDb->addOdDbObject( pDependency );
  pDependency->setIsReadDependency( true );
  pDependency->setIsWriteDependency( true );
  pDependency->setOrder( 0 - 2147483648u );
  compoundObjectId.set( pArrayBlock->objectId() );
  pDependency->attachToObject( compoundObjectId );
  pAction->addDependency( pDependency->objectId(), true );

  //END

  if (GETBIT(enSkipOdaSpecific, kSkip_EraseSourceEntities))
    return res;

  //erasing of source entities
  OdDbObjectIdArray::const_iterator it_Scr;
  for ( it_Scr = sourceEntites.begin(); it_Scr != sourceEntites.end(); it_Scr++ )
  {
    OdDbObjectPtr pObj = (*it_Scr).safeOpenObject( OdDb::kForWrite );
    pObj->erase( true );
    pObj->downgradeOpen();
  }

  return res;
}

OdDbObjectId OdDbImpAssocArrayActionBody::getControllingActionBody( const OdDbEntity* pEntity, 
                                                                    const OdDbItemLocator* pItemIndex )
{
  if ( !pEntity )
    return OdDbObjectId();

  OdDbObjectIdArray aReactors = pEntity->getPersistentReactors();
  for (OdDbObjectIdArray::const_iterator it = aReactors.begin(); it != aReactors.end(); it++)
  {
    OdDbAssocDependencyPtr pDep = OdDbAssocDependency::cast(it->openObject());
    if (!pDep.isNull())
    {
      OdDbAssocActionPtr pAction = OdDbAssocAction::cast(pDep->owningAction().openObject());
      if (!pAction.isNull())
      {
        OdDbObjectIdArray objectIDs;
        pAction->getDependentObjects(false, true, objectIDs);

        if (objectIDs.contains(pEntity->objectId()))
        {
          OdDbObjectId bodyId = pAction->actionBody();
          if (bodyId.objectClass()->isDerivedFrom(OdDbAssocArrayActionBody::desc())) // CORE-16084
            return bodyId;
        }
      }
    }
  }
  return OdDbObjectId();
}

bool OdDbImpAssocArrayActionBody::isAssociativeArray( const OdDbEntity* pEntity )
{
  return !getControllingActionBody( pEntity, NULL ).isNull();
}

OdResult OdDbImpAssocArrayActionBody::resetArrayItems( OdDbObjectId arrayEntityId, 
                                                       const OdArray<OdDbItemLocator>& indices, 
                                                       bool resetAll /*= false*/ )
{
  OdDbEntityPtr pEntity = OdDbEntity::cast( arrayEntityId.openObject( OdDb::kForWrite ) );
  if ( pEntity.isNull() )
  {
    return eInvalidInput;
  }

  OdDbObjectId bodyId = getControllingActionBody( pEntity );
  OdDbAssocArrayActionBodyPtr pBody = OdDbAssocArrayActionBody::cast( bodyId.openObject( OdDb::kForWrite ) );
  if ( pBody.isNull() )
  {
    return eInvalidInput;
  }
  pBody->disableUndoRecording( true );
  pBody->assertWriteEnabled( false, true );
  pBody->disableUndoRecording( false );

  OdGeMatrix3d IdentityMatrix;
  OdArray<OdDbItemLocator> itemIndices;

  OdArray<OdDbItemLocator>::const_iterator it;
  OdArray<OdDbItemLocator>::const_iterator it_end;

  if ( resetAll )
  {
    pBody->getItems( itemIndices, false );

    it = itemIndices.begin();
    it_end = itemIndices.end(); 
  }
  else
  {
    it = indices.begin();
    it_end = indices.end();
  }

  OdDbAssocArrayItemPtr pItem;
  for ( ; it != it_end; it++ )
  {
    pItem = pBody->getItemAt( *it );
    OdDbImpAssocArrayItem *pItemImplp = OdDbImpAssocArrayItem::getImpl( pItem );
    pItemImplp->reset();
  }

  return eOk;
}

OdResult OdDbImpAssocArrayActionBody::transformBy( const OdGeMatrix3d& xform )
{
  m_matrix = xform;
  return eOk;
}

OdGeMatrix3d OdDbImpAssocArrayActionBody::getTransform() const
{
  return m_matrix;
}

OdDbObjectId OdDbImpAssocArrayActionBody::getArraySourceBTR( OdDbObjectId parentActionId ) const
{
  OdDbObjectIdArray objIDs;
  OdDbAssocActionPtr pAction = OdDbAssocAction::cast( parentActionId.safeOpenObject(OdDb::kForRead) );
  pAction->getDependentObjects( true, false, objIDs );

  OdDbObjectId IDsourceBTR;

  if ( m_pParams->isKindOf( OdDbAssocArrayRectangularParameters::desc() ) 
    || m_pParams->isKindOf( OdDbAssocArrayPolarParameters::desc() )  )
  {        
    IDsourceBTR = objIDs.first();
  }

  if ( m_pParams->isKindOf( OdDbAssocArrayPathParameters::desc() ) )
  {
    switch ( objIDs.length() )
    {
    case 3:
      IDsourceBTR = objIDs[1];
      break;
    case 2:
      IDsourceBTR = objIDs[0];
      break;
    }
  }

  if ( m_pParams->isKindOf( OdDbAssocArrayModifyParameters::desc() ) )
  {
    IDsourceBTR = objIDs[1];
  }

  return IDsourceBTR;
}

OdDbObjectId OdDbImpAssocArrayActionBody::getArrayEntity( OdDbObjectId parentActionId ) const
{
  OdDbObjectIdArray objIDs;
  OdDbAssocActionPtr pAction = parentActionId.safeOpenObject(OdDb::kForRead);
  pAction->getDependencies( true, true, objIDs );

  OdDbObjectId idDepEntity = getIdDepArrayEntity(objIDs);
  if (idDepEntity.isNull())
    return OdDbObjectId();
  return OdDbAssocDependencyPtr(idDepEntity.openObject())->dependentOnObject();
}

void addContentBlocks(OdDbAssocAction* action, OdDbObjectIdArray& additionalObjectsToClone) {
  OdDbObjectIdArray dependentObjects;
  action->getDependentObjects(true, true, dependentObjects);

  OdDbObjectIdArray::iterator it;
  for (it = dependentObjects.begin(); it != dependentObjects.end(); it++) {
    OdDbBlockTableRecordPtr block = OdDbBlockTableRecord::cast(it->openObject());
    if (block.get())
      additionalObjectsToClone.append(*it);
  }
}

OdResult OdDbImpAssocArrayActionBody::addMoreObjectsToDeepCloneOverride(
  const OdDbAssocActionBody *body, OdDbIdMapping& idMap, OdDbObjectIdArray& additionalObjectsToClone) const
{
  OdDbIdPair arrEntIdPair(getArrayEntity(body->parentAction()));
  if (idMap.compute(arrEntIdPair) && arrEntIdPair.isCloned()) {
    OdDbAssocActionPtr action = body->parentAction().safeOpenObject();
    ::addContentBlocks(action, additionalObjectsToClone);
    additionalObjectsToClone.append(body->parentAction());
  }
  return eOk;
}

bool OdDbAssocNetCloneCtx::cloneObject(OdDbIdMapping& idMap,
  const OdDbObject* original, OdDbObject* destOwner, OdDbObjectPtr* pClone)
{
  OdDbObjectPtr clone;
  if (idMap.destDb() == idMap.origDb())
    clone = original->deepClone(idMap, destOwner);
  else
    clone = original->wblockClone(idMap, destOwner);
  if(pClone)
    *pClone = clone;
  return clone.get()!=0;
}

OdResult OdDbAssocArrayActionBody::postProcessAfterDeepCloneOverride( OdDbIdMapping& idMap )
{
  // Map dependencies of destination action
  OdDbAssocActionPtr pAction = OdDbAssocAction::cast( parentAction().openObject( OdDb::kForRead ) );

  OdDbIdPair searchForDest( pAction->objectId() );
  if (!idMap.compute(searchForDest) || !searchForDest.isCloned() || searchForDest.value().isErased() )
  {
    return eOk;
  }

  OdDbAssocActionPtr pClonedAction = OdDbAssocAction::cast(searchForDest.value().openObject(OdDb::kForWrite));

  OdDbAssocArrayActionBodyPtr pClonedBody = pClonedAction->actionBody().openObject( OdDb::kForWrite );
  pClonedBody->parameters()->setOwner( pClonedBody );

  //TODO who and why added them
  OdDbObjectIdArray clonedActionPersReactIDs = pClonedAction->getPersistentReactors();
  if ( ! clonedActionPersReactIDs.isEmpty() )
  {
//     std::for_each( clonedActionPersReactIDs.begin(),
//                    clonedActionPersReactIDs.end(),
//                    [&pClonedAction]( const OdDbObjectId &reactorId )
    OdDbObjectIdArray::iterator it;
    for ( it = clonedActionPersReactIDs.begin(); 
          it != clonedActionPersReactIDs.end();
          it++)
    { 
      pClonedAction->removePersistentReactor( *it );
    }
  }

  OdDbObjectIdArray DepIDs; 
  OdDbIdPair pairId;

  OdDbGeomRefPtr rowProfile;  
  OdDbObjectId rowProfileID;
  OdDbIdPair pathEntityIdPair;
  OdDbObjectId pathDependencyId;
  if ( parameters()->isA()->isEqualTo( OdDbAssocArrayPathParameters::desc() )
    && eOk == parameters()->getGeomParam( OD_T("RowProfile"), rowProfile, rowProfileID ) )
  {
    OdDbAssocEdgeActionParamPtr pActionParam = OdDbAssocEdgeActionParam::cast( 
                                                  rowProfileID.openObject( OdDb::kForRead ) );

    OdDbCompoundObjectId cmpObjId;
    pActionParam->getDependentOnCompoundObject( cmpObjId );
    OdDbObjectId srcPathEntityId = cmpObjId.leafId();
      
    pActionParam->getDependencies( true, false, DepIDs );
    if (DepIDs.length() > 0)
      pathDependencyId = DepIDs.first();
  
    //path entity absent 
    pathEntityIdPair.setKey( srcPathEntityId );
    if ( !idMap.compute(pathEntityIdPair) )
     {            
       pairId.setKey( pathDependencyId );
       if ( idMap.compute( pairId ) )
       {
         OdDbObjectId clonedRowProfileID = pClonedAction->paramAtName( OD_T("RowProfile") );
         OdDbAssocEdgeActionParamPtr pClonedActionParam = 
           OdDbAssocEdgeActionParam::cast( clonedRowProfileID.openObject( OdDb::kForWrite ) );

         pClonedActionParam->detachDependencies();
       }
     }    
   } 

  //check src arrayEntity presence, remove if absent
  DepIDs.clear();
  pAction->getDependencies( true, true, DepIDs );

  OdDbObjectId idDepEntity = getIdDepArrayEntity(DepIDs);
  if (idDepEntity.isNull())
    return eKeyNotFound;
  OdDbAssocDependencyPtr pDepArrayEntity = idDepEntity.openObject( OdDb::kForRead );

  pairId.setKey( pDepArrayEntity->dependentOnObject() );
  if ( !idMap.compute(pairId) )
  {
    if ( !pathEntityIdPair.value().isNull() )
    {
      OdDbEntityPtr pPath = OdDbEntity::cast( pathEntityIdPair.value().safeOpenObject( OdDb::kForWrite ) );
      
      pairId.setKey( pathDependencyId );
      if ( idMap.compute(pairId) )
        pPath->removePersistentReactor( pairId.value() );
    }
  }

  return eOk;
}

OdResult OdDbImpAssocArrayActionBody::explode( OdDbEntity* pEntity, OdDbObjectIdArray& newIds )
{
  OdDbObjectId BodyId = getControllingActionBody( pEntity );
  if ( BodyId.isNull() )
    return eInvalidInput;

  newIds.clear();

  OdDbDatabase *pDb = pEntity->database();

  OdDbAssocArrayActionBodyPtr pArrayBody = BodyId.safeOpenObject( OdDb::kForWrite );

  OdDbBlockReferencePtr pArrayEntity = pArrayBody->getArrayEntity().safeOpenObject( OdDb::kForWrite );

  OdDbObjectId ownerId = pArrayEntity->ownerId();
  ODA_ASSERT( pArrayEntity.get() == pEntity );

  OdDbObjectIdArray origEntities = pArrayBody->getSourceEntities();

  OdGeMatrix3d arrayTramsform = pArrayBody->getTransform();

  OdDbImpAssocArrayParameters *pParamImpl = OdDbImpAssocArrayParameters::getImpl( pArrayBody->parameters() );

  OdDbImpAssocArrayParameters::ItemArray::iterator itItem;
  for ( itItem = pParamImpl->m_items.begin();
        itItem != pParamImpl->m_items.end();
        itItem++ )
  {
    if ((*itItem)->isErased())
      continue;

    OdGeMatrix3d defaulTransform;
    (*itItem)->getTransform(defaulTransform);

    OdDbImpAssocArrayItem *pItemImpl = OdDbImpAssocArrayItem::getImpl( *itItem );
    OdDbAssocArrayModifyActionBodyPtr pModBody = pItemImpl->modifyActionBody().openObject();

    OdDbObjectIdArray srcEntIds = pModBody.isNull() ? origEntities : pModBody->getSourceEntities();

    // clone item
    OdDbIdMappingPtr pMapping = OdDbIdMapping::createObject(OdDb::kDcExplode);
    pMapping->setDestDb( pDb );
    pDb->deepCloneObjects( srcEntIds, ownerId, *pMapping );

    // apply transform
    OdGeMatrix3d transform = arrayTramsform * defaulTransform;
    OdDbObjectIdArray::const_iterator itSrcId;
    for ( itSrcId = srcEntIds.begin(); itSrcId != srcEntIds.end(); itSrcId++ )
    {
      OdDbIdPair pair;
      pair.setKey( *itSrcId );
      pMapping->compute( pair );
      newIds.append( pair.value() );
      OdDbEntityPtr pExplodedEnt = OdDbEntity::cast( pair.value().openObject( OdDb::kForWrite ) );
      if ( pExplodedEnt.get() )
        pExplodedEnt->transformBy( transform );
    }
  }

  pArrayEntity->erase();

  OdDbBlockTableRecordPtr pOwnerBTR = ownerId.safeOpenObject();
  OdDbSortentsTablePtr pSET = pOwnerBTR->getSortentsTable();
  pSET->moveAbove( newIds, pArrayEntity->objectId() );

  return eOk;
}

bool OdDbImpAssocArrayActionBody::controlsItem( const OdDbItemLocator& index ) const
{
  OdDbAssocArrayItemPtr pItem;

  if ( eOk != getItem( pItem, index ) )
    return false;

  OdDbImpAssocArrayItem *pItemImpl = OdDbImpAssocArrayItem::getImpl( pItem );
  return !pItemImpl->replaced();
}

OdDbObjectIdArray OdDbImpAssocArrayActionBody::getSourceEntities( OdDbObjectId parentActionId ) const
{  
  OdDbObjectIdArray objIDs;

  OdDbBlockTableRecordPtr pSourceBTR = getSourceBTR( parentActionId );
  OdDbObjectIteratorPtr pOjIt = pSourceBTR->newIterator();
  for ( ; !pOjIt->done(); pOjIt->step() )
  {
    objIDs.append( pOjIt->objectId() );
  }

  return objIDs;
}

OdResult OdDbImpAssocArrayActionBody::addSourceEntity( OdDbObjectId entity, const OdGePoint3d& basePoint, OdDbObjectId parentActionId )
{
  OdDbBlockTableRecordPtr pSourceBTR = getSourceBTR( parentActionId );
  OdDbEntityPtr pEnt = OdDbEntity::cast( entity.safeOpenObject( OdDb::kForWrite ) );
  if ( pEnt.isNull() )
    return eInvalidInput;

  OdDbIdMappingPtr pMapping = OdDbIdMapping::createObject();
  pMapping->setDestDb( pEnt->database() );
  OdDbObjectIdArray newEntIds;
  newEntIds.append( entity );
  pEnt->database()->deepCloneObjects( newEntIds, pSourceBTR->objectId(), *pMapping );

  OdDbIdPair entIdPair( entity );
  if ( !pMapping->compute( entIdPair ) )
    return eInvalidInput;
  
  OdDbEntityPtr pAddedEnt = entIdPair.value().openObject( OdDb::kForWrite );
  OdGeVector3d transVec = OdGePoint3d( 0, 0, 0 ) - basePoint;
  pAddedEnt->transformBy( OdGeMatrix3d().setToTranslation( transVec ) );

  return eOk;
}


OdResult OdDbImpAssocArrayActionBody::removeSourceEntity( OdDbObjectId entity, OdDbObjectId parentActionId )
{
  OdDbEntityPtr pEnt = OdDbEntity::cast( entity.safeOpenObject( OdDb::kForWrite ) );
  pEnt->erase();
  return eOk;
}

OdResult OdDbImpAssocArrayActionBody::setSourceBasePoint( const OdDbVertexRef& basePoint, const OdDbObjectId &parentActionId )
{
  OdResult res;

  OdDbObjectIdArray srcEnt = getSourceEntities( parentActionId );

  OdDbImpAssocArrayParameters *pParamImpl = OdDbImpAssocArrayParameters::getImpl( parameters() );
  OdDbObjectIdArray modBodyIds = pParamImpl->getModifyBodies();
  OdDbObjectIdArray::iterator itModBodyId;
  for ( itModBodyId = modBodyIds.begin(); itModBodyId != modBodyIds.end(); itModBodyId++ )
  {
    OdDbAssocArrayActionBodyPtr pModifyBody = OdDbAssocArrayActionBody::cast( itModBodyId->openObject() );
    srcEnt.append( pModifyBody->getSourceEntities() );
  }

  OdDbObjectIdArray::iterator it;
  OdDbEntityPtr pEnt;
  OdGeMatrix3d transMat;

  OdDbVertexRef bp;
  OdGePoint3d pos;
  getSourceBasePoint( bp, pos );

  OdDbAssocArrayPathParametersPtr pPathParam = OdDbAssocArrayPathParameters::cast( m_pParams );
  OdDbAssocArrayPolarParametersPtr pPolarParam = OdDbAssocArrayPolarParameters::cast( m_pParams );
  OdDbAssocArrayRectangularParametersPtr pRectParam = OdDbAssocArrayRectangularParameters::cast( m_pParams );

  //PATH
  if ( !pPathParam.isNull() )
  {
    OdDbObjectId paramId;
    OdDbGeomRefPtr pGeomRef;
    res = pPathParam->getGeomParam( OD_T( "BaseNormal" ), pGeomRef, paramId );
    if ( eOk != res )
      return res;

    OdGeVector3d vZ = OdDbVertexRef::cast( pGeomRef )->point() - OdGePoint3d::kOrigin;
    OdGeVector3d vX = OdGeVector3d::kZAxis.crossProduct( vZ );
    if ( vX.isZeroLength() )
    {
      vX = OdGeVector3d::kZAxis.isCodirectionalTo( vZ )
         ? OdGeVector3d::kXAxis
         : -OdGeVector3d::kXAxis;
    }
    OdGeVector3d vY = vZ.crossProduct( vX );

    OdDbEdgeRef path = pPathParam->path();

    OdGeInterval interval;
    path.curve()->getInterval( interval );

    OdGeVector3dArray deriv;
    OdGePoint3d pointStartPath = path.curve()->evalPoint( interval.lowerBound(), 1, deriv );

    OdGeVector3d vTranslateSrcBlock( basePoint.point().x,
                                     basePoint.point().y,
                                     basePoint.point().z );

    OdGeMatrix3d mat;
    mat.setToTranslation( pos - basePoint.point() );

    OdGeMatrix3d matExtenceToZeroState;
    
    matExtenceToZeroState = OdGeMatrix3d().setCoordSystem( OdGePoint3d::kOrigin, vX, vY, vZ ).invert();

    double ang = OdGeVector3d::kYAxis.angleTo( (vZ.crossProduct( deriv.first() )).transformBy( matExtenceToZeroState ),
                                               OdGeVector3d::kZAxis );
    matExtenceToZeroState *=
        OdGeMatrix3d().rotation( -ang, vZ )
      * OdGeMatrix3d().setToTranslation( vTranslateSrcBlock.negate() );

    OdGeMatrix3d invMatExtenceToZeroState( matExtenceToZeroState );
    invMatExtenceToZeroState.invert();

    transMat = matExtenceToZeroState*mat*invMatExtenceToZeroState;
  }

  //POLAR
  if ( !pPolarParam.isNull() )
  {
    const OdGeMatrix3d invMat = m_matrix.inverse();
    const OdGePoint3d oldPosEc = invMat * pos;
    const OdGePoint3d newPosEc = invMat * basePoint.point();
    transMat.setToTranslation(oldPosEc - newPosEc);
//    transMat.setToTranslation( pos - basePoint.point() );

    OdDbObjectId paramId;
    OdDbGeomRefPtr pGeomRef;
    res = pPolarParam->getGeomParam( OD_T( "BaseNormal" ), pGeomRef, paramId );
    if ( eOk != res )
      return res;
    OdGeVector3d vZ = OdDbVertexRef::cast( pGeomRef )->point() - OdGePoint3d::kOrigin;

    res = pPolarParam->getGeomParam( OD_T( "Origin" ), pGeomRef, paramId );
    if ( eOk != res )
      return res;
    OdGePoint3d origin = OdDbVertexRef::cast( pGeomRef )->point();

    OdGePlane newPlaneEc(newPosEc, vZ);
    OdGePoint3d newOriginEc; // on new plane
    newPlaneEc.project(origin, newOriginEc);
    
    OdGeVector3d offsetOriginEc = newOriginEc - origin;
    OdGeVector3d offsetOrigin = m_matrix * offsetOriginEc;
    // Adapt transformation of array (top BlockRef).
    // The new m_matrix will be applied to BlockRef in OdDbImpAssocArrayActionBody::evaluateOverride()
    m_matrix.setTranslation(m_matrix.translation() + offsetOrigin);

    OdGeVector3d vCenterToFirstItem(newPosEc - newOriginEc);
//    OdGeVector3d vCenterToFirstItem( basePoint.point() - origin );

    OdGeVector3d vX = OdGeVector3d::kZAxis.crossProduct( vZ );
    if ( vX.isZeroLength() )
    {
      vX = OdGeVector3d::kZAxis.isCodirectionalTo( vZ )
         ? OdGeVector3d::kXAxis
         : -OdGeVector3d::kXAxis;
    }
    double startAngle = vX.angleTo( vCenterToFirstItem, vZ );

    OdString expression, evaluatorID;
    pPolarParam->setStartAngle(OdaToDegree(startAngle), expression, evaluatorID );
    pPolarParam->setRadius( vCenterToFirstItem.length() );
  }
  //RECTANGULAR
  if ( !pRectParam.isNull() )
  {
    OdDbBlockReferencePtr pArrayEnt = getArrayEntity( parentActionId ).openObject( OdDb::kForWrite );

    OdGeVector3d vTranslateSrcBlock( pos - basePoint.point() );
    transMat.setToTranslation( vTranslateSrcBlock );
    pArrayEnt->transformBy( OdGeMatrix3d().setToTranslation( vTranslateSrcBlock.negate() ) );
    pArrayEnt->downgradeOpen();
  }

  for ( it = srcEnt.begin(); it != srcEnt.end(); it++ )
  {
    pEnt = it->openObject( OdDb::kForWrite );
    pEnt->transformBy( transMat );
    pEnt->downgradeOpen();
  }
  return eOk;
}

OdResult OdDbImpAssocArrayActionBody::getSourceBasePoint( OdDbVertexRef& vertexRef, OdGePoint3d& position ) const
{
  OdResult res;
  OdDbObjectId basePointId;
  OdDbGeomRefPtr basePoint;  

  res = parameters()->getGeomParam( OD_T("basePoint"), basePoint, basePointId );
  if (eOk != res)
    return res;

  vertexRef = *OdDbVertexRef::cast( basePoint );

  if ( parameters()->isA()->isEqualTo( OdDbAssocArrayRectangularParameters::desc() ) )
  {
    OdGeVector3d vTrans = m_matrix.translation();
    position = OdGePoint3d( vTrans.x, vTrans.y, vTrans.z );
  }
  else if (parameters()->isA()->isEqualTo(OdDbAssocArrayPolarParameters::desc()))
  {
      OdGeMatrix3d mat;
      parameters()->getItemPosition(OdDbItemLocator(0, 0, 0), position, mat);
      // ACAD help: The point evaluated on vertexRef may not match the output 
      // position of the base point, as it is evaluated in the space of source BTR.
      position.transformBy(m_matrix);
  }
  //TODO: position.transformBy(m_matrix); must be applied to Path arrays too
  else
  {
    OdGeMatrix3d mat;
    parameters()->getItemPosition( OdDbItemLocator( 0, 0, 0 ), position, mat );
  }

  return eOk;
}

OdDbBlockTableRecordPtr OdDbImpAssocArrayActionBody::getSourceBTR( const OdDbObjectId &parentActionId ) const
{
  return getArraySourceBTR( parentActionId ).openObject( OdDb::kForRead );
}

OdDbBlockTableRecordPtr OdDbImpAssocArrayActionBody::getSourceBTR( const OdDbObjectId &parentActionId ) 
{
  return getArraySourceBTR( parentActionId ).openObject( OdDb::kForWrite );
}

OdResult OdDbImpAssocArrayActionBody::getArrayItemLocators( const OdDbFullSubentPathArray& subents, OdArray<OdDbItemLocator>& indices )
{
  OdDbFullSubentPathArray::const_iterator itSubent;
  for ( itSubent = subents.begin(); itSubent != subents.end(); itSubent++ )
  {
  }
  return eNotImplementedYet;
}

static OdDbObjectId getIdDepArrayEntity( const OdDbObjectIdArray& depIDs )
{
  OdDbAssocDependencyPtr pDepArrayEntity;
  for ( unsigned int i = 0; i < depIDs.length(); i++ )
  {
    pDepArrayEntity = depIDs[i].openObject();
    if ( pDepArrayEntity.isNull() )
      continue;

    if ( pDepArrayEntity->isReadDependency() && pDepArrayEntity->isWriteDependency() )
    {
      return pDepArrayEntity->objectId();
    }
  }

  // CORE-14184 //ODA_ASSERT_ONCE( !"Array entity dep not found" );
  return OdDbObjectId();
}

void OdDbImpAssocArrayActionBody::audit( OdDbObject* pObj, OdDbAuditInfo* pAuditInfo )
{
  OdDbHostAppServices* pHostApp = pObj->database()->appServices();

  OdDbAssocArrayActionBodyPtr pBody = OdDbAssocArrayActionBody::cast(pObj);
  
  //source BTR absent
  OdDbObjectId srcBTRid;
  if (pBody->parameters().get()) // CORE-16902
    srcBTRid = getArraySourceBTR( pBody->parentAction() );

  if ( srcBTRid.openObject().isNull() )
  {
    pAuditInfo->errorsFound( 1 );
    pAuditInfo->printError( pObj,
      pHostApp->formatMessage( sidAssArrAbsentSrcBTR ),
      pHostApp->formatMessage( sidVarValidNonZero ) );

    if ( pAuditInfo->fixErrors() )
    {
      OdDbAssocActionPtr pAction = pBody->parentAction().openObject( OdDb::kForWrite );
      pBody->erase();
      pAction->setActionBody( OdDbObjectId() );
      pAction->removeAllDependencies( true );
      pAction->erase();
      pAuditInfo->errorsFixed( 1 );
    }
  }

  if (pAuditInfo->fixErrors())
  {
    // CORE-15709
    OdDbBlockReferencePtr pArrayEnt = getArrayEntity(pBody->parentAction()).safeOpenObject();
    const OdDbObjectId& idLayer = pArrayEnt->layerId();

    OdArray <OdDbAssocArrayItemPtr> pItems;
    if (m_pParams->getItems(pItems) != eOk)
      return;

    OdArray<OdDbAssocArrayItemPtr>::iterator itItem;
    for (itItem = pItems.begin(); itItem != pItems.end(); itItem++)
    {
      if (!(*itItem)->isErased() && !(*itItem)->entityProvider().isNull() && !(*itItem)->entityProvider().isErased())
      {
        OdDbBlockReferencePtr pBlkRef = OdDbBlockReference::cast((*itItem)->entityProvider().safeOpenObject());
        if (pBlkRef->layerId() != idLayer)
        {
          pBlkRef->upgradeOpen();
          pBlkRef->setLayer(idLayer);
          pBlkRef->downgradeOpen();
        }
      }
    }
  }
}

OdDbImpAssocArrayModifyActionBody::OdDbImpAssocArrayModifyActionBody()
{

}

OdDbImpAssocArrayModifyActionBody::~OdDbImpAssocArrayModifyActionBody()
{

}

OdResult OdDbImpAssocArrayModifyActionBody::dwgInFields( OdDbDwgFiler* pFiler )
{
  OdResult res = OdDbImpAssocArrayActionBody::dwgInFields( pFiler );
  if ( res != eOk )
    return res;

  //pFiler->rdAddress();
  int ver = pFiler->rdInt16();
  int cnt = pFiler->rdInt32();

  int itemIndex;
  int rowIndex;
  int levelIndex;

  m_indices.resize( cnt );
  for ( int i = 0; i < cnt; i++ )
  {
    itemIndex = pFiler->rdInt32();
    rowIndex = pFiler->rdInt32();
    levelIndex  = pFiler->rdInt32();

    m_indices[i] = OdDbItemLocator( itemIndex, rowIndex, levelIndex );
  }

  return res;
}

void OdDbImpAssocArrayModifyActionBody::dwgOutFields( OdDbDwgFiler* pFiler ) const
{
  OdDbImpAssocArrayActionBody::dwgOutFields( pFiler );
  
  //pFiler->wrAddress();
  pFiler->wrInt16( 0 );
  pFiler->wrInt32( m_indices.size() );

  OdArray<OdDbItemLocator>::const_iterator itIndices;
  for ( itIndices = m_indices.begin(); itIndices != m_indices.end(); itIndices++ )
  {
    pFiler->wrInt32( (*itIndices)[OdDbItemLocator::kItemIndex] );
    pFiler->wrInt32( (*itIndices)[OdDbItemLocator::kRowIndex] );
    pFiler->wrInt32( (*itIndices)[OdDbItemLocator::kLevelIndex] );
  }
}

OdResult OdDbImpAssocArrayModifyActionBody::dxfInFields( OdDbDxfFiler* pFiler )
{
  return eOk;
}

void OdDbImpAssocArrayModifyActionBody::dxfOutFields( OdDbDxfFiler* pFiler ) const
{
  OdDbImpAssocArrayActionBody::dxfOutFields( pFiler );

  pFiler->wrSubclassMarker( OdDbAssocArrayModifyActionBody::desc()->name() );


  pFiler->wrInt16( 70, 0 );
  pFiler->wrInt32( 90, m_indices.size() );

  OdArray<OdDbItemLocator>::const_iterator it;
  for ( it = m_indices.begin(); it != m_indices.end(); it++ )
  {
    pFiler->wrInt32( 90, (*it)[OdDbItemLocator::kItemIndex] );
    pFiler->wrInt32( 90, (*it)[OdDbItemLocator::kRowIndex] );
    pFiler->wrInt32( 90, (*it)[OdDbItemLocator::kLevelIndex] );
  }  
}

void OdDbImpAssocArrayModifyActionBody::composeForLoad( OdDbObject *pObject, OdDb::SaveType format, OdDb::DwgVersion version, OdDbAuditInfo* pAuditInfo )
{

}

OdResult OdDbImpAssocArrayModifyActionBody::getOverridenItems( OdArray<OdDbItemLocator> &indices ) const
{
  indices = m_indices;
  return eOk;
}

OdResult OdDbImpAssocArrayModifyActionBody::setOverridenItems( const OdArray<OdDbItemLocator> &indices )
{
  m_indices = indices;
  return eOk;
}

bool OdDbImpAssocArrayModifyActionBody::controlsItem( const OdDbItemLocator& index ) const
{
  return m_indices.contains( index );  
}

OdResult OdDbImpAssocArrayModifyActionBody::createInstance( OdDbObjectId arrayId, const OdArray<OdDbItemLocator> &indices, const OdDbObjectIdArray &subEnt, OdDbVertexRef &basePoint, OdDbObjectId& actionBodyId )
{
  OdResult res = eOk;

  if ( subEnt.isEmpty() )
    return eInvalidInput;

  OdDbAssocArrayParametersPtr pModifyParam = OdDbAssocArrayModifyParameters::createObject();

  OdDbDatabase *pDb = subEnt.first().database();
  OdDbBlockTablePtr pMainBT = pDb->getBlockTableId().safeOpenObject( OdDb::kForWrite );

  OdDbEntityPtr pArrayEnt = arrayId.openObject();
  OdDbAssocArrayActionBodyPtr pMasterBody = OdDbImpAssocArrayActionBody::getControllingActionBody( pArrayEnt ).openObject();

  OdDbAssocActionPtr pMasterAction = pMasterBody->ownerId().openObject();

  OdDbAssocArrayRectangularParametersPtr pRectParam = OdDbAssocArrayRectangularParameters::cast( pMasterBody->parameters() );
  OdDbAssocArrayPolarParametersPtr pPolarParam = OdDbAssocArrayPolarParameters::cast( pMasterBody->parameters() );
  OdDbAssocArrayPathParametersPtr pPathParam = OdDbAssocArrayPathParameters::cast( pMasterBody->parameters() );


  OdDbObjectId networkId = pMasterAction->ownerId();
  OdDbAssocNetworkPtr pAssocNetwork = OdDbAssocNetwork::cast( networkId.openObject( OdDb::kForWrite ) );

  //create source BTR
  //BEGIN
  OdDbBlockTableRecordPtr pReplaceBTR = OdDbBlockTableRecord::createObject();
  pReplaceBTR->setName( L"*U" );
  OdDbObjectId blockId = pDb->addOdDbObject( pReplaceBTR, pMainBT->objectId() );

  OdDbIdMappingPtr pMapping = OdDbIdMapping::createObject();
  pMapping->setDestDb( pDb );
  pDb->deepCloneObjects( subEnt, blockId, *pMapping );
  //END
  
  //calculate transformation matrix to Zero state
  //BEGIN
  OdGeMatrix3d matExtenceToZeroState;
  matExtenceToZeroState.setToTranslation( OdGePoint3d::kOrigin - basePoint.point() ); 
  
  //transform source BTR to zero state
  //BEGIN
  OdDbObjectIteratorPtr pEntIter;
  for ( pEntIter = pReplaceBTR->newIterator(); !pEntIter->done(); pEntIter->step() )
  {
    OdDbEntityPtr pEnt = OdDbEntity::cast( pEntIter->entity( OdDb::kForWrite ) );
    pEnt->transformBy( matExtenceToZeroState );
  }
  //END

  //get destination BTR
  OdDbObjectIdArray objIDs;
  pMasterAction->getDependencies( false, true, objIDs );

  OdDbAssocDependencyPtr pDepDstBTR = OdDbAssocDependency::cast( objIDs[0].openObject( OdDb::kForWrite ) );
  OdDbBlockTableRecordPtr pDestinationBTR =
    OdDbBlockTableRecord::cast( pDepDstBTR->dependentOnObject().safeOpenObject( OdDb::kForWrite ) );  

  //create action and action body
  //BEGIN
  OdRxClass* pClass;
  pClass = (OdRxClass*)odrxClassDictionary()->getAt( OD_T( "AcDbAssocArrayModifyActionBody" ) ).get();

  OdDbObjectId actionId;

  if ( eOk != createActionAndActionBodyAndPostToDatabase( pClass, networkId, actionId, actionBodyId ) )
  {
    return eInvalidInput;
  }
  //END

  //append BTR and block ref
  pMainBT->add( pReplaceBTR );

  //set owner for array parameter 
  OdDbAssocArrayModifyActionBodyPtr pActionBody =
    OdDbAssocArrayModifyActionBody::cast( actionBodyId.openObject( OdDb::kForWrite ) );

  pActionBody->parameters() = pModifyParam;
  const_cast<OdDbAssocArrayParametersPtr &>(pModifyParam)->setOwner( pActionBody );

  //set items
  pActionBody->setOverridenItems( indices );

  OdDbAssocActionPtr pAction = OdDbAssocAction::cast( actionId.openObject( OdDb::kForWrite ) );
  pAction->setActionBody( actionBodyId );
 
  //create dependency
  //BEGIN

  //Destination block table record
  OdDbAssocDependencyPtr pDependency = OdDbAssocDependency::createObject();
  pDb->addOdDbObject( pDependency );
  pDependency->setIsReadDependency( true );
  pDependency->setIsWriteDependency( true );
  OdDbCompoundObjectId compoundObjectId;
  compoundObjectId.set( pDestinationBTR->objectId() );
  pDependency->attachToObject( compoundObjectId );
  pAction->addDependency( pDependency->objectId(), true );

  //Source block table record
  pDependency = OdDbAssocDependency::createObject();
  pDb->addOdDbObject( pDependency );
  pDependency->setIsReadDependency( true );
  pDependency->setIsWriteDependency( false );
  compoundObjectId.set( pReplaceBTR->objectId() );
  pDependency->attachToObject( compoundObjectId );
  pAction->addDependency( pDependency->objectId(), true );

  //END

  //create action parameters
  //BEGIN
  int paramIndex = 0;
  OdDbObjectId paramId;

  OdString paramName = OD_T( "basePoint" );
  pClass = (OdRxClass*)odrxClassDictionary()->getAt( OD_T( "AcDbAssocVertexActionParam" ) ).get();
  OdDbAssocVertexActionParamPtr pBasePoint;
  res = pAction->addParam( paramName, pClass, paramId, paramIndex );
  if ( eOk != res )
  {
    ODA_FAIL_ONCE();
    return res;
  }
  pBasePoint = OdDbAssocVertexActionParam::cast( paramId.openObject( OdDb::kForWrite ) );

  if ( !basePoint.entity().isEmpty() )
  {
    OdDbObjectPtr pCentroidObj = basePoint.entity().topId().openObject();

    OdDbIdPair pair;
    pair.setKey( pCentroidObj->objectId() );
    pMapping->compute( pair );

    OdDbSubentRefPtr pSubRef = OdDbSubentRef::cast( basePoint.referencedRef() );

    OdDbVertexRefPtr pBasePointRef =
      OdDbVertexRef::createObject( pair.value(), pSubRef->subentId(), basePoint.point() );

    res = pBasePoint->setVertexRef( *pBasePointRef );
    if ( eOk != res )
    {
      return res;
    }
  }
  //END

  //erasing of source entities
  OdDbObjectIdArray::const_iterator it_Scr;
  for ( it_Scr = subEnt.begin(); it_Scr != subEnt.end(); it_Scr++ )
  {
    OdDbObjectPtr pObj = (*it_Scr).safeOpenObject( OdDb::kForWrite );
    pObj->erase( true );
    pObj->downgradeOpen();
  }
  //END

  pAction->setStatus( kChangedDirectlyAssocStatus, true );

  return res;
}

void OdDbImpAssocArrayModifyActionBody::evaluateOverride( OdDbObjectId parentActionId )
{
  OdDbAssocActionPtr pParentAction = parentActionId.openObject();
  m_pParams->setOwner(OdDbAssocArrayActionBodyPtr(pParentAction->actionBody().safeOpenObject()));

  OdDbObjectIdArray depIds;
  pParentAction->getDependencies( true, false, depIds );

  OdDbAssocDependencyPtr pSourceDep = depIds[1].openObject();
  ODA_ASSERT_ONCE( !pSourceDep.isNull() );

  switch ( pSourceDep->status() )
  {
    case kErasedAssocStatus:
      pParentAction->setStatus( kErasedAssocStatus, false );
      break;

    case kChangedDirectlyAssocStatus:       // Explicitly changed
    case kChangedTransitivelyAssocStatus:   // Changed indirectly due to changes
    case kChangedNoDifferenceAssocStatus:   // No real change (only forced to evaluate)
      pSourceDep->upgradeOpen();
      pSourceDep->setStatus( kIsUpToDateAssocStatus, true );
      break;
  }

  depIds.clear();
  pParentAction->getDependencies( true, true, depIds );

  OdDbAssocDependencyPtr pDestDep = depIds[0].openObject();
  ODA_ASSERT_ONCE( !pDestDep.isNull() );
  if (pDestDep->status() != kErasedAssocStatus)
  {
    OdDbBlockTableRecordPtr pDestBTR = pDestDep->dependentOnObject().openObject();
    bool erase = pDestBTR.isNull();
    if (!erase)
    {
      OdDbObjectIdArray aDestInsertIds;
      pDestBTR->getBlockReferenceIds(aDestInsertIds);
      erase = aDestInsertIds.isEmpty();
    }
    if (erase)
    {
      pDestDep->upgradeOpen();
      pDestDep->setStatus(kErasedAssocStatus, false);
    }
  }

  switch ( pDestDep->status() )
  {
    case kErasedAssocStatus:
      {
        OdDbBlockTableRecordPtr pSrcBTR = pSourceDep->dependentOnObject().openObject( OdDb::kForWrite );
        if (pSourceDep.get()) {
          OdDbObjectIteratorPtr pOjIt = pSrcBTR->newIterator();
          for ( ; !pOjIt->done(); pOjIt->step() )
          {
            pOjIt->objectId().openObject( OdDb::kForWrite )->erase();
          }
          pSrcBTR->erase(true);
          pSrcBTR->downgradeOpen();
        }
        pParentAction->removeAllDependencies( true );
        pParentAction->setStatus( kErasedAssocStatus, false );
      }
      return;

    case kChangedDirectlyAssocStatus:       // Explicitly changed
    case kChangedTransitivelyAssocStatus:   // Changed indirectly due to changes
    case kChangedNoDifferenceAssocStatus:   // No real change (only forced to evaluate)
      pDestDep->upgradeOpen();
      pDestDep->setStatus( kIsUpToDateAssocStatus, false );
      break;
  }

  restoreSrcPosition( pParentAction );

  OdArray<OdDbAssocArrayItemPtr> items;
  if (m_pParams->getItems(items) != eOk)
  {
    pParentAction->upgradeOpen();
    pParentAction->setStatus(kFailedToEvaluateAssocStatus);
    return;
  }

  OdArray<OdDbAssocArrayItemPtr>::iterator it;
  for ( it = items.begin(); it != items.end(); it++ )
  {
    OdDbBlockReferencePtr pBlkRef = (*it)->entityProvider().openObject( OdDb::kForWrite );
    if ( pBlkRef.isNull() )
      continue;

    pBlkRef->setBlockTableRecord( getSourceBTR( parentActionId )->objectId() );
  }

  pParentAction->upgradeOpen();
  if ( items.empty() )
  {
    OdDbBlockTableRecordPtr pSrcBTR = pSourceDep->dependentOnObject().openObject( OdDb::kForWrite );
    pSrcBTR->erase();
    pParentAction->removeAllDependencies( true );
    pParentAction->setStatus( kErasedAssocStatus, false );
  }
  else
  {
    pParentAction->setStatus( kIsUpToDateAssocStatus );
  }
  
}

void OdDbImpAssocArrayModifyActionBody::restoreSrcPosition( OdDbAssocAction* pParentAction )
{
  OdDbAssocActionParamPtr pParamBasePoint = OdDbAssocActionParam::cast(
            pParentAction->paramAtName( OD_T( "basePoint" ) ).openObject( OdDb::kForWrite ) );
  ODA_ASSERT_ONCE( !pParamBasePoint.isNull() );

  OdDbObjectIdArray depIds;
  pParamBasePoint->getDependencies( true, false, depIds );
  switch ( depIds.length() )
  {
    case 1:
    {
      OdDbAssocGeomDependencyPtr pDepBasePoint = OdDbAssocGeomDependency::cast(
        depIds.first().openObject() );
      switch ( pDepBasePoint->status() )
      {
        //begin CHANGE
        case kChangedDirectlyAssocStatus:
        {
          OdDbObjectId objId = pDepBasePoint->dependentOnObject();
          OdDbEntityPtr pObj = OdDbEntity::cast( objId.openObject() );
          if ( pObj.isNull() )
          {
            ODA_ASSERT_ONCE( !"AssocArray->KeyPoint: dependentOnObject absent" );
            throw OdError(eNullObjectPointer);
          }

          OdSmartPtr<OdDbAssocPersSubentIdPE> pPE =
            pObj->isA()->getX( OdDbAssocPersSubentIdPE::desc() );
          if ( pPE.isNull() )
          {
            ODA_ASSERT_ONCE( !"AssocArray->KeyPoint: OdDbAssocPersSubentIdPE is null" );
            throw OdError( eNullObjectPointer );
          }

          OdGePoint3d newPos;

          OdDbAssocSimplePersSubentIdPtr pSimplePersSubentId = OdDbAssocSimplePersSubentId::cast(
            pDepBasePoint->persistentSubentId() );
          if ( !pSimplePersSubentId.isNull() )
          {
            pSimplePersSubentId->subentType( NULL );

            pPE->getVertexSubentityGeometry( pObj, pSimplePersSubentId->subentId(NULL), newPos );
          }

          OdDbObjectIdArray srcEnt = getSourceEntities( pParentAction->objectId() );
          OdDbObjectIdArray::iterator it;
          for ( it = srcEnt.begin(); it != srcEnt.end(); it++ )
          {
            OdDbEntityPtr pEnt = it->openObject( OdDb::kForWrite );
            pEnt->transformBy( OdGeMatrix3d().setToTranslation( OdGePoint3d::kOrigin - newPos ) );
            pEnt->downgradeOpen();
          }

          pDepBasePoint->upgradeOpen();
          pDepBasePoint->setStatus( kIsUpToDateAssocStatus, false );
        }
          break;
        //end CHANGE

        //begin ERASE
        case kErasedAssocStatus:
          pParentAction->removeDependency( pDepBasePoint->objectId(), true );
          break;
        //end ERASE

        default:
          break;
      }
    }
      break;

    case 0: break;

    default:
      ODA_ASSERT_ONCE( !"pParamBasePoint->getDependencies() returned more than one id" );
  }
}

OdDbObjectId OdDbImpAssocArrayModifyActionBody::getArrayEntity( OdDbObjectId parentActionId ) const
{
  OdDbAssocActionPtr pParentAction = parentActionId.safeOpenObject();

  //get master action
  OdDbObjectIdArray depIds;
  pParentAction->getDependencies( true, true, depIds );

  OdDbAssocDependencyPtr pDep = depIds[0].safeOpenObject();
  OdDbBlockTableRecordPtr pDestBTR = OdDbBlockTableRecord::cast( pDep->dependentOnObject().openObject() );
  ODA_ASSERT_ONCE( !pDestBTR.isNull() );

  OdDbAssocActionPtr pMasterAction;
  OdDbObjectIdArray reactors = pDestBTR->getPersistentReactors();
  OdDbObjectIdArray::iterator itReact;
  for ( itReact = reactors.begin(); itReact != reactors.end(); itReact++ )
  {
    OdDbAssocDependencyPtr pMasterDep = OdDbAssocDependency::cast( itReact->openObject() );
    if ( !pMasterDep.isNull() )
    {
      if ( !pMasterDep->isReadDependency() )
      {
        pMasterAction = pMasterDep->owningAction().safeOpenObject();
        break;
      }
    }
  }

  OdDbAssocArrayActionBodyPtr pMasterBody;

  if ( !pMasterAction.isNull())
    pMasterBody = pMasterAction->actionBody().safeOpenObject();

  return pMasterBody.isNull() ? OdDbObjectId::kNull : pMasterBody->getArrayEntity();
}

OdResult OdDbImpAssocArrayModifyActionBody::addMoreObjectsToDeepCloneOverride(
  const OdDbAssocActionBody* thisBody, OdDbIdMapping& idMap, OdDbObjectIdArray& additionalObjectsToClone) const
{
  OdDbIdPair actionIdPair(thisBody->parentAction());
  OdDbIdPair arrEntIdPair(getArrayEntity(actionIdPair.key()));
  if (idMap.compute(arrEntIdPair) && arrEntIdPair.isCloned()) {
    OdDbAssocActionPtr action = actionIdPair.key().safeOpenObject();
    if (!idMap.compute(actionIdPair) || !actionIdPair.isCloned()) {
      const OdDbAssocArrayModifyActionBody* body = (const OdDbAssocArrayModifyActionBody*)thisBody;
      OdDbBlockReferencePtr ent = arrEntIdPair.value().openObject();
      if (ent.get()) {
        OdDbObjectPtr owner = OdDbAssocNetCloneCtx::lookupActionCloneOwner(idMap, ent);
        if (!owner.isNull())
          OdDbAssocNetCloneCtx::cloneObject(idMap, action, owner);
      }
    }
    ::addContentBlocks(action, additionalObjectsToClone);
  }
  return eOk;
}

