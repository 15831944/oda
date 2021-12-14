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
// OdqPropertyPalette.h
//

#ifndef ODA_QT_PROPERTY_PALETTE_H_
#define ODA_QT_PROPERTY_PALETTE_H_

#include "OdqPropertyDialog.h"

#include <QTreeWidget> //<QTreeView>

//QT_BEGIN_NAMESPACE
//  class QPainter;
//QT_END_NAMESPACE

class OdqPropPaletteView : public QTreeWidget  // : public QTreeView
{
  Q_OBJECT

public:
  OdqPropPaletteView(QWidget* pParent = NULL);
  virtual ~OdqPropPaletteView();
  void saveSettings();
  void restoreSettings();

  void setModel(QAbstractItemModel* pModel);
  int necessaryHeight(const QStyleOptionViewItem& options, 
                      const QModelIndex& index) const;

protected:
  virtual void drawRow(QPainter* pPainter, 
                       const QStyleOptionViewItem& options, 
                       const QModelIndex& index) const;
  virtual void drawBranches(QPainter* pPainter,
                            const QRect& rect,
                            const QModelIndex& index) const;

  virtual void mouseMoveEvent(QMouseEvent* pEvent);
  virtual void mouseReleaseEvent(QMouseEvent* pEvent);
  virtual void mousePressEvent(QMouseEvent* pEvent);
  virtual bool event(QEvent* pEvent);

private:
  QModelIndex m_idxPicture;

  QPoint m_mousePos;
  bool m_bTracking;

  Q_DISABLE_COPY(OdqPropPaletteView)
};

class OdqStyledPropPaletteView : public OdqPropPaletteView
{
  Q_OBJECT

public:
  OdqStyledPropPaletteView(QWidget* pParent = NULL);
  virtual ~OdqStyledPropPaletteView();

  virtual void mouseMoveEvent(QMouseEvent* pEvent);
  virtual void mouseReleaseEvent(QMouseEvent* pEvent);
  virtual void mousePressEvent(QMouseEvent* pEvent);

//  virtual void keyPressEvent(QKeyEvent* pEvent);
//  virtual void contextMenuEvent(QContextMenuEvent* pEvent);
//public slots:
//  void copyRequested();

protected:
  virtual void drawRow(QPainter* pPainter, 
                       const QStyleOptionViewItem& options, 
                       const QModelIndex& index) const;

protected:
  int columnUnder(const QPoint &pos);

  bool m_bResize;
  int m_iColumn;
  int m_iLastPos;

private:
  Q_DISABLE_COPY(OdqStyledPropPaletteView)
};

class OdqPropPaletteDelegate : public OdqPropDelegate
{
  Q_OBJECT

public:
  OdqPropPaletteDelegate(OdqPropPaletteView *pView);
  virtual ~OdqPropPaletteDelegate();

  bool editorEvent(QEvent* pEvent, 
                   QAbstractItemModel* pModel, 
                   const QStyleOptionViewItem& option,
                   const QModelIndex& index);
  QSize sizeHint(const QStyleOptionViewItem& option, 
                 const QModelIndex& index) const;    
  void paint(QPainter* pPainter, 
             const QStyleOptionViewItem& option, 
             const QModelIndex& index) const;    

protected:
  virtual QRect arrowRect(const QRect& itemRect, 
                          const QModelIndex& index) const;
  virtual QRect checkRect(const QRect& itemRect, 
                          const QModelIndex& index) const;
  virtual void paintArrows(QPainter* pPainter, 
                           const QRect &arrowsRect,
                           const QModelIndex &index) const;
  void paintContents(QPainter* pPainter, 
                     const QStyleOptionViewItem& option,
                     const QModelIndex& index) const;

  OdqPropPaletteView* m_pView;

private:
  Q_DISABLE_COPY(OdqPropPaletteDelegate)
};

class OdqStyledPropPaletteDelegate : public OdqPropPaletteDelegate
{
  Q_OBJECT

public:
  OdqStyledPropPaletteDelegate(OdqPropPaletteView* pParent);
  ~OdqStyledPropPaletteDelegate();

  virtual void paint(QPainter* pPainter, 
                     const QStyleOptionViewItem& option,
                     const QModelIndex& index) const;
  virtual bool editorEvent(QEvent* pEvent, 
                           QAbstractItemModel* pModel,
                           const QStyleOptionViewItem& option,
                           const QModelIndex& index);
  virtual QSize sizeHint(const QStyleOptionViewItem& option,
                         const QModelIndex& index) const;
  virtual QWidget* createEditor(QWidget* pParent,
                                const QStyleOptionViewItem& option,
                                const QModelIndex& index) const;
protected:
  virtual QRect arrowRect(const QRect& itemRect, const QModelIndex& index) const;

private:
  QPainterPath createCountour(const QRect& rect, const QModelIndex& index) const;
  QRect outborderedRect(const QRect& rect, const QModelIndex& index) const;

  Q_DISABLE_COPY(OdqStyledPropPaletteDelegate)
};

#endif // ODA_QT_PROPERTY_PALETTE_H_
