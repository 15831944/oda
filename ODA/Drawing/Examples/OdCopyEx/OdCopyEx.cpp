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
/* Copies the specified file to the specified file in                   */
/* the specified version and type.                                      */
/*                                                                      */
/************************************************************************/

static const char cUsage[] =
{
"Usage:\n"
"OdCopyEx <source file> <target file> [OutVer] [OutType]\n"
"    OutVer can be any of:\n"
"           ACAD9, ACAD10 (DXF and DXB only)\n"
"           ACAD12, ACAD13, ACAD14, ACAD2000, ACAD2004, ACAD2007\n"
"           ACAD2010, ACAD2013, ACAD2018\n"
"           (default is latest supported version)\n"
"    OutType can be any of DWG, DXF, DXB (binary DXF)\n"
"           (default is DWG)\n\n"
};
/************************************************************************/

#include "OdaCommon.h"
#include "DbDatabase.h"
#include "DbAudit.h"
#include "RxDynamicModule.h"
#include "ExSystemServices.h"
#include "ExHostAppServices.h"
#include "OdFileBuf.h"
#include "diagnostics.h"

#define STL_USING_IOSTREAM
#include "OdaSTL.h"
#define  STD(a)  std:: a

#ifdef OD_HAVE_CONSOLE_H_FILE
#include <console.h>
#endif

/************************************************************************/
/* Define a module map for statically linked modules:                   */
/************************************************************************/
#ifndef _TOOLKIT_IN_DLL_

ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(BitmapModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdRecomputeDimBlockModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(ModelerModule);

ODRX_BEGIN_STATIC_MODULE_MAP()
  ODRX_DEFINE_STATIC_APPMODULE  (OdWinBitmapModuleName,     BitmapModule)
  ODRX_DEFINE_STATIC_APPLICATION(OdRecomputeDimBlockModuleName, OdRecomputeDimBlockModule)
  ODRX_DEFINE_STATIC_APPLICATION(OdModelerGeometryModuleName,  ModelerModule)
ODRX_END_STATIC_MODULE_MAP()

#endif


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
};

/********************************************************************************/
/* Define Assert function to not crash Debug application if assertion is fired. */
/********************************************************************************/
static void MyAssert(const char* expression, const char* fileName, int nLineNo)
{
  OdString message;
  message.format(L"\n!!! Assertion failed: \"%s\"\n    file: %ls, line %d\n\n", OdString(expression).c_str(), OdString(fileName).c_str(), nLineNo);
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

#ifndef _TOOLKIT_IN_DLL_
    ODRX_INIT_STATIC_MODULE_MAP();
#endif

  int nRes = 1;
  /**********************************************************************/
  /* Set customized assert function                                     */
  /**********************************************************************/
  odSetAssertFunc(MyAssert);

  /**********************************************************************/
  /* Define a Custom Services object                                    */
  /**********************************************************************/
  OdStaticRxObject<MyServices> svcs;
  odInitialize(&svcs);

  /**********************************************************************/
  /* Verify the argument count and display an error message as required */
  /**********************************************************************/
  if (argc < 3) 
  {
    STD(cout) << cUsage;
    nRes = 1;
  }
  else
  {
    OdDb::SaveType fileType = OdDb::kDwg;
    OdDb::DwgVersion outVer = OdDb::kDHL_CURRENT;

    if (argc >= 4) 
    {
      OdString argv3(argv[3]);
      if      (!odStrICmp(argv3,OD_T("ACAD9")))     outVer = OdDb::vAC09;
      else if (!odStrICmp(argv3,OD_T("ACAD10")))    outVer = OdDb::vAC10;
      else if (!odStrICmp(argv3,OD_T("ACAD12")))    outVer = OdDb::vAC12;
      else if (!odStrICmp(argv3,OD_T("ACAD13")))    outVer = OdDb::vAC13;
      else if (!odStrICmp(argv3,OD_T("ACAD14")))    outVer = OdDb::vAC14;
      else if (!odStrICmp(argv3,OD_T("ACAD2000")))  outVer = OdDb::vAC15;
      else if (!odStrICmp(argv3,OD_T("ACAD2004")))  outVer = OdDb::vAC18;
      else if (!odStrICmp(argv3,OD_T("ACAD2007")))  outVer = OdDb::vAC21;
      else if (!odStrICmp(argv3,OD_T("ACAD2010")))  outVer = OdDb::vAC24;
      else if (!odStrICmp(argv3,OD_T("ACAD2013")))  outVer = OdDb::vAC27;
      else if (!odStrICmp(argv3,OD_T("ACAD2018")))  outVer = OdDb::vAC32;
    }

    if (argc >= 5)  
    {
      OdString argv4(argv[4]);
      if      (!odStrICmp(argv4,OD_T("DWG"))) fileType = OdDb::kDwg;
      else if (!odStrICmp(argv4,OD_T("DXF"))) fileType = OdDb::kDxf;
      else if (!odStrICmp(argv4,OD_T("DXB"))) fileType = OdDb::kDxb;
    }

    /********************************************************************/
    /* Disable the progress meter                                       */
    /********************************************************************/
    svcs.disableOutput(true);

    OdDbDatabasePtr pDb;
    try 
    {
      /******************************************************************/
      /* Create a database and read <source file> into it               */
      /******************************************************************/
      OdString inFile(argv[1]);
      pDb = svcs.readFile(inFile); 

      /******************************************************************/
      /* Audit the database, and fix any errors                         */
      /******************************************************************/
      OdDbAuditInfo ainfo;
      ainfo.setFixErrors(true);
      pDb->auditDatabase(&ainfo);
      OdString strMessage;
      int nErrorsFound = ainfo.numErrors();
      strMessage.format(L"Errors found %d\n", nErrorsFound);
      odPrintConsoleString(strMessage.c_str());
      if (nErrorsFound)
      {
        strMessage.format(L"Errors fixed %d\n", ainfo.numFixes());
        odPrintConsoleString(strMessage.c_str());
      }
    
      /******************************************************************/
      /* Create and open a write file buffer for <target file>          */
      /******************************************************************/
      OdRxObjectImpl<OdWrFileBuf> fb;
      OdString outFile(argv[2]);
      fb.open(outFile.c_str());
      
      /******************************************************************/
      /* Write the database without generating a bitmap                 */
      /******************************************************************/
      pDb->writeFile(&fb, fileType, outVer , false /* Generate Bitmap */ );
      nRes = 0;
    }
    catch (OdError& err)
    {
      odPrintConsoleString(L"\nOdError thrown: %ls\n",err.description().c_str());
      nRes = -1;
    }
    catch (...)
    {
      STD(cout) << "\nGeneral exception thrown, exiting\n";
      nRes = -1;
    }
  }

  /**********************************************************************/
  /* Uninitialize Drawings.                                             */
  /**********************************************************************/
  odUninitialize();

  if (nRes != 0)
  {
    STD(cout) << "Press ENTER to exit..." << STD(endl);
    STD(cin).get();
  }
  else
  {
    STD(cout) << "File saved OK\n";
  }

  return nRes;
}
