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
// OdqPlotDialog.cpp
//

#include <QFileInfo>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QComboBox> 
#include <QPushButton>
#include <QLineEdit>
#include <QCheckBox>
#include <QSpinBox>
#include <QRadioButton>
#include <QBitmap>
#include <QPrinterInfo>
#include <QPrintEngine>
#include <QSettings>
#include <QIntValidator>
#include <QMessageBox>

#include "OdaCommon.h"
#include "DbBaseDatabase.h"
#include "DbDatabase.h"
#include "DbDictionary.h"
#include "DbHostAppServices.h"
#include "DbPlotSettingsValidator.h"
#include "DbPlotSettingsValidatorPE.h"
#include "DbViewTable.h"
#include "DbViewTableRecord.h"
#include "TDVersion.h"
#include "Ps/PlotStyles.h"
#include "DbUnitsFormatter.h"

#include "OdqPlotDialog.h" 
#include "OdqPropertyDialog.h"
#include "OdqMainWindow.h"
#include "OdqApplication.h"

//////////////////////////////////////////////////////////////////////////

static QString AreaTypeToString(OdDbPlotSettings::PlotType type)
{
  static QMap<OdDbPlotSettings::PlotType, QString> s_map;
  if (s_map.isEmpty())
  {
    s_map[OdDbPlotSettings::kDisplay] = QObject::tr("Display");
    s_map[OdDbPlotSettings::kExtents] = QObject::tr("Extents");
    s_map[OdDbPlotSettings::kLimits] = QObject::tr("Limits");
    s_map[OdDbPlotSettings::kView] = QObject::tr("View");
    s_map[OdDbPlotSettings::kWindow] = QObject::tr("Window");
    s_map[OdDbPlotSettings::kLayout] = QObject::tr("Layout");
  }

  QMap<OdDbPlotSettings::PlotType, QString>::Iterator itr = s_map.find(type);
  if (itr == s_map.end())
  {
    ODA_FAIL_ONCE();
    return QString();
  }

  QString qsValue = itr.value();
  return qsValue;
}

static QString ScaleTypeToString(OdDbPlotSettings::StdScaleType type, // 75 (dxf)
                                 int idx = 0)
{
  ODA_ASSERT_ONCE(   type >= OdDbPlotSettings::kScaleToFit 
                  && type <= OdDbPlotSettings::k1and1_2in_1ft
                  && idx >= 0);

  static QMap<OdDbPlotSettings::StdScaleType, QString> s_map;
  if (s_map.isEmpty())
  {
          // =   name 
          //   + ; enPaperScaleUnits (numerator 142(dxf)) 
          //   + ; enDrawingScaleUnits (denominator  143(dxf))
          //   + ; enScaleFactor (147(dxf))
    s_map[OdDbPlotSettings::kScaleToFit] = QObject::tr("Custom") + ";1;1;1";
    s_map[OdDbPlotSettings::k1_128in_1ft] = QObject::tr("1/128\" = 1'") + ";1;1536;0.0006510416666667";
    s_map[OdDbPlotSettings::k1_64in_1ft] = QObject::tr("1/64\" = 1'") + ";1;768;0.0013020833333333";
    s_map[OdDbPlotSettings::k1_32in_1ft] = QObject::tr("1/32\" = 1'") + ";1;384;0.0026041666666667";
    s_map[OdDbPlotSettings::k1_16in_1ft] = QObject::tr("1/16\" = 1'") + ";1;192;0.0052083333333333";
    s_map[OdDbPlotSettings::k3_32in_1ft] = QObject::tr("3/32\" = 1'") + ";1;128;0.0078125";
    s_map[OdDbPlotSettings::k1_8in_1ft] = QObject::tr("1/8\" = 1'") + ";1;960.0104166666666667";
    s_map[OdDbPlotSettings::k3_16in_1ft] = QObject::tr("3/16\" = 1'") + ";1;64;0.015625";
    s_map[OdDbPlotSettings::k1_4in_1ft] = QObject::tr("1/4\" = 1'") + ";1;48;0.0208333333333333";
    s_map[OdDbPlotSettings::k3_8in_1ft] = QObject::tr("3/8\" = 1'") + ";1;32;0.03125";
    s_map[OdDbPlotSettings::k1_2in_1ft] = QObject::tr("1/2\" = 1'") + ";1;24;0.0416666666666667}";
    s_map[OdDbPlotSettings::k3_4in_1ft] = QObject::tr("3/4\" = 1'") + ";1;16;0.0625";
    s_map[OdDbPlotSettings::k1in_1ft] = QObject::tr("1\" = 1'") + ";1;12;0.0833333333333333";
    s_map[OdDbPlotSettings::k3in_1ft] = QObject::tr("3\" = 1'") + ";1;4;0.25";
    s_map[OdDbPlotSettings::k6in_1ft] = QObject::tr("6\" = 1'") + ";1;2;0.5";
    s_map[OdDbPlotSettings::k1ft_1ft] = QObject::tr("1' = 1'") + ";1;1;1";
    s_map[OdDbPlotSettings::k1_1] = QObject::tr("1:1") + ";1;1;1";
    s_map[OdDbPlotSettings::k1_2] = QObject::tr("1:2") + ";1;2;0.5";
    s_map[OdDbPlotSettings::k1_4] = QObject::tr("1:4") + ";1;4;0.25";
    s_map[OdDbPlotSettings::k1_5] = QObject::tr("1:5") + ";1;5;0.2";
    s_map[OdDbPlotSettings::k1_8] = QObject::tr("1:8") + ";1;8;0.125";
    s_map[OdDbPlotSettings::k1_10] = QObject::tr("1:10") + ";1;10;0.1";
    s_map[OdDbPlotSettings::k1_16] = QObject::tr("1:16") + ";1;16;0.0625";
    s_map[OdDbPlotSettings::k1_20] = QObject::tr("1:20") + ";1;20;0.05";
    s_map[OdDbPlotSettings::k1_30] = QObject::tr("1:30") + ";1;30;0.03333333333333";
    s_map[OdDbPlotSettings::k1_40] = QObject::tr("1:40") + ";1;40;0.025";
    s_map[OdDbPlotSettings::k1_50] = QObject::tr("1:50") + ";1;50;0.2";
    s_map[OdDbPlotSettings::k1_100] = QObject::tr("1:100") + ";1;100;0.01";
    s_map[OdDbPlotSettings::k2_1] = QObject::tr("2:1") + ";2;1;2";
    s_map[OdDbPlotSettings::k4_1] = QObject::tr("4:1") + ";4;1;4";
    s_map[OdDbPlotSettings::k8_1] = QObject::tr("8:1") + ";8;1;8";
    s_map[OdDbPlotSettings::k10_1] = QObject::tr("10:1") + ";10;1;10";
    s_map[OdDbPlotSettings::k100_1] = QObject::tr("100:1") + ";100;1;100";
    s_map[OdDbPlotSettings::k1000_1] = QObject::tr("1000:1") + ";1000;1;1000";
    s_map[OdDbPlotSettings::k1and1_2in_1ft] = QObject::tr("1-1/2\" = 1'") + ";1.5;12;0.125";
  }

  QMap<OdDbPlotSettings::StdScaleType, QString>::Iterator itr = s_map.find(type);
  if (itr == s_map.end())
  {
    //ODA_FAIL_ONCE();
    return QString();
  }

  QStringList lstValues = itr.value().split(";");
  if (idx >= lstValues.size() || idx < 0)
  {
    ODA_FAIL_ONCE();
    return QString();
  }

  QString qsValue = lstValues.at(idx);
  return qsValue;
}

typedef enum {
  enPaperScaleUnits,
  enDrawingScaleUnits,
  enScaleFactor
} TScaleDataValue;
static double ScaleTypeToValue(OdDbPlotSettings::StdScaleType type,
                               TScaleDataValue typeValue)
{
  QString qsData = ScaleTypeToString(type, typeValue + 1);
  bool bOk = false;
  double dValue = 0.0;
  if (   qsData.isEmpty()
      || (dValue = qsData.toDouble(&bOk), !bOk))
  {
    ODA_FAIL_ONCE();
    return 0.0;
  }
  return dValue;
}


static QString PaperUnitsToString(OdDbPlotSettings::PlotPaperUnits ppu)
{
  switch (ppu)
  {
  case OdDbPlotSettings::kInches:
    return QObject::tr("inches");
  case OdDbPlotSettings::kMillimeters:
    return QObject::tr("mm");
  case OdDbPlotSettings::kPixels:
    return QObject::tr("pixels");
  }

  ODA_FAIL_ONCE(); // TODO
  return "";
}

//////////////////////////////////////////////////////////////////////////
// after class PropertyDialog: public QDialog

OdqPlotDialog::OdqPlotDialog(OdRxObject* pRxDatabase,
                             QWidget* pParent, // = NULL
                             Qt::WindowFlags flags) // = 0
  : QDialog(pParent, flags)
  , m_pRxDatabase(pRxDatabase)
  , m_isAppliedToLayout(false)
{
  OdDbBaseDatabasePEPtr pDbPE = OdDbBaseDatabasePE::cast(pRxDatabase);
  ODA_ASSERT_ONCE(!pDbPE.isNull());

  //QString qsFileName = getIApp()->toQString(getIConsole()->getFilePathName(pRxDatabase)); 
  //qsFileName = QDir::toNativeSeparators(QFileInfo(qsFileName).fileName());

  OdRxObjectPtr pCurrLayout = pDbPE->currentLayout(pRxDatabase);
  if (pCurrLayout.isNull())
    FIX_MAC_APP_TROW(OdError(eNotApplicable));

  OdDbBaseLayoutPEPtr pLayoutInfo(pCurrLayout);
  ODA_ASSERT_ONCE(!pLayoutInfo.isNull());

  QString qsCurrLayoutName = getIApp()->toQString(pLayoutInfo->name(pCurrLayout));
  //bool isModel = pLayoutInfo->isModelLayout(pCurrLayout);

  m_pPlotSettings = OdDbPlotSettings::createObject();
  //  already done via create //if (!m_pPlotSettings->isWriteEnabled()) m_pPlotSettings->upgradeOpen();

 #if !defined(Q_OS_WIN32) && TD_MAJOR_VERSION <= 3 && TD_MINOR_VERSION <= 4 && TD_MAJOR_BUILD == 0
  // temp solution for IPF in destructor in v 3.4(non-Windows)
  OdDbDatabase::cast(pRxDatabase)->addOdDbObject(m_pPlotSettings);
 #endif
  
  m_pPlotSettings->copyFrom(pCurrLayout);

  setWindowTitle(QObject::tr("Plot - %1").arg(qsCurrLayoutName));

  OdqMainWindow* pMainWindow = qobject_cast<OdqMainWindow*>(getIApp()->getMainWindow());
  ODA_ASSERT_ONCE(pMainWindow);
  pMainWindow->restoreWidgetPlacement(this, "Plot", 
                                      650, 650); // #8775
  setMinimumSize(650, 650);
 #if defined(Q_OS_LINUX) && QT_VERSION <= 0x040503 
  setMaximumWidth(650 * 3 / 2);
 #endif

  bool bExtended = false;
  {
    QSettings settings("ODA", 
      QFileInfo(QCoreApplication::applicationFilePath()).baseName());
    bExtended = settings.value(TD_SHORT_STRING_VER_S "/Plot/extended", false).toBool();
  }

  QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  sizePolicy.setHorizontalStretch(0);
  sizePolicy.setVerticalStretch(0);

  QVBoxLayout* pLayout = new QVBoxLayout;
  setLayout(pLayout);
  QHBoxLayout* pHLayout = new QHBoxLayout;
  pLayout->addLayout(pHLayout);

  m_pLeftWidget = new QWidget; // extended
  pHLayout->addWidget(m_pLeftWidget);
  QGridLayout* pLeftLayout = new QGridLayout;
  m_pLeftWidget->setLayout(pLeftLayout);

  m_pRightWidget = new QWidget; // extended
  pHLayout->addWidget(m_pRightWidget);
  if (!bExtended)
    m_pRightWidget->hide();
  QVBoxLayout* pRightLayout = new QVBoxLayout;
  m_pRightWidget->setLayout(pRightLayout);

  // ===== on pLeftLayout : =====

  // --- PageSetup ---

  QGroupBox* pPageSetupGroupBox = new QGroupBox(tr("Page Setup")); // title
  pLeftLayout->addWidget(pPageSetupGroupBox, 0, 0, 1, 4);

  QGridLayout* pPageSetupHorLayout = new QGridLayout; // QHBoxLayout
  pPageSetupGroupBox->setLayout(pPageSetupHorLayout);
  //pPageSetupHorLayout->setMargin(4);

  QLabel* pPageSetupLabel_Name = new QLabel(QObject::tr("Name:"));
  pPageSetupHorLayout->addWidget(pPageSetupLabel_Name, 0, 0);

  m_pPageSetupCombo = new QComboBox;
  pPageSetupHorLayout->addWidget(m_pPageSetupCombo, 0, 1);
  sizePolicy.setHeightForWidth(m_pPageSetupCombo->sizePolicy().hasHeightForWidth());
  m_pPageSetupCombo->setSizePolicy(sizePolicy);
  m_pPageSetupCombo->setInsertPolicy(QComboBox::InsertAlphabetically);
  pPageSetupLabel_Name->setBuddy(m_pPageSetupCombo);

  QPushButton* pPageSetupButton_Add = new QPushButton(QObject::tr("Add..."));
  pPageSetupHorLayout->addWidget(pPageSetupButton_Add, 0, 2);

  connect(m_pPageSetupCombo, SIGNAL(currentIndexChanged(const QString&)), 
          this, SLOT(PageSetup_currentIndexChanged(const QString&)));

  connect(pPageSetupButton_Add, SIGNAL(clicked()), 
          this, SLOT(PageSetupButtonAdd_clicked()));

  // --- PrinterPlotter ---

  QGroupBox* pPrinterGroupBox = new QGroupBox(tr("Printer/plotter"));
  pLeftLayout->addWidget(pPrinterGroupBox, 1, 0, 1, 4);

  QGridLayout* pPrinterGridLayout = new QGridLayout; // (pPrinterGroupBox);
  pPrinterGroupBox->setLayout(pPrinterGridLayout); 

  QLabel* pPrinterLabel_Name = new QLabel(QObject::tr("Name:"));
  pPrinterGridLayout->addWidget(pPrinterLabel_Name, 0, 0);

  m_pPrinterCombo = new QComboBox;
  sizePolicy.setHeightForWidth(m_pPrinterCombo->sizePolicy().hasHeightForWidth());
  m_pPrinterCombo->setSizePolicy(sizePolicy);
  pPrinterGridLayout->addWidget(m_pPrinterCombo, 0, 1);
  pPrinterLabel_Name->setBuddy(m_pPrinterCombo);

  m_pPrinter_PropertiesButton = new QPushButton(tr("Properties..."));
  pPrinterGridLayout->addWidget(m_pPrinter_PropertiesButton, 0, 2);
  m_pPrinter_PropertiesButton->setEnabled(false);

  QLabel* pPrinterLabel_Plotter = new QLabel(tr("Plotter:"));
  pPrinterGridLayout->addWidget(pPrinterLabel_Plotter, 1, 0);

  //QSize sizeInfo = m_pPrinterCombo->minimumSize(); // (width / 2, 16)
  //QSize sizeInfo2(sizeInfo.width(), sizeInfo.height() * 2);

  m_pPrinter_ConfigurationName = new QLabel(tr("None"));
  pPrinterGridLayout->addWidget(m_pPrinter_ConfigurationName, 1, 1, 1, 2);
  //m_pPrinter_ConfigurationName->setMinimumSize(sizeInfo);
  //m_pPrinter_ConfigurationName->setMaximumSize(sizeInfo);

  QLabel* pPrinterLabel_Where = new QLabel(tr("Where:"));
  pPrinterGridLayout->addWidget(pPrinterLabel_Where, 2, 0);

  m_pPrinter_Placement = new QLabel(tr("Not applicable"));
  pPrinterGridLayout->addWidget(m_pPrinter_Placement, 2, 1, 1, 2);
  //m_pPrinter_Placement->setText(...);

  QLabel* pPrinterLabel_Description = new QLabel(tr("Description:"));
  pPrinterGridLayout->addWidget(pPrinterLabel_Description, 3, 0, 2, 1);

  m_pPrinter_Description = new QLabel(
    tr("The layout will not be plotted unless a new plotter configuration name is selected."));
  pPrinterGridLayout->addWidget(m_pPrinter_Description, 3, 1, 2, 2);
  //m_pPrinter_Description->setMinimumSize(sizeInfo2); 
  //m_pPrinter_Description->setMaximumSize(sizeInfo2);
  m_pPrinter_Description->setWordWrap(true);

  // TODO
  m_pPrinter_ToFileCheckBox = new QCheckBox(tr("Plot to file"));
  pPrinterGridLayout->addWidget(m_pPrinter_ToFileCheckBox, 5, 0);
  //m_pPrinter_ToFileCheckBox->setVisible(false);
  m_pPrinter_ToFileCheckBox->setEnabled(false);

  connect(m_pPrinterCombo, SIGNAL(currentIndexChanged(const QString&)), 
          this, SLOT(PrinterPlotter_currentIndexChanged(const QString&)));

  // ----- PaperSize -----

  QGroupBox* pPaperSizeGroupBox = new QGroupBox(tr("Paper size"));
  pLeftLayout->addWidget(pPaperSizeGroupBox, 2, 0, 1, 3);
  QVBoxLayout* pPaperSizeLayout = new QVBoxLayout;
  pPaperSizeGroupBox ->setLayout(pPaperSizeLayout); 

  m_pPaperSizeCombo = new QComboBox;
  pPaperSizeLayout->addWidget(m_pPaperSizeCombo); 
  sizePolicy.setHeightForWidth(m_pPaperSizeCombo->sizePolicy().hasHeightForWidth());
  m_pPaperSizeCombo->setSizePolicy(sizePolicy);
  m_pPaperSizeCombo->setInsertPolicy(QComboBox::InsertAlphabetically); //QComboBox::InsertAtBottom
  //m_pPaperSizeCombo->setSizeAdjustPolicy(QComboBox::AdjustToContents);

  connect(m_pPaperSizeCombo, SIGNAL(currentIndexChanged(const QString&)), 
          this, SLOT(PaperSizes_currentIndexChanged(const QString&)));

  // ----- NumberOfCopies -----

  QGroupBox* pNumberOfCopiesGroupBox = new QGroupBox(tr("Number of copies"));
  pLeftLayout->addWidget(pNumberOfCopiesGroupBox, 2, 3);
  QVBoxLayout* pNumberOfCopiesLayout = new QVBoxLayout;
  pNumberOfCopiesGroupBox->setLayout(pNumberOfCopiesLayout); 

  m_pNumberOfCopiesSpin = new QSpinBox;
  pNumberOfCopiesLayout->addWidget(m_pNumberOfCopiesSpin); 
  //sizePolicy.setHeightForWidth(m_pNumberOfCopiesSpin->sizePolicy().hasHeightForWidth());
  //m_pNumberOfCopiesSpin->setSizePolicy(sizePolicy);
  m_pNumberOfCopiesSpin->setMinimum(1);

  // ----- PlotArea -----

  QGroupBox* pPlotAreaGroupBox = new QGroupBox(tr("Plot area"));
  pLeftLayout->addWidget(pPlotAreaGroupBox, 3, 0, 1, 2);

  QGridLayout* pPlotAreaLayout = new QGridLayout;
  pPlotAreaGroupBox->setLayout(pPlotAreaLayout); 
  
  QLabel* pPlotAreaLabel = new QLabel(tr("What to plot:"));
  pPlotAreaLayout->addWidget(pPlotAreaLabel, 0, 0, 1, 2);

  m_pPlotAreaCombo = new QComboBox;
  pPlotAreaLayout->addWidget(m_pPlotAreaCombo, 1, 0);
  m_pPlotAreaCombo->addItem(tr("Display"));
  m_pPlotAreaCombo->addItem(tr("Extents")); // TODO : depends on entities existance
  m_pPlotAreaCombo->addItem(tr(m_pPlotSettings->modelType() ? "Limits" : "Layout"));
  OdDbDatabasePtr pDb = OdDbDatabase::cast(m_pRxDatabase);
  if (!pDb.isNull())
  {
    OdDbViewTablePtr pViewTable =  pDb->getViewTableId().safeOpenObject();
    for (OdDbSymbolTableIteratorPtr pIt = pViewTable->newIterator();
         !pIt->done(); pIt->step())
    {
      OdDbViewTableRecordPtr pView = pIt->getRecord();
      if (pView->isPaperspaceView() == m_pPlotSettings->modelType())
        continue;

      if (m_pPlotAreaViewsCombo.isNull())
      {
        m_pPlotAreaCombo->addItem(tr("View"));
        m_pPlotAreaViewsCombo = new QComboBox;
        pPlotAreaLayout->addWidget(m_pPlotAreaViewsCombo, 1, 1);
        m_pPlotAreaViewsCombo->hide();
      }

      QString qsViewName = getIApp()->toQString(pView->getName());
      m_pPlotAreaViewsCombo->addItem(qsViewName);
    }
  }
  m_pPlotAreaCombo->addItem(tr("Window"));
  pPlotAreaLabel->setBuddy(m_pShadePlotCombo);

  m_pPlotAreaWindowButton = new QPushButton(tr("Window<"));
  pPlotAreaLayout->addWidget(m_pPlotAreaWindowButton, 1, 1);
  //m_pPlotAreaWindowButton->setMinimumWidth(40);
  m_pPlotAreaWindowButton->hide();

  connect(m_pPlotAreaCombo, SIGNAL(currentIndexChanged(const QString&)), 
          this, SLOT(PlotArea_currentIndexChanged(const QString&)));
  if (!m_pPlotAreaViewsCombo.isNull())
    connect(m_pPlotAreaViewsCombo, SIGNAL(currentIndexChanged(const QString&)), 
            this, SLOT(PlotAreaView_currentIndexChanged(const QString&)));
  connect(m_pPlotAreaWindowButton, SIGNAL(clicked()), 
          this, SLOT(PlotAreaWindowButton_clicked()));

  // ----- PlotScale -----

  QGroupBox* pPlotScaleGroupBox = new QGroupBox(tr("Plot Scale"));
  pLeftLayout->addWidget(pPlotScaleGroupBox, 3, 2, 2, 2);

  QGridLayout* pPlotScaleLayout = new QGridLayout;
  pPlotScaleGroupBox->setLayout(pPlotScaleLayout); 

  m_pFitToPaperCheckBox = new QCheckBox(tr("Fit to paper"));
  pPlotScaleLayout->addWidget(m_pFitToPaperCheckBox, 0, 0, 1, 4);

  QLabel* pUnitScaleLabel = new QLabel(tr("Scale:"));
  pPlotScaleLayout->addWidget(pUnitScaleLabel, 1, 0);

  m_pScaleCombo = new QComboBox;
  pPlotScaleLayout->addWidget(m_pScaleCombo, 1, 1, 1, 3);
  ODA_ASSERT_ONCE(!ScaleTypeToString(OdDbPlotSettings::kScaleToFit).isEmpty());
  m_pScaleCombo->addItem(ScaleTypeToString(OdDbPlotSettings::kScaleToFit));
  int index = 0;
  for (index = OdDbPlotSettings::k1_1; index <= OdDbPlotSettings::k1and1_2in_1ft; index++)
  {
    QString qsType = ScaleTypeToString(OdDbPlotSettings::StdScaleType(index));
    if (!qsType.isEmpty())
      m_pScaleCombo->addItem(qsType);
  }
  for (index = OdDbPlotSettings::k1_128in_1ft; index <= OdDbPlotSettings::k1ft_1ft; index++)
  {
    QString qsType = ScaleTypeToString(OdDbPlotSettings::StdScaleType(index));
    if (!qsType.isEmpty())
      m_pScaleCombo->addItem(qsType);
  }
  pUnitScaleLabel->setBuddy(m_pScaleCombo);

  m_pUnitPaperEdit = new QLineEdit;
  pPlotScaleLayout->addWidget(m_pUnitPaperEdit, 2, 1);

  m_pUnitCombo = new QComboBox;
  pPlotScaleLayout->addWidget(m_pUnitCombo, 2, 2);
  m_pUnitCombo->addItems(QStringList() << tr("inches") << tr("mm"));

  QLabel* pUnitIsEqualLabel = new QLabel(tr("="));
  pPlotScaleLayout->addWidget(pUnitIsEqualLabel, 2, 3);
  pUnitIsEqualLabel->setBuddy(m_pUnitCombo);

  m_pUnitDrawingEdit = new QLineEdit;
  pPlotScaleLayout->addWidget(m_pUnitDrawingEdit, 3, 1);

  m_pUnitDrawingLabel = new QLabel(tr("unit"));
  pPlotScaleLayout->addWidget(m_pUnitDrawingLabel, 3, 2);
  m_pUnitDrawingLabel->setBuddy(m_pUnitDrawingEdit);

  m_pScaleLineweightsCheckBox = new QCheckBox(tr("Scale lineweights"));
  pPlotScaleLayout->addWidget(m_pScaleLineweightsCheckBox, 4, 1);

  connect(m_pFitToPaperCheckBox, SIGNAL(toggled(bool)), 
          this, SLOT(PlotScaleFitToPaper_changed()));
  connect(m_pScaleCombo, SIGNAL(currentIndexChanged(const QString&)), 
          this, SLOT(PlotScale_currentIndexChanged(const QString&)));
  connect(m_pUnitPaperEdit, SIGNAL(editingFinished()), 
          this, SLOT(Unit_edited()));
  connect(m_pUnitDrawingEdit, SIGNAL(editingFinished()), 
          this, SLOT(Unit_edited()));
  connect(m_pUnitCombo, SIGNAL(currentIndexChanged(const QString&)), 
          this, SLOT(PlotUnit_currentIndexChanged(const QString&)));
  connect(m_pScaleLineweightsCheckBox, SIGNAL(toggled(bool)), 
          this, SLOT(PlotScaleLineweights_changed()));

  // ----- PlotOffset -----

  QGroupBox* pPlotOffsetGroupBox = new QGroupBox(tr("Plot offset (origin set to printable area)"));
  pLeftLayout->addWidget(pPlotOffsetGroupBox, 4, 0, 1, 2);
  
  QGridLayout* pPlotOffsetLayout = new QGridLayout;
  pPlotOffsetGroupBox->setLayout(pPlotOffsetLayout); 

  QLabel* pPlotOffsetXLabel = new QLabel(tr("X:"));
  pPlotOffsetLayout->addWidget(pPlotOffsetXLabel, 0, 0);

  m_pPlotOffsetXEdit = new QLineEdit;
  pPlotOffsetLayout->addWidget(m_pPlotOffsetXEdit, 0, 1);
  m_pPlotOffsetXEdit->setMinimumWidth(80); // is usefull after switch ti inces
  pPlotOffsetXLabel->setBuddy(m_pPlotOffsetXEdit);

  m_pPlotOffsetUnitXLabel = new QLabel(tr("mm"));
  pPlotOffsetLayout->addWidget(m_pPlotOffsetUnitXLabel, 0, 2);

  QLabel* pPlotOffsetYLabel = new QLabel(tr("Y:"));
  pPlotOffsetLayout->addWidget(pPlotOffsetYLabel, 1, 0);
  
  m_pPlotOffsetYEdit = new QLineEdit;
  pPlotOffsetLayout->addWidget(m_pPlotOffsetYEdit, 1, 1);
  //m_pPlotOffsetYEdit->setMinimumWidth(80);
  pPlotOffsetYLabel->setBuddy(m_pPlotOffsetYEdit);

  m_pPlotOffsetUnitYLabel = new QLabel(tr("mm"));
  pPlotOffsetLayout->addWidget(m_pPlotOffsetUnitYLabel, 1, 2);

  m_pCenterThePlotCheckBox = new QCheckBox(tr("Center the plot"));
  pPlotOffsetLayout->addWidget(m_pCenterThePlotCheckBox, 0, 3);

  connect(m_pPlotOffsetXEdit, SIGNAL(editingFinished()), 
          this, SLOT(PlotOffset_edited()));
  connect(m_pPlotOffsetYEdit, SIGNAL(editingFinished()), 
          this, SLOT(PlotOffset_edited()));
  connect(m_pCenterThePlotCheckBox, SIGNAL(toggled(bool)), 
          this, SLOT(PlotOffset_changed()));

  // ===== on pRightLayout : =====

  // ----- PlotStyleTable -----

  QGroupBox* pPlotStyleTableGroupBox = new QGroupBox(tr("Plot style table (pen assignments)"));
  pRightLayout->addWidget(pPlotStyleTableGroupBox);
  //QGridLayout* pPlotStyleTableLayout = new QGridLayout;
  QHBoxLayout* pPlotStyleTableLayout = new QHBoxLayout;
  pPlotStyleTableGroupBox->setLayout(pPlotStyleTableLayout);
  
  m_pPlotStyleFilesCombo = new QComboBox;
  //pPlotStyleTableLayout->addWidget(m_pPlotStyleFilesCombo, 0, 0); 
  pPlotStyleTableLayout->addWidget(m_pPlotStyleFilesCombo); 
  sizePolicy.setHeightForWidth(m_pPlotStyleFilesCombo->sizePolicy().hasHeightForWidth());
  m_pPlotStyleFilesCombo->setSizePolicy(sizePolicy);
  m_pPlotStyleFilesCombo->setInsertPolicy(QComboBox::InsertAtBottom); // QComboBox::InsertAlphabetically);
  m_pPlotStyleFilesCombo->addItem(tr("None"));

  QPixmap pixmap(":/images/plot/style.png");
  pixmap.setMask(pixmap.createHeuristicMask());
  m_pPlotStyleEditButton = new QPushButton(QIcon(pixmap), ""); // (tr("Edit"));
  //pPlotStyleTableLayout->addWidget(m_pPlotStyleEditButton, 0, 1);
  pPlotStyleTableLayout->addWidget(m_pPlotStyleEditButton);
  m_pPlotStyleEditButton->setToolTip(tr("Edit..."));
  m_pPlotStyleFilesCombo->adjustSize(); //layout()->activate();
  int sz = m_pPlotStyleFilesCombo->height();
  if (!sz || sz > 50)
    sz = 20;
  m_pPlotStyleEditButton->setMinimumSize(sz, sz);
  m_pPlotStyleEditButton->setMaximumSize(sz, sz);

  // missing in new ACAD
  //m_pDisplayPlotStylesCheckBox = new QCheckBox(tr("Display plot styles"));
  //pPlotStyleTableLayout->addWidget(m_pDisplayPlotStylesCheckBox, 1, 0); 

  connect(m_pPlotStyleFilesCombo, SIGNAL(activated(const QString&)), // SIGNAL(currentIndexChanged(const QString&)), 
          this, SLOT(PlotStyles_currentIndexChanged(const QString&)));
  connect(m_pPlotStyleEditButton, SIGNAL(clicked()), 
          this, SLOT(PlotStyleEditButton_clicked()));
  //connect(m_pDisplayPlotStylesCheckBox, SIGNAL(toggled(bool)), 
  //        this, SLOT(PlotStylesDisplay_changed()));

  // ----- ShadedViewportOptions -----

  QGroupBox* pViewportOptionsGroupBox = new QGroupBox(tr("Shaded viewport options"));
  pRightLayout->addWidget(pViewportOptionsGroupBox);

  QGridLayout* pViewportOptionsLayout = new QGridLayout;
  pViewportOptionsGroupBox->setLayout(pViewportOptionsLayout);

  QLabel* pShadePlotLabel = new QLabel(tr("Shade plot"));
  pViewportOptionsLayout->addWidget(pShadePlotLabel, 0, 0);

  m_pShadePlotCombo = new QComboBox;
  pViewportOptionsLayout->addWidget(m_pShadePlotCombo, 0, 1);
  m_pShadePlotCombo->addItem(tr("As displayed"));
  m_pShadePlotCombo->addItem(tr("Wireframe"));
  m_pShadePlotCombo->addItem(tr("Hidden"));
  m_pShadePlotCombo->addItem(tr("Rendered"));
  m_pShadePlotCombo->setCurrentIndex(0);
  pShadePlotLabel->setBuddy(m_pShadePlotCombo);

  QLabel* pQualityQLabel = new QLabel(tr("Quality"));
  pViewportOptionsLayout->addWidget(pQualityQLabel, 1, 0);

  m_pQualityCombo = new QComboBox;
  pViewportOptionsLayout->addWidget(m_pQualityCombo, 1, 1);
  m_pQualityCombo->addItem(tr("Draft"));
  m_pQualityCombo->addItem(tr("Preview"));
  m_pQualityCombo->addItem(tr("Normal"));
  m_pQualityCombo->addItem(tr("Presentation"));
  m_pQualityCombo->addItem(tr("Maximum"));
  m_pQualityCombo->addItem(tr("Custom"));
  m_pQualityCombo->setCurrentIndex(0);
  pQualityQLabel->setBuddy(m_pQualityCombo);

  QLabel* pDpiLabel = new QLabel(tr("DPI"));
  pViewportOptionsLayout->addWidget(pDpiLabel, 2, 0);

  m_pDpiEdit = new QLineEdit;
  pViewportOptionsLayout->addWidget(m_pDpiEdit, 2, 1);
  m_pDpiEdit->setValidator(new QIntValidator(100, 600, m_pDpiEdit));
  pDpiLabel->setBuddy(m_pDpiEdit);

  connect(m_pShadePlotCombo, SIGNAL(currentIndexChanged(int)), 
          this, SLOT(Shade_currentIndexChanged(int)));

  connect(m_pQualityCombo, SIGNAL(currentIndexChanged(int)), 
          this, SLOT(Quality_currentIndexChanged(int)));

  //connect(m_pQualityCombo, SIGNAL(editingFinished()), 
  //        this, SLOT(DpiEdit_edited()));
  // as way to fix missing signal editingFinished (? side effect of QIntValidator)
  connect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)),
          this, SLOT(focusChanged(QWidget*,QWidget*)));

  // ----- PlotOptions -----

  QGroupBox* pPlotOptionsGroupBox = new QGroupBox(tr("Plot options"));
  pRightLayout->addWidget(pPlotOptionsGroupBox);

  QVBoxLayout* pPlotOptionsLayout = new QVBoxLayout;
  pPlotOptionsGroupBox->setLayout(pPlotOptionsLayout);

  //m_pPlotInBackgroundCheckBox = new QCheckBox(tr("Plot in Background"));
  //pPlotOptionsLayout->addWidget(m_pPlotInBackgroundCheckBox);
  //m_pPlotInBackgroundCheckBox->setCheckState(Qt::Unchecked);
  //m_pPlotInBackgroundCheckBox->setEnabled(false); // TODO

  m_pPlotObjectLineweightsCheckBox = new QCheckBox(tr("Plot object lineweights"));
  pPlotOptionsLayout->addWidget(m_pPlotObjectLineweightsCheckBox);

  m_pPlotWithPlotStylesCheckBox = new QCheckBox(tr("Plot with plot styles"));
  pPlotOptionsLayout->addWidget(m_pPlotWithPlotStylesCheckBox);

  m_pPlotPaperspaceLastCheckBox = new QCheckBox(tr("Plot paperspace last"));
  pPlotOptionsLayout->addWidget(m_pPlotPaperspaceLastCheckBox);

  m_pHidePaperspaceObjectsCheckBox = new QCheckBox(tr("Hide Paperspace objects"));
  pPlotOptionsLayout->addWidget(m_pHidePaperspaceObjectsCheckBox);

  //m_pPlotStampOnCheckBox = new QCheckBox(tr("Plot stamp on"));
  //pPlotOptionsLayout->addWidget(m_pPlotStampOnCheckBox);
  //pPlotStampOnCheckBox->setCheckState(Qt::Unchecked);
  //pPlotStampOnCheckBox->setEnabled(false); // TODO

  m_pSaveChangesToLayoutCheckBox = new QCheckBox(tr("Save changes to layout"));
  pPlotOptionsLayout->addWidget(m_pSaveChangesToLayoutCheckBox);

  ////connect(m_pPlotInBackgroundCheckBox, SIGNAL(toggled(bool)),
  ////        this, SLOT(PlotOptions_changed()));
  connect(m_pPlotObjectLineweightsCheckBox, SIGNAL(toggled(bool)),
          this, SLOT(PlotOptions_changed()));
  connect(m_pPlotWithPlotStylesCheckBox, SIGNAL(toggled(bool)),
          this, SLOT(PlotOptions_changed()));
  connect(m_pPlotPaperspaceLastCheckBox, SIGNAL(toggled(bool)),
          this, SLOT(PlotOptions_changed()));
  connect(m_pHidePaperspaceObjectsCheckBox, SIGNAL(toggled(bool)),
          this, SLOT(PlotOptions_changed()));
  ////connect(m_pPlotStampOnCheckBox, SIGNAL(toggled(bool)),
  ////        this, SLOT(PlotOptions_changed()));
  //connect(m_pSaveChangesToLayoutCheckBox, SIGNAL(toggled(bool)),
  //        this, SLOT(PlotOptions_changed()));

  // ----- DrawingOrientation -----

  QGroupBox* pDrawingOrientationGroupBox = new QGroupBox(tr("Drawing orientation"));
  pRightLayout->addWidget(pDrawingOrientationGroupBox);
  
  QGridLayout* pDrawingOrientationLayout = new QGridLayout;
  pDrawingOrientationGroupBox->setLayout(pDrawingOrientationLayout);

  m_pPortraitRadio = new QRadioButton(tr("Portrait"));
  pDrawingOrientationLayout->addWidget(m_pPortraitRadio, 0, 0);
  m_pLandscapeRadio = new QRadioButton(tr("Landscape"));
  pDrawingOrientationLayout->addWidget(m_pLandscapeRadio, 1, 0);

  m_pUpsideDownCheckBox = new QCheckBox(tr("Plot upside-Down"));
  pDrawingOrientationLayout->addWidget(m_pUpsideDownCheckBox , 2, 0);

  connect(m_pPortraitRadio, SIGNAL(toggled(bool)),
          this, SLOT(DrawingOrientation_changed()));
  connect(m_pUpsideDownCheckBox, SIGNAL(toggled(bool)),
          this, SLOT(DrawingOrientation_changed()));

  m_pOrientationLabel = new QLabel;
  pDrawingOrientationLayout->addWidget(m_pOrientationLabel, 0, 1, 2, 1);
  sizePolicy.setHeightForWidth(m_pOrientationLabel->sizePolicy().hasHeightForWidth());
  m_pOrientationLabel->setSizePolicy(sizePolicy);
  m_pOrientationLabel->setAlignment(  Qt::AlignCenter | Qt::AlignLeft | Qt::AlignLeading
                                    | Qt::AlignHCenter | Qt::AlignVCenter);

  // ---

  QDialogButtonBox* pButtonBox = new QDialogButtonBox();
 #if (defined(Q_OS_WIN) || defined(Q_WS_WIN)) && defined(QT_DEBUG) 
  pLayout->addWidget(pButtonBox);
  pButtonBox->addButton(tr("Preview"), QDialogButtonBox::ResetRole); // TODO
 #else
  pLayout->addWidget(pButtonBox, 0, Qt::AlignRight);
 #endif
  pButtonBox->addButton(tr("Apply to Layout"), QDialogButtonBox::ActionRole);   
  pButtonBox->addButton(tr("OK"), QDialogButtonBox::ActionRole)->setDefault(true); // (QDialogButtonBox::Ok);
  m_pCancel = pButtonBox->addButton(tr("Cancel"), QDialogButtonBox::ActionRole); // (QDialogButtonBox::Cancel);
  m_pMore = pButtonBox->addButton(tr(">"), QDialogButtonBox::ActionRole);
  m_pMore->setCheckable(true);
  QString qsStyle;
  // round button
  qsStyle = "  QPushButton {"
            "  border-style: groove;"
            "  border-width: 2px;"
            "  border-radius: 10px;"
            "  width: 15px;"
            "  padding: 5px;"
            "  left: -2px;"
            "}"
            "QPushButton:pressed {"
            "  border-style: outset;"
            "}"
            "QPushButton:checked {"
            "  border-style: ridge;"
            "}";
  m_pMore->setMinimumSize(20, 20);
  m_pMore->setMaximumSize(20, 20);
  m_pMore->setStyleSheet(qsStyle);
  //
  // ? TODO // http://thesmithfam.org/blog/2009/09/17/qt-stylesheets-button-bar-tutorial/
  //qsStyle = "QPushButton {"
  //          "  color: #333;"
  //          "  border: 2px solid #555;"
  //          "  border-radius: 11px;"
  //          "  padding: 5px;"
  //          "  background: qradialgradient(cx: 0.3, cy: -0.4,"
  //          "  fx: 0.3, fy: -0.4,"
  //          "  radius: 1.35, stop: 0 #fff, stop: 1 #888);"
  //          "    min-width: 15px;" // 80
  //          "}"
  //          "QPushButton:hover {"
  //          "  background: qradialgradient(cx: 0.3, cy: -0.4,"
  //          "  fx: 0.3, fy: -0.4,"
  //          "  radius: 1.35, stop: 0 #fff, stop: 1 #bbb);"
  //          "}"
  //          "QPushButton:pressed {"
  //          "  background: qradialgradient(cx: 0.4, cy: -0.1,"
  //          "  fx: 0.4, fy: -0.1,"
  //          "  radius: 1.35, stop: 0 #fff, stop: 1 #ddd);"
  //          "}";
  //
  //qsStyle = m_pMore->styleSheet() + QChar('\n', 0) + qsStyle; //qsStyle += QChar('\n', 0) + m_pMore->styleSheet();
  //m_pMore->setStyleSheet(qsStyle);
  //
  m_pMore->setChecked(bExtended);
  m_pMore->setText(bExtended ? tr("<") : tr(">"));
  m_pMore->setToolTip(bExtended ? tr("Less options") : tr("More options"));

  connect(pButtonBox, SIGNAL(clicked(QAbstractButton*)), 
          this, SLOT(ButtonBox_clicked(QAbstractButton*)));
  //connect(pButtonBox, SIGNAL(accepted()), this, SLOT(accept()));
  //connect(pButtonBox, SIGNAL(rejected()), this, SLOT(reject()));

  // ---

  fillPageSetup();
  fillPrinterPlotter_Device();
  fillPaperSizes();
  fillNumberOfCopies();
  fillPlotArea();
  fillPlotOffset();
  fillPlotScale();

  fillPlotStyles(); 
  fillShadedViewportOptions();
  fillPlotOptions();
  fillDrawingOrientation();
}   

OdqPlotDialog::~OdqPlotDialog()
{
  OdqMainWindow* pMainWindow = qobject_cast<OdqMainWindow*>(getIApp()->getMainWindow());
  ODA_ASSERT_ONCE(pMainWindow);

  pMainWindow->saveWidgetPlacement(this, "Plot");
  bool bExtended = m_pMore->isChecked(); // m_pRightWidget->isVisible() always is false here
  QSettings settings("ODA", 
    QFileInfo(QCoreApplication::applicationFilePath()).baseName());
  settings.setValue(TD_SHORT_STRING_VER_S "/Plot/extended", bExtended);

  // temp solution for IPF in destructor in v 3.4(non-Windows)
  if (!m_pPlotSettings->objectId().isNull())
  {
    OdDbDatabasePtr pDb = OdDbDatabase::cast(m_pRxDatabase);
    if (!pDb.isNull() && pDb->currentLayoutId() != m_pPlotSettings->objectId())
      m_pPlotSettings->erase();
  }
  m_pPlotSettings = NULL;
}

OdDbPlotSettingsValidator* OdqPlotDialog::plotSettingsValidator()
{
  OdDbHostAppServices* pAppServices = NULL;
  OdDbDatabasePtr pDatabase = OdDbDatabase::cast(m_pRxDatabase);
  if (!pDatabase.isNull())
    pAppServices = pDatabase->appServices();
  else
    pAppServices = OdDbHostAppServices::cast(getIApp()->getBaseHostAppServices());
  if (!pAppServices)
  {
    ODA_FAIL_ONCE();
    return NULL;
  }

  return pAppServices->plotSettingsValidator();
}

OdDbPlotSettingsPtr OdqPlotDialog::plotSettings()
{
  return m_pPlotSettings;
}

OdUnitsFormatter* OdqPlotDialog::formatter()
{
  OdExtDbModule* pExtDb = getIApp()->getExtDbModule(m_pRxDatabase);
  if (!pExtDb)
  {
    OdDbDatabasePtr pDwgDb = OdDbDatabase::cast(m_pRxDatabase);
    if (pDwgDb.get())
      return &pDwgDb->formatter();
  }
  else
    return pExtDb->getFormatter(m_pRxDatabase);

  ODA_FAIL_ONCE();
  return NULL;
}

bool OdqPlotDialog::resetPageSetupName(bool bFill) // = true
{
  OdString sPageSetupName = m_pPlotSettings->getPlotSettingsName();
  if (sPageSetupName == OD_T("None") || sPageSetupName == OD_T("<None>"))
    sPageSetupName = OdString::kEmpty;

  if (sPageSetupName.isEmpty())
    return false;

  m_pPlotSettings->setPlotSettingsName(OD_T("")); // OD_T("None"));
  m_isAppliedToLayout = false;
  if (bFill)
  {
    fillPageSetup();
    fillPrinterPlotter_Info();
  }
  return true;
}

void OdqPlotDialog::fillPageSetup()
{
  ODA_ASSERT_ONCE(!m_pPageSetupCombo.isNull());

  OdString sPageSetupName = m_pPlotSettings->getPlotSettingsName();
  if (sPageSetupName == OD_T("None") || sPageSetupName == OD_T("<None>"))
    sPageSetupName = OdString::kEmpty;
  QString qsPageSetupName = getIApp()->toQString(sPageSetupName);
  if (qsPageSetupName.isEmpty())
    qsPageSetupName = tr("<None>");

  m_pPageSetupCombo->blockSignals(true);
  m_pPageSetupCombo->clear();

  m_pPageSetupCombo->addItem(tr("<None>"));

  OdDbDatabasePtr pDatabase = OdDbDatabase::cast(m_pRxDatabase);
  if (!pDatabase.isNull())
  {
    QStringList lstPages;

    OdDbDictionaryPtr pDict = pDatabase->getPlotSettingsDictionaryId().safeOpenObject();
    for (OdDbDictionaryIteratorPtr pDictIter = pDict->newIterator();
         !pDictIter->done(); pDictIter->next())
    {
      QString qsPage = getIApp()->toQString(pDictIter->name());
      if (qsPage.isEmpty())
      {
        ODA_FAIL_ONCE();
        continue;
      }
      if (!qsPage.indexOf("<Previous plot>", Qt::CaseInsensitive))
      {
        m_pPageSetupCombo->addItem(tr("<Previous plot>"));
        continue;
      }
      lstPages << qsPage;
    }
    lstPages.sort();
    m_pPageSetupCombo->addItems(lstPages);
  }

  //m_pPageSetupCombo->addItem(tr("Import...")); //by ACAD 2009

  int index = m_pPageSetupCombo->findText(qsPageSetupName);
  ODA_ASSERT_ONCE(index >= 0);
  m_pPageSetupCombo->setCurrentIndex(index);

  m_pPageSetupCombo->blockSignals(false);
}

//slot
void OdqPlotDialog::PageSetup_currentIndexChanged(const QString& qsText)
{
  ODA_ASSERT_ONCE(!qsText.isEmpty())

  if (!m_pPageSetupCombo->currentIndex()) // <None>
  {
    resetPageSetupName(false);
    return;
  }

  OdDbDatabasePtr pDatabase = OdDbDatabase::cast(m_pRxDatabase);
  if (pDatabase.isNull())
  {
    ODA_FAIL_ONCE();
    return;
  }

  OdDbDictionaryPtr pDict = pDatabase->getPlotSettingsDictionaryId().safeOpenObject();
  OdDbObjectId idPlotSettings = pDict->getAt( getIApp()->toOdString(qsText));
  if (idPlotSettings.isNull())
  {
    ODA_FAIL_ONCE();
    return;
  }

  m_pPlotSettings->copyFrom(idPlotSettings.safeOpenObject());

  fillPrinterPlotter_Device();
  fillPaperSizes();
}

//slot
void OdqPlotDialog::PageSetupButtonAdd_clicked()
{
  OdDbDatabasePtr pDatabase = OdDbDatabase::cast(m_pRxDatabase);
  if (pDatabase.isNull())
  {
    ODA_FAIL_ONCE();
    return;
  }
  OdDbDictionaryPtr pDict = pDatabase->getPlotSettingsDictionaryId().safeOpenObject(OdDb::kForWrite);

  QString qsPageSetupName;
  for (int cnt = 1; true; cnt++)
  {
    qsPageSetupName = QString("Setup%1").arg(cnt);
    if (pDict->getAt(getIApp()->toOdString(qsPageSetupName)).isNull())
      break;
  }

  OdqPropertyDialog dlg(this);
  dlg.setWindowTitle(QObject::tr("Add page setup"));
  dlg.addWidget(new QLabel(QObject::tr("New page setup name:")));
  QPointer<QLineEdit> pSetupName = qobject_cast<QLineEdit*>(dlg.addWidget(new QLineEdit()));
  pSetupName->setText(qsPageSetupName);
  dlg.setButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

  if (dlg.exec() != QDialog::Accepted || pSetupName->text().isEmpty())
    return;

  qsPageSetupName = pSetupName->text();
  OdString sPageSetupName = getIApp()->toOdString(qsPageSetupName);

  OdDbObjectId idPlotSettings = pDict->getAt(sPageSetupName);
  if (idPlotSettings.isNull())
  {
    bool bDBRO = !m_pPlotSettings->objectId().isNull();
    if (!bDBRO)
      OdDbDatabase::cast(m_pRxDatabase)->addOdDbObject(m_pPlotSettings);
    m_pPlotSettings->setPlotSettingsName(sPageSetupName);
    m_isAppliedToLayout = false;
    pDict->setAt(sPageSetupName, m_pPlotSettings);

    OdDbPlotSettingsPtr pNewPlotSettings = OdDbPlotSettings::createObject();
    if (bDBRO)
      OdDbDatabase::cast(m_pRxDatabase)->addOdDbObject(pNewPlotSettings);
    pNewPlotSettings->copyFrom(m_pPlotSettings);
    m_pPlotSettings = pNewPlotSettings;

    m_pPageSetupCombo->addItem(qsPageSetupName);
    int index = m_pPageSetupCombo->findText(qsPageSetupName);
    ODA_ASSERT_ONCE(index >= 0);
    m_pPageSetupCombo->setCurrentIndex(index);
  }
}

bool OdqPlotDialog::isPrintToFile() const
{
  bool bValue = (m_pPrinter_ToFileCheckBox->checkState() == Qt::Checked);
  return bValue;
}

void OdqPlotDialog::fillPrinterPlotter_Device()
{
  OdArray<const OdChar*> devices;
  plotSettingsValidator()->plotDeviceList(devices);

  m_pPrinterCombo->blockSignals(true);
  m_pPrinterCombo->clear();

  OdString sDeviceName = m_pPlotSettings->getPlotCfgName();
  if (sDeviceName == OD_T("None"))
    sDeviceName = OdString::kEmpty;
  QString qsDeviceName = getIApp()->toQString(sDeviceName);
  if (sDeviceName.isEmpty())
    qsDeviceName = tr("None");

  for (OdArray<const OdChar*>::const_iterator pIt = devices.begin();
       pIt != devices.end(); pIt++)
  {
    QString qsName = getIApp()->toQString(*pIt);
    m_pPrinterCombo->addItem(qsName);
  }

  int index = m_pPrinterCombo->findText(qsDeviceName);
  if (   index < 0
    //&& qsDeviceName == QObject::tr("Default Windows System Printer.pc3")
     )
  {
    QPrinterInfo info = QPrinterInfo::defaultPrinter(); 

    // It is not crtical because of getPlotCfgName is too simple (returns the first in list).
    //if (qsDeviceName != "DWF6 ePlot.pc3")
    //{
    //  QMessageBox::warning(this, //getIApp()->getMainWindow()
    //                       tr("%1 Warning").arg(getIApp()->getAppName()), // caption
    //                       tr("Plotter configuration \"%1\" cannot be used for one of these reasons:"
    //                          " the driver cannot be found, the device cannot be found"
    //                          " or the driver has a problem."
    //                          " The %2 plot device has been substituted.")
    //                            .arg(qsDeviceName)
    //                            .arg((info.isNull() || info.printerName().isEmpty()) ? tr("None") 
    //                                                                                 : tr("Default")),
    //                       QMessageBox::Ok, QMessageBox::Ok);
    //}
    if (!info.isNull())
    {
      qsDeviceName = info.printerName();
      OdString sDeviceName = getIApp()->toOdString(qsDeviceName);

      OdDbPlotSettingsValidator* pValidator = plotSettingsValidator();
      if (   pValidator
          && (   pValidator->setPlotCfgName(m_pPlotSettings, sDeviceName, 
                                           m_pPlotSettings->getCanonicalMediaName()) == eOk // good device, but wrong paper
              || pValidator->setPlotCfgName(m_pPlotSettings, sDeviceName) == eOk)) // wrong device
      {
        index = m_pPrinterCombo->findText(qsDeviceName);
      }
    }
  }
  if (index < 0)
  {
    ODA_FAIL_ONCE(); // test
    OdDbPlotSettingsValidator* pValidator = plotSettingsValidator();
    if (pValidator)
      pValidator->setPlotCfgName(m_pPlotSettings, OD_T("None"), OD_T("none_user_media"));
    index = 0;
  }
  m_pPrinterCombo->setCurrentIndex(index);
  
  m_pPrinterCombo->blockSignals(false);
}

void OdqPlotDialog::fillPrinterPlotter_Info()
{
  OdString sDeviceName = m_pPlotSettings->getPlotCfgName();
  if (sDeviceName == OD_T("None"))
    sDeviceName = OdString::kEmpty;
  QString qsDeviceName = getIApp()->toQString(sDeviceName);

  if (sDeviceName.isEmpty())
  {
    //default
    m_pPrinter_ConfigurationName->setText(tr("None"));
    m_pPrinter_Placement->setText(tr("Not applicable"));
    m_pPrinter_Description->setText(
      tr("The layout will not be plotted unless a new plotter configuration name is selected."));
    return;
  }      

  //foreach (QPrinterInfo info, QPrinterInfo::availablePrinters())
  //{
  //  QString qsName = info.printerName();
  //  if (!qsDeviceName.compare(qsName, Qt::CaseInsensitive))
  //  {
  //    // TODO // QPrinter printer(info)
      m_pPrinter_ConfigurationName->setText(qsDeviceName);
      m_pPrinter_Placement->setText("");

      QString qsDescription;
  //    QPrinter prn(info);
  //    qsDescription = prn.printEngine()->property(QPrintEngine::PPK_PrinterName).toString(); // QPrintEngine::PPK_PrinterProgram
      m_pPrinter_Description->setText(qsDescription);
  //    //
  //    break;
  //  }
  //}

  m_pPrinter_ToFileCheckBox->setCheckState(Qt::Unchecked);
}

//slot
void OdqPlotDialog::PrinterPlotter_currentIndexChanged(const QString& qsText)
{
  ODA_ASSERT_ONCE(   !qsText.isEmpty()
                  && m_pPrinterCombo->currentIndex() == m_pPrinterCombo->findText(qsText));
  OdString sDeviceName = getIApp()->toOdString(qsText); 
  if (qsText == tr("None"))
    sDeviceName = OdString::kEmpty;

  OdString sPrevDeviceName = m_pPlotSettings->getPlotCfgName(); // CURRENT DEVICE
  if (sPrevDeviceName == OD_T("None"))
    sPrevDeviceName = OdString::kEmpty;

  if (sDeviceName == sPrevDeviceName)
    return;
  resetPageSetupName();

  OdDbPlotSettingsValidator* pValidator = plotSettingsValidator();
  if (pValidator)
  {
    if (!sDeviceName.isEmpty())
    {
      if (   pValidator->setPlotCfgName(m_pPlotSettings, sDeviceName, 
                                        m_pPlotSettings->getCanonicalMediaName()) != eOk // good device, but wrong paper
          && pValidator->setPlotCfgName(m_pPlotSettings, sDeviceName) != eOk) // wrong device
      {
        sDeviceName = OdString::kEmpty;
      }
    }
    if (sDeviceName.isEmpty())
    {
      // select None.
      if (pValidator->setPlotCfgName(m_pPlotSettings, OD_T("None"), OD_T("none_user_media")) != eOk)
      {
        ODA_FAIL_ONCE();
      }
      //if (m_pPageSetupCombo->currentIndex())
      //  m_pPageSetupCombo->setCurrentIndex(0);
    }
  }

  fillPageSetup();
  fillPrinterPlotter_Info();
  fillPaperSizes();

  //pValidator->refreshLists(m_pPlotSettings);
}

OdString OdqPlotDialog::toCanonicalMediaName(const OdString& csLocaleMediaName)
{
  OdDbPlotSettingsValidator* pValidator = plotSettingsValidator();

  OdArray<const OdChar *> canonicalMediaNames;
  pValidator->canonicalMediaNameList(m_pPlotSettings, canonicalMediaNames);

  for (OdUInt32 cnt = 0; cnt < canonicalMediaNames.size(); cnt++)
  {
    OdString sLocalMediaName = pValidator->getLocaleMediaName(m_pPlotSettings, cnt);
    ODA_ASSERT_ONCE(   sLocalMediaName == OD_T("Custom Size")
                    || (   getIApp()->toQString(sLocalMediaName)
                        == getIApp()->toQString(canonicalMediaNames[cnt]).replace("_", " ")));
    if (sLocalMediaName == csLocaleMediaName)
      return canonicalMediaNames[cnt];
  }

  ODA_FAIL_ONCE();
  return canonicalMediaNames.first();
}

void OdqPlotDialog::fillPaperSizes()
{
  OdArray<const OdChar *> canonicalMediaNames;
  OdDbPlotSettingsValidator* pValidator = plotSettingsValidator();
  pValidator->canonicalMediaNameList(m_pPlotSettings, canonicalMediaNames);

  m_pPaperSizeCombo->blockSignals(true);
  m_pPaperSizeCombo->clear();

  QString qsLocalMediaName;
  int indexA4 = -1; 
  for (OdUInt32 cnt = 0; cnt < canonicalMediaNames.size(); cnt++)
  {
    qsLocalMediaName = getIApp()->toQString(
                           pValidator->getLocaleMediaName(m_pPlotSettings, cnt));
    ODA_ASSERT_ONCE(!qsLocalMediaName.isEmpty());
    if (indexA4 < 0 && qsLocalMediaName.indexOf("A4") >= 0)
      indexA4 = (int) cnt;

    m_pPaperSizeCombo->addItem(qsLocalMediaName);
  }

  // select active media
  qsLocalMediaName = getIApp()->toQString(pValidator->getLocaleMediaName(
                                                          m_pPlotSettings,
                                                          m_pPlotSettings->getCanonicalMediaName())); 
  int index = m_pPaperSizeCombo->findText(qsLocalMediaName);
  if (index < 0)
  {
    index = indexA4;
    if (index < 0)
      index = 0;
  }
  m_pPaperSizeCombo->setCurrentIndex(index);
  
  m_pPaperSizeCombo->blockSignals(false);
}

//slot
void OdqPlotDialog::PaperSizes_currentIndexChanged(const QString& qsText)
{
  OdString sNewLocaleMediaName = getIApp()->toOdString(qsText);
  OdString sNewCanonicalMediaName = toCanonicalMediaName(sNewLocaleMediaName);

  if (sNewCanonicalMediaName == m_pPlotSettings->getCanonicalMediaName())
    return;
  
  resetPageSetupName();

  OdDbPlotSettingsValidator* pValidator = plotSettingsValidator();

  if (pValidator->setCanonicalMediaName(m_pPlotSettings, sNewCanonicalMediaName) != eOk)
  {
    ODA_FAIL_ONCE();
    fillPaperSizes();
    return;
  }
  { 
    // CPageSetupDlg::getMediaNativePPU()
    // This method uses the backdoor way to define PPU from Media.
    OdDbPlotSettings params;
    pValidator->setPlotCfgName(&params, m_pPlotSettings->getPlotCfgName(), m_pPlotSettings->getCanonicalMediaName());
    OdDbPlotSettings::PlotPaperUnits ppu = params.plotPaperUnits();
    if (ppu != m_pPlotSettings->plotPaperUnits())
      pValidator->setPlotPaperUnits(m_pPlotSettings, ppu);
  }

  fillPlotArea();
  fillPlotOffset();
  fillPlotScale();
  fillDrawingOrientation();
}

//slot
void OdqPlotDialog::PlotArea_currentIndexChanged(const QString& qsText)
{
  OdDbPlotSettings::PlotType typeArea = OdDbPlotSettings::kDisplay,
                             typeSvArea = m_pPlotSettings->plotType();
  while (typeArea <= (OdDbPlotSettings::kLayout + 1) && qsText != AreaTypeToString(typeArea))
    (*(int *) &typeArea)++;
  if (typeArea == typeSvArea)
    return;

  OdDbPlotSettingsValidator* pValidator = plotSettingsValidator();
  if (   typeArea > OdDbPlotSettings::kLayout
      || !pValidator)
  {
    ODA_ASSERT_ONCE(typeArea <= OdDbPlotSettings::kLayout);
    fillPlotArea();
    return;
  }

  if (typeArea == OdDbPlotSettings::kWindow)
  {
    double xMin = 0.0, yMin = 0.0,
           xMax = 0.0, yMax = 0.0;
    m_pPlotSettings->getPlotWindowArea(xMin, yMin, xMax, yMax);
    if (xMax <= xMin || yMax <= yMin)
    {
      PlotAreaWindowButton_clicked();
      return;
    }
  }

  pValidator->setPlotType(m_pPlotSettings, typeArea);
  typeArea = m_pPlotSettings->plotType();

  if (typeArea == OdDbPlotSettings::kLayout)
  {
    // This is differences between dialog and validator. Validator doesn't
    // change UseStandardScale to false. Example is kExtents, kFit2Paper -> kLayout ->kExtents
    // Dialog has kFit2Paper disabled, but validator don't clear kFit2Paper flag.
    // Validator also don't change PlotOrigin to 0,0, if plotsenteres was true, but it change scale to 1:1 if fittopaper was true
    if ( m_pCenterThePlotCheckBox->isChecked())
      pValidator->setPlotOrigin(m_pPlotSettings, 0., 0.);
    if (m_pFitToPaperCheckBox->checkState() == Qt::Checked)
      pValidator->setUseStandardScale(m_pPlotSettings, false);
  }

  fillPlotArea();
  fillPlotOffset();
  fillPlotScale();
}

//slot
void OdqPlotDialog::PlotAreaView_currentIndexChanged(const QString& qsText)
{
  ODA_ASSERT_ONCE(   !qsText.isEmpty() 
                  && m_pPlotAreaViewsCombo->currentIndex() == m_pPlotAreaViewsCombo->findText(qsText));

  OdDbPlotSettingsValidator* pValidator = plotSettingsValidator();
  if (pValidator)
    pValidator->setPlotViewName(m_pPlotSettings, getIApp()->toOdString(qsText));

  fillPlotArea();
}

//slot
void OdqPlotDialog::PlotAreaWindowButton_clicked()
{
  done(enPlorDlg_WindowArea); 
}

//slot
void OdqPlotDialog::PlotOffset_edited()
{
  QString qsX,
          qsY;
  PlotOffsetsToStrings(qsX, qsY);

  if (qsX == m_pPlotOffsetXEdit->text() && qsY == m_pPlotOffsetYEdit->text())
    return;
  qsX = m_pPlotOffsetXEdit->text();
  qsY = m_pPlotOffsetYEdit->text();

  OdDbPlotSettingsValidator* pValidator = plotSettingsValidator();
  double xCoordinate = 0.0,
         yCoordinate = 0.0;

  bool bOk = (!qsX.isEmpty() && !qsY.isEmpty());
  if (bOk)
    xCoordinate = qsX.toDouble(&bOk);
  if (bOk)
    yCoordinate = qsY.toDouble(&bOk);
  if (!bOk || !pValidator)
  {
    fillPlotOffset();
    return;
  }
  OdDbPlotSettings::PlotPaperUnits ppu = m_pPlotSettings->plotPaperUnits();
  if (ppu == OdDbPlotSettings::kInches)
  {
    xCoordinate *= kMmPerInch; // 25.4
    yCoordinate *= kMmPerInch;
  }
  OdDbPlotSettings::PlotRotation rotation = m_pPlotSettings->plotRotation();
  if (rotation  == OdDbPlotSettings::k90degrees || rotation == OdDbPlotSettings::k270degrees)
    qSwap(xCoordinate, yCoordinate);

  double xSv = 0.0,
         ySv = 0.0;
  m_pPlotSettings->getPlotOrigin(xSv, ySv);
  if (OdZero(xCoordinate - xSv) && OdZero(yCoordinate - ySv))
  {
    fillPlotOffset();
    return;
  }
  resetPageSetupName();

  pValidator->setPlotOrigin(m_pPlotSettings, xCoordinate, yCoordinate);

  fillPlotOffset();
}

//slot
void OdqPlotDialog::PlotOffset_changed()
{
  bool bCentered = m_pCenterThePlotCheckBox->isChecked();
  if (bCentered == m_pPlotSettings->plotCentered())
    return;

  OdDbPlotSettingsValidator* pValidator = plotSettingsValidator();
  if (!pValidator)
  {
    fillPlotOffset();
    return;
  }
  resetPageSetupName();

  pValidator->setPlotCentered(m_pPlotSettings, bCentered);

  fillPlotOffset();
}

//slot
void OdqPlotDialog::PlotScaleFitToPaper_changed()
{
  ODA_ASSERT_ONCE(m_pPlotSettings->plotType() != OdDbPlotSettings::kLayout);

  bool bFitToPaper = (m_pFitToPaperCheckBox->checkState() == Qt::Checked),
       bFitToPaperPrev = (   m_pPlotSettings->useStandardScale() 
                && m_pPlotSettings->stdScaleType() == OdDbPlotSettings::kScaleToFit);
  if (bFitToPaper == bFitToPaperPrev)
    return;

  OdDbPlotSettingsValidator* pValidator = plotSettingsValidator();
  if (!pValidator)
  {
    fillPlotScale();
    return;
  }
  resetPageSetupName();

  if (bFitToPaper)
    pValidator->setStdScaleType(m_pPlotSettings, OdDbPlotSettings::kScaleToFit);
  else
    pValidator->setUseStandardScale(m_pPlotSettings, false);

  fillPlotScale();
}

//slot
void OdqPlotDialog::PlotScale_currentIndexChanged(const QString& qsText)
{
  OdDbPlotSettings::StdScaleType typeScale = OdDbPlotSettings::kScaleToFit;
  while (typeScale <= (OdDbPlotSettings::k1and1_2in_1ft + 1) && qsText != ScaleTypeToString(typeScale))
    (*(int *) &typeScale)++;

  OdDbPlotSettingsValidator* pValidator = plotSettingsValidator();
  if (   typeScale > OdDbPlotSettings::k1and1_2in_1ft 
      || typeScale == OdDbPlotSettings::kScaleToFit // skip
      || !pValidator)
  {
    ODA_ASSERT_ONCE(typeScale <= OdDbPlotSettings::k1and1_2in_1ft);
    fillPlotScale();
    return;
  }
  if (typeScale == m_pPlotSettings->stdScaleType())
    return;
  resetPageSetupName();

  pValidator->setStdScaleType(m_pPlotSettings, typeScale);

  fillPlotScale();
}

//slot
void OdqPlotDialog::Unit_edited()
{
  bool bFitToPaper = (   m_pPlotSettings->useStandardScale() 
                      && m_pPlotSettings->plotType() != OdDbPlotSettings::kLayout
                      && m_pPlotSettings->stdScaleType() == OdDbPlotSettings::kScaleToFit);
  if (bFitToPaper)
  {
    ODA_FAIL_ONCE();
    return;
  }

  QString qsPaperUnuts = m_pUnitPaperEdit->text(),
          qsDrawingUnuts = m_pUnitDrawingEdit->text();
  double dPaperScaleUnit = 0.0,
         dDrawingScaleUnit = 0.0;

  bool bOk = (!qsPaperUnuts.isEmpty() && !qsDrawingUnuts.isEmpty());
  if (bOk)
    dPaperScaleUnit = qsPaperUnuts.toDouble(&bOk);
  if (bOk)
    dDrawingScaleUnit = qsDrawingUnuts.toDouble(&bOk);
  double dSvPaperScaleUnit = 0.0,
         dSvDrawingScaleUnit = 0.0;
  if (bOk)
    m_pPlotSettings->getCustomPrintScale(dSvPaperScaleUnit, dSvDrawingScaleUnit);
  OdDbPlotSettingsValidator* pValidator = plotSettingsValidator();
  if (   !bOk 
      || (OdZero(dPaperScaleUnit - dSvPaperScaleUnit) && OdZero(dDrawingScaleUnit - dSvDrawingScaleUnit))
      || !pValidator)
  {
    fillPlotScale();
    return;
  }

  pValidator->setCustomPrintScale(m_pPlotSettings,
                                  dPaperScaleUnit, dDrawingScaleUnit);

  fillPlotScale();
}

//slot
void OdqPlotDialog::PlotUnit_currentIndexChanged(const QString& qsText)
{
  OdDbPlotSettings::PlotPaperUnits ppu = OdDbPlotSettings::kInches;
  while (ppu <= (OdDbPlotSettings::kPixels + 1) && qsText != PaperUnitsToString(ppu))
    (*(int *) &ppu)++;

  OdDbPlotSettingsValidator* pValidator = plotSettingsValidator();
  if (ppu > OdDbPlotSettings::kPixels || !pValidator)
  {
    ODA_ASSERT_ONCE(!pValidator);
    fillPlotScale();
    return;
  }
  if (ppu == m_pPlotSettings->plotPaperUnits())
    return;
  resetPageSetupName();

  pValidator->setPlotPaperUnits(m_pPlotSettings, ppu);

  fillPlotScale();
  fillPlotOffset();
}

//slot
void OdqPlotDialog::PlotScaleLineweights_changed()
{
  bool bValue = (m_pScaleLineweightsCheckBox->checkState() == Qt::Checked);
  if (bValue == m_pPlotSettings->scaleLineweights())
    return;

  m_pPlotSettings->setScaleLineweights(bValue);
  m_isAppliedToLayout = false;

  fillPlotScale();
}

//slot
void OdqPlotDialog::Shade_currentIndexChanged(int index)
{
  ODA_ASSERT_ONCE(   index == m_pShadePlotCombo->currentIndex()
                  && index >= 0 && index <= OdDbPlotSettings::kRenderPreset);

  OdDbPlotSettings::ShadePlotType shade = OdDbPlotSettings::ShadePlotType(index);
  if (shade == m_pPlotSettings->shadePlot())
    return;

  m_pPlotSettings->setShadePlot(shade);
  m_isAppliedToLayout = false;

  resetPageSetupName();
  fillShadedViewportOptions();
}

//slot
void OdqPlotDialog::Quality_currentIndexChanged(int index)
{
  ODA_ASSERT_ONCE(   index == m_pQualityCombo->currentIndex()
                  && index >= 0 && index <= OdDbPlotSettings::kCustom);
  OdDbPlotSettings::ShadePlotResLevel level = OdDbPlotSettings::ShadePlotResLevel(index);
  if (level == m_pPlotSettings->shadePlotResLevel())
    return;
  
  int nDpi = m_pPlotSettings->shadePlotCustomDPI();
  switch (level)
  {
  case OdDbPlotSettings::kDraft:
    nDpi = 200;
    break;
  case OdDbPlotSettings::kPreview:
    nDpi = 150;
    break;
  case OdDbPlotSettings::kNormal:
    nDpi = 300;
    break;
  case OdDbPlotSettings::kPresentation:
  case OdDbPlotSettings::kMaximum:
    nDpi = 600;
    break;
  default:
    ODA_FAIL_ONCE();
  case OdDbPlotSettings::kCustom:
    break;
  }
  m_pPlotSettings->setShadePlotResLevel(OdDbPlotSettings::kCustom);
  m_pPlotSettings->setShadePlotCustomDPI(nDpi);
  m_pPlotSettings->setShadePlotResLevel(level);
  m_isAppliedToLayout = false;

  resetPageSetupName();
  fillShadedViewportOptions();
}

//slot
void OdqPlotDialog::focusChanged(QWidget* pOld, QWidget* pNew)
{
  if (   pOld == m_pDpiEdit 
      && pNew != m_pQualityCombo && pNew != m_pCancel
      && !m_pDpiEdit.isNull() && pOld != pNew)
  {
    QObject* pParent = pNew->parent();
    while (pParent && pParent != this)
      pParent = pParent->parent();
    if (pParent == this)
      DpiEdit_edited(); // emit m_pDpiEdit->editingFinished();
    //else
    //  pNew = pNew; // brk (skip via press Esc)
  }
}

//slot
void OdqPlotDialog::DpiEdit_edited()
{
  QString qsDpi = m_pDpiEdit->text();

  bool bOk = true;
  if (m_pDpiEdit->validator())
  {
    int pos = 0;
    QValidator::State state = m_pDpiEdit->validator()->validate(qsDpi, pos);
    if (state != QValidator::Acceptable)
      bOk = false;
  }
  int nDpi = 0;
  if (bOk)
  {
    nDpi = qsDpi.toInt(&bOk);
    if (!bOk || nDpi < 100 || nDpi > 600)
    {
      ODA_FAIL_ONCE();
      bOk = false;
    }
  }
  if (!bOk)
  {
    QMessageBox::warning(this, //getIApp()->getMainWindow()
      QObject::tr("%1 Error").arg(getIApp()->getAppName()), // caption
      tr("Please enter a DPI between 100 and 600."),
      QMessageBox::Ok, QMessageBox::Ok);
    m_pDpiEdit->setFocus();
    return;
  }
  ODA_ASSERT_ONCE(m_pPlotSettings->shadePlotResLevel() == OdDbPlotSettings::kCustom);

  if (nDpi == m_pPlotSettings->shadePlotCustomDPI())
    return;

  m_pPlotSettings->setShadePlotCustomDPI(nDpi);
  m_isAppliedToLayout = false;

  resetPageSetupName();
  fillShadedViewportOptions();
}

//slot
void OdqPlotDialog::PlotOptions_changed()
{
  bool bChanged = false;

  // bool bPlotInBackground = (m_pPlotInBackgroundCheckBox->checkState() == Qt::Checked);

  bool bPrintLineweights = (m_pPlotObjectLineweightsCheckBox->checkState() == Qt::Checked);
  if (bPrintLineweights != (m_pPlotSettings->printLineweights() || m_pPlotSettings->plotPlotStyles()))
  {
    ODA_ASSERT_ONCE(!m_pPlotSettings->plotPlotStyles());
    bChanged = true;
    m_pPlotSettings->setPrintLineweights(bPrintLineweights);
  }

  bool bPlotWithPlotStyles = (m_pPlotWithPlotStylesCheckBox->checkState() == Qt::Checked);
  if (bPlotWithPlotStyles != m_pPlotSettings->plotPlotStyles())
  {
    bChanged = true;
    m_pPlotSettings->setPlotPlotStyles(bPlotWithPlotStyles);
  }

  bool bPlotPaperspaceLast = (m_pPlotPaperspaceLastCheckBox->checkState() == Qt::Checked);
  if (bPlotPaperspaceLast != m_pPlotSettings->drawViewportsFirst())
  {
    bChanged = true;
    m_pPlotSettings->setDrawViewportsFirst(bPlotPaperspaceLast);
  }

  bool bHidePaperspaceObjects = (m_pHidePaperspaceObjectsCheckBox->checkState() == Qt::Checked);
  if (bHidePaperspaceObjects != m_pPlotSettings->plotHidden())
  {
    bChanged = true;
    m_pPlotSettings->setPlotHidden(bHidePaperspaceObjects);
  }

  //bool bPlotStampOn = (m_pPlotStampOnCheckBox->checkState() == Qt::Checked);
  //bool bSaveChangesToLayout = (m_pSaveChangesToLayoutCheckBox->checkState() == Qt::Checked);

  if (!bChanged)
    return;

  m_isAppliedToLayout = false;
  resetPageSetupName();
  fillPlotOptions();
}

//slot
void OdqPlotDialog::DrawingOrientation_changed()
{
  OdDbPlotSettingsValidator* pValidator = plotSettingsValidator();
  if (pValidator)
  {
    OdDbPlotSettings::PlotRotation rotation = OdDbPlotSettings::k0degrees;

    bool bUpsideDown = (m_pUpsideDownCheckBox->checkState() == Qt::Checked);
    bool isWidthLessHeight = isPaperWidthLessHeight();
    if (m_pPortraitRadio->isChecked() == isWidthLessHeight) // CORE-17447 // if (m_pPortraitRadio->isChecked())
      rotation = bUpsideDown ? OdDbPlotSettings::k180degrees : OdDbPlotSettings::k0degrees;
    else
      rotation = bUpsideDown ? OdDbPlotSettings::k270degrees : OdDbPlotSettings::k90degrees;

    if (rotation != m_pPlotSettings->plotRotation())
    {
      resetPageSetupName();
      pValidator->setPlotRotation(m_pPlotSettings, rotation);
    }
    else
      ODA_FAIL_ONCE();
  }

  fillDrawingOrientation();
  fillPlotOffset();
}

//slot
void OdqPlotDialog::ButtonBox_clicked(QAbstractButton* pButton)
{
  QString sName = pButton->text();
  bool bSave = (m_pSaveChangesToLayoutCheckBox->checkState() == Qt::Checked);

  if (sName == tr("Preview"))
  {
    ODA_FAIL_ONCE(); // possible TODO
    pButton->setEnabled(false);
    return;
  }

  if (sName == tr("Apply to Layout"))
  {
    applyToLayout();
    return;
  }

  if (sName == tr("OK"))
  {
    if (bSave)
      applyToLayout();

    bool bPrevSave = getIAppProps()->toBool(OD_T("Vars/plotsavechangestolayout"), false);
    if (bSave != bPrevSave)
      getIAppProps()->set(OD_T("Vars/plotsavechangestolayout"), bSave, OD_T("bool"));

    accept();
    return;
  }
  if (sName == tr("Cancel"))
  {
    reject();
    return;
  }

  if (sName == tr(">") || sName == tr("<"))
  {
    bool bExtended = pButton->isChecked();
    ODA_ASSERT_ONCE(bExtended == (sName == tr(">")));

    QRect rect = geometry();
    double dWidth = m_pLeftWidget->width();

    pButton->setText(bExtended ? tr("<") : tr(">"));
    pButton->setToolTip(bExtended ? tr("Less options") : tr("More options"));

    ODA_ASSERT_ONCE(!m_pRightWidget.isNull());
    m_pRightWidget->setVisible(bExtended);

    m_pLeftWidget->blockSignals(true);
    if (bExtended)
    {
      rect.setWidth(rect.width() * 3 / 2);
      setGeometry(rect);
    }
    layout()->activate(); // adjustSize();
    double dWidthNew = m_pLeftWidget->width();
    rect.setWidth(rect.width() * dWidth / dWidthNew);
    setGeometry(rect);
    m_pLeftWidget->blockSignals(false);

    //return;
  }
}

bool OdqPlotDialog::isAppliedToLayout() const
{
  return m_isAppliedToLayout;
}

void OdqPlotDialog::applyToLayout(OdDbPlotSettings* pPlotSettings) // = NULL
{
  ODA_ASSERT_ONCE(m_pRxDatabase && m_pPlotSettings.get());
  if (!pPlotSettings)
    pPlotSettings = m_pPlotSettings.get();
  m_isAppliedToLayout = (pPlotSettings == m_pPlotSettings.get());

  OdDbBaseDatabasePEPtr pDbPE = OdDbBaseDatabasePE::cast(m_pRxDatabase);
  ODA_ASSERT_ONCE(!pDbPE.isNull());

  OdRxObjectPtr pCurrLayout = pDbPE->currentLayout(m_pRxDatabase);
  ODA_ASSERT_ONCE(!pCurrLayout.isNull());
  OdDbPlotSettingsPtr pCurrLayoutSettings = OdDbPlotSettings::cast(pCurrLayout);

  ODA_ASSERT_ONCE(pCurrLayoutSettings->objectId() != pPlotSettings->objectId());

  // apply pPlotSettings to current layout
  pCurrLayoutSettings->upgradeOpen();
  pCurrLayoutSettings->copyFrom(pPlotSettings);
}

void OdqPlotDialog::fillNumberOfCopies()
{
  m_pNumberOfCopiesSpin->setValue(1);
  m_pNumberOfCopiesSpin->setEnabled(!isPrintToFile());
  //m_pNumberOfCopiesSpin->setReadOnly(isPrintToFile());
}

int OdqPlotDialog::numberOfCopies() const
{
  int nValue = m_pNumberOfCopiesSpin->value();
  ODA_ASSERT_ONCE(nValue > 0);
  return nValue;
}

void OdqPlotDialog::fillPlotArea()
{
  OdDbPlotSettings::PlotType typeArea = m_pPlotSettings->plotType();
  QString qsArea = AreaTypeToString(typeArea);
  
  int index = 0;
  if (!qsArea.isEmpty())
  {
    index = m_pPlotAreaCombo->findText(qsArea);
    if (index < 0) // "pixels"
    {
      ODA_FAIL_ONCE();
      m_pPlotAreaCombo->addItem(qsArea);
      index = m_pPlotAreaCombo->findText(qsArea);
    }
    m_pPlotAreaCombo->blockSignals(true);
    m_pPlotAreaCombo->setCurrentIndex(index);
    m_pPlotAreaCombo->blockSignals(false);
  }

  if (!m_pPlotAreaViewsCombo.isNull())
  {
    m_pPlotAreaViewsCombo->blockSignals(true);
    QString qsViewName = getIApp()->toQString(m_pPlotSettings->getPlotViewName());
    if (!qsViewName.isEmpty())
    {
      int index = m_pPlotAreaViewsCombo->findText(qsViewName);
      if (index < 0)
      {
        ODA_FAIL_ONCE();
        m_pPlotAreaViewsCombo->addItem(qsViewName);
        index = m_pPlotAreaViewsCombo->findText(qsViewName);
      }
      m_pPlotAreaViewsCombo->setCurrentIndex(index);
    }
    m_pPlotAreaViewsCombo->setVisible(typeArea == OdDbPlotSettings::kView);
    m_pPlotAreaViewsCombo->blockSignals(false);
    if (qsViewName.isEmpty() && typeArea == OdDbPlotSettings::kView)
      m_pPlotAreaViewsCombo->setCurrentIndex(0);
  }

  m_pPlotAreaWindowButton->setVisible(typeArea == OdDbPlotSettings::kWindow);
}

void OdqPlotDialog::PlotOffsetsToStrings(QString& qsX, QString& qsY)
{
  double xCoordinate = 0.0,
         yCoordinate = 0.0;
  m_pPlotSettings->getPlotOrigin(xCoordinate, yCoordinate); // in mm
  OdDbPlotSettings::PlotRotation rotation = m_pPlotSettings->plotRotation();
  if (rotation  == OdDbPlotSettings::k90degrees || rotation == OdDbPlotSettings::k270degrees)
    qSwap(xCoordinate, yCoordinate);

  OdDbPlotSettings::PlotPaperUnits ppu = m_pPlotSettings->plotPaperUnits();
  if (ppu == OdDbPlotSettings::kInches)
  {
    xCoordinate /= kMmPerInch; // 25.4
    yCoordinate /= kMmPerInch;
  }
  // ? TODO via formatter();
  qsX = QString::number(xCoordinate, 'f', ppu == OdDbPlotSettings::kInches ? 6 : 2);
  qsY = QString::number(yCoordinate, 'f', ppu == OdDbPlotSettings::kInches ? 6 : 2);
}

void OdqPlotDialog::fillPlotOffset()
{
  bool bCentered = m_pPlotSettings->plotCentered();

  QString qsX,
          qsY;
  PlotOffsetsToStrings(qsX, qsY);
  m_pPlotOffsetXEdit->setText(qsX);
  m_pPlotOffsetYEdit->setText(qsY);

  QString qsUnits = PaperUnitsToString(m_pPlotSettings->plotPaperUnits());
  m_pPlotOffsetUnitXLabel->setText(qsUnits);
  m_pPlotOffsetUnitYLabel->setText(qsUnits);

  m_pPlotOffsetXEdit->setEnabled(!bCentered);
  m_pPlotOffsetYEdit->setEnabled(!bCentered);
  m_pPlotOffsetUnitXLabel->setEnabled(!bCentered);
  m_pPlotOffsetUnitYLabel->setEnabled(!bCentered);

  m_pCenterThePlotCheckBox->blockSignals(true);
  m_pCenterThePlotCheckBox->setChecked(bCentered);
  m_pCenterThePlotCheckBox->setEnabled(m_pPlotSettings->plotType() != OdDbPlotSettings::kLayout);
  m_pCenterThePlotCheckBox->blockSignals(false);
}

void OdqPlotDialog::fillPlotScale()
{
  OdDbPlotSettings::StdScaleType typeScale = m_pPlotSettings->stdScaleType();
  if (   typeScale < 0 || typeScale > OdDbPlotSettings::k1and1_2in_1ft
      || !m_pPlotSettings->useStandardScale())
    typeScale = OdDbPlotSettings::kScaleToFit;
  QString qsScaleType = ScaleTypeToString(typeScale);
  int index = m_pScaleCombo->findText(qsScaleType);
  ODA_ASSERT_ONCE(index >= 0);
  bool bFitToPaper = (   m_pPlotSettings->useStandardScale() 
                      && m_pPlotSettings->plotType() != OdDbPlotSettings::kLayout
                      && typeScale == OdDbPlotSettings::kScaleToFit);

  m_pScaleCombo->blockSignals(true);
  m_pScaleCombo->setCurrentIndex(index);
  m_pScaleCombo->setEnabled(!bFitToPaper);
  m_pScaleCombo->blockSignals(false);


  m_pFitToPaperCheckBox->blockSignals(true);
  m_pFitToPaperCheckBox->setChecked(bFitToPaper);
  m_pFitToPaperCheckBox->setEnabled(m_pPlotSettings->plotType() != OdDbPlotSettings::kLayout);
  m_pFitToPaperCheckBox->blockSignals(false);
  
  double dPaperScaleUnit = 0.0,
         dDrawingScaleUnit = 0.0;
  if (!bFitToPaper)
    m_pPlotSettings->getCustomPrintScale(dPaperScaleUnit, dDrawingScaleUnit);
  else
  {
    dPaperScaleUnit = ScaleTypeToValue(typeScale, enPaperScaleUnits);
    dDrawingScaleUnit = ScaleTypeToValue(typeScale, enDrawingScaleUnits);
  }
  OdDbPlotSettings::PlotPaperUnits ppu = m_pPlotSettings->plotPaperUnits();

  m_pUnitPaperEdit->blockSignals(true);
  m_pUnitPaperEdit->setText(QString::number(dPaperScaleUnit, 'f', 
                                            ppu == OdDbPlotSettings::kInches ? 2 : 4));
  m_pUnitPaperEdit->setEnabled(!bFitToPaper);
  m_pUnitPaperEdit->blockSignals(false);

  m_pUnitDrawingEdit->blockSignals(true);
  m_pUnitDrawingEdit->setText(QString::number(dDrawingScaleUnit, 'f', 
                                              ppu == OdDbPlotSettings::kInches ? 2 : 4));
  m_pUnitDrawingEdit->setEnabled(!bFitToPaper);
  m_pUnitDrawingLabel->setEnabled(!bFitToPaper);
  m_pUnitDrawingEdit->blockSignals(false);

  m_pScaleLineweightsCheckBox->blockSignals(true);
  m_pScaleLineweightsCheckBox->setChecked(m_pPlotSettings->scaleLineweights());
  m_pScaleLineweightsCheckBox->setEnabled(!m_pPlotSettings->modelType());
  m_pScaleLineweightsCheckBox->blockSignals(false);

  QString qsPpu = PaperUnitsToString(ppu);
  index = 1; // "mm"
  if (!qsPpu.isEmpty())
  {
    index = m_pUnitCombo->findText(qsPpu);
    if (index < 0) // "pixels"
    {
      m_pUnitCombo->addItem(qsPpu);
      index = m_pUnitCombo->findText(qsPpu);
    }
  }
  m_pUnitCombo->blockSignals(true);
  m_pUnitCombo->setCurrentIndex(index);
  m_pUnitCombo->blockSignals(false);
}

bool OdqPlotDialog::isPaperWidthLessHeight() const
{
  double widthPaper, heightPaper;
  m_pPlotSettings->getPlotPaperSize(widthPaper, heightPaper);
  return widthPaper < heightPaper;
}

void OdqPlotDialog::fillPlotStyles()
{
  OdDbPlotSettingsValidator* pValidator = plotSettingsValidator();
  OdArray<const OdChar *> arrPlotStyles;
  if (!pValidator || pValidator->plotStyleSheetList(arrPlotStyles) != eOk)
    return;

  OdArray<OdString> arrPlotStyles_;
  OdDbPlotSettingsValidatorPEPtr pPSVPe = OdDbPlotSettingsValidatorPE::cast(pValidator);
  if (   !pPSVPe.isNull() && pPSVPe->getPlotStyleSheetList(arrPlotStyles_, getIApp()->getBaseHostAppServices()) == eOk
      && arrPlotStyles.size() != arrPlotStyles_.size())
  {
    // DDPLOTSTYLEPATHS variable was changed
    pValidator->refreshLists(0);
    arrPlotStyles.clear();
    pValidator->plotStyleSheetList(arrPlotStyles);
  }

  OdDbDatabase* pDwgDb = OdDbDatabase::cast(m_pRxDatabase).get();
  if (!pDwgDb || pDwgDb->getPSTYLEMODE()) // ByColor
  {
    // remove all *.stb as useless for such drawings
    OdArray<const OdChar *>::iterator pItr = arrPlotStyles.begin();
    while (pItr != arrPlotStyles.end())
    {
      QString qsStyle = getIApp()->toQString(*pItr);
      QString qsExt = QFileInfo(qsStyle).suffix().toLower();
      if (qsExt == "ctb")
      {
        pItr++;
        continue;
      }
      pItr = arrPlotStyles.erase(pItr);
    }
  }

  m_pPlotStyleFilesCombo->blockSignals(true);
  m_pPlotStyleFilesCombo->clear();
  m_pPlotStyleFilesCombo->addItem(tr("None"));
  for(unsigned int idx = 0; idx < arrPlotStyles.length(); idx++)
  {
    OdString sPlotStyle = arrPlotStyles[idx];
    m_pPlotStyleFilesCombo->addItem(getIApp()->toQString(sPlotStyle));
  }
  QString qsCurrentPlotStyle = getIApp()->toQString(m_pPlotSettings->getCurrentStyleSheet());
  if (qsCurrentPlotStyle.isEmpty())
    qsCurrentPlotStyle = tr("None");
  int index = m_pPlotStyleFilesCombo->findText(qsCurrentPlotStyle);
  bool bEditable = false;
 #ifdef _DEBUG // temp
  bEditable = (index != 0);
 #endif
  if (index < 0)
  {
    bEditable = false;
    qsCurrentPlotStyle += tr(" (missing)");
    m_pPlotStyleFilesCombo->insertItem(1, qsCurrentPlotStyle);
    index = m_pPlotStyleFilesCombo->findText(qsCurrentPlotStyle);
  }
  ODA_ASSERT_ONCE(index >= 0);
  m_pPlotStyleFilesCombo->setCurrentIndex(index);
  m_pPlotStyleFilesCombo->blockSignals(false);

  m_pPlotStyleEditButton->setEnabled(bEditable);

  //m_pDisplayPlotStylesCheckBox->blockSignals(true);
  //m_pDisplayPlotStylesCheckBox->setCheckState(m_pPlotSettings->showPlotStyles() ? Qt::Checked 
  //                                                                              : Qt::Unchecked);
  //m_pDisplayPlotStylesCheckBox->setEnabled(!m_pPlotSettings->modelType());
  //m_pDisplayPlotStylesCheckBox->blockSignals(false);
}

//slot 
void OdqPlotDialog::PlotStyles_currentIndexChanged(const QString& qsText)
{
  ODA_ASSERT_ONCE(!qsText.isEmpty());
  QString qsPlotStyle = qsText;
  if (qsPlotStyle == tr("None"))
    qsPlotStyle.clear();
  int pos = qsPlotStyle.lastIndexOf(tr(" (missing)"));
  if (pos > 0)
    qsPlotStyle = qsPlotStyle.left(pos);
  OdString sPlotStyle = getIApp()->toOdString(qsPlotStyle);

  if (sPlotStyle == m_pPlotSettings->getCurrentStyleSheet())
  {
    static bool dFirst = true;
    int cnt = m_pPlotStyleFilesCombo->count();
    if (   dFirst 
        && (   cnt == 1 // none
            || (cnt == 2 && pos > 0))) // none + ...(missing)
    {
      OdString sPathPS = getIAppProps()->toString(OD_T("Vars/ddplotstylepaths"), 
                                                  getIApp()->toOdString(getenv("DDPLOTSTYLEPATHS"))); //getenv() is a standard ISO C function from stdlib.h
      if (sPathPS.isEmpty())
        QMessageBox::warning(this, //getIApp()->getMainWindow()
                             QObject::tr("%1 Warning").arg(getIApp()->getAppName()), // caption
                             tr("You should define environment variable DDPLOTSTYLEPATHS to get plot styles in this combo box."),
                             QMessageBox::Ok, QMessageBox::Ok);
    }
    dFirst = false;
    return;
  }

  OdDbPlotSettingsValidator* pValidator = plotSettingsValidator();
  if (pValidator)
  {
    resetPageSetupName();
    OdResult es = pValidator->setCurrentStyleSheet(m_pPlotSettings, sPlotStyle);
    ODA_ASSERT_ONCE(es == eOk && m_pPlotSettings->getCurrentStyleSheet() == sPlotStyle);
    m_pPlotSettings->setShowPlotStyles(!m_pPlotSettings->getCurrentStyleSheet().isEmpty());
    m_isAppliedToLayout = false;
  }

  fillPlotStyles();
}

//slot
void OdqPlotDialog::PlotStyleEditButton_clicked() // via m_pPlotStyleEditButton
{
  OdString sCurrentPlotStyle = m_pPlotSettings->getCurrentStyleSheet();

  if (sCurrentPlotStyle.isEmpty())
  {
    ODA_FAIL_ONCE();
    return;
  }

  OdPsPlotStyleServicesPtr pPSS = odrxDynamicLinker()->loadApp(ODPS_PLOTSTYLE_SERVICES_APPNAME);
  if (pPSS.isNull())
  {
    getIConsole()->putString(QObject::tr("Module %1 was not found")
          .arg(getIApp()->toQString(ODPS_PLOTSTYLE_SERVICES_APPNAME)));
    return;
  }

  try
  {  
    OdDbSystemServices* pSs = ::odSystemServices();
    ODA_ASSERT_ONCE(OdDbBaseDatabasePE::cast(m_pRxDatabase).get());
    OdDbBaseHostAppServices* pAppServices = OdDbBaseDatabasePE::cast(m_pRxDatabase)
                                                  ->appServices(m_pRxDatabase);
    ODA_ASSERT_ONCE(pAppServices);
    OdString sPath = pAppServices->findFile(sCurrentPlotStyle, m_pRxDatabase);
    if (   sPath.isEmpty()
        || !pSs->accessFile(sPath, Oda::kFileRead))
    {
      ODA_FAIL_ONCE();
      //fillPlotStyles();
      return;
    }

    OdStreamBufPtr pFileBuf = pSs->createFile(sPath);
    if (!pFileBuf.get())
    {
      ODA_FAIL_ONCE();
      //fillPlotStyles();
      return;
    }

    OdPsPlotStyleTablePtr pPlotStyleTable = pPSS->loadPlotStyleTable(pFileBuf);

    ODA_FAIL_ONCE(); // TODO
    //CPsTableEditorDlg PsTableEditorDlg(theApp.GetMainWnd());
    //PsTableEditorDlg.SetFileBufPath(sPath);
    //PsTableEditorDlg.SetPlotStyleTable(pPlotStyleTable);
    //if (PsTableEditorDlg.DoModal() == IDOK)
    //{
    //  pPlotStyleTable->copyFrom(PsTableEditorDlg.GetPlotStyleTable());
    //}
  }
  catch (...)
  {
  }
  
  //fillPlotStyles();
}

// missing in new ACAD
//slot
//void OdqPlotDialog::PlotStylesDisplay_changed()
//{
//  bool bValue = (m_pDisplayPlotStylesCheckBox->checkState() == Qt::Checked);
//  if (bValue == m_pPlotSettings->showPlotStyles())
//  {
//    ODA_FAIL_ONCE();
//    return;
//  }
//  resetPageSetupName();
//
//  m_pPlotSettings->setShowPlotStyles(bValue);
//  m_isAppliedToLayout = false;
//
//  fillPlotStyles();
//}

void OdqPlotDialog::fillShadedViewportOptions() // fillShadePlotQualityDPI
{
  OdDbPlotSettings::ShadePlotType shade = m_pPlotSettings->shadePlot();
  ODA_ASSERT_ONCE(shade >= 0 && shade < m_pShadePlotCombo->count());
  m_pShadePlotCombo->blockSignals(true);
  m_pShadePlotCombo->setCurrentIndex(shade);
  m_pShadePlotCombo->blockSignals(false);

  OdDbPlotSettings::ShadePlotResLevel level = m_pPlotSettings->shadePlotResLevel();
  ODA_ASSERT_ONCE(level >= 0 && level < m_pQualityCombo->count());
  m_pQualityCombo->blockSignals(true);
  m_pQualityCombo->setCurrentIndex(level);
  m_pQualityCombo->blockSignals(false);

  OdInt16 nDpi = m_pPlotSettings->shadePlotCustomDPI();
  if (level == OdDbPlotSettings::kDraft)
    nDpi = 0;
  QString qsDpi = QString::number(nDpi);
  // ? TODO
  //OdUnitsFormatter* pFormatter = formatter();
  //if (pFormatter)    
  //  qsDpi = getIApp()->toQString(pFormatter->formatL(nDpi, OdDbUnitsFormatter::kDecimal, 0, 0, 0));
  m_pDpiEdit->blockSignals(true);
  m_pDpiEdit->setText(qsDpi);
  m_pDpiEdit->setEnabled(level == OdDbPlotSettings::kCustom);
  m_pDpiEdit->blockSignals(false);
}

void OdqPlotDialog::fillPlotOptions()
{
  //unsupported //m_pPlotInBackgroundCheckBox 

  bool bPrintLineweights = m_pPlotSettings->printLineweights() || m_pPlotSettings->plotPlotStyles();
  m_pPlotObjectLineweightsCheckBox->blockSignals(true);
  m_pPlotObjectLineweightsCheckBox->setCheckState(bPrintLineweights ? Qt::Checked : Qt::Unchecked);
  m_pPlotObjectLineweightsCheckBox->setEnabled(!m_pPlotSettings->plotPlotStyles());
  m_pPlotObjectLineweightsCheckBox->blockSignals(false);

  m_pPlotWithPlotStylesCheckBox->blockSignals(true);
  m_pPlotWithPlotStylesCheckBox->setCheckState(m_pPlotSettings->plotPlotStyles() ? Qt::Checked 
                                                                                 : Qt::Unchecked);
  m_pPlotWithPlotStylesCheckBox->blockSignals(false);

  m_pPlotPaperspaceLastCheckBox->blockSignals(true);
  m_pPlotPaperspaceLastCheckBox->setCheckState(m_pPlotSettings->drawViewportsFirst() ? Qt::Checked 
                                                                                     : Qt::Unchecked);
  m_pPlotPaperspaceLastCheckBox->setEnabled(!m_pPlotSettings->modelType());
  m_pPlotPaperspaceLastCheckBox->blockSignals(false);
  
  m_pHidePaperspaceObjectsCheckBox->blockSignals(true);
  m_pHidePaperspaceObjectsCheckBox->setCheckState(m_pPlotSettings->plotHidden() ? Qt::Checked 
                                                                                : Qt::Unchecked);
  m_pHidePaperspaceObjectsCheckBox->setEnabled(!m_pPlotSettings->modelType());
  m_pHidePaperspaceObjectsCheckBox->blockSignals(false);

  // unsipported //m_pPlotStampOnCheckBox

  m_pSaveChangesToLayoutCheckBox->blockSignals(true);
  bool bSave = getIAppProps()->toBool(OD_T("Vars/plotsavechangestolayout"), false);
  m_pSaveChangesToLayoutCheckBox->setCheckState(bSave ? Qt::Checked : Qt::Unchecked);
  m_pSaveChangesToLayoutCheckBox->blockSignals(false);
}

void OdqPlotDialog::fillDrawingOrientation()
{
  // change paper orientation to dialog values

  OdDbPlotSettings::PlotRotation rotation = m_pPlotSettings->plotRotation();
  QString qsImage = ":/images/plot/";

  m_pPortraitRadio->blockSignals(true);
  m_pLandscapeRadio->blockSignals(true);
  m_pUpsideDownCheckBox->blockSignals(true);
  bool isWidthLessHeight = isPaperWidthLessHeight();
  switch (rotation)
  {
  default:
    ODA_FAIL_ONCE(); // ? TODO
  case OdDbPlotSettings::k0degrees: // No rotation
    qsImage += isWidthLessHeight ? "orientation_p.png" : "orientation_l.png";
    m_pPortraitRadio->setChecked(isWidthLessHeight); // CORE-17447 // (true);
    m_pLandscapeRadio->setChecked(!isWidthLessHeight);
    m_pUpsideDownCheckBox->setCheckState(Qt::Unchecked);
    break;
  case OdDbPlotSettings::k90degrees: // 90° CW // CCW (for paper)
    qsImage += isWidthLessHeight  ? "orientation_l.png" : "orientation_p.png";
    m_pPortraitRadio->setChecked(!isWidthLessHeight);
    m_pLandscapeRadio->setChecked(isWidthLessHeight); // CORE-17447 // (true);
    m_pUpsideDownCheckBox->setCheckState(Qt::Unchecked);
    break;
  case OdDbPlotSettings::k180degrees: // Inverted
    qsImage += isWidthLessHeight ? "orientation_p_rev.png" : "orientation_l_rev.png";
    m_pPortraitRadio->setChecked(isWidthLessHeight); // CORE-17447 // (true);
    m_pLandscapeRadio->setChecked(!isWidthLessHeight);
    m_pUpsideDownCheckBox->setCheckState(Qt::Checked);
    break;
  case OdDbPlotSettings::k270degrees: // 90° CCW // CW (for paper)
    qsImage += isWidthLessHeight ? "orientation_l_rev.png" : "orientation_p_rev.png";
    m_pPortraitRadio->setChecked(!isWidthLessHeight);
    m_pLandscapeRadio->setChecked(isWidthLessHeight); // CORE-17447 // (true);
    m_pUpsideDownCheckBox->setCheckState(Qt::Checked);
    break;
  }

  QPixmap pixmap(qsImage);
  pixmap.setMask(pixmap.createHeuristicMask());
  m_pOrientationLabel->setPixmap(pixmap);
  m_pUpsideDownCheckBox->blockSignals(false);
  m_pLandscapeRadio->blockSignals(false);
  m_pPortraitRadio->blockSignals(false);
}

