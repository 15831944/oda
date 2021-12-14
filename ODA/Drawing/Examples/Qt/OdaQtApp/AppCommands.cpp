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
// AppCommands.cpp
//

#include "OdqMainWindow.h"
#include "AppModule.h"
#include "Ed/EdCommandContext.h"
#include "Ed/EdUserIO.h"
#include "DbBaseHostAppServices.h"
#include "DynamicLinker.h"
#include "DbDatabase.h"
#include "LyLayerFilter.h"
#include "DbSymUtl.h"
#include "DbLayerTable.h"
#include "DbLayerTableRecord.h" 
#include "DbViewport.h"
#include "DbLinetypeTable.h"
#include "DbLinetypeTableRecord.h"
#include "Gs/GsModel.h"

#include "OdqApplication.h" 
#include "OdqCustomization.h"
#include "OdqPropertyDialog.h"
#include "OdqAppProps.h"
#include "Services.h"
#include "ExtDbModule.h"

#include <QFileInfo>
#include <QLabel>
#include <QLayout>
#include <QScrollArea>
#include <QGroupBox>
#include <QRegExp>
#include <QUrl>

#ifndef ODA_QT_NO_QZIP
#if QT_VERSION > 0x040603
#include "qzipreader_p.h" // use for menu command
#else
#include "private/qzipreader_p.h" // use for menu command
#endif
#endif

// as loading cui-file (Command User Interface - part of zipped cuix file of acad)

void OdqCmd_Menu::execute(OdEdCommandContext* pCmdCtx)
{
  OdString sPath;
  QString qsPath;

  if (getIConsole()->isInputStringReady(pCmdCtx))
  {
    sPath = pCmdCtx->userIO()->getString(OD_T("Enter CUI file name"));
    if (sPath.isEmpty())
      return; // FIX_MAC_APP_TROW(OdEdCancel()); // throw OdEdCancel();
    // QFile does not work with "qrc:/..." prefix (should be ":/...")
    if (!sPath.find(OD_T("qrc:/")))
      sPath = sPath.mid(3);

    if (sPath == L"...")
      sPath.empty();
    else
    {
      qsPath = getIApp()->toQString(sPath);
      if (!QFileInfo(qsPath).exists())
      {
        OdString sMsg = getIApp()->toOdString(
          QObject::tr("CUI file \"%1\" was not found").arg(qsPath));
        getIApp()->getBaseHostAppServices()->warning(sMsg);
        return;
      }
    }
  }

  static OdString s_sExtension;
  if (sPath.isEmpty())
  {
    static OdString s_sFilter;
    if (s_sFilter.isEmpty())
    {
      // fill filters

      s_sFilter += OD_T("Customized User Information (*.cui)|*.cui|");
      s_sExtension = OD_T("cui");
     #if defined(_DEBUG) && !defined(ODA_QT_NO_QZIP)
      // TODO
      s_sFilter += OD_T("Partial Customized User Information (*.cuix)|*.cuix|");
      s_sExtension = OD_T("cuix");
     #endif
      s_sFilter += OD_T("|");
    }

    sPath = pCmdCtx->userIO()->getFilePath(OD_T("Enter CUI file name"), 
                                           OdEd::kGfpForOpen, 
                                           OD_T("Select Customization File"), // dialog caption 
                                           s_sExtension, // default extension
                                           OdString::kEmpty, // file name
                                           s_sFilter);
    if (sPath.isEmpty())
      return; // without message "Cancel by user" // FIX_MAC_APP_TROW(OdEdCancel()); // throw OdEdCancel();

    qsPath = getIApp()->toQString(sPath);
  }

  QFileInfo info(qsPath);
  if (!info.suffix().isEmpty())
    s_sExtension = getIApp()->toOdString(info.suffix().toLower());
  else
    ODA_FAIL_ONCE(); // suspicion to invalid file

  if (s_sExtension == "cui")
  {
    OdqApplication* pApp = qobject_cast<OdqApplication*>(qApp);
    QString qsLoadedCustomGroup = pApp->getCustom()->loadCustomGroup(qsPath);
    if (!qsLoadedCustomGroup.isEmpty())
      pApp->getCustom()->refreshCustomGroupWidgets(true); // now menu only
    return;
  }

  if (s_sExtension == "cuix")
  {
    ODA_FAIL_ONCE(); // TODO
    FIX_MAC_APP_TROW(OdError(eNotImplemented)); // throw OdError(eNotImplemented);
    #ifndef ODA_QT_NO_QZIP
      QZipReader unzip(qsPath, QIODevice::ReadOnly);
      QList<QZipReader::FileInfo> lst = unzip.fileInfoList();
      if (!lst.size() || !unzip.exists())
        return;
      foreach (QZipReader::FileInfo fi, lst)
      {
        QFile file(fi.filePath);
        if( file.open(QFile::WriteOnly) )
        {
          ////QApplication::setOverrideCursor(Qt::WaitCursor);
          //file.write(unzip.fileData(fi.filePath), unzip.fileData(fi.filePath).size());
          ////QApplication::restoreOverrideCursor();
          file.close();
        }
      }
      unzip.close();
    //#else
    //  // linnex with KDE way (out of Qt SDK)
    //  //return;
    //
    //  ////The following example runs gzip to compress the string "Qt rocks!", without an event loop: 
    //  //QProcess gzip;
    //  //gzip.start("gzip", QStringList() << "-c");
    //  //if (!gzip.waitForStarted())
    //  //  return false;
    //  //gzip.write("Qt rocks!");
    //  //gzip.closeWriteChannel();
    //  //if (!gzip.waitForFinished())
    //  //   return false;
    //
    //  //QByteArray result = gzip.readAll(); 
    #endif
  }

  ODA_FAIL_ONCE(); // TODO ?
  FIX_MAC_APP_TROW(OdError(eNotImplemented)); // throw OdError(eNotImplemented);
}

void OdqCmd_CuiLoad::execute(OdEdCommandContext* pCmdCtx)
{
  OdqMainWindow* pMainWindow = qobject_cast<OdqMainWindow*>(getIApp()->getMainWindow());
  ODA_ASSERT_ONCE(pMainWindow);

  OdqAppModule* pModule = static_cast<OdqAppModule*>(pMainWindow->getAppModule());
  ODA_ASSERT_ONCE(pModule);
      
  pModule->m_cmd_Menu.execute(pCmdCtx);
}

void OdqCmd_CuiUnload::execute(OdEdCommandContext* pCmdCtx)
{
  OdqApplication* pApp = qobject_cast<OdqApplication*>(qApp);
  
  QStringList lstGroups = pApp->getCustom()->getCustomGroups();
  OdString sMsg;
  if (!lstGroups.size())
  {
    sMsg = OD_T("There is no loaded custom group data");
    pApp->getBaseHostAppServices()->warning(sMsg);
    return;
  }

  OdString sPrompt = OD_T("Enter custom group name to unload ["),
           sKeywords;
  foreach (QString qsGroup, lstGroups)
  {
    OdString sGroup = pApp->toOdString(qsGroup);
    if (!sKeywords.isEmpty())
    {
      sPrompt += OD_T("/");
      sKeywords += OD_T(" ");
    }
    sPrompt += sGroup;
    sKeywords += sGroup;
  }
  sPrompt += OD_T("]");
  
  OdString sValue;
  try
  {
    sValue = pCmdCtx->userIO()->getString(sPrompt, OdEd::kInpDefault, 
                                          OdString::kEmpty, // defValue 
                                          sKeywords);
  }
  catch (OdEdKeyword& key)
  {
    sValue = key.keyword();
  }
  if (sValue.isEmpty())
    return;

  QRegExp rx(pApp->toQString(sValue), Qt::CaseInsensitive, QRegExp::Wildcard);
  QString qsRes;
  foreach (QString qsGroup, lstGroups)
  {
    if (!rx.exactMatch(qsGroup))
      continue;

    if (pApp->getCustom()->unloadCustomGroup(qsGroup))
      qsRes += qsGroup;
  }

  if (qsRes.isEmpty())
  {
    sMsg = OD_T("There is no matched custom group to unload");
    pApp->getBaseHostAppServices()->warning(sMsg);
    return;
  }

  sMsg = pApp->toOdString(QObject::tr(qsRes.contains(" ") 
                                        ? "Custom groups %1 were unloaded"
                                        : "Custom group %1 was unloaded")
                                      .arg(qsRes));
  pCmdCtx->userIO()->putString(sMsg);
  pApp->getCustom()->refreshCustomGroupWidgets();
}

///////////////////////////////////////////////////////////////////////////////

class odqLayerPropertyManager
{
private:
  odqLayerPropertyManager()
    : m_pRxDatabase(NULL)
    , m_bInvertFilter(false)
    , m_bRefreshFilters(false)
    , m_bRefreshLayers(false)
  {
  }
public:
  static OdSharedPtr<odqLayerPropertyManager> create(OdRxObject* pRxDatabase);
  virtual ~odqLayerPropertyManager()
  {
  }

  OdqPropertyModel* getModelFilters()
  {
    // DGN way //ODA_ASSERT_ONCE(!m_pModelFilters.isNull());
    return m_pModelFilters.get();
  }

  OdqPropertyModel* getModelLayers()
  {
    ODA_ASSERT_ONCE(!m_pModelLayers.isNull()); // test
    return m_pModelLayers.get();
  }

  static bool updateXml(void* pSrc);
  static bool setPropertyValue(void* pSrc, const QString& qsPath, const QString& qsValue);
  static bool getPropertyPossibleValues(void* pSrc, const QString& qsPath, QStringList& values, QStringList& icons);

private:
  static QString getFiltersAsXml(const OdLyLayerFilter* pRoot, const OdLyLayerFilter* pCurrent, 
                                 const QString& cqsSubPath);
  static OdLyLayerFilterPtr getFilterByPath(const OdLyLayerFilterPtr pRoot, const QString& qsPath);
  static OdRxObjectPtr getLayerObjectByName(OdRxObject* pRxDatabase, const QString& qsLayerName);

  OdRxObject* m_pRxDatabase;
  OdSharedPtr<OdqPropertyModel> m_pModelFilters;
  bool m_bInvertFilter;
  OdSharedPtr<OdqPropertyModel> m_pModelLayers;
  // force refilling of widget (change number of columns or rows in grid or item in tree)
  bool m_bRefreshFilters,
       m_bRefreshLayers;

  Q_DISABLE_COPY(odqLayerPropertyManager)
};

//static 
OdSharedPtr<odqLayerPropertyManager> odqLayerPropertyManager::create(OdRxObject* pRxDatabase)
{
  if (!pRxDatabase)
    FIX_MAC_APP_TROW(OdError(eNoDatabase)); // throw OdError(eNoDatabase);

  OdSharedPtr<odqLayerPropertyManager> pInstance = new odqLayerPropertyManager();
  pInstance->m_pRxDatabase = pRxDatabase;

  if (!OdDbDatabase::cast(pRxDatabase).isNull())
  {
    pInstance->m_pModelFilters = OdqPropertyModel::createModel();
    pInstance->m_pModelFilters->setPropertySource(pInstance);
    pInstance->m_pModelFilters->setFun_updateXml(odqLayerPropertyManager::updateXml);
    pInstance->m_pModelFilters->setFun_setPropertyValue(odqLayerPropertyManager::setPropertyValue);
    pInstance->m_pModelFilters->setFun_getPropertyPossibleValues(odqLayerPropertyManager::getPropertyPossibleValues);
  }
  else
  {
    // TODO for DGN
    // create OdDbBaseLayerFilterPE like OdDbBaseLayerPE
    // and use it intead OdLyLayerFilterManager (= ::odlyGetLayerFilterManager(pDb)) and OdLyLayerFilter 
  }

  pInstance->m_pModelLayers = OdqPropertyModel::createModel();
  pInstance->m_pModelLayers->setPropertySource(pInstance);
  pInstance->m_pModelLayers->setFun_updateXml(odqLayerPropertyManager::updateXml);
  pInstance->m_pModelLayers->setFun_setPropertyValue(odqLayerPropertyManager::setPropertyValue);
  pInstance->m_pModelLayers->setFun_getPropertyPossibleValues(odqLayerPropertyManager::getPropertyPossibleValues);

  if (!updateXml(pInstance))
    pInstance = NULL;
  return pInstance;
}

//static 
bool odqLayerPropertyManager::updateXml(void* pSrc)
{
  ODA_ASSERT_ONCE(pSrc);
  odqLayerPropertyManager* pThis = (odqLayerPropertyManager*) pSrc;
  ODA_ASSERT_ONCE(!pThis->m_pModelFilters.isNull() || !pThis->m_pModelLayers.isNull());

  QString qsXml;
  qsXml += "<?xml version=\"1.0\" encoding=\"windows-1252\"?>\n";
  qsXml += "<Properties>\n";

  OdLyLayerFilterPtr pCurrentFilter;
  OdDbDatabasePtr pDb = OdDbDatabase::cast(pThis->m_pRxDatabase);

  if (pThis->getModelFilters())
  {
    // add Filters Property in Tree

    ODA_ASSERT_ONCE(!pDb.isNull());

    OdLyLayerFilterPtr pRootFilter;
    OdLyLayerFilterManagerPtr pFilterManager = ::odlyGetLayerFilterManager(pDb);
    if (pFilterManager->getFilters(pRootFilter, pCurrentFilter) != eOk)
    {
      ODA_FAIL_ONCE();
      return false;
    }

    qsXml += "<Property Path=\"\" Name=\"Filters\" Type=\"tree\" />\n";
    qsXml += getFiltersAsXml(pRootFilter, pCurrentFilter, "Filters");
    // TODO
    //qsXml += "<Property Path=\"\" Name=\"Filters\" Type=\"category\" />\n";
    //qsXml += "<Property Path=\"Filters\" Name=\"Tree\" Type=\"tree\" />\n";
    //qsXml += getFiltersAsXml(pRootFilter, pCurrentFilter, "Filters/Tree");
    //qsXml += QString("<Property Path=\"Filters\" Name=\"InvertFilter\" VisibleName=\"Invert Filter\" Type=\"bool\" Value=\"%2\""
    //                 " />\n").arg(pThis->m_bInvertFilter ? "true" : "false");
  }

  if (pThis->getModelLayers())
  {
    // add Layers Property in Grid

    qsXml += "<Property Path=\"\" Name=\"Layers\" Type=\"grid\" />\n";

    // add 

    //OdDbLayerTablePtr pLayers = pDb->getLayerTableId().safeOpenObject();
    //pLayers->generateUsageData();
    for (OdRxIteratorPtr pIter = OdDbBaseDatabasePEPtr(pThis->m_pRxDatabase)->layers(pThis->m_pRxDatabase); 
         pIter.get() && !pIter->done(); pIter->next())
    {
      OdRxObjectPtr pLayerObject = pIter->object();
      OdDbBaseLayerPEPtr pLayerPE = OdDbBaseLayerPE::cast(pLayerObject);
      if (pLayerPE.isNull())
      {
        ODA_FAIL_ONCE();
        continue;
      }
      // TODO //OdDbBaseLayerHoopsPEPtr pLayerHoopsPE = OdDbBaseLayerHoopsPE::cast(pLayerObject);
      OdDbLayerTableRecordPtr pLayer = OdDbLayerTableRecord::cast(pLayerObject);

      QString qsLayerName = getIApp()->toQString(pLayerPE->name(pLayerObject));
      ODA_ASSERT_ONCE(!qsLayerName.isEmpty());

      bool isCurrent = false;
      if (!pLayer.isNull())
      {
        isCurrent = (pLayer->objectId() == pDb->getCLAYER());
        //if (!isCurrent && !pLayer->isInUse()) // filled via generateUsageData()
        //  continue;
      }

      bool bSkip = false;
      // useless as is not implemented yet //bSkip = pCurrentFilter && !pCurrentFilter->filter(pLayer);
      QString qsExpression; 
      if (!pCurrentFilter.isNull())
        qsExpression = getIApp()->toQString(pCurrentFilter->filterExpression());

      if (!qsExpression.isEmpty())
      {
        foreach (QString qsExp, qsExpression.split("\" OR "))
        {
          if (qsExp.isEmpty())
          {
            ODA_FAIL_ONCE();
            continue;
          }

          if (qsExp.lastIndexOf("\"") != (qsExp.length() - 1))
            qsExp += "\"";

          int idx = -1;
          foreach (QString qsProp, QStringList() << "NAME" << "USED" << "OFF" << "FROZEN" 
                                                 << "LOCKED" << "COLOR" << "LINETYPE" << "LINEWEIGHT"
                                                 << "PLOTSTYLENAME" << "PLOTTABLE")
          {
            QString qsPref = qsProp + "==\"";
            int pos = qsExp.indexOf(qsPref);
            if (pos < 0)
              continue;
            
            QString qsValue = qsExp.mid(pos + qsPref.length());
            pos = qsValue.indexOf("\"");
            if (pos <= 0)
            {
              ODA_FAIL_ONCE();
              continue;
            }
            qsValue = qsValue.left(pos);

            // USED=="True" AND OFF=="False" AND FROZEN=="True" AND LOCKED=="True" AND COLOR=="53"
            // AND LINETYPE=="ACAD_ISO08W100" AND LINEWEIGHT=="0.15" AND PLOTSTYLENAME=="Moyen" AND PLOTTABLE=="False"

            switch (++idx)
            {
            case 0: // "NAME" // "NAME=="A*"" ...
              {
                QRegExp rxExp(qsValue, Qt::CaseInsensitive, QRegExp::Wildcard);
                if (!rxExp.exactMatch(qsLayerName))
                  bSkip = true;
                break;
              }
            case 1: // "USED"
              {
                bool bValue = QVariant(qsValue).toBool();
                if (!pLayer.isNull() && pLayer->isInUse() != bValue) // ? filled via generateUsageData()
                  bSkip = true;
                break;
              }
            case 2: // "OFF"
              {
                bool bValue = QVariant(qsValue).toBool();
                if (pLayerPE->isOff(pLayerObject) != bValue)
                  bSkip = true;
                break;
              }
            case 3: // "FROZEN" 
              {
                bool bValue = QVariant(qsValue).toBool();
                if (pLayerPE->isFrozen(pLayerObject) != bValue)
                  bSkip = true;
                break;
              }
            case 4: // "LOCKED" 
              {
                bool bValue = QVariant(qsValue).toBool();
                if (!pLayer.isNull() && pLayer->isLocked() != bValue)
                  bSkip = true;
                break;
              }
            case 5: // "COLOR" 
              {
                OdUInt32 iValue = (OdUInt32) QVariant(qsValue).toUInt();
                if (!pLayer.isNull() && pLayer->color().color() != iValue)
                  bSkip = true;
                break;
              }
            case 6: // "LINETYPE" 
              {
                OdString sValue = getIApp()->toOdString(qsValue);
                if (!pLayer.isNull() && OdDbSymUtil::getSymbolName(pLayer->linetypeObjectId()) != sValue)
                  bSkip = true;
                break;
              }
            case 7: // "LINEWEIGHT"
              {
                OdDb::LineWeight enValue = (OdDb::LineWeight) QVariant(qsValue).toInt();
                if (!pLayer.isNull() && pLayer->lineWeight() != enValue)
                  bSkip = true;
                break;
              }
            case 8: // "PLOTSTYLENAME" 
              {
                OdString sValue = getIApp()->toOdString(qsValue);
                if (!pLayer.isNull() && pLayer->plotStyleName() != sValue)
                  bSkip = true;
                break;
              }
            case 9: // "PLOTTABLE"
              {
                bool bValue = QVariant(qsValue).toBool();
                if (!pLayer.isNull() && pLayer->isPlottable()  != bValue)
                  bSkip = true;
                break;
              }
            default:
              ODA_FAIL_ONCE(); // TODO
              break;
            } // end switch
            if (bSkip)
              break;
          } // end foreach
          if (!bSkip)
            break;
        } // end foreach
      } // if (!qsExp.isEmpty())

      if (pThis->m_bInvertFilter)
        bSkip = !bSkip;
      //if (isCurrent)
      //  bSkip = false;
      if (bSkip)
        continue;

      QString qsValue;

      qsLayerName = getIApp()->encodeToXmlAttrib(qsLayerName); // #12642

      // column_status
      if (!pLayer.isNull())
        qsXml += QString("<Property Path=\"Layers/%1\" Name=\"Status\" Type=\"boolPictured\" Value=\"%2\""
                         " Image=\":true=:/images/layer/layer_status_cur.png:false=:/images/layer/layer_status.png\""
                         " />\n").arg(qsLayerName).arg(isCurrent ? "true" : "false");
      // column_name
      qsXml += QString("<Property Path=\"Layers/%1\" Name=\"Name\" Type=\"string\" Value=\"%1\""
                       " ReadOnly=\"%2\"" // is not gray differ to " Enabled=\"false\""
                       " />\n").arg(qsLayerName).arg(pLayer.isNull() ? "true" : "false");
      // column_on
      qsXml += QString("<Property Path=\"Layers/%1\" Name=\"On\" Type=\"boolPictured\" Value=\"%2\""
                       " Image=\":true=:/images/layer/layer_on.png:false=:/images/layer/layer_off.png\""
                       " />\n").arg(qsLayerName).arg(pLayerPE->isOff(pLayerObject) ? "false" : "true");
      //column_freeze
      qsXml += QString("<Property Path=\"Layers/%1\" Name=\"FreezeInAllVP\" VisibleName=\"Freeze\" Type=\"boolPictured\" Value=\"%2\""
                       " Image=\":true=:/images/layer/layer_freeze.png:false=:/images/layer/layer_unfreeze.png\""
                       " />\n").arg(qsLayerName).arg(pLayerPE->isFrozen(pLayerObject) ? "true" : "false");

      if (pLayer.isNull())
        continue;

      // column_lock
      qsXml += QString("<Property Path=\"Layers/%1\" Name=\"Lock\" Type=\"boolPictured\" Value=\"%2\""
                       " Image=\":true=:/images/layer/layer_lock.png:false=:/images/layer/layer_unlock.png\""
                       " />\n").arg(qsLayerName).arg(pLayer->isLocked()  ? "true" : "false");

      // column_color
      qsValue = OdqPropertyModel::stringByOdColor(pLayer->color());
      qsXml += QString("<Property Path=\"Layers/%1\" Name=\"Color\" Type=\"odColor\" Value=\"%2\""
                       " />\n").arg(qsLayerName).arg(qsValue);

      // column_linetype
      qsValue = OdqPropertyModel::stringByLineType(pLayer->linetypeObjectId());
      qsXml += QString("<Property Path=\"Layers/%1\" Name=\"Linetype\" Type=\"combo\" Value=\"%2\""
                       " />\n").arg(qsLayerName).arg(qsValue);
      // column_lineweight
      qsValue = OdqPropertyModel::stringByLineWeight(pLayer->lineWeight());
      qsXml += QString("<Property Path=\"Layers/%1\" Name=\"Lineweight\" Type=\"combo\" Value=\"%2\""
                       " />\n").arg(qsLayerName).arg(qsValue);
      // column_plotstyle
      qsValue = getIApp()->toQString(pLayer->plotStyleName());
      qsXml += QString("<Property Path=\"Layers/%1\" Name=\"PlotStyle\" VisibleName=\"Plot Style\" Type=\"combo\" Value=\"%2\""
                       " />\n").arg(qsLayerName).arg(qsValue);

      // column_plot
      qsXml += QString("<Property Path=\"Layers/%1\" Name=\"Plot\" Type=\"boolPictured\" Value=\"%2\""
                       " Image=\":true=:/images/layer/layer_plot.png:false=:/images/layer/layer_unplot.png\""
                       " />\n").arg(qsLayerName).arg(pLayer->isPlottable() ? "true" : "false");

      if (!pDb->getTILEMODE())
      {
        OdDbObjectId  idVp = pDb->activeViewportId();
        OdDbViewportPtr pVp = OdDbViewport::cast(idVp.safeOpenObject());
        if (pVp.get())
        {
          //column_vpfreeze
          //qsXml += QString("<Property Path=\"Layers/%1\" Name=\"FreezeInVP\" VisibleName=\"Freeze\" Type=\"boolPictured\" Value=\"%2\""
          //                 " Image=\":true=:/images/layer/layer_freeze.png:false=:/images/layer/layer_unfreeze.png\""
          //                 " />\n").arg(qsLayerName).arg(pVp->isLayerFrozenInViewport(pLayer->objectId())  ? "true" : "false");

          // column_vpcolor, "VP Color" pLayer->color(m_vp)
          // column_vplinetype, "VP Linetype" OdqPropertyModel::stringByLineType(pLayer->linetypeObjectId(m_vp))
          // column_vplineweight, "VP Lineweight" OdqPropertyModel::stringByLineWeight(pLayer->lineWeight(m_vp)

          //column_vpplotstyle
          //qsValue = getIApp()->toQString(pLayer->plotStyleName(idVp));
          //qsXml += QString("<Property Path=\"Layers/%1\" Name=\"PlotStyleInVP\" VisibleName=\"Plot Style\" Type=\"combo\" Value=\"%2\""
          //                 " />\n").arg(qsLayerName).arg(qsValue);
        }
        else
          ODA_FAIL_ONCE(); // test
      }

      // column_descr
      qsValue = getIApp()->toQString(pLayer->description());
      qsXml += QString("<Property Path=\"Layers/%1\" Name=\"Description\" Type=\"string\" Value=\"%2\""
                       " />\n").arg(qsLayerName).arg(qsValue);
    }
  }
  
  //   Status(current) Name On Freeze Lock Color Linetype LineWeight "Plot Style" Plot "New VP Freeze" Description
  qsXml += "</Properties>\n";

  bool bRes = true;
  if (!pThis->m_pModelFilters.isNull())
  {
    if (!pThis->m_pModelFilters->updateFromXml(qsXml, "Filters"))
      bRes = false;
    pThis->m_pModelFilters->updateWidget("Filters", pThis->m_bRefreshFilters);
  }
  if (!pThis->m_pModelLayers.isNull())
  {
    if (!pThis->m_pModelLayers->updateFromXml(qsXml, "Layers"))
      bRes = false;
    pThis->m_pModelLayers->updateWidget("Layers", pThis->m_bRefreshLayers);
  }
  return bRes;
}

//static 
QString odqLayerPropertyManager::getFiltersAsXml(const OdLyLayerFilter* pRoot, 
                                                 const OdLyLayerFilter* pCurrent, 
                                                 const QString& cqsSubPath)
{
  QString qsXml;
  if (!pRoot) 
    return qsXml;

  QString qsName = getIApp()->toQString(pRoot->name()),
          qsImage = QString(" Image=\":/images/layer/%1\"")
                      .arg(pRoot->isIdFilter() ? "layer_filter_group.png" : "layer_filter.png"),
          qsSelect = QString(" IsCurrent=\"%1\"")
                      .arg((pCurrent == pRoot) ? "true" : "false");

  qsXml += QString("<Property Path=\"%1\" Name=\"%2\" Type=\"string\"%3%4 />\n")
              .arg(cqsSubPath).arg(qsName).arg(qsImage).arg(qsSelect);

  QString qsSubPath = cqsSubPath;
  qsSubPath += "/";
  qsSubPath += qsName;

  for (unsigned int index = 0; index < pRoot->getNestedFilters().length(); index++)
    qsXml += getFiltersAsXml(pRoot->getNestedFilters()[index], pCurrent, qsSubPath);

  return qsXml;
}

//static 
OdLyLayerFilterPtr odqLayerPropertyManager::getFilterByPath(const OdLyLayerFilterPtr pRoot, 
                                                            const QString& cqsPath)
{
  QString qsPath = cqsPath; 
  QString qsName = OdqPropertyModel::extractFirst(qsPath);
  QString qsSubPath = OdqPropertyModel::extractFirst(qsPath); 
  ODA_ASSERT_ONCE(!qsName.isEmpty());

  if (qsName != getIApp()->toQString(pRoot->name()))
    return OdLyLayerFilterPtr();

  if (qsSubPath.isEmpty())
    return pRoot;

  for (unsigned int index = 0; index < pRoot->getNestedFilters().length(); index++)
  {
    OdLyLayerFilterPtr pFilter = pRoot->getNestedFilters()[index];
    if (!(pFilter = getFilterByPath(pFilter, qsSubPath)).get())
      continue;
    return pFilter;
  }

  return OdLyLayerFilterPtr();
}

//static
OdRxObjectPtr odqLayerPropertyManager::getLayerObjectByName(OdRxObject* pRxDatabase,
                                                            const QString& cqsLayerName)
{
  if (!pRxDatabase)
  {
    ODA_FAIL_ONCE();
    return OdRxObjectPtr();
  }

  QString qsLayerName = getIApp()->decodeFromXmlAttrib(cqsLayerName); // #12642
  OdString sLayerName = getIApp()->toOdString(qsLayerName);

  for (OdRxIteratorPtr pIter = OdDbBaseDatabasePEPtr(pRxDatabase)->layers(pRxDatabase); 
       !pIter->done(); pIter->next())
  {
    OdRxObjectPtr pLayerObject = pIter->object();
    OdDbBaseLayerPEPtr pLayerPE = OdDbBaseLayerPE::cast(pLayerObject);
    ODA_ASSERT_ONCE(!pLayerPE.isNull());
    if (pLayerPE.isNull() || pLayerPE->name(pLayerObject) != sLayerName)
      continue;
    
    return pLayerObject;
  }

  return OdRxObjectPtr(); // was not found
}

// #8581: Modify ODA File Viewer to directly update the screen when turning 
// layers on/off (rather than waiting until the dialog box is closed).
inline void refreshView(OdRxObject* pRxDatabase, bool bForce = true)
{
  IqViewSystem* iVS = getIViewSystem();
  if (!iVS)
    return;

  IqView* iView = iVS->getView(pRxDatabase);
  if (!iView)
    return;

  if (bForce)
  {
    OdGsModel* pGsModel = iView->getGsModel();
    if (pGsModel)
      pGsModel->invalidate(OdGsModel::kInvalidateAll);
  }

  iView->updateGL(); // repaint
}

//static 
bool odqLayerPropertyManager::setPropertyValue(void* pSrc, const QString& cqsPath, const QString& qsValue)
{
  ODA_ASSERT_ONCE(pSrc);
  odqLayerPropertyManager* pThis = (odqLayerPropertyManager*) pSrc;
  ODA_ASSERT_ONCE(!pThis->m_pModelFilters.isNull() || !pThis->m_pModelLayers.isNull());

  QString qsPath = cqsPath;
  QString qsSubPath = OdqPropertyModel::extractFirst(qsPath);

  if (qsSubPath == "Filters")
  {
    if (qsPath == "InvertFilter")
    {
      ODA_ASSERT_ONCE(qsPath == "Filters/InvertFilter");
      pThis->m_bInvertFilter = !pThis->m_bInvertFilter;
      pThis->m_bRefreshLayers = true;
      return true;
    }

    if (!qsPath.indexOf("Tree")) // "Filters" or "Filters/Tree"
      OdqPropertyModel::extractFirst(qsPath);

    ODA_ASSERT(qsValue == "<setCurrent>"); // TODO

    // filter was changed

    OdDbDatabasePtr pDb = OdDbDatabase::cast(pThis->m_pRxDatabase);
    ODA_ASSERT_ONCE(!pDb.isNull());

    OdLyLayerFilterManagerPtr pFilterManager = ::odlyGetLayerFilterManager(pDb);
    OdLyLayerFilterPtr pRootFilter,
                       pCurrentFilter;
    if (pFilterManager->getFilters(pRootFilter, pCurrentFilter) != eOk)
    {
      ODA_FAIL_ONCE();
      return false;
    }

    OdLyLayerFilterPtr pChosenFilter = getFilterByPath(pRootFilter, qsPath);
    ODA_ASSERT_ONCE(pChosenFilter.get());

    if (pCurrentFilter == pChosenFilter || !pChosenFilter.get())
      return false;

    pFilterManager->setFilters(pRootFilter, pChosenFilter);
    pThis->m_bRefreshLayers = true;
    return true;
  }

  if (qsSubPath == "Layers" )
  {
    QString qsLayerName = OdqPropertyModel::extractFirst(qsPath);
    if (qsValue == "<setCurrent>")
    {
      ODA_ASSERT_ONCE(qsPath.isEmpty());
      return false; // unused here
    }

    if (!qsPath.isEmpty()) // column
    {
      OdRxObjectPtr pLayerObject = getLayerObjectByName(pThis->m_pRxDatabase, qsLayerName);
      if (pLayerObject.isNull())
      {
        ODA_FAIL_ONCE();
        return false;
      }
      
      OdDbBaseLayerPEPtr pLayerPE = OdDbBaseLayerPE::cast(pLayerObject);
      ODA_ASSERT_ONCE(!pLayerPE.isNull());

      if (qsPath == "On") // column_on
      {
        pLayerPE->setIsOff(pLayerObject, !pLayerPE->isOff(pLayerObject));
        OdExtDbModule* pExtDb = getIApp()->getExtDbModule(pThis->m_pRxDatabase);
        refreshView(pThis->m_pRxDatabase, pExtDb && pExtDb->baseModuleName(true) == L"dwf"); // #8581
        return true;
      }
      if (qsPath == "FreezeInAllVP") // column_freeze
      {
        pLayerPE->setIsFrozen(pLayerObject, !pLayerPE->isFrozen(pLayerObject));
        return true;
      }

      OdDbLayerTableRecordPtr pLayer = OdDbLayerTableRecord::cast(pLayerObject);
      if (!pLayer.isNull())
      {
        pLayer->upgradeOpen();

        if (qsPath == "Status") // column_status
        {
          // set current layer in database

          OdDbDatabasePtr pDb = OdDbDatabase::cast(pThis->m_pRxDatabase);
          ODA_ASSERT_ONCE(!pDb.isNull());

          bool bValue = QVariant(qsValue).toBool();
          if (!bValue || pLayer->objectId() == pDb->getCLAYER())
            return false;

          pDb->setCLAYER(pLayer->objectId());
          bValue = pLayer->objectId() == pDb->getCLAYER();
          ODA_ASSERT_ONCE(bValue);
          return bValue;
        }
        if (qsPath == "Name") // column_name
        {
          if (   qsValue.isEmpty() // invalid
              || getLayerObjectByName(pThis->m_pRxDatabase, qsValue).get()) // duplicated
            return false;            
          pLayer->setName(getIApp()->toOdString(qsValue));
          return true;
        }
        if (qsPath == "Lock") // column_lock
        {
          pLayer->setIsLocked(!pLayer->isLocked());
          return true;
        }
        if (qsPath == "Color") // column_color
        {
          bool bOk = false;

          OdCmEntityColor colorEnt = OdqPropertyModel::OdColorByString(qsValue, &bOk);
          ODA_ASSERT_ONCE(bOk);
          OdCmColor color(colorEnt.colorMethod());
          color.setColor(colorEnt.color());

          if (!bOk || color == pLayer->color())
            return false;

          pLayer->setColor(color);
          refreshView(pThis->m_pRxDatabase); // #8581
          return true;
        }
        if (qsPath == "Linetype") // column_linetype
        {
          //static 
          OdDbObjectId id = OdqPropertyModel::lineTypeIdByString(pThis->m_pRxDatabase, qsValue);
          if (!id.isNull())
          {
            pLayer->setLinetypeObjectId(id);
            if (id != pLayer->linetypeObjectId())
              return false;
            refreshView(pThis->m_pRxDatabase); // #8581
            return true;
          }
          ODA_FAIL_ONCE();
          return false;
        }
        if (qsPath == "Lineweight") // column_lineweight
        {
          OdDb::LineWeight lw = OdqPropertyModel::lineWeightByString(qsValue);
          if (lw >= OdDb::kLnWtByLwDefault) 
          {
            pLayer->setLineWeight(lw);
            if (lw != pLayer->lineWeight())
              return false;
            refreshView(pThis->m_pRxDatabase); // #8581
            return true;
          }
          ODA_FAIL_ONCE();
          return false;
        }
        if (qsPath == "PlotStyle") // column_plotstyle
        {
          //ODA_ASSERT_ONCE(!qsValue.isEmpty());
          if (   !qsValue.isEmpty() 
              && pLayer->setPlotStyleName(getIApp()->toOdString(qsValue)) != eOk)
          {
            ODA_FAIL_ONCE();
            return false;
          }
          return true;
        }
        if (qsPath == "Plot") // column_plot
        {
          pLayer->setIsPlottable(!pLayer->isPlottable());
          return true;
        }
        if (qsPath == "Description") // column_descr
        {
          pLayer->setDescription(getIApp()->toOdString(qsValue));
          return true;
        }

        // TODO //OdDbBaseLayerHoopsPEPtr pLayerHoopsPE = OdDbBaseLayerHoopsPE::cast(pLayerObject);
        OdDbObjectId id;
        if (!pLayer.isNull())
        {
          id = pLayer->objectId();
          //OdDbLayerTablePtr pLayers = pDb->getLayerTableId().safeOpenObject();
          //pLayers->generateUsageData();
        }

        OdDbDatabasePtr pDb = OdDbDatabase::cast(pThis->m_pRxDatabase);

        if (qsPath == "FreezeInVP") // column_vpfreeze
        {
          OdDbViewportPtr pVp = OdDbViewport::cast(pDb->activeViewportId().safeOpenObject(OdDb::kForWrite));
          if (!pVp.get())
          {
            ODA_FAIL_ONCE(); //TODO
            return false;
          }

          OdDbObjectIdArray ids(1);
          ids.append(id);
          if (pVp->isLayerFrozenInViewport(id))
            pVp->thawLayersInViewport(ids);
          else
            pVp->freezeLayersInViewport(ids);
          return true;
        }
      }

      ODA_FAIL_ONCE(); //TODO
      return false;
    }
  }

  ODA_FAIL_ONCE(); //TODO
  return false;
}

//static 
bool odqLayerPropertyManager::getPropertyPossibleValues(void* pSrc, const QString& cqsPath, 
                                                        QStringList& values, QStringList& icons)
{
  ODA_ASSERT_ONCE(pSrc);
  odqLayerPropertyManager* pThis = (odqLayerPropertyManager*) pSrc;
  ODA_ASSERT_ONCE(!pThis->m_pModelFilters.isNull() && !pThis->m_pModelLayers.isNull());

  QString qsPath = cqsPath;
  QString qsSubPath = OdqPropertyModel::extractFirst(qsPath);

  if (qsSubPath == "Layers")
  {
    QString qsLayerName = OdqPropertyModel::extractFirst(qsPath);
    // qsPath is column name in grid

    if (qsPath == "Color")
    {
      ODA_ASSERT_ONCE(!icons.size());
      return OdqPropertyModel::getPossibleOdColors(values, true); // without byLayer etc
    }

    OdRxObjectPtr pLayerObject = getLayerObjectByName(pThis->m_pRxDatabase, qsLayerName);
    //OdDbBaseLayerPEPtr pLayerPE = OdDbBaseLayerPE::cast(pLayerObject);
    OdDbLayerTableRecordPtr pLayer = OdDbLayerTableRecord::cast(pLayerObject);
    if (pLayerObject.isNull() || pLayer.isNull())
    {
      ODA_FAIL_ONCE();
      return false;
    }

    OdDbDatabasePtr pDb = OdDbDatabase::cast(pThis->m_pRxDatabase);
    ODA_ASSERT_ONCE(!pDb.isNull());

    if (qsPath == "Linetype") // column_linetype
    {
      OdqPropertyModel::getPossibleLineTypes(pThis->m_pRxDatabase, values, icons, true);
      if (values.isEmpty())
        values << OdqPropertyModel::stringByLineType(pLayer->linetypeObjectId());
      return true;
    }
    if (qsPath == "Lineweight") // column_lineweight
    {
      OdqPropertyModel::getPossibleLineWeights(values, icons, true);
      return true;
    }

    if (qsPath == "PlotStyle") // column_plotstyle
    {
      //OdDbDictionaryWithDefaultPtr pPlotStDic;
      OdDbDictionaryPtr pPlotStyleDic = pDb->getPlotStyleNameDictionaryId(false).openObject();
      if (!pPlotStyleDic.isNull())
      {
        for (OdDbDictionaryIteratorPtr pItr = pPlotStyleDic->newIterator();
             !pItr->done(); pItr->next() )
        {
          QString qsName = getIApp()->toQString(pItr->name());
          ODA_ASSERT_ONCE(!qsName.isEmpty());
          values << qsName;
        }
      }
      else
      {
        ODA_FAIL_ONCE(); // test
      }

      if (values.isEmpty())
        values << getIApp()->toQString(pLayer->plotStyleName());
      return true;
    }

  } // end if (!qsPath.indexOf("Layers/"))

  ODA_FAIL_ONCE(); // TODO
  return false;
}

///////////////////////////////////////////////////////////////////////////////

void OdqCmd_Layer::execute(OdEdCommandContext* pCmdCtx)
{
  static OdSharedPtr<odqLayerPropertyManager> s_pLayerManager;
  ODA_ASSERT_ONCE(s_pLayerManager.isNull());

  s_pLayerManager = odqLayerPropertyManager::create(pCmdCtx->baseDatabase());

  OdqMainWindow* pMainWindow = qobject_cast<OdqMainWindow*>(getIApp()->getMainWindow());
  ODA_ASSERT_ONCE(pMainWindow);

  OdqPropertyDialog dlg(pMainWindow);
  dlg.setWindowTitle(QObject::tr("Layer Properties Manager"));

  OdqPropertyModel* pModelFilters = NULL;
  if (!s_pLayerManager.isNull())
    pModelFilters = s_pLayerManager->getModelFilters();
  QScrollArea* pScroll = NULL;
  QGroupBox* pWidgetFilters = NULL;
  if (pModelFilters)
  {
    QLayout* pHorLayout = dlg.pushLayout(); // QHBoxLayout

    pWidgetFilters = new QGroupBox();
    pHorLayout->addWidget(pWidgetFilters);
    //pWidgetFilters->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    pModelFilters->setWidget(pWidgetFilters);
    pModelFilters->updateWidget("Filters");

    pScroll = new QScrollArea();
    pHorLayout->addWidget(pScroll);
  }

  OdqPropertyModel* pModelLayers = NULL;
  if (!s_pLayerManager.isNull())
    pModelLayers = s_pLayerManager->getModelLayers();
  QGroupBox* pWidgetLayers = NULL;
  if (pModelLayers)
  {
    pWidgetLayers = new QGroupBox(pScroll);
    pModelLayers->setWidget(pWidgetLayers);
    if (pScroll)
    {
      pScroll->setWidget(pWidgetLayers);
      pScroll->setWidgetResizable(true);
      pScroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    }
    else
      dlg.addWidget(pWidgetLayers);
    //pWidgetLayers->setSizePolicy(pWidgetLayers->sizePolicy().setHorizontalPolicy(QSizePolicy::Maximum);
    pModelLayers->updateWidget("Layers");
  }
  else
  {
    ODA_FAIL_ONCE(); // test
  }

  dlg.setButtons(QDialogButtonBox::Ok);
  // TODO with undo // dlg.setButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

  pMainWindow->restoreWidgetPlacement(&dlg, "LayerPropertyManager",
                                      pMainWindow->size().width() * 9 / 10,
                                      pMainWindow->size().height() / 2);
  if (pWidgetFilters)
    pWidgetFilters->setFixedWidth(dlg.size().width() / 4); // TODO // setMinimumWidth

  int res = dlg.exec();

  pMainWindow->saveWidgetPlacement(&dlg, "LayerPropertyManager");
  s_pLayerManager = NULL;

  if (res != QDialog::Accepted)
    FIX_MAC_APP_TROW(OdEdCancel()); //  throw OdEdCancel();

  //ODA_FAIL_ONCE(); // TODO ? refresh current view
}

//void OdqCmd_Options::execute(OdEdCommandContext* pCmdCtx)
//{
//  OdqMainWindow* pMainWindow = qobject_cast<OdqMainWindow*>(getIApp()->getMainWindow());
//  ODA_ASSERT_ONCE(pMainWindow);
//
//  OdqPropertyDialog dlg(pMainWindow);
//  dlg.setWindowTitle(QObject::tr("Options / Display"));
//
//  OdRxObject* pRxDatabase = pCmdCtx->baseDatabase();
//  if (pRxDatabase)
//  {
//    QString qsFileName = getIApp()->toQString(getIConsole()->getFilePathName(pRxDatabase)); 
//    qsFileName = QFileInfo(qsFileName).fileName();
//    dlg.addWidget(new QLabel(QObject::tr("Current drawing: %1").arg(qsFileName)));
//  }
//
//  // “+options 1” ==> "display" sheet of ACAD options (contains background color ...)
//
//  QPointer<QTabWidget> pTabWidget = qobject_cast<QTabWidget*>(dlg.addWidget(new QTabWidget(&dlg)));
//  pTabWidget->setTabPosition(QTabWidget::North);
//
//  QPointer<OdqPropertyDialog> pFilesTab  = new OdqPropertyDialog(pTabWidget);
//  pFilesTab->setModal(false);
//  pTabWidget->addTab(pFilesTab, QObject::tr("Files"));
//
//  QPointer<OdqPropertyDialog> pDisplayTab = new OdqPropertyDialog(pTabWidget);
//  pDisplayTab->setModal(false);
//  pTabWidget->addTab(pDisplayTab, QObject::tr("Display"));
//
//  // TODO // test
//  pDisplayTab->addTableProperty(QObject::tr("2D model space color"), // sName 
//                                QVariant(Qt::darkGreen));
//  pDisplayTab->addTableProperty(QObject::tr("Sheet / Layout color"), // sName 
//                                QVariant::Color); // type
//
//  dlg.setButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
//
//  if (dlg.exec() != QDialog::Accepted)
//    return;
//  
//  QList<QVariant> lstValues = pDisplayTab->tableValues();
//  ODA_ASSERT_ONCE(lstValues.size() == 2);
//
//  QVariant vClr = lstValues[0];
//  QColor clr = vClr.value<QColor>();
//  if (clr != Qt::darkGreen)
//  {
//    ODA_FAIL_ONCE(); // TODO
//  }
//}

// (Console only) : restore default values of group registry variables [ALL/DWG/DGN/APP]<APP>

void OdqCmd_ResetVars::execute(OdEdCommandContext* pCmdCtx)
{
  OdString sKeysInPrompt = L"ALl/Dwg/App",
           sKeys = L"ALl Dwg App",
           sDefault = L"app";
  OdRxObject* pRxDatabase = pCmdCtx->baseDatabase();
  OdExtDbModule* pExtDb = getIApp()->getExtDbModule(pRxDatabase);
  int indexDefault = !pExtDb ? 2 : 3;
  if (pRxDatabase)
  {
    if (!pExtDb)
    {
      sDefault = L"dwg";
      indexDefault = 1;
    }
    else
    {
      OdString sName = pExtDb->baseModuleName(true);
      ODA_ASSERT_ONCE(!sName.isEmpty());
      if (sName == L"dgn")
      {
        sKeysInPrompt = L"ALl/DWg/DGn/App";
        sKeys = L"ALl DWg DGn App";
        sDefault = sName;
        indexDefault = 1;
      }
      else if (sName == L"bim")
      {
        sKeysInPrompt = L"ALl/Dwg/Bim/App";
        sKeys = L"ALl Dwg Bim App";
        sDefault = sName;
        indexDefault = 1;
      }
      else
      {
        ODA_FAIL_ONCE(); // TODO
        pExtDb = NULL;
      }
    }
  }

  OdString sPrompt;
  sPrompt.format(OD_T("Enter group of registry variables to restore [%ls]<%ls>"), 
                 sKeysInPrompt.c_str(), // L"ALl/Dwg/App" or L"ALl/DWg/DGn/App" or L"ALl/Dwg/Bim/App"
                 sDefault.c_str());
  OdEdUserIO* pIO = pCmdCtx->userIO();
  int indexKeyword = pIO->getKeyword(sPrompt, 
                                     sKeys, // L"ALl Dwg App" or L"ALl DWg DGn App" or L"ALl Dwg Bim App"
                                     indexDefault);
  OdStringArray vars;
  OdqAppProps* pAppProps = qobject_cast<OdqAppProps*>(getIApp()->getSingle(IqAppProps_iid));
  ODA_ASSERT_ONCE(pAppProps);
  switch (indexKeyword)
  {
  case 0: // all
    vars.append(static_cast<OdqServices*>(getIApp()->getBaseHostAppServices())->resetRegVariables());
    if (pExtDb)
      vars.append(pExtDb->resetRegVariables());
    if (pAppProps)
      vars.append(pAppProps->resetRegVariables());
    break;
  case 1: // dwg
    vars.append(static_cast<OdqServices*>(getIApp()->getBaseHostAppServices())->resetRegVariables());
    break;
  case 2: // dgn/bim or app
    if (pExtDb)
    { // dgn 
      vars.append(pExtDb->resetRegVariables());
      break;
    }
  case 3: // app
    if (pAppProps)
      vars.append(pAppProps->resetRegVariables());
    break;
  default:
    ODA_FAIL_ONCE(); // TODO
    throw OdError(eNotImplemented);
  }
  if (!vars.size())
  {
    pIO->putString(getIApp()->toOdString(
      QObject::tr("  Alredy done.")));
    return;
  }

  pIO->putString(getIApp()->toOdString(
                   QObject::tr("  The next variables were restored to default values:")));
  for (int index = 0; index < (int) vars.size(); index++)
  {
    OdString sVar = vars.getAt(index),
             sMsg = L"  " + sVar;

    foreach (int offs, QList<int>() << 24 << 46)
    {
      if (index >= (int)(vars.size() - 1))
        continue;

      index++;
      int numSpaces = offs - sMsg.getLength();
      if (numSpaces < 1)
        numSpaces = 1;
      sMsg += OdString(L' ', numSpaces) + vars.getAt(index);
    }

    pIO->putString(sMsg);

    if (OdString(sVar).makeLower() == L"largetoolbars")
      getIApp()->refreshQObjetcts("<event-refresh:toolbarsize_changed>");
  }
}

void OdqCmd_About::execute(OdEdCommandContext*) // pCmdCtx
{
  OdqMainWindow* pMainWindow = qobject_cast<OdqMainWindow*>(getIApp()->getMainWindow());
  ODA_ASSERT_ONCE(pMainWindow);

  QString qsAboutUrl = "qrc:/about.html";

  if (   ::odrxDynamicLinker()->loadModule(OD_T("OdaQtOpenRL.txv")).get()
    #ifdef Q_CC_MSVC
      || ::odrxDynamicLinker()->loadModule(OD_T("WinOpenRL.txv")).get()
    #endif
      )
  {  
    qsAboutUrl = "qrc:/about_caustic.html";
  }

  pMainWindow->showUrl(qsAboutUrl, "About %1");
}

void OdqCmd_Help::execute(OdEdCommandContext* pCmdCtx)
{
  OdqMainWindow* pMainWindow = qobject_cast<OdqMainWindow*>(getIApp()->getMainWindow());
  ODA_ASSERT_ONCE(pMainWindow);

  QString qsHelpUrl = "qrc:/help/help.html";
  if (getIConsole()->isInputStringReady(pCmdCtx->baseDatabase()))
  {
    OdString sPrompt = OD_T("Enter path of tested HTML file");
    OdString sPath = pCmdCtx->userIO()->getString(sPrompt);
    if (sPath == L"...")
    {
      sPath = pCmdCtx->userIO()->getFilePath(sPrompt, 
                                             OdEd::kGfpForOpen, 
                                             OD_T("Browse File"), // dialog caption 
                                             OD_T("html"), // default extension
                                             OdString::kEmpty, // file name
                                             OD_T("HTML files (*.html)|*.html||"));
      if (sPath.isEmpty())
        return;
      qsHelpUrl = QUrl::fromLocalFile(getIApp()->toQString(sPath)).toString();
    }
  }

#if !(defined(ANDROID) || defined(__IPHONE_OS_VERSION_MIN_REQUIRED))
  bool bModal = false;
 #else
  bool bModal = true;
 #endif

  pMainWindow->showUrl(qsHelpUrl, "Help %1", bModal);
}
