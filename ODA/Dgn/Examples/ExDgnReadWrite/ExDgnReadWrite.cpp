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
/* This console application reads and writes a DGN file.                */
/*                                                                      */
/* Calling sequence:                                                    */
/*                                                                      */
/* ExDgnReadWrite <source filename> <destination filename>              */
/*                                                                      */
/************************************************************************/

#include "OdaCommon.h"

#include "StaticRxObject.h"
#include "RxInit.h"
#include "RxDynamicModule.h"
#include "DynamicLinker.h"
#include "ExDgnServices.h"
#include "ExDgnHostAppServices.h"
#include "DgDatabase.h"
#include "RxDynamicModule.h"
#include "diagnostics.h"

#define STL_USING_IOSTREAM
#include "OdaSTL.h"
#define  STD(a)  std:: a

// Auxiliary function to display different messages
//void oddgPrintConsoleString(const wchar_t* fmt, ...);

/************************************************************************/
/* Define a Custom Services class.                                      */
/*                                                                      */
/* Combines the platform dependent functionality of                     */
/* OdExDgnSystemServices and OdExDgnHostAppServices                     */ 
/************************************************************************/
class MyServices : public OdExDgnSystemServices, public OdExDgnHostAppServices
{
protected:
  ODRX_USING_HEAP_OPERATORS(OdExDgnSystemServices);
};

/************************************************************************/
/* User defined function to write a message                             */
/* to the console for each ODA_ASSERT                                   */
/************************************************************************/
static void customAssertFunc(const char* expr, const char* fileName, int nLine)
{
  printf( 
    "Assertion has occurs:\n"
    " Expression: %s\n"
    " Filename: %s\n"
    " Line number: %d\n",
    expr, fileName, nLine );
}

static void myTrace(const OdChar* debugString)
{
#if defined(_DEBUG) && defined(_WIN32)
  OutputDebugStringW((const wchar_t*)debugString);
#endif
}

#ifndef _TOOLKIT_IN_DLL_
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdDgnModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdDgn7IOModuleImpl);   //this library allows to read V7 files
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdRxThreadPoolService);

ODRX_BEGIN_STATIC_MODULE_MAP()
  ODRX_DEFINE_STATIC_APPLICATION(L"TG_Db", OdDgnModule)
  ODRX_DEFINE_STATIC_APPLICATION(L"TG_Dgn7IO", OdDgn7IOModuleImpl)
  ODRX_DEFINE_STATIC_APPLICATION(OdThreadPoolModuleName, OdRxThreadPoolService)
ODRX_END_STATIC_MODULE_MAP()
#endif

/************************************************************************/
/* Main                                                                 */
/************************************************************************/
#if defined(OD_USE_WMAIN)
int wmain(int argc, wchar_t* argv[])
#else
int main(int argc, char* argv[])
#endif
{
  ::odSetAssertFunc( customAssertFunc );

  // if the last cmd-parameter is -NoWait then disable prompt
  bool bMTLoading = false;
  bool promptEnabled = true;
  
  if( argc > 3 )
  {
    for( OdInt32 k = 3; k < argc; k++ )
    {
      if( odStrICmp(OdString(argv[k]), OD_T("-NoWait")) )
        promptEnabled = false;
      else if( odStrICmp(OdString(argv[k]), OD_T("MtLoading")) )
        bMTLoading = true;
    }
  }

  /**********************************************************************/
  /* Verify the argument count and display an error message as required */
  /**********************************************************************/
  if ( argc < 3 )
  {
    oddgPrintConsoleString(L"\nExDgnReadWrite sample program. Copyright (c) 2019, Open Design Alliance\n");
    oddgPrintConsoleString(L"\tusage: ExDgnReadWrite <source file> <destination file> <MtLoading> <NoWait>\n");
    oddgPrintConsoleString(L"\t<MtLoading> (optional) - enable multi thread loading.\n");
    oddgPrintConsoleString(L"\t<NoWait>    (optional) - disable prompt.\n");
    oddgPrintConsoleString(L"\nPress ENTER to continue...\n");
	if ( promptEnabled )
	{
		STD(cin).get();
	}
    return 1;
  }
  OdString srcFileName = argv[1];
  OdString dstFileName = argv[2];

#ifndef _TOOLKIT_IN_DLL_
  ODRX_INIT_STATIC_MODULE_MAP();
#endif

  /**********************************************************************/
  /* Create a Services object                                           */
  /**********************************************************************/
  OdStaticRxObject<MyServices> svcs;

  /**********************************************************************/
  /* Initialize Runtime Extension environment                           */
  /**********************************************************************/
  odrxInitialize(&svcs);
    odSetTraceFunc(&myTrace);
    svcs.disableProgressMeterOutput(true);

    if( bMTLoading )
    {
      svcs.setMtMode( OdDb::kMTLoading, true );
      svcs.setNumThreads( OdDb::kMTLoading, 0 );
    }
    else
      svcs.setMtMode( OdDb::kMTLoading, false );

  try 
  {
    /**********************************************************************/
    /* Initialize ODA Drawings API for DGN                                */
    /**********************************************************************/
    ::odrxDynamicLinker()->loadModule(L"TG_Db", false);

    /********************************************************************/
    /* Read the DGN file into the OdDgDatabase object                   */
    /********************************************************************/
    OdDgDatabasePtr pDb = svcs.readFile( srcFileName );

    /********************************************************************/
    /* Write the OdDgDatabase data into the DGN file                    */
    /********************************************************************/
    pDb->writeFile( dstFileName );
  }
  /********************************************************************/
  /* Display the error                                                */
  /********************************************************************/
  catch (OdError& e)
  {
    oddgPrintConsoleString(L"\nODA Drawings API for DGN Error: %ls\n", e.description().c_str());

  }
  catch (const std::exception &exc)
  {
    oddgPrintConsoleString(L"\nCatch std::exception\n");
    oddgPrintConsoleString(L"\nODA Drawings API for DGN Error: %s\n", exc.what());
  }
  catch (...)
  {
    oddgPrintConsoleString(L"\nUnknown Error.\n");
  }

  /**********************************************************************/
  /* Uninitialize Runtime Extension environment                         */
  /**********************************************************************/
  odrxUninitialize();

  return 0;
}
