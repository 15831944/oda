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
/* This console application reads a DWG file and vectorizes its         */
/* contents to the console.                                             */
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
#include "ExGsSimpleDevice.h"
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
/* Define our own Assert function.                                      */
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
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdRecomputeDimBlockModule);

ODRX_BEGIN_STATIC_MODULE_MAP()
  ODRX_DEFINE_STATIC_APPLICATION(OdModelerGeometryModuleName,  ModelerModule)
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
    odPrintConsoleString(
      L"usage: OdVectorizeEx <input file>\n"
      L"  Output is written to stdout, redirect if necessary.\n");
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
    /* Load database from file                                        */
    /******************************************************************/
    OdDbDatabasePtr pDb;
    OdString f(argv[1]);
    pDb = svcs.readFile( f.c_str()); 

    if (!pDb.isNull())
    {
      /****************************************************************/
      /* Create the vectorization context.                            */
      /* This class defines the operations and properties that are    */
      /* used in the vectorization of an OdDbDatabase.                */
      /****************************************************************/
      OdGiContextForDbDatabasePtr pDwgContext = OdGiContextForDbDatabase::createObject();
      /****************************************************************/
      /* Create the custom rendering device, and set the output       */
      /* stream for the device.                                       */
      /* This class implements bitmapped GUI display windows.         */
      /****************************************************************/
      OdGsDevicePtr pDevice = ExGsSimpleDevice::createObject(ExGsSimpleDevice::k3dDevice);

      /****************************************************************/
      /* Set the database to be vectorized and background.            */
      /****************************************************************/
      pDwgContext->setDatabase(pDb);
      pDwgContext->setPaletteBackground(pDevice->getBackgroundColor());

      /****************************************************************/
      /* Prepare the device to render the active layout in            */
      /* this database.                                               */
      /****************************************************************/
      pDevice = OdDbGsManager::setupActiveLayoutViews(pDevice, pDwgContext);

      /****************************************************************/
      /* Set the screen size for the generated (vectorized) geometry. */
      /****************************************************************/
      OdGsDCRect screenRect(OdGsDCPoint(0, 0), OdGsDCPoint(1000, 1000));
      pDevice->onSize(screenRect);

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
    odPrintConsoleString(L"\nOdVectorizeEx Finished Successfully");
  }
  return nRes;
}
