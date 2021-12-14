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
using System.IO;
using System.Collections.Generic;

using System.Text;
using System.Runtime.InteropServices;
using Teigha.Core;
using Teigha.TG;

namespace ExDgnVectorizeMgd
{
  public abstract class OdExGiDumper
  {
    public abstract void output(string label, OdGeMatrix3d xfm);
    public abstract void output(string str);
    public abstract void output(string str1, string str2);
    public abstract void output(OdGePoint3d[] points);
    public abstract void output(OdGiTextStyle textStyle);
    public abstract void outputEdgeData(Teigha.Core.EdgeData pEdgeData);
    public abstract void outputFaceData(Teigha.Core.FaceData pFaceData);
    public abstract void outputVertexData(Teigha.Core.VertexData pVertexData);
    public abstract void outputColors(UInt16[] colors, string name);
    public abstract void outputTrueColors(OdCmEntityColor[] trueColors, string name);
    public abstract void outputIds(OdDbStub[] ids, string name, string table);
    public abstract void outputSelectionMarkers(IntPtr[] selectionMarkers, string name);
    public abstract void outputVisibility(byte[] visibility, string name);
    public abstract void pushIndent();
    public abstract void popIndent();
  }

}