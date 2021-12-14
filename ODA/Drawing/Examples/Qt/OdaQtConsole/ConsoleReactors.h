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
//
// ConsoleReactors.h
//

#ifndef ODA_QT_CONSOLE_REACTORS_H_
#define ODA_QT_CONSOLE_REACTORS_H_

#include "OdaCommon.h"
#include "RxObjectImpl.h"
#include "Ed/EdCommandStack.h"
#include "RxDLinkerReactor.h"

#include <QPointer>
#include "OdqCommandLineEdit.h" // for TCmdMap

//////////////////////////////////////////////////////////////////////////

class OdqCommandReactor : public OdRxObjectImpl<OdEdCommandStackReactor>
{
  TCmdMap m_mapCmds;
  mutable QPointer<class OdqConsoleTab> m_pTestLast;

  OdqCommandReactor();
public:
  virtual ~OdqCommandReactor();
  // Creates an instance
  static OdEdCommandStackReactorPtr createObject();

  // ----- support to get command list -----
  virtual void commandAdded(OdEdCommand* pCommand);
  virtual void commandWillBeRemoved(OdEdCommand* pCommand);

  // ----- support command diagnostic in console -----
  virtual void commandWillStart(OdEdCommand* pCommand, 
                                OdEdCommandContext* pCmdCtx);
  virtual void commandEnded(OdEdCommand* pCommand, 
                            OdEdCommandContext* pCmdCtx);
  virtual void commandCancelled(OdEdCommand* pCommand, 
                                OdEdCommandContext* pCmdCtx);
  virtual void commandFailed(OdEdCommand* pCommand, 
                             OdEdCommandContext* pCmdCtx);
  virtual OdEdCommandPtr unknownCommand(const OdString& commandName, 
                                        OdEdCommandContext* pCmdCtx);

  // -----

  const TCmdMap& getCommandMap() const;
};

//////////////////////////////////////////////////////////////////////////
// instance of OdqCommandLastReactor should be added the last 
// to prevent message about unknown command
// via entering of system variable name in console
// see unknownCommand of class VarNameToSetVarCmdReactor

class OdqCommandLastReactor : public OdRxObjectImpl<OdEdCommandStackReactor>
{
  OdqCommandLastReactor();
public:
  virtual ~OdqCommandLastReactor();
  // Creates an instance
  static OdEdCommandStackReactorPtr getObject(); // single instance // instead createObject()

  virtual OdEdCommandPtr unknownCommand(const OdString& commandName, 
                                        OdEdCommandContext* pCmdCtx);
};

//////////////////////////////////////////////////////////////////////////
// TODO to get module names and ? command dictionaty : any, dwg, dgn

typedef OdSmartPtr<OdRxDLinkerReactor> OdRxDLinkerReactorPtr;

class OdqDynLinkerReactor : public OdRxObjectImpl<OdRxDLinkerReactor>
{
  OdString m_sModuleName;

  OdqDynLinkerReactor();
public:
  virtual ~OdqDynLinkerReactor();
  // Creates an instance
  static OdRxDLinkerReactorPtr createObject();

  // TODO : update m_sModuleName via next overloads
  //virtual void rxAppWillBeLoaded(const OdString &sDrxModulePathName);

  virtual void rxAppLoaded(OdRxModule* pModule);
  virtual void rxAppUnloaded(const OdString& sModuleName);
};

#endif // ODA_QT_CONSOLE_REACTORS_H_
