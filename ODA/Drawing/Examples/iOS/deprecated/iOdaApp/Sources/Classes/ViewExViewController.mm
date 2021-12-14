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

#import "iOdaAppAppDelegate.h"
#import "ViewExViewController.h"
#import "ODBaseView.h"

@implementation ViewExViewController

-(void) setFileName:(NSString*)fName 
{
	[fName retain];
	[fileName release];
	fileName = fName;
	
	[(ODBaseView*)self.view setStrCadFile:self.fileName];
}

-(NSString*) fileName {
	return fileName;
}


// The designated initializer.  Override if you create the controller programmatically and want to perform customization that is not appropriate for viewDidLoad.
/*
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization.
    }
    return self;
}
*/

/*
// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
}
*/

/*
// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Return YES for supported orientations.
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}
*/

- (void)didReceiveMemoryWarning {
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc. that aren't in use.
}

- (void)viewDidUnload {
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

-(IBAction)onBack:(id)sender
{
    iOdaAppAppDelegate *appDelegate =
    (iOdaAppAppDelegate*)[[UIApplication sharedApplication] delegate];
    [[self view] removeFromSuperview];
    self.view = nil ;
    [appDelegate switchToDefaultView];
}

- (void)dealloc {
    [super dealloc];
}

// Screen rotation support for this view controller
// for 5.1 and older
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation
{
  return YES;
}
// for 6.0 and later
- (BOOL)shouldAutorotate
{
  return YES;
}
- (UIInterfaceOrientation)prefferedInterfaceOrientationForPresentation
{
  return UIInterfaceOrientationPortrait;
}
- (NSUInteger)supportedInterfaceOrientations
{
  typedef enum { // For compile in 5.1
    _UIInterfaceOrientationMaskPortrait = (1 << UIInterfaceOrientationPortrait),
    _UIInterfaceOrientationMaskLandscapeLeft = (1 << UIInterfaceOrientationLandscapeLeft),
    _UIInterfaceOrientationMaskLandscapeRight = (1 << UIInterfaceOrientationLandscapeRight),
    _UIInterfaceOrientationMaskPortraitUpsideDown = (1 << UIInterfaceOrientationPortraitUpsideDown),
    _UIInterfaceOrientationMaskLandscape =
    (_UIInterfaceOrientationMaskLandscapeLeft | _UIInterfaceOrientationMaskLandscapeRight),
    _UIInterfaceOrientationMaskAll =
    (_UIInterfaceOrientationMaskPortrait | _UIInterfaceOrientationMaskLandscapeLeft |
     _UIInterfaceOrientationMaskLandscapeRight | _UIInterfaceOrientationMaskPortraitUpsideDown),
    _UIInterfaceOrientationMaskAllButUpsideDown =
    (_UIInterfaceOrientationMaskPortrait | _UIInterfaceOrientationMaskLandscapeLeft |
     _UIInterfaceOrientationMaskLandscapeRight),
  } _UIInterfaceOrientationMask;
  return _UIInterfaceOrientationMaskAll;
}

@end
