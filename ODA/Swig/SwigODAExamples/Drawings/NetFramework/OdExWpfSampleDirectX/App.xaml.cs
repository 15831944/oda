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

using Microsoft.Win32;
using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.Diagnostics;
using System.Linq;
using System.Windows;
using System.Windows.Interop;

namespace OdExWpfSampleDirectX
{
  /// <summary>
  /// Interaction logic for App.xaml
  /// </summary>
  public partial class App : Application
  {
    protected override void OnStartup(StartupEventArgs e)
    {
      if (ForceSoftwareRendering)
        System.Windows.Media.RenderOptions.ProcessRenderMode = RenderMode.SoftwareOnly;
      base.OnStartup(e);
    }

    public bool ForceSoftwareRendering
    {
      get
      {
        bool bDisableHWAcceleration;
        {
          const string pathToDisableHWAcceleration = @"HKEY_CURRENT_USER\SOFTWARE\Microsoft\Avalon.Graphics";
          const string property = "DisableHWAcceleration";

          int value_DisableHWAcceleration = (int)Registry.GetValue(pathToDisableHWAcceleration, property, -1);

          if (value_DisableHWAcceleration == -1)
            bDisableHWAcceleration = false;
          else if (value_DisableHWAcceleration == 0)
            bDisableHWAcceleration = false;
          else if (value_DisableHWAcceleration == 1)
            bDisableHWAcceleration = true;
          else
            throw new Exception("Unexpected value in DisableHWAcceleration");
          Debug.WriteLine("OdExWpfSampleDirectX: DisableHWAcceleration is {0}", bDisableHWAcceleration);
        }

        int renderingTier = (System.Windows.Media.RenderCapability.Tier >> 16);

        Debug.WriteLine("OdExWpfSampleDirectX: RenderingTier is {0}", renderingTier);

        bool ForceSoftwareRendering = renderingTier == 0 || bDisableHWAcceleration;

        Debug.WriteLine("OdExWpfSampleDirectX: ForceSoftwareRendering is {0}", ForceSoftwareRendering);

        return ForceSoftwareRendering;
      }
    }
  }
}
