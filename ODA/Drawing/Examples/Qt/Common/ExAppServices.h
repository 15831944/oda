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
// ExAppServices.h
//

#ifndef EX_APP_SERVICES_H_
#define EX_APP_SERVICES_H_

#include "TD_PackPush.h"

#include "SysVarPE.h"

#include "OdaDefs.h"

class ExAppServices : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(ExAppServices);

  virtual void executeCommand(const OdString& sCmd,
                              bool bEcho = true,
                              OdRxObject* pRxDatabase = NULL, // current
                              class OdVariant* pvRes = NULL) = 0;
  virtual void postponeCommand(const OdString& sCmd,
                               bool bEcho = true,
                               OdRxObject* pRxDatabase = NULL, // current
                               int timeStep = 0) = 0; // milliseconds
  
  virtual bool readRegistryValue(const OdString& sKey, 
				                         OdSysVarType typeRb, void* pValue) = 0;
  virtual void writeRegistryValue(const OdString& sKey, 
				                          OdSysVarType typeRb, const void* pValue) = 0;

  virtual bool isNetAssembly(OdString& sPath, // in / out(normalized as side effect)
                             OdString* psVersion = NULL) = 0;
};
#define EX_APP_SERVICES OD_T("ExAppServices")
typedef OdSmartPtr<ExAppServices> ExAppServicesPtr;

inline OdSysVarType toRbType(const bool&)
{
  return kRtBool;
}

inline OdSysVarType toRbType(const OdInt8&)
{
  return kRtInt8;
}

inline OdSysVarType toRbType(const OdUInt8&)
{
  return kRtInt8;
}

inline OdSysVarType toRbType(const OdInt16&)
{
  return kRtInt16;
}

inline OdSysVarType toRbType(const OdUInt16&)
{
  return kRtInt16;
}

inline OdSysVarType toRbType(const OdInt32&)
{
  return kRtInt32;
}

inline OdSysVarType toRbType(const OdUInt32&)
{
  return kRtInt32;
}

inline OdSysVarType toRbType(const OdDb::LineWeight&)
{
  return kRtInt32;
}

// moved into Qt/OdaQtApp/Services.cpp because of OdDb::ProxyImage is out of Kernel now
//inline OdSysVarType toRbType(const OdDb::ProxyImage&)
//{
//  return kRtInt32;
//}

inline OdSysVarType toRbType(const double&)
{
  return kRtDouble;
}

inline OdSysVarType toRbType(const OdString&)
{
  return kRtString;
}

#include "TD_PackPop.h"

#endif // EX_APP_SERVICES_H_
