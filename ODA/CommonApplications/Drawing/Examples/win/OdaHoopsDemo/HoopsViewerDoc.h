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

// HoopsViewerDoc.h : interface of the CHoopsViewerDoc class
//

#if !defined(_HOOPSVIEWERDOC_INCLUDED)
#define _HOOPSVIEWERDOC_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct CDWGLayoutInfo
{
  CString strSegmentName;
  HC_KEY  lSegmentKey;
  HPoint  ptCameraPosition;
  HPoint  ptCameraTarget;
  HPoint  vrCameraUp;
};

class CHoopsViewerDoc : public CDocument
{
protected: // create from serialization only
	CHoopsViewerDoc();
	DECLARE_DYNCREATE(CHoopsViewerDoc)

public:
  const char*		GetModelSegment( void ) { return (const char*)m_sModelSegment; }
  bool IsBrepGeometry(){ return m_bIsBrepGeometry; };

public:
  OdArray<CDWGLayoutInfo>     m_layoutInfoArr;
  int                         m_iCurLayoutRecord;
  bool                        m_bInitLayouts;
  bool                        m_bMultyLayouts;
  bool                        m_bShowText;
  bool                        m_bShowLines;
  bool                        m_bLoadDWG;
  bool                        m_bLoadDGN;

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
  virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
  virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CHoopsViewerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
  afx_msg void OnFileSaveAs();
  afx_msg void OnFileSave();

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

private:
  char	m_sModelSegment[1024];
  bool m_bIsBrepGeometry;

};

#endif

