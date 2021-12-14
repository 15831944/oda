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

// ExCustEntityWrapper.h : Declaration of the CExCustEntityWrapper

#pragma once
#include "resource.h"       // main symbols

#include "ExCustObjCOM.h"
#include "OdaCommon.h"
#include "ExCustEntity.h"
#include "DbHostAppServices.h"
#include "odaxwrap.h"


// CExCustEntityWrapper

class ATL_NO_VTABLE CExCustEntityWrapper : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CExCustEntityWrapper, &CLSID_ExCustEntityWrapper>,
	public ISupportErrorInfo,
	public IDispatchImpl<IExCustEntityWrapper, &IID_IExCustEntityWrapper, &LIBID_ExCustObjCOMLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
  IUnknown* m_pCircle;
  CExCustEntityWrapper() : m_pCircle(0)
  {
  }

DECLARE_REGISTRY_RESOURCEID(IDR_EXCUSTENTITYWRAPPER)


BEGIN_COM_MAP(CExCustEntityWrapper)
  COM_INTERFACE_ENTRY2(IDispatch,IExCustEntityWrapper)
  COM_INTERFACE_ENTRY(IExCustEntityWrapper)
  COM_INTERFACE_ENTRY_AGGREGATE(IID_IAcadObject, m_pCircle)
  COM_INTERFACE_ENTRY_AGGREGATE(IID_IAcadEntity, m_pCircle)
  COM_INTERFACE_ENTRY_AGGREGATE(__uuidof(IRetrieveApplication), m_pCircle)
  COM_INTERFACE_ENTRY_AGGREGATE(__uuidof(IAcadBaseObject), m_pCircle)
  COM_INTERFACE_ENTRY_AGGREGATE(IID_IAcadCircle, m_pCircle)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);
  DECLARE_GET_CONTROLLING_UNKNOWN()
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
    return CoCreateInstance(CLSID_AcadCircle, GetControllingUnknown(), 
      CLSCTX_ALL, IID_IUnknown, (void**) &m_pCircle);
	}
	
	void FinalRelease() 
	{
    if (m_pCircle)
      m_pCircle->Release();
	}

  STDMETHOD(get_Count)(SHORT* pVal);
  STDMETHOD(put_Count)(SHORT newVal);
};

OBJECT_ENTRY_AUTO(__uuidof(ExCustEntityWrapper), CExCustEntityWrapper)
