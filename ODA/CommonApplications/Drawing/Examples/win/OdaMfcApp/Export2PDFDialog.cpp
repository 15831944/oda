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

// Export2PDFDialog.cpp : implementation file
//

#include "stdafx.h"
#include "odamfcapp.h"
#include "Export2PDFDialog.h"

#include "PdfExport.h"
using namespace TD_PDF_2D_EXPORT;

#include "MainFrm.h"
#include "DbDictionary.h"
#include "DbBlockTableRecord.h"
#include "PdfResultEx.h"
#include "PrcDefaultContextForTD.h"

#include "CustomPDFPropertiesDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define PDF_DEVICE_NAME L"DWG To PDF.pc3"
#define CUSTOM_DEVICE_NAME L"Custom Device"

class StatusPdfExportReactor : public TD_PDF_2D_EXPORT::PdfExportReactor
{
public:
  StatusPdfExportReactor(COdaMfcAppApp* theApp) 
    : m_App(theApp)
    , m_viewportVectoriztionCounter(0)
  {};
  virtual void beginViewVectorization(const OdGsClientViewInfo&)
  {
    m_viewportVectoriztionCounter++;

    OdString txt = OdString().format(OD_T("Start export viewport %u"), m_viewportVectoriztionCounter);

    if (m_App)
      m_App->setStatusText((LPCTSTR)txt);
  };
  virtual void endViewVectorization()
  {
    if (m_App)
      m_App->setStatusText(_T("End export viewport."));
  };
private:
  COdaMfcAppApp* m_App;
  int m_viewportVectoriztionCounter;
};

/////////////////////////////////////////////////////////////////////////////
// CExport2PDFDialog dialog


CExport2PDFDialog::CExport2PDFDialog(OdDbDatabaseCollection* pDatabases, CWnd* pParent /*=NULL*/)
	: CDialog(CExport2PDFDialog::IDD, pParent)
{
  //{{AFX_DATA_INIT(CExport2PDFDialog)
  m_FileName = _T("");
  m_Autor = _T("");
  m_Keywords = _T("");
  m_Creator = _T("");
  m_Title = _T("");
  m_bEmbedded = FALSE;
  m_bEmbeddedOptimized = FALSE;
  m_bSimpleGeomOpt = FALSE;
  m_bSHXAsGeometry = TRUE;
  m_bTTFAsGeometry = TRUE;
  m_bZoomToExtents = TRUE;
  m_bEncoded = TRUE;
  m_bASCIIHEXEncoded = FALSE;
  m_bEnableLayers = FALSE;
  m_bExportOffLayers = FALSE;
  m_Producer = _T("");
  m_Subject = _T("");
  m_dPaperH = 0.0;
  m_dPaperW = 0.0;
  m_iHatchDPI = 150;
  m_Layouts = 0;
  m_Databases = 0;
  m_bUseHLR = TRUE;
  m_bUsePRC = FALSE;
  m_bUsePRCAsBRep = FALSE;
  m_bUsePRCCompressTess = FALSE;
  m_bUsePRCSingleViewMode = FALSE;
  m_iGeomDPI = 600;
  m_iColorImagesDPI = 400;
  m_iBWImagesDPI = 400;
  m_bExportHyperlinks = FALSE;
  m_bTextSearchable = FALSE;
  m_bCropImages = TRUE;
  m_bDCTCompression = TRUE;
  m_bMeasuring = FALSE;
  m_iDCTQuality = 50;
  m_bLinearized = FALSE;
  m_bIgnoreInvisibleViewportBorders = FALSE;
  m_bMergeLinesCrossing = FALSE;
  m_iPdfAMode = 0;
  m_iShadedVpMode = 0;
  m_bForceDisableGsDevice = FALSE;
  m_bTranspBgForShadedViewports = FALSE;
  m_bUpscaleImages = FALSE;
  m_bUseGsCache = FALSE;
  m_bUsePdfBlocks = FALSE;
  m_pDbCollection = pDatabases;
  m_bSearchTextOnShadedVp = FALSE;
  m_bTextSearchableWith_CTRL_F = FALSE;
  //}}AFX_DATA_INIT
}


void CExport2PDFDialog::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CExport2PDFDialog)
  DDX_Text(pDX, IDC_EDIT_FILE, m_FileName);
  DDX_Text(pDX, IDC_AUTHOR, m_Autor);
  DDV_MaxChars(pDX, m_Autor, 255);
  DDX_Text(pDX, IDC_KEYWORDS, m_Keywords);
  DDV_MaxChars(pDX, m_Keywords, 255);
  DDX_Text(pDX, IDC_CREATOR, m_Creator);
  DDV_MaxChars(pDX, m_Creator, 255);
  DDX_Text(pDX, IDC_TITLE, m_Title);
  DDV_MaxChars(pDX, m_Title, 255);
  DDX_Check(pDX, IDC_EMEDDED, m_bEmbedded);
  DDX_Check(pDX, IDC_EMEDDED_OPTIMIZED, m_bEmbeddedOptimized);
  DDX_Check(pDX, IDC_GEOM_OPT, m_bSimpleGeomOpt);
  DDX_Check(pDX, IDC_SHX_AS_GEOM, m_bSHXAsGeometry);
  DDX_Check(pDX, IDC_TTF_AS_GEOM, m_bTTFAsGeometry);
  DDX_Check(pDX, IDC_ZOOM, m_bZoomToExtents);

  DDX_Check(pDX, IDC_LAYERS, m_bEnableLayers);
  DDX_Check(pDX, IDC_OFFLAYERS, m_bExportOffLayers);

  DDX_Check(pDX, IDC_PRC, m_bUsePRC);
  DDX_Check(pDX, IDC_PRC2, m_bUsePRCAsBRep);
  DDX_Check(pDX, IDC_PRC_TESS_COMPRESSION, m_bUsePRCCompressTess);
  DDX_Check(pDX, IDC_PRC3, m_bUsePRCSingleViewMode);
  DDX_Check(pDX, IDC_ENCODED, m_bEncoded);
  DDX_Check(pDX, IDC_ENCODED2, m_bASCIIHEXEncoded);

  DDX_Text(pDX, IDC_PRODUCER, m_Producer);
  DDV_MaxChars(pDX, m_Producer, 255);
  DDX_Text(pDX, IDC_SUBJECT, m_Subject);
  DDV_MaxChars(pDX, m_Subject, 255);
  DDX_Text(pDX, IDC_EDIT_PAPER_HEIGHT, m_dPaperH);
  DDX_Text(pDX, IDC_EDIT_PAPER_WIDTH, m_dPaperW);
  DDX_Radio(pDX, IDC_RADIO_ACTIVE, m_Layouts);
  DDX_Radio(pDX, IDC_RADIO_DB_ACTIVE, m_Databases);

  DDX_Check(pDX, IDC_HLR, m_bUseHLR);

  DDX_Control(pDX, IDC_PDFEXPORT_HATCHEXPTYPE, m_comboHatchExportType);
  DDX_Control(pDX, IDC_PDFEXPORT_GRADIENTHATCHEXPTYPE, m_comboGradientHatchExportType);
  DDX_Control(pDX, IDC_PDFEXPORT_OTHERHATCHEXPTYPE, m_comboOtherHatchExportType);
  DDX_Control(pDX, IDC_PDFEXPORT_COLOR_POLICY, m_colorPolicy);
  DDX_Control(pDX, IDC_PRC_COMPRESSION, m_PRCCompression);

  DDX_Check(pDX, IDC_HYPERLINK, m_bExportHyperlinks);
  DDX_Check(pDX, IDC_TEXT_SEARCHABLE, m_bTextSearchable);
  DDX_Check(pDX, IDC_TEXT_SEARCHABLE_CTRL_F, m_bTextSearchableWith_CTRL_F);
  DDX_Check(pDX, IDC_LINEARIZED, m_bLinearized);
  DDX_Check(pDX, IDC_VIEWPORT_BORDERS, m_bIgnoreInvisibleViewportBorders);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CExport2PDFDialog, CDialog)
	//{{AFX_MSG_MAP(CExport2PDFDialog)
	ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
  ON_BN_CLICKED(IDOK, OnExport)
  ON_BN_CLICKED(IDPREVIEW, OnPreview)
  ON_BN_CLICKED(IDC_LAYERS, OnCheckLayers)
  ON_BN_CLICKED(IDC_PRC, OnCheckPRC)
  ON_BN_CLICKED(IDC_HLR, OnCheckHLR)
  ON_BN_CLICKED(IDC_EMEDDED, OnCheckEmbedded)
  ON_BN_CLICKED(IDC_ZOOM, OnCheckZoom)
  
  ON_BN_CLICKED(IDC_RADIO_ACTIVE, OnParametersUpdated)
  ON_BN_CLICKED(IDC_EX_MONO_CHECK, OnParametersUpdated)
  ON_BN_CLICKED(IDC_RADIO_ALL, OnParametersUpdated)
  ON_BN_CLICKED(IDC_EMEDDED_OPTIMIZED, OnParametersUpdated)
  ON_BN_CLICKED(IDC_TTF_AS_GEOM, OnCheckTextAsGeometry)
  ON_BN_CLICKED(IDC_SHX_AS_GEOM, OnCheckTextAsGeometry)
  ON_BN_CLICKED(IDC_TEXT_SEARCHABLE, OnCheckTextSearchable)
  ON_BN_CLICKED(IDC_GEOM_OPT, OnParametersUpdated)
  ON_BN_CLICKED(IDC_HLR, OnParametersUpdated)
  ON_BN_CLICKED(IDC_HYPERLINK, OnParametersUpdated)
  ON_BN_CLICKED(IDC_TEXT_SEARCHABLE, OnParametersUpdated)
  ON_BN_CLICKED(IDC_RADIO_DB_ACTIVE, OnParametersUpdated)
  ON_BN_CLICKED(IDC_RADIO_DB_ALL, OnParametersUpdated)

  ON_EN_CHANGE(IDC_EDIT_PAPER_WIDTH, OnParametersUpdated)
  ON_EN_CHANGE(IDC_EDIT_PAPER_HEIGHT, OnParametersUpdated)
  ON_BN_CLICKED(IDC_BUTTON_CUSTOM_PROPERTIES, OnClickCustomButton)

	//}}AFX_MSG_MAP
  ON_BN_CLICKED(IDC_PRC2, OnBnClickedPrcAsBrep)
END_MESSAGE_MAP()

BOOL CExport2PDFDialog::OnInitDialog()
{
  if (!CDialog::OnInitDialog())
    return FALSE;

  m_comboHatchExportType.ResetContent();

  m_comboHatchExportType.InsertString(0, _T("bitmap export"));
  m_comboHatchExportType.InsertString(1, _T("vectorizer"));
  m_comboHatchExportType.InsertString(2, _T("PDF paths"));
  m_comboHatchExportType.InsertString(3, _T("polygons"));
  m_comboHatchExportType.SetCurSel(2);

  m_comboOtherHatchExportType.ResetContent();

  m_comboOtherHatchExportType.InsertString(0, _T("bitmap export"));
  m_comboOtherHatchExportType.InsertString(1, _T("vectorizer"));
  m_comboOtherHatchExportType.SetCurSel(1);

  m_comboGradientHatchExportType.ResetContent();

  m_comboGradientHatchExportType.InsertString(0, _T("bitmap export"));
  m_comboGradientHatchExportType.InsertString(1, _T("vectorizer"));
  m_comboGradientHatchExportType.SetCurSel(0);

  m_colorPolicy.ResetContent();

  m_colorPolicy.InsertString(0, _T("As Is"));
  m_colorPolicy.InsertString(1, _T("Monochrome"));
  m_colorPolicy.InsertString(2, _T("Grayscale"));
  m_colorPolicy.InsertString(3, _T("Use mono plotstyle"));
  m_colorPolicy.SetCurSel(0);

  m_PRCCompression.InsertString(0, _T("No Compression"));
  m_PRCCompression.InsertString(1, _T("Loose Compression"));
  m_PRCCompression.InsertString(2, _T("Medium Compression"));
  m_PRCCompression.InsertString(3, _T("High Compression"));
  m_PRCCompression.SetCurSel(0);

  GetDlgItem(IDC_PRC)->EnableWindow(m_bUsePRC);
  GetDlgItem(IDC_PRC2)->EnableWindow( m_bUsePRC );
  GetDlgItem(IDC_PRC3)->EnableWindow( m_bUsePRC );
  GetDlgItem(IDC_STATIC_PRC_COMPRESSION)->EnableWindow(m_bUsePRC);
  GetDlgItem(IDC_PRC_COMPRESSION)->EnableWindow(m_bUsePRCAsBRep);
  GetDlgItem(IDC_PRC_TESS_COMPRESSION)->EnableWindow(m_bUsePRC);

  if (!m_pDbCollection || m_pDbCollection->count() == 1)
  {
    GetDlgItem(IDC_STATIC_EXPORT_DBS)->ShowWindow(SW_HIDE);
    GetDlgItem(IDC_RADIO_DB_ACTIVE)->ShowWindow(SW_HIDE);
    GetDlgItem(IDC_RADIO_DB_ALL)->ShowWindow(SW_HIDE);
  }

  GetDlgItem(IDC_TEXT_SEARCHABLE)->ShowWindow((m_bSHXAsGeometry || m_bTTFAsGeometry) ? SW_SHOWNORMAL : SW_HIDE);
  GetDlgItem(IDC_TEXT_SEARCHABLE_CTRL_F)->ShowWindow(m_bTextSearchable ? SW_SHOWNORMAL : SW_HIDE);

  UpdateData( FALSE );
  
  return true;
}

/////////////////////////////////////////////////////////////////////////////
// CExport2PDFDialog message handlers

void CExport2PDFDialog::OnBrowse() 
{
	// TODO: Add your control notification handler code here
  UpdateData(true); // save and validate

  CFileDialog dlgFile(false);

  dlgFile.m_ofn.Flags |= OFN_HIDEREADONLY | OFN_EXPLORER | OFN_PATHMUSTEXIST;

  CString strFilter;

  strFilter = "PDF files (*.pdf)";
  strFilter += (TCHAR)'\0';   // next string please
  strFilter += _T("*.pdf");
  strFilter += (TCHAR)'\0';   // last string
  dlgFile.m_ofn.lpstrFilter = strFilter;

  dlgFile.m_ofn.lpstrDefExt = _T("pdf");
  dlgFile.m_ofn.lpstrTitle = _T("Export To Pdf");

  if (dlgFile.DoModal() == IDOK)
  {
    m_FileName = dlgFile.GetPathName();
    if (m_FileName.Find('.') == -1)
    {
      m_FileName += ".pdf";
    }

    UpdateData(false); // save and validate
  }
}

void CExport2PDFDialog::fillListForDb(OdDbDatabasePtr pDb, OdArray<OdString> &layouts, OdRxObjectPtrArray& databases)
{
  OdDbDictionaryPtr pLayoutDict = pDb->getLayoutDictionaryId().safeOpenObject();
  OdDbDictionaryIteratorPtr pIt = pLayoutDict->newIterator();
  OdUInt32 startIndex = layouts.size();

  layouts.resize(startIndex + pLayoutDict->numEntries());
  if(m_Databases)
    databases.resize(startIndex + pLayoutDict->numEntries());

  while (!pIt->done())
  {
    OdDbLayoutPtr pLayout = pIt->getObject(OdDb::kForWrite);
    OdString sName = pLayout->getLayoutName();
    int tabOrder = pLayout->getTabOrder();

    OdDbBlockTableRecordPtr pRec = pLayout->getBlockTableRecordId().safeOpenObject();
    OdDbObjectIteratorPtr pItRec = pRec->newIterator();
    if(!pItRec->done()) //non-empty layout
    {
      layouts.setAt(startIndex + tabOrder, sName);
      if(m_Databases)
        databases.setAt(startIndex + tabOrder, pDb);
    }
    pIt->next();
  }
}

void CExport2PDFDialog::fillLayoutList(OdArray<OdString> &layouts, OdRxObjectPtrArray& databases, OdArray<double> &divWH)
{
  layouts.clear();
  databases.clear();
  if (m_Layouts == 1)
  {
    if (m_Databases && m_pDbCollection && m_pDbCollection->count() > 1)
    {
      for (OdRxIteratorPtr it = oddbDatabaseCollection()->newIterator(); !it->done(); it->next())
      {
        fillListForDb(OdDbDatabasePtr(it->object()), layouts, databases);
      }
    }
    else
    {
      fillListForDb(m_pDb, layouts, databases);
    }
  }
  else
  {
    if (m_Databases && m_pDbCollection && m_pDbCollection->count() > 1)
    {
      for (OdRxIteratorPtr it = oddbDatabaseCollection()->newIterator(); !it->done(); it->next())
      {
        OdDbBlockTableRecordPtr pLayoutBlock = OdDbDatabasePtr(it->object())->getActiveLayoutBTRId().safeOpenObject();
        OdDbLayoutPtr pLayout = pLayoutBlock->getLayoutId().safeOpenObject(OdDb::kForWrite);
        layouts.push_back(pLayout->getLayoutName());
        databases.push_back(it->object());
      }
    } 
    else
    {
      OdDbBlockTableRecordPtr pLayoutBlock = m_pDb->getActiveLayoutBTRId().safeOpenObject();
      OdDbLayoutPtr pLayout = pLayoutBlock->getLayoutId().safeOpenObject(OdDb::kForWrite);
      layouts.push_back(pLayout->getLayoutName());
    }
  }
}

OdUInt32 CExport2PDFDialog::ExportToPdf(OdString pdfFileName, bool bPreview)
{
  OdPdfExportModulePtr pModule;
  try
  {
    pModule = ::odrxDynamicLinker()->loadApp(OdPdfExportModuleName, false);
  }
  catch (const OdError& e)
  {
    theApp.reportError(m_hWnd, _T("Error"), e);
    return exOk;
  }
  {
    OdPdfExportPtr exporter = pModule->create();
    {
      PDFExportParams params;

      if (!m_bEnableLayers)
        m_bExportOffLayers = false;

      bool bV15 = m_bEnableLayers || m_bExportOffLayers;
      params.setDatabase(m_pDb);
      params.setVersion(bV15 ? PDFExportParams::kPDFv1_5 : PDFExportParams::kPDFv1_4);

      params.setOutput(odSystemServices()->createFile(pdfFileName, Oda::kFileWrite, Oda::kShareDenyNo, Oda::kCreateAlways));

      params.setExportFlags(PDFExportParams::PDFExportFlags(
                            (m_bEmbeddedOptimized ? PDFExportParams::kEmbededOptimizedTTF : 0) |
                            (m_bEmbedded ? PDFExportParams::kEmbededTTF : 0) |
                            (m_bSHXAsGeometry ? PDFExportParams::kSHXTextAsGeometry : 0) |
                            (m_bTTFAsGeometry ? PDFExportParams::kTTFTextAsGeometry : 0) |
                            (m_bSimpleGeomOpt ? PDFExportParams::kSimpleGeomOptimization : 0) |
                            (m_bEnableLayers ? PDFExportParams::kEnableLayers : 0) |
                            ((!bPreview && m_bExportOffLayers) ? PDFExportParams::kIncludeOffLayers : 0) |
                            (m_bUseHLR ? PDFExportParams::kUseHLR : 0) |
                            ((!bPreview && m_bEncoded) ? PDFExportParams::kFlateCompression : 0) |
                            ((!bPreview && m_bASCIIHEXEncoded) ? PDFExportParams::kASCIIHexEncoding : 0) |
                            ((!bPreview && m_bExportHyperlinks) ? PDFExportParams::kExportHyperlinks : 0) |
                            ((!bPreview && m_bLinearized) ? PDFExportParams::kLinearized : 0) |
                            (m_bMergeLinesCrossing ? PDFExportParams::kMergeLines : 0) |
                            ((!bPreview && m_bMeasuring) ? PDFExportParams::kMeasuring : 0) |
                            (m_bZoomToExtents ? PDFExportParams::kZoomToExtentsMode : 0)));

      params.setTitle(OdString(m_Title));
      params.setAuthor(OdString(m_Autor));
      params.setSubject(OdString(m_Subject));
      params.setKeywords(OdString(m_Keywords));
      params.setCreator(OdString(m_Creator));
      params.setProducer(OdString(m_Producer));

      if (m_Watermark.text.getLength())
        params.addWatermark(m_Watermark);

      //StatusPdfExportReactor reactor((COdaMfcAppApp*)m_pHost);
      //params.setExportReactor(&reactor);

      //params.setBackground(ODRGB(123, 200, 121));

      switch (m_iShadedVpMode)
      {
      case 0:
        params.setShadedVpExportMode(PDFExportParams::kExportOnePiece);
        break;
      case 1:
        params.setShadedVpExportMode(PDFExportParams::kExportAsTiles);
        break;
      case 2:
        params.setShadedVpExportMode(PDFExportParams::kExportAsStrips);
        break;
      default:
        break;
      }

      params.setUseGsCache(m_bUseGsCache == TRUE ? true : false);
      //params.setUsePdfBlocks(m_bUsePdfBlocks == TRUE ? true : false); //TODO: unhide it when pdf blocks are implemented
      params.setUseViewExtents(m_bIgnoreInvisibleViewportBorders == TRUE ? true : false);
      params.setImageCropping(m_bCropImages == TRUE ? true : false);
      params.setDCTCompression(m_bDCTCompression == TRUE ? true : false);
      params.setDCTQuality(!bPreview ? m_iDCTQuality : 10);
      params.setArchived(!bPreview ? (PDFExportParams::PDF_A_mode)m_iPdfAMode : (PDFExportParams::PDF_A_mode)0);
      params.setUpscaleImages((m_bUpscaleImages == TRUE && !bPreview) ? true : false);

      params.setForceDisableGsDevice(m_bForceDisableGsDevice == TRUE ? true : false);
      params.setTransparentShadedVpBg(m_bTranspBgForShadedViewports == TRUE ? true : false);

      params.setSearchableTextAsHiddenText((m_bTextSearchableWith_CTRL_F == TRUE && m_bTextSearchable == TRUE) ? true : false);
      params.setSearchableTextInRenderedViews(m_bSearchTextOnShadedVp == TRUE ? true : false);

      PDFExportParams::SearchableTextType STType = (!bPreview && m_bTextSearchable && m_bTTFAsGeometry) ? PDFExportParams::kTTF : PDFExportParams::kNoSearch;
      if (!bPreview && m_bTextSearchable && m_bSHXAsGeometry)
        STType = (PDFExportParams::SearchableTextType)(STType | PDFExportParams::kSHX);
      params.setSearchableTextType(STType);

      if (m_bUsePRC) {
        params.setPRCMode(m_bUsePRCAsBRep == TRUE ? PDFExportParams::kAsBrep : PDFExportParams::kAsMesh);
        try
        {
          OdRxModulePtr pModule = ::odrxDynamicLinker()->loadApp("OdPrcModule", false);
          if (!pModule.isNull())
          {
            pModule = ::odrxDynamicLinker()->loadApp("OdDwg2PrcExport", false);
          }
        }
        catch (const OdError& e)
        {
          theApp.reportError(m_hWnd, _T("Error"), e);
          return exOk;
        }

        PDF3D_ENUMS::PRCCompressionLevel compressLevel = PDF3D_ENUMS::kA3DLooseCompression;
        bool bCompress = true;
        switch (m_PRCCompression.GetCurSel())
        {
          case 0:
            bCompress = false;
          break;
          case 2:
            compressLevel = PDF3D_ENUMS::kA3DMeddiumCompression;
            break;
          case 3:
            compressLevel = PDF3D_ENUMS::kA3DHighCompression;
          break;
        }

        params.setPRCCompression(compressLevel, bCompress, (m_bUsePRCCompressTess != 0));

        if (m_bUsePRCSingleViewMode == TRUE)
        {
          params.setPRCContext(odCreatePrcAllInSingleViewContextForTD());
        }
        else
        {
          params.setPRCContext(odCreatePrcDefaultContextForTD());
        }
      }

      // setting solid hatch export type
      switch (m_comboHatchExportType.GetCurSel())
      {
        case 1: // as a drawing (vectorizer)
        {
          params.setSolidHatchesExportType(PDFExportParams::kDrawing);
        }
        break;
        case 2: // as a pdf path
        {
          params.setSolidHatchesExportType(PDFExportParams::kPdfPaths);
        }
        break;
        case 3: // as a polygon
        {
          params.setSolidHatchesExportType(PDFExportParams::kPolygons);
        }
        break;
        case 0: // as a bitmap
        default:
        {
          params.setSolidHatchesExportType(PDFExportParams::kBitmap);
        }
        break;
      }
      switch (m_comboGradientHatchExportType.GetCurSel())
      {
        case 1: // as a drawing (vectorizer)
        {
          params.setGradientHatchesExportType(PDFExportParams::kDrawing);
        }
        break;
        case 0: // as a bitmap
        default:
        {
          params.setGradientHatchesExportType(PDFExportParams::kBitmap);
        }
        break;
      }
      switch (m_comboOtherHatchExportType.GetCurSel())
      {
        case 1: // as a drawing (vectorizer)
        {
          params.setOtherHatchesExportType(PDFExportParams::kDrawing);
        }
        break;
        case 0: // as a bitmap
        default:
        {
          params.setOtherHatchesExportType(PDFExportParams::kBitmap);
        }
        break;
      }
      params.setGeomDPI(!bPreview ? m_iGeomDPI : 72);
      params.setBWImagesDPI(!bPreview ? m_iBWImagesDPI : 72);
      params.setColorImagesDPI(!bPreview ? m_iColorImagesDPI : 72);

      OdArray<double> divWH;
      fillLayoutList(params.layouts(), params.databases(), divWH);

      bool bMonoPalette = false;

      switch (m_colorPolicy.GetCurSel())
      {
        case 1:
          params.setColorPolicy(PDFExportParams::kMono);
          break;
        case 2:
          params.setColorPolicy(PDFExportParams::kGrayscale);
          break;
        case 3:
          bMonoPalette = true;
        case 0:
        default:
          break;
      }

      bool bMonochrome = false;
      OdDbPlotSettingsValidatorPtr pValidator;

      int i = m_pDb->getPSTYLEMODE();
      OdString strMono((i == 1) ? "monochrome.ctb" : "monochrome.stb");
      if(bMonoPalette)
      {
        pValidator = theApp.plotSettingsValidator();
        OdArray<const OdChar *> PSSlist;
        pValidator->plotStyleSheetList(PSSlist);

        unsigned int iSize = PSSlist.size();
        const OdChar* mono = strMono.c_str();
        for(unsigned int i = 0; i < iSize; ++i)
        {
          if (*mono == *PSSlist[i])
          {
            bMonochrome = true;
            break;
          }
        }
      }

      if (bMonochrome)
      {
        OdDbDictionaryPtr pLayoutDict = m_pDb->getLayoutDictionaryId().safeOpenObject();

        for(unsigned f=0; f<params.layouts().size(); ++f)
        {
          OdDbLayoutPtr pLayout = pLayoutDict->getAt(params.layouts()[f]).safeOpenObject(OdDb::kForWrite);
          pValidator->setCurrentStyleSheet((OdDbPlotSettings*)pLayout, strMono);
        }
      }

      if (m_Layouts != 1 && m_Databases != 1)
      {
        params.layouts().clear();
      }

      OdUInt32 nPages = odmax(1, params.layouts().size());

      OdGsPageParams pageParams;
      pageParams.set(m_dPaperW, m_dPaperH);
      params.pageParams().resize(nPages, pageParams);
      params.setHatchDPI(bPreview ? m_iHatchDPI : 72);

      {//Bug 20676 fix/workaround/you_name_it begin (see Comment 2 in that bug)
        OdArray<double> divWH;
        OdArray<OdString> layouts;
        OdRxObjectPtrArray databases;
        fillLayoutList(layouts, databases, divWH);
        OdDbDictionaryPtr pLayoutDict;
        if (!databases.size())
          pLayoutDict = m_pDb->getLayoutDictionaryId().safeOpenObject();

        for (unsigned f = 0; f < layouts.size(); ++f)
        {
          if (databases.size())
          {
            OdDbDatabasePtr pDb = (OdDbDatabasePtr)databases[f];
            pLayoutDict = pDb->getLayoutDictionaryId().safeOpenObject();
          }
          OdDbLayoutPtr pLayout = pLayoutDict->getAt(layouts[f]).safeOpenObject(OdDb::kForWrite);
          OdString printer = pLayout->getPlotCfgName();
          
          if (printer != OdString(PDF_DEVICE_NAME) && printer != OdString(CUSTOM_DEVICE_NAME))
          {
            double dW, dH;
            pLayout->getPlotPaperSize(dW, dH);
            OdDbPlotSettings::PlotPaperUnits units = pLayout->plotPaperUnits();
            if (pValidator.isNull())
              pValidator = theApp.plotSettingsValidator();
            if (!pValidator.isNull())
            {
              OdString mediaName = pLayout->getCanonicalMediaName();
              OdResult res = pValidator->setPlotCfgName((OdDbPlotSettings*)pLayout, PDF_DEVICE_NAME, mediaName);
            }
          }
        }
      }//20676 end
      
      OdUInt32 errCode = exporter->exportPdf(params);
      return errCode;
    }
  }
}

void CExport2PDFDialog::OnExport() 
{
	// TODO: Add your control notification handler code here
  UpdateData(true); // save and validate

  if (m_dPaperH > 5080 || m_dPaperW > 5080) //PDF limits
  {
    if (m_dPaperH > 5080)
      m_dPaperH = 5080;
    if (m_dPaperW > 5080)
      m_dPaperW = 5080;
    ::MessageBox(NULL, _T("Page size cannot exceed PDF limits"), _T("Size reset"), MB_OK);
    UpdateData(FALSE);
    return;
  }

  CMainFrame* pMFrame = (CMainFrame*)theApp.GetMainWnd();
  pMFrame->StartTimer();

  try
  {
    odSystemServices()->createFile(OdString(m_FileName), Oda::kFileWrite, Oda::kShareDenyNo, Oda::kCreateAlways);
  }
  catch (OdError)
  {
    ::MessageBox(NULL, _T("Unable to create file"), _T("PDF error"), MB_OK);
    return;
  }

  bool bMonochrome = false;
  OdDbPlotSettingsValidatorPtr pValidator;

  int i = m_pDb->getPSTYLEMODE();
  OdString strMono((i == 1) ? "monochrome.ctb" : "monochrome.stb");

  bool bMonoPalette = false;

  switch (m_colorPolicy.GetCurSel())
  {
    case 3:
      bMonoPalette = true;
    default:
      break;
  }

  if(bMonoPalette)
  {
    pValidator = theApp.plotSettingsValidator();
    OdArray<const OdChar *> PSSlist;
    pValidator->plotStyleSheetList(PSSlist);

    unsigned int iSize = PSSlist.size();
    const OdChar* mono = strMono.c_str();
    for(unsigned int i = 0; i < iSize; ++i)
    {
      if (*mono == *PSSlist[i])
      {
        bMonochrome = true;
        break;
      }
    }
    if(!bMonochrome)
      ::MessageBox(NULL, _T("monochrome.ctb does not found"), _T("PDF error"), MB_OK);
  }

  OdUInt32 errCode = ExportToPdf(OdString(m_FileName), false);

  pMFrame->StopTimer(_T("PDF export"));

  if (errCode != 0)
  {
    OdPdfExportModulePtr pModule = ::odrxDynamicLinker()->loadApp(OdPdfExportModuleName);
    {
      OdPdfExportPtr exporter = pModule->create();

      OdString errMes = exporter->exportPdfErrorCode(errCode);
      CString str;
      str.Format(_T("Error number : 0x%x. \n %ls"), errCode, errMes.c_str());

      if (errCode == 0x10008)
      {
        str += _T("\nPlease enable Zoom to extents check box or\ndefine page parameters for layout in page setup dialog.");
      }

      ::MessageBox(NULL, str, _T("PDF error"), MB_OK|MB_ICONERROR);
    }
  }
  
  CExport2PDFDialog::OnOK();
}

void CExport2PDFDialog::OnPreview() 
{
  // TODO: Add your control notification handler code here
  UpdateData(true); // save and validate
  
  OdArray<OdString> layouts;
  OdRxObjectPtrArray databases;
  OdArray<double> divWH;
  fillLayoutList(layouts, databases, divWH);

  OdString m_tmpPath = m_pDb->appServices()->getTempPath();
  TCHAR tempname[MAX_PATH];
  ::GetTempFileName(m_tmpPath.c_str(),_T(""),0,tempname);
  m_tmpPath = tempname;
  m_tmpPath.makeLower();
  m_tmpPath.replace(L".tmp",L".pdf");

  ExportToPdf(m_tmpPath, true);
  
  dlg.Init(m_tmpPath, m_pHost);
  if (dlg.m_hWnd == 0)
  {
    dlg.Create(CExport2PDFPreview::IDD, NULL);
  }
  dlg.fillLayouts(layouts, databases);
  dlg.ShowWindow(SW_SHOW);
}

void CExport2PDFDialog::OnCheckLayers()
{
  UpdateData( );

  GetDlgItem(IDC_OFFLAYERS)->EnableWindow( m_bEnableLayers );
}

void CExport2PDFDialog::OnCheckPRC()
{
  UpdateData();
  if (m_bUsePRC)
  {
    m_bUseHLR = FALSE;
    UpdateData(false); // save and validate
  }

  GetDlgItem(IDC_PRC2)->EnableWindow( m_bUsePRC );
  GetDlgItem(IDC_PRC3)->EnableWindow( m_bUsePRC );
  GetDlgItem(IDC_HLR)->EnableWindow( !m_bUsePRC );
  GetDlgItem(IDC_STATIC_PRC_COMPRESSION)->EnableWindow(m_bUsePRC);
  GetDlgItem(IDC_PRC_TESS_COMPRESSION)->EnableWindow(m_bUsePRC);
  GetDlgItem(IDC_PRC_COMPRESSION)->EnableWindow(m_bUsePRCAsBRep);

}

void CExport2PDFDialog::OnCheckHLR()
{
  UpdateData();
  
  if (m_bUseHLR)
  {
    m_bUsePRC = FALSE;
    m_bUsePRCAsBRep = FALSE;
    m_bUsePRCCompressTess = FALSE;
    m_bUsePRCSingleViewMode = FALSE;
    UpdateData(false); // save and validate
    GetDlgItem(IDC_PRC3)->EnableWindow(m_bUsePRCSingleViewMode);
    GetDlgItem(IDC_PRC2)->EnableWindow(m_bUsePRCAsBRep);
    GetDlgItem(IDC_PRC_TESS_COMPRESSION)->EnableWindow(m_bUsePRCCompressTess);
    GetDlgItem(IDC_PRC)->EnableWindow(m_bUsePRC);
  }
  else
  {
    GetDlgItem(IDC_PRC)->EnableWindow(TRUE);
  }
}

void CExport2PDFDialog::OnCheckTextAsGeometry()
{
  UpdateData();
  GetDlgItem(IDC_TEXT_SEARCHABLE)->ShowWindow((m_bSHXAsGeometry || m_bTTFAsGeometry) ? SW_SHOWNORMAL : SW_HIDE);

  OnParametersUpdated();
}

void CExport2PDFDialog::OnCheckTextSearchable()
{
  UpdateData();
  GetDlgItem(IDC_TEXT_SEARCHABLE_CTRL_F)->ShowWindow(m_bTextSearchable ? SW_SHOWNORMAL : SW_HIDE);

  OnParametersUpdated();
}

void CExport2PDFDialog::OnCheckEmbedded()
{
  UpdateData( );

  GetDlgItem(IDC_EMEDDED_OPTIMIZED)->EnableWindow( m_bEmbedded );
  m_bEmbeddedOptimized = false;
  UpdateData(false); // save and validate

  OnParametersUpdated();
}

void CExport2PDFDialog::OnParametersUpdated()
{
  if (dlg.m_hWnd != 0 && dlg.IsWindowVisible() == TRUE)
  {
    OnPreview();
    dlg.Invalidate();
  }
}

void CExport2PDFDialog::OnCheckZoom()
{
  UpdateData( );

  GetDlgItem(IDC_EDIT_PAPER_HEIGHT)->EnableWindow( m_bZoomToExtents );
  GetDlgItem(IDC_EDIT_PAPER_WIDTH)->EnableWindow( m_bZoomToExtents );

  OnParametersUpdated();
}

void CExport2PDFDialog::OnClickCustomButton()
{
  AuxPdfProps props;
  props.piHatchDPI = &m_iHatchDPI;
  props.piGeomDPI = &m_iGeomDPI;
  props.piColorImagesDPI = &m_iColorImagesDPI;
  props.piBWImagesDPI = &m_iBWImagesDPI;
  props.piDCTQuality = &m_iDCTQuality;
  props.piPdfAMode = &m_iPdfAMode;
  props.piShadedVpMode = &m_iShadedVpMode;
  props.pbMergeLines = &m_bMergeLinesCrossing;
  props.pbCropImages = &m_bCropImages;
  props.pbDCTCompression = &m_bDCTCompression;
  props.pbMeasuring = &m_bMeasuring;
  props.pbForceDisableGsDevice = &m_bForceDisableGsDevice;
  props.pbTranspBgForShadedViewports = &m_bTranspBgForShadedViewports;
  props.pbSearchTextOnShadedVp = &m_bSearchTextOnShadedVp;
  props.pbUpscaleImages = &m_bUpscaleImages;
  props.pbUseGsCache = &m_bUseGsCache;
  props.pbUsePdfBlocks = &m_bUsePdfBlocks;
  props.watermark = &m_Watermark;
  CCustomPDFPropertiesDialog dlgCustom(props, this);
  dlgCustom.DoModal();
  if (m_iPdfAMode == (unsigned int)PDFExportParams::kPDFA_1b)
  {
    GetDlgItem(IDC_LAYERS)->EnableWindow(false);
    GetDlgItem(IDC_OFFLAYERS)->EnableWindow(false);
  }
  else
  {
    GetDlgItem(IDC_LAYERS)->EnableWindow(true);
    GetDlgItem(IDC_OFFLAYERS)->EnableWindow(m_bEnableLayers);
  }
  if(!m_bUseGsCache)
    m_bUsePdfBlocks = FALSE;
}

void CExport2PDFDialog::OnBnClickedPrcAsBrep()
{
  UpdateData();
  if (!m_bUsePRCAsBRep)
  {
    m_PRCCompression.SetCurSel(0);
  }
  GetDlgItem(IDC_PRC_COMPRESSION)->EnableWindow(m_bUsePRCAsBRep);
}
