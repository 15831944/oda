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
#include "OdaDgnApp.h"
#include "OdaDgnAppDoc.h"
#include "OdaDgnTreeView.h"
#include "OdaDgnAppVectorizer.h"
#include "MainFrm.h"
#include "STLModule.h"
#include "OdStreamBuf.h"
#include "StlExportDlg.h"
#include "CSwitchModel.h"
#include "SummDlg.h"
#include "CameraView.h"
#include "CellLibraryPreview.h"
#include "Gs/GsBaseInclude.h"
#include "DgElementId.h"
#include "DgLevelMask.h"
#include "BgColorTool.h"
#include "MaterialsEditor.h"
#include "LevelPropManagerDlg.h"
#include "CompressDlg.h"
#include "ItemTypeLibsDlg.h"
#include "DgExtendedElement.h"
#include "CPdfRasterAttachmentDialog.h"
#include "LineStyleScaleDlg.h"

#ifdef OBJTOOLKIT_ENABLED
#include "OBJToolkit.h"
#endif

//#include "Ed/EdCommandStack.h"
#include "ExDgCommandContext.h"
#include "SaveState.h"
#include "ExStringIO.h"
#include "UserIOConsole.h"

#include "PageSetupDlg.h"
#include "PreviewDib.h"
#include "AbstractViewPE.h"
#include "dgbrepentitype.h"
#include "Ge/GeCircArc3d.h"
#include "DgModelerGeometryCreator.h"
#include "MemoryStream.h"
#include "XRefMergeDlg.h"
#include "Ge/GeLine3d.h"

//#define ENABLE_HOOPS_EXPORT

#ifdef ENABLE_HOOPS_EXPORT

#pragma comment(lib,"TD_HoopsExport.lib")
#include "HoopsExport.h"

#endif

#define ENABLE_PDF_EXPORT
#ifdef ENABLE_PDF_EXPORT
#include "PdfExport.h"
#include "Export2PDFDialog.h"
using namespace TD_PDF_2D_EXPORT;
#endif

#define ENABLE_DWF_EXPORT
#ifdef ENABLE_DWF_EXPORT
#include "DwfExport.h"
using namespace TD_DWF_EXPORT;
#endif


//#define CREATE_DATABASEDOC(ClassN) OdDgDatabaseDocPtr(new OdDgDatabaseDoc, kOdRxObjAttach)
//
//ODRX_CONS_DEFINE_MEMBERS(OdDgDatabaseDoc, OdDgDatabase, CREATE_DATABASEDOC);

#define NEW_CONSTR(CLASS) OdSmartPtr<CLASS>(new CLASS, kOdRxObjAttach)

ODRX_CONS_DEFINE_MEMBERS(OdDgDatabaseDoc, OdDgDatabase, NEW_CONSTR);

COdaDgnAppDoc* OdDgDatabaseDoc::g_pDoc = 0;
#include "STLModule.h"
using namespace TD_STL_EXPORT;

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
	ON_COMMAND(ID_VECTORIZE, OnVectorize)
  ON_COMMAND(ID_FILE_COMPRESS, OnFileCompress)
	ON_COMMAND(ID_FILE_STL, OnExportStl)
	ON_COMMAND(ID_FILE_EXPORT_SVG, OnExportSvg)
	ON_COMMAND(ID_FILE_EXPORT_HOOPS, OnExportHoops)
  ON_COMMAND(ID_FILE_OBJ, OnExportObj)
	ON_COMMAND(ID_FILE_EXPORT_PDF, OnExportPdf)
	ON_UPDATE_COMMAND_UI(ID_VECTORIZE, OnUpdateVectorize)
	ON_UPDATE_COMMAND_UI(ID_SWITCH_MODEL, OnUpdateSwitchModel)
	ON_COMMAND(ID_SWITCH_MODEL, OnSwitchModel)
  ON_UPDATE_COMMAND_UI(ID_ITEM_TYPE_LIB_BUTTON, OnUpdateItemTypeLibs)
  ON_COMMAND(ID_ITEM_TYPE_LIB_BUTTON, OnItemTypeLibs)
	ON_COMMAND(ID_TESTS_VIEWCAMERA, OnTestsViewcamera)
	ON_COMMAND(ID_TESTS_CELLLIBRARYPREVIEW, OnTestsCellLibraryPreview)
	ON_COMMAND(ID_TESTS_LOADSMARTSOLID, OnTestsLoadSmartSolid)
  ON_COMMAND(ID_TESTS_LOADLINESTYLES, OnTestsLoadLineStyles)
  ON_COMMAND(ID_TESTS_MERGE_MODELS, OnTestsMergeModels)
  ON_COMMAND(ID_TESTS_MERGE_XREF, OnTestsMergeXRef)
	ON_COMMAND(ID_VIEW_SUMMINFO, OnViewSumminfo)
	ON_COMMAND(ID_EDIT_SELECT_ALL, OnEditSelectall)
	ON_COMMAND(ID_EDIT_CLEARSELECTION, OnEditClearselection)
	ON_COMMAND(ID_EDIT_SELECTIONREACTOR, OnEditSelectionReactor)
	ON_COMMAND(ID_FILE_PAGE_SETUP, OnFilePageSetup)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SELECT_ALL, OnUpdateEditSelectall)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLEARSELECTION, OnUpdateClearselection)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SELECTIONREACTOR, OnUpdateEditSelectionReactor)
  ON_COMMAND(ID_OPTIONS_APPLYGLOBALORIGIN, OnOptionsApplyGlobalOrigin)
  ON_UPDATE_COMMAND_UI(ID_OPTIONS_APPLYGLOBALORIGIN, OnUpdateOptionsApplyGlobalOrigin)
	ON_COMMAND(ID_EDIT_RECENTCOMMAND, OnEditRecentcommand)
	ON_UPDATE_COMMAND_UI(ID_EDIT_RECENTCOMMAND, OnUpdateEditRecentcommand)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, OnUpdateEditRedo)
	ON_COMMAND(ID_EDIT_REDO, OnEditRedo)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_FILE_EXPORT_DWF, OnExportDwf)
	ON_COMMAND(ID_FILE_BMPEXPORT, OnBmpExport)
	ON_UPDATE_COMMAND_UI(ID_FILE_BMPEXPORT, OnUpdateBmpExport)
	ON_COMMAND(ID_TOOLS_BGCOLOR, OnToolsOptionsBgColor)
  ON_COMMAND(ID_OPTIONS_LSSCALE, OnToolOptionsLineStyleScale)
	ON_COMMAND_RANGE(ID_VIEW1, ID_VIEW8, OnViewNumber)
	ON_UPDATE_COMMAND_UI(ID_VIEW1, OnUpdateView1)
	ON_UPDATE_COMMAND_UI(ID_VIEW2, OnUpdateView2)
	ON_UPDATE_COMMAND_UI(ID_VIEW3, OnUpdateView3)
	ON_UPDATE_COMMAND_UI(ID_VIEW4, OnUpdateView4)
	ON_UPDATE_COMMAND_UI(ID_VIEW5, OnUpdateView5)
	ON_UPDATE_COMMAND_UI(ID_VIEW6, OnUpdateView6)
	ON_UPDATE_COMMAND_UI(ID_VIEW7, OnUpdateView7)
	ON_UPDATE_COMMAND_UI(ID_VIEW8, OnUpdateView8)
	ON_COMMAND(ID_RENDER_LIGHTS, &COdaDgnAppDoc::OnRenderLights)
	ON_COMMAND(ID_RENDER_MATERIALS, &COdaDgnAppDoc::OnRenderMaterials)
  ON_COMMAND(ID_INSERT_RASTER_ATTACH, OnRasterAttach)
	ON_COMMAND(ID_TOOLS_LEVEL, OnToolsLevelManager)
END_MESSAGE_MAP()

void CDgPreviewDIB::SetPreviewFile(LPCTSTR lpFileName)
{
	CString  csFileName(lpFileName);

	m_odImage.header.clear();
	m_odImage.bmp.clear();
	m_odImage.wmf.clear();
	m_odImage.png.clear();

	if (!csFileName.GetLength())
		return;

	CString ext = csFileName.Right(4);
	ext.MakeLower();

	if (ext == _T(".dgn"))
	{
		try
		{
			OdBinaryData dibData;

			if (theApp.getThumbnail(lpFileName, dibData))
			{
				m_odImage.bmp = dibData;
			}
		}
		catch (...)
		{
			return;
		}
	}
#ifdef TDRAWING_ENABLED
  else if (ext == _T(".dwg") || ext == _T(".dxf"))
  {
    OdStreamBufPtr pFile;
    try
    {
      odDbGetPreviewBitmap(theApp.createFile(OdString((LPCTSTR)csFileName)), &m_odImage);
      m_odImage.convPngToBmp();
    }
    catch (...)
    {
      return;
    }
  }
#endif
}

CRect CDgPreviewDIB::Calc(int bmpWid, int bmpDep, int wndWid, int wndDep)
{
	int d, w, x, y;
	if (bmpDep > bmpWid)
	{
		d = __min(bmpDep, wndDep);
		w = (bmpWid*d) / bmpDep;
		if (w > wndWid)
		{
			d = (d*wndWid) / w;
			w = wndWid;
		}
	}
	else
	{
		w = __min(bmpWid, wndWid);
		d = (bmpDep*w) / bmpWid;
		if (d > wndDep)
		{
			w = (w*wndDep) / d;
			d = wndDep;
		}
	}
	x = (wndWid - w) / 2;
	y = (wndDep - d) / 2;
	return CRect(x, y, x + w, y + d);
}

void CDgPreviewDIB::DrawPreview(HDC dc, int X, int Y, int width, int height)
{
	CRect   cr;

	if (m_odImage.hasBmp())
	{
		BITMAPINFOHEADER *pHeader;
		pHeader = (BITMAPINFOHEADER *)(m_odImage.bmp.begin());

    if (!pHeader->biWidth || !pHeader->biHeight)
      return;
		cr = Calc(pHeader->biWidth, pHeader->biHeight, width, height);

		unsigned char *p = (unsigned char*)pHeader;
		p += pHeader->biSize;
		switch (pHeader->biBitCount)
		{
		case 1:
			p += sizeof(RGBQUAD) * 2;
			break;
		case 4:
			p += sizeof(RGBQUAD) * 16;
			break;
		case 8:
			p += sizeof(RGBQUAD) * 256;
			break;
		}

		StretchDIBits(dc, cr.left + X, cr.top + Y, cr.Width(), cr.Height(), 0, 0,
			pHeader->biWidth, pHeader->biHeight, (const void *)p,
			(CONST BITMAPINFO *)pHeader, DIB_RGB_COLORS, SRCCOPY);
	}
}

// COdaDgnAppDoc construction/destruction

COdaDgnAppDoc::COdaDgnAppDoc()
	: m_SaveAsVer(0)
	, m_SaveAsType(0)
	, m_bConsole(false)
	, m_bConsoleResponded(false)
	, m_nCmdActive(0)
	, m_pVectorizer(0)
	, m_bDisableClearSel(false)
{
	// TODO: add one-time construction code here

}

COdaDgnAppDoc::~COdaDgnAppDoc()
{
}

void COdaDgnAppDoc::OnCloseDocument()
{
	CDocument::OnCloseDocument();
}

BOOL COdaDgnAppDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	OdDgDatabaseDoc::setDocToAssign(this);

	m_pDb = theApp.createDatabase();

	theApp.setStatusText(_T("Default Database Created."));
	return TRUE;
}

BOOL COdaDgnAppDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	OdDgDatabaseDoc::setDocToAssign(this);
	m_pDb = theApp.openFile(lpszPathName);
	return !m_pDb.isNull();
}

BOOL COdaDgnAppDoc::DoPromptFileName(CString& fileName, UINT nIDSTitle, DWORD lFlags, BOOL bOpenFileDialog, CDocTemplate* /*pTemplate*/)
{
	//OdDg::DwgVersion dwgver = m_pDb->originalFileVersion();
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

	//strFilter += _T("DGN V7 file format (*.dgn)");
	//strFilter += (TCHAR)'\0';   // next string please
	//strFilter += _T("*.dgn");
	//strFilter += (TCHAR)'\0';   // last string
	//dlgFile.m_ofn.nMaxCustFilter++;

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
	switch (dlgFile.m_ofn.nFilterIndex)
	{
	case 1:
		m_SaveAsVer = 0;
		break;
	case 2:
		m_SaveAsVer = 1;
		break;
	default:
		m_SaveAsVer = 0;
		//m_SaveAsVer = m_pDb->originalFileVersion();
	};

	return nResult == IDOK;
}

BOOL COdaDgnAppDoc::DoSave(LPCTSTR lpszPathName, BOOL bReplace)
// Save the document data to a file
// lpszPathName = path name where to save document file
// if lpszPathName is NULL then the user will be prompted (SaveAs)
// note: lpszPathName can be different than 'm_strPathName'
// if 'bReplace' is TRUE will change file name if successful (SaveAs)
// if 'bReplace' is FALSE will not change path name (SaveCopyAs)
{
	//m_SaveAsVer = m_pDb->originalFileVersion();

	CString newName = lpszPathName;
	if (newName.IsEmpty())
	{
		CDocTemplate* pTemplate = GetDocTemplate();
		ODA_ASSERT(pTemplate != NULL);

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
				ODA_ASSERT(strExt[0] == '.');
				newName += strExt;
			}
		}

		if (!DoPromptFileName(newName,
			bReplace ? AFX_IDS_SAVEFILE : AFX_IDS_SAVEFILECOPY,
			OFN_HIDEREADONLY | OFN_EXPLORER | OFN_PATHMUSTEXIST, FALSE, pTemplate))
			return FALSE;       // don't even attempt to save

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

void COdaDgnAppDoc::OnToolOptionsLineStyleScale()
{
  CLineStyleScaleDlg dlg;

  if (!m_pDb.isNull())
  {
    OdDgElementId idModel = m_pDb->getActiveModelId();

    if (idModel.isNull())
      idModel = m_pDb->getDefaultModelId();

    if (!idModel.isNull())
    {
      OdDgModelPtr pModel = idModel.openObject(OdDg::kForWrite);

      if( pModel->getCompoundLineScaleFlag() )
        dlg.m_iLineStyleScaleType = 2;
      else if( pModel->getAnnotationLineScaleFlag() )
        dlg.m_iLineStyleScaleType = 1;
      else
        dlg.m_iLineStyleScaleType = 0;

      dlg.m_dModelGlobalScale     = pModel->getLinestyleScale();
      dlg.m_dModelAnnotationScale = pModel->getAnnotationScale();
      dlg.m_dDbGlobalScale        = m_pDb->getLineStyleScale();

      if( dlg.DoModal() == IDOK )
      {
        m_pDb->startUndoRecord();
        m_pDb->startTransaction();

        switch( dlg.m_iLineStyleScaleType )
        {
          case 0:
          {
            pModel->setCompoundLineScaleFlag(false);
            pModel->setAnnotationLineScaleFlag(false);
          } break;

          case 1:
          {
            pModel->setCompoundLineScaleFlag(false);
            pModel->setAnnotationLineScaleFlag(true);
          } break;

          case 2:
          {
            pModel->setCompoundLineScaleFlag(true);
            pModel->setAnnotationLineScaleFlag(false);
          } break;
        }

        pModel->setAnnotationScale(dlg.m_dModelAnnotationScale);
        pModel->setLinestyleScale(dlg.m_dModelGlobalScale);
        m_pDb->setLineStyleScale(dlg.m_dDbGlobalScale);

        m_pDb->endTransaction();

        if (m_pVectorizers.size() > 0)
        {
          COdaDgnAppVectorizer* pVectorizer;

          for (std::set< COdaDgnAppVectorizer* >::iterator i = m_pVectorizers.begin(); i != m_pVectorizers.end(); i++)
          {
            pVectorizer = *i;
            OdGsDeviceForDgModelPtr pDevice = pVectorizer->getDevice();

            pDevice->invalidate();
            if (pDevice->gsModel())
              pDevice->gsModel()->invalidate(OdGsModel::kInvalidateAll);
          }
        }
      }
    }
  }
}

void COdaDgnAppDoc::OnOptionsApplyGlobalOrigin()
{
  theApp.setApplyGlobalOriginFlag(!theApp.applyGlobalOriginFlag());

  if( !m_pDb.isNull() )
  {
    OdDgModelTablePtr pModelTable = m_pDb->getModelTable(OdDg::kForWrite);
    OdDgElementIteratorPtr pIter = pModelTable->createIterator();

    for(; !pIter->done(); pIter->step())
    {
      OdDgModelPtr pModel = pIter->item().openObject(OdDg::kForWrite);
      pModel->enableGlobalOriginUsage(theApp.applyGlobalOriginFlag());
    }

    COdaDgnAppVectorizer* pVectorizer = getVectorizer();

    if( pVectorizer )
    {
      OdGsDeviceForDgModelPtr pDevice = pVectorizer->getDevice();

      OdDgViewPtr pDgView = pVectorizer->getViewId().openObject(OdDg::kForRead);

      if( !pDevice.isNull() && !pDgView.isNull() )
      {
        for (int j = 0; j < pDevice->numViews(); j++)
        {
          OdGsView* pGsView = pDevice->viewAt(j);

          OdAbstractViewPEPtr pAVP(pGsView);
          pAVP->setView(pGsView, pDgView);
        }

        pDevice->invalidate();
        if (pDevice->gsModel())
          pDevice->gsModel()->invalidate(OdGsModel::kInvalidateAll);

        pVectorizer->PostMessage(WM_PAINT);
      }
    }

    UpdateAllViews(NULL);
  }
}

void COdaDgnAppDoc::OnUpdateOptionsApplyGlobalOrigin(CCmdUI* pCmdUI)
{
  pCmdUI->SetCheck(theApp.applyGlobalOriginFlag());
}

void COdaDgnAppDoc::OnToolsOptionsBgColor()
{
	bool bPaletteAltered = false;
	const ODCOLORREF* pColorPaletteOriginal;
	ODCOLORREF pColorPaletteNew[256];

	OdDgColorTablePtr pColorTable = m_pDb->getColorTable(OdDg::kForRead);

	if (!pColorTable.isNull())
	{
		pColorPaletteOriginal = pColorTable->palette();
	}
	else
	{
		pColorPaletteOriginal = OdDgColorTable::defaultPalette();
		bPaletteAltered = true;
	}

	CBgDialog appDlg(pColorTable, m_pDb);

	if (appDlg.DoModal() == IDOK)
	{
		OdArray<ODCOLORREF> colorPaletteArray;
		colorPaletteArray = appDlg.GetColorPaletteArray();
		for (int i = 0; i < 256; i++)
		{
			pColorPaletteNew[i] = colorPaletteArray[i];
		}

		for (int i = 0; i < 256; i++)
		{
			if (pColorPaletteOriginal[i] != pColorPaletteNew[i])
			{
				bPaletteAltered = true;
				break;
			}
		}
		if (bPaletteAltered)
		{
			m_pDb->getColorTable(OdDg::kForWrite)->setPalette(colorPaletteArray);
			OdDgModelPtr pModel = m_pDb->getActiveModelId().safeOpenObject(OdDg::kForWrite);
			pModel->setBackground(colorPaletteArray[255]);
			UpdateVectorizersColorPalette(pColorPaletteNew);
		}
	}
}

BOOL COdaDgnAppDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	OdString strFileName(lpszPathName);
	int extPos = strFileName.reverseFind('.');
	OdString strExt = strFileName.mid(extPos + 1);
	CMainFrame* pMFrame = (CMainFrame*)theApp.GetMainWnd();
	bool bError = false;

  bool bShowFence = hideFence();

	try
	{
		pMFrame->StartTimer();
		m_pDb->writeFile(strFileName);
	}
	catch (const OdError& e)
	{
		theApp.reportError(_T("Error Saving Document..."), e);
		bError = true;
	}
#ifndef _DEBUG
	catch (...)
	{
		AfxMessageBox(_T("Error while writing DB."), MB_ICONERROR);
		bError = true;
	}
#endif // #ifndef _DEBUG
	pMFrame->StopTimer(_T("Writing"));

  if( bShowFence )
    showFence();

	if (bError)
	{
		AfxThrowUserException();
		return FALSE;
	}

	UpdateAllViews(NULL);
	return TRUE;
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


#pragma warning(push)
#pragma warning(disable:4510)
#pragma warning(disable:4610)


void COdaDgnAppDoc::OnUpdateView1(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(true);
	pCmdUI->SetCheck(1 == getActiveView());
}
void COdaDgnAppDoc::OnUpdateView2(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(true);
	pCmdUI->SetCheck(2 == getActiveView());
}
void COdaDgnAppDoc::OnUpdateView3(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(true);
	pCmdUI->SetCheck(3 == getActiveView());
}
void COdaDgnAppDoc::OnUpdateView4(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(true);
	pCmdUI->SetCheck(4 == getActiveView());
}
void COdaDgnAppDoc::OnUpdateView5(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(true);
	pCmdUI->SetCheck(5 == getActiveView());
}
void COdaDgnAppDoc::OnUpdateView6(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(true);
	pCmdUI->SetCheck(6 == getActiveView());
}
void COdaDgnAppDoc::OnUpdateView7(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(true);
	pCmdUI->SetCheck(7 == getActiveView());
}
void COdaDgnAppDoc::OnUpdateView8(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(true);
	pCmdUI->SetCheck(8 == getActiveView());
}

afx_msg void COdaDgnAppDoc::OnReToolbar(bool bSingleView)
{
  if( m_pVectorizers.size() == 0 )
    return;

	int nSize;
	if (bSingleView)
	{
		nSize = getViewsCount();
	}
	else
	{
		nSize = 0;
	}


	// Set the toolbar to show only partial commmand list
	m_pToolBar->SetButtons(NULL, 31 + nSize);

	m_pToolBar->SetButtonInfo(0, ID_VIEW_ZOOM_WINDOW, TBBS_BUTTON, 0);
	m_pToolBar->SetButtonInfo(1, ID_VIEW_ZOOMIN, TBBS_BUTTON, 1);
	m_pToolBar->SetButtonInfo(2, ID_VIEW_ZOOMOUT, TBBS_BUTTON, 2);
	m_pToolBar->SetButtonInfo(3, ID_VIEW_ZOOMEXTENTS, TBBS_BUTTON, 3);
	m_pToolBar->SetButtonInfo(4, ID_VIEW_PAN, TBBS_BUTTON, 4);
	m_pToolBar->SetButtonInfo(5, ID_VIEW_3DORBIT, TBBS_BUTTON, 5);

	m_pToolBar->SetButtonInfo(6, ID_SEPARATOR, TBBS_SEPARATOR, 33);

  m_pToolBar->SetButtonInfo(7, ID_CREATEFENCE_BTN, TBBS_BUTTON, 6);
  m_pToolBar->SetButtonInfo(8, ID_CREATEFENCE_CIRCLE, TBBS_BUTTON, 7);

	m_pToolBar->SetButtonInfo(9, ID_SEPARATOR, TBBS_SEPARATOR, 33);

	m_pToolBar->SetButtonInfo(10, ID_2DWIREFRAME, TBBS_BUTTON, 8);
	m_pToolBar->SetButtonInfo(11, ID_3DWIREFRAME, TBBS_BUTTON, 9);
	m_pToolBar->SetButtonInfo(12, ID_HIDDEN, TBBS_BUTTON, 10);
	m_pToolBar->SetButtonInfo(13, ID_FLAT_SHADED, TBBS_BUTTON, 11);
	m_pToolBar->SetButtonInfo(14, ID_GOURAUD_SHADED, TBBS_BUTTON, 12);
	m_pToolBar->SetButtonInfo(15, ID_FLAT_SHADED_WITH_EDGES, TBBS_BUTTON, 13);
	m_pToolBar->SetButtonInfo(16, ID_GOURAUD_SHADED_EDGES_ON, TBBS_BUTTON, 14);

	m_pToolBar->SetButtonInfo(17, ID_SEPARATOR, TBBS_SEPARATOR, 33);

	m_pToolBar->SetButtonInfo(18, ID_VIEWER_REGEN, TBBS_BUTTON, 25);

	m_pToolBar->SetButtonInfo(19, ID_SEPARATOR, TBBS_SEPARATOR, 33);

	m_pToolBar->SetButtonInfo(20, ID_XY_PLANE, TBBS_BUTTON, 15);
	m_pToolBar->SetButtonInfo(21, ID_BOTTOM, TBBS_BUTTON, 16);
	m_pToolBar->SetButtonInfo(22, ID_YZ_PLANE, TBBS_BUTTON, 17);
	m_pToolBar->SetButtonInfo(23, ID_RIGHT, TBBS_BUTTON, 18);
	m_pToolBar->SetButtonInfo(24, ID_XZ_PLANE, TBBS_BUTTON, 19);
	m_pToolBar->SetButtonInfo(25, ID_BACK, TBBS_BUTTON, 20);
	m_pToolBar->SetButtonInfo(26, ID_SW_ISOMETRIC, TBBS_BUTTON, 21);
	m_pToolBar->SetButtonInfo(27, ID_SE_ISOMETRIC, TBBS_BUTTON, 22);
	m_pToolBar->SetButtonInfo(28, ID_NE_ISOMETRIC, TBBS_BUTTON, 23);
	m_pToolBar->SetButtonInfo(29, ID_NW_ISOMETRIC, TBBS_BUTTON, 24);

  if( nSize > 0 )
  {
    m_pToolBar->SetButtonInfo(30, ID_SEPARATOR, TBBS_SEPARATOR, 33);
  }

	for (int i = 0; i < nSize; i++)
	{
		m_pToolBar->SetButtonInfo(31 + i, ID_VIEW1 + i, TBBS_BUTTON, 26 + i);
		m_pToolBar->SetButtonStyle(31 + i, TBBS_CHECKGROUP);
	}

	m_pToolBar->Invalidate();
	AfxGetApp()->OnIdle(-1);
}

afx_msg void COdaDgnAppDoc::OnViewNumber(UINT nID)
{
	OdUInt16 iElementNumber = OdUInt16(nID - ID_VIEW1 + 1);
	OnSwitchView(theApp.settings().getGsDevicePath(), iElementNumber);
	OnReToolbar(true);
}

void COdaDgnAppDoc::OnSwitchView(const OdString& sVectorizerPath, UINT iElementNumber)
{
  try
  {
    ::odrxDynamicLinker()->loadModule(sVectorizerPath, false);
  }
  catch(...)
  {
    AfxMessageBox(L"Wrong version of rendering device library.", MB_OK);
    return;
  }

	while (m_pVectorizers.size())
	{
		COdaDgnAppVectorizer* vectorizer = *m_pVectorizers.begin();
		vectorizer->GetOwner()->DestroyWindow(); //vectorizers are packed to CFrameWindow objects
	}
	theApp.settings().setGsDevicePath(sVectorizerPath);

	struct CDocTemplateEx : CMultiDocTemplate
	{
		void SetViewToCreate(CRuntimeClass* pViewClass)
		{
			m_pViewClass = pViewClass;
		}
	};
	CDocTemplateEx* pTemplate = (CDocTemplateEx*)GetDocTemplate();
	ASSERT_VALID(pTemplate);
	pTemplate->SetViewToCreate(RUNTIME_CLASS(COdaDgnAppVectorizer));

	// create vectorizers for all viewable views of the active view group
	// MKU 24\11\2010 #8852 (case: /dgn/Bugzilla/Rendering/Crash/5223/PwayUpsideOld.dgn)
	//  Some files can have invalid id for View Group. Try to get & use a valid (recommended) View Group object.
	OdDgViewGroupPtr pViewGroup = m_pDb->getActiveViewGroupId().openObject();

	m_vectorizedModelId = m_pDb->getActiveModelId();
	if (m_vectorizedModelId.isNull())
	{
		// MKU 24\11\2010 #8852 (case: /dgn/V7_V8legacy/Regression/out/ExLoadToModel_dll/Building/Dgn/BSI300AE9-Atrium.dgn
		//  If Model is is invalid let's try to get & use a Default.
		m_vectorizedModelId = m_pDb->getDefaultModelId();
		m_pDb->setActiveModelId(m_vectorizedModelId);
	}
	if (m_vectorizedModelId.isNull())
	{
		ODA_FAIL_ONCE();
		return;
	}

	if (pViewGroup.isNull())
	{
		pViewGroup = m_pDb->recommendActiveViewGroupId().openObject();
		if (pViewGroup.isNull())
		{
			// Add View group
			OdDgModelPtr pModel = OdDgModel::cast(m_vectorizedModelId.openObject());
			pModel->createViewGroup();
			pModel->fitToView();
			pViewGroup = m_pDb->recommendActiveViewGroupId().openObject();
		}
	}
	if (pViewGroup.isNull())
	{
		ODA_FAIL_ONCE();
		return;
	}

	//OdDgElementIteratorPtr pIt = pViewGroup->createIterator();
	CFrameWnd* pTheLastFrame = 0;
	setActiveView(iElementNumber);
	recountVeiws(pViewGroup);
	//for (UINT i = 0; i < iElementNumber - 1; i++)
	//{
	//	pIt->step();
	//}

	UINT iViewIndex = iElementNumber - 1;

	OdDgViewPtr pView;

	if (m_pViews.size() >= 1)
	{
		pView = OdDgView::cast(m_pViews[iViewIndex].openObject());

	}
	else
	{
		return;
	}

	if (pView.get() && pView->getVisibleFlag())
	{
		m_vectorizedViewId = m_pViews[iViewIndex];

		if (!pView->getModelId().isNull())
		{
			m_vectorizedModelId = pView->getModelId();
		}

		CFrameWnd* pFrame = pTemplate->CreateNewFrame(this, NULL);

		pTheLastFrame = pFrame;

		pTemplate->InitialUpdateFrame(pFrame, this);

		COdaDgnAppVectorizer* pVectorizer = static_cast<COdaDgnAppVectorizer*>(pFrame->GetActiveView());

		if (pVectorizer)
		{
			pVectorizer->setPrintSettings(m_printSettings);
		}

		m_pVectorizers.insert(pVectorizer);

		//set its position
		OdGsDCRect  rectangle = pView->getViewRectangle();
		OdGsDCPoint low = rectangle.m_min,
			hi = rectangle.m_max;

		//pFrame->MoveWindow(low.x, hi.y, hi.x - low.x, low.y - hi.y);

		//create the toolbar for the frame
		m_pToolBar = new CToolBar();
		m_pToolBar->m_bAutoDelete = TRUE;

		//toolbar->Create( pFrame );
		if (!m_pToolBar->CreateEx(pFrame,
			TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC)
			||
			!m_pToolBar->LoadToolBar(IDR_VECTORIZER))
		{
			TRACE0("Failed to create vectorizer toolbar\n");
			return;      // fail to create
		}

		//toolbar->LoadToolBar( IDR_VECTORIZER );

		m_pToolBar->EnableDocking(CBRS_ALIGN_ANY);
		pFrame->EnableDocking(CBRS_ALIGN_ANY);
		pFrame->DockControlBar(m_pToolBar);
	}
	OnReToolbar(true);
	pTemplate->SetViewToCreate(RUNTIME_CLASS(COdaDgnTreeView));

	if (pTheLastFrame)
	{
		m_pVectorizer = static_cast<COdaDgnAppVectorizer*>(pTheLastFrame->GetActiveView());
	}

}

void COdaDgnAppDoc::OnFileCompress()
{
  CCompressDatabaseDlg dlg;

  if( dlg.DoModal() == IDOK )
  {
    OdDgCompressDatabaseResult compressResult;
    m_pDb->compressDatabase( dlg.getCompressFlags(), &compressResult );
    compressResult.eraseAll();
    UpdateAllViews(0);
  }
}

// COdaDgnAppDoc commands
void COdaDgnAppDoc::OnVectorize(const OdString& sVectorizerPath)
{
	theApp.settings().setGsDevicePath(sVectorizerPath);

  try
  {
    ::odrxDynamicLinker()->loadModule(sVectorizerPath, false);
  }
  catch(...)
  {
    AfxMessageBox(L"Wrong version of rendering device library.", MB_OK);
    return;
  }

	struct CDocTemplateEx : CMultiDocTemplate
	{
		void SetViewToCreate(CRuntimeClass* pViewClass)
		{
			m_pViewClass = pViewClass;
		}
	};
	CDocTemplateEx* pTemplate = (CDocTemplateEx*)GetDocTemplate();
	ASSERT_VALID(pTemplate);
	pTemplate->SetViewToCreate(RUNTIME_CLASS(COdaDgnAppVectorizer));

	// create vectorizers for all viewable views of the active view group
	// MKU 24\11\2010 #8852 (case: /dgn/Bugzilla/Rendering/Crash/5223/PwayUpsideOld.dgn)
	//  Some files can have invalid id for View Group. Try to get & use a valid (recommended) View Group object.
	OdDgViewGroupPtr pViewGroup = m_pDb->getActiveViewGroupId().openObject();

	m_vectorizedModelId = m_pDb->getActiveModelId();
	if (m_vectorizedModelId.isNull())
	{
		// MKU 24\11\2010 #8852 (case: /dgn/V7_V8legacy/Regression/out/ExLoadToModel_dll/Building/Dgn/BSI300AE9-Atrium.dgn
		//  If Model is is invalid let's try to get & use a Default.
		m_vectorizedModelId = m_pDb->getDefaultModelId();
		m_pDb->setActiveModelId(m_vectorizedModelId);
	}
	if (m_vectorizedModelId.isNull())
	{
		ODA_FAIL_ONCE();
		return;
	}

	if (pViewGroup.isNull())
	{
		pViewGroup = m_pDb->recommendActiveViewGroupId().openObject();
		if (pViewGroup.isNull())
		{
			// Add View group
			OdDgModelPtr pModel = OdDgModel::cast(m_vectorizedModelId.openObject());
			pModel->createViewGroup();
			pModel->fitToView();
			pViewGroup = m_pDb->recommendActiveViewGroupId().openObject();
		}
	}
	if (pViewGroup.isNull())
	{
		ODA_FAIL_ONCE();
		return;
	}

	OdDgElementIteratorPtr pIt = pViewGroup->createIterator();
	CFrameWnd* pTheLastFrame = 0;
	for (; !pIt->done(); pIt->step())
	{

		OdDgViewPtr pView = OdDgView::cast(pIt->item().openObject());
		if (pView.get() && pView->getVisibleFlag())
		{
			m_vectorizedViewId = pIt->item();

			if (!pView->getModelId().isNull())
			{
				m_vectorizedModelId = pView->getModelId();
			}

			CFrameWnd* pFrame = pTemplate->CreateNewFrame(this, NULL);

			pTheLastFrame = pFrame;

			pTemplate->InitialUpdateFrame(pFrame, this);

			COdaDgnAppVectorizer* pVectorizer = static_cast<COdaDgnAppVectorizer*>(pFrame->GetActiveView());

			if (pVectorizer)
			{
				pVectorizer->setPrintSettings(m_printSettings);
			}

			m_pVectorizers.insert(pVectorizer);

			//set its position
			OdGsDCRect  rectangle = pView->getViewRectangle();
			OdGsDCPoint low = rectangle.m_min,
				hi = rectangle.m_max;

			pFrame->MoveWindow(low.x, hi.y, hi.x - low.x, low.y - hi.y);

			//create the toolbar for the frame
			m_pToolBar = new CToolBar();
			m_pToolBar->m_bAutoDelete = TRUE;

			//toolbar->Create( pFrame );
			if (!m_pToolBar->CreateEx(pFrame,
				TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC)
				||
				!m_pToolBar->LoadToolBar(IDR_VECTORIZER))
			{
				TRACE0("Failed to create vectorizer toolbar\n");
				return;      // fail to create
			}

			//toolbar->LoadToolBar( IDR_VECTORIZER );

			m_pToolBar->EnableDocking(CBRS_ALIGN_ANY);
			pFrame->EnableDocking(CBRS_ALIGN_ANY);
			pFrame->DockControlBar(m_pToolBar);
			OnReToolbar(false);
		}

	}
	//OnReToolbar();
	pTemplate->SetViewToCreate(RUNTIME_CLASS(COdaDgnTreeView));

	if (pTheLastFrame)
	{
		m_pVectorizer = static_cast<COdaDgnAppVectorizer*>(pTheLastFrame->GetActiveView());
	}
}

bool COdaDgnAppDoc::hideFence()
{
  bool bRet = false;

  if( !m_idFence.isNull() )
  {
    OdDgElementPtr pFence = m_idFence.openObject(OdDg::kForWrite);

    if( !pFence.isNull() )
    {
      pFence->erase(true);
      bRet = true;
    }
  }

  return bRet;
}

void COdaDgnAppDoc::showFence()
{
  if( !m_idFence.isNull() )
  {
    OdDgElementPtr pFence = m_idFence.openObject(OdDg::kForWrite, true);

    if( !pFence.isNull() )
      pFence->erase(false);
  }
}

void COdaDgnAppDoc::OnRasterAttach()
{
  CString filter = _T("Pictures (*.bmp;*.png;*.jpg)|*.bmp; *.png; *.jpg|");
#if defined(PDFIUM_MODULE_ENABLED)
  filter += _T("Pdf files (*.pdf)|*.pdf||");
#else
  filter += _T("|");
#endif // (PDFIUM_MODULE_ENABLED)
  DWORD dwFlags = OFN_HIDEREADONLY | OFN_EXPLORER | OFN_FILEMUSTEXIST;

  CFileDialog dlgFile(true, 0, 0, dwFlags, filter);
  dlgFile.m_ofn.lpstrDefExt = _T("Select file");
  if (dlgFile.DoModal() == IDOK)
  {
    CString path = dlgFile.GetPathName();
    CString fileName = dlgFile.GetFileName();
    CString spdf = _T(".pdf");
    if (fileName.Find(spdf) == fileName.GetLength() - 4)
    {
      CPdfRasterAttachmentDialog dlg(theApp.GetMainWnd());
      dlg.setData(path, fileName, m_pDb);
      if (dlg.DoModal() == IDOK)
      {
      }
    }
    else
    {
      OdDgModelPtr pModel = m_pDb->getActiveModelId().safeOpenObject(OdDg::kForWrite);

      OdDgRasterAttachmentHeaderPtr raster = OdDgRasterAttachmentHeader::createObject();
      raster->setFilename(OdString(fileName));
      raster->setFilePath(OdString(path));
      pModel->addElement(raster);

      int iDefaultDpi = 96;

      DISPLAY_DEVICE dd;
      dd.cb = sizeof(dd);
      if( EnumDisplayDevices(NULL, 0, &dd, 0) != FALSE)
      {
        HDC hDC = CreateDC(dd.DeviceName, dd.DeviceName, NULL, NULL);
        int iDpiX  = GetDeviceCaps(hDC, LOGPIXELSX);
        int iDpiY  = GetDeviceCaps(hDC, LOGPIXELSY);
        DeleteDC(hDC);
        iDefaultDpi = iDpiX > iDpiY ? iDpiX : iDpiY;
      }

      double dPixelSizeInMeter = 25.4 / iDefaultDpi / 1000.0; // pixel size in meter.

      OdDgModel::UnitDescription masterUnits;
      pModel->getMasterUnit(masterUnits);

      double dPixelSize = dPixelSizeInMeter * masterUnits.m_numerator / masterUnits.m_denominator;

      OdGiRasterImagePtr pImage = raster->getImage();

      if( pImage.isNull() )
        raster->setExtent( OdGePoint2d(100.0 * dPixelSize, 100.0 * dPixelSize) );

      raster->setOrientation(OdGePoint3d::kOrigin - OdGeVector3d(0., 1., 0.) * pImage->pixelHeight() * dPixelSize, OdGeVector3d(1., 0., 0.) * dPixelSize, OdGeVector3d(0., 1., 0.) * dPixelSize);

      OdDgRasterFramePtr pRasterFrame = OdDgRasterFrame::createObject();
      pModel->addElement( pRasterFrame );
      pRasterFrame->setRasterReferenceId( raster->elementId() );

      pModel->fitToView();
    }
  }
}

void COdaDgnAppDoc::recountVeiws(const OdDgViewGroupPtr& pVG)
{
	//m_nActiveView = 0;
	m_pViews.resize(0);

	OdDgViewGroupPtr pViewGroup = pVG;

	if (pViewGroup.isNull())
	{
		pViewGroup = m_pDb->getActiveViewGroupId().openObject();
	}

	if (pViewGroup.isNull())
	{
		return;
	}
	//m_nActiveView = 1;
	OdDgElementIteratorPtr pIt = pViewGroup->createIterator();
	for (; !pIt->done(); pIt->step())
	{
		OdDgViewPtr pView = OdDgView::cast(pIt->item().openObject());
		if (pView.get() && pView->getVisibleFlag())
		{
			m_pViews.push_back(pIt->item());
		}
	}
	//  resetTolbar();
}

#pragma warning(pop)

CMenu* getVectorizeMenu2(CMenu* pMainMenu);


//
// Command and Selection
//
OdEdBaseIO* COdaDgnAppDoc::cmdIO()
{
	return this;
}

OdDgCommandContextPtr COdaDgnAppDoc::cmdCtx()
{
	if (m_pCmdCtx.isNull())
	{
		m_pCmdCtx = ExDgCommandContext::createObject(cmdIO(), m_pDb);
	}
	return m_pCmdCtx;
}

OdDgSelectionSetPtr COdaDgnAppDoc::selectionSet() const
{
	OdDgCommandContext* pCtx = const_cast<COdaDgnAppDoc*>(this)->cmdCtx();
	OdDgSelectionSetPtr pRes = pCtx->arbitraryData(L"OdaDgnApp Working Selection Set");
	if (pRes.isNull())
	{
		pRes = OdDgSelectionSet::createObject(m_pDb);
		pCtx->setArbitraryData(L"OdaDgnApp Working Selection Set", pRes);
	}
	ODA_TRACE1("Working Selection set contains %d items\n", pRes->numEntities());
	return pRes;
}

BOOL COdaDgnAppDoc::CanCloseFrame(CFrameWnd* pFrame)
{
	CView* pView = pFrame->GetActiveView();
	if (pView->IsKindOf(&COdaDgnAppVectorizer::classCOdaDgnAppVectorizer))
	{
		if (!static_cast<COdaDgnAppVectorizer*>(pView)->canClose())
			return FALSE;
	}
	return CDocument::CanCloseFrame(pFrame);
}

OdString commandMessageCaption(const OdString& sCmd)
{
	OdString sCaption;
	sCaption.format(L"Command: %ls", sCmd.c_str());
	return sCaption;
}

class CmdReactor : public OdStaticRxObject<OdEdCommandStackReactor>,
	public OdStaticRxObject<OdDgDatabaseReactor>
{
	ODRX_NO_HEAP_OPERATORS();
	OdDgCommandContext* m_pCmdCtx;
	bool                m_bModified;
	OdString            m_sLastInput;

	void setModified()
	{
		m_bModified = true;
		m_pCmdCtx->database()->removeReactor(this);
	}
public:
	CmdReactor(OdDgCommandContext* pCmdCtx)
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
		if (!m_bModified)
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

	void objectOpenedForModify(const OdDgDatabase*, const OdDgElement*)
	{
		setModified();
	}

	void headerSysVarWillChange(const OdDgDatabase*, const char*)
	{
		setModified();
	}

	OdEdCommandPtr unknownCommand(const OdString& sCmdName, OdEdCommandContext* /*pCmdCtx*/)
	{

		COdaDgnAppVectorizer* pVectorizer = OdDgDatabaseDocPtr(m_pCmdCtx->database())->document()->getVectorizer();
		if (pVectorizer)
		{
			OdEdCommandPtr pRes = pVectorizer->command(sCmdName);
			if (pRes.get())
				return pRes;
		}
		OdString sMsg;
		sMsg.format(L"Unknown command \"%ls\".", sCmdName.c_str());
		m_pCmdCtx->userIO()->putString(sMsg);

		return OdEdCommandPtr();
	}
	void commandWillStart(OdEdCommand* pCmd, OdEdCommandContext* /*pCmdCtx*/)
	{
		m_sLastInput.makeUpper();
		if (!GETBIT(pCmd->flags(), OdEdCommand::kNoHistory))
			theApp.setRecentCmd(m_sLastInput);

		if (!GETBIT(pCmd->flags(), OdEdCommand::kNoUndoMarker))
			m_pCmdCtx->database()->startUndoRecord();
	}
	void commandCancelled(OdEdCommand*, OdEdCommandContext*)
	{
		undoCmd();
	}
	void commandFailed(OdEdCommand*, OdEdCommandContext*)
	{
		undoCmd();
	}
private:
	void undoCmd()
	{
		OdDgDatabase* pDb = m_pCmdCtx->database();
		try
		{
			pDb->disableUndoRecording(true);
			pDb->undo();
			pDb->disableUndoRecording(false);
		}
		catch (const OdError& err)
		{
			theApp.reportError(_T("Can't repair database"), err);
		}
#ifndef _DEBUG
		catch (...)
		{
			::MessageBox(NULL, _T("Unknown error occurred..."),
				_T("Can't repair database"), MB_OK | MB_ICONERROR);
		}
#endif //_DEBUG
	}
};

void COdaDgnAppDoc::DeleteSelection()
{
	if (m_pDb->appServices()->getPICKFIRST() && selectionSet()->numEntities())
	{
		if (AfxMessageBox(IDS_ERASE_REQUEST, MB_YESNO) == IDYES)
		{
			ExecuteCommand(L"ERASE");
		}
	}
}

void COdaDgnAppDoc::ExecuteCommand(const OdString& sCmd, bool bEcho)
{
	OdSaveState<int> save_m_nCmdActive(m_nCmdActive);
	++m_nCmdActive;

	OdDgCommandContextPtr pCmdCtx = cmdCtx();
	CmdReactor cr(pCmdCtx);

	//  m_pDb->startUndoRecord();
	//  m_pDb->startTransaction();

	try
	{
		OdEdCommandStackPtr pCommands = ::odedRegCmds();
		ExDgCommandContext *pExCmdCtx = static_cast<ExDgCommandContext*>(pCmdCtx.get());
		if (m_pDb->appServices()->getPICKFIRST())
			pExCmdCtx->setPickfirst(selectionSet());

		{
			OdString s = sCmd.spanExcluding(L" \t\r\n");
			if (s.getLength() == sCmd.getLength())
			{
				if (bEcho)
					pCmdCtx->userIO()->putString(commandPrompt() + L" " + s);
				s.makeUpper();
				cr.setLastInput(s);
				pCommands->executeCommand(s, pCmdCtx);

        if( s == OD_T("FENCE") )
        {
          OdDgResBufPtr pHandle = pCmdCtx->arbitraryData(OD_T("Fence"));

          if( !m_idFence.isNull() && !pHandle.isNull() )
          {
            OdDgElementPtr pFence = m_idFence.openObject(OdDg::kForWrite);

            if( !pFence.isNull() )
              pFence->erase(true);
          }

          if( !pHandle.isNull() )
          {
            OdInt64 uHandle = pHandle->getInt64();
            m_idFence = m_pDb->getElementId( OdDbHandle((OdUInt64)uHandle));
          }
        }
			}
			else
			{
				m_pMacro = ExStringIO::create(sCmd);
				while (!m_pMacro->isEof())
				{
					try
					{
						s = pCmdCtx->userIO()->getString(commandPrompt());
						s.makeUpper();
						cr.setLastInput(s);
					}
					catch (const OdEdEmptyInput)
					{
						s = recentCmdName();
					}

					pCommands->executeCommand(s, pCmdCtx);

          if( s == OD_T("FENCE") )
          {
            OdDgResBufPtr pHandle = pCmdCtx->arbitraryData(OD_T("Fence"));

            if( !m_idFence.isNull() && !pHandle.isNull() )
            {
              OdDgElementPtr pFence = m_idFence.openObject(OdDg::kForWrite);

              if( !pFence.isNull() )
              {
                pFence->erase(true);
              }
            }

            if( !pHandle.isNull() )
            {
              OdInt64 uHandle = pHandle->getInt64();
              m_idFence = m_pDb->getElementId( OdDbHandle((OdUInt64)uHandle));
            }
          }
				}
			}
		}

		if (getVectorizer())
		{
			getVectorizer()->propagateActiveViewChanges();
		}
	}
	catch (const OdEdEmptyInput)
	{
		//    m_pDb->abortTransaction();
	}
	catch (const OdEdCancel)
	{
		//    m_pDb->abortTransaction();
	}
	catch (const OdError& err)
	{
		if (!m_bConsole)
		{
			theApp.reportError(commandMessageCaption(sCmd), err);
		}
		cmdIO()->putString(err.description());

		//    m_pDb->abortTransaction();
	}

	//  m_pDb->endTransaction();

	if ((cr.isDatabaseModified() || selectionSet()->numEntities()) /*&& 0 != cr.lastInput().iCompare(_T("SELECT"))*/)
	{
		//selectionSet()->clear();
		// Call here OdExEditorObject::unselect() instead sset->clear(), if you want affect changes on grip points and etc.
		OnEditClearselection();
		UpdateAllViews(0);
	}
	//static_cast<OdEdCommandContext*>(pCmdCtx.get())->setMacroIOPresent(false);
}


BOOL COdaDgnAppDoc::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// If pHandlerInfo is NULL, then handle the message
	if (pHandlerInfo == NULL)
	{
		CMenu* pTopMenu = theApp.GetMainWnd()->GetMenu();
		if (pTopMenu)
		{
			// Check if it is a OdaDgnApp's dynamic menu item:
			MENUITEMINFO menuItemInfo;
			menuItemInfo.cbSize = sizeof(menuItemInfo);
			menuItemInfo.fMask = MIIM_DATA;
			if (pTopMenu->GetMenuItemInfo(nID, &menuItemInfo, FALSE)) // command from menu
			{
				if (menuItemInfo.dwItemData == theApp.getGSMenuItemMarker())
				{
					CString s;
					getVectorizeMenu2(pTopMenu)->GetMenuString(nID, s, MF_BYCOMMAND);
					if (nCode == CN_COMMAND)
					{
						OnVectorize((LPCTSTR)s);
					}
					else if (nCode == CN_UPDATE_COMMAND_UI)
					{
						((CCmdUI*)pExtra)->Enable(m_pVectorizers.size() == 0);
						((CCmdUI*)pExtra)->SetCheck(s == (LPCTSTR)theApp.settings().getGsDevicePath());
					}
					return TRUE;
				}

				// Custom commands
				//
				if (nID >= _APS_NEXT_COMMAND_VALUE + 100 && nID <= _APS_NEXT_COMMAND_VALUE + theApp.numCustomCommands() + 100)
				{
					OdRxObjectPtr pData(reinterpret_cast<OdRxObject*>(menuItemInfo.dwItemData));
					if (pData.get()) // dynamic
					{
						if (nCode == CN_COMMAND)
						{
							OdEdCommandPtr pCmd = OdEdCommand::cast(pData);
							if (pCmd.get())
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
				//        else if (nID >= _APS_NEXT_COMMAND_VALUE && nID < _APS_NEXT_COMMAND_VALUE + 100) // scales
				//        {
				//          if (nCode == CN_COMMAND)
				//          {
				//            int nScale = nID - _APS_NEXT_COMMAND_VALUE - 1;
				//            OdDbObjectContextCollectionIteratorPtr ci = m_pDb->objectContextManager()->contextCollection(ODDB_ANNOTATIONSCALES_COLLECTION)->newIterator();
				//            for (int i = 0; !ci->done(); ci->next())
				//            {
				//              if (i++ == nScale)
				//              {
				//                m_pDb->setCANNOSCALE(OdDbAnnotationScalePtr(ci->getContext()));
				//                menuItemInfo.fMask = MIIM_STATE;
				//                menuItemInfo.fState = MFS_CHECKED;
				//#if (WINVER >= 0x0500)
				//                pTopMenu->SetMenuItemInfo(nID, &menuItemInfo, FALSE);
				//#else
				//                ::InsertMenuItem(pTopMenu->GetSafeHmenu(), nID, FALSE, &menuItemInfo);
				//#endif
				//                ExecuteCommand(L"REGEN");
				//                UpdateAllViews(0);
				//              }
				//              else
				//              {
				//                menuItemInfo.fMask = MIIM_STATE;
				//                menuItemInfo.fState = MFS_UNCHECKED;
				//#if (WINVER >= 0x0500)
				//                pTopMenu->SetMenuItemInfo(_APS_NEXT_COMMAND_VALUE + i, &menuItemInfo, FALSE);
				//#else
				//                ::InsertMenuItem(pTopMenu->GetSafeHmenu(), _APS_NEXT_COMMAND_VALUE + i, FALSE, &menuItemInfo);
				//#endif
				//              }
				//            }
				//          }
				//          else if (nCode == CN_UPDATE_COMMAND_UI)
				//          {
				//            ((CCmdUI*)pExtra)->Enable(TRUE);
				//          }
				//          return TRUE;
				//        }

			}
		}
	}

	// If we didn't process the command, call the base class
	// version of OnCmdMsg so the message-map can handle the message
	return CDocument::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void COdaDgnAppDoc::OnCloseVectorizer(COdaDgnAppVectorizer* pVectorizer)
{
	std::set< COdaDgnAppVectorizer* >::iterator it = m_pVectorizers.find(pVectorizer);

	ODA_ASSERT(it != m_pVectorizers.end());
	m_pVectorizers.erase(it);

	if (m_pVectorizers.size())
	{
		m_pVectorizer = *m_pVectorizers.begin();
	}
	else
	{
		m_pVectorizer = 0;
	}
}

void COdaDgnAppDoc::UpdateVectorizersColorPalette(const COLORREF* pColorPaletteRef)
{
	COdaDgnAppVectorizer* pVectorizer;

	for (std::set< COdaDgnAppVectorizer* >::iterator i = m_pVectorizers.begin(); i != m_pVectorizers.end(); i++)
	{
		pVectorizer = *i;
		pVectorizer->UpdateVectorizersColorPalette(pColorPaletteRef);
	}
}

void COdaDgnAppDoc::setVectorizer(COdaDgnAppVectorizer* pVectorizer)
{
	ODA_ASSERT(pVectorizer != 0);
	m_pVectorizer = pVectorizer;
}

void COdaDgnAppDoc::OnExportStl()
{
	CStlExportDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		OdDgDatabase* pDb = database();

		STLModulePtr module = odrxDynamicLinker()->loadModule(OdSTLExportModuleName);

		OdString fileName = dlg.m_Path;

		OdDbHandle han((LPCTSTR)dlg.m_Handle);

		// Requesting ObjectId
		OdDgElementId targetId = pDb->getElementId(han);

		if (targetId.isNull())
		{
			AfxMessageBox(_T("Error : Element not found."), MB_ICONERROR);
		}
		else
		{
			OdRxObjectPtr pObj = targetId.safeOpenObject(OdDg::kForRead);
			OdGiDrawablePtr pDr = OdGiDrawable::cast(pObj);
			if (pDr.isNull())
			{
				AfxMessageBox(_T("Error : Not drawable element."), MB_ICONERROR);
			}
			else
			{
				OdStreamBufPtr pFile;
				try
				{
					pFile = odrxSystemServices()->createFile(fileName, Oda::kFileWrite, Oda::kShareDenyNo, Oda::kCreateAlways);
				}
				catch (OdError*)
				{
					AfxMessageBox(_T("Error : Can't create the file."), MB_ICONERROR);
					return;
				}

				OdGeExtents3d extents;
				pDr->getGeomExtents(extents);
				double deviation = extents.minPoint().distanceTo(extents.maxPoint()) / 100.;

				OdResult res = module->exportSTL(pDb, *pDr, *pFile, dlg.m_Binary == FALSE, deviation);
				if (res != eOk)
				{
					CString tmp = _T("Export error : ");
					tmp += CString(OdError(res).description().c_str());
					AfxMessageBox(tmp, MB_ICONERROR);
				}
			}
		}
	}
}

//void COdaDgnAppDoc::OnExportSvg()
//{
//  OdGsModulePtr pModule = ::odrxDynamicLinker()->loadModule(OdSvgExportModuleName);
//  if (pModule.isNull())
//    AfxMessageBox( OdSvgExportModuleName L" is missing" );
//  else
//  {
//    // svgout command parameters explanation see in \Exports\SvgExport\Source\SvgExport.cpp, SvgExportCommand::execute() 
//    //if (m_pDb->appServices()->getFILEDIA())
//      ExecuteCommand(L"svgout 1 6 \n\n.png sans-serif 768 1024 Yes Yes\n");
//    //else
//    //  ExecuteCommand(L"svgout ~\n1 6 \n\n.png sans-serif 768 1024 Yes Yes\n");
//  }
//}

#include "RxVariantValue.h"
#include "DgGsManager.h"
#include "DbBaseDatabase.h"
#include "ColorMapping.h"
void COdaDgnAppDoc::OnExportSvg()
{
	OdDgDatabasePtr db = database();

	//  OdDgViewGroupTablePtr table = db->getViewGroupTable();
	//  OdDgElementIteratorPtr it = table->createIterator();
	//  for( ; !it->done(); it->step() )
	//  {
	//    OdDgViewGroupPtr viewGroup = OdDgViewGroup::cast( it->item().openObject( OdDg::kForWrite ) );
	//    if ( viewGroup.get() )
	//    {
	//      OdDgElementIteratorPtr it1 = viewGroup->createIterator();
	//      for( ; !it1->done(); it1->step() )
	//      {
	//        OdDgViewPtr view = OdDgView::cast( it1->item().openObject( OdDg::kForWrite ) );
	//        if ( view.get() )
	//        {
	//          bool flag = view->getVisibleFlag();
	//          if (flag)

	{
		// export to svg

		CFileDialog dlgFile(FALSE, _T("svg"), 0, OFN_HIDEREADONLY | OFN_EXPLORER | OFN_PATHMUSTEXIST,
			_T("Scalable Vector Graphics (*.svg)|*.svg||"), AfxGetMainWnd());
		if (dlgFile.DoModal() != IDOK) return;

		OdGsModulePtr pModule = ::odrxDynamicLinker()->loadModule(OdSvgExportModuleName);
		if (pModule.isNull())
			AfxMessageBox(_T("TD_SvgExport.tx is missing"));
		else
		{
			// Create output file
			OdString fileName = dlgFile.GetPathName();
			OdStreamBufPtr file = odrxSystemServices()->createFile(fileName, Oda::kFileWrite, Oda::kShareDenyNo, Oda::kCreateAlways);
			if (!file.isNull())
			{
				// Create export device
				OdGsDevicePtr dev = pModule->createDevice();
				//#else
				//        OdGsDevicePtr dev = createSvgDevice()->create();
				//  OdGsDevicePtr pRes = OdRxObjectImpl<Od2dExportDevice, OdGsDevice>::createObject();
				//#endif

				{
					// Some actions to correct invalid settings as view group and so on before exporting
					OdDgDatabase* pDb = db;
					OdDgViewGroupPtr pViewGroup = pDb->getActiveViewGroupId().openObject();

					OdDgElementId vectorizedModelId = pDb->getActiveModelId();
					if (vectorizedModelId.isNull())
					{
						vectorizedModelId = pDb->getDefaultModelId();
						pDb->setActiveModelId(vectorizedModelId);
					}
					if (vectorizedModelId.isNull())
					{
						ODA_FAIL_ONCE();
						return;
					}

					if (pViewGroup.isNull())
					{
						pViewGroup = pDb->recommendActiveViewGroupId().openObject();
						if (pViewGroup.isNull())
						{
							// Add View group
							OdDgModelPtr pModel = OdDgModel::cast(vectorizedModelId.openObject());
							pModel->createViewGroup();
							pModel->fitToView();
							pViewGroup = pDb->recommendActiveViewGroupId().openObject();
						}
					}
					if (!pViewGroup.isNull())
					{
						pDb->setActiveViewGroupId(pViewGroup->elementId());
					}
					else
					{
						ODA_FAIL_ONCE();
						return;
					}
				}
				if (!dev.isNull())
				{
					// Setup output stream
					dev->properties()->putAt("Output", file.get());
					// size of pixel in device units
					dev->properties()->putAt("LineWeightScale", OdRxVariantValue(1.));
					// size of pixel in device units
					dev->properties()->putAt("Precision", OdRxVariantValue(9l));
					// where copy images
					CString base = dlgFile.GetPathName();
					base = base.Left(base.GetLength() - dlgFile.GetFileName().GetLength());
					dev->properties()->putAt("ImageBase", OdRxVariantValue(OdString(base)));
					// prefix to prepend to image name
					dev->properties()->putAt("ImageUrl", OdRxVariantValue(OdString("./")));
					// default image format
					dev->properties()->putAt("DefaultImageExt", OdRxVariantValue(OdString(".png")));
					OdDgDatabase* pDb = db;
					OdDgModelPtr pModel = pDb->getActiveModelId().safeOpenObject();
					dev->properties()->putAt("BackGround", OdRxVariantValue(OdInt32(pModel->getBackground())));
					//dev->properties()->putAt( "BackGround", OdRxVariantValue(OdInt32(ODRGB(255, 255, 255))));
					dev->properties()->putAt("ExplodeShxTexts", OdRxVariantValue(true));
					// size of pixel in device units

					const ODCOLORREF* refColors = OdDgColorTable::currentPalette(pDb);

					ODGSPALETTE pPalCpy;
					pPalCpy.insert(pPalCpy.begin(), refColors, refColors + 256);
					COLORREF background = pModel->getBackground();
					pPalCpy[255] = background;
					//pPalCpy[255] = ODRGB(255, 255, 255);
					bool bCorrected = OdDgColorTable::correctPaletteForWhiteBackground(pPalCpy.asArrayPtr());

					// Set active palette
					dev->setLogicalPalette(pPalCpy.asArrayPtr(), 256);
					dev->setBackgroundColor(pPalCpy[255]);
					// Prepare database context for device
					OdGiContextForDgDatabasePtr pDgnContext = OdGiContextForDgDatabaseToExport::createObject();
					pDgnContext->setDatabase(database());
					// do not render paper
					pDgnContext->setPlotGeneration(true);
					// Prepare the device to render the active layout in this database.
					OdDbBaseDatabasePEPtr pDbPE = database();
					OdGsDevicePtr wrapper = pDbPE->setupActiveLayoutViews(dev, pDgnContext);
					// Setup device coordinate space
					wrapper->onSize(OdGsDCRect(0, 1024, 768, 0));
					// Initiate rendering.
					wrapper->update(0);
				}
				else
				{
					ODA_FAIL();
				}
			}
		}
	}

	//  }
	//      }
	//      view->setVisibleFlag(false);
	//    }
	//  }
	//    }
	//  }
}

void COdaDgnAppDoc::OnExportHoops()
{
	CFileDialog dlgFile(FALSE, _T("hoops"), 0,
		OFN_HIDEREADONLY | OFN_EXPLORER | OFN_PATHMUSTEXIST,
		_T("Scalable Vector Graphics (*.hmf)|*.hmf|Hoops Stream File (*.hsf)|*.hsf||"), AfxGetMainWnd());

	if (dlgFile.DoModal() != IDOK) return;

	OdString fileName = dlgFile.GetPathName();

#ifdef ENABLE_HOOPS_EXPORT
	CMainFrame* pMFrame = (CMainFrame*)theApp.GetMainWnd();
	//pMFrame->StartTimer();
	HoopsExportParams hoopsParams;
	hoopsParams.strFileName = fileName;
	hoopsParams.pDb = database();

	//pMFrame->StopTimer(_T("Hoops export"));
	OdUInt32 errCode = exportHoops(hoopsParams);

	if (errCode != 0)
	{
		ODA_FAIL();
	}
#endif
}

void COdaDgnAppDoc::OnExportPdf()
{
#ifdef ENABLE_PDF_EXPORT

	OdPdfExportModulePtr pModule = ::odrxDynamicLinker()->loadApp(OdPdfExportModuleName);
	if (pModule.isNull())
		AfxMessageBox(OdPdfExportModuleName L"." DEFAULT_MODULE_EXTENSION_W L" is missing");
	else
	{
		OdPdfExportPtr exporter = pModule->create();

		CExport2PDFDialog dlgPdf;

		dlgPdf.m_dPaperH = m_printSettings.m_dPaperHeight;
		dlgPdf.m_dPaperW = m_printSettings.m_dPaperWidth;
		dlgPdf.m_dLeftMargin = m_printSettings.m_dLeftMargin;
		dlgPdf.m_dRightMargin = m_printSettings.m_dRightMargin;
		dlgPdf.m_dTopMargin = m_printSettings.m_dTopMargin;
		dlgPdf.m_dBottomMargin = m_printSettings.m_dBottomMargin;
    dlgPdf.m_idFence = m_idFence;

		dlgPdf.m_pDb = m_pDb;

		dlgPdf.DoModal();
		return;

		PDFExportParams params;
		dlgPdf.m_dPaperH = m_printSettings.m_dPaperHeight;
		dlgPdf.m_dPaperW = m_printSettings.m_dPaperWidth;
		dlgPdf.m_dLeftMargin = m_printSettings.m_dLeftMargin;
		dlgPdf.m_dRightMargin = m_printSettings.m_dRightMargin;
		dlgPdf.m_dTopMargin = m_printSettings.m_dTopMargin;
		dlgPdf.m_dBottomMargin = m_printSettings.m_dBottomMargin;

		if (dlgPdf.DoModal() == IDOK)
		{
			CMainFrame* pMFrame = (CMainFrame*)theApp.GetMainWnd();
			pMFrame->StartTimer();

			if (!dlgPdf.m_bEnableLayers)
				dlgPdf.m_bExportOffLayers = false;

			bool bV15 = dlgPdf.m_bEnableLayers || dlgPdf.m_bExportOffLayers;

			params.setDatabase(m_pDb);
			params.setVersion(bV15 ? PDFExportParams::kPDFv1_5 : PDFExportParams::kPDFv1_4);

			const ODCOLORREF* refColors = OdDgColorTable::currentPalette(m_pDb);
			ODGSPALETTE pPalCpy;
			pPalCpy.insert(pPalCpy.begin(), refColors, refColors + 256);
			// Color with #255 always defines background.
	  //???      if (dlgPdf.m_bMono)
			{
				// monochrome
				pPalCpy.setAll(ODRGB(0, 0, 0));
			}
			pPalCpy[255] = ODRGB(255, 255, 255);
			// This method should be called to resolve "white background issue" before setting device palette
			bool bCorrected = OdDgColorTable::correctPaletteForWhiteBackground(pPalCpy.asArrayPtr());

			params.setPalette(pPalCpy.asArrayPtr());

			try
			{
				params.setOutput(odrxSystemServices()->createFile(OdString(dlgPdf.m_FileName), Oda::kFileWrite, Oda::kShareDenyNo, Oda::kCreateAlways));
			}
			catch (OdError*)
			{
				AfxMessageBox(_T("Error : Can't create the file."), MB_ICONERROR);
				return;
			}

			params.setExportFlags(PDFExportParams::PDFExportFlags(
				(dlgPdf.m_bEmbedded ? PDFExportParams::kEmbededTTF : 0) |
				(dlgPdf.m_bSHXAsGeometry ? PDFExportParams::kSHXTextAsGeometry : 0) |
				(dlgPdf.m_bTTFAsGeometry ? PDFExportParams::kTTFTextAsGeometry : 0) |
				(dlgPdf.m_bSimpleGeomOpt ? PDFExportParams::kSimpleGeomOptimization : 0) |
				(dlgPdf.m_bZoomToExtents ? PDFExportParams::kZoomToExtentsMode : 0) |
				(dlgPdf.m_bEnableLayers ? PDFExportParams::kEnableLayers : 0) |
				(dlgPdf.m_bExportOffLayers ? PDFExportParams::kIncludeOffLayers : 0) |
				(dlgPdf.m_bEncoded ? PDFExportParams::kFlateCompression : 0) |
				(dlgPdf.m_bEncoded ? PDFExportParams::kASCIIHexEncoding : 0)));

			params.setTitle(OdString(dlgPdf.m_Title));
			params.setAuthor(OdString(dlgPdf.m_Autor));
			params.setSubject(OdString(dlgPdf.m_Subject));
			params.setKeywords(OdString(dlgPdf.m_Keywords));
			params.setCreator(OdString(dlgPdf.m_Creator));
			params.setProducer(OdString(dlgPdf.m_Producer));
			//params.setTGSpecific(true);
			//params.bAllViews = /*dlgPdf.m_bUseHLR != 0*/ true;

			m_pDb->setPaperSize(dlgPdf.m_dPaperW, dlgPdf.m_dPaperH);
			m_pDb->setPlotType(OdDg::kView);
			//???      m_pDb->setMargins( dlgPdf.m_dTopMargin, dlgPdf.m_dBottomMargin, dlgPdf.m_dLeftMargin, dlgPdf.m_dRightMargin );
			m_pDb->setPlotPaperUnits(OdDg::kMillimeters);

			if (dlgPdf.m_Layouts == 1) // all
			{
				OdDgModelTablePtr pModelTable = m_pDb->getModelTable();
				if (!pModelTable.isNull())
				{
					OdDgElementIteratorPtr pIter = pModelTable->createIterator();
					for (; !pIter->done(); pIter->step())
					{
						OdDgModelPtr pModel = OdDgModel::cast(pIter->item().openObject());
						if (pModel.get())
						{
							params.layouts().push_back(pModel->getName());
						}
					}
				}
			}

			OdUInt32 nPages = odmax(1, params.layouts().size());

			OdGsPageParams pageParams;
			//???      pageParams.set(dlgPdf.m_dPaperW, dlgPdf.m_dPaperH, dlgPdf.m_dLeftMargin, dlgPdf.m_dRightMargin, dlgPdf.m_dTopMargin, dlgPdf.m_dBottomMargin);
			params.pageParams().resize(nPages, pageParams);

			//// sample
			if (!dlgPdf.m_sPlotStyleTable.IsEmpty())
			{
				//OdString strDgnPlotFilename("d:\\Sample\\dgn\\mono.tbl");
				//OdString strDgnPlotFilename("D:\\Oda\\DataFiles\\tests\\TGRenderingRegressionTestSuite\\12734\\ColorBasedPlotStyles.tbl");
				OdString strDgnPlotFilename(dlgPdf.m_sPlotStyleTable);
				m_pDb->setCurrentPenTableFile(strDgnPlotFilename);
				//        OdGiContextForDgDatabasePtr pDgnContext = OdGiContextForDgDatabaseToExport::createObject();
				//        pDgnContext->setDatabase(m_pDb);
						//pDgnContext->enableGsModel( true );
				//        pDgnContext->setPlotGeneration( true );
				//        if (pDgnContext->loadPlotStyle(strDgnPlotFilename))
				//        {
				//          params.giContext = pDgnContext;
				//        }
			}

			OdUInt32 errCode = exporter->exportPdf(params);

			pMFrame->StopTimer(_T("PDF export"));

			if (errCode != 0)
			{
				OdString errMes = exporter->exportPdfErrorCode(errCode);
				CString str;
				str.Format(_T("Error number : 0x%x. \n %ls"), errCode, errMes.c_str());

				if (errCode == 0x10008)
				{
					str += _T("\nPlease enable Zoom to extents check box or\ndefine page parameters for layout in page setup dialog.");
				}

				::MessageBox(NULL, str, _T("PDF error"), MB_OK | MB_ICONERROR);
			}
		}
	}
#endif
}

void COdaDgnAppDoc::OnVectorize()
{
	if (theApp.useSingleViewMode())
	{
		OnSwitchView(theApp.settings().getGsDevicePath(), 1);
		OnReToolbar(true);
	}
	else
	{
		OnVectorize(theApp.settings().getGsDevicePath());
	}
}

void COdaDgnAppDoc::OnUpdateVectorize(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(!m_pVectorizers.size() && !theApp.settings().getGsDevicePath().isEmpty());
}

afx_msg void COdaDgnAppDoc::OnUpdateSwitchModel(CCmdUI*pCmdUI)
{
	//actually, MFC switches this button enabled/disabled if ON_UPDATE_COMMAND_UI created for the ID;
	//so, I have nothing to write inside this method
}

afx_msg void COdaDgnAppDoc::OnUpdateItemTypeLibs(CCmdUI*pCmdUI)
{
  pCmdUI->Enable( !m_pDb.isNull() );
}

afx_msg void COdaDgnAppDoc::OnTestsViewcamera()
{
	CCameraViewDlg dlg(NULL, m_pDb);
	dlg.DoModal();
}

void COdaDgnAppDoc::createCellTable2d(OdDgModelPtr& pModel,
	OdDgDatabasePtr pDbLib,
	OdArray<OdDgElementId> arrModelIds,
	bool bSharedCell,
	OdUInt32 uModelCount
)
{
	OdUInt32 nCols = 4;
	OdUInt32 nRows = (OdUInt32)(ceil(uModelCount / (nCols * 1.0)));
	double   dHeaderHeight = 5.0;
	double   dSpacing = 2.0;
	double   dCellSide = 10.0;
	double   dCellSideFactor = 0.95;
	double   dCaptionHeight = 1.0;
	double   dHeaderTextHeight = 1.0;
	double   dCaptionTextHeight = 0.25;

	OdDgLineString2dPtr pLineString = OdDgLineString2d::createObject();

	pLineString->addVertex(OdGePoint2d(0, 0));
	pLineString->addVertex(OdGePoint2d(0, nRows*(dCellSide + dCaptionHeight + dSpacing) + dSpacing + dHeaderHeight));
	pLineString->addVertex(OdGePoint2d(nCols * (dCellSide + dSpacing) + dSpacing, nRows*(dCellSide + dCaptionHeight + dSpacing) + dSpacing + dHeaderHeight));
	pLineString->addVertex(OdGePoint2d(nCols * (dCellSide + dSpacing) + dSpacing, 0));
	pLineString->addVertex(OdGePoint2d(0, 0));

	pModel->addElement(pLineString);

	OdDgLine2dPtr pLine2d = OdDgLine2d::createObject();

	pLine2d->setStartPoint(OdGePoint2d(0, nRows*(dCellSide + dCaptionHeight + dSpacing) + dSpacing));
	pLine2d->setEndPoint(OdGePoint2d(nCols * (dCellSide + dSpacing) + dSpacing, nRows*(dCellSide + dCaptionHeight + dSpacing) + dSpacing));

	pModel->addElement(pLine2d);

	OdDgText2dPtr pText2d = OdDgText2d::createObject();

	pText2d->setHeightMultiplier(dHeaderTextHeight);
	pText2d->setLengthMultiplier(dHeaderTextHeight);
	pText2d->setText(pModel->getName());

	pModel->addElement(pText2d);

	OdGeExtents3d extText2d;
	pText2d->getGeomExtents(extText2d);

	OdGePoint2d ptTextPosition;
	ptTextPosition.x = (nCols * (dCellSide + dSpacing) + dSpacing - extText2d.maxPoint().x + extText2d.minPoint().x) / 2.0;
	ptTextPosition.y = nRows*(dCellSide + dCaptionHeight + dSpacing) + dSpacing + (dHeaderHeight - dHeaderTextHeight) / 2.0;

	pText2d->setOrigin(ptTextPosition);

	for (OdUInt32 i = 0; i < nRows; i++)
	{
		for (OdUInt32 j = 0; j < nCols; j++)
		{
			pLineString = OdDgLineString2d::createObject();

			pLineString->addVertex(OdGePoint2d(dSpacing + j * (dCellSide + dSpacing), (nRows - i)*(dCellSide + dCaptionHeight + dSpacing)));
			pLineString->addVertex(OdGePoint2d(dSpacing + j * (dCellSide + dSpacing), (nRows - i)*(dCellSide + dCaptionHeight + dSpacing) - dCellSide - dCaptionHeight));
			pLineString->addVertex(OdGePoint2d(dSpacing + j * (dCellSide + dSpacing) + dCellSide, (nRows - i)*(dCellSide + dCaptionHeight + dSpacing) - dCellSide - dCaptionHeight));
			pLineString->addVertex(OdGePoint2d(dSpacing + j * (dCellSide + dSpacing) + dCellSide, (nRows - i)*(dCellSide + dCaptionHeight + dSpacing)));
			pLineString->addVertex(OdGePoint2d(dSpacing + j * (dCellSide + dSpacing), (nRows - i)*(dCellSide + dCaptionHeight + dSpacing)));

			pModel->addElement(pLineString);

			pLine2d = OdDgLine2d::createObject();

			pLine2d->setStartPoint(OdGePoint2d(dSpacing + j * (dCellSide + dSpacing), (nRows - i)*(dCellSide + dCaptionHeight + dSpacing) - dCaptionHeight));
			pLine2d->setEndPoint(OdGePoint2d(dSpacing + j * (dCellSide + dSpacing) + dCellSide, (nRows - i)*(dCellSide + dCaptionHeight + dSpacing) - dCaptionHeight));

			pModel->addElement(pLine2d);
		}
	}

	OdUInt32 uCurCol = 0;
	OdUInt32 uCurRow = 0;

	for (OdUInt32 i = 0; i < arrModelIds.size(); i++)
	{
		OdDgModelPtr pModelLib = arrModelIds[i].openObject(OdDg::kForRead);

		pText2d = OdDgText2d::createObject();

		pText2d->setHeightMultiplier(dCaptionTextHeight);
		pText2d->setLengthMultiplier(dCaptionTextHeight);
		pText2d->setText(pModelLib->getName());

		pModel->addElement(pText2d);

		pText2d->getGeomExtents(extText2d);

		ptTextPosition.x = (uCurCol * (dCellSide + dSpacing) + dSpacing) + (dCellSide - extText2d.maxPoint().x + extText2d.minPoint().x) / 2.0;
		ptTextPosition.y = (nRows - uCurRow)*(dCellSide + dCaptionHeight + dSpacing) - dCaptionHeight + (dCaptionHeight - dCaptionTextHeight) / 2.0;

		pText2d->setOrigin(ptTextPosition);

		if (bSharedCell)
		{
			OdDgSharedCellDefinitionPtr pSharedCellDef = OdDgSharedCellDefinition::createObject();

			OdDgSharedCellDefinitionTablePtr pDefTable = pModel->database()->getSharedCellDefinitionTable(OdDg::kForWrite);

			pDefTable->add(pSharedCellDef);

			pSharedCellDef->setName(pModelLib->getName() + L"_Internal");
			pSharedCellDef->setOrigin(OdGePoint3d(0, 0, 0));
			pSharedCellDef->loadFromLibrary(pModelLib);

			OdGeExtents3d extCellDef;
			pSharedCellDef->getGeomExtents(extCellDef);

			double dCellWidth = extCellDef.maxPoint().x - extCellDef.minPoint().x;
			double dCellHeight = extCellDef.maxPoint().y - extCellDef.minPoint().y;

			double dCellMax = (dCellWidth > dCellHeight) ? dCellWidth : dCellHeight;

			double dCellScale = dCellSide * dCellSideFactor / dCellMax;

			OdGeMatrix3d matScale;
			matScale.setToScaling(dCellScale);

			dCellWidth *= dCellScale;
			dCellHeight *= dCellScale;

			dCellMax *= dCellScale;

			OdGePoint3d ptCellPosition;

			ptCellPosition.x = dSpacing + uCurCol * (dCellSide + dSpacing) + ((1 - dCellSideFactor)* dCellSide) / 2.0 + (dCellMax - dCellWidth) / 2.0;
			ptCellPosition.y = dSpacing + (nRows - uCurRow - 1)*(dCellSide + dCaptionHeight + dSpacing) + ((1 - dCellSideFactor)* dCellSide) / 2.0 + (dCellMax - dCellHeight) / 2.0;
			ptCellPosition.z = 0;

			OdGeVector3d vrOffset = ptCellPosition - OdGePoint3d(extCellDef.minPoint().x, extCellDef.minPoint().y, 0)  * dCellScale;

			OdGeMatrix3d matTranslate;
			matTranslate.setToTranslation(vrOffset);

			OdGeMatrix3d matTransform = matTranslate * matScale;

			OdDgSharedCellReferencePtr pSharedCellRef = OdDgSharedCellReference::createObject();

			pModel->addElement(pSharedCellRef);

			pSharedCellRef->setDefinitionName(pSharedCellDef->getName());
			pSharedCellRef->setOrigin(OdGePoint3d(0, 0, 0));
			pSharedCellRef->setTransformation(OdGeMatrix3d::kIdentity);
			pSharedCellRef->transformBy(matTransform);
		}
		else
		{
			OdDgCellHeader2dPtr pCell2d = OdDgCellHeader2d::createObject();
			pModel->addElement(pCell2d);

			pCell2d->setName(pModelLib->getName());
			pCell2d->setOrigin(OdGePoint2d(0, 0));
			pCell2d->loadFromLibrary(pModelLib);

			OdGeExtents3d extCell2d;
			pCell2d->getGeomExtents(extCell2d);

			double dCellWidth = extCell2d.maxPoint().x - extCell2d.minPoint().x;
			double dCellHeight = extCell2d.maxPoint().y - extCell2d.minPoint().y;

			double dCellMax = (dCellWidth > dCellHeight) ? dCellWidth : dCellHeight;

			double dCellScale = dCellSide * dCellSideFactor / dCellMax;

			OdGeMatrix3d matScale;
			matScale.setToScaling(dCellScale);

			pCell2d->transformBy(matScale);
			extCell2d.transformBy(matScale);

			dCellWidth *= dCellScale;
			dCellHeight *= dCellScale;

			dCellMax *= dCellScale;

			OdGePoint3d ptCellPosition;

			ptCellPosition.x = dSpacing + uCurCol * (dCellSide + dSpacing) + ((1 - dCellSideFactor)* dCellSide) / 2.0 + (dCellMax - dCellWidth) / 2.0;
			ptCellPosition.y = dSpacing + (nRows - uCurRow - 1)*(dCellSide + dCaptionHeight + dSpacing) + ((1 - dCellSideFactor)* dCellSide) / 2.0 + (dCellMax - dCellHeight) / 2.0;
			ptCellPosition.z = 0;

			OdGeVector3d vrOffset = ptCellPosition - extCell2d.minPoint();

			OdGeMatrix3d matTranslate;
			matTranslate.setToTranslation(vrOffset);

			pCell2d->transformBy(matTranslate);
		}

		uCurCol++;

		if (uCurCol >= nCols)
		{
			uCurCol = 0;
			uCurRow++;
		}
	}
}

void COdaDgnAppDoc::createCellTable3d(OdDgModelPtr& pModel,
	OdDgDatabasePtr pDbLib,
	OdArray<OdDgElementId> arrModelIds,
	bool bSharedCell,
	OdUInt32 uModelCount
)
{
	OdUInt32 nCols = 4;
	OdUInt32 nRows = (OdUInt32)(ceil(uModelCount / (nCols * 1.0)));
	double   dHeaderHeight = 5.0;
	double   dSpacing = 2.0;
	double   dCellSide = 10.0;
	double   dCellSideFactor = 0.95;
	double   dCaptionHeight = 1.0;
	double   dHeaderTextHeight = 1.0;
	double   dCaptionTextHeight = 0.25;

	OdDgLineString3dPtr pLineString = OdDgLineString3d::createObject();

	pLineString->addVertex(OdGePoint3d(0, 0, 0));
	pLineString->addVertex(OdGePoint3d(0, nRows*(dCellSide + dCaptionHeight + dSpacing) + dSpacing + dHeaderHeight, 0));
	pLineString->addVertex(OdGePoint3d(nCols * (dCellSide + dSpacing) + dSpacing, nRows*(dCellSide + dCaptionHeight + dSpacing) + dSpacing + dHeaderHeight, 0));
	pLineString->addVertex(OdGePoint3d(nCols * (dCellSide + dSpacing) + dSpacing, 0, 0));
	pLineString->addVertex(OdGePoint3d(0, 0, 0));

	pModel->addElement(pLineString);

	OdDgLine3dPtr pLine3d = OdDgLine3d::createObject();

	pLine3d->setStartPoint(OdGePoint3d(0, nRows*(dCellSide + dCaptionHeight + dSpacing) + dSpacing, 0));
	pLine3d->setEndPoint(OdGePoint3d(nCols * (dCellSide + dSpacing) + dSpacing, nRows*(dCellSide + dCaptionHeight + dSpacing) + dSpacing, 0));

	pModel->addElement(pLine3d);

	OdDgText3dPtr pText3d = OdDgText3d::createObject();

	pText3d->setHeightMultiplier(dHeaderTextHeight);
	pText3d->setLengthMultiplier(dHeaderTextHeight);
	pText3d->setText(pModel->getName());

	pModel->addElement(pText3d);

	OdGeExtents3d extText3d;
	pText3d->getGeomExtents(extText3d);

	OdGePoint3d ptTextPosition;
	ptTextPosition.x = (nCols * (dCellSide + dSpacing) + dSpacing - extText3d.maxPoint().x + extText3d.minPoint().x) / 2.0;
	ptTextPosition.y = nRows*(dCellSide + dCaptionHeight + dSpacing) + dSpacing + (dHeaderHeight - dHeaderTextHeight) / 2.0;
	ptTextPosition.z = 0;

	pText3d->setOrigin(ptTextPosition);

	for (OdUInt32 i = 0; i < nRows; i++)
	{
		for (OdUInt32 j = 0; j < nCols; j++)
		{
			pLineString = OdDgLineString3d::createObject();

			pLineString->addVertex(OdGePoint3d(dSpacing + j * (dCellSide + dSpacing), (nRows - i)*(dCellSide + dCaptionHeight + dSpacing), 0));
			pLineString->addVertex(OdGePoint3d(dSpacing + j * (dCellSide + dSpacing), (nRows - i)*(dCellSide + dCaptionHeight + dSpacing) - dCellSide - dCaptionHeight, 0));
			pLineString->addVertex(OdGePoint3d(dSpacing + j * (dCellSide + dSpacing) + dCellSide, (nRows - i)*(dCellSide + dCaptionHeight + dSpacing) - dCellSide - dCaptionHeight, 0));
			pLineString->addVertex(OdGePoint3d(dSpacing + j * (dCellSide + dSpacing) + dCellSide, (nRows - i)*(dCellSide + dCaptionHeight + dSpacing), 0));
			pLineString->addVertex(OdGePoint3d(dSpacing + j * (dCellSide + dSpacing), (nRows - i)*(dCellSide + dCaptionHeight + dSpacing), 0));

			pModel->addElement(pLineString);

			pLine3d = OdDgLine3d::createObject();

			pLine3d->setStartPoint(OdGePoint3d(dSpacing + j * (dCellSide + dSpacing), (nRows - i)*(dCellSide + dCaptionHeight + dSpacing) - dCaptionHeight, 0));
			pLine3d->setEndPoint(OdGePoint3d(dSpacing + j * (dCellSide + dSpacing) + dCellSide, (nRows - i)*(dCellSide + dCaptionHeight + dSpacing) - dCaptionHeight, 0));

			pModel->addElement(pLine3d);
		}
	}

	OdUInt32 uCurCol = 0;
	OdUInt32 uCurRow = 0;

	for (OdUInt32 i = 0; i < arrModelIds.size(); i++)
	{
		OdDgModelPtr pModelLib = arrModelIds[i].openObject(OdDg::kForRead);

		pText3d = OdDgText3d::createObject();

		pText3d->setHeightMultiplier(dCaptionTextHeight);
		pText3d->setLengthMultiplier(dCaptionTextHeight);
		pText3d->setText(pModelLib->getName());

		pModel->addElement(pText3d);

		pText3d->getGeomExtents(extText3d);

		ptTextPosition.x = (uCurCol * (dCellSide + dSpacing) + dSpacing) + (dCellSide - extText3d.maxPoint().x + extText3d.minPoint().x) / 2.0;
		ptTextPosition.y = (nRows - uCurRow)*(dCellSide + dCaptionHeight + dSpacing) - dCaptionHeight + (dCaptionHeight - dCaptionTextHeight) / 2.0;
		ptTextPosition.z = 0;

		pText3d->setOrigin(ptTextPosition);

		if (bSharedCell)
		{
			OdDgSharedCellDefinitionPtr pSharedCellDef = OdDgSharedCellDefinition::createObject();

			OdDgSharedCellDefinitionTablePtr pDefTable = pModel->database()->getSharedCellDefinitionTable(OdDg::kForWrite);

			pDefTable->add(pSharedCellDef);

			pSharedCellDef->setName(pModelLib->getName() + L"_Internal");
			pSharedCellDef->setOrigin(OdGePoint3d(0, 0, 0));
			pSharedCellDef->loadFromLibrary(pModelLib);

			OdGeExtents3d extCellDef;
			pSharedCellDef->getGeomExtents(extCellDef);

			double dCellWidth = extCellDef.maxPoint().x - extCellDef.minPoint().x;
			double dCellHeight = extCellDef.maxPoint().y - extCellDef.minPoint().y;

			double dCellMax = (dCellWidth > dCellHeight) ? dCellWidth : dCellHeight;

			double dCellScale = dCellSide * dCellSideFactor / dCellMax;

			OdGeMatrix3d matScale;
			matScale.setToScaling(dCellScale);

			dCellWidth *= dCellScale;
			dCellHeight *= dCellScale;

			dCellMax *= dCellScale;

			OdGePoint3d ptCellPosition;

			ptCellPosition.x = dSpacing + uCurCol * (dCellSide + dSpacing) + ((1 - dCellSideFactor)* dCellSide) / 2.0 + (dCellMax - dCellWidth) / 2.0;
			ptCellPosition.y = dSpacing + (nRows - uCurRow - 1)*(dCellSide + dCaptionHeight + dSpacing) + ((1 - dCellSideFactor)* dCellSide) / 2.0 + (dCellMax - dCellHeight) / 2.0;
			ptCellPosition.z = 0;

			OdGeVector3d vrOffset = ptCellPosition - OdGePoint3d(extCellDef.minPoint().x, extCellDef.minPoint().y, 0)  * dCellScale;

			OdGeMatrix3d matTranslate;
			matTranslate.setToTranslation(vrOffset);

			OdGeMatrix3d matTransform = matTranslate * matScale;

			OdDgSharedCellReferencePtr pSharedCellRef = OdDgSharedCellReference::createObject();

			pModel->addElement(pSharedCellRef);

			pSharedCellRef->setDefinitionName(pSharedCellDef->getName());
			pSharedCellRef->setOrigin(OdGePoint3d(0, 0, 0));
			pSharedCellRef->setTransformation(OdGeMatrix3d::kIdentity);
			pSharedCellRef->transformBy(matTransform);
		}
		else
		{
			OdDgCellHeader3dPtr pCell3d = OdDgCellHeader3d::createObject();
			pModel->addElement(pCell3d);

			pCell3d->setName(pModelLib->getName());
			pCell3d->setOrigin(OdGePoint3d(0, 0, 0));
			pCell3d->loadFromLibrary(pModelLib);

			OdGeExtents3d extCell3d;
			pCell3d->getGeomExtents(extCell3d);

			double dCellWidth = extCell3d.maxPoint().x - extCell3d.minPoint().x;
			double dCellHeight = extCell3d.maxPoint().y - extCell3d.minPoint().y;

			double dCellMax = (dCellWidth > dCellHeight) ? dCellWidth : dCellHeight;

			double dCellScale = dCellSide * dCellSideFactor / dCellMax;

			OdGeMatrix3d matScale;
			matScale.setToScaling(dCellScale);

			pCell3d->transformBy(matScale);
			extCell3d.transformBy(matScale);

			dCellWidth *= dCellScale;
			dCellHeight *= dCellScale;

			dCellMax *= dCellScale;

			OdGePoint3d ptCellPosition;

			ptCellPosition.x = dSpacing + uCurCol * (dCellSide + dSpacing) + ((1 - dCellSideFactor)* dCellSide) / 2.0 + (dCellMax - dCellWidth) / 2.0;
			ptCellPosition.y = dSpacing + (nRows - uCurRow - 1)*(dCellSide + dCaptionHeight + dSpacing) + ((1 - dCellSideFactor)* dCellSide) / 2.0 + (dCellMax - dCellHeight) / 2.0;
			ptCellPosition.z = 0;

			OdGeVector3d vrOffset = ptCellPosition - extCell3d.minPoint();

			OdGeMatrix3d matTranslate;
			matTranslate.setToTranslation(vrOffset);

			pCell3d->transformBy(matTranslate);
		}

		uCurCol++;

		if (uCurCol >= nCols)
		{
			uCurCol = 0;
			uCurRow++;
		}
	}
}

bool COdaDgnAppDoc::createCellLibraryPreview(OdDgDatabasePtr pDb,
	const OdString& strFileName,
	bool bModel2d,
	bool bSharedCell
)
{
	bool bRet = true;

	OdDgDatabasePtr pDbLib = theApp.readFile(strFileName);

	OdDgModelTablePtr pModelTableLib = pDbLib->getModelTable(OdDg::kForRead);

	OdUInt32 uModelCount = 0;

	OdArray<OdDgElementId> arrModelIds;

	OdDgElementIteratorPtr pModelLibIter = pModelTableLib->createIterator();

	for (; !pModelLibIter->done(); pModelLibIter->step())
	{
		OdDgModelPtr pModelLib = pModelLibIter->item().openObject(OdDg::kForRead);

		OdDgElementIteratorPtr pGraphIter = pModelLib->createGraphicsElementsIterator();
		OdDgElementIteratorPtr pCtrlIter = pModelLib->createControlElementsIterator();

		if (pGraphIter->done())
		{
			bool bCtrlEmpty = true;

			for (; !pCtrlIter->done(); pCtrlIter->step())
			{
				OdDgElementPtr pElm = pCtrlIter->item().openObject(OdDg::kForRead);

				if (!pElm.isNull() && ((pElm->getElementType() == OdDgElement::kTypeRasterHeader) ||
					(pElm->getElementType() == OdDgElement::kTypeReferenceAttachmentHeader)
					)
					)
				{
					bCtrlEmpty = false;
					break;
				}
			}

			if (bCtrlEmpty)
			{
				continue;
			}
		}

		arrModelIds.push_back(pModelLib->elementId());

		uModelCount++;
	}

	if (uModelCount == 0)
	{
		return false;
	}

	OdDgModelPtr pModel = OdDgModel::createObject();
	pDb->getModelTable(OdDg::kForWrite)->add(pModel);

	pModel->setModelIs3dFlag(!bModel2d);
	pModel->setWorkingUnit(OdDgModel::kWuWorldUnit);

	OdString strFName = pDbLib->getFilename();
	strFName.replace(L'\\', L'/');

	if (strFName.reverseFind(L'/') != -1)
	{
		strFName = strFName.right(strFName.getLength() - strFName.reverseFind(L'/') - 1);
	}

	pModel->setName(strFName);

	if (bModel2d)
	{
		createCellTable2d(pModel, pDbLib, arrModelIds, bSharedCell, uModelCount);
	}
	else
	{
		createCellTable3d(pModel, pDbLib, arrModelIds, bSharedCell, uModelCount);
	}

	OdDgViewGroupTablePtr pViewGroup = pDb->getViewGroupTable(OdDg::kForWrite);

	OdDgElementIteratorPtr pVGIter = pViewGroup->createIterator();

	for (; !pVGIter->done(); pVGIter->step())
	{
		OdDgViewGroupPtr pViewGroup = pVGIter->item().openObject(OdDg::kForWrite);

		if (pViewGroup->getModelId() == pModel->elementId())
		{
			pViewGroup->setName(strFName);

			OdDgElementIteratorPtr pViewIter = pViewGroup->createIterator();

			bool bFirstView = true;

			for (; !pViewIter->done(); pViewIter->step())
			{
				OdDgViewPtr pView = pViewIter->item().openObject(OdDg::kForWrite);

				if (bFirstView)
				{
					pView->setVisibleFlag(true);
					pView->setShowFillsFlag(true);
					bFirstView = false;
				}
				else
				{
					pView->setVisibleFlag(false);
				}
			}
		}
	}

	pModel->fitToView();

	pDb->setActiveModelId(pModel->elementId());

	return bRet;
}

afx_msg void COdaDgnAppDoc::OnTestsMergeXRef()
{
  CXRefMergeDlg dlg(NULL, m_pDb);

  if( dlg.DoModal() == IDOK && !dlg.getXRefId().isNull() )
  {
    OdDgElementId idXRef  = dlg.getXRefId();
    OdDgElementId idModel = m_pDb->getActiveModelId();

    OdDgReferenceAttachmentHeaderPtr pXRef = idXRef.openObject(OdDg::kForWrite);
    OdDgModelPtr pModel = idModel.openObject(OdDg::kForWrite);

    OdDgElementId idView;

    OdDgViewGroupTablePtr pViewGroupTable = m_pDb->getViewGroupTable();

    OdDgElementIteratorPtr pViewGroupIter = pViewGroupTable->createIterator();

    for(; !pViewGroupIter->done(); pViewGroupIter->step())
    {
      OdDgViewGroupPtr pViewGroup = pViewGroupIter->item().openObject(OdDg::kForRead);

      if( pViewGroup->isMultiModelGroup() )
        continue;

      if (pViewGroup->getModelId() == idModel)
      {
        idView = pViewGroup->getAt(0);
        break;
      }
    }

    if( !pModel.isNull() && !pXRef.isNull() )
    {
      m_pDb->startUndoRecord();
      m_pDb->startTransaction();

      if( pModel->bind(pXRef, idView, OdDg::kDrcXrefMangleName, dlg.m_bXRefColorTable, dlg.m_bClipXRef, dlg.m_bBindToCell) )
      {
        pXRef->erase(true);
        pModel->fitToView();
        UpdateAllViews(0);
        m_pDb->endTransaction();
      }
      else
      {
        m_pDb->endTransaction();
        m_pDb->undo();
      }
    }
  }
}

afx_msg void COdaDgnAppDoc::OnTestsMergeModels()
{
  CFileDialog fileDlg(TRUE, _T("Merge active models"), _T(""), OFN_HIDEREADONLY | OFN_EXPLORER | OFN_PATHMUSTEXIST,
    _T("dgn V8 files (*.dgn)|*.dgn|"), NULL);

  if( fileDlg.DoModal() == IDOK )
  {
    OdString strFileName = fileDlg.GetPathName();

    OdDgDatabasePtr pMergeDb = theApp.readFile(strFileName);

    if( !pMergeDb.isNull() && !pMergeDb->getActiveModelId().isNull() && !m_pDb->getActiveModelId().isNull() )
    {
      m_pDb->startUndoRecord();
      m_pDb->startTransaction();

      OdDgModelPtr pActiveModel = m_pDb->getActiveModelId().openObject(OdDg::kForWrite);
      OdDgModelPtr pMergeModel  = pMergeDb->getActiveModelId().openObject(OdDg::kForRead);

      pActiveModel->merge(pMergeModel, OdGeMatrix3d::kIdentity, OdDg::kDrcMangleName);
      pActiveModel->fitToView();
      UpdateAllViews(0);

      m_pDb->endTransaction();
    }
  }
}

afx_msg void COdaDgnAppDoc::OnTestsLoadLineStyles()
{
  CFileDialog fileDlg(TRUE, _T("Line style rsc files"), _T(""), OFN_HIDEREADONLY | OFN_EXPLORER | OFN_PATHMUSTEXIST,
    _T("resource files (*.rsc)|*.rsc|all files |*.*|"), NULL);

  if(fileDlg.DoModal() == IDOK)
  {
    OdString strFileName = fileDlg.GetPathName();
    theApp.loadRscLineStyle(strFileName);
  }
}

afx_msg void COdaDgnAppDoc::OnTestsLoadSmartSolid()
{
	CFileDialog fileDlg(TRUE, _T("sat"), _T(""), OFN_HIDEREADONLY | OFN_EXPLORER | OFN_PATHMUSTEXIST,
		_T("sat files (*.sat)|*.sat|sab files (*.sab)|*.sab|parasolid files (*.x_b)|*.x_b|all files |*.*|"), NULL);

	if (fileDlg.DoModal() == IDOK)
	{
		m_pDb->startUndoRecord();
		m_pDb->startTransaction();

		OdDgModelTablePtr pModelTable = m_pDb->getModelTable(OdDg::kForWrite);

		if (!pModelTable.isNull())
		{
			OdString strFileName = fileDlg.GetPathName();
			OdString strShortName = strFileName;
			strShortName.trimLeft();
			strShortName.trimRight();
			strShortName.replace(L'\\', L'/');

      bool bACIS = false;
      bool bSab  = false;

      if( strShortName.getLength() > 3 )
      {
        if( strShortName.right(3).makeUpper() == L"SAT" )
        {
          bACIS = true;
          bSab  = false;
        }
        else if( strShortName.right(3).makeUpper() == L"SAB" )
        {
          bACIS = true;
          bSab  = true;
        }
      }

      OdRxModulePtr pModelerModule;
      OdRxClassPtr  pCreatorService;

      if( bACIS )
      {
        pModelerModule = ::odrxDynamicLinker()->loadModule("TG_ModelerGeometry");
        pCreatorService = OdRxClassPtr( odrxServiceDictionary()->getAt(OD_T("OdDgModelerGeometryCreator")) );

        if(pModelerModule.isNull() || pCreatorService.isNull() )
          return;
      }

			if (strShortName.find(L'/') != -1)
				strShortName = strShortName.right(strShortName.getLength() - strShortName.reverseFind(L'/') - 1);

			OdDgModelPtr pModel = OdDgModel::createObject();
			pModel->setName(strShortName);
			pModel->setWorkingUnit(OdDgModel::kWuWorldUnit);
			pModelTable->add(pModel);

			OdDgLine3dPtr pXAxis = OdDgLine3d::createObject();
			pXAxis->setColorIndex(1);
			pXAxis->setStartPoint(OdGePoint3d(0.0, 0.0, 0.0));
			pXAxis->setEndPoint(OdGePoint3d(1.0, 0.0, 0.0));
			pModel->addElement(pXAxis);

			OdDgLine3dPtr pYAxis = OdDgLine3d::createObject();
			pYAxis->setColorIndex(2);
			pYAxis->setStartPoint(OdGePoint3d(0.0, 0.0, 0.0));
			pYAxis->setEndPoint(OdGePoint3d(0.0, 1.0, 0.0));
			pModel->addElement(pYAxis);

			OdDgLine3dPtr pZAxis = OdDgLine3d::createObject();
			pZAxis->setColorIndex(3);
			pZAxis->setStartPoint(OdGePoint3d(0.0, 0.0, 0.0));
			pZAxis->setEndPoint(OdGePoint3d(0.0, 0.0, 1.0));
			pModel->addElement(pZAxis);

 			OdDgCellHeader3dPtr pCellAcisHdr = OdDgCellHeader3d::createObject();
      pCellAcisHdr->setLevelEntryId(0x40);

			// pCell must be DBRO entity.

			pModel->addElement(pCellAcisHdr);

			OdStreamBufPtr pStream = odrxSystemServices()->createFile(strFileName, Oda::kFileRead, Oda::kShareDenyNo, Oda::kOpenExisting);

      if( bACIS )
      {
        OdDgModelerGeometryCreatorPtr pCreator = pCreatorService->create();

        if( !pCreator.isNull() && !bSab )
        {
          OdArray<OdDgModelerGeometryPtr> models;
          pCreator->createModeler(models, pStream);

          OdStreamBufPtr pSABStream = OdMemoryStream::createNew();

          if( models.size() > 0 )
          {
            models[0]->out(pSABStream, OdDgModelerGeometry::kSab | OdDgModelerGeometry::kAS_7_0 );
            pSABStream->rewind();
            pStream = pSABStream;
          }
        }
      }

			if (!pStream.isNull())
				OdDgBRepEntityPEPtr(pCellAcisHdr)->fillSmartSolid(*pCellAcisHdr, *pStream.get(), bACIS);

			OdDgViewGroupTablePtr pViewGroup = m_pDb->getViewGroupTable(OdDg::kForWrite);

			OdDgElementIteratorPtr pVGIter = pViewGroup->createIterator();

			for (; !pVGIter->done(); pVGIter->step())
			{
				OdDgViewGroupPtr pViewGroup = pVGIter->item().openObject(OdDg::kForWrite);

				if (pViewGroup->getModelId() == pModel->elementId())
				{
					pViewGroup->setName(strShortName);

					OdDgElementIteratorPtr pViewIter = pViewGroup->createIterator();

					bool bFirstView = true;

					for (; !pViewIter->done(); pViewIter->step())
					{
						OdDgViewPtr pView = pViewIter->item().openObject(OdDg::kForWrite);

						if (bFirstView)
						{
							pView->setVisibleFlag(true);
							pView->setShowFillsFlag(true);
							bFirstView = false;
						}
						else
						{
							pView->setVisibleFlag(false);
						}
					}
				}
			}

			pModel->fitToView();

			m_pDb->setActiveModelId(pModel->elementId());
		}

		m_pDb->endTransaction();
	}
}

afx_msg void COdaDgnAppDoc::OnTestsCellLibraryPreview()
{
	CCellLibraryPreviewDlg dlg(NULL);

	if (!m_pDb.isNull() && (dlg.DoModal() == IDOK))
	{
		bool bModel2d = (dlg.m_iModelType == 0);
		bool bSharedCell = (dlg.m_iCellType > 0);
		OdString strFileName = dlg.m_strFileName.GetBuffer(dlg.m_strFileName.GetLength());
		dlg.m_strFileName.ReleaseBuffer();

		m_pDb->startUndoRecord();
		m_pDb->startTransaction();

		createCellLibraryPreview(m_pDb, strFileName, bModel2d, bSharedCell);

		m_pDb->endTransaction();

		UpdateAllViews(0);
	}
}

afx_msg void COdaDgnAppDoc::OnSwitchModel()
{
	CSwitchModel newDialog(this->database());

	//do nothing if either new settings were not accepted or there is no vectorization at the moment
	if (newDialog.DoModal() == IDOK && m_pVectorizers.size())
	{
		//remove all opened vectorizers (=viewports)
		while (m_pVectorizers.size())
		{
			COdaDgnAppVectorizer* vectorizer = *m_pVectorizers.begin();
			vectorizer->GetOwner()->DestroyWindow(); //vectorizers are packed to CFrameWindow objects
		}

		//create it all at new (but with the new active model & view group)
		OnVectorize();
	}
}

afx_msg void COdaDgnAppDoc::OnItemTypeLibs()
{
  CItemTypeLibsDlg newDialog(NULL, database());
  newDialog.DoModal();
}

void COdaDgnAppDoc::OnViewSumminfo()
{
	CSummDlg sd(database(), theApp.GetMainWnd());
	sd.DoModal();
}


inline CUserIOConsole* COdaDgnAppDoc::console()
{
	if (m_pConsole.isNull())
		m_pConsole = CUserIOConsole::create(theApp.GetMainWnd());
	return m_pConsole;
}

//
// OdEdBaseIO
//
OdString COdaDgnAppDoc::getString(const OdString& prompt, int options, OdEdStringTracker* pTracker)
{
	OdString sRes;

	if (m_pMacro.get() && !m_pMacro->isEof())
	{
		sRes = m_pMacro->getString(prompt, options, pTracker);
		putString(OdString(prompt) + L" " + sRes);
		return sRes;
	}

	if (m_bConsole)
		return console()->getString(prompt, options, pTracker);

	if (m_pVectorizer)
	{
		m_bConsoleResponded = false;
		sRes = m_pVectorizer->getString(prompt, options, pTracker);
		if (!m_bConsoleResponded)
			putString(OdString(prompt) + L" " + sRes);
		return sRes;
	}

	return console()->getString(prompt, options, pTracker);
}

void COdaDgnAppDoc::putString(const OdString& string)
{
	if (m_pVectorizer)
		m_pVectorizer->putString(string);

	console()->putString(string);
}

OdGePoint3d COdaDgnAppDoc::getPoint(const OdString& prompt, int options, OdEdPointTracker* pTracker)
{
	if (m_pMacro.get() && !m_pMacro->isEof())
	{
		console()->putString(prompt);
		return m_pMacro->getPoint(prompt, options, pTracker);
	}

	if (m_bConsole)
		return m_pConsole->getPoint(prompt, options, pTracker);

	if (m_pVectorizer)
	{
		console()->putString(prompt);
		return m_pVectorizer->getPoint(prompt, options, pTracker);
	}

	return console()->getPoint(prompt, options, pTracker);
}
OdUInt32 COdaDgnAppDoc::getKeyState()
{
	OdUInt32 res = 0;
	if (::GetKeyState(VK_CONTROL) != 0)
		res |= MK_CONTROL;
	if (::GetKeyState(VK_SHIFT) != 0)
		res |= MK_SHIFT;
	return res;
}

OdString COdaDgnAppDoc::commandPrompt()
{
	return _T("Command:");
}

OdString COdaDgnAppDoc::recentCmdName()
{
	return theApp.getRecentCmd().spanExcluding(L" \n");
}

COdaDgnAppVectorizer* COdaDgnAppDoc::getVectorizer()
{
	return m_pVectorizer;
}

void COdaDgnAppDoc::OnFilePageSetup()
{
	OdDgElementId idModel;

	if (!m_pDb.isNull())
	{
		idModel = m_pDb->getActiveModelId();
	}

	CPageSetupDlg dlg(NULL, m_printSettings, idModel);

	if (dlg.DoModal() == IDOK)
	{
		m_printSettings = dlg.getPrinterSettings();

		std::set< COdaDgnAppVectorizer* >::iterator it = m_pVectorizers.begin();

		for (; it != m_pVectorizers.end(); it++)
		{
			(*it)->setPrintSettings(m_printSettings);
		}
	}
}

void COdaDgnAppDoc::OnEditSelectionReactor()
{
	if (!m_pVectorizer)
		return;

	OdGsDCPointArray dcpts;

	OdGsView* pView = m_pVectorizer->getActiveView();
	OdGsDCRect  rectangle;
	pView->getViewport(rectangle);

	OdGsDCPoint low = rectangle.m_min,
		hi = rectangle.m_max;

	dcpts.append(low);
	dcpts.append(hi);

	//-- Reactor
	struct SelectionReactor : OdGsSelectionReactor
	{

		SelectionReactor()
		{
		}
		bool selected(const OdGiDrawableDesc& drawableDesc)
		{
			bool bStatus = true;

			OdDgElementId id(drawableDesc.persistId);
			OdDgElementPtr pObj = id.safeOpenObject(OdDg::kForRead);

			if (!pObj.isNull())
			{
				CString tmp = _T("The element: ");

				OdString msg;
				msg.format(OD_T(" [%s]:<%s>"), pObj->elementId().getHandle().ascii().c_str(), pObj->isA()->name().c_str());
				tmp += msg.c_str();
				if (AfxMessageBox(tmp, MB_OKCANCEL/*MB_OK*/) != IDOK)
				{
					bStatus = false;
					throw OdEdCancel();
				}
			}

			return bStatus;
		}
	}
	selectionReactor;

	pView->select(dcpts.getPtr(), dcpts.size(), &selectionReactor, OdGsView::kCrossing);
}

void COdaDgnAppDoc::OnEditClearselection()
{
	//return;

	selectionSet()->clear();
	if (m_bDisableClearSel) return;
	bool cleared = false;
	POSITION pos = GetFirstViewPosition();
	while (pos != NULL)
	{
		CView *view = GetNextView(pos);
		if (CString(view->GetRuntimeClass()->m_lpszClassName).Compare(_T("COdaDgnAppVectorizer")) == 0 &&
			view->GetDocument() == this)
		{
			COdaDgnAppVectorizer *pDgnAppVectorizer = static_cast<COdaDgnAppVectorizer*>(view);
			pDgnAppVectorizer->editorObject().unselect();
			cleared = true;
		}
	}
	if (!cleared) // No view found
	{
		selectionSet()->clear();
	}
}


void COdaDgnAppDoc::OnEditSelectall()
{
	if (!m_pVectorizer)
		return;

	//  OdDgViewGroupPtr pViewGroup = m_pDb->getActiveViewGroupId().openObject();
	//  OdDgElementId vectorizedViewId;
	//  OdDgViewPtr pView;
	//
	//  OdDgElementIteratorPtr pIt = pViewGroup->createIterator();
	//  CFrameWnd* pTheFirstFrame = 0;
	//  for( ; !pIt->done(); pIt->step() )
	//  {
	//    pView = OdDgView::cast( pIt->item().openObject() );
	//    if( pView.get() && pView->getVisibleFlag() )
	//    {
	//      vectorizedViewId = pIt->item();
	//      OdGsDCRect  rectangle = pView->getViewRectangle();
	//      OdGsDCPoint low = rectangle.m_min,
	//                  hi = rectangle.m_max;
	//
	//      dcpts.append(low);
	//      dcpts.append(hi);
	//
	//      break;
	//    }
	//  }
	//
	//  struct SelectionReactor : OdGsSelectionReactor
	//  {
	//
	//    SelectionReactor()
	//    {
	//    }
	//    bool selected(const OdGiDrawableDesc& drawableDesc)
	//    {
	//      bool bStatus = true;
	//      //const OdGiDrawableDesc* pDesc = &drawableDesc;
	//
	//      OdDgElementId id(drawableDesc.persistId);
	//      OdDgElementPtr pObj = id.safeOpenObject(OdDg::kForRead);
	//
	//      if (!pObj.isNull())
	//      {
	//        CString tmp = _T("The element: ");
	//
	//        OdString msg;
	//        msg.format(OD_T(" [%s]:<%s>"), pObj->elementId().getHandle().ascii().c_str(), pObj->isA()->name().c_str());
	//        tmp += msg.c_str();
	//        if (AfxMessageBox(tmp, MB_OKCANCEL/*MB_OK*/) != IDOK)
	//        {
	//          bStatus = false;
	//          throw OdEdCancel();
	//        }   
	//      }
	//
	//      return bStatus;
	//    }
	//  }
	//  selectionReactor;
	//
	//  pView->select(dcpts.getPtr(), dcpts.size(), &selectionReactor, OdGsView::kCrossing);


	OnEditClearselection();

	m_bDisableClearSel = true;
	ExecuteCommand(OD_T("select single all"));
	//  m_bDisableClearSel = false;
	//  POSITION pos = GetFirstViewPosition();
	//  while (pos != NULL)
	//  {
	//    CView *view = GetNextView(pos);
	//    if (CString(view->GetRuntimeClass()->m_lpszClassName).Compare(_T("COdaDgnAppVectorizer")) == 0 &&
	//        view->GetDocument() == this)
	//    {
	//      COdaDgnAppVectorizer *pDgnAppVectorizer = static_cast<COdaDgnAppVectorizer*>(view);
#pragma MARKMESSAGE("TODO: 3.5 - editing ")
//      pDgnAppVectorizer->editorObject().selectionSetChanged();
//    }
//  }  
}

void COdaDgnAppDoc::OnUpdateEditSelectall(CCmdUI* pCmdUI)
{
	pCmdUI->Enable((m_pVectorizer == NULL) ? false : true);
	//pCmdUI->Enable( false );
}
void COdaDgnAppDoc::OnUpdateClearselection(CCmdUI* pCmdUI)
{
	pCmdUI->Enable((m_pVectorizer == NULL) ? false : true);
	//pCmdUI->Enable( false );
}
void COdaDgnAppDoc::OnUpdateEditSelectionReactor(CCmdUI* pCmdUI)
{
	pCmdUI->Enable((m_pVectorizer == NULL) ? false : true);
}

///////////////////////////////////////////////////////////////////
//  Implementation of "copyLineStylesFromRscFile" command
///////////////////////////////////////////////////////////////////

const OdString Cmd_COPY_LINE_STYLES_FROM_RSC::name()
{
	return L"copyLineStylesFromRscFile";
}

const OdString Cmd_COPY_LINE_STYLES_FROM_RSC::groupName() const
{
	return L"ODADGNAPP";
}

const OdString Cmd_COPY_LINE_STYLES_FROM_RSC::globalName() const
{
	return name();
}

void Cmd_COPY_LINE_STYLES_FROM_RSC::execute(OdEdCommandContext* pCmdCtx)
{
	OdDgCommandContextPtr pDgCmdCtx(pCmdCtx);
	OdDgDatabaseDoc* pDg = (OdDgDatabaseDoc*)pDgCmdCtx->database();

	OdDgLineStyleTablePtr    pLSTable = pDg->getLineStyleTable(OdDg::kForWrite);
	OdDgLineStyleDefTablePtr pLineStyleDef = pDg->getLineStyleDefTable(OdDg::kForWrite);

	if (pLSTable.isNull() || pLineStyleDef.isNull())
	{
		return;
	}

	for (OdUInt32 i = 0; i < pLSTable->getRscLineStyleCount(); i++)
	{
		OdDgLineStyleTableRecordPtr pRscLineStyle = pLSTable->getRscLineStyle(i);

		if (pRscLineStyle.isNull())
		{
			continue;
		}

		if (pLSTable->getAt(pRscLineStyle->getName()).isNull())
		{
			if (pRscLineStyle->getType() != OdDg::kLsTypeInternal)
			{
				OdDgElementId idDefinition = addRscDefinition(pLineStyleDef, pRscLineStyle->getName());

				pRscLineStyle->setRefersToId(idDefinition);
				pRscLineStyle->setRefersToElementFlag(true);
				pLSTable->add(pRscLineStyle);
			}
      else
      {
        pRscLineStyle->setContinuousFlag(true);
        pRscLineStyle->setRefersToId(0);
        pRscLineStyle->setRefersToElementFlag(true);
        pLSTable->add(pRscLineStyle);
      }
		}
	}
}

OdDgElementId Cmd_COPY_LINE_STYLES_FROM_RSC::addRscDefinition(OdDgLineStyleDefTablePtr& pTable, const OdString& strName)
{
	OdDgElementId idRet;

	OdDgLineStyleDefTableRecordPtr pDef = pTable->getRscLineStyleDef(strName);

	if (!pDef.isNull())
	{
		bool bFindDef = false;

		for (OdUInt32 i = 0; i < m_arrNameDef.size(); i++)
		{
			if (m_arrNameDef[i].m_strLSName == strName)
			{
				idRet = m_arrNameDef[i].m_idElement;
				bFindDef = true;
				break;
			}
		}

		if (!bFindDef)
		{
      if (!pTable->getAt(pDef->getName()).isNull())
        idRet = pTable->getAt(pDef->getName());
      else
      {
        OdDgLineStyleDefTableRecordPtr pDefClone = pDef->clone();

        switch (pDef->getType())
        {
          case OdDg::kLsTypeCompound:
          {
            OdDgCompoundLineStyleResourcePtr pRes = pDefClone->getResource();

            for (OdUInt32 j = 0; j < pRes->getComponentCount(); j++)
            {
              OdDgCompoundLineStyleComponentInfo componentInfo;

              pRes->getComponent(j, componentInfo);

              OdDgElementId idComponent = addRscDefinition(pTable, componentInfo.getComponentEntryId(), componentInfo.getComponentType());

              OdUInt32 uComponentHandle = (OdUInt64)(idComponent.getHandle());

              if (uComponentHandle > 0)
              {
                componentInfo.setComponentEntryId(0);
                componentInfo.setComponentHandleId(uComponentHandle);
              }

              pRes->setComponent(j, componentInfo);
            }

            pTable->add(pDefClone);
            idRet = pDefClone->id();

          } break;

          case OdDg::kLsTypeLinePoint:
          {
            OdDgLinePointResourcePtr pRes = pDefClone->getResource();

            OdDgElementId idBasePattern = addRscDefinition(pTable, pRes->getBasePatternEntryId(), pRes->getBasePatternType());

            OdUInt32 uBaseHandle = (OdUInt64)(idBasePattern.getHandle());

            if (uBaseHandle > 0)
            {
              pRes->setBasePatternEntryId(0);
              pRes->setBasePatternHandleId(uBaseHandle);
            }

            for (OdUInt32 j = 0; j < pRes->getSymbolCount(); j++)
            {
              OdDgLinePointResourceSymInfo symbolInfo;

              pRes->getSymbol(j, symbolInfo);

              OdDgElementId idSymbolPattern = addRscDefinition(pTable, symbolInfo.getPointSymbolEntryId(), symbolInfo.getSymbolType());

              OdUInt32 uSymbolHandle = (OdUInt64)(idSymbolPattern.getHandle());

              if (uSymbolHandle > 0)
              {
                symbolInfo.setPointSymbolEntryId(0);
                symbolInfo.setPointSymbolHandleId(uSymbolHandle);
              }

              pRes->setSymbol(j, symbolInfo);
            }

            pTable->add(pDefClone);
            idRet = pDefClone->id();

          } break;

          case OdDg::kLsTypeLineCode:
          {
            pTable->add(pDefClone);
            idRet = pDefClone->id();
          } break;
        }
      }

			lineStyleNamePairForCopyCmd newRecord;
			newRecord.m_strLSName = strName;
			newRecord.m_idElement = idRet;

			m_arrNameDef.push_back(newRecord);
		}
	}

	return idRet;
}

OdDgElementId Cmd_COPY_LINE_STYLES_FROM_RSC::addRscDefinition(OdDgLineStyleDefTablePtr& pTable, OdUInt32 entryId, OdDgLineStyleResource::OdLsResourceType rsType)
{
	OdDgElementId idRet;

	OdDgLineStyleDefTableRecordPtr pDef = pTable->getRscLineStyleDef(entryId, rsType);

	if (!pDef.isNull())
	{
		bool bFindDef = false;

		for (OdUInt32 i = 0; i < m_arrLineStyleDef.size(); i++)
		{
			if ((m_arrLineStyleDef[i].m_uEntryId == entryId) && (m_arrLineStyleDef[i].m_rsType == rsType))
			{
				idRet = m_arrLineStyleDef[i].m_idDef;
				bFindDef = true;
				break;
			}
		}

		if (!bFindDef)
		{
			switch (pDef->getType())
			{
			case OdDg::kLsTypeCompound:
			{
				OdDgLineStyleDefTableRecordPtr pDefClone = pDef->clone();
				OdDgCompoundLineStyleResourcePtr pRes = pDefClone->getResource();

				for (OdUInt32 j = 0; j < pRes->getComponentCount(); j++)
				{
					OdDgCompoundLineStyleComponentInfo componentInfo;

					pRes->getComponent(j, componentInfo);

					OdDgElementId idComponent = addRscDefinition(pTable, componentInfo.getComponentEntryId(), componentInfo.getComponentType());

					OdUInt32 uComponentHandle = (OdUInt64)(idComponent.getHandle());

					if (uComponentHandle > 0)
					{
						componentInfo.setComponentEntryId(0);
						componentInfo.setComponentHandleId(uComponentHandle);
					}

					pRes->setComponent(j, componentInfo);
				}

				pTable->add(pDefClone);
				idRet = pDefClone->id();

			} break;

			case OdDg::kLsTypeLinePoint:
			{
				OdDgLineStyleDefTableRecordPtr pDefClone = pDef->clone();

				OdDgLinePointResourcePtr pRes = pDefClone->getResource();

				OdDgElementId idBasePattern = addRscDefinition(pTable, pRes->getBasePatternEntryId(), pRes->getBasePatternType());

				OdUInt32 uBaseHandle = (OdUInt64)(idBasePattern.getHandle());

				if (uBaseHandle > 0)
				{
					pRes->setBasePatternEntryId(0);
					pRes->setBasePatternHandleId(uBaseHandle);
				}

				for (OdUInt32 j = 0; j < pRes->getSymbolCount(); j++)
				{
					OdDgLinePointResourceSymInfo symbolInfo;

					pRes->getSymbol(j, symbolInfo);

					OdDgElementId idSymbolPattern = addRscDefinition(pTable, symbolInfo.getPointSymbolEntryId(), symbolInfo.getSymbolType());

					OdUInt32 uSymbolHandle = (OdUInt64)(idSymbolPattern.getHandle());

					if (uSymbolHandle > 0)
					{
						symbolInfo.setPointSymbolEntryId(0);
						symbolInfo.setPointSymbolHandleId(uSymbolHandle);
					}

					pRes->setSymbol(j, symbolInfo);
				}

				pTable->add(pDefClone);
				idRet = pDefClone->id();

			} break;

			case OdDg::kLsTypePointSymbol:
			{
				OdDgLineStyleDefTableRecordPtr pDefClone = pDef->clone();

				OdDgPointSymbolResourcePtr pRes = pDef->getResource();
				OdDgPointSymbolResourcePtr pResClone = pDefClone->getResource();

				OdDgElementId idCell3d = pRes->getCell(pTable->database(), entryId, rsType == OdDgLineStyleResource::kLsPointSymbolResV7);

				if (!idCell3d.isNull())
				{
					bool bFindCell = false;

					OdDgElementId idCellToAdd;

					for (OdUInt32 k = 0; k < m_arrCellDef.size(); k++)
					{
						if (m_arrCellDef[k].m_idOldElement == idCell3d)
						{
							idCellToAdd = m_arrCellDef[k].m_idElement;
							bFindCell = true;
							break;
						}
					}

					if (!bFindCell)
					{
						OdDgCellHeader3dPtr pCell3d = idCell3d.openObject(OdDg::kForRead);

						OdDgCellHeader3dPtr pCell3dClone = pCell3d->clone();

						pTable->addSymbol(pCell3dClone);

						idCellToAdd = pCell3dClone->id();
					}

					pResClone->setDependedCellHeaderHandle((OdUInt64)(idCellToAdd.getHandle()));
				}

				pTable->add(pDefClone);
				idRet = pDefClone->id();
			}
			break;

			case OdDg::kLsTypeLineCode:
			{
				OdDgLineStyleDefTableRecordPtr pDefClone = pDef->clone();
				pTable->add(pDefClone);
				idRet = pDefClone->id();
			} break;
			}

			lineStyleDefInfoForCopyCmd recordToAdd;

			recordToAdd.m_rsType = rsType;
			recordToAdd.m_uEntryId = entryId;
			recordToAdd.m_idDef = idRet;

			m_arrLineStyleDef.push_back(recordToAdd);
		}
	}


	return idRet;
}

///////////////////////////////////////////////////////////////////
//  Implementation of "ERASE_ALL_BY_TYPE" command
///////////////////////////////////////////////////////////////////

const OdString Cmd_ERASE_ALL_BY_TYPE::groupName() const { return L"DEBUG_CMD"; }

const OdString Cmd_ERASE_ALL_BY_TYPE::name() { return L"EraseAllByType"; }

const OdString Cmd_ERASE_ALL_BY_TYPE::globalName() const { return name(); }

void Cmd_ERASE_ALL_BY_TYPE::execute(OdEdCommandContext* pCmdCtx)
{
	OdDgCommandContextPtr pDgCmdCtx(pCmdCtx);
	OdDgDatabasePtr pDb = pDgCmdCtx->database();
	OdDgUserIO* pIO = pDgCmdCtx->dbUserIO();

	OdDgElementIdArray entIds = Cmd_SELECT::selectEntities(pIO, L"Select entity to choose entity type");

	for (OdUInt32 i = 0; i < entIds.size(); i++)
	{
		if (!entIds[i].isNull())
		{
			OdDgElementPtr pElm = entIds[i].openObject(OdDg::kForWrite);

			if (pElm->isErased())
			{
				continue;
			}

			OdDgElementId idOwner = pElm->ownerId();

			if (!idOwner.isNull())
			{
				OdDgElementPtr pOwner = idOwner.openObject(OdDg::kForWrite);

				OdDgElementIteratorPtr pOwnerIter;

				if (pOwner->isKindOf(OdDgModel::desc()))
				{
					OdDgModelPtr pModel = pOwner;

					if (pElm->isKindOf(OdDgGraphicsElement::desc()))
					{
						pOwnerIter = pModel->createGraphicsElementsIterator();
					}
					else
					{
						pOwnerIter = pModel->createControlElementsIterator();
					}
				}
				else if (pOwner->isKindOf(OdDgSharedCellDefinition::desc()))
				{
					OdDgSharedCellDefinitionPtr pSharedCellDef = pOwner;

					pOwnerIter = pSharedCellDef->createIterator();
				}
				else if (pOwner->isKindOf(OdDgCellHeader2d::desc()))
				{
					OdDgCellHeader2dPtr pCell = pOwner;

					pOwnerIter = pCell->createIterator();
				}
				else if (pOwner->isKindOf(OdDgCellHeader3d::desc()))
				{
					OdDgCellHeader3dPtr pCell = pOwner;

					pOwnerIter = pCell->createIterator();
				}

				if (!pOwnerIter.isNull())
				{
					for (; !pOwnerIter->done(); pOwnerIter->step())
					{
						OdDgElementPtr pChildElm = pOwnerIter->item().openObject(OdDg::kForWrite);

						if (pChildElm->getElementType() == pElm->getElementType())
						{
							pChildElm->erase(true);
						}
					}
				}
			}
		}
	}
}

///////////////////////////////////////////////////////////////////
//  Implementation of "ISOLATE" command
///////////////////////////////////////////////////////////////////

const OdString Cmd_ISOLATE::groupName() const { return L"DEBUG_CMD"; }

const OdString Cmd_ISOLATE::name() { return L"Isolate"; }

const OdString Cmd_ISOLATE::globalName() const { return name(); }

void  Cmd_ISOLATE::extendSelectionArray(const OdDgElementIdArray& arrSelected, std::set<OdDgElementId>& arrFullSet)
{
	for (OdUInt32 i = 0; i < arrSelected.size(); i++)
	{
		addElementIdToSet(arrFullSet, arrSelected[i]);

		OdDgElementPtr pBaseElm = arrSelected[i].openObject(OdDg::kForRead);

		while (!pBaseElm->ownerId().isNull())
		{
			pBaseElm = pBaseElm->ownerId().openObject(OdDg::kForRead);
			addElementIdToSet(arrFullSet, pBaseElm->elementId());
		}

		pBaseElm = arrSelected[i].openObject(OdDg::kForRead);

		OdDgElementIteratorPtr pIter = getElementIterator(pBaseElm);

		if (!pIter.isNull())
		{
			addComplexIdToSet(arrFullSet, pIter);
		}
	}
}

void Cmd_ISOLATE::addComplexIdToSet(std::set<OdDgElementId>& setSelected,
	OdDgElementIteratorPtr& pIter)
{
	for (; !pIter->done(); pIter->step())
	{
		addElementIdToSet(setSelected, pIter->item());

		OdDgElementPtr pItem = pIter->item().openObject(OdDg::kForRead);

		OdDgElementIteratorPtr pChildIter = getElementIterator(pItem);

		if (!pChildIter.isNull())
		{
			addComplexIdToSet(setSelected, pChildIter);
		}
	}
}

void Cmd_ISOLATE::addElementIdToSet(std::set<OdDgElementId>& setSelected,
	const OdDgElementId& idElement)
{
	if (idElement.isNull())
	{
		return;
	}

	setSelected.insert(idElement);
}

OdDgElementId Cmd_ISOLATE::getTopContainerId(const OdDgElementPtr& pElm)
{
	OdDgElementPtr pChild = pElm;

	OdDgElementId idRetVal = pChild->ownerId();

	while (!pChild->ownerId().isNull())
	{
		idRetVal = pChild->ownerId();
		pChild = idRetVal.openObject(OdDg::kForRead);
	}

	return idRetVal;
}

OdDgElementIteratorPtr Cmd_ISOLATE::getElementIterator(const OdDgElementPtr& pElm)
{
	OdDgElementIteratorPtr pRetVal;

	if (pElm->isKindOf(OdDgModel::desc()))
	{
		OdDgModelPtr pModel = pElm;

		pRetVal = pModel->createGraphicsElementsIterator();
	}
	else if (pElm->isKindOf(OdDgSharedCellDefinition::desc()))
	{
		OdDgSharedCellDefinitionPtr pSharedCellDef = pElm;

		pRetVal = pSharedCellDef->createIterator();
	}
	else if (pElm->isKindOf(OdDgCellHeader2d::desc()))
	{
		OdDgCellHeader2dPtr pCell = pElm;

		pRetVal = pCell->createIterator();
	}
	else if (pElm->isKindOf(OdDgCellHeader3d::desc()))
	{
		OdDgCellHeader3dPtr pCell = pElm;

		pRetVal = pCell->createIterator();
	}
	else if (pElm->isKindOf(OdDgTextNode2d::desc()))
	{
		OdDgTextNode2dPtr pTextNode = pElm;

		pRetVal = pTextNode->createIterator();
	}
	else if (pElm->isKindOf(OdDgTextNode3d::desc()))
	{
		OdDgTextNode3dPtr pTextNode = pElm;

		pRetVal = pTextNode->createIterator();
	}
	else if (pElm->isKindOf(OdDgComplexShape::desc()))
	{
		OdDgComplexShapePtr pCplxShape = pElm;

		pRetVal = pCplxShape->createIterator();
	}
	else if (pElm->isKindOf(OdDgComplexString::desc()))
	{
		OdDgComplexStringPtr pCplxString = pElm;

		pRetVal = pCplxString->createIterator();
	}
	else if (pElm->isKindOf(OdDgTable::desc()))
	{
		OdDgTablePtr pTable = pElm;

		pRetVal = pTable->createIterator();
	}

	return pRetVal;
}

bool Cmd_ISOLATE::isElementSelected(const OdDgElementId& idElement,
	const std::set<OdDgElementId>& setSelected)
{
	bool bRet = false;

	std::set<OdDgElementId>::const_iterator pIter = setSelected.find(idElement);

	if (pIter != setSelected.end())
	{
		bRet = true;
	}

	return bRet;
}

void Cmd_ISOLATE::isolateElements(OdDgElementIteratorPtr& pIter,
	const std::set<OdDgElementId>& setSelected)
{
	for (; !pIter->done(); pIter->step())
	{
		OdDgElementPtr pItem = pIter->item().openObject(OdDg::kForWrite);

		if (!isElementSelected(pIter->item(), setSelected))
		{
			pItem->erase(true);
		}
		else
		{
			OdDgElementIteratorPtr pChildIter = getElementIterator(pItem);

			if (!pChildIter.isNull())
			{
				isolateElements(pChildIter, setSelected);
			}
		}
	}
}

void Cmd_ISOLATE::execute(OdEdCommandContext* pCmdCtx)
{
	OdDgCommandContextPtr pDgCmdCtx(pCmdCtx);
	OdDgDatabasePtr pDb = pDgCmdCtx->database();
	OdDgUserIO* pIO = pDgCmdCtx->dbUserIO();

	OdDgElementIdArray entIds = Cmd_SELECT::selectEntities(pIO, L"Select entity to isolate");

	if (!entIds.isEmpty())
	{
		std::set<OdDgElementId> setSelected;
		extendSelectionArray(entIds, setSelected);

		std::set<OdDgElementId> setOwners;

		for (OdUInt32 i = 0; i < entIds.size(); i++)
		{
			OdDgElementId idOwner = getTopContainerId(entIds[i].openObject(OdDg::kForRead));

			addElementIdToSet(setOwners, idOwner);
		}

		if (!setOwners.empty())
		{
			std::set<OdDgElementId>::iterator pOwnerIter = setOwners.begin();

			for (; pOwnerIter != setOwners.end(); pOwnerIter++)
			{
				OdDgElementIteratorPtr pElmIter = getElementIterator(pOwnerIter->openObject(OdDg::kForWrite));

				if (!pElmIter.isNull())
				{
					isolateElements(pElmIter, setSelected);
				}
			}
		}
	}
}

///////////////////////////////////////////////////////////////////
//  Implementation of "ERASE" command
///////////////////////////////////////////////////////////////////

const OdString Cmd_CraeteDefaultInst::groupName() const { return L"ODDB_COMMANDS"; }

const OdString Cmd_CraeteDefaultInst::name() { return L"CraeteDefaultInst"; }

const OdString Cmd_CraeteDefaultInst::globalName() const { return name(); }

#include "DgECSchema.h"
#ifdef DEFAULT_ECCLASSINSTANCE_GEN
#include "DgECDefaultClassInstanceCodeGen.h"
#endif
void Cmd_CraeteDefaultInst::execute(OdEdCommandContext* pCmdCtx)
{
#ifdef DEFAULT_ECCLASSINSTANCE_GEN
  FILE* pSchemaFile = fopen("C:/Users/aryabinin/Desktop/ECSchemas/Dgn/DgnCustomAttributes.01.00.ecschema.xml", "rt");
  OdBinaryData binData;
  binData.resize(1048576, 0);
  OdUInt64 nBytes = fread(binData.asArrayPtr(), 1, 1048576, pSchemaFile);
  binData.resize(nBytes + 1);
  OdAnsiString strAnsi((char*)binData.asArrayPtr(), CP_ANSI_1252);
  OdString strSchema = strAnsi;

  OdDgECSchemaPtr pSchema = OdDgECSchema::createObject();
  pSchema->loadFromXMLString(strSchema);

  OdDgECDefaultClassInstanceGen codeGen;
  codeGen.generateCodeSections(pSchema, L"D:/TestOut/DefaultClassInstances.txt", 137);
  fclose(pSchemaFile);
#endif
}

///////////////////////////////////////////////////////////////////
//  Implementation of "ERASE" command
///////////////////////////////////////////////////////////////////

const OdString Cmd_ERASE::groupName() const { return L"ODDB_COMMANDS"; }

const OdString Cmd_ERASE::name() { return L"ERASE"; }

const OdString Cmd_ERASE::globalName() const { return name(); }

///////////////////////////////////////////////////////////////////
//  Implementation of "Scale2" command
///////////////////////////////////////////////////////////////////

const OdString Cmd_Scale2::groupName() const { return L"Transformation"; }

const OdString Cmd_Scale2::name() { return L"Scale_2.0"; }

const OdString Cmd_Scale2::globalName() const { return name(); }

///////////////////////////////////////////////////////////////////
//  Implementation of "RotateXY30" command
///////////////////////////////////////////////////////////////////

const OdString Cmd_RotateXY30::groupName() const { return L"Transformation"; }

const OdString Cmd_RotateXY30::name() { return L"Rotate_XY_30.0"; }

const OdString Cmd_RotateXY30::globalName() const { return name(); }

///////////////////////////////////////////////////////////////////
//  Implementation of "ERASE_Invisible" command
///////////////////////////////////////////////////////////////////

const OdString Cmd_ERASE_Invisible::groupName() const { return L"ODADGNAPP"; }

const OdString Cmd_ERASE_Invisible::name() { return L"ERASE_Invisible"; }

const OdString Cmd_ERASE_Invisible::globalName() const { return name(); }


///////////////////////////////////////////////////////////////////
//  Implementation of "ConvertToExtGraphics" command
///////////////////////////////////////////////////////////////////

const OdString Cmd_ConvertToExtGraphics::groupName() const { return L"ODADGNAPP"; }

const OdString Cmd_ConvertToExtGraphics::name() { return L"ConvertToExtGraphics"; }

const OdString Cmd_ConvertToExtGraphics::globalName() const { return name(); }

void Cmd_ConvertToExtGraphics::execute(OdEdCommandContext* pCmdCtx)
{
  OdDgCommandContextPtr pDgCmdCtx(pCmdCtx);
  OdDgDatabasePtr pDb = pDgCmdCtx->database();
  OdDgUserIO* pIO = pDgCmdCtx->dbUserIO();

  OdDgElementIdArray entIds = Cmd_SELECT::selectEntities(pIO, L"Select entity to convert");

  OdDgModelPtr pModel;
  OdDgElementId idModel = pDb->getActiveModelId();

  if( !idModel.isNull() )
    pModel = idModel.openObject(OdDg::kForWrite);

  if( pModel.isNull() )
    return;

  for (OdUInt32 i = 0; i < entIds.size(); i++)
  {
    if (!entIds[i].isNull())
    {
      OdDgElementPtr pElm = entIds[i].openObject(OdDg::kForWrite);

      if( pElm->isKindOf(OdDgGraphicsElement::desc()) )
      {
        OdDgCreateGraphicsCacheTool tool;
        OdDgXAttributePtr pXAttr = tool.createGraphicsCache(pElm);

        if( !pXAttr.isNull() )
        {
          OdDgGeometryCacheXAttributePtr pCacheXAttr = pXAttr;

          OdDgExtendedGraphicsElementPtr pGraph = OdDgExtendedGraphicsElement::createObject();
          pGraph->setPropertiesFrom( pElm );
          pGraph->setTypeValue( pCacheXAttr->getTypeValue() );

          for( OdUInt32 i = 0; i < pCacheXAttr->getActionCount(); i++ )
            pGraph->addCacheItem(pCacheXAttr->getAction(i));

          pModel->addElement( pGraph );
          pElm->erase(true);
        }
      }
    }
  }
}

///////////////////////////////////////////////////////////////////
//  Implementation of "SetGlobalOrigin" command
///////////////////////////////////////////////////////////////////

const OdString Cmd_SetGlobalOrigin::groupName() const { return L"ODADGNAPP"; }

const OdString Cmd_SetGlobalOrigin::name() { return L"SetGlobalOrigin"; }

const OdString Cmd_SetGlobalOrigin::globalName() const { return name(); }

void Cmd_SetGlobalOrigin::execute(OdEdCommandContext* pCmdCtx)
{
  OdDgCommandContextPtr pDgCmdCtx(pCmdCtx);
  OdDgDatabasePtr pDb = pDgCmdCtx->database();
  OdDgUserIO* pIO = pDgCmdCtx->dbUserIO();

  OdGePoint3d ptNewGO = pIO->getPoint(L"Specify new global origin point:");

  OdDgModelPtr pActiveModel = pDb->getActiveModelId().openObject(OdDg::kForWrite);
  pActiveModel->setGlobalOrigin(ptNewGO + pActiveModel->getGlobalOrigin().asVector());

  OdDgDatabaseDoc* pDg = (OdDgDatabaseDoc*)pDgCmdCtx->database();
  COdaDgnAppDoc *pDoc = pDg->document();
  COdaDgnAppVectorizer* pVectorizer = pDoc->getVectorizer();

  if (pVectorizer)
  {
    OdGsDeviceForDgModelPtr pDevice = pVectorizer->getDevice();

    OdDgViewPtr pDgView = pVectorizer->getViewId().openObject(OdDg::kForRead);

    if (!pDevice.isNull() && !pDgView.isNull())
    {
      for (int j = 0; j < pDevice->numViews(); j++)
      {
        OdGsView* pGsView = pDevice->viewAt(j);

        OdAbstractViewPEPtr pAVP(pGsView);
        pAVP->setView(pGsView, pDgView);
      }

      pDevice->invalidate();
      if (pDevice->gsModel())
        pDevice->gsModel()->invalidate(OdGsModel::kInvalidateAll);

      pVectorizer->PostMessage(WM_PAINT);
    }
  }
}

///////////////////////////////////////////////////////////////////
//  Implementation of "ConvertToMesh" command
///////////////////////////////////////////////////////////////////

const OdString Cmd_ConvertToMesh::groupName() const { return L"ODADGNAPP"; }

const OdString Cmd_ConvertToMesh::name() { return L"ConvertToMesh"; }

const OdString Cmd_ConvertToMesh::globalName() const { return name(); }

void Cmd_ConvertToMesh::execute(OdEdCommandContext* pCmdCtx)
{
  OdDgCommandContextPtr pDgCmdCtx(pCmdCtx);
  OdDgDatabasePtr pDb = pDgCmdCtx->database();
  OdDgUserIO* pIO = pDgCmdCtx->dbUserIO();

  OdDgElementIdArray entIds = Cmd_SELECT::selectEntities(pIO, L"Select entity to convert");

  OdDgModelPtr pModel;
  OdDgElementId idModel = pDb->getActiveModelId();

  if (!idModel.isNull())
    pModel = idModel.openObject(OdDg::kForWrite);

  if (pModel.isNull())
    return;

  for (OdUInt32 i = 0; i < entIds.size(); i++)
  {
    if (!entIds[i].isNull())
    {
      OdDgElementPtr pElm = entIds[i].openObject(OdDg::kForWrite);

      if (pElm->isKindOf(OdDgGraphicsElement::desc()))
      {
        OdDgGraphicsElementPtr pGrElm = pElm;

        if( pGrElm->getElementType() == OdDgElement::kTypeCellHeader )
        {
          if( pGrElm->isKindOf(OdDgCellHeader3d::desc()))
          {
            OdDgCellHeader3dPtr pCell3d = pGrElm;

            OdDgElementIteratorPtr pIter = pCell3d->createIterator();

            if( !pIter->done() )
            {
              OdDgElementPtr pItem = pIter->item().openObject(OdDg::kForRead);

              if (pItem->isKindOf(OdDgGraphicsElement::desc()))
                pGrElm = pItem;
            }
          }
          else if (pGrElm->isKindOf(OdDgCellHeader2d::desc()))
          {
            OdDgCellHeader2dPtr pCell2d = pGrElm;

            OdDgElementIteratorPtr pIter = pCell2d->createIterator();

            if (!pIter->done())
            {
              OdDgElementPtr pItem = pIter->item().openObject(OdDg::kForRead);

              if (pItem->isKindOf(OdDgGraphicsElement::desc()))
                pGrElm = pItem;
            }
          }
        }

        double dTolerance = 1e-6;
        OdGeExtents3d extElement;
        pElm->getGeomExtents(extElement);

        if( extElement.isValidExtents() )
          dTolerance = extElement.maxPoint().distanceTo(extElement.minPoint()) / 1000.0;

        OdGePoint3dArray vertexArray;
        OdInt32Array     faceArray;
        OdGiFaceData*    pFaceData = NULL;
        bool bApplyColors = false;

        if( odDgGetObjectMesh(pElm, dTolerance, vertexArray, faceArray, pFaceData) == eOk )
        {
          OdDgMeshFaceLoopsPtr pMesh = OdDgMeshFaceLoops::createObject();
          pMesh->setDatabaseDefaults(pElm->database());
          pModel->addElement(pMesh);

          if( pGrElm->getFillType() == OdDg::kFillColor )
            pMesh->setColorIndex(pGrElm->getFillColorIndex());
          else
            pMesh->setColorIndex(pGrElm->getColorIndex());

          pMesh->setMaterial(pGrElm->getMaterial());
          pMesh->setTransparency(pGrElm->getTransparency());
          pMesh->setLevelEntryId(pGrElm->getLevelEntryId());

          pMesh->setPointsNumber(vertexArray.size());

          for( OdUInt32 i = 0; i < vertexArray.size(); i++ )
            pMesh->setPoint(i, vertexArray[i]);

          OdUInt32 uFaceNum = 0;

          for (OdUInt32 j = 0; j < faceArray.size(); j++)
          {
            OdUInt32 nPoints = abs(faceArray[j]);
            bool bHole = faceArray[j] < 0;
            j++;

            OdDgMeshFaceLoops::VerticesArray faceData;

            for( OdUInt32 k = 0; k < nPoints; k++, j++ )
            {
              OdDgMeshFaceLoops::FacePoint newPoint;
              newPoint.m_pointIndex = faceArray[j];

              if( pFaceData )
              {
                if( pFaceData->trueColors() )
                {
                  OdCmEntityColor curColor = pFaceData->trueColors()[uFaceNum];

                  if( bHole )
                    newPoint.m_colorIndex = 0;
                  else if( curColor.isByDgnIndex() )
                    newPoint.m_colorIndex = curColor.colorIndex();

                  if( !bApplyColors )
                  {
                    pMesh->setUseColorTableIndexesFlag(true);
                    bApplyColors = true;
                  }
                }
                else if (bHole)
                {
                  newPoint.m_colorIndex = 0;

                  if (!bApplyColors)
                  {
                    pMesh->setUseColorTableIndexesFlag(true);
                    bApplyColors = true;
                  }
                }
              }
              else if (bHole)
              {
                newPoint.m_colorIndex = 0;

                if (!bApplyColors)
                {
                  pMesh->setUseColorTableIndexesFlag(true);
                  bApplyColors = true;
                }
              }

              faceData.push_back(newPoint);
            }

            pMesh->addFace(faceData);
            uFaceNum++;

            j--;
          }

          pElm->erase(true);
        }
      }
    }
  }
}

///////////////////////////////////////////////////////////////////
//  Implementation of "UNDO" command
///////////////////////////////////////////////////////////////////

const OdString Cmd_UNDO::groupName() const { return L"ODDB_COMMANDS"; }

const OdString Cmd_UNDO::name() { return L"UNDO"; }

const OdString Cmd_UNDO::globalName() const { return name(); }

///////////////////////////////////////////////////////////////////
//  Implementation of "LINE" command
///////////////////////////////////////////////////////////////////

const OdString Cmd_LINE::groupName() const { return L"ODDB_COMMANDS"; }

const OdString Cmd_LINE::name() { return L"LINE"; }

const OdString Cmd_LINE::globalName() const { return name(); }

///////////////////////////////////////////////////////////////////
//  Implementation of "ARC" command
///////////////////////////////////////////////////////////////////

const OdString Cmd_ARC::groupName() const { return L"ODDB_COMMANDS"; }

const OdString Cmd_ARC::name() { return L"ARC"; }

const OdString Cmd_ARC::globalName() const { return name(); }

///////////////////////////////////////////////////////////////////
//  Implementation of "CIRCLE" command
///////////////////////////////////////////////////////////////////

const OdString Cmd_CIRCLE::groupName() const { return L"ODDB_COMMANDS"; }

const OdString Cmd_CIRCLE::name() { return L"CIRCLE"; }

const OdString Cmd_CIRCLE::globalName() const { return name(); }

///////////////////////////////////////////////////////////////////
//  Implementation of "TESTUNDOPL" command
///////////////////////////////////////////////////////////////////

const OdString Cmd_TESTUNDOPL::groupName() const { return L"ODDB_COMMANDS"; }

const OdString Cmd_TESTUNDOPL::name() { return L"TESTUNDOPL"; }

const OdString Cmd_TESTUNDOPL::globalName() const { return name(); }

///////////////////////////////////////////////////////////////////
//  Implementation of "CONE" command
///////////////////////////////////////////////////////////////////

const OdString Cmd_CONE::groupName() const { return L"ODDB_COMMANDS"; }

const OdString Cmd_CONE::name() { return L"CONE"; }

const OdString Cmd_CONE::globalName() const { return name(); }

///////////////////////////////////////////////////////////////////
//  Implementation of "TEXT" command
///////////////////////////////////////////////////////////////////

const OdString Cmd_TEXT::groupName() const { return L"ODDB_COMMANDS"; }

const OdString Cmd_TEXT::name() { return L"TEXT"; }

const OdString Cmd_TEXT::globalName() const { return name(); }

///////////////////////////////////////////////////////////////////
//  Implementation of "CREATE_VIEW_GROUPS" command
///////////////////////////////////////////////////////////////////

const OdString Cmd_CREATE_VIEW_GROUPS::groupName() const { return L"ODADGNAPP"; }

const OdString Cmd_CREATE_VIEW_GROUPS::name() { return L"Cmd_CREATE_VIEW_GROUPS"; }

const OdString Cmd_CREATE_VIEW_GROUPS::globalName() const { return name(); }

void Cmd_CREATE_VIEW_GROUPS::execute(OdEdCommandContext* pCmdCtx)
{
	OdDgCommandContextPtr pDgCmdCtx(pCmdCtx);
	OdDgDatabasePtr pDb = pDgCmdCtx->database();
	OdDgUserIO* pIO = pDgCmdCtx->dbUserIO();

	OdDgModelTablePtr pModelTable = pDb->getModelTable(OdDg::kForRead);
	OdDgViewGroupTablePtr pVGTable = pDb->getViewGroupTable(OdDg::kForWrite);

	OdDgElementIteratorPtr pModelIter = pModelTable->createIterator();

	for (; !pModelIter->done(); pModelIter->step())
	{
		OdDgElementId idModel = pModelIter->item();

		bool bCreateViewGroup = true;

		OdDgElementIteratorPtr pVGIter = pVGTable->createIterator();

		for (; !pVGIter->done(); pVGIter->step())
		{
			OdDgViewGroupPtr pViewGroup = pVGIter->item().openObject(OdDg::kForRead);

			if (pViewGroup->getModelId() == idModel)
			{
				bCreateViewGroup = false;
				break;
			}
		}

		if (bCreateViewGroup)
		{
			OdDgModelPtr pModel = idModel.openObject(OdDg::kForRead);
			pModel->createViewGroup();
			pModel->fitToView();
		}
	}
}

///////////////////////////////////////////////////////////////////
//  Implementation of "ERASE_UNUSED_SHARED_CELLS" command
///////////////////////////////////////////////////////////////////

const OdString Cmd_ERASE_UNUSED_SHARED_CELLS::groupName() const { return L"ODDB_COMMANDS"; }

const OdString Cmd_ERASE_UNUSED_SHARED_CELLS::name() { return L"ERASE_UNUSED_SHARED_CELLS"; }

const OdString Cmd_ERASE_UNUSED_SHARED_CELLS::globalName() const { return name(); }

void Cmd_ERASE_UNUSED_SHARED_CELLS::execute(OdEdCommandContext* pCmdCtx)
{
	OdDgCommandContextPtr pDgCmdCtx(pCmdCtx);
	OdDgDatabasePtr pDb = pDgCmdCtx->database();

	OdArray<OdDgElementId> m_arrSharedCellIds;
	OdArray<bool>          m_arrSharedCellUsed;

	OdDgSharedCellDefinitionTablePtr pSharedCellDefTable = pDb->getSharedCellDefinitionTable(OdDg::kForWrite);

	if (!pSharedCellDefTable.isNull())
	{
		OdDgElementIteratorPtr pIter = pSharedCellDefTable->createIterator();

		for (; !pIter->done(); pIter->step())
		{
			m_arrSharedCellIds.push_back(pIter->item());
			m_arrSharedCellUsed.push_back(false);
		}
	}

	if (m_arrSharedCellIds.size() > 0)
	{
		OdDgModelTablePtr pModelTable = pDb->getModelTable(OdDg::kForRead);

		if (!pModelTable.isNull())
		{
			OdDgElementIteratorPtr pModelIter = pModelTable->createIterator();

			for (; !pModelIter->done(); pModelIter->step())
			{
				OdDgModelPtr pModel = pModelIter->item().openObject(OdDg::kForRead);

				if (!pModel.isNull())
				{
					OdDgElementIteratorPtr pGraphIter = pModel->createGraphicsElementsIterator();
					OdDgElementIteratorPtr pCtrlIter = pModel->createGraphicsElementsIterator();

					markSharedCells(pGraphIter, m_arrSharedCellIds, m_arrSharedCellUsed, pDb);
					markSharedCells(pCtrlIter, m_arrSharedCellIds, m_arrSharedCellUsed, pDb);
				}
			}
		}

		OdDgNonModelElementCollectionPtr pNonModel = pDb->getNonModelElementCollection(OdDg::kForRead);

		if (!pNonModel.isNull())
		{
			OdDgElementIteratorPtr pNonModelIter = pNonModel->createIterator();
			markSharedCells(pNonModelIter, m_arrSharedCellIds, m_arrSharedCellUsed, pDb);
		}

		markNestedSharedCells(m_arrSharedCellIds, m_arrSharedCellUsed, pDb);

		for (OdUInt32 t = 0; t < m_arrSharedCellUsed.size(); t++)
		{
			if (m_arrSharedCellUsed[t])
			{
				continue;
			}

			OdDgElementPtr pCellToRemove = m_arrSharedCellIds[t].openObject(OdDg::kForWrite);

			pCellToRemove->erase(true);
		}
	}
}

void Cmd_ERASE_UNUSED_SHARED_CELLS::markNestedSharedCells(OdArray<OdDgElementId> arrCellIds,
	OdArray<bool>& arrCellUsage,
	OdDgDatabasePtr pDb
)
{
	OdArray<bool> arrCellProcessed;

	for (OdUInt32 i = 0; i < arrCellUsage.size(); i++)
	{
		arrCellProcessed.push_back(false);
	}

	bool bCellProcessingComplete = false;

	while (!bCellProcessingComplete)
	{
		bCellProcessingComplete = true;

		for (OdUInt32 j = 0; j < arrCellProcessed.size(); j++)
		{
			if (arrCellProcessed[j])
			{
				continue;
			}

			if (arrCellUsage[j])
			{
				bCellProcessingComplete = false;

				OdDgSharedCellDefinitionPtr pSharedCellDef = arrCellIds[j].openObject(OdDg::kForRead);

				if (!pSharedCellDef.isNull())
				{
					OdDgElementIteratorPtr pCellIter = pSharedCellDef->createIterator();

					markSharedCells(pCellIter, arrCellIds, arrCellUsage, pDb);
				}

				arrCellProcessed[j] = true;
			}
		}
	}
}

void Cmd_ERASE_UNUSED_SHARED_CELLS::markSharedCells(OdDgElementIteratorPtr pElmIter,
	OdArray<OdDgElementId> arrCellIds,
	OdArray<bool>& arrCellUsage,
	OdDgDatabasePtr pDb
)
{
	if (pElmIter.isNull())
	{
		return;
	}

	for (; !pElmIter->done(); pElmIter->step())
	{
		OdDgElementPtr pElm = pElmIter->item().openObject(OdDg::kForRead);

		if (!pElm.isNull())
		{
			OdArray<OdDgElementId> arrCells;

			if (pElm->isKindOf(OdDgSharedCellReference::desc()))
			{
				OdDgSharedCellReferencePtr pSharedCellRef = pElm;

				if (!pSharedCellRef.isNull())
				{
					OdDgSharedCellDefinitionPtr pSharedCellDef = pSharedCellRef->findDefinition();

					if (!pSharedCellDef.isNull())
					{
						arrCells.push_back(pSharedCellDef->elementId());
					}
				}
			}
			else if (pElm->getElementType() == OdDgElement::kTypeDimension)
			{
				OdDgDimensionPtr pDim = pElm;

				if (!pDim.isNull())
				{
					OdDgDimOptionTerminatorsPtr pTermOptions = pDim->getOption(OdDgDimOption::kTerminators);

					if (!pTermOptions.isNull())
					{
						if (pTermOptions->getArrowTermStyle() == OdDgDimOptionTerminators::kTermScaledCell ||
							  pTermOptions->getArrowTermStyle() == OdDgDimOptionTerminators::kTermCell
							)
						{
							OdDgElementId idTerm = pDb->getElementId(OdDbHandle(pTermOptions->getArrowCellID()));

							if (!idTerm.isNull())
							{
								arrCells.push_back(idTerm);
							}
						}

						if (pTermOptions->getDotTermStyle() == OdDgDimOptionTerminators::kTermScaledCell ||
							  pTermOptions->getDotTermStyle() == OdDgDimOptionTerminators::kTermCell
							)
						{
							OdDgElementId idTerm = pDb->getElementId(OdDbHandle(pTermOptions->getDotCellID()));

							if (!idTerm.isNull())
							{
								arrCells.push_back(idTerm);
							}
						}

						if (pTermOptions->getOriginTermStyle() == OdDgDimOptionTerminators::kTermScaledCell ||
							  pTermOptions->getOriginTermStyle() == OdDgDimOptionTerminators::kTermCell
							 )
						{
							OdDgElementId idTerm = pDb->getElementId(OdDbHandle(pTermOptions->getOriginCellID()));

							if (!idTerm.isNull())
							{
								arrCells.push_back(idTerm);
							}
						}

						if (pTermOptions->getStrokeTermStyle() == OdDgDimOptionTerminators::kTermScaledCell ||
							  pTermOptions->getStrokeTermStyle() == OdDgDimOptionTerminators::kTermCell
							)
						{
							OdDgElementId idTerm = pDb->getElementId(OdDbHandle(pTermOptions->getStrokeCellID()));

							if (!idTerm.isNull())
							{
								arrCells.push_back(idTerm);
							}
						}

            if (pTermOptions->getNoteTermStyle() == OdDgDimOptionTerminators::kTermScaledCell ||
                pTermOptions->getNoteTermStyle() == OdDgDimOptionTerminators::kTermCell
              )
            {
              OdDgElementId idTerm = pDb->getElementId(OdDbHandle(pTermOptions->getNoteCellID()));

              if (!idTerm.isNull())
              {
                arrCells.push_back(idTerm);
              }
            }
					}

					OdDgDimOptionProxyCellPtr pCellOptions = pDim->getOption(OdDgDimOption::kProxyCell);

					if (!pCellOptions.isNull())
					{
            OdDgElementId idProxyCell = pDb->getElementId(OdDbHandle(pCellOptions->getProxyCellId()));

            if (!idProxyCell.isNull())
            {
              arrCells.push_back(idProxyCell);
            }
					}

          OdDgDimOptionPrefixSymbolPtr pPrefixOptions = pDim->getOption(OdDgDimOption::kPrefixSymbol);

          if( !pPrefixOptions.isNull() && (pPrefixOptions->getStyle() == OdDgDimOptionPrefixSuffixSymbol::kScaledCell) )
          {
            OdDgElementId idCell = pDb->getElementId(OdDbHandle(pPrefixOptions->getCellId()));

            if (!idCell.isNull())
            {
              arrCells.push_back(idCell);
            }
          }

          OdDgDimOptionSuffixSymbolPtr pSuffixOptions = pDim->getOption(OdDgDimOption::kSuffixSymbol);

          if( !pSuffixOptions.isNull() && (pSuffixOptions->getStyle() == OdDgDimOptionPrefixSuffixSymbol::kScaledCell) )
          {
            OdDgElementId idCell = pDb->getElementId(OdDbHandle(pSuffixOptions->getCellId()));

            if (!idCell.isNull())
            {
              arrCells.push_back(idCell);
            }
          }
				}
			}
			else if (pElm->isKindOf(OdDgCellHeader2d::desc()))
			{
				OdDgCellHeader2dPtr pCell2d = pElm;

				if (!pCell2d.isNull())
				{
					OdDgElementIteratorPtr pCellIter = pCell2d->createIterator();

					markSharedCells(pCellIter, arrCellIds, arrCellUsage, pDb);
				}
			}
			else if (pElm->isKindOf(OdDgCellHeader3d::desc()))
			{
				OdDgCellHeader3dPtr pCell3d = pElm;

				if (!pCell3d.isNull())
				{
					OdDgElementIteratorPtr pCellIter = pCell3d->createIterator();

					markSharedCells(pCellIter, arrCellIds, arrCellUsage, pDb);
				}
			}

      //

      OdArray<OdDgHatchPatternPtr> arrSymbolHatches;

      if( pElm->isKindOf(OdDgMultiline::desc()) )
      {
        OdDgMultilinePtr pMultiline = pElm;

        for( OdUInt32 iPattern = 0; iPattern < pMultiline->getHatchPatternCount(); iPattern++ )
        {
          OdDgHatchPatternPtr pHatchPattern = pMultiline->getHatchPattern( iPattern );

          if( !pHatchPattern.isNull() && (pHatchPattern->getType() == OdDgHatchPattern::kSymbolPattern) )
            arrSymbolHatches.push_back( pHatchPattern );
        }
      }
      else if( pElm->isKindOf( OdDgGraphicsElement::desc()) )
      {
        OdDgGraphicsElementPtr pGrElm = pElm;

        OdDgHatchPatternPtr pHatchPattern = pGrElm->getHatchPattern();

        if( !pHatchPattern.isNull() && (pHatchPattern->getType() == OdDgHatchPattern::kSymbolPattern) )
          arrSymbolHatches.push_back( pHatchPattern );
      }

      for( OdUInt32 iHatch = 0; iHatch < arrSymbolHatches.size(); iHatch++ )
      {
        OdDgSymbolHatchPatternPtr pSymbolHatch = arrSymbolHatches[iHatch];

        OdUInt64 uSymbolId = pSymbolHatch->getSymbolId();

        if( uSymbolId != 0 )
        {
          OdDgElementId idElm = pDb->getElementId(OdDbHandle(uSymbolId));

          if (!idElm.isNull())
          {
            OdDgElementPtr pDefElm = idElm.openObject(OdDg::kForRead);

            if (!pDefElm.isNull() && pDefElm->isKindOf(OdDgSharedCellDefinition::desc()))
            {
              arrCells.push_back(idElm);
            }
          }
        }
      }

      //

			for (OdUInt32 i = 0; i < arrCells.size(); i++)
			{
				for (OdUInt32 j = 0; j < arrCellIds.size(); j++)
				{
					if (arrCells[i] == arrCellIds[j])
					{
						arrCellUsage[j] = true;
						break;
					}
				}
			}
		}
	}
}

/************************************************************************/
/* Explode commands.                                                    */
/************************************************************************/

void Cmd_ERASE::execute(OdEdCommandContext* pCmdCtx)
{
	OdDgCommandContextPtr pDgCmdCtx(pCmdCtx);
	OdDgDatabasePtr pDb = pDgCmdCtx->database();
	OdDgUserIO* pIO = pDgCmdCtx->dbUserIO();

	OdDgElementIdArray entIds = Cmd_SELECT::selectEntities(pIO, L"Select entity to erase");

	for (OdUInt32 i = 0; i < entIds.size(); i++)
	{
		if (!entIds[i].isNull())
		{
			OdDgElementPtr pElm = entIds[i].openObject(OdDg::kForWrite);

			if (!pElm.isNull())
			{
				pElm->erase(true);
			}
		}
	}
}

void Cmd_Scale2::execute(OdEdCommandContext* pCmdCtx)
{
  OdDgCommandContextPtr pDgCmdCtx(pCmdCtx);
  OdDgDatabasePtr pDb = pDgCmdCtx->database();
  OdDgUserIO* pIO = pDgCmdCtx->dbUserIO();

  OdDgElementIdArray entIds = Cmd_SELECT::selectEntities(pIO, L"Select entity to erase");

  for (OdUInt32 i = 0; i < entIds.size(); i++)
  {
    if (!entIds[i].isNull())
    {
      OdDgElementPtr pElm = entIds[i].openObject(OdDg::kForWrite);

      if (!pElm.isNull())
      {
        OdGeExtents3d ext;
        pElm->getGeomExtents(ext);
        pElm->transformBy(OdGeMatrix3d::scaling(2, ext.center()));
      }
    }
  }
}

void Cmd_RotateXY30::execute(OdEdCommandContext* pCmdCtx)
{
  OdDgCommandContextPtr pDgCmdCtx(pCmdCtx);
  OdDgDatabasePtr pDb = pDgCmdCtx->database();
  OdDgUserIO* pIO = pDgCmdCtx->dbUserIO();

  OdDgElementIdArray entIds = Cmd_SELECT::selectEntities(pIO, L"Select entity to erase");

  for (OdUInt32 i = 0; i < entIds.size(); i++)
  {
    if (!entIds[i].isNull())
    {
      OdDgElementPtr pElm = entIds[i].openObject(OdDg::kForWrite);

      if (!pElm.isNull())
      {
        OdGeExtents3d ext;
        pElm->getGeomExtents(ext);
        pElm->transformBy(OdGeMatrix3d::rotation(OdaPI/6, OdGeVector3d::kZAxis, ext.center()));
      }
    }
  }
}

void Cmd_UNDO::execute(OdEdCommandContext* pCmdCtx)
{
	OdDgCommandContextPtr pDgCmdCtx(pCmdCtx);
	OdEdUserIO* pIO = pDgCmdCtx->userIO();
	OdDgDatabasePtr pDb = pDgCmdCtx->database();

	OdString sKwds = L"Auto Control Begin End Mark Back";

	int opt = -1;
	try
	{
		int nSteps = pIO->getInt(L"Enter the number of operations to undo or [Auto/Control/BEgin/End/Mark/Back] <1>:", 0, 1, sKwds);
		if (pDb->hasUndo())
		{
			while (nSteps--)
			{
				pDb->undo();
			}
			if (!pDb->hasUndo())
				pIO->putString("Everything has been undone");
		}
		else
		{
			pIO->putString("Nothing to undo");
		}
	}
	catch (const OdEdKeyword& kw)
	{
		opt = kw.keywordIndex();
	}

	switch (opt)
	{
	case -1:// empty input
		break;
	case 0: // Auto
		if (pIO->getKeyword("Enter UNDO Auto mode [ON/OFF]:", "ON OFf", 0) == 0)
		{
		}
		else
		{
		}
		break;
	case 1: // Control
		switch (pIO->getKeyword("Enter an UNDO control option [All/None/One/Combine] <All>:", "All None One Combine"))
		{
		case 0: // All
			break;
		case 1: // None
			break;
		case 2: // One
			break;
		case 3: // Combine
			if (pIO->getKeyword("Enter UNDO Auto mode [ON/OFF] <%ls>:", "Yes No") == 0)
			{
			}
			else
			{
			}
			break;
		}
		break;
	case 2: // BEgin
		if (pDb->isUndoBlockStarted())
			pDb->blockUndoRecording(false);
		pDb->startUndoRecord();
		pDb->blockUndoRecording(true);
		break;
	case 3: // End
		if (pDb->isUndoBlockStarted())
			pDb->blockUndoRecording(false);
		break;
	case 4: // Mark
		pDb->startUndoRecord();
		pDb->setUndoMark();
		break;
	case 5: // Back
		if (pDb->hasUndoMark() || pIO->getKeyword("This will undo everything. OK? <Y>", "Yes No", 0) == 0)
		{
			pDb->undoBack();
		}
		break;
	}
}

void Cmd_LINE::execute(OdEdCommandContext* pCmdCtx)
{
	OdDgCommandContextPtr pDgCmdCtx(pCmdCtx);
	OdDgDatabasePtr pDb = pDgCmdCtx->database();
	OdDgUserIO* pIO = pDgCmdCtx->dbUserIO();

	OdString strNext_U(L"Specify next point:");

	OdString strNext_CU(L"Specify next point or [Close]:");
	OdString strKeywords_CU(L"Close");
	OdGePoint3d ptFirst, ptStart, ptEnd;
	int nLines = 0;

	for (bool bFinished = false; !bFinished;)
	{
		ptStart = ptFirst = pIO->getPoint(L"Specify first point:");

		for (;;)
		{
			try
			{
				if (nLines >= 2)
				{
					ptEnd = pIO->getPoint(strNext_CU, OdEd::kGptRubberBand, NULL, strKeywords_CU);
				}
				else
				{
					ptEnd = pIO->getPoint(strNext_U, OdEd::kGptRubberBand, NULL);
				}
			}
			catch (const OdEdKeyword &kwd)
			{
				if (kwd.keywordIndex() == 0)
				{ //Close
					ptEnd = ptFirst;
					bFinished = true;
				}
				else
				{ //Undo
// 					if (nLines == 0)
// 					{ //Undo first point
// 						break;
// 					}
// 					OdDbObjectId idLast = oddbEntLast(pDb);
// 					OdDbLinePtr pLine = idLast.openObject(OdDg::kForWrite);
// 					if (!pLine.isNull())
// 					{
// 						ptStart = pLine->startPoint();
// 						pLine->erase();
// 						pIO->setLASTPOINT(ptStart);
// 						--nLines;
// 					}
// 					else
// 					{
// 						ODA_FAIL();
// 					}
// 					continue;
				}
			}
			catch (const OdEdCancel&)
			{
				return;
			}

			OdDgLine3dPtr pNewLine = OdDgLine3d::createObject();
			pNewLine->setDatabaseDefaults(pDb);
			//			pNewLine->setNormal(vNormal);
			pNewLine->setStartPoint(ptStart);
			pNewLine->setEndPoint(ptEnd);
			OdDgElementId ownerId = pDb->getActiveModelId();
			OdDgModelPtr pOwner = ownerId.safeOpenObject(OdDg::kForWrite);
			pOwner->addElement(pNewLine);

			pNewLine->setLineWeight(pDb->getActiveLineWeight());
			pNewLine->setLineStyleEntryId(pDb->getActiveLineStyleEntryId());
			pNewLine->setColorIndex(pDb->getActiveColorIndex());

			ptStart = ptEnd;
			++nLines;

			if (bFinished)
				return;
		}
	}
}

void Cmd_CIRCLE::execute(OdEdCommandContext* pCmdCtx)
{
	OdDgCommandContextPtr pDgCmdCtx(pCmdCtx);
	OdDgDatabasePtr pDb = pDgCmdCtx->database();
	OdDgModelPtr pModel = pDb->getActiveModelId().safeOpenObject(OdDg::kForWrite);
	OdDgUserIO* pIO = pDgCmdCtx->dbUserIO();

	OdDgEllipse3dPtr pCircle = OdDgEllipse3d::createObject();
	OdGePoint3d ptCenter;
	pCircle->setDatabaseDefaults(pDb);

	int nVar = -1;
	try
	{
		ptCenter = pIO->getPoint(L"Specify circle center point or [3P/2P]: ",
			OdEd::kGptDefault, NULL, L"3P 2P");
	}
	catch (const OdEdKeyword &kwd)
	{
		nVar = kwd.keywordIndex();
	}

	if (nVar == -1) {

		pCircle->setOrigin(ptCenter);
		struct CircleTracker : OdStaticRxObject<OdEdRealTracker>
		{
		private:
			OdDgEllipse3d* m_pCircle;
			OdGePoint3d m_pCenter;
			OdDgText3dPtr m_pText;

		public:
			CircleTracker(OdDgEllipse3d *circle, OdGePoint3d center)
				: m_pCircle(circle)
				, m_pCenter(center)
			{
				m_pText = OdDgText3d::createObject();
				m_pText->setDatabaseDefaults(m_pCircle->database());
				m_pText->setColorIndex(2);
			}

			virtual void setValue(double value)
			{
				m_pCircle->setPrimaryAxis(value);
				m_pCircle->setSecondaryAxis(value);

				OdString str;
				str.format(L"%.3f", value);
				m_pText->setText(str);
				m_pText->setPosition(m_pCenter);
			}

			virtual int addDrawables(OdGsView* pView)
			{
				pView->add(m_pCircle, 0);

				OdGePoint2d pixelDensity;
				pView->getNumPixelsInUnitSquare(OdGePoint3d(), pixelDensity);
				m_pText->setHeight(10);
				m_pText->setHeightMultiplier(10. / pixelDensity.x);
				m_pText->setLengthMultiplier(10. / pixelDensity.x);

				pView->add(m_pText, 0);

				return 1;
			}

			virtual void removeDrawables(OdGsView* pView)
			{
				pView->erase(m_pCircle);
				pView->erase(m_pText);
			}
		}
		circleTracker(pCircle, ptCenter);

		double radius = pIO->getDist(L"Specify radius of circle ",
			OdEd::kGdsFromLastPoint | OdEd::kGptRubberBand /*| OdEd::kInpThrowEmpty*/,
			1., OdString::kEmpty, &circleTracker);

		//if tracker is not supported
		pCircle->setPrimaryAxis(radius);
		pCircle->setSecondaryAxis(radius);
	}
	else if (nVar == 0) {
		OdGePoint3d pt1, pt2;
		pt1 = pIO->getPoint(L"Specify first point on circle");
		pt2 = pIO->getPoint(L"Specify second point on circle",
			OdEd::kGdsFromLastPoint | OdEd::kGptRubberBand);

		struct Cr3PointTracker : OdStaticRxObject<OdEdPointTracker>
		{
			OdGePoint3d m_pt1;
			OdGePoint3d m_pt2;
			OdDgEllipse3d* m_pCircle;
			OdDgText3dPtr m_pText;

			Cr3PointTracker(OdDgEllipse3d* pCircle, OdGePoint3d point1, OdGePoint3d point2)
				: m_pCircle(pCircle)
				, m_pt1(point1)
				, m_pt2(point2)
			{
				m_pText = OdDgText3d::createObject();
				m_pText->setDatabaseDefaults(m_pCircle->database());
				m_pText->setColorIndex(2);
			}

			virtual void setValue(const OdGePoint3d& pt3)
			{
				// Check if arc changed direction. Normal should not be flipped.
				OdGeVector3d v1 = m_pt2 - m_pt1;
				OdGeVector3d v2 = pt3 - m_pt2;
				OdGeVector3d vNewNormal = v1.crossProduct(v2);
				if (vNewNormal == OdGeVector3d::kIdentity)
					return;
				OdGeCircArc3d geArc;
				geArc.set(m_pt1, m_pt2, pt3);

				geArc.setAngles(0., Oda2PI);
				m_pCircle->setFromOdGeCurve(geArc);

				OdGePoint3d center;
				m_pCircle->getOrigin(center);
				OdGeVector3d vC = (m_pt1 - center);
				double dRadius = vC.length();
				OdString str;
				str.format(L"%.3f", dRadius);
				m_pText->setText(str);
				m_pText->setJustification(OdDg::kRightBottom);
				m_pText->setPosition(pt3);

			}
			virtual int addDrawables(OdGsView* pView)
			{
				pView->add(m_pCircle, 0);

				OdGePoint2d pixelDensity;
				pView->getNumPixelsInUnitSquare(OdGePoint3d(), pixelDensity);
				m_pText->setHeight(10);
				m_pText->setHeightMultiplier(10. / pixelDensity.x);
				m_pText->setLengthMultiplier(10. / pixelDensity.x);

				pView->add(m_pText, 0);
				return 1;
			}

			virtual void removeDrawables(OdGsView* pView)
			{
				pView->erase(m_pCircle);
				pView->erase(m_pText);
			}
		}
		tracker(pCircle, pt1, pt2);

		pIO->getPoint(L"Specify third point on circle",
			OdEd::kGdsFromLastPoint | OdEd::kGptRubberBand,
			NULL, OdString::kEmpty, &tracker);
	}
	else if (nVar == 1) {
		OdGePoint3d pt1;
		pt1 = pIO->getPoint(L"Specify first end point of circle's diameter: ");

		struct Cr3PointTracker : OdStaticRxObject<OdEdPointTracker>
		{
			OdGePoint3d m_pt1;
			OdDgEllipse3d* m_pCircle;
			OdDgText3dPtr m_pText;

			Cr3PointTracker(OdDgEllipse3d* pCircle, OdGePoint3d point1)
				: m_pCircle(pCircle)
				, m_pt1(point1)
			{
				m_pText = OdDgText3d::createObject();
				m_pText->setDatabaseDefaults(m_pCircle->database());
				m_pText->setColorIndex(2);
			}

			virtual void setValue(const OdGePoint3d& pt2)
			{
				OdGeVector3d vC = (pt2 - m_pt1) / 2.;
				double dRadius = vC.length();
				if (!OdZero(dRadius))
				{
					m_pCircle->setOrigin(m_pt1 + vC);
					m_pCircle->setPrimaryAxis(vC.length());
					m_pCircle->setSecondaryAxis(vC.length());
				}

				OdString str;
				str.format(L"%.3f", vC.length());
				m_pText->setText(str);
				m_pText->setPosition(m_pt1 + vC);
			}
			virtual int addDrawables(OdGsView* pView)
			{
				pView->add(m_pCircle, 0);
				OdGePoint2d pixelDensity;
				pView->getNumPixelsInUnitSquare(OdGePoint3d(), pixelDensity);
				m_pText->setHeight(10);
				m_pText->setHeightMultiplier(10. / pixelDensity.x);
				m_pText->setLengthMultiplier(10. / pixelDensity.x);

				pView->add(m_pText, 0);

				return 1;
			}

			virtual void removeDrawables(OdGsView* pView)
			{
				pView->erase(m_pCircle);
				pView->erase(m_pText);
			}
		}
		tracker(pCircle, pt1);

		pIO->getPoint(L"Specify second end point of circle's diameter: ",
			OdEd::kGdsFromLastPoint | OdEd::kGptRubberBand,
			NULL, OdString::kEmpty, &tracker);
	}
	else {
		ODA_FAIL();
	}
	pModel->addElement(pCircle);

	pCircle->setLineWeight(pDb->getActiveLineWeight());
	pCircle->setLineStyleEntryId(pDb->getActiveLineStyleEntryId());
	pCircle->setColorIndex(pDb->getActiveColorIndex());

}

void Cmd_TESTUNDOPL::execute(OdEdCommandContext* pCmdCtx)
{
	OdDgCommandContextPtr pDgCmdCtx(pCmdCtx);
	OdDgDatabasePtr pDb = pDgCmdCtx->database();
	OdDgModelPtr pModel = pDb->getActiveModelId().safeOpenObject(OdDg::kForWrite);
	OdDgUserIO* pIO = pDgCmdCtx->dbUserIO();

	OdDgElementIteratorPtr modelIterator = pModel->createGraphicsElementsIterator();
	OdDgGraphicsElementPtr existingEntity = modelIterator->item().safeOpenObject(OdDg::kForWrite);
	if (existingEntity->isKindOf(OdDgComplexString::desc()))
	{
		OdDgComplexStringPtr complexString(existingEntity);

		{
			OdDgLine3dPtr firstInsertion = OdDgLine3d::createObject();
			firstInsertion->setDatabaseDefaults(pDb);
			firstInsertion->setStartPoint(OdGePoint3d(0,0,0));
			firstInsertion->setEndPoint(OdGePoint3d(1,1,0));
			complexString->insertItem(1, firstInsertion);

			OdDgLine3dPtr secondInsertion = OdDgLine3d::createObject();
			secondInsertion->setDatabaseDefaults(pDb);
			secondInsertion->setStartPoint(OdGePoint3d(0,0,0));
			secondInsertion->setEndPoint(OdGePoint3d(1,1,0));
			complexString->insertItem(2, secondInsertion);
		}
	}
}

void Cmd_ARC::execute(OdEdCommandContext* pCmdCtx)
{
	OdDgCommandContextPtr pDgCmdCtx(pCmdCtx);
	OdDgDatabasePtr pDb = pDgCmdCtx->database();
	OdDgModelPtr pModel = pDb->getActiveModelId().safeOpenObject(OdDg::kForWrite);
	OdDgUserIO* pIO = pDgCmdCtx->dbUserIO();

	OdDgArc3dPtr pArc3d = OdDgArc3d::createObject();
	pArc3d->setDatabaseDefaults(pDb);
	OdGePoint3d ptCenter;

	int nVar = -1;
	try
	{
		ptCenter = pIO->getPoint(L"Specify center point for arc or [3P]: ",
			OdEd::kGptDefault, NULL, L"3P");
	}
	catch (const OdEdKeyword &kwd)
	{
		nVar = kwd.keywordIndex();
	}

	if (nVar == -1)
	{
		pArc3d->setOrigin(ptCenter);

		double dPrimAxis = pIO->getDist(L"Specify major radius: ",
			OdEd::kGdsFromLastPoint | OdEd::kGptRubberBand, 1000);
		pArc3d->setPrimaryAxis(dPrimAxis);

		struct CircleTracker : OdStaticRxObject<OdEdRealTracker>
		{
		private:
			OdDgArc3d * m_pCircle;
			OdGePoint3d m_pCenter;
			double m_pRadius1;
			OdDgText3dPtr m_pText;

		public:
			CircleTracker(OdDgArc3d * arc, OdGePoint3d point, double radius)
				: m_pCircle(arc)
				, m_pCenter(point)
				, m_pRadius1(radius)
			{
				m_pText = OdDgText3d::createObject();
				m_pText->setDatabaseDefaults(m_pCircle->database());
				m_pText->setColorIndex(2);
			}

			virtual void setValue(double value)
			{
				m_pCircle->setPrimaryAxis(m_pRadius1);
				m_pCircle->setSecondaryAxis(value);
				m_pCircle->setStartAngle(0);
				m_pCircle->setSweepAngle(7);
			}

			virtual int addDrawables(OdGsView* pView)
			{
				pView->add(m_pCircle, 0);
				return 1;
			}

			virtual void removeDrawables(OdGsView* pView)
			{
				pView->erase(m_pCircle);
			}
		}
		tracker(pArc3d, ptCenter, dPrimAxis);

		double dSecAxis = pIO->getDist(L"Specify minor radius: ",
			OdEd::kGdsFromLastPoint | OdEd::kGptRubberBand, dPrimAxis,
			OdString::kEmpty, &tracker);
		pArc3d->setSecondaryAxis(dSecAxis);

		double dStartAngle = pIO->getAngle(L"Specify start angle: ");
		pArc3d->setStartAngle(dStartAngle);

		double dSweepAngle = pIO->getAngle(L"Specify sweep angle: ");
		pArc3d->setSweepAngle(dSweepAngle);
	}
	else if (nVar == 0) {
		OdGeCircArc3d geArc;

		ptCenter = pIO->getPoint(L"Specify first arc point");
		OdGePoint3d ptSecond = pIO->getPoint(L"Specify second arc point",
			OdEd::kGdsFromLastPoint | OdEd::kGptRubberBand);

		struct Cr3PointTracker : OdStaticRxObject<OdEdPointTracker>
		{
			OdGePoint3d m_pt1;
			OdGePoint3d m_pt2;
			OdDgArc3d* m_pArc3d;
			OdDgText3dPtr m_pText;

			Cr3PointTracker(OdDgArc3d* pCircle, OdGePoint3d point1, OdGePoint3d point2)
				: m_pArc3d(pCircle)
				, m_pt1(point1)
				, m_pt2(point2)
			{}

			virtual void setValue(const OdGePoint3d& pt3)
			{
				// Check if arc changed direction. Normal should not be flipped.
				OdGeVector3d v1 = m_pt2 - m_pt1;
				OdGeVector3d v2 = pt3 - m_pt2;
				OdGeVector3d vNewNormal = v1.crossProduct(v2);
				if (vNewNormal == OdGeVector3d::kIdentity)
					return;
				OdGeCircArc3d geArc;
				geArc.set(m_pt1, m_pt2, pt3);

				m_pArc3d->setFromOdGeCurve(geArc);
			}
			virtual int addDrawables(OdGsView* pView)
			{
				pView->add(m_pArc3d, 0);
				return 1;
			}

			virtual void removeDrawables(OdGsView* pView)
			{
				pView->erase(m_pArc3d);
			}
		}
		tracker(pArc3d, ptCenter, ptSecond);

		OdGePoint3d ptThird = pIO->getPoint(L"Specify third point on circle",
			OdEd::kGdsFromLastPoint | OdEd::kGptRubberBand,
			NULL, OdString::kEmpty, &tracker);

		//in case of not supported tracker
		geArc.set(ptCenter, ptSecond, ptThird);
		pArc3d->setFromOdGeCurve(geArc);
	}
	else {
		ODA_FAIL();
	}

	pModel->addElement(pArc3d);

	pArc3d->setLineWeight(pDb->getActiveLineWeight());
	pArc3d->setLineStyleEntryId(pDb->getActiveLineStyleEntryId());
	pArc3d->setColorIndex(pDb->getActiveColorIndex());
}

void Cmd_TEXT::execute(OdEdCommandContext* pCmdCtx)
{
	OdDgCommandContextPtr pDgCmdCtx(pCmdCtx);
	OdDgDatabasePtr pDb = pDgCmdCtx->database();
	OdDgModelPtr pModel = pDb->getActiveModelId().safeOpenObject(OdDg::kForWrite);
	OdDgUserIO* pIO = pDgCmdCtx->dbUserIO();

	OdGePoint3d ptPosition;
	int nKeyIndex;
	OdDgElementId styleId;

	OdDgText3dPtr pText3d = OdDgText3d::createObject();
	pText3d->setDatabaseDefaults(pDb);

	for (;;) // While point or "Justify" option are not specified
	{
		nKeyIndex = -1;
		try
		{
			ptPosition = pIO->getPoint(L"Specify start point of text or [Justify/Style]: ", OdEd::kGptDefault,
				NULL, L"Justify Style");
		}
		catch (const OdEdKeyword &kwd)
		{
			nKeyIndex = kwd.keywordIndex();
		}

		if (nKeyIndex != 1)
		{ // Point was specified or Justification is to be set
			break;
		}

		OdDgTextStyleTablePtr pTextStyleTable = pModel->database()->getTextStyleTable();
		OdDgElementIteratorPtr pIter = pTextStyleTable->createIterator();
		OdDgTextStyleTableRecordPtr style;
		OdString styleNames = L"Enter style name or [?]: ";
		for (;;)
		{
			OdString sName = pIO->getString(styleNames);

			if (sName.isEmpty())
				break;           // user pressed Enter, i.e. has left default Style

			if (sName == L"?") {
				styleNames = L"Text styles: ";
				for (pIter->start(); !pIter->done(); pIter->step())
				{
					style = pIter->item().safeOpenObject();
					styleNames += L"," + style->getName();
				}
				pIO->putString(styleNames);
			}
			else
			{
				for (pIter->start(); !pIter->done(); pIter->step())
				{
					style = pIter->item().safeOpenObject();
					if (sName == style->getName())
					{
						styleId = style->elementId();
					}
				}
				if (!styleId.isNull())
					break;
			}
		}
	}

	if (nKeyIndex == 0)
	{ // Justification
		nKeyIndex = pIO->getKeyword(L"Enter an option [LT/LC/LB/LMT/LMC/LMB/CT/CC/CB/RMT/RMC/RMB/RT/RC/RB/LD/CD/RD]: ",
			L"LT LC LB LMT LMC LMB CT CC CB RMT RMC RMB RT RC RB LD CD RD");
	}

	const OdChar* strFirstPoint = L"Specify point of text base line: ";
	OdDg::TextJustification just;

	switch (nKeyIndex)
	{
	case -1:  // Default
		just = OdDg::kLeftBottom;
		break;

	case 0:   // LeftTop
		just = OdDg::kLeftTop;
		ptPosition = (pIO->getPoint(strFirstPoint));
		break;

	case 1:   // LeftCenter
		just = OdDg::kLeftCenter;
		ptPosition = (pIO->getPoint(strFirstPoint));
		break;

	case 2: // LeftBottom
		just = OdDg::kLeftBottom;
		ptPosition = (pIO->getPoint(strFirstPoint));
		break;

	case 3: // kLeftMarginTop
		just = OdDg::kLeftMarginTop;
		ptPosition = (pIO->getPoint(strFirstPoint));
		break;

	case 4: // kLeftMarginCenter
		just = OdDg::kLeftMarginCenter;
		ptPosition = (pIO->getPoint(strFirstPoint));
		break;

	case 5: //kLeftMarginBottom
		just = OdDg::kLeftMarginBottom;
		ptPosition = (pIO->getPoint(strFirstPoint));
		break;

	case 6: // kCenterTop
		just = OdDg::kCenterTop;
		ptPosition = (pIO->getPoint(strFirstPoint));
		break;

	case 7: // kCenterCenter
		just = OdDg::kCenterCenter;
		ptPosition = (pIO->getPoint(strFirstPoint));
		break;

	case 8: // kCenterBottom
		just = OdDg::kCenterBottom;
		ptPosition = (pIO->getPoint(strFirstPoint));
		break;

	case 9: // kRightMarginTop
		just = OdDg::kRightMarginTop;
		ptPosition = (pIO->getPoint(strFirstPoint));
		break;

	case 10: // kRightMarginCenter
		just = OdDg::kRightMarginCenter;
		ptPosition = (pIO->getPoint(strFirstPoint));
		break;

	case 11: //kRightMarginBottom
		just = OdDg::kRightMarginBottom;
		ptPosition = (pIO->getPoint(strFirstPoint));
		break;

	case 12: //kRightTop
		just = OdDg::kRightTop;
		ptPosition = (pIO->getPoint(strFirstPoint));
		break;

	case 13: //kRightCenter
		just = OdDg::kRightCenter;
		ptPosition = (pIO->getPoint(strFirstPoint));
		break;

	case 14: //kRightBottom
		just = OdDg::kRightBottom;
		ptPosition = (pIO->getPoint(strFirstPoint));
		break;

	case 15: //kLeftDescender
		just = OdDg::kLeftDescender;
		ptPosition = (pIO->getPoint(strFirstPoint));
		break;

	case 16: //kCenterDescender
		just = OdDg::kCenterDescender;
		ptPosition = (pIO->getPoint(strFirstPoint));
		break;

	case 17: //kRightDescender
		just = OdDg::kRightDescender;
		ptPosition = (pIO->getPoint(strFirstPoint));
		break;
	}

	//height
	double dH = 1;
	OdString strHeightPrompt(L"Specify height <1.0>: ");
	double dHeight = pIO->getDist(strHeightPrompt,
		OdEd::kGdsFromLastPoint | OdEd::kGptRubberBand, dH);
	pText3d->setHeight(dH);
	pText3d->setHeightMultiplier(dHeight);
	pText3d->setLengthMultiplier(dHeight);

	//angle
	double dAngle = pIO->getAngle(L"Specify rotation angle of text <0>: ",
		OdEd::kGanFromLastPoint | OdEd::kGptRubberBand, 0.);
	OdGeMatrix3d matRotation = OdGeMatrix3d::rotation(dAngle, OdGeVector3d::kZAxis);
	OdGeQuaternion quatern;
	quatern.set(matRotation);
	pText3d->setRotation(quatern);

	struct StringTracker : OdStaticRxObject<OdEdStringTracker>
	{
	private:
		OdDgText3d*     m_pText;
		OdDg::TextJustification m_just;
		OdGePoint3d m_pPos;
		OdDgElementId	m_styleId;
		double m_dH;
		double m_dHeight;

	public:
		StringTracker(OdDgText3d* pText, OdDg::TextJustification just,
			OdGePoint3d pos, OdDgElementId styleId, double dH, double dHeight)
			: m_pText(pText)
			, m_just(just)
			, m_pPos(pos)
			, m_styleId(styleId)
			, m_dH(dH)
			, m_dHeight(dHeight)
		{}

		virtual void setValue(const OdString& value)
		{
			m_pText->setText(value);
			if (!m_styleId.isNull()) {
				m_pText->applyTextStyle(m_styleId);
			}
			m_pText->setHeight(m_dH);
			m_pText->setHeightMultiplier(m_dHeight);
			m_pText->setLengthMultiplier(m_dHeight);
			m_pText->setJustification(m_just);
			m_pText->setPosition(m_pPos);
		}

		virtual int addDrawables(OdGsView* pView)
		{
			pView->add(m_pText, 0);
			return 1;
		}

		virtual void removeDrawables(OdGsView* pView)
		{
			pView->erase(m_pText);
		}
	}
	stracker(pText3d, just, ptPosition, styleId, dH, dHeight);

	pModel->addElement(pText3d);

	OdString strRes = pIO->getString(L"Specify text string: ",
		OdEd::kGstAllowSpaces | OdEd::kGstNoEmpty,
		OdString::kEmpty, OdString::kEmpty, &stracker);

	pText3d->setLineWeight(pDb->getActiveLineWeight());
	pText3d->setLineStyleEntryId(pDb->getActiveLineStyleEntryId());
	pText3d->setColorIndex(pDb->getActiveColorIndex());

}

void Cmd_CONE::execute(OdEdCommandContext* pCmdCtx)
{
	OdDgCommandContextPtr pDgCmdCtx(pCmdCtx);
	OdDgDatabasePtr pDb = pDgCmdCtx->database();
	OdDgModelPtr pModel = pDb->getActiveModelId().safeOpenObject(OdDg::kForWrite);
	OdDgUserIO* pIO = pDgCmdCtx->dbUserIO();

	OdDgConePtr pCone = OdDgCone::createObject();
	pCone->setDatabaseDefaults(pDb);
	OdGePoint3d ptCenter1, ptCenter2;
	pCone->setConeType(OdDgCone::kCylinder);

	ptCenter1 = pIO->getPoint(L"Specify center 1 for cone: ",
		OdEd::kInpThrowEmpty);
	pCone->setCenter1(ptCenter1);

	struct CircleTracker : OdStaticRxObject<OdEdRealTracker>
	{
	private:
		OdDgEllipse3dPtr m_pCircle;
		OdDgCone * m_pCone;
		OdGePoint3d m_pCenter;

	public:
		CircleTracker(OdDgCone*cone, OdGePoint3d center)
			: m_pCone(cone)
			, m_pCenter(center)
		{
			m_pCircle = OdDgEllipse3d::createObject();
			m_pCircle->setDatabaseDefaults(m_pCone->database());
			m_pCircle->setOrigin(m_pCenter);
		}

		virtual void setValue(double value)
		{
			m_pCircle->setPrimaryAxis(value);
			m_pCircle->setSecondaryAxis(value);
		}

		virtual int addDrawables(OdGsView* pView)
		{
			pView->add(m_pCircle, 0);
			return 1;
		}

		virtual void removeDrawables(OdGsView* pView)
		{
			pView->erase(m_pCircle);
		}
	};

	CircleTracker cTracker(pCone, ptCenter1);

	double dRadius1 = pIO->getDist(L"Specify radius 1 ",
		OdEd::kGdsFromLastPoint | OdEd::kGptRubberBand,
		1., OdString::kEmpty, &cTracker);

	pCone->setRadius1(dRadius1);

	ptCenter2 = pIO->getPoint(L"Specify center 2 for cone: ",
		OdEd::kInpThrowEmpty);
	pCone->setCenter2(ptCenter2);

	CircleTracker cTracker2(pCone, ptCenter2);

	double dRadius2 = pIO->getDist(L"Specify radius 2 ",
		OdEd::kGdsFromLastPoint | OdEd::kGptRubberBand,
		1., OdString::kEmpty, &cTracker2);
	pCone->setRadius2(dRadius2);

	pModel->addElement(pCone);

	pCone->setLineWeight(pDb->getActiveLineWeight());
	pCone->setLineStyleEntryId(pDb->getActiveLineStyleEntryId());
	pCone->setColorIndex(pDb->getActiveColorIndex());
}

void Cmd_REDUCE::execute(OdEdCommandContext* pCmdCtx)
{
	OdDgCommandContextPtr pDgCmdCtx(pCmdCtx);
	OdDgDatabasePtr pDb = pDgCmdCtx->database();
	OdDgElementIdArray ia = pDgCmdCtx->dbUserIO()->select(NULL, L"Select entities to reduce to", OdEd::kSelAllowInactSpaces)->elementIdArray();
	if (ia.isEmpty())
		return;
	OdDgModelPtr m = OdDgModel::cast(OdDgElementPtr(ia[0].safeOpenObject())->ownerId().safeOpenObject(OdDg::kForWrite));
	if (!m.isNull())
	{
		for (OdDgElementIteratorPtr pIter = m->createGraphicsElementsIterator(); !pIter->done(); pIter->step())
		{
			if (ia.contains(pIter->item()))
				continue;
			OdDgElementPtr(pIter->item().openObject(OdDg::kForWrite))->erase(true);
		}
	}
}

void Cmd_ERASE_Invisible::execute(OdEdCommandContext* pCmdCtx)
{
  OdDgCommandContextPtr pDgCmdCtx(pCmdCtx);
  OdDgDatabasePtr pDb = pDgCmdCtx->database();

  OdDgElementIteratorPtr pIter = pDb->getModelTable(OdDg::kForWrite)->createIterator();

  for (; !pIter->done(); pIter->step())
  {
    OdDgModelPtr pModel = pIter->item().openObject(OdDg::kForWrite);

    OdDgElementIteratorPtr pModelIter = pModel->createGraphicsElementsIterator();

    for(; !pModelIter->done(); pModelIter->step())
    {
      OdDgElementPtr pElm = pModelIter->item().openObject(OdDg::kForWrite);

      if( !pElm.isNull() && pElm->isKindOf(OdDgGraphicsElement::desc()))
      {
        OdDgGraphicsElementPtr pGrElm = pElm;

        if (pGrElm->getInvisibleFlag())
          pGrElm->erase(true);
      }
    }
  }
}

///////////////////////////////////////////////////////////////////
//  Implementation of "SELECT" command
///////////////////////////////////////////////////////////////////

const OdString Cmd_SELECT::groupName() const { return L"ODADGNAPP"; }

const OdString Cmd_SELECT::name() { return L"SELECT"; }

const OdString Cmd_SELECT::globalName() const { return name(); }

void Cmd_SELECT::execute(OdEdCommandContext* pCmdCtx)
{
	OdDgCommandContextPtr pDgCmdCtx(pCmdCtx);
	OdDgDatabaseDoc* pDg = (OdDgDatabaseDoc*)pDgCmdCtx->database();
	COdaDgnAppDoc *pDoc = pDg->document();
	COdaDgnAppVectorizer* pVectorizer = pDoc->getVectorizer();
	if (pVectorizer == NULL)
	{
		throw OdEdCancel();
	}
	pDoc->OnEditClearselection();
	pDoc->UpdateAllViews(NULL);
	OdDgUserIO* pIO = pDgCmdCtx->dbUserIO();
	pIO->setPickfirst(0);
	int iOpt = OdEd::kSelLeaveHighlighted | OdEd::kSelAllowEmpty;
	OdDgSelectionSetPtr pSSet;
	try
	{
		OdDgSelectionSetPtr pSSet = pDoc->selectionSet();
		pSSet = pIO->select(pVectorizer->getActiveView(), OdString::kEmpty, iOpt, pSSet);
		//    pSSet = pIO->select(OdString::kEmpty, iOpt, pVectorizer->editorObject().workingSSet());
		//    pVectorizer->editorObject().setWorkingSSet(pSSet);
	}
	catch (const OdError&)
	{
		throw OdEdCancel();
	}
	//  pVectorizer->editorObject().selectionSetChanged();
}

OdDgElementIdArray Cmd_SELECT::selectEntities(OdDgUserIO* pIO, const OdString& strCaption)
{
  OdDgElementIdArray entIds;
  OdDgSelectionSetIteratorPtr it = pIO->select(NULL, strCaption, OdEd::kSelAllowInactSpaces)->newIterator();
  while (!it->done())
  {
    OdDgElementPtr pEnt = OdDgElement::cast(it->objectId().openObject());
    if (pEnt.get())
    {
      entIds.append(it->objectId());
    }
    else
    {
      pIO->putString(L"1 selected object is not an entity - skipped");
    }
    it->next();
  }
  if (entIds.size() == 0)
  {
    pIO->putString(L"Nothing selected");
  }
  return entIds;
}

//#ifdef ENABLE_DWF_EXPORT
//
//typedef void(*ExportDwfFn)(const DwExportParams& pParams);
//
//void ExportDwfWrapper(const DwExportParams& params)
//{
//#ifdef _TOOLKIT_IN_DLL_
//
//	HINSTANCE hinstLib = LoadLibrary(OdString(OdDwf7ExportModuleName) + OdString(TD_DLL_VERSION_SUFFIX_STR) + OdString(_T(".dll")));
//
//	ExportDwfFn exportFn = 0;
//	if (hinstLib != NULL)
//	{
//#ifdef _WIN64
//		exportFn = (ExportDwfFn)GetProcAddress(hinstLib, "?exportDwf@@YAXAEBVDwExportParams@TD_DWF_EXPORT@@@Z");
//#else
//		exportFn = (ExportDwfFn)GetProcAddress(hinstLib, "?exportDwf@@YAXABVDwExportParams@TD_DWF_EXPORT@@@Z");
//#endif
//	}
//	if (exportFn != NULL)
//	{
//		exportFn(params);
//		FreeLibrary(hinstLib);
//	}
//	else
//	{
//		AfxMessageBox(_T("The TD_Dwf7Export.dll missed"));
//	}
//
//#else // #ifdef _TOOLKIT_IN_DLL_
//
//	exportDwf(params);
//
//#endif // #ifdef _TOOLKIT_IN_DLL_
//}
//
//#endif // ENABLE_DWF_EXPORT

void COdaDgnAppDoc::OnExportDwf()
{
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

#ifdef ENABLE_DWF_EXPORT

			//OdGsModulePtr pModule = ::odrxDynamicLinker()->lo->loadModule(OdDwf7ExportModuleName);
			//if (pModule.isNull())
			//  AfxMessageBox( _T("TD_Dwf7Export.tx is missing") );
			//else
	//		{
				// Create output file
				//OdString fileName = dlgFile.GetPathName();
				//OdStreamBufPtr file = odrxSystemServices()->createFile( fileName, Oda::kFileWrite, Oda::kShareDenyNo, Oda::kCreateAlways );
				//if ( !file.isNull() )
				//{
				//}

			DwExportParams params;
			//OdDbBaseDatabasePEPtr pDbPE = database();
			//OdDbObjectPtr pOPtr = m_pDb;
			OdGiContextForDgDatabaseToExportPtr pCtx = OdGiContextForDgDatabaseToExport::createObject();
			pCtx->setDatabase(m_pDb);
			params.setDatabase(m_pDb);
			params.setDwfFileName(OdString(strFileName));

			// MKU 03\01\02 - resolved "white background issue"
			const ODCOLORREF* refColors = OdDgColorTable::currentPalette(m_pDb);
			//params.pPalette = refColors;
			//params.background = refColors[255];
			ODGSPALETTE pPalCpy;
			pPalCpy.insert(pPalCpy.begin(), refColors, refColors + 256);
			// Color with #255 always defines backround.
			//pPalCpy[255] = ODRGB(255, 255, 255);
			OdDgModelPtr pModel = m_pDb->getActiveModelId().safeOpenObject();
			COLORREF background = pModel->getBackground();
			pPalCpy[255] = background;

			// This method should be called to resolve "white background issue" before setting device palette
			bool bCorrected = OdDgColorTable::correctPaletteForWhiteBackground(pPalCpy.asArrayPtr());
			params.setPalette(pPalCpy.asArrayPtr());
			params.setBackground(pPalCpy[255]);


			params.setInkedArea(false);                                                        // MKU 1/21/2004
			params.setColorMapOptimize(false);                                                 // MKU 1/21/2004
			params.setExportInvisibleText(true);
			//params.bForceInitialViewToExtents = true;

			DwfPageData pageData;

			//OdDgElementId pId = m_pDb->getActiveModelId();
			//OdDgModelPtr pModel = OdDgModel::cast(pId.openObject());

			switch (dlgFile.m_ofn.nFilterIndex)
			{
			case 1:
				params.setFormat(DW_UNCOMPRESSED_BINARY);
				params.setVersion(nDwf_v60);

				pageData.sLayout = pModel->getName();//m_pDb->findActiveLayout(true);
				params.pageData().append(pageData);
				break;
			case 2:
				params.setFormat(DW_ASCII);
				params.setVersion(nDwf_v60);
				pageData.sLayout = pModel->getName();//m_pDb->findActiveLayout(true);
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
				pageData.sLayout = pModel->getName();//m_pDb->findActiveLayout(true);
				params.pageData().append(pageData);
				break;
			default:
				ASSERT(0);
				return;
			}
			exporter->exportDwf(params);
#endif
			//		}
		}
	}
}

void COdaDgnAppDoc::OnEditRecentcommand()
{
	ExecuteCommand(theApp.getRecentCmd());
}

void COdaDgnAppDoc::OnUpdateEditRecentcommand(CCmdUI* pCmdUI)
{
	OdString sTxt;
	OdString sRecentCmd = theApp.getRecentCmd();
	OdEdCommandPtr pCmd;
	if (!sRecentCmd.isEmpty())
		pCmd = ::odedRegCmds()->lookupCmd(sRecentCmd.spanExcluding(L" \n"));
	if (pCmd.get())
	{
		pCmdUI->Enable(TRUE);
		sRecentCmd.replace(L"\n", L"\\n");
		sTxt.format(L"ExecuteCommand \"%s\"", sRecentCmd.c_str());
	}
	else
	{
		pCmdUI->Enable(FALSE);
		sTxt = L"Recent command";
	}
	pCmdUI->SetText(sTxt);
}

void COdaDgnAppDoc::OnEditUndo()
{
	//m_bLayoutSwitchable = true;
	m_pDb->undo();
	//m_bLayoutSwitchable = false;
	UpdateAllViews(NULL);
}

void COdaDgnAppDoc::OnUpdateEditUndo(CCmdUI* pCmdUI)
{
	theApp.RefreshCommandMenu();
	pCmdUI->Enable(FALSE);
	pCmdUI->Enable(m_pDb->hasUndo() ? TRUE : FALSE);
}

void COdaDgnAppDoc::OnEditRedo()
{
	//m_bLayoutSwitchable = true;
	m_pDb->redo();
	//m_bLayoutSwitchable = false;
	UpdateAllViews(NULL);
}

void COdaDgnAppDoc::OnUpdateEditRedo(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_pDb->hasRedo() ? TRUE : FALSE);
}

void COdaDgnAppDoc::OnBmpExport()
{
	OdRxModulePtr pModule = ::odrxDynamicLinker()->loadModule(OdRasterExportModuleName);
	if (pModule.isNull())
		AfxMessageBox(OdRasterExportModuleName L"." DEFAULT_MODULE_EXTENSION_W L" is missing");
	else
		ExecuteCommand(OD_T("bmpout\n"));

	//	OdRxModulePtr pModule = ::odrxDynamicLinker()->loadModule(L"ExRasterSaving"); //OdRasterSavingModuleName
	//	if (pModule.isNull())
	//		AfxMessageBox(L"ExRasterSaving" L"." DEFAULT_MODULE_EXTENSION_W L" is missing");
	//	else
	//		ExecuteCommand(OD_T("tg_bmpout\n"));
}

void COdaDgnAppDoc::OnUpdateBmpExport(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

///////////////////////////////////////////////////////////////////
//  Implementation of "Fence" command
///////////////////////////////////////////////////////////////////

const OdString Cmd_Fence::groupName() const
{
  return globalName();
  //return L"ODADGNAPP"; 
}

const OdString Cmd_Fence::globalName() const
{
  return OD_T("Fence");
}

static void create_Fence(OdGePoint3d pt1, OdGePoint3d pt2, OdGsView* pView, OdDgElementId& idModel, OdEdCommandContext* pCmdCtx, bool bFenceType );

void Cmd_Fence::execute(OdEdCommandContext* pCmdCtx)
{
  OdDgCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDgDatabasePtr pDb = pDbCmdCtx->database();
  OdSmartPtr<OdDgUserIO> pIO = pDbCmdCtx->userIO();

  OdDgDatabaseDoc* pDg = (OdDgDatabaseDoc*)pDbCmdCtx->database();
  COdaDgnAppDoc *pDoc = pDg->document();
  COdaDgnAppVectorizer* pVectorizer = pDoc->getVectorizer();

  const OdChar* szKeywords = OD_T("Rectangle Circle");

  OdGsView* pView = pVectorizer->getActiveView();

  OdDgViewPtr pDgView;

  if (!pVectorizer->getViewId().isNull())
    pDgView = pVectorizer->getViewId().openObject(OdDg::kForWrite);

  OdGePoint3d pt1, pt2;

  try
  {
    pt1 = pIO->getPoint(L"Specify corner of window, enter a scale factor (nX or nXP), or\n"
      L"[Rectangle/Circle] <real time>:",
      OdEd::kInpThrowEmpty|OdEd::kInpThrowOther|OdEd::kGptNoOSnap, 0, szKeywords);

    pt1 = pIO->getPoint(OD_T("Specify corner of Fence:"),OdEd::kInpThrowEmpty  | OdEd::kGptNoOSnap);
    pt2 = pIO->getPoint(OD_T("Specify opposite corner of Fence:"), OdEd::kGptNoUCS | OdEd::kGptRectFrame | OdEd::kGptNoOSnap);

    create_Fence(pt1, pt2, pView, pDgView->getModelId(), pCmdCtx, false );
  }
  catch (const OdEdEmptyInput) // real time
  {
    //OdStaticRxObject<RTZoomTracker> tracker;
    for (;;)
    {
      try
      {
        ODA_FAIL();
        //tracker.init(pView, pIO->getPoint(OD_T("Press ESC or ENTER to exit."),
        //  OdEd::kInpThrowEmpty|OdEd::kGptNoUCS|OdEd::kGptBeginDrag|OdEd::kGptNoOSnap));
        //pIO->getPoint(OD_T("Press ESC or ENTER to exit."),
        //  OdEd::kInpThrowEmpty|OdEd::kGptNoUCS|OdEd::kGptEndDrag|OdEd::kGptNoOSnap, 0, OdString::kEmpty, &tracker);
      }
      catch(const OdEdCancel)
      {
        break;
      }
    }
  }
  catch(const OdEdKeyword& kw)
  {
    switch(kw.keywordIndex())
    {
          case 0: // Rectangle
            pt1 = pIO->getPoint(OD_T("Specify corner of Fence:"),OdEd::kInpThrowEmpty  | OdEd::kGptNoOSnap);
            pt2 = pIO->getPoint(OD_T("Specify opposite corner of Fence:"), OdEd::kGptNoUCS | OdEd::kGptRectFrame | OdEd::kGptNoOSnap);

            create_Fence(pt1, pt2, pView, pDgView->getModelId(), pCmdCtx, false );
            break;
          case 1: // Circle
            pt1 = pIO->getPoint(OD_T("Specify center of Fence:"),OdEd::kInpThrowEmpty  | OdEd::kGptNoOSnap);
            pt2 = pIO->getPoint(OD_T("Specify radius Fence:"), OdEd::kGptNoUCS | OdEd::kGptRubberBand | OdEd::kGptNoOSnap);

            create_Fence(pt1, pt2, pView, pDgView->getModelId(), pCmdCtx, true );
            break;
    }
  }

  pDgView = 0;

  //pAVD->setView(pVpObj, pView);
}

void create_Fence(OdGePoint3d pt1, 
                    OdGePoint3d pt2, 
                      OdGsView* pView, 
                        OdDgElementId& idModel, 
                          OdEdCommandContext* pCmdCtx, bool bFenceType )
{
  if( !pView || idModel.isNull() )
    return;

  OdGeVector3d vrXAxis = OdGeVector3d::kXAxis;
  OdGeVector3d vrYAxis = OdGeVector3d::kYAxis;
  OdGeMatrix3d xEyeToWorld = OdAbstractViewPEPtr(pView)->eyeToWorld(pView);
  vrXAxis.transformBy(xEyeToWorld);
  vrYAxis.transformBy(xEyeToWorld);
  vrXAxis.normalize();
  vrYAxis.normalize();

  OdGeVector3d vrNormal = vrXAxis.crossProduct(vrYAxis);

  OdGePlane ucsPlane( pt1, vrNormal );
  pt2 = pt2.project( ucsPlane, vrNormal );

  OdGeLine3d linePt1OrdX( pt1, vrXAxis );
  OdGeLine3d linePt1OrdY( pt1, vrYAxis );
  OdGeLine3d linePt2OrdX( pt2, vrXAxis );
  OdGeLine3d linePt2OrdY( pt2, vrYAxis );

  OdGePoint3d ptIsect1, ptIsect2;
  bool bAllOk = true;

  if( !linePt1OrdY.intersectWith( linePt2OrdX, ptIsect1 ) )
    bAllOk = false;

  if( !linePt2OrdY.intersectWith( linePt1OrdX, ptIsect2 ) )
    bAllOk = false;

  if( bAllOk )
  {
    OdDgModelPtr pModel = idModel.openObject(OdDg::kForWrite);

    if (bFenceType)
    {
      // circle
      OdDgEllipse3dPtr pCircle = OdDgEllipse3d::createObject();
      OdGePoint3d ptCenter = pt1;
      double dRadius = pt1.distanceTo(pt2);

      pCircle->setPrimaryAxis( dRadius );
      pCircle->setSecondaryAxis( dRadius );
      pCircle->setOrigin( ptCenter );
      pCircle->setLineStyleEntryId(2);
      pCircle->setColorIndex(4);

      OdGeMatrix3d matRotate;
      matRotate.setCoordSystem(OdGePoint3d::kOrigin, vrXAxis, vrYAxis, vrNormal);
      OdGeQuaternion quatRotation;
      quatRotation.set(matRotate);

      pCircle->setRotation(quatRotation);
      pModel->addElement( pCircle );
      OdDgResBufPtr pHandle = OdDgResBuf::createObject();
      pHandle->setInt64((OdInt64)pCircle->elementId().getHandle());
      pCmdCtx->setArbitraryData(L"Fence", pHandle );
    } 
    else
    {
      // rectangle
      OdDgLineString3dPtr pLineString = OdDgLineString3d::createObject();
      pLineString->setDatabaseDefaults(pModel->database());
      pLineString->addVertex( pt1 );
      pLineString->addVertex( ptIsect1 );
      pLineString->addVertex( pt2 );
      pLineString->addVertex( ptIsect2 );
      pLineString->addVertex( pt1 );
      pLineString->setLineStyleEntryId(2);
      pLineString->setColorIndex(4);
      pModel->addElement( pLineString );
      OdDgResBufPtr pHandle = OdDgResBuf::createObject();
      pHandle->setInt64((OdInt64)pLineString->elementId().getHandle());
      pCmdCtx->setArbitraryData(L"Fence", pHandle );
    }
  }
}

///////////////////////////////////////////////////////////////////
//  Implementation of "ZOOM" command
///////////////////////////////////////////////////////////////////

const OdString Cmd_ZOOM::groupName() const
{
	return globalName();
	//return L"ODADGNAPP"; 
}

const OdString Cmd_ZOOM::globalName() const
{
	return OD_T("ZOOM");
}

static void sinchronizeDgViewWithGsView(OdDgView* pDgView, OdGsView* pView);
static void zoom_extents(OdGsView* pView, OdEdCommandContext* pCmdCtx);
static void zoom_window(OdGePoint3d pt1, OdGePoint3d pt2, OdGsView* pView);

void Cmd_ZOOM::execute(OdEdCommandContext* pCmdCtx)
{
	OdDgCommandContextPtr pDbCmdCtx(pCmdCtx);
	OdDgDatabasePtr pDb = pDbCmdCtx->database();
	OdSmartPtr<OdDgUserIO> pIO = pDbCmdCtx->userIO();

	OdDgDatabaseDoc* pDg = (OdDgDatabaseDoc*)pDbCmdCtx->database();
	COdaDgnAppDoc *pDoc = pDg->document();
	COdaDgnAppVectorizer* pVectorizer = pDoc->getVectorizer();

	const OdChar* szKeywords = OD_T("All Center Dynamic Extents Previous Scale Window Object");

	OdGsView* pView = pVectorizer->getActiveView();

	OdDgViewPtr pDgView;

	if (!pVectorizer->getViewId().isNull())
	{
		pDgView = pVectorizer->getViewId().openObject(OdDg::kForWrite);
	}

	OdGePoint3d pt1, pt2;

	try
	{
		pt1 = pIO->getPoint(L"Specify corner of window, enter a scale factor (nX or nXP), or\n"
			L"[All/Center/Dynamic/Extents/Previous/Scale/Window/Object] <real time>:",
			OdEd::kInpThrowEmpty | OdEd::kInpThrowOther | OdEd::kGptNoOSnap, 0, szKeywords);

		pt2 = pIO->getPoint(OD_T("Specify opposite corner:"), OdEd::kGptNoUCS | OdEd::kGptRectFrame | OdEd::kGptNoOSnap);
		zoom_window(pt1, pt2, pView);
	}
	catch (const OdEdEmptyInput) // real time
	{
		//OdStaticRxObject<RTZoomTracker> tracker;
		for (;;)
		{
			try
			{
				ODA_FAIL();
				//tracker.init(pView, pIO->getPoint(OD_T("Press ESC or ENTER to exit."),
				//  OdEd::kInpThrowEmpty|OdEd::kGptNoUCS|OdEd::kGptBeginDrag|OdEd::kGptNoOSnap));
				//pIO->getPoint(OD_T("Press ESC or ENTER to exit."),
				//  OdEd::kInpThrowEmpty|OdEd::kGptNoUCS|OdEd::kGptEndDrag|OdEd::kGptNoOSnap, 0, OdString::kEmpty, &tracker);
			}
			catch (const OdEdCancel)
			{
				break;
			}
		}
	}
	catch (const OdEdOtherInput& otherInput) // nX or nXP
	{
		OdChar* pEnd;
		double scale = odStrToD(otherInput.string(), &pEnd);
		if (pEnd > otherInput.string().c_str())
		{
			OdString sEnd(pEnd);
			if (sEnd.iCompare(OD_T("X")) == 0)
			{
				pView->zoom(scale);

				sinchronizeDgViewWithGsView(pDgView, pView);
			}
			else if (sEnd.iCompare(OD_T("XP")) == 0)
			{
				ODA_FAIL();
				//zoom_scaleXP(scale);
			}
			else if (!*pEnd)
			{
				pView->zoom(scale);

				sinchronizeDgViewWithGsView(pDgView, pView);
			}
		}
		pIO->putString(OD_T("Requires a distance, numberX, or option keyword."));
	}
	catch (const OdEdKeyword& kw)
	{
		switch (kw.keywordIndex())
		{
		case 0: // All
			break;
		case 1: // Center
			break;
		case 2: // Dynamic
			break;
		case 3: // Extents
			::zoom_extents(pView, pCmdCtx);
			sinchronizeDgViewWithGsView(pDgView, pView);

			break;
		case 4: // Previous
			break;
		case 5: // Scale
			break;
		case 6: // Window
			pt1 = pIO->getPoint(OD_T("Specify first corner:"), OdEd::kGptNoUCS | OdEd::kGptNoOSnap);
			pt2 = pIO->getPoint(OD_T("Specify opposite corner:"), OdEd::kGptNoUCS | OdEd::kGptNoOSnap | OdEd::kGptRectFrame);
			::zoom_window(pt1, pt2, pView);
			sinchronizeDgViewWithGsView(pDgView, pView);
			break;
		case 7: // Object
			break;
		}
	}

	pDgView = 0;

	//pAVD->setView(pVpObj, pView);
}

void sinchronizeDgViewWithGsView(OdDgView* pDgView, OdGsView* pView)
{
	if (pDgView)
	{
		OdAbstractViewPEPtr pAVP(pDgView);
		pAVP->setView(pDgView, pView);
	}
}

void zoom_extents(OdGsView* view, OdEdCommandContext* pCmdCtx)
{
	//OdGsViewPtr view = getActiveView();

	//it is not calculated yet for perspective cameras
	// initial actions for perspective camera
	// find a better solution
	if (view->isPerspective())
	{
		view->zoom(1.);
	}

	//get active pModel
	OdDgCommandContextPtr pDbCmdCtx(pCmdCtx);
	//OdDgDatabasePtr pDb = pDbCmdCtx->database();
	OdDgDatabaseDoc* pDg = (OdDgDatabaseDoc*)pDbCmdCtx->database();
	COdaDgnAppDoc *pDoc = pDg->document();

	OdDgDatabase* pDb = pDoc->database();
	OdDgModelPtr pModel = pDb->getActiveModelId().safeOpenObject();

	//get the extents of that pModel
	OdGeExtents3d extents;
	pModel->getGeomExtents(pDoc->getVectorizer()->getViewId(), extents);
	if (!extents.isValidExtents())
	{
		return;
	}

	//get all points
	OdGePoint3d realPoints[8];
	{
		OdGePoint3d min = extents.minPoint(), max = extents.maxPoint();

		realPoints[0] = min;
		realPoints[1] = max;
		realPoints[2] = OdGePoint3d(min.x, max.y, min.z);
		realPoints[3] = OdGePoint3d(max.x, max.y, min.z);
		realPoints[4] = OdGePoint3d(max.x, min.y, min.z);
		realPoints[5] = OdGePoint3d(min.x, min.y, max.z);
		realPoints[6] = OdGePoint3d(min.x, max.y, max.z);
		realPoints[7] = OdGePoint3d(max.x, min.y, max.z);
	}

	//get 'relative' points
	OdGePoint2d   relativePoints[8];
	OdGePoint3d   position = view->position(), target = view->target();
	OdGeVector3d  n = view->upVector(), m = (target - position).crossProduct(n).normal();
	{
		int         i;

		for (i = 0; i < 8; i++)
		{
			relativePoints[i].x = (realPoints[i] - position).dotProduct(m);
			relativePoints[i].y = (realPoints[i] - position).dotProduct(n);
		}
	}

	//get characteristic points
	OdGeVector2d min, medium, max;
	{
		int i;

		max.x = relativePoints[0].x;
		max.y = relativePoints[0].y;

		min = max;

		for (i = 0; i < 8; i++)
		{
			if (min.x > relativePoints[i].x)
			{
				min.x = relativePoints[i].x;
			}
			if (max.x < relativePoints[i].x)
			{
				max.x = relativePoints[i].x;
			}
			if (min.y > relativePoints[i].y)
			{
				min.y = relativePoints[i].y;
			}
			if (max.y < relativePoints[i].y)
			{
				max.y = relativePoints[i].y;
			}
		}

		medium = (max + min) / 2.;
	}

	//shift the camera (if new size is not zero; it is prohibited by Ge library)
	if (min.x < max.x || min.y < max.y)
	{
		try
		{
			//view->setView(  position + m * medium.x + n * medium.y, target + m * medium.x + n * medium.y, n,
			//                ( max.x - min.x ) * 1.1, ( max.y - min.y ) * 1.1 );

			OdGePoint3d pPos = position + m * medium.x + n * medium.y;
			OdGePoint3d pTar = target + m * medium.x + n * medium.y;
			OdGeVector3d vDir = pTar - pPos;
			double dirLen = vDir.length();
			if (OdZero(dirLen, 10e-8))
			{
				pTar += OdGeVector3d(0, 0, -1);
				vDir = pTar - pPos;
				dirLen = vDir.length();
			}
			double targetLen = pTar.asVector().length();

			// #9204 (Possibly Invalid Model extents - cannot zoom to full extents)
			//        As a result 'position' point is equal 'target' point that it is wrong.
			if (targetLen > 1. && OdZero(dirLen / targetLen, 10e-8))
			{
				view->setView(pPos / dirLen * targetLen, pTar, n,
					(max.x - min.x) * 1.1, (max.y - min.y) * 1.1, view->isPerspective() ? OdGsView::kPerspective : OdGsView::kParallel);
			}
			else
			{
				view->setView(pPos, pTar, n,
					(max.x - min.x) * 1.1, (max.y - min.y) * 1.1, view->isPerspective() ? OdGsView::kPerspective : OdGsView::kParallel);
			}

			pDoc->getVectorizer()->PostMessage(WM_PAINT);
		}
		catch (const OdError& e)
		{
			theApp.reportError(_T("OnViewZoomextents error "), e);
			pDoc->getVectorizer()->GetParent()->PostMessage(WM_CLOSE);
		}
	}
}

void zoom_window(OdGePoint3d pt1, OdGePoint3d pt2, OdGsView* pView)
{
	OdGeMatrix3d xWorldToEye = OdAbstractViewPEPtr(pView)->worldToEye(pView);
	pt1.transformBy(xWorldToEye);
	pt2.transformBy(xWorldToEye);
	OdGeVector3d eyeVec = pt2 - pt1;
	if (OdNonZero(eyeVec.x) && OdNonZero(eyeVec.y))
	{
		OdGePoint3d newPos = pt1 + eyeVec / 2.;

		eyeVec.x = fabs(eyeVec.x);
		eyeVec.y = fabs(eyeVec.y);

		pView->dolly(newPos.asVector());

		double wf = pView->fieldWidth() / eyeVec.x;
		double hf = pView->fieldHeight() / eyeVec.y;

		pView->zoom(odmin(wf, hf));
	}
}

// 3d orbit command

const OdString OdDgEx3dOrbitCmd::groupName() const
{
	return globalName();
}

const OdString OdDgEx3dOrbitCmd::globalName() const
{
	return OD_T("3DORBIT");
}

class DgOrbitCtrl : public OdGiDrawableImpl<>
{
public:
	OdUInt32 subSetAttributes(OdGiDrawableTraits* pTraits) const
	{
		return kDrawableIsAnEntity | kDrawableRegenDraw;
	}
	bool subWorldDraw(OdGiWorldDraw* pWd) const
	{
		return false;
	}
	void subViewportDraw(OdGiViewportDraw* pVd) const
	{
		OdGiViewport& vp = pVd->viewport();
		OdGiGeometry& geom = pVd->geometry();
		pVd->subEntityTraits().setColor(2);
		pVd->subEntityTraits().setFillType(kOdGiFillNever);

		OdGiModelTransformSaver mt(geom, vp.getEyeToModelTransform());
		OdGiDrawFlagsHelper _dfh(pVd->subEntityTraits(), OdGiSubEntityTraits::kDrawNoPlotstyle);

		OdGePoint3d pt1;
		OdGePoint2d pt2;
		vp.getViewportDcCorners((OdGePoint2d&)pt1, pt2);
		pt2.x -= pt1.x;
		pt2.y -= pt1.y;
		double r = odmin(pt2.x, pt2.y) / 9. * 7. / 2.;
		((OdGePoint2d&)pt1) += (pt2.asVector() / 2.);
		geom.circle(pt1, r, OdGeVector3d::kZAxis);

		geom.circle(pt1 + OdGeVector3d(0., r, 0.), r / 20., OdGeVector3d::kZAxis);
		geom.circle(pt1 + OdGeVector3d(0., -r, 0.), r / 20., OdGeVector3d::kZAxis);
		geom.circle(pt1 + OdGeVector3d(r, 0., 0.), r / 20., OdGeVector3d::kZAxis);
		geom.circle(pt1 + OdGeVector3d(-r, 0., 0.), r / 20., OdGeVector3d::kZAxis);
	}
};

class DgRTOrbitTracker : public OdEdPointTracker
{
	OdGsView*       m_pView;
	OdGePoint3d     m_pt;
	OdGiDrawablePtr m_pDrw;
	OdGePoint3d     m_pos;
	OdGePoint3d     m_trg;
	OdGeVector3d    m_up;
	OdGeVector3d    m_x;
	OdGePoint3d     m_viewCenter;
	OdGeMatrix3d    m_initViewingMatrixInv;
	double          m_D; // diameter of orbit control in projected coordinates
	OdGsModelPtr    m_pModel;

	enum Axis
	{
		kHorizontal,
		kVertical,
		kPerpDir, // orbit around perpendicular to mouse direction
		kEye,
	}
	m_axis;

	void viewportDcCorners(OdGePoint2d& lower_left, OdGePoint2d& upper_right) const
	{
		OdGePoint3d target = m_pView->viewingMatrix() * m_pView->target();
		double halfFieldWidth = m_pView->fieldWidth() / 2.0;
		double halfFieldHeight = m_pView->fieldHeight() / 2.0;
		lower_left.x = target.x - halfFieldWidth;
		lower_left.y = target.y - halfFieldHeight;
		upper_right.x = target.x + halfFieldWidth;
		upper_right.y = target.y + halfFieldHeight;
	}
public:
	DgRTOrbitTracker()
		: m_pView(0)
		, m_D(0)
	{
	}
	void reset() { m_pView = 0; }
	void init(OdGsView* pView, const OdGePoint3d& pt)
	{
		m_pView = pView;
		m_pos = pView->position();
		m_trg = pView->target();
		m_up = pView->upVector();
		m_x = m_up.crossProduct(pView->target() - m_pos).normal();

		m_initViewingMatrixInv = m_pView->viewingMatrix();
		m_pt = m_initViewingMatrixInv * pt;
		m_pt.z = 0.;
		m_initViewingMatrixInv.invert();

		OdGePoint3d pt1;
		OdGePoint2d pt2;
		viewportDcCorners((OdGePoint2d&)pt1, pt2);
		pt2.x -= pt1.x;
		pt2.y -= pt1.y;
		double r = odmin(pt2.x, pt2.y) / 9. * 7. / 2.;
		m_D = 2.0 * r;
		((OdGePoint2d&)pt1) += (pt2.asVector() / 2.);
		double r2sqrd = r*r / 400.;

		pt1.y += r;
		if ((pt1 - m_pt).lengthSqrd() <= r2sqrd)
		{
			m_axis = kHorizontal;
		}
		else
		{
			pt1.y -= r;
			pt1.y -= r;
			if ((pt1 - m_pt).lengthSqrd() <= r2sqrd)
			{
				m_axis = kHorizontal;
			}
			else
			{
				pt1.y += r;
				pt1.x += r;
				if ((pt1 - m_pt).lengthSqrd() <= r2sqrd)
				{
					m_axis = kVertical;
				}
				else
				{
					pt1.x -= r;
					pt1.x -= r;
					if ((pt1 - m_pt).lengthSqrd() <= r2sqrd)
					{
						m_axis = kVertical;
					}
					else
					{
						pt1.x += r;
						if ((pt1 - m_pt).lengthSqrd() <= r*r)
						{
							m_axis = kPerpDir;
						}
						else
						{
							m_axis = kEye;
						}
					}
				}
			}
		}

    bool bComputeExtents = true;
    { // Try to extract cached extents
      OdGsClientViewInfo viewInfo;
      pView->clientViewInfo(viewInfo);
      OdGiContextForDgDatabasePtr pContext = (OdGiContextForDgDatabase*)(pView->userGiContext());

      if( !pContext.isNull() )
      {
        OdDgViewPtr pDgView = pContext->getView();

        if( !pDgView.isNull() )
        {
          OdDgElementId idModel = pDgView->getModelId();

          OdDgModelPtr pBTR = idModel.openObject();
          OdGeExtents3d wcsExt;

          if (pBTR->gsNode() && pBTR->gsNode()->extents(wcsExt))
          {
            OdGeMatrix3d matViewing = pView->viewingMatrix();
            wcsExt.transformBy( matViewing );

            m_viewCenter = wcsExt.center();
            bComputeExtents = false;
          }
        }
      }
    }

    if( bComputeExtents )
    {
      OdAbstractViewPEPtr pAView = pView;
      OdGeBoundBlock3d extents;
      pAView->viewExtents(pView, extents);
      m_viewCenter = extents.center();
    }

		m_viewCenter.transformBy(m_initViewingMatrixInv);
	}

	double angle(const OdGePoint3d& value) const
	{
		OdGePoint3d pt2 = m_pView->viewingMatrix() * value;
		double dist = 0.0;
		if (m_axis == kHorizontal)
			dist = pt2.y - m_pt.y;
		else if (m_axis == kVertical)
			dist = pt2.x - m_pt.x;
		return dist * OdaPI / m_D;
	}

	double angleZ(const OdGePoint3d& value) const
	{
		OdGePoint3d pt2 = m_pView->viewingMatrix() * value;
		OdGePoint3d targ = m_pView->viewingMatrix() * m_viewCenter;
		pt2.z = targ.z = m_pt.z;
		return (pt2 - targ).angleTo((m_pt - targ), OdGeVector3d::kZAxis);
	}

	double anglePerp(const OdGePoint3d& value) const
	{
		OdGePoint3d pt2 = m_pView->viewingMatrix() * value;
		pt2.z = 0.0;
		return pt2.distanceTo(m_pt) * OdaPI / m_D;
	}

	void setValue(const OdGePoint3d& value)
	{
		if (m_pView)
		{
			OdGeMatrix3d x;
			switch (m_axis)
			{
			case kHorizontal:
				x.setToRotation(-angle(value), m_x, m_viewCenter);
				break;
			case kVertical:
				x.setToRotation(-angle(value), m_up, m_viewCenter);
				break;
			case kEye:
				x.setToRotation(-angleZ(value), m_trg - m_pos, m_viewCenter);
				break;
			case kPerpDir:
			{
				OdGePoint3d value1 = value;
				value1.transformBy(m_pView->viewingMatrix());
				value1.z = 0.0;
				OdGeVector2d dir = (value1 - m_pt).convert2d();
				OdGeVector2d perp = dir.perpVector();
				OdGeVector3d perp3d(perp.x, perp.y, 0.0);
				perp3d.normalizeGetLength();
				perp3d.transformBy(m_initViewingMatrixInv);
				x.setToRotation(-anglePerp(value), perp3d, m_viewCenter);
				break;
			}
			}
			OdGePoint3d newPos = x * m_pos, newTarget = x * m_trg;
			OdGeVector3d newPosDir = newPos - newTarget;
			newPosDir.normalizeGetLength();
			newPosDir *= m_pos.distanceTo(m_trg);
			newPos = newTarget + newPosDir;

			m_pView->setView(newPos,
				newTarget,
				x * m_up,
				m_pView->fieldWidth(),
				m_pView->fieldHeight(),
				m_pView->isPerspective() ? OdGsView::kPerspective : OdGsView::kParallel
			);
		}
	}

	int addDrawables(OdGsView* pView)
	{
		m_pDrw = OdRxObjectImpl<DgOrbitCtrl>::createObject();
		if (m_pModel.isNull())
		{
			m_pModel = pView->device()->createModel();
			if (!m_pModel.isNull())
				m_pModel->setRenderType(OdGsModel::kDirect);
		}
		pView->add(m_pDrw, m_pModel.get());
		return 1;
	}

	void removeDrawables(OdGsView* pView)
	{
		pView->erase(m_pDrw);
	}
};

void OdDgEx3dOrbitCmd::execute(OdEdCommandContext* pCmdCtx)
{
	OdDgCommandContextPtr pDbCmdCtx(pCmdCtx);
	OdDgDatabasePtr pDb = pDbCmdCtx->database();
	OdSmartPtr<OdDgUserIO> pIO = pDbCmdCtx->userIO();

	OdDgDatabaseDoc* pDg = (OdDgDatabaseDoc*)pDbCmdCtx->database();
	COdaDgnAppDoc *pDoc = pDg->document();
	COdaDgnAppVectorizer* pVectorizer = pDoc->getVectorizer();

	OdGsView* pView = pVectorizer->getActiveView();

	OdStaticRxObject<DgRTOrbitTracker> tracker;
	for (;;)
	{
		try
		{
			tracker.init(pView, pIO->getPoint(OD_T("Press ESC or ENTER to exit."),
				OdEd::kInpThrowEmpty | OdEd::kGptNoUCS | OdEd::kGptNoOSnap | OdEd::kGptBeginDrag, 0, OdString::kEmpty, &tracker));
			pIO->getPoint(OD_T("Press ESC or ENTER to exit."),
				OdEd::kInpThrowEmpty | OdEd::kGptNoUCS | OdEd::kGptNoOSnap | OdEd::kGptEndDrag, 0, OdString::kEmpty, &tracker);
			tracker.reset();

			OdDgViewPtr pDgView;

			if (!pVectorizer->getViewId().isNull())
			{
				pDgView = pVectorizer->getViewId().openObject(OdDg::kForWrite);
			}

			sinchronizeDgViewWithGsView(pDgView, pView);
		}
		catch (const OdEdCancel)
		{
			break;
		}
	}
}

// Dolly command

const OdString OdDgExDollyCmd::groupName() const
{
	return globalName();
}

const OdString OdDgExDollyCmd::globalName() const
{
	return OD_T("DOLLY");
}

class DgRTDollyTracker : public OdEdPointTracker
{
	OdGsView*       m_pView;
	OdGePoint3d     m_pt;
	OdGePoint3d     m_pos;
public:
	DgRTDollyTracker()
		: m_pView(0)
	{
	}
	void reset() { m_pView = 0; }
	void init(OdGsView* pView, const OdGePoint3d& pt)
	{
		m_pView = pView;
		m_pos = pView->position();
		m_pt = pt - m_pos.asVector();
	}

	void setValue(const OdGePoint3d& value)
	{
		if (m_pView)
		{
			OdGeVector3d delta = (m_pt - (value - m_pos)).asVector();
			m_pt = value - m_pos.asVector();
			delta.transformBy(m_pView->viewingMatrix());
			m_pView->dolly(delta.x, delta.y, delta.z);
			m_pos = m_pView->position();
		}
	}

	int addDrawables(OdGsView* /*pView*/)
	{
		return 0;
	}
	void removeDrawables(OdGsView* /*pView*/)
	{
	}
};

void OdDgExDollyCmd::execute(OdEdCommandContext* pCmdCtx)
{
	OdDgCommandContextPtr pDbCmdCtx(pCmdCtx);
	OdDgDatabasePtr pDb = pDbCmdCtx->database();
	OdSmartPtr<OdDgUserIO> pIO = pDbCmdCtx->userIO();

	OdDgDatabaseDoc* pDg = (OdDgDatabaseDoc*)pDbCmdCtx->database();
	COdaDgnAppDoc *pDoc = pDg->document();
	COdaDgnAppVectorizer* pVectorizer = pDoc->getVectorizer();

	OdGsView* pView = pVectorizer->getActiveView();

	OdStaticRxObject<DgRTDollyTracker> tracker;
	for (;;)
	{
		try
		{
			tracker.init(pView, pIO->getPoint(OD_T("Press ESC or ENTER to exit."),
				OdEd::kInpThrowEmpty | OdEd::kGptNoUCS | OdEd::kGptNoOSnap | OdEd::kGptBeginDrag, 0, OdString::kEmpty, &tracker));
			pIO->getPoint(OD_T("Press ESC or ENTER to exit."),
				OdEd::kInpThrowEmpty | OdEd::kGptNoUCS | OdEd::kGptNoOSnap | OdEd::kGptEndDrag, 0, OdString::kEmpty, &tracker);
			tracker.reset();

			OdDgViewPtr pDgView;

			if (!pVectorizer->getViewId().isNull())
			{
				pDgView = pVectorizer->getViewId().openObject(OdDg::kForWrite);
			}

			sinchronizeDgViewWithGsView(pDgView, pView);
		}
		catch (const OdEdCancel)
		{
			break;
		}
	}
}

void COdaDgnAppDoc::OnRenderLights()
{
	CDbLightsDlg dlg(m_pDb);
	dlg.DoModal();
}


void COdaDgnAppDoc::OnRenderMaterials()
{
	CDbMaterialsDlg dlg(m_pDb);
	dlg.DoModal();
}


void COdaDgnAppDoc::OnToolsLevelManager()
{
	CLevelPropManagerDlg dlg(m_pDb);
	dlg.DoModal();
}

void COdaDgnAppDoc::OnExportObj()
{
#ifdef OBJTOOLKIT_ENABLED

  CFileDialog dlgFile(FALSE, _T("obj"), 0, OFN_HIDEREADONLY | OFN_EXPLORER | OFN_PATHMUSTEXIST,
    _T("Wavefront Obj (*.obj)|*.obj||"), AfxGetMainWnd());
  if (dlgFile.DoModal() != IDOK)
    return;

  OBJToolkit::OdObjToolkitModulePtr pModule = ::odrxDynamicLinker()->loadApp(OdObjToolkitModuleName);
  if (pModule.isNull())
  {
    AfxMessageBox(L"Cannot load module : " OdObjToolkitModuleName, MB_OK | MB_ICONERROR);
    return;
  }

  OdDgDatabasePtr pDb = database();
  OdString pathName = dlgFile.GetPathName();

  OdDgElementId activeModelId = pDb->getActiveModelId();
  OdDgModelPtr pModel = OdDgModel::cast(activeModelId.openObject(OdDg::kForRead));
  OBJToolkit::OdObjExportOptions options;
  options.m_applicationComment = OD_T("Open Design Alliance OdaDgnApp");
  OdResult res = pModule->exportObj(pDb, pModel, pathName, &options);

  if (res != eOk)
  {
    OdString tmp = OD_T("Export error: ") + OdError(res).description();
    AfxMessageBox(tmp.c_str(), MB_OK | MB_ICONERROR);
  }

#else

  AfxMessageBox(L"TD_Components should be enabled...", MB_OK | MB_ICONERROR);

#endif // OBJTOOLKIT_ENABLED
}
