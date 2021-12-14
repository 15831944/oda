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
#include "Spa_solidedit_Command.h"

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
#include "DbRegion.h"
#include "DbBody.h"
#include "Db3dSolid.h"

void SolidEditCommand::execute(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbUserIOPtr pIO = pDbCmdCtx->dbUserIO();

  OdUInt32 nKeyword(0);

  nKeyword = pIO->getKeyword(OD_T("[Separeate/OffsetBody/CleanBody/Finish]:"),
    OD_T("Separate OffsetBody CleanBody Finish"));

  pIO->putString(OD_T("Select object :"));
  OdDbSelectionSetPtr pSet = pIO->select(OdString::kEmpty, OdEd::kSelAllowEmpty);
  if (pSet->numEntities() == 0)
  {
    pIO->putString(OD_T("Nothing selected."));
    return;
  }

  OdDb3dSolidPtr pSolid = OdDb3dSolid::cast(pSet->objectIdArray()[0].openObject(OdDb::kForWrite));
  if (pSolid.isNull())
  {
    pIO->putString(OD_T("3dSolid required."));
    return;
  }

  switch(nKeyword)
  {
  case 0: // Separate
    {
      OdDb3dSolidPtrArray newSolids;
      OdResult res = pSolid->separateBody(newSolids);
      if (res != eOk)
      {
        OdString str("Error : ");
        pIO->putString(str + OdError(res).description().c_str());
        return;
      }
        
      if (newSolids.isEmpty())
      {
        pIO->putString(OD_T("The selected solid does not have multiple lumps."));
        return;
      }

      OdDbObjectId ownerId = pSolid->ownerId();
      if (ownerId && ownerId.database())
      {
        OdDb3dSolidPtrArray::iterator pIt = newSolids.begin();
        OdDb3dSolidPtrArray::iterator pItEnd = newSolids.end();
        while(pIt != pItEnd)
        {
          OdDbBlockTableRecordPtr pBlock = ownerId.openObject(OdDb::kForWrite);
          pBlock->appendOdDbEntity( *pIt );
          ++pIt;
        }
      }
      pSolid->erase();

      break;
    }
  case 1: //  OffsetBody
    {
      OdGeExtents3d ext;
      pSolid->getGeomExtents(ext);
      pSolid->offsetBody(ext.minPoint().distanceTo(ext.maxPoint()));
      break;
    }
  case 2: //   CleanBody
    {
      pSolid->cleanBody();
      break;
    }
  default: //    Finish
    {
      return;
    }
  }
}
