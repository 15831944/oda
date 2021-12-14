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

#if !defined(AFX_UNDOLIST_H__INCLUDED_)
#define AFX_UNDOLIST_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetActiveLayoutDlg.h : header file
//

#include "DbDatabase.h"

// UndoDialog dialog

class UndoDialog : public CDialog
{
  OdDbDatabase* m_pDb;
  int     m_nSelected;
public:
  UndoDialog(OdDbDatabase* pDb, CWnd* pParent = NULL);   // standard constructor
	virtual ~UndoDialog();

  void FillListBox();
  int getSelected() { return m_nSelected; }
// Dialog Data
  enum { IDD = IDD_UNDO_LIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  virtual BOOL OnInitDialog();
  afx_msg void OnUndoDlgClose();
  afx_msg void OnSelchangeUndolist();
  afx_msg void OnDblclkUndolist();

	DECLARE_MESSAGE_MAP()
};

#endif
