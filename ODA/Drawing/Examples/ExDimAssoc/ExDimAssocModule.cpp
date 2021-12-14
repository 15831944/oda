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
#include "RxDynamicModule.h"
#include "ExDimAssocModule.h"
#include "DbEvalWatchers.h"
#include "DbDimAssoc.h"
#include "DbEntityIntersectionPE.h"
#include "DbCurve.h"
#include "ExDimAssoc.h"
#include "DbGripPoints.h"

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

#endif

ODRX_DEFINE_DYNAMIC_MODULE(ExDimAssocModule);

ExDimAssocModule::ExDimAssocModule()
{
}

ExDimAssocModule::~ExDimAssocModule()
{
}

void ExDimAssocModule::initApp()
{
  if ( !OdDbDimAssoc::desc() )
  {
    throw OdError(eNotInitializedYet);
  }
  OdDbDimAssoc::desc()->module()->addRef();

  OdDbDimAssoc::desc()->addX( OdDbEvalWatcherPE::desc(), &_mDA );

  OdDbEntityIntersectionPEPtr pEntInt = OdDbCurve::desc()->getX(OdDbEntityIntersectionPE::desc());
  if (pEntInt.isNull())
  {
    m_pEntityIntersection = ::odrxDynamicLinker()->loadModule(L"ExEntityIntersection");
  }

  OdDbGripPointsPEPtr pGrips = OdDbEntity::desc()->getX(OdDbGripPointsPE::desc());
  if (pGrips.isNull())
  {
    m_pGripPoints = ::odrxDynamicLinker()->loadModule(OdGripPointsModuleName);
  }
  OdExDimAssoc::rxInit();
}

void ExDimAssocModule::uninitApp()
{
  OdExDimAssoc::rxUninit();
  m_pGripPoints = NULL;
  m_pEntityIntersection = NULL;
  OdDbDimAssoc::desc()->delX( OdDbEvalWatcherPE::desc() );
  OdDbDimAssoc::desc()->module()->release();
}
