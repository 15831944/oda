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
#include "DbAssocNetwork.h"
#include "DbFiler.h"
#include "DbDictionary.h"
#include "DbImpAssocNetwork.h"
#include "DbBlockTableRecord.h"

ODRX_DEFINE_MEMBERS_EX(OdDbAssocNetwork,     // ClassName
                       OdDbAssocAction,      // ParentClass
                       DBOBJECT_CONSTR,      // DOCREATE
                       OdDb::kDHL_1021,      // DwgVer
                       OdDb::kMRelease6,     // MaintVer
                       1025,                 // nProxyFlags (kEraseAllowed | kDisableProxyWarning)
                       L"AcDbAssocNetwork",  // DWG class name
                       L"ACDBASSOCNETWORK",  // DxfName
                       L"ObjectDBX Classes", // AppName
                       OdRx::kMTLoading | OdRx::kHistoryAware);

OdDbAssocNetwork::OdDbAssocNetwork(): OdDbAssocAction(false)
{
  m_pImpObj = new OdDbImpAssocNetwork();
}

OdDbObjectIdArray OdDbAssocNetwork::getActions() const
{
  assertReadEnabled();
  return ((OdDbImpAssocNetwork*)m_pImpObj)->getActions();
}

OdResult OdDbAssocNetwork::addAction( const OdDbObjectId& actionId, bool alsoSetAsDatabaseOwner )
{
  assertWriteEnabled();
  return ((OdDbImpAssocNetwork*)m_pImpObj)->addAction(this, actionId, alsoSetAsDatabaseOwner);
}
OdResult OdDbAssocNetwork::removeAction(const OdDbObjectId& actionId, bool alsoEraseIt)
{
  assertWriteEnabled();
  return ((OdDbImpAssocNetwork*)m_pImpObj)->removeAction(this, actionId, alsoEraseIt);
}

OdDbObjectIdArray OdDbAssocNetwork::getActionsToEvaluate() const {
  assertReadEnabled();
  return ((OdDbImpAssocNetwork*)m_pImpObj)->getActionsToEvaluate();
}

#if 0
    OdResult addActions(const OdDbObjectIdArray& actionIds, bool alsoSetAsDatabaseOwner);

    OdResult removeAllActions(bool alsoEraseThem);
#endif

OdResult OdDbAssocNetwork::ownedActionStatusChanged(OdDbAssocAction* pOwnedAction, OdDbAssocStatus previousStatus) {
  assertReadEnabled(); // assertWriteEnabled() is called inside ownedActionStatusChanged() if necessary
  return ((OdDbImpAssocNetwork*)m_pImpObj)->ownedActionStatusChanged(this, pOwnedAction, previousStatus);
}

void OdDbAssocNetwork::composeForLoad(OdDb::SaveType format, 
                                      OdDb::DwgVersion version, 
                                      OdDbAuditInfo* pAuditInfo)
{
  OdDbAssocAction::composeForLoad(format, version, pAuditInfo);

  ((OdDbImpAssocNetwork*)m_pImpObj)->composeForLoad(this, format, version, pAuditInfo);
}

OdDbObjectPtr OdDbAssocNetwork::decomposeForSave(OdDb::DwgVersion ver,
                                                 OdDbObjectId& replaceId, 
                                                 bool& exchangeXData)
{
  OdDbObjectPtr pObj = OdDbAssocAction::decomposeForSave(ver, replaceId, exchangeXData);
  OdDbObjectIdArray ids = getActions();
  for (OdUInt32 index = 0; index < ids.size(); index++)
  {
    OdDbObjectPtr pChild = ids[index].openObject();
    if (pChild.isNull())
      continue;

    if (pChild->decomposeForSave(ver, replaceId, exchangeXData).get())
    {	
      ODA_FAIL_ONCE();
	  }
  }
  return pObj;
}

OdDbObjectId OdDbAssocNetwork::getInstanceFromDatabase(OdDbDatabase*   pDatabase,
                                                       bool            createIfDoesNotExist,
                                                       const OdString& dictionaryKey)
{
  if (!pDatabase)
    return OdDbObjectId::kNull;
  OdString dictName = dictionaryKey.isEmpty() ? OdString(OD_T("ACAD_ASSOCNETWORK")) : dictionaryKey;

  OdDbObjectId dictId = pDatabase->getNamedObjectsDictionaryId();
  OdDbObjectId networkDict;

  OdDbDictionaryPtr dict = dictId.safeOpenObject(OdDb::kForWrite);
  if ( !dict.isNull() )
    networkDict = dict->getAt(dictName);
  else
    return OdDbObjectId::kNull;

  bool createDict = false;

  OdDbDictionaryPtr pNetworkDict;
  OdDbObjectId res;

  if ( !networkDict.isNull() )
  {
    pNetworkDict = networkDict.openObject(OdDb::kForWrite);
    res = pNetworkDict->getAt(dictName);

    if ( !res.isNull() )
      return res;

  }
  else
  {
    createDict = true;
  }

  if ( createIfDoesNotExist )
  {
    if ( createDict )
    {
      pNetworkDict = OdDbDictionary::createObject();
      networkDict = dict->setAt(dictName, pNetworkDict);
    }
    
    OdDbAssocNetworkPtr pNetwork = OdDbAssocNetwork::createObject();
    res = pNetworkDict->setAt(dictName, pNetwork);
    
    return res;
  }

  return OdDbObjectId::kNull;

}
OdDbObjectId OdDbAssocNetwork::getInstanceFromObject(const OdDbObjectId& owningObjectId,
                                          bool                createIfDoesNotExist,
                                          bool                addToTopLevelNetwork,
                                          const OdString&     dictionaryKey)
{
  OdString dictName = dictionaryKey.isEmpty() ? OdString(OD_T("ACAD_ASSOCNETWORK")) : dictionaryKey;

  OdDbObjectId res = OdDbObjectId::kNull;

  OdDbObjectPtr pOwningObject = owningObjectId.openObject();
  if (pOwningObject.isNull())
    return res;

  OdDbObjectId extDictId = pOwningObject->extensionDictionary();

  OdDbDictionaryPtr pDict;

  OdDbDictionaryPtr pNetworkDict;

  bool createExtDict = false;
  bool createDict = false;
  bool clearTopLevelNetwork = false;

  if (extDictId.isValid())
  {
    pDict = extDictId.safeOpenObject(OdDb::kForWrite);
    OdDbObjectId networkDict = pDict->getAt(dictName);

    if (networkDict.isValid())
    {
      pNetworkDict = networkDict.safeOpenObject(OdDb::kForWrite);
      res = pNetworkDict->getAt(dictName);

      if ( !res.isNull() )
      {
        OdDbObjectPtr network = res.openObject(OdDb::kForWrite);
        if (OdDbAssocNetwork::cast(network).isNull()) { // it actually might be proxy
          if (!createIfDoesNotExist)
            return OdDbObjectId::kNull;
          network->erase();
          res.setNull();
          clearTopLevelNetwork = true;
          createIfDoesNotExist = false;
        }
        else
          return res;
      }
    }
    else
    {
      createDict = true;
    }
  }
  else
  {
    createExtDict = true;
    createDict = true;
  }

  if ( createIfDoesNotExist )
  {
    if ( createExtDict )
    {
      pOwningObject->upgradeOpen();
      pOwningObject->createExtensionDictionary();
      extDictId = pOwningObject->extensionDictionary();
      pDict = extDictId.safeOpenObject(OdDb::kForWrite);
    }

    if ( createDict )
    {
      pNetworkDict = OdDbDictionary::createObject();
      pDict->setAt(dictName, pNetworkDict);
    }

    OdDbAssocNetworkPtr pNetwork = OdDbAssocNetwork::createObject();
    res = pNetworkDict->setAt(dictName, pNetwork);
  }

  if ( (addToTopLevelNetwork && createIfDoesNotExist) || clearTopLevelNetwork )
  {
    OdDbObjectId topLevelNetId = getInstanceFromDatabase(pOwningObject->database(), true, dictionaryKey);
    OdDbAssocNetworkPtr pTopLevelNet = topLevelNetId.safeOpenObject(OdDb::kForWrite);
    if (clearTopLevelNetwork)
      pTopLevelNet->removeAction(res, false);
    else
      pTopLevelNet->addAction(res, false);
  }

  return res;
}

OdResult OdDbAssocNetwork::removeInstanceFromObject(
  const OdDbObjectId& owningObjectId,
  bool                alsoEraseIt,
  const OdString&     dictionaryKey)
{
  OdString dictName = dictionaryKey.isEmpty() ? OdString(OD_T("ACAD_ASSOCNETWORK")) : dictionaryKey;

  OdDbObjectPtr pOwningObject = owningObjectId.safeOpenObject();
  OdDbObjectId extDictId = pOwningObject->extensionDictionary();

  // Get network id to erase network
  OdDbObjectId networkId = getInstanceFromObject(owningObjectId, false, false, dictionaryKey);

  OdDbDictionaryPtr pDict = OdDbDictionary::cast(extDictId.openObject(OdDb::kForWrite));
  if (!pDict.isNull())
  {
    OdDbDictionaryPtr pNetworkDict = pDict->getAt(dictName, OdDb::kForWrite);
    pDict->remove(dictName);
    if (!pNetworkDict.isNull())
    {
      // Under this name there is network itself
      pNetworkDict->remove(dictName);
      pNetworkDict->erase();
      pNetworkDict = 0;
    }
    pDict = 0;
  }

  // will be released if empty
  pOwningObject->releaseExtensionDictionary();
  pOwningObject = 0;

  if (alsoEraseIt)
  {
    OdDbAssocNetworkPtr pNetwork = networkId.openObject(OdDb::kForWrite);
    if (!pNetwork.isNull())
    {
      OdDbAssocNetworkPtr pOwningNetwork = OdDbAssocNetwork::cast(pNetwork->owningNetwork().openObject(OdDb::kForWrite));
      if (pOwningNetwork.isNull())
      {
        pNetwork->erase();
        pNetwork = 0;
      }
      else
      {
        pNetwork = 0;
        pOwningNetwork->removeAction(networkId, true);
      }
    }
  }

  return eOk;
}

OdResult OdDbAssocNetwork::removeInstanceFromDatabase(
  OdDbDatabase*   pDatabase,
  bool            alsoEraseIt,
  const OdString& dictionaryKey)
{
  OdString dictName = dictionaryKey.isEmpty() ? OdString(OD_T("ACAD_ASSOCNETWORK")) : dictionaryKey;

  OdDbObjectId dictId = pDatabase->getNamedObjectsDictionaryId();
  OdDbDictionaryPtr dict = dictId.safeOpenObject();

  // Get network id to erase network
  OdDbObjectId networkId = getInstanceFromDatabase(pDatabase, false, dictionaryKey);

  OdDbDictionaryPtr pDict = OdDbDictionary::cast(dictId.openObject(OdDb::kForWrite));
  if (!pDict.isNull())
  {
    OdDbDictionaryPtr pNetworkDict = pDict->getAt(dictName, OdDb::kForWrite);
    pDict->remove(dictName);
    if (!pNetworkDict.isNull())
    {
      // Under this name there is network itself
      pNetworkDict->remove(dictName);
      pNetworkDict->erase();
      pNetworkDict = 0;
    }   
    pDict = 0;
  }

  if (alsoEraseIt)
  {
    OdDbAssocNetworkPtr pNetwork = networkId.openObject(OdDb::kForWrite);
    if (!pNetwork.isNull())
      pNetwork->erase();
    pNetwork = 0; 
  }

  return eOk;
}

void registerInGlobalNetwork(OdDbDatabase* db, OdDbObjectId network) {
  OdDbAssocNetworkPtr globalDestNet =
    OdDbAssocNetwork::getInstanceFromDatabase(db, true).safeOpenObject(OdDb::kForWrite);
  globalDestNet->addAction(network, false);
}

OdDbObjectPtr OdDbAssocNetwork::subDeepClone(OdDbIdMapping& idMap, OdDbObject* owner, bool primary) const {
  OdDbObjectPtr clone = OdDbAssocAction::subDeepClone(idMap, owner, primary);
  if (clone.get() && !primary)
    ::registerInGlobalNetwork(idMap.destDb(), clone->objectId());
  return clone;
}

OdDbObjectPtr OdDbAssocNetwork::subWblockClone(OdDbIdMapping& idMap, OdDbObject* owner, bool primary) const {
  OdDbObjectPtr clone = OdDbAssocAction::subWblockClone(idMap, owner, primary);
  if (clone.get() && !primary)
    ::registerInGlobalNetwork(idMap.destDb(), clone->objectId());
  return clone;
}

OdResult OdDbAssocNetwork::subErase(bool erasing) {
  OdResult res = OdDbAssocAction::subErase(erasing);
  if (erasing && res == eOk) {
    OdDbDictionaryPtr owningDic = OdDbDictionary::cast(ownerId().openObject(OdDb::kForWrite));
    if (owningDic.get()) {
      OdDbDictionaryPtr owning2Dic = OdDbDictionary::cast(owningDic->ownerId().openObject(OdDb::kForWrite));
      if (owning2Dic.get()) {
        owning2Dic->remove(owningDic->objectId());
        owningDic->erase();
        OdDbBlockTableRecordPtr block = OdDbBlockTableRecord::cast(owning2Dic->ownerId().openObject(OdDb::kForWrite));
        if (block.get())
          block->releaseExtensionDictionary();
      }
    }
  }
  return res;
}
