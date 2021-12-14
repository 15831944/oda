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
/* This console application reads a DWG file and dumps its contents     */
/* to the console.                                                      */
/************************************************************************/
#ifdef MECHANICAL_MODE
#define ODREADEX_S "TmReadEx"

#include "OdaCommon.h"

#else
#define ODREADEX_S "OdReadEx"
#endif

#include "OdaCommon.h"

static const char cUsage[] =
{
  ODREADEX_S " sample program. Copyright (C) " TD_COPYRIGHT_START_YEAR_S  TD_COPYRIGHT_END_S "\n"
  "Usage: " ODREADEX_S " filename [MT] [DO] [DR] [PL] [DD] [DA] [FD [SS H1,H2,...HN] [BL h1,h2,...hN]]\n"
  "  Output is written to stdout, redirect if necessary.\n"
  "    MT - loading using multithreaded mode\n"
  "    DO - disable output of progress meters\n"
  "    DR - disable recomputing of dimensions.\n"
  "    PL - enable partial loading\n"
  "    DD - only load, completely disable dump\n"
  "    DA - disable acis audit\n"
  "    FD - ODA Format dump.\n"
  "    SS - Selection Set to dump in ODA Format (only with FD).\n"
  "    BL - Black list; Do not dump specified objects (only with FD).\n"
  "    RH - Remap handles -- reassign new value to handle of each dumping object in the order of dumping (only with FD).\n"
  "    RR - dump all referenced objects recursively (only with FD).\n"
  "\nPress ENTER to continue...\n"
};


#include "diagnostics.h"
#include "DbDatabase.h"
#include "DbEntity.h"
#include "DbDimAssoc.h"

#include "OdCharMapper.h"
#include "RxObjectImpl.h"

#include "ExSystemServices.h"
#include "ExHostAppServices.h"
#include "ExProtocolExtension.h"
#include "OdFileBuf.h"
#include "RxDynamicModule.h"
#include "FdField.h"

#define STL_USING_IOSTREAM
#include "OdaSTL.h"
#define  STD(a)  std:: a

#include "DbDumper.h"
#include "DbFDumper.h"

#ifdef OD_HAVE_CONSOLE_H_FILE
#include <console.h>
#endif

/************************************************************************/
/* Compile password support for R18 Drawings                            */
/************************************************************************/

#ifdef MECHANICAL_MODE
#include "TmExProtocolExtension.h"
#include "TmModuleNames.h"
#include "AcMeOE/MechanicalVersion.h"

#define ODREADEX_STR L"TmReadEx"
#define INIT_PROTOCOL_EXTENSION(theProtocolExtensions)\
  TmExProtocolExtension theProtocolExtensions;
#else
#define ODRX_DECLARE_STATIC_MECHANICAL
#define ODRX_DEFINE_STATIC_MECHANICAL
#define ODREADEX_STR L"OdReadEx"
#define INIT_PROTOCOL_EXTENSION(theProtocolExtensions) ExProtocolExtension theProtocolExtensions;
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

private:

#ifdef _MSC_VER

  /**********************************************************************/
  /* Define a console interface for password support.                   */
  /**********************************************************************/
  bool getPassword(const OdString& dwgName, bool /*isXref*/, OdPassword& password)
  {
    odPrintConsoleString(L"Enter password to open drawing: %ls\n", dwgName.c_str());
    wchar_t pwdBuff[_MAX_PATH] = {0};
    STD(wcin).get(pwdBuff, L'\n').get();
    ::CharUpperW(pwdBuff);
    password = pwdBuff;
    return !password.isEmpty();
  }
#endif
};

/************************************************************************/
/* Define a module map for statically linked modules:                   */
/************************************************************************/
#ifndef _TOOLKIT_IN_DLL_

ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(ModelerModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdRecomputeDimBlockModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(ExFieldEvaluatorModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(TD_3DSolidHistoryModule);
ODRX_DECLARE_STATIC_MECHANICAL;
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(DbCryptModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdTfModule);

ODRX_BEGIN_STATIC_MODULE_MAP()
  ODRX_DEFINE_STATIC_APPLICATION(OdModelerGeometryModuleName,   ModelerModule)
  ODRX_DEFINE_STATIC_APPLICATION(OdRecomputeDimBlockModuleName, OdRecomputeDimBlockModule)
  ODRX_DEFINE_STATIC_APPMODULE(OdExFieldEvaluatorModuleName, ExFieldEvaluatorModule)
  ODRX_DEFINE_STATIC_APPMODULE(Od3DSolidHistoryTxModuleName, TD_3DSolidHistoryModule)
  ODRX_DEFINE_STATIC_MECHANICAL
  ODRX_DEFINE_STATIC_APPMODULE(DbCryptModuleName,            DbCryptModule)
  ODRX_DEFINE_STATIC_APPMODULE(TfModuleName,                    OdTfModule)
ODRX_END_STATIC_MODULE_MAP()

#endif

/********************************************************************************/
/* Define Assert function to not crash Debug application if assertion is fired. */
/********************************************************************************/
static void MyAssert(const char* expression, const char* fileName, int nLineNo)
{
  OdString message;
  message.format(L"/!\\ Assertion failed: \"%ls\" in file: \"%ls\", line: %d\n", OdString(expression).c_str(), OdString(fileName).c_str(), nLineNo);
  odPrintConsoleString(message);
}

/********************************************************************************/
/* Define Trace function to dump trace messages in Debug configuration.         */
/********************************************************************************/
void MyTraceFn(const OdChar* debugString)
{
  odPrintConsoleString(L"Trace: %ls\n", debugString);
}
/********************************************************************************/
/* Define Ge error handler to not crash OdReadEx application and dump errors.   */
/********************************************************************************/
static void MyGeError(OdResult res)
{
  OdString message;
  message.format(L"\n!!! Ge error: \"%ls\"\n", OdError(res).description().c_str());
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
#ifdef OD_HAVE_CCOMMAND_FUNC
  argc = ccommand(&argv);
#endif
  
  /**********************************************************************/
  /* Verify the argument count and display an error message as required */
  /**********************************************************************/
  if (argc < 2) 
  {
    STD(cout) << cUsage;
    STD(cin).get();
    return 1;
  }
  else
  {
#ifndef _TOOLKIT_IN_DLL_
    ODRX_INIT_STATIC_MODULE_MAP();
#endif

    bool isMTMode = false;
    bool enablePartialLoading = false;
    bool disableSvcsOutput = false;
    bool disableRecompute = false;
    bool disableDump = false;
    bool formatDump = false;
    bool dumpRefs = false;
    bool remap = false;
    OdString sset;
    OdString blist;
    bool enableAcisAudit = true;
    for (int i = 2; i < argc; ++i)
    {
      /**********************************************************************/
      /* Use MT loading mode                                                */
      /**********************************************************************/
      if (!OdString(argv[i]).iCompare(L"MT"))
        isMTMode = true;
      /**********************************************************************/
      /* Disable progress meter                                             */
      /**********************************************************************/
      else if (!OdString(argv[i]).iCompare(L"DO"))
        disableSvcsOutput = true;
      /**********************************************************************/
      /* Enable partial loading                                             */
      /**********************************************************************/
      else if (!OdString(argv[i]).iCompare(L"PL"))
        enablePartialLoading = true;
      /**********************************************************************/
      /* Disable dimension recompute                                        */
      /**********************************************************************/
      else if (!OdString(argv[i]).iCompare(L"DR"))
        disableRecompute = true;
      /**********************************************************************/
      /* Disable dump                                                       */
      /**********************************************************************/
      else if (!OdString(argv[i]).iCompare(L"DD"))
          disableDump = true;
      /**********************************************************************/
      /* Disable acis audit                                                 */
      /**********************************************************************/
      else if (!OdString(argv[i]).iCompare(L"DA"))
        enableAcisAudit = false;
      /**********************************************************************/
      /* ODA Format dump                                                    */
      /**********************************************************************/
      else if (!OdString(argv[i]).iCompare(L"FD"))
        formatDump = true;
      /**********************************************************************/
      /* Selection Set                                                      */
      /**********************************************************************/
      else if (!OdString(argv[i]).iCompare(L"SS") && ++i < argc)
        sset = argv[i];
      /**********************************************************************/
      /* Black list */
      /**********************************************************************/
      else if (!OdString(argv[i]).iCompare(L"BL") && ++i < argc)
        blist = argv[i];
      /**********************************************************************/
      /* dump all referenced objects recursively                            */
      /**********************************************************************/
      else if (!OdString(argv[i]).iCompare(L"RR"))
        dumpRefs = true;
      /**********************************************************************/
      /* remap handles                                                      */
      /**********************************************************************/
      else if (!OdString(argv[i]).iCompare(L"RH"))
        remap = true;
    }

    /********************************************************************/
    /* Create a custom Services instance.                               */
    /********************************************************************/
    OdStaticRxObject<MyServices> svcs;
    svcs.disableOutput(disableSvcsOutput);
    if (disableRecompute)
    {
      svcs.setRecomputeDimBlocksRequired(false);
    }

    /**********************************************************************/
    /* Overload OdTimeStamp::getLocalTime internal implementation         */
    /**********************************************************************/
    g_pLocalTimeFunc = getLocalTime;

    /**********************************************************************/
    /* Set customized assert function                                     */
    /**********************************************************************/
    odSetAssertFunc(MyAssert);
    /**********************************************************************/
    /* Set customized trace function                                     */
    /**********************************************************************/
    //odSetTraceFunc(&MyTraceFn);
    /**********************************************************************/
    /* Set customized Ge exception processing                             */
    /**********************************************************************/
    OdGeContext::gErrorFunc = MyGeError;

    /********************************************************************/
    /* Initialize the Drawings SDK.                                     */
    /********************************************************************/
    odInitialize(&svcs);
    /********************************************************************/
    /* Initialize Encription/Decription                      */
    /********************************************************************/
    ::odrxDynamicLinker()->loadModule(DbCryptModuleName);
	
#ifndef _MSC_VER

    /********************************************************************/
    /* Find the data file and and initialize the character mapper       */
    /********************************************************************/
    odPrintConsoleString(L"Initializing OdCharMapper: ");
    OdString iniFile = svcs.findFile(OD_T("adinit.dat"));
    if (!iniFile.isEmpty()) {
      OdCharMapper::initialize(iniFile);
      odPrintConsoleString(L"ok\n");
    } else
      odPrintConsoleString(L"\"adinit.dat\" is not found. String CP conversion is not supported in this run.\n");
#endif

// ODA_MT_DBIO_BEGIN
    /********************************************************************/
    /* Set variable for MT loading.                        */
    /********************************************************************/
    OdInt16 nMode = svcs.getMtMode();
    SETBIT(nMode, 1, isMTMode);
    svcs.setMtMode(nMode);
// ODA_MT_DBIO_END

    /********************************************************************/
    /* Display the Product and Version that created the executable      */
    /********************************************************************/
    odPrintConsoleString(L"\n" ODREADEX_STR L" developed using %ls ver %ls\n", 
      svcs.product().c_str(), svcs.versionString().c_str());

    /********************************************************************/
    /* This ExProtocolExtension class defines an OdDbEntity_Dumper      */
    /* protocol extension for each of the supported OdDbEntity classes  */
    /* and a default dumper for the non-supported classes.              */
    /********************************************************************/
    INIT_PROTOCOL_EXTENSION(theProtocolExtensions);

    bool bSuccess = true;
    try 
    {
      ::odrxDynamicLinker()->loadModule(OdExFieldEvaluatorModuleName);
      ::odrxDynamicLinker()->loadModule(Od3DSolidHistoryTxModuleName);
      if (false == disableRecompute)
      {
        ::odrxDynamicLinker()->loadModule(OdDynBlocksModuleName);
      }
#ifdef MECHANICAL_MODE
      LoadTeighaMechanicalModules(false);
#endif
//      ::odrxDynamicLinker()->loadModule(DbConstraintsModuleName);

      /********************************************************************/
      /* Initialize protocol extensions                                   */
      /********************************************************************/
      if(!formatDump)
        theProtocolExtensions.initialize();
      /********************************************************************/
      /* Disable/enable acis audit                                        */
      /********************************************************************/
      svcs.setEnableAcisAudit(enableAcisAudit);
      /******************************************************************/
      /* Create a database and load the drawing into it.                */
      /* readFile() allways should return loaded database.              */
      /* It throws OdError() exception if an error occurred.            */
      /* Specified arguments are as followed:                           */
      /*     filename, allowCPConversion, partialLoad, openMode         */      
      /******************************************************************/
      OdString f(argv[1]); // for UNIX UNICODE support
      OdDbDatabasePtr pDb = svcs.readFile(
        f.c_str(), true, enablePartialLoading, Oda::kShareDenyNo); 

      oddbEvaluateFields(pDb, OdDbField::kOpen);
      /****************************************************************/
      /* Display the File Name and Version                            */
      /****************************************************************/
      odPrintConsoleString(L"\nFile Name:   \"%ls\"", pDb->getFilename().c_str());
      odPrintConsoleString(L"\nFile Version: %d\n", pDb->originalFileVersion()); 

#ifdef MECHANICAL_MODE
      MechanicalVersion ver;
      if (getMechanicalVersion(pDb, ver) == eOk)
      {
        OdString str = "Loading Mechanical file " + verString(ver) + " version";
        odPrintConsoleString(str, "\n");
      }
#endif

      if (enablePartialLoading)
      {
        /****************************************************************/
        /* Will reload all objects
        /****************************************************************/
        pDb->closeInput();
      }

      /****************************************************************/
      /* Dump the database                                            */
      /****************************************************************/
      if (!disableDump) {
        if(formatDump) {
          DbFDumper fdumper;
          fdumper.fdump(pDb, sset, blist, dumpRefs, remap);
        }
        else {
          DbDumper dumper;
          dumper.dump(pDb);
        }
        odPrintConsoleString(L"Dumping finished\n");
      } else {
        odPrintConsoleString(L"Dumping disabled\n");
      }
    }
    /********************************************************************/
    /* Display the error                                                */
    /********************************************************************/
    catch (OdError& e)
    {
      odPrintConsoleString(L"\nTeigha Error: %ls\n", e.description().c_str());
      bSuccess = false;
    }
    catch (...)
    {
      odPrintConsoleString(L"\nUnknown Error.\nPress ENTER to continue...\n");
      STD(cin).get();
      return 0;
    }

    /********************************************************************/
    /* Uninitialize protocol extensions                                 */
    /********************************************************************/
    if (!formatDump) {
      theProtocolExtensions.uninitialize();
      odPrintConsoleString(L"Protocol Extensions uninitialized\n");
    }

    /********************************************************************/
    /* Uninitialize the Drawings SDK                                    */
    /********************************************************************/
    
    try
    {
      odUninitialize();
      odPrintConsoleString(L"Teigha uninitialized\n");
      if (bSuccess)
        odPrintConsoleString(ODREADEX_STR L" Finished Successfully\n");
    }
    catch(OdError& e)
    {
      odPrintConsoleString(L"odUninitialize() failed\n");
      odPrintConsoleString(L"OdError description:\n%ls\n", e.description().c_str());
    }
    catch(...)
    {
      odPrintConsoleString(L"odUninitialize() failed\n");
    }
    
    return 0;
  }
}
