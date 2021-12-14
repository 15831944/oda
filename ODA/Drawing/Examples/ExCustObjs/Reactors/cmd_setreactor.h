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

#ifndef _SETREACTOR_CMD_INCLUDED
#define _SETREACTOR_CMD_INCLUDED

#include "Ed/EdCommandStack.h"
#include "DbgObjectReactor.h"
#include "DbgDatabaseReactor.h"
#include "DbgTransactionReactor.h"
#include "DbgEventReactor.h"
#include "DbgLongTransactionReactor.h"
#include "DbgReactorOut.h"
#include "StaticRxObject.h"


class _setreactor_cmd : public OdEdCommand
{
  OdStaticRxObject<OdDbgReactorMsg<OdDbgObjectReactor> >      m_ObjReactor;
  OdStaticRxObject<OdDbgReactorMsg<OdDbgDatabaseReactor> >    m_DbReactor;
  OdStaticRxObject<OdDbgReactorMsg<OdDbgTransactionReactor> > m_TransReactor;
  OdStaticRxObject<OdDbgReactorMsg<OdDbgEditorReactor> >      m_EditorReactor;
  OdStaticRxObject<OdDbgReactorIOMsg<OdDbgLongTransactionReactor> > m_LongTransactionReactor;
public:
  ~_setreactor_cmd();

  const OdString groupName() const;
  const OdString globalName() const;
  void execute(OdEdCommandContext* pCmdCtx);
};

#endif  //#ifndef _SETREACTOR_CMD_INCLUDED

