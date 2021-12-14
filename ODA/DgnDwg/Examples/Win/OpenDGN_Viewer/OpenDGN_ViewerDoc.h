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

#ifndef _AFX_ODADGNAPPDOC__INCLUDED_
#define _AFX_ODADGNAPPDOC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxwin.h>
#include "DgDatabase.h"

class COdaDgnAppDoc;
class COdaDgnAppVectorizer;

class OdDgDatabaseDoc : public OdDgDatabase
{
  static  COdaDgnAppDoc*  g_pDoc;
  mutable COdaDgnAppDoc*  m_pDoc;

  using OdDgDatabase::operator new;
  using OdDgDatabase::operator delete;
public:
  ODRX_DECLARE_MEMBERS(OdDgDatabaseDoc);

  OdDgDatabaseDoc();

  COdaDgnAppDoc* document() const;

  static void setDocToAssign(COdaDgnAppDoc* pDoc);
};

typedef OdSmartPtr<OdDgDatabaseDoc> OdDgDatabaseDocPtr;


class COdaDgnAppDoc : public CDocument
{
  // Constructor/destructor
protected:
  // create from serialization only
  using CDocument::operator new;
  using CDocument::operator delete;
  COdaDgnAppDoc();
  DECLARE_DYNCREATE(COdaDgnAppDoc)
public:
  virtual ~COdaDgnAppDoc();

  // Attributes
public:
  OdDgDatabase* database() { return m_pDb; }
  std::vector< COdaDgnAppVectorizer* >& vectorizers() { return m_pVectorizers; }
  OdDgElementId vectorizedViewId() { return m_vectorizedViewId; }
  OdDgElementId vectorizedModelId() { return m_vectorizedModelId; }

  OdUInt16 getActiveView() { return m_nActiveView; }
  void setActiveView(OdUInt16 item) { m_nActiveView = item; }
  OdDgElementId getVeiwId(OdUInt16 item);
  int getViews() { return (int)m_pViews.size(); }
  void resetTolbar();


  void recountVeiws( const OdDgViewGroupPtr& pViewGroup );

  // Operations
public:
  void OnCloseVectorizer(COdaDgnAppVectorizer* pView);
  void OnVectorize(UINT nActiveGS);

  // Overrides
public:
  virtual void OnCloseDocument();
  virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
  virtual void Serialize(CArchive& ar);
  virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);


  // Generated message map functions
protected:
  //{{AFX_MSG(COdaMfcAppDoc)
  afx_msg void OnUpdateSwitchModel(CCmdUI*pCmdUI);
  afx_msg void OnSwitchModel();
  afx_msg void OnViewSumminfo();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()

  // Implementation
public:
  BOOL DoPromptFileName(CString& fileName, UINT nIDSTitle, DWORD lFlags, BOOL bOpenFileDialog, CDocTemplate* pTemplate);
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

protected:
  OdDgDatabasePtr                   m_pDb;
  std::vector< COdaDgnAppVectorizer* > m_pVectorizers;
  std::vector< OdDgElementId > m_pViews;
  OdUInt16 m_nActiveView;

  OdDgElementId m_vectorizedModelId;
  OdDgElementId m_vectorizedViewId;

  int m_SaveAsVer;
  int m_SaveAsType;
};

#endif // _AFX_ODADGNAPPDOC__INCLUDED_

