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
using OdKernelExamplesMgdLib.ExExecuteCommands;
using OdKernelExamplesMgdLib.Export.Svg;

namespace OdDrawingsExamplesMgdLib.Export.Svg
{
  public class OdExSVGExportParamsAsCmdFiller : OdExSVGExportParamsFiller
  {
    public String GenerateCommandLine()
    {
      String result = String.Format("svgout {0} {1} {2} {3} {4} {5} {6} {7} {8} {9} {10}",
        OdExCommandHelpers.DoubleToStringCommaNormalize(LineWeightScale),
        OdExCommandHelpers.DoubleToStringCommaNormalize(FloatingPointPrecision),
        String.Concat(ImageBase, "\n"),
        String.Concat(ImageUrl, "\n"),
        DefaultImageExt, GenericFontFamily,
        OdExCommandHelpers.DoubleToStringCommaNormalize(ScaleFactor),
        OdExCommandHelpers.DoubleToStringCommaNormalize(OutputHeight),
        OdExCommandHelpers.DoubleToStringCommaNormalize(OutputWidth),
        OdExCommandHelpers.BooleanToStringNormalize_YesNo(UseSoftHLR),
        OdExCommandHelpers.BooleanToStringNormalize_YesNo(EnableGouraudShading));
      return result;
    }

    public static OdExSVGExportParamsAsCmdFiller CreateDefault()
    {
      OdExSVGExportParamsAsCmdFiller result = new OdExSVGExportParamsAsCmdFiller();
      FillByDefault(result);
      return result;
    }
  }
}