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

#include "StdAfx.h"

#include "StringArray.h"
#include "OdCryptoServices/OdCryptoServices.h"

void _DIGITALSIGN_func(OdEdCommandContext* pCmdCtx)
{
#ifdef _MSC_VER
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();

  OdRxClassPtr pService = odrxServiceDictionary()->getAt(OD_T("OdCryptoServices"));
  if (!pService.isNull())
  {
    OdCryptoServicesPtr pCryptoServices = pService->create();

    //get suitable certificates list:
    OdArray<OdCertificateDescription> certDescArray;
    int certsNumber = 0;
    certsNumber = pCryptoServices->getPersonalCertsWithTrustedStatus(certDescArray);
    if (certsNumber)
    {
      //get current settings for digital signature:
      OdSecurityParams secParam;
      pDb->securityParams(secParam);
      bool attachDigitalSignature = static_cast<bool>(secParam.nFlags & SECURITYPARAMS_SIGN_DATA);
      if (attachDigitalSignature)
      {
        pIO->putString(OD_T("The next certificate selected as current: "));
        OdString currentSelectedCertData;
        currentSelectedCertData.format(OD_T("Subject = %ls, Issuer = %ls, Serial number = %ls\n"), 
          secParam.sCertSubject.c_str(), secParam.sCertIssuer.c_str(), secParam.sCertSerialNum.c_str());
        pIO->putString(currentSelectedCertData);
        
        pIO->putString(OD_T("\nThe following parameters for timestamp and comment are set:\n"));
        if (secParam.nFlags & SECURITYPARAMS_ADD_TIMESTAMP)
        {
          pIO->putString(OD_T("Timeserver = "));
          pIO->putString(secParam.sTimeServer);
        }
        else
          pIO->putString(OD_T("No timestamp."));
        if (!secParam.sComment.isEmpty())
        {
          pIO->putString(OD_T("\nComment = "));
          pIO->putString(secParam.sComment);
        }
        else
          pIO->putString(OD_T("No comment."));
      }

      OdString promptString;
      promptString.format(OD_T("Attach digital signature after saving? [Y/N] <%ls>"), 
        attachDigitalSignature ? OD_T("Y") : OD_T("N"));
      OdUInt32 nKeyword = pIO->getKeyword(promptString, OD_T("Y N"), 
        attachDigitalSignature ? 0 : 1);

      switch (nKeyword)
      {
      case 0:
        {
          OdUInt32 nNoChange = 0;
          if (attachDigitalSignature)
          {
            nNoChange = pIO->getKeyword(OD_T("Change signature settings? [Y/N] <N>"), OD_T("Y N"), 1);
          }
          if (!nNoChange)
          {
            //certificate selection:
            OdString keyWords;
            pIO->putString(OD_T("Certificate list:\n"));
            for (int i = 0; i < certsNumber; i++)
            {
              OdString certDataString;
              certDataString.format(OD_T("[%d] Subject = %ls; Issuer = %ls; Expiration date = %ls\n"), 
                i + 1, certDescArray[i].m_CertSubject.c_str(), certDescArray[i].m_CertIssuer.c_str(),
                certDescArray[i].m_CertValidTo.c_str());

              pIO->putString(certDataString);

              OdString certNumInList;
              certNumInList.format(OD_T("%d"), i + 1);
              if (keyWords.isEmpty())
                keyWords = certNumInList;
              else
                keyWords = keyWords + OD_T(" ") + certNumInList;
            }

            OdString promptString;
            promptString.format(OD_T("Select a certificate by its number in the list [%ls] <1>"), 
              keyWords.c_str());
            OdUInt32 nSelectedCert = pIO->getKeyword(promptString, keyWords, 0);
            
            //Select timeserver if needed:
            OdString timeServerString;
            OdUInt32 noTimeStamp = 0;
            noTimeStamp = pIO->getKeyword(OD_T("Add timestamp? [Y/N] <N>"), OD_T("Y N"), 1);
            if (!noTimeStamp)
            {
              pIO->putString(OD_T("List of timeservers available to use:\n"));

              OdArray<OdString> timeServersDescArray;
              timeServersDescArray.append(OD_T("Local computer"));
              timeServersDescArray.append(OD_T("time-a-g.nist.gov"));

              OdString keyWordsForTimeserver;
              for (unsigned int i = 0; i < timeServersDescArray.size(); i++)
              {
                pIO->putString(timeServersDescArray[i]);

                OdString serverNumInList;
                serverNumInList.format(OD_T("%d"), i + 1);
                if (keyWordsForTimeserver.isEmpty())
                  keyWordsForTimeserver = serverNumInList;
                else
                  keyWordsForTimeserver = keyWordsForTimeserver + OD_T(" ") + serverNumInList;
              }
              
              OdString timeServerPromptString;
              timeServerPromptString.format(OD_T("Select a timeserver by its number in the list [%ls] <1>"), 
                keyWordsForTimeserver.c_str());
              OdUInt32 nSelectedTimeserver = pIO->getKeyword(timeServerPromptString, keyWordsForTimeserver, 0);
              if (nSelectedTimeserver > 0)
                timeServerString = timeServersDescArray[nSelectedTimeserver];

              secParam.nFlags = secParam.nFlags | SECURITYPARAMS_ADD_TIMESTAMP;
            }
            else
            {
              secParam.nFlags = secParam.nFlags & (~SECURITYPARAMS_ADD_TIMESTAMP);
            }

            //Add comment if needed:
            OdString commentString;
            OdUInt32 noComment = 0;
            noComment = pIO->getKeyword(OD_T("Add comment? [Y/N] <N>"), OD_T("Y N"), 1);
            if (!noComment)
            {              
              commentString = pIO->getString(OD_T("Input a comment string:\n"));
            }

            //set security params:
            secParam.sCertSubject = certDescArray[nSelectedCert].m_CertSubject;
            secParam.sCertIssuer = certDescArray[nSelectedCert].m_CertIssuer;
            secParam.sCertSerialNum = certDescArray[nSelectedCert].m_CertSerialNum;
            secParam.nFlags = secParam.nFlags | SECURITYPARAMS_SIGN_DATA;

            secParam.sTimeServer = timeServerString;
            secParam.sComment = commentString;

            pDb->setSecurityParams(secParam);
          }        
        }
        break;
      case 1:
        {
          secParam.nFlags = secParam.nFlags & (~SECURITYPARAMS_SIGN_DATA);
          pDb->setSecurityParams(secParam);
        }
        break;
      default:
        break;
      }
    }
    else
    {
      pIO->putString(OD_T("No valid digital ID (Certificate) is available on your system!\n"));
    }
  }
  else
  {
    pIO->putString(OD_T("OdCryptoServices have not been initialized to process signatures!"));
  }  
#endif
}

void _SIGVALIDATE_func(OdEdCommandContext* pCmdCtx)
{
#ifdef _MSC_VER
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();
    
  OdString drawingPath = pDb->getFilename();
  if (!drawingPath.isEmpty())
  {
    OdResult res;
    OdCryptoServices::OdSignatureVerificationResult verifResult;
    OdSignatureDescription sigDescription;
    res = validateDrawingSignature(drawingPath, verifResult, sigDescription);
    if (res == eOk)
    {
      OdString resultMessage;
      switch (verifResult)
      {
      case OdCryptoServices::kSuccess:
        resultMessage = L"The digital signature is valid.";
        break;
      case OdCryptoServices::kHasNoSignature:
        resultMessage = L"Drawing has no signature.";
        break;
      case OdCryptoServices::kBadSignature:
        resultMessage = L"Bad signature!";
        break;
      case OdCryptoServices::kCertificateChainProblem:
        resultMessage = L"Problem with one of the certificates in the path!";
        break;
      default:
        resultMessage = L"Error during verification!";
        break;
      }
      pIO->putString(resultMessage.makeUpper());

      if (verifResult != OdCryptoServices::kHasNoSignature)
      {
        pIO->putString(OD_T("\nCertificate info:\n"));
        pIO->putString(OD_T("Subject = ") + sigDescription.m_certDesc.m_CertSubject);
        pIO->putString(OD_T("Issuer = ") + sigDescription.m_certDesc.m_CertIssuer);
        pIO->putString(OD_T("Serial number = ") + sigDescription.m_certDesc.m_CertSerialNum);
        pIO->putString(OD_T("Valid from = ") + sigDescription.m_certDesc.m_CertValidFrom);
        pIO->putString(OD_T("Valid to = ") + sigDescription.m_certDesc.m_CertValidTo);

        pIO->putString(OD_T("\nSignature attributes:\n"));
        pIO->putString(OD_T("Signature date and time = ") + sigDescription.m_signatureDateAndTime);
        pIO->putString(OD_T("Time service used = ") + sigDescription.m_timeServiceUsed);
        pIO->putString(OD_T("Comment = ") + sigDescription.m_comment);
      }
    }
    else if (res == eCantOpenFile)
    {
      pIO->putString(OD_T("Can't open file!"));
    }
    else
    {
      pIO->putString(OD_T("Can't check signature for this file format version."));
    }
  }
  else
  {
    pIO->putString(OD_T("No digital signature. This drawing has not been saved."));
  }
#endif
}
