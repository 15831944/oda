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

// CCompressDatabaseDlg dialog

class CCompressDatabaseDlg : public CDialog
{
	DECLARE_DYNAMIC(CCompressDatabaseDlg)

public:
	CCompressDatabaseDlg(CWnd* pParent = NULL );   // standard constructor
	virtual ~CCompressDatabaseDlg();

// Dialog Data
	enum { IDD = IDD_COMPRESS_DIALOG };

private:

  int                             m_bEmptyCells;
  int                             m_bEmptyTexts;
  int                             m_bSpaceTexts;
  int                             m_bSharedCellDefs;
  int                             m_bLineStyles;
  int                             m_bTextStyles;
  int                             m_bLevels;
  int                             m_bDimStyles;
  int                             m_bXRefLevels;
  int                             m_bMultilineStyles;
  int                             m_bTemplates;
  int                             m_bItemTypeLibs;
  int                             m_bFileNotFoundXRefs;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
  OdDg::OdDgCompressDatabaseFlags getCompressFlags();
};
