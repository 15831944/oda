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

/************************************************************************/
/* ExDgnCloneCommands.cpp: Defines example of deep cloning commands      */
/************************************************************************/
#include "OdaCommon.h"
#include "Ed/EdUserIO.h"
#include "Ed/EdCommandStack.h"
#include "DgCommandContext.h"
#include "DgSSet.h"
#include "DgDatabase.h"
#include "ExDgnCmdDefs.h"
#include "DgIdMapping.h"


#define STL_USING_MAP
#include "OdaSTL.h"

/************************************************************************/
/* Support functions for explode commands.                              */
/************************************************************************/
OdDgElementIdArray selectEntitiesForExplode(OdDgUserIO* pIO)
{
  OdDgElementIdArray entIds;
  OdDgSelectionSetIteratorPtr it = pIO->select(NULL, L"Select entities for explode",OdEd::kSelAllowInactSpaces)->newIterator();
  while (!it->done())
  {
    OdDgElementPtr pEnt = OdDgElement::cast(it->objectId().openObject());
    if (pEnt.get())
    {
      entIds.append(it->objectId());
    }
    else
    {
      pIO->putString(L"1 selected object is not an entity - skipped");
    }
    it->next();
  }
  if (entIds.size()==0)
  {
    pIO->putString(L"Nothing selected");
  }
  return entIds;
}

/************************************************************************/
/* Explode commands.                                                    */
/************************************************************************/

void _dgExplodeGeometry_func(OdEdCommandContext* pCmdCtx)
{
  OdDgCommandContextPtr pDgCmdCtx(pCmdCtx);
  OdDgDatabasePtr pDb = pDgCmdCtx->database();
  OdDgUserIO* pIO = pDgCmdCtx->dbUserIO();

  OdDgElementIdArray entIds = selectEntitiesForExplode(pIO);

  for( OdUInt32 i = 0; i < entIds.size(); i++ )
  {
    OdDgElementId elmId = entIds[i];
    OdDgElementPtr pEnt = elmId.openObject(OdDg::kForWrite);

    if( !pEnt.isNull() )
    {
      OdDgModelPtr pModel = pEnt->ownerId().openObject(OdDg::kForWrite);

      if( !pModel.isNull() )
      {
        OdRxObjectPtrArray arrRes;

        if( pEnt->explodeGeometry(arrRes) == eOk )
        {
          int nSize = arrRes.size();

          if (nSize)
          {
            OdRxObjectPtr* pObj = arrRes.asArrayPtr();

            while (nSize--)
            {
              OdDgElement* pResEnt = (OdDgElement*)((OdRxObject*)(*pObj++));

              pModel->addElement(pResEnt);
            }

          }

          pEnt->erase();
        }
      }
    }
  }
}

void _dgExplode_func(OdEdCommandContext* pCmdCtx)
{
  OdDgCommandContextPtr pDgCmdCtx(pCmdCtx);
  OdDgDatabasePtr pDb = pDgCmdCtx->database();
  OdDgUserIO* pIO = pDgCmdCtx->dbUserIO();

  OdDgElementIdArray entIds = selectEntitiesForExplode(pIO);

  for( OdUInt32 i = 0; i < entIds.size(); i++ )
  {
    OdDgElementId elmId = entIds[i];
    OdDgElementPtr pEnt = elmId.openObject(OdDg::kForWrite);

    if( !pEnt.isNull() )
    {
      OdDgModelPtr pModel = pEnt->ownerId().openObject(OdDg::kForWrite);

      if( !pModel.isNull() )
      {
        OdRxObjectPtrArray arrRes;

        if( pEnt->explode(arrRes) == eOk )
        {
          int nSize = arrRes.size();

          if (nSize)
          {
            OdRxObjectPtr* pObj = arrRes.asArrayPtr();

            while (nSize--)
            {
              OdDgElement* pResEnt = (OdDgElement*)((OdRxObject*)(*pObj++));

              pModel->addElement(pResEnt);
            }

          }

          pEnt->erase();
        }
      }
    }
  }
}
