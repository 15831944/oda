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
// OdqPropSystem.h
//

#ifndef ODA_QT_PROP_SYSTEM_H_
#define ODA_QT_PROP_SYSTEM_H_

#include <QPointer>
#include <QMap>

QT_BEGIN_NAMESPACE
  class QDockWidget;
  class QTabWidget;
QT_END_NAMESPACE

//#include "OdaCommon.h"

#include "OdqInterfaces.h"
#include "OdqPropTab.h"

//////////////////////////////////////////////////////////////////////////

class OdqPropSystem : public QObject
                    , public IqPropSystem
{
  Q_OBJECT
  Q_INTERFACES(IqPropSystem)

  OdqPropSystem(); 
  void saveSettings();
  friend class OdqPropSystemModule;

public:
  virtual ~OdqPropSystem();

  // ----- IqPropSystem -----

  virtual bool isPropVisible() const;
  virtual ODGSPALETTE& cacheColors(OdRxObject* pRxDatabase) const; // for color palette via ExtDb

  // ----- OdqPropSystem -----

  static OdqPropSystem& getPropSystem();
  static QDockWidget* getDockPalette(); 

  static OdqPropTab* getPropTab(OdRxObject* pRxDatabase,
                                bool bSetCurrentAndCreateIfMissing = false,
                                bool bCreateIfMissing = false);
  static bool removePropTab(OdRxObject* pRxDatabase);
  static bool removePropTab(OdqPropTab &tab);

public slots:
  void refresh(const QString& qsKey, const QString& qsArg); // refresh states of widgets

private slots:
  void currentTabChanged(int index);

private:
  OdSharedPtr<QDockWidget> m_pDockPalette;

  QMap<OdRxObject*, OdSharedPtr<OdqPropTab> > m_mapTabs; // via rx database (base database) as key
  QPointer<QTabWidget> m_pTabWidget; // is m_pDockPalette->widget()
};

#endif // ODA_QT_PROP_SYSTEM_H_
