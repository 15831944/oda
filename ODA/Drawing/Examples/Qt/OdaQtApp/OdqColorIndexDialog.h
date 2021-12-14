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
// OdqColorIndexDialog.h
//

#ifndef ODA_QT_COLOR_INDEX_DIALOG_H_
#define ODA_QT_COLOR_INDEX_DIALOG_H_

#include "OdaCommon.h"
#include "Gs/GsBaseVectorizeDevice.h" // for ODGSPALETTE only

#include "OdqPropertyDialog.h"

///////////////////////////////////////////////////////////////////////////////
// see also QWellArray in Qt source

#if !defined(Q_DECL_OVERRIDE)
#define Q_DECL_OVERRIDE // for QT4
#endif

class OdqWellArray : public QWidget
{
  Q_OBJECT
  Q_PROPERTY(int selectedColumn READ selectedColumn)
  Q_PROPERTY(int selectedRow READ selectedRow)

public:
  OdqWellArray(int rows, int cols, 
               QWidget* parent = 0,
               int cellheight = 18, // 24
               int cellwidth = 21); // 28
  ~OdqWellArray() {}
  QString cellContent(int row, int col) const;

  int selectedColumn() const { return selCol; }
  int selectedRow() const { return selRow; }

  virtual void setCurrent(int row, int col);
  virtual void setSelected(int row, int col);

  QSize sizeHint() const Q_DECL_OVERRIDE;

  inline int cellWidth() const
  {
    return cellw;
  }

  inline int cellHeight() const
  {
    return cellh;
  }

  inline int rowAt(int y) const
  {
    return y / cellh;
  }

  inline int columnAt(int x) const
  {
    if (isRightToLeft()) return ncols - (x / cellw) - 1; return x / cellw;
  }

  inline int rowY(int row) const
  {
    return cellh * row;
  }

  inline int columnX(int column) const
  {
    if (isRightToLeft()) return cellw * (ncols - column - 1); return cellw * column;
  }

  inline int numRows() const
  {
    return nrows;
  }

  inline int numCols() const
  {
    return ncols;
  }

  inline QRect cellRect() const
  {
    return QRect(0, 0, cellw, cellh);
  }

  inline QSize gridSize() const
  {
    return QSize(ncols * cellw, nrows * cellh);
  }

  QRect cellGeometry(int row, int column)
  {
    QRect r;
    if (row >= 0 && row < nrows && column >= 0 && column < ncols)
      r.setRect(columnX(column), rowY(row), cellw, cellh);
    return r;
  }

  inline void updateCell(int row, int column) { update(cellGeometry(row, column)); }

signals:
  void selected(int row, int col);
  void currentChanged(int row, int col);

protected:
  virtual void paintCell(QPainter *, int row, int col, const QRect&);
  virtual void paintCellContents(QPainter *, int row, int col, const QRect&);

  void mousePressEvent(QMouseEvent*) Q_DECL_OVERRIDE;
  void mouseReleaseEvent(QMouseEvent*) Q_DECL_OVERRIDE;
  void keyPressEvent(QKeyEvent*) Q_DECL_OVERRIDE;
  void focusInEvent(QFocusEvent*) Q_DECL_OVERRIDE;
  void focusOutEvent(QFocusEvent*) Q_DECL_OVERRIDE;
  void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;

private:
  Q_DISABLE_COPY(OdqWellArray)

  int nrows;
  int ncols;
  int cellw;
  int cellh;
  int curRow;
  int curCol;
  int selRow;
  int selCol;
};

///////////////////////////////////////////////////////////////////////////////
// see also QColorWell in Qt source

typedef int(*Fn_getColorIndex)(int row, int col, int numRow, int numCol);

class OdqColorWell : public OdqWellArray
{
public:
  OdqColorWell(QWidget* parent, int r, int c, ODCOLORREF* vals, 
               Fn_getColorIndex pFnGetIndex = NULL,
               int cellheight = 18, // 24
               int cellwidth = 21); // 28

protected:
  void paintCellContents(QPainter *, int row, int col, const QRect&) Q_DECL_OVERRIDE;
  void mousePressEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
  void mouseMoveEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
  void mouseReleaseEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
//#ifndef QT_NO_DRAGANDDROP
//  void dragEnterEvent(QDragEnterEvent *e) Q_DECL_OVERRIDE;
//  void dragLeaveEvent(QDragLeaveEvent *e) Q_DECL_OVERRIDE;
//  void dragMoveEvent(QDragMoveEvent *e) Q_DECL_OVERRIDE;
//  void dropEvent(QDropEvent *e) Q_DECL_OVERRIDE;
//#endif

private:
  ODCOLORREF* _values;
  Fn_getColorIndex _fnGetIndex;

  bool _mousePressed;
  QPoint _pressPos;
  QPoint _oldCurrent;
};

///////////////////////////////////////////////////////////////////////////////

class OdqColorIndexDialog : public QDialog
{
  Q_OBJECT
public:
  OdqColorIndexDialog(OdRxObject* pRxDb, const OdCmEntityColor& colorInitial,
                      QWidget* pParent = NULL, Qt::WindowFlags flags = 0);
  virtual ~OdqColorIndexDialog();

  void setColorText(const OdCmEntityColor& color);

  QPointer<QLabel> pIndexedColorsLabel;

  QPointer<OdqColorWell> pColors16x16;
  QPointer<OdqColorWell> pColorsAcadHead, pColorsAcadBody, pColorsAcadTail;

  QPointer<QLineEdit> pIndexEdit;
  QPointer<QPushButton> pByLayer;
  QPointer<QPushButton> pByBlock;

private slots:
  void setByIndex16x16(int, int);
  void setByIndexAcadHead(int, int);
  void setByIndexAcadBody(int, int);
  void setByIndexAcadTail(int, int);
  void setByLayer();
  void setByBlock();
protected:
  OdRxObject* m_pRxDb;
  ODGSPALETTE m_palette;
  OdCmEntityColor m_colorInitial;
};

///////////////////////////////////////////////////////////////////////////////

#endif // ODA_QT_COLOR_INDEX_DIALOG_H_
