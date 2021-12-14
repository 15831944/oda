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

// AcisSaveDialog.cpp : implementation file
//

#include "stdafx.h"
#include "odamfcapp.h"
#include "ModelerGeometryCreator.h"
#include "AcisSaveDialog.h"
#include "DbRegion.h"
#include "DbBody.h"
#include "Db3dSolid.h"
#include "DbProxyEntity.h"
#include "MemoryStream.h"
#include "GeometryFromProxy.h"
#include "OdFileBuf.h"
#include "DbBlockTableRecord.h"
#include "AsSurfBody.h"
#include "DbSurface.h"
#include "BBValidationErrDlg.h"
#include "Br/BrBrep.h"
#include "BrepBuilder/BrepBuilder.h"
#include "BrepBuilderFillerModule.h"
#ifdef ACIS_CHECKING
#include "ModelerTestSpa/ModelerTestSpa.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAcisSaveDialog dialog

TOOLKIT_EXPORT OdRxClassPtr odrxGetModelerGeometryCreatorService();

CAcisSaveDialog::CAcisSaveDialog(const OdDbObjectId &id, CWnd* pParent /*=NULL*/)
	: CDialog(CAcisSaveDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAcisSaveDialog)
	m_ACIS_base_ver = _T("");
	m_bAddIndexes = TRUE;
	m_bStandardSave = TRUE;
	//}}AFX_DATA_INIT

  m_bReadOnly = false;

  ::odrxDynamicLinker()->loadModule(OdModelerCommandsModuleName);
  OdRxClass* pAsSurfBodyDesc = (OdRxClass*)::odrxClassDictionary()->getAt(OD_T("AcAsSurfBody")).get();

  m_id = id;
  OdDbObjectPtr pObject = id.openObject(OdDb::kForWrite);
  OdRxClassPtr pService = ::odrxGetModelerGeometryCreatorService();
  OdModelerGeometryCreatorPtr pCreator;
  if (pObject.get() && pService.get() && (pCreator = pService->create()).get())
  {
    if (OdDbProxyEntity::cast(pObject).get())
    {
      OdDbProxyEntityPtr pProxy(pObject);
      OdString className = pProxy->originalClassName();
      if (className == _T("AcAdPart"))
      {
        m_pObject = pObject;

        OdModelerGeometryPtr pModelerGeometry;
        if (odGetSatFromProxy(pProxy, pModelerGeometry))
          m_Bodies.push_back(pModelerGeometry);

        m_bReadOnly = true;
      }
    }
    else if ( pAsSurfBodyDesc && pObject->isKindOf( pAsSurfBodyDesc ) )
    {
        m_pObject = pObject;

        OdAsSurfBody* pAsSurf = (OdAsSurfBody*)pObject.get();

        OdStreamBufPtr pOut = OdMemoryStream::createNew();
        pAsSurf->acisOut(pOut, kAfTypeASCII|kAfTypeIndexed|kAfVerAny);
        pOut->rewind();
        pCreator->createModeler(m_Bodies, pOut);

        m_bReadOnly = true;
    }
    else
    {
      OdStreamBufPtr pOut = OdMemoryStream::createNew();
      if (OdDbRegion::cast(pObject).get())
        OdDbRegion::cast(pObject)->acisOut(pOut, kAfTypeASCII|kAfTypeIndexed|kAfVerAny);
      else if (OdDbBody::cast(pObject).get())
        OdDbBody::cast(pObject)->acisOut(pOut, kAfTypeASCII|kAfTypeIndexed|kAfVerAny);
      else if (OdDb3dSolid::cast(pObject).get())
        OdDb3dSolid::cast(pObject)->acisOut(pOut, kAfTypeASCII|kAfTypeIndexed|kAfVerAny);
      else if (OdDbSurface::cast(pObject).get())
        OdDbSurface::cast(pObject)->acisOut(pOut, kAfTypeASCII|kAfTypeIndexed|kAfVerAny);
      else
        pObject = NULL;
      if (!pObject.isNull())
      {
        m_pObject = pObject;
        pOut->rewind();
        pCreator->createModeler(m_Bodies, pOut);
      }
    }
  }
}

void CAcisSaveDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAcisSaveDialog)
	DDX_Control(pDX, IDC_BODY_LIST, m_BodyList);
	DDX_Control(pDX, IDC_EDIT_ACIS, m_AcisEditCtrl);
	DDX_Control(pDX, IDC_ACIS_VER, m_AcisVerCombo);
	DDX_Text(pDX, IDC_BASE_ACIS_VER, m_ACIS_base_ver);
	DDX_Check(pDX, IDC_CHECK_INDEXES, m_bAddIndexes);
	DDX_Check(pDX, IDC_CHECK_STANDARD_SAVE, m_bStandardSave);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAcisSaveDialog, CDialog)
	//{{AFX_MSG_MAP(CAcisSaveDialog)
	ON_BN_CLICKED(IDCOPY, OnCopy)
	ON_BN_CLICKED(IDC_CHECK_INDEXES, OnCheckIndexes)
	ON_BN_CLICKED(IDC_CHECK_STANDARD_SAVE, OnCheckStandardSave)
	ON_CBN_SELCHANGE(IDC_ACIS_VER, OnSelchangeAcisVer)
	ON_BN_CLICKED(IDSAVE, OnSave)
  ON_BN_CLICKED(IDSAVESAB, OnSaveSab)
	ON_BN_CLICKED(IDLOADFROM, OnLoadfrom)
	ON_BN_CLICKED(IDCREATENEW, OnCreatenew)
	ON_BN_CLICKED(IDSETTOENTITY, OnSettoentity)
	ON_LBN_SELCHANGE(IDC_BODY_LIST, OnSelchangeBodyList)
	ON_BN_CLICKED(IDCREATENEWALL, OnCreatenewall)
  ON_BN_CLICKED(IDBBCHECK, OnBBCheck)
  ON_BN_CLICKED(IDACISCHECK, OnAcisCheck)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAcisSaveDialog message handlers

void CAcisSaveDialog::OnCopy() 
{
	// TODO: Add your control notification handler code here
  m_AcisEditCtrl.SetSel(0, -1, true);
	m_AcisEditCtrl.Copy();
  m_AcisEditCtrl.SetSel(0, 1, true);
}

BOOL CAcisSaveDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
  if (!m_pObject.isNull())
  {
    CWnd *pW = GetDlgItem(IDSETTOENTITY);
    if (pW) pW->EnableWindow(!m_bReadOnly);
    pW = GetDlgItem(IDBBCHECK);
    if (pW) pW->EnableWindow(TRUE);
#ifdef ACIS_CHECKING
    pW = GetDlgItem(IDACISCHECK);
    if (pW) pW->ShowWindow(SW_SHOW);
    if (pW) pW->EnableWindow(TRUE);
#endif
    UpdateBodyList();
    UpdateData();
      InitBaseVersion();
    UpdateData(false);

    UpdateEditBox();
  }
  else
  {
    CWnd *pW = GetDlgItem(IDSETTOENTITY);
    if (pW) pW->EnableWindow(FALSE);
    pW = GetDlgItem(IDC_ACIS_VER);
    if (pW) pW->EnableWindow(FALSE);
    pW = GetDlgItem(IDC_CHECK_INDEXES);
    if (pW) pW->EnableWindow(FALSE);
    pW = GetDlgItem(IDC_CHECK_STANDARD_SAVE);
    if (pW) pW->EnableWindow(FALSE);
    pW = GetDlgItem(IDC_EDIT_ACIS);
    if (pW) pW->EnableWindow(FALSE);
    pW = GetDlgItem(IDCREATENEW);
    if (pW) pW->EnableWindow(FALSE);
    pW = GetDlgItem(IDCREATENEWALL);
    if (pW) pW->EnableWindow(FALSE);
    pW = GetDlgItem(IDSAVE);
    if (pW) pW->EnableWindow(FALSE);
    pW = GetDlgItem(IDSAVESAB);
    if (pW) pW->EnableWindow(FALSE);
    pW = GetDlgItem(IDCOPY);
    if (pW) pW->EnableWindow(FALSE);
    pW = GetDlgItem(IDBBCHECK);
    if (pW) pW->EnableWindow(FALSE);
#ifdef ACIS_CHECKING
    pW = GetDlgItem(IDACISCHECK);
    if (pW) pW->ShowWindow(SW_SHOW);
    if (pW) pW->EnableWindow(FALSE);
#endif
  }

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAcisSaveDialog::InitBaseVersion()
{
  if (selectedBody().isNull())
    return;

  AfTypeVer typeVer;
  selectedBody()->in(NULL, &typeVer);

  switch(typeVer&kAfVerMask)
  {
    case kAfVer105:   m_ACIS_base_ver = _T("kAfVer105");   break;
    case kAfVer106:   m_ACIS_base_ver = _T("kAfVer106");   break;
    case kAfVer107:   m_ACIS_base_ver = _T("kAfVer107");   break;
    case kAfVer200:   m_ACIS_base_ver = _T("kAfVer200");   break;
    case kAfVer201:   m_ACIS_base_ver = _T("kAfVer201");   break;
    case kAfVer400:   m_ACIS_base_ver = _T("kAfVer400");   break;
    case kAfVer500:   m_ACIS_base_ver = _T("kAfVer500");   break;
    case kAfVer600:   m_ACIS_base_ver = _T("kAfVer600");   break;
    case kAfVer700:   m_ACIS_base_ver = _T("kAfVer700");   break;
    case kAfVer1000:  m_ACIS_base_ver = _T("kAfVer1000");  break;
    case kAfVer1100:  m_ACIS_base_ver = _T("kAfVer1100");  break;
    case kAfVer20800: m_ACIS_base_ver = _T("kAfVer20800"); break;
    case kAfVer21200: m_ACIS_base_ver = _T("kAfVer21200"); break;
    case kAfVer21500: m_ACIS_base_ver = _T("kAfVer21500"); break;
    case kAfVer21600: m_ACIS_base_ver = _T("kAfVer21600"); break;
    case kAfVer21700: m_ACIS_base_ver = _T("kAfVer21700"); break;
    case kAfVer21800: m_ACIS_base_ver = _T("kAfVer21800"); break;
    case kAfVer22300: m_ACIS_base_ver = _T("kAfVer22300"); break;
  }

  m_AcisVerCombo.SelectString(0, m_ACIS_base_ver);
}

AfTypeVer CAcisSaveDialog::getSelectedByUserVersion(bool bSab) const
{
  AfTypeVer typeVer(bSab ? kAfTypeBinary : (m_bAddIndexes ? kAfTypeIndexed|kAfTypeASCII : kAfTypeASCII));
  switch (m_AcisVerCombo.GetCurSel())
  {
    case 0: typeVer |= kAfVer105;   break;
    case 1: typeVer |= kAfVer106;   break;
    case 2: typeVer |= kAfVer107;   break;
    case 3: typeVer |= kAfVer200;   break;
    case 4: typeVer |= kAfVer201;   break;
    case 5: typeVer |= kAfVer400;   break;
    case 6: typeVer |= kAfVer500;   break;
    case 7: typeVer |= kAfVer700;   break;
    case 8: typeVer |= kAfVer20800; break;
    case 9: typeVer |= kAfVer21200; break;
    case 10: typeVer |= kAfVer21500; break;
    case 11: typeVer |= kAfVer21600; break;
    case 12: typeVer |= kAfVer21700; break;
    case 13: typeVer |= kAfVer21800; break;
    case 14: typeVer |= kAfVer22300; break;
  };
  return typeVer;
}

void CAcisSaveDialog::UpdateEditBox()
{
  if (selectedBody().isNull())
    return;

  UpdateData();
    OdStreamBufPtr pOut = OdMemoryStream::createNew();

    OdResult res = selectedBody()->out(pOut, getSelectedByUserVersion(), m_bStandardSave == TRUE);
    pOut->rewind();
    ODA_TRACE0("OUT\n");
    if (res != eOk)
    {
      ODA_TRACE0("  Error\n");
      pOut->truncate();
      OdAnsiString tmp = OdError(res).description();
      pOut->putBytes(tmp.c_str(), tmp.getLength());
      pOut->rewind();
    }

    OdUInt32 len = (OdUInt32)pOut->length();
    CStringA tmp;
    LPSTR pBuf = tmp.GetBufferSetLength(len+1);
#ifdef _UNICODE
    for(unsigned i = 0; i < len; i++)
      pBuf[i] = pOut->getByte();
    pBuf[len] = '\0';
    USES_CONVERSION;
    m_AcisEditCtrl.SetWindowText(A2W(tmp));
#else
    pOut->getBytes((void *)pBuf, len);
    pBuf[len] = '\0';
    m_AcisEditCtrl.SetWindowText(tmp);
#endif
   
  UpdateData(false);
}

void CAcisSaveDialog::OnCheckIndexes() 
{
  UpdateEditBox();
}

void CAcisSaveDialog::OnCheckStandardSave() 
{
  UpdateEditBox();
}

void CAcisSaveDialog::OnSelchangeAcisVer() 
{
  UpdateEditBox();
}

void CAcisSaveDialog::OnSave() 
{
	CFileDialog saveFile(false, _T("sat"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("Sat files (*.sat)|*.sat|All Files (*.*)|*.*||"), this);
  if (saveFile.DoModal() == IDOK)
  {
    CStdioFile file(saveFile.GetPathName(), CFile::modeCreate|CFile::modeWrite|CFile::typeText);
    CString rString;
    m_AcisEditCtrl.GetWindowText(rString);
    rString.Replace(_T("\r\n"),_T("\n"));
    file.WriteString(rString);
  }
}

void CAcisSaveDialog::OnSaveSab() 
{
  CFileDialog saveFile(false, _T("sab"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("Sab files (*.sab)|*.sab|All Files (*.*)|*.*||"), this);
  if (saveFile.DoModal() == IDOK)
  {
    OdWrFileBufPtr pOut = OdWrFileBuf::createObject((LPCTSTR)saveFile.GetPathName());
    selectedBody()->out(pOut, getSelectedByUserVersion(true), m_bStandardSave == TRUE);
  }
}

void CAcisSaveDialog::OnLoadfrom() 
{
	CFileDialog loadFile(true, _T("sat"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("Sat files (*.sat)|*.sat|Sab files (*.sab)|*.sab|All Files (*.*)|*.*||"), this);
  if (loadFile.DoModal() == IDOK)
  {
    m_Bodies.clear();
    UpdateData();
      OdRdFileBuf inFile(OdString((LPCTSTR)loadFile.GetPathName()));
      OdRxClassPtr pService = ::odrxGetModelerGeometryCreatorService();
      OdModelerGeometryCreatorPtr pCreator;
      if (pService.get() && (pCreator = pService->create()).get())
      {
        pCreator->createModeler(m_Bodies, &inFile, NULL, m_bStandardSave == TRUE, m_id.database()->appServices()->getEnableAcisAudit());
        inFile.rewind();
      }
      UpdateBodyList();
      InitBaseVersion();
    UpdateData(false);

    UpdateEditBox();

    if (m_pObject.isNull())
    {
      CWnd *pW = GetDlgItem(IDSETTOENTITY);
      if (pW) pW->EnableWindow(FALSE);
      pW = GetDlgItem(IDC_ACIS_VER);
      if (pW) pW->EnableWindow(TRUE);
      pW = GetDlgItem(IDC_CHECK_INDEXES);
      if (pW) pW->EnableWindow(TRUE);
      pW = GetDlgItem(IDC_CHECK_STANDARD_SAVE);
      if (pW) pW->EnableWindow(TRUE);
      pW = GetDlgItem(IDC_EDIT_ACIS);
      if (pW) pW->EnableWindow(TRUE);
      pW = GetDlgItem(IDCREATENEW);
      if (pW) pW->EnableWindow(TRUE);
      pW = GetDlgItem(IDCREATENEWALL);
      if (pW) pW->EnableWindow(TRUE);
      pW = GetDlgItem(IDSAVE);
      if (pW) pW->EnableWindow(TRUE);
      pW = GetDlgItem(IDSAVESAB);
      if (pW) pW->EnableWindow(TRUE);
      pW = GetDlgItem(IDCOPY);
      if (pW) pW->EnableWindow(TRUE);
    }
    if (m_Bodies.length())
    {
      CWnd *pW = GetDlgItem(IDBBCHECK);
      if (pW) pW->EnableWindow(TRUE);
      pW = GetDlgItem(IDACISCHECK);
      if (pW) pW->EnableWindow(TRUE);
    }
  }
}

void CAcisSaveDialog::OnCreatenew() 
{
	// TODO: Add your control notification handler code here
  if (selectedBody().isNull())
    return;

  UpdateData();
    OdSmartPtr<OdStreamBuf> pOut = OdMemoryStream::createNew();
    selectedBody()->out(pOut, getSelectedByUserVersion(), m_bStandardSave == TRUE);
    pOut->rewind();

    OdDbBlockTableRecordPtr pBlock;
    if (m_pObject.isNull())
    {
      pBlock = m_id.database()->getModelSpaceId().safeOpenObject(OdDb::kForWrite);
    }
    else
    {
      OdDbEntityPtr pEnt = OdDbEntity::cast(m_pObject);
      pBlock = pEnt->ownerId().safeOpenObject(OdDb::kForWrite);
    }

    createTypedEntity(selectedBody(), pBlock, pOut);
  UpdateData(false);
}

void CAcisSaveDialog::UpdateBodyList()
{
  m_BodyList.ResetContent();
  for(unsigned int f = 0; f < m_Bodies.size(); ++f)
  {
    CString str;
    OdModelerGeometry::geomType type = m_Bodies[f]->bodyType();
    switch(type)
    {
      case OdModelerGeometry::kUndefined:
        str.Format(_T("undef  %d"), f);
        break;
      case OdModelerGeometry::kSolid:
        str.Format(_T("solid  %d"), f);
        break;
      case OdModelerGeometry::kBody:
        str.Format(_T("body   %d"), f);
        break;
      case OdModelerGeometry::kRegion:
        str.Format(_T("region %d"), f);
        break;
      case OdModelerGeometry::kSurface:
        str.Format(_T("surface %d"), f);
        break;
    };

    m_BodyList.AddString(str);
  }
  if (m_Bodies.size())
    m_BodyList.SetCurSel( 0 );
}

void CAcisSaveDialog::OnSettoentity() 
{
  if (selectedBody().isNull())
    return;

  OdSmartPtr<OdStreamBuf> pOut = OdMemoryStream::createNew();
  selectedBody()->out(pOut, getSelectedByUserVersion(), m_bStandardSave == TRUE);
  pOut->rewind();

  if (OdDbRegion::cast(m_pObject).get())
    OdDbRegion::cast(m_pObject)->acisIn(pOut.get());
  else if (OdDbBody::cast(m_pObject).get())
    OdDbBody::cast(m_pObject)->acisIn(pOut.get());
  else if (OdDb3dSolid::cast(m_pObject).get())
    OdDb3dSolid::cast(m_pObject)->acisIn(pOut.get());
  else if (OdDbSurface::cast(m_pObject).get())
    OdDbSurface::cast(m_pObject)->acisIn(pOut.get());
}

OdModelerGeometryPtr CAcisSaveDialog::selectedBody()
{
  if (m_Bodies.isEmpty())
    return OdModelerGeometryPtr();

  int curSel = m_BodyList.GetCurSel();
  return (curSel ==  LB_ERR) ? OdModelerGeometryPtr() : m_Bodies[curSel];
}

void CAcisSaveDialog::OnSelchangeBodyList() 
{
  UpdateEditBox();  	
}

void CAcisSaveDialog::createTypedEntity(OdModelerGeometryPtr pModeller, OdDbBlockTableRecordPtr pBlock, OdSmartPtr<OdStreamBuf> pOut)
{
  OdModelerGeometry::geomType type = pModeller->bodyType();
  switch(type)
  {
    case OdModelerGeometry::kUndefined:
    case OdModelerGeometry::kSolid:
      {
        OdDb3dSolidPtr p3dSolid = OdDb3dSolid::createObject();
        p3dSolid->acisIn(pOut.get());
        pBlock->appendOdDbEntity(p3dSolid);
      }
      break;
    case OdModelerGeometry::kBody:
      {
        OdDbBodyPtr pBody = OdDbBody::createObject();
        pBody->acisIn(pOut.get());
        pBlock->appendOdDbEntity(pBody);
      }
      break;
    case OdModelerGeometry::kRegion:
      {
        OdDbRegionPtr pRegion = OdDbRegion::createObject();
        pRegion->acisIn(pOut.get());
        pBlock->appendOdDbEntity(pRegion);
      }
      break;
    case OdModelerGeometry::kSurface:
      {
        OdDbSurfacePtr pSurf = OdDbSurface::createObject();
        pSurf->acisIn(pOut.get());
        pBlock->appendOdDbEntity(pSurf);
      }
      break;
  };
}

void CAcisSaveDialog::OnCreatenewall() 
{
	// TODO: Add your control notification handler code here
  if (m_Bodies.isEmpty())
    return;

  UpdateData();

  OdDbBlockTableRecordPtr pBlock;
  if (m_pObject.isNull())
  {
    pBlock = m_id.database()->getModelSpaceId().safeOpenObject(OdDb::kForWrite);
  }
  else
  {
    OdDbEntityPtr pEnt = OdDbEntity::cast(m_pObject);
    pBlock = pEnt->ownerId().safeOpenObject(OdDb::kForWrite);
  }

  for(OdUInt32 f=0; f<m_Bodies.size();++f)
  {
    OdSmartPtr<OdStreamBuf> pOut = OdMemoryStream::createNew();
    m_Bodies[f]->out(pOut, getSelectedByUserVersion(), m_bStandardSave == TRUE);
    pOut->rewind();
    createTypedEntity(m_Bodies[f], pBlock, pOut);
    UpdateEditBox();
  }

  UpdateData(false);
}

void CAcisSaveDialog::OnBBCheck()
{
  // TODO: Add your control notification handler code here
  if (selectedBody().isNull())
  {
    int msgboxID = ::MessageBox(NULL, (LPCWSTR)L"Entity is NULL.\nPlease,  select entity",
      (LPCWSTR)L"No entity", MB_ICONWARNING | MB_OK);
    return;
  }
  UpdateData();

  OdBrBrep brep;
  BrepType bbType = kOpenShell;
  OdBrepBuilder bbuilder;
  OdDbBody bodyChecked;
  bodyChecked.setBody(selectedBody());
  bodyChecked.brep(brep);
  OdDbEntityPtr pEntity(&bodyChecked);

  OdDbEntityPtr pRetEntity;
  switch (selectedBody()->bodyType())
  {
    case OdModelerGeometry::kSolid:
    {
      pRetEntity = OdDb3dSolid::createObject();
      bbType = kSolid;
    }
    break;
    case OdModelerGeometry::kSurface:
      pRetEntity = OdDbSurface::createObject();
    break;
    case OdModelerGeometry::kRegion:
      pRetEntity = OdDbRegion::createObject();
    break;
    case OdModelerGeometry::kBody:
      pRetEntity = OdDbBody::createObject();
    break;
    default:
      int msgboxID = ::MessageBox(NULL, (LPCWSTR)L"Brep builder doesn't support this entity types.\nDo you want to continue?",
        (LPCWSTR)L"Unsupported entity", MB_ICONWARNING | MB_OK);
      return;
  }
  m_id.database()->appServices()->brepBuilder(bbuilder, bbType);
  try
  {
    OdBrepBuilderFiller BBFiller;
    BBFiller.params().setupFor(OdBrepBuilderFillerParams::kBrepAcisDwg, OdBrepBuilderFillerParams::kBrepAcisDwg);
    if (BBFiller.initFrom(bbuilder, brep) == eOk)
    {
      if (pRetEntity->isKindOf(OdDbRegion::desc()))
        OdDbRegion::cast(pRetEntity)->setBody(bbuilder.finish());
      else if (pRetEntity->isKindOf(OdDbBody::desc()))
        OdDbBody::cast(pRetEntity)->setBody(bbuilder.finish());
      else if (pRetEntity->isKindOf(OdDb3dSolid::desc()))
        OdDb3dSolid::cast(pRetEntity)->setBody(bbuilder.finish());
      else if (pRetEntity->isKindOf(OdDbSurface::desc()))
        OdDbSurface::cast(pRetEntity)->setBody(bbuilder.finish());
    }
    else
    {
      int msgboxID = ::MessageBox(NULL, (LPCWSTR)L"Can't get BREP data for processing (brep filler error)", (LPCWSTR)L"Error", MB_ICONERROR | MB_OK);
      return;
    }
  }
  catch (OdError &/*err*/)
  {
    const ValidationErrors *bbErr = bbuilder.getValidationErrors();
    CString strErr;
    for (ValidationErrors::size_type i = 0; i < bbErr->length(); i++)
    {
      strErr += bbErr->at(i).m_errorMessage.c_str();
    }
    strErr.Replace(L"\n", L"\r\n");
    CBBValidationErrDlg pd(strErr, NULL);
    pd.DoModal();
    UpdateData(false);
    return;
  }
  ::MessageBox(NULL, (LPCWSTR)L"Entity copied successfully", (LPCWSTR)L"Entity copied", MB_ICONINFORMATION | MB_OK);

  OdStreamBufPtr pOut = OdMemoryStream::createNew();
  if (pRetEntity->isKindOf(OdDbRegion::desc()))
    OdDbRegion::cast(pRetEntity)->acisOut(pOut, kAfTypeASCII | kAfTypeIndexed | kAfVerAny);
  else if (pRetEntity->isKindOf(OdDbBody::desc()))
    OdDbBody::cast(pRetEntity)->acisOut(pOut, kAfTypeASCII | kAfTypeIndexed | kAfVerAny);
  else if (pRetEntity->isKindOf(OdDb3dSolid::desc()))
    OdDb3dSolid::cast(pRetEntity)->acisOut(pOut, kAfTypeASCII | kAfTypeIndexed | kAfVerAny);
  else if (pRetEntity->isKindOf(OdDbSurface::desc()))
    OdDbSurface::cast(pRetEntity)->acisOut(pOut, kAfTypeASCII | kAfTypeIndexed | kAfVerAny);
  else
    return;

  OdRxClassPtr pService = ::odrxGetModelerGeometryCreatorService();
  OdModelerGeometryCreatorPtr pCreator;
  if (!pRetEntity.isNull() && pService.get() && (pCreator = pService->create()).get())
  {
    pOut->rewind();
    OdArray<OdModelerGeometryPtr> createdBodies;
    pCreator->createModeler(createdBodies, pOut);

    int curSel = m_BodyList.GetCurSel();
    m_Bodies.insertAt(curSel, createdBodies[0]);
    m_Bodies.removeAt(curSel+1);

    UpdateBodyList();
    InitBaseVersion();
    UpdateEditBox();
  }
  UpdateData(false);
}

void CAcisSaveDialog::OnAcisCheck()
{
#ifdef ACIS_CHECKING
  if (selectedBody().isNull())
    return;

  OdString strErrors;
  OdDbBody bodyChecked;
  bodyChecked.setBody(selectedBody());
  OdDbEntityPtr pEnt(&bodyChecked);
  OdModelerTestSpaWrapper pSpaModule;
  OdResult res = pSpaModule.checkDbEntity(pEnt, kAfVer700, &strErrors);
  if (res == eOk)
  {
    int msgboxID = ::MessageBox(NULL, (LPCWSTR)L"No errors were found", (LPCWSTR)L"Checking complete", MB_ICONINFORMATION | MB_OK);
  }
  else
  {
    CString strErr(strErrors.c_str());
    strErr.Replace(L"\n", L"\r\n");
    CBBValidationErrDlg pd(strErr, NULL);
    pd.DoModal();
  }
#endif
}

