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

#include "OdaCommon.h"

#include "DbSystemServices.h"
#include "DbHostAppServices.h"

#include "GeoMapPEModule.h"
#include "RxDynamicModule.h"

#include "DbGeoMapPEImpl.h"
#include "DbGeoMap.h"
#include "OdBingMapsCache.h"

ODRX_DEFINE_DYNAMIC_MODULE(OdDbGeoMapPEModule);

OdDbGeoMapPEModule::OdDbGeoMapPEModule()
{
}

OdDbGeoMapPEModule::~OdDbGeoMapPEModule()
{
}

void OdDbGeoMapPEModule::initApp()  
{
  m_pOdGeolocationObjModule = odrxDynamicLinker()->loadModule(OdGeolocationObjModuleName, false);
  m_pOdGeoDataModule = odrxDynamicLinker()->loadModule(OdGeoDataModuleName, false);

  OdDbGeoMapPEImpl::rxInit();
  OdDbGeoMap::desc()->addX(OdDbGeoMapPE::desc(), &m_pGeoMapPE);

  OdDbGeoDataChunksPEImpl::rxInit();
  OdDbGeoData::desc()->addX(OdDbGeoDataChunksPE::desc(), &m_pGeoDataChunksPE);

  g_OdBingMapsCache = new OdBingMapsCache;
}

void OdDbGeoMapPEModule::uninitApp()
{
  if (NULL != g_OdBingMapsCache)
  {
    delete g_OdBingMapsCache;
  }

  OdDbGeoData::desc()->delX(OdDbGeoDataChunksPE::desc());
  OdDbGeoDataChunksPEImpl::rxUninit();

  OdDbGeoMap::desc()->delX(OdDbGeoMapPE::desc());
  OdDbGeoMapPEImpl::rxUninit();
}
