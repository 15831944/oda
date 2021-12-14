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
// SysVarPEImpl.cpp
//

#include "OdaCommon.h"
#include "SysVarPEImpl.h"
#include "StaticRxObject.h"

// for OdDbSysVarPEImpl :
#include "DbDatabase.h"
#include "DbCommandContext.h"
#include "RxDictionary.h"
#include "SysVarInfo.h"

#include "OdqInterfaces.h"
// for OdqAppSysVarPEImpl :
#include <QColorDialog>
#include <QFileDialog>
#include "OdqInterfaces.h"
#define STL_USING_MAP
#include "OdaSTL.h"

// for OdDgSysVarPEImpl :
#include "ExtDbModule.h"

#include "PropServices.h"
#include "ExAppServices.h"
#include "OdCharMapper.h"

///////////////////////////////////////////////////////////////////////////////

#if defined(ODA_SYS_VAR_PE_UNLINKED_RXINIT)
  ODRX_NO_CONS_DEFINE_MEMBERS(OdSysVarPE, OdRxObject);
#endif

  // it's here to prevent impossible linking with OdaQtApp :
#if defined(ODA_PROP_SERVICES_UNLINKED_RXINIT) && defined(_TOOLKIT_IN_DLL_)
  ODRX_NO_CONS_DEFINE_MEMBERS(OdPropServices, OdRxObject);
#endif

  // it's here to prevent impossible linking with OdaQtApp :
#if defined(ODA_EX_APP_SERVICES_UNLINKED_RXINIT) && defined(_TOOLKIT_IN_DLL_)
  ODRX_NO_CONS_DEFINE_MEMBERS(ExAppServices, OdRxObject);
#endif

///////////////////////////////////////////////////////////////////////////////

static bool editResBuf(OdDbUserIOPtr pIO, const OdString& csName,
                       OdResBufPtr pRb, // out
                       enum OdResBuf::ValueType typeRb,
                       const OdString& sValue)
{
  OdString sName = csName,
           sPrompt;
  sPrompt.format(OD_T("Enter new value for %ls <%ls>:"), sName.makeUpper().c_str(), sValue.c_str()); // pmtEnterNewValueFor

  try
  {
    switch (typeRb)
    {
    case OdResBuf::kRtDouble:
      pRb->setRestype(OdResBuf::kRtDouble);
      pRb->setDouble(pIO->getReal(sPrompt, OdEd::kInpThrowEmpty, 0.0, sValue));
      break;
    case OdResBuf::kRtPoint2d:
      pRb->setRestype(OdResBuf::kRtPoint2d);
      pRb->setPoint3d(pIO->getPoint(sPrompt, OdEd::kInpThrowEmpty, NULL, sValue));
      break;
    case OdResBuf::kRtInt16:
      pRb->setRestype(OdResBuf::kRtInt16);
      pRb->setInt16(OdInt16(pIO->getInt(sPrompt, OdEd::kInpThrowEmpty, 0, sValue)));
      break;
    case OdResBuf::kRtAngle:
      pRb->setRestype(OdResBuf::kRtAngle);
      pRb->setDouble(pIO->getAngle(sPrompt, OdEd::kInpThrowEmpty, 0.0, sValue));
      break;
    case OdResBuf::kRtString:
      pRb->setRestype(OdResBuf::kRtString);
      pRb->setString(pIO->getString(sPrompt, OdEd::kGstAllowSpaces | OdEd::kInpThrowEmpty, sValue, sValue));
      break;
    case OdResBuf::kRtOrient:
      pRb->setRestype(OdResBuf::kRtOrient);
      pRb->setDouble(pIO->getAngle(sPrompt, OdEd::kGanNoAngBase | OdEd::kInpThrowEmpty, 0.0, sValue));
      break;
    case OdResBuf::kRtPoint3d:
      pRb->setRestype(OdResBuf::kRtPoint3d);
      pRb->setPoint3d(pIO->getPoint(sPrompt, OdEd::kInpThrowEmpty, NULL, sValue));
      break;
    case OdResBuf::kRtInt32:
      pRb->setRestype(OdResBuf::kRtInt32);
      pRb->setInt32(OdInt32(pIO->getInt(sPrompt, OdEd::kInpThrowEmpty, 0, sValue)));
      break;
    case OdResBuf::kRtColor:
      pRb->setRestype(OdResBuf::kRtColor);
      pRb->setColor(pIO->getColor(sPrompt, OdEd::kInpThrowEmpty, NULL, sValue));
      break;
    case OdResBuf::kRtBool:
      pRb->setRestype(OdResBuf::kRtBool);
      try
      {
        for (;;)
        {
          //int n = pIO->getInt(sPrompt, OdEd::kInpThrowEmpty); //, 0, sValue); if (n != 0 && n != 1) ... //pRb->setBool(n == 1);
          OdString sVal = pIO->getString(sPrompt, OdEd::kInpThrowEmpty);
          if (sVal.makeLower() == L"true" || sVal == L"on" || sVal == L"1")
            pRb->setBool(true);
          else if (sVal == L"false" || sVal == L"off" || sVal == L"0")
            pRb->setBool(false);
          else
          {
            pIO->putString(OD_T("Requires 0 or 1 only")); // , or option keyword."));  // formatMsg(msgRequires0or1onlyOrOptionKeyword)
            continue;
          }
          break;
        }
      }
      catch (const OdEdKeyword& kw)
      {
        pRb->setBool(!kw.keywordIndex());
      }
      break;
    case OdResBuf::kRtInt8:
      pRb->setInt8(OdInt8(pIO->getInt(sPrompt, OdEd::kInpThrowEmpty, 0, sValue)));
      break;
    default:
      ODA_FAIL_ONCE(); // TODO
      return false;
    }
    return true;
  }
  catch (const OdEdKeyword&)
  {
  }
  catch (const OdEdEmptyInput&)
  {
  }
  catch (const OdEdCancel&)
  {
  }
  return false;
}

///////////////////////////////////////////////////////////////////////////////

static bool osmode_edit(OdEdCommandContext* pCmdCtx,
                        const OdString& sName, const OdString& sDefVal, enum OdResBuf::ValueType typeRb)
{
  OdRxObject* pRxDb = pCmdCtx->baseDatabase();
  OdDbUserIOPtr pIO = OdDbUserIO::cast(pCmdCtx->userIO());
  OdResBufPtr pRb = OdResBuf::createObject();
  if (!editResBuf(pIO, sName, pRb, typeRb, sDefVal))
    return false;

  OdString sValue = pRb->getString();
  sValue.makeLower();
  return getIAppProps()->set(L"Vars/osmode", sValue, L"int");
}

static bool perspective_value(OdRxObject* pRxDb)
{
  bool bVal = false;
  OdDbDatabasePtr pDb = OdDbDatabase::cast(pRxDb);
  OdExtDbModule* pExtDb = NULL;
  ODA_ASSERT_ONCE(pDb.isNull()); // unused case
  if (pDb.get())
    bVal = pDb->getSysVar(L"PERSPECTIVE")->getBool();
  else if (pRxDb && (pExtDb = getIApp()->getExtDbModule(pRxDb)) != NULL)
  {
    OdGsDevice* pGsDevice = NULL;
    if (IqViewSystem* iVS = getIViewSystem())
      if (IqView* iView = iVS->getView(pRxDb))
        pGsDevice = iView->getGsDevice();
    bVal = pExtDb->isPerspective(pRxDb, pGsDevice);
  }
  return bVal;
}

static bool perspective_edit(OdEdCommandContext* pCmdCtx, 
                             const OdString& sName, const OdString& sDefVal, enum OdResBuf::ValueType typeRb)
{
  OdRxObject* pRxDb = pCmdCtx->baseDatabase();
  OdDbUserIOPtr pIO = OdDbUserIO::cast(pCmdCtx->userIO());
  OdResBufPtr pRb = OdResBuf::createObject();
  if (!editResBuf(pIO, sName, pRb, typeRb, sDefVal))
    return false;

  OdDbDatabasePtr pDb = OdDbDatabase::cast(pRxDb);
  if (pDb.get())
  {
    ODA_FAIL_ONCE(); // unused case
    //OdResBufPtr val = OdResBuf::createObject(); val->setRestype(OdResBuf::kRtBool); val->setBool(bValue);
    pDb->setSysVar(L"PERSPECTIVE", pRb);
    return true;
  }
  if (OdExtDbModule* pExtDb = getIApp()->getExtDbModule(pRxDb))
  {
    bool bValue = pRb->getBool();
    return pExtDb->setPerspective(pRxDb, bValue);
  }
  return false;
}

inline bool perspective_hidden(OdRxObject* pRxDb)
{
  OdDbDatabasePtr pDwgDb = OdDbDatabase::cast(pRxDb);
  if (!pRxDb || pDwgDb.get())
    return true;
  return false;
}

///////////////////////////////////////////////////////////////////////////////

class OdDbSysVarPEImpl : public OdSysVarPE
{
public:
  ODRX_DECLARE_MEMBERS(OdDbSysVarPEImpl);

  // pRxObject - is OdDbBaseDatabase

  virtual bool getNames(OdRxObject* pRxObject, OdStringArray& names, bool bSkipHidden = true);
  virtual OdString valueToString(OdRxObject* pRxObject, const OdString& sName,
                                 OdSysVarType* pRbType = NULL, bool* pbExist = NULL);
  virtual bool isEditable(OdRxObject* pRxObject, const OdString& sName, bool* pbExist = NULL);
  virtual bool editValue(OdEdCommandContext* pCmdCtx, const OdString& sName, bool* pbExist = NULL);
};
//typedef OdSmartPtr<OdDbSysVarPEImpl> OdDbSysVarPEImplPtr;

ODRX_NO_CONS_DEFINE_MEMBERS(OdDbSysVarPEImpl, OdSysVarPE);

bool OdDbSysVarPEImpl::getNames(OdRxObject*, // pRxObject
                                OdStringArray& names, 
                                bool) // bSkipHidden = true
{
  OdRxDictionaryPtr pVarDict = odrxSysRegistry()->getAt(OD_T("ODDB_SYSVARDICT"));

  for (OdRxDictionaryIteratorPtr pVarInfos = pVarDict->newIterator(OdRx::kDictSorted);
       !pVarInfos->done(); pVarInfos->next())
  {
    OdString sName = pVarInfos->getKey();
    ODA_ASSERT_ONCE(!sName.isEmpty());
    names.push_back(sName);
  }
  return true;
}

TOOLKIT_EXPORT OdString OdAutoFormatVariable(OdDbDatabase* pDb, const OdResBuf* pRb);

// see also VarCmd::format(...)
static OdString RbToString(OdResBufPtr& pRb, OdDbDatabase* pDb, OdSetVarInfo* pSetVarInfo)
{
  OdString sValue;
  if (pRb->restype() != OdResBuf::kRtString)
  {
    if (pSetVarInfo && pSetVarInfo->m_formatFn) 
      sValue = pSetVarInfo->m_formatFn(pDb, pRb);
    else
      sValue = OdAutoFormatVariable(pDb, pRb);
  }
  else
    sValue = pRb->getString();

  return sValue;
}

OdString OdDbSysVarPEImpl::valueToString(OdRxObject* pRxObject, const OdString& csName,
                                         OdSysVarType* pRbType, // = NULL
                                         bool* pbExist) // = NULL
{
  if (pbExist)
    *pbExist = false;
  if (pRbType)
    *pRbType = kRtNone;

  OdString sName = csName;
  sName.makeUpper();

  OdDbDatabasePtr pDb = OdDbDatabase::cast(pRxObject);
  ODA_ASSERT_ONCE(!pDb.isNull() && !sName.isEmpty());
  if (pDb.isNull() || sName.isEmpty())
    return OdString::kEmpty;

  OdRxDictionaryPtr pSysVarDict = odrxSysRegistry()->getAt(OD_T("ODDB_SYSVARDICT"));
  OdSysVarInfo* pSysVarInfo = static_cast<OdSysVarInfo*>(pSysVarDict->getAt(sName).get());
  if (!pSysVarInfo)
    return OdString::kEmpty; // was not found

  OdResBufPtr pRb = pSysVarInfo->m_getFn(pDb); // = pDb->getSysVar(sName);
  ODA_ASSERT_ONCE(!pRb.isNull());
  if (pSysVarInfo->m_mapTypeFn)
    pSysVarInfo->m_mapTypeFn(pDb, pRb, OdSysVarInfo::kToAcadType);

  OdRxDictionaryPtr pSetVarDict = odrxSysRegistry()->getAt(OD_T("ODDB_SETVARDICT"));
  OdSetVarInfo* pSetVarInfo = static_cast<OdSetVarInfo*>(pSetVarDict->getAt(sName).get());

  // see also VarCmd::format(...)
  OdString sValue = RbToString(pRb, pDb, pSetVarInfo);

  if (pRbType)
    *pRbType = (OdSysVarType) pRb->restype();
  if (pbExist)
    *pbExist = true;
  return sValue;
}

bool OdDbSysVarPEImpl::isEditable(OdRxObject* pRxObject, const OdString& csName,
                                  bool* pbExist) // = NULL
{
  if (pbExist)
    *pbExist = false;

  OdString sName = csName;
  sName.makeUpper();

  OdDbDatabasePtr pDb = OdDbDatabase::cast(pRxObject);
  ODA_ASSERT_ONCE(!pDb.isNull() && !sName.isEmpty());
  if (pDb.isNull() || sName.isEmpty())
    return false;

  OdRxDictionaryPtr pSysVarDict = odrxSysRegistry()->getAt(OD_T("ODDB_SYSVARDICT"));
  OdSysVarInfo* pSysVarInfo = static_cast<OdSysVarInfo*>(pSysVarDict->getAt(sName).get());
  if (!pSysVarInfo)
    return false; // was not found

  if (pbExist)
    *pbExist = true;

  if (!pSysVarInfo->m_setFn)
    return false;      

  //OdResBufPtr pRb = pSysVarInfo->m_getFn(pDb); //OdResBufPtr pRb = pDb->getSysVar(sName);
  //if (pSysVarInfo->m_mapTypeFn)
  //  pSysVarInfo->m_mapTypeFn(pDb, pRb, OdSysVarInfo::kToAcadType);
  //if (!pRb->next().isNull())
  //{
  //  // unsupported to edit now but marked in list without read only by ACAD
  //  // (show message via editValue)
  //  return false;
  //}

  return true;
}

bool OdDbSysVarPEImpl::editValue(OdEdCommandContext* pCmdCtx, const OdString& csName, 
                                 bool* pbExist) // = NULL
{
  if (pbExist)
    *pbExist = false;

  OdString sName = csName;
  sName.makeUpper();

  ODA_ASSERT_ONCE(pCmdCtx && !sName.isEmpty());
  if (!pCmdCtx || sName.isEmpty())
    return false;

  OdDbDatabasePtr pDb = OdDbDatabase::cast(pCmdCtx->baseDatabase());
  ODA_ASSERT_ONCE(!pDb.isNull());
  if (pDb.isNull())
    return false;

  OdRxDictionaryPtr pSysVarDict = odrxSysRegistry()->getAt(OD_T("ODDB_SYSVARDICT"));
  OdSysVarInfo* pSysVarInfo = static_cast<OdSysVarInfo*>(pSysVarDict->getAt(sName).get());
  if (!pSysVarInfo)
    return false; // was not found

  if (pbExist)
    *pbExist = true;

  OdResBufPtr pRb = pSysVarInfo->m_getFn(pDb); //OdResBufPtr pRb = pDb->getSysVar(sName);
  ODA_ASSERT_ONCE(!pRb.isNull());
  if (pSysVarInfo->m_mapTypeFn)
    pSysVarInfo->m_mapTypeFn(pDb, pRb, OdSysVarInfo::kToAcadType);

  OdRxDictionaryPtr pSetVarDict = odrxSysRegistry()->getAt(OD_T("ODDB_SETVARDICT"));
  OdSetVarInfo* pSetVarInfo = static_cast<OdSetVarInfo*>(pSetVarDict->getAt(sName).get());

  OdString sValue = RbToString(pRb, pDb, pSetVarInfo);
  enum OdResBuf::ValueType typeRb = (enum OdResBuf::ValueType) pRb->restype();
  if (typeRb == OdResBuf::kRtString)
    sValue = L"\"" + sValue + L"\"";

  OdDbUserIOPtr pIO = OdDbUserIO::cast(pCmdCtx->userIO());
  if (!pIO.get())
  {
    ODA_FAIL_ONCE();
    return false;
  }

  if (!pSysVarInfo->m_setFn || !pRb->next().isNull())
  {
    // show value as read only (ACAD way)
    OdString sMsg;
    sMsg.format(OD_T("%ls = %ls (read only)"), sName.c_str(), sValue.c_str());
    pIO->putString(sMsg);

    return false;
  }

  if (pSetVarInfo && pSetVarInfo->m_promptFn)
  {
    pSetVarInfo->m_promptFn(OdDbCommandContext::cast(pCmdCtx), sName, pRb);
    if (pSysVarInfo->m_mapTypeFn)
      pSysVarInfo->m_mapTypeFn(pDb, pRb, OdSysVarInfo::kToDDType);
    pDb->setSysVar(sName, pRb);

    return true;
  }

  // see also VarNameToSetVarCmdReactor::unknownCommand

  try
  {
    if (!editResBuf(pIO, sName, pRb, typeRb, sValue))
      return false;
    if (pSysVarInfo->m_mapTypeFn)
      pSysVarInfo->m_mapTypeFn(pDb, pRb, OdSysVarInfo::kToDDType);
    pDb->setSysVar(sName, pRb);
    return true;
  }
  catch(const OdEdKeyword&)
  {
  }
  catch(const OdEdEmptyInput&)
  {
  }
  catch(const OdEdCancel&)
  {
  }
  return false;
}

///////////////////////////////////////////////////////////////////////////////

typedef enum { enDb_All, enDb_Dwg, enDb_Dgn, enDb_Tig} OdDatabaseType;

static OdString getFileName(OdRxObject* pRxDatabase, 
                            OdDatabaseType typeDb = enDb_All)
{
  if (   !pRxDatabase 
      || (typeDb == enDb_Dwg && !OdDbDatabase::cast(pRxDatabase).get())
      || (typeDb == enDb_Dgn && OdDbDatabase::cast(pRxDatabase).get())
      || (typeDb == enDb_Tig && (!OdDbDatabase::cast(pRxDatabase).get() || !getIApp()->getExtDbModule(pRxDatabase))))
    return OdString::kEmpty;

  OdString sName = getIConsole()->getFilePathName(pRxDatabase);
  int n = sName.reverseFind('/');
  if (n < 0)
    n = sName.reverseFind('\\');
  if (n >= 0)
    sName = sName.right(sName.getLength() - n - 1);

  return sName;
}

static OdString getFilePrefix(OdRxObject* pRxDatabase, 
                              OdDatabaseType typeDb = enDb_All)
{
  if (   !pRxDatabase 
      || (typeDb == enDb_Dwg && !OdDbDatabase::cast(pRxDatabase).get())
      || (typeDb == enDb_Dgn && OdDbDatabase::cast(pRxDatabase).get()))
    return OdString::kEmpty;

  OdString sPath = getIConsole()->getFilePathName(pRxDatabase);
  int n = sPath.reverseFind('/');
  if (n < 0)
    n = sPath.reverseFind('\\');
  if (n >= 0)
    sPath = sPath.left(n + 1);
  return sPath;
}

static bool getTitled(OdRxObject* pRxDatabase, 
                      OdDatabaseType typeDb = enDb_All)
{
  if (   !pRxDatabase
      || (typeDb == enDb_Dwg && !OdDbDatabase::cast(pRxDatabase).get())
      || (typeDb == enDb_Dgn && OdDbDatabase::cast(pRxDatabase).get()))
    return false;

  OdString sPath = getIConsole()->getFilePathName(pRxDatabase),
           // of file or template or empty(if created  from scratch)
           sPath_ = OdDbBaseDatabasePE::cast(pRxDatabase)->getFilename(pRxDatabase);
  return sPath == sPath_;
}

///////////////////////////////////////////////////////////////////////////////
// used to get united sys variables

class OdqAppSysVarPEImpl : public OdSysVarPE
{
public:
  ODRX_DECLARE_MEMBERS(OdqAppSysVarPEImpl);

  // pRxObject - is OdEdCommandContext
  virtual bool getNames(OdRxObject* pRxObject, OdStringArray& names, bool bSkipHidden = true);
  virtual OdString valueToString(OdRxObject* pRxObject, const OdString& sName, 
                                 OdSysVarType* pRbType = NULL, bool* pbExist = NULL);
  virtual bool isEditable(OdRxObject* pRxObject, const OdString& sName, bool* pbExist = NULL);
  virtual bool editValue(OdEdCommandContext* pCmdCtx, const OdString& sName, bool* pbExist = NULL);
};
//typedef OdSmartPtr<OdqAppSysVarPEImpl> OdqAppSysVarPEImplPtr;

ODRX_NO_CONS_DEFINE_MEMBERS(OdqAppSysVarPEImpl, OdSysVarPE);

static bool lessThan(const OdString& s1, const OdString& s2)
{
  return s1 < s2;
}

bool OdqAppSysVarPEImpl::getNames(OdRxObject* pRxObject, 
                                  OdStringArray& names, 
                                  bool bSkipHidden) // = true
{
  names.clear();
  OdEdCommandContext* pCmdCtx = OdEdCommandContext::cast(pRxObject).get();
  ODA_ASSERT_ONCE(pCmdCtx);
  if (!pCmdCtx)
    return false;
  OdRxObject* pRxDb = pCmdCtx->baseDatabase();
  bool bRes = false;
  if (pRxDb)
  {
    OdSysVarPEPtr pSysVarPE = OdSysVarPE::cast(pRxDb);
    if (!pSysVarPE.isNull())
      bRes = pSysVarPE->getNames(pRxDb, names, bSkipHidden);
  }

  QList<OdString> lstVars = getIAppProps()->subpaths(OD_T("Vars/"), bSkipHidden);
  qSort(lstVars.begin(), lstVars.end(), lessThan);
  OdString sName;
  foreach (sName, lstVars)
  {
    if (sName.isEmpty() || sName.findOneOf(OD_T("/\\")) >= 0)
    {
      ODA_FAIL_ONCE();
      continue;
    }
    sName.makeUpper();
    if (names.contains(sName))
      continue;

    bRes = true;
    names.push_back(sName);
  }

  // --- advanced quasi variables  ---

  #define ODA_QT_VAR_DEF_RO(type, name, value, setValue, hidden) \
    if (!(bSkipHidden && hidden) && !names.contains((sName = OD_T(#name)).makeUpper())) \
      names.push_back(sName);
  #define ODA_QT_VAR_DEF ODA_QT_VAR_DEF_RO

  #include "OdqSysVarDefs.h"

  #undef ODA_QT_VAR_DEF
  #undef ODA_QT_VAR_DEF_RO

  // ---

  return bRes;
}

// --- advanced quasi variables (all read only now) ---

static OdString OdString_toOdString(const OdString& val, OdSysVarType& typeRb)
{
  typeRb = kRtString;
  return val;
}

static OdString OdInt16_toOdString(OdInt16 val, OdSysVarType& typeRb)
{
  typeRb = kRtInt16;
  OdString sValue;
  sValue.format(OD_T("%d"), val);
  return sValue;
}

#define bool_toOdString ODTBOOL_toOdString
static OdString ODTBOOL_toOdString(bool val, OdSysVarType& typeRb)
{
  typeRb = kRtBool;
  return val ? OD_T("true") : OD_T("false");
}

static OdString OdCodePageId_toOdString(OdCodePageId val, OdSysVarType& typeRb)
{
  typeRb = kRtString;
  OdString description;
  if (OdCharMapper::codepageIdToDesc(val, description) != eOk)
  {
    ODA_FAIL_ONCE();
  }
  return description;
}

// TODO  static OdResult OdCharMapper::codepageDescToId(description, val)


typedef OdString (*TQuasiGetFunc)(OdEdCommandContext* pCmdCtx, OdSysVarType& typeRb);
typedef bool (*TQuasiEditFunc)(OdEdCommandContext* pCmdCtx, const OdString& sName, const OdString& sDefVal);

#define ODA_QT_VAR_DEF_RO(type, name, value, setValue, hidden) \
  static OdString get_##name(OdEdCommandContext* pCmdCtx, OdSysVarType& typeRb) \
  { \
    return type##_toOdString(value, typeRb); \
  }
#define ODA_QT_VAR_DEF(type, name, value, edtValue, hidden) \
  ODA_QT_VAR_DEF_RO(type, name, value, edtValue, hidden) \
  static bool edit_##name(OdEdCommandContext* pCmdCtx, \
                          const OdString& sName, const OdString& sDefVal) \
  { \
    return edtValue; \
  }

#include "OdqSysVarDefs.h"

#undef ODA_QT_VAR_DEF
#undef ODA_QT_VAR_DEF_RO

static std::map<OdString, TQuasiGetFunc>& mapQuasiGetFuncs()
{
  static std::map<OdString, TQuasiGetFunc> s_mapGetFuncs;
  if (s_mapGetFuncs.empty())
  {
    #define ODA_QT_VAR_DEF_RO(type, name, value, setValue, hidden) \
      s_mapGetFuncs[OD_T(#name)] = get_##name;
    #define ODA_QT_VAR_DEF ODA_QT_VAR_DEF_RO

    #include "OdqSysVarDefs.h"

    #undef ODA_QT_VAR_DEF
    #undef ODA_QT_VAR_DEF_RO
  }
  return s_mapGetFuncs;
}

// ---

static std::map<OdString, TQuasiEditFunc>& mapQuasiEditFuncs()
{
  static std::map<OdString, TQuasiEditFunc> s_mapEditFuncs;
  if (s_mapEditFuncs.empty())
  {
    #define ODA_QT_VAR_DEF_RO(type, name, value, setValue, hidden) \
      s_mapEditFuncs[OD_T(#name)] = NULL;
    #define ODA_QT_VAR_DEF(type, name, value, setValue, hidden) \
      s_mapEditFuncs[OD_T(#name)] = edit_##name;

    #include "OdqSysVarDefs.h"

    #undef ODA_QT_VAR_DEF
    #undef ODA_QT_VAR_DEF_RO
  }
  return s_mapEditFuncs;
}

// ---

OdString OdqAppSysVarPEImpl::valueToString(OdRxObject* pRxObject, const OdString& csName,
                                           OdSysVarType* pRbType, // = NULL
                                           bool* pbExist) // = NULL
{
  if (pbExist)
    *pbExist = false;

  OdString sName(csName);
  ODA_ASSERT_ONCE(!sName.isEmpty());
  if (sName.isEmpty())
    return OdString::kEmpty;
  OdString sNameLow(sName);
  sNameLow.makeLower();

  OdEdCommandContext* pCmdCtx = OdEdCommandContext::cast(pRxObject).get();
  OdRxObject* pRxDb = pCmdCtx ? pCmdCtx->baseDatabase() : NULL;
  if (   pRxDb
    // workaround for invalid dwgname, dwgprefix and dwgtitled values after commands new and qnew
      && sNameLow != OD_T("dwgname") && sNameLow != OD_T("dwgprefix") && sNameLow != OD_T("dwgtitled"))
  {
    OdSysVarPEPtr pSysVarPE = OdSysVarPE::cast(pRxDb);
    if (!pSysVarPE.isNull())
    {
      bool bExist = false;
      OdString sValue = pSysVarPE->valueToString(pRxDb, sName, pRbType, &bExist);
      if (pbExist)
        *pbExist = bExist;
      if (bExist)
        return sValue;
    }
  }

  OdString sVarPath = OD_T("Vars/");
  sVarPath += sName.makeLower();
  IqAppProps::TPropType type = getIAppProps()->type(sVarPath);
  if (type != IqAppProps::prpNone)
  {
    if (pbExist)
      *pbExist = true;
    if (pRbType)
      *pRbType = (OdSysVarType) type;

    OdString sValue = getIAppProps()->toString(sVarPath);
    //if (   type == IqAppProps::prpString
    //    || type == IqAppProps::prpCombo // ++ IqAppProps::prpLinetype IqAppProps::prpLineweight IqAppProps::prpTransparency
    //    || type == IqAppProps::prpFolder)
    //  sValue = L"\"" + sValue + L"\"";
    return sValue;
  }

  // --- advanced quasi variables (all read only now) ---

  //sName.makeLower();
  std::map<OdString, TQuasiGetFunc>::iterator itr = mapQuasiGetFuncs().find(sName);
  if (itr != mapQuasiGetFuncs().end()) 
  {
    TQuasiGetFunc func = itr->second;
    OdSysVarType typeRb = kRtString;
    OdString sValue = (*func)(pCmdCtx, typeRb);
    //if (typeRb == kRtString)
    //  sValue = L"\"" + sValue + L"\"";

    if (pbExist)
      *pbExist = true;
    if (pRbType)
      *pRbType = (OdSysVarType) typeRb;
    return sValue;
  }

  // ---

  return OdString::kEmpty;
}

bool OdqAppSysVarPEImpl::isEditable(OdRxObject* pRxObject, const OdString& csName,
                                    bool* pbExist) // = NULL
{
  if (pbExist)
    *pbExist = false;

  OdString sName(csName);
  OdEdCommandContext* pCmdCtx = OdEdCommandContext::cast(pRxObject).get();
  ODA_ASSERT_ONCE(pCmdCtx && !sName.isEmpty());
  if (!pCmdCtx || sName.isEmpty())
    return false;

  OdRxObject* pRxDb = pCmdCtx->baseDatabase();
  if (pRxDb)
  {
    OdSysVarPEPtr pSysVarPE = OdSysVarPE::cast(pRxDb);
    if (pSysVarPE.get())
    {
      bool bExist = false;
      bool bRes = pSysVarPE->isEditable(pRxDb, sName, &bExist);
      if (bExist)
      {
        if (pbExist)
          *pbExist = true;
        return bRes;
      }
    }
    if (sName.makeLower() == L"perspective")
    {
      if (pbExist)
        *pbExist = true;
      return true;
    }
  }

  OdString sVarPath = OD_T("Vars/");
  sVarPath += sName.makeLower();
  IqAppProps::TPropType type = getIAppProps()->type(sVarPath);
  if (type != IqAppProps::prpNone)
  {
    if (pbExist)
      *pbExist = true;
    // TODO return getIAppProps()->isReadOnly(sVarPath);
    return true;
  }

  // --- advanced quasi variables (all read only now) ---

  //sName.makeLower();
  std::map<OdString, TQuasiGetFunc>::iterator itr = mapQuasiGetFuncs().find(sName);
  if (itr != mapQuasiGetFuncs().end()) 
  {
    if (pbExist)
      *pbExist = true;
    return false;
  }

  // ---

  return false;
}

bool OdqAppSysVarPEImpl::editValue(OdEdCommandContext* pCmdCtx, const OdString& csName,
                                   bool* pbExist) // = NULL
{
  if (pbExist)
    *pbExist = false;

  OdString sName(csName);
  ODA_ASSERT_ONCE(pCmdCtx && !sName.isEmpty());
  if (!pCmdCtx || sName.isEmpty())
    return false;
  OdString sNameLow(sName);
  sNameLow.makeLower();

  OdRxObject* pRxDb = pCmdCtx->baseDatabase();
  if (   pRxDb
      // workaround for invalid dwgname, dwgprefix and dwgtitled values after commands new and qnew
      && sNameLow != OD_T("dwgname") && sNameLow != OD_T("dwgprefix") && sNameLow != OD_T("dwgtitled"))
  {
    OdSysVarPEPtr pSysVarPE = OdSysVarPE::cast(pRxDb);
    if (pSysVarPE.get())
    {
      bool bExist = false;
      bool bRes = pSysVarPE->editValue(pCmdCtx, sName, &bExist);
      if (bExist)
      {
        if (pbExist)
          *pbExist = true;
        return bRes;
      }
    }
  }

  OdString sVarPath = OD_T("Vars/");
  sVarPath += sName.makeLower();
  IqAppProps::TPropType type = getIAppProps()->type(sVarPath);
  if (type != IqAppProps::prpNone)
  {
    if (pbExist)
      *pbExist = true;

    OdString sPrompt,
             sKeywords,
             sDefValue = getIAppProps()->toString(sVarPath);
    sPrompt.format(OD_T("Enter new value for %ls "), 
                   sName.makeUpper().c_str());
    int options = OdEd::kInpDefault; // 0
    switch (type)
    {
    default:
      ODA_FAIL_ONCE(); // TODO
    case IqAppProps::prpFolder:
      sKeywords += OD_T("dialog");
      sPrompt += OD_T("[dialog]");
    case IqAppProps::prpString:
      options |= OdEd::kGstAllowSpaces | OdEd::kInpThrowEmptyInQuotes;
      break;
    case IqAppProps::prpInt:
    case IqAppProps::prpDouble:
    case IqAppProps::prpDistance:
    case IqAppProps::prpAngle:
      break;
    case IqAppProps::prpBool:
      sKeywords += OD_T("true false invert");
      sPrompt += OD_T("[true/false]");
      break;
    case IqAppProps::prpColorref:
      sKeywords += OD_T("dialog black white darkGray gray lightGray");
      sKeywords += OD_T(" red green blue cyan magenta yellow");
      sKeywords += OD_T(" darkRed darkGreen darkBlue darkCyan darkMagenta darkYellow");
      sKeywords += OD_T(" transparent");
      sPrompt += OD_T("[dialog]");
      break;
    }
    sPrompt += OD_T("<");
    sPrompt += sDefValue;
    sPrompt += OD_T(">");

    OdString sValue;
    try
    {
      sValue = pCmdCtx->userIO()->getString(sPrompt, options, sDefValue, sKeywords);
    }
    catch (const OdEdEmptyInput&)
    {
      ODA_ASSERT_ONCE(   GETBIT(options, OdEd::kInpThrowEmptyInQuotes)
                      && type == IqAppProps::prpString);
    }
    catch (const OdEdKeyword& kw)
    {
      if (type == IqAppProps::prpColorref)
      {
        ODCOLORREF clrDef = getIAppProps()->toColorref(sVarPath);
        QColor colorDef(ODGETRED(clrDef), ODGETGREEN(clrDef), ODGETBLUE(clrDef), 
                        ODGETALPHA(clrDef));
        QColor color = colorDef;
        switch (kw.keywordIndex())
        {
        case 0: // dialog
          color = QColorDialog::getColor(color, (QWidget*) getIApp()->getMainWindow());
          if (!color.isValid())
            return false; // cancel
          break;
        case 1: // black
          color = QColor(Qt::black);
          break;
        case 2: // white
          color = QColor(Qt::white);
          break;
        case 3: // darkGray
          color = QColor(Qt::darkGray);
          break;
        case 4: // gray
          color = QColor(Qt::gray);
          break;
        case 5: // lightGray
          color = QColor(Qt::lightGray);
          break;
        case 6: // red
          color = QColor(Qt::red);
          break;
        case 7: // green
          color = QColor(Qt::green);
          break;
        case 8: // blue
          color = QColor(Qt::blue);
          break;
        case 9: // cyan
          color = QColor(Qt::cyan);
          break;
        case 10: // magenta
          color = QColor(Qt::magenta);
          break;
        case 11: // yellow
          color = QColor(Qt::yellow);
          break;
        case 12: // darkRed
          color = QColor(Qt::darkRed);
          break;
        case 13: // darkGreen
          color = QColor(Qt::darkGreen);
          break;
        case 14: // darkBlue
          color = QColor(Qt::darkBlue);
          break;
        case 15: // darkCyan
          color = QColor(Qt::darkCyan);
          break;
        case 16: // darkMagenta
          color = QColor(Qt::darkMagenta);
          break;
        case 17: // darkYellow
          color = QColor(Qt::darkYellow);
          break;
        case 18: // transparent
          color = QColor(Qt::transparent);
          break;
        default:
          ODA_FAIL_ONCE(); // TODO
          break;
        } // end switch

        if (!color.isValid() || color == colorDef)
          return false;

        ODCOLORREF clr = ODRGBA(color.red(), color.green(), color.blue(), ODGETALPHA(clrDef)); //color.alpha());
        if (!getIAppProps()->set(sVarPath, clr))
        {
          ODA_FAIL_ONCE();
        }

        return true;
      }

      if (type == IqAppProps::prpFolder)
      {
        sValue = getIApp()->toOdString(QFileDialog::getExistingDirectory(
                                       (QWidget*) getIApp()->getMainWindow(), 
                                       QString(), //QObject::tr("%1 : Browse for folder").arg(getIApp()->getAppName()),
                                       getIApp()->toQString(sDefValue)));
        if (!sValue.isEmpty() && sValue != sDefValue)
          getIAppProps()->set(sVarPath, sValue);

        return true;
      }

      sValue = kw.keyword();

      if (type == IqAppProps::prpBool && kw.keywordIndex() == 2)
        sValue = (sDefValue == OD_T("true")) ? OD_T("false") : OD_T("true");
    }
    if (sValue != sDefValue)
    {
      getIAppProps()->set(sVarPath, sValue);
      if (   sVarPath.find(L"snap") >= 0 || sVarPath.find(L"osmode") >= 0
          || sVarPath.find(L"background") >= 0)
      {
        OdString sArg = sVarPath; 
        if (sVarPath.find(L'/') >= 0) 
          sArg = sVarPath.mid(sVarPath.reverseFind(L'/') + 1);
        getIApp()->refreshQObjetcts("<event-refresh:appvar_changed>", getIApp()->toQString(sArg));
      }
    }

    return true;
  }

  // --- advanced quasi variables (all except perspective are read only now) ---

  //sName.makeLower();
  std::map<OdString, TQuasiGetFunc>::iterator itrGet = mapQuasiGetFuncs().find(sName);
  if (itrGet != mapQuasiGetFuncs().end())
  {

    if (pbExist)
      *pbExist = true;

    TQuasiGetFunc funcGet = itrGet->second;
    OdSysVarType typeRb = kRtString;
    OdString sValue = (*funcGet)(pCmdCtx, typeRb);
    if (typeRb == kRtString)
      sValue = L"\"" + sValue + L"\"";

    bool isEditable = false;
    std::map<OdString, TQuasiEditFunc>::iterator itrEdt = mapQuasiEditFuncs().find(sName);
    if (itrEdt != mapQuasiEditFuncs().end() && itrEdt->second)
      isEditable = true;

    OdDbUserIOPtr pIO;
    if (!isEditable || (pIO = OdDbUserIO::cast(pCmdCtx->userIO())).isNull())
    {
      ODA_ASSERT_ONCE(!isEditable);
      OdString sMsg;
      sMsg.format(OD_T("%ls = %ls (read only)"), sName.makeUpper().c_str(), sValue.c_str());
      pCmdCtx->userIO()->putString(sMsg);
      return true;
    }

    TQuasiEditFunc funcEdit = itrEdt->second;
    return (*funcEdit)(pCmdCtx, sName, sValue);
  }

  // ---

  return false;
}

///////////////////////////////////////////////////////////////////////////////
static bool s_bOdSysVarPEinitialized = false;
static bool s_bUnlinkedWithPropServices = false;
static bool s_bUnlinkedWithExAppServices = false;

void initOdSysVarPE()
{
  static OdStaticRxObject<OdDbSysVarPEImpl> s_DbSysVarPE;
  static OdStaticRxObject<OdqAppSysVarPEImpl> s_AppSysVarPE;
  if (s_bOdSysVarPEinitialized)
    return;
  s_bOdSysVarPEinitialized = true;

 #if defined(ODA_SYS_VAR_PE_UNLINKED_RXINIT)
  OdSysVarPE::rxInit();
 #endif
  OdDbSysVarPEImpl::rxInit();
  OdDbDatabase::desc()->addX(OdSysVarPE::desc(), &s_DbSysVarPE);
  OdqAppSysVarPEImpl::rxInit();
  OdEdCommandContext::desc()->addX(OdSysVarPE::desc(), &s_AppSysVarPE);
  ::odrxSysRegistry()->putAt(OD_SYS_VAR_PE, &s_AppSysVarPE);

  OdStringArray names = getIApp()->getExtDbModuleNames();
  for (unsigned int idx = 0, sz = names.size(); idx < sz; idx++)
  {
    const OdString& sModuleName = names[idx];
    OdExtDbModule* pExtDb = OdExtDbModule_cast(::odrxDynamicLinker()->loadModule(sModuleName));
    ODA_ASSERT_ONCE(pExtDb);
    if (!pExtDb)
      continue;
    pExtDb->initOdExtPE();
    pExtDb->setBaseHostAppServices(getIApp()->getBaseHostAppServices());
  }

  OdRxDictionary* pClassDict = ::odrxClassDictionary().get();

  s_bUnlinkedWithPropServices = (!OdPropServices::g_pDesc);
  if (s_bUnlinkedWithPropServices)
  {
    // We have duplicated definition of g_pDesc, desc(), isA() and queryX(...) 
    // via ODRX_NO_CONS_DEFINE_MEMBERS(OdPropServices, OdRxObject);
    // (to prevent impossible linking with OdaQtApp)
    #if !defined(ODA_PROP_SERVICES_UNLINKED_RXINIT)
    ODA_FAIL_ONCE();
    #endif

    // see also newOdRxClass
    OdRxClass* pOdPropServicesClass = OdRxClass::cast(pClassDict->getAt(OD_T("OdPropServices"))).get();
    ODA_ASSERT_ONCE(pOdPropServicesClass);
    OdPropServices::g_pDesc = pOdPropServicesClass;
  }

  s_bUnlinkedWithExAppServices = (!ExAppServices::g_pDesc);
  if (s_bUnlinkedWithExAppServices)
  {
    // We have duplicated definition of g_pDesc, desc(), isA() and queryX(...) 
    // via ODRX_NO_CONS_DEFINE_MEMBERS(ExAppServices, OdRxObject);
    // (to prevent impossible linking with OdaQtApp)
    #if !defined(ODA_EX_APP_SERVICES_UNLINKED_RXINIT)
    ODA_FAIL_ONCE();
    #endif

    // see also newOdRxClass
    OdRxClass* pExAppServicesClass = OdRxClass::cast(pClassDict->getAt(OD_T("ExAppServices"))).get();
    ODA_ASSERT_ONCE(pExAppServicesClass);
    ExAppServices::g_pDesc = pExAppServicesClass;
  }
}

void uninitOdSysVarPE()
{
  if (!s_bOdSysVarPEinitialized) 
    return;
  s_bOdSysVarPEinitialized = false;

  if (s_bUnlinkedWithExAppServices)
    ExAppServices::g_pDesc = NULL;

  if (s_bUnlinkedWithPropServices)
    OdPropServices::g_pDesc = NULL;

  OdStringArray names = getIApp()->getExtDbModuleNames();
  for (unsigned int idx = 0, sz = names.size(); idx < sz; idx++)
  {
    const OdString& sModuleName = names[idx];
    OdRxModulePtr pModule = ::odrxDynamicLinker()->getModule(sModuleName);
    OdExtDbModule* pExtDb = OdExtDbModule_cast(pModule.get());
    ODA_ASSERT_ONCE(pExtDb || pModule.isNull());
    if (!pExtDb)
      continue;
    pExtDb->setBaseHostAppServices(NULL);
    pExtDb->uninitOdExtPE();
  }

  ::odrxSysRegistry()->remove(OD_SYS_VAR_PE);
  OdEdCommandContext::desc()->delX(OdSysVarPE::desc());
  OdqAppSysVarPEImpl::rxUninit();
  OdDbDatabase::desc()->delX(OdSysVarPE::desc());
  OdDbSysVarPEImpl::rxUninit();
 #if defined(ODA_SYS_VAR_PE_UNLINKED_RXINIT)
  OdSysVarPE::rxUninit();
 #endif
}
