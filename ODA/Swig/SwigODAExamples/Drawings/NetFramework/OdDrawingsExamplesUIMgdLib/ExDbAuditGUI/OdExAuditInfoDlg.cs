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
using System.ComponentModel;
using System.Drawing;
using System.Linq;
using System.Text;
//using System.Threading.Tasks;
using System.Windows.Forms;
using Teigha.TD;
using Teigha.Core;

namespace OdDrawingsExamplesUIMgdLib
{
  public partial class OdExAuditInfoDlg : Form
  {
    ListView m_AuditErrList;
    ListView m_AuditInfoList;

    public OdExAuditInfoDlg()
    {
      InitializeComponent();
      m_AuditErrList = this.listViewErrors;
      m_AuditInfoList = this.listViewSummary;
    }

    public void printReport(OdDbAuditInfo pAuditInfo)
    {
      if (pAuditInfo.getLastInfo().bIsError)
      {
        int nItem = m_AuditErrList.Items.Count;
        m_AuditErrList.Items.Add(pAuditInfo.getLastInfo().strName);
        m_AuditErrList.Items[nItem].SubItems.Add(pAuditInfo.getLastInfo().strValue);
        m_AuditErrList.Items[nItem].SubItems.Add(pAuditInfo.getLastInfo().strValidation);
        m_AuditErrList.Items[nItem].SubItems.Add(pAuditInfo.getLastInfo().strDefaultValue);
      }
      else
      {
        //int nItem = m_AuditInfoList.Items.Count;
        m_AuditInfoList.Items.Add(pAuditInfo.getLastInfo().strName);
      }
    }

  }
}
