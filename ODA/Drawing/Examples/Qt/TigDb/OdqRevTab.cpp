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
// OdqRevTab.cpp
//
#include <QtCore>
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtGui>
#else
#include <QtWidgets>
#endif
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QComboBox> 
#include <QTreeWidget>
#include <QPushButton>
#include <QSlider>
#include <QCheckBox>
#include <QMenu>
#include <QTableWidget>
#include <QBitmap>
#include <QIcon>

#include "OdqRevSystem.h" // #include "OdqRevTab.h"

#include "Ed/EdCommandContext.h"
#include "DbHostAppServices.h"

//////////////////////////////////////////////////////////////////////////

OdqRevTab::OdqRevTab(QWidget* pParent, OdRxObject* pRxDb)
  : QWidget(pParent)
  , m_pRxDb(pRxDb) // m_pCtx(pCtx)
  , m_curRevIndex(-1) // none
  , m_curSelIndex(-1)
  , m_curPropIndex(-1)
  , m_isCompareMode(false)
  , m_curDiffIndex(-1)
  , m_iDiffView(NULL)
{
  init();
}

OdqRevTab::~OdqRevTab()
{
  if (!m_pBranchTree.isNull() && m_pBranchTree->columnCount() > 1)
    m_pBranchTree->saveSettings();
  uninit();
}

//OdEdCommandContext* OdqRevTab::commandContext()
//{
//  return getIConsole()->getCommandContext(m_pRxDb);
//}

void OdqRevTab::init()
{
  if (isInitialized())
    return;

  ////if (qApp->activeWindow())
  ////  setMinimumWidth(qApp->activeWindow()->width() / 15);

  QVBoxLayout* pLayout = new QVBoxLayout();
  ODA_ASSERT_ONCE(!layout());
  setLayout(pLayout);
  
  if (!controller() || !controller()->isResponsible(m_pRxDb))
  {
    QString qsUnsupport;
    if (m_pRxDb)
      qsUnsupport = QObject::tr("This functionality is unsupported for %1")
                      .arg(getIConsole()->getQTitle(m_pRxDb));
    m_pUnsuppotLabel = new QLabel(qsUnsupport);
    if (m_pRxDb)
      pLayout->addWidget(m_pUnsuppotLabel);
    return;
  }

  QGridLayout* pHLayout = new QGridLayout; // = new QHBoxLayout;
  pLayout->addLayout(pHLayout);

  QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  sizePolicy.setHorizontalStretch(0);
  sizePolicy.setVerticalStretch(0);

  m_pBranchLabel = new QLabel(QObject::tr("Branches:"));
  pHLayout->addWidget(m_pBranchLabel, 0, 0);
  m_pBranchCombo = new QComboBox;
  pHLayout->addWidget(m_pBranchCombo, 0, 1);
  sizePolicy.setHeightForWidth(m_pBranchCombo->sizePolicy().hasHeightForWidth());
  m_pBranchCombo->setSizePolicy(sizePolicy);
  //m_pBranchCombo->setInsertPolicy(QComboBox::InsertAlphabetically);
  m_pBranchLabel->setBuddy(m_pBranchCombo);

  if (!connect(m_pBranchCombo, SIGNAL(currentIndexChanged(const QString&)),
               this, SLOT(currentBranchIndexChanged(const QString&))))
  {
    ODA_FAIL_ONCE();
  }

  m_pBranchTree = new OdqBranchTree(); // = new QTreeWidget();
  m_pBranchTree->setObjectName("revisions");
  m_pBranchTree->setColumnCount(1);
  m_pBranchTree->header()->hide();
  pLayout->addWidget(m_pBranchTree);
  m_pBranchTree->restoreSettings();

  if (   !connect(m_pBranchTree, SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)),
                  this, SLOT(currentTreeItemChanged(QTreeWidgetItem*)))
      || !connect(m_pBranchTree, SIGNAL(itemPressed(QTreeWidgetItem*, int)),
                  this, SLOT(currentTreeitemPressed(QTreeWidgetItem*, int)))
      || !connect(m_pBranchTree, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)),
                  this, SLOT(doubleClickedTreeItem(QTreeWidgetItem *, int))))
  {
    ODA_FAIL_ONCE();
  }

  //-----

  m_pChangesGroup = new QGroupBox(tr("Properties"));
  pLayout->addWidget(m_pChangesGroup);
  m_pChangesGroup->setVisible(false);

  QGridLayout* pPropLayout = new QGridLayout;
  //pLayout->addLayout(pPropLayout);
  m_pChangesGroup->setLayout(pPropLayout);

  m_pDiffCheckBox = new QCheckBox(tr("Diff Only"));
  pPropLayout->addWidget(m_pDiffCheckBox, 0, 0);
  m_pDiffCheckBox->setChecked(true);
  m_pDiffCheckBox->setEnabled(false); // setVisible(false);

  m_pDiffSlider = new QSlider(Qt::Horizontal);
  m_pDiffSlider->setMinimum(0);
  m_pDiffSlider->setMaximum(100);
  pPropLayout->addWidget(m_pDiffSlider, 0, 1, 1, 4);
  m_pDiffSlider->setValue(50);
  m_pDiffSlider->setEnabled(false); // setVisible(false);

  m_pChangesTitle = new QLabel(tr("Revision Description"));
  pPropLayout->addWidget(m_pChangesTitle, 1, 0, 1, 5, Qt::AlignHCenter);
  m_pChangesTitle->setVisible(false);

  m_pChangesTable = new QTableWidget();
  m_pChangesTable->verticalHeader()->setVisible(false);
  QHeaderView* pHorHeader = m_pChangesTable->horizontalHeader();
  pHorHeader->setVisible(false);

  m_pChangesTable->setColumnCount(1);
  m_pChangesTable->setRowCount(1);

  pPropLayout->addWidget(m_pChangesTable, 2, 0, 1, 5); // , Qt::AlignHCenter);

  m_pBackButton = new QPushButton(tr("Close"));
  pPropLayout->addWidget(m_pBackButton, 3, 3);

  m_pDiffButton = new QPushButton(tr("View diff"));
  pPropLayout->addWidget(m_pDiffButton, 3, 4);
  //m_pDiffButton->setVisible(false);

  if (   !connect(m_pBackButton, SIGNAL(clicked()),
                  this, SLOT(clickedBackButton()))
      || !connect(m_pDiffButton, SIGNAL(clicked()),
                  this, SLOT(clickedDiffButton()))
      || !connect(m_pDiffCheckBox, SIGNAL(toggled(bool)),
                  this, SLOT(diffOnlyChanged(bool)))
      || !connect(m_pDiffSlider, SIGNAL(valueChanged(int)),
                  this, SLOT(diffSlidervalueChanged(int))))
  {
    ODA_FAIL_ONCE();
  }

  m_curPropIndex = -1;
  m_curDiffIndex = -1;
  m_isCompareMode = false;

  m_curRevIndex = 0;
  m_curSelIndex = 0;
  updateBranches(false);
}

void OdqRevTab::uninit()
{
  //if (!isInitialized())
  //  return;
}

OdqRevisionController* OdqRevTab::controller()
{
  return OdqRevSystem::getRevSystem().controller();
}

bool OdqRevTab::isInitialized() const
{
  if (m_pUnsuppotLabel.isNull() && m_pBranchCombo.isNull())
    return false;
  return true;
}

OdRxObject* OdqRevTab::getRxDatabase()
{
  return m_pRxDb;
}

static QPixmap getPixmap(QString qsIconPath)
{
  if (!QFileInfo(qsIconPath).exists())
    return QPixmap();
  QPixmap pixmap(qsIconPath);
  if (pixmap.isNull())
    return QPixmap();
  pixmap.setMask(pixmap.createHeuristicMask());
  return pixmap;
}

void OdqRevTab::updateBranches(bool bKeepCurrent) // = true
{
  if (m_pBranchCombo.isNull())
    return;
  m_pBranchCombo->blockSignals(true);

  QString qsActiveInComboBranchName;
  int indexBranchComboCurrent = 0;
  int num = m_pBranchCombo->count();
  if (num > 1)
    qsActiveInComboBranchName = m_pBranchCombo->currentText();
  else if (num == 1)
    indexBranchComboCurrent = 1;
  m_pBranchCombo->clear();

  OdString sCurBranch = controller()->currentBranchName(m_pRxDb);
  ODA_ASSERT_ONCE(!sCurBranch.isEmpty());
  m_pBranchCombo->addItem("All");
  m_pBranchCombo->addItem(tr("Active - ") + getIApp()->toQString(sCurBranch));

  OdTfDigest shaCurRev, shaCurSel;
  OdInt32 szRevs = (OdInt32) m_lstRevisions.size();
  if (!bKeepCurrent)
  {
    szRevs = 0;
    m_curRevIndex = 0;
    m_curSelIndex = 0;
  }
  if (m_curRevIndex >= 0 && szRevs)
    shaCurRev = m_lstRevisions[m_curRevIndex].sha;
  if (m_curSelIndex >= 0 && szRevs)
    shaCurSel = m_lstRevisions[m_curSelIndex].sha;

  controller()->loadBranches(m_pRxDb, m_lstBranchNames, m_lstRevisions);

  if (szRevs && szRevs != m_lstBranchNames.size() && (m_curRevIndex >= 0 || m_curSelIndex >= 0))
  {
    szRevs = m_lstRevisions.size();
    // find new current indexes via sha
    for (OdInt32 indexChild = 0; indexChild < szRevs; indexChild++)
    {
      OdCommitDescription& info = m_lstRevisions[indexChild];
      if (m_curRevIndex >= 0 && info.sha == shaCurRev)
        m_curRevIndex = indexChild;
      if (m_curSelIndex >= 0 && info.sha == shaCurSel)
        m_curSelIndex = indexChild;
    }
  }

  for (int idx = 0, idxCombo = 2, sz = m_lstBranchNames.size(); idx < sz; idx++)
  {
    if (sCurBranch == m_lstBranchNames[idx])
      continue;
    QString qsBranchName = getIApp()->toQString(m_lstBranchNames[idx]);
    ODA_ASSERT_ONCE(!qsBranchName.isEmpty());

    m_pBranchCombo->addItem(qsBranchName);
    if (qsBranchName == qsActiveInComboBranchName)
      indexBranchComboCurrent = idxCombo;
    idxCombo++;
  }
  m_pBranchCombo->setCurrentIndex(indexBranchComboCurrent);

  m_pBranchCombo->blockSignals(false);

  //m_curRevIndex = 0;
  m_curSelIndex = -1;
  updateRevisions();
}

void OdqRevTab::updateRevisions()
{
  ODA_ASSERT_ONCE(!m_pBranchCombo.isNull());

  OdString sBranchOnly; // all if an empty
  int indexBranchComboCurrent = m_pBranchCombo->currentIndex();
  if (indexBranchComboCurrent > 1)
    sBranchOnly = getIApp()->toOdString(m_pBranchCombo->currentText());
  else if (indexBranchComboCurrent == 1)
    sBranchOnly = controller()->currentBranchName(m_pRxDb);

  unsigned int indexChild = 0,
               sz = m_lstRevisions.size();
  ODA_ASSERT_ONCE(sz);

  int flagsAdvInfo = getIAppProps()->toInt(L"Vars/tigdeb", 1);
  
  std::map<OdString, OdUInt32> mapIndexByParentKey;
  if (sz && GETBIT(flagsAdvInfo, 2))
  {
    for (; indexChild < sz; indexChild++)
    {
      OdCommitDescription& info = m_lstRevisions[indexChild];
      ODA_ASSERT_ONCE(mapIndexByParentKey.find(info.id) == mapIndexByParentKey.end());
      mapIndexByParentKey[info.id] = indexChild;
    }
  }

  m_pBranchTree->blockSignals(true);
  m_pBranchTree->clear();

  if (sz)
  {
    m_pBranchTree->setColumnCount(4);
    //m_pBranchTree->header()->hide();
    QStringList lstLabels;
    lstLabels << ""; //"graph";
    lstLabels << "description" << "author" << "date"; 
    if (m_pBranchTree->columnCount() > 4)
      lstLabels << "";
    m_pBranchTree->header()->show();
    m_pBranchTree->setHeaderLabels(lstLabels);
    //m_pBranchTree->setSelectionModel(QItemSelectionModel * selectionModel)

    for (indexChild = 0; indexChild < sz; indexChild++)
    {
      QStringList lst;
      lst << "";

      OdCommitDescription& info = m_lstRevisions[indexChild];
      ODA_ASSERT_ONCE(!info.branches.isEmpty());
      if (!sBranchOnly.isEmpty() && !info.branches.contains(sBranchOnly))
        continue;

      QString qsDescr = getIApp()->toQString(info.msg);

      QString qsTooltip = qsDescr;
      ODA_ASSERT_ONCE(info.branches.size());
      if (flagsAdvInfo)
      {
        qsTooltip.clear();
        if (GETBIT(flagsAdvInfo, 1))
        {
          for (OdUInt32 idxBr = 0, szBr = info.branches.size(); idxBr < szBr; idxBr++)
          {
            if (!qsTooltip.isEmpty())
              qsTooltip += ", ";
            qsTooltip += getIApp()->toQString(info.branches[idxBr]);
          }
        }
        if (GETBIT(flagsAdvInfo, 2))
        {
          qsTooltip += QString(" (%1: ").arg(indexChild);
          for (OdUInt32 idxPrnt = 0, szPrnt = info.parents.size(); idxPrnt < szPrnt; idxPrnt++)
          {
            const OdString& keyParent = info.parents[idxPrnt];
            ODA_ASSERT_ONCE(mapIndexByParentKey.find(keyParent) != mapIndexByParentKey.end());
            unsigned int idx = mapIndexByParentKey[keyParent];
            if (idxPrnt)
              qsTooltip += ", ";
            qsTooltip += QString::number(idx);
          }
          qsTooltip += ")";
        }
        qsTooltip += ": " + qsDescr;
        if (GETBIT(flagsAdvInfo, 4))
          qsDescr = qsTooltip;
      }

      lst << qsDescr;
      lst << getIApp()->toQString(info.author);
      //OdString sTime; info.c.date.ctime(sTime);
      lst << getIApp()->toQString(info.date); // sTime);

      QTreeWidgetItem* pChildItem = new QTreeWidgetItem(lst);
      pChildItem->setData(0, Qt::UserRole, indexChild);
      //pChildItem->setData(0, Qt::DecorationRole, ":/images/graph/node.png");
      if (!qsTooltip.isEmpty())
        for (OdInt32 idx = 1, sz = m_pBranchTree->columnCount(); idx < sz; idx++)
          pChildItem->setToolTip(idx, qsTooltip);

      m_pBranchTree->addTopLevelItem(pChildItem);

      //QPixmap pixmap = getPixmap(":/images/graph/node.png");
      //if (!pixmap.isNull())
      //{
      //  //pChildItem->setTextAlignment(0, Qt::AlignLeft); // | Qt::AlignVCenter);
      //  //pChildItem->setIcon(0, QIcon(pixmap));

      //  //https://stackoverflow.com/questions/25915254/qt-centering-icon-in-qtreewidgetitem
      //  //https://stackoverflow.com/questions/8274123/position-of-icon-in-qtreewidgetitem/8274944#8274944
      //  //setItemWidget and set as widget an instance of QLabel containing a well centered pixmap.
      //  QLabel* pIconLabel = new QLabel();
      //  pIconLabel->setPixmap(pixmap);
      //  pIconLabel->setAlignment(Qt::AlignLeft);
      //  //pIconLabel->setWordWrap(bool on);
      //  pIconLabel->setIndent(0);
      //  pIconLabel->setMargin(0);
      //  m_pBranchTree->setItemWidget(pChildItem, 0, pIconLabel);

      //  //create a new class child of QTreeWidgetItem and tweak the painting actions, 
      //  //you will maybe be able to draw the icon where you want.
      //}

      //if (!indexChild)
      //  m_pBranchTree->setFirstItemColumnSpanned(pChildItem, true);
    }
    if (m_curRevIndex >= 0)
    {
      QTreeWidgetItem* pCheckedoutItem = branchItemByIndex(m_curRevIndex);
      if (pCheckedoutItem)
      {
        if (m_pDefBrush.isNull())
        {
          m_pDefBrush = new QBrush();
          *(m_pDefBrush.get()) = pCheckedoutItem->background(1);
        }

        QBrush brush(QColor(150, 168, 250)); //QWidget::setAutoFillBackground
        for (OdInt32 idx = 0, sz = m_pBranchTree->columnCount(); idx < sz; idx++)
          pCheckedoutItem->setBackground(idx, brush);
      }
      //else
      //  m_curRevIndex = -1;
    }
    if (m_curSelIndex >= 0)
    {
      QTreeWidgetItem* pSelectedItem = branchItemByIndex(m_curSelIndex);
      if (pSelectedItem)
      {
        m_pBranchTree->setCurrentItem(pSelectedItem);
        pSelectedItem->setSelected(true);
      }
      else
        m_curSelIndex = -1;
    }
  }

  m_pBranchTree->updateGraphData(&m_lstBranchNames, &m_lstRevisions, sBranchOnly);

  //for (OdInt32 idx = 0, sz = m_pBranchTree->columnCount(); idx < sz; idx++)
  //  m_pBranchTree->setColumnWidth(0, 50);

  m_pBranchTree->blockSignals(false);

  QHeaderView* pHeader = m_pBranchTree->header();

  //pHeader->resizeSection(0, 50);
  //pHeader->resizeSection(1, 100);
  //pHeader->resizeSection(2, 50);
  //pHeader->resizeSection(3, 50);
  //pHeader->setStretchLastSection(false);

#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
  pHeader->setSectionResizeMode(QHeaderView::ResizeToContents);
#else
  pHeader->setResizeMode(QHeaderView::ResizeToContents);
#endif
  //pHeader->resizeSections(QHeaderView::ResizeToContents);

  ////QHeaderView::ResizeMode svMode = pHeader->sectionResizeMode(0);
  //pHeader->setSectionResizeMode(QHeaderView::ResizeToContents);
  //m_pBranchTree->show();
  //pHeader->setSectionResizeMode(QHeaderView::Stretch);
  ////pHeader->setSectionResizeMode(svMode); // QHeaderView::Stretch);
}

bool OdqRevTab::getIndexesToDiff(OdInt32& idxFirst, OdInt32& idxSecond)
{
  if (m_curPropIndex >= 0)
  {
    idxFirst = m_curPropIndex;
    OdInt32 sz = (OdInt32)m_lstRevisions.size();
    if ((idxSecond = idxFirst + 1) >= sz)
      idxSecond = idxFirst;
  }
  else
  {
    ODA_ASSERT_ONCE(m_isCompareMode);
    idxFirst = m_curRevIndex;
    idxSecond = m_curSelIndex;
  }
  if (idxFirst < 0 || idxSecond < 0 || idxFirst == idxSecond)
    return false;
  //swap revisions based on date
  if (m_lstRevisions[idxFirst].commit.annotation.date > m_lstRevisions[idxSecond].commit.annotation.date)
  {
    OdInt32 tmp = idxFirst;
    idxFirst = idxSecond;
    idxSecond = tmp;
  }

  return true;
}

QTreeWidgetItem* OdqRevTab::branchItemByIndex(OdInt32 index)
{
  if (index < 0)
    return NULL;
  ODA_ASSERT_ONCE(!m_pBranchTree.isNull());
  OdInt32 szTree = (OdInt32)m_pBranchTree->topLevelItemCount(),
          szRevs = (OdInt32)m_lstRevisions.size();
  ODA_ASSERT_ONCE(index >= 0 && index < szRevs && szTree <= szRevs);
  if (szTree == szRevs)
    return m_pBranchTree->topLevelItem(index);
  for (OdInt32 idx = 0; idx < szTree; idx++)
  {
    QTreeWidgetItem* pTreeItem = m_pBranchTree->topLevelItem(idx);
    OdInt32 indexChild = (OdInt32) pTreeItem->data(0, Qt::UserRole).toInt();
    if (indexChild == index)
      return pTreeItem;
  }
  return NULL;
}

OdTfDigest* OdqRevTab::digestByIndex(OdInt32 index)
{
  OdInt32 sz = (OdInt32)m_lstRevisions.size();
  ODA_ASSERT_ONCE(index >= 0 && index < sz);
  if (index < 0 || index >= sz)
    return NULL;

  OdCommitDescription& info = m_lstRevisions[index];
  return &info.sha;
}

void OdqRevTab::updateChanges(bool bSliderOnly) // = false
{
  OdInt32 idxFirst = 0, idxSecond = 0;
  if (!getIndexesToDiff(idxFirst, idxSecond))
    return;

  if (!m_pChangesGroup->isVisible())
  {
    m_pChangesGroup->setVisible(true);
    bSliderOnly = false;
    //m_curPropIndex = m_curSelIndex;
  }

  bool isDiffMode = (m_curDiffIndex >= 0);
  m_pDiffCheckBox->setEnabled(isDiffMode); //->setVisible(isDiffMode);
  m_pDiffSlider->setEnabled(isDiffMode); //->setVisible(isDiffMode);
  m_pDiffButton->setEnabled(!isDiffMode); // setVisible(!isDiffMode);
  
  m_pBranchLabel->setVisible(false);
  m_pBranchCombo->setVisible(false);
  m_pBranchTree->setVisible(false);

  if (bSliderOnly)
    return;

  //QTreeWidgetItem* pSelectedItem = branchItemByIndex(m_curSelIndex);
  //QString	qsTitle = pSelectedItem->text(1);
  QString	qsTitle = tr(m_isCompareMode ? "Compare revisions" : "Revision properties");
  //m_pChangesTitle->setText(qsTitle);
  m_pChangesGroup->setTitle(qsTitle);

  m_pChangesTable->clearContents();
  QStringList lstLabels;
  lstLabels << tr("Entity") << tr("Operation");
  m_pChangesTable->verticalHeader()->setVisible(false);
  QHeaderView* pHorHeader = m_pChangesTable->horizontalHeader();
  pHorHeader->setDefaultAlignment(Qt::AlignLeft);

  pHorHeader->setVisible(true);
  //#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
  //  pHorHeader->setSectionResizeMode(QHeaderView::Stretch); // QHeaderView::ResizeToContents); // Fixed);
  //#else
  //  pHorHeader->setResizeMode(QHeaderView::Fixed); // // , Stretch, ResizeToContents, Custom Interactive
  //#endif

  m_pRxFirstDb = NULL;
  m_pRxSecondDb = NULL;

  OdStringArray entities,
                changeTypes;
  //OdStringArray handles;

  if (idxFirst != idxSecond)
    controller()->getDifferences(m_pRxDb,
                                 m_lstRevisions[idxFirst], m_lstRevisions[idxSecond],
                                 entities, changeTypes,
                                 NULL, // &handles // pHandles
                                 &m_pRxFirstDb, &m_pRxSecondDb);

  int sz = entities.size();
  ODA_ASSERT_ONCE(sz == changeTypes.size());

  m_pChangesTable->setColumnCount(lstLabels.length());
  m_pChangesTable->setRowCount(sz);
  m_pChangesTable->setHorizontalHeaderLabels(lstLabels);
  //m_pChangesTable->horizontalHeader()->setStretchLastSection(true);
  for (int idxRow = 0; idxRow < sz; idxRow++)
  {
    QTableWidgetItem* pEntItem = new QTableWidgetItem(getIApp()->toQString(entities[idxRow]));
    pEntItem->setFlags(pEntItem->flags() & ~(Qt::ItemIsEditable | Qt::ItemIsSelectable));
    m_pChangesTable->setItem(idxRow, 0, pEntItem);

    QTableWidgetItem* pTypeItem = new QTableWidgetItem(getIApp()->toQString(changeTypes[idxRow]));
    pTypeItem->setFlags(pTypeItem->flags() & ~(Qt::ItemIsEditable | Qt::ItemIsSelectable));
    m_pChangesTable->setItem(idxRow, 1, pTypeItem);
  }
  m_pChangesTable->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  QSize qtSize = m_pChangesTable->size();
  pHorHeader->setDefaultSectionSize(qtSize.width() / 2 - 1);
}

void OdqRevTab::setGearVisible(OdInt32 index, bool bVal)
{
  if (index < 0)
    return;
  OdInt32 indexGear = m_pBranchTree->columnCount() - 1;

  QTreeWidgetItem* pSelectedItem = branchItemByIndex(index); // pBranchTree->topLevelItem(index);
  if (!pSelectedItem)
    return;
  bool bAdded = !pSelectedItem->icon(indexGear).isNull();
  if (bVal && !bAdded)
    pSelectedItem->setIcon(indexGear, QIcon(getPixmap(":/images/rev/gear.png")));
  else if (!bVal && bAdded)
    pSelectedItem->setIcon(indexGear, QIcon());
}

// slot
void OdqRevTab::currentBranchIndexChanged(const QString& qsText)
{
  int indexBranchComboCurrent = m_pBranchCombo->currentIndex();
  ODA_ASSERT_VAR(if (!indexBranchComboCurrent))
    ODA_ASSERT(true); // brk

  m_curSelIndex = -1;
  updateRevisions();
  ODA_ASSERT_ONCE(indexBranchComboCurrent >= 0);
  if (!indexBranchComboCurrent) 
    return;

  QString qsBranchToSwitch = m_pBranchCombo->currentText();
  OdString sBranchToSwitch = getIApp()->toOdString(qsBranchToSwitch),
           sCurBranch = controller()->currentBranchName(m_pRxDb);
  ODA_ASSERT_ONCE(!sCurBranch.isEmpty());
  if (indexBranchComboCurrent == 1)
  {
    QString qsPref = tr("Active - ");
    if (!qsBranchToSwitch.indexOf(qsPref))
    {
      qsBranchToSwitch = qsBranchToSwitch.mid(qsPref.length());
      sBranchToSwitch = getIApp()->toOdString(qsBranchToSwitch);
    }

    ODA_ASSERT_VAR(if (sBranchToSwitch != sCurBranch))
      ODA_ASSERT(true); // brk
  }
  if (sBranchToSwitch == sCurBranch) // possible with switching name -> All -> same_name
  {
    if (indexBranchComboCurrent > 1)
      updateBranches(true);
    return;
  }

  if (controller()->switchToBranch(m_pRxDb, sBranchToSwitch))
  {
    updateBranches(true);
    m_pBranchCombo->setCurrentIndex(1);

    QTreeWidgetItem* pTreeItem = m_pBranchTree->topLevelItem(0);
    //currentTreeItemChanged(pTreeItem);
    OdUInt32 indexChild = (OdUInt32) pTreeItem->data(0, Qt::UserRole).toInt();
    m_curSelIndex = indexChild;
    
    clickedSwitchRevButton();
    
    getIConsole()->postponeCommand("regenall", false, m_pRxDb);
  }
}

// slot
void OdqRevTab::currentTreeItemChanged(QTreeWidgetItem* pTreeItem)
{
  if (!pTreeItem)
    return;

  OdUInt32 indexChild = (OdUInt32) pTreeItem->data(0, Qt::UserRole).toInt();
  if (indexChild != m_curSelIndex)
  {
    setGearVisible(m_curSelIndex, false);
    m_curSelIndex = indexChild;
    setGearVisible(m_curSelIndex, true);
  }
}

// slot
void OdqRevTab::currentTreeitemPressed(QTreeWidgetItem* pTreeItem, int column)
{
  int numColums = m_pBranchTree->columnCount();
  if (!column || column != (numColums - 1))
    return;

  if (!m_pMenu.isNull())
    m_pMenu->close();

  OdUInt32 indexChild = (OdUInt32)pTreeItem->data(0, Qt::UserRole).toInt();
  if (indexChild != m_curSelIndex)
    return;

  QPoint pos = m_pBranchTree->mousePos();
  if (pos == QPoint())
    return;
  pos = m_pBranchTree->viewport()->mapToGlobal(pos);

  QMenu* pMenu = new QMenu(this);
  QAction* pAction = NULL;
  if (m_curSelIndex != m_curRevIndex)
  {
    pAction = new QAction(tr("Open"), this);
    pAction->setObjectName("open");
    pMenu->addAction(pAction);
  }
  if (m_curSelIndex >= 0 && m_curSelIndex < (OdInt32(m_lstRevisions.size()) - 1))
  {
    pAction = new QAction(tr("Properties"), this);
    pAction->setObjectName("property");
    pMenu->addAction(pAction);
  }
  if (m_curSelIndex != m_curRevIndex)
  {
    pAction = new QAction(tr("Compare"), this);
    pAction->setObjectName("compare");
    pMenu->addAction(pAction);
  }
  if (!connect(pMenu, SIGNAL(triggered(QAction*)), this, SLOT(menuTriggered(QAction*))))
  {
    ODA_FAIL_ONCE();
  }

  pMenu->popup(pos);
}

// slot
void OdqRevTab::menuTriggered(QAction* pAction)
{
  QString qsOperation = pAction->objectName();
  if (qsOperation == "open")
    clickedSwitchRevButton();
  else if (qsOperation == "property")
    clickedPropertyButton();
  else if (qsOperation == "compare")
  {
    m_isCompareMode = true;
    m_curPropIndex = -1;
    clickedDiffButton();
  }
}

// slot
void OdqRevTab::doubleClickedTreeItem(QTreeWidgetItem* pTreeItem, int) //column
{
  currentTreeItemChanged(pTreeItem);
  if (m_curRevIndex == m_curSelIndex || m_curSelIndex < 0 || !controller())
    return;
  clickedSwitchRevButton();
}

// slot
void OdqRevTab::clickedBackButton()
{
  if (m_curDiffIndex >= 0)
  {
    if (m_iDiffView)
    {
      m_iDiffView->emitClose();
      return;
    }
    ODA_FAIL_ONCE();
    m_curPropIndex = -1;
  }

  if (m_pChangesGroup->isVisible())
  {
    m_pChangesGroup->setVisible(false);
    m_pBranchLabel->setVisible(true);
    m_pBranchCombo->setVisible(true);
    m_pBranchTree->setVisible(true);
  }
  m_curPropIndex = -1;
  m_isCompareMode = false;
}

// slot
void OdqRevTab::diffOnlyChanged(bool checked)
{
  if( !m_iDiffView ) return;
  if( !m_rdHelper.isValid() ) return;

  m_rdHelper.reloadMainView( !checked );
  m_iDiffView->updateGL();
}

void OdqRevTab::diffSlidervalueChanged(int value)
{
  //value = 100 - value;
  if( !m_iDiffView ) return;
  if( !m_rdHelper.isValid() ) return;
  if( value == 0 )
  {
    m_rdHelper.setVSOpacity( 1.0, true );
    m_rdHelper.setViewsVisible( true, false );
  }
  else if( value < 50 )
  {
    m_rdHelper.setVSOpacity( 1.0, true );
    double alpha = double( 2 * value ) / 100.0;
    m_rdHelper.setVSOpacity( alpha, false );
    m_rdHelper.setViewsVisible( true, true );
  }
  else if( value == 50 )
  {
    m_rdHelper.setVSOpacity( 1.0, true );
    m_rdHelper.setVSOpacity( 1.0, false );
    m_rdHelper.setViewsVisible( true, true );
  }
  else if( value < 100 )
  {
    m_rdHelper.setVSOpacity( 1.0, false );
    double alpha = double( (100 - value) * 2 ) / 100.0;
    m_rdHelper.setVSOpacity( alpha, true );
    m_rdHelper.setViewsVisible( true, true );
  }
  else
  {
    m_rdHelper.setVSOpacity( 1.0, false );
    m_rdHelper.setViewsVisible( false, true );
  }
  m_iDiffView->updateGL();
}

// slot
void OdqRevTab::clickedSwitchRevButton()
{
  if (m_curRevIndex == m_curSelIndex || m_curSelIndex < 0 || !controller())
    return;
  OdString sBranchPrev = controller()->currentBranchName(m_pRxDb);

  if (!controller()->loadDatabaseAtRevision(m_pRxDb, m_lstRevisions[m_curSelIndex]))
  {
    //ODA_FAIL_ONCE();
    return;
  }

  QTreeWidgetItem* pPrevCheckedoutItem = branchItemByIndex(m_curRevIndex);
  m_curRevIndex = m_curSelIndex;

  QTreeWidgetItem* pCheckedoutItem = branchItemByIndex(m_curRevIndex);
  QBrush brush(QColor(150, 168, 250)); //QWidget::setAutoFillBackground
  for (OdInt32 idx = 0, sz = m_pBranchTree->columnCount(); idx < sz; idx++)
  {
    if (pPrevCheckedoutItem)
      pPrevCheckedoutItem->setBackground(idx, *(m_pDefBrush.get()));
    pCheckedoutItem->setBackground(idx, brush);
  }

  OdString sBranch = controller()->currentBranchName(m_pRxDb);
  if (sBranch != sBranchPrev)
    updateBranches(true); // mark active inside combo

  //m_pDiffButton->setEnabled(false);
  getIConsole()->postponeCommand("regenall", false, m_pRxDb);
}

// slot
void OdqRevTab::clickedPropertyButton()
{
  m_curPropIndex = m_curSelIndex;
  m_isCompareMode = false;
  updateChanges();
}

// slot
void OdqRevTab::clickedCommitButton()
{
  getIConsole()->postponeCommand("commit", false, m_pRxDb);
}

// slot
void OdqRevTab::clickedDiffButton()
{
  IqViewSystem* iVS = getIViewSystem();
  ODA_ASSERT_ONCE(iVS);
  if (!iVS)
    return;
  //if (!m_pDiffButton.isNull())
  //  m_pDiffButton->setEnabled(false);

  if (m_curPropIndex >= 0)
    m_curDiffIndex = m_curPropIndex;
  else
    m_curDiffIndex = m_curSelIndex;
 
  updateChanges(true);
  //if (!m_pChangesGroup->isVisible())
  //  m_pChangesGroup->setVisible(true);

  m_pDiffSlider->setValue( 50 );
  m_pDiffCheckBox->setChecked( true );

  m_iDiffView = iVS->createViews(m_pRxDb);
  if (m_iDiffView)
    m_iDiffView->setActive();

  setGearVisible(m_curSelIndex, false);
}

bool OdqRevTab::isResposibleDiffView(IqView* iDiffView)
{
  if (!m_iDiffView && m_curDiffIndex >= 0)
    m_iDiffView = iDiffView; // catch first
  return m_iDiffView == iDiffView;
}

OdString OdqRevTab::currentBranchName() // in combo (an empty for All)
{
  ODA_ASSERT_ONCE(!m_pBranchCombo.isNull());
  if (m_pBranchCombo.isNull())
    return OdString::kEmpty;

  int idx = m_pBranchCombo->currentIndex();
  ODA_ASSERT_ONCE(idx >= 0);

  if (!idx)
    return OdString::kEmpty;
  if (idx == 1)
  {
    OdString sCurBranch = controller()->currentBranchName(m_pRxDb);
    ODA_ASSERT_ONCE(!sCurBranch.isEmpty());
    return sCurBranch;
  }

  OdString sActiveInComboBranchName = getIApp()->toOdString(m_pBranchCombo->currentText());
  return sActiveInComboBranchName;
}

OdCommitDescription* OdqRevTab::checkedOutRevision()
{
  if (m_curRevIndex < 0 || m_curRevIndex >(OdInt32) m_lstRevisions.size())
  {
    ODA_FAIL_ONCE(); // test
    return NULL;
  }

  return &m_lstRevisions[m_curRevIndex];
}

OdCommitDescription* OdqRevTab::topRevisionpOf(const OdString& sBranch)
{
  ODA_ASSERT_ONCE(!sBranch.isEmpty());

  for (OdUInt32 idx = 0, sz = m_lstRevisions.size(); idx < sz; idx++)
  {
    OdCommitDescription& rev = m_lstRevisions[idx];
    if (!rev.branches.contains(sBranch))
      continue;
    return &rev;
  }

  ODA_FAIL_ONCE(); // was not found // test
  return NULL;
}

const OdStringArray& OdqRevTab::branchNames()
{
  return m_lstBranchNames;
}

const OdString& OdqRevTab::lastCreatedBranchName()
{
  return m_sLastCreatedBranch;
}

void OdqRevTab::setLastCreatedBranchName(const OdString& sName)
{
  m_sLastCreatedBranch = sName;
}


#include "DbGsManager.h"
#include "ExtDbModule.h"

#include "DbVisualStyle.h"
#include "DbDictionary.h"
#include "AbstractViewPE.h"
#include "GiContextForDbDatabase.h"

OdSmartPtr<OdGsDevice> OdqRevTab::setupLayoutDiff(OdRxObject* pRxDb, OdGsDevice* pDevice, OdGiDefaultContext* pGiContext)
{
  m_rdHelper.clear();
  OdGsDevicePtr devicePtr;
  OdqRevisionController* ctrl = controller();
  
  OdSmartPtr<OdDbHostAppServices> hostApp = OdDbHostAppServices::cast(getIApp()->getBaseHostAppServices());

  OdInt32 idxFirst = 0, idxSecond = 0;
  if (!getIndexesToDiff(idxFirst, idxSecond))
  {
    ODA_FAIL_ONCE();
  }

  if (m_pRxFirstDb.isNull())
    m_pRxFirstDb = ctrl->checkoutDatabaseFix(pRxDb, m_lstRevisions[idxFirst], m_lstRevisions[m_lstRevisions.size() - 1], hostApp);
  if (m_pRxSecondDb.isNull())
    m_pRxSecondDb = ctrl->checkoutDatabaseFix(pRxDb, m_lstRevisions[idxSecond], m_lstRevisions[m_lstRevisions.size() - 1], hostApp);
  OdDbDatabasePtr mainDB = OdDbDatabase::cast(m_pRxFirstDb.get());
  OdDbDatabasePtr secondDB = OdDbDatabase::cast(m_pRxSecondDb.get());

  if( mainDB.isNull() || secondDB.isNull() )
  {
    ODA_ASSERT( false );
    return OdSmartPtr<OdGsDevice>();
  }

  OdDbBaseDatabasePEPtr pDbPE = OdDbBaseDatabasePE::cast(mainDB);
  OdGiContextForDbDatabase* pGiCtxDB = static_cast<OdGiContextForDbDatabase*>(pGiContext);
  pGiCtxDB->setDatabase( mainDB );
  
  devicePtr = pDbPE->setupActiveLayoutViews(pDevice, pGiContext);

  OdGsModel* pGsModel = NULL;
  OdGsLayoutHelperPtr pWrapper = OdGsLayoutHelper::cast(devicePtr.get());

  if (!pWrapper.isNull())
    pGsModel = pWrapper->gsModel();
  else
  {
    OdExtDbModule* pExtDb = getIApp()->getExtDbModule(mainDB);
    if (pExtDb)
      pGsModel = pExtDb->getGsModel(devicePtr.get());
  }
  m_rdHelper.setModel( pGsModel );
  m_rdHelper.setDevice( devicePtr.get() );
  m_rdHelper.setDB( mainDB, secondDB );

  m_rdHelper.setupDefaultDifferenceView( pGiContext->isPlotGeneration() );

  OdDbHandleList handles;
  ctrl->loadCommitDifference(pRxDb, handles, *digestByIndex(idxFirst), *digestByIndex(idxSecond));
  OdCmColor reColor;
  reColor.setRGB( 52, 152, 219 );
  m_rdHelper.setReColorOption( true, true, reColor, false );
  reColor.setRGB( 231, 76, 60 );
  m_rdHelper.setReColorOption( false, true, reColor, false );

  m_rdHelper.initViews( handles );
  m_rdHelper.reloadMainView( false );

  m_rdHelper.setVSbyVS( OdDb::kszVSRealistic ); 

  return devicePtr;
}

void OdqRevTab::syncView(OdRxObject* pRxDb, OdGsDevice* pDevice)
{
  m_rdHelper.syncViews();
}

void OdqRevTab::closeDiffView(OdRxObject* pRxDb)
{
  m_rdHelper.clear();
  m_curDiffIndex = -1;

  m_iDiffView = NULL;
  // will be done by close //m_isCompareMode = false;
  if (m_curPropIndex < 0 && m_pChangesGroup->isVisible())
    m_pChangesGroup->setVisible(false);
  updateChanges(true);

  setGearVisible(m_curSelIndex, true);
}

void OdqRevTab::setVisualStyle( const OdString& name )
{
  if( !m_iDiffView ) return;
  if( !m_rdHelper.isValid() ) return;
  m_rdHelper.setVSbyVS( name );
}
