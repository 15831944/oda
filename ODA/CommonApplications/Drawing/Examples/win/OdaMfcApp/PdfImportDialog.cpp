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

// DwfUnderlayDialog.cpp : implementation file
//

#include "stdafx.h"
#include "odamfcapp.h"
#include "PdfImportDialog.h"
#include "DbUnderlayHost.h"
#include "DbUnderlayDefinition.h"
#include "DbUnderlayReference.h"
#include "DbDictionary.h"
#include "DbBlockTableRecord.h"
#include "Gi\GiRasterWrappers.h"
#include "OdPath.h"
#include "PdfImportTextOptDialog.h"
#if defined (USE_PDFIUM)
#include "PdfImport.h"
#include "PdfImportEx.h"
#endif

#include "DbObjectContextCollection.h"
#include "DbObjectContextManager.h"
#include "DbScale.h"
#include "DbSymUtl.h"

#if _MSC_VER < 1400
#include <atlbase.h>
#include <atlwin.h>
#endif


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPdfImportDialog dialog


CPdfImportDialog::CPdfImportDialog(OdDbDatabase *pDb, CWnd* pParent /*=NULL*/)
	: CDialog(CPdfImportDialog::IDD, pParent)
  , m_pDb(pDb)
{
	//{{AFX_DATA_INIT(CPdfImportDialog)
	m_Path = _T("");
  m_SelectedPageNum = 0;
  m_LayersUseType = 0;
  m_Scaling = 1;
  m_Rotation = 0;

  m_ImportVectorGeometry = TRUE;
  m_ImportSolidFills = TRUE;
  m_ImportTrueTypeText = TRUE;
  m_ImportRasterImages = TRUE;
  m_ImportGradientFills = TRUE;
  m_ImportWidgets = TRUE;

  m_ImportAsBlock = FALSE;
  m_JoinLineAndArcSegments = TRUE;
  m_ConvertSolidsToHatches = TRUE;
  m_ApplyLineweight = TRUE;
  m_ImportTrueTypeTextAsGeometry = FALSE;
  m_UseGeometryOptimization = TRUE;
  m_ImportType3AsTrueTypeText = FALSE;
  m_ImportAsRgb = FALSE;
  m_ImportInvalidTTTextAsGeometry = TRUE;
  m_UseClipping = FALSE;
  m_UsePageBorderClipping = FALSE;
  m_CombineTextObjects = FALSE;
  m_UsePdfTransparency = FALSE;
  m_UseINSUNITS = FALSE;
  m_ExtractEmbeddedFonts = FALSE;

	//}}AFX_DATA_INIT
}

OdString CPdfImportDialog::GetCommandString()
{
  return m_CommandString;
}

void CPdfImportDialog::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CPdfImportDialog)
  DDX_Text(pDX, IDC_EDIT_FILENAME, m_Path);
  DDX_Control(pDX, IDC_LIST_SHEETS, m_SheetList);
  DDX_Control(pDX, IDC_PDF_PAGE_ROTAION, m_comboRotation);
  DDX_Radio(pDX, IDC_RADIO_USE_PDF_LAYERS, m_LayersUseType);
  DDX_Check(pDX, IDC_PDF_IMP_VECTOR_GEOM, m_ImportVectorGeometry);
  DDX_Check(pDX, IDC_PDF_IMP_WIDGETS, m_ImportWidgets);
  DDX_Check(pDX, IDC_PDF_IMP_SOLID_FILLS, m_ImportSolidFills);
  DDX_Check(pDX, IDC_PDF_IMP_TT_TEXT, m_ImportTrueTypeText);
  DDX_Check(pDX, IDC_PDF_IMP_RASTER_IMAGES, m_ImportRasterImages);
  DDX_Check(pDX, IDC_PDF_IMP_SHADING, m_ImportGradientFills);
  DDX_Check(pDX, IDC_PDF_AS_BLOCK, m_ImportAsBlock);
  DDX_Check(pDX, IDC_PDF_JOIN_LINE_ARC, m_JoinLineAndArcSegments);
  DDX_Check(pDX, IDC_PDF_CONVERT_SOLIDS2HATCHES, m_ConvertSolidsToHatches);
  DDX_Check(pDX, IDC_PDF_LINEWEIGHT, m_ApplyLineweight);
  //DDX_Check(pDX, IDC_TTF_AS_GEOM, m_ImportTrueTypeTextAsGeometry);
  //DDX_Check(pDX, IDC_PDF_IMP_TYPE3_AS_TT, m_ImportType3AsTrueTypeText);
  DDX_Check(pDX, IDC_PDF_IMP_USE_2DGEOM_OPTIM, m_UseGeometryOptimization);
  DDX_Check(pDX, IDC_PDF_IMP_AS_RGB, m_ImportAsRgb);
  DDX_Text(pDX, IDC_EDIT_SCALE, m_Scaling);
  DDX_Text(pDX, IDC_PDF_PAGE_ROTAION, m_Rotation);
  DDX_Text(pDX, IDC_EDIT_PAGE_NUM, m_SelectedPageNum);
  DDX_Check(pDX, IDC_INSERTION_POINT, m_UseInsertionPoint);
  //DDX_Check(pDX, IDC_INVALID_TTF_AS_GEOM, m_ImportInvalidTTTextAsGeometry);
  DDX_Check(pDX, IDC_PDF_IMP_USE_CLIPPING, m_UseClipping);
  DDX_Check(pDX, IDC_PDF_IMP_PAGE_BRD_CLIPPING, m_UsePageBorderClipping);
  DDX_Check(pDX, IDC_PDF_IMP_USE_IMG_CLIPPING, m_UseImageClipping);
  //DDX_Check(pDX, IDC_PDF_IMP_NEED_TO_COMB_TEXT, m_CombineTextObjects);
  DDX_Check(pDX, IDC_PDF_IMP_USE_PDF_TRANSPARENT, m_UsePdfTransparency);
  DDX_Control(pDX, IDC_PDF_IMP_RADIO_HYPER_AS_ENTITY, m_comboHyperLink);
  DDX_Check(pDX, IDC_USE_INSUNITS, m_UseINSUNITS);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPdfImportDialog, CDialog)
	//{{AFX_MSG_MAP(CPdfImportDialog)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnBrowse)
	ON_BN_CLICKED(IDOK, OnInsert)
  ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_SHEETS, OnSelchangeSheetList)
  ON_EN_KILLFOCUS(IDC_EDIT_PAGE_NUM, OnPageNumChanged)
  ON_BN_CLICKED(IDC_PDF_IMP_VECTOR_GEOM, OnCheckImportVectorGeometry)
  ON_BN_CLICKED(IDC_PDF_IMP_USE_CLIPPING, OnCheckImportClipping)
	ON_BN_CLICKED(IDC_PDFIMPORT_TEXT_OPTIION_BUTTON, OnEditTextOptions)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPdfImportDialog message handlers

BOOL CPdfImportDialog::OnInitDialog()
{
  CDialog::OnInitDialog();
  CenterWindow();

  CRect rect, rectButton;
  GetWindowRect(&rect);
  GetDlgItem(IDC_BUTTON_BROWSE)->GetWindowRect(&rectButton);
  m_DialogHeight = rect.Height() - rectButton.Height() - 40;
  rect.bottom = rectButton.bottom + 17;
  MoveWindow(&rect);

  m_SheetList.SetBkColor(::GetSysColor(COLOR_GRAYTEXT));

  m_comboRotation.ResetContent();
  for (OdUInt8 i = 0; i < 4; ++i)
  {
    CString rotation_str;
    rotation_str.Format(_T("%i"), i*90);
    m_comboRotation.InsertString(i, rotation_str);
  }
  m_comboRotation.SetCurSel(0);

  m_comboHyperLink.ResetContent();
  m_comboHyperLink.InsertString(0, _T("As hyperlink owned by entity"));
  m_comboHyperLink.InsertString(1, _T("As transparency hatch"));
  m_comboHyperLink.InsertString(2, _T("Mixed method"));


  UpdateData(true); // save and validate
  m_LayersUseType = AfxGetApp()->GetProfileInt(L"PdfImport", L"PDFIMPORTLAYERS", 0);
  int import_filter = AfxGetApp()->GetProfileInt(L"PdfImport", L"PDFIMPORTFILTER", 0);
  m_ImportVectorGeometry = !GETBIT(import_filter, 1);
  GetDlgItem(IDC_PDF_IMP_SOLID_FILLS)->EnableWindow(m_ImportVectorGeometry);
  m_ImportSolidFills = !GETBIT(import_filter, 4);
  m_ImportTrueTypeText = !GETBIT(import_filter, 2);
  m_ImportRasterImages = !GETBIT(import_filter, 8);
  m_ImportGradientFills = !GETBIT(import_filter, 16);
  int import_mode = AfxGetApp()->GetProfileInt(L"PdfImport", L"PDFIMPORTMODE", 46);
  m_ImportAsBlock = GETBIT(import_mode, 1);
  m_JoinLineAndArcSegments = GETBIT(import_mode, 4);
  m_ConvertSolidsToHatches = GETBIT(import_mode, 8);
  m_ApplyLineweight = GETBIT(import_mode, 2);
  m_ImportTrueTypeTextAsGeometry = GETBIT(import_mode, 16);
  m_UseGeometryOptimization = GETBIT(import_mode, 32);
  m_ImportType3AsTrueTypeText = GETBIT(import_mode, 64);
  m_ImportAsRgb = GETBIT(import_mode, 128);

  int import_mode2 = AfxGetApp()->GetProfileInt(L"PdfImport", L"PDFIMPORTMODE2", 0);
  m_ImportInvalidTTTextAsGeometry = GETBIT(import_mode2, 1);
  m_UseClipping = GETBIT(import_mode2, 2);
  GetDlgItem(IDC_PDF_IMP_PAGE_BRD_CLIPPING)->EnableWindow(m_UseClipping);
  GetDlgItem(IDC_PDF_IMP_USE_IMG_CLIPPING)->EnableWindow(m_UseClipping);
  m_UsePageBorderClipping = GETBIT(import_mode2, 4);
  m_CombineTextObjects = GETBIT(import_mode2, 8);
  m_UsePdfTransparency = GETBIT(import_mode2, 16);
  m_ImportWidgets = GETBIT(import_mode2, 32);
  m_comboHyperLink.SetCurSel(AfxGetApp()->GetProfileInt(L"PdfImport", L"PDFIMPORTHYPERLINKMETHOD", 0));
  m_UseImageClipping = GETBIT(import_mode2, 64);
  m_UseINSUNITS = GETBIT(import_mode2, 128);

  m_Type3FontString = AfxGetApp()->GetProfileStringW(L"PdfImport", L"Type3FontString", L"");
  m_NotFoundFontString = AfxGetApp()->GetProfileStringW(L"PdfImport", L"NotFoundFontString", L"");

  m_ReplaceFontName1 = AfxGetApp()->GetProfileStringW(L"PdfImport", L"ReplaceFontName1", L"");
  m_ReplaceFontFamily1 = AfxGetApp()->GetProfileStringW(L"PdfImport", L"ReplaceFontFamily1", L"");
  m_ReplaceFontString1 = AfxGetApp()->GetProfileStringW(L"PdfImport", L"ReplaceFontString1", L"");

  m_ReplaceFontName2 = AfxGetApp()->GetProfileStringW(L"PdfImport", L"ReplaceFontName2", L"");
  m_ReplaceFontFamily2 = AfxGetApp()->GetProfileStringW(L"PdfImport", L"ReplaceFontFamily2", L"");
  m_ReplaceFontString2 = AfxGetApp()->GetProfileStringW(L"PdfImport", L"ReplaceFontString2", L"");

  m_ReplaceFontName3 = AfxGetApp()->GetProfileStringW(L"PdfImport", L"ReplaceFontName3", L"");
  m_ReplaceFontFamily3 = AfxGetApp()->GetProfileStringW(L"PdfImport", L"ReplaceFontFamily3", L"");
  m_ReplaceFontString3 = AfxGetApp()->GetProfileStringW(L"PdfImport", L"ReplaceFontString3", L"");

  int import_mode3 = AfxGetApp()->GetProfileInt(L"PdfImport", L"PDFIMPORTMODE3", 0);
  m_ExtractEmbeddedFonts = GETBIT(import_mode3, 1);

  UpdateData(false);

  return TRUE;
}

void CPdfImportDialog::OnEditTextOptions()
{
  CPdfImportTextOptDialog dlg(theApp.GetMainWnd(), m_FontMap);

  dlg.SetImportTrueTypeTextAsGeometry(m_ImportTrueTypeTextAsGeometry);
  dlg.SetImportType3AsTrueTypeText(m_ImportType3AsTrueTypeText);
  dlg.SetImportInvalidTTTextAsGeometry(m_ImportInvalidTTTextAsGeometry);
  dlg.SetCombineTextObjects(m_CombineTextObjects);

  dlg.SetType3FontString(m_Type3FontString);
  dlg.SetNotFoundFontString(m_NotFoundFontString);

  dlg.SetReplaceFontName1(m_ReplaceFontName1);
  dlg.SetReplaceFontFamily1(m_ReplaceFontFamily1);
  dlg.SetReplaceFontString1(m_ReplaceFontString1);

  dlg.SetReplaceFontName2(m_ReplaceFontName2);
  dlg.SetReplaceFontFamily2(m_ReplaceFontFamily2);
  dlg.SetReplaceFontString2(m_ReplaceFontString2);

  dlg.SetReplaceFontName3(m_ReplaceFontName3);
  dlg.SetReplaceFontFamily3(m_ReplaceFontFamily3);
  dlg.SetReplaceFontString3(m_ReplaceFontString3);

  dlg.SetExtractEmbeddedFonts(m_ExtractEmbeddedFonts);

  if (dlg.DoModal() == IDOK)
  {
    m_ImportTrueTypeTextAsGeometry = dlg.GetImportTrueTypeTextAsGeometry();
    m_ImportType3AsTrueTypeText = dlg.GetImportType3AsTrueTypeText();
    m_ImportInvalidTTTextAsGeometry = dlg.GetImportInvalidTTTextAsGeometry();
    m_CombineTextObjects = dlg.GetCombineTextObjects();

    m_Type3FontString = dlg.GetType3FontString();
    m_NotFoundFontString = dlg.GetNotFoundFontString();
    m_ReplaceFontName1 = dlg.GetReplaceFontName1();
    m_ReplaceFontFamily1 = dlg.GetReplaceFontFamily1();
    m_ReplaceFontString1 = dlg.GetReplaceFontString1();
    m_ReplaceFontName2 = dlg.GetReplaceFontName2();
    m_ReplaceFontFamily2 = dlg.GetReplaceFontFamily2();
    m_ReplaceFontString2 = dlg.GetReplaceFontString2();
    m_ReplaceFontName3 = dlg.GetReplaceFontName3();
    m_ReplaceFontFamily3 = dlg.GetReplaceFontFamily3();
    m_ReplaceFontString3 = dlg.GetReplaceFontString3();

    m_ExtractEmbeddedFonts = dlg.GetExtractEmbeddedFonts();
  }
}

void CPdfImportDialog::OnBrowse() 
{
	// TODO: Add your control notification handler code here

  CFileDialog dlgFile(true);

  dlgFile.m_ofn.Flags |= OFN_HIDEREADONLY | OFN_EXPLORER | OFN_FILEMUSTEXIST;


  dlgFile.m_ofn.lpstrFilter = _T("PDF files (*.pdf)\0*.pdf\0");
  dlgFile.m_ofn.lpstrDefExt = _T("pdf");
  dlgFile.m_ofn.lpstrTitle = _T("Select pdf file");

  if (dlgFile.DoModal() == IDOK)
  {
    
    UpdateData(true); // save and validate
    m_Path = dlgFile.GetPathName();
    UpdateData(false);

    bool bInit = InitProperties();

    if (bInit)
    {
      CRect rect;
      GetWindowRect(&rect);
      rect.bottom += m_DialogHeight;
      m_DialogHeight = 0;
      MoveWindow(&rect);
    }
  }
}

void CPdfImportDialog::OnError(CString str)
{
  MessageBox(str, CString("Error"));
}

void CPdfImportDialog::OnError(OdResult res)
{
  OdError err(res);
  CString str((LPCTSTR)err.description());
  OnError(str);
}

void CPdfImportDialog::OnPageNumChanged()
{
  UpdateData(true);
  m_SheetList.SetItemState(m_SelectedPageNum - 1, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
  m_SheetList.EnsureVisible(m_SelectedPageNum - 1, FALSE);
  m_SheetList.SetFocus();
  UpdateData(false);
}

bool CPdfImportDialog::InitProperties()
{
  // ask for UnderlayHostPE
  OdDbUnderlayHostPEPtr pHost;

  OdDbPdfDefinition::loadPdfUnderlayModule();

  pHost = OdDbPdfDefinition::desc()->getX(OdDbUnderlayHostPE::desc());


  if(!pHost.get())
  {
    OnError(_T("PdfUnderlayModule is not loaded."));
    return false;
  }

  OdResult res = pHost->load(*m_pDb, (LPCTSTR)m_Path, "", m_pFile);
  if(res != eOk)
  {
    m_pFile = OdDbUnderlayFilePtr();
    if (res == eInvalidPassword)
    {
      //check password cache for the valid password.
      OdPwdCachePtr pCache = m_pDb->appServices()->getPasswordCache();
      if (!pCache.isNull())
      {
        OdPwdIteratorPtr pIter = pCache->newIterator();
        if (!pIter.isNull())
        {
          for(;!pIter->done() && res != eOk; pIter->next())
          {
            OdPassword password;
            pIter->get(password);
            res = pHost->load(*m_pDb, (LPCTSTR)m_Path, password, m_pFile);
            if (eOk == res)
            {
              m_Password = password;
              break;
            }
          }
        }
      }

      // Means that there are no valid password in cache.
      if (res == eInvalidPassword)
      {
        // ask user about valid password
        OdPassword odPass;
        if (!m_pDb->appServices()->getPassword((LPCTSTR)m_Path, false, odPass))
        {
          OnError("Password protected file."); 
          return false;
        }

        // try to load again with new password
        res = pHost->load(*m_pDb, (LPCTSTR)m_Path, odPass, m_pFile);
        if (res == eInvalidPassword)
        { 
          OnError("Invalid password."); 
          return false;
        }
        else
        {
          pCache->add(odPass);
          m_Password = odPass;
        }
      }
    }

    if(res != eOk)
    {
      OnError("OdDbUnderlayHostPE cannot load file"); 
      return false;
    }
  }

  FillSheetList();

#if defined (USE_PDFIUM)
  m_FontMap.clear();

  OdPdfImportModulePtr pModule = ::odrxDynamicLinker()->loadModule(OdPdfImportModuleName, false);
  PdfImporterExPtr pPdfImporter = pModule->create();
  if (!pPdfImporter.isNull())
  {
    OdString path((LPCTSTR)m_Path);
    pPdfImporter->properties()->putAt(OD_T("PdfPath"), OdRxVariantValue(path));
    pPdfImporter->properties()->putAt(OD_T("Password"), OdRxVariantValue(m_Password));

    //The same settings as in the dialog. If text import isn't selected, fonts won't be filled. 
    pPdfImporter->properties()->putAt(OD_T("ImportTrueTypeText"), OdRxVariantValue((bool)m_ImportTrueTypeText));

    pPdfImporter->properties()->putAt(OD_T("ImportTrueTypeTextAsGeometry"), OdRxVariantValue((bool)m_ImportTrueTypeTextAsGeometry));
    pPdfImporter->properties()->putAt(OD_T("ImportType3AsTrueTypeText"), OdRxVariantValue((bool)m_ImportType3AsTrueTypeText));
    pPdfImporter->properties()->putAt(OD_T("ImportInvalidTTTextAsGeometry"), OdRxVariantValue((bool)m_ImportInvalidTTTextAsGeometry));
    pPdfImporter->properties()->putAt(OD_T("ImportWidgets"), OdRxVariantValue((bool)m_ImportWidgets));


    pPdfImporter->loadDocument();

    OdUInt32 page_count = pPdfImporter->getPageCount();

    for (OdUInt32 i = 0; i < page_count; ++i)
    {
      pPdfImporter->properties()->putAt(OD_T("PageNumber"), OdRxVariantValue((OdUInt32)i + 1));
      pPdfImporter->loadPage();
      if (pPdfImporter->isPageLoaded())
      {
        pPdfImporter->fillFontsInfo(m_FontMap);
      }
    }
  }

#endif

  return true;
}

void CPdfImportDialog::FillSheetList()
{

  CString total_text;
  total_text.Format(_T("Total: %i"), m_pFile->getItemCount());
  GetDlgItem(IDC_STATIC_PAGE_COUNT)->SetWindowText(total_text);

  m_SheetList.SetRedraw(false);

  m_SheetList.DeleteAllItems();

  for (OdInt32 page_num = 0; page_num < m_pFile->getItemCount(); ++page_num)
  {
    OdDbUnderlayItemPtr pItem = m_pFile->getItem(page_num);
    OdGiRasterImagePtr pImage = pItem->getThumbnail();

    int width = pImage->pixelWidth();
    int height = pImage->pixelHeight();
    if (0 == page_num)
    {
      m_stateImages.DeleteImageList();
      if (1 == m_pFile->getItemCount())
      {
        CRect rect;
        m_SheetList.GetClientRect(rect);
        double coef = odmin(((double)rect.Width() - 50) / width, ((double)rect.Height() - 50) / height);
        width *= coef;
        height *= coef;
      }

      m_stateImages.Create(width, height, ILC_COLOR24, 0, m_pFile->getItemCount());
    }

    if (!pImage.isNull())
    {
      OdSmartPtr<OdGiRasterImageDesc> pDesc = OdRxObjectImpl<OdGiRasterImageDesc>::createObject();
      pDesc->setPixelWidth(width);
      pDesc->setPixelHeight(height);
      pDesc->setColorDepth(24);
      pDesc->pixelFormat().setBGR();
      pDesc->setScanLinesAlignment(4);
      OdGiRasterImagePtr pCopyImg;
      pCopyImg = pImage->convert(true, 50.0, 50.0, 0.0, 0, false, false, false, pDesc);
      if (!pCopyImg.isNull())
      {
        BITMAPINFO* pHeader;
        OdUInt8Array headerMemCont;
        headerMemCont.resize(sizeof(BITMAPINFOHEADER) + pCopyImg->paletteDataSize());
        pHeader = (BITMAPINFO*)headerMemCont.getPtr();
        pHeader->bmiHeader.biBitCount = (WORD)pCopyImg->colorDepth();
        pHeader->bmiHeader.biClrImportant = 0;
        pHeader->bmiHeader.biClrUsed = 0;
        pHeader->bmiHeader.biCompression = BI_RGB;
        pHeader->bmiHeader.biHeight = pCopyImg->pixelHeight();
        pHeader->bmiHeader.biPlanes = 1;
        pHeader->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        pHeader->bmiHeader.biSizeImage = 0;
        pHeader->bmiHeader.biWidth = pCopyImg->pixelWidth();
        pHeader->bmiHeader.biXPelsPerMeter = 0;
        pHeader->bmiHeader.biYPelsPerMeter = 0;
        pCopyImg->paletteData((OdUInt8*)pHeader->bmiColors);

        OdUInt8* pBuf;
        HBITMAP hBmp = CreateDIBSection(NULL, pHeader, DIB_RGB_COLORS, (void**)&pBuf, NULL, 0);

        if (pCopyImg->scanLineSize())
          pCopyImg->scanLines(pBuf, 0, pCopyImg->pixelHeight());

        CBitmap bitmap;
        bitmap.Attach(hBmp);

        m_stateImages.Add(&bitmap, RGB(0, 0, 0));

        LVITEM lvItem;
        lvItem.iItem = page_num;
        lvItem.iImage = page_num;    // image index that refers to your image list
        CString str;
        str.Format(_T("%i"), page_num + 1);
        lvItem.pszText = str.GetBuffer();
        lvItem.mask = LVIF_IMAGE | LVIF_TEXT;
        lvItem.lParam = 0;
        lvItem.iSubItem = 0;
        m_SheetList.InsertItem(&lvItem);

      }
    }
  }
  m_SheetList.SetImageList(&m_stateImages, LVSIL_NORMAL);

  m_SheetList.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);

  m_SheetList.SetRedraw(true);

}

#define BOOL_TO_TEXT(val) val ? OD_T("Yes") : OD_T("No")

void CPdfImportDialog::OnInsert() 
{
  CWnd* pWnd = GetFocus();

  if (GetDlgItem(IDOK) != pWnd)
  {
    if (GetDlgItem(IDC_EDIT_PAGE_NUM) == pWnd)
    {
      OnPageNumChanged();
    }
    return;
  }

	// TODO: Add your control notification handler code here
  UpdateData(true); // save and validate

  OdString layers_type(OD_T("UsePDFLayers"));
  switch (m_LayersUseType)
  {
    case 1:
      layers_type = OD_T("ObjectLayers");
      break;
    case 2:
      layers_type = OD_T("CurrentLayer");
      break;
  }

  OdString export_hyperlink_method(OD_T("ImportAsObjectsHyperlink"));
  switch (m_comboHyperLink.GetCurSel())
  {
    case 1:
      export_hyperlink_method = OD_T("ImportAsTrasparentHatch");
      break;
    case 2:
      export_hyperlink_method = OD_T("MixedMethod");
      break;
  }

  OdString font_replacemen_str;

  const bool has_replace_font1 = (!m_ReplaceFontName1.IsEmpty() || !m_ReplaceFontFamily1.IsEmpty()) && !m_ReplaceFontString1.IsEmpty();
  const bool has_replace_font2 = (!m_ReplaceFontName2.IsEmpty() || !m_ReplaceFontFamily2.IsEmpty()) && !m_ReplaceFontString2.IsEmpty();
  const bool has_replace_font3 = (!m_ReplaceFontName3.IsEmpty() || !m_ReplaceFontFamily3.IsEmpty()) && !m_ReplaceFontString3.IsEmpty();

  if (!m_Type3FontString.IsEmpty() || !m_NotFoundFontString.IsEmpty() || has_replace_font1 || has_replace_font2 || has_replace_font3)
    font_replacemen_str = OD_T("Yes ");

  if (!font_replacemen_str.isEmpty())
  {
    font_replacemen_str += m_Type3FontString + OD_T("\n");
  }

  if (!m_NotFoundFontString.IsEmpty())
  {
    if (font_replacemen_str.isEmpty())
      font_replacemen_str += OD_T("Yes ");

    font_replacemen_str += OD_T("Yes ") + m_NotFoundFontString + OD_T("\n");
  }
  else
  {
    if(!font_replacemen_str.isEmpty())
      font_replacemen_str += OD_T("No ");
  }

  OdString font_replacemen_for_specific_fonts_str;
  if (has_replace_font1)
  {
    font_replacemen_for_specific_fonts_str += OD_T("Yes ") + m_ReplaceFontName1 + OD_T("\n") + m_ReplaceFontFamily1 + OD_T("\n") + m_ReplaceFontString1 + OD_T("\n");
  }

  if (has_replace_font2)
  {
    font_replacemen_for_specific_fonts_str += OD_T("Yes ") + m_ReplaceFontName2 + OD_T("\n") + m_ReplaceFontFamily2 + OD_T("\n") + m_ReplaceFontString2 + OD_T("\n");
  }

  if (has_replace_font3)
  {
    font_replacemen_for_specific_fonts_str += OD_T("Yes ") + m_ReplaceFontName3 + OD_T("\n") + m_ReplaceFontFamily3 + OD_T("\n") + m_ReplaceFontString3 + OD_T("\n");
  }

  font_replacemen_for_specific_fonts_str += OD_T("No");

  font_replacemen_str += font_replacemen_for_specific_fonts_str;

  m_CommandString = OdString().format(OD_T("pdfin File %ls\n %ls %i %ls %ls Yes %ls %ls %ls %ls %ls %ls %ls %ls %ls %ls %ls %ls %ls %ls %ls %ls %ls %ls %ls %ls %ls %ls %ls %ls %ls"), m_Path, 
    m_Password.isEmpty()? OD_T("\n"):m_Password.c_str(), 
    (int)m_SelectedPageNum,
    m_pDb->formatter().formatLinear(OdaToRadian(m_Rotation)).c_str(), m_pDb->formatter().formatLinear(m_Scaling).c_str(),
    layers_type.c_str(),
    BOOL_TO_TEXT(m_ImportVectorGeometry),
    BOOL_TO_TEXT(m_ImportVectorGeometry && m_ImportSolidFills),
    BOOL_TO_TEXT(m_ImportTrueTypeText),
    BOOL_TO_TEXT(m_ImportRasterImages),
    BOOL_TO_TEXT(m_ImportGradientFills),
    BOOL_TO_TEXT(m_ImportWidgets),
    BOOL_TO_TEXT(m_ImportAsBlock),
    BOOL_TO_TEXT(m_JoinLineAndArcSegments),
    BOOL_TO_TEXT(m_ConvertSolidsToHatches),
    BOOL_TO_TEXT(m_ApplyLineweight),
    BOOL_TO_TEXT(m_ImportTrueTypeTextAsGeometry),
    BOOL_TO_TEXT(m_UseGeometryOptimization),
    BOOL_TO_TEXT(m_ImportType3AsTrueTypeText),
    BOOL_TO_TEXT(m_ImportAsRgb),
    BOOL_TO_TEXT(m_ImportInvalidTTTextAsGeometry),
    BOOL_TO_TEXT(m_UseClipping),
    BOOL_TO_TEXT(m_UsePageBorderClipping && m_UseClipping),
    BOOL_TO_TEXT(m_UseImageClipping && m_UseClipping),
    BOOL_TO_TEXT(m_CombineTextObjects),
    BOOL_TO_TEXT(m_UsePdfTransparency),
    export_hyperlink_method.c_str(),
    BOOL_TO_TEXT(m_UseINSUNITS),
    font_replacemen_str.c_str(),
    BOOL_TO_TEXT(m_ExtractEmbeddedFonts)
    );
  if (FALSE == m_UseInsertionPoint)
    m_CommandString += OD_T(" 0.,0.,0.");

  AfxGetApp()->WriteProfileInt(L"PdfImport", L"PDFIMPORTLAYERS", m_LayersUseType);
  m_LayersUseType = AfxGetApp()->GetProfileInt(L"PdfImport", L"PDFIMPORTLAYERS", 0);
  int import_filter(0);
  SETBIT(import_filter, 1, !m_ImportVectorGeometry);
  SETBIT(import_filter, 4, !m_ImportSolidFills);
  SETBIT(import_filter, 2, !m_ImportTrueTypeText);
  SETBIT(import_filter, 8, !m_ImportRasterImages);
  SETBIT(import_filter, 16, !m_ImportGradientFills);
  AfxGetApp()->WriteProfileInt(L"PdfImport", L"PDFIMPORTFILTER", import_filter);
  int import_mode(0);
  SETBIT(import_mode, 1, m_ImportAsBlock);
  SETBIT(import_mode, 4, m_JoinLineAndArcSegments);
  SETBIT(import_mode, 8, m_ConvertSolidsToHatches);
  SETBIT(import_mode, 2, m_ApplyLineweight);
  SETBIT(import_mode, 16, m_ImportTrueTypeTextAsGeometry);
  SETBIT(import_mode, 32, m_UseGeometryOptimization);
  SETBIT(import_mode, 64, m_ImportType3AsTrueTypeText);
  SETBIT(import_mode, 128, m_ImportAsRgb);
  m_LayersUseType = AfxGetApp()->WriteProfileInt(L"PdfImport", L"PDFIMPORTMODE", import_mode);

  int import_mode2(0);
  SETBIT(import_mode2, 1, m_ImportInvalidTTTextAsGeometry);
  SETBIT(import_mode2, 2, m_UseClipping);
  SETBIT(import_mode2, 4, m_UsePageBorderClipping);
  SETBIT(import_mode2, 8, m_CombineTextObjects);
  SETBIT(import_mode2, 16, m_UsePdfTransparency);
  SETBIT(import_mode2, 32, m_ImportWidgets);
  SETBIT(import_mode2, 64, m_UseImageClipping);
  SETBIT(import_mode2, 128, m_UseINSUNITS);
  AfxGetApp()->WriteProfileInt(L"PdfImport", L"PDFIMPORTMODE2", import_mode2);

  AfxGetApp()->WriteProfileInt(L"PdfImport", L"PDFIMPORTHYPERLINKMETHOD", m_comboHyperLink.GetCurSel());

  AfxGetApp()->WriteProfileStringW(L"PdfImport", L"Type3FontString", m_Type3FontString);
  AfxGetApp()->WriteProfileStringW(L"PdfImport", L"NotFoundFontString", m_NotFoundFontString);

  AfxGetApp()->WriteProfileStringW(L"PdfImport", L"ReplaceFontName1", m_ReplaceFontName1);
  AfxGetApp()->WriteProfileStringW(L"PdfImport", L"ReplaceFontFamily1", m_ReplaceFontFamily1);
  AfxGetApp()->WriteProfileStringW(L"PdfImport", L"ReplaceFontString1", m_ReplaceFontString1);

  AfxGetApp()->WriteProfileStringW(L"PdfImport", L"ReplaceFontName2", m_ReplaceFontName2);
  AfxGetApp()->WriteProfileStringW(L"PdfImport", L"ReplaceFontFamily2", m_ReplaceFontFamily2);
  AfxGetApp()->WriteProfileStringW(L"PdfImport", L"ReplaceFontString2", m_ReplaceFontString2);

  AfxGetApp()->WriteProfileStringW(L"PdfImport", L"ReplaceFontName3", m_ReplaceFontName3);
  AfxGetApp()->WriteProfileStringW(L"PdfImport", L"ReplaceFontFamily3", m_ReplaceFontFamily3);
  AfxGetApp()->WriteProfileStringW(L"PdfImport", L"ReplaceFontString3", m_ReplaceFontString3);

  int import_mode3(0);
  SETBIT(import_mode3, 1, m_ExtractEmbeddedFonts);
  AfxGetApp()->WriteProfileInt(L"PdfImport", L"PDFIMPORTMODE3", import_mode3);

  CPdfImportDialog::OnOK();

}

void CPdfImportDialog::OnCheckImportVectorGeometry()
{
  UpdateData(true);
  GetDlgItem(IDC_PDF_IMP_SOLID_FILLS)->EnableWindow(m_ImportVectorGeometry);
  UpdateData(false);
}

void CPdfImportDialog::OnCheckImportClipping()
{
  UpdateData(true);
  GetDlgItem(IDC_PDF_IMP_PAGE_BRD_CLIPPING)->EnableWindow(m_UseClipping);
  GetDlgItem(IDC_PDF_IMP_USE_IMG_CLIPPING)->EnableWindow(m_UseClipping);
  UpdateData(false);
}

void CPdfImportDialog::OnSelchangeSheetList(NMHDR* /*pNMHDR*/, LRESULT* /*pResult*/)
{
  UpdateData( true );

  POSITION pos = m_SheetList.GetFirstSelectedItemPosition();
  int page_num = 0;
  if (pos)
    page_num = m_SheetList.GetNextSelectedItem(pos);
  else
  {
    UpdateData(false);
    return;
  }

  if(m_SelectedPageNum != (page_num + 1))
    m_SelectedPageNum = page_num + 1;

#if defined (USE_PDFIUM)
  OdPdfImportModulePtr pModule = ::odrxDynamicLinker()->loadModule(OdPdfImportModuleName, false);
  PdfImporterExPtr pPdfImporter = pModule->create();
  if (!pPdfImporter.isNull())
  {
    OdString path((LPCTSTR)m_Path);
    pPdfImporter->properties()->putAt(OD_T("PdfPath"), OdRxVariantValue(path));
    pPdfImporter->properties()->putAt(OD_T("PageNumber"), OdRxVariantValue((OdUInt32)m_SelectedPageNum));
    pPdfImporter->properties()->putAt(OD_T("Password"), OdRxVariantValue(m_Password));
    
    pPdfImporter->loadDocument();
    pPdfImporter->loadPage();
    if (pPdfImporter->isPageLoaded())
    {
      static const double DPI = 72.;

      double page_width = pPdfImporter->getPageWidth()/DPI;
      double page_height = pPdfImporter->getPageHeight()/DPI;
      double measure = pPdfImporter->getMeasureDictInfo();

      OdDbPlotSettingsValidatorPtr pValidator = m_pDb->appServices()->plotSettingsValidator();
      OdDbPlotSettings plot_settings;

      OdString page_size_text;
      bool page_size_found = false;

      pValidator->setClosestMediaName(&plot_settings, page_width, page_height, OdDbPlotSettings::kInches, false);

      double paper_width, paper_height;
      plot_settings.getPlotPaperSize(paper_width, paper_height);
      double tmp_width = page_width * 25.4;
      double tmp_height = page_height * 25.4;
      if (OdDbPlotSettings::kMillimeters == plot_settings.plotPaperUnits())
      {
        tmp_width = OdRound(tmp_width);
        tmp_height = OdRound(tmp_height);
      }
      if (OdEqual(paper_width,  tmp_width, 1e-1) && OdEqual(paper_height, tmp_height, 1e-1))
        page_size_found = true;

      if(page_size_found)
      {
        page_size_text.format(OD_T("Page size: %ls"), pValidator->getLocaleMediaName(&plot_settings, plot_settings.getCanonicalMediaName()).c_str());
      }
      else
      {
        page_size_text.format(OD_T("Page size: %.2f x %.2f Inches"), OdRound(page_width * 100.) / 100, OdRound(page_height * 100.) / 100);
      }

      GetDlgItem(IDC_STATIC_PAGE_SIZE)->SetWindowText(page_size_text);

      OdString scale_text;
      if (-1 == page_size_text.find(L"Inches"))
      {
        double val1 = 25.4;
        double val2 = measure;
        double tmp = std::min(val1, val2);
        val1 = OdRound(10. * val1 / tmp) / 10.;
        val2 = OdRound(val2 / tmp);
        scale_text.format(OD_T("PDF scale: %g : %g"), val1, val2);
      }
      else
      {
        bool is_scale_found = false;
        if (!OdEqual(measure, 1., 1e-3))
        {
          OdDbObjectContextManagerPtr pManager = m_pDb->objectContextManager();
          if (!pManager.isNull())
          {
            OdSmartPtr<OdDbObjectContextCollection> pCollection = pManager->contextCollection(ODDB_ANNOTATIONSCALES_COLLECTION);
            if (!pCollection.isNull())
            {
              double tmp = 1. / OdRound(measure);
              OdDbObjectContextCollectionIteratorPtr it = pCollection->newIterator();
              for (; !it->done(); it->next())
              {
                double dScale = 1.0;
                OdDbAnnotationScalePtr pContext = it->getContext();
                if (!pContext.isNull() && pContext->getScale(dScale) == eOk && !pContext->getIsTemporaryScale())
                {
                  if (OdEqual(tmp, dScale, 1e-3))
                  {
                    is_scale_found = true;
                    scale_text.format(OD_T("PDF scale: %ls"), pContext->getName().c_str());
                    break;
                  }
                }
              }
            }
          }
        }
        if (!is_scale_found)
        {
          double val1 = 1;
          double val2 = measure;
          double tmp = std::min(val1, val2);
          val1 = OdRound(10. * val1 / tmp) / 10.;
          val2 = OdRound(val2 / tmp);
          scale_text.format(OD_T("PDF scale: %g : %g"), val1, val2);
        }
      }
      GetDlgItem(IDC_STATIC_SCALE_TEXT)->SetWindowText(scale_text);
    }
  }
#endif

  UpdateData( false );
}
