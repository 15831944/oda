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
using Teigha.Core;

namespace OdKernelExamplesMgdLib.Export.Svg
{
  public abstract class OdExSVGExportParamsFiller
  {
    public double LineWeightScale { get; set; }
    public int FloatingPointPrecision { get; set; }
    public String ImageBase { get; set; }
    public String ImageUrl { get; set; }
    public String DefaultImageExt { get; set; }
    public String GenericFontFamily { get; set; }
    public double ScaleFactor { get; set; }
    public double OutputHeight { get; set; }
    public double OutputWidth { get; set; }
    public bool UseSoftHLR { get; set; }
    public bool EnableGouraudShading { get; set; }

    protected OdExSVGExportParamsFiller()
    {
      LineWeightScale = 1.0;
      FloatingPointPrecision = 6;
      ImageBase = String.Format("{0}", String.Empty);
      ImageUrl = String.Format("{0}", String.Empty);
      DefaultImageExt = ".png";
      GenericFontFamily = "sans-serif";
      ScaleFactor = 0.0;
      OutputWidth = 1024;
      OutputHeight = 768;
      UseSoftHLR = true;
      EnableGouraudShading = true;
    }

    protected static void FillByDefault(OdExSVGExportParamsFiller instance)
    {
      instance.LineWeightScale = 1.0;
      instance.FloatingPointPrecision = 6;
      instance.ImageBase = String.Format("{0}", String.Empty);
      instance.ImageUrl = String.Format("{0}", String.Empty);
      instance.DefaultImageExt = ".png";
      instance.GenericFontFamily = "sans-serif";
      instance.ScaleFactor = 0;
      instance.OutputHeight = 768;
      instance.OutputWidth = 1024;
      instance.UseSoftHLR = true;
      instance.EnableGouraudShading = true;
    }
  }
}