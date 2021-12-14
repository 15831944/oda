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
// TrXml renderer module

#include "OdaCommon.h"
#include "RxDynamicModule.h"
#include "TrVisRenderClient.h"
#include "TrXmlRendition.h"
#include "TrRndRenderModule.h"

class OdTrXmlRenderModule : public OdTrRndRenderModuleStub
{
  public:
    // Create new rendition
    virtual OdTrVisRenditionPtr createRendition(OdTrRndLocalRenditionClient *pRenderClient);

    virtual void initApp() {}
    virtual void uninitApp() {}
};

ODRX_DEFINE_DYNAMIC_MODULE(OdTrXmlRenderModule);

OdTrVisRenditionPtr OdTrXmlRenderModule::createRendition(OdTrRndLocalRenditionClient *pRenderClient)
{
  OdSmartPtr<OdTrXmlRenditionRedir> pXmlRendition = OdRxObjectImpl<OdTrXmlRenditionRedir>::createObject();
  pXmlRendition->setupClientContext(pRenderClient, this);
  return pXmlRendition;
}

#if defined(_TOOLKIT_IN_DLL_) && defined(_MSC_VER)

extern "C" int APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID )
{
  switch ( dwReason )
  {
  case DLL_PROCESS_ATTACH:
    // remove this if you need per-thread initialization
    DisableThreadLibraryCalls( (HMODULE)hInstance );
    break;
  }
  return TRUE;
}

#endif //_TOOLKIT_IN_DLL_

//
