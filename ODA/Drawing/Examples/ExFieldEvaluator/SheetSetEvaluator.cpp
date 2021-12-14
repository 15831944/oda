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
#include "ExFieldEvaluator.h"
#include "RxObjectImpl.h"
#include "DbHostAppServices.h"


ODRX_CONS_DEFINE_MEMBERS(OdSheetSetEvaluator, OdEvaluatorBase, RXIMPL_CONSTR);

OdSheetSetEvaluator::OdSheetSetEvaluator() : OdEvaluatorBase(OD_T("AcSm")) {}

const OdString OdSheetSetEvaluator::evaluatorId(OdDbField* pField) const
{
  if (!pField)
    return OdString::kEmpty;
  OdString code = pField->getFieldCode(OdDbField::kFieldCode);
  if (code.find(OD_T("\\AcSm ")) != -1)
    return OD_T("AcSm");
  else if (code.find(OD_T("\\AcSm.16.2 ")) != -1)
    return OD_T("AcSm.16.2");
  return OdString::kEmpty;
}

/*
Here are examples of sheetset fields created by acad in its Field dialog - their field codes (with kStripOptions) and data items.

CurrentSheetSet:                                  SheetSet.Name
                                                  |      | |  |
                        SheetSetCompName ---------+------+ |  |
                        SheetSetPropertyName --------------+--+

CurrentSheetSubSet:                               Subset.Name
                                                  |    | |  |
                        SheetSetCompName ---------+----+ |  |
                        SheetSetPropertyName ------------+--+

CurrentSheetTitle:                                Sheet.Title
                                                  |   | |   |
                        SheetSetCompName ---------+---+ |   |
                        SheetSetPropertyName -----------+---+


SheetSetPlaceholder > SheetTitle:                 ?Sheet.Title
                                                  |    | |   |
                        SheetSetCompName ---------+----+ |   |
                        SheetSetPropertyName ------------+---+

SheetSetPlaceholder > ViewTitle:                  ?View.Title
                                                  |   | |   |
                        SheetSetCompName ---------+---+ |   |
                        SheetSetPropertyName -----------+---+


SheetSet > sheet set > SheetSetName:              Database("C:\mysheetset.dst").SheetSet("gA607CDB0-8806-429F-B134-7A00AA1835B8").Name
                                                            |               |             |                                   |   |  |
                        SheetSetFile -----------------------+---------------+             |                                   |   |  |
                        SheetSetId -------------------------------------------------------+-----------------------------------+   |  |
                        SheetSetPropertyName -------------------------------------------------------------------------------------+--+

SheetSet > subset:                                Database("C:\mysheetset.dst").SheetSet("gA607CDB0-8806-429F-B134-7A00AA1835B8").Component("gED49D703-892B-490B-B018-F4DCD3AF1AD4").Name
                                                            |               |             |                                   |   |       |  |                                   |   |  |
                        SheetSetFile -----------------------+---------------+             |                                   |   |       |  |                                   |   |  |
                        SheetSetId -------------------------------------------------------+-----------------------------------+   |       |  |                                   |   |  |
                        SheetSetCompName -----------------------------------------------------------------------------------------+-------+  |                                   |   |  |
                        SheetSetCompId ------------------------------------------------------------------------------------------------------+-----------------------------------+   |  |
                        SheetSetPropertyName ----------------------------------------------------------------------------------------------------------------------------------------+--+

SheetSet > sheet > SheetTitle:                    Database("C:\mysheetset.dst").SheetSet("gA607CDB0-8806-429F-B134-7A00AA1835B8").Component("gBFA67B7E-EEF2-4B7C-AAF8-6303C930DFB5").Title
                                                            |               |             |                                   |   |       |  |                                   |   |   |
                        SheetSetFile -----------------------+---------------+             |                                   |   |       |  |                                   |   |   |
                        SheetSetId -------------------------------------------------------+-----------------------------------+   |       |  |                                   |   |   |
                        SheetSetCompName -----------------------------------------------------------------------------------------+-------+  |                                   |   |   |
                        SheetSetCompId ------------------------------------------------------------------------------------------------------+-----------------------------------+   |   |
                        SheetSetPropertyName ----------------------------------------------------------------------------------------------------------------------------------------+---+

SheetView > view > ViewTitle:                     Database("C:\mysheetset.dst").SheetSet("gA607CDB0-8806-429F-B134-7A00AA1835B8").Component("g4A04C0C7-5457-42B3-A560-340E1185E286").Title
                                                            |               |             |                                   |   |       |  |                                   |   |   |
                        SheetSetFile -----------------------+---------------+             |                                   |   |       |  |                                   |   |   |
                        SheetSetId -------------------------------------------------------+-----------------------------------+   |       |  |                                   |   |   |
                        SheetSetCompName -----------------------------------------------------------------------------------------+-------+  |                                   |   |   |
                        SheetSetCompId ------------------------------------------------------------------------------------------------------+-----------------------------------+   |   |
                        SheetSetPropertyName ----------------------------------------------------------------------------------------------------------------------------------------+---+
*/

static bool setDataItem(OdString& codeStr, const OdString& codeItem, const OdString& dataItem, OdDbField* pField)
{
  if (codeStr.left(codeItem.getLength()) == codeItem)
  {
    codeStr = codeStr.mid(codeItem.getLength());
    int pos = codeStr.find('\"');
    if (pos > 0)
    {
      OdFieldValuePtr fldData = OdFieldValue::createObject();
      fldData->set(codeStr.left(pos));
      pField->setData(dataItem, *fldData);
      codeStr = codeStr.mid(pos + 2);
      return true;
    }
  }
  return false;
}

bool compileHref(OdDbField* pField, OdFdFieldResult* pResult);

OdResult OdSheetSetEvaluator::compile(OdDbField* pField, OdDbDatabase* /*pDb*/, OdFdFieldResult* pResult) const
{
  OdString code = pField->getFieldCode(OdDbField::kStripOptions);
  int dot = code.reverseFind('.');
  if (dot < 0)
  {
    pResult->setEvaluationStatus(OdDbField::kSyntaxError, 0, OD_T("Sheetset field code must contain a dot."));
    return eInvalidInput;
  }
  OdString compName = code.left(dot);
  OdString propName = code.mid(dot + 1);
  OdFieldValuePtr fData = OdFieldValue::createObject();
  if (!compName.isEmpty())
  {
    // format is expected one of:
    // 1) {component-name}
    // 2) Database("dst-file-path").SheetSet("sheetset-ID")
    // 3) Database("dst-file-path").SheetSet("sheetset-ID").Component("component-ID")
    if (setDataItem(compName, OD_T("Database(\""), OD_T("SheetSetFile"), pField))
    {
      if (setDataItem(compName, OD_T(".SheetSet(\""), OD_T("SheetSetId"), pField) &&
        setDataItem(compName, OD_T(".Component(\""), OD_T("SheetSetCompId"), pField))
      {
        fData->set(OD_T("Component"));
        pField->setData(OD_T("SheetSetCompName"), *fData);
      }
    }
    else
    {
      fData->set(compName);
      pField->setData(OD_T("SheetSetCompName"), *fData);
    }
  }
  fData = OdFieldValue::createObject();
  if (!propName.isEmpty())
  {
    fData->set(propName);
    pField->setData(OD_T("SheetSetPropertyName"), *fData);
  }
  if (!compileHref(pField, pResult))
    return eInvalidInput;
  pResult->setEvaluationStatus(OdDbField::kSuccess);
  return eOk;
}

OdResult OdSheetSetEvaluator::evaluate(OdDbField* pField, int /*nContext*/, OdDbDatabase* pDb, OdFdFieldResult* pResult) const
{
  OdString propValue;
  OdFdFieldEnginePEPtr pFieldEnginePE = OdFdFieldEngine::desc()->getX(OdFdFieldEnginePE::desc());
  OdResult result = pFieldEnginePE.isNull() ? eWrongDatabase :
    pFieldEnginePE->getSheetSetProperty(propValue, pField, pDb);
  if (result == eWrongDatabase)  // if PE or sheetset were not found, do not update the text
  {
    pResult->setEvaluationStatus(OdDbField::kNotYetEvaluated);
    return eOk;
  }
  else if (result == eInvalidInput)
  {
    pResult->setEvaluationStatus(OdDbField::kOtherError);
    return result;
  }
  else
    ODA_ASSERT(result == eOk);
  OdFieldValuePtr fValue = OdFieldValue::createObject();
  fValue->set(propValue);
  pResult->setFieldValue(fValue);
  pResult->setEvaluationStatus(OdDbField::kSuccess);
  return eOk;
}

OdResult OdSheetSetEvaluator::format(OdDbField* pField, OdString& value) const
{
  OdFieldValue fValue;
  OdResult res = pField->getValue(fValue);
  if (res != eOk) return res;
  OdString format = pField->getFormat();
  if (fValue.format(format, value, pField->database()))
    return eOk;
  return eNotImplementedYet;
}
