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
// ViewFlagsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "OdaDgnApp.h"
#include "ViewFlagsDlg.h"

// CViewFlagsDlg dialog

IMPLEMENT_DYNAMIC(CViewFlagsDlg, CDialog)

CViewFlagsDlg::CViewFlagsDlg(CWnd* pParent /*=NULL*/, OdDgDatabase* pDb, OdDgView* pView )
	: CDialog(CViewFlagsDlg::IDD, pParent)
{
  m_pDb   = pDb;
  m_pView = pView;

  m_bShowACS            = 0;
  m_bDrawBackground     = 1;
  m_bBoundaryDispaly    = 0;
  m_bUseCamera          = 0;
  m_bClipFront          = 0;
  m_bClipBack           = 0;
  m_bClipVolume         = 0;
  m_bShowConstructions  = 1;
  m_bFastCurves         = 0;
  m_bShowDimensions     = 1;
  m_bShowDataFields     = 1;
  m_bDisplayset         = 0;
  m_bFastCells          = 0;
  m_bShowFills          = 1;
  m_bShowGrid           = 0;
  m_bLevelOverrides     = 0;
  m_bUseLineStyles      = 1;
  m_bUseLineWeights     = 1;
  m_bShowTextures       = 1;
  m_bShowPatterns       = 1;
  m_bShowTags           = 1;
  m_bShowTexts          = 1;
  m_bShowTextNodes      = 1;
  m_bUseTransparency    = 1;
  m_iSelectedItem       = -1;
  m_bRegenerateDrawing  = false;
}

//=================================================================================

CViewFlagsDlg::~CViewFlagsDlg()
{
}

//=================================================================================

void CViewFlagsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

  DDX_Check(pDX, IDC_ACS_CHECK, m_bShowACS );
  DDX_Check(pDX, IDC_BG_CHECK, m_bDrawBackground );
  DDX_Check(pDX, IDC_DISPLAYBOUNDARY_CHECK, m_bBoundaryDispaly );
  DDX_Check(pDX, IDC_CAMERA_CHECK, m_bUseCamera );
  DDX_Check(pDX, IDC_CLIPFRONT_CHECK, m_bClipFront );
  DDX_Check(pDX, IDC_CLIPBACK_CHECK, m_bClipBack );
  DDX_Check(pDX, IDC_CLIPVOLUME_CHECK, m_bClipVolume );
  DDX_Check(pDX, IDC_CONSTRUCTIONS_CHECK, m_bShowConstructions );
  DDX_Check(pDX, IDC_FAST_CURVES_CHECK, m_bFastCurves );
  DDX_Check(pDX, IDC_DIM_CHECK, m_bShowDimensions );
  DDX_Check(pDX, IDC_FIELDS_CHECK, m_bShowDataFields );
  DDX_Check(pDX, IDC_DISPLAYSET_CHECK, m_bDisplayset );
  DDX_Check(pDX, IDC_FASTCELLS_CHECK, m_bFastCells );
  DDX_Check(pDX, IDC_FILLS_CHECK, m_bShowFills );
  DDX_Check(pDX, IDC_GRID_CHECK, m_bShowGrid );
  DDX_Check(pDX, IDC_LEVELOVERRIDE_CHECK, m_bLevelOverrides );
  DDX_Check(pDX, IDC_LSTYLE_CHECK, m_bUseLineStyles );
  DDX_Check(pDX, IDC_LWEIGHT_CHECK, m_bUseLineWeights );
  DDX_Check(pDX, IDC_PATTERNS_CHECK, m_bShowPatterns );
  DDX_Check(pDX, IDC_TEXTURE_CHECK, m_bShowTextures );
  DDX_Check(pDX, IDC_TAGS_CHECK, m_bShowTags );
  DDX_Check(pDX, IDC_TEXT_CHECK, m_bShowTexts );
  DDX_Check(pDX, IDC_TEXTNODE_CHECK, m_bShowTextNodes );
  DDX_Check(pDX, IDC_TRANSPARENCY_CHECK, m_bUseTransparency );
  DDX_CBIndex(pDX, IDC_DISPLAYSTYLE_COMBO, m_iSelectedItem );
}

//=================================================================================

BEGIN_MESSAGE_MAP(CViewFlagsDlg, CDialog)
  ON_BN_CLICKED(IDC_ACS_CHECK, CViewFlagsDlg::OnBnClickedAcsCheck)
  ON_BN_CLICKED(IDC_BG_CHECK, CViewFlagsDlg::OnBnClickedBackgroundCheck)
  ON_BN_CLICKED(IDC_DISPLAYBOUNDARY_CHECK, CViewFlagsDlg::OnBnClickedBoundaryDisplayCheck)
  ON_BN_CLICKED(IDC_CAMERA_CHECK, CViewFlagsDlg::OnBnClickedCameraCheck)
  ON_BN_CLICKED(IDC_CLIPFRONT_CHECK, CViewFlagsDlg::OnBnClickedClipFrontCheck)
  ON_BN_CLICKED(IDC_CLIPBACK_CHECK, CViewFlagsDlg::OnBnClickedClipBackCheck)
  ON_BN_CLICKED(IDC_CLIPVOLUME_CHECK, CViewFlagsDlg::OnBnClickedClipVolumeCheck)
  ON_BN_CLICKED(IDC_CONSTRUCTIONS_CHECK, CViewFlagsDlg::OnBnClickedConstructionsCheck)
  ON_BN_CLICKED(IDC_FAST_CURVES_CHECK, CViewFlagsDlg::OnBnClickedFastCurvesCheck)
  ON_BN_CLICKED(IDC_DIM_CHECK, CViewFlagsDlg::OnBnClickedDimensionCheck)
  ON_BN_CLICKED(IDC_FIELDS_CHECK, CViewFlagsDlg::OnBnClickedDataFieldCheck)
  ON_BN_CLICKED(IDC_DISPLAYSET_CHECK, CViewFlagsDlg::OnBnClickedDisplaysetCheck)
  ON_BN_CLICKED(IDC_FASTCELLS_CHECK, CViewFlagsDlg::OnBnClickedFastCellsCheck)
  ON_BN_CLICKED(IDC_FILLS_CHECK, CViewFlagsDlg::OnBnClickedFillsCheck)
  ON_BN_CLICKED(IDC_GRID_CHECK, CViewFlagsDlg::OnBnClickedGridCheck)
  ON_BN_CLICKED(IDC_LEVELOVERRIDE_CHECK, CViewFlagsDlg::OnBnClickedLevelOverrigeCheck)
  ON_BN_CLICKED(IDC_LSTYLE_CHECK, CViewFlagsDlg::OnBnClickedLineStyleCheck)
  ON_BN_CLICKED(IDC_LWEIGHT_CHECK, CViewFlagsDlg::OnBnClickedLineWeightCheck)
  ON_BN_CLICKED(IDC_TEXTURE_CHECK, CViewFlagsDlg::OnBnClickedTexturesCheck)
  ON_BN_CLICKED(IDC_PATTERNS_CHECK, CViewFlagsDlg::OnBnClickedPatternsCheck)
  ON_BN_CLICKED(IDC_TAGS_CHECK, CViewFlagsDlg::OnBnClickedTagsCheck)
  ON_BN_CLICKED(IDC_TEXT_CHECK, CViewFlagsDlg::OnBnClickedTextsCheck)
  ON_BN_CLICKED(IDC_TEXTNODE_CHECK, CViewFlagsDlg::OnBnClickedTextNodesCheck)
  ON_BN_CLICKED(IDC_TRANSPARENCY_CHECK, CViewFlagsDlg::OnBnClickedTransparencyCheck)
  ON_CBN_SELCHANGE(IDC_DISPLAYSTYLE_COMBO, CViewFlagsDlg::OnDisplayStyleComboSelchange )
END_MESSAGE_MAP()

//=================================================================================

void CViewFlagsDlg::OnDisplayStyleComboSelchange()
{
  UpdateData(TRUE);

  CComboBox* pDisplayStyleCombo = (CComboBox*)(GetDlgItem(IDC_DISPLAYSTYLE_COMBO));

  if( pDisplayStyleCombo && (m_iSelectedItem >= 0) )
  {
    CString strText;
    pDisplayStyleCombo->GetLBText(m_iSelectedItem, strText);

    if( m_pDb && m_pView )
    {
      bool bDisplayStyleWasApplied = false;

      OdDgDisplayStyleTablePtr pDisplayStyleTable = m_pDb->getDisplayStyleTable(OdDg::kForRead);

      if( !pDisplayStyleTable.isNull() )
      {
        OdDgElementId idDisplayStyle = pDisplayStyleTable->getAt(OdString(strText));

        if( !idDisplayStyle.isNull() )
        {
          OdDgDisplayStyleViewPEPtr pDisplayStylePE( m_pView );

          if( !pDisplayStylePE.isNull() )
          {
            pDisplayStylePE->setDisplayStyle(m_pView, idDisplayStyle );
            bDisplayStyleWasApplied = true;
          }
        }
      }

      if( !bDisplayStyleWasApplied )
      {
        if( strText == L"Wireframe" )
        {
          OdDgDisplayStyleViewPEPtr pDisplayStylePE( m_pView );

          if( !pDisplayStylePE.isNull() )
          {
            pDisplayStylePE->setDisplayStyle(m_pView, NULL );
          }

          m_pView->setDisplayMode( OdDgView::kWireFrame );
          bDisplayStyleWasApplied = true;
        }
        else if( strText == L"3dWireframe" )
        {
          OdDgDisplayStyleViewPEPtr pDisplayStylePE( m_pView );

          if( !pDisplayStylePE.isNull() )
          {
            pDisplayStylePE->setDisplayStyle(m_pView, NULL );
          }

          m_pView->setDisplayMode( OdDgView::kWireMesh );
          bDisplayStyleWasApplied = true;
        }
        else if( strText == L"HiddenLine" )
        {
          OdDgDisplayStyleViewPEPtr pDisplayStylePE( m_pView );

          if( !pDisplayStylePE.isNull() )
          {
            pDisplayStylePE->setDisplayStyle(m_pView, NULL );
          }

          m_pView->setDisplayMode( OdDgView::kHiddenLine );
          bDisplayStyleWasApplied = true;
        }
        else if( strText == L"FlatShaded" || strText == L"FlatShadedWithEdges" )
        {
          OdDgDisplayStyleViewPEPtr pDisplayStylePE( m_pView );

          if( !pDisplayStylePE.isNull() )
          {
            pDisplayStylePE->setDisplayStyle(m_pView, NULL );
          }

          m_pView->setDisplayMode( OdDgView::kConstantShade );
          m_pView->setDisplayEdgesFlag( strText == L"FlatShadedWithEdges" );
          bDisplayStyleWasApplied = true;
        }
        else if( strText == L"GouraudShaded" || strText == L"GouraudShadedWithEdges" )
        {
          OdDgDisplayStyleViewPEPtr pDisplayStylePE( m_pView );

          if( !pDisplayStylePE.isNull() )
          {
            pDisplayStylePE->setDisplayStyle(m_pView, NULL );
          }

          m_pView->setDisplayMode( OdDgView::kPhongShade );
          m_pView->setDisplayEdgesFlag( strText == L"GouraudShadedWithEdges" );
          bDisplayStyleWasApplied = true;
        }
      }

      if( bDisplayStyleWasApplied )
      {
        m_bRegenerateDrawing = true;
      }
    }
  }
}

//=================================================================================

void CViewFlagsDlg::OnBnClickedAcsCheck()
{
  UpdateData(TRUE);

  if( m_pView )
  {
    m_pView->setShowAxisTriadFlag( m_bShowACS != 0 ? true : false);
  }
}

//=================================================================================

void CViewFlagsDlg::OnBnClickedBackgroundCheck()
{
  UpdateData(TRUE);

  if( m_pView )
  {
    m_pView->setShowBackgroundFlag( m_bDrawBackground != 0 ? true : false);
    m_bRegenerateDrawing = true;
  }
}

//=================================================================================

void CViewFlagsDlg::OnBnClickedBoundaryDisplayCheck()
{
  UpdateData(TRUE);

  if( m_pView )
  {
    m_pView->setShowBoundaryFlag( m_bBoundaryDispaly != 0 ? true : false);
  }
}

//=================================================================================

void CViewFlagsDlg::OnBnClickedCameraCheck()
{
  UpdateData(TRUE);

  if( m_pView )
  {
    bool bOldUseCamera = m_pView->getUseCameraFlag();

    OdAbstractViewPEPtr pAVP(m_pView);

    if( bOldUseCamera && (m_bUseCamera == 0)) // Perspective to orthography
    {
      pAVP->setView( m_pView, pAVP->target(m_pView), pAVP->direction(m_pView), pAVP->upVector(m_pView),
                     pAVP->fieldWidth(m_pView), pAVP->fieldHeight(m_pView), false);

      OdDgModelPtr pModel = m_pView->getModelId().openObject(OdDg::kForRead);

      OdGeExtents3d extModel;
      pModel->getGeomExtents(extModel);
      OdGePoint3d  ptViewPos = m_pView->getOrthographyPosition();
      OdGeMatrix3d matRotation;
      m_pView->getOrthographyRotation(matRotation);
      OdGePoint3d ptTransformedPos = matRotation * ptViewPos;
      extModel.transformBy(matRotation);
      double dBackClip = extModel.maxPoint().z + ptTransformedPos.z;
      double dFrontClip = extModel.minPoint().z + ptTransformedPos.z;

      pAVP->setFrontClipDistance(m_pView, 1 - dFrontClip);
      pAVP->setBackClipDistance(m_pView, 1 - dBackClip);
    }
    else if( !bOldUseCamera &&(m_bUseCamera == 1)) // Orthography to perspective
    {
      pAVP->setView( m_pView, pAVP->target(m_pView), pAVP->direction(m_pView), pAVP->upVector(m_pView),
                     pAVP->fieldWidth(m_pView), pAVP->fieldHeight(m_pView), true);
    }

    m_bRegenerateDrawing = true;
  }
}

//=================================================================================

void CViewFlagsDlg::OnBnClickedClipFrontCheck()
{
  UpdateData(TRUE);

  if( m_pView )
  {
    m_pView->setShowClipFrontFlag( m_bClipFront != 0 ? true : false);
    m_bRegenerateDrawing = true;
  }
}

//=================================================================================

void CViewFlagsDlg::OnBnClickedClipBackCheck()
{
  UpdateData(TRUE);

  if( m_pView )
  {
    m_pView->setShowClipBackFlag( m_bClipBack != 0 ? true : false);
    m_bRegenerateDrawing = true;
  }
}

//=================================================================================

void CViewFlagsDlg::OnBnClickedClipVolumeCheck()
{
  UpdateData(TRUE);

  if( m_pView )
  {
    m_pView->setShowClipVolumeFlag( m_bClipVolume != 0 ? true : false);
    m_bRegenerateDrawing = true;
  }
}

//=================================================================================

void CViewFlagsDlg::OnBnClickedConstructionsCheck()
{
  UpdateData(TRUE);

  if( m_pView )
  {
    m_pView->setShowConstructionFlag( m_bShowConstructions != 0 ? true : false);
    m_bRegenerateDrawing = true;
  }
}

//=================================================================================

void CViewFlagsDlg::OnBnClickedFastCurvesCheck()
{
  UpdateData(TRUE);

  if( m_pView )
  {
    m_pView->setFastCurveFlag( m_bFastCurves != 0 ? true : false);
  }
}

//=================================================================================

void CViewFlagsDlg::OnBnClickedDimensionCheck()
{
  UpdateData(TRUE);

  if( m_pView )
  {
    m_pView->setShowDimensionsFlag( m_bShowDimensions != 0 ? true : false);
    m_bRegenerateDrawing = true;
  }
}

//=================================================================================

void CViewFlagsDlg::OnBnClickedDataFieldCheck()
{
  UpdateData(TRUE);

  if( m_pView )
  {
    m_pView->setShowDataFieldsFlag( m_bShowDataFields != 0 ? true : false);
    m_bRegenerateDrawing = true;
  }
}

//=================================================================================

void CViewFlagsDlg::OnBnClickedDisplaysetCheck()
{
  UpdateData(TRUE);

  if( m_pView )
  {
    m_pView->setUseDisplaySetFlag( m_bDisplayset != 0 ? true : false);
  }
}

//=================================================================================

void CViewFlagsDlg::OnBnClickedFastCellsCheck()
{
  UpdateData(TRUE);

  if( m_pView )
  {
    m_pView->setFastCellsFlag( m_bFastCells != 0 ? true : false);
  }
}

//=================================================================================

void CViewFlagsDlg::OnBnClickedFillsCheck()
{
  UpdateData(TRUE);

  if( m_pView )
  {
    m_pView->setShowFillsFlag( m_bShowFills != 0 ? true : false);
    m_bRegenerateDrawing = true;
  }
}

//=================================================================================

void CViewFlagsDlg::OnBnClickedGridCheck()
{
  UpdateData(TRUE);

  if( m_pView )
  {
    m_pView->setShowGridFlag( m_bShowGrid != 0 ? true : false);
  }
}

//=================================================================================

void CViewFlagsDlg::OnBnClickedLevelOverrigeCheck()
{
  UpdateData(TRUE);

  if( m_pView )
  {
    m_pView->setShowLevelSymbologyFlag( m_bLevelOverrides != 0 ? true : false);
    m_bRegenerateDrawing = true;
  }
}

//=================================================================================

void CViewFlagsDlg::OnBnClickedLineStyleCheck()
{
  UpdateData(TRUE);

  if( m_pView )
  {
    m_pView->setIgnoreLineStylesFlag( m_bUseLineStyles != 0 ? false : true);
    m_bRegenerateDrawing = true;
  }
}

//=================================================================================

void CViewFlagsDlg::OnBnClickedLineWeightCheck()
{
  UpdateData(TRUE);

  if( m_pView )
  {
    m_pView->setShowLineWeightsFlag( m_bUseLineWeights != 0 ? true : false);
    m_bRegenerateDrawing = true;
  }
}

//=================================================================================

void CViewFlagsDlg::OnBnClickedTexturesCheck()
{
  UpdateData(TRUE);

  if( m_pView )
  {
    m_pView->setShowTextureMapsFlag( m_bShowTextures != 0 ? true : false);
    m_bRegenerateDrawing = true;
  }
}

//=================================================================================

void CViewFlagsDlg::OnBnClickedPatternsCheck()
{
  UpdateData(TRUE);

  if( m_pView )
  {
    m_pView->setShowPatternsFlag( m_bShowPatterns != 0 ? true : false);
    m_bRegenerateDrawing = true;
  }
}

//=================================================================================

void CViewFlagsDlg::OnBnClickedTagsCheck()
{
  UpdateData(TRUE);

  if( m_pView )
  {
    m_pView->setShowTagsFlag( m_bShowTags != 0 ? true : false);
    m_bRegenerateDrawing = true;
  }
}

//=================================================================================

void CViewFlagsDlg::OnBnClickedTextsCheck()
{
  UpdateData(TRUE);

  if( m_pView )
  {
    m_pView->setHideTextsFlag( m_bShowTexts != 0 ? false : true);
    m_bRegenerateDrawing = true;
  }
}

//=================================================================================

void CViewFlagsDlg::OnBnClickedTextNodesCheck()
{
  UpdateData(TRUE);

  if( m_pView )
  {
    m_pView->setShowTextNodesFlag( m_bShowTextNodes != 0 ? true : false);
    m_bRegenerateDrawing = true;
  }
}

//=================================================================================

void CViewFlagsDlg::OnBnClickedTransparencyCheck()
{
  UpdateData(TRUE);

  if( m_pView )
  {
    m_pView->setUseTransparencyFlag( m_bUseTransparency != 0 ? true : false);
    m_bRegenerateDrawing = true;
  }
}

//=================================================================================

BOOL CViewFlagsDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  if( !m_pDb || !m_pView)
  {
    return FALSE;
  }

  m_bShowACS            = m_pView->getShowAxisTriadFlag() ? 1 : 0;
  m_bDrawBackground     = m_pView->getShowBackgroundFlag() ? 1 : 0;
  m_bBoundaryDispaly    = m_pView->getShowBoundaryFlag() ? 1 : 0;
  m_bUseCamera          = m_pView->getUseCameraFlag() ? 1 : 0;
  m_bClipFront          = m_pView->getShowClipFrontFlag() ? 1 : 0;
  m_bClipBack           = m_pView->getShowClipBackFlag() ? 1 : 0;
  m_bClipVolume         = m_pView->getShowClipVolumeFlag() ? 1 : 0;
  m_bShowConstructions  = m_pView->getShowConstructionFlag() ? 1 : 0;
  m_bFastCurves         = m_pView->getFastCurveFlag() ? 1 : 0;
  m_bShowDimensions     = m_pView->getShowDimensionsFlag() ? 1 : 0;
  m_bShowDataFields     = m_pView->getShowDataFieldsFlag() ? 1 : 0;
  m_bDisplayset         = m_pView->getUseDisplaySetFlag() ? 1 : 0;
  m_bFastCells          = m_pView->getFastCellsFlag() ? 1 : 0;
  m_bShowFills          = m_pView->getShowFillsFlag() ? 1 : 0;
  m_bShowGrid           = m_pView->getShowGridFlag() ? 1 : 0;
  m_bLevelOverrides     = m_pView->getShowLevelSymbologyFlag() ? 1 : 0;
  m_bUseLineStyles      = m_pView->getIgnoreLineStylesFlag() ? 0 : 1;
  m_bUseLineWeights     = m_pView->getShowLineWeightsFlag() ? 1 : 0;
  m_bShowTextures       = m_pView->getShowTextureMapsFlag() ? 1 : 0;
  m_bShowPatterns       = m_pView->getShowPatternsFlag() ? 1 : 0;
  m_bShowTags           = m_pView->getShowTagsFlag() ? 1 : 0;
  m_bShowTexts          = m_pView->getHideTextsFlag() ? 0 : 1;
  m_bShowTextNodes      = m_pView->getShowTextNodesFlag() ? 1 : 0;
  m_bUseTransparency    = m_pView->getUseTransparencyFlag() ? 1 : 0;
  m_iSelectedItem       = -1;

  CComboBox* pCBDisplayStyle = (CComboBox*)(GetDlgItem(IDC_DISPLAYSTYLE_COMBO));

  pCBDisplayStyle->AddString(L"Wireframe");
  pCBDisplayStyle->AddString(L"3dWireframe");
  pCBDisplayStyle->AddString(L"HiddenLine");
  pCBDisplayStyle->AddString(L"FlatShaded");
  pCBDisplayStyle->AddString(L"GouraudShaded");
  pCBDisplayStyle->AddString(L"FlatShadedWithEdges");
  pCBDisplayStyle->AddString(L"GouraudShadedWithEdges");

  OdDgDisplayStyleTablePtr pDisplayStyleTable = m_pDb->getDisplayStyleTable(OdDg::kForRead);

  bool bAddSeparator = true;

  if( !pDisplayStyleTable.isNull() )
  {
    OdDgElementIteratorPtr pDispalyStyleIter = pDisplayStyleTable->createIterator();

    for(; !pDispalyStyleIter->done(); pDispalyStyleIter->step() )
    {
      OdDgDisplayStyleTableRecordPtr pDisplayStyle = pDispalyStyleIter->item().openObject(OdDg::kForRead);

      if( !pDisplayStyle.isNull() && !pDisplayStyle->getHideInPickerFlag() )
      {
        if( pCBDisplayStyle->FindString(0,pDisplayStyle->getName()) != -1 )
        {
          continue;
        }

        if( bAddSeparator )
        {
          pCBDisplayStyle->AddString(L"----------------------");
          bAddSeparator = false;
        }
        pCBDisplayStyle->AddString(pDisplayStyle->getName());
      }
    }
  }

  OdDgDisplayStyleViewPEPtr pDisplayStylePE( m_pView );

  if( !pDisplayStylePE->getDisplayStyle(m_pView).isNull() )
  {
    OdDgElementId idDisplayStyle = pDisplayStylePE->getDisplayStyle(m_pView);

    OdDgDisplayStyleTableRecordPtr pDisplayStyle = idDisplayStyle.openObject(OdDg::kForRead);

    if( !pDisplayStyle.isNull() )
    {
      OdString strDisplayStyleName = pDisplayStyle->getName();

      m_iSelectedItem = pCBDisplayStyle->FindString(0, strDisplayStyleName );
    }
  }
  else
  {
    switch( m_pView->getDisplayMode() )
    {
      case OdDgView::kWireFrame :
      {
        m_iSelectedItem = 0;
      } break;

      case OdDgView::kWireMesh :
      {
        m_iSelectedItem = 1;
      } break;

      case OdDgView::kHiddenLine :
      case OdDgView::kFilledHiddenLine :
      {
        m_iSelectedItem = 2;
      } break;

      case OdDgView::kConstantShade :
      {
        if( m_pView->getDisplayEdgesFlag() )
        {
          m_iSelectedItem = 5;
        }
        else
        {
          m_iSelectedItem = 3;
        }
      } break;

      case OdDgView::kSmoothShade :
      case OdDgView::kPhongShade :
      {
        if( m_pView->getDisplayEdgesFlag() )
        {
          m_iSelectedItem = 6;
        }
        else
        {
          m_iSelectedItem = 4;
        }
      } break;
    }
  }

  UpdateData(FALSE);

  return TRUE;
}

//=================================================================================



