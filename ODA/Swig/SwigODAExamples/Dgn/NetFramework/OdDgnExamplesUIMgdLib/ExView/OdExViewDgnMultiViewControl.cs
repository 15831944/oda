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
using System.Windows.Forms;
using Teigha.Core;
using Teigha.TG;
using OdKernelExamplesUIMgdLib.ExView;
using OdDgnExamplesMgdLib.ExView;
using System.Collections.Generic;

namespace OdDgnExamplesUIMgdLib.ExView
{
  public class OdExViewDgnMultiViewControl : Control
  {
    OdDgDatabase m_DgnDatabase;
    OdExViewDgnControl[] m_viewControls;
    TableLayoutPanel m_tableLayoutPanel;

    public OdExViewDgnMultiViewControl()
    {
      this.m_tableLayoutPanel = new TableLayoutPanel();
      this.m_tableLayoutPanel.Dock = System.Windows.Forms.DockStyle.Fill;
      this.Controls.Add(m_tableLayoutPanel);
    }

    public void Init(OdDgDatabase database)
    {
      m_DgnDatabase = database;

      m_viewControls = CreateViews(m_DgnDatabase);

      InitViewControlsAsTable(m_tableLayoutPanel, m_viewControls, m_DgnDatabase);
    }

    public void UnInit()
    {
      UnInitViewControls();

      m_DgnDatabase = null;
    }

    public void OnZoomToExtents()
    {
      foreach (OdExViewDgnControl view in m_viewControls)
        view.OnZoomToExtents();
    }

    public void EnableZoom(bool state)
    {
      foreach (OdExViewDgnControl view in m_viewControls)
        view.EnableZoom(state);
    }

    public void EnablePan(bool state)
    {
      foreach (OdExViewDgnControl view in m_viewControls)
        view.EnablePan(state);
    }

    public void EnableRotation(bool state)
    {
      foreach (OdExViewDgnControl view in m_viewControls)
        view.EnableRotation(state);
    }

    private void UnInitViewControls()
    {
      foreach (var view in m_viewControls)
        view.UnInit();
      m_viewControls = null;
    }

    private static OdExViewDgnControl[] CreateViews(OdDgDatabase database)
    {
      OdDgViewGroup pViewGroup = GetViewGroup(database);
      if (pViewGroup == null)
        throw new Exception("Not found view group");

      List<OdExViewDgnControl> viewControls = new List<OdExViewDgnControl>();
      OdDgElementIterator viewIter = pViewGroup.createIterator();
      for (; !viewIter.done(); viewIter.step())
      {
        OdDgView pView = OdDgView.cast(viewIter.item().openObject());
        if (pView != null && pView.getVisibleFlag())
        {
          OdExViewDgnControl viewCtrl = new OdExViewDgnControl();
          OdDgElementId modelId = GetModelId(database);
          OdDgElementId vectorizeId = new OdDgElementId(pView.id());
          viewCtrl.PrepareInit(modelId, vectorizeId);
          viewControls.Add(viewCtrl);
        }
      }
      return viewControls.ToArray();
    }

    private static OdDgViewGroup GetViewGroup(OdDgDatabase db)
    {
      if (db == null)
        throw new Exception("OdDgDatabase is null");
      OdDgViewGroup pViewGroup = (OdDgViewGroup)db.getActiveViewGroupId().openObject();
      if (pViewGroup == null)
        pViewGroup = (OdDgViewGroup)db.recommendActiveViewGroupId().openObject();
      return pViewGroup;
    }

    private static OdDgElementId GetModelId(OdDgDatabase db)
    {
      if (db == null)
        throw new Exception("OdDgDatabase is null");
      OdDgElementId modelId;
      modelId = db.getActiveModelId();
      if (modelId.isNull())
      {
        modelId = db.getDefaultModelId();
        db.setActiveModelId(modelId);
      }
      return modelId;
    }

    private static void InitViewControlsAsTable(TableLayoutPanel tableLayoutPanel, OdExViewDgnControl[] viewControls, OdDgDatabase database)
    {
      tableLayoutPanel.CellBorderStyle = TableLayoutPanelCellBorderStyle.InsetDouble;
      tableLayoutPanel.RowCount = (int)Math.Round(Math.Sqrt(viewControls.Length));
      tableLayoutPanel.ColumnCount = (int)Math.Round(viewControls.Length / (float)tableLayoutPanel.RowCount);

      tableLayoutPanel.ColumnStyles.Clear();
      for (int i = 0; i < tableLayoutPanel.ColumnCount; i++)
        tableLayoutPanel.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F / tableLayoutPanel.ColumnCount));

      tableLayoutPanel.RowStyles.Clear();
      for (int i = 0; i < tableLayoutPanel.RowCount; i++)
        tableLayoutPanel.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F / tableLayoutPanel.RowCount));

      tableLayoutPanel.Controls.Clear();
      foreach (OdExViewDgnControl viewCtrl in viewControls)
      {
        viewCtrl.Dock = DockStyle.Fill;
        viewCtrl.Margin = new Padding(1);
        tableLayoutPanel.Controls.Add(viewCtrl);
        viewCtrl.Init(database);
      }
    }
  }
}