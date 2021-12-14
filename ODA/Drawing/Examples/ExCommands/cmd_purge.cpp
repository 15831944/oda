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

// PurgeCommand.cpp: implementation of the PurgeCommand class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "DbDatabase.h"
#include "DbSymbolTable.h"
#include "DbDictionary.h"
#include "DbSymbolTable.h"
#include "DbSymbolTableRecord.h"
#include "DbCommandContext.h"
#include "Ed/EdUserIO.h"
#include "DbSymUtl.h"
#include "DbObjectIdGraph.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

namespace PurgeCommand
{
  void addTableRecords(const OdArray<OdString> *pNames, OdDbSymbolTable* pTable, OdDbObjectIdArray& ids);
  void addDictionaryRecords(const OdArray<OdString> *pNames, OdDbDictionary* pDict, OdDbObjectIdArray& ids);
  bool fillNames(const OdString& sNames, OdArray<OdString>& names);
  void addBlocks(const OdArray<OdString>* pNames, OdDbSymbolTable* pTable, OdDbObjectIdArray& ids);
}

void PurgeCommand::addBlocks(const OdArray<OdString>* pNames, OdDbSymbolTable* pTable, OdDbObjectIdArray& ids)
{
  if (pNames)
  {
    OdDbObjectId id;
    for (OdUInt32 i = 0; i < pNames->size(); ++i)
    {
      id = pTable->getAt((*pNames)[i]);
      OdDbBlockTableRecordPtr b = id.openObject();
      if (!id.isNull() && !b.isNull() && !b->isLayout())
        ids.append(id);
    }
  }
  else
  {
    OdDbSymbolTableIteratorPtr pIter = pTable->newIterator();
    for (; !pIter->done(); pIter->step())
    {
      OdDbBlockTableRecordPtr b = pIter->getRecordId().openObject();
      if (!b.isNull() && !b->isLayout())
        ids.append(b->objectId());
    }
  }
}
 
void PurgeCommand::addTableRecords(const OdArray<OdString> *pNames, OdDbSymbolTable* pTable, OdDbObjectIdArray& ids)
{
  if (pNames)
  {
    OdDbObjectId id;
    for (OdUInt32 i=0; i < pNames->size(); ++i)
    {
      id = pTable->getAt((*pNames)[i]);
      if(!id.isNull())
        ids.append(id);
    }
  }
  else
  {
    OdDbSymbolTableIteratorPtr pIter = pTable->newIterator();
    for ( ; !pIter->done(); pIter->step())
    {
      ids.append(pIter->getRecordId());
    }
  }
}

void PurgeCommand::addDictionaryRecords(const OdArray<OdString> *pNames, OdDbDictionary* pDict, OdDbObjectIdArray& ids)
{
  if (!pDict)
    return;

  if (pNames)
  {
    OdDbObjectId id;
    for (OdUInt32 i=0; i < pNames->size(); ++i)
    {
      id = pDict->getAt((*pNames)[i]);
      if(!id.isNull())
        ids.append(id);
    }
  }
  else
  {
    OdDbDictionaryIteratorPtr pIter = pDict->newIterator();
    for ( ; !pIter->done(); pIter->next())
    {
      ids.append(pIter->objectId());
    }
  }
}

bool PurgeCommand::fillNames(const OdString& sNames, OdArray<OdString>& names)
{
  ODA_ASSERT(!sNames.isEmpty());
  if(sNames!=OD_T("*"))
  {
    //int len = sNames.getLength();
    int n = 0, n2 = 0;
    OdString name;
    for(;;)
    {
      n = sNames.find(OD_T(","), n2);
      if(n > -1)
      {
        name = sNames.mid(n2, n-n2);
        n2 = n;
        ++n2;
        name.trimLeft(' ');
        name.trimRight(' ');
        names.append(name);
      }
      else
      {
        name = sNames.right(sNames.getLength() - n2);
        name.trimLeft(' ');
        name.trimRight(' ');
        names.append(name);
        break;
      }
    }
    return true;
  }
  return false;
}


OdString objectName(OdDbObject* pObj)
{
  OdDbSymbolTableRecordPtr pSymTbRec = OdDbSymbolTableRecord::cast(pObj);
  if (!pSymTbRec.isNull())
    return pSymTbRec->getName();

  OdDbDictionaryPtr pDict = OdDbDictionary::cast(pObj->ownerId().openObject());
  if (!pDict.isNull())
  {
    OdDbDictionaryIteratorPtr pIter = pDict->newIterator();
    pIter->setPosition(pObj->objectId());
    return pIter->name();
  }
  ODA_FAIL();
  return OdString();
}

OdString objectTypeName(OdDbObject* pObj)
{
  OdString res = pObj->isA()->name();
  res.replace(OD_T("AcDb"), OD_T(""));
  res.replace(OD_T("TableRecord"), OD_T(""));
  return res.makeLower();
}

void _Purge_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdEdUserIO* pIO = pDbCmdCtx->userIO();
  
  OdDbSymbolTablePtr pTable;

  // Note: , OD_T("...") OD_T("..."),    Will not compile on UNIX, so leave as a single string.
  // Note: Current UIO does not support keywords like textSTyles, only TExtstyles
  OdUInt32 nKeyword = pIO->getKeyword(
    L"Enter type of unused objects to purge [Blocks/DEtailviewstyles/DImstyles/Groups/LAyers/LTypes/MAterials/MUltileaderstyles/Plotstyles/SHapes/TExtstyles/MLinestyles/SEctionviewstyles/TAblestyles/Visualstyles/Regapps/SCales/All]:",
                                           L"Blocks DEtailviewstyles DImstyles Groups LAyers LTypes MAterials MUltileaderstyles Plotstyles SHapes TExtstyles MLinestyles SEctionviewstyles TAblestyles Visualstyles Regapps SCales All"
    );
  
  OdDbObjectIdArray ids(100,-100);
  
  OdArray<OdString> names;
  OdArray<OdString> * pNames = NULL;
  if(PurgeCommand::fillNames(pIO->getString(OD_T("Enter name(s) to purge <*>:"), OdEd::kGstAllowSpaces, OD_T("*")), names))
  {
    pNames = &names;
  }
  static const int kAll = 17;
  switch(nKeyword)
  {
  case kAll:
  case 0: //Blocks
    PurgeCommand::addBlocks(pNames, OdDbSymbolTablePtr(pDb->getBlockTableId().openObject()), ids);
    if (nKeyword != kAll)
      break;
  case 1: //Detailviewstyles
    PurgeCommand::addDictionaryRecords(pNames, (OdDbDictionary*)pDb->getDetailViewStyleDictionaryId(false).openObject().get(),   ids);
    if (nKeyword != kAll)
      break;
  case 2: //Dimstyles
    PurgeCommand::addTableRecords(pNames, OdDbSymbolTablePtr(pDb->getDimStyleTableId().openObject()),ids);
    if (nKeyword != kAll)
      break;
  case 3: //Groups
    PurgeCommand::addDictionaryRecords(pNames, (OdDbDictionary*)pDb->getGroupDictionaryId(false).openObject().get(), ids);
    if (nKeyword != kAll)
      break;
  case 4: //LAyers
    PurgeCommand::addTableRecords(pNames, OdDbSymbolTablePtr(pDb->getLayerTableId().openObject()),   ids);
    if (nKeyword != kAll)
      break;
  case 5: //LTypes
    PurgeCommand::addTableRecords(pNames, OdDbSymbolTablePtr(pDb->getLinetypeTableId().openObject()),ids);
    if (nKeyword != kAll)
      break;
  case 6: //Materials
    PurgeCommand::addDictionaryRecords(pNames, (OdDbDictionary*)pDb->getMaterialDictionaryId(false).openObject().get(),   ids);
    if (nKeyword != kAll)
      break;
  case 7: //Multileaderstyles
    PurgeCommand::addDictionaryRecords(pNames, (OdDbDictionary*)pDb->getMLeaderStyleDictionaryId(false).openObject().get(), ids);
    if (nKeyword != kAll)
      break;
  case 8: //Plotstyles
    PurgeCommand::addDictionaryRecords(pNames, (OdDbDictionary*)pDb->getPlotStyleNameDictionaryId(false).openObject().get(), ids);
    if (nKeyword != kAll)
      break;
  case 9: //SHapes
    //add SHapes here here
    if (nKeyword != kAll)
      break;
  case 10: //textSTyles
    PurgeCommand::addTableRecords(pNames, OdDbSymbolTablePtr(pDb->getTextStyleTableId().openObject()), ids);
    if (nKeyword != kAll)
      break;
  case 11: //Mlinestyles
    PurgeCommand::addDictionaryRecords(pNames, (OdDbDictionary*)pDb->getMLStyleDictionaryId(false).openObject().get(), ids);
    if (nKeyword != kAll)
      break;
  case 12: //Sectionviewstyles
    PurgeCommand::addDictionaryRecords(pNames, (OdDbDictionary*)pDb->getSectionViewStyleDictionaryId(false).openObject().get(), ids);
    if (nKeyword != kAll)
      break;
  case 13: //Tablestyles
    PurgeCommand::addDictionaryRecords(pNames, (OdDbDictionary*)pDb->getTableStyleDictionaryId(false).openObject().get(), ids);
    if (nKeyword != kAll)
      break;
  case 14: //Visualstyles
    PurgeCommand::addDictionaryRecords(pNames, (OdDbDictionary*)pDb->getVisualStyleDictionaryId(false).openObject().get(), ids);
    if (nKeyword != kAll)
      break;
  case 15: //Regapps
    if (nKeyword != kAll) // Acad does not purge regapps, and they may be referenced via name, so, leave that option only as explicit choice
    {
      PurgeCommand::addTableRecords(pNames, OdDbSymbolTablePtr(pDb->getRegAppTableId().openObject()), ids);
      break;
    }
  case 16: //Annotation Scales
    {
      OdDbDictionaryPtr pDict = pDb->getNamedObjectsDictionaryId().safeOpenObject();
      OdDbDictionaryPtr pScales = pDict->getAt(ACAD_SCALELIST, OdDb::kForRead);
      PurgeCommand::addDictionaryRecords(pNames, pScales, ids);
    }
    if (nKeyword != kAll)
      break;
  default:
    break;
  }
  OdDbObjectId dgnLSid;
  if (nKeyword == kAll) // Application may add all the dictionaries in the named dictionary to purge
  {
    OdDbDictionaryPtr pDict = pDb->getNamedObjectsDictionaryId().safeOpenObject();
    dgnLSid = pDict->getAt(L"ACAD_DGNLINESTYLECOMP");
    if (!dgnLSid.isErased())
    {
      OdDbDictionaryPtr pDgnLinestyles = dgnLSid.safeOpenObject();
      PurgeCommand::addDictionaryRecords(pNames, pDgnLinestyles, ids);
      ids.append(dgnLSid); // we add dictionary id itself, so that the styles can be purged if referenced only by the dictionary
    }
  }

  OdDbObjectIdGraph ig;
  if (nKeyword == kAll || nKeyword == 0)
  {
    for (unsigned int i = 0; i < ids.size(); ++i)
    {
      OdSmartPtr<OdDbObjectIdGraphNode> n = OdDbObjectIdGraphNode::createObject();
      n->setId(ids[i]);
      ig.addNode(n);
    }
    pDb->purge(ig);
    if (OdDbObjectIdGraphNode*n = ig.findNode(dgnLSid)) // do not erase the dictionary
      ig.delNode(n);
#ifdef _DEBUG
    ids.remove(dgnLSid);
    pDb->purge(ids);
    ODA_ASSERT(ids.size() <= (unsigned int)ig.numNodes());
#endif
    ids.clear();
    for (int i = 0; i < ig.numNodes(); ++i)
      ids.append(((OdDbObjectIdGraphNode*)ig.node(i))->id());
  }
  else
    pDb->purge(ids);
  if (ids.size() == 0)
  {
    pIO->putString(L"Nothing to purge");
    return;
  }
  
  const OdChar* szYesNoKwds = OD_T("Yes No");
  bool bManyItems = ids.size() > 1000;
  
  if (bManyItems)
    nKeyword = pIO->getKeyword(OdString().format(L"Erase %d items? [Yes/No] <Y>:", ids.size()), szYesNoKwds, 0);
  else
    nKeyword = pIO->getKeyword(OD_T("Verify each name to be purged? [Yes/No] <Y>:"), szYesNoKwds, 0);
  
  if (bManyItems && nKeyword != 0)
    return;

  OdDbObjectIdArray::iterator pId = ids.begin();
  for (unsigned int i = ids.size(); i > 0; i--, ++pId)
  {
    OdDbObjectPtr pObj = pId->openObject(OdDb::kForWrite);
    if (!pObj.isNull())
    {
      if (!bManyItems)
      {
        OdString sMsg;
        bool hasReferences = false;
        // do not ask for confirmation if the object references other objects from the set, because we may get dangling references if we skip it
        if (OdDbObjectIdGraphNode* node = ig.findNode(*pId))
          hasReferences = node->numOut() > 0;
        if (nKeyword == 0 && !hasReferences)
        {
          sMsg.format(OD_T("Purge %ls \"%ls\"? <N>"), ::objectTypeName(pObj).c_str(), ::objectName(pObj).c_str());
          if (pIO->getKeyword(sMsg, szYesNoKwds, 1) == 1)
            continue;
        }
        else
        {
          sMsg.format(OD_T("Deleting %ls \"%ls\"."), ::objectTypeName(pObj).c_str(), ::objectName(pObj).c_str());
          pIO->putString(sMsg);
        }
      }
      pObj->erase();
    }
    else
      ODA_FAIL();
  }
}
