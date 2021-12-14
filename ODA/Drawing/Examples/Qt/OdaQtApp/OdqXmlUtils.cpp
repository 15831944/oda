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
// OdqXmlUtils.cpp
//

#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QMessageBox>
#include <QMainWindow>
#include <QVariant>
//#include <QHash>

#include "OdqXmlUtils.h" 
#include "PropServices.h"

#include "TDVersion.h"

//////////////////////////////////////////////////////////////////////////

OdqXmlUtils::OdqXmlUtils()
  : m_bModified(false)
  , m_qsRootNodeName("Properties")
{
  //loadSettings("qrc:/oda_qt_props.xml");
}

OdqXmlUtils::~OdqXmlUtils()
{
}

// ----- OdqXmlUtils -----

bool OdqXmlUtils::isLoaded() const
{
  bool bValue = !m_docXml.isNull();
  return bValue;
}

const QString& OdqXmlUtils::getBasePath() const
{
  return m_qsBasePath;
}

// return message if error
QString OdqXmlUtils::loadXmlFile(const QString& cqsXmlFilePath,
                                 bool bCheckUserOverrideXmlFile) // = true
{
  QString qsXmlFilePath = cqsXmlFilePath;
  if (qsXmlFilePath.isEmpty())
  {
    if (bCheckUserOverrideXmlFile || m_qsXmlFilePath.isEmpty())
      return QObject::tr("Empty Xml path name.");
    qsXmlFilePath = m_qsXmlFilePath;
    QString qsUserXmlFilePath = getUserOverrideXmlFilePath(qsXmlFilePath);
    if (!qsUserXmlFilePath.isEmpty() && qsUserXmlFilePath != qsXmlFilePath)
    {
      QFile file(qsUserXmlFilePath);
      if (file.exists() && !file.remove())
        return QObject::tr("Access deneed to delete Xml file \"%1\".").arg(qsUserXmlFilePath);
    }
  }

  // QFile does not work with "qrc:/..." prefix (should be ":/...")
  if (!qsXmlFilePath.indexOf("qrc:/"))
    qsXmlFilePath = qsXmlFilePath.mid(3);

  QFileInfo info(qsXmlFilePath);
  if (qsXmlFilePath.isEmpty() || !info.exists())
    return QObject::tr("Xml file \"%1\" was not found.").arg(qsXmlFilePath);

  if (bCheckUserOverrideXmlFile)
  {
    QString qsUserXmlFilePath = getUserOverrideXmlFilePath(qsXmlFilePath);
    if (   !qsUserXmlFilePath.isEmpty() 
        && qsUserXmlFilePath != qsXmlFilePath
        && QFileInfo(qsUserXmlFilePath).exists())
    {
      QString qsErr = loadXmlFile(qsUserXmlFilePath, false);
      if (qsErr.isEmpty())
      {
        QString qsBaseSize = QString::number(info.size());
        QString qsTime = info.created().toString(); // "" for files in resource now (QDateTime().isValid() is false)
        //QString qsTime = QString::number(info.created().toTime_t());
        if (   qsBaseSize == m_docXml.documentElement().attribute("BaseSize")
            && qsTime == m_docXml.documentElement().attribute("BaseTime"))
        {
          m_qsXmlFilePath = qsXmlFilePath;
          return ""; // no error
        }
        // override file is too old
      }
    }
  }

  QFile file(qsXmlFilePath);
  if (!file.open(QFile::ReadOnly | QFile::Text))
    return QObject::tr("Xml file \"%1\" is inaccesible").arg(qsXmlFilePath);

  QDomDocument docXml;

  QString qsErrorMsg;
  int iErrorLine = 0,
      iErrorColumn = 0;
  if (!docXml.setContent(&file, &qsErrorMsg, &iErrorLine, &iErrorColumn))
  {
    QString qsErrMsg = QObject::tr("%1 - at line %2 column %3 of XML file \"%4\" ")
                            .arg(qsErrorMsg)
                            .arg(iErrorLine)
                            .arg(iErrorColumn)
                            .arg(qsXmlFilePath);
    return qsErrMsg;
  }

  m_qsXmlFilePath = qsXmlFilePath;
  m_docXml = docXml;
  m_bModified = false;
  m_keys.clear();
  m_mapValues.clear();
  m_mapElements.clear();
  return ""; // no error
}

QString OdqXmlUtils::loadXmlFromString(const QString& qsXml,
                                       const QString& qsBasePath, // = "" all
                                       const QString& qsRootNodeName) // = "Properties"
{
  m_qsBasePath = qsBasePath;
  if (!qsRootNodeName.isEmpty())
    m_qsRootNodeName = qsRootNodeName;
  else
  {
    ODA_FAIL_ONCE();
  }

  ODA_ASSERT_ONCE(!qsXml.isEmpty());

  QDomDocument docXml;

  QString qsErrorMsg;
  int iErrorLine = 0,
      iErrorColumn = 0;
  if (!docXml.setContent(qsXml, &qsErrorMsg, &iErrorLine, &iErrorColumn))
  {
#  ifdef QT_DEBUG
    {
      QString qsPath = QDir::toNativeSeparators(QDir::homePath() + "/ODA/prop");
      if (QDir(qsPath).exists())
      {
        qsPath = QDir::toNativeSeparators(qsPath + "/prop_error.xml");
        OdString sData = getIApp()->toOdString(qsXml);
        OdInt32 len = sData.getLength();
        if (len < (200 * 1024 * 1024))
        {
          OdStreamBufPtr io = ::odrxSystemServices()->createFile(getIApp()->toOdString(qsPath),
                                                                 Oda::kFileWrite, Oda::kShareDenyNo, Oda::kCreateAlways);
          ODA_ASSERT_ONCE(io.get());
          if (io.get())
            io->putBytes(sData.c_str(), len);
        }
      }
    }
#  endif
    QString qsErrMsg = QObject::tr("%1 - at line %2 column %3")
                            .arg(qsErrorMsg)
                            .arg(iErrorLine)
                            .arg(iErrorColumn);
    return qsErrMsg;
  }

  m_qsXmlFilePath = "";
  m_docXml = docXml;
  m_bModified = false;
  m_keys.clear();
  m_mapValues.clear();
  m_mapElements.clear();

  merge();

  return ""; // no error
}

static void addWithLeadingSeparator(const QString& qsValue, QString& qsRes)
{
  QString qsSep = ":";
  if (!qsRes.isEmpty())
    qsSep = qsRes.left(1);
  qsRes += qsSep;
  
  if (qsValue.indexOf(qsSep) >= 0)
  {
    // TODO (only one alternative is supported)
    ODA_ASSERT_ONCE(   qsSep == ":" 
                    && qsValue.indexOf(";") < 0
                    && qsRes.indexOf(";") < 0);

    qsRes.replace(qsSep, ";");
  }
  qsRes += qsValue;
}

bool OdqXmlUtils::setMergeFilter(const QString& qsMergeFilter, // = "" all
                                 const QString& qsMergeFilterAttribName, // = "" // is already set
                                 const QString& qsUnmergedBasePath) // = "" // is already set
{
  ODA_ASSERT_ONCE(!m_docXml.isNull());

  bool bSkipMerging = true;
  ODA_ASSERT_ONCE(m_qsBasePath.isEmpty() || m_qsUnmergedBasePath.indexOf(m_qsBasePath));
  if (   !qsUnmergedBasePath.isEmpty() 
      && qsUnmergedBasePath.indexOf(m_qsBasePath)
      && m_qsUnmergedBasePath != qsUnmergedBasePath)
  {
    bSkipMerging = false;
    m_qsUnmergedBasePath = qsUnmergedBasePath;
  }
  if (m_qsUnmergedBasePath.isEmpty())
  {
    // without merging
    ODA_ASSERT_ONCE(bSkipMerging && m_qsMergeFilter.isEmpty());
    m_qsMergeFilter.clear();
    return false;
  }
  ODA_ASSERT_ONCE(m_qsUnmergedBasePath.indexOf(m_qsBasePath));

  if (   !qsMergeFilterAttribName.isEmpty()
      && m_qsMergeFilterAttribName != qsMergeFilterAttribName)
  {
    bSkipMerging = false;
    m_qsMergeFilterAttribName = qsMergeFilterAttribName;
  }
  ODA_ASSERT_ONCE(!m_qsMergeFilterAttribName.isEmpty());
  if (m_qsMergeFilterAttribName.isEmpty())
    m_qsMergeFilterAttribName = "Filter"; // default

  if (m_qsMergeFilter != qsMergeFilter)
  {
    //if (!m_qsMergeFilter.isEmpty() && qsMergeFilter.isEmpty())
    //{
    //  // PROD-147 Property palette problem with an empty categories via back switching to all entities
    //  //m_keys.clear();
    //  m_mapElements.clear(); // to renew merged categories
    //  //m_mapValues.clear();
    //  //m_bModified = true;
    //}

    bSkipMerging = false;
    m_qsMergeFilter = qsMergeFilter;
  }
  if (bSkipMerging)
    return false; // already done (via loadXmlFromString for example)
 
  return merge();
}

static QString getElemPath(QDomElement& elem)
{
  QString qsPath = elem.attribute("Path");
  if (!qsPath.isEmpty())
    qsPath += "/";
  qsPath += elem.attribute("Name");
  return qsPath;
}

bool OdqXmlUtils::merge()
{
  if (m_qsUnmergedBasePath.isEmpty())
  {
    // without merging
    ODA_ASSERT_ONCE(m_qsMergeFilter.isEmpty());
    m_qsMergeFilter.clear();
    return false;
  }
  ODA_ASSERT_ONCE(m_qsUnmergedBasePath.indexOf(m_qsBasePath));

  QDomElement elemRoot = m_docXml.documentElement();
  ODA_ASSERT_ONCE(!elemRoot.isNull() && elemRoot.tagName() == m_qsRootNodeName); // "Properties" // ?TODO
  if (elemRoot.isNull() || elemRoot.tagName() != m_qsRootNodeName)
    return false;
  //OdString sBasePath = getIApp()->toOdString(m_qsBasePath);

  // remove all child of m_qsBasePath
  checkMaps();
  if (!m_mapElements.isEmpty())
  {
    QDomElement elemBasePath = getElement(m_qsBasePath);
    ODA_ASSERT_ONCE(!elemBasePath.isNull());
    if (!elemBasePath.isNull())
    {
      foreach(QString qsPath, keys())
      {
        ODA_ASSERT_ONCE(!qsPath.indexOf(m_qsBasePath));

        QDomElement elem = getElement(qsPath);
        ODA_ASSERT_ONCE(!elem.isNull());

        elemRoot.removeChild(elem);
      }
    }
  }
 #ifdef QT_DEBUG
  {
    QString qsPath = QDir::toNativeSeparators(QDir::homePath() + "/ODA/prop");
    if (QDir(qsPath).exists())
    {
      qsPath = QDir::toNativeSeparators(qsPath + "/prop_unmerged.xml");
      saveXmlFile(qsPath);
    }
  }
 #endif

  // collect unmerged elements to multimap,
  // get count of merged entities via required for each entity base node
  // ( see !!!***Attention***!!! )
  // and fix attrib Filter on unmerged children

  QMultiHash<QString, QDomElement> mapMatchedUnmerged; // faster than QMultiMap (but has problems with OdString)
  QStringList keysMatched; // in source order (differ to mapMatchedUnmerged.keys())
  int countEntities = 0,
      indexEntity = -1,
      countMatchedEntities = 0;
  QString qsLastFilter;

  static uint limitPropEntities = 0; // like "GRIPOBJLIMIT"
  if (!limitPropEntities)
    limitPropEntities = (uint) getIAppProps()->toInt(L"Vars/propobjlimit", 500);
  bool bLockByLimit = false;

  QDomNodeList lstProps = elemRoot.elementsByTagName("Property");
  uint cntNodes = lstProps.length();
  for (uint index = 0; index < cntNodes; index++) // foreach is unsupported for QDomNodeList
  {
    QDomElement elem = lstProps.at(index).toElement();
    QString qsPath = getElemPath(elem);
    if (qsPath.isEmpty())
    {
      ODA_FAIL_ONCE();
      continue;
    }

    bool isUnmergedBasePath = (qsPath == m_qsUnmergedBasePath);
    if (!isUnmergedBasePath && qsPath.indexOf(m_qsUnmergedBasePath + "/"))
      continue;

    if (isUnmergedBasePath)
    {
      countEntities++;
      indexEntity++;
      qsLastFilter = elem.attribute(m_qsMergeFilterAttribName);
    }
    else
    {
      // fix filter on unmerged child
      QString qsFilter = elem.attribute(m_qsMergeFilterAttribName);
      ODA_ASSERT_ONCE(qsFilter.isEmpty() || qsFilter == qsLastFilter); 
      if (qsFilter != qsLastFilter)
        elem.setAttribute(m_qsMergeFilterAttribName, qsLastFilter);
    }
    // add EntityIndex to merging intersected content 
    // of one entity via several BagFiller wrapper-property-collector
    ODA_ASSERT_ONCE(indexEntity >= 0);
    if (indexEntity >= 0)
      elem.setAttribute("EntityIndex", indexEntity);

    if (bLockByLimit) // notation was changed for num of entities //if (cntNodes > limitPropEntities)
      continue;
    else if (countEntities > (int)limitPropEntities)
    {
      keysMatched.clear(); mapMatchedUnmerged.clear();
      bLockByLimit = true;
      continue;
    }

    bool isMatched = (   m_qsMergeFilter.isEmpty() || qsLastFilter.isEmpty() 
                      || qsLastFilter == m_qsMergeFilter);
    if (!isMatched)
      continue;

    if (isUnmergedBasePath)
      countMatchedEntities++;

    QMultiHash<QString, QDomElement>::Iterator itr = mapMatchedUnmerged.find(qsPath);
    if (itr  == mapMatchedUnmerged.end())
      keysMatched.append(qsPath);

    bool bSkip = false;
    for (; itr != mapMatchedUnmerged.end() && itr.key() == qsPath; itr++)
    {
      ODA_ASSERT_ONCE(indexEntity >= 0);
      QString qsEntityIndex = itr.value().attribute("EntityIndex");
      ODA_ASSERT_ONCE(!qsEntityIndex.isEmpty());
      if (qsEntityIndex.isEmpty() || indexEntity != qsEntityIndex.toInt())
        continue;

      bSkip = true;
      break;
    }
    if (!bSkip)
      mapMatchedUnmerged.insert(qsPath, elem);
  }
  OdString sUnmergedBasePath = getIApp()->toOdString(m_qsUnmergedBasePath);
  QDomElement elemMerged;
 
  if (countMatchedEntities)
  {
    // merge

    QStringList keysMergedCatMerged;
    QMap<QString, int> mapMergedPropsInCat;

    foreach(QString sPath, keysMatched)
    {
      int cnt = mapMatchedUnmerged.count(sPath);
      ODA_ASSERT_ONCE(cnt > 0 && cnt <= countMatchedEntities);
      if (cnt < countMatchedEntities)
        continue;

      QMultiHash<QString, QDomElement>::iterator itr = mapMatchedUnmerged.find(sPath);
      QDomElement elem = itr.value();
      ODA_ASSERT_ONCE(!elem.isNull())

      //if (!elemMerged.isNull() && elemMerged.attribute("Type") == "category")
      //  elemMerged = QDomElement(); // skip categories without children
      if (!elemMerged.isNull())
      {
        elemRoot.appendChild(elemMerged);
        if (countMatchedEntities > 1)
        {
          QString qsParentPath = elemMerged.attribute("Path");
          if (!qsParentPath.isEmpty())
          {
            // PROD-147 Property palette problem with an empty categories via merging of diff source props
            QString qsPath = getElemPath(elemMerged);
            if (elemMerged.attribute("Type") == "category")
            {
              ODA_ASSERT_ONCE(!mapMergedPropsInCat.contains(qsPath));
              keysMergedCatMerged << qsPath;
            }
            else
            {
              QMap<QString, int>::iterator itr = mapMergedPropsInCat.find(qsParentPath);
              if (itr == mapMergedPropsInCat.end())
                mapMergedPropsInCat[qsParentPath] = 1;
              else
                itr.value()++;
            }
          }
        }
      }
      elemMerged = QDomElement();

      elemMerged = elem.cloneNode(false).toElement();
      QString qsMergedValues; // list of possible values for editing

      for (itr++; itr != mapMatchedUnmerged.end() && itr.key() == sPath; itr++)
      {
        QDomElement elem = itr.value();
        ODA_ASSERT_ONCE(!elem.isNull());
        QDomNamedNodeMap attrs = elem.attributes();
        uint idx = 0;
        for (; idx < (uint) attrs.length(); idx++)
        {
          QDomAttr attr = attrs.item(idx).toAttr();
          ODA_ASSERT_ONCE(!attr.isNull())
          if (attr.isNull())
            continue;
          QString qsName = attr.name(),
                  qsValue = attr.value();
          ODA_ASSERT_ONCE(!qsName.isEmpty());
          if (qsName.isEmpty())
            continue;
          if ((qsName == "Skip" || qsName == "Hidden") && qvariant_cast<bool>(attr.value()))
            elemMerged.setAttribute(qsName, "true");

          QDomAttr attrMerged = elemMerged.attributes().namedItem(qsName).toAttr();
          //ODA_ASSERT_ONCE(!attrMerged.isNull()); // test
          QString qsMergedValue;
          if (   !attrMerged.isNull() 
              && ((qsMergedValue = attrMerged.value()) == qsValue || qsMergedValue == QObject::tr("*VARIES*")))
            continue;
          if (qsName == "Value")
          {
            if (qsMergedValues.isEmpty())
              addWithLeadingSeparator(qsMergedValue, qsMergedValues);
            addWithLeadingSeparator(qsValue, qsMergedValues);
          }
          if (qsMergedValue == QObject::tr("*VARIES*"))
            continue;
          if (qsName == "Type")
          {
            // PROD-147 merging problem in General category via differ types
            if (qsValue == tDouble && (qsMergedValue == tCombo || qsMergedValue == tString))
            {
              elemMerged.setAttribute(qsName, tDouble);
              continue;
            }
            else if (   (qsValue == tCombo || qsValue == tString) 
                     && (qsMergedValue == tDouble || qsMergedValue == tTransparency))
            {
              elemMerged.setAttribute(qsName, qsMergedValue);
              continue;
            }
            if (   (qsValue == tTransparency && qsMergedValue == tCombo)
                || (qsValue == tCombo && qsMergedValue == tTransparency))
            {
              elemMerged.setAttribute(qsName, tTransparency);
              continue;
            }
            ODA_ASSERT_VAR(else)
              ODA_ASSERT(true); // brk
          }
          if (   qsName == "Path" || qsName == "Name" 
            || (qsName == "Type" && qsValue != "double" && qsValue != "combo")) // PROD-147 merging problem in General category
          {
            ODA_FAIL_ONCE();
            elemMerged = QDomElement();
            break;
          }

          elemMerged.setAttribute(qsName, QObject::tr("*VARIES*"));
        }
        if (elemMerged.isNull())
          continue;

        QDomNamedNodeMap attrsMerged = elemMerged.attributes();
        for (idx = 0; idx < (uint) attrsMerged.length(); idx++)
        {
          QDomAttr attr = attrsMerged.item(idx).toAttr();
          ODA_ASSERT_ONCE(!attr.isNull())
          if (attr.isNull())
            continue;
          QString qsName = attr.name();
          ODA_ASSERT_ONCE(!qsName.isEmpty());
          if (qsName.isEmpty() || !elem.attributes().namedItem(qsName).toAttr().isNull())
            continue;
          QString qsMergedValue = attr.value();
          if (qsName == "Value")
          {
            if (qsMergedValues.isEmpty())
              addWithLeadingSeparator(qsMergedValue, qsMergedValues);
            addWithLeadingSeparator("", qsMergedValues);
          }
          if (qsMergedValue == QObject::tr("*VARIES*"))
            continue;
          if (qsName == "Path" || qsName == "Name" || qsName == "Type")
          {
            ODA_FAIL_ONCE();
            elemMerged = QDomElement();
            break;
          }

          elemMerged.setAttribute(qsName, QObject::tr("*VARIES*"));
          idx--;
        }
      }
      if (elemMerged.isNull())
        continue;

      QString qsParentPath = elemMerged.attribute("Path");
      if (!qsParentPath.isEmpty())
      {
        ODA_ASSERT_ONCE(   qsParentPath == m_qsUnmergedBasePath 
                        || !qsParentPath.indexOf(m_qsUnmergedBasePath + "/"));
        qsParentPath = m_qsBasePath + qsParentPath.mid(m_qsUnmergedBasePath.length());
        elemMerged.setAttribute("Path", qsParentPath);
      }
      else
      {
        ODA_ASSERT_ONCE(m_qsUnmergedBasePath == elemMerged.attribute("Name"));
        elemMerged.setAttribute("Name", m_qsBasePath);
      }
      if (!qsParentPath.isEmpty())
        elemMerged.removeAttribute(m_qsMergeFilterAttribName); 
      else
        elemMerged.setAttribute(m_qsMergeFilterAttribName, m_qsMergeFilter); // debug info

      foreach (QString qsInNotVaiesAttr, QStringList() << "Image" << "VisibleName" << "Comment")
      {
        if (elemMerged.attribute(qsInNotVaiesAttr) == QObject::tr("*VARIES*"))
          elemMerged.removeAttribute(qsInNotVaiesAttr);
      }
      elemMerged.removeAttribute("EntityIndex");

      if (!qsMergedValues.isEmpty())
        elemMerged.setAttribute("Values", qsMergedValues);
    }
    if (!elemMerged.isNull() && elemMerged.attribute("Type") == "category")
      elemMerged = QDomElement(); // skip categories without children
    if (!elemMerged.isNull())
      elemRoot.appendChild(elemMerged);

    if (countMatchedEntities > 1 && keysMergedCatMerged.length())
    {
      // PROD-147 Property palette problem with an empty categories via merging of diff source props
      QDomElement elemBasePath = getElement(m_qsBasePath);
      ODA_ASSERT_ONCE(!elemBasePath.isNull());
      if (!elemBasePath.isNull())
      {
        foreach(QString qsPath, keysMergedCatMerged)
        {
          ODA_ASSERT_ONCE(!qsPath.indexOf(m_qsBasePath));

          QMap<QString, int>::iterator itr = mapMergedPropsInCat.find(qsPath);
          if (itr != mapMergedPropsInCat.end())
          {
            ODA_ASSERT_ONCE(itr.value() > 0);
            continue;
          }

          // remove an empty category
          QDomElement elem = getElement(qsPath);
          ODA_ASSERT_ONCE(!elem.isNull());
          if (elem.isNull())
            continue;
          //QDomElement elemRoot = m_docXml.documentElement();
          if (elemRoot.removeChild(elem).isNull())
          {
            // error by QT manual
            // TODO problem with an empty categories via back switching to all entities from some else
            // (it is strange but removed category is present in result prop_merged.xml in this case)
            // useless as cleared at end of this function: //m_mapElements.clear(); // m_mapElements.remove(qsPath); m_keys.removeAt(m_keys.indexOf(qsPath));

            QDomElement el = getElement(qsPath, true); // workaroud
            ODA_ASSERT_ONCE(el != elem); // TODO investigate reason of it
            if (elemRoot.removeChild(el).isNull())
            {
              ODA_FAIL_ONCE(); 
#            ifdef QT_DEBUG
              {
                QString qsPath = QDir::toNativeSeparators(QDir::homePath() + "/ODA/prop");
                if (QDir(qsPath).exists())
                {
                  qsPath = QDir::toNativeSeparators(qsPath + "/prop_merged_strange.xml");
                  saveXmlFile(qsPath);
                }
              }
#            endif
            }
          }
        }
      }
    } // of if (countMatchedEntities > 1 && keysMergedCatMerged.length())
  }
  else // if (countMatchedEntities)
  {
    // create alone merged node to clear content m_pWidet palette model

    QString qsPath = m_qsBasePath,
            qsName = m_qsBasePath;
    int pos = qsPath.indexOf('/');
    if (pos >= 0)
    {
      ODA_FAIL_ONCE(); // TODO test with DEB
      qsName = qsPath.left(pos);
      qsPath = qsPath.mid(++pos);
    }
    else
      qsPath.clear();
    elemMerged = m_docXml.createElement("Property").toElement();
    elemMerged.setAttribute("Path", qsPath);
    elemMerged.setAttribute("Name", qsName);
    elemMerged.setAttribute("Type", "category");
    elemMerged.setAttribute(m_qsMergeFilterAttribName, m_qsMergeFilter); // debug info
    elemRoot.appendChild(elemMerged);
  }

 #ifdef QT_DEBUG
  {
    QString qsPath = QDir::toNativeSeparators(QDir::homePath() + "/ODA/prop");
    if (QDir(qsPath).exists())
    {
      qsPath = QDir::toNativeSeparators(qsPath + "/prop_merged.xml");
      saveXmlFile(qsPath);
    }
  }
 #endif

  m_bModified = false;
  m_keys.clear();
  m_mapValues.clear();
  m_mapElements.clear();
  return true;
}

bool OdqXmlUtils::saveXmlFile()
{
  if (!m_bModified)
    return true;

  QString qsXmlFilePath = getUserOverrideXmlFilePath(m_qsXmlFilePath, true);
  if (qsXmlFilePath.isEmpty())
    return false;

  if (qsXmlFilePath != m_qsXmlFilePath)
  {
    QFileInfo info(m_qsXmlFilePath);
    QString qsBaseSize = QString::number(info.size());
    m_docXml.documentElement().setAttribute("BaseSize", qsBaseSize);
    QString qsTime = info.created().toString();  // "" for files in resource now (QDateTime().isValid() is false)
    //QString qsTime = QString::number(info.created().toTime_t()); // return (unsigned) -1
    if (!qsTime.isEmpty())
      m_docXml.documentElement().setAttribute("BaseTime", qsTime);
  }
  
  if (!saveXmlFile(qsXmlFilePath))
    return false;
  
  m_bModified = false;
  return true;
}

bool OdqXmlUtils::saveXmlFile(const QString& qsXmlFilePath)
{
  QFile file(qsXmlFilePath);
  if (!file.open(QFile::WriteOnly | QFile::Text)) 
  {
    ODA_FAIL_ONCE();
    return false;
  }

  QTextStream stream(&file);
  m_docXml.save(stream, 2); // indent size
  return true;
}

//static 
QString OdqXmlUtils::getUserOverrideXmlFilePath(const QString& qsXmlFilePath,
                                                bool bCreateSubFolders) // = false
{
  if (qsXmlFilePath.indexOf(":/") != 0)
    return qsXmlFilePath; // overriden file may be for resource based files only
  
  QString qsPath =   QDir::toNativeSeparators(QDir::homePath() 
                   + "/ODA/" 
                   + QFileInfo(QCoreApplication::applicationFilePath()).baseName()
                   + "/" + TD_SHORT_STRING_VER_S);
  if (bCreateSubFolders && !QDir().mkpath(qsPath))
  {
    QMessageBox::warning(getIApp()->getMainWindow(), 
                         QObject::tr("%1 Warning").arg(getIApp()->getAppName()), // caption
                         QObject::tr("Unable to create folder\"%1\"").arg(qsPath),
                         QMessageBox::Ok, QMessageBox::Ok);
    return "";
  }

  qsPath += QDir::separator();
  qsPath += QFileInfo(qsXmlFilePath).fileName();
  return qsPath;
}

void OdqXmlUtils::checkMaps()
{
  if (!m_mapElements.isEmpty() && !m_mapValues.isEmpty())
    return;

  // fill maps
  m_keys.clear();
  m_mapElements.clear();
  m_mapValues.clear();

  QDomElement elemRoot = m_docXml.documentElement();
  if (elemRoot.tagName() != m_qsRootNodeName) // "Properties"
  {
    ODA_FAIL_ONCE(); // TODO
    return;
  }
  
  QDomNodeList lstProps = elemRoot.elementsByTagName("Property");
  QHash<QString, uint> mapElementIdx;
  for (uint index = 0, sz; index < (sz = (uint)lstProps.length()); index++) // foreach is unsupported for QDomNodeList
  {
    QDomElement elem = lstProps.at(index).toElement();
    ODA_ASSERT_ONCE(!elem.isNull());
    QString qsPath = getElemPath(elem);
    if (qsPath.isEmpty())
    {
      ODA_FAIL_ONCE();
      continue;
    }

    bool bSkipBase = qvariant_cast<bool>(elem.attribute("SkipBase", "false"));
    if (bSkipBase)
    {
      if (mapElementIdx.find(qsPath) != mapElementIdx.end())
      {
        // only propagate Skip attrib to elems(+ child) in pattern of base class
        for (uint idx = mapElementIdx[qsPath]; idx < index; idx++)
        {
          QDomElement el = lstProps.at(idx).toElement();
          ODA_ASSERT_ONCE(!el.isNull());
          QString qsPath_ = getElemPath(el);
          if (qsPath.isEmpty() || qsPath_.indexOf(qsPath) != 0)
            continue;
          int len = qsPath.length();
          if (qsPath_.length() > len && qsPath_.at(len) != '/')
            continue;
          el.setAttribute("Skip", "true");
        }
        elemRoot.removeChild(elem);
        ODA_ASSERT_ONCE(sz == uint(lstProps.length() + 1));
        --index; // autoremoved from lstProps by removeChild
        continue;
      }
    }
    bool wasPathOutIdx = (mapElementIdx.find(qsPath) == mapElementIdx.end());
    if (wasPathOutIdx)
      mapElementIdx[qsPath] = index;

    if (   !m_qsBasePath.isEmpty()
        && !(qsPath == m_qsBasePath || !qsPath.indexOf(m_qsBasePath + "/")))
      continue;

    ODA_ASSERT_ONCE(m_mapElements.find(qsPath) == m_mapElements.end());
    m_mapElements[qsPath] = elem;
    m_keys.append(qsPath);

    QString qsType = elem.attribute("Type", "string").toLower();
    IqAppProps::TPropType type = toType(qsType);

    if (!wasPathOutIdx)
    {
      ODA_ASSERT_ONCE(m_keys.contains(qsPath));
      ODA_ASSERT_ONCE(m_mapElements.find(qsPath) != m_mapElements.end());
      ODA_ASSERT_ONCE(toType(m_mapElements[qsPath].attribute("Type", "string").toLower()) == IqAppProps::prpCategory);
      ODA_ASSERT_ONCE(type == IqAppProps::prpCategory); // test (non-critical) TOCOMMENT
      //ODA_ASSERT_VAR(uint idxPrev = mapElementIdx[qsPath];)
      ////ODA_ASSERT_ONCE(idxPrev == index);
      // it is normal via duplicated category by other collected classes or patterns
      continue; // already existed category 
    }

    QString qsValue = elem.attribute("Value");
    ODA_ASSERT_ONCE(qsValue.indexOf('&') < 0); // TODO //qsValue = getIApp()->decodeFromXmlAttrib(qsValue);
    QVariant qvValue = qsValue;
    OdString sValue = getIApp()->toOdString(qsValue);
    OdVariant vValue = sValue;

    switch (type)
    {
    default:
      ODA_FAIL_ONCE(); // TODO
    case IqAppProps::prpString:
    case IqAppProps::prpCombo: 
    case IqAppProps::prpLinetype: case IqAppProps::prpLineweight: case IqAppProps::prpTransparency:
    case IqAppProps::prpFolder:
      break;
    case IqAppProps::prpInt:
      vValue.setInt32(qvValue.toInt());
      break;
    case IqAppProps::prpBool:
    case IqAppProps::prpBoolPictured:
      vValue.setBool(qvValue.toBool());
      break;
    case IqAppProps::prpBoolNamed:
      ODA_ASSERT_ONCE(qsValue.isEmpty());
      vValue.setBool(true);
      break;
    case IqAppProps::prpRadio:
      if (!qsValue.isEmpty())
        vValue.setBool(qvValue.toBool());
      break;
    case IqAppProps::prpDouble:
    case IqAppProps::prpDistance:
    case IqAppProps::prpAngle:
      vValue.setDouble(qvValue.toDouble());
      break;
    case IqAppProps::prpColorref:
    case IqAppProps::prpOdColor:
      //{
      //  bool bRes = false;
      //  uint uiValue = qvValue.toUInt(&bRes);
      //  if (!bRes)
      //  {
      //    ODA_FAIL_ONCE();
      //    uiValue = 0;
      //  }
      //  vValue.setInt32(uiValue);
          break; // store as string
      //}
    case IqAppProps::prpImage:
    case IqAppProps::prpGrid:
    case IqAppProps::prpTree:
    case IqAppProps::prpPalette:
    case IqAppProps::prpFileDialog:
    case IqAppProps::prpCategory:
      break;
    case IqAppProps::prpReference:
      if (m_mapValues.find(qsValue) != m_mapValues.end())
        vValue = m_mapValues[qsValue];
      else
        ODA_FAIL_ONCE(); // TODO (now all references are placed at end
      break;
    } // end switch

    ODA_ASSERT_ONCE(m_mapValues.find(qsPath) == m_mapValues.end());
    m_mapValues[qsPath] = vValue;
  }
}

QStringList OdqXmlUtils::keys()
{
  checkMaps();

  ODA_ASSERT_ONCE(m_keys.size() == m_mapElements.keys().size());
  return m_keys;
}

QDomElement OdqXmlUtils::getElement(const QString& qsPropPath)
{
  ODA_ASSERT_ONCE(!qsPropPath.isEmpty());
  checkMaps();

  QHash<QString, QDomElement>::iterator itr = m_mapElements.find(qsPropPath);
  if (itr == m_mapElements.end())
    return QDomElement(); // was not found

  return itr.value();
}

QDomElement OdqXmlUtils::getElement(const QString& qsPropPath, bool bIgnoreCache)
{
  if (!bIgnoreCache)
    return getElement(qsPropPath);

  QDomElement elemRoot = m_docXml.documentElement();
  QDomNodeList lstProps = elemRoot.elementsByTagName("Property");
  for (uint index = 0, sz = (uint)lstProps.length(); index < sz; index++) // foreach is unsupported for QDomNodeList
  {
    QDomElement elem = lstProps.at(index).toElement();
    ODA_ASSERT_ONCE(!elem.isNull());
    QString qsPath = getElemPath(elem);
    if (qsPath == qsPropPath)
      return elem;
  }

  return QDomElement();
}

//QDomElement OdqXmlUtils::getElement(const OdString& sPropPath)
//{
//  return getElement(getIApp()->toQString(sPropPath));
//}

bool OdqXmlUtils::getValue(const OdString& sPropPath, OdVariant& vValue)
{
  ODA_ASSERT_ONCE(!sPropPath.isEmpty());
  checkMaps();

  QHash<QString, OdVariant>::iterator itr = m_mapValues.find(getIApp()->toQString(sPropPath));
  if (itr == m_mapValues.end())
    return false; // was not found

  vValue = itr.value();
  return true;
}

IqAppProps::TPropType OdqXmlUtils::getType(const QString& qsPropPath)
{
  QDomElement elem = getElement(qsPropPath);
  if (elem.isNull())
    return IqAppProps::prpNone;

  IqAppProps::TPropType type = IqAppProps::prpNone;
  while (true)
  {
    QString qsType = elem.attribute("Type", 
                                    "string").toLower(); // default
    type = toType(qsType);
    if (type != IqAppProps::prpReference)
      break;
    QString qsRefPath = elem.attribute("Value");
    elem = getElement(qsRefPath);
    if (elem.isNull())
    {
      ODA_FAIL_ONCE(); // test
      return IqAppProps::prpNone;
    }
  }

  return type;
}

bool OdqXmlUtils::setValue(const OdString& sPropPath, 
                           const OdVariant& vValue,
                           const OdString& sTypeToCreate) // = OdString::kEmpty // only override is enabled
{
  QString qsPropPath = getIApp()->toQString(sPropPath);
  QDomElement elem = getElement(qsPropPath);
  if (elem.isNull())
  {
    ODA_ASSERT_ONCE(!sTypeToCreate.isEmpty());
    if (sTypeToCreate.isEmpty())
      return false;

    int pos = qsPropPath.lastIndexOf("/");
    if (pos <= 0)
    {
      ODA_FAIL_ONCE(); // parent was not found
      return false;
    }
    QString qsName = qsPropPath.mid(pos + 1);
    ODA_ASSERT_ONCE(!qsName.isEmpty());
    qsPropPath = qsPropPath.left(pos);

    QDomElement elemRoot = m_docXml.documentElement();
    if (elemRoot.isNull() || elemRoot.tagName() != m_qsRootNodeName) // "Properties"
    {
      ODA_FAIL_ONCE(); // ?TODO
      return false;
    }

    elem = elemRoot.appendChild(m_docXml.createElement("Property")).toElement();
    if (elem.isNull())
    {
      ODA_FAIL_ONCE();
      return false;
    }
    elem.setAttribute("Path", qsPropPath);
    elem.setAttribute("Name", qsName);
    elem.setAttribute("Type", getIApp()->toQString(sTypeToCreate).toLower());
    elem.setAttribute("Hidden", "true");
    // m_keys, m_mapElements, m_mapValues will be recreated
  }

  IqAppProps::TPropType type = IqAppProps::prpNone;
  while (true)
  {
    QString qsType = elem.attribute("Type", "string").toLower();
    type = toType(qsType);
    if (type != IqAppProps::prpReference)
      break;
    QString qsRefPath = elem.attribute("Value");
    elem = getElement(qsRefPath);
    if (elem.isNull())
    {
      ODA_FAIL_ONCE(); // test
      return false;
    }
  }

  bool bUnsigned = (type == IqAppProps::prpColorref);
  QString qsValue = getIApp()->toQString(toString(vValue, bUnsigned));
  if (type == IqAppProps::prpColorref)
    qsValue = QString("0x%1").arg(toColorref(vValue), 6, 16, QLatin1Char('0'));
  elem.setAttribute("Value", qsValue);
  m_keys.clear();
  m_mapElements.clear();
  m_mapValues.clear();
  m_bModified = true;
  return true;
}

// static 
IqAppProps::TPropType OdqXmlUtils::toType(const QString& cqsType)
{
  QString qsType = cqsType.toLower();

  static QHash<QString, IqAppProps::TPropType> s_map;
  if (s_map.isEmpty())
  {
    s_map["string"] = IqAppProps::prpString;
    s_map["combo"] = IqAppProps::prpCombo;
    s_map["bool"] = IqAppProps::prpBool;
    s_map["boolpictured"] = IqAppProps::prpBoolPictured;
    s_map["boolnamed"] = IqAppProps::prpBoolNamed;
    s_map["button"] = IqAppProps::prpBoolNamed;
    s_map["radiobutton"] = IqAppProps::prpRadio;
    s_map["radio"] = IqAppProps::prpRadio;
    s_map["int"] = IqAppProps::prpInt;
    s_map["double"] = IqAppProps::prpDouble;
    s_map["distance"] = IqAppProps::prpDistance;
    s_map["x"] = s_map["y"] = s_map["z"] = IqAppProps::prpDistance;
    s_map["angle"] = IqAppProps::prpAngle;
    s_map["colorref"] = IqAppProps::prpColorref;
    s_map["odcolor"] = IqAppProps::prpOdColor;
    s_map["linetype"] = IqAppProps::prpLinetype;
    s_map["lineweight"] = IqAppProps::prpLineweight;
    s_map["transparency"] = IqAppProps::prpTransparency;
    s_map["folder"] = IqAppProps::prpFolder;
    s_map["image"] = IqAppProps::prpImage;
    s_map["grid"] = IqAppProps::prpGrid;
    s_map["tree"] = IqAppProps::prpTree;
    s_map["palette"] = IqAppProps::prpPalette;
    s_map["filedialog"] = IqAppProps::prpFileDialog;
    s_map["category"] = IqAppProps::prpCategory;
    s_map["ref"] = IqAppProps::prpReference;
  }
  
  QHash<QString, IqAppProps::TPropType>::Iterator itr = s_map.find(qsType);
  if (itr == s_map.end())
  {
    ODA_FAIL_ONCE();
    return IqAppProps::prpNone;
  }

  IqAppProps::TPropType type = itr.value();
  return type;
}

// static
OdString OdqXmlUtils::toString(const OdVariant& vValue, 
                               bool bIntAsUnsigned) // = false
{
  OdString sValue;
  switch (vValue.varType())
  {
  case OdVariant::kVoid:
    break;
  case OdVariant::kBool:
    sValue = vValue.getBool() ? "true" : "false";
    break;
  case OdVariant::kString:
    sValue = vValue.getString();
    break;
  case OdVariant::kInt8: // == OdVariant::kUInt8
    if (bIntAsUnsigned)
      sValue = getIApp()->toOdString(QString::number(vValue.getUInt8()));
    else
      sValue = getIApp()->toOdString(QString::number(vValue.getInt8()));
    break;
  case OdVariant::kInt16: // == OdVariant::kUInt16
    if (bIntAsUnsigned)
      sValue = getIApp()->toOdString(QString::number(vValue.getUInt16()));
    else
      sValue = getIApp()->toOdString(QString::number(vValue.getInt16()));
    break;
  case OdVariant::kInt32: // == OdVariant::kUInt32
    if (bIntAsUnsigned)
      sValue = getIApp()->toOdString(QString::number(vValue.getUInt32()));
    else
      sValue = getIApp()->toOdString(QString::number(vValue.getInt32()));
    break;
  case OdVariant::kInt64: // == OdVariant::kUInt64
    if (bIntAsUnsigned)
      sValue = getIApp()->toOdString(QString::number(vValue.getUInt64(), 16)).makeUpper();
    else
      sValue = getIApp()->toOdString(QString::number(vValue.getInt64()));
    break;
  case OdVariant::kDouble:
    sValue = getIApp()->toOdString(QString::number(vValue.getDouble()));
    break;
  case OdVariant::kWString: // // == OdVariant::kAnsiString
    sValue = OdString(vValue.getAnsiString());
    break;
  //case OdVariant::kRxObjectPtr:
  default:
    ODA_FAIL_ONCE(); // TODO
    break;
  }
  return sValue;
}

// static
bool OdqXmlUtils::toBool(const OdVariant& vValue)
{
  bool bValue = false;
  switch (vValue.varType())
  {
  case OdVariant::kBool:
    bValue = vValue.getBool();
    break;
  case OdVariant::kString:
    bValue = OdString(vValue.getString()).makeLower() == OD_T("true");
    break;
  case OdVariant::kInt8: // == OdVariant::kUInt8
    bValue = (vValue.getInt8() != 0);
    break;
  case OdVariant::kInt16: // == OdVariant::kUInt16
    bValue = (vValue.getInt16() != 0);
    break;
  case OdVariant::kInt32: // == OdVariant::kUInt32
    bValue = (vValue.getInt32() != 0);
    break;
  case OdVariant::kInt64: // == OdVariant::kUInt64
    bValue = (vValue.getInt64() != 0);
    break;
  case OdVariant::kWString: // == OdVariant::kAnsiString
    bValue = OdString(vValue.getAnsiString()).makeLower() == OD_T("true");
    break;
  case OdVariant::kRxObjectPtr:
    bValue = !vValue.getRxObjectPtr().isNull();
    break;
  //case OdVariant::kVoid:
  //case OdVariant::kDouble:
  //case OdVariant::kNextType:
  //case OdVariant::kByRef:
  //case OdVariant::kArray:
  default:
    ODA_FAIL_ONCE(); // TODO
    break;
  } // end switch
  return bValue;
}

//static 
int OdqXmlUtils::toInt(const OdVariant& vValue)
{
  int iValue = 0;
  switch (vValue.varType())
  {
  case OdVariant::kBool:
    iValue = vValue.getBool() ? 1 : 0;
    break;
  case OdVariant::kString:
    iValue = getIApp()->toQString(vValue.getString()).toInt();
    break;
  case OdVariant::kInt8: // == OdVariant::kUInt8
    iValue = vValue.getInt8();
    break;
  case OdVariant::kInt16: // == OdVariant::kUInt16
    iValue = vValue.getInt16();
    break;
  case OdVariant::kInt32: // == OdVariant::kUInt32
    iValue = vValue.getInt32();
    break;
  case OdVariant::kInt64: // == OdVariant::kUInt64
    iValue = (int) vValue.getInt64();
    break;
  case OdVariant::kWString: // == OdVariant::kAnsiString
    iValue = getIApp()->toQString(OdString(vValue.getAnsiString())).toInt();
    break;
  case OdVariant::kRxObjectPtr:
#  ifdef _MSC_VER
#   pragma warning(push)
#   pragma warning(disable:4311)
#   pragma warning(disable:4302)
#  endif
    iValue = (int)(long) vValue.getRxObjectPtr().get();
#  ifdef _MSC_VER
#   pragma warning(pop)
#  endif
    break;
  //case OdVariant::kVoid:
  //case OdVariant::kDouble:
  //case OdVariant::kNextType:
  //case OdVariant::kByRef:
  //case OdVariant::kArray:
  default:
    ODA_FAIL_ONCE(); // TODO
    break;
  } // end switch
  return iValue;
}

//static
double OdqXmlUtils::toDouble(const OdVariant& vValue)
{
  double dValue = 0;
  switch (vValue.varType())
  {
  case OdVariant::kBool:
    dValue = vValue.getBool() ? 1.0 : 0.0;
    break;
  case OdVariant::kString:
    dValue = getIApp()->toQString(vValue.getString()).toDouble();
    break;
  case OdVariant::kInt8: // == OdVariant::kUInt8
    dValue = vValue.getInt8();
    break;
  case OdVariant::kInt16: // == OdVariant::kUInt16
    dValue = vValue.getInt16();
    break;
  case OdVariant::kInt32: // == OdVariant::kUInt32
    dValue = vValue.getInt32();
    break;
  case OdVariant::kInt64: // == OdVariant::kUInt64
    dValue = (double) vValue.getInt64();
    break;
  case OdVariant::kWString: // == OdVariant::kAnsiString
    dValue = getIApp()->toQString(OdString(vValue.getAnsiString())).toDouble();
    break;
  //case OdVariant::kRxObjectPtr:
  //case OdVariant::kVoid:
  //case OdVariant::kDouble:
  //case OdVariant::kNextType:
  //case OdVariant::kByRef:
  //case OdVariant::kArray:
  default:
    ODA_FAIL_ONCE(); // TODO
    break;
  } // end switch
  return dValue;
}

// static
ODCOLORREF OdqXmlUtils::toColorref(const OdVariant& vValue)
{
  QString qsValue = getIApp()->toQString(toString(vValue, true)).toLower();
  bool ok = false;
  int base = 10;
  if (!qsValue.indexOf("0x"))
  {
    base = 16;
    qsValue = qsValue.mid(2);
  }
  uint uiValue = qsValue.toULong(&ok, base);
  if (!ok)
  {    
    ODA_ASSERT_ONCE(ok);
    return 0;
  }
  return (ODCOLORREF) uiValue;
}
