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
// Animation host abtraction layer

#ifndef OD_XML_GLES2VIEW_ANIMATIONCLIENT_H_
#define OD_XML_GLES2VIEW_ANIMATIONCLIENT_H_

#include "TD_PackPush.h"

class AnimationHost;

/** <group ExRender_Classes>
*/
class AnimationClient
{
  public:
    AnimationClient() { }
    virtual ~AnimationClient() { }

    virtual bool process(AnimationHost * /*pAnimHost*/) { return false; }
    virtual void finalize(AnimationHost * /*pAnimHost*/) { }

    virtual void play(AnimationHost * /*pAnimHost*/) { }
    virtual void pause(AnimationHost * /*pAnimHost*/) { }
    virtual void stop(AnimationHost * /*pAnimHost*/) { }
    virtual void forward(AnimationHost * /*pAnimHost*/) { }
};

/** <group ExRender_Classes>
*/
class AnimationHost
{
  public:
    AnimationHost() { }
    virtual ~AnimationHost() { }

    virtual void runAnimationLoop(AnimationClient *pClient) = 0;

    virtual void closeGate(OdUInt32 nInterval) = 0;
    virtual void openGate(bool bForce = false) = 0;

    virtual void updateStatus(const char *pStatus) = 0;

    virtual void processThreadCallback(AnimationClient *pCallback) = 0;
};

#include "TD_PackPop.h"

#endif // OD_XML_GLES2VIEW_ANIMATIONCLIENT_H_
