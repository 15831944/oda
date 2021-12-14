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

// OdDwgSignExDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SummInfo.h"
#include "OdDwgSignEx.h"
#include "OdDwgSignExDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();  

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BOOL CAboutDlg::OnInitDialog()
{
  OdDbLibraryInfo*  libInfo = oddbGetLibraryInfo();
  OdString  sLib = libInfo->getBuildComments();
  OdString  sCopyright = libInfo->getCopyright();

  CDialog::OnInitDialog();

  SetDlgItemText(IDC_INF_COPYRIGHT, sCopyright);
  SetDlgItemText(IDC_INF_LIBRARY, sLib);

  return TRUE;
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// COdDwgSignExDlg dialog




COdDwgSignExDlg::COdDwgSignExDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(COdDwgSignExDlg::IDD, pParent), m_pSignatureApp(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void COdDwgSignExDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialogEx::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_LIST2, m_fileList);
  DDX_Control(pDX, IDC_LIST3, m_certList);
  DDX_Control(pDX, IDC_COMBO1, m_timeServerCombo);
  DDX_Control(pDX, IDC_EDIT2, m_commentEdit);
}

BEGIN_MESSAGE_MAP(COdDwgSignExDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
  ON_BN_CLICKED(IDC_BUTTON1, &COdDwgSignExDlg::OnSelectFile)
  ON_BN_CLICKED(IDC_BUTTON2, &COdDwgSignExDlg::OnCheckSignatureStatus)
  ON_BN_CLICKED(IDOK, &COdDwgSignExDlg::OnSignFile)
END_MESSAGE_MAP()


// COdDwgSignExDlg message handlers

BOOL COdDwgSignExDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
      
  CRect rect;
  GetClientRect(&rect);

  //init cert list control:
  {
    m_certList.InsertColumn(0,_T("Subject"), LVCFMT_LEFT, rect.Width()*0.3);
    m_certList.InsertColumn(1,_T("Issuer"), LVCFMT_LEFT, rect.Width()*0.3);
    m_certList.InsertColumn(2,_T("Serial Number"), LVCFMT_LEFT, rect.Width()*0.3);    
    DWORD ExStyle = m_certList.GetExtendedStyle();
    ExStyle |= LVS_EX_FULLROWSELECT;
    m_certList.SetExtendedStyle(ExStyle);

    int certsNumber = m_certDescArray.size();
    for (int i = 0; i < certsNumber; i++)
    {
      m_certList.InsertItem(i, m_certDescArray[i].m_CertSubject.c_str());
      m_certList.SetItemText(i, 1, m_certDescArray[i].m_CertIssuer.c_str());
      m_certList.SetItemText(i, 2, m_certDescArray[i].m_CertSerialNum.c_str());
    }
  }

  //init file list control:
  {
    m_fileList.InsertColumn(0,_T("File Name"), LVCFMT_LEFT, rect.Width());
    DWORD ExStyle = m_certList.GetExtendedStyle();
    ExStyle |= LVS_EX_FULLROWSELECT;
    m_certList.SetExtendedStyle(ExStyle);
  }

  //init time server combo:
  {
    m_timeServerCombo.InsertString(0, _T("(No time stamp)"));
    m_timeServerCombo.InsertString(1, _T("Local computer"));
    m_timeServerCombo.InsertString(2, _T("time-a-g.nist.gov"));

    m_timeServerCombo.SetCurSel(0);
  }

  //init comment edit control:
  {
    m_commentEdit.Clear();
  }


	return TRUE;  // return TRUE  unless you set the focus to a control
}

void COdDwgSignExDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void COdDwgSignExDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR COdDwgSignExDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL DoPromptFileName(CString& fileName, DWORD lFlags, BOOL bOpenFileDialog)
{
  CFileDialog dlgFile(bOpenFileDialog);
  dlgFile.m_ofn.Flags |= lFlags;

  CString strFilter;

  strFilter += _T("dwg files (*.dwg)");
  strFilter += (TCHAR)'\0';   // next string please
  strFilter += _T("*.dwg");
  strFilter += (TCHAR)'\0';   // last string
  dlgFile.m_ofn.nMaxCustFilter++;
  dlgFile.m_ofn.nFilterIndex = 1;

  dlgFile.m_ofn.lpstrFilter = strFilter;
  dlgFile.m_ofn.lpstrTitle = L"Select File";
  dlgFile.m_ofn.lpstrFile = fileName.GetBuffer(_MAX_PATH);

  LPARAM nResult = dlgFile.DoModal();
  fileName.ReleaseBuffer();

  return (nResult == IDOK);
}

void COdDwgSignExDlg::OnSelectFile()
{
  CString fileName;
  if (IDOK == DoPromptFileName(fileName, OFN_HIDEREADONLY | OFN_EXPLORER | OFN_PATHMUSTEXIST, TRUE))
  {
    m_pSignatureApp->m_drawingFilePath = fileName;

    m_fileList.DeleteAllItems();
    m_fileList.InsertItem(0, fileName);
  }
}

void COdDwgSignExDlg::OnCheckSignatureStatus()
{
  if (!m_pSignatureApp->m_drawingFilePath.isEmpty())
  {
    OdCryptoServices::OdSignatureVerificationResult verifResult;
    OdSignatureDescription sigData;
    OdResult res;
    res = m_pSignatureApp->checkSignatureStatus(verifResult, sigData);
    if (res == eOk)
    {
      OdString resultMessage;
      switch (verifResult)
      {
      case OdCryptoServices::kSuccess:
        resultMessage = L"The digital signature is valid.";
        break;
      case OdCryptoServices::kHasNoSignature:
        resultMessage = L"Drawing has no signature.";
        break;
      case OdCryptoServices::kBadSignature:
        resultMessage = L"Bad signature!";
        break;
      case OdCryptoServices::kCertificateChainProblem:
        resultMessage = L"Problem with one of the certificates in the path!";
        break;
      default:
        resultMessage = L"Error during verification!";
        break;
      }

      OdString fullMessage;
      fullMessage = resultMessage + L"\n\n" + 
        L"Digital ID (Certificate) info:\n\n" +
        L"Subject\t\t=\t" + sigData.m_certDesc.m_CertSubject + 
        L"\nIssuer\t\t=\t" + sigData.m_certDesc.m_CertIssuer +
        L"\nSerial number\t=\t" + sigData.m_certDesc.m_CertSerialNum +        
        L"\nValid from\t\t=\t" + sigData.m_certDesc.m_CertValidFrom +
        L"\nValid to\t\t=\t" + sigData.m_certDesc.m_CertValidTo +
        L"\n\nSignature attributes:\n\n" +
        L"Date and time\t=\t" + sigData.m_signatureDateAndTime +
        L"\nTime service used\t=\t" + sigData.m_timeServiceUsed +
        L"\nComment\t\t=\t" + sigData.m_comment;

      CString resultDataMsg(fullMessage.c_str());
      MessageBox(resultDataMsg, L"OdDwgSignEx", MB_OK);
    }
    else if (res == eCantOpenFile)
    {
      MessageBox(L"Can't open file!", L"OdDwgSignEx", MB_OK);
    }
    else
    {
      MessageBox(L"Can't check signature for this file format version.", L"OdDwgSignEx", MB_OK);
    }
  }
  else
  {
    MessageBox(L"Please, select file to check signature.", L"OdDwgSignEx", MB_OK);
  }
}

void COdDwgSignExDlg::OnSignFile()
{
  int indexOfSelectedCert = m_certList.GetSelectionMark();
  int indexOfSelectedTimeServer = m_timeServerCombo.GetCurSel();

  if (indexOfSelectedCert >= 0)
  {
    if (!m_pSignatureApp->m_drawingFilePath.isEmpty())
    {
      OdString subject = m_certDescArray[indexOfSelectedCert].m_CertSubject;
      OdString issuer = m_certDescArray[indexOfSelectedCert].m_CertIssuer;
      OdString serailNum = m_certDescArray[indexOfSelectedCert].m_CertSerialNum;

      OdString timeServer;
      bool addTimestamp = false;
      if (indexOfSelectedTimeServer > 0)
      {
        addTimestamp = true;
        if (indexOfSelectedTimeServer == 2)
          timeServer = OD_T("time-a-g.nist.gov");
      }
      OdString comment;
      CString commentText;
      m_commentEdit.GetWindowText(commentText);
      comment = (LPCWSTR)commentText;

      try
      {
        m_pSignatureApp->signDrawingFile(subject, issuer, serailNum, addTimestamp, timeServer, comment);
        MessageBox(L"File is successfully signed!", L"OdDwgSignEx", MB_OK);
      }
      catch (OdError_CantOpenFile& )
      {
        MessageBox(L"Can't open file in Read/Write mode!", L"OdDwgSignEx", MB_OK);
      }
    }
    else
    {
      MessageBox(L"Please, select file to be signed.", L"OdDwgSignEx", MB_OK);
    }
  }
  else
  {
    MessageBox(L"Please, select digital ID (Certificate) from the list before signing.", L"OdDwgSignEx", MB_OK);
  }
}
