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

// simple edit example
// - pass file that was created by SimpleCreate example to this example
// - it will change the color of sphere that was in that file and then re-save the file by input name
#include "OdaCommon.h"
#include "RxDynamicModule.h"
#include "DbDatabase.h"
#include "ExSystemServices.h"
#include "ExHostAppServices.h"
#include "OdFileBuf.h"
#include <DbBaseHostAppServices.h>
#include "Db3dSolid.h"
#include "DbBlockTableRecord.h"

#define STL_USING_IOSTREAM
#include "OdaSTL.h"
#define  STD(a)  std:: a

// Define module map for statically linked modules:
#if !defined(_TOOLKIT_IN_DLL_)
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(ModelerModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdRecomputeDimBlockModule);

ODRX_BEGIN_STATIC_MODULE_MAP()
ODRX_DEFINE_STATIC_APPLICATION(OdModelerGeometryModuleName, ModelerModule)
ODRX_DEFINE_STATIC_APPLICATION(OdRecomputeDimBlockModuleName, OdRecomputeDimBlockModule)
ODRX_END_STATIC_MODULE_MAP()
#endif

class MyServices : public ExSystemServices, public ExHostAppServices
{
protected:
  ODRX_USING_HEAP_OPERATORS(ExSystemServices);
};

#if defined(ODA_WINDOWS)
int wmain(int argc, wchar_t* argv[])
#else
int main(int argc, char* argv[])
#endif
{
  int nRes = 0; // Return value for main

#ifdef OD_HAVE_CCOMMAND_FUNC
  argc = ccommand(&argv);
#endif

  // ensure valid input
  if (argc != 3)
  {
    odPrintConsoleString(L"Not enough parameters\n");
    odPrintConsoleString(L"Pass name of the file to be edited and handle of object to be edited\n");
    odPrintConsoleString(L"\n\nPress ENTER to continue...");
    STD(cin).get();
    return 1;
  }

#ifndef _TOOLKIT_IN_DLL_
  ODRX_INIT_STATIC_MODULE_MAP();
#endif

  // ***** initialize
  OdStaticRxObject<MyServices> svcs;
  odInitialize(&svcs);
  // ***** complete initialize

  try
  {
    // ***** read file
    OdString f(argv[1]); // for UNIX UNICODE support
    OdDbDatabasePtr pDb = svcs.readFile(f.c_str());
    // ***** file loaded

    // ***** edit file
    // - get box from file
#if defined(ODA_WINDOWS) // different converters have to be used by different platforms because of different main parameters
    int handle_value = wcstol(argv[2], NULL, 16);
#else
    int handle_value = strtol(argv[2], NULL, 16);
#endif
    OdDbEntityPtr e = pDb->getOdDbObjectId(OdDbHandle(handle_value)).safeOpenObject(OdDb::kForWrite);
    if (e.isNull())
    {
      throw OdError(eInvalidObjectId);
    }
    // - change box color
    OdCmColor blue;
    blue.setRGB(0, 0, 255);
    e->setColor(blue);
    // ***** file editing completed

    // ***** save file
    OdDb::SaveType fileType = OdDb::kDwg;
    OdDb::DwgVersion outVer = OdDb::kDHL_CURRENT;
    pDb->writeFile(pDb->getFilename(), fileType, outVer, true /* save preview */);
    odPrintConsoleString(L"\nDatabase has been saved as %ls", pDb->getFilename().c_str());
    // ***** file saved
  }
  catch (OdError& e) // while errors won't happen in this example its a good idea to run teigha inside try/catch block to make further debugging easier
  {
    odPrintConsoleString(L"\nTeigha Error: %ls\n", e.description().c_str());
    nRes = -1;
  }
  catch (...)
  {
    odPrintConsoleString(L"\nUnknown Error.\nPress ENTER to continue...\n");
    nRes = -1;
    throw;
  }

  // ***** uninitialize
  try
  {
    odUninitialize();
  }
  catch (OdError& e)
  {
    odPrintConsoleString(L"odUninitialize() failed\n");
    odPrintConsoleString(L"OdError description:\n%ls\n", e.description().c_str());
    nRes = -1;
  }
  catch (...)
  {
    odPrintConsoleString(L"odUninitialize() failed\n");
    nRes = -1;
  }
  // ***** uninitialize complete

  // ***** for convenience
  if (nRes)
  {
    odPrintConsoleString(L"\n\nPress ENTER to continue...");
    STD(cin).get();
  }
  odPrintConsoleString(L"\n\n");
  return nRes;
  // ***** end
}
