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

#include "OdaCommon.h"
#include "StaticRxObject.h"
#include "ExSystemServices.h"
#include "ExHostAppServices.h"
#include "DynamicLinker.h"
#include "diagnostics.h"
#include "RxDynamicModule.h"

#include "DbDatabase.h"
#include "DbSecurity.h"

#define STL_USING_IOSTREAM
#include "OdaSTL.h"
#define  STD(a)  std:: a

#ifdef OD_HAVE_CONSOLE_H_FILE
#include <console.h>
#endif

#define MAX_PATH_LENGTH 1024

class MyServices : public ExSystemServices, public ExHostAppServices
{
protected:
  ODRX_USING_HEAP_OPERATORS(ExSystemServices);

private:

  /**********************************************************************/
  /* Define a console interface for password support.                   */
  /**********************************************************************/
  bool getPassword(const OdString& dwgName, bool /*isXref*/, OdPassword& password)
  {
    OdAnsiString pwdBuff;
    odPrintConsoleString(L"Enter password to open drawing: %ls\n", dwgName.c_str());
    scanf("%s", (char*)pwdBuff.getBuffer(MAX_PATH_LENGTH));
    pwdBuff.releaseBuffer();
    password = pwdBuff;
    password = password.makeUpper();
    return !password.isEmpty();
  }
};

#ifndef _TOOLKIT_IN_DLL_
    ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(DbCryptModule);
    ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdRecomputeDimBlockModule);
#ifdef ODA_WINDOWS
    ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(ExTimeProviderWin);
#else
    ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(ExTimeProvider);
#endif

    ODRX_BEGIN_STATIC_MODULE_MAP()
      ODRX_DEFINE_STATIC_APPMODULE(DbCryptModuleName,            DbCryptModule)
      ODRX_DEFINE_STATIC_APPLICATION(OdRecomputeDimBlockModuleName, OdRecomputeDimBlockModule)
#ifdef ODA_WINDOWS
      ODRX_DEFINE_STATIC_APPMODULE(OdTimeProviderModuleName, ExTimeProviderWin)
#else
      ODRX_DEFINE_STATIC_APPMODULE(OdTimeProviderModuleName, ExTimeProvider)
#endif
    ODRX_END_STATIC_MODULE_MAP()
#endif

void printCertData(const OdCertificateDescription& certData);


static const char cUsage[] =
{
  "OdSignEx sample program. Copyright (C) " TD_COPYRIGHT_START_YEAR_S  TD_COPYRIGHT_END_S "\n"
  "Usage: OdSignEx filepath\n"
  "  filepath - full path to .dwg file.\n"
  "\nPress ENTER to continue...\n"
};


#if defined(OD_USE_WMAIN)
int wmain(int argc, wchar_t* argv[])
#else
int main(int argc, char* argv[])
#endif
{
#ifdef OD_HAVE_CCOMMAND_FUNC
  argc = ccommand(&argv);
#endif

  if (argc < 2) 
  {
    STD(cout) << cUsage;
    STD(cin).get();
    return 1;
  }

#ifndef _TOOLKIT_IN_DLL_
  ODRX_INIT_STATIC_MODULE_MAP();
#endif

  /**********************************************************************/
  /* Initialize Runtime Extension environment                           */
  /**********************************************************************/
  OdStaticRxObject<MyServices> svcs;
  odInitialize(&svcs);

  //load DbCryptModule:
  ::odrxDynamicLinker()->loadModule(DbCryptModuleName);
  
  try
  {
    OdRxClassPtr pService = odrxServiceDictionary()->getAt(OD_T("OdCryptoServices"));
    if (!pService.isNull())
    {
      OdCryptoServicesPtr pCryptoServices = pService->create();

      OdString filePath(argv[1]);

      if (!svcs.accessFile(filePath, Oda::kFileRead))
      {
        OdPrintf("\nError: Can't access the file specified!\n");
      }
      else
      {
        OdAnsiString opNumber;
        int caseCode = 0;
        bool exitFlag = false;
        do {
          OdPrintf("\n\nInput the number of the operation you want to perform and press Enter:\n");
          OdPrintf("1 - to validate signature;\n");
          OdPrintf("2 - to sign;\n");          
          OdPrintf("3 - to exit;\n");
          scanf("%s", (char*)opNumber.getBuffer(MAX_PATH_LENGTH));
          opNumber.releaseBuffer();
          caseCode = atol(opNumber.c_str());
          switch (caseCode)
          {
          case 1://VALIDATION:
            {
              OdCryptoServices::OdSignatureVerificationResult validationRes;
              OdSignatureDescription sigData;

              OdResult res;
              res = validateDrawingSignature(filePath, validationRes, sigData);
              if (res == eOk)
              {
                OdString resultMessage;
                switch (validationRes)
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

                OdString fullMessage;
                fullMessage = resultMessage + L"\n\n" + 
                  L"Digital ID (Certificate) info:\n\n" +
                  L"Subject\t\t=\t" + sigData.m_certDesc.m_CertSubject + 
                  L"\nIssuer\t\t=\t" + sigData.m_certDesc.m_CertIssuer +
                  L"\nSerial number\t=\t" + sigData.m_certDesc.m_CertSerialNum +        
                  L"\nValid from\t\t=\t" + sigData.m_certDesc.m_CertValidFrom +
                  L"\nValid to\t\t=\t" + sigData.m_certDesc.m_CertValidTo +
                  L"\n\nSignature attributes:\n\n" +
                  L"Date and time\t=\t" + sigData.m_signatureDateAndTime +
                  L"\nTime service used\t=\t" + sigData.m_timeServiceUsed +
                  L"\nComment\t\t=\t" + sigData.m_comment;

                OdPrintf(fullMessage);
                OdPrintf("\n");
              }
              else if (res == eCantOpenFile)
              {
                OdPrintf("\nCan't open file!\n");
              }
              else
              {
                OdPrintf("\nCan't check signature for this file format version.\n");
              }
            }          
            break;
          case 2://SIGNING:
            {
              //init suitable certificates list:
              OdArray<OdCertificateDescription> certDescArray;
              int certsNumber = 0;
              certsNumber = pCryptoServices->getPersonalCertsWithTrustedStatus(certDescArray);
              if (certsNumber)
              {    	
                OdPrintf("Certificate list:\n");
                for (int i = 0; i < certsNumber; i++)
                {
                  OdPrintf("[%d] ", i + 1);
                  printCertData(certDescArray[i]);
                }
                OdAnsiString certIndex;
                OdPrintf("Select a certificate by its number in the list:\n");
                scanf("%s", (char*)certIndex.getBuffer(MAX_PATH_LENGTH));
                certIndex.releaseBuffer();
                int nSelectedCert = atol(certIndex.c_str());
                if ((nSelectedCert > 0)&&(nSelectedCert <= certsNumber))
                {
                  OdSecurityParams secParam;

                  OdAnsiString commentString;
                  OdPrintf("Input comment for signature:\n");
                  scanf("%s", (char*)commentString.getBuffer(MAX_PATH_LENGTH));
                  commentString.releaseBuffer();                  
                  OdString commentAsOdString(commentString);

                  //set comment:
                  secParam.sComment = commentAsOdString;

                  if (svcs.accessFile(filePath, (Oda::kFileRead | Oda::kFileWrite)))
                  {
                    OdDbDatabasePtr pDb;
                    pDb = svcs.readFile(filePath);
                    
                    secParam.sCertSubject = certDescArray[nSelectedCert - 1].m_CertSubject;
                    secParam.sCertIssuer = certDescArray[nSelectedCert - 1].m_CertIssuer;
                    secParam.sCertSerialNum = certDescArray[nSelectedCert - 1].m_CertSerialNum;
                    secParam.nFlags = SECURITYPARAMS_SIGN_DATA|SECURITYPARAMS_ADD_TIMESTAMP;

                    /*
                    //to try other params while signing:
                    secParam.sTimeServer = OD_T("time-a-g.nist.gov");

                    secParam.nFlags = secParam.nFlags|SECURITYPARAMS_ENCRYPT_DATA|SECURITYPARAMS_ENCRYPT_PROPS;
                    secParam.password = commentAsOdString;

                    secParam.nProvType = 0xd;
                    secParam.provName = OD_T("Microsoft Base DSS and Diffie-Hellman Cryptographic Provider");
                    */
                    
                    pDb->setSecurityParams(secParam);

                    OdStreamBufPtr pTargetFileBuf = svcs.createFile(filePath, (Oda::FileAccessMode)(Oda::kFileRead | Oda::kFileWrite), 
                      Oda::kShareDenyReadWrite, Oda::kCreateAlways);

                    //pDb->writeFile(pTargetFileBuf, OdDb::kDwg, OdDb::kDHL_CURRENT);
                    //
                    pDb->writeFile(pTargetFileBuf, OdDb::kDwg, OdDb::vAC27);
                    //
                  }
                  else
                  {
                    OdPrintf("\nCan't open file to read/write!\n");
                  }
                }
                else
                {
                  OdPrintf("Wrong input!\n");
                }
              }
              else
              {
                OdPrintf("\nNo valid digital ID (Certificate) is available on your system!\n");
              }
            }
            break;
          case 3://EXIT:
            exitFlag = true;
            break;
          default:
            OdPrintf("\nWrong input!\n");
            break;
          }
        } while (!exitFlag);
      }      
    }
    else
    {
      OdPrintf("\nCan't init CryptoServices!\n");
    }
  }
  catch (OdError& e)
  {
    OdPrintf("Exception (%ls) during the file processing!\n", e.description().c_str());    
  }
  catch (...)
  {
    OdPrintf("Unknown Exception during the file processing!\n");
  }

  /**********************************************************************/
  /* Uninitialize Runtime Extension environment                         */
  /**********************************************************************/
  ::odUninitialize();

	return 0;
}

void printCertData(const OdCertificateDescription& certData)
{  
  OdPrintf("Subject = %ls, Issuer = %ls, Serial number = %ls, Expiration date = %ls\n", 
    certData.m_CertSubject.c_str(), certData.m_CertIssuer.c_str(), 
    certData.m_CertSerialNum.c_str(), certData.m_CertValidTo.c_str());
}
