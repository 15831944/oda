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
#include "resource.h"
#include "EditProperties.h"
#include "DbFiler.h"
#include "DbObject.h"
#include "DbUnitsFormatter.h"
#include "CmColor.h"

// CEditProperties dialog

IMPLEMENT_DYNAMIC(CEditProperties, CDialog)
CEditProperties::CEditProperties(OdDbObjectId &id, CWnd* pParent /*=NULL*/)
	: CDialog(CEditProperties::IDD, pParent), m_pObjectId(id), m_nCurItem(-1)
{
}

CEditProperties::~CEditProperties()
{
}

void CEditProperties::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON1, m_doset);
	DDX_Control(pDX, IDC_PROPLIST, m_propList);
	DDX_Text(pDX, IDC_VALUE, m_sValue);
}


BEGIN_MESSAGE_MAP(CEditProperties, CDialog)
	ON_EN_SETFOCUS(IDC_VALUE, OnSetfocusValue)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton)
	ON_NOTIFY(NM_CLICK, IDC_PROPLIST, OnClickProplist)
	ON_NOTIFY(LVN_KEYDOWN, IDC_PROPLIST, OnKeydownProplist)
END_MESSAGE_MAP()

static OdString FormatValue( const OdResBuf* rb )
{
  OdString s;

  if (rb->restype() == OdResBuf::kRtEntName || rb->restype() == OdResBuf::kDxfEnd)
  {
    OdDbObjectId id = rb->getObjectId(0);
    s = id.getHandle().ascii();
    return s;
  }

  switch ( OdDxfCode::_getType( rb->restype() ) )
  {
  case OdDxfCode::Unknown: // to use RT codes
    if( rb->restype() == OdResBuf::kRtColor )
      s = OdDbUnitsFormatter::formatColor( rb->getColor() );
    break;
  case OdDxfCode::Name:
  case OdDxfCode::String:
  case OdDxfCode::Handle:
  case OdDxfCode::LayerName:
    s = rb->getString();
    break;
  case OdDxfCode::Bool:
    s.format(OD_T("%d"), rb->getBool());
    break;
  case OdDxfCode::Integer8:
    s.format(OD_T("%d"), rb->getInt8());
    break;
  case OdDxfCode::Integer16:
    s.format(OD_T("%d"), rb->getInt16());
    break;
  case OdDxfCode::Integer32:
    s.format(OD_T("%d"), rb->getInt32());
    break;
  case OdDxfCode::Integer64:
    s.format( L"%" PRId64W, rb->getInt64());
    break;
  case OdDxfCode::Double:
  case OdDxfCode::Angle:
    s.format(OD_T("%g"), rb->getDouble());
    break;
  case OdDxfCode::Point:
    s.format(OD_T("%g %g %g"), rb->getPoint3d().x,rb->getPoint3d().y, rb->getPoint3d().z );
    break;
  case OdDxfCode::ObjectId:
  case OdDxfCode::SoftPointerId:
  case OdDxfCode::HardPointerId:
  case OdDxfCode::SoftOwnershipId:
  case OdDxfCode::HardOwnershipId:
    s = rb->getHandle().ascii();
    break;
  default:
    break;
  }
  return s;
}

static OdString FormatCode( const OdResBuf* rb )
{
  OdString s;
  s.format( OD_T("%d"), rb->restype() );
  return s;
}

BOOL CEditProperties::OnInitDialog() 
{
	CDialog::OnInitDialog();
  m_propList.InsertColumn(0, _T("DXF code"), LVCFMT_LEFT, 180);
  m_propList.InsertColumn(1, _T("Value"), LVCFMT_LEFT, 120);
  
  m_pResBuf = oddbEntGet( m_pObjectId, L"*" );
  int i = 0;
  for ( OdResBufPtr rb = m_pResBuf ; !rb.isNull(); ++i, rb = rb->next() )
  {
    m_propList.InsertItem( i, FormatCode( rb ) );
    m_propList.SetItemText(i,1, FormatValue( rb ) );
  }
  return TRUE;
}

void CEditProperties::OnButton() 
{
  UpdateData();
  if (m_nCurItem == -1) return;
  OdResBufPtr rb = m_pResBuf;
  int i = 0;
  while ( !rb.isNull() && i < m_nCurItem )
  {
    ++i;
    rb = rb->next();
  }
  if ( rb.isNull() ) return;

  switch ( rb->restype() )
  {
  case OdResBuf::kRtColor:
    rb->setColor(OdDbUnitsFormatter::unformatColor((LPCTSTR)m_sValue));
    break;
  default:
  switch ( OdDxfCode::_getType( rb->restype() ) )
  {
  case OdDxfCode::Name:
  case OdDxfCode::String:
  case OdDxfCode::Handle:
  case OdDxfCode::LayerName:
    rb->setString((LPCTSTR)m_sValue);
    break;
  case OdDxfCode::Bool:
    rb->setBool(_ttoi(m_sValue) != 0);
    break;
  case OdDxfCode::Integer8:
    rb->setInt8(OdInt8(_ttoi(m_sValue)));
    break;
  case OdDxfCode::Integer16:
    rb->setInt16(OdInt16(_ttoi(m_sValue)));
    break;
  case OdDxfCode::Integer32:
    rb->setInt32(_ttoi(m_sValue));
    break;
  case OdDxfCode::Double:
  case OdDxfCode::Angle:
    rb->setDouble(_tcstod(m_sValue,0));
    break;
  case OdDxfCode::Point:
    {
      int sp1 = m_sValue.Find(' ');
      int sp2 = m_sValue.Find(' ',sp1+1);
      double x = _tcstod(m_sValue.Left(sp1),0);
      double y = _tcstod(m_sValue.Mid(sp1+1, sp2-sp1-1),0);
      double z = _tcstod(m_sValue.Mid(sp2+1),0);
      rb->setPoint3d(OdGePoint3d(x,y,z));
    }
    break;
  case OdDxfCode::ObjectId:
  case OdDxfCode::SoftPointerId:
  case OdDxfCode::HardPointerId:
  case OdDxfCode::SoftOwnershipId:
  case OdDxfCode::HardOwnershipId:
    rb->setHandle(OdDbHandle((LPCTSTR)m_sValue));
    break;
  default:
    break;
  }
  }
  m_propList.SetItemText(m_nCurItem, 1, m_sValue);
  try
  {
    oddbEntMod(m_pObjectId,m_pResBuf);
  }
  catch( const OdError& ex )
  {
    AfxMessageBox(ex.description());
  }
}

void CEditProperties::OnClickProplist(NMHDR* /*pNMHDR*/, LRESULT* pResult) 
{
  OnSetfocusValue();
	*pResult = 0;
}

void CEditProperties::OnSetfocusValue() 
{
	m_nCurItem = m_propList.GetSelectionMark();
  m_doset.EnableWindow(m_nCurItem != -1);
  if(m_nCurItem != -1)
  {
    m_sValue = m_propList.GetItemText(m_nCurItem, 1);
    UpdateData(FALSE);
  }
}

void CEditProperties::OnKeydownProplist(NMHDR* /*pNMHDR*/, LRESULT* pResult) 
{
  OnSetfocusValue();
	*pResult = 0;
}
