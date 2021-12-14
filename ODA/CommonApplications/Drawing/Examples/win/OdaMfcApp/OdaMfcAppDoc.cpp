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

// OdaMfcAppDoc.cpp : implementation of the COdaMfcAppDoc class
//

#include "stdafx.h"
#include <atlconv.h>
#include "RxDictionary.h"
#include "DynamicLinker.h"
#include "OdaMfcApp.h"
#include "DebugStuff.h"
#include "DwgViewer.h"
#include "MaterialsEditor.h"

#include "OdaMfcAppDoc.h"
#include "DwgView.h"
#include "OdStreamBuf.h"
#include "OdaMfcApp.h"
#include "MainFrm.h"
#include "DbTransactionReactor.h"
#include "DbSafeTransaction.h"
#include "DbDatabaseReactor.h"

#include "DbBlockTable.h"
#include "DbBlockTableRecord.h"
#include "DbSymUtl.h"
#include "DbEntity.h"
#include "DbBlockReference.h"
#include "FileDlgExt.h"
#include "PreviewDib.h"
#include "AuditDlg.h"
#include "AppAuditInfo.h"
#include "SetActiveLayoutDlg.h"
#include "DbAbstractViewportData.h"
#include "DbPoint.h"
#include "DbLayout.h"
#include "LoadLineTypes.h"
#include "OdaLayerPropManagerDlg.h" // AE 18.02.2003 
#include "OdaMultiLineStyleDlg.h"   // OSHA 19.08.2005 
#include "NamedViewsDlg.h"
#include "Ed/EdCommandStack.h"
#include "RemoteGeomViewer.h"
#include "MemoryStream.h"
#include "RxRasterServices.h"
#include "Gi/GiRasterImage.h"
#include "DbGsManager.h"
#include "PdfImportDialog.h"


#if defined (ENABLE_DWF_EXPORT_SUPPORT) || (ENABLE_3DDWF_EXPORT_SUPPORT)
#include "DwfExport.h"              // MKU 04.01.2003
using namespace TD_DWF_EXPORT;
#endif

#include "PublishFileDlg.h"         // MKU 25.06.2003
#include "LwSettingsDlg.h"
#include "XrefViewDlg.h"
#include "RxVariantValue.h"
#include "ColorMapping.h"
#include "SummDlg.h"
#include "PageSetupDlg.h"                                                               // MKU 24.05.2004
#include "winspool.h"                                                       // printer  // MKU 24.05.2004
#include "XRefMan.h"
#include "DbSSet.h"
#include "ExDbCommandContext.h"
#include "ExStringIO.h"
#include "UserIOConsole.h"
#include ".\odamfcappdoc.h"
#include "DbSortentsTable.h"
#include "CreateDlg.h"
#include "PsTbEditDlg.h"
#include "SaveState.h"
#include "EditProperties.h"
#include "DwfUnderlayDialog.h"
#define OD_GIGRADIENTGENERATOR_LINKED
#ifdef OD_GIGRADIENTGENERATOR_LINKED
#include "ColorPickerDlg.h"
#endif
#include "Ed/EdLispEngine.h"
#include "DbObjectContextCollection.h"
#include "DbObjectContextManager.h"
#include "FdField.h"
#include "OdValue.h"
#include "DbHistoryManager.h"
#include "summinfo.h"

//#define ENABLE_HOOPS_EXPORT
#ifdef ENABLE_HOOPS_EXPORT
#pragma comment(lib,"TD_HoopsExport.lib")
#include "HoopsExport.h"

#endif

#ifdef ENABLE_COLLADA_EXPORT_SUPPORT
#pragma comment(lib,"TD_ColladaExport.lib")
#include "ColladaModule.h"
using namespace TD_COLLADA_EXPORT;
#endif

#include "PdfExport.h"
#include "Export2PDFDialog.h"
using namespace TD_PDF_2D_EXPORT;

#include "Tf/TfRevisionControl.h"
#include "RevisionDialog.h"
#include "CommitMessageDialog.h"

#include "OdaTrianParamsDlg.h"
#include "ModelerGeometry/ModelerModule.h"
#include "Wr/wrTriangulationParams.h"

#ifdef DWG2IFC_SUPPORT
#include "Imports/Dwg2Ifc/Include/IfcImport.h"
using namespace DWG_IFC_IMPORT;
#endif // DWG2IFC_SUPPORT

/////////////////////////////////////////////////////////////////////////////
// COdaMfcAppDoc

IMPLEMENT_DYNCREATE(COdaMfcAppDoc, COleDocument)

#define NEW_CONSTR(CLASS) OdSmartPtr<CLASS>(new CLASS, kOdRxObjAttach)

ODRX_CONS_DEFINE_MEMBERS(OdDbDatabaseDoc, PARAMDATABASE, NEW_CONSTR);

COdaMfcAppDoc* OdDbDatabaseDoc::g_pDoc = 0;

OdDbDatabaseDoc::OdDbDatabaseDoc()
  : m_pDoc(g_pDoc)
{
  g_pDoc = 0;
}

COdaMfcAppDoc* OdDbDatabaseDoc::document() const
{
  return m_pDoc;
}

void OdDbDatabaseDoc::setDocToAssign(COdaMfcAppDoc* pDoc)
{
  g_pDoc = pDoc;
}

BEGIN_MESSAGE_MAP(COdaMfcAppDoc, CDocument)
  //{{AFX_MSG_MAP(COdaMfcAppDoc)
  ON_COMMAND(ID_REFRESH, OnRefresh)
  ON_COMMAND(ID_EDIT_AUDIT, OnEditAudit)
  ON_COMMAND(ID_VIEW_SETACTIVELAYOUT, OnViewSetactivelayout)
  ON_COMMAND(ID_VIEW_CONSTRDISPLAY, OnViewConstrDisplay)
  ON_UPDATE_COMMAND_UI(ID_VIEW_CONSTRDISPLAY, OnUpdateViewConstrDisplay)
  ON_COMMAND(ID_VIEW_NAMEDVIEWS, OnViewNamedViews)
  ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
  ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
  ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, OnUpdateEditRedo)
  ON_COMMAND(ID_EDIT_REDO, OnEditRedo)
  ON_COMMAND(ID_EDIT_ZOOMEXTENTS, OnEditZoomextents)
  ON_COMMAND(ID_EDIT_IDEDITLINETYPE, OnEditIdeditlinetype)
  ON_COMMAND(ID_VIEW_SUMMINFO, OnViewSumminfo)
  ON_COMMAND(ID_FILE_EXPORT_PDF, OnExportPdf)                                         // AlexR 062705
  ON_COMMAND(ID_FILE_EXPORT_HOOPS, OnExportHoops)                                     // RAV 12.26.08
  ON_COMMAND(ID_FILE_EXPORT_COLLADA, OnExportCollada)                                 // ET Feb 08, 2012
  ON_COMMAND(ID_FILE_EXPORTTOIFC, OnExportIfc)                                        // AMatvienko, exportToIfc
  ON_COMMAND(ID_FILE_EXPORT, OnExport)                                                // MKU 04.01.2003
  ON_COMMAND(ID_FILE_PUBLISH, OnPublish)                                              // MKU 25.06.2003
  ON_COMMAND(ID_FILE_3DPUBLISH, On3DPublish)                                          // MKU 09.08.2005
  ON_COMMAND(ID_FILE_BMPEXPORT, OnBmpExport)                                          // AMark 28.12.2009
  ON_UPDATE_COMMAND_UI(ID_FILE_EXPORT_HOOPS, OnUpdateHoopsExport)                     // RAV 12.26.08
  ON_UPDATE_COMMAND_UI(ID_FILE_EXPORT_COLLADA, OnUpdateColladaExport)                 // ET Feb 08, 2012
  ON_UPDATE_COMMAND_UI(ID_FILE_EXPORT_PDF, OnUpdatePdfExport)                         // AlexR 093005
  ON_UPDATE_COMMAND_UI(ID_FILE_EXPORT, OnUpdateExport)                                // MKU 26.06.2003
  ON_UPDATE_COMMAND_UI(ID_FILE_PUBLISH, OnUpdatePublish)                              // MKU 26.06.2003
  ON_UPDATE_COMMAND_UI(ID_FILE_3DPUBLISH, OnUpdate3DPublish)                          // MKU 09.08.2005
  ON_UPDATE_COMMAND_UI(ID_FILE_BMPEXPORT, OnUpdateBmpExport)                          // AMark 28.12.2009
  ON_UPDATE_COMMAND_UI(ID_FILE_EXPORTTOIFC, OnUpdateIfcExport)                        // AMatvienko, exportToIfc
  ON_COMMAND(ID_FORMAT_LAYER, OnFormatLayer)                                          // AE 18.02.2003 
  ON_COMMAND(ID_FORMAT_LINETYPE, OnFormatLinetype)                                    // OSHA 18.08.2005
  ON_COMMAND(ID_FORMAT_MLSTYLE, OnFormatMlstyle)                                      // OSHA 19.08.2005
  ON_COMMAND(ID_FORMAT_BACKGROUNDCOLOR, OnFormatBackgroundColor)
  ON_COMMAND(ID_FORMAT_LINEWEIGHTSETTINGS, OnFormatLineweightsettings)
  ON_COMMAND(ID_INSERT_XREFMANAGER, OnInsertXrefmanager)
  ON_COMMAND(ID_FILE_IMPORT_DWF, OnImportDwf)                                         // WVK 01.10.2003
  ON_COMMAND(ID_FILE_IMPORT_COLLADA, OnImportCollada)
  ON_COMMAND(ID_FILE_IMPORT_PDF, OnImportPdf)
  ON_COMMAND(ID_VECTORIZE, OnVectorize)
  ON_COMMAND(ID_DEBUG_GEOMETRY, OnRemoteGeomViewer)
  ON_COMMAND(ID_FILE_EXPORT_SVG, OnExportSvg)
  ON_COMMAND(ID_EDIT_CONSOLE, OnEditConsole)
  ON_COMMAND(ID_OPTIONS_TRIANGULATION_PARAMS, OnOptionsTriangulationParams)
  ON_COMMAND(ID_FILE_PAGE_SETUP, OnPageSetup)                                         // MKU 24.05.2004
  ON_COMMAND(ID_EDIT_RECENTCOMMAND, OnEditRecentcommand)
  ON_UPDATE_COMMAND_UI(ID_EDIT_RECENTCOMMAND, OnUpdateEditRecentcommand)
  ON_COMMAND(ID_PURGE_ALL, OnPurgeAll)
  ON_UPDATE_COMMAND_UI(ID_PURGE_ALL, OnUpdatePurge)
  ON_COMMAND(ID_PURGE_BLOCKS, OnPurgeBlocks)
  ON_COMMAND(ID_PURGE_DIM_STYLES, OnPurgeDimStyles)
  ON_COMMAND(ID_PURGE_LAYERS, OnPurgeLayers)
  ON_COMMAND(ID_PURGE_LTYPES, OnPurgeLtypes)
  ON_COMMAND(ID_PURGE_TEXT_STYLES, OnPurgeTextStyles)
  ON_COMMAND(ID_INSERT_OLEOBJECT, OnInsertOleobject)
  ON_COMMAND(ID_INSERT_DWFUNDERLAY, OnInsertDwfUnderlay)
  ON_COMMAND(ID_INSERT_DGNUNDERLAY, OnInsertDgnUnderlay)                             // AMark 06.12.2007
  ON_COMMAND(ID_INSERT_PDFUNDERLAY, OnInsertPdfUnderlay)
  ON_COMMAND(ID_FILE_PLOT_STYLE_MANAGER, OnFilePlotStyleManager)                     // OSHA 02.02.2006
  ON_UPDATE_COMMAND_UI(ID_VECTORIZE, OnUpdateVectorize)
  ON_UPDATE_COMMAND_UI(ID_PURGE_BLOCKS, OnUpdatePurge)
  ON_UPDATE_COMMAND_UI(ID_PURGE_DIM_STYLES, OnUpdatePurge)
  ON_UPDATE_COMMAND_UI(ID_PURGE_LAYERS, OnUpdatePurge)
  ON_UPDATE_COMMAND_UI(ID_PURGE_LTYPES, OnUpdatePurge)
  ON_UPDATE_COMMAND_UI(ID_PURGE_TEXT_STYLES, OnUpdatePurge)
  ON_COMMAND(ID_EDIT_REDUCE, OnEditReduce)
  ON_COMMAND(ID_EDIT_SELECT_ALL, OnEditSelectall)
  ON_COMMAND(ID_EDIT_CLEARSELECTION, OnEditClearselection)
  ON_COMMAND(ID_EXPLODE_ALL, OnEditExplode)
  ON_COMMAND(ID_EDIT_ENTGET, OnEditEntget)
  ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
  ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
  ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
  ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
  ON_COMMAND(ID_OPTIONS_DISABLE_ANNOTATIONS, OnOptionsDisableAnnotations)
  ON_UPDATE_COMMAND_UI(ID_OPTIONS_DISABLE_ANNOTATIONS, OnOptionsDisableAnnotationsUI)
  ON_COMMAND(ID_OPTIONS_DISABLEFIELDUPDATEONLOAD, OnOptionsDisableFieldUpdate)
  ON_UPDATE_COMMAND_UI(ID_OPTIONS_DISABLEFIELDUPDATEONLOAD, OnOptionsDisableFieldUpdateUI)
  ON_COMMAND(ID_VIEW_RENDER_DBLIGHTS, OnViewRenderDbLights)
  ON_COMMAND(ID_VIEW_RENDER_DBMATERIALS, OnViewRenderDbMaterials)
  ON_COMMAND(ID_VIEW_RENDER_DBVISUALSTYLES, OnViewRenderDbVisualStyles)
  ON_COMMAND(ID_VIEW_RENDER_ENVIRONMENT, OnViewRenderEnvironment)
  ON_COMMAND(ID_TOOLS_REVISIONLOG, OnRevisionLog)
  ON_UPDATE_COMMAND_UI(ID_TOOLS_REVISIONLOG, OnRevisionLogUI)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COdaMfcAppDoc construction/destruction

unsigned short COdaMfcAppDoc::ClipboardData::m_FormatR15 = (CLIPFORMAT)::RegisterClipboardFormat(_T("AutoCAD.r15"));  // r15
unsigned short COdaMfcAppDoc::ClipboardData::m_FormatR16 = (CLIPFORMAT)::RegisterClipboardFormat(_T("AutoCAD.r16"));  // r18
unsigned short COdaMfcAppDoc::ClipboardData::m_FormatR17 = (CLIPFORMAT)::RegisterClipboardFormat(_T("AutoCAD.r17"));  // r21
unsigned short COdaMfcAppDoc::ClipboardData::m_FormatR18 = (CLIPFORMAT)::RegisterClipboardFormat(_T("AutoCAD.r18"));  // r24
unsigned short COdaMfcAppDoc::ClipboardData::m_FormatR19 = (CLIPFORMAT)::RegisterClipboardFormat(_T("AutoCAD.r19"));  // r27

COdaMfcAppDoc* g_pDoc = 0;

COdaMfcAppDoc::COdaMfcAppDoc()
  : m_bPartial(false)
  , m_pViewer(0)
  , m_SaveAsType(OdDb::kDwg)
  , m_SaveAsVer(OdDb::kDHL_CURRENT)
  , m_bConsole(false)
  , m_nCmdActive(0)
  , m_bLayoutSwitchable(false)
  , m_bDisableClearSel(false)
  , m_bTryIncSave(false)
{
  g_pDoc = this;

#ifdef ODAMFC_EXPORT_SYMBOL
  m_pRefDocument = OdApDocumentImpl::createObject( this );
#endif // ODAMFC_EXPORT_SYMBOL
}

COdaMfcAppDoc::~COdaMfcAppDoc()
{
#ifdef ODAMFC_EXPORT_SYMBOL
  m_pRefDocument->m_pImp->SetNull();
#endif // ODAMFC_EXPORT_SYMBOL
}

#include "ExTraceDbReactor.h"
#include "ExTraceDbReactor.cpp"
BOOL COdaMfcAppDoc::OnNewDocument() 
{
#ifdef ODAMFC_EXPORT
  COdaMfcAppApp* pApp = ( COdaMfcAppApp* ) AfxGetApp();
  size_t i, iSize = pApp->m_aAppReactors.size();
  for ( i = 0; i < iSize; i++ )
    pApp->m_aAppReactors[ i ]->documentCreateStarted( this );
#endif // ODAMFC_EXPORT
  if(COleDocument::OnNewDocument())
  {
    CCreateDlg askDlg;
    if ( pApp->remoteGeomViewer() || askDlg.DoModal() == IDOK )
    {
      OdDb::MeasurementValue measure = OdDb::kEnglish;
      if ( !askDlg.IsEnglishChoice() )
        measure = OdDb::kMetric;
      
      OdDbDatabaseDoc::setDocToAssign(this);
      try
      {
        m_pDb = theApp.createDatabase(true, measure);
      }
      catch(const OdError& e)
      {
        m_pDb = 0;
        pApp->reportError(_T("Database Creating Error..."), e);
        return FALSE;
      }

      TRACE_DB_EVENTS(m_pDb);
      if (!m_pDb.isNull())
        m_pDb->appServices()->layoutManager()->addReactor(this);
      
      if ( pApp->remoteGeomViewer() ) {
        theApp.setStatusText(_T("Waiting for remote geometry."));
        SetTitle(_T("Remote Geometry Viewer"));
      }
      else
        theApp.setStatusText(_T("Default Database Created."));

#ifdef ODAMFC_EXPORT
      for ( i = 0; i < iSize; i++ )
        pApp->m_aAppReactors[ i ]->documentCreated( this );
#endif // ODAMFC_EXPORT
      return TRUE;
    }
  }
#ifdef ODAMFC_EXPORT
  for ( i = 0; i < iSize; i++ )
    pApp->m_aAppReactors[ i ]->documentCreateCanceled( this );
#endif // ODAMFC_EXPORT

  return FALSE;
}
#include "DbDatabaseCollection.h"
BOOL COdaMfcAppDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
  OdDbDatabaseDoc::setDocToAssign(this);

  // Check if ready in-memory database to be used
  // Get Currently active document
  COdaMfcAppDoc* pActiveDoc = NULL;
  CWnd* pWndMain = AfxGetMainWnd();
  ASSERT(pWndMain);
  ASSERT(pWndMain->IsKindOf(RUNTIME_CLASS(CMDIFrameWnd))); // Not an MDI app.
  CFrameWnd* pFrame = ((CMDIFrameWnd*)pWndMain)->MDIGetActive();
  if(NULL != pFrame)
  {
      pActiveDoc = (COdaMfcAppDoc*)pFrame->GetActiveDocument(); // get the active document
  }
  OdDbDatabasePtr pInMemoryDb;
  if (pActiveDoc != NULL)
  {
    OdEdCommandContext* pCmdCtx = pActiveDoc->cmdCtx();
    if (pCmdCtx)
    {
      pInMemoryDb = pCmdCtx->arbitraryData(L"OdDbDatabasePtr");
      if (!pInMemoryDb.isNull())
      { // Clean the value
        pCmdCtx->setArbitraryData(L"OdDbDatabasePtr", NULL);
      }
    }
  }

  if (!pInMemoryDb.isNull())
  {
    m_pDb = pInMemoryDb;
  }
  else
  {
    m_pDb = theApp.openFile(lpszPathName);
  }
  //OdDbXRefMan::loadAll(m_pDb);
  for ( OdRxIteratorPtr it = oddbDatabaseCollection()->newIterator(); !it->done(); it->next())
    TRACE_DB_EVENTS(OdDbDatabasePtr(it->object()));
  if (!m_pDb.isNull())
  {
    m_pDb->appServices()->layoutManager()->addReactor(this);
    if (!theApp.m_bDisableFieldUpdateOnOpen && (m_pDb->getFIELDEVAL() & OdDbField::kOpen) != 0)
      ::oddbEvaluateFields(m_pDb, OdDbField::kOpen);
  }
  return (!m_pDb.isNull());
}

void COdaMfcAppDoc::DeleteContents() 
{
#ifdef ODAMFC_EXPORT
  COdaMfcAppApp* pApp = ( COdaMfcAppApp* ) AfxGetApp();
  size_t i, iSize = pApp->m_aAppReactors.size();
  for ( i = 0; i < iSize; i++ )
    pApp->m_aAppReactors[ i ]->documentToBeDestroyed( this );
#endif // ODAMFC_EXPORT

  if (!m_pDb.isNull())
    m_pDb->appServices()->layoutManager()->removeReactor(this);
  m_pDb.release();

  COleDocument::DeleteContents();

#ifdef ODAMFC_EXPORT
  for ( i = 0; i < iSize; i++ )
    pApp->m_aAppReactors[ i ]->documentDestroyed( ( const wchar_t* ) GetPathName() );
#endif // ODAMFC_EXPORT
}

#include "OdaMfcAppSaveAsFiltersEnum.h"

CString generateFiltersForDialog()
{
  static const TCHAR* filterStrings[][2] = 
  {
    { _T(""), _T("") },
#define OD_MFCAPP_SAVEAS_FILTER_DEF(code, desc, ext)  { desc, ext },
#include "OdaMfcAppSaveAsFiltersDef.h"
#undef OD_MFCAPP_SAVEAS_FILTER_DEF
    { _T(""), _T("") }
  };
  
  CString strFilter = _T("");
  DWORD c = kDummyFirstFilter + 1;
  while (c < kDummyLastFilter)
  {
    strFilter += filterStrings[c][0];
    strFilter += (TCHAR)'\0';   // next string please
    strFilter += filterStrings[c][1];
    strFilter += (TCHAR)'\0';   // last string

    c++;
  }

  return strFilter;
}

BOOL COdaMfcAppDoc::DoPromptFileName(CString& fileName, UINT nIDSTitle, DWORD lFlags, BOOL bOpenFileDialog, CDocTemplate* /*pTemplate*/)
{
  //OdDb::DwgVersion dwgver = m_pDb->originalFileVersion();

  OdInt32 numOfSavesSinceOpened = m_pDb->numberOfSaves();
  OdDb::DwgVersion dwgver = (numOfSavesSinceOpened > 0) ? m_pDb->lastSavedAsVersion() : 
                                                          m_pDb->originalFileVersion();

  CString ext = fileName.Right(3);
  bool isDwg = ext.CompareNoCase(_T("dxf")) != 0,
       isDxb = (m_pDb->originalFileType() == OdDb::kDxb);

  CFileDialog dlgFile(bOpenFileDialog);

  CString title;
  VERIFY(title.LoadString(nIDSTitle));

  dlgFile.m_ofn.Flags |= lFlags;


  //Add filters for all supported file type:
  CString strFilter = generateFiltersForDialog();
  dlgFile.m_ofn.lpstrFilter = strFilter;
  dlgFile.m_ofn.nMaxCustFilter = kDummyLastFilter - 1;

  //Set default filter:
  if (!isDwg && dwgver == OdDb::vAC32)
    dlgFile.m_ofn.nFilterIndex = kDxfR32;
  if (!isDwg && (dwgver == OdDb::vAC27 || dwgver == OdDb::kDHL_1027))
    dlgFile.m_ofn.nFilterIndex = kDxfR27;
  if (!isDwg && (dwgver == OdDb::vAC24 || dwgver == OdDb::kDHL_1024))
    dlgFile.m_ofn.nFilterIndex = kDxfR24;
  if (!isDwg && (dwgver == OdDb::vAC21 || dwgver == OdDb::kDHL_1021))
    dlgFile.m_ofn.nFilterIndex = kDxfR21;
  if (!isDwg && (dwgver == OdDb::kDHL_1800a || dwgver == OdDb::kDHL_1800))
    dlgFile.m_ofn.nFilterIndex = kDxfR18;
  if (!isDwg && dwgver ==OdDb::vAC15)
    dlgFile.m_ofn.nFilterIndex = kDxfR15;
  if (!isDwg && dwgver ==OdDb::vAC14)
    dlgFile.m_ofn.nFilterIndex = kDxfR14;
  if (!isDwg && dwgver ==OdDb::vAC13)
    dlgFile.m_ofn.nFilterIndex = kDxfR13;
  if (!isDwg && dwgver ==OdDb::vAC12)
    dlgFile.m_ofn.nFilterIndex = kDxfR12;
  if (!isDwg && dwgver ==OdDb::vAC10)
    dlgFile.m_ofn.nFilterIndex = kDxfR10;
  if (!isDwg && dwgver ==OdDb::vAC09)
    dlgFile.m_ofn.nFilterIndex = kDxfR9;

  if (isDxb && (dwgver == OdDb::kDHL_2700a || dwgver == OdDb::kDHL_1027))
    dlgFile.m_ofn.nFilterIndex = kBinDxfR27;
  if (isDxb && (dwgver == OdDb::kDHL_2400a || dwgver == OdDb::kDHL_1024))
    dlgFile.m_ofn.nFilterIndex = kBinDxfR24;
  if (isDxb && (dwgver == OdDb::kDHL_2100a || dwgver == OdDb::kDHL_1021))
    dlgFile.m_ofn.nFilterIndex = kBinDxfR21;
  if (isDxb && (dwgver == OdDb::kDHL_1800a || dwgver == OdDb::kDHL_1800))
    dlgFile.m_ofn.nFilterIndex = kBinDxfR18;
  if (isDxb && dwgver ==OdDb::vAC15)
    dlgFile.m_ofn.nFilterIndex = kBinDxfR15;
  if (isDxb && dwgver ==OdDb::vAC14)
    dlgFile.m_ofn.nFilterIndex = kBinDxfR14;
  if (isDxb && dwgver ==OdDb::vAC13)
    dlgFile.m_ofn.nFilterIndex = kBinDxfR13;
  if (isDxb && dwgver ==OdDb::vAC12)
    dlgFile.m_ofn.nFilterIndex = kBinDxfR12;
  if (isDxb && dwgver ==OdDb::vAC10)
    dlgFile.m_ofn.nFilterIndex = kBinDxfR10;

  if (isDwg && dwgver == OdDb::vAC32)
    dlgFile.m_ofn.nFilterIndex = kDwgR32;
  if (isDwg && dwgver == OdDb::vAC27)
    dlgFile.m_ofn.nFilterIndex = kDwgR27;
  if (isDwg && dwgver == OdDb::vAC24)
    dlgFile.m_ofn.nFilterIndex = kDwgR24;
  if (isDwg && dwgver == OdDb::vAC21)
    dlgFile.m_ofn.nFilterIndex = kDwgR21;
  if (isDwg && (dwgver == OdDb::kDHL_1800a || dwgver == OdDb::kDHL_1800))
    dlgFile.m_ofn.nFilterIndex = kDwgR18;
  if (isDwg && dwgver ==OdDb::vAC15)
    dlgFile.m_ofn.nFilterIndex = kDwgR15;
  if (isDwg && dwgver ==OdDb::vAC14)
    dlgFile.m_ofn.nFilterIndex = kDwgR14;
  if (isDwg && dwgver ==OdDb::vAC13)
    dlgFile.m_ofn.nFilterIndex = kDwgR13;
  if (isDwg && dwgver <= OdDb::vAC12)
    dlgFile.m_ofn.nFilterIndex = kDwgR12;


  if (fileName.Find('.') != -1)
  {
    fileName = fileName.Left(fileName.Find('.'));
  }

  
  dlgFile.m_ofn.lpstrTitle = title;
  dlgFile.m_ofn.lpstrFile = fileName.GetBuffer(_MAX_PATH);

  LPARAM nResult = dlgFile.DoModal();
  fileName.ReleaseBuffer();

  //Set extension by type if needed:
  if (fileName.Find('.') == -1)
  {
    if ( (dlgFile.m_ofn.nFilterIndex >= kDxfR32 && dlgFile.m_ofn.nFilterIndex <= kBinDxfR10) )
      fileName += _T(".dxf");
    else if ( (dlgFile.m_ofn.nFilterIndex >= kDwgR32 && dlgFile.m_ofn.nFilterIndex <= kDwgR12) )
      fileName += _T(".dwg");
    else if (dlgFile.m_ofn.nFilterIndex == kTeighaFormat)
      fileName += _T(".tig");
    else if (dlgFile.m_ofn.nFilterIndex == kTeighaBinaryFormat)
      fileName += _T(".dsf");
  }

  //Set m_SaveAsType:
  if( (dlgFile.m_ofn.nFilterIndex >= kDxfR32 && dlgFile.m_ofn.nFilterIndex <= kDxfR9) )
  {
    m_SaveAsType = OdDb::kDxf;
  }
  else if( (dlgFile.m_ofn.nFilterIndex >= kBinDxfR27 && dlgFile.m_ofn.nFilterIndex <= kBinDxfR10) )
  {
    m_SaveAsType = OdDb::kDxb;
  }
  else
  {
    m_SaveAsType = OdDb::kDwg;
  }

  //Set m_SaveAsVer:
  switch(dlgFile.m_ofn.nFilterIndex)
  {
  case kDxfR32:
  case kDwgR32:
    m_SaveAsVer = OdDb::vAC32;
    break;
  case kDxfR27:
  case kBinDxfR27:
  case kDwgR27:
    m_SaveAsVer = OdDb::vAC27;
    break;
  case kDxfR24:
  case kBinDxfR24:
  case kDwgR24:
    m_SaveAsVer = OdDb::vAC24;
    break;
  case kDxfR21:
  case kBinDxfR21:
  case kDwgR21:
    m_SaveAsVer = OdDb::vAC21;
    break;
  case kDxfR18:
  case kBinDxfR18:
  case kDwgR18:
    m_SaveAsVer = OdDb::vAC18;
    break;
  case kDxfR15:
  case kBinDxfR15:
  case kDwgR15:
    m_SaveAsVer = OdDb::vAC15;
    break;
  case kDxfR14:
  case kBinDxfR14:
  case kDwgR14:
    m_SaveAsVer = OdDb::vAC14;
    break;
  case kDxfR13:
  case kBinDxfR13:
  case kDwgR13:
    m_SaveAsVer = OdDb::vAC13;
    break;
  case kDxfR12:
  case kBinDxfR12:
  case kDwgR12:
    m_SaveAsVer = OdDb::vAC12;
    break;
  case kDxfR10:
  case kBinDxfR10:
    m_SaveAsVer = OdDb::vAC10;
    break;
  case kDxfR9:
    m_SaveAsVer = OdDb::vAC09;
    break;
  default:
    m_SaveAsVer = m_pDb->originalFileVersion();
  };

  return nResult == IDOK;
}

BOOL COdaMfcAppDoc::DoSave(LPCTSTR lpszPathName, BOOL bReplace)
// Save the document data to a file
// lpszPathName = path name where to save document file
// if lpszPathName is NULL then the user will be prompted (SaveAs)
// note: lpszPathName can be different than 'm_strPathName'
// if 'bReplace' is TRUE will change file name if successful (SaveAs)
// if 'bReplace' is FALSE will not change path name (SaveCopyAs)
{
  //m_SaveAsVer = m_pDb->originalFileVersion();

  OdInt32 numOfSavesSinceOpened = m_pDb->numberOfSaves();
  m_SaveAsVer = (numOfSavesSinceOpened > 0) ? m_pDb->lastSavedAsVersion() : 
                                              m_pDb->originalFileVersion();
    
  CString newName = lpszPathName;
  CString extension = newName.Right(3);

  if ( (newName.IsEmpty()) || ((extension.CompareNoCase(_T("dwg")) != 0) && (extension.CompareNoCase(_T("tig")) != 0) && (extension.CompareNoCase(_T("dsf")) != 0)) )
  {
    setTryIncSave(false);

    CDocTemplate* pTemplate = GetDocTemplate();
    ASSERT(pTemplate != NULL);

    newName = m_strPathName;
    if (bReplace && newName.IsEmpty())
    {
      newName = m_strTitle;
      // check for dubious filename
      int iBad = newName.FindOneOf(_T(" #%;/\\"));
      if (iBad != -1)
        newName.ReleaseBuffer(iBad);

      // append the default suffix if there is one
      CString strExt;
      if (pTemplate->GetDocString(strExt, CDocTemplate::filterExt) &&
        !strExt.IsEmpty())
      {
        ASSERT(strExt[0] == '.');
        newName += strExt;
      }
    }

    if (!DoPromptFileName(newName,
              bReplace ? AFX_IDS_SAVEFILE : AFX_IDS_SAVEFILECOPY,
              OFN_HIDEREADONLY | OFN_EXPLORER | OFN_PATHMUSTEXIST, FALSE, pTemplate))
                    return FALSE;       // don't even attempt to save

  }
  else
  { 
    if (m_SaveAsVer == OdDb::kDHL_CURRENT)
    {
      setTryIncSave(true);
    }
    else
    {
      setTryIncSave(false);
    }
  }

  CWaitCursor wait;

  if (!OnSaveDocument(newName))
  {
    if (lpszPathName == NULL)
    {
      // be sure to delete the file
      TRY
      {
        CFile::Remove(newName);
      }
      CATCH_ALL(e)
      {
        TRACE0("Warning: failed to delete file after failed SaveAs.\n");
        do { e->Delete(); } while (0);
      }
      END_CATCH_ALL
    }
    return FALSE;
  }

  // reset the title and change the document name
  if (bReplace)
    SetPathName(newName);

  return TRUE;        // success
}

/////////////////////////////////////////////////////////////////////////////
// COdaMfcAppDoc diagnostics

#ifdef _DEBUG
void COdaMfcAppDoc::AssertValid() const
{
  COleDocument::AssertValid();
}

void COdaMfcAppDoc::Dump(CDumpContext& dc) const
{
  COleDocument::Dump(dc);
}
#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


void CMainFrame::OnFilePartialOpen()
{
  theApp.setPartialOption(true);
  theApp.OnFileOpen();
  theApp.setPartialOption(false);
}

void CMainFrame::OnFileRecover()
{
  theApp.setRecoverOption(true);
  theApp.OnFileOpen();
  theApp.setRecoverOption(false);
}

// ODA_MT_DBIO_BEGIN
void CMainFrame::OnFileMTOpen()
{
  theApp.setMTLoadingOption(true);
  theApp.OnFileOpen();
  theApp.setMTLoadingOption(false);
}
// ODA_MT_DBIO_END

/////////////////////////////////////////////////////////////////////////////
// COdaMfcAppDoc commands


void COdaMfcAppDoc::OnRefresh()
{
  // TODO: Add your command handler code here

}

void COdaMfcAppDoc::OnEditAudit()
{
  // Fix errors?
  bool bFixErrors =
    ::AfxMessageBox(_T("Fix any errors detected?"), MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON2)==IDYES;

  // Create dialog to print audit report
  COdaMfcAppApp* pApp = (COdaMfcAppApp*)AfxGetApp();
  ODA_ASSERT (!pApp->m_pAuditDlg);
  pApp->m_pAuditDlg = new CAuditDlg();
  if (!pApp->m_pAuditDlg)
  {
    ::AfxMessageBox(_T("Error Creating Audit Dialog Object"));
    return;
  }
  if (!pApp->m_pAuditDlg->Create(IDD_AUDITINFO))
  {
    ::AfxMessageBox(_T("Error Creating Audit Dialog Window"));
    return;
  }

  try
  {
    // Audit
    CAppAuditInfo aiAppAudit;
    aiAppAudit.setFixErrors(bFixErrors);
    aiAppAudit.setPrintDest(OdDbAuditInfo::kBoth);
    aiAppAudit.setHostAppServices(&theApp);

    m_pDb->auditDatabase(&aiAppAudit);
  }
  catch(const OdError& e)
  {
    delete pApp->m_pAuditDlg;
    pApp->m_pAuditDlg = NULL;
    theApp.reportError(_T("Error Auditing Database..."), e);
    AfxThrowUserException( );
  }
  catch(const UserBreak&)
  {
    delete pApp->m_pAuditDlg;
    pApp->m_pAuditDlg = NULL;
  }
  if (pApp->m_pAuditDlg == NULL)
    return;
  // Show result
  CString strTitle = _T("Audit info - ");
  strTitle += GetTitle();
  pApp->m_pAuditDlg->SetWindowText (strTitle);
  pApp->m_pAuditDlg->ShowWindow (SW_SHOW);

  // Dialog will be destroyed while exit
  pApp->m_pAuditDlg = NULL;
}

void CPreviewDIB::SetPreviewFile(LPCTSTR lpFileName)
{
  CString  csFileName(lpFileName);

  m_odImage.header.clear();
  m_odImage.bmp.clear();
  m_odImage.wmf.clear();
  m_odImage.png.clear();

  if ( ! csFileName.GetLength() )
    return;

  CString ext = csFileName.Right(4);
  ext.MakeLower();

  if ( ext == _T(".dwg") || ext == _T(".dxf") )
  {
    OdStreamBufPtr pFile;
    try
    {
      odDbGetPreviewBitmap(theApp.createFile(OdString((LPCTSTR)csFileName)), &m_odImage);
      m_odImage.convPngToBmp();
    }
    catch(...)
    {
      return;
    }
  }
}

CRect CPreviewDIB::Calc(int bmpWid, int bmpDep, int wndWid, int wndDep)
{
  int d,w,x,y;
  if(bmpDep > bmpWid)
  {
    d = __min(bmpDep,wndDep);
    w = (bmpWid*d)/bmpDep;
    if ( w > wndWid)
    {
      d = (d*wndWid)/w;
      w = wndWid;
    }
  }
  else
  {
    w = __min(bmpWid,wndWid);
    d = (bmpDep*w)/bmpWid;
    if ( d > wndDep )
    {
      w = (w*wndDep)/d;
      d = wndDep;
    }
  }
  x = (wndWid- w)/2;
  y = (wndDep - d)/2;
  return CRect(x,y,x+w,y+d);
}


// Placeable metafile data definitions
typedef struct tagOLDRECT
{
  short left;
  short top;
  short right;
  short bottom;
} OLDRECT;

// Placeable metafile header
typedef struct {
  DWORD   key;
  WORD    hmf;
  OLDRECT bbox;
  WORD    inch;
  DWORD   reserved;
  WORD    checksum;
} ALDUSMFHEADER;

#define METAFILE_VERSION    1
#define ALDUSKEY            0x9AC6CDD7
#define ALDUSMFHEADERSIZE   22  // Avoid sizeof is struct alignment >1

void CPreviewDIB::DrawPreview(HDC dc, int X, int Y, int width, int height)
{
  CRect   cr;

  if (m_odImage.hasBmp())
  {
    BITMAPINFOHEADER *pHeader;
    pHeader = (BITMAPINFOHEADER *)(m_odImage.bmp.begin() );

    cr = Calc(pHeader->biWidth, pHeader->biHeight,width,height);

    unsigned char *p = (unsigned char*)pHeader;
    p += pHeader->biSize;
    switch(pHeader->biBitCount)
    {
    case 1:
      p += sizeof(RGBQUAD)*2;
      break;
    case 4:
      p += sizeof(RGBQUAD)*16;
      break;
    case 8:
      p += sizeof(RGBQUAD)*256;
      break;
    }

    StretchDIBits(dc, cr.left+X, cr.top+Y, cr.Width(), cr.Height(), 0, 0,
      pHeader->biWidth, pHeader->biHeight, (const void *) p,
      (CONST BITMAPINFO * )pHeader, DIB_RGB_COLORS, SRCCOPY);
  }
  else if (m_odImage.hasWmf())
  {
    CDC     newDC;
    DWORD   dwIsAldus;
    METAHEADER*    mfHeader = NULL;
    ALDUSMFHEADER* aldusMFHeader= NULL;

    DWORD dwSize;
    DWORD seekpos;

    newDC.Attach(dc);
    dwIsAldus = *((DWORD*)m_odImage.wmf.begin());
    if (dwIsAldus != ALDUSKEY)
    {
      seekpos = 0;
    }
    else
    {
      aldusMFHeader = (ALDUSMFHEADER*)m_odImage.wmf.begin();
      seekpos = ALDUSMFHEADERSIZE;
    }
    BYTE *p = (BYTE *)m_odImage.wmf.begin();
    mfHeader = (METAHEADER*)( p + seekpos);

    if ((mfHeader->mtType != 1) && (mfHeader->mtType != 2))
        return;

    dwSize = mfHeader->mtSize * 2;
    // Create the enhanced metafile
    HENHMETAFILE m_emf;
    m_emf = ::SetWinMetaFileBits(dwSize, (const BYTE*)mfHeader, NULL, NULL);

    CSize size(0,0);
    if ( aldusMFHeader )
    {
      size.cx = 254 * (aldusMFHeader->bbox.right - aldusMFHeader->bbox.left)
              / aldusMFHeader->inch;
      size.cy = 254 * (aldusMFHeader->bbox.bottom - aldusMFHeader->bbox.top)
              / aldusMFHeader->inch;
    }

    cr = Calc(size.cx, size.cy, width, height );
    cr.OffsetRect(X,Y);
    newDC.PlayMetaFile(m_emf, &cr );

  }
}

BOOL COdaMfcAppDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
  OdString strFileName(lpszPathName);
  int extPos = strFileName.reverseFind('.');
  OdString strExt = strFileName.mid(extPos + 1);
  CMainFrame* pMFrame = (CMainFrame*)theApp.GetMainWnd();
  bool bError = false;
  try
  {
    pMFrame->StartTimer();

    if (theApp.getSaveWithPassword() &&
        m_SaveAsType == OdDb::kDwg &&
        m_SaveAsVer >= OdDb::vAC18)
    {
      OdSecurityParams tmp;
      m_pDb->securityParams(tmp);
      if (m_pDb->appServices()->getPassword(strFileName, false, tmp.password))
      {
        tmp.nFlags = tmp.nFlags | SECURITYPARAMS_ENCRYPT_DATA;
        tmp.nProvType = 0xd;
        // L"Microsoft Base DSS and Diffie-Hellman Cryptographic Provider"
        static OdCharW pn[] =
        {
          0x4D, 0x69, 0x63, 0x72, 0x6F,
          0x73, 0x6F, 0x66, 0x74, 0x20,
          0x42, 0x61, 0x73, 0x65, 0x20,
          0x44, 0x53, 0x53, 0x20, 0x61,
          0x6E, 0x64, 0x20, 0x44, 0x69,
          0x66, 0x66, 0x69, 0x65, 0x2D,
          0x48, 0x65, 0x6C, 0x6C, 0x6D,
          0x61, 0x6E, 0x20, 0x43, 0x72,
          0x79, 0x70, 0x74, 0x6F, 0x67,
          0x72, 0x61, 0x70, 0x68, 0x69,
          0x63, 0x20, 0x50, 0x72, 0x6F,
          0x76, 0x69, 0x64, 0x65, 0x72,
          0x00
        };
        tmp.provName = pn;

        m_pDb->setSecurityParams(tmp);
      }
    }
    UNTRACE_DB_EVENTS(m_pDb);
    m_pDb->appServices()->layoutManager()->removeReactor(this);
    if (strExt == L"dsf")
    {
      if (m_pDb->getFilename() == strFileName)
      {
        TCHAR buf[1024];
        DWORD len = 1023;
        GetUserName(buf, &len);
        CCommitMessageDialog dlg;
        dlg.m_Author = buf;
        dlg.DoModal();
        OdTfRevisionControl::commit(m_pDb, OdTfCommitAnnotation(dlg.m_Author.GetString(), (LPCTSTR)dlg.m_Message));
      }
      else
      {
        OdTfRepositoryPtr pRep = OdTfRepository::createObject(odrxSystemServices()->createFile(strFileName, Oda::FileAccessMode(Oda::kFileWrite | Oda::kFileRead), Oda::kShareDenyWrite, Oda::kCreateAlways), m_pDb->appServices());
        pRep->import(m_pDb, OdString(L"master"));
      }
    }
    else
    {
      if (getTryIncSave())
        m_pDb->save(strFileName, theApp.getSavePreview());
      else
        m_pDb->writeFile(strFileName, m_SaveAsType, m_SaveAsVer, theApp.getSavePreview());
    }
    TRACE_DB_EVENTS(m_pDb);
    m_pDb->appServices()->layoutManager()->addReactor(this);
  }
  catch(const OdError& e)
  {
    theApp.reportError(_T("Error Saving Document..."), e);
    bError = true;
  }
  catch(const UserBreak&)
  {
    theApp.reportError(_T("Saving Document"), OdError(eUserBreak));
    bError = true;
  }
#ifndef _DEBUG
  catch(...)
  {
    AfxMessageBox(_T("Error while writing DB."), MB_ICONERROR);
    bError = true;
  }
#endif // #ifndef _DEBUG
#if 0
#ifndef _DEBUG
  catch(...)
  {    AfxMessageBox(_T("Fatal Error..."), MB_ICONERROR);

    bError = true;
  }
#endif // #ifndef _DEBUG
#endif
  pMFrame->StopTimer(_T("Writing"));
  if(bError)
  {
    AfxThrowUserException( );
    return FALSE;
  }
  // the next step is optional
  //m_pDb->clearUndo();
  UpdateAllViews(NULL);
  return TRUE;
}
void COdaMfcAppDoc::OnViewConstrDisplay()
{
  m_pDb->setDYNCONSTRAINTDISPLAY(!m_pDb->getDYNCONSTRAINTDISPLAY());
  ExecuteCommand(L"REGEN");
}

void COdaMfcAppDoc::OnUpdateViewConstrDisplay(CCmdUI* pCmdUI) 
{
  pCmdUI->SetCheck(m_pDb->getDYNCONSTRAINTDISPLAY());
}

void odDbCreateLayoutThumnailImage(OdDbDatabase* pDb, OdBinaryData& imageData)
{
  OdGsDevicePtr pDevice = pDb->appServices()->gsBitmapDevice(NULL, pDb, OdDbBaseHostAppServices::kForThumbnail);
  if ( pDevice.get() )
  {
    OdGsDCRect gsRect(0, 256 , 256, 0);
    OdGiContextForDbDatabasePtr pCtx = OdGiContextForDbDatabase::createObject();
    pCtx->setDatabase(pDb);
    pDevice = OdDbGsManager::setupActiveLayoutViews(pDevice, pCtx);
    ODCOLORREF clrBackground(ODRGB(255,255,255));
    pCtx->setPaletteBackground(clrBackground);
    pDevice->setBackgroundColor(clrBackground);
    pDevice->setLogicalPalette(odcmAcadPalette(clrBackground), 256);
    pDevice->onSize(gsRect);

    // Ask for 8 bpp resolution (this is not RGB - 24 bpp)
    pDevice->properties()->putAt(OD_T("BitPerPixel"), OdRxVariantValue(OdUInt32(8)));

    // Update device - fill raster image
    pDevice->update();

    // Get rendering results
    OdGiRasterImagePtr pRaster = pDevice->properties()->getAt(OD_T("RasterImage"));
    OdRxRasterServicesPtr pRasSvcs = odrxDynamicLinker()->loadApp(RX_RASTER_SERVICES_APPNAME);
    if ( !pRasSvcs.get() || !pRasSvcs->isRasterImageTypeSupported(OdRxRasterServices::kBMP))
    {
      return;
    }

    OdStreamBufPtr pPng = OdMemoryStream::createNew();
    if ( pRasSvcs->convertRasterImage(pRaster,OdRxRasterServices::kPNG, pPng) )
    {
      imageData.resize(pPng->length());
      pPng->seek(0, OdDb::kSeekFromStart);
      pPng->getBytes(imageData.asArrayPtr(), pPng->length());
    }
  }
}

void COdaMfcAppDoc::OnViewSetactivelayout()
{
  CSetActiveLayoutDlg dlg(m_pDb, theApp.GetMainWnd());
  m_bLayoutSwitchable = true;
  if(dlg.DoModal()==IDOK)
  {
    try
    {
      m_pDb->startUndoRecord();
      m_pDb->setCurrentLayout(OdString(dlg.m_sNewLayoutName));
      ::oddbEvaluateFields(m_pDb, OdDbField::kRegen);

      OdDbObjectId id = m_pDb->findLayoutNamed(OdString(dlg.m_sNewLayoutName));
      if (!id.isNull())
      {
        OdDbLayoutPtr layout = id.safeOpenObject(OdDb::kForWrite);
        if ( !layout.isNull() )
        {
          OdBinaryData imageData;
          ::odDbCreateLayoutThumnailImage(m_pDb, imageData);
          layout->setThumbnailBitmap(imageData.asArrayPtr(), imageData.size());
        }
      }
    }
    catch(OdError& e)
    {
      theApp.reportError(_T("Error Setting Layout..."), e);
      m_pDb->disableUndoRecording(true);
      m_pDb->undo();
      m_pDb->disableUndoRecording(false);
    }
  }
  m_bLayoutSwitchable = false;
}

void COdaMfcAppDoc::layoutSwitched(const OdString&, const OdDbObjectId&)
{
  // AMark : Prevent Zoom/Rotate crashes
  // AMark : Prevent Undo/Redo crashes
  if (m_bLayoutSwitchable)
  // This test can be exchanged by remove/add reactor in layout manager, but this operations must be added
  // into all functions which can call setCurrentLayout (but where vectorization no need to be changed).
  {
    POSITION pos = GetFirstViewPosition();
    while (pos != NULL)
    {
      CView *view = GetNextView(pos);
      if (view->IsKindOf(RUNTIME_CLASS(CDwgViewer)) && !view->IsKindOf(RUNTIME_CLASS(CRemoteGeomViewer)))
      {
        if (view->GetDocument() == this)
        {
#if 0
          static_cast<CDwgViewer*>(view)->recreateDevice();
#else
          CWnd *pParent = view->GetParent();
          // Get prev params
          bool bIconic = pParent->IsIconic() != FALSE;
          bool bZoomed = pParent->IsZoomed() != FALSE;
          CRect wRect;
          pParent->GetWindowRect(&wRect);
          POINT point1, point2;
          point1.x = wRect.left; point1.y = wRect.top;
          point2.x = wRect.right; point2.y = wRect.bottom;
          pParent->GetParent()->ScreenToClient(&point1);
          pParent->GetParent()->ScreenToClient(&point2);
          wRect.left = point1.x; wRect.top = point1.y;
          wRect.right = point2.x; wRect.bottom = point2.y;
          //
          view->GetParent()->SendMessage(WM_CLOSE);
          OnVectorize();
          // Search again for new view
          POSITION pos = GetFirstViewPosition();
          while (pos != NULL)
          {
            CView *view = GetNextView(pos);
            if (view->IsKindOf(RUNTIME_CLASS(CDwgViewer)) && !view->IsKindOf(RUNTIME_CLASS(CRemoteGeomViewer)))
            {
              if (view->GetDocument() == this)
              {
                CWnd *pParent = view->GetParent();
                if (bZoomed)
                {
                  if (!pParent->IsZoomed())
                  {
                    reinterpret_cast<CMDIChildWnd*>(pParent)->MDIMaximize();
                  }
                }
                else
                {
                  reinterpret_cast<CMDIChildWnd*>(pParent)->MDIRestore();
                  if (!bIconic)
                    pParent->SetWindowPos(NULL, wRect.left, wRect.top, wRect.right - wRect.left, wRect.bottom - wRect.top, SWP_NOZORDER);
                }
                break;
              }
            }
          }
          //
#endif
        }
      }
    }
  }
}

const OdString Cmd_VIEW::groupName() const { return L"ODAMFCAPP"; }

const OdString Cmd_VIEW::name() { return L"VIEW"; }

const OdString Cmd_VIEW::globalName() const { return name(); }

void Cmd_VIEW::execute(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabaseDocPtr pDb = pDbCmdCtx->database();
  CNamedViewsDlg dlg(pDb->document(), theApp.GetMainWnd());
  if(dlg.DoModal()!=IDOK)
    throw OdEdCancel();
}

const OdString Cmd_SELECT::groupName() const { return L"ODAMFCAPP"; }

const OdString Cmd_SELECT::name() { return L"SELECT"; }

const OdString Cmd_SELECT::globalName() const { return name(); }

void Cmd_SELECT::execute(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabaseDocPtr pDb = pDbCmdCtx->database();
  COdaMfcAppDoc *pDoc = pDb->document();
  CDwgViewer *pView = pDoc->getViewer();
  if (pView == NULL)
  {
    throw OdEdCancel();
  }
  pDoc->OnEditClearselection();
  pDoc->UpdateAllViews(NULL);
  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();
  pIO->setPickfirst(0);
  int iOpt = OdEd::kSelLeaveHighlighted | OdEd::kSelAllowEmpty;

  OdDbSelectionSetPtr pSSet;
  try
  {
    pSSet = pIO->select(OdString::kEmpty, iOpt, pView->editorObject().workingSSet());
    pView->editorObject().setWorkingSSet(pSSet);
  }
  catch( const OdError& )
  {
    throw OdEdCancel();
  }
  pView->editorObject().selectionSetChanged();
  pDb->pageObjects();
}


const OdString Cmd_DISPLAY_DIFFS::groupName() const { return L"ODAMFCAPP"; }

const OdString Cmd_DISPLAY_DIFFS::name() { return L"DISPLAY_DIFFS"; }

const OdString Cmd_DISPLAY_DIFFS::globalName() const { return name(); }

void Cmd_DISPLAY_DIFFS::execute(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabaseDocPtr pDb = pDbCmdCtx->database();
  COdaMfcAppDoc *pDoc = pDb->document();
  CDwgViewer *pViewer = pDoc->getViewer();

  // Get handle of failed object
  OdValuePtr vHandle = pCmdCtx->arbitraryData(L"Handle");
  OdDbHandle hFailed = (OdUInt64)(OdInt64)(*vHandle);

  if (!hFailed.isNull())
  {
    // Clear the value in context
    pCmdCtx->setArbitraryData(L"Handle", NULL);
    // Set selection to faile object in original database
    POSITION pos = pDoc->GetFirstViewPosition();
    while (pos != NULL)
    {
      CView *view = pDoc->GetNextView(pos);
      LPCSTR className = view->GetRuntimeClass()->m_lpszClassName;
      if (CString(view->GetRuntimeClass()->m_lpszClassName).Compare(_T("CDwgView")) == 0)
      {
        CDwgView * pView = (CDwgView*)view;
        pView->Find(pDb->getOdDbObjectId(hFailed));
      }
    }
  }

  OdDbDatabasePtr pNewDb = pCmdCtx->arbitraryData(L"OdDbDatabasePtr");
  if (pNewDb.isNull())
  {
    pCmdCtx->userIO()->putString(L"No database passed");
    return;
  }
  // Open document with round-tripped database
  COdaMfcAppDoc *pNewDoc = (COdaMfcAppDoc*)theApp.OpenDocumentFile(L"Round-tripped");
  // Set selection to failed object in round-tripped database
  POSITION pos = pNewDoc->GetFirstViewPosition();
  while (pos != NULL)
  {
    CView *view = pNewDoc->GetNextView(pos);
    LPCSTR className = view->GetRuntimeClass()->m_lpszClassName;
    if (CString(view->GetRuntimeClass()->m_lpszClassName).Compare(_T("CDwgView")) == 0)
    {
      CDwgView * pView = (CDwgView*)view;
      pView->Find(pNewDoc->m_pDb->getOdDbObjectId(hFailed));
    }
  }
}

///////////////////////////////////////////////////////////////////////
const OdString Cmd_SAVE::groupName() const { return L"ODAMFCAPP"; }

const OdString Cmd_SAVE::name() { return L"SAVE"; }

const OdString Cmd_SAVE::globalName() const { return name(); }

void Cmd_SAVE::execute(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabaseDocPtr pDb = pDbCmdCtx->database();
  COdaMfcAppDoc *pDoc = pDb->document();

  OdInt32 numOfSavesSinceOpened = pDb->numberOfSaves();
  pDoc->m_SaveAsVer = (numOfSavesSinceOpened > 0) ? pDb->lastSavedAsVersion() : 
                                                    pDb->originalFileVersion();

  OdString fileName = pDb->getFilename();
  int extensionPos = fileName.reverseFind('.');
  OdString strExtension = fileName.mid(extensionPos + 1);

  if ( (!fileName.isEmpty()) && (strExtension.iCompare(L"dwg") == 0) )
  {
    if (pDoc->m_SaveAsVer == OdDb::kDHL_CURRENT)
    {
      pDoc->setTryIncSave(true);
    }
    else
    {
      pDoc->setTryIncSave(false);
    }
    pDoc->OnSaveDocument(fileName.c_str());
  }
  else
  {
    OdDbHostAppServices* hostAppSvcs = pDb->appServices();
    if ((hostAppSvcs)&&(hostAppSvcs->getFILEDIA()))
    {
      CString newName;
      CDocTemplate* pTemplate = pDoc->GetDocTemplate();
      ASSERT(pTemplate != NULL);
      if (!pDoc->DoPromptFileName(newName, AFX_IDS_SAVEFILE,
        OFN_HIDEREADONLY | OFN_EXPLORER | OFN_PATHMUSTEXIST, FALSE, pTemplate))
        return;

      pDoc->setTryIncSave(false);    
      if (pDoc->OnSaveDocument(newName))
        pDoc->SetPathName(newName);
    }
    else
    {
      OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();
      OdString strPath = pIO->getFilePath(L"Enter file name to save:", OdEd::kGfpForSave, L"Save As", L"dwg");

      int extPos = strPath.reverseFind('.');
      OdString strExt = strPath.mid(extPos + 1);
      if (strExt.iCompare(L"dxf") == 0) pDoc->m_SaveAsType = OdDb::kDxf;
      if (strExt.iCompare(L"dwg") == 0) pDoc->m_SaveAsType = OdDb::kDwg;

      pDoc->m_SaveAsVer = OdDb::kDHL_CURRENT;

      pDoc->setTryIncSave(false);    
      if (pDoc->OnSaveDocument(strPath.c_str()))
        pDoc->SetPathName(strPath.c_str());
    }
  }  
}
///////////////////////////////////////////////////////////////////////
const OdString Cmd_SAVEAS::groupName() const { return L"ODAMFCAPP"; }

const OdString Cmd_SAVEAS::name() { return L"SAVEAS"; }

const OdString Cmd_SAVEAS::globalName() const { return name(); }

void Cmd_SAVEAS::execute(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabaseDocPtr pDb = pDbCmdCtx->database();
  COdaMfcAppDoc *pDoc = pDb->document();

  OdDbHostAppServices* hostAppSvcs = pDb->appServices();
  if ((hostAppSvcs)&&(hostAppSvcs->getFILEDIA()))
  {
    CString newName;
    CDocTemplate* pTemplate = pDoc->GetDocTemplate();
    ASSERT(pTemplate != NULL);
    if (!pDoc->DoPromptFileName(newName, AFX_IDS_SAVEFILE,
      OFN_HIDEREADONLY | OFN_EXPLORER | OFN_PATHMUSTEXIST, FALSE, pTemplate))
      return;

    pDoc->setTryIncSave(false);    
    if (pDoc->OnSaveDocument(newName))
      pDoc->SetPathName(newName);
  }
  else
  {
    OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();
    OdString strPath = pIO->getFilePath(L"Enter file name to save:", OdEd::kGfpForSave, L"Save As", L"dwg");

    int extPos = strPath.reverseFind('.');
    OdString strExt = strPath.mid(extPos + 1);
    if (strExt.iCompare(L"dxf") == 0) pDoc->m_SaveAsType = OdDb::kDxf;
    if (strExt.iCompare(L"dwg") == 0) pDoc->m_SaveAsType = OdDb::kDwg;

    pDoc->m_SaveAsVer = OdDb::kDHL_CURRENT;
    
    pDoc->setTryIncSave(false);    
    if (pDoc->OnSaveDocument(strPath.c_str()))
      pDoc->SetPathName(strPath.c_str());
  }  
}
/////////////////////////////////////////////////////////////////////////
void COdaMfcAppDoc::OnViewNamedViews()
{
  ExecuteCommand(L"VIEW");
}

void COdaMfcAppDoc::OnEditUndo() 
{
  m_bLayoutSwitchable = true;
  m_pDb->undo();
  m_bLayoutSwitchable = false;
  UpdateAllViews(NULL);
}

void COdaMfcAppDoc::OnUpdateEditUndo(CCmdUI* pCmdUI) 
{
  theApp.RefreshCommandMenu();
  pCmdUI->Enable(m_pDb->hasUndo() ? TRUE : FALSE);
}

void COdaMfcAppDoc::OnEditRedo() 
{
  m_bLayoutSwitchable = true;
  m_pDb->redo();
  m_bLayoutSwitchable = false;
  UpdateAllViews(NULL);
}

void COdaMfcAppDoc::OnUpdateEditRedo(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable(m_pDb->hasRedo() ? TRUE : FALSE);
}

void COdaMfcAppDoc::OnEditZoomextents() 
{
  OdDbObjectId id = m_pDb->activeViewportId();
  if (!id.isNull())
  {
    m_pDb->startUndoRecord();
    OdDbObjectPtr pVp = id.safeOpenObject(OdDb::kForWrite);
    OdAbstractViewPEPtr pAVP(pVp);
    pAVP->zoomExtents(pVp);
  }
}

void COdaMfcAppDoc::OnEditIdeditlinetype() 
{
  // TODO: Add your command handler code here
  CLoadLineTypes dialog(m_pDb);
  dialog.DoModal();
  if (dialog.isSomethingDone())
  {
    UpdateAllViews(NULL);    
  }
}

// MKU 18.12.2002
//
//  In order to check DwfExport including

void COdaMfcAppDoc::OnUpdateExport(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable(
#ifdef ENABLE_DWF_EXPORT_SUPPORT
                  TRUE 
#else
                  FALSE
#endif
                  );
}

void COdaMfcAppDoc::OnUpdatePublish(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable(
#ifdef ENABLE_DWF_EXPORT_SUPPORT
                  TRUE 
#else
                  FALSE
#endif
                  );
}

void COdaMfcAppDoc::OnUpdate3DPublish(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable(
#if defined ENABLE_3DDWF_EXPORT_SUPPORT
                  TRUE 
#else
                  FALSE
#endif
                  );
}

//#ifdef ENABLE_DWF_EXPORT_SUPPORT
//
//typedef void (*ExportDwfFn)(const DwExportParams& pParams);
//
//void ExportDwfWrapper(const DwExportParams& params)
//{
//#ifdef _TOOLKIT_IN_DLL_
//
//    HINSTANCE hinstLib = LoadLibrary(OdString(OdDwf7ExportModuleName) + OdString(TD_DLL_VERSION_SUFFIX_STR) + OdString(_T(".dll")));
//
//    ExportDwfFn exportFn = 0;
//    if (hinstLib != NULL) 
//    { 
//#ifdef _WIN64
//      exportFn = (ExportDwfFn)GetProcAddress(hinstLib, "?exportDwf@@YAXAEBVDwExportParams@TD_DWF_EXPORT@@@Z");
//#else
//      exportFn = (ExportDwfFn)GetProcAddress(hinstLib, "?exportDwf@@YAXABVDwExportParams@TD_DWF_EXPORT@@@Z");
//#endif
//    }
//    if (exportFn != NULL) 
//    { 
//      exportFn(params);
//      FreeLibrary(hinstLib);
//    }
//    else
//    {
//      AfxMessageBox(_T("The TD_Dwf7Export.dll missed"));
//    }
//
//#else // #ifdef _TOOLKIT_IN_DLL_
//
//    exportDwf(params);
//
//#endif // #ifdef _TOOLKIT_IN_DLL_
//}
//
//#endif // ENABLE_DWF_EXPORT_SUPPORT

void COdaMfcAppDoc::OnUpdatePdfExport(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable(TRUE);
}

void COdaMfcAppDoc::OnUpdateColladaExport(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable(
#ifdef ENABLE_COLLADA_EXPORT_SUPPORT
    TRUE 
#else
    FALSE
#endif
    );
}

void COdaMfcAppDoc::OnUpdateHoopsExport(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable(
#ifdef ENABLE_HOOPS_EXPORT
    TRUE 
#else
    FALSE
#endif
    );
}

void COdaMfcAppDoc::OnExportCollada()
{
#ifdef ENABLE_COLLADA_EXPORT_SUPPORT

  ColladaModulePtr pModule = odrxDynamicLinker()->loadModule(OdColladaExportModuleName);
  if (pModule.isNull())
  {
    AfxMessageBox(OdColladaExportModuleName L"." DEFAULT_MODULE_EXTENSION_W L" is missing");
  }
  else
  {
    OdString sInput = "";
    try
    {
      sInput = m_pDb->appServices()->fileDialog(true, L"Collada file to save", L"dae", L"", "Collada file *.dae|*.dae||");

      if (sInput != OD_T("*unsupported*"))
      {
        CMainFrame* pMFrame = (CMainFrame*)theApp.GetMainWnd();
        OdDbCommandContextPtr pCmdCtx = cmdCtx();
        OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
        OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();
        bool useDoublePrecision = OdRxVariantValue(pIO->getKeyword(OD_T("Use double precision [Yes/No] <No>:"), OD_T("Yes No"), 1) == 0);
        pMFrame->StartTimer();
        pModule->exportCollada(m_pDb, sInput, odcmAcadLightPalette(), 256, OdGeMatrix3d::kIdentity, NULL, useDoublePrecision);
        pMFrame->StopTimer(_T("Collada export"));
      }
    }
    catch (...)
    {
    }
  }
#endif
}

void COdaMfcAppDoc::OnExportHoops()
{
#ifdef ENABLE_HOOPS_EXPORT
  OdString sInput = "";
  try
  {
    sInput = m_pDb->appServices()->fileDialog(true, L"Hoops file to save", L"hmf", L"", "Hoops Metafile *.hmf|*.hmf|Hoops Stream File *.hsf|*.hsf||");

    if( sInput != OD_T("*unsupported*") )
    {
      CMainFrame* pMFrame = (CMainFrame*)theApp.GetMainWnd();
      pMFrame->StartTimer();
      HoopsExportParams hoopsParams;
      hoopsParams.strFileName = sInput;
      hoopsParams.pDb = m_pDb;
      OdUInt32 errCode = exportHoops( hoopsParams );
      pMFrame->StopTimer(_T("Hoops export"));
    }
  }
  catch(...)
  {
  }
#endif
}

void COdaMfcAppDoc::OnExportPdf()
{
  OdPdfExportModulePtr pModule = ::odrxDynamicLinker()->loadApp(OdPdfExportModuleName);
  if (pModule.isNull())
  {
    AfxMessageBox(OdPdfExportModuleName L"." DEFAULT_MODULE_EXTENSION_W L" is missing");
  }
  else
  {
    COdaMfcAppApp* pApp = (COdaMfcAppApp*)AfxGetApp();
    pApp->setSupportFileSelectionViaDialog(false);
    CExport2PDFDialog dlgPdf(oddbDatabaseCollection());

    dlgPdf.m_dPaperH = OdGsPageParams().getPaperHeight();
    dlgPdf.m_dPaperW = OdGsPageParams().getPaperWidth();
    dlgPdf.m_pDb = m_pDb;
    dlgPdf.m_pHost = &theApp;

    dlgPdf.DoModal();
    pApp->setSupportFileSelectionViaDialog(true);
  }
}

void COdaMfcAppDoc::OnExportIfc()
{
#ifdef DWG2IFC_SUPPORT
  OdDwg2IfcModulePtr pModule = odrxDynamicLinker()->loadApp(OdDwg2IfcModuleName);
  if (pModule.isNull())
  {
    AfxMessageBox(OdDwg2IfcModuleName L"." DEFAULT_MODULE_EXTENSION_W L" is missing");
  }
  else
  {
    OdIfcImportPtr pImporter = pModule->create();
    OdString path("D:\\Test\\dwg2ifc.ifc");

    pImporter->properties()->putAt(L"Database", m_pDb);
    pImporter->properties()->putAt(L"SchemaVersion", OdRxVariantValue(OdInt16(kIfc2x3)));
    pImporter->properties()->putAt(L"IfcFilePath", OdRxVariantValue(path));

    if (pImporter->import() != OdIfcImport::success)
      AfxMessageBox(L"Export failed");
  }
#endif // DWG2IFC_SUPPORT
}

#define DWF_DEVICE_NAME L"DWF6 ePlot.pc3"

void COdaMfcAppDoc::OnExport()
{
//  OdDbLayoutPtr layout = OdDbBlockTableRecordPtr(getPaperSpaceId().safeOpenObject())->getLayoutId().safeOpenObject(OdDb::kForWrite);
//  OdDbPlotSettingsValidatorPtr pv = appServices()->plotSettingsValidator();
//  pv->setCanonicalMediaName( layout, "ANSI full bleed B (11.00 x 17.00 Inches)" );
  
  OdDwfExportModulePtr pModule = ::odrxDynamicLinker()->loadApp(OdDwf7ExportModuleName);
  if (pModule.isNull())
  {
    AfxMessageBox(OdDwf7ExportModuleName L"." DEFAULT_MODULE_EXTENSION_W L" is missing");
  }
  else
  {
    OdDwfExportPtr exporter = pModule->create();

    DWORD lFlags = OFN_HIDEREADONLY | OFN_EXPLORER | OFN_PATHMUSTEXIST;

    CFileDialog dlgFile(false);

    dlgFile.m_ofn.Flags |= lFlags;

    CString strFilter;

    strFilter = _T("Binary DWF (Desgin Web Format) v6.0 (*.dwf)");
    strFilter += (TCHAR)'\0';   // next string please
    strFilter += _T("*.dwf");
    strFilter += (TCHAR)'\0';   // last string

    strFilter += _T("Zipped Ascii Encoded 2D Stream DWF (Design Web Format) v6.0 (*.dwf)");
    strFilter += (TCHAR)'\0';   // next string please
    strFilter += _T("*.dwf");
    strFilter += (TCHAR)'\0';   // last string

    strFilter += _T("Compressed DWF (Design Web Format) v5.5 (*.dwf)");
    strFilter += (TCHAR)'\0';   // next string please
    strFilter += _T("*.dwf");
    strFilter += (TCHAR)'\0';   // last string

    strFilter += _T("Binary DWF (Design Web Format) v5.5 (*.dwf)");
    strFilter += (TCHAR)'\0';   // next string please
    strFilter += _T("*.dwf");
    strFilter += (TCHAR)'\0';   // last string

    strFilter += _T("Ascii DWF (Design Web Format) v5.5 (*.dwf)");
    strFilter += (TCHAR)'\0';   // next string please
    strFilter += _T("*.dwf");
    strFilter += (TCHAR)'\0';   // last string

    strFilter += _T("Compressed DWF (Design Web Format) v4.2 (*.dwf)");
    strFilter += (TCHAR)'\0';   // next string please
    strFilter += _T("*.dwf");
    strFilter += (TCHAR)'\0';   // last string

    strFilter += _T("Binary DWF (Design Web Format) v4.2 (*.dwf)");
    strFilter += (TCHAR)'\0';   // next string please
    strFilter += _T("*.dwf");
    strFilter += (TCHAR)'\0';   // last string

    strFilter += _T("Ascii DWF (Design Web Format) v4.2 (*.dwf)");
    strFilter += (TCHAR)'\0';   // next string please
    strFilter += _T("*.dwf");
    strFilter += (TCHAR)'\0';   // last string

    strFilter += _T("XPS (*.dwfx)");
    strFilter += (TCHAR)'\0';
    strFilter += _T("*.dwfx");
    strFilter += (TCHAR)'\0';

    dlgFile.m_ofn.lpstrDefExt = _T("dwf");    // MKU 27.06.2003   Only the first three characters are appended.
                                              //                  The string should not contain a period (.). 
    dlgFile.m_ofn.lpstrFilter = strFilter;

    dlgFile.m_ofn.lpstrTitle = _T("Export To Dwf");

    if (dlgFile.DoModal() == IDOK)
    {
      CString strFileName = dlgFile.GetPathName();
      if (strFileName.Find('.') == -1)
      {
        strFileName += _T(".dwf");
      }

#ifdef ENABLE_DWF_EXPORT_SUPPORT
      DwExportParams params;
      params.setDatabase(m_pDb);
      params.setDwfFileName(OdString(strFileName));
      params.setBackground(theApp.activeBackground());
      //params.pPalette = theApp.curPalette();
      //params.sSourceFileName = m_strPathName;
      params.setInkedArea(false);                                                        // MKU 1/21/2004
      params.setColorMapOptimize(false);                                                 // MKU 1/21/2004
      params.setExportInvisibleText(true);
      params.setLineweightScale(0.5);
      // CORE-14030
      bool bForceModelSpaceToExtents = theApp.dwfExportModelSpaceToExtents();
      if (bForceModelSpaceToExtents)
        params.setForceInitialViewToExtents(bForceModelSpaceToExtents); // true
      else
      {
        OdRxObjectPtr pRxLayout = OdRxObject::cast(m_pDb->currentLayoutId().openObject());
        OdDbBaseLayoutPEPtr pLayoutPe = OdDbBaseLayoutPE::cast(pRxLayout);
        if (pLayoutPe.get() && pLayoutPe->isModelLayout(pRxLayout))
        {
          OdDbPlotSettings* pPlotSettings = OdDbPlotSettings::cast(pRxLayout).get();
          ODA_ASSERT_ONCE_X(TDWF, pPlotSettings);

          OdResult res = eOk;
          OdString sDeviceName = pPlotSettings->getPlotCfgName(),
                   sMediaName = pPlotSettings->getCanonicalMediaName(),
                   sDwfDeviceName = DWF_DEVICE_NAME; // L"DWF6 ePlot.pc3"
          double paperWidth = 0.0, paperHeight = 0.0;
          pPlotSettings->getPlotPaperSize(paperWidth, paperHeight);
          OdDbPlotSettings::PlotPaperUnits units = pPlotSettings->plotPaperUnits();
          if (units == OdDbPlotSettings::kInches)
            paperWidth /= kMmPerInch, paperHeight /= kMmPerInch;
          //bool bPlotStyles = pPlotSettings->plotPlotStyles();
          //bool bShowPlotStyles = pPlotSettings->showPlotStyles();
          //OdString sStyleSheet = pPlotSettings->getCurrentStyleSheet();
          OdDbPlotSettingsValidator* pValidator = m_pDb->appServices()->plotSettingsValidator();
          ODA_ASSERT_ONCE_X(TDWF, pValidator);
          pPlotSettings->upgradeOpen();
          res = pValidator->setPlotCfgName(pPlotSettings, sDeviceName, sMediaName); // is valid
          if (res != eOk)
          {
            res = pValidator->setPlotCfgName(pPlotSettings, sDwfDeviceName, sMediaName);
            if (res != eOk)
            {
              res = pValidator->setPlotCfgName(pPlotSettings, sDwfDeviceName);
              if (res == eOk)
                res = pValidator->setClosestMediaName(pPlotSettings, paperWidth, paperHeight, units, true);
            }
            ODA_ASSERT_ONCE_X(TDWF, res == eOk);
            ODA_ASSERT_VAR(if (res == eOk))
            {
              ODA_ASSERT_ONCE_X(TDWF, sDwfDeviceName == pPlotSettings->getPlotCfgName());
              ODA_ASSERT_VAR(OdString sDwfMediaName = pPlotSettings->getCanonicalMediaName();)
              ODA_ASSERT_ONCE_X(TDWF, sDwfMediaName.find(L"full_bleed") >= 0);
              //ODA_ASSERT_ONCE_X(TDWF, bPlotStyles == pPlotSettings->plotPlotStyles());
              //ODA_ASSERT_ONCE_X(TDWF, bShowPlotStyles == pPlotSettings->showPlotStyles());
              //ODA_ASSERT_ONCE_X(TDWF, sStyleSheet == pPlotSettings->getCurrentStyleSheet());
            }
          }
        }
      }

      DwfPageData pageData;

      switch(dlgFile.m_ofn.nFilterIndex)
      {
        case 1:
          params.setFormat(DW_UNCOMPRESSED_BINARY);
          params.setVersion(nDwf_v60);
          pageData.sLayout = m_pDb->findActiveLayout(true);
          params.pageData().append(pageData);
          break;
        case 2:
          params.setFormat(DW_ASCII);
          params.setVersion(nDwf_v60);
          pageData.sLayout = m_pDb->findActiveLayout(true);
          params.pageData().append(pageData);
          break;
        case 3:
          params.setFormat(DW_COMPRESSED_BINARY);
          params.setVersion(nDwf_v55);
          break;
        case 4:
          params.setFormat(DW_UNCOMPRESSED_BINARY);
          params.setVersion(nDwf_v55);
          break;
        case 5:
          params.setFormat(DW_ASCII);
          params.setVersion(nDwf_v55);
          break;
        case 6:
          params.setFormat(DW_COMPRESSED_BINARY);
        params.setVersion(nDwf_v42);
        break;
        case 7:
          params.setFormat(DW_UNCOMPRESSED_BINARY);
          params.setVersion(nDwf_v42);
          break;
        case 8:
          params.setFormat(DW_ASCII);
          params.setVersion(nDwf_v42);
          break;
        case 9:
          params.setFormat(DW_XPS);
          params.setVersion(nDwf_v6_unknown);
          pageData.sLayout = m_pDb->findActiveLayout(true);
          params.pageData().append(pageData);
          break;
        default:
          ASSERT(0);
          return;
      }
      exporter->exportDwf(params);

#endif // ENABLE_DWF_EXPORT_SUPPORT
    }
  }
}

// MKU 25.06.2003
//
// Publishes drawings to DWF files starting with v6
//
void COdaMfcAppDoc::OnPublish()
{
#ifdef ENABLE_DWF_EXPORT_SUPPORT
  OdDwfExportModulePtr pModule = ::odrxDynamicLinker()->loadApp(OdDwf7ExportModuleName);
  if (pModule.isNull())
  {
    AfxMessageBox(OdDwf7ExportModuleName L"." DEFAULT_MODULE_EXTENSION_W L" is missing");
  }
  else
  {
    OdDwfExportPtr exporter = pModule->create();

    DwExportParams params;
    params.setDatabase(m_pDb);
    params.setBackground(theApp.activeBackground());
    //params.pPalette = theApp.curPalette();
    //params.sSourceFileName = m_strPathName;
    params.setInkedArea(false);                                                        // MKU 1/21/2004
    params.setColorMapOptimize(false);                                                 // MKU 1/21/2004

    CPublishSheets  dlgPublishSheets(&params);
    if (dlgPublishSheets.DoModal() == IDOK)
    {
      exporter->exportDwf(params);
    }
  }
#endif // ENABLE_DWF_EXPORT_SUPPORT
}

// MKU 09.08.05
//
// Publishes drawings to 3D DWF files (DWF V06.01) using DWF Toolkit 7
//
//#if defined ENABLE_3DDWF_EXPORT_SUPPORT
//
//typedef void (*Export3dDwfFn)(const Dwf3dExportParams& pParams);
//
//void Export3dDwfWrapper(const Dwf3dExportParams& params)
//{
//#ifdef _TOOLKIT_IN_DLL_
//
//    HINSTANCE hinstLib = LoadLibrary(OdString(OdDwf7ExportModuleName) + OdString(TD_DLL_VERSION_SUFFIX_STR) + OdString(_T(".dll")));
//    Export3dDwfFn exportFn = 0;
//    if (hinstLib != NULL) 
//    { 
//#ifdef _WIN64
//      exportFn = (Export3dDwfFn)GetProcAddress(hinstLib, "?export3dDwf@@YAXAEBVDwf3dExportParams@TD_DWF_EXPORT@@@Z");
//#else
//      exportFn = (Export3dDwfFn)GetProcAddress(hinstLib, "?export3dDwf@@YAXABVDwf3dExportParams@TD_DWF_EXPORT@@@Z");
//#endif
//    }
//    if (exportFn != NULL) 
//    { 
//      exportFn(params);
//      FreeLibrary(hinstLib);
//    }
//    else
//    {
//      AfxMessageBox(_T("The TD_Dwf7Export.dll missed"));
//    }
//
//#else // #ifdef _TOOLKIT_IN_DLL_
//
//    export3dDwf(params);
//
//#endif // #ifdef _TOOLKIT_IN_DLL_
//}
//
//#endif // #if defined ENABLE_3DDWF_EXPORT_SUPPORT


void COdaMfcAppDoc::On3DPublish()
{
#if defined ENABLE_3DDWF_EXPORT_SUPPORT
  OdDwfExportModulePtr pModule = ::odrxDynamicLinker()->loadApp(OdDwf7ExportModuleName);
  if (pModule.isNull())
  {
    AfxMessageBox(OdDwf7ExportModuleName L"." DEFAULT_MODULE_EXTENSION_W L" is missing");
  }
  else
  {
    OdDwfExportPtr exporter = pModule->create();

    Dwf3dExportParams params;
    params.setDatabase(m_pDb);
    params.setBackground(theApp.activeBackground());

    //params.pPalette = theApp.curPalette();
  
    DWORD lFlags = OFN_HIDEREADONLY | OFN_EXPLORER | OFN_PATHMUSTEXIST;

    CFileDialog dlgFile(false);

    dlgFile.m_ofn.Flags |= lFlags;

    CString strFilter;

    strFilter = _T("3D DWF (Desgin Web Format) v6.01 (*.dwf)");
    strFilter += (TCHAR)'\0';
    strFilter += _T("*.dwf");
    strFilter += (TCHAR)'\0';
    strFilter += _T("3D DWFX (XPS) (*.dwfx)");
    strFilter += (TCHAR)'\0';
    strFilter += _T("*.dwfx");
    strFilter += (TCHAR)'\0';

    dlgFile.m_ofn.lpstrDefExt = _T("dwf");    // MKU 27.06.2003   Only the first three characters are appended.
                                            //                  The string should not contain a period (.). 
    dlgFile.m_ofn.lpstrFilter = strFilter;

    dlgFile.m_ofn.lpstrTitle = _T("3D DWF Publish");

    if (dlgFile.DoModal() == IDOK)
    {
      CString strFileName = dlgFile.GetPathName();
      if (strFileName.Find('.') == -1)
      {
        strFileName += _T(".dwf");
      }

      //params.sDwfFileName = _T("d:\\sample\\w3d\\test3DPublish.dwf");
      params.setDwfFileName(OdString(strFileName));

      exporter->export3dDwf(params);
    }
  }
#endif // #if defined ENABLE_3DDWF_EXPORT_SUPPORT
}

void COdaMfcAppDoc::OnBmpExport()
{
  OdRxModulePtr pModule = ::odrxDynamicLinker()->loadModule(OdRasterExportModuleName);
  if (pModule.isNull())
    AfxMessageBox(OdRasterExportModuleName L"." DEFAULT_MODULE_EXTENSION_W L" is missing");
  else
    ExecuteCommand(OD_T("bmpout\n"));
}

void COdaMfcAppDoc::OnUpdateBmpExport(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable(TRUE);
}

void COdaMfcAppDoc::OnUpdateIfcExport(CCmdUI* pCmdUI)
{
#ifdef DWG2IFC_SUPPORT
  pCmdUI->Enable(TRUE);
#else
  pCmdUI->Enable(FALSE);
#endif // DWG2IFC_SUPPORT
}

#pragma warning(push)
#pragma warning(disable:4510)
#pragma warning(disable:4610)

struct CDocTemplateEx : CDocTemplate {
  void SetViewToCreate(CRuntimeClass* pViewClass){m_pViewClass = pViewClass;}
};

void COdaMfcAppDoc::OnVectorize(const OdString& sVectorizerPath) 
{
  theApp.setRecentGsDevicePath(sVectorizerPath);

  CDocTemplateEx* pTemplate = (CDocTemplateEx*)GetDocTemplate();
  ASSERT_VALID(pTemplate);

  pTemplate->SetViewToCreate(RUNTIME_CLASS(CDwgViewer));
  CFrameWnd* pFrame = pTemplate->CreateNewFrame(this, NULL);
  pTemplate->SetViewToCreate(RUNTIME_CLASS(CDwgView));

  pTemplate->InitialUpdateFrame(pFrame, this);

  m_pViewer = static_cast<CDwgViewer*>(pFrame->GetActiveView());
}

void COdaMfcAppDoc::OnRemoteGeomViewer() 
{
  theApp.setRemoteGeomViewer();

  CDocTemplateEx* pTemplate = (CDocTemplateEx*)GetDocTemplate();
  ASSERT_VALID(pTemplate);

  pTemplate->SetViewToCreate(RUNTIME_CLASS(CRemoteGeomViewer));
  CFrameWnd* pFrame = pTemplate->CreateNewFrame(this, NULL);
  pTemplate->SetViewToCreate(RUNTIME_CLASS(CDwgView));

  pTemplate->InitialUpdateFrame(pFrame, this);

  m_pViewer = static_cast<CDwgViewer*>(pFrame->GetActiveView());
}

#pragma warning(pop)

void COdaMfcAppDoc::OnCloseVectorizer(CDwgViewer* pView)
{
  ODA_ASSERT(pView==m_pViewer);
  m_pViewer = 0;
}

void COdaMfcAppDoc::setVectorizer(CDwgViewer* pView)
{
  ODA_ASSERT(m_pViewer==0);
  m_pViewer = pView;
}

void COdaMfcAppDoc::OnVectorize() 
{
  OnVectorize(theApp.recentGsDevicePath());
  // Populate secondary compatible device
  //OnVectorize(theApp.recentGsDevicePath());
}

void COdaMfcAppDoc::OnUpdateVectorize(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable(m_pViewer==0 && !theApp.recentGsDevicePath().isEmpty());
}

void COdaMfcAppDoc::OnViewSumminfo()
{
  CSummDlg sd(m_pDb, theApp.GetMainWnd());
  sd.DoModal();
}

void COdaMfcAppDoc::OnFormatLayer()
{
  OdaLayerPropManagerDlg dlgLayerPropManeger(m_pDb);
  if (IDOK == dlgLayerPropManeger.DoModal())
  {
    UpdateAllViews(0);
  }
}

void COdaMfcAppDoc::OnFormatLinetype()
{
   OdaLayerPropLineTypeDlg dlg(m_pDb, m_pDb->database()->getCELTYPE());
   if (IDOK  == dlg.DoModal())
   {
       m_pDb->database()->setCELTYPE(dlg.m_originalId);
   }
}

void COdaMfcAppDoc::OnFormatMlstyle()
{
   OdaMultiLineStyleDlg dlg(m_pDb);
   if (IDOK  == dlg.DoModal())
   {
       UpdateAllViews(NULL);
   }
}

void COdaMfcAppDoc::OnFormatBackgroundColor()
{
#ifndef OD_GIGRADIENTGENERATOR_LINKED
  CColorDialog dlg;
  dlg.m_cc.Flags |= CC_FULLOPEN | CC_RGBINIT;
  dlg.m_cc.rgbResult =theApp.activeBackground();

  if (IDOK == dlg.DoModal())
  {
    COLORREF color = dlg.GetColor();
    theApp.setActiveBackground(color);
    theApp.WriteProfileInt(_T("format"), _T("Background colour"), color);
  }
#else
  CColorPickerDlg dlg;
  dlg.SetColor((ODCOLORREF)theApp.activeBackground());

  if (dlg.DoModal() == IDOK)
  {
    COLORREF color = (COLORREF)dlg.GetColor();
    theApp.setActiveBackground(color);
    theApp.WriteProfileInt(_T("format"), _T("Background colour"), color);
  }
#endif
}

void COdaMfcAppDoc::OnFormatLineweightsettings() 
{
  CLwSettingsDlg lwsdlg(theApp.GetMainWnd());
  lwsdlg.m_bDisplayLw = m_pDb->getLWDISPLAY();
  lwsdlg.m_currentLw = m_pDb->getCELWEIGHT();
  OdDbHostAppServices* pAppSvcs = &theApp;
  lwsdlg.m_defaultLw = pAppSvcs->getLWDEFAULT();
  double step = 0.9 / 18.0;
  lwsdlg.m_lwScaleIndex = int((pAppSvcs->getLWDISPSCALE() - 0.1) / step);
  if(lwsdlg.DoModal()==IDOK)
  {
    m_pDb->setLWDISPLAY(lwsdlg.m_bDisplayLw!=0);
    m_pDb->setCELWEIGHT(lwsdlg.m_currentLw);
    pAppSvcs->setLWDEFAULT(lwsdlg.m_defaultLw);
    pAppSvcs->setLWDISPSCALE( double(lwsdlg.m_lwScaleIndex) * step + 0.1 );

    UpdateAllViews(NULL);
  }
}

void COdaMfcAppDoc::OnInsertDwfUnderlay()
{
  CDwfUnderlayDialog dlg(m_pDb, theApp.GetMainWnd(), CDwfUnderlayDialog::eDwf);
  if(dlg.DoModal()==IDOK)
  {
  }
}

// AMark 06.12.2007
void COdaMfcAppDoc::OnInsertDgnUnderlay()
{
  CDwfUnderlayDialog dlg(m_pDb, theApp.GetMainWnd(), CDwfUnderlayDialog::eDgn);
  if(dlg.DoModal()==IDOK)
  {
  }
}

void COdaMfcAppDoc::OnInsertPdfUnderlay()
{
  CDwfUnderlayDialog dlg(m_pDb, theApp.GetMainWnd(), CDwfUnderlayDialog::ePdf);
  if(dlg.DoModal()==IDOK)
  {
  }
}

void COdaMfcAppDoc::OnInsertXrefmanager() 
{
  CXrefViewDlg dlg(m_pDb, theApp.GetMainWnd());
  if(dlg.DoModal()==IDOK)
  {
    OdResult res = eOk;
    m_pDb->startUndoRecord();
    try
    {
      OdDbXRefMan::unload(dlg.m_xrefsToUnload);

      res = OdDbXRefMan::load(dlg.m_xrefsToReload);
      if (res == eOk)
      {
        for (unsigned i = 0; i < dlg.m_xrefsToDetach.size(); ++i)
        {
          OdDbBlockTableRecordPtr btr = dlg.m_xrefsToDetach[i].safeOpenObject(OdDb::kForWrite);
          res = OdDbXRefMan::detach(btr);
          if (res != eOk)
            break;
        }
      }
    }
    catch(const OdError& e)
    {
      ODA_FAIL(); // Should not get here
      m_pDb->undo();
      theApp.reportError(_T("Xref Manager:"), e);
    }

    if (res != eOk)
    {
      theApp.reportError(_T("Xref Manager:"), res);
    }
    UpdateAllViews(NULL);
  }
}

void COdaMfcAppDoc::OnImportDwf()
{
  OdRxModulePtr pModule = ::odrxDynamicLinker()->loadModule(OdDwf7ImportModuleName);

  if (pModule.isNull())
  {
    AfxMessageBox(OdDwf7ImportModuleName L" is missing");
  }
  else
  {
    // dwf import will add a lot of objects
    // tracing will slow down the process
    UNTRACE_DB_EVENTS(m_pDb);
    // DWFIN command is documented in ../../../Imports/Dwf7Import/Source/Dwf7ImportImpl.cpp, DwfImportCommand::execute()
    ExecuteCommand( L"dwfin ~\n\n297 210 No -1 Yes" );
    OdString s = m_pConsole->getLastString();
    ODCOLORREF c;
    odSScanf(s, L"Background = %x", &c);
    ((COdaMfcAppApp*)AfxGetApp())->setActiveBackground(c);
    TRACE_DB_EVENTS(m_pDb);
  }
}

void COdaMfcAppDoc::OnImportCollada()
{
  OdRxModulePtr pModule = ::odrxDynamicLinker()->loadModule(OdColladaImportModuleName);

  if (pModule.isNull())
  {
    AfxMessageBox(OdColladaImportModuleName L" is missing");
  }
  else
  {
    UNTRACE_DB_EVENTS(m_pDb);

    ExecuteCommand( L"ColladaIn Yes" );
    OdString s = m_pConsole->getLastString();
    ODCOLORREF c = ((COdaMfcAppApp*)AfxGetApp())->activeBackground();
    odSScanf(s, L"Background = %x", &c);
    ((COdaMfcAppApp*)AfxGetApp())->setActiveBackground(c);
    TRACE_DB_EVENTS(m_pDb);
  }
}

void COdaMfcAppDoc::OnExportSvg()
{
  OdGsModulePtr pModule = ::odrxDynamicLinker()->loadModule(OdSvgExportModuleName);
  if (pModule.isNull())
    AfxMessageBox( OdSvgExportModuleName L" is missing" );
  else
  {
    // svgout command parameters explanation see in \Exports\SvgExport\Source\SvgExport.cpp, SvgExportCommand::execute() 
    if (m_pDb->appServices()->getFILEDIA())
      ExecuteCommand(L"svgout 1 6 \n\n.png sans-serif 0 768 1024 Yes Yes Default\n");
    else
      ExecuteCommand(L"svgout ~\n1 6 \n\n.png sans-serif 0 768 1024 Yes Yes Default\n");
  }
}

OdDbCommandContextPtr COdaMfcAppDoc::cmdCtx()
{
  if(m_pCmdCtx.isNull())
  {
    m_pCmdCtx = ExDbCommandContext::createObject(cmdIO(), m_pDb);
    m_pCmdCtx->setArbitraryData( OD_T("OdaMfcApp InteractiveMode" ), OdRxVariantValue( true ) );
    m_pCmdCtx->setArbitraryData( OD_T("OdaMfcApp InteractiveFrameRate" ), OdRxVariantValue( (double)2.0 ) );
  }
  return m_pCmdCtx;
}

OdDbSelectionSetPtr COdaMfcAppDoc::selectionSet() const
{
  OdDbCommandContext* pCtx = const_cast<COdaMfcAppDoc*>(this)->cmdCtx();
  OdDbSelectionSetPtr pRes = pCtx->arbitraryData(L"OdaMfcApp Working Selection Set");
  if(pRes.isNull())
  {
    pRes = OdDbSelectionSet::createObject(m_pDb);
    pCtx->setArbitraryData(L"OdaMfcApp Working Selection Set", pRes);
  }
  ODA_TRACE1("Working Selection set contains %d items\n", pRes->numEntities());
  return pRes;
}

OdEdBaseIO* COdaMfcAppDoc::cmdIO()
{
  return this;
}

inline CUserIOConsole* COdaMfcAppDoc::console()
{
  if(m_pConsole.isNull())
    m_pConsole = CUserIOConsole::create(theApp.GetMainWnd());
  return m_pConsole;
}

OdString COdaMfcAppDoc::getString(const OdString& prompt, int options, OdEdStringTracker* pTracker)
{
  OdString sRes;
  if(m_pMacro.get() && !m_pMacro->isEof())
  {
    sRes = m_pMacro->getString(prompt, options, pTracker);
    putString(prompt + L" " + sRes);
    return sRes;
  }

  if(m_bConsole)
    return console()->getString(prompt, options, pTracker);

  if(m_pViewer)
  {
    m_bConsoleResponded = prompt.isEmpty();
    sRes = m_pViewer->getString(prompt, options, pTracker);
    if(!m_bConsoleResponded)
      putString(prompt + L" " + sRes);
    return sRes;
  }

  return console()->getString(prompt, options, pTracker);
}

void COdaMfcAppDoc::putString(const OdString& string)
{
  if(m_pViewer)
    m_pViewer->putString(string);

  console()->putString(string);
}

OdGePoint3d COdaMfcAppDoc::getPoint(const OdString& prompt, int options, OdEdPointTracker* pTracker)
{
  if(m_pMacro.get() && !m_pMacro->isEof())
  {
    OdString strRes = getString(prompt, options, 0);
    throw OdEdOtherInput(strRes);
  }

  if(m_bConsole)
    return m_pConsole->getPoint(prompt, options, pTracker);

  if(m_pViewer)
  {
    console()->putString(prompt);
    return m_pViewer->getPoint(prompt, options, pTracker);
  }

  return console()->getPoint(prompt, options, pTracker);
}
OdUInt32 COdaMfcAppDoc::getKeyState()
{
  OdUInt32 res = 0;
  SHORT state = ::GetKeyState(VK_CONTROL);
  if (state < 0)
    res |= MK_CONTROL;
  state = ::GetKeyState(VK_SHIFT);
  if (state < 0)
    res |= MK_SHIFT;
  return res;
}

OdString COdaMfcAppDoc::recentCmd()
{
  return theApp.getRecentCmd();
}

OdString COdaMfcAppDoc::recentCmdName()
{
  return theApp.getRecentCmd().spanExcluding(L" \n");
}

OdString COdaMfcAppDoc::commandPrompt()
{
  return _T("Command:");
}

void COdaMfcAppDoc::OnEditConsole() 
{
  OdEdCommandPtr pCommand;
  OdEdCommandStackPtr pCommands = ::odedRegCmds();
  OdDbCommandContextPtr pCmdCtx = cmdCtx();
  OdSaveState<bool> saveConsoleMode(m_bConsole, true);
  try
  {
    if(m_pViewer && m_pViewer->isGettingString())
    {
      m_pViewer->respond(console()->getString(m_pViewer->prompt(), m_pViewer->inpOptions(), 0));
      m_bConsoleResponded = true;
    }
    else
    {
      for(;;)
      {
        OdString s = pCmdCtx->userIO()->getString(commandPrompt(), 0, L"");
        if(s.isEmpty())
        {
          s = recentCmdName();
          if(!s.isEmpty())
          {
            pCmdCtx->userIO()->putString(s);
            ExecuteCommand(s, false);
          }
        }
        else
        {
          ExecuteCommand(s, false);
        }
      }
    }
  }
  catch(const OdEdCancel& )
  {
  }
}

OdString commandMessageCaption(const OdString& sCmd)
{
  OdString sCaption;
  sCaption.format(L"Command: %ls", sCmd.c_str());
  return sCaption;
}

class CmdReactor : public OdStaticRxObject<OdEdCommandStackReactor>,
                   public OdStaticRxObject<OdDbDatabaseReactor>
{
  ODRX_NO_HEAP_OPERATORS();
  OdDbCommandContext* m_pCmdCtx;
  bool                m_bModified;
  OdString            m_sLastInput;
  
  void setModified()
  {
    m_bModified = true;
    m_pCmdCtx->database()->removeReactor(this);
  }
public:
  CmdReactor(OdDbCommandContext* pCmdCtx) 
    : m_pCmdCtx(pCmdCtx)
    , m_bModified(false)
  {
    ODA_ASSERT(m_pCmdCtx);
    ::odedRegCmds()->addReactor(this);
    m_pCmdCtx->database()->addReactor(this);
  }
  ~CmdReactor()
  {
    ::odedRegCmds()->removeReactor(this);
    if(!m_bModified)
      m_pCmdCtx->database()->removeReactor(this);
  }
  void setLastInput(const OdString& sLastInput)
  {
    m_sLastInput = sLastInput;
  }
  const OdString &lastInput() const
  {
    return m_sLastInput;
  }

  bool isDatabaseModified() const { return m_bModified; }

  virtual void objectOpenedForModify(const OdDbDatabase*, const OdDbObject*) ODRX_OVERRIDE
  {
    setModified();
  }

  virtual void headerSysVarWillChange(const OdDbDatabase*, const OdString&) ODRX_OVERRIDE
  {
    setModified();
  }

  virtual OdEdCommandPtr unknownCommand(const OdString& sCmdName, OdEdCommandContext* /*pCmdCtx*/) ODRX_OVERRIDE
  {
    CDwgViewer* pViewer = OdDbDatabaseDocPtr(m_pCmdCtx->database())->document()->getViewer();
    if(pViewer)
    {
      OdEdCommandPtr pRes = pViewer->command(sCmdName);
      if(pRes.get())
        return pRes;
    }
    OdString sMsg;
    sMsg.format(L"Unknown command \"%ls\".", sCmdName.c_str());
    m_pCmdCtx->userIO()->putString(sMsg);
    return OdEdCommandPtr();
  }
  virtual void commandWillStart(OdEdCommand* pCmd, OdEdCommandContext* /*pCmdCtx*/) ODRX_OVERRIDE
  {
    m_sLastInput.makeUpper();
    if ( !GETBIT(pCmd->flags(), OdEdCommand::kNoHistory) )
      theApp.setRecentCmd(m_sLastInput);

    if ( !GETBIT(pCmd->flags(), OdEdCommand::kNoUndoMarker) )
      m_pCmdCtx->database()->startUndoRecord();
  }
  virtual void commandCancelled(OdEdCommand*, OdEdCommandContext* ) ODRX_OVERRIDE
  {
    undoCmd();
  }
  virtual void commandFailed(OdEdCommand*, OdEdCommandContext* ) ODRX_OVERRIDE
  {
    undoCmd();
  }
private:
  void undoCmd()
  {
    OdDbDatabase* pDb = m_pCmdCtx->database();
    try
    {
      pDb->disableUndoRecording(true);
      pDb->undo();
      pDb->disableUndoRecording(false);
    }
    catch(const OdError& err)
    {
      theApp.reportError(_T("Can't repair database"), err);
    }
#ifndef _DEBUG
    catch(...)
    {
      ::MessageBox(NULL, _T("Unknown error occurred..."),
        _T("Can't repair database"), MB_OK|MB_ICONERROR);
    }
#endif //_DEBUG
  }
};

void COdaMfcAppDoc::ExecuteCommand(const OdString& sCmd, bool bEcho) 
{
  OdSaveState<int> save_m_nCmdActive(m_nCmdActive);
  ++m_nCmdActive;

  OdDbCommandContextPtr pCmdCtx = cmdCtx();
  CmdReactor cr(pCmdCtx);

  pCmdCtx->setArbitraryData( OD_T("DynamicSubEntHlt"), OdRxVariantValue( theApp.enableDynamicSubEntHlt() ) );

  try
  {
    OdEdCommandStackPtr pCommands = ::odedRegCmds();
    ExDbCommandContext *pExCmdCtx = static_cast<ExDbCommandContext*>(pCmdCtx.get());
    if(m_pDb->appServices()->getPICKFIRST())
      pExCmdCtx->setPickfirst(selectionSet());

    if (sCmd[0]=='(')
    {
      OdEdLispModulePtr lspMod = odrxDynamicLinker()->loadApp(OdLspModuleName);
      if (!lspMod.isNull())
        lspMod->createLispEngine()->execute(pExCmdCtx, sCmd);
    }
    else
    {
      OdString s = sCmd.spanExcluding(L" \t\r\n");
      if(s.getLength() == sCmd.getLength())
      {
        if(bEcho)
          pCmdCtx->userIO()->putString(commandPrompt() + L" " + s);
        s.makeUpper();
        cr.setLastInput(s);
        pCommands->executeCommand(s, pCmdCtx);
      }
      else
      {
        m_pMacro = ExStringIO::create(sCmd);
        while(!m_pMacro->isEof())
        {
          try
          {
            s = pCmdCtx->userIO()->getString(commandPrompt());
            s.makeUpper();
            cr.setLastInput(s);
          }
          catch(const OdEdEmptyInput)
          {
            s = recentCmdName();
          }

          pCommands->executeCommand(s, pCmdCtx);
        }
      }
    }

    if (getViewer())
    {
      getViewer()->Invalidate();

      getViewer()->propagateActiveViewChanges();
    }
  }
  catch(const OdEdEmptyInput )
  {
  }
  catch(const OdEdCancel )
  {
  }
  catch(const OdError& err)
  {
    if(!m_bConsole)
    {
      theApp.reportError(commandMessageCaption(sCmd), err);
    }
    cmdIO()->putString(err.description());
  }
  if ((cr.isDatabaseModified() || selectionSet()->numEntities()) /*&& 0 != cr.lastInput().iCompare(_T("SELECT"))*/)
  {
    //selectionSet()->clear();
    // Call here OdExEditorObject::unselect() instead sset->clear(), if you want affect changes on grip points and etc.
    if ((0 != cr.lastInput().iCompare(_T("SELECT")) || pCmdCtx->database()->appServices()->getPICKADD() != 2) &&
        (0 != cr.lastInput().iCompare(_T("ExSelectionStyle"))))
      OnEditClearselection();
    UpdateAllViews(0);
  }
  //static_cast<ExDbCommandContext*>(pCmdCtx.get())->setMacroIOPresent(false);
}

BOOL COdaMfcAppDoc::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
  // If pHandlerInfo is NULL, then handle the message
  if (pHandlerInfo == NULL)
  {
    CMenu* pTopMenu = theApp.GetMainWnd()->GetMenu();
    if(pTopMenu)
    {
      // Check if it is a OdaMfcApp's dynamic menu item:
      MENUITEMINFO menuItemInfo;
      menuItemInfo.cbSize = sizeof(menuItemInfo);
      menuItemInfo.fMask = MIIM_DATA;
      if(pTopMenu->GetMenuItemInfo(nID, &menuItemInfo, FALSE)) // command from menu
      {
        if(menuItemInfo.dwItemData == theApp.getGSMenuItemMarker())
        {
          CString s;
          pTopMenu->GetSubMenu(4)->GetMenuString(nID, s, MF_BYCOMMAND);
          if (nCode == CN_COMMAND)
          {
            OnVectorize((LPCTSTR)s);
          }
          else if (nCode == CN_UPDATE_COMMAND_UI)
          {
            ((CCmdUI*)pExtra)->Enable(m_pViewer==0);
            ((CCmdUI*)pExtra)->SetCheck(s==(LPCTSTR)theApp.recentGsDevicePath());
          }
          return TRUE;
        }
        if (nID >= _APS_NEXT_COMMAND_VALUE + 100 && nID <= _APS_NEXT_COMMAND_VALUE + theApp.numCustomCommands() + 100) // custom commands
        {
          OdRxObjectPtr pData(reinterpret_cast<OdRxObject*>(menuItemInfo.dwItemData));
          if(pData.get()) // dynamic
          {
            if (nCode == CN_COMMAND)
            {
              OdEdCommandPtr pCmd = OdEdCommand::cast(pData);
              if(pCmd.get())
              {
                ExecuteCommand(pCmd->globalName());
                return TRUE;
              }
            }
            else if (nCode == CN_UPDATE_COMMAND_UI)
            {
              ((CCmdUI*)pExtra)->Enable(TRUE);
            }
            return TRUE;
          }
        }
        else if (nID >= _APS_NEXT_COMMAND_VALUE && nID < _APS_NEXT_COMMAND_VALUE + 100) // scales
        {
          if (nCode == CN_COMMAND)
          {
            int nScale = nID - _APS_NEXT_COMMAND_VALUE - 1;
            OdDbObjectContextCollectionIteratorPtr ci = m_pDb->objectContextManager()->contextCollection(ODDB_ANNOTATIONSCALES_COLLECTION)->newIterator();
            for (int i = 0; !ci->done(); ci->next())
            {
              if (i++ == nScale)
              {
                m_pDb->setCANNOSCALE(OdDbAnnotationScalePtr(ci->getContext()));
                menuItemInfo.fMask = MIIM_STATE;
                menuItemInfo.fState = MFS_CHECKED;
#if (WINVER >= 0x0500)
                pTopMenu->SetMenuItemInfo(nID, &menuItemInfo, FALSE);
#else
                ::InsertMenuItem(pTopMenu->GetSafeHmenu(), nID, FALSE, &menuItemInfo);
#endif
                ExecuteCommand(L"REGEN");
                UpdateAllViews(0);
              }
              else
              {
                menuItemInfo.fMask = MIIM_STATE;
                menuItemInfo.fState = MFS_UNCHECKED;
#if (WINVER >= 0x0500)
                pTopMenu->SetMenuItemInfo(_APS_NEXT_COMMAND_VALUE + i, &menuItemInfo, FALSE);
#else
                ::InsertMenuItem(pTopMenu->GetSafeHmenu(), _APS_NEXT_COMMAND_VALUE + i, FALSE, &menuItemInfo);
#endif
              }
            }
          }
          else if (nCode == CN_UPDATE_COMMAND_UI)
          {
            ((CCmdUI*)pExtra)->Enable(TRUE);
          }
          return TRUE;
        }
      }
    }
  }

  // If we didn't process the command, call the base class
  // version of OnCmdMsg so the message-map can handle the message
  return COleDocument::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

TOOLKIT_EXPORT OdRxClassPtr odrxGetModelerGeometryCreatorService();
void COdaMfcAppDoc::OnOptionsTriangulationParams() 
{
  OdRxModulePtr pModule;
  if (odrxDynamicLinker()->getModule(OdC3dModelerModuleName).get() || odrxDynamicLinker()->getModule(OdSpaModelerModuleName).get())
  {
    AfxMessageBox(OD_T("Properties supported for modelerGeometry only"), MB_OK);
    return;
  }
  pModule = odrxDynamicLinker()->loadModule(OdModelerGeometryModuleName);
  if (!pModule.get())
  {
    AfxMessageBox(OD_T("ModelerGeometry failed to be loaded"), MB_OK);
    return;
  }

  // TODO create common interface with virtual getTriangulationParams/setTriangulationParams
  ModelerModule* pModModule = static_cast<ModelerModule*>(pModule.get());

  wrTriangulationParams TriangulationParams;
  pModModule->getTriangulationParams(TriangulationParams);

  OdDbCommandContextPtr pCmdCtx = cmdCtx();
  OdDbDatabasePtr pDb = OdDbDatabase::cast(pCmdCtx->baseDatabase());
  double facetRes = pDb->getSysVar(L"FACETRES")->getDouble();

  OdaTrianParamsDlg triangulationParametersDlg(NULL, &TriangulationParams, &facetRes);

  LPARAM nResult = triangulationParametersDlg.DoModal();
  if (nResult == IDOK)
  {
    pModModule->setTriangulationParams(TriangulationParams);
    OdResBufPtr val = OdResBuf::createObject(); val->setRestype(OdResBuf::kRtDouble); val->setDouble(facetRes);
    pDb->setSysVar(L"FACETRES", val);
  }
}

/////////////////////////////////////////////////////////////////
// Page Setup Command

class OdPageSetupCmd : public OdEdCommand
{
public:

  const OdString groupName() const
  {
    return globalName();
  }

  const OdString globalName() const
  {
    return OD_T("PageSetup");
  }

  void execute(OdEdCommandContext* pCmdCtx)
  {
    OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
    OdDbDatabasePtr pDb = pDbCmdCtx->database();
    OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();

    pDb->startTransaction();

    OdDbObjectId idLayout = OdDbBlockTableRecordPtr(pDb->getActiveLayoutBTRId().safeOpenObject())->getLayoutId();
    OdDbLayoutPtr pLayout = idLayout.safeOpenObject(OdDb::kForWrite);

    OdDbPlotSettings *pPlotSettings = pLayout.get();
    CPageSetupDlg dlgPageSetup(*pPlotSettings, pIO);
    if (dlgPageSetup.DoModal() == IDOK)
    {
      pDb->endTransaction();
    }
    else
    {
      pDb->abortTransaction();
    }
  }
};

void COdaMfcAppDoc::OnPageSetup()
{
  if (getViewer())
  {
    const OdGsView* pView = getViewer()->getActiveView();
    if (pView)
    {
      OdDbObjectPtr pVpObj = m_pDb->activeViewportId().safeOpenObject(OdDb::kForWrite);
      OdDbAbstractViewportDataPtr pAVD(pVpObj);
      pAVD->setView(pVpObj, pView);
    }
  }

  OdStaticRxObject<OdPageSetupCmd> m_cmd_PageSetup;

  ::odedRegCmds()->addCommand(&m_cmd_PageSetup);
  ExecuteCommand( OD_T("PageSetup "));
  ::odedRegCmds()->removeCmd(&m_cmd_PageSetup);

  // Get PlotInfo for active layout (Printer/Plotter name, Paper size...)
}

void COdaMfcAppDoc::OnEditRecentcommand() 
{
  ExecuteCommand(theApp.getRecentCmd());
}

void COdaMfcAppDoc::OnUpdateEditRecentcommand(CCmdUI* pCmdUI) 
{
  OdString sTxt;
  OdString sRecentCmd = theApp.getRecentCmd();
  OdEdCommandPtr pCmd;
  if (!sRecentCmd.isEmpty())
    pCmd = ::odedRegCmds()->lookupCmd(sRecentCmd.spanExcluding(L" \n"));
  if (pCmd.get())
  {
    pCmdUI->Enable(TRUE);
    sRecentCmd.replace(L"\n",L"\\n");
    sTxt.format(L"ExecuteCommand \"%s\"", sRecentCmd.c_str());
  }
  else
  {
    pCmdUI->Enable(FALSE);
    sTxt = L"Recent command";
  }
  pCmdUI->SetText(sTxt);
}

void COdaMfcAppDoc::OnPurgeAll() 
{
  ExecuteCommand(L"-purge A *\nN");
}

void COdaMfcAppDoc::OnPurgeBlocks() 
{
  ExecuteCommand(L"-purge B *\nN");
}

void COdaMfcAppDoc::OnPurgeDimStyles() 
{
  ExecuteCommand(L"-purge D *\nN");
}

void COdaMfcAppDoc::OnPurgeLayers() 
{
  ExecuteCommand(L"-purge LA *\nN");
}

void COdaMfcAppDoc::OnPurgeLtypes() 
{
  ExecuteCommand(L"-purge LT *\nN");
}

void COdaMfcAppDoc::OnPurgeTextStyles() 
{
  ExecuteCommand(L"-purge ST *\nN");
}

void COdaMfcAppDoc::OnUpdatePurge(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable(!::odedRegCmds()->lookupCmd(L"-purge").isNull());
}

void COdaMfcAppDoc::OnEditPaste()
{
  COleDataObject dobj;
  OdSharedPtr<ClipboardData> pData = ClipboardData::get(&dobj, true);
  if (pData.get())
  {
    try
    {
      OdDbDatabasePtr pDb = theApp.readFile( pData->tempFileName(), true, false, Oda::kShareDenyNo ); 
      m_pDb->insert( OdGeMatrix3d::kIdentity, pDb );
    }
    catch(const OdError& e)
    {
      AfxMessageBox(e.description(),MB_OK);
    }
    UpdateAllViews(0);
  }
}

void COdaMfcAppDoc::startDrag(const OdGePoint3d& pt)
{
  DataSource ds;
  ds.Create(this, pt);
  if ( ds.DoDragDrop( ) )
    UpdateAllViews(0);
}

void COdaMfcAppDoc::DataSource::Create(COdaMfcAppDoc* pDoc, const OdGePoint3d& p2)
{
  Empty();

  OdDbObjectIdArray objs = pDoc->selectionSet()->objectIdArray();

  OdDbDatabasePtr pDb = pDoc->m_pDb->wblock( objs, OdGePoint3d::kOrigin );
  OdString tmpPath = pDoc->m_pDb->appServices()->getTempPath();
  TCHAR tempname[MAX_PATH];
  ::GetTempFileName(tmpPath.c_str(),_T(""),0,tempname);
  m_tmpPath = tempname;
  m_tmpPath.makeLower();
  m_tmpPath.replace(L".tmp",L".dwg");
  OdStreamBufPtr sbuf = theApp.createFile( m_tmpPath,Oda::kFileWrite,Oda::kShareDenyWrite,Oda::kCreateNew );

  //pDb->writeFile(sbuf,OdDb::kDwg,OdDb::vAC21);
  //HGLOBAL hGlobal = GlobalAlloc(GMEM_FIXED, sizeof(AcadClipDataR15));
  //new (hGlobal) AcadClipDataR15( m_tmpPath, OdString(pDoc->GetPathName()), p2 );
  //CacheGlobalData(ClipboardData::m_FormatR16, hGlobal);

  pDb->writeFile(sbuf,OdDb::kDwg,OdDb::vAC21);
  HGLOBAL hGlobalR21 = GlobalAlloc(GMEM_FIXED, sizeof(AcadClipDataR21));
  new (hGlobalR21) AcadClipDataR21( m_tmpPath, OdString(pDoc->GetPathName()), p2 );
  CacheGlobalData(ClipboardData::m_FormatR17, hGlobalR21);
}

COdaMfcAppDoc::DataSource::DataSource()
{
}

bool COdaMfcAppDoc::DataSource::DoDragDrop()
{
  return ( COleDataSource::DoDragDrop(DROPEFFECT_COPY|DROPEFFECT_MOVE) != DROPEFFECT_NONE );
}

void COdaMfcAppDoc::DataSource::Empty()
{
  COleDataSource::Empty();
  if ( !m_tmpPath.isEmpty() )
    DeleteFile( m_tmpPath );
}

COdaMfcAppDoc::DataSource::~DataSource()
{
  Empty();
}

void COdaMfcAppDoc::OnEditCopy()
{
  try
  {
    DataSource* pDS = new DataSource();
    pDS->Create( this );
    pDS->SetClipboard();
  }
  catch (OdError er)
  {
    theApp.messageBox(NULL, _T("EditCopy"), er.description().c_str(), MB_ICONWARNING);
  }
}

void COdaMfcAppDoc::OnUpdateEditCopy(CCmdUI *pCmdUI)
{
  pCmdUI->Enable( selectionSet()->numEntities() == 0 ? 0 : 1 );
}

void COdaMfcAppDoc::OnUpdateEditPaste(CCmdUI *pCmdUI)
{
  COleDataObject dobj;
  pCmdUI->Enable( ClipboardData::isAcadDataAvailable(&dobj, true) );
}


void COdaMfcAppDoc::OnEditReduce() 
{
  ExecuteCommand(L"reduce No");
}

void COdaMfcAppDoc::DeleteSelection(bool bForce)
{
  if( m_pDb->appServices()->getPICKFIRST() && selectionSet()->numEntities() )
  {
    if(bForce)
      ExecuteCommand(L"ForceErase");
    else
      ExecuteCommand(L"erase");

    if (m_pViewer)
      m_pViewer->editorObject().Set_Entity_centers();
  }
}

void COdaMfcAppDoc::OnFilePlotStyleManager()
{

    OdString sPath = COdaMfcAppApp::BrowseWithPreview(theApp.GetMainWnd()->GetSafeHwnd(),
    _T("CTB files (*.ctb)|*.ctb|STB files (*.stb)|*.stb|All Files (*.*)|*.*||"));
    if(!sPath.getLength())
    {
      return;
    }

    OdStreamBufPtr pFileBuf;
    bool bSucc(false);
    OdDbSystemServices* pSs = odSystemServices();
    try{

      if(pSs->accessFile(sPath, Oda::kFileRead))
      {
         bSucc = true;
         pFileBuf = pSs->createFile(sPath);
      }
      if(!bSucc) return;
      
      OdPsPlotStyleTablePtr pPlotStyleTable;
      if(pFileBuf.get())
      {
        OdPsPlotStyleServicesPtr pPSS = odrxDynamicLinker()->loadApp(ODPS_PLOTSTYLE_SERVICES_APPNAME);
        if(pPSS.get())
        {
          pPlotStyleTable = pPSS->loadPlotStyleTable(pFileBuf);
        }
      }

      CPsTableEditorDlg PsTableEditorDlg(theApp.GetMainWnd());
      PsTableEditorDlg.SetFileBufPath(sPath);
      PsTableEditorDlg.SetPlotStyleTable(pPlotStyleTable);
      if( PsTableEditorDlg.DoModal() == IDOK)
      {
        pPlotStyleTable->copyFrom(PsTableEditorDlg.GetPlotStyleTable());
      }
    }catch(...)
    {
      return;
    }
}

CDwgViewer* COdaMfcAppDoc::getViewer() 
{
  return m_pViewer;
}

bool COdaMfcAppDoc::getTryIncSave()
{
  return m_bTryIncSave;
}

void COdaMfcAppDoc::setTryIncSave(bool tryIncSave)
{
  m_bTryIncSave = tryIncSave;
}

BOOL COdaMfcAppDoc::CanCloseFrame(CFrameWnd* pFrame) 
{
  CView* pView = pFrame->GetActiveView();
  if(pView->IsKindOf(&CDwgViewer::classCDwgViewer))
  {
    if(!static_cast<CDwgViewer*>(pView)->canClose())
      return FALSE;
  }
  return CDocument::CanCloseFrame(pFrame);
}

void COdaMfcAppDoc::OnEditSelectall()
{
  OnEditClearselection();
  m_bDisableClearSel = true;
  ExecuteCommand(OD_T("select single all"));
  m_bDisableClearSel = false;
  POSITION pos = GetFirstViewPosition();
  while (pos != NULL)
  {
    CView *view = GetNextView(pos);
    if (view->IsKindOf(RUNTIME_CLASS(CDwgViewer)) && !view->IsKindOf(RUNTIME_CLASS(CRemoteGeomViewer)) &&
        view->GetDocument() == this)
    {
      CDwgViewer *pDwgViewer = static_cast<CDwgViewer*>(view);
      pDwgViewer->editorObject().selectionSetChanged();
    }
  }  
}

void COdaMfcAppDoc::OnEditClearselection() 
{
  //selectionSet()->clear();
  if (m_bDisableClearSel) return;
  bool cleared = false;
  POSITION pos = GetFirstViewPosition();
  while (pos != NULL)
  {
    CView *view = GetNextView(pos);
    if (view->IsKindOf(RUNTIME_CLASS(CDwgViewer)) && !view->IsKindOf(RUNTIME_CLASS(CRemoteGeomViewer)) &&
        view->GetDocument() == this)
    {
      CDwgViewer *pDwgViewer = static_cast<CDwgViewer*>(view);
      pDwgViewer->editorObject().unselect();
      cleared = true;
    }
  }
  if (!cleared) // No view found
    selectionSet()->clear();
}

void COdaMfcAppDoc::OnEditExplode() 
{
  ExecuteCommand(L"explode");
}

void COdaMfcAppDoc::OnEditEntget()
{
  OdDbSelectionSetIteratorPtr pIter = selectionSet()->newIterator();
  if (!pIter->done())
  {
    OdDbObjectId selId = pIter->objectId();
    CEditProperties dlg( selId, theApp.GetMainWnd() );
    m_pDb->startUndoRecord();
    dlg.DoModal();
    OdDbAssocManager::evaluateTopLevelNetwork(m_pDb);
  }
}
void COdaMfcAppDoc::OnOptionsDisableAnnotations()
{
  OdRxObjectPtr obj = odrxSysRegistry()->getAt(L"ODRX_ANNOTATION_COMPOSITION_DISABLE");
  if (obj.isNull())
    odrxSysRegistry()->putAt(L"ODRX_ANNOTATION_COMPOSITION_DISABLE", OdDbLine::createObject());
  else
    odrxSysRegistry()->remove(L"ODRX_ANNOTATION_COMPOSITION_DISABLE");
}
void COdaMfcAppDoc::OnOptionsDisableAnnotationsUI(CCmdUI *pCmdUI)
{
#ifdef _DEBUG
  pCmdUI->Enable(TRUE);
  OdRxObjectPtr obj = odrxSysRegistry()->getAt(L"ODRX_ANNOTATION_COMPOSITION_DISABLE");
  pCmdUI->SetCheck(obj.isNull() ? FALSE : TRUE);
#else
  pCmdUI->Enable(FALSE);
#endif
}

void COdaMfcAppDoc::OnOptionsDisableFieldUpdate()
{
  
  theApp.m_bDisableFieldUpdateOnOpen = !theApp.m_bDisableFieldUpdateOnOpen;
}

void COdaMfcAppDoc::OnOptionsDisableFieldUpdateUI(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(TRUE);
  pCmdUI->SetCheck(theApp.m_bDisableFieldUpdateOnOpen ? TRUE : FALSE);
}

void COdaMfcAppDoc::OnViewRenderDbLights()
{
  CDbLightsDlg dlg(m_pDb);
  dlg.DoModal();
}

void COdaMfcAppDoc::OnViewRenderDbMaterials()
{
  CDbMaterialsDlg dlg(m_pDb);
  dlg.DoModal();
}

void COdaMfcAppDoc::OnViewRenderDbVisualStyles()
{
  CDbVisualStylesDlg dlg(m_pDb);
  if (dlg.DoModal() == IDYES)
  {
    POSITION pos = GetFirstViewPosition();
    while (pos != NULL)
    {
      CView *view = GetNextView(pos);
      if (view->IsKindOf(RUNTIME_CLASS(CDwgViewer)) && !view->IsKindOf(RUNTIME_CLASS(CRemoteGeomViewer)) &&
          view->GetDocument() == this)
      {
        CDwgViewer *pDwgViewer = static_cast<CDwgViewer*>(view);
        pDwgViewer->getDevice()->invalidate();
        pDwgViewer->PostMessage(WM_PAINT);
        pDwgViewer->propagateActiveViewChanges();
      }
    }  
  }
}

void COdaMfcAppDoc::OnViewRenderEnvironment()
{
  CDbRenderEnvironmentDlg dlg(m_pDb);
  dlg.DoModal();
}

#ifdef ENABLE_VBA_SUPPORT
STDMETHODIMP Documents::InterfaceSupportsErrorInfo(REFIID riid)
{
  static const IID* arr[] = 
  {
    &IID_IOdaMfcDocs,
  };

  for (int i=0;i<sizeof(arr)/sizeof(arr[0]);i++)
  {
    if (InlineIsEqualGUID(*arr[i],riid))
      return S_OK;
  }
  return S_FALSE;
}

STDMETHODIMP Documents::get_Application(IOdaMfcApp **pVal)
{
  return theApp.ApcGetPrimary(pVal);
}

STDMETHODIMP Documents::get_Parent(IOdaMfcApp **pVal)
{
  return theApp.ApcGetPrimary(pVal);
}

STDMETHODIMP Documents::get_Count(long *pVal)
{
  if(!pVal) 
    return E_INVALIDARG;

  *pVal = m_listDocs.GetCount();

  return NOERROR;
}

STDMETHODIMP Documents::get__NewEnum(IUnknown **pVal)
{
  if(!pVal)
    return E_INVALIDARG;

  EnumDocs* pNew = new EnumDocs(this);

  if(!pNew)
    return E_OUTOFMEMORY;

  return pNew->QueryInterface(IID_IUnknown, (LPVOID*)pVal);
}

STDMETHODIMP Documents::Item(VARIANT Index, IOdaMfcDoc **pVal)
{
  POSITION pos = NULL;
  HRESULT hr;
  VARIANT varUse;
  VariantInit(&varUse);
  *pVal = NULL;

  if(SUCCEEDED(hr = VariantChangeType(&varUse, &Index, 0, VT_I4)))
  {
    //1 based indexing in Basic.
    pos = m_listDocs.FindIndex(varUse.lVal - 1);
    if(pos)
    {
      *pVal = (IOdaMfcDoc*)m_listDocs.GetAt(pos);
      (*pVal)->AddRef();
      return NOERROR;
    }
  }
  else if(hr == DISP_E_TYPEMISMATCH && Index.vt == VT_BSTR)
  {
    CString strTest(Index.bstrVal);

    pos = m_listDocs.GetHeadPosition();
    while(pos)
    {
      IOdaMfcDoc* pTest;
      BSTR bstrTest;
      pTest = (IOdaMfcDoc*)m_listDocs.GetNext(pos);
      pTest->get_Name(&bstrTest);
      if(!strTest.CompareNoCase(CString(bstrTest)))
      {
        SysFreeString(bstrTest);
        pTest->AddRef();
        *pVal = pTest;
        return NOERROR;
      }
      SysFreeString(bstrTest);
    }
  }

  SetErrorInfo(IDS_ERROR_NOITEM);
  return E_INVALIDARG;
}

STDMETHODIMP Documents::Open(BSTR FileName, IOdaMfcDoc **ppVal)
{
  CString strFileName(FileName);

  COdaMfcAppDoc* pDoc = (COdaMfcAppDoc*)theApp.OpenDocumentFile(strFileName);

  if(pDoc)
    pDoc->ApcGetPrimary(ppVal);
  else
  {
    SetErrorInfo(IDS_ERROR_OPENFAILED);
    return E_FAIL;
  }
  
  return S_OK;
}

STDMETHODIMP Documents::Add(IOdaMfcDoc **ppVal)
{
  return theApp.AutomationFileNew(ppVal);
}

STDMETHODIMP EnumDocs::QueryInterface(REFIID riid, LPVOID* ppvObj)
{
  if(IsEqualGUID(riid, IID_IUnknown))
  {
    *ppvObj = (LPVOID)(IUnknown*)this;
  }
  else if(IsEqualGUID(riid, IID_IEnumVARIANT))
  {
    *ppvObj = (LPVOID)(IEnumVARIANT*)this;
  }
  else
  {
    return E_NOINTERFACE;
  }

  AddRef();

  return NOERROR;
}

STDMETHODIMP EnumDocs::Next(ULONG celt, VARIANT* rgvar, ULONG* celtFetched)
{
  ULONG ulFetched = 0;
  IOdaMfcDoc* pDoc;

  for(ULONG i = 0; i < celt; i++)
  {
    if(m_posCur == NULL)
      break;

    VariantInit(&(rgvar[i]));

    pDoc = (IOdaMfcDoc*)m_plistDocs->GetNext(m_posCur);

    V_VT(&(rgvar[i])) = VT_DISPATCH;
    pDoc->QueryInterface(IID_IDispatch, (LPVOID*)&(rgvar[i].pdispVal));

    ulFetched++;
  }

  if(celtFetched != NULL)
  {
    *celtFetched = ulFetched;
  }

  return ulFetched < celt ? S_FALSE : NOERROR;
}

STDMETHODIMP EnumDocs::Skip(ULONG celt)
{
  ULONG ulSkipped = 0;
  void* pSkip;

  for(ULONG i = 0; i < celt; i++)
  {
    if(m_posCur == NULL)
      break;

    pSkip = m_plistDocs->GetNext(m_posCur);

    ulSkipped++;
  }

  return ulSkipped < celt ? S_FALSE : NOERROR;
}

STDMETHODIMP EnumDocs::Reset()
{
  m_posCur = m_plistDocs->GetHeadPosition();

  return NOERROR;
}

STDMETHODIMP EnumDocs::Clone(IEnumVARIANT** ppEnum)
{
  if(!ppEnum)
    return E_INVALIDARG;

  EnumDocs* pNew = new EnumDocs(this);

  if( !pNew ) return E_OUTOFMEMORY;

  return pNew->QueryInterface(IID_IEnumVARIANT, (LPVOID*)ppEnum);
}

EnumDocs::EnumDocs(Documents* pDocs)
{
  m_cRef = 0;
  m_plistDocs = &(pDocs->m_listDocs);
  m_posCur = m_plistDocs->GetHeadPosition();
}

EnumDocs::EnumDocs(EnumDocs* pEnum)
{
  m_cRef = 0;
  m_plistDocs = pEnum->m_plistDocs;
  m_posCur = pEnum->m_posCur;
}


HRESULT Documents::AddToCollection(IOdaMfcDoc *pDoc)
{
  m_listDocs.AddTail((LPVOID)pDoc);
  return NOERROR;
}

HRESULT Documents::RemoveFromCollection(CString strName)
{
  POSITION posCur, posDel;
  posCur = m_listDocs.GetHeadPosition();
  while(posCur)
  {
    IOdaMfcDoc* pDoc;
    BSTR bstrName;
    posDel = posCur;
    pDoc = (IOdaMfcDoc*)m_listDocs.GetNext(posCur);
    if(SUCCEEDED(pDoc->get_Name(&bstrName)))
    {
      if(!strName.CompareNoCase(CString(bstrName)))
      {
        SysFreeString(bstrName);
        m_listDocs.RemoveAt(posDel);
        pDoc->Release();
        break;
      }
      SysFreeString(bstrName);
    }
  }

  return NOERROR;
}

HRESULT Documents::ClearCollection()
{
  POSITION pos;
  pos = m_listDocs.GetHeadPosition();
  while(pos)
  {
    IOdaMfcDoc* pDoc;
    pDoc = (IOdaMfcDoc*)m_listDocs.GetNext(pos);
    pDoc->Release();
  }

  m_listDocs.RemoveAll();

  return NOERROR;
}
#endif // ENABLE_VBA_SUPPORT

void COdaMfcAppDoc::OnRevisionLog()
{
  CRevisionDialog dlg;
  dlg.m_pDb = m_pDb;
  dlg.DoModal();
}

void COdaMfcAppDoc::OnRevisionLogUI(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(!OdTfRevisionControl::getLocalBranch(m_pDb).isEmpty());
}

void COdaMfcAppDoc::OnImportPdf()
{
  OdRxModulePtr pModule = ::odrxDynamicLinker()->loadModule(OdPdfImportModuleName);

  if (pModule.isNull())
  {
    AfxMessageBox(OdPdfImportModuleName L" is missing (available only for vc14(2015), vc15(2017) )");
  }
  else
  {
    CPdfImportDialog dlg(m_pDb, theApp.GetMainWnd());
    if (dlg.DoModal() == IDOK)
    {
      // pdf import will add a lot of objects
      // tracing will slow down the process
      UNTRACE_DB_EVENTS(m_pDb);

      theApp.setFILEDIA(false);
      ExecuteCommand(dlg.GetCommandString());
      theApp.setFILEDIA(true);

      TRACE_DB_EVENTS(m_pDb);
    }

  }
}

