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
#include "Spa_region_Command.h"

#include "DbDatabase.h"
#include "RxObject.h"
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
#include "DbRegion.h"

class RegionFilter : public OdStaticRxObject<OdEdSSetTracker>
{
public:
  
  RegionFilter( )
  {}

  int addDrawables(OdGsView* ) { return 0; }
  void removeDrawables(OdGsView* ) { }

  bool check(const OdDbObjectId& /*entId*/)
  {
    return true;
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

void RegionCommand::execute(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbUserIOPtr pIO = pDbCmdCtx->dbUserIO();

  RegionFilter filter;
  OdDbSelectionSetPtr pSet = pIO->select(OdString::kEmpty, OdEd::kSelAllowEmpty, 0, OdString::kEmpty, &filter);
  
  if (pSet->numEntities() == 0)
  {
    pIO->putString(OD_T("None found."));
    return;
  }

  // fill segment array and define database\owner block
  OdRxObjectPtrArray curveSegments, regions;

  OdDbDatabase *pDb = 0;
  OdDbObjectId ownerId;

  OdDbSelectionSetIteratorPtr pItSS = pSet->newIterator();
  while(!pItSS->done())
  {
    OdDbObjectPtr pO = pItSS->objectId().openObject();
    if (!pDb && pO->database())
    {
      pDb = pO->database();
      ownerId = pO->ownerId();
    }
    curveSegments.push_back(pO.get());
    pItSS->next();
  }

  if (!pDb || ownerId.isNull())
  {
    pIO->putString(OD_T("Undefined database or owner."));
    return;
  }

  // create regions
  OdResult res = OdDbRegion::createFromCurves(curveSegments, regions);
  if (res != eOk)
  {
    OdString tmp;
    tmp.format(OD_T("Error : %s"), OdError(res).description().c_str());
    pIO->putString(tmp);
    return;
  }

  if (regions.size() < 1)
  {
    pIO->putString(OD_T("Cannot create region from curves."));
    return;
  }

  // add created regions to database
  OdRxObjectPtrArray::iterator pIt = regions.begin();
  OdRxObjectPtrArray::iterator pEnd = regions.end();
 
  while(pIt != pEnd)
  {
    OdDbRegionPtr pR(*pIt);

    OdDbBlockTableRecordPtr pBlock = ownerId.openObject(OdDb::kForWrite);
    pBlock->appendOdDbEntity( pR );
    ++pIt;
  }

  // delete selected segments
  pItSS = pSet->newIterator();
  while(!pItSS->done())
  {
    OdDbObjectPtr pO = pItSS->objectId().openObject(OdDb::kForWrite);

    pItSS->next();

    pO->erase();
  }

  OdString tmp;
  tmp.format(OD_T("%d Region created."), regions.size());
  pIO->putString(tmp);
}
