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

// OpenCADDoc.h : interface of the COpenCADDoc class
//


#pragma once

#include "DbDatabase.h"
#include "../Extensions/ExServices/ExDbCommandContext.h"

class COpenCADView;
class CChildFrame;

class COpenCADDoc : public CDocument, protected OdStaticRxObject<OdEdBaseIO>
{
protected:
	using CDocument::operator new;
	using CDocument::operator delete;

protected: // create from serialization only
	COpenCADDoc();
	DECLARE_DYNCREATE(COpenCADDoc)

// Attributes
public:
	OdDbDatabasePtr			m_pDatabase;
	OdDbCommandContextPtr	m_pCommandContext;
	OdString				m_strLastCommand;
	BOOL					m_bCommandActive;

// Operations
public:
	OdString getString(const OdString& prompt, int options, OdEdStringTracker* pTracker);
	void putString(const OdString& string);
	OdEdBaseIO* GetIO();
	OdDbCommandContextPtr GetCommandContext();
	BOOL ExecuteCommand(const OdString& strCommand, BOOL bEcho = TRUE);
	COpenCADView* GetView();
	CChildFrame* GetChildFrame();
	OdGePoint3d getPoint(const OdString& prompt, int options, OdEdPointTracker* pTracker);

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
  virtual OdUInt32 getKeyState();

// Implementation
public:
	virtual ~COpenCADDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
public:
	afx_msg void OnFileImport();
public:
	virtual BOOL CanCloseFrame(CFrameWnd* pFrame);
public:
	afx_msg void OnDrawLine();

  virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
  virtual void OnCloseDocument();
};


