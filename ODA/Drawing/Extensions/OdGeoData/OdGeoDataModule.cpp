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

#include "OdGeoDataModule.h"
#include "RxDynamicModule.h"
#include "cs_map.h"

#include "OdDbGeoCoordinateSystemImpl.h"

ODRX_DEFINE_DYNAMIC_MODULE(OdGeoDataModule);

OdGeoDataModule::OdGeoDataModule()
{
}

OdGeoDataModule::~OdGeoDataModule()
{
}

void OdGeoDataModule::initApp()
{
  {
    //csmap library init:
    OdString sEnv;
    odrxSystemServices()->getEnvVar(L"MENTOR_DICTIONARY_PATH", sEnv);
    int st = -1;
    if (!sEnv.isEmpty())
    {
      st = CS_altdr(sEnv);
    }
    if (st != 0)
    {
      st = CS_altdr(NULL); //use CS_MAP_DIR
    }

    //hack for windows: CSDicts directory near .dll
#if defined(ODA_WINDOWS) && !defined(_WINRT)
    if (st != 0)
    {
      OdString sCSDicts;
      TCHAR path[MAX_PATH];
      ::GetModuleFileName(::GetModuleHandle(0), path, MAX_PATH);
      OdString sPath(path);
      int nLen = sPath.reverseFind('\\');
      sPath = sPath.left(nLen);
      sCSDicts = sPath + "\\CSDicts";
      st = CS_altdr(sCSDicts);
    }
#endif

#if 1
    //throw Exception if csmap initialization failed:
    if (st != 0)
    {
      throw OdError(eNotInitializedYet);
    }
#endif

    //user directory init (optional, can be empty):
    OdString sEnvUserDir;
    odrxSystemServices()->getEnvVar(L"MENTOR_USER_DICTIONARY_PATH", sEnvUserDir);
    if (!sEnvUserDir.isEmpty())
    {
      CS_usrdr(sEnvUserDir);
    }
    else
    {
      OdString sCSUserDicts;
      //hack for windows: CSUserDicts directory near .dll
#if defined(ODA_WINDOWS) && !defined(_WINRT)
      TCHAR path[MAX_PATH];
      ::GetModuleFileName(::GetModuleHandle(0), path, MAX_PATH);
      OdString sPath(path);
      int nLen = sPath.reverseFind('\\');
      sPath = sPath.left(nLen);
      sCSUserDicts = sPath + "\\CSUserDicts";
#endif
      CS_usrdr(sCSUserDicts);
    }
  }

  OdDbGeoCoordinateSystemCategoryImpl::rxInit();
  OdDbGeoCoordinateSystemImpl::rxInit();
  OdDbGeoCoordinateSystemTransformerImpl::rxInit();

  OdDbGeoDataPEImpl::rxInit();
  OdDbGeoData::desc()->addX(OdDbGeoDataPE::desc(), &m_pGeoDataPE);
  
  OdDbGeoCoordinateSystemCategoryPEImpl::rxInit();
  OdDbGeoCoordinateSystemCategory::desc()->addX(OdDbGeoCoordinateSystemCategoryPE::desc(), &m_pGeoCoordinateSystemCategoryPE);

  OdDbGeoCoordinateSystemPEImpl::rxInit();
  OdDbGeoCoordinateSystem::desc()->addX(OdDbGeoCoordinateSystemPE::desc(), &m_pGeoCoordinateSystemPE);

  OdDbGeoCoordinateSystemTransformerPEImpl::rxInit();
  OdDbGeoCoordinateSystemTransformer::desc()->addX(OdDbGeoCoordinateSystemTransformerPE::desc(), &m_pGeoCoordinateSystemTransformerPE);
}

void OdGeoDataModule::uninitApp()
{
  //csmap library uninit:
  CS_recvr();

  OdDbGeoCoordinateSystemTransformer::desc()->delX(OdDbGeoCoordinateSystemTransformerPE::desc());
  OdDbGeoCoordinateSystemTransformerPEImpl::rxUninit();

  OdDbGeoCoordinateSystem::desc()->delX(OdDbGeoCoordinateSystemPE::desc());
  OdDbGeoCoordinateSystemPEImpl::rxUninit();

  OdDbGeoCoordinateSystemCategory::desc()->delX(OdDbGeoCoordinateSystemCategoryPE::desc());
  OdDbGeoCoordinateSystemCategoryPEImpl::rxUninit();
  
  OdDbGeoData::desc()->delX(OdDbGeoDataPE::desc());
  OdDbGeoDataPEImpl::rxUninit();

  OdDbGeoCoordinateSystemTransformerImpl::rxUninit();
  OdDbGeoCoordinateSystemImpl::rxUninit();
  OdDbGeoCoordinateSystemCategoryImpl::rxUninit();
}
