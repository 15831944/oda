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
/* This console application reads a DWG file and extracts               */ 
/* its text contents to the console.                                    */
/*                                                                      */
/* Calling sequence:                                                    */
/*                                                                      */
/*    OdExtractTextEx <filename>                                        */
/*                                                                      */
/************************************************************************/

#include "OdaCommon.h"
#include "diagnostics.h"
#include "DbDatabase.h"
#include "ExSystemServices.h"
#include "ExHostAppServices.h"
#include "ExProtocolExtension.h"

#define STL_USING_IOSTREAM
#include "OdaSTL.h"
#define  STD(a)  std:: a

#include "DbParser.h"

#ifdef OD_HAVE_CONSOLE_H_FILE
#include <console.h>
#endif


#define EXTRACTTEXT_STR L"OdExtractTextEx"

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


/********************************************************************************/
/* Define Assert function to not crash Debug application if assertion is fired. */
/********************************************************************************/
static void MyAssert(const char* expression, const char* fileName, int nLineNo)
{
  OdString message;
  message.format(L"\n!!! Assertion failed: \"%ls\"\n    file: %ls, line %d\n", OdString(expression).c_str(), OdString(fileName).c_str(), nLineNo);
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
    odPrintConsoleString(EXTRACTTEXT_STR L" sample program. Copyright (C) " TD_COPYRIGHT_START_YEAR_W  TD_COPYRIGHT_END_W L"\n");
    odPrintConsoleString(L"\tusage: " EXTRACTTEXT_STR L" <filename>\n");
    odPrintConsoleString(L"\nPress ENTER to continue...\n");
    STD(cin).get();
    return 1;
  }
  /********************************************************************/
  /* Create a custom Services instance.                               */
  /********************************************************************/
  OdStaticRxObject<MyServices> svcs;
  svcs.disableOutput(true);   // Disable progress meter output
  svcs.setRecomputeDimBlocksRequired(false);  // We need not Dimension blocks recomputing
  odSetAssertFunc(MyAssert);

  /********************************************************************/
  /* Initialize the Drawings SDK.                                     */
  /********************************************************************/
  odInitialize(&svcs);
  /********************************************************************/
  /* Display the Product and Version that created the executable      */
  /********************************************************************/
  odPrintConsoleString(L"\n" EXTRACTTEXT_STR L" developed using %ls ver %ls\n", 
    svcs.product().c_str(), svcs.versionString().c_str());

#ifndef _MSC_VER
  /********************************************************************/
  /* Find the data file and and initialize the character mapper       */
  /********************************************************************/
  OdString iniFile = svcs.findFile(OD_T("adinit.dat"));
  if (!iniFile.isEmpty())
  {
    OdCharMapper::initialize(iniFile);
    odPrintConsoleString(L"adinit.dat is loaded OK!\n");
  }
  else
  {
    odPrintConsoleString(L"adinit.dat is NOT found\n");
  }
#endif

  /********************************************************************/
  /* This ExProtocolExtension class defines an OdDbEntity_Parser      */
  /* protocol extension for each of the supported OdDbEntity classes  */
  /* and a default parser for the non-supported classes.              */
  /********************************************************************/
  ExProtocolExtension theProtocolExtensions;

  /********************************************************************/
  /* Initialize protocol extensions                                   */
  /********************************************************************/
  theProtocolExtensions.initialize();
  bool bSuccess = false;
  try 
  {
    /******************************************************************/
    /* Load the drawing.                                              */
    /******************************************************************/
    OdString f(argv[1]); // for UNIX UNICODE support
    OdDbDatabasePtr pDb = svcs.readFile(f.c_str()); 

    if (!pDb.isNull())
    {
      theProtocolExtensions.setDatabase(pDb);
      /****************************************************************/
      /* Parse the database                                            */
      /****************************************************************/
      DbParser parser;
      parser.parse(pDb);
      odPrintConsoleString(L"Parsing finished\n");
      bSuccess = true;
    }
  }
  /********************************************************************/
  /* Display the error                                                */
  /********************************************************************/
  catch (OdError& e)
  {
    odPrintConsoleString(L"\nTeigha Error: %ls\n", e.description().c_str());
  }
  catch (...)
  {
    odPrintConsoleString(L"\nUnknown Error.\n");
    return 3;
  }

  /********************************************************************/
  /* Uninitialize protocol extensions                                 */
  /********************************************************************/
  theProtocolExtensions.uninitialize();
  odPrintConsoleString(L"Protocol Extensions uninitialized\n");

  /********************************************************************/
  /* Uninitialize the Drawings SDK                                        */
  /********************************************************************/
  
  try
  {
    odUninitialize();
    odPrintConsoleString(L"Teigha uninitialized\n");
    if (bSuccess)
      odPrintConsoleString(EXTRACTTEXT_STR L" Finished Successfully\n");
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
  
  return bSuccess ? 0 : 1;
}
