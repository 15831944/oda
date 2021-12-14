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

using System;
using System.Collections.Generic;
using System.Text;
using Teigha.Core;
using Teigha.TD;

namespace OdDrawingsExamplesMgdLib
{
  public class OdExDbAppAuditInfo : OdDbAuditInfo
  {
    private OdDbHostAppServices m_pHostAppServices;

    /**********************************************************************/
    /* Sets the HostAppServices object for this AppAuditInfo object.      */
    /**********************************************************************/
    public void setHostAppServices(OdDbHostAppServices pAppServices) { m_pHostAppServices = pAppServices; }

    /************************************************************************/
    /* Returns the last error information that is intended for printing.    */
    /************************************************************************/
    //public override MsgInfo getLastInfo()
    //{
    //	return base.getLastInfo();
    //}

    /************************************************************************/
    /* Sets the last error information that is intended for printing.       */
    /************************************************************************/
    //public override void setLastInfo(MsgInfo lastInfo)
    //{
    //	base.setLastInfo(lastInfo);
    //}

    /************************************************************************/
    /* Notification function called to print log information                */
    /************************************************************************/
    public override void printInfo(String strInfo)
    {
      OdDbAuditInfo.MsgInfo miTmp = new OdAuditInfo.MsgInfo();


      if (m_pHostAppServices == null)
        return;

      miTmp.bIsError = false;
      miTmp.strName = strInfo;
      miTmp.strDefaultValue = ("");
      miTmp.strValue = ("");
      miTmp.strValidation = ("");

      setLastInfo(miTmp);

      m_pHostAppServices.auditPrintReport(this, strInfo, (int)getPrintDest());

    }

    /************************************************************************/
    /* Notification function called to print error information.             */
    /************************************************************************/
    public override void printError(String strName,
                                    String strValue,
                                    String strValidation,
                                    String strDefaultValue)
    {
      OdDbAuditInfo.MsgInfo miTmp = new OdAuditInfo.MsgInfo();

      if (m_pHostAppServices == null)
        return;

      miTmp.bIsError = true;
      miTmp.strName = strName;
      miTmp.strDefaultValue = strDefaultValue;
      miTmp.strValue = strValue;
      miTmp.strValidation = strValidation;
      setLastInfo(miTmp);

      String strLine = "";
      if (strName != "")
      {
        strLine += strName;
        strLine += (". ");
      }

      if (strValue != "")
      {
        strLine += ("An invalid ");
        strLine += strValue;
        strLine += (" was found. ");
      }

      if (strValidation != "")
      {
        strLine += ("Validation: ");
        strLine += strValidation;
        strLine += (". ");
      }

      if (strDefaultValue != "")
      {
        if (fixErrors())
          strLine += ("Replaced by ");
        else
          strLine += ("Default value is ");
        strLine += strDefaultValue;
        strLine += (".");
      }

      m_pHostAppServices.auditPrintReport(this, strLine, (int)getPrintDest());
    }

  }
}
