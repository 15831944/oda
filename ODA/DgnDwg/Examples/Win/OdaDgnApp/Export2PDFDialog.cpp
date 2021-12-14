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
#include "OdaDgnApp.h"
#include "Export2PDFDialog.h"
//#include ".\export2pdfdialog.h"
#include "PdfExport.h"
using namespace TD_PDF_2D_EXPORT;
#include "MainFrm.h"
#include "PrcDefaultContextForDGN.h"

#include "CustomPDFPropertiesDialog.h"
#include "CPdfExportSelectViewsDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


typedef OdArray<ODCOLORREF, OdMemoryAllocator<ODCOLORREF> > ODGSPALETTE;

/////////////////////////////////////////////////////////////////////////////
// CExport2PDFDialog dialog


CExport2PDFDialog::CExport2PDFDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CExport2PDFDialog::IDD, pParent)
{
  //{{AFX_DATA_INIT(CExport2PDFDialog)
  m_FileName = _T("");
  m_Autor = _T("");
  m_Keywords = _T("");
  m_Creator = _T("");
  m_Title = _T("");
  m_bEmbedded = FALSE;
  m_sPlotStyleTable = _T("");
  m_bEmbeddedOptimized = FALSE;
  m_bSimpleGeomOpt = FALSE;
  m_bSHXAsGeometry = TRUE;
  m_bTTFAsGeometry = TRUE;
  m_bZoomToExtents = TRUE;
  m_bEncoded = TRUE;
  m_bASCIIHEXEncoded = TRUE;
  m_bEnableLayers = FALSE;
  m_bExportOffLayers = FALSE;
  m_Producer = _T("");
  m_Subject = _T("");
  m_dPaperH = 0.0;
  m_dPaperW = 0.0;
  m_iHatchDPI = 150;
  m_Layouts = 0;
  m_Views = 0;
  m_bUseHLR = TRUE;
  m_bUsePRC = FALSE;
  m_iGeomDPI = 600;
  m_iColorImagesDPI = 400;
  m_iBWImagesDPI = 400;
  m_bExportHyperlinks = FALSE;
  m_bTextSearchable = FALSE;
  m_bCropImages = TRUE;
  m_bDCTCompression = TRUE;
  m_bMeasuring = FALSE;
  m_bUseGsCache = FALSE;
  m_iDCTQuality = 50;
  m_bLinearized = FALSE;
  m_bMergeLinesCrossing = FALSE;
  m_iPdfAMode = 0;
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

  DDX_Check(pDX, IDC_ENCODED, m_bEncoded);
  DDX_Check(pDX, IDC_PLOTSTYLETABLE, m_bPlotStyleTable);
  DDX_Text(pDX, IDC_EDIT_PLOTSTYLETABLE, m_sPlotStyleTable);
  DDX_Check(pDX, IDC_ENCODED2, m_bASCIIHEXEncoded);

  DDX_Text(pDX, IDC_PRODUCER, m_Producer);
  DDV_MaxChars(pDX, m_Producer, 255);
  DDX_Text(pDX, IDC_SUBJECT, m_Subject);
  DDV_MaxChars(pDX, m_Subject, 255);
  DDX_Text(pDX, IDC_EDIT_PAPER_HEIGHT, m_dPaperH);
  DDX_Text(pDX, IDC_EDIT_PAPER_WIDTH, m_dPaperW);
  DDX_Radio(pDX, IDC_RADIO_ACTIVE, m_Layouts);
  DDX_Radio(pDX, IDC_RADIO_ACTIVE2, m_Views);

  DDX_Check(pDX, IDC_HLR, m_bUseHLR);
  DDX_Check(pDX, IDC_PRC, m_bUsePRC);
  DDX_Check(pDX, IDC_HYPERLINK, m_bExportHyperlinks);
  DDX_Check(pDX, IDC_TEXT_SEARCHABLE, m_bTextSearchable);
  DDX_Check(pDX, IDC_LINEARIZED, m_bLinearized);

  DDX_Control(pDX, IDC_PDFEXPORT_COLOR_POLICY, m_colorPolicy);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CExport2PDFDialog, CDialog)
	//{{AFX_MSG_MAP(CExport2PDFDialog)
	ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
  ON_BN_CLICKED(IDOK, OnExport)
  //ON_BN_CLICKED(IDPREVIEW, OnPreview)
  ON_BN_CLICKED(IDC_LAYERS, OnCheckLayers)
  ON_BN_CLICKED(IDC_PLOTSTYLETABLE, OnBnClickedPlotStyleTable)
	ON_BN_CLICKED(IDC_BROWSE2, OnBrowse2)
  ON_BN_CLICKED(IDC_EMEDDED, OnCheckEmbedded)
  ON_BN_CLICKED(IDC_ZOOM, OnCheckZoom)

  ON_BN_CLICKED(IDC_RADIO_ACTIVE, OnParametersUpdated)
  ON_BN_CLICKED(IDC_RADIO_ACTIVE2, OnParametersUpdated)
  ON_BN_CLICKED(IDC_EX_MONO_CHECK, OnParametersUpdated)
  ON_BN_CLICKED(IDC_RADIO_ALL, OnParametersUpdated)
  ON_BN_CLICKED(IDC_RADIO_ALL2, OnParametersUpdated)
  ON_BN_CLICKED(IDC_RADIO_FENCE, OnParametersUpdated)
  ON_BN_CLICKED(IDC_EMEDDED_OPTIMIZED, OnParametersUpdated)
  ON_BN_CLICKED(IDC_TTF_AS_GEOM, OnCheckTextAsGeometry)
  ON_BN_CLICKED(IDC_SHX_AS_GEOM, OnCheckTextAsGeometry)
  ON_BN_CLICKED(IDC_GEOM_OPT, OnParametersUpdated)
  ON_BN_CLICKED(IDC_HLR, OnParametersUpdated)
  ON_BN_CLICKED(IDC_PRC, OnBnClickedPrc)
  ON_BN_CLICKED(IDC_HYPERLINK, OnParametersUpdated)
  ON_BN_CLICKED(IDC_TRANSPARENCY, OnParametersUpdated)
  ON_BN_CLICKED(IDC_TEXT_SEARCHABLE, OnParametersUpdated)

  ON_EN_CHANGE(IDC_EDIT_PAPER_WIDTH, OnParametersUpdated)
  ON_EN_CHANGE(IDC_EDIT_PAPER_HEIGHT, OnParametersUpdated)
  ON_BN_CLICKED(IDC_BUTTON_CUSTOM_PROPERTIES, OnClickCustomButton)
  ON_BN_CLICKED(IDC_BUTTON_SELECT_VIEWS, OnClickSelectViewButton)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CExport2PDFDialog::OnInitDialog()
{
  if (!CDialog::OnInitDialog())
    return FALSE;

  m_colorPolicy.ResetContent();

  m_colorPolicy.AddString(_T("As Is"));
  m_colorPolicy.AddString(_T("Monochrome"));
  m_colorPolicy.AddString(_T("Grayscale"));
  m_colorPolicy.AddString(_T("Use mono plotstyle"));
  m_colorPolicy.SetCurSel(0);

  if( !m_idFence.isNull() )
    GetDlgItem(IDC_RADIO_FENCE)->EnableWindow(TRUE);
  else
    GetDlgItem(IDC_RADIO_FENCE)->EnableWindow(FALSE);

  UpdateData( FALSE );

  return true;
}
/////////////////////////////////////////////////////////////////////////////
// CExport2PDFDialog message handlers

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

OdDbStub* CExport2PDFDialog::getNextViewForActiveLayout(OdDbStub* objectId)
{
  OdDgElementId prevVectorizedViewId(objectId);
  OdDgElementId nextVectorizedViewId;
  OdDgElementId vectorizedViewId;

  // OdDgElementId vectorizedViewId;
  OdDgViewGroupPtr pViewGroup = m_pDb->recommendActiveViewGroupId().openObject();

  //pDb->getActiveViewGroupId().openObject();
  if (!pViewGroup.isNull())
  {
    OdDgElementIteratorPtr pIt = pViewGroup->createIterator();
    bool bFound(false);
    for (; !pIt->done(); pIt->step())
    {
      OdDgViewPtr pView = OdDgView::cast(pIt->item().openObject());
      if (pView.get() && pView->getVisibleFlag())
      {
        vectorizedViewId = pIt->item();
        if (prevVectorizedViewId.isNull())
        {
          nextVectorizedViewId = vectorizedViewId;
          break;
        }
        else if (prevVectorizedViewId == vectorizedViewId)
        {
          bFound = true;
        }
        else if (bFound)
        {
          nextVectorizedViewId = vectorizedViewId;
          break;
        }
      }
    }
  }
  return nextVectorizedViewId;
}

void CExport2PDFDialog::fillLayoutList(OdArray<OdString> &layouts/*, OdArray<double> &divWH*/)
{
  if (m_Layouts == 1) // all
  {
    OdDgModelTablePtr pModelTable = m_pDb->getModelTable();
    if (!pModelTable.isNull())
    {
      OdDgElementIteratorPtr pIter = pModelTable->createIterator();
      for ( ; !pIter->done(); pIter->step() )
      {
        OdDgModelPtr pModel = OdDgModel::cast( pIter->item().openObject() );
        if ( pModel.get() )
        {
          OdString name = pModel->getName();

          OdDgElementId idViewGroup = pModel->getViewGroup(true);

          if( !idViewGroup.isNull() )
          {
            OdDgViewGroupPtr pViewGroup = idViewGroup.openObject(OdDg::kForRead);

            if( !pViewGroup.isNull() )
            {
              OdDgElementIteratorPtr pViewIter = pViewGroup->createIterator();

              for(; !pViewIter->done(); pViewIter->step() )
              {
                OdDgViewPtr pView = pViewIter->item().openObject(OdDg::kForRead);

                if( !pView.isNull() && pView->getVisibleFlag() )
                {
                  OdString strLayoutAndView;
                  strLayoutAndView.format(L"$%d", pView->getIndex() );
                  strLayoutAndView = name + strLayoutAndView;
                  layouts.push_back(strLayoutAndView);
                }
              }
            }
          }
        }
      }
    }
  }
  else if( m_Views == 1)
  {
    OdDgModelPtr pModel;
    OdDgElementId idModel = m_pDb->getActiveModelId();

    if( !idModel.isNull() )
      pModel = idModel.openObject(OdDg::kForRead);

    if( !pModel.isNull() )
    {
      OdString name = pModel->getName();

      OdDgElementId idViewGroup = pModel->getViewGroup(true);

      if( !idViewGroup.isNull() )
      {
        OdDgViewGroupPtr pViewGroup = idViewGroup.openObject(OdDg::kForRead);

        if( !pViewGroup.isNull() )
        {
          OdDgElementIteratorPtr pViewIter = pViewGroup->createIterator();

          for(; !pViewIter->done(); pViewIter->step() )
          {
            OdDgViewPtr pView = pViewIter->item().openObject(OdDg::kForRead);

            if( !pView.isNull() && pView->getVisibleFlag() )
            {
              OdString strLayoutAndView;
              strLayoutAndView.format(L"$%d", pView->getIndex() );
              strLayoutAndView = name + strLayoutAndView;
              layouts.push_back(strLayoutAndView);
            }
          }
        }
      }
    }
  }
}

OdDgElementId getActiveViewId( OdDgDatabase* pDb )
{
  OdDgElementId idRet;

  OdDgViewGroupPtr pViewGroup = pDb->getActiveViewGroupId().openObject();
  if( !pViewGroup.isNull() )
  {
    OdDgElementIteratorPtr pIt = pViewGroup->createIterator();
    for( ; !pIt->done(); pIt->step() )
    {
      OdDgViewPtr pView = OdDgView::cast( pIt->item().openObject() );
      if( pView.get() && pView->getVisibleFlag() )
      {
        idRet = pView->elementId();
        break;
      }
    }
  }

  return idRet;
}

OdUInt32 CExport2PDFDialog::ExportToPdf(OdString pdfFileName)
{
  OdPdfExportModulePtr pModule = ::odrxDynamicLinker()->loadApp(OdPdfExportModuleName);
  {
    OdPdfExportPtr exporter = pModule->create();
    {
      PDFExportParams params;

      if (!m_bEnableLayers)
        m_bExportOffLayers = false;

      bool bV15 = m_bEnableLayers || m_bExportOffLayers;
      params.setDatabase(m_pDb);
      params.setVersion(bV15 ? PDFExportParams::kPDFv1_5 : PDFExportParams::kPDFv1_4);

      params.setOutput(odrxSystemServices()->createFile(pdfFileName, Oda::kFileWrite, Oda::kShareDenyNo, Oda::kCreateAlways));

      params.setExportFlags(PDFExportParams::PDFExportFlags(
                            (m_bEmbeddedOptimized ? PDFExportParams::kEmbededOptimizedTTF : 0) |
                            (m_bEmbedded ? PDFExportParams::kEmbededTTF : 0) |
                            (m_bSHXAsGeometry ? PDFExportParams::kSHXTextAsGeometry : 0) |
                            (m_bTTFAsGeometry ? PDFExportParams::kTTFTextAsGeometry : 0) |
                            (m_bSimpleGeomOpt ? PDFExportParams::kSimpleGeomOptimization : 0) |
                            (m_bEnableLayers ? PDFExportParams::kEnableLayers : 0) |
                            (m_bExportOffLayers ? PDFExportParams::kIncludeOffLayers : 0) |
                            (m_bUseHLR ? PDFExportParams::kUseHLR : 0) |
                            (m_bEncoded ? PDFExportParams::kFlateCompression : 0) |
                            (m_bASCIIHEXEncoded ? PDFExportParams::kASCIIHexEncoding : 0) |
                            (m_bExportHyperlinks ? PDFExportParams::kExportHyperlinks : 0) |
                            (m_bLinearized ? PDFExportParams::kLinearized : 0) |
                            (m_bMergeLinesCrossing ? PDFExportParams::kMergeLines: 0) |
                            (m_bMeasuring ? PDFExportParams::kMeasuring : 0) |
                            (m_bZoomToExtents ? PDFExportParams::kZoomToExtentsMode : 0)));

      params.setTitle(OdString(m_Title));
      params.setAuthor(OdString(m_Autor));
      params.setSubject(OdString(m_Subject));
      params.setKeywords(OdString(m_Keywords));
      params.setCreator(OdString(m_Creator));
      params.setProducer(OdString(m_Producer));

      //params.setTGSpecific((m_Views == 1) ? true : false);

      params.setImageCropping(m_bCropImages==TRUE?true:false);

      params.setDCTCompression(m_bDCTCompression == TRUE ? true : false);
      params.setUseGsCache(m_bUseGsCache == TRUE ? true : false);
      params.setDCTQuality(m_iDCTQuality);
      params.setArchived((PDFExportParams::PDF_A_mode)m_iPdfAMode);

      PDFExportParams::SearchableTextType STType = (m_bTextSearchable && m_bTTFAsGeometry) ? PDFExportParams::kTTF : PDFExportParams::kNoSearch;
      if (m_bTextSearchable && m_bSHXAsGeometry)
        STType = (PDFExportParams::SearchableTextType)(STType | PDFExportParams::kSHX);
      params.setSearchableTextType(STType);

      // *** PRC begins here!
      if (m_bUsePRC)
      {
        params.setPRCMode (PDFExportParams::kAsMesh); //(m_bUsePRCAsBRep == TRUE ? PDFExportParams::kAsBrep : PDFExportParams::kAsMesh);
        OdRxModulePtr pModule = ::odrxDynamicLinker()->loadApp("OdPrcModule");
        if (!pModule.isNull())
        {
          pModule = ::odrxDynamicLinker()->loadApp("OdDgn2PrcExport"/*"OdPrcExport"*/);
        }
        if (pModule.isNull())
        {
          return /*exPdfExportServiceMissed*/ ERROR;
        }

        params.setPRCContext(odCreatePrcAllInSingleViewContextForDGN());
      }
      // *** PRC ends here!

      params.setGeomDPI(m_iGeomDPI);
      params.setBWImagesDPI(m_iBWImagesDPI);
      params.setColorImagesDPI(m_iColorImagesDPI);

      m_pDb->startUndoRecord();
      m_pDb->startTransaction();
      m_pDb->setPaperSize( m_dPaperW, m_dPaperH );
      m_pDb->setMargins( m_dTopMargin, m_dBottomMargin, m_dLeftMargin, m_dRightMargin );

      OdDg::OdDgPlotType uOldPlotType = m_pDb->getPlotType();

      if(params.exportFlags() & PDFExportParams::kZoomToExtentsMode )
        m_pDb->setPlotType(OdDg::kExtents);
      else
        m_pDb->setPlotType(OdDg::kView);

      //OdArray<double> divWH;
      if (m_LayoutsViews.isEmpty())
        fillLayoutList(params.layouts());
      else
        params.layouts() = m_LayoutsViews;

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

      if (bMonoPalette)
      {
        ODGSPALETTE pPalCpy(256);
        // monochrome
	      pPalCpy.setAll( ODRGB(0, 0, 0));
        // Color with #255 always defines background.
        pPalCpy[255] = ODRGB(255, 255, 255);
        // This method should be called to resolve "white background issue" before setting device palette
        bool bCorrected = OdDgColorTable::correctPaletteForWhiteBackground(pPalCpy.asArrayPtr());
        params.setPalette(pPalCpy.asArrayPtr());
      }

      if( (m_Layouts != 1) && (m_Views != 1) && m_LayoutsViews.isEmpty())
      {
        params.layouts().clear();
      }

      OdUInt32 nPages = odmax(1, params.layouts().size());

      OdGsPageParams pageParams;
      pageParams.set(m_dPaperW, m_dPaperH, m_dLeftMargin, m_dRightMargin, m_dTopMargin, m_dBottomMargin);
      params.pageParams().resize(nPages, pageParams);
      params.setHatchDPI(m_iHatchDPI);

      if (!m_sPlotStyleTable.IsEmpty())
      {
        OdString strDgnPlotFilename(m_sPlotStyleTable);
        m_pDb->setCurrentPenTableFile(strDgnPlotFilename);
      }

      OdDgElementId idVolumeClip;
      OdDgElementId idView;
      bool          bVolumeClip = true;

      // Use fence for pdf export.

      if( m_Views == 2 && !m_idFence.isNull() )
      {
         idView = getActiveViewId(m_pDb); // Get active view.

         if( !idView.isNull() )
         {
           OdDgViewPtr pView = idView.openObject(OdDg::kForWrite);
           idVolumeClip = pView->getVolumeClipElementId(); // Store old volume clip value.
           bVolumeClip = pView->getShowClipVolumeFlag(); // Store old volume clip status.

           pView->disableUndoRecording(true);  // Don't save undo. Modifications are local.
           pView->setShowClipVolumeFlag(true); // Set clip volume flag true.
           pView->setVolumeClipElementId( m_idFence ); // Set fence as volume clip element.
           pView->disableUndoRecording(false);

           OdDgGraphicsElementPtr pFence = m_idFence.openObject(OdDg::kForWrite, true);

           if( !pFence.isNull() )
           {
             pFence->disableUndoRecording(true);
             pFence->setInvisibleFlag(true); // Set fence as invisible element.
             pFence->disableUndoRecording(false);
           }
         }
      }

      OdUInt32 errCode = exporter->exportPdf(params);

      if( m_Views == 2 && !idView.isNull() )
      {
        OdDgViewPtr pView = idView.openObject(OdDg::kForWrite);
        pView->disableUndoRecording(true); // Don't save undo. Modifications are local.
        pView->setVolumeClipElementId( idVolumeClip ); // Restore volume clip and volume clip values for view.
        pView->setShowClipVolumeFlag(bVolumeClip);
        pView->disableUndoRecording(false);

        OdDgGraphicsElementPtr pFence = m_idFence.openObject(OdDg::kForWrite, true);
 
        if( !pFence.isNull() )
        {
          pFence->disableUndoRecording(true);
          pFence->setInvisibleFlag(false); // Restore fence visibility.
          pFence->disableUndoRecording(false);
        }
      }

      m_pDb->setPlotType(uOldPlotType);

      m_pDb->endTransaction();
      m_pDb->undo();

      return errCode;
    }
  }
}
//------
void CExport2PDFDialog::OnExport()
{
	// TODO: Add your control notification handler code here
  UpdateData(true); // save and validate

  CMainFrame* pMFrame = (CMainFrame*)theApp.GetMainWnd();
  pMFrame->StartTimer();

  try
  {
    odrxSystemServices()->createFile(OdString(m_FileName), Oda::kFileWrite, Oda::kShareDenyNo, Oda::kCreateAlways);
  }
  catch (OdError)
  {
    ::MessageBox(NULL, _T("Unable to create file"), _T("PDF error"), MB_OK);
    return;
  }

  OdUInt32 errCode = ExportToPdf(OdString(m_FileName));

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

void CExport2PDFDialog::OnBnClickedPrc()
{
  UpdateData( );

  if (m_bUsePRC)
  {
    m_bUseHLR = FALSE;
    UpdateData(false); // save and validate
  }

  GetDlgItem(IDC_HLR)->EnableWindow( !m_bUsePRC );
}

void CExport2PDFDialog::OnCheckLayers()
{
  UpdateData( );

  GetDlgItem(IDC_OFFLAYERS)->EnableWindow( m_bEnableLayers );
}

void CExport2PDFDialog::OnBnClickedPlotStyleTable()
{
  // TODO: Add your control notification handler code here
  UpdateData( );

  GetDlgItem(IDC_EDIT_PLOTSTYLETABLE)->EnableWindow( m_bPlotStyleTable );
  GetDlgItem(IDC_BROWSE2)->EnableWindow( m_bPlotStyleTable );
}

void CExport2PDFDialog::OnBrowse2()
{
  UpdateData(true); // save and validate

  CFileDialog dlgFile(true);

  dlgFile.m_ofn.Flags |= OFN_HIDEREADONLY | OFN_EXPLORER | OFN_PATHMUSTEXIST;

  CString strFilter;

  strFilter = "Pen Table Files (*.tbl)";
  strFilter += (TCHAR)'\0';   // next string please
  strFilter += _T("*.tbl");
  strFilter += (TCHAR)'\0';   // last string
  dlgFile.m_ofn.lpstrFilter = strFilter;

  dlgFile.m_ofn.lpstrDefExt = _T("tbl");
  dlgFile.m_ofn.lpstrTitle = _T("Select Pen Table File");

  if (dlgFile.DoModal() == IDOK)
  {
    m_sPlotStyleTable = dlgFile.GetPathName();
    if (m_sPlotStyleTable.Find('.') == -1)
    {
      m_sPlotStyleTable += ".tbl";
    }

    UpdateData(false); // save and validate
  }
}

void CExport2PDFDialog::OnCheckTextAsGeometry()
{
  UpdateData();
  GetDlgItem(IDC_TEXT_SEARCHABLE)->ShowWindow((m_bSHXAsGeometry || m_bTTFAsGeometry) ? SW_SHOWNORMAL : SW_HIDE);

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
  UpdateData();

  if( m_Views == 2 )
  {
    m_Layouts = 0;
    m_bZoomToExtents = true;
    UpdateData(FALSE);
    GetDlgItem(IDC_RADIO_ACTIVE)->EnableWindow(FALSE);
    GetDlgItem(IDC_RADIO_ALL)->EnableWindow(FALSE);
    GetDlgItem(IDC_ZOOM)->EnableWindow(FALSE);
  }
  else
  {
    GetDlgItem(IDC_RADIO_ACTIVE)->EnableWindow(TRUE);
    GetDlgItem(IDC_RADIO_ALL)->EnableWindow(TRUE);
    GetDlgItem(IDC_ZOOM)->EnableWindow(TRUE);
  }
}

void CExport2PDFDialog::OnCheckZoom()
{
  UpdateData( );

//   GetDlgItem(IDC_EDIT_PAPER_HEIGHT)->EnableWindow( m_bZoomToExtents );
//   GetDlgItem(IDC_EDIT_PAPER_WIDTH)->EnableWindow( m_bZoomToExtents );

  OnParametersUpdated();
}

void CExport2PDFDialog::OnClickCustomButton()
{
  CCustomPDFPropertiesDialog dlgCustom(&m_iHatchDPI, &m_iGeomDPI, &m_iColorImagesDPI, &m_iBWImagesDPI, &m_iDCTQuality, &m_iPdfAMode, &m_bMergeLinesCrossing, &m_bCropImages, &m_bDCTCompression, &m_bMeasuring, &m_bUseGsCache, this);
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
}

void CExport2PDFDialog::OnClickSelectViewButton()
{
  m_LayoutsViews.clear();
  CPdfExportSelectViewsDialog dlg(m_pDb, &m_LayoutsViews, this);
  if (dlg.DoModal() != IDOK)
    m_LayoutsViews.clear();
}
