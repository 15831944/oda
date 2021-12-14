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
//
// SysVarPE.h
//

#ifndef OD_SYS_VAR_PE_H_
#define OD_SYS_VAR_PE_H_

#include "TD_PackPush.h"

#include "RxObject.h"
#include "StringArray.h"
#include "Ed/EdCommandContext.h"

//#if 0
//#include "ResBuf.h"
//typedef enum OdResBuf::ValueType OdSysVarType;
//#else

typedef enum
{
  // minimal part of enum OdResBuf::ValueType :
  kRtNone = 5000,
  kRtDouble = 5001,
  kRtInt16 = 5003,
  kRtString = 5005,
  kRtInt32 = 5010,
  kRtBool = 290,
  kRtInt8 = 280
} OdSysVarType;

//#endif

class OdSysVarPE : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdSysVarPE);

  // pRxObject - is OdDbBaseDatabase or OdEdCommandContext
  virtual bool getNames(OdRxObject* pRxObject, OdStringArray& names, bool bSkipHidden = true) = 0;
  virtual OdString valueToString(OdRxObject* pRxObject, const OdString& sName, 
                                 OdSysVarType* pRbType = NULL, bool* pbExist = NULL) = 0;
  virtual bool isEditable(OdRxObject* pRxObject, const OdString& sName, bool* pbExist = NULL) = 0;
  virtual bool editValue(OdEdCommandContext* pCmdCtx, const OdString& sName, bool* pbExist = NULL) = 0;
};
#define OD_SYS_VAR_PE L"OdSysVarPE"
typedef OdSmartPtr<OdSysVarPE> OdSysVarPEPtr;

#include "TD_PackPop.h"

#endif // OD_SYS_VAR_PE_H_
