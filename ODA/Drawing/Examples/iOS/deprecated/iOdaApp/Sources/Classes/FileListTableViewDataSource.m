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

#import "FileListTableViewDataSource.h"
#import "iOdaAppAppDelegate.h"
#import "iUtils.h"

@implementation FileListTableViewDataSource

@synthesize directoryContent;
@synthesize isValid;
@synthesize selectedFile;

- (FileListTableViewDataSource*) init {
	FileListTableViewDataSource* r = [super init];
	if (r!=NULL) {
		r.isValid = FALSE;
		selectedFile = @"";
	}
	return r;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
	
		[self listFiles];
	
	NSInteger index = indexPath.row;
	
	NSString* cellID = [self.directoryContent objectAtIndex:index];
	
	UITableViewCell* cell = [tableView dequeueReusableCellWithIdentifier:cellID];
	
	if (cell == NULL) {
		cell = [[UITableViewCell alloc] 
				 initWithStyle:UITableViewCellStyleSubtitle 
				 reuseIdentifier:cellID];
		cell.selectionStyle = UITableViewCellSelectionStyleBlue;
	}
	cell.textLabel.text = cellID;
	return cell;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
	[self listFiles];
	NSInteger r = [self.directoryContent count];
	return r;
}

- (void) listFiles {
		
	NSString* sharedDocumentsDirectory = [iUtils sharedDocumentsPath];
	
	NSError* err;
	
	self.directoryContent = [[NSFileManager defaultManager] 
									   contentsOfDirectoryAtPath:sharedDocumentsDirectory 
									   error:err];	
	self.isValid = TRUE;
}	


- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
	[self listFiles];
	NSInteger index = indexPath.row;
	NSString* fileName = [self.directoryContent objectAtIndex:index];
	self.selectedFile = fileName;
	
	iOdaAppAppDelegate *appDelegate = 
		(iOdaAppAppDelegate*)[[UIApplication sharedApplication] delegate];
	[appDelegate fileSelected:fileName];
	
}

@end
