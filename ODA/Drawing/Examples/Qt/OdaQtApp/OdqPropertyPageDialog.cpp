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
// OdqPropertyPageDialog.cpp
//

#include <QtCore>
#include <QScrollArea>
#include <QSplitter>
#include <QGroupBox>
#include <QLabel>
#include <QHeaderView>
#include <QBitmap>

#include "OdaCommon.h"
#include "OdqPropertyPageDialog.h" 

//////////////////////////////////////////////////////////////////////////

OdqPropertyPageDialog::OdqPropertyPageDialog(QWidget* pParent, // = NULL
                                             Qt::WindowFlags flags, // = 0
                                             OdPropServices::TSeparateCategoryTreePlace catPlace) // = OdPropServices::enSepCatPlace_Left
  : OdqPropertyDialog(pParent, flags)
  //, m_bFilePropertyWrappersWereSaved(false)
  , m_bWasCanceled(false)
{
  if (catPlace == OdPropServices::enSepCatPlace_None)
    catPlace = OdPropServices::enSepCatPlace_Left;

  QSize dialogSize = getIApp()->getMainWindow()->size() * 2 / 3;

  QBoxLayout* pLayout = new QHBoxLayout(this);
  pLayout = pushLayout(pLayout);

  m_categories = new QTreeWidget(this);
  //m_categories->setFixedWidth(dialogSize.width() / 4);

  QVBoxLayout* pFrameLayout = new QVBoxLayout();
  QHBoxLayout* pgLayout = new QHBoxLayout();
  pFrameLayout->addLayout(pgLayout);

  m_pScroll = new QScrollArea(this);
  m_pPage = new QGroupBox(m_pScroll);
  m_pPage->sizePolicy().setHorizontalPolicy(QSizePolicy::Maximum);
  m_pScroll->setWidget(m_pPage);
  m_pScroll->setWidgetResizable(true);
  m_pScroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  pgLayout->addWidget(m_pScroll);

  QSplitter* pSplitter = new QSplitter(this);
  pSplitter->setOpaqueResize(false);
  pSplitter->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  QFrame* pFrame = new QFrame(this);
  pFrame->setFrameStyle(QFrame::StyledPanel); // (QFrame::NoFrame);
  pFrame->setLayout(pFrameLayout);
  int dim = 0;
  QList<int> lstSize;

  switch (catPlace)
  {
  case OdPropServices::enSepCatPlace_Left: case OdPropServices::enSepCatPlace_Right:
    pSplitter->setOrientation(Qt::Horizontal); 
    dim = dialogSize.width() / 3;
    break;
  case OdPropServices::enSepCatPlace_Top: case OdPropServices::enSepCatPlace_Bottom:
    pSplitter->setOrientation(Qt::Vertical); 
    dim = dialogSize.height() / 3;
    break;
  }
  switch (catPlace)
  {
  case OdPropServices::enSepCatPlace_Left: case OdPropServices::enSepCatPlace_Top:
    pSplitter->addWidget(m_categories);
    lstSize.push_back(dim);
    pSplitter->addWidget(pFrame);
    lstSize.push_back(dim * 2);
    break;
  case OdPropServices::enSepCatPlace_Right: case OdPropServices::enSepCatPlace_Bottom:
    pSplitter->addWidget(pFrame);
    lstSize.push_back(dim * 2);
    pSplitter->addWidget(m_categories);
    lstSize.push_back(dim);
    break;
  }
  pLayout->addWidget(pSplitter);
  pSplitter->setSizes(lstSize);

  QHBoxLayout *buttonLayout = new QHBoxLayout;
  pFrameLayout->addLayout(buttonLayout);

  QPushButton *ok = new QPushButton(tr("OK"));
  connect(ok, SIGNAL(clicked()), SLOT(accept()));
  ok->setDefault(true);

  QPushButton *cancel = new QPushButton(tr("Cancel"));
  connect(cancel, SIGNAL(clicked()), SLOT(reject()));

  m_warning = new QLabel();
  m_warning->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

  buttonLayout->addWidget(m_warning);
  buttonLayout->addStretch(1);
  buttonLayout->addWidget(ok);
  buttonLayout->addWidget(cancel);

  resize(dialogSize);

  QPoint centerPoint = getIApp()->getMainWindow()->mapToGlobal(getIApp()->getMainWindow()->rect().center());
  QPoint leftTop(centerPoint.x() - dialogSize.width() / 2, centerPoint.y() - dialogSize.height() / 2);
  move(leftTop);
}

QAbstractItemModel* OdqPropertyPageDialog::model()
{
  return m_model.data();
}

void OdqPropertyPageDialog::setModel(QAbstractItemModel* pModel)
{
  OdqPropertyModel* pPropModel = qobject_cast<OdqPropertyModel*>(pModel);
  ODA_ASSERT_ONCE(pPropModel);
  m_model = pPropModel;

  m_categories->setColumnCount(1);
  m_categories->header()->hide();

  fillCategoriesTree(QModelIndex(), NULL);

  connect(m_categories, SIGNAL(currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)),
                        SLOT(currentCategoryChanged(QTreeWidgetItem *)));

  QTreeWidgetItem *pItem = m_categories->topLevelItem(0);
  m_categories->setCurrentItem(pItem);

  connect(m_categories, SIGNAL(itemExpanded(QTreeWidgetItem *)), SLOT(emitExpanded(QTreeWidgetItem *)));
  connect(m_categories, SIGNAL(itemCollapsed(QTreeWidgetItem *)), SLOT(emitCollapsed(QTreeWidgetItem *)));

  m_model->connect(this, SIGNAL(expanded(const QModelIndex &)), SLOT(expanded(const QModelIndex &)));
  m_model->connect(this, SIGNAL(collapsed(const QModelIndex &)), SLOT(collapsed(const QModelIndex &)));

  connect(m_model, SIGNAL(expand(const QModelIndex &)), SLOT(expand(const QModelIndex &)));
  connect(m_model, SIGNAL(collapse(const QModelIndex &)), SLOT(collapse(const QModelIndex &)));

  connect(m_model, SIGNAL(layoutChanged()), SLOT(layoutChanged()));
  connect(m_model, SIGNAL(modelReset()), SLOT(modelReset()));
  connect(m_model, SIGNAL(valueRejected(QModelIndex, QString)), SLOT(valueRejected(QModelIndex, QString)));
}

OdqPropertyPageDialog::~OdqPropertyPageDialog()
{
}

int OdqPropertyPageDialog::exec()
{
  return OdqPropertyDialog::exec();
}

bool OdqPropertyPageDialog::setCurrentPage(const QString &qsFullName)
{
  QModelIndex index = m_model->modelIndex(qsFullName);
  if (!index.isValid())
  {
    ODA_ASSERT_ONCE(!"invalid focused category name");
    return false;
  }
  if (m_model->isProperty(index))
  {
    ODA_ASSERT_ONCE(!"TODO via parent");
    return false;
  }

  QTreeWidgetItem *pItem = lookupCategory(index);
  if (!pItem)
  {
    ODA_FAIL_ONCE();
    return false;
  }

  m_categories->setCurrentItem(pItem); //currentCategoryChanged(pItem);
  return true;
}

// return false if there is no file wrappers
//bool OdqPropertyPageDialog::setRestoredFilePropertyWrappersByCancel()
//{
//  // used to support cancel of property dialog
//  ODA_ASSERT_ONCE(!m_bFilePropertyWrappersWereSaved);
//  // maybe used via reject()
//  return m_bFilePropertyWrappersWereSaved;
//}

bool OdqPropertyPageDialog::wasCanceled() const
{
  return m_bWasCanceled;
}

void OdqPropertyPageDialog::addEditors(const QModelIndex &index, QBoxLayout *pLayout)
{
  IqAppProps::TPropType type = m_model->type(index);
  //if (type == IqAppProps::prpFilter)
  //{
  //  QModelIndex par = index.parent();
  //  while (m_model->type(par) == IqAppProps::prpFilter)
  //  {
  //    par = par.parent();
  //  }
  //  type = m_model->type(par);
  //}

  if (type == IqAppProps::prpGrid) // MSY 070108 //if (type)
  {
    QWidget *pView = createGrid(index);
    pLayout->addWidget(pView);

    // add filters
    return;
  }

  const int count = m_model->rowCount(index);

  for (int prop = 0; prop < count; prop++)
  {
    QModelIndex child = index.child(prop, 0);
    QModelIndex value = index.child(prop, 1);

    //TEnablerType enType = m_model->enablerType(child);
    //if (enType != enNone)
    //{
    //  ODA_ASSERT_ONCE(m_model->type(child) == IqAppProps::prpBool);
    //
    //  OdqPropControlEnabler *pEnabler = createEnabler(child, pLayout->parentWidget());
    //  QVBoxLayout *pGroupLayout = new QVBoxLayout();
    //  addEditors(child, pGroupLayout);
    //  pEnabler->setLayout(pGroupLayout);
    //  pLayout->addWidget(pEnabler);
    //}

    if (m_model->isProperty(child))
    {
      QHBoxLayout *propLayout = new QHBoxLayout;

      QWidget *label = createLabel(child);
      QWidget *editor = createEditor(value);

      if (label)
      {
        label->setParent(m_pPage);
        propLayout->addWidget(label, 1);
      }
      if (editor)
      {
        editor->setParent(m_pPage);
        propLayout->addWidget(editor, 1);
      }
      pLayout->addLayout(propLayout);
    }
  }
  pLayout->addStretch(1);
}

void OdqPropertyPageDialog::updatePage(const QModelIndex &qsPageIndex)
{
  m_pPage->hide();

  QObjectList children = m_pPage->children();
  foreach(QObject *pChild, children)
  {
    IqPropControl* pPropEditor = qobject_cast<IqPropControl *>(pChild);
    // temp
    QTreeView *pView = qobject_cast<QTreeView *>(pChild);
    if (pPropEditor || pView)
    {
      QWidget *pWidget = static_cast<QWidget *>(pChild);
      pWidget->hide();
    }
    else
    {
      delete pChild;
    }
  }

  if (m_pPage->layout())
  {
    delete m_pPage->layout();
  }

  QVBoxLayout *pLayout = new QVBoxLayout(m_pPage);
  addEditors(qsPageIndex, pLayout);

  m_pPage->show();

  m_pageIndex = qsPageIndex;
}

void OdqPropertyPageDialog::fillCategoriesTree(QModelIndex index, QTreeWidgetItem *item)
{
  if (index.isValid())
  {
    if (item)
    {
      item = new QTreeWidgetItem(item, QStringList(index.data().toString()));
    }
    else
    {
      item = new QTreeWidgetItem(QStringList(index.data().toString()));
      m_categories->addTopLevelItem(item);
    }

    QString qsCatName = m_model->path(index);
    item->setData(0, Qt::UserRole, qsCatName);
    if (index == m_pageIndex)
    {
      m_categories->setCurrentItem(item);
    }
  }


  bool bFiltersOnly = false;
  if (index.isValid())
  {
    IqAppProps::TPropType type = m_model->type(index);
    bFiltersOnly = (type == IqAppProps::prpGrid); // || type == IqAppProps::prpFilter);
  }

  const int count = m_model->rowCount(index);
  for (int childRow = 0; childRow < count; childRow++)
  {
    QModelIndex child = m_model->index(childRow, 0, index);
    if (!m_model->isProperty(child))
    {
      //TEnablerType enType = m_model->enablerType(child);
      //if (enType != enNone)
      //  continue; // enablers are not shown in tree

      if (bFiltersOnly) // && m_model->type(child) != IqAppProps::prpFilter)
        continue;

      fillCategoriesTree(child, item);
    }
  }
}

void OdqPropertyPageDialog::currentCategoryChanged(QTreeWidgetItem *current)
{
  if (!current)
  {
    return;
  }

  QString qsPageName = current->data(0, Qt::UserRole).toString();
  updatePage(m_model->modelIndex(qsPageName));
}

QWidget *OdqPropertyPageDialog::createLabel(QModelIndex index)
{
  IqAppProps::TPropType type = m_model->type(index);

  if (type == IqAppProps::prpBool)
  {
    return NULL;
  }
  if (type == IqAppProps::prpImage)
  {
    QString qsPath = index.data(Qt::DecorationRole).toString();
    QPixmap pixmap(qsPath);
    ODA_ASSERT_ONCE(!pixmap.isNull());
    pixmap.setMask(pixmap.createHeuristicMask());
    QLabel* image = new QLabel();
    image->setFixedSize(pixmap.size());
    image->setPixmap(pixmap);
    return image;
  }

  QLabel *pLabel = new QLabel(index.data().toString());

  QModelIndex value = index.sibling(index.row(), 1);
  Qt::ItemFlags flags = m_model->flags(value);
  bool bEnabled = (flags & Qt::ItemIsEditable) || (flags & Qt::ItemIsUserCheckable);

  pLabel->setEnabled(bEnabled);

  return pLabel;
}

QAbstractItemView *OdqPropertyPageDialog::createGrid(const QModelIndex &index)
{
  if (m_model->canFetchMore(index))
  {
    m_model->fetchMore(index);
  }

  if (QTreeView *pCreated = m_pPage->findChild<QTreeView *>(m_model->path(index)))
  {
    pCreated->show();
    pCreated->setEnabled(m_model->isEnabled(index));

    OdqGridProxyModel* pProxy = static_cast<OdqGridProxyModel *>(pCreated->model());
    pProxy->setRootIndex(index);

    for (int iColumn = 0; iColumn < pProxy->columnCount(); iColumn++)
    {
      pCreated->resizeColumnToContents(iColumn);
    }

    return pCreated;
  }

  QTreeView *pView = new QTreeView(m_pPage);

  OdqGridProxyModel* pProxyModel = new OdqGridProxyModel(pView, m_model);
  pProxyModel->setRootIndex(index);

  pView->setModel(pProxyModel);
  pView->setRootIsDecorated(true);
  pView->setSelectionMode(QAbstractItemView::NoSelection);
  pView->header()->setStretchLastSection(true);
  pView->setAllColumnsShowFocus(true);

  QObject::connect(pView->selectionModel(), SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)),
    pProxyModel, SLOT(setCurrentIndex(const QModelIndex &)));

  QObject::connect(pProxyModel, SIGNAL(currentSourceChanged(const QModelIndex &)),
    m_model, SLOT(currentChanged(const QModelIndex &)));

  for (int iColumn = 0; iColumn < pProxyModel->columnCount(); iColumn++)
  {
    pView->resizeColumnToContents(iColumn);
  }

  QString qsName = m_model->path(index);
  pView->setObjectName(qsName);

  QItemDelegate *pDelegate = new OdqGridProxyDelegate(pProxyModel);
  pView->setItemDelegate(pDelegate);

  return pView;
}

QWidget* OdqPropertyPageDialog::createEditor(QModelIndex index)
{
  if (QWidget* pCreated = m_pPage->findChild<QWidget *>(m_model->path(index)))
  {
    pCreated->show();

    Qt::ItemFlags flags = m_model->flags(index);
    bool bEnabled = (flags & Qt::ItemIsEditable) || (flags & Qt::ItemIsUserCheckable);
    pCreated->setEnabled(bEnabled);

    IqAppProps::TPropType tp = m_model->type(index);

    QVariant value = index.data();

    if (tp == IqAppProps::prpBool)
    {
      value = index.data(Qt::CheckStateRole);
    }
    if (tp == IqAppProps::prpColorref)
    {
      value = index.data(Qt::BackgroundColorRole);
    }
    if (tp == IqAppProps::prpOdColor)
    {
      value = index.data(Qt::UserRole);
    }

    IqPropControl* pPropEditor = qobject_cast<IqPropControl *>(pCreated);

    QObject::disconnect(pCreated, SIGNAL(propertyEdited(QWidget *)), this, SLOT(propertyEdited(QWidget *)));
     
    if (   tp == IqAppProps::prpCombo
        || tp == IqAppProps::prpLinetype || tp == IqAppProps::prpLineweight || tp == IqAppProps::prpTransparency
        || m_model->isVarious(index))
    {
      OdqPropControlComboBox *pPropCombo = qobject_cast<OdqPropControlComboBox *>(pCreated);
      if (pPropCombo)
      {
        QStringList qsItems, qsImages;
        m_model->getPropertyPossibleValues(index, qsItems, qsImages);
        pPropCombo->fillCombo(qsItems, qsImages);
      }
    }
    if (pPropEditor)
      pPropEditor->setValue(value);

    QObject::connect(pCreated, SIGNAL(propertyEdited(QWidget *)), this, SLOT(propertyEdited(QWidget *)));

    return pCreated;
  }

  IqAppProps::TPropType type = m_model->type(index);

  QWidget *pEditor = NULL;

  if (type == IqAppProps::prpBool)
  {
    QString text = index.sibling(index.row(), 0).data().toString();
    OdqPropControlCheckBox *check = new OdqPropControlCheckBox(text);
    QVariant val = index.data(Qt::CheckStateRole);
    check->setCheckState(static_cast<Qt::CheckState>(val.toInt()));
    pEditor = check;
  }

  if (type == IqAppProps::prpImage)
  {
    return NULL;
  }

  if (type == IqAppProps::prpInt)
  {
    OdqPropControlSpinBox *spinBox = new OdqPropControlSpinBox;
    spinBox->setValue(index.data().toInt());
    pEditor = spinBox;
  }

  if (type == IqAppProps::prpDouble)
  {
    OdqPropControlDoubleSpinBox *doubleSpinBox = new OdqPropControlDoubleSpinBox;
    doubleSpinBox->setValue(index.data().toDouble());
    pEditor = doubleSpinBox;
  }

  if (type == IqAppProps::prpCombo || m_model->isVarious(index))
  {
    OdqPropControlComboBox *combo = new OdqPropControlComboBox();
    combo->setModelIndex(index);
    QStringList qsItems, qsImages;
    m_model->getPropertyPossibleValues(index, qsItems, qsImages);
    combo->fillCombo(qsItems, qsImages);
    combo->setEditable(m_model->isVarious(index));
    combo->setValue(index.data()); // ->updateValue(index.data());
    pEditor = combo;
  }

  if (type == IqAppProps::prpFolder)
  {
    OdqPropControlFolder *folder = new OdqPropControlFolder(index.data().toString(), NULL);
    pEditor = folder;
  }

  if (type == IqAppProps::prpColorref)
  {
    QColor color = qvariant_cast<QColor>(index.data(Qt::BackgroundColorRole));
    OdqPropControlColorref* pColorPick = new OdqPropControlColorref(color, NULL);
    pEditor = pColorPick;
  }

  if (type == IqAppProps::prpOdColor)
  {
    QString qsColor = index.data(Qt::UserRole).toString();
    OdqPropControlOdColor *pColorPick = new OdqPropControlOdColor(qsColor, NULL);
    pEditor = pColorPick;
  }

  if (!pEditor)
  {
    OdqPropControlLineEdit *pLineEdit = new OdqPropControlLineEdit;
    pLineEdit->setText(index.data().toString());
    pEditor = pLineEdit;
  }

  IqPropControl* pPropEditor = qobject_cast<IqPropControl*>(pEditor);
  if (pPropEditor)
  {
    pPropEditor->setModelIndex(index);
    pPropEditor->createConnections();
    QObject::connect(pEditor, SIGNAL(propertyEdited(QWidget*)), this, SLOT(propertyEdited(QWidget*)));
  }

  Qt::ItemFlags flags = m_model->flags(index);
  bool bEnabled = (flags & Qt::ItemIsEditable) || (flags & Qt::ItemIsUserCheckable);
  pEditor->setEnabled(bEnabled);
  pEditor->setObjectName(m_model->path(index));

  return pEditor;
}

//OdqPropControlEnabler* OdqPropertyPageDialog::createEnabler(QModelIndex index, QWidget *pParent)
//{
//  OdqPropControlEnabler *pEnabler = NULL;
//
//  QModelIndex value = index.sibling(index.row(), 1);
//
//  QString qsTitle = index.data().toString();
//  QVariant val = value.data(Qt::CheckStateRole);
//  Qt::CheckState checkState = static_cast<Qt::CheckState>(val.toInt());
//  
//  TEnablerType enType = m_model->enablerType(index);
//  if (QWidget *pCreated = m_pPage->findChild<QWidget *>(m_model->path(index)))
//  {
//    pEnabler = qobject_cast<OdqPropControlEnabler *>(pCreated);
//    pEnabler->show();
//  }
//  else
//  {
//    pEnabler = new OdqPropControlEnabler(qsTitle, checkState, enType, pParent);
//    pEnabler->setModelIndex(value);
//    pEnabler->createConnections();
//    QObject::connect(pEnabler, SIGNAL(propertyEdited(QWidget *)), this, SLOT(propertyEdited(QWidget *)));
//  }
//
//  if (pEnabler->layout())
//  {
//    delete pEnabler->layout();
//    pEnabler->setLayout(NULL);
//  }
//
//  pEnabler->setChecked(checkState == Qt::Checked);
//  pEnabler->setObjectName(m_model->path(index));
//
//  return pEnabler;
//}

void OdqPropertyPageDialog::propertyEdited(QWidget *pEditor)
{
  IqPropControl* pPropEditor = qobject_cast<IqPropControl*>(pEditor);
  QModelIndex idx = pPropEditor->modelIndex();
  IqAppProps::TPropType type = m_model->type(idx);
  if (type == IqAppProps::prpBool)
    m_model->setData(idx, pPropEditor->value(), Qt::CheckStateRole);
  else
    m_model->setData(idx, pPropEditor->value());
}

// reset model
void OdqPropertyPageDialog::modelReset()
{
  m_categories->clear();
  fillCategoriesTree(QModelIndex(), NULL);
}

void OdqPropertyPageDialog::layoutChanged()
{
  m_categories->clear();
  fillCategoriesTree(QModelIndex(), NULL);
  if (!m_pageIndex.isValid())
  {
    m_categories->setCurrentItem(m_categories->topLevelItem(0));
  }

  qApp->processEvents();
}


void OdqPropertyPageDialog::valueRejected(QModelIndex index, QString errorMessage)
{
  m_warning->setText(errorMessage);
  m_warning->show();
}

void OdqPropertyPageDialog::valueAccepted()
{
  m_warning->hide();
}

void OdqPropertyPageDialog::expand(const QModelIndex &index)
{
  QTreeWidgetItem *pItem = lookupCategory(index);
  if (pItem)
  {
    m_categories->expandItem(pItem);
  }
}

void OdqPropertyPageDialog::collapse(const QModelIndex &index)
{
  QTreeWidgetItem *pItem = lookupCategory(index);
  if (pItem)
  {
    m_categories->collapseItem(pItem);
  }
}

QTreeWidgetItem *OdqPropertyPageDialog::lookupCategory(const QModelIndex &index)
{
  QString qsName = m_model->path(index);

  QTreeWidgetItemIterator itemIterator(m_categories);
  while (QTreeWidgetItem *pItem = *itemIterator)
  {
    if (pItem->data(0, Qt::UserRole) == qsName)
    {
      return pItem;
    }
    itemIterator++;
  }

  return NULL;
}

void OdqPropertyPageDialog::emitExpanded(QTreeWidgetItem *pItem)
{
  QString qsName = pItem->data(0, Qt::UserRole).toString();
  QModelIndex index = m_model->modelIndex(qsName);
  emit expanded(index);
}

void OdqPropertyPageDialog::emitCollapsed(QTreeWidgetItem *pItem)
{
  QString qsName = pItem->data(0, Qt::UserRole).toString();
  QModelIndex index = m_model->modelIndex(qsName);
  emit collapsed(index);
}

void OdqPropertyPageDialog::reject()
{
  //if (m_bFilePropertyWrappersWereSaved)
  //{
  //}

  m_bWasCanceled = true;

  OdqPropertyDialog::reject();
  close();
}

void OdqPropertyPageDialog::accept()
{
  OdqPropertyDialog::accept();
  close();
}

//void OdqPropertyPageDialog::addMenuXml(const QString &qsMenuXml)
//{
//  if (m_iMenu.isNull())
//    m_iMenu = getIApp()->createNewObject("oda.main.menu");
//
//  if (!m_iMenu.isNull())
//  {
//    m_iMenu->addMenuXml(qsMenuXml);
//
//    QMenu *pMenu = new QMenu();
//    m_iMenu->fillContextMenu(pMenu);
//
//    QMenuBar *pMenubar = menuBar();
//    pMenubar->addMenu(pMenu);
//  }
//}

//void OdqPropertyPageDialog::addToolbarXml(const QString &qsToolbarXml)
//{
//  if (m_iToolbars.isNull())
//    m_iToolbars = getIApp()->createNewObject("oda.main.toolbars");
//
//  if (!m_iToolbars.isNull())
//  {
//    QWidget temp;
//    temp.setLayout(new QVBoxLayout(&temp));
//
//    m_iToolbars->addToolbarsXml(qsToolbarXml);
//    m_iToolbars->fillContextToolbars(&temp);
//
//    QList<QToolBar *> toolbars = temp.findChildren<QToolBar *>();
//    for (int iter = 0; iter < toolbars.size(); iter++)
//    {
//      QToolBar *pToolBar = toolbars[iter];
//      addToolBar(pToolBar);
//    }
//  }
//}
