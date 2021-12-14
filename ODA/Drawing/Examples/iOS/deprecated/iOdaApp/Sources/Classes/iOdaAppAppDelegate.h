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

#import <UIKit/UIKit.h>
#import "ReadExViewController.h"
#import "ViewExViewController.h"
#import "ViewBmpController.h"

@class iOdaAppViewController;

@interface iOdaAppAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    iOdaAppViewController *viewController;
	ReadExViewController  *readexController;
	ViewExViewController  *viewexController;
	ViewBmpController     *viewBmpExController;
    NSString* selectedFile;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet iOdaAppViewController *viewController;
@property (nonatomic, retain) IBOutlet ReadExViewController *readexController;
@property (nonatomic, retain) IBOutlet ViewExViewController *viewexController;
@property (nonatomic, retain) IBOutlet ViewBmpController    *viewBmpExController;
@property (nonatomic, retain) NSString* selectedFile;

-(void)switchToReadExView;
-(void)switchToDefaultView;
-(void)switchToViewExView;
-(void)switchToViewBmpExView;
-(void)fileSelected:(NSString*)fileName;

@end

