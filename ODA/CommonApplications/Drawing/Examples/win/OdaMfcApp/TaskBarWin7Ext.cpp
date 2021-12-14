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

#include "StdAfx.h"
#include "TaskBarWin7Ext.h"

static const GUID W7EXT_CLSID_TaskbarList = { 0x56FDF344, 0xFD6D, 0x11D0, { 0x95, 0x8A, 0x00, 0x60, 0x97, 0xC9, 0xA0, 0x90 } };
static const GUID W7EXT_IID_ITaskbarList3 = { 0xEA1AFB91, 0x9E28, 0x4B86, { 0x90, 0xE9, 0x9E, 0x9F, 0x8A, 0x5E, 0xEF, 0xAF } };

/* Copied from ShObjIdl.h (Win7 SDK) to avoid depends from Win7 SDK. */
/* C-style interfaces excluded. RPC definitions excluded.            */

#ifndef __ITaskbarList_INTERFACE_DEFINED__
#define __ITaskbarList_INTERFACE_DEFINED__

/* interface ITaskbarList */

MIDL_INTERFACE("56FDF342-FD6D-11d0-958A-006097C9A090")
ITaskbarList : public IUnknown
{
  public:
    virtual HRESULT STDMETHODCALLTYPE HrInit(void) = 0;
    virtual HRESULT STDMETHODCALLTYPE AddTab(
      /* [in] */ HWND hwnd) = 0;
    virtual HRESULT STDMETHODCALLTYPE DeleteTab(
      /* [in] */ HWND hwnd) = 0;
    virtual HRESULT STDMETHODCALLTYPE ActivateTab(
      /* [in] */ HWND hwnd) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetActiveAlt(
      /* [in] */ HWND hwnd) = 0;
};

#endif /* __ITaskbarList_INTERFACE_DEFINED__ */

#ifndef __ITaskbarList2_INTERFACE_DEFINED__
#define __ITaskbarList2_INTERFACE_DEFINED__

/* interface ITaskbarList2 */

MIDL_INTERFACE("602D4995-B13A-429b-A66E-1935E44F4317")
ITaskbarList2 : public ITaskbarList
{
  public:
    virtual HRESULT STDMETHODCALLTYPE MarkFullscreenWindow(
      /* [in] */ HWND hwnd,
      /* [in] */ BOOL fFullscreen) = 0;
};

#endif /* __ITaskbarList2_INTERFACE_DEFINED__ */

#ifndef __ITaskbarList3_INTERFACE_DEFINED__
#define __ITaskbarList3_INTERFACE_DEFINED__

typedef /* [v1_enum] */ 
enum THUMBBUTTONFLAGS
{
  THBF_ENABLED        = 0,
  THBF_DISABLED       = 0x1,
  THBF_DISMISSONCLICK = 0x2,
  THBF_NOBACKGROUND   = 0x4,
  THBF_HIDDEN         = 0x8,
  THBF_NONINTERACTIVE = 0x10
} THUMBBUTTONFLAGS;

typedef /* [v1_enum] */ 
enum THUMBBUTTONMASK
{
  THB_BITMAP  = 0x1,
  THB_ICON    = 0x2,
  THB_TOOLTIP = 0x4,
  THB_FLAGS   = 0x8
} THUMBBUTTONMASK;

#include <pshpack8.h>
typedef
struct THUMBBUTTON
{
  THUMBBUTTONMASK  dwMask;
  UINT             iId;
  UINT             iBitmap;
  HICON            hIcon;
  WCHAR            szTip[260];
  THUMBBUTTONFLAGS dwFlags;
} THUMBBUTTON;

typedef struct THUMBBUTTON *LPTHUMBBUTTON;
#include <poppack.h>

#define THBN_CLICKED 0x1800

/* interface ITaskbarList3 */

typedef /* [v1_enum] */ 
enum TBPFLAG
{
  TBPF_NOPROGRESS    = 0,
  TBPF_INDETERMINATE = 0x1,
  TBPF_NORMAL        = 0x2,
  TBPF_ERROR         = 0x4,
  TBPF_PAUSED        = 0x8
} TBPFLAG;

MIDL_INTERFACE("ea1afb91-9e28-4b86-90e9-9e9f8a5eefaf")
ITaskbarList3 : public ITaskbarList2
{
  public:
    virtual HRESULT STDMETHODCALLTYPE SetProgressValue(
      /* [in] */ HWND hwnd,
      /* [in] */ ULONGLONG ullCompleted,
      /* [in] */ ULONGLONG ullTotal) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetProgressState(
      /* [in] */ HWND hwnd,
      /* [in] */ TBPFLAG tbpFlags) = 0;
    virtual HRESULT STDMETHODCALLTYPE RegisterTab(
      /* [in] */ HWND hwndTab,
      /* [in] */ HWND hwndMDI) = 0;
    virtual HRESULT STDMETHODCALLTYPE UnregisterTab(
      /* [in] */ HWND hwndTab) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetTabOrder(
      /* [in] */ HWND hwndTab,
      /* [in] */ HWND hwndInsertBefore) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetTabActive(
      /* [in] */ HWND hwndTab,
      /* [in] */ HWND hwndMDI,
      /* [in] */ DWORD dwReserved) = 0;
    virtual HRESULT STDMETHODCALLTYPE ThumbBarAddButtons(
      /* [in] */ HWND hwnd,
      /* [in] */ UINT cButtons,
      /* [size_is][in] */ LPTHUMBBUTTON pButton) = 0;
    virtual HRESULT STDMETHODCALLTYPE ThumbBarUpdateButtons(
      /* [in] */ HWND hwnd,
      /* [in] */ UINT cButtons,
      /* [size_is][in] */ LPTHUMBBUTTON pButton) = 0;
    virtual HRESULT STDMETHODCALLTYPE ThumbBarSetImageList(
      /* [in] */ HWND hwnd,
      /* [in] */ HIMAGELIST himl) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetOverlayIcon(
      /* [in] */ HWND hwnd,
      /* [in] */ HICON hIcon,
      /* [string][unique][in] */ LPCWSTR pszDescription) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetThumbnailTooltip(
      /* [in] */ HWND hwnd,
      /* [string][unique][in] */ LPCWSTR pszTip) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetThumbnailClip(
      /* [in] */ HWND hwnd,
      /* [in] */ RECT *prcClip) = 0;
};

#endif /* __ITaskbarList3_INTERFACE_DEFINED__ */

#ifndef __ITaskbarList4_INTERFACE_DEFINED__
#define __ITaskbarList4_INTERFACE_DEFINED__

/* interface ITaskbarList4 */

typedef /* [v1_enum] */ 
enum STPFLAG
{
  STPF_NONE                      = 0,
  STPF_USEAPPTHUMBNAILALWAYS     = 0x1,
  STPF_USEAPPTHUMBNAILWHENACTIVE = 0x2,
  STPF_USEAPPPEEKALWAYS          = 0x4,
  STPF_USEAPPPEEKWHENACTIVE      = 0x8
} STPFLAG;

MIDL_INTERFACE("c43dc798-95d1-4bea-9030-bb99e2983a1a")
ITaskbarList4 : public ITaskbarList3
{
public:
    virtual HRESULT STDMETHODCALLTYPE SetTabProperties(
      /* [in] */ HWND hwndTab,
      /* [in] */ STPFLAG stpFlags) = 0;
};

#endif /* __ITaskbarList4_INTERFACE_DEFINED__ */

CTaskBarWin7Ext::CTaskBarWin7Ext()
  : m_pTaskBar(NULL)
  , m_bInitialized(false)
  , m_bCoInit(false)
{
}

CTaskBarWin7Ext::~CTaskBarWin7Ext()
{
  EndManagement();
}

bool CTaskBarWin7Ext::StartManagement(bool bCoInit)
{
  if (IsManagement())
    return IsApplicable();
  m_bCoInit = bCoInit;
  if (bCoInit)
    ::CoInitialize(NULL);
  m_bInitialized = true;
  ITaskbarList3 *pTBIface = NULL;
  ::CoCreateInstance(W7EXT_CLSID_TaskbarList, NULL, CLSCTX_ALL, W7EXT_IID_ITaskbarList3, (void**)&pTBIface);
  if (pTBIface != NULL)
    m_pTaskBar = pTBIface;
  return m_pTaskBar != NULL;
}

void CTaskBarWin7Ext::EndManagement()
{
  if (!IsManagement())
    return;
  if (m_pTaskBar != NULL)
  {
    m_pTaskBar->Release();
    m_pTaskBar = NULL;
  }
  if (m_bCoInit)
    ::CoUninitialize();
  m_bInitialized = false;
}

bool CTaskBarWin7Ext::IsManagement() const
{
  return m_bInitialized;
}

bool CTaskBarWin7Ext::IsApplicable() const
{
  return m_pTaskBar != NULL;
}

void CTaskBarWin7Ext::SetProgressState(HWND hWnd, ProgressState state)
{
  if (m_pTaskBar != NULL)
  {
    TBPFLAG state2;
    switch (state)
    {
      case PS_Intermediate:
        state2 = TBPF_INDETERMINATE;
      break;
      case PS_Normal:
        state2 = TBPF_NORMAL;
      break;
      case PS_Error:
        state2 = TBPF_ERROR;
      break;
      case PS_Paused:
        state2 = TBPF_PAUSED;
      break;
      default:
        state2 = TBPF_NOPROGRESS;
    }
    m_pTaskBar->SetProgressState(hWnd, state2);
  }
}

void CTaskBarWin7Ext::SetProgressValue(HWND hWnd, ULONG curVal, ULONG maxVal)
{
  if (m_pTaskBar != NULL)
  {
    m_pTaskBar->SetProgressValue(hWnd, (ULONGLONG)curVal, (ULONGLONG)maxVal);
  }
}

void CTaskBarWin7Ext::FlashWindow(HWND hWnd, bool bInvert)
{
  ::FlashWindow(hWnd, (bInvert) ? TRUE : FALSE);
  // FlashWindowEx provides more features, but them looks like unnecessary for our needs currently.
}

//
