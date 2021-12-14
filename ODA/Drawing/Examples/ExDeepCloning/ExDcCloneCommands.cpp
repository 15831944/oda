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
/* ExDcCloneCommands.cpp: Defines example of deep cloning commands      */
/************************************************************************/
#include "OdaCommon.h"
#include "Ed/EdUserIO.h"
#include "Ed/EdCommandStack.h"
#include "DbCommandContext.h"
#include "DbSSet.h"
#include "DbDatabase.h"
#include "ExDcCmdDefs.h"
#include "DbBlockTable.h"
#include "DbBlockTableRecord.h"
#include "DbEntity.h"
#include "DbDictionary.h"
#include "DbIdMapping.h"
#include "DbHostAppServices.h"
#include "Ge/GeScale3d.h"
#include "StaticRxObject.h"
#include "Reactors/DbgEventReactor.h"
#include "Reactors/DbgDatabaseReactor.h"
#include "Reactors/DbgObjectReactor.h"
#include "Reactors/DbgReactorOut.h"


#define STL_USING_MAP
#include "OdaSTL.h"

/************************************************************************/
/* Support functions for deep cloning commands.                         */
/************************************************************************/
OdDbObjectIdArray selectEntities(OdDbUserIO* pIO)
{
  OdDbObjectIdArray entIds;
  OdDbSelectionSetIteratorPtr it = pIO->select(L"Select entities",OdEd::kSelAllowInactSpaces)->newIterator();
  while (!it->done())
  {
    OdDbEntityPtr pEnt = OdDbEntity::cast(it->objectId().openObject());
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

OdDbObjectIdArray selectBlocks(OdDbUserIO* pIO)
{
  OdDbObjectIdArray recIds;
  OdDbSelectionSetIteratorPtr it = pIO->select(L"Select blocks",OdEd::kSelAllowInactSpaces|OdEd::kSelAllowObjects)->newIterator();
  while (!it->done())
  {
    OdDbObjectPtr pObj = it->objectId().safeOpenObject();
    if (pObj->isKindOf(OdDbBlockTableRecord::desc()))
    {
      recIds.push_back(pObj->objectId());
    }
    else
    {
      pIO->putString(L"1 selected object is not a block - skipped");
    }
    it->next();
  }

  if (!recIds.size())
  {
    pIO->putString(L"Nothing selected");
  }
  return recIds;
}

OdDb::DuplicateRecordCloning selectDrcType(OdDbUserIO* pIO)
{
  OdUInt32 nKeyword = pIO->getKeyword(
    OD_T("Enter type of DuplicateRecordCloning [Ignore/Replace/MangleName] <\"Ignore\">:")
    , OD_T("Ignore Replace MangleName")
    , 0
    );
  OdDb::DuplicateRecordCloning drcType = OdDb::kDrcIgnore;
  switch(nKeyword)
  {
  case 1: drcType = OdDb::kDrcReplace; break;
  case 2: drcType = OdDb::kDrcMangleName; break;
  };
  return drcType;
}

OdDbDatabasePtr selectDestDatabase(OdDbUserIO* pIO, OdDbDatabase* pSrcDb)
{
  // Select or create a destination database
  OdDbDatabasePtr pDestDb;
  try
  {
    pDestDb = pSrcDb->appServices()->readFile(
      pIO->getFilePath(L"Enter file name:", OdEd::kGfpForOpen, L"Open destination file", L"dwg", L"", "DWG files (*.dwg)|*.dwg")
      );
  }
  catch (OdError&)
  {
    pIO->putString(L"Create a new destination database");
    pDestDb = pSrcDb->appServices()->createDatabase(true, pSrcDb->getMEASUREMENT());
  }
  return pDestDb;
}

void saveClonedDatabase(OdDbUserIO* pIO, OdDbDatabase* pDestDb)
{
  if (!pDestDb)
    return;

  OdStreamBufPtr pFile = ::odSystemServices()->createFile(
    pIO->getFilePath( OD_T("Enter file name:"), OdEd::kGfpForSave, OD_T("Save cloned database as"), OD_T("dwg")),
    Oda::kFileWrite,
    Oda::kShareDenyReadWrite,
    Oda::kCreateAlways);
  pDestDb->writeFile(pFile, OdDb::kDwg, OdDb::kDHL_CURRENT);
}

/************************************************************************/
/* Deep cloning commands.                                               */
/************************************************************************/
void _EXDC_DEEPCLONE_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();

  // Select entities to clone
  OdDbObjectIdArray entIds = selectEntities(pIO);
  if (!entIds.size())
    return;

  // Select owner record for cloned entities
  OdString recName = pIO->getString(OD_T("Enter name of Block to clone entities <\"*Model_Space\">:"), OdEd::kGstNoEmpty, OD_T("*Model_Space"));
  OdDbObjectId destRecId = OdDbBlockTablePtr(pDb->getBlockTableId().safeOpenObject())->getAt(recName);
  if (destRecId.isErased())
  {
    destRecId = pDb->getModelSpaceId();
  }

  // Clone
  OdDbIdMappingPtr pMapping = OdDbIdMapping::createObject();
  pMapping->setDestDb(pDb);
  pDb->deepCloneObjects(entIds, destRecId, *pMapping);
}

void _EXDC_DEEPCLONE_MULTIOWNERS_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();

  // Select entities to clone and assign appropriate destination blocks
  std::map<OdDbObjectId, OdDbObjectIdArray> clonedEnts;
  OdDbSelectionSetIteratorPtr it = pIO->select(L"Select entities",OdEd::kSelAllowInactSpaces)->newIterator();
  while (!it->done())
  {
    OdDbEntityPtr pEnt = OdDbEntity::cast(it->objectId().openObject());
    if (pEnt.get())
    {
      if (OdDbBlockTableRecord::cast(pEnt->ownerId().openObject()).get())
      {
        clonedEnts[pEnt->ownerId()].push_back(pEnt->objectId());
      }
      else
      {
        pIO->putString(L"1 selected entity is not owned by block - skipped");
      }
    }
    else
    {
      pIO->putString(L"1 selected object is not an entity - skipped");
    }
    it->next();
  }

  size_t mapSize = clonedEnts.size();
  if (!mapSize)
  {
    pIO->putString(L"Nothing selected");
    return;
  }

  // Clone
  OdDbIdMappingPtr pMapping = OdDbIdMapping::createObject();
  pMapping->setDestDb(pDb);
  std::map<OdDbObjectId, OdDbObjectIdArray>::iterator mapIt = clonedEnts.begin();
  for ( ; mapIt != clonedEnts.end(); ++mapIt )
  {
    pDb->deepCloneObjects(mapIt->second, mapIt->first, *pMapping, (--mapSize != 0));
  }
}

void _EXDC_DEEPCLONE_REACTORS_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();

  // Select entities to clone
  OdDbObjectIdArray entIds = selectEntities(pIO);
  if (!entIds.size())
    return;

  // Select owner record for cloned entities
  OdString recName = pIO->getString(OD_T("Enter name of Block to clone entities <\"*ModelSpace\">:"), OdEd::kGstNoEmpty, OD_T("*ModelSpace"));
  OdDbObjectId destRecId = OdDbBlockTablePtr(pDb->getBlockTableId().safeOpenObject())->getAt(recName);
  if (destRecId.isErased())
  {
    destRecId = pDb->getModelSpaceId();
  }

  // Set RxEvent reactor
  OdStaticRxObject<OdDbgReactorIOMsg<OdDbgEditorReactor> > editorReactor;
  editorReactor.init(pIO);
  odedEditor()->addReactor(&editorReactor);

  // Set database reactor
  OdStaticRxObject<OdDbgReactorIOMsg<OdDbgDatabaseReactor> > dbReactor;
  dbReactor.init(pIO);
  pDb->addReactor(&dbReactor);

  // Set transient reactor
  OdStaticRxObject<OdDbgReactorIOMsg<OdDbgObjectReactor> > objReactor;
  objReactor.init(pIO);
  unsigned i = 0;
  for (; i < entIds.size(); ++i)
  {
    entIds[i].safeOpenObject()->addReactor(&objReactor);
  }

  // Clone
  OdDbIdMappingPtr pMapping = OdDbIdMapping::createObject();
  pMapping->setDestDb(pDb);
  pDb->deepCloneObjects(entIds, destRecId, *pMapping);

  //try
  //{
  //  _EXDC_DEEPCLONE_func(pCmdCtx);
  //}
  //catch (...)
  //{
  //  odedEditor()->removeReactor(&editorReactor);
  //  throw;
  //}

  // Remove reactors
  odedEditor()->removeReactor(&editorReactor);
  pDb->removeReactor(&dbReactor);
  for (i = 0; i < entIds.size(); ++i)
  {
    entIds[i].safeOpenObject()->removeReactor(&objReactor);
  }
}

void _EXDC_WBLOCKCLONE_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();

  // Select entities to clone
  OdDbObjectIdArray entIds = selectEntities(pIO);
  if (!entIds.size())
    return;

  // Select type of DuplicateRecordCloning
  OdDb::DuplicateRecordCloning drcType = selectDrcType(pIO);

  // Select or create a destination database
  OdDbDatabasePtr pDestDb = selectDestDatabase(pIO, pDb);

  // Clone
  OdDbIdMappingPtr pMapping = OdDbIdMapping::createObject();
  pMapping->setDestDb(pDestDb);
  pDb->wblockCloneObjects(entIds, pDestDb->getModelSpaceId(), *pMapping, drcType);

  // Write database
  saveClonedDatabase(pIO, pDestDb);
}

void _EXDC_WBLOCKCLONE_BLOCKS_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();

  // Select blocks to clone and assign appropriate destination owners
  OdDbObjectIdArray objIds = selectBlocks(pIO);
  if (!objIds.size())
    return;

  // Select type of DuplicateRecordCloning
  OdDb::DuplicateRecordCloning drcType = selectDrcType(pIO);

  // Select or create a destination database
  OdDbDatabasePtr pDestDb = selectDestDatabase(pIO, pDb);

  // Clone
  OdDbIdMappingPtr pMapping = OdDbIdMapping::createObject();
  pMapping->setDestDb(pDestDb);
  pDb->wblockCloneObjects(objIds, pDestDb->getBlockTableId(), *pMapping, drcType);

  // Write database
  saveClonedDatabase(pIO, pDestDb);
}

void _EXDC_WBLOCK_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();

  OdUInt32 nKeyword = pIO->getKeyword(
      OD_T("Enter WBLOCK type [Database/Block/Entities] <Database>:")
    , OD_T("Database Block Entities")
    , 0
    );
  OdDbDatabasePtr pDestDb;
  switch (nKeyword)
  {
  case 0: //Database
    {
      pDestDb = pDb->wblock();
    }
    break;
  case 1: //Block
    {
      OdDbSelectionSetIteratorPtr it = pIO->select(L"Select a block",OdEd::kSelAllowInactSpaces|OdEd::kSelAllowObjects)->newIterator();
      if (!it->done())
      {
        if (!OdDbBlockTableRecord::cast(it->objectId().openObject()).isNull())
        {
          pDestDb = pDb->wblock(it->objectId());
        }
      }
    }
    break;
  case 2: //Entities
    {
      OdDbObjectIdArray entIds = selectEntities(pIO);
      if (entIds.size())
      {
        OdGePoint3d pt = OdGePoint3d::kOrigin;
        pIO->getPoint(OD_T("Enter base point <Origin>:"), 0, &pt);
        pDestDb = pDb->wblock(entIds, pt);
      }
    }
    break;
  }

  // Write database
  saveClonedDatabase(pIO, pDestDb);
}

void _EXDC_INSERT_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();

  OdString fname = pIO->getFilePath(OD_T("Enter file name to insert:")
    , OdEd::kGfpForOpen
    , OD_T("Select drawing to insert")
    , OD_T("dwg")
    , OdString::kEmpty
    , OD_T("TD drawing (*.dwg)|*.dwg||")
    );
  OdDbDatabasePtr pSrcDb = pDb->appServices()->readFile(fname);
  if (pSrcDb.get())
  {
    bool bPreserveSrcDb = (0 == pIO->getKeyword(OD_T("Preserve source database [Yes/No] <Yes>:"), OD_T("Yes No"), 0 ));
    OdString dstRecName = pIO->getString(OD_T("Enter name of destination block <\"\">:"));
    if (!dstRecName.isEmpty())
    {
      OdString srcRecName = pIO->getString(OD_T("Enter name of source block <\"\">:"));
      if (!srcRecName.isEmpty())
      {
        pDb->insert(srcRecName, dstRecName, pSrcDb, bPreserveSrcDb);
      }
      else
      {
        pDb->insert(dstRecName, pSrcDb, bPreserveSrcDb);
      }
    }
    else
    {
      double scaleX = pIO->getReal(L"Specify X scale factor<1.0>", OdEd::kInpDefault, 1.0);
      double scaleY = pIO->getReal(L"Specify Y scale factor<1.0>", OdEd::kInpDefault, 1.0);
      double scaleZ = pIO->getReal(L"Specify Z scale factor<1.0>", OdEd::kInpDefault, 1.0);
      pDb->insert(OdGeScale3d(scaleX, scaleY, scaleZ), pSrcDb, bPreserveSrcDb);
      //pDb->insert(OdGeMatrix3d::kIdentity, pSrcDb, bPreserveSrcDb);
    }
  }
}
