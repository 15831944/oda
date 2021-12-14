/////////////////////////////////////////////////////////////////////////////// 
// Copyright (C) 2002-2020, Open Design Alliance (the "Alliance"). 
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
//   Open Design Alliance Copyright (C) 2002-2020 by Open Design Alliance. 
//   All rights reserved.
//
// By use of this software, its documentation or related materials, you 
// acknowledge and accept the above terms.
///////////////////////////////////////////////////////////////////////////////

namespace OdKernelExamplesMgdLib
{
  public static class OdExColorHelpers
  {
    public static uint ODRGB(short r, short g, short b)
    {
      return (uint)((b << 16) |
                    (g << 8) | (r << 0));
    }

    public static uint ODRGBA(short r, short g, short b, short a)
    {
      return (uint)((a << 24) | (b << 16) |
                    (g << 8) | (r << 0));
    }

    public static uint ODGETRED(uint rgb)
    {
      return ((byte)(rgb));
    }

    public static uint ODGETGREEN(uint rgb)
    {
      return ((byte)(((ushort)(rgb)) >> 8));
    }

    public static uint ODGETBLUE(uint rgb)
    {
      return ((byte)((rgb) >> 16));
    }

    public static uint ODGETALPHA(uint rgba)
    {
      return ((byte)((rgba) >> 24));
    }
  }
}