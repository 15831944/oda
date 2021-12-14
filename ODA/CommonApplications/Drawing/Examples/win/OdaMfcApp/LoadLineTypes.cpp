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

// LoadLineTypes.cpp : implementation file
//

#include "stdafx.h"
#include "odamfcapp.h"
#include "LoadLineTypes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLoadLineTypes dialog


CLoadLineTypes::CLoadLineTypes(OdDbDatabasePtr pDb, CWnd* pParent /*=NULL*/)
  : CDialog(CLoadLineTypes::IDD, pParent)
  , m_pDb(pDb)
  , m_SomethingDoing(FALSE)
  , m_DescIndx(0)
{
  //{{AFX_DATA_INIT(CLoadLineTypes)
    // NOTE: the ClassWizard will add member initialization here
  //}}AFX_DATA_INIT
}


void CLoadLineTypes::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CLoadLineTypes)
  DDX_Control(pDX, IDC_LIST_LINETYPE, m_ListCtrl);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLoadLineTypes, CDialog)
  //{{AFX_MSG_MAP(CLoadLineTypes)
  ON_BN_CLICKED(ID_OPEN_LINETYPEFILE, OnOpenLinetypefile)
  ON_BN_CLICKED(ID_LOAD_SELECTION, OnLoadSelection)
  ON_BN_CLICKED(IDC_BUTTON_LOAD_ALL, OnButtonLoadAll)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

static inline void CLoadLineTypes_Swap2Bytes(void *ptr)
{
  BYTE *buf = (BYTE*)ptr;
  buf[0] ^= buf[1] ^= buf[0] ^= buf[1];
}
static inline void CLoadLineTypes_Swap4Bytes(void *ptr)
{
  BYTE *buf = (BYTE*)ptr;
  buf[0] ^= buf[3] ^= buf[0] ^= buf[3];
  buf[1] ^= buf[2] ^= buf[1] ^= buf[2];
}

BOOL CLoadLineTypes::ReadLinetypeFileLines(const TCHAR *fileName, CStringArray &ret)
{
  CFile file;
  ret.RemoveAll();
  if (!file.Open(fileName, CFile::modeRead | CFile::shareDenyWrite | CFile::typeBinary /*| CFile::osSequentialScan*/))
    return FALSE;
  ULONGLONG flen = file.GetLength();
  if (flen < 4) return FALSE;
  BYTE *buffer = (BYTE*)malloc((size_t)flen);
  if (buffer == NULL) return false;
  file.Read(buffer, (UINT)flen);
  file.Close();
  int type = 0, hlen = 0, ssize = 1; // ANSI
  unsigned long magic = *((unsigned long*)buffer);
  if ((magic & 0x00ffffff) == 0xbfbbef) { type = 1; hlen = 3; } // UTF8
  else if (magic == 0x0000feff) { type = 4; hlen = ssize = 4; } // UTF32LE
  else if (magic == 0xfffe0000) { type = 5; hlen = ssize = 4; } // UTF32BE
  else if ((magic & 0x0000ffff) == 0xfeff) { type = 2; hlen = ssize = 2; } // UTF16LE
  else if ((magic & 0x0000ffff) == 0xfffe) { type = 3; hlen = ssize = 2; } // UTF16BE
  TCHAR *syms = (TCHAR*)malloc(((size_t)flen - hlen) / ssize * sizeof(TCHAR) * 4 + sizeof(TCHAR));
  if (syms == NULL)
  {
    free(buffer);
    return FALSE;
  }
  ULONG n_syms = (ULONG)(flen - hlen) / ssize;
  ZeroMemory(syms, (n_syms * 4 + 1) * sizeof(TCHAR));
  buffer += hlen;
  switch (type) {
    case 0: // ANSI
      {
#ifndef UNICODE
        CopyMemory(syms, buffer, n_syms * sizeof(TCHAR));
#else
        MultiByteToWideChar(CP_ACP, 0, (LPCSTR)buffer, n_syms, syms, n_syms * 4 + 1);
#endif
      }
    break;
    case 1: // UTF8
      {
#ifndef UNICODE
        WCHAR *indata = (WCHAR*)malloc((n_syms + 1) * sizeof(WCHAR));
        ZeroMemory(indata, (n_syms + 1) * sizeof(WCHAR));
        MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)buffer, n_syms, indata, n_syms + 1);
        WideCharToMultiByte(CP_ACP, 0, indata, -1, syms, n_syms * 4 + 1, "?", NULL);
        free(indata);
#else
        MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)buffer, n_syms, syms, n_syms * 4 + 1);
#endif
      }
    break;
    default: // Unicode
      {
        ULONG outBuf = 0;
        for (ULONG n = 0; n < n_syms; n++)
        {
          WCHAR sym[3] = { 0, 0, 0 };
          switch (type)
          {
            case 2: // UTF16LE
            case 3: // UTF16BE
              {
                sym[0] = ((WCHAR*)buffer)[n];
                if (type == 3)
                  CLoadLineTypes_Swap2Bytes(sym);
                if (sym[0] >= 0xD800 && sym[0] <= 0xDFFF)
                {
                  sym[1] = ((WCHAR*)buffer)[++n];
                  if (type == 3)
                    CLoadLineTypes_Swap2Bytes(sym + 1);
                }
              }
            break;
            case 4: // UTF32LE
            case 5: // UTF32BE
              {
                OdUInt32 rd = ((OdUInt32*)buffer)[n];
                if (type == 5)
                  CLoadLineTypes_Swap4Bytes(&rd);
                if (rd >= 0x10000 && rd <= 0x10FFFF)
                {
                  sym[0] = (WCHAR)((0xD800 - (0x10000 >> 10)) + (rd >> 10));
                  sym[1] = (WCHAR)(0xDC00 + (rd & 0x3FF));
                }
                else
                  sym[0] = (WCHAR)rd;
              }
            break;
          }
#ifndef UNICODE
          outBuf += WideCharToMultiByte(CP_ACP, 0, sym, -1, syms, n_syms * 4 + 1 - outBuf, "?", NULL);
#else
          syms[outBuf++] = sym[0];
          if (sym[1] != 0)
            syms[outBuf++] = sym[1];
#endif
        }
      }
  }
  n_syms = (ULONG)_tcslen(syms);
  free(buffer - hlen);
  ULONG j;
  for (j = 0; j < n_syms; j++)
  {
    if (syms[j] == _T('\x0d') || syms[j] == _T('\x0a')) syms[j] = 0;
  }
  bool tried = false;
  for (j = 0; j < n_syms; j++)
  {
    if (syms[j] != 0 && !tried)
    {
      ret.Add(CString(syms + j));
      tried = true;
    }
    else if (syms[j] == 0 && tried)
    {
      tried = false;
    }
  }
  free(syms);
  return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CLoadLineTypes message handlers

void CLoadLineTypes::OnOpenLinetypefile() 
{
  // TODO: Add your control notification handler code here
  CFileDialog OpenFileDlg(TRUE,
    _T("*.lin"),
    NULL,
    OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT|OFN_FILEMUSTEXIST,
    _T("Linetype files (*.lin)|*.lin|All Files (*.*)|*.*||"));

  LPARAM pressed = OpenFileDlg.DoModal();
  if (pressed == IDOK)
  {
    m_ListCtrl.DeleteAllItems();

    CString fileName = OpenFileDlg.GetPathName();

#if 0 // * * * old code
    CStdioFile linFile(fileName, CFile::modeRead);
    m_FileName = fileName;

    CString tmp;

//    int subindx(0);

    while(linFile.ReadString(tmp))
    {
      if (!tmp.IsEmpty() && tmp[0] == '*')
      {
        int i = tmp.Find(',');
        int nItem = m_ListCtrl.GetItemCount();
        if (i != -1)
        {
          int item = m_ListCtrl.InsertItem (nItem, tmp.Mid(1, i-1));
          tmp.Delete(0, i+1);
          m_ListCtrl.SetItemText (item, m_DescIndx, tmp);
        }
        else
        {
          tmp.Delete(0);
          m_ListCtrl.InsertItem (nItem, tmp);
        }

        
      }
    }
    linFile.Close();
#else // * * * new code
    m_FileName = fileName;
    CStringArray strings;
    if (!ReadLinetypeFileLines(fileName, strings) || /*strings.IsEmpty()*/strings.GetSize() <= 0) return;
    CString tmp;
    for (INT_PTR i = 0; i < strings.GetSize()/*GetCount()*/; i++)
    {
      tmp = strings.GetAt(i);
      if (!tmp.IsEmpty() && tmp[0] == _T('*'))
      {
        int i = tmp.Find(_T(','));
        int nItem = m_ListCtrl.GetItemCount();
        if (i != -1)
        {
          int item = m_ListCtrl.InsertItem (nItem, tmp.Mid(1, i - 1));
          tmp.Delete(0, i + 1);
          m_ListCtrl.SetItemText (item, m_DescIndx, tmp);
        }
        else
        {
          tmp.Delete(0);
          m_ListCtrl.InsertItem (nItem, tmp);
        }
      }
    }
#endif
  }
}

void CLoadLineTypes::OnLoadSelection() 
{
  CString tmp;
  try
  {
    POSITION pos =  m_ListCtrl.GetFirstSelectedItemPosition( );
    if (pos)
    {
      while(pos)
      {
        int i = m_ListCtrl.GetNextSelectedItem(pos);
        tmp = m_ListCtrl.GetItemText( i, 0);
        m_pDb->loadLineTypeFile(OdString(tmp), OdString(m_FileName), OdDb::kDltReplace);
      }
      m_SomethingDoing = TRUE;
      MessageBox(_T("Succeed."));
    }
  }
  catch(OdError& err)
  {
    tmp = _T("Fail to load : ") + tmp;
    tmp += _T("\r\n");
    tmp += (LPCTSTR)err.description();
    MessageBox(tmp);
  }
  catch(...)
  {
    tmp = _T("Fail to load : ") + tmp;
    MessageBox(tmp);
  }
}

void CLoadLineTypes::OnButtonLoadAll() 
{
  // TODO: Add your control notification handler code here
  try
  {
    m_pDb->loadLineTypeFile(OD_T("*"), OdString(m_FileName), OdDb::kDltReplace);
    m_SomethingDoing = TRUE;
    MessageBox(_T("Succeed."));
  }
  catch(OdError& err)
  {
    CString tmp = _T("Fail to load : ") + m_FileName;
    tmp += _T("\r\n");
    tmp += (LPCTSTR)err.description();
    MessageBox(tmp);
  }
  catch(...)
  {
    m_FileName = _T("Fail to load : ") + m_FileName;
    MessageBox(m_FileName);
  }
}

BOOL CLoadLineTypes::OnInitDialog() 
{
  CDialog::OnInitDialog();
  
  // TODO: Add extra initialization here
  m_ListCtrl.InsertColumn( 0, _T("Name"), LVCFMT_LEFT, 100);
  m_DescIndx = m_ListCtrl.InsertColumn( 1, _T("Description"), LVCFMT_LEFT, 200); 

  return TRUE;  // return TRUE unless you set the focus to a control
                // EXCEPTION: OCX Property Pages should return FALSE
}
