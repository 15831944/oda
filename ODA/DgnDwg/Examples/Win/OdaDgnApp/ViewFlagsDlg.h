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

class CViewFlagsDlg : public CDialog
{
	DECLARE_DYNAMIC(CViewFlagsDlg)

public:
	CViewFlagsDlg(CWnd* pParent, OdDgDatabase* pDb, OdDgView* pView );   // standard constructor
	virtual ~CViewFlagsDlg();

// Dialog Data
	enum { IDD = IDD_VIEWFLAGS_DIALOG };

private:

  OdDgDatabase*        m_pDb;
  OdDgView*            m_pView;

  int                  m_bShowACS;
  int                  m_bDrawBackground;
  int                  m_bBoundaryDispaly;
  int                  m_bUseCamera;
  int                  m_bClipFront;
  int                  m_bClipBack;
  int                  m_bClipVolume;
  int                  m_bShowConstructions;
  int                  m_bFastCurves;
  int                  m_bShowDimensions;
  int                  m_bShowDataFields;
  int                  m_bDisplayset;
  int                  m_bFastCells;
  int                  m_bShowFills;
  int                  m_bShowGrid;
  int                  m_bLevelOverrides;
  int                  m_bUseLineStyles;
  int                  m_bUseLineWeights;
  int                  m_bShowTextures;
  int                  m_bShowPatterns;
  int                  m_bShowTags;
  int                  m_bShowTexts;
  int                  m_bShowTextNodes;
  int                  m_bUseTransparency;
  int                  m_iSelectedItem;

public:
  bool                 m_bRegenerateDrawing;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnBnClickedAcsCheck();
  afx_msg void OnBnClickedBackgroundCheck();
  afx_msg void OnBnClickedBoundaryDisplayCheck();
  afx_msg void OnBnClickedCameraCheck();
  afx_msg void OnBnClickedClipFrontCheck();
  afx_msg void OnBnClickedClipBackCheck();
  afx_msg void OnBnClickedClipVolumeCheck();
  afx_msg void OnBnClickedConstructionsCheck();
  afx_msg void OnBnClickedFastCurvesCheck();
  afx_msg void OnBnClickedDimensionCheck();
  afx_msg void OnBnClickedDataFieldCheck();
  afx_msg void OnBnClickedDisplaysetCheck();
  afx_msg void OnBnClickedFastCellsCheck();
  afx_msg void OnBnClickedFillsCheck();
  afx_msg void OnBnClickedGridCheck();
  afx_msg void OnBnClickedLevelOverrigeCheck();
  afx_msg void OnBnClickedLineStyleCheck();
  afx_msg void OnBnClickedLineWeightCheck();
  afx_msg void OnBnClickedTexturesCheck();
  afx_msg void OnBnClickedPatternsCheck();
  afx_msg void OnBnClickedTagsCheck();
  afx_msg void OnBnClickedTextsCheck();
  afx_msg void OnBnClickedTextNodesCheck();
  afx_msg void OnBnClickedTransparencyCheck();
  afx_msg void OnDisplayStyleComboSelchange();
};
