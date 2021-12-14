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
// ConsoleCommands.cpp
//

#include <QtCore>
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
#include <QtWidgets>
#endif
#include <QTabBar>
#include <QTabWidget>
#include <QDockWidget>
#include <QFileInfo>
#include <QDir>
#include <QMainWindow>
#include <QTimer>
#include <QClipboard>
#include <QMimeData>
#include <QSettings>

#include "OdqConsole.h"
#include "ConsoleModule.h"
#include "ExtDbModule.h"
#include "PropServices.h"
#include "OdqAudit.h"
#include "OdqDragTracker.h"
#include "OdqClipData.h"

#include "OdqTdSaveAsFilters.h"
#include "DbIdMapping.h"
#include "Ed/EdCommandContext.h" // #include "DbCommandContext.h"
#include "Ed/EdUserIO.h"
#include "DbLayerTableRecord.h"
#include "DbHostAppServices.h"
#include "DbBlockTableRecord.h"
#include "AbstractViewPE.h"
#include "SysVarPE.h"
#define STL_USING_ALGORITHM
#include "OdaSTL.h"
#include "Gi/GiRasterImage.h"
#include "RxRasterServices.h"
#include "FdField.h"

#include "Ge/GeEllipArc3d.h"
#include "OdDToStr.h"
#include "ConsoleCommands.h"

static OdString getNewDrawingName()
{
  QSettings settings("ODA", 
                     QFileInfo(QCoreApplication::applicationFilePath()).baseName());
  settings.beginGroup(TD_SHORT_STRING_VER_S "/recent");
  QString qsRecentPath = settings.value("lastpath").toString();
  if (qsRecentPath.isEmpty())
  {
    QString qsSelectedFilter = settings.value("filter").toString(),
      qsSelectedExt;
    if (!qsSelectedFilter.isEmpty())
      qsSelectedExt = getIApp()->getExtensionByFilter(qsSelectedFilter, "+");
    if (qsSelectedExt.isEmpty())
      qsSelectedExt = "dwg"; // default
    qsRecentPath = settings.value(QString("%1/path").arg(qsSelectedExt),
                                  getIApp()->getInitialDocumentsPath()).toString();
  }
  settings.endGroup();
  QDir dir(qsRecentPath);
  
  OdString sNewDrawingName;
  for (int index = 1; true; index++)
  {
    QString qsNewDrawingName = QString("Drawing%1.dwg").arg(index);
    if (dir.exists(qsNewDrawingName))
      continue;

    qsNewDrawingName = QDir::toNativeSeparators(dir.absoluteFilePath(qsNewDrawingName));
    sNewDrawingName = getIApp()->toOdString(qsNewDrawingName);
    foreach (OdRxObject* pRxDb, getIConsole()->getOpenDatabases())
    {
      ODA_ASSERT_ONCE(pRxDb);
      OdqConsoleTab& tab = *OdqConsole::getConsoleTab(pRxDb);
      if (sNewDrawingName != tab.getFilePathName())
        continue;

      sNewDrawingName.empty();
      break;
    }
    if (sNewDrawingName.isEmpty())
      continue;
    break;
  }

  return sNewDrawingName;
}

static OdString takeInputString(OdRxObject* pRxDb)
{
  OdString sInput;
  OdqConsoleTab* pTab = OdqConsole::getConsole().getConsoleTab(pRxDb);
  ODA_ASSERT_ONCE(pTab);

  if (pTab && !pTab->getTopCmdState().getInputString().isEmpty())
  {
    CommandState& stateCmd = pTab->getTopCmdState();
    sInput = stateCmd.getInputString();
    stateCmd.setInputString(OdString::kEmpty);
  }
  return sInput;
}

static void moveInputString(OdString sInput, OdRxObject* pRxDbTo)
{
  if (sInput.isEmpty())
    return;

  OdqConsoleTab* pTabTo = OdqConsole::getConsole().getConsoleTab(pRxDbTo);
  ODA_ASSERT_ONCE(pTabTo);

  pTabTo->executeCommand(sInput, true);
}

static void moveInputString(OdRxObject* pRxDbFrom, OdRxObject* pRxDbTo)
{
  moveInputString(takeInputString(pRxDbFrom), pRxDbTo);
}

void OdqCmd_New::execute(OdEdCommandContext* pCmdCtx)
{
  OdDbHostAppServices* pAppServices = OdDbHostAppServices::cast(getIApp()->getBaseHostAppServices()).get();
  ODA_ASSERT_ONCE(pAppServices);
  if (!pAppServices)
    throw OdError(eNotApplicable);

  OdString sMsg; 
  OdString sTemplatePathName = getIAppProps()->toString(OD_T("Options/Files/TemplateSettings/FileNameForQNew"));
  if (   !sTemplatePathName.isEmpty()
      && !QFileInfo(getIApp()->toQString(sTemplatePathName)).exists())
  {
    ODA_FAIL_ONCE();
    sMsg = getIApp()->toOdString(
      QObject::tr("Template file \"%1\" was not found").arg(getIApp()->toQString(sTemplatePathName)));
    getIApp()->getBaseHostAppServices()->warning(sMsg);
    sTemplatePathName.empty();
  }
  bool bViaConsole = (   !pAppServices->getFILEDIA() 
                      || getIConsole()->isInputStringReady(pCmdCtx->baseDatabase()));
  OdDbDatabasePtr pDatabase; 

  // default value of reg variable startup :
  //  is 0 for ACAD
  //  is 1 for us if template folder was not found
  bool bStartup = getIAppProps()->toBool(OD_T("Vars/startup"), false);
  if (!bStartup || !sTemplatePathName.isEmpty())
  {
    OdString sTemplatePath,
             sTemplateName;
    if (sTemplatePathName.isEmpty())
    {
      sTemplatePath = getIAppProps()->toString(OD_T("Options/Files/TemplateSettings/FileLocation"));
      if (sTemplatePath.isEmpty())
      {
        // try to get it via the installed ACAD on computer
        foreach(QString qsKey, getIApp()->getAcadRegProfileGeneralKeys())
        {
          QSettings settings(qsKey, QSettings::NativeFormat);
          QString qsPath = settings.value("TemplatePath", "").toString();
          if (qsPath.isEmpty() || !QDir(qsPath).exists())
            continue;
          sTemplatePath = getIApp()->toOdString(qsPath);
          break;
        }
      }
      if (!sTemplatePath.isEmpty())
      {
        QDir dir(getIApp()->toQString(sTemplatePath));
        ODA_ASSERT_ONCE(dir.exists());
        if (dir.exists("acad.dwt"))
        {
          sTemplateName = OD_T("acad.dwt");
          sTemplatePathName = getIApp()->toOdString(QDir::toNativeSeparators(dir.absoluteFilePath("acad.dwt")));
        }
      }
    }
    else
    {
      QFileInfo info(getIApp()->toQString(sTemplatePathName));
      sTemplateName = getIApp()->toOdString(info.fileName());
      sTemplatePath = getIApp()->toOdString(QDir::toNativeSeparators(info.filePath()));
    }

    if (!sTemplatePathName.isEmpty())
    {
      if (!sTemplatePathName.isEmpty() || !skipTempleteDialogByNonEmptyQNewTemplate())
      {
        if (bViaConsole)
        {
          ODA_ASSERT_ONCE(!sTemplateName.isEmpty());
          OdString sPrompt;
          sPrompt.format(OD_T("Enter template file name <%ls>"), sTemplateName.c_str());

          sTemplatePathName = pCmdCtx->userIO()->getString(sPrompt, OdEd::kInpThrowEmpty, sTemplateName, sTemplateName);
          if (!sTemplatePathName.isEmpty())
          {
            QString qsTemplatePathName = getIApp()->toQString(sTemplatePathName);
            if (!QFileInfo(qsTemplatePathName).exists())
            {
              QDir dir(getIApp()->toQString(sTemplatePath));
              qsTemplatePathName = QDir::toNativeSeparators(dir.absoluteFilePath(qsTemplatePathName));
              if (QFileInfo(qsTemplatePathName).exists())
                sTemplatePathName = getIApp()->toOdString(qsTemplatePathName);
              else
              {
                sMsg = getIApp()->toOdString(
                  QObject::tr("Template file \"%1\" was not found").arg(qsTemplatePathName));
                getIApp()->getBaseHostAppServices()->warning(sMsg);
                sTemplatePathName.empty();
              }
            }
          }
        }
        else
          sTemplatePathName = pCmdCtx->userIO()->getFilePath(OD_T("Enter template file name"),
                                                         OdEd::kGfpForOpen, 
                                                         OD_T("Select template"), // dialog caption 
                                                         OD_T("dwt"), // default extension
                                                         sTemplatePathName, // file name
                                                         OD_T("Drawing template (*.dwt)|*.dwt||"));
        if (sTemplatePathName.isEmpty())
          return; // canceled
      }
      pDatabase = pAppServices->readFile(sTemplatePathName);
    }
    else
    {
      sMsg = getIApp()->toOdString(
        QObject::tr("Folder with templates was not found.\n"
                    "Enter in this console :\n"
                    "  \"ddtemplatepaths dialog\" to set path of folder with your templates\n"
                    "  \"startup false\" to restore working command \"new\" via templates\n"));
      pAppServices->warning(sMsg);
      int index = pCmdCtx->userIO()->getKeyword(OD_T("Do you want to create drawing from scratch [Yes/No]<Yes>"),
                                                OD_T("Yes No"), 0);
      ODA_ASSERT_ONCE(!index || index == 1);
      if (index)
        return;
      if (!getIAppProps()->set(OD_T("Vars/startup"), true, OD_T("bool")))
      {
        ODA_FAIL_ONCE();
      }
    }
  }
  
  if (pDatabase.isNull())
  {
    bool bMeasurement = getIAppProps()->toBool(OD_T("Vars/measurement"), false);
    OdDb::MeasurementValue measureDef = bMeasurement ? OdDb::kMetric : OdDb::kEnglish,
                           measure = measureDef;
    if (bViaConsole)
    {
      OdString sPrompt;
      sPrompt.format(OD_T("Enter settings for new from scratch [Imperial/Metric]<%ls>"), 
                     bMeasurement ? OD_T("Metric") : OD_T("Imperial"));

      int index = pCmdCtx->userIO()->getKeyword(sPrompt, OD_T("Imperial Metric"), measure);
      ODA_ASSERT_ONCE(index == OdDb::kEnglish || index == OdDb::kMetric);
      if (index != measure && (index == OdDb::kEnglish || index == OdDb::kMetric))
        measure = (OdDb::MeasurementValue) index;
    }
    else
    {
      OdPropServicesPtr pPropServices = OdPropServices::cast(pCmdCtx);
      ODA_ASSERT_ONCE(!pPropServices.isNull());
      if (pPropServices.isNull())
        throw OdError(eNotApplicable);

      OdString sDlgKey = pPropServices->openDialog(L"%ls - Create New Drawing from Scratch",
                                                   NULL, false, OdPropServices::enSepCatPlace_None,
                                                   0.35, 0.25);
      ODA_ASSERT_ONCE(!sDlgKey.isEmpty());
      if (sDlgKey.isEmpty())
        return;
      class NewDrawingPropSourceImpl : public OdPropSource
      {
        OdEdCommandContext* m_pCmdCtx;
      public:
        OdDb::MeasurementValue m_measure;

        NewDrawingPropSourceImpl(OdEdCommandContext* pCmdCtx,
                                 OdDb::MeasurementValue measure)
         : m_pCmdCtx(pCmdCtx)
         , m_measure(measure)
        {
        }

        virtual bool updateXml(OdString& sXml)
        {
          sXml += OD_T("<Property Path=\"New\" Name=\"FromScratch\" VisibleName=\"New from Scratch Settings\"  Type=\"category\" Layout=\"hor\"  Alignment=\"center\" />\n");
          sXml += OdString().format(OD_T("<Property Path=\"New/FromScratch\" Name=\"Imperial\" Type=\"radio\" Value=\"%ls\" />\n"),
                                    (m_measure == OdDb::kEnglish) ? OD_T("true") : OD_T("false"));
          sXml += OdString().format(OD_T("<Property Path=\"New/FromScratch\" Name=\"Metric\" Type=\"radio\" Value=\"%ls\" />\n"),
                                    (m_measure == OdDb::kMetric) ? OD_T("true") : OD_T("false"));

          sXml += OD_T("<Property Path=\"New\" Name=\"StandardButtons\" Type=\"category\" Layout=\"hor\" Alignment=\"right\" />\n");
          sXml += OD_T("<Property Path=\"New/StandardButtons\" Name=\"Ok\" Type=\"button\" />\n");
          sXml += OD_T("<Property Path=\"New/StandardButtons\" Name=\"Cancel\" Type=\"button\" />\n");
          return true;
        }
        virtual bool setPropertyValue(const OdString& csPath,
                                      const OdString& sValue,
                                      Action& action)
        {
          OdString sPath = csPath;
          if (extractFirst(sPath) != OD_T("New"))
            return false;
          OdString sName = extractFirst(sPath);

          if (sName == OD_T("StandardButtons"))
          {
            if (sPath == OD_T("Ok"))
            {
              action = enAction_Close;
              return true;
            }
            ODA_ASSERT_ONCE(sPath == OD_T("Cancel"));

            action = enAction_Cancel;
            return true;
          }

          if (sName == OD_T("FromScratch"))
          {
            ODA_ASSERT_ONCE(sValue == OD_T("true") || sValue == OD_T("false"));
            bool bValue = (sValue == OD_T("true"));
            
            if (sPath == OD_T("Imperial"))
              m_measure = bValue ? OdDb::kEnglish : OdDb::kMetric;
            else if (sPath == OD_T("Metric"))
              m_measure = bValue ? OdDb::kMetric : OdDb::kEnglish;
            else
            {
              ODA_FAIL_ONCE();
            }
            return true;
          }

          return false;
        }
      };
      NewDrawingPropSourceImpl source(pCmdCtx, measureDef);
      pPropServices->addSource(sDlgKey, &source, OD_T("New/FromScratch"));
      pPropServices->addSource(sDlgKey, &source, OD_T("New/StandardButtons"));

      bool bRes = pPropServices->executeDialog(sDlgKey);
      measure = source.m_measure;
      pPropServices->closeDialog(sDlgKey);
      if (!bRes)
        return; // canceled
    }

    if (   measure != measureDef
        && !getIAppProps()->set(OD_T("Vars/measurement"), !bMeasurement, OD_T("bool")))
    {
      ODA_FAIL_ONCE();
    }

    pDatabase = pAppServices->createDatabase(true, measure);
  }
  ODA_ASSERT_ONCE(!pDatabase.isNull());
  if (pDatabase.isNull())
    return;

  OdString sNewDrawingName = getNewDrawingName();
  OdqConsoleTab& tab = *OdqConsole::getConsoleTab(pDatabase, true, sNewDrawingName); // create and activate tab control
  //tab.setFilePathName(sNewDrawingName);
  qApp->processEvents(); // show it

  sMsg = getIApp()->toOdString(QObject::tr("Default Database Created."));
  tab.getCmdIO()->putString(sMsg);

  getIApp()->toStatusBar();
  getIApp()->refreshQObjetcts("<event-refresh:drawing_history_changed>", 
                              getIApp()->toQString(sNewDrawingName));

  IqViewSystem* iVS = NULL;
  if (   ::odrxDynamicLinker()->loadModule(OD_T("OdaQtViewSystem.dll")).isNull()
      || !(iVS = getIViewSystem()))
  {
    sMsg = getIApp()->toOdString(
      QObject::tr("Module %1 was not found or invalid").arg("OdaQtViewSystem.dll"));
    pAppServices->warning(sMsg);
    return;
  }
  ::odrxDynamicLinker()->loadModule(OdGripPointsModuleName);

  iVS->createViews(pDatabase);
  moveInputString(pCmdCtx->baseDatabase(), pDatabase);
}

void OdqCmd_QNew::execute(OdEdCommandContext* pCmdCtx)
{
  OdqCmd_New::execute(pCmdCtx);
}

void OdqCmd_Open::execute(OdEdCommandContext* pCmdCtx)
{
  OdString sPath;
 
  OdDbHostAppServices* pServices = OdDbHostAppServices::cast(getIApp()->getBaseHostAppServices());
  ODA_ASSERT_ONCE(pServices);
  bool bFileDialog = pServices->getFILEDIA();

  if (!bFileDialog || getIConsole()->isInputStringReady(pCmdCtx->baseDatabase()))
  {
    sPath = pCmdCtx->userIO()->getString(OD_T("Enter name of drawing to open"));
    if (!sPath.isEmpty() && !QFileInfo(getIApp()->toQString(sPath)).exists())
    {
      OdString sMsg = getIApp()->toOdString(
        QObject::tr("Drawing \"%1\" was not found").arg(getIApp()->toQString(sPath)));
      getIApp()->getBaseHostAppServices()->warning(sMsg);
      return;
    }
  }

  static OdStringArray s_extensions;
  static std::map<OdString, OdString> s_mapExtBySynonym;
  static OdString s_sExtension;
  static OdString s_sFilters;
  static OdStringArray s_lstExtDbNames;
  {
    OdStringArray lst = getIApp()->getExtDbModuleNames(pCmdCtx->baseDatabase());
    if (s_lstExtDbNames.size() != lst.size())
    {
      s_sFilters.empty(); // to recollect
      s_extensions.clear();
      s_mapExtBySynonym.clear();
      s_lstExtDbNames = lst;
    }
  }
  if (sPath.isEmpty())
  {
    if (s_sFilters.isEmpty())
    {
      // fill filters

      s_sFilters += L"DWG Drawing files (*.dwg)|*.dwg|";
      s_sFilters += L"DXF files (*.dxf)|*.dxf|";
      s_sFilters += L"DWS Standards files (*.dws)|*.dws|";
      s_sFilters += L"DWT Drawing Template files (*.dwt)|*.dwt|";
      if (s_sExtension.isEmpty())
        s_sExtension = L"dwg";
      s_extensions.push_back(L"dwg"); s_extensions.push_back(L"dxf");
      s_extensions.push_back(L"dws"); s_extensions.push_back(L"dwt");
      OdString sAllExts = L"*.dwg *.dxf *.dws *.dwt",
               sAllDrawingExts = sAllExts;

      for (unsigned int idx = 0, sz = s_lstExtDbNames.size(); idx < sz; idx++)
      {
        OdString sModuleName = s_lstExtDbNames[idx];
        OdExtDbModule* pExtDb = OdExtDbModule_cast(::odrxDynamicLinker()->loadModule(sModuleName));
        if (!pExtDb)
          continue;
        bool isImportToDwg = false;
        try
        {
          OdRxObjectPtr pRxDb = pExtDb->createUninitializedDatabase();
          if (pRxDb.get())
            isImportToDwg = !OdDbDatabase::cast(pRxDb).isNull();
        }
        catch (OdError&)
        {
          ODA_FAIL_ONCE();
        }
        OdStringArray exts = pExtDb->getExtensions();
        for (unsigned int idxExt = 0, szExt = exts.size(); idxExt < szExt; idxExt++)
        {
          OdString sExt = exts[idxExt]; 
          sExt.makeLower();
          OdStringArray extSynonyms;
          OdString sFilter = pExtDb->getFilterByExtension(sExt, 0, OdEd::kGfpForOpen, &extSynonyms);
          ODA_ASSERT_ONCE(pExtDb->getFilterByExtension(sExt, 1).isEmpty()); // TODO
          if (sFilter.isEmpty())
            continue;
          if (!s_extensions.contains(sExt))
            s_extensions.push_back(sExt);

          OdString sFltExts = L"*." + sExt;
          unsigned int szSyn = extSynonyms.size();
          if (szSyn)
          {
            for (unsigned int idxSyn = 0, szSyn = extSynonyms.size(); idxSyn < szSyn; idxSyn++)
            {
              OdString extSyn = extSynonyms[idxSyn];
              ODA_ASSERT_ONCE(!extSyn.isEmpty() && !exts.contains(extSyn));

              sFltExts += L" *." + extSyn;
              s_mapExtBySynonym[extSyn] = sExt;
            }
            sFilter.replace(L';', L' '); // problem with QFileDialog
          }
          sAllExts += L" " + sFltExts;
          if (!isImportToDwg)
            sAllDrawingExts += L" " + sFltExts; 

          if (sFilter.find(L"|*") < 0)
            sFilter += OdString().format(L"|%ls|", sFltExts.c_str());

          s_sFilters += sFilter;
        }
      }

      s_sFilters += OdString().format(L"All drawings (%ls)|%ls|", sAllDrawingExts.c_str(), sAllDrawingExts.c_str());
      if (sAllDrawingExts != sAllExts)
        s_sFilters += OdString().format(L"All files (%ls)|%ls|", sAllExts.c_str(), sAllExts.c_str());
      s_sFilters += L"|";
      // or 
      // s_sFilters += L"All files (*.*)|*.*||";
    }

    if (s_sFilters.isEmpty())
    {
      ODA_FAIL_ONCE();
      throw OdError(eNotApplicable); // There is no acceptable filter to select a file
    }

    sPath = pCmdCtx->userIO()->getFilePath(OD_T("Enter name of drawing to open"), 
                                           OdEd::kGfpForOpen, 
                                           OD_T("Select File"), // dialog caption 
                                           s_sExtension, // default extension
                                           OdString::kEmpty, // file name
                                           s_sFilters);
    if (sPath.isEmpty())
      return; // without message "Cancel by user" // throw OdEdCancel();
  }

  OdDbBaseHostAppServices* pBaseAppServices = getIApp()->getBaseHostAppServices();
  ODA_ASSERT_ONCE(pBaseAppServices);

  QFileInfo info(getIApp()->toQString(sPath));
  if (!info.suffix().isEmpty())
  {
    s_sExtension = getIApp()->toOdString(info.suffix().toLower());
    if (!s_extensions.contains(s_sExtension))
    {
      if (!s_mapExtBySynonym[s_sExtension].isEmpty())
        s_sExtension = s_mapExtBySynonym[s_sExtension];
      ODA_ASSERT_ONCE(s_extensions.isEmpty() || s_extensions.contains(s_sExtension));
    }
  }
  else
    ODA_FAIL_ONCE(); // suspicion to invalid file

  // check for already opened databases
  QList<OdRxObject*> lstRxDatabases = getIConsole()->getOpenDatabases();
  foreach (OdRxObject* pRxDb, lstRxDatabases)
  {
    OdString sDbPath = getIConsole()->getFilePathName(pRxDb); 
    if (sDbPath == sPath)
    {
      OdqConsole::getConsoleTab(pRxDb, true); // set active
      getIApp()->refreshQObjetcts("<event-refresh:drawing_history_changed>", 
                                  getIApp()->toQString(sPath));
      return;
#ifdef ANDROID
    } else {
      getIConsole()->postponeCommand(QString("close "),
                                 false, pRxDb);
#endif
    }
  }

  if (   s_sExtension.isEmpty() 
      || (!s_extensions.isEmpty() && !s_extensions.contains(s_sExtension)))
  {
    ODA_FAIL_ONCE();
    s_sExtension = L"dwg"; // restore default
  }

  OdDbHostAppServices* pAppServices = OdDbHostAppServices::cast(pBaseAppServices);
  if (!pAppServices)
  {
    ODA_FAIL_ONCE();
    throw OdError(eNotApplicable);
  }

  getIApp()->toStatusBar(QObject::tr("Loading of file %1").arg(info.fileName()));

  OdRxObjectPtr pRxDatabase; 
  OdExtDbModule* pExtDb = getIApp()->getExtDbModuleByExtension(s_sExtension);
  OdResult res = eOk;
  try
  {
    if (!pExtDb)
    {
      pRxDatabase = pAppServices->readFile(sPath);
      ::oddbSetFieldEngineValueFormatByError(L"%ls"); // keep result of previous evaluation
      ::oddbEvaluateFields(OdDbDatabase::cast(pRxDatabase).get(), OdDbField::kOpen);
    }
    else
      pRxDatabase = pExtDb->readFile(sPath, Oda::kShareDenyWrite);
  }
  catch (const OdError& err)
  {
    res = err.code();
#ifndef Q_WS_WIN
    if (res != eDwgFileIsEncrypted)
      pCmdCtx->userIO()->putString(OdString().format(L"Error: %ls", err.description().c_str()));
#endif
  }

  if (pRxDatabase.isNull())
  {
    QString qsMess;
    if (!pExtDb)
    {
      OdPropServicesPtr pPropServices = OdPropServices::cast(pCmdCtx);
      ODA_ASSERT_ONCE(!pPropServices.isNull());
      if (   pPropServices.get() 
                // #17626
                // Encrypting is supported for Windows only.
                // Therefore we have recover dialog instead password request for other platforms.
                // see odrxInitWinNTCrypt() and odrxInitCryptlibCrypt()
                // (Cryptlib is not distributed with the ODA Platform. It must be obtained separately.
                //  See http://www.cryptlib.com for details.)
          && res != eDwgFileIsEncrypted && res != eDecryptionError)
      {
        qsMess = QObject::tr("Errors were detected in the drawing. "
                             "The file cannot be opened without being recovered. "
                             "Use command RECOVER to do it.\n\"%1\"")
                               .arg(QDir::toNativeSeparators(info.canonicalFilePath()));
        if (!pPropServices->messageBox(OD_T("%ls Error"), getIApp()->toOdString(qsMess), 
                                      //OD_T("~OK"), NULL);
                                       OD_T("~Recover"), OD_T("Cancel"), NULL))
          getIConsole()->postponeCommand(QString("recover \"%1\"").arg(QDir::toNativeSeparators(info.canonicalFilePath())), true, NULL, 100);
      }
    }
    qsMess = QObject::tr("Loading of file \"%1\" was failed.%2")
               .arg(QDir::toNativeSeparators(info.canonicalFilePath()))
               .arg(  (res == eDwgFileIsEncrypted || res == eDecryptionError) 
                    ? " File is encrypted." : "");
#if !defined(_USE_CRYPTLIB) && !defined(Q_OS_WIN) && !defined(Q_WS_WIN)
    if (res == eDwgFileIsEncrypted || res == eDecryptionError)
    // #17626
    // Encrypting is supported for Windows only.
    // Therefore we have recover dialog instead password request for other platforms.
    // see odrxInitWinNTCrypt() and odrxInitCryptlibCrypt()
    // (Cryptlib is not distributed with the ODA Platform. It must be obtained separately.
    //  See http://www.cryptlib.com for details.)
      qsMess += "\nCryptlib is not distributed with ODA SDK. Therefore encrypting is supported for Windows only.";
#endif

    throw OdError(getIApp()->toOdString(qsMess));
  }

  OdString sFilePathNameOverride;
  if (pExtDb) 
  {
    if (OdDbDatabase* pDwgDb = OdDbDatabase::cast(pRxDatabase).get())
    {
      OdString sName = pDwgDb->getFilename();
      if (sName.isEmpty())
        sFilePathNameOverride = sPath + L".dwg"; // set nane of new drawing with import result // PROD-59 for TD_ImgDb.tx
      else if (s_sExtension == L"dsf" || s_sExtension == L"tig")
        sFilePathNameOverride = sPath;  // sFilePathNameOverride = sPath + L"." + sName + L".dwg"; // sName is "master" usually
    }
  }
  //OdqConsoleTab& tab = *
  OdqConsole::getConsoleTab(pRxDatabase, true, sFilePathNameOverride); // create and activate tab control

  qApp->processEvents(); // show it

  getIApp()->toStatusBar();
  getIApp()->refreshQObjetcts("<event-refresh:drawing_history_changed>", 
                              getIApp()->toQString(sPath));

  IqViewSystem* iVS = NULL;
  if (   ::odrxDynamicLinker()->loadModule(OD_T("OdaQtViewSystem.dll")).isNull()
      || !(iVS = getIViewSystem()))
  {
    OdString sMsg = getIApp()->toOdString(
        QObject::tr("Module %1 was not found or invalid").arg("OdaQtViewSystem.dll"));
    pBaseAppServices->warning(sMsg);
    return;
  }
  ::odrxDynamicLinker()->loadModule(OdGripPointsModuleName);

  iVS->createViews(pRxDatabase);
  moveInputString(pCmdCtx->baseDatabase(), pRxDatabase);
}

void OdqCmd_Recover::execute(OdEdCommandContext* pCmdCtx)
{
  OdString sPath;

  OdDbHostAppServices* pServices = OdDbHostAppServices::cast(getIApp()->getBaseHostAppServices());
  ODA_ASSERT_ONCE(pServices);
  bool bFileDialog = pServices->getFILEDIA();

  if (!bFileDialog || getIConsole()->isInputStringReady(pCmdCtx->baseDatabase()))
  {
    sPath = pCmdCtx->userIO()->getString(OD_T("Enter name of drawing to open"));
    if (!sPath.isEmpty() && !QFileInfo(getIApp()->toQString(sPath)).exists())
    {
      OdString sMsg = getIApp()->toOdString(
        QObject::tr("Drawing \"%1\" was not found").arg(getIApp()->toQString(sPath)));
      getIApp()->getBaseHostAppServices()->warning(sMsg);
      return;
    }
  }

  static OdString s_sExtension;
  if (sPath.isEmpty())
  {
    static OdString s_sFilters;
    if (s_sFilters.isEmpty())
    {
      // fill filters for .dwg files only (DGN has not recovering functionality
      s_sFilters += L"DWG Drawing files (*.dwg)|*.dwg|";
      s_sFilters += L"DXF files (*.dxf)|*.dxf|";
      //s_sFilters += OD_T("Binary DXF files (*.dxb)|*.dxb|");
      s_sFilters += L"DWS Standards files (*.dws)|*.dws|";
      s_sFilters += L"DWT Drawing Template files (*.dwt)|*.dwt|";
      s_sFilters += OD_T("|"); // or // s_sFilters += OD_T("All files (*.*)|*.*||");
      if (s_sExtension.isEmpty())
        s_sExtension = OD_T("dwg");
    }
    if (s_sFilters.isEmpty())
    {
      ODA_FAIL_ONCE();
      throw OdError(eNotApplicable); // There is no acceptable filter to select a file
    }

    sPath = pCmdCtx->userIO()->getFilePath(OD_T("Enter name of drawing to recover"), 
                                           OdEd::kGfpForOpen, 
                                           OD_T("Select File"), // dialog caption 
                                           s_sExtension, // default extension
                                           OdString::kEmpty, // file name
                                           s_sFilters);
    if (sPath.isEmpty())
      return; // without message "Cancel by user" // throw OdEdCancel();
  }

  OdDbBaseHostAppServices* pBaseAppServices = getIApp()->getBaseHostAppServices();
  ODA_ASSERT_ONCE(pBaseAppServices);

  QFileInfo info(getIApp()->toQString(sPath));
  if (!info.suffix().isEmpty())
    s_sExtension = getIApp()->toOdString(info.suffix().toLower());
  else
    ODA_FAIL_ONCE(); // suspicion to invalid file

  // check for already opened databases
  QList<OdRxObject*> lstRxDatabases = getIConsole()->getOpenDatabases();
  foreach (OdRxObject* pRxDb, lstRxDatabases)
  {
    OdString sDbPath = getIConsole()->getFilePathName(pRxDb); 
    if (sDbPath == sPath)
    {
      OdqConsole::getConsoleTab(pRxDb, true); // set active
      getIApp()->refreshQObjetcts("<event-refresh:drawing_history_changed>", 
                                  getIApp()->toQString(sPath));
      return;
    }
  }
  if (s_sExtension != OD_T("dwg") && s_sExtension != OD_T("dxf") && s_sExtension != OD_T("dxb"))
  {
    ODA_FAIL_ONCE();
    s_sExtension = OD_T("dwg"); // restore default
  }

  OdDbHostAppServices* pAppServices = OdDbHostAppServices::cast(pBaseAppServices);
  if (!pAppServices)
  {
    ODA_FAIL_ONCE();
    throw OdError(eNotApplicable);
  }

  getIApp()->toStatusBar(QObject::tr("Recovering of file %1").arg(info.fileName()));

  OdDbDatabasePtr pDatabase = pAppServices->createDatabase(false);
  OdRxObject* pRxDatabase = OdRxObject::cast(pDatabase).get();

  OdqConsoleTab& tab = *OdqConsole::getConsoleTab(pRxDatabase, 
                                                  true); // create and activate tab control

  OdqAuditInfo infoAudit(pRxDatabase);
  infoAudit.setFixErrors(true);
  infoAudit.setPrintDest(OdDbAuditInfo::kCmdLine); //kBoth

  try
  {
    // recoverFile
    OdStreamBufPtr pFileBuff = odrxSystemServices()->createFile(sPath);
    pDatabase->readFile(pFileBuff, false, &infoAudit);
    ::oddbSetFieldEngineValueFormatByError(L"%ls"); // keep result of previous evaluation
    ::oddbEvaluateFields(OdDbDatabase::cast(pRxDatabase).get(), OdDbField::kOpen);
  }
  catch (const OdError& e)
  {
    if (!getIConsole()->closeDatabase(pRxDatabase)) // if (!OdqConsole::removeConsoleTab(pRxDatabase))
    {
      ODA_FAIL_ONCE();
    }
    //QString qsMess = QObject::tr("  Recovering of file \"%1\" was failed.").arg(QDir::toNativeSeparators(info.canonicalFilePath()));
    //throw OdError(getIApp()->toOdString(qsMess));

    ODA_ASSERT_ONCE(OdPropServices::cast(pCmdCtx).get());

    OdString sMsg = OdPropServices::cast(pCmdCtx)->tr("Error Recovering Database...");
    sMsg += e.description();
    pCmdCtx->userIO()->putString(sMsg);
    return;
  }

  tab.updateQTitle();

  //if (!infoAudit.numErrors()) // infoAudit.numFixes()
  //  pAppServices->auditPrintReport(&infoAudit, OD_T("Test..."), infoAudit.getPrintDest());

  getIApp()->toStatusBar();
  getIApp()->refreshQObjetcts("<event-refresh:drawing_history_changed>", 
                              getIApp()->toQString(sPath));
  getIApp()->refreshQObjetcts("<event-refresh:database_changed>");

  IqViewSystem* iVS = NULL;
  if (   ::odrxDynamicLinker()->loadModule(OD_T("OdaQtViewSystem.dll")).isNull()
      || !(iVS = getIViewSystem()))
  {
    OdString sMsg = getIApp()->toOdString(
      QObject::tr("Module %1 was not found or invalid").arg("OdaQtViewSystem.dll"));
    pBaseAppServices->warning(sMsg);
    return;
  }
  ::odrxDynamicLinker()->loadModule(OdGripPointsModuleName);

  iVS->createViews(pRxDatabase);
  moveInputString(pCmdCtx->baseDatabase(), pRxDatabase);
}

void OdqCmd_Audit::execute(OdEdCommandContext* pCmdCtx)
{
  OdDbDatabasePtr pDwgDb = OdDbDatabase::cast(pCmdCtx->baseDatabase());
  if (pDwgDb.isNull())
    throw OdError(eNoDatabase); // there is no audit functionality for DGN

  OdEdUserIO* pIO = pCmdCtx->userIO();
  bool bFixErrors = !pIO->getKeyword(OD_T("Fix any errors detected? [Yes/No] <No>"), 
                                     OD_T("Yes No"), 1, OdEd::kGstNoEmpty);
  try
  {
    // Audit
    OdqAuditInfo info(pDwgDb);
    info.setFixErrors(bFixErrors);
    info.setPrintDest(OdDbAuditInfo::kCmdLine); //kBoth

    pDwgDb->auditDatabase(&info);

    //if (!info.numErrors()) // info.numFixes()
    //  getIApp()->getBaseHostAppServices()->auditPrintReport(&info, OD_T("Test..."), info.getPrintDest());
    //  //pIO->putString(...);
  }
  catch (const OdError& e)
  {
    //theApp.reportError(_T("Error Auditing Database..."), e);
    ODA_ASSERT_ONCE(OdPropServices::cast(pCmdCtx).get());

    OdString sMsg = OdPropServices::cast(pCmdCtx)->tr("Error Auditing Database...");
    sMsg += e.description();
    pIO->putString(sMsg);
  }
}

void OdqCmd_SaveAs::execute(OdEdCommandContext* pCmdCtx)
{
  static OdqTdSaveAsFilters s_filtersDwg;

  OdRxObject* pRxDatabase = pCmdCtx->baseDatabase();
  if (!pRxDatabase)
    throw OdError(eNoDatabase); // there is nothing to save

  OdEdUserIO* pIO = pCmdCtx->userIO();
  OdExtDbModule* pExtDb = getIApp()->getExtDbModule(pRxDatabase);
  OdStringArray extensionExtDb;
  if (pExtDb)
  {
    extensionExtDb = pExtDb->getExtensions(OdEd::kGfpForSave, pRxDatabase);
    if (!extensionExtDb.size())
    {
      if (getIConsole()->isInputStringReady(pCmdCtx->baseDatabase()))
        pIO->getString(L"skip",  OdEd::kGstAllowSpaces | OdEd::kGstNoEmpty);
      pIO->putString(L"This format is unsupported to save.");
      return;
    }
  }

  OdString sPath;
  QFileInfo infoPrev(getIApp()->toQString(getIConsole()->getFilePathName(pRxDatabase)));

  OdDbDatabasePtr pDwgDb = OdDbDatabase::cast(pCmdCtx->baseDatabase());
  OdDb::SaveType typeSaveAs = OdDb::kDwg;
  OdDb::DwgVersion verSaveAs = OdDb::kDHL_CURRENT; //OdDb::vAC24;
  if (pDwgDb.get())
  {
    // get drawing format
    verSaveAs = pDwgDb->lastSavedAsVersion();
    if (verSaveAs == OdDb::kDHL_MC0_0) // initial
      verSaveAs = pDwgDb->originalFileVersion();

    // invalid after first resaving
    //typeSaveAs = pDwgDb->originalFileType();
    //if (typeSaveAs == OdDb::kDxf && infoPrev.suffix().toLower() == "dxb")
    //  typeSaveAs = OdDb::kDxb;
    QString qsExt = infoPrev.suffix().toLower();
    typeSaveAs = (qsExt == "dxf") ? OdDb::kDxf : (qsExt == "dxb") ? OdDb::kDxb : OdDb::kDwg;
  }

  OdDbHostAppServices* pServices = OdDbHostAppServices::cast(getIApp()->getBaseHostAppServices());
  ODA_ASSERT_ONCE(pServices);
  bool bFileDialog = pServices->getFILEDIA();
  QString qsExtension;

  if (!bFileDialog || getIConsole()->isInputStringReady(pCmdCtx->baseDatabase()))
  {
    if (pDwgDb.get())
    {
      OdString sMsg = getIApp()->toOdString(
                        QObject::tr("Current file format: %1 %2 Drawing")
                          .arg(s_filtersDwg.getKeyword(verSaveAs, typeSaveAs))
                          .arg((typeSaveAs == OdDb::kDwg) ? "DWG"
                                                          : ((typeSaveAs == OdDb::kDxf) ? "ASCII DXF" 
                                                                                        : "Binary DXF")));
      pIO->putString(sMsg);
      
      int indexKeyword = 0;
      do 
      {
        QString qsPrompt = "Enter file format",
                qsKeywords;
        int indexDefault = 0;
        s_filtersDwg.getKeywordsDataIO(verSaveAs, typeSaveAs, qsPrompt, qsKeywords, indexDefault);
        indexKeyword = pIO->getKeyword(getIApp()->toOdString(qsPrompt), 
                                       getIApp()->toOdString(qsKeywords),
                                       indexDefault, OdEd::kGstDefault); // | OdEd::kGstNoEmpty

      } 
      while (indexKeyword < 0 || s_filtersDwg.updateTypeByIndexKeyword(indexKeyword, typeSaveAs));

      verSaveAs = s_filtersDwg.getVersionByIndexKeyword(typeSaveAs, indexKeyword);
      qsExtension = (typeSaveAs == OdDb::kDwg) ? "dwg" : (typeSaveAs == OdDb::kDxf) ? "dxf" : "dxb";
    }
    else
    {
      ODA_ASSERT_ONCE(extensionExtDb.size());
      qsExtension = extensionExtDb.first();
    }

    OdString sPathDefault = getIApp()->toOdString(QDir(infoPrev.absolutePath())
      .absoluteFilePath(infoPrev.completeBaseName() + "." + qsExtension));
    OdString sPrompt = OdString().format(OD_T("Save drawing as <%ls>"), sPathDefault.c_str());
    try
    {
      sPath = pIO->getString(sPrompt,  
                             OdEd::kGstAllowSpaces | OdEd::kGstNoEmpty, // OdEd::kInpDefault | OdEd::kInpThrowEmpty, 
                             sPathDefault, // default
                             sPathDefault); // keywords
    }
    catch (const OdEdKeyword& kw)
    {
      sPath = kw.keyword();
    }

    QFileInfo info(getIApp()->toQString(sPath));
    if (sPath.isEmpty() || (info.exists() && !info.isWritable()))
    {
      OdString sMsg = getIApp()->toOdString(
        QObject::tr("File \"%1\" is not accessible to write").arg(getIApp()->toQString(sPath)));
      getIApp()->getBaseHostAppServices()->warning(sMsg);
      return;
    }
  }
  OdqConsoleTab& tab = *OdqConsole::getConsoleTab(pRxDatabase);

  if (sPath.isEmpty())
  {
    sPath = getIConsole()->getFilePathName(pRxDatabase); 

    // TODO possible 
    // uncomment the next to save as latest DWG :
    //typeSaveAs = OdDb::kDwg;
    //verSaveAs = OdDb::kDHL_CURRENT; //OdDb::vAC24;

    // fill filters

    OdString sFilters,
             sCurrFilter;
    OdString sExtension;
    if (pDwgDb.get())
    {
      //sCurrFilter = L"DWG files (*.dwg)|*.dwg|";
      //sFilters += sCurrFilter;
      //sFilters += L"DXF files (*.dxf)|*.dxf|";
      //sFilters += L"Binary DXF files (*.dxb)|*.dxb|";
      //
      sCurrFilter = getIApp()->toOdString(s_filtersDwg.getFilter(verSaveAs, typeSaveAs));
      sFilters = getIApp()->toOdString(s_filtersDwg.getFiltersAsString(""));
      sExtension = (typeSaveAs == OdDb::kDwg) ? OD_T("dwg") 
                                              : ((typeSaveAs == OdDb::kDxf) ? OD_T("dxf") : OD_T("dxb"));
      OdString sExt = getIApp()->toOdString(infoPrev.suffix().toLower()),
               sFlt;
      sFilters += sFlt = L"DWS Standards files (*.dws)|*.dws|";
      if (sExt == L"dws")
      {
        sCurrFilter = sFlt;
        sExtension = sExt;
      }
      sFilters += sFlt = L"DWT Drawing Template files (*.dwt)|*.dwt|";
      if (sExt == L"dwt")
      {
        sCurrFilter = sFlt;
        sExtension = sExt;
      }
      IqRevSystem* iRevSystem = getIRevSystem();
      if (iRevSystem && !iRevSystem->isUnder(pRxDatabase))
      {
        sFilters += sFlt = L"Drawings Stream Format files (*.dsf)|*.dsf|";
        sFilters += sFlt = L"ODA SDK Format files (*.tig)|*.tig|";
      }
      sFilters += L"|";
    }
    else
    {
      for (unsigned int idx = 0, sz = extensionExtDb.size(); idx < sz; idx++)
      {
        const OdString& sExt = extensionExtDb[idx];
        sFilters += pExtDb->getFilterByExtension(sExt);
        ODA_ASSERT_ONCE(pExtDb->getFilterByExtension(sExt, 1).isEmpty()); // TODO
        if (!idx)
        {
          sCurrFilter = sFilters;
          sExtension = sExt;
        }
      }

      if (!sFilters.isEmpty())
      {
        sFilters.replace(L';', L' '); // problem with QFileDialog
        sFilters += L"|";
      }
    }
    if (sFilters.isEmpty())
    {
      ODA_FAIL_ONCE();
      throw OdError(eNotApplicable); // There is no acceptable filter to select a file
    }

    qsExtension = getIApp()->toQString(sExtension);
    if (infoPrev.suffix().isEmpty() || infoPrev.suffix().toLower() != qsExtension)
      sPath = getIApp()->toOdString(QDir(infoPrev.absolutePath())
                                     .absoluteFilePath(infoPrev.completeBaseName() + "." + qsExtension));
    ODA_ASSERT_ONCE(!sPath.isEmpty())

    // TODO: add sCurrentFileter as in / out argument in OdEdUserIO::getFilePath
    // and OdDbBaseHostAppServices::fileDialog.
    // (Unfortunately - as usual - now is a bad time to change interfaces;
    //  way to use tracker for current filter transferring is bad too after 
    //  missing this argument in OdDbBaseHostAppServices::fileDialog declaration) 
    sExtension = sCurrFilter; // see special support for it in CommandContextImpl::getFilePath and OdqServices::fileDialog

    OdString sPrompt = OD_T("Save drawing as");
    sPath = pCmdCtx->userIO()->getFilePath(sPrompt, OdEd::kGfpForSave, 
                                           OD_T("Save Drawing As"), // dialog caption 
                                           sExtension, // default extension
                                           sPath, // file name
                                           sFilters);
    int pos = sPath.find(OD_T("|"));
    if (pos >= 0)
    {
      sCurrFilter = sPath.mid(pos + 1); 
      sPath = sPath.left(pos);
    }

    QFileInfo info(getIApp()->toQString(sPath));
    if (sPath.isEmpty() || (info.exists() && !info.isWritable()))
    {
      OdString sMsg = getIApp()->toOdString(
        QObject::tr("File \"%1\" is not accessible to write").arg(getIApp()->toQString(sPath)));
      getIApp()->getBaseHostAppServices()->warning(sMsg);
      return; // without message "Cancel by user" // throw OdEdCancel();
    }

    if (pDwgDb.get())
    {
      ODA_ASSERT_ONCE(!sCurrFilter.isEmpty());

      //if (sCurrFilter == OD_T("DWG files (*.dwg)|*.dwg|"))
      //  typeSaveAs = OdDb::kDwg;
      //else if (sCurrFilter == OD_T("DXF files (*.dxf)|*.dxf|"))
      //  typeSaveAs = OdDb::kDxf;
      //else if (sCurrFilter == OD_T("Binary DXF files (*.dxb)|*.dxb|"))
      //  typeSaveAs = OdDb::kDxb;
      OdDb::DwgVersion verPrev = verSaveAs;
      OdDb::SaveType typePrev = typeSaveAs;
      QString qsCurrFilter = getIApp()->toQString(sCurrFilter);
      if (sCurrFilter == L"DWS Standards files (*.dws)|*.dws|")
      {
        verSaveAs = OdDb::kDHL_CURRENT;
        typeSaveAs = OdDb::kDwg;
        qsExtension = "dws";
      }
      else if (sCurrFilter == L"DWT Drawing Template files (*.dwt)|*.dwt|")
      {
        verSaveAs = OdDb::kDHL_CURRENT;
        typeSaveAs = OdDb::kDwg;
        qsExtension = "dwt";
      }
      else if (sCurrFilter == L"ODA SDK Format files (*.tig)|*.tig|")
      {
        ODA_ASSERT_ONCE(getIRevSystem() && !getIRevSystem()->isUnder(pRxDatabase));

        OdString sCmd = L"saveastig \"" + sPath + L"\"";
        tab.executeCommand(sCmd);
        return;
      }
      else if (sCurrFilter == L"Drawings Stream Format files (*.dsf)|*.dsf|")
      {
        ODA_ASSERT_ONCE(getIRevSystem() && !getIRevSystem()->isUnder(pRxDatabase));

        OdString sCmd = L"saveasdsf \"" + sPath + L"\"";
        tab.executeCommand(sCmd);
        return;
      }
      else
      {
        verSaveAs = s_filtersDwg.getDwgVer(qsCurrFilter);
        typeSaveAs = s_filtersDwg.getDwgType(qsCurrFilter);
        qsExtension = (typeSaveAs == OdDb::kDwg) ? "dwg" : (typeSaveAs == OdDb::kDxf) ? "dxf" : "dxb";
      }

      // echo to console like with FILEDIA = 0

      OdString sMsg = getIApp()->toOdString(
                        QObject::tr("Current file format: %1 %2 Drawing")
                          .arg(s_filtersDwg.getKeyword(verPrev, typePrev))
                          .arg((typePrev == OdDb::kDwg) ? "DWG"
                                                        : ((typePrev == OdDb::kDxf) ? "ASCII DXF" 
                                                                                    : "Binary DXF")));
      pIO->putString(sMsg);

      QString qsPromptFormat,
              qsKeywords;
      int indexDefault = 0;
      if (typeSaveAs != typePrev)
      {
        qsPromptFormat = "Enter file format";
        s_filtersDwg.getKeywordsDataIO(verPrev, typePrev, qsPromptFormat, qsKeywords, indexDefault);
        tab.getTopCmdState().setPrompt(qsPromptFormat);
        tab.putMsgText(qsExtension.toUpper(),
                       TMsgType(enMsgTextPrompted | enMsgTextByInput));
      }
      qsPromptFormat = "Enter file format";
      s_filtersDwg.getKeywordsDataIO(verPrev, typeSaveAs, qsPromptFormat, qsKeywords, indexDefault);
      tab.getTopCmdState().setPrompt(qsPromptFormat);
      tab.putMsgText(s_filtersDwg.getKeyword(verSaveAs, typeSaveAs),
                     TMsgType(enMsgTextPrompted | enMsgTextByInput));
    }

    OdString sPathDefault = getIApp()->toOdString(QDir(infoPrev.absolutePath())
              .absoluteFilePath(infoPrev.completeBaseName() + "." + qsExtension));
    sPrompt += OdString().format(OD_T(" <%ls>"), sPathDefault.c_str());
    tab.getTopCmdState().setPrompt(sPrompt);
    tab.putMsgText(OdString().format(OD_T("\"%ls\""), sPath.c_str()),
      TMsgType(enMsgTextPrompted | enMsgTextByInput));
    tab.getTopCmdState().setPrompt(CommandState::getDefaultQPrompt());
  }

  QFileInfo info(getIApp()->toQString(sPath));
  getIApp()->toStatusBar(QObject::tr("Saving of file %1").arg(info.fileName()));

  try
  {
    if (!pExtDb)
    {
      pDwgDb->writeFile(sPath, typeSaveAs, verSaveAs, 
                        true); // saveThumbnailImage
      tab.setFilePathName(OdString::kEmpty); // clear titled
    }
    else
    {
      if (!pExtDb->writeFile(sPath, pCmdCtx))
        throw OdError(eFileWriteError);
    }

    OdqConsoleTab& tab = *OdqConsole::getConsoleTab(pRxDatabase);
    tab.updateQTitle();
    IqViewSystem* iVS = getIViewSystem();
    if (iVS)
      iVS->updateViewTitles(pRxDatabase);

    getIApp()->refreshQObjetcts("<event-refresh:drawing_history_changed>", 
                                getIApp()->toQString(sPath));
  }
  catch (const OdError& err)
  {
    ODA_FAIL_ONCE();
    QString qsMess = QObject::tr("Saving of file \"%1\" was failed (%2).")
                                  .arg(getIApp()->toQString(sPath))
                                  .arg(getIApp()->toQString(err.description()));
    throw OdError(getIApp()->toOdString(qsMess));
  }

  getIApp()->toStatusBar();
}

void OdqCmd_Save::execute(OdEdCommandContext* pCmdCtx)
{
  OdRxObject* pRxDatabase = pCmdCtx->baseDatabase();
  if (!pRxDatabase)
    throw OdError(eNoDatabase); // there is nothing to save

  OdString sFilePathName = getIConsole()->getFilePathName(pRxDatabase),
           // of file or template or empty(if created  from scratch)
           sFilePathName_ = OdDbBaseDatabasePE::cast(pRxDatabase)->getFilename(pRxDatabase);
  bool isTitled = (sFilePathName == sFilePathName_);

  if (sFilePathName.isEmpty() || !isTitled)
  {
    OdqConsoleModule* pModule = static_cast<OdqConsoleModule*>(
                                  // TODO const_cast<OdRxModule*>(module()));
                                  ::odrxDynamicLinker()->loadModule(OD_T("OdaQtConsole.dll")).get());
    ODA_ASSERT_ONCE(pModule);
    pModule->m_cmd_SaveAs.execute(pCmdCtx);
    return;
  }

  QString qsCmd = QString("saveAs %2\"%1\"").arg(getIApp()->toQString(sFilePathName))
                                               .arg(OdDbDatabase::cast(pCmdCtx->baseDatabase()).isNull() 
                                                        ? "" : "\"\" ");
  getIConsole()->executeCommand(qsCmd, false, pRxDatabase);
}

void OdqCmd_Quit::execute(OdEdCommandContext* pCmdCtx)
{
  //getIConsole()->executeCommand("closeall", false);
  OdqConsoleModule* pModule = static_cast<OdqConsoleModule*>(
                                 // TODO const_cast<OdRxModule*>(module()));
                                 ::odrxDynamicLinker()->loadModule(OD_T("OdaQtConsole.dll")).get());
  ODA_ASSERT_ONCE(pModule);
  
  bool hasViews = false;
  IqViewSystem* iVS = getIViewSystem();
  if (iVS)
    hasViews = iVS->markToCloseAppByLastView();

  pModule->m_cmd_CloseAll.execute(pCmdCtx);

  if (!iVS)
  {
    QTimer::singleShot(1, getIApp()->getMainWindow(), SLOT(close())); //quit if ViewSystem was not loaded
    return;
  }

#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
  hasViews = iVS->markToCloseAppByLastView();
#endif
  if (!hasViews)
    iVS->checkMarkToCloseApp();
}

void OdqCmd_CloseAll::execute(OdEdCommandContext*) // pCmdCtx
{
  QList<OdRxObject*> lst = getIConsole()->getOpenDatabases();
  IqViewSystem* iVS = getIViewSystem();
  if (iVS)
  {
    QList<OdRxObject*> lstExceptDatabases;
    foreach (OdRxObject* pRxDatabase, lst)
    {
      CommandContextImpl* pCtx = static_cast<CommandContextImpl*>(getIConsole()->getCommandContext(pRxDatabase));
      ODA_ASSERT_ONCE(pCtx);
      if (!pCtx || !pCtx->isMarkedToCloseWithDeferredCommand()) 
        continue;
      
      lstExceptDatabases.push_back(pRxDatabase);
    }

    if (!iVS->closeAllViews(lstExceptDatabases.size() ? &lstExceptDatabases : NULL))
      throw OdEdCancel();
    return;
  }

  // way without view system
  foreach (OdRxObject* pRxDatabase, lst)
  {
    ODA_ASSERT_ONCE(pRxDatabase);
    if (!getIConsole()->closeDatabase(pRxDatabase))
      throw OdEdCancel();
  }
}

void OdqCmd_Close::execute(OdEdCommandContext* pCmdCtx)
{
  OdRxObject* pRxDatabase = pCmdCtx->baseDatabase();
  OdString sInput;

  IqViewSystem* iVS = getIViewSystem();
  if (iVS)
  {
    if (!pRxDatabase && !iVS->getView(pRxDatabase))
      return; // there is nothing to close
    sInput = takeInputString(pRxDatabase);
    if (!iVS->closeViews(pRxDatabase))
      throw OdEdCancel();
    //return;
  }
  else
  {
    if (!pRxDatabase)
      throw OdError(eNoDatabase); // there is nothing to close
    sInput = takeInputString(pRxDatabase);
    if (!getIConsole()->closeDatabase(pRxDatabase))
      throw OdEdCancel();
  }

  if (!sInput.isEmpty())
    moveInputString(sInput, getIConsole()->getCurrentDatabase());
}

void OdqCmd_Script::execute(OdEdCommandContext* pCmdCtx)
{
  OdString sPath;

  OdDbHostAppServices* pServices = OdDbHostAppServices::cast(getIApp()->getBaseHostAppServices());
  ODA_ASSERT_ONCE(pServices);
  bool bFileDialog = pServices->getFILEDIA();

  if (!bFileDialog || getIConsole()->isInputStringReady(pCmdCtx->baseDatabase()))
  {
    sPath = pCmdCtx->userIO()->getString(OD_T("Enter name of script to execute"));
    if (!sPath.isEmpty())
    {
      QString qsPath = getIApp()->toQString(sPath);
      QFileInfo info(qsPath);
      if (!info.exists())
      {
        if (info.suffix().isEmpty())
        {
          qsPath = QDir(info.filePath()).absoluteFilePath(info.completeBaseName() + ".scr");
          if (info.exists())
            sPath = getIApp()->toOdString(qsPath);
          else if (pServices->findFile(sPath = getIApp()->toOdString(qsPath = info.completeBaseName() + ".scr"),
                                       pCmdCtx->baseDatabase()).isEmpty())
          {
            OdString sMsg = getIApp()->toOdString(
              QObject::tr("Script file \"%1\" was not found").arg(getIApp()->toQString(sPath)));
            getIApp()->getBaseHostAppServices()->warning(sMsg);
            return;
          }
        }
      }
    }
  }

  if (sPath.isEmpty())
    sPath = pCmdCtx->userIO()->getFilePath(OD_T("Enter name of script to execute"), 
                                           OdEd::kGfpForOpen, 
                                           OD_T("Select Script File"), // dialog caption 
                                           OD_T("scr"), // default extension
                                           OdString::kEmpty, // file name
                                           OD_T("Script (*.scr)|*.scr||")); // filters

  if (sPath.isEmpty())
    return; // without message "Cancel by user" // throw OdEdCancel();

  bool bWasFound = false;
  OdVariant vValue = getIConsole()->executeCommandScript(getIApp()->toQString(sPath),
                                                         pCmdCtx->baseDatabase(),
                                                         &bWasFound);
  ODA_ASSERT_ONCE(bWasFound);
  OdqConsole::getConsoleTab(pCmdCtx->baseDatabase())->getTopCmdState().setValue(vValue);
}

// change visibility of console palette

void OdqCmd_UiConsoleHide::execute(OdEdCommandContext*) // pCmdCtx
{
  OdqConsole::getDockPalette()->setVisible(false);
}

void OdqCmd_UiConsoleShow::execute(OdEdCommandContext*) // pCmdCtx
{
  OdqConsole::getDockPalette()->setVisible(true);
}

void OdqCmd_UiConsoleTab::execute(OdEdCommandContext* pCmdCtx)
{
  // change visibility and placement of tabbar
  QList<QTabBar*> widgets = OdqConsole::getDockPalette()->widget()->findChildren<QTabBar*>();
  if (widgets.count() != 1)
    throw OdError(eNotApplicable);

  QTabWidget* pTabWidget = qobject_cast<QTabWidget*>(OdqConsole::getDockPalette()->widget());
  ODA_ASSERT_ONCE(pTabWidget);

  OdString sPrompt,
           sKeywords;
  int indexDefault;

  bool bVisible = widgets.last()->isVisible();
  if (!bVisible)
  {
    sPrompt = OD_T("Enter console tab state [North/South/On/Off]<Off>");
    sKeywords = OD_T("North South On Off");
    indexDefault = 3;
  }
  else  
  {
    sKeywords += OD_T("North South Off");

    QTabWidget::TabPosition pos = pTabWidget->tabPosition();
    switch (pos)
    {
    case QTabWidget::North:
      sPrompt += OD_T("Enter console tab state [North/South/Off]<North>");
      indexDefault = 0;
      break;
    default:
      ODA_FAIL_ONCE();
    case QTabWidget::South:
      sPrompt += OD_T("Enter console tab state [North/South/Off]<South>");
      indexDefault = 1;
      break;
    }
  }

  int indexKeyword = pCmdCtx->userIO()
                      ->getKeyword(sPrompt, sKeywords, indexDefault, OdEd::kGstNoEmpty);
                                   // OdEd::kInpDefault | OdEd::kGstNoEmpty);
  switch (indexKeyword)
  {
  case 0: // North
    pTabWidget->setTabPosition(QTabWidget::North);
    widgets.last()->setVisible(true);
    return;
  case 1: // South
    pTabWidget->setTabPosition(QTabWidget::South);
    widgets.last()->setVisible(true);
    return;
  case 3:
    if (!bVisible)
      return; // current
    break;
  }

  widgets.last()->setVisible(!bVisible);
}

// Nearest ACAD analog via AutoLISP:
//  http://www.crlf.de/Dokumente/Diesel/Diesel.html
//
//  (defun c:DIESEL (/ s)
//    (while (/= "" (setq s (getstring "\nDIESEL: " T)))
//      (princ (menucmd (strcat "m=" s)))
//    )
//    (princ)
//  )

void OdqCmd_Diesel::execute(OdEdCommandContext* pCmdCtx)
{
  //static OdString s_sLast = OD_T("m=$(if,$(and,$(>,$(getvar,clistate),0)),commandlinehide,commandline)");
  //static OdString s_sLast = OD_T("$(if,0,bad,ok)");
  //static OdString s_sLast = OD_T("$(if,1,ok,bad)");
  static OdString s_sLast = OD_T("$(strlen,\"Hello world!\")");
  
  OdEdUserIO* pIO = pCmdCtx->userIO();
  while (true)
  {
    OdString sPrompt;
    sPrompt.format(OD_T("Enter Expression to evaluate<%ls>"), s_sLast.c_str());
    try
    {
      s_sLast = pIO->getString(sPrompt, OdEd::kGstAllowSpaces | OdEd::kInpThrowEmpty, s_sLast);
    }
    catch (const OdEdEmptyInput&)
    {
    }
    s_sLast.trimRight(OD_T("\n"));
    if (s_sLast.isEmpty())
      return;
    OdString sResult = getIConsole()->evaluateDieselExpression(s_sLast, pCmdCtx->baseDatabase());
    pIO->putString(sResult);
  }
}

void OdqCmd_PSelect::execute(OdEdCommandContext* pCmdCtx)
{
  ODA_ASSERT_ONCE(module());  // or via ::odrxDynamicLinker()->loadModule(OD_T("OdaQtConsole.dll")).get()
  static_cast<OdqConsoleModule*>(const_cast<OdRxModule*>(module()))->m_cmd_Select.execute(pCmdCtx);
}

void OdqCmd_Select::execute(OdEdCommandContext* pCmdCtx)
{
  OdRxObject* pRxDatabase = pCmdCtx->baseDatabase();
  IqView* iView = NULL;
  IqViewSystem* iVS = getIViewSystem();
  if (iVS && pRxDatabase)
    iView = iVS->getView(pRxDatabase);
  if (!iView)
    throw OdEdCancel();

  bool bClearSSet = !getIConsole()->isInputStringReady(pRxDatabase);
  if (!bClearSSet)
  {
    bool bPickadd = true;
    OdSysVarPEPtr pSysVarPE = OdSysVarPE::cast(pCmdCtx);
    if (!pSysVarPE.isNull())
    {
      OdString sPickadd = pSysVarPE->valueToString(pCmdCtx, "PICKADD");
      bPickadd = !(sPickadd.makeLower() == OD_T("false") || sPickadd == OD_T("0"));
    }
    if (!bPickadd)
      bClearSSet = true;
  }

  if (bClearSSet)
  {
    //getIConsole()->executeCommand("ai_deselect", false);
    ODA_ASSERT_ONCE(module());  // or via ::odrxDynamicLinker()->loadModule(OD_T("OdaQtConsole.dll")).get()
    static_cast<OdqConsoleModule*>(const_cast<OdRxModule*>(module()))->m_cmd_DeselectAll.execute(pCmdCtx);
  }

  OdqConsoleTab& tab = *OdqConsole::getConsoleTab(pRxDatabase, true);
  OdSelectionSetPtr pSSet = tab.workingSelectionSet();
  ODA_ASSERT_VAR(OdExtDbModule* pExtDb = getIApp()->getExtDbModule(pRxDatabase);)
  ODA_ASSERT_ONCE(   pSSet.get() 
                  || (pExtDb && pExtDb->baseModuleName(true) == L"bim")
                  || (pExtDb && pExtDb->baseModuleName(true) == L"prc")
               //PROD-139/PROD-114 //  || (pExtDb && pExtDb->baseModuleName(true) == L"ifc") 
                  || (pExtDb && pExtDb->baseModuleName(true) == L"dwf"));
  if (pSSet.isNull())
    throw OdEdCancel();

  //OdEdUserIO* pEdIO = pCmdCtx->userIO();
  OdDbUserIO* pIO = OdDbUserIO::cast(pCmdCtx->userIO()).get();
  ODA_ASSERT_ONCE(pIO);
  pIO->setPickfirst(NULL);

  int iOpt = OdEd::kSelLeaveHighlighted | OdEd::kSelAllowEmpty | OdEd::kSelAllowInactSpaces;
  //
  //iOpt |= OdEd::kSelPickLastPoint;
  //pIO->setLASTPOINT(pt);
  //
  //iOpt |= OdEd::kSelSinglePass; // now via "select single ..."
  //iOpt |= OdEd::kSelLeaveHighlighted; // TODO via !bEchoOn

  if (tab.getKeyState() & OdEdBaseIO::kShiftIsDown)
    iOpt |= OdEd::kSelRemove;
  if (tab.getKeyState() & OdEdBaseIO::kControlIsDown)
    iOpt |= OdEd::kSelAllowSubents;

  try
  {
    pSSet = pIO->select(OD_T("Select objects"), // OdString::kEmpty, 
                        iOpt, 
                        pSSet, // we use current selection set independently of PICKFIRST value
                        OdString::kEmpty); // with internal integrated keywords (see CommandContextImpl::select)
    bool bSubobjectsHint = false;
    if (   !bClearSSet 
        && OdDbDatabase::cast(pRxDatabase).get()
        && pSSet->numEntities() == 1
        && !pSSet->newIterator()->subentCount())
    {
      OdDbObjectPtr pObject = OdDbObjectId(pSSet->newIterator()->id()).safeOpenObject();
      OdDbEntityPtr pEntity = OdDbEntity::cast(pObject);
      OdDbFullSubentPath subPath;
      OdGsMarkerArray gsMarkers;
      if (!pEntity.isNull())
      {
        OdResult res = pEntity->getGsMarkersAtSubentPath(subPath, gsMarkers);
        if (res == eOk || res == eWrongSubentityType)
          bSubobjectsHint = true;
      }
    }
    if (bSubobjectsHint)
      getIApp()->toStatusBar(QObject::tr("Press and hold the CTRL key"
                                       //" and use the cursor"
                                         " to select a set of subobjects to manipulate."),
                                         2000);
    tab.setWorkingSelectionSet(pSSet);
  }
  catch (const OdError&)
  {
    throw OdEdCancel();
  }

  tab.workingSelectionSetChanged();
}

void OdqCmd_SelectAll::execute(OdEdCommandContext* pCmdCtx)
{
  //void OdqCmd_DeselectAll::execute(OdEdCommandContext* pCmdCtx)
  QString qsCmd = "select single all";
  getIConsole()->postponeCommand(qsCmd, false, pCmdCtx->baseDatabase());
}

void OdqCmd_DeselectAll::execute(OdEdCommandContext* pCmdCtx)
{
  OdRxObject* pRxDatabase = pCmdCtx->baseDatabase();
  if (!pRxDatabase)
    return;

  OdqConsoleTab& tab = *OdqConsole::getConsoleTab(pRxDatabase, true);
  OdSelectionSetPtr pSSet = tab.workingSelectionSet();
  ODA_ASSERT_ONCE(!pSSet.isNull());
  if (pSSet.isNull() || !pSSet->numEntities())
    return;

  IqViewSystem* iVS = getIViewSystem();
  if (iVS)
  {
    for (OdSelectionSetIteratorPtr pItr = pSSet->newIterator();
         !pItr->done(); pItr->next())
      iVS->highlightObject(false, pItr->id());
  }
  // Don't clear working selection set to prevent previous selection modification // pSSet->clear();
  pSSet = tab.getCmdContext().createSelectionSet();
  tab.setWorkingSelectionSet(pSSet);
  tab.workingSelectionSetChanged();
}

void OdqCmd_Stretch::execute(OdEdCommandContext* pCmdCtx)
{
  OdRxObject* pRxDatabase = pCmdCtx->baseDatabase();
  IqView* iView = NULL;
  IqViewSystem* iVS = getIViewSystem();
  if (iVS && pRxDatabase)
    iView = iVS->getView(pRxDatabase);
  if (!iView)
    throw OdEdCancel();

  //bool bClearSSet = !getIConsole()->isInputStringReady(pRxDatabase);
  //if (bClearSSet)
  //{
  //  //getIConsole()->executeCommand("ai_deselect", false);
  //  ODA_ASSERT_ONCE(module());  // or via ::odrxDynamicLinker()->loadModule(OD_T("OdaQtConsole.dll")).get()
  //  static_cast<OdqConsoleModule*>(const_cast<OdRxModule*>(module()))->m_cmd_DeselectAll.execute(pCmdCtx);
  //}

  OdqConsoleTab& tab = *OdqConsole::getConsoleTab(pRxDatabase, true);
  OdSelectionSetPtr pSSet = tab.workingSelectionSet();
  if (pSSet.isNull() || !pSSet->numEntities())
  {
    ODA_ASSERT_ONCE(OdPropServices::cast(pCmdCtx).get());

    pCmdCtx->userIO()->putString(OdPropServices::cast(pCmdCtx)->tr("Select objects to stretch by crossing-window or crossing-polygon..."));
    ODA_ASSERT_ONCE(module());
    static_cast<OdqConsoleModule*>(const_cast<OdRxModule*>(module()))->m_cmd_Select.execute(pCmdCtx);
    pSSet = tab.workingSelectionSet();
    if (pSSet.isNull() || !pSSet->numEntities())
      return; // throw OdEdCancel();
  }

  //Specify base point or [Displacement] <Displacement>:
  //Specify second point or <use first point as displacement>:

  OdDbUserIO* pIO = OdDbUserIO::cast(pCmdCtx->userIO()).get();
  ODA_ASSERT_ONCE(pIO);

  // TODO: Notify active grips.
  // AEC grips use CTRL key to change mode,
  // but how to pass it throw standard interface is currently unknown.

  try
  {
    OdqSnapManagerPtr pSnapManager;
    if (getIAppProps()->toBool(L"Vars/snap", true) && OdqSnapManager::isSupported(pRxDatabase))
    {
      CommandState& stateCmd = tab.getTopCmdState();
      if (   !stateCmd.m_pStringIO->isEof() // input string is ready
          && stateCmd.m_pStringIO->getInitString().getAt(0) != L'\\') // and is not marked to skip
      {
        if (   (pSnapManager = stateCmd.snapManager()).isNull()
            && (pSnapManager = OdqSnapManager::createObject(pRxDatabase)).get())
          stateCmd.setSnapManager(pSnapManager);
      }
    }

    OdGePoint3d ptBase = pIO->getPoint(OD_T("Specify base point"), 
                                       OdEd::kGptNoLimCheck); // | OdEd::kGptNoUCS);
    if (pSnapManager.get())
    {
      // Base point came into command via text coordinates.
      // Therefore getPoint had skipped snapping for it
      OdGsView* pGsView = iView->getActiveTopGsView();
      if (pGsView)
      {
        //ODA_ASSERT_VAR(bool isBaseSnapped =)
        pSnapManager->snap(pGsView, ptBase, NULL);
        //ODA_ASSERT_ONCE(isBaseSnapped);
      }
    }

    bool bUseClones = true;
    if (!OdDbDatabase::cast(pRxDatabase).get())
      bUseClones = false;
    OdqDragTrackerPtr pDragTracker = OdqDragTracker::createObject(tab.getCmdContext().gripManager(),
                                                                  ptBase, bUseClones);
    OdString sKeywords;
    OdGePoint3d ptSecond = pIO->getPoint(OD_T("Specify second point"), 
                                         OdEd::kGptNoLimCheck, // | OdEd::kGptNoUCS,
                                         &ptBase,
                                         sKeywords,
                                         pDragTracker);
    if (ptSecond != ptBase)
    {
      pDragTracker->finishAt(ptSecond);
      if ((pSnapManager = tab.getTopCmdState().snapManager()).get())
        pSnapManager->Set_Entity_centers(pRxDatabase);
    }
  }
  catch (const OdError&)
  {
    throw OdEdCancel();
  }
}

void OdqCmd_Handle::execute(OdEdCommandContext* pCmdCtx)
{
  OdDbBaseDatabasePEPtr pDbPE = OdDbBaseDatabasePE::cast(pCmdCtx->baseDatabase());
  if (pDbPE.isNull())
    return;
  OdDbUserIOPtr pIO = pCmdCtx->userIO();
  OdSelectionSetPtr pSSet = pIO->select();
  OdUInt32 num = pSSet->numEntities();
  if (!num)
    return;

  OdString sRes;
  for (OdSelectionSetIteratorPtr pItr = pSSet->newIterator(); !pItr->done(); pItr->next())
  {
    OdDbHandle handle = pDbPE->getHandle(pItr->id());
    ODA_ASSERT_ONCE(!handle.isNull());
    if (handle.isNull())
      continue;
    if (sRes.isEmpty())
      sRes = L"\nUse to select via lisp:\n  select (handent \""; // select (handent "b0,b1,f0") ^J
    else
      sRes += L',';

    sRes += handle.ascii();
  }
  if (sRes.isEmpty())
    return;

  sRes += L"\") ^J\n";
  pIO->putString(sRes);
}

// extract to separate files
void OdqCmd_Extract::execute(OdEdCommandContext* pCmdCtx)
{
  OdRxObject* pRxDb = pCmdCtx->baseDatabase();
  OdDbDatabasePtr pDb = pRxDb;
  OdDbUserIOPtr pIO = pCmdCtx->userIO();
  OdArray<OdDbHandle> handles;
  OdDbBlockTableRecordPtr pBlk = pDb->getActiveLayoutBTRId().safeOpenObject();
  for (OdDbObjectIteratorPtr pItr = pBlk->newIterator(); !pItr->done(); pItr->step())
    handles.push_back(pItr->objectId().getHandle().ascii());
  if (!handles.size())
    return;
  OdArray<OdDbHandle> handlesToExtract;
  OdqConsoleTab& tab = *OdqConsole::getConsoleTab(pRxDb);
  OdDbSelectionSet* pSSet = OdDbSelectionSet::cast(tab.workingSelectionSet()).get();
  if (pSSet->numEntities())
  {
    for (OdDbSelectionSetIteratorPtr pItr = pSSet->newIterator(); !pItr->done(); pItr->next())
      handlesToExtract.push_back(pItr->objectId().getHandle().ascii());
  }
  else
    handlesToExtract = handles;

  for (int index = 0, sz = (int)handlesToExtract.size(); index < sz; index++)
  {
    OdString sFileName = pDb->getFilename(), sFileNameRes = sFileName;
    int pos = sFileName.reverseFind(L'.');
    if (pos > 0)
      sFileNameRes = sFileName.left(pos);
    sFileNameRes += OdString().format(L"_(%02d)_%ls.dwg", index, handlesToExtract[index].ascii().c_str());
    
    OdDbDatabasePtr pTmpDb = pDb->appServices()->readFile(sFileName, true, false, Oda::kShareDenyNo);
    OdDbObjectId id = pTmpDb->getOdDbObjectId(handlesToExtract[index], false);
    for (int idx = 0, szAll = (int) handles.size(); idx < szAll; idx++)
    {
      if (id.getHandle() == handles[idx])
        continue;
      OdDbObjectId idErase = pTmpDb->getOdDbObjectId(handlesToExtract[idx], false);

      OdDbEntityPtr pEnt = OdDbEntity::cast(idErase.openObject(OdDb::kForWrite));
      if (pEnt.get())
        pEnt->erase();
    }
    OdDbObjectPtr pVpObj = pTmpDb->activeViewportId().safeOpenObject(OdDb::kForWrite);
    OdAbstractViewPEPtr pVpPE(pVpObj);
    OdGeBoundBlock3d fullExtents;
    if (pVpPE->viewExtents(pVpObj, fullExtents))
      pVpPE->zoomExtents(pVpObj, &fullExtents);

    pTmpDb->writeFile(sFileNameRes, OdDb::kDwg, OdDb::vAC27, true);
  }
}

struct ReactorSort
{
  bool operator()(OdDbObjectId id1, OdDbObjectId id2)
  {
    OdDbObjectPtr o2 = id2.openObject();
    if (o2.isNull())
      return false;
    OdDbObjectIdArray r2 = o2->getPersistentReactors();
    if (r2.contains(id1))
      return true;
    return false;
  }
};

void OdqCmd_DropGeom::execute(OdEdCommandContext* pCmdCtx)
{
  OdDbDatabase* pDwgDb = OdDbDatabase::cast(pCmdCtx->baseDatabase()).get();
  if (!pDwgDb)
    throw OdError(eNoDatabase);

  QClipboard* pClipboard = QApplication::clipboard();
  ODA_ASSERT_ONCE(pClipboard);
  if (!pClipboard)
    return;
  const QMimeData* pMimeData = pClipboard->mimeData(OdqClipboardData::mimeDataMode());

  OdDbUserIOPtr pIO = pCmdCtx->userIO();
 #if 0 // test 
  if (!pMimeData)
    return;
  //application/x-qt-windows-mime;value="AutoCAD.r17"
  foreach (QString qsFmt, pMimeData->formats())
  {
    pIO->putString(getIApp()->toOdString(qsFmt));
  }
 #endif

  OdSharedPtr<OdqClipboardData> pData = OdqClipboardData::get(pMimeData, pCmdCtx->baseDatabase());
  if (pData.isNull())
  {
    pCmdCtx->userIO()->putString(OdPropServices::cast(pCmdCtx)->tr("Clipboard is Empty."));
    return;
  }

  IqViewSystem* iVS = getIViewSystem();
  if (!iVS)
    return;
  IqView* iView = iVS->getView(pDwgDb);
  if (!iView)
    return;
  OdRxObject* pRxDbDragSource = CommandContextImpl::baseDatabaseDragSource();

  try
  {
    OdDbDatabasePtr pTmpDb = pDwgDb->appServices()->readFile(pData->tempFileName(), true, false, Oda::kShareDenyNo);
    OdGeMatrix3d xform = OdGeMatrix3d::kIdentity;

    //Specify insertion point:
    OdGePoint3d point = pIO->getPoint(OD_T("Specify point"), 
                                      OdEd::kGptNoLimCheck); // | OdEd::kGptNoUCS | OdEd::kGptEndDrag | OdEd::kGptNoOSnap,
    ODA_ASSERT_ONCE(OdZero(point.z));
    xform = OdGeMatrix3d::translation(point - pData->pickPoint());

    bool bQuiescent = (pRxDbDragSource != pDwgDb);
    if (!bQuiescent)
    {
      OdDbUserIOPtr pIO = pCmdCtx->userIO();
      OdDbSelectionSetPtr pSSet = pIO->select(); // should be already selected
      OdDbObjectIdArray ids = pSSet->objectIdArray();
      unsigned index;

      OdqConsoleTab& tab = *OdqConsole::getConsoleTab(pDwgDb);
      //OdEdBaseIOPtr pBaseIO = tab.getCmdIO(); // pBaseIO->getKeyState()
      if (tab.getKeyState() & OdEdBaseIO::kControlIsDown)
      {
        OdDbIdMappingPtr pIdMapping = OdDbIdMapping::createObject();
        pDwgDb->deepCloneObjects(ids, pDwgDb->getActiveLayoutBTRId(), *pIdMapping);
        for (index = 0; index < ids.size(); index++)
        {
          OdDbIdPair idPair(ids[index]);
          pIdMapping->compute(idPair);
          ids[index] = idPair.value();
        }
      }
      
      // transform_object_set
      std::sort(ids.begin(), ids.end(), ReactorSort());
      for (index = 0; index < ids.size(); index++)
      {
        OdDbObjectId id = ids[index];
        OdDbEntityPtr pEnt = id.safeOpenObject(OdDb::kForWrite);
        pEnt->transformBy(xform);
      }
    }
    else
      pDwgDb->insert(xform, pTmpDb);

    iView->updateGL(); // repaint
  }
  catch (const OdError& e)
  {
    //ODA_FAIL_ONCE(); // test
    //UpdateAllViews(0);
    throw e;
  }
  
  //{
  //  OdGsModel* pGsModel = iView->getGsModel();
  //  if (pGsModel)
  //    pGsModel->invalidate(OdGsModel::kInvalidateAll);
  //}
  iView->updateGL(); // repaint
}

//void OdqCmd_Copyclip::execute(OdEdCommandContext*) // pCmdCtx
//{
//  // see COdaMfcAppDoc::DataSource::Create
//  throw OdError(eNotImplemented);
//}
//
//void OdqCmd_Pasteclip::execute(OdEdCommandContext*) // pCmdCtx
//{
//  // see COdaMfcAppDoc::OnEditPaste()
//  throw OdError(eNotImplemented);
//}

#ifndef SHRT_MAX
  #define SHRT_MAX 32767
#endif

void OdqCmd_Undo::execute(OdEdCommandContext* pCmdCtx)
{
  OdDbBaseDatabase* pRxDatabase = pCmdCtx->baseDatabase();
  OdqConsoleTab& tab = *OdqConsole::getConsoleTab(pRxDatabase);
  if (tab.m_levelUndoBlock)
  {
    ODA_FAIL_ONCE(); // test
    return;
  }

  OdDbDatabasePtr pDatabase = OdDbDatabase::cast(pRxDatabase);
  OdExtDbModule* pExtDb = NULL;
  if (pDatabase.isNull())
    pExtDb = getIApp()->getExtDbModule(pRxDatabase);

  if (   (pDatabase.isNull() && !pExtDb)
      || (pDatabase.get() && !pDatabase->hasUndo())
      || (pExtDb && !pExtDb->hasUndo(pRxDatabase)))
  {
    ODA_ASSERT_ONCE(OdPropServices::cast(pCmdCtx).get());

    pCmdCtx->userIO()->putString(OdPropServices::cast(pCmdCtx)->tr("Nothing to undo"));
    return;
  }
  ODA_ASSERT_ONCE(OdPropServices::cast(pCmdCtx).get());

  // clear selection
  //
  ////getIConsole()->executeCommand("ai_deselect", false);
  //ODA_ASSERT_ONCE(module());  // or via ::odrxDynamicLinker()->loadModule(OD_T("OdaQtConsole.dll")).get()
  //static_cast<OdqConsoleModule*>(const_cast<OdRxModule*>(module()))->m_cmd_DeselectAll.execute(pCmdCtx);

  OdEdUserIO* pIO = pCmdCtx->userIO();
  try
  {
    int nSteps = 1;
    // nSteps always 1 for _U command
    //while (true)
    //{
    //  nSteps = pIO->getInt(OD_T("Enter the number of operations to undo <1>"), OdEd::kInpDefault, 1);
    //  if (nSteps > 0 && nSteps <= SHRT_MAX)
    //    break;
    //  pIO->putString(OD_T("Requires an integer between 1 and 32767."));
    //}
    
    if (pDatabase.get() && pDatabase->hasUndo())
    {
      while (nSteps-- && pDatabase->hasUndo())
        pDatabase->undo();
      if (!pDatabase->hasUndo())
        pIO->putString(OdPropServices::cast(pCmdCtx)->tr("Everything has been undone"));

      OdqGripManagerPtr pGripManager = tab.getCmdContext().gripManager();
      if (!pGripManager.isNull())
        pGripManager->updateAllGrips();
      if (!pDatabase->hasUndo())
        getIApp()->refreshQObjetcts("<event-refresh:undo_state_changed>");
      if (pDatabase->hasRedo())
        getIApp()->refreshQObjetcts("<event-refresh:redo_state_changed>");

      OdSelectionSetPtr pSSet = tab.workingSelectionSet();
      if (pSSet.get() && pSSet->numEntities())
        getIApp()->refreshQObjetcts("<event-refresh:selection_changed>", "force"); // renew property palette
    }
    else if (pExtDb && pExtDb->hasUndo(pRxDatabase))
    {
      while (nSteps-- && pExtDb->hasUndo(pRxDatabase))
        pExtDb->undo(pRxDatabase);
      if (!pExtDb->hasUndo(pRxDatabase))
        pIO->putString(OdPropServices::cast(pCmdCtx)->tr("Everything has been undone"));

      OdqGripManagerPtr pGripManager = tab.getCmdContext().gripManager();
      if (!pGripManager.isNull())
        pGripManager->updateAllGrips();
      if (!pExtDb->hasUndo(pRxDatabase))
        getIApp()->refreshQObjetcts("<event-refresh:undo_state_changed>");
      if (pExtDb->hasRedo(pRxDatabase))
        getIApp()->refreshQObjetcts("<event-refresh:redo_state_changed>");

      OdSelectionSetPtr pSSet = tab.workingSelectionSet();
      if (pSSet.get() && pSSet->numEntities())
        getIApp()->refreshQObjetcts("<event-refresh:selection_changed>", "force"); // renew property palette
    }
    else
      pIO->putString(OdPropServices::cast(pCmdCtx)->tr("Nothing to undo"));
  }
  catch (const OdError& err)
  {
    ODA_ASSERT_ONCE(pExtDb && pExtDb->baseModuleName(true) == L"dgn"); // undo/redo is broken for ExDgnCreate.dgn 
    pIO->putError(err.description());
  }
}

void OdqCmd_Redo::execute(OdEdCommandContext* pCmdCtx)
{
  OdDbBaseDatabase* pRxDatabase = pCmdCtx->baseDatabase();
  OdqConsoleTab& tab = *OdqConsole::getConsoleTab(pRxDatabase);
  if (tab.m_levelUndoBlock)
  {
    ODA_FAIL_ONCE(); // test
    return;
  }
  ODA_ASSERT_ONCE(OdPropServices::cast(pCmdCtx).get());
  OdEdUserIO* pIO = pCmdCtx->userIO();

  OdDbDatabasePtr pDatabase = OdDbDatabase::cast(pRxDatabase);
  OdExtDbModule* pExtDb = NULL;
  if (pDatabase.isNull())
    pExtDb = getIApp()->getExtDbModule(pRxDatabase);

  if (   (pDatabase.isNull() && !pExtDb)
      || (pDatabase.get() && !pDatabase->hasRedo())
      || (pExtDb && !pExtDb->hasRedo(pRxDatabase)))
  {
    ODA_ASSERT_ONCE(OdPropServices::cast(pCmdCtx).get());

    pIO->putString(OdPropServices::cast(pCmdCtx)->tr("Nothing to redo"));
    return;
  }

  // clear selection
  //
  ////getIConsole()->executeCommand("ai_deselect", false);
  //ODA_ASSERT_ONCE(module());  // or via ::odrxDynamicLinker()->loadModule(OD_T("OdaQtConsole.dll")).get()
  //static_cast<OdqConsoleModule*>(const_cast<OdRxModule*>(module()))->m_cmd_DeselectAll.execute(pCmdCtx);

  try
  {
    int nSteps = 1;
    while (true)
    {
      try
      {
        nSteps = pIO->getInt(OD_T("Enter number of actions or [All/Last] <1>"), OdEd::kInpDefault, 1,
                             OD_T("All Last"));
      }
      catch (const OdEdKeyword& kw)
      {
        switch (kw.keywordIndex())
        {
        case 0:
          nSteps = SHRT_MAX; break;
        default:
          ODA_FAIL_ONCE();
        case 1:
          nSteps = 1; break;
        }
      }
      if (nSteps > 0 && nSteps <= SHRT_MAX)
        break;
      pIO->putString(OdPropServices::cast(pCmdCtx)->tr("Requires an integer between 1 and 32767."));
    }

    if (pDatabase.get() && pDatabase->hasRedo())
    {
      while (nSteps-- && pDatabase->hasRedo())
        pDatabase->redo();
      if (!pDatabase->hasRedo())
        pIO->putString(OdPropServices::cast(pCmdCtx)->tr("Everything has been redone"));

      OdqGripManagerPtr pGripManager = tab.getCmdContext().gripManager();
      if (!pGripManager.isNull())
        pGripManager->updateAllGrips();
      if (!pDatabase->hasRedo())
        getIApp()->refreshQObjetcts("<event-refresh:redo_state_changed>");
      if (pDatabase->hasUndo())
        getIApp()->refreshQObjetcts("<event-refresh:undo_state_changed>");

      OdSelectionSetPtr pSSet = tab.workingSelectionSet();
      if (pSSet.get() && pSSet->numEntities())
        getIApp()->refreshQObjetcts("<event-refresh:selection_changed>", "force"); // renew property palette
    }
    else if (pExtDb && pExtDb->hasRedo(pRxDatabase))
    {
      while (nSteps-- && pExtDb->hasRedo(pRxDatabase))
        pExtDb->redo(pRxDatabase);
      if (!pExtDb->hasRedo(pRxDatabase))
        pIO->putString(OdPropServices::cast(pCmdCtx)->tr("Everything has been redone"));

      OdqGripManagerPtr pGripManager = tab.getCmdContext().gripManager();
      if (!pGripManager.isNull())
        pGripManager->updateAllGrips();
      if (!pExtDb->hasRedo(pRxDatabase))
        getIApp()->refreshQObjetcts("<event-refresh:redo_state_changed>");
      if (pExtDb->hasUndo(pRxDatabase))
        getIApp()->refreshQObjetcts("<event-refresh:undo_state_changed>");

      OdSelectionSetPtr pSSet = tab.workingSelectionSet();
      if (pSSet.get() && pSSet->numEntities())
        getIApp()->refreshQObjetcts("<event-refresh:selection_changed>", "force"); // renew property palette
    }
    else
      pIO->putString(OdPropServices::cast(pCmdCtx)->tr("Nothing to redo"));
      return;
  }
  catch (const OdError &) // err
  {
    ODA_FAIL_ONCE();
  }
}

void OdqCmd_EraseOvr::execute(OdEdCommandContext* pCmdCtx)
{
  ODA_ASSERT_ONCE(module());  // or via ::odrxDynamicLinker()->loadModule(OD_T("OdaQtConsole.dll")).get()
  static_cast<OdqConsoleModule*>(const_cast<OdRxModule*>(module()))->m_cmd_Erase.execute(pCmdCtx);
}

void OdqCmd_Erase::execute(OdEdCommandContext* pCmdCtx)
{
  OdqConsoleTab& tab = *OdqConsole::getConsoleTab(pCmdCtx->baseDatabase());
  IqViewSystem* iVS = getIViewSystem();

  OdRxObject* pRxDatabase = pCmdCtx->baseDatabase();
  OdExtDbModule* pExtDb = NULL;
  if (OdDbDatabase::cast(pRxDatabase).isNull())
    pExtDb = getIApp()->getExtDbModule(pRxDatabase);
  if (pExtDb)
  {
    if (pExtDb->cmdErase(pCmdCtx))
    {
      OdqGripManagerPtr pGripManager = tab.getCmdContext().gripManager();
      if (!pGripManager.isNull())
        pGripManager->updateAllGrips();
      if (iVS)
        iVS->updateViews(pCmdCtx->baseDatabase());
    }
    //else
    //{
    //  ODA_FAIL_ONCE(); // erased
    //}
    return;
  }

  OdDbUserIOPtr pIO = pCmdCtx->userIO();
  OdSelectionSetPtr pSSet = pIO->select();

  int nLockedLayer = 0,
      nCount = 0;
  for (OdDbSelectionSetIteratorPtr pItr = pSSet->newIterator();
       !pItr->done(); pItr->next())
  {
    OdDbEntityPtr pEnt = OdDbEntity::cast(pItr->objectId().safeOpenObject(OdDb::kForWrite));
    ODA_ASSERT_ONCE(!pEnt.isNull());
    if (pEnt.isNull())
      continue;
    nCount++;

    OdDbLayerTableRecordPtr pLayer = pEnt->layerId().safeOpenObject();
    if (pLayer->isLocked())
    {
      nLockedLayer++;
      continue;
    }

    pEnt->erase();
  }
  if (!nCount)
    return;

  //OdqGripManagerPtr pGripManager = tab.getCmdContext().gripManager();
  //if (!pGripManager.isNull())
  //  pGripManager->updateAllGrips();
  //if (iVS)
  //  iVS->updateViews(pCmdCtx->baseDatabase());
  static_cast<OdqConsoleModule*>(const_cast<OdRxModule*>(module()))->m_cmd_DeselectAll.execute(pCmdCtx);

  ODA_ASSERT_ONCE(OdPropServices::cast(pCmdCtx).get());
  OdString sMsg;
  sMsg.format(OdPropServices::cast(pCmdCtx)->tr("  %d found.").c_str(),
              nCount);
  pIO->putString(sMsg);

  if (nLockedLayer)
  {
    sMsg.format(OdPropServices::cast(pCmdCtx)->tr( // msgNWasWereOnALockedLayer
                    (nLockedLayer > 1) ? "  %d were on a locked layer."
                                       : "  %d was on a locked layer.").c_str(), 
                nLockedLayer);
    pIO->putString(sMsg);
  }
}

static OdInt16 GetOSMode(OdEdCommandContext* pCmdCtx)
{
  OdSysVarPEPtr pSysVarPE = OdSysVarPE::cast(pCmdCtx);
  ODA_ASSERT_ONCE(!pSysVarPE.isNull());
  OdSysVarType typeRb = kRtNone;
  bool bOk = false;
  OdString sValue = pSysVarPE->valueToString(pCmdCtx, "osmode", &typeRb, &bOk);
  int nSnapModes = bOk ? odStrToD(sValue) : getIAppProps()->toInt(L"Vars/osmode", 4133);
  return (OdInt16) nSnapModes;
}

static bool SetOSMode(OdEdCommandContext* pCmdCtx, OdInt16 nSnapModes)
{
  if (!getIAppProps()->set(L"Vars/osmode", (OdInt16) nSnapModes, L"int"))
  {
    ODA_FAIL_ONCE();
    return false;
  }
  OdDbDatabasePtr pDwgDb = pCmdCtx->baseDatabase();
  if (pDwgDb.get())
    pDwgDb->appServices()->setOSMODE(nSnapModes);

  ODA_ASSERT_ONCE(GetOSMode(pCmdCtx) == nSnapModes);
  return true;
}


void OdqCmd_OSnap::execute(OdEdCommandContext* pCmdCtx)
{
  OdDbUserIOPtr pIO = pCmdCtx->userIO();
  //if (!getIConsole()->isInputStringReady(pCmdCtx->baseDatabase())
  //{
  //  // TODO
  //  return;
  //}
  QStringList lst;
  lst << "Endpoint"      // kOsModeEnd 1
      << "Midpoint"      // kOsModeMid 2
      << "Center"        // kOsModeCen 3
      << "Node"          // kOsModeNode 4
      << "Quadrant"      // kOsModeQuad 5
    //<< "Intersection"  // kOsModeIntersec 6
      << "Insertion"     // kOsModeIns 7
      << "Perpendicular" // kOsModePerp 8
      << "Tangent"       // kOsModeTan 9
      << "Nearest";      // kOsModeNear 10
    //<< "ApparentIntersection" // kOsModeApint 11
    //<< "Parallel"      // kOsModePar 12
  OdString sPrompt = L"Enter osmode name[",
           sKeywords;
  foreach(QString qsKey, lst)
  {
    OdString sKey = getIApp()->toOdString(qsKey);
    if (!sKeywords.isEmpty())
    {
      sKeywords += L" ";
      sPrompt += L"/"; 
    }
    sKeywords += sKey;
    sPrompt += sKey; 
  }
  sPrompt += OdString().format(L"]<%ls>", getIApp()->toOdString(lst.first()).c_str());
  int idxMode = pIO->getKeyword(sPrompt, sKeywords, 0);
  OdDb::OsnapMode mode = OdDb::OsnapMode(idxMode + 1);
  if (mode > OdDb::kOsModeQuad)
    mode = OdDb::OsnapMode(mode + 1);
  
  int nSnapModes = (int) GetOSMode(pCmdCtx);
  bool bValue = GETBIT(nSnapModes, 1 << (mode - 1)) != 0;
  sPrompt.format(L"Enter %ls state[Yes/No]<%ls>", getIApp()->toOdString(lst[idxMode]).c_str(), 
                                                  bValue  ? L"Yes" : L"No");
  int index = pIO->getKeyword(sPrompt, L"Yes No", bValue ? 0 : 1);
  ODA_ASSERT_ONCE(!index || index == 1);
  if (index == (bValue ? 0 : 1))
    return; // no changes
  if (!index)
    nSnapModes |= 1 << (mode - 1);
  else
    nSnapModes &= ~(1 << (mode - 1));
  if (!SetOSMode(pCmdCtx, (OdInt16) nSnapModes))
  {
    ODA_FAIL_ONCE();
    return;
  }

  getIApp()->refreshQObjetcts("<event-refresh:appvar_changed>", "osmode");
}

void OdqCmd_Export::execute(OdEdCommandContext* pCmdCtx)
{
  //3D DWF (*.dwf) 3D DWFx (*.dwfx): Autodesk Design Web Format (see 3DDWF)
  //Metafile (*.wmf): Microsoft Windows® Metafile (see WMFOUT)
  //ACIS (*.sat): ACIS solid object file (see ACISOUT)
  //Lithography (*.stl): Solid object stereolithography file (see STLOUT)
  //Encapsulated PS (*.eps): Encapsulated PostScript file (see PSOUT)
  //DXX Extract (*.dxx): Attribute extract DXF™ file (see ATTEXT)
  //Bitmap (*.bmp): Device-independent bitmap file (see BMPOUT)
  //Block (*.dwg): Drawing file (see WBLOCK)
  //V7 DGN (*.dgn): MicroStation DGN file (see DGNEXPORT)
  //V8 DGN (*.dgn): MicroStation DGN file (see DGNEXPORT)

  OdEdUserIO* pIO = pCmdCtx->userIO();
  OdRxObject* pRxDb = pCmdCtx->baseDatabase();
  OdString sPath;

  OdDbHostAppServices* pAppServices = OdDbHostAppServices::cast(getIApp()->getBaseHostAppServices()).get();
  bool bFileDialog = pAppServices->getFILEDIA();
  if (!bFileDialog || getIConsole()->isInputStringReady(pCmdCtx->baseDatabase()))
  {
    sPath = pCmdCtx->userIO()->getString(OD_T("Enter file name to export")); // extension defines export type
    if (!sPath.isEmpty())
      return;
  }


  OdString sExtOnly, sFltOnly;
  OdSmartPtr<ExStringIO> pSubCmd = pCmdCtx->arbitraryData(L"EXPORT/SUBCOMMAND");
  OdString sSubCmd;
  if (pSubCmd.get() && !(sSubCmd = pSubCmd->getString(OdString::kEmpty)).isEmpty())
  {
    sSubCmd.makeUpper();
    if (sSubCmd == L"DWFONLY")
      sExtOnly = L"dwf";
    else
    {
      //ODA_ASSERT_ONCE(sSubCmd == L"PDFONLY");
      ODA_FAIL_ONCE(); // TODO
      throw OdError(eNotImplemented);
    }
  }

  static OdString s_sExtension;
  static OdString s_sCurrFilter;
  OdStringArray lstExtDbNames = getIApp()->getExtDbModuleNames(pRxDb);
  OdStringArray extensions;
  OdString sFilters;
  std::map<OdString, OdExtDbModule*> mapExtDbModuleByFilter;
  std::map<OdString, int> mapFltIndexByFilter;
  std::map<OdString, OdString> mapExtByFilter;
  {
    // fill filters

    for (unsigned int idx = 0, sz = lstExtDbNames.size(); idx < sz; idx++)
    {
      OdString sModuleName = lstExtDbNames[idx];
      OdExtDbModule* pExtDb = OdExtDbModule_cast(::odrxDynamicLinker()->loadModule(sModuleName));
      if (!pExtDb)
        continue;
      OdStringArray exts = pExtDb->getImportExportExtensions(pRxDb, false);
      for (unsigned int idxExt = 0, szExt = exts.size(); idxExt < szExt; idxExt++)
      {
        OdString sExt = exts[idxExt];
        sExt.makeLower();

        OdString sFlt = pExtDb->getImportExportFilterByExtension(sExt, 0, false);
        if (sFlt.isEmpty())
          continue;
        if (s_sExtension.isEmpty())
          s_sExtension = sExt;
        for (int idxFilter = 0; !sFlt.isEmpty();
             sFlt = pExtDb->getImportExportFilterByExtension(sExt, ++idxFilter, false))
        {
          if (!extensions.contains(sExt))
            extensions.push_back(sExt);

          OdString sFltExts = L"*." + sExt;

          if (sFlt.find(L"|*") < 0)
            sFlt += OdString().format(L"|%ls|", sFltExts.c_str());
          sFlt.replace(L';', L' '); // problem with QFileDialog

          if (s_sCurrFilter.isEmpty())
            s_sCurrFilter = sFlt;
          if (!sExtOnly.isEmpty())
          {
            if (sExtOnly == sExt && sFltOnly.isEmpty() && sFlt.find(L"3D") < 0)
            {
              sFltOnly = sFlt;
              sFilters += sFlt;
            }
          }
          else
            sFilters += sFlt;
          mapExtDbModuleByFilter[sFlt] = pExtDb;
          mapFltIndexByFilter[sFlt] = idxFilter;
          mapExtByFilter[sFlt] = sExt;
        }
      }
    }
    sFilters += L"|";
  }
  if (sFilters.getLength() < 2)
  {
    pIO->putString(L"There is no supported formats for export.");
    return;
  }

  if (sPath.isEmpty())
  {
    sPath = getIConsole()->getFilePathName(pRxDb);
    int pos = sPath.reverseFind(L'.');
    if (pos > 0)
      sPath = sPath.left(++pos) + (sExtOnly.isEmpty() ? s_sExtension : sExtOnly);

    //qsExtension = getIApp()->toQString(sExtension);
    //if (infoPrev.suffix().isEmpty() || infoPrev.suffix().toLower() != qsExtension)
    //  sPath = getIApp()->toOdString(QDir(infoPrev.absolutePath())
    //    .absoluteFilePath(infoPrev.completeBaseName() + "." + qsExtension));
    //ODA_ASSERT_ONCE(!sPath.isEmpty())

    // TODO: add sCurrentFileter as in / out argument in OdEdUserIO::getFilePath
    // and OdDbBaseHostAppServices::fileDialog.
    // (Unfortunately - as usual - now is a bad time to change interfaces;
    //  way to use tracker for current filter transferring is bad too after 
    //  missing this argument in OdDbBaseHostAppServices::fileDialog declaration) 
    OdString sExtension = sExtOnly.isEmpty() ? s_sCurrFilter : sFltOnly; // see special support for it in CommandContextImpl::getFilePath and OdqServices::fileDialog

    OdString sPrompt = OD_T("Save drawing as");
    sPath = pIO->getFilePath(L"Enter file name to export",
                             OdEd::kGfpForSave,
                             L"Select File to export", // dialog caption 
                             sExtension, // default extension
                             sPath, // file name
                             sFilters);
  }

  int pos = sPath.find(L'|');
  ODA_ASSERT_ONCE(pos >= 0);
  if (pos < 0)
  {
    pIO->putString(L"Error: Unknow filter to export.");
    return;
  }

  s_sCurrFilter = sPath.mid(pos + 1);
  sPath = sPath.left(pos);
  if (sPath.isEmpty() || s_sCurrFilter.isEmpty())
    return; // without message "Cancel by user" // throw OdEdCancel();

  OdString sExtPrev = s_sExtension;
  s_sExtension = mapExtByFilter[s_sCurrFilter];
  if ((pos = sPath.reverseFind(L'.')) > 0)
  {
    OdString sSuffix = sPath.mid(pos + 1);
    if (s_sExtension != sSuffix)
    {
      if (sExtPrev == sSuffix)
        sPath = sPath.left(pos);
      pos = -1;
    }
  }
  if (   (pos = sPath.reverseFind(L'.')) <= 0 
      || pos < sPath.reverseFind(L'\\') || pos < sPath.reverseFind(L'/'))
    sPath += "." + s_sExtension;

  QFileInfo info(getIApp()->toQString(sPath));
  if (sPath.isEmpty() || (info.exists() && !info.isWritable()))
  {
    OdString sMsg = getIApp()->toOdString(
      QObject::tr("File \"%1\" is not accessible to write").arg(getIApp()->toQString(sPath)));
    getIApp()->getBaseHostAppServices()->warning(sMsg);
    return; // without message "Cancel by user" // throw OdEdCancel();
  }

  OdExtDbModule* pExtDb = mapExtDbModuleByFilter[s_sCurrFilter];
  int filterIndex = mapFltIndexByFilter[s_sCurrFilter];
  ODA_ASSERT_ONCE(pExtDb && filterIndex >= 0);
  ODCOLORREF clrBackground = ODRGB(255, 255, 255); // getIAppProps()->toColorref(L"Options/Display/ModelBackground", ODRGB(255, 255, 255));
  getIApp()->toStatusBar(QObject::tr("Export to file %1").arg(info.fileName()));

  pExtDb->exportToFile(pCmdCtx, sPath, filterIndex, clrBackground);
}

static OdString currentLayoutName(OdRxObject* pRxDb)
{
  OdDbBaseDatabasePEPtr pDbPE = OdDbBaseDatabasePE::cast(pRxDb);
  if (pDbPE.isNull())
    return OdString::kEmpty;
  OdDbStub* idLayout = pDbPE->currentLayoutId(pRxDb);
  if (!idLayout)
    return OdString::kEmpty;
  OdRxObjectPtr pRxLayout = pDbPE->openObject(idLayout);
  if (pRxLayout.isNull())
    return OdString::kEmpty;
  OdDbBaseLayoutPEPtr pLayoutPE = OdDbBaseLayoutPE::cast(pRxLayout);
  if (pLayoutPE.isNull())
    return OdString::kEmpty;
  return pLayoutPE->name(pRxLayout);
}

void OdqCmd_Import::execute(OdEdCommandContext* pCmdCtx)
{
  //Metafile (*.wmf): Microsoft Windows® Metafile (see WMFIN and WMFOPTS)
  //ACIS (*.sat): ACIS solid object file (see ACISIN)
  //3D Studio (*.3ds): 3D Studio file (see 3DSIN)
  //MicroStation DGN (*.dgn): MicroStation DGN file (see DGNIMPORT)
  //All DGN files (*.*): All DGN files (see DGNIMPORT)

  OdRxObject* pRxDatabase = pCmdCtx->baseDatabase();
  OdDbDatabase* pDb = OdDbDatabase::cast(pRxDatabase).get();
  if (!pDb)
  {
    // possible TODO for import to non-DWG database
    throw OdError(eNoDatabase);
  }
  OdDbHostAppServices* pAppServices = OdDbHostAppServices::cast(getIApp()->getBaseHostAppServices()).get();
  bool bFileDialog = pAppServices->getFILEDIA();

  OdPropServicesPtr pPropServices = OdPropServices::cast(pCmdCtx);
  ODA_ASSERT_ONCE(!pPropServices.isNull());

  OdString sPath,
           sMsg;
  if (!bFileDialog || (!pPropServices.isNull() && pPropServices->isInputStringReady(pCmdCtx)))
  {
    sPath = pCmdCtx->userIO()->getString(OD_T("Enter name file to import"));
    if (!sPath.isEmpty() && !odrxSystemServices()->accessFile(sPath, Oda::kFileRead))
    {
      sMsg.format(L"File \"%1s\" was not found", sPath.c_str());
      pAppServices->warning(sMsg);
      return;
    }
  }

  static OdString s_sExtension;
  static OdStringArray s_extensions;
  static std::map<OdString, OdString> s_mapExtBySynonym;
  static OdString s_sFilters;
  static OdStringArray s_lstExtDbNames;
  {
    OdStringArray lst = getIApp()->getExtDbModuleNames(pCmdCtx->baseDatabase());
    if (s_lstExtDbNames.size() != lst.size())
    {
      s_sFilters.empty(); // to recollect
      s_extensions.clear();
      s_mapExtBySynonym.clear();
      s_lstExtDbNames = lst;
    }
  }
  if (sPath.isEmpty() && s_sFilters.isEmpty())
  {
    // fill filters

    OdString sAllExts;

    int numFilters = 0;
    for (unsigned int idx = 0, sz = s_lstExtDbNames.size(); idx < sz; idx++)
    {
      OdString sModuleName = s_lstExtDbNames[idx];
      OdExtDbModule* pExtDb = OdExtDbModule_cast(::odrxDynamicLinker()->loadModule(sModuleName));
      if (!pExtDb)
        continue;
      OdStringArray exts = pExtDb->getImportExportExtensions(pRxDatabase);
      for (unsigned int idxExt = 0, szExt = exts.size(); idxExt < szExt; idxExt++)
      {
        OdString sExt = exts[idxExt];
        sExt.makeLower();

        OdString sFilter = pExtDb->getImportExportFilterByExtension(sExt);
        if (sFilter.isEmpty())
          continue;
        if (s_sExtension.isEmpty())
          s_sExtension = sExt;
        for (int idxFilter = 0; !sFilter.isEmpty();
             sFilter = pExtDb->getImportExportFilterByExtension(sExt, ++idxFilter))
        {
          OdStringArray extSynonyms;

          if (!s_extensions.contains(sExt))
            s_extensions.push_back(sExt);

          OdString sFltExts = L"*." + sExt;
          unsigned int szSyn = extSynonyms.size();
          if (szSyn)
          {
            for (unsigned int idxSyn = 0, szSyn = extSynonyms.size(); idxSyn < szSyn; idxSyn++)
            {
              OdString extSyn = extSynonyms[idxSyn];
              ODA_ASSERT_ONCE(!extSyn.isEmpty() && !exts.contains(extSyn));

              sFltExts += L" *." + extSyn;
              s_mapExtBySynonym[extSyn] = sExt;
            }
            sFilter.replace(L';', L' '); // problem with QFileDialog
          }
          sAllExts += (sAllExts.isEmpty() ? L"" : L" ") + sFltExts;

          if (sFilter.find(L"|*") < 0)
            sFilter += OdString().format(L"|%ls|", sFltExts.c_str());

          s_sFilters += sFilter;
          numFilters++;
        }
      }
    }

    if (numFilters > 1)
      s_sFilters += OdString().format(L"All files (%ls)|%ls|", sAllExts.c_str(), sAllExts.c_str());
    s_sFilters += L"|";
    // or 
    // s_sFilters += L"All files (*.*)|*.*||";
  }


  if (sPath.isEmpty())
  {
    sPath = pCmdCtx->userIO()->getFilePath(OD_T("Enter file name to import"), 
                                           OdEd::kGfpForOpen, 
                                           OD_T("Select File to import"), // dialog caption 
                                           s_sExtension, // default extension
                                           OdString::kEmpty, // file name
                                           s_sFilters);
    if (sPath.isEmpty())
      return; // without message "Cancel by user" // throw OdEdCancel();
  }

  QFileInfo info(getIApp()->toQString(sPath));
  if (info.suffix().isEmpty())
  {
    ODA_FAIL_ONCE(); // suspicion to invalid file
    return;
  }
  s_sExtension = getIApp()->toOdString(info.suffix().toLower());
  if (!s_extensions.contains(s_sExtension))
  {
    if (!s_mapExtBySynonym[s_sExtension].isEmpty())
      s_sExtension = s_mapExtBySynonym[s_sExtension];
    ODA_ASSERT_ONCE(s_extensions.isEmpty() || s_extensions.contains(s_sExtension));
  }

  OdExtDbModule* pExtDb = getIApp()->getExtDbModuleByExtension(s_sExtension);
  ODA_ASSERT_ONCE(pExtDb);
  if (!pExtDb)
    return;

  OdString sPrevLayout = currentLayoutName(pRxDatabase);
  ODCOLORREF clrBackground = getIAppProps()->toColorref(L"Options/Display/ModelBackground", ODRGB(255, 255, 255));

  if (getIApp()->importFromFile(pExtDb, pCmdCtx, sPath, clrBackground))
  {
    pDb->setXCLIPFRAME(0); // 2 is default (clipped rect is invisible with plot only)
    OdString sLayout = currentLayoutName(pRxDatabase);
    if (sLayout != sPrevLayout && !sLayout.isEmpty())
      getIConsole()->postponeCommand(QString("qvlayout \"%1\"").arg(getIApp()->toQString(sLayout)),
                                     false, pRxDatabase);

    //if (clrBackground != getIAppProps()->toColorref(L"Options/Display/ModelBackground", ODRGB(255, 255, 255)))
    //{
    //  OdVariant vBackground(clrBackground);
    //  //QString qsValue = QString("0x%1").arg(clrBackground, 6, 16, QLatin1Char('0'));
    //  //OdVariant vBackground(getIApp()->toOdString(qsValue));
    //  getIAppProps()->set(L"Options/Display/ModelBackground", vBackground);
    //}
  }
}

//////////////////////////////////////////////////////////////////////////

#ifdef _TEST_COMMAND

#include "DbBlockTableRecord.h"
#include "XRefMan.h"
#include "OdDToStr.h" // for odStrToD
#include <QUuid>

#include "DbViewport.h"
#include "DbLine.h"
#include "DbSymUtl.h"

#if defined(__APPLE__)
inline int _tdswscanf(const wchar_t* buffer, const wchar_t* format, ...)
{
  va_list argList;
  va_start(argList, format);
  int count = Od_vswscanfV(buffer, format, argList);
  va_end(argList);
  return count;
}
#define tst_odSScanf _tdswscanf
#else
#define tst_odSScanf odSScanf
#endif

//------------------------------------------------------------------------
// This code was generated by test command of ODADrawingsExplorer (Debug only)
// (see function putCode_ToLowerAndUpper)

inline OdUInt16 getOffsetToUpper(const OdChar& sym)
{
  if (OdUInt32(sym) >= 0x10000) return 0;
  OdUInt16 chSym = OdUInt16(sym);
  if (chSym >= 0x61 && chSym <= 0x7A) return 0xFFE0;
  if (chSym >= 0xE0 && chSym <= 0xF6) return 0xFFE0;
  if (chSym >= 0xF8 && chSym <= 0xFE) return 0xFFE0;
  if (chSym >= 0x256 && chSym <= 0x257) return 0xFF33;
  if (chSym >= 0x28A && chSym <= 0x28B) return 0xFF27;
  if (chSym >= 0x37B && chSym <= 0x37D) return 0x82;
  if (chSym >= 0x3AD && chSym <= 0x3AF) return 0xFFDB;
  if (chSym >= 0x3B1 && chSym <= 0x3C1) return 0xFFE0;
  if (chSym >= 0x3C3 && chSym <= 0x3CB) return 0xFFE0;
  if (chSym >= 0x3CD && chSym <= 0x3CE) return 0xFFC1;
  if (chSym >= 0x430 && chSym <= 0x44F) return 0xFFE0;
  if (chSym >= 0x450 && chSym <= 0x45F) return 0xFFB0;
  if (chSym >= 0x561 && chSym <= 0x586) return 0xFFD0;
  if (chSym >= 0x1F00 && chSym <= 0x1F07) return 0x8;
  if (chSym >= 0x1F10 && chSym <= 0x1F15) return 0x8;
  if (chSym >= 0x1F20 && chSym <= 0x1F27) return 0x8;
  if (chSym >= 0x1F30 && chSym <= 0x1F37) return 0x8;
  if (chSym >= 0x1F40 && chSym <= 0x1F45) return 0x8;
  if (chSym >= 0x1F60 && chSym <= 0x1F67) return 0x8;
  if (chSym >= 0x1F70 && chSym <= 0x1F71) return 0x4A;
  if (chSym >= 0x1F72 && chSym <= 0x1F75) return 0x56;
  if (chSym >= 0x1F76 && chSym <= 0x1F77) return 0x64;
  if (chSym >= 0x1F78 && chSym <= 0x1F79) return 0x80;
  if (chSym >= 0x1F7A && chSym <= 0x1F7B) return 0x70;
  if (chSym >= 0x1F7C && chSym <= 0x1F7D) return 0x7E;
  if (chSym >= 0x1FB0 && chSym <= 0x1FB1) return 0x8;
  if (chSym >= 0x1FD0 && chSym <= 0x1FD1) return 0x8;
  if (chSym >= 0x1FE0 && chSym <= 0x1FE1) return 0x8;
  if (chSym >= 0x2170 && chSym <= 0x217F) return 0xFFF0;
  if (chSym >= 0x24D0 && chSym <= 0x24E9) return 0xFFE6;
  if (chSym >= 0x2C30 && chSym <= 0x2C5E) return 0xFFD0;
  if (chSym >= 0x2D00 && chSym <= 0x2D25) return 0xE3A0;
  if (chSym >= 0xFF41 && chSym <= 0xFF5A) return 0xFFE0;
  switch ((OdUInt16) chSym)
  {
  case 0xFF:
    return 0x79;
  case 0x101:  case 0x103:  case 0x105:  case 0x107:  case 0x109:  case 0x10B:  case 0x10D:
  case 0x10F:  case 0x111:  case 0x113:  case 0x115:  case 0x117:  case 0x119:  case 0x11B:
  case 0x11D:  case 0x11F:  case 0x121:  case 0x123:  case 0x125:  case 0x127:  case 0x129:
  case 0x12B:  case 0x12D:  case 0x12F:  case 0x133:  case 0x135:  case 0x137:  case 0x13A:
  case 0x13C:  case 0x13E:  case 0x140:  case 0x142:  case 0x144:  case 0x146:  case 0x148:
  case 0x14B:  case 0x14D:  case 0x14F:  case 0x151:  case 0x153:  case 0x155:  case 0x157:
  case 0x159:  case 0x15B:  case 0x15D:  case 0x15F:  case 0x161:  case 0x163:  case 0x165:
  case 0x167:  case 0x169:  case 0x16B:  case 0x16D:  case 0x16F:  case 0x171:  case 0x173:
  case 0x175:  case 0x177:  case 0x17A:  case 0x17C:  case 0x17E:
    return 0xFFFF;
  case 0x180:
    return 0xC3;
  case 0x183:  case 0x185:  case 0x188:  case 0x18C:  case 0x192:
    return 0xFFFF;
  case 0x195:
    return 0x61;
  case 0x199:
    return 0xFFFF;
  case 0x19A:
    return 0xA3;
  case 0x19E:
    return 0x82;
  case 0x1A1:  case 0x1A3:  case 0x1A5:  case 0x1A8:  case 0x1AD:  case 0x1B0:  case 0x1B4:
  case 0x1B6:  case 0x1B9:  case 0x1BD:
    return 0xFFFF;
  case 0x1BF:
    return 0x38;
  case 0x1C6:  case 0x1C9:  case 0x1CC:
    return 0xFFFE;
  case 0x1CE:  case 0x1D0:  case 0x1D2:  case 0x1D4:  case 0x1D6:  case 0x1D8:  case 0x1DA:
  case 0x1DC:
    return 0xFFFF;
  case 0x1DD:
    return 0xFFB1;
  case 0x1DF:  case 0x1E1:  case 0x1E3:  case 0x1E5:  case 0x1E7:  case 0x1E9:  case 0x1EB:
  case 0x1ED:  case 0x1EF:
    return 0xFFFF;
  case 0x1F3:
    return 0xFFFE;
  case 0x1F5:  case 0x1F9:  case 0x1FB:  case 0x1FD:  case 0x1FF:  case 0x201:  case 0x203:
  case 0x205:  case 0x207:  case 0x209:  case 0x20B:  case 0x20D:  case 0x20F:  case 0x211:
  case 0x213:  case 0x215:  case 0x217:  case 0x219:  case 0x21B:  case 0x21D:  case 0x21F:
  case 0x223:  case 0x225:  case 0x227:  case 0x229:  case 0x22B:  case 0x22D:  case 0x22F:
  case 0x231:  case 0x233:  case 0x23C:  case 0x242:  case 0x247:  case 0x249:  case 0x24B:
  case 0x24D:  case 0x24F:
    return 0xFFFF;
  case 0x253:
    return 0xFF2E;
  case 0x254:
    return 0xFF32;
  case 0x259:
    return 0xFF36;
  case 0x25B:
    return 0xFF35;
  case 0x260:
    return 0xFF33;
  case 0x263:
    return 0xFF31;
  case 0x268:
    return 0xFF2F;
  case 0x269:
    return 0xFF2D;
  case 0x26B:
    return 0x29F7;
  case 0x26F:
    return 0xFF2D;
  case 0x272:
    return 0xFF2B;
  case 0x275:
    return 0xFF2A;
  case 0x27D:
    return 0x29E7;
  case 0x280:  case 0x283:  case 0x288:
    return 0xFF26;
  case 0x289:
    return 0xFFBB;
  case 0x28C:
    return 0xFFB9;
  case 0x292:
    return 0xFF25;
  case 0x3AC:
    return 0xFFDA;
  case 0x3CC:
    return 0xFFC0;
  case 0x3D9:  case 0x3DB:  case 0x3DD:  case 0x3DF:  case 0x3E1:  case 0x3E3:  case 0x3E5:
  case 0x3E7:  case 0x3E9:  case 0x3EB:  case 0x3ED:  case 0x3EF:
    return 0xFFFF;
  case 0x3F2:
    return 0x7;
  case 0x3F8:  case 0x3FB:  case 0x461:  case 0x463:  case 0x465:  case 0x467:  case 0x469:
  case 0x46B:  case 0x46D:  case 0x46F:  case 0x471:  case 0x473:  case 0x475:  case 0x477:
  case 0x479:  case 0x47B:  case 0x47D:  case 0x47F:  case 0x481:  case 0x48B:  case 0x48D:
  case 0x48F:  case 0x491:  case 0x493:  case 0x495:  case 0x497:  case 0x499:  case 0x49B:
  case 0x49D:  case 0x49F:  case 0x4A1:  case 0x4A3:  case 0x4A5:  case 0x4A7:  case 0x4A9:
  case 0x4AB:  case 0x4AD:  case 0x4AF:  case 0x4B1:  case 0x4B3:  case 0x4B5:  case 0x4B7:
  case 0x4B9:  case 0x4BB:  case 0x4BD:  case 0x4BF:  case 0x4C2:  case 0x4C4:  case 0x4C6:
  case 0x4C8:  case 0x4CA:  case 0x4CC:  case 0x4CE:
    return 0xFFFF;
  case 0x4CF:
    return 0xFFF1;
  case 0x4D1:  case 0x4D3:  case 0x4D5:  case 0x4D7:  case 0x4D9:  case 0x4DB:  case 0x4DD:
  case 0x4DF:  case 0x4E1:  case 0x4E3:  case 0x4E5:  case 0x4E7:  case 0x4E9:  case 0x4EB:
  case 0x4ED:  case 0x4EF:  case 0x4F1:  case 0x4F3:  case 0x4F5:  case 0x4F7:  case 0x4F9:
  case 0x4FB:  case 0x4FD:  case 0x4FF:  case 0x501:  case 0x503:  case 0x505:  case 0x507:
  case 0x509:  case 0x50B:  case 0x50D:  case 0x50F:  case 0x511:  case 0x513:
    return 0xFFFF;
  case 0x1D7D:
    return 0xEE6;
  case 0x1E01:  case 0x1E03:  case 0x1E05:  case 0x1E07:  case 0x1E09:  case 0x1E0B:
  case 0x1E0D:  case 0x1E0F:  case 0x1E11:  case 0x1E13:  case 0x1E15:  case 0x1E17:
  case 0x1E19:  case 0x1E1B:  case 0x1E1D:  case 0x1E1F:  case 0x1E21:  case 0x1E23:
  case 0x1E25:  case 0x1E27:  case 0x1E29:  case 0x1E2B:  case 0x1E2D:  case 0x1E2F:
  case 0x1E31:  case 0x1E33:  case 0x1E35:  case 0x1E37:  case 0x1E39:  case 0x1E3B:
  case 0x1E3D:  case 0x1E3F:  case 0x1E41:  case 0x1E43:  case 0x1E45:  case 0x1E47:
  case 0x1E49:  case 0x1E4B:  case 0x1E4D:  case 0x1E4F:  case 0x1E51:  case 0x1E53:
  case 0x1E55:  case 0x1E57:  case 0x1E59:  case 0x1E5B:  case 0x1E5D:  case 0x1E5F:
  case 0x1E61:  case 0x1E63:  case 0x1E65:  case 0x1E67:  case 0x1E69:  case 0x1E6B:
  case 0x1E6D:  case 0x1E6F:  case 0x1E71:  case 0x1E73:  case 0x1E75:  case 0x1E77:
  case 0x1E79:  case 0x1E7B:  case 0x1E7D:  case 0x1E7F:  case 0x1E81:  case 0x1E83:
  case 0x1E85:  case 0x1E87:  case 0x1E89:  case 0x1E8B:  case 0x1E8D:  case 0x1E8F:
  case 0x1E91:  case 0x1E93:  case 0x1E95:  case 0x1EA1:  case 0x1EA3:  case 0x1EA5:
  case 0x1EA7:  case 0x1EA9:  case 0x1EAB:  case 0x1EAD:  case 0x1EAF:  case 0x1EB1:
  case 0x1EB3:  case 0x1EB5:  case 0x1EB7:  case 0x1EB9:  case 0x1EBB:  case 0x1EBD:
  case 0x1EBF:  case 0x1EC1:  case 0x1EC3:  case 0x1EC5:  case 0x1EC7:  case 0x1EC9:
  case 0x1ECB:  case 0x1ECD:  case 0x1ECF:  case 0x1ED1:  case 0x1ED3:  case 0x1ED5:
  case 0x1ED7:  case 0x1ED9:  case 0x1EDB:  case 0x1EDD:  case 0x1EDF:  case 0x1EE1:
  case 0x1EE3:  case 0x1EE5:  case 0x1EE7:  case 0x1EE9:  case 0x1EEB:  case 0x1EED:
  case 0x1EEF:  case 0x1EF1:  case 0x1EF3:  case 0x1EF5:  case 0x1EF7:  case 0x1EF9:
    return 0xFFFF;
  case 0x1F51:  case 0x1F53:  case 0x1F55:  case 0x1F57:
    return 0x8;
  case 0x1FE5:
    return 0x7;
  case 0x214E:
    return 0xFFE4;
  case 0x2184:  case 0x2C61:
    return 0xFFFF;
  case 0x2C65:
    return 0xD5D5;
  case 0x2C66:
    return 0xD5D8;
  case 0x2C68:  case 0x2C6A:  case 0x2C6C:  case 0x2C76:  case 0x2C81:  case 0x2C83:
  case 0x2C85:  case 0x2C87:  case 0x2C89:  case 0x2C8B:  case 0x2C8D:  case 0x2C8F:
  case 0x2C91:  case 0x2C93:  case 0x2C95:  case 0x2C97:  case 0x2C99:  case 0x2C9B:
  case 0x2C9D:  case 0x2C9F:  case 0x2CA1:  case 0x2CA3:  case 0x2CA5:  case 0x2CA7:
  case 0x2CA9:  case 0x2CAB:  case 0x2CAD:  case 0x2CAF:  case 0x2CB1:  case 0x2CB3:
  case 0x2CB5:  case 0x2CB7:  case 0x2CB9:  case 0x2CBB:  case 0x2CBD:  case 0x2CBF:
  case 0x2CC1:  case 0x2CC3:  case 0x2CC5:  case 0x2CC7:  case 0x2CC9:  case 0x2CCB:
  case 0x2CCD:  case 0x2CCF:  case 0x2CD1:  case 0x2CD3:  case 0x2CD5:  case 0x2CD7:
  case 0x2CD9:  case 0x2CDB:  case 0x2CDD:  case 0x2CDF:  case 0x2CE1:  case 0x2CE3:
    return 0xFFFF;
  }
  return 0;
}

inline OdUInt16 getOffsetToLower(const OdChar& sym)
{
  if (OdUInt32(sym) >= 0x10000) return 0;
  OdUInt16 chSym = OdUInt16(sym);
  if (chSym >= 0x41 && chSym <= 0x5A) return 0x20;
  if (chSym >= 0xC0 && chSym <= 0xD6) return 0x20;
  if (chSym >= 0xD8 && chSym <= 0xDE) return 0x20;
  if (chSym >= 0x189 && chSym <= 0x18A) return 0xCD;
  if (chSym >= 0x1B1 && chSym <= 0x1B2) return 0xD9;
  if (chSym >= 0x388 && chSym <= 0x38A) return 0x25;
  if (chSym >= 0x38E && chSym <= 0x38F) return 0x3F;
  if (chSym >= 0x391 && chSym <= 0x3A1) return 0x20;
  if (chSym >= 0x3A3 && chSym <= 0x3AB) return 0x20;
  if (chSym >= 0x3FD && chSym <= 0x3FF) return 0xFF7E;
  if (chSym >= 0x400 && chSym <= 0x40F) return 0x50;
  if (chSym >= 0x410 && chSym <= 0x42F) return 0x20;
  if (chSym >= 0x531 && chSym <= 0x556) return 0x30;
  if (chSym >= 0x10A0 && chSym <= 0x10C5) return 0x1C60;
  if (chSym >= 0x1F08 && chSym <= 0x1F0F) return 0xFFF8;
  if (chSym >= 0x1F18 && chSym <= 0x1F1D) return 0xFFF8;
  if (chSym >= 0x1F28 && chSym <= 0x1F2F) return 0xFFF8;
  if (chSym >= 0x1F38 && chSym <= 0x1F3F) return 0xFFF8;
  if (chSym >= 0x1F48 && chSym <= 0x1F4D) return 0xFFF8;
  if (chSym >= 0x1F68 && chSym <= 0x1F6F) return 0xFFF8;
  if (chSym >= 0x1F88 && chSym <= 0x1F8F) return 0xFFF8;
  if (chSym >= 0x1F98 && chSym <= 0x1F9F) return 0xFFF8;
  if (chSym >= 0x1FA8 && chSym <= 0x1FAF) return 0xFFF8;
  if (chSym >= 0x1FB8 && chSym <= 0x1FB9) return 0xFFF8;
  if (chSym >= 0x1FBA && chSym <= 0x1FBB) return 0xFFB6;
  if (chSym >= 0x1FC8 && chSym <= 0x1FCB) return 0xFFAA;
  if (chSym >= 0x1FD8 && chSym <= 0x1FD9) return 0xFFF8;
  if (chSym >= 0x1FDA && chSym <= 0x1FDB) return 0xFF9C;
  if (chSym >= 0x1FE8 && chSym <= 0x1FE9) return 0xFFF8;
  if (chSym >= 0x1FEA && chSym <= 0x1FEB) return 0xFF90;
  if (chSym >= 0x1FF8 && chSym <= 0x1FF9) return 0xFF80;
  if (chSym >= 0x1FFA && chSym <= 0x1FFB) return 0xFF82;
  if (chSym >= 0x2160 && chSym <= 0x216F) return 0x10;
  if (chSym >= 0x24B6 && chSym <= 0x24CF) return 0x1A;
  if (chSym >= 0x2C00 && chSym <= 0x2C2E) return 0x30;
  if (chSym >= 0xFF21 && chSym <= 0xFF3A) return 0x20;
  switch ((OdUInt16) chSym)
  {
  case 0x100:  case 0x102:  case 0x104:  case 0x106:  case 0x108:  case 0x10A:  case 0x10C:
  case 0x10E:  case 0x110:  case 0x112:  case 0x114:  case 0x116:  case 0x118:  case 0x11A:
  case 0x11C:  case 0x11E:  case 0x120:  case 0x122:  case 0x124:  case 0x126:  case 0x128:
  case 0x12A:  case 0x12C:  case 0x12E:  case 0x132:  case 0x134:  case 0x136:  case 0x139:
  case 0x13B:  case 0x13D:  case 0x13F:  case 0x141:  case 0x143:  case 0x145:  case 0x147:
  case 0x14A:  case 0x14C:  case 0x14E:  case 0x150:  case 0x152:  case 0x154:  case 0x156:
  case 0x158:  case 0x15A:  case 0x15C:  case 0x15E:  case 0x160:  case 0x162:  case 0x164:
  case 0x166:  case 0x168:  case 0x16A:  case 0x16C:  case 0x16E:  case 0x170:  case 0x172:
  case 0x174:  case 0x176:
    return 0x1;
  case 0x178:
    return 0xFF87;
  case 0x179:  case 0x17B:  case 0x17D:
    return 0x1;
  case 0x181:
    return 0xD2;
  case 0x182:  case 0x184:
    return 0x1;
  case 0x186:
    return 0xCE;
  case 0x187:  case 0x18B:
    return 0x1;
  case 0x18E:
    return 0x4F;
  case 0x18F:
    return 0xCA;
  case 0x190:
    return 0xCB;
  case 0x191:
    return 0x1;
  case 0x193:
    return 0xCD;
  case 0x194:
    return 0xCF;
  case 0x196:
    return 0xD3;
  case 0x197:
    return 0xD1;
  case 0x198:
    return 0x1;
  case 0x19C:
    return 0xD3;
  case 0x19D:
    return 0xD5;
  case 0x19F:
    return 0xD6;
  case 0x1A0:  case 0x1A2:  case 0x1A4:
    return 0x1;
  case 0x1A6:
    return 0xDA;
  case 0x1A7:
    return 0x1;
  case 0x1A9:
    return 0xDA;
  case 0x1AC:
    return 0x1;
  case 0x1AE:
    return 0xDA;
  case 0x1AF:  case 0x1B3:  case 0x1B5:
    return 0x1;
  case 0x1B7:
    return 0xDB;
  case 0x1B8:  case 0x1BC:
    return 0x1;
  case 0x1C4:  case 0x1C7:  case 0x1CA:
    return 0x2;
  case 0x1CD:  case 0x1CF:  case 0x1D1:  case 0x1D3:  case 0x1D5:  case 0x1D7:  case 0x1D9:
  case 0x1DB:  case 0x1DE:  case 0x1E0:  case 0x1E2:  case 0x1E4:  case 0x1E6:  case 0x1E8:
  case 0x1EA:  case 0x1EC:  case 0x1EE:
    return 0x1;
  case 0x1F1:
    return 0x2;
  case 0x1F4:
    return 0x1;
  case 0x1F6:
    return 0xFF9F;
  case 0x1F7:
    return 0xFFC8;
  case 0x1F8:  case 0x1FA:  case 0x1FC:  case 0x1FE:  case 0x200:  case 0x202:  case 0x204:
  case 0x206:  case 0x208:  case 0x20A:  case 0x20C:  case 0x20E:  case 0x210:  case 0x212:
  case 0x214:  case 0x216:  case 0x218:  case 0x21A:  case 0x21C:  case 0x21E:
    return 0x1;
  case 0x220:
    return 0xFF7E;
  case 0x222:  case 0x224:  case 0x226:  case 0x228:  case 0x22A:  case 0x22C:  case 0x22E:
  case 0x230:  case 0x232:
    return 0x1;
  case 0x23A:
    return 0x2A2B;
  case 0x23B:
    return 0x1;
  case 0x23D:
    return 0xFF5D;
  case 0x23E:
    return 0x2A28;
  case 0x241:
    return 0x1;
  case 0x243:
    return 0xFF3D;
  case 0x244:
    return 0x45;
  case 0x245:
    return 0x47;
  case 0x246:  case 0x248:  case 0x24A:  case 0x24C:  case 0x24E:
    return 0x1;
  case 0x386:
    return 0x26;
  case 0x38C:
    return 0x40;
  case 0x3D8:  case 0x3DA:  case 0x3DC:  case 0x3DE:  case 0x3E0:  case 0x3E2:  case 0x3E4:
  case 0x3E6:  case 0x3E8:  case 0x3EA:  case 0x3EC:  case 0x3EE:  case 0x3F7:
    return 0x1;
  case 0x3F9:
    return 0xFFF9;
  case 0x3FA:  case 0x460:  case 0x462:  case 0x464:  case 0x466:  case 0x468:  case 0x46A:
  case 0x46C:  case 0x46E:  case 0x470:  case 0x472:  case 0x474:  case 0x476:  case 0x478:
  case 0x47A:  case 0x47C:  case 0x47E:  case 0x480:  case 0x48A:  case 0x48C:  case 0x48E:
  case 0x490:  case 0x492:  case 0x494:  case 0x496:  case 0x498:  case 0x49A:  case 0x49C:
  case 0x49E:  case 0x4A0:  case 0x4A2:  case 0x4A4:  case 0x4A6:  case 0x4A8:  case 0x4AA:
  case 0x4AC:  case 0x4AE:  case 0x4B0:  case 0x4B2:  case 0x4B4:  case 0x4B6:  case 0x4B8:
  case 0x4BA:  case 0x4BC:  case 0x4BE:
    return 0x1;
  case 0x4C0:
    return 0xF;
  case 0x4C1:  case 0x4C3:  case 0x4C5:  case 0x4C7:  case 0x4C9:  case 0x4CB:  case 0x4CD:
  case 0x4D0:  case 0x4D2:  case 0x4D4:  case 0x4D6:  case 0x4D8:  case 0x4DA:  case 0x4DC:
  case 0x4DE:  case 0x4E0:  case 0x4E2:  case 0x4E4:  case 0x4E6:  case 0x4E8:  case 0x4EA:
  case 0x4EC:  case 0x4EE:  case 0x4F0:  case 0x4F2:  case 0x4F4:  case 0x4F6:  case 0x4F8:
  case 0x4FA:  case 0x4FC:  case 0x4FE:  case 0x500:  case 0x502:  case 0x504:  case 0x506:
  case 0x508:  case 0x50A:  case 0x50C:  case 0x50E:  case 0x510:  case 0x512:  case 0x1E00:
  case 0x1E02:  case 0x1E04:  case 0x1E06:  case 0x1E08:  case 0x1E0A:  case 0x1E0C:
  case 0x1E0E:  case 0x1E10:  case 0x1E12:  case 0x1E14:  case 0x1E16:  case 0x1E18:
  case 0x1E1A:  case 0x1E1C:  case 0x1E1E:  case 0x1E20:  case 0x1E22:  case 0x1E24:
  case 0x1E26:  case 0x1E28:  case 0x1E2A:  case 0x1E2C:  case 0x1E2E:  case 0x1E30:
  case 0x1E32:  case 0x1E34:  case 0x1E36:  case 0x1E38:  case 0x1E3A:  case 0x1E3C:
  case 0x1E3E:  case 0x1E40:  case 0x1E42:  case 0x1E44:  case 0x1E46:  case 0x1E48:
  case 0x1E4A:  case 0x1E4C:  case 0x1E4E:  case 0x1E50:  case 0x1E52:  case 0x1E54:
  case 0x1E56:  case 0x1E58:  case 0x1E5A:  case 0x1E5C:  case 0x1E5E:  case 0x1E60:
  case 0x1E62:  case 0x1E64:  case 0x1E66:  case 0x1E68:  case 0x1E6A:  case 0x1E6C:
  case 0x1E6E:  case 0x1E70:  case 0x1E72:  case 0x1E74:  case 0x1E76:  case 0x1E78:
  case 0x1E7A:  case 0x1E7C:  case 0x1E7E:  case 0x1E80:  case 0x1E82:  case 0x1E84:
  case 0x1E86:  case 0x1E88:  case 0x1E8A:  case 0x1E8C:  case 0x1E8E:  case 0x1E90:
  case 0x1E92:  case 0x1E94:  case 0x1EA0:  case 0x1EA2:  case 0x1EA4:  case 0x1EA6:
  case 0x1EA8:  case 0x1EAA:  case 0x1EAC:  case 0x1EAE:  case 0x1EB0:  case 0x1EB2:
  case 0x1EB4:  case 0x1EB6:  case 0x1EB8:  case 0x1EBA:  case 0x1EBC:  case 0x1EBE:
  case 0x1EC0:  case 0x1EC2:  case 0x1EC4:  case 0x1EC6:  case 0x1EC8:  case 0x1ECA:
  case 0x1ECC:  case 0x1ECE:  case 0x1ED0:  case 0x1ED2:  case 0x1ED4:  case 0x1ED6:
  case 0x1ED8:  case 0x1EDA:  case 0x1EDC:  case 0x1EDE:  case 0x1EE0:  case 0x1EE2:
  case 0x1EE4:  case 0x1EE6:  case 0x1EE8:  case 0x1EEA:  case 0x1EEC:  case 0x1EEE:
  case 0x1EF0:  case 0x1EF2:  case 0x1EF4:  case 0x1EF6:  case 0x1EF8:
    return 0x1;
  case 0x1F59:  case 0x1F5B:  case 0x1F5D:  case 0x1F5F:
    return 0xFFF8;
  case 0x1FBC:  case 0x1FCC:
    return 0xFFF7;
  case 0x1FEC:
    return 0xFFF9;
  case 0x1FFC:
    return 0xFFF7;
  case 0x2132:
    return 0x1C;
  case 0x2183:  case 0x2C60:
    return 0x1;
  case 0x2C62:
    return 0xD609;
  case 0x2C63:
    return 0xF11A;
  case 0x2C64:
    return 0xD619;
  case 0x2C67:  case 0x2C69:  case 0x2C6B:  case 0x2C75:  case 0x2C80:  case 0x2C82:
  case 0x2C84:  case 0x2C86:  case 0x2C88:  case 0x2C8A:  case 0x2C8C:  case 0x2C8E:
  case 0x2C90:  case 0x2C92:  case 0x2C94:  case 0x2C96:  case 0x2C98:  case 0x2C9A:
  case 0x2C9C:  case 0x2C9E:  case 0x2CA0:  case 0x2CA2:  case 0x2CA4:  case 0x2CA6:
  case 0x2CA8:  case 0x2CAA:  case 0x2CAC:  case 0x2CAE:  case 0x2CB0:  case 0x2CB2:
  case 0x2CB4:  case 0x2CB6:  case 0x2CB8:  case 0x2CBA:  case 0x2CBC:  case 0x2CBE:
  case 0x2CC0:  case 0x2CC2:  case 0x2CC4:  case 0x2CC6:  case 0x2CC8:  case 0x2CCA:
  case 0x2CCC:  case 0x2CCE:  case 0x2CD0:  case 0x2CD2:  case 0x2CD4:  case 0x2CD6:
  case 0x2CD8:  case 0x2CDA:  case 0x2CDC:  case 0x2CDE:  case 0x2CE0:  case 0x2CE2:
    return 0x1;
  }
  return 0;
}

//------------------------------------------------------------------------

//ODSTRUPR(Od_strupr, OdChar)
OdChar* Test_odStrUpr(OdChar* str)
{
  OdChar* temp = str;
  while (*str)
  {
    OdChar offs = (OdChar)(OdInt16) getOffsetToUpper(*str);
    if (offs)
      *str += offs;
    str++;
  }
  return temp;
}

//ODSTRLWR(Od_strlwr, OdChar)
OdChar* Test_odStrLwr(OdChar* str)
{
  OdChar* temp = str;
  while (*str)
  {
    OdChar offs = (OdChar)(OdInt16) getOffsetToLower(*str);
    if (offs)
      *str += offs;
    str++;
  }
  return temp;
}

OdString& OdString_makeUpper(OdString& str)
{
  //copyBeforeWrite();
  //odStrUpr(m_pData->unicodeBuffer);
  Test_odStrUpr(str.lockBuffer());
  str.unlockBuffer();
  return str;
}

OdString& OdString_makeLower(OdString& str)
{
  //copyBeforeWrite();
  //odStrLwr(m_pData->unicodeBuffer);
  Test_odStrLwr(str.lockBuffer());
  str.unlockBuffer();

  return str;
}

//////////////////////////////////////////////////////////////////////////

#if defined(Q_OS_WIN) || defined(Q_WS_WIN) // Q_CC_MSVC

struct OffsData
{
public:
  OdUInt16 _chStart,
           _chEnd,
           _offset;
  OffsData(OdUInt16 chStart = 0, OdUInt16 chEnd = 0, OdUInt16 offset = 0)
    : _offset(offset)
    , _chStart(chStart)
    , _chEnd(chEnd)
  {
  }
};

inline void collectData(QList<OffsData>& lst, OdUInt16 ch, OdUInt16 offset)
{
  if (!offset)
    return;
  ODA_ASSERT_ONCE(ch);

  if (   lst.isEmpty() 
      || lst.last()._offset != offset 
      || (lst.last()._chEnd + 1) != ch) // is not the next
    lst << OffsData(ch, ch, offset);
  OffsData& last = lst.last();
  ODA_ASSERT_ONCE(last._chEnd <= ch);
  last._chEnd = ch;
}

inline void generateFuctionCodeByData(OdEdUserIO* pIO, const OdChar* pcszFuncName, QList<OffsData>& lst)
{
  ODA_ASSERT_ONCE(pcszFuncName && *pcszFuncName);

  pIO->putString(OdString().format(OD_T("inline OdUInt16 %ls(const OdChar& sym)"), pcszFuncName));
  pIO->putString(OD_T("{"));
  pIO->putString(OD_T("  if (OdUInt32(sym) >= 0x10000) return 0;"));
  pIO->putString(OD_T("  OdUInt16 chSym = OdUInt16(sym);"));

  //OdUInt16 chStart, OdUInt16 chEnd, OdUInt16 offset)

  pIO->putString(OD_T("  switch ((OdUInt16)(chSym & 0xffff))"));
  pIO->putString(OD_T("  {"));

  QList<OffsData> lstConditional;
  OffsData dataPrev;
  OdString sCase;
  foreach (OffsData data, lst)
  {
    ODA_ASSERT_ONCE(data._offset && data._chEnd >= data._chStart);
    if (data._chEnd > data._chStart)
    {
      lstConditional << data;
      continue;
    }

    if (!dataPrev._offset)
    {
      //dataPrev = data;
      ODA_ASSERT_ONCE(sCase.isEmpty());
    }
    else if (data._offset != dataPrev._offset)
    {
      if (!sCase.isEmpty())
      {
        pIO->putString(sCase);
        sCase.empty();
      }
      pIO->putString(OdString().format(OD_T("    return 0x%lX;"), dataPrev._offset));
    }
  #ifdef _DEBUG
    else
      data._offset = data._offset; // brk
  #endif

    dataPrev = data;
    if (sCase.getLength() > 80)
    {
      pIO->putString(sCase);
      sCase.empty();
    }
    sCase += OdString().format(OD_T("  case 0x%lX:"), data._chStart);
  } // end foreach
  if (dataPrev._offset)
  {
    if (!sCase.isEmpty())
      pIO->putString(sCase);
    pIO->putString(OdString().format(OD_T("    return 0x%lX;"), dataPrev._offset));
  }
  pIO->putString(OD_T("  }"));

  foreach (OffsData data, lstConditional)
  {
    ODA_ASSERT_ONCE(data._offset && data._chEnd >= data._chStart);
    pIO->putString(OdString().format(OD_T("  if (chSym >= 0x%lX && chSym <= 0x%lX) return 0x%lX;"), 
                                     data._chStart, data._chEnd, data._offset));
  }
  pIO->putString(OD_T("  return 0;"));

  pIO->putString(OD_T("}"));
  pIO->putString(OD_T(""));
}

static void putCode_ToLowerAndUpper(OdEdUserIO* pIO)
{
  // only once 
  static bool bFirst = true;
  if (!bFirst)
    return;
  bFirst = false;

  QList<OffsData> lstToUpperData,
                  lstToLowerData;
  
  for (OdUInt16 ch = 1; ch < OdUInt16(-1); ch++)
  {
    OdString sCh;
    OdChar* pCh = sCh.getBuffer(10);
    *pCh++ = OdChar(ch);
    *pCh++ = OdChar(0); *pCh++ = OdChar(0); *pCh = OdChar(0);
    sCh.releaseBuffer();
    ODA_ASSERT_ONCE(sCh.getLength() == 1);

    OdString sConv = sCh;
    sConv.makeUpper();
    OdString sConv_ = getIApp()->toOdString(getIApp()->toQString(OdString(sCh)).toUpper());
    ////sConv = sConv_;
    //ODA_ASSERT_ONCE(sConv_ == sConv);
    //ODA_ASSERT_ONCE(sConv.getLength() == 1);

    OdUInt16 offset = 0;
    if (sConv_ == sConv && sConv.getLength() == 1)
    {
      offset = sConv[0]; 
      offset -= ch;

      ODA_ASSERT_ONCE((OdString_makeUpper((sConv_ = sCh)), sConv_ == sConv));
      ODA_ASSERT_ONCE(!offset || sCh == sConv.makeLower());
    }
    collectData(lstToUpperData, ch, offset);

    sConv = sCh;
    sConv.makeLower();
    sConv_ = getIApp()->toOdString(getIApp()->toQString(OdString(sCh)).toLower());
    ////sConv = sConv_;
    //ODA_ASSERT_ONCE(sConv_ == sConv);
    //ODA_ASSERT_ONCE(sConv.getLength() == 1);

    offset = 0;
    if (sConv_ == sConv && sConv.getLength() == 1)
    {
      offset = sConv[0]; 
      offset -= ch;

      ODA_ASSERT_ONCE((OdString_makeLower((sConv_ = sCh)), sConv_ == sConv));
      ODA_ASSERT_ONCE(!offset || sCh == sConv.makeUpper());
    }
    collectData(lstToLowerData, ch, offset);
  }

  pIO->putString(OD_T("//------------------------------------------------------------------------"));
  pIO->putString(OD_T("// This code was generated by test command of ODADrawingsExplorer (Debug only)"));
  pIO->putString(OD_T("// (see function putCode_ToLowerAndUpper)"));
  pIO->putString(OD_T(""));

  generateFuctionCodeByData(pIO, OD_T("getOffsetToUpper"), lstToUpperData);
  generateFuctionCodeByData(pIO, OD_T("getOffsetToLower"), lstToLowerData);

  pIO->putString(OD_T(""));
  pIO->putString(OD_T("//------------------------------------------------------------------------"));
}

#else
#define putCode_ToLowerAndUpper(pIO) 
#endif

static void putStringAsHex(OdEdUserIO* pIO, const OdString& sText)
{
  pIO->putString(OD_T("as Hex:"));
  QString qsHex = "  unsigned int hex[] = { ";
  for (const OdChar* pChar = sText.c_str(); *pChar; pChar++)
    qsHex += QString("0x%1, ").arg(uint(*pChar) & 0xffff,
                                   (*pChar & 0xff00) ? 4 : 2, 
                                   16, 
                                   QChar('0'));
  qsHex += "0x00 };";
  pIO->putString(getIApp()->toOdString(qsHex));

  // or OdString sValue(OdAnsiString("\\U+30EA\\U+30F3\\U+30AF"));
  qsHex = "  OdString sValue(OdAnsiString(\"";
  for (const OdChar* pChar = sText.c_str(); *pChar; pChar++)
    qsHex += QString("\\\\U+%1").arg(uint(*pChar) & 0xffff, 4, 16, QChar('0'));
  qsHex += "\"));";
  pIO->putString(getIApp()->toOdString(qsHex));
}

static OdString getStringFromHex(const unsigned int* pHex)
{
  OdString sText;
  OdChar* pToStart = sText.getBuffer(512);
  OdChar* pTo = pToStart;
  do 
    *pTo++ = *(const OdChar*) pHex++;
  while (*pHex);
  *pTo = OdChar('\0');
  sText.releaseBuffer();
  return sText;
}

static void checkMakeLowerUpper(OdEdUserIO* pIO, const OdChar* pszComment, const unsigned int* pSrcHex, 
                                const unsigned int* pLowerHex, const unsigned int* pUpperHex = NULL)
{
  OdString sComment(pszComment),
           sTxt = getStringFromHex(pSrcHex);
  // preparing
  //sTxt.makeLower();
  //putStringAsHex(pIO, sTxt);
  //pIO->putString(sTxt);
  // preparing
  //sTxt.makeUpper();
  //putStringAsHex(pIO, sTxt);
  //pIO->putString(sTxt);

  if (pSrcHex)
  {
    sTxt.makeLower();
    //OdString_makeLower(sTxt);

    OdString sOk = getStringFromHex(pLowerHex);

    if (sTxt == sOk)
      pIO->putString(OD_T("  ") + sComment + OD_T("  \"") + sTxt + OD_T("\" == \"") + sOk + OD_T("\" : OK"));
    else
      pIO->putString(OD_T("  ") + sComment + OD_T("  \"") + sTxt + OD_T("\" != \"") + sOk + OD_T("\" : FAILED"));
  }
  if (pUpperHex)
  {
    sTxt.makeUpper();
    //OdString_makeUpper(sTxt);

    OdString sOk = getStringFromHex(pUpperHex);

    if (sTxt == sOk)
      pIO->putString(OD_T("  ") + sComment + OD_T("  \"") + sTxt + OD_T("\" == \"") + sOk + OD_T("\" : OK"));
    else
      pIO->putString(OD_T("  ") + sComment + OD_T("  \"") + sTxt + OD_T("\" != \"") + sOk + OD_T("\" : FAILED"));
  }
}

#if defined(__APPLE__) && defined(ODA_LINKED_WITH_FRAMEWORK_FONDATION) // gcc ... -framework Foundation
#include <CoreFoundation/CoreFoundation.h>
#endif

void OdqCmd_Test::execute(OdEdCommandContext* pCmdCtx)
{
  OdEdUserIO* pIO = pCmdCtx->userIO();

  typedef enum
  {
    en_macLocale
  , en_repairSymbolName
  , en_setPlotStyle
  , en_loadRasterImage
  , en_odFltToF
  , en_intervalArc
  , en_getColor
  , en_toLowerUpper_9860
  , en_odDToStr
  , en_odSScanf
  , en_odStrToD
  , en_7324
  , en_guid

  , en_Number
  };
  QStringList lst;
  lst << "macLocale";
  lst << "repairSymbolName";
  lst << "setPlotStyle"; 
  //lst << "loadRasterImage";
  //lst << "odFltToF";
  //lst << "intervalArc";
  //lst << "getColor";
  //lst << "toLowerUpper";
  //lst << "odDToStr";
  //lst << "odSScanf";
  //lst << "odStrToD";
  //lst << "7324";
  //lst << "guid";
  OdString sPrompt = L"Enter bug number[",
           sKeywords;
  foreach(QString qsKey, lst)
  {
    OdString sKey = getIApp()->toOdString(qsKey);
    if (!sKeywords.isEmpty())
    {
      sKeywords += L" ";
      sPrompt += L"/"; 
    }
    sKeywords += sKey;
    sPrompt += sKey; 
  }
  sPrompt += OdString().format(L"]<%ls>", getIApp()->toOdString(lst.first()).c_str());
  int indexKeyword = pIO->getKeyword(sPrompt, sKeywords, 0);

  if (indexKeyword == en_macLocale)
  {
#if defined(__APPLE__) && defined(ODA_LINKED_WITH_FRAMEWORK_FONDATION) // gcc ... -framework Foundation

    CFArrayRef langs = CFLocaleCopyPreferredLanguages();
    ODA_ASSERT_ONCE(CFArrayGetCount(langs) > 0);
    CFStringRef langCode = static_cast<CFStringRef>(CFArrayGetValueAtIndex(langs, 0));
    // TODO CFRelease((CFTypeRef) ...);
    // next is localized by first language :
    //CFStringRef langName = CFLocaleCopyDisplayNameForPropertyValue(CFLocaleCopyCurrent(), kCFLocaleLanguageCode, langCode); 
    CFIndex length = ::CFStringGetLength(langCode);
    ODA_ASSERT_ONCE(length);
    if (!length)
      return;
    OdString sLangCode;
    // it works if differ to CFStringGetCharactersPtr and CFStringGetCharacters
    for (int idx = 0; idx < length; idx++) 
      sLangCode += (OdChar) CFStringGetCharacterAtIndex(langCode, idx);
    sLangCode.makeLower();
    pIO->putString(OdString().format(L"  sLangCode = \"%ls\"\n", sLangCode.c_str()).c_str());
    // "en" ...
#elif defined(__APPLE__)
    pIO->putString(L"  ODA_LINKED_WITH_FRAMEWORK_FONDATION should be uncommented in OdaQtConsole.pri\n");
#endif
    return;
  }
  //if (indexKeyword == ...)
  //{
  //  char japFaceName[] = {0x82, 0x6c,   // This is font face name in 932 code page (MBCS)
  //    0x82, 0x72,
  //    0x20,
  //    0x83, 0x53,
  //    0x83, 0x56,
  //    0x83, 0x62,
  //    0x83, 0x4e,
  //    0};
  //  OdString strJapFaceName(japFaceName, CP_ANSI_932);
  //  int nLen = strJapFaceName.getLength();
  //  // nLen should 7.
  //  // Here strJapFaceName already has Unicode buffer.
  //  // Its contents should be the same as OdChar array below.
  //  OdChar cFaceName[] = {  0xff2d, 0xff33, 0x0020, 0x30b4, 0x30b7, 0x30c3, 0x30af, 0};
  //  if (strJapFaceName != cFaceName)
  //  {
  //    int N = 1; // Should not get here.
  //  }
  //  OdString fontName(cFaceName);
  //  OdString fontKey;
  //  // Does this call work? Buffer is as expected? (2 japanese characters, space, 4 japanese characters, "00128049")
  //  fontKey.format(OD_T("%ls%ls%ls%03d%03d") 
  //    , fontName.c_str()
  //    , OD_T("0")
  //    , OD_T("0")
  //    , 128
  //    , 49
  //    );
  //  pIO->putString(OdString().format(L"\n  res: %ls\n", fontKey.c_str()).c_str());
  //  // Does this call work? Buffer is as expected? (2 japanese characters, space, 4 japanese characters, "00128049")
  //  fontKey.format(OD_T("%ls%ls%ls%03d%03d") 
  //    , strJapFaceName.c_str()
  //    , OD_T("0")
  //    , OD_T("0")
  //    , 128
  //    , 49
  //    );
  //  pIO->putString(OdString().format(L"\n  res: %ls\n", fontKey.c_str()).c_str());
  //  return;
  //}

  if (indexKeyword == en_repairSymbolName)
  {
    const char* pTst[] = {
      "\\U+0054\\U+0065\\U+0078\\U+0074\\U+0053\\U+0074\\U+0079\\U+006c\\U+0065\\U+002d\\U+2172"
    , "0*1 2.3(4)5[6]7+8&9"
    , "\\U+30EA*\\U+30F3\\U+30AF"
    , ""
    };
    for (int index = 0; pTst[index] && *pTst[index]; index++)
    {
      OdAnsiString aName(pTst[index]);
      OdString sName(aName);
      sName.getLength(); // force

      OdString sValidName;
      if (OdDbSymUtil::repairSymbolName(sValidName, sName, 
                                        OdDbDatabase::cast(pCmdCtx->baseDatabase()),
                                        false, L' ', false) == eOk)
      {
        if (sValidName.isEmpty())
          sValidName = sName;
      }
      else
      {
        ODA_FAIL_ONCE();
      }
      ODA_ASSERT_ONCE(!sValidName.isEmpty());
    }
    return;
  }

  if (indexKeyword == en_setPlotStyle)
  {
    OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
    OdDbDatabasePtr pDb = pDbCmdCtx->database();
    OdSmartPtr<OdDbUserIO> pDbIO = pDbCmdCtx->userIO();

    OdDbObjectId spaceId = pDb->getActiveLayoutBTRId();
    if(spaceId == pDb->getPaperSpaceId())
    {
      OdDbViewportPtr pVp = pDb->activeViewportId().safeOpenObject();
      if (pVp->number()>1)
        spaceId = pDb->getModelSpaceId();
    }
    OdDbBlockTableRecordPtr pSpace = spaceId.safeOpenObject(OdDb::kForWrite);
    OdDbLinePtr pNewLine = OdDbLine::createObject();
    pNewLine->setDatabaseDefaults(pDb);
    pNewLine->setStartPoint(pDbIO->getPoint(OD_T("Specify first point:")));
    pNewLine->setEndPoint(pDbIO->getPoint(OD_T("Specify next point:"), OdEd::kGptRubberBand));
    pSpace->appendOdDbEntity(pNewLine);

    OdDbEntityPtr pEnt = OdDbEntity::cast(pNewLine);
    OdResult res = pEnt->setPlotStyleName(L"Oda_Test");

    pIO->putString(OdString().format(L"\n res: %d\n", res).c_str());
    return;
  }

  if (indexKeyword == en_loadRasterImage)
  {
    OdRxRasterServicesPtr pRasSvcs = odrxDynamicLinker()->loadApp(RX_RASTER_SERVICES_APPNAME);
    ODA_ASSERT_ONCE(pRasSvcs.get());
    if (!pRasSvcs.get())
      return;

    static OdString s_sInPrompt = OD_T("Enter file name to load BMP:"),
                    s_sInDialogCaption = OD_T("Load BMP"), 
                    s_sInImage,
                    s_sInDefExt = OD_T("bmp"),
                    s_sInFilters,
                    s_sOutPrompt = OD_T("Enter file name to save BMP:"),
                    s_sOutDialogCaption = OD_T("Save BMP"), 
                    s_sOutImage,
                    s_sOutDefExt = OD_T("bmp"),
                    s_sOutFilters;
    if (s_sInFilters.isEmpty())
    {
      OdUInt32Array types = pRasSvcs->getRasterImageTypes();
      if (types.size())
      {
        for (unsigned index = 0; index < types.size(); index++)
        {
          OdUInt32 tp = types.getAt(index);
          OdString sFilterName,
            sExt = pRasSvcs->mapTypeToExtension(tp, &sFilterName);
          if (sExt.isEmpty())
            continue;
          sExt.makeLower();
          ODA_ASSERT_ONCE(!sExt.find(OD_T(".")));
          if (sFilterName.isEmpty())
          {
            sFilterName = sExt;
            sFilterName.makeUpper().remove(__OD_T('.'));
            sFilterName += OD_T(" (*") + sExt + OD_T(")");
          }

          s_sInFilters += sFilterName + OD_T("|*") + sExt + OD_T("|");
          s_sOutFilters += sFilterName + OD_T("|*") + sExt + OD_T("|");
        }

        s_sInPrompt.replace(OD_T("BMP"), OD_T("image"));
        s_sInDialogCaption.replace(OD_T("BMP"), OD_T("image"));
        s_sOutPrompt.replace(OD_T("BMP"), OD_T("image"));
        s_sOutDialogCaption.replace(OD_T("BMP"), OD_T("image"));
      }
      if (s_sInFilters.isEmpty())
      {
        s_sInFilters = OD_T("Windows Bitmap Files (*.bmp)|*.bmp|");
        s_sOutFilters = OD_T("Windows Bitmap Files (*.bmp)|*.bmp|");
      }
      else
        s_sInFilters += OD_T("All Files (*.*)|*.*|");
      s_sInFilters += OD_T("|");
      s_sOutFilters += OD_T("|");
    }

    s_sInImage = pIO->getFilePath(s_sInPrompt, OdEd::kGfpForOpen, s_sInDialogCaption,
                                  s_sInDefExt, s_sInImage, s_sInFilters);
    if (s_sInImage.isEmpty())
      return; // canceled

    OdGiRasterImagePtr pImage = pRasSvcs->loadRasterImage(s_sInImage);
    if (pImage.isNull())
    {
      pIO->putString(OD_T("  Error: Unsupported/Invalid format of input image"));
      return;
    }

    s_sOutImage = pIO->getFilePath(s_sOutPrompt, OdEd::kGfpForSave, s_sOutDialogCaption,
                                   s_sOutDefExt, s_sOutImage, s_sOutFilters);
    if (s_sOutImage.isEmpty())
      return; // canceled

    pRasSvcs->saveRasterImage(pImage, s_sOutImage);
    return;
  }

  if (indexKeyword == en_odFltToF)
  {
    // 0.235 is shown in debugger as 0.234(9)
    //OdString odFltToF(double val, int prec)
    //#include "OdDToStr.h"

    OdString sVal = odDToStr(0.2349999999999999, 'f', 3);
    ODA_ASSERT_ONCE(sVal == OD_T("0.235"));
    return;
  }

  if (indexKeyword == en_intervalArc) // BR Merge
  {
    OdGeEllipArc3d arc;
    double l_ = arc.startAng(), u_ = arc.endAng();
    OdGeInterval interval;
    arc.getInterval(interval);
    double l = interval.lowerBound(), u = interval.upperBound();
    ODA_ASSERT_ONCE(l == l_ && u ==u_);
    return;
  }

  if (indexKeyword == en_getColor)
  {
    OdCmColor color;
    color.setRGB(255, 0, 0);
    color = OdCmColor(*pIO->getCmColor(L"enter color", OdEd::kInpDefault, &color).get());

    OdDbUserIO* pDbIO = OdDbUserIO::cast(pCmdCtx->userIO()).get();
    ODA_ASSERT_ONCE(pDbIO);
    color = pDbIO->getColor(L"enter color", OdEd::kInpDefault, &color);
    return;
  }

  if (indexKeyword == en_toLowerUpper_9860)
  {
    // #9860
    // preparing
    //OdString sTst = pIO->getString(OD_T("Enter text"), OdEd::kGstAllowSpaces);
    //putStringAsHex(pIO, sTst);
    //pIO->putString(sTst);
    //

    OdUInt32 offs = (OdUInt32)(OdInt16) 0xfffe;
    OdUInt32 ch = 2;
    ch += offs;
    ODA_ASSERT_ONCE(!ch);

    putCode_ToLowerAndUpper(pIO); 

    unsigned int hex0[] = { 0x41, 0x5a, 0x61, 0x7a, 0x20, 0x4a, 0x6f, 0x68, 0x6e, 0x20, 0x44, 0x6f, 0x65, 0x00 },
                 hexLow0[] = { 0x61, 0x7a, 0x61, 0x7a, 0x20, 0x6a, 0x6f, 0x68, 0x6e, 0x20, 0x64, 0x6f, 0x65, 0x00 },
                 hexUp0[] = { 0x41, 0x5a, 0x41, 0x5a, 0x20, 0x4a, 0x4f, 0x48, 0x4e, 0x20, 0x44, 0x4f, 0x45, 0x00 };
    checkMakeLowerUpper(pIO, OD_T("en:"), hex0, hexLow0, hexUp0);
    unsigned int hex1[] = { 0x0412, 0x0430, 0x0441, 0x044f, 0x20, 0x041f, 0x0443, 0x043f, 0x043a, 0x0438, 0x043d, 0x00 },
                 hexLow1[] = { 0x0432, 0x0430, 0x0441, 0x044f, 0x20, 0x043f, 0x0443, 0x043f, 0x043a, 0x0438, 0x043d, 0x00 },
                 hexUp1[] = { 0x0412, 0x0410, 0x0421, 0x042f, 0x20, 0x041f, 0x0423, 0x041f, 0x041a, 0x0418, 0x041d, 0x00 };
    checkMakeLowerUpper(pIO, OD_T("rus:"), hex1, hexLow1, hexUp1);
    unsigned int hex2[] = { 0xdc, 0xfc, 0xd6, 0xf6, 0x00 },
                 hexLow2[] = { 0xfc, 0xfc, 0xf6, 0xf6, 0x00 },
                 hexUp2[] = { 0xdc, 0xdc, 0xd6, 0xd6, 0x00 };
    checkMakeLowerUpper(pIO, OD_T("de:"), hex2, hexLow2, hexUp2);
    unsigned int hex3[] = { 0x30ea, 0x30f3, 0x30af, 0x00 };
    checkMakeLowerUpper(pIO, OD_T("jap:"), hex3, hex3, hex3);

    return;
  }

  if (indexKeyword == en_odDToStr)
  {
    char buf[100];
    // linux with gcc4.4 and -O3 :
    //*** odDToStr(BFD77E8C, 2043.16, G, 16, 1);
    //    ==> segmentation error
    odDToStr(buf, 2043.16, 'g', 16, 1);
    if (OdString(buf) == OD_T("2043.16"))
      pIO->putString(OD_T("  2043.16 : OK"));
    else
      pIO->putString(OD_T("  2043.16 != \"") + OdString(buf) + OD_T("\" : Wrong"));
    // linux with gcc4.4 and -O3 :
    //*** odDToStr(BFE61C24, 1e-06, g, 17, 0)
    //*** ==> *** odDToStr(...) = "9.9999999999999995e-007"
    //*** odDToStr(BFE61C24, 1e-10, g, 17, 0)
    //*** ==> *** odDToStr(...) = ":.0000000000000004e-011"
    //*** odDToStr(BFE61D24, 18.308, g, 17, 0)
    //*** ==> *** odDToStr(...) = "000018.30799484253"
    //*** odDToStr(BFE61B64, 1804.46, g, 17, 0)
    //    ==> segmentation error
    odDToStr(buf, 1804.46, 'g', 17, 1);
    if (OdString(buf) == OD_T("1804.46"))
      pIO->putString(OD_T("  1804.46 : OK"));
    else
      pIO->putString(OD_T("  1804.46 != \"") + OdString(buf) + OD_T("\" : Wrong"));
    return;
  }

  if (indexKeyword == en_odSScanf)
  {
    // check problem with odSScanf on Mac vos diesel implementation
    // see: return odSScanf(earg.c_str(), L"%i", intres) == 1;
    int iVal0 = 0,
        iVal1 = 0;
    odSScanf(__OD_T("0"), __OD_T("%i"), &iVal0);
    odSScanf(__OD_T("1"), __OD_T("%i"), &iVal1);
    if (iVal0 == 0 && iVal1 == 1)
      pIO->putString(OD_T("  int - OK"));
    else
      pIO->putString(OD_T("  int - Wrong"));

    double dVal = 0.0;
    odSScanf(__OD_T("1.1"), __OD_T("%lf"), &dVal);
    if (fabs(dVal - 1.1) < 1e-7)
      pIO->putString(OD_T("  double - OK"));
    else
      pIO->putString(OD_T("  double - Wrong"));

    tst_odSScanf(__OD_T("0"), __OD_T("%i"), &iVal0);
    tst_odSScanf(__OD_T("1"), __OD_T("%i"), &iVal1);
    tst_odSScanf(__OD_T("1.1"), __OD_T("%lf"), &dVal);
    if (iVal0 == 0 && iVal1 == 1 && fabs(dVal - 1.1) < 1e-7)
      pIO->putString(OD_T("  OK"));
    else
      pIO->putString(OD_T("  Wrong"));

   #if defined(_MSC_VER)
    setlocale( LC_ALL, "Russian" ); // setlocale( LC_ALL, "" );

    double dVal1 = 0.0,
           dVal2 = 0.0;
    odSScanf(L"1,123", L"%lf", &dVal1);
    dVal2 = odStrToD(L"1.123"); //odSScanf(L"1.123", L"%lf", &dVal2);
    if (fabs(dVal1 - 1.123) < 1e-7 && fabs(dVal2 - 1.123) < 1e-7)
      pIO->putString(OD_T("  localed double - OK"));
    else
      pIO->putString(OD_T("  localed double - Wrong"));

    OdString sCoord;
    ////sCoord.format(OD_T("%lg,%lg,%lg"), 1.1, 2.2, 3.3);
    //sCoord = getIApp()->toOdString(QString("%1,%2,%3").arg(1.1).arg(2.2).arg(3.3));
    sCoord.format(L"%lg;%lg;%lg", 1.1, 2.2, 3.3);
    sCoord.replace(L',', L'.');
    sCoord.replace(L';', L',');

    if (sCoord == L"1.1,2.2,3.3")
      pIO->putString(OD_T("  localed double format - OK"));
    else
      pIO->putString(OD_T("  localed double format - Wrong"));

    setlocale(LC_NUMERIC, "C"); // restore
    setlocale(LC_TIME, ""); 
   #endif

    return;
  }

  if (indexKeyword = en_odStrToD)
  {
    OdString str1(OD_T("0.5"));
    double d1 = odStrToD(str1);
    if ( OdNonZero(0.5 - d1) )
    {
      // see near YES_ALIAS
      pIO->putString(OD_T("  Wrong 0.5"));
      pIO->putString(getIApp()->toOdString(QString("%1").arg(d1)));
    } else
    {
      pIO->putString(OD_T("  OK 0.5"));
    }
    OdString str2(OD_T("1.8"));
    double d2 = odStrToD(str2);
    if ( OdNonZero(1.8 - d2) )
    {
      pIO->putString(OD_T("  Wrong 1.8"));
      pIO->putString(getIApp()->toOdString(QString("%1").arg(d2)));
    } else
    {
      pIO->putString(OD_T("  OK 1.8"));
    }

    return;
  }

  if (indexKeyword == en_7324)
  {
    //#7324

    OdDbDatabasePtr pDwgDb = OdDbDatabase::cast(pCmdCtx->baseDatabase());
    if (pDwgDb.isNull())
    {
      //throw OdError(eNoDatabase); // there is no audit functionality for DGN

      OdAnsiString aStr = "\\U+30EA\\U+30F3\\U+30AF";
      OdString sName(aStr);
      sName.getLength(); // force

      pIO->putString(OD_T("in:"));
      pIO->putString(sName);

      OdString sName1;
      sName1.format(OD_T("%ls$7324$"), sName.c_str());
      pIO->putString(OD_T("out:"));
      pIO->putString(sName1);

      bool bOk = (sName1 == sName + OD_T("$7324$"));
      if (bOk)
        pIO->putString(OD_T("OdString::format - OK"));
      else
        pIO->putString(OD_T("OdString::format - FAILED"));

      //sName1 = sName + OD_T("$7324$");
      //int nVal = 0;
      //odSScanf(sName1.c_str(), (sName + OD_T("$%d$")).c_str(), &nVal);
      //bOk = (nVal == 7324);
      //if (bOk)
      //  pIO->putString(OD_T("odSScanf (value) - OK"));
      //else
      //  pIO->putString(OD_T("odSScanf (value) - FAILED"));

      OdChar ch1 = __OD_T('?'), ch2 = __OD_T('?'), ch3 = __OD_T('?');
      int nVal = 0;
      OdString sTst;

      odSScanf((wchar_t*)sName1.c_str(), __OD_T("%lc%lc%lc$%d$"), &ch1, &ch2, &ch3, &nVal);
      sTst += ch1;
      sTst += ch2;
      sTst += ch3;
      bOk = (sName == sTst && nVal == 7324);
      pIO->putString(sTst);
      if (bOk)
        pIO->putString(OD_T("odSScanf (FORCE) - OK"));
      else
        pIO->putString(OD_T("odSScanf (FORCE)  - FAILED"));

      sTst.empty();
      sTst.format(OD_T("%lc%lc%lc"), ch1, ch2, ch3);
      pIO->putString(sTst);
      bOk = (sName == sTst);
      if (bOk)
        pIO->putString(OD_T("OdString::format (FORCE) - OK"));
      else
        pIO->putString(OD_T("OdString::format (FORCE) - FAILED"));

      ch1 = __OD_T('?');
      ch2 = __OD_T('?');
      ch3 = __OD_T('?');
      nVal = 0;
      sTst.empty();
      odSScanf((const wchar_t*)sName1.c_str(), __OD_T("%c%c%c$%d$"), &ch1, &ch2, &ch3, &nVal);
      sTst += ch1;
      sTst += ch2;
      sTst += ch3;
      bOk = (sName == sTst && nVal == 7324);
      pIO->putString(sTst);
      if (bOk)
        pIO->putString(OD_T("odSScanf (without FORCE) - OK"));
      else
        pIO->putString(OD_T("odSScanf (without FORCE) - FAILED"));

      
      sTst.empty();
      // there is no solution now (works via vswprintf) // sTst.format(OD_T("%c%c%c"), ch1, ch2, ch3);
      sTst.format(OD_T("%c%c%c%ls"), ch1, ch2, ch3, sName.c_str()); // %ls is used to force via Od_vswprintfV
      pIO->putString(sTst);
      bOk = ((sName + sName) == sTst);
      if (bOk)
        pIO->putString(OD_T("OdString::format (without FORCE) - OK"));
      else
        pIO->putString(OD_T("OdString::format (without FORCE) - FAILED"));

      return;
    }

    OdDbBlockTableRecordPtr pBlock;
    OdDbObjectId idObj = pDwgDb->getOdDbObjectId(OdDbHandle(OD_T("72")));
    if (!idObj.isNull())
      pBlock = OdDbBlockTableRecord::cast(idObj.openObject(OdDb::kForWrite));
    if (pBlock.isNull())
    {
      pIO->putString(OD_T("  Error: BlockTableRecord with handle 72 was not found"));
      return;
    }
    if (!(pBlock->isFromExternalReference() && pBlock->isResolved()))
    {
      pIO->putString(OD_T("  Error: BlockTableRecord is invalid to bind"));
      return;
    }

    OdString sName = pBlock->getName();
    pIO->putString(OD_T("in:"));
    pIO->putString(sName);
    putStringAsHex(pIO, sName);
    sName.empty();

    //unsigned int hex[] = { 0xea, 0x30, 0xf3, 0x30, 0xaf, 0x30, 0x00, 0x00 };
    //sName = getStringFromHex(hex);

    //pIO->putString(OD_T("in:"));
    //pIO->putString(sName);

    //OdString sName1;
    //sName1.format(OD_T("%ls$0$"), sName.c_str());
    //pIO->putString(OD_T("out:"));
    //pIO->putString(sName1);

    OdDbXRefMan::bind(pBlock);
    pIO->putString(OD_T("  Ok"));

    return;
  }

  if (indexKeyword == en_guid)
  {
    //reform guid by string (version + app name)

    //{1FF14C63-6F95-4CD7-8DD3-7D793595BD5D} {95612628-EEAF-4BDC-8927-3E8BE2928CDE} {B5BBBAF9-E33B-494D-8F8B-BFB8B220498B}
    //{A26F7544-4BA1-43AA-82AD-6D1B3E57B0DA} {A4B1F541-7C7F-4C66-AD48-AA76149B31D3} {F9BF7A01-6E7E-4C9C-B63B-4FD583994D02}
    //
    // {67C8770B-44F1-410A-AB9A-F9B5446F13EE}
    //QUuid IID_MyInterface(0x67c8770b, 0x44f1, 0x410a, 0xab, 0x9a, 0xf9, 0xb5, 0x44, 0x6f, 0x13, 0xee)

    pIO->putString(OD_T("input guid: {1FF14C63-6F95-4CD7-8DD3-7D793595BD5D}"));
    QString qsBy = "3.4.0+ODADrawingsExplorer";
    
    QString qsMsg = QString("reform by \"%1\"").arg(qsBy);
    pIO->putString(getIApp()->toOdString(qsMsg));

    QUuid iid("{1FF14C63-6F95-4CD7-8DD3-7D793595BD5D}");

    // via data4[0]
    if (iid.variant() != QUuid::DCE) // strange but it DCE by Qt // QUuid::Microsoft)
    {
      ODA_FAIL_ONCE(); 
    }
    // via data3
    if (iid.version() != QUuid::Random)
    {
      ODA_FAIL_ONCE(); 
    }

    //uint    data1;
    //ushort  data2;
    //ushort  data3;
    //uchar   data4[8];

    //QUuid result;
    //uint *data = &(result.data1);
    //int chunks = 16 / sizeof(uint);
    //while (chunks--) {
    //  uint randNumber = 0;
    //  for (int filled = 0; filled < intbits; filled += randbits)
    //    randNumber |= qrand()<<filled;
    //  *(data+chunks) = randNumber;
    //}

    uchar* pData = &iid.data4[1];
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
    const uchar* pBy = (const uchar*) qsBy.toAscii().data();
#else
    const uchar* pBy = (const uchar*) qsBy.toLatin1().data();
#endif
    int index = 0;
    while (*pBy)
    {
      pData[index] += *pBy++;
      if (++index >= 7)
        index = 0;
    }

    qsMsg = "result=";
    qsMsg += iid.toString().toUpper();
    pIO->putString(getIApp()->toOdString(qsMsg));

    return;
  }

  pIO->putString(OD_T("  TODO"));
  return;
}

#endif
#if defined(_TST_COMMAND) || defined(_DEBUG)

static void OdqCmd_Sup827(OdEdCommandContext* pCmdCtx)
{
  OdString sLevelName = pCmdCtx->userIO()->getString(L"Enter level name",  OdEd::kGstAllowSpaces, L"Default");
  if (sLevelName.isEmpty())
    return;

  OdRxObject* pRxDatabase = pCmdCtx->baseDatabase();
  for (OdRxIteratorPtr pIter = OdDbBaseDatabasePEPtr(pRxDatabase)->layers(pRxDatabase); 
       !pIter->done(); pIter->next())
  {
    OdRxObjectPtr pLayerObject = pIter->object();
    OdDbBaseLayerPEPtr pLayerPE = OdDbBaseLayerPE::cast(pLayerObject);
    if (pLayerPE.isNull() || pLayerPE->name(pLayerObject) != sLevelName)
      continue;

    bool isOff = pLayerPE->isOff(pLayerObject);
    pLayerPE->setIsOff(pLayerObject, !isOff);
    pCmdCtx->userIO()->putString(L"Visibility was inverted.\n");

    foreach (IqView* iView, getIViewSystem()->getOpenViews(pRxDatabase))
    {
      OdGsModel* pGsModel = iView->getGsModel();
      if (pGsModel)
        pGsModel->invalidate(OdGsModel::kInvalidateAll); // hard refresh
    }

    return;
  }
}

void OdqCmd_Tst::execute(OdEdCommandContext* pCmdCtx)
{
  OdRxObject* pRxDb = pCmdCtx->baseDatabase();
  OdString sFilePathName = OdDbBaseDatabasePE::cast(pRxDb)->getFilename(pRxDb);
  if (sFilePathName.find(L"seed3d.dgn") >= 0)
    OdqCmd_Sup827(pCmdCtx);
}

#endif

