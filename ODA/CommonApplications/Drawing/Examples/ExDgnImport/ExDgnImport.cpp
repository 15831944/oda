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
/* This console application reads a DGN file, and saves it to a DWG     */
/* file in the current version                                          */
/*                                                                      */
/* Calling sequence:                                                    */
/*                                                                      */
/*    ExDgnImport <source file> <target file>                           */
/*                                                                      */
/************************************************************************/


#include "OdaCommon.h"
#define STL_USING_IOSTREAM
#include "OdaSTL.h"
#define  STD(a)  std:: a
#include "DbDatabase.h"
#include "ExSystemServices.h"
#include "ExHostAppServices.h"
#include "OdFileBuf.h"
#include "RxModule.h"
#include "RxDynamicModule.h"
#include "RxVariantValue.h"
#include "Imports/DgnImport/DgnImport.h"
#include <locale.h>
#include "diagnostics.h"
#include <ExDgnHostAppServices.h>

using namespace TD_DGN_IMPORT;
#ifdef OD_HAVE_CONSOLE_H_FILE
#include <console.h>
#endif

static void myTrace(const OdChar* debugString)
{
#if defined(_DEBUG) && defined(_WIN32)
  OutputDebugStringW((const wchar_t*)debugString);
#endif
}

//----------------------------------------------------------------------------------

class MyServices : public ExSystemServices, public ExHostAppServices
{
protected:
  ODRX_USING_HEAP_OPERATORS(ExSystemServices);
  virtual void warning(const char*, const OdString& msg) ODRX_OVERRIDE
  {
    odPrintConsoleString(L"Exception: ");
    odPrintConsoleString(msg.c_str());
    odPrintConsoleString(L"\n");
  }
};

#ifndef _TOOLKIT_IN_DLL_
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(DgnImportModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdDgnModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdDgn7IOModuleImpl);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdRecomputeDimBlockModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdDgnLSModule);
ODRX_BEGIN_STATIC_MODULE_MAP()
  ODRX_DEFINE_STATIC_APPLICATION(OdDgnDbModuleName,               OdDgnModule)
  ODRX_DEFINE_STATIC_APPLICATION(OdDgnImportModuleName,           DgnImportModule)
  ODRX_DEFINE_STATIC_APPLICATION(OdDgn7IOModuleName,              OdDgn7IOModuleImpl)
  ODRX_DEFINE_STATIC_APPMODULE(OdRecomputeDimBlockModuleName,     OdRecomputeDimBlockModule)
  ODRX_DEFINE_STATIC_APPMODULE(OdDgnLSModuleName,                 OdDgnLSModule)
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
#ifdef OD_HAVE_CCOMMAND_FUNC
  argc = ccommand(&argv);
#endif
  
  if (argc < 3) 
  {
    odPrintConsoleString(L"usage: ExDgnImport <source file> <target file>\n");
  }
  else
  {
#ifdef OD_HAVE_SETLOCALE_FUNC
    /********************************************************************/
    /* For correct Unicode translation, apply the current system locale.*/
    /********************************************************************/
    setlocale(LC_ALL, "");
    /********************************************************************/
    /* But use usual conversion for scanf()/sprintf()                   */
    /********************************************************************/
    setlocale(LC_NUMERIC, "C");
#endif

#ifndef _TOOLKIT_IN_DLL_
    ODRX_INIT_STATIC_MODULE_MAP();
#endif

    /********************************************************************/
    /* Create a custom Services instance.                               */
    /********************************************************************/
    OdStaticRxObject<MyServices> svcs;
    odInitialize(&svcs);
    odSetTraceFunc(&myTrace);
    svcs.disableOutput(true);

    /********************************************************************/
    /* Display the Product and Version that created the executable      */
    /********************************************************************/
    odPrintConsoleString(L"Developed using %ls, %ls\n", svcs.product().c_str(), svcs.versionString().c_str());

    try 
    {
      /****************************************************************/
      /* Create importer object                                       */
      /****************************************************************/
      OdStaticRxObject<OdExDgnHostAppServices> dgSvc;
      OdDgnImportPtr importer = createDgnImporter();
      if (importer.isNull())
      {
        odPrintConsoleString(L"Could not create dgn importer\n");
      }
      else
      {
        /****************************************************************************************/
        /* Set the conversion parameters. To get more information about                         */
        /* dgn import parameters see http://docs.opendesign.com/td/#Import_dgn_parameters.html  */
        /****************************************************************************************/
        OdStringArray arrNoRsrc;

        importer->properties()->putAt( L"Services", static_cast<OdDbHostAppServices*>(&svcs) );
        importer->properties()->putAt( L"DgnPath", OdRxVariantValue( OdString(argv[1]) ) );
        dgSvc.disableProgressMeterOutput(true);
        importer->properties()->putAt( L"DgnServices", static_cast<OdDgHostAppServices*>(&dgSvc));
        importer->properties()->putAt( L"XRefImportMode", OdRxVariantValue( OdUInt8(2) ));
        importer->properties()->putAt( L"ImportActiveModelToModelSpace", OdRxVariantValue(true));
        importer->properties()->putAt( L"ImportPaperSpaceModels", OdRxVariantValue(true));
        importer->properties()->putAt( L"ExplodeTextNodes", OdRxVariantValue(false));
        importer->properties()->putAt( L"RecomputeDimensionsAfterImport", OdRxVariantValue(false));
        importer->properties()->putAt( L"MS_SYMBRSRC", OdRxVariantValue(arrNoRsrc));
        importer->properties()->putAt( L"DontImportInvisibleElements", OdRxVariantValue(false));
        importer->properties()->putAt( L"ImportViewIndex", OdRxVariantValue(OdUInt8(-1)));
        importer->properties()->putAt( L"3dEllipseImportMode", OdRxVariantValue(OdUInt8(0)));
        importer->properties()->putAt( L"2dEllipseImportMode", OdRxVariantValue(OdUInt8(0)));
        importer->properties()->putAt( L"3dShapeImportMode", OdRxVariantValue(OdUInt8(1)));
        importer->properties()->putAt( L"2dShapeImportMode", OdRxVariantValue(OdUInt8(0)));
        importer->properties()->putAt( L"3dClosedBSplineCurveImportMode", OdRxVariantValue(OdUInt8(1)));
        importer->properties()->putAt( L"3dClosedBSplineCurveImportMode", OdRxVariantValue(OdUInt8(0)));
        importer->properties()->putAt( L"shxFontsPath", OdRxVariantValue(OdString::kEmpty));
        importer->properties()->putAt( L"BreakDimensionAssociation", OdRxVariantValue(false));

        /**************************************************************/
        /* Import DGN file                                            */
        /**************************************************************/
        OdDgnImport::ImportResult res = importer->import();
        switch (res)
        {
        case OdDgnImport::fail:
          odPrintConsoleString(L"Unknown error.");
          break;
        case OdDgnImport::bad_password:
          odPrintConsoleString(L"Invalid password.");
          break;
        case OdDgnImport::bad_file:
          odPrintConsoleString(L"Cannot open file.");
          break;
        case OdDgnImport::bad_database:
          odPrintConsoleString(L"Invalid file.");
          break;
        case OdDgnImport::encrypted_file:
          odPrintConsoleString(L"Decryption error.");
        case OdDgnImport::success:
          /**************************************************************/
          /* Write DWG file                                             */
          /**************************************************************/
          OdDbDatabasePtr pDb = importer->properties()->getAt(L"Database");
          pDb->writeFile(OdString(argv[2]), OdDb::kDwg, OdDb::vAC24);
          odPrintConsoleString(L"Success.");
          break;
        }
      }
    }
    catch (const OdError& err)
    {
      odPrintConsoleString(L"Teigha Error: %ls\n\n", err.description().c_str());
    }
    catch (...)
    {
      odPrintConsoleString(L"Unknown Error.\n\n");
      return 0;
    }
    odUninitialize();
  }
  return 0;
}
