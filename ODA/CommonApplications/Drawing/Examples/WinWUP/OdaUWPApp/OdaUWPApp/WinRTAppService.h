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

#ifndef __WINRT_APP_SERVICE_H__
#define __WINRT_APP_SERVICE_H__
#include <OdaCommon.h>
#include <DbDatabase.h>
#include <DbSystemServices.h>
#include <ExHostAppServices.h>
#include <ExSystemServices.h>

class WinRTAppServices;

static WinRTAppServices* instance = NULL;
static OdMutex WinRTAppServices_classMutex;

class WinRTAppServices : public ExSystemServices, public ExHostAppServices {
    
public:
    static WinRTAppServices* getInstance()
    {
        if (instance == NULL)
        {
            instance = new WinRTAppServices();
        }
        return instance;
    }

    ~WinRTAppServices() {}

    WinRTAppServices() : mCnt (0){}

    void addRef() {
        TD_AUTOLOCK(WinRTAppServices_classMutex);
        mCnt++;
    }

    void release() {
        TD_AUTOLOCK(WinRTAppServices_classMutex);
        mCnt--;
        if (mCnt == 0) { delete this; instance = NULL; }
    }
    
protected:
    
private:
    int mCnt;
};

#endif
