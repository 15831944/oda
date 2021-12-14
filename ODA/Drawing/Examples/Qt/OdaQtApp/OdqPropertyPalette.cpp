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
// OdqPropertyPalette.cpp
//

#include "OdaCommon.h"
#include "TDVersion.h"

#include <QMainWindow>
#include <QHeaderView>
#include <QBitmap>
#include <QPainter>
#include <QSettings>
#include <QMouseEvent>
#include <QtCore>

#include "OdqPropertyPalette.h"

static QModelIndex rootIndex(const QModelIndex& index)
{
  const OdqPropertyModel* pModel = qobject_cast<const OdqPropertyModel*>(index.model());
  ODA_ASSERT_ONCE(pModel);
  if (!pModel)
    return QModelIndex();

  return pModel->rootIndex(); // base path
}

#ifdef QT_DEBUG
 #define _DEBUG_qsPath(index) \
  QString qsPath; \
  { \
    const OdqPropertyModel* pModel = qobject_cast<const OdqPropertyModel*>(index.model()); \
    ODA_ASSERT_ONCE(pModel); \
    if (pModel) \
      qsPath = pModel->path(index); \
  }
#else
 #define _DEBUG_qsPath(index)  
#endif


//////////////////////////////////////////////////////////////////////////

OdqPropPaletteView::OdqPropPaletteView(QWidget* pParent) // = NULL
  : QTreeWidget(pParent)
  , m_bTracking(false)
{    
  setItemDelegate(new OdqPropPaletteDelegate(this));
  setEditTriggers(AllEditTriggers);
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
  header()->setMovable(false);
#else
  header()->setSectionsMovable(false);
#endif
  setRootIsDecorated(false);
}

void OdqPropPaletteView::setModel(QAbstractItemModel* pModel)
{
  OdqPropertyModel* pPropModel = qobject_cast<OdqPropertyModel*>(pModel);
  ODA_ASSERT_ONCE(pPropModel);

  if (model() != pModel)
  {
    QTreeView::setModel(pModel);
    if (pPropModel)
    {
      QObject::connect(this, SIGNAL(collapsed(const QModelIndex&)), 
                       pPropModel, SLOT(collapsed(const QModelIndex&)));
      QObject::connect(this, SIGNAL(expanded(const QModelIndex&)),
                       pPropModel, SLOT(expanded(const QModelIndex&)));
      //QObject::connect(pPropModel, SIGNAL(collapse(const QModelIndex&)),
      //                 this, SLOT(collapse(const QModelIndex&)));
      //QObject::connect(pPropModel, SIGNAL(expand(const QModelIndex&)),
      //                 this, SLOT(expand(const QModelIndex&)));

      QObject::connect(selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)),
                       pPropModel, SLOT(currentChanged(const QModelIndex&)));
    }
  }

  //if (pPropModel)
  //  pPropModel->restoreExpanded();
}

OdqPropPaletteView::~OdqPropPaletteView()
{
}

void OdqPropPaletteView::saveSettings()
{
  OdqPropertyModel* pModel = qobject_cast<OdqPropertyModel*>(model());
  if (!pModel)
    return;
  QString qsPath = pModel->path(pModel->rootIndex()).toLower();
  int widthColumn0 = columnWidth(0);
  if (qsPath.isEmpty() || widthColumn0 < 2)
    return;
  QSettings settings("ODA", 
                     QFileInfo(QCoreApplication::applicationFilePath()).baseName());
  settings.setValue(QString(TD_SHORT_STRING_VER_S "/palette/%1/column0").arg(qsPath),
                    widthColumn0);
}

void OdqPropPaletteView::restoreSettings()
{
  OdqPropertyModel* pModel = qobject_cast<OdqPropertyModel*>(model());
  if (!pModel)
    return;
  QString qsPath = pModel->path(pModel->rootIndex()).toLower();
  if (qsPath.isEmpty())
    return;
  QSettings settings("ODA", 
                     QFileInfo(QCoreApplication::applicationFilePath()).baseName());
  bool bOk = false;
  int widthColumn0 = settings.value(QString(TD_SHORT_STRING_VER_S "/palette/%1/column0").arg(qsPath),
                                    "-1").toInt(&bOk);
  if (!bOk || widthColumn0 == -1)
    widthColumn0 = columnWidth(0) * 2 / 3;
  if (widthColumn0 < 2)
    return;
  setColumnWidth(0, widthColumn0);
}

void OdqPropPaletteView::drawRow(QPainter* pPainter,
                                 const QStyleOptionViewItem& options, 
                                 const QModelIndex& index) const
{
  //ODA_ASSERT_ONCE(model()->rowCount(index));
  //if (!model()->rowCount(index))
  //  return;
  //OdqPropertyModel* pModel = qobject_cast<OdqPropertyModel*>(model());

  if (   OdqPropertyModel::isProperty(index) 
      && OdqPropertyModel::type(index) == IqAppProps::prpImage)
  {
    QRect imageRect(options.rect);
    imageRect.setLeft(columnViewportPosition(0) + OdqPropertyModel::indentation(index) * indentation());
    imageRect.setWidth(columnWidth(0) + columnWidth(1));

    QString qsImagePath = model()->data(index, Qt::DecorationRole).toString();
    QPixmap pixmap(qsImagePath);
    ODA_ASSERT_ONCE(!pixmap.isNull());
    pixmap.setMask(pixmap.createHeuristicMask());

    QRect rect(QPoint(), pixmap.size());
    rect.moveCenter(imageRect.center());
    rect.moveTop(imageRect.top());
    if (m_bTracking && (index.internalPointer() == m_idxPicture.internalPointer()))
    {
      QSize sz = rect.size();
      int height = qMax(qAbs(m_mousePos.x()), qAbs(m_mousePos.y()));
      if (height > imageRect.top() && sz.height())
      {
        int widthHalf = height * sz.width() / sz.height() / 2;
        rect.setRight(imageRect.center().x() + widthHalf);
        rect.setLeft(imageRect.center().x() - widthHalf);
        rect.setBottom(height);
      }
    }

    pPainter->drawPixmap(rect.normalized(), pixmap);
  }
  else
    QTreeView::drawRow(pPainter, options, index);
}

void OdqPropPaletteView::drawBranches(QPainter* pPainter, 
                                      const QRect& rect, 
                                      const QModelIndex& index) const
{
  _DEBUG_qsPath(index)

  int indent = OdqPropertyModel::indentation(index) + 1;
  if (  indent <= 0
      || OdqPropertyModel::isProperty(index))
    return;

  QRectF markRect(rect);
  markRect.adjust(markRect.width() * 0.7, 0, 0, 0);

  //{ // test
  //  QPainterPath path;
  //  path.addRect(markRect);
  //  pPainter->fillPath(path, QBrush(QColor(Qt::blue)));// (QColor(Qt::gray)
  //  return;
  //}

  markRect.moveCenter(QPointF(markRect.right(), markRect.center().y()));
  QPainterPath path(markRect.center());
  path.arcTo(markRect, 90.0, 180.0);
  pPainter->fillPath(path, QBrush(QColor(Qt::gray)));
}

void OdqPropPaletteView::mouseMoveEvent(QMouseEvent* pEvent)
{
  m_mousePos = pEvent->pos();

  if (m_bTracking)
    doItemsLayout();
  else
    QTreeView::mouseMoveEvent(pEvent);
}

void OdqPropPaletteView::mouseReleaseEvent(QMouseEvent* pEvent)
{
  if (m_bTracking)
  {
    m_bTracking = false;
    doItemsLayout();
  }

  QTreeView::mouseReleaseEvent(pEvent);
}

void OdqPropPaletteView::mousePressEvent(QMouseEvent* pEvent)
{
  QModelIndex index = indexAt(pEvent->pos());
  //OdqPropertyModel* pModel = qobject_cast<OdqPropertyModel*>(model());

  if (index.isValid() && OdqPropertyModel::type(index) == IqAppProps::prpImage)
  {
    m_bTracking = true;
    m_idxPicture = index;
  }

  QTreeView::mousePressEvent(pEvent);
}

bool OdqPropPaletteView::event(QEvent* pEvent)
{
  //const OdqPropertyModel* pModel = qobject_cast<const OdqPropertyModel*>(model());
  //if (pModel && pModel->rowCount(pModel->rootIndex()) < 1)
  //  return false;
  //#16431 
  if (pEvent->type() == QEvent::Polish && !isVisible())
    return false;

  return QTreeWidget::event(pEvent);
}

int OdqPropPaletteView::necessaryHeight(const QStyleOptionViewItem& options,
                                        const QModelIndex& index) const
{
  if (   !m_bTracking
      || index.internalPointer() != m_idxPicture.internalPointer())
    return 0;

  int height = qMax(qAbs(m_mousePos.x()), qAbs(m_mousePos.y()));
  return height - options.rect.top();
}

//////////////////////////////////////////////////////////////////////////

OdqStyledPropPaletteView::OdqStyledPropPaletteView(QWidget* pParent) // = NULL
 : OdqPropPaletteView(pParent)
 , m_bResize(false)
{ 
  setItemDelegate(new OdqStyledPropPaletteDelegate(this));

  if (!pParent)
    pParent = getIApp()->getMainWindow();
  QPalette palette = pParent->palette();
  palette.setColor(QPalette::Base, palette.color(QPalette::Window));
  palette.setColor(QPalette::Window, palette.color(QPalette::Window));
  setPalette(palette);

  header()->hide();
  setMouseTracking(true);
}

OdqStyledPropPaletteView::~OdqStyledPropPaletteView()
{
}

int OdqStyledPropPaletteView::columnUnder(const QPoint& pos)
{
  int xpos = pos.x();

  int visual = header()->visualIndexAt(xpos);
  if (visual == -1)
    return -1;

  int log = header()->logicalIndex(visual);
  int secPos = header()->sectionViewportPosition(log);
  int grip = style()->pixelMetric(QStyle::PM_HeaderGripMargin);

  int res = -1;
  if (visual > 0 && xpos < secPos + grip)
    res = header()->logicalIndex(visual - 1);
  if (xpos > secPos + header()->sectionSize(log) - grip)
    res = log;
  if (res == header()->count() - 1)
    res = -1;
  return res;
}

void OdqStyledPropPaletteView::mouseMoveEvent(QMouseEvent* pEvent)
{    
  if (m_bResize)
  {
    int delta = pEvent->pos().x() - m_iLastPos;
    int newsize = header()->sectionSize(m_iColumn) + delta;
    header()->resizeSection(m_iColumn, newsize);
    m_iLastPos = pEvent->pos().x();
  }
  else
  {
    int sec = columnUnder(pEvent->pos());
    setCursor(sec == -1 ? Qt::ArrowCursor : Qt::SplitHCursor);
  }

  OdqPropPaletteView::mouseMoveEvent(pEvent);        
}

void OdqStyledPropPaletteView::mousePressEvent(QMouseEvent* pEvent)
{    
  QPoint position = pEvent->pos();
  m_iColumn = columnUnder(position);

  m_bResize = (m_iColumn != -1);
  if (m_bResize)
    m_iLastPos = position.x();

  OdqPropPaletteView::mousePressEvent(pEvent);
}

void OdqStyledPropPaletteView::mouseReleaseEvent(QMouseEvent* pEvent)
{
  if (m_bResize)
  {
    m_bResize = false;
    return;
  }    

  OdqPropPaletteView::mouseReleaseEvent(pEvent);
}

void OdqStyledPropPaletteView::drawRow(QPainter* pPainter, 
                                    const QStyleOptionViewItem& options, 
                                    const QModelIndex& index) const
{
  QStyleOptionViewItem modified(options);
  modified.showDecorationSelected = false;

  OdqPropPaletteView::drawRow(pPainter, modified, index);    
}

//void OdqStyledPropPaletteView::keyPressEvent(QKeyEvent* pEvent)
//{
//  if (pEvent == QKeySequence::Copy)
//  {
//    copyRequested();
//    return;
//  }
//
//  OdqPropPaletteView::keyPressEvent(pEvent);
//}

//void OdqStyledPropPaletteView::contextMenuEvent(QContextMenuEvent* pEvent)
//{
//  QModelIndex index = selectionModel()->currentIndex();
//  if (index.column() == 0)
//    index = index.sibling(index.row(), 1);
//
//  QAction *pCopyAction = new QAction(QObject::tr("Copy"), this);
//  pCopyAction->setEnabled(index.isValid());
//  QObject::connect(pCopyAction, SIGNAL(triggered()), this, SLOT(copyRequested()));
//
//  QMenu menu;    
//  menu.addAction(pCopyAction);
//  menu.exec(pEvent->globalPos());
//}

//void OdqStyledPropPaletteView::copyRequested()
//{
//  QModelIndex index = selectionModel()->currentIndex();
//
//  if (!index.isValid())
//    return;
//  
//  if (index.column() == 0)
//    index = index.sibling(index.row(), 1);
//
//  QString qsText = index.data().toString();
//  QApplication::clipboard()->setText(qsText);
//}

//////////////////////////////////////////////////////////////////////////

OdqPropPaletteDelegate::OdqPropPaletteDelegate(OdqPropPaletteView* pView) 
  : OdqPropDelegate(pView)
{
  m_pView = pView;
}

OdqPropPaletteDelegate::~OdqPropPaletteDelegate()
{
}

bool OdqPropPaletteDelegate::editorEvent(QEvent* pEvent, 
                                         QAbstractItemModel* pModel, 
                                         const QStyleOptionViewItem& option, 
                                         const QModelIndex& index)
{
  OdqPropertyModel* pPropModel = qobject_cast<OdqPropertyModel*>(pModel);
  if (pEvent->type() == QEvent::MouseButtonRelease)
  {        
    if (!OdqPropertyModel::isProperty(index))
    {
      QMouseEvent* pMouseEvent = static_cast<QMouseEvent*>(pEvent);
      QPoint pt = pMouseEvent->pos();
      if (   checkRect(option.rect, index).contains(pt) 
          && (pPropModel->flags(index) & Qt::ItemIsUserCheckable))
      {
        QVariant value = index.data(Qt::UserRole);
        Qt::CheckState state = static_cast<Qt::CheckState>(value.toInt());
        return pPropModel->setData(index, (state == Qt::Unchecked ? Qt::Checked : Qt::Unchecked), Qt::UserRole);
      }
      if (arrowRect(option.rect, index).contains(pt))
      {
        bool bValue = OdqPropertyModel::isExpanded(index);
        m_pView->setExpanded(index, !bValue);
#      if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
        if (index.column() && OdqPropertyModel::isExpanded(index) != !bValue)
        {
          QModelIndex idx = index.sibling(index.row(), 0);
          m_pView->setExpanded(idx, !bValue);
        }
#      endif
        ODA_ASSERT_ONCE_X(PROPPAL, OdqPropertyModel::isExpanded(index) == !bValue);
        return true;
      }
    }
  }

  return OdqPropDelegate::editorEvent(pEvent, pModel, option, index);
}

QSize OdqPropPaletteDelegate::sizeHint(const QStyleOptionViewItem& option,
                                       const QModelIndex& index) const
{
  QSize size = OdqPropDelegate::sizeHint(option, index);

  if (OdqPropertyModel::type(index) != IqAppProps::prpImage)
    return size;
  
  const OdqPropertyModel* pModel = qobject_cast<const OdqPropertyModel*>(index.model());
  QPixmap pixmap(pModel->data(index, Qt::DecorationRole).toString());
  ODA_ASSERT_ONCE(!pixmap.isNull());
  int height = qMax(pixmap.height(), m_pView->necessaryHeight(option, index));
  size.setHeight(height);
  return size;
}

void OdqPropPaletteDelegate::paintContents(QPainter* pPainter, 
                                           const QStyleOptionViewItem& option, 
                                           const QModelIndex& index) const
{
  OdqPropDelegate::paint(pPainter, option, index);

  if (OdqPropertyModel::isProperty(index))
    return;

  _DEBUG_qsPath(index)

  const OdqPropertyModel* pModel = qobject_cast<const OdqPropertyModel*>(index.model());
  if (index.column() != pModel->columnCount() - 1)
    return;

  // draw category

  QColor color(Qt::black);
  pPainter->setPen(color);
  QRect check = checkRect(option.rect, index);

  QString qsImagePath; 
  if (OdqPropertyModel::isWithImage(index, &qsImagePath))
  {
    pPainter->drawRect(check);
    pPainter->fillRect(check, QBrush(QColor(Qt::white)));
    //if (pModel->data(index, Qt::UserRole) == Qt::Checked)
    //  pPainter->drawLine(check.bottomLeft(), check.topRight());
    ODA_ASSERT_ONCE(!qsImagePath.isEmpty());
    if (!qsImagePath.isEmpty())
    {
      QPixmap pixmap(qsImagePath);
      ODA_ASSERT_ONCE(!pixmap.isNull());
      pixmap.setMask(pixmap.createHeuristicMask());
      pPainter->drawPixmap(check, pixmap);
    }
  }

  QRect rect = arrowRect(option.rect, index);
  paintArrows(pPainter, rect, index);
}

void OdqPropPaletteDelegate::paint(QPainter* pPainter, 
                                   const QStyleOptionViewItem& option, 
                                   const QModelIndex& index) const
{
  paintContents(pPainter, option, index);

  QRect frame = option.rect;
  frame.adjust(0, 0, -1, -1);
  pPainter->setPen(QColor(Qt::gray));
  pPainter->drawRect(frame);
}

QRect OdqPropPaletteDelegate::arrowRect(const QRect& itemRect, const QModelIndex& ) const
{
  QRect rect(itemRect);
  int offset = rect.height() / 10;
  rect.adjust(0, offset, -offset, -offset);
  rect.adjust(rect.width() - rect.height(), 0, 0, 0);

  return rect;
}

QRect OdqPropPaletteDelegate::checkRect(const QRect& itemRect, 
                                        const QModelIndex& index) const
{
  QRect rect = arrowRect(itemRect, index);
  rect.moveLeft(rect.left() - (rect.width() * 3 / 2));
  return rect;
}

void OdqPropPaletteDelegate::paintArrows(QPainter* pPainter, 
                                         const QRect& rect, 
                                         const QModelIndex& index) const
{
  _DEBUG_qsPath(index)

  QColor colorStripe((!index.parent().isValid() || index.parent() == rootIndex(index)) ? Qt::white 
                                                                                       : Qt::black);
  QPen pen(colorStripe);
  pPainter->setPen(pen);
  int sign = OdqPropertyModel::isExpanded(index) ? 1 : -1;

  foreach(int offs, QList<int>() << -2 << -1 << 2 << 3)
  {
    QPoint cent = rect.center();
    if (sign)
      offs  += rect.width() / 5;
    cent.ry() -= offs;

    QPoint left = cent;
    left.setX(left.x() - rect.width() / 5);
    left.setY(left.y() + sign * rect.width() / 5);

    pPainter->drawLine(QLine(cent, left));

    cent.rx() += 1;
    QPoint right = cent;
    right.setX(right.x() + rect.width() / 5);
    right.setY(right.y() + sign * rect.width() / 5);

    pPainter->drawLine(QLine(cent, right));
  }
}

//////////////////////////////////////////////////////////////////////////

OdqStyledPropPaletteDelegate::OdqStyledPropPaletteDelegate(OdqPropPaletteView* pParent) 
 : OdqPropPaletteDelegate(pParent)
{
}

OdqStyledPropPaletteDelegate::~OdqStyledPropPaletteDelegate()
{
}

bool OdqStyledPropPaletteDelegate::editorEvent(QEvent* pEvent, 
                                               QAbstractItemModel* pModel, 
                                               const QStyleOptionViewItem& option, 
                                               const QModelIndex& index)
{
  return OdqPropPaletteDelegate::editorEvent(pEvent, pModel, option, index);
}

QSize OdqStyledPropPaletteDelegate::sizeHint(const QStyleOptionViewItem& option,
                                             const QModelIndex& index) const
{
  QSize size = OdqPropDelegate::sizeHint(option, index);

  _DEBUG_qsPath(index)

  if (OdqPropertyModel::type(index) == IqAppProps::prpImage)
  {
    const OdqPropertyModel* pModel = qobject_cast<const OdqPropertyModel*>(index.model());
    QPixmap pixmap(pModel->data(index, Qt::DecorationRole).toString());
    ODA_ASSERT_ONCE(!pixmap.isNull());
    int height = qMax(pixmap.height(), m_pView->necessaryHeight(option, index));
    size.setHeight(height);
    return size;
  }

  if (!index.parent().isValid() || index.parent() == rootIndex(index))
    size.setHeight(size.height() * 3 / 2);
  return size;
}

QPainterPath OdqStyledPropPaletteDelegate::createCountour(const QRect& bound, 
                                                          const QModelIndex& index) const
{
  _DEBUG_qsPath(index)

  QRectF rect(bound);
  rect.adjust(0, 0, 1, 1);

  QPainterPath path;

  if (!index.parent().isValid() || index.parent() == rootIndex(index))
  {
    int xl = rect.left();
    int xc = rect.left() + rect.height() / 4;
    int xrc = rect.right() - rect.height() / 4;
    int xr = rect.right();
    int yl = rect.bottom();
    int yr = rect.top();
    int yc = rect.top() + rect.height() / 4;

    if (index.column() == 0)
    {
      path.moveTo(QPoint(xl, yl));
      path.lineTo(QPoint(xl, yc));
      path.lineTo(QPoint(xc, yr));
      path.lineTo(QPoint(xr, yr));
      path.lineTo(QPoint(xr, yl));
      path.lineTo(QPoint(xl, yl));
    }
    else
    {
      path.moveTo(QPoint(xl, yl));
      path.lineTo(QPoint(xl, yr));
      path.lineTo(QPoint(xrc, yr));
      path.lineTo(QPoint(xr, yc));
      path.lineTo(QPoint(xr, yl));
      path.lineTo(QPoint(xl, yl));
    }

    return path;
  }

  path.moveTo(rect.bottomLeft());
  path.lineTo(rect.bottomRight());
  path.lineTo(rect.topRight());
  path.lineTo(rect.topLeft());
  path.lineTo(rect.bottomLeft());

  return path;
}

QRect OdqStyledPropPaletteDelegate::outborderedRect(const QRect& rect, 
                                                    const QModelIndex& index) const
{
  _DEBUG_qsPath(index)

  QRect res(rect);

  if (!index.parent().isValid() || index.parent() == rootIndex(index))
    res.adjust(0, res.height() / 3, 0, 0);

  if (index.column() == 0)
    res.adjust(res.height() / 4 , 0, 0, 0);
  else
    res.adjust(0, 0, -res.height() / 4, 0);

  return res;
}

QRect OdqStyledPropPaletteDelegate::arrowRect(const QRect& itemRect, 
                                              const QModelIndex& index) const
{
  _DEBUG_qsPath(index)

  QRect alignedRect(itemRect);
  if (index.parent().isValid() && index.parent() != rootIndex(index))
    alignedRect = outborderedRect(alignedRect, index);

  return OdqPropPaletteDelegate::arrowRect(alignedRect, index);
}

QWidget * OdqStyledPropPaletteDelegate::createEditor(QWidget *parent, 
                                                     const QStyleOptionViewItem& option, 
                                                     const QModelIndex& index) const
{
  QWidget *pEditor = OdqPropPaletteDelegate::createEditor(parent, option, index);
  if (pEditor)
    pEditor->setPalette(QApplication::palette());
  return pEditor;
}

void OdqStyledPropPaletteDelegate::paint(QPainter *painter, 
                                         const QStyleOptionViewItem& option, 
                                         const QModelIndex& index) const
{
  QStyleOptionViewItem pass(option);
  pass.palette.setColor(QPalette::Base, QApplication::palette().button().color());
  pass.state &= (~QStyle::State_HasFocus);

  bool bActive = m_pView->selectionModel()->isSelected(index);
  if (bActive)
    pass.state |= QStyle::State_Active;
  else
    pass.state &= (~QStyle::State_Active);

  _DEBUG_qsPath(index)

  bool bDrawPropValue = !index.model()->hasChildren(index) && (index.column() != 0);
  if (bDrawPropValue)
  {
    QColor backColor = qvariant_cast<QColor>(index.data(Qt::BackgroundColorRole));
    pass.palette.setColor(QPalette::Highlight, backColor);
    pass.palette.setColor(QPalette::HighlightedText, pass.palette.text().color());
  }
 #ifdef QT_DEBUG
  else
    bDrawPropValue = bDrawPropValue; // brk
 #endif

  if (index.parent().isValid() && index.parent() != rootIndex(index))
  {
    OdqPropPaletteDelegate::paint(painter, pass, index);
    if (bDrawPropValue && bActive)
    {
      QRect frame(option.rect);
      frame.adjust(0, 0, -1, -1);

      QPen pen(option.palette.highlight().color());
      painter->setPen(pen);
      painter->drawRect(frame);
    }
    return;
  }

  pass.rect = outborderedRect(option.rect, index);
  pass.font.setBold(true);

  QRect frame = option.rect;
  frame.setTop(pass.rect.top());

  QColor penColor(Qt::gray);
  if (m_pView->selectionModel()->isSelected(index))
    penColor = option.palette.highlight().color();
  painter->setPen(penColor);

  QVariant varColor = index.data(Qt::BackgroundColorRole);
  QColor backColor = qvariant_cast<QColor>(varColor);
  if (m_pView->selectionModel()->isSelected(index))
    backColor = m_pView->palette().highlight().color();

  QPainterPath path = createCountour(frame, index);
  painter->drawPath(path);
  painter->fillPath(path, backColor);

  paintContents(painter, pass, index);
}
