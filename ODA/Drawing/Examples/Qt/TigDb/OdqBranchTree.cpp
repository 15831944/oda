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
// OdqBranchTree.cpp
//

#include "OdaCommon.h"
#include "TDVersion.h"

#include <QMainWindow>
#include <QtCore>
#include <QBitmap>
#include <QPainter>
#include <QSettings>
#include <QMouseEvent>
#include <QLabel>
#include <QHeaderView>

#include <QGraphicsScene>
#include <QImage>

#include "OdqBranchTree.h"

#include "OdqInterfaces.h"
#include "OdqRevisionPainter.h"

//static QModelIndex rootIndex(const QModelIndex& index)
//{
//  const OdqPropertyModel* pModel = qobject_cast<const OdqPropertyModel*>(index.model());
//  ODA_ASSERT_ONCE(pModel);
//  if (!pModel)
//    return QModelIndex();
//
//  return pModel->rootIndex(); // base path
//}
//
//#ifdef QT_DEBUG
// #define _DEBUG_qsPath(index) \
//  QString qsPath; \
//  { \
//    const OdqPropertyModel* pModel = qobject_cast<const OdqPropertyModel*>(index.model()); \
//    ODA_ASSERT_ONCE(pModel); \
//    if (pModel) \
//      qsPath = pModel->path(index); \
//  }
//#else
// #define _DEBUG_qsPath(index)  
//#endif


//////////////////////////////////////////////////////////////////////////

OdqBranchTree::OdqBranchTree(QWidget* pParent) // = NULL
  : QTreeWidget(pParent)
  , m_pLstBranchNames(NULL)
  , m_pLstRevisions(NULL)
//, m_bTracking(false)
{    
//  setItemDelegate(new OdqPropPaletteDelegate(this));
//  setEditTriggers(AllEditTriggers);
//#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
//  header()->setMovable(false);
//#else
//  header()->setSectionsMovable(false);
//#endif
//  setRootIsDecorated(false);
#if QT_VERSION >= QT_VERSION_CHECK(4,4,0)
  setUniformRowHeights(true);
  setItemsExpandable(false);
  setAutoScroll(false);
#endif
}

//void OdqBranchTree::setModel(QAbstractItemModel* pModel)
//{
//  OdqPropertyModel* pPropModel = qobject_cast<OdqPropertyModel*>(pModel);
//  ODA_ASSERT_ONCE(pPropModel);
//
//  if (model() != pModel)
//  {
//    QTreeView::setModel(pModel);
//    if (pPropModel)
//    {
//      QObject::connect(this, SIGNAL(collapsed(const QModelIndex&)), 
//                       pPropModel, SLOT(collapsed(const QModelIndex&)));
//      QObject::connect(this, SIGNAL(expanded(const QModelIndex&)),
//                       pPropModel, SLOT(expanded(const QModelIndex&)));
//      //QObject::connect(pPropModel, SIGNAL(collapse(const QModelIndex&)),
//      //                 this, SLOT(collapse(const QModelIndex&)));
//      //QObject::connect(pPropModel, SIGNAL(expand(const QModelIndex&)),
//      //                 this, SLOT(expand(const QModelIndex&)));
//
//      QObject::connect(selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)),
//                       pPropModel, SLOT(currentChanged(const QModelIndex&)));
//    }
//  }
//}

OdqBranchTree::~OdqBranchTree()
{
}

void OdqBranchTree::saveSettings()
{
  //OdqPropertyModel* pModel = qobject_cast<OdqPropertyModel*>(model());
  //if (!pModel)
  //  return;
  //QString qsPath = pModel->path(pModel->rootIndex()).toLower();
  //int widthColumn0 = columnWidth(0);
  //if (widthColumn0 < 2) // || qsPath.isEmpty())
  //  return;
  //QSettings settings("ODA", 
  //                   QFileInfo(QCoreApplication::applicationFilePath()).baseName());
  //settings.setValue(QString(TD_SHORT_STRING_VER_S "/palette/%1/column0").arg(qsPath),
  //                  widthColumn0);
}

void OdqBranchTree::restoreSettings()
{
  //OdqPropertyModel* pModel = qobject_cast<OdqPropertyModel*>(model());
  //if (!pModel)
  //  return;
  //QString qsPath = pModel->path(pModel->rootIndex()).toLower();
  //if (qsPath.isEmpty())
  //  return;
  //QSettings settings("ODA", 
  //                   QFileInfo(QCoreApplication::applicationFilePath()).baseName());
  //bool bOk = false;
  //int widthColumn0 = settings.value(QString(TD_SHORT_STRING_VER_S "/palette/%1/column0").arg(qsPath),
  //                                  "-1").toInt(&bOk);
  //if (!bOk || widthColumn0 == -1)
  //  widthColumn0 = columnWidth(0) * 2 / 3;
  //if (widthColumn0 < 2)
  //  return;
  //setColumnWidth(0, widthColumn0);
}

void OdqBranchTree::drawRow(QPainter* pPainter,
                            const QStyleOptionViewItem& options, 
                            const QModelIndex& index) const
{
  ODA_ASSERT_ONCE(index.isValid() && !index.column()); // TODO
  {
    //http://www.qtforum.org/article/14762/position-of-icon-in-tree-view.html
    QStyleOptionViewItem myopt(options);
    myopt.decorationPosition = QStyleOptionViewItem::Right;
    myopt.decorationAlignment = Qt::AlignLeft;
    QTreeView::drawRow(pPainter, myopt, index);
  }

  int idxRow = index.row();
  if (idxRow >= 0 && idxRow < m_pixmaps.length())
  {
    QPixmap pixmap = m_pixmaps[idxRow];
    QSize szPixmap = pixmap.size();

    QRect rectPixmap(options.rect);
    rectPixmap.setLeft(columnViewportPosition(0));
    //rectPixmap.setWidth(columnWidth(0));
    int height = rowHeight(index);
    int width = (szPixmap.width() * height) / szPixmap.height();
    rectPixmap.setHeight(height);
    //int width = columnWidth(0);
    rectPixmap.setWidth(width);

    //int widthCol = columnWidth(0);
    //if (width > widthCol)
    //  const_cast<OdqBranchTree*>(this)->setColumnWidth(0, width);

    //QRect rect(QPoint(), szPixmap);
    //rect.moveCenter(rectPixmap.center());
    //rect.moveTop(rectPixmap.top());

    pPainter->drawPixmap(rectPixmap.normalized(), pixmap);
  }
}

//void OdqBranchTree::drawBranches(QPainter* pPainter, 
//                                      const QRect& rect, 
//                                      const QModelIndex& index) const
//{
//  _DEBUG_qsPath(index)
//
//  int indent = OdqPropertyModel::indentation(index) + 1;
//  if (  indent <= 0
//      || OdqPropertyModel::isProperty(index))
//    return;
//
//  QRectF markRect(rect);
//  markRect.adjust(markRect.width() * 0.7, 0, 0, 0);
//
//  //{ // test
//  //  QPainterPath path;
//  //  path.addRect(markRect);
//  //  pPainter->fillPath(path, QBrush(QColor(Qt::blue)));// (QColor(Qt::gray)
//  //  return;
//  //}
//
//  markRect.moveCenter(QPointF(markRect.right(), markRect.center().y()));
//  QPainterPath path(markRect.center());
//  path.arcTo(markRect, 90.0, 180.0);
//  pPainter->fillPath(path, QBrush(QColor(Qt::gray)));
//}

//void OdqBranchTree::mouseMoveEvent(QMouseEvent* pEvent)
//{
//  m_mousePos = pEvent->pos();
//
//  if (m_bTracking)
//    doItemsLayout();
//  else
//    QTreeView::mouseMoveEvent(pEvent);
//}

QPoint OdqBranchTree::mousePos() const
{
  return m_posMouse;
}

static QImage createSubImage(QImage* image, const QRect & rect)
{
  size_t offset = rect.x() * image->depth() / 8 + rect.y() * image->bytesPerLine();
  return QImage(image->bits() + offset, rect.width(), rect.height(), image->bytesPerLine(), image->format());
}

void OdqBranchTree::updateGraphData(OdStringArray* pLstBranchNames, OdCommitDescriptionList* pLstRevisions, const OdString& sBranchOnly)
{
  if (pLstBranchNames)
    m_pLstBranchNames = pLstBranchNames;
  if (pLstRevisions)
    m_pLstRevisions = pLstRevisions;
  m_sBranchOnly = sBranchOnly;

  ODA_ASSERT_ONCE(m_pLstRevisions);
  if (!m_pLstRevisions)
    return;
  int szTree = topLevelItemCount();

  QGraphicsScene scene(getIApp()->getMainWindow());
  ////gv->setScene(scene); gv->setSceneRect(QRect(0, 0, 100, 1000));
  //QGraphicsScene scene(0, 100, 40, 187, getIApp()->getMainWindow());
  //QGraphicsScene(qreal x, qreal y, qreal width, qreal height, QObject * parent = 0)//

  OdqRevisionPainter revPainter(&scene);

  OdStringArray branches;
  OdCommitDescriptionList revs;

#if !defined(REVISION_PAITER_LOCAL_TEST)

  for (int idx = 0, szRevs = (int)m_pLstRevisions->size(); idx < szTree; idx++)
  {
    QTreeWidgetItem* pTreeItem = topLevelItem(idx);
    int indexChild = (int) pTreeItem->data(0, Qt::UserRole).toInt();
    ODA_ASSERT_ONCE(indexChild >= 0 && indexChild < szRevs);

    OdCommitDescription desc = (*m_pLstRevisions)[indexChild];
    revs.append(desc);

    // to add branches in reverse order
    //indexChild = (int)topLevelItem(szTree - idx - 1)->data(0, Qt::UserRole).toInt();
    //desc = (*m_pLstRevisions)[indexChild];

    for (int idxBr = 0, szBr = desc.branches.size(); idxBr < szBr; idxBr++)
    {
      OdString sBr = desc.branches[idxBr];
      if (!sBr.isEmpty() && !branches.contains(sBr))
        branches.append(sBr);
    }
  }

#else
  branches.append("master");
  branches.append("branch1");
  for (int i = 0; i < 10; i++) {
    OdCommitDescription desc;
    desc.parents.clear();
    if (i == 1) {
      desc.id = getIApp()->toOdString(QString::number(i));
      desc.branches.push_back(L"master");
      desc.parents.append(getIApp()->toOdString(QString::number(5)));
      desc.parents.append(getIApp()->toOdString(QString::number(2)));
    }
    else if (i >= 2 && i <= 4) {
      desc.id = getIApp()->toOdString(QString::number(i));
      desc.branches.push_back(L"branch1");
      desc.parents.append(getIApp()->toOdString(QString::number(i + 1)));
    }
    else {
      desc.id = getIApp()->toOdString(QString::number(i));
      desc.branches.push_back(L"master");
      desc.parents.append(getIApp()->toOdString(QString::number(i + 1)));
    }
    revs.append(desc);
  }
#endif
  revPainter.setBranchWidth(3); //(6);
  revPainter.setSpace(15, 30); //(20, 30);
  revPainter.setRadius(10, 15); //(16);

  if (branches.size() > 1 && pLstBranchNames)
  {
    OdStringArray branchesUnsorted = branches;
    branches.clear();
    for (OdUInt32 idx = 0, sz = pLstBranchNames->size(); idx < sz; idx++)
    {
      OdString sName = pLstBranchNames->getAt(idx);
      if (!branchesUnsorted.contains(sName) || sName.isEmpty())
        continue;
      branches.push_back(sName);
    }
  }

  int verGraph = getIAppProps()->toInt(L"Vars/tiggraph",
                                  1); // initial version in conference 2017
  revPainter.setData(branches, revs, verGraph);

  revPainter.draw();

  QImage image(scene.width(), scene.height(), QImage::Format_RGB888); // = QImage::Format_RGB32; //QImage::Format_ARGB32_Premultiplied);
  image.fill(QColor(Qt::white).rgb());
  QPainter painter(&image);
  scene.render(&painter);

  //QPixmap pixmap;
  //QPainter pixPainter(&pixmap);
  //YourEditorScene->render(&pixPainter, sceneRect, sceneRect);
  //pixmap.save(filename, "PNG"); // writes pixmap into bytes in PNG format

  ODA_ASSERT_ONCE(!image.isNull());

  QString qsPath = QDir::toNativeSeparators(QDir::homePath() + "/ODA/grab");
  if (QDir(qsPath).exists())
    image.save(QDir::toNativeSeparators(qsPath + "/graph_tig.png"));

  QRect rect = image.rect();

  int num = revs.size();
  int heightImage = image.height();

  double dHeight = double(heightImage) / double(num);
  rect.setBottom(rect.top() + (int)dHeight);

  m_pixmaps.clear();
  for (int idx = 0; idx < num; idx++)
  {
    QRect rectIdx = rect.translated(0, (int)(dHeight * idx)); // (0, (int)(dHeight * idx + 0.5));
    if (rectIdx.bottom() == heightImage)
      rectIdx.setBottom(heightImage - 1); //workaround for last unfilled raster line

    QImage imageIdx = createSubImage(&image, rectIdx);
    if (QDir(qsPath).exists())
      imageIdx.save(QDir::toNativeSeparators(qsPath + QString("/graph_tig_%1.png").arg(idx)));

    if (idx >= szTree)
      continue;

    QTreeWidgetItem* pTreeItem = topLevelItem(idx);
    //OdInt32 indexChild = (OdInt32)pTreeItem->data(0, Qt::UserRole).toInt();

    QPixmap pixmap = QPixmap::fromImage(imageIdx);
    ODA_ASSERT_ONCE(!pixmap.isNull());
    pixmap.setMask(pixmap.createHeuristicMask());
    m_pixmaps.push_back(pixmap);

    // better set an empty image fot uatoresizing by content //pTreeItem->setIcon(0, QIcon());
    QImage imgEmpty(pixmap.width(), pixmap.height(), QImage::Format_RGB888);
    imgEmpty.fill(QColor(Qt::white).rgb());
    //if (QDir(qsPath).exists())
    //  imgEmpty.save(QDir::toNativeSeparators(qsPath + QString("/graph_empty_%1.png").arg(idx)));
    pixmap = QPixmap::fromImage(imgEmpty);
    pixmap.setMask(pixmap.createHeuristicMask());
    //pTreeItem->setIcon(0, QIcon(pixmap));
    QLabel* pIconLabel = qobject_cast<QLabel*>(itemWidget(pTreeItem, 0));
    if (!pIconLabel)
      pIconLabel = new QLabel();
    pIconLabel->setPixmap(pixmap);
    pIconLabel->setAlignment(Qt::AlignLeft);
    pIconLabel->setIndent(0);
    pIconLabel->setMargin(0);
    setItemWidget(pTreeItem, 0, pIconLabel);
  }
}

void OdqBranchTree::mouseReleaseEvent(QMouseEvent* pEvent)
{
//  if (m_bTracking)
//  {
//    m_bTracking = false;
//    doItemsLayout();
//  }
//
  QTreeView::mouseReleaseEvent(pEvent);
}

void OdqBranchTree::mousePressEvent(QMouseEvent* pEvent)
{
  m_posMouse = pEvent->pos(); // = pEvent->globalPos());
//  QModelIndex index = indexAt(pEvent->pos());
//
//  if (index.isValid() && OdqPropertyModel::type(index) == IqAppProps::prpImage)
//  {
//    m_bTracking = true;
//    m_idxPicture = index;
//  }
//
  QTreeView::mousePressEvent(pEvent);
}

//bool OdqBranchTree::event(QEvent* pEvent)
//{
//  if (pEvent->type() == QEvent::Polish && !isVisible())
//    return false;
//
//  return QTreeWidget::event(pEvent);
//}

//int OdqBranchTree::necessaryHeight(const QStyleOptionViewItem& options,
//                                   const QModelIndex& index) const
//{
//  if (   !m_bTracking
//      || index.internalPointer() != m_idxPicture.internalPointer())
//    return 0;
//
//  int height = qMax(qAbs(m_mousePos.x()), qAbs(m_mousePos.y()));
//  return height - options.rect.top();
//}


