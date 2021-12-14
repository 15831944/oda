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


#import <Foundation/Foundation.h>
#import "DgnViewController.h"

#include <Gs/GsBaseVectorizer.h>
#include <RxVariantValue.h>
#include <RxDictionary.h>
#include <ColorMapping.h>
#include <AbstractViewPE.h>

#include "iOdaAppServices.h"
#import "iStrConv.h"

#include "Gi/GiBaseVectorizer.h"
#include "DgGiContext.h"


@interface DgnViewController ()
@end

@implementation DgnViewController

@synthesize cadFileName;
@synthesize glContext;

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.

    NSLog(@"DgnViewController: %@", cadFileName);
    backingWidth = 0;
    backingHeight = 0;
    prevAngleRad = 0.0;
    prevScale = 1.0;
    prevPanPoint = {0.0, 0.0};
    
    self.glContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2 ];
    
    if (!self.glContext) {
        NSLog(@"Failed to create OpenGL ES 2.0 context");
    }
 
    
  //  [EAGLContext setCurrentContext:self.glContext]; // VERY important! GL silently stops working without this
    
    GLKView *view = (GLKView *)self.view;
    view.context = self.glContext;
    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
    
    [self setupGL];
    odgsInitialize();
    [self LoadCadFile];
}

- (void)setupGL
{
    [EAGLContext setCurrentContext:self.glContext];
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
    if (gsDevice.isNull())
        return;
    // Set actual screen size for the generated geometry
    [self resizeGsDevice:rect];
    // Draw geometry
    gsDevice->invalidate();
    gsDevice->update();

}


-(void)resizeGsDevice: (CGRect)rect
{
    // Get size of our current window
    if (((GLint)rect.size.width != backingWidth) ||
        ((GLint)rect.size.height != backingHeight))
    {
      backingWidth = (GLint)rect.size.width;
      backingHeight = (GLint)rect.size.height;
      // Set the screen size for the generated geometry.
      double scaleF = [[UIScreen mainScreen] scale];
        
      OdGsDCRect screenRect( OdGsDCPoint(0, self.view.frame.size.height*scaleF),
                             OdGsDCPoint(self.view.frame.size.width*scaleF, 0) );
      gsDevice->onSize(screenRect);
    }
}

-(void)UninitOpenGLES
{
    if (!gsDevice.isNull())
    {
        // Release vectorizer and render buffers
        gsDevice.release();
    }
}

- (void)dealloc
{
    if ([EAGLContext currentContext] == self.glContext)
    {
        [EAGLContext setCurrentContext:nil];
    }
    
    self.glContext = nil;
    
    // Uninitialize OpenGL context
    [self UninitOpenGLES];
    // Uninitialize GS
    odgsUninitialize();
}


- (OdGePoint3d)toWCS: (CGFloat)x andY:(CGFloat) y
{
    OdGePoint3d wcsPt((double)x, (double)y, 0.0);
    const OdGsView* pView = gsDevice->viewAt(0);
    if (pView->isPerspective())
    {
        wcsPt.z = pView->projectionMatrix()(2, 3);
    }
    wcsPt.transformBy((pView->screenMatrix() * pView->projectionMatrix()).inverse());
    wcsPt.z =0.;
    wcsPt.transformBy(OdAbstractViewPEPtr(pView)->eyeToWorld(pView));
    return wcsPt;
}

- (IBAction)handlePan:(UIPanGestureRecognizer *)recognizer {
    
    CGPoint translation = [recognizer translationInView:self.view];
   
    OdGsView* pView = gsDevice->viewAt(0);
    OdGeVector3d delta = [self toWCS:-translation.x andY:-translation.y] - [self toWCS:-prevPanPoint.x andY:-prevPanPoint.y];
    delta.transformBy(pView->viewingMatrix());
    pView->dolly(delta.x, delta.y, delta.z);

    prevPanPoint = translation;
    if ([recognizer state] == UIGestureRecognizerStateEnded)
    {
        prevPanPoint = {0.0, 0.0};
    }
}


- (IBAction)handlePinch:(UIPinchGestureRecognizer *)recognizer
{
    if (!gsDevice.isNull())
    {
        OdGsView* pView = gsDevice->viewAt(0);
        CGFloat scale = 1.0 - (prevScale - recognizer.scale);
        //pView->zoom((double)(1.0 + (recognizer.scale - 1.0)/16.0));
        pView->zoom((double)(scale));
        prevScale = recognizer.scale;
        if ([recognizer state] == UIGestureRecognizerStateEnded)
        {
            prevScale = 1.0;
        }
    }
}

- (bool)viewCanRotate
{
    if (pDb.isNull())
    {
        return false;
    }
    return false;
}


- (IBAction)handleRotate:(UIRotationGestureRecognizer *)recognizer
{
    CGFloat curAngleRad = recognizer.rotation;
    
    if (!gsDevice.isNull() && [self viewCanRotate])
    {
        OdGsView* pView = gsDevice->viewAt(0);
        double angle = ((curAngleRad - prevAngleRad)/90)*180/OdaPI;
        pView->roll(angle);
        prevAngleRad = recognizer.rotation;
        if ([recognizer state] == UIGestureRecognizerStateEnded)
        {
            prevAngleRad = 0.0;
        }
    }
}


-(BOOL) LoadCadFile
{
    BOOL rtn = false;
    try
    {
        // Load file
        pDb = iOdaAppServices::getInstance()->readFile(NSString2OdString(cadFileName));
        if (pDb.isNull())
        {
            // load database failed
        }
        else
        {
            // Create the rendering context.
            
            OdGiContextForDgDatabasePtr pDgnContext = OdGiContextForDgDatabase::createObject();
            

            // Create the rendering context.
//            OdGiContextForDbDatabasePtr pDwgContext = OdGiContextForDbDatabase::createObject();
            // Create the custom rendering device and set the output stream for the device.
            if (!gsDevice.isNull())
            {
                gsDevice.release();
            }
            if (gsDevice.isNull())
            {
                // Load graphics device module
                OdGsModulePtr m = ::odrxDynamicLinker()->loadModule(OdWinGLES2ModuleName);

                // Create graphics device
                gsDevice = m->createDevice();
                // Prepare callbacks for WinOpenGL vectorizer
               // m_protoCallback.m_pDelegate = self;
                // Setup graphics device properties
                OdRxDictionaryPtr pProps = gsDevice->properties();
                if (!pProps.isNull())
                {
                   //TODO
                }
                else
                {
                    NSLog(@"\nCan't access device properties");
                }
            }
            // Set the database to be vectorized.
            pDgnContext->setDatabase(pDb);
            pDgnContext->enableGsModel(true);
            // Prepare the device to render the active layout in this database.
            //gsDevice = OdDbGsManager::setupActiveLayoutViews(gsDevice, pDgnContext);
            
            OdDbBaseDatabasePEPtr pDbPE = pDb ;
            gsDevice = pDbPE->setupActiveLayoutViews( gsDevice, pDgnContext);
            
            // Setup device palette
            //if ( 1 ) //OdGsPaperLayoutHelper::cast(gsDevice).isNull())
            {
                // ModelSpace
                gsDevice->setLogicalPalette(::odcmAcadDarkPalette(), 256);
                gsDevice->setBackgroundColor(ODRGB(0, 0, 0));
                pDgnContext->setPaletteBackground(ODRGB(0, 0, 0));
            }
            /*else
            {
                gsDevice->setLogicalPalette(::odcmAcadLightPalette(), 256);
                gsDevice->setBackgroundColor(ODRGB(173, 174, 173));
                pDgnContext->setPaletteBackground(ODRGB(255, 255, 255));
            }*/
            // Setup rendering surface size
            [self resizeGsDevice:self.view.frame];
            OdAbstractViewPEPtr(gsDevice->viewAt(0))->zoomExtents(gsDevice->viewAt(0));

            // Layer must be set for WinGLES2 device
            gsDevice->properties()->putAt(OD_T("EAGLContext"), OdRxVariantValue((OdIntPtr)self.glContext));
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

