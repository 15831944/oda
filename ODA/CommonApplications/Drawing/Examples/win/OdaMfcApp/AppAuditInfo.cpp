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

// AppAuditInfo.cpp: implementation of the CAppAuditInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "odamfcapp.h"
#include "AppAuditInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAppAuditInfo::CAppAuditInfo()
{
  m_pHostAppServices = NULL;
}

/************************************************************************/
/* Returns the last error information that is intended for printing.    */
/************************************************************************/
const OdDbAuditInfo::MsgInfo& CAppAuditInfo::getLastInfo()
{
  return OdDbAuditInfo::getLastInfo(); 
}

/************************************************************************/
/* Sets the last error information that is intended for printing.       */
/************************************************************************/
void CAppAuditInfo::setLastInfo(OdDbAuditInfo::MsgInfo &miLast)
{
  OdDbAuditInfo::setLastInfo(miLast);
  return;
}

/************************************************************************/
/* Notification function called to print log information                */
/************************************************************************/
void CAppAuditInfo::printInfo(const OdString& strInfo)
{
  OdDbAuditInfo::MsgInfo  miTmp;

  ODA_ASSERT(m_pHostAppServices);

  miTmp.bIsError = false;
  miTmp.strName = strInfo;
  miTmp.strDefaultValue = _T("");
  miTmp.strValue = _T("");
  miTmp.strValidation = _T("");
  setLastInfo(miTmp);
  
  m_pHostAppServices->auditPrintReport(this, strInfo, getPrintDest());
  ODA_TRACE1("%ls\n", strInfo.c_str());
}

/************************************************************************/
/* Notification function called to print error information.             */
/************************************************************************/
void CAppAuditInfo::printError(const OdString& strName,
                               const OdString& strValue, 
                               const OdString& strValidation,
                               const OdString& strDefaultValue)
{
  OdDbAuditInfo::MsgInfo  miTmp;

  ODA_ASSERT(m_pHostAppServices);

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
      strLine += OD_T("Default value is ");
    strLine += strDefaultValue;
    strLine += OD_T(".");
  }

  m_pHostAppServices->auditPrintReport(this, strLine, getPrintDest());
  ODA_TRACE1("%ls\n", strLine.c_str());
}

