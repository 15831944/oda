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
// OdqAppProps.cpp
//

#include <QtGui>
#include <QMessageBox>
#include <QMainWindow>

#include "OdaCommon.h"
//#include "DbBaseHostAppServices.h"

#include "OdqAppProps.h" 

//////////////////////////////////////////////////////////////////////////

OdqAppProps::OdqAppProps()
{
}

OdqAppProps::~OdqAppProps()
{
}

bool OdqAppProps::load(const QString& qsXmlFilePath)
{
  QString qsErrMsg = m_utlXml.loadXmlFile(qsXmlFilePath);
  if (qsErrMsg.isEmpty())
    return true;

  //getIApp()->getBaseHostAppServices()->warning(getIApp()->toOdString(qsErrMsg));
  // console can be not loaded yet
  QMessageBox::critical(getIApp()->getMainWindow(), 
                        QObject::tr("%1 Error").arg(getIApp()->getAppName()), // caption
                        qsErrMsg, QMessageBox::Ok, QMessageBox::Ok);
  return false;
}

static bool lessThan(const OdString& s1, const OdString& s2)
{
  return s1 < s2;
}

OdStringArray OdqAppProps::resetRegVariables()
{
  QList<OdString> lstNames = subpaths(OD_T("Vars/")),
                  lstPrevValues; 
  qSort(lstNames.begin(), lstNames.end(), lessThan);
  foreach (OdString sName, lstNames)
  {
    OdString sVarPath = OD_T("Vars/");
    sVarPath += OdString(sName).makeLower();
    OdString sValue = getIAppProps()->toString(sVarPath);
    lstPrevValues << sValue;
  }

  OdStringArray vars;

  QString qsErrMsg = m_utlXml.loadXmlFile("", false);
  if (!qsErrMsg.isEmpty())
  {
    QMessageBox::critical(getIApp()->getMainWindow(), 
                          QObject::tr("%1 Error").arg(getIApp()->getAppName()), // caption
                          qsErrMsg, QMessageBox::Ok, QMessageBox::Ok);
    return vars;
  }

  QList<OdString> lstRestored = subpaths(OD_T("Vars/"));
  int index = 0;
  foreach (OdString sName, lstNames)
  {
    OdString sPrevValue = lstPrevValues.value(index++);

    if (!lstRestored.contains(sName))
      continue;

    OdString sVarPath = OD_T("Vars/");
    sVarPath += sName.makeLower();
    OdString sValue = getIAppProps()->toString(sVarPath);
    if (sValue != sPrevValue)
      vars.push_back(sName.makeUpper());
  }

  return vars;
}

void OdqAppProps::save()
{
  m_utlXml.saveXmlFile();
}

QList<OdString> OdqAppProps::subpaths(const OdString& csParentPath,
                                      bool bSkipHidden) // = true
{
  ODA_ASSERT_ONCE(!csParentPath.isEmpty());
  QString qsParentPath = getIApp()->toQString(csParentPath);
  qsParentPath.replace('\\', '/');
  int pos = qsParentPath.lastIndexOf('/');
  if (pos != (qsParentPath.length()  - 1))
    qsParentPath += '/';

  QList<OdString> lstPaths;

  foreach (QString qsPath, m_utlXml.keys())
  {
    if (qsPath.indexOf(qsParentPath))
      continue;
    if (bSkipHidden && isHidden(qsPath))
      continue;
    
    qsPath.remove(qsParentPath);
    lstPaths << getIApp()->toOdString(qsPath);
  }

  return lstPaths;
}  

IqAppProps::TPropType OdqAppProps::type(const OdString& sPropPath)
{
  return type(getIApp()->toQString(sPropPath));
}

IqAppProps::TPropType OdqAppProps::type(const QString& qsPropPath)
{
  return m_utlXml.getType(qsPropPath);
}

bool OdqAppProps::isHidden(const OdString& sPropPath)
{
  return isHidden(getIApp()->toQString(sPropPath));
}

bool OdqAppProps::isHidden(const QString& qsPropPath)
{
  QDomElement elem = m_utlXml.getElement(qsPropPath);
  bool bHidden = true;
  if (!elem.isNull())
    bHidden = variantToBool(getIApp()->toOdString(elem.attribute("Hidden", "false")));
  return bHidden;
}

bool OdqAppProps::set(const OdString& sPropPath, const OdVariant& vValue,
                      const OdString& sTypeToCreate) // = OdString::kEmpty // only override is enabled
{
  return m_utlXml.setValue(sPropPath, vValue, sTypeToCreate);
}

OdString OdqAppProps::toString(const OdString& sPropPath, 
                               const OdString& sDefValue) // = OdString::kEmpty
{
  OdVariant vValue;
  if (!m_utlXml.isLoaded() || !m_utlXml.getValue(sPropPath, vValue))
    return sDefValue;
  return OdqXmlUtils::toString(vValue); // , type(sPropPath) == IqAppProps::prpColorref);
}

bool OdqAppProps::toBool(const OdString& sPropPath, 
                         bool bDefValue) // = false
{
  OdVariant vValue;
  if (!m_utlXml.getValue(sPropPath, vValue))
    return bDefValue;
  return OdqXmlUtils::toBool(vValue);
}

int OdqAppProps::toInt(const OdString & sPropPath, 
                       int iDefValue) // = 0
{
  OdVariant vValue;
  if (!m_utlXml.getValue(sPropPath, vValue))
    return iDefValue;
  return OdqXmlUtils::toInt(vValue);
}

double OdqAppProps::toDouble(const OdString& sPropPath, 
                             double dDefValue) // = 0.0
{
  OdVariant vValue;
  if (!m_utlXml.getValue(sPropPath, vValue))
    return dDefValue;
  return OdqXmlUtils::toDouble(vValue);
}

ODCOLORREF OdqAppProps::toColorref(const OdString& sPropPath, 
                                   ODCOLORREF clrDefValue) // = ODRGB(0, 0, 0)
{
  OdVariant vValue;
  if (!m_utlXml.getValue(sPropPath, vValue))
    return clrDefValue;
  return OdqXmlUtils::toColorref(vValue);
}

OdString OdqAppProps::variantToString(const OdVariant& vValue, 
                                      const OdString& sDefValue) // = OdString::kEmpty
{
  if (vValue.type() == OdVariant::kVoid) // undefined
    return sDefValue;
  return OdqXmlUtils::toString(vValue); // , type(sPropPath) == IqAppProps::prpColorref);
}

bool OdqAppProps::variantToBool(const OdVariant& vValue, 
                                bool bDefValue) // = false
{
  if (vValue.type() == OdVariant::kVoid)
    return bDefValue;
  return OdqXmlUtils::toBool(vValue);
}

int OdqAppProps::variantToInt(const OdVariant& vValue, 
                              int iDefValue) // = 0
{
  if (vValue.type() == OdVariant::kVoid)
    return iDefValue;
  return OdqXmlUtils::toInt(vValue);
}

double OdqAppProps::variantToDouble(const OdVariant& vValue, 
                                    double dDefValue) // = 0.0
{
  if (vValue.type() == OdVariant::kVoid)
    return dDefValue;
  return OdqXmlUtils::toDouble(vValue);
}

ODCOLORREF OdqAppProps::variantToColorref(const OdVariant& vValue, 
                                          ODCOLORREF clrDefValue) // = ODRGB(0, 0, 0)
{
  if (vValue.type() == OdVariant::kVoid)
    return clrDefValue;
  return OdqXmlUtils::toColorref(vValue);
}
