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
// OdqRevSystem.h
//

#ifndef ODA_QT_REV_SYSTEM_H_
#define ODA_QT_REV_SYSTEM_H_

#include <QPointer>
#include <QMap>

QT_BEGIN_NAMESPACE
  class QDockWidget;
  class QTabWidget;
QT_END_NAMESPACE

//#include "OdaCommon.h"

#include "OdqInterfaces.h"
#include "OdqRevTab.h"
#include "RevisionController.h"

//////////////////////////////////////////////////////////////////////////

class OdqRevSystem : public QObject
                   , public IqRevSystem
{
  Q_OBJECT
  Q_INTERFACES(IqRevSystem)

  OdqRevSystem(); 
  void saveSettings();
  friend class OdTigDbModuleImpl;

public:
  virtual ~OdqRevSystem();

  // ----- IqRevSystem -----

  virtual IqRevDiff* getIRevDiff(IqView* iView);
  virtual bool isRevVisible() const;
  virtual bool isUnder(OdRxObject* pRxDb) const;

  // ----- OdqRevSystem -----

  static OdqRevSystem& getRevSystem();
  static QDockWidget* getDockPalette(); 

  static OdqRevTab* getRevTab(OdRxObject* pRxDatabase,
                              bool bSetCurrentAndCreateIfMissing = false,
                              bool bCreateIfMissing = false);
  static bool removeRevTab(OdRxObject* pRxDatabase);
  static bool removeRevTab(OdqRevTab& tab);

  OdqRevisionController* controller() const;

public slots:
  void refresh(const QString& qsKey, const QString& qsArg); // refresh states of widgets

private slots:
  void currentTabChanged(int index);

private:
  OdSharedPtr<QDockWidget> m_pDockPalette;

  QMap<OdRxObject*, OdSharedPtr<OdqRevTab> > m_mapTabs; // via rx database (base database) as key
  QPointer<QTabWidget> m_pTabWidget; // is m_pDockPalette->widget()

  mutable OdSharedPtr<OdqRevisionController> m_pRevCtrl; // g_RevisionCtrl;
};

#endif // ODA_QT_REV_SYSTEM_H_
