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

#include <OdaCommon.h>
#include "StaticRxObject.h"
#include "ExSystemServices.h"
#include "ExPrintConsole.h"
#include "StringArray.h"

#include "FMDataSerialize.h"
#include "FMGeometry.h"
#include "Modeler/FMMdlBody.h"

#include "FMCreateStart.h"
#include "BodyExample.h"


class MyServices : public ExSystemServices
{
protected:
  ODRX_USING_HEAP_OPERATORS(ExSystemServices);
};

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

#if defined(TARGET_OS_MAC) && !defined(__MACH__)
  argc = ccommand(&argv);
#endif

  // Convert parameters for searching
  OdStringArray arguments;
  arguments.resize(argc);
  for (int i = 0; i < argc; i++)
  {
    arguments[i] = argv[i];
  }

  CreationMode mode;
  bool bInvalidArgs = (arguments.length() != 3);
  if (bInvalidArgs)
  {
    bInvalidArgs = true;
    nRes = 1;
  }
  else
  {
    OdString s = arguments[1];
#define FMCREATECASE(a, b, c) if (s == #a) mode = k##a; else
#include "ExampleCases.h"
    bInvalidArgs = true;
  }

  if (bInvalidArgs)
  {
    odPrintConsoleString(L"Wrong number of parameters\n");
    odPrintConsoleString(L"usage: %s <example> <filename>\n", arguments[0].c_str());
    odPrintConsoleString(L"\n<example>:");
#define FMCREATECASE(a, b, c) odPrintConsoleString(L"\n\t%ls - %ls", OdString(#a).c_str(), OdString(b).c_str());
#include "ExampleCases.h"
    odPrintConsoleString(L"\n<filename> - output file\n");

    return nRes;
  }

  /**********************************************************************/
  /* Initialize system services.                                        */
  /**********************************************************************/
  OdStaticRxObject<ExSystemServices> svcs;
  odrxInitialize(&svcs);

  FacetModeler::DeviationParams devParams;

  try
  {
    OdString filename(arguments[2]);
    OdStreamBufPtr pStream = odrxSystemServices()->createFile(filename, Oda::kFileWrite, Oda::kShareDenyNo, Oda::kCreateAlways);
    FMCreateStart(devParams, pStream, mode);
  }
  catch (OdError& e)
  {
    odPrintConsoleString(L"\n\nError: %ls", e.description().c_str());
    nRes = -1;
  }
  catch (...)
  {
    odPrintConsoleString(L"\n\nUnexpected error.");
    nRes = -1;
    throw;
  }

  odrxUninitialize();
  //rts::ProcInfo::LogMemoryInfoForCurrentProcess();
  return nRes;
}
