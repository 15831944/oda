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

// HoopsViewerDoc.cpp : implementation of the CHoopsViewerDoc class
//

#include "stdafx.h"
#include "HoopsViewer.h"
#include "HoopsViewerDoc.h"
//#include "HUtilityLocaleString.h"
#include "HIODwgImpl.h"
#include "utf_utils.h"

#ifdef _TOOLKIT_IN_DLL_
#ifdef _DEBUG
#define HOOPSSTREAM_DLL_NAME OD_T("hoops_stream1700_vc80d.dll")
#else
#define HOOPSSTREAM_DLL_NAME OD_T("hoops_stream1700_vc80.dll")
#endif
#endif

extern CHoopsViewerApp theApp;

// CHoopsViewerDoc

IMPLEMENT_DYNCREATE(CHoopsViewerDoc, CDocument)

BEGIN_MESSAGE_MAP(CHoopsViewerDoc, CDocument)
  ON_COMMAND(ID_FILE_SAVE, OnFileSave )
  ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs )
END_MESSAGE_MAP()


// CHoopsViewerDoc construction/destruction

CHoopsViewerDoc::CHoopsViewerDoc()
{
  m_bIsBrepGeometry = false;
  m_iCurLayoutRecord = -1;
  m_bInitLayouts = false;
  m_bMultyLayouts = false;
  m_bShowText     = true;
  m_bShowLines    = true;
  m_bLoadDWG      = false;
  m_bLoadDGN      = false;
  m_sModelSegment[0] = 0;
	// TODO: add one-time construction code here
}

CHoopsViewerDoc::~CHoopsViewerDoc()
{
  if( m_sModelSegment[0] )
    HC_Delete_Segment( m_sModelSegment );
}

BOOL CHoopsViewerDoc::OnNewDocument()
{
  if (!CDocument::OnNewDocument())
    return FALSE;

  int count = theApp.IncrementCounter();

  sprintf (m_sModelSegment, "/include library/model%d", count);
  HC_Open_Segment (m_sModelSegment);
  HC_Close_Segment();

  // TODO: add reinitialization code here
  // (SDI documents will reuse this document)

  return TRUE;
}

BOOL CHoopsViewerDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
  CFileFind tmp;

  if( !tmp.FindFile(lpszPathName) )
  {
    tmp.Close();
    return FALSE;
  }
  else
    tmp.Close();

  int count = theApp.IncrementCounter();
  sprintf (m_sModelSegment, "/include library/model%d", count);

  char extension[1024]; 
  unsigned int i;

  extension[0] = '\0';
  HC_Parse_String(H_ASCII_TEXT(lpszPathName), ".", -1, extension);

  // make it lower case
  for(i = 0; i < strlen(extension); i++)
    extension[i] = (char) tolower(extension[i]);


  if(!strcmp(extension, "hmf"))
  {
    HC_Open_Segment (m_sModelSegment);
    HC_Read_Metafile(H_ASCII_TEXT(lpszPathName), ".", "follow cross-references, restore state = (color names, fonts, textures)");
    HC_Close_Segment();

    return TRUE;
  }
  else if(!strcmp(extension, "hsf"))
  {
    int sflags = 0;
    sflags |= TK_Full_Resolution_Vertices;
    sflags |= TK_Full_Resolution_Normals;
    sflags |= TK_Full_Resolution_Parameters;
    sflags |= TK_Disable_Instancing;
    sflags |= TK_Disable_Tristrips;

    HStreamFileToolkit* tk = new HStreamFileToolkit;

    HC_Open_Segment (m_sModelSegment);
    HTK_Read_Stream_File(H_ASCII_TEXT(lpszPathName),sflags,tk);
    HC_Close_Segment();

    delete tk;

    return TRUE;
  }
  else if( !strcmp(extension, "dwg") || !strcmp(extension, "dxf") )
  {
    HC_KEY m_ModelKey = HC_KOpen_Segment (m_sModelSegment);

    CDwgDumper DwgDumper;

    if( ((CHoopsViewerApp*)AfxGetApp())->m_bImportAllLayouts )
      m_bMultyLayouts = true;

    DwgDumper.setFilePath( lpszPathName );
    DwgDumper.setModelKey( m_ModelKey );

    DwgDumper.initialize();

    BOOL bRet = DwgDumper.loadToHoops();

    DwgDumper.uninitialize();

    HC_Close_Segment();

    if( bRet )
      m_bLoadDWG = true;
    else
    {
      if( DwgDumper.getLastErrorDescription().isEmpty() )
        MessageBox( GetActiveWindow(),L"There was errors during converting file to hoops. Operation was canceled.", L"Converting error", MB_ICONWARNING|MB_OK);
      else
      {
        CString strError = DwgDumper.getLastErrorDescription().c_str();
        strError += ". Operation was canceled.";
        MessageBox( GetActiveWindow(),strError, L"Error", MB_ICONWARNING|MB_OK);
      }
    }

    return bRet;
  }
  else if( !strcmp(extension, "dgn") )
  {
    HC_KEY m_ModelKey = HC_KOpen_Segment (m_sModelSegment);

    CDgnDumper DgnDumper;

    if( ((CHoopsViewerApp*)AfxGetApp())->m_bImportAllLayouts )
      m_bMultyLayouts = true;

    DgnDumper.setFilePath( lpszPathName );
    DgnDumper.setModelKey( m_ModelKey );

    DgnDumper.initialize();

    BOOL bRet = DgnDumper.loadToHoops();

    DgnDumper.uninitialize();

    HC_Close_Segment();

    if( bRet )
      m_bLoadDGN = true;
    else
    {
      if( DgnDumper.getLastErrorDescription().isEmpty() )
        MessageBox( GetActiveWindow(),L"There was errors during converting file to hoops. Operation was canceled.", L"Converting error", MB_ICONWARNING|MB_OK);
      else
      {
        CString strError = DgnDumper.getLastErrorDescription().c_str();
        strError += ". Operation was canceled.";
        MessageBox( GetActiveWindow(),strError, L"Error", MB_ICONWARNING|MB_OK);
      }
    }

    return bRet;
  }
  else
  {
    // this format not supported
    return FALSE;
  }
}

BOOL CHoopsViewerDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
  char extension[1024]; 
  unsigned int i;

  extension[0] = '\0';
  HC_Parse_String(H_ASCII_TEXT(lpszPathName), ".", -1, extension);

  // make it lower case
  for(i = 0; i < strlen(extension); i++)
    extension[i] = (char) tolower(extension[i]);

  if(!strcmp(extension, "hmf"))
  {
    if( m_iCurLayoutRecord >= 0  && m_iCurLayoutRecord < (int)m_layoutInfoArr.size() )
      HC_Open_Segment_By_Key(m_layoutInfoArr[m_iCurLayoutRecord].lSegmentKey);
    else
      HC_Open_Segment( m_sModelSegment );

    HC_Write_Metafile(".", H_ASCII_TEXT(lpszPathName), "use color names, follow cross-references, save state = (color names, fonts, textures)"); 
    HC_Close_Segment();
    return TRUE;
  }
  else if(!strcmp(extension, "hsf"))
  {
#ifdef _TOOLKIT_IN_DLL_
    HMODULE hmHoopsStream = ::LoadLibrary( HOOPSSTREAM_DLL_NAME );

    if( hmHoopsStream )
#endif
    {
      int sflags = 0;
      sflags |= TK_Full_Resolution_Vertices;
      sflags |= TK_Full_Resolution_Normals;
      sflags |= TK_Full_Resolution_Parameters;
      sflags |= TK_Disable_Instancing;
      sflags |= TK_Disable_Tristrips;

      HStreamFileToolkit * tk = new HStreamFileToolkit();

      if( m_iCurLayoutRecord >= 0  && m_iCurLayoutRecord < (int)m_layoutInfoArr.size() )
        HC_Open_Segment_By_Key(m_layoutInfoArr[m_iCurLayoutRecord].lSegmentKey);
      else
        HC_Open_Segment( m_sModelSegment );

      HTK_Write_Stream_File(H_ASCII_TEXT(lpszPathName), sflags, tk);
      HC_Close_Segment();

      delete tk;

#ifdef _TOOLKIT_IN_DLL_
      ::FreeLibrary( hmHoopsStream );
#endif
    }

    SetPathName(lpszPathName);
    return true;
  }
  else
  {
    // file format not supported
    return CDocument::OnSaveDocument(lpszPathName);
  }
}

// CHoopsViewerDoc serialization

void CHoopsViewerDoc::Serialize(CArchive& ar)
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


// CHoopsViewerDoc diagnostics

#ifdef _DEBUG
void CHoopsViewerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CHoopsViewerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


void CHoopsViewerDoc::OnFileSaveAs()
{
  CFileDialog saveDlg(false, L"*.hmf", NULL, 4|2, L"Hoops Metafile *.hmf|*.hmf|Hoops Stream File *.hsf|*.hsf|" );

  if( saveDlg.DoModal() == IDOK )
  {
    OnSaveDocument( saveDlg.GetPathName() );
  }
}

void CHoopsViewerDoc::OnFileSave()
{
  CString strFileExt = m_strPathName;

  if( strFileExt.GetLength() > 3)
    strFileExt = strFileExt.Right(3);

  strFileExt.MakeUpper();

  if( strFileExt != L"HMF" || strFileExt != L"HSF" )
    OnFileSaveAs();
  else
    CDocument::OnFileSave();
}

// CHoopsViewerDoc commands
