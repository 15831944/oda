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
// SHelperPage1.cpp : implementation file
//

#include "stdafx.h"
#include "OdaDgnApp.h"
#include "SHelperPage1.h"

#include "Dg3DObject.h"
#include "DgCellHeader.h"
#include "CSHelperSheet.h"
#include "DgModelerGeometryCreator.h"
#include "DgBRepEntityPE.h"
#include "DgSharedCellReference.h"
#include "MemoryStream.h"

// CSHelperPage1 dialog

IMPLEMENT_DYNAMIC(CSHelperPage1, CPropertyPage)

CSHelperPage1::CSHelperPage1(/*CSHelperSheet &pSheet*/)
	: CPropertyPage(CSHelperPage1::IDD)
  //, m_pSheet(pSheet)
  , m_EnableSubfolders(FALSE)
  , m_dgn(TRUE)
  , m_xt(FALSE)
  , m_xb(FALSE)
  , m_sat(FALSE)
{
}

CSHelperPage1::~CSHelperPage1()
{
}

void CSHelperPage1::setSheet(CSHelperSheet* pSheet)
{
  m_pSheet = pSheet;
}

void CSHelperPage1::DoDataExchange(CDataExchange* pDX)
{
  CPropertyPage::DoDataExchange(pDX);
  DDX_Text(pDX, IDC_PATH, m_FolderPath);
  DDV_MaxChars(pDX, m_FolderPath, 255);
  DDX_Control(pDX, IDC_FILE_LIST, m_FileList);
  DDX_Check(pDX, IDC_CHECK_SUBFOLDERS, m_EnableSubfolders);
  DDX_Check(pDX, IDC_CHECK_DGN, m_dgn);
  DDX_Check(pDX, IDC_CHECK_X_T, m_xt);
  DDX_Check(pDX, IDC_CHECK_X_B, m_xb);
  DDX_Check(pDX, IDC_CHECK_SAT, m_sat);
}


BEGIN_MESSAGE_MAP(CSHelperPage1, CPropertyPage)
  ON_BN_CLICKED(IDC_GET_PATH, OnBnClickedGetPath)
  ON_BN_CLICKED(IDC_SEARCH, OnBnClickedSearch)
  ON_NOTIFY(NM_DBLCLK, IDC_FILE_LIST, OnNMDblclkFileList)
END_MESSAGE_MAP()


// CSHelperPage1 message handlers

void CSHelperPage1::OnBnClickedGetPath()
{
  // TODO: Add your control notification handler code here
  LPMALLOC pMalloc = NULL; 

  // Retrieve a pointer to the shell’s IMalloc interface. 
  if(::SHGetMalloc(&pMalloc) == NOERROR) 
  { 
    LPITEMIDLIST pItemIDList = NULL; 

    BROWSEINFO bi;
    bi.hwndOwner = m_hWnd;
    bi.pidlRoot = 0;

    TCHAR buf1[MAX_PATH];

    bi.pszDisplayName = buf1;        // Return display name of item selected.
    bi.lpszTitle = 0;                     // text to go in the banner over the tree.
    bi.ulFlags = 0;                       // Flags that control the return stuff
    bi.lpfn = 0;
    bi.lParam = 0;                        // extra info that's passed back in callbacks
    bi.iImage = 0;

    // Display the browse dialog box that enables the user to  
    // select a shell folder. 
    if((pItemIDList = ::SHBrowseForFolder(&bi)) != NULL) 
    { 
      // Converts an item identifier list to a file system path, 
      // this will be the path that the user has just selected. 
      if(::SHGetPathFromIDList(pItemIDList, buf1)) 
      {
        m_FolderPath = buf1;
        UpdateData(FALSE);
      }

      pMalloc->Free(pItemIDList); 
    } 
    pMalloc->Release(); 
  } 
}

BOOL CSHelperPage1::OnInitDialog()
{
  CPropertyPage::OnInitDialog();

  RECT rect;
  GetWindowRect(&rect);
  double w = rect.right - rect.left;

  m_FileList.InsertColumn(0, _T("File"), 0, int(w*0.3));
  m_FileList.InsertColumn(1, _T("Status"), 0, int(w*0.1));
  m_FileList.InsertColumn(2, _T("PS"), 0, int(w*0.1));
  m_FileList.InsertColumn(3, _T("Acis"), 0, int(w*0.1));
  m_FileList.InsertColumn(4, _T("Feature"), 0, int(w*0.1));
  m_FileList.InsertColumn(5, _T("Solid"), 0, int(w*0.1));
  m_FileList.InsertColumn(6, _T("Path"), 0, int(w-w*0.8));

  DWORD  dw = m_FileList.GetExtendedStyle();
  m_FileList.SetExtendedStyle(dw|LVS_EX_FULLROWSELECT);

  m_FolderPath = theApp.GetProfileString(_T("options"), _T("Helper path"), _T(""));
  UpdateData(FALSE);

  return TRUE;
}

bool testExt(const CString &fullPath, LPCTSTR pExt)
{
  if (fullPath.GetLength() < CString(pExt).GetLength())
    return false;
  return fullPath.Right(4).CompareNoCase(pExt) == 0;
}

#define isDGN(fullPath) testExt(fullPath, _T(".dgn"))
#define isXT(fullPath) testExt(fullPath, _T(".x_t"))
#define isXB(fullPath) testExt(fullPath, _T(".x_b"))
#define isACIS(fullPath) (testExt(fullPath, _T(".sat")) || testExt(fullPath, _T(".sab")))

void file2binaryData(CString filePath, OdStreamBufPtr &data)
{
  OdStreamBufPtr pStreamBuf = odrxSystemServices()->createFile(OdString(filePath), Oda::kFileRead);
  pStreamBuf->copyDataTo(data);
  data->rewind();
}

void CSHelperPage1::getAttributesForSATSAB(CString fullPath, FlagState &bCrash)
{
  bCrash = CSHelperPage1::kCrash;

  OdRxModulePtr pModule = ::odrxDynamicLinker()->loadModule("TG_ModelerGeometry");
  OdRxClassPtr pService( odrxServiceDictionary()->getAt(OD_T("OdDgModelerGeometryCreator")) );
  if (!pModule.isNull() && !pService.isNull())
  {
    OdDgModelerGeometryCreatorPtr pCreator = pService->create();
    if (!pCreator.isNull())
    {
      OdStreamBufPtr pStreamBuf = odrxSystemServices()->createFile(OdString(fullPath), Oda::kFileRead);

      OdArray<OdDgModelerGeometryPtr> models;
      pCreator->createModeler(models, pStreamBuf);
      if (!models.isEmpty())
      {
        bCrash = CSHelperPage1::kOk;
      }
    }
  }
}

void CSHelperPage1::getAttributesForXTXB(CString fullPath, FlagState &bCrash)
{
  getAttributesForSATSAB(fullPath, bCrash);
}

void CSHelperPage1::proceedForDgn(OdDgElementIteratorPtr pIter, FlagState &bCrash, FlagState &bXT, FlagState &bSat, FlagState &bFeature, FlagState &bSolid)
{
  for ( ; !pIter->done(); pIter->step() )
  {
    OdRxObjectPtr pObj = pIter->item().openObject();

    OdDgModelPtr pModel = OdDgModel::cast( pObj );
    if ( pModel.get() )
    {
      proceedForDgn(pModel->createGraphicsElementsIterator(), bCrash, bXT, bSat, bFeature, bSolid);
      proceedForDgn(pModel->createControlElementsIterator(), bCrash, bXT, bSat, bFeature, bSolid);
      continue;
    }

    OdDgSharedCellReferencePtr pShared = OdDgSharedCellReference::cast( pObj );
    if (!pShared.isNull())
    {
      OdDgSharedCellDefinitionPtr pSharedDef = pShared->findDefinition();
      if (!pSharedDef.isNull())
      {
        proceedForDgn(pSharedDef->createIterator(), bCrash, bXT, bSat, bFeature, bSolid);
      }
      continue;
    }
    
    OdDgCellHeader3dPtr pCell = OdDgCellHeader3d::cast( pObj );
    if (!pCell.isNull())
    {
      OdDgBRepEntityPEPtr pBrepPE = OdDgBRepEntityPEPtr(OdRxObjectPtr(pCell));
      OdDgBRepEntityPE::BRepEntityType bt = pBrepPE->type(pCell);
      if (bt == OdDgBRepEntityPE::eFeatureSolid)
      {
        OdStreamBufPtr binData = OdMemoryStream::createNew(); 
        OdUInt32 version = pBrepPE->brepVersion(pCell);
        if (pBrepPE->brepOutNative(pCell, *binData) == eOk && version != 0)
        {
          if ((version & OdDgModelerGeometry::kParasolidMask) && (bXT != CSHelperPage1::kCrash))
            bFeature = bXT = CSHelperPage1::kOk;
          else if ((version & OdDgModelerGeometry::kAcisMask) && (bSat != CSHelperPage1::kCrash))
            bFeature = bSat = CSHelperPage1::kOk;
        }
        else
          bFeature = bXT = bSat = CSHelperPage1::kCrash;
      }
      else if (bt == OdDgBRepEntityPE::eSmartSolid || bt == OdDgBRepEntityPE::eSmartSurface)
      {
        OdStreamBufPtr binData = OdMemoryStream::createNew(); 
        OdUInt32 version = pBrepPE->brepVersion(pCell);
        if (pBrepPE->brepOutNative(pCell, *binData) == eOk && version != 0)
        {
          if (version & OdDgModelerGeometry::kParasolidMask)
          {
            if (bXT != CSHelperPage1::kCrash ) bXT = CSHelperPage1::kOk;
          } 
          else if (bSat != CSHelperPage1::kCrash ) bSat = CSHelperPage1::kOk;
        }
        else
        {
          bXT = CSHelperPage1::kCrash;
          bSat = CSHelperPage1::kCrash;
        }
        proceedForDgn(pCell->createIterator(), bCrash, bXT, bSat, bFeature, bSolid);
      }

      continue;
    }
    
    if (!OdDgSolid::cast(pObj).isNull())
    {
      bSolid = CSHelperPage1::kOk;
      continue;
    }  
  }
}

void CSHelperPage1::getAttributesForDgn(CString fullPath, FlagState &bCrash, FlagState &bXT, FlagState &bSat, FlagState &bFeature, FlagState &bSolid)
{
  OdDgDatabasePtr pDb = theApp.readFile(OdString(fullPath));
  if (!pDb.isNull())
  {
    OdDgModelTablePtr pModelTable = pDb->getModelTable();
    OdDgElementIteratorPtr pIter = pModelTable->createIterator();

    proceedForDgn(pIter, bCrash, bXT, bSat, bFeature, bSolid);
  }
  else
  {
    bCrash = CSHelperPage1::kCrash;
  }
}

void CSHelperPage1::getAttributesForFile(CString fullPath, FlagState &bCrash, FlagState &bXT, FlagState &bSat, FlagState &bFeature, FlagState &bSolid)
{
  bCrash = CSHelperPage1::kOk;
  bXT = CSHelperPage1::kNotFound;
  bSat = CSHelperPage1::kNotFound;
  bFeature = CSHelperPage1::kNotFound;
  bSolid = CSHelperPage1::kNotFound;

  try
  {
    if (isDGN(fullPath))
      getAttributesForDgn(fullPath, bCrash, bXT, bSat, bFeature, bSolid);
    else if (isXT(fullPath) || isXB(fullPath))
    {
      bXT = CSHelperPage1::kOk;
      getAttributesForXTXB(fullPath, bCrash);
    }

    
  }
  catch (...)
  {
    bCrash = kCrash;
  }
}

void CSHelperPage1::addFileToList(CString fullPath)
{
  int n = fullPath.ReverseFind('\\');
  if (n != -1)
  {
    CString name = fullPath.Right(fullPath.GetLength() - n - 1);
    CString path = fullPath.Left(n);

    LV_ITEM lvi;

    lvi.mask = LVIF_TEXT;
    lvi.iItem = 0;
    lvi.iSubItem = 0;
    lvi.pszText = _T("0");
    lvi.iImage = 0;
    lvi.stateMask = 0;
    lvi.state = 0;

    m_FileList.InsertItem(&lvi);

    FlagState bCrash, bXT, bSat, bFeature, bSolid;
    getAttributesForFile(fullPath, bCrash, bXT, bSat, bFeature, bSolid);

    m_FileList.SetItemText(0, 0, name);
    m_FileList.SetItemText(0, 1, bCrash == kCrash ? _T("X") : _T("+"));
    m_FileList.SetItemText(0, 2, bXT == kCrash ? _T("X") : (bXT == kOk ? _T("+") : _T("-")));
    m_FileList.SetItemText(0, 3, bSat == kCrash ? _T("X") : (bSat == kOk ? _T("+") : _T("-")));
    m_FileList.SetItemText(0, 4, bFeature == kCrash ? _T("X") : (bFeature == kOk ? _T("+") : _T("-")));
    m_FileList.SetItemText(0, 5, bSolid == kCrash ? _T("X") : (bSolid == kOk ? _T("+") : _T("-")));
    m_FileList.SetItemText(0, 6, path);
  }
}

void CSHelperPage1::searchProcedure(CString folder, CString ext)
{
  WIN32_FIND_DATA FindFileData;
  HANDLE fffH = FindFirstFile( folder + ext, &FindFileData );
  if (fffH != INVALID_HANDLE_VALUE)
  {
    do 
    {
      CString str = folder + _T("\\") + CString(FindFileData.cFileName);
      if ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
      {
        addFileToList(str);
      }
    } while(FindNextFile(fffH, &FindFileData));
  }

  if (!m_EnableSubfolders )
    return;

  fffH = FindFirstFile( folder + _T("\\*.*"), &FindFileData );
  if (fffH != INVALID_HANDLE_VALUE)
  {
    do 
    {
      CString str = folder + _T("\\") + CString(FindFileData.cFileName);
      if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
      {
        if (FindFileData.cFileName != CString(_T("..")) && FindFileData.cFileName != CString(_T(".")))
          searchProcedure(str, ext);
      }
    } while(FindNextFile(fffH, &FindFileData));
  }
}

void CSHelperPage1::OnBnClickedSearch()
{
  // TODO: Add your control notification handler code here
  UpdateData(TRUE);

  theApp.WriteProfileString(_T("options"), _T("Helper path"), m_FolderPath);

  m_FileList.DeleteAllItems();
  UpdateData(FALSE);

  if (m_dgn)
    searchProcedure(m_FolderPath, _T("\\*.dgn"));
  if (m_xt)
    searchProcedure(m_FolderPath, _T("\\*.x_t"));
  if (m_xb)
    searchProcedure(m_FolderPath, _T("\\*.x_b"));
  if (m_sat)
  {
    searchProcedure(m_FolderPath, _T("\\*.sat"));
    searchProcedure(m_FolderPath, _T("\\*.sab"));
  }
}

void CSHelperPage1::OnNMDblclkFileList(NMHDR *pNMHDR, LRESULT *pResult)
{
  // TODO: Add your control notification handler code here
  *pResult = 0;

  LPNMITEMACTIVATE lpnmitem = (LPNMITEMACTIVATE) pNMHDR;

  UINT pFlags = 0;
  int selectedItem = m_FileList.HitTest(lpnmitem->ptAction, &pFlags);
  if (selectedItem != -1)
  {
    CString fileName = m_FileList.GetItemText(selectedItem,0);
    CString filePath = m_FileList.GetItemText(selectedItem,6);

    filePath = filePath + _T("\\") + fileName;

    if (isDGN(filePath))
      m_pSheet->fillAndSelect2Page(filePath);
    else if (isXT(filePath) || isXB(filePath) || isACIS(filePath))
    {
      OdStreamBufPtr data = OdMemoryStream::createNew();
      file2binaryData(filePath, data);
      m_pSheet->fillAndSelect3Page(*data);
    }
  }
}
