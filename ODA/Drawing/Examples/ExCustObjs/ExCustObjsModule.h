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

#ifndef EXCUSTMODULE_INCLUDED
#define EXCUSTMODULE_INCLUDED


#include "RxDynamicModule.h"
#include "Ed/EdCommandStack.h"
#include "StaticRxObject.h"

#define ODDB_COMMANDS_GROUP_NAME OD_T("ODDB_EDIT")

#define CMD_DEF_CLASS(CmdName, GroupName)
#define CMD_DEF(CmdName, GroupName) \
class _##CmdName##_cmd : public OdEdCommand \
{ \
public: \
  const OdString groupName() const;\
  const OdString globalName() const;\
  void execute(OdEdCommandContext* pCmdCtx);\
};
#include "DbCmdDef.h"
#undef CMD_DEF_CLASS
#undef CMD_DEF

#include "cmd_setreactor.h"


namespace OdDbCmd
{
#define CMD_MSG_DEF(code, string)  code,
  enum MessageId
  {
#include "DbCmdMsgDef.h"
    kLastMassage
  };
#undef CMD_MSG_DEF

  OdString formatMessage(unsigned int id, va_list* = 0);
  OdString formatMsg(unsigned int id, ...);
};

class ExCustObjsModule : public OdRxModule
{

#define CMD_DEF(CmdName, GroupName) \
  OdStaticRxObject<_##CmdName##_cmd> m_##CmdName##_cmd;
#include "DbCmdDef.h"
#undef CMD_DEF

public:

  void initApp();
  void uninitApp();
};

#endif  //#ifndef EXCUSTMODULE_INCLUDED

