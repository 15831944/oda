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

// Simple read and export into raster example
#include "OdaCommon.h"
#include "RxDynamicModule.h"
#include "DbDatabase.h"
#include "ExSystemServices.h"
#include "ExHostAppServices.h"
#include "OdFileBuf.h"
#include "GiContextForDbDatabase.h"
#include "DbGsManager.h"
#include "RxRasterServices.h"
#include "ColorMapping.h"
#include "AbstractViewPE.h"
#include <DbBaseHostAppServices.h>

#define STL_USING_IOSTREAM
#include "OdaSTL.h"
#define  STD(a)  std:: a

// Define module map for statically linked modules:
#if !defined(_TOOLKIT_IN_DLL_)
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(ModelerModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdRecomputeDimBlockModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(ExRasterModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdTrGL2RenderModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(GLES2Module);

ODRX_BEGIN_STATIC_MODULE_MAP()
ODRX_DEFINE_STATIC_APPLICATION(OdModelerGeometryModuleName, ModelerModule)
ODRX_DEFINE_STATIC_APPLICATION(OdRecomputeDimBlockModuleName, OdRecomputeDimBlockModule)
ODRX_DEFINE_STATIC_APPMODULE(RX_RASTER_SERVICES_APPNAME, ExRasterModule)
ODRX_DEFINE_STATIC_APPLICATION(OdTrGL2ModuleName, OdTrGL2RenderModule)
ODRX_DEFINE_STATIC_APPLICATION(OdWinGLES2ModuleName, GLES2Module)
ODRX_END_STATIC_MODULE_MAP()
#endif

// Simple export to raster example
// - this example shows how to export .dwg file to raster image

class MyServices : public ExSystemServices, public ExHostAppServices
{
protected:
  ODRX_USING_HEAP_OPERATORS(ExSystemServices);
};

static bool is_dark_palette(ODCOLORREF bgColor)
{
  return (ODGETRED(bgColor) < 140) && (ODGETGREEN(bgColor) < 140) && (ODGETBLUE(bgColor) < 140);
}

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

    // ***** convert file to raster image
    // - initialize device
    OdString sGS(/*OdWinOpenGLModuleName*/OdWinGLES2ModuleName); // *OdWinOpenGLModuleName - doesn't support transparency well
    OdDb::RenderMode rMode = OdDb::kFlatShaded;
    OdGsModulePtr pGsModule = ::odrxDynamicLinker()->loadModule(sGS);
    OdGsDevicePtr pBitmapDevice;
    if (pGsModule.get())
    {
      pBitmapDevice = pGsModule->createBitmapDevice();
    }
    if (!pBitmapDevice.get())
    {
      throw OdError(eDeviceNotFound);
    }

    // - setup render params
    OdGiContextForDbDatabasePtr pDwgContext = OdGiContextForDbDatabase::createObject();
    pDwgContext->setDatabase(pDb);
    pDwgContext->enableGsModel(false);
    OdGsDevicePtr pDevice = OdDbGsManager::setupActiveLayoutViews(pBitmapDevice, pDwgContext);
    ODCOLORREF bgColor = ODRGB(0, 0, 0); // set background color
    pDevice->setLogicalPalette((is_dark_palette(bgColor)) ? (::odcmAcadDarkPalette()) : (::odcmAcadLightPalette()), 256);
    pDevice->setBackgroundColor(bgColor);
    pDwgContext->setPaletteBackground(bgColor);

    // - setup output dims
    OdGsDCRect screenRect(OdGsDCPoint(0, 1024), OdGsDCPoint(1024, 0));
    pDevice->onSize(screenRect);

    // - zoom to extents
    OdGsView *pView = pDevice->viewAt(0);
    OdAbstractViewPEPtr(pView)->zoomExtents(pView);
    OdAbstractViewPEPtr(pView)->setRenderMode(pView, rMode);

    // - render image (internal)
    pDevice->update();

    // - save image
    // - NOTICE that type of raster file is deducted from example input (e.g. if output file is D:\picture.bmp image will be saved as .bmp)
    OdGiRasterImagePtr ptrImage = pDevice->properties()->getAt(OD_T("RasterImage"));
    OdRxRasterServicesPtr pRasSvcs = ::odrxDynamicLinker()->loadApp(RX_RASTER_SERVICES_APPNAME);
    if (pRasSvcs.get())
    {
      OdString img(argv[2]);
      pRasSvcs->saveRasterImage(ptrImage, img);
    }
    // ***** conversion completed
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
