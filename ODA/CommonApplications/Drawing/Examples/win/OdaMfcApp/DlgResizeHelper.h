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

#ifndef DLGRESIZEHELPER_H_
#define DLGRESIZEHELPER_H_

#pragma warning (disable: 4786)
#include <list>

class DlgResizeHelper
{
public:

  // fix horizontal dimension/position
  enum EHFix {
    kNoHFix     = 0,
    kWidth      = 1,
    kLeft       = 2,
    kRight      = 4,
    kWidthLeft  = 3,
    kWidthRight = 5,
    kLeftRight  = 6
  };

  // fix vertical dimension/position
  enum EVFix {
    kNoVFix       = 0,
    kHeight       = 1,
    kTop          = 2,
    kBottom       = 4,
    kHeightTop    = 3,
    kHeightBottom = 5,
    kTopBottom    = 6
  };

  // initialize with parent window, all child windows must already have
  // their original position/size
  void Init(HWND a_hParent);

  // explicitly add a window to the list of child windows (e.g. a sibling window)
  // Note: you've got to call Init() before you can add a window
  void Add(HWND a_hWnd);

  // fix position/dimension for a child window, determine child by...
  // ...HWND...
  BOOL Fix(HWND a_hCtrl, EHFix a_hFix, EVFix a_vFix);
  // ...item ID (if it's a dialog item)...
  BOOL Fix(int a_itemId, EHFix a_hFix, EVFix a_vFix);
  // ...all child windows with a common class name (e.g. "Edit")
  UINT Fix(LPCTSTR a_pszClassName, EHFix a_hFix, EVFix a_vFix);
  // ...or all registered windows
  BOOL Fix(EHFix a_hFix, EVFix a_vFix);

  // resize child windows according to changes of parent window and fix attributes
  void OnSize();
private:
  struct CtrlSize {
    CRect m_origSize;
    HWND  m_hCtrl;
    EHFix  m_hFix;
    EVFix  m_vFix;
    CtrlSize() : m_hFix(kNoHFix), m_vFix(kNoVFix) {
    }
  };
  typedef std::list<CtrlSize> CtrlCont_t;
  CtrlCont_t m_ctrls;
  HWND       m_hParent;
  CRect      m_origParentSize;
};

#endif // DLGRESIZEHELPER_H_
