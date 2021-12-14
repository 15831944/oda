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


// CPdfRasterAttachmentDialog.cpp : implementation file
//

#include "stdafx.h"
#include "CPdfRasterAttachmentDialog.h"
#include "odadgnapp.h"

#include "Gi\GiRasterWrappers.h"

CPdfRasterAttachmentDialog::CPdfRasterAttachmentDialog(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_PDF_ATTACH_DIALOG, pParent)
{
}

CPdfRasterAttachmentDialog::~CPdfRasterAttachmentDialog()
{
}

void CPdfRasterAttachmentDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

  DDX_Text(pDX, IDC_PDF_FILENAME, m_Path);
  DDX_Control(pDX, IDC_LIST_PDF_PAGES, m_SheetList);
  DDX_Control(pDX, IDC_LIST_LAYERS, m_LayerList);
  DDX_Control(pDX, IDC_PDF_PREVIEW, m_Preview);
}

void CPdfRasterAttachmentDialog::setData(CString path, CString fName, OdDgDatabasePtr pDb)
{
  m_Path = path;
  m_FileName = fName;
  m_pDb = pDb;
}

BOOL CPdfRasterAttachmentDialog::OnInitDialog()
{
  CDialog::OnInitDialog();

  m_LayerList.DeleteAllItems();
  m_LayerList.InsertColumn(0, _T("On"), LVCFMT_LEFT, 30);
  m_LayerList.InsertColumn(1, _T("Name"), LVCFMT_LEFT, 130);

  CBitmap bmp;
  bmp.LoadBitmap(IDB_LAYER_STATES);
  m_stateImages.Create(12, 12, ILC_COLOR16 | ILC_MASK, 0, 1);
  m_stateImages.Add(&bmp, RGB(0, 0, 128));

  GetDlgItem(IDC_LIST_PDF_PAGES)->EnableWindow(FALSE);
  GetDlgItem(IDC_LIST_LAYERS)->EnableWindow(FALSE);

#if defined (PDFIUM_MODULE_ENABLED)
  OdRxPdfToRasterServicesPtr pSvcs = odrxDynamicLinker()->loadApp(RX_PDF_TO_RASTER_SERVICES_APPNAME);
  if (!pSvcs.isNull())
  {
    m_pConverter = pSvcs->createConverter();
    if (!m_pConverter.isNull())
    {
      OdString path(m_Path, m_Path.GetLength());
      if (m_pConverter->loadPdf(path) == eOk)
      {
        FillSheetList();
        FillLayerList();
        updatePreview();

        UpdateData(false); // save and validate
        GetDlgItem(IDC_LIST_PDF_PAGES)->EnableWindow(m_pConverter->getPagesCount() != 0);
        GetDlgItem(IDC_LIST_LAYERS)->EnableWindow(m_LayerList.GetItemCount() != 0);
      }
    }
  }
#else
  MessageBox(_T("PdfToImageConverter is unavailble"));
#endif

  return TRUE;
}

void CPdfRasterAttachmentDialog::DrawItem(CDC& dc, int itemID, int labelIndex, const RECT& rcItem)
{
  int n = m_SheetList.GetCurSel();
  if (n == -1)
    return;

  CString sTextOut = (LPCTSTR)m_layers[itemID].layer_name;

  switch (labelIndex)
  {
  case 1:
    dc.ExtTextOut(rcItem.left + 6, rcItem.top + 1, ETO_CLIPPED, &rcItem, sTextOut, sTextOut.GetLength(), NULL);
    break;
  case 0:
    m_stateImages.Draw(&dc, m_layers[itemID].is_on ? 2 : 3, ((CRect&)rcItem).TopLeft(), ILD_TRANSPARENT);
    break;
  }
}

void CPdfRasterAttachmentDialog::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
  if (IDC_LIST_LAYERS == nIDCtl)
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
      if (lpDrawItemStruct->itemState & ODS_FOCUS)
      {
        dc.DrawFocusRect(rcItem);
      }
      int itemID = lpDrawItemStruct->itemID;
      if (itemID != -1)
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

void CPdfRasterAttachmentDialog::FillSheetList()
{
  m_SheetList.ResetContent();
#if defined (PDFIUM_MODULE_ENABLED)
  if (!m_pConverter->getPagesCount())
  {
    MessageBox(_T("No valid sheets in selected file."), _T("Error"), MB_OK);
    return;
  }

  char buffer[4];
  for (int f = 1; f <= m_pConverter->getPagesCount(); ++f)
  {
    memset(buffer, 0, 4);
    _itoa_s(f, buffer, _countof(buffer), 10);
    OdString buf(buffer);
    m_SheetList.InsertString(f-1, (LPCTSTR)buf);
  }
  m_SheetList.SetCurSel(0);
#endif
}

void CPdfRasterAttachmentDialog::FillLayerList()
{
  m_LayerList.DeleteAllItems();

  int n = m_SheetList.GetCurSel();
  if (n == -1)
    return;

#if defined (PDFIUM_MODULE_ENABLED)
  CString text;
  m_SheetList.GetText(m_SheetList.GetCurSel(), text);

  m_pConverter->setActivePage(odStrToUInt(text));
  m_layers.clear();
  m_pConverter->getLayers(m_layers);

  OdUInt32 layerC = m_layers.size();
  for (OdUInt32 f = 0; f < layerC; ++f)
  {
    CString str = (LPCTSTR)m_layers[f].layer_name;
    m_LayerList.InsertItem(f, str, 0);
   }
   m_LayerList.RedrawItems(0, layerC - 1);
#endif
}

void CPdfRasterAttachmentDialog::OnInsert()
{
  UpdateData(true); // save and validate

  if (m_Path.IsEmpty())
  {
    MessageBox(_T("Please choose underlay file first."), _T("No file selected"), MB_OK | MB_ICONEXCLAMATION);
    return;
  }

  if (!m_pDb.isNull())
  {
    OdDgModelPtr pModel = m_pDb->getActiveModelId().safeOpenObject(OdDg::kForWrite);
    OdDgRasterFramePtr pRasterFrame = OdDgRasterFrame::createObject();

    OdDgRasterAttachmentHeaderPtr raster = OdDgRasterAttachmentHeader::createObject();

    pModel->addElement(raster);
    pModel->addElement(pRasterFrame);

    raster->setFilename(OdString(m_FileName));
    raster->setFilePath(OdString(m_Path));

    OdGeVector3d u(1., 0., 0.);
    OdGeVector3d v(0., 1., 0.);
    //////////////////////////////////////////////////////////////////////////
#if defined (PDFIUM_MODULE_ENABLED)
    OdGsDCRect pageSize;
    if (eOk == m_pConverter->getPageSize(pageSize)) //get pdf page size in 1/72-inch units
    {
      //calculate size in meters
      double width = (double)pageSize.m_max.x * .0254 / 72.;
      double height = (double)pageSize.m_max.y  * .0254 / 72.;

      OdDgModel::StorageUnitDescription description;
      pModel->getStorageUnit(description);

      //calculate size in master units
      width = width * description.m_numerator / description.m_denominator;
      height = height * description.m_numerator / description.m_denominator;

      raster->setExtent(OdGePoint2d(width, height));

      int iDefaultDpi = 96;

      DISPLAY_DEVICE dd;
      dd.cb = sizeof(dd);
      if (EnumDisplayDevices(NULL, 0, &dd, 0) != FALSE)
      {
        HDC hDC = CreateDC(dd.DeviceName, dd.DeviceName, NULL, NULL);
        int iDpiX = GetDeviceCaps(hDC, LOGPIXELSX);
        int iDpiY = GetDeviceCaps(hDC, LOGPIXELSY);
        DeleteDC(hDC);
        iDefaultDpi = iDpiX > iDpiY ? iDpiX : iDpiY;
      }

      pRasterFrame->setScanningResolution(OdGePoint2d(iDefaultDpi, iDefaultDpi));//default system resolution
    }
#endif
    //////////////////////////////////////////////////////////////////////////
    //raster->setOrientation(OdGePoint3d::kOrigin, OdGeVector3d(.0003, 0., 0.), OdGeVector3d(0., 0.0003, 0.));

    pRasterFrame->setRasterReferenceId(raster->id());

    pRasterFrame->setPrintableFlag(true);
    pRasterFrame->setPageNumber(m_SheetList.GetCurSel());
    for (OdUInt16 i = 0; i < m_layers.size(); i++)
      pRasterFrame->setPdfLayerVisibility(m_layers[i].layer_name, m_layers[i].is_on);

  //raster->unload();
  //raster->load();

    pModel->fitToView();
  }

  CPdfRasterAttachmentDialog::OnOK();
}

void CPdfRasterAttachmentDialog::updatePreview()
{
  OdGiRasterImagePtr pImage;
#if defined (PDFIUM_MODULE_ENABLED)
  CString text;
  m_SheetList.GetText(m_SheetList.GetCurSel(), text);

  m_pConverter->setActivePage(odStrToUInt(text));
  OdGsDCRect pageSize;
  m_pConverter->getPageSize(pageSize);
  double scale = pageSize.m_max.y > pageSize.m_max.x ? (double)pageSize.m_max.y / 260. : (double)pageSize.m_max.x / 260.;

  OdPdf2ImageConversionParams params;
  params.bg_color = 0xFFFFFFFF;
  params.dpi = 72./scale;
  params.flags = (int)(kDisableAAforText | kDisableAAforImage | kDisableAAforGeom);
  params.layers = m_layers;

  pImage = m_pConverter->convertPdf(&params);
#endif

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
    m_Preview.MoveWindow(Rect.left, Rect.top, pImage->pixelWidth(), pImage->pixelHeight());
  }
  m_Preview.ShowWindow(pImage.isNull() ? SW_HIDE : SW_SHOW);
}

void CPdfRasterAttachmentDialog::OnSelchangeSheetList()
{
  UpdateData(FALSE);
  updatePreview();
  FillLayerList();
  UpdateData(TRUE);
}

void CPdfRasterAttachmentDialog::OnClickListLayersList(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
  DWORD pos = GetMessagePos();

  CPoint pt(GET_X_LPARAM(pos), GET_Y_LPARAM(pos));
  m_LayerList.ScreenToClient(&pt);

  int index, labelIndex;
  if (hittestSubitem(m_LayerList, 2, pt, index, labelIndex))
  {
    m_layers[index].is_on = !m_layers[index].is_on;
    m_LayerList.RedrawItems(index, index);
    updatePreview();
  }

  *pResult = 0;
}

bool CPdfRasterAttachmentDialog::hittestSubitem(CListCtrl& listLayersList, int columns_count, const CPoint& pt, int &index, int &labelIndex)
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
      break;
    }
    // Get the next item in listview control.
    index++;
  }
  return false;
}

BEGIN_MESSAGE_MAP(CPdfRasterAttachmentDialog, CDialog)
  ON_WM_DRAWITEM()
  ON_BN_CLICKED(IDOK, OnInsert)
  ON_CBN_SELCHANGE(IDC_LIST_PDF_PAGES, OnSelchangeSheetList)
  ON_NOTIFY(NM_CLICK, IDC_LIST_LAYERS, OnClickListLayersList)
END_MESSAGE_MAP()

// CPdfRasterAttachmentDialog message handlers
