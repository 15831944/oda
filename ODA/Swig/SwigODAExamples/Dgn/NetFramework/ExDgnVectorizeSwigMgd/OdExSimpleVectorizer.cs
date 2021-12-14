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
using Teigha.TG;

namespace ExDgnVectorizeMgd
{
  class OdExSimpleVectorizer : OdGsBaseVectorizer
  {
    private OdGiClipBoundary m_eyeClip = new OdGiClipBoundary();
    private OdExGsSimpleDevice _device = null;
    public OdExSimpleView _view = null;
    private OdGsView _gsView = null;

    public OdExSimpleVectorizer(OdExGsSimpleDevice d)
    {
      // pay attention - if Memory Transactions are used, newly created custom vectorizer should be added to current transaction
      MemoryTransaction mTr = MemoryManager.GetMemoryManager().GetCurrentTransaction();
      if (null != mTr)
      {
        mTr.AddObject(this);
      }
      _device = d;
      m_eyeClip.m_bDrawBoundary = false;
    }

    public override void setUp(OdGsViewImpl view)
    {
      ((OdExSimpleView)view)._vect = this;
      base.setUp(view);
    }

    public override void beginViewVectorization()
    {
      base.beginViewVectorization();
      _device.setupSimplifier(drawContext().eyeDeviation());
    }

    public OdGsView View
    {
      get
      {
        if (_gsView == null)
          _gsView = gsView();
        return _gsView;
      }
    }

    public override void draw(OdGiDrawable pDrawable)
    {
      OdDgElement pElm = (OdDgElement)pDrawable;

      _device.dumper.output("");
      String sClassName = "<" + pElm.isA().name() + ">";
      String sHandle = pElm.isDBRO() ? pElm.elementId().getHandle().ascii() : "non-DbResident";
      _device.dumper.output("Start Drawing " + sClassName, sHandle);
      _device.dumper.pushIndent();

      /**********************************************************************/
      /* The parent class version of this function must be called.          */
      /**********************************************************************/
      base.draw(pDrawable);

      _device.dumper.popIndent();
      _device.dumper.output("End Drawing " + sClassName, sHandle);
    }

    public override bool regenAbort()
    {
      return false;
    }

    public override double deviation(OdGiDeviationType deviationType, OdGePoint3d pointOnCurve)
    {
      return 0.8; //0.000001; 
    }

    public override void onTraitsModified()
    {
      MemoryTransaction mTr = MemoryManager.GetMemoryManager().StartTransaction();
      OdGiSubEntityTraitsData currTraits = effectiveTraits();
      if (currTraits.trueColor().isByColor())
      {
        _device.draw_color(currTraits.trueColor());
      }
      else
      {
        _device.draw_color_index(currTraits.color());
      }

      LineWeight lw = currTraits.lineWeight();
      _device.draw_lineWidth(lw, view().lineweightToPixels(lw));
      _device.draw_fill_mode(currTraits.fillType());
      MemoryManager.GetMemoryManager().StopTransaction(mTr);
    }

    public override void updateViewport()
    {
      MemoryTransaction mTr = MemoryManager.GetMemoryManager().StartTransaction();
      /**********************************************************************/
      /* If geometry receiver is a simplifier, we must re-set the draw      */
      /* context for it                                                     */
      /**********************************************************************/
      ((OdGiGeometrySimplifier)(_device.destGeometry)).setDrawContext(drawContext());
      /**********************************************************************/
      /* Comment these functions to get primitives in eye coordinates.      */
      /**********************************************************************/
      OdGeMatrix3d eye2Screen = _view.eyeToScreenMatrix();
      OdGeMatrix3d screen2Eye = eye2Screen.inverse();

      setEyeToOutputTransform(eye2Screen);

      /**********************************************************************/
      /* Perform viewport clipping in eye coordinates.                      */
      /**********************************************************************/
      m_eyeClip.m_bClippingFront = _view.isFrontClipped();
      m_eyeClip.m_bClippingBack = _view.isBackClipped();
      m_eyeClip.m_dFrontClipZ = _view.frontClip();
      m_eyeClip.m_dBackClipZ = _view.backClip();
      m_eyeClip.m_vNormal = viewDir();
      m_eyeClip.m_ptPoint = _view.target();
      m_eyeClip.m_Points.Clear();
      OdGeVector2d halfDiagonal = new OdGeVector2d(_view.fieldWidth() / 2.0, _view.fieldHeight() / 2.0);

      // rectangular clipping
      m_eyeClip.m_Points.Add(OdGePoint2d.kOrigin.Sub(halfDiagonal));
      m_eyeClip.m_Points.Add(OdGePoint2d.kOrigin.Add(halfDiagonal));

      m_eyeClip.m_xToClipSpace = getWorldToEyeTransform();

      pushClipBoundary(m_eyeClip);
      base.updateViewport();
      popClipBoundary();
      MemoryManager.GetMemoryManager().StopTransaction(mTr);
    }
  }
}