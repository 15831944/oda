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

// ExClipUnderlayCommand.h: implementation of the ClipUnderlay commands.
//
//////////////////////////////////////////////////////////////////////

#ifndef __EX_CLIP_UNDERLAY_COMMANDS__
#define __EX_CLIP_UNDERLAY_COMMANDS__

#include "Ed/EdCommandStack.h"


template<class Underlay>
class ClipUnderlayCommand : public OdEdCommand
{
public:
  const OdString groupName() const { return OD_T("Underlay"); }
  const OdString globalName() const { return moduleName() + OD_T("clip"); };
  OdString moduleName() const;
  void execute(OdEdCommandContext* pCmdCtx);
};

#endif // __EX_CLIP_UNDERLAY_COMMANDS__
