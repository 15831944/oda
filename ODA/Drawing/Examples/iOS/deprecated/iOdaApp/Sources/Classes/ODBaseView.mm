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

#import "ODBaseView.h"
#import "QuartzCore/CAEAGLLayer.h"
#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES1/glext.h>
#import "iOdaApplication.h"
#import "iUtils.h"
#import "iStrConv.h"

#include <Gs/Gs.h>
#include <DbDatabase.h>
#include <GiContextForDbDatabase.h>
#include <DbGsManager.h>
#include "iOdaAppServices.h"
#include <Gs/GsBaseVectorizer.h>
#include <RxVariantValue.h>
#include <RxDictionary.h>
#include <ColorMapping.h>
#include <AbstractViewPE.h>

// Enable this definition to use OpenGL ES 2.0 vectorizer
//#define USE_GLES2_DEVICE

@implementation ODBaseView

@synthesize glContext;
@synthesize strCadFile;
@synthesize gsDevice;
@synthesize pDb;

-(void) awakeFromNib
 {
  [super awakeFromNib];
  NSLog(@"awakeFromNib");
  // Initialize OpenGL context
  [self InitOpenGLES];
  // Initialize GS
  odgsInitialize();
}

- (id)initWithFrame:(CGRect)frame
{
  self = [super initWithFrame:frame];
  if (self) {
    // Initialization code.
  }
  return self;
}

- (void)dealloc
{
  // Uninitialize OpenGL context
  [self UninitOpenGLES];
  // Uninitialize GS
  odgsUninitialize();
  [super dealloc];
}

-(void) draw:(BOOL)resize
 {
#ifndef USE_GLES2_DEVICE
  // Make context current for WinOpenGL vectorizer
  [EAGLContext setCurrentContext:glContext];
#endif
  if (gsDevice.isNull())
    return;
  // Set actual screen size for the generated geometry
  if (resize)
    [self resizeGsDevice:gsDevice.get()];
  // Draw geometry
  gsDevice->update();		
}

// You must implement this method
+ (Class)layerClass
{
  return [CAEAGLLayer class];
}

-(BOOL)InitOpenGLES
{
  // Get the layer
  CAEAGLLayer *eaglLayer = (CAEAGLLayer*)self.layer;
  // Setup layer
  eaglLayer.opaque = TRUE;
  eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
    [NSNumber numberWithBool:FALSE], kEAGLDrawablePropertyRetainedBacking, kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat, nil];
#ifndef USE_GLES2_DEVICE
  // Setup OpenGL context for WinOpenGL vectorizer
  glContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
  if (!glContext || ![EAGLContext setCurrentContext:glContext])
    return NO;
#endif
  return YES;
}

-(void)UninitOpenGLES
{
  if (!gsDevice.isNull())
  {
#ifndef USE_GLES2_DEVICE
    // Make context current for WinOpenGL vectorizer
    [EAGLContext setCurrentContext:glContext];
#endif
    // Release vectorizer and render buffers
    gsDevice.release();
  }
#ifndef USE_GLES2_DEVICE
  // Tear down OpenGL context for WinOpenGL vectorizer
  if ([EAGLContext currentContext] == glContext)
    [EAGLContext setCurrentContext:nil];
  [glContext release];
  glContext = nil;
#endif
}

-(BOOL)resizeFromLayer:(CAEAGLLayer*)layer
{
#ifndef USE_GLES2_DEVICE
  // Setup OpenGL render buffer from layer for WinOpenGL vectorizer
  [glContext renderbufferStorage:GL_RENDERBUFFER_OES fromDrawable:layer];
#endif
  return YES;
}

-(void)resizeGsDevice:(OdGsDevice*)pDevice
{
  // Get size of our current window
  CGSize layerRect = self.layer.bounds.size;
  backingWidth = (GLint)layerRect.width;
  backingHeight = (GLint)layerRect.height;
  // Set the screen size for the generated geometry.
  OdGsDCRect screenRect(OdGsDCPoint(0, backingHeight), OdGsDCPoint(backingWidth, 0));
  gsDevice->onSize(screenRect);
}

-(void)protoDelegateFromDrawable
{
  // protoDelegate implementation for WinOpenGL vectorizer
  [self resizeFromLayer:(CAEAGLLayer*)self.layer];
}

void EAGLProtoRedirector::eaglProtoFromDrawable()
{
  [m_pDelegate protoDelegateFromDrawable];
}

-(void)protoDelegatePresentBuffer
{
  // protoDelegate implementation for WinOpenGL vectorizer
  [glContext presentRenderbuffer:GL_RENDERBUFFER_OES];
}

void EAGLProtoRedirector::eaglProtoPresentRenderBuffer()
{
  [m_pDelegate protoDelegatePresentBuffer];
}

-(void)layoutSubviews 
{
  // This method will be called on frame initialization and on autorotation
  [NSThread sleepForTimeInterval:5.0/1000];
  // Load file on first initialization or update screen on autorotation
  if (gsDevice.isNull() && [self LoadCadFile])
    [self draw:FALSE];
  else
    [self draw:TRUE];
}

-(BOOL) LoadCadFile
{
  BOOL rtn = false;
  try 
  {
    // Load file
    NSString* fileFullPath = [iUtils sharedDocumentFullPath:strCadFile];
    pDb = iOdaAppServices::getInstance()->readFile(NSString2OdString(fileFullPath));
    if (pDb.isNull())
    {
      // load database failed
    }
    else 
    {
      // Create the rendering context.
      OdGiContextForDbDatabasePtr pDwgContext = OdGiContextForDbDatabase::createObject();
      // Create the custom rendering device and set the output stream for the device.
      if (!gsDevice.isNull())
      {
        gsDevice.release();
      }
      if (gsDevice.isNull())
      {
        // Load graphics device module
#ifndef USE_GLES2_DEVICE
        OdGsModulePtr m = ::odrxDynamicLinker()->loadModule(OdWinOpenGLModuleName);
#else
        OdGsModulePtr m = ::odrxDynamicLinker()->loadModule(OdWinGLES2ModuleName);
#endif
        // Create graphics device
        gsDevice = m->createDevice();
        // Prepare callbacks for WinOpenGL vectorizer
        m_protoCallback.m_pDelegate = self;
        // Setup graphics device properties
        OdRxDictionaryPtr pProps = gsDevice->properties();
        if (!pProps.isNull())
        {
#ifndef USE_GLES2_DEVICE
          // This property must be set for WinOpenGL device only
          if (pProps->has(OD_T("EAGLProto")))
          {
            pProps->putAt(OD_T("EAGLProto"), OdRxVariantValue((OdIntPtr)&m_protoCallback));
          }
          else
          {
            NSLog(@"\nCan't access necessary device property");
          }
#endif
        }
        else
        {
          NSLog(@"\nCan't access device properties");
        }
      }
      // Set the database to be vectorized.
      pDwgContext->setDatabase(pDb);
      // Prepare the device to render the active layout in this database.
      gsDevice = OdDbGsManager::setupActiveLayoutViews(gsDevice, pDwgContext);
      // Setup device palette
      if (OdGsPaperLayoutHelper::cast(gsDevice).isNull())
      {
        // ModelSpace
        gsDevice->setLogicalPalette(::odcmAcadDarkPalette(), 256);
        gsDevice->setBackgroundColor(ODRGB(0, 0, 0));
        pDwgContext->setPaletteBackground(ODRGB(0, 0, 0));
      }
      else
      {
        gsDevice->setLogicalPalette(::odcmAcadLightPalette(), 256);
        gsDevice->setBackgroundColor(ODRGB(173, 174, 173));
        pDwgContext->setPaletteBackground(ODRGB(255, 255, 255));
      }
      // Setup rendering surface size
      [self resizeGsDevice:gsDevice.get()];
      // Zoom drawing to extents
      OdAbstractViewPEPtr(gsDevice->viewAt(0))->zoomExtents(gsDevice->viewAt(0));
#ifdef USE_GLES2_DEVICE
      // Layer must be set for WinGLES2 device
      gsDevice->properties()->putAt(OD_T("EAGLLayer"), OdRxVariantValue((OdIntPtr)self.layer));
#endif
      rtn = true;
    }
  }
  catch (int)
  {
    NSLog(@"\nNormal termination");
  }
  catch (const OdError& e)
  {
    printf("\nException Caught: %ls\n", e.description().c_str());
  }
  catch (...)
  {
    NSLog(@"\nUnknown Exception Caught\n");
  }				
  return rtn;
}

@end
