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
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using Teigha.Core;
using Teigha.TD;

namespace OdaSdk_First_NET_App
{
  public partial class Form1 : Form
  {
    public Form1()
    {
      InitializeComponent();

      this.Load += Form1_Load;
      this.FormClosing += Form1_FormClosing;
    }

    private void Form1_FormClosing(object sender, FormClosingEventArgs e)
    {
      UnInitODA();
    }

    private void Form1_Load(object sender, EventArgs e)
    {
      InitODA();
      CreateGUI();
    }

    private MemoryTransaction rootMemoryTransaction;
    private SystemServices sysServices;
    private HostAppServices hostAppServices;
    private void InitODA()
    {
      //Create Root Memory Transaction , more detail see: https://docs.opendesign.com/tnet/tnet_Memory.html
      rootMemoryTransaction = MemoryManager.GetMemoryManager().StartTransaction();

      //Create System Services
      sysServices = new SystemServices();

      //Create Host Application Services
      hostAppServices = new HostAppServices();

      //Initialize ODA SDK
      TD_Db.odInitialize(sysServices);
    }

    private void UnInitODA()
    {
      MemoryManager.GetMemoryManager().StopTransaction(rootMemoryTransaction);

      //Uninitialize ODA SDK
      TD_Db.odUninitialize();
    }

    private OdaRenderingPanel renderingPanel;
    private void CreateGUI()
    {
      TableLayoutPanel mainPanel = new TableLayoutPanel();
      this.Controls.Add(mainPanel);
      mainPanel.Dock = DockStyle.Fill;
      mainPanel.RowCount = 2;
      mainPanel.ColumnCount = 1;

      Button btnOpenDwgFile = new Button();
      btnOpenDwgFile.Text = "Open Dwg File";
      mainPanel.Controls.Add(btnOpenDwgFile, 0, 1);
      btnOpenDwgFile.Click += BtnOpenDwgFile_Click;

      renderingPanel = new OdaRenderingPanel();
      renderingPanel.Dock = DockStyle.Fill;
      renderingPanel.BorderStyle = BorderStyle.FixedSingle;
      mainPanel.Controls.Add(renderingPanel, 0, 2);
    }

    private void BtnOpenDwgFile_Click(object sender, EventArgs e)
    {
      OpenAndRenderDwg();
    }

    private MemoryTransaction mTrDatabase;
    private void OpenAndRenderDwg()
    {
      OpenFileDialog openFileDialog = new OpenFileDialog();
      openFileDialog.Filter = "dwg files (*.dwg)|*.dwg|All files (*.*)|*.*";

      if (openFileDialog.ShowDialog() == DialogResult.OK)
      {
        StartTransactionForDatabase();
        String dwgFilePath = openFileDialog.FileName;
        OdDbDatabase database = hostAppServices.readFile(dwgFilePath);
        renderingPanel.SetDatabase(database);
      }
    }

    private void StartTransactionForDatabase()
    {
      if (mTrDatabase != null)
        StopTransactionForDatabase();

      mTrDatabase = MemoryManager.GetMemoryManager().StartTransaction();
    }

    private void StopTransactionForDatabase()
    {
      if (mTrDatabase != null)
      {
        MemoryManager.GetMemoryManager().StopTransaction(mTrDatabase);
        mTrDatabase = null;
      }
    }
  }
}
