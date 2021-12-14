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


#import "iOdaAppAppDelegate.h"
#import "iOdaAppViewController.h"
#import "iOdaApplication.h"
#import "ReadExViewController.h"
#import "ViewBmpController.h"

@implementation iOdaAppAppDelegate

@synthesize window;
@synthesize viewController;
@synthesize readexController;
@synthesize viewexController;
@synthesize viewBmpExController;

@synthesize selectedFile;

#pragma mark -
#pragma mark Application lifecycle

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {    
    
    // Override point for customization after application launch.
	self.readexController = [[ReadExViewController alloc] 
							 initWithNibName:@"ReadExViewController" 
							 bundle:[NSBundle mainBundle]];
	
	self.viewexController = [[ViewExViewController alloc]
							 initWithNibName:@"ViewExViewController" 
							 bundle:[NSBundle mainBundle]];
	
    self.viewBmpExController = [[ViewBmpController alloc]
                                initWithNibName:@"ViewBmpController"
                                bundle:[NSBundle mainBundle]];

    // Add the view controller's view to the window and display.
    //[self.window addSubview:viewController.view];
    //[self.window makeKeyAndVisible];
	
	[[iOdaApplication getInstance] appInitialize];
	
	[self switchToDefaultView];
	
    return YES;
}


- (void)applicationWillResignActive:(UIApplication *)application {
    /*
     Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
     Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
     */
}


- (void)applicationDidEnterBackground:(UIApplication *)application {
    /*
     Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later. 
     If your application supports background execution, called instead of applicationWillTerminate: when the user quits.
     */
}


- (void)applicationWillEnterForeground:(UIApplication *)application {
    /*
     Called as part of  transition from the background to the inactive state: here you can undo many of the changes made on entering the background.
     */
}


- (void)applicationDidBecomeActive:(UIApplication *)application {
    /*
     Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
     */
}


- (void)applicationWillTerminate:(UIApplication *)application {
    /*
     Called when the application is about to terminate.
     See also applicationDidEnterBackground:.
     */
}


#pragma mark -
#pragma mark Memory management

- (void)applicationDidReceiveMemoryWarning:(UIApplication *)application {
    /*
     Free up as much memory as possible by purging cached data objects that can be recreated (or reloaded from disk) later.
     */
}

- (void)dealloc {
    [viewController release];
	[readexController release];
    [viewBmpExController release];
    [window release];
    [super dealloc];
}

-(void)switchToReadExView {
	[(ReadExViewController*)readexController setFileName:selectedFile];
	[[readexController view] removeFromSuperview];
	[self.window addSubview:readexController.view];
	[self.window makeKeyAndVisible];
}

-(void)switchToDefaultView {
	
	if ([viewController view]) {
		[[viewController view] removeFromSuperview];
	}
	
	[self.window addSubview:viewController.view];
	[self.window makeKeyAndVisible];
}

-(void)switchToViewExView {
	[(ViewExViewController*)viewexController setFileName:selectedFile];
	[[viewexController view] removeFromSuperview];
	[self.window addSubview:viewexController.view];
	[self.window makeKeyAndVisible];
}

-(void)switchToViewBmpExView {
	[(ViewBmpController*)viewBmpExController setFileName:selectedFile];
	[[viewBmpExController view] removeFromSuperview];
	[self.window addSubview:viewBmpExController.view];
	[self.window makeKeyAndVisible];
}

-(void)fileSelected:(NSString*)fileName {
	self.selectedFile = fileName;
}
@end
