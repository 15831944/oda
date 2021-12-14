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
using System.IO;
using Microsoft.Win32;

namespace OdReadExSwigMgd
{
  /************************************************************************/
  /* Define an a Custom OdDbAuditInfo class                               */
  /************************************************************************/
  class OdExDbAuditInfo : OdDbAuditInfo
  {
    OdStreamBuf m_ReportFile;
    bool m_isSomething;
    String m_ReportFileName;

    public OdExDbAuditInfo(String reportFileName)
    {
      m_isSomething = false;
      m_ReportFileName = reportFileName;
      /********************************************************************/
      /* Create a report file                                             */
      /********************************************************************/
      m_ReportFile = Globals.odrxSystemServices().createFile(m_ReportFileName,
        FileAccessMode.kFileWrite, FileShareMode.kShareDenyNo, FileCreationDisposition.kCreateAlways);
    }
    public override void Dispose()
    {
      /********************************************************************/
      /* Refer user to OD_T("Audit Report.txt") if error(s) have been output.   */
      /********************************************************************/
      if (m_isSomething)
      {
        Console.WriteLine("\n\nAudit error : Check \"{0}\" for errors.", m_ReportFileName);
      }
      else
      {
        Console.WriteLine("\n\nAudit : ok.\n");
      }
      base.Dispose();
    }

    /**********************************************************************/
    /* Print error to ReportFile                                          */
    /**********************************************************************/
    public override void printError(String strName, String strValue, String strValidation, String strDefaultValue)
    {
      m_isSomething = true;
      String str = String.Format("{0} {1} {2} {3}\n",
        strName, strValue, strValidation, strDefaultValue);
      m_ReportFile.putBytes(Encoding.Unicode.GetBytes(str));
    }

    /**********************************************************************/
    /* Print info to ReportFile                                           */
    /**********************************************************************/
    public override void printInfo(String strInfo)
    {
      m_ReportFile.putBytes(Encoding.Unicode.GetBytes(strInfo + "\n"));
    }
  }

}