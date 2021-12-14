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
// AllControlsSheet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSHelperSheet

#include "SHelperPage1.h"
#include "SHelperPage2.h"
#include "SHelperPage3.h"
#include "SHelperPage4.h"

class CSHelperSheet : public CPropertySheet
{
  DECLARE_DYNAMIC(CSHelperSheet)

  // Construction
public:
  CSHelperSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
  CSHelperSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

  void fillAndSelect3Page(OdStreamBuf &data);
  void fillAndSelect2Page(CString file);
protected:
  void AddControlPages(void);

  // Attributes

  CSHelperPage1    m_page1;
  CSHelperPage2    m_page2;
  CSHelperPage3    m_page3;
  CSHelperPage4    m_page4;
  // Operations
public:

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CSHelperSheet)
  //}}AFX_VIRTUAL

  // Implementation
public:
  virtual ~CSHelperSheet();
  virtual BOOL OnInitDialog();

// MKU 02/01/10 warning C4355: 'this' : used in base member initializer list
  void setHelperPages()
  {
    m_page1.setSheet(this);
    m_page2.setSheet(this);
  }

  // Generated message map functions
protected:

  //{{AFX_MSG(CSHelperSheet)
  afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
