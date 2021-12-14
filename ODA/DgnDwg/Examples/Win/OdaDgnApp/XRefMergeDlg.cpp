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
#include "XRefMergeDlg.h"
#include "DgModel.h"
#include "DgReferenceAttach.h"

// CXRefMergeDlg dialog

IMPLEMENT_DYNAMIC(CXRefMergeDlg, CDialog)

CXRefMergeDlg::CXRefMergeDlg(CWnd* pParent /*=NULL*/, OdDgDatabase* pDb )
	: CDialog(CXRefMergeDlg::IDD, pParent)
{
  m_iXRefIndex      = 0;
  m_pDb             = pDb;
  m_bClipXRef       = 1;
  m_bXRefColorTable = 1;
  m_bBindToCell     = 0;
}

//=================================================================================

CXRefMergeDlg::~CXRefMergeDlg()
{
}

//=================================================================================

void CXRefMergeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

  DDX_CBIndex(pDX, IDC_COMBO_XREFNAME, m_iXRefIndex);
  DDX_Check(pDX, IDC_BINDTOCELL, m_bBindToCell);
  DDX_Check(pDX, IDC_COLORTABLESUPPORT, m_bXRefColorTable);
  DDX_Check(pDX, IDC_CLIPSUPPORT, m_bClipXRef);
}

//=================================================================================

BOOL CXRefMergeDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  CComboBox* pCBXRefName = (CComboBox*)(GetDlgItem(IDC_COMBO_XREFNAME));
  pCBXRefName->Clear();
  m_arrxRefs.clear();

  if( m_pDb )
  {
    OdDgElementId idModel = m_pDb->getActiveModelId();

    if( !idModel.isNull() )
    {
      OdDgModelPtr pModel = idModel.openObject(OdDg::kForRead);

      OdDgElementIteratorPtr pIter = pModel->createControlElementsIterator();

      for(; !pIter->done(); pIter->step())
      {
        OdDgElementPtr pElm = pIter->item().openObject(OdDg::kForRead);

        if( !pElm.isNull() && pElm->isKindOf(OdDgReferenceAttachmentHeader::desc()) )
        {
          OdDgReferenceAttachmentHeaderPtr pXRef = pElm;

          OdString strId;
          strId.format(L"0x%X - ", (OdUInt64)(pXRef->elementId().getHandle()));
          OdString strName = strId + pXRef->getFileName();
          
          if( !pXRef->getModelName().isEmpty() )
            strName += L" (" + pXRef->getModelName() + L")";

          m_arrxRefs.push_back(pXRef->elementId());
          pCBXRefName->AddString(strName);
        }
      }

      if (m_arrxRefs.size() > 0)
        m_iXRefIndex = 0;

      UpdateData(FALSE);
    }
  }

  return TRUE;
}

//=================================================================================

OdDgElementId CXRefMergeDlg::getXRefId() const
{
  OdDgElementId idRet;

  if (m_iXRefIndex >= 0 && m_iXRefIndex < (int)m_arrxRefs.size())
    idRet = m_arrxRefs[m_iXRefIndex];

  return idRet;
}

//=================================================================================

BEGIN_MESSAGE_MAP(CXRefMergeDlg, CDialog)
END_MESSAGE_MAP()

//=================================================================================



