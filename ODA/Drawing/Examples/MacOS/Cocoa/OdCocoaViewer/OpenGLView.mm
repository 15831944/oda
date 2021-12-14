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
#import "OpenGLView.h"

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

#define GLES2_MODULE // Uncomment to enable GLES2 vectorizer

@implementation OpenGLView
{
    bool _isDeviceInitialized;
}

+ (NSOpenGLPixelFormat*) basicPixelFormat
{
    NSOpenGLPixelFormatAttribute attributes [] = {
        NSOpenGLPFAWindow,
        NSOpenGLPFADoubleBuffer, // double buffered
        NSOpenGLPFADepthSize, (NSOpenGLPixelFormatAttribute)16, // 16 bit depth buffer
        NSOpenGLPFAStencilSize, (NSOpenGLPixelFormatAttribute)2, // 2 bit stencil buffer
        (NSOpenGLPixelFormatAttribute)nil
    };
    return [[[NSOpenGLPixelFormat alloc] initWithAttributes:attributes] autorelease];
}

- (void) resizeGL
{
    if(_isDeviceInitialized == false)
        return;
    
	NSRect rectView = [self bounds];
  if ((curWidth != rectView.size.width ||
      curHeight != rectView.size.height) &&
     (!pDevice.isNull()))
  {
    OdGsDCRect screenRect(OdGsDCPoint(0, rectView.size.height), OdGsDCPoint(rectView.size.width, 0));
    pDevice->onSize(screenRect);
    if (curWidth == 0 && curHeight == 0)
      OdAbstractViewPEPtr(pDevice->viewAt(0))->zoomExtents(pDevice->viewAt(0));
    curWidth = rectView.size.width;
    curHeight = rectView.size.height;
#if 0 // Can be used for GLES2 vectorizer to update GL context in specific cases
    OdRxDictionaryPtr pProperties = pDevice->properties();
    if (!pProperties.isNull())
    {
      if (pProperties->has(OD_T("NSOpenGLContext")) &&
          OdRxVariantValue(pProperties->getAt(OD_T("NSOpenGLContext")))->getIntPtr())
      {
        id glContext = (id)OdRxVariantValue(pProperties->getAt(OD_T("NSOpenGLContext")))->getIntPtr();
        [(NSOpenGLContext*)glContext update];
      }
    }
#endif
  }
}

- (void) drawRect:(NSRect)rect
{
    if(_isDeviceInitialized == false)
        return;
    
	[self resizeGL];

  if (!pDevice.isNull())
    pDevice->update();

#ifndef GLES2_MODULE
	if ([self inLiveResize])
		glFlush ();
	else
		[[self openGLContext] flushBuffer];
    [self clearGLContext];
#endif // GLES2_MODULE
}

- (void) update
{
    if(_isDeviceInitialized == false)
        return;
    
	[super update];
}

-(id) initWithFrame: (NSRect) frameRect
{
	NSOpenGLPixelFormat * pf = [OpenGLView basicPixelFormat];

	self = [super initWithFrame: frameRect pixelFormat: pf];
    return self;
}

- (BOOL)acceptsFirstResponder
{
  return YES;
}

- (BOOL)becomeFirstResponder
{
  return  YES;
}

- (BOOL)resignFirstResponder
{
  return YES;
}

// Diagnostics
#include "diagnostics.h"
static void MyAssert(const char *expression, const char *fileName, int nLineNo)
{
  NSString *errString = [NSString stringWithFormat:@"Assertion failed: %s\nfile: %s\nLine: %d", expression, fileName, nLineNo];
  NSLog(@"%@\n", errString);
}

// Static module map
#if !defined(_TOOLKIT_IN_DLL_)

ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(ModelerModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(ExRasterModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdRasterProcessingServicesImpl);
#ifdef GLES2_MODULE
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdTrGL2RenderModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(GLES2Module);
#else
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(WinOpenGLModule);
#endif
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdRecomputeDimBlockModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdRxThreadPoolService);

ODRX_BEGIN_STATIC_MODULE_MAP()
ODRX_DEFINE_STATIC_APPLICATION(OdModelerGeometryModuleName, ModelerModule)
ODRX_DEFINE_STATIC_APPLICATION(RX_RASTER_SERVICES_APPNAME, ExRasterModule)
ODRX_DEFINE_STATIC_APPLICATION(OdRasterProcessorModuleName, OdRasterProcessingServicesImpl)
#ifdef GLES2_MODULE
ODRX_DEFINE_STATIC_APPMODULE(OdTrGL2ModuleName, OdTrGL2RenderModule)
ODRX_DEFINE_STATIC_APPLICATION(OdWinGLES2ModuleName, GLES2Module)
#else
ODRX_DEFINE_STATIC_APPLICATION(OdWinOpenGLModuleName, WinOpenGLModule)
#endif
ODRX_DEFINE_STATIC_APPLICATION(OdRecomputeDimBlockModuleName, OdRecomputeDimBlockModule)
ODRX_DEFINE_STATIC_APPLICATION(OdThreadPoolModuleName, OdRxThreadPoolService)
ODRX_END_STATIC_MODULE_MAP()

#endif 

- (void) awakeFromNib
{
    _isDeviceInitialized = false;
  NSArray *myArgs = [[NSProcessInfo processInfo]arguments];
#if !defined(_TOOLKIT_IN_DLL_)
  ODRX_INIT_STATIC_MODULE_MAP();
#endif
  odSetAssertFunc(MyAssert);
  odInitialize(&svcs);
  try
  {
    // Load database
    if ([myArgs count] > 1)
    {
      NSString* f0 = [myArgs objectAtIndex:1];
      OdString f([f0 cStringUsingEncoding:[NSString defaultCStringEncoding]]);
      OdInt16 nMode = svcs.getMtMode();
      SETBIT(nMode, 1, true);
      svcs.setMtMode(nMode);
      pDb = svcs.readFile(f.c_str(), false, false, Oda::kShareDenyNo);
        
    }
    if (pDb.isNull())
    {
      pDb = svcs.createDatabase();
    }
    pDwgContext = OdGiContextForDbDatabase::createObject();
    // Load OpenGL vectorizer
#ifndef GLES2_MODULE
    OdGsModulePtr pGs = ::odrxDynamicLinker()->loadModule(OdWinOpenGLModuleName);
#else
    OdGsModulePtr pGs = ::odrxDynamicLinker()->loadModule(OdWinGLES2ModuleName);
#endif
    pDevice = pGs->createDevice();
#ifdef GLES2_MODULE
    OdRxDictionaryPtr pProperties = pDevice->properties();
    if (!pProperties.isNull())
    {
      if (pProperties->has(OD_T("NSView")))
        pProperties->putAt(OD_T("NSView"), OdRxVariantValue((OdIntPtr)self));
      if (pProperties->has(OD_T("NSOpenGLContext")))
        pProperties->putAt(OD_T("NSOpenGLContext"), OdRxVariantValue((OdIntPtr)[self openGLContext]));
    }
#endif
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
    curWidth = 0; curHeight = 0;
      
      _isDeviceInitialized = true;
  }
  catch (const OdError& e)
  {
      NSString *errString = [self WCHARToString:(wchar_t*) e.description().c_str()];
      NSLog(@"%@\n", errString);
  }
  catch (...)
  {
    NSLog(@"Unknown exception caught");
  }
}

- (void) clearGLContext
{
    if(pDevice.isNull())
        return;
    [super clearGLContext];
    
  pDevice.release();
  pDwgContext.release();
  pDb.release();
  odUninitialize();
}

- (NSString*) WCHARToString: (wchar_t*) wcharIn {
    if (NSHostByteOrder() == NS_LittleEndian){
        return [NSString stringWithCString: (char *)wcharIn encoding:NSUTF32LittleEndianStringEncoding];
    }
    else{
        return [NSString stringWithCString: (char *)wcharIn encoding:NSUTF32BigEndianStringEncoding];
    }
}

@end
