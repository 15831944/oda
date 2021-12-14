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

// SnapshotDlg.cpp : implementation file
//

#include "stdafx.h"
#include "odamfcapp.h"
#include "SnapshotDlg.h"

#include "Gi/GiRasterWrappers.h"
#include "RxRasterServices.h"
#include "DynamicLinker.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CPreviewGiRasterImage

IMPLEMENT_DYNAMIC(CPreviewGiRasterImage, CStatic)
CPreviewGiRasterImage::CPreviewGiRasterImage()
{
}

CPreviewGiRasterImage::~CPreviewGiRasterImage()
{
}

BEGIN_MESSAGE_MAP(CPreviewGiRasterImage, CStatic)
  ON_WM_CREATE()
  ON_WM_DESTROY()
  ON_WM_PAINT()
END_MESSAGE_MAP()

// CPreviewGiRasterImage message handlers

int CPreviewGiRasterImage::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if (CStatic::OnCreate(lpCreateStruct) == -1)
    return -1;
  // . . .
  return 0;
}

void CPreviewGiRasterImage::OnDestroy()
{
  CStatic::OnDestroy();
}

void CPreviewGiRasterImage::SetImage(OdGiRasterImage *pImage)
{
  m_pImage = pImage;
  Invalidate();
}

void CPreviewGiRasterImage::OnPaint()
{
  CPaintDC dc(this);
  CRect cliArea;
  GetClientRect(&cliArea);
  dc.FillSolidRect(&cliArea, ::GetSysColor(COLOR_BTNFACE));
  if (!m_pImage.isNull())
  {
    // Convert to BGR representation
    OdSmartPtr<OdGiRasterImageDesc> pDesc = OdGiRasterImageDesc::createObject(m_pImage->pixelWidth(), m_pImage->pixelHeight(), 24);
    pDesc->setScanLinesAlignment(4);
    pDesc->pixelFormat().setBGR();
    OdGiRasterImagePtr pAcImage = m_pImage->convert(true, 50.0, 50.0, 0.0, 0, false, false, false, pDesc);
    // Convert to DIB representation
    BITMAPINFO biInfo;
    ZeroMemory(&biInfo, sizeof(BITMAPINFO));
    biInfo.bmiHeader.biBitCount = 24;
    biInfo.bmiHeader.biHeight = pAcImage->pixelHeight();
    biInfo.bmiHeader.biPlanes = 1;
    biInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    biInfo.bmiHeader.biSizeImage = pAcImage->scanLineSize() * pAcImage->pixelHeight();
    biInfo.bmiHeader.biWidth = pAcImage->pixelWidth();
    // Compute output rectangle
    CRect outArea(0, 0, m_pImage->pixelWidth(), m_pImage->pixelHeight());
    //if (outArea.Width() > cliArea.Width())
    {
      float scale = (float)outArea.Width() / cliArea.Width();
      outArea.right = (int)(outArea.right / scale);
      outArea.bottom = (int)(outArea.bottom / scale);
    }
    if (outArea.Height() > cliArea.Height())
    {
      float scale = (float)outArea.Height() / cliArea.Height();
      outArea.right = (int)(outArea.right / scale);
      outArea.bottom = (int)(outArea.bottom / scale);
    }
    outArea.MoveToXY(cliArea.CenterPoint().x - outArea.Width() / 2, cliArea.CenterPoint().y - outArea.Height() / 2);
    ::SetStretchBltMode(dc.GetSafeHdc(), HALFTONE);
    if (pAcImage->scanLines())
      ::StretchDIBits(dc.GetSafeHdc(), outArea.left, outArea.top, outArea.Width(), outArea.Height(),
        0, 0, pAcImage->pixelWidth(), pAcImage->pixelHeight(), pAcImage->scanLines(), &biInfo, DIB_RGB_COLORS, SRCCOPY);
    else
    {
      OdUInt8Array tmpArray;
      tmpArray.resize(biInfo.bmiHeader.biSizeImage);
      pAcImage->scanLines(tmpArray.asArrayPtr(), 0, pAcImage->pixelHeight());
      ::StretchDIBits(dc.GetSafeHdc(), outArea.left, outArea.top, outArea.Width(), outArea.Height(),
        0, 0, pAcImage->pixelWidth(), pAcImage->pixelHeight(), tmpArray.getPtr(), &biInfo, DIB_RGB_COLORS, SRCCOPY);
    }
  }
}

/////////////////////////////////////////////////////////////////////////////
// CSnapshotDlg dialog

CSnapshotDlg::CSnapshotDlg(OdGsDevice* pDevice, HWND pParentWnd, CWnd* pParent /*=NULL*/)
	: CDialog(CSnapshotDlg::IDD, pParent)
  , m_pDevice(pDevice)
  , m_pParentWnd(pParentWnd)
{
	//{{AFX_DATA_INIT(CSnapshotDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSnapshotDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSnapshotDlg)
	DDX_Control(pDX, IDC_LBVIEWS, m_ViewsList);
	DDX_Control(pDX, IDC_SPREVIEW, m_Preview);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSnapshotDlg, CDialog)
	//{{AFX_MSG_MAP(CSnapshotDlg)
	//}}AFX_MSG_MAP
  ON_LBN_SELCHANGE(IDC_LBVIEWS, &CSnapshotDlg::OnLbnSelchangeLbviews)
  ON_BN_CLICKED(IDC_BUTTON1, &CSnapshotDlg::OnBnClickedButton1)
  ON_BN_CLICKED(IDC_CBVIEWEXTENTS, &CSnapshotDlg::OnBnClickedCbviewextents)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSnapshotDlg message handlers

BOOL CSnapshotDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

  if (m_pDevice)
  {
    RECT rect;
    m_ViewsList.GetClientRect(&rect);
#if 0
    m_ViewsList.InsertColumn(0, _T("Type")  , LVCFMT_LEFT, (rect.right - rect.left) / 4);
    m_ViewsList.InsertColumn(1, _T("Num")   , LVCFMT_LEFT, (rect.right - rect.left) / 4);
    m_ViewsList.InsertColumn(2, _T("VpId")  , LVCFMT_LEFT, (rect.right - rect.left) / 4);
    m_ViewsList.InsertColumn(3, _T("Handle"), LVCFMT_LEFT, (rect.right - rect.left) / 4);
    m_ViewsList.InsertItem(0, _T("Device"));
    int nViews = m_pDevice->numViews();
    for (int nView = 0; nView < nViews; nView++)
    {
      int nItem = nView + 1;
      m_ViewsList.InsertItem(nItem, _T("View"));
      { // Num
        CString numView;
        numView.Format(_T("%d"), nView);
        m_ViewsList.SetItemText(nItem, 1, numView);
      }
      { // VpId
        OdGsClientViewInfo cliInfo;
        m_pDevice->viewAt(nView)->clientViewInfo(cliInfo);
        CString idView;
        idView.Format(_T("%d"), cliInfo.viewportId);
        m_ViewsList.SetItemText(nItem, 2, idView);
      }
      { // Handle
        OdGsClientViewInfo cliInfo;
        m_pDevice->viewAt(nView)->clientViewInfo(cliInfo);
        OdDbObjectId id(cliInfo.viewportObjectId);
        if (id.isValid())
        {
          OdString hndlView = id.getHandle().ascii();
          m_ViewsList.SetItemText(nItem, 3, hndlView.c_str());
        }
      }
    }
#else
    int colWidth = (rect.right - rect.left) / 6;
    m_ViewsList.SetTabStops(colWidth);
    m_ViewsList.AddString(_T("Device"));
    int nViews = m_pDevice->numViews();
    for (int nView = 0; nView < nViews; nView++)
    {
      OdGsClientViewInfo cliInfo;
      m_pDevice->viewAt(nView)->clientViewInfo(cliInfo);
      OdDbObjectId id(cliInfo.viewportObjectId);
      OdString hndlView;
      if (id.isValid())
        hndlView = id.getHandle().ascii();
      else
        hndlView = _T("NULL");
      CString viewStr;
      viewStr.Format(_T("View\t%d\t%d\t%s"), nView, cliInfo.viewportId, hndlView.c_str());
      m_ViewsList.AddString(viewStr);
    }
#endif
    m_ViewsList.SetCurSel(0);
    OnLbnSelchangeLbviews();
  }
   
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSnapshotDlg::OnLbnSelchangeLbviews()
{
  CString selStr;
  m_ViewsList.GetText(m_ViewsList.GetCurSel(), selStr);
  RECT cliRect;
  ::GetClientRect(m_pParentWnd, &cliRect);
  OdGsDCRect setRect(cliRect.left, cliRect.right, cliRect.bottom, cliRect.top);
  OdGiRasterImagePtr pImg;
  try
  {
    int iTk = 0;
    if (selStr.Tokenize(_T("\t"), iTk).Compare(_T("View")) != 0)
    {
      m_pDevice->getSnapShot(pImg, setRect);
    }
    else
    {
      CString nView = selStr.Tokenize(_T("\t"), iTk);
      OdGsView *pView = m_pDevice->viewAt(odStrToInt(nView.GetBuffer()));
      if (IsDlgButtonChecked(IDC_CBVIEWEXTENTS) == BST_CHECKED)
      {
        OdGePoint2d lowerLeft, upperRight;
        OdGsDCRect vptRect;
        pView->getViewport(lowerLeft, upperRight);
        pView->getViewport(vptRect);
        vptRect.m_min.y = cliRect.bottom - vptRect.m_min.y;
        vptRect.m_max.y = cliRect.bottom - vptRect.m_max.y;
        setRect.m_min.x = (long)(lowerLeft.x * cliRect.right); setRect.m_min.y = (long)(lowerLeft.y * cliRect.bottom);
        setRect.m_max.x = (long)(upperRight.x * cliRect.right); setRect.m_max.y = (long)(upperRight.y * cliRect.bottom);
        setRect &= vptRect;
        if (setRect.is_null()) // Since we haven't area for aquire
          throw OdError(eNotImplementedYet);
        std::swap(setRect.m_min.y, setRect.m_max.y);
      }
      pView->getSnapShot(pImg, setRect);
    }
  }
  catch (const OdError &err)
  {
    if (err.code() != eNotImplementedYet)
      throw;
  }
  m_Preview.SetImage(pImg);
  if (!pImg.isNull())
  {
    SetDlgItemInt(IDC_SWIDTH, (UINT)pImg->pixelWidth(), FALSE);
    SetDlgItemInt(IDC_SHEIGHT, (UINT)pImg->pixelHeight(), FALSE);
    SetDlgItemInt(IDC_SBPP, (UINT)pImg->colorDepth(), FALSE);
    GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
  }
  else
  {
    SetDlgItemText(IDC_SWIDTH, _T("N/A"));
    SetDlgItemText(IDC_SHEIGHT, _T("N/A"));
    SetDlgItemText(IDC_SBPP, _T("N/A"));
    GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
  }
}

void CSnapshotDlg::OnBnClickedButton1()
{
  OdRxRasterServicesPtr pRasSvcs = odrxDynamicLinker()->loadApp(RX_RASTER_SERVICES_APPNAME);
  if (!pRasSvcs.isNull())
  {
    CFileDialog saveFile(false, _T("bmp"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("Windows Bitmap (*.bmp)|*.bmp|All Files (*.*)|*.*||"), this);
    if (saveFile.DoModal() == IDOK)
    {
      pRasSvcs->saveRasterImage(m_Preview.GetImage(), (LPCTSTR)saveFile.GetPathName(), OdRxRasterServices::kBMP);
    }
  }
}

void CSnapshotDlg::OnBnClickedCbviewextents()
{
  OnLbnSelchangeLbviews();
}

//
