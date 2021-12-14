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
#include "DbLeader.h"
#include "DbFcf.h"
#include "DbMLeader.h"
#include "DbAssocArrayActionBody.h"
#include "DbSmartCenterActionBody.h"
#include "DbBlockReference.h"
#include "DbImpAssocMLeaderActionBody.h"
#include "ExSelectionUtils.h"

void _MLeader_func(OdEdCommandContext* pCmdCtx)
{
	OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
	OdDbDatabasePtr pDb = pDbCmdCtx->database();
	OdDbUserIO* pIO     = pDbCmdCtx->dbUserIO();

	int llIndex;

	OdGePoint3d first  = pIO->getPoint("Set first point");
	OdGePoint3d second = pIO->getPoint("Set second point");
	OdString strLabel  = pIO->getString("Set label");

	if (first == second) {
		pIO->putError("The points coincide");
		return;
	}

	if (strLabel.isEmpty()) {
		pIO->putError("MLeader text is empty");
		return;
	}

	OdDbObjectId spaceId = ExCommandsUtils::activeBlockTableRecord(pDb);
	OdDbBlockTableRecordPtr pSpace = spaceId.safeOpenObject(OdDb::kForWrite);

	OdDbMLeaderPtr pMLeader = OdDbMLeader::createObject();
	pMLeader->setDatabaseDefaults(pDb);
	pSpace->appendOdDbEntity(pMLeader);

	OdDbMTextPtr pMText = OdDbMText::createObject();
	pMText->setDatabaseDefaults(pDb);
	pMLeader->setEnableFrameText(true);
	pMText->setContents(strLabel);
	pMText->setLocation(second);
  pMText->setHeight(10.0);

	pMLeader->setMText(pMText);
	pMLeader->addLeaderLine(first, llIndex);
}

//bool FindSlectedEntityForAssocMLeader(OdDbUserIO* pIO, OdDbSelectionSetPtr &pSSet, OdGePoint3dArray* ptsOfSelEntityPointer, bool isFirstEnt)
//{
//  struct ArrayFilter : public OdStaticRxObject<OdEdSSetTracker>
//  {
//    virtual int addDrawables(OdGsView*) { return 0; }
//    virtual void removeDrawables(OdGsView*) {}
//
//    virtual bool append(const OdDbObjectId& entId, const OdDbSelectionMethod*)
//    {
//      OdDbEntityPtr pEnt = OdDbEntity::cast(entId.safeOpenObject(OdDb::kForRead));
//      if (pEnt.isNull())
//        return false;
//
//      return OdDbAssocArrayActionBody::isAssociativeArray(pEnt);
//    }
//    virtual bool remove(const OdDbObjectId&, const OdDbSelectionMethod*)
//    {
//      return true;
//    }
//  } arrayFilter;
//
//  OdString message = isFirstEnt ? L"Select first point for assoc rotateds dimension:" : L"Select second point for assoc rotateds dimension:";
//  pSSet = pIO->select(message, OdEd::kSelSingleEntity | OdEd::kSelSinglePass,//
//    NULL, //const OdSelectionSet* pDefVal,
//    L"",//const OdString& sKeywords,
//    NULL, //OdSSetTracker* pBaseTracker,
//    ptsOfSelEntityPointer //OdGePoint3dArray* ptsPointer)
//    );// , 0, OdString::kEmpty, &arrayFilter);
//
//  return true;
//}

void _AssocMLeader_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();
  OdDbSelectionSetPtr pSmartObject;
  OdDbSmartCenterActionBodyPtr smartBody;
  OdGePoint3dArray ptsSelected;
  OdDbObjectId     selectedEntId;
  OdDbBlockReferencePtr pBody;
  OdDbObjectId bodyId;
  OdDbFullSubentPathArray entPathArray;

  OdDbSubentId SentId;

  //if (false == FindSlectedEntityForAssocMLeader(pIO, pSmartObject, &ptsOfSelEntity, true))
  //  return;

  OdDbFullSubentPath path = selectedFullSubentPath(pIO, pDb, L"Select first point for assoc rotateds dimension:", ptsSelected, pSmartObject);

  if (path.objectIds().length() > 0)
  {
    entPathArray.append(path);
  }

  if (!pSmartObject->objectIdArray().isEmpty())
  {
    selectedEntId = pSmartObject->objectIdArray().first();
    pBody = OdDbBlockReference::cast(selectedEntId.openObject(OdDb::kForRead));

    if (!pBody.isNull())
    {
      bodyId = OdDbAssocArrayActionBody::getControllingActionBody(pBody);

      if (!bodyId.isNull())
        smartBody = OdDbSmartCenterActionBody::cast(bodyId.openObject());
    }
  }

  if (smartBody.isNull())
  {
    pIO->putError("This is not smart object, try again");
    return;
  }

  OdGePoint3d second = pIO->getPoint("Set second point");
  OdString strLabel = pIO->getString("Set label");

  if (ptsSelected.first() == second)
  {
    pIO->putError("The points coincide");
    return;
  }
  
  ptsSelected.append(second);

  if (strLabel.isEmpty()) 
  {
    pIO->putError("MLeader text is empty");
    return;
  }

  OdDbObjectId arrayId, actionBodyId;

  if (OdDbImpAssocMLeaderActionBody::createInstance(entPathArray, smartBody->objectId(), ptsSelected, strLabel, actionBodyId) != eOk)
  {
    pIO->putError("Object can't be created");
  }
}
