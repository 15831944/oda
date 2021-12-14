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
// OdqMainWindow.cpp
//

#include <QtCore>
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtGui>
#else
#include <QtWidgets>
#endif
#include <QDir>
#include <QSettings>
#include <QMetaProperty>

#include "OdqApplication.h" 
#include "OdqMainWindow.h"
#include "OdqCustomization.h"
#include "OdqClipData.h"
#include "Services.h"
#include "AppModule.h"
#include "ExtDbModule.h"
#include "PropServicesImpl.h"

#include "RxVariantValue.h"
#include "TDVersion.h"

//////////////////////////////////////////////////////////////////////////

//#define OD_ANDROID_TRACEDEFS

#ifdef OD_ANDROID_TRACEDEFS
#ifdef ANDROID
#include <android/log.h>
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,  "OdqMainWindow", __VA_ARGS__)
#else
#undef OD_ANDROID_TRACEDEFS
#endif
#endif

ODRX_DEFINE_STATIC_MODULE(OdqAppModule)

//////////////////////////////////////////////////////////////////////////

OdqMainWindow::OdqMainWindow()
  : m_bValid(false)
  , m_pAppModule(NULL)
  , m_bDragging(false)
{
  setWindowTitle(getIApp()->getAppName());
#ifndef __IPHONE_OS_VERSION_MIN_REQUIRED
  QPixmap pixmap(QLatin1String(":odalogo.png"));
  ODA_ASSERT_ONCE(!pixmap.isNull());
  pixmap.setMask(pixmap.createHeuristicMask());
  setWindowIcon(QIcon(pixmap));
#endif
  restoreWidgetPlacement(this, "", 1000, 600);

  setAcceptDrops(true);

  // create QMdiArea widget 
  // - provides an area in which MDI windows are displayed
  QMdiArea* pMdiArea = new QMdiArea();
  pMdiArea->setObjectName("MdiArea");
  setCentralWidget(pMdiArea);

  initAppModule(); // uninitAppModule() will be executed via closeEvent
  m_bValid = true;
}

OdqMainWindow::~OdqMainWindow()
{
  ODA_ASSERT(!m_pAppModule);
}

OdRxModule* OdqMainWindow::getAppModule()
{
  ODA_ASSERT(m_pAppModule);
  return m_pAppModule;
}

bool OdqMainWindow::initAppModule()
{
  qApp->setActiveWindow(this); // app.setActiveWindow(&window);

  // do not hold console here via OdRxModulePtr as member 
  OdRxModule* pConsole = ::odrxDynamicLinker()->loadModule(OD_T("OdaQtConsole.dll")).get(); // silent loading
  if (!pConsole)
  {
    // unfortunately toPlatformFileName is inaccessible for external use now
    //OdString sConsolePath;
    //app.getServices()->toPlatformFileName(OD_T("OdaQtConsole.dll"), &sConsolePath);
    //QString qsConsolePath = app.toQString(sConsolePath);
    QString qsConsolePath = "OdaQtConsole.dll";
    QMessageBox::critical(NULL, QObject::tr("Fatal error"),
      QObject::tr("File \"%1\" was not found.").arg(qsConsolePath));
    return false;
  }

  // preloading all modules with "ObjectDBX Classes"
  //::odrxDynamicLinker()->loadModule(OdDynBlocksModuleName);
  ::odrxDynamicLinker()->loadModule(DbConstraintsModuleName);
  //::odrxDynamicLinker()->loadModule(Od3DSolidHistoryTxModuleName);
#if defined(_DEBUG) && defined(ODA_WINDOWS) && !defined(_WINRT)
  if (false)
    ::odrxDynamicLinker()->loadModule(OdOleSsItemHandlerModuleName);
  else
#endif
    ::odrxDynamicLinker()->loadModule(OdOlePlatformItemHandlerModuleName);

  ODA_ASSERT(!m_pAppModule);
  m_pAppModule = ODRX_STATIC_MODULE_ENTRY_POINT(OdqAppModule)(OD_T("OdqAppModule"));
  m_pAppModule->initApp();

  // (void)
  // by OdaMfcApp
  new QShortcut(Qt::SHIFT + Qt::Key_C, this, SLOT(revertConsoleVisibility()));
  
  // by ACAD
  // Ctrl+9 shows/hides Command line (2006+)
  new QShortcut(Qt::CTRL + Qt::Key_9, this, SLOT(revertConsoleVisibility()));

  //Ctrl+Q quits AutoCAD (2004+)  
  //new QShortcut(QKeySequence(tr("Ctrl+Q", "File|Exit")), this);

  new QShortcut(Qt::Key_F1, this, SLOT(showHelp()));

  return true;
}

void OdqMainWindow::uninitAppModule()
{
  if (!m_pAppModule)
    return;

  m_pAppModule->uninitApp();
  m_pAppModule->deleteModule();
  m_pAppModule = NULL;

  OdPropServices* pPropServices = OdPropServices::cast(getIConsole()->getCommandContext()).get();
  if (pPropServices)
    ((OdPropServicesImpl*)pPropServices)->saveCatWeights();

  // it's here to save valid visibility of console palette via QSettings
  if (!::odrxDynamicLinker()->unloadModule(L"OdaQtConsole.dll"))
  {
    ODA_FAIL_ONCE();
  }
}

bool OdqMainWindow::isValid() const
{
  return m_bValid;
}

bool OdqMainWindow::isClosed() const // by closeEvent
{
  return !m_pAppModule;
}

//OdqServices* OdqMainWindow::getServices() const
//{
//  static OdqServices* s_pServices = NULL;
//  if (!s_pServices)
//  {
//    OdqApplication* pApp = (OdqApplication*) ::getIApp();
//    s_pServices = pApp->getServices();
//    ODA_ASSERT_ONCE(s_pServices);
//  }
//
//  return s_pServices;
//}

void OdqMainWindow::saveWidgetPlacement(QWidget* pWidget, 
                                        const QString cqsKey) const // = "" // for QMainWindow
{
  if (!pWidget)
    return;

  QSettings settings("ODA", 
                     QFileInfo(QCoreApplication::applicationFilePath()).baseName());
  settings.beginGroup(TD_SHORT_STRING_VER_S);

  QString qsKey(cqsKey);
  if (!qsKey.isEmpty())
    qsKey += "/";
  else
    ODA_ASSERT_ONCE(pWidget == this);

  QRect rect = pWidget->geometry();
  settings.setValue(qsKey + "w", rect.width());
  settings.setValue(qsKey + "h", rect.height());
  settings.setValue(qsKey + "x", rect.x());
  settings.setValue(qsKey + "y", rect.y());

  settings.endGroup();
}

void OdqMainWindow::restoreWidgetPlacement(QWidget* pWidget, 
                                           const QString cqsKey,  // = "" // for QMainWindow
                                           int widthDefault, // = 600
                                           int heightDefault) const // = 200
{
  if (!pWidget)
    return;

  QDesktopWidget* pDesktop = NULL;
  QSize sizeParentWnd = size();
  QPoint posParentWnd = pos();

  QString qsKey(cqsKey);
  if (!cqsKey.isEmpty())
    qsKey += "/";
  else
  {
    ODA_ASSERT_ONCE(pWidget == this);

    pDesktop = QApplication::desktop();
    sizeParentWnd = pDesktop->screenGeometry().size(); // primary screen
    posParentWnd = QPoint();
  }

  QSettings settings("ODA", 
                     QFileInfo(QCoreApplication::applicationFilePath()).baseName());
  settings.beginGroup(TD_SHORT_STRING_VER_S);

  QSize size(settings.value(qsKey + "w", widthDefault).toInt(), 
             settings.value(qsKey + "h", heightDefault).toInt());

  QPoint pos((sizeParentWnd.width() - size.width()) / 2,
             (sizeParentWnd.height() - size.height()) / 2);
  pos += posParentWnd;

  pos.setX(settings.value(qsKey + "x", pos.x()).toInt());
  pos.setY(settings.value(qsKey + "y", pos.y()).toInt());

  settings.endGroup();

  if (cqsKey.isEmpty())
  {
    ODA_ASSERT_ONCE(pDesktop);
    QSize sizeParentVirtualWnd = pDesktop->geometry().size(); // full virtual size of screens

    if (   size.width() < 400 || size.height() < 300 || pos.x() < 0 || pos.y() < 0
        || (pos.x() + size.width()) > sizeParentVirtualWnd.width() 
        || (pos.y() + size.height()) > sizeParentVirtualWnd.height())
    {
      // restore default application placement
      ODA_ASSERT_ONCE(pDesktop);
      sizeParentWnd = pDesktop->screenGeometry().size(); // primary screen
      size.setWidth(widthDefault);
      size.setHeight(heightDefault);
      pos.setX((sizeParentWnd.width() - widthDefault) / 2);
      pos.setY((sizeParentWnd.height() - heightDefault) / 2);
    }
  }

  pWidget->setGeometry(pos.x(), pos.y(), size.width(), size.height());
}

bool OdqMainWindow::saveQObjectProperties(QObject* pObj,
                                          const QString& qsProps, // = "" // an empty for all
                                          const QString& qsSkipProps, // = ""
                                          const QString& qsDelim) const // = " "
{
  const QMetaObject* pMeta = pObj->metaObject();
  ODA_ASSERT_ONCE(pMeta);
  if (!pMeta)
    return false;

  QString qsObjectName = pObj->objectName(),
          qsClassName = pMeta->className();
  ODA_ASSERT_ONCE(!qsObjectName.isEmpty());
  if (qsObjectName.isEmpty())
    return false;

  QSettings settings("ODA", 
                     QFileInfo(QCoreApplication::applicationFilePath()).baseName());
  settings.beginGroup(TD_SHORT_STRING_VER_S);
  
  QStringList lstProps,
              lstSkipProps;
  if (!qsProps.isEmpty() && !qsDelim.isEmpty())
    lstProps = qsProps.split(qsDelim, QString::SkipEmptyParts);
  if (!qsSkipProps.isEmpty() && !qsDelim.isEmpty())
    lstSkipProps = qsSkipProps.split(qsDelim, QString::SkipEmptyParts);

  bool bRes = false;
  for (int index = 0; index < pMeta->propertyCount(); index++)
  {
    QMetaProperty prop = pMeta->property(index);
    QString qsName = prop.name();
    if (qsName.isEmpty())
      continue;
    if (!prop.isStored()) // || !prop.isWritable() || !prop.isValid())
    {
      ODA_ASSERT_ONCE(!lstProps.size() || !lstProps.contains(qsName));
      continue;
    }
    QVariant qvValue = prop.read(pObj);
    ODA_ASSERT_VAR(if (!qvValue.toBool()))
      ODA_ASSERT_ONCE(true); // brk

    if (qsName != "objectName") // is used as indication of existence
    {
      if (lstProps.size() && !lstProps.contains(qsName))
        continue;
      if (qsSkipProps.size() && qsSkipProps.contains(qsName))
        continue;
      bRes = true;
    }
    //QString qsValue = qvValue.toString();
    //if (qsValue.isEmpty())
    //  continue;

    settings.setValue(QString("objectProps/%1/%2/%3").arg(qsClassName).arg(qsObjectName).arg(qsName), qvValue);
  }
  ODA_ASSERT_ONCE(bRes);
  
  settings.endGroup();
  return bRes;
}

bool OdqMainWindow::restoreQObjectProperties(QObject* pObj,
                                             const QString& qsProps, // = "" // an empty for all
                                             const QString& qsSkipProps, // = ""
                                             const QString& qsDelim) const // = " "
{
  const QMetaObject* pMeta = pObj->metaObject();
  ODA_ASSERT_ONCE(pMeta);
  if (!pMeta)
    return false;

  QString qsObjectName = pObj->objectName(),
          qsClassName = pMeta->className();
  ODA_ASSERT_ONCE(!qsObjectName.isEmpty());
  if (qsObjectName.isEmpty())
    return false;

  QSettings settings("ODA", 
                     QFileInfo(QCoreApplication::applicationFilePath()).baseName());
  settings.beginGroup(TD_SHORT_STRING_VER_S);

  if (settings.value(QString("objectProps/%1/%2/objectName").arg(qsClassName).arg(qsObjectName), 
                     "").toString() != qsObjectName)
  {
    settings.endGroup();
    return false;
  }

  QStringList lstProps,
              lstSkipProps;
  if (!qsProps.isEmpty() && !qsDelim.isEmpty())
    lstProps = qsProps.split(qsDelim, QString::SkipEmptyParts);
  if (!qsSkipProps.isEmpty() && !qsDelim.isEmpty())
    lstSkipProps = qsSkipProps.split(qsDelim, QString::SkipEmptyParts);

  bool bRes = false;
  for (int index = 0; index < pMeta->propertyCount(); index++)
  {
    QMetaProperty prop = pMeta->property(index);
    QString qsName = prop.name();
    ODA_ASSERT_ONCE(!qsName.isEmpty());
    if (qsName.isEmpty())
      continue;
    if (!prop.isStored()) // || !prop.isWritable() || !prop.isValid())
    {
      ODA_ASSERT_ONCE(!lstProps.size() || !lstProps.contains(qsName));
      continue;
    }
    if (lstProps.size() && !lstProps.contains(qsName))
      continue;
    if (qsSkipProps.size() && qsSkipProps.contains(qsName))
      continue;
    bRes = true;

    QVariant qvValue = settings.value(QString("objectProps/%1/%2/%3").arg(qsClassName)
                                                                     .arg(qsObjectName)
                                                                     .arg(qsName), "");
    //QString qsValue = qvValue.toString();
    //ODA_ASSERT_ONCE(!qsValue.isEmpty());
    //if (qsValue.isEmpty())
    //  continue;

    prop.write(pObj, qvValue);
  }
  ODA_ASSERT_ONCE(bRes);

  settings.endGroup();
  return bRes;
}

bool OdqMainWindow::showUrl(const QString& qsUrl,
                            const QString& cqsTitle, // = "%1"
                            bool bModal) // = true
{
  QUrl url(qsUrl);
  if (!url.isValid())
    return false;
  QString qsScheme = url.scheme();

  QString qsLocal = url.toLocalFile();
  if (!qsLocal.isEmpty() && !QFileInfo(qsLocal).exists())
    return false;

  if (qsScheme.isEmpty() || qsScheme == "qrc")
  {
    QFileInfo info(QString(qsUrl).replace("qrc:", ":"));
    if (!info.exists())
      return false;
  }

  QString qsTitle;
  if (!cqsTitle.isEmpty()){
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
    qsTitle = QObject::tr(QString(cqsTitle).arg(getIApp()->getAppName()).toAscii().data());
#else
    qsTitle = QObject::tr(QString(cqsTitle).arg(getIApp()->getAppName()).toLatin1().data());
#endif
  }

  if (bModal)
  {
    // Workaround for : macx : QTextBrowser close button unavailable on modal window
    QDialog dlg(this);
    if (!qsTitle.isEmpty())
      dlg.setWindowTitle(qsTitle);
    restoreWidgetPlacement(&dlg, "modal", 600, 350);
   // dlg.setWindowState(Qt::WindowMaximized);

    QVBoxLayout* pLayout = new QVBoxLayout(&dlg);

    OdSharedPtr<QTextBrowser> sv_pBrowser = m_pBrowser;

    m_pBrowser = new QTextBrowser(&dlg);
    m_pBrowser->setFrameStyle(QFrame::NoFrame);
    m_pBrowser->setReadOnly(true);  
    m_pBrowser->setAttribute(Qt::WA_DeleteOnClose, false);
    connect(m_pBrowser, SIGNAL(sourceChanged(QUrl)), 
            this, SLOT(changedBrowserSource(QUrl)), Qt::QueuedConnection);
    connect(m_pBrowser, SIGNAL(anchorClicked(QUrl)), 
            this, SLOT(clickedBrowserAnchor(QUrl)));
    m_pBrowser->setSource(url);
    pLayout->addWidget(m_pBrowser);

    QPushButton* pCloseButton = new QPushButton(tr("OK"));
    pCloseButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    pCloseButton->setMinimumSize(100, pCloseButton->minimumSize().height());
    connect(pCloseButton, SIGNAL(clicked()),
            this, SLOT(clickedBrowserCloseButton()));
    pLayout->addWidget(pCloseButton,
                       0, //stretch
                       Qt::AlignHCenter); // alignment
  //  dlg.adjustSize();
    dlg.exec();

    saveWidgetPlacement(&dlg, "modal");
    dlg.close();

    m_pBrowser = sv_pBrowser;
    return true;
  }

  if (m_pBrowser.isNull())
  {
    m_pBrowser = new QTextBrowser(this);
    m_pBrowser->setWindowIcon(windowIcon());
    m_pBrowser->setReadOnly(true);  
    restoreWidgetPlacement(m_pBrowser, "browser", 
                           700, 400); // size at first start
  }
  m_pBrowser->setWindowModality(Qt::NonModal);

  if (!cqsTitle.isEmpty())
    m_pBrowser->setWindowTitle(qsTitle);

  m_pBrowser->setParent(NULL, Qt::Dialog);
  //m_pBrowser->setAttribute(Qt::WA_DeleteOnClose);

  connect(m_pBrowser, SIGNAL(sourceChanged(QUrl)), 
          this, SLOT(changedBrowserSource(QUrl)), Qt::QueuedConnection);
  connect(m_pBrowser, SIGNAL(anchorClicked(QUrl)), 
          this, SLOT(clickedBrowserAnchor(QUrl)));
  m_pBrowser->setSource(url);

  m_pBrowser->show();
  return true;
}

bool OdqMainWindow::isBrowserClosed() const
{
  if (m_pBrowser.isNull())
    return true;
  QDialog* pParent = qobject_cast<QDialog*>(m_pBrowser->parent());
  ODA_ASSERT_ONCE(pParent || m_pBrowser->windowModality() == Qt::NonModal);
  if (!pParent || !pParent->isVisible())
    return true;

  return false;
}

//void OdqMainWindow::showEvent(QShowEvent* pEvent)
//{
//  QMainWindow::showEvent(pEvent);
//}
//
//void OdqMainWindow::paintEvent(QPaintEvent* pEvent)
//{
//  QMainWindow::paintEvent(pEvent);
//}

void OdqMainWindow::closeEvent(QCloseEvent* pEvent)
{
  //SEA crush on exit workaround
  if( !getIConsole() )
  {
    return;
  }
  QList<OdRxObject*> lst = getIConsole()->getOpenDatabases();
  if (!lst.count())
  {
    saveWidgetPlacement(this);
    saveWidgetPlacement(m_pBrowser.get(), "browser");
    OdqApplication* pApp = qobject_cast<OdqApplication*>(qApp);
    pApp->getCustom()->saveSettings();
    QMdiArea* pMdiArea = qobject_cast<QMdiArea*>(centralWidget());
    ODA_ASSERT_ONCE(pMdiArea);
    if (pMdiArea && !pMdiArea->activeSubWindow())
    {
      {
        // unload custom unreferenced modules
        // (possible crash without it (via references 
        //  to overloaded command of console like ERASE - see ExCustObjs.tx : ExCustObjsModule::uninitApp() : pPrevErase
        OdRxModulePtr pHolderConsole = ::odrxDynamicLinker()->getModule(L"OdaQtConsole.dll");
        OdRxModulePtr pHolderDgnDb = ::odrxDynamicLinker()->getModule(L"TD_DgnDb.tx"); // TODO move this holder in real needed place
        OdRxModulePtr pHolderBimDb = ::odrxDynamicLinker()->getModule(L"TD_BimDb.tx");
        OdRxModulePtr pHolderTigDb = ::odrxDynamicLinker()->getModule(L"TD_TigDb.tx");
        ::odrxDynamicLinker()->unloadUnreferenced();
      }  
      // it's here to save valid visibility of console palette via QSettings
      uninitAppModule();
      QMainWindow::closeEvent(pEvent);
      return;
    }
  }

  if(lst.count() && !getIConsole()->isMarkedToClose(lst.first()))
    getIConsole()->postponeCommand("^C^C quit"); //getIConsole()->postponeCommand("quit", true, NULL, 100);

  pEvent->ignore();
}

void OdqMainWindow::dragEnterEvent(QDragEnterEvent* pEvent) 
{
  if (m_bDragging)
  {
    QMainWindow::dragEnterEvent(pEvent);
    pEvent->acceptProposedAction();
    return;
  }

  const QMimeData *pData = pEvent->mimeData();
  if (!pData)
  {
    pEvent->ignore();
    return;
  }

  OdRxObject* pRxDatabase = getIConsole()->getCurrentDatabase();
  if (!OdqClipboardData::isAcadDataAvailable(pData, pRxDatabase))
  {
    if (!pData->hasUrls())
    {    
      pEvent->ignore();
      return;
    }
    QList<QUrl> files = pData->urls();
    bool bRes = false;
    foreach(QUrl url, files)
    {
      if (url.isEmpty())
        continue;
      QString qsFileName = url.toLocalFile();
      OdString sExt = getIApp()->toOdString(QFileInfo(qsFileName).suffix().toLower()); // .dgn .rfa .rvt .dwf ...
      if (!isAcadExtension(sExt) && !getIApp()->getExtDbModuleByExtension(sExt))
          continue;
      bRes = true;
    }
    if (bRes)
      pEvent->acceptProposedAction(); // pEvent->accept();
    else
      pEvent->ignore();
    return;
  }

  //if (getIConsole()->activeCommandState(pRxDatabase) != IqConsole::enCmdActive_No)
  //{
  //  if (m_bDragging)
  //    pEvent->acceptProposedAction();
  //  return;
  //}

  if (!OdqClipboardData::copyToClipboard(pData, pRxDatabase))
  {
    ODA_FAIL_ONCE(); // test
    return;
  }

  m_bDragging = true;
  pEvent->acceptProposedAction();
}

//void OdqMainWindow::dragMoveEvent(QDragMoveEvent* pEvent)
//{
//  pEvent->acceptProposedAction();
//
//
//  //if (!m_bDragging)
//  //{
//    QMainWindow::dragMoveEvent(pEvent);
//    return;
//  //}
//
//  //pEvent->acceptProposedAction();
//  // TODO
//}

//void OdqMainWindow::dragLeaveEvent(QDragLeaveEvent* pEvent)
//{
//  //if (!m_bDragging)
//  //{
//  //  QMainWindow::dragLeaveEvent(pEvent);
//  return;
//  //}
//
//  //m_bDragging = false;
//  //OdRxObject* pRxDatabase = getIConsole()->getCurrentDatabase();
//  //if (getIConsole()->activeCommandName(pRxDatabase).makeLower().find(OD_T("pasteclip")))
//  //  getIConsole()->markToCancel(pRxDatabase);
//}

void OdqMainWindow::dropEvent(QDropEvent* pEvent)
{
  if (!m_bDragging)
  {
    const QMimeData *pData = pEvent->mimeData();
    if (!pData || !pData->hasUrls())
    {    
      pEvent->ignore();
      return;
    }

    QList<QUrl> files = pData->urls();
    QString qsCmd;
    foreach(QUrl url, files)
    {
      QString qsFileName = url.toLocalFile();
      OdString sExt = getIApp()->toOdString(QFileInfo(qsFileName).suffix().toLower()); // .dgn .rfa .rvt .dwf ...
      if (!isAcadExtension(sExt) && !getIApp()->getExtDbModuleByExtension(sExt))
        continue;

      //if (!qsCmd.isEmpty())
      //  qsCmd += "\n";
      qsCmd.clear();
      qsCmd += QString("open \"%1\"").arg(qsFileName);
      getIConsole()->postponeCommand(qsCmd, true, (OdRxObject*) -1);
    }
    if (!qsCmd.isEmpty())
    {
      //getIConsole()->postponeCommand(qsCmd, true, (OdRxObject*) -1);
      pEvent->acceptProposedAction(); // pEvent->accept();
      return;
    }

    //pEvent->ignore();
    QMainWindow::dropEvent(pEvent);
    return;
  }
  IqViewSystem* iVS = getIViewSystem();
  if (!iVS)
    return;

  QPoint point(pEvent->pos().x(), pEvent->pos().y());
  point = mapToGlobal(point); // to screen coords

  IqView* iView = iVS->getViewByScreenPoint(point, // to view coords
                                            true); // do Active
  if (!iView)
    return;
  OdRxObject* pRxDatabase = iView->getRxDatabase();
    
  QString qsCmd = "dropgeom";

  //if (pRxDatabase == getIConsole()->getDatabaseDragSource())
  {
    OdGePoint3d pt;
    if (!iView->eyesTo(point, 0, pt))
      return;
    qsCmd += QString(" %1,%2").arg(pt.x).arg(pt.y);
    if (!OdZero(pt.z))
      qsCmd += QString(",%1").arg(pt.z);
  }
  getIConsole()->executeCommand(qsCmd, false, pRxDatabase);
  if (pRxDatabase == getIConsole()->getDatabaseDragSource())
    getIConsole()->updateAllGrips(pRxDatabase);

  m_bDragging = false;
}

//slot
void OdqMainWindow::revertConsoleVisibility()
{
  getIConsole()->postponeCommand(getIConsole()->isConsoleVisible() ? "commandlinehide" 
                                                                   : "commandline");
}

//slot
void OdqMainWindow::showHelp()
{
  getIConsole()->postponeCommand("help");
}

//slot
void OdqMainWindow::changedBrowserSource(const QUrl & url)
{
  //QString qsPath = url.path();
  QString qsText = m_pBrowser->toHtml();
  QString qsCopyrightDD = getIApp()->toQString(TD_COPYRIGHT_W);
  qsCopyrightDD.replace("Copyright ", "Copyright"); // remove useless space
  QString qsCompanyNameDD = qsCopyrightDD;
  qsCompanyNameDD.remove(" ('Open Design')", Qt::CaseInsensitive);
  qsText.replace("$(TD_COMPANY_NAME)", qsCompanyNameDD, Qt::CaseInsensitive);
  //qsCopyrightDD.replace("\xA9", "&copy;");
  //qsCopyrightDD.replace(" Inc. ('Open Design')", ".");
  //qsCopyrightDD.replace("2012", "2013"); // temp solution // TODO: remove after realese v3.? !!!
  qsText.replace("$(TD_COPYRIGHT)", qsCopyrightDD, Qt::CaseInsensitive);
  
  qsText.replace("$(TD_PRODUCT_NAME)",  getIApp()->toQString(TD_PRODUCT_NAME_W), Qt::CaseInsensitive);
  qsText.replace("$(TD_PRODUCT_NAME_START)",  getIApp()->toQString(TD_PRODUCT_NAME_START_W), Qt::CaseInsensitive);
  // In the About box, the TM symbol for ODA SDK needs to change to ®. // From: Allison Angus February 21, 2011
  //qsText.replace("$(TD_PRODUCT_NAME)", QString(TD_PRODUCT_NAME_S).replace("ODA SDK™", "ODA SDK&reg;"), Qt::CaseInsensitive);
  //qsText.replace("$(TD_PRODUCT_NAME_START)", QString(TD_PRODUCT_NAME_START_S).replace("ODA SDK™", "ODA SDK&reg;"), Qt::CaseInsensitive);
  
  qsText.replace("$(TD_APP_NAME)", getIApp()->getAppName(), Qt::CaseInsensitive);
  qsText.replace("$(TD_EXE_NAME)", QFileInfo(QCoreApplication::applicationFilePath()).baseName(), Qt::CaseInsensitive);

  //QString qsVerTD =  TD_PRODUCT_VER_STR_S; // TD_SHORT_STRING_VER_S // "3.02"
  //qsVerTD.replace(", ", "."); // for common style with Qt (same with DD short ver style)
  QString qsVerTD = QString("%1.%2.%3").arg(TD_MAJOR_VERSION).arg(TD_MINOR_VERSION).arg(TD_MAJOR_BUILD);
 #if TD_MINOR_BUILD > 0
  qsVerTD += QString(".%1").arg(TD_MINOR_BUILD)
 #endif
  qsText.replace("$(TD_VER_STR)", qsVerTD, Qt::CaseInsensitive);
  qsText.replace("$(TD_SHORT_VER_STR)", TD_SHORT_STRING_VER_S, Qt::CaseInsensitive);
  qsText.replace("$(QT_VER_STR)", QT_VERSION_STR, Qt::CaseInsensitive); // "4.5.3"
  if (qsText.indexOf("$(CAUSTIC", Qt::CaseInsensitive) >= 0)
  {
    OdGsModulePtr pCausticGs = ::odrxDynamicLinker()->loadModule(OD_T("OdaQtOpenRL.txv"));
   #ifdef Q_CC_MSVC
    if (pCausticGs.isNull())
      pCausticGs = ::odrxDynamicLinker()->loadModule(OD_T("WinOpenRL.txv"));
   #endif
    if (!pCausticGs.isNull())
    {
      OdRxDictionaryPtr pProperties = pCausticGs->createDevice()->properties();
      int index = -1;
      QStringList lstMacro;
      lstMacro << "$(CAUSTIC_VER_STR)" << "$(CAUSTIC_VER_STR)"
               << "$(CAUSTIC_COMPANY_NAME)" << "$(CAUSTIC_COMPANY_NAME)"
               << "$(CAUSTIC_PRODUCT_NAME)" << "$(CAUSTIC_PRODUCT_NAME)";
                                                // more priority    -       less priority
      foreach (QString qsProp, QStringList() << "OpenRLProductVersionVI" << "OpenRLVersion" 
                                             << "OpenRLCompanyVI"        << "OpenRLVendor" 
                                             << "OpenRLProductNameVI"    << "OpenRLRenderer")
      {
        OdString sProp = getIApp()->toOdString(qsProp);
        QString qsMacro = lstMacro.at(++index);
        if (pProperties->has(sProp))
        {
          QString qsValue = getIApp()->toQString(OdRxVariantValue(pProperties->getAt(sProp))->getString());
          if (!qsValue.isEmpty())
            qsText.replace(qsMacro, qsValue, Qt::CaseInsensitive);
        }
      }
    }
    else
      ODA_FAIL_ONCE();
  }

  // set invisible background color
  // (unfortunately m_pBrowser->setTextBackgroundColor is not usable for it)
  QPalette palette = qApp->palette();
  QColor colorSysGrey = palette.color(QPalette::Normal, QPalette::Window); //Base);
  int red = colorSysGrey.red(), // 236
      green = colorSysGrey.green(), // 233
      blue = colorSysGrey.blue(); // 216
  QString qsSysGrey = QString("bgcolor=\"#%1%2%3\"").arg(red, 2, 16, QLatin1Char('0'))
                                                    .arg(green, 2, 16, QLatin1Char('0'))
                                                    .arg(blue, 2, 16, QLatin1Char('0'));
  qsSysGrey = qsSysGrey.toUpper();
  qsText.replace("bgcolor=\"#ffffff\"", qsSysGrey, Qt::CaseInsensitive);
  m_pBrowser->setHtml(qsSysGrey);

  m_pBrowser->setHtml(qsText);
}

//slot
void OdqMainWindow::clickedBrowserAnchor(const QUrl& url)
{
  QString qsScheme = url.scheme();
  QString qsLocal = url.toLocalFile();
  QString qsCommand = url.toString();
  if (!qsCommand.indexOf("cmd:/"))
  {
    //https://stackoverflow.com/questions/3700257/how-to-retrieve-the-string-of-a-qurl-with-space-in-it
    ODA_ASSERT_VAR(if (qsScheme.isEmpty()))
      qsScheme = "cmd";
  }

  if (!qsLocal.isEmpty() || qsScheme.isEmpty() || qsScheme == "qrc")
  {
    QString qsUrl = qsLocal;
    if (qsUrl.isEmpty())
    {
      qsUrl = url.toString();
      qsUrl.replace("qrc:", ":");
      m_pBrowser->setSource(url);
    }
    if (QFileInfo(qsUrl).exists())
      return;
  }
  if (qsScheme == "http")
  {
    // open with standard OS browser
    if (!QDesktopServices::openUrl(url))
    {
      ODA_FAIL_ONCE(); 
    }
  }
  if (qsScheme == "cmd")
  {
    IqConsole* iConsole = getIConsole();
    if (iConsole)
    {
      if (qsCommand.isEmpty())
      {
        // solution for spaces 
        qsCommand = url.toEncoded(QUrl::RemoveScheme);
        qsCommand = qsCommand.mid(2);
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
        qsCommand = QUrl::fromPercentEncoding(qsCommand.toAscii());
#else
        qsCommand = QUrl::fromPercentEncoding(qsCommand.toLatin1());
#endif
      }
      else
      {
        //qsCommand = url.toString(QUrl::RemoveScheme | QUrl::RemoveFilename | QUrl::StripTrailingSlash); //  QUrl::FullyEncoded); // | QUrl::RemoveScheme);
        int pos = qsCommand.indexOf("//");
        if (pos >= 0)
          pos += 2;
        else if ((pos = qsCommand.indexOf("/")) >= 0)
          pos++;
        if (pos > 0)
          qsCommand = qsCommand.mid(pos);
      }
      OdChar xA0('\x00a0'); xA0 &= 0xff; // strange but without it xA0 contains 0xFFA0
      qsCommand.replace(xA0, OdChar(' ')); // instead // qsCommand.replace("&nbsp;", " ");

      // # Clicking "recover" in the help file to go into a loop
      if (qsCommand != m_qsBrowserLastCommand)
      {
        m_qsBrowserLastCommand = qsCommand;
        //m_pBrowser->blockSignals(true);
        getIConsole()->executeCommand(qsCommand);
        //m_pBrowser->blockSignals(false); 
        QTimer::singleShot(100, this, SLOT(unlockCommandViaBrowser()));
      }
      //else
      //  m_qsBrowserLastCommand,clear();
    }
    //int posV = m_pBrowser->verticalScrollBar()->value();
    //m_pBrowser->reload(); //m_pBrowser->setSource(m_pBrowser->source());
    //m_pBrowser->verticalScrollBar()->setValue(posV);
    //return;
  }
  m_pBrowser->setSource(m_pBrowser->source());
}

//slot
void OdqMainWindow::clickedBrowserCloseButton()
{
  ODA_ASSERT_ONCE(!m_pBrowser.isNull());

  // ways to close modal dialog on mac
  QDialog* pParent = qobject_cast<QDialog*>(m_pBrowser->parent());
  ODA_ASSERT_ONCE(pParent || m_pBrowser->windowModality() == Qt::NonModal);
  if (pParent)
    pParent->close();
  else
    m_pBrowser->close();
}

//slot
void OdqMainWindow::unlockCommandViaBrowser()
{
  m_qsBrowserLastCommand.clear(); 
}
