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
using OdExGUIComponentsMgdLib.BackCompatibleUIElements;
using Teigha.Core;
using Teigha.TD;

namespace OdaDwgAppSwigMgd
{
  public partial class Tree : Form
  {
    public OdDbDatabase Database { get; private set; }

    private OdExCompatibleSplitContainer splitContainer1;
    public Tree(OdDbDatabase db)
    {
      Database = db;
      InitializeComponent();
      this.splitContainer1 = new OdExCompatibleSplitContainer();
      ((ISupportInitialize)this.splitContainer1).BeginInit();
      this.Controls.Add(splitContainer1);
      this.splitContainer1.Dock = DockStyle.Fill;
      this.splitContainer1.Panel1.Controls.Add(this.treeView);
      this.splitContainer1.Panel2.Controls.Add(this.listView);
      this.treeView.Dock = DockStyle.Fill;
      this.listView.Dock = DockStyle.Fill;
      ((ISupportInitialize)this.splitContainer1).EndInit();
      treeView.AfterSelect += new TreeViewEventHandler(treeView_AfterSelect);

      this.menuStrip1.Visible = false;
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
        OdDbObject pObj = (OdDbObject)TreeFiller.getObject(e.Node);
        if (pObj != null)
        {
          fillDump(pObj);
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
    ListFillerDwg m_pListFillerDwg = null;
    ListFillerDwg ListFillerDwg
    {
      get
      {
        if (m_pListFillerDwg == null)
          m_pListFillerDwg = new ListFillerDwg(Database, listView);
        return m_pListFillerDwg;
      }
    }
    ListFillerDxf m_pListFillerDxf = null;
    ListFillerDxf ListFillerDxf
    {
      get
      {
        if (m_pListFillerDxf == null)
          m_pListFillerDxf = new ListFillerDxf(Database, listView);
        return m_pListFillerDxf;
      }
    }
    DwgTreeFiller m_pTreeFiller = null;
    DwgTreeFiller TreeFiller
    {
      get
      {
        if (m_pTreeFiller == null)
          m_pTreeFiller = new DwgTreeFiller(Database, treeView);
        return m_pTreeFiller;
      }
    }
    public bool Find(TreeNode item, OdDbObjectId id)
    {
      for (; item != null; item = item.NextNode)
      {
        OdDbObjectId data = item.Tag as OdDbObjectId;
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
    void Find(OdDbObjectId targetId)
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
        OdDbObjectId targetId = Database.getOdDbObjectId(han);
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
        TreeFiller.explandItem(TreeFiller.DbTreeItem); //show tables
        TreeFiller.DbTreeItem.Expand();
      }
      catch (OdError e)
      {
        MessageBox.Show(this, e.Message, "Error reading DB...");
      }
    }

    List<VectorizeForm> _vectorizers = new List<VectorizeForm>();

    private void vectorizeToolStripMenuItem_Click(object sender, EventArgs e)
    {
      var _vectorizer = ((Form1)Parent.Parent).createVectorizationWindow(this.Database);
    }

    private void zoomExtentsToolStripMenuItem_Click(object sender, EventArgs e)
    {
      if (_vectorizers.Count == 0)
        return;
      _vectorizers[0].OnZoomToExtents();
    }

    private void Tree_Activated(object sender, EventArgs e)
    {
      ((Form1)Parent.Parent).UpdateRenderFlags(false);
      ((Form1)Parent.Parent).SolveEnableInsertItems(false);
      ((Form1)Parent.Parent).EnableVectorizerToolButtons(false);
    }

    private void fillDump(OdDbObject pObj)
    {
      // if app isDwgOut
      //if (pObj.isErased())
      //	ListFillerDwg.wrString("ERASED");
      //else
      //	pObj.dwgOutFields(ListFillerDwg);
      //else{

      OdResult err;
      ListFillerDxf.wrString(999, "database: " + pObj.database().getFilename());
      pObj.dxfOut(ListFillerDxf);

      if (pObj.isKindOf(OdDbEntity.desc())
        || pObj.isKindOf(OdDbBlockTableRecord.desc())
        || pObj.isKindOf(OdDbLayout.desc()))
      {
        OdGeExtents3d extents = new OdGeExtents3d();
        err = pObj.getGeomExtents(extents);
        if (err == OdResult.eOk)
        {
          ListFillerDxf.wrString(10, "getGeomExtents()");
          ListFillerDxf.wrPoint3d(111, extents.minPoint());
          ListFillerDxf.wrPoint3d(111, extents.maxPoint());
        }
        else
        {
          ListFillerDxf.wrString(10, "getGeomExtents() : " + pObj.database().appServices().getErrorDescription((uint)err));
        }
        if (pObj.isKindOf(OdDbEntity.desc()))
        {
          if (pObj.isKindOf(OdDbBlockReference.desc()))
          {
            ListFillerDxf.wrString(10, ("------------ OdDbBlockReference ------------"));
            ListFillerDxf.wrString(10, ("position()"));
            ListFillerDxf.wrPoint3d(111, ((OdDbBlockReference)pObj).position());
          }

          if (pObj.isKindOf(OdDbRegion.desc()))
          {

            OdGePlane regPlane = new OdGePlane();
            Planarity planarity;
            err = ((OdDbRegion)pObj).getPlane(regPlane, out planarity);
            if (err == OdResult.eOk)
            {
              OdGePoint3d origin = new OdGePoint3d();
              OdGeVector3d xAxis = new OdGeVector3d();
              OdGeVector3d yAxis = new OdGeVector3d(); ;

              regPlane.get(origin, xAxis, yAxis);

              double perimeter, area;
              OdGePoint2d centroid = new OdGePoint2d();
              double[] momInertia = new double[2];
              double prodInertia;
              double[] prinMoments = new double[2];
              OdGeVector2d prinAxes = new OdGeVector2d()/*[2]*/;
              double[] radiiGyration = new double[2];
              OdGePoint2d extentsLow = new OdGePoint2d();
              OdGePoint2d extentsHigh = new OdGePoint2d();

              err = ((OdDbRegion)pObj).getAreaProp(origin, xAxis, yAxis, out perimeter, out area, centroid,
                momInertia, out prodInertia, prinMoments, prinAxes,
                radiiGyration, extentsLow, extentsHigh);

              if (err == OdResult.eOk)
              {
                ListFillerDxf.wrString(10, ("getAreaProp()"));
                ListFillerDxf.wrDouble(40, perimeter);
                ListFillerDxf.wrDouble(40, area);
                ListFillerDxf.wrPoint2d(111, centroid);
                ListFillerDxf.wrDouble(40, momInertia[0]);
                ListFillerDxf.wrDouble(40, momInertia[1]);
                ListFillerDxf.wrDouble(40, prodInertia); ;
                ListFillerDxf.wrDouble(40, prinMoments[0]);
                ListFillerDxf.wrDouble(40, prinMoments[1]);
                ListFillerDxf.wrVector2d(111, prinAxes);
                //ListFillerDxf.wrVector2d(111, prinAxes[0]);
                //ListFillerDxf.wrVector2d(111, prinAxes[1]);
                ListFillerDxf.wrDouble(40, radiiGyration[0]);
                ListFillerDxf.wrDouble(40, radiiGyration[1]);
                ListFillerDxf.wrPoint2d(111, extentsLow);
                ListFillerDxf.wrPoint2d(111, extentsHigh);
              }
            }
            if (err != OdResult.eOk)
            {
              String tmp2 = pObj.database().appServices().getErrorDescription((uint)err);
              String tmp = ("getAreaProp() : ");
              ListFillerDxf.wrString(10, tmp + tmp2);
            }
          }
        }

        OdDbOle2Frame pOle = OdDbOle2Frame.cast(pObj);
        if (pOle != null)
        {
          ListFillerDxf.wrString(10, ("------------ OdDbOle2Frame ------------"));

          ListFillerDxf.wrString(10, ("himetric size:"));
          ListFillerDxf.wrUInt16(70, pOle.unhandled_himetricWidth());
          ListFillerDxf.wrUInt16(71, pOle.unhandled_himetricHeight());

          OdRectangle3d rect = new OdRectangle3d();
          pOle.position(rect);
          ListFillerDxf.wrString(10, ("position:"));
          ListFillerDxf.wrPoint3d(111, rect.upLeft);
          ListFillerDxf.wrPoint3d(111, rect.upRight);
          ListFillerDxf.wrPoint3d(111, rect.lowRight);
          ListFillerDxf.wrPoint3d(111, rect.lowLeft);

          if (pOle.getType() == OdDbOle2Frame.Type.kLink)
          {
            String tmp = ("LinkName: \"" + pOle.getLinkName() + "\"");
            ListFillerDxf.wrString(1, tmp);
            tmp = (("LinkPath: \"" + pOle.getLinkPath() + "\""));
            ListFillerDxf.wrString(1, tmp);
            //SWIGTYPE_p_COleClientItem pCOle = pOle.getOleClientItem();
            //if (pCOle)
            //{
            //	OLEUPDATE update = pCOle.GetLinkUpdateOptions();
            //	switch (update)
            //	{
            //		case OLEUPDATE_ALWAYS:
            //			tmp = L"OLEUPDATE_ALWAYS";
            //			break;
            //		case OLEUPDATE_ONCALL:
            //			tmp = L"OLEUPDATE_ONCALL";
            //			break;
            //		default:
            //			tmp = L"Update Unknown";
            //	}
            //	ListFillerDxf.wrString(1, tmp);
            //}
          }
        }

      }
      //} //else ifDwgOut
    }

  }
}