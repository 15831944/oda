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


/************************************************************************/
/* This console application creates and populates DWG/DXF/DXB file of   */
/* the specified version.                                               */
/*                                                                      */
/* Calling sequence - see below                                         */
/*                                                                      */
/* The following files from the Examples\OdWriteEx folder are           */
/* referenced by <filename> and must either be in the same folder as    */
/* <filename> or in a folder in the support path                        */
/*                                                                      */
/*   OdWriteEx.jpg, OdWriteEx.sat, OdWriteEx XRef.dwg                   */
/*                                                                      */
/* In addition, the folder containing OdWriteEx.txt must be hard-coded  */
/* in DbFiller::addRText                                                */
/************************************************************************/

#define OD_TEST_PAGING
static const char cUsage[] =
{
  "\nUsage: OdWriteEx <filename> [OutVer] [OutType] [-DO] [-NoWait]"
#ifdef OD_TEST_PAGING
  " [UseUnloadPaging|UseFilePaging]"
#endif
"\n"
"    OutVer can be any of:\n"
"           ACAD12, ACAD13, ACAD14, ACAD2000, ACAD2004, ACAD2007\n"
"           ACAD2010, ACAD2013, ACAD2018\n"
"           (default is latest supported version)\n"
"    OutType can be any of DWG, DXF, DXB (binary DXF)\n"
"           (default is DWG)\n\n"
"    -DO disables progress meter output.\n"
"    -NoWait disables prompt of pressing ENTER at the end of execution.\n"
#ifdef OD_TEST_PAGING
"    UseUnloadPaging|UseFilePaging - sets type of paging\n"
#endif
};

#ifdef _MSC_VER
#define _WIN32_WINNT 0x0400 // to enable Windows Cryptographic API
#endif

#include "OdaCommon.h"
#include "DbDatabase.h"
#include "DbAudit.h"

#include "StaticRxObject.h"
#include "OdFileBuf.h"
#include "DbFiller.h"
#include "ExSystemServices.h"
#include "ExHostAppServices.h"
#include "DynamicLinker.h"
#include "ExPageController.h"
#include "summinfo.h"
#include "diagnostics.h"

#include "RxDynamicModule.h"
#include "Gs/Gs.h"

#include <math.h>
#include <locale.h>

#define STL_USING_IOSTREAM
#include "OdaSTL.h"
#define  STD(a)  std:: a

#include "RcsFileServices/RxRcsFileServices.h"

/************************************************************************/
/* Compile password support for R18 Drawings                            */
/************************************************************************/

#ifdef OD_HAVE_CONSOLE_H_FILE
#include <console.h>
#endif

/************************************************************************/
/* Freeze a local time                                                  */
/************************************************************************/
typedef void (*PgetLocalTime)( OdTimeStamp& ts );
extern FIRSTDLL_EXPORT_STATIC PgetLocalTime g_pLocalTimeFunc; // global variable defined in TD_Root
static void getLocalTime(OdTimeStamp &localTime)
{
  localTime.setDate(12, 22, 2009);
  localTime.setTime(18, 0, 0, 0);
}

/************************************************************************/
/* Define a Custom Services class.                                      */
/*                                                                      */
/* Combines the platform dependent functionality of                     */
/* ExSystemServices and ExHostAppServices                               */ 
/************************************************************************/
class MyServices : public ExSystemServices, public ExHostAppServices
{
protected:
  ODRX_USING_HEAP_OPERATORS(ExSystemServices);
  OdGsDevicePtr gsBitmapDevice(OdRxObject* /*pViewObj*/ = NULL,
                               OdDbBaseDatabase* /*pDb*/ = NULL,
                               OdUInt32 /*flags*/ = 0)
  {
    try
    {
      OdGsModulePtr pGsModule = ::odrxDynamicLinker()->loadModule(OdWinBitmapModuleName);
      return pGsModule->createBitmapDevice();
    }
    catch(const OdError&)
    {
    }
    return OdGsDevicePtr();
  }

#ifdef OD_TEST_PAGING
  OdDbPageControllerPtr newPageController()
  {
    switch (m_pagingType)
    {
    case 1: //OdDb::kUnload
      // Simple unloading of objects for partially loaded database.
      return OdRxObjectImpl<ExUnloadController>::createObject();
    case 2: //OdDb::kPage
    case 3: //OdDb::kUnload | OdDb::kPage
      // Unloading of objects for partially loaded database and paging of objects thru ExPageController.
      return OdRxObjectImpl<ExPageController>::createObject();
    }
    // Paging is not used.
    return (OdDbPageController*)0;
  }
  
  MyServices()
    : m_myDb(0)
    , m_pagingType(0)
  {}

  int  m_pagingType;
  long m_myMeterCurrent;
  long m_myMeterOld;
  long m_myMeterLimit;
public:
  OdDbDatabase* m_myDb;
  
  int setPagingType(int pagingType) 
  { 
    int oldType = m_pagingType;  
    m_pagingType = pagingType; 
    return oldType; 
  }

  void setLimit(int max)
  {
    m_myMeterLimit = max;
    m_myMeterCurrent = 0;
    m_myMeterOld = 0;
    ExHostAppServices::setLimit(max);
  }
  
  void meterProgress()
  {
    if (m_myDb)
    {
      m_myMeterCurrent++;
      double  f1, f2;

      f1 = (double)m_myMeterCurrent / m_myMeterLimit * 100;
      f2 = (double)m_myMeterOld / m_myMeterLimit * 100;
      if ((f1 - f2) > 0.7)
      {
        m_myDb->pageObjects();
        m_myMeterOld = m_myMeterCurrent;
      }
    }
    ExHostAppServices::meterProgress();
  }

#endif

};

/************************************************************************/
/* Define an a Custom OdDbAuditInfo class                               */
/************************************************************************/
class AuditInfo : public OdDbAuditInfo
{
private:
  OdRxObjectImpl<ExSystemServices> m_FileServ;
  OdStreamBufPtr m_ReportFile;
  bool m_isSomething;
  OdString m_ReportFileName;
  
public:
  AuditInfo(OdString reportFileName)
    : m_isSomething(false),
      m_ReportFileName(reportFileName)
  {
    /********************************************************************/
    /* Create a report file                                             */
    /********************************************************************/
    m_ReportFile = m_FileServ.createFile(m_ReportFileName, 
      Oda::kFileWrite, Oda::kShareDenyNo, Oda::kCreateAlways);
  }
  virtual ~AuditInfo() 
  {
    /********************************************************************/
    /* Refer user to OD_T("Audit Report.txt") if error(s) have been output.   */
    /********************************************************************/
    if (m_isSomething)
    {
      odPrintConsoleString(L"\n\nAudit error : Check \"%ls\" for errors.", m_ReportFileName.c_str());
      //odPrintConsoleString(L"\n\nPress ENTER to continue...");
      //STD(cin).get();
    }
    else
    {
      odPrintConsoleString(L"\n\nAudit : ok.\n");
    }
  }

public:

  /**********************************************************************/
  /* Print error to ReportFile                                          */
  /**********************************************************************/
  virtual void printError(const OdString& strName, const OdString& strValue, 
                          const OdString& strValidation, const OdString& strDefaultValue)
  {
    m_isSomething = true;
    OdString str;
    str.format(OD_T("%ls %ls %ls %ls\n"), strName.c_str(), strValue.c_str(), strValidation.c_str(), strDefaultValue.c_str());
    m_ReportFile->putBytes((const char*)str, str.getLengthA());
  }

  TD_USING(OdDbAuditInfo::printError);

  /**********************************************************************/
  /* Print info to ReportFile                                           */
  /**********************************************************************/
  virtual void printInfo (const OdString& strInfo)
  {
    OdString str;
    str.format(OD_T("%ls\n"), strInfo.c_str());
    m_ReportFile->putBytes((const char*)str, str.getLengthA());
  }
};

#ifdef _MSC_VER
/************************************************************************/
/* Set the ACAD environment variable to the AutoCAD support path        */
/************************************************************************/
static bool findAcadInProfile(OdString& supportPath)
{
#define  ACAD_KEY_STRING  L"Software\\Autodesk\\AutoCAD"
  LONG        nRet;
  HKEY        rkAcadKey;
  DWORD       lType;
  DWORD       cbData = 1024;
  BYTE        baData[1024];
  
  /********************************************************************/
  /* Open the Registry key Software\Autodesk\AutoCAD                  */
  /********************************************************************/
  if (ERROR_SUCCESS != RegOpenKeyExW(HKEY_CURRENT_USER, ACAD_KEY_STRING, 
                                  0, KEY_QUERY_VALUE, &rkAcadKey))
    return false;

  /********************************************************************/
  /* Get the current AutoCAD Version                                  */
  /********************************************************************/
  nRet = RegQueryValueExW(rkAcadKey, L"CurVer", NULL, &lType, baData, &cbData);
  RegCloseKey(rkAcadKey);
  if (ERROR_SUCCESS != nRet)
    return false;

  OdString sTmp = ACAD_KEY_STRING;
  sTmp += L'\\';
  sTmp += (LPCTSTR)baData;
  
  /******************************************************************/
  /* The key is in the form                                         */
  /* HKEY_CURRENT_USER\Software\Autodesk\AutoCAD\Rxx.x\             */
  /******************************************************************/
  if (ERROR_SUCCESS != RegOpenKeyExW(HKEY_CURRENT_USER, sTmp, 
                                0, KEY_QUERY_VALUE, &rkAcadKey))
    return false;

  cbData = 1024;
  nRet = RegQueryValueExW(rkAcadKey, L"CurVer", NULL, &lType, baData, &cbData);
  RegCloseKey(rkAcadKey);
  if (ERROR_SUCCESS != nRet)
    return false;

  sTmp += OD_T("\\");
  sTmp += (const wchar_t*)baData;
  sTmp += OD_T("\\Profiles\\<<Unnamed Profile>>\\General");

  /******************************************************************/
  /* The key is in the form                                         */
  /* HKEY_CURRENT_USER\Software\Autodesk\AutoCAD\Rxx.x\             */
  /* ACAD-xxx:xxx\Profiles\<<Unnamed Profile>>\General              */
  /******************************************************************/
  if (ERROR_SUCCESS != RegOpenKeyExW(HKEY_CURRENT_USER, sTmp, 
                                0, KEY_QUERY_VALUE, &rkAcadKey))
    return false;

  cbData = 1024;
  nRet = RegQueryValueExW(rkAcadKey, L"ACAD", NULL, &lType, baData, &cbData);
  RegCloseKey(rkAcadKey);
  if (ERROR_SUCCESS != nRet)
    return false;

  /**********************************************************************/
  /* We now have the Support Path                                       */
  /**********************************************************************/
  supportPath = (LPCTSTR)baData;
  /**********************************************************************/
  /* Set ACAD environment variable                                      */
  /**********************************************************************/
  OdString  sEnv(OD_T("ACAD="));
  sEnv += supportPath;
  putenv(sEnv);
  return true;
}
#endif
/************************************************************************/
/* Set the ACAD environment variable from the AutoCAD profile or the    */ 
/*the Windows Application path                                          */
/************************************************************************/
static bool  tryToFindAcad(void)
{
#ifdef _MSC_VER
  if (!getenv("ACAD"))
  {
    OdString    supportPath;
    LONG        nRet;
    HKEY        rkAcadKey;
    DWORD       lType;
    DWORD       cbData = 1024;
    BYTE        baData[1024];
    
    if (!findAcadInProfile(supportPath))
    {
      /********************************************************************/
      /* Try the Windows Application Path                                 */
      /********************************************************************/
      if (ERROR_SUCCESS != RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\"
                                        L"Windows\\CurrentVersion\\App Paths\\acad.exe", 
                                        0, KEY_QUERY_VALUE, &rkAcadKey))
        return false;

      cbData = 1024;
      nRet = RegQueryValueExW(rkAcadKey, L"Path", NULL, &lType, baData, &cbData);
      RegCloseKey(rkAcadKey);
      if (ERROR_SUCCESS != nRet)
        return false;

      /******************************************************************/
      /* We now have the Support Path                                   */
      /******************************************************************/
      supportPath = (LPCTSTR)baData;
      /******************************************************************/
      /* Set ACAD environment variable                                  */
      /******************************************************************/
      OdString  sEnv(OD_T("ACAD="));
      sEnv += supportPath;
      putenv(sEnv);
    }
  }
#endif
  return true;
}

// Define module map for statically linked modules:
#if !defined(_TOOLKIT_IN_DLL_)

ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(ModelerModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(BitmapModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdRasterProcessingServicesImpl);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdRecomputeDimBlockModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(ExFieldEvaluatorModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(TD_DynBlocksModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(DbPointCloudObjModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(GeolocationObjModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdGeoDataModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdDbGeoMapPEModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(ExRasterModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(DbCryptModule);
#if(POINTCLOUD_ENABLED)
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(RcsFileServicesModule);
#endif

ODRX_BEGIN_STATIC_MODULE_MAP()
  ODRX_DEFINE_STATIC_APPLICATION(OdModelerGeometryModuleName,   ModelerModule)
  ODRX_DEFINE_STATIC_APPMODULE  (OdWinBitmapModuleName,         BitmapModule)
  ODRX_DEFINE_STATIC_APPLICATION(OdRasterProcessorModuleName,   OdRasterProcessingServicesImpl)
  ODRX_DEFINE_STATIC_APPLICATION(OdRecomputeDimBlockModuleName, OdRecomputeDimBlockModule)
  ODRX_DEFINE_STATIC_APPLICATION(OdExFieldEvaluatorModuleName,  ExFieldEvaluatorModule)
  ODRX_DEFINE_STATIC_APPLICATION(DbPointCloudObjModuleName,     DbPointCloudObjModule)
  ODRX_DEFINE_STATIC_APPLICATION(OdGeolocationObjModuleName,    GeolocationObjModule)
  ODRX_DEFINE_STATIC_APPLICATION(OdGeoDataModuleName,           OdGeoDataModule)
  ODRX_DEFINE_STATIC_APPLICATION(OdGeoMapPEModuleName,          OdDbGeoMapPEModule)
  ODRX_DEFINE_STATIC_APPMODULE(RX_RASTER_SERVICES_APPNAME,      ExRasterModule)
  ODRX_DEFINE_STATIC_APPMODULE(DbCryptModuleName,               DbCryptModule)
  ODRX_DEFINE_STATIC_APPMODULE(OdDynBlocksModuleName,           TD_DynBlocksModule)
#if(POINTCLOUD_ENABLED)
  ODRX_DEFINE_STATIC_APPMODULE(RX_RCS_FILE_SERVICES, RcsFileServicesModule)
#endif
  ODRX_END_STATIC_MODULE_MAP()

#endif

/********************************************************************************/
/* Define Assert function to not crash Debug application if assertion is fired. */
/********************************************************************************/
static void MyAssert(const char* expression, const char* fileName, int nLineNo)
{
  OdString message;
  message.format(L"\n!!! Assertion failed: \"%s\"\n    file: %ls, line %d\n", OdString(expression).c_str(), OdString(fileName).c_str(), nLineNo);
  odPrintConsoleString(message);
}


/************************************************************************/
/* Main                                                                 */
/************************************************************************/
#if defined(OD_USE_WMAIN)
int wmain(int argc, wchar_t* argv[])
#else
int main(int argc, char* argv[])
#endif
{
  int nRes = 0; // Return value for main

  /**********************************************************************/
  /* Overload OdTimeStamp::getLocalTime internal implementation         */
  /**********************************************************************/
  g_pLocalTimeFunc = getLocalTime;

  /**********************************************************************/
  /* Set customized assert function                                     */
  /**********************************************************************/
  odSetAssertFunc(MyAssert);

  /**********************************************************************/
  /* Set command line defaults                                          */
  /**********************************************************************/
  OdDb::SaveType fileType = OdDb::kDwg;
  OdDb::DwgVersion outVer = OdDb::kDHL_CURRENT;
  bool disableOutput = false;
  bool disableWait = false;
 
#ifdef OD_HAVE_CCOMMAND_FUNC
  argc = ccommand(&argv);
#endif

  setlocale(LC_TIME, ""); // set current user locale (not OD_T("C")), for strftime

  /**********************************************************************/
  /* Create a Services object                                           */
  /**********************************************************************/
  OdStaticRxObject<MyServices> svcs;

  /**********************************************************************/
  /* Display the Product and Version that created the executable        */
  /**********************************************************************/
  odPrintConsoleString(L"\nOdWriteEx developed using %ls ver %ls", 
    svcs.product().c_str(), svcs.versionString().c_str());

  /**********************************************************************/
  /* Display MBCS or UNICODE project                                    */
  /**********************************************************************/
#if defined(_MBCS)
  odPrintConsoleString(L"\nMulti-Byte Character Set");
#else
  odPrintConsoleString(L"\nUnicode Character Set");
#endif
  /**********************************************************************/
  /* Parse Command Line inputs                                          */
  /**********************************************************************/
  bool bInvalidArgs = (argc < 2);
  if (bInvalidArgs)
  {
    bInvalidArgs = true;
    nRes  = 1;
  }

  /**********************************************************************/
  /* Set file version                                                   */
  /**********************************************************************/
  if (argc >= 3)
  {
    OdString argv2(argv[2]); // for Unix Unicode
    if (!odStrICmp(argv2,OD_T("ACAD12")))        outVer = OdDb::vAC12; // R11-12
    else if (!odStrICmp(argv2,OD_T("ACAD13")))   outVer = OdDb::vAC13; // R13
    else if (!odStrICmp(argv2,OD_T("ACAD14")))   outVer = OdDb::vAC14; // R14
    else if (!odStrICmp(argv2,OD_T("ACAD2000"))) outVer = OdDb::vAC15; // 2000-2002
    else if (!odStrICmp(argv2,OD_T("ACAD2004"))) outVer = OdDb::vAC18; // 2004-2006
    else if (!odStrICmp(argv2,OD_T("ACAD2007"))) outVer = OdDb::vAC21; // 2007
    else if (!odStrICmp(argv2,OD_T("ACAD2010"))) outVer = OdDb::vAC24; // 2010
    else if (!odStrICmp(argv2,OD_T("ACAD2013"))) outVer = OdDb::vAC27; // ACAD2013
    else if (!odStrICmp(argv2,OD_T("ACAD2018"))) outVer = OdDb::vAC32; // ACAD2018
    else
    {
      bInvalidArgs = true;
      nRes = 1;
    }
  }

  /**********************************************************************/
  /* Set file type                                                      */
  /**********************************************************************/
  if (argc >= 4)
  {
    OdString argv3(argv[3]);
    if (!odStrICmp(argv3,OD_T("DWG"))) fileType = OdDb::kDwg;
    else if (!odStrICmp(argv3,OD_T("DXF"))) fileType=OdDb::kDxf;
    else if (!odStrICmp(argv3,OD_T("DXB"))) fileType=OdDb::kDxb;
    else
    {
      bInvalidArgs = true;
      nRes  = 1;
    }
  }

  /**********************************************************************/
  /* Disable progress meter                                             */
  /**********************************************************************/
  if (argc >= 5)
  {
    OdString argv4(argv[4]);
    if (!odStrICmp(argv4, OD_T("-DO")))
    {
      disableOutput = true;
    }
  }
  
  /**********************************************************************/
  /* Disable prompt of pressing ENTER                                   */
  /**********************************************************************/
  for(int i=4; i<argc; ++i)
  {
    if (!odStrICmp(OdString(argv[i]), OD_T("-NoWait")))
    {
		  disableWait = true;
      break;
    }
  }

#ifdef OD_TEST_PAGING
  /**********************************************************************/
  /* Set type of paging controller                                      */
  /**********************************************************************/
  for(int i=4; i<argc; ++i)
  {
    if (!odStrICmp(OdString(argv[i]), OD_T("UseUnloadPaging")))
    {
      svcs.setPagingType(OdDb::kUnload);
      break;
    }
    else if (!odStrICmp(OdString(argv[i]), OD_T("UseFilePaging")))
    {
      svcs.setPagingType(OdDb::kUnload|OdDb::kPage);
      break;
    }
  }
#endif

  if (bInvalidArgs)    
  {
    STD(cout) << cUsage;
    STD(cout) << "\nPress Enter to continue...\n";
    STD(cin).get();
    return nRes;
  }

  tryToFindAcad();


  /************************************************************************/
  /* Disable/Enable progress meter                                       */
  /************************************************************************/
  svcs.disableOutput(disableOutput);

#if !defined(_TOOLKIT_IN_DLL_)
  ODRX_INIT_STATIC_MODULE_MAP();
#endif

  /**********************************************************************/
  /* Initialize the Drawings SDK                                        */
  /**********************************************************************/
  odInitialize(&svcs);
  
  ::odrxDynamicLinker()->loadApp(OdExFieldEvaluatorModuleName).release();
  //::odrxDynamicLinker()->loadApp(DbPointCloudObjModuleName).release();

  /**********************************************************************/
  /* Initialize Encription/Decription                        */
  /**********************************************************************/
  ::odrxDynamicLinker()->loadModule(DbCryptModuleName);

  /**********************************************************************/
  /* Support of modeling operations                                    */
  /**********************************************************************/
  ::odrxDynamicLinker()->loadModule(OdModelerGeometryModuleName, false);

  /**********************************************************************/
  /* Find the folder of the output file                                 */
  /**********************************************************************/
  OdString outputFolder(argv[1]);
#if defined(_WIN32) || defined(_WIN64)
  int n = outputFolder.reverseFind('\\');
#else
  int n = outputFolder.reverseFind('/');
#endif
  outputFolder = outputFolder.left(n+1);
   
  try
  {
    /********************************************************************/
    /* Create a default OdDbDatabase object                             */
    /********************************************************************/
    OdString f(argv[1]); // for UNIX UNICODE support
    OdWrFileBuf fb(f.c_str());
    // Create & initialize a default database.
    OdDbDatabasePtr pDb = svcs.createDatabase(true, OdDb::kEnglish);
#ifdef OD_TEST_PAGING
    svcs.m_myDb = pDb;
#endif

    OdDbDatabaseSummaryInfoPtr pSumInfo = oddbGetSummaryInfo(pDb);
    pSumInfo->setComments(OdString(OD_T("File was created by OdWriteEx")));
    pSumInfo->setTitle(OD_T("output of OdWriteEx"));
    pSumInfo->setSubject(OD_T("test file"));
    pSumInfo->setAuthor(OD_T("Open Design Alliance Copyright (C) 2002-2021 by Open Design Alliance"));

    OdString newSIKey("CustomProperty key");
    OdString newSIValue("CustomProperty value");
    pSumInfo->addCustomSummaryInfo(newSIKey, newSIValue);
    oddbPutSummaryInfo(pSumInfo);

    /********************************************************************/
    /* Fill the database                                                */
    /********************************************************************/
    DbFiller filler;
    filler.fillDatabase(pDb);

    /********************************************************************/
    /* Audit the database                                               */
    /********************************************************************/
    AuditInfo aiAppAudit(outputFolder + OD_T("AuditReport.txt"));
    aiAppAudit.setFixErrors(true);
    aiAppAudit.setPrintDest(OdDbAuditInfo::kBoth);
    odPrintConsoleString(L"\n\n");
    pDb->auditDatabase(&aiAppAudit);
    
#ifdef _MSC_VER
    {
      OdSecurityParams securityParams;
      securityParams.password = L"Teigha";
      securityParams.nFlags = 1;
      securityParams.nProvType = 0xd;
 
      // OD_T("Microsoft Base DSS and Diffie-Hellman Cryptographic Provider")
      static wchar_t pn[] =
      {
        0x4D, 0x69, 0x63, 0x72, 0x6F,
        0x73, 0x6F, 0x66, 0x74, 0x20,
        0x42, 0x61, 0x73, 0x65, 0x20,
        0x44, 0x53, 0x53, 0x20, 0x61,
        0x6E, 0x64, 0x20, 0x44, 0x69,
        0x66, 0x66, 0x69, 0x65, 0x2D,
        0x48, 0x65, 0x6C, 0x6C, 0x6D,
        0x61, 0x6E, 0x20, 0x43, 0x72,
        0x79, 0x70, 0x74, 0x6F, 0x67,
        0x72, 0x61, 0x70, 0x68, 0x69,
        0x63, 0x20, 0x50, 0x72, 0x6F,
        0x76, 0x69, 0x64, 0x65, 0x72,
        0x00
      };
      securityParams.provName = pn;

      /************************************************************************/
      /* Uncomment the following line to add the password "Teigha" to the     */
      /* R18+ file to be created                                              */
      /************************************************************************/
      //pDb->setSecurityParams(securityParams);
    }
#endif

    /********************************************************************/
    /* Write the database                                                */
    /********************************************************************/
    pDb->writeFile(&fb, fileType, outVer, true /* save preview */);
    odPrintConsoleString(L"\nDatabase has been saved as %ls", pDb->getFilename().c_str());
  }
  catch(OdError& e)
  {
    odPrintConsoleString(L"\n\nError: %ls", e.description().c_str());
    nRes = -1;
  }
  catch(...)
  {
    odPrintConsoleString(L"\n\nUnexpected error.");
    nRes = -1;
    throw;
  }

  /**********************************************************************/
  /* Uninitialize the Drawings SDK                                      */
  /**********************************************************************/
  odUninitialize();

  if (nRes && !disableWait)
  {
    odPrintConsoleString(L"\n\nPress ENTER to continue...");
    STD(cin).get();
  }
  odPrintConsoleString(L"\n\n");
  return nRes;
}
