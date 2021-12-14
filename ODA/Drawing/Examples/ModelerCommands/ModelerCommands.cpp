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
#include "ModelerCommands.h"
#include "RxSysRegistry.h"
#include "Ed/EdCommandContext.h"
#include "RxDynamicModule.h"
#include "DynamicLinker.h"
#include "ModelerGeometry/ModelerModule.h"

ODRX_DEFINE_DYNAMIC_MODULE(ModelerCommandsModule);

void ModelerCommandsModule::initApp()
{ 
  ::odedRegCmds()->addCommand(&m_pUnion);
  ::odedRegCmds()->addCommand(&m_pSubtract);
  ::odedRegCmds()->addCommand(&m_pIntersect);
  ::odedRegCmds()->addCommand(&m_pUnionEx);
  ::odedRegCmds()->addCommand(&m_pSubtractEx);
  ::odedRegCmds()->addCommand(&m_pIntersectEx);
  ::odedRegCmds()->addCommand(&m_pIntersectSurf);
  ::odedRegCmds()->addCommand(&m_pSculpt);
  ::odedRegCmds()->addCommand(&m_pImprint);
  ::odedRegCmds()->addCommand(&m_pSolidEdit);
  ::odedRegCmds()->addCommand(&m_pRegion);
  ::odedRegCmds()->addCommand(&m_pProperties);
  ::odedRegCmds()->addCommand(&m_pNewTessellation);
  ::odedRegCmds()->addCommand(&m_pAcisIn);
  ::odedRegCmds()->addCommand(&m_pBrLineContainmentCommand);
  ::odedRegCmds()->addCommand(&m_pBrSurfaceLineContainmentCommand);
  ::odedRegCmds()->addCommand(&m_pAuditBrep);
  ::odedRegCmds()->addCommand(&m_pCopyBrep);
  ::odedRegCmds()->addCommand(&m_pExSweep);
}

void ModelerCommandsModule::uninitApp()
{
  ::odedRegCmds()->removeCmd(m_pBrSurfaceLineContainmentCommand.groupName(), m_pBrSurfaceLineContainmentCommand.globalName());
  ::odedRegCmds()->removeCmd(m_pBrLineContainmentCommand.groupName(), m_pBrLineContainmentCommand.globalName());

  ::odedRegCmds()->removeCmd(m_pAcisIn.groupName()         , m_pAcisIn.globalName());
  ::odedRegCmds()->removeCmd(m_pProperties.groupName()     , m_pProperties.globalName());
  ::odedRegCmds()->removeCmd(m_pNewTessellation.groupName(), m_pNewTessellation.globalName());

  ::odedRegCmds()->removeCmd(m_pRegion.groupName()       , m_pRegion.globalName());
  ::odedRegCmds()->removeCmd(m_pSolidEdit.groupName()    , m_pSolidEdit.globalName());
  ::odedRegCmds()->removeCmd(m_pImprint.groupName()      , m_pImprint.globalName());
  ::odedRegCmds()->removeCmd(m_pSculpt.groupName()       , m_pSculpt.globalName());
  ::odedRegCmds()->removeCmd(m_pIntersectSurf.groupName(), m_pIntersectSurf.globalName());
  ::odedRegCmds()->removeCmd(m_pIntersectEx.groupName()  , m_pIntersectEx.globalName());
  ::odedRegCmds()->removeCmd(m_pSubtractEx.groupName()   , m_pSubtractEx.globalName());
  ::odedRegCmds()->removeCmd(m_pUnionEx.groupName()      , m_pUnionEx.globalName());
  ::odedRegCmds()->removeCmd(m_pIntersect.groupName()    , m_pIntersect.globalName());
  ::odedRegCmds()->removeCmd(m_pSubtract.groupName()     , m_pSubtract.globalName());
  ::odedRegCmds()->removeCmd(m_pUnion.groupName()        , m_pUnion.globalName());
  ::odedRegCmds()->removeCmd(m_pAuditBrep.groupName()    , m_pAuditBrep.globalName());
  ::odedRegCmds()->removeCmd(m_pCopyBrep.groupName()     , m_pCopyBrep.globalName());
  ::odedRegCmds()->removeCmd(m_pExSweep.groupName()      , m_pExSweep.globalName());
}
