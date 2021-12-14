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
/* This console application reads a DWF file, and saves it to a DWG     */
/* file in the current version                                          */
/*                                                                      */
/* Calling sequence:                                                    */
/*                                                                      */
/*    OdDwfImportEx <source file> <target file>                         */
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
#include "../../Imports/DwfImport/Include/DwfImport.h"
#include <locale.h>
#include "diagnostics.h"

using namespace TD_DWF_IMPORT;
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
};

#ifndef _TOOLKIT_IN_DLL_
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(Dwf7ImportModule);
ODRX_BEGIN_STATIC_MODULE_MAP()
  ODRX_DEFINE_STATIC_APPLICATION(OdDwf7ImportModuleName,   Dwf7ImportModule)
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
    odPrintConsoleString(L"usage: OdDwfImportEx <source file> <target file>\n");
  }
  else
  {
    /********************************************************************/
    /* For correct Unicode translation, apply the current system locale.*/
    /********************************************************************/
    setlocale(LC_ALL, "");
    /********************************************************************/
    /* But use usual conversion for scanf()/sprintf()                   */
    /********************************************************************/
    setlocale(LC_NUMERIC, "C");

#ifndef _TOOLKIT_IN_DLL_
    ODRX_INIT_STATIC_MODULE_MAP();
#endif

    /********************************************************************/
    /* Create a custom Services instance.                               */
    /********************************************************************/
    OdStaticRxObject<MyServices> svcs;
    svcs.setRecomputeDimBlocksRequired(false);
    odInitialize(&svcs);
    odSetTraceFunc(&myTrace);


    /********************************************************************/
    /* Display the Product and Version that created the executable      */
    /********************************************************************/
    odPrintConsoleString(L"Developed using %ls, %ls\n", svcs.product().c_str(), svcs.versionString().c_str());

    try 
    {
      /****************************************************************/
      /* Create importer object                                       */
      /****************************************************************/
      OdDwfImportPtr importer = createImporter();
      if (importer.isNull())
      {
        odPrintConsoleString(L"Could not create dwf importer\n");
      }
      else
        /****************************************************************/
        /* Import DWF file                                              */
        /****************************************************************/
      {
        OdDbDatabasePtr pDb = svcs.createDatabase();
        /**************************************************************/
        /* Set the conversion parameters                              */
        /**************************************************************/
        importer->properties()->putAt( L"Database", pDb );
        importer->properties()->putAt( L"DwfPath", OdRxVariantValue( OdString(argv[1]) ) );
        importer->properties()->putAt( L"Password", OdRxVariantValue( OdString::kEmpty ) );
        importer->properties()->putAt( L"PaperWidth", OdRxVariantValue( 297. ) );
        importer->properties()->putAt( L"PaperHeight", OdRxVariantValue( 210. ) );
        /**************************************************************/
        /* If color indices are preserved, RGB color may differ       */
        /**************************************************************/
        importer->properties()->putAt( L"PreserveColorIndices", OdRxVariantValue(true) );
        /**************************************************************/
        /* Select which layout to import (-1, for all)                             */
        /**************************************************************/
        importer->properties()->putAt( L"LayoutNumber", OdRxVariantValue(OdInt32(-1)) );
        /**************************************************************/
        /* Enable importing HSF 3d model from new DWF files           */
        /**************************************************************/
        importer->properties()->putAt( L"ImportW3d", OdRxVariantValue(true) );
        /**************************************************************/
        /* Import DWF file                                            */
        /**************************************************************/
        importer->import();
        importer = 0;
        /**************************************************************/
        /* Write DWG file                                             */
        /**************************************************************/
        OdString out(argv[2]);
        OdWrFileBuf fb(out);
        pDb->writeFile(&fb, OdDb::kDwg, OdDb::kDHL_CURRENT);
      }
    }
    catch (OdError& err)
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
