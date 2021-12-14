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

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef H_WIN7_TASKBAR_EXTENSIONS
#define H_WIN7_TASKBAR_EXTENSIONS

struct ITaskbarList3;

// class: CTaskBarWin7Ext
class CTaskBarWin7Ext
{
  public:
    enum ProgressState
    {
      PS_NoProgress = 0,
      PS_Intermediate,
      PS_Normal,
      PS_Error,
      PS_Paused
    };
  protected:
    ITaskbarList3 *m_pTaskBar;
    bool m_bInitialized;
    bool m_bCoInit;
  public:
    CTaskBarWin7Ext();
    ~CTaskBarWin7Ext();

    bool StartManagement(bool bCoInit = true);
    void EndManagement();

    bool IsManagement() const;
    bool IsApplicable() const;

    void SetProgressState(HWND hWnd, ProgressState state);
    void SetProgressValue(HWND hWnd, ULONG curVal, ULONG maxVal);

    /* NYI */
    ProgressState GetProgressState(HWND /*hWnd*/) const { return PS_NoProgress; }
    ULONG GetCurrentProgress(HWND /*hWnd*/) const { return 0; }
    ULONG GetMaxProgressVal(HWND /*hWnd*/) const { return 0; }
    /* --- */

    void FlashWindow(HWND hWnd, bool bInvert = true);
};
// end of class: CTaskBarWin7Ext

#endif // H_WIN7_TASKBAR_EXTENSIONS
