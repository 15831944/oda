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
#include "Spa_imprint_Command.h"
#include "RxDynamicModule.h"
#include "Ed/EdUserIO.h"
#include "DbCommandContext.h"
#include "StaticRxObject.h"

#include "DbSurface.h"
#include "DbRegion.h"
#include "Db3dSolid.h"
#include "OdErrorContext.h"
#include "DbBlockTableRecord.h"
#include "DbCurve.h"

class ImprintObjectFilter : public OdStaticRxObject<OdEdSSetTracker>
{
public:

  ImprintObjectFilter( )
  {}

  int addDrawables(OdGsView* ) { return 0; }
  void removeDrawables(OdGsView* ) { }

  bool check(const OdDbObjectId& entId)
  {
    OdDbEntityPtr pEnt = entId.openObject();
    return pEnt->isKindOf(OdDb3dSolid::desc());
  }

  bool append(const OdDbObjectId& entId, const OdDbSelectionMethod* )
  {
    return check(entId);
  }

  bool remove(const OdDbObjectId& entId, const OdDbSelectionMethod* )
  {
    return check(entId);
  }
};

class ImprintEntityFilter : public OdStaticRxObject<OdEdSSetTracker>
{
public:

  ImprintEntityFilter( )
  {}

  int addDrawables(OdGsView* ) { return 0; }
  void removeDrawables(OdGsView* ) { }

  bool check(const OdDbObjectId& entId)
  {
    OdDbEntityPtr pEnt = entId.openObject();
    return pEnt->isKindOf(OdDb3dSolid::desc());
  }

  bool append(const OdDbObjectId& entId, const OdDbSelectionMethod* )
  {
    return check(entId);
  }

  bool remove(const OdDbObjectId& entId, const OdDbSelectionMethod* )
  {
    return check(entId);
  }
};


void ImprintCommand::execute(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();
  if (pIO.isNull())
    return;

  ImprintObjectFilter filter;
  OdDbSelectionSetPtr pSet = pIO->select(OD_T("Select a 3D solid or surface:"), OdEd::kSelSingleEntity/*, 0, OdString::kEmpty, &filter*/);

  if (pSet->numEntities() == 0)
  {
    pIO->putString(OD_T("Nothing selected."));
    return;
  }

  OdDbSelectionSetIteratorPtr pIter = pSet->newIterator();
  OdDbObjectPtr pImpEnt = pIter->objectId().openObject(OdDb::kForWrite);
  if( !pImpEnt.isNull() && (pImpEnt->isKindOf(OdDb3dSolid::desc()) || pImpEnt->isKindOf(OdDbSurface::desc())))
  {
    ImprintEntityFilter filter2;
    OdDbSelectionSetPtr pEntitySet = pIO->select(OD_T("Select an object to imprint:"), OdEd::kSelSingleEntity/*, 0, OdString::kEmpty, &filter2*/);

    if (pEntitySet->numEntities() == 0)
    {
      pIO->putString(OD_T("Nothing selected."));
      return;
    }

    bool bDelete = pIO->getKeyword(OD_T("Delete the source object <Y> : "), OD_T("Y N"), 0, OdEd::kInpDefault) == 0;

    pIter = pEntitySet->newIterator();
    OdDbEntityPtr pEntity( pIter->objectId().openObject(OdDb::kForWrite) );
    OdResult res = eOk;
    if (pImpEnt->isKindOf(OdDb3dSolid::desc()))
    {
      res = OdDb3dSolidPtr(pImpEnt)->imprintEntity(pEntity);
    }
    if (pImpEnt->isKindOf(OdDbSurface::desc()))
    {
      res = OdDbSurfacePtr(pImpEnt)->imprintEntity(pEntity);
    }      
      
    if (res != eOk)
    {
      OdString tmp;
      tmp.format(OD_T("Error : %s"), OdError(res).description().c_str());
      pIO->putString(tmp);
      return;
    }

    if (bDelete)
      pEntity->erase();
  }
}
