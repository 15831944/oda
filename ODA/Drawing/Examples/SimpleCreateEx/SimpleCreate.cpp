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

// Simple create example
// - this example creates an object in .dwg file and saves under the name that is given to this application

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
  if (argc != 2)
  {
    odPrintConsoleString(L"Not enough parameters\n");
    odPrintConsoleString(L"Pass name that saved file should have\n");
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
    // ***** create file with sample object
    OdDbDatabasePtr pDb = svcs.createDatabase();
    OdDb3dSolidPtr pSolid;
    pSolid = OdDb3dSolid::createObject();
    pSolid->setDatabaseDefaults(pDb);
    pSolid->createBox(1.0, 1.0, 1.0);
    OdCmColor red;
    red.setRGB(255, 0, 0);
    pSolid->setColor(red);
    OdDbBlockTableRecordPtr pRecord = pDb->getModelSpaceId().safeOpenObject(OdDb::kForWrite);
    pRecord->appendOdDbEntity(pSolid);
    // ***** file created

    // ***** write file
    OdString f_out(argv[1]); // for UNIX UNICODE support
    OdWrFileBuf fb(f_out.c_str());
    OdDb::SaveType fileType = OdDb::kDwg;
    OdDb::DwgVersion outVer = OdDb::kDHL_CURRENT;
    pDb->writeFile(&fb, fileType, outVer, true /* save preview */);
    odPrintConsoleString(L"\nDatabase has been saved as %ls", pDb->getFilename().c_str());
    // ***** file written
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
