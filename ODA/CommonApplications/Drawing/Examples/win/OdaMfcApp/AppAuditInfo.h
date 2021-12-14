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

// AppAuditInfo.h: interface for the CAppAuditInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APPAUDITINFO_H__E4A98885_213C_4307_81BA_DFAEB284F6A9__INCLUDED_)
#define AFX_APPAUDITINFO_H__E4A98885_213C_4307_81BA_DFAEB284F6A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "DbAudit.h"
/************************************************************************/
/* Specialization of OdDbAuditInfo for AuditDlg.                        */
/************************************************************************/
class CAppAuditInfo : public OdDbAuditInfo
{
public:
  CAppAuditInfo();
  virtual ~CAppAuditInfo() {}

public:
  /**********************************************************************/
  /* Sets the HostAppServices object for this AppAuditInfo object.      */
  /**********************************************************************/
  void setHostAppServices(OdDbHostAppServices* pAppServices) { m_pHostAppServices = pAppServices; }

  /**********************************************************************/
  /* Notification function called to print error information            */
  /**********************************************************************/

  virtual void printError(const OdString& strName, 
                          const OdString& strValue, 
                          const OdString& strValidation = OdString(), 
                          const OdString& strDefaultValue = OdString());

  /**********************************************************************/
  /* Notification function called to print log information              */
  /**********************************************************************/
  virtual void printInfo (const OdString& strInfo);

  /**********************************************************************/
  /* Returns the last error information that is intended for printing.  */
  /**********************************************************************/
  virtual const OdDbAuditInfo::MsgInfo& getLastInfo();

  /**********************************************************************/
  /* Sets the last error information that is intended for printing.     */
  /**********************************************************************/
  virtual void setLastInfo(OdDbAuditInfo::MsgInfo &miLast);
private:
  OdDbHostAppServices*    m_pHostAppServices;

};

#endif // !defined(AFX_APPAUDITINFO_H__E4A98885_213C_4307_81BA_DFAEB284F6A9__INCLUDED_)
