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
// OdqXmlUtils.h
//

#ifndef ODA_QT_XML_UTILS_H_
#define ODA_QT_XML_UTILS_H_

#include <QDomDocument>
#include <QHash>
#include <QStringList>

#include "OdaCommon.h"

#include "OdqInterfaces.h"

class OdqXmlUtils
{
public:
  OdqXmlUtils(); 
  virtual ~OdqXmlUtils();

  // ----- OdqXmlUtils -----

  bool isLoaded() const;
  const QString& getBasePath() const;

  // return message if error
  QString loadXmlFile(const QString& qsXmlFilePath, 
                      bool bCheckUserOverrideXmlFile = true);
  QString loadXmlFromString(const QString& qsXml, 
                            const QString& qsBasePath = "", // all
                            const QString& qsRootNodeName = "Properties");
  bool setMergeFilter(const QString& qsMergeFilter = "", // all
                      const QString& qsMergeFilterAttribName = "", // is already set
                      const QString& qsUnmergedBasePath = ""); // is already set

  bool saveXmlFile();
  bool saveXmlFile(const QString& qsXmlFilePath);

  QStringList keys();

  //QDomElement getElement(const OdString& sPropPath);
  QDomElement getElement(const QString& qsPropPath);
  QDomElement getElement(const QString& qsPropPath, bool bIgnoreCache);

  IqAppProps::TPropType getType(const QString& sPropPath);

  bool getValue(const OdString& sPropPath, OdVariant& vValue);
  bool setValue(const OdString& sPropPath, const OdVariant& vValue, 
                const OdString& sTypeToCreate = OdString::kEmpty); // only override is enabled

  // ---

  static IqAppProps::TPropType toType(const QString& cqsType);

  static OdString toString(const OdVariant& vValue, bool bIntAsUnsigned = false);
  static bool toBool(const OdVariant& vValue);
  static int toInt(const OdVariant& vValue);
  static double toDouble(const OdVariant& vValue);
  static ODCOLORREF toColorref(const OdVariant& vValue);

  static QString getUserOverrideXmlFilePath(const QString& qsXmlFilePath,
                                            bool bCreateSubFolders = false);

  // ---
  // TODO : move here xml service functions from OdqCustomization.cpp

private:
  void checkMaps();
  bool merge();

  QString m_qsXmlFilePath;
  QDomDocument m_docXml;
  bool m_bModified;
  QStringList m_keys; // in source order (differ to m_mapElements.keys())
  QHash<QString, QDomElement> m_mapElements;
  QHash<QString, OdVariant> m_mapValues;

  QString m_qsRootNodeName, // "Properties"
          m_qsBasePath, // "" - all
          m_qsUnmergedBasePath, // "" - without merging
          m_qsMergeFilter, // "" - all
          m_qsMergeFilterAttribName; // "Filter"
};

#endif // ODA_QT_XML_UTILS_H_
