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
// OdqPlotDialog.h
//

#ifndef ODA_QT_PLOT_DIALOG_H_
#define ODA_QT_PLOT_DIALOG_H_

#include <QDialog> 
#include <QPointer> 
QT_BEGIN_NAMESPACE
  class QLabel;
  class QComboBox;
  class QCheckBox;
  class QSpinBox;
  class QAbstractButton;
  class QPushButton;
  class QLineEdit;
  class QRadioButton;
QT_END_NAMESPACE

#include "OdaCommon.h"
#include "DbPlotSettings.h"
#include "ExtDbModule.h"

#include "OdqInterfaces.h"

///////////////////////////////////////////////////////////////////////////////

typedef enum {
  enPlorDlg_Rejected = QDialog::Rejected
, enPlorDlg_Accepted = QDialog::Accepted
, enPlorDlg_WindowArea
, enPlorDlg_Preview // TODO
} TPlotDialog;

class OdqPlotDialog : public QDialog
{                 
  Q_OBJECT 

public:
  OdqPlotDialog(OdRxObject* pRxDatabase,
                QWidget* pParent = NULL,
                Qt::WindowFlags flags = 0);   
  virtual ~OdqPlotDialog();

  OdDbPlotSettingsPtr plotSettings();
  class OdDbPlotSettingsValidator* plotSettingsValidator();
  OdUnitsFormatter* formatter();

  bool isAppliedToLayout() const;
  void applyToLayout(OdDbPlotSettings* pPlotSettings = NULL);

  bool isPrintToFile() const;
  int numberOfCopies() const;
  bool isPaperWidthLessHeight() const;

  void fillPageSetup();
  void fillPrinterPlotter_Device();
  void fillPrinterPlotter_Info();
  void fillPaperSizes();
  void fillNumberOfCopies();
  void fillPlotArea();
  void fillPlotOffset();
  void fillPlotScale();
  void fillPlotStyles(); 
  void fillShadedViewportOptions();
  void fillPlotOptions();
  void fillDrawingOrientation();

public slots:
  void PageSetup_currentIndexChanged(const QString& qsText);
  void PageSetupButtonAdd_clicked();
  void PrinterPlotter_currentIndexChanged(const QString& qsText);
  void PaperSizes_currentIndexChanged(const QString& qsText);
  void PlotArea_currentIndexChanged(const QString& qsText);
  void PlotAreaView_currentIndexChanged(const QString& qsText);
  void PlotAreaWindowButton_clicked();
  void PlotOffset_edited();
  void PlotOffset_changed();
  void PlotScaleFitToPaper_changed();
  void PlotScale_currentIndexChanged(const QString& qsText);
  void Unit_edited();
  void PlotUnit_currentIndexChanged(const QString& qsText);
  void PlotScaleLineweights_changed();
  void PlotStyles_currentIndexChanged(const QString& qsText);
  void PlotStyleEditButton_clicked();
  //void PlotStylesDisplay_changed();
  void Shade_currentIndexChanged(int index);
  void Quality_currentIndexChanged(int index);
  void focusChanged(QWidget* pOld, QWidget* pNew);
  void DpiEdit_edited();
  void PlotOptions_changed();
  void DrawingOrientation_changed();
  void ButtonBox_clicked(QAbstractButton* pButton);

protected:
  bool resetPageSetupName(bool bFill = true);
  OdString toCanonicalMediaName(const OdString& sLocaleMediaName);
  void PlotOffsetsToStrings(QString& qsX, QString& qsY);

  OdRxObject* m_pRxDatabase;
  OdDbPlotSettingsPtr m_pPlotSettings;
  bool m_isAppliedToLayout;

  QPointer<QWidget> m_pLeftWidget,
                    m_pRightWidget; // extended

  QPointer<QComboBox> m_pPageSetupCombo;

  QPointer<QComboBox> m_pPrinterCombo;
  QPointer<QPushButton> m_pPrinter_PropertiesButton;
  QPointer<QLabel> m_pPrinter_ConfigurationName,
                   m_pPrinter_Placement,
                   m_pPrinter_Description;
  QPointer<QCheckBox> m_pPrinter_ToFileCheckBox;

  QPointer<QComboBox> m_pPaperSizeCombo;
  QPointer<QSpinBox> m_pNumberOfCopiesSpin;

  QPointer<QComboBox> m_pPlotAreaCombo,
                      m_pPlotAreaViewsCombo;
  QPointer<QPushButton> m_pPlotAreaWindowButton;

  QPointer<QLineEdit> m_pPlotOffsetXEdit,
                      m_pPlotOffsetYEdit;
  QPointer<QLabel> m_pPlotOffsetUnitXLabel, 
                   m_pPlotOffsetUnitYLabel;
  QPointer<QCheckBox> m_pCenterThePlotCheckBox;

  QPointer<QCheckBox> m_pFitToPaperCheckBox,
                      m_pScaleLineweightsCheckBox;
  QPointer<QComboBox> m_pScaleCombo,
                      m_pUnitCombo;
  QPointer<QLineEdit> m_pUnitPaperEdit,
                      m_pUnitDrawingEdit;
  QPointer<QLabel>    m_pUnitDrawingLabel;

  QPointer<QComboBox> m_pPlotStyleFilesCombo;
  QPointer<QPushButton> m_pPlotStyleEditButton;
  // missing in new ACAD //QPointer<QCheckBox> m_pDisplayPlotStylesCheckBox;

  QPointer<QComboBox> m_pShadePlotCombo,
                      m_pQualityCombo;
  QPointer<QLineEdit> m_pDpiEdit;

  QPointer<QCheckBox> //m_pPlotInBackgroundCheckBox, 
                      m_pPlotObjectLineweightsCheckBox,
                      m_pPlotWithPlotStylesCheckBox,
                      m_pPlotPaperspaceLastCheckBox,
                      m_pHidePaperspaceObjectsCheckBox,
                      //m_pPlotStampOnCheckBox,
                      m_pSaveChangesToLayoutCheckBox;

  QPointer<QRadioButton> m_pPortraitRadio,
                         m_pLandscapeRadio;
  QPointer<QCheckBox> m_pUpsideDownCheckBox;
  QPointer<QLabel> m_pOrientationLabel;

  QPointer<QPushButton> m_pCancel, 
                        m_pMore; 
};  

#endif // ODA_QT_PLOT_DIALOG_H_
