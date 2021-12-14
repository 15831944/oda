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
#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES1/gl.h>
#import <QuartzCore/CAEAGLLayer.h>

#include <Gs/Gs.h>
#include <DbDatabase.h>
#include "../Extensions/ExRender/WinOpenGL/iOS/EAGLProto.h"

@class ODBaseView;

class EAGLProtoRedirector : public OdEAGLProtocol
{
public:
  ODBaseView *m_pDelegate;

  void eaglProtoFromDrawable();
  void eaglProtoPresentRenderBuffer();
};

@interface ODBaseView : UIView {
  EAGLContext* glContext;
	
  GLint backingWidth;
  GLint backingHeight;
	
  NSString* strCadFile;
	
  OdGsDevicePtr gsDevice;
  OdDbDatabasePtr pDb;
	
  EAGLProtoRedirector m_protoCallback;
}

+(Class) layerClass;

@property (assign) EAGLContext* glContext;
@property (assign) NSString* strCadFile;
@property (assign) OdGsDevicePtr gsDevice;
@property (assign) OdDbDatabasePtr pDb;

-(void) awakeFromNib;

-(void) draw:(BOOL) resize;
-(BOOL) resizeFromLayer:(CAEAGLLayer*)layer;
-(void) resizeGsDevice:(OdGsDevice*)pDevice;

-(BOOL) InitOpenGLES;
-(void) UninitOpenGLES;

-(BOOL) LoadCadFile;

// Delegate is required to correctly attach layer drawable to render buffer
-(void) protoDelegateFromDrawable;
-(void) protoDelegatePresentBuffer;
//

@end
