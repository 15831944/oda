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
// OdqViewSystem.cpp
//

#include <QMainWindow>
#include <QMdiArea>
#include <QStringList> 
#include <QTimer>
#include <QFileInfo>
#include <QSettings>

#include "OdqViewSystem.h"
#include "OdqView.h"
#include "ExGripManager.h" //#include "ExGripData.h"

#include "RxVariantValue.h"
#include "Ge/GePoint3d.h"

#include "ExtDbModule.h"

//------------------------------------------------------------------------

OdqViewSystem::OdqViewSystem()
  : m_markToCloseAppByLastView(false)
  , m_bGlGsChecked(false)
  , m_bGlesGsChecked(false)
  , m_bCausticGsChecked(false)
{
  // find or create central widget 
  QMainWindow* pMainWindow = getIApp()->getMainWindow();
  if (pMainWindow)
  {
    m_pMdiArea = qobject_cast<QMdiArea*>(pMainWindow->centralWidget());
    if (m_pMdiArea.isNull())
    {
      m_pMdiArea = new QMdiArea();
      m_pMdiArea->setObjectName("MdiArea");
      pMainWindow->setCentralWidget(m_pMdiArea);
    }
    ODA_ASSERT_ONCE(m_pMdiArea->objectName() == "MdiArea");
  }
  else
    ODA_FAIL_ONCE();
  if (!connect(m_pMdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)),
               this, SLOT(activateMdiSubWindow(QMdiSubWindow*)),
               Qt::QueuedConnection))
  {
    ODA_FAIL_ONCE();
  }
  if (!QObject::connect(this, SIGNAL(postOperations()),
                        this, SLOT(postOperationsSlot()),
                        Qt::QueuedConnection))
  {
    ODA_FAIL_ONCE();                                  ;
  }
}

OdqViewSystem::~OdqViewSystem()
{
  closeAllViews();
  do {
  } while (deleteView());
}

// static 
OdqViewSystem* OdqViewSystem::create()
{
  static QPointer<OdqViewSystem> s_pVS;
  if (!s_pVS.isNull())
  {
    // only single object is enabled
    ODA_FAIL_ONCE();
    return s_pVS;
  }

  s_pVS = new OdqViewSystem();
  return s_pVS;
}

bool OdqViewSystem::isValid()
{
  return !getGsModule(NULL).isNull(); 
}

OdGsModulePtr OdqViewSystem::getGsModule(OdRxObject* pRxDb) // = NULL
{
  bool isGlesEnabled = true, isCausticEnabled = false;
  if (pRxDb)
  {
    OdDbBaseDatabasePEPtr pDbPE = OdDbBaseDatabasePE::cast(pRxDb);
    ODA_ASSERT_ONCE(pDbPE.get());
    OdRxObjectPtr pCurrLayout = pDbPE->currentLayout(pRxDb);
    if (pCurrLayout.get())
    {
      OdDbBaseLayoutPEPtr pLayoutInfo(pCurrLayout);
      ODA_ASSERT_ONCE(pLayoutInfo.get());
      if (pLayoutInfo.isNull())
        return OdGsModulePtr();
      bool isModel = pLayoutInfo->isModelLayout(pCurrLayout);
      if (isModel)
        isCausticEnabled = true; // caustic is usable for models only
    }
    ODA_ASSERT_VAR(else)
    {
      ODA_ASSERT_VAR(OdExtDbModule* pExtDb = getIApp()->getExtDbModule(pRxDb);)
      ODA_ASSERT_ONCE(pExtDb && pExtDb->baseModuleName(true) == L"ifc");
      isCausticEnabled = true; // caustic is usable for models only
    }
  }

  OdString sDeviceReduction = getIAppProps()->toString(L"Vars/device");
  if (sDeviceReduction.isEmpty() || sDeviceReduction != m_sDeviceReduction)
  {
    ODA_ASSERT_VAR(if (!m_sDeviceReduction.isEmpty()))
    {
      m_sDeviceReduction.empty();
      m_pDeviceGs = NULL;
    }
  }
  if (!sDeviceReduction.isEmpty())
  {
    ODA_ASSERT_ONCE(m_sDeviceReduction.isEmpty() && m_pDeviceGs.isNull());

    OdString sDevice = sDeviceReduction,
             sDeviceLow = sDevice;
    int pos = sDeviceLow.makeLower().find(L"win"),
        len = 3;
    if (pos >= 0)
      len = 6;
    sDeviceLow = sDeviceLow.mid(pos);
    OdString sShort = sDevice.left(len);
    QStringList lst;
    if (isGlesEnabled)
      lst << "OdaQtGLES2"; 
    lst << "OdaQtOpenGL";
    lst << "WinMetal";
    if (isGlesEnabled)
      lst << "WinGLES2";
    lst << "WinOpenGL" << "WinDirectX" << "WinGDI";
    //lst << "WinBitmap" << "WinComposite";
    foreach(QString qsDevice, lst)
    {
      sDevice = getIApp()->toOdString(qsDevice.toLower());
      if (sDeviceLow.find(sShort))
        continue;

      m_sDeviceReduction = sDeviceReduction;
      m_pDeviceGs = ::odrxDynamicLinker()->loadModule(sDevice);
      if (m_pDeviceGs.isNull())
        getIConsole()->putString(OdString().format(L"Module %ls was not found.",
                                                    sDevice.c_str()));
      break;
    }
  }
  if (m_pDeviceGs.get())
    return m_pDeviceGs;

  if (   isCausticEnabled 
      && (   (m_bCausticGsChecked && m_pCausticGs.isNull())
          || !getIAppProps()->toBool(OD_T("Vars/caustic"), false)))
    isCausticEnabled = false;
  if (isCausticEnabled)  
  {
    if (m_pCausticGs.isNull())
    {
      QStringList lstTxv;
      lstTxv << "OdaQtOpenRL.txv";
     #ifdef Q_CC_MSVC
      lstTxv << "WinOpenRL.txv" << "BmpOpenRL.txv";
     #endif

      foreach (QString qsTxv, lstTxv)
      {
        OdString sTxv = getIApp()->toOdString(qsTxv);

        m_pCausticGs = ::odrxDynamicLinker()->loadModule(sTxv);
        if (!m_pCausticGs.isNull())
          break;

        getIConsole()->putString(QObject::tr("Module %1 was not found").arg(qsTxv));
      }
      m_bCausticGsChecked = true;
    }

    if (m_pCausticGs.get())
    {
      OdRxDictionaryPtr pProperties = m_pCausticGs->createDevice()->properties();
      if (   pProperties->has(OD_T("MulticontextSupport"))
          && pProperties->has(OD_T("OpenRLContextsCount")))
      {
        OdRxVariantValue pRcVar = (OdRxVariantValue) pProperties->getAt(OD_T("MulticontextSupport"));
        // such conversion gives an invalid result on Mac OS X (true instead false here) // bool isValid = (bool) pRcVar;
        bool isValid = pRcVar->getBool();
        if (isValid)
          return m_pCausticGs;
        pRcVar = (OdRxVariantValue) pProperties->getAt(OD_T("OpenRLContextsCount"));
        OdUInt32 nCnt = (OdUInt32) pRcVar;
        isValid = !nCnt;
        if (isValid)
          return m_pCausticGs;
        static bool bFirst = true;
        if (bFirst)
        {
          bFirst = false;
          getIConsole()->putString(QObject::tr(
            "Only one context is supported by Caustic OpenRL library now (Caustic DevSupport #4330)"));
        }
      }
    }
  } //if (isCausticEnabled)

  if (   isGlesEnabled 
      && ((m_bGlesGsChecked && m_pGlesGs.isNull()) || !getIAppProps()->toBool(OD_T("Vars/gles"), true)))
    isGlesEnabled = false;
#if defined(Q_OS_LINUX) || defined(ANDROID) || defined(__IPHONE_OS_VERSION_MIN_REQUIRED)
  isGlesEnabled = true;
#endif

  if (isGlesEnabled)
  {
    if (m_pGlesGs.isNull())
    {
      m_pGlesGs = ::odrxDynamicLinker()->loadModule(OdQtGLES2ModuleName);
      m_bGlesGsChecked = true;
      if (m_pGlesGs.isNull())
        getIConsole()->putString(QObject::tr("Module %1 was not found").arg(getIApp()->toQString(OdQtGLES2ModuleName)));
      else
      {
//#if (QT_VERSION < QT_VERSION_CHECK(5,0,0) && defined(Q_CC_MSVC))
//      QGLFormat::OpenGLVersionFlags flg = QGLFormat::defaultFormat().openGLVersionFlags(); // 0x0001f07f
//#else
        QGLFormat::OpenGLVersionFlags flg = QGLFormat::OpenGL_Version_None;
        const QGLContext* pCurrCtx = QGLContext::currentContext();
        OdString sVers;
        if (!pCurrCtx)
        {
          struct QGLWidgetTmp : public QGLWidget { QGLWidgetTmp() { glInit(); } };
          QGLWidgetTmp* pTmp = new QGLWidgetTmp();
          sVers = reinterpret_cast<const char*>(glGetString(GL_VERSION));
          pTmp->close(); delete pTmp;
        }
        if (pCurrCtx || !sVers.isEmpty()) // workaround to skip QT5 crash in openGLVersionFlags with using via remote desktop
          flg = QGLFormat::defaultFormat().openGLVersionFlags(); // 0x0001f07f
//#endif
#ifndef ANDROID
        bool isGlesGsSupported = (flg & (  QGLFormat::OpenGL_Version_2_0 | QGLFormat::OpenGL_Version_2_1
                                       //| QGLFormat::OpenGL_ES_Version_2_0 // strange but it is missing
                                         | QGLFormat::OpenGL_Version_3_0 | QGLFormat::OpenGL_Version_3_1
                                         | QGLFormat::OpenGL_Version_3_2 | QGLFormat::OpenGL_Version_3_3
                                         | QGLFormat::OpenGL_Version_4_0)) != 0;
#else
        bool isGlesGsSupported = (!sVers.isEmpty());
#endif
        //class OdGLES2RenderClientTst : public OdGLES2RenderClient
        //{
        //public:
        //  QPointer<QGLWidget> m_pGlesWidget;
        //  OdString m_sError;
        //  OdGLES2RenderClientTst() : m_pGlesWidget(new QGLWidget()) // (new QGLWidget(getIApp()->getMainWindow()))
        //  {
        //    m_pGlesWidget->setAttribute(Qt::WA_DeleteOnClose, true);
        //    QGLContext* pContext = const_cast<QGLContext*>(m_pGlesWidget->context());
        //    if (pContext)
        //      pContext->create();
        //  }
        //  ~OdGLES2RenderClientTst()
        //  { 
        //    m_pGlesWidget->close(); 
        //    qApp->processEvents();
        //    //ODA_ASSERT_ONCE(m_pGlesWidget.isNull());
        //  }
        //  virtual bool hasProperty(const OdChar* pPropName) const { return OdString(L"QGLWidget") == pPropName; }
        //  virtual OdRxVariantValue getProperty(const OdChar *pPropName) const 
        //  { 
        //    if (OdString(L"QGLsWidget") == pPropName)
        //      return OdRxVariantValue((OdIntPtr) qobject_cast<QObject*>(m_pGlesWidget));
        //    return OdRxVariantValue(false);
        //  }
        //  virtual void setProperty(const OdChar *pPropName, OdRxVariantValue pValue) {}
        //  virtual int outputWindowWidth() const { return 0; }
        //  virtual int outputWindowHeight() const { return 0; }
        //  virtual void emitError(const char *pError) { m_sError = pError; throw OdEdCancel(); }
        //  virtual void emitWarning(const char *pWarn) { }
        //} rendClient;
        //try
        //{
        //  OdGsDevicePtr pDevice = m_pGlesGs->createDevice();
        //  OdGLES2Device* pGlesDevice = static_cast<OdGLES2Device*>(pDevice.get());
        //  OdGLES2Rendition* pRend = pGlesDevice->rendition();
        //  pRend->onDeviceUpdateBegin(&rendClient);  // pRend->createContext(NULL); //OdGLES2RenderClient *pDevice)
        //  qApp->processEvents();
        //  // TODO 
        //  //OdGsDevicePtr pDevice = m_pGlesGs->createDevice();
        //  //// pDevice->setProperty(L"QGLsWidget", ...);
        //  //setProperty(const OdChar *pPropName, OdRxVariantValue pValue): {}
        //  //const OdUInt32 minOpenGLVersion = OdRxVariantValue(pDevice->properties()->getAt(L"CheckRendererVersion"))->getUInt32();
        //
        //  ODA_ASSERT_ONCE(isGlesGsSupported); // test
        //  isGlesGsSupported = true;
        //}
        //catch (OdEdCancel&)
        //{
        //  // unfortunately it is normal for Windows with Remote Desktop using
        //  //getIConsole()->putString(QObject::tr("Module %1 was unsupported (Error: %2)")
        //  //                           .arg(getIApp()->toQString(OdQtGLES2ModuleName))
        //  //                           .arg(getIApp()->toQString(rendClient.m_sError)));
        //  ODA_ASSERT_ONCE(!isGlesGsSupported); // test
        //  isGlesGsSupported = false;
        //}

        if (!isGlesGsSupported)
        {
          getIConsole()->putString(QObject::tr("  Module %1 is unsupported for this desktop")
                                     .arg(getIApp()->toQString(OdQtGLES2ModuleName)));
          m_pGlesGs = NULL;
          ::odrxDynamicLinker()->unloadModule(OdQtGLES2ModuleName);
#        if (QT_VERSION >= QT_VERSION_CHECK(5,0,0) && defined(Q_CC_MSVC))
          QGLFormat fmt = QGLFormat::defaultFormat();
          fmt.setDirectRendering(false);
          QGLFormat::setDefaultFormat(fmt);
#        endif
        }
      }
    }

    if (m_pGlesGs.get())
      return m_pGlesGs;

  } //if (isGlesEnabled)

  if (m_bGlGsChecked || m_pGlGs.get())
    return m_pGlGs;
  m_pGlGs = ::odrxDynamicLinker()->loadModule(OdQtOpenGLModuleName);
  m_bGlGsChecked = true;
  if (m_pGlGs.isNull())
  {
    getIConsole()->putString(QObject::tr("Module %1 was not found").arg("OdaQtOpenGL.txv"));
#  ifdef Q_CC_MSVC
    // try to work through Windows non-portable gs
    m_pGlGs = ::odrxDynamicLinker()->loadModule(OdWinOpenGLModuleName);
    if (m_pGlGs.isNull())
      getIConsole()->putString(QObject::tr("Module %1 was not found").arg(getIApp()->toQString(OdWinOpenGLModuleName)));
#  endif
  }
  return m_pGlGs;
}

void OdqViewSystem::saveSettings()
{
  // TODO store via QSettings
}

void OdqViewSystem::activateMdiSubWindow(QMdiSubWindow* pWindow)
{
  if (!pWindow || m_mapViews.isEmpty() || !pWindow->parentWidget())
    return;
  ODA_ASSERT_ONCE(pWindow->parentWidget()->parentWidget() == m_pMdiArea);

  OdqView* pView = qobject_cast<OdqView*>(pWindow);
  ODA_ASSERT_ONCE(pView);
  
  static OdqView* s_pViewPrev = NULL; 
  if (s_pViewPrev == pView)
    return;
  s_pViewPrev = pView;

  getIConsole()->setCurrentDatabase(pView->getRxDatabase());

  QString qsTitle = pView->windowTitle();
  ODA_ASSERT_ONCE(!qsTitle.isEmpty());
  if (!qsTitle.isEmpty())
    getIApp()->refreshQObjetcts("<event-refresh:window_activated>", qsTitle);

  // Workaround to refresh View
#if defined(ANDROID) || (defined(__APPLE__) && (QT_VERSION < QT_VERSION_CHECK(5,5,0)))
  QSize actSize = getIApp()->getMainWindow()->size();
  getIApp()->getMainWindow()->resize(actSize.width()+1, actSize.height()+1);
#endif // ANDROID
}

IqView* OdqViewSystem::getView(OdRxObject* pRxDatabase,
                               bool bDoActive, // = false
                               bool* pIsSingle) // = NULL
{
  if (pIsSingle)
    *pIsSingle = true;
  QList<QPointer<OdqView> > lst = m_mapViews.values(pRxDatabase);
  if (pIsSingle)
    *pIsSingle = !(lst.size() > 1 && !lst[0].isNull() && !lst[1].isNull());

  OdqView* pActiveView = qobject_cast<OdqView*>(m_pMdiArea->activeSubWindow());
  if (pActiveView && pRxDatabase == pActiveView->getRxDatabase())
    return pActiveView;

  OdqView* pView = NULL;
  if (!lst.size() || !(pView = lst.first()))
  {
    //ODA_ASSERT_ONCE(bDoActive); // try to activate view before it was created
    return NULL;
  }
  ODA_ASSERT_ONCE(pView->getRxDatabase() == pRxDatabase);

  if (bDoActive)
    // activate the first view for this database
    m_pMdiArea->setActiveSubWindow(pView);

  return pView;
}

QList<IqView*> OdqViewSystem::getOpenViews(OdRxObject* pRxDatabase)
{
  QList<IqView*> lst;
  foreach (QPointer<OdqView> pView, m_mapViews.values(pRxDatabase))
  {
    ODA_ASSERT_ONCE(!pView.isNull());
    if (pView.isNull())
      continue;
    lst << (IqView *) pView;
  }
  return lst;
}

// return NULL if point out of view widget
IqView* OdqViewSystem::getViewByScreenPoint(QPoint& point, // in[screen] // out[view] 
                                            bool bDoActive, // = false
                                            bool bDwgOnly) // = true
{
  //point = QCursor::pos(); // test
  QWidget* pWidget = qApp->widgetAt(point); //finds exact widget under cursor
  if (!pWidget)
    return NULL;
  
  OdqBaseRenderArea* pBaseRenderArea = OdqBaseRenderArea::renderArea(pWidget);
  if (!pBaseRenderArea)
    return NULL;

  OdqView* pView = pBaseRenderArea->getView();
  ODA_ASSERT_ONCE(pView);
  if (!pView || (bDwgOnly && !OdDbDatabase::cast(pView->getRxDatabase()).get()))
    return NULL;  
  OdqView* pActiveView = qobject_cast<OdqView*>(m_pMdiArea->activeSubWindow());
  // subWindowActivated
  if (bDoActive && pView && pView != pActiveView)
    m_pMdiArea->setActiveSubWindow(pView);

  point = pBaseRenderArea->widget()->mapFromGlobal(point);
  return pView;
}

bool OdqViewSystem::isMaximized()
{
  bool bMaximized = true; // with the same way as in constructor of OdqView
  QMdiSubWindow* pSubWindow = NULL;
  if (m_mapViews.size())
    pSubWindow = getMdiArea()->currentSubWindow();
  if (pSubWindow)
    bMaximized = pSubWindow->isMaximized();
#if !(defined(ANDROID))
  else
  {
    QSettings settings("ODA", QFileInfo(QCoreApplication::applicationFilePath()).baseName());
    bMaximized = settings.value(TD_SHORT_STRING_VER_S "/view/maximized", true).toBool();
  }
#endif
  return bMaximized;
}

OdqView* OdqViewSystem::createView(OdRxObject* pRxDatabase, 
                                   OdDbStub* idLayoutView, // = NULL
                                   const OdGsDCRect* pRect) // = NULL
{
  bool bMaximized = isMaximized(); // correct before OdqView::create only
  OdqView* pView = OdqView::create(m_pMdiArea, pRxDatabase, idLayoutView, pRect);
  if (m_mapViews.end() == m_mapViews.insert(pRxDatabase, pView))
  {
    ODA_FAIL_ONCE();
    return NULL;
  }
  //pView->resize(m_pMdiArea->size()); // TOTEST

  updateViewTitles(pRxDatabase);

  if (bMaximized)
    pView->showMaximized();
  else
    pView->showNormal();
  qApp->processEvents();

  // moved from OdqView constuctor 
  pView->updateLayouts(); // contains resetRenderArea() & resetDevice()

#if defined(ANDROID)
  pView->showMaximized();
//#elif QT_VERSION >= QT_VERSION_CHECK(5,4,0) && (defined(Q_OS_WIN32) || defined(Q_OS_WIN64))
//#elif (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
//  if (bMaximized)
//    QTimer::singleShot(0, pView, SLOT(showMaximized()));
//  else
//    QTimer::singleShot(0, pView, SLOT(showNormal()));
//#else
//  if (bMaximized)
//    pView->setWindowState(Qt::WindowMaximized);
//  else
//    pView->show();
#endif
  m_pMdiArea->setActiveSubWindow(pView);
  return pView;
}

IqView* OdqViewSystem::createViews(OdRxObject* pRxDatabase)
{
  ODA_ASSERT_ONCE(m_pMdiArea && pRxDatabase);

  OdDbStub* idActiveLayoutView = NULL;
  OdDbStubPtrArray idsLayoutViews;
  OdGsDCRectArray rects;

  OdExtDbModule* pExtDb = getIApp()->getExtDbModule(pRxDatabase);
  if (pExtDb)
  {
    if (pExtDb->getVisibleLayoutViewIds(pRxDatabase, idActiveLayoutView, idsLayoutViews, rects))
    {
      ODA_ASSERT_ONCE(idActiveLayoutView);
    }
    else
    {
      ODA_ASSERT_ONCE(pExtDb->baseModuleName(true) != L"dgn");
    }
  }
  if (!idActiveLayoutView)
    idsLayoutViews.push_back(NULL);

  int sz = (int)idsLayoutViews.size();
  ODA_ASSERT_ONCE(sz > 0);
  if (sz <= 0)
    return NULL;

  // INT-5534
  OdqView* pActiveView = createView(pRxDatabase, idsLayoutViews[0], rects.size() ? &rects[0] : NULL);
  for (int idx = 1; idx < sz; idx++)
  {
    if (postCreateView(pRxDatabase, idsLayoutViews[idx], (idx < (int)rects.size()) ? &rects[idx] : NULL))
      continue;
    ODA_FAIL_ONCE();
  }
  return pActiveView;
}

// ----- OdqViewSystem -----

QMdiArea* OdqViewSystem::getMdiArea()
{
  ODA_ASSERT_ONCE(!m_pMdiArea.isNull());
  return m_pMdiArea;
}

bool OdqViewSystem::closeViews(OdRxObject* pRxDatabase)
{
  bool bRes = false;
  QList<QPointer<OdqView> > lst = m_mapViews.values(pRxDatabase);
  int index = -1;
  foreach (QPointer<OdqView> pView, lst)
  {
    index++;
    ODA_ASSERT_ONCE(!pView.isNull());
    if (!pRxDatabase && m_pMdiArea->activeSubWindow() != pView)
      continue;

    if (pView->emitClose()) // execute removeView via close event of view
      bRes = true;
    else if (pRxDatabase)
    {
      // possible only via cancel of save override dialog by closing (with saving) of last view
      ODA_ASSERT_ONCE(index == (lst.size() - 1)); // temp brk test 
      bRes = false;
      break;
    }

    if (!pRxDatabase)
    {
      ODA_ASSERT_ONCE(pView.isNull());
      bRes = true;
      break;
    }
  }
  return bRes;
}

bool OdqViewSystem::markToCloseAppByLastView()
{
  m_markToCloseAppByLastView = true;
  return m_mapViews.size();
}

void OdqViewSystem::checkMarkToCloseApp()
{
  if (!m_markToCloseAppByLastView || m_mapViews.size())
    return;
  if (!getIApp()->getMainWindow())
    return;

  QTimer::singleShot(1, getIApp()->getMainWindow(), SLOT(close())); // pMainWindow->close();
}

bool OdqViewSystem::closeAllViews(QList<OdRxObject*>* pLstExceptDatabases) // = NULL
{
  QList<OdRxObject*> qlstKeys = m_mapViews.keys(); 

  QMap<OdRxObject*, OdRxObject*> mapSingle; 
  foreach(OdRxObject*pDB, qlstKeys) 
    mapSingle[pDB] = pDB;
  qlstKeys = mapSingle.keys(); // list without duplicates

  bool bRes = true;
  foreach(OdRxObject* pRxDatabase, qlstKeys) 
  {
    if (pLstExceptDatabases && pLstExceptDatabases->contains(pRxDatabase))
      continue;
    if (!closeViews(pRxDatabase))
    {
      bRes = false;
      break;
    }
  }

  if (bRes && !pLstExceptDatabases)
    while (closeViews(NULL))
    {
    }

  //m_mapViews.clear();
  if (!pLstExceptDatabases)
    bRes = !m_mapViews.size();
  return bRes;
}

// executed via close event of OdqView
// and execute closeDatabase by last view of database

static QList<QPointer<OdqView> > s_lstViewesToDelete;

bool OdqViewSystem::removeView(OdqView* pChild)
{
  ODA_ASSERT_ONCE(pChild);
  OdRxObject* pRxDatabase = pChild->getRxDatabase();

  // for NULL database different view may contains 
  // absolutely independent geometry 
  // (pdf ... - in minimal impl without base-database)

  QPointer<OdqView> pRemovedView;
  bool bDoCloseDatabase = (pRxDatabase != NULL),
       bMaximized = isMaximized();

  for (TViewMap::Iterator itr = m_mapViews.find(pRxDatabase);
       itr != m_mapViews.end() && itr.key() == pRxDatabase;) 
  {
    QPointer<OdqView> pView = itr.value();
    if (pChild != pView)
    {
      ODA_ASSERT_ONCE(!pView.isNull());
      bDoCloseDatabase = false;
      if (pRemovedView.data())
      {
        if (bMaximized)
          pView->showMaximized();
        break;
      }
      itr++;
      continue;
    }

    pRemovedView = pView;

    itr = m_mapViews.erase(itr);

    IqRevDiff* pRevDiff = getIRevSystem() ? getIRevSystem()->getIRevDiff(pRemovedView.data()) : NULL;
    if (pRevDiff)
      pRevDiff->closeDiffView(pRxDatabase);

    if (!bDoCloseDatabase)
      break;
  } // end for
  if (pRemovedView.isNull())
  {
    ODA_FAIL_ONCE();
    return false;
  }
  ODA_ASSERT_ONCE(!pRemovedView->windowTitle().isEmpty());
  getIApp()->refreshQObjetcts("<event-refresh:window_removed>", pRemovedView->windowTitle());
  //pRemovedView->lockMaximize();

  if (bDoCloseDatabase)
  {
    pRemovedView->clearDevice(); // device should be deleted earlier than database one
    pRemovedView->saveSettings();
    if (!getIConsole()->closeDatabase(pRxDatabase)) // contains database destroying
    {
      // restore in map
      if (m_mapViews.end() == m_mapViews.insert(pRxDatabase, pRemovedView ))
      {
        ODA_FAIL_ONCE();
      }
      return false;
    }
    //useless for QT4 & do nothing for QT5(too earlier) //qApp->processEvents();
    checkMarkToCloseApp(); // really works from here for QT4 only
  }
  else
    updateViewTitles(pRxDatabase);

  m_pMdiArea->removeSubWindow(pRemovedView); // remove Child
  ODA_ASSERT_ONCE(!pRemovedView->parentWidget());

  //pRemovedView->emitClose();
  //
  // problem with wake14.dwg (22Mb) (via posted event QEvent::DeferredDelete  on Windows only)
  bool bDeleteOnClose = pRemovedView->testAttribute(Qt::WA_DeleteOnClose);
  if (!bDeleteOnClose)
  {
    pRemovedView->clearDevice();
    // bad way (destroying should be deferred in any case to avoid problems with current event) //delete pRemovedView;
    ////pRemovedView->setAttribute(Qt::WA_DeleteOnClose, true);
    pRemovedView->emitClose();
    ////QTimer::singleShot(0, pRemovedView, SLOT(deleteView()));
    s_lstViewesToDelete << pRemovedView;
    QTimer::singleShot(10, this, SLOT(deleteView())); 
  }
  else
    pRemovedView->emitClose();

  return true;
}

// slot
bool OdqViewSystem::deleteView()
{
  if (s_lstViewesToDelete.isEmpty())
    return false;
  QPointer<OdqView> pView = s_lstViewesToDelete.takeFirst();

  if (pView.isNull() || pView->testAttribute(Qt::WA_DeleteOnClose))
  {
    ODA_FAIL_ONCE();
    return false;
  }

  delete pView;
  return true;
}

// add / remove :N(of view) suffix in title
void OdqViewSystem::updateViewTitles(OdRxObject *pRxDatabase,
                                     bool bForce) // = false
{
  QList<QPointer<OdqView> > lst = m_mapViews.values(pRxDatabase);
  updateViewTitles(pRxDatabase, lst, bForce);
}

void OdqViewSystem::updateViewTitles(OdRxObject* pRxDatabase,
                                     QList<QPointer<OdqView> >& lst,
                                     bool bForce) // = false
{
  if (!pRxDatabase)
  {
    if (bForce)
    {
      OdqView* pActiveView = qobject_cast<OdqView*>(m_pMdiArea->activeSubWindow());
      QString qsTitleActive;
      if (pActiveView && !(qsTitleActive = pActiveView->windowTitle()).isEmpty())
        getIApp()->refreshQObjetcts("<event-refresh:window_activated>", qsTitleActive);
    }
    return; // there is nothing to update (always one view)
  }

  QString qsTitle(getIConsole()->getQTitle(pRxDatabase)),
          qsTitlePrev;

  switch (lst.size())
  {
  case 0:
    //ODA_FAIL_ONCE();
    return;
  case 1:
    ODA_ASSERT_ONCE(!lst.first().isNull());
    if (bForce || qsTitle != (qsTitlePrev = lst.first()->windowTitle()))
    {
      lst.first()->setWindowTitle(qsTitle);
      if (!qsTitlePrev.isEmpty())
        getIApp()->refreshQObjetcts("<event-refresh:window_removed>", qsTitlePrev);
      getIApp()->refreshQObjetcts("<event-refresh:window_added>", qsTitle);
    }
    return;
  }
  
  int numNonDiff = 0;
  IqRevSystem* iRevSystem = getIRevSystem();
  QPointer<OdqView> pView;
  foreach (pView, lst)
  {
    ODA_ASSERT_ONCE(!pView.isNull() && pRxDatabase == pView->getRxDatabase());
    if (!iRevSystem || !iRevSystem->getIRevDiff(pView.data()))
      numNonDiff++;
    QString qsTitlePrev = pView->windowTitle();
    if (!qsTitlePrev.isEmpty())
      getIApp()->refreshQObjetcts("<event-refresh:window_removed>", qsTitlePrev);
  }
  int indexView = 1; // numerate since 1
  foreach (pView, lst)
  {
    QString qsIndexedTitle = numNonDiff > 1 ? QString("%1:%2").arg(qsTitle).arg(indexView++) : qsTitle;
    if (iRevSystem && iRevSystem->getIRevDiff(pView.data()))
      qsIndexedTitle = QString("%1:diff").arg(qsTitle);
    pView->setWindowTitle(qsIndexedTitle);
    getIApp()->refreshQObjetcts("<event-refresh:window_added>", qsIndexedTitle);
  }
}

bool OdqViewSystem::setActiveViewbyTitle(const OdString& sTitle)
{
  ODA_ASSERT_ONCE(!sTitle.isEmpty());
  if (sTitle.isEmpty())
    return false;
  OdqView* pActiveView = qobject_cast<OdqView*>(m_pMdiArea->activeSubWindow());

  if (pActiveView && pActiveView->windowTitle() == sTitle)
    return true; // already done
  
  foreach (QPointer<OdqView> pView, m_mapViews.values())
  {
    ODA_ASSERT_ONCE(!pView.isNull());
    if (!pView.isNull()&& pView->windowTitle() == sTitle)
    {
#if defined(ANDROID)
      pActiveView->emitClose();

      m_pMdiArea->setActiveSubWindow(pView);

      pView->activateWindow();
      pView->showMaximized();
#else
      m_pMdiArea->setActiveSubWindow(pView);
#endif
      return true;
    }
  }
  return false; // was not found
}

void OdqViewSystem::updateGL(OdRxObject* pRxDatabase,
                             bool bActiveOrFirstOnly) // = false
{
  if (!pRxDatabase)
    return; // noting to repaint

  if (bActiveOrFirstOnly)
  {
    IqView* iFirst = getView(pRxDatabase);
    if (iFirst)
      iFirst->updateGL();
    return;
  }

  QList<QPointer<OdqView> > lst = m_mapViews.values(pRxDatabase);
  foreach (QPointer<OdqView> pView, lst)
  {
    ODA_ASSERT_ONCE(!pView.isNull() && pRxDatabase == pView->getRxDatabase());

    pView->updateGL();
  }
}

//////////////////////////////////////////////////////////////////////////

typedef enum
{
  enPO_None
, enPO_PostCreateView
, enPO_UpdateView // repaint
, enPO_UpdateObject
, enPO_HighlightObject
, enPO_OnModifiedGrip
, enPO_HighlightGripPoint
, enPO_HighlightSnapPoint
} TPostOperation;

class PostOperation
{
  friend class OdqViewSystem;

  TPostOperation m_type;
  bool m_bValue; // on / off
  OdDbStub* m_id;
  OdSharedPtr<OdDbBaseFullSubentPath> m_pPath;
  OdGiDrawablePtr m_pGripData;
  OdRxObject* m_pRxDatabase; // we need it after OdDbStub.h placement in inaccessible folder
  int m_index; // material or snap ...
  OdGePoint3d m_point;
  OdSharedPtr<OdGsDCRect> m_pRect; // for enPO_PostCreateView only

public:
  PostOperation(TPostOperation type = enPO_None, 
                bool bValue = true, // on / off
                OdRxObject* pRxDatabase = NULL,
                OdDbStub* id = NULL, 
                const OdDbBaseFullSubentPath* pPath = NULL,
                int index = 0,
                const OdGePoint3d& point = OdGePoint3d(),
                const OdGsDCRect* pRect = NULL)
    : m_type((!id && type != enPO_UpdateView) ? enPO_None : type)
    , m_bValue(bValue)
    , m_id(id)
    , m_index(index)
    , m_point(point)
    , m_pRxDatabase(pRxDatabase)
  {
    if (pPath)
      m_pPath = new OdDbBaseFullSubentPath(*pPath);
    if (pRect)
      m_pRect = new OdGsDCRect(*pRect);
  }

  PostOperation(OdGiDrawablePtr pGripData,
                OdRxObject* pRxDatabase)
    : m_type(enPO_OnModifiedGrip)
    , m_bValue(false)
    , m_pGripData(pGripData)
    , m_id(NULL)
    , m_index(0)
    , m_pRxDatabase(pRxDatabase)
  {
    ODA_ASSERT_ONCE(m_pGripData.get());
  }

  PostOperation(bool bValue, // on / off
                OdGiDrawablePtr pGripData,
                OdRxObject* pRxDatabase)
    : m_type(enPO_HighlightGripPoint)
    , m_bValue(bValue)
    , m_pGripData(pGripData)
    , m_id(NULL)
    , m_index(0)
    , m_pRxDatabase(pRxDatabase)
  {
    ODA_ASSERT_ONCE(m_pGripData.get());
  }

  PostOperation(const PostOperation& from)
    : m_type(enPO_None)
    , m_bValue(true)
    , m_id(NULL)
    , m_index(0)
  {
    operator=(from);
  }

  PostOperation& operator=(const PostOperation& from)
  {
    if (this == &from)
      return *this;
  
    m_type = from.m_type;
    m_bValue = from.m_bValue;
    m_pRxDatabase = from.m_pRxDatabase;
    m_id = from.m_id;
    m_pPath = NULL;
    if (from.m_pPath.get())
      m_pPath = new OdDbBaseFullSubentPath(*from.m_pPath.get());
    m_pGripData = from.m_pGripData,
    m_index = from.m_index;
    m_point = from.m_point;
    m_pRect = from.m_pRect;
    return *this;
  }

  bool operator==(const PostOperation &with) const
  {
    bool bRes =    m_type == with.m_type
                && m_bValue == with.m_bValue
                && m_pRxDatabase == with.m_pRxDatabase
                && m_id == with.m_id
                && m_pPath.get() == with.m_pPath.get()
                && m_pGripData == with.m_pGripData
                && m_index == with.m_index
                && m_point == with.m_point
                && m_pRect.get() == m_pRect.get();
    return bRes;
  }
};

static QList<PostOperation> s_operationsPost;
static bool s_bPostOperationsBusy = false;

bool OdqViewSystem::postCreateView(OdRxObject* pRxDatabase, OdDbStub* idLayoutView, const OdGsDCRect* pRect)
{
  ODA_ASSERT_ONCE(pRxDatabase)
  if (   !pRxDatabase
      || m_mapViews.find(pRxDatabase) == m_mapViews.end()
      || s_bPostOperationsBusy)
    return false;

  PostOperation operation(enPO_PostCreateView, true, pRxDatabase, idLayoutView, NULL, 0, OdGePoint3d(), pRect);

  ODA_ASSERT_ONCE(!s_bPostOperationsBusy);
  if (s_operationsPost.indexOf(operation) < 0) // is absent
  {
    s_operationsPost.push_back(operation);
    emit postOperations();
  }
  else
    ODA_ASSERT_ONCE(true); // brk

  return true;
}

bool OdqViewSystem::updateViews(OdRxObject* pRxDatabase)
{
  ODA_ASSERT_ONCE(pRxDatabase)
  if (   !pRxDatabase
      || m_mapViews.find(pRxDatabase) == m_mapViews.end()
      || s_bPostOperationsBusy)
    return false;

  PostOperation operation(enPO_UpdateView, true, pRxDatabase);

  ODA_ASSERT_ONCE(!s_bPostOperationsBusy); 
  if (s_operationsPost.indexOf(operation) < 0) // is absent
  {
    s_operationsPost.push_back(operation);
    emit postOperations();
  }
  else
    ODA_ASSERT_ONCE(true); // brk

  return true;
}

bool OdqViewSystem::updateObject(OdDbStub* id, OdRxObject* pRxDatabase)
{
  ODA_ASSERT_ONCE(id);
  if (id && !pRxDatabase)
    pRxDatabase = baseDatabaseBy(id);

  ODA_ASSERT_ONCE(pRxDatabase && pRxDatabase == baseDatabaseBy(id));
  if (   !id || !pRxDatabase
      || m_mapViews.find(pRxDatabase) == m_mapViews.end()
      || s_bPostOperationsBusy)
    return false;

  PostOperation operation(enPO_UpdateObject, true, pRxDatabase, id);

  ODA_ASSERT_ONCE(!s_bPostOperationsBusy); 
  if (s_operationsPost.indexOf(operation) < 0) // is absent
  {
    s_operationsPost.push_back(operation);
    emit postOperations();
  }
  else
    ODA_ASSERT_ONCE(true); // brk

  return true;
}

// select the scene graph node corresponding with the DB object
bool OdqViewSystem::highlightObject(bool bValue, OdDbStub* id, 
                                    const OdDbBaseFullSubentPath* pPath, // = NULL
                                    OdRxObject* pRxDatabase) // = NULL
{
  ODA_ASSERT_ONCE(id);
  if (id && !pRxDatabase)
  {
    pRxDatabase = baseDatabaseBy(id);
    ODA_ASSERT_ONCE(OdDbBaseDatabasePE::cast(pRxDatabase).get()); // TODO for IFC
  }
  
  ODA_ASSERT_ONCE(pRxDatabase && pRxDatabase == baseDatabaseBy(id));
  if (   !id || !pRxDatabase 
      || m_mapViews.find(pRxDatabase) == m_mapViews.end())
    return false;

  PostOperation operation(enPO_HighlightObject, bValue, pRxDatabase, id, pPath);
  
  if (s_operationsPost.indexOf(operation) >= 0) // already added
  {
    //ODA_FAIL_ONCE(); // test
    return false;
  }

  s_operationsPost.push_back(operation);
  emit postOperations();
  return true;
}

bool OdqViewSystem::onModifiedGrip(OdGiDrawablePtr pGripData, 
                                   OdRxObject* pRxDatabase)
{
  ODA_ASSERT_ONCE(pGripData.get() && pRxDatabase);
  if (pGripData.isNull() || !pRxDatabase)
    return false;
  if (m_mapViews.find(pRxDatabase) == m_mapViews.end())
    return false;

  PostOperation operation(pGripData, pRxDatabase);
  if (s_operationsPost.indexOf(operation) >= 0) // already added
  {
    // possible via hover grip points //ODA_FAIL_ONCE(); // test
    return false;
  }

  s_operationsPost.push_back(operation);
  emit postOperations();
  return true;
}

bool OdqViewSystem::highlightGripPoint(bool bValue, 
                                       OdGiDrawablePtr pGripData,
                                       OdRxObject* pRxDatabase)
{
  ODA_ASSERT_ONCE(pGripData.get() && pRxDatabase);
  if (pGripData.isNull() || !pRxDatabase)
    return false;
  if (m_mapViews.find(pRxDatabase) == m_mapViews.end())
    return false;

  PostOperation operation(bValue, pGripData, pRxDatabase);
  if (s_operationsPost.indexOf(operation) >= 0) // already added
  {
    // possible via hover grip points //ODA_FAIL_ONCE(); // test
    return false;
  }

  s_operationsPost.push_back(operation);
  emit postOperations();
  return true;
}

// show / hide snap point

bool OdqViewSystem::highlightSnapPoint(bool bValue, OdDbStub* id, const OdDbBaseFullSubentPath* pPath,
                                       OdDb::OsnapMode osm, // = OdDb::OsnapMode(0)
                                       const OdGePoint3d& point, // = OdGePoint3d()
                                       OdRxObject* pRxDatabase) // = NULL                                                                                   
{
  ODA_ASSERT_ONCE(id && pRxDatabase);
  if (   !id || !pRxDatabase 
      || m_mapViews.find(pRxDatabase) == m_mapViews.end())
    return false;

  PostOperation operation(enPO_HighlightSnapPoint, bValue, pRxDatabase, id, pPath, (int) osm, point);

  if (s_operationsPost.indexOf(operation) >= 0) // already added
  {
    ODA_FAIL_ONCE(); // test
    return false;
  }

  s_operationsPost.push_back(operation);
  emit postOperations();
  return true;
}

// private slot
void OdqViewSystem::postOperationsSlot()
{
  if (s_bPostOperationsBusy)
  {
    //ODA_FAIL_ONCE(); // test
    return;
  }
  s_bPostOperationsBusy = true;

  bool bSkipCreateView = false; // is used to skip it by 1st database
  int idx = 0;
  while (idx < (int) s_operationsPost.size())
  {
    // #9203
    bool bUpdateGl = (idx == int(s_operationsPost.size() - 1)); // for the last only

    PostOperation operation = s_operationsPost[idx];
    ODA_ASSERT_ONCE(   operation.m_type != enPO_None 
                    && (   operation.m_id 
                        || operation.m_type == enPO_HighlightGripPoint
                        || operation.m_type == enPO_OnModifiedGrip
                        || operation.m_type == enPO_HighlightSnapPoint
                        || operation.m_type == enPO_PostCreateView
                        || operation.m_type == enPO_UpdateView));

    if (   (operation.m_type != enPO_PostCreateView || !bSkipCreateView)
        && doOperations(operation, bUpdateGl))
    {
      s_operationsPost.removeFirst();
      continue;
    }

    if (operation.m_type == enPO_PostCreateView)
      bSkipCreateView = true;
    idx++; // is used for enPO_PostCreateView

  } // end for

  s_bPostOperationsBusy = false;
#if 1 // TODO check for is it really useless now
  if (idx)
    QTimer::singleShot(20, this, SLOT(postOperationsSlot())); // emit postOperations();
#endif
}

// return false to keep operation in query (usable for enPO_PostCreateView)
bool OdqViewSystem::doOperations(const PostOperation& operation, 
                                 bool bUpdateGl) // = true
{
  bool bOtherViewFound = false;
  QList<QPointer<OdqView> > lst = m_mapViews.values(operation.m_pRxDatabase);
  foreach (QPointer<OdqView> pView, lst)
  {
    if (pView.isNull())
    {
      ODA_FAIL_ONCE();
      continue;
    }

    switch (operation.m_type)
    {
    case enPO_PostCreateView:
      if (pView->getBusyState() == enBusy_Free)
      {
        bOtherViewFound = true;
        continue;
      }
      return pView->getBusyState() == enBusy_Closed;
    case enPO_UpdateView:
      bUpdateGl = false;
      ODA_ASSERT_ONCE(!operation.m_id);
      pView->updateGL();
      break;
    case enPO_UpdateObject:
      pView->updateObject(operation.m_id);
      break;
    case enPO_HighlightObject:
      pView->highlightObject(operation.m_bValue, operation.m_id, operation.m_pPath.get());
      break;
    case enPO_OnModifiedGrip:
      pView->onModifiedGrip(operation.m_pGripData);
      break;
    case enPO_HighlightGripPoint:
      pView->highlightGripPoint(operation.m_bValue, operation.m_pGripData);
      break;
    case enPO_HighlightSnapPoint:
      pView->highlightSnapPoint(operation.m_bValue, operation.m_id, operation.m_pPath.get(), 
                                (OdDb::OsnapMode) operation.m_index, operation.m_point);
      break;
    default:
        ODA_FAIL_ONCE(); // TODO
        return true;
    } // end switch

    //pView->invalidate();
    if (bUpdateGl)
      pView->updateGL(); // repaint

  } // end foreach

  if (operation.m_type == enPO_PostCreateView && bOtherViewFound)
    createView(operation.m_pRxDatabase, operation.m_id, operation.m_pRect.get());

  return  true;
}
