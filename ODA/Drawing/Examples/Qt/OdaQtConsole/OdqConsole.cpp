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
// OdqConsole.cpp
//
#include <QtCore>
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtGui>
#else
#include <QtWidgets>
#endif
#include <QTabWidget>

#include "OdqConsole.h" // #include "ConsoleTab.h"

#include "DbDiesel.h"
#include "SysVarPE.h"
#include "TDVersion.h"
#include "summinfo.h"
#include "ExDbCommandContext.h"

//#define OD_ANDROID_TRACEDEFS

#ifdef OD_ANDROID_TRACEDEFS
#ifdef ANDROID
#include <android/log.h>
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,  "OdqConsole", __VA_ARGS__)
#else
#undef OD_ANDROID_TRACEDEFS
#endif
#endif

//////////////////////////////////////////////////////////////////////////

OdqConsole::OdqConsole(const TCmdMap& mapCmds)
  : m_mapCmds(mapCmds)
{
  QSettings settings("ODA", 
                     QFileInfo(QCoreApplication::applicationFilePath()).baseName());
  settings.beginGroup(TD_SHORT_STRING_VER_S);

  QMainWindow* pMainWindow = getIApp()->getMainWindow();

  // create console palette

  m_pDockPalette = new QDockWidget(tr("Console"), pMainWindow);
  m_pDockPalette->setAttribute(Qt::WA_DeleteOnClose, false);
  m_pDockPalette->setObjectName("Console");
  m_pDockPalette->setAllowedAreas(Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea); // (Qt::AllDockWidgetAreas)

  //QPixmap pixmap("Palette_console.png");
  //ODA_ASSERT_ONCE(!pixmap.isNull());
  //pixmap.setMask(pixmap.createHeuristicMask());
  //m_pDockPalette->setWindowIcon(QIcon(pixmap));

  QTabWidget::TabPosition posTab = (QTabWidget::TabPosition) settings.value(
                                                                "console/tab_pos",
                                                                QTabWidget::North).toInt();
  m_pTabWidget = new QTabWidget(m_pDockPalette);
  m_pTabWidget->setTabPosition(posTab); // North South [ West East ]
  if (!QObject::connect(m_pTabWidget, SIGNAL(currentChanged(int)),
                        this, SLOT(currentTabChanged(int))))
  {
    ODA_FAIL_ONCE();
  }

  if (!QObject::connect(this, SIGNAL(postponeCommandsAvailable()), 
                        SLOT(executePostponedCommands()), Qt::QueuedConnection))
  {
    ODA_FAIL_ONCE();
  }

  m_pDockPalette->setWidget(m_pTabWidget);

  Qt::DockWidgetArea area = (Qt::DockWidgetArea) settings.value(
                                                    "console/area",
                                                    Qt::BottomDockWidgetArea).toInt();
  pMainWindow->addDockWidget(area, m_pDockPalette);

  // get protected tabbar and set visibility
  QList<QTabBar*> widgets = m_pDockPalette->widget()->findChildren<QTabBar*>();
  if (widgets.count() == 1)
    widgets.last()->setVisible(settings.value("console/tab_vis", 0) != 0);
  else
    ODA_FAIL();

  m_pDockPalette->setFeatures(  QDockWidget::DockWidgetMovable
                              | QDockWidget::DockWidgetClosable
                              | QDockWidget::DockWidgetFloatable); 

  //pMainWindow->layout()->activate();

  int h = settings.value("h", 580).toInt();
  int height = settings.value("console/h", h / 3).toInt();

  //int width = settings.value("console/w", m_pDockPalette->geometry().width()).toInt();
  //QPoint pos = m_pDockPalette->pos();
  //pos.setX(settings.value("console/x", pos.x()).toInt());
  //pos.setY(settings.value("console/y", pos.y()).toInt());
  //m_pDockPalette->move(pos);
  //int minWidth = pMainWindow->minimumWidth(),
  //    maxWidth = pMainWindow->maximumWidth();
  //m_pDockPalette->setFixedWidth(width);

  m_pDockPalette->setFixedHeight(height);

  pMainWindow->layout()->activate();

  //m_pDockPalette->setMinimumWidth(minWidth);
  //m_pDockPalette->setMaximumWidth(maxWidth);
  m_pDockPalette->setMinimumHeight(h / 10);
  m_pDockPalette->setMaximumHeight(h * 9 / 10);

#if defined(__IPHONE_OS_VERSION_MIN_REQUIRED) || defined(ANDROID)
  //Hide Console to show file on FullScreen
  m_pDockPalette->setVisible(false);
#else
  m_pDockPalette->setVisible(settings.value("console/vis", 1) != 0);
#endif
  settings.endGroup();
}

OdqConsole::~OdqConsole()
{
  if (m_pDockPalette.isNull())
  {
    ODA_ASSERT_ONCE(m_pTabWidget.isNull());
    return;
  }
  ODA_ASSERT_ONCE(!m_pTabWidget.isNull());

  QWidget *pCurrTab = NULL;
  while ((pCurrTab = m_pTabWidget->currentWidget()) != NULL)
  {
    OdRxObjectPtr pRxDatabase = ((OdqConsoleTab *) pCurrTab)->getRxDatabase();
    removeConsoleTab(pRxDatabase);
  }

  m_pDockPalette = NULL;
}

void OdqConsole::saveSettings()
{
  if (m_pDockPalette.isNull() || m_pTabWidget.isNull())
  {
    //via help(/?) of command line switches //ODA_FAIL_ONCE();
    ODA_ASSERT_ONCE(qApp->arguments().contains("/?") || qApp->arguments().contains("-?"));
    return;
  }

  // save palette size and placement via QSettings
  QSettings settings("ODA", 
                     QFileInfo(QCoreApplication::applicationFilePath()).baseName());
  settings.beginGroup(TD_SHORT_STRING_VER_S);

  QPoint pos = m_pDockPalette->pos();
  settings.setValue("console/x", pos.x());
  settings.setValue("console/y", pos.x());

  QRect rect = m_pDockPalette->geometry();
  settings.setValue("console/w", rect.width());
  settings.setValue("console/h", rect.height());

  QMainWindow* pMainWindow = qobject_cast<QMainWindow*>(m_pDockPalette->parent());
  ODA_ASSERT_ONCE(pMainWindow);

  settings.setValue("console/area", pMainWindow->dockWidgetArea(m_pDockPalette));
  settings.setValue("console/vis", m_pDockPalette->isVisible() ? 1 : 0);

  settings.setValue("console/tab_pos", m_pTabWidget->tabPosition());

  QList<QTabBar*> widgets = OdqConsole::getDockPalette()->widget()->findChildren<QTabBar*>();
  if (widgets.count() == 1)
    settings.setValue("console/tab_vis", widgets.last()->isVisible() ? 1 : 0);
  else
    ODA_FAIL();

  settings.endGroup();

  OdqCommandLineEdit::saveSettings();
}

//static
OdqConsole& OdqConsole::getConsole()
{
  static QPointer<OdqConsole> s_pConsole;
  if (s_pConsole.isNull())
    s_pConsole = qobject_cast<OdqConsole*>(getIApp()->getSingle(IqConsole_iid));
  ODA_ASSERT_ONCE(!s_pConsole.isNull()); // test

  return *s_pConsole;
}

//static 
QDockWidget* OdqConsole::getDockPalette() 
{
  return getConsole().m_pDockPalette.get(); 
}

// ----- IConsole -----

OdVariant OdqConsole::executeCommand(const QString &qsCmd,
                                     bool bEcho, // = true // less priority than echo... commands
                                     OdRxObject* pRxDatabase) // = NULL current
{
  if (!pRxDatabase)
    pRxDatabase = getCurrentDatabase();

  OdqConsoleTab& tabConsole = *getConsoleTab(pRxDatabase, true);
  return tabConsole.executeCommand(qsCmd, bEcho);
}

static bool s_bBusy = false;
static CmdInfo s_infoCurrExececuted;

void OdqConsole::postponeCommand(const QString &qsCmd, 
                                 bool bEcho, // = true
                                 OdRxObject* pRxDatabase, // = NULL (use ((OdRxObject*) -1) to avoid setCurrentDatabase at execution moment)
                                 int timestep) // = 0
{
  if (!pRxDatabase)
    pRxDatabase = getCurrentDatabase();

  CmdInfo infoCmd(qsCmd, bEcho, pRxDatabase);
  if (s_bBusy && s_infoCurrExececuted == infoCmd) // fix problem with recursive quit
    return;
  QString qsCmdLow = qsCmd.toLower();
  if (   qsCmdLow == "quit" || qsCmdLow == "close"
      || qsCmdLow.endsWith(" quit") || qsCmdLow.endsWith(" close")) 
    m_cmdsPostponed.clear(); // special to close application of drawing

  m_cmdsPostponed.enqueue(infoCmd);

  if (timestep > 0)
    QTimer::singleShot(timestep, this, SLOT(executePostponedCommands()));
  else
    emit postponeCommandsAvailable();
}

QString OdqConsole::findPostponedCommand(const QString &qsCmdPref, 
                                         OdRxObject* pRxDatabase) // = NULL
{
  ODA_ASSERT_ONCE(!qsCmdPref.isEmpty());

  if (!pRxDatabase)
    pRxDatabase = getCurrentDatabase();

  foreach (CmdInfo info, m_cmdsPostponed)
  {
    if (   (pRxDatabase == ((OdRxObject*) -1) || info.getRxDatabase() == ((OdRxObject*) -1) || pRxDatabase == info.getRxDatabase())
        && !info.getCommand().indexOf(qsCmdPref))
      return info.getCommand();
  }

  return QString(); // was not found
}

void OdqConsole::executePostponedCommands()
{
  if (s_bBusy)
    return;
  s_bBusy = true;

  while (!m_cmdsPostponed.isEmpty())
  {
    s_infoCurrExececuted = m_cmdsPostponed.dequeue();
    OdRxObject* pRxDatabase = s_infoCurrExececuted.getRxDatabase();
    if (pRxDatabase == ((OdRxObject*) -1)) // marked to avoid setCurrentDatabase at execution moment
      pRxDatabase = NULL;
    if (pRxDatabase && !setCurrentDatabase(pRxDatabase))
    {
      ODA_ASSERT_VAR(QString qsCmd = s_infoCurrExececuted.getCommand();)
      ODA_ASSERT_ONCE(qsCmd == "quit" || qsCmd.indexOf("zoom") >= 0);
      continue; // already closed
    }

    executeCommand(s_infoCurrExececuted.getCommand(), s_infoCurrExececuted.getEcho(), pRxDatabase);
  }

  s_bBusy = false;
}

OdVariant OdqConsole::executeCommandScript(const QString& qsScriptFileName, // [.scr] for current database
                                           bool* pbWasFound) // = NULL
{
  return executeCommandScript(qsScriptFileName, getCurrentDatabase(), pbWasFound);
}

OdVariant OdqConsole::executeCommandScript(const QString& qsScriptFileName, // [.scr]
                                           OdRxObject* pRxDatabase, // = NULL current
                                           bool* pbWasFound) // = NULL
{
  OdqConsoleTab& tabConsole = *getConsoleTab(pRxDatabase, true);
  return tabConsole.executeCommandScript(qsScriptFileName, pbWasFound);
}

void OdqConsole::putString(const QString& qsMsg,
                           OdRxObject* pRxDatabase) // = NULL current
{
  if (qsMsg.isEmpty())
    return;
  putString(getIApp()->toOdString(qsMsg), pRxDatabase);
}

void OdqConsole::putString(const OdString& sMsg,
                           OdRxObject* pRxDatabase) // = NULL current
{
  if (sMsg.isEmpty())
    return;

  if (!pRxDatabase)
    pRxDatabase = getCurrentDatabase();

  OdqConsoleTab& tabConsole = *getConsoleTab(pRxDatabase);
  tabConsole.getCmdContext().dbUserIO()->putString(sMsg);
}

void OdqConsole::putTrace(const OdString& sMsg,
                          OdRxObject* pRxDatabase) // = NULL current
{
  if (sMsg.isEmpty())
    return;

  if (!pRxDatabase)
    pRxDatabase = getCurrentDatabase();

  OdqConsoleTab& tabConsole = *getConsoleTab(pRxDatabase);
  tabConsole.traceBuf() += sMsg;
}

bool OdqConsole::isInputStringReady() const // current database
{
  return isInputStringReady(getCurrentDatabase());
}

bool OdqConsole::isInputStringReady(OdRxObject* pRxDatabase) const
{
  OdqConsoleTab* pTab = getConsoleTab(pRxDatabase);
  ODA_ASSERT_ONCE(pTab);
  if (!pTab)
    return false;

  return !pTab->getTopCmdState().getInputString().isEmpty();
}

bool OdqConsole::isInputStringReady(OdEdCommandContext* pCmdCtx) const
{
  ODA_ASSERT_ONCE(pCmdCtx);
  OdRxObject* pRxDatabase = pCmdCtx->baseDatabase();

  ExDbCommandContext* pExCtx = (ExDbCommandContext*) pCmdCtx;
  ExDbCommandContext* pExCtxEmb = (ExDbCommandContext*) &getConsoleTab(pRxDatabase ? pRxDatabase 
                                                                                    : getCurrentDatabase())->getCmdContext();
  if (pExCtx != pExCtxEmb)
  {
    // support for args of command executed by iniApp of module without Qt (with separate independent context) 
    // like next of TD_TestCommands (INT-6233)
    // cuiload "c:/_Oda/trunk/Drawing/Examples/Qt/OdaQtApp/data/cui/odq_toolbar_tstcmds_tx.cui"
    ExStringIO* pStringIO = (ExStringIO*) pExCtx->baseIO();
    if (pStringIO && !pStringIO->isEof())
      return true;
  }

  return isInputStringReady(pCmdCtx->baseDatabase());
}

const TCmdMap& OdqConsole::getCommandMap() const
{
  return m_mapCmds;
}

OdRxObject* OdqConsole::getCurrentDatabase() const
{
  QWidget *pCurrTab = m_pTabWidget->currentWidget();
  if (!pCurrTab)
    return NULL;

  OdRxObject* pRxDatabase = ((OdqConsoleTab *) pCurrTab)->getRxDatabase();
  return pRxDatabase;
}

QList<OdRxObject*> OdqConsole::getOpenDatabases() const
{
  QList<OdRxObject*> lst = m_mapTabs.keys();
  if (!lst.first())
    lst.removeFirst(); 

  return lst;
}

OdEdCommandContext* OdqConsole::getCommandContext() const // current
{
  return getCommandContext(getCurrentDatabase());
}

OdEdCommandContext* OdqConsole::getCommandContext(OdRxObject* pRxDatabase) const
{
  OdqConsoleTab* pTab = getConsoleTab(pRxDatabase);
  if (!pTab && !pRxDatabase)
    pTab = getConsoleTab(pRxDatabase, true);
  // possible via ClearDevice od OdqView //ODA_ASSERT_ONCE(pTab);
  if (!pTab)
    return NULL;

  OdEdCommandContext* pCmdCtx = OdEdCommandContext::cast((OdDbCommandContext*) &pTab->getCmdContext()).get();
  return pCmdCtx; 
}

OdRxObject* OdqConsole::getDatabaseDragSource() const
{
  return CommandContextImpl::baseDatabaseDragSource();
}

bool OdqConsole::isMarkedToClose(OdRxObject* pRxDatabase)
{
  if (m_pTabWidget.isNull())
    return true;
  if (!pRxDatabase)
  {
    ODA_FAIL_ONCE(); // test
    return true;
  }

  QMap<OdRxObject*, 
       OdSharedPtr<OdqConsoleTab> >::iterator itrTabs = m_mapTabs.find(pRxDatabase);
  if (itrTabs == m_mapTabs.end())
  {
    ODA_FAIL_ONCE(); // tab was not found
    return true;
  }
  OdSharedPtr<OdqConsoleTab>& pTab = itrTabs.value();

  // look also <mark to delete tab with end of command>
  return !pTab->parentWidget();
}

bool OdqConsole::setCurrentDatabase(OdRxObject* pRxDatabase)
{
  if (pRxDatabase == getCurrentDatabase())
    return true;

  QMap<OdRxObject*, 
       OdSharedPtr<OdqConsoleTab> >::iterator itrTabs = m_mapTabs.find(pRxDatabase);
  if (itrTabs == m_mapTabs.end())
  {
    //ODA_FAIL_ONCE(); // tab was not found
    return false;
  }

  OdSharedPtr<OdqConsoleTab>& pTab = itrTabs.value();
  if (pTab.isNull())
  {
    ODA_FAIL_ONCE(); // tab was removed
    return false;
  }

  m_pTabWidget->setCurrentWidget(pTab);
  return true;
}

bool OdqConsole::closeDatabase(OdRxObject* pRxDatabase)
{
  if (!pRxDatabase)
    // this tab always exists
    // (for NULL database different view contains 
    //  absolutely independent geometry)
    return false;

  // isModified() of OdDbDatabase always returns true 
  OdqConsoleTab& tabConsole = *getConsoleTab(pRxDatabase);

  int reply = QMessageBox::No;
  if (pRxDatabase && tabConsole.isDatabaseModified())
  {
    QString qsTabName = getIApp()->toQString(getFilePathName(pRxDatabase)); 
    if (!qsTabName.isEmpty())
      reply = QMessageBox::question(qApp->activeWindow(), 
                                    qsTabName, // caption
                                    QObject::tr("Do you want to save changes?"),
                                    QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, 
                                    QMessageBox::Yes);
  }

  switch (reply)
  {
  case QMessageBox::Yes:
    tabConsole.executeCommand(QString("qsave"), false, true); // , echo, subCommand)
    //break;
  case QMessageBox::No:
    //tabConsole.setDatabaseModified(false);
    break;
  case QMessageBox::Cancel:
    return false;
  default:
    ODA_FAIL_ONCE(); // TODO
    break;
  } // end switch

  //if (tabConsole.getCmdContext().hasCmdWithUndo())
    // we came here via closeall command and cannot wait to end 
    // of all commands in stack before execute closeall 
    // (by return from close event all will be closed and inaccessible)
    //
    // as minimum we should mark all command to cancel later
    //
    tabConsole.getCmdContext().markToCancelAndExecute(OD_T(""), false);

  removeConsoleTab(pRxDatabase);
  return true;
}

// static 
OdqConsoleTab* OdqConsole::getConsoleTab(OdRxObject* pRxDatabase,
                                         bool bSetCurrent, // = false
                                         const OdString& sFilePathNameOverride) // = OdString::kEmpty
{
  QMap<OdRxObject*, OdSharedPtr<OdqConsoleTab> >::iterator itrTabs
      = getConsole().m_mapTabs.find(pRxDatabase);
  if (itrTabs != getConsole().m_mapTabs.end())
  {
    OdSharedPtr<OdqConsoleTab>& pTab = itrTabs.value();
    ODA_ASSERT_ONCE(!pTab.isNull());
    if (bSetCurrent)
    {
      getConsole().m_pTabWidget->setCurrentWidget(pTab);
    }
    return pTab;
  }
  if (!bSetCurrent)
    return NULL;

  itrTabs = getConsole().m_mapTabs.insert(pRxDatabase, OdSharedPtr<OdqConsoleTab>());
  ODA_ASSERT_ONCE(itrTabs != getConsole().m_mapTabs.end());

  OdSharedPtr<OdqConsoleTab>& pTab = itrTabs.value();
  ODA_ASSERT_ONCE(pTab.isNull());
  pTab = new OdqConsoleTab(getConsole().m_pTabWidget, pRxDatabase, getConsole().getCommandMap());

  if (!sFilePathNameOverride.isEmpty())
    pTab->setFilePathName(sFilePathNameOverride);

  QWidget* pCurrTab = pTab;
  OdRxObjectPtr pCurDatabase(pRxDatabase);
  if (!bSetCurrent && getConsole().m_pTabWidget->currentWidget())
  {
    pCurrTab = getConsole().m_pTabWidget->currentWidget();
    pCurDatabase = ((OdqConsoleTab *) pCurrTab)->getRxDatabase(); 
  }

  //QString qsTabName(qsFilePathName);
  QString qsTabName(pTab->getQTitle());

  getConsole().m_pTabWidget->addTab(pTab, 
                                    qsTabName); // TODO generate tab name though pRxDatabase
  pTab->updateQTitle();
  qApp->processEvents(); // show it

  if (pCurrTab)
  {
    getConsole().m_pTabWidget->setCurrentWidget(pCurrTab);
  }

  return pTab;
}

//////////////////////////////////////////////////////////////////////////

// slot
void OdqConsole::currentTabChanged(int index)
{
  ODA_ASSERT_ONCE(   !m_pTabWidget.isNull()
                  && index == m_pTabWidget->currentIndex()); // already switched
  if (index < 0)
    return;

  OdqConsoleTab* pTab = qobject_cast<OdqConsoleTab*>(m_pTabWidget->widget(index));
  ODA_ASSERT_ONCE(pTab);
  OdRxObjectPtr pRxDatabase = pTab->getRxDatabase();

  IqViewSystem* iVS = getIViewSystem();
  if (iVS)
    iVS->getView(pRxDatabase, true); // bDoActive

  OdDbDatabase* pDwgDb = OdDbDatabase::cast(pRxDatabase.get()).get();
  if (   pDwgDb 
    //&& pDwgDb->getModelSpaceId().isNull()
      && pDwgDb->getViewportTableId().isNull())
  // it's maybe for such command like recover.
  // in this case database is just created and is not valid for such 
  // things as diesel evaluation for example
  //
  // getIApp()->refreshQObjetcts("<event-refresh:database_changed>") will be call
  // after database loading directly
    return;

  getIApp()->refreshQObjetcts("<event-refresh:database_changed>"); // , SLOT(refresh(const QString&)));
}

OdString OdqConsole::getFilePathName(OdRxObject* pRxDatabase)
{
  OdqConsoleTab* pTabConsole = getConsoleTab(pRxDatabase, false);
  if (!pTabConsole)
    return OdString::kEmpty;
  return pTabConsole->getFilePathName();
}

QString OdqConsole::getQTitle(OdRxObject* pRxDatabase)
{
  OdqConsoleTab& tabConsole = *getConsoleTab(pRxDatabase, false);
  return tabConsole.getQTitle();
}

// static 
bool OdqConsole::removeConsoleTab(OdRxObject* pRxDatabase)
{
  QMap<OdRxObject*, OdSharedPtr<OdqConsoleTab> >::iterator itrTabs
      = getConsole().m_mapTabs.find(pRxDatabase);
  if (itrTabs == getConsole().m_mapTabs.end())
  {
    ODA_FAIL_ONCE(); // test // tab was not found
    return true;
  }

  OdSharedPtr<OdqConsoleTab>& pTab = itrTabs.value();
  if (pTab.isNull())
  {
    ODA_FAIL_ONCE(); // test // tab already was removed
    return true;
  }

  QWidget *pTabToRestore = getConsole().m_pTabWidget->currentWidget();
  // fix crash at close application if current property tab was set to None
  if (pRxDatabase != getConsole().getCurrentDatabase())
    getConsole().setCurrentDatabase(pRxDatabase);
  //
  getIApp()->refreshQObjetcts("<event-refresh:database_closed>");

  if (pTabToRestore != pTab)
    getConsole().m_pTabWidget->setCurrentWidget(pTab);
  else
    pTabToRestore = NULL;

  ODA_ASSERT_ONCE(pTab->parentWidget()->parentWidget() == getConsole().m_pTabWidget); 
  
  //Removes the page at position index from this stack of widgets. Does not delete the page widget
  getConsole().m_pTabWidget->removeTab(getConsole().m_pTabWidget->currentIndex());
  //qApp->processEvents(); /// show it

  CommandState& stateCmdRes = pTab->getTopCmdState();
  if (!stateCmdRes.isAllocated())
  {
    pTab = NULL; // delete
    getConsole().m_mapTabs.remove(pRxDatabase);
  }
  else
  {
    // we have it with such command as close and closeAll
    // (tab should be deleted with finish of command 
    //  via removeConsoleTab // confirmToDeleteMarkedTab

    pTab->setParent(NULL); // look also <mark to delete tab with end of command>
    ODA_ASSERT_ONCE(!pTab->parentWidget()); 
  }

  if (pTabToRestore) // restore current tab
    getConsole().m_pTabWidget->setCurrentWidget(pTabToRestore);

  return true;
}

// static 
bool OdqConsole::removeConsoleTab(OdqConsoleTab &tab)
{
  if (tab.parentWidget()) // look also <mark to delete tab with end of command>
  {
    ODA_FAIL_ONCE(); // test
    return false;
    // TODO
    //return removeConsoleTab(tab.getRxDatabase());
  }
  ODA_ASSERT_ONCE(!tab.getRxDatabase().isNull());

  OdRxObject* pRxDatabase = tab.getRxDatabase();

  QMap<OdRxObject*, OdSharedPtr<OdqConsoleTab> >::iterator itrTabs
    = getConsole().m_mapTabs.find(pRxDatabase);
  if (   itrTabs == getConsole().m_mapTabs.end()
      || itrTabs.value().isNull())
  {
    ODA_FAIL_ONCE(); // test // tab already was removed
    return false;
  }

  OdSharedPtr<OdqConsoleTab>& pTab = itrTabs.value();

  pTab = NULL; // delete
  getConsole().m_mapTabs.remove(pRxDatabase);
  return true;
}

bool OdqConsole::getSlotData(QEvent::Type eventType, // in
                             OdRxObject* pRxDatabase, // in
                             QObject*& pReceiver, // out
                             const char*& pcszMember) // out
{
  pReceiver = getConsoleTab(pRxDatabase);
  pcszMember = NULL;
  switch (eventType)
  {
  case QEvent::MouseMove:
    pcszMember = SLOT(slotMouseMove(IqView*, QEvent*, bool&));
    return true;
  case QEvent::MouseButtonPress:
    pcszMember = SLOT(slotMouseButtonPress(IqView*, QEvent*, bool&));
    return true;
  case QEvent::MouseButtonRelease:
    pcszMember = SLOT(slotMouseButtonRelease(IqView*, QEvent*, bool&));
    return true;
  case QEvent::MouseButtonDblClick:
    pcszMember = SLOT(slotMouseDoubleClick(IqView*, QEvent*, bool&));
    return true;
  case QEvent::Wheel:
    pcszMember = SLOT(slotMouseWheel(IqView*, QEvent*, bool&));
    return true;
 case QEvent::KeyPress:
    pcszMember = SLOT(slotKeyPress(IqView*, QEvent*, bool&));
    return true;
  case QEvent::KeyRelease:
    pcszMember = SLOT(slotKeyRelease(IqView*, QEvent*, bool&));
    return true;

//Disabled since Qt5.3
#if defined(ANDROID) || defined(__IPHONE_OS_VERSION_MIN_REQUIRED)
  case QEvent::TouchBegin:
    pcszMember = SLOT(slotTouchPressed(IqView*, QEvent*, bool&));
    return true;
  case QEvent::TouchEnd:
    pcszMember = SLOT(slotTouchReleased(IqView*, QEvent*, bool&));
    return true;
  case QEvent::TouchUpdate:
    pcszMember = SLOT(slotTouchMove(IqView*, QEvent*, bool&));
    return true;
#endif
  } // end switch

  //ODA_FAIL_ONCE(); // TODO
  return false;
}

class OdqDieselServiceImpl : public OdDieselService
{
  OdEdCommandContext* m_pCmdCtx;

public:
  OdqDieselServiceImpl(OdEdCommandContext* pCmdCtx)
    : m_pCmdCtx(pCmdCtx)
  {
  }

  virtual bool getSystemVariable(const OdString& sName, OdString& sValue)
  {
    QString qsCmd = getIApp()->toQString(sName).toLower();
    if (!qsCmd.indexOf("iscmd("))
    {
      // is command loaded
      qsCmd = qsCmd.mid(6);
      int pos = qsCmd.indexOf(')');
      ODA_ASSERT_ONCE(pos == (qsCmd.length() - 1));
      qsCmd = qsCmd.left(pos);
      ODA_ASSERT_ONCE(!qsCmd.isEmpty());
      if (qsCmd.isEmpty())
        return false;
      OdEdCommandStackPtr pCommands = ::odedRegCmds();
      OdEdCommandPtr pCmd = pCommands->lookupCmd(getIApp()->toOdString(qsCmd));
      if (!pCmd.isNull())
        sValue = OD_T("1");
      else
        sValue = OD_T("0");
      return true;
    }

    OdSysVarPEPtr pSysVarPE = OdSysVarPE::cast(m_pCmdCtx);
    ODA_ASSERT_ONCE(!pSysVarPE.isNull());
    if (pSysVarPE.isNull())
      return false; 

    OdSysVarType typeRb = kRtNone;
    bool bOk = false;
    sValue = pSysVarPE->valueToString(m_pCmdCtx, sName, &typeRb, &bOk);
    //if (   sValue.getLength() >= 2
    //    && (   typeRb == kRtString
    //        || typeRb == (OdSysVarType) IqAppProps::prpCombo // ++ IqAppProps::prpLinetype IqAppProps::prpLineweight IqAppProps::prpTransparency
    //        || typeRb == (OdSysVarType) IqAppProps::prpFolder))
    //{
    //  // removeDoubleQuotes
    //  int pos = sValue.find(__OD_T('"'));
    //  if (!pos)
    //    sValue = sValue.mid(1, sValue.getLength() - 2);
    //}
    if (bOk && typeRb == kRtBool)
    {
      if (sValue == L"true")
        sValue = L"1";
      else if (sValue == L"false")
        sValue = L"0";
    }

    return bOk;
  }

  virtual void getUnits(int* pAngularUnits, // AUNITS
                        int* pAngularPrec, // AUPREC
                        int* pLinearUnits, // LUNITS
                        int* pLinearPrec, // LUPREC
                        int* pDimzin, // DIMZIN
                        int* pUnitMode) // UNITMODE
  {
    OdDbDatabase* pDb = NULL;
    if (m_pCmdCtx)
      pDb = OdDbDatabase::cast(m_pCmdCtx->baseDatabase());

    if (pAngularUnits)
      *pAngularUnits = pDb ? pDb->getAUNITS() : 0;
    if (pAngularPrec)
      *pAngularPrec = pDb ? pDb->getAUPREC() : 0;
    if (pLinearUnits)
      *pLinearUnits = pDb ? pDb->getLUNITS() : 2;
    if (pLinearPrec)
      *pLinearPrec = pDb ? pDb->getLUPREC() : 4;
    if (pDimzin)
      *pDimzin = pDb ? pDb->dimzin() : 0; // TODO (8 - metric)
    if (pUnitMode)
      *pUnitMode = pDb ? pDb->getUNITMODE() : 0;
  }

  virtual bool getEnvironmentVariable(const OdString& sName, OdString& sValue)
  {
    sValue.empty();
    if (char* epA = getenv(sName))
    {
      sValue = epA;
      return true;
    }
    return false;
  }
  virtual bool getDrawingProperty(const OdString& sName, OdString& sValue)
  {
    if (!m_pCmdCtx)
      return false;
    OdDbDatabase* pDb = OdDbDatabase::cast(m_pCmdCtx->baseDatabase()).get();
    if (!pDb)
      return false;
    OdDbDatabaseSummaryInfoPtr pInf = oddbGetSummaryInfo(pDb);

    OdString strRet;
    if (sName[0] == L'%')
    { //Custom property
      OdString strKey = sName.c_str() + 1;
      return pInf->getCustomSummaryInfo(strKey, sValue);
    }

    if (sName == L"TITLE")
      sValue = pInf->getTitle();
    else if (sName == L"SUBJECT")
      sValue = pInf->getSubject();
    else if (sName == L"AUTHOR")
      sValue = pInf->getAuthor();
    else if (sName == L"KEYWORDS")
      sValue = pInf->getKeywords();
    else if (sName == L"COMMENTS")
      sValue = pInf->getComments();
    else if (sName == L"LASTSAVEDBY")
      sValue = pInf->getLastSavedBy();
    else if (sName == L"REVISIONNUMBER")
      sValue = pInf->getRevisionNumber();
    else if (sName == L"HYPERLINKBASE")
      sValue = pInf->getHyperlinkBase();
    else
      return false;

    return true;
  }
};

QString OdqConsole::evaluateDieselExpression(const QString& qsExpression,
                                             OdRxObject* pRxDatabase) // = NULL current
{
  return getIApp()->toQString(evaluateDieselExpression(getIApp()->toOdString(qsExpression),
                                                       pRxDatabase));
}

OdString OdqConsole::evaluateDieselExpression(const OdString& csExpression,
                                              OdRxObject* pRxDatabase) // = NULL current
{
  OdString sExpression(csExpression);
  sExpression.makeLower();
  if (sExpression.find(OD_T("$(")) < 0)
    // likely there is nothing to evaluate
    // TODO (or it's degenerate expression (without conditions)
    // like "Hello,world!" (?via command diesel)
    return csExpression;

  OdEdCommandContext* pCmdCtx = pRxDatabase ? getCommandContext(pRxDatabase)
                                            : getCommandContext();
  OdqDieselServiceImpl service(pCmdCtx);

  OdString sPref;
  int pos = sExpression.find(OD_T("$m=")),
      offsErr = 0;

  //sExpression = csExpression; // restore case
  if (pos < 0)
  {
    pos = sExpression.find(OD_T("m=")); // like for menucmd of AutoLISP
    if (!pos)
    {
      sExpression = csExpression.mid(2);
      offsErr = 2;
    }
    else
      sExpression = csExpression; // restore case
  }
  else
  {
    sPref = csExpression.left(pos);
    offsErr = pos + 3;
    sExpression = csExpression.mid(offsErr);
    offsErr = offsErr;
  }

  OdString sOut;
  int posErr = ::OdDieselEvaluate(sExpression.c_str(), sOut.getBuffer(512), &service);
  sOut.releaseBuffer();
  if (posErr) // returns the column	at which the error was found
  {
    OdqConsoleTab* pTab = getConsoleTab(pRxDatabase);
    ODA_ASSERT_ONCE(pTab);
    if (!pTab)
      return OdString::kEmpty;

    posErr += offsErr;

    QString sMsg = tr("*** Diesel error at colunm %1 in next expression ***:").arg(posErr);
    pTab->putMsgText(sMsg);

    if (posErr > 1)
      pTab->putMsgText(QString("%1v").arg(QString(posErr - 1, ' '))); // marker
    pTab->putMsgText(csExpression);
    return OdString::kEmpty;
  }
  return sPref + sOut;
}

void OdqConsole::updateAllGrips(OdRxObject* pRxDatabase) // = NULL current
{
  if (!pRxDatabase)
    pRxDatabase = getCurrentDatabase();

  OdqConsoleTab* pTab = getConsoleTab(pRxDatabase);
  if (!pTab)
    return;

  OdqGripManagerPtr pGripManager = pTab->getCmdContext().gripManager();
  ODA_ASSERT_ONCE(!pGripManager.isNull()); // test
  if (pGripManager.isNull())
    return;

  pGripManager->updateAllGrips();
}

OdSelectionSet* OdqConsole::workingSelectionSet(OdRxObject* pRxDatabase) // = NULL current
{
  if (!pRxDatabase)
    pRxDatabase = getCurrentDatabase();
  if (!pRxDatabase)
    return NULL;

  OdqConsoleTab* pTab = getConsoleTab(pRxDatabase);
  if (!pTab)
    return NULL;

  return pTab->workingSelectionSet();
}

void OdqConsole::clearSelectionSet(OdRxObject* pRxDatabase) // = NULL current
{
  if (!pRxDatabase)
    pRxDatabase = getCurrentDatabase();
  if (!pRxDatabase)
    return;
  OdqConsoleTab* pTab = getConsoleTab(pRxDatabase);
  if (!pTab)
    return;

  pTab->clearSelectionSet();
}

bool OdqConsole::isConsoleVisible() const
{
  bool bVisible = OdqConsole::getDockPalette()->isVisible();
  return bVisible;
}

OdString OdqConsole::activeCommandName(OdRxObject* pRxDatabase) // = NULL current
{
  if (!pRxDatabase)
    pRxDatabase = getCurrentDatabase();

  OdqConsoleTab* pTab = getConsoleTab(pRxDatabase);
  if (!pTab)
    return OdString::kEmpty;
  CommandState& stateCmdRes = pTab->getTopCmdState();
  if (!stateCmdRes.isAllocated() || stateCmdRes.m_qsLastCommandName.isEmpty())
    return OdString::kEmpty;

  OdString sActiveCmd = getIApp()->toOdString(stateCmdRes.m_qsLastCommandName);
  return sActiveCmd;
}

IqConsole::TCmdActiveState OdqConsole::activeCommandState(OdRxObject* pRxDatabase) // = NULL current
{
  if (!pRxDatabase)
    pRxDatabase = getCurrentDatabase();

  OdqConsoleTab* pTab = getConsoleTab(pRxDatabase);
  if (!pTab)
    return enCmdActive_No;
  CommandState& stateCmdRes = pTab->getTopCmdState();
  if (!stateCmdRes.isAllocated() || stateCmdRes.m_qsLastCommandName.isEmpty())
    return enCmdActive_No;
  ODA_ASSERT_ONCE(!stateCmdRes.m_qsLastCommandName.isEmpty());

  TCmdActiveState st = enCmdActive_Ordinary;
  // TODO
  return st;
}

bool OdqConsole::markToCancel(OdRxObject* pRxDatabase) // = NULL current
{
  if (!pRxDatabase)
    pRxDatabase = getCurrentDatabase();

  OdqConsoleTab* pTab = getConsoleTab(pRxDatabase);
  if (!pTab)
    return enCmdActive_No;
  CommandState& stateCmdRes = pTab->getTopCmdState();
  if (!stateCmdRes.isAllocated() || stateCmdRes.m_qsLastCommandName.isEmpty())
    return false;
  ODA_ASSERT_ONCE(!stateCmdRes.m_qsLastCommandName.isEmpty());

  pTab->getCmdContext().markToCancelAndExecute(OD_T(""), false);
  return true;
}

//////////////////////////////////////////////////////////////////////////

#if defined(Q_WS_MAC) || defined(Q_OS_MAC) || defined(__APPLE__)
OdqFixPlatform::OdqFixPlatform()
{
}

OdqFixPlatform::~OdqFixPlatform()
{
}

// used via FIX_MAC_APP_TROW to prevent exceptions from app to dylib
void OdqFixPlatform::fix_mac_throw(const OdError& err)
{
  //if (err.context())
  //{
  //  // TODO // EdExceptionCtx, CancelException ... are not accessible here (and are not registred via RTTI)
  //  if (err.context()->isKindOf(CancelException::desc())
  //    throw OdEdCancel();
  //  // ...
  //}

  if (err.description().find(OD_T("Exception: OdEdEmptyInput")) >= 0)
    throw OdEdEmptyInput();
  if (err.description().find(OD_T("Exception: OdEdOtherInput")) >= 0)
    throw OdEdOtherInput(((const OdEdOtherInput*) &err)->string());
  if (err.description().find(OD_T("Exception: OdEdKeyword")) >= 0)
    throw OdEdKeyword(((const OdEdKeyword*) &err)->keywordIndex(),
                      ((const OdEdKeyword*) &err)->keyword());
  if (err.description().find(odrxSystemServices()->formatMessage(sidCommandCanceled)) >= 0) // "Command canceled."
    throw OdEdCancel();

  static QList<OdResult> s_lstCode;
  if (s_lstCode.isEmpty())
    s_lstCode << eNoDatabase 
              << eNotApplicable
              << eInvalidInput 
              << eNotImplemented;
  foreach (OdResult code, s_lstCode)
  {
    if (code != err.code())
      continue;
    throw OdError(code);
  }

  ODA_FAIL_ONCE(); // TODO
  throw OdEdCancel(); // throw;
}
#endif

//////////////////////////////////////////////////////////////////////////
