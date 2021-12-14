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
// main.cpp
//

#include <QtCore>
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtGui>
#else
#include <QtWidgets>
#endif
#include <QDir> 
#include <QSysInfo>
#include <QFile>
#include <QMessageBox>
#include <QFileInfo>

#include "OdqApplication.h"
#include "OdqMainWindow.h"
#include "OdqAppProps.h"

#include "RxDynamicModule.h"
#include "DynamicLinker.h"

#include "ExStringIO.h"
#include "Ed/EdCommandContext.h"
#include "Ed/EdCommandStack.h" 
#include "DbHostAppServices.h"
#define STL_USING_STREAM
#include "OdaSTL.h"

#if defined(_MSC_VER)
 #if _MSC_VER < 1600 // removed in VC 2010
  extern "C" _CRTIMP int __cdecl _set_sbh_threshold(size_t);
  inline void attuneHeap()
  {
    // http://forum.opendesign.com/showthread.php?t=5114&highlight=heap+performance
    // http://support.microsoft.com/?kbid=323635. 
    //
    // On Windows 2000 or Windows XP, repeated loading of large files in TD 
    // may cause performance to degrade. The cause of this degradation is that
    // the CRT small-block heap is not used by default on Windows 2000 or XP. 
    _set_sbh_threshold(1016);
  }
 #else
  #define attuneHeap()
 #endif
  inline void attuneLocale()
  {
    setlocale(LC_NUMERIC, "C"); // saving doubles to dxf
    setlocale(LC_TIME, ""); // correct strftime formatting
  }
#else
 #define attuneHeap()
 #define attuneLocale()
#endif

int main(int argc, char *argv[])
{
  attuneHeap();
  attuneLocale();
  ODA_ASSERT(sizeof(OdChar) == sizeof(wchar_t));
#if defined(ANDROID)
  QString qsPath = "LD_LIBRARY_PATH=/system/lib/egl:";
  qsPath += getenv("LD_LIBRARY_PATH");
  if (putenv(
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
              qsPath.toAscii().data()))

#else
              qsPath.toLatin1().data()))
#endif
  {
    ODA_FAIL_ONCE();
  }
#endif

  //OdqApplication app(argc, argv); // contains services and initialize it
  OdSharedPtr<OdqApplication> pApp = new OdqApplication(argc, argv); // contains services and initialize it
  OdqApplication& app = *pApp.get();

  QStringList lstArgs = app.arguments();
  if (!lstArgs.isEmpty())
    lstArgs.removeFirst(); // skip arg[0] (is application path)
  if (lstArgs.contains("/?") || lstArgs.contains("-?"))
  {
    QString qsTxt = QObject::tr(
      "The syntax for using command line switches is\n" 
      "\"<path>/%1\" [\"drawingname\"] [/switch \"name\"]\n" 
      "\n" 
      "Command line switches are\n"
      "  /s support_folder_path[...;support_folder_path]\n"
      "    Designates support folders other than the current\n"
      "    folder (text fonts, linetypes, and hatch patterns)\n" // TODO ,menus
      "  /ld <path>/tx_application.tx\n"
      "    Loads a specified TX application.\n"
      "  /b script_name\n"
      "    Designates a script to run after you start the program.\n" 
      "  /v view_name\n"
      "    Designates a particular view of the drawing for display at startup.\n"
    ).arg(QFileInfo(app.applicationFilePath()).fileName());
    //QTextStream streamOut(stdout);
    //if (streamOut.device()->isWritable())
    //{
    //  streamOut << qsTxt;
    //  streamOut.flush();
    //}
    QMessageBox::information(QApplication::activeWindow(), app.getAppName(), qsTxt);
    return 0;
  }

  // init static app module and load console
  OdSharedPtr<OdqMainWindow> pWindow = new OdqMainWindow();
  OdqMainWindow& window = *pWindow.get();

  if (!window.isValid())
    return 0;

  QString qsPropProfile = "qrc:/oda_qt_props.xml",
          qsMenuProfile = "qrc:/cui/odq_menu_base.cui",
          qsToolbarsProfile = "qrc:/cui/odq_toolbar_base.cui";

  if (   ::odrxDynamicLinker()->loadModule(OD_T("OdaQtOpenRL.txv")).get()
#   if defined(ODA_WINDOWS)
      || ::odrxDynamicLinker()->loadModule(OD_T("WinOpenRL.txv")).get()
      || ::odrxDynamicLinker()->loadModule(OD_T("BmpOpenRL.txv")).get()
#   endif
     )
    qsPropProfile = "qrc:/oda_qt_props_caustic.xml";

  OdqAppProps* pAppProps = qobject_cast<OdqAppProps*>(app.getSingle(IqAppProps_iid));
  if (!pAppProps || !pAppProps->load(qsPropProfile))
  {
    ODA_FAIL();
  }

  IqConsole* iConsole = getIConsole();
  if (!iConsole)
  {
    ODA_FAIL();
    return 0;
  }

  bool bPersistentUndo = pAppProps->toBool(L"Vars/persistentundo", false); // INT-6233
  if (bPersistentUndo && ::odrxDynamicLinker()->loadApp(OdUndoHistory).isNull())
    bPersistentUndo = false;
  OdDbHostAppServices* pAppServices = OdDbHostAppServices::cast(getIApp()->getBaseHostAppServices()).get();
//  if (bPersistentUndo != pAppServices->getPersistentUndoMode())
//    pAppServices->setPersistentUndoMode(bPersistentUndo); // at start only

  OdInt16 nSnapModes = (OdInt16) pAppProps->toInt(L"Vars/osmode", 4133);
  if (nSnapModes != pAppServices->getOSMODE())
    pAppServices->setOSMODE(nSnapModes);

  // created by ACAD 2009
  iConsole->postponeCommand(QString("menu \"%1\"").arg(qsMenuProfile), true, NULL, 100);
#if !(defined(ANDROID) || defined(__IPHONE_OS_VERSION_MIN_REQUIRED))
  iConsole->postponeCommand(QString("cuiload \"%1\"").arg(qsToolbarsProfile), true, NULL, 100);
  if (getIRevSystem())
    iConsole->executeCommand("cuiload \"qrc:/cui/odq_toolbar_tigdb_tx.cui\"");
  ::odrxDynamicLinker()->loadModule(OdExCommandsModuleName); // "ExCommands.tx"// ::odrxDynamicLinker()->loadModule(OD_T("OdaQtObjects.tx"));
  ::odrxDynamicLinker()->loadModule(ExDimAssocModuleName); //"ExDimAssoc.tx"
  ::odrxDynamicLinker()->loadModule(OdGripPointsModuleName);
  ::odrxDynamicLinker()->loadModule(OdExFieldEvaluatorModuleName); //CORE-11630
  ::odrxDynamicLinker()->loadModule(L"TD_RasterExport.tx"); // for bmpout & bmpoutbg
  //if (   GETBIT(getIAppProps()->toInt(L"Vars/propsrc", 0), 4)
  //    || GETBIT(getIAppProps()->toInt(L"Vars/dlgpropsrc", 0), 4)
  //    || GETBIT(getIAppProps()->toInt(L"Vars/optpropsrc", 0), 4))
  {
    ::odrxDynamicLinker()->loadModule(L"RxProperties");
    ::odrxDynamicLinker()->loadModule(L"DbProperties");
    ::odrxDynamicLinker()->loadModule(L"DgProperties");
  }
#endif
  // TODO // created by ACAD 2010
  //iConsole->postponeCommand("menu \"qrc:/oda_qt_base.cuix\"", false);

  bool bArgOrSwitchFound = false;
  
  // load Startup Suite
  OdEdCommand* pCommand = ::odedRegCmds()->lookupCmd(OD_T("APPLOAD")).get();
  if (pCommand)
  {
    OdEdCommandContext* pCmdCtx = iConsole->getCommandContext();
    ODA_ASSERT_ONCE(pCmdCtx);
    if (pCmdCtx)
    {
      OdSmartPtr<ExStringIO> pStringIO = ExStringIO::create(OD_T("LOADSTARTUPSUITE")); // LOADCONTENTS
      pCmdCtx->setArbitraryData(OD_T("APPLOAD/SUBCOMMAND"), pStringIO);

      try
      {
        pCommand->execute(pCmdCtx);
        bool isLdParam = false;
        foreach (QString qsArg, lstArgs)
        {
          QString qsArgLow = qsArg.toLower();
          if (!isLdParam && (qsArgLow == "/ld" || qsArgLow == "-ld"))
          {
            isLdParam = true;
            continue;
          }
          if (!isLdParam)
            continue;
          isLdParam = false;

          if (qsArg.size() >= 2 && qsArg.startsWith('\"') && qsArg.endsWith('\"'))
            qsArg = qsArg.mid(1, qsArg.size() - 2);

          foreach (QString qsPath, qsArg.split(';', QString::SkipEmptyParts))
          {
            if (qsPath.startsWith('\"'))
            {
              if (qsPath.size() >= 2 && qsPath.endsWith('\"'))
                qsPath = qsPath.mid(1, qsPath.size() - 2);
              else
              {
                ODA_FAIL_ONCE();
              }
            }
            if (qsPath.isEmpty())
              continue;

            if (!bArgOrSwitchFound)
            {
              pCmdCtx->userIO()->putString(app.toOdString(QObject::tr("Command-line switch /ld loading :")));
              bArgOrSwitchFound = true;
            }
            OdSmartPtr<ExStringIO> pLdIO = ExStringIO::create(getIApp()->toOdString(qsPath));
            pCmdCtx->setArbitraryData(OD_T("APPLOAD/SUBCOMMAND"), pLdIO);
            pCommand->execute(pCmdCtx);
            pCmdCtx->setArbitraryData(OD_T("APPLOAD/SUBCOMMAND"), NULL);

          } // end foreach qsPath
        } // end foreach qsArg
      }
      catch (...)
      {
        ODA_FAIL_ONCE();
      }
      pCmdCtx->setArbitraryData(OD_T("APPLOAD/SUBCOMMAND"), NULL);
    }
  }

#if !(defined(ANDROID) || defined(__IPHONE_OS_VERSION_MIN_REQUIRED))
  ::odrxDynamicLinker()->loadModule(OD_T("OdaQtPropSystem.dll"));
#endif

#if defined(ODA_WINDOWS)
  if (   QSysInfo::windowsVersion() < QSysInfo::WV_VISTA
      && qsPropProfile == "qrc:/oda_qt_props_caustic.xml")
  {
    OdRxModule* pModuleGL = ::odrxDynamicLinker()->loadModule(OdWinOpenGLModuleName).get();
    if (pModuleGL)
    {
      QString qsPath = app.toQString(pModuleGL->moduleName());
      qsPath.replace("OdaQtOpenGL", "OdaQtOpenRL");
      if (QFile::exists(qsPath)) // is earlier than Win Vista
      {
        window.show();
        QMessageBox::warning(app.getMainWindow(), 
                             QObject::tr("%1 Warning").arg(app.getAppName()), // caption
                             QObject::tr("You should reinstall the application on Windows Vista"
                                         " or later to have OpenRL%1 rendering support" // OpenRL™
                                         " (see requirements on www.caustic.com).")
                                           .arg(app.toQString(OdString(OD_T("™")))),
                             QMessageBox::Ok, QMessageBox::Ok);
      }
    }
  }
#endif
  
  bool isBParam = false,
       isVParam = false,
       isOtherParam = false;
  QStringList lstOtherParams;
  lstOtherParams << "/s" << "-s" << "/ld" << "-ld"; // << "/v" << "-v";
  bArgOrSwitchFound = false;
  foreach (QString qsArg, lstArgs)
  {
    QString qsArgLow = qsArg.toLower();
    if (!isBParam && !isVParam && (qsArgLow == "/b" || qsArgLow == "-b"))
    {
      isBParam = true;
      continue;
    }
    if (!isBParam && !isVParam && (qsArgLow == "/v" || qsArgLow == "-v"))
    {
      isVParam = true;
      continue;
    }
    if (!isBParam && !isVParam)
    {
      if (isOtherParam)
        isOtherParam = false;
      else if (lstOtherParams.contains(qsArgLow))
        isOtherParam = true;
      else
      {
        bArgOrSwitchFound = true;
        iConsole->postponeCommand(QString("open \"%1\"").arg(qsArg), true, NULL, 100);
      }
      continue;
    }
    if (isVParam)
    {
      isVParam = false;
      if (bArgOrSwitchFound)
        iConsole->postponeCommand(QString("view restore %1").arg(qsArg), true, NULL, 100);
      continue;
    }
    isBParam = false;

    if (qsArg.size() >= 2 && qsArg.startsWith('\"') && qsArg.endsWith('\"'))
      qsArg = qsArg.mid(1, qsArg.size() - 2);

    foreach (QString qsPath, qsArg.split(';', QString::SkipEmptyParts))
    {
      if (qsPath.isEmpty())
        continue;

      bArgOrSwitchFound = true;
      iConsole->postponeCommand(QString("script %1").arg(qsPath), true, NULL, 100);

    } // end foreach qsPath
  } // end foreach qsArg

 #ifdef QT_DEBUG
  app.toStatusBar(QObject::tr("Please wait... (cfg-folder is \"%1\")").
                    arg(QDir::toNativeSeparators(QDir::homePath() + "/ODA")), 3000);
 #else
  app.toStatusBar(QObject::tr("Please wait..."), 200);
 #endif
  window.show();

  int res = 0;
  #if !(defined(ANDROID) || defined(__IPHONE_OS_VERSION_MIN_REQUIRED))
  #if defined(Q_WS_MAC) || defined(Q_OS_MAC) || defined(__APPLE__)
  QString dblClckFileName = app.getDoubleClickedFileName();
  if ((!bArgOrSwitchFound) && !(dblClckFileName.isEmpty())) {
    bArgOrSwitchFound = true;
    iConsole->postponeCommand(QString("open \"%1\"").arg(dblClckFileName), true, NULL, 100);
  }
  #endif
  if (!bArgOrSwitchFound)
    iConsole->postponeCommand("open", true, NULL, 100);
  res = app.exec();
  #else
  #if defined(ANDROID)
    QStyle* android = QStyleFactory::create("Android");
    app.setStyle(android);
  #endif
  if (!bArgOrSwitchFound)
    iConsole->postponeCommand("open", true, NULL, 100);
  ODA_ASSERT(!window.isClosed());
  do {
    res = app.exec();
    if (!window.isClosed())
    {
      if (!window.isVisible())
        window.show();
      if (!window.isBrowserClosed())
        QTimer::singleShot(100, pWindow.get(), SLOT(clickedBrowserCloseButton()));
    }

  //
  // http://sourceforge.net/p/necessitas/tickets/76/
  // http://comments.gmane.org/gmane.comp.lib.qt.android/1874
  // Back button handling is not work ! When i press back my app close.
  } while (!window.isClosed()); // should be closed via closeEvent
  #endif
  pWindow = NULL;
  pApp = NULL;
  return res;
}
