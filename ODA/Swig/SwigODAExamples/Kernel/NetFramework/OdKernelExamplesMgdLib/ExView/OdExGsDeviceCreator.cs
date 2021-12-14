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

namespace OdKernelExamplesMgdLib.ExView
{
  public static class OdExGsDeviceCreator
  {
    /// <summary>
    /// Enumeration ODA SDK graphics module types for .NET examples
    /// </summary>
    public enum OdExGsModuleType
    {
      None,
      WinGLES2,
      WinOpenGL,
      WinDirectX,
      WinGDI,
      WinComposite,
      WinBitmap,
    }

    public static OdGsDevice CreateGsDeviceFromGsModule(OdExGsModuleType gsModuleType, IntPtr HWND, IntPtr HDC)
    {
      OdGsDevice gsDevice;
      switch (gsModuleType)
      {
        case OdExGsModuleType.WinGLES2: gsDevice = OdExGsDeviceCreator.Create_WinGLES2_GsDevice(HWND, HDC); break;
        case OdExGsModuleType.WinOpenGL: gsDevice = OdExGsDeviceCreator.Create_WinOpenGL_GsDevice(HWND, HDC); break;
        case OdExGsModuleType.WinDirectX: gsDevice = OdExGsDeviceCreator.Create_WinDirectX_GsDevice(HWND, HDC); break;
        case OdExGsModuleType.WinGDI: gsDevice = OdExGsDeviceCreator.Create_WinGDI_GsDevice(HWND, HDC); break;
        case OdExGsModuleType.None: throw new Exception("GsModuleType not selected");
        default: throw new Exception("GsModuleType not implemented");
      }
      return gsDevice;
    }

    public static OdGsDevice Create_WinGLES2_GsDevice(IntPtr ControlHWND, IntPtr ControHDC)
    {
      return CreateGsDevice("WinGLES2.txv", ControlHWND, ControHDC);
    }

    public static OdGsDevice Create_WinGDI_GsDevice(IntPtr ControlHWND, IntPtr ControHDC)
    {
      return CreateGsDevice("WinGDI.txv", ControlHWND, ControHDC);
    }

    public static OdGsDevice Create_WinDirectX_GsDevice(IntPtr ControlHWND, IntPtr ControHDC)
    {
      return CreateGsDevice("WinDirectX.txv", ControlHWND, ControHDC);
    }

    public static OdGsDevice Create_WinOpenGL_GsDevice(IntPtr ControlHWND, IntPtr ControHDC)
    {
      return CreateGsDevice("WinOpenGL.txv", ControlHWND, ControHDC);
    }

    public static OdGsDevice CreateGsDevice(String gsModuleName, IntPtr ControlHWND, IntPtr ControHDC)
    {
      // Create a new OdGsDevice object by spec graphic module
      OdGsDevice device = CreateGsDeviceFromGsModule(gsModuleName);

      //Default initialization OdGsDevice Properties
      DefaultSetupPropertiesGsDevice(device, ControlHWND, ControHDC);

      return device;
    }

    public static OdGsDevice CreateGsDeviceFromGsModule(String gsModuleName)
    {
      OdGsModule pGs = null;
      try
      {
        // Load the vectorization module
        pGs = (OdGsModule)Globals.odrxDynamicLinker().loadModule(gsModuleName, false);
      }
      catch (Exception err)
      {
        throw new Exception(String.Format("Module {0} missing", gsModuleName));
      }

      // Create a new OdGsDevice object, and associate with the vectorization GsDevice
      OdGsDevice pDevice = pGs.createDevice();
      return pDevice;
    }

    public static void DefaultSetupPropertiesGsDevice(OdGsDevice pDevice, IntPtr HWND, IntPtr HDC)
    {
      // Return a pointer to the dictionary entity containing the device properties
      OdRxDictionary Properties = pDevice.properties();

      if (Properties != null)
      {
        if (Properties.has("WindowHWND") && HWND != IntPtr.Zero) // Check if property is supported
          Properties.putAt("WindowHWND", new OdRxVariantValue(HWND));
        if (Properties.has("WindowHDC") && HDC != IntPtr.Zero) // Check if property is supported
          Properties.putAt("WindowHDC", new OdRxVariantValue(HDC)); // hWindowDC necessary for Bitmap device
        if (Properties.has("DoubleBufferEnabled")) // Check if property is supported
          Properties.putAt("DoubleBufferEnabled", new OdRxVariantValue(true));
        if (Properties.has("DiscardBackFaces")) // Check if property is supported
          Properties.putAt("DiscardBackFaces", new OdRxVariantValue(true));
        // Enable scene graph usage onto renderer side
        if (Properties.has("UseSceneGraph")) // Check if property is supported
          Properties.putAt("UseSceneGraph", new OdRxVariantValue(true));
        if (Properties.has("UseVisualStyles")) // Check if property is supported
          Properties.putAt("UseVisualStyles", new OdRxVariantValue(true));
      }
    }
  }

}