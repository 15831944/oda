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

#if !defined(EXDRAWINGSIGNATUREAPP_H__F56FD889_AB13_4A21_B633_482615D820EF__INCLUDED_)
#define EXDRAWINGSIGNATUREAPP_H__F56FD889_AB13_4A21_B633_482615D820EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TD_PackPush.h"

#include "OdString.h"
#include "DbSecurity.h"
#include "ExSystemServices.h"
#include "ExHostAppServices.h"

class ExDrawingSignatureApp : public ExSystemServices, public ExHostAppServices
{
public:
  ExDrawingSignatureApp();
  virtual ~ExDrawingSignatureApp();

  void addRef() {}
  void release() {}

  void setServices(OdDbHostAppServices* pSvcs);

  OdString m_drawingFilePath;
  
  OdResult checkSignatureStatus(OdCryptoServices::OdSignatureVerificationResult& verificationResult, 
    OdSignatureDescription& signatureDesc);
  OdResult signDrawingFile(const OdString& certSubject, const OdString& certIssuer, const OdString& certSerialNum,
    bool addTimeStamp, const OdString& timeServer, const OdString& comment);
};

#include "TD_PackPop.h"

#endif
