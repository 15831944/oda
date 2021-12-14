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
// CameraView.cpp : implementation file
//

#include "stdafx.h"
#include "OdaDgnApp.h"
#include "CameraView.h"

#define UPDATE_CTRL_TIMER 156

// CCameraView dialog

IMPLEMENT_DYNAMIC(CCameraViewDlg, CDialog)

CCameraViewDlg::CCameraViewDlg(CWnd* pParent /*=NULL*/, OdDgDatabase* pDb )
	: CDialog(CCameraViewDlg::IDD, pParent)
{
  m_pDb = pDb;
  m_iItemSelected = 0;
  m_iProjection = 0;
  m_iVisibility = 1;
  m_dPositionX  = 0;
  m_dPositionY  = 0;
  m_dPositionZ  = 0;
  m_dTargetX    = 0;
  m_dTargetY    = 0;
  m_dTargetZ    = 1;
  m_dOrientAngle  = 0;
  m_dElevateAngle = 0;
  m_dRollAngle    = 0;
  m_dFocusLength  = 1;
  m_dLensAngle    = 0.1;
  m_dClipFront    = 0;
  m_dClipBack     = 1;
}

CCameraViewDlg::~CCameraViewDlg()
{
}

void CCameraViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

  DDX_Text(pDX, IDC_POSXEDIT, m_dPositionX);
  DDX_Text(pDX, IDC_POSYEDIT, m_dPositionY);
  DDX_Text(pDX, IDC_POSZEDIT, m_dPositionZ);
  DDX_Text(pDX, IDC_TARGETXEDIT, m_dTargetX);
  DDX_Text(pDX, IDC_TARGETYEDIT, m_dTargetY);
  DDX_Text(pDX, IDC_TARGETZEDIT, m_dTargetZ);
  DDX_Text(pDX, IDC_FOCUSEDIT, m_dFocusLength);
  DDX_Text(pDX, IDC_LENSEDIT, m_dLensAngle);
  DDX_Text(pDX, IDC_CLIPFRONTEDIT, m_dClipFront);
  DDX_Text(pDX, IDC_CLIPBACKEDIT, m_dClipBack);
  DDX_Text(pDX, IDC_ORIENTEDIT, m_dOrientAngle);
  DDX_Text(pDX, IDC_ELEVATEEDIT, m_dElevateAngle);
  DDX_Text(pDX, IDC_ROLLEDIT, m_dRollAngle);
  DDX_Radio(pDX, IDC_PARALLELRADIO, m_iProjection );
  DDX_Radio(pDX, IDC_ONRADIO, m_iVisibility );
}


BEGIN_MESSAGE_MAP(CCameraViewDlg, CDialog)
  ON_NOTIFY(LVN_ITEMCHANGED, IDC_VIEWLIST, CCameraViewDlg::OnLvnItemchangedViewlist)
  ON_BN_CLICKED(IDC_PARALLELRADIO, CCameraViewDlg::OnBnClickedParallelradio)
  ON_BN_CLICKED(IDC_PROGECTIONRADIO, CCameraViewDlg::OnBnClickedParallelradio)
  ON_BN_CLICKED(IDC_ONRADIO, CCameraViewDlg::OnBnClickedOnradio)
  ON_BN_CLICKED(IDC_OFFRADIO, CCameraViewDlg::OnBnClickedOnradio)
  ON_BN_CLICKED(IDC_CLOSE, CCameraViewDlg::OnBnClickedClose)
  ON_BN_CLICKED(IDC_EXPAND_CLIPPING_PLANES, CCameraViewDlg::OnBnClickedExpandClippingPlanes)
  ON_EN_KILLFOCUS(IDC_FOCUSEDIT, CCameraViewDlg::OnEnKillfocusFocusedit)
  ON_EN_KILLFOCUS(IDC_LENSEDIT, CCameraViewDlg::OnEnKillfocusLensedit)
  ON_EN_KILLFOCUS(IDC_CLIPFRONTEDIT, CCameraViewDlg::OnEnKillfocusClipfrontedit)
  ON_EN_KILLFOCUS(IDC_CLIPBACKEDIT, CCameraViewDlg::OnEnKillfocusClipbackedit)
  ON_WM_TIMER()
  ON_EN_KILLFOCUS(IDC_ORIENTEDIT, CCameraViewDlg::OnEnKillfocusOrientedit)
  ON_EN_KILLFOCUS(IDC_ELEVATEEDIT, CCameraViewDlg::OnEnKillfocusElevateedit)
  ON_EN_KILLFOCUS(IDC_ROLLEDIT, CCameraViewDlg::OnEnKillfocusRolledit)
  ON_EN_KILLFOCUS(IDC_POSXEDIT, CCameraViewDlg::OnEnKillfocusPosxedit)
  ON_EN_KILLFOCUS(IDC_POSYEDIT, CCameraViewDlg::OnEnKillfocusPosxedit)
  ON_EN_KILLFOCUS(IDC_POSZEDIT, CCameraViewDlg::OnEnKillfocusPosxedit)
  ON_EN_KILLFOCUS(IDC_TARGETXEDIT, CCameraViewDlg::OnEnKillfocusTargetxedit)
  ON_EN_KILLFOCUS(IDC_TARGETYEDIT, CCameraViewDlg::OnEnKillfocusTargetxedit)
  ON_EN_KILLFOCUS(IDC_TARGETZEDIT, CCameraViewDlg::OnEnKillfocusTargetxedit)
END_MESSAGE_MAP()

void CCameraViewDlg::updateListCtrl( bool bCreateNew )
{
  CListCtrl* pList = (CListCtrl*)(GetDlgItem(IDC_VIEWLIST));

  if( pList )
  {
    pList->DeleteAllItems();

    if( bCreateNew )
    {
      pList->SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

      pList->InsertColumn(0,L"View number",LVCFMT_CENTER, 80 );
      pList->InsertColumn(1,L"Projection",LVCFMT_CENTER, 80 );
      pList->InsertColumn(2,L"Visibility",LVCFMT_CENTER, 52 );
      pList->InsertColumn(3,L"Clip Front",LVCFMT_CENTER, 100 );
      pList->InsertColumn(4,L"Clip Back",LVCFMT_CENTER, 100 );

      OdDgViewGroupTablePtr pViewTable = m_pDb->getViewGroupTable(OdDg::kForWrite);

      ODA_ASSERT( !pViewTable.isNull() );

      OdDgElementIteratorPtr pIter = pViewTable->createIterator();

      OdUInt32 iViewIndex = 0;

      for ( ; !pIter->done(); pIter->step() )
      {
        OdDgViewGroupPtr pViewGroup = OdDgViewGroup::cast( pIter->item().openObject(OdDg::kForWrite) );

        OdDgElementIteratorPtr pViewIter = pViewGroup->createIterator();

        for ( ; !pViewIter->done(); pViewIter->step() )
        {
          OdDgViewPtr pView = OdDgView::cast( pViewIter->item().openObject(OdDg::kForWrite) );

          if( !pView.isNull() )
          {
            m_pViewArr.push_back(pView);
            CString strName;

            if( pView->isNamed() )
            {
              strName = pView->getName().c_str();
            }
            else
            {
              strName.Format(L"%d", pView->getIndex() );
            }

            pList->InsertItem( iViewIndex,strName );

            if( pView->getVisibleFlag() )
            {
              pList->SetItem( iViewIndex, 2, LVIF_TEXT, L"On", 0, 0, 0, 0 );
            }
            else
            {
              pList->SetItem( iViewIndex, 2, LVIF_TEXT, L"Off", 0, 0, 0, 0 );
            }

            CString strFront;
            CString strBack;

            if( pView->getUseCameraFlag() )
            {
              pList->SetItem( iViewIndex, 1, LVIF_TEXT, L"Perspective", 0, 0, 0, 0 );

              strFront.Format( L"%4.4f", pView->getCameraFrontClippingDistance());
              strBack.Format( L"%4.4f", pView->getCameraBackClippingDistance());
            }
            else
            {
              pList->SetItem( iViewIndex, 1, LVIF_TEXT, L"Parallel", 0, 0, 0, 0 );

              strFront.Format( L"%4.4f", pView->getOrthographyFrontClippingDistance());
              strBack.Format( L"%4.4f", pView->getOrthographyBackClippingDistance());
            }

            pList->SetItem( iViewIndex, 3, LVIF_TEXT, strFront, 0, 0, 0, 0 );
            pList->SetItem( iViewIndex, 4, LVIF_TEXT, strBack, 0, 0, 0, 0 );

            iViewIndex++;
          }
        }
      }
    }
    else
    {
      OdUInt32 iViewIndex = 0;

      for ( OdUInt32 j = 0; j < m_pViewArr.size(); j++ )
      {
        OdDgViewPtr pView = m_pViewArr[j];

        if( !pView.isNull() )
        {
          CString strName;

          if( pView->isNamed() )
          {
            strName = pView->getName().c_str();
          }
          else
          {
            strName.Format(L"%d", pView->getIndex() );
          }

          pList->InsertItem( iViewIndex,strName );

          if( pView->getVisibleFlag() )
          {
            pList->SetItem( iViewIndex, 2, LVIF_TEXT, L"On", 0, 0, 0, 0 );
          }
          else
          {
            pList->SetItem( iViewIndex, 2, LVIF_TEXT, L"Off", 0, 0, 0, 0 );
          }

          CString strFront;
          CString strBack;

          if( pView->getUseCameraFlag() )
          {
            pList->SetItem( iViewIndex, 1, LVIF_TEXT, L"Perspective", 0, 0, 0, 0 );

            strFront.Format( L"%4.4f", pView->getCameraFrontClippingDistance());
            strBack.Format( L"%4.4f", pView->getCameraBackClippingDistance());
          }
          else
          {
            pList->SetItem( iViewIndex, 1, LVIF_TEXT, L"Parallel", 0, 0, 0, 0 );

            strFront.Format( L"%4.4f", pView->getOrthographyFrontClippingDistance());
            strBack.Format( L"%4.4f", pView->getOrthographyBackClippingDistance());
          }

          pList->SetItem( iViewIndex, 3, LVIF_TEXT, strFront, 0, 0, 0, 0 );
          pList->SetItem( iViewIndex, 4, LVIF_TEXT, strBack, 0, 0, 0, 0 );

          iViewIndex++;
        }
      }
    }

    pList->SetSelectionMark(m_iItemSelected);

    if( m_iItemSelected < pList->GetItemCount() )
    {
      pList->SetItemState( m_iItemSelected, LVIS_SELECTED,(UINT)-1);
    }

    pList->Invalidate(false);
  }
}

BOOL CCameraViewDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  if( !m_pDb )
  {
    return FALSE;
  }

  m_iItemSelected = 0;
  updateListCtrl(true);
  updateCtrls();

  return TRUE;
}

void CCameraViewDlg::updateCtrls()
{
  if( m_iItemSelected >= 0 && m_iItemSelected < (int)m_pViewArr.size() )
  {
    OdDgViewPtr pView = m_pViewArr[m_iItemSelected];

    ODA_ASSERT( !pView.isNull() );

    OdGeVector3d upVector = OdGeVector3d::kYAxis;
    OdGeVector3d rightVector = OdGeVector3d::kXAxis;
    OdGeVector3d directionVector = OdGeVector3d::kZAxis;
    OdGeVector3d vrDirZAxis = directionVector;
    OdGePoint3d  ptPos;
    OdGePoint3d  ptTarget;
    OdGeMatrix3d matRotation;
    double       dFocusLength;
    double       dLensAngle;
    double       dClipFront;
    double       dClipBack;

    m_iProjection = pView->getUseCameraFlag();
    m_iVisibility = !pView->getVisibleFlag();

    if( pView->getUseCameraFlag() )
    {
      pView->getCameraPosition(ptPos);
      pView->getCameraRotation( matRotation );
      matRotation.transposeIt();

      dFocusLength = pView->getCameraFocalLength();
      dLensAngle   = pView->getCameraLensAngle();
      dClipFront   = pView->getCameraFrontClippingDistance();
      dClipBack    = pView->getCameraBackClippingDistance();

      GetDlgItem(IDC_LENSEDIT)->EnableWindow(TRUE);
      GetDlgItem(IDC_FOCUSEDIT)->EnableWindow(TRUE);
    }
    else
    {
      directionVector = -directionVector;

      ptPos = pView->getOrthographyPosition();
      pView->getOrthographyRotation( matRotation );
      matRotation.transposeIt();

      dFocusLength = pView->getCameraFocalLength();
      dLensAngle   = pView->getCameraLensAngle();
      dClipFront   = pView->getOrthographyFrontClippingDistance();
      dClipBack    = pView->getOrthographyBackClippingDistance();

      GetDlgItem(IDC_LENSEDIT)->EnableWindow(FALSE);
      GetDlgItem(IDC_FOCUSEDIT)->EnableWindow(FALSE);
    }

    directionVector.transformBy(matRotation);
    upVector.transformBy(matRotation);
    rightVector.transformBy(matRotation);
    vrDirZAxis.transformBy(matRotation);

    OdGeVector3d vrXYDir = vrDirZAxis;
    vrXYDir.z = 0;

    if( !OdZero(vrXYDir.length()) )
    {
      vrXYDir.normalize();
      m_dOrientAngle = vrXYDir.angleTo( -OdGeVector3d::kXAxis, -OdGeVector3d::kZAxis );
    }
    else
    {
      m_dOrientAngle = 0;
    }

    if( m_dOrientAngle > OdaPI )
    {
      m_dOrientAngle = m_dOrientAngle - Oda2PI;
    }

    OdGeVector3d vrDirZCopy = vrDirZAxis;

    vrDirZAxis.rotateBy( m_dOrientAngle, -OdGeVector3d::kZAxis );
    m_dElevateAngle = vrDirZAxis.angleTo( -OdGeVector3d::kXAxis, OdGeVector3d::kYAxis );

    if( m_dElevateAngle > OdaPI )
    {
      m_dElevateAngle = m_dElevateAngle - Oda2PI;
    }

    OdGeVector3d vrUp = OdGeVector3d::kZAxis;

    vrUp.rotateBy( -m_dElevateAngle, OdGeVector3d::kYAxis );
    vrUp.rotateBy( -m_dOrientAngle, -OdGeVector3d::kZAxis );

    m_dRollAngle = vrUp.angleTo( upVector, vrDirZCopy );

    m_dRollAngle    *= 180/OdaPI;
    m_dElevateAngle *= 180/OdaPI;
    m_dOrientAngle  *= 180/OdaPI;

    ptTarget = ptPos + vrDirZAxis * dFocusLength;

    m_dPositionX   = ptPos.x;
    m_dPositionY   = ptPos.y;
    m_dPositionZ   = ptPos.z;
    m_dFocusLength = dFocusLength;
    m_dLensAngle   = dLensAngle * 180.0/OdaPI;
    m_dClipFront   = dClipFront;
    m_dClipBack    = dClipBack;
    m_dTargetX     = ptTarget.x;
    m_dTargetY     = ptTarget.y;
    m_dTargetZ     = ptTarget.z;
  }

  UpdateData(FALSE);
}
void CCameraViewDlg::OnLvnItemchangedViewlist(NMHDR *pNMHDR, LRESULT *pResult)
{
  UNUSED_ALWAYS( pNMHDR );

  SetTimer( UPDATE_CTRL_TIMER, 500, NULL );

  *pResult = 0;
}

void CCameraViewDlg::OnBnClickedParallelradio()
{
  int iProj = m_iProjection;

  UpdateData(TRUE);

  if( iProj != m_iProjection )
  {
    if( m_iItemSelected >= 0 && m_iItemSelected < (int)m_pViewArr.size() )
    {
      OdDgViewPtr pView = m_pViewArr[ m_iItemSelected ];

      if( m_iProjection == 0 ) // Parallel camera
      {
        pView->setUseCameraFlag( false );
      }
      else if( m_iProjection == 1 ) // Perspective camera
      {
        pView->setUseCameraFlag( true );
      }

      updateListCtrl();
      updateCtrls();
    }
  }
}

void CCameraViewDlg::OnBnClickedOnradio()
{
  int iOldState = m_iVisibility;

  UpdateData(TRUE);

  if( iOldState != m_iVisibility )
  {
    if( m_iItemSelected >= 0 && m_iItemSelected < (int)m_pViewArr.size() )
    {
      OdDgViewPtr pView = m_pViewArr[ m_iItemSelected ];

      if( m_iVisibility == 0 )
      {
        pView->setVisibleFlag( true );
      }
      else if( m_iVisibility == 1 )
      {
        pView->setVisibleFlag( false );
      }

      updateListCtrl();
    }
  }
}

void CCameraViewDlg::OnBnClickedExpandClippingPlanes()
{
  if( m_iItemSelected >= 0 && m_iItemSelected < (int)m_pViewArr.size() )
  {
    OdDgViewPtr pView = m_pViewArr[ m_iItemSelected ];
    OdDgModelPtr pModel = pView->getModelId().openObject(OdDg::kForRead);

    OdGeExtents3d extModel;

    pModel->getGeomExtents( extModel );

    if( pView->getUseCameraFlag() )
    {
      OdGePoint3d  ptViewPos;
      pView->getCameraPosition(ptViewPos);
      OdGeMatrix3d matRotation;
      pView->getCameraRotation( matRotation );

      OdGePoint3d ptTransformedPos = matRotation * ptViewPos;

      extModel.transformBy( matRotation );

      double dBackClip = extModel.maxPoint().z + ptTransformedPos.z;
      double dFrontClip  = extModel.minPoint().z + ptTransformedPos.z;

      pView->setCameraFrontClippingDistance( dFrontClip );
      pView->setCameraBackClippingDistance( dBackClip );
    }
    else
    {
      OdGePoint3d  ptViewPos = pView->getOrthographyPosition();
      OdGeMatrix3d matRotation;
      pView->getOrthographyRotation( matRotation );

      OdGePoint3d ptTransformedPos = matRotation * ptViewPos;

      extModel.transformBy( matRotation );

      double dBackClip = extModel.maxPoint().z - ptTransformedPos.z;
      double dFrontClip  = extModel.minPoint().z - ptTransformedPos.z;

      pView->setOrthographyFrontClippingDistance( dFrontClip );
      pView->setOrthographyBackClippingDistance( dBackClip );
    }

    updateListCtrl();
    updateCtrls();
  }
}

void CCameraViewDlg::OnBnClickedClose()
{
  CDialog::OnCancel();
}

void CCameraViewDlg::OnEnKillfocusFocusedit()
{
  double dOldLength = m_dFocusLength;

  UpdateData(TRUE);

  if( !OdZero(dOldLength - m_dFocusLength) )
  {
    if( m_iItemSelected >= 0 && m_iItemSelected < (int)m_pViewArr.size() )
    {
      OdDgViewPtr pView = m_pViewArr[ m_iItemSelected ];

      pView->setCameraFocalLength( m_dFocusLength );

      updateCtrls();
    }
  }
}

void CCameraViewDlg::OnEnKillfocusLensedit()
{
  double dOldAngel = m_dLensAngle;

  UpdateData(TRUE);

  if( !OdZero(dOldAngel - m_dLensAngle) )
  {
    if( m_iItemSelected >= 0 && m_iItemSelected < (int)m_pViewArr.size() )
    {
      OdDgViewPtr pView = m_pViewArr[ m_iItemSelected ];

      pView->setCameraLensAngle( m_dLensAngle );
    }
  }
}

void CCameraViewDlg::OnEnKillfocusClipfrontedit()
{
  double dOldFrontClip = m_dClipFront;

  bool bRestoreData = false;

  UpdateData(TRUE);

  if( !OdZero(dOldFrontClip - m_dClipFront) && (m_dClipFront < m_dClipBack) )
  {
    if( m_iItemSelected >= 0 && m_iItemSelected < (int)m_pViewArr.size() )
    {
      OdDgViewPtr pView = m_pViewArr[ m_iItemSelected ];

      if( pView->getUseCameraFlag() )
      {
        pView->setCameraFrontClippingDistance( m_dClipFront );
      }
      else
      {
        pView->setOrthographyFrontClippingDistance( m_dClipFront );
      }

      updateListCtrl();
      updateCtrls();
    }
    else
    {
      bRestoreData = true;
    }
  }
  else
  {
    bRestoreData = true;
  }

  if( bRestoreData )
  {
    m_dClipFront = dOldFrontClip;
    UpdateData(FALSE);
  }
}

void CCameraViewDlg::OnEnKillfocusClipbackedit()
{
  double dOldBackClip = m_dClipBack;

  bool bRestoreData = false;

  UpdateData(TRUE);

  if( !OdZero(dOldBackClip - m_dClipBack) && (m_dClipFront < m_dClipBack) )
  {
    if( m_iItemSelected >= 0 && m_iItemSelected < (int)m_pViewArr.size() )
    {
      OdDgViewPtr pView = m_pViewArr[ m_iItemSelected ];

      if( pView->getUseCameraFlag() )
      {
        pView->setCameraBackClippingDistance( m_dClipBack );
      }
      else
      {
        pView->setOrthographyBackClippingDistance( m_dClipBack );
      }

      updateListCtrl();
      updateCtrls();
    }
    else
    {
      bRestoreData = true;
    }
  }
  else
  {
    bRestoreData = true;
  }

  if( bRestoreData )
  {
    m_dClipBack = dOldBackClip;
    UpdateData(FALSE);
  }  
}

void CCameraViewDlg::OnTimer( UINT_PTR nIDEvent )
{
  if( nIDEvent == UPDATE_CTRL_TIMER )
  {
    CListCtrl* pList = (CListCtrl*)(GetDlgItem(IDC_VIEWLIST));

    if( pList )
    {
      int iItemSelected = pList->GetSelectionMark();

      if( (m_iItemSelected != iItemSelected) && 
        (iItemSelected >= 0) && 
        (iItemSelected < (int)m_pViewArr.size()) 
        )
      {
        m_iItemSelected = iItemSelected;
        updateCtrls();
      }
    }

    KillTimer(UPDATE_CTRL_TIMER);
  }
  else
  {
    CDialog::OnTimer( nIDEvent );
  }
}


void CCameraViewDlg::OnEnKillfocusOrientedit()
{
  double dOldAngle = m_dOrientAngle;

  UpdateData(TRUE);

  if( !OdZero( dOldAngle - m_dOrientAngle) )
  {
    if( m_iItemSelected >= 0 && m_iItemSelected < (int)m_pViewArr.size() )
    {
      OdDgViewPtr pView = m_pViewArr[ m_iItemSelected ];

      OdGeMatrix3d matRoatate = createRotationMatrix();

      if( pView->getUseCameraFlag() )
      {
        pView->setCameraRotation( matRoatate );
      }
      else
      {
        pView->setOrthographyRotation( matRoatate );
      }

      updateListCtrl();
      updateCtrls();
    }
  }
}

void CCameraViewDlg::OnEnKillfocusElevateedit()
{
  double dOldAngle = m_dElevateAngle;

  UpdateData(TRUE);

  if( !OdZero( dOldAngle - m_dElevateAngle) )
  {
    if( m_iItemSelected >= 0 && m_iItemSelected < (int)m_pViewArr.size() )
    {
      OdDgViewPtr pView = m_pViewArr[ m_iItemSelected ];

      OdGeMatrix3d matRoatate = createRotationMatrix();

      if( pView->getUseCameraFlag() )
      {
        pView->setCameraRotation( matRoatate );
      }
      else
      {
        pView->setOrthographyRotation( matRoatate );
      }

      updateListCtrl();
      updateCtrls();
    }
  }
}

void CCameraViewDlg::OnEnKillfocusRolledit()
{
  double dOldAngle = m_dRollAngle;

  UpdateData(TRUE);

  if( !OdZero( dOldAngle - m_dRollAngle) )
  {
    if( m_iItemSelected >= 0 && m_iItemSelected < (int)m_pViewArr.size() )
    {
      OdDgViewPtr pView = m_pViewArr[ m_iItemSelected ];

      OdGeMatrix3d matRoatate = createRotationMatrix();

      if( pView->getUseCameraFlag() )
      {
        pView->setCameraRotation( matRoatate );
      }
      else
      {
        pView->setOrthographyRotation( matRoatate );
      }

      updateListCtrl();
      updateCtrls();
    }
  }
}

OdGeMatrix3d CCameraViewDlg::createRotationMatrix()
{
  OdGeMatrix3d matRet;

  double dRollAngle    = m_dRollAngle * OdaPI / 180.0;
  double dElevateAngle = m_dElevateAngle * OdaPI / 180.0;
  double dOrientAngle  = m_dOrientAngle  * OdaPI / 180.0;

  OdGeMatrix3d matOrient;
  OdGeMatrix3d matElevate;
  OdGeMatrix3d matRoll;

  matRoll.setToRotation( OdaPI2 - dRollAngle, -OdGeVector3d::kZAxis);
  matElevate.setToRotation(-dElevateAngle - OdaPI2,OdGeVector3d::kYAxis);
  matOrient.setToRotation(-dOrientAngle, -OdGeVector3d::kZAxis );

  matRet *= matOrient;
  matRet *= matElevate;
  matRet *= matRoll;

  matRet.transposeIt();

  return matRet;
}

void CCameraViewDlg::OnEnKillfocusPosxedit()
{
  double dOldXPos = m_dPositionX;
  double dOldYPos = m_dPositionY;
  double dOldZPos = m_dPositionZ;

  UpdateData(TRUE);

  if( !OdZero( dOldXPos - m_dPositionX) || !OdZero( dOldYPos - m_dPositionY) || !OdZero( dOldZPos - m_dPositionZ) )
  {
    if( m_iItemSelected >= 0 && m_iItemSelected < (int)m_pViewArr.size() )
    {
      OdDgViewPtr pView = m_pViewArr[ m_iItemSelected ];

      OdGePoint3d ptPos( m_dPositionX, m_dPositionY, m_dPositionZ );

      if( pView->getUseCameraFlag() )
      {
        pView->setCameraPosition( ptPos );
      }
      else
      {
        pView->setOrthographyPosition( ptPos );
      }

      updateListCtrl();
      updateCtrls();
    }
  }
}

void CCameraViewDlg::OnEnKillfocusTargetxedit()
{
  double dOldXPos = m_dTargetX;
  double dOldYPos = m_dTargetY;
  double dOldZPos = m_dTargetZ;

  UpdateData(TRUE);

  if( !OdZero( dOldXPos - m_dTargetX) || !OdZero( dOldYPos - m_dTargetY) || !OdZero( dOldZPos - m_dTargetZ) )
  {
    if( m_iItemSelected >= 0 && m_iItemSelected < (int)m_pViewArr.size() )
    {
      OdDgViewPtr pView = m_pViewArr[ m_iItemSelected ];

      OdGePoint3d ptPos( m_dPositionX, m_dPositionY, m_dPositionZ );
      OdGePoint3d ptTarget( m_dTargetX, m_dTargetY, m_dTargetZ );

      OdGeVector3d vrDirection = ptTarget - ptPos;

      double dNewFocus = vrDirection.length();

      if( OdZero(dNewFocus) )
      {
        m_dTargetX = dOldXPos;
        m_dTargetY = dOldYPos;
        m_dTargetZ = dOldZPos;

        UpdateData(FALSE);
      }
      else
      {
        vrDirection.normalize();

        OdGeVector3d vrXYDir = vrDirection;
        vrXYDir.z = 0;

        if( !OdZero(vrXYDir.length()) )
        {
          vrXYDir.normalize();
          m_dOrientAngle = vrXYDir.angleTo( -OdGeVector3d::kXAxis, -OdGeVector3d::kZAxis );
        }
        else
        {
          m_dOrientAngle = 0;
        }

        if( m_dOrientAngle > OdaPI )
        {
          m_dOrientAngle = m_dOrientAngle - Oda2PI;
        }

        vrDirection.rotateBy( m_dOrientAngle, -OdGeVector3d::kZAxis );
        m_dElevateAngle = vrDirection.angleTo( -OdGeVector3d::kXAxis, OdGeVector3d::kYAxis );

        if( m_dElevateAngle > OdaPI )
        {
          m_dElevateAngle = m_dElevateAngle - Oda2PI;
        }

        m_dOrientAngle  *= 180/OdaPI;
        m_dElevateAngle *= 180/OdaPI;

        pView->setCameraFocalLength( dNewFocus );

        OdGeMatrix3d matRoatate = createRotationMatrix();

        if( pView->getUseCameraFlag() )
        {
          pView->setCameraRotation( matRoatate );
        }
        else
        {
          pView->setOrthographyRotation( matRoatate );
        }

        updateListCtrl();
        updateCtrls();
      }
    }
  }
}
