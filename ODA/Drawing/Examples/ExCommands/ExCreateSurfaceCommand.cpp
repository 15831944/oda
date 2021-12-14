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
#include "ExCreateSurfaceCommand.h"
#include "StaticRxObject.h"
#include "DbUserIO.h"
#include "DbRegion.h"
#include "Db3dSolid.h"
#include "DbCommandContext.h"
#include "DbSurface.h"
#include "DbBlockTableRecord.h"

class SurfaceFilter : public OdStaticRxObject<OdEdSSetTracker>
{
public:

  SurfaceFilter()
  {}

  int addDrawables(OdGsView*) { return 0; }
  void removeDrawables(OdGsView*) { }

  bool check(const OdDbObjectId& entId)
  {
    return true;
  }

  bool append(const OdDbObjectId& entId, const OdDbSelectionMethod*)
  {
    return check(entId);
  }

  bool remove(const OdDbObjectId& entId, const OdDbSelectionMethod*)
  {
    return check(entId);
  }
};

const OdString CreateSurfaceCommand::globalName() const
{
  return OD_T("createSurface");
}

const OdString CreateSurfaceCommand::groupName() const
{
  return OD_T("MODELER GEOMETRY");
}

void CreateSurfaceCommand::execute(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();
  if (pIO.isNull())
    return;

  try
  {
    SurfaceFilter filter;
    OdDbSelectionSetPtr pSet = pIO->select(OD_T("Select objects for surface creation:"), OdEd::kSelAllowSubentsAlways, 0, OdString::kEmpty, &filter);
    if (pSet->numEntities() == 0)
    {
      pIO->putString(OD_T("Nothing selected."));
      return;
    }

    OdDbSelectionSetIteratorPtr pIt = pSet->newIterator();
    unsigned int surfaceCount = 0;
    while (!pIt->done())
    {
      OdDbObjectPtr pObj = pIt->objectId().safeOpenObject(OdDb::kForWrite);

      OdDbEntityPtr pEntity = OdDbEntity::cast(pObj);
      if (pEntity.isNull())
      {
        pIO->putString(OD_T("Unsupported entity."));
        return;
      }

      OdDbSurfacePtr pNewSurface;
      OdResult res = OdDbSurface::createFrom(pEntity, pNewSurface);
      if (res != eOk)
      {
        OdString tmp;
        tmp.format(OD_T("Error : %s"), OdError(res).description().c_str());
        pIO->putString(tmp);
        return;
      }
      surfaceCount++;
      OdDbObjectId ownerId = pEntity->ownerId();
      if (ownerId && ownerId.database())
      {
        OdDbBlockTableRecordPtr pBlock = ownerId.openObject(OdDb::kForWrite);
        pBlock->appendOdDbEntity(pNewSurface);
        pEntity->erase();

        OdString tmp;
        tmp.format(OD_T("%d Surface created"), surfaceCount);
        pIO->putString(tmp);
      }
      pIt->next();
    }
  }
  catch (...)
  {
    pIO->putString(OD_T("Create Surface Failed!"));
  }

}
