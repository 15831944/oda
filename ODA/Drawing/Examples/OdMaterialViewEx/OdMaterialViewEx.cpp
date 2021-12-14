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
/* This console application reads a DWG file and dumps texture          */
/* coordinates for all 3d solid entities to the console.                */
/*                                                                      */
/* Calling sequence:                                                    */
/*                                                                      */
/*    OdMaterialViewEx <input file>                                     */
/*                                                                      */
/************************************************************************/

#include "OdaCommon.h"
#include "DbDatabase.h"
#include "DbGsManager.h"

#include "RxObjectImpl.h"
#define STL_USING_IOSTREAM
#include "OdaSTL.h"
#include <fstream>
#include <locale.h>

#include "ExSystemServices.h"
#include "ExHostAppServices.h"
#include "ExGsMaterialDumpDevice.h"
#include "GiContextForDbDatabase.h"

#include "RxDynamicModule.h"

#ifdef OD_HAVE_CONSOLE_H_FILE
#include <console.h>
#endif

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
#include "diagnostics.h"

static void MyAssert(const char* expresssion, const char* fileName, int nLineNo)
{
  std::cout << "\nAssertion Failed: " << expresssion
    << "\nfile: " << fileName << "\nline: " << nLineNo << "\n";
}

/************************************************************************/
/* Define a module map for statically linked modules                    */
/************************************************************************/
#if !defined(_TOOLKIT_IN_DLL_)

ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(ModelerModule);

ODRX_BEGIN_STATIC_MODULE_MAP()
  ODRX_DEFINE_STATIC_APPLICATION(OdModelerGeometryModuleName,  ModelerModule)
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
  int nRes = 1; // Return value for main

#ifdef OD_HAVE_CCOMMAND_FUNC
  argc = ccommand(&argv);
#endif
  setlocale(LC_ALL,"");
  /**********************************************************************/
  /* Verify the argument count and display an error message as required */
  /**********************************************************************/
  if (argc < 2) 
  {
    odPrintConsoleString(L"usage: OdMaterialViewEx <input file>\n  Output is written to stdout, redirect if necessary.\n");
    return 1;
  }

#if !defined(_TOOLKIT_IN_DLL_)
  ODRX_INIT_STATIC_MODULE_MAP();
#endif
  odSetAssertFunc(MyAssert);

  // Create a custom Services object.
  OdStaticRxObject<MyServices> svcs;
  // initialize the Drawings SDK
  odInitialize(&svcs);
  // initialize GS subsystem
  odgsInitialize();

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
      /****************************************************************/
      /* Create the vectorization context.                            */
      /* This class defines the operations and properties that are    */
      /* used in vectorization of an OdDbDatabase.                    */
      /****************************************************************/
      OdGiContextForDbDatabasePtr pDwgContext = OdGiContextForDbDatabase::createObject();

      /****************************************************************/
      /* Create the custom rendering device, and set the output       */
      /* stream for the device.                                       */
      /****************************************************************/
      OdGsDevicePtr pDevice = ExGsMaterialDumpDevice::createObject();

      /****************************************************************/
      /* Set the database to be vectorized.                           */
      /****************************************************************/
      pDwgContext->setDatabase(pDb);

      /****************************************************************/
      /* Switch to model space layout if paper space layout enabled.  */
      /****************************************************************/
      if (pDb->getTILEMODE() == 0)
      {
        odPrintConsoleString(L"\nSwitching to Model Space layout.\n");
        pDb->setTILEMODE(1);
      }

      /****************************************************************/
      /* Prepare the device to render the active layout in            */
      /* this database.                                               */
      /****************************************************************/
      pDevice = OdDbGsManager::setupActiveLayoutViews(pDevice, pDwgContext);

      /****************************************************************/
      /* No need to set screen rectangle, because coordinates of the  */
      /* geometry will be dumped in World Coordinate Space (WCS).     */
      /****************************************************************/
      //OdGsDCRect screenRect(OdGsDCPoint(0, 0), OdGsDCPoint(1000, 1000));
      //pDevice->onSize(screenRect);

      /****************************************************************/
      /* Initiate Vectorization                                       */
      /****************************************************************/
      pDevice->update();
    }
    odPrintConsoleString(L"\nDone.\n");
    nRes = 0;
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
  odgsUninitialize();
  odUninitialize();

  if (!nRes)
  {
    odPrintConsoleString(L"\nOdMaterialViewEx Finished Successfully");
  }
  return nRes;
}

//
