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
#include "ExFieldEvaluator.h"
#include "RxObjectImpl.h"
#include "DbField.h"
#include "OdTimeStamp.h"
#include "DbDatabase.h"
#include "summinfo.h"
#include "DbLayout.h"
#include "DbHyperlink.h"
#include "OdPlatformSettings.h"
#include "SysVarInfo.h"
#include "DbFiler.h"
#include "OdaObjProp.h"
#include "OdExprEval.h"
#include "DbHostAppServices.h"

bool compileHref(OdDbField* pField, OdFdFieldResult* pResult)
{
  OdString s = pField->getFieldCode(OdDbField::kFieldCode);
  int hrefStart = s.find(OD_T("\\href"));
  if (hrefStart != -1)
  {
    bool bIncorrectSyntax = true;
    int nPar = s.find('\"', hrefStart);
    if (nPar != -1)
    {
      s = s.mid(nPar + 1, s.getLength() - nPar - 1);
      nPar = s.reverseFind('\"');
      if (nPar != -1)
      {
        s = s.left(nPar);
        int hash1 = s.find('#');
        if (hash1 != -1)
        {
          OdString name = s.left(hash1);
          int hash2 = s.find('#', hash1 + 1);
          if (hash2 != -1)
          {
            OdString subLocation = s.mid(hash1 + 1, hash2 - hash1 - 1);
            int hash3 = s.find('#', hash2 + 1);
            if (hash3 != -1)
            {
              OdString displayName = s.mid(hash2 + 1, hash3 - hash2 - 1);
              OdInt32 flags = odStrToInt(s.mid(hash3 + 1));
              bIncorrectSyntax = false;
              OdDbEntityHyperlinkPEPtr hpe = OdDbEntityHyperlinkPE::cast(pField);
              OdDbHyperlinkCollectionPtr hColl = hpe->getHyperlinkCollection(pField);
              while (hColl->count())
                hColl->removeTail();
              hColl->addTail(name, displayName, subLocation);
              hColl->item(0)->setFlags(flags);
              hpe->setHyperlinkCollection(pField, hColl);
            }
          }
        }
      }
    }
    if (bIncorrectSyntax)
    {
      pResult->setEvaluationStatus(OdDbField::kSyntaxError, 0, OD_T("Hyperlink field must be of the form \"\\AcVar \\href \"link#view#displayname#flags\"\""));
      return false;
    }
  }
  return true;
}


ODRX_NO_CONS_DEFINE_MEMBERS(OdEvaluatorBase,OdFdFieldEvaluator);

OdEvaluatorBase::OdEvaluatorBase(const OdString& id) : m_sId(id)
{
}

const OdString OdEvaluatorBase::evaluatorId() const { return m_sId; }

OdResult OdEvaluatorBase::initialize(OdDbField*) const
{
  return eOk;
}

OdResult OdEvaluatorBase::compile(OdDbField*, OdDbDatabase*, OdFdFieldResult* pResult) const
{
  pResult->setEvaluationStatus(OdDbField::kSuccess);
  return eOk;
}

ODRX_CONS_DEFINE_MEMBERS(OdTextEvaluator,OdEvaluatorBase,RXIMPL_CONSTR);

OdTextEvaluator::OdTextEvaluator() : OdEvaluatorBase(OD_T("_text")) {}

const OdString OdTextEvaluator::evaluatorId(OdDbField* pField) const 
{
  if (!pField) 
    return OdString::kEmpty;
  if (pField->getFieldCode(OdDbField::kFieldCode).find(OD_T("\\FldIdx ")) != -1)
    return OD_T("_text");
  return OdString::kEmpty;
}

OdResult OdTextEvaluator::evaluate(OdDbField* pField, int, OdDbDatabase*, OdFdFieldResult* pResult) const
{
  OdFieldValue v(pField->getFieldCode(OdDbField::kEvaluatedText));
  pResult->setFieldValue(&v);
  pResult->setEvaluationStatus(OdDbField::kSuccess);
  return eOk;
}

OdResult OdTextEvaluator::format(OdDbField* pField, OdString& pszValue) const
{
  pszValue = pField->getFieldCode(OdDbField::kEvaluatedText);
  return eOk;
}


ODRX_CONS_DEFINE_MEMBERS(OdVarEvaluator,OdEvaluatorBase,RXIMPL_CONSTR);

OdVarEvaluator::OdVarEvaluator() : OdEvaluatorBase(OD_T("AcVar")) {}

const OdString OdVarEvaluator::evaluatorId(OdDbField* pField) const 
{
  if (!pField) 
    return OdString::kEmpty;
  OdString code = pField->getFieldCode(OdDbField::kFieldCode);
  if (code.find(OD_T("\\AcVar ")) != -1)
    return OD_T("AcVar");
  else if (code.find(OD_T("\\AcVar.16.2 ")) != -1)
      return OD_T("AcVar.16.2");
  else if (code.find(OD_T("\\AcVar.17.0 ")) != -1)
    return OD_T("AcVar.17.0");
  return OdString::kEmpty;
}

OdResult OdVarEvaluator::compile(OdDbField* pField, OdDbDatabase* /*pDb*/, OdFdFieldResult* pResult) const
{
  // Set variable name, for later use
  OdString varName = pField->getFieldCode(OdDbField::kStripOptions);
  OdFieldValue var(varName);
  pField->setData(OD_T("Variable"), var);
  
  // variable name may be missing
  if (varName.isEmpty())
  {
    if (!compileHref(pField, pResult))
      return eInvalidInput;
  }
  pResult->setEvaluationStatus(OdDbField::kSuccess);
  return eOk;
}

TOOLKIT_EXPORT OdString OdAutoFormatVariable(OdDbDatabase* pDb, const OdResBuf* pRb);

static OdString displayNameFromHref(const OdString& href)
{
  int hash1, hash2, hash3;

  if (((hash1 = href.find('#')) != -1) &&
    ((hash2 = href.find('#', hash1 + 1)) != -1) &&
    ((hash3 = href.find('#', hash2 + 1)) != -1))
  {
    return href.mid(hash2 + 1, hash3 - hash2 - 1);
  }
  return OdString::kEmpty;
}

static OdString getFileName(OdDbDatabase* pDb)
{
  if(pDb)
  {
    OdFdFieldEnginePEPtr pFieldEnginePE = OdFdFieldEngine::desc()->getX(OdFdFieldEnginePE::desc());
    return pFieldEnginePE.isNull() ? pDb->getFilename() : pFieldEnginePE->getFileName(pDb);
  }
  return OdString::kEmpty;
}

OdDbDatabase* database(OdDbField* pField)
{
  if (pField && pField->database())
    return pField->database();
  OdFdFieldEnginePEPtr pFieldEnginePE = OdFdFieldEngine::desc()->getX(OdFdFieldEnginePE::desc());
  return pFieldEnginePE.isNull() ? NULL : pFieldEnginePE->getCurrentDb();
}

bool hasDecimals(OdValue::DataType type)
{
  return (type == OdFieldValue::kDouble || type == OdFieldValue::kPoint || type == OdFieldValue::k3dPoint);
}

void fixPointCoords(OdString& format, OdValue::DataType type)
{
  // HACK: when "%pt" format is omitted, only Y coord is printed (should be X,Y for 2dpt or X,Y,Z for 3dpt)
  // (cfr. OdValueImpl::formatPoint)
  if (type == OdFieldValue::kPoint)
  {
    if (format.find(OD_T("%pt")) < 0)
      format += OD_T("%pt3");  // kXIncluded and kYIncluded (0x01|0x02)
  }
  else if (type == OdFieldValue::k3dPoint)
  {
    if (format.find(OD_T("%pt")) < 0)
      format += OD_T("%pt7");  // kXIncluded, kYIncluded and kZIncluded (0x01|0x02|0x04)
  }
}

void fixPrecision(OdString& format, OdDbDatabase* pDb)
{
  if (format.find(OD_T("%sn")) >= 0)
    return;

  if (format.find(OD_T("%lu6")) >= 0)
  {
    // "%lu6" means "use current units (LUNITS from current database)"
    OdString lunits;
    lunits.format(OD_T("%%lu%d"), pDb->getLUNITS());
    format.replace(OD_T("%lu6"), lunits.c_str());
  }
  else if (format.find(OD_T("%au5")) >= 0)
  {
    // "%au5" means "use current units (AUNITS from current database)"
    OdString aunits;
    aunits.format(OD_T("%%au%d"), pDb->getAUNITS());
    format.replace(OD_T("%au5"), aunits.c_str());
  }

  if (format.find(OD_T("%pr")) < 0)  // no fixed precision specified
  {
    OdString prec;
    int units_place = format.find(OD_T("%lu"));
    if (units_place >= 0)        // use current precision
      prec.format(OD_T("%%pr%d"), pDb->getLUPREC());
    else if ((units_place = format.find(OD_T("%au"))) >= 0)   // use current precision
      prec.format(OD_T("%%pr%d"), pDb->getAUPREC());
    else
      prec = OD_T("%lu2%pr6"); // to support additional formats (%ct, %ps, %ds, %th)
    if (units_place >= 0)
      format.insert(units_place + 4, prec);
    else
      format += prec;
  }
}

OdResult OdVarEvaluator::evaluate(OdDbField* pField, int nContext, OdDbDatabase* pDb, OdFdFieldResult* pResult) const
{
  OdFieldValue varV = pField->getData(OD_T("Variable"));
  if (!varV.isValid())
  {
    pResult->setEvaluationStatus(OdDbField::kOtherError);
    return eInvalidInput;
  }
  OdString var = varV;
  OdFieldValue v;
  bool bEvaluated = false;
  if (var.isEmpty()) // may be href
  {
    OdString link;
    OdDbHyperlinkCollectionPtr hlc = OdDbEntityHyperlinkPE::cast(pField)->getHyperlinkCollection(pField);
    if (!hlc.isNull() && hlc->count() != 0)
      link = hlc->item(0)->getDisplayString();
    v.set(link);
  }
  else if (var.left(6).iCompare(OD_T("\\href ")) == 0)
  {
    v.set(displayNameFromHref(var));
  }
  if (var.iCompare(OD_T("Date")) == 0)
  {
    v.set((OdInt64)OdTimeStamp(OdTimeStamp::kInitUniversalTime).packedValue());
  }
  else if (var.iCompare(OD_T("PlotDate")) == 0)
  {
    if ((nContext & OdDbField::kPlot) != 0)
    {
      v.set((OdInt64)OdTimeStamp(OdTimeStamp::kInitUniversalTime).packedValue());
    }
    bEvaluated = true;
  }
  else if (var.iCompare(OD_T("Author")) == 0)
  {
    v.set(oddbGetSummaryInfo(pDb)->getAuthor());
  }
  else if (var.iCompare(OD_T("Comments")) == 0)
  {
    v.set(oddbGetSummaryInfo(pDb)->getComments());
  }
  else if (var.iCompare(OD_T("HyperlinkBase")) == 0)
  {
    v.set(oddbGetSummaryInfo(pDb)->getHyperlinkBase());
  }
  else if (var.iCompare(OD_T("Keywords")) == 0)
  {
    v.set(oddbGetSummaryInfo(pDb)->getKeywords());
  }
  else if (var.iCompare(OD_T("LastSavedBy")) == 0)
  {
    v.set(oddbGetSummaryInfo(pDb)->getLastSavedBy());
  }
  else if (var.iCompare(OD_T("Subject")) == 0)
  {
    v.set(oddbGetSummaryInfo(pDb)->getSubject());
  }
  else if (var.iCompare(OD_T("Title")) == 0)
  {
    v.set(oddbGetSummaryInfo(pDb)->getTitle());
  }
  else if (var.iCompare(OD_T("DeviceName")) == 0)
  {
    v.set(OdDbLayoutPtr(pDb->currentLayoutId().safeOpenObject())->getPlotCfgName());
  }
  else if (var.iCompare(OD_T("PageSetupName")) == 0)
  {
    v.set(OdDbLayoutPtr(pDb->currentLayoutId().safeOpenObject())->getPlotSettingsName());
  }
  else if (var.iCompare(OD_T("PaperSize")) == 0)
  {
    OdDbLayoutPtr layout = pDb->currentLayoutId().safeOpenObject();
    v.set(pDb->appServices()->plotSettingsValidator()->getLocaleMediaName(layout, layout->getCanonicalMediaName()));
  }
  else if (var.iCompare(OD_T("PlotStyleTable")) == 0)
  {
    v.set(OdDbLayoutPtr(pDb->currentLayoutId().safeOpenObject())->getCurrentStyleSheet());
  }
  else if (var.iCompare(OD_T("PlotScale")) == 0)
  {
    OdDbLayoutPtr l = pDb->currentLayoutId().safeOpenObject();
    if (l->useStandardScale())
    {
      double d;
      l->getStdScale(d);
      v.set(d);
    }
    else
    {
      double n,d;
      l->getCustomPrintScale(n, d);
      v.set(n/d);
    }
  }
  else if (var.iCompare(OD_T("PlotOrientation")) == 0)
  {
    OdString s;
    switch(OdDbLayoutPtr(pDb->currentLayoutId().safeOpenObject())->plotRotation())
    {
    case OdDbPlotSettings::k0degrees:
      s = OD_T("Portrait");
    	break;
    case OdDbPlotSettings::k180degrees:
      s = OD_T("Portrait (upside-down)");
      break;
    case OdDbPlotSettings::k90degrees:
      s = OD_T("Landscape");
      break;
    case OdDbPlotSettings::k270degrees:
      s = OD_T("Landscape (upside-down)");
      break;
    }
    v.set(s);
  }
  else if (var.iCompare(OD_T("Login")) == 0)
  {
    v.set(pDb->getSysVar(OD_T("LOGINNAME"))->getString());
  }
  else if (var.iCompare(OD_T("CreateDate")) == 0)
  {
    OdString fn = getFileName(pDb);
    if (!fn.isEmpty())
    {
      v.set((OdInt64)odrxSystemServices()->getFileCTime(fn));
    }
    bEvaluated = true;
  }
  else if (var.iCompare(OD_T("SaveDate")) == 0)
  {
    OdString fn = getFileName(pDb);
    if (!fn.isEmpty())
    {
      v.set((OdInt64)odrxSystemServices()->getFileMTime(fn));
    }
    bEvaluated = true;
  }
  else if (var.iCompare(OD_T("FileName")) == 0)
  {
    v.set(getFileName(pDb));
  }
  else if (var.iCompare(OD_T("FileSize")) == 0)
  {
    OdString fn = getFileName(pDb);
    if (!fn.isEmpty())
    {
      OdInt64 fileSize = odrxSystemServices()->getFileSize(fn);
      v.set((OdInt32)(fileSize & 0xffffffff));
    }
    bEvaluated = true;
  }
  else if (var.left(9).iCompare(L"CustomDP.") == 0)
  {
    OdString customDP = var.mid(9);
    OdDbDatabaseSummaryInfoPtr summ = oddbGetSummaryInfo(pDb);
    OdString val;
    if (summ->getCustomSummaryInfo(customDP, val))
    {
      v.set(val);
    }
    bEvaluated = true;
  }
  else if (var.left(5).iCompare(OD_T("Lisp.")) == 0)
  {
    OdFdFieldEnginePEPtr pFieldEnginePE = OdFdFieldEngine::desc()->getX(OdFdFieldEnginePE::desc());
    if(!pFieldEnginePE.isNull())
      pFieldEnginePE->getLispVariable(var.mid(5), v);
  }
  else try // check if this is system variable
  {
    OdRxDictionaryPtr pVarDict = odrxSysRegistry()->getAt(L"ODDB_SYSVARDICT");
    OdSysVarInfo* pSysVarInfo = static_cast<OdSysVarInfo*>(pVarDict->getAt(var).get());
    if (pSysVarInfo)
    {
      OdResBufPtr pRb = pSysVarInfo->m_getFn(pDb);
      if (!pRb.isNull())
      {
        OdRxDictionaryPtr pVarDict2 = odrxSysRegistry()->getAt(L"ODDB_SETVARDICT");
        if(pSysVarInfo->m_mapTypeFn)
          pSysVarInfo->m_mapTypeFn(pDb, pRb, OdSysVarInfo::kToAcadType);
      }
      v.set(pRb);
    }
  }
  catch(const OdError&)
  {
  }

  if (v.isValid() || bEvaluated)
  {
    pResult->setFieldValue(&v);
    pResult->setEvaluationStatus(OdDbField::kSuccess);
  }
  else
  {
    pResult->setFieldValue(0);
    pResult->setEvaluationStatus(OdDbField::kOtherError, 0, OD_T("Variable not supported"));
  }

  return eOk;
}


OdResult OdVarEvaluator::format(OdDbField* pField, OdString& pszValue) const
{
  OdFieldValue fv; 
  OdResult res = pField->getValue(fv);
  if (res != eOk) return res;
  OdString format = pField->getFormat();
  fixPointCoords(format, fv.dataType());

  // OdValue can't handle specifiers that need the database
  OdDbDatabase* pDb = database(pField);
  if (pDb && hasDecimals(fv.dataType()))
  {
    if (format.find('q') != -1) // linear units, current format (from db variables)
    {
      if (format.find(OD_T("q0")) != -1)
      {
        format.format(OD_T("%%lu%d%%pr%d"), pDb->getLUNITS(), pDb->getLUPREC());
      }
      else if (format.find(OD_T("q1")) != -1)
      {
        format.format(OD_T("%%lu%d%%pr%d"), pDb->dimaltu(), pDb->dimaltd());
      }
      // else // q2 and q3 are impossible in AcVar context
    }
    else
    {
      fixPrecision(format, pDb);
    }
  }
  if (fv.format(format, pszValue, pField->database()))
    return eOk;
  return eNotImplemented;
}

ODRX_CONS_DEFINE_MEMBERS(OdExEvaluatorLoader,OdFdFieldEvaluatorLoader,RXIMPL_CONSTR)

OdExEvaluatorLoader::OdExEvaluatorLoader()
{
  m_pEvaluators[L"AcVar"] = OdVarEvaluator::createObject(); 
  m_pEvaluators[L"AcVar.16.2"] = OdVarEvaluator::createObject(); 
  m_pEvaluators[L"AcVar.17.0"] = OdVarEvaluator::createObject(); 
  m_pEvaluators[L"_text"] = OdTextEvaluator::createObject(); 
  m_pEvaluators[L"AcDiesel"] = OdDieselEvaluator::createObject();
  m_pEvaluators[L"AcSm"] = OdSheetSetEvaluator::createObject(); 
  m_pEvaluators[L"AcSm.16.2"] = OdSheetSetEvaluator::createObject(); 
  m_pEvaluators[L"AcObjProp"] = OdObjPropEvaluator::createObject(); 
  m_pEvaluators[L"AcObjProp.16.2"] = OdObjPropEvaluator::createObject(); 
  m_pEvaluators[L"AcExpr"] = OdExprEvaluator::createObject();
  m_pEvaluators[L"AcExpr.16.2"] = OdExprEvaluator::createObject();
}

OdFdFieldEvaluator* OdExEvaluatorLoader::getEvaluator(const OdString& pszEvalId)
{
  EvaluatorMap::iterator i = m_pEvaluators.find(pszEvalId);
  if (i == m_pEvaluators.end()) 
    return 0;
  return i->second;
}

OdFdFieldEvaluator * OdExEvaluatorLoader::findEvaluator(OdDbField* pField, OdString& pszEvalId)
{
  for (EvaluatorMap::iterator i = m_pEvaluators.begin(); i != m_pEvaluators.end(); ++i)
  {
    pszEvalId = i->second->evaluatorId( pField );
    if (!pszEvalId.isEmpty())
      return i->second;
  }
  return 0;
}

ODRX_CONS_DEFINE_MEMBERS(OdDieselEvaluator,OdEvaluatorBase,RXIMPL_CONSTR);

OdDieselEvaluator::OdDieselEvaluator() : OdEvaluatorBase(OD_T("AcDiesel")) {}

const OdString OdDieselEvaluator::evaluatorId(OdDbField* pField) const
{
  if (!pField) 
    return OdString::kEmpty;
  OdString code = pField->getFieldCode(OdDbField::kFieldCode);
  if (code.find(L"\\AcDiesel ") != -1)
    return L"AcDiesel";
  return OdString::kEmpty;
}

DBENT_EXPORT int OdDieselEvaluate(const OdChar* in, OdChar* out, OdDbDatabase* db);

OdResult OdDieselEvaluator::compile(OdDbField* pField, OdDbDatabase*, OdFdFieldResult* pResult) const
{
  OdFieldValue var(pField->getFieldCode(OdDbField::kStripOptions));
  pField->setData(L"DieselExpression", var);
  pResult->setEvaluationStatus(OdDbField::kSuccess);
  return eOk;
}

OdResult OdDieselEvaluator::evaluate(OdDbField* pField, int, OdDbDatabase* pDb, OdFdFieldResult* pResult) const
{
  OdFieldValue varV = pField->getData(OD_T("DieselExpression"));
  OdString s;
  if (!varV.get(s))
  {
    pResult->setEvaluationStatus(OdDbField::kOtherError);
    return eInvalidInput;
  }
  
  OdChar res[257];
  OdDieselEvaluate(s.c_str(), res, pDb);
  OdFieldValue fv = OdString(res);
  pResult->setFieldValue(&fv);
  pResult->setEvaluationStatus(OdDbField::kSuccess);
  return eOk;
}

OdResult OdDieselEvaluator::format(OdDbField* pField, OdString& pszValue) const
{
  OdFieldValue fv; 
  OdResult res = pField->getValue(fv);
  if (res != eOk) 
    return res;
  fv.get(pszValue);
  return eOk;
}
