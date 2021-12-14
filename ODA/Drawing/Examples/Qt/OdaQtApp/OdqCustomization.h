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
// OdqCustomization.h
//

#ifndef ODA_QT_CUSTOMIZATION_H_
#define ODA_QT_CUSTOMIZATION_H_

#include <QMap>
#include <QHash>
#include <QPointer>
#include <QStringList>

#include "OdaCommon.h"
#include "SharedPtr.h"

#include "OdqInterfaces.h"

QT_BEGIN_NAMESPACE
  class QAction;
  class QMenu;
  class QToolBar;
  class QDomElement;
QT_END_NAMESPACE

///////////////////////////////////////////////////////////////////////////////

class OdqMenuMacro : public QObject
{
  Q_OBJECT
public:
  OdqMenuMacro(const QString& qsCommand, const QString& qsIconPath = "");
  virtual ~OdqMenuMacro();

  QAction* action() const;
  void setAction(QAction* pAction);

  bool isReferenced() const;

  // evaluated via diesel
  QString getCommand(bool bEvaluated = true) const;
  QString getIconPath(bool bEvaluated = true) const;
  bool hasIconPathDiesel() const;

  QIcon getIcon() const;

  void updateIconToAction();

  static OdSharedPtr<OdqMenuMacro> createMenuControlMacro(const QString& qsUID,
                                                          class OdqCustomization* pCustomization);
  static OdSharedPtr<OdqMenuMacro> createToolbarControlMacro(const QString& qsUID,
                                                             const QString& qsControl);
protected slots:
  void execute() const;

protected:
  QPointer<QAction> m_pAction;
  QString m_qsCommand,
          m_qsIconPath;
private:
  Q_DISABLE_COPY(OdqMenuMacro)
};

///////////////////////////////////////////////////////////////////////////////
// Drawing history, Windows etc

class OdqMenuControlMacro : public OdqMenuMacro
{
  Q_OBJECT
public:
  OdqMenuControlMacro(const QString& qsEventKeys = "");
  virtual ~OdqMenuControlMacro();

  QString getEventKey(int indexKey = 0) const;

  virtual bool isMenu() const;
  virtual bool isVisible() const;
  virtual bool isEnabled() const;
  virtual void refreshData(const QString& qsKey, const QString& qsArg) = 0;

public slots:
  void refresh(const QString& qsKey, const QString& qsArg); // refresh states

protected:
  QString m_qsEventKeys;

private:
  Q_DISABLE_COPY(OdqMenuControlMacro)
};

///////////////////////////////////////////////////////////////////////////////

class OdqToolbarControlMacro : public OdqMenuMacro
{
  Q_OBJECT
public:
  OdqToolbarControlMacro(const QString& qsButtonText, // tooltip
                         const QString& qsCommand, const QString& qsIconPath,
                         const QString& qsEventKeys = "");
  virtual ~OdqToolbarControlMacro();

  virtual QString text(bool bEvaluated = true) const; // tooltip
  virtual bool isVisible() const;
  virtual bool isEnabled() const;

public slots:
  void refresh(const QString& qsKey, const QString& qsArg); // refresh states

protected:
  QString m_qsButtonText,
          m_qsEventKeys;

private:
  Q_DISABLE_COPY(OdqToolbarControlMacro)
};

///////////////////////////////////////////////////////////////////////////////

class OdqCustomization : public QObject
{
  Q_OBJECT
  OdqCustomization(); friend class OdqApplication;
public:
  virtual ~OdqCustomization();
  void saveSettings(bool bToolbarsOnly = false);

  QStringList getCustomGroups();
  QString getLoadedCuiCustomGroupNameByFileName(const QString& qsCuiFileName) const;

  bool unloadCustomGroup(const QString& qsGroupName);

  // return name of loaded custom group
  QString loadCustomGroup(const QString& qsXmlFilePath);

  void refreshCustomGroupWidgets(bool bSkipSaveToolbars = false); // now for menu only

public slots:
  void refresh(const QString& qsKey, const QString& qsArg); // refresh states of widgets (now for menu only)

private:
  void fillPopMenuItems(QMenu* pVrtMenu, 
                        QDomElement elemPopMenu,
                        class OdqCustomGroup* pCustomGroup);
  void fillToolbarButtons(QToolBar* pToolbar, 
                          QDomElement elemTollbar,
                          class OdqCustomGroup* pCustomGroup);

  QStringList m_lstCustomGroups; // in loading order
  QHash<QString, QString> m_mapGroupNameByFilePath;
  QHash<QString, OdSharedPtr<OdqCustomGroup> > m_mapCustomGroups;
  QMap<QPointer<QObject>, QString> m_mapConditionalTexts;

  // drawing history
  friend class OdqMenuControlDrawingHistory;
  int m_sizeMaxDrawingHistory;
  QStringList m_lstDrawingHistory;
};

#endif // ODA_QT_CUSTOMIZATION_H_

