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

#import "iUtils.h"

@implementation iUtils

+(void) showMessage: (NSString*) message {
    /*	UIAlertView *alert = [[UIAlertView alloc]
						   initWithTitle:@"Message" 
						   message:message 
						   delegate:nil 
						   cancelButtonTitle:@"OK"
						   otherButtonTitles:nil];
	[alert show];
	[alert release];
     */
}	

+(NSString*) sharedDocumentsPath {
	NSArray* paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
	NSString* sharedDocumentsDirectory = [paths objectAtIndex:0];
	return sharedDocumentsDirectory;
}

+(NSString*) strCurrentTimestamp {
	NSDate* now = [NSDate date];
	NSDateFormatter* dateFormat = [[NSDateFormatter alloc] init];
	[dateFormat setDateFormat:@"YY-MMM-dd_HH-mm-SS"];
	NSString* resStr = [dateFormat stringFromDate:now];
	return resStr;
}

+(NSString*) sharedDocumentFullPath:(NSString*)fileName
{
	NSString* fullFilePath = [NSString stringWithFormat:@"%@/%@", 
							  [iUtils sharedDocumentsPath],
							  fileName];
	return fullFilePath;
}

@end
