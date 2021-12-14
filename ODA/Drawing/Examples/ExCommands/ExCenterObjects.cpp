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

// ExLine.cpp: implementation of the Center Objects (CenterMark and Center Line) function.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"

#include "DbAssocManager.h"
#include "DbAssocAction.h"
#include "DbHostAppServices.h"
#include "DbObjectId.h"
#include "DbLine.h"
#include "DbCenterLineActionBody.h"
#include "DbCenterMarkActionBody.h"
#include "DbImpCenterLineActionBody.h"
#include "DbSmartCenterObjectsParameters.h"
#include "DbAssocRotatedDimActionBody.h"
#include "DbAssocAlignedDimActionBody.h"
#include "DbAssocLoftedSurfaceActionBody.h"
#include "DbAssocBlendSurfaceActionBody.h"

#include "Ed/EdCommandStack.h"
#include "../Editor/OSnapManager.h"
#include "OdDbAssocCurvePersSubentIdPE.h"

#include "DbBlockReference.h"
#include "DbSubentId.h"
#include "ExSelectionUtils.h"
#include "DbAssocNetwork.h"

OdGePoint3d SetPointFromBlock(OdDbBlockReferencePtr bRef, int numberLine, bool  isStartPoint)
{
  OdGePoint3d retPoint;
  OdDbBlockTableRecordPtr pBR = bRef->blockTableRecord().openObject();
  OdDbObjectIteratorPtr pObjIt = pBR->newIterator();
  int counter = 0;

  for (; !pObjIt->done(); pObjIt->step())
  {
    OdDbObjectId tempId = pObjIt->entity()->objectId();

    OdDbLinePtr pLineInBlock = OdDbLine::cast(tempId.openObject(OdDb::kForWrite));

    if (!pLineInBlock.isNull())
    {
      if (counter == numberLine)
      {
        retPoint = (isStartPoint) ? pLineInBlock->startPoint() : pLineInBlock->endPoint();
        break;
      }
    }

    counter++;
  }
  OdGePoint3d blockPosition = bRef->position();
  OdGeMatrix3d matrix = bRef->blockTransform();
  retPoint.transformBy(matrix);

  return retPoint;
}

void _CenterMark_func(OdEdCommandContext* pCmdCtx)
{
  OdRxModulePtr pConstrMod = ::odrxDynamicLinker()->loadModule(L"DbConstraints", false);
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();
  OdGePoint3dArray ptsSelected;
  OdDbSelectionSetPtr pSSet;
  OdDbFullSubentPath path = selectedFullSubentPath(pIO, pDb, L"Select circle, arc, or arc segment:", ptsSelected, pSSet);

  ODA_ASSERT(path.subentId().type() != OdDb::kFaceSubentType);

  OdResult res = eInvalidInput;
  OdDbObjectId bodyId;

  pDb->startTransaction();

  try
  {
    OdDbObjectId arrayId;
  
    if (path.objectIds().length() > 0)
      res = OdDbCenterMarkActionBody::createInstance(path, arrayId, bodyId);
  }

  catch (.../*OdError& e*/)
  {
    res = eInvalidInput;
  }
  
  if (res == eOk)
  {
    pDb->endTransaction();
    if (!bodyId.isNull())
    {
      OdDbCenterMarkActionBodyPtr pActionBody =
        OdDbAssocActionBody::cast(bodyId.openObject(OdDb::kForWrite));

      if (!pActionBody.isNull())
      {
        OdDbObjectId currentActionId = pActionBody->ownerId();// pAction->owningNetwork();

        OdDbAssocNetworkPtr pNetOwner = OdDbAssocNetwork::cast(pActionBody->owningNetwork().openObject(OdDb::kForRead));

        OdDbObjectIdArray arrActions = pNetOwner->getActions();

        for (OdUInt32 counter = 0; counter < arrActions.length(); counter++)
        {
          OdDbAssocActionPtr pTempAction = OdDbAssocAction::cast(arrActions[counter].openObject(OdDb::kForWrite));

          if (!pTempAction.isNull() && arrActions[counter] != currentActionId)
            pTempAction->setStatus(kIsUpToDateAssocStatus, false);
        }
      }
    }

    OdDbAssocManager::evaluateTopLevelNetwork(pDb);
    pIO->putString(L"Ready");
  }
  else
  {
    pDb->abortTransaction();
    pIO->putString(L"Failed");
  }
}

void _CenterLine_func(OdEdCommandContext* pCmdCtx)
{
  OdRxModulePtr pConstrMod = ::odrxDynamicLinker()->loadModule(L"DbConstraints", false);

  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();
  OdDbHostAppServices* pSvs = pDb->appServices();

  OdDbObjectId spaceId = ExCommandsUtils::activeBlockTableRecord(pDb);
  OdDbBlockTableRecordPtr pSpace = spaceId.safeOpenObject(OdDb::kForWrite);

  //OdGeVector3d vNormal(ExCommandsUtils::ucsNormal(pDb));

  OdGePoint3dArray ptsSelected;
  OdString strInvitation = L"Select first segment:";
  OdDbFullSubentPathArray entPathArray;
  OdDbSelectionSetPtr pSSet;

  while (true)
  {
    OdDbFullSubentPath path = selectedFullSubentPath(pIO, pDb, strInvitation, ptsSelected, pSSet);
    ODA_ASSERT(path.subentId().type() != OdDb::kFaceSubentType);

    if (path.objectIds().length() > 0 && path.subentId().type() != OdDb::kFaceSubentType)
    {
      entPathArray.append(path);

      if (entPathArray.length() == 2)
        break;

      strInvitation = L"Select second segment:";
    }
  }

  pDb->startTransaction();
  OdResult res = eInvalidInput;
  OdDbObjectId idCenterLine;
  OdDbObjectId bodyId;

  try
  {
    OdDbObjectId lineId;
    res = OdDbCenterLineActionBody::createInstance(entPathArray, /*ptsSelected,*/ lineId, bodyId);
  }

  catch (...)
  {
    res = eInvalidInput;
  }

  if (res == eOk)
  {
    pDb->endTransaction();

    if (!bodyId.isNull()) // TODO
    {
      OdDbCenterLineActionBodyPtr pActionBody =
        OdDbAssocActionBody::cast(bodyId.openObject(OdDb::kForWrite));

      if (!pActionBody.isNull())
      {
        OdDbObjectId currentActionId = pActionBody->ownerId();// pAction->owningNetwork();

        OdDbAssocNetworkPtr pNetOwner = OdDbAssocNetwork::cast(pActionBody->owningNetwork().openObject(OdDb::kForRead));

        OdDbObjectIdArray arrActions = pNetOwner->getActions();

        for (OdUInt32 counter = 0; counter < arrActions.length(); counter++)
        {
          OdDbAssocActionPtr pTempAction = OdDbAssocAction::cast(arrActions[counter].openObject(OdDb::kForWrite));

          if (!pTempAction.isNull() && arrActions[counter] != currentActionId)
            pTempAction->setStatus(kIsUpToDateAssocStatus, false);
        }
      }
    }

    OdDbAssocManager::evaluateTopLevelNetwork(pDb);
    pIO->putString(L"Ready");
  }
  else
  {
    pDb->abortTransaction();
    pIO->putString(L"Failed");
  }
}

//bool _CreateAssocRotatedDim_selectEntity(OdDbUserIO* pIO, OdDbSelectionSetPtr &pSSet, OdGePoint3dArray* ptsOfSelEntityPointer, bool isFirstEnt)
//{
//	struct ArrayFilter : public OdStaticRxObject<OdEdSSetTracker>
//	{
//		virtual int addDrawables(OdGsView*) { return 0; }
//		virtual void removeDrawables(OdGsView*) {}
//
//		virtual bool append(const OdDbObjectId& entId, const OdDbSelectionMethod*)
//		{
//			OdDbEntityPtr pEnt = OdDbEntity::cast(entId.safeOpenObject(OdDb::kForRead));
//			if (pEnt.isNull())
//				return false;
//
//			return OdDbAssocArrayActionBody::isAssociativeArray(pEnt);
//		}
//		virtual bool remove(const OdDbObjectId&, const OdDbSelectionMethod*)
//		{
//			return true;
//		}
//	} arrayFilter;
//		
//	OdString message = isFirstEnt ? L"Select first point for assoc rotateds dimension:" : L"Select second point for assoc rotateds dimension:";
//		pSSet = pIO->select(message, OdEd::kSelAllowEmpty | OdEd::kSelSinglePass | OdEd::kSelAllowSubentsAlways,//OdEd::kSelSingleEntity | 
//			NULL, //const OdSelectionSet* pDefVal,
//			L"",//const OdString& sKeywords,
//			NULL, //OdSSetTracker* pBaseTracker,
//			ptsOfSelEntityPointer //OdGePoint3dArray* ptsPointer)
//			);// , 0, OdString::kEmpty, &arrayFilter);
//	
//	return true;
//}

void _AssocRotatedAlignedDimension_func(OdEdCommandContext* pCmdCtx)
{
	OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
	OdDbDatabasePtr pDb = pDbCmdCtx->database();
	OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();

	OdDbObjectId spaceId = ExCommandsUtils::activeBlockTableRecord(pDb);
	OdDbBlockTableRecordPtr pSpace = spaceId.safeOpenObject(OdDb::kForWrite);

	//OdGeVector3d vNormal(ExCommandsUtils::ucsNormal(pDb));

	OdDbSelectionSetPtr pSArray1, pSArray2;
	OdGePoint3dArray ptsOfSelEntity;
  OdGePoint3d pointUdefined(DBL_EPSILON, DBL_EPSILON, DBL_EPSILON);

  OdInt16 nArrayType = -1;
  
  try
  {
    nArrayType = pIO->getKeyword(L"Enter dimension type[Rotated/Aligned]", L"Rotated Aligned");

    switch (nArrayType)
    {
      case 0:
      case 1:
      {
        break;
      }

      default:
        return;
    }
  }
  catch (...)
  {
    bool bOk = false;
  }

  OdGePoint3dArray ptsSelected;
  OdDbFullSubentPathArray entPathArray;
  OdString strInvitation = L"Select first point for assoc rotateds dimension:";
  OdDbSelectionSetPtr pSSet;

  while (true)
  {
    OdDbFullSubentPath path = selectedFullSubentPath(pIO, pDb, strInvitation, ptsSelected, pSSet);

    if (path.objectIds().length() > 0)
    {
      entPathArray.append(path);

      if (entPathArray.length() == 2)
        break;

      strInvitation = L"Select second point for assoc rotateds dimension : ";
    }
  }
  ///////////////////////////////////////////////////////////
  const OdGePoint3d dimLinePt = pIO->getPoint(L"Specify point of dimension line");
  ptsSelected.append(dimLinePt);
	pDb->startTransaction();
  OdResult res = eInvalidInput;
  OdDbObjectId arrayAssocId, bodyAssocId;

	try
	{
    if (0 == nArrayType)
    {
      res = OdDbAssocRotatedDimActionBody::createInstance(entPathArray, ptsSelected, bodyAssocId);
    }
    if (1 == nArrayType)
    {
      res = OdDbAssocAlignedDimActionBody::createInstance(entPathArray, ptsSelected, bodyAssocId);
    }

    if (bodyAssocId.isNull())
      res = eInvalidInput;
	}
	catch (...)
	{
    res = eInvalidInput;
	}

	if (res == eOk)
	{
		if (!bodyAssocId.isNull())
		{
      OdDbAssocRotatedDimActionBodyPtr assocRotDimBodyPtr = OdDbAssocRotatedDimActionBody::cast(bodyAssocId.openObject(OdDb::kForWrite));

      if (!assocRotDimBodyPtr.isNull())
      {
        OdDbAssocActionPtr pAction = assocRotDimBodyPtr->parentAction().openObject(OdDb::kForWrite);
        pAction->setStatus(kChangedDirectlyAssocStatus);
        pAction->downgradeOpen();
      }
		}

		pDb->endTransaction();
		OdDbAssocManager::evaluateTopLevelNetwork(pDb);
		::odedRegCmds()->executeCommand(OD_T("REGEN"), pCmdCtx);

		pIO->putString(OD_T("Ready"));
	}
	else
	{
		pDb->abortTransaction();
		pIO->putString(OD_T("Failed. Check selected entities."));
	}
}

void _Loft_func(OdEdCommandContext* pCmdCtx)
{
  OdRxModulePtr pConstrMod = ::odrxDynamicLinker()->loadModule(L"DbConstraints", false);
  OdRxModulePtr pSpaModeler = ::odrxDynamicLinker()->loadModule(L"SpaModeler", false);
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();
  //OdDbObjectId spaceId = ExCommandsUtils::activeBlockTableRecord(pDb);
  //OdDbBlockTableRecordPtr pSpace = spaceId.safeOpenObject(OdDb::kForWrite);
  OdDbSelectionSetPtr pSSet;
  //OdDbObjectIdArray selectedSetArray;

  OdGePoint3dArray ptsSelected;
	OdDbFullSubentPathArray entPathArray, guideCurves;
	OdDbFullSubentPath pathCurve;
	const OdDbLoftOptions loftOptions;
  OdDbFullSubentPath path;

  do
  {
    OdString strInvitation = entPathArray.length() < 1 ? L"Select cross section entities for lofting:" :
      L"Select cross section entities for lofting or press ENTER to finish:";

    if (entPathArray.length() > 0)
    {
	    const OdString strPrefics = OdString().format(OD_T("%d"), entPathArray.length()) + L" entities selected. ";
      strInvitation = strPrefics + strInvitation;
    }
    
    path = selectedFullSubentPath(pIO, pDb, strInvitation, ptsSelected, pSSet);

    if (path.objectIds().length() > 0)
    {
      entPathArray.append(path);
    }

    //pSSet = pIO->select(strInvitation,
    //OdEd::kSelAllowSubents | OdEd::kSelSinglePass    );
    
    //selectedSetArray.append(pSSet->objectIdArray());
  } while (path.objectIds().length() != 0);// pSSet->numEntities());

  if (entPathArray.length() < 2)
  {
    return;
  }

  OdResult res = eOk;

  pDb->startTransaction();

  OdString strMessage(L"Ready");
  try
  {
    OdDbObjectId arrayAssocId, bodyAssocId;

		res = OdDbAssocLoftedSurfaceActionBody::createInstance(entPathArray, guideCurves,
			pathCurve,
			loftOptions,
			bodyAssocId);
  }
  catch (const OdError& err)
  {
    res = eInvalidInput;
    strMessage = L"Failed: ";
    strMessage += err.description();
  }
  catch (...)
  {
    res = eInvalidInput;
    strMessage = L"Failed: Unknown error";
  }

  if (res == eOk)
  {
    pDb->endTransaction();
    OdDbAssocManager::evaluateTopLevelNetwork(pDb);
  }
  else
  {
    pDb->abortTransaction();
  }
  pIO->putString(strMessage);
}

void _BlendSurface_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();
  OdDbSelectionSetPtr pSSet;
  OdGePoint3dArray ptsSelected;
  OdDbFullSubentPathArray entPathArray;
  OdDbFullSubentPath path;

  do
  {
    OdString strInvitation = entPathArray.length() < 1 ? L"Select cross section entities for lofting:" :
      L"Select cross section entities for lofting or press ENTER to finish:";

    if (entPathArray.length() > 0)
    {
	    const OdString strPrefics = OdString().format(OD_T("%d"), entPathArray.length()) + L" lines selected. ";
      strInvitation = strPrefics + strInvitation;
    }

    path = selectedFullSubentPath(pIO, pDb, strInvitation, ptsSelected, pSSet);

    if (path.objectIds().length() > 0)
    {
      entPathArray.append(path);
    }

    //pSSet = pIO->select(strInvitation,
    //OdEd::kSelAllowSubents | OdEd::kSelSinglePass    );

    //selectedSetArray.append(pSSet->objectIdArray());
  } while (path.objectIds().length() != 0);// pSSet->numEntities());

  //if (entPathArray.length() < 2)
  //{
  //  return;
  //}

  OdResult res = eOk;

  pDb->startTransaction();

  try
  {
    OdDbObjectId arrayAssocId, bodyAssocId;
    res = OdDbAssocBlendSurfaceActionBody::createInstance(entPathArray, ptsSelected, arrayAssocId, bodyAssocId);
  }

  catch (.../*OdError& e*/)
  {
    res = eInvalidInput;
  }

  if (res == eOk)
  {
    pDb->endTransaction();
    OdDbAssocManager::evaluateTopLevelNetwork(pDb);
    pIO->putString(L"Ready");
  }
  else
  {
    pDb->abortTransaction();
    pIO->putString(L"Failed");
  }
}
