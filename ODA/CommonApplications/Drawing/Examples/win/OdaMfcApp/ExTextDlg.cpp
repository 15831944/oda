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
#include "OdaMfcApp.h"
#include "DbText.h"

#include "ExTextDlg.h"

#include "DbSymUtl.h"                   // for OdDbTextStyleTablePtr
#include "DbTextStyleTable.h"

//------------------------------------------------------------------------------------------------------------------

BEGIN_MESSAGE_MAP(CExTextDlg, CDialog)
 ON_BN_CLICKED(IDOK, &CExTextDlg::OnBnClickedOk)
 ON_WM_SHOWWINDOW()
 ON_CBN_SELCHANGE(IDC_JUSTIFY, &CExTextDlg::OnCbnSelchangeJustify)
 ON_CBN_SELCHANGE(IDC_STYLE, &CExTextDlg::OnCbnSelchangeStyle)
END_MESSAGE_MAP()

IMPLEMENT_DYNAMIC(CExTextDlg, CDialog)
//------------------------------------------------------------------------------------------------------------------

CExTextDlg::CExTextDlg(OdDbDatabasePtr& pDb, OdDbTextPtr& pText, CWnd* pParent /*=NULL*/)
         	: CDialog(CExTextDlg::IDD, pParent), pDb(pDb), pText(pText)
          , m_Text(_T(""))
          , m_Height(_T(""))
          , m_Angle(_T(""))
{
  m_hIcon  = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
  sCurrent = L" (current text style)";
}
//------------------------------------------------------------------------------------------------------------------

void CExTextDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	 CDialog::OnShowWindow(bShow, nStatus);

  CRect rct;
  GetWindowRect(&rct);
  if (rct.top == 0)
    MoveWindow(rct.left, rct.top + 55, rct.Width(), rct.Height());
}
//------------------------------------------------------------------------------------------------------------------

BOOL CExTextDlg::OnInitDialog() 
{
  CDialog::OnInitDialog();

  SetIcon(m_hIcon, TRUE);	 

  m_Justify.ResetContent();
  m_Justify.AddString(L" Left");
  m_Justify.AddString(L" Center");
  m_Justify.AddString(L" Right");
  m_Justify.AddString(L" Align");
  m_Justify.AddString(L" Midle");
  m_Justify.AddString(L" Fit");
  m_Justify.AddString(L" Top left");
  m_Justify.AddString(L" Top center");
  m_Justify.AddString(L" Top right");
  m_Justify.AddString(L" Midle left");
  m_Justify.AddString(L" Midle center");
  m_Justify.AddString(L" Midle right");
  m_Justify.AddString(L" Bottom left");
  m_Justify.AddString(L" Bottom center");
  m_Justify.AddString(L" Bottom right");
  m_Justify.SetCurSel(0);
  
  m_Style.ResetContent();
  idStyle = pDb->getTEXTSTYLE();      // default text style

  OdDbTextStyleTablePtr pTextStyleTable = pDb->getTextStyleTableId().openObject();
  OdDbSymbolTableIteratorPtr pIter = pTextStyleTable->newIterator();
  int i=0;
  for(pIter->start(); !pIter->done(); pIter->step(), i++)
  {
    if (idStyle == pIter->getRecord()->objectId())
    {
      m_Style.AddString((pIter->getRecord()->getName() + OdString(sCurrent)).c_str());
      m_Style.SetCurSel(i);
    }
    else
      m_Style.AddString(pIter->getRecord()->getName());
  }

  m_Height.Format(L"%.3lf",  pDb->getTEXTSIZE());
  ((CEdit*)GetDlgItem(IDC_HEIGHT))->SetLimitText(5);

  m_Angle = L"0";
  ((CEdit*)GetDlgItem(IDC_ANGLE))->SetLimitText(4);

  m_Text = L"Single line text";

  UpdateData(false);

  ((CEdit*)GetDlgItem(IDC_TEXT))->SetSel(0, m_Text.GetLength() + 1);
  GetDlgItem(IDC_TEXT)->SetFocus();

  return FALSE;                                               // return TRUE unless you set the focus to a control
}
//------------------------------------------------------------------------------------------------------------------

void CExTextDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);

  DDX_Control(pDX, IDC_JUSTIFY, m_Justify);
  DDX_Control(pDX, IDC_STYLE, m_Style);
  DDX_Text(pDX, IDC_TEXT, m_Text);
  DDX_Text(pDX, IDC_HEIGHT, m_Height);
  DDX_Text(pDX, IDC_ANGLE, m_Angle);
}
//------------------------------------------------------------------------------------------------------------------

void CExTextDlg::OnCbnSelchangeJustify()
{
  bool bEnable = m_Justify.GetCurSel() != Justify_Align && m_Justify.GetCurSel() != Justify_Fit;

  GetDlgItem(IDC_ANGLE)->EnableWindow(bEnable);
  GetDlgItem(IDC_ANGLE_ST)->EnableWindow(bEnable);
}
//------------------------------------------------------------------------------------------------------------------

void CExTextDlg::OnBnClickedOk()
{
  UpdateData(true);

  if (m_Text == L"")
  {                        
    MessageBox(L"Please, fill property \"Text\" ", L"Warning", MB_ICONWARNING);
    GetDlgItem(IDC_TEXT)->SetFocus();
    return;
  }

  double height;
  try
  {
    swscanf(m_Height.GetBuffer(), L"%lf", &height);
    if (height <= 0) throw(0);
  }
  catch(...)
  {                                                           
    MessageBox(L"Please, fill correct property \"Height\" ", L"Warning", MB_ICONWARNING);
   ((CEdit*)GetDlgItem(IDC_HEIGHT))->SetSel(0, m_Height.GetLength() + 1);
    GetDlgItem(IDC_HEIGHT)->SetFocus();
    return;
  }

  int angle = -77777;
  try
  {
    swscanf(m_Angle.GetBuffer(), L"%d", &angle);
    if (angle == -77777) throw(0);
  }
  catch(...)
  {                                                           
    MessageBox(L"Please, fill correct property \"Angle\" ", L"Warning", MB_ICONWARNING);
   ((CEdit*)GetDlgItem(IDC_ANGLE))->SetSel(0, m_Angle.GetLength() + 1);
    GetDlgItem(IDC_ANGLE)->SetFocus();
    return;
  }

  CString sName;
  m_Style.GetWindowText(sName);
  if (sName.Find(sCurrent)<0)
  {
    idStyle = OdDbSymUtil::getTextStyleId(sName.GetBuffer(0), pDb);
    if (!idStyle.isNull())
      pDb->setTEXTSTYLE(idStyle);
  }

  pDb->setTEXTSIZE(height);

  pText = OdDbText::createObject();
  pText->setDatabaseDefaults(pDb);

  pText->setTextString(OdString(m_Text));

//pText->setHeight(height);
  pText->setRotation(OdaToRadian(angle));

  OdDb::TextHorzMode hMode = OdDb::kTextLeft;
  OdDb::TextVertMode vMode = OdDb::kTextBase;

  switch(m_Justify.GetCurSel())
  {
    case  Justify_Left:
            break;
    case  Justify_Center:
            hMode = OdDb::kTextCenter;
            break;
    case  Justify_Right:
            hMode = OdDb::kTextRight;
            break;
    case  Justify_Align:
            hMode = OdDb::kTextAlign;
            break;
    case  Justify_Midle:
            hMode = OdDb::kTextMid;
            break;
    case  Justify_Fit:
            hMode = OdDb::kTextFit;
            break;
    case  Justify_Top_left:
            hMode = OdDb::kTextLeft;
            vMode = OdDb::kTextTop;
            break;
    case  Justify_Top_center:
            hMode = OdDb::kTextCenter;
            vMode = OdDb::kTextTop;
            break;
    case  Justify_Top_right:
            hMode = OdDb::kTextRight;
            vMode = OdDb::kTextTop;
            break;
    case  Justify_Midle_left:
            hMode = OdDb::kTextLeft;
            vMode = OdDb::kTextVertMid;
            break;
    case  Justify_Midle_center:
            hMode = OdDb::kTextCenter;
            vMode = OdDb::kTextVertMid;
           break;
    case Justify_Midle_right:
            hMode = OdDb::kTextRight;
            vMode = OdDb::kTextVertMid;
            break;
    case Justify_Bottom_left:
            hMode = OdDb::kTextLeft;
            vMode = OdDb::kTextBottom;
            break;
    case Justify_Bottom_center:
            hMode = OdDb::kTextCenter;
            vMode = OdDb::kTextBottom;
            break;
    case Justify_Bottom_right:
            hMode = OdDb::kTextRight;
            vMode = OdDb::kTextBottom;
            break;
    default:
            break;
  }
  pText->setHorizontalMode(hMode);
  pText->setVerticalMode  (vMode);

  CDialog::OnOK();
}
//------------------------------------------------------------------------------------------------------------------

void CExTextDlg::OnCbnSelchangeStyle()
{
  CString sName;
  m_Style.GetWindowText(sName);
  sName.Replace(sCurrent, L"");

  OdDbObjectId Temp_idStyle = OdDbSymUtil::getTextStyleId(sName.GetBuffer(0), pDb);
  if (!Temp_idStyle.isNull())
  {
    pDb->setTEXTSTYLE(Temp_idStyle);
    m_Height.Format(L"%.3lf",  pDb->getTEXTSIZE());
    UpdateData(false);
    pDb->setTEXTSTYLE(idStyle);
  }

}
//------------------------------------------------------------------------------------------------------------------
