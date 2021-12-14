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
//
//  DwgViewController.h
//  iOdaApp
//
//  Created by Irina Bezruk on 03/11/15.
//  Copyright © 2015 ODA. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>

#include <OdaCommon.h>
#include <Gs/Gs.h>
#include <DbDatabase.h>
#include <GiContextForDbDatabase.h>
#include <DbGsManager.h>

@interface DwgViewController : GLKViewController
{
    GLint backingWidth;
    GLint backingHeight;
    CGFloat prevAngleRad;
    CGPoint prevPanPoint;
    CGFloat prevScale;
    CGPoint viewStartLocation;
    
    OdGsDevicePtr gsDevice;
    OdDbDatabasePtr pDb;
}

@property (nonatomic, strong) NSString *cadFileName;
@property (strong, nonatomic) EAGLContext *glContext;

- (void) setupGL;
- (IBAction)handlePan:(UIPanGestureRecognizer *)recognizer;
- (IBAction)handlePinch:(UIPinchGestureRecognizer *)recognizer;
- (IBAction)handleRotate:(UIRotationGestureRecognizer *)recognizer;

- (OdGePoint3d)toWCS: (CGFloat)x andY:(CGFloat) y;
- (bool)viewCanRotate;

@end
