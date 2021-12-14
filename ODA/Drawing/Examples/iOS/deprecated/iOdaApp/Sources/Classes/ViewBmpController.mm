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

#import "ViewBmpController.h"
#import "iOdaAppAppDelegate.h"
#import "iUtils.h"
#import "iStrConv.h"

#include "iOdaAppServices.h"
#include <DbDumper.h>

#include <diagnostics.h>
#include <Gs/Gs.h>
#include <DbDatabase.h>
#include <GiContextForDbDatabase.h>
#include <DbGsManager.h>
#include <Gs/GsBaseVectorizer.h>
#include <RxVariantValue.h>
#include <RxDictionary.h>
#include <ColorMapping.h>
#include <AbstractViewPE.h>
#include "Gi/GiRasterImage.h"
#include "Gs/Gs.h"
#include "Gs/GsBaseInclude.h"
#include "ColorMapping.h"
#include "RxRasterServices.h"

ViewBmpController* gViewBmpController = NULL;

@implementation ViewBmpController

@synthesize backButton;
@synthesize fileName;
@synthesize bmpFile;
@synthesize imageView;

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization.
		gViewBmpController = self;
    }
    return self;
}


- (void)viewDidLoad
{
    [super viewDidLoad];
  //  [self drawImage];

}

- (void)viewDidUnload
{
   // self.imageView = nil;
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

- (void)dealloc
{
 //   [imageView release];
    [super dealloc];
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}

-(IBAction) onBackButton:(id) sender {
	iOdaAppAppDelegate *appDelegate =
	(iOdaAppAppDelegate*)[[UIApplication sharedApplication] delegate];
	[[self view] removeFromSuperview];
    [imageView setImage:Nil];
	[appDelegate switchToDefaultView];
}

- (void)viewDidAppear:(BOOL)animated
{
	[self drawImage];
}

- (void) drawImage
{
        try {      
            NSString* fullFilePath = [iUtils sharedDocumentFullPath:self.fileName];
			
            OdDbDatabasePtr db = iOdaAppServices::getInstance()->readFile(NSString2OdString(fullFilePath), true, false);
            
            if (!db.isNull())
            {
                // Create the rendering context.
                OdGiContextForDbDatabasePtr pDwgContext = OdGiContextForDbDatabase::createObject();
                OdGsDevicePtr gsDevice;
                // Create the custom rendering device and set the output stream for the device.
                if ( !gsDevice.isNull() )
                {
                    gsDevice.release();
                }
                
                if ( gsDevice.isNull() )
                {
                    OdGsModulePtr m = ::odrxDynamicLinker()->loadModule(OdWinBitmapModuleName);
                    gsDevice = m->createBitmapDevice();
                }
                
                // Set the database to be vectorized.
                pDwgContext->setDatabase(db);
                
                // Prepare the device to render the active layout in this database.
                gsDevice = OdDbGsManager::setupActiveLayoutViews(gsDevice, pDwgContext);
                
                // Setup device palette
                if (db->getTILEMODE())//OdGsPaperLayoutHelper::cast(gsDevice).isNull())
                {
                    // ModelSpace
                    gsDevice->setLogicalPalette(::odcmAcadDarkPalette(), 256);
                    gsDevice->setBackgroundColor(ODRGB(0, 0, 0));
                    pDwgContext->setPaletteBackground(ODRGB(0, 0, 0));
                }
                else
                {
                    gsDevice->setLogicalPalette(::odcmAcadLightPalette(), 256);
                    gsDevice->setBackgroundColor(ODRGB(168, 168, 168));
                    pDwgContext->setPaletteBackground(ODRGB(255, 255, 255));
                }
                // Resize
                gsDevice->properties()->putAt(OD_T("BitPerPixel"), OdRxVariantValue(OdUInt32(24)));
                // Set the screen size for the generated geometry.
                int backingHeight = 600,  backingWidth = 740;
                OdGsDCRect screenRect( OdGsDCPoint(0, backingHeight), OdGsDCPoint(backingWidth, 0) );
                gsDevice->onSize(screenRect);
                
                // Zoom to extents
                OdAbstractViewPEPtr(gsDevice->viewAt(0))->zoomExtents(gsDevice->viewAt(0));
                gsDevice->update();
                
                OdGiRasterImagePtr ptrImage = gsDevice->properties()->getAt( OD_T("RasterImage") );
                
                OdRxRasterServicesPtr pRasSvcs = odrxDynamicLinker()->loadApp(RX_RASTER_SERVICES_APPNAME);
                if (!pRasSvcs.isNull())
                {
                    NSString *ext = @".bmp";
                    bmpFile = [fullFilePath stringByAppendingString:ext];

                    pRasSvcs->saveRasterImage(OdGiRasterImagePtr(gsDevice->properties()->getAt(OD_T("RasterImage"))), NSString2OdString(bmpFile));
                    
                    UIImage* image = [UIImage imageWithContentsOfFile:[NSString stringWithFormat:@"%@", bmpFile]];
                    [imageView setImage:image];
                }
                
            }
            
        } catch (...) {
          //  [self msg:[NSString stringWithFormat:@"Unknown error"]];
        }
}

@end
