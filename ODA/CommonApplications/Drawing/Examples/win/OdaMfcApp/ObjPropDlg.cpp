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

  // ObjPropDlg.cpp : implementation file
//

#include "stdafx.h"

#if defined(_TOOLKIT_IN_DLL_) && (_MSC_VER > 1310)

#include "odamfcapp.h"
#include "ObjPropDlg.h"
#include "odaxwrap.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CObjPropDlg dialog

CObjPropDlg::CObjPropDlg(const OdDbObjectId &id, CWnd* pParent /*=NULL*/)
	: CDialog(CObjPropDlg::IDD, pParent), m_pObjWrapper(NULL), m_nCookie(0)
{
	//{{AFX_DATA_INIT(CObjPropDlg)
	m_sValue = _T("");
	//}}AFX_DATA_INIT
  HRESULT hr = ::CoInitialize(NULL);
  if(SUCCEEDED(hr))
  {
    m_pObjWrapper = IUnknownPtr(::OdOxGetIUnknownOfObject(id, NULL), false);
  }
  else
    throw(hr);
}

CObjPropDlg::~CObjPropDlg()
{
  IConnectionPointContainerPtr pCPC = m_pObjWrapper;
  ODA_ASSERT(pCPC!=NULL); // Object must support this interface
  if(pCPC!=NULL)
  {
    IConnectionPointPtr pCP;
    HRESULT hr = pCPC->FindConnectionPoint(__uuidof(IPropertyNotifySink), &pCP);
    ODA_ASSERT(SUCCEEDED(hr)); // Object must support this interface
    if(SUCCEEDED(hr))
    {
      hr = pCP->Unadvise(m_nCookie);
      ODA_ASSERT(SUCCEEDED(hr)); // Object must support this interface
    }
  }
}

void CObjPropDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CObjPropDlg)
	DDX_Control(pDX, IDC_BUTTON1, m_doset);
	DDX_Control(pDX, IDC_PROPLIST, m_propList);
	DDX_Text(pDX, IDC_VALUE, m_sValue);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CObjPropDlg, CDialog)
	//{{AFX_MSG_MAP(CObjPropDlg)
	ON_EN_SETFOCUS(IDC_VALUE, OnSetfocusValue)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton)
	ON_NOTIFY(NM_CLICK, IDC_PROPLIST, OnClickProplist)
	ON_NOTIFY(LVN_KEYDOWN, IDC_PROPLIST, OnKeydownProplist)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CObjPropDlg message handlers

_COM_SMARTPTR_TYPEDEF(IOPMPropertyExpander, __uuidof(IOPMPropertyExpander));
_COM_SMARTPTR_TYPEDEF(IOPMPropertyExtension, __uuidof(IOPMPropertyExtension));

CString getErrorMsg(HRESULT hr)
{
  LPVOID lpMsgBuf;
  ::FormatMessageW( 
      FORMAT_MESSAGE_ALLOCATE_BUFFER | 
      FORMAT_MESSAGE_FROM_SYSTEM | 
      FORMAT_MESSAGE_IGNORE_INSERTS,
      NULL,
      hr,
      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
      (LPTSTR) &lpMsgBuf,
      0,
      NULL 
  );
  CString res((const wchar_t*)lpMsgBuf);
  // Free the buffer.
  ::LocalFree( lpMsgBuf );
  res.TrimRight();
  return res;
}

CString getPropValue(MEMBERID memid, IDispatch* pDisp)
{
  CString       val;
  unsigned int  uArgErr;
  EXCEPINFO     except;
  _variant_t    varRes;
  DISPPARAMS    param = { NULL, NULL, 0, 0 };
  
  HRESULT hr = pDisp->Invoke(memid, IID_NULL, ::GetSystemDefaultLCID(), 
                             DISPATCH_PROPERTYGET, &param, &varRes, &except, &uArgErr);
  if (SUCCEEDED(hr))
  {
    if (VT_EMPTY == varRes.vt)
    {
      val = CString("No Data");
    }
    // is it interface pointer or objects 
    else if (VT_DISPATCH == varRes.vt || (VT_DISPATCH | VT_ARRAY) == varRes.vt ||
             VT_UNKNOWN  == varRes.vt || (VT_UNKNOWN  | VT_ARRAY) == varRes.vt )
    {
      IAcadAcCmColor* pColor = 0;
      if (VT_DISPATCH == varRes.vt && varRes.pdispVal != 0 && SUCCEEDED(varRes.pdispVal->QueryInterface(&pColor)))
      {
        long ec;
        if (SUCCEEDED(pColor->get_EntityColor(&ec)))
        {
          OdCmColor c; c.setColor(ec);
          val = (LPCTSTR)c.colorNameForDisplay();
        }
        pColor->Release();
      }
      else
        val = "Interface pointer";
    }
      //  is it array ?
    else if (VT_ARRAY == (varRes.vt & VT_ARRAY))
    {
      OdString   sTmp;
      long       lLow, lHigh, index;
      SAFEARRAY* pSafeArray = varRes.parray;

      val = "";
      SafeArrayGetLBound(pSafeArray, 1, &lLow);
      SafeArrayGetUBound(pSafeArray, 1, &lHigh);

      for (index = lLow; index <= lHigh; ++index)
      {
        if ((VT_ARRAY | VT_R8) == varRes.vt)
        {
          double  tmp;
          SafeArrayGetElement(pSafeArray, &index, &tmp);
          sTmp.format(OD_T("%s "), odDToStr(tmp, 'f', 6).c_str());
        }
        else if ((VT_ARRAY | VT_I4) == varRes.vt)
        {
          long tmp;
          SafeArrayGetElement(pSafeArray, &index, &tmp);
          sTmp.format(OD_T("%d "), tmp);
        }
        val += (LPCTSTR)sTmp;
      }
    }
    else if (VT_BOOL == varRes.vt)
    {
      if (VARIANT_TRUE == varRes.boolVal)
        val = CString(_T("TRUE"));
      else
        val = CString(_T("FALSE"));
    }
    else
    {
      val = (const WCHAR*)(_bstr_t)varRes;
    }
  }
  else if (hr == DISP_E_EXCEPTION)
  {
    if (except.scode == E_NOTIMPL)
    {
      val = _T("E_NOTIMPL");
    }
    else
    {
      val = ::getErrorMsg(except.scode);
    }
  } // end of if(pDisp->Invoke) ... else  ...
  return val;
}

HRESULT STDMETHODCALLTYPE CObjPropDlg::OnRequestEdit(DISPID /*dispID*/)
{
  return S_OK;
}

HRESULT STDMETHODCALLTYPE CObjPropDlg::OnChanged(DISPID /*dispID*/)
{
  IDispatchPtr pDisp = m_pObjWrapper;
  for(int i=0; i< m_propList.GetItemCount(); ++i)
  {
    m_propList.SetItemText(i, 1, getPropValue(m_propList.GetItemData(i), pDisp));
  }
  return S_OK;
}

HRESULT STDMETHODCALLTYPE CObjPropDlg::QueryInterface(REFIID riid, void __RPC_FAR*__RPC_FAR* ppvObject)
{
  if(IsEqualGUID(riid, __uuidof(IUnknown)) || IsEqualGUID(riid, __uuidof(IPropertyNotifySink)))
  {
    *ppvObject = dynamic_cast<IPropertyNotifySink*>(this);
    return S_OK;
  }
  return E_NOINTERFACE;
}

BOOL CObjPropDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
  m_propList.InsertColumn(0, _T("Property"), LVCFMT_LEFT, 180);
  m_propList.InsertColumn(1, _T("Value"), LVCFMT_LEFT, 120);

  fill();
  IConnectionPointContainerPtr pCPC = m_pObjWrapper;
  ODA_ASSERT(pCPC!=NULL); // Object must support this interface
  if(pCPC!=NULL)
  {
    IConnectionPointPtr pCP;
    HRESULT hr = pCPC->FindConnectionPoint(__uuidof(IPropertyNotifySink), &pCP);
    ODA_ASSERT(SUCCEEDED(hr)); // Object must support this interface
    if(SUCCEEDED(hr))
    {
      hr = pCP->Advise(this, &m_nCookie);
      ODA_ASSERT(SUCCEEDED(hr)); // Object must support this interface
    }
  }
	return TRUE;
}

void CObjPropDlg::OnButton() 
{
  DWORD memid = m_propList.GetItemData(m_nCurItem);
  UpdateData();
  _variant_t varArg = m_sValue;
  DISPID dispidPut = DISPID_PROPERTYPUT;
  DISPPARAMS param = { &varArg, &dispidPut, 1, 1 };
  _variant_t varRes;
  unsigned int uArgErr;
      
  EXCEPINFO except;
  IDispatchPtr pDisp = m_pObjWrapper;
  HRESULT hr = pDisp->Invoke(
    memid,
    IID_NULL,
    ::GetSystemDefaultLCID(),
    DISPATCH_PROPERTYPUT,
    &param,
    &varRes,
    &except,
    &uArgErr
    );

  if(FAILED(hr))
  {
    m_sValue = ::getErrorMsg(hr);
    UpdateData(FALSE);
  }
}

void CObjPropDlg::fill(void)
{
  if (m_pObjWrapper)
  {
    int                       i = 0;
    FUNCDESC                 *pFD;
    ITypeInfoPtr              pTi;
    IDispatchPtr              pDisp = m_pObjWrapper;
    IPerPropertyBrowsingPtr   pPerPropBrows = m_pObjWrapper;
    IOPMPropertyExpanderPtr   pExp = m_pObjWrapper;
    IOPMPropertyExtensionPtr  pPExt = m_pObjWrapper;

    m_propList.DeleteAllItems();    // clear the list 
    pDisp->GetTypeInfo(0, ::GetSystemDefaultLCID(), &pTi);
    while (SUCCEEDED(pTi->GetFuncDesc(i, &pFD)))
    {
      if (pFD->invkind == INVOKE_PROPERTYGET 
           // && (pFD->wFuncFlags & FUNCFLAG_FNONBROWSABLE)==0
         )
      {
        HRESULT     hr;
        CALPOLESTR  opmStrings;
        CADWORD     opmCookies;

        if ((pExp != NULL) && SUCCEEDED(hr = pExp->GetElementStrings(pFD->memid, &opmStrings, &opmCookies)) && opmStrings.cElems)
        {
          ::CoTaskMemFree(opmCookies.pElems);
          while(opmStrings.cElems--)
          {
            _bstr_t str(opmStrings.pElems[opmStrings.cElems], false);
            m_propList.InsertItem(m_propList.GetItemCount(), str);
          }
          ::CoTaskMemFree(opmStrings.pElems);
        }
        else if (pPExt)
        {
          BSTR res;
          hr = pPExt->GetDisplayName(pFD->memid, &res);
          if(SUCCEEDED(hr))
          {
            _bstr_t str(res, false);
            m_propList.InsertItem(m_propList.GetItemCount(), str);
          }
        }
        else
        {
          BSTR  res;
          hr = pTi->GetDocumentation(pFD->memid, &res, NULL, NULL, NULL);
          if (SUCCEEDED(hr))
          {
            _bstr_t str(res, false);
            int n = m_propList.InsertItem(m_propList.GetItemCount(), str);
            m_propList.SetItemData(n, pFD->memid);
            m_propList.SetItemText(n, 1, getPropValue(pFD->memid, pDisp));
          } // end of if(SUCCEEDED(pTi->GetDocumentation()))
        }
      } //  end of if (pFD->invkind == INVOKE_PROPERTYGET )
      i++;
      pTi->ReleaseFuncDesc(pFD);
    } // end of while (SUCCEEDED(pTi->GetFuncDesc(i, &pFD)))
  } // end of if (m_pObjWrapper)
  return;
}

void CObjPropDlg::OnClickProplist(NMHDR* /*pNMHDR*/, LRESULT* pResult) 
{
  OnSetfocusValue();
	*pResult = 0;
}

void CObjPropDlg::OnSetfocusValue() 
{
	m_nCurItem = m_propList.GetSelectionMark();
  m_doset.EnableWindow(m_nCurItem != -1);
  if(m_nCurItem != -1)
  {
    m_sValue = m_propList.GetItemText(m_nCurItem, 1);
    UpdateData(FALSE);
  }
}


void CObjPropDlg::OnKeydownProplist(NMHDR* /*pNMHDR*/, LRESULT* pResult) 
{
//	LV_KEYDOWN* pLVKeyDow = (LV_KEYDOWN*)pNMHDR;
  OnSetfocusValue();
	*pResult = 0;
}
#endif //#if defined(_TOOLKIT_IN_DLL_) && (_MSC_VER > 1310)
