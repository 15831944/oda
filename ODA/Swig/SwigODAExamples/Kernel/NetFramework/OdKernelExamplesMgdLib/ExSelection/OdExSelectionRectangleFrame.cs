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

using System.Linq;
using Teigha.Core;

namespace OdKernelExamplesMgdLib.ExSelection
{
  public class OdExSelectionRectangleFrame : OdGiDrawable
  {
    private OdGsModel m_model;
    private OdGePoint3dArray m_pts;
    public OdExSelectionRectangleFrame()
    {
      m_pts = new OdGePoint3dArray();
      m_pts.Add(new OdGePoint3d());
      m_pts.Add(new OdGePoint3d());
      m_pts.Add(new OdGePoint3d());
      m_pts.Add(new OdGePoint3d());
    }

    public OdExSelectionRectangleFrame(OdGePoint3d pt)
    {
      m_pts = new OdGePoint3dArray();
      m_pts.Add(pt);
      m_pts.Add(pt);
      m_pts.Add(pt);
      m_pts.Add(pt);
    }

    public override bool isPersistent()
    {
      return false;
    }

    private OdGsCache m_gsNode;

    public override OdGsCache gsNode()
    {
      return m_gsNode;
    }
    public override void setGsNode(OdGsCache pGsNode)
    {
      m_gsNode = pGsNode;
    }

    public override OdDbStub id()
    {
      return null;
    }

    protected override uint subSetAttributes(OdGiDrawableTraits pTraits)
    {
      return (uint)SetAttributesFlags.kDrawableRegenDraw;
    }

    protected override bool subWorldDraw(OdGiWorldDraw wd)
    {
      return false;
    }

    protected override void subViewportDraw(OdGiViewportDraw vd)
    {
      var vp = vd.viewport();
      OdGiViewportGeometry vpg = vd.geometry();
      {
        vd.subEntityTraits().setFillType(OdGiFillType.kOdGiFillNever);

        OdGeMatrix3d mat = vp.getWorldToEyeTransform();
        OdGePoint3d p0 = mat * m_pts[0];
        OdGePoint3d p2 = mat * m_pts[2];
        m_pts[1] = new OdGePoint3d(p0.x, p2.y, p2.z);
        m_pts[3] = new OdGePoint3d(p2.x, p0.y, p2.z);

        mat = vp.getEyeToWorldTransform();
        m_pts[1] = m_pts[1].transformBy(mat);
        m_pts[3] = m_pts[3].transformBy(mat);
        vpg.polygon(m_pts.ToArray());
      }
    }

    public void setValue(OdGePoint3d pt)
    {
      m_pts[2] = pt;
      if (m_model != null)
        m_model.onModified(this, null as OdGiDrawable);
    }

    public OdGePoint3d BasePoint
    {
      get
      {
        return m_pts[0];
      }
    }

    public void AddToView(OdGsView pView)
    {
      if (m_model == null)
      {
        m_model = pView.device().createModel();
        m_model.setRenderType(OdGsModel.RenderType.kDirect);
      }
      pView.add(this, m_model);
    }
  }
}