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
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using Teigha.Core;
using Teigha.TG;
using System.Diagnostics;
using System.IO;
using System.Reflection;
using OdDgnExamplesUIMgdLib.ExView;

namespace OdaDgnAppSwigMgd
{
  public partial class VectorizeForm : Form
  {
    private OdExViewDgnControl viewCtrl;
    private OdDgDatabase m_Database;

    public VectorizeForm(OdDgDatabase database, OdDgElementId vectorizedViewId, OdDgElementId vectorizedModelId)
    {
      InitializeComponent();

      m_Database = database;

      viewCtrl = new OdExViewDgnControl();
      viewCtrl.PrepareInit(vectorizedModelId, vectorizedViewId);
      viewCtrl.Dock = DockStyle.Fill;
      this.Controls.Add(viewCtrl);

      this.Load += VectorizeForm_Load;
      this.FormClosed += VectorizeForm_FormClosed;
    }

    public void OnZoomToExtents()
    {
      viewCtrl.OnZoomToExtents();
    }

    public void EnableZoom(bool state)
    {
      viewCtrl.EnableZoom(state);
    }

    public void EnablePan(bool state)
    {
      viewCtrl.EnablePan(state);
    }

    public void EnableRotation(bool state)
    {
      viewCtrl.EnableRotation(state);
    }

    #region Init/UnInit Form

    private void VectorizeForm_Load(object sender, EventArgs e)
    {
      Init();
    }

    private void VectorizeForm_FormClosed(object sender, FormClosedEventArgs e)
    {
      UnInit();
    }

    private void Init()
    {
      try
      {
        InitFormTransaction();

        viewCtrl.Init(m_Database);
      }
      catch (Exception ex)
      {
        UnInit();
        MessageBox.Show(ex.Message, "Error Initialize Vectorize Form");
      }
    }

    private void UnInit()
    {
      viewCtrl.UnInit();

      UnInitFormTransaction();
    }

    #endregion

    #region FormTransaction
    MemoryTransaction manTr = null;
    private void InitFormTransaction()
    {
      manTr = MemoryManager.GetMemoryManager().StartTransaction();
    }

    private void UnInitFormTransaction()
    {
      if (manTr != null)
        MemoryManager.GetMemoryManager().StopTransaction(manTr);
    }
    #endregion

  }
}
