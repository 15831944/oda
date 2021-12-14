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
#pragma once

#include "resource.h"
#include "RemoteGeomBlockPreviewDlg.h"

#define WM_RGV_CHANGE_OUTPUT_STATE (WM_USER + 777)
#define WM_RGV_DELETE_BLOCK (WM_USER + 778)
#define WM_RGV_UNDO_DELETE_BLOCK (WM_USER + 779)
#define WM_RGV_PERMANENT_ERASE_BLOCK (WM_USER + 780)
// CRemoteGeomPropsDlg dialog

struct PreviewBlockData
{
  OdDbDatabasePtr pDb;
  OdDbObjectId idBlock;
};

class CRemoteGeomPropsDlg : public CDialog
{
  CButton m_bmEnableOutput;
  CButton m_bmTogglePreviewWnd;
  CListBox m_lstRemoteBlocks;
  CListBox m_lstHiddenRemoteBlocks;

  CWnd* m_pWndToNotify;

  CRemoteGeomBlockPreviewDlg m_preview;

private:
	DECLARE_DYNAMIC(CRemoteGeomPropsDlg)

public:
	CRemoteGeomPropsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CRemoteGeomPropsDlg();

  void SetNotificationWindow(CWnd* pWndToNotify);
  void OnNewRemoteBlockAdded(OdUInt32 uNewBlockKey);

// Dialog Data
	enum { IDD = IDD_REMOTE_GEOMETRY_PROPS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  virtual BOOL OnInitDialog();

  virtual void OnOK() {};
  virtual void OnCancel() {};

  virtual int CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct);

	DECLARE_MESSAGE_MAP()

protected:
  bool IsEnableOutput();
  void SetIsEnableOutput(bool fEnable);
  void SelectNextListBoxItem(CListBox& lBox, int iItem);
  void AddBlockToList(CListBox& lBox, OdUInt32 uBlockKey);

  void AddBlockToHiddenListBox(OdUInt32 uBlockKey);

  void SendParentMessage(UINT uMsg, WPARAM wp = 0, LPARAM lp = 0);

  void AlignPreviewWindowToPropsDlg();
  void TogglePreviewWindow();

public:
  afx_msg void OnBnClickedOk();
  afx_msg void OnBnClickedEnableOutputCheck();
  afx_msg void OnSize(UINT nType, int cx, int cy);
  //afx_msg int OnMouseActivate( CWnd*, UINT, UINT );
  //afx_msg void OnSetFocus( CWnd* );
  afx_msg void OnBnClickedDeleteBlock();
  afx_msg void OnBnClickedUndoDeleteBlock();
  afx_msg void OnBnClickedHide();
  afx_msg void OnBlocksListSelChange();
  afx_msg void OnHiddenBlocksListSelChange();
  afx_msg void OnBnClickedHideAllBlocks();
  afx_msg void OnBnClickedShowAllBlocks();
  afx_msg void OnBnClickedDeleteAllBlocks();
  afx_msg void OnBnClickedPreviewWindow();
};
