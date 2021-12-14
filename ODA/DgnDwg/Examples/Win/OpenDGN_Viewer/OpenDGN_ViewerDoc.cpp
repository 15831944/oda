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
// OdaDgnAppDoc.cpp : implementation of the COdaDgnAppDoc class
//

#include "stdafx.h"
#include "OpenDGN_Viewer.h"
#include "OpenDGN_ViewerDoc.h"
#include "OpenDGN_ViewerVectorizer.h"
#include "MainFrm.h"
#include "OdStreamBuf.h"
#include "CSwitchModel.h"
#include "SummDlg.h"

#define CREATE_DATABASEDOC(ClassN) OdDgDatabaseDocPtr(new OdDgDatabaseDoc, kOdRxObjAttach)

ODRX_CONS_DEFINE_MEMBERS(OdDgDatabaseDoc, OdDgDatabase, CREATE_DATABASEDOC);

COdaDgnAppDoc* OdDgDatabaseDoc::g_pDoc = 0;

OdDgDatabaseDoc::OdDgDatabaseDoc()
  : m_pDoc(g_pDoc)
{
  g_pDoc = 0;
}

COdaDgnAppDoc* OdDgDatabaseDoc::document() const
{
  return m_pDoc;
}

void OdDgDatabaseDoc::setDocToAssign(COdaDgnAppDoc* pDoc)
{
  g_pDoc = pDoc;
}


// COdaDgnAppDoc

IMPLEMENT_DYNCREATE(COdaDgnAppDoc, CDocument)

BEGIN_MESSAGE_MAP(COdaDgnAppDoc, CDocument)
  //{{AFX_MSG_MAP(COdaDgnAppDoc)
  ON_UPDATE_COMMAND_UI( ID_SWITCH_MODEL, OnUpdateSwitchModel )
  ON_COMMAND( ID_SWITCH_MODEL, OnSwitchModel )
  ON_COMMAND(ID_VIEW_SUMMINFO, OnViewSumminfo)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


// COdaDgnAppDoc construction/destruction

COdaDgnAppDoc::COdaDgnAppDoc()
  : m_SaveAsVer(0)
  , m_SaveAsType(0)
  , m_nActiveView(0)
{
  // TODO: add one-time construction code here

}

COdaDgnAppDoc::~COdaDgnAppDoc()
{
}

void COdaDgnAppDoc::OnCloseDocument( )
{
  CDocument::OnCloseDocument();
}

void COdaDgnAppDoc::recountVeiws( const OdDgViewGroupPtr& pVG )
{
  m_nActiveView = 0;
  m_pViews.resize(0);

  OdDgViewGroupPtr pViewGroup = pVG;

  if( pViewGroup.isNull() )
  {
    pViewGroup = m_pDb->getActiveViewGroupId().openObject();
  }

  if( pViewGroup.isNull() )
  {
    return;
  }
  m_nActiveView = 1;
  OdDgElementIteratorPtr pIt = pViewGroup->createIterator();
  for( ; !pIt->done(); pIt->step() )
  {
    OdDgViewPtr pView = OdDgView::cast( pIt->item().openObject() );
    if( pView.get() && pView->getVisibleFlag() )
    {
      m_pViews.push_back( pIt->item() );
    }
  }
//  resetTolbar();
}
OdDgElementId COdaDgnAppDoc::getVeiwId(OdUInt16 item)
{
  m_nActiveView = item;
  ODA_ASSERT(item <= m_pViews.size());
  return m_pViews[item - 1];
}

BOOL COdaDgnAppDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
  OdDgDatabaseDoc::setDocToAssign(this);
  m_pDb = theApp.openFile(lpszPathName);
  recountVeiws( OdDgViewGroupPtr() );
  return !m_pDb.isNull();
}

BOOL COdaDgnAppDoc::DoPromptFileName(CString& fileName, UINT nIDSTitle, DWORD lFlags, BOOL bOpenFileDialog, CDocTemplate* /*pTemplate*/)
{
  CString ext = fileName.Right(3);

  CFileDialog dlgFile(bOpenFileDialog);

  CString title;
  VERIFY(title.LoadString(nIDSTitle));

  dlgFile.m_ofn.Flags |= lFlags;

  CString strFilter;
  CString strDefault;

  strFilter = _T("DGN V8 file format (*.dgn)");
  strFilter += (TCHAR)'\0';   // next string please
  strFilter += _T("*.dgn");
  strFilter += (TCHAR)'\0';   // last string
  dlgFile.m_ofn.nMaxCustFilter++;

  dlgFile.m_ofn.nFilterIndex = 1;

  if (fileName.Find('.') != -1)
  {
    fileName = fileName.Left(fileName.Find('.'));
  }

  dlgFile.m_ofn.lpstrFilter = strFilter;
  dlgFile.m_ofn.lpstrTitle = title;
  dlgFile.m_ofn.lpstrFile = fileName.GetBuffer(_MAX_PATH);

  INT_PTR nResult = dlgFile.DoModal();
  fileName.ReleaseBuffer();

  if (fileName.Find('.') == -1)
  {
    fileName += _T(".dgn");
  }

  m_SaveAsType = 0;
  switch ( dlgFile.m_ofn.nFilterIndex )
  {
  case 1:
    m_SaveAsVer = 0;
    break;
  case 2:
    m_SaveAsVer = 1;
    break;
  default:
    m_SaveAsVer = 0;
  };

  return nResult == IDOK;
}


// COdaDgnAppDoc serialization

void COdaDgnAppDoc::Serialize(CArchive& ar)
{
  if (ar.IsStoring())
  {
    // TODO: add storing code here
  }
  else
  {
    // TODO: add loading code here
  }
}



// COdaDgnAppDoc diagnostics

#ifdef _DEBUG
void COdaDgnAppDoc::AssertValid() const
{
  CDocument::AssertValid();
}

void COdaDgnAppDoc::Dump(CDumpContext& dc) const
{
  CDocument::Dump(dc);
}
#endif //_DEBUG


// COdaDgnAppDoc commands

void COdaDgnAppDoc::OnVectorize(UINT nActiveGS /*const OdString& sVectorizerPath*/) 
{
}

//#pragma warning(pop)
CMenu* getVectorizeMenu2(CMenu* pMainMenu);

BOOL COdaDgnAppDoc::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
  // If pHandlerInfo is NULL, then handle the message
  if (pHandlerInfo == NULL)
  {
    CMenu* pTopMenu = theApp.GetMainWnd()->GetMenu();
    if(pTopMenu)
    {
      // Check if it is a OdaDgnApp's dynamic menu item:
      MENUITEMINFO menuItemInfo;
      menuItemInfo.cbSize = sizeof(menuItemInfo);
      menuItemInfo.fMask = MIIM_DATA;
      if(pTopMenu->GetMenuItemInfo(nID, &menuItemInfo, FALSE)) // command from menu
      {
        if(menuItemInfo.dwItemData==theApp.getGSMenuItemMarker())
        {
          CString s;
          getVectorizeMenu2(pTopMenu)->GetMenuString(nID, s, MF_BYCOMMAND);
          if (nCode == CN_COMMAND)
          {
//            OnVectorize((LPCTSTR)s);
          }
          else if (nCode == CN_UPDATE_COMMAND_UI)
          {
            ((CCmdUI*)pExtra)->Enable(m_pVectorizers.size()==0);
//            ((CCmdUI*)pExtra)->SetCheck(s==(LPCTSTR)theApp.settings().getGsDevice());
          }
          return TRUE;
        }
      }
    }
  }

  // If we didn't process the command, call the base class
  // version of OnCmdMsg so the message-map can handle the message
  return CDocument::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void COdaDgnAppDoc::OnCloseVectorizer(COdaDgnAppVectorizer* pVectorizer)
{
  //std::set< COdaDgnAppVectorizer* >::iterator iterator = m_pVectorizers.find( pVectorizer );

  //ODA_ASSERT( iterator != m_pVectorizers.end() );
  //m_pVectorizers.erase( iterator );
}


afx_msg void COdaDgnAppDoc::OnUpdateSwitchModel(CCmdUI*pCmdUI)
{
  //actually, MFC switches this button enabled/disabled if ON_UPDATE_COMMAND_UI created for the ID;
  //so, I have nothing to write inside this method
}

afx_msg void COdaDgnAppDoc::OnSwitchModel()
{
  CSwitchModel newDialog( database() );

  //do nothing if either new settings were not accepted or there is no vectorization at the moment
  if( newDialog.DoModal() == IDOK && m_pVectorizers.size() )
  {
    recountVeiws( OdDgViewGroupPtr() );

    //remove all opened vectorizers (=viewports)
    for ( OdUInt32 i = 0, iSize = (OdUInt32)m_pVectorizers.size(); i < iSize; i++ )
    {
      CWnd* wndVectorizer = m_pVectorizers[i]->GetActiveWindow();
      wndVectorizer->SendMessage( WM_COMMAND, theApp.settings().getActiveGS() );
    }
  }
}

void COdaDgnAppDoc::OnViewSumminfo()
{
  CSummDlg sd(database(), theApp.GetMainWnd());
  sd.DoModal();
}

void COdaDgnAppDoc::resetTolbar()
{
  CWnd* wndVectorizer = m_pVectorizers[0]->GetActiveWindow();
  wndVectorizer->SendMessage( WM_COMMAND, ID_VIEWS );
}
