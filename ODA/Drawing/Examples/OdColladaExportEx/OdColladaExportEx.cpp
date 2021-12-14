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

/*****************************************************************************/
/* This console application reads a .dwg or .dxf file and writes Collada file*/
/*                                                                           */
/* Calling sequence:                                                         */
/*                                                                           */
/*   OdColladaExportEx <input file> <output file>                              */
/*****************************************************************************/

#include "OdaCommon.h"
#include "DbDatabase.h"
#include "ExSystemServices.h"
#include "ExHostAppServices.h"
#include "ColladaModule.h"
#include "ColorMapping.h"

#include "diagnostics.h"

#ifdef OD_HAVE_CONSOLE_H_FILE
#include <console.h>
#endif

using namespace TD_COLLADA_EXPORT;

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

/************************************************************************/
/* Define a our own Assert function.                                    */
/************************************************************************/

static void MyAssert(const char* expression, const char* fileName, int nLineNo)
{
  OdString message;
  message.format(L"\n!!! Assertion failed: \"%s\"\n    file: %ls, line %d\n", OdString(expression).c_str(), OdString(fileName).c_str(), nLineNo);
  odPrintConsoleString(message);
}

/************************************************************************/
/* Define a module map for statically linked modules                    */
/************************************************************************/
#if !defined(_TOOLKIT_IN_DLL_)

ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(ColladaModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(ModelerModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(ExRasterModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdRasterProcessingServicesImpl);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdRecomputeDimBlockModule);

ODRX_BEGIN_STATIC_MODULE_MAP()
  ODRX_DEFINE_STATIC_APPMODULE(OdColladaExportModuleName,   ColladaModule)
  ODRX_DEFINE_STATIC_APPMODULE(OdModelerGeometryModuleName, ModelerModule)
  ODRX_DEFINE_STATIC_APPMODULE(RX_RASTER_SERVICES_APPNAME,  ExRasterModule)
  ODRX_DEFINE_STATIC_APPMODULE(OdRasterProcessorModuleName, OdRasterProcessingServicesImpl)
  ODRX_DEFINE_STATIC_APPLICATION(OdRecomputeDimBlockModuleName, OdRecomputeDimBlockModule)
ODRX_END_STATIC_MODULE_MAP()
#endif

/************************************************************************/
/* Main                                                                 */
/************************************************************************/
#if defined(_UNICODE) && defined(OD_USE_WMAIN)
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
  if (argc < 3) 
  {
    odPrintConsoleString(L"usage: OdColladaExportEx <input file> <output file>\n");
    odPrintConsoleString(L"   <input file>  - .dxf or .dwg file\n");
    odPrintConsoleString(L"   <output file> - .dae file\n");
    return 1;
  }

#if !defined(_TOOLKIT_IN_DLL_)
  ODRX_INIT_STATIC_MODULE_MAP();
#endif
  odSetAssertFunc(MyAssert);

  // Create a custom Services object.
  OdStaticRxObject<MyServices> svcs;
  // Initialize Drawings.
  odInitialize(&svcs);

  odPrintConsoleString(L"Developed using %ls ver %ls\n", svcs.product().c_str(), svcs.versionString().c_str());

  try 
  {
    /******************************************************************/
    /* Create a database and load the drawing into it.                */
    /*                                                                */
    /* Specified arguments are as followed:                           */
    /*     filename, allowCPConversion, partialLoad, openMode         */
    /******************************************************************/
    OdDbDatabasePtr pDb;
    OdString f(argv[1]);
    pDb = svcs.readFile( f.c_str(), false, false, Oda::kShareDenyNo ); 

    if (!pDb.isNull())
    {
      ColladaModulePtr module = odrxDynamicLinker()->loadModule(OdColladaExportModuleName);
      if (module.isNull())
      {
        odPrintConsoleString( L"Cannot load module : ");
        odPrintConsoleString( OdColladaExportModuleName );
        return 2;
      }

      OdString arg2(argv[2]);
      OdResult res = module->exportCollada(pDb, arg2, odcmAcadLightPalette(), 256);
      if (eOk == res)
      {
        odPrintConsoleString( L"Successfully exported." );
      }
      else
      {
        OdString tmp = OD_T("Error : ") + OdError(res).description();
        odPrintConsoleString( tmp );
        return 3;
      }
    }
    odPrintConsoleString(L"\nDone.\n");
  }

  /********************************************************************/
  /* Display the error                                                */
  /********************************************************************/
  catch (const OdError& e)
  {
    odPrintConsoleString(L"\nException Caught: %ls\n", e.description().c_str());
  }
  catch (...)
  {
    odPrintConsoleString(L"\nUnknown Exception Caught\n");
  }
  odUninitialize();
  return 0;
}

//
