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
// OdqColorIndexDialog.cpp
//

#include <QtCore>
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtGui>
#else
#include <QtWidgets>
#endif

#include "OdaCommon.h"
#include "DbDatabase.h"
#include "ColorMapping.h"
#include "TDVersion.h"
#include "ExtDbModule.h"

#include "OdqColorIndexDialog.h" 

//////////////////////////////////////////////////////////////////////////

void OdqWellArray::paintEvent(QPaintEvent *e)
{
  QRect r = e->rect();
  int cx = r.x();
  int cy = r.y();
  int ch = r.height();
  int cw = r.width();
  int colfirst = columnAt(cx);
  int collast = columnAt(cx + cw);
  int rowfirst = rowAt(cy);
  int rowlast = rowAt(cy + ch);

  if (isRightToLeft()) {
    int t = colfirst;
    colfirst = collast;
    collast = t;
  }

  QPainter painter(this);
  QPainter *p = &painter;
  QRect rect(0, 0, cellWidth(), cellHeight());


  if (collast < 0 || collast >= ncols)
    collast = ncols - 1;
  if (rowlast < 0 || rowlast >= nrows)
    rowlast = nrows - 1;

  // Go through the rows
  for (int r = rowfirst; r <= rowlast; ++r) {
    // get row position and height
    int rowp = rowY(r);

    // Go through the columns in the row r
    // if we know from where to where, go through [colfirst, collast],
    // else go through all of them
    for (int c = colfirst; c <= collast; ++c) {
      // get position and width of column c
      int colp = columnX(c);
      // Translate painter and draw the cell
      rect.translate(colp, rowp);
      paintCell(p, r, c, rect);
      rect.translate(-colp, -rowp);
    }
  }
}

OdqWellArray::OdqWellArray(int rows, int cols, 
                           QWidget* parent, // = NULL
                           int cellheight, // = 18 // 24
                           int cellwidth) // = 21 // 28
  : QWidget(parent)
  , nrows(rows), ncols(cols)
  , cellh(cellheight)
  , cellw(cellwidth)
{
  setFocusPolicy(Qt::StrongFocus);
  curCol = 0;
  curRow = 0;
  selCol = -1;
  selRow = -1;
}

QSize OdqWellArray::sizeHint() const
{
  ensurePolished();
  return gridSize().boundedTo(QSize(640, 480));
}

void OdqWellArray::paintCell(QPainter* p, int row, int col, const QRect &rect)
{
  int b = 1; //margin // int b = 3; //margin

  const QPalette & g = palette();
  QStyleOptionFrame opt;
  int dfw = style()->pixelMetric(QStyle::PM_DefaultFrameWidth);
  opt.lineWidth = dfw;
  opt.midLineWidth = 1;
  opt.rect = rect.adjusted(b, b, -b, -b);
  opt.palette = g;
  opt.state = QStyle::State_Enabled | QStyle::State_Sunken;
  style()->drawPrimitive(QStyle::PE_Frame, &opt, p, this);
  b += dfw;

  if ((row == curRow) && (col == curCol)) {
    if (hasFocus()) {
      QStyleOptionFocusRect opt;
      opt.palette = g;
      opt.rect = rect;
      opt.state = QStyle::State_None | QStyle::State_KeyboardFocusChange;
      style()->drawPrimitive(QStyle::PE_FrameFocusRect, &opt, p, this);
    }
  }
  paintCellContents(p, row, col, opt.rect.adjusted(dfw, dfw, -dfw, -dfw));
}

//Reimplement this function to change the contents of the well array.

void OdqWellArray::paintCellContents(QPainter *p, int row, int col, const QRect &r)
{
  Q_UNUSED(row);
  Q_UNUSED(col);
  p->fillRect(r, Qt::white);
  p->setPen(Qt::black);
  p->drawLine(r.topLeft(), r.bottomRight());
  p->drawLine(r.topRight(), r.bottomLeft());
}

void OdqWellArray::mousePressEvent(QMouseEvent *e)
{
  // The current cell marker is set to the cell the mouse is pressed in
  QPoint pos = e->pos();
  setCurrent(rowAt(pos.y()), columnAt(pos.x()));
}

void OdqWellArray::mouseReleaseEvent(QMouseEvent * /* event */)
{
  // The current cell marker is set to the cell the mouse is clicked in
  setSelected(curRow, curCol);
}

//Sets the cell currently having the focus. This is not necessarily
//the same as the currently selected cell.

void OdqWellArray::setCurrent(int row, int col)
{
  if ((curRow == row) && (curCol == col))
    return;

  if (row < 0 || col < 0)
    row = col = -1;

  int oldRow = curRow;
  int oldCol = curCol;

  curRow = row;
  curCol = col;

  updateCell(oldRow, oldCol);
  updateCell(curRow, curCol);

  emit currentChanged(curRow, curCol);
}

// Sets the currently selected cell to \a row, \a column. If \a row or
// \a column are less than zero, the current cell is unselected.
//
// Does not set the position of the focus indicator.

void OdqWellArray::setSelected(int row, int col)
{
  int oldRow = selRow;
  int oldCol = selCol;

  if (row < 0 || col < 0)
    row = col = -1;

  selCol = col;
  selRow = row;

  updateCell(oldRow, oldCol);
  updateCell(selRow, selCol);
  if (row >= 0)
    emit selected(row, col);
}

void OdqWellArray::focusInEvent(QFocusEvent*)
{
  updateCell(curRow, curCol);
  emit currentChanged(curRow, curCol);
}

void OdqWellArray::focusOutEvent(QFocusEvent*)
{
  updateCell(curRow, curCol);
}

void OdqWellArray::keyPressEvent(QKeyEvent* e)
{
  switch (e->key()) {                      // Look at the key code
  case Qt::Key_Left:                       // If 'left arrow'-key,
    if (curCol > 0)                        // and cr't not in leftmost col
      setCurrent(curRow, curCol - 1);      // set cr't to next left column
    break;
  case Qt::Key_Right:                      // Correspondingly...
    if (curCol < numCols() - 1)
      setCurrent(curRow, curCol + 1);
    break;
  case Qt::Key_Up:
    if (curRow > 0)
      setCurrent(curRow - 1, curCol);
    break;
  case Qt::Key_Down:
    if (curRow < numRows() - 1)
      setCurrent(curRow + 1, curCol);
    break;
//#if 0
//    // bad idea that shouldn't have been implemented; very counterintuitive
//  case Qt::Key_Return:
//  case Qt::Key_Enter:
//    /*
//    ignore the key, so that the dialog get it, but still select
//    the current row/col
//    */
//    e->ignore();
//    // fallthrough intended
//#endif
  case Qt::Key_Space:
    setSelected(curRow, curCol);
    break;
  default:                                // If not an interesting key,
    e->ignore();                        // we don't accept the event
    return;
  }
}

///////////////////////////////////////////////////////////////////////////////

OdqColorWell::OdqColorWell(QWidget* parent, int r, int c, ODCOLORREF* vals,
                           Fn_getColorIndex fnGetIndex, // = NULL
                           int cellheight, // = 18 // 24
                           int cellwidth) // = 21 // 28
  : OdqWellArray(r, c, parent, cellheight, cellwidth)
  , _values(vals), _fnGetIndex(fnGetIndex)
  , _mousePressed(false), _oldCurrent(-1, -1)
{
  setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum));
}

void OdqColorWell::paintCellContents(QPainter *p, int row, int col, const QRect &r)
{
  int i = row + col * numRows();
  if (_fnGetIndex)
    i = (*_fnGetIndex)(row, col, numRows(), numCols());

  int red = ODGETRED(_values[i]), green = ODGETGREEN(_values[i]), blue = ODGETBLUE(_values[i]);
  QColor color(red, green, blue);
  p->fillRect(r, color);
}

void OdqColorWell::mousePressEvent(QMouseEvent *e)
{
  _oldCurrent = QPoint(selectedRow(), selectedColumn());
  OdqWellArray::mousePressEvent(e);
  _mousePressed = true;
  _pressPos = e->pos();
}

void OdqColorWell::mouseMoveEvent(QMouseEvent *e)
{
  OdqWellArray::mouseMoveEvent(e);
  //#ifndef QT_NO_DRAGANDDROP
  //  if (!_mousePressed)
  //    return;
  //  if ((_pressPos - e->pos()).manhattanLength() > QApplication::startDragDistance()) {
  //    setCurrent(_oldCurrent.x(), _oldCurrent.y());
  //    int i = rowAt(_pressPos.y()) + columnAt(_pressPos.x()) * numRows();
  //    if (_fnGetIndex)
  //      i = (*_fnGetIndex)(rowAt(_pressPos.y()), columnAt(_pressPos.x()), numRows(), numCols());
  //    QColor col(values[i]);
  //    QMimeData *mime = new QMimeData;
  //    mime->setColorData(col);
  //    QPixmap pix(cellWidth(), cellHeight());
  //    pix.fill(col);
  //    QPainter p(&pix);
  //    p.drawRect(0, 0, pix.width() - 1, pix.height() - 1);
  //    p.end();
  //    QDrag *drg = new QDrag(this);
  //    drg->setMimeData(mime);
  //    drg->setPixmap(pix);
  //    _mousePressed = false;
  //    drg->start();
  //  }
  //#endif
}

//#ifndef QT_NO_DRAGANDDROP
//void OdqColorWell::dragEnterEvent(QDragEnterEvent *e)
//{
//  if (qvariant_cast<QColor>(e->mimeData()->colorData()).isValid())
//    e->accept();
//  else
//    e->ignore();
//}
//
//void OdqColorWell::dragLeaveEvent(QDragLeaveEvent *)
//{
//  if (hasFocus())
//    parentWidget()->setFocus();
//}
//
//void OdqColorWell::dragMoveEvent(QDragMoveEvent *e)
//{
//  if (qvariant_cast<QColor>(e->mimeData()->colorData()).isValid()) {
//    setCurrent(rowAt(e->pos().y()), columnAt(e->pos().x()));
//    e->accept();
//  }
//  else {
//    e->ignore();
//  }
//}
//
//void OdqColorWell::dropEvent(QDropEvent *e)
//{
//  QColor col = qvariant_cast<QColor>(e->mimeData()->colorData());
//  if (col.isValid()) {
//    int i = rowAt(e->pos().y()) + columnAt(e->pos().x()) * numRows();
//    values[i] = col.rgb();
//    update();
//    e->accept();
//  }
//  else {
//    e->ignore();
//  }
//}
//
//#endif // QT_NO_DRAGANDDROP

void OdqColorWell::mouseReleaseEvent(QMouseEvent *e)
{
  if (!_mousePressed)
    return;
  OdqWellArray::mouseReleaseEvent(e);
  _mousePressed = false;
}

//////////////////////////////////////////////////////////////////////////

static int fnGetIndex16x16(int row, int col, int numRow, int numCol)
{
  return row * numCol + col;
}

static int fnGetIndexAcad(int row, int col, int numRow, int numCol)
{
  if (numRow == 10) // && row < 9)
    row = (row <= 4) ? (8 - 2 * row) : (1 + 2 * (row - 5));
  return col * numRow + row;
}

OdqColorIndexDialog::OdqColorIndexDialog(OdRxObject* pRxDb, 
                                         const OdCmEntityColor& colorInitial,
                                         QWidget* pParent, // = NULL
                                         Qt::WindowFlags flags) // = 0
  : QDialog(pParent, flags)
  , m_pRxDb(pRxDb)
  , m_colorInitial(colorInitial)
{
  QVBoxLayout* mainLay = new QVBoxLayout(pParent);
  mainLay->setSizeConstraint(QLayout::SetFixedSize);
  setLayout(mainLay);

  QHBoxLayout* topLay = new QHBoxLayout();
  mainLay->addLayout(topLay);
  
  QVBoxLayout* leftLay = new QVBoxLayout;
  topLay->addLayout(leftLay);

  QString qsByLayer = QObject::tr("ByLayer"), qsByBlock = QObject::tr("ByBlock");
  ODCOLORREF* pColors = NULL;
  if (!m_pRxDb)
    m_pRxDb = getIConsole()->getCurrentDatabase();
  OdExtDbModule* pExtDb = getIApp()->getExtDbModule(pRxDb);
  if (pExtDb)
  {
    ODCOLORREF clrBackground;
    if (   !pExtDb->getModelBackground(pRxDb, clrBackground, &m_palette)
        || m_palette.size() != 256)
    {
      ODA_FAIL_ONCE();
      pExtDb = NULL;
    }
    else if (pExtDb->baseModuleName(true) == L"dgn")
      qsByBlock = QObject::tr("ByCell");
  }
  if (!pExtDb)
  {
    ODCOLORREF clrBackground = getIAppProps()->toColorref(L"Options/Display/ModelBackground",
                                                          ODRGB(255, 255, 255));
    const ODCOLORREF* pClrs = odcmAcadPalette(clrBackground); // odcmAcadDarkPalette() or odcmAcadLightPalette();
    m_palette.clear();
    m_palette.insert(m_palette.begin(), pClrs, pClrs + 256);
  }
  ODA_ASSERT_ONCE(m_palette.size() == 256);
  pColors = m_palette.asArrayPtr();

  if (pExtDb)
  {
    //int colorRows = 10, colorColumns = 24;
    int colorRows = 16, colorColumns = 16;
    pColors16x16 = new OdqColorWell(pParent, colorRows, colorColumns, pColors, &fnGetIndex16x16);
    pIndexedColorsLabel = new QLabel(pParent);
    pIndexedColorsLabel->setBuddy(pColors16x16);
    leftLay->addWidget(pIndexedColorsLabel);
    QObject::connect(pColors16x16, SIGNAL(selected(int, int)), SLOT(setByIndex16x16(int, int)));
    leftLay->addWidget(pColors16x16);
  }
  else
  {
    pColorsAcadBody = new OdqColorWell(pParent, 10, 24, &pColors[10], &fnGetIndexAcad);
    QObject::connect(pColorsAcadBody, SIGNAL(selected(int, int)), SLOT(setByIndexAcadBody(int, int)));
    leftLay->addWidget(pColorsAcadBody);

    pColorsAcadHead = new OdqColorWell(pParent, 1, 9, &pColors[1], &fnGetIndexAcad, 24, 28);

    QObject::connect(pColorsAcadHead, SIGNAL(selected(int, int)), SLOT(setByIndexAcadHead(int, int)));
    leftLay->addWidget(pColorsAcadHead);

    pColorsAcadTail = new OdqColorWell(pParent, 1, 6, &pColors[250], &fnGetIndexAcad, 24, 28);
    QObject::connect(pColorsAcadTail, SIGNAL(selected(int, int)), SLOT(setByIndexAcadTail(int, int)));
    leftLay->addWidget(pColorsAcadTail);
  }

  QHBoxLayout* lowLay = new QHBoxLayout();
  mainLay->addLayout(lowLay);

  QLabel* pLabel = new QLabel(QObject::tr("Color:"));
  lowLay->addWidget(pLabel, 0, Qt::AlignLeft);
  pIndexEdit = new QLineEdit(pParent);
  lowLay->addWidget(pIndexEdit, 0, Qt::AlignLeft);
  QSizePolicy policy = pIndexEdit->sizePolicy();
  policy.setHorizontalPolicy(QSizePolicy::Fixed);
  pIndexEdit->setSizePolicy(policy);
  setColorText(m_colorInitial);
  pLabel->setBuddy(pIndexEdit);

  lowLay->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));

  QPushButton* byLayer = new QPushButton(qsByLayer, pParent);
  lowLay->addWidget(byLayer, 0, Qt::AlignRight);
  connect(byLayer, SIGNAL(clicked()), this, SLOT(setByLayer()));
  QPushButton* byBlock = new QPushButton(qsByBlock, pParent);
  lowLay->addWidget(byBlock, 0, Qt::AlignRight);
  connect(byBlock, SIGNAL(clicked()), this, SLOT(setByBlock()));

  mainLay->addItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));

  if (qobject_cast<QTabWidget*>(pParent))
    return; // skip ok & cancel

  QDialogButtonBox* buttons = new QDialogButtonBox(pParent);
  mainLay->addWidget(buttons);

  QPushButton* ok = buttons->addButton(QDialogButtonBox::Ok);
  connect(ok, SIGNAL(clicked()), this, SLOT(accept()));
  ok->setDefault(true);
  QPushButton* cancel = buttons->addButton(QDialogButtonBox::Cancel);
  connect(cancel, SIGNAL(clicked()), this, SLOT(reject()));
}

OdqColorIndexDialog::~OdqColorIndexDialog()
{
}

void OdqColorIndexDialog::setColorText(const OdCmEntityColor& color)
{
  pIndexEdit->setText(OdqPropertyModel::stringByOdColor(color, m_pRxDb));
}

void OdqColorIndexDialog::setByIndex16x16(int row, int col)
{
  int index = fnGetIndex16x16(row, col, 16, 16);
  OdCmEntityColor color; color.setColorIndex(index);
  setColorText(color);
}

void OdqColorIndexDialog::setByIndexAcadHead(int row, int col)
{
  int index = 1 + fnGetIndexAcad(row, col, 1, 9);
  OdCmEntityColor color; color.setColorIndex(index);
  setColorText(color);
}

void OdqColorIndexDialog::setByIndexAcadBody(int row, int col)
{
  int index = 10 + fnGetIndexAcad(row, col, 10, 24);
  OdCmEntityColor color; color.setColorIndex(index);
  setColorText(color);
}

void OdqColorIndexDialog::setByIndexAcadTail(int row, int col)
{
  int index = 250 + fnGetIndexAcad(row, col, 1, 6);
  OdCmEntityColor color; color.setColorIndex(index);
  setColorText(color);
}

void OdqColorIndexDialog::setByLayer()
{
  OdCmEntityColor color(OdCmEntityColor::kByLayer);
  setColorText(color);
}

void OdqColorIndexDialog::setByBlock()
{
  OdCmEntityColor color(OdCmEntityColor::kByBlock);
  setColorText(color);
}

//////////////////////////////////////////////////////////////////////////
