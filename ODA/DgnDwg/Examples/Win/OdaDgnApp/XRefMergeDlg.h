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

// CXRefMergeDlg dialog

class CXRefMergeDlg : public CDialog
{
	DECLARE_DYNAMIC(CXRefMergeDlg)

public:
  CXRefMergeDlg(CWnd* pParent = NULL, OdDgDatabase* pDb = NULL );   // standard constructor
	virtual ~CXRefMergeDlg();

  OdDgElementId getXRefId() const;

// Dialog Data
	enum { IDD = IDD_DIALOG_XREFMERGE };

public:

  int                             m_bClipXRef;
  int                             m_bXRefColorTable;
  int                             m_bBindToCell;

private:

  int                             m_iXRefIndex;
  OdDgDatabase*                   m_pDb;
  OdDgElementIdArray              m_arrxRefs;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
};
