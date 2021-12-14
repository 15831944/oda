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

#include "stdafx.h"
#include "OdaCommon.h"
#include "ExDrawingSignatureApp.h"

#include "DbDatabase.h"
#include "DbSecurity.h"

ExDrawingSignatureApp::ExDrawingSignatureApp()
{
}

ExDrawingSignatureApp::~ExDrawingSignatureApp()
{
}

OdResult ExDrawingSignatureApp::checkSignatureStatus(OdCryptoServices::OdSignatureVerificationResult& verificationResult, 
  OdSignatureDescription& signatureDesc)
{
  OdResult res;
  res = validateDrawingSignature(m_drawingFilePath, verificationResult, signatureDesc);
  return res;
}

OdResult ExDrawingSignatureApp::signDrawingFile(const OdString& certSubject,
  const OdString& certIssuer, const OdString& certSerialNum,
  bool addTimeStamp, const OdString& timeServer, const OdString& comment)
{
  if (accessFile(m_drawingFilePath, (Oda::kFileRead | Oda::kFileWrite)))
  {
    OdDbDatabasePtr pDb;
    pDb = readFile(m_drawingFilePath);

    OdSecurityParams secParam;
    secParam.sCertSubject = certSubject;
    secParam.sCertIssuer = certIssuer;
    secParam.sCertSerialNum = certSerialNum;
    secParam.nFlags = SECURITYPARAMS_SIGN_DATA;
    if (addTimeStamp)
    {
      secParam.nFlags = secParam.nFlags | SECURITYPARAMS_ADD_TIMESTAMP;
      secParam.sTimeServer = timeServer;
    }
    secParam.sComment = comment;

    pDb->setSecurityParams(secParam);

    OdStreamBufPtr pTargetFileBuf = createFile(m_drawingFilePath, (Oda::FileAccessMode)(Oda::kFileRead | Oda::kFileWrite), 
      Oda::kShareDenyReadWrite, Oda::kCreateAlways);

    pDb->writeFile(pTargetFileBuf, OdDb::kDwg, OdDb::kDHL_CURRENT);
  }
  else
  {
    throw OdError_CantOpenFile(m_drawingFilePath);
  }

  return eOk;
}
