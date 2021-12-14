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
// OdqApplication.cpp
//

#include <QtCore>
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtGui>
#else
#include <QtWidgets>
#endif
#include <QDir>
#include <QMessageBox>
#include <QRegExp>
#include <QStringList>
#include <QDesktopServices>
#include <QThread>

#include "OdqApplication.h" 
#include "OdqMainWindow.h"
#include "OdqCustomization.h"
#include "OdqAppProps.h" 
#include "Services.h"
#include "ExtDbModule.h"
#include "TDVersion.h"

#if defined(Q_OS_LINUX)
  #include <signal.h>
  #include <QProcess>
#endif

//////////////////////////////////////////////////////////////////////////

static OdStaticRxObject<OdqServices> s_services;
static QPointer<QMainWindow> s_pMainWindow;
static OdSharedPtr<OdqAppProps> s_pAppProps;

OdqApplication::OdqApplication(int& argc, char** argv)
  : QApplication(argc, argv)
  , m_modeMeter(0) // will be set by setMeterLimit
  , m_curMeter(0)
  , m_dumpedMeter(0)
  , m_maxMeter(0) // uninitialize
{
  try
  {
    s_services.init(); // after using OdStaticRxObject should be out of constructor / destructor
  }
  catch (OdError& err)
  {
    QString qsError = toQString(err.description());
    QMessageBox::information(QApplication::activeWindow(),
                             QObject::tr("Error"), qsError, QMessageBox::Yes);
    throw err;
  }

  ODA_ASSERT_ONCE(s_pAppProps.isNull());
  registerQObject(IqAppProps_iid, (s_pAppProps = new OdqAppProps()));

#if defined(Q_OS_WIN) || defined(Q_WS_WIN) //_MSC_VER
  // asked on forum to open drawing in ODA File Viewer programmically
  QFileInfo info(QCoreApplication::applicationFilePath());
  QString qsAppPath = info.absoluteFilePath(),
          qsKey = "HKEY_CURRENT_USER\\Software\\ODA\\Windows\\CurrentVersion\\App Paths\\";
  qsKey += info.baseName() + "." + info.suffix();
  QSettings settings(qsKey, QSettings::NativeFormat);
  QString qsAppPathLast = settings.value("Path", "").toString();
  if (qsAppPathLast != qsAppPath)
    settings.setValue("Path", qsAppPath);
#endif
}

OdqApplication::~OdqApplication()
{
  ODA_ASSERT_ONCE(!s_pAppProps.isNull());
  s_pAppProps->save();
  unregisterQObject(IqAppProps_iid, s_pAppProps);
  s_pAppProps = NULL;

  // was moved int OdqConsoleModule::uninitApp() just after uninitOdSysVarPE()
  //OdRxModule* pDgnDb = ::odrxDynamicLinker()->loadModule(OD_T("TD_DgnDb.tx")).get();
  //if (pDgnDb)
  //{
  //  //OdDgnDbModule_cast(pDgnDb)->setBaseHostAppServices(NULL);
  //  if (!::odrxDynamicLinker()->unloadModule(OD_T("TD_DgnDb.tx")))
  //  {
  //    ODA_FAIL_ONCE();
  //  }
  //}

  m_pCustom = NULL;
  m_mapRegObjects.clear();
  s_services.uninit(); // after using OdStaticRxObject should be out of constructor / destructor

  //Solution for segmentation fault at close application if an any drawing was opened On Linux.
  //(With first using of QGlWidget from OdqViewSystem txv invokes qAddPostRoutine(qt_gl_choose_cmapp)
  //from QtOpenGL to clear this static data by close application(= in destructor ofQApplication - see
  //qt_call_post_routines()). Therefore we should keep QtOpenGL from application to prevent it 
  //unloading through OdqViewSystem.)
  //
  // now += opengl added into pri-file of OdaQtApp for Linux
  //
 #if defined(Q_OS_LINUX) && !defined(QT_DEBUG) && (TD_VERSION < 30600) // && defined(_TOOLKIT_IN_DLL_)
  // since v3.6.0 ODA_TTF_SHARED will be defined in build/platforms/lnxX*_*.*dll.cfg
  // to fix / minimize problem of using 2 freetype versions at once on Linux. 
  // Qt uses libfreetype.so (OpenSUSE 11.2) or libfreetype.6.0.0(Ubuntu 10.10).
  // (see: #error "TODO define ODA_TTF_SHARED in build/platforms/lnxX*_*.*dll.cfg for v3.6.0 and later.")
  //
  // for ubuntu 10.10 
  // Segmentation fault during QApplication destructor via
  // conflict of our libFreeType.a (old vers of freetype) with 
  // standard freetype6 (v >=2.4 - used by Qt /usr/lib/libfreetype.so.6.6.0)
  //
  // the next is temp solution only till static old version is in using.
  //if (OdqApplication::getLinuxInfo().toLower().indexOf("ubuntu") >= 0)
    ::raise(SIGQUIT); //return;
 #endif
}

// static 
QString OdqApplication::getLinuxInfo()
{
 #if !defined(Q_OS_LINUX)
  return "";
 #else
  static QString s_qsInfo;
  if (s_qsInfo.isEmpty())
  {
    QProcess proc;
    proc.start("uname -a");
    proc.waitForFinished();
    s_qsInfo = QString::fromLocal8Bit(proc.readAllStandardOutput());
  }
  return s_qsInfo;
 #endif
}

QString OdqApplication::getAppName() const
{
  static QString s_qsAppName;
  if (s_qsAppName.isEmpty())
  {
    s_qsAppName = QFileInfo(QCoreApplication::applicationFilePath()).baseName();
    if (!s_qsAppName.indexOf("ODA_OpenRL_Demo", Qt::CaseInsensitive))
    {
      s_qsAppName = tr("ODA OpenRL Demo");
      qApp->setApplicationName(s_qsAppName);
    }
    else
    {
      QStringList namesInternal; 
      namesInternal << "ODADrawingsExplorer" << "ODE"
                    << "OdaViewer" << "TV" << "OdaQtApp";
      foreach(QString qsAppName, namesInternal)
      {
        if (s_qsAppName.toLower().indexOf(qsAppName.toLower()))
            continue;

        int len = qsAppName.length();
        s_qsAppName = s_qsAppName.mid(len).trimmed();
        if (!s_qsAppName.isEmpty())
          s_qsAppName = " " + s_qsAppName;
        s_qsAppName = tr("ODA Drawings Explorer") + s_qsAppName;
        qApp->setApplicationName(s_qsAppName);
        break;
      }
    }
  }
  return s_qsAppName;
}

//static 
QMainWindow* OdqApplication::getMainWnd()
{
  //static QPointer<QMainWindow> s_pMainWindow;
  if (s_pMainWindow.isNull())
    s_pMainWindow = qobject_cast<QMainWindow*>(qApp->activeWindow());

  ODA_ASSERT_ONCE(!s_pMainWindow.isNull()); // test
  return s_pMainWindow;
}

QMainWindow* OdqApplication::getMainWindow() const
{
  return OdqApplication::getMainWnd();
}

OdDbBaseHostAppServices* OdqApplication::getBaseHostAppServices() const
{
  return &s_services;
}

OdqCustomization* OdqApplication::getCustom() const
{
  if (m_pCustom.isNull())
    m_pCustom = new OdqCustomization();
  return m_pCustom.get();
}

bool OdqApplication::registerQObject(const QString& cqsKeys, QObject* pObj)
{
  ODA_ASSERT_ONCE(pObj);

  QString qsKeys = cqsKeys.toLower() + "|";
  qsKeys.replace(" ", "");

  bool bRes = !qsKeys.isEmpty();
  while (!qsKeys.isEmpty())
  {
    int pos = qsKeys.indexOf("|");
    QString qsKey = qsKeys.left(pos);
    ODA_ASSERT_ONCE(!qsKey.isEmpty());
    qsKeys = qsKeys.mid(pos + 1);
    
    if (qsKey.indexOf("[]") < 0 && qsKey.indexOf("<event-") < 0)
    {
      // single mode
      if (!m_mapRegObjects.count(qsKey))
        m_mapRegObjects.insert(qsKey, pObj);
      else if (((QObject*) m_mapRegObjects.values(qsKey).first()) != ((QObject*) pObj))
      {
        ODA_FAIL_ONCE();
        bRes = false;
      }
      continue;
    }

    // multi mode (TODO for refresh events etc)

    QList<QPointer<QObject> > values = m_mapRegObjects.values(qsKey);
    bool bDuplicate = false;
    foreach (QPointer<QObject> pObject, values)
    {
      if (((QObject*) pObject) != ((QObject*) pObj))
        continue;
      //ODA_FAIL_ONCE();
      bDuplicate = true;
      break;
    }
    if (!bDuplicate)
      m_mapRegObjects.insert(qsKey, pObj);

  } // end while

  return bRes;
}

bool OdqApplication::unregisterQObject(const QString& cqsKeys,
                                       QObject* pObj) // = NULL
{
  QString qsKeys = cqsKeys.toLower() + "|";
  qsKeys.replace(" ", "");

  bool bRes = !qsKeys.isEmpty();
  while (!qsKeys.isEmpty())
  {
    int pos = qsKeys.indexOf("|");
    QString qsKey = qsKeys.left(pos);
    ODA_ASSERT_ONCE(!qsKey.isEmpty());
    qsKeys = qsKeys.mid(pos + 1);

    if (pObj)
    {
      QMultiHash<QString, QPointer<QObject> >::iterator itr = m_mapRegObjects.find(qsKey);
      while (itr != m_mapRegObjects.end() && itr.key() == qsKey)
      {
        QMultiHash<QString, QPointer<QObject> >::iterator prev = itr;
        itr++;
        QPointer<QObject> pObj_ = prev.value();
        if (pObj == pObj_)
          m_mapRegObjects.erase(prev);
        else
          ODA_ASSERT_ONCE(qsKey.indexOf("[]") >= 0 || qsKey.indexOf("<event-") >= 0); // multi mode (TODO for refresh events etc)
      } 
      continue;
    }

    m_mapRegObjects.remove(qsKey);

  } // end while

  return bRes;
}

QList<QPointer<QObject> > OdqApplication::getQObjects(const QString& cqsKey) const
{
  QString qsKey = cqsKey.toLower();
  qsKey.replace(" ", "");
  ODA_ASSERT_ONCE(!qsKey.isEmpty() && qsKey.indexOf("|") < 0);
  
  return m_mapRegObjects.values(qsKey);
}

QObject* OdqApplication::getSingle(const QString& qsKey) const
{
  ODA_ASSERT_ONCE(qsKey.indexOf("[]") < 0);
  QList<QPointer<QObject> > lst = getQObjects(qsKey);

  QObject* pObject = NULL;
  if (lst.size() == 1)
    pObject = lst.takeFirst();
  else
    ODA_ASSERT_ONCE(   !lst.size()  // test
                    && (   qsKey == IqConsole_iid || qsKey == IqViewSystem_iid
                        || qsKey == IqPropSystem_iid || qsKey == IqRevSystem_iid)); 
  return pObject;
}

void OdqApplication::refreshQObjetcts(const QString& qsKey, // event : "<event-refresh:database_changed>" etc
                                      const QString& qsArg, // = ""
                                      const char* pcszMember) // = SLOT(refresh(const QString&, const QString&))
{
  QList<QPointer<QObject> > lst = getQObjects(qsKey);
  foreach (QPointer<QObject> pObject, lst)
  {
    if (pObject.isNull())
    {
      // possible for example via force prop module unloading 
      //ODA_FAIL_ONCE();s
      continue;
    }

    if (!connect(this, SIGNAL(refreshSignal(const QString&, const QString&)), pObject, pcszMember))
    {
      ODA_FAIL_ONCE(); // test
      continue;
    }
    emit refreshSignal(qsKey, qsArg);

    disconnect(this, SIGNAL(refreshSignal(const QString&, const QString&)), pObject, pcszMember);
  }
}

static bool isCurrentThread()
{
  QMainWindow* pMainWindow = OdqApplication::getMainWnd();
  if (!pMainWindow || !pMainWindow->statusBar())
  {
    ODA_FAIL_ONCE(); // test
    return false;
  }
  QThread *currentThread = QThread::currentThread();
  QThread *thr = pMainWindow->thread();
  if (currentThread != thr)
    return false;
  return true;
}

void OdqApplication::toStatusBar(const QString& qsText, // = ""
                                 int timeout) // = 0
{
  if (!isCurrentThread())
    return; // brk via any of DWG attachment of CORE-14571 // TODO
  getMainWnd()->statusBar()->showMessage(qsText, timeout);
}

void OdqApplication::setMeterLimit(unsigned int max) // = 100 // specify the maximum number of times to increment
{
  bool bStartBeforeSetLimit = false;
  m_modeMeter = getIAppProps()->toInt(L"Vars/meter", 0);
  if (GETBIT(m_modeMeter, 1) && isCurrentThread())
  {
    if (!m_pMeter.isNull())
    {
      bStartBeforeSetLimit = !m_maxMeter;
      if (!bStartBeforeSetLimit && m_pMeter->isVisible())
      {
        if (m_curMeter < m_maxMeter)
          m_pMeter->setValue(m_curMeter = m_maxMeter);
        m_sMeterTextToHide = L"<$>"; // marker
        m_pMeter->setVisible(false);
      }
    }
  }

  m_curMeter = m_dumpedMeter = 0;
  m_maxMeter = max ? max : 100;

  if (GETBIT(m_modeMeter, 2))
    getIConsole()->putTrace(OdString().format(L"\n%lsMeter Limit: %d", 
                                              m_sMeterPrefix.c_str(), m_maxMeter));

  if (!GETBIT(m_modeMeter, 1) || !isCurrentThread() || m_pMeter.isNull())
    return;

  m_pMeter->setRange(m_curMeter, m_maxMeter);
  bool bVisible = bStartBeforeSetLimit;
  if (bVisible != m_pMeter->isVisible())
  {
    //m_pMeter->setTextVisible(bVisible);
    m_pMeter->setVisible(bVisible);
  }
}

void OdqApplication::startMeter(const QString& qsText) // = "" // show progress bar (zero level)
{
  bool bStartBeforeSetLimit = !m_maxMeter || m_curMeter == m_maxMeter;
  if (bStartBeforeSetLimit)
  {
    // setLimit after start - (DWG/DGN) way
    m_modeMeter = getIAppProps()->toInt(L"Vars/meter", 0);
    m_maxMeter = 0;
  }
  m_curMeter = m_dumpedMeter = 0;

  m_sMeterText = toOdString(qsText);
  ODA_ASSERT_ONCE(m_sMeterText != L"<$>"); // marker
  if (GETBIT(m_modeMeter, 2))
    getIConsole()->putTrace(OdString().format(L"\n%ls------- Started %ls",
                                              m_sMeterPrefix.c_str(), m_sMeterText.c_str()));

  if (!GETBIT(m_modeMeter, 1) || !isCurrentThread())
    return;

  if (m_pMeter.isNull())
  {
    // https://forum.qt.io/topic/19190/solved-how-to-set-the-position-of-progress-bar-at-right-corner-of-statusbar/4
    m_pMeter = new QProgressBar(getMainWnd()->statusBar());
    m_pMeter->setAlignment(Qt::AlignRight);
    m_pMeter->setMaximumSize(450, 19); //(180, 19);
    m_pMeter->setTextVisible(true);
    // way via palette is not working in Qt //QPalette pal(m_pMeter->palette()); pal.setColor(QPalette::Highlight, Qt::blue); pal.setBrush(QPalette::Highlight, QBrush(Qt::blue)); pal.setColor(QPalette::Base, Qt::blue); m_pMeter->setPalette(pal);
    // TODO ? //m_pMeter->setStyleSheet(QString("QProgressBar::chunk:horizontal {background: qlineargradient(x1: 0, y1: 0.5, x2: 1, y2: 0.5, stop: 0 grey, stop: 1 grey);}") + QString("QProgressBar::horizontal {border: 1px solid gray; border-radius: 3px; background: yellow; padding: 0px; text-align: left; margin-right: 4ex;}"));
    m_pMeter->setStyleSheet("QProgressBar {"
                         // " background-color: #74c8ff;"
                         // " color: #0a9dff;"
                         // " border-style: outset;"
                         // " border-width: 2px;"
                         // " border-color: #74c8ff;"
                         // " border-radius: 7px;"
                            " text-align: center;"
                            "} "
                            "QProgressBar::chunk {" //"QProgressBar::chunk:horizontal {"
                         // " background-color: #8383AD;" // violet
                            " background-color: rgba(146,146,249,255);"
                         // TODO
                         // " width: 20px;" " height: 15px;"
                         // " margin: 0.5px;"
                         // " padding: 0px;"
                         // " text-align: center;" 
                         // " text-align: left;"
                         // " margin-right: 4ex;"
                            "}");
    getMainWnd()->statusBar()->addPermanentWidget(m_pMeter);
    m_pMeter->setRange(m_curMeter, m_maxMeter ? m_maxMeter : 100);
  }
  if (m_pMeter->isTextVisible())
    m_pMeter->setFormat(qsText + ": %p%"); // TODO
  m_pMeter->setValue(m_curMeter);
  bool bVisible = !bStartBeforeSetLimit;
  if (bVisible != m_pMeter->isVisible())
  {
    //m_pMeter->setTextVisible(bVisible);
    m_pMeter->setVisible(bVisible);
  }
  //toStatusBar(qsText);
}

void OdqApplication::stopMeter(int timeout) // = 0 // hide progress meter
{
  if (GETBIT(m_modeMeter, 2))
    getIConsole()->putTrace(OdString().format(L"\n%ls------- Stopped", m_sMeterPrefix.c_str()));

  if (!GETBIT(m_modeMeter, 1) || !isCurrentThread())
    return;
  
  if (m_curMeter < m_maxMeter)
    m_pMeter->setValue(m_maxMeter); //m_pMeter->show(); //qApp->processEvents(); // show
  m_curMeter = m_maxMeter;

  if (!m_pMeter->isVisible())
    return;

  if (!timeout)
    m_pMeter->hide();
  else
  {
    m_sMeterTextToHide = m_sMeterText; // = toOdString(m_pMeter->format());
    QTimer::singleShot(timeout, this, SLOT(hideMeter()));
  }
}

//slot
void OdqApplication::hideMeter()
{
  if (   m_pMeter.isNull() || !m_pMeter->isVisible()
      || m_sMeterTextToHide != m_sMeterText || m_curMeter != m_maxMeter)
    return;
  m_sMeterTextToHide = L"<$>"; // marker
  m_pMeter->hide();
}

void OdqApplication::stepMeter() // increment the progress meter
{
  double dPrev = double(m_curMeter);
  if (m_curMeter >= m_maxMeter)
    return;
  m_curMeter++;

  if (GETBIT(m_modeMeter, 2))
  {
    double f1 = (m_curMeter * 100.0) / m_maxMeter,
           f2 = (m_dumpedMeter * 100.0) / m_maxMeter;
    if ((f1 - f2) > 0.7)
    {
      m_dumpedMeter = m_curMeter;
      getIConsole()->putTrace(OdString().format(L"\n%lsProgress: %2.2lf%%", 
                                                m_sMeterPrefix.c_str(), f1));
    }
  }

  if (!GETBIT(m_modeMeter, 1) || !isCurrentThread())
    return;
  if (m_pMeter.isNull() || !m_pMeter->isVisible())
    return;

  m_pMeter->setValue(m_curMeter);
}

OdString OdqApplication::toOdString(const QString& qsStr) const
{
  std::wstring wstr(qsStr.toStdWString());
  OdString sText(wstr.c_str());
  return sText;
}

QString OdqApplication::toQString(const OdString& sStr) const
{
  std::wstring wstr((wchar_t*)sStr.c_str());
  QString qsStr(QString::fromStdWString(wstr));
  return qsStr;
}

QString OdqApplication::encodeToXmlAttrib(const QString& cqsStr) const
{
  QString qsStr = cqsStr;
  qsStr.replace("&", "&amp;").replace("\"", "&quot;").replace("<", "&lt;")
       .replace(">", "&gt;").replace("\'", "&apos;");
  return qsStr;
}

QString OdqApplication::decodeFromXmlAttrib(const QString& cqsStr) const
{
  QString qsStr = cqsStr;
  qsStr.replace("&apos;", "\'").replace("&gt;", ">").replace("&lt;", "<")
       .replace("&quot;", "\"").replace("&amp;", "&");
  return qsStr;
}

QString OdqApplication::getExtensionByFilter(const QString& cqsSelectedFilter, 
                                             const QString& qsDelim) const //  = "" (return only 1st if empty)
{
  QString qsSelectedFilter = cqsSelectedFilter,
          qsSelectedExt;
  int pos = 0;
  while ((pos = qsSelectedFilter.indexOf("*.")) >= 0)
  {
    qsSelectedFilter = qsSelectedFilter.mid(pos + QString("*.").length());
    QRegExp rxExt("\\w+");
    if (rxExt.indexIn(qsSelectedFilter))
    {
      ODA_ASSERT_ONCE(!qsSelectedFilter.indexOf("*)"));
      continue;
    }

    if (!qsSelectedExt.isEmpty())
      qsSelectedExt += qsDelim;

    qsSelectedExt += rxExt.cap(0);

    if (qsDelim.isEmpty())
      break;
  }

  ODA_ASSERT_ONCE(!qsSelectedExt.isEmpty() || !qsSelectedFilter.indexOf("*)"));
  return qsSelectedExt;
}

OdStringArray OdqApplication::getExtDbModuleNames(OdRxObject* pRxDb, // = NULL
                                                  bool bNamesOnly) const // = true
{
  return static_cast<OdqServices*>(getBaseHostAppServices())->getExtDbModuleNames(pRxDb, bNamesOnly);
}

OdExtDbModule* OdqApplication::getExtDbModule(OdRxObject* pRxDb) const
{
  ODA_ASSERT_ONCE(pRxDb);
  if (!pRxDb)
    return NULL;
  OdStringArray names = getExtDbModuleNames(pRxDb);
  for (unsigned int idx = 0, sz = names.size(); idx < sz; idx++)
  {
    OdString sModuleName = names[idx];
    OdExtDbModule* pExtDbModule = OdExtDbModule_cast(::odrxDynamicLinker()->loadModule(sModuleName));
    if (pExtDbModule && pExtDbModule->isResponsible(pRxDb))
      return pExtDbModule;
  }
  return NULL;
}

OdExtDbModule* OdqApplication::getExtDbModuleByExtension(const OdString& csExt) const
{
  OdString sExt = csExt;
  if (!sExt.makeLower().find(L'.'))
    sExt = sExt.mid(1);
  ODA_ASSERT_ONCE(!sExt.isEmpty());
  if (sExt.isEmpty())
    return NULL;

  if (!m_mapExtDbModuleNameByExtension.size())
  {
    OdStringArray names = getExtDbModuleNames();
    for (unsigned int idx = 0, sz = names.size(); idx < sz; idx++)
    {
      OdString sModuleName = names[idx];
      OdExtDbModule* pExtDbModule = OdExtDbModule_cast(::odrxDynamicLinker()->loadModule(sModuleName));
      if (!pExtDbModule)
        continue;
      OdStringArray lst = pExtDbModule->getExtensions(OdEd::kGfpForOpen, NULL, true); // with synonyms of extension
      for (unsigned int idxEx = 0, szEx = lst.size(); idxEx < szEx; idxEx++)
      {
        OdString sEx = lst[idxEx];
        ODA_ASSERT_ONCE(   !sEx.isEmpty()
                        && m_mapExtDbModuleNameByExtension.find(sEx) == m_mapExtDbModuleNameByExtension.end());
        if (sEx.isEmpty())
          continue;

        m_mapExtDbModuleNameByExtension[sEx] = sModuleName;
      }
    }
    if (!m_mapExtDbModuleNameByExtension.size())
      m_mapExtDbModuleNameByExtension[L""] = L"";
  }

  std::map<OdString, OdString >::iterator itr = m_mapExtDbModuleNameByExtension.find(sExt);
  if (itr == m_mapExtDbModuleNameByExtension.end())
    return NULL;
  OdString sModuleName = itr->second;
  if (sModuleName.isEmpty())
    return NULL;
  return OdExtDbModule_cast(::odrxDynamicLinker()->loadModule(sModuleName));
}

const QStringList& OdqApplication::getAcadRegProfileGeneralKeys() const
{
  static QStringList keys;
#if defined(Q_OS_WIN32)
  if (keys.isEmpty())
  {
    // from current ACAD only
    QString qsPath = "HKEY_CURRENT_USER\\Software\\Autodesk\\AutoCAD";
    QSettings settings(qsPath, QSettings::NativeFormat);
    QString qsVer = settings.value("CurVer", "").toString();
    if (!qsVer.isEmpty())
    {
      qsPath += "\\";
      qsPath += qsVer;
      QSettings settings(qsPath, QSettings::NativeFormat);
      qsVer = settings.value("CurVer", "").toString();
      if (!qsVer.isEmpty())
      {
        qsPath += "\\";
        qsPath += qsVer;
        qsPath += "\\Profiles\\<<Unnamed Profile>>\\General";
        keys << qsPath;
      }
    }
  }
  if (keys.isEmpty())
  {
    // from all
    keys << "HKEY_CURRENT_USER\\Software\\Autodesk\\AutoCAD\\R23.1\\ACAD-3001:409\\Profiles\\<<Unnamed Profile>>\\General"
         << "HKEY_CURRENT_USER\\Software\\Autodesk\\AutoCAD\\R22.0\\ACAD-1001:409\\Profiles\\<<Unnamed Profile>>\\General"
         << "HKEY_CURRENT_USER\\Software\\Autodesk\\AutoCAD\\R19.0\\ACAD-B001:409\\Profiles\\<<Unnamed Profile>>\\General"
         << "HKEY_CURRENT_USER\\Software\\Autodesk\\AutoCAD\\R19.0\\ACAD-A001:409\\Profiles\\<<Unnamed Profile>>\\General"
         << "HKEY_CURRENT_USER\\Software\\Autodesk\\AutoCAD\\R18.2\\ACAD-A001:409\\Profiles\\<<Unnamed Profile>>\\General"
         << "HKEY_CURRENT_USER\\Software\\Autodesk\\AutoCAD\\R18.0\\ACAD-8001:409\\Profiles\\<<Unnamed Profile>>\\General"
         << "HKEY_CURRENT_USER\\Software\\Autodesk\\AutoCAD\\R18.0\\ACAD-8001:409\\Profiles\\<<AutoCAD 2010 Unnamed Profile>>\\General"
         << "HKEY_CURRENT_USER\\Software\\Autodesk\\AutoCAD\\R17.2\\ACAD-7001:409\\Profiles\\<<Unnamed Profile>>\\General"
         << "HKEY_CURRENT_USER\\Software\\Autodesk\\AutoCAD\\R17.0\\ACAD-5001:409\\Profiles\\<<Unnamed Profile>>\\General";
  }

 #endif
  return keys;
}

static QString firstEnvVariable(const QString qsKey)
{
  int pos = qsKey.indexOf('%');
  if (pos < 0)
    return "";
  QString qsVar = qsKey.mid(++pos);
  pos = qsVar.indexOf('%');
  ODA_ASSERT_ONCE(pos > 0);
  if (pos < 0)
    return "";
  qsVar = qsVar.left(pos);
  return qsVar;
}

QString& OdqApplication::substituteEnvVariables(QString& qsPath) const
{
  QString qsVar;
  while (!(qsVar = firstEnvVariable(qsPath)).isEmpty())
  {
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
    QString qsValue = getenv(qsVar.toAscii().data());
#else
    QString qsValue = getenv(qsVar.toLatin1().data());
#endif
    ODA_ASSERT_ONCE(!qsValue.isEmpty());

    QString qsBefore = QString("%%1%").arg(qsVar);
    qsPath.replace(qsBefore, qsValue);
    ODA_ASSERT_ONCE(qsVar != firstEnvVariable(qsPath));
  }
  return qsPath;
}

QString OdqApplication::getInitialDocumentsPath() const
{
  QString qsInitialPath;
#ifndef QT_DEBUG
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
  qsInitialPath = QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation);
#else
  qsInitialPath = QStandardPaths::displayName(QStandardPaths::DocumentsLocation);
#endif
  if (qsInitialPath.length() <= 1)
    qsInitialPath = "/sdcard";
#else
#ifdef ANDROID
  QByteArray sdcardBArr = qgetenv("EXTERNAL_STORAGE");
  if (sdcardBArr.size() > 0)
  {
    qsInitialPath = sdcardBArr.data();
    qsInitialPath.replace("\\", "/");
  } else
  {
    qsInitialPath = QCoreApplication::applicationDirPath();
    qsInitialPath.replace("\\", "/");
    int pos = qsInitialPath.lastIndexOf("Oda/", -1, Qt::CaseInsensitive);
    if (pos > 0)
    {
      if (QDir(qsInitialPath.left(pos + 3)).cd("data"))
        qsInitialPath = qsInitialPath.left(pos + 3) + "/data";
    }
  }
#elif __IPHONE_OS_VERSION_MIN_REQUIRED
  qsInitialPath = QCoreApplication::applicationDirPath();
  qsInitialPath.replace("OdaFileViewer.app", "Documents");
#else
  qsInitialPath = QCoreApplication::applicationDirPath();
  qsInitialPath.replace("\\", "/");
  int pos = qsInitialPath.lastIndexOf("Oda/", -1, Qt::CaseInsensitive);
  if (pos > 0)
  {
    if (QDir(qsInitialPath.left(pos + 3)).cd("data"))
    {
      qsInitialPath = qsInitialPath.left(pos + 3) + "/data";
#    if defined(ODA_WINDOWS) && defined(_DEBUG)
      qsInitialPath.replace("C:/", "E:/");
#    endif
    }
  }
#endif //ANDROID
#endif
  return qsInitialPath;
}

void OdqApplication::saveWidgetPlacement(QWidget* pWidget, const QString cqsKey) const
{
  OdqMainWindow* pMainWindow = qobject_cast<OdqMainWindow*>(getIApp()->getMainWindow());
  ODA_ASSERT_ONCE(pMainWindow);
  if (pMainWindow)
    pMainWindow->saveWidgetPlacement(pWidget, cqsKey);
}

void OdqApplication::restoreWidgetPlacement(QWidget* pWidget, const QString cqsKey,
                                            int widthDefault, // = 600
                                            int heightDefault) const //  = 200
{
  OdqMainWindow* pMainWindow = qobject_cast<OdqMainWindow*>(getIApp()->getMainWindow());
  ODA_ASSERT_ONCE(pMainWindow);
  if (pMainWindow)
    pMainWindow->restoreWidgetPlacement(pWidget, cqsKey, widthDefault, heightDefault);
}

QString OdqApplication::getLoadedCuiCustomGroupNameByFileName(const QString& qsCuiFileName) const
{
  return getCustom()->getLoadedCuiCustomGroupNameByFileName(qsCuiFileName);
}

bool OdqApplication::importFromFile(OdExtDbModule* pExtDb, OdEdCommandContext* pCmdCtx,
                                    const OdString& sFileName, ODCOLORREF& clrBackground)
{
  return static_cast<OdqServices*>(getBaseHostAppServices())->importFromFile(pExtDb, pCmdCtx, sFileName, clrBackground);
}

void OdqApplication::saveTextToFile(const QString& qsText, const QString& qsFilePathName) const
{
  ODA_ASSERT_ONCE(!qsText.isEmpty() && !qsFilePathName.isEmpty());

  QFile file(qsFilePathName);
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
  {
    ODA_FAIL_ONCE();
    return;
  }
  QTextStream out(&file);
  out << qsText << endl;
}
#if defined(Q_WS_MAC) || defined(Q_OS_MAC) || defined(__APPLE__)
bool OdqApplication::event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::FileOpen:
    {
        QFileOpenEvent *openEvent = static_cast<QFileOpenEvent *>(event);
        doubleClickedFileName = openEvent->file();   
        return true;
    }
    default:
        return QApplication::event(event);
    }
    return QApplication::event(event);
}
QString OdqApplication::getDoubleClickedFileName() const
{
  return doubleClickedFileName;
}
#endif
