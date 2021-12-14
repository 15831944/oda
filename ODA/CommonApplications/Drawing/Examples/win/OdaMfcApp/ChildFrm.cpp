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

// ChildFrm.cpp : implementation of the CChildFrame class
//

#include "stdafx.h"
#include "OdaMfcApp.h"
#include "ChildFrm.h"
#include "OdaMfcAppDoc.h"
#include "DbObjectContextCollection.h"
#include "DbObjectContextManager.h"
#include "DbSymUtl.h"

/////////////////////////////////////////////////////////////////////////////
// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CChildFrame)
	//}}AFX_MSG_MAP
  ON_WM_MDIACTIVATE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFrame construction/destruction

CChildFrame::CChildFrame()
{
}

CChildFrame::~CChildFrame()
{
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChildFrame message handlers

void CChildFrame::ActivateFrame(int nCmdShow) 
{
	nCmdShow = SW_SHOWMAXIMIZED;
	CMDIChildWnd::ActivateFrame(nCmdShow);
}

void CChildFrame::OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd)
{
  CMDIChildWnd::OnMDIActivate( bActivate, pActivateWnd, pDeactivateWnd );

#ifdef ODAMFC_EXPORT
  CFrameWnd* pActFrm = (CFrameWnd*) pActivateWnd;
  CFrameWnd* pDctFrm = (CFrameWnd*) pDeactivateWnd;

  CDocument* pActDoc = pActFrm != 0 ? pActFrm->GetActiveDocument() : 0;
  CDocument* pDctDoc = pDctFrm != 0 ? pDctFrm->GetActiveDocument() : 0;
  //if ( pActDoc == pDctDoc )
  //  return;

  COdaMfcAppApp* pApp = ( COdaMfcAppApp* ) AfxGetApp();
  int i, iSize = (int)pApp->m_aAppReactors.size();

  if ( bActivate )
  {
    if ( pDctDoc ) for ( i = 0; i < iSize; i++ )
      pApp->m_aAppReactors[ i ]->documentToBeDeactivated( pDctDoc );

    if ( pActDoc ) for ( i = 0; i < iSize; i++ )
    {
      pApp->m_aAppReactors[ i ]->documentActivated( pActDoc );
      pApp->m_aAppReactors[ i ]->documentBecameCurrent( pActDoc );
    }
  }
  else
  {
    if ( pActDoc ) for ( i = 0; i < iSize; i++ )
      pApp->m_aAppReactors[ i ]->documentToBeActivated( pActDoc );
  }
#endif // ODAMFC_EXPORT
}

static CMenu* findViewMenu()
{
  int N = theApp.GetMainWnd()->GetMenu()->GetMenuItemCount();
  for (int i = 0; i < N; ++i)
  {
    CString name;
    theApp.GetMainWnd()->GetMenu()->GetMenuString(i, name, MF_BYPOSITION);
    if (name == L"&View")
      return theApp.GetMainWnd()->GetMenu()->GetSubMenu(i);
  }
  return 0;
}

static CMenu* findAnnotationSubmenu()
{
  if (CMenu* viewMenu = findViewMenu())
  {
    for (int i = 0; i < viewMenu->GetMenuItemCount(); ++i)
    {
      CString name;
      viewMenu->GetMenuString(i, name, MF_BYPOSITION);
      if (name == L"Annotation scale")
        return viewMenu->GetSubMenu(i);
    }
    ODA_FAIL_ONCE();
  }
  return 0;
}
static void UpdateAnnotations(OdDbDatabase* db)
{
  if (CMenu* m = findAnnotationSubmenu())
  {
    unsigned i = m->GetMenuItemCount() - 1;
    for (; i > 0; i--)
    {
      if (CMenu* pSubMenu = m->GetSubMenu(i))
        pSubMenu->DestroyMenu();
      m->DeleteMenu(i, MF_BYPOSITION);
    }
    OdDbObjectContextCollectionIteratorPtr ci = db->objectContextManager()->contextCollection(ODDB_ANNOTATIONSCALES_COLLECTION)->newIterator();
    i = 1;
    OdIntPtr cid = 0;
    OdDbAnnotationScalePtr pAnSc = db->getCANNOSCALE();
    if (!pAnSc.isNull())
    {
      cid = pAnSc->uniqueIdentifier();
    }
    for (; !ci->done() && i < 100; ci->next()) // if there are more than 100 scales - something's wrong with the drawing
    {
#ifndef MIIM_STRING
#define MIIM_STRING 0x00000040
#endif
      MENUITEMINFO mi = 
      {
        sizeof(MENUITEMINFO), 
        MIIM_DATA|MIIM_ID|MIIM_STRING, 
        MFT_STRING, 
        MFS_ENABLED,
        i + _APS_NEXT_COMMAND_VALUE,
        0, 0, 0, 
        (UINT)i, 
        (LPTSTR)(LPCTSTR)ci->getContext()->getName(), 
        0 
      };
      if (ci->getContext()->uniqueIdentifier() == cid)
      {
        mi.fMask |= MIIM_STATE;
        mi.fState |= MFS_CHECKED;
      }
#if (WINVER >= 0x0500)
      m->InsertMenuItem(i++, &mi, TRUE);
#else
      ::InsertMenuItem(m->GetSafeHmenu(), i++, TRUE, &mi);
#endif
    }
  }
}

void CChildFrame::OnUpdateFrameMenu(BOOL bActive, CWnd* pActiveWnd, HMENU hMenuAlt)
{
  CMDIChildWnd::OnUpdateFrameMenu(bActive, pActiveWnd, hMenuAlt);
  CDocument* pActDoc = GetActiveDocument();
  if (bActive && pActDoc)
  {
    UpdateAnnotations(((COdaMfcAppDoc*)pActDoc)->m_pDb);
  }
}
