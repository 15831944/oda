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
// ViewFlagsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "OdaDgnApp.h"
#include "CompressDlg.h"

// CViewFlagsDlg dialog

IMPLEMENT_DYNAMIC(CCompressDatabaseDlg, CDialog)

CCompressDatabaseDlg::CCompressDatabaseDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCompressDatabaseDlg::IDD, pParent)
{
  m_bEmptyCells        = 1;
  m_bEmptyTexts        = 1;
  m_bSpaceTexts        = 1;
  m_bSharedCellDefs    = 1;
  m_bLineStyles        = 1;
  m_bTextStyles        = 1;
  m_bLevels            = 1;
  m_bDimStyles         = 1;
  m_bXRefLevels        = 1;
  m_bMultilineStyles   = 1;
  m_bTemplates         = 1;
  m_bItemTypeLibs      = 1;
  m_bFileNotFoundXRefs = 1;
}

//=================================================================================

CCompressDatabaseDlg::~CCompressDatabaseDlg()
{
}

//=================================================================================

void CCompressDatabaseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

  DDX_Check(pDX, IDC_EMPTYCELL_CHECK, m_bEmptyCells );
  DDX_Check(pDX, IDC_EMPTYTEXTS_CHECK, m_bEmptyTexts );
  DDX_Check(pDX, IDC_SPACETEXTS_CHECK, m_bSpaceTexts );
  DDX_Check(pDX, IDC_CELLDEF_CHECK, m_bSharedCellDefs );
  DDX_Check(pDX, IDC_LS_CHECK, m_bLineStyles );
  DDX_Check(pDX, IDC_TS_CHECK, m_bTextStyles );
  DDX_Check(pDX, IDC_LEVELS_CHECK, m_bLevels );
  DDX_Check(pDX, IDC_DS_CHECK, m_bDimStyles );
  DDX_Check(pDX, IDC_XREFLEVELS_CHECK, m_bXRefLevels );
  DDX_Check(pDX, IDC_MLS_CHECK, m_bMultilineStyles );
  DDX_Check(pDX, IDC_TEMP_CHECK, m_bTemplates );
  DDX_Check(pDX, IDC_ITEMLIBS_CHECK, m_bItemTypeLibs );
  DDX_Check(pDX, IDC_XREFNOTLOADED_CHECK, m_bFileNotFoundXRefs );
}

//=================================================================================

BEGIN_MESSAGE_MAP(CCompressDatabaseDlg, CDialog)
END_MESSAGE_MAP()

//=================================================================================

OdDg::OdDgCompressDatabaseFlags CCompressDatabaseDlg::getCompressFlags()
{
  OdUInt16 uRet = 0;

  if( m_bEmptyCells )
    uRet |= OdDg::kRemoveEmptyCellHeaders;

  if( m_bEmptyTexts )
    uRet |= OdDg::kRemoveEmptyTexts;

  if( m_bSpaceTexts )
    uRet |= OdDg::kRemoveTextsSpaceOnly;

  if( m_bSharedCellDefs )
    uRet |= OdDg::kRemoveUnusedCellDefinitions;

  if( m_bLineStyles )
    uRet |= OdDg::kRemoveUnusedLineStyles;

  if( m_bTextStyles )
    uRet |= OdDg::kRemoveUnusedTextStyles;

  if( m_bLevels )
    uRet |= OdDg::kRemoveUnusedLevels;

  if( m_bDimStyles )
    uRet |= OdDg::kRemoveUnusedDimStyles;

  if( m_bXRefLevels )
    uRet |= OdDg::kRemoveUnusedNestedXRefLevels;

  if( m_bMultilineStyles )
    uRet |= OdDg::kRemoveUnusedMultilineStyles;

  if( m_bTemplates )
    uRet |= OdDg::kRemoveUnusedElementTemplates;

  if( m_bItemTypeLibs )
    uRet |= OdDg::kRemoveUnusedItemTypeLibraries;

  if( m_bFileNotFoundXRefs )
    uRet |= OdDg::kRemoveXRefsIfFileNotFound;

  return (OdDg::OdDgCompressDatabaseFlags)(uRet);
}

//=================================================================================



