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

/************************************************************************/
/* Defines entry points for the DLL                                     */
/************************************************************************/
//#include "stdafx.h"
#include "OdaCommon.h"
#include "GeolocationObjModule.h"
#include "RxDynamicModule.h"
#include "DbGeoMap.h"

#include "DbGeoMapPE.h"
#include "DbGeoMapFieldsPE.h"

#if defined(_TOOLKIT_IN_DLL_) && defined(_MSC_VER) //  MacOS X dynamic library loading. //#ifdef _TOOLKIT_IN_DLL_

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

#endif //_TOOLKIT_IN_DLL_ //  MacOS X dynamic library loading.

ODRX_DEFINE_DYNAMIC_MODULE(GeolocationObjModule);

GeolocationObjModule::GeolocationObjModule()
{
}

GeolocationObjModule::~GeolocationObjModule()
{
}

void GeolocationObjModule::initApp()
{
  if ( !OdDbRasterImage::desc() )
  {
    throw OdError(eNotInitializedYet);
  }
  // These classes need to be registered before loading GeoMapPEModule
  // Its loading should not throw exception
  OdDbGeoMap::rxInit();
  OdDbGeoMapDef::rxInit();
  OdDbGeoMapPE::rxInit();
  OdDbGeoMapFieldsPE::rxInit();
  OdDbGeoMap::desc()->addX(OdDbGeoMapFieldsPE::desc(), &m_pGeoMapFiledsPE);
  OdDbGeoMap::desc()->addX(OdDbGeoMapGripPointsPE::desc(), &m_grippoints);
}

void GeolocationObjModule::uninitApp()
{
  OdDbGeoMap::desc()->delX(OdDbGeoMapGripPointsPE::desc());
  OdDbGeoMap::desc()->delX(OdDbGeoMapFieldsPE::desc());
  OdDbGeoMapFieldsPE::rxUninit();
  OdDbGeoMapPE::rxUninit();
  OdDbGeoMapDef::rxUninit();
  OdDbGeoMap::rxUninit();
}
