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
// Simple TD viewing sample for X11 window system

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
// TD include
#include "OdaCommon.h"
#include "DbDatabase.h"
#include "DbGsManager.h"
#include "RxDynamicModule.h"
#include "RxObjectImpl.h"
#include "RxVariantValue.h"
#include "AbstractViewPE.h"
#include "ColorMapping.h"
#include "GiContextForDbDatabase.h"
#include "ExSystemServices.h"
#include "ExHostAppServices.h"
// X11 includes
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>

// Forward declarations
void DisplayFunc(OdGsDevice *pDevice);
void IdleFunc(OdGsDevice *pDevice);
void ReshapeFunc(OdGsDevice *pDevice, int width, int height);

// Services class
class MyServices : public ExSystemServices, public ExHostAppServices
{
  protected:
    ODRX_USING_HEAP_OPERATORS(ExSystemServices);
};

// Diagnostics
#include "diagnostics.h"
static void MyAssert(const char *expression, const char *fileName, int nLineNo)
{
  std::cout << "\nAssertion failed: " << expression << "\nfile: " << fileName << "\nLine: " << nLineNo << "\n";
}

#if 0
#include "RxRasterServices.h"
#include "Gi/GiRasterImage.h"
void bmpRenderTest(OdDbDatabase *pDb)
{
  printf("bmpRenderTest begin\n");
  OdGsModulePtr pGs = odrxDynamicLinker()->loadModule(OdWinGLES2ModuleName, false);
  if (!pGs.get())
    return;
  OdGsDevicePtr pDevice = pGs->createBitmapDevice();
  OdGiContextForDbDatabasePtr pCtx = OdGiContextForDbDatabase::createObject();
  pCtx->setDatabase(pDb);
  pDevice = OdDbGsManager::setupActiveLayoutViews(pDevice, pCtx);
  int nColorBits = 24;
  int xsize = 2000;
  int ysize = 2000;
  bool plotGeneration = true;
  ODCOLORREF bg = ODRGB(0, 0, 0); // palette background
  OdRxRasterServicesPtr pRasSvcs = odrxDynamicLinker()->loadApp(RX_RASTER_SERVICES_APPNAME);
  ODA_ASSERT_ONCE(pRasSvcs.get())
  if (pRasSvcs.isNull())
    throw OdError(eNotApplicable);
  OdString m_sOutput = OD_T("~/Bitmap.bmp");
  pDevice->properties()->putAt(OD_T("BitPerPixel"), OdRxVariantValue(OdUInt32(nColorBits)));
  pDevice->setBackgroundColor(ODRGB(173,174,173)); // ACAD's color for paper bg, "device background"
  pCtx->setPlotGeneration(plotGeneration);
  pCtx->setPaletteBackground(bg);
  const ODCOLORREF* palette = odcmAcadPalette(bg);
  OdArray<ODCOLORREF> pPalCpy;
  pPalCpy.insert(pPalCpy.begin(), palette, palette+256);
  pPalCpy[0] = bg;
  pDevice->setLogicalPalette(pPalCpy.asArrayPtr(), 256);
  OdGsDCRect gsRect(0, xsize, ysize, 0 );
  pDevice->onSize(gsRect);
  if(pCtx->isPlotGeneration())
    pDevice->setBackgroundColor(bg);
  else
    pDevice->setBackgroundColor(ODRGB(173,174,173)); // ACAD's color for paper bg, "device background"
  pDevice->update();
  pRasSvcs->saveRasterImage(OdGiRasterImagePtr(pDevice->properties()->getAt(OD_T("RasterImage"))), 
                            m_sOutput);
  printf("bmpRenderTest end\n");
}
#endif

// Static module map
#if !defined(_TOOLKIT_IN_DLL_)

ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(ModelerModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(ExRasterModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdRasterProcessingServicesImpl);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(WinOpenGLModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdRecomputeDimBlockModule);

ODRX_BEGIN_STATIC_MODULE_MAP()
  ODRX_DEFINE_STATIC_APPLICATION(OdModelerGeometryModuleName, ModelerModule)
  ODRX_DEFINE_STATIC_APPLICATION(RX_RASTER_SERVICES_APPNAME, ExRasterModule)
  ODRX_DEFINE_STATIC_APPLICATION(OdRasterProcessorModuleName, OdRasterProcessingServicesImpl)
  ODRX_DEFINE_STATIC_APPLICATION(OdWinOpenGLModuleName, WinOpenGLModule)
  ODRX_DEFINE_STATIC_APPLICATION(OdRecomputeDimBlockModuleName,   OdRecomputeDimBlockModule)
ODRX_END_STATIC_MODULE_MAP()

#endif

// Main
int main(int argc, char *argv[])
{
  bool bMapped;
  Display *pDisplay = NULL;
  Window window;
  XVisualInfo *pVInfo;
  XSetWindowAttributes winattrs;
  int winmask;
  XEvent event;
  XWindowAttributes windata;

  std::cout << "\nOdX11Viewer running . . .\n";
  std::cout << "Command line: OdX11Viewer <inFileName> - optional\n";

  // Initialize TD
#if !defined(_TOOLKIT_IN_DLL_)
  ODRX_INIT_STATIC_MODULE_MAP();
#endif

  odSetAssertFunc(MyAssert);

  OdStaticRxObject<MyServices> svcs;
  odInitialize(&svcs);

  try
  {
    // Load database
    OdDbDatabasePtr pDb;
    bool bGLES2 = false;
    if (argc > 1)
    {
      OdString f(argv[1]);
      if (!f.compare(OD_T("GLES2")))
      {
        bGLES2 = true;
        f = argv[2];
      }
      pDb = svcs.readFile(f.c_str(), false, false, Oda::kShareDenyNo);
    }
    if (pDb.isNull())
    {
      pDb = svcs.createDatabase();
    }

    //bmpRenderTest(pDb);

    OdGiContextForDbDatabasePtr pDwgContext = OdGiContextForDbDatabase::createObject();
    // Load OpenGL vectorizer
    OdGsModulePtr pGs = ::odrxDynamicLinker()->loadModule((!bGLES2) ? OdWinOpenGLModuleName : OdWinGLES2ModuleName);
    OdGsDevicePtr pDevice = pGs->createDevice();

    // Begin OpenGL initialization
    OdRxDictionaryPtr pProperties = pDevice->properties();

    // X11 display
    pDisplay = XOpenDisplay(getenv("DISPLAY"));
    if (pDisplay != NULL)
    {
    pProperties->putAt(OD_T("XDisplay"), OdRxVariantValue((OdIntPtr)pDisplay));
    // X11 visual info
    pVInfo = (XVisualInfo*)OdRxVariantValue(pProperties->getAt(OD_T("XChooseVisual")))->getIntPtr();
    ODA_ASSERT(pVInfo != NULL);
    // Create X11 window
    winattrs.event_mask = ExposureMask | VisibilityChangeMask | StructureNotifyMask;
    winattrs.border_pixel = 0;
    winattrs.bit_gravity = StaticGravity;
    winattrs.background_pixel = 0;
    winattrs.colormap = XCreateColormap(pDisplay, DefaultRootWindow(pDisplay), pVInfo->visual, AllocNone);
    winmask = CWBorderPixel | CWBitGravity | CWEventMask | CWBackPixel | CWColormap;
    window = XCreateWindow(pDisplay, DefaultRootWindow(pDisplay), 0, 0, 640, 480, 0, pVInfo->depth, InputOutput, pVInfo->visual, winmask, &winattrs);
    pProperties->putAt(OD_T("XWindow"), OdRxVariantValue((OdIntPtr)window));
    // Setup window name
    XChangeProperty(pDisplay, window, XA_WM_NAME, XA_STRING, 8, 0, (unsigned char*)argv[0], strlen(argv[0]));
    XChangeProperty(pDisplay, window, XA_WM_ICON_NAME, XA_STRING, 8, 0, (unsigned char*)argv[0], strlen(argv[0]));
    Atom wmDeleteMessage = XInternAtom(pDisplay, "WM_DELETE_WINDOW", false);
    XSetWMProtocols(pDisplay, window, &wmDeleteMessage, 1);
    // Initialize Gs device
    pDwgContext->setDatabase(pDb);
    pDevice = OdDbGsManager::setupActiveLayoutViews(pDevice, pDwgContext);
    if (pDb->getTILEMODE())
    {
      pDevice->setLogicalPalette(::odcmAcadDarkPalette(), 256);
      pDevice->setBackgroundColor(ODRGB(0, 0, 0));
      pDwgContext->setPaletteBackground(ODRGB(0, 0, 0));
    }
    else
    {
      pDevice->setLogicalPalette(::odcmAcadLightPalette(), 256);
      pDevice->setBackgroundColor(ODRGB(168, 168, 168));
      pDwgContext->setPaletteBackground(ODRGB(255, 255, 255));
    }
    // Resize
    OdGsDCRect screenRect(OdGsDCPoint(0, 480), OdGsDCPoint(640, 0));
    pDevice->onSize(screenRect);
    // Zoom to extents
    OdAbstractViewPEPtr(pDevice->viewAt(0))->zoomExtents(pDevice->viewAt(0));
    // Window loop
    XMapWindow(pDisplay, window);
    bMapped = false;
    for (;;)
    {
      XNextEvent(pDisplay, &event);
      switch (event.type)
      {
        case MapNotify:
          bMapped = true;
        case ConfigureNotify:
          XGetWindowAttributes(pDisplay, window, &windata);
          ReshapeFunc(pDevice, windata.width, windata.height);
        break;
        case UnmapNotify:
          bMapped = false;
        break;
      }
      if (event.type == ClientMessage && event.xclient.data.l[0] == wmDeleteMessage)
        break;
      if (bMapped)
      {
        IdleFunc(pDevice);
        DisplayFunc(pDevice);
      }
    }
    } else
    {
      std::cout <<"\nOdX11Viewer: DISPLAY is not set. Exiting...\n";
    }
  }
  catch (const OdError& e)
  {
    std::cout << "\nException caught: " << e.description().c_str() << "\n";
  }
  catch (...)
  {
    std::cout << "\nUnknown exception caught\n";
  }

  odUninitialize();

  if (pDisplay)
  {
    XCloseDisplay(pDisplay);
  }

  return 0;
}

void DisplayFunc(OdGsDevice *pDevice)
{
  pDevice->update();
}

void IdleFunc(OdGsDevice*)
{
}

void ReshapeFunc(OdGsDevice *pDevice, int width, int height)
{
  OdGsDCRect screenRect(OdGsDCPoint(0, height), OdGsDCPoint(width, 0));
  pDevice->onSize(screenRect);
}

//
