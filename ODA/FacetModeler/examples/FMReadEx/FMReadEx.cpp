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
/* This example demonstrates reading files and streams in Facet Modeler.*/
/* To read a file, you need to know what data is written there,         */
/* since several objects can be written to the stream sequentially.     */
/*                                                                      */
/* Through command line parameters,                                     */
/* the user can specify a sequence of object types to be read.          */
/************************************************************************/

#include "OdaCommon.h"
#include "FMFileDump.h"

#include "diagnostics.h"
#include "OdFileBuf.h"
#include "StringArray.h"
#include "StaticRxObject.h"
#include "ExPrintConsole.h"
#include "ExSystemServices.h"
#include "FMDataSerialize.h"
#include "Modeler/FMMdlFace.h"

#define STL_USING_IOSTREAM
#include "OdaSTL.h"
#define  STD(a)  std:: a

#ifdef OD_HAVE_CONSOLE_H_FILE
#include <console.h>
#endif
#include "Modeler/FMMdlIterators.h"

/************************************************************************/
/* Define a Custom Services class.                                      */
/*                                                                      */
/* Combines the platform dependent functionality of                     */
/* ExSystemServices and ExHostAppServices                               */ 
/************************************************************************/
class MyServices : public ExSystemServices
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
  message.format(L"!!! Assertion failed: \"%s\"\n    file: %ls, line %d\n", OdString(expression).c_str(), OdString(fileName).c_str(), nLineNo);
  odPrintConsoleString(message);
}

/********************************************************************************/
/* Define Ge error handler to not crash FMReadEx application and dump errors.   */
/********************************************************************************/
static void MyGeError(const OdResult res)
{
  OdString message;
  message.format(L"!!! Ge error: \"%s\"\n", OdError(res).description().c_str());
  odPrintConsoleString(message);
}

/********************************************************************************/
/* Print usage example                                                          */
/********************************************************************************/
static void printUsage(const OdStringArray& args)
{
  odPrintConsoleString(L"usage: %s FILENAME [TYPE...]\n", args[0].c_str());
  odPrintConsoleString(L"TYPE - array of types to read:\n");

  odPrintConsoleString(L"body\n");
  odPrintConsoleString(L"contour2d\n");
  odPrintConsoleString(L"profile2d\n");
  odPrintConsoleString(L"point2d\n");
  odPrintConsoleString(L"point3d\n");

  odPrintConsoleString(L"FILENAME - input file.\n");
  odPrintConsoleString(L"Example:\n");
  odPrintConsoleString(L"%s D:/some-fm-filename.fdb body contour2d profile2d point2d point3d\n", args[0].c_str());
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
  
  // Convert parameters for searching
  OdStringArray arguments;
  arguments.resize(argc);
  for (int i = 0; i < argc; i++)
  {
    arguments[i] = argv[i];
  }

  /**********************************************************************/
  /* Verify the argument count and display an error message as required */
  /**********************************************************************/
  if (argc < 2) 
  {
    printUsage(arguments);
    return 1;
  }
  else
  {
    /********************************************************************/
    /* Create a custom Services instance.                               */
    /********************************************************************/
    OdStaticRxObject<MyServices> svcs;

    /**********************************************************************/
    /* Set customized assert function                                     */
    /**********************************************************************/
    odSetAssertFunc(MyAssert);

    /**********************************************************************/
    /* Set customized Ge exception processing                             */
    /**********************************************************************/
    OdGeContext::gErrorFunc = MyGeError;

    /********************************************************************/
    /* Initialize TD                                                    */
    /********************************************************************/
    odrxInitialize(&svcs);

    bool bSuccess = true;
    try 
    {
      readFromFile(arguments);
    }
    /********************************************************************/
    /* Display the error                                                */
    /********************************************************************/
    catch (OdError_FileNotFound& fe)
    {
        odPrintConsoleString(L"OdError_FileNotFound: %ls\n",fe.getFileName().c_str());
        bSuccess = false;
    }
    catch (OdError_FileException& fe)
    {
        odPrintConsoleString(L"OdError_FileException: %ls\n",fe.getFileName().c_str());
        bSuccess = false;
    }
    catch (OdError& e)
    {
      odPrintConsoleString(L"OdError: %ls\n",e.description().c_str());
      bSuccess = false;
    }
    catch (...)
    {
      odPrintConsoleString(L"Unknown Error.\n");
      return 0;
    }

    /********************************************************************/
    /* Uninitialize TD                                                  */
    /********************************************************************/
    odrxUninitialize();
    if (bSuccess)
      odPrintConsoleString(L"FMReadEx Finished Successfully");
    return 0;
  }
}
