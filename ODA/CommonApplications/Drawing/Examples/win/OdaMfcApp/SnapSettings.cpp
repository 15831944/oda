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

#include "stdafx.h"
#include "DbEntity.h"
#include "resource.h"
#include "SnapSettings.h"


// CSnapSettings dialog

IMPLEMENT_DYNAMIC(CSnapSettings, CDialog)
CSnapSettings::CSnapSettings(CWnd* pParent /*=NULL*/)
	: CDialog(CSnapSettings::IDD, pParent)
  , m_bSnapOn(false)
  , m_bEndPoint(false)
  , m_bMidPoint(false)
  , m_bCenter(false)
  , m_bNode(false)
  , m_bQuadrant(false)
  , m_bInsertion(false)
  , m_bPerpendicular(false)
  , m_bTangent(false)
  , m_bNearest(false)
  , m_dwSnapModes( 1 << OdDb::kOsModeEnd )
{
}

CSnapSettings::~CSnapSettings()
{
}

void CSnapSettings::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
  DDX_Check(pDX, IDC_CHECK1, m_bSnapOn);
  DDX_Check(pDX, IDC_CHECK2, m_bEndPoint);
  DDX_Check(pDX, IDC_CHECK3, m_bMidPoint);
  DDX_Check(pDX, IDC_CHECK4, m_bCenter);
  DDX_Check(pDX, IDC_CHECK5, m_bNode);
  DDX_Check(pDX, IDC_CHECK6, m_bQuadrant);
  DDX_Check(pDX, IDC_CHECK7, m_bInsertion);
  DDX_Check(pDX, IDC_CHECK8, m_bPerpendicular);
  DDX_Check(pDX, IDC_CHECK9, m_bTangent);
  DDX_Check(pDX, IDC_CHECK10, m_bNearest);
}


BEGIN_MESSAGE_MAP(CSnapSettings, CDialog)
END_MESSAGE_MAP()

BOOL CSnapSettings::OnInitDialog()
{
  m_bEndPoint = (m_dwSnapModes & ( 1 << OdDb::kOsModeEnd )) != 0;
  m_bMidPoint = (m_dwSnapModes & ( 1 << OdDb::kOsModeMid )) != 0;
  m_bCenter = (m_dwSnapModes & ( 1 << OdDb::kOsModeCen )) != 0;
  m_bNode = (m_dwSnapModes & ( 1 << OdDb::kOsModeNode )) != 0;
  m_bQuadrant = (m_dwSnapModes & ( 1 << OdDb::kOsModeQuad )) != 0;
  m_bInsertion = (m_dwSnapModes & ( 1 << OdDb::kOsModeIns )) != 0;
  m_bPerpendicular = (m_dwSnapModes & ( 1 << OdDb::kOsModePerp )) != 0;
  m_bTangent = (m_dwSnapModes & ( 1 << OdDb::kOsModeTan )) != 0;
  m_bNearest = (m_dwSnapModes & ( 1 << OdDb::kOsModeNear )) != 0;
  
  return CDialog::OnInitDialog();
}

void CSnapSettings::OnOK()
{
  CDialog::OnOK();
  
  m_dwSnapModes = 0;
  
  if ( m_bEndPoint ) m_dwSnapModes |= ( 1 << OdDb::kOsModeEnd );
  if ( m_bMidPoint ) m_dwSnapModes |= ( 1 << OdDb::kOsModeMid );
  if ( m_bCenter ) m_dwSnapModes |= ( 1 << OdDb::kOsModeCen );
  if ( m_bNode ) m_dwSnapModes |= ( 1 << OdDb::kOsModeNode );
  if ( m_bQuadrant ) m_dwSnapModes |= ( 1 << OdDb::kOsModeQuad );
  if ( m_bInsertion ) m_dwSnapModes |= ( 1 << OdDb::kOsModeIns );
  if ( m_bPerpendicular ) m_dwSnapModes |= ( 1 << OdDb::kOsModePerp );
  if ( m_bTangent ) m_dwSnapModes |= ( 1 << OdDb::kOsModeTan );
  if ( m_bNearest ) m_dwSnapModes |= ( 1 << OdDb::kOsModeNear );
}

// CSnapSettings message handlers
