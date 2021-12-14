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
  public class OdExSVGExportParamsAsExportDeviceFiller : OdExSVGExportParamsFiller
  {
    public String OutputPath { get; set; }
    public OdGiDefaultContext GiContext { get; set; }
    public uint[] Pallete { get; set; }
    public uint BackGroundColor { get; set; }

    public static OdExSVGExportParamsAsExportDeviceFiller CreateDefault(OdGiDefaultContext giContext,
      String outputPath,
      uint[] pallete,
      uint backgroundColor)
    {
      OdExSVGExportParamsAsExportDeviceFiller result = new OdExSVGExportParamsAsExportDeviceFiller();
      FillByDefault(result);
      result.GiContext = giContext;
      result.OutputPath = outputPath;
      result.Pallete = pallete;
      result.BackGroundColor = backgroundColor;
      return result;
    }

    public OdGsDevice CreateSvgExportDevice()
    {
      OdGsDevice exportDevice = CreateDevice();

      FillOutput(exportDevice.properties());

      FillLineWeightScale(exportDevice.properties());

      FillPrecision(exportDevice.properties());

      FillImageBase(exportDevice.properties());

      FillImageUrl(exportDevice.properties());

      FillDefaultImageExt(exportDevice.properties());

      FillBackGround(exportDevice.properties());

      FillPallete(exportDevice);

      return PrepareExportDeviceWrapper(exportDevice);
    }

    private OdGsDevice CreateDevice()
    {
      OdGsModule pModule = (OdGsModule)Teigha.Core.Globals.odrxDynamicLinker().loadModule("TD_SvgExport");
      if (null == pModule)
        throw new Exception("TD_SvgExport.tx is missing");

      OdGsDevice exportDevice = pModule.createDevice();
      if (null == exportDevice)
        throw new Exception("Svg export device not created");

      return exportDevice;
    }

    private void FillOutput(OdRxDictionary properties)
    {
      if (String.IsNullOrEmpty(OutputPath))
        throw new Exception("OutputPath is Empty");

      OdStreamBuf file = Globals.odrxSystemServices().createFile(OutputPath, FileAccessMode.kFileWrite, FileShareMode.kShareDenyNo, FileCreationDisposition.kCreateAlways);

      if (null == file)
        throw new Exception("Target file not created in file system");

      properties.putAt("Output", file);
    }

    private void FillLineWeightScale(OdRxDictionary properties)
    {
      // size of pixel in device units
      properties.putAt("LineWeightScale", new OdRxVariantValue(LineWeightScale));
    }

    private void FillPrecision(OdRxDictionary properties)
    {
      // size of pixel in device units
      properties.putAt("Precision", new OdRxVariantValue(FloatingPointPrecision));
    }

    private void FillImageBase(OdRxDictionary properties)
    {
      if (String.IsNullOrEmpty(OutputPath))
        throw new Exception("OutputPath is Empty");
      // where copy images
      String s_base = OutputPath;
      s_base = s_base.Substring(0, s_base.LastIndexOf("\\"));
      properties.putAt("ImageBase", new OdRxVariantValue(s_base));
    }

    private void FillImageUrl(OdRxDictionary properties)
    {
      // prefix to prepend to image name
      properties.putAt("ImageUrl", new OdRxVariantValue("./"));
    }

    private void FillDefaultImageExt(OdRxDictionary properties)
    {
      if (String.IsNullOrEmpty(DefaultImageExt))
        throw new Exception("DefaultImageExt is Empty");
      // default image format
      properties.putAt("DefaultImageExt", new OdRxVariantValue(DefaultImageExt));
    }

    private void FillBackGround(OdRxDictionary properties)
    {
      properties.putAt("BackGround", new OdRxVariantValue(BackGroundColor));
    }

    private void FillPallete(OdGsDevice exportDevice)
    {
      if (exportDevice == null)
        throw new Exception("exportDevice == null");
      if (Pallete == null)
        throw new Exception("Pallete == null");
      Pallete[255] = BackGroundColor;
      exportDevice.setLogicalPalette(Pallete, 256);
      exportDevice.setBackgroundColor(Pallete[255]);
    }

    private OdGsDevice PrepareExportDeviceWrapper(OdGsDevice exportDevice)
    {
      if (exportDevice == null)
        throw new Exception("exportDevice == null");
      if (GiContext == null)
        throw new Exception("GiContext == null");
      if (OutputWidth <= 0 || OutputHeight <= 0)
        throw new Exception("OutputWidth <= 0 || OutputHeight<=0");

      // Prepare the device to render the active layout in this database.
      OdDbBaseDatabasePE pDbPE = OdDbBaseDatabasePE.cast(GiContext.database());
      OdGsDevice wrapper = pDbPE.setupActiveLayoutViews(exportDevice, GiContext);
      // Setup device coordinate space
      wrapper.onSize(new OdGsDCRect(0, (int)OutputWidth, (int)OutputHeight, 0));
      return wrapper;
    }
  }
}