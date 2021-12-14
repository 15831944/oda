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

#include "StdAfx.h"
#include "ExFieldEvaluatorModule.h"
#include "RxDynamicModule.h"
#include "DynamicLinker.h"
#include "FdField.h"

#if defined(_TOOLKIT_IN_DLL_) && defined(_MSC_VER) && (_MSC_VER > 1310) && !defined(_WINRT)


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
#include "OdaObjProp.h"
#include "OdExprEval.h"

//DD:EXPORT_ON
ODRX_DEFINE_DYNAMIC_MODULE(ExFieldEvaluatorModule);
//DD:EXPORT_OFF

ExFieldEvaluatorModule::ExFieldEvaluatorModule()
{
}

ExFieldEvaluatorModule::~ExFieldEvaluatorModule()
{
}

void ExFieldEvaluatorModule::initApp()
{
  OdEvaluatorBase::rxInit();
  OdVarEvaluator::rxInit();
  OdTextEvaluator::rxInit();
  OdDieselEvaluator::rxInit();
  OdExEvaluatorLoader::rxInit();
  OdSheetSetEvaluator::rxInit();
  OdObjPropEvaluator::rxInit();
  OdExprEvaluator::rxInit();
  m_pEvaluatorLoader = OdExEvaluatorLoader::createObject();
  oddbGetFieldEngine()->registerEvaluatorLoader(m_pEvaluatorLoader);
  m_pDynBlocks = ::odrxDynamicLinker()->loadModule(OdDynBlocksModuleName);
}

void ExFieldEvaluatorModule::uninitApp()
{
  m_pDynBlocks = 0;
  oddbGetFieldEngine()->unregisterEvaluatorLoader(m_pEvaluatorLoader);
  m_pEvaluatorLoader = 0;
  OdExEvaluatorLoader::rxUninit();
  OdDieselEvaluator::rxUninit();
  OdVarEvaluator::rxUninit();
  OdTextEvaluator::rxUninit();
  OdEvaluatorBase::rxUninit();
  OdSheetSetEvaluator::rxUninit();
  OdObjPropEvaluator::rxUninit();
  OdExprEvaluator::rxUninit();
}
