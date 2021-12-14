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
// SHelperPage3.cpp : implementation file
//

#include "stdafx.h"
#include "OdaDgnApp.h"
#include "SHelperPage3.h"
#include "DgModelerGeometry.h"
#include "DgModelerGeometryCreator.h"
#include "MemoryStream.h"
#include "DgCellHeader.h"
#include "DgBRepEntityPE.h"

// CSHelperPage3 dialog

IMPLEMENT_DYNAMIC(CSHelperPage3, CPropertyPage)

CSHelperPage3::PSComboVers CSHelperPage3::m_ComboFill[] = 
  {
  "v12 text", OdDgModelerGeometry::kXT|OdDgModelerGeometry::kPS_12_0,
  "v12 neutral bin", OdDgModelerGeometry::kXB_Neutral|OdDgModelerGeometry::kPS_12_0,
  "v12 bare bin", OdDgModelerGeometry::kXB_Bare|OdDgModelerGeometry::kPS_12_0,
  "v12 typedBE bin", OdDgModelerGeometry::kXB_Typed0|OdDgModelerGeometry::kPS_12_0,
  "v12 typedLE bin", OdDgModelerGeometry::kXB_Typed1|OdDgModelerGeometry::kPS_12_0,
  "v9 text", OdDgModelerGeometry::kXT|OdDgModelerGeometry::kPS_9_0,
  "v9 bin", OdDgModelerGeometry::kXB_Neutral|OdDgModelerGeometry::kPS_9_0};

    CSHelperPage3::PSComboVers CSHelperPage3::m_ComboFillSat[] = 
    {
      "v1.5 sab", OdDgModelerGeometry::kSab|OdDgModelerGeometry::kAS_1_5,
      "v1.6 sab", OdDgModelerGeometry::kSab|OdDgModelerGeometry::kAS_1_6,
      "v1.7 sab", OdDgModelerGeometry::kSab|OdDgModelerGeometry::kAS_1_7,
      "v2.1 sab", OdDgModelerGeometry::kSab|OdDgModelerGeometry::kAS_2_1,
      "v3.0 sab", OdDgModelerGeometry::kSab|OdDgModelerGeometry::kAS_3_0,
      "v4.0 sab", OdDgModelerGeometry::kSab|OdDgModelerGeometry::kAS_4_0,
      "v5.0 sab", OdDgModelerGeometry::kSab|OdDgModelerGeometry::kAS_5_0,
      "v6.0 sab", OdDgModelerGeometry::kSab|OdDgModelerGeometry::kAS_6_0,
      "v7.0 sab", OdDgModelerGeometry::kSab|OdDgModelerGeometry::kAS_7_0,
      "v8.0 sab", OdDgModelerGeometry::kSab|OdDgModelerGeometry::kAS_8_0,
      "v1.5 sat", OdDgModelerGeometry::kSat|OdDgModelerGeometry::kAS_1_5,
      "v1.6 sat", OdDgModelerGeometry::kSat|OdDgModelerGeometry::kAS_1_6,
      "v1.7 sat", OdDgModelerGeometry::kSat|OdDgModelerGeometry::kAS_1_7,
      "v2.1 sat", OdDgModelerGeometry::kSat|OdDgModelerGeometry::kAS_2_1,
      "v3.0 sat", OdDgModelerGeometry::kSat|OdDgModelerGeometry::kAS_3_0,
      "v4.0 sat", OdDgModelerGeometry::kSat|OdDgModelerGeometry::kAS_4_0,
      "v5.0 sat", OdDgModelerGeometry::kSat|OdDgModelerGeometry::kAS_5_0,
      "v6.0 sat", OdDgModelerGeometry::kSat|OdDgModelerGeometry::kAS_6_0,
      "v7.0 sat", OdDgModelerGeometry::kSat|OdDgModelerGeometry::kAS_7_0,
      "v8.0 sat", OdDgModelerGeometry::kSat|OdDgModelerGeometry::kAS_8_0};

CSHelperPage3::CSHelperPage3()
	: CPropertyPage(CSHelperPage3::IDD)
  , m_bDebugMode(FALSE)
  , m_bSat(FALSE)
{

}

CSHelperPage3::~CSHelperPage3()
{
}

void CSHelperPage3::DoDataExchange(CDataExchange* pDX)
{
  CPropertyPage::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_EDIT_STREAM, m_StreamEdit);
  DDX_Control(pDX, IDC_COMBO1, m_Vers);
  DDX_Check(pDX, IDC_CHECK_DEBUG, m_bDebugMode);
}

BEGIN_MESSAGE_MAP(CSHelperPage3, CPropertyPage)
  ON_BN_CLICKED(IDC_BUTTON_SAVE, OnBnClickedButtonSave)
  ON_BN_CLICKED(IDC_BUTTON_ORIGINAL, OnBnClickedButtonOriginal)
  ON_CBN_SELCHANGE(IDC_COMBO1, OnSelchangeVer)
  ON_BN_CLICKED(IDC_BUTTON3, CSHelperPage3::OnBnClickedButton3)
  ON_BN_CLICKED(IDC_BUTTON_SAVE2, CSHelperPage3::OnBnClickedButtonSave2)
  ON_BN_CLICKED(IDC_BUTTON_COMPARE, CSHelperPage3::OnBnClickedButtonCompare)
END_MESSAGE_MAP()


OdDgModelerGeometry::OdDgModelerVersion CSHelperPage3::getCurrVer() const
{
  int i  = m_Vers.GetCurSel();
  if (i != -1)
    return m_bSat ? m_ComboFillSat[i].ver : m_ComboFill[i].ver;
  return OdDgModelerGeometry::kUndefined;
}

OdDgModelerGeometryPtr CSHelperPage3::getModeler() const
{
  // Load modeler module
  OdRxModulePtr pModule = ::odrxDynamicLinker()->loadModule("TG_ModelerGeometry");
  OdRxClassPtr pService( odrxServiceDictionary()->getAt(OD_T("OdDgModelerGeometryCreator")) );
  if (!pModule.isNull() && !pService.isNull())
  {
    OdDgModelerGeometryCreatorPtr pCreator = pService->create();
    if (!pCreator.isNull())
    {
      m_data->rewind();
      OdArray<OdDgModelerGeometryPtr> models;
      pCreator->createModeler(models, m_data);

      if (!models.isEmpty())
        return models.first();
    }
  }
  return OdDgModelerGeometryPtr();
}

OdDgModelerGeometry::OdDgModelerVersion CSHelperPage3::getDataVer() const
{
  OdDgModelerGeometry::OdDgModelerVersion version = OdDgModelerGeometry::kUndefined;

  OdDgModelerGeometryPtr pM = getModeler();
  if (!pM.isNull())
    pM->comeFromVersion(version);

  return version;
}

// CSHelperPage3 message handlers

void CSHelperPage3::OnBnClickedButtonOriginal()
{
  m_data->rewind();
  fill(*m_data);
}

void CSHelperPage3::OnBnClickedButtonSave()
{
  // TODO: Add your control notification handler code here
  if (m_data->length() != 0)
  {
    OdUInt32 version = getDataVer()&OdDgModelerGeometry::kTypeMask;

    CString ext;
    if ( version==OdDgModelerGeometry::kSat || version==OdDgModelerGeometry::kSatIndexed)
      ext = _T("sat");
    else if (version==OdDgModelerGeometry::kSab || version==OdDgModelerGeometry::kSabIndexed)
      ext = _T("sab");
    else if (version==OdDgModelerGeometry::kXT)
      ext = _T("x_t");
    else 
      ext = _T("x_b");

    CString ext2;
    ext2.Format(_T("%s files (*.%s)|*.%s|All Files (*.*)|*.*||"), ext, ext, ext);

    CFileDialog saveFile(false, ext, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, ext2, this);
    if (saveFile.DoModal() == IDOK)
    {
      SaveOriginalTo(saveFile.GetPathName());
    }
  }
}

void CSHelperPage3::OnSelchangeVer() 
{
  UpdateEditBox();
}

 //#define ENABLE_DEBUG_FOR_PARASOLID
 
 // for debug parasolid library only
 #ifndef _DEBUG
 #undef ENABLE_DEBUG_FOR_PARASOLID
 #endif
 
 #ifdef ENABLE_DEBUG_FOR_PARASOLID
 extern void enableDebugModeForPSToolkit(bool b);
 #endif // _DEBUG

void CSHelperPage3::fill(OdStreamBuf &data)
{
  OdUInt32 len = (OdUInt32)data.length();

  OdStreamBufPtr tmpBuf = OdMemoryStream::createNew();
  data.copyDataTo(tmpBuf);

  m_data = tmpBuf;

  CString tmp;
  LPTSTR pBuf = tmp.GetBufferSetLength(len+1);
  unsigned int i = 0;

  m_data->rewind();
#ifdef _UNICODE
  for(OdUInt32 f=0; f<len; ++f)
    pBuf[f] = m_data->getByte();
  pBuf[len] = '\0';
#else
  m_data->getBytes(pBuf, len)
  pBuf[len] = '\0';
#endif

  for(unsigned j = 0; j < len; j++)
    if (pBuf[j] == 0) pBuf[j] = ' ';

  m_StreamEdit.SetWindowText(tmp);

  CWnd *pW = GetDlgItem(IDC_BUTTON_SAVE);
  if (pW) pW->EnableWindow(TRUE);
  pW = GetDlgItem(IDC_BUTTON_SAVE2);
  if (pW) pW->EnableWindow(TRUE);

#ifdef ENABLE_DEBUG_FOR_PARASOLID
  pW = GetDlgItem(IDC_CHECK_DEBUG);
  if (pW) {pW->ShowWindow(TRUE); pW->EnableWindow(TRUE);}
#endif // _DEBUG

  OdDgModelerGeometry::OdDgModelerVersion currVer = getDataVer();
  m_bSat = currVer & OdDgModelerGeometry::kAcisMask;

  m_Vers.ResetContent();
  int v = -1;

  if (m_bSat)
  {
    for(i = 0; i < sizeof(m_ComboFillSat)/sizeof(m_ComboFillSat[0]); i++)
    {
      m_Vers.AddString(CString(m_ComboFillSat[i].pText));
      if (currVer == m_ComboFillSat[i].ver)
        v = i;
    }
  }
  else
  {
    for(i = 0; i < sizeof(m_ComboFill)/sizeof(m_ComboFill[0]); i++)
    {
      m_Vers.AddString(CString(m_ComboFill[i].pText));
      if (currVer == m_ComboFill[i].ver)
        v = i;
    }
  }
  ODA_ASSERT_ONCE(v != -1);

  m_Vers.SetCurSel(v);
}

void CSHelperPage3::UpdateEditBox()
{
  UpdateData();
  m_StreamEdit.Clear();

  OdDgModelerGeometry::OdDgModelerVersion v = getCurrVer();
  OdDgModelerGeometryPtr pM = getModeler();
  if (!pM.isNull())
  {
    OdStreamBufPtr pOut = OdMemoryStream::createNew();
    #ifdef ENABLE_DEBUG_FOR_PARASOLID
      enableDebugModeForPSToolkit(m_bDebugMode == TRUE); 
      OdResult res = pM->out(pOut, v);
      enableDebugModeForPSToolkit(FALSE);
    #else
      OdResult res = pM->out(pOut, v);
    #endif

    pOut->rewind();

    if (res != eOk)
    {
      pOut->truncate();
      OdAnsiString tmp = OdError(res).description();
      pOut->putBytes(tmp.c_str(), tmp.getLength());
      pOut->rewind();
    }

    OdUInt32 len = (OdUInt32)pOut->length();
    CString tmp;
    LPTSTR pBuf = tmp.GetBufferSetLength(len+1);
#ifdef _UNICODE
    for(unsigned i = 0; i < len; i++)
      pBuf[i] = pOut->getByte();
    pBuf[len] = '\0';
#else
    pOut->getBytes((void *)pBuf, len);
    pBuf[len] = '\0';
#endif

    for(unsigned j = 0; j < len; j++)
      if (pBuf[j] == 0) pBuf[j] = ' ';

    m_StreamEdit.SetWindowText(tmp);
    UpdateData(false);
  }
}

void CSHelperPage3::OnBnClickedButton3()
{
  CFileDialog loadFile(true, _T("x_t"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("x_t files (*.x_t)|*.x_t|x_b files (*.x_b)|*.x_b||"), this);
  if (loadFile.DoModal() == IDOK)
  {
    OdStreamBufPtr pFile = odrxSystemServices()->createFile(LPCTSTR(loadFile.GetPathName()), Oda::kFileRead);
    fill(*pFile);
  }
}

void CSHelperPage3::SaveOriginalTo(CString path)
{
  OdStreamBufPtr pFile = odrxSystemServices()->createFile(LPCTSTR(path), Oda::kFileWrite,Oda::kShareDenyNo, Oda::kCreateAlways);
  m_data->rewind();
  m_data->copyDataTo(pFile);
}

void CSHelperPage3::SaveModifiedTo(CString path)
{
  OdDgModelerGeometry::OdDgModelerVersion v = getCurrVer();
  OdDgModelerGeometryPtr pM = getModeler();
  if (!pM.isNull())
  {
    OdStreamBufPtr pOut = odrxSystemServices()->createFile(OdString(path), Oda::kFileWrite, Oda::kShareDenyNo, Oda::kCreateAlways);
#ifdef ENABLE_DEBUG_FOR_PARASOLID
    enableDebugModeForPSToolkit(m_bDebugMode == TRUE); 
    OdResult res = pM->out(pOut, v);
    enableDebugModeForPSToolkit(FALSE);
#else
    OdResult res = pM->out(pOut, v);
#endif

    pOut->rewind();

    if (res != eOk)
    {
      pOut->truncate();
      OdAnsiString tmp = OdError(res).description();
      pOut->putBytes(tmp.c_str(), tmp.getLength());
      pOut->rewind();
    }
  }
}

void CSHelperPage3::OnBnClickedButtonSave2()
{
  if (m_data->length() != 0)
  {
    OdUInt32 version = getCurrVer()&OdDgModelerGeometry::kTypeMask;

    CString ext;
    if ( version==OdDgModelerGeometry::kSat || version==OdDgModelerGeometry::kSatIndexed)
      ext = _T("sat");
    else if (version==OdDgModelerGeometry::kSab || version==OdDgModelerGeometry::kSabIndexed)
      ext = _T("sab");
    else if (version==OdDgModelerGeometry::kXT)
      ext = _T("x_t");
    else 
      ext = _T("x_b");

    CString ext2;
    ext2.Format(_T("%s files (*.%s)|*.%s|All Files (*.*)|*.*||"), ext, ext, ext);

    CFileDialog saveFile(false, ext, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, ext2, this);
    if (saveFile.DoModal() == IDOK)
    {
      SaveModifiedTo(saveFile.GetPathName());
    }
  }
}

void CSHelperPage3::OnBnClickedButtonCompare()
{
  // TODO: Add your control notification handler code here

#ifdef ENABLE_DEBUG_FOR_PARASOLID
  if (getCurrVer()&OdDgModelerGeometry::kXB_Neutral)
  {
    CStringA toolPath = ("C:\\Program Files\\HexCmp\\HexCmp.exe");
    CStringA Path1 = ("e:\\tmp_org.x_b");
    CStringA Path2 = ("e:\\tmp_mod.x_b");

    SaveOriginalTo(CString(Path1));
    SaveModifiedTo(CString(Path2));

    CStringA tmp;
    tmp.Format("%s %s %s", toolPath, Path1, Path2);
    WinExec(tmp, 0);

  }
  else
  {
    CStringA toolPath = ("C:\\Program Files\\Microsoft Visual Studio\\Common\\Tools\\WINDIFF.EXE");
    CStringA Path1 = ("e:\\tmp_org.x_t");
    CStringA Path2 = ("e:\\tmp_mod.x_t");

    SaveOriginalTo(CString(Path1));
    SaveModifiedTo(CString(Path2));

    CStringA tmp;
    tmp.Format("%s %s %s", toolPath, Path1, Path2);
    WinExec(tmp, 0);

  }
#endif
}
