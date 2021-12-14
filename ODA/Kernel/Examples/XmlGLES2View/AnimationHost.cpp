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
// Animation host implementation

#include "OdaCommon.h"
#include "UInt32Array.h"
#include "AnimationHost.h"
#include "resource.h"

#define WM_EXREACT (WM_USER + 1)
#define WM_EXFINAL (WM_USER + 2)

class MenuBarRestorator
{
  protected:
    HMENU m_hMenu;
    OdUInt32Array m_menuItems;
  public:
    MenuBarRestorator(HWND hWnd = NULL)
      : m_hMenu(NULL) { if (hWnd) disable(hWnd); }
    void disable(HWND hWnd)
    {
      m_hMenu = ::GetMenu(hWnd);
      disableSubMenu(m_hMenu);
    }
    void enable()
    {
      const OdUInt32 *pItems = m_menuItems.getPtr();
      OdUInt32 nItems = m_menuItems.size();
      for (OdUInt32 nItem = 0; nItem < nItems; nItem++)
        ::EnableMenuItem(m_hMenu, (UINT)pItems[nItem], MF_BYCOMMAND | MF_ENABLED);
      m_hMenu = NULL;
      m_menuItems.setPhysicalLength(0);
    }
  protected:
    void disableSubMenu(HMENU hMenu)
    {
      const int nItems = ::GetMenuItemCount(hMenu);
      for (int nItem = 0; nItem < nItems; nItem++)
      {
        UINT state = ::GetMenuState(hMenu, (UINT)nItem, MF_BYPOSITION);
        if (GETBIT(state, MF_POPUP))
          disableSubMenu(::GetSubMenu(hMenu, nItem));
        else if (!GETBIT(state, MF_SEPARATOR | MF_GRAYED))
        {
          UINT id = ::GetMenuItemID(hMenu, nItem);
          if ((id != 0xFFFFFFFF) && (id != IDC_MENU_QUIT))
          {
            m_menuItems.push_back((OdUInt32)id);
            ::EnableMenuItem(hMenu, id, MF_BYCOMMAND | MF_GRAYED);
          }
        }
      }
    }
};

class AnimationHostImpl : public AnimationHost
{
  protected:
    HWND m_hOwner, m_hDlg;
    HANDLE m_animFiniteGate, m_animInfiniteGate;
    HANDLE m_threadId; OdUInt32 m_nTiming;
  public:
    AnimationHostImpl(HWND hWnd)
      : m_hOwner(hWnd), m_hDlg(NULL), m_animFiniteGate(NULL), m_animInfiniteGate(NULL), m_threadId(NULL)
      , m_nTiming(0)
    {
      m_animFiniteGate   = ::CreateEvent(NULL, TRUE,  FALSE, NULL);
      m_animInfiniteGate = ::CreateEvent(NULL, FALSE, FALSE, NULL);
    }
    ~AnimationHostImpl()
    {
      ::CloseHandle(m_animFiniteGate);
      ::CloseHandle(m_animInfiniteGate);
    }
    HANDLE threadId() { return m_threadId; }

    struct ThreadLoopParam { AnimationHostImpl *m_pThis; AnimationClient *m_pClient; bool m_bFinalized; MenuBarRestorator m_menuBar;
      ThreadLoopParam(AnimationHostImpl *pThis, AnimationClient *pClient) : m_pThis(pThis), m_pClient(pClient), m_bFinalized(false) { } };
    static DWORD WINAPI winThreadLoop(LPVOID lpThreadParameter)
    { ThreadLoopParam param(*reinterpret_cast<ThreadLoopParam*>(lpThreadParameter));
      while (param.m_pClient->process(param.m_pThis)) { }
      param.m_pThis->finalize(param.m_pClient); return EXIT_SUCCESS;
    }
    void runAnimationLoop(AnimationClient *pClient)
    { 
      ThreadLoopParam *pParam = new ThreadLoopParam(this, pClient);
      // Create dialog
      m_hDlg = ::CreateDialogParam(NULL, MAKEINTRESOURCE(IDD_ANIMATION), m_hOwner, dlgClientProc, (LPARAM)pParam);
      { RECT ownerRect, clientRect;
        ::GetWindowRect(m_hOwner, &ownerRect); ::GetWindowRect(m_hDlg, &clientRect);
        ::MoveWindow(m_hDlg, ownerRect.right - (clientRect.right - clientRect.left) - ::GetSystemMetrics(SM_CXFRAME) * 2,
                     ownerRect.bottom - (clientRect.bottom - clientRect.top) - ::GetSystemMetrics(SM_CYFRAME) * 2,
                     clientRect.right - clientRect.left, clientRect.bottom - clientRect.top, FALSE);
      }
      ::ShowWindow(m_hDlg, SW_SHOWNORMAL);
      ::UpdateWindow(m_hDlg);
      // Create thread
      m_threadId = ::CreateThread(NULL, 0, winThreadLoop, pParam, 0, NULL);
    }

    void closeGate(OdUInt32 nInterval)
    {
      runTiming(nInterval);
      ::WaitForSingleObject(m_animFiniteGate, nInterval);
      stopTiming();
      ::WaitForSingleObject(m_animInfiniteGate, INFINITE);
      ::ResetEvent(m_animFiniteGate);
    }
    void openGate(bool bForce)
    {
      if (bForce)
        ::SetEvent(m_animFiniteGate);
      ::SetEvent(m_animInfiniteGate);
    }

    void resetDlg(HWND hDlg)
    {
      m_hDlg = hDlg;
    }
    void updateStatus(const char *pStatus)
    {
      if (m_hDlg)
        ::SetDlgItemTextA(m_hDlg, IDC_SSTATUS, pStatus);
    }

    void processThreadCallback(AnimationClient *pCallback)
    {
      if (m_hDlg)
        ::PostMessage(m_hDlg, WM_EXREACT, 0, (LPARAM)pCallback);
    }
    void finalize(AnimationClient *pClient)
    {
      if (m_hDlg)
        ::PostMessage(m_hDlg, WM_EXFINAL, 0, (LPARAM)0);
    }

    void displayTiming(bool bDisplayTiming = true)
    {
      if (!bDisplayTiming)
        ::SetDlgItemTextA(m_hDlg, IDC_STIMING, "");
      else
      {
        OdUInt32 nMsec = m_nTiming % 1000;
        OdUInt32 nSec = m_nTiming / 1000;
        OdUInt32 nMin = nSec / 60; nSec %= 60;
        ::SetDlgItemTextA(m_hDlg, IDC_STIMING, OdAnsiString().format("%02u:%02u:%03u", (unsigned)nMin, (unsigned)nSec, (unsigned)nMsec).c_str());
      }
    }
    void runTiming(OdUInt32 nTiming)
    {
      m_nTiming = nTiming;
      displayTiming();
      ::SetTimer(m_hDlg, 100, 1000, NULL);
    }
    void reduceTiming()
    {
      if (m_nTiming > 1000)
        m_nTiming -= 1000;
      else
        m_nTiming = 0;
      displayTiming();
    }
    void stopTiming()
    {
      ::KillTimer(m_hDlg, 100);
      displayTiming(false);
    }

    void updateUI(HWND hDlg, bool bPlay = true, bool bPause = true, bool bStop = true, bool bForward = true)
    {
      ::EnableWindow(::GetDlgItem(hDlg, IDC_BPLAY),        (bPlay)    ? TRUE : FALSE);
      ::EnableWindow(::GetDlgItem(hDlg, IDC_BPAUSE),       (bPause)   ? TRUE : FALSE);
      ::EnableWindow(::GetDlgItem(hDlg, IDC_BSTOP),        (bStop)    ? TRUE : FALSE);
      ::EnableWindow(::GetDlgItem(hDlg, IDC_BFORWARDSTEP), (bForward) ? TRUE : FALSE);
    }
    static void setDlgButtonImage(HWND hDlg, UINT btnId, UINT bmpId)
    {
      HANDLE hBitmap = ::LoadImage(::GetModuleHandle(NULL), MAKEINTRESOURCE(bmpId), IMAGE_BITMAP, 16, 16, LR_DEFAULTCOLOR);
      ::SendMessage(::GetDlgItem(hDlg, btnId), BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap);
    }

    static ThreadLoopParam *dlgParam(HWND hDlg)
    {
      return (ThreadLoopParam*)(OdIntPtr)::GetWindowLongPtr(hDlg, DWLP_USER);
    }

    static INT_PTR CALLBACK dlgClientProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
      switch (uMsg) {
        case WM_INITDIALOG:
          {
#ifndef WIN64
            ::SetWindowLongPtr(hDlg, DWLP_USER, (LONG)lParam);
#else
            ::SetWindowLongPtr(hDlg, DWLP_USER, (LONG_PTR)lParam);
#endif
            // Prepare dialog controls.
            setDlgButtonImage(hDlg, IDC_BPLAY, IDB_PLAY);
            setDlgButtonImage(hDlg, IDC_BPAUSE, IDB_PAUSE);
            setDlgButtonImage(hDlg, IDC_BSTOP, IDB_STOP);
            setDlgButtonImage(hDlg, IDC_BFORWARDSTEP, IDB_FORWARD);
            dlgParam(hDlg)->m_pThis->updateUI(hDlg, false);
            dlgParam(hDlg)->m_menuBar.disable(::GetParent(hDlg));
          }
        break;
        case WM_DESTROY:
          dlgParam(hDlg)->m_pThis->resetDlg(NULL);
          if (!dlgParam(hDlg)->m_bFinalized)
          {
            if (::IsWindowEnabled(::GetDlgItem(hDlg, IDC_BSTOP)) == TRUE)
              dlgParam(hDlg)->m_pClient->stop(dlgParam(hDlg)->m_pThis);
          }
          ::WaitForSingleObject(dlgParam(hDlg)->m_pThis->threadId(), INFINITE);
          if (!dlgParam(hDlg)->m_bFinalized)
            dlgParam(hDlg)->m_pClient->finalize(dlgParam(hDlg)->m_pThis);
          else
            dlgParam(hDlg)->m_menuBar.enable();
          delete dlgParam(hDlg)->m_pThis;
          delete dlgParam(hDlg);
        break;
        case WM_EXREACT:
          ((AnimationClient*)(OdIntPtr)lParam)->process(dlgParam(hDlg)->m_pThis);
        break;
        case WM_EXFINAL:
          dlgParam(hDlg)->m_bFinalized = true;
          dlgParam(hDlg)->m_pClient->finalize(dlgParam(hDlg)->m_pThis);
          ::DestroyWindow(hDlg);
        break;
        case WM_TIMER:
          dlgParam(hDlg)->m_pThis->reduceTiming();
        break;
        case WM_COMMAND:
          switch (HIWORD(wParam))
          {
            case BN_CLICKED:
              switch (LOWORD(wParam))
              {
                case IDC_BPLAY:
                  dlgParam(hDlg)->m_pClient->play(dlgParam(hDlg)->m_pThis);
                  dlgParam(hDlg)->m_pThis->updateUI(hDlg, false);
                break;
                case IDC_BPAUSE:
                  dlgParam(hDlg)->m_pClient->pause(dlgParam(hDlg)->m_pThis);
                  dlgParam(hDlg)->m_pThis->updateUI(hDlg, true, false);
                break;
                case IDC_BSTOP:
                  dlgParam(hDlg)->m_pClient->stop(dlgParam(hDlg)->m_pThis);
                  dlgParam(hDlg)->m_pThis->updateUI(hDlg, false, false, false, false);
                break;
                case IDC_BFORWARDSTEP:
                  dlgParam(hDlg)->m_pClient->forward(dlgParam(hDlg)->m_pThis);
                break;
              }
            break;
          }
        default:
        return FALSE;
      }
      return TRUE;
    }
};

AnimationHost *createAnimationHost(HWND hWnd)
{
  return new AnimationHostImpl(hWnd);
}

//
