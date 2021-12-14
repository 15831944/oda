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

#ifndef _ODRXMETHOD_INCLUDED_
#define _ODRXMETHOD_INCLUDED_

#include "RxMember.h"

class OdRxMethod;
typedef OdSmartPtr<OdRxMethod>OdRxMethodPtr;

/** \details
  This class allows implementing "protocol extension-like" members, or late binding calls for scripting.
	
  <group OdRx_Classes>
*/
class ODRX_ABSTRACT FIRSTDLL_EXPORT OdRxMethod : public OdRxMember
{
public:
  ODRX_DECLARE_MEMBERS(OdRxMethod);
  /** \details
    Executes the underlying method of the object.
    
    \param pO [in] The object which method to call.
    \param param [in,out] Method parameter(s) wrapped in OdRxValue. If method needs to return a value it is assigned to the param.
    \remarks Parameter may be OdRxValue::empty() if no input is required.
    \returns Returns eOk if the call was successful.
  */
  OdResult invoke(OdRxObject* pO, OdRxValue& param) const;
  /** \details
    Returns eOk if the method can be executed with this parameter. 
  */
  OdResult isExecutable(const OdRxObject* pO, const OdRxValue& param) const;
protected:
  virtual OdResult subInvoke(OdRxObject* pO, OdRxValue& param) const = 0;
  virtual OdResult subIsExecutable(const OdRxObject* pO, const OdRxValue& param) const;
};

#endif 
