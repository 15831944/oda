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
using System.Windows;
using System.Windows.Interop;
using System.Windows.Media;
using Teigha.Core;

namespace OdExWpfSampleDirectX
{
  /// <summary>
  /// This example working with D3DSurface from OdGsDevice
  /// </summary>
  public class OdExRenderD3DImage : D3DImage
  {
    public OdExRenderD3DImage()
    {
    }

    /// <summary>
    /// Target OdGsDevice
    /// </summary>
    private OdGsDevice m_GsDevice;

    /// <summary>
    /// Current D3DSurface object
    /// </summary>
    private OdRxObject m_D3DSurfaceObject;

    /// <summary>
    /// Method set target OdGsDevice
    /// </summary>
    /// <param name="gsDevice"></param>
    public void Initialize(OdGsDevice gsDevice)
    {
      m_GsDevice = gsDevice;
    }

    /// <summary>
    /// Method update D3DSurface to D3DImage back buffer
    /// </summary>
    public void UpdateSurface()
    {
      if (m_GsDevice == null)
        return;

      if (!IsSoftwareOnlyRenderMode() && !this.IsFrontBufferAvailable)
        return;

      UpdateBackBuffer();
    }

    /// <summary>
    /// Method release D3DSurface & clear back buffer in D3DImage
    /// </summary>
    public void ReleaseSurface()
    {
      ClearBackBuffer();
      ReleaseRxObjectSurface();
    }

    /// <summary>
    /// Method release D3DSurface, clear back buffer in D3DImage & reset internal variables
    /// </summary>
    public void UnInitializeSurface()
    {
      if (!IsSoftwareOnlyRenderMode() && !this.IsFrontBufferAvailable)
        return;

      if (m_GsDevice == null)
        return;

      ClearBackBuffer();

      ReleaseRxObjectSurface();

      m_GsDevice = null;
      m_D3DSurfaceObject = null;
    }

    /// <summary>
    /// Update back buffer D3DImage
    /// </summary>
    private void UpdateBackBuffer()
    {
      //Update D3DSurface object
      UpdateD3DSurfaceObject();

      //Get D3DSurface object pointer
      IntPtr surface = GetRxObjectSurfacePtr();

      //Set D3DSurface to back buffer D3DImage
      SetD3DSurfacePtrToBackBuffer(surface);
    }

    /// <summary>
    /// Clear back buffer
    /// </summary>
    private void ClearBackBuffer()
    {
      this.Lock();
      SetBackBuffer(IntPtr.Zero);
      this.Unlock();
    }

    /// <summary>
    /// Set D3DSurface to back buffer D3DImage
    /// </summary>
    /// <param name="surface"></param>
    private void SetD3DSurfacePtrToBackBuffer(IntPtr surface)
    {
      if (surface == IntPtr.Zero)
        return;

      this.Lock();

      SetBackBuffer(surface);

      this.Unlock();

      UpdateDirtyRect();
    }

    private void SetBackBuffer(IntPtr ptr)
    {
      SetBackBufferMethod().Invoke(this, new object[] { D3DResourceType.IDirect3DSurface9, ptr, IsSoftwareOnlyRenderMode() });
    }

    /// <summary>
    /// For compatible with .NET Framework 4
    /// </summary>
    /// <returns></returns>
    private static System.Reflection.MethodInfo SetBackBufferMethod()
    {
      if (fSetBackBuffer != null)
        return fSetBackBuffer;
      fSetBackBuffer = typeof(OdExRenderD3DImage).GetMethod("SetBackBuffer", new Type[] { typeof(D3DResourceType), typeof(IntPtr), typeof(bool) });
      return fSetBackBuffer;
    }

    private static System.Reflection.MethodInfo fSetBackBuffer = null;

    /// <summary>
    /// Update D3DSurface Object from OdGsDevice.
    /// </summary>
    private void UpdateD3DSurfaceObject()
    {
      OdRxObject newSurface = getD3DSurface();

      if (newSurface == null)
        return;

      m_D3DSurfaceObject = newSurface;
    }

    private void ReleaseRxObjectSurface()
    {
      if (m_D3DSurfaceObject != null)
      {
        OdRxObject.ReleasePointer(m_D3DSurfaceObject);
        m_D3DSurfaceObject = null;
      }
    }

    /// <summary>
    /// Get D3DSurface object from OdGsDevice.
    /// Before get object need do OdGsDevice.update(), so that update property D3DSurface inside OdGsDevice
    /// Note: Before execute OdGsDevice.update() need release received earlier D3DSurface
    /// </summary>
    private OdRxObject getD3DSurface()
    {
      if (!m_GsDevice.properties().has("D3DSurface"))
        return null;
      OdRxObject obj = m_GsDevice.properties().getAt("D3DSurface");
      return obj;
    }

    /// <summary>
    /// Get D3DSurface native pointer
    /// </summary>
    /// <returns></returns>
    private IntPtr GetRxObjectSurfacePtr()
    {
      IntPtr surface = IntPtr.Zero;
      if (m_D3DSurfaceObject == null)
        return surface;

      using (OdRxVariantValue surfaceVariantValue = new OdRxVariantValue(m_D3DSurfaceObject))
      {
        if (IntPtr.Size == 8)
          surface = (IntPtr)surfaceVariantValue.AsInt64();
        else
          surface = (IntPtr)surfaceVariantValue.AsInt32();
      }
      return surface;
    }

    /// <summary>
    /// Specifies the area of the back buffer that changed.
    /// </summary>
    private void UpdateDirtyRect()
    {
      Int32Rect updateRect = new Int32Rect();
      updateRect.X = updateRect.Y = 0;

      this.Lock();
      updateRect.Width = this.PixelWidth;
      updateRect.Height = this.PixelHeight;
      this.AddDirtyRect(updateRect);
      this.Unlock();
    }

    private bool IsSoftwareOnlyRenderMode()
    {
      return RenderOptions.ProcessRenderMode == System.Windows.Interop.RenderMode.SoftwareOnly;
    }
  }
}
