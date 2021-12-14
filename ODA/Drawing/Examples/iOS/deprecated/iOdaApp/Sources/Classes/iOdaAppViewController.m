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
#import "iUtils.h"

@implementation iOdaAppViewController

@synthesize newButton;
@synthesize readExButton;
@synthesize viewExButton;
@synthesize viewBmpExButton;
@synthesize fileListTableView;
@synthesize fileListDataSource;


// The designated initializer. Override to perform setup that is required before the view is loaded.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

/*
// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView {
}
*/



// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];

	self.fileListDataSource = [[FileListTableViewDataSource alloc] init];
	((UITableView*)(self.fileListTableView)).dataSource = self.fileListDataSource;
	((UITableView*)(self.fileListTableView)).delegate = self.fileListDataSource;

}



/*
// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Return YES for supported orientations
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}
*/

- (void)didReceiveMemoryWarning {
	// Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
	
	// Release any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload {
	// Release any retained subviews of the main view.
	// e.g. self.myOutlet = nil;
}

- (void)dealloc {
    [super dealloc];
}

- (IBAction) newFileAction: (id)sender {
	NSString* filePath = [NSString stringWithFormat:@"%@/%@.dwg", 
						  [iUtils sharedDocumentsPath],
						  [iUtils strCurrentTimestamp]];
	
	[[iOdaApplication getInstance] odWriteEx: filePath];
	[iUtils showMessage:@"WriteEx finished"];
	[self.fileListTableView	reloadData];
}
	
- (IBAction) readExAction: (id)sender {	
	iOdaAppAppDelegate *appDelegate = 
		(iOdaAppAppDelegate*)[[UIApplication sharedApplication] delegate];
	[[self view] removeFromSuperview];
	[appDelegate switchToReadExView];

}

- (IBAction) viewExAction: (id)sender {
	iOdaAppAppDelegate *appDelegate = 
	(iOdaAppAppDelegate*)[[UIApplication sharedApplication] delegate];
	[[self view] removeFromSuperview];
	[appDelegate switchToViewExView];
}

- (IBAction) viewBmpExAction: (id)sender {
	iOdaAppAppDelegate *appDelegate =
    (iOdaAppAppDelegate*)[[UIApplication sharedApplication] delegate];
	[[self view] removeFromSuperview];
	[appDelegate switchToViewBmpExView];
    
}
@end
