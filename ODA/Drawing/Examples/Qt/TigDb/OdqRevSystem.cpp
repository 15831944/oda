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
// OdqRevSystem.cpp
//

#include <QMainWindow>
#include <QDockWidget>
#include <QTabWidget>
#include <QTabBar>
#include <QLayout>
#include <QSettings>
#include <QFileInfo>
//#include <QTimer>

#include "OdaCommon.h"
#include "TDVersion.h"

#include "OdqRevSystem.h"

//------------------------------------------------------------------------

OdqRevSystem::OdqRevSystem()
{
  m_pRevCtrl = new OdqRevisionController();

  QSettings settings("ODA", 
                     QFileInfo(QCoreApplication::applicationFilePath()).baseName());
  settings.beginGroup(TD_SHORT_STRING_VER_S);

  QMainWindow* pMainWindow = getIApp()->getMainWindow();

  // create RevSystem palette

  m_pDockPalette = new QDockWidget(tr("Revision control"), pMainWindow); // "History revisions"
  m_pDockPalette->setAttribute(Qt::WA_DeleteOnClose, false);
  m_pDockPalette->setObjectName("RevSystem");
  m_pDockPalette->setAllowedAreas(Qt::AllDockWidgetAreas);

  //QPixmap pixmap("Palette_RevSystem.png");
  //ODA_ASSERT_ONCE(!pixmap.isNull());
  //pixmap.setMask(pixmap.createHeuristicMask());
  //m_pDockPalette->setWindowIcon(QIcon(pixmap));

  QTabWidget::TabPosition posTab = (QTabWidget::TabPosition) settings.value("RevSystem/tab_pos",
                                                                            QTabWidget::East).toInt();
  m_pTabWidget = new QTabWidget(m_pDockPalette);
  m_pTabWidget->setTabPosition(posTab); // North South [ West East ]
  if (!QObject::connect(m_pTabWidget, SIGNAL(currentChanged(int)),
                        this, SLOT(currentTabChanged(int))))
  {
    ODA_FAIL_ONCE();
  }

  m_pDockPalette->setWidget(m_pTabWidget);

  Qt::DockWidgetArea area = (Qt::DockWidgetArea) settings.value("RevSystem/area",
                                                                Qt::LeftDockWidgetArea).toInt();
  pMainWindow->addDockWidget(area, m_pDockPalette);

  QString qsTabified = settings.value("RevSystem/tabified").toString();
  if (!qsTabified.isEmpty())
  {
    QDockWidget* pDockWidgetFirst = NULL;
    foreach (QString qsObj, qsTabified.split(','))
    {
      QList<QDockWidget*> lst = pMainWindow->findChildren<QDockWidget*>(qsObj);
      foreach (QDockWidget* pDockWidget, lst)
      {
        pMainWindow->tabifyDockWidget(pDockWidget, m_pDockPalette);
        if (!pDockWidgetFirst)
          pDockWidgetFirst = pDockWidget;
      }
    }
    if (pDockWidgetFirst)
      pDockWidgetFirst->raise(); //QTimer::singleShot(0, pDockWidgetFirst, SLOT(setFocus()));
  }

  // get protected tabbar and set visibility
  QList<QTabBar*> widgets = m_pDockPalette->widget()->findChildren<QTabBar*>();
  if (widgets.count() == 1)
    widgets.last()->setVisible(settings.value("RevSystem/tab_vis", 0) != 0);
  else
    ODA_FAIL();

  m_pDockPalette->setFeatures(  QDockWidget::DockWidgetMovable
                              | QDockWidget::DockWidgetClosable
                              | QDockWidget::DockWidgetFloatable); 
  //pMainWindow->layout()->activate();

  int w = settings.value("w", 580).toInt();
  int width = settings.value("RevSystem/w", w / 3).toInt();
  m_pDockPalette->setFixedWidth(width);

  pMainWindow->layout()->activate();

  m_pDockPalette->setMinimumWidth(w / 10);
  m_pDockPalette->setMaximumWidth(w * 9 / 10);

  m_pDockPalette->setVisible(settings.value("RevSystem/vis", 1) != 0);

  settings.endGroup();
}

OdqRevSystem::~OdqRevSystem()
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
    OdRxObject* pRxDatabase = ((OdqRevTab *) pCurrTab)->getRxDatabase();
    removeRevTab(pRxDatabase);
  }

  m_pDockPalette = NULL;
}

void OdqRevSystem::saveSettings()
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

  QList<QDockWidget*> lst = getIApp()->getMainWindow()->tabifiedDockWidgets(m_pDockPalette);
  if (lst.size())
  {
    QString qsTabified;
    foreach (QDockWidget* pDocWidgwet, lst)
    {
      ODA_ASSERT_ONCE(pDocWidgwet->objectName() != m_pDockPalette->objectName());
      if (!qsTabified.isEmpty())
        qsTabified += ",";
      qsTabified += pDocWidgwet->objectName();
    }
    if (!qsTabified.isEmpty())
      settings.setValue("RevSystem/tabified", qsTabified);
  }

  QPoint pos = m_pDockPalette->pos();
  settings.setValue("RevSystem/x", pos.x());
  settings.setValue("RevSystem/y", pos.x());

  QRect rect = m_pDockPalette->geometry();
  settings.setValue("RevSystem/w", rect.width());
  settings.setValue("RevSystem/h", rect.height());

  QMainWindow* pMainWindow = qobject_cast<QMainWindow*>(m_pDockPalette->parent());
  ODA_ASSERT_ONCE(pMainWindow);

  settings.setValue("RevSystem/area", pMainWindow->dockWidgetArea(m_pDockPalette));
  settings.setValue("RevSystem/vis", m_pDockPalette->isVisible() ? 1 : 0);

  settings.setValue("RevSystem/tab_pos", m_pTabWidget->tabPosition());

  QList<QTabBar*> widgets = OdqRevSystem::getDockPalette()->widget()->findChildren<QTabBar*>();
  if (widgets.count() == 1)
    settings.setValue("RevSystem/tab_vis", widgets.last()->isVisible() ? 1 : 0);
  else
    ODA_FAIL();

  settings.endGroup();
}

OdqRevisionController* OdqRevSystem::controller() const
{
  if (!m_pRevCtrl || !m_pRevCtrl->isValid())
    return NULL;
  return m_pRevCtrl.get();
}

//static
OdqRevSystem& OdqRevSystem::getRevSystem()
{
  static QPointer<OdqRevSystem> s_pRevSystem;
  if (s_pRevSystem.isNull())
    s_pRevSystem = qobject_cast<OdqRevSystem*>(getIApp()->getSingle(IqRevSystem_iid));
  ODA_ASSERT_ONCE(!s_pRevSystem.isNull()); // test

  return *s_pRevSystem;
}

//static 
QDockWidget* OdqRevSystem::getDockPalette()
{
  return getRevSystem().m_pDockPalette.get(); 
}

// ----- IRevSystem -----

bool OdqRevSystem::isRevVisible() const
{
  bool bVisible = OdqRevSystem::getDockPalette()->isVisible();
  return bVisible;
}

IqRevDiff* OdqRevSystem::getIRevDiff(IqView* iView)
{
  if (!iView)
    return NULL;
  OdRxObject* pRxDb = iView->getRxDatabase();
  if (!pRxDb)
    return NULL;
  QMap<OdRxObject*, OdSharedPtr<OdqRevTab> >::iterator itrTabs = m_mapTabs.find(pRxDb);
  if (itrTabs == getRevSystem().m_mapTabs.end())
    return NULL;
  OdSharedPtr<OdqRevTab>& pTab = itrTabs.value();
  if (!pTab->isResposibleDiffView(iView))
    return NULL;

  return qobject_cast<IqRevDiff*>(pTab.get());
}

bool OdqRevSystem::isUnder(OdRxObject* pRxDb) const
{
  if (!pRxDb || !controller() || !controller()->isResponsible(pRxDb))
    return false;
  return true;
}

// -----

// static 
OdqRevTab* OdqRevSystem::getRevTab(OdRxObject* pRxDatabase,
                                   bool bSetCurrentAndCreateIfMissing, // = false
                                   bool bCreateIfMissing) // = false
{
  if (bSetCurrentAndCreateIfMissing)
    bCreateIfMissing = true;

  QMap<OdRxObject*, OdSharedPtr<OdqRevTab> >::iterator itrTabs
    = getRevSystem().m_mapTabs.find(pRxDatabase);
  if (itrTabs != getRevSystem().m_mapTabs.end())
  {
    OdSharedPtr<OdqRevTab>& pTab = itrTabs.value();
    ODA_ASSERT_ONCE(!pTab.isNull());
    if (   bSetCurrentAndCreateIfMissing 
        && qobject_cast<OdqRevTab*>(getRevSystem().m_pTabWidget->currentWidget()) != pTab.get())
    {
      getRevSystem().m_pTabWidget->setCurrentWidget(pTab);
      if (pRxDatabase && getRevSystem().isRevVisible())
        // maybe non-updated as invisible
        getRevSystem().refresh("<event-refresh:revision_changed>", ""); 
    }
    return pTab;
  }
  if (!bCreateIfMissing)
    return NULL;

  itrTabs = getRevSystem().m_mapTabs.insert(pRxDatabase, OdSharedPtr<OdqRevTab>());
  ODA_ASSERT_ONCE(itrTabs != getRevSystem().m_mapTabs.end());

  OdSharedPtr<OdqRevTab>& pTab = itrTabs.value();
  ODA_ASSERT_ONCE(pTab.isNull());
  pTab = new OdqRevTab(getRevSystem().m_pTabWidget, pRxDatabase);
  ODA_ASSERT_ONCE(pRxDatabase == pTab->getRxDatabase());

  QWidget* pCurrTab = pTab;
  OdRxObjectPtr pCurDatabase(pRxDatabase);
  if (!bSetCurrentAndCreateIfMissing && getRevSystem().m_pTabWidget->currentWidget())
  {
    pCurrTab = getRevSystem().m_pTabWidget->currentWidget();
    pCurDatabase = ((OdqRevTab *) pCurrTab)->getRxDatabase();
  }
  //QString qsTabName(qsFilePathName);
  QString qsTabName = pRxDatabase ? getIConsole()->getQTitle(pRxDatabase) : QObject::tr("None");

  // indexes in revision palette tab widget 
  // should have the same order as in console.
  // but current should created the first if prop palette was reloaded.
  int idxToInsert = 0;
  if (pRxDatabase)
  {
    QList<OdRxObject*> lst = getIConsole()->getOpenDatabases();
    lst.push_front(NULL);
    idxToInsert = lst.indexOf(pRxDatabase);
  }
  int count = getRevSystem().m_pTabWidget->count();
  if (idxToInsert >= 0 && count && idxToInsert < count)
    getRevSystem().m_pTabWidget->insertTab(count - 1, pTab, qsTabName);
  else
    getRevSystem().m_pTabWidget->addTab(pTab, qsTabName);

  qApp->processEvents(); // show it

  if (pCurrTab)
    getRevSystem().m_pTabWidget->setCurrentWidget(pCurrTab);

  return pTab;
}

//////////////////////////////////////////////////////////////////////////

// slot
void OdqRevSystem::refresh(const QString& qsKey, const QString& sVal) // refresh states of widgets
{
  ODA_ASSERT_ONCE(   qsKey == "<event-refresh:database_closed>"
                  || qsKey == "<event-refresh:database_changed>"
                  || qsKey == "<event-refresh:revision_changed>"
                  || qsKey == "<event-refresh:window_removed>"); // test
  IqConsole* iConsole = getIConsole();
  if (!iConsole)
    return;
  OdRxObject* pRxDatabase = iConsole->getCurrentDatabase();
  if (qsKey == "<event-refresh:database_closed>")
  {
    OdqRevSystem::removeRevTab(pRxDatabase);
    if (controller())
      controller()->closeStorage(pRxDatabase);
    return;
  }
  if (qsKey == "<event-refresh:revision_changed>")
  {
    OdqRevTab* pTab = getRevTab(pRxDatabase);
    ODA_ASSERT_ONCE(pTab);
    if (pTab && pTab->isVisible())
      pTab->updateBranches(sVal.isEmpty());
    return;
  }

  OdqRevSystem::getRevTab(pRxDatabase,
                          true); // set active
}

// slot
void OdqRevSystem::currentTabChanged(int index)
{
  ODA_ASSERT_ONCE(   !m_pTabWidget.isNull()
    && index == m_pTabWidget->currentIndex()); // already switched
  if (index < 0)
    return;

  OdqRevTab* pTab = qobject_cast<OdqRevTab*>(m_pTabWidget->widget(index));
  ODA_ASSERT_ONCE(pTab);
  OdRxObject* pRxDatabase = pTab->getRxDatabase();
  int count = m_pTabWidget->count();
  if (!pRxDatabase && count == 1)
    return;

  IqConsole* iConsole = getIConsole();
  if (!iConsole)
    return;
  
  OdRxObject* pCurrRxDatabase = iConsole->getCurrentDatabase();
  if (m_mapTabs.find(pCurrRxDatabase) == m_mapTabs.end())
    return; // just deleted

  if (pRxDatabase != pCurrRxDatabase)
    iConsole->setCurrentDatabase(pRxDatabase);
}

// static 
bool OdqRevSystem::removeRevTab(OdRxObject* pRxDatabase)
{
  QMap<OdRxObject*, OdSharedPtr<OdqRevTab> >::iterator itrTabs
    = getRevSystem().m_mapTabs.find(pRxDatabase);
  if (itrTabs == getRevSystem().m_mapTabs.end())
  {
    //ODA_FAIL_ONCE(); // test // tab was not found
    return true;
  }

  OdSharedPtr<OdqRevTab>& pTab = itrTabs.value();
  if (pTab.isNull())
  {
    ODA_FAIL_ONCE(); // test // tab already was removed
    return true;
  }

  QWidget *pTabToRestore = getRevSystem().m_pTabWidget->currentWidget();
  if (pTabToRestore != pTab)
    getRevSystem().m_pTabWidget->setCurrentWidget(pTab);
  else
    pTabToRestore = NULL;

  ODA_ASSERT_ONCE(pTab->parentWidget()->parentWidget() == getRevSystem().m_pTabWidget); 

  OdSharedPtr<OdqRevTab> pTabHolder(pTab);
  pTab->uninit(); 
  pTab = NULL; // delete
  getRevSystem().m_mapTabs.remove(pRxDatabase);

  //Removes the page at position index from this stack of widgets. Does not delete the page widget
  getRevSystem().m_pTabWidget->removeTab(getRevSystem().m_pTabWidget->currentIndex());
  //qApp->processEvents(); /// show it
  pTabHolder = NULL;

  if (pTabToRestore) // restore current tab
    getRevSystem().m_pTabWidget->setCurrentWidget(pTabToRestore);

  return true;
}

// static 
bool OdqRevSystem::removeRevTab(OdqRevTab &tab)
{
  if (tab.parentWidget()) // look also <mark to delete tab with end of command>
  {
    ODA_FAIL_ONCE(); // test
    return false;
    // TODO
    //return removeRevTab(tab.getRxDatabase());
  }
  ODA_ASSERT_ONCE(tab.getRxDatabase());

  OdRxObject* pRxDatabase = tab.getRxDatabase();

  QMap<OdRxObject*, OdSharedPtr<OdqRevTab> >::iterator itrTabs
    = getRevSystem().m_mapTabs.find(pRxDatabase);
  if (   itrTabs == getRevSystem().m_mapTabs.end()
    || itrTabs.value().isNull())
  {
    ODA_FAIL_ONCE(); // test // tab already was removed
    return false;
  }

  OdSharedPtr<OdqRevTab>& pTab = itrTabs.value();

  pTab = NULL; // delete
  getRevSystem().m_mapTabs.remove(pRxDatabase);
  return true;
}

//////////////////////////////////////////////////////////////////////////
