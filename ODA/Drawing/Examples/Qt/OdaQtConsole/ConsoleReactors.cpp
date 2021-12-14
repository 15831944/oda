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
// ConsoleReactors.cpp
//

#include "ConsoleReactors.h"
#include "OdqConsole.h"
#include "ConsoleModule.h"
#include "ExtDbModule.h"
#include "DbHostAppServices.h"
#include "DbAssocManager.h"
#include "SysVarPE.h"
#include "TDVersion.h"

#include <QFileInfo>
#include <QDir>

//////////////////////////////////////////////////////////////////////////

OdqCommandReactor::OdqCommandReactor()
{
}

OdqCommandReactor::~OdqCommandReactor()
{
}

// Creates an instance
//static
OdEdCommandStackReactorPtr OdqCommandReactor::createObject()
{
  OdEdCommandStackReactorPtr pReactor;
  pReactor.attach(new OdqCommandReactor()); // without addRef
  return pReactor;
}

// ----- support to get command list -----

void OdqCommandReactor::commandAdded(OdEdCommand* pCommand)
{
  ODA_ASSERT_ONCE(pCommand);

  QString qsCmdName = getIApp()->toQString(pCommand->globalName()).toLower(),
          qsGroupNameToAdd = getIApp()->toQString(pCommand->groupName()),
          qsGroupName = qsGroupNameToAdd,
          qsGroupNameToRemove;
  ODA_ASSERT_ONCE(!qsCmdName.isEmpty());
  //ODA_ASSERT_ONCE(m_mapCmds.end() == m_mapCmds.find(qsCmdName)); // test to redefine

  QString qsSearchedGroupName = QString("%1%2%1").arg(cqchCmdGroupDelim).arg(qsGroupNameToAdd),
          qsPrevGroupName = m_mapCmds[qsCmdName];
  int pos = qsPrevGroupName.indexOf(qsSearchedGroupName);
  ODA_ASSERT_ONCE(pos < 0);
  if (!pos)
    return;
  if (pos > 0)
    qsPrevGroupName.replace(qsSearchedGroupName, QString(cqchCmdGroupDelim));

  if (!qsPrevGroupName.isEmpty())
  {
    qsGroupNameToRemove = qsPrevGroupName.split(cqchCmdGroupDelim, QString::SkipEmptyParts).first();
    ODA_ASSERT_ONCE(!qsGroupNameToRemove.isEmpty());
    qsGroupName = QString("%1%2%3").arg(cqchCmdGroupDelim).arg(qsGroupName).arg(qsPrevGroupName);
  }
  else
    qsGroupName = qsSearchedGroupName;
  m_mapCmds[qsCmdName] = qsGroupName;

  if (!qsGroupNameToRemove.isEmpty())
    getIApp()->refreshQObjetcts("<event-refresh:command_removed>", 
                                QString("%1%2%3").arg(qsGroupNameToRemove)
                                                 .arg(cqchCmdGroupDelim)
                                                 .arg(qsCmdName));
  if (!qsGroupNameToAdd.isEmpty())
    getIApp()->refreshQObjetcts("<event-refresh:command_added>", 
                                QString("%1%2%3").arg(qsGroupNameToAdd)
                                                 .arg(cqchCmdGroupDelim)
                                                 .arg(qsCmdName));
}

void OdqCommandReactor::commandWillBeRemoved(OdEdCommand* pCommand)
{
  ODA_ASSERT_ONCE(pCommand);

  QString qsCmdName = getIApp()->toQString(pCommand->globalName()).toLower(),
          qsGroupNameToRemove = getIApp()->toQString(pCommand->groupName()); 
  ODA_ASSERT_ONCE(!qsCmdName.isEmpty());

  TCmdMap::iterator itr = m_mapCmds.find(qsCmdName);
  if (itr == m_mapCmds.end())
  {
    // TODO it is possible now with removing of previously overloadded command
    // like erase, stretch ...
    ODA_ASSERT_ONCE(   qsCmdName.toLower() == "setvar"
                  //|| qsCmdName.toLower() == "stretch"
                    || qsCmdName.toLower() == "explode"
                    || qsCmdName.toLower() == "erase"); 
    return; // command was not found
  }

  QString qsSearchedGroupName = QString("%1%2%1").arg(cqchCmdGroupDelim).arg(qsGroupNameToRemove),
          qsPrevGroupName = m_mapCmds[qsCmdName],
          qsGroupName = qsPrevGroupName,
          qsGroupNameToAdd;
  int pos = qsPrevGroupName.indexOf(qsSearchedGroupName);
  if (pos < 0)
  {
    ODA_FAIL_ONCE();
    return;
  }
  if (pos > 0)
  {
    qsGroupName.replace(qsSearchedGroupName, QString(cqchCmdGroupDelim));
    m_mapCmds[qsCmdName] = qsGroupName; // opendbfunc of TD_DwfDb & TD_IfcDb
    return;
  }
  ODA_ASSERT_ONCE(!pos);
  qsGroupName.replace(qsSearchedGroupName, QString(cqchCmdGroupDelim));
  if (qsGroupName.size() <= 1)
    qsGroupName.clear();

  if (!qsGroupName.isEmpty())
  {
    qsGroupNameToAdd = qsGroupName.split(cqchCmdGroupDelim, QString::SkipEmptyParts).first();
    ODA_ASSERT_ONCE(!qsGroupNameToAdd.isEmpty());
    m_mapCmds[qsCmdName] = qsGroupName;
  }
  else
    m_mapCmds.erase(itr);

  if (!qsGroupNameToRemove.isEmpty())
    getIApp()->refreshQObjetcts("<event-refresh:command_removed>", 
                                QString("%1%2%3").arg(qsGroupNameToRemove)
                                                 .arg(cqchCmdGroupDelim)
                                                 .arg(qsCmdName));
  if (!qsGroupNameToAdd.isEmpty())
    getIApp()->refreshQObjetcts("<event-refresh:command_added>", 
                                QString("%1%2%3").arg(qsGroupNameToAdd)
                                                 .arg(cqchCmdGroupDelim)
                                                 .arg(qsCmdName));
}

// ----- support command diagnostic in console -----

void OdqCommandReactor::commandWillStart(OdEdCommand* pCommand, OdEdCommandContext* pCmdCtx)
{
  OdDbBaseDatabase* pRxDatabase = pCmdCtx->baseDatabase();

  OdqConsoleTab* pTab = OdqConsole::getConsoleTab(pRxDatabase);
  if (!pTab)
    return;
  m_pTestLast = pTab;
  CommandState& state = pTab->getTopCmdState();
  state.putCmdStringEcho(pCmdCtx, pCommand);
  state.m_qsLastCommandName = pCommand->globalName();

  if (!pRxDatabase)
    return;

  state.m_flagsCmd = pCommand->flags();
  if (GETBIT(pCommand->flags(), OdEdCommand::kNoUndoMarker))
    return;

  if (pTab->m_levelUndoBlock)
  {
    pTab->m_levelUndoBlock++;
    return;
  }
  OdDbDatabasePtr pDatabase = OdDbDatabase::cast(pRxDatabase);
  if (pDatabase.isNull())
  {
    OdExtDbModule* pExtDb = getIApp()->getExtDbModule(pRxDatabase);
    if (!pExtDb)
      return;
    if (pExtDb->startUndoRecord(pRxDatabase))
    {
      pTab->m_levelUndoBlock = 1;
      pExtDb->blockUndoRecording(pRxDatabase, true); // Begin
    }
    return;
  }
  pTab->m_levelUndoBlock = 1;

  //if (pDatabase->isUndoBlockStarted())
  //{
  //  ODA_FAIL_ONCE(); // test
  //  pDatabase->blockUndoRecording(false);
  //}
  pDatabase->startUndoRecord();
  pDatabase->blockUndoRecording(true); // Begin
  ODA_ASSERT_ONCE(pDatabase->undoFiler());
  
  // Mark
  //pDatabase->startUndoRecord();
  //pDatabase->setUndoMark();

  //pDatabase->blockUndoRecording(true); // Begin
}

void OdqCommandReactor::commandEnded(OdEdCommand* pCommand, OdEdCommandContext* pCmdCtx)
{
  if (m_pTestLast.isNull())
    return;

  OdDbBaseDatabase* pRxDatabase = pCmdCtx->baseDatabase();
  OdqConsoleTab* pTab = OdqConsole::getConsoleTab(pRxDatabase);
  if (!pTab)
    return;
  m_pTestLast = pTab;
  CommandState& state = pTab->getTopCmdState();
  state.setPrompt(CommandState::getDefaultPrompt());  // "Command"
  state.m_flagsCmd = 0;

  if (GETBIT(pCommand->flags(), OdEdCommand::kNoUndoMarker))
    return;

  if (!pRxDatabase || !pTab->m_levelUndoBlock)
    return;
  if (--(pTab->m_levelUndoBlock) > 0)
    return;
  ODA_ASSERT_ONCE(!pTab->m_levelUndoBlock);

  OdDbDatabasePtr pDatabase = OdDbDatabase::cast(pRxDatabase);
  if (pDatabase.isNull())
  {
    OdExtDbModule* pExtDb = getIApp()->getExtDbModule(pRxDatabase);
    ODA_ASSERT_ONCE(pExtDb);
    if (!pExtDb)
      return;
    ODA_ASSERT_ONCE(pExtDb->isUndoBlockStarted(pRxDatabase));

    pExtDb->blockUndoRecording(pRxDatabase, false); // End
    ODA_ASSERT_ONCE(pExtDb->hasUndo(pRxDatabase));
    getIApp()->refreshQObjetcts("<event-refresh:undo_state_changed>");
    return;
  }

  ODA_ASSERT_ONCE(pDatabase->isUndoBlockStarted());

  if (::odrxDynamicLinker()->getModule(DbConstraintsModuleName).get())
    OdDbAssocManager::evaluateTopLevelNetwork(pDatabase);

  pDatabase->blockUndoRecording(false); // End
  ODA_ASSERT_ONCE(pDatabase->hasUndo());
  getIApp()->refreshQObjetcts("<event-refresh:undo_state_changed>");
}

void OdqCommandReactor::commandCancelled(OdEdCommand* pCommand, OdEdCommandContext* pCmdCtx)
{
  commandFailed(pCommand, pCmdCtx);
}

void OdqCommandReactor::commandFailed(OdEdCommand* pCommand, OdEdCommandContext* pCmdCtx)
{
  if (m_pTestLast.isNull())
    return;

  OdDbBaseDatabase* pRxDatabase = pCmdCtx->baseDatabase();
  OdqConsoleTab* pTab = OdqConsole::getConsoleTab(pRxDatabase);
  if (!pTab)
    return;
  m_pTestLast = pTab;
  CommandState& state = pTab->getTopCmdState();
  state.setPrompt(CommandState::getDefaultPrompt());  // "Command"
  state.m_flagsCmd = 0;

  if (GETBIT(pCommand->flags(), OdEdCommand::kNoUndoMarker))
    return;

  if (!pRxDatabase || !pTab->m_levelUndoBlock)
    return;
  if (--(pTab->m_levelUndoBlock) > 0)
    return;
  ODA_ASSERT_ONCE(!pTab->m_levelUndoBlock);

  OdDbDatabasePtr pDatabase = OdDbDatabase::cast(pRxDatabase);
  if (pDatabase.isNull())
  {
    OdExtDbModule* pExtDb = getIApp()->getExtDbModule(pRxDatabase);

    ODA_ASSERT_ONCE(pExtDb->isUndoBlockStarted(pRxDatabase)); // test
    if (!pExtDb->isUndoBlockStarted(pRxDatabase))
      return;

    pExtDb->blockUndoRecording(pRxDatabase, false); // End
    ODA_ASSERT_ONCE(pExtDb->hasUndo(pRxDatabase));
    getIApp()->refreshQObjetcts("<event-refresh:undo_state_changed>");
    return;
  }

  ODA_ASSERT_ONCE(pDatabase->isUndoBlockStarted()); // test
  if (!pDatabase->isUndoBlockStarted())
    return;
  
  pDatabase->blockUndoRecording(false); // End
  ODA_ASSERT_ONCE(pDatabase->hasUndo());
  getIApp()->refreshQObjetcts("<event-refresh:undo_state_changed>");
}

OdEdCommandPtr OdqCommandReactor::unknownCommand(const OdString&, // csCommandName
                                                 OdEdCommandContext*) // pCmdCtx
{
  return OdEdCommandPtr();
}

const TCmdMap& OdqCommandReactor::getCommandMap() const
{
  return m_mapCmds;
}

//////////////////////////////////////////////////////////////////////////

static OdString s_sLastVar;
static OdString s_sLastWildcard;

void OdqCmd_SetVarOvr::execute(OdEdCommandContext* pCmdCtx)
{
  ODA_ASSERT_ONCE(module());  // or via ::odrxDynamicLinker()->loadModule(OD_T("OdaQtConsole.dll")).get()
  static_cast<OdqConsoleModule*>(const_cast<OdRxModule*>(module()))->m_cmd_SetVar.execute(pCmdCtx);
}

void OdqCmd_SetVar::execute(OdEdCommandContext* pCmdCtx)
{
  OdEdUserIO* pIO = pCmdCtx->userIO();
  OdString sInput = s_sLastVar.isEmpty() ? OD_T("?") : s_sLastVar.c_str();
  OdString sPrompt;
  sPrompt.format(OD_T("Enter variable name or [%ls]:"), sInput.c_str());// pmtEnterVariableNameOr
  try
  {
    sInput = pIO->getString(sPrompt, OdEd::kInpDefault, sInput, sInput);
  }
  catch (const OdEdKeyword& kw)
  {
    sInput = kw.keyword();
  }

  if (sInput.isEmpty())
    return;

  OdSysVarPEPtr pSysVarPE = OdSysVarPE::cast(pCmdCtx);
  if (pSysVarPE.isNull())
    throw OdError(eNotApplicable);

  if(sInput == OD_T("?"))
  {
    s_sLastVar = sInput;

    sInput = s_sLastWildcard.isEmpty() ? OD_T("*") : s_sLastWildcard.c_str();
    sPrompt.format(OD_T("Enter variable(s) to list <%ls>:"), sInput.c_str()); // pmtEnterVariablesToList
    try
    {
      sInput = pIO->getString(sPrompt, OdEd::kInpDefault, sInput, sInput);
    }
    catch (const OdEdKeyword& kw)
    {
      sInput = kw.keyword();
    }

    OdStringArray names;
    if (sInput.isEmpty() || !pSysVarPE->getNames(pCmdCtx, names, 
                                                 !getIAppProps()->toBool(OD_T("Vars/showhiddenprops"), false)))
      return;
    
    s_sLastWildcard = sInput;
    QRegExp rx(getIApp()->toQString(sInput), Qt::CaseInsensitive, QRegExp::Wildcard);
    ODA_ASSERT_ONCE(rx.isValid());
    OdStringArray::size_type sz = names.size();
    for (OdStringArray::size_type index = 0; index < sz; index++)
    {
      OdString sName = names[index];
      if (!rx.exactMatch(getIApp()->toQString(sName)))
        continue;

      OdSysVarType typeRb = kRtNone;
      OdString sValue = pSysVarPE->valueToString(pCmdCtx, sName, &typeRb);
      if (typeRb == kRtString)
        sValue = L"\"" + sValue + L"\"";

      OdString sMsg;
      sMsg.format(OD_T("  %-24ls%ls"), sName.c_str(), sValue.c_str());
      if (!pSysVarPE->isEditable(pCmdCtx, sName))
      {
        int numSpaces = 40 - sValue.getLength(); // 32 - sValue.getLength();
        if (numSpaces < 1)
          numSpaces = 1;
        sMsg += OdString(__OD_T(' '), numSpaces);
        sMsg += getIApp()->toOdString(QObject::tr("(read only)"));
      }
      pIO->putString(sMsg);
    }
    return;
  }

  OdString sVarName = sInput;
  bool bExist = false;
  pSysVarPE->editValue(pCmdCtx, sVarName, &bExist);

  // possible TODO fix it via reactors in OdGsModelLayoutHelperImpl (see #9094)
  //#if TD_MAJOR_VERSION <= 4 && TD_MINOR_VERSION <= 5
  if (OdString(sVarName).makeLower().find(OD_T("cvport")) >= 0)
  {
    // fix for direct changes via CVPORT variable
    // (OdGsModelLayoutHelperImpl::syncWithVT will be call too late in this case)
    IqView* iView = NULL;
    OdGsDevice* pGsDevice = NULL;
    if (   getIViewSystem() 
      && (iView = getIViewSystem()->getView(pCmdCtx->baseDatabase()))
      && (pGsDevice = iView->getGsDevice()))
    {
      pGsDevice->update();
      iView->setViewportBorderProperties();
      iView->updateGL();
    }
  }
  //#endif

  if (bExist)
    s_sLastVar = sVarName;
  else
    pIO->putString(OD_T("Unknown variable name. Type SETVAR ? for a list of variables.")); // msgUnknownVariableName
}

//////////////////////////////////////////////////////////////////////////
// instance of this reactor should be added the last

OdqCommandLastReactor::OdqCommandLastReactor()
{
}

OdqCommandLastReactor::~OdqCommandLastReactor()
{
}

// get single instance
//static
OdEdCommandStackReactorPtr OdqCommandLastReactor::getObject() // single instance // instead createObject()
{
  static OdEdCommandStackReactorPtr s_pCmdLastReactor;
  if (s_pCmdLastReactor.isNull())
    s_pCmdLastReactor.attach(new OdqCommandLastReactor()); // without addRef
  return s_pCmdLastReactor;
}

OdEdCommandPtr OdqCommandLastReactor::unknownCommand(const OdString& sCommandName, 
                                                     OdEdCommandContext* pCmdCtx)
{
  ODA_ASSERT_ONCE(pCmdCtx && !sCommandName.isEmpty());

  OdRxObject* pRxDb = pCmdCtx->baseDatabase();
  OdqConsoleTab* pTab = OdqConsole::getConsoleTab(pRxDb);
  if (!pTab)
    return OdEdCommandPtr();
  CommandState& state = pTab->getTopCmdState();
  state.putCmdStringEcho(pCmdCtx);

  OdSysVarPEPtr pSysVarPE = OdSysVarPE::cast(pCmdCtx);
  if (pSysVarPE.get())
  {
    OdString sVarName = sCommandName;
    bool bExist = false;
    bool isEditable = pSysVarPE->isEditable(pCmdCtx, sVarName, &bExist);
    if (bExist)
    {
      state.m_qsLastCommandName = sCommandName;

      if (isEditable)
      {
        pSysVarPE->editValue(pCmdCtx, sVarName, &bExist);
        ODA_ASSERT_ONCE(bExist);
      
       // possible TODO fix it via reactors in OdGsModelLayoutHelperImpl (see #9094)
       //#if TD_MAJOR_VERSION <= 4 && TD_MINOR_VERSION <= 5
        OdString sVarNameLow(sVarName);
        sVarNameLow.makeLower();
        if (sVarNameLow.find(OD_T("cvport")) >= 0)
        {
          // fix for direct changes via CVPORT variable
          // (OdGsModelLayoutHelperImpl::syncWithVT will be call too late in this case)
          IqView* iView = NULL;
          OdGsDevice* pGsDevice = NULL;
          if (   getIViewSystem() 
              && (iView = getIViewSystem()->getView(pCmdCtx->baseDatabase()))
              && (pGsDevice = iView->getGsDevice()))
          {
            pGsDevice->update();
            iView->setViewportBorderProperties();
            iView->updateGL();
          }
        }
       //#endif
        else if (sVarNameLow.find(L"osmode") >= 0)
        {
          OdString sValue = pSysVarPE->valueToString(pCmdCtx, sVarName);
          OdDbHostAppServices* pHostAppServ = OdDbHostAppServices::cast(getIApp()->getBaseHostAppServices()).get();
          IqAppProps* iAppProps = getIAppProps();
          OdInt16 nSnapModes = 0;
          if (sValue != iAppProps->toString(L"Vars/osmode"))
            iAppProps->set(L"Vars/osmode", sValue, L"int");
          else if (   pHostAppServ 
                   && (nSnapModes= (OdInt16) iAppProps->toInt(L"Vars/osmode")) != pHostAppServ->getOSMODE())
            pHostAppServ->setOSMODE(nSnapModes);

          getIApp()->refreshQObjetcts("<event-refresh:appvar_changed>", 
                                      getIApp()->toQString(sVarNameLow));
        }
        else if (sVarNameLow.find(L"cannoscale") >= 0)
        {
          pTab->executeCommand(L"regenall", false);
          // next event is not used now
          //getIApp()->refreshQObjetcts("<event-refresh:sysvar_changed>", 
          //                            getIApp()->toQString(sVarNameLow));
        }
        else if (sVarNameLow.find(L"perspective") >= 0)
        {
          if (IqView* iView = getIViewSystem()->getView(pRxDb))
            iView->updateLayouts();
        }
        if (sVarNameLow.find(OD_T("largetoolbars")) >= 0)
          getIApp()->refreshQObjetcts("<event-refresh:toolbarsize_changed>");
      }
      else
      {
        OdSysVarType typeRb = kRtNone;
        OdString sValue = pSysVarPE->valueToString(pCmdCtx, sVarName, &typeRb);
        if (typeRb == kRtString)
          sValue = L"\"" + sValue + L"\"";
        OdString sMsg;
        sMsg.format(OD_T("%ls = %ls (read only)"), sVarName.makeUpper().c_str(), sValue.c_str());
        pCmdCtx->userIO()->putString(sMsg);
      }

      s_sLastVar = sVarName;
      return OdEdCommandPtr();
    }
  }

  OdString sMessage = getIApp()->toOdString(QObject::tr("Unknown command \"%1\".")
                                                .arg(getIApp()->toQString(sCommandName)));
  pCmdCtx->userIO()->putString(sMessage);
  return OdEdCommandPtr();
}

//////////////////////////////////////////////////////////////////////////
// TODO to get module names and ? command dictionaty : any, dwg, dgn

OdqDynLinkerReactor::OdqDynLinkerReactor()
  : m_sModuleName(OD_T("OdqConsoleModule"))
{
}

OdqDynLinkerReactor::~OdqDynLinkerReactor()
{
}

// Creates an instance
// static
OdRxDLinkerReactorPtr OdqDynLinkerReactor::createObject()
{
  OdRxDLinkerReactorPtr pReactor;
  pReactor.attach(new OdqDynLinkerReactor()); // without addRef
  return pReactor;
}

// TODO : update m_sModuleName via next overloads

//void OdqDynLinkerReactor::rxAppWillBeLoaded(const OdString &sDrxModulePathName)
//{
//}

void OdqDynLinkerReactor::rxAppLoaded(OdRxModule *pModule)
{
  // instance of OdqCommandLastReactor should be added the last 
  // to prevent message about unknown command
  // via entering of system variable name in console
  // see unknownCommand of class VarNameToSetVarCmdReactor

  static OdString sDbCommands;
  if (sDbCommands.isEmpty())
  {
    // now it's enough to fix placement after DbCommand loading
    sDbCommands = OdDbCommandsModuleName;
   #ifdef _MSC_VER
    sDbCommands += TD_DLL_VERSION_SUFFIX_STR;
   #endif
    sDbCommands += DEFAULT_MODULE_EXTENSION_DOT_W;
  }

  OdString sModuleName = pModule->moduleName();
  ODA_ASSERT_ONCE(!sModuleName.isEmpty());
  if (sDbCommands.isEmpty() || sModuleName.find(sDbCommands) >= 0)
  {
    // restore reactor as last
    OdEdCommandStackReactorPtr pCmdLastReactor = OdqCommandLastReactor::getObject();
    ::odedRegCmds()->removeReactor(pCmdLastReactor);
    ::odedRegCmds()->addReactor(pCmdLastReactor);
  }

  QFileInfo info(getIApp()->toQString(sModuleName));
  //if (info.suffix().toLower() != "tx")
  //  return;

  QString qsCuiFileName = info.completeBaseName(),
          qsCuiFilePathName;
#if defined(TD_DLL_VERSION_SUFFIX_STR)
  qsCuiFileName.remove(getIApp()->toQString(TD_DLL_VERSION_SUFFIX_STR));
#endif
  qsCuiFileName += ".cui";

  QString qsCuiPaths = getIApp()->toQString(getIAppProps()->toString(OD_T("Vars/tdcuipaths")));
  if (!qsCuiPaths.isEmpty())
  {
    foreach (QString qsPath, qsCuiPaths.split(';', QString::SkipEmptyParts))
    {
      if ((qsPath = qsPath.trimmed()).isEmpty())
        continue;
      QDir dir(qsPath);
      if (!dir.exists() || !dir.exists(qsCuiFileName))
      {
        ODA_ASSERT_ONCE(dir.exists()); // non-existed path cui path // test
        continue;
      }
      
      qsCuiFilePathName = QDir::toNativeSeparators(dir.absoluteFilePath(qsCuiFileName));
      break;
    }
  }
  if (qsCuiFilePathName.isEmpty())
  {
    info = QFileInfo(QString(":/cui/") + qsCuiFileName);
    if (info.exists())
      qsCuiFilePathName = QString("qrc:/cui/") + qsCuiFileName;
  }

  if (!qsCuiFilePathName.isEmpty())
  {
    QString qsCmd = QString("cuiload \"%1\"").arg(qsCuiFilePathName);
    getIConsole()->postponeCommand(qsCmd, 
                                   true, // echo
                                   NULL, 100);
  }
}

void OdqDynLinkerReactor::rxAppUnloaded(const OdString& sModuleName)
{
  ODA_ASSERT_ONCE(!sModuleName.isEmpty());
//  QFileInfo info(getIApp()->toQString(sModuleName));
//  //if (info.suffix().toLower() != "tx")
//  //  return;
//  QString qsCuiFileName = info.completeBaseName();
//#if defined(TD_DLL_VERSION_SUFFIX_STR)
//  qsCuiFileName.remove(getIApp()->toQString(TD_DLL_VERSION_SUFFIX_STR));
//#endif
//  qsCuiFileName += ".cui";

  QString qsGroupName = getIApp()->getLoadedCuiCustomGroupNameByFileName(getIApp()->toQString(sModuleName)); // qsCuiFileName
  if (qsGroupName.isEmpty())
    return;

  QString qsCmd = QString("cuiunload %1").arg(qsGroupName);
  getIConsole()->postponeCommand(qsCmd, 
                                 true, // echo
                                 NULL, 100);
}

