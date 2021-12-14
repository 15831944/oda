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
/* Implements notification functions for the evaluation of OdDbField    */ 
/* objects                                                              */
/************************************************************************/
#include "StdAfx.h"
#include "OdaObjProp.h"
#include <RxObjectImpl.h>
#include <DbHostAppServices.h>
#include <Ge/GePoint3d.h>
#include <DbAttribute.h>
#include <DbDictionary.h>
#include <DbBlockReference.h>
#include <DynamicBlocks/DbBlockRepresentation.h>
#include <DynamicBlocks/DbBlockParameter.h>
#include <DbDynBlockReference.h>
#include <DbTransactionWrapper.h>
#include <RxProperty.h>
#include <RxValue.h>
#include <RxAttribute.h>
#if defined(_TOOLKIT_IN_DLL_) && defined(_MSC_VER) && !defined(_WINRT)
#include <odaxwrap.h>
#endif

OdDbDatabase* database(OdDbField* pField);
bool hasDecimals(OdValue::DataType type);
void fixPointCoords(OdString& format, OdValue::DataType type);
void fixPrecision(OdString& format, OdDbDatabase* pDb);

ODRX_CONS_DEFINE_MEMBERS(OdObjPropEvaluator,OdEvaluatorBase,RXIMPL_CONSTR);

OdObjPropEvaluator::OdObjPropEvaluator() : OdEvaluatorBase(OD_T("AcObjProp")) {}

const OdString OdObjPropEvaluator::evaluatorId(OdDbField* pField) const 
{
  if (!pField) 
    return OdString::kEmpty;
  OdString code = pField->getFieldCode(OdDbField::kFieldCode);
  if (code.find(OD_T("\\AcObjProp ")) != -1)
    return OD_T("AcObjProp");
  if (code.find(OD_T("\\AcObjProp.16.2 ")) != -1)
    return OD_T("AcObjProp.16.2");
  return OdString::kEmpty;
}

/**
\details
Parses block placeholder ObjProp field code retrieving subparts of the code string.

\param code         [in]   code string, see possible variants below
\param name         [out]  property name, the last substring
\param option       [out]  option after "?BlockRefId,"
\param parameterId  [out]  value of "Parameter" attribute

\remarks
Variants of code string: par(nnn).xx ,1).
Object(?BlockRefId).XXX                 - 'Display value for block reference' is off
Object(?BlockRefId,1).XXX               - 'Display value for block reference' is on
Object(?BlockRefId).Parameter(NNN).XXX  - custom props of dynblocks

Returns
'true' on success,
'false' if the code string is not a valid block placeholder field code
*/
static bool parseBlockPlaceholder(const OdString& code, OdString& name, OdString& option, OdString& parameterId)
{
  const OdString BEGIN = OD_T("Object(?BlockRefId");
  const int BEGIN_LENGTH = BEGIN.getLength();
  if (code.left(BEGIN_LENGTH) != BEGIN)
    return false;
  OdString remaining = code.mid(BEGIN_LENGTH);
  int pos = remaining.find(')');
  if (pos == -1)
    return false;
  if (pos > 0)
  {
    if (remaining[0] != ',')
      return false;
    option = remaining.mid(1, pos - 1);
  }
  if (remaining[++pos] != '.')
    return false;
  remaining = remaining.mid(++pos);

  const OdString ParameterSTR = OD_T("Parameter(");
  const int ParameterSTR_LENGTH = ParameterSTR.getLength();
  if (remaining.left(ParameterSTR_LENGTH) == ParameterSTR)
  {
    pos = remaining.find(')', ParameterSTR_LENGTH);
    if (pos == -1)
      return false;
    parameterId = remaining.mid(ParameterSTR_LENGTH, pos - ParameterSTR_LENGTH);
    if (remaining[++pos] != '.')
      return false;
    remaining = remaining.mid(++pos);
  }
  name = remaining;
  return true;
}

#ifdef _WINDOWS
#include "Windows.h"
#endif

static OdDbObjectId parseObjId(const OdString& code)
{
  OdDbObjectId res;
  int idStart = code.find(L"_ObjId");
  if (idStart == -1)
    return res;
  idStart += 6;
  int idEnd = code.find(L">%", idStart);
  if (idEnd == -1)
    return res;
  OdString s = code.mid(idStart, idEnd - idStart);
  s.trimLeft();
  s.trimRight();

  OdUInt64 iVal;
  odSScanf(s.c_str(), L"%" PRIu64W L"\"", &iVal);
  res = (OdDbStub*)iVal;

#ifdef _WINDOWS
  // Pointer validation 
  int size = sizeof(void *);
  bool b = IsBadReadPtr((VOID *)res, 8 + size * 3);
  if (b)
    return OdDbObjectId();
#endif

  return res;
}

OdResult OdObjPropEvaluator::compile(OdDbField* pField, OdDbDatabase* pDb, OdFdFieldResult* pResult) const
{
  OdString code = pField->getFieldCode(OdDbField::kStripOptions);
  OdString name, option, parameterId;
  if (parseBlockPlaceholder(code, name, option, parameterId))
  {
    pField->setData(OD_T("ObjectPropertyUnresolvedId"), OdFieldValue(OD_T("?BlockRefId")));
    if (!name.isEmpty())
      pField->setData(OD_T("ObjectPropertyName"), OdFieldValue(name));
    if (!parameterId.isEmpty())
      pField->setData(OD_T("ObjectPropertyParameterId"), OdFieldValue(odStrToInt(parameterId)));
    if (!option.isEmpty())
    {
      pField->setData(OD_T("ObjectPropertyOptionString"), OdFieldValue(option));
      pField->setData(OD_T("ObjectPropertyOption"), OdFieldValue(odStrToInt(option)));
    }
    pResult->setEvaluationStatus(OdDbField::kSuccess);
    return eOk;
  }
  OdFdFieldEnginePEPtr pFieldEnginePE = OdFdFieldEngine::desc()->getX(OdFdFieldEnginePE::desc());
  if (!pFieldEnginePE.isNull())
  {
    OdString propName;
    OdDbObjectId id;
    if (!pFieldEnginePE->parseObjProp(code, pDb, id, propName))
      return eInvalidInput;
    if ( !id.isNull() )
      pField->setData(OD_T("ObjectPropertyId"), OdFieldValue(id));
    if ( !propName.isEmpty() )
      pField->setData(OD_T("ObjectPropertyName"), OdFieldValue(propName));  
  }
  else
  {
    int propNameStart = code.reverseFind('.');
    if (propNameStart != -1)
    {
      OdFieldValue propName = pField->getData(OD_T("ObjectPropertyName"));
      if (!propName.isValid())
      {
        pField->setData(OD_T("ObjectPropertyName"), OdFieldValue(code.mid(propNameStart + 1)));
      }
      OdDbObjectId id = parseObjId(code);
      if (!id.isNull() && !pField->getData(L"ObjectPropertyId").isValid())
        pField->setData(L"ObjectPropertyId", id);
    }
    else
    {
      pResult->setEvaluationStatus(OdDbField::kSyntaxError, 0, OD_T("Property name not found"));
      return eInvalidInput;
    }
  }
  int pos = code.find(OD_T(">%,"));
  if (pos != -1)
  {
    pos += 3;
    int pos2 = code.find(')', pos);
    if (pos2 != -1)
    {
      OdString option = code.mid(pos, pos2 - pos);
      if (!option.isEmpty())
      {
        pField->setData(OD_T("ObjectPropertyOptionString"), OdFieldValue(option));
        pField->setData(OD_T("ObjectPropertyOption"), OdFieldValue(odStrToInt(option)));
      }
    }
  }
  pResult->setEvaluationStatus(OdDbField::kSuccess);
  return eOk;
}

OdResult OdObjPropEvaluator::format(OdDbField* pField, OdString& pszValue) const
{
  OdFieldValue fv; 
  OdResult res = pField->getValue(fv);
  if (res != eOk) return res;
  OdString format = pField->getFormat();
  fixPointCoords(format, fv.dataType());
  // OdValue can't handle specifiers that need the database
  OdDbDatabase* pDb = database(pField);
  if (pDb && hasDecimals(fv.dataType()))
    fixPrecision(format, pDb);
  if (fv.format(format, pszValue, pDb))
    return eOk;
  return eNotImplemented;
}

#if defined(_TOOLKIT_IN_DLL_) && defined(_MSC_VER) && (_MSC_VER > 1310) && !defined(_WINRT)
static OdFieldValue getPropValue(MEMBERID memid, IDispatch* pDisp)
{
  unsigned int  uArgErr;
  EXCEPINFO     except;
  _variant_t    varRes;
  DISPPARAMS    param = { NULL, NULL, 0, 0 };
  OdFieldValue fv;
  HRESULT hr = pDisp->Invoke(memid, IID_NULL, ::GetSystemDefaultLCID(),
    DISPATCH_PROPERTYGET, &param, &varRes, &except, &uArgErr);
  if (!SUCCEEDED(hr))
    return fv;
  switch (varRes.vt)
  {
  case VT_DISPATCH:
  case VT_UNKNOWN:
  {
    IAcadAcCmColor* pColor = 0;
    if (SUCCEEDED(varRes.pdispVal->QueryInterface(&pColor)))
    {
      long ec;
      pColor->get_EntityColor(&ec);
      OdCmColor c;
      c.setColor(ec);
      fv = c.colorNameForDisplay();
      pColor->Release();
    }
    // what other types are possible?
  }
  break;
  case (VT_ARRAY | VT_R8): // treat as point
  {
    LONG index = 0;
    double x, y, z;
    SafeArrayGetElement(varRes.parray, &index, &x);
    index = 1;
    SafeArrayGetElement(varRes.parray, &index, &y);
    index = 2;
    if (S_OK == SafeArrayGetElement(varRes.parray, &index, &z))
      fv = OdFieldValue(x, y, z);
    else
      fv = OdFieldValue(x, y);
  }
  break;
  case VT_BOOL:
    fv = OdInt32(VARIANT_TRUE == varRes.boolVal);
    break;
  case VT_I2:
  case VT_UI2:
    fv = OdInt32(varRes.iVal);
    break;
  case VT_I4:
  case VT_UI4:
    fv = OdInt32(varRes.lVal);
    break;
  case VT_R8:
    fv = varRes.dblVal;
    break;
  case VT_BSTR:
    fv = OdString(varRes.bstrVal);
    break;
  default:
    break;
  }
  return fv;
}

bool oddbGetObjectProperty(OdDbObjectId id, const OdString& propName, OdFieldValue& fv)
{
  HRESULT hr = S_OK;
#if _WIN32_WINNT >= 0x400
  if ((hr = ::CoInitializeEx(0, COINIT_APARTMENTTHREADED)) == RPC_E_CHANGED_MODE)
    hr = ::CoInitializeEx(0, COINIT_MULTITHREADED);
#else
  if (HMODULE hm = ::LoadLibrary(OD_T("ole32.dll")))
  {
    typedef HRESULT(PASCAL* PCoInitializeEx)(LPVOID, DWORD);
    if (PCoInitializeEx pf = (PCoInitializeEx)::GetProcAddress(hm, "CoInitializeEx"))
    {
      if (pf(0, COINIT_APARTMENTTHREADED) == RPC_E_CHANGED_MODE)
        pf(0, 0);
    }
    ::FreeLibrary(hm);
  }
#endif

  if (FAILED(hr))
    return false;
  bool bFound = false;
  try
  {
    // weird construction to take care of the reference counter
    // we pass NULL for application, because this sample is application independent 
    // (also, properties should be read w/o application access)
    IDispatchPtr pDisp = IUnknownPtr(::OdOxGetIUnknownOfObject(id, NULL), false);
    ITypeInfoPtr pTi;
    pDisp->GetTypeInfo(0, ::GetSystemDefaultLCID(), &pTi);
    FUNCDESC* pFD;
    UINT i = 0;
    while (!bFound && SUCCEEDED(pTi->GetFuncDesc(i, &pFD)))
    {
      if (pFD->invkind == INVOKE_PROPERTYGET)
      {
        _bstr_t res;
        hr = pTi->GetDocumentation(pFD->memid, res.GetAddress(), NULL, NULL, NULL);
        if (SUCCEEDED(hr))
        {
          if (propName.iCompare((const wchar_t*)res) == 0)
          {
            fv = getPropValue(pFD->memid, pDisp);
            bFound = true;
          }
        }
      }
      i++;
      pTi->ReleaseFuncDesc(pFD);
    }
  }
  catch (const _com_error&)
  {
    bFound = false;
  }
  catch (...)
  {
    ::CoUninitialize();
    throw;
  }
  ::CoUninitialize();
  return bFound;
}
#else
bool oddbGetObjectProperty(OdDbObjectId, const OdString& , OdFieldValue&)
{
  return false;
}
#endif

bool oddbToFieldValue(const OdRxValue& val, OdFieldValue& res)
{
  if (const bool* v = rxvalue_cast<bool>(&val))
  {
    res = (OdInt32)*v;
  }
  else if (const short* v = rxvalue_cast<short>(&val))
  {
    res = (OdInt32)*v;
  }
  else if (const unsigned short* v = rxvalue_cast<unsigned short>(&val))
  {
    res = (OdInt32)*v;
  }
  else if (const int* v = rxvalue_cast<int>(&val))
  {
    res = (OdInt32)*v;
  }
  else if (const unsigned int* v = rxvalue_cast<unsigned int>(&val))
  {
    res = (OdInt32)*v;
  }
  else if (const long* v = rxvalue_cast<long>(&val))
  {
    res = sizeof(int) == sizeof(long) ? OdFieldValue((OdInt32)*v) : OdFieldValue((OdInt64)*v);
  }
  else if (const unsigned long* v = rxvalue_cast<unsigned long>(&val))
  {
    res = sizeof(int) == sizeof(long) ? OdFieldValue((OdInt32)*v) : OdFieldValue((OdInt64)*v);
  }
  else if (const long long* v = rxvalue_cast<long long>(&val))
  {
    res = (OdInt64)*v;
  }
  else if (const unsigned long long* v = rxvalue_cast<unsigned long long>(&val))
  {
    res = (OdInt64)*v;
  }
  else if (const double* v = rxvalue_cast<double>(&val))
  {
    res = *v;
  }
  else if (const OdChar* const * v = rxvalue_cast<const OdChar*>(&val))
  {
    res = OdString(*v);
  }
  else if (const OdString* v = rxvalue_cast<OdString>(&val))
  {
    res = *v;
  }
  else if (const OdGePoint3d* v = rxvalue_cast<OdGePoint3d>(&val))
  {
    res = *v;
  }
  else if (const OdGeVector3d* v = rxvalue_cast<OdGeVector3d>(&val))
  {
    res = *v;
  }
  else 
    return false;
  return true;
}

 bool oddbGetObjectPropertyRx(OdDbObjectId id, const OdString& propName, OdFieldValue& res)
{
  OdDbObjectPtr pObj = id.openObject();
  if (pObj.isNull())
    return false;
  ::odrxDynamicLinker()->loadModule(DbPropertiesModuleName);
  OdRxPropertyPtr m;
  
  for (OdRxMemberIteratorPtr it = OdRxMemberQueryEngine::theEngine()->newMemberIterator(pObj); !it->done(); it->next())
  {
    if (OdRxCOMAttribute* ca = (OdRxCOMAttribute*)it->current()->attributes().get(OdRxCOMAttribute::desc()))
    {
      if (ca->name() == propName)
      {
        m = it->current();
        break;
      }
    }
    if (it->current()->name() == propName)
    {
      m = it->current();
      break;
    }
  }
  if (m.isNull())
    return false;
  OdRxValue val;
  if (m->getValue(pObj, val) != eOk)
    return false;
  if (const OdDbObjectId* v = rxvalue_cast<OdDbObjectId>(&val))
  {
    if (OdRxDisplayAsAttribute* dva = (OdRxDisplayAsAttribute*)m->attributes().get(OdRxDisplayAsAttribute::desc()))
    {
      res = dva->getDisplayValue(val);
      return true;
    }
    else
      return false;
  }
  else 
    return oddbToFieldValue(val, res);
}

static inline OdResult invalidInput(OdFdFieldResult* pResult)
{
  pResult->setEvaluationStatus(OdDbField::kInvalidContext);
  return eInvalidInput;
}

static OdDbObjectId isFieldInAttribute(OdDbObject* pField)
{
  OdDbObjectPtr pOwner = pField->ownerId().openObject();
  if (pOwner.isNull())
    return OdDbObjectId::kNull;
  if (pOwner->isKindOf(OdDbField::desc()) || pOwner->isKindOf(OdDbDictionary::desc()))
    return isFieldInAttribute(pOwner);
  if (pOwner->isKindOf(OdDbAttribute::desc()))
    return pOwner->ownerId();
  else
    return OdDbObjectId::kNull;
}

static OdDbBlockReferencePtr getOwnerBlockReference(OdDbObject* pObj)
{
  OdDbObjectPtr owner = pObj->ownerId().openObject();
  if (owner.isNull())
    return OdDbBlockReferencePtr();
  if (owner->isKindOf(OdDbBlockReference::desc()))
    return OdDbBlockReferencePtr(owner);
  return getOwnerBlockReference(owner);
}


static bool tryGetBlockAttribute(OdDbObjectId& id, OdString propName, OdFieldValue& fv)
{
  OdDbBlockReferencePtr br = OdDbBlockReference::cast(id.openObject());
  if (!br.isNull()) {
    for (OdDbObjectIteratorPtr ai = br->attributeIterator(); !ai->done(); ai->step())
    {
      OdDbAttributePtr a = OdDbAttribute::cast(ai->objectId().openObject());
      if (a.isNull())
        continue;
      if (a->tag() == propName) {
        fv = a->textString();
        return true;
      }
    }
  }
  return false;
}

OdResult OdObjPropEvaluator::evaluate(OdDbField* pField, int nContext, OdDbDatabase* pDb, OdFdFieldResult* pResult) const
{
  OdFieldValue objId = pField->getData(OD_T("ObjectPropertyId"));
  OdFieldValue propNameValue = pField->getData(OD_T("ObjectPropertyName"));
  if (!propNameValue.isValid())
    return invalidInput(pResult);

  if (!objId.isValid() || !((OdDbObjectId)objId).isValid() || ((OdDbObjectId)objId).isErased())
  {
    OdFieldValue blockRefIdValue = pField->getData(OD_T("ObjectPropertyUnresolvedId"));
    if (blockRefIdValue.isValid())
    {
      if (OdString(blockRefIdValue) != OD_T("?BlockRefId"))
        return invalidInput(pResult);
      OdFieldValuePtr fValue = OdFieldValue::createObject();
      fValue->set(propNameValue);
      pResult->setFieldValue(fValue);
      pResult->setEvaluationStatus(OdDbField::kSuccess);
      return eOk;
    }
    return invalidInput(pResult);
  }
  OdDbBlockReferencePtr bref = getOwnerBlockReference(pField);
  if (!bref.isNull() && (OdDbObjectId)objId != bref->objectId())
  {
    OdDbDynBlockReference dynBref(bref->objectId());
    if (OdDbBlockRepresentationContext* ctx = dynBref.getRepresentationContext())
    {
      OdDbObjectIdArray ia;
      ia.append((OdDbObjectId)objId);
      OdDbEntityPtrArray repEnts;
      if (ctx->originalPathToRepresentationPath(ia, repEnts) && repEnts.size() > 0 && repEnts[0]->objectId().isValid())
        objId.set(repEnts[0]->objectId());
    }
  }
  bool bFound = false;
  OdDbTransactionWrapper tw(OdDbObjectId(objId).database());
  OdFieldValue option = pField->getData(OD_T("ObjectPropertyOption"));
  if (option.isValid() && OdInt32(option) > 0)
  {
    OdDbObjectId brId = isFieldInAttribute(pField);
    if (brId.isValid())
    {
      OdDbBlockReferencePtr br = OdDbBlockReference::cast(brId.openObject());
      if (!br.isNull())
      {
        OdDbEntityPtr pObj = OdDbEntity::cast(((OdDbObjectId)objId).openObject(OdDb::kForWrite));
        if (!pObj.isNull() && pObj->ownerId() == br->blockTableRecord())
        {
          pObj->transformBy(br->blockTransform());
        }
      }
    }
  }
  OdFieldValue propParamId = pField->getData(OD_T("ObjectPropertyParameterId"));
  OdInt32 paramId = propParamId.isValid() ? OdInt32(propParamId) : 0;
  OdFdFieldEnginePEPtr pFieldEnginePE = OdFdFieldEngine::desc()->getX(OdFdFieldEnginePE::desc());
  if (pFieldEnginePE.isNull())
  {
    OdFieldValue fv;
    if (paramId != 0 && !bref.isNull())
    {
      OdDbDynBlockReference dynBref(bref->objectId());
      if (dynBref.isDynamicBlock())
      {
        OdDbDynBlockReferencePropertyArray dynProps; dynBref.getBlockProperties(dynProps);
        for (unsigned int i = 0; i < dynProps.size(); ++i)
        {
          OdDbBlockParameterPtr pParam = dynProps[i]->getParameter();
          if (!pParam.isNull() && pParam->nodeId() == paramId)
          {
            bFound = true;
            fv.set(pParam->getPropertyValue(OdString(propNameValue), bref->blockTransform()));
            break;
          }
        }
      }
    }
    if (!bFound) {
      OdDbObjectId id(objId);
      OdString propName(propNameValue);
      bFound = oddbGetObjectPropertyRx(id, propName, fv) || tryGetBlockAttribute(id, propName, fv) || oddbGetObjectProperty(id, propName, fv);
    }

    if (bFound)
    {
      pResult->setFieldValue(fv.isValid() ? &fv : 0);
      pResult->setEvaluationStatus(OdDbField::kSuccess);
    }
    else
    {
      pResult->setFieldValue(0);
      pResult->setEvaluationStatus(OdDbField::kOtherError);
      return eNotImplemented;
    }
  }
  else
  {
    bFound = pFieldEnginePE->getObjPropValue((OdString)propNameValue, paramId, (OdDbObjectId)objId, *pResult);
  }
  return bFound ? eOk : eInvalidInput;
}
