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
// OdqPropSystem.cpp
//

#include <QMainWindow>
#include <QDockWidget>
#include <QTabWidget>
#include <QTabBar>
#include <QLayout>
#include <QSettings>
#include <QFileInfo>

#include "OdaCommon.h"
#include "TDVersion.h"

#include "OdqPropSystem.h"

//------------------------------------------------------------------------

OdqPropSystem::OdqPropSystem()
{
  QSettings settings("ODA", 
                     QFileInfo(QCoreApplication::applicationFilePath()).baseName());
  settings.beginGroup(TD_SHORT_STRING_VER_S);

  QMainWindow* pMainWindow = getIApp()->getMainWindow();

  // create PropSystem palette

  m_pDockPalette = new QDockWidget(tr("Properties"), pMainWindow);
  m_pDockPalette->setAttribute(Qt::WA_DeleteOnClose, false);
  m_pDockPalette->setObjectName("PropSystem");
  m_pDockPalette->setAllowedAreas(Qt::AllDockWidgetAreas);

  //QPixmap pixmap("Palette_PropSystem.png");
  //ODA_ASSERT_ONCE(!pixmap.isNull());
  //pixmap.setMask(pixmap.createHeuristicMask());
  //m_pDockPalette->setWindowIcon(QIcon(pixmap));

  QTabWidget::TabPosition posTab = (QTabWidget::TabPosition) settings.value("PropSystem/tab_pos",
                                                                            QTabWidget::East).toInt();
  m_pTabWidget = new QTabWidget(m_pDockPalette);
  m_pTabWidget->setTabPosition(posTab); // North South [ West East ]
  if (!QObject::connect(m_pTabWidget, SIGNAL(currentChanged(int)),
                        this, SLOT(currentTabChanged(int))))
  {
    ODA_FAIL_ONCE();
  }

  m_pDockPalette->setWidget(m_pTabWidget);

  Qt::DockWidgetArea area = (Qt::DockWidgetArea) settings.value("PropSystem/area",
                                                                Qt::LeftDockWidgetArea).toInt();
  pMainWindow->addDockWidget(area, m_pDockPalette);

  QString qsTabified = settings.value("PropSystem/tabified", "RevSystem").toString();
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
    widgets.last()->setVisible(settings.value("PropSystem/tab_vis", 0) != 0);
  else
    ODA_FAIL();

  m_pDockPalette->setFeatures(  QDockWidget::DockWidgetMovable
                              | QDockWidget::DockWidgetClosable
                              | QDockWidget::DockWidgetFloatable); 
  //pMainWindow->layout()->activate();

  int w = settings.value("w", 580).toInt();
  int width = settings.value("PropSystem/w", w / 3).toInt();
  m_pDockPalette->setFixedWidth(width);

  pMainWindow->layout()->activate();

  m_pDockPalette->setMinimumWidth(w / 10);
  m_pDockPalette->setMaximumWidth(w * 9 / 10);

  m_pDockPalette->setVisible(settings.value("PropSystem/vis", 1) != 0);

  settings.endGroup();
}

OdqPropSystem::~OdqPropSystem()
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
    OdRxObject* pRxDatabase = ((OdqPropTab *) pCurrTab)->getRxDatabase();
    removePropTab(pRxDatabase);
  }

  m_pDockPalette = NULL;
}

void OdqPropSystem::saveSettings()
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
      settings.setValue("PropSystem/tabified", qsTabified);
  }

  QPoint pos = m_pDockPalette->pos();
  settings.setValue("PropSystem/x", pos.x());
  settings.setValue("PropSystem/y", pos.x());

  QRect rect = m_pDockPalette->geometry();
  settings.setValue("PropSystem/w", rect.width());
  settings.setValue("PropSystem/h", rect.height());

  QMainWindow* pMainWindow = qobject_cast<QMainWindow*>(m_pDockPalette->parent());
  ODA_ASSERT_ONCE(pMainWindow);

  settings.setValue("PropSystem/area", pMainWindow->dockWidgetArea(m_pDockPalette));
  settings.setValue("PropSystem/vis", m_pDockPalette->isVisible() ? 1 : 0);

  settings.setValue("PropSystem/tab_pos", m_pTabWidget->tabPosition());

  QList<QTabBar*> widgets = OdqPropSystem::getDockPalette()->widget()->findChildren<QTabBar*>();
  if (widgets.count() == 1)
    settings.setValue("PropSystem/tab_vis", widgets.last()->isVisible() ? 1 : 0);
  else
    ODA_FAIL();

  settings.endGroup();
}

//static
OdqPropSystem& OdqPropSystem::getPropSystem()
{
  static QPointer<OdqPropSystem> s_pPropSystem;
  if (s_pPropSystem.isNull())
    s_pPropSystem = qobject_cast<OdqPropSystem*>(getIApp()->getSingle(IqPropSystem_iid));
  ODA_ASSERT_ONCE(!s_pPropSystem.isNull()); // test

  return *s_pPropSystem;
}

//static 
QDockWidget* OdqPropSystem::getDockPalette() 
{
  return getPropSystem().m_pDockPalette.get(); 
}

// ----- IPropSystem -----

bool OdqPropSystem::isPropVisible() const
{
  bool bVisible = OdqPropSystem::getDockPalette()->isVisible();
  return bVisible;
}

ODGSPALETTE& OdqPropSystem::cacheColors(OdRxObject* pRxDatabase) const
{
  OdqPropTab* pPropTab = getPropTab(pRxDatabase);
  if (pPropTab)
    return pPropTab->cacheColors();
  
  ODA_FAIL_ONCE();
  static OdSharedPtr<ODGSPALETTE> s_pColors;
  if (s_pColors.isNull())
    s_pColors = new ODGSPALETTE;
  return *s_pColors;
}

// -----

// static 
OdqPropTab* OdqPropSystem::getPropTab(OdRxObject* pRxDatabase,
                                      bool bSetCurrentAndCreateIfMissing, // = false
                                      bool bCreateIfMissing) // = false
{
  if (bSetCurrentAndCreateIfMissing)
    bCreateIfMissing = true;

  QMap<OdRxObject*, OdSharedPtr<OdqPropTab> >::iterator itrTabs
    = getPropSystem().m_mapTabs.find(pRxDatabase);
  if (itrTabs != getPropSystem().m_mapTabs.end())
  {
    OdSharedPtr<OdqPropTab>& pTab = itrTabs.value();
    ODA_ASSERT_ONCE(!pTab.isNull());
    if (   bSetCurrentAndCreateIfMissing 
        && qobject_cast<OdqPropTab*>(getPropSystem().m_pTabWidget->currentWidget()) != pTab.get())
    {
      getPropSystem().m_pTabWidget->setCurrentWidget(pTab);
      if (pRxDatabase && getPropSystem().isPropVisible())
        // maybe non-updated as invisible
        getPropSystem().refresh("<event-refresh:selection_changed>", ""); 
    }
    return pTab;
  }
  if (!bCreateIfMissing)
    return NULL;

  itrTabs = getPropSystem().m_mapTabs.insert(pRxDatabase, OdSharedPtr<OdqPropTab>());
  ODA_ASSERT_ONCE(itrTabs != getPropSystem().m_mapTabs.end());

  OdSharedPtr<OdqPropTab>& pTab = itrTabs.value();
  ODA_ASSERT_ONCE(pTab.isNull());
  pTab = new OdqPropTab(getPropSystem().m_pTabWidget, getIConsole()->getCommandContext(pRxDatabase));
  ODA_ASSERT_ONCE(pRxDatabase == pTab->getRxDatabase());

  QWidget* pCurrTab = pTab;
  OdRxObjectPtr pCurDatabase(pRxDatabase);
  if (!bSetCurrentAndCreateIfMissing && getPropSystem().m_pTabWidget->currentWidget())
  {
    pCurrTab = getPropSystem().m_pTabWidget->currentWidget();
    pCurDatabase = ((OdqPropTab *) pCurrTab)->getRxDatabase(); 
  }
  //QString qsTabName(qsFilePathName);
  QString qsTabName = getIConsole()->getQTitle(pRxDatabase);

  // indexes in property palette tab widget 
  // should have the same order as in console.
  // but current should created the first if prop palette was reloaded.
  QList<OdRxObject*> lst = getIConsole()->getOpenDatabases();
  lst.push_front(NULL);
  int idxToInsert = lst.indexOf(pRxDatabase);
  int count = getPropSystem().m_pTabWidget->count();
  if (idxToInsert >= 0 && count && idxToInsert < count)
    getPropSystem().m_pTabWidget->insertTab(count - 1, pTab, qsTabName);
  else
    getPropSystem().m_pTabWidget->addTab(pTab, qsTabName);

  qApp->processEvents(); // show it

  if (pCurrTab)
    getPropSystem().m_pTabWidget->setCurrentWidget(pCurrTab);

  return pTab;
}

//////////////////////////////////////////////////////////////////////////

// slot
void OdqPropSystem::refresh(const QString& qsKey, const QString& qsArg) // refresh states of widgets
{
  ODA_ASSERT_ONCE(   qsKey == "<event-refresh:database_closed>"
                  || qsKey == "<event-refresh:database_changed>"
                  || qsKey == "<event-refresh:selection_changed>"); // test
  IqConsole* iConsole = getIConsole();
  if (!iConsole)
    return;
  OdRxObject* pRxDatabase = iConsole->getCurrentDatabase();
  if (qsKey == "<event-refresh:database_closed>")
  {
    OdqPropSystem::removePropTab(pRxDatabase);
    return;
  }
  if (qsKey == "<event-refresh:selection_changed>")
  {
    OdqPropTab* pTab = getPropTab(pRxDatabase);
    ODA_ASSERT_ONCE(pTab);
    if (pTab && pTab->isVisible())
      pTab->update(NULL, true, qsArg == "force");
    return;
  }

  OdqPropSystem::getPropTab(pRxDatabase, 
                            true); // set active
}

// slot
void OdqPropSystem::currentTabChanged(int index)
{
  ODA_ASSERT_ONCE(   !m_pTabWidget.isNull()
    && index == m_pTabWidget->currentIndex()); // already switched
  if (index < 0)
    return;

  OdqPropTab* pTab = qobject_cast<OdqPropTab*>(m_pTabWidget->widget(index));
  ODA_ASSERT_ONCE(pTab);
  OdRxObject* pRxDatabase = pTab->getRxDatabase();

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
bool OdqPropSystem::removePropTab(OdRxObject* pRxDatabase)
{
  QMap<OdRxObject*, OdSharedPtr<OdqPropTab> >::iterator itrTabs
    = getPropSystem().m_mapTabs.find(pRxDatabase);
  if (itrTabs == getPropSystem().m_mapTabs.end())
  {
    //ODA_FAIL_ONCE(); // test // tab was not found
    return true;
  }

  OdSharedPtr<OdqPropTab>& pTab = itrTabs.value();
  if (pTab.isNull())
  {
    ODA_FAIL_ONCE(); // test // tab already was removed
    return true;
  }

  QWidget *pTabToRestore = getPropSystem().m_pTabWidget->currentWidget();
  if (pTabToRestore != pTab)
    getPropSystem().m_pTabWidget->setCurrentWidget(pTab);
  else
    pTabToRestore = NULL;

  ODA_ASSERT_ONCE(pTab->parentWidget()->parentWidget() == getPropSystem().m_pTabWidget); 

  OdSharedPtr<OdqPropTab> pTabHolder(pTab); 
  pTab->uninit(); 
  pTab = NULL; // delete
  getPropSystem().m_mapTabs.remove(pRxDatabase);

  //Removes the page at position index from this stack of widgets. Does not delete the page widget
  getPropSystem().m_pTabWidget->removeTab(getPropSystem().m_pTabWidget->currentIndex());
  //qApp->processEvents(); /// show it

  // the next was moved upper to skip iConsole->setCurrentDatabase(pRxDatabase) 
  // via slot OdqPropSystem::currentTabChanged
  //
  //pTab = NULL; // delete
  //getPropSystem().m_mapTabs.remove(pRxDatabase);
  pTabHolder = NULL;

  if (pTabToRestore) // restore current tab
    getPropSystem().m_pTabWidget->setCurrentWidget(pTabToRestore);

  return true;
}

// static 
bool OdqPropSystem::removePropTab(OdqPropTab &tab)
{
  if (tab.parentWidget()) // look also <mark to delete tab with end of command>
  {
    ODA_FAIL_ONCE(); // test
    return false;
    // TODO
    //return removePropTab(tab.getRxDatabase());
  }
  ODA_ASSERT_ONCE(tab.getRxDatabase());

  OdRxObject* pRxDatabase = tab.getRxDatabase();

  QMap<OdRxObject*, OdSharedPtr<OdqPropTab> >::iterator itrTabs
    = getPropSystem().m_mapTabs.find(pRxDatabase);
  if (   itrTabs == getPropSystem().m_mapTabs.end()
    || itrTabs.value().isNull())
  {
    ODA_FAIL_ONCE(); // test // tab already was removed
    return false;
  }

  OdSharedPtr<OdqPropTab>& pTab = itrTabs.value();

  pTab = NULL; // delete
  getPropSystem().m_mapTabs.remove(pRxDatabase);
  return true;
}

//////////////////////////////////////////////////////////////////////////
