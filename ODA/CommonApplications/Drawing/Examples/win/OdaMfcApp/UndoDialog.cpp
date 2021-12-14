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
// UndoDialog.cpp : implementation file
//

#include "stdafx.h"
#include "odamfcapp.h"
#include "UndoDialog.h"
#include "DbHistoryManager.h"

// UndoDialog dialog

UndoDialog::UndoDialog(OdDbDatabase* pDb, CWnd* pParent /*=NULL*/)
	: CDialog(UndoDialog::IDD, pParent)
  , m_pDb(pDb)
  , m_nSelected(0)
{

}

UndoDialog::~UndoDialog()
{
}

void UndoDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
  // DDX_Control(pDX, IDC_UNDOLIST, m_Preview);
}


BEGIN_MESSAGE_MAP(UndoDialog, CDialog)
  ON_COMMAND(IDC_ALAYOUT_CLOSE, OnUndoDlgClose)
  ON_LBN_SELCHANGE(IDC_UNDOLIST, OnSelchangeUndolist)
  ON_LBN_DBLCLK(IDC_UNDOLIST, OnDblclkUndolist)

END_MESSAGE_MAP()

BOOL UndoDialog::OnInitDialog()
{
  CDialog::OnInitDialog();
  FillListBox();

  CPoint pt;
  GetCursorPos(&pt);
  SetWindowPos(NULL, pt.x, pt.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
  return TRUE;
}

void UndoDialog::FillListBox()
{
  try
  {
    OdArray<OdString>         items;

    OdDbHistoryManager* hist = m_pDb->historyManager();
    if (hist)
    {
      OdDbHistoryLogRecord log;
      int i = 0;
      while (hist->getLogMessage(i++, log))
      {
        OdString format(L"%m//%d//%Y %H:%M");
        OdString time;

        log.timestamp.strftime(format, time);
        if (log.message.isEmpty())
          items.push_back(log.author + " -> ");
        else
          items.push_back(log.author + " -> " + log.message + " -> " + time);
      }
    }
    else
    {
      items.push_back(L"Empty");
    }

    CListBox* pUndo = (CListBox*)GetDlgItem(IDC_UNDOLIST);
    pUndo->ResetContent();
    if ( items.size() > 1 )
    {
      for (int i = 0, n = items.size()-1; n >= 0; --n, i++)
      {
        pUndo->InsertString(i, items[n]);
      }
    }
    else
    {
      if ( items[0].isEmpty() )
        pUndo->InsertString(0, L"Empty");
      else
        pUndo->InsertString(0, items[0]);
    }
  }
  catch (OdError& e)
  {
    theApp.reportError(_T("Error Selecting Layout"), e);
  }
}

void UndoDialog::OnUndoDlgClose()
{
  CListBox* pUndo = (CListBox*)GetDlgItem(IDC_UNDOLIST);
  m_nSelected = pUndo->GetCurSel();
}

void UndoDialog::OnSelchangeUndolist()
{
  CListBox* pUndo = (CListBox*)GetDlgItem(IDC_UNDOLIST);
  int curSel = pUndo->GetCurSel();
  pUndo->SelItemRange(true, 0, curSel);
}

void UndoDialog::OnDblclkUndolist()
{
  CListBox* pUndo = (CListBox*)GetDlgItem(IDC_UNDOLIST);
  m_nSelected = pUndo->GetCurSel();
  EndDialog(IDOK);
}

// UndoDialog message handlers
