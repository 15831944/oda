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
#include "DbEntity.h"
#include "DbDictionary.h"
#include "DbXrecord.h"
#include "DbIdBuffer.h"
#include "ExCommandsUtils.h"
#include "DbHostAppServices.h"

#define ISOLATED_OBJECTS_XDATA L"AEC_ISOLATEDOBJECTS_XDATA"


OdDbIdBufferPtr getIsolatedIdBuf(OdDbDatabase* pDb, bool bCreate)
{
  OdString strBuf(L"ISOLATED_OBJECTS_IDBUFFER");

  OdDb::OpenMode mode = bCreate ? OdDb::kForWrite : OdDb::kForRead;
  if (bCreate)
  {
    pDb->createExtensionDictionary();
  }
  
  OdDbIdBufferPtr pIdBuf;

  OdDbDictionaryPtr pExDict = pDb->extensionDictionary().openObject(mode);
  if (!pExDict.isNull())
  {
    pIdBuf = pExDict->getAt(strBuf, OdDb::kForWrite);
    if (pIdBuf.isNull() && bCreate)
    {
      pIdBuf = OdDbIdBuffer::createObject();
      pExDict->setAt(strBuf, pIdBuf);
    }
  }
  return pIdBuf;
}

/////////////////////////////////////////////////////////////////////
OdDbXrecordPtr getIsolatedXRecord(OdDbDatabase* pDb, bool bCreate)
{
  OdString strDict(L"AEC_ISOLATED_OBJECTS");
  OdString strRec(L"AEC_ISOLATED_OBJECTS_XRECORD");

  OdDb::OpenMode mode = bCreate ? OdDb::kForWrite : OdDb::kForRead;

  OdDbDictionaryPtr pMainDict = pDb->getNamedObjectsDictionaryId().openObject(mode);
  OdDbDictionaryPtr pRecDict = pMainDict->getAt(strDict, mode);
  OdDbXrecordPtr pXRec;
  if (pRecDict.isNull())
  {
    if (bCreate)
    {
      pRecDict = OdDbDictionary::createObject();
      pMainDict->setAt(strDict, pRecDict);
    }
    else
    {
      return pXRec;
    }
  }
  else
  {
    pXRec = pRecDict->getAt(strRec, OdDb::kForWrite);
  }
  if (pXRec.isNull() && bCreate)
  {
    pXRec = OdDbXrecord::createObject();
    pRecDict->setAt(strRec, pXRec);
  }
  return pXRec;
}

/////////////////////////////////////////////////////////////////////
void hideObjectsPerSession(OdDbDatabase* pDb, OdDbSelectionSetIterator* pIter)
{
  // IdBuffer to store hidden entities Ids
  OdDbIdBufferPtr pIdBuf = getIsolatedIdBuf(pDb, true);

  while (!pIter->done())
  {
    OdDbObjectId objId  = pIter->objectId();
    OdDbEntityPtr pEnt  = OdDbEntity::cast(objId.openObject(OdDb::kForWrite));

    if (!pEnt.isNull() && pEnt->visibility() == OdDb::kVisible)
    {
      pEnt->setTempVisibility(OdDb::kInvisible);
      pIdBuf->addId(objId);
    }
    pIter->next();
  }
}

/////////////////////////////////////////////////////////////////////
void hideObjectsPersistently(OdDbDatabase* pDb, OdDbSelectionSetIterator* pIter)
{
  // XData to append to entity
  OdString strRegApp(ISOLATED_OBJECTS_XDATA);
  pDb->newRegApp(strRegApp);  // Be sure it exists
  OdResBufPtr pXd = OdResBuf::newRb(1001, strRegApp);
  pXd->insert(OdResBuf::newRb(1070,(OdInt16)1));
  pXd->insert(OdResBuf::newRb(1070,(OdInt16)100));

  // XRecord to store hidden entities Ids
  OdDbXrecordPtr pXRec = getIsolatedXRecord(pDb, true);
  OdResBufPtr pEntList = OdResBuf::newRb(); // Dummy first entry

  while (!pIter->done())
  {
    OdDbObjectId objId  = pIter->objectId();
    OdDbEntityPtr pEnt  = OdDbEntity::cast(objId.openObject(OdDb::kForWrite));

    if (!pEnt.isNull() && pEnt->visibility() == OdDb::kVisible)
    {
      pEnt->setVisibility(OdDb::kInvisible);
      pEnt->setXData(pXd);
      pEntList->insert(OdResBuf::newRb(340, pEnt->objectId()));
    }
    pIter->next();
  }
  OdDbXrecordIteratorPtr pRecIt = pXRec->newIterator();
  if (!pRecIt->done() && pRecIt->curRestype() == 340)
  { // Hidden enities already present
    pXRec->appendRbChain(pEntList->next());
  }
  else
  {
    // In case all entities are unisolated XRecord contains ResBuf (70, -1)
    pXRec->setFromRbChain(pEntList->next());
  }
}

void hideObjects(OdDbDatabase* pDb, OdDbSelectionSetIterator* pIter)
{
  try
  {
    pDb->startTransaction();
    OdDbHostAppServices * pHostApp = pDb->appServices();

    if (pHostApp->getOBJECTISOLATIONMODE())
    {
      hideObjectsPersistently(pDb, pIter);
    }
    else
    {
      hideObjectsPerSession(pDb, pIter);
    }

    pDb->endTransaction();
  }
  catch (const OdError&)
  {
    pDb->abortTransaction();
    throw;
  }
}




void _HideObjects_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();
  OdDbSelectionSetPtr pSSet = pIO->select(L"Select objects:", 
                                          OdEd::kSelAllowObjects);

  if (pSSet->numEntities())
  {
    OdDbSelectionSetIteratorPtr pIter = pSSet->newIterator();
    hideObjects(pDbCmdCtx->database(), pIter);
    pIO->putString(L"Done");
  }
  else
  {
    pIO->putString(L"Nothing to hide");
  }
}


void _IsolateObjects_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();
  OdDbSelectionSetPtr pSSet = pIO->select(L"Select objects:", 
                                          OdEd::kSelAllowObjects);

  if (pSSet->numEntities())
  {
    OdStaticRxObject<OdDbInvertedSelectionSetIterator> Iter;
    Iter.init(pSSet);
    OdDbSelectionSetIterator* pIter = (OdDbSelectionSetIterator*)&Iter;
    hideObjects(pDbCmdCtx->database(), pIter);
    pIO->putString(L"Done");
  }
  else
  {
    pIO->putString(L"Nothing to isolate");
  }
}


void _UnIsolateObjects_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();

  OdDbDatabasePtr pDb = pDbCmdCtx->database();

  int nUnisolated = 0;
  pDb->startTransaction();
  try
  {
    // XRecord storing persistently hidden entities Ids
    OdDbXrecordPtr pXRec = getIsolatedXRecord(pDb, false);
    if (!pXRec.isNull())
    {
      // XData to be removed
      OdResBufPtr pXd = OdResBuf::newRb(1001, ISOLATED_OBJECTS_XDATA);
      OdDbXrecordIteratorPtr pIter = pXRec->newIterator();
      OdResBufPtr pCur;
      while (!pIter->done())
      {
        pCur = pIter->getCurResbuf();
        if (pCur->restype() == 340)
        {
          OdDbEntityPtr pEnt = OdDbEntity::cast(pCur->getObjectId(pDb).openObject(OdDb::kForWrite));
          if (!pEnt.isNull())
          {
            pEnt->setVisibility(OdDb::kVisible);
            pEnt->setXData(pXd);
            ++nUnisolated;
          }
        }
        pIter->next();
      }
      // Clear XRecord
      pXRec->setFromRbChain(OdResBuf::newRb(70, (OdInt16)-1));
    }

    // IdBuffer storing temporary hidden entities Ids
    OdDbIdBufferPtr pIdBuf = getIsolatedIdBuf(pDb, false);
    if (!pIdBuf.isNull())
    {
      OdDbIdBufferIteratorPtr pIter = pIdBuf->newIterator();
      while (!pIter->done())
      {
        OdDbEntityPtr pEnt = OdDbEntity::cast(pIter->id().openObject(OdDb::kForWrite));
        if (!pEnt.isNull())
        {
          pEnt->setTempVisibility(OdDb::kVisible);
          ++nUnisolated;
        }
        pIter->next();
      }
      // Clear IdBuf
      pIdBuf->removeAll();
    }

    pDb->endTransaction();
  }
  catch (const OdError&)
  {
    pDb->abortTransaction();
    throw;
  }

  OdString strMessage;
  if (!nUnisolated)
  {
    strMessage = L"No objects to unisolate.";
  }
  else
  {
    strMessage.format(L"%d object(s) unisolated.", nUnisolated);
  }
  pIO->putString(strMessage);
}
