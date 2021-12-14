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
// OdqAudit.cpp
//

#include "OdqInterfaces.h"

#include "OdqAudit.h"
#include "DbDatabase.h"
#include "DbBaseHostAppServices.h"

//////////////////////////////////////////////////////////////////////////

OdqAuditInfo::OdqAuditInfo(OdRxObject* pRxDatabase)
: m_pRxDatabase(pRxDatabase)
{
  // there is no audit functionality for DGN
  ODA_ASSERT_ONCE(OdDbDatabase::cast(pRxDatabase).get()); 
}

OdqAuditInfo::~OdqAuditInfo() 
{
}

OdRxObject* OdqAuditInfo::baseDatabase()
{
  ODA_ASSERT_ONCE(m_pRxDatabase)
  return m_pRxDatabase;
}

// Notification function called to print error information
void OdqAuditInfo::printError(const OdString& strName, 
                              const OdString& strValue, 
                              const OdString& strValidation, // = OdString()
                              const OdString& strDefaultValue) // = OdString()
{
  OdDbAuditInfo::MsgInfo  miTmp;

  ODA_ASSERT(getIApp()->getBaseHostAppServices());

  miTmp.bIsError = true;
  miTmp.strName = strName;
  miTmp.strDefaultValue = strDefaultValue;
  miTmp.strValue = strValue;
  miTmp.strValidation = strValidation;
  setLastInfo(miTmp);

  OdString strLine;
  if (!strName.isEmpty())
  {
    strLine += strName;
    strLine += OD_T(". ");
  }

  if (!strValue.isEmpty())
  {
    strLine += OD_T("An invalid ");
    strLine += strValue;
    strLine += OD_T(" was found. ");
  }

  if (!strValidation.isEmpty())
  {
    strLine += OD_T("Validation: ");
    strLine += strValidation;
    strLine += OD_T(". ");
  }

  if (!strDefaultValue.isEmpty())
  {
    if (fixErrors())
      strLine += OD_T("Replaced by ");
    else
      strLine += OD_T("Devault value is ");
    strLine += strDefaultValue;
    strLine += OD_T(".");
  }

  getIApp()->getBaseHostAppServices()->auditPrintReport(this, strLine, getPrintDest());
  ODA_TRACE1("%ls\n", strLine.c_str());
}

// Notification function called to print log information
void OdqAuditInfo::printInfo(const OdString& strInfo)
{
  OdDbAuditInfo::MsgInfo  miTmp;

  ODA_ASSERT(getIApp()->getBaseHostAppServices());

  miTmp.bIsError = false;
  miTmp.strName = strInfo;
  miTmp.strDefaultValue = OD_T("");
  miTmp.strValue = OD_T("");
  miTmp.strValidation = OD_T("");
  setLastInfo(miTmp);

  getIApp()->getBaseHostAppServices()->auditPrintReport(this, strInfo, getPrintDest());
  ODA_TRACE1("%ls\n", strInfo.c_str());
}

// Returns the last error information that is intended for printing
const OdDbAuditInfo::MsgInfo& OdqAuditInfo::getLastInfo()
{
  return OdDbAuditInfo::getLastInfo(); 
}

// Sets the last error information that is intended for printing
void OdqAuditInfo::setLastInfo(OdDbAuditInfo::MsgInfo &miLast)
{
  OdDbAuditInfo::setLastInfo(miLast);
}
