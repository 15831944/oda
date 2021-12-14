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
// UI for direct vectorizer properties editing

#include "stdafx.h"
#include "VectProps.h"

#include "OdaCommon.h"
#include "Gs/Gs.h"

#if _MSC_VER < 1400
#include <atlbase.h>
#include <atlwin.h>
#endif

/////////////////////////////////////////////////////////////////////////////
// CSimpleInputDlg dialog
CSimpleInputDlg::CSimpleInputDlg(CWnd* pParent /*=NULL*/)
  : CDialog(CSimpleInputDlg::IDD, pParent)
  , m_IpType(kIpNone)
  , m_pIp(NULL)
{
  //{{AFX_DATA_INIT(CSimpleInputDlg)
    // NOTE: the ClassWizard will add member initialization here
  //}}AFX_DATA_INIT
}

void CSimpleInputDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CSimpleInputDlg)
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSimpleInputDlg, CDialog)
  //{{AFX_MSG_MAP(CSimpleInputDlg)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CSimpleInputDlg::OnInitDialog() 
{
  CDialog::OnInitDialog();

  SetWindowText(m_IpCap);
  SetDlgItemText(IDC_SDESCRIPTION, m_IpDesc);

  if (m_IpType != kIpBool)
  {
    GetDlgItem(IDC_RBTRUE)->ShowWindow(SW_HIDE);
    GetDlgItem(IDC_RBFALSE)->ShowWindow(SW_HIDE);
  }
  else
  {
    GetDlgItem(IDC_EVALUE)->ShowWindow(SW_HIDE);
  }

  switch (m_IpType)
  {
    case kIpNone:
      GetDlgItem(IDC_EVALUE)->EnableWindow(FALSE);
    break;
    case kIpBool:
      if (*(bool*)m_pIp)
        CheckRadioButton(IDC_RBTRUE, IDC_RBFALSE, IDC_RBTRUE);
      else
        CheckRadioButton(IDC_RBTRUE, IDC_RBFALSE, IDC_RBFALSE);
    break;
    case kIpInt:
      SetDlgItemInt(IDC_EVALUE, (UINT)*(OdInt32*)m_pIp, TRUE);
    break;
    case kIpUInt:
      SetDlgItemInt(IDC_EVALUE, (UINT)*(OdInt32*)m_pIp, FALSE);
    break;
    case kIpDouble:
      { OdString txt = odDToStr(*(double*)m_pIp, 'f', 8);
        SetDlgItemText(IDC_EVALUE, txt.c_str());
      }
    break;
    case kIpString:
      SetDlgItemText(IDC_EVALUE, ((OdString*)m_pIp)->c_str());
    break;
  }
   
  return TRUE;  // return TRUE unless you set the focus to a control
                // EXCEPTION: OCX Property Pages should return FALSE
}

static bool preValidate(const OdString &pStr, const OdChar *syms)
{
  const OdChar *pIn = pStr.c_str();
  while (*pIn)
  {
    const OdChar *pOp = syms;
    while (*pOp)
    {
      if (*pIn == *pOp)
        break;
      pOp++;
    }
    if (!*pOp)
      return false;
    pIn++;
  }
  return true;
}

void CSimpleInputDlg::OnOK() 
{
  switch (m_IpType)
  {
    case kIpBool:
      *(bool*)m_pIp = IsDlgButtonChecked(IDC_RBTRUE) == BST_CHECKED;
    break;
    case kIpInt:
      { CString tmpStr; GetDlgItemText(IDC_EVALUE, tmpStr);
        OdString valIn(tmpStr);
        bool bValid = false;
        OdInt32 val = 0;
        if (preValidate(valIn, OD_T("-+0123456789")))
        {
          val = (OdInt32)GetDlgItemInt(IDC_EVALUE, NULL, TRUE);
          if (val >= m_IpLim.m_int[0] && val <= m_IpLim.m_int[1])
            bValid = true;
        }
        if (!bValid)
        {
          MessageBox(OdString().format(OD_T("Value must be an signed integer in range %d to %d."), m_IpLim.m_int[0], m_IpLim.m_int[1]).c_str(),
                     OD_T("Input Validation Error"), MB_ICONWARNING);
          return;
        }
        *(OdInt32*)m_pIp = val;
      }
    break;
    case kIpUInt:
      { CString tmpStr; GetDlgItemText(IDC_EVALUE, tmpStr);
        OdString valIn(tmpStr);
        bool bValid = false;
        OdUInt32 val = 0;
        if (preValidate(valIn, OD_T("+0123456789")))
        {
          val = (OdUInt32)GetDlgItemInt(IDC_EVALUE, NULL, FALSE);
          if (val >= m_IpLim.m_uint[0] && val <= m_IpLim.m_uint[1])
            bValid = true;
        }
        if (!bValid)
        {
          MessageBox(OdString().format(OD_T("Value must be an unsigned integer in range %u to %u."), m_IpLim.m_uint[0], m_IpLim.m_uint[1]).c_str(),
                     OD_T("Input Validation Error"), MB_ICONWARNING);
          return;
        }
        *(OdUInt32*)m_pIp = val;
      }
    break;
    case kIpDouble:
      { CString tmpStr; GetDlgItemText(IDC_EVALUE, tmpStr);
        OdString valIn(tmpStr);
        bool bValid = false;
        double val = 0.0;
        if (preValidate(valIn, OD_T("-+.eE0123456789")))
        {
          val = odStrToD(valIn);
          if (val >= m_IpLim.m_double[0] && val <= m_IpLim.m_double[1])
            bValid = true;
        }
        if (!bValid)
        {
          MessageBox(OdString().format(OD_T("Value must be an floating point number in range %f to %f."), m_IpLim.m_double[0], m_IpLim.m_double[1]).c_str(),
                     OD_T("Input Validation Error"), MB_ICONWARNING);
          return;
        }
        *(double*)m_pIp = val;
      }
    break;
    case kIpString:
      {
        CString tmpStr; GetDlgItemText(IDC_EVALUE, tmpStr);
        OdString valIn(tmpStr);
        *(OdString*)m_pIp = valIn;
      }
    break;
  }

  CDialog::OnOK();
}

bool CSimpleInputDlg::DoInput(LPCTSTR pCaption, LPCTSTR pDesc, bool *pBool)
{
  m_IpCap = pCaption; m_IpDesc = pDesc;
  m_IpType = kIpBool; m_pIp = pBool;
  return (DoModal() == IDOK);
}

bool CSimpleInputDlg::DoInput(LPCTSTR pCaption, LPCTSTR pDesc, OdInt32 *pInt, OdInt32 rangeFrom, OdInt32 rangeTo)
{
  m_IpCap = pCaption; m_IpDesc = pDesc;
  m_IpType = kIpInt; m_pIp = pInt;
  m_IpLim.m_int[0] = rangeFrom; m_IpLim.m_int[1] = rangeTo;
  return (DoModal() == IDOK);
}

bool CSimpleInputDlg::DoInput(LPCTSTR pCaption, LPCTSTR pDesc, OdUInt32 *pUInt, OdUInt32 rangeFrom, OdUInt32 rangeTo)
{
  m_IpCap = pCaption; m_IpDesc = pDesc;
  m_IpType = kIpUInt; m_pIp = pUInt;
  m_IpLim.m_uint[0] = rangeFrom; m_IpLim.m_uint[1] = rangeTo;
  return (DoModal() == IDOK);
}

bool CSimpleInputDlg::DoInput(LPCTSTR pCaption, LPCTSTR pDesc, double *pDouble, double rangeFrom, double rangeTo)
{
  m_IpCap = pCaption; m_IpDesc = pDesc;
  m_IpType = kIpDouble; m_pIp = pDouble;
  m_IpLim.m_double[0] = rangeFrom; m_IpLim.m_double[1] = rangeTo;
  return (DoModal() == IDOK);
}

bool CSimpleInputDlg::DoInput(LPCTSTR pCaption, LPCTSTR pDesc, OdString *pStr)
{
  m_IpCap = pCaption; m_IpDesc = pDesc;
  m_IpType = kIpString; m_pIp = pStr;
  return (DoModal() == IDOK);
}

/////////////////////////////////////////////////////////////////////////////
// CVectPropsDlg dialog
CVectPropsDlg::CVectPropsDlg(OdGsDevice* pDevice, OdGsModule *pModule, CWnd* pParent /*=NULL*/)
  : CDialog(CVectPropsDlg::IDD, pParent)
  , m_pDevice(pDevice)
  , m_pModule(pModule)
{
  //{{AFX_DATA_INIT(CVectPropsDlg)
    // NOTE: the ClassWizard will add member initialization here
  //}}AFX_DATA_INIT
}

void CVectPropsDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CVectPropsDlg)
  DDX_Control(pDX, IDC_VECTPROPS_LIST, m_PropsList);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CVectPropsDlg, CDialog)
  //{{AFX_MSG_MAP(CVectPropsDlg)
  ON_NOTIFY(NM_CLICK, IDC_VECTPROPS_LIST, OnClickListPropsList)
  ON_BN_CLICKED(IDC_BUTTON2, &CVectPropsDlg::OnBitmapDeviceButtonClicked)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVectPropsDlg message handlers

static void RxVarToString(OdRxVariantValue &vVal, OdString &type, OdString &data)
{
  if (vVal.isNull())
  {
    type = OD_T("corrupted"); data = OD_T("null");
  }
  else
  {
    switch (vVal->type())
    {
      case OdVariant::kString:
      case OdVariant::kWString:
        type = OD_T("string"); data = vVal->getString();
      break;
      case OdVariant::kBool:
        type = OD_T("bool"); data = (vVal->getBool()) ? OD_T("true") : OD_T("false");
      break;
      //case OdVariant::kInt8:
      //  type = OD_T("int8"); data.format(OD_T("%d"), (OdInt32)vVal->getInt8());
      //break;
      case OdVariant::kUInt8:
        type = OD_T("uint8"); data.format(OD_T("%u"), (OdUInt32)vVal->getUInt8());
      break;
      //case OdVariant::kInt16:
      //  type = OD_T("int16"); data.format(OD_T("%d"), (OdInt32)vVal->getInt16());
      //break;
      case OdVariant::kUInt16:
        type = OD_T("uint16"); data.format(OD_T("%u"), (OdUInt32)vVal->getUInt16());
      break;
      //case OdVariant::kInt32:
      //  type = OD_T("int32"); data.format(OD_T("%d"), vVal->getInt32());
      //break;
      case OdVariant::kUInt32:
        type = OD_T("uint32"); data.format(OD_T("%u"), vVal->getUInt32());
      break;
      //case OdVariant::kInt64:
      //  type = OD_T("int64"); data.format(OD_T("%d"), (OdInt32)vVal->getInt64());
      //break;
      case OdVariant::kUInt64:
        type = OD_T("uint64"); data.format(OD_T("%u"), (OdUInt32)vVal->getUInt64());
      break;
      case OdVariant::kDouble:
        type = OD_T("double"); data.format(OD_T("%f"), vVal->getDouble());
      break;
      //case OdVariant::kAnsiString:
      //  type = OD_T("ansi string"); data = vVal->getAnsiString();
      //break;
      case OdVariant::kRxObjectPtr:
        type = OD_T("rx object"); data = (vVal->getRxObjectPtr().isNull()) ? OD_T("null") : vVal->getRxObjectPtr()->isA()->name().c_str();
      break;
      case (OdVariant::kArray | OdVariant::kString):
        type = OD_T("string array"); 
        { OdStringArray parseArray = vVal->getStringArray();
          data.format(OD_T("%u entries:"), (unsigned)parseArray.size());
          for (OdStringArray::size_type nv = 0; nv < parseArray.size(); nv++)
            data += OdString().format(OD_T("\n-------> %u entry: %s"), (unsigned)nv, parseArray.at(nv).c_str());
        }
      break;
      default:
        type = OD_T("unknown"); data.format(OD_T("type 0x%x"), (unsigned)vVal->type());
    }
  }
}

static OdVariant::Type StringToRxVarType(const OdString &str)
{
  if (!str.compare(OD_T("string")))
    return OdVariant::kString;
  else if (!str.compare(OD_T("bool")))
    return OdVariant::kBool;
  else if (!str.compare(OD_T("uint8")))
    return OdVariant::kUInt8;
  else if (!str.compare(OD_T("uint16")))
    return OdVariant::kUInt16;
  else if (!str.compare(OD_T("uint32")))
    return OdVariant::kUInt32;
  else if (!str.compare(OD_T("uint64")))
    return OdVariant::kUInt64;
  else if (!str.compare(OD_T("double")))
    return OdVariant::kDouble;
  return OdVariant::kVoid;
}

BOOL CVectPropsDlg::OnInitDialog() 
{
  CDialog::OnInitDialog();

  if (m_pDevice)
  {
    RECT rect;
    m_PropsList.GetClientRect(&rect);
    m_PropsList.InsertColumn(0, _T("Name") , LVCFMT_LEFT, (rect.right - rect.left) / 4);
    m_PropsList.InsertColumn(1, _T("Type") , LVCFMT_LEFT, (rect.right - rect.left) / 4);
    m_PropsList.InsertColumn(2, _T("Value"), LVCFMT_LEFT, (rect.right - rect.left) / 2);
    OdRxDictionaryPtr pProps = m_pDevice->properties();
    int nItem = 0;
    if (!pProps.isNull())
    {
      OdRxDictionaryIteratorPtr pIt = pProps->newIterator();
      while (!pIt->done())
      { OdString type, data;
        if (pIt->object().isNull() || pIt->object()->isA() == OdRxObject::desc())
        { OdRxVariantValue vVal(pIt->object());
          RxVarToString(vVal, type, data);
        } else
          type = OdRxObject::desc()->name(),
          data = pIt->object()->isA()->name();
        m_PropsList.InsertItem(nItem, pIt->getKey().c_str());
        m_PropsList.SetItemText(nItem, 1, type.c_str());
        m_PropsList.SetItemText(nItem, 2, data.c_str());
        nItem++;
        pIt->next();
      }
    }
  }

  {
    CRect rect, winRect;
    WINDOWINFO winInfo;
    GetDlgItem(IDOK)->GetWindowRect(&rect);
    ScreenToClient(&rect);
    GetDlgItem(IDOK)->GetWindowInfo(&winInfo);
    GetClientRect(&winRect);
    rect.SetRect(winRect.right - rect.right, rect.top, (winRect.right - rect.right) + (rect.right - rect.left) / 2 * 3, rect.bottom);
    m_BitmapDevice.Create(_T("BitmapDeviceProps"), winInfo.dwStyle & ~BS_DEFPUSHBUTTON, rect, this, IDC_BUTTON2);
    m_BitmapDevice.SetFont(GetFont(), FALSE);
  }

  if (!m_pModule)
    m_BitmapDevice.EnableWindow(FALSE);

  return TRUE;  // return TRUE unless you set the focus to a control
                // EXCEPTION: OCX Property Pages should return FALSE
}

// From OdaLayerPropManagerDlg.cpp
bool hittestSubitem(CListCtrl& listLayersList, int columns_count, const CPoint& pt, int &index, int &labelIndex);

void CVectPropsDlg::OnClickListPropsList(NMHDR* /*pNMHDR*/, LRESULT* pResult) 
{
  if (!m_pModule)
    return; // Do not edit bitmap device properties

  DWORD pos = GetMessagePos();

  CPoint pt(GET_X_LPARAM(pos), GET_Y_LPARAM(pos));
  m_PropsList.ScreenToClient(&pt);

  int index, labelIndex;
  if (hittestSubitem(m_PropsList, 3, pt, index, labelIndex) /*&& (labelIndex == 2)*/)
  {
    OdString dataType(m_PropsList.GetItemText(index, 1));
    OdVariant::Type varType = StringToRxVarType(dataType);
    if (varType == OdVariant::kVoid)
    {
      MessageBox(_T("This type of data isn't editable."), OD_T("Value Editor"), MB_ICONWARNING);
    }
    else
    {
      OdString varName(m_PropsList.GetItemText(index, 0));
      OdRxDictionaryPtr pProps = m_pDevice->properties();
      int nItem = 0;
      OdRxDictionaryIteratorPtr pIt = pProps->newIterator();
      while (!pIt->done())
      {
        if (nItem == index)
        {
          bool bUnchanged = false;
          OdRxVariantValue vVal(pIt->object());
          switch (varType)
          {
            case OdVariant::kString:
              {
                OdString val(vVal);
                OdString prevVal(val);
                if (CSimpleInputDlg().DoInput(_T("Input Property Value"), _T("Enter new text string:"), &val))
                {
                  if (val != prevVal)
                  {
                    pProps->putAt(varName, OdRxVariantValue(val));
                    if ((const OdString&)OdRxVariantValue(pProps->getAt(varName)) == prevVal)
                      bUnchanged = true;
                  }
                }
              }
            break;
            case OdVariant::kUInt8:
              {
                OdUInt8 val(vVal);
                OdUInt8 prevVal(val);
                OdUInt32 nVal(val);
                if (CSimpleInputDlg().DoInput(_T("Input Property Value"), _T("Enter new unsigned integer value:"), &nVal, 0, 0xFF))
                {
                  if (nVal != prevVal)
                  {
                    pProps->putAt(varName, OdRxVariantValue((OdUInt8)nVal));
                    if ((OdUInt8)OdRxVariantValue(pProps->getAt(varName)) == prevVal)
                      bUnchanged = true;
                  }
                }
              }
            break;
            case OdVariant::kUInt16:
              {
                OdUInt16 val(vVal);
                OdUInt16 prevVal(val);
                OdUInt32 nVal(val);
                if (CSimpleInputDlg().DoInput(_T("Input Property Value"), _T("Enter new unsigned integer value:"), &nVal, 0, 0xFFFF))
                {
                  if (nVal != prevVal)
                  {
                    pProps->putAt(varName, OdRxVariantValue((OdUInt16)nVal));
                    if ((OdUInt16)OdRxVariantValue(pProps->getAt(varName)) == prevVal)
                      bUnchanged = true;
                  }
                }
              }
            break;
            case OdVariant::kUInt32:
              {
                OdUInt32 val(vVal);
                OdUInt32 prevVal(val);
                OdUInt32 nVal(val);
                if (CSimpleInputDlg().DoInput(_T("Input Property Value"), _T("Enter new unsigned integer value:"), &nVal))
                {
                  if (nVal != prevVal)
                  {
                    pProps->putAt(varName, OdRxVariantValue(nVal));
                    if ((OdUInt32)OdRxVariantValue(pProps->getAt(varName)) == prevVal)
                      bUnchanged = true;
                  }
                }
              }
            break;
            case OdVariant::kUInt64:
              {
                OdUInt64 val(vVal);
                OdUInt64 prevVal(val);
                OdUInt32 nVal(val);
                if (CSimpleInputDlg().DoInput(_T("Input Property Value"), _T("Enter new unsigned integer value:"), &nVal, 0, 0xFFFF))
                {
                  if (nVal != prevVal)
                  {
                    pProps->putAt(varName, OdRxVariantValue((OdUInt64)nVal));
                    if ((OdUInt64)OdRxVariantValue(pProps->getAt(varName)) == prevVal)
                      bUnchanged = true;
                  }
                }
              }
            break;
            case OdVariant::kDouble:
              {
                double val(vVal);
                double prevVal(val);
                double nVal(val);
                if (CSimpleInputDlg().DoInput(_T("Input Property Value"), _T("Enter new floating point value:"), &nVal))
                {
                  if (nVal != prevVal)
                  {
                    pProps->putAt(varName, OdRxVariantValue(nVal));
                    if ((double)OdRxVariantValue(pProps->getAt(varName)) == prevVal)
                      bUnchanged = true;
                  }
                }
              }
            break;
            case OdVariant::kBool:
              {
                bool val(vVal);
                bool prevVal(val);
                bool nVal(val);
                if (CSimpleInputDlg().DoInput(_T("Input Property Value"), _T("Enter new boolean value:"), &nVal))
                {
                  if (nVal != prevVal)
                  {
                    pProps->putAt(varName, OdRxVariantValue(nVal));
                    if ((bool)OdRxVariantValue(pProps->getAt(varName)) == prevVal)
                      bUnchanged = true;
                  }
                }
              }
            break;
          }
          if (bUnchanged)
            MessageBox(_T("Value doesn't changed.\nLooks like this is read only property."), _T("Read Only Property"), MB_ICONINFORMATION);
          else
          {
            OdRxVariantValue vVal(pProps->getAt(varName));
            OdString type, data;
            RxVarToString(vVal, type, data);
            m_PropsList.SetItemText(nItem, 1, type.c_str());
            m_PropsList.SetItemText(nItem, 2, data.c_str());
          }
          break;
        }
        nItem++;
        pIt->next();
      }
    }
  }

  *pResult = 0;
}

void CVectPropsDlg::OnBitmapDeviceButtonClicked()
{
  OdGsDevicePtr pDevice = m_pModule->createBitmapDevice();
  if (!pDevice.isNull())
  {
    CVectPropsDlg vPropsDisp(pDevice);
    vPropsDisp.DoModal();
  }
  else
    MessageBox(_T("Can't create bitmap device."), _T("OdaMfcApp Error"), MB_ICONERROR);
}

//
