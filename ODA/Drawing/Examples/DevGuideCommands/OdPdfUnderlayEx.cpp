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

#include <Ge/GeScale3d.h>
#include "DbUnderlayHost.h"
#include "DbUnderlayDefinition.h"
#include "DbUnderlayReference.h"


/******************************************************************************/
/* ODPDFUNDERLAYEX command demonstates the creation of an PDF Underlay entity */
/******************************************************************************/
void _OdPdfUnderlayEx_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr       pDb = pDbCmdCtx->database();
  OdDbUserIO*           pIO = pDbCmdCtx->dbUserIO();

  pIO->putString(L"\n/////////////START ADDING PDF UNDERLAY/////////////");

  //Create the pointer to current active space
  //OdDbBlockTableRecordPtr bBTR = DevGuideCommandsUtils::activeBlockTableRecord(pDb).safeOpenObject(OdDb::kForWrite);

  OdDbBlockTableRecordPtr bBTR = pDb->getModelSpaceId().safeOpenObject(OdDb::kForWrite);

  if (OdDbPdfDefinition::loadPdfUnderlayModule().isNull())
  {
    pIO->putString(OD_T("PdfUnderlay module is not loaded."));
    return;
  }

  // ask for UnderlayHostPE
  OdDbUnderlayHostPEPtr pHost = OdDbPdfDefinition::desc()->getX(OdDbUnderlayHostPE::desc());

  if (!pHost.get())
  {
    pIO->putString(L"PdfUnderlay Host protocol extension is not loaded.");
    return;
  }

  OdString underlayFilePath = pIO->getFilePath(OD_T("Set the PDF file path for the underlay entity: "));;
  OdDbUnderlayFilePtr pUnderlayFile;

  OdResult res = pHost->load(*pDb, underlayFilePath, "", pUnderlayFile);

  if (res != eOk)
  {
    pIO->putString(L"OdDbUnderlayHostPE cannot load file!");
    return;
  }

  OdDbUnderlayDefinitionPtr pPdfDef = OdDbPdfDefinition::createObject();
  OdString itemName(OD_T("1"));
  pPdfDef->setSourceFileName(underlayFilePath);

  //For a PDF definition this method sets the number of the document's page.
  pPdfDef->setItemName(itemName);

  OdDbObjectId idDef = pPdfDef->postDefinitionToDb(pDb, OD_T("DocSnippets_Cmds_") + itemName);

  OdDbUnderlayReferencePtr pPdfRef = OdDbPdfReference::createObject();
  pPdfRef->setDatabaseDefaults(pDb);
  bBTR->appendOdDbEntity(pPdfRef.get());

  pPdfRef->setDefinitionId(idDef);
  


  pIO->putString(L"\n//////////////END ADDING PDF UNDERLAY//////////////");

  return;
}
