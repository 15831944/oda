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
#include "DwfUnderlayDialog.h"
#include ".\DwfUnderlayDialog.h"
#include "DbUnderlayHost.h"
#include "DbUnderlayDefinition.h"
#include "DbUnderlayReference.h"
#include "DbDictionary.h"
#include "DbBlockTableRecord.h"
#include "Gi\GiRasterWrappers.h"
#include "OdPath.h"
#include "DbPolyline.h"

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
// CDwfUnderlayDialog dialog


CDwfUnderlayDialog::CDwfUnderlayDialog(OdDbDatabase *pDb, CWnd* pParent /*=NULL*/, UnderlayType type)
	: CDialog(CDwfUnderlayDialog::IDD, pParent)
  , m_pDb(pDb), m_Type(type)
{
	//{{AFX_DATA_INIT(CDwfUnderlayDialog)
	m_Path = _T("");
  m_FileName = _T("");
  m_PathType = 0;
	//}}AFX_DATA_INIT
}


void CDwfUnderlayDialog::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CDwfUnderlayDialog)
  DDX_Text(pDX, IDC_EDIT_FILENAME, m_Path);
  DDX_Control(pDX, IDC_LIST_SHEETS, m_SheetList);
  DDX_Control(pDX, IDC_LIST_LAYER, m_LayerList);
  DDX_Control(pDX, IDC_PREVIEW, m_Preview);
  DDX_Radio(pDX, IDC_RADIO_FPATH, m_PathType);
  //}}AFX_DATA_MAP
}
/*
	DDX_Text(pDX, IDC_AUTHOR, m_Autor);
		DDV_MaxChars(pDX, m_Autor, 255);
		DDX_Text(pDX, IDC_KEYWORDS, m_Keywords);
		DDV_MaxChars(pDX, m_Keywords, 255);
		DDX_Text(pDX, IDC_CREATOR, m_Creator);
		DDV_MaxChars(pDX, m_Creator, 255);
		DDX_Text(pDX, IDC_TITLE, m_Title);
		DDV_MaxChars(pDX, m_Title, 255);
		DDX_Check(pDX, IDC_EMEDDED, m_bEmbedded);
		DDX_Check(pDX, IDC_GEOM_OPT, m_bSimpleGeomOpt);
		DDX_Check(pDX, IDC_SHX_AS_GEOM, m_bSHXAsGeometry);
		DDX_Check(pDX, IDC_TTF_AS_GEOM, m_bTTFAsGeometry);
	  DDX_Check(pDX, IDC_ZOOM, m_bZoomToExtents);
	
	  DDX_Check(pDX, IDC_LAYERS, m_bEnableLayers);
	  DDX_Check(pDX, IDC_OFFLAYERS, m_bExportOffLayers);
	  
		DDX_Text(pDX, IDC_PRODUCER, m_Producer);
		DDV_MaxChars(pDX, m_Producer, 255);
		DDX_Text(pDX, IDC_SUBJECT, m_Subject);
		DDV_MaxChars(pDX, m_Subject, 255);
		DDX_Text(pDX, IDC_EDIT_PAPER_HEIGHT, m_dPaperH);
		DDX_Text(pDX, IDC_EDIT_PAPER_WIDTH, m_dPaperW);
		DDX_Radio(pDX, IDC_RADIO_ACTIVE, m_Layouts);*/

BEGIN_MESSAGE_MAP(CDwfUnderlayDialog, CDialog)
	//{{AFX_MSG_MAP(CDwfUnderlayDialog)
  ON_WM_DRAWITEM()
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnBrowse)
	ON_BN_CLICKED(IDOK, OnInsert)
  ON_CBN_SELCHANGE(IDC_LIST_SHEETS, OnSelchangeSheetList)
  ON_NOTIFY(NM_CLICK, IDC_LIST_LAYER, OnClickListLayersList)

  ON_BN_CLICKED(IDC_RADIO_FPATH, OnClickPathType)
  ON_BN_CLICKED(IDC_RADIO_RPATH, OnClickPathType)
  ON_BN_CLICKED(IDC_RADIO_NPATH, OnClickPathType)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDwfUnderlayDialog message handlers

BOOL CDwfUnderlayDialog::OnInitDialog()
{
  CDialog::OnInitDialog();

  CRect rect, rectButton;
  GetWindowRect(&rect);
  GetDlgItem(IDCANCEL)->GetWindowRect(&rectButton);
  m_DialogHeight = rect.Height();
  rect.bottom = rectButton.bottom + 20;
  MoveWindow(&rect);

  m_LayerList.DeleteAllItems();
  m_LayerList.InsertColumn(0, _T("On"), LVCFMT_LEFT, 30);
  m_LayerList.InsertColumn(1, _T("Name"), LVCFMT_LEFT, 100);

  CBitmap bmp; 
  bmp.LoadBitmap( IDB_LAYER_STATES );
  m_stateImages.Create( 12, 12, ILC_COLOR16|ILC_MASK, 0, 1 );
  m_stateImages.Add( &bmp, RGB( 0, 0, 128 ) );

  GetDlgItem(IDC_LIST_SHEETS)->EnableWindow( FALSE );
  GetDlgItem(IDC_LIST_LAYER)->EnableWindow( FALSE );

  if (m_Type == eDgn)
  {
    CString caption;
    GetWindowText(caption);
    caption.Replace(_T("Dwf"), _T("Dgn"));
    SetWindowText(caption);

    GetDlgItemText(IDC_SDWFSHEETNAME, caption);
    caption.Replace(_T("sheet"), _T("model"));
    SetDlgItemText(IDC_SDWFSHEETNAME, caption);

    /*GetDlgItem(IDC_GBDGNUNITS)->ShowWindow(SW_SHOWNORMAL);
    GetDlgItem(IDC_RBDGNUNITS1)->ShowWindow(SW_SHOWNORMAL);
    GetDlgItem(IDC_RBDGNUNITS2)->ShowWindow(SW_SHOWNORMAL);
    CheckRadioButton(IDC_RBDGNUNITS1, IDC_RBDGNUNITS2, IDC_RBDGNUNITS1);*/
  }
  if (m_Type == ePdf)
  {
    CString caption;
    GetWindowText(caption);
    caption.Replace(_T("Dwf"), _T("Pdf"));
    SetWindowText(caption);

    // PDF preview is 170x170 pixels
    /*RECT Rect;
    GetWindowRect(&Rect);
    Rect.right += 60;
    MoveWindow(&Rect);

    GetDlgItem(IDC_DU_PROPERTIES)->GetWindowRect(&Rect);
    Rect.right += 60;
    GetDlgItem(IDC_DU_PROPERTIES)->SetWindowPos(0, Rect.left, Rect.top, Rect.right - Rect.left, Rect.bottom - Rect.top, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);

    GetDlgItem(IDC_PREVIEW)->GetWindowRect(&Rect);
    ScreenToClient(&Rect) ;
    Rect.top -= 60;
    GetDlgItem(IDC_PREVIEW)->SetWindowPos(0, Rect.left, Rect.top, Rect.right - Rect.left, Rect.bottom - Rect.top, SWP_NOACTIVATE | SWP_NOZORDER);*/
  }

  return TRUE;
}

void CDwfUnderlayDialog::DrawItem(CDC& dc, int itemID, int labelIndex, const RECT& rcItem) 
{
  int n = m_SheetList.GetCurSel();
  if (n == -1)
    return;

  OdDbUnderlayItemPtr pItem = m_pFile->getItem(n);

  if ((int)pItem->underlayLayerCount() <= itemID)
    return;

  OdUnderlayLayer layer;
  pItem->getUnderlayLayer(itemID, layer);
  CString sTextOut = (LPCTSTR)layer.name();

  switch (labelIndex)
  {
  case 1:
    dc.ExtTextOut(rcItem.left+6, rcItem.top+1, ETO_CLIPPED, &rcItem, sTextOut, sTextOut.GetLength(), NULL);
    break;
  case 0:
    m_stateImages.Draw(&dc, m_Visibility[itemID] ? 2 : 3, ((CRect&)rcItem).TopLeft(), ILD_TRANSPARENT);
    break;
  }
}

void CDwfUnderlayDialog::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
  if (IDC_LIST_LAYER == nIDCtl)
  {
    CRect rcItem;
    switch (lpDrawItemStruct->itemAction) 
    {
    case ODA_DRAWENTIRE:
      {
        //clear item
        rcItem = lpDrawItemStruct->rcItem;
        CDC dc;
        COLORREF rgbBkgnd = ::GetSysColor((lpDrawItemStruct->itemState & ODS_SELECTED) ?
        COLOR_HIGHLIGHT : COLOR_WINDOW);
        dc.Attach(lpDrawItemStruct->hDC);
        CBrush br(rgbBkgnd);
        dc.FillRect(rcItem, &br);
        if( lpDrawItemStruct->itemState & ODS_FOCUS )
        {
          dc.DrawFocusRect(rcItem);
        }
        int itemID = lpDrawItemStruct->itemID;
        if( itemID != -1 )
        {
          // The text color is stored as the item data.
          COLORREF rgbText = (lpDrawItemStruct->itemState & ODS_SELECTED) ?
            ::GetSysColor(COLOR_HIGHLIGHTTEXT) : ::GetSysColor(COLOR_WINDOWTEXT);
          dc.SetBkColor(rgbBkgnd);
          dc.SetTextColor(rgbText);
          for (int labelIndex = 0; labelIndex < 2; ++labelIndex)
          {
            m_LayerList.GetSubItemRect(itemID, labelIndex, LVIR_LABEL, rcItem);
            DrawItem(dc, itemID, labelIndex, rcItem);
          }
        }
        dc.Detach();
      }
      break;
    case ODA_SELECT:
      ::InvertRect(lpDrawItemStruct->hDC, &(lpDrawItemStruct->rcItem));
      break;
    case ODA_FOCUS:
      //::DrawFocusRect(lpDrawItemStruct->hDC, &(lpDrawItemStruct->rcItem));
      break;
    }

    return;
  }
  CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

void CDwfUnderlayDialog::OnBrowse() 
{
	// TODO: Add your control notification handler code here
  UpdateData(true); // save and validate

  CFileDialog dlgFile(true);

  dlgFile.m_ofn.Flags |= OFN_HIDEREADONLY | OFN_EXPLORER | OFN_FILEMUSTEXIST;


  static TCHAR* strFilter[] = { _T("DGN files (*.dgn)\0*.dgn\0"),  _T("DWF files (*.dwf)\0*.dwf\0"), _T("PDF files (*.pdf)\0*.pdf\0") };
  static TCHAR* lpstrDefExt[] = { _T("dgn"),  _T("dwf"), _T("pdf") };
  static TCHAR* lpstrTitle[] = { _T("Select dgn file"),  _T("Select dwf file"), _T("Select pdf file") };

  dlgFile.m_ofn.lpstrFilter = strFilter[m_Type];
  dlgFile.m_ofn.lpstrDefExt = lpstrDefExt[m_Type];
  dlgFile.m_ofn.lpstrTitle = lpstrTitle[m_Type];

  if (dlgFile.DoModal() == IDOK)
  {
    GetDlgItem(IDC_LIST_SHEETS)->EnableWindow( FALSE );
    GetDlgItem(IDC_LIST_LAYER)->EnableWindow( FALSE );

    m_Path = dlgFile.GetPathName();
    m_FileName = dlgFile.GetFileName();
    bool bRelpath = Oda::makeRelativePath(m_pDb->getFilename(), OdString(m_Path), m_RelativePath, true);
    m_RelativePath.trimRight("\\");
    m_RelativePath.trimLeft(".");
    m_RelativePath.trimLeft("\\");


    bool bInit = InitDwfProperties();

    if (bInit)
    {
      GetDlgItem(IDC_STATIC_P2S)->ShowWindow(SW_SHOW);
      GetDlgItem(IDC_STATIC_PATH2SAVE)->ShowWindow(SW_SHOW);
      GetDlgItem(IDC_STATIC_PATH_TYPE)->ShowWindow(SW_SHOW);
      GetDlgItem(IDC_RADIO_FPATH)->ShowWindow(SW_SHOW);
      GetDlgItem(IDC_RADIO_RPATH)->ShowWindow(SW_SHOW);
      GetDlgItem(IDC_RADIO_NPATH)->ShowWindow(SW_SHOW);
      GetDlgItem(IDC_STATIC_PATH2SAVE)->SetWindowText(m_Path);

      GetDlgItem(IDC_DU_PROPERTIES)->ShowWindow(SW_SHOW);
      GetDlgItem(IDC_SDWFSHEETNAME)->ShowWindow(SW_SHOW);
      GetDlgItem(IDC_LIST_SHEETS)->ShowWindow(SW_SHOW);
      GetDlgItem(IDC_LIST_LAYER)->ShowWindow(SW_SHOW);

      if (m_Type == eDgn)
      {
        GetDlgItem(IDC_GBDGNUNITS)->ShowWindow(SW_SHOWNORMAL);
        GetDlgItem(IDC_RBDGNUNITS1)->ShowWindow(SW_SHOWNORMAL);
        GetDlgItem(IDC_RBDGNUNITS2)->ShowWindow(SW_SHOWNORMAL);
        CheckRadioButton(IDC_RBDGNUNITS1, IDC_RBDGNUNITS2, IDC_RBDGNUNITS1);
      }

      if (m_pDb->getFilename().isEmpty() || !bRelpath)
        GetDlgItem(IDC_RADIO_RPATH)->EnableWindow(FALSE);
      else
        GetDlgItem(IDC_RADIO_RPATH)->EnableWindow(TRUE);

      GetDlgItem(IDOK)->EnableWindow(TRUE);

      CRect rect;
      GetWindowRect(&rect);
      rect.bottom += m_DialogHeight;
      m_DialogHeight = 0;
      MoveWindow(&rect);
    }
  }
}

void CDwfUnderlayDialog::OnError(CString str)
{
  MessageBox(str, CString("Error"));
}

void CDwfUnderlayDialog::OnError(OdResult res)
{
  OdError err(res);
  CString str((LPCTSTR)err.description());
  OnError(str);
}

bool CDwfUnderlayDialog::InitDwfProperties()
{
  // ask for UnderlayHostPE
  OdDbUnderlayHostPEPtr pHost;
  if (m_Type == eDwf)
  {
    ::odrxDynamicLinker()->loadModule(OdDwfUnderlayModuleName);
    pHost = OdDbDwfDefinition::desc()->getX(OdDbUnderlayHostPE::desc());
  }
  else if (m_Type == eDgn)
  {
    ::odrxDynamicLinker()->loadModule(OdDgnUnderlayModuleName);
    pHost = OdDbDgnDefinition::desc()->getX(OdDbUnderlayHostPE::desc());
  }
  else
  {
    //::odrxDynamicLinker()->loadModule(OdPdfModuleVIModuleName);
    OdDbPdfDefinition::loadPdfUnderlayModule();

    pHost = OdDbPdfDefinition::desc()->getX(OdDbUnderlayHostPE::desc());
  }

  if(!pHost.get())
  {
    static TCHAR* error[] = { _T("TD_DgnUnderlay.tx is not loaded."),  _T("TD_DwfUnderlay.tx is not loaded."), _T("PdfUnderlayModule is not loaded.") };

    OnError(error[m_Type]); 
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
          OnError("Password protected file."); return false;
        }

        // try to load again with new password
        res = pHost->load(*m_pDb, (LPCTSTR)m_Path, odPass, m_pFile);
        if (res == eInvalidPassword)
        { 
          OnError("Invalid password."); return false;
        }
        else
        {
          pCache->add(odPass);
        }
      }
    }

    if(res != eOk)
    {
      OnError("OdDbUnderlayHostPE cannot load file"); return false;
    }
  }

  FillSheetList();
  FillLayerList();
  updatePreview();

  UpdateData(false); // save and validate
  GetDlgItem(IDC_LIST_SHEETS)->EnableWindow( m_pFile->getItemCount() != 0 );
  GetDlgItem(IDC_LIST_LAYER)->EnableWindow( m_LayerList.GetItemCount() != 0 );
  return true;
}

void CDwfUnderlayDialog::FillSheetList()
{
  m_SheetList.ResetContent();
  if (!m_pFile->getItemCount())
  {
    OnError("No valid sheets in selected file."); return;
  }

  for(int f=0; f<m_pFile->getItemCount(); ++f)
  {
    OdDbUnderlayItemPtr pItem = m_pFile->getItem(f);
    OdString name = pItem->getName();
    m_SheetList.InsertString(f, (LPCTSTR)name);
  }
  m_SheetList.SetCurSel(0);
}

void CDwfUnderlayDialog::FillLayerList()
{
  m_LayerList.DeleteAllItems();

  int n = m_SheetList.GetCurSel();
  if (n == -1)
    return;

  CString text;
  m_SheetList.GetText(n, text);

  OdDbUnderlayItemPtr pItem = m_pFile->getItem(n);

  OdUInt32 layerC = pItem->underlayLayerCount();
  m_Visibility.clear();
  m_Visibility.resize(layerC, true);
  for (OdUInt32 f = 0; f < layerC; ++f)
  {
    OdUnderlayLayer layer;
    pItem->getUnderlayLayer(f, layer);
    CString str = (LPCTSTR)layer.name();
    m_LayerList.InsertItem(f, str, 0);
    m_Visibility[f] = layer.state() == OdUnderlayLayer::kOn;
  }
  m_LayerList.RedrawItems(0, layerC - 1);
}

void CDwfUnderlayDialog::OnInsert() 
{
	// TODO: Add your control notification handler code here
  UpdateData(true); // save and validate

  if (m_Path.IsEmpty())
  {
    MessageBox(_T("Please choose underlay file first."), _T("No file selected"), MB_OK | MB_ICONEXCLAMATION);
    return;
  }

  if (m_pDb)
  {
    OdDbObjectId spaceId = m_pDb->getActiveLayoutBTRId();
    OdDbBlockTableRecordPtr pSpace = spaceId.safeOpenObject(OdDb::kForWrite);

    OdString path = m_Path;
    OdString shortFileName;
    // under windows in the path there may be both '\' and '/' separators
    {
      int i1 = path.reverseFind('\\');
      int i2 = path.reverseFind('/');
      int i = odmax(i1, i2);
      shortFileName = path.mid(i+1);
      path = path.left(i+1);
      i = shortFileName.find(L'.');
      if (i != -1)
        shortFileName = shortFileName.left(i);
    }

    OdDbUnderlayDefinitionPtr pDef = getDef();

    CString text;
    m_SheetList.GetText(m_SheetList.GetCurSel(), text);

    OdDbObjectId definitionId;

    bool bStop = false;
    int nCurIndx = 0;
    while(!bStop)
    {
      OdString dictName;
      if (nCurIndx == 0)
        dictName = shortFileName + OD_T(" - ") + (LPCTSTR)text;
      else
        dictName.format(OD_T("%ls_%d - %ls"), shortFileName.c_str(), nCurIndx, (LPCTSTR)text);

      try
      {
        definitionId = pDef->postDefinitionToDb(m_pDb, dictName);
        bStop = true;
      }
      catch(OdError e)
      {
        if (e.code() == eDuplicateKey)
        {
          OdString dicDefName = pDef->dictionaryKey(pDef->isA());
          OdDbDictionaryPtr pDict = m_pDb->getNamedObjectsDictionaryId().safeOpenObject();
          OdDbDictionaryPtr pDefDict = pDict->getAt(dicDefName, OdDb::kForRead);

          OdDbUnderlayDefinitionPtr pOldDef = pDefDict->getAt(dictName, OdDb::kForRead);

          if (pOldDef->getSourceFileName().iCompare( pDef->getSourceFileName() ) == 0
            &&pOldDef->getItemName().iCompare( pDef->getItemName() ) == 0)
          {
            definitionId = pOldDef->objectId();
            bStop = true;
          }
          else
          {
            ++nCurIndx;
          }
        }
        else
        {
          MessageBox(_T("Cannot add definition."), _T("Error"), MB_OK | MB_ICONEXCLAMATION);
          bStop = true;
        }
      }
    }

    if (!definitionId.isNull())
    {
      OdDbUnderlayReferencePtr pRef;
      if (m_Type == eDwf)
      {
        pRef = OdDbDwfReference::createObject();
      }
      else if (m_Type == eDgn)
      {
        pRef = OdDbDgnReference::createObject();
      }
      else
      {
        pRef = OdDbPdfReference::createObject();
      }
      pRef->setDatabaseDefaults(m_pDb);
      pSpace->appendOdDbEntity(pRef);
      pRef->setDefinitionId(definitionId);

      for(int f=0; f<(int)m_Visibility.size(); ++f)
      {
        OdUnderlayLayer layer;
        pRef->getUnderlayLayer(f, layer);
        layer.setState(m_Visibility[f] ? OdUnderlayLayer::kOn : OdUnderlayLayer::kOff);
        pRef->setUnderlayLayer(f, layer);
      }

      if (m_Type == eDgn)
      {
        OdDbDgnDefinitionPtr pDgnDef = pDef;
        pDgnDef->setUseMasterUnits(IsDlgButtonChecked(IDC_RBDGNUNITS1) == BST_CHECKED);
      }
    }
  }
  
  CDwfUnderlayDialog::OnOK();
}

void CDwfUnderlayDialog::OnCheckLayers()
{
  UpdateData( );

  //GetDlgItem(IDC_OFFLAYERS)->EnableWindow( m_bEnableLayers );
}

OdDbUnderlayDefinitionPtr CDwfUnderlayDialog::getDef() const
{
  CString text;
  m_SheetList.GetText(m_SheetList.GetCurSel(), text);

  OdDbUnderlayDefinitionPtr pDef;
  if (m_Type == eDwf)
  {
    pDef = OdDbDwfDefinition::createObject();
  }
  else if (m_Type == eDgn)
  {
    pDef = OdDbDgnDefinition::createObject();
  }
  else
  {
    pDef = OdDbPdfDefinition::createObject();
  }
  CString path;
  GetDlgItemText(IDC_STATIC_PATH2SAVE, path);
  pDef->setSourceFileName((LPCTSTR)path);
  pDef->setItemName((LPCTSTR)text);

  return pDef;
}

void CDwfUnderlayDialog::updatePreview()
{
  OdGiRasterImagePtr pImage;
  CString text;
  m_SheetList.GetText(m_SheetList.GetCurSel(), text);
  OdDbUnderlayItemPtr pItem = m_pFile->getItem((LPCTSTR)text);
  if (!pItem.isNull())
    pImage = pItem->getThumbnail();

  if (!pImage.isNull())
  {
    OdSmartPtr<OdGiRasterImageDesc> pDesc = OdRxObjectImpl<OdGiRasterImageDesc>::createObject();
    pDesc->setPixelWidth(pImage->pixelWidth());
    pDesc->setPixelHeight(pImage->pixelHeight());
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

      DeleteObject(m_Preview.SetBitmap(hBmp));
    }

    RECT Rect;
    m_Preview.GetWindowRect(&Rect);
    m_Preview.GetOwner()->ScreenToClient(&Rect);
    m_Preview.MoveWindow(Rect.left,Rect.top,pImage->pixelWidth(), pImage->pixelHeight());  
  }

  m_Preview.ShowWindow(pImage.isNull() ? SW_HIDE : SW_SHOW);
}

void CDwfUnderlayDialog::OnClickPathType()
{
  UpdateData();
  switch (m_PathType)
  {
    case 0:
      GetDlgItem(IDC_STATIC_PATH2SAVE)->SetWindowText(m_Path);
    break;
    case 1:
      GetDlgItem(IDC_STATIC_PATH2SAVE)->SetWindowText(m_RelativePath);
    break;
    case 2:
      GetDlgItem(IDC_STATIC_PATH2SAVE)->SetWindowText(m_FileName);
    break;
    default:
    break;
  }
}

void CDwfUnderlayDialog::OnSelchangeSheetList()
{
  UpdateData( FALSE );

  updatePreview();

  FillLayerList();

  UpdateData( TRUE );
}

bool CDwfUnderlayDialog::hittestSubitem(CListCtrl& listLayersList, int columns_count, const CPoint& pt, int &index, int &labelIndex) 
{
  index = listLayersList.GetTopIndex();
  int last_visible_index = index + listLayersList.GetCountPerPage();
  if (last_visible_index > listLayersList.GetItemCount())
  {
    last_visible_index = listLayersList.GetItemCount();
  }

  while (index <= last_visible_index)
  {
    // Get the bounding rectangle of an item. If the mouse
    // location is within the bounding rectangle of the item,
    // you know you have found the item that was being clicked.
    CRect rectItem;
    listLayersList.GetItemRect(index, &rectItem, LVIR_BOUNDS);
    if (rectItem.PtInRect(pt))
    {
      CRect rectColumn;
      for (labelIndex = 0; labelIndex < columns_count; ++labelIndex)
      {
        listLayersList.GetSubItemRect(index, labelIndex, LVIR_LABEL, rectColumn);
        if (rectColumn.PtInRect(pt))
        {
          return true;
        }
      }
      //UINT flag = LVIS_SELECTED | LVIS_FOCUSED;
      //listLayersList.SetItemState(index, flag, flag);
      break;
    }

    // Get the next item in listview control.
    index++;
  }
  return false;
}

void CDwfUnderlayDialog::OnClickListLayersList(NMHDR* /*pNMHDR*/, LRESULT* pResult) 
{
  DWORD pos = GetMessagePos();

  CPoint pt(GET_X_LPARAM(pos), GET_Y_LPARAM(pos));
  m_LayerList.ScreenToClient(&pt);

  int index, labelIndex;
  if(hittestSubitem(m_LayerList, 2, pt, index, labelIndex))
  {
    m_Visibility[index] = !m_Visibility[index];
    m_LayerList.RedrawItems(index, index);
  }

  *pResult = 0;
}
