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
#include "Brep_LineContainment_Command.h"

#include "DbDatabase.h"
#include "DbEntity.h"
#include "DbCommandContext.h"
#include "DbSSet.h"
#include "DbBlockTableRecord.h"
#include "DbBlockReference.h"
#include "DbHostAppServices.h"
#include "Ge/GeScale3d.h"
#include "DbErrorInvalidSysvar.h"
#include "SysVarInfo.h"
#include "StaticRxObject.h"
#include "DbUserIO.h"
#include "Db3dSolid.h"
#include "DbSurface.h"
#include "DbLine.h"

#include "Br/BrBrep.h"
#include "Br/BrHit.h"

static void getLineContainment(OdDbBlockTableRecordPtr pMs, OdBrBrep& brBrep, OdGeLine3d& line3d)
{
  OdUInt32   iNumHitsFound;
  OdBrHit*   pBrHit = NULL;
  if(odbrOK == brBrep.getLineContainment(line3d, 5, iNumHitsFound, pBrHit))
  {
    for (OdUInt32 i = 0; i < iNumHitsFound; i++)
    {
      OdGePoint3d ptHit;
      if(odbrOK == pBrHit[i].getPoint(ptHit))
      {
        bool bTmp = false;
        OdDb3dSolidPtr pSphere = OdDb3dSolid::createObject();
        pSphere->createSphere(0.1);
        pSphere->transformBy(OdGeMatrix3d::translation( ptHit.asVector() ) );
        pSphere->setColorIndex(1);
        pMs->appendOdDbEntity(pSphere);
      }
    }
    if (pBrHit)
      delete[] pBrHit;
  }
}

void BrepLineContainmentCommand::execute(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabase* pDatabase = pDbCmdCtx->database();
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();
  OdDbBlockTableRecordPtr pMs = pDatabase->getModelSpaceId().safeOpenObject(OdDb::kForWrite);

  OdDbSelectionSetPtr pFirstSel = pIO->select(OD_T("Select 3dSolid"), OdEd::kSelSingleEntity);

  if (pFirstSel->numEntities() == 0)
  {
    pIO->putString(OD_T("no solid selected."));
    return;
  }

  OdDbSelectionSetIteratorPtr pIter = pFirstSel->newIterator();
  OdDbObjectId objId = pIter->objectId();
  OdDbEntityPtr p3dSolid = objId.openObject(OdDb::kForWrite);

  if (!p3dSolid.isNull() && p3dSolid->isKindOf(OdDb3dSolid::desc()))
  {
    OdGePoint3d ptStart = pIO->getPoint(OD_T("Specify start line point:"));
    OdGePoint3d ptEnd   = pIO->getPoint(OD_T("Specify end line point:"));
    OdDbLinePtr pLine = OdDbLine::createObject();
    pLine->setStartPoint(ptStart);
    pLine->setEndPoint(ptEnd);
    pMs->appendOdDbEntity(pLine);

    OdBrBrep brBrep;
    OdDb3dSolidPtr(p3dSolid)->brep(brBrep);

    OdGeLine3d line3d(ptStart, ptEnd);
    getLineContainment(pMs, brBrep, line3d);
  }
}

void BrepSurfaceLineContainmentCommand::execute(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabase* pDatabase = pDbCmdCtx->database();
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();
  OdDbBlockTableRecordPtr pMs = pDatabase->getModelSpaceId().safeOpenObject(OdDb::kForWrite);

  OdDbSelectionSetPtr pFirstSel = pIO->select(OD_T("Select a Surface"), OdEd::kSelSingleEntity);

  if (pFirstSel->numEntities() == 0)
  {
    pIO->putString(OD_T("A surface not selected."));
    return;
  }

  OdDbSelectionSetIteratorPtr pIter = pFirstSel->newIterator();
  OdDbObjectId objId = pIter->objectId();
  OdDbEntityPtr pSurface = objId.openObject(OdDb::kForWrite);

  if (!pSurface.isNull() && pSurface->isKindOf(OdDbSurface::desc()))
  {
    OdDbSelectionSetPtr pFirstSel = pIO->select(OD_T("Select Line"), OdEd::kSelSingleEntity);

    if (pFirstSel->numEntities() == 0)
    {
      pIO->putString(OD_T("no line selected."));
      return;
    }
    OdDbSelectionSetIteratorPtr pIter = pFirstSel->newIterator();
    OdDbObjectId objId = pIter->objectId();
    OdDbEntityPtr pLnObj = objId.openObject(OdDb::kForWrite);
    if (!pLnObj.isNull() && pLnObj->isKindOf(OdDbLine::desc()))
    {
      OdDbLinePtr pLine = (OdDbLinePtr)pLnObj;

      OdBrBrep brBrep;
      OdDbSurfacePtr(pSurface)->brep(brBrep);
      OdGeLine3d line3d(pLine->startPoint(), pLine->endPoint());

      getLineContainment(pMs, brBrep, line3d);
    }
  }
}
