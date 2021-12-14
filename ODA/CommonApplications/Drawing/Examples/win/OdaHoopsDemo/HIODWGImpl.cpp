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

#include "stdafx.h"
#include "HIODwgImpl.h"
#include "StaticRxObject.h"
#include "HoopsViewer.h"
#include "BrowseFileDlg.h"
#include "MainFrm.h"

#ifndef _TOOLKIT_IN_DLL_

#include "rxdynamicmodule.h"

ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdOleItemHandlerModuleImpl);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(ExRasterModule);;
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdRasterProcessingServicesImpl);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(ModelerModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdRecomputeDimBlockModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(ExFieldEvaluatorModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(TD_IdViewObjModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(DgnUnderlayModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdDgnModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(DgModelerModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdDgn7IOModuleImpl);   //this library allows to read V7 files


ODRX_BEGIN_STATIC_MODULE_MAP()
  ODRX_DEFINE_STATIC_APPMODULE(OdOleItemHandlerModuleName,              OdOleItemHandlerModuleImpl)
  ODRX_DEFINE_STATIC_APPMODULE(RX_RASTER_SERVICES_APPNAME,              ExRasterModule)
  ODRX_DEFINE_STATIC_APPMODULE(OdRasterProcessorModuleName,               OdRasterProcessingServicesImpl)
  ODRX_DEFINE_STATIC_APPMODULE(OdModelerGeometryModuleName,      ModelerModule)
  ODRX_DEFINE_STATIC_APPMODULE(OdRecomputeDimBlockModuleName,             OdRecomputeDimBlockModule)
  ODRX_DEFINE_STATIC_APPMODULE(OdExFieldEvaluatorModuleName,              ExFieldEvaluatorModule)
  ODRX_DEFINE_STATIC_APPMODULE(OdAcIdViewObjModuleName,                   TD_IdViewObjModule)
  ODRX_DEFINE_STATIC_APPMODULE(OdDgnUnderlayModuleName,          DgnUnderlayModule)
  ODRX_DEFINE_STATIC_APPMODULE(L"TG_Db",                         OdDgnModule)
  ODRX_DEFINE_STATIC_APPMODULE(L"TG_ModelerGeometry",            DgModelerModule)
  ODRX_DEFINE_STATIC_APPLICATION(L"TG_Dgn7IO",                   OdDgn7IOModuleImpl)
ODRX_END_STATIC_MODULE_MAP()

#endif

class UserBreak
{
};

//////////////////////////////////////////////////////////////////
//   HIODWGImpl.cpp implementation of DwgExportServices class   //
//////////////////////////////////////////////////////////////////

OdString DwgExportServices::findFile(
  const OdString& pcFilename,
  OdDbBaseDatabase* pDb,
  OdDbBaseHostAppServices::FindFileHint hint )
{
  OdString sFile = ExHostAppServices::findFile(pcFilename, pDb, hint);

  if( !sFile.isEmpty() )
    return sFile;

  if(pcFilename.isEmpty())
    return sFile;

  OdString sFileName = pcFilename;

  OdString ext = sFileName.right(4);
  ext.makeUpper();

  if(sFileName.getLength() < 4 || ext[0] != L'.' )
    sFileName += L".shx";

  sFileName.replace(L'/', L'\\');
  sFileName.deleteChars(0, sFileName.reverseFind(L'\\')+1);

  OdArray<OdString>* pStrFontDirArr = &(((CHoopsViewerApp*)AfxGetApp())->m_strFontDirArr);

  OdDbSystemServices* pSs = odSystemServices();

  for( OdUInt32 i = 0; i < pStrFontDirArr->size(); i++ )
  {
    OdString strFilePath = (*pStrFontDirArr)[i];
    strFilePath = strFilePath + sFileName;

    if( pSs->accessFile( strFilePath, Oda::kFileRead) )
    {
      sFile = strFilePath;
      break;
    }
  }

  if( sFile.isEmpty() && !m_bSkipAll )
  {
    CBrowseFileDlg dlg;
    dlg.m_strFileName = CString( (wchar_t*)sFileName.c_str());

    if( dlg.DoModal() == IDOK )
    {
      OdString strPath = dlg.m_strPathName;
      strPath.replace('/','\\');

      if( strPath.reverseFind('\\') != -1 )
      {
        OdString strFileName = strPath.right(strPath.getLength() - strPath.reverseFind('\\') - 1);
        OdString strDirName  = strPath.left(strPath.getLength() - strFileName.getLength());
        pStrFontDirArr->push_back( strDirName );

        OdString strFilePath = strDirName;
        strFilePath = strFilePath + sFileName;

        if( pSs->accessFile( strFilePath, Oda::kFileRead) )
          sFile = strFilePath;
      }
    }
    else
      m_bSkipAll = dlg.m_bSkipAll;
  }

  return sFile;
}

//////////////////////////////////////////////////////////////////

OdDbHostAppProgressMeter* DwgExportServices::newProgressMeter()
{
  return ExHostAppServices::newProgressMeter();
}

void DwgExportServices::start(const OdString& displayString)
{
  m_Msg = (LPCTSTR)displayString;
  m_nProgressPos = 0;
  m_nPercent = -1;
}

/************************************************************************/
/* Progress Meter override                                              */
/************************************************************************/
void DwgExportServices::stop()
{
  m_nProgressPos = m_nProgressLimit;
  meterProgress();
}

/************************************************************************/
/* Progress Meter override                                              */
/************************************************************************/
void DwgExportServices::meterProgress()
{
  int nOldPercent = m_nPercent;

  m_nPercent = int( ( double(m_nProgressPos++) / double(m_nProgressLimit)) * 100 );

  if( nOldPercent!=m_nPercent && m_pWndStatusBar )
  {
    OdString str;

    if( m_Msg.isEmpty() )
      str.format(_T("Loading dwg file... %d"), m_nPercent);
    else
      str.format(_T("%s %d"), m_Msg.c_str(), m_nPercent);

    ((CMainFrame*)(AfxGetApp()->GetMainWnd()))->m_wndStatusBar.SetPaneText(0, str);
    MSG msg;
    while(::PeekMessage(&msg, AfxGetApp()->GetMainWnd()->m_hWnd, WM_KEYUP, WM_KEYUP, 1))
    {
      bool bDup = false;
      if(msg.wParam == VK_ESCAPE && !bDup)
      {
        bDup = true;
        str.format(_T("Are you sure you want to terminate\n%s ?"), m_Msg);
        if(::AfxMessageBox(str, MB_YESNO|MB_ICONQUESTION)==IDYES)
        {
          throw UserBreak();
        }
      }
    }
  }
}

/************************************************************************/

void DwgExportServices::setLimit(int max)
{
  m_nProgressLimit = max ? max : 1;
}

//////////////////////////////////////////////////////////////////
//      HIODWGImpl.cpp implementation of CDwgDumper class       //
//////////////////////////////////////////////////////////////////

CDwgDumper::CDwgDumper()
{
  m_lModelKey       = 0;
  m_strFileName     = "";
  m_bInitSuccessful = false;
}

///////////////////////////////////////////////////////////////////

CDwgDumper::~CDwgDumper()
{
  uninitialize();
}

///////////////////////////////////////////////////////////////////

bool CDwgDumper::initialize()
{
  bool bRet = false;

  m_strErrorDescription.empty();

  if( m_strFileName != "" && !m_bInitSuccessful )
  {
#ifndef _TOOLKIT_IN_DLL_
    ODRX_INIT_STATIC_MODULE_MAP();
#endif
    odInitialize(&svcs);

    try
    {
      m_pDb = svcs.readFile( m_strFileName, true, false );
    }
    catch(const OdError& e)
    {
      if( !m_pDb.isNull() )
        m_pDb.release();

      m_strErrorDescription = e.description();
    }
    catch(...)
    {
      if( !m_pDb.isNull() )
        m_pDb.release();
    }

    if( m_pDb.isNull() )
    {
      m_bInitSuccessful = false;
      odUninitialize();
    }
    else
      m_bInitSuccessful = true;
  }

  return bRet;
}
///////////////////////////////////////////////////////////////////

bool CDwgDumper::loadToHoops()
{
  if( m_bInitSuccessful )
  {
    HoopsExportParams params;

    params.iStartKey   = m_lModelKey;
    params.bSetCamera  = true;
    params.pDb         = m_pDb;
    params.strFileName = "";
    params.background = RGB( 255,255,255);
    params.bExportCurrentLayout = !((CHoopsViewerApp*)AfxGetApp())->m_bImportAllLayouts;
    params.iExportFileType = DWG_FILE_TYPE;

    AfxGetApp()->BeginWaitCursor();
    ((CMainFrame*)(AfxGetApp()->GetMainWnd()))->m_wndStatusBar.SetPaneText(0, L"Converting to hoops...");
    OdInt32 iRes = exportHoops( params );
    AfxGetApp()->EndWaitCursor();

    return (iRes == ::exOk);
  }

  return false;
}

///////////////////////////////////////////////////////////////////

void CDwgDumper::uninitialize()
{
  if( m_bInitSuccessful )
  {
    m_pDb = 0;
    odUninitialize();
  }

  m_bInitSuccessful = false;
}

///////////////////////////////////////////////////////////////////

HC_KEY    CDwgDumper::getModelKey()
{
  return m_lModelKey;
}

///////////////////////////////////////////////////////////////////

void      CDwgDumper::setModelKey( HC_KEY lNewKey )
{
  m_lModelKey = lNewKey;
}

///////////////////////////////////////////////////////////////////

OdString  CDwgDumper::getFilePath()
{
  return m_strFileName;
}

///////////////////////////////////////////////////////////////////

void      CDwgDumper::setFilePath( OdString strPath )
{
  m_strFileName = strPath;
}

///////////////////////////////////////////////////////////////////

OdString  CDwgDumper::getLastErrorDescription()
{
  return m_strErrorDescription;
}

///////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////
//   HIODWGImpl.cpp implementation of DgnExportServices class   //
//////////////////////////////////////////////////////////////////

OdString DgnExportServices::findFile(
                                     const OdString& pcFilename,
                                     OdDbBaseDatabase* pDb,
                                     OdDbBaseHostAppServices::FindFileHint hint )
{
  OdString sFile = OdExDgnHostAppServices::findFile(pcFilename, pDb, hint);

  if( !sFile.isEmpty() )
    return sFile;

  if(pcFilename.isEmpty())
    return sFile;

  OdString sFileName = pcFilename;

  OdString ext = sFileName.right(4);
  ext.makeUpper();

  if(sFileName.getLength() < 4 || ext[0] != L'.' )
    sFileName += L".shx";

  sFileName.replace(L'/', L'\\');
  sFileName.deleteChars(0, sFileName.reverseFind(L'\\')+1);

  OdArray<OdString>* pStrFontDirArr = &(((CHoopsViewerApp*)AfxGetApp())->m_strFontDirArr);

  OdDbSystemServices* pSs = odSystemServices();

  for( OdUInt32 i = 0; i < pStrFontDirArr->size(); i++ )
  {
    OdString strFilePath = (*pStrFontDirArr)[i];
    strFilePath = strFilePath + sFileName;

    if( pSs->accessFile( strFilePath, Oda::kFileRead) )
    {
      sFile = strFilePath;
      break;
    }
  }

  if( sFile.isEmpty() && !m_bSkipAll && ext != L".RSC" )
  {
    CBrowseFileDlg dlg;
    dlg.m_strFileName = CString( (wchar_t*)sFileName.c_str());

    if( dlg.DoModal() == IDOK )
    {
      OdString strPath = dlg.m_strPathName;
      strPath.replace('/','\\');

      if( strPath.reverseFind('\\') != -1 )
      {
        OdString strFileName = strPath.right(strPath.getLength() - strPath.reverseFind('\\') - 1);
        OdString strDirName  = strPath.left(strPath.getLength() - strFileName.getLength());
        pStrFontDirArr->push_back( strDirName );

        OdString strFilePath = strDirName;
        strFilePath = strFilePath + sFileName;

        if( pSs->accessFile( strFilePath, Oda::kFileRead) )
          sFile = strFilePath;
      }
    }
    else
      m_bSkipAll = dlg.m_bSkipAll;
  }

  return sFile;
}

//////////////////////////////////////////////////////////////////

OdDbHostAppProgressMeter* DgnExportServices::newProgressMeter()
{
  return OdExDgnHostAppServices::newProgressMeter();
}

void DgnExportServices::start(const OdString& displayString)
{
  m_Msg = (LPCTSTR)displayString;
  m_nProgressPos = 0;
  m_nPercent = -1;
}

/************************************************************************/
/* Progress Meter override                                              */
/************************************************************************/
void DgnExportServices::stop()
{
  m_nProgressPos = m_nProgressLimit;
  meterProgress();
}

/************************************************************************/
/* Progress Meter override                                              */
/************************************************************************/
void DgnExportServices::meterProgress()
{
  int nOldPercent = m_nPercent;

  m_nPercent = int( ( double(m_nProgressPos++) / double(m_nProgressLimit)) * 100 );

  if( nOldPercent!=m_nPercent && m_pWndStatusBar )
  {
    OdString str;

    if( m_Msg.isEmpty() )
      str.format(_T("Loading DWG file... %d"), m_nPercent);
    else
      str.format(_T("%s %d"), m_Msg.c_str(), m_nPercent);

    ((CMainFrame*)(AfxGetApp()->GetMainWnd()))->m_wndStatusBar.SetPaneText(0, str);
    MSG msg;
    while(::PeekMessage(&msg, AfxGetApp()->GetMainWnd()->m_hWnd, WM_KEYUP, WM_KEYUP, 1))
    {
      bool bDup = false;
      if(msg.wParam == VK_ESCAPE && !bDup)
      {
        bDup = true;
        str.format(_T("Are you sure you want to terminate\n%s ?"), m_Msg);
        if(::AfxMessageBox(str, MB_YESNO|MB_ICONQUESTION)==IDYES)
        {
          throw UserBreak();
        }
      }
    }
  }
}

/************************************************************************/

void DgnExportServices::setLimit(int max)
{
  m_nProgressLimit = max ? max : 1;
}

//////////////////////////////////////////////////////////////////
//      HIODWGImpl.cpp implementation of CDgnDumper class       //
//////////////////////////////////////////////////////////////////

CDgnDumper::CDgnDumper()
{
  m_lModelKey       = 0;
  m_strFileName     = "";
  m_bInitSuccessful = false;
}

///////////////////////////////////////////////////////////////////

CDgnDumper::~CDgnDumper()
{
  uninitialize();
}

///////////////////////////////////////////////////////////////////

bool CDgnDumper::initialize()
{
  bool bRet = false;
  m_strErrorDescription.empty();

  if( m_strFileName != "" && !m_bInitSuccessful )
  {
#ifndef _TOOLKIT_IN_DLL_
    ODRX_INIT_STATIC_MODULE_MAP();
#endif
    odrxInitialize(&svcs);
    OdRxModulePtr m_pDGNDirect = ::odrxDynamicLinker()->loadModule(L"TG_Db", false);

    if( !m_pDGNDirect.isNull() )
    {
      try
      {
        m_pDb = svcs.readFile( m_strFileName );
      }
      catch(const OdError& e)
      {
        if( !m_pDb.isNull() )
          m_pDb.release();

        m_strErrorDescription = e.description();
      }
      catch(...)
      {
        if( !m_pDb.isNull() )
          m_pDb.release();
      }

      if( m_pDb.isNull() )
      {
        m_bInitSuccessful = false;
        ::odrxDynamicLinker()->unloadModule(OD_T("TG_Db"));
        odrxUninitialize();
      }
      else
        m_bInitSuccessful = true;

      bRet = true;
    }
  }

  return bRet;
}
///////////////////////////////////////////////////////////////////

bool CDgnDumper::loadToHoops()
{
  if( m_bInitSuccessful )
  {
    HoopsExportParams params;

    params.iStartKey   = m_lModelKey;
    params.bSetCamera  = true;
    params.pDb         = m_pDb;
    params.strFileName = "";
    params.background = RGB( 255,255,0);
    params.flags |= SHX_TEXT_AS_GEOMETRY;
    params.bExportCurrentLayout = true;
    params.iExportFileType = DGN_FILE_TYPE;

    if( m_pDb->getColorTable().isNull() )
    {
      params.setPalette( OdDgColorTable::defaultPalette() );
    }
    else
    {
      params.setPalette( m_pDb->getColorTable()->palette() );
    }

    AfxGetApp()->BeginWaitCursor();
    ((CMainFrame*)(AfxGetApp()->GetMainWnd()))->m_wndStatusBar.SetPaneText(0, L"Converting to hoops...");
    OdInt32 iRes = exportHoops( params );
    AfxGetApp()->EndWaitCursor();

    return (iRes == ::exOk);
  }

  return false;
}

///////////////////////////////////////////////////////////////////

void CDgnDumper::uninitialize()
{
  if( m_bInitSuccessful )
  {
    m_pDb = 0;
    ::odrxDynamicLinker()->unloadModule(OD_T("TG_Db"));
    odUninitialize();
  }

  m_bInitSuccessful = false;
}

///////////////////////////////////////////////////////////////////

HC_KEY    CDgnDumper::getModelKey()
{
  return m_lModelKey;
}

///////////////////////////////////////////////////////////////////

void      CDgnDumper::setModelKey( HC_KEY lNewKey )
{
  m_lModelKey = lNewKey;
}

///////////////////////////////////////////////////////////////////

OdString  CDgnDumper::getFilePath()
{
  return m_strFileName;
}

///////////////////////////////////////////////////////////////////

void      CDgnDumper::setFilePath( OdString strPath )
{
  m_strFileName = strPath;
}

///////////////////////////////////////////////////////////////////

OdString CDgnDumper::getLastErrorDescription()
{
  return m_strErrorDescription;
}

///////////////////////////////////////////////////////////////////
