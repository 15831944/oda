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
// PropServicesImpl.cpp
//

#include <QUuid>
#include <QMap>
#include <QMessageBox>
#include <QPushButton>
#include <QLayout>
#include <QDateTime>
#include <QtCore>

#include "OdaCommon.h"
#include "PropServicesImpl.h"
#include "StaticRxObject.h"
#include "RxDictionary.h"
#include "DbFiler.h"
#include "CmColorBase.h"
#include "Ge/GePoint3d.h"
#include "DbDatabase.h"
#include "DbBaseSubentId.h"
#include "SharedPtr.h"

#include "OdqInterfaces.h"
#include "OdqMainWindow.h"
#include "OdqPropertyDialog.h"
#include "OdqPropertyPalette.h"
#include "OdqPropertyPageDialog.h"
#include "ExtDbModule.h"

///////////////////////////////////////////////////////////////////////////////

#if defined(ODA_PROP_SERVICES_UNLINKED_RXINIT)
  ODRX_NO_CONS_DEFINE_MEMBERS(OdPropServices, OdRxObject);
#endif

///////////////////////////////////////////////////////////////////////////////

class OdqPropSourceData
{
public:
  OdqPropSourceData(OdPropSource* pSource,
                    const OdString& sBasePath,
                    const OdString& sRootNodeName = OD_T("Properties"),
                    class OdqPropDialogData* pOwnerDlgData = NULL,
                    bool bUseBasePathForRoot = true);
  virtual ~OdqPropSourceData();

  bool clearMergeFilter(); // invoked by selection set changing only
  bool setMergeFilter(const OdString& sMergeFilter = OdString::kEmpty, // empty for all
                      const OdString& sMergeFilterAttribName =  OD_T("Filter"), // empty is already set by previous call
                      const OdString& sUnmergedBasePath = OdString::kEmpty); // already set by previous call
  bool isSourceMatched(const OdPropSource* pSource) const;
  
  OdqPropertyModel* getModel();

  static bool updateXml(void* pSrcDataVoid, bool bRecreateWidgets);

  static bool updateXml(void* pSrcDataVoid);
  static bool setPropertyValue(void* pSrcDataVoid, const QString& qsPath, const QString& qsValue);
  static bool getPropertyPossibleValues(void* pSrcDataVoid, const QString& qsPath, 
                                        QStringList& values, QStringList& icons);
  
  static QString checkXmlHeader(const QString& qsXmlSrc, 
                                const QString& cqsRootNodeName = "");
protected:
  OdPropSource* m_pSource;
  OdSharedPtr<OdqPropertyModel> m_pModel;
  QString m_qsBasePath, // merging result places to this path too
          m_qsRootNodeName;
  QString m_qsUnmergedBasePath,
          m_qsMergeFilter, // empty for all
          m_qsMergeFilterAttribName; // empty is already set by previous call

  OdqPropDialogData* m_pOwnerDlgData;

private:
  Q_DISABLE_COPY(OdqPropSourceData)
};

///////////////////////////////////////////////////////////////////////////////

OdqPropDialogData::OdqPropDialogData(QWidget* pParent, // = NULL
                                     Qt::WindowFlags flags, // = 0
                                     OdPropServices::TSeparateCategoryTreePlace catPlace) // = OdPropServices::enSepCatPlace_None
  : m_pDialog((catPlace == OdPropServices::enSepCatPlace_None)
                ? new OdqPropertyDialog(pParent, flags) : new OdqPropertyPageDialog(pParent, flags, catPlace))
{
}

OdqPropDialogData::~OdqPropDialogData()
{
}

OdqPropertyDialog* OdqPropDialogData::getDialog()
{
  return m_pDialog.get();
}

bool OdqPropDialogData::refreshSources(OdPropSource* pSourceOnly, // = NULL // null for all
                                        bool bRecreateWidgets) // = false
{
  bool bRes = !pSourceOnly;
  foreach (OdSharedPtr<OdqPropSourceData> pSrcData, m_lstSrcData)
  {
    ODA_ASSERT_ONCE(!pSrcData.isNull());
    if (!pSrcData->isSourceMatched(pSourceOnly))
      continue;

    pSrcData->clearMergeFilter();
    if (!OdqPropSourceData::updateXml(pSrcData.get(), bRecreateWidgets))
      bRes = false;
    if (pSourceOnly)
      break;
  }
  return bRes;
}

//slots
void OdqPropDialogData::refresh(const QString& qsKey, const QString& qsArg)
{
  refreshSources();
}

///////////////////////////////////////////////////////////////////////////////

OdqPropSourceData::OdqPropSourceData(OdPropSource* pSource,
                                     const OdString& sBasePath,
                                     const OdString& sRootNodeName, // = OD_T("Properties")
                                     OdqPropDialogData* pOwnerDlgData, // = NULL
                                     bool bUseBasePathForRoot) // = true
  : m_pSource(pSource)
  , m_qsBasePath(getIApp()->toQString(sBasePath))
  , m_qsRootNodeName(getIApp()->toQString(sRootNodeName))
  , m_pOwnerDlgData(pOwnerDlgData)
{
  if (m_qsRootNodeName.isEmpty())
    m_qsRootNodeName = "Properties";

  m_pModel = OdqPropertyModel::createModel(bUseBasePathForRoot ? m_qsBasePath 
                                                               : "", // old way (invalid for palette tree as widget)
                                           m_qsRootNodeName);
  ODA_ASSERT_ONCE(pSource && !m_qsBasePath.isEmpty() && !m_pModel.isNull());

  m_pModel->setPropertySource(this);
  m_pModel->setFun_updateXml(OdqPropSourceData::updateXml);
  m_pModel->setFun_setPropertyValue(OdqPropSourceData::setPropertyValue);
  m_pModel->setFun_getPropertyPossibleValues(OdqPropSourceData::getPropertyPossibleValues);
}

OdqPropSourceData::~OdqPropSourceData()
{
}

bool OdqPropSourceData::clearMergeFilter()
{
  if (m_qsMergeFilter.isEmpty())
    return false;
  m_qsMergeFilter.clear(); // m_qsMergeFilter = "";
  return true;
}

bool OdqPropSourceData::setMergeFilter(const OdString& sMergeFilter, // = OdString::kEmpty // empty for all
                                       const OdString& sMergeFilterAttribName, // =  OD_T("Filter") // empty is already set by previous call
                                       const OdString& sUnmergedBasePath) // = OdString::kEmpty // already set by previous call
{
  m_qsMergeFilter = getIApp()->toQString(sMergeFilter);
  if (m_qsMergeFilter == "*")
    m_qsMergeFilter.clear();

  QString qsMergeFilterAttribName = getIApp()->toQString(sMergeFilterAttribName);
  if (!qsMergeFilterAttribName.isEmpty())
    m_qsMergeFilterAttribName = qsMergeFilterAttribName;

  QString qsUnmergedBasePath = getIApp()->toQString(sUnmergedBasePath);
  if (!qsUnmergedBasePath.isEmpty() && qsUnmergedBasePath.indexOf(m_qsBasePath))
    m_qsUnmergedBasePath = qsUnmergedBasePath;

  ODA_ASSERT_ONCE(!m_qsUnmergedBasePath.isEmpty());
  if (m_qsUnmergedBasePath.isEmpty())
    return false;

  bool bRes = m_pModel->setMergeFilter(m_qsMergeFilter, m_qsMergeFilterAttribName, m_qsUnmergedBasePath);
  if (bRes)
    m_pModel->updateWidget(m_qsBasePath, 
                           true); // bRecreateWidgets // TODO
  return bRes;
}

bool OdqPropSourceData::isSourceMatched(const OdPropSource* pSource) const
{
  if (pSource && pSource != m_pSource)
    return false;
  return true; // for brk
}

OdqPropertyModel* OdqPropSourceData::getModel()
{
  ODA_ASSERT_ONCE(!m_pModel.isNull());
  return m_pModel.get();
}

//static 
bool OdqPropSourceData::updateXml(void* pSrcDataVoid)
{
  return OdqPropSourceData::updateXml(pSrcDataVoid, false);
}

//static 
bool OdqPropSourceData::updateXml(void* pSrcDataVoid, bool bRecreateWidgets)
{
  ODA_ASSERT_ONCE(pSrcDataVoid);
  OdqPropSourceData* pThis = (OdqPropSourceData*) pSrcDataVoid;

  OdString sXmlSrc;
  if (!pThis->m_pSource->updateXml(sXmlSrc))
    return false;

  QString qsXmlSrc = getIApp()->toQString(sXmlSrc);

  QString qsXml = checkXmlHeader(qsXmlSrc, pThis->m_qsRootNodeName);

  // update model
  bool bRes = pThis->m_pModel->updateFromXml(qsXml, pThis->m_qsBasePath, pThis->m_qsRootNodeName,
                                             pThis->m_qsUnmergedBasePath, pThis->m_qsMergeFilter, pThis->m_qsMergeFilterAttribName);
  pThis->m_pModel->updateWidget(pThis->m_qsBasePath, bRecreateWidgets);

  return bRes;
}

//static 
bool OdqPropSourceData::setPropertyValue(void* pSrcDataVoid, 
                                         const QString& qsPath, const QString& qsValue)
{
  OdqPropSourceData* pThis = (OdqPropSourceData*) pSrcDataVoid;
  ODA_ASSERT_ONCE(pThis && pThis->m_pSource);
  OdPropSource* pSourceOnly = pThis->m_pSource;

  OdString sPath = getIApp()->toOdString(qsPath),
           sValue = getIApp()->toOdString(qsValue);
  OdPropSource::Action action = OdPropSource::enAction_None;
  if (!pSourceOnly->setPropertyValue(sPath, sValue, action))
    return false;

  bool bRecreateWidgets = false;
  switch (OdPropSource::Action(action & ~4)) // ~OdPropSource::enAction_RecreateWidgets
  {
  case OdPropSource::enAction_Close: // Ok
    {
      ODA_ASSERT_ONCE(pThis->m_pOwnerDlgData && pThis->m_pOwnerDlgData->getDialog());
      if (pThis->m_pOwnerDlgData && pThis->m_pOwnerDlgData->getDialog())
        pThis->m_pOwnerDlgData->getDialog()->accept();
      return false; // true is useless here
    }
  case OdPropSource::enAction_Cancel:
    {
      ODA_ASSERT_ONCE(pThis->m_pOwnerDlgData && pThis->m_pOwnerDlgData->getDialog());
      if (pThis->m_pOwnerDlgData && pThis->m_pOwnerDlgData->getDialog())
        pThis->m_pOwnerDlgData->getDialog()->reject();
      return false; // true is useless here
    }
  case OdPropSource::enAction_RefreshAllSources:
    pSourceOnly = NULL;
  case OdPropSource::enAction_RefreshSource:
    ODA_ASSERT_ONCE(pThis->m_pOwnerDlgData && pThis->m_pOwnerDlgData->getDialog());
    if (action & 4) // & OdPropSource::enAction_RecreateWidgets
      bRecreateWidgets = true;
    if (   pThis->m_pOwnerDlgData && pThis->m_pOwnerDlgData->getDialog()
        && pThis->m_pOwnerDlgData->refreshSources(pSourceOnly, bRecreateWidgets))
      return false; // already refreshed
    break;
  default:
    ODA_FAIL_ONCE(); // TODO
  case OdPropSource::enAction_None:
    break;
  }

  return true;
}

//static 
bool OdqPropSourceData::getPropertyPossibleValues(void* pSrcDataVoid, 
                                                  const QString& qsPath, 
                                                  QStringList& values, QStringList& icons)
{
  ODA_ASSERT_ONCE(pSrcDataVoid);
  OdqPropSourceData* pThis = (OdqPropSourceData*) pSrcDataVoid;

  OdString sPath = getIApp()->toOdString(qsPath);
  OdStringArray arrValues,
                arrIconPaths;
  if (!pThis->m_pSource->getPropertyPossibleValues(sPath, arrValues, arrIconPaths))
    return false;

  OdStringArray::size_type sz = arrValues.size(),
                           index;
  for (index = 0; index < sz; index++)
  {
    OdString sValue = arrValues[index];
    values << getIApp()->toQString(sValue);
  }
  sz = arrIconPaths.size();
  for (index = 0; index < sz; index++)
  {
    OdString sIconPath = arrIconPaths[index];
    icons << getIApp()->toQString(sIconPath);
  }
  return true;
}

//static 
QString OdqPropSourceData::checkXmlHeader(const QString& qsXmlSrc, 
                                          const QString& cqsRootNodeName) // = ""
{
  QString qsRootNodeName(cqsRootNodeName);
  if (qsRootNodeName.isEmpty())
    qsRootNodeName = "Properties";

  QString qsXml; 
  if (qsXmlSrc.indexOf("<?xml ") < 0)
    qsXml += "<?xml version=\"1.0\" encoding=\"windows-1252\"?>\n";

  bool bAddRootNode = (qsXmlSrc.indexOf("<" + qsRootNodeName + ">") < 0);
  if (bAddRootNode)
    qsXml += "<" + qsRootNodeName + ">\n";

  qsXml += qsXmlSrc;

  if (bAddRootNode)
    qsXml += "</" + qsRootNodeName + ">\n";

  return qsXml;
}


///////////////////////////////////////////////////////////////////////////////

ODRX_NO_CONS_DEFINE_MEMBERS(OdPropServicesImpl, OdPropServices);

OdPropServicesImpl::OdPropServicesImpl()
  : m_pStrConvController(NULL)
{
  loadCatWeights();
}

OdPropServicesImpl::~OdPropServicesImpl()
{
  // moved into OdqMainWindow::uninitAppModule() // saveCatWeights();
}

OdqPropDialogData* OdPropServicesImpl::getDialogData(const OdString& sDlgKey,
                                                     QMap<OdString, OdSharedPtr<OdqPropDialogData> >::iterator* pItr) // = NULL
{
  ODA_ASSERT_ONCE(!sDlgKey.isEmpty());

  QMap<OdString, OdSharedPtr<OdqPropDialogData> >::iterator itr = m_mapData.find(sDlgKey);
  if (itr == m_mapData.end())
    return NULL; // was not found

  OdqPropDialogData* pData = itr.value().get();
  if (pItr)
    *pItr = itr;
  return pData;
}

QString OdPropServicesImpl::getPlacementKey(OdqPropertyDialog* pDlg)
{
  ODA_ASSERT_ONCE(pDlg);

  QString qsTitle = pDlg->windowTitle();
  qsTitle.replace(getIApp()->getAppName(), "");
  qsTitle.replace("/", "");
  qsTitle.replace(" ", "");
  while (!qsTitle.indexOf(":") || !qsTitle.indexOf("-"))
    qsTitle = qsTitle.mid(1);

  QString qsPlaceKey = "dlgs";
  if (qsTitle.isEmpty())
    return qsPlaceKey;
  
  qsPlaceKey += "/";
  qsPlaceKey += qsTitle;
  return qsPlaceKey;
}

// return unique dialog key (sDlgKey)
OdString OdPropServicesImpl::openDialog(const OdString& sTitle, // = OD_T("%ls Property Dialog") // %ls = product name
                                        void* pLayoutQObjectToAddDlg, // = NULL
                                        bool bRefreshBySetPropEnd, // = false // via "<event-refresh:setprop_ended>"
                                        TSeparateCategoryTreePlace sepCatPlace, // = enSepCatPlace_None
                                        double dWidthPercentsOfMain, // = 0.75
                                        double dHeightPercentsOfMain) // = 0.5
{
  OdString sDlgKey = getIApp()->toOdString(QUuid::createUuid().toString());
  ODA_ASSERT_ONCE(!getDialogData(sDlgKey));

  OdqMainWindow* pMainWindow = qobject_cast<OdqMainWindow*>(getIApp()->getMainWindow());
  ODA_ASSERT_ONCE(pMainWindow);
  QWidget* pDialogParent = qobject_cast<QWidget*>(pMainWindow);
  QLayout* pLayout = NULL; 
  if (pLayoutQObjectToAddDlg)
  {
    pLayout = qobject_cast<QLayout*>((QObject*) pLayoutQObjectToAddDlg);
    //pDialogParent = pLayout->parentWidget();
    //ODA_ASSERT_ONCE(pDialogParent);
    pDialogParent = NULL; // TODO ? why in else case the content of dialog is invisibles
  }

  OdSharedPtr<OdqPropDialogData> pDlgData = new OdqPropDialogData(pDialogParent, 0, sepCatPlace);
  if (bRefreshBySetPropEnd)
    getIApp()->registerQObject("<event-refresh:setprop_ended>", pDlgData.get());

  QString qsTitle = getIApp()->toQString(sTitle);
  qsTitle.replace("%ls", "%1");
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
  qsTitle = QObject::tr(qsTitle.toAscii().data()).arg(getIApp()->getAppName());
#else
  qsTitle = QObject::tr(qsTitle.toLatin1().data()).arg(getIApp()->getAppName());
#endif

  pDlgData->m_pDialog->setWindowTitle(qsTitle);

  ODA_ASSERT_ONCE(!pDlgData->m_pDialog->testAttribute(Qt::WA_LaidOut));
  if (!pLayout)
  {
    pMainWindow->restoreWidgetPlacement(pDlgData->m_pDialog, getPlacementKey(pDlgData->m_pDialog),
                                        int(pMainWindow->size().width() * dWidthPercentsOfMain),
                                        int(pMainWindow->size().height() * dHeightPercentsOfMain));
    m_stackDlgKeys.push(sDlgKey);
  }
  else
  {
    pLayout->addWidget(pDlgData->m_pDialog);
    ODA_ASSERT_ONCE(pDlgData->m_pDialog->testAttribute(Qt::WA_LaidOut));
  }

  m_mapData[sDlgKey] = pDlgData;
  return sDlgKey;
}

// in perpendicular direction to current (default internal first layout in vertical)
bool OdPropServicesImpl::pushLayout(const OdString& sDlgKey)
{
  OdqPropDialogData* pDlgData = getDialogData(sDlgKey);
  if (!pDlgData)
    return false;

  pDlgData->m_pDialog->pushLayout();
  return true;
}

bool OdPropServicesImpl::popLayout(const OdString& sDlgKey)
{
  OdqPropDialogData* pDlgData = getDialogData(sDlgKey);
  if (!pDlgData)
    return false;

  return pDlgData->m_pDialog->popLayout();
}

bool OdPropServicesImpl::addSource(const OdString& sDlgKey, 
                                   OdPropSource* pSource,
                                   const OdString& sXmlBasePath, // = OdString::kEmpty
                                   const OdString& sXmlRootNodeName, // = OD_T("Properties")
                                   const OdChar* pcszWidgetAttribs, // = NULL // use like "title=:type=palette:type=palette_simple:alignment=top:..."
                                   const OdChar cDelimWidgetAttribs) // = __OD_T(':')
{
  OdqPropDialogData* pDlgData = getDialogData(sDlgKey);
  if (!pSource || !pDlgData)
    return false;

  QWidget* pWidget = NULL;
  int stretch = 0;
  Qt::Alignment alignment = 0;

  if (pcszWidgetAttribs) 
  {
    QStringList asAttribs = getIApp()->toQString(OdString(pcszWidgetAttribs)).split(cDelimWidgetAttribs, QString::SkipEmptyParts);
    foreach (QString qsAttrib, asAttribs)
    {
      QString qsAttribLow = qsAttrib.toLower();
      qsAttribLow.remove(" ");

      if (!qsAttribLow.indexOf("type="))
      {
        if (qsAttribLow.indexOf("palette") > 0) // "type=palette"
        {
          ODA_ASSERT_ONCE(!pWidget);
          OdqPropPaletteView* pView = (qsAttribLow.indexOf("simple") > 0) ? new OdqPropPaletteView()
                                                                          : new OdqStyledPropPaletteView();
          pView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
          pView->setFrameStyle(QFrame::VLine);
          pWidget = pView;
          continue;
        }
        if (qsAttribLow.indexOf("dialog") > 0) // "type=dialog"
        {
          //QTreeView* pView = new QTreeView();
          //pView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
          //pView->setFrameStyle(QFrame::VLine);
          //pWidget = pView;
          continue;
        }
        ODA_FAIL_ONCE(); // TODO
      }

      if (!qsAttribLow.indexOf("title="))
      {
        ODA_ASSERT_ONCE(!pWidget);
      
        QGroupBox* pGroupBox = new QGroupBox();

        qsAttrib = qsAttrib.mid(qsAttrib.indexOf("=") + 1).trimmed();
        if (!qsAttrib.isEmpty())
        {
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
          pGroupBox->setTitle(QObject::tr(qsAttrib.toAscii().data()));
#else
          pGroupBox->setTitle(QObject::tr(qsAttrib.toLatin1().data()));
#endif
        }
        pWidget = pGroupBox;
        continue;
      }

      if (!qsAttribLow.indexOf("alignment="))
      {
        if (qsAttribLow.indexOf("left") > 0)
          alignment |= Qt::AlignLeft;
        else if (qsAttribLow.indexOf("right") > 0)
          alignment |= Qt::AlignRight;
        else if (qsAttribLow.indexOf("top") > 0)
          alignment |= Qt::AlignTop;
        else if (qsAttribLow.indexOf("bottom") > 0)
          alignment |= Qt::AlignBottom;
        else if (qsAttribLow.indexOf("center") > 0)
          alignment |= Qt::AlignCenter;
        else
        {
          ODA_FAIL_ONCE();
        }
        continue;
      }

      ODA_FAIL_ONCE(); // TODO

    } // end foreach
  }
  if (!pWidget)
    pWidget = new QWidget(); // default
  QTreeView* pTreeView = qobject_cast<QTreeView*>(pWidget);

  OdSharedPtr<OdqPropSourceData> pDataSource = new OdqPropSourceData(pSource,
                                                                     sXmlBasePath, sXmlRootNodeName,
                                                                     pDlgData,
                                                                     pTreeView); // TODO for true(default)
  pDataSource->getModel()->setWidget(pWidget);

  pDlgData->m_pDialog->addWidget(pWidget, stretch, alignment); // should be earlier than OdqPropSourceData::updateXml
  if (pTreeView)
    pTreeView->setModel(pDataSource->getModel());

  bool bRes = OdqPropSourceData::updateXml(pDataSource);
  if (bRes)
    pDlgData->m_lstSrcData << pDataSource;
  else
  {
    ODA_FAIL_ONCE(); // test
    pDlgData->m_pDialog->removeWidget(pWidget);
    delete pWidget;
  }

  return bRes;
}

bool OdPropServicesImpl::setMergeFilter(const OdString& sDlgKey, 
                                        OdPropSource* pSource,
                                        const OdString& sMergeFilter, // = OdString::kEmpty // empty for all
                                        const OdString& sMergeFilterAttribName, // =  OD_T("Filter") // empty is already set by previous call
                                        const OdString& sUnmergedBasePath) // = OdString::kEmpty // already set by previous call
{
  OdqPropDialogData* pDlgData = getDialogData(sDlgKey);
  if (!pDlgData)
    return false;

  ODA_ASSERT_ONCE(!pDlgData->m_lstSrcData.isEmpty());
  if (pDlgData->m_lstSrcData.isEmpty())
    return false;
  //ODA_ASSERT_ONCE(pDlgData->m_pDialog->testAttribute(Qt::WA_LaidOut)); // test

  bool bRes = false;
  foreach(OdSharedPtr<OdqPropSourceData> pSrcData, pDlgData->m_lstSrcData)
  {
    ODA_ASSERT_ONCE(!pSrcData.isNull());
    if (!pSrcData->isSourceMatched(pSource))
      continue;

    if (!pSrcData->setMergeFilter(sMergeFilter, sMergeFilterAttribName, sUnmergedBasePath))
      continue;
    bRes = true;
    OdqPropertyPageDialog* pPageDialog = qobject_cast<OdqPropertyPageDialog*>(pDlgData->m_pDialog.get());
    if (!pPageDialog)
      continue;
    OdqPropertyModel* pModel = pSrcData->getModel();
    if (pPageDialog->model() == pModel)
      continue;
    pPageDialog->setModel(pModel); // fillCategoriesTree etc
  }
  ODA_ASSERT_ONCE(bRes);
  return bRes;
}

bool OdPropServicesImpl::refreshSources(const OdString& sDlgKey, 
                                        OdPropSource* pSourceOnly, // = NULL(for all)
                                        bool bRecreateWidgets) // = false
{
  OdqPropDialogData* pDlgData = getDialogData(sDlgKey);
  if (!pDlgData)
    return false;

  ODA_ASSERT_ONCE(!pDlgData->m_lstSrcData.isEmpty());
  if (pDlgData->m_lstSrcData.isEmpty())
    return false;
  // for external update via events 
  // - is used for palettes only (pLayoutQObjectToAddDlg is not NULL)
  //ODA_ASSERT_ONCE(pDlgData->m_pDialog->testAttribute(Qt::WA_LaidOut)); // test

  return pDlgData->refreshSources(pSourceOnly, bRecreateWidgets);
}

bool OdPropServicesImpl::executeDialog(const OdString& sDlgKey)
{
  OdqPropDialogData* pDlgData = getDialogData(sDlgKey);
  if (!pDlgData)
    return false;

  bool bRes = (pDlgData->m_pDialog->exec() == QDialog::Accepted);
  return bRes;
}

bool OdPropServicesImpl::closeDialog(const OdString& sDlgKey)
{
  QMap<OdString, OdSharedPtr<OdqPropDialogData> >::iterator itr;
  OdqPropDialogData* pDlgData = getDialogData(sDlgKey, &itr);
  if (!pDlgData)
    return false;
  getIApp()->unregisterQObject("<event-refresh:setprop_ended>", pDlgData);

  OdqMainWindow* pMainWindow = qobject_cast<OdqMainWindow*>(getIApp()->getMainWindow());
  ODA_ASSERT_ONCE(pMainWindow);

  OdqPropertyDialog* pDialog = pDlgData->m_pDialog;
  pMainWindow->saveWidgetPlacement(pDialog, getPlacementKey(pDlgData->m_pDialog));
  bool bLaidOut = pDialog->testAttribute(Qt::WA_LaidOut);
  //if (bLaidOut && pDialog->testAttribute(Qt::WA_DeleteOnClose))
  //  pDlgData->m_pDialog.detach();
  pDialog->close();

  *itr = NULL; // IPF without it via menu/File/exit aith several opened drawing
  m_mapData.erase(itr);
  
  ODA_ASSERT_ONCE(bLaidOut || sDlgKey == m_stackDlgKeys.top());
  if (!bLaidOut && sDlgKey == m_stackDlgKeys.top())
    m_stackDlgKeys.pop();

  return true;
}

// return index of chosen button
int OdPropServicesImpl::messageBox(const OdChar* pszTitle, // %ls = product name (L"%ls Error")
                                   const OdChar* pszText,
                                   const OdChar* pszButton0, // = NULL // [text_of_button, ... ~text_of_button_in focus, ... text_of_button,] NULL(should be at end - is used as end of arguments)
                                   ...)
{
  enum QMessageBox::Icon icon = QMessageBox::Information;

  QString qsTitle = getIApp()->toQString(pszTitle);
  qsTitle.replace("%ls", "%1");
  QString qsTitleLow = qsTitle.toLower();  
  if (qsTitleLow.indexOf("error") >= 0)
    icon = QMessageBox::Critical;
  else if (qsTitleLow.indexOf("warning") >= 0)
    icon = QMessageBox::Warning;
  else if (qsTitleLow.indexOf("are you") >= 0)
    icon = QMessageBox::Question;
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
  qsTitle = QObject::tr(qsTitle.toAscii().data()).arg(getIApp()->getAppName());
#else
  qsTitle = QObject::tr(qsTitle.toLatin1().data()).arg(getIApp()->getAppName());
#endif
  QString qsText = getIApp()->toQString(pszText);
  qsText.replace("%ls", "%1");
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
  qsText = QObject::tr(qsText.toAscii().data()).arg(getIApp()->getAppName());
#else
  qsText = QObject::tr(qsText.toLatin1().data()).arg(getIApp()->getAppName());
#endif

  QWidget* pParent = getIApp()->getMainWindow();
  if (!m_stackDlgKeys.isEmpty())
  {
    OdqPropDialogData* pDlgData = getDialogData(m_stackDlgKeys.top());
    if (pDlgData)
    {
      pParent = pDlgData->m_pDialog;
      ODA_ASSERT_ONCE(!pDlgData->m_pDialog->testAttribute(Qt::WA_LaidOut));
    }
  }

  QMessageBox msgBox(icon, qsTitle, qsText, QMessageBox::NoButton, pParent);

  va_list argList;
  va_start(argList, pszButton0);
  int index = 0,
      indexFocusButton = -1;
  QList<QPushButton*> lstButtons;
  for (const OdChar* pszButton = pszButton0; pszButton && *pszButton; 
       pszButton = va_arg(argList, const OdChar*), index++)
  {
    QString qsButton = getIApp()->toQString(pszButton);
    int pos = qsButton.indexOf("~");
    if (!pos)
    {
      qsButton = qsButton.mid(1);
      indexFocusButton = index;
    }
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
    qsButton = QObject::tr(qsButton.toAscii().data());
#else
    qsButton = QObject::tr(qsButton.toLatin1().data());
#endif

    // add button
    QPushButton* pButton = msgBox.addButton(qsButton, QMessageBox::ActionRole);
    lstButtons << pButton;
    if (indexFocusButton == index)
      msgBox.setDefaultButton(pButton);
  }
  va_end(argList);
  if (lstButtons.isEmpty())
  {
    QString qsButton = QObject::tr("Close");
    QPushButton* pButton = msgBox.addButton(qsButton, QMessageBox::ActionRole);
    lstButtons << pButton;
  }

  msgBox.exec();

  index = 0;
  foreach(QPushButton* pButton, lstButtons)
  {
    if (pButton == msgBox.clickedButton())
      return index;
    index++;
  }

  // Escape was pressed
  return -1;
}

OdDbBaseHostAppServices* OdPropServicesImpl::getBaseHostAppServices()
{
  return getIApp()->getBaseHostAppServices();
}

OdString OdPropServicesImpl::tr(const char* pcszText) // translate via Qt Dictionaties (via QObject::tr(pcszText))
{
  if (!pcszText || !*pcszText)
    return OdString::kEmpty;

  OdString sTranslated = getIApp()->toOdString(QObject::tr(pcszText));
  return sTranslated;
}

bool OdPropServicesImpl::isInputStringReady(OdEdCommandContext* pCmdCtx)
{
  return getIConsole()->isInputStringReady(pCmdCtx);
}

//////////////////////////////////////////////////////////////////////////

class OdqResBufPropWrapper
{
  typedef enum {
      fo_None // undef
    , fo_And  // default
    , fo_Or   // []
    // ...
  } TFindOperation;
  bool doFindOperation(TFindOperation operation, 
                       QString& qsGrCodeExp, // in / out
                       int nLevel,
                       bool bParseOnly = false, // skip real find if true
                       bool bCreateNewIfAbsent = false,
                       const QString& qsHintGrCodeToCreateNew = "");

public:
  OdqResBufPropWrapper(const OdResBufPtr& data)
    : m_pData(*const_cast<OdResBufPtr*>(&data))
    , m_pReadOnly(true)
  {
  }
  OdqResBufPropWrapper(OdResBufPtr& data)
    : m_pData(data)
    , m_pReadOnly(false)
  {
  }

  void checkMap()
  {
    if (!m_mapValues.isEmpty())
      return;

    QString qsClassName,
            qsSubName;
    bool isEmbeddedObject = false,
         isContextData = false;
    for (OdResBufPtr pResBuf = m_pData; pResBuf.get(); pResBuf = pResBuf->next())
    {
      enum OdResBuf::ValueType code = (enum OdResBuf::ValueType) pResBuf->restype();
      if (code == OdResBuf::kDxfEmbeddedObjectStart) // 101
      {
        ODA_ASSERT_ONCE(!isEmbeddedObject);
        isEmbeddedObject = true; // INT-6265
      }
      if (   code == OdResBuf::kDxfSubclass // 100
          || code == OdResBuf::kDxfEmbeddedObjectStart) // 101
      {
        QString qsName = getIApp()->toQString(pResBuf->getString());
        ODA_ASSERT_ONCE(   !qsName.isEmpty() 
                        && (   qsName != qsClassName 
                            || qsName == "AcDbText")); // TOCHECK later
        qsClassName = qsName;
        m_mapValues[qsClassName] = pResBuf; // first - used in findNext for alternative
      }

      if (isEmbeddedObject)
      {
        if (int(code) <= 0)
          isEmbeddedObject = false;
        continue; // INT-6265
      }
      if (   !isContextData 
          && code == OdResBuf::kDxfXTextString // 300
          && pResBuf->getString() == L"CONTEXT_DATA{")
      {
        isContextData = true;
        qsSubName = "CONTEXT_DATA";
        m_mapValues[qsClassName + ":" + qsSubName] = pResBuf;
        continue;
      }
      if (   isContextData 
          && code == 301 // unnamed now
          && pResBuf->getString() == L"}")
      {
        isContextData = false;
        qsSubName.clear();
        continue;
      }

      if (qsClassName.isEmpty())
        continue;

      ODA_ASSERT_ONCE(int(code) > 0);
      if (int(code) <= 0)
        continue;

      QString qsKey = QString("%1:").arg(qsClassName);
      m_mapValues[qsKey] = pResBuf; // last - used to add new overrides
      qsKey = qsClassName;
      if (!qsSubName.isEmpty())
        qsKey += ":" + qsSubName;
      qsKey += QString(":%1").arg(code);
      if (m_mapValues.find(qsKey) != m_mapValues.end())
        continue; // the first will be found through map

      m_mapValues[qsKey] = pResBuf;
    }
  }

  static QString getFirstParsed(QString& qsGrCodeExp, QString& qsClassName)
  {
    qsGrCodeExp = qsGrCodeExp.trimmed();
    QString qsFirst,
            qsSpecials = "[]*+? "; // non or more times, one or more time, any group code, separator
    int pos = 0, 
        sz = qsGrCodeExp.length();
   
    for (; pos < sz; pos++)
    {
      if (qsSpecials.indexOf(qsGrCodeExp.data()[pos]) < 0)
        continue;
      if (!pos)
        pos++;

      qsFirst = qsGrCodeExp.left(pos);
      qsGrCodeExp = qsGrCodeExp.mid(pos).trimmed();
      break;
    }
    if (qsFirst.isEmpty())
    {
      qsFirst = qsGrCodeExp;
      qsGrCodeExp.clear();
    }
    if ((pos = qsFirst.indexOf(':')) >= 0)
    {
      ODA_ASSERT_ONCE(pos);
      qsClassName = qsFirst.left(pos++);
      if (qsClassName.indexOf("AcDb") < 0)
        qsClassName = "AcDb" + qsClassName;
      qsFirst = qsFirst.mid(pos).trimmed();
    }
    return qsFirst;
  }

  bool findClassName(const QString& qsClassName)
  {
    m_qsCurrentClassName.clear();
    m_pCurrent = NULL;

    ODA_ASSERT_ONCE(!qsClassName.isEmpty());
    if (qsClassName.isEmpty())
      return false;

    checkMap();
    QHash<QString, OdResBufPtr>::iterator itr = m_mapValues.find(qsClassName + ":");
    if (itr == m_mapValues.end())
    {
      ODA_FAIL_ONCE();
      return false;
    }

    m_pCurrent = itr.value();
    m_qsCurrentClassName = qsClassName;
    return true;
  }

  bool find(const QString& cqsGrCodeExp, 
            bool bCreateNewIfAbsent = false,
            const QString& qsHintGrCodeToCreateNew = "") // other alternative to it will be removed if not empty
  {
    ODA_ASSERT_ONCE(!m_qsCurrentClassName.isEmpty());
    if (m_qsCurrentClassName.isEmpty() || cqsGrCodeExp.isEmpty())
      return false;
    QString qsClassName = m_qsCurrentClassName;
    m_pCurrent = NULL;

    QString qsGrCodeExp = cqsGrCodeExp,
            qsCode = getFirstParsed(qsGrCodeExp, qsClassName);
    if (!qsGrCodeExp.isEmpty())
    {
      if (!doFindOperation(fo_And, (qsGrCodeExp = cqsGrCodeExp), 
                           0, false, bCreateNewIfAbsent, qsHintGrCodeToCreateNew))
        return false;                           ;
      ODA_ASSERT_ONCE(!m_pCurrent.isNull() && qsGrCodeExp.isEmpty());
      return !m_pCurrent.isNull(); // true
    }
    ODA_ASSERT_ONCE(!qsCode.isEmpty());

    QString qsKey = QString("%1:%2").arg(qsClassName).arg(qsCode);
    QHash<QString, OdResBufPtr>::iterator itr = m_mapValues.find(qsKey);
    if (itr == m_mapValues.end())
    {
      if (!bCreateNewIfAbsent)
        return false;
      itr = m_mapValues.find(qsClassName + ":");
      if (itr == m_mapValues.end())
      {
        ODA_FAIL_ONCE();
        return false;
      }
      ODA_ASSERT_ONCE(qsGrCodeExp.isEmpty()); // TODO - unsupported now
      if (!qsGrCodeExp.isEmpty())
        return false;

      OdResBufPtr pResBufPrev = itr.value(),
                  pResBufNext = itr.value()->next();
      for (; pResBufNext.get(); pResBufNext = pResBufNext->next())
      {
        if (pResBufNext->restype() == OdResBuf::kDxfSubclass) // 100
          break;
        pResBufPrev = pResBufNext;
      }

      bool isOk = false;
      OdResBuf::ValueType code = (OdResBuf::ValueType) qsCode.toUInt(&isOk);\
      ODA_ASSERT_ONCE(isOk);
      if (!isOk)
        return false;
      ODA_ASSERT_ONCE(pResBufPrev.get() && pResBufPrev->next() == pResBufNext);

      OdResBufPtr pResBuf = OdResBuf::newRb(code); 
      pResBufPrev->setNext(pResBuf);
      if (pResBufNext.get())
        pResBuf->setNext(pResBufNext);

      m_mapValues[qsKey] = pResBuf;
      m_pCurrent = pResBuf;
      return true;
    }

    m_pCurrent = itr.value();
    ODA_ASSERT_ONCE(qsGrCodeExp.isEmpty());
    return true;
  }

  bool findNext(const QString& qsGrCode, 
                bool bCreateNewIfAbsent = false,
                bool bStrongTheNextOnly = false)
  {
    if (m_qsCurrentClassName.isEmpty() || m_pCurrent.isNull())
    {
      ODA_FAIL_ONCE(); // test
      return false;
    }
    bool bOk = false;
    OdResBuf::ValueType code = (OdResBuf::ValueType) qsGrCode.toInt(&bOk);
    if (!bOk || code == OdResBuf::kDxfSubclass)
    {
      ODA_FAIL_ONCE();
      return false;
    }

    OdResBufPtr pResBuf = m_pCurrent->next();
    if (!bStrongTheNextOnly)
    {
      while (   !pResBuf.isNull() 
             && pResBuf->restype() != OdResBuf::kDxfSubclass
             && code != pResBuf->restype())
        pResBuf = pResBuf->next();
    }
    if (pResBuf.isNull() || code != pResBuf->restype())
    {
      if (!bCreateNewIfAbsent)
        return false;

      pResBuf = OdResBuf::newRb(code); 
      if (m_pCurrent->next().get())
        pResBuf->setNext(m_pCurrent->next());
      m_pCurrent->setNext(pResBuf);

      QString qsKey = QString("%1:%2").arg(m_qsCurrentClassName).arg(code);
      m_mapValues[qsKey] = pResBuf;
    }

    m_pCurrent = pResBuf;
    return !m_pCurrent.isNull();
  }

  bool removeNext(const QString& qsGrCode,
                  bool bStrongTheNextOnly = false,
                  OdResBufPtr* ppPrev = NULL)
  {
    ODA_ASSERT_ONCE(m_qsCurrentClassName.isEmpty() || !m_pCurrent.isNull()); // test
    if (m_qsCurrentClassName.isEmpty() || m_pCurrent.isNull())
      return false;
    bool bOk = false;
    OdResBuf::ValueType code = (OdResBuf::ValueType) qsGrCode.toInt(&bOk);
    if (!bOk || code == OdResBuf::kDxfSubclass)
    {
      ODA_FAIL_ONCE();
      return false;
    }

    OdResBufPtr pResBufPrev = m_pCurrent,
                pResBuf = m_pCurrent->next(); 
    if (!bStrongTheNextOnly)
    {
      while (   !pResBuf.isNull() 
             && pResBuf->restype() != OdResBuf::kDxfSubclass 
             && code != pResBuf->restype())
      {
        pResBufPrev = pResBuf;
        pResBuf = pResBufPrev->next();
      }
    }
    if (pResBuf.isNull() || code != pResBuf->restype())
      return false;

    pResBufPrev->setNext(pResBuf->next());
    pResBuf->setNext(NULL);

    QString qsKey = QString("%1:%2").arg(m_qsCurrentClassName).arg(code);
    m_mapValues.erase(m_mapValues.find(qsKey));

    if (ppPrev)
      *ppPrev = pResBufPrev; 
    return true;
  }

  QString code()
  {
    ODA_ASSERT_ONCE(!m_pCurrent.isNull());
    if (m_pCurrent.isNull()) 
      return "";
    return QString::number(m_pCurrent->restype());
  }

  QString value()
  {
    ODA_ASSERT_ONCE(!m_pCurrent.isNull());
    if (m_pCurrent.isNull()) 
      return "";
    
    OdString sValue;
    //via OdVariant or OdRxVariantValue
    OdDxfCode::Type type = OdDxfCode::_getType(m_pCurrent->restype());
    switch (type)
    {
    case OdDxfCode::String:
    case OdDxfCode::Name:
    case OdDxfCode::Handle:
    case OdDxfCode::LayerName:
      sValue = m_pCurrent->getString();
      break;
    case OdDxfCode::Bool:
      sValue = OdqXmlUtils::toString(OdVariant(m_pCurrent->getBool()));
      break;
    case OdDxfCode::Integer8:
      sValue = OdqXmlUtils::toString(OdVariant(m_pCurrent->getInt8()));
      break;
    case OdDxfCode::Integer16:
      sValue = OdqXmlUtils::toString(OdVariant(m_pCurrent->getInt16()));
      break;
    case OdDxfCode::Integer32:
      sValue = OdqXmlUtils::toString(OdVariant(m_pCurrent->getInt32()));
      break;
    case OdDxfCode::Integer64:
      sValue = OdqXmlUtils::toString(OdVariant(m_pCurrent->getInt64()));
      break;
    case OdDxfCode::Double:
    case OdDxfCode::Angle:
      sValue = OdqXmlUtils::toString(OdVariant(m_pCurrent->getDouble()));
      break;
    case OdDxfCode::Point:
      {
        const OdGePoint3d& point = m_pCurrent->getPoint3d();
        //sValue.format(OD_T("%lg,%lg,%lg"), point.x, point.y, point.z);
        sValue = getIApp()->toOdString(QString("%1,%2,%3").arg(point.x).arg(point.y).arg(point.z));
      }
      break;
  //case OdDxfCode::BinaryChunk:
  //  sValue = OdqXmlUtils::toString(OdVariant(m_pCurrent->getBinaryChunk()));
  //  break;
    case OdDxfCode::ObjectId:
    case OdDxfCode::SoftPointerId:
    case OdDxfCode::HardPointerId:
    case OdDxfCode::SoftOwnershipId:
    case OdDxfCode::HardOwnershipId:
      sValue.format(L"%" PRIX64W, (OdUInt64) m_pCurrent->getHandle());
      ODA_ASSERT_ONCE(   sValue 
                      == OdqXmlUtils::toString(OdVariant((OdUInt64) m_pCurrent->getHandle()), true));
      break;
    default:
      ODA_FAIL_ONCE();
      break;
    }
  
    QString qsValue = getIApp()->toQString(sValue);
    return qsValue;
  }

  bool setValue(const QString& qsValue)
  {
    ODA_ASSERT_ONCE(!m_qsCurrentClassName.isEmpty() && !m_pCurrent.isNull());
    if (m_pCurrent.isNull()) 
      return false;
    OdString sValue = getIApp()->toOdString(qsValue);

    OdDxfCode::Type type = OdDxfCode::_getType(m_pCurrent->restype());
    switch (type)
    {
    case OdDxfCode::String:
    case OdDxfCode::Name:
    case OdDxfCode::Handle:
    case OdDxfCode::LayerName:
      m_pCurrent->setString(sValue);
      return true;
    case OdDxfCode::Bool:
      m_pCurrent->setBool(OdqXmlUtils::toBool(OdVariant(sValue)));
      return true;
    case OdDxfCode::Integer8:
      m_pCurrent->setInt8((OdInt8) OdqXmlUtils::toInt(OdVariant(sValue)));
      return true;
    case OdDxfCode::Integer16:
      m_pCurrent->setInt16((OdInt16) OdqXmlUtils::toInt(OdVariant(sValue)));
      return true;
    case OdDxfCode::Integer32:
      m_pCurrent->setInt32((OdInt32) OdqXmlUtils::toInt(OdVariant(sValue)));
      return true;
    case OdDxfCode::Integer64:
      m_pCurrent->setInt64((OdInt64) OdqXmlUtils::toInt(OdVariant(sValue)));
      return true;
    case OdDxfCode::Double:
    case OdDxfCode::Angle:
      m_pCurrent->setDouble(OdqXmlUtils::toDouble(OdVariant(sValue)));
      return true;
    case OdDxfCode::Point:
      {
        OdGePoint3d point = m_pCurrent->getPoint3d();
        bool bOk = false;
        int index = -1;
        foreach (QString qsValue, getIApp()->toQString(sValue).split(',', QString::KeepEmptyParts))
        {
          if (++index > 2)
          {
            ODA_FAIL_ONCE(); // internal error (catching upper)
            return false;
          }
          qsValue = qsValue.trimmed();
          if (qsValue.isEmpty())
            continue;
          double& dCoord = (!index ? point.x : (index == 1 ? point.y : point.z));
          dCoord = qsValue.toDouble(&bOk);
          ODA_ASSERT_ONCE(bOk); // invalid input format (should be caught ealier via diagnostic)
          if (!bOk)
            return false;
        }
        if (!bOk)
          return false; // no changes
        m_pCurrent->setPoint3d(point);
        return true;
      }
    case OdDxfCode::ObjectId:
    case OdDxfCode::SoftPointerId:
    case OdDxfCode::HardPointerId:
    case OdDxfCode::SoftOwnershipId:
    case OdDxfCode::HardOwnershipId:
      // OdRxObject* baseDatabase
      m_pCurrent->setHandle(OdDbHandle(sValue));
      return true;
    }

    ODA_FAIL_ONCE(); // TODO
    return false;
  }

private:
  bool m_pReadOnly;
  OdResBufPtr m_pData;
  OdResBufPtr m_pCurrent;
  QString m_qsCurrentClassName;
  // Therefore we cam mapped all elements at start.
  QHash<QString, OdResBufPtr> m_mapValues;

  Q_DISABLE_COPY(OdqResBufPropWrapper)
};

bool OdqResBufPropWrapper::doFindOperation(TFindOperation operation, 
                                           QString& qsGrCodeExp, // in / out
                                           int nLevel,
                                           bool bParseOnly, // = false // skip real find if true
                                           bool bCreateNewIfAbsent, // = false
                                           const QString& qsHintGrCodeToCreateNew) // = ""
{
  ODA_ASSERT_ONCE(!m_qsCurrentClassName.isEmpty());
  if (m_qsCurrentClassName.isEmpty())
    return false;
  QString qsClassName = m_qsCurrentClassName;

  bool bValue = true;
  if (operation == fo_Or)
    bValue = false;

  QString qsSvGrCodeExp = qsGrCodeExp, // TODO for repeat
          qsCode;

  // data to remove alternative gr codes (!= qsHintGrCodeToCreateNew)
  // in latest fo_Or group [alterbative_code ... hint_code ... alterbative_code]
  QStringList lstUnconfirmedCodesToRemove; // will be removed at end if isConfirmedListToRemoveViaFoundedHint is true
  bool isConfirmedListToRemoveViaFoundedHint = false;
  OdResBufPtr pSvPrevToRemoved = m_pCurrent;

  while (!(qsCode = getFirstParsed(qsGrCodeExp, qsClassName)).isEmpty())
  {
    if (qsCode.length() == 1)
    {
      switch (qsCode.at(0).
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
          toAscii())
#else
          toLatin1())
#endif
      {
      case '[':
        {
          bool bParseOnlyLocal = bParseOnly;
          if (operation == fo_And && !bValue)
            bParseOnlyLocal = true;

          bool bValueLocal = doFindOperation(fo_Or, qsGrCodeExp, nLevel + 1, 
                                             bParseOnlyLocal, bCreateNewIfAbsent, qsHintGrCodeToCreateNew);
          if (bParseOnlyLocal)
            continue;
          switch (operation)
          {
          case fo_And:
            if (bValue && !bValueLocal)
            {
              bValue = false;
              bParseOnly = true;
            }
            break;
          case fo_Or:
            if (!bValue && bValueLocal)
            {
              bValue = true;
              bParseOnly = true;
            }
            break;
          }
        }
        continue;
      case ']':
        ODA_ASSERT_ONCE(operation == fo_Or);
        if (operation != fo_Or)
          return false;
        if (!qsHintGrCodeToCreateNew.isEmpty())
        {
          ODA_ASSERT_ONCE(   !isConfirmedListToRemoveViaFoundedHint // test
                          || qsGrCodeExp.isEmpty()); 
          if (qsGrCodeExp.isEmpty())
          {
            if (bValue && code() == qsHintGrCodeToCreateNew)
              return bValue;
            ODA_ASSERT_ONCE(!bParseOnly && isConfirmedListToRemoveViaFoundedHint);

            OdResBufPtr pSvPrev = pSvPrevToRemoved;
            if (isConfirmedListToRemoveViaFoundedHint)
            {
              foreach(QString qsCodeToRemove, lstUnconfirmedCodesToRemove)
              {
                if ((m_pCurrent = pSvPrevToRemoved).isNull())
                {
                  ODA_ASSERT_ONCE(m_mapValues.find(m_qsCurrentClassName) != m_mapValues.end());

                  m_pCurrent = m_mapValues.find(m_qsCurrentClassName).value();
                  ODA_ASSERT_ONCE(!m_pCurrent.isNull());
                }
                removeNext(qsCodeToRemove, false, &pSvPrev);
              }
            }

            if ((m_pCurrent = pSvPrev).isNull())
              bValue = find(qsHintGrCodeToCreateNew, bCreateNewIfAbsent);
            else
              bValue = findNext(qsHintGrCodeToCreateNew, bCreateNewIfAbsent);
          }
        }
        return bValue;

      case '*': // non or more times
      case '+': // one or more time
      case '?': // any group code
        ODA_FAIL_ONCE(); // TODO
        return false;
      }
    }

    if (bParseOnly)
      continue;
     
    bool bValueLocal = false;
    switch (operation)
    {
    case fo_And:
      if (m_pCurrent.isNull())
        bValueLocal = find(qsCode, bCreateNewIfAbsent);
      else 
        bValueLocal = findNext(qsCode, bCreateNewIfAbsent);
      if (bValue && !bValueLocal)
      {
        bValue = false;
        bParseOnly = true;
      }
      break;
    case fo_Or:
      if (   bCreateNewIfAbsent && !isConfirmedListToRemoveViaFoundedHint 
          && qsCode == qsHintGrCodeToCreateNew)
        isConfirmedListToRemoveViaFoundedHint = true;
      if (m_pCurrent.isNull())
        bValueLocal = find(qsCode);
      else
        bValueLocal = findNext(qsCode);
      if (bValueLocal)
      {
        if (!bValue)
          bValue = true;
        if (qsHintGrCodeToCreateNew.isEmpty() || qsCode == qsHintGrCodeToCreateNew)
          bParseOnly = true;
        else
          lstUnconfirmedCodesToRemove << qsCode;
      }
      break;
    } // end switch

  } // end while

  return bValue;
}

//////////////////////////////////////////////////////////////////////////

static OdResBufPtr getResBuf(OdDbStub* id)
{
  OdResBufPtr pResBuf;

  OdRxObject* pRxDb = baseDatabaseBy(id);
  if (OdDbDatabase::cast(pRxDb).isNull())
    return pResBuf; // DGN & others

  OdDbObjectPtr pObject = OdDbObjectId(id).openObject();
  ODA_ASSERT_ONCE(!pObject.isNull());
  if (pObject.isNull())
    return pResBuf;

  pResBuf = oddbEntGet(pObject); // , L"*);
  ODA_ASSERT_ONCE(!pResBuf.isNull());
  if (pResBuf.isNull())
    return pResBuf;

  return pResBuf;
}

static bool setResBuf(OdDbStub* id, OdResBuf* pData)
{
  OdRxObject* pRxDb = baseDatabaseBy(id);
  if (OdDbDatabase::cast(pRxDb).isNull())
    return false; // DGN & others

  OdDbObjectPtr pObject = OdDbObjectId(id).openObject(OdDb::kForWrite);
  ODA_ASSERT_ONCE(!pObject.isNull());
  if (pObject.isNull())
    return false;

  if (oddbEntMod(pObject, pData) != eOk)
  {
    ODA_FAIL_ONCE();
    return false;
  }

  return true;
}

void OdPropServicesImpl::registerXmlProp(const OdString& sClassName, OdXmlPropCreateFunction* pFunc)
{
  ODA_ASSERT_ONCE(   !sClassName.isEmpty() && pFunc
                  && m_mapXmlPropCreatorsByClassName.find(sClassName) == m_mapXmlPropCreatorsByClassName.end()); // test
  m_mapXmlPropCreatorsByClassName[sClassName] = pFunc;
}

void OdPropServicesImpl::unregisterXmlProp(const OdString& sClassName)
{
  ODA_ASSERT_ONCE(   !sClassName.isEmpty() 
                  && m_mapXmlPropCreatorsByClassName.find(sClassName) != m_mapXmlPropCreatorsByClassName.end()); // test
  m_mapXmlPropCreatorsByClassName[sClassName] = NULL;
}

bool OdPropServicesImpl::isRegisteredXmlProp(const OdString& sClassName)
{
  QMap<OdString, OdXmlPropCreateFunction*>::iterator itr = m_mapXmlPropCreatorsByClassName.find(sClassName);
  return itr != m_mapXmlPropCreatorsByClassName.end();
}

OdPropBasePtr OdPropServicesImpl::createXmlPropByClassName(const OdString& sClassName,
                                                           int propSrcFlags,
                                                           OdDbStub* id, 
                                                           OdRxObject* pRxDb) // = NULL
{
  QMap<OdString, OdXmlPropCreateFunction*>::iterator itr = m_mapXmlPropCreatorsByClassName.find(sClassName);
  OdPropBasePtr pPropSrc;
  if (itr == m_mapXmlPropCreatorsByClassName.end())
    return pPropSrc;
  if (OdXmlPropCreateFunction* pFunc = itr.value())
  {
    pPropSrc = (*pFunc)();
    ODA_ASSERT_ONCE(pPropSrc.get());
    if (id)
    {
      pPropSrc->setId(id, pRxDb);
      ODA_ASSERT_ONCE(!pRxDb || pRxDb == pPropSrc->baseDatabase());
    }
    else
    {
      ODA_ASSERT_ONCE(pRxDb);
      pPropSrc->setRxDb(pRxDb);
    }
    pPropSrc->setPropSrcFlags(propSrcFlags);
    ODA_ASSERT_VAR(pPropSrc->m_sRegistredForClassName = sClassName;)
  }
  return pPropSrc;
}

OdString OdPropServicesImpl::collectXmlProps(const OdString& sXmlBasePath,
                                             OdDbStub* id, const OdStringArray& asClassNames, 
                                             int flagsToUse, OdRxObject* pRxDb) // = NULL
{
  ODA_ASSERT_ONCE(!sXmlBasePath.isEmpty() && !asClassNames.isEmpty()); // test
  if (sXmlBasePath.isEmpty() || asClassNames.isEmpty())
    return OdString::kEmpty;

  OdString sXmlLines;

  // via special registered interfaces
  OdPropBasePtr pPropSrc;
  if (   GETBIT(flagsToUse, OdPropBase::enPropSrcBased_Cda)
           // PROD-143 // wrapper existance of upper class (the last in list) is more priority than CDA
      && (   !id || !GETBIT(flagsToUse, OdPropBase::enPropSrcBased_RegistredXmlSrc)
          || !isRegisteredXmlProp(asClassNames.last())))
  {
    pPropSrc = createXmlPropByClassName(L"CdaBased", flagsToUse, id, pRxDb);
    ODA_ASSERT_ONCE(pPropSrc.get());
    if (pPropSrc.get() && !pPropSrc->updateXml(sXmlLines))
    {
      ODA_FAIL_ONCE(); // test
    }
    return sXmlLines;
  }
  if (   !GETBIT(flagsToUse, OdPropBase::enPropSrcBased_RegistredXmlSrc)
      && (!GETBIT(flagsToUse, OdPropBase::enPropSrcBased_XmlPattern) || !id))
    return sXmlLines;

  OdResBufPtr data;
  for (OdUInt32 index = 0, numClasses = asClassNames.size(); index < numClasses; index++)
  {
    OdString sClassName = asClassNames.getAt(index);
    ODA_ASSERT_ONCE(!sClassName.isEmpty());
    if (sClassName.isEmpty())
      continue;

    // via registered interfaces
    pPropSrc = NULL;
    if (GETBIT(flagsToUse, OdPropBase::enPropSrcBased_RegistredXmlSrc))
      pPropSrc = createXmlPropByClassName(sClassName, flagsToUse, id, pRxDb);
    if (pPropSrc.get())
    {
      // more priority than way via patterns
      if (!pPropSrc->updateXml(sXmlLines))
      {
        ODA_FAIL_ONCE(); // test
      }
      continue;
    }

    // via XML patterns

    if (!GETBIT(flagsToUse, OdPropBase::enPropSrcBased_XmlPattern) || !id)
      continue;
    QString qsClassName = getIApp()->toQString(sClassName);
    OdSharedPtr<OdqXmlUtils>& wrpUtils = loadXmlPattern(qsClassName);
    if (wrpUtils.isNull())
      continue; // is absent
    if (data.isNull())
      data = getResBuf(id); // read an entity as a list of resbufs - for use in findGroupCode()
    if (data.isNull())
      continue;
    OdqResBufPropWrapper dataWrp(data);
    if (!dataWrp.findClassName(qsClassName))
    {
      ODA_FAIL_ONCE();
      continue;
    }
    sXmlLines += getIApp()->toOdString(collectByPatternUtils(sXmlBasePath, *wrpUtils.get(), dataWrp));
  }
  return sXmlLines;
}

QString OdPropServicesImpl::collectByPatternUtils(const OdString& sXmlBasePath,
                                                  OdqXmlUtils& utils, OdqResBufPropWrapper& data) // in 
{
  ODA_ASSERT_ONCE(!sXmlBasePath.isEmpty());

  QStringList lstKeys = utils.keys();
  if (lstKeys.isEmpty())
    return "";
  QStringList lstAllLines,
              lstCatLines;
  QString qsPathFrom,
          qsPathTo,
          qsCurrentPath;
  QHash<QString, bool> mapHasKids;

  foreach (QString qsPathName, lstKeys)
  {
    QDomElement elemProp = utils.getElement(qsPathName);
    ODA_ASSERT_ONCE(!elemProp.isNull());

    QString qsGrCodeExp = elemProp.attribute("GrCode"),
            qsPath = elemProp.attribute("Path"),
            qsName = elemProp.attribute("Name");
    ODA_ASSERT_ONCE(!qsName.isEmpty());
    if (qsName.isEmpty())
      continue;

    if (qsPathTo.isEmpty())
    {
      ODA_ASSERT_ONCE(qsGrCodeExp.isEmpty());
      qsPathFrom = qsPath;
      qsPathTo = sXmlBasePath;
    }
    // attune path to sXmlBasePath
    int lenPathFrom = qsPathFrom.length();
    if (lenPathFrom && qsPath.indexOf(qsPathFrom))
    {
      ODA_FAIL_ONCE(); // invalid
      continue;
    }
    if (lenPathFrom) 
      qsPath = qsPathTo + qsPath.mid(lenPathFrom);
    else if (!qsPath.isEmpty())
      qsPath = qsPathTo + "/" + qsPath;
    else
      qsPath = qsPathTo;
    if (qsCurrentPath.isEmpty())
      qsCurrentPath = qsPath;

    if (qsPath != qsCurrentPath)
    {
      bool isKidOfCurrent = !qsPath.indexOf(qsCurrentPath);
      // skip an empty category (without kids)
      if (   lstCatLines.size() > 1 || isKidOfCurrent 
          || (lstCatLines.size() == 1 && lstCatLines.first().indexOf(" Type=\"category\"") < 0)  // is not category
          || mapHasKids.find(qsCurrentPath) != mapHasKids.end()) 
      {
        lstAllLines << lstCatLines; 
        mapHasKids[qsCurrentPath] = true;
      }
      lstCatLines.clear();
      qsCurrentPath = qsPath;
    } 

    QString qsType = elemProp.attribute("Type", qsGrCodeExp.isEmpty() ? "category" : "string");
    ODA_ASSERT_ONCE(!qsType.isEmpty());
    
    bool isCategory = (qsType == "category");
    if (!isCategory && lstCatLines.isEmpty() && lstAllLines.isEmpty())
      continue; // named category is absent

    //  [ <Property ]   Path="..." Name="..." Type="..." Value   [ />\n ]
    QString qsLine; // without [] parts
    QStringList lstSkipAttribs;
    lstSkipAttribs << "grcode";

    qsLine += QString(" Path=\"%1\"").arg(qsPath);
    lstSkipAttribs << "path";
    qsLine += QString(" Name=\"%1\"").arg(qsName);
    lstSkipAttribs << "name";

    IqAppProps::TPropType type = OdqXmlUtils::toType(qsType);
    if (isCategory && type != IqAppProps::prpCategory)
    {
      if (type == IqAppProps::prpOdColor)
      {
        qsGrCodeExp = "62"; // OdResBuf::kDxfColor
        isCategory = false;
      }
      else
      {
        ODA_FAIL_ONCE(); // test
        qsType = "category";
        type = OdqXmlUtils::toType(qsType);
      }
    }
    switch (type)
    {
    case IqAppProps::prpNone:
    case IqAppProps::prpImage:
    case IqAppProps::prpGrid:
    case IqAppProps::prpTree:
    case IqAppProps::prpPalette:
    case IqAppProps::prpFileDialog:
    case IqAppProps::prpReference:
      ODA_FAIL_ONCE(); // unsupported
      continue;
    }
    qsLine += QString(" Type=\"%1\"").arg(qsType);
    lstSkipAttribs << "type";

    if (!isCategory)
    {
      QString qsVal = elemProp.attribute("Value");
      bool isOk = false;
      if (data.find(qsGrCodeExp))
      {
        QString qsFmt = qsVal,
                qsCode = data.code();
        qsVal = data.value();

        switch (type)
        {
        case IqAppProps::prpOdColor:
          {
            ODA_ASSERT_ONCE(qsGrCodeExp == "62"); // test OdResBuf::kDxfColor
            // qsVal is colorIndex
            OdCmEntityColor color = OdqPropertyModel::OdColorByString(qsVal, &isOk);
            ODA_ASSERT_ONCE(isOk);
            // gr code 420 & 430
            if (isOk && data.findNext("420", false, true))
            {
              OdUInt32 clr = data.value().toUInt(&isOk); 
              clr |= (OdUInt32) OdCmEntityColor::kByColor << 24; // clr |= 0xC2000000; //sets kByColor
              color.setColor(clr);
              // the next is useless for us
              //if (data.find("430"))
              //{
              //  OdCmColor clr;
              //  if (clr.setNamesFromDictionaryKey(getIApp()->toOdString(data.value()))
              //    //pColor->setColor(clr);
              //}
            }

            qsVal = OdqPropertyModel::stringByOdColor(color);
            ODA_ASSERT_ONCE(!qsVal.isEmpty() && isOk);
            break;
          }
        //case ... : // TODO
        } // switch

        if (qsFmt.indexOf("%") >= 0) // with argument (handle for example)
          qsVal = qsFmt.arg(qsVal).arg(qsCode);
      }
      else
      {
        if (qsVal.indexOf("%") >= 0)
          qsVal.remove("%1").remove("%2");
        ODA_ASSERT_ONCE(!qsVal.isEmpty());
        switch (type)
        {
        case IqAppProps::prpOdColor:
          ODA_ASSERT_ONCE(!qsVal.isEmpty());
          if (qsVal.isEmpty())
          {
            qsVal = "ByLayer";
            break;
          }
          // normalize
          qsVal = OdqPropertyModel::stringByOdColor(OdqPropertyModel::OdColorByString(qsVal, &isOk));
          ODA_ASSERT_ONCE(isOk);
          break;
        }
      }
    
      qsLine += QString(" Value=\"%1\"").arg(qsVal);
      lstSkipAttribs << "value";
    }

    QDomNamedNodeMap attrs = elemProp.attributes();
    for (uint idx = 0; idx < (uint) attrs.length(); idx++)
    {
      QDomAttr attr = attrs.item(idx).toAttr();
      ODA_ASSERT_ONCE(!attr.isNull())
      if (attr.isNull())
        continue;
      QString qsName = attr.name(),
              qsValue = attr.value();
      ODA_ASSERT_ONCE(!qsName.isEmpty());
      if (qsName.isEmpty() || lstSkipAttribs.contains(qsName.toLower()))
        continue;

      qsLine += QString(" %1=\"%2\"").arg(qsName).arg(qsValue);
    }

    lstCatLines << qsLine;
  }
  // skip an empty category (without kids)
  if (   lstCatLines.size() > 1
      || (lstCatLines.size() == 1 && lstCatLines.first().indexOf(" Type=\"category\"") < 0) // is not category
      || mapHasKids.find(qsCurrentPath) != mapHasKids.end())
    lstAllLines << lstCatLines;

  QString qsXml;
  foreach (QString qsLine, lstAllLines)
  {
    ODA_ASSERT_ONCE(!qsLine.isEmpty());

    //  <Property Path="..." Name="..." Type="..." ... />
    qsXml += QString("<Property%1 />\n").arg(qsLine);
  }
  return qsXml;
}

bool OdPropServicesImpl::getPossibleValuesXmlProps(OdDbStub* id, const OdStringArray& asClassNames,
                                                   const OdString& sPathName, 
                                                   OdStringArray& values, OdStringArray& iconPaths, 
                                                   int flagsToUse, OdRxObject* pRxDb) // = NULL
{
  ODA_ASSERT_ONCE(!asClassNames.isEmpty() && !sPathName.isEmpty()); // test

  OdString sXmlLines;

  // via special registered interfaces
  OdPropBasePtr pPropSrc;
  if (   GETBIT(flagsToUse, OdPropBase::enPropSrcBased_Cda)
           // PROD-143 // wrapper existance of upper class (the last in list) is more priority than CDA
      && (   !id || !GETBIT(flagsToUse, OdPropBase::enPropSrcBased_RegistredXmlSrc)
          || !isRegisteredXmlProp(asClassNames.last())))
  {
    pPropSrc = createXmlPropByClassName(L"CdaBased", flagsToUse, id, pRxDb);
    ODA_ASSERT_ONCE(pPropSrc.get());
  }
  if (pPropSrc.get() && pPropSrc->getPropertyPossibleValues(sPathName, values, iconPaths))
    return true;
  if (!GETBIT(flagsToUse, OdPropBase::enPropSrcBased_RegistredXmlSrc)) // && !GETBIT(flagsToUse, OdPropBase::enPropSrcBased_XmlPattern))
    return false;

  for (uint index = 0, numClasses = asClassNames.size(); index < numClasses; index++)
  {
    OdString sClassName = asClassNames.getAt(index);

    // via registered interfaces
    pPropSrc = NULL;
    if (GETBIT(flagsToUse, OdPropBase::enPropSrcBased_RegistredXmlSrc))
      pPropSrc = createXmlPropByClassName(sClassName, flagsToUse, id, pRxDb);
    if (pPropSrc.get() && pPropSrc->getPropertyPossibleValues(sPathName, values, iconPaths))
      return true;
  }
  return false;
}

// return error message (an empty string if OK)
OdString OdPropServicesImpl::setValueXmlProps(OdDbStub* id, const OdStringArray& asClassNames,
                                              const OdString& sPathName,
                                              const OdString& sValue,
                                              const OdString& sHintGrCode, // for alternative gr code - like PloteStyle
                                              OdPropSource::Action& action,
                                              int flagsToUse, OdRxObject* pRxDb) // = NULL
{
  ODA_ASSERT_ONCE(!asClassNames.isEmpty() && !sPathName.isEmpty()); // test
  if (asClassNames.isEmpty() || sPathName.isEmpty())
    return getIApp()->toOdString(QObject::tr("Invalid params of OdPropServices::setValueXmlProps."));

  OdString sXmlLines;

  // via special registered interfaces
  OdPropBasePtr pPropSrc;
  if (   GETBIT(flagsToUse, OdPropBase::enPropSrcBased_Cda)
           // PROD-143 // wrapper existance of upper class (the last in list) is more priority than CDA
    && (   !id || !GETBIT(flagsToUse, OdPropBase::enPropSrcBased_RegistredXmlSrc)
          || !isRegisteredXmlProp(asClassNames.last())))
  {
    pPropSrc = createXmlPropByClassName(L"CdaBased", flagsToUse, id, pRxDb);
    ODA_ASSERT_ONCE(pPropSrc.get());
  }
  if (pPropSrc.get())
  {
    try
    {
      if (pPropSrc->setPropertyValue(sPathName, sValue, action))
        return OdString::kEmpty; // no Error
      if (GETBIT(action, OdPropSource::enAction_FailedToSet))
        return L"Failed to set Non-Com baset property " + sPathName;
      return OdString::kEmpty;
    }
    catch (OdError& err)
    {
      return err.description(); // ? out of range
    }
    catch (const OdChar* psErr)
    {
      return psErr; // ? some text about limits
    }
    catch (const char* pcszErr)
    {
      return pcszErr; // ? some text about limits
    }
  }
  if (   !GETBIT(flagsToUse, OdPropBase::enPropSrcBased_RegistredXmlSrc)
      && (!GETBIT(flagsToUse, OdPropBase::enPropSrcBased_XmlPattern) || !id))
    return OdString::kEmpty;

  OdResBufPtr data;

  QString qsPathName = getIApp()->toQString(sPathName),
          qsValue = getIApp()->toQString(sValue),
          qsError;
  for (uint index = 0, numClasses = asClassNames.size(); index < numClasses; index++)
  {
    OdString sClassName = asClassNames.getAt(index);

      // via registered interfaces
    pPropSrc = NULL;
    if (GETBIT(flagsToUse, OdPropBase::enPropSrcBased_RegistredXmlSrc))
      pPropSrc = createXmlPropByClassName(sClassName, flagsToUse, id, pRxDb);
    if (pPropSrc.get())
    {
      // more priority than way via patterns
      try
      {
        if (pPropSrc->setPropertyValue(sPathName, sValue, action))
          return OdString::kEmpty; // no Error
        //ODA_FAIL_ONCE(); // test (TOCOMMENT it later)
      }
      catch (OdError& err)
      {
        return err.description(); // ? out of range
      }
      catch (const OdChar* psErr)
      {
        return psErr; // ? some text about limits
      }
      catch (const char* pcszErr)
      {
        return pcszErr; // ? some text about limits
      }

      continue;
    }

    if (GETBIT(flagsToUse, OdPropBase::enPropSrcBased_XmlPattern) && id)
    {
      QString qsClassName = getIApp()->toQString(sClassName);
      OdSharedPtr<OdqXmlUtils>& wrpUtils = loadXmlPattern(qsClassName);
      if (wrpUtils.isNull())
        continue; // is absent

      if (data.isNull())
        data = getResBuf(id); // read an entity as a list of resbufs - for use in findGroupCode()
      ODA_ASSERT_ONCE(data.get()); // test
      if (data.isNull())
        return getIApp()->toOdString(QObject::tr("Invalid params of OdPropServices::setValueXmlProps."));
      OdqResBufPropWrapper dataWrp(data);
      if (!dataWrp.findClassName(qsClassName))
      {
        ODA_FAIL_ONCE();
        continue;
      }

      if (!setValueByPatternUtils(*wrpUtils.get(), dataWrp,
                                  qsPathName, qsValue, getIApp()->toQString(sHintGrCode),
                                  action, qsError))
        continue; // path was not found

      if (qsError.isEmpty() && !setResBuf(id, data.get()))
      {
        ODA_FAIL_ONCE(); // test
        qsError = QObject::tr("OdPropServices::setValueXmlProps: setResBuf error.");
      }
      return getIApp()->toOdString(qsError);
    }
  }

  ODA_FAIL_ONCE(); // test
  return getIApp()->toOdString(QObject::tr("OdPropServices::setValueXmlProps error : path to set value was not found."));
}

// return true if path was found
bool OdPropServicesImpl::setValueByPatternUtils(OdqXmlUtils& utils, OdqResBufPropWrapper& data,
                                                const QString& qsPathName, const QString& cqsValue,
                                                const QString& qsHintGrCode, // for alternative gr code - like PloteStyle
                                                OdPropSource::Action& action, QString& qsError) // out
{
  qsError.clear();
  ODA_ASSERT_ONCE(!qsPathName.isEmpty());

  QStringList lstKeys = utils.keys();
  if (!lstKeys.contains(qsPathName))
    return false;
  QString qsValue(cqsValue);
  
  QDomElement elemProp = utils.getElement(qsPathName);
  ODA_ASSERT_ONCE(   !elemProp.isNull() 
                  && qsPathName == (elemProp.attribute("Path") + "/" + elemProp.attribute("Name")));

  QString qsGrCodeExp = elemProp.attribute("GrCode");
  if (qsGrCodeExp.isEmpty())
  {
    ODA_FAIL_ONCE(); // unsupported for categories
    qsError = QObject::tr("OdPropServices::setValueXmlProps error : an empty group code for path \"%1\".")
                            .arg(qsPathName);
    return true; // was found
  }
  if (!data.find(qsGrCodeExp, true, qsHintGrCode))
  {
    ODA_FAIL_ONCE();
    qsError = QObject::tr("OdPropServices::setValueXmlProps error : internal error to set value at path \"%1\".")
                            .arg(qsPathName);
    return true; // was found
  }

  //  [ <Property ]   Path="..." Name="..." Type="..." Value   [ />\n ]
  QString qsType = elemProp.attribute("Type", "string");
  IqAppProps::TPropType type = OdqXmlUtils::toType(qsType);
  // unsupported for set
  ODA_ASSERT_ONCE(   type != IqAppProps::prpNone
                  && type != IqAppProps::prpCategory
                  && type != IqAppProps::prpImage
                  && type != IqAppProps::prpGrid
                  && type != IqAppProps::prpTree
                  && type != IqAppProps::prpPalette
                  && type != IqAppProps::prpFileDialog
                  && type != IqAppProps::prpReference);
  // check ranges
  QString qsMin,
          qsMax;
  bool isOk = false;
  switch (type)
  {
  case IqAppProps::prpDouble:
  case IqAppProps::prpDistance:
  case IqAppProps::prpAngle:
    {
      foreach(QString qsValueCheck, qsValue.split(',', QString::SkipEmptyParts))
      {
        double dDoubleMinLimit = - 1.0e+100,
               dDoubleMaxLimit = 1.0e+100,
               dValue = qsValueCheck.toDouble(&isOk);
        if (!isOk)
        {
          qsError = QObject::tr("Error : Invalid input.");
          return true; // was found
        }
        if (!(qsMin = elemProp.attribute("Min")).isEmpty())
        {
          dDoubleMinLimit = qsMin.toDouble(&isOk);
          ODA_ASSERT_ONCE(isOk);
        }
        if (!(qsMax = elemProp.attribute("Max")).isEmpty())
        {
          dDoubleMaxLimit = qsMax.toDouble(&isOk);
          ODA_ASSERT_ONCE(isOk);
        }
        ODA_ASSERT_ONCE(dDoubleMaxLimit > dDoubleMinLimit);
        if (dDoubleMaxLimit < dDoubleMinLimit)
        {
          qsError = QObject::tr("Error : Invalid range values at path \"%1\".").arg(qsPathName);
          return true; // was found
        }
        if (dValue <= dDoubleMinLimit || dValue >= dDoubleMaxLimit)
        {
          qsError = QObject::tr("Error : Out of range (%1, %2).").arg(dDoubleMinLimit).arg(dDoubleMaxLimit);
          return true; // was found
        }
      } // foreach
    }
    break;
  case IqAppProps::prpInt:
    {
      foreach(QString qsValueCheck, qsValue.split(',', QString::SkipEmptyParts))
      {
        int nIntMinLimit = INT_MIN,
            nIntMaxLimit = INT_MAX,
            nValue = qsValueCheck.toInt(&isOk);
        if (!isOk)
        {
          qsError = QObject::tr("Error : Invalid input.");
          return true; // was found
        }
        if (   !(qsMin = elemProp.attribute("Min")).isEmpty() 
            && (nValue < (nIntMinLimit = qsMin.toInt(&isOk))))
        {
          ODA_ASSERT_ONCE(isOk);
          if (isOk)
          {
            qsError = QObject::tr("Error : Out of range (< %1).").arg(nIntMinLimit);
            return true; // was found
          }
        }
        if (  !(qsMax = elemProp.attribute("Max")).isEmpty()
            && (nValue > (nIntMaxLimit = qsMax.toInt(&isOk))))
        {
          ODA_ASSERT_ONCE(isOk);
          if (isOk)
          {
            qsError = QObject::tr("Error : Out of range (> %1).").arg(nIntMaxLimit);
            return true; // was found
          }
        }
      } // foreach
    }
    break;
  case IqAppProps::prpOdColor:
    {
      OdCmEntityColor color = OdqPropertyModel::OdColorByString(qsValue, &isOk);
      ODA_ASSERT_ONCE(isOk);
      if (!isOk)
      {
        qsError = QObject::tr("Error : Invalid input.");
        return true; // was found
      }
      qsValue = QString::number(color.colorIndex());

      // gr code 420 & 430
      if (color.colorMethod() == OdCmEntityColor::kByColor)
      {
        QString qsColor = QString::number(color.color() & 0xFFFFFF);
        if (   !data.setValue(qsValue)
            || !data.findNext("420", true, true) // strong the next
            || !data.setValue(qsColor))
        {
          qsError = QObject::tr("Error : Internal error to set color.");
          return true; // was found
        }
        data.removeNext("430", true);
        return true; // was found
      }
      if (data.removeNext("420", true))
        data.removeNext("430", true);

      break;
    }
  }
  ODA_ASSERT_ONCE(qsHintGrCode.isEmpty() || qsHintGrCode == data.code())

  if (!data.setValue(qsValue))
  {
    ODA_FAIL_ONCE(); // unsupported GrCode
    qsError = QObject::tr("OdPropServices::setValueXmlProps error : unsupported group code to set value at path \"%1\".")
                  .arg(qsPathName);
    //return true; // was found
  }

  return true;
}

OdSharedPtr<OdqXmlUtils>& OdPropServicesImpl::loadXmlPattern(const QString& cqsPatternFileName)
{
  static OdSharedPtr<OdqXmlUtils> rtnNull;
  ODA_ASSERT_ONCE(   !cqsPatternFileName.isEmpty() 
                  && cqsPatternFileName.indexOf('/') < 0 && cqsPatternFileName.indexOf('\\') < 0);
  if (cqsPatternFileName.isEmpty())
    return rtnNull;

  QString qsPatternFileName = cqsPatternFileName;
  if (!qsPatternFileName.indexOf("AcDb"))
    qsPatternFileName = "OdDb" + qsPatternFileName.mid(4); //  for DWG only
  if (!qsPatternFileName.endsWith(".xml", Qt::CaseInsensitive))
    qsPatternFileName += ".xml";
  
  QHash<QString, OdSharedPtr<OdqXmlUtils> >::iterator itr = m_mapXmlPatterns.find(qsPatternFileName);
 #ifndef QT_DEBUG
  if (itr != m_mapXmlPatterns.end())
    return itr.value(); // may be NULL
 #else
  static uint s_timeLastestMappedFile = 0; // secs past 1970-01-01T00:00:00
 #endif

  QStringList lstPaths;
  QString qsPriorityPath = getIApp()->toQString(getIAppProps()->toString(OD_T("Vars/proppatternpaths")));
  if (!qsPriorityPath.isEmpty())
    lstPaths << qsPriorityPath.split(chPathSeparator, QString::SkipEmptyParts);
  if (itr == m_mapXmlPatterns.end())
    lstPaths << ":/prop-patterns"; // default place in resources

  foreach (QString qsPath, lstPaths)
  {
    QString qsPathName = QDir(qsPath).absoluteFilePath(qsPatternFileName);
    QFileInfo info(qsPathName);
    if (!info.exists())
      continue;
   #ifdef QT_DEBUG
    // for non-resourced files in debug only
    bool isFromResouce = !qsPathName.indexOf(':');
    uint time = info.lastModified().toTime_t();
    if (itr != m_mapXmlPatterns.end())
    {
      ODA_ASSERT_ONCE(!isFromResouce);
      if (time <= s_timeLastestMappedFile)
        return itr.value(); // already loaded
    }
   #endif

    OdSharedPtr<OdqXmlUtils> wrpPattern = new OdqXmlUtils();
    QString qsErr = wrpPattern->loadXmlFile(qsPathName, false);
    if (!qsErr.isEmpty())
    {
      ODA_FAIL_ONCE(); // syntax error
      getIApp()->toStatusBar(qsErr, 4000);
      continue;
    }
   #ifdef QT_DEBUG
    if (!isFromResouce && time > s_timeLastestMappedFile)
      s_timeLastestMappedFile = time;
   #endif

    return (m_mapXmlPatterns[qsPatternFileName] = wrpPattern);
  }

  if (itr != m_mapXmlPatterns.end())
    return itr.value();
  return (m_mapXmlPatterns[qsPatternFileName] = rtnNull);
}

static OdString convertorFromResBuf_LINETYPE(const OdString& sResBufValue, OdDbStub*) // id 
{
  if (sResBufValue.isEmpty())
    return getIApp()->toOdString(OdqPropertyModel::stringByLineType(OdDbObjectId()));
  return sResBufValue;
}

static OdString convertorToResBuf_LINETYPE(const OdString& sPaletteValue, OdDbStub*, OdString&) // id, sHintGrCode
{
  ODA_ASSERT_ONCE(!sPaletteValue.isEmpty());
  if (sPaletteValue.isEmpty())
    return getIApp()->toOdString(OdqPropertyModel::stringByLineType(OdDbObjectId()));
  return sPaletteValue;
}

static bool convertorGetPossible_LINETYPE(OdStringArray& values, OdStringArray& iconPaths, 
                                          OdRxObject* pRxDatabase)
{
  ODA_ASSERT_ONCE(pRxDatabase);

  QStringList lstValues, 
              lstIcons;
  if (!OdqPropertyModel::getPossibleLineTypes(pRxDatabase, lstValues, lstIcons))
    return false;;
  foreach (QString qsValue, lstValues)
    values.push_back(getIApp()->toOdString(qsValue));
  foreach (QString qsPath, lstIcons)
    iconPaths.push_back(getIApp()->toOdString(qsPath));
  return true;
}

static OdString convertorFromResBuf_LINEWEIGHT(const OdString& sResBufValue, OdDbStub*) // id 
{
  OdDb::LineWeight nLineWeight = OdDb::kLnWtByLayer;
  if (!sResBufValue.isEmpty())
    odSScanf(sResBufValue,  __OD_T("%i"), &nLineWeight);

  return getIApp()->toOdString(OdqPropertyModel::stringByLineWeight(nLineWeight, false));
}

static OdString convertorToResBuf_LINEWEIGHT(const OdString& sPaletteValue, OdDbStub* id, OdString&) // sHintGrCode
{
  OdDb::LineWeight nLineWeight = OdqPropertyModel::lineWeightByString(getIApp()->toQString(sPaletteValue));
  OdString sValue;
  sValue.format(OD_T("%d"), nLineWeight);
  return sValue;
}

static bool convertorGetPossible_LINEWEIGHT(OdStringArray& values, OdStringArray& iconPaths, 
                                            OdRxObject* pRxDatabase)
{
  ODA_ASSERT_ONCE(pRxDatabase);
  QStringList lstValues, 
              lstIcons;
  if (!OdqPropertyModel::getPossibleLineWeights(lstValues, lstIcons))
    return false;
  foreach (QString qsValue, lstValues)
    values.push_back(getIApp()->toOdString(qsValue));
  foreach (QString qsPath, lstIcons)
    iconPaths.push_back(getIApp()->toOdString(qsPath));
  return true;
}

static OdString convertorFromResBuf_TRANSPARENCY(const OdString& sResBufValue, OdDbStub*) // id 
{
  OdCmTransparency transparency;
  if (!sResBufValue.isEmpty())
  {
    OdUInt32 val;
    odSScanf(sResBufValue,  L"%i", &val);
    transparency.serializeIn(val);
  }

  return getIApp()->toOdString(OdqPropertyModel::stringByTransparency(transparency));
}

static OdString convertorToResBuf_TRANSPARENCY(const OdString& sPaletteValue, OdDbStub* id, OdString&) // sHintGrCode
{
  OdCmTransparency transparency = OdqPropertyModel::transparencyByString(getIApp()->toQString(sPaletteValue));
  OdString sValue;
  if (!transparency.isByLayer())
  {
    OdUInt32 val = transparency.serializeOut();
    sValue.format(L"%d", val);
  }
  return sValue;
}

static bool convertorGetPossible_TRANSPARENCY(OdStringArray& values, OdStringArray& iconPaths, 
                                              OdRxObject* pRxDatabase)
{
  ODA_ASSERT_ONCE(pRxDatabase);
  QStringList lstValues;
  if (!OdqPropertyModel::getPossibleTransparencies(lstValues))
    return false;
  foreach (QString qsValue, lstValues)
    values.push_back(getIApp()->toOdString(qsValue));
  return true;
}

static OdString convertorFromResBuf_YESNO(const OdString& csResBufValue, OdDbStub*) // id 
{
  OdString sResBufValue = csResBufValue;
  sResBufValue.makeLower().trimLeft().trimRight();
  bool bResBufValue = false;
  if (sResBufValue == L"true" || sResBufValue == "1")
    bResBufValue = true;
  else
  {
    ODA_ASSERT_ONCE(sResBufValue == L"false" || sResBufValue == "0");
  }
  OdString sValue = bResBufValue ? L"Yes" : L"No";
  return sValue;
}

static OdString convertorToResBuf_YESNO(const OdString& csPaletteValue, OdDbStub* id, OdString&) // sHintGrCode
{
  OdString sPaletteValue = csPaletteValue;
  sPaletteValue.makeLower().trimLeft().trimRight();
  bool bValue = false;
  if (!sPaletteValue.isEmpty() && !OdString(L"yes").find(sPaletteValue))
    bValue = true;
  else
  {
    ODA_ASSERT_ONCE(!sPaletteValue.isEmpty() && !OdString(L"no").find(sPaletteValue));
  }
  return bValue ? L"true" : L"false";
}

static bool convertorGetPossible_YESNO(OdStringArray& values, OdStringArray&, OdRxObject*)
{
  values.push_back(L"Yes");
  values.push_back(L"No");
  return true;
}

// is used to convert value from resbuf string form to palette one and back
OdControllerOfResBufPropValueConvertors* OdPropServicesImpl::controllerOfResBufPropValueConvertors()
{
  return m_pStrConvController;
}

void OdPropServicesImpl::setControllerOfResBufPropValueConvertors(OdControllerOfResBufPropValueConvertors* pController)
{
  if (m_pStrConvController == pController)
    return;

  if (m_pStrConvController)
  {
    ODA_ASSERT_ONCE(!pController); // TODO palette always use controller from OdaQtPropSystem now
    if (!pController)
    {
      // remove local convertors
      // (attension - L#convertor is correct for Windows only)
      #define OD_RB_VALUE_CONV(convertor, from, to, possible) \
        m_pStrConvController->removeConvertor(OdString(#convertor));

      #include "ResBufPropValueBaseConvertors.h"
    }
  }

  m_pStrConvController = pController;
  if (m_pStrConvController)
  {
    #define ARG_true(convertor, prefix) prefix##convertor
    #define ARG_false(convertor, prefix) NULL
    #define OD_RB_VALUE_CONV(convertor, from, to, possible) \
      m_pStrConvController->addConvertor(OdString(#convertor), \
                                         ARG_##from(convertor, convertorFromResBuf_), \
                                         ARG_##to(convertor, convertorToResBuf_), \
                                         ARG_##possible(convertor, convertorGetPossible_));

    #include "ResBufPropValueBaseConvertors.h"

    #undef ARG_true
    #undef ARG_false
  }
}

OdString OdPropServicesImpl::colorBaseToStr(OdRxObject* pRxDb, const OdCmColorBase& color)
{
  OdCmEntityColor clr;
  clr.setColor(color.color());
  OdString sVal = getIApp()->toOdString(OdqPropertyModel::stringByOdColor(clr, pRxDb));
  return sVal;
}

bool OdPropServicesImpl::colorBaseByStr(OdRxObject* pRxDb, const OdString& sColor, OdCmColorBase& colorDest)
{
  bool isOk = false;
  OdCmEntityColor clr = OdqPropertyModel::OdColorByString(getIApp()->toQString(sColor), &isOk, pRxDb);
  if (!isOk)
    return false;
  colorDest.setColor(clr.color());
  return true;
}

OdString OdPropServicesImpl::linetypeToStr(OdRxObject* pRxDb, OdDbStub* id)
{
  OdString sVal = getIApp()->toOdString(OdqPropertyModel::stringByLineType(OdDbObjectId(id)));
  return sVal;
}

bool OdPropServicesImpl::linetypeByStr(OdRxObject* pRxDb, const OdString& sLinetype, OdDbStub** pIdDest)
{
  OdDbObjectId id = OdqPropertyModel::lineTypeIdByString(pRxDb, getIApp()->toQString(sLinetype));
  *pIdDest = id;
  return true;
}

OdString OdPropServicesImpl::lineweightToStr(OdRxObject*, int nLineWeight, 
                                             bool bLineWeightAsIndex) // = false
{
  OdString sVal = getIApp()->toOdString(OdqPropertyModel::stringByLineWeight(nLineWeight, bLineWeightAsIndex));
  return sVal;
}

bool OdPropServicesImpl::lineweightByStr(OdRxObject*, const OdString& sLineweight, OdDb::LineWeight& lwDest)
{
  lwDest = OdqPropertyModel::lineWeightByString(getIApp()->toQString(sLineweight));
  return true;
}

OdString OdPropServicesImpl::transparencyToStr(OdRxObject*, const class OdCmTransparency& transparency)
{
  OdString sVal = getIApp()->toOdString(OdqPropertyModel::stringByTransparency(transparency));
  return sVal;
}

bool OdPropServicesImpl::transparencyByStr(OdRxObject*, const OdString& sTransparency, OdCmTransparency& transparencyDest)
{
  transparencyDest = OdqPropertyModel::transparencyByString(getIApp()->toQString(sTransparency));
  return true;
}

void OdPropServicesImpl::loadCatWeights()
{
  if (!m_mapCatWeights.isEmpty())
    return;

  QFile file;
  QString qsPath = QDir::toNativeSeparators(QDir::homePath() + "/ODA/prop");
  if (QDir(qsPath).exists())
  {
    qsPath = QDir::toNativeSeparators(qsPath + "/prop_categories.ini"); // override of resources
    file.setFileName(qsPath);
  }
  if (file.fileName().isEmpty() || !file.exists())
    file.setFileName(":/prop_categories.ini"); // ("qrc:/prop_categories.ini");

  if (file.open(QIODevice::ReadOnly)) // | QIODevice::Text))
  {
    QTextStream input(&file);
    QString qsLines;
    if (!input.atEnd())
      qsLines = input.readAll();
    qsLines.replace('\r', '\n');
    while (!qsLines.isEmpty())
    {
      QString qsLine;
      int pos = qsLines.indexOf('\n');
      if (pos >= 0)
        qsLine = qsLines.left(pos).trimmed(), qsLines = qsLines.mid(++pos);
      else
        qsLine = qsLines.trimmed(), qsLines.clear();
      if (qsLine.isEmpty())
        continue;

      switch (qsLine.at(0).toLatin1())
      {
      case ':': case ';': case '/':
        continue; // as comment line
      }

      pos = qsLine.lastIndexOf(':');
      ODA_ASSERT_ONCE(pos > 0);
      if (pos <= 0)
        continue;

      OdString sCat = getIApp()->toOdString(qsLine.left(pos).trimmed());
      QString qsValue = qsLine.mid(++pos).trimmed();
      float weight = qsValue.toFloat();

      m_mapCatWeights[sCat] = weight;
    }
  }

  if (m_mapCatWeights.find(L"dwg") == m_mapCatWeights.end())
    m_mapCatWeights[L"dwg"] = 10.0f;
}

void OdPropServicesImpl::saveCatWeights()
{
  QString qsPath = QDir::toNativeSeparators(QDir::homePath() + "/ODA/prop");
  if (QDir(qsPath).exists())
  {
    // rename result file into prop_categories.ini to load it with next app executing
    qsPath = QDir::toNativeSeparators(qsPath + "/prop_categories.txt");
    QFile file(qsPath);
    if (file.exists())
      file.remove();
    if (file.open(QFile::WriteOnly | QFile::Text))
    {
      QTextStream output(&file);
      foreach(OdString sCat, m_mapCatWeights.keys())
      {
        QString qsCat = getIApp()->toQString(sCat);
        output << qsCat << ": " << QString::number(m_mapCatWeights[sCat]) << "\n";
      }
    }
  }
}

bool OdPropServicesImpl::overrideCategoryWeight(OdRxObject* pRxDb, const OdString& csCat,
                                                float& weight) // out
{
  ODA_ASSERT_ONCE(pRxDb && !csCat.isEmpty() && !m_mapCatWeights.isEmpty());

  OdString sPref;
  OdExtDbModule* pExtDb = getExtDbModule(pRxDb);
  if (pExtDb)
  {
    sPref = pExtDb->baseModuleName(true);
    ODA_ASSERT_ONCE(!sPref.isEmpty());

    if (m_mapCatWeights.find(sPref) == m_mapCatWeights.end())
      m_mapCatWeights[sPref] = 10.0f;
  }
  OdString sCat = sPref;
  if (!sPref.isEmpty())
    sCat += L':';
  sCat += csCat;

  QMap<OdString, float>::iterator itr = m_mapCatWeights.find(sCat);
  if (itr != m_mapCatWeights.end())
    weight = itr.value();
  else
  {
    if (sPref.isEmpty())
      sPref = L"dwg";
    weight = m_mapCatWeights[sPref];
    m_mapCatWeights[sPref] = weight + 10.0f;
    m_mapCatWeights[sCat] = weight;
  }

  return true; // return false; 
}

bool OdPropServicesImpl::overrideCategoryName(OdRxObject* pRxDb, OdString& sCat, // in / out
                                              const OdString& sHintPropName, 
                                              int) // propSrcFlags
{
  bool bRes = false;
  if (sCat.isEmpty())
    bRes = true, sCat = L"Internal"; // sCat = L"None"; // DGN: sName == L"ClassName"|| + L"DisplayStyleId" ...

  // obsolete
  //if (   GETBIT(propSrcFlags, OdPropBase::enPropSrcBased_XmlPattern)
  //    || GETBIT(propSrcFlags, OdPropBase::enPropSrcBased_RegistredXmlSrc))
  //  sCat += L".";

  return bRes;
}

bool OdPropServicesImpl::overridePropertyName(OdRxObject* pRxDb, const OdString& sCat,
                                              OdString& sName, // in / out
                                              OdString* psVisName) // = NULL
{
  ODA_ASSERT_ONCE(!sCat.isEmpty() && !sName.isEmpty());
  int len = sName.getLength();
  if (len > 2 && sName.mid(len - 2) == L"Id")
  {
    // remove Id at end of name to use it as visible name
    sName = sName.left(len - 2);
    return true;
  }

  if (sCat == L"General")
  {
    if (sName == L"LinetypeScale" && psVisName && psVisName->isEmpty())
    {
      *psVisName = L"Linetype scale";
      return true;
    }
  }

  return false; 
}

bool OdPropServicesImpl::overridePropertyType(OdRxObject* pRxDb, const OdString& sCat, const OdString& sName,
                                              OdString& sType) // in / out
{
  OdString sTypePrev = sType;
  if (sName == L"General")
  {
    if (sName == L"LinetypeScale")
      sType = tCombo;
    //else 
    if (sName == L"Transparency")
      sType = tTransparency;
  }
  return sType != sTypePrev;
}

bool OdPropServicesImpl::overrideClassName(OdRxObject* pRxDb, OdDbStub* id,
                                           OdString& sClassName) // in / out
{
  OdPropBasePtr pPropSrc = createXmlPropByClassName(sClassName, OdPropBase::enPropSrcBased_RegistredXmlSrc, // L"CdaBased", OdPropBase::enPropSrcBased_Cda,
                                                    id, pRxDb);
  if (pPropSrc.isNull())
    return false;
  return pPropSrc->overrideClassName(sClassName);
}

///////////////////////////////////////////////////////////////////////////////
static bool s_bOdPropServicesInitialized = false;

void initOdPropServices()
{
  static OdStaticRxObject<OdPropServicesImpl> s_PropServices;
  if (s_bOdPropServicesInitialized)
    return;
  s_bOdPropServicesInitialized = true;

 #if defined(ODA_PROP_SERVICES_UNLINKED_RXINIT)
  OdPropServices::rxInit();
 #endif
  OdPropServicesImpl::rxInit();
  OdEdCommandContext::desc()->addX(OdPropServices::desc(), &s_PropServices);
  ::odrxSysRegistry()->putAt(OD_PROP_SERVICES, &s_PropServices);
}

void uninitOdPropServices()
{
  if (!s_bOdPropServicesInitialized) 
    return;
  s_bOdPropServicesInitialized = false;

  ::odrxSysRegistry()->remove(OD_PROP_SERVICES);
  OdEdCommandContext::desc()->delX(OdPropServices::desc());
  OdPropServicesImpl::rxUninit();
 #if defined(ODA_PROP_SERVICES_UNLINKED_RXINIT)
  OdPropServices::rxUninit();
 #endif
}
