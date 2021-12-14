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
// OdqPropertyPageDialog.h
//

#ifndef ODA_QT_PROPERTY_PAGE_DIALOG_H_
#define ODA_QT_PROPERTY_PAGE_DIALOG_H_

#include "OdaCommon.h"

#include "SharedPtr.h"
#include "OdqXmlUtils.h"
#include "OdqPropertyDialog.h"
#include "PropServices.h"

QT_BEGIN_NAMESPACE
  class QLabel;
  class QBoxLayout;
  class QScrollArea;
QT_END_NAMESPACE

///////////////////////////////////////////////////////////////////////////////

#include <QDialog> 
#include <QObject>
#include <QDialog>
#include <QTreeWidget>
#include <QList>
#include <QStackedLayout>
#include <QAbstractItemModel>
#include <QEvent>
#include <QLayout>
#include <QMainWindow>

class OdqPropertyPageDialog : public OdqPropertyDialog
{
  Q_OBJECT

public:
  OdqPropertyPageDialog(QWidget* pParent = NULL,
                        Qt::WindowFlags flags = 0,
                        OdPropServices::TSeparateCategoryTreePlace catPlace = OdPropServices::enSepCatPlace_Left);

  virtual ~OdqPropertyPageDialog();

  bool setCurrentPage(const QString &qsFullName);
  // return false if there is no file wrappers
  //bool setRestoredFilePropertyWrappersByCancel();

  bool wasCanceled() const;

  //void addMenuXml(const QString &qsMenuXml);
  //void addToolbarXml(const QString &qsToolbarXml);

  int exec();

  //void saveSettings();
  //void restoreSettings();
  QAbstractItemModel* model();
  void setModel(QAbstractItemModel* pModel);

public slots:
  void currentCategoryChanged(QTreeWidgetItem* current);
  void propertyEdited(QWidget* editor);
  void modelReset();
  void valueRejected(QModelIndex index, QString errorMessage);
  void valueAccepted();
  void layoutChanged();
  void expand(const QModelIndex& index);
  void collapse(const QModelIndex& index);

private slots:
  void emitExpanded(QTreeWidgetItem* pItem);
  void emitCollapsed(QTreeWidgetItem* pItem);
  void accept();
  void reject();

signals:
  void expanded(const QModelIndex& index);
  void collapsed(const QModelIndex& index);

private:
  void updatePage(const QModelIndex& qsPageIndex);
  void addEditors(const QModelIndex& parent, QBoxLayout* pLayout);
  void fillCategoriesTree(QModelIndex index, QTreeWidgetItem* item);
  QWidget* createEditor(QModelIndex index);
  QWidget* createLabel(QModelIndex index);
  //OdqPropControlEnabler* createEnabler(QModelIndex index, QWidget* pParent);
  QTreeWidgetItem *lookupCategory(const QModelIndex& index);
  QAbstractItemView *createGrid(const QModelIndex& index);

  QPointer<QTreeWidget> m_categories;
  QPointer<OdqPropertyModel> m_model;
  
  QPointer<QLabel> m_warning;
  QPointer<QWidget> m_pPage;
  QPointer<QScrollArea> m_pScroll;
  QPersistentModelIndex m_pageIndex;
  //bool m_bFilePropertyWrappersWereSaved;
  bool m_bWasCanceled;

  //IqPtr<IqToolbars> m_iToolbars;
  //IqPtr<IqMenu> m_iMenu;
};

#endif // ODA_QT_PROPERTY_PAGE_DIALOG_H_
