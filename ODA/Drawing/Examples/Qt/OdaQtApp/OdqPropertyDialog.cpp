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
// OdqPropertyDialog.cpp
//

#include "OdaCommon.h"
#include "Gs/Gs.h" // SCALAR_MIN SCALAR_MAX....
#include "CmColor.h"
#include "DbDatabase.h"
#include "DbSymUtl.h"
#include "DbLinetypeTable.h"
#include "DbLinetypeTableRecord.h"
#include "TDVersion.h"
#include "ExtDbModule.h"

#include <QtCore>
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtGui>
#else
#include <QtWidgets>
#endif
#include <QDir>
#include <QQueue>
#include <QSet> 
#include <QBitmap>
#include <QTableWidget>
#include <QTreeWidget>
#include <QHeaderView>
#include <QVBoxLayout>
#include <QLabel>
#include <QTextEdit>
#include <QTreeView>
#include <QPainter>
#include <QMouseEvent>
#include <QSettings>
#include <QTimer>
#include <QTextStream>

#include "OdqPropertyDialog.h" 
#include "OdqPropertyPalette.h" 
#include "OdqColorIndexDialog.h"

//////////////////////////////////////////////////////////////////////////

OdqPropControlSpinBox::OdqPropControlSpinBox(QWidget *widget) : QSpinBox(widget)
{    
}

void OdqPropControlSpinBox::createConnections()
{
  if (!QObject::connect(this, SIGNAL(editingFinished()), this, SLOT(notifyParent()), Qt::QueuedConnection))
  {
    ODA_FAIL_ONCE();
  }
}

QVariant OdqPropControlSpinBox::value()
{
  return value();
}

void OdqPropControlSpinBox::setValue(const QVariant &value)
{
  QSpinBox::setValue(value.toInt());
}

//-----------------------------------------------------------------------------

OdqPropControlSpinBox::~OdqPropControlSpinBox()
{
}

OdqPropControlDoubleSpinBox::OdqPropControlDoubleSpinBox(QWidget *widget) : QDoubleSpinBox(widget)
{
}

void OdqPropControlDoubleSpinBox::createConnections()
{
  if (!QObject::connect(this, SIGNAL(editingFinished()),
                        this, SLOT(notifyParent()), Qt::QueuedConnection))
  {
    ODA_FAIL_ONCE();
  }
}


OdqPropControlDoubleSpinBox::~OdqPropControlDoubleSpinBox()
{
}

//-----------------------------------------------------------------------------

QVariant OdqPropControlDoubleSpinBox::value()
{
  return QDoubleSpinBox::value();
}

void OdqPropControlDoubleSpinBox::setValue(const QVariant &value)
{
  QDoubleSpinBox::setValue(value.toDouble());
}

//-----------------------------------------------------------------------------

OdqPropControlComboBox::OdqPropControlComboBox(QWidget* widget, // = NULL
                                               IqAppProps::TPropType type) // = IqAppProps::prpNone
  : QComboBox(widget)
  , m_type(type)
{    
}

OdqPropControlComboBox::~OdqPropControlComboBox()
{
}

void OdqPropControlComboBox::createConnections()
{
  if (!QObject::connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(notifyParent())))
  {
    ODA_FAIL_ONCE();
  }

  if (lineEdit())
  {
    if (!QObject::connect(lineEdit(), SIGNAL(editingFinished()), this, SLOT(notifyParent())))
    {
      ODA_FAIL_ONCE();
    }
  }    
}

IqAppProps::TPropType OdqPropControlComboBox::type() const
{
  return m_type;
}

QVariant OdqPropControlComboBox::value()
{
  return currentText();
}

void OdqPropControlComboBox::setValue(const QVariant &value)
{
  int idx = findText(value.toString());
  if (idx == -1)
  {
    if (lineEdit())
      lineEdit()->setText(value.toString());
  }
  else
    setCurrentIndex(idx);
}

void OdqPropControlComboBox::fillCombo(const QStringList& qsItems, const QStringList& qsImages)
{
  blockSignals(true); // disconnect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(notifyParent()));
  if (lineEdit())
    lineEdit()->blockSignals(true); // disconnect(lineEdit(), SIGNAL(editingFinished()), this, SLOT(notifyParent()));

  clear();
  QStringList values;
  QStringList icons;
  addItems(qsItems);
  for (int iIcon = 0; iIcon < qsImages.size(); iIcon++)
  {
    if (!qsImages[iIcon].isEmpty())
      setItemIcon(iIcon, QIcon(qsImages[iIcon]));
  }

  blockSignals(false); // connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(notifyParent())); 
  if (lineEdit())
    lineEdit()->blockSignals(false); // connect(lineEdit(), SIGNAL(editingFinished()), this, SLOT(notifyParent()));
}

//-----------------------------------------------------------------------------

static QString aciColor(int index, OdRxObject* pRxDb = NULL)
{
  QString qsColor = QString::number(index);
  if (!pRxDb || OdDbDatabase::cast(pRxDb).get())
  {
    static QStringList lstColors;
    if (lstColors.isEmpty())
      lstColors << QObject::tr("red")
      << QObject::tr("yellow")
      << QObject::tr("green")
      << QObject::tr("cyan")
      << QObject::tr("blue")
      << QObject::tr("magenta")
      << QObject::tr("white");
    if (index >= OdCmEntityColor::kACIRed && index <= OdCmEntityColor::kACIWhite)
      qsColor = lstColors.at(index - OdCmEntityColor::kACIRed);
  }
  return qsColor;
}

OdqPropControlOdColor::OdqPropControlOdColor(const QString& qsColor, QWidget* pParent)
  : QComboBox(pParent)
{
  m_qsColor = qsColor;

  QObject::connect(this, SIGNAL(activated(const QString &)), SLOT(onColorChanged()));
}

OdqPropControlOdColor::~OdqPropControlOdColor()
{
}

QVariant OdqPropControlOdColor::value()
{
  return m_qsColor;
}

void OdqPropControlOdColor::setValue(const QVariant& value)
{
  m_qsColor = value.toString();
  ODA_ASSERT_ONCE(m_qsColor != QObject::tr("Select Color...") && m_qsColor != "-1");
  //addColor(m_qsColor);

  updateSelection();
}

void OdqPropControlOdColor::createConnections()
{
}

void OdqPropControlOdColor::addColor(const QString& qsColor,
                                     OdRxObject* pRxDb) // = NULL
{
  QIcon icon;
  QVariant data = qsColor;
  if (qsColor == QObject::tr("Select Color..."))
  {
    icon = createIcon(m_qsColor, pRxDb);
    data = -1;
  }
  else
    icon = createIcon(qsColor, pRxDb);

  if (findText(qsColor) == -1)
    addItem(icon, qsColor, data);
}

void OdqPropControlOdColor::setVisible(bool visible)
{
  QComboBox::setVisible(visible);
}

// static
QIcon OdqPropControlOdColor::createIcon(const QColor& color)
{
  int nHeight = 14;

  QPixmap pixmap(nHeight, nHeight);

  QPainter painter(&pixmap);
  QColor black(Qt::black);
  painter.fillRect(0, 0, nHeight, nHeight, black);
  painter.fillRect(1, 1, nHeight - 2, nHeight - 2, color);

  QIcon icon(pixmap);
  return icon;
}

// static
QColor OdqPropControlOdColor::toQColor(const class OdCmEntityColor& color,
                                       OdRxObject* pRxDb) // = NULL
{
  if (color.colorMethod() == OdCmEntityColor::kByACI)
  {
    OdUInt8 blue = 0, green = 0, red = 0;
    OdExtDbModule* pExtDb = getExtDbModule(pRxDb);
    if (pExtDb)
    {
      ODCOLORREF clrBackground = ODCOLORREF(0);
      ODGSPALETTE& palette = getIPropSystem()->cacheColors(pRxDb);
      if (   palette.size()
          && (   palette.size() != 256
              || !pExtDb->getModelBackground(pRxDb, clrBackground)
              || clrBackground != palette[255]))
        palette.clear(); // renew

      if (!palette.size())
      {
        if (   !pExtDb->getModelBackground(pRxDb, clrBackground, &palette)
            || palette.size() != 256)
        {
          ODA_FAIL_ONCE(); // test
          palette.clear();
          pExtDb = NULL;
        }
        else
          ODA_ASSERT_ONCE(clrBackground == palette[255]);
      }
      if (pExtDb)
      {
        ODCOLORREF clr = palette[color.colorIndex()];
        red = ODGETRED(clr); green = ODGETGREEN(clr); blue = ODGETBLUE(clr);
      }
    }
    if (!pExtDb)
    {
      OdUInt32 rgb = OdCmEntityColor::lookUpRGB(color.colorIndex());
      blue = (rgb & 0xFF);
      green = (rgb >> 8) & 0xFF;
      red = (rgb >> 16) & 0xFF;
    }
    return QColor(red, green, blue);
  }

  if (color.colorMethod() == OdCmEntityColor::kByColor)
  {
    OdUInt8 red = color.red(), green = color.green(), blue = color.blue();
    QColor qColor(red, green, blue);
    return qColor;                
  }

  if (color.isByLayer() || color.isByBlock())
  {
    // byLayer, byLevel, byBlock, byCell
    IqView* iView = NULL;
    if (pRxDb)
      iView = getIViewSystem()->getView(pRxDb);
    if (iView)
    {
      ODCOLORREF clrBck = iView->backgroundColor(); getIAppProps()->toColorref(OD_T("Options/Display/ModelBackground"), ODRGB(255, 255, 255));
      OdUInt8 red = 255 - ODGETRED(clrBck), green = 255 - ODGETGREEN(clrBck), blue = 255 - ODGETBLUE(clrBck);
      return QColor(red, green, blue);
    }
  }

  return QColor(Qt::white); // TODO
}

void OdqPropControlOdColor::onColorChanged()
{
  int idx = currentIndex();
  QVariant data = itemData(idx);

  if (data == -1) // "Select Color..."
  {
    QPointer<OdqPropControlOdColor> pThis = this;

    OdRxObject* pRxDb = getIConsole()->getCurrentDatabase();
    OdCmEntityColor colorPrev = OdqPropertyModel::OdColorByString(m_qsColor, NULL, pRxDb);
    // restore prev index
    blockSignals(true);
    updateSelection();
    blockSignals(false);
    hidePopup();

    // prevent delete via QEvent::DeferredDelete:
    QWidget* pSvParent = qobject_cast<QWidget*>(parent());
    setParent(NULL);

    OdCmEntityColor color = OdqOdColorDialog::getColor(colorPrev, this);
    if (!pThis.isNull() && color.colorMethod() != OdCmEntityColor::kNone && color != colorPrev)
    {
      OdRxObject* pRxDb = getIConsole()->getCurrentDatabase();
      QString qsColor = OdqPropertyModel::stringByOdColor(color, pRxDb);
      setValue(qsColor); // m_qsColor =
      setItemIcon(count() - 1, createIcon(getQColor(pRxDb)));
      updateSelection();
    }
    if (pThis.isNull())
      return;

    setParent(pSvParent); // restore
  }    
  else
    setValue(data); // m_qsColor = data.toString();

  emit colorChanged();
}

QColor OdqPropControlOdColor::getQColor(OdRxObject* pRxDb) // = NULL
{
  OdCmEntityColor color = OdqPropertyModel::OdColorByString(m_qsColor, NULL, pRxDb);   
  return toQColor(color, pRxDb);
}

// static
QIcon OdqPropControlOdColor::createIcon(const QString& qsColor,
                                        OdRxObject* pRxDb) // = NULL
{
  if (qsColor == QObject::tr("*VARIES*"))
    return QIcon();

  OdCmEntityColor color = OdqPropertyModel::OdColorByString(qsColor, NULL, pRxDb);
  QColor qColor = toQColor(color, pRxDb);
  return createIcon(qColor);
}

void OdqPropControlOdColor::updateSelection()
{
  int idx = findText(m_qsColor);
  if (idx == -1)
    idx = count() - 1;
  setCurrentIndex(idx);
}

//-----------------------------------------------------------------------------

OdqPropControlLineEdit::OdqPropControlLineEdit(QWidget *widget) : QLineEdit(widget)
{    
}

OdqPropControlLineEdit::~OdqPropControlLineEdit()
{
}

void OdqPropControlLineEdit::createConnections()
{
  if (!QObject::connect(this, SIGNAL(editingFinished()), this, SLOT(notifyParent()), Qt::QueuedConnection))
  {
    ODA_FAIL_ONCE();
  }
}

QVariant OdqPropControlLineEdit::value()
{
  return text();
}

void OdqPropControlLineEdit::setValue(const QVariant& value)
{
  setText(value.toString());
}

//-----------------------------------------------------------------------------
OdqPropControlButton::OdqPropControlButton(QWidget* pParent) // = NULL
  : QPushButton(pParent)
{    
}

OdqPropControlButton::OdqPropControlButton(const QString& qsText, 
                                           QWidget* pParent) // = NULL
  : QPushButton(qsText, pParent)
{    
}

OdqPropControlButton::OdqPropControlButton(const QIcon& icon, 
                                           const QString& qsText, // = ""
                                           QWidget* pParent) // = NULL
  : QPushButton(icon, qsText, pParent)
{    
}

OdqPropControlButton::~OdqPropControlButton()
{
}

QVariant OdqPropControlButton::value()
{
  if (!isCheckable())
    return true;
  return isChecked();
}

void OdqPropControlButton::setValue(const QVariant &value)
{
  if (isCheckable())
    setChecked(static_cast<Qt::CheckState>(value.toInt()));
}

void OdqPropControlButton::createConnections()
{
  //if (!QObject::connect(this, SIGNAL(toggled(bool)), this, SLOT(notifyParent()), Qt::QueuedConnection))
  if (!QObject::connect(this, SIGNAL(clicked()), this, SLOT(notifyParent()), Qt::QueuedConnection))
  {
    ODA_FAIL_ONCE();
  }
}

//-----------------------------------------------------------------------------
OdqPropControlRadioButton::OdqPropControlRadioButton(QWidget* pParent) // = NULL
  : QRadioButton(pParent)
{    
}

OdqPropControlRadioButton::OdqPropControlRadioButton(const QString& qsText, 
                                                     QWidget* pParent) // = NULL
  : QRadioButton(qsText, pParent)
{    
}

OdqPropControlRadioButton::~OdqPropControlRadioButton()
{
}

QVariant OdqPropControlRadioButton::value()
{
  if (!isCheckable())
    return true;
  return isChecked();
}

void OdqPropControlRadioButton::setValue(const QVariant &value)
{
  ODA_ASSERT_ONCE(isCheckable());
  bool bVal = value.toBool();
  setChecked(bVal);
}

void OdqPropControlRadioButton::createConnections()
{
  if (!QObject::connect(this, SIGNAL(clicked()), this, SLOT(notifyParent()), Qt::QueuedConnection))
  {
    ODA_FAIL_ONCE();
  }
}

//-----------------------------------------------------------------------------

OdqPropControlCheckBox::OdqPropControlCheckBox(const QString& qsText, QWidget* pParent) 
  : QCheckBox(qsText, pParent)
{    
}

OdqPropControlCheckBox::~OdqPropControlCheckBox()
{
}

QVariant OdqPropControlCheckBox::value()
{
  return checkState();
}

void OdqPropControlCheckBox::setValue(const QVariant &value)
{
  setCheckState(static_cast<Qt::CheckState>(value.toInt()));
}

void OdqPropControlCheckBox::createConnections()
{
  if (!QObject::connect(this, SIGNAL(stateChanged(int)), this, SLOT(notifyParent()), Qt::QueuedConnection))
  {
    ODA_FAIL_ONCE();
  }
}

//-----------------------------------------------------------------------------

OdqPropControlFolder::OdqPropControlFolder(const QString &folder, QWidget* pParent) 
 : QWidget(pParent)
{
  m_pText = new QLineEdit(folder, this);
  m_pButton = new QPushButton("...", this);
}

void OdqPropControlFolder::createConnections()
{
  if (   !QObject::connect(m_pButton, SIGNAL(clicked()), this, SLOT(selectDirectory()), Qt::QueuedConnection)
      || !QObject::connect(m_pText, SIGNAL(editingFinished()), this, SLOT(notifyParent()), Qt::QueuedConnection))
  {
    ODA_FAIL_ONCE();
  }
}

OdqPropControlFolder::~OdqPropControlFolder()
{
}

void OdqPropControlFolder::resizeEvent(QResizeEvent *)
{
  QRect rect(geometry());
  rect.moveLeft(0);
  rect.moveTop(0);
  QRect btnRect(rect);
  rect.setWidth(rect.width() * 4 / 5);
  btnRect.setLeft(rect.right());

  m_pText->setGeometry(rect);
  m_pButton->setGeometry(btnRect);    
}

void OdqPropControlFolder::selectDirectory()
{
  QString dir = QFileDialog::getExistingDirectory(parentWidget(), QString(), m_pText->text());
  if (!dir.isEmpty())
  {
    m_pText->setText(dir);
    notifyParent();
  }
}

QVariant OdqPropControlFolder::value()
{
  return getPath();
}

void OdqPropControlFolder::setValue(const QVariant &value)
{
  m_pText->setText(value.toString());
}

QString OdqPropControlFolder::getPath()
{
  return m_pText->text();
}

//-----------------------------------------------------------------------------

OdqPropControlColorref::OdqPropControlColorref(const QColor& color, QWidget* pParent)
  : QPushButton(pParent)
{
  m_color = color;    
}

void OdqPropControlColorref::createConnections()
{
  if (!QObject::connect(this, SIGNAL(clicked()), this, SLOT(pickColor()), Qt::QueuedConnection))
  {
    ODA_FAIL_ONCE();
  }
}

QVariant OdqPropControlColorref::value()
{
  QColor val = getColor();
  ODCOLORREF colorref = ODRGBA(val.red(), val.green(), val.blue(), val.alpha());
  return (uint)colorref;
}

void OdqPropControlColorref::setValue(const QVariant &value)
{
  m_color = qvariant_cast<QColor>(value);
  repaint();
}

OdqPropControlColorref::~OdqPropControlColorref()
{
}

QColor OdqPropControlColorref::getColor()
{
  return m_color;
}

void OdqPropControlColorref::paintEvent(QPaintEvent *)
{
  QPainter painter(this);
  painter.fillRect(rect(), QBrush(m_color));
}

void OdqPropControlColorref::pickColor()
{
  QColor clr = QColorDialog::getColor(m_color);
  if (clr.isValid())
  {
    m_color = clr;
    notifyParent();
  }
}

//-----------------------------------------------------------------------------

OdqPropControlFileDialog::OdqPropControlFileDialog(QWidget* pParent,
                                                   const QString& cqsFilter, // = "" - all
                                                   bool bForSave, // = false - Open
                                                   const QString& qsAcceptLabel, // = "" - default
                                                   const QString& qsRejectLabel, // = "" - without cancel
                                                   const QString& cqsDefFileName) // = "" 
  : QFileDialog(NULL, //pParent
                QString()) // without title
{
  if (bForSave)
  {
    setAcceptMode(QFileDialog::AcceptSave);
    setFileMode(QFileDialog::AnyFile); 
    setConfirmOverwrite(true);
  }
  else
  {
    setAcceptMode(QFileDialog::AcceptOpen);
    setFileMode(QFileDialog::ExistingFile); 
  }

  QString qsFilter = cqsFilter;
  if (qsFilter.isEmpty())
    qsFilter = "All files (*.*)|*.*||";
  
  QStringList flts = QString(qsFilter)
                        .replace("||", "").split("|", QString::SkipEmptyParts),
                     filters;
  int idx = 0;
  foreach (QString qsFlt, flts)
  {
    if (idx++ % 2)
    {
      ODA_ASSERT_ONCE(flts.at(idx - 1).contains(qsFlt));
      continue;
    }
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
    filters << QObject::tr(qsFlt.toAscii().data());
#else
    filters << QObject::tr(qsFlt.toLatin1().data());
#endif

  }
  ODA_ASSERT(filters.count());
  setNameFilters(filters);

  QSettings settings("ODA", 
                     QFileInfo(QCoreApplication::applicationFilePath()).baseName());
  settings.beginGroup(TD_SHORT_STRING_VER_S "/recent");

  QString qsSelectedFilter = settings.value("filter", filters.first()).toString();
  if (qsSelectedFilter.isEmpty() || !filters.contains(qsSelectedFilter))
    qsSelectedFilter = filters.first();

  selectNameFilter(qsSelectedFilter); // current

  QString qsSelectedExt = getIApp()->getExtensionByFilter(qsSelectedFilter);
  setDefaultSuffix(qsSelectedExt);
  qsSelectedExt = getIApp()->getExtensionByFilter(qsSelectedFilter, "+"); 
  if (qsSelectedExt.isEmpty())
    qsSelectedExt = "all";
  QString qsRecentPath = settings.value(QString("%1/path").arg(qsSelectedExt)).toString(),
          qsLastPath = settings.value("lastpath", getIApp()->getInitialDocumentsPath()).toString();
  if (qsRecentPath.isEmpty())
    qsRecentPath = qsLastPath;

  setDirectory(qsRecentPath);

  if (!cqsDefFileName.isEmpty())
  {
    QString qsDefFileName = cqsDefFileName;
    qsDefFileName.replace("\\", "/");
    if (qsDefFileName.indexOf("/") >= 0)
      setDirectory(QFileInfo(qsDefFileName).path());
    selectFile(qsDefFileName);
  }

  settings.endGroup();

  QList<QPushButton*> lstButtons = findChildren<QPushButton*>();
  foreach(QPushButton* pButton, lstButtons)
  {
    QString qsText = pButton->text(),
            qsName = pButton->objectName();
    if (   !qsAcceptLabel.isEmpty()
        && (qsText.indexOf("Open") >= 0 || qsText.indexOf("Save") >= 0))
      pButton->setText(qsAcceptLabel);
    else if (qsText.indexOf("Cancel") >= 0)
    {
      if (qsRejectLabel.isEmpty())
        pButton->hide();
      else
        pButton->setText(qsRejectLabel);
    }
  }

  //installEventFilter(this);
  setSizeGripEnabled(false);
  if (pParent)
    setParent(pParent);

  // CORE-13352 //https://stackoverflow.com/questions/45986165/adding-qfiledialog-as-a-widget-inside-another-qdialog
  setOption(QFileDialog::DontUseNativeDialog, true);
  setWindowFlags(windowFlags() & ~Qt::Dialog); // otherwise the QFileDialog will always be created as a top level window. 

  if (!connect(this, SIGNAL(accepted()),
               this, SLOT(notifyParent()), Qt::QueuedConnection))
  {
    ODA_FAIL_ONCE();
  }
}

OdqPropControlFileDialog::~OdqPropControlFileDialog()
{
  QString qsFilePathName = value().toString();
  if (qsFilePathName.isEmpty())
    return;

  QSettings settings("ODA", 
                     QFileInfo(QCoreApplication::applicationFilePath()).baseName());
  settings.beginGroup(TD_SHORT_STRING_VER_S "/recent");
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
  settings.setValue("filter", selectedFilter());
#else
  settings.setValue("filter", selectedNameFilter());
#endif

  QFileInfo info(qsFilePathName);
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
  QString qsSelectedExt = getIApp()->getExtensionByFilter(selectedFilter(), "+"); 
#else
  QString qsSelectedExt = getIApp()->getExtensionByFilter(selectedNameFilter(), "+"); 
#endif
  if (qsSelectedExt.isEmpty())
    qsSelectedExt = "all";

  settings.setValue(QString("%1/path").arg(qsSelectedExt), info.path());
  settings.setValue("lastpath", info.path());

  settings.endGroup();
}

void OdqPropControlFileDialog::createConnections()
{
}

QVariant OdqPropControlFileDialog::value()
{
  QDialog::DialogCode code = QDialog::DialogCode(result());
  if (code != QDialog::Accepted)
    return QVariant(); 
  QStringList files = selectedFiles();
  QString qsFilePathName;
  if (files.size() != 1 || (qsFilePathName = files.first()).isEmpty())
  {
    ODA_FAIL_ONCE(); // test
    return QVariant(); 
  }
  return QVariant(qsFilePathName); 
}

void OdqPropControlFileDialog::setValue(const QVariant&) // value
{
  // unused
}

// slot
void OdqPropControlFileDialog::notifyParent()
{
  emit propEdited(this);
  show();
}

//bool OdqPropControlFileDialog::event(QEvent* pEvent)
//{
//  if (  pEvent->type() == QEvent::KeyPress
//      || pEvent->type() == QEvent::KeyRelease)
//  {
//    QKeyEvent* pKeyEvent = (QKeyEvent*) pEvent;
//    if (pKeyEvent->key() == Qt::Key_Escape)
//      return QObject::event(pEvent); // skip
//  }
//  return QFileDialog::event(pEvent);
//}

void OdqPropControlFileDialog::hideEvent(QHideEvent* event)
{
  QWidget* pParent = qobject_cast<QWidget*>(parent());
  if (pParent && pParent->isVisible())
  {
    //show(); return;
    QTimer::singleShot(0, this, SLOT(show()));
  }
  QWidget::hideEvent(event);
}

//-----------------------------------------------------------------------------
// TODO

//OdqPropControlEnabler::OdqPropControlEnabler(const QString &qsTitle,
//                           Qt::CheckState checkState,
//                           TEnablerType enType,
//                           QWidget *pParent) 
//                           : QGroupBox(qsTitle, pParent)
//{
//  setCheckable(enType != enGroup);
//  if (isCheckable())
//    setChecked(checkState == Qt::Checked);
//}
//
//OdqPropControlEnabler::~OdqPropControlEnabler()
//{
//}
//
//QVariant OdqPropControlEnabler::value()
//{
//  return isChecked();
//}
//
//void OdqPropControlEnabler::setValue(const QVariant &value)
//{
//  setChecked(value.toBool());
//}
//
//void OdqPropControlEnabler::createConnections()
//{
//  if (!QObject::connect(this, SIGNAL(toggled(bool)), this, SLOT(notifyParent()), Qt::QueuedConnection))\
//  {
//    ODA_FAIL_ONCE();
//  }
//}
//
//void OdqPropControlEnabler::childEvent(QChildEvent *event)
//{
//  if (event->type() != QEvent::ChildAdded || !event->child()->isWidgetType())
//    QGroupBox::childEvent(event);
//}
//
//void OdqPropControlEnabler::changeEvent(QEvent *event)
//{
//  if(event->type() == QEvent::EnabledChange)
//    QGroupBox::changeEvent(event);
//}

//-----------------------------------------------------------------------------
#if defined(Q_WS_MAC) || defined(__APPLE__)

OdqColorDialog::OdqColorDialog(QWidget* pParent) // = NULL
  : QColorDialog(pParent)
{
  setOption(QColorDialog::DontUseNativeDialog);
  setOption(QColorDialog::ShowAlphaChannel, false);
}

void OdqColorDialog::setVisible(bool visible)
{
  setModal(false);
  QDialog::setVisible(visible);
}

#endif

//-----------------------------------------------------------------------------

OdqOdColorDialog::OdqOdColorDialog(QWidget* pParent) // = NULL
  : QDialog(pParent)
{
}

//#define UNUSE_ODQ_COLOR_INDEX_DIALOG

//static 
OdCmEntityColor OdqOdColorDialog::getColor(const OdCmEntityColor& colorInitial, 
                                           QWidget* pParent, // = NULL
                                           OdRxObject* pRxDb, // = NULL - means current
                                           bool isLayerEnabled, // = true
                                           bool isBlockEnabled) // = true
{
  if (!pRxDb)
    pRxDb = getIConsole()->getCurrentDatabase();
  QColor qcolorInit = OdqPropControlOdColor::toQColor(colorInitial, pRxDb);

  OdqPropertyDialog dlg; // after problem with Mac OS X Snow Leopard //OdqPropertyDialog dlg(pParent);
  dlg.setWindowTitle(QObject::tr("Select Color"));

  QPointer<QTabWidget> pTabWidget = qobject_cast<QTabWidget*>(dlg.addWidget(new QTabWidget(&dlg)));
  pTabWidget->setTabPosition(QTabWidget::North);

//#ifndef UNUSE_ODQ_COLOR_INDEX_DIALOG
  OdqColorIndexDialog* pIndexTab = new OdqColorIndexDialog(pRxDb, colorInitial, pTabWidget);
  pIndexTab->setModal(false);
  pTabWidget->addTab(pIndexTab, QObject::tr("Index Color"));
  if (!isLayerEnabled && pIndexTab->pByLayer.data())
    pIndexTab->pByLayer->setEnabled(false);
  if (!isBlockEnabled && pIndexTab->pByBlock.data())
    pIndexTab->pByBlock->setEnabled(false);
  QPointer<QLineEdit> pIndexEdit = pIndexTab->pIndexEdit;
//#else
//  OdqPropertyDialog* pIndexTab = new OdqPropertyDialog(pTabWidget);
//  pIndexTab->setModal(false);
//  pTabWidget->addTab(pIndexTab, QObject::tr("Index Color"));
//
//  QLabel* pLabel = new QLabel(QObject::tr("Color:\n"));
//  pIndexTab->addWidget(pLabel); //, 0, Qt::AlignTop);
//  QPointer<QLineEdit> pIndexEdit = qobject_cast<QLineEdit*>(pIndexTab->addWidget(new QLineEdit())); 
//                                                          //, 0, Qt::AlignTop));
//  QSizePolicy policy = pIndexEdit->sizePolicy();
//  policy.setHorizontalPolicy(QSizePolicy::Fixed);
//  pIndexEdit->setSizePolicy(policy);
//  pIndexEdit->setText(OdqPropertyModel::stringByOdColor(colorInitial, pRxDb));
//  pLabel->setBuddy(pIndexEdit);
//  pIndexTab->topLayout()->addItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
//#endif

 #if defined(Q_WS_MAC) // defined(__APPLE__)
  // after problem with use the QColorDialog inside another widget on Mac OS X
  QColorDialog* pRgbTab = new OdqColorDialog(pTabWidget);
 #else
  QColorDialog* pRgbTab = new QColorDialog(pTabWidget);
 #endif
  pTabWidget->addTab(pRgbTab, QObject::tr("True Color"));
  pRgbTab->setModal(false);
  pRgbTab->setOption(QColorDialog::NoButtons);
  pRgbTab->setCurrentColor(qcolorInit);

  int indexTab = (   colorInitial.isByACI()
                  || colorInitial.isByLayer() || colorInitial.isByBlock()
                  || pTabWidget->count() < 2) ? 0 : 1;
  pTabWidget->setCurrentIndex(indexTab);

  dlg.setButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

  if (dlg.exec() != QDialog::Accepted)
    return OdCmEntityColor(OdCmEntityColor::kNone);

  if (pTabWidget->currentWidget() == pRgbTab)
  {
    QColor qcolor = pRgbTab->currentColor();
    ODA_ASSERT_ONCE(qcolor.isValid());

    OdCmEntityColor color(OdCmEntityColor::kByColor);
    color.setRGB(qcolor.red(), qcolor.green(), qcolor.blue());

    return color;
  }

  if (pTabWidget->currentWidget() == pIndexTab)
  {
    QString qsColor = pIndexEdit->text();
    bool bOk = false;
    OdCmEntityColor color = OdqPropertyModel::OdColorByString(qsColor, &bOk, pRxDb);
    if (!bOk)
      return OdCmEntityColor(OdCmEntityColor::kNone);
    return color;
  }

  ODA_FAIL_ONCE(); // TODO
  return OdCmEntityColor(OdCmEntityColor::kNone);
}

//-----------------------------------------------------------------------------

OdqPropDelegate::OdqPropDelegate(QObject* pParent) 
  : QItemDelegate(pParent)
{
}

OdqPropDelegate::~OdqPropDelegate()
{
}

bool OdqPropDelegate::editorEvent(QEvent* event, 
                                  QAbstractItemModel* model,
                                  const QStyleOptionViewItem& option, 
                                  const QModelIndex& idx)
{
  QPersistentModelIndex index(idx);

  OdqPropertyModel* pPropModel = static_cast<OdqPropertyModel*>(model);
  if (index.isValid() && event->type() == QEvent::MouseButtonRelease)
  {
    switch (pPropModel->type(index))
    {
    case IqAppProps::prpFolder:
      if (index.column() == pPropModel->columnCount() - 1)
      {
        if (model->flags(index) & Qt::ItemIsEditable)
        {
          QString dir = index.data().toString();
          dir = QFileDialog::getExistingDirectory(NULL, QString(), dir);
          if (!dir.isEmpty())
            model->setData(index, dir, Qt::EditRole);
        }
      }
      break;

    case IqAppProps::prpColorref:
      if (index.column() == pPropModel->columnCount() - 1 && (pPropModel->flags(index) & Qt::ItemIsEditable))
      {
        QColor color = qvariant_cast<QColor>(index.data(Qt::BackgroundColorRole));
        QColor next = QColorDialog::getColor(color);
        if (next.isValid())
        {
          ODCOLORREF colorref = ODRGBA(next.red(), next.green(), next.blue(), next.alpha());
          model->setData(index, (uint)colorref);
        }
      }
      break;

    case IqAppProps::prpBoolPictured:
      if (   index.column() == pPropModel->columnCount() - 1 
          && (pPropModel->flags(index) & Qt::ItemIsEditable))
      {
        bool bValue = (index.data(Qt::UserRole).toBool());
        pPropModel->setData(index, !bValue, Qt::UserRole);
      }
      break;

    } // end switch
  }

  // extracted from Qt 4.1.3 sources
  if ((event->type() != QEvent::MouseButtonRelease) || !(model->flags(index) & Qt::ItemIsUserCheckable))
    return false;

  // check if the event happened in the right place
  QVariant value = index.data(Qt::CheckStateRole);
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
  QRect checkRect = QStyle::alignedRect(option.direction, Qt::AlignLeft | Qt::AlignVCenter,
                                        check(option, option.rect, value).size(),
                                        QRect(option.rect.x() + 3, option.rect.y(),
                                        option.rect.width(), 
                                        option.rect.height()));
#else
  QRect checkRect = option.rect;
#endif
  if (checkRect.contains(((QMouseEvent*) event)->pos()))
  {
    Qt::CheckState state = static_cast<Qt::CheckState>(value.toInt());
    return model->setData(index, (state == Qt::Unchecked ? Qt::Checked : Qt::Unchecked), Qt::CheckStateRole);
  }

  return QItemDelegate::editorEvent(event, model, option, index);
}

QSize OdqPropDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex& index) const
{
  QSize size = QItemDelegate::sizeHint(option, index);
  return size;
}

QWidget *OdqPropDelegate::createEditor(QWidget* pParent, 
                                       const QStyleOptionViewItem&,
                                       const QModelIndex& index) const
{
  //OdqPropertyModel* pModel = qobject_cast<OdqPropertyModel*>(index.model());
  IqAppProps::TPropType type_ = OdqPropertyModel::type(index);

  if (OdqPropertyModel::isReadOnly(index))
    return NULL;

  QWidget* pWidget = NULL;
  if (OdqPropertyModel::isVarious(index))
  {
    if (type_ == IqAppProps::prpOdColor)
    {
      QString qsVaries = QObject::tr("*VARIES*");
      OdqPropControlOdColor* pPicker = new OdqPropControlOdColor(qsVaries, pParent);
      pPicker->setModelIndex(index);
      QObject::connect(pPicker, SIGNAL(colorChanged()), this, SLOT(emitCommitData()));
      return pPicker;
    }

    OdqPropControlComboBox* pCombo = new OdqPropControlComboBox(pParent, type_); //QComboBox* pCombo = new QComboBox(pParent);
    pCombo->setEditable(true);
    
    //switch (type_)
    //{
    //case IqAppProps::prpInt:
    //  {
    //    QIntValidator *pValidator = new QIntValidator(pCombo);
    //    pCombo->lineEdit()->setValidator(pValidator);
    //    break;
    //  }
    //case IqAppProps::prpDouble:
    //  {
    //    QDoubleValidator *pValidator = new QDoubleValidator(pCombo);
    //    pCombo->lineEdit()->setValidator(pValidator);
    //    break;
    //  }
    //} // ens switch

    QObject::connect(pCombo, SIGNAL(activated(int)), this, SLOT(emitCommitData()));
    pWidget = pCombo;
  }
  else
  {
    switch (type_)
    {
    case IqAppProps::prpFolder:        
    case IqAppProps::prpTree:
    case IqAppProps::prpPalette:
    case IqAppProps::prpBoolPictured:
    case IqAppProps::prpColorref:
      return NULL;

    case IqAppProps::prpCombo: case IqAppProps::prpLinetype: case IqAppProps::prpLineweight: case IqAppProps::prpTransparency:
      {
        OdqPropControlComboBox* pCombo = new OdqPropControlComboBox(pParent, type_);
        pCombo->setModelIndex(index);
        QObject::connect(pCombo, SIGNAL(activated(int)), this, SLOT(emitCommitData()));
        pWidget = pCombo;
      }
      break;

    case IqAppProps::prpInt:
      {
        //pWidget = new QSpinBox(pParent);
        int step = OdqPropertyModel::attrToInt(index, "Step", 1);
        step = qAbs(step);
        if (step > 0)
        {
          int min = OdqPropertyModel::attrToInt(index, "Min", INT_MIN),
              max = OdqPropertyModel::attrToInt(index, "Max", INT_MAX);
          OdqPropControlSpinBox *spinBox = new OdqPropControlSpinBox(pParent);
          spinBox->setSingleStep(step);
          int val = index.data().toInt();
          spinBox->setValue(val);
          ODA_ASSERT_ONCE(max > min && val >= min && val <= max);
          if (max > min && val >= min && val <= max)
          {
            if (min != INT_MAX)
              spinBox->setMinimum(min);
            if (max != INT_MIN)
              spinBox->setMaximum(max);
          }
          pWidget = spinBox;
        }
        break;
      }

    case IqAppProps::prpDouble:
    case IqAppProps::prpDistance:
    case IqAppProps::prpAngle:
    {
        //pWidget = new QDoubleSpinBox(pParent);
        double step = OdqPropertyModel::attrToDouble(index, "Step", 0); // default as string
        step = qAbs(step);
        if (step > 0.0)
        {
          double min = OdqPropertyModel::attrToDouble(index, "Min", - 1.0e+100),
                 max = OdqPropertyModel::attrToDouble(index, "Max", 1.0e+100);
          OdqPropControlDoubleSpinBox *doubleSpinBox = new OdqPropControlDoubleSpinBox(pParent);
          doubleSpinBox->setSingleStep(step);
          double val = index.data().toDouble();
          doubleSpinBox->setValue(val);
          ODA_ASSERT_ONCE(max > min && val > min && val < max);
          if (max > min && val > min && val < max)
          {
            if (min != INT_MAX)
              doubleSpinBox->setMinimum(min);
            if (max != INT_MIN)
              doubleSpinBox->setMaximum(max);
          }
          pWidget = doubleSpinBox;
        }
        break;
      }

    case IqAppProps::prpOdColor:
      pWidget = new OdqPropControlOdColor(index.data(Qt::UserRole).toString(), pParent);
      qobject_cast<OdqPropControlOdColor*>(pWidget)->setModelIndex(index);
      QObject::connect(pWidget, SIGNAL(colorChanged()), this, SLOT(emitCommitData()));
      break;

    //default: //case IqAppProps::prpString:
    //  pWidget = new QLineEdit(pParent);
    //  break;
    }
    
    if (!pWidget)
      pWidget = new QLineEdit(pParent);
  }
  if (pWidget)
    pWidget->installEventFilter(const_cast<OdqPropDelegate*>(this));

  return pWidget;
}

void OdqPropDelegate::setEditorData(QWidget* pEditor, const QModelIndex& index) const
{
  //const OdqPropertyModel* pModel = qobject_cast<OdqPropertyModel*>(index.model());

  QLineEdit* pLineEdit = qobject_cast<QLineEdit*>(pEditor);
  if (pLineEdit)
  {
    pLineEdit->setText(index.data(Qt::DisplayRole).toString());
    return;
  }

  QTextEdit* pTextEdit = qobject_cast<QTextEdit *>(pEditor);
  if (pTextEdit)
  {
    pTextEdit->setPlainText(index.data(Qt::DisplayRole).toString());
    return;
  }

  OdqPropControlOdColor* pPicker = qobject_cast<OdqPropControlOdColor *>(pEditor);
  if (pPicker)
  {
    //if (OdqPropertyModel::isVarious(index))
    {
      QStringList values;
      QStringList icons;

      if (   (OdqPropertyModel::getPropertyPossibleValues(index, values, icons) && values.size())
          || OdqPropertyModel::getPossibleOdColors(values))
      {
        ODA_ASSERT_ONCE(!icons.size());

        QString qsColor = pPicker->value().toString();
        OdRxObject* pRxDb = getIConsole()->getCurrentDatabase();
        if (qsColor == QObject::tr("*VARIES*"))
          pPicker->addColor(qsColor, pRxDb);
        for (int iItem = 0; iItem < values.size(); iItem++)
        {
          QString qsColor_ = values[iItem];
          if (qsColor_ == QObject::tr("Select Color..."))
            pPicker->addColor(qsColor, pRxDb);
          pPicker->addColor(qsColor_, pRxDb);
        }
        pPicker->addColor(qsColor, pRxDb);
        pPicker->updateSelection();
        
      #if defined(Q_OS_WIN32) // !defined(__APPLE__) && !defined(Q_OS_LINUX)
        //pPicker->view()->setWindowFlags(pPicker->view()->windowFlags() & ~Qt::Popup);
        if (!pPicker->isVisible())
          pPicker->showPopup();
      #endif
      }
    }
    return;
  }

  OdqPropControlComboBox* pCombo = qobject_cast<OdqPropControlComboBox*>(pEditor);
  if (pCombo)
  {
    QStringList values;
    QStringList icons;
    if (   (OdqPropertyModel::getPropertyPossibleValues(index, values, icons) && values.size())
        || (   pCombo->type() == IqAppProps::prpLinetype 
            && OdqPropertyModel::getPossibleLineTypes(getIConsole()->getCurrentDatabase(), values, icons))
        || (   pCombo->type() == IqAppProps::prpLineweight
            && OdqPropertyModel::getPossibleLineWeights(values, icons))
        || (   pCombo->type() == IqAppProps::prpTransparency
            && OdqPropertyModel::getPossibleTransparencies(values)))
    {
      pCombo->addItems(values);
      for (int iIcon = 0; iIcon < icons.size(); iIcon++)
      {
        QString qsImagePath = icons[iIcon];
        if (!qsImagePath.isEmpty())
        {           
          QPixmap pixmap(qsImagePath);
          pixmap.setMask(pixmap.createHeuristicMask());
          pCombo->setItemIcon(iIcon, pixmap);
        }
      }
    }
    pCombo->setCurrentIndex(pCombo->findText(index.data().toString()));

  #if defined(Q_OS_WIN32) // !defined(__APPLE__) && !defined(Q_OS_LINUX)
    //pCombo->view()->setWindowFlags(pCombo->view()->windowFlags() & ~Qt::Popup);
    if (!pCombo->isVisible())
      pCombo->showPopup();
  #endif

    //??? through initEventFilter
    //pCombo->show();
    //qApp->processEvents(); // show it
    //pCombo->setFocusPolicy(Qt::StrongFocus);
    //pCombo->hidePopup();
    //pCombo->showPopup();
    //qApp->processEvents(); // show it
    //QTimer::singleShot(500, pCombo, SLOT(show()));
    return;
  }

  QSpinBox* pSpin = qobject_cast<QSpinBox *>(pEditor);
  if (pSpin)
  {
    pSpin->setMinimum(SCALAR_MIN);
    pSpin->setMaximum(SCALAR_MAX);
    pSpin->setValue(index.data(Qt::DisplayRole).toInt());
    return;
  }

  QDoubleSpinBox* pDoubleSpin = qobject_cast<QDoubleSpinBox *>(pEditor);
  if (pDoubleSpin)
  {
    pDoubleSpin->setMaximum(3.402823466e+38); // FLT_MAX
    pDoubleSpin->setMinimum(1.175494351e-38); // FLT_MIN
    pDoubleSpin->setValue(index.data(Qt::DisplayRole).toDouble());
    return;
  }
}

void OdqPropDelegate::setModelData(QWidget* pEditor, QAbstractItemModel* pModel, const QModelIndex& index) const
{ 
  QPointer<QLineEdit> pLineEdit;
  QPointer<OdqPropControlOdColor> pPicker;
  QPointer<QComboBox> pCombo;
  QPointer<QSpinBox> pSpin;
  QPointer<QDoubleSpinBox> pDoubleSpin;

  QString qsValue;
  QVariant vValue;
  ODA_ASSERT_ONCE(!vValue.isValid());

  if (!(pLineEdit = qobject_cast<QLineEdit*>(pEditor)).isNull())
    vValue = qsValue = pLineEdit->text();
  else if (!(pPicker = qobject_cast<OdqPropControlOdColor*>(pEditor)).isNull())
    vValue = qsValue = pPicker->value().toString();
  else if (!(pCombo = qobject_cast<QComboBox*>(pEditor)).isNull()) 
    vValue = qsValue = pCombo->currentText();
  else if (!(pSpin = qobject_cast<QSpinBox*>(pEditor)).isNull())
  {
    pSpin->interpretText();
    vValue = pSpin->value();
  }
  else if (!(pDoubleSpin = qobject_cast<QDoubleSpinBox*>(pEditor)).isNull())
  {
    pDoubleSpin->interpretText();
    vValue = pDoubleSpin->value();
  }

  if (vValue.isValid())
  {
    //if (OdqPropertyModel::isPaletteItem(index))
    //  pModel = pModel; // brk
    pEditor->close(); // closeEditor(pEditor);
    if (pModel)
      pModel->setData(index, vValue);
  }
  //else
  //  pModel = pModel; // brk
}

void OdqPropDelegate::updateEditorGeometry(QWidget* pEditor, 
                                           const QStyleOptionViewItem& option, 
                                           const QModelIndex& index) const
{
  pEditor->setGeometry(option.rect);
  if (!OdqPropertyModel::isPaletteItem(index))
    return;
  QComboBox* pCombo = qobject_cast<QComboBox*>(pEditor);
  if (pCombo && !pCombo->isVisible())
      pCombo->showPopup();
}

void OdqPropDelegate::paint(QPainter* pPainter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
  QItemDelegate::paint(pPainter, option, index);
}

void OdqPropDelegate::emitCommitData()
{
  QWidget* pWidget = qobject_cast<QWidget*>(sender());
  ODA_ASSERT_ONCE(pWidget);

  IqPropControl* iPropControl = qobject_cast<IqPropControl*>(pWidget);
  ODA_ASSERT_ONCE(iPropControl);
  if (!iPropControl)
    return;

  QModelIndex index = iPropControl->modelIndex();
  const OdqPropertyModel* pModel = qobject_cast<const OdqPropertyModel*>(index.model());

  QPointer<OdqPropControlOdColor> pPicker = qobject_cast<OdqPropControlOdColor *>(pWidget);
  if (!pPicker.isNull())
    setModelData(pWidget, const_cast<QAbstractItemModel *>(index.model()), // (pPicker->modelIndex().model()), 
                          pPicker->modelIndex());
  else
  {
    QPointer<OdqPropControlComboBox> pCombo = qobject_cast<OdqPropControlComboBox*>(pWidget);
    if (!pCombo.isNull() && (!pModel || !qobject_cast<OdqPropPaletteView*>(pModel->widget())))
      setModelData(pWidget, const_cast<QAbstractItemModel *>(index.model()), // (pCombo->modelIndex().model()), 
                            pCombo->modelIndex());
  }

  emit commitData(pWidget);
  //emit 
  closeEditor(pWidget);
}

//////////////////////////////////////////////////////////////////////////

class OdqGridProxyItem
{
  friend class OdqGridProxyModel;
public:
  OdqGridProxyItem(OdqGridProxyItem* pParent, const QModelIndex& index) 
    : m_pParent(pParent)
    , m_pRow(NULL)
  {
    m_index = index;
  }

  ~OdqGridProxyItem()
  {
    qDeleteAll(m_children);
    qDeleteAll(m_columns);
  }

  int rowCount() const
  {
    return m_children.size();
  }

  OdqGridProxyItem *child(int index)
  {
    if (index < 0 || index >= m_children.size())
      return NULL;
    return m_children[index];
  }

  int row()
  {
    if (m_pParent == NULL)
      return 0;
    return m_pParent->m_children.indexOf(this);
  }

  QString name()
  {
    QModelIndex idxName = m_index.sibling(m_index.row(), 0);
    QString qsName = idxName.data().toString();
    ODA_ASSERT_ONCE(!qsName.isEmpty());
    return qsName;
  }

  OdqGridProxyItem* column(int index)
  {
    if (index < 0 || index >= m_columns.size())
    {
      ODA_FAIL_ONCE();
      return NULL;
    }   
    return m_columns[index];
  }

  OdqGridProxyItem* parent()
  {
    return m_pParent;
  }

  OdqGridProxyItem* rowItem()
  {
    return m_pRow;
  }

private:
  OdqGridProxyItem* m_pParent;
  OdqGridProxyItem* m_pRow;
  QVector<OdqGridProxyItem*> m_children;
  QPersistentModelIndex m_index;
  QVector<OdqGridProxyItem*> m_columns;
};

//////////////////////////////////////////////////////////////////////////

OdqGridProxyModel::OdqGridProxyModel(QObject* pParent, OdqPropertyModel* pModel) 
  : QAbstractItemModel(pParent)
{
  m_pModel = pModel;
  m_pRoot = NULL;

  //QObject::connect(pModel, SIGNAL(rowsInserted(const QModelIndex&, int, int)), 
  //  this, SLOT(sourceRowsInserted(const QModelIndex&, int, int)));

  //QObject::connect(pModel, SIGNAL(rowsInserted(const QModelIndex&, int, int)),
  //  this, SLOT(sourceRowsRemoved(const QModelIndex&, int, int)));

  QObject::connect(pModel, SIGNAL(layoutChanged()), this, SLOT(sourceLayoutChanged()));
}

OdqGridProxyModel::~OdqGridProxyModel()
{
  m_pRoot = NULL; // delete m_pRoot;
}

void OdqGridProxyModel::setRootIndex(const QPersistentModelIndex& idxRoot)
{
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
  beginResetModel();
#endif
  m_idxRoot = idxRoot;

  m_namesColumn.clear();
  collectColumnNames(idxRoot, m_namesColumn);

  if (m_pRoot)
    m_pRoot = NULL; // delete m_pRoot;
  m_pRoot = createProxyIndex(NULL, idxRoot);
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
  emit reset(); // important to clear correctly after update
#else
  endResetModel();
#endif
}

bool OdqGridProxyModel::insertRows(int row, int count, const QModelIndex& parent)
{
  beginInsertRows(parent, row, row + count);
  QModelIndex srcParent = mapToSource(parent);
  endInsertRows();
  return true;
}

QModelIndex OdqGridProxyModel::mapToSource(const QModelIndex& proxyIndex) const
{
  OdqGridProxyItem* pItem = static_cast<OdqGridProxyItem*>(proxyIndex.internalPointer());
  if (pItem)
    return pItem->m_index;
  return QModelIndex();
}

QModelIndex OdqGridProxyModel::mapFromSource(const QModelIndex& sourceIndex) const
{
  if (sourceIndex == m_idxRoot)
    return QModelIndex();

  QModelIndex parent = mapFromSource(sourceIndex.parent());
  return index(sourceIndex.row(), sourceIndex.column(), parent);
}

void OdqGridProxyModel::collectColumnNames(const QModelIndex& index, QStringList& namesColumn)
{
  int rowCount = index.model()->rowCount(index);
  for (int idx = 0; idx < rowCount; idx++)
  {
    QModelIndex indexChild = index.child(idx, 0);
    if (!index.model()->hasChildren(indexChild))
    {
      QString qsColumnName = indexChild.data().toString();
      ODA_ASSERT_ONCE(!qsColumnName.isEmpty());
      // must be column in grid
      if (!namesColumn.contains(qsColumnName))
        namesColumn.append(qsColumnName);
    }
    else
      collectColumnNames(indexChild, namesColumn);
  }    
}

QVariant OdqGridProxyModel::headerData(int section, Qt::Orientation, int role) const
{
  if (section >= m_namesColumn.size() || role != Qt::DisplayRole)
    return QVariant();

  QString qsName = m_namesColumn[section];
  ODA_ASSERT_ONCE(!qsName.isEmpty());
  return qsName;
}

QVariant OdqGridProxyModel::data(const QModelIndex& index, int role) const
{
  QModelIndex indexSrc = mapToSource(index);
  return indexSrc.data(role);
}

QModelIndex OdqGridProxyModel::index(int row, int column, const QModelIndex& parent) const
{
  OdqGridProxyItem* pItem = (OdqGridProxyItem*) parent.internalPointer();
  if (!pItem)
    pItem = m_pRoot;
  else
    pItem = pItem->rowItem();

  OdqGridProxyItem* pChild = pItem->child(row);
  if (pChild)
  {
    QString qsColumnName = m_namesColumn[column];
    for (int iColumn = 0; iColumn < pChild->m_columns.size(); iColumn++)
    {
      OdqGridProxyItem* pColumn = pChild->column(iColumn);
      if (pColumn->name() == qsColumnName)
        return createIndex(row, column, pColumn);
    }
  }

  // grid is empty - valid for some filters // ODA_FAIL_ONCE();
  return QModelIndex();
}

int OdqGridProxyModel::columnCount(const QModelIndex&) const
{
  return m_namesColumn.size();
}

int OdqGridProxyModel::rowCount(const QModelIndex& parent) const
{
  void* pPtr = parent.internalPointer();    
  OdqGridProxyItem* pItem = static_cast<OdqGridProxyItem*>(pPtr);
  if (!pItem)
    pItem = m_pRoot;
  else
    pItem = pItem->rowItem();

  return pItem->rowCount();
}

QModelIndex OdqGridProxyModel::parent(const QModelIndex& index) const
{
  OdqGridProxyItem* pItem = (OdqGridProxyItem*) index.internalPointer();
  if (pItem)
  {
    OdqGridProxyItem* pParent = pItem->parent();
    if (pParent == m_pRoot)
      return QModelIndex();
    return createIndex(pParent->row(), 0, pParent->column(0));
  }
  return QModelIndex();
}

// TODO
//void OdqGridProxyModel::sourceRowsInserted(const QModelIndex&, int, int)
//{
//}
//void OdqGridProxyModel::sourceRowsRemoved(const QModelIndex&, int, int)
//{
//}
//void OdqGridProxyModel::sourceDataChanged(const QModelIndex&)
//{
//}

Qt::ItemFlags OdqGridProxyModel::flags(const QModelIndex& index) const
{
  QModelIndex srcIndex = mapToSource(index);
  if (srcIndex != QModelIndex())
    return srcIndex.model()->flags(srcIndex);

  return Qt::ItemFlags();
}

OdqGridProxyItem* OdqGridProxyModel::createProxyIndex(OdqGridProxyItem* pParent, const QModelIndex& index)
{
  const OdqPropertyModel* pModel = qobject_cast<const OdqPropertyModel*>(index.model());

  OdqGridProxyItem* pItem = new OdqGridProxyItem(pParent, index);
  if (pParent)
    pParent->m_children.append(pItem);

  int childCount = index.model()->rowCount(index);
  for (int row = 0; row < childCount; row++)
  {
    QModelIndex indexChild = index.child(row, 0);
    IqAppProps::TPropType tp = pModel->type(indexChild);

    if (tp == IqAppProps::prpTree)
    {
      ODA_FAIL_ONCE(); // test
      continue;
    }
    if (tp == IqAppProps::prpPalette)
    {
      ODA_FAIL_ONCE(); // test
      continue;
    }

    QString qsColumnName = indexChild.data().toString();
    if (!pModel->hasChildren(indexChild))
    {
      // create column            
      OdqGridProxyItem* pColumn = new OdqGridProxyItem(pParent, indexChild.sibling(row, 1)); // TODO : remove constant
      pColumn->m_pRow = pItem;
      pItem->m_columns.append(pColumn);
    }
    else
    {
      OdqGridProxyItem* pChild = createProxyIndex(pItem, indexChild);
      if (!pChild)
        continue;

      if (pChild->m_columns.size() != m_namesColumn.size())
      {
        for (int iColumn = 0; iColumn < pChild->m_columns.size(); iColumn++)
        {
          OdqGridProxyItem* pColumn = pChild->column(iColumn);
          pColumn->m_pParent = pParent;
          pColumn->m_pRow = pItem;
          pItem->m_columns.append(pColumn);
        }
        pChild->m_columns.clear();
        pItem->m_children.pop_back();
        delete pChild;
      }
    }
  }

  return pItem;
}

// slot
void OdqGridProxyModel::sourceLayoutChanged()
{
  setRootIndex(m_idxRoot); 
}

// slot
void OdqGridProxyModel::setCurrentIndex(const QModelIndex& indexProxy)
{
  emit notifyGridSourceChanged(mapToSource(indexProxy));
}

// slot
void OdqGridProxyModel::currentRowChanged(const QModelIndex& indexProxy)
{
  emit notifyGridRowChanged(mapToSource(indexProxy));
}

//////////////////////////////////////////////////////////////////////////

OdqGridProxyDelegate::OdqGridProxyDelegate(OdqGridProxyModel* pProxyModel)
{
  m_pProxyModel = pProxyModel;
  m_pDelegate = new OdqPropDelegate(this);
}

OdqGridProxyDelegate::~OdqGridProxyDelegate()
{
}

QWidget *OdqGridProxyDelegate::createEditor(QWidget* parent, 
                                            const QStyleOptionViewItem& option, 
                                            const QModelIndex& index) const
{
  QModelIndex srcIndex = m_pProxyModel->mapToSource(index);
  return m_pDelegate->createEditor(parent, option, srcIndex);
}

bool OdqGridProxyDelegate::editorEvent(QEvent* event, 
                                       QAbstractItemModel*, 
                                       const QStyleOptionViewItem& option, 
                                       const QModelIndex& index)
{
  QModelIndex srcIndex = m_pProxyModel->mapToSource(index);
  QAbstractItemModel* pModel = const_cast<QAbstractItemModel*>(srcIndex.model());
  return m_pDelegate->editorEvent(event, pModel, option, srcIndex);
}

void OdqGridProxyDelegate::updateEditorGeometry(QWidget* pWidget, 
                                                const QStyleOptionViewItem& item, 
                                                const QModelIndex& index) const
{
  QModelIndex srcIndex = m_pProxyModel->mapToSource(index);
  m_pDelegate->updateEditorGeometry(pWidget, item, srcIndex);
}

void OdqGridProxyDelegate::setEditorData(QWidget* pWidget, const QModelIndex& index) const
{
  QModelIndex srcIndex = m_pProxyModel->mapToSource(index);
  return m_pDelegate->setEditorData(pWidget, srcIndex);
}

void OdqGridProxyDelegate::setModelData(QWidget* pWidget, QAbstractItemModel*, const QModelIndex& index) const
{
  QModelIndex srcIndex = m_pProxyModel->mapToSource(index);
  QAbstractItemModel* pModel = const_cast<QAbstractItemModel*>(srcIndex.model());
  return m_pDelegate->setModelData(pWidget, pModel, srcIndex);
}

//////////////////////////////////////////////////////////////////////////

class OdqPropertyItem 
{
public:
  OdqPropertyItem(const QString& qsPath, OdqPropertyItem* pParent, OdqPropertyModel* pModel);
  virtual ~OdqPropertyItem();

  //void copyChildrenFrom(OdqPropertyItem* pFrom, OdqPropertyModel* pModel);

  OdqPropertyItem* parent()
  {
    return m_pParent;
  }

  bool isProperty() const
  {
    return countChildren() == 0;
  }

  bool isTreeItem() const
  {
    for (const OdqPropertyItem* pItem = this; pItem; pItem = pItem->m_pParent)
    {
      if (pItem->type() == IqAppProps::prpTree)
        return true;
    }
    return false;
  }

  bool isPaletteItem(int* pLevel = NULL) const
  {
    if (pLevel)
      *pLevel = 0;
    for (const OdqPropertyItem* pItem = this; pItem; pItem = pItem->m_pParent)
    {
      if (pItem->type() == IqAppProps::prpPalette)
        return true;
      if (pLevel)
        *pLevel += 1;
    }
    return false;
  }

  bool isVarious() const
  {
    return m_bVarious;
  }

  void setVarious(bool bVarious)
  {
    m_bVarious = bVarious;
  }

  IqAppProps::TPropType type() const
  {
    ODA_ASSERT_ONCE(  m_type != IqAppProps::prpNone 
                    || m_qsPath.isEmpty() // undefined for root 
                    || m_lstChildren.size()); // or autogenerated parent for grid row
    return m_type;
  }

  void setType(IqAppProps::TPropType type)
  {
    m_type = type;
  }

  void setType(const QString &qsType)
  {
    m_type = OdqXmlUtils::toType(qsType);
  }

  QString imagePath() const
  {
    ODA_ASSERT_ONCE(m_pImage && m_pImage->type() == IqAppProps::prpImage);
    if (!m_pImage)
      return "";
    QString qsImage = m_pImage->value(1);
    ODA_ASSERT_ONCE(!qsImage.isEmpty());

    return qsImage;
  }

  void setImageItem(OdqPropertyItem* pItem)
  {
    ODA_ASSERT_ONCE(!pItem || (pItem->type() == IqAppProps::prpImage));
    if (m_pImage)
    {
      m_pModel->removeRow(m_pImage);
      m_lstChildren.removeAll(m_pImage);            
    }
    m_pImage = pItem;
  }

  //void appendChild(OdqPropertyItem* item)
  //{
  //  m_lstChildren.push_back(item);
  //}

  void insertChild(uint at, OdqPropertyItem* pItem)
  {
    m_lstChildren.insert(at, pItem);
  }

  int row() const
  {
    if (!m_pParent)
      return 0;
    return m_pParent->m_lstChildren.indexOf(const_cast<OdqPropertyItem*>(this));
  }

  OdqPropertyItem* child(int idx) const // row
  {
    //ODA_ASSERT_ONCE(!idx || idx < countChildren());
    if (idx >= countChildren())
      return 0;
    return m_lstChildren.at(idx);
  }

  OdSharedPtr<OdqPropertyItem> takeChild(int idx)
  {
    if (idx < 0 || idx >= countChildren())
      return OdSharedPtr<OdqPropertyItem>();

    return OdSharedPtr<OdqPropertyItem>(m_lstChildren.takeAt(idx));
  }

  QString value(int column)
  {
    if (column >= m_value.size())
      return QString();
    QString qsValue = m_value[column];
    return qsValue;
  }

  void setValue(const QString& qsName, const QString& cqsValue)
  {
    QString qsValue = getIApp()->decodeFromXmlAttrib(cqsValue); // CORE-13627
    m_value.clear();
    m_value << qsName << qsValue;
  }

  void showImage(bool bShow)
  {
    if (bShow && !imageShown())
      m_lstChildren.push_back(m_pImage);
    else if (imageShown())
      m_lstChildren.removeAt(m_lstChildren.indexOf(m_pImage));
  }

  bool imageShown()
  {
    ODA_ASSERT_ONCE(m_pImage); // test
    if (!m_pImage)
      return false;
    return m_lstChildren.indexOf(m_pImage) != -1;
  }

  QString decoration(int column)
  {
    if (column >= m_lstDecorations.size())
      return QString();
    QString qsFilePath = m_lstDecorations[column];
    int pos = qsFilePath.lastIndexOf(":");
    if (pos > 0)
    {
      ODA_ASSERT_ONCE(   m_type == IqAppProps::prpBool
                      || m_type == IqAppProps::prpBoolPictured
                      || m_type == IqAppProps::prpInt);
      // extract by value
      QString qsValue = value(column);
      ODA_ASSERT_ONCE(!qsValue.isEmpty());
      qsValue = QString(":%1=").arg(qsValue);
      pos = qsFilePath.indexOf(qsValue);
      if (pos >= 0)
      {
        qsFilePath = qsFilePath.mid(pos + qsValue.length());
        pos = qsFilePath.indexOf(":", 1);
        if (pos > 0)
          qsFilePath = qsFilePath.left(pos);
      }
      else
      {
        ODA_FAIL_ONCE(); // TODO
        qsFilePath = "";
      }
    }
    return qsFilePath;
  }

  void setDecoration(int column, const QString& qsValue)
  {
    while (column >= m_lstDecorations.size())
      m_lstDecorations.push_back(QString());
    m_lstDecorations[column] = qsValue;
  }

  int indentation() const
  {
    return m_indent;
  }

  const QString& path() const
  {
    return m_qsPath;
  }

  OdqPropertyItem* findChild(const QString& qsChildPath) const
  {
    foreach(OdqPropertyItem* pChild, m_lstChildren)
    {
      if (pChild->path() == qsChildPath)
        return pChild;
    }

    return NULL;
  }

  int countChildren() const
  {
    return m_lstChildren.size();
  }

  bool isWithImage() const
  {
    ODA_ASSERT_ONCE(   !m_pImage 
                    || (   m_pImage->type() == IqAppProps::prpImage
                        && m_pImage->m_pParent == this));
    return m_pImage != NULL;
  }

  bool isEnabled()
  {
    return m_bEnabled; // && getEnablerState();
  }

  bool isReadOnly()
  {
    return !isEnabled() || m_bReadOnly; // && getEnablerState();
  }

  //bool getEnablerState()
  //{
  //  bool bEnablerState = true;
  //  if (m_pParent != NULL)
  //  {
  //    bEnablerState = m_pParent->getEnablerState();
  //    if (m_pParent->m_enablerType != enNone && m_pParent->m_enablerType != enGroup)
  //    {
  //      bool bDirect = m_pParent->m_enablerType == enDirect;
  //      bool bValue = qvariant_cast<bool>(m_pParent->value(COLUMN_COUNT - 1));
  //      bEnablerState = bEnablerState & (bDirect == bValue);
  //    }
  //  }
  //  return bEnablerState;
  //}

  void setEnabled(bool bEnabled) 
  {
    m_bEnabled = bEnabled;
  }

  void setReadOnly(bool bValue) 
  {
    m_bReadOnly = bValue;
  }

  const QString& comment() const
  {
    return m_qsComment;
  }

  void setComment(const QString& qsValue)
  {
    m_qsComment = qsValue;
  }

  //TEnablerType enablerType() const
  //{
  //  return m_enablerType;
  //}

  //void setEnablerType(TEnablerType enType)
  //{
  //  m_enablerType = enType;
  //}

  void setInlineWithParent(bool bValue)
  {
    m_bInlineWithParent = bValue;
  }

  bool isInlineWithParent() const
  {
    return m_bInlineWithParent;
  }

  // used for current item in tree
  void setSelected(bool bValue)
  {
    m_isSelected = bValue;
  }

  bool isSelected() const
  {
    return m_isSelected;
  }

  // except standard
  QString attr(const QString& qsName, 
               const QString& qsDefValue = "", 
               bool* pbFound = NULL) const;

  void updateFromXml(QDomElement elemProp);

protected:
  QString m_qsPath;
  IqAppProps::TPropType m_type;
  QStringList m_value;

  OdqPropertyModel* m_pModel;
  OdqPropertyItem* m_pParent;
  QList<OdqPropertyItem*> m_lstChildren;

  bool m_bEnabled;
  bool m_bReadOnly; // is not gray differ to m_bEnabled

  QString m_qsComment; // may be used for tooltips
  QStringList m_lstDecorations;
  OdqPropertyItem* m_pImage;
  int m_indent;
  bool m_bInlineWithParent;
  //TEnablerType m_enablerType;
  bool m_bVarious; // TODO for multiple selection
  bool m_isSelected;
  QString m_qsExAttibs; // other attributes in form ";key=value;...;"

private:
  Q_DISABLE_COPY(OdqPropertyItem)
};

OdqPropertyItem::OdqPropertyItem(const QString& qsPath, OdqPropertyItem* pParent, OdqPropertyModel* pModel) 
  : m_qsPath(qsPath)
  , m_type(IqAppProps::prpNone)
  , m_pModel(pModel)
  , m_pParent(pParent)
  , m_bEnabled(true)
  , m_bReadOnly(false)
  , m_indent(pParent ? (pParent->indentation() + 1) : -1)
  , m_pImage(NULL)
  , m_bInlineWithParent(false)
  //m_enablerType
  , m_bVarious(false)
  , m_isSelected(false)
{
}

OdqPropertyItem::~OdqPropertyItem()
{
  if (m_pImage && !imageShown())
    delete m_pImage;

  qDeleteAll(m_lstChildren);
}

void OdqPropertyItem::updateFromXml(QDomElement elemProp)
{
#ifdef QT_DEBUG
  QString qsPathName = elemProp.attribute("Path");
  if (!qsPathName.isEmpty())
    qsPathName += "/";
  qsPathName += elemProp.attribute("Name");
#endif

  setType(elemProp.attribute("Type"));

  QString qsVisibleName = elemProp.attribute("VisibleName", elemProp.attribute("Name"));
  //ODA_ASSERT_ONCE(!qsVisibleName.isEmpty());
  QString qsValue = elemProp.attribute("Value");
  setValue(qsVisibleName, qsValue);
  setVarious(qsValue == QObject::tr("*VARIES*"));
  // TODO
  //setVarious(QVariant(elemProp.attribute("Various", "false")).toBool());
  //if (isVarious())
  //  setValue(qsVisibleName, QObject::tr("*VARIES*"));
  //setValue(qsVisibleName, QObject::tr("*VARIES*"));

  setComment(elemProp.attribute("Comment"));
  setEnabled(QVariant(elemProp.attribute("Enabled", "true")).toBool());
  setReadOnly(QVariant(elemProp.attribute("ReadOnly", "false")).toBool());

  //if (elemProp.hasAttribute("EnablerType"))
  //{
  //  QString qsEnablerType = elemProp.attribute("EnablerType");
  //  if (qsEnablerType == "Direct")
  //    setEnablerType(enDirect);
  //  if (qsEnablerType == "Reverse")
  //    setEnablerType(enReverse);
  //  if (qsEnablerType == "Group")
  //    setEnablerType(enGroup);
  //}
  //else
  //  setEnablerType(enNone);

  QString qsImage = elemProp.attribute("Image");
  int level = 0;
  if (type() == IqAppProps::prpCategory && isPaletteItem(&level) && level == 1)
  {
    if (qsImage.isEmpty())
      setImageItem(NULL);        
    else
    {
      OdqPropertyItem* pImageItem = new OdqPropertyItem(path() + "_image", this, m_pModel);
      pImageItem->setType(IqAppProps::prpImage);
      pImageItem->setValue("", qsImage);
      //pImageItem->setEnablerType(enNone);
      setImageItem(pImageItem);
      showImage(true);
    }
  }
  else if (!qsImage.isEmpty())
  {
    if (isProperty() || isTreeItem())
      setDecoration(1, qsImage);
    else
    {
      ODA_FAIL_ONCE(); // TODO
    }
  }

  setSelected(QVariant(elemProp.attribute("IsCurrent", "false")).toBool());

  setInlineWithParent(qvariant_cast<bool>(elemProp.attribute("InlineWithParent")));

  // pack all other attribs to m_qsExAttibs in form ";key=value;...;"
  static QStringList s_lstAttribsToSkipInEx;
  if (s_lstAttribsToSkipInEx.isEmpty())
  {
    s_lstAttribsToSkipInEx << "path" << "name" << "various" << "type" << "visiblename" 
                           << "value" << "comment" << "enabled" << "readonly" << "image" 
                           << "iscurrent" << "inlinewithparent";
  }
  QDomNamedNodeMap attrs = elemProp.attributes();
  int cnt = attrs.count();
  for (int index = 0; index < cnt; index++)
  {
    QDomNode nodeAttr = attrs.item(index);
    QString qsName = nodeAttr.nodeName().toLower(),
            qsValue = nodeAttr.nodeValue();
    qsName.replace(";", "");
    qsName.replace("=", "");
    qsValue.replace(";", "");
    if (qsName.isEmpty() || qsValue.isEmpty() || s_lstAttribsToSkipInEx.contains(qsName))
      continue;

    if (m_qsExAttibs.isEmpty())
      m_qsExAttibs += ";";
    m_qsExAttibs += qsName;
    m_qsExAttibs += "=";
    m_qsExAttibs += qsValue;
    m_qsExAttibs += ";";
  }
}

QString OdqPropertyItem::attr(const QString& qsName, 
                              const QString& qsDefValue, // = ""
                              bool* pbFound) const // = NULL
{
  if (pbFound)
    *pbFound = false;

  // all attribs (except standard now) are stored in to m_qsExAttibs in form ";key=value;...;"

  if (m_qsExAttibs.isEmpty())
    return qsDefValue;

  QString qsKey = ";";
  qsKey += qsName.toLower();
  qsKey += "=";
  qsKey.replace(";", "");
  
  int pos = m_qsExAttibs.indexOf(qsKey);
  if (pos < 0)
    return qsDefValue;
  pos += qsKey.length();
  if (pbFound)
    *pbFound = true;
  
  int len = m_qsExAttibs.indexOf(";", pos);
  if (pos >= 0)
    len -= pos;
  QString qsValue = m_qsExAttibs.mid(pos, len);
  return qsValue;
}

//////////////////////////////////////////////////////////////////////////

OdqPropertyModel::OdqPropertyModel(const QString& qsRootPath) // = ""
 : m_pRoot(new OdqPropertyItem(qsRootPath, NULL, this)) // m_pRoot(new OdqPropertyItem("", NULL, this))
 , m_pPropertySource(NULL)
 , m_pFun_updateXml(NULL)
 , m_pFun_setPropertyValue(NULL)
 , m_pFun_getPropertyPossibleValues(NULL)
 , m_pRxDb(NULL)
{
  m_pRxDb = getIConsole()->getCurrentDatabase();
}

OdqPropertyModel::~OdqPropertyModel()
{
  OdqPropPaletteView* pView = qobject_cast<OdqPropPaletteView*>(m_pWidget);
  if (pView && !m_mapExpanded.isEmpty())
    pView->saveSettings();
}

//static 
OdSharedPtr<OdqPropertyModel> OdqPropertyModel::createModel(const QString& qsBasePath, // = ""
                                                            const QString& qsRootNodeName) // = "Properties"
{
  OdSharedPtr<OdqPropertyModel> pModel = new OdqPropertyModel(qsBasePath);
  //if (!qsXml.isEmpty())
  //{
  //  if (!pModel->loadXmlFromString(qsXml, qsBasePath, qsRootNodeName))
  //  {
  //    ODA_FAIL_ONCE();
  //    pModel = NULL;
  //    return pModel;
  //  }
  //  pModel->updateFromXml();
  //}
  return pModel;
}

bool OdqPropertyModel::isLoaded()
{
  return m_utils.isLoaded();
}

bool OdqPropertyModel::loadXmlFromString(const QString& qsXml,
                                         const QString& qsBasePath, // = ""
                                         const QString& qsRootNodeName) // = "Properties"
{
  ODA_ASSERT_ONCE(!qsXml.isEmpty());

  QString qsErr = m_utils.loadXmlFromString(qsXml, qsBasePath, qsRootNodeName);
  if (!qsErr.isEmpty())
  {
    //ODA_FAIL_ONCE();
    getIConsole()->putString(qsErr);

#ifdef QT_DEBUG
    QString qsPath = QDir::toNativeSeparators(QDir::homePath() + "/ODA/prop");
    if (QDir(qsPath).exists())
    {
      qsPath = QDir::toNativeSeparators(qsPath + "/layers.xml");
      QFile file(qsPath);
      if (file.open(QFile::WriteOnly | QFile::Text)) 
      {
        QTextStream stream(&file);
        stream << qsXml;
        //stream.flush();
        return false;
      }
      else
      {
        ODA_FAIL_ONCE();
      }
    }
#endif

    return false;
  }

 #ifdef QT_DEBUG
  QString qsPath = QDir::toNativeSeparators(QDir::homePath() + "/ODA/prop");
  if (QDir(qsPath).exists() && !qobject_cast<OdqPropPaletteView*>(m_pWidget))
  {
    qsPath = QDir::toNativeSeparators(qsPath + "/prop_model.xml");
    m_utils.saveXmlFile(qsPath);
  }
 #endif

  return true;
}

bool OdqPropertyModel::updateFromXml(const QString& qsXml, // = ""
                                     const QString& qsBasePath, // = ""
                                     const QString& qsRootNodeName, // = "Properties"
                                     const QString& qsUnmergedBasePath, // = "" without merging
                                     const QString& qsMergeFilter, // = "" all
                                     const QString& qsMergeFilterAttribName) // = "Filter"
{
  if (   !qsXml.isEmpty()
      && !loadXmlFromString(qsXml, qsBasePath, qsRootNodeName))
  {
    ODA_ASSERT_ONCE(!qsXml.isEmpty());
    return false;
  }
  setMergeFilter(qsMergeFilter, qsMergeFilterAttribName, qsUnmergedBasePath);
  return true;
}

bool OdqPropertyModel::setMergeFilter(const QString& qsMergeFilter, // = ""  all
                                      const QString& qsMergeFilterAttribName, // = "Filter"
                                      const QString& qsUnmergedBasePath) // = "" without merging
{
  ODA_ASSERT_ONCE(isLoaded());
  if (!isLoaded())
    return false;
  m_utils.setMergeFilter(qsMergeFilter, qsMergeFilterAttribName, qsUnmergedBasePath);
  QString qsBasePath = m_utils.getBasePath();

  bool bShowHidden = getIAppProps()->toBool(OD_T("Vars/showhiddenprops"), false);
  QSet<QString> qsetPathNames;

  // mark existed
  QStringList lstKeys = m_utils.keys();
  foreach (QString qsPathName, lstKeys)
  {
    QDomElement elemProp = m_utils.getElement(qsPathName);

    ODA_ASSERT_ONCE(!qsBasePath.isEmpty() || !qsPathName.indexOf(qsBasePath));
    bool bHidden = qvariant_cast<bool>(elemProp.attribute("Hidden", "false")),
         bSkip = qvariant_cast<bool>(elemProp.attribute("Skip", "false"));
    if (bHidden && bShowHidden)
      bHidden = false;
    if (bSkip)
      bHidden = true;
    if (bHidden)
      continue;

    bool bSibling = qvariant_cast<bool>(elemProp.attribute("InlineWithParent"));
    m_mapInlineWithParent[qsPathName] = bSibling;
    qsetPathNames.insert(qsPathName);
    // include autogenerated parents of grid columns in cache
    int pos = qsPathName.lastIndexOf("/");
    if (pos > 0)
    {
      qsPathName = qsPathName.left(pos);
      if (!qsetPathNames.contains(qsPathName))
        qsetPathNames.insert(qsPathName);
    }
  }
  qsetPathNames.insert(QString());

  m_mapItems.clear();
  m_mapLastChildName.clear();

  // remove not presented
  // fill cache
  QQueue<OdqPropertyItem*> items;    
  for (items.append(m_pRoot); !items.isEmpty(); items.pop_front())
  {
    OdqPropertyItem* pItem = items.front();

    QString qsPath = pItem->path();
    m_mapItems[qsPath] = pItem;

    if (!qsetPathNames.contains(qsPath))
    {
      if (qsPath.length() > qsBasePath.length())
        removeRow(pItem);
      else
      {
        ODA_ASSERT_ONCE(!qsBasePath.indexOf(qsPath));
      }
      continue;
    }        

    for (int idx = 0; idx < pItem->countChildren(); idx++)
    {
      OdqPropertyItem* pKid = pItem->child(idx);
      if (pKid->type() != IqAppProps::prpImage)
        items.append(pKid);
    }
  }
  
  // add absent in required order
  foreach (QString qsPathName, lstKeys)
  {
    QDomElement elemProp = m_utils.getElement(qsPathName);

    bool bHidden = qvariant_cast<bool>(elemProp.attribute("Hidden", "false")),
         bSkip = qvariant_cast<bool>(elemProp.attribute("Skip", "false"));
    if (bHidden && bShowHidden)
      bHidden = false;
    if (bSkip)
      bHidden = true;
    if (bHidden)
      continue;

    OdqPropertyItem* pItem = createItem(qsPathName);
    pItem->updateFromXml(elemProp);
    if (   pItem == m_pRoot.get()
        && qobject_cast<OdqPropPaletteView*>(m_pWidget)
        && pItem->type() == IqAppProps::prpCategory)
    {
      pItem->setType(IqAppProps::prpPalette); // fix for correct isPaletteItem() value
      ODA_ASSERT_ONCE(pItem->isPaletteItem());
    }
  }

  //? setRootIndex(m_idxRoot); //emit layoutChanged();
  return true;
}

OdqPropertyItem * OdqPropertyModel::createItem(const QString& qsPathName, 
                                               bool bMarkAsLastForParent) // = true
{
  if (qsPathName.isEmpty() || qsPathName == m_pRoot->path())
    return m_pRoot;

  int pos = qsPathName.lastIndexOf('/');
  QString qsPath = (pos >= 0) ? qsPathName.left(pos) : QString();

  OdqPropertyItem* pParent = createItem(qsPath, false);

  bool bSibling = m_mapInlineWithParent[qsPathName];
  if (bSibling)
  {
    pParent = pParent->parent();
    while (pParent->isInlineWithParent())
      pParent = pParent->parent();
  }

  if (m_mapItems.contains(qsPathName))
  {
    if (bMarkAsLastForParent)
      m_mapLastChildName[pParent->path()] = qsPathName;

    return m_mapItems[qsPathName];
  }

  OdqPropertyItem* pItem = new OdqPropertyItem(qsPathName, pParent, this);
  QModelIndex index = createIndex(pParent->row(), 0, pParent);  
  QString qsLastProcessed = m_mapLastChildName[pParent->path()];
  OdqPropertyItem* pPrev = pParent->findChild(qsLastProcessed);
  int row = pPrev ? pPrev->row() + 1 : 0;

  insertRow(row, index, pItem);

  m_mapItems[qsPathName] = pItem;
  m_mapLastChildName[pParent->path()] = qsPathName;   

  return pItem;
}

//const QModelIndex& OdqPropertyModel::activeIndex()
//{
//  return m_indexCurrent;
//}

QModelIndex OdqPropertyModel::modelIndex(const QString& qsPathName)
{
  OdqPropertyItem* pItem = m_mapItems[qsPathName];
  //ODA_ASSERT_ONCE(pItem);
  if (!pItem)
    return QModelIndex();

  return createIndex(pItem->row(), 0, pItem);
}

QModelIndex OdqPropertyModel::rootIndex() const // base path
{
  ODA_ASSERT_ONCE(!m_pRoot.isNull());
  if (m_pRoot.isNull())
    return QModelIndex();

  return createIndex(m_pRoot->row(), 0, const_cast<OdqPropertyItem*>(m_pRoot.get()));
}

// slot
void OdqPropertyModel::currentChanged(const QModelIndex& index)
{
  m_indexCurrent = index;
}

// slot
void OdqPropertyModel::propEdited(QWidget* pPropControl)
{
  IqPropControl* iPropControl = qobject_cast<IqPropControl*>(pPropControl);
  QModelIndex index = iPropControl->modelIndex();
  IqAppProps::TPropType type_ = type(index);
  if (   type_ == IqAppProps::prpBool
      || type_ == IqAppProps::prpRadio)
    setData(index, iPropControl->value(), Qt::CheckStateRole);
  else
    setData(index, iPropControl->value());
}

// slot
void OdqPropertyModel::expanded(const QModelIndex& index)
{
  QString qsPath = path(index);
  m_mapExpanded[qsPath] = true;
}

// slot
void OdqPropertyModel::collapsed(const QModelIndex& index)
{
  QString qsPath = path(index);
  m_mapExpanded[qsPath] = false;
}

QString OdqPropertyModel::path(const QModelIndex &index) const
{
  const OdqPropertyItem* pItem = static_cast<OdqPropertyItem*>(index.internalPointer());
  if (!pItem)
    pItem = m_pRoot;
  ODA_ASSERT(pItem);

  return pItem->path();
}

//static 
IqAppProps::TPropType OdqPropertyModel::type(const QModelIndex& index)
{
  OdqPropertyItem* pItem = static_cast<OdqPropertyItem*>(index.internalPointer());
  if (!pItem)
  {
    ODA_FAIL_ONCE();
    return IqAppProps::prpString;
  }

  return pItem->type();
}

//static 
bool OdqPropertyModel::isEnabled(const QModelIndex& index)
{
  if (!index.isValid())
    return false;

  OdqPropertyItem* pItem = static_cast<OdqPropertyItem*>(index.internalPointer());
  if (!pItem)
  {
    ODA_FAIL_ONCE();
    return false;
  }
  return pItem->isEnabled();
}

//static 
bool OdqPropertyModel::isReadOnly(const QModelIndex& index)
{
  if (!index.isValid())
    return true;

  OdqPropertyItem* pItem = static_cast<OdqPropertyItem*>(index.internalPointer());
  if (!pItem)
  {
    ODA_FAIL_ONCE();
    return true;
  }
  return pItem->isReadOnly();
}

//static 
bool OdqPropertyModel::isExpanded(const QModelIndex& index)
{
  const OdqPropertyModel* pModel = qobject_cast<const OdqPropertyModel*>(index.model());
  ODA_ASSERT_ONCE(pModel);
  if (!pModel)
    return false; // is not tree item

  QString qsPath = pModel->path(index);
  bool bValue = false;
  QHash<QString, bool>::const_iterator itr = pModel->m_mapExpanded.find(qsPath);
  if (itr != pModel->m_mapExpanded.end())
    bValue = itr.value();
  return bValue;
}

//static 
int OdqPropertyModel::indentation(const QModelIndex& index)
{
  const OdqPropertyItem* pItem = static_cast<OdqPropertyItem*>(index.internalPointer());
  ODA_ASSERT_ONCE(pItem);
  if (!pItem)
    return -1;

  const OdqPropertyModel* pModel = qobject_cast<const OdqPropertyModel*>(index.model());
  ODA_ASSERT_ONCE(pModel);
  if (!pModel)
    return -1;
  QModelIndex indexRoot = pModel->rootIndex();
  if (index == pModel->rootIndex())
    return -1;

  int indent = pItem->indentation();
  const OdqPropertyItem* pRootItem = static_cast<OdqPropertyItem*>(indexRoot.internalPointer());
  ODA_ASSERT_ONCE(pRootItem);
  if (pRootItem)
    indent -= pRootItem->indentation() + 1;

  return indent;
}

//static 
bool OdqPropertyModel::isWithImage(const QModelIndex& index,
                                   QString* pqsImagePath) // = NULL
{
  if (pqsImagePath)
    pqsImagePath->clear();

  const OdqPropertyItem* pItem = static_cast<OdqPropertyItem*>(index.internalPointer());
  if (!pItem)
  {
    ODA_FAIL_ONCE();
    return false;
  }

  bool bRes = pItem->isWithImage();
  if (bRes && pqsImagePath)
    *pqsImagePath = pItem->imagePath();

  return bRes;
}

//static 
int OdqPropertyModel::countChildren(const QModelIndex& index)
{
  const OdqPropertyItem* pItem = static_cast<OdqPropertyItem*>(index.internalPointer());
  //ODA_ASSERT_ONCE(pItem);
  if (!pItem)
    return 0;

  return pItem->countChildren();
}

//static 
bool OdqPropertyModel::isProperty(const QModelIndex& index)
{
  return !countChildren(index);
}

//static 
bool OdqPropertyModel::isTreeItem(const QModelIndex& index)
{
  const OdqPropertyItem* pItem = static_cast<OdqPropertyItem*>(index.internalPointer());
  if (!pItem)
  {
    ODA_FAIL_ONCE();
    return false;
  }

  return pItem->isSelected(); // ? return pItem->isTreeItem(); 
}

//static
bool OdqPropertyModel::isPaletteItem(const QModelIndex& index)
{
  const OdqPropertyItem* pItem = static_cast<OdqPropertyItem*>(index.internalPointer());
  if (!pItem)
  {
    ODA_FAIL_ONCE();
    return false;
  }

  return pItem->isPaletteItem();
}

//static 
bool OdqPropertyModel::isVarious(const QModelIndex& index)
{
  const OdqPropertyItem* pItem = static_cast<OdqPropertyItem*>(index.internalPointer());
  if (!pItem)
  {
    ODA_FAIL_ONCE();
    return 0;
  }

  return pItem->isVarious();
}

//static 
bool OdqPropertyModel::isSelected(const QModelIndex& index)
{
  const OdqPropertyItem* pItem = static_cast<OdqPropertyItem*>(index.internalPointer());
  if (!pItem)
  {
    ODA_FAIL_ONCE();
    return false;
  }

  return pItem->isSelected();
}

// static
QString OdqPropertyModel::attr(const QModelIndex& index, const QString& qsName, 
                               const QString& qsDefValue, // = ""
                               bool* pbFound) // = NULL
{
  const OdqPropertyItem* pItem = static_cast<OdqPropertyItem*>(index.internalPointer());
  if (!pItem)
  {
    ODA_FAIL_ONCE();
    if (pbFound)
      *pbFound = false;
    return qsDefValue;
  }

  return pItem->attr(qsName, qsDefValue, pbFound);
}

// static
int OdqPropertyModel::attrToInt(const QModelIndex& index, const QString& qsName, 
                                int nDefValue) // = 0
{
  bool bFound = false;
  QString qsValue = attr(index, qsName, "", &bFound);
  if (!bFound || qsValue.isEmpty())
    return nDefValue;

  bool bOk = false;
  int nValue = qsValue.toInt(&bOk);
  ODA_ASSERT_ONCE(bOk);
  if (!bOk)
    return nDefValue;

  return nValue;
}

//static 
double OdqPropertyModel::attrToDouble(const QModelIndex& index, const QString& qsName, 
                                      double dDefValue) //  = 0.0
{
  bool bFound = false;
  QString qsValue = attr(index, qsName, "", &bFound);
  if (!bFound || qsValue.isEmpty())
    return dDefValue;

  bool bOk = false;
  double dValue = qsValue.toDouble(&bOk);
  ODA_ASSERT_ONCE(bOk);
  if (!bOk)
    return dDefValue;

  return dValue;
}

//static 
bool OdqPropertyModel::getPropertyPossibleValues(const QModelIndex& index, 
                                                 QStringList& values, 
                                                 QStringList& icons)
{
  values.clear();
  icons.clear();

  const OdqPropertyModel* pModel = qobject_cast<const OdqPropertyModel*>(index.model());
  const OdqPropertyItem* pItem = static_cast<OdqPropertyItem*>(index.internalPointer());
  if (!pModel || !pItem)
  {
    ODA_FAIL_ONCE();
    return false;
  }

  const QString& qsPath = pItem->path();
  ODA_ASSERT_ONCE(!qsPath.isEmpty());

  QString qsValues = pItem->attr("Values");
  if (!qsValues.isEmpty())
  {
    // was added via merging through addWithLeadingSeparator
    QString qsSep = qsValues.left(1);
    qsValues = qsValues.mid(1);
    values  = qsValues.split(qsSep); // , QString::SkipEmptyParts) - maybe an empty
    ODA_ASSERT_ONCE(values.size());

    if (isVarious(index))
      values.prepend(QObject::tr("*VARIES*"));

    QStringList lstVal, lstIcn;
    if (   pModel->m_pFun_getPropertyPossibleValues
        && (*pModel->m_pFun_getPropertyPossibleValues)(pModel->m_pPropertySource, 
                                                       qsPath, lstVal, lstIcn)
        && lstIcn.size())
    {
      // TODO // add icons (value is key of temp map ...)
      ODA_FAIL_ONCE();
    }

    return true;
  }

  if (!pModel->m_pFun_getPropertyPossibleValues)
  {
    ODA_FAIL_ONCE();
    return false;
  }

  return (*pModel->m_pFun_getPropertyPossibleValues)(pModel->m_pPropertySource, qsPath, values, icons);
}

QModelIndex OdqPropertyModel::index(int row, int column, 
                                    const QModelIndex& indexParent) const // = QModelIndex()
{
  const OdqPropertyItem* pItem = m_pRoot;
  if (indexParent.isValid())
    pItem = static_cast<OdqPropertyItem*>(indexParent.internalPointer());

  ODA_ASSERT_ONCE(pItem); // test
  if (!pItem)
    return QModelIndex();

  OdqPropertyItem* pChild = pItem->child(row);
  if (!pChild)
    return QModelIndex();

  return createIndex(row, column, static_cast<void*>(pChild));
}

QModelIndex OdqPropertyModel::parent(const QModelIndex& index) const
{
  //OdqPropPaletteView* pView = qobject_cast<OdqPropPaletteView*>(m_pWidget);
  //ODA_ASSERT_ONCE(index.isValid()); // || !pView); // test
  if (!index.isValid())
  {
    ODA_FAIL_ONCE();
    return rootIndex(); //return QModelIndex();
  }

  OdqPropertyItem* pChild = static_cast<OdqPropertyItem*>(index.internalPointer());
  ODA_ASSERT_ONCE(pChild); // || !pView); // test
  if (!pChild)
    return rootIndex(); //return QModelIndex(); 
  if (pChild == m_pRoot.get())
    return QModelIndex(); 

  OdqPropertyItem* pParent = pChild->parent();
  ODA_ASSERT_ONCE(pParent); // || !pView); // test
  if (!pParent)
    return rootIndex(); //return QModelIndex(); 

  // problem pView->resizeColumnToContents(0) via 
  // NULL item parent (instead root) with Qt SDK 4.6.3
  //ODA_ASSERT_ONCE(pParent != m_pRoot) // || !pView) // test
  //if (!pView && pParent == m_pRoot)
  //  return QModelIndex(); // TODO return rootIndex(); 
  return createIndex(pParent->row(), 0, pParent);
}

int OdqPropertyModel::rowCount(const QModelIndex& indexParent) const // = QModelIndex()
{
  // TODO return OdqPropertyModel::countChildren(indexParent);
  const OdqPropertyItem* pItem = static_cast<OdqPropertyItem*>(indexParent.internalPointer());
  if (!pItem)
  {
    pItem = m_pRoot;
    if (!pItem)
      return 0;
    //ODA_FAIL_ONCE(); // test
  }

  return pItem->countChildren();
}

int OdqPropertyModel::columnCount(const QModelIndex& indexParent) const // = QModelIndex()
{
  return 2; // name & value
}

QVariant OdqPropertyModel::data(const QModelIndex& index, 
                                int role) const // = Qt::DisplayRole
{
  OdqPropertyItem* pItem = static_cast<OdqPropertyItem *>(index.internalPointer());
  if (!pItem)
  {
    ODA_FAIL_ONCE(); // test
    return QVariant();
  }

  IqAppProps::TPropType type_ = OdqPropertyModel::type(index);

  switch (role)
  {    
  case Qt::DecorationRole: // render data as decoration (else as icon)
    if (pItem->isTreeItem())
    {        
      QString qsImagePath = pItem->decoration(index.column() + 1);
      return qsImagePath;
    }

    if (pItem->isVarious() && index.column() == columnCount(index) - 1)
      return QVariant();

    if (type_ == IqAppProps::prpImage)
      return pItem->value(columnCount(index) - 1);

    if (type_ == IqAppProps::prpOdColor && index.column() == columnCount(index) - 1)
      return OdqPropControlOdColor::createIcon(pItem->value(index.column()), m_pRxDb);
    // TODO
    //if (type_ == IqAppProps::prpLinetype && index.column() == columnCount(index) - 1)
    //  return ...::createIcon(pItem->value(index.column()));
    //if (type_ == IqAppProps::prpLineweight && index.column() == columnCount(index) - 1)
    //  return ...::createIcon(pItem->value(index.column()));

    if (pItem->isProperty())
    {        
      QString qsImagePath = pItem->decoration(index.column());
      if (!qsImagePath.isEmpty())
      {
        QImage img(qsImagePath);
        QPixmap pixmap = QPixmap::fromImage(img);
        pixmap.setMask(pixmap.createHeuristicMask());
        return pixmap.toImage();
      }
    }
    return QVariant();

    // data to be rendered (usually text)
  case Qt::DisplayRole:
    if (   (   type_ == IqAppProps::prpBool
            || type_ == IqAppProps::prpBoolNamed
            || type_ == IqAppProps::prpRadio
            || type_ == IqAppProps::prpColorref)
        && index.column() == columnCount(index) - 1)
      return QVariant();
    if (type_ == IqAppProps::prpBoolPictured && index.column() == columnCount(index) - 1)
    {
      if (pItem->isVarious())
        return QObject::tr("*VARIES*");
      return QVariant();
    }
    if (pItem->isVarious() && index.column() == columnCount(index) - 1)
      return QObject::tr("*VARIES*");
    //if (type_ == IqAppProps::prpDistance && index.column() == columnCount(index) - 1)
    //{
    //  QString dValue = pItem->value(index.column());
    //  return formatDistance(dValue);
    //}
    //if (type_ == IqAppProps::prpAngle && index.column() == columnCount(index) - 1)
    //{
    //  QString dValue = pItem->value(index.column());
    //  return formatAngle(dValue);
    //}
    //
    //if (type_ == IqAppProps::prpOdColor && index.column() == columnCount(index) - 1)
    //  return pItem->value(index.column());
    return pItem->value(index.column());

  case Qt::WhatsThisRole:
  case Qt::ToolTipRole:
    return pItem->comment();

  case Qt::BackgroundColorRole:
    if (pItem->parent() == m_pRoot)
    {   
      // light text for dark background
      return QColor(Qt::gray);
    }
    if (index.column() == 1) // value
    {
      if (type_ == IqAppProps::prpColorref)
      {
        QVariant value = pItem->value(index.column());
        uint rgb = value.toLongLong();

        int red = ODGETRED(rgb);
        int blue = ODGETBLUE(rgb);
        int green = ODGETGREEN(rgb);

        return QColor(qRgb(red, green, blue));
      }

      if (isEnabled(index))
        return QColor(Qt::white);
      else
      {
        QPalette palette = QApplication::palette();
        palette.setCurrentColorGroup(QPalette::Disabled);
        return palette.base().color();
      }
      ODA_FAIL_ONCE(); // TODO
    }
    return QVariant();

  case Qt::CheckStateRole:
    if (  (   type_ == IqAppProps::prpBool
           || type_ == IqAppProps::prpRadio)
        && (index.column() == columnCount(index) - 1))
    {
      if (pItem->isVarious())
        return Qt::PartiallyChecked;
      return QVariant(pItem->value(index.column())).toBool() ? Qt::Checked : Qt::Unchecked;
    }
    return QVariant();

  case Qt::UserRole: // used for categories to show bitmap
    if (!isProperty(index) && pItem->isWithImage())
      return QVariant(pItem->imageShown() ? Qt::Checked : Qt::Unchecked);
    if (isProperty(index) && type_ == IqAppProps::prpOdColor)
      return pItem->value(index.column());
    if (type_ == IqAppProps::prpBoolPictured && (index.column() == columnCount(index) - 1))
      return qvariant_cast<bool>(pItem->value(index.column()));
    ODA_FAIL_ONCE(); // TODO
    return QVariant();
  }
  return QVariant();
}

bool OdqPropertyModel::setData(const QModelIndex& index, const QVariant& value_, 
                               int role) // = Qt::EditRole
{   
  QVariant var = value_;
  if (role == Qt::CheckStateRole)
    var.convert(QVariant::Bool);

  OdqPropertyItem* pItem = static_cast<OdqPropertyItem*>(index.internalPointer());
  if (!pItem)
  {
    ODA_FAIL_ONCE();
    return false;
  }

  if (role == Qt::UserRole && pItem->isWithImage())
  {
    pItem->showImage(!pItem->imageShown());
    layoutChanged();
    return true;
  }

  QString qsPath = pItem->path(),
          qsValue = var.toString();

  //if (type(index) == IqAppProps::prpDistance)
  //  qsValue = unformatDistance(qsValue, pItem->value(index.column()));
  //if (type(index) == == IqAppProps::prpAngle)
  //  qsValue = unformatAngle(qsValue, pItem->value(index.column())));

  if (qsValue == pItem->value(index.column()))
    return true;
  if (!m_pFun_setPropertyValue)
    return false;

  bool bRes = (*m_pFun_setPropertyValue)(m_pPropertySource, qsPath, qsValue);
  if (bRes && m_pFun_updateXml)
    bRes = (*m_pFun_updateXml)(m_pPropertySource);

  if (bRes && qobject_cast<OdqPropPaletteView*>(m_pWidget))
  {
    IqViewSystem* iVS = getIViewSystem();
    if (iVS)
      iVS->updateViews(getIConsole()->getCurrentDatabase());
  }

  return bRes;
}

QVariant OdqPropertyModel::headerData(int section, Qt::Orientation, int role) const
{
  if (role == Qt::DisplayRole)
  {
    switch (section)
    {
    case 0:
      return tr("Name");
    case 1:
      return tr("Value");
    default:
      ODA_FAIL_ONCE(); // test
    }
  }
  return QVariant();
}

Qt::ItemFlags OdqPropertyModel::flags(const QModelIndex &index) const
{
  if (!index.isValid())
    return Qt::ItemIsEnabled;

  Qt::ItemFlags flg = QAbstractItemModel::flags(index);

  if (isProperty(index) && (index.column() == columnCount() - 1) && !isReadOnly(index)) // && isEnabled(index))
    flg |= Qt::ItemIsEditable;
  else
    flg &= ~Qt::ItemIsEditable;

  if (type(index) == IqAppProps::prpImage)
    flg &= ~Qt::ItemIsEditable;

  if (type(index) == IqAppProps::prpBool)
  {
    if (!isReadOnly(index)) // if (isEnabled(index))
      flg |= Qt::ItemIsUserCheckable;
    flg &= ~Qt::ItemIsEditable;
  }

  //OdqPropertyItem* pItem = static_cast<OdqPropertyItem*>(index.internalPointer());
  //if (!pItem)
  //  return flg;
  if (!isProperty(index) && isWithImage(index))
    flg |= Qt::ItemIsUserCheckable;
  return flg;
}

//bool OdqPropertyModel::canFetchMore(const QModelIndex& indexParent) const
//{
//  if (indexParent == QModelIndex())
//    return false;
//
//  return type(indexParent) == IqAppProps::prpTree;
//}

//void OdqPropertyModel::fetchMore(const QModelIndex& indexParent)
//{
//  OdqPropertyItem* pItem = static_cast<OdqPropertyItem*>(indexParent.internalPointer());
//  if (!pItem)
//  {
//    ODA_FAIL_ONCE(); // test
//    return;
//  }
//
//  OdqPropertyItem* pFrom = pItem->parent();
//  while (pFrom->type() == IqAppProps::prpTree)
//    pFrom = pFrom->parent();
//
//  //QString qsFilter = pItem->value(COLUMN_COUNT - 1);
//
//  for (int idx = 0; idx < pItem->countChildren(); idx++)
//  {
//    OdqPropertyItem* pChild = pItem->child(idx);
//    if (pChild->type() != IqAppProps::prpTree)
//      removeRow(pChild);
//  }
//
//  ODA_FAIL_ONCE(); // test
//  //pItem->copyChildrenFrom(pFrom, this);
//}

//void OdqPropertyItem::copyChildrenFrom(OdqPropertyItem* pFrom, OdqPropertyModel* pModel)
//{
//  QModelIndex index = pModel->createIndex(pFrom->row(), 0, this);
//
//  for (int idx = 0; idx < pFrom->childCount(); idx++)
//  {
//    OdqPropertyItem* pItem = pFrom->child(idx);
//
//    // create copy of pItem
//
//    QString qsPath = pItem->path();
//    QString qsShort = qsPath.mid(qsPath.lastIndexOf('/') + 1);
//    qsPath = path() + '/' + qsShort;
//
//    OdqPropertyItem* pCopy = new OdqPropertyItem(qsPath, this, pModel);
//    pCopy->m_type = pFrom->m_type;
//    pCopy->m_value = pFrom->m_value;
//    pCopy->m_lstChildren.clear();
//    pCopy->m_bEnabled = pFrom->m_bEnabled;
//    pCopy->m_bReadOnly = pFrom->m_bReadOnly;
//    pCopy->m_qsComment = pFrom->m_qsComment;
//    pCopy->m_lstDecorations = pFrom->m_lstDecorations;
//    pCopy->m_pImage = pFrom->m_pImage;
//    pCopy->m_indent = pFrom->m_indent;
//    pCopy->m_bInlineWithParent = pFrom->m_bInlineWithParent;
//    //pCopy->m_enablerType = pFrom->m_enablerType;
//    pCopy->m_bVarious = pFrom->m_bVarious;
//    pCopy->m_isSelected = pFrom->m_isSelected;
//
//    pModel->insertRow(rowCount(index), index, pCopy);
//    pCopy->copyChildrenFrom(pItem, pModel);
//  }
//}

bool OdqPropertyModel::hasChildren(const QModelIndex& indexParent) const // = QModelIndex()
{
  return countChildren(indexParent) > 0;
}

void OdqPropertyModel::insertRow(int row, QModelIndex indexParent, OdqPropertyItem* pItem)
{
  beginInsertRows(indexParent, row, row);

  OdqPropertyItem* pParent = static_cast<OdqPropertyItem*>(indexParent.internalPointer());
  if (!pParent)
    pParent = m_pRoot;

  if (pParent)
    pParent->insertChild(row, pItem);

  endInsertRows();
}

bool OdqPropertyModel::removeRow(int row, QModelIndex indexParent)
{
  OdqPropertyItem* pItem = static_cast<OdqPropertyItem*>(indexParent.internalPointer());
  if (!pItem)
    pItem = m_pRoot;

  beginRemoveRows(indexParent, row, row);
  OdSharedPtr<OdqPropertyItem> pRemoved = pItem->takeChild(row);
  endRemoveRows();
  if (pRemoved.get() == m_pRoot.get())
    pRemoved.detach(); // skip root 
  else
    pRemoved = NULL;//delete pRemoved;

  return true;
}

bool OdqPropertyModel::removeRow(OdqPropertyItem* pItem)
{
  if (!pItem)
    return false;

  OdqPropertyItem* pSubTree = pItem->parent();
  QModelIndex index = createIndex(pSubTree->row(), 0, pSubTree);
  if (pSubTree == m_pRoot)
    index = rootIndex(); // index = QModelIndex();

  int row = pItem->row();
  if (row == -1)
  {
    // item is removed from parent's kids
    delete pItem;
    return true;
  }

  return removeRow(row, index);
}

void OdqPropertyModel::restoreExpanded()
{
  OdqPropPaletteView* pView = qobject_cast<OdqPropPaletteView*>(m_pWidget);
  if (!pView || rowCount(rootIndex()) < 1)
    return;

  bool isInitial = m_mapExpanded.isEmpty(),
       enqueueKids = true;

  QQueue<OdqPropertyItem*> queue;
  queue.enqueue(m_pRoot);
  while (!queue.isEmpty())
  {
    OdqPropertyItem* pItem = queue.dequeue();
    if (pItem->isProperty())
      continue;
    QString qsPath = pItem->path();
      
    QModelIndex index = createIndex(pItem->row(), 0, pItem);
    if (isInitial) // is initial
    {
      pView->setExpanded(index, true);
      ODA_ASSERT_ONCE(!m_mapExpanded.isEmpty());

      if (index != rootIndex())
      {
        pView->resizeColumnToContents(0);
        pView->restoreSettings();
        enqueueKids = false; // CORE-13603 Do all property categories initially expanded //return;
      }
    }
    else
    {
      bool bValue = isExpanded(index);
      pView->setExpanded(index, bValue);
    }
    if (!enqueueKids)
      continue;

    int cntRow = pItem->countChildren();
    for (int row = 0; row < cntRow; row++)
      queue.enqueue(pItem->child(row));
  }
}

// ---

void OdqPropertyModel::setWidget(QWidget* pWidget)
{
  m_pWidget = pWidget;
}

QWidget* OdqPropertyModel::widget() const
{
  return m_pWidget;
}

void OdqPropertyModel::setPropertySource(void* pSrc)
{
  m_pPropertySource = pSrc;
}

void OdqPropertyModel::setFun_updateXml(TFun_updateXml pFun)
{
  m_pFun_updateXml = pFun;
}

void OdqPropertyModel::setFun_setPropertyValue(TFun_setPropertyValue pFun)
{
  m_pFun_setPropertyValue = pFun;
}

void OdqPropertyModel::setFun_getPropertyPossibleValues(TFun_getPropertyPossibleValues pFun)
{
  m_pFun_getPropertyPossibleValues = pFun;
}

void OdqPropertyModel::updateWidget(const QString& qsPagePath, 
                                    bool bRecreateWidgets) // = false
{
  if (m_pWidget.isNull())
    return;

  QModelIndex indexPage = modelIndex(qsPagePath);
  if (OdqPropPaletteView* pTreeView = qobject_cast<OdqPropPaletteView*>(m_pWidget))
  {
    if (rowCount(indexPage))
    {
      m_indexCurrent = indexPage;
      ODA_ASSERT_ONCE(rowCount());

      pTreeView->setModel(this);
      pTreeView->setRootIndex(m_indexCurrent);
      restoreExpanded();
      //pView->doItemsLayout(); // emit layoutChanged(); 
      pTreeView->show();
    }
    else
      ////pTreeView->setModel(NULL);
      ////pTreeView->setRootIndex(QModelIndex());
      pTreeView->hide();
    return;
  }
  //if (QTreeView* pTreeView = qobject_cast<QTreeView*>(m_pWidget))
  //{
  //  pTreeView->setModel(this); // NULL
  //  pTreeView->setRootIndex(QModelIndex());
  //  return;
  //}
  if (!indexPage.isValid()) // == QModelIndex())
    return;
  ODA_ASSERT_VAR(if (type(indexPage) == IqAppProps::prpPalette))
    ODA_ASSERT(true); // brk

  if (!bRecreateWidgets && m_indexCurrent == indexPage)
    return;

  m_pWidget->hide();

  QObjectList children = m_pWidget->children();
  foreach(QObject* pChild, children)
  {
    IqPropControl* iPropControl = qobject_cast<IqPropControl*>(pChild);

    QTreeView* pView = qobject_cast<QTreeView*>(pChild);
    if (iPropControl || pView)
    {
      QWidget* pWidget = static_cast<QWidget*>(pChild);
      pWidget->hide();
    }
    else
      delete pChild;
  }

  if (m_pWidget->layout())
    delete m_pWidget->layout();

  IqAppProps::TPropType tp = type(indexPage);
  if (tp == IqAppProps::prpPalette)
  {
    //if (!qobject_cast<OdqPropPaletteView*>(m_pWidget))
    //{
    //  OdqPropertyDialog* pDialog = qobject_cast<OdqPropertyDialog*>(m_pWidget->parent());
    //  //if (!pDialog)
    //  //  pDialog = qobject_cast<OdqPropertyDialog*>(m_pWidget->parent()->parent());
    //  ODA_ASSERT_ONCE(!m_pWidget->children().size());
    //  if (pDialog && pDialog->removeWidget(m_pWidget))
    //  {
    //    delete m_pWidget;
    //    setWidget(NULL); // m_pWidget = pView;
    //    // see also
    //    //pDataSource->getModel()->setWidget(pWidget);
    //    //pDlgData->m_pDialog->addWidget(pWidget);
    //    
    //    // test
    //    //QWidget* pView = new QWidget();
    //    //
    //    QWidget* pView = createPaletteControl(indexPage);
    //    if (pView)
    //    {
    //      m_indexCurrent = indexPage;
    //      setWidget(pView); // m_pWidget = pView;
    //      pDialog->addWidget(pView);
    //      //ODA_ASSERT_ONCE(!m_pWidget->children().size());
    //      ODA_ASSERT_ONCE(qobject_cast<OdqPropPaletteView*>(m_pWidget));
    //      qobject_cast<OdqPropPaletteView*>(m_pWidget)->setRootIndex(indexPage); // test
    //      //? redo updateFromXml
    //    }
    //    else
    //    {
    //      ODA_FAIL_ONCE();
    //      delete pView;
    //    }
    //  }
    //}

    ODA_ASSERT_ONCE(qobject_cast<OdqPropPaletteView*>(m_pWidget));
    if (qobject_cast<OdqPropPaletteView*>(m_pWidget))
    {
      //m_indexCurrent = indexPage;
      //restoreExpanded();
      ////emit layoutChanged();
      //m_pWidget->show();
      return;
    }
  }

  QVBoxLayout *pLayout = new QVBoxLayout(m_pWidget);
  addPropControls(indexPage, pLayout);
  
  m_indexCurrent = indexPage;
  //emit layoutChanged();

  m_pWidget->show();

  //m_indexCurrent = indexPage;
}

static Qt::Alignment toAlignment(const QString& cqsAlignment)
{
  Qt::Alignment alignment = 0; // default

  QString qsAlignment = cqsAlignment.toLower();
  if (qsAlignment.indexOf("right") >= 0)
    alignment = Qt::AlignRight;
  else if (qsAlignment.indexOf("left") >= 0)
    alignment = Qt::AlignLeft;
  else if (qsAlignment.indexOf("center") >= 0)
    alignment = Qt::AlignHCenter;

  return alignment;
}

// add widgets
void OdqPropertyModel::addPropControls(const QModelIndex& index, 
                                       QBoxLayout* pLayout)
{
  IqAppProps::TPropType type_ = type(index);

  Qt::Alignment alignment = 0; // default
  //bool bHorLayout = false;
  if (type_ == IqAppProps::prpCategory)
  {
    QString qsAlignment = attr(index, "alignment");
    if (!qsAlignment.isEmpty())
      alignment = toAlignment(qsAlignment);
    QString qsLayout = attr(index, "layout").toLower();
    if (!qsLayout.isEmpty())
    {
      if (!qsLayout.indexOf("hor"))
      {
        if (!qobject_cast<QHBoxLayout*>(pLayout))
        {
          QBoxLayout* pCatLayout = new QHBoxLayout;
          pLayout->addLayout(pCatLayout);
          pLayout = pCatLayout;
          // TODO
          if (alignment == Qt::AlignRight)
          {
            QSpacerItem* pSpacer = new QSpacerItem(1000, 1, QSizePolicy::Expanding, QSizePolicy::Fixed);
            pLayout->addSpacerItem(pSpacer);
            // ? //pLayout->addStretch(1); //  pLayout->addSpacing(1); // pLayout->addWidget(new QFrame()); 
          }
        }
      }
      else if (!qsLayout.indexOf("ver"))
      {
        if (!qobject_cast<QVBoxLayout*>(pLayout))
        {
          QBoxLayout* pCatLayout = new QVBoxLayout;
          pLayout->addLayout(pCatLayout);
          pLayout = pCatLayout;
        }
      }
      else
      {
        ODA_FAIL_ONCE(); // unsupported atrribute value
      }
    }
  }

  if (type_ == IqAppProps::prpTree)
  {
    QWidget* pView = createTreeControl(index);
    if (pView)
    {
      pView->setParent(m_pWidget);
      pLayout->addWidget(pView);
      populateTreeChildren(index);
    }
    return;
  }
  ODA_ASSERT_ONCE(!isTreeItem(index)); // test

  if (type_ == IqAppProps::prpGrid)
  {
    QWidget* pView = createGridControl(index);
    pLayout->addWidget(pView);
    return;
  }

  if (type_ == IqAppProps::prpFileDialog)
  {
    QWidget* pView = createFileDialogControl(index);
    pLayout->addWidget(pView);
    return;
  }

  Qt::Alignment alignmentLast = alignment;

  int count = rowCount(index);
  for (int idxProp = 0; idxProp < count; idxProp++)
  {
    QModelIndex indexChild = index.child(idxProp, 0);
    QModelIndex indexValue = index.child(idxProp, 1);

    //if (enablerType(indexChild) != enNone)
    //{
    //  ODA_ASSERT_ONCE(type(indexChild) == IqAppProps::prpBool);
    //  OdqPropControlEnabler* pEnabler = createEnabler(indexChild, pLayout->parentWidget());
    //  QVBoxLayout* pGroupLayout = new QVBoxLayout();
    //  addPropControls(indexChild, pGroupLayout);
    //  pEnabler->setLayout(pGroupLayout);
    //  pLayout->addWidget(pEnabler);
    //}

    IqAppProps::TPropType typeChild = type(indexChild);
    if (typeChild == IqAppProps::prpTree)
    {
      QWidget* pView = createTreeControl(index);
      pView->setParent(m_pWidget);
      pLayout->addWidget(pView);

      QSizePolicy policy = pView->sizePolicy();
      policy.setVerticalPolicy(QSizePolicy::Maximum);
      //policy.setVerticalStretch(vers);
      pView->setSizePolicy(policy);

      populateTreeChildren(index);
      continue;
    }
    if (typeChild == IqAppProps::prpPalette)
    {
      ODA_FAIL_ONCE(); // unsupported
      continue;
    }

    if (isProperty(indexChild))
    {
      //QHBoxLayout* pPropLayout = new QHBoxLayout;
      //pLayout->addLayout(pPropLayout);
      QBoxLayout* pPropLayout = pLayout;

      QWidget* pLabel = createLabelControl(indexChild);
      QWidget* pControl = createPropControl(indexValue);

      if (pLabel)
      {
        //
        pPropLayout = new QHBoxLayout;
        pLayout->addLayout(pPropLayout);

        pLabel->setParent(m_pWidget);
        pPropLayout->addWidget(pLabel, 1);
      }
      if (pControl)
      {
        pControl->setParent(m_pWidget);
        
        int stretch = 1;
        alignmentLast = alignment;
        QString qsAlignment = attr(indexChild, "alignment");
        if (!qsAlignment.isEmpty())
          alignmentLast = toAlignment(qsAlignment);
        if (   typeChild == IqAppProps::prpBoolNamed
            || type_ == IqAppProps::prpRadio
            || typeChild == IqAppProps::prpBoolPictured)
          stretch = 0;

        pPropLayout->addWidget(pControl, stretch, alignmentLast);
      }
    }
  }
  if (alignmentLast != Qt::AlignRight)
    pLayout->addStretch(1);
}

QWidget* OdqPropertyModel::createPropControl(const QModelIndex& index)
{
  IqAppProps::TPropType typeParent = type(index.parent());

  QWidget* pView = m_pWidget->findChild<QWidget*>(path(index));
  if (pView)
  {
    pView->show();

    Qt::ItemFlags flgs = flags(index);
    bool bEnabled = (flgs & Qt::ItemIsEditable) || (flgs & Qt::ItemIsUserCheckable);
    pView->setEnabled(bEnabled);

    IqAppProps::TPropType tp = type(index);

    QVariant value = index.data();

    if (tp == IqAppProps::prpBool)
      value = index.data(Qt::CheckStateRole);
    if (tp == IqAppProps::prpColorref)
      value = index.data(Qt::BackgroundColorRole);
    if (tp == IqAppProps::prpOdColor)
      value = index.data(Qt::UserRole);

    IqPropControl* iPropControl = qobject_cast<IqPropControl*>(pView);

    pView->blockSignals(true); // QObject::disconnect(pView, SIGNAL(propEdited(QWidget *)), this, SLOT(propEdited(QWidget *)));

    switch (tp)
    {
    case IqAppProps::prpCombo: case IqAppProps::prpLinetype: case IqAppProps::prpLineweight: case IqAppProps::prpTransparency:
      {
        OdqPropControlComboBox* pPropCombo = qobject_cast<OdqPropControlComboBox*>(pView);
        if (pPropCombo)
        {
          QStringList qsItems, qsImages;
          if (getPropertyPossibleValues(index, qsItems, qsImages))
            pPropCombo->fillCombo(qsItems, qsImages);
        }
        break;
      }
    case IqAppProps::prpBoolPictured:
      if (typeParent == IqAppProps::prpCategory)
      {
        OdqPropertyItem* pItem = static_cast<OdqPropertyItem *>(index.internalPointer());
        ODA_ASSERT_ONCE(pItem);
        QString qsPath = pItem->decoration(index.column()); // TODO via index.data(Qt::DecorationRole)
        ODA_ASSERT_ONCE(!qsPath.isEmpty());
        if (!qsPath.isEmpty())
        {
          QPixmap pixmap(qsPath);
          ODA_ASSERT_ONCE(!pixmap.isNull());
          if (!pixmap.isNull())
          {
            pixmap.setMask(pixmap.createHeuristicMask());
            OdqPropControlButton* pButton = qobject_cast<OdqPropControlButton*>(pView);
            pButton->setIcon(QIcon(pixmap));
            if (pButton->isCheckable())
            {
              QString qsValue = pItem->value(index.column()).toLower();
              pButton->setValue(qsValue == "true");
              iPropControl = NULL;
            }
          }
        }
      }
      break;
    } // switch

    if (iPropControl)
      iPropControl->setValue(value);

    pView->blockSignals(false); // QObject::connect(pView, SIGNAL(propEdited(QWidget *)), this, SLOT(propEdited(QWidget *)));

    return pView;
  }

  IqAppProps::TPropType type_ = type(index);
  QWidget* pWidget = NULL;

  switch (type_)
  {
  case IqAppProps::prpBool:
    {
      QString qsText = index.sibling(index.row(), 0).data().toString();
      OdqPropControlCheckBox* check = new OdqPropControlCheckBox(qsText);
      QVariant val = index.data(Qt::CheckStateRole);
      check->setCheckState(static_cast<Qt::CheckState>(val.toInt()));
      pWidget = check;
      break;
    }
  case IqAppProps::prpBoolPictured:
    {
      if (typeParent != IqAppProps::prpCategory)
        break;
      QString qsText = index.sibling(index.row(), 0).data().toString();
      OdqPropertyItem* pItem = static_cast<OdqPropertyItem *>(index.internalPointer());
      ODA_ASSERT_ONCE(pItem);
      QString qsPath = pItem->decoration(index.column()); // TODO via index.data(Qt::DecorationRole)
      OdqPropControlButton* pButton = NULL;
      ODA_ASSERT_ONCE(!qsPath.isEmpty());
      if (!qsPath.isEmpty())
      {
        QPixmap pixmap(qsPath);
        ODA_ASSERT_ONCE(!pixmap.isNull());
        if (!pixmap.isNull())
        {
          pixmap.setMask(pixmap.createHeuristicMask());
          pButton = new OdqPropControlButton(QIcon(pixmap)); //, qsText);
          QSize sizeButton(pixmap.size());
          QComboBox* pCombo = m_pWidget->findChild<QComboBox*>();
          if (pCombo)
          {
            pCombo->adjustSize();
            int sz = pCombo->height();
            if (sz > 5 && sz < 50)
              sizeButton = QSize(sz, sz);
          }
          pButton->setFixedSize(sizeButton);
          //pButton->setIconSize(QSize(sizeButton.width() - 4, sizeButton.height() - 4));

          QString qsCheckable = attr(index, "checkable", "false").toLower();
          if (qsCheckable == "true")
          {
            pButton->setCheckable(true);
            QString qsValue = pItem->value(index.column()).toLower();
            pButton->setValue(qsValue == "true");
          }
        }
      }
      if (!pButton)
        pButton = new OdqPropControlButton(qsText);
      pWidget = pButton;
      break;
    }
  case IqAppProps::prpBoolNamed: // button
    {
      QString text = index.sibling(index.row(), 0).data().toString();
      OdqPropControlButton* pButton = new OdqPropControlButton(text);
      pWidget = pButton;
      break;
    }
  case IqAppProps::prpRadio:
    {
      QString text = index.sibling(index.row(), 0).data().toString();
      OdqPropControlRadioButton* pButton = new OdqPropControlRadioButton(text);
      bool bVal = index.data(Qt::CheckStateRole).toBool();
      pButton->setChecked(bVal);
      pWidget = pButton;
      break;
    }
  case IqAppProps::prpImage:
    return NULL;
  case IqAppProps::prpInt:
    {
      int step = attrToInt(index, "Step", 1);
      step = qAbs(step);
      if (step > 0)
      {
        int min = attrToInt(index, "Min", INT_MIN),
            max = attrToInt(index, "Max", INT_MAX);
        OdqPropControlSpinBox *spinBox = new OdqPropControlSpinBox;
        spinBox->setSingleStep(step);
        int val = index.data().toInt();
        spinBox->setValue(val);
        ODA_ASSERT_ONCE(max > min && val >= min && val <= max);
        if (max > min && val >= min && val <= max)
        {
          if (min != INT_MAX)
            spinBox->setMinimum(min);
          if (max != INT_MIN)
            spinBox->setMaximum(max);
        }
        pWidget = spinBox;
      }
      break;
    }
  case IqAppProps::prpDouble:
  case IqAppProps::prpDistance:
  case IqAppProps::prpAngle:
  {
      double step = attrToDouble(index, "Step", 0); // default as string
      step = qAbs(step);
      if (step > 0.0)
      {
        double min = attrToDouble(index, "Min", - 1.0e+100),
               max = attrToDouble(index, "Max", 1.0e+100);
        OdqPropControlDoubleSpinBox *doubleSpinBox = new OdqPropControlDoubleSpinBox;
        doubleSpinBox->setSingleStep(step);
        double val = index.data().toDouble();
        doubleSpinBox->setValue(val);
        ODA_ASSERT_ONCE(max > min && val > min && val < max);
        if (max > min && val > min && val < max)
        {
          if (min != INT_MAX)
            doubleSpinBox->setMinimum(min);
          if (max != INT_MIN)
            doubleSpinBox->setMaximum(max);
        }
        pWidget = doubleSpinBox;
      }
      break;
    }
  case IqAppProps::prpFolder:
    {
      OdqPropControlFolder* folder = new OdqPropControlFolder(index.data().toString(), NULL);
      pWidget = folder;
      break;
    }
  case IqAppProps::prpColorref:
    {
      QColor color = qvariant_cast<QColor>(index.data(Qt::BackgroundColorRole));
      OdqPropControlColorref *pColorref = new OdqPropControlColorref(color, NULL);
      pWidget = pColorref;
      break;
    }
  } // end switch

  if (   type_ == IqAppProps::prpCombo 
      || type_ == IqAppProps::prpLinetype || type_ == IqAppProps::prpLineweight || type_ == IqAppProps::prpTransparency
      || isVarious(index))
  {
    OdqPropControlComboBox* pCombo = new OdqPropControlComboBox();
    pCombo->setModelIndex(index);
    QStringList qsItems, qsImages;
    if (getPropertyPossibleValues(index, qsItems, qsImages))
      pCombo->fillCombo(qsItems, qsImages);
    pCombo->setEditable(isVarious(index));
    pCombo->setValue(index.data());
    pWidget = pCombo;
  }

  if (!pWidget)
  {
    OdqPropControlLineEdit *pLineEdit = new OdqPropControlLineEdit;
    pLineEdit->setText(index.data().toString());
    pWidget = pLineEdit;
  }

  IqPropControl* iPropControl = qobject_cast<IqPropControl*>(pWidget);
  if (iPropControl)
  {    
    iPropControl->setModelIndex(index);
    iPropControl->createConnections();
    QObject::connect(pWidget, SIGNAL(propEdited(QWidget *)), this, SLOT(propEdited(QWidget *)));
  }

  Qt::ItemFlags flgs = flags(index);
  bool bEnabled = (flgs & Qt::ItemIsEditable) || (flgs & Qt::ItemIsUserCheckable);
  pWidget->setEnabled(bEnabled);
  pWidget->setObjectName(path(index));

  return pWidget;
}

QWidget* OdqPropertyModel::createLabelControl(const QModelIndex& index)
{
  IqAppProps::TPropType type_ = type(index);

  if (   type_ == IqAppProps::prpBool
      || type_ == IqAppProps::prpBoolPictured
      || type_ == IqAppProps::prpBoolNamed // button with text
      || type_ == IqAppProps::prpRadio)
    return NULL;

  if (type_ == IqAppProps::prpImage)
  {
    QLabel* pImage = new QLabel();
    QString qsPath = index.data(Qt::DecorationRole).toString();
    ODA_ASSERT_ONCE(!qsPath.isEmpty());
    QPixmap pixmap(qsPath);
    ODA_ASSERT_ONCE(!pixmap.isNull());
    pixmap.setMask(pixmap.createHeuristicMask());
    pImage->setFixedSize(pixmap.size());
    pImage->setPixmap(pixmap);
    return pImage;
  }

  QString qsLabel = index.data().toString();
  if (qsLabel.isEmpty()) // via VisibleName
    return NULL;

  QLabel* pLabel = new QLabel(qsLabel);

  QModelIndex value = index.sibling(index.row(), 1);
  Qt::ItemFlags flgs = flags(value);

  bool bEnabled = (flgs & Qt::ItemIsEditable) || (flgs & Qt::ItemIsUserCheckable);
  pLabel->setEnabled(bEnabled);

  return pLabel;
}

QAbstractItemView* OdqPropertyModel::createGridControl(const QModelIndex& index)
{
  //if (canFetchMore(index))
  //  fetchMore(index);

  QTreeView* pView = m_pWidget->findChild<QTreeView*>(path(index));
  if (pView)
  {
    pView->show();
    pView->setEnabled(isEnabled(index));
    //isReadOnly

    OdqGridProxyModel* pProxy = qobject_cast<OdqGridProxyModel*>(pView->model());        
    pProxy->setRootIndex(index);

    for (int iColumn = 0; iColumn < pProxy->columnCount(); iColumn++)
      pView->resizeColumnToContents(iColumn);

    return pView;
  }

  pView = new QTreeView(m_pWidget);

  OdqGridProxyModel* pProxyModel = new OdqGridProxyModel(pView, this);
  pProxyModel->setRootIndex(index);

  pView->setModel(pProxyModel);
  pView->setRootIsDecorated(true);
  pView->setSelectionMode(QAbstractItemView::NoSelection);
  pView->header()->setStretchLastSection(true);
  pView->setAllColumnsShowFocus(true);

  if (   !QObject::connect(pView->selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)),
                           pProxyModel, SLOT(setCurrentIndex(const QModelIndex&)))
      || !QObject::connect(pProxyModel, SIGNAL(notifyGridSourceChanged(const QModelIndex&)),
                           this, SLOT(currentChanged(const QModelIndex&)))

      || !QObject::connect(pView->selectionModel(), SIGNAL(currentRowChanged(const QModelIndex&, const QModelIndex&)),
                           pProxyModel, SLOT(currentRowChanged(const QModelIndex&))) // currentTreeItemChanged
      || !QObject::connect(pProxyModel, SIGNAL(notifyGridRowChanged(const QModelIndex&)),
                           this, SLOT(currentGridRowChanged(const QModelIndex&)))) // currentTreeItemChanged
  {
    ODA_FAIL_ONCE();
  }

  for (int iColumn = 0; iColumn < pProxyModel->columnCount(); iColumn++)
    pView->resizeColumnToContents(iColumn);

  QString qsPath = path(index);
  pView->setObjectName(qsPath);

  QItemDelegate *pDelegate = new OdqGridProxyDelegate(pProxyModel);
  pView->setItemDelegate(pDelegate);
  if (!isReadOnly(index))
    pView->setEditTriggers(QAbstractItemView::AllEditTriggers);

  return pView;
}

QWidget* OdqPropertyModel::createTreeControl(const QModelIndex& index)
{
  // POSSIBLE TODO
  // create special TreeModel like in qt/examples/itemviews/simpletreemodel

  QString qsPath = path(index);
  QTreeWidget* pView = m_pWidget->findChild<QTreeWidget*>(qsPath); // QTreeView
  if (pView)
  {
    pView->show();
    //pView->setEnabled(isEnabled(index));

    return pView;
  }

  pView = new QTreeWidget(m_pWidget);
  pView->setObjectName(qsPath);

  pView->setColumnCount(1);
  pView->header()->hide();

  if (!connect(pView, SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)),
               this, SLOT(currentTreeItemChanged(QTreeWidgetItem*))))
  {
    ODA_FAIL_ONCE();
  }

  return pView;
}

void OdqPropertyModel::populateTreeChildren(const QModelIndex& index, 
                                            QTreeWidgetItem* pTreeItem) // = NULL
{
  int rowCount = index.model()->rowCount(index);

  QTreeWidget* pTree = NULL;
  if (pTreeItem)
    pTree = pTreeItem->treeWidget();
  else
    pTree = m_pWidget->findChild<QTreeWidget*>(path(index));
  ODA_ASSERT_ONCE(pTree);

  for (int idx = 0; idx < rowCount; idx++)
  {
    QModelIndex indexChild = index.child(idx, 0);

    QString qsName = indexChild.data().toString();
    ODA_ASSERT_ONCE(!qsName.isEmpty());

    QTreeWidgetItem* pChildItem = NULL;
    if (!pTreeItem)
    {
      pTree->blockSignals(true);

      pChildItem = pTree->topLevelItem(idx);
      if (!pChildItem)
      {
        pChildItem = new QTreeWidgetItem(QStringList(qsName));
        pTree->addTopLevelItem(pChildItem);
      }
    }
    else
    {
      for (int idxChild = 0; idxChild < pTreeItem->childCount(); idxChild++)
      {
        QTreeWidgetItem* pChild = pTreeItem->child(idxChild);
        ODA_ASSERT_ONCE(pChild);

        QString qsTxt = pChild->text(0);
        ODA_ASSERT_ONCE(!qsTxt.isEmpty());
        if (qsName != qsTxt)
          continue;

        pChildItem = pChild;
        break;
      }

      if (!pChildItem)
      {
        pChildItem = new QTreeWidgetItem(pTreeItem, QStringList(indexChild.data().toString()));
        pTreeItem->addChild(pChildItem);
      }
    }
    pChildItem->setData(0, Qt::UserRole, path(indexChild));
    QString qsImagePath = indexChild.data(Qt::DecorationRole).toString();
    if (!qsImagePath.isEmpty())
    {
      QPixmap pixmap(qsImagePath);
      ODA_ASSERT_ONCE(!pixmap.isNull());
      pixmap.setMask(pixmap.createHeuristicMask());
      QIcon icon(pixmap);
      pChildItem->setIcon(0, icon);
    }

    pTree->expandItem(pChildItem); // expandAll

    if (index.model()->hasChildren(indexChild))
      populateTreeChildren(indexChild, pChildItem);

    if (isSelected(indexChild))
    {
      m_indexCurrent = indexChild; // signal blocks here
      pTree->setCurrentItem(pChildItem);
    }
  }

  if (!pTreeItem)
  {
    pTree->blockSignals(false);
    if (!m_indexCurrent.isValid())
    {
      ODA_ASSERT_ONCE(isSelected(index));
      QTreeWidgetItem* pCurrentItem = pTree->topLevelItem(0);
      pTree->setCurrentItem(pCurrentItem);
      ODA_ASSERT_ONCE(m_indexCurrent == index); // was set via signal 
    }
  }
}

QWidget* OdqPropertyModel::createPaletteControl(const QModelIndex& index)
{
  // create special TreeModel like in qt/examples/itemviews/simpletreemodel

  QString qsPath = path(index);

  //OdqPropPaletteView* pView = m_pWidget->findChild<OdqPropPaletteView*>(qsPath);
  OdqPropPaletteView* pView = qobject_cast<OdqPropPaletteView*>(m_pWidget);
  if (pView)
  {
    pView->show();
    //pView->setEnabled(isEnabled(index));

    return pView;
  }

  pView = new OdqStyledPropPaletteView(m_pWidget); //pView = new OdqPropPaletteView(m_pWidget);
  pView->setObjectName(qsPath);
  //OdqPropertyModel* pModel = qobject_cast<OdqPropertyModel*>(const_cast<QAbstractItemModel *>(index.model()));
  pView->setModel(this); // pView->setModel(pProxyModel);

  //pView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  //pView->setFrameStyle(QFrame::VLine);

  //pView->setColumnCount(1);
  //pView->header()->hide();

  if (!connect(pView, SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)),
               this, SLOT(currentTreeItemChanged(QTreeWidgetItem*))))
  {
    ODA_FAIL_ONCE();
  }

  return pView;
}

QWidget* OdqPropertyModel::createFileDialogControl(const QModelIndex& index)
{
  QString qsPath = path(index);
  QWidget* pWidget = m_pWidget->findChild<QWidget*>(qsPath);
  if (pWidget)
  {
    pWidget->show();
    return pWidget;
  }

  QString qsFilter = attr(index, "filters"),
          qsAcceptLabel = attr(index, "acceptname"), // "" - default (Open)
          qsRejectLabel; // "" - without cancel
  bool bForSave = false; // TODO

  OdqPropControlFileDialog* pDlg = new OdqPropControlFileDialog(m_pWidget, // pParent
                                                                qsFilter, bForSave,
                                                                qsAcceptLabel, qsRejectLabel);
  pDlg->setObjectName(qsPath);
  pDlg->setModelIndex(index);
  pDlg->createConnections();
  QObject::connect(pDlg, SIGNAL(propEdited(QWidget*)), this, SLOT(propEdited(QWidget*)));

  return pDlg;
}

// slot
void OdqPropertyModel::currentTreeItemChanged(QTreeWidgetItem* pTreeItem)
{
  QString qsPath = pTreeItem->data(0, Qt::UserRole).toString();
  ODA_ASSERT_ONCE(!qsPath.isEmpty());
  
  QModelIndex index = modelIndex(qsPath);
  emit currentChanged(index);

  if (m_pFun_setPropertyValue)
  {
    bool bRes = (*m_pFun_setPropertyValue)(m_pPropertySource, qsPath, "<setCurrent>");
    if (bRes && m_pFun_updateXml)
      (*m_pFun_updateXml)(m_pPropertySource);
  }
}

// slot
void OdqPropertyModel::currentGridRowChanged(const QModelIndex& index)
{
  if (!m_pFun_setPropertyValue)
    return;
  QString qsPath = path(index);
  // remove colum name from path
  int pos = qsPath.lastIndexOf("/");
  if (pos > 0)
    qsPath = qsPath.left(pos);
  if (qsPath.isEmpty())
    return;

  bool bRes = (*m_pFun_setPropertyValue)(m_pPropertySource, qsPath, "<setCurrent>");
  if (bRes && m_pFun_updateXml)
    (*m_pFun_updateXml)(m_pPropertySource);
}

//////////////////////////////////////////////////////////////////////////

//static 
QString OdqPropertyModel::extractFirst(QString& qsPath)
{
  QString qsFirst;
  int pos = qsPath.indexOf("/");
  if (pos < 0)
  {
    qsFirst = qsPath;
    qsPath.clear();
    return qsFirst;
  }

  qsFirst = qsPath.left(pos);
  qsPath = qsPath.mid(++pos);
  return qsFirst;
}

//static 
QString OdqPropertyModel::stringByOdColor(const OdCmColor& color)
{
  return stringByOdColor(color.entityColor());
}

//static 
QString OdqPropertyModel::stringByOdColor(const OdCmEntityColor& color, 
                                          OdRxObject* pRxDb) // = NULL
{
  QString qsColor;
  switch (color.colorMethod())
  {
  case OdCmEntityColor::kByLayer:
    return QObject::tr((!pRxDb || OdDbDatabase::cast(pRxDb).get()) ? "ByLayer" : "ByLevel");
  case OdCmEntityColor::kByBlock:
    return QObject::tr((!pRxDb || OdDbDatabase::cast(pRxDb).get()) ? "ByBlock" : "ByCell");
  case OdCmEntityColor::kForeground:
    return QObject::tr("Foreground");
  case OdCmEntityColor::kNone:
    ODA_FAIL_ONCE();
    qsColor = QObject::tr("None");
    break;
  case OdCmEntityColor::kByDgnIndex:
    ODA_ASSERT_ONCE(!pRxDb || getExtDbModule(pRxDb)->baseModuleName(true) == "dgn");
    //qsColor = aciColor(color.dgnColorIndex(), pRxDb);  // TODO? // it is strange but dgnColorIndex() is missing diifer to setDgnColorIndex
    //break;
  case OdCmEntityColor::kByACI: // index in palette "Index Color"
    qsColor = aciColor(color.colorIndex(), pRxDb);
    break;
  case OdCmEntityColor::kByColor:
    qsColor = QString("%1,%2,%3").arg(color.red()).arg(color.green()).arg(color.blue());
    break;
  }

  if (qsColor.isEmpty())
  {
    ODA_FAIL_ONCE(); // test
    qsColor = QObject::tr("Select Color...");    
  }
  return qsColor;
}

//static 
OdCmEntityColor OdqPropertyModel::OdColorByString(const QString& qsColor,
                                                  bool* pIsOk, // = NULL
                                                  OdRxObject* pRxDb) // = NULL
{
  if (!pIsOk)
  {
    static bool s_bOk = false;
    pIsOk = &s_bOk;
  }
  *pIsOk = false;

  if (qsColor.isEmpty() || qsColor == QObject::tr("*VARIES*"))
  {
    ODA_FAIL_ONCE();
    return OdCmEntityColor();
  }

  QStringList lstRgb = qsColor.split(',');
  if (lstRgb.size() == 3)
  {
    OdUInt8 red = (OdUInt8) lstRgb[0].toUInt(pIsOk);
    if (*pIsOk)
    {
      OdUInt8 green = (OdUInt8) lstRgb[1].toUInt(pIsOk);
      if (*pIsOk)
      {
        OdUInt8 blue = (OdUInt8) lstRgb[2].toUInt(pIsOk);
        if (*pIsOk)
          return OdCmEntityColor(red, green, blue);
      }
    }
    ODA_FAIL_ONCE(); // test
  }

  OdInt16 indexColor = (OdInt16) qsColor.toInt(pIsOk);
  if (*pIsOk)
  {
    //kACIbyBlock       0       ByBlock.
    //kACIforeground    7       Foreground *color*.
    //kACIbyLayer       256     ByLayer. 
    //kACIRed           1       Red. 
    //kACIYellow        2       Yellow. 
    //kACIGreen         3       Green. 
    //kACICyan          4       Cyan. 
    //kACIBlue          5       Blue. 
    //kACIMagenta       6       Magenta. 
    //..                8-255   Defined by display device.
    //kACInone          257     No *color*.

    OdCmEntityColor color(OdCmEntityColor::kByACI);
    ODA_ASSERT_VAR(if (!pRxDb && OdDbDatabase::cast(pRxDb).get()))
      ODA_ASSERT_ONCE(   (indexColor >= OdCmEntityColor::kACIRed && indexColor < OdCmEntityColor::kACInone)
                      || indexColor == OdCmEntityColor::kACIbyBlock || indexColor == OdCmEntityColor::kACIbyLayer);
    color.setColorIndex(indexColor);
    return color;
  }

  if (!pRxDb || OdDbDatabase::cast(pRxDb).get())
  {
    for (indexColor = OdCmEntityColor::kACIRed; indexColor <= OdCmEntityColor::kACIWhite; indexColor++)
    {
      if (qsColor != aciColor(indexColor)) //, pRxDb))
        continue;

      OdCmEntityColor color(OdCmEntityColor::kByACI);
      color.setColorIndex(indexColor);
      *pIsOk = true;
      return color;
    }
  }

  if (qsColor == QObject::tr("ByLayer") || qsColor == QObject::tr("ByLevel"))
  {
    *pIsOk = true;
    return OdCmEntityColor(OdCmEntityColor::kByLayer);
  }
  if (qsColor == QObject::tr("ByBlock") || qsColor == QObject::tr("ByCell"))
  {
    *pIsOk = true;
    return OdCmEntityColor(OdCmEntityColor::kByBlock);
  }
  if (qsColor == QObject::tr("Foreground"))
  {
    *pIsOk = true;
    return OdCmEntityColor(OdCmEntityColor::kForeground);
  }
  
  ODA_FAIL_ONCE(); // ODA_ASSERT_ONCE(qsColor == QObject::tr("None"));
  return OdCmEntityColor(OdCmEntityColor::kNone); // OdCmEntityColor::kACInone
}

//static 
bool OdqPropertyModel::getPossibleOdColors(QStringList& values, 
                                           bool bSkipByLayer) // = false
{
  ODA_ASSERT_ONCE(values.isEmpty());

  bool isDgn = false;
  OdRxObject* pRxDb = getIConsole()->getCurrentDatabase();
  if (pRxDb && OdDbDatabase::cast(pRxDb).isNull())
    isDgn = true;

  if (!bSkipByLayer)
  {
    values << (isDgn ? QObject::tr("ByLevel") : QObject::tr("ByLayer"))
           << (isDgn ? QObject::tr("ByCell") :QObject::tr("ByBlock"));
    //values << QObject::tr("Foreground");
  }

  if (!isDgn)
  {
    OdCmEntityColor color(OdCmEntityColor::kByACI);
    for (int iAci = OdCmEntityColor::kACIRed; iAci <= OdCmEntityColor::kACIWhite; iAci++)
      values << aciColor(iAci);
  }

  values << QObject::tr("Select Color...");
  return true;
}

static OdDb::LineWeight lineWeightByIndex(OdInt8 lw)
{
  switch (lw)
  {
  case 0:
    return OdDb::kLnWt000;
  case 1:
    return OdDb::kLnWt005;
  case 2:
    return OdDb::kLnWt009;
  case 3:
    return OdDb::kLnWt013;
  case 4:
    return OdDb::kLnWt015;
  case 5:
    return OdDb::kLnWt018;
  case 6:
    return OdDb::kLnWt020;
  case 7:
    return OdDb::kLnWt025;
  case 8:
    return OdDb::kLnWt030;
  case 9:
    return OdDb::kLnWt035;
  case 10:
    return OdDb::kLnWt040;
  case 11:
    return OdDb::kLnWt050;
  case 12:
    return OdDb::kLnWt053;
  case 13:
    return OdDb::kLnWt060;
  case 14:
    return OdDb::kLnWt070;
  case 15:
    return OdDb::kLnWt080;
  case 16:
    return OdDb::kLnWt090;
  case 17:
    return OdDb::kLnWt100;
  case 18:
    return OdDb::kLnWt106;
  case 19:
    return OdDb::kLnWt120;
  case 20:
    return OdDb::kLnWt140;
  case 21:
    return OdDb::kLnWt158;
  case 22:
    return OdDb::kLnWt200;
  case 23:
    return OdDb::kLnWt211;
  case 30:
    return OdDb::kLnWtByBlock;
  case 31:
    return OdDb::kLnWtByLwDefault;
  }
  return OdDb::kLnWtByLayer;
}

//static 
QString OdqPropertyModel::stringByLineType(OdDbObjectId idLinetype)
{
  QString qsValue = getIApp()->toQString(OdDbSymUtil::getSymbolName(idLinetype));
  if (qsValue.isEmpty())
  {
    ODA_ASSERT_ONCE(idLinetype.isNull()); 
    qsValue = getIApp()->toQString(OdDbSymUtil::linetypeByLayerName());
  }
  ODA_ASSERT_ONCE(!qsValue.isEmpty());

  // TODO apperance + name like ACAD in property palette (maybe take of from comment)
  //OdDbLinetypeTableRecordPtr pLineTypeRec = id.safeOpenObject();
  //qsValue = getIApp()->toQString(pLineTypeRec->getName());
  //QString qsComment = getIApp()->toQString(pLineTypeRec->comments());
  //if (!qsComment.isEmpty())
  //{
  //  qsValue += "\t/\t";
  //  qsValue += qsComment;
  //}

  return qsValue;
}

//static 
OdDbObjectId OdqPropertyModel::lineTypeIdByString(OdRxObject* pRxDatabase, const QString& qsValue)
{
  ODA_ASSERT_ONCE(pRxDatabase)
  OdDbDatabasePtr pDb = OdDbDatabase::cast(pRxDatabase);
  if (pDb.get())
  {
    OdDbLinetypeTablePtr pLinetypes = pDb->getLinetypeTableId().safeOpenObject();
    OdDbLinetypeTableIteratorPtr pIter = pLinetypes->newIterator();
    for (pIter->start(); !pIter->done(); pIter->step())
    {
      OdDbObjectId idLinetype = pIter->getRecordId();
      if (qsValue != stringByLineType(idLinetype))
        continue;

      return idLinetype;
    }
  }
  
  ODA_FAIL_ONCE();
  return OdDbObjectId();
}

//static
QString OdqPropertyModel::stringByLineWeight(int nLineWeight, 
                                             bool bLineWeightIsIndex) // = false
{
  if (bLineWeightIsIndex)
    nLineWeight = lineWeightByIndex((char) nLineWeight);
  QString qsLineWeight;
  switch (nLineWeight)
  {
  case OdDb::kLnWtByLayer:
    qsLineWeight = QObject::tr("ByLayer");
    break;    
  case OdDb::kLnWtByBlock:
    qsLineWeight = QObject::tr("ByBlock");
    break;
  case OdDb::kLnWtByLwDefault:
    qsLineWeight = QObject::tr("Default");
    break;
  default: // "%1.2f mm"
    qsLineWeight = QObject::tr("%1 mm").arg(nLineWeight / 100.0,
                                            4, //fieldWidth
                                            'f', // fmt
                                            2); //prec
    break;
  }
  return qsLineWeight;
}

//static
OdDb::LineWeight OdqPropertyModel::lineWeightByString(const QString& qsValue)
{
  for (int index = 0; index < 32; index++)
  {
    if (qsValue != stringByLineWeight(index, true))
      continue;

    OdDb::LineWeight lw = lineWeightByIndex(index);
    return lw;
  }
  return OdDb::LineWeight(-100); // invalid
}

//static 
bool OdqPropertyModel::getPossibleLineTypes(OdRxObject* pRxDatabase,
                                            QStringList& values, QStringList& icons,
                                            bool bSkipByLayer) // = false
{
  OdDbDatabasePtr pDb = OdDbDatabase::cast(pRxDatabase);
  ODA_ASSERT_ONCE(!pDb.isNull());
  if (pDb.isNull())
    false;
  OdDbLinetypeTablePtr pLinetypes = pDb->getLinetypeTableId().safeOpenObject();
  OdDbLinetypeTableIteratorPtr pIter = pLinetypes->newIterator();
  for (pIter->start(); !pIter->done(); pIter->step())
  {
    OdDbObjectId id = pIter->getRecordId();
    // skip byBlock and byLayer
    if (bSkipByLayer && (id == pDb->getLinetypeByBlockId() || id == pDb->getLinetypeByLayerId()))
      continue;

    values << OdqPropertyModel::stringByLineType(id);
  }
  return true;
}

//static 
bool OdqPropertyModel::getPossibleLineWeights(QStringList& values, QStringList& icons, 
                                              bool bSkipByLayer) // = false
{
  ODA_ASSERT_ONCE(icons.isEmpty()); // TODO

  if (!bSkipByLayer)
  {
    values << OdqPropertyModel::stringByLineWeight(OdDb::kLnWtByLayer);
    values << OdqPropertyModel::stringByLineWeight(OdDb::kLnWtByBlock);
  }
  values << OdqPropertyModel::stringByLineWeight(OdDb::kLnWtByLwDefault);

  for (int index = 0; index < 24; index++)
    values << OdqPropertyModel::stringByLineWeight(index, true);

  return true;
}

//static 
QString OdqPropertyModel::stringByTransparency(const OdCmTransparency& transparency)
{
  QString qsValue;
  switch (transparency.method())
  {
  case OdCmTransparency::kByLayer:
    if (OdDbDatabase::cast(getIConsole()->getCurrentDatabase()).get())
      qsValue = QObject::tr("ByLayer");
    else
      qsValue = QObject::tr("ByLevel"); // simplest way for DGN
    break;    
  case OdCmTransparency::kByBlock:
    qsValue = QObject::tr("ByBlock");
    break;
  case OdCmTransparency::kErrorValue: // Error value.
    qsValue = QObject::tr("ErrorValue");
    break;
  default: 
    ODA_FAIL_ONCE();
  case OdCmTransparency::kByAlpha:
    qsValue = QString::number(int(100.5 - transparency.alphaPercent() * 100.0));
    break;
  }
  return qsValue;
}

//static 
OdCmTransparency OdqPropertyModel::transparencyByString(const QString& qsValue)
{
  if (!qsValue.compare("bylayer", Qt::CaseInsensitive) || !qsValue.compare("bylevel", Qt::CaseInsensitive))
      return OdCmTransparency(OdCmTransparency::kByLayer);
  if (!qsValue.compare("byblock", Qt::CaseInsensitive))
    return OdCmTransparency(OdCmTransparency::kByBlock);

  bool bOk = false;
  OdUInt32 val = qsValue.toUInt(&bOk);
  if (!bOk || val > 100)
    return OdCmTransparency(OdCmTransparency::kErrorValue);

  return OdCmTransparency((100.0 - val) / 100.0);
}

//static 
bool OdqPropertyModel::getPossibleTransparencies(QStringList& values, bool bSkipByLayer) // = false
{
  if (!bSkipByLayer)
  {
    values << OdqPropertyModel::stringByTransparency(OdCmTransparency(OdCmTransparency::kByLayer));
    values << OdqPropertyModel::stringByTransparency(OdCmTransparency(OdCmTransparency::kByBlock));
  }

  // ByLayer ByBlock 0 10 ... 100
  for (int val = 0; val <= 100; val += 10)
    values << OdqPropertyModel::stringByTransparency(OdCmTransparency((100.0 - val) / 100.0));
  return true;
}

//////////////////////////////////////////////////////////////////////////

OdqPropertyDialog::OdqPropertyDialog(QWidget* pParent, // = NULL
                                     Qt::WindowFlags flags) // = 0
  : QDialog(pParent, flags)
{
  m_stackLayouts.push(new QVBoxLayout);
  setLayout(m_stackLayouts.top());
}   

OdqPropertyDialog::~OdqPropertyDialog()
{
}

void OdqPropertyDialog::setButtons(QDialogButtonBox::StandardButtons buttons) // = (QDialogButtonBox::Ok | QDialogButtonBox::Cancel)
{
  ODA_ASSERT_ONCE(!m_stackLayouts.isEmpty())
  QBoxLayout* pFrameLayout = m_stackLayouts.first().data();

  if (buttons == QDialogButtonBox::NoButton || !pFrameLayout)
    return;
  
  QDialogButtonBox* pButtonBox = new QDialogButtonBox;
  pButtonBox->setStandardButtons(buttons);
  connect(pButtonBox, SIGNAL(accepted()), SLOT(accept()), Qt::QueuedConnection);
  connect(pButtonBox, SIGNAL(rejected()), SLOT(reject()), Qt::QueuedConnection);
  pFrameLayout->addWidget(pButtonBox);
}

QBoxLayout* OdqPropertyDialog::topLayout()
{
  return m_stackLayouts.top().data();
}

QBoxLayout* OdqPropertyDialog::pushLayout(QBoxLayout* pLayout, // = NULL
                                         int stretch) // = 0
{
  QVBoxLayout* pVertLayout = qobject_cast<QVBoxLayout*>(topLayout());
  if (!pLayout)
  {
    if (pVertLayout)
      pLayout = new QHBoxLayout();
    else
      pLayout = new QVBoxLayout();
  }

  pVertLayout->addLayout(pLayout, stretch);
  m_stackLayouts.push(pLayout);
  return pLayout;
}

bool OdqPropertyDialog::popLayout()
{
  if (m_stackLayouts.size() <= 1)
  {
    // there is nothing to remove excepty base internal vertical layout
    ODA_FAIL_ONCE(); // test
    return false;
  }
  m_stackLayouts.pop();
  return true;
}

QWidget* OdqPropertyDialog::addWidget(QWidget* pWidget,
                                      int stretch, // = 0
                                      Qt::Alignment alignment) // = 0
{
  QBoxLayout* pLayout = topLayout();
  if (!pLayout || !pWidget)
  {
    ODA_FAIL_ONCE();
    return NULL;
  }

  pLayout->addWidget(pWidget, stretch, alignment);
  return pWidget;
}

bool OdqPropertyDialog::removeWidget(QWidget* pWidget)
{
  QBoxLayout* pLayout = topLayout();
  if (!pLayout || !pWidget)
  {
    ODA_FAIL_ONCE();
    return false;
  }

  pLayout->removeWidget(pWidget);
  return true;
}

//bool OdqPropertyDialog::setTableHeaders(int numColumns, // = 2
//                                        const QStringList& lstLabels) // = QStringList()
//{
//  QBoxLayout* pLayout = topLayout();
//  if (!pLayout)
//  {
//    ODA_FAIL_ONCE();
//    return false;
//  }
//
//  if (m_pPropertyTable.isNull())
//  {
//    m_pPropertyTable = new QTableWidget;
//    pLayout->addWidget(m_pPropertyTable);
//  }
//
//  ODA_ASSERT_ONCE(numColumns > 0);
//  if (numColumns <= 0)
//    numColumns = 1;
//
//  int countLabels = lstLabels.size();
//  m_pPropertyTable->setColumnCount(numColumns);
//  m_pPropertyTable->verticalHeader()->setVisible(false);
//  if (numColumns == countLabels)
//    m_pPropertyTable->setHorizontalHeaderLabels(lstLabels);
//  else
//  {
//    ODA_ASSERT_ONCE(!countLabels);
//    m_pPropertyTable->horizontalHeader()->setVisible(false);
//  }
//  m_pPropertyTable->horizontalHeader()->setStretchLastSection(true);
//  m_pPropertyTable->setEditTriggers(QAbstractItemView::AllEditTriggers);
//
//  return true;
//}
//
//bool OdqPropertyDialog::addTableProperty(const QString& cqsName, const QVariant& vValue)
//{
//  if (m_pPropertyTable.isNull() && !setTableHeaders(cqsName.isEmpty() ? 1 : 2))
//    return false;
//
//  int indexValueColumn = m_pPropertyTable->columnCount() - 1;
//  ODA_ASSERT_ONCE(indexValueColumn >= 0);
//
//  int rowCount = m_pPropertyTable->rowCount();
//  m_pPropertyTable->setRowCount(rowCount + 1);
//
//  if (indexValueColumn)
//  {
//    QVariant::Type type = vValue.type();
//
//    QString qsName = cqsName;
//    if (qsName.isEmpty())
//      qsName = QLatin1String("argument ") + QString::number(rowCount + 1);
//    qsName += QLatin1String(" (");
//    qsName += QLatin1String(QVariant::typeToName(type));
//    qsName += QLatin1String(")");
//    QTableWidgetItem* pNameItem = new QTableWidgetItem(qsName);
//    pNameItem->setFlags(pNameItem->flags() & ~(Qt::ItemIsEditable | Qt::ItemIsSelectable));
//    m_pPropertyTable->setItem(rowCount, 0, pNameItem);
//  }
//  else
//    ODA_ASSERT_ONCE(cqsName.isEmpty());
//
//  QTableWidgetItem* pValueItem = new QTableWidgetItem;
//  pValueItem->setData(Qt::DisplayRole, vValue);
//  m_pPropertyTable->setItem(rowCount, indexValueColumn, pValueItem);
//  return true;
//}
//
//bool OdqPropertyDialog::addTableProperty(const QString& qsName, 
//                                         QVariant::Type type)
//{
//  return addTableProperty(qsName, QVariant(type));
//}
//
//QList<QVariant> OdqPropertyDialog::tableValues() const
//{
//  QList<QVariant> lstRes;
//  if (m_pPropertyTable.isNull())
//    return lstRes;
//
//  int indexValueColumn = m_pPropertyTable->columnCount() - 1;
//  ODA_ASSERT_ONCE(indexValueColumn >= 0);
//
//  for (int index = 0; index < m_pPropertyTable->rowCount(); index++)
//    lstRes << m_pPropertyTable->item(index, indexValueColumn)->data(Qt::EditRole);
//
//  return lstRes;
//}

int OdqPropertyDialog::exec()
{
  //if (!m_pPropertyTable.isNull())
  //{
  //  m_pPropertyTable->resizeColumnToContents(0);
  //  m_pPropertyTable->setFocus();
  //  m_pPropertyTable->setCurrentCell(0, m_pPropertyTable->columnCount() - 1);
  //}
  return QDialog::exec();
}

//////////////////////////////////////////////////////////////////////////

// http://www.qtcentre.org/threads/33593-How-can-i-have-a-QFileDialog-with-a-preview-of-the-picture

PreviewFileDialog::PreviewFileDialog(QWidget* parent,
                                     const QString & caption,
                                     const QString & directory,
                                     const QString & filter)
  : QFileDialog(parent, caption, directory, filter)
{
  this->setOption(QFileDialog::DontUseNativeDialog, true);
  setObjectName("PreviewFileDialog");
  QVBoxLayout* box = new QVBoxLayout();

  mpPreview = new QLabel(tr("Preview"), this);
  mpPreview->setAlignment(Qt::AlignCenter);
  mpPreview->setObjectName("labelPreview");
  mpPreview->setMinimumSize(512, 512);
  box->addWidget(mpPreview);
  box->addStretch();

  // add to QFileDialog layout
  QGridLayout *layout = static_cast<QGridLayout*>(this->layout());

  QList< QPair<QLayoutItem*, QList<int> > > movedItems;
  for (int i = 0; i < layout->count(); i++)
  {
    int row, column, rowSpan, columnSpan;
    layout->getItemPosition(i, &row, &column, &rowSpan, &columnSpan);
    if (row > 2)
    {
      QList<int> list;
      list << row << column << rowSpan << columnSpan;
      movedItems << qMakePair(layout->takeAt(i), list);
      i--;
    }
  }
  for (int i = 0; i < movedItems.count(); i++)
  {
    layout->addItem(movedItems[i].first,
      movedItems[i].second[0],
      movedItems[i].second[1],
      movedItems[i].second[2],
      movedItems[i].second[3]
    );
  }

  layout->addItem(box, 1, 3, 1, 1);
  connect(this, SIGNAL(currentChanged(const QString&)), this, SLOT(OnCurrentChanged(const QString&)));
}

void PreviewFileDialog::OnCurrentChanged(const QString & path)
{
  QPixmap pixmap = QPixmap(path);
  if (pixmap.isNull()) {
    mpPreview->setText("not an image");
  }
  else {
    mpPreview->setPixmap(pixmap.scaled(mpPreview->width(), mpPreview->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
  }
}
