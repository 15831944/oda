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
//
// PropSourcesImpl.cpp
//

#include "OdaCommon.h"
#include "DbSSet.h"
#include "OdModuleNames.h"
#include "Ed/EdUserIO.h"
#include "OdCharConverter.h"

#include "PropServices.h"
#include "SysVarPE.h"
#include "ExAppServices.h"
#include "PropSourcesImpl.h"
#include "ExDynamicModule.h" // for ODRX_SELECTION_SET
#include "ExtDbModule.h"
#include "ResBufPropValueConvertors.h"

//------------------------------------------------------------------------

ObjectPropSourceImpl::ObjectPropSourceImpl(OdEdCommandContext* pCtx,
                                           int flagsToUseSources, // = 0 // 0 to get via PROPSRC variable
                                           OdSelectionSet* pIds) // = NULL
  : m_pCtx(pCtx)
  , m_pIds(pIds)
  , m_flagsToUse(flagsToUseSources)
{
  ODA_ASSERT_ONCE(m_pCtx);
  if (m_flagsToUse)
    return;

  m_flagsToUse = 7;
  OdSysVarPEPtr pSysVarPE = OdSysVarPE::cast(m_pCtx);
  ODA_ASSERT_ONCE(pSysVarPE.get());
  if (pSysVarPE.isNull())
    return;
  OdString sVal = pSysVarPE->valueToString(m_pCtx, L"PROPSRC");
  ODA_ASSERT_ONCE(!sVal.isEmpty());
  if (!sVal.isEmpty())
    m_flagsToUse = overridePropSourceFlagsForPalette(m_pCtx->baseDatabase(), odStrToInt(sVal));
}

OdSelectionSet* ObjectPropSourceImpl::ids()
{
  return m_pIds.get();
}

void ObjectPropSourceImpl::setIds(OdSelectionSet* pIds)
{
  ODA_ASSERT_VAR(if (m_pIds.get() != pIds))
    m_pIds = pIds;
}

OdSelectionSetPtr ObjectPropSourceImpl::idsByCurrentFilter()
{
  ODA_ASSERT_ONCE(m_pIds.get());
  return m_pIds;
}

int ObjectPropSourceImpl::flagsToUsePropSources()
{
  return m_flagsToUse;
}

bool ObjectPropSourceImpl::updateXml(OdString& sXml)
{
  OdSelectionSetPtr pSSet = ids(); // = m_pCtx->arbitraryData(ODRX_SELECTION_SET);
  OdUInt32 numEntities = 0;
  if (pSSet.isNull() || !(numEntities = pSSet->numEntities()))
    return true; // "No Drawing" or "No Selection"

  static OdUInt32 limitPropEntities = 0; // like "GRIPOBJLIMIT"
  if (!limitPropEntities)
  {
    limitPropEntities = 500;
    OdSysVarPEPtr pSysVarPE = OdSysVarPE::cast(m_pCtx);
    ODA_ASSERT_ONCE(pSysVarPE.get());
    if (pSysVarPE.get())
    {
      OdString sVal = pSysVarPE->valueToString(m_pCtx, L"PROPOBJLIMIT"); // like "GRIPOBJLIMIT"
      ODA_ASSERT_ONCE(!sVal.isEmpty());
      if (!sVal.isEmpty())
        limitPropEntities = odStrToInt(sVal);
    }
  }
  int flgsToUse = flagsToUsePropSources();

  OdPropServicesPtr pPropServices = OdPropServices::cast(m_pCtx);
  ODA_ASSERT_ONCE(!pPropServices.isNull());

  int indexEnt = 0;
  for (OdSelectionSetIteratorPtr pIter = pSSet->newIterator(); 
       !pIter->done(); pIter->next(), indexEnt++)
  {
    OdDbStub* id = pIter->id();

    // !!!***Attention***!!!
    // the next parent node should be added for each selected entiry
    // (number of it is used for requested internal merging operation)
    sXml += L"<Property Path=\"\" Name=\"Entity\" Type=\"category\" Filter=\"";
    sXml += ToolPropSourceImpl::entityClassName(m_pCtx, id);
    sXml += L"\" />\n";

    if (pPropServices.isNull() || numEntities > limitPropEntities)
      continue;

    OdStringArray asClassNames = collectClassNames(id);
    if (asClassNames.isEmpty())
      continue;

    OdString sXmlEntity = pPropServices->collectXmlProps(L"Entity", id, asClassNames, 
                                                         flgsToUse, m_pCtx->baseDatabase());
    if (sXmlEntity.isEmpty())
      continue;

    substituteValues(id, sXmlEntity);
    sXml += sXmlEntity;
  }

  return true;
}

// for $(...)
bool ObjectPropSourceImpl::substituteValues(OdDbStub* id, OdString& sXmlEntity)
{
  OdPropServicesPtr pPropServices = OdPropServices::cast(m_pCtx);
  ODA_ASSERT_ONCE(!pPropServices.isNull());

  bool bRes = false;
  int posStart = 0,
      pos = sXmlEntity.find(OD_T("$("));
  for (; pos >= 0; pos = sXmlEntity.find(OD_T("$("), posStart))
  {
    int posLn = posStart;
    posStart = pos;

    int posEnd = sXmlEntity.find(OD_T(")"), pos);
    ODA_ASSERT_ONCE(posEnd > pos);
    if (posEnd < pos)
      return false;
    OdString sKeyToReplace = sXmlEntity.mid(pos, (++posEnd) - pos),
             sConvertor = sXmlEntity.mid(pos + 2, (posEnd) - pos - 3).trimLeft().trimRight(),
             sValue,
             sArgs; // (GroupCode(%2)  maybe one in it)
    sConvertor = extractFirst((sArgs = sConvertor), __OD_T(','), true);

    // extract path/name - is a key to get convertor name
    //                     for setPropetyValue and getPropertyPossibleValues
    OdString sPathName;
    int posNext = posLn;
    for (; posNext >= 0 && posNext < posStart; posNext = sXmlEntity.find(__OD_T('\n'), posLn + 1))
    {
      posLn = posNext;
    }
    posNext = sXmlEntity.find(OD_T(" Path=\""), posLn);
    ODA_ASSERT_ONCE(posNext > 0);
    if (posNext > 0)
    {
      int posPath = posNext + OdString(OD_T(" Path=\"")).getLength();
      posNext = sXmlEntity.find(__OD_T('\"'), posPath);
      ODA_ASSERT_ONCE(posNext > posPath);
      if (posNext > posPath)
        sPathName = sXmlEntity.mid(posPath, posNext - posPath);
      posNext = sXmlEntity.find(OD_T(" Name=\""), posLn);
      ODA_ASSERT_ONCE(posNext > 0);
      if (posNext > 0)
      {
        if (!sPathName.isEmpty())
          sPathName += OD_T("/");
        int posName = posNext + OdString(OD_T(" Name=\"")).getLength();
        posNext = sXmlEntity.find(__OD_T('\"'), posName);
        ODA_ASSERT_ONCE(posNext > posName);
        if (posNext > posName)
          sPathName += sXmlEntity.mid(posName, posNext - posName);
      }
    }
    if (extractFirst(sPathName) != OD_T("Entity"))
    {
      ODA_FAIL_ONCE();
    }
    ODA_ASSERT_ONCE(!sPathName.isEmpty());
    if (!sPathName.isEmpty())
    {
      getResBufPropController().addPath(sPathName, sConvertor);
      TConvertorFromResBufPropValue* pFunc = getResBufPropController().getConvFuncFromRB(sPathName);

      if (pFunc)
      {
        OdString sSvPath = getResBufPropController().setCurrentPath(sPathName); 
        sValue = (*pFunc)(sArgs, id);
        getResBufPropController().setCurrentPath(sSvPath); // restore
      }
      else
      {
        // LAYER etc :
        sValue = sArgs;
        ODA_ASSERT_ONCE(!extractFirst(sArgs, __OD_T(','), true).isEmpty() && sArgs.isEmpty()); // test
      }
    }

    sXmlEntity.replace(sKeyToReplace, sValue);
  } // end for
  return bRes;
}

bool ObjectPropSourceImpl::setPropertyValue(const OdString& csPath,
                                            const OdString& csValue,
                                            Action& action)
{
  //Dgn supports undo/redo now too
  //OdDbDatabase* pDwgDb = OdDbDatabase::cast(m_pCtx->baseDatabase()).get();
  //if (!pDwgDb)
  //  return setPropertyValueDirect(csPath, csValue, action);

  ExAppServicesPtr pExAppServices = ExAppServices::cast(::odrxSysRegistry()->getAt(EX_APP_SERVICES).get());
  ODA_ASSERT_ONCE(!pExAppServices.isNull());
  if (pExAppServices.isNull())
    return setPropertyValueDirect(csPath, csValue, action);

  OdString sCmd,
           sValue = OdPropContext::encodeToXmlAttrib(csValue); // CORE-13627
  sCmd.format(OD_T("setprop \"%ls\" \"%ls\""), csPath.c_str(), sValue.c_str());
  pExAppServices->postponeCommand(sCmd, false, m_pCtx->baseDatabase());
  return false;
}

bool ObjectPropSourceImpl::setPropertyValueDirect(const OdString& csPath,
                                                  const OdString& csValue,
                                                  Action& action)
{
  OdString sPath = csPath;
  OdString sName = extractFirst(sPath);
  if (sName != OD_T("Entity") && sName != OD_T("Merged"))
  {
    ODA_FAIL_ONCE();
    return false;
  }
  ODA_ASSERT_ONCE(csValue != OD_T("*VARIES*"));

  OdSelectionSetPtr pSSet = idsByCurrentFilter(); //  = m_pToolSource->selectionSetByCurrentFilter();
  if (pSSet.isNull() || !pSSet->numEntities())
    return true; // "No Drawing" or "No Selection"

  int flgsToUse = flagsToUsePropSources();

  OdPropServicesPtr pPropServices = OdPropServices::cast(m_pCtx);
  ODA_ASSERT_ONCE(!pPropServices.isNull());
  if (pPropServices.isNull())
    return false;

  OdSelectionSetIteratorPtr pIter = pSSet->newIterator();

  OdString sValue = csValue,
           sHintGrCode;
  TConvertorToResBufPropValue* pFunc = getResBufPropController().getConvFuncToRB(sPath);
  bool bFuncForeach = false;

  OdString sErrMsg;
  bool bRes = true;
  int indexEnt = 0;
  for (; !pIter->done(); pIter->next(), indexEnt++)
  {
    OdDbStub* id = pIter->id();
    if (pFunc)
    {
      OdString sSvPath = getResBufPropController().setCurrentPath(sPath); 
      sValue = (*pFunc)(csValue, id, 
                        sHintGrCode); // out - for alternative gr code - like PloteStyle
      getResBufPropController().setCurrentPath(sSvPath); // restore
      if (sValue == TD_MARKER_TO_CANCEL)
        return false;
      if (sValue == TD_MARKER_OUT_OF_RANGE)
      {
        sErrMsg = TD_MARKER_OUT_OF_RANGE;
        bRes = false;
        break;
      }
      if (sValue == TD_MARKER_FOREACH_TO_SET_DIRECTLY)
        bFuncForeach = true;
    }
    if (bFuncForeach)
      continue; // already set via convertor
    else
      pFunc = NULL; // skip for others

    OdStringArray asClassNames = collectClassNames(id);
    if (asClassNames.isEmpty())
    {
      bRes = false;
      break;
    }

    sErrMsg = pPropServices->setValueXmlProps(id, asClassNames,
                                              sPath, sValue, sHintGrCode, action, 
                                              flgsToUse, m_pCtx->baseDatabase());
    if (!sErrMsg.isEmpty())
    {
      bRes = false;
      break;
    }
  }
  if (!bRes)
  {
    if (!sErrMsg.isEmpty())
    {
      sErrMsg = OdString(OD_T("Warninig:  ")) + sErrMsg;
      m_pCtx->userIO()->putString(sErrMsg);
    }
    //ODA_FAIL_ONCE();
    return false;
  }
  ODA_ASSERT_ONCE(sErrMsg.isEmpty());

  return true;
}

bool ObjectPropSourceImpl::getPropertyPossibleValues(const OdString& csPath,
                                                     OdStringArray& values,
                                                     OdStringArray& iconPaths)
{
  OdString sPath = csPath;
  OdString sName = extractFirst(sPath);
  if (sName != L"Entity" && sName != L"Merged")
  {
    ODA_FAIL_ONCE();
    return false;
  }

  TConvertorPossibleValues* pFunc = getResBufPropController().getConvFuncGetPossible(sPath);
  if (pFunc)
  {
    OdString sSvPath = getResBufPropController().setCurrentPath(sPath); 
    bool bRes = (*pFunc)(values, iconPaths, m_pCtx->baseDatabase());
    getResBufPropController().setCurrentPath(sSvPath); // restore
    return bRes;
  }

  OdSelectionSetPtr pSSet = idsByCurrentFilter(); // = m_pToolSource->selectionSetByCurrentFilter();
  if (pSSet.isNull() || !pSSet->numEntities())
    return false; // "No Drawing" or "No Selection"

  int flgsToUse = flagsToUsePropSources();

  OdPropServicesPtr pPropServices = OdPropServices::cast(m_pCtx);
  ODA_ASSERT_ONCE(!pPropServices.isNull());

  OdSelectionSetIteratorPtr pIter = pSSet->newIterator();
  ODA_ASSERT_VAR(int indexEnt = -1;)
  for (; !pIter->done(); pIter->next())
  {
    ODA_ASSERT_VAR(indexEnt++;)
    OdDbStub* id = pIter->id();

    OdStringArray asClassNames = collectClassNames(id);
    if (asClassNames.isEmpty())
      continue;

    if (pPropServices->getPossibleValuesXmlProps(id, asClassNames, sPath, values, iconPaths,
                                                 flgsToUse, m_pCtx->baseDatabase()))
      return true;
  }
  return false;
}

OdStringArray ObjectPropSourceImpl::collectClassNames(OdDbStub* id)
{
  OdStringArray names;
  OdRxObject* pRxDb = m_pCtx->baseDatabase();
  OdDbBaseDatabasePEPtr pDbPe = OdDbBaseDatabasePE::cast(pRxDb);
  if (pDbPe.isNull())
    return names;

  OdRxObjectPtr pObject = pDbPe->openObject(id);
  if (pObject.isNull())
  {
    //ODA_ASSERT_ONCE(id); // erased or database if NULL
    OdString sName = ToolPropSourceImpl::entityClassName(m_pCtx, id, false);
    names.push_back(sName);
    return names;
  }

  for (OdRxClass *pClass = pObject->isA(); pClass; pClass = pClass->myParent())
  {
    ODA_ASSERT_ONCE(pClass && !pClass->name().isEmpty());
    OdString sName = pClass->name();
    if (sName == L"OdRxObject" || sName == "AcGiDrawable")
      continue; // are useless for props

    names.insertAt(0, sName);
  }

  return names;
}

//------------------------------------------------------------------------

ToolPropSourceImpl::ToolPropSourceImpl(OdEdCommandContext* pCtx)
  : m_pCtx(pCtx)
  , m_numSelEntitiesOfCurrFileter(0)
  , m_pEntitySource(NULL)
{
  ODA_ASSERT_ONCE(m_pCtx);
}

// is used for setMergeFilter
void ToolPropSourceImpl::setEntitySource(const OdString& sDlgKey, EntityPropSourceImpl* pEntitySource)
{
  m_sDlgKey = sDlgKey;
  m_pEntitySource = pEntitySource;
}

//static 
OdString ToolPropSourceImpl::entityClassName(OdEdCommandContext* pCtx,
                                             OdDbStub* id, 
                                             bool bCutPrefix) // = true
{
  ODA_ASSERT_ONCE(pCtx);
  OdRxObject* pRxDb = pCtx->baseDatabase();
  OdString sName, sPrefToCut;
  OdExtDbModule* pExtDb = getExtDbModule(pRxDb);
  if (!pExtDb)
  {
    sPrefToCut = L"AcDb";
    if (id)
    {
      OdDbObjectId idObject(id);
      OdDbObjectPtr pObject = idObject.safeOpenObject();
      sName = pObject->isA()->name();
    }
    else
    {
      sName = OdDbDatabase::desc()->name();
      sPrefToCut = L"OdDb";
    }
  }
  else
  {
    sName = pExtDb->getElementClassName(id);
    sPrefToCut.empty();

    if (!sName.isEmpty())
    {
      if (!sName.find(L"OdDg"))
        sPrefToCut = L"OdDg";
      else if (!sName.find(L"OdIfc")) // L"OdIfc...::Ifc"
      {
        int pos = sName.find(L"::Ifc");
        if (pos > 0)
          sPrefToCut = sName.left(pos + 5);
        else if (sName == L"OdIfcFile")
          sPrefToCut = L"Od";
        else
          ODA_FAIL_ONCE(); // TODO
      }
      else
        ODA_FAIL_ONCE(); // TODO
    }
    else
    {
      //ODA_FAIL_ONCE(); // erased
      sName = L"Unknown";
      bCutPrefix = false;
    }
  }

  if (!sName.isEmpty())
  {
    OdPropServicesPtr pPropServices = OdPropServices::cast(pCtx);
    ODA_ASSERT_ONCE(pPropServices.get());
    if (pPropServices.get())
    {
      if (   pPropServices->overrideClassName(pRxDb, id, sName) // PROD-143
          && bCutPrefix && sName.find(sPrefToCut)) // is not started with ...
        bCutPrefix = false, sPrefToCut.empty();
    }
  }

  ODA_ASSERT_ONCE(   !bCutPrefix 
                  || (   !sPrefToCut.isEmpty() 
                      && (   !sName.find(sPrefToCut) 
                          || !sName.find(L"Asdk"))));   // AsdkSmiley // test (? for custom proxy)
  if (bCutPrefix && !sName.find(sPrefToCut))
    sName = sName.mid(sPrefToCut.getLength());

  if (sName == L"ViewBorder")
    sName = L"Drawing View";

  ODA_ASSERT_ONCE(!sName.isEmpty());
  return sName;
}

// return false if there is not changes
const OdStringArray& ToolPropSourceImpl::collectFilters(bool* pbChanged) const // = NULL
{
  if (pbChanged)
    *pbChanged = false;

  if (!m_pCtx->baseDatabase())
  {
    if (!m_sCurrFilter.isEmpty())
      return m_asFilters;
    if (pbChanged)
      *pbChanged = true;

    m_sCurrFilter = OD_T("No Drawing");
    m_asFilters.push_back(m_sCurrFilter);
    return m_asFilters;
  }

  OdSelectionSetPtr pSSet = m_pCtx->arbitraryData(ODRX_SELECTION_SET);
  int count = 0;
  if (!pSSet.isNull())
    count = pSSet->numEntities();

  if (count == m_numSelEntitiesOfCurrFileter && !m_sCurrFilter.isEmpty())
    return m_asFilters;
  if (pbChanged)
    *pbChanged = true;
  m_pFilteredSSet = NULL;

  if (!count)
  {
    m_sCurrFilter = OD_T("No Selection");
    m_asFilters.push_back(m_sCurrFilter);
    m_numSelEntitiesOfCurrFileter = count;
    return m_asFilters;
  }

  m_sCurrFilter.empty();
  m_asFilters.clear();

  std::map<OdString, int> mapFilters; // <name, count>
  std::map<OdString, int>::iterator pMapItr;

  for (OdSelectionSetIteratorPtr pIter = pSSet->newIterator();
       !pIter->done(); pIter->next())
  {
    OdString sName = entityClassName(m_pCtx, pIter->id());
    ODA_ASSERT_ONCE(!sName.isEmpty());

    int cnt = 1;
    pMapItr = mapFilters.find(sName);
    if (pMapItr != mapFilters.end())
      cnt = pMapItr->second + 1;
    mapFilters[sName] = cnt;
  }

  int numTypes = (int) mapFilters.size();
  ODA_ASSERT_ONCE(numTypes >= 1);
  if (numTypes > 1)
    mapFilters[OD_T("All")] = count;

  for (pMapItr = mapFilters.begin(); pMapItr != mapFilters.end(); pMapItr++)
  {
    OdString sFilter = pMapItr->first;
    int cnt = pMapItr->second;
    ODA_ASSERT_ONCE(!pMapItr->first.isEmpty() && cnt >= 1);
    if (cnt > 1 || numTypes > 1)
      sFilter.format(OD_T("%ls (%d)"), pMapItr->first.c_str(), cnt);

    m_asFilters.push_back(sFilter);
    if (m_sCurrFilter.isEmpty())
      m_sCurrFilter = sFilter;
  }
  m_numSelEntitiesOfCurrFileter = count;
  return m_asFilters;
}

OdString ToolPropSourceImpl::getCurrentFilter(bool bAsMergeFilter) const //  = false
{
  bool bChanged = false;
  collectFilters(&bChanged);
  if (bChanged)
    m_pFilteredSSet = NULL;
  if (!bAsMergeFilter)
    return m_sCurrFilter;

  OdString sMergeFilter = m_sCurrFilter;
  int pos = sMergeFilter.find(L'(');
  if (pos > 0 && !OdCharConverter::isDigit(sMergeFilter.getAt(pos + 1)))
    pos = sMergeFilter.find(L'(', pos + 1);
  if (pos > 0)
  {
    sMergeFilter = sMergeFilter.left(pos);
    sMergeFilter.trimRight();
  }
  ODA_ASSERT_ONCE(!sMergeFilter.isEmpty());

  if (sMergeFilter == L"All")
    sMergeFilter.empty();

  return sMergeFilter;
}

//static 
OdSelectionSetPtr ToolPropSourceImpl::createSelectionSet(OdRxObject* pRxDatabase)
{
  ODA_ASSERT_ONCE(pRxDatabase);
  if (!pRxDatabase)
    return OdSelectionSetPtr();

  OdSelectionSetPtr pSSet;
  OdExtDbModule* pExtDb = getExtDbModule(pRxDatabase);
  if (!pExtDb)
    pSSet = OdDbSelectionSet::createObject(OdDbDatabase::cast(pRxDatabase));
  else
    pSSet = pExtDb->createSelectionSet(pRxDatabase);
  ODA_ASSERT_ONCE(   pSSet.get() 
                  || (pExtDb && pExtDb->baseModuleName(true) == L"bim")
                  || (pExtDb && pExtDb->baseModuleName(true) == L"prc")
                  || (pExtDb && pExtDb->baseModuleName(true) == L"dwf"));
  return pSSet;
}

OdSelectionSetPtr ToolPropSourceImpl::selectionSetByCurrentFilter(bool* pbChanged) const // = NULL
{
  if (pbChanged)
    *pbChanged = false;

  OdString sFilter = getCurrentFilter();
  if (!m_pFilteredSSet.isNull())
    return m_pFilteredSSet;
  
  if (pbChanged)
    *pbChanged = true;

  OdSelectionSetPtr pSSet = m_pCtx->arbitraryData(ODRX_SELECTION_SET);
  if (pSSet.isNull() || !pSSet->numEntities())
    return OdSelectionSetPtr(); // "No Drawing" or "No Selection"

  if (m_pFilteredSSet.isNull())
  {
    m_pFilteredSSet = createSelectionSet(m_pCtx->baseDatabase());
    if (m_pFilteredSSet.isNull())
      return OdSelectionSetPtr();
  }
  else
    m_pFilteredSSet->clear();

  if (!pSSet->numEntities())
    return m_pFilteredSSet; // "No Selection"

  int pos = sFilter.find(L'(');
  if (pos > 0 && !OdCharConverter::isDigit(sFilter.getAt(pos + 1)))
    pos = sFilter.find(L'(', pos + 1);
  //int countFlt = 1;
  if (pos > 0)
  {
    //odSScanf(sFilter.mid(pos).c_str(), OD_T("(%i)"), &countFlt);
    sFilter = sFilter.left(pos);
    sFilter.trimRight();
  }
  ODA_ASSERT_ONCE(!sFilter.isEmpty());

  // populate selection set by filter
  for (OdSelectionSetIteratorPtr pIter = pSSet->newIterator();
       !pIter->done(); pIter->next())
  {
    if (sFilter != OD_T("All") && !sFilter.isEmpty())
    {
      OdString sName = entityClassName(m_pCtx, pIter->id());
      if (sName != sFilter)
        continue;
    }

    if (!pIter->subentCount())
    {
      m_pFilteredSSet->append(pIter->id());
      continue;
    }
      
    for (OdUInt32 se = 0; se < pIter->subentCount(); se++)
    {
      OdDbBaseFullSubentPath subEntPath;
      if (pIter->getSubentity(se, subEntPath))
        m_pFilteredSSet->append(subEntPath);
    }
  }
  return m_pFilteredSSet;
}

bool ToolPropSourceImpl::updateXml(OdString& sXml)
{
  sXml += OD_T("<Property Path=\"\" Name=\"Tools\" Type=\"category\" Layout=\"hor\" />\n");
  sXml += OD_T("<Property Path=\"Tools\" Name=\"SelFilter\" VisibleName=\"\" Type=\"combo\" Value=\"");
  sXml += getCurrentFilter();
  sXml += OD_T("\" />\n");

  OdSysVarPEPtr pSysVarPE = OdSysVarPE::cast(m_pCtx);
  ODA_ASSERT_ONCE(!pSysVarPE.isNull());
  if (!pSysVarPE.isNull())
  {
    //sXml += __OD_T("<Property Path=\"Tools\" Name=\"PICKADD\" Type=\"button\"")
    //                 __OD_T(" Comment=\"Toggle value of PICKADD Sysvar\" />\n");
    OdString sPickadd = pSysVarPE->valueToString(m_pCtx, L"PICKADD");
    bool bPickadd = !(sPickadd.makeLower() == OD_T("false") || sPickadd == OD_T("0"));
    sXml += __OD_T("<Property Path=\"Tools\" Name=\"PICKADD\" Type=\"boolPictured\"")
            __OD_T(" Comment=\"Toggle value of PICKADD Sysvar\"")
            __OD_T(" Image=\":true=:/images/property/tool_pickadd1.png:false=:/images/property/tool_pickadd0.png\"")
            __OD_T(" Checkable=\"true\"")
            __OD_T(" Alignment=\"right\"")
            __OD_T(" Value=\"");
    sXml += (bPickadd ? OD_T("true") : OD_T("false"));
    sXml += OD_T("\" />\n");
  }

  sXml += __OD_T("<Property Path=\"Tools\" Name=\"SelectObjects\" Type=\"boolPictured\"")
          __OD_T(" Comment=\"Select Objects\"")
          __OD_T(" Alignment=\"right\"")
          __OD_T(" Image=\":/images/property/tool_select.png\" />\n");
  //sXml += __OD_T("<Property Path=\"Tools\" Name=\"QuickSelect\" Type=\"boolPictured\"")
  //        __OD_T(" Comment=\"Quick Select\"");
  //        __OD_T(" Alignment=\"right\"")
  //        __OD_T(" Image=\":/images/property/tool_qselect.png\" />\n");

  return true;
}

bool ToolPropSourceImpl::setPropertyValue(const OdString& csPath,
                                          const OdString& sValue,
                                          Action& action)
{
  OdString sPath = csPath;
  OdString sName = extractFirst(sPath);
  if (sName == OD_T("Tools"))
  {
    if (sPath == OD_T("SelFilter"))
    {
      ODA_ASSERT_ONCE(!sValue.isEmpty())
      if (m_sCurrFilter == sValue)
        return false;
      m_pFilteredSSet = NULL;
      m_sCurrFilter = sValue;

      OdPropServicesPtr pPropServices = OdPropServices::cast(m_pCtx);
      ODA_ASSERT_ONCE(!pPropServices.isNull() && !m_sDlgKey.isEmpty() && m_pEntitySource);
      if (!pPropServices.isNull() && !m_sDlgKey.isEmpty() && m_pEntitySource)
        pPropServices->setMergeFilter(m_sDlgKey, m_pEntitySource, getCurrentFilter(true));
      else
        action = enAction_RefreshSource;
      return true;
    }

    if (sPath == OD_T("PICKADD"))
    {
      ExAppServicesPtr pExAppServices = ExAppServices::cast(::odrxSysRegistry()->getAt(EX_APP_SERVICES).get());
      ODA_ASSERT_ONCE(!pExAppServices.isNull());
      if (pExAppServices.isNull())
        return false;
      OdString sCmd = OD_T("PICKADD");
      if (sValue == OD_T("false") || sValue == OD_T("0"))
        sCmd += OD_T(" 0");
      else
        sCmd += OD_T(" 1");
      pExAppServices->executeCommand(sCmd,
                                     true, // bEcho
                                     m_pCtx->baseDatabase());
      action = enAction_RefreshSourceWidgets;
      return true;
    }
  
    if (sPath == OD_T("SelectObjects"))
    {
      ExAppServicesPtr pExAppServices = ExAppServices::cast(::odrxSysRegistry()->getAt(EX_APP_SERVICES).get());
      ODA_ASSERT_ONCE(!pExAppServices.isNull());
      if (pExAppServices.isNull())
        return false;
      pExAppServices->executeCommand(OD_T("pselect"), true, m_pCtx->baseDatabase());
      return true;
    }

    ODA_FAIL_ONCE(); // TODO
    return false;
  }

  return false;
}

bool ToolPropSourceImpl::getPropertyPossibleValues(const OdString& csPath,
                                                   OdStringArray& values,
                                                   OdStringArray&) // iconPaths
{
  OdString sPath = csPath;
  OdString sName = extractFirst(sPath);
  if (   sName == OD_T("Tools")
      && sPath == OD_T("SelFilter"))
  {
    const OdStringArray& filters = collectFilters();
    ODA_ASSERT_ONCE(filters.size());
    for (unsigned index = 0; index < filters.size(); index++)
      values.push_back(filters[index]);
    return true;
  }

  return false;
}

//------------------------------------------------------------------------

EntityPropSourceImpl::EntityPropSourceImpl(OdEdCommandContext* pCtx, 
                                           int flagsToUseSources, // = 0 // 0 to get via PROPSRC variable
                                           ToolPropSourceImpl* pToolSource)
  : ObjectPropSourceImpl(pCtx, flagsToUseSources)
  , m_pToolSource(pToolSource)
{
  ODA_ASSERT_ONCE(m_pCtx); // && pToolSource);
  setIds(OdSelectionSet::cast(m_pCtx->arbitraryData(ODRX_SELECTION_SET)).get());
}

OdSelectionSet* EntityPropSourceImpl::ids()
{
  // ids should be updated always (we came here via "<event-refresh:selection_changed>" too //if (m_pIds.isNull())
    setIds(OdSelectionSet::cast(m_pCtx->arbitraryData(ODRX_SELECTION_SET)).get());
  return m_pIds.get();
}

OdSelectionSetPtr EntityPropSourceImpl::idsByCurrentFilter()
{
  if (m_pToolSource)
    return m_pToolSource->selectionSetByCurrentFilter();
  return ids();
}
