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
// CommandContextImpl.cpp
//

#include <QtCore>
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtGui>
#else
#include <QtWidgets>
#endif

#include "CommandContextImpl.h"
#include "OdqConsole.h" //#include "OdqConsoleTab.h"
#include "ConsoleModule.h"
#include "OdqClipData.h"
#include "SysVarPE.h"

#include "DbBaseDatabase.h"
#include "DbHostAppServices.h"
#include "ExtDbModule.h"

#include "Ge/GeExtents2d.h"
#include "DbViewport.h"
#include "DbViewportTable.h"
#include "DbLayerTableRecord.h"

#include "SaveState.h"
#include "ExKWIndex.h"

//////////////////////////////////////////////////////////////////////////

ODRX_NO_CONS_DEFINE_MEMBERS(OdErrorExecuteCommandAfterCancelContext, OdErrorContext)

OdSmartPtr<OdErrorExecuteCommandAfterCancelContext> 
    OdErrorExecuteCommandAfterCancelContext::init(
                                const OdString &sCmdExecuteAfterCancel,
                                bool bEchoExecuteAfterCancel)
{
  m_sCmdExecuteAfterCancel = sCmdExecuteAfterCancel;
  m_bEchoExecuteAfterCancel = bEchoExecuteAfterCancel;
  return this;
}

OdResult OdErrorExecuteCommandAfterCancelContext::code() const 
{ 
  return eExtendedError; // return eOk; 
}

OdString OdErrorExecuteCommandAfterCancelContext::description() const
{
  return m_sCmdExecuteAfterCancel;
}

//////////////////////////////////////////////////////////////////////////

ConsoleIoImpl::ConsoleIoImpl(OdqConsoleTab& tabConsole)
 : m_tabConsole(tabConsole)
{
}

ConsoleIoImpl::~ConsoleIoImpl()
{
}

// Creates an instance of and returns SmartPointer to it
// (addref & release implemented through OdRxObjectImpl)
//static 
OdSmartPtr<OdEdBaseIO> ConsoleIoImpl::createObject(OdqConsoleTab &tabConsole)
{
  OdSmartPtr<OdEdBaseIO> pOdEdBaseIO;
  // (addref & release implemented through OdRxObjectImpl)
  pOdEdBaseIO.attach(new ConsoleIoImpl(tabConsole)); // without addRef
  return pOdEdBaseIO;
}

// ----- OdEdBaseIO -----

OdString ConsoleIoImpl::getString(const OdString& sPrompt,
                                  int options,
                                  OdEdStringTracker* pTracker)
{
  CommandState& stateCmd = m_tabConsole.getTopCmdState();
  ODA_ASSERT_ONCE(stateCmd.isAllocated());

  stateCmd.setPrompt(sPrompt);

  if (stateCmd.m_pStringIO.get() && !stateCmd.m_pStringIO->isEof())
  {
    OdString sRes(stateCmd.m_pStringIO->getString(sPrompt, options, pTracker));
    if (options & OdEd::kGstAllowSpaces)
      sRes.trimRight();

    if (!sRes.find(L'\\')) // #13896 - Menu Draw/Circle/Center, Diameter
    {
      // skip entering of point form stateCmd.m_pStringIO content
      OdString sNext = sRes.mid(1);
      if (stateCmd.m_pStringIO.get() && !stateCmd.m_pStringIO->isEof())
      {
        if (!sNext.isEmpty())
          sNext += L' '; 
        sNext += stateCmd.m_pStringIO->getInitString(); // save
      }
      stateCmd.m_pStringIO->reset(L"");

      OdString sRes = getString(sPrompt, options, pTracker);
      
      stateCmd.m_pStringIO->reset(sNext); // restore
      return sRes;
    }
    ODA_ASSERT_ONCE(sRes.find(L"^C") && sRes.trimRight() != L";"); // possible TODO

    if (!stateCmd.isDefaultPrompt()) // is argument
      m_tabConsole.putMsgText(sRes, enMsgTextPrompted); 
    else
      // the next will be done before execute 
      // m_tabConsole.putMsgText(sRes, enMsgTextPrompted);
      ODA_ASSERT_ONCE(true); // brk

    if (sRes == OD_T("\"\""))
    {
      if (GETBIT(options, OdEd::kInpThrowEmptyInQuotes))
        throw OdEdEmptyInput();
      sRes = OdString::kEmpty;
    }
    return sRes;
  }

  OdString sRes = m_tabConsole.getPromptedString(options, pTracker);

  m_tabConsole.putMsgText(sRes, TMsgType(enMsgTextPrompted | enMsgTextByInput)); 

  if (sRes == OD_T("\"\""))
  {
    if (GETBIT(options, OdEd::kInpThrowEmptyInQuotes))
      throw OdEdEmptyInput();
    sRes = OdString::kEmpty;
  }
  return sRes;
}

void ConsoleIoImpl::putString(const OdString &sText)
{
  m_tabConsole.putMsgText(sText);
}

OdGePoint3d ConsoleIoImpl::getPoint(const OdString& sPrompt, 
                                    int options, 
                                    OdEdPointTracker* pTracker)
{
  CommandState& stateCmd = m_tabConsole.getTopCmdState();
  ODA_ASSERT_ONCE(stateCmd.isAllocated());

  stateCmd.setPrompt(sPrompt);

  if (stateCmd.m_pStringIO.get() && !stateCmd.m_pStringIO->isEof())
  {
    //OdString sRes(stateCmd.m_pStringIO->getString(sPrompt, options, pTracker));
    OdString sRes;
    do 
    {
      sRes = stateCmd.m_pStringIO->getString(sPrompt, OdEd::kGstDefault, NULL);
      if (sRes.isEmpty() && GETBIT(options, OdEd::kInpThrowEmpty)) // possible via ^M ^J
        throw OdEdEmptyInput();
    }
    while (sRes.isEmpty() && stateCmd.m_pStringIO.get() && !stateCmd.m_pStringIO->isEof());

    if (!sRes.find(L'\\')) // #13896 - Menu Draw/Circle/Center, Diameter
    {
      // skip entering of point form stateCmd.m_pStringIO content
      OdString sNext = sRes.mid(1);
      if (stateCmd.m_pStringIO.get() && !stateCmd.m_pStringIO->isEof())
      {
        if (!sNext.isEmpty())
          sNext += L' '; 
        sNext += stateCmd.m_pStringIO->getInitString(); // save
      }
      stateCmd.m_pStringIO->reset(L"");

      OdGePoint3d ptRes = getPoint(sPrompt, options, pTracker);
      
      stateCmd.m_pStringIO->reset(sNext); // restore
      return ptRes;
    }
    ODA_ASSERT_ONCE(sRes.find(L"^C") && sRes.trimRight() != L";"); // possible TODO

    if (!sRes.isEmpty())
    {
      if (!stateCmd.isDefaultPrompt()) // is argument
        m_tabConsole.putMsgText(sRes, enMsgTextPrompted); 
      else
        // the next will be done before execute 
        // m_tabConsole.putMsgText(sRes, enMsgTextPrompted);
        ODA_ASSERT_ONCE(true); // brk

      throw OdEdOtherInput(sRes);
    }
  }

  OdGePoint3d pointRes = m_tabConsole.getPromptedPoint(options);
  return pointRes;
}

OdUInt32 ConsoleIoImpl::getKeyState()
{
  return m_tabConsole.getKeyState();
}

//------------------------------------------------------------------------

CommandState::CommandState()
  : m_bInUse(false)
  , m_flagsCmd(0)
  , m_levelCmd(0)
  , m_optionsRequestToGet(0)
  , m_bRequestToGetString(false)
  , m_bGetStringReady(false)
  , m_bRequestToGetPoint(false)
  , m_bGetPointReady(false)
//, m_pErrCtx(NULL) 
  , m_qsCommandPrompt(CommandState::getDefaultQPrompt()) // "Command"
{
  ODA_ASSERT_ONCE(isEmpty());
}

CommandState::CommandState(const CommandState &from)
  : m_bInUse(false)
  , m_flagsCmd(0)
  , m_levelCmd(0)
  , m_optionsRequestToGet(0)
  , m_bRequestToGetString(false)
  , m_pStringTracker(NULL)
  , m_bGetStringReady(false)
  , m_bRequestToGetPoint(false)
  , m_bGetPointReady(false)
//, m_pErrCtx(NULL) 
  , m_qsCommandPrompt(CommandState::getDefaultQPrompt()) // "Command"
{
  operator=(from);
}

CommandState &CommandState::operator=(const CommandState &from)
{
  if (this == &from)
    return *this;

  m_bInUse = from.m_bInUse;
  m_flagsCmd = from.m_flagsCmd;
  m_qsLastInput.clear(); // = from.m_qsLastInput; 
  m_qsLastCommandName.clear(); // = from.m_qsLastCommandName;

  m_levelCmd = from.m_levelCmd;
  
  m_qsCommandPrompt = from.m_qsCommandPrompt;
  m_qsPromptAndKeywords.clear(); // = from.m_qsPromptAndKeywords

  m_qsCommandEcho = from.m_qsCommandEcho;

  m_pStringIO = from.m_pStringIO;

  m_optionsRequestToGet = from.m_optionsRequestToGet;

  m_bRequestToGetString = from.m_bRequestToGetString;
  m_pStringTracker = NULL;
  m_bGetStringReady = from.m_bGetStringReady;
  m_qsGetString = from.m_qsGetString;

  m_bRequestToGetPoint = from.m_bRequestToGetPoint;
  m_bGetPointReady = from.m_bGetPointReady;
  m_ptGetPoint = from.m_ptGetPoint;
  m_ptGetEyes = from.m_ptGetEyes;
  //m_pPointTracker = NULL;
  
  ODA_ASSERT_ONCE(from.m_pErrCtx.isNull());
  m_pErrCtx = NULL;

  m_vValue = from.m_vValue;

  return *this;
}

CommandState::~CommandState()
{
 #ifdef QT_DEBUG 
  if (isErrorExecuteAfterCancel(m_pErrCtx))
  {
    const ExCancelAndExecute &err 
        = *(const ExCancelAndExecute *) m_pErrCtx.get();
    ODA_ASSERT_ONCE(err.getCommand().isEmpty()); // possible when app was closed
  }
  else
      ODA_ASSERT_ONCE(m_pErrCtx.isNull());
 #endif

  m_pErrCtx = NULL; //delete m_pErrCtx;
}

bool CommandState::isAllocated() const
{
  return m_bInUse;
}

//static
OdString CommandState::getDefaultPrompt()
{
  return getIApp()->toOdString(getDefaultQPrompt());
}

//static
QString CommandState::getDefaultQPrompt()
{
  return QObject::tr("Command");
}

bool CommandState::isDefaultPrompt() const
{
  return (m_qsCommandPrompt == getDefaultQPrompt());
}

OdString CommandState::getPrompt() const
{
  return getIApp()->toOdString(getQPrompt());
}

const QString &CommandState::getQPrompt() const
{
  return m_qsCommandPrompt;
}

void CommandState::setPrompt(const OdString &sPrompt)
{
  setPrompt(getIApp()->toQString(sPrompt));
}

static QString cutColon(const QString& qsStr)
{
  int indexColon = qsStr.lastIndexOf(':');
  if (indexColon > 0 && indexColon >= (qsStr.length() - 2))
    // cut the colon
    return qsStr.left(indexColon);

  return qsStr;
}

void CommandState::setPrompt(const QString &qsPrompt)
{
  ODA_ASSERT_ONCE(!qsPrompt.isEmpty());

  m_qsCommandPrompt = cutColon(qsPrompt);
  if (!m_qsPromptAndKeywords.isEmpty() && m_qsPromptAndKeywords.indexOf(m_qsCommandPrompt) < 0)
    m_qsPromptAndKeywords.clear();
}

void CommandState::setKeywordsAtPrompt(const OdString& sPrompt, 
                                       const OdString& sKeywords)
{
  QString qsPrompt = getIApp()->toQString(sPrompt), 
          qsKeywords = getIApp()->toQString(sKeywords);

  if (qsPrompt.isEmpty() || qsKeywords.isEmpty())
  {
    ODA_ASSERT_ONCE(!qsPrompt.isEmpty());
    return;
  }

  m_qsPromptAndKeywords = cutColon(qsPrompt);
  m_qsPromptAndKeywords += ": ";
  m_qsPromptAndKeywords += qsKeywords;
}

bool CommandState::isEmpty() const
{
  return m_vValue.varType() == OdVariant::kVoid; // undefined
}

const OdVariant& CommandState::getValue() const
{
  return m_vValue;
}

void CommandState::setValue(const OdVariant& vValue)
{
  m_vValue = vValue;
}

const OdString& CommandState::getInputString() const
{
  if (m_pStringIO.isNull())
    return OdString::kEmpty;
  return m_pStringIO->getInitString();
}

void CommandState::setInputString(const OdString& sVal)
{
  if (m_pStringIO.isNull())
  {
    if (!sVal.isEmpty())
      m_pStringIO = ExStringIO::create(sVal);
  }
  else
    m_pStringIO->reset(sVal);
}

void CommandState::putCmdStringEcho(OdEdCommandContext* pCmdCtx,
                                    OdEdCommand* pCmd) // = NULL
{
  ODA_ASSERT_ONCE(pCmdCtx);

  if (m_qsLastCommandName.indexOf("'") >= 0)
    return;

  OdqConsoleTab& tabConsole = *OdqConsole::getConsoleTab(pCmdCtx->baseDatabase(),
                                                         false); // bSetCurrent
  if (!m_qsCommandEcho.isEmpty())
  {
    // echo of full command string
    tabConsole.putMsgText(m_qsCommandEcho, TMsgType(enMsgTextPrompted | enMsgTextByInput));
    m_qsCommandEcho.clear();
  }

  if (pCmd)
  {
    // possible via executing command with tmp local context in some of test tx-es // ODA_ASSERT_ONCE(!m_qsLastInput.isEmpty());
    tabConsole.putMsgText(m_qsLastInput, enMsgTextPrompted);
  }
}

OdEdPointTrackerPtr CommandState::pointTracker() const
{
  return m_pPointTracker;
}

void CommandState::setPointTracker(OdEdPointTracker* pPointTracker)
{
  m_pPointTracker = pPointTracker;
}

OdqSnapManagerPtr CommandState::snapManager() const
{
  return m_pSnapManager;
}

void CommandState::setSnapManager(OdqSnapManager* pSnapManager)
{
  m_pSnapManager = pSnapManager;
}

//////////////////////////////////////////////////////////////////////////

class SaveViewParams
{
  CommandContextImpl& m_contextCmd;
  OdEdPointTrackerPtr m_pTrackerSave;
  OdSharedPtr<QCursor> m_pCursorSave;
  
  OdqSnapManagerPtr m_pSnapManager;
  OdGsView* m_pGsView;
  OdGePoint3d m_basePt;

public:
  SaveViewParams(CommandContextImpl& contextCmd, 
                 int options,
                 const OdGePoint3d* pBasePt,
                 OdEdInputTracker* pTracker, 
                 const QCursor* pQCursor = NULL)
    : m_contextCmd(contextCmd)
    , m_pGsView(NULL)
  {
    IqView* iView = getView();
    if (!iView || !(m_pGsView = iView->getActiveTopGsView()))
    {
      ODA_FAIL_ONCE();
      return;
    }

    CommandState& stateCmd = m_contextCmd.getTopCmdState();

    m_pTrackerSave = stateCmd.m_pPointTracker;
    if (pTracker)
    {
      stateCmd.m_pPointTracker = pTracker;

      if (stateCmd.m_pPointTracker.get() != m_pTrackerSave.get())
        pTracker->addDrawables(m_pGsView); //m_pGsView->track(pTracker);
    }

    if (pQCursor)
    {
      m_pCursorSave = new QCursor(iView->cursor());
      iView->setCursor(*pQCursor);
    }

    m_pSnapManager = stateCmd.snapManager();
    bool isSnapEnabled = getIAppProps()->toBool(L"Vars/snap", true);
    if (   isSnapEnabled
        && OdqSnapManager::isSupported(contextCmd.baseDatabase()) 
        && !GETBIT(options, OdEd::kGptNoOSnap)
        && (   stateCmd.m_pStringIO.isNull() || stateCmd.m_pStringIO->isEof() // input string is not ready
            || stateCmd.m_pStringIO->getInitString().getAt(0) == L'\\')) // is used to skip once in menu
    {
      // initSnapping
      if (m_pSnapManager.isNull())
      {
        // it is here (out grip manager code) to support draw commands (line, circle ...) too
        m_pSnapManager = OdqSnapManager::createObject(contextCmd.baseDatabase());
        if (m_pSnapManager.isNull())
          return;
        stateCmd.setSnapManager(m_pSnapManager);
      }

      m_pSnapManager->track(pTracker);
      m_pSnapManager->setBasePtRef(NULL);
      OdGsModel* p2dModel = iView->getGsModel(true);
      m_pGsView->add(m_pSnapManager.get(), p2dModel);

      if (pBasePt)
      {
        m_basePt = *pBasePt;
        m_pSnapManager->setBasePtRef(&m_basePt);
      }
      else if (pTracker)
      {
        OdEdPointDefTrackerPtr pPointDefTracker = OdEdPointDefTracker::cast(pTracker);
        if (pPointDefTracker.get())
        {
          m_basePt = pPointDefTracker->basePoint();
          m_pSnapManager->setBasePtRef(&m_basePt);
        }
      }
    }
    else if (!isSnapEnabled && m_pSnapManager.get())
      m_pSnapManager = NULL;
  }

  ~SaveViewParams()
  {
    IqView* iView = getView();
    ODA_ASSERT_ONCE(m_pGsView);
    CommandState& stateCmd = m_contextCmd.getTopCmdState();

    if (   iView && !stateCmd.m_pPointTracker.isNull()
        && stateCmd.m_pPointTracker.get() != m_pTrackerSave.get())
      stateCmd.m_pPointTracker->removeDrawables(m_pGsView); //m_pGsView->track(NULL);

    stateCmd.m_pPointTracker = m_pTrackerSave;

    if (iView && !m_pCursorSave.isNull())
      iView->setCursor(*m_pCursorSave);

    if (m_pSnapManager.get())
    {
      // uninitSnapping
      if (m_pSnapManager->basePtRef() == &m_basePt)
        m_pSnapManager->setBasePtRef(NULL);
      m_pGsView->erase(m_pSnapManager.get());
      m_pSnapManager->track(NULL);
    }
  }

  IqView* getView()
  {
    OdRxObject* pRxDatabase = m_contextCmd.baseDatabase();
    IqViewSystem* iVS = getIViewSystem();
    if (!iVS)
      return NULL;
    return iVS->getView(pRxDatabase);
  }
};

//////////////////////////////////////////////////////////////////////////

CommandContextImpl::CommandContextImpl()
    : m_levelCmdToOffEcho(1)
    , m_typeEcho(enEchoByMessageAndInput) // (enEchoFull)
{
  m_stackCommandStates.push_back(CommandState()); // top element should exist
  m_stackCommandStates.last().m_levelCmd = 0; // m_stackCommandStates.top().m_levelCmd = 0;
}

CommandContextImpl::~CommandContextImpl()
{
}

//static 
OdDbCommandContextPtr CommandContextImpl::createObject(OdEdBaseIO* pIOStream, 
                                                       OdRxObject* pRxDb)
{
  //ExDbCommandContextPtr pCtx = OdRxObjectImpl<CommandContextImpl, 
  //                                            ExDbCommandContext>::createObject();
  // fix  ambiguous conversions from 'OdRxObject *' via get() * ->
  OdDbCommandContextPtr pCtx = OdRxObjectImpl<CommandContextImpl, 
                                              OdDbCommandContext>::createObject();
  CommandContextImpl* pExCmdCtx = static_cast<CommandContextImpl*>(pCtx.get());
  pExCmdCtx->m_pRxDb = pRxDb;
  pExCmdCtx->m_pDb = NULL;
  OdDbDatabasePtr pDwgDb = OdDbDatabase::cast(pRxDb);
  if (!pDwgDb.isNull())
    pExCmdCtx->m_pDb = pDwgDb; // database() is not virtual now
  pExCmdCtx->m_pIoStream = pIOStream;
  ODA_ASSERT_ONCE(!pExCmdCtx->m_stackCommandStates.isEmpty()); // top element should exist

  return pCtx;
}

OdRxObject* CommandContextImpl::baseDatabase()
{
  return m_pRxDb; // return m_pDb;
}

//static 
OdRxObject* CommandContextImpl::m_pRxDatabaseDragSource = NULL;
// static 
OdRxObject* CommandContextImpl::baseDatabaseDragSource()
{
  return m_pRxDatabaseDragSource;
}

CommandState& CommandContextImpl::getTopCmdState()
{
  ODA_ASSERT_ONCE(!m_stackCommandStates.isEmpty());

  CommandState& stateCmd = m_stackCommandStates.last(); // = m_stackCommandStates.top();
  return stateCmd;
}

const CommandState& CommandContextImpl::getTopCmdState() const
{
  ODA_ASSERT_ONCE(!m_stackCommandStates.isEmpty());

  const CommandState& stateCmd = m_stackCommandStates.last(); // = m_stackCommandStates.top();
  return stateCmd;
}

//////////////////////////////////////////////////////////////////////////

class SaveHintType
{
  QPointer<OdqCommandLineEdit> m_pEditCmdLine;
  THintType m_typeSave;

public:
  SaveHintType(OdqCommandLineEdit& editCmdLine, THintType type)
    : m_pEditCmdLine(&editCmdLine)
  {
    m_typeSave = m_pEditCmdLine->setType(type);
  }

  ~SaveHintType()
  {
    if (!m_pEditCmdLine.isNull()) // #10155 Crash by close application during 3dConstrainedOrbit
      m_pEditCmdLine->setType(m_typeSave);
  }

private:
  Q_DISABLE_COPY(SaveHintType)
};

//////////////////////////////////////////////////////////////////////////

bool CommandContextImpl::isEchoOn(TMsgType typeMsg) // = enMsgTextNormal
{
  if (!(typeMsg & enMsgTextPrompted))
  {
    if (!(m_typeEcho & enEchoByMessage))
    {
      ODA_FAIL_ONCE(); // unused now
      return false;
    }

    return true;
  }

  if (!(typeMsg & enMsgTextByInput))
  {
    if (!(m_typeEcho & enEchoByExecution))
      return false;
  }
  else
  {
    if (!(m_typeEcho & enEchoByInput))
      return false;
  }

  return getTopCmdState().m_levelCmd < m_levelCmdToOffEcho;
}

TEchoType CommandContextImpl::getEchoType() const
{
  return m_typeEcho;
}

void CommandContextImpl::setEchoType(TEchoType typeEcho)
{
  m_typeEcho = typeEcho;
}

int CommandContextImpl::getCmdLevelToOffEcho() const
{
  return m_levelCmdToOffEcho;
}

void CommandContextImpl::setCmdLevelToOffEcho(int level)
{
  m_levelCmdToOffEcho = level;
}

bool CommandContextImpl::hasCmdWithUndo(bool bIgnoreMarkedToCancel) const // = false
{
  OdRxObject* pRxDatabase = const_cast<CommandContextImpl*>(this)->baseDatabase();
  if (!pRxDatabase)
    return false;

  QLinkedListIterator<CommandState> itr(m_stackCommandStates);
  itr.toBack();
  while (itr.hasPrevious())
  {
    const CommandState& stateCmd = itr.previous();
    if (!stateCmd.isAllocated())
    {
      ODA_ASSERT_ONCE(!itr.hasPrevious());
      break;
    }

    if (GETBIT(stateCmd.m_flagsCmd, OdEdCommand::kNoUndoMarker))
      continue;

    if (   bIgnoreMarkedToCancel
        && isErrorExecuteAfterCancel(stateCmd.m_pErrCtx))
      continue;

    return true;
  }

  return false;
}

bool CommandContextImpl::markToCancelAndExecute(const OdString& csCmdAfterCancel,
                                                bool bEchoAfterCancel)
{
  //ODA_ASSERT_ONCE(hasCmdWithUndo()); // && !csCmdAfterCancel.isEmpty()

  QLinkedList<CommandState>::iterator itr;
  bool bRes = false; // false if there is nothing to cancel
  OdString sCmdAfterCancel(csCmdAfterCancel),
           sCmdLow; 
  bool bCancelForAllDbs = (   !(sCmdLow = sCmdAfterCancel).makeLower().find(L"quit") 
                           || !sCmdLow.find(L"closeall"));
  if (bCancelForAllDbs)
  {
    foreach(OdRxObject* pRxDb, getIConsole()->getOpenDatabases())
    {
      if (pRxDb == baseDatabase())
        continue;
      CommandContextImpl* pCtx = static_cast<CommandContextImpl*>(getIConsole()->getCommandContext(pRxDb));
      ODA_ASSERT_ONCE(pCtx);
      if (!pCtx)
        continue;

      for (itr = pCtx->m_stackCommandStates.begin(); 
           itr != pCtx->m_stackCommandStates.end(); itr++)
      {
        CommandState& stateCmd = *itr;
        if (   !stateCmd.isAllocated() 
            || (   !sCmdAfterCancel.isEmpty() 
                && GETBIT(stateCmd.m_flagsCmd, OdEdCommand::kNoUndoMarker)))
          continue;
        ODA_ASSERT_ONCE(   stateCmd.m_pErrCtx.isNull()
                        || !isErrorExecuteAfterCancel(stateCmd.m_pErrCtx));
        // retuen value via own context only//bRes = true;
        stateCmd.m_pErrCtx 
          = OdRxObjectImpl<OdErrorExecuteCommandAfterCancelContext>::createObject()
                ->init(L"close", false);
        stateCmd.m_qsGetString = MARKER_TO_CANCEL;
        stateCmd.m_bGetStringReady = true;
      }
    }
  }

  for (itr = m_stackCommandStates.begin(); itr != m_stackCommandStates.end(); itr++)
  {
    CommandState& stateCmd = *itr;
    if (   !stateCmd.isAllocated()
        || (   !sCmdAfterCancel.isEmpty()
            && GETBIT(stateCmd.m_flagsCmd, OdEdCommand::kNoUndoMarker)))
      continue;

    //if (stateCmd.m_pErrCtx)
    //{
    //  ODA_ASSERT_ONCE(stateCmd.m_pErrCtx->code() == eOk); // temp solution for ExHover
    //  // throw *(ExHover *)(OdError *) err;
    //  delete stateCmd.m_pErrCtx;
    //  stateCmd.m_pErrCtx = NULL;
    //}

    bRes = true;
    if (stateCmd.m_pErrCtx.isNull() || !isErrorExecuteAfterCancel(stateCmd.m_pErrCtx))
      stateCmd.m_pErrCtx 
        = OdRxObjectImpl<OdErrorExecuteCommandAfterCancelContext>::createObject()
              ->init(sCmdAfterCancel, bEchoAfterCancel);
    else
    {
      ODA_ASSERT_ONCE(stateCmd.m_pErrCtx->description().find(L"close") >= 0);
    }

    // try to cancel it normally at first
    stateCmd.m_qsGetString = MARKER_TO_CANCEL;
    stateCmd.m_bGetStringReady = true;

    sCmdAfterCancel.empty();
  }
  return bRes;
}

// return true if it contains mark to execute with close command
bool CommandContextImpl::isMarkedToCloseWithDeferredCommand()
{
  for (QLinkedList<CommandState>::iterator itr = m_stackCommandStates.begin(); 
       itr != m_stackCommandStates.end(); itr++)
  {
    CommandState& stateCmd = *itr;
    //if (!stateCmd.isAllocated() || GETBIT(stateCmd.m_flagsCmd, OdEdCommand::kNoUndoMarker))
    //  continue;
    if (stateCmd.m_pErrCtx.isNull() || !isErrorExecuteAfterCancel(stateCmd.m_pErrCtx))
      continue;
    if (stateCmd.m_pErrCtx->description().makeLower().find(L"close") <  0)
      continue;

    return true;
  }
  return false;
}


//static 
QString CommandContextImpl::m_qsScriptFileNameToRecord;
//static 
QString CommandContextImpl::m_qsScriptDataToRecord;

// check for script accumulation
void CommandContextImpl::accumulateScriptData(const QString &qsText, 
                                                TMsgType type)
{
  if (m_qsScriptFileNameToRecord.isEmpty())
    return;

  if (   !(type & enMsgTextPrompted)
      || (type & enMsgTextByInput))
    return;

  m_qsScriptDataToRecord += qsText;
  m_qsScriptDataToRecord += "\r\n";
}

// ----- OdEdCommandContext -----

void CommandContextImpl::setArbitraryData(const OdString &sPathName, OdRxObject *pDataObj)
{
  OdRxObject* pRxDatabase = baseDatabase();

  OdqConsoleTab& tab = *OdqConsole::getConsoleTab(pRxDatabase, false);
  tab.setArbitraryData(sPathName, pDataObj);
}

OdRxObjectPtr CommandContextImpl::arbitraryData(const OdString &sPathName) const
{
  OdRxObject* pRxDatabase = const_cast<CommandContextImpl*>(this)->baseDatabase();

  OdqConsoleTab& tab = *OdqConsole::getConsoleTab(pRxDatabase, false);
  return tab.arbitraryData(sPathName);
}

// ----- ExCommandContext -----

//OdString CommandContextImpl::getSubCommandString(bool bSubCmdOpenBracketWasGotten) // = false
//{
//  if (!bSubCmdOpenBracketWasGotten)
//  {
//    OdString sSubCommand(userIO()->getString(getTopCmdState().getPrompt()));
//    ODA_ASSERT_ONCE(!sSubCommand.isEmpty());
//
//    if (sSubCommand != "{")
//      return sSubCommand;
//  }
//
//  CommandState& stateCmd = getTopCmdState();
//  ODA_ASSERT_ONCE(stateCmd.isAllocated());
//
//  if (stateCmd.m_pStringIO.get() && !stateCmd.m_pStringIO->isEof())
//  {
//    OdString sPrompt(getIApp()->toOdString(QObject::tr("{_>")));
//    int option = (OdEd::kGstNoEmpty | OdEd::kGst_BracketStarted);
//
//    OdString sSubCommand(userIO()->getString(sPrompt, option));
//    sSubCommand.trimLeft().trimRight();
//
//    if (   sSubCommand.isEmpty() 
//        || sSubCommand[sSubCommand.getLength() - 1] != '}')
//    {
//      ODA_FAIL_ONCE();
//      throw OdError(eInvalidInput);
//    }
//    sSubCommand.deleteChars(sSubCommand.getLength() - 1);
//    sSubCommand.trimRight();
//
//    return sSubCommand;
//  }
//
//  ODA_FAIL_ONCE(); // TODO
//  throw OdError(eInvalidInput);
//}

//OdVariant CommandContextImpl::executeSubCommand(const OdString &sSubCmd)
//{
//  OdqConsoleTab &tab = *OdqConsole::getConsoleTab(baseDatabase(), false);
//  OdVariant vValue = tab.executeCommand(sSubCmd,
//                                        true, // echo
//                                        true); // is subcommand
//  return vValue;
//}

//const OdVariant &CommandContextImpl::getValue() const
//{
//  ODA_ASSERT_ONCE(!m_stackCommandStates.isEmpty());
//  const CommandState& state = m_stackCommandStates.last(); // = m_stackCommandStates.top();
//  
//  return state.m_vValue;
//}
//
//void CommandContextImpl::setValue(const OdVariant &vValue)
//{
//  CommandState& state = getTopCmdState();
//  state.setValue(vValue);
//}
//
//void CommandContextImpl::setValue(bool bValue)
//{
//  OdVariant vValue(bValue);
//  setValue(vValue);
//}

OdqGripManagerPtr CommandContextImpl::gripManager() const
{
  return OdqGripManager::cast(getTopCmdState().pointTracker());
}

void CommandContextImpl::setGripManager(OdqGripManager* pGripManager)
{
  return getTopCmdState().setPointTracker(pGripManager);
}

OdqSnapManagerPtr CommandContextImpl::snapManager() const
{
  return getTopCmdState().snapManager();
}

// for formatting exception
//void CommandContextImpl::setException(const OdSharedPtr<OdException> &wrpException)
//{
//  ODA_ASSERT_ONCE(   !wrpException.isNull()
//            && wrpException->context()
//            && wrpException->context()->isKindOf(
//                  PropFormattingException::desc()));
//  ODA_ASSERT_ONCE(getTopCmdState().m_pErrCtx.isNull()); // test
//
//  getTopCmdState().m_pErrCtx = wrpException;
//}

//OdString CommandContextImpl::tr(const OdString &sText,
//                                bool bMarkResultAsNonTraslated) // = false
//{
//  if (sText.isEmpty())
//    return sText; // there is nothing to translate
//
//  int pos = sText.find(csMarkerToSkipTranslation); // "##"
//  if (!pos)
//    return sText.mid(csMarkerToSkipTranslation.getLength());
//  ODA_ASSERT_ONCE(pos < 0);
//
//  QString qsText(getIApp()->toQString(sText));
//  qsText = QObject::tr(qsText.toAscii());
//
//  OdString sResText(bMarkResultAsNonTraslated ? csMarkerToSkipTranslation : "");
//  sResText += getIApp()->toOdString(qsText);
//
//  return sResText;
//}

// ----- OdEdUserIO -----

// Description:
//     Returns user input of a keyword.
//     Returns the *index* of the entered keyword in the *keyword* list, 
//     or -1 it's not there.
// Arguments:
//     prompt (I) Prompt output before pausing for user input.
//     keywords (I) Keyword list.
//     options (I) Bitwise OR combination of OdEd::CommonInputOptions enum.
//     defVal (I) Returned if user input nothing.
// Note:
//     This function does not throw a KeywordException.

int CommandContextImpl::getKeyword(const OdString& sPrompt,
                                   const OdString& sKeywords,
                                   int defVal, // = -1
                                   int options, // = OdEd::kGstNoEmpty
                                   OdEdIntegerTracker* pTracker) // = NULL
{
  OdqConsoleTab& consoleTab = *OdqConsole::getConsoleTab(baseDatabase(), true);
  SaveHintType svType(consoleTab.getCommandLineEdit(), enHintKeyword);
  getTopCmdState().setKeywordsAtPrompt(sPrompt, sKeywords);

  return ExDbCommandContext::getKeyword(sPrompt, sKeywords, defVal, options, pTracker);
}

static QString removeDoubleQuotes(const QString& qsText, bool bTrim = true)
{
  QChar qchQuotes('\"');
  QString qsRes(bTrim ? qsText.trimmed() : qsText);

  while (!qsRes.indexOf(qchQuotes))
  {
    if (   qsRes.length() == 1
        || !qsRes.endsWith(qchQuotes))
    {
      ODA_FAIL_ONCE(); // syntax error: unpaired quotes
      break;
    }

    qsRes = qsRes.mid(1, qsRes.length() - 2);
  }
  return qsRes;
}

static OdString removeDoubleQuotes(const OdString& csText, bool bTrim = true)
{
  OdString sText(csText);
  if (bTrim)
    sText.trimLeft();
  int pos = sText.find(OdChar('\"'));
  if (!pos)
    sText = getIApp()->toOdString(removeDoubleQuotes(getIApp()->toQString(sText), bTrim));
  return sText;
}

static OdString addDoubleQuotes(const OdString& csText)
{
  OdString sText(csText);
  int pos = sText.find(OdChar('\"'));
  if (pos)
  {
    sText = OD_T("\"");
    sText += csText;
    sText += OD_T("\"");
  }
  return sText;
}

OdString CommandContextImpl::getString(const OdString& sPrompt, 
                                       int options, // = OdEd::kGstNoEmpty
                                       const OdString& sDefVal, // = OdString::kEmpty
                                       const OdString& sKeywords, // = OdString::kEmpty
                                       OdEdStringTracker* pTracker) // = NULL
{
  OdqConsoleTab& consoleTab = *OdqConsole::getConsoleTab(baseDatabase(), false);
  SaveHintType svType(consoleTab.getCommandLineEdit(), pTracker ? enHintTrackingString : enHintString);
  getTopCmdState().setKeywordsAtPrompt(sPrompt, sKeywords);

  OdString sValue = ExDbCommandContext::getString(sPrompt, options, sDefVal, sKeywords, pTracker);
  sValue = removeDoubleQuotes(sValue, !GETBIT(options, OdEd::kGstAllowSpaces));
  if (!sKeywords.isEmpty())
    KWIndex(sKeywords).check(sValue);
  return sValue;
}

int CommandContextImpl::getInt(const OdString& csPrompt,
                               int options, // = OdEd::kGstNoEmpty
                               int defVal, // = 0
                               const OdString& sKeywords, // = OdString::kEmpty
                               OdEdIntegerTracker* ) // pTracker // = NULL
{
  //return ExDbCommandContext::getInt(sPrompt, options, defVal, keywords, pTracker);

  OdString sPrompt(csPrompt);
  if (sPrompt.isEmpty())
    sPrompt = OD_T("Enter an integer value");

  OdqConsoleTab& consoleTab = *OdqConsole::getConsoleTab(baseDatabase(), true);
  SaveHintType svType(consoleTab.getCommandLineEdit(), enHintInt);
  getTopCmdState().setKeywordsAtPrompt(sPrompt, sKeywords);

  int nValue = defVal;

  //if (GETBIT(options, OdEd::kGstNoEmpty))
  //  options |= OdEd::kInpThrowEmpty;

  try
  {
    OdString sDefVal;
    sDefVal.format(OD_T("%d"), defVal);

    OdString sInput = ExDbCommandContext::getString(sPrompt, options, // with exception by empty
                                                    sDefVal, sKeywords);
    if (!sInput.isEmpty())
      nValue = ::odStrToInt(sInput.c_str());
  }
  catch (const OdEdEmptyInput& e)
  {
    if (!GETBIT(options, OdEd::kGstNoEmpty))
      throw e;
  }

  return nValue;
}

double CommandContextImpl::getReal(const OdString &sPrompt, 
                                   int options, // = OdEd::kInpDefault
                                   double defVal, // = 0.0
                                   const OdString &sKeywords, // = OdString::kEmpty
                                   OdEdRealTracker *pTracker) // = NULL
{
  OdqConsoleTab& consoleTab = *OdqConsole::getConsoleTab(baseDatabase(), true);
  SaveHintType svType(consoleTab.getCommandLineEdit(), enHintReal);
  getTopCmdState().setKeywordsAtPrompt(sPrompt, sKeywords);

  return ExDbCommandContext::getReal(sPrompt, options,
                                     defVal, sKeywords, pTracker);
}

OdSelectionSetPtr CommandContextImpl::box(const OdGePoint3d& firstCorner, 
                                          OdDbVisualSelection::SubentSelectionMode ssm)
{
  OdGePoint3d pts[2] = {
    firstCorner,
    dbUserIO()->getPoint(L"Specify opposite corner:", 
                         OdEd::kGptNoLimCheck | OdEd::kGptNoUCS | OdEd::kGptRectFrame | OdEd::kGptNoOSnap)
  };

  return select(2, pts, OdDbVisualSelection::kBox, ssm);
}

bool CommandContextImpl::try_lisp(const OdString& sInput)
{
  if (!OdDbDatabase::cast(baseDatabase()).get())
    return false;

  return ExDbCommandContext::try_lisp(sInput);
}

// Returns user input of a filename obtained in some manner.
// Arguments:
//    options (I) combination of OdEd::CommonInputOptions and OdEd::GetFilePathFlags enum values.
//    sPrompt (I) Prompt output before pausing for user input.
//    sDialogCaption (I) Dialog caption.
//    sDefExt (I) Default file extension without "."
//    sFileName (I) Initial filename to initialize dialog with.
//    sFilter (I) Filter pattern.
//    pTracker (I/O) File path tracker; may be used to display file preview.
// Remarks:
//    flags must be one of the following:
//        Name                Value   Description
//        OdEd::kGfpForOpen   0       For Open.
//        OdEd::kGfpForSave   1       For Save.

OdString CommandContextImpl::getFilePath(const OdString& sPrompt, 
                                         int options, // = OdEd::kGfpForOpen // or OdEd::kGfpForSave
                                         const OdString& csDialogCaption, // = OdString::kEmpty
                                         const OdString& sDefExt, // = OdString::kEmpty
                                         const OdString& sFileName, // = OdString::kEmpty
                                         const OdString& sFilter, // = OdString::kEmpty
                                         const OdString& sKeywords, // = OdString::kEmpty
                                         OdEdStringTracker* pTracker) // = NULL
{
  OdString sInput;
  bool bFILEDIA = true; // default

  OdDbBaseHostAppServices* pBaseAppServices = getIApp()->getBaseHostAppServices();
  if (!pBaseAppServices)
  {
    ODA_FAIL_ONCE();
    throw OdError(eNotApplicable); // sInput = OD_T("*unsupported*"); return sInput;
  }

  OdDbHostAppServices* pAppServices = OdDbHostAppServices::cast(pBaseAppServices);
  if (pAppServices)
    bFILEDIA = (pAppServices->getFILEDIA() != 0);

  if (!bFILEDIA || getIConsole()->isInputStringReady(baseDatabase()))
  {
    // TODO
    //OdEdPointTrackerPtr pPointTracker = getTopCmdState().m_pPointTracker;
    //if (!pPointTracker.isNull())
    //  pPointTracker->setCurrentKeywords(sKeywords); // save for context menu

    // from ExDbCommandContext::getFilePath :

    sInput = OdDbUserIO::getFilePath(sPrompt, options, csDialogCaption, sDefExt, 
                                     sFileName, sFilter, sKeywords, pTracker);
    if(sInput != OD_T("~"))
    {
      sInput.trimLeft().trimRight();
      if (sInput.findOneOf(L"/\\") < 0 && sFileName.findOneOf(L"/\\") >= 0)
      {
        // support for console input:
        // appload TD_RasterExport.tx bmpoutbg "" "" "" "" "" "" "out.png"
        // to place result at drawing
        OdString sPath = sFileName;
        sPath.replace(L'\\', L'/');
        sPath = sPath.left(sPath.reverseFind(L'/') + 1);
        sInput = sPath + sInput;
      }
      return sInput;
    }
  }

  OdString sDialogCaption(csDialogCaption);
  if (sDialogCaption.isEmpty())
    sDialogCaption = sPrompt; // usable for netload ...

  try
  {
    sInput = pBaseAppServices->fileDialog(options & 3, sDialogCaption, sDefExt, 
                                          sFileName, sFilter);
  }
  catch (const OdEdCancel&) // for netload ....
  {
  }
  if (sInput.isEmpty())
    throw OdEdCancel();

  OdRxObject* pRxDatabase = baseDatabase();
  OdqConsoleTab& tab = *OdqConsole::getConsoleTab(pRxDatabase);
  
  tab.getTopCmdState().setPrompt(sPrompt);
  OdString sFilePath = sInput;
  if (!(options & OdEd::kGfpForSave) || sDefExt.find(OD_T("|")) < 0) // see saveAs command
  {
    //int pos = -1;
    //if ((options & OdEd::kGfpForSave) != 0 && (pos = sInput.find(OD_T("|"))) > 0)
    //  sFilePath = sInput.left(pos);
    tab.putMsgText(addDoubleQuotes(sFilePath), TMsgType(enMsgTextPrompted | enMsgTextByInput));
  }

  tab.getTopCmdState().setPrompt(CommandState::getDefaultQPrompt());

  return sInput;
}

//OdDbUnitsFormatter& CommandContextImpl::formatter() // better use baseFormatter() instead it
//{
//  OdRxObject* pRxDatabase = baseDatabase();
//  OdExtDbModule* pExtDb = getIApp()->getExtDbModule(pRxDatabase);
//  if (pExtDb)
//  {
//    OdUnitsFormatter* pFormatter = pExtDb->getFormatter(pRxDatabase);
//    if (!pFormatter)
//    {
//      OdDbBaseDatabasePEPtr pPe = OdDbBaseDatabasePE::cast(pRxDatabase);
//      if (pPe.get())
//        pFormatter = pPe->baseFormatter(pRxDatabase);
//    }
//    if (pFormatter)
//    {
//      OdDbUnitsFormatter* pDbFormatter = OdDbUnitsFormatter::cast(pFormatter);
//      if (pDbFormatter)
//        return *pDbFormatter;
//    }
//    ODA_FAIL_ONCE();
//    throw OdError(eNoDatabase); // throw OdError(eNotApplicable);
//  }
//
//  return ExDbCommandContext::formatter();
//}

// ----- OdDbUserIO -----

static OdString GetNextValue(OdString &list, 
                             const OdChar *delim = (const OdChar *) L",")
{
  OdString sRes = list.spanExcluding(delim);
  if (sRes.getLength() != list.getLength())
    list = list.mid(sRes.getLength() + 1);
  else
    list.empty();
  return sRes;
}

static OdGsModelPtr getGsModel2d(OdRxObject* pRxDatabase)
{
  IqViewSystem* iVS = getIViewSystem();
  if (!iVS)
    return OdGsModelPtr();
  IqView* iView = iVS->getView(pRxDatabase);
  if (!iView)
    return OdGsModelPtr();

  return iView->getGsModel(true); //return OdGsModelPtr();
  // or
  //OdGsModelPtr pModel = iView->getGsDevice()->createModel();
  //pModel->setRenderType(OdGsModel::kDirect); // Skip Z-buffer for 2d drawables
  //return pModel;
}

OdGePoint3d CommandContextImpl::getPoint(const OdString& csPrompt,
                                         int options, // = OdEd::kGptDefault
                                         const OdGePoint3d* pDefVal, // = NULL
                                         const OdString& sKeywords, // = OdString::kEmpty
                                         OdEdPointTracker* pTracker) // = NULL
{
  //return ExDbCommandContext::getPoint(csPrompt, options,
  //                                    pDefVal,
  //                                    sKeywords, pTracker);

  OdString sPrompt(csPrompt);

  if (sPrompt.isEmpty())
  {
    ODA_FAIL_ONCE(); // test
    sPrompt = OD_T("Enter a point");
  }

  OdRxObject* pRxDatabase = baseDatabase();

  OdEdPointTrackerPtr pPredef;
  if (!pTracker)
  {
    if (GETBIT(options, OdEd::kGptRectFrame))
      pTracker = pPredef = createRectFrame(dbUserIO()->getLASTPOINT(), getGsModel2d(pRxDatabase));
    else if (GETBIT(options, OdEd::kGptRubberBand))
      pTracker = pPredef = createRubberBand(dbUserIO()->getLASTPOINT(), getGsModel2d(pRxDatabase));
  }

  OdqConsoleTab& consoleTab = *OdqConsole::getConsoleTab(pRxDatabase);
  SaveHintType svType(consoleTab.getCommandLineEdit(), enHintPoint);
  getTopCmdState().setKeywordsAtPrompt(sPrompt, sKeywords);

  SaveViewParams svParams(*this, options, pDefVal, pTracker);
  OdGePoint3d point;

  while (true)
  {
    OdString sOther;
    try
    {
      point = consoleTab.getCmdIO()->getPoint(sPrompt, options);

      OdString sPoint;
      sPoint.format(point.z == 0.0 ? OD_T("%g,%g") : OD_T("%g,%g,%g"), 
                    point.x, point.y, point.z);
      consoleTab.putMsgText(sPoint, TMsgType(enMsgTextPrompted | enMsgTextByInput)); 
    }
    catch (const OdEdEmptyInput& errEmpty)
    {
      consoleTab.putMsgText(OD_T(""), TMsgType(enMsgTextPrompted | enMsgTextByInput)); 

      if (!pDefVal) // if (GETBIT(options, OdEd::kInpThrowEmpty))
        throw errEmpty;
      ODA_FAIL_ONCE(); // test
      point = *pDefVal; //continue;
    }
    catch (const OdEdOtherInput& other)
    {
      sOther = other.string();
      consoleTab.putMsgText(sOther, TMsgType(enMsgTextPrompted | enMsgTextByInput)); 

      if (sOther.isEmpty())
      {
        if (GETBIT(options, OdEd::kInpThrowEmpty))
          throw OdEdEmptyInput();

        ODA_ASSERT_ONCE(pDefVal);
        if (!pDefVal)
          continue;
        point = *pDefVal; //continue;
      }
    }

    if (!sOther.isEmpty())
    {
      //if (!pPointTracker.isNull())
      //  pPointTracker->unlock();

      if (!sKeywords.isEmpty())
        KWIndex(sKeywords).check(sOther); // throw OdEdKeyword

      try
      {
        point = OdGePoint3d::kOrigin;
        if (pDefVal)
          point = *pDefVal;

        OdString sInput(sOther);
        int index = 0;
        if (   sInput.trimLeft().find(L'(') // skip for lisp
            && sInput.find(L',') > 0) // support throw other input for: zoom <scale>at ...
        {
          for (; index < 3 && !sInput.isEmpty(); ++index)
          {
            OdString sCoord = GetNextValue(sInput);
            point[index] = baseFormatter().unformatLinear(sCoord);
          }
        }
        if (index < 2)
        {
          if (GETBIT(options, OdEd::kInpThrowOther)) // support throw other input for: zoom <scale>at ...
            throw OdEdOtherInput(sOther); // throw other
          throw OdError(eInvalidInput);
        }
        if (pTracker)
        {
          pTracker->setValue(point);
          IqViewSystem* iVS = getIViewSystem();
          if (iVS)
            iVS->updateViews(pRxDatabase);
        }
      }
      catch (const OdEdOtherInput& error)
      {
        ODA_ASSERT_ONCE(GETBIT(options, OdEd::kInpThrowOther));
        throw error;
      }
      catch (const OdError& error)
      {
        OdResult code = error.code();
        if (code != eInvalidInput)
        {
          consoleTab.putMsgText(OD_T(""), TMsgType(enMsgTextPrompted | enMsgTextByInput)); 
          throw error;
        }

        if (GETBIT(options, OdEd::kInpThrowOther))
          throw OdEdOtherInput(sOther); // throw other;

        ODA_FAIL_ONCE(); // test
        putString(sKeywords.isEmpty() ? OD_T("Invalid point.") 
                                      : OD_T("Point or option keyword required."));
        continue;
      }
    }

    if (!GETBIT(options, OdEd::kGptNoLimCheck))
    {
      OdDbDatabasePtr pDwgDb = OdDbDatabase::cast(pRxDatabase);
      if (!pDwgDb.isNull() && pDwgDb->getLIMCHECK())
      {
        OdGeExtents2d ext(pDwgDb->getLIMMIN(), pDwgDb->getLIMMAX());
        if (!ext.contains((const OdGePoint2d &) point))
        {
          putString(OD_T("Error: Out of limits")); // "**Outside limits"
          continue;
        }
      }
    }

    break;

  } // end while(true)

  dbUserIO()->setLASTPOINT(point);

  //if (!pPointTracker.isNull())
  //  pPointTracker->unlock();

  return point;
}

double CommandContextImpl::getAngle(const OdString& sPrompt,
                                    int options, // = OdEd::kInpDefault
                                    double defaultValue, // = 0.0
                                    const OdString& sKeywords, // = OdString::kEmpty
                                    OdEdRealTracker* pTracker) // = NULL
{
  OdqConsoleTab& consoleTab = *OdqConsole::getConsoleTab(baseDatabase(), true);
  SaveHintType svType(consoleTab.getCommandLineEdit(), enHintAngle);
  getTopCmdState().setKeywordsAtPrompt(sPrompt, sKeywords);

  return ExDbCommandContext::getAngle(sPrompt, options,
                                      defaultValue,
                                      sKeywords, pTracker);
}

double CommandContextImpl::getDist(const OdString& csPrompt,
                                   int options, // = OdEd::kInpDefault
                                   double defVal, // = 0.0
                                   const OdString& sKeywords, // = OdString::kEmpty
                                   OdEdRealTracker* pTracker) // = NULL
{
  OdString sPrompt = csPrompt;
  if (sPrompt.isEmpty())
    sPrompt = OD_T("Enter a distance:");

  OdqConsoleTab& consoleTab = *OdqConsole::getConsoleTab(baseDatabase(), true);
  SaveHintType svType(consoleTab.getCommandLineEdit(), enHintDistance);
  getTopCmdState().setKeywordsAtPrompt(sPrompt, sKeywords);

  return ExDbCommandContext::getReal(sPrompt, options, defVal, 
                                     OdResBuf::kDxfXdDist, 
                                     sKeywords, pTracker);
}

OdSharedPtr<OdCmColorBase> CommandContextImpl::getCmColor(const OdString& sPrompt,
                                                          int options, // = OdEd::kInpDefault
                                                          const OdCmColorBase* pDefVal, // = NULL
                                                          const OdString& sKeywords, // = OdString::kEmpty
                                                          OdEdColorTracker* pTracker) // = NULL
{
  OdqConsoleTab& consoleTab = *OdqConsole::getConsoleTab(baseDatabase(), true);
  SaveHintType svType(consoleTab.getCommandLineEdit(), enHintColor);
  getTopCmdState().setKeywordsAtPrompt(sPrompt, sKeywords);

  return ExDbCommandContext::getCmColor(sPrompt, options,
                                        pDefVal,
                                        sKeywords, pTracker);
}

OdSelectionSetPtr CommandContextImpl::createSelectionSet()
{
  OdRxObject* pRxDatabase = baseDatabase();
  ODA_ASSERT_ONCE(pRxDatabase);
  if (!pRxDatabase)
    throw OdError(eNoDatabase);
 
  OdSelectionSetPtr pSSet;
  OdDbDatabase* pDwgDb = OdDbDatabase::cast(pRxDatabase).get();
  OdExtDbModule* pExtDb = NULL;
  if (!pDwgDb)
    pExtDb = getIApp()->getExtDbModule(pRxDatabase);
  if (!pExtDb)
    pSSet = OdDbSelectionSet::createObject(pDwgDb);
  else
    pSSet = OdExtDbModule_cast(pExtDb)->createSelectionSet(pRxDatabase);
  ODA_ASSERT_ONCE(   pSSet.get() 
                  || (pExtDb && pExtDb->baseModuleName(true) == L"bim")
                  || (pExtDb && pExtDb->baseModuleName(true) == L"prc")
               //PROD-139/PROD-114 //  || (pExtDb && pExtDb->baseModuleName(true) == L"ifc")
                  || (pExtDb && pExtDb->baseModuleName(true) == L"dwf")
                  || (pExtDb && pExtDb->baseModuleName(true) == L"vsf"));
  return pSSet;
}

OdSelectionSetPtr CommandContextImpl::select(int nPoints,
                                             const OdGePoint3d* wcsPts,
                                             OdDbVisualSelection::Mode mode, // = OdDbVisualSelection::kCrossing
                                             OdDbVisualSelection::SubentSelectionMode sm, // = OdDbVisualSelection::kDisableSubents
                                             const OdRxObject* pFilter) // = NULL
{
  OdRxObject* pRxDatabase = baseDatabase();
  ODA_ASSERT_ONCE(pRxDatabase);
  if (!pRxDatabase)
    throw OdError(eNoDatabase);

  OdSelectionSetPtr pSSet;
  OdDbDatabase* pDwgDb = OdDbDatabase::cast(pRxDatabase).get();
  OdExtDbModule* pExtDb = NULL;
  if (!pDwgDb)
    pExtDb = getIApp()->getExtDbModule(pRxDatabase);
  if (!pExtDb)
    pSSet = OdDbSelectionSet::select(OdDbDatabase::cast(pRxDatabase)->activeViewportId(), 
                                     nPoints, wcsPts, mode, sm, pFilter);
  else
  {
    OdGsView* gsView = NULL;
    if (getIViewSystem()) 
    {
      IqView* iView = getIViewSystem()->getView(pRxDatabase);
      gsView = iView->getActiveTopGsView(); // gsView = iView->getActiveGsView();
    }

    if (gsView)
      pSSet = pExtDb->select(gsView, nPoints, wcsPts, mode, sm, pFilter);
  }
  ODA_ASSERT_ONCE(!pSSet.isNull());
  //if (pSSet.isNull())
  //  throw OdError(eNotApplicable);
  return pSSet;
}

inline bool isOverallVp(const OdDbEntity* pObj)
{
  OdDbViewportPtr pVp = OdDbViewport::cast(pObj);
  if (pVp.get() && pVp->number() == 1)
    return true;
  return false;
}

void CommandContextImpl::highlight(OdSelectionSetIterator* pIter, 
                                   bool bDoIt) const // = true
{
  IqViewSystem* iVS = getIViewSystem();
  if (!pIter || !iVS)
    return;

  if (!pIter->subentCount())
  {
    iVS->highlightObject(bDoIt, pIter->id());
    return;
  }

  for (OdUInt32 se = 0; se < pIter->subentCount(); se++)
  {
    OdDbBaseFullSubentPath subEntPath;
    if (pIter->getSubentity(se, subEntPath))
      iVS->highlightObject(bDoIt, pIter->id(), &subEntPath);
  }
}

//void CommandContextImpl::highlight(OdSelectionSet* pSSet, 
//                                   bool bDoIt) const // = true
//{
//  if (!pSSet || !getIViewSystem())
//    return;
//
//  for (OdSelectionSetIteratorPtr pIter = pSSet->newIterator();
//       !pIter->done(); pIter->next())
//    highlight(pIter, bDoIt);
//}

//void CommandContextImpl::merge(OdSelectionSet* pRes,
//                               int options,
//                               const OdSelectionSet* pSSet,
//                               OdSSetTracker* pTracker,
//                               OdEdUserIO* pIO) const // = NULL
//{
//  ODA_ASSERT_ONCE(pRes);
//  if (!pSSet || !pSSet->baseDatabase() || !pRes)
//    return;
//
//  OdString s;
//  if (pIO)
//  {
//    s.format(OD_T("%d found"), pSSet->numEntities());
//    pIO->putString(s);
//  }
//  ODA_ASSERT_ONCE(pRes->baseDatabase() == pSSet->baseDatabase());
//  OdDbDatabase* pDwgDb = OdDbDatabase::cast(pRes->baseDatabase()).get();
//
//  int nOnLockedLayer = 0,
//      nNotInCurrentSpace = 0;
//  int nObjects = 0;
//  bool bPsVp = false;
//  OdSelectionSetIteratorPtr pIter,
//                            pResIter;
//
//  if (!isPickadd() && pRes->numEntities() && pSSet->numEntities())
//  {
//    highlight(pRes, false);
//    pRes->clear();
//  }
//
//  bool bAppending = !GETBIT(options, OdEd::kSelRemove);
//  OdDbObjectId BTRId;
//  if (pDwgDb)
//    BTRId = pDwgDb->getActiveLayoutBTRId();
//
//  for (pIter = pSSet->newIterator(); !pIter->done(); pIter->next())
//  {
//    OdDbStub* id = pIter->id();
//    ODA_ASSERT_ONCE(id && pRes->baseDatabase() == baseDatabaseBy(id));
//    bool bMember = pRes->isMember(id);
//
//    if (bMember && GETBIT(options, OdEd::kSelAllowSubents))
//    {
//      for (OdUInt32 se = 0; se < pIter->subentCount(); se++)
//      {
//        OdDbBaseFullSubentPath subEntPath;
//        pIter->getSubentity(se, subEntPath);
//        if (!pRes->isMember(subEntPath))
//        {
//          bMember = false;
//          break;
//        }
//      }
//    }
//    if (!(bMember ^ bAppending))
//      continue;
//
//    if (pDwgDb)
//    {
//      OdDbEntityPtr pEnt = OdDbEntity::cast(OdDbObjectId(id).openObject());
//      if (pEnt.get())
//      {
//        OdDbLayerTableRecordPtr pLayer = pEnt->layerId().openObject();
//        if (!GETBIT(options, OdEd::kSelAllowLocked) && pLayer.get() && pLayer->isLocked())
//        {
//          ++nOnLockedLayer;
//          continue;
//        }
//        else if (!GETBIT(options, OdEd::kSelAllowInactSpaces) && pEnt->ownerId() != BTRId)
//        {
//          ++nNotInCurrentSpace;
//          continue;
//        }
//        else if (!bPsVp && !GETBIT(options, OdEd::kSelAllowPSVP) && ::isOverallVp(pEnt))
//        {
//          bPsVp = true;
//          continue;
//        }
//      }
//      else if (!GETBIT(options, OdEd::kSelAllowObjects))
//      {
//        ++nObjects;
//        continue;
//      }
//    }
//
//    highlight(pIter, bAppending);
//    if (bAppending)
//    {
//      if (!pTracker || pTracker->append(id, pIter->method()))
//      {
//        pRes->append(id, pIter->method());
//        for (OdUInt32 se = 0; se < pIter->subentCount(); se++)
//        {
//          OdDbBaseFullSubentPath subEntPath;
//          if (pIter->getSubentity(se, subEntPath))
//            pRes->append(subEntPath, pIter->method());
//        }
//      }
//    }
//    else
//    {
//      if (!pTracker || pTracker->remove(id, pIter->method()))
//      {
//        if (GETBIT(options, OdEd::kSelAllowSubents))
//        {
//          for (pResIter = pRes->newIterator();
//               !pResIter->done(); pResIter->next())
//          {
//            if (pResIter->id() != id)
//              continue;
//              
//            if (pResIter->subentCount())
//            {
//              for (OdUInt32 se = 0; se < pIter->subentCount(); se++)
//              {
//                OdDbBaseFullSubentPath subEntPath;
//                if (pIter->getSubentity(se, subEntPath))
//                  pRes->remove(subEntPath);
//              }
//              if (!pResIter->subentCount())
//                pRes->remove(id);
//            }
//            break;
//          } // end for
//        }
//        else
//          pRes->remove(id);
//      }
//    }
//  } // end for
//  if (pIO && nObjects)
//  {
//    if (nObjects==1)
//      s = OD_T("1 was not an entity.");
//    else
//      s.format(OD_T("%d were not entities."), nObjects);
//    pIO->putString(s);
//  }
//  if (pIO && nNotInCurrentSpace)
//  {
//    if (nNotInCurrentSpace==1)
//      s = OD_T("1 was not in current space.");
//    else
//      s.format(OD_T("%d were not in current space."), nNotInCurrentSpace);
//    pIO->putString(s);
//  }
//  if (pIO && nOnLockedLayer)
//  {
//    if (nOnLockedLayer == 1)
//      s = L"1 was on a locked layer.";
//    else
//      s.format(L"%d were on a locked layer.", nOnLockedLayer);
//    pIO->putString(s);
//  }
//  if (pIO && bPsVp)
//  {
//    pIO->putString(OD_T("1 was the paper space viewport."));
//  }
//}

bool CommandContextImpl::isPickadd() const
{
  bool bPickadd = true;
  OdSysVarPEPtr pSysVarPE = OdSysVarPE::cast((OdEdCommandContext*) this);
  if (!pSysVarPE.isNull())
  {
    OdString sPickadd = pSysVarPE->valueToString((OdEdCommandContext*) this, "PICKADD");
    bPickadd = !(sPickadd.makeLower() == OD_T("false") || sPickadd == OD_T("0"));
  }
  return bPickadd;
}

//#include "DbEntity.h"
#include "Ge/GeLineSeg2d.h"
#include "Gi/GiDrawableImpl.h"
#include "Gi/GiWorldDraw.h"
#include "Gi/GiViewportDraw.h"
#include "Gi/GiUtils.h"
#include "OdDToStr.h"
#include "Gs/Gs.h"
#include "DbViewport.h"
#include "DbLayerTableRecord.h"
#include "DbBlockTableRecord.h"
//#include "Ed/EdLispEngine.h"

class PolylineSelectionTracker : public OdEdPointDefTracker, public OdGiDrawableImpl<>
{
protected:
  ODRX_USING_HEAP_OPERATORS(OdEdPointDefTracker);
  const OdGePoint3dArray *m_pts;
  OdGePoint3d m_pt;
  //PolylineSelectionTracker() : m_pts(NULL) { }
  void setValue(const OdGePoint3d &pt) { m_pt = pt; }
  void setValue(const OdGePoint3dArray *pts)
  {
    m_pts = pts;
  }
  OdUInt32 subSetAttributes(OdGiDrawableTraits* ) const ODRX_OVERRIDE
  {
    return kDrawableIsAnEntity;
  }
  bool subWorldDraw(OdGiWorldDraw* pWd) const ODRX_OVERRIDE
  {
    if (m_pts && m_pts->size() > 1)
    {
      pWd->geometry().polyline((OdInt32)m_pts->size(), m_pts->asArrayPtr());
    }
    if (m_pts && m_pts->size() > 0)
    {
      OdGePoint3d tailPts[2] =
      {
        m_pts->last(),
        m_pt
      };
      pWd->geometry().polyline(2, tailPts);
    }
    return true;
  }
  OdGePoint3d basePoint() const
  {
    if (m_pts && !m_pts->empty())
      return m_pts->first();
    return OdGePoint3d::kOrigin;
  }
public:
  static OdEdPointTrackerPtr create(const OdGePoint3dArray *pts = NULL)
  {
    OdEdPointTrackerPtr pRes = OdRxObjectImpl<PolylineSelectionTracker, OdEdPointTracker>::createObject();
    static_cast<PolylineSelectionTracker*>(pRes.get())->setValue(pts);
    return pRes;
  }

  int addDrawables(OdGsView* pView)
  {
    pView->add(this, 0);
    return 1;
  }
  void removeDrawables(OdGsView* pView)
  {
    pView->erase(this);
  }
};

class PolygonSelectionTracker : public PolylineSelectionTracker
{
protected:
  bool subWorldDraw(OdGiWorldDraw* pWd) const
  {
    PolylineSelectionTracker::subWorldDraw(pWd);
    // Polygon segments must not intersect. This check must be implemented in selection command.
    if (m_pts && m_pts->size() > 1)
    {
      OdGePoint3d closePts[2] =
      {
        m_pt,
        m_pts->first()
      };
      pWd->geometry().polyline(2, closePts);
    }
    return true;
  }
public:
  static OdEdPointTrackerPtr create(const OdGePoint3dArray *pts = NULL)
  {
    OdEdPointTrackerPtr pRes = OdRxObjectImpl<PolygonSelectionTracker, OdEdPointTracker>::createObject();
    static_cast<PolygonSelectionTracker*>(pRes.get())->setValue(pts);
    return pRes;
  }
  static bool doesPolySegmentsNotIntersects(const OdGePoint3dArray &segments, const OdGePoint3d &newPoint)
  {
    if (segments.size() < 3)
      return true;
    OdGePoint3dArray poly = segments;
    //poly.push_back(newPoint);
    OdGeVector3d zAxis = ::odgiFaceNormal(poly.size(), poly.asArrayPtr());
    if (OdZero(zAxis.length()))
      return true; // All segments are parallel
    poly.push_back(newPoint);
    OdUInt32 nSeg;
    OdGeVector3d xAxis;
    for (nSeg = 0; nSeg < poly.size() - 1; nSeg++)
    {
      xAxis = segments[nSeg + 1] - segments[nSeg];
      OdGe::ErrorCondition error;
      xAxis.normalize(OdGeContext::gZeroTol, error);
      if (error == OdGe::kOk)
        break;
    }
    OdGeVector3d yAxis = zAxis.crossProduct(xAxis).normal();
    OdGePoint2dArray poly2d;
    poly2d.resize(poly.size());
    for (nSeg = 0; nSeg < poly.size(); nSeg++)
      poly2d[nSeg].set(poly[nSeg].asVector().dotProduct(xAxis), poly[nSeg].asVector().dotProduct(yAxis));
    // 1-st segment with all without last
    for (nSeg = 0; nSeg < poly2d.size() - 3; nSeg++)
    {
      OdGePoint2d rVal;
      if (OdGeLineSeg2d(poly2d[nSeg], poly2d[nSeg + 1]).intersectWith(OdGeLineSeg2d(poly2d[poly2d.size() - 2], poly2d.last()), rVal))
        return false;
    }
    // 2-nd segment with all without first
    for (nSeg = 1; nSeg < poly2d.size() - 2; nSeg++)
    {
      OdGePoint2d rVal;
      if (OdGeLineSeg2d(poly2d[nSeg], poly2d[nSeg + 1]).intersectWith(OdGeLineSeg2d(poly2d.first(), poly2d.last()), rVal))
        return false;
    }
    return true;
  }
};

class ExSelectionMethods
{
protected:
  // Returns true if entity is on visible layer
  static bool isEntityVisible(OdDbEntityPtr pEntity)
  {
    if (!pEntity.isNull() && pEntity->visibility() == OdDb::kVisible)
    {
      OdDbLayerTableRecordPtr pLayer = OdDbLayerTableRecord::cast(pEntity->layerId().openObject(OdDb::kForRead));
      if (!pLayer->isFrozen() && !pLayer->isOff())
        return true;
    }
    return false;
  }
  // Return object Id of current selection block
  static OdDbObjectId selectionBlockId(const OdDbObjectId& vpId)
  {
    OdDbDatabase* pDb = vpId.database();
    OdDbViewportPtr pVp = OdDbViewport::cast(vpId.safeOpenObject());
    if (!pVp.isNull())
      return (pVp->number() == 1) ? pDb->getPaperSpaceId() : pDb->getModelSpaceId();
    return pDb->getModelSpaceId();
  }
public:
  static void selectLast(OdRxObject* pRxDatabase, OdDbStubPtrArray& ids)
  {
    OdExtDbModule* pExtDb = getIApp()->getExtDbModule(pRxDatabase);
    if (pExtDb)
    {
      OdDbStub* id = OdExtDbModule_cast(pExtDb)->getVisibleLastId(pRxDatabase);
      if (id)
        ids.push_back(id);
      return;
    }
    OdDbDatabase* pDwgDb = OdDbDatabase::cast(pRxDatabase).get();

    OdDbObjectId blockId = selectionBlockId(pDwgDb->activeViewportId());
    OdDbBlockTableRecordPtr pBlockRec = OdDbBlockTableRecord::cast(blockId.openObject(OdDb::kForRead));
    OdDbObjectIteratorPtr pIt = pBlockRec->newIterator(false);
    while (!pIt->done())
    {
      OdDbEntityPtr pEntity = pIt->entity(OdDb::kForRead);
      if (isEntityVisible(pEntity))
      {
        ids.push_back(pEntity->id());
        break;
      }
      pIt->step(false);
    }
  }
  static void selectAll(OdRxObject* pRxDatabase, OdDbStubPtrArray& ids)
  {
    OdExtDbModule* pExtDb = getIApp()->getExtDbModule(pRxDatabase);
    if (pExtDb)
    {
      pExtDb->getVisibleAllIds(pRxDatabase, ids);
      return;
    }
    OdDbDatabase* pDwgDb = OdDbDatabase::cast(pRxDatabase).get();

    OdDbObjectId blockId = selectionBlockId(pDwgDb->activeViewportId());
    OdDbBlockTableRecordPtr pBlockRec = OdDbBlockTableRecord::cast(blockId.openObject(OdDb::kForRead));
    OdDbObjectIteratorPtr pIt = pBlockRec->newIterator();
    while (!pIt->done())
    {
      OdDbEntityPtr pEntity = pIt->entity(OdDb::kForRead);
      if (isEntityVisible(pEntity))
      {
        ids.push_back(pEntity->id());
      }
      pIt->step();
    }
  }
  static void selectRect(OdSmartPtr<OdEdBaseIO> pIoStream, OdDbUserIO* pIO, OdDbVisualSelection::SubentSelectionMode &ssm, OdGePoint3dArray &pts)
  {
    pts.resize(2);
    pts[0] = pIO->getPoint(L"Specify first corner:", 
                           OdEd::kGptNoLimCheck | OdEd::kGptNoUCS | OdEd::kGptNoOSnap);
    if ((pIoStream->getKeyState() & OdEdBaseIO::kControlIsDown) == 0)
      ssm = OdDbVisualSelection::kDisableSubents;
    pts[1] = pIO->getPoint(L"Specify opposite corner:",
                           OdEd::kGptNoLimCheck | OdEd::kGptNoUCS | OdEd::kGptRectFrame | OdEd::kGptNoOSnap);
  }
  static void selectPolyline(OdSmartPtr<OdEdBaseIO> pIoStream, OdDbUserIO* pIO, OdDbVisualSelection::SubentSelectionMode &ssm, OdGePoint3dArray &pts)
  {
    pts.clear();
    OdEdPointTrackerPtr pTracker = PolylineSelectionTracker::create(&pts);
    for (;;)
    {
      if (pts.size() == 0)
      {
        pts.resize(1);
        pts[0] = pIO->getPoint(L"Specify first fence point:",
                               OdEd::kGptNoLimCheck | OdEd::kGptNoUCS | OdEd::kGptNoOSnap);
      }
      if ((pIoStream->getKeyState() & OdEdBaseIO::kControlIsDown) == 0)
        ssm = OdDbVisualSelection::kDisableSubents;
      try
      {
        pts.append(pIO->getPoint(L"Specify next fence point:",
                                 OdEd::kGptNoLimCheck | OdEd::kGptNoUCS //|OdEd::kGptRubberBand|*/
                                 | OdEd::kInpThrowEmpty | OdEd::kGptNoOSnap, // | OdEd::kInpThrowOther
                                 NULL, L"Undo", pTracker));
      }
      catch(const OdEdKeyword& kw)
      {
        if (kw.keywordIndex() == 0)
          pts.removeLast();
      }
      catch (const OdEdEmptyInput)
      {
        break;
      }
    }
  }
  static void selectPolygon(OdSmartPtr<OdEdBaseIO> pIoStream, OdDbUserIO* pIO, OdDbVisualSelection::SubentSelectionMode &ssm, OdGePoint3dArray &pts)
  {
    pts.clear();
    OdEdPointTrackerPtr pTracker = PolygonSelectionTracker::create(&pts);
    for (;;)
    {
      if (pts.size() == 0)
      {
        pts.resize(1);
        pts[0] = pIO->getPoint(L"First polygon point:",
                                OdEd::kGptNoLimCheck | OdEd::kGptNoUCS | OdEd::kGptNoOSnap);
      }
      if ((pIoStream->getKeyState() & OdEdBaseIO::kControlIsDown) == 0)
        ssm = OdDbVisualSelection::kDisableSubents;
      try
      {
        OdGePoint3d newPoint = pIO->getPoint(L"Specify endpoint of line:",
                                               OdEd::kGptNoLimCheck | OdEd::kGptNoUCS
                                             | OdEd::kInpThrowEmpty | OdEd::kGptNoOSnap,
                                             NULL, L"Undo", pTracker);
        if (PolygonSelectionTracker::doesPolySegmentsNotIntersects(pts, newPoint))
        {
          pts.append(newPoint);
        }
        else
        {
          pIO->putString(OD_T("Invalid point, polygon segments cannot intersect."));
        }
      }
      catch(const OdEdKeyword& kw)
      {
        if (kw.keywordIndex() == 0)
          pts.removeLast();
      }
      catch (const OdEdEmptyInput)
      {
        break;
      }
    }
  }
};

// Launches interactive object selection to fill selection set.
// Each subsequent call to this function modifies current selection set.
// Returned value is guaranteed to be not null.
OdSelectionSetPtr CommandContextImpl::select(const OdString& sPrompt, // = OdString::kEmpty
                                             int options, // = OdEd::kSelDefault
                                             const OdSelectionSet* pDefVal, // = NULL
                                             const OdString& sKeywords, // = OdString::kEmpty
                                             OdSSetTracker* pTracker, // = NULL
                                             OdGePoint3dArray*) // ptsPointer = NULL
{
  // way for DWG only
  //return ExDbCommandContext::select(sPrompt, options,
  //                                  pDefVal,
  //                                  sKeywords, pTracker);

  OdDbDatabase* pDwgDb = OdDbDatabase::cast(baseDatabase()).get();

  bool bEcho = isEchoOn(enMsgTextPromptedByInput); //(enMsgTextPrompted);
  OdDbUserIO* pIO = dbUserIO();
  OdSelectionSetPtr pResSet = createSelectionSet();
  OdString sMsg;

  OdSelectionSetPtr pSSet = pickfirst();
  setPickfirst(NULL);
  OdSelectionSetIteratorPtr pIter;

  // already done in OdqCmd_Select::execute
  //if (getKeyState() & OdEdBaseIO::kShiftIsDown)
  //  options |= OdEd::kSelRemove;
  //if (getKeyState() & OdEdBaseIO::kControlIsDown)
  //  options |= OdEd::kSelAllowSubents;

  if (pSSet.get() && pSSet->numEntities() && !pDefVal && !GETBIT(options, OdEd::kSelIgnorePickFirst))
  {
    merge(pResSet, 
          (options | OdEd::kSelRemove) & ~OdEd::kSelRemove, 
          pSSet, 
          pTracker, 
          bEcho ? pIO : NULL);
    return pResSet;
  }
  pSSet.release();

  OdString prompt(sPrompt);
  if(prompt.isEmpty())
    prompt = OD_T("Select objects:");

  OdString keywords(sKeywords);
  if(keywords.isEmpty())
    keywords = OD_T("Window Last Crossing BOX ALL Fence WPolygon CPolygon Group Add Remove Multiple Previous Undo Auto Single");

  if (pDefVal)
    merge(pResSet, 
          (options | OdEd::kSelRemove) & ~OdEd::kSelRemove, 
          pDefVal, 
          pTracker, 
          NULL);

  OdGePoint3dArray pts;
  bool bRepeat;
  do
  {
    bRepeat = false; //selection_loop_start:
    OdDbVisualSelection::SubentSelectionMode ssm = GETBIT(options, OdEd::kSelAllowSubents)? OdDbVisualSelection::kEnableSubents : OdDbVisualSelection::kDisableSubents;
    int nKword = -1;
    pts.resize(1);
    try
    {
      try
      {
        if (GETBIT(options, OdEd::kSelPickLastPoint))
        {
          SETBIT(options, OdEd::kSelPickLastPoint, false);
          pts[0] = pIO->getLASTPOINT();
        }
        else
        {
          pts[0] = pIO->getPoint(prompt, 
                                   OdEd::kGptNoUCS | OdEd::kGptNoLimCheck | OdEd::kInpThrowEmpty
                                 | OdEd::kInpThrowOther | OdEd::kGptNoOSnap,
                                 NULL, keywords);
        }
        if ((m_pIoStream->getKeyState() & OdEdBaseIO::kControlIsDown) == 0)
          ssm = OdDbVisualSelection::kDisableSubents;

        pSSet = select(1, &pts[0], OdDbVisualSelection::kPoint, ssm);

        CommandState& stateCmd = getTopCmdState();

        if (!pSSet->numEntities())
        {
          if (!GETBIT(options, OdEd::kSelSingleEntity))
          {
            IqView* iView = NULL;
            if (getIViewSystem())
              iView = getIViewSystem()->getView(baseDatabase());

            bool bActiveViewChanged = false;
            if (   GETBIT(options, OdEd::kSelLeaveHighlighted)
                && !bEcho
                && pDwgDb
                && !stateCmd.m_qsLastCommandName.toLower().indexOf("select")
                && stateCmd.m_ptGetPoint == pts[0])
            {
              // Simplest way to get eyes point. This point cannot be correctly transformed from pts[0].
              // (see also slot OdqConsoleTab::slotMouseButtonPress)
              OdGePoint2d ptEyes(stateCmd.m_ptGetEyes.x(), stateCmd.m_ptGetEyes.y());

              if (iView)
              {
                OdGsView* pActiveGsView = iView->getActiveGsView();
                OdGsLayoutHelperPtr pGsLayoutHelper = OdGsLayoutHelper::cast(iView->getGsDevice());
                bActiveViewChanged = false;
                if (!pGsLayoutHelper.isNull())
                {
                  OdGsPaperLayoutHelper* pPsHelper = OdGsPaperLayoutHelper::cast(pGsLayoutHelper).get();
                  int numViews = 0;
                  if (pPsHelper)
                    numViews = pPsHelper->numViews();

                  if (!pPsHelper || numViews < 2)
                  {
                    bActiveViewChanged = pGsLayoutHelper->setActiveViewport(ptEyes);
                    // TODO //ODA_ASSERT_ONCE(bActiveViewChanged == (pActiveGsView != iView->getActiveGsView()));
                  }
                  else
                  {
                    //bActiveViewChanged = pGsLayoutHelper->setActiveViewport(ptEyes);
                    //
                    // Current implementation of setActiveViewport takes 
                    // the latest viewport if it contains the point.
                    // Some of viewports can contain other and better take
                    // the minimal one.

                    OdGsView* pActiveNew = NULL;   
                    for (int index = 0; index < numViews; index++)
                    {
                      OdGsView* pGsView = pPsHelper->viewAt(index);
                      if (!pGsView->isVisible())
                        continue;

                      OdGsClientViewInfo infoView;
                      pGsView->clientViewInfo(infoView);
                      if (GETBIT(infoView.viewportFlags, OdGsClientViewInfo::kDependentGeometry))
                        continue;

                      if (!pGsView->pointInViewport(ptEyes))
                        continue;

                      if (pActiveNew)
                      {
                        // compare squares
                        double sqActiveNew = pActiveNew->fieldWidth() * pActiveNew->fieldHeight(),
                               sqGsView = pGsView->fieldWidth() * pGsView->fieldHeight();
                        if (sqGsView >= sqActiveNew)
                          continue;
                      }

                      pActiveNew = pGsView;
                    } // end for
                    if (!pActiveNew)
                      pActiveNew = pPsHelper->overallView().get();

                    if (pActiveNew && pActiveNew != pActiveGsView)
                    {
                      pPsHelper->makeViewActive(pActiveNew);
                      bActiveViewChanged = true;
                    }
                    ODA_ASSERT_ONCE(bActiveViewChanged == (pActiveGsView != iView->getActiveGsView()));
                  }
                }
                bActiveViewChanged = (pActiveGsView != iView->getActiveGsView());

                if (bActiveViewChanged)
                {
                  // @@@ probably move this code to GsLayoutHelper's?
                  OdGsClientViewInfo infoView;
                  iView->getActiveGsView()->clientViewInfo(infoView);
                  OdDbObjectId idActiveVp(infoView.viewportObjectId);
                  if (pDwgDb->getTILEMODE())
                    OdDbViewportTable::cast(pDwgDb->getViewportTableId().safeOpenObject(OdDb::kForWrite))
                      ->SetActiveViewport(idActiveVp);
                  else
                    OdDbLayout::cast(OdDbBlockTableRecord::cast(pDwgDb->getPaperSpaceId().safeOpenObject())
                      ->getLayoutId().safeOpenObject(OdDb::kForWrite))->setActiveViewportId(idActiveVp);
                  iView->setViewportBorderProperties();
                  iView->updateGL();
                  getIApp()->refreshQObjetcts("<event-refresh:dwg_cvport_changed>");
                }
              }
            }
            if (!bActiveViewChanged)
            {
              if (!iView->isFullScreenMode() || !iView->isFullScreenActive())
              {
                pSSet = box(pts[0], ssm);
                iView->updateGL(); // clear select-rectangle in active viewe
              }
            }
          }
        }
        else
        {
          ODA_ASSERT_ONCE(pSSet->numEntities() == 1);
          bool bAlreadySelected = (   pSSet->numEntities() == 1
                                   && pResSet->isMember(pSSet->newIterator()->id()));
          if (   bAlreadySelected
              && GETBIT(options, OdEd::kSelLeaveHighlighted)
              && !GETBIT(options, OdEd::kSelSingleEntity)
              && !bEcho
              && pts.size() == 1
              && pDwgDb // (pDwgDb || getIAppProps()->toBool(OD_T("Vars/dgngrips"), false))
              && !stateCmd.m_qsLastCommandName.toLower().indexOf("select"))
          {
            OdGePoint3d pt = pts[0]; // = toEyeToWorld(x, y);

            OdSaveState<OdRxObject*> saveDragging(m_pRxDatabaseDragSource, baseDatabase());
            OdqDataSource::beginDrag(pResSet, pt);
          }
        }
      }
      catch (const OdEdOtherInput& sInp)
      {
        // #9077
        if (try_lisp(sInp.string()))
        {
          pSSet = createSelectionSet();
          for (OdResBufPtr pRb(result()); pRb.get(); pRb = pRb->next())
            pSSet->append(pRb->getObjectId(pDwgDb));
        }
        else
          throw;
      }
      catch (const OdEdCancel&)
      {
        break; // from do-while cycle
      }
    }
    catch (const OdEdKeyword& kw)
    {
      nKword = kw.keywordIndex();
    }
    catch (const OdEdEmptyInput&)
    {
      if (pResSet->numEntities() || GETBIT(options, OdEd::kSelAllowEmpty))
        break;

      if (GETBIT(options, OdEd::kInpThrowEmpty))
        throw;

      continue;
    }
    catch (const OdEdOtherInput&)
    {
      if (GETBIT(options, OdEd::kInpThrowOther))
        throw;

      // *Invalid selection*
      // Expects a point or
      // Window/Last/Crossing/BOX/ALL/Fence/WPolygon/CPolygon/Group/Add/Remove/Multiple
      // /Previous/Undo/AUto/SIngle/SUbobject/Object
      OdString sMsg = keywords;
      sMsg.replace(__OD_T(' '), __OD_T('/'));
      sMsg = OD_T("*Invalid selection*\nExpects a point or\n") + sMsg;
      pIO->putString(sMsg);

      continue;
    }

    bool bNYI = false;
    switch(nKword)
    {
    case -1: // no op
      break;
    case  0: // Window
      ExSelectionMethods::selectRect(m_pIoStream, pIO, ssm, pts);
      pSSet = select(2, pts.getPtr(), OdDbVisualSelection::kWindow, ssm);
      break;
    case  1: // Last
      {
        pSSet = createSelectionSet();
        if (pDwgDb)
        {
          OdDbStubPtrArray ids;
          ExSelectionMethods::selectLast(baseDatabase(), ids);
          for (unsigned index = 0; index < ids.size(); index++)
            pSSet->append(ids[index]);
        }
      }
      break;
    case  2: // Crossing
      ExSelectionMethods::selectRect(m_pIoStream, pIO, ssm, pts);
      pSSet = select(2, pts.getPtr(), OdDbVisualSelection::kCrossing, ssm);
      break;
    case  3: // BOX
      pts[0] = pIO->getPoint(L"Specify first corner:",
                             OdEd::kGptNoLimCheck | OdEd::kGptNoUCS | OdEd::kGptNoOSnap);
      if ((m_pIoStream->getKeyState() & OdEdBaseIO::kControlIsDown) == 0)
        ssm = OdDbVisualSelection::kDisableSubents;
      pSSet = box(pts[0], ssm);
      break;
    case  4: // ALL
      {
        OdDbStubPtrArray ids;
        pSSet = createSelectionSet();
        ExSelectionMethods::selectAll(baseDatabase(), ids);
        for (unsigned index = 0; index < ids.size(); index++)
          pSSet->append(ids[index]);
      }
      break;
    case  5: // Fence
      ExSelectionMethods::selectPolyline(m_pIoStream, pIO, ssm, pts);
      if (pts.size() > 1)
        pSSet = select((int)pts.size(), pts.getPtr(), OdDbVisualSelection::kFence, ssm);
      break;
    case  6: // WPolygon
      ExSelectionMethods::selectPolygon(m_pIoStream, pIO, ssm, pts);
      if (pts.size() > 2)
        pSSet = select((int)pts.size(), pts.getPtr(), OdDbVisualSelection::kWPoly, ssm);
      break;
    case  7: // CPolygon
      ExSelectionMethods::selectPolygon(m_pIoStream, pIO, ssm, pts);
      if (pts.size() > 2)
        pSSet = select((int)pts.size(), pts.getPtr(), OdDbVisualSelection::kCPoly, ssm);
      break;
    case  8: // Group
      bNYI = true;
      break;
    case  9: // Add
      SETBIT(options, OdEd::kSelRemove, false);
      break;
    case 10: // Remove
      SETBIT(options, OdEd::kSelRemove, true);
      break;
    case 11: // Multiple
      SETBIT(options, OdEd::kSelSinglePass, false);
      break;
    case 12: // Previous
      pSSet = previousSelection();
      break;
    case 13: // Undo
      bNYI = true;
      break;
    case 14: // AUto
      SETBIT(options, OdEd::kSelRemove, false); // Same as 'Add'
      SETBIT(options, OdEd::kSelSinglePass, false); // Same as 'Multiple'
      break;
    case 15: // SIngle
      SETBIT(options, OdEd::kSelSinglePass, true);
      // goto selection_loop_start;
      bRepeat = true;
      continue;
    }
    if (!pSSet.isNull())
      merge(pResSet, 
            options, 
            pSSet, 
            pTracker, 
            bEcho ? pIO : NULL);
    else if (bNYI)
      pIO->putString(OD_T("Not Yet Implemented"));
  }
  while (bRepeat || !GETBIT(options, OdEd::kSelSinglePass));

  if (!GETBIT(options, OdEd::kSelLeaveHighlighted))
    highlight(pResSet, false);
  setPreviousSelection(pResSet);
  return pResSet;
}

// moved to OdDbUnitsFormatterImpl :
//// implementations of next 2 methods are swapped mistakenly in OdDbUnitsFormatterImpl
//// (compare it with getQVar_VIEWCTR for example)
//
//OdGePoint3d CommandContextImpl::toUCS(const OdGePoint3d& wcsPt) const
//{
//  OdDbDatabase* pDatabase = OdDbDatabase::cast(const_cast<CommandContextImpl*>(this)->baseDatabase()).get();
//  ODA_ASSERT_ONCE(pDatabase); // test
//  if (!pDatabase)
//    return wcsPt;
//
//  OdGeMatrix3d xToUCS;
//  xToUCS.setCoordSystem(pDatabase->getUCSORG(), pDatabase->getUCSXDIR(), pDatabase->getUCSYDIR(),
//                        pDatabase->getUCSXDIR().crossProduct(pDatabase->getUCSYDIR())).invert();
//  return xToUCS * wcsPt;
//}
//
//OdGePoint3d CommandContextImpl::fromUCS(const OdGePoint3d& ucsPt) const
//{
//  OdDbDatabase* pDatabase = OdDbDatabase::cast(const_cast<CommandContextImpl*>(this)->baseDatabase()).get();
//  ODA_ASSERT_ONCE(pDatabase); // test
//  if (!pDatabase)
//    return ucsPt;
//
//  OdGeMatrix3d xFromUCS;
//  xFromUCS.setCoordSystem(pDatabase->getUCSORG(), pDatabase->getUCSXDIR(), pDatabase->getUCSYDIR(),
//                          pDatabase->getUCSXDIR().crossProduct(pDatabase->getUCSYDIR()));
//  return xFromUCS * ucsPt;
//}
