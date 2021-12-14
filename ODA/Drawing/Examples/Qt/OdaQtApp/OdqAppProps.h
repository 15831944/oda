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
// OdqAppProps.h
//

#ifndef ODA_QT_APP_PROPS_H_
#define ODA_QT_APP_PROPS_H_

#include <QDomDocument>
#include <QMap>

#include "OdaCommon.h"

#include "OdqInterfaces.h"
#include "OdqXmlUtils.h" 

class OdqAppProps : public QObject
                  , public IqAppProps
{
  Q_OBJECT
  Q_INTERFACES(IqAppProps)

  OdqAppProps(); 
  // single instance via 
  friend class OdqApplication;
public:
  virtual ~OdqAppProps();

  // ----- IqAppProps -----

  virtual QList<OdString> subpaths(const OdString& sParentPath, 
                                   bool bSkipHidden = true); 

  virtual TPropType type(const OdString& sPropPath);
  virtual bool isHidden(const OdString& sPropPath);
  virtual bool set(const OdString& sPropPath, const OdVariant& vValue,
                   const OdString& sTypeToCreate = OdString::kEmpty); // only override is enabled

  virtual OdString toString(const OdString& sPropPath, 
                            const OdString& sDefValue = OdString::kEmpty);
  virtual bool toBool(const OdString& sPropPath, 
                       bool bDefValue = false);
  virtual int toInt(const OdString & sPropPath, 
                    int iDefValue = 0);
  virtual double toDouble(const OdString& sPropPath, 
                           double dDefValue = 0.0);
  virtual ODCOLORREF toColorref(const OdString& sPropPath, 
                                ODCOLORREF clrDefValue = ODRGB(0, 0, 0));

  // ----- OdqAppProps -----

  TPropType type(const QString& qsPropPath);
  bool isHidden(const QString& qsPropPath);

  OdString variantToString(const OdVariant& vValue,
                           const OdString& sDefValue = OdString::kEmpty);
  bool variantToBool(const OdVariant& vValue,
                     bool bDefValue = false);
  int variantToInt(const OdVariant& vValue,
                   int iDefValue = 0);
  double variantToDouble(const OdVariant& vValue,
                         double dDefValue = 0.0);
  ODCOLORREF variantToColorref(const OdVariant& vValue,
                               ODCOLORREF clrDefValue = ODRGB(0, 0, 0));

  bool load(const QString& qsXmlFilePath);
  OdStringArray resetRegVariables();
  void save();

private:
  OdqXmlUtils m_utlXml;
};

#endif // ODA_QT_APP_PROPS_H_
