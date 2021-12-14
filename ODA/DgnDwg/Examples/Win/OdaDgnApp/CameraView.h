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

#include "DgDatabase.h"
#include "DgView.h"

// CCameraView dialog

class CCameraViewDlg : public CDialog
{
	DECLARE_DYNAMIC(CCameraViewDlg)

public:
	CCameraViewDlg(CWnd* pParent = NULL, OdDgDatabase* pDb  = NULL );   // standard constructor
	virtual ~CCameraViewDlg();

// Dialog Data
	enum { IDD = IDD_CAMERAVIEW };

private:

  void updateListCtrl( bool bCreateNew = false );
  void updateCtrls();
  OdGeMatrix3d createRotationMatrix();

private:

  OdDgDatabase*        m_pDb;
  OdArray<OdDgViewPtr> m_pViewArr;
  int                  m_iItemSelected;
  int                  m_iProjection;
  int                  m_iVisibility;
  double               m_dPositionX;
  double               m_dPositionY;
  double               m_dPositionZ;
  double               m_dTargetX;
  double               m_dTargetY;
  double               m_dTargetZ;
  double               m_dOrientAngle;
  double               m_dElevateAngle;
  double               m_dRollAngle;
  double               m_dFocusLength;
  double               m_dLensAngle;
  double               m_dClipFront;
  double               m_dClipBack;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnLvnItemchangedViewlist(NMHDR *pNMHDR, LRESULT *pResult);
  afx_msg void OnBnClickedParallelradio();
  afx_msg void OnBnClickedOnradio();
  afx_msg void OnBnClickedClose();
  afx_msg void OnBnClickedExpandClippingPlanes();
  afx_msg void OnEnKillfocusFocusedit();
  afx_msg void OnEnKillfocusLensedit();
  afx_msg void OnEnKillfocusClipfrontedit();
  afx_msg void OnEnKillfocusClipbackedit();
  afx_msg void OnTimer( UINT_PTR nIDEvent );

  afx_msg void OnEnKillfocusOrientedit();
  afx_msg void OnEnKillfocusElevateedit();
  afx_msg void OnEnKillfocusRolledit();
  afx_msg void OnEnKillfocusPosxedit();
  afx_msg void OnEnKillfocusTargetxedit();
};
