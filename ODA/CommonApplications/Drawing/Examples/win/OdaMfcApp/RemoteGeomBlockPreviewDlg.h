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

#include "StaticRxObject.h"

#include "Ge/GeMatrix3d.h"
#include "Gi/Gi.h"
#include "DbGsManager.h"
#include "GiContextForDbDatabase.h"
#include "DbLine.h"
#include "EditorObject.h"
#include "ExDbCommandContext.h"
#include "ExEdInputParser.h"
#include "OdaMfcExport.h"
#include <afxole.h>
#include "ExSystemServices.h"
#include "ExHostAppServices.h"

// CRemoteGeomBlockPreviewDlg dialog
class RemoteGeomServices : public ExSystemServices, public ExHostAppServices
{
protected:
  ODRX_USING_HEAP_OPERATORS(ExSystemServices);

private:

#ifdef _MSC_VER
  bool getPassword(const OdString& dwgName, bool /*isXref*/, OdPassword& password)
  {    
    return false;
  }
#endif
};


class CRemoteGeomBlockPreviewDlg : public CDialog
                                   , OdGiContextForDbDatabase
                                   , OdEdBaseIO
                                   , OdExEditorObject::OleDragCallback
{
  static UINT       g_nRedrawMSG;
  OdStaticRxObject<RemoteGeomServices> svcs;

  OdGsLayoutHelperPtr m_pDevice;
  HDC                 m_hWindowDC;
  OdDbDatabasePtr       m_pDb;

  OdDbObjectId      m_layoutId;
  BOOL m_bPsOverall;

private:
	DECLARE_DYNAMIC(CRemoteGeomBlockPreviewDlg)

  void CreateDevice();
  void DestroyDevice();

  void RegenAll();

  void ClearDatabase();
  void ZoomExtents(OdDbDatabasePtr pDb);

public:
	CRemoteGeomBlockPreviewDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CRemoteGeomBlockPreviewDlg();

  void CreateViewer();

  void SetPreview(OdDbDatabasePtr pDb, OdDbObjectId idBlock);

  OdGsView* getActiveView();
  const OdGsView* getActiveView() const;
  OdGsView* getActiveTopView();
  const OdGsView* getActiveTopView() const;
  void propagateActiveViewChanges() const;

  virtual OdString getString(const OdString& prompt, int options, OdEdStringTracker* pTracker);
  virtual void putString(const OdString& string);
  virtual OdUInt32 getKeyState();
  virtual bool beginDragCallback(const OdGePoint3d& pt);

// Dialog Data
	enum { IDD = IDD_RGV_PREVIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  virtual BOOL OnInitDialog();

  virtual void OnDraw(CDC* pDC);
  
  // Implementation
  void addRef() {}
  void release() {}

  using CDialog::operator new;
  using CDialog::operator delete;

protected:
  CRect viewportRect() const;
  static CRect viewRect(OdGsView*);

protected:
	DECLARE_MESSAGE_MAP()

  afx_msg LRESULT OnRedraw(WPARAM wParam, LPARAM lParam);
  afx_msg void OnPaint();
  afx_msg BOOL OnEraseBkgnd(CDC* /*pDC*/) ;
  afx_msg void OnSize(UINT nType, int cx, int cy) ;
  afx_msg void OnDestroy() ;
};
