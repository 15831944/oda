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

// Export2PDFPreview.cpp : implementation file
//

#include "stdafx.h"
#include "odamfcapp.h"
#include "Export2PDFPreview.h"

#include "RxRasterServices.h"
#include "RxPdfToRasterServices.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


class Pdf2Bmp
{
  Pdf2Bmp();
protected:
  OdDbHostAppServices& m_pHostApp;
  int m_OutputWindowW;
  int m_OutputWindowH;
  OdString m_inputFile;
  OdUInt16 m_pageNumber;
  double m_outBmpW;
  double m_outBmpH;

  virtual int outputBitmapW() { return m_OutputWindowW; }
  virtual int outputBitmapH() { return m_OutputWindowH; }
public:
  virtual double outBmpW() { return m_outBmpW; }
  virtual double outBmpH() { return m_outBmpH; }

  Pdf2Bmp(OdDbHostAppServices& pHostApp, OdString inputFile, int outputWindowW, int outputWindowH, OdUInt16 pageNumber);

  virtual void run();

  virtual ~Pdf2Bmp() {}
};

Pdf2Bmp::Pdf2Bmp(OdDbHostAppServices& pHostApp, OdString inputFile, int outputWindowW, int outputWindowH, OdUInt16 pageNumber)
  : m_pHostApp(pHostApp)
  , m_OutputWindowW(outputWindowW)
  , m_OutputWindowH(outputWindowH)
  , m_inputFile(inputFile)
  , m_pageNumber(pageNumber)
{
}

void Pdf2Bmp::run()
{
  OdRxRasterServicesPtr pRasterSvcs = odrxDynamicLinker()->loadApp(RX_RASTER_SERVICES_APPNAME);
  OdString outFile = m_inputFile + L".bmp";
  OdUInt16 dpi = 72;
  OdGiRasterImagePtr pImg;
#if defined (PDFIUM_MODULE_ENABLED)
  OdRxPdfToRasterServicesPtr pSvcs = odrxDynamicLinker()->loadApp(RX_PDF_TO_RASTER_SERVICES_APPNAME);

  if (!pSvcs.isNull() && !pRasterSvcs.isNull())
  {
    OdPdf2ImageConverterPtr pConverter = pSvcs->createConverter();
    if (!pConverter.isNull())
    {
      if (pConverter->loadPdf(m_inputFile) == eOk)
      {
        pConverter->setActivePage(m_pageNumber);
        OdGsDCRect rect;
        pConverter->getPageSize(rect);

        double scale = m_OutputWindowH / (double)rect.m_max.y;

        OdPdf2ImageConversionParams params;
        params.bg_color = 0xFFFFFFFF;
        params.dpi = dpi * scale;
        params.flags = (int)(kDisableAAforText | kDisableAAforImage | kDisableAAforGeom);

        pImg = pConverter->convertPdf(&params);
        if (!pImg.isNull())
        {
          m_outBmpH = pImg->pixelHeight();
          m_outBmpW = pImg->pixelWidth();
          pRasterSvcs->saveRasterImage(pImg, outFile);
          return;
        }
      }
    }
  }
#endif //(PDFIUM_MODULE_ENABLED)
  ::MessageBox(NULL, _T("Unable to create preview"), _T("PDF error"), MB_OK);
}

/////////////////////////////////////////////////////////////////////////////
// CExport2PDFPreview dialog

CExport2PDFPreview::CExport2PDFPreview(CWnd* pParent /*=NULL*/)
	: CDialog(CExport2PDFPreview::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExport2PDFPreview)
  m_bSlowMode = false;
	//}}AFX_DATA_INIT
  m_nPaperH = 297;
  m_nPaperW = 210;
  m_pHost = 0;
  m_BoolSizing = false;
  m_hBitmap = NULL;
}

void CExport2PDFPreview::fillLayouts(OdArray<OdString> layouts, OdRxObjectPtrArray databases)
{
  OdRxObjectPtrArray tmpDbs;

  UpdateData(FALSE);
  m_Layouts.ResetContent();
  for (unsigned f = 0; f < layouts.size(); ++f)
  {
    OdString name = layouts[f];
    if (databases.size())
    {
      OdDbDatabasePtr pDb = (OdDbDatabasePtr)databases[f];
      OdString filename = pDb->getFilename();
      int index = filename.reverseFind('\\');
      filename = filename.right(filename.getLength() - index - 1);
      name.insert(name.getLength(), L" - ");
      name.insert(name.getLength(), filename);
    }
    m_Layouts.AddString(name);
  }
  m_Layouts.SetCurSel(0);
  
  if (m_hBitmap != NULL)
  {
    DeleteObject(m_hBitmap);
    m_hBitmap = NULL;
  }
}

void CExport2PDFPreview::Init(OdString pdfFilePath, OdDbHostAppServices *pHost)
{
  if (pdfFilePath != m_pdfFilePath && !m_pdfFilePath.isEmpty())
  {
    _wremove(m_pdfFilePath);
  }
  m_pdfFilePath = pdfFilePath;
  m_pHost = pHost;
  if (m_hBitmap != NULL)
  {
    DeleteObject(m_hBitmap);
    m_hBitmap = NULL;
  }
}

void CExport2PDFPreview::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CExport2PDFPreview)
  DDX_Control(pDX, IDC_COMBO1, m_Layouts);
  DDX_Control(pDX, IDC_STATIC_PREVIEW, m_PdfPreview);
  DDX_Check(pDX, IDC_SLOWMODE, m_bSlowMode);
  //}}AFX_DATA_MAP
}

HBITMAP ResizeBmp(HWND hWnd,HBITMAP hBmp,int cx,int cy)
{
  HDC dc =GetDC(hWnd);
  HDC copyDC = CreateCompatibleDC(dc);
  HDC srcDC = CreateCompatibleDC(dc);
  int oriCx,oriCy;

  //Get Original Size
  BITMAP bm;
  GetObject(hBmp, sizeof(bm),&bm);
  oriCx = bm.bmWidth;
  oriCy = bm.bmHeight;

  RECT bmpRect;
  bmpRect.top=0;
  bmpRect.left=0;
  bmpRect.right=cx;
  bmpRect.bottom=cy;

  RECT oriRect = bmpRect;
  oriRect.right=oriCx;
  oriRect.bottom=oriCy;

  HRGN rgn1 = CreateRectRgnIndirect( &bmpRect );
  HRGN rgn2 = CreateRectRgnIndirect( &oriRect );
  SelectClipRgn(copyDC,rgn1);
  SelectClipRgn(srcDC,rgn2);
  DeleteObject(rgn1);
  DeleteObject(rgn2);

  HBITMAP copyBMP = CreateCompatibleBitmap(dc,cx,cy);
  HBITMAP copyBMPold = (HBITMAP)SelectObject(copyDC,copyBMP);
  HBITMAP srcBMPold = (HBITMAP)SelectObject(srcDC,hBmp);

  if(oriCx!=cx || oriCy!=cy)
    StretchBlt(copyDC,0,0,cx,cy,srcDC,0,0,oriCx,oriCy,SRCCOPY);
  else
    BitBlt(copyDC,0,0,cx,cy,srcDC,0,0,SRCCOPY);

  copyBMP = (HBITMAP)SelectObject(copyDC, copyBMPold);

  DeleteDC(copyDC);
  DeleteDC(srcDC);

  return copyBMP;
}

void CExport2PDFPreview::OnPaint()
{
#ifdef ON_WM_EXITSIZEMOVE
  if (!m_BoolSizing)
#endif
  {
    UpdateData(TRUE);
    if (!m_bSlowMode && m_hBitmap == NULL)
    {
      RECT rc;
      GetDesktopWindow()->GetWindowRect(&rc);
      m_hBitmap = CreateBitmapFromPdf(rc);
    }

    CDialog::OnPaint();
    RECT lpRect;
    m_PdfPreview.GetParent()->GetClientRect(&lpRect);

    RECT lpRectChild;
    m_PdfPreview.GetClientRect(&lpRectChild);
    m_PdfPreview.MapWindowPoints(m_PdfPreview.GetParent() ,&lpRectChild);

    double forPreviewW = lpRect.right - lpRect.left;
    double forPreviewH = lpRect.bottom - lpRectChild.top;

    // calculate preview bitmap size

    double bitmapW = forPreviewW;
    double bitmapH = m_nPaperH * forPreviewW/m_nPaperW;

    if (bitmapH > forPreviewH)
    {
      bitmapH = forPreviewH;
      bitmapW = m_nPaperW * forPreviewH/m_nPaperH;
    }

    lpRectChild.bottom = lpRectChild.top + bitmapH;
    lpRectChild.left = lpRect.left + ((lpRect.right - lpRect.left) - bitmapW) / 2.;
    lpRectChild.right = lpRectChild.left + bitmapW;

    m_PdfPreview.SetWindowPos(0, lpRectChild.left, lpRectChild.top, bitmapW, bitmapH, 0);
    
    //UpdateData();
    HBITMAP newBitmap;
    if (!m_bSlowMode)
    {
      newBitmap = ResizeBmp(GetSafeHwnd(), m_hBitmap, bitmapW, bitmapH);
    }
    else
    {
      newBitmap = CreateBitmapFromPdf(lpRectChild);;
    }
    DeleteObject(m_PdfPreview.SetBitmap(newBitmap));
    m_PdfPreview.Invalidate();
  }
}

CExport2PDFPreview::~CExport2PDFPreview()
{
  if (m_hBitmap != NULL)
  {
    DeleteObject(m_hBitmap);
  }
  if (!m_pdfFilePath.isEmpty())
  {
    _wremove(m_pdfFilePath);
  }
}

void CExport2PDFPreview::OnSize(UINT nType, int cx, int cy)
{
   if (nType == SIZE_RESTORED)
   {
     Invalidate();
   }
}

BEGIN_MESSAGE_MAP(CExport2PDFPreview, CDialog)
	//{{AFX_MSG_MAP(CExport2PDFPreview)
  ON_WM_SIZE()
  ON_WM_SIZING()
#ifdef ON_WM_EXITSIZEMOVE
  ON_WM_EXITSIZEMOVE()
  ON_WM_ENTERSIZEMOVE()
#endif
  ON_WM_PAINT()
  ON_CBN_SELCHANGE(IDC_COMBO1, OnSelChangeLayout)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CExport2PDFPreview::OnSelChangeLayout()
{
  UpdateData();

  int idx = m_Layouts.GetCurSel();
  if (m_hBitmap != NULL)
  {
    DeleteObject(m_hBitmap);
    m_hBitmap = NULL;
  }

  Invalidate();
}

void CExport2PDFPreview::OnEnterSizeMove()
{
  m_BoolSizing = true;
}

void CExport2PDFPreview::OnExitSizeMove()
{
  m_BoolSizing = false;
  Invalidate();
}

BOOL CExport2PDFPreview::OnInitDialog()
{
  ModifyStyle(0, WS_THICKFRAME, 0);
  GetDlgItem(IDC_SLOWMODE)->ShowWindow(SW_HIDE);
  return TRUE;
}

HBITMAP CExport2PDFPreview::CreateBitmapFromPdf(RECT &maximumAvailableRect)
{
  int OutputWindowW = maximumAvailableRect.right - maximumAvailableRect.left; 
  int OutputWindowH = maximumAvailableRect.bottom - maximumAvailableRect.top;

  Pdf2Bmp tmp(*m_pHost, m_pdfFilePath, OutputWindowW, OutputWindowH, m_Layouts.GetCurSel() + 1);
  tmp.run();

  m_nPaperW = tmp.outBmpW();
  m_nPaperH = tmp.outBmpH();

  CString bmpFile = CString(m_pdfFilePath.c_str()) + ".bmp";

  HBITMAP hBitmap = (HBITMAP) ::LoadImage(
    AfxGetInstanceHandle(),
    bmpFile,
    IMAGE_BITMAP, 0, 0, 
    LR_LOADFROMFILE | LR_CREATEDIBSECTION);

  _wremove(bmpFile);

  return hBitmap;
}
