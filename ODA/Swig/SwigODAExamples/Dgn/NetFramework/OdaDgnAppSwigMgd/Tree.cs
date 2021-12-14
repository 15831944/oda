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
using OdDgnExamplesUIMgdLib.ExDumper;

namespace OdaDgnAppSwigMgd
{
  public partial class Tree : Form
  {
    public OdDgDatabase Database { get; private set; }

    public Tree(OdDgDatabase db)
    {
      Database = db;
      InitializeComponent();
      treeView.AfterSelect += new TreeViewEventHandler(treeView_AfterSelect);
    }

    void treeView_AfterSelect(object sender, TreeViewEventArgs e)
    {
      if (e.Node.Nodes.Count == 0)
        TreeFiller.explandItem(e.Node);
      listView.Items.Clear();
      listView.SuspendLayout();
      listView.UseWaitCursor = true;
      try
      {
        OdRxObject pObj = TreeFiller.getObject(e.Node);
        if (pObj != null)
        {
          ListFiller.dump(pObj);
        }
      }
      catch (OdError ex)
      {
        ListViewItem item = listView.Items.Add("OdError:");
        item.SubItems.Add(ex.description());
      }
      catch (Exception ex1)
      {
        ListViewItem item = listView.Items.Add("OdError:");
        item.SubItems.Add(ex1.Message);
      }
      listView.ResumeLayout(true);
      listView.UseWaitCursor = false;
      listView.Update();
    }

    OdExDgnListFiller m_pListFiller = null;
    OdExDgnListFiller ListFiller
    {
      get
      {
        if (m_pListFiller == null)
          m_pListFiller = new OdExDgnListFiller(Database, listView);
        return m_pListFiller;
      }
    }
    OdExDgnTreeFiller m_pTreeFiller = null;
    OdExDgnTreeFiller TreeFiller
    {
      get
      {
        if (m_pTreeFiller == null)
          m_pTreeFiller = new OdExDgnTreeFiller(Database, treeView);
        return m_pTreeFiller;
      }
    }
    public bool Find(TreeNode item, OdDgElementId id)
    {
      for (; item != null; item = item.NextNode)
      {
        OdDgElementId data = item.Tag as OdDgElementId;
        OdDbHandle h1 = id.getHandle();
        OdDbHandle h2 = data.getHandle();
        if ((UInt64)h1 == (UInt64)h2)
        {
          treeView.SelectedNode = item;
          return true;
        }
        if (item.Nodes.Count > 0)
        {
          item.Expand();
          if (Find(item.Nodes[0], id))
            return true;
          item.Collapse(true);
        }
      }
      return false;
    }
    void Find(OdDgElementId targetId)
    {
      Find(treeView.Nodes[0], targetId);
    }

    private void findToolStripMenuItem_Click(object sender, EventArgs e)
    {
      FindObjectDlg fd = new FindObjectDlg();
      if (fd.ShowDialog() == DialogResult.OK)
      {
        // Calculating handle
        OdDbHandle han = new OdDbHandle(fd.textBox.Text);
        // Requesting ObjectId
        OdDgElementId targetId = Database.getElementId(han);
        if (targetId.isNull())
          MessageBox.Show(this, "Incorrect handle", "Error", MessageBoxButtons.OK, MessageBoxIcon.Stop);
        else
          Find(targetId);
      }
    }
    public void FillTree()
    {
      treeView.Nodes.Clear();
      try
      {
        // Tables
        TreeFiller.addElement(Database.getLevelTable(), TreeFiller.DbTreeItem);
        TreeFiller.addElement(Database.getLevelFilterTable(), TreeFiller.DbTreeItem);
        TreeFiller.addElement(Database.getFontTable(), TreeFiller.DbTreeItem);
        TreeFiller.addElement(Database.getTextStyleTable(), TreeFiller.DbTreeItem);
        TreeFiller.addElement(Database.getDimStyleTable(), TreeFiller.DbTreeItem);
        TreeFiller.addElement(Database.getMaterialTable(), TreeFiller.DbTreeItem);
        TreeFiller.addElement(Database.getMultilineStyleTable(), TreeFiller.DbTreeItem);
        TreeFiller.addElement(Database.getLineStyleTable(), TreeFiller.DbTreeItem);
        TreeFiller.addElement(Database.getLineStyleDefTable(), TreeFiller.DbTreeItem);
        TreeFiller.addElement(Database.getRegAppTable(), TreeFiller.DbTreeItem);
        TreeFiller.addElement(Database.getViewGroupTable(), TreeFiller.DbTreeItem);
        TreeFiller.addElement(Database.getNamedViewTable(), TreeFiller.DbTreeItem);
        TreeFiller.addElement(Database.getModelTable(), TreeFiller.DbTreeItem);
        TreeFiller.addElement(Database.getSharedCellDefinitionTable(), TreeFiller.DbTreeItem);
        TreeFiller.addElement(Database.getTagDefinitionSetTable(), TreeFiller.DbTreeItem);
        TreeFiller.addElement(Database.getColorTable(), TreeFiller.DbTreeItem);
        TreeFiller.addElement(Database.getElementTemplateTable(), TreeFiller.DbTreeItem);
        //TreeFiller.addElement(_db.getDisplayStyleTable(), TreeFiller.DbTreeItem);
        //TreeFiller.addElement(_db.getLinkSetTable(), TreeFiller.DbTreeItem);
        //TreeFiller.addElement(_db.getColorBookTable(), TreeFiller.DbTreeItem);
        TreeFiller.addElement(Database.getNonModelElementCollection(), TreeFiller.DbTreeItem);

        TreeFiller.DbTreeItem.Expand();
      }
      catch (OdError e)
      {
        MessageBox.Show(this, e.Message, "Error reading DB...");
        //theApp.reportError(_T(), e);
      }
    }
    List<VectorizeForm> _vectorizers = new List<VectorizeForm>();
    OdDgElementId _vectorizedModelId = null;
    private void vectorizeToolStripMenuItem_Click(object sender, EventArgs e)
    {
      if (_vectorizers.Count == 0)
      {
        OdDgViewGroup pViewGroup = Database.getActiveViewGroupId().openObject() as OdDgViewGroup;

        _vectorizedModelId = Database.getActiveModelId();
        if (_vectorizedModelId.isNull())
        {
          _vectorizedModelId = Database.getDefaultModelId();
          Database.setActiveModelId(_vectorizedModelId);
        }

        if (pViewGroup == null)
        {
          pViewGroup = Database.recommendActiveViewGroupId().openObject() as OdDgViewGroup;
          if (pViewGroup == null)
            return;
        }

        OdDgElementIterator pIt = pViewGroup.createIterator();
        for (; !pIt.done(); pIt.step())
        {
          OdDgView pView = OdDgView.cast(pIt.item().openObject());
          if (pView != null && pView.getVisibleFlag())
          {
            OdDgElementId _vectorizedViewId = pIt.item();
            var _vectorizer = ((Form1)Parent.Parent).createVectorizationWindow(this.Database, _vectorizedViewId, _vectorizedModelId);
            //set its position
            OdGsDCRect rectangle = pView.getViewRectangle();
            OdGsDCPoint low = rectangle.m_min,
                        hi = rectangle.m_max;
            _vectorizer.Bounds = new Rectangle(low.x, hi.y, hi.x - low.x, low.y - hi.y);
            _vectorizers.Add(_vectorizer);
            break; //At now render one view only
          }
        }
      }
      else
        _vectorizers[0].Activate();
    }

    private void zoomExtentsToolStripMenuItem_Click(object sender, EventArgs e)
    {
      if (_vectorizers.Count == 0)
        return;
      _vectorizers[0].OnZoomToExtents();
    }

    private void Tree_Activated(object sender, EventArgs e)
    {
      ((Form1)Parent.Parent).EnableVectorizerToolButtons(false);
    }

  }
}