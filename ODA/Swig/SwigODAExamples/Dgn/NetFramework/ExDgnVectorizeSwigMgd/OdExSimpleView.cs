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
  class OdExSimpleView : OdGsBaseVectorizeView
  {
    private OdGiClipBoundary m_eyeClip = new OdGiClipBoundary();
    private OdExGsSimpleDevice _device = null;
    public OdExSimpleVectorizer _vect = null;

    public OdExSimpleView(OdExGsSimpleDevice d)
    {
      _device = d;
    }

    protected override OdGsBaseVectorizer getVectorizer(bool bDisplay)
    {
      return _vect;
    }

    public override OdGsDevice device()
    {
      return _device;
    }

    public void ownerDrawDc(OdGePoint3d origin, OdGeVector3d u, OdGeVector3d v, OdGiSelfGdiDrawable pDrawable, bool dcAligned, bool allowClipping)
    {
      MemoryTransaction mTr = MemoryManager.GetMemoryManager().StartTransaction();
      // ownerDrawDc is not conveyor primitive. It means we must take all rendering processings
      // (transforms) by ourselves
      OdGeMatrix3d eyeToOutput = _vect.eyeToOutputTransform();
      OdGePoint3d originXformed = new OdGePoint3d(eyeToOutput * origin);
      OdGeVector3d uXformed = new OdGeVector3d(eyeToOutput * u);
      OdGeVector3d vXformed = new OdGeVector3d(eyeToOutput * v);

      OdExGiDumper pDumper = _device.dumper;

      pDumper.output("ownerDrawDc");

      // It's shown only in 2d mode.
      if (mode() == RenderMode.k2DOptimized)
      {
        pDumper.pushIndent();
        pDumper.output("origin xformed", originXformed.ToString());
        pDumper.output("u xformed", uXformed.ToString());
        pDumper.output("v xformed", vXformed.ToString());
        pDumper.output("dcAligned", dcAligned.ToString());//
        pDumper.output("allowClipping", allowClipping.ToString());
        pDumper.popIndent();
      }
      MemoryManager.GetMemoryManager().StopTransaction(mTr);
    }

    protected override void releaseVectorizer(OdGsBaseVectorizer pVect)
    {
      // override is needed, otherwise an attempt to call a pure virtual method will be performed
    }
  }
}