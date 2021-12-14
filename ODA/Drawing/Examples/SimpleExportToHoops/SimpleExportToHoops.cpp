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

// Simple read and export into hoops example
// - this example shows how to export .dwg to hoops
#include "OdaCommon.h"
#include "DbDatabase.h"
#include "ExSystemServices.h"
#include "ExHostAppServices.h"

// hoops-specific 
// please note that hoops can not be loaded as a dynamic module but have to be linked to project instead
#include "HoopsCommon.h"
#include "HoopsExportParams.h"
#include "visualize_license.h"

#define STL_USING_IOSTREAM
#include "OdaSTL.h"
#define  STD(a)  std:: a

//#define EXPORT_TO_HOOPES_ENABLED


#ifdef	_TOOLKIT_IN_DLL_
#ifdef _DEBUG
#pragma comment(lib,"hoopsd.lib")
#pragma comment(lib,"hoops_streamd.lib")
#pragma comment(lib,"hoops_utilsstatd_md.lib")
#else
#pragma comment(lib,"hoops.lib")
#pragma comment(lib,"hoops_stream.lib")
#pragma comment(lib,"hoops_utilsstat_md.lib")
#endif
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

#ifdef EXPORT_TO_HOOPES_ENABLED

#ifdef OD_HAVE_CCOMMAND_FUNC
  argc = ccommand(&argv);
#endif

  // ensure valid input
  if (argc != 3)
  {
    odPrintConsoleString(L"Not enough parameters\n");
    odPrintConsoleString(L"Pass name of file to read and name that saved file should have\n");
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
    //  ***** file loaded

    // ***** export file to hoops
    // - setup
    long iExportSegment = HC_KOpen_Segment("/ExportedModel");
    HC_Close_Segment();

    // - set export params
    TD_HOOPS_COMMON_EXPORT::HoopsExportParams params;
    params.pDb = (OdDbBaseDatabase*)pDb;
    params.iStartKey = iExportSegment;
    params.bSetCamera = true;
    params.bExportCurrentLayout = true;
    params.bResetSystemExport = false;
    params.strFileName = OdString::kEmpty;
    //params.background = bgColor; // optional
    params.iExportFileType = 1;
    //const ODCOLORREF* palette = odcmAcadPalette(bgColor); // optional
    //params.setPalette(palette); // optional

    // call hoops export
    OdUInt32 retValue = exportHoops(params);

    if (retValue != exOk)
    {
      // - export failed
    }
    // ***** export completed

	  // ***** write file
	  HC_Open_Segment_By_Key(iExportSegment);
	  OdString f_out(argv[2]); // for UNIX UNICODE support
	  HC_Write_Metafile(".", f_out, "use color names, follow cross-references, save state = (color names, fonts, textures)"); 
    HC_Close_Segment();
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
#endif

  return nRes;
  // ***** end
}

