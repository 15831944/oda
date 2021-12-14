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

// ExDynamicBlocks.h : main header file for the EXDYNAMICBLOCKS DLL
//

#if !defined(AFX_CODA1_14EXAMPLESEXDYNAMICBLOCKSEXDYNAMICBLOCKSMODULE_H__15EEEEB5_ABDB_46CE_92FF_FAE8BCB28A87__INCLUDED_)
#define AFX_CODA1_14EXAMPLESEXDYNAMICBLOCKSEXDYNAMICBLOCKSMODULE_H__15EEEEB5_ABDB_46CE_92FF_FAE8BCB28A87__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RxModule.h"
#include "Ed/EdCommandStack.h"
#include "StaticRxObject.h"
#include "DbDynBlockReference.h"
#include "DbCommandContext.h"
#define DYNAMIC_GRIPS_TEST
#ifdef DYNAMIC_GRIPS_TEST
#include "GripPoints.h"
#endif
#define DECLARE_COMMAND( Name, GroupName )\
class Name##Cmd : public OdEdCommand\
{\
public:\
  const OdString groupName() const { return OD_T(#GroupName); }\
  const OdString globalName() const { return OD_T(#Name); }\
  void execute(OdEdCommandContext* pCmdCtx);\
}

DECLARE_COMMAND( ListProperties, DynamicBlocks );
DECLARE_COMMAND( BlockMove, DynamicBlocks );
DECLARE_COMMAND( ResetBlock, DynamicBlocks );
DECLARE_COMMAND( ConvertToStaticBlock, DynamicBlocks );
DECLARE_COMMAND( ModifyProperties, DynamicBlocks );
DECLARE_COMMAND( PrintDynamicBlockNames, DynamicBlocks );
DECLARE_COMMAND( PrintGraph, DynamicBlocks );
DECLARE_COMMAND( UpdateAnonymousBlocks, DynamicBlocks);

class ExDynamicBlocksModule : public OdRxModule
{
  OdStaticRxObject<ListPropertiesCmd> _cmd1;
  OdStaticRxObject<BlockMoveCmd> _cmd2;
  OdStaticRxObject<ModifyPropertiesCmd> _cmd3;
  OdStaticRxObject<ResetBlockCmd> _cmd4;
  OdStaticRxObject<ConvertToStaticBlockCmd> _cmd5;
  OdStaticRxObject<PrintDynamicBlockNamesCmd> _cmd6;
  OdStaticRxObject<PrintGraphCmd> _cmd7;
  OdStaticRxObject<UpdateAnonymousBlocksCmd> _cmd8;
#ifdef DYNAMIC_GRIPS_TEST
  OdStaticRxObject<OdDbDynBlockReferenceGripPointsOverrule> _grips;
#endif
protected:
	ExDynamicBlocksModule();

  void initApp();

  void uninitApp();

public:
	~ExDynamicBlocksModule();
};

typedef OdSmartPtr<ExDynamicBlocksModule> ExDynamicBlocksModulePtr;

#endif // !defined(AFX_CODA1_14EXAMPLESEXDYNAMICBLOCKSEXDYNAMICBLOCKSMODULE_H__15EEEEB5_ABDB_46CE_92FF_FAE8BCB28A87__INCLUDED_)
