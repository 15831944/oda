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
/* This console application dumps the AVE information from a DWG, DXF,  */
/* or MLI file, or creates a DWG file with AVE stuff.                   */
/*                                                                      */
/* Calling sequence:                                                    */
/*                                                                      */
/*    OdAveEx [-w] [-x] <filename>                                      */
/*                                                                      */
/************************************************************************/
#include "OdaCommon.h"
#include "DbDatabase.h"
#include "OdFileBuf.h"
#include "diagnostics.h"

#include "StaticRxObject.h"
#include "ExSystemServices.h"
#include "ExHostAppServices.h"
#include "RxDynamicModule.h"

#define STL_USING_IOSTREAM
#include "OdaSTL.h"
#define  STD(a)  std:: a

#include "DbAveDumper.h"


#ifdef OD_HAVE_CONSOLE_H_FILE
#include <console.h>
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
};

/*static void dbgTrace(const OdChar* debugString)
{
  STD(_tcout) << debugString;
}

static void dbgAssert(const OdChar* expresssion, const OdChar* fileName, int nLineNo)
{
  OdString msg;
  msg.format(OD_T("\nAssertion Failed: %ls\nfile: %ls\nline: %d\n"),
    expresssion, fileName, nLineNo);      
  STD(_tcout) << msg.c_str();
}
*/
/************************************************************************/
/* Define a module map for statically linked modules:                   */
/************************************************************************/
#if !defined(_TOOLKIT_IN_DLL_)

ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdAveModuleImpl);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(ModelerModule);

ODRX_BEGIN_STATIC_MODULE_MAP()
  ODRX_DEFINE_STATIC_APPLICATION(OdAveModuleName,  OdAveModuleImpl)
  ODRX_DEFINE_STATIC_APPMODULE(OdModelerGeometryModuleName,       ModelerModule)
ODRX_END_STATIC_MODULE_MAP()

#endif


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
  int opt = 0;
  if (argc >= 3) 
  {
    OdString argv1(argv[1]);
    if (!odStrICmp(argv1,OD_T("-r")))
      opt = 1;
    else if (!odStrICmp(argv1,OD_T("-w")))
      opt = 2;
  }

  OdDb::SaveType fileType;
  OdDb::DwgVersion outVer;
  int nRes = 0;
  if (!opt) 
  {
    odPrintConsoleString(L"OdAveEx sample program Copyright 2006, Open Design Alliance\n");
    odPrintConsoleString(L"\tusage: OdAveEx -option filename\n");
    odPrintConsoleString(L"\toption can be:\n");
    odPrintConsoleString(L"\t-r - dump ave info from file\n");
    odPrintConsoleString(L"\t-w - create sample file with ave info using box.sat file data\n");
    nRes = 1;
  }
  else
  {
#if !defined(_TOOLKIT_IN_DLL_)
    ODRX_INIT_STATIC_MODULE_MAP();
#endif
    //::odSetTraceFunc(dbgTrace);
    //::odSetAssertFunc(dbgAssert);

    /********************************************************************/
    /* Create a custom Services instance.                               */
    /********************************************************************/
    OdStaticRxObject<MyServices> svcs;
    svcs.setRecomputeDimBlocksRequired(false); // Do not need this module
    /********************************************************************/
    /* Initialize Drawings.                                             */
    /********************************************************************/
    odInitialize(&svcs);

    /********************************************************************/
    /* Load the AVE extensions.                                         */
    /********************************************************************/
    {
      ::odrxDynamicLinker()->loadApp(OdAveModuleName);
    }

    /********************************************************************/
    /* Display the Product and Version that created the executable      */
    /********************************************************************/
    odPrintConsoleString(L"\nOdAveEx developed using %ls ver %ls\n", 
      svcs.product().c_str(), svcs.versionString().c_str());

    try 
    {
      if (opt == 1)
      {
        OdString fname = argv[2];
        if (fname.trimRight().right(4).iCompare(OD_T(".mli")) == 0)
        {
          /**************************************************************/
          /* Dump a material library                                    */
          /**************************************************************/
          OdStreamBufPtr pFile = odSystemServices()->createFile(fname, Oda::kFileRead, Oda::kShareDenyNo);
          DbAveDumper dumper;
          dumper.dumpMaterials(pFile);
        }
        else
        {
          /**************************************************************/
          /* Dump a DWG or DXF file                                     */
          /**************************************************************/
          OdDbDatabasePtr pDb;
          OdString f(argv[2]);
          pDb = svcs.readFile( f.c_str(), false, false, Oda::kShareDenyNo ); 
          if (!pDb.isNull())
          {
            DbAveDumper dumper;
            dumper.dump(pDb);
          }
        }
      }
      else // opt == 2
      {
        /****************************************************************/
        /* Create and dump a DWG file                                   */
        /****************************************************************/

        /****************************************************************/
        /* Create and initialize a default database                     */
        /****************************************************************/
        OdDbDatabasePtr pDb = svcs.createDatabase();
    
        /****************************************************************/
        /* Fill the database                                            */
        /****************************************************************/
        DbAveFiller filler;
        filler.fillDatabase(pDb);
    
        /****************************************************************/
        /* Write the database                                           */
        /****************************************************************/
        OdString outFile(argv[2]);
        OdWrFileBuf fb(outFile.c_str());
        fileType = OdDb::kDwg;
        outVer = OdDb::vAC18;
        pDb->writeFile(&fb, fileType, outVer);

        /****************************************************************/
        /* Dump the database                                            */
        /****************************************************************/
        DbAveDumper dumper;
        dumper.dump(pDb);
        odPrintConsoleString(L"\nDatabase has been saved to: %ls\n", pDb->getFilename().c_str());
      }
    }
    catch(OdError& e)
    {
      odPrintConsoleString(L"Error :%ls\n", e.description().c_str());
      nRes = -1;
    }
    catch(...)
    {
      odPrintConsoleString(L"Unexpected error.\n");
      nRes = -1;
    }

    odUninitialize();
  }
  if (nRes) {
    odPrintConsoleString(L"\nPress ENTER to continue...\n");
    STD(cin).get();
  }
  return nRes;
}
