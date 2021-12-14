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


using OdKernelExamplesMgdLib;
using System.Windows.Input;

namespace OdKernelExamplesUIMgdLib.Helpers
{
  public static class OdExWpfMouseHelper
  {
    public static OdExMouseButton ConvertMouseButtons(System.Windows.Input.MouseEventArgs button)
    {
      OdExMouseButton result = OdExMouseButton.None;

      if (button.LeftButton == MouseButtonState.Pressed)
        result |= OdExMouseButton.Left;
      if (button.RightButton == MouseButtonState.Pressed)
        result |= OdExMouseButton.Right;
      if (button.MiddleButton == MouseButtonState.Pressed)
        result |= OdExMouseButton.Middle;
      if (button.XButton1 == MouseButtonState.Pressed)
        result |= OdExMouseButton.XButton1;
      if (button.XButton2 == MouseButtonState.Pressed)
        result |= OdExMouseButton.XButton2;

      return result;
    }

    public static OdExMouseButton ConvertMouseButtons(MouseButton button)
    {
      switch (button)
      {
        case MouseButton.Left: return OdExMouseButton.Left;
        case MouseButton.Middle: return OdExMouseButton.Middle;
        case MouseButton.Right: return OdExMouseButton.Right;
        case MouseButton.XButton1: return OdExMouseButton.XButton1;
        case MouseButton.XButton2: return OdExMouseButton.XButton2;
        default: return OdExMouseButton.None;
      }
    }
  }
}