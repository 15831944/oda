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
using System.Text;
using Teigha.Core;
using OdKernelExamplesMgdLib.Export.Svg;

namespace OdKernelExamplesMgdLib.Export.Svg
{
  public static class OdExSvgExport
  {
    public static void Export(OdGiDefaultContext giContext,
      String outputPath,
      uint[] pallete,
      uint backgroundColor)
    {
      MemoryTransaction mTr = MemoryManager.GetMemoryManager().StartTransaction();
      try
      {
        OdGsModule pModule = (OdGsModule)Teigha.Core.Globals.odrxDynamicLinker().loadModule("TD_SvgExport");
        if (null == pModule)
          throw new System.Exception("Module TD_SvgExport.tx is missing");

        OdExSVGExportParamsAsExportDeviceFiller exSvgParamsFiller =
          OdExSVGExportParamsAsExportDeviceFiller.CreateDefault(giContext, outputPath, pallete, backgroundColor);

        using (OdGsDevice exportDevice = exSvgParamsFiller.CreateSvgExportDevice())
        {
          exportDevice.update();
        }
      }
      catch (Exception err)
      {
        throw err;
      }
      finally
      {
        MemoryManager.GetMemoryManager().StopTransaction(mTr);
      }
    }
  }
}