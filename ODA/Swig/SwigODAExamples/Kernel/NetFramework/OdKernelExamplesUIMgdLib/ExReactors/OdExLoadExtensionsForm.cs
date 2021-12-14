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
using System.Windows.Forms;
using Teigha.Core;
using OdKernelExamplesUIMgdLib;

namespace OdKernelExamplesUIMgdLib.ExReactors
{
  public partial class OdExLoadExtensionsForm : Form
  {
    private static OdExRxDLinkerLoadAppsReactor m_pLoadedApps;
    private ListView m_AppsList;

    public OdExLoadExtensionsForm()
    {
      InitializeComponent();
      m_AppsList = this.listViewApps;
      OnInitDialog();
    }

    public static void rxInit()
    {
      m_pLoadedApps = new OdExRxDLinkerLoadAppsReactor();
      Teigha.Core.Globals.odrxDynamicLinker().addReactor(m_pLoadedApps);
    }

    public static void rxUninit()
    {
      Teigha.Core.Globals.odrxDynamicLinker().removeReactor(m_pLoadedApps);
    }

    public bool OnInitDialog()
    {
      m_AppsList.Items.Clear();

      foreach (ListViewItem item in m_pLoadedApps.m_pListBox.Items)
      {
        ListViewItem itemClone = (ListViewItem)item.Clone();
        m_AppsList.Items.Add(itemClone);
      }

      OnAppsListEvent();

      return true;
    }

    private void OnLoadApp(object sender, EventArgs e)
    {
      OpenFileDialog dlg = new OpenFileDialog();
      dlg.InitialDirectory = getApplicationPath();
      dlg.Filter = "Teigha Run-time Extension (*.dll,*.tx)|*.dll;*.tx|Any file (*.*)|*.*||";
      dlg.Title = "Load application";

      if (dlg.ShowDialog() == DialogResult.OK)
      {
        try
        {
          Teigha.Core.Globals.odrxDynamicLinker().loadModule(dlg.FileName, false);
          OnInitDialog();
        }
        catch (OdError error)
        {
          MessageBox.Show(error.description());
        }
      }

    }

    private void OnUnloadApp(object sender, EventArgs e)
    {
      ListViewItem curItem = m_AppsList.FocusedItem;
      if (curItem != null)
      {
        if (Teigha.Core.Globals.odrxDynamicLinker().unloadModule(curItem.Text))
        {
          m_AppsList.Items.Remove(curItem);
        }
        else
        {
          MessageBox.Show("Module is referenced.", "Failed");
        }
      }
      OnAppsListEvent();
    }

    private void listViewApps_SelectedIndexChanged(object sender, EventArgs e)
    {
      OnAppsListEvent();
    }

    private void OnAppsListEvent()
    {
      this.btnUnload.Enabled = m_AppsList.FocusedItem != null;
    }

    private string getApplicationPath()
    {
      String absPath;
      absPath = System.Diagnostics.Process.GetCurrentProcess().MainModule.FileName;
      int lastIndex = absPath.LastIndexOf("\\");
      return absPath.Substring(0, lastIndex + 1);
    }

  }
}
