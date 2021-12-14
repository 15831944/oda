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

#import "ReadExViewController.h"
#import "iOdaAppAppDelegate.h"
#import "iUtils.h"
#import "iStrConv.h"

#include "iOdaAppServices.h"
#include <DbDumper.h>

#include <diagnostics.h>

ReadExViewController* gReadExViewController = NULL;

static void iReadExViewTrace(const OdChar* lineToTrace) {
	printf("%ls", lineToTrace);
}	 

@implementation ReadExViewController

@synthesize backButton;
@synthesize textView;
@synthesize fileName;
@synthesize isRunning;

// The designated initializer.  Override if you create the controller programmatically and want to perform customization that is not appropriate for viewDidLoad.

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization.
		[self.textView setFont: [UIFont systemFontOfSize:12]];
		gReadExViewController = self;
		odSetTraceFunc(&iReadExViewTrace);
    }
    return self;
}



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


- (void)dealloc {
    [super dealloc];
}

-(IBAction) onBackButton:(id) sender {
	iOdaAppAppDelegate *appDelegate = 
	(iOdaAppAppDelegate*)[[UIApplication sharedApplication] delegate];
	[[self view] removeFromSuperview];
	[appDelegate switchToDefaultView];
}

- (void)viewDidAppear:(BOOL)animated
{
	[self startReadEx];
}

-(void) startReadEx {
	[self msg:@"Starting thread..."];
	dispatch_queue_t aQueue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, NULL);
	dispatch_async(aQueue, ^{[self executeReadEx];});
	[self msg:@"Thread started"];
}

-(void) executeReadEx {
	try {
		
		self.isRunning = YES;
		[self resetTextOutput];
	
		NSString* fullFilePath = [iUtils sharedDocumentFullPath:self.fileName];
			
		[self msg:[NSString stringWithFormat:@"ReadEx: %@", fullFilePath]];
	
		OdDbDatabasePtr db = iOdaAppServices::getInstance()->readFile(NSString2OdString(fullFilePath), true, false);
	
		ExProtocolExtension pext;
		pext.initialize();
	
		DbDumper dumper;
		dumper.dump(db);
		
	} catch (OdError& e) {
		[self msg:[NSString stringWithFormat:@"OdError: %s", e.description().c_str()]];
	} catch (...) {
		[self msg:[NSString stringWithFormat:@"Unknown error"]];
	}
	
	[self msg:@"Finished"];
	self.isRunning = NO;
}
	
-(void) resetTextOutput {
	dispatch_async(dispatch_get_main_queue(), ^{
		((UITextView*)(self.textView)).text = @"";
	});
	
}

-(void) msg:(NSString*)line {
	NSString* strLine = [ NSString stringWithFormat:@"%@\n", line ]; 
	[self postMessageToMainThread:strLine];
}

-(void) postMessageToMainThread:(NSString*)strLine {
	dispatch_queue_t mainQueue = dispatch_get_main_queue();
	dispatch_async(mainQueue, ^ {
		NSString* aText = ((UITextView*)(self.textView)).text;
		((UITextView*)(self.textView)).text = [aText stringByAppendingString:strLine];		
	} );
}


@end
