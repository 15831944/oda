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

// ExCustEntityWrapper.cpp : Implementation of CExCustEntityWrapper

#include "stdafx.h"
#include "ExCustEntityWrapper.h"


// CExCustEntityWrapper

STDMETHODIMP CExCustEntityWrapper::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IExCustEntityWrapper
	};

	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

STDMETHODIMP CExCustEntityWrapper::get_Count(SHORT* pVal)
{
  if (!pVal) 
    return E_POINTER;

  if (!m_pCircle) 
    return E_FAIL;
  CComQIPtr<IAcadBaseObject> pBase = m_pCircle;
  if (pBase == 0)
    return E_FAIL;

  OdDbObjectId id;
  pBase->GetObjectId(&id);
  ExCustEntityPtr e = id.safeOpenObject();
  *pVal = e->getCount();
  return S_OK;
}

STDMETHODIMP CExCustEntityWrapper::put_Count(SHORT newVal)
{
  if (!m_pCircle) 
    return E_FAIL;

  CComQIPtr<IAcadBaseObject> pBase = m_pCircle;
  if (pBase == 0)
    return E_FAIL;

  OdDbObjectId id;
  pBase->GetObjectId(&id);
  ExCustEntityPtr e = id.safeOpenObject(OdDb::kForWrite);
  e->setCount(newVal);
  return S_OK;
}
