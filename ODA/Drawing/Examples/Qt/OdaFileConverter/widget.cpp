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
// widget.cpp - renamed from window.cpp as temp solution for #8614
//
#include <QtCore>
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtGui>
#else
#include <QtWidgets>
#endif

#include <QDir>
#include <QSettings>
#include <QErrorMessage>
#include <QDesktopServices>

#include "widget.h"
#include "convertor.h" 
#include "TDVersion.h"

#include "OdqTdSaveAsFilters.h"

static int s_countDefaultInputFileFilters = 0;

Window::Window(const QStringList& args)
  : m_pCovertor(NULL)
  , m_enState(state_Invalid)
{
  unsigned int argc = args.size();
  if (argc > 1)
  {
    if (argc != 7 && argc != 8)
    {
      QString qsTxt = QObject::tr(
        "Command Line Format is:\n"
        "  Quoted Input Folder\n"
        "  Quoted Output Folder\n"
        //"  Output version { \"ACAD9\",\"ACAD10\",\"ACAD12\",\"ACAD13\",\"ACAD14\","
        //                   "\"ACAD2000\",\"ACAD2004\",\"ACAD2007\",\"ACAD2010\" }\n"
        // after problem with long strings in QMessageBox
        "  Output_version\n"
        "    {\"ACAD9\",\"ACAD10\",\"ACAD12\",\n"
        "     \"ACAD13\",\"ACAD14\",\n"
        "     \"ACAD2000\",\"ACAD2004\",\n"
        "     \"ACAD2007\",\"ACAD2010\",\n"
        "     \"ACAD2013\",\"ACAD2018\"}\n"
        "  Output File type\n"
        "    {\"DWG\",\"DXF\",\"DXB\"}\n"
        "  Recurse Input Folder\n"
        "    {\"0\",\"1\"}\n"
        "  Audit each file\n"
        "    {\"0\",\"1\"}\n"
        "  [optional] Input files filter\n"
      //"    (default: \"*.DWG;*.DXF;*.DXB\")\n");
        "    (default: \"*.DWG;*.DXF\")\n");
      QString qsTitle = QObject::tr("ODA File Converter");
#if 1
      QMessageBox::information(QApplication::activeWindow(), qsTitle, qsTxt);
#else
      // TODO
      // http://bugreports.qt.nokia.com/browse/QTBUG-7851?page=com.atlassian.jira.plugin.system.issuetabpanels%3Aall-tabpanel
      // #QTBUG-7851 QMessageBox does not resize. 
      // http://bugreports.qt.nokia.com/browse/QTBUG-3696
      // QTBUG-3696 Cannot make a QMessageBox resizable
      // Out of scope - This has been like that for years.

      QMessageBox msgBox(QMessageBox::Information, qsTitle, qsTxt, QMessageBox::Ok, QApplication::activeWindow());
      msgBox.adjustSize();
      //msgBox.setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
      //msgBox.setSizeGripEnabled(true);
      //msgBox.setAttribute(Qt::WA_Resized);
      QDesktopWidget* pDesktop = QApplication::desktop();
      QSize sizeScreen = pDesktop->screenGeometry(this).size(); // primary screen
      QRect rect = msgBox.geometry();
      int w = msgBox.width(),
          h = msgBox.height();
      //w = w * 2;
      int x = (sizeScreen.width() - w) / 2,
          y = (sizeScreen.height() - h) / 2;
      //msgBox.setGeometry(x, y, w, h);

      QErrorMessage errMsg(QApplication::activeWindow());
      //errMsg.setWindowTitle(qsTitle);
      errMsg.showMessage(qsTxt, "");
      errMsg.setGeometry(x, y, w, h);
      errMsg.exec();
#endif
      return;
    }
    m_enState = state_CmdLine;
  }
  else
    m_enState = state_Normal;

  QVBoxLayout *mainLayout = new QVBoxLayout;

  QGroupBox* pInputFolder = new QGroupBox(tr("Input Folder"));
  
  QSettings settings("ODA", 
                     QFileInfo(QCoreApplication::applicationFilePath()).baseName());
  settings.beginGroup(TD_SHORT_STRING_VER_S);
  bool areSettingsValid = settings.contains("InputPath");

  QString qsInitialPath = settings.value("InputPath").toString();
  if (!areSettingsValid)
  {
    ODA_ASSERT_ONCE(qsInitialPath.isEmpty());

    qsInitialPath = Window::getInitialDocPath();
#if __IPHONE_OS_VERSION_MIN_REQUIRED
    qsInitialPath.replace("OdaFileConverter.app", "Documents");
#endif
  }
  m_pInputPath = new QLineEdit(qsInitialPath);
  m_pInputPath->setReadOnly(true);

  QPushButton* pInputBrowserButton = new QPushButton("...");
  QHBoxLayout* pInputFolderLayout = new QHBoxLayout;
  pInputFolderLayout->addWidget(m_pInputPath);
  pInputFolderLayout->addWidget(pInputBrowserButton);
  pInputFolder->setLayout(pInputFolderLayout);
  mainLayout->addWidget(pInputFolder);

  QObject::connect(pInputBrowserButton, SIGNAL(clicked()), 
                   this, SLOT(selectInputDirectory()), Qt::QueuedConnection);
  QObject::connect(m_pInputPath, SIGNAL(editingFinished()), 
                   this, SLOT(changedInputDirectory()), Qt::QueuedConnection);

  QGroupBox* pOutputFolder = new QGroupBox(tr("Output Folder"));
  qsInitialPath = settings.value("OutputPath").toString();
    
#if __IPHONE_OS_VERSION_MIN_REQUIRED
  if (qsInitialPath.isEmpty())
  {
    ODA_ASSERT_ONCE(qsInitialPath.isEmpty());
    qsInitialPath = Window::getInitialDocPath();
    qsInitialPath.replace("OdaFileConverter.app", "Documents/Output");
  }
#endif
  m_pOutputPath = new QLineEdit(qsInitialPath);
  QPushButton* pOutputBrowserButton = new QPushButton("...");
  QHBoxLayout* pOutputFolderLayout = new QHBoxLayout;
  pOutputFolderLayout->addWidget(m_pOutputPath);
  pOutputFolderLayout->addWidget(pOutputBrowserButton);
  pOutputFolder->setLayout(pOutputFolderLayout);
  mainLayout->addWidget(pOutputFolder);

  QObject::connect(pOutputBrowserButton, SIGNAL(clicked()), 
                   this, SLOT(selectOutputDirectory()), Qt::QueuedConnection);
  QObject::connect(m_pOutputPath, SIGNAL(editingFinished()), 
                   this, SLOT(changedOutputDirectory()), Qt::QueuedConnection);

  QWidget* pGridBox = new QWidget(); //QGroupBox* pGridBox = new QGroupBox;
  QGridLayout* pGridLayout = new QGridLayout;

  m_pRecurseFoldersCheckBox = new QCheckBox(tr("Recurse folders"));
  bool bVal = settings.value("Recurse", false).toBool();
  m_pRecurseFoldersCheckBox->setCheckState(bVal ? Qt::Checked : Qt::Unchecked);
  pGridLayout->addWidget(m_pRecurseFoldersCheckBox, 0, 0);
  //connect(m_pRecurseFoldersCheckBox, SIGNAL(toggled(bool)),
  //        this, SLOT(filterRegExpChanged()));

  m_pAuditCheckBox = new QCheckBox(tr("Audit"));
  bVal = settings.value("Audit", true).toBool();
  m_pAuditCheckBox->setCheckState(bVal ? Qt::Checked : Qt::Unchecked);
  pGridLayout->addWidget(m_pAuditCheckBox, 1, 0);
  //connect(m_pAuditCheckBox, SIGNAL(toggled(bool)),
  //        this, SLOT(filterRegExpChanged()));

  m_pInputFileFilter = new QComboBox;
  m_pInputFileFilter->addItem("*.DWG");
  m_pInputFileFilter->addItem("*.DXF");
  //m_pInputFileFilter->addItem("*.DXB");
  m_pInputFileFilter->addItem("*.DWG ; *.DXF");
  //m_pInputFileFilter->addItem("*.DWG ; *.DXF ; *.DXB");
  s_countDefaultInputFileFilters = m_pInputFileFilter->count();
  int indexFileFilter = settings.value("FilterIndex", 
                                       s_countDefaultInputFileFilters - 1).toInt();
  ODA_ASSERT_ONCE(   indexFileFilter >= 0 
                  && indexFileFilter < s_countDefaultInputFileFilters);
  m_pInputFileFilter->setCurrentIndex(indexFileFilter);
  m_pInputFileFilter->setEditable(true);

  QLabel* pInputFileFilterLabel = new QLabel(tr("Input files filter:"));
  pInputFileFilterLabel->setBuddy(m_pInputFileFilter);
  pGridLayout->addWidget(pInputFileFilterLabel, 0, 1, 1, 1, Qt::AlignRight);
  pGridLayout->addWidget(m_pInputFileFilter, 0, 2);

  m_pOutputVersion = new QComboBox;
  static OdqTdSaveAsFilters s_filters("");
  foreach (QString qsFlt, s_filters.getFilters())
  {
    OdDb::DwgVersion ver = s_filters.getDwgVer(qsFlt);
    m_pOutputVersion->addItem(qsFlt, ver);
  }
  int indexOutputVersion = settings.value("OutputIndex", 0).toInt();
  ODA_ASSERT_ONCE(   indexOutputVersion >= 0
                  && indexOutputVersion < m_pOutputVersion->count());
  m_pOutputVersion->setCurrentIndex(indexOutputVersion); 
  QLabel* pOutputVersionLabel = new QLabel(tr("Output version:"));
  pOutputVersionLabel->setBuddy(m_pOutputVersion);
  pGridLayout->addWidget(pOutputVersionLabel, 1, 1, 1, 1, Qt::AlignRight);
  pGridLayout->addWidget(m_pOutputVersion, 1, 2);

  QGroupBox* pStartStopButtonBox = new QGroupBox;
  m_pStartStopButton = new QPushButton(tr("Start"));
  QHBoxLayout* pStartStopButtonLayout = new QHBoxLayout;
  pStartStopButtonLayout->addWidget(m_pStartStopButton);
  pStartStopButtonBox->setLayout(pStartStopButtonLayout);
  pGridLayout->addWidget(pStartStopButtonBox, 0, 3, 2, 1);
  connect(m_pStartStopButton, SIGNAL(clicked()),
          this, SLOT(pressedStartStopButton()));

  pGridBox->setLayout(pGridLayout);
  mainLayout->addWidget(pGridBox);

  QGroupBox* pProgressBox = new QGroupBox(tr("Folder progress"));
  QLayout* pProgressLayout = new QVBoxLayout;
  m_pStateBar = new QLineEdit;
  m_pStateBar->setEnabled(false); //pState->setReadOnly(true);
  pProgressLayout->addWidget(m_pStateBar);
  m_pProgressBar = new QProgressBar;
  m_pProgressBar->setTextVisible(false);
  pProgressLayout->addWidget(m_pProgressBar);
  pProgressBox->setLayout(pProgressLayout);
  mainLayout->addWidget(pProgressBox);

  m_pBannerBrowser = new QTextBrowser; //QTextEdit* pBanner = new QTextEdit;
  m_pBannerBrowser->setFrameStyle(QFrame::NoFrame);
  // unfortunately urls do not work with it //m_pBannerBrowser->setEnabled(false);
  m_pBannerBrowser->setReadOnly(true);  
  connect(m_pBannerBrowser, SIGNAL(sourceChanged(QUrl)), 
          this, SLOT(changedBannerSource(QUrl)), Qt::QueuedConnection);
  connect(m_pBannerBrowser, SIGNAL(anchorClicked(QUrl)), 
          this, SLOT(clickedBannerAnchor(QUrl)));
  m_pBannerBrowser->setSource(QUrl("qrc:/about.html"));
  mainLayout->addWidget(m_pBannerBrowser);

  setLayout(mainLayout);

  setWindowTitle(tr("ODA File Converter"));
  // usable for Mac and Linux
  QPixmap pixmap(QLatin1String(":odalogo.png"));
  ODA_ASSERT_ONCE(!pixmap.isNull());
  pixmap.setMask(pixmap.createHeuristicMask());
  setWindowIcon(QIcon(pixmap));

  {
    int w = settings.value("w", 700).toInt(),
        h = settings.value("h", 640).toInt();
    //resize(w, h);

    QDesktopWidget* pDesktop = QApplication::desktop();
    QSize sizeScreen = pDesktop->screenGeometry(this).size(); // primary screen
    QSize sizeDesktop = pDesktop->geometry().size(); // full virtual size
    if (!sizeDesktop.isValid())
      sizeDesktop = sizeScreen;

    int x = settings.value("x", (sizeScreen.width() - w) / 2).toInt(),
        y = settings.value("y", (sizeScreen.height() - h) / 2).toInt();

    if (   w < 400 || h < 300 || x < 0 || y < 0
        || (x + w) > sizeDesktop.width() || (y + h) > sizeDesktop.height())
    {
      // restore default
      w = 700;
      h = 580;
      x = (sizeScreen.width() - w) / 2;
      y = (sizeScreen.height() - h) / 2;
    }

    setGeometry(x, y, w, h);
  }

  settings.endGroup();

  if (m_enState == state_CmdLine)
  {
    // fill controls via command line arguments
    //
    // sample of test cmd:
    // OdaFileConverter.exe "d:\_Oda\data\_qt\in" d:\_Oda\data\_qt\out_cmd_line "ACAD2010" DXF 1 1 "*.dwg"

    // arg 1 - Input Folder
    qsInitialPath = args.at(1);
    if (qsInitialPath.isEmpty())
    {
      QMessageBox::critical(QApplication::activeWindow(), QObject::tr("ODA File Converter error"),
                            tr("Input Folder is empty."));
      m_enState = state_Invalid;
      return;
    }
    m_pInputPath->setText(qsInitialPath);
    if (!changedInputDirectory())
    {
      QMessageBox::critical(QApplication::activeWindow(), QObject::tr("ODA File Converter error"),
                            tr("Input Folder \"%1\" does not exist.").arg(qsInitialPath));
      m_enState = state_Invalid;
      return;
    }

    // arg 2 - Output Folder
    qsInitialPath = args.at(2);
    if (qsInitialPath.isEmpty())
    {
      QMessageBox::critical(QApplication::activeWindow(), QObject::tr("ODA File Converter error"),
                            tr("Output Folder is empty."));
      m_enState = state_Invalid;
      return;
    }
    m_pOutputPath->setText(qsInitialPath);
    if (!changedOutputDirectory())
    {
      QMessageBox::critical(QApplication::activeWindow(), QObject::tr("ODA File Converter error"),
                            tr("Output Folder \"%1\" is invalid.").arg(qsInitialPath));
      m_enState = state_Invalid;
      return;
    }

    // arg 3 - Output_version
    QString qsValue = args.at(3).toUpper();
    int lenVer;
    if (   !qsValue.startsWith("ACAD") 
        || !(lenVer = qsValue.remove("ACAD").length())
        || lenVer == 3 || lenVer > 4)
    {
      QMessageBox::critical(QApplication::activeWindow(), QObject::tr("ODA File Converter error"),
                            tr("Output version \"%1\" is invalid.").arg(qsValue));
      m_enState = state_Invalid;
      return;
    }
    QString qsOutVer = (lenVer < 4) ? QString("R%1").arg(qsValue) : qsValue;

    // arg 4 - Output File type
    qsValue = args.at(4).toUpper();
    if (qsValue == "DWG")
      qsOutVer += " DWG";
    else if (qsValue == "DXF")
      qsOutVer += " ASCII DXF";
    else if (qsValue == "DXB")
      qsOutVer += " Binary DXF";
    else
    {
      QMessageBox::critical(QApplication::activeWindow(), QObject::tr("ODA File Converter error"),
                            tr("Output File type \"%1\" is invalid.").arg(qsValue));
      m_enState = state_Invalid;
      return;
    }

    int index = m_pOutputVersion->findText(qsOutVer);
    if (index < 0)
    {
      QMessageBox::critical(QApplication::activeWindow(), QObject::tr("ODA File Converter error"),
                            tr("Output version \"%1\" for file type \"%2\" is unsupported.")
                              .arg(args.at(3).toUpper())
                              .arg(qsValue));
      m_enState = state_Invalid;
      return;
    }
    m_pOutputVersion->setCurrentIndex(index);

    // arg 5 - Recurse Input Folder
    qsValue = args.at(5).toUpper();
    bVal = !(qsValue.isEmpty() || qsValue == "0" || qsValue == "off");
    m_pRecurseFoldersCheckBox->setCheckState(bVal ? Qt::Checked : Qt::Unchecked);

    // arg 6 - Audit each file
    qsValue = args.at(6).toUpper();
    bVal = !(qsValue.isEmpty() || qsValue == "0" || qsValue == "off");
    m_pAuditCheckBox->setCheckState(bVal ? Qt::Checked : Qt::Unchecked);

    // arg 7 - [optional] Input files filter
    qsValue.clear();
    if (argc > 7)
      qsValue = args.at(7).toUpper();
    if (qsValue.isEmpty())
      qsValue = "*.DWG;*.DXF"; // default //qsValue = "*.DWG;*.DXF;*.DXB";
    qsValue.replace("; ", ";").replace(" ;", ";").replace(";", " ; ");
    index = m_pInputFileFilter->findText(qsValue);
    // #9764 
    if (index < 0)
    {
      m_pInputFileFilter->addItem(qsValue);
      index = m_pInputFileFilter->findText(qsValue);
    }
    if (index < 0)
    {
      QMessageBox::critical(QApplication::activeWindow(), QObject::tr("ODA File Converter error"),
                            tr("Files filter \"%1\" is invalid.").arg(qsValue));
      m_enState = state_Invalid;
      return;
    }
    m_pInputFileFilter->setCurrentIndex(index);

    QTimer::singleShot(200, this, SLOT(pressedStartStopButton())); 
  }
}

Window::~Window()
{
  if (m_enState != state_Normal)
    return;
  ODA_ASSERT_ONCE(m_pCovertor);

  QSettings settings("ODA", 
                     QFileInfo(QCoreApplication::applicationFilePath()).baseName());
  settings.beginGroup(TD_SHORT_STRING_VER_S);

  settings.setValue("InputPath", m_pInputPath->text());
  settings.setValue("OutputPath", m_pOutputPath->text());
  settings.setValue("Recurse", m_pRecurseFoldersCheckBox->checkState() == Qt::Checked);
  settings.setValue("Audit", m_pAuditCheckBox->checkState() == Qt::Checked);
  int index = m_pInputFileFilter->currentIndex();
  if (index < s_countDefaultInputFileFilters)
    settings.setValue("FilterIndex", index);
  settings.setValue("OutputIndex", m_pOutputVersion->currentIndex());
  QRect rect = geometry();
  settings.setValue("x", rect.x());
  settings.setValue("y", rect.y());
  settings.setValue("w", rect.width());
  settings.setValue("h", rect.height());

  settings.endGroup();
}

void Window::setConvertor(class CConvertor* pCovertor)
{
  ODA_ASSERT(pCovertor && !m_pCovertor);
  m_pCovertor = pCovertor;    
}

bool Window::isValid() const
{
  if (!m_pCovertor)
    return false;
  return m_enState != state_Invalid;
}

// slots:

void Window::selectInputDirectory()
{
  QString qsPath = QFileDialog::getExistingDirectory(parentWidget(), 
                                                     QString(), 
                                                     m_pInputPath->text());
  if (!qsPath.isEmpty())
  {
    m_pInputPath->setText(qsPath);
  }
}

bool Window::changedInputDirectory()
{
  QDir dir(m_pInputPath->text());
  if (dir.exists())
  {
    m_pInputPath->setText(dir.absolutePath());
    return true;
  }
  
  static QString qsInputPathDefault;
  if (qsInputPathDefault.isEmpty())
  {
    QSettings settings("ODA", 
      QFileInfo(QCoreApplication::applicationFilePath()).baseName());
    qsInputPathDefault = settings.value(TD_SHORT_STRING_VER_S "/InputPath").toString();
  }

  m_pInputPath->setText(qsInputPathDefault);
  return false;
}

void Window::selectOutputDirectory()
{
  QString qsPath = QFileDialog::getExistingDirectory(parentWidget(), 
                                                     QString(), 
                                                     !m_pOutputPath->text().isEmpty() 
                                                       ? m_pOutputPath->text() : m_pInputPath->text());
  if (!qsPath.isEmpty())
  {
    m_pOutputPath->setText(qsPath);
  }
}

bool Window::changedOutputDirectory()
{
  QDir dir(m_pOutputPath->text());
  if (dir.exists())
  {
    m_pOutputPath->setText(dir.absolutePath());
    return true;
  }

  static QString qsOutputPathDefault;
  if (qsOutputPathDefault.isEmpty())
  {
    QSettings settings("ODA", 
                       QFileInfo(QCoreApplication::applicationFilePath()).baseName());
    qsOutputPathDefault = settings.value(TD_SHORT_STRING_VER_S "/OutputPath").toString();
  }

  if (   m_enState != state_CmdLine
      && QMessageBox::Yes != QMessageBox::question(
            QApplication::activeWindow(), 
            QObject::tr("ODA File Converter"), // caption
            QObject::tr("Directory %1 does not exist.\nDo you want to create it.")
              .arg(m_pOutputPath->text()),
            QMessageBox::Yes | QMessageBox::No,
            QMessageBox::Yes))
  {
    m_pOutputPath->setText(qsOutputPathDefault);
    return false;
  }

  if (!QDir().mkpath(m_pOutputPath->text()))
  {
    QMessageBox::warning(
      QApplication::activeWindow(), QObject::tr("Error"),
      QObject::tr("Unable to create output folder [%1]").arg(m_pOutputPath->text()));
    m_pOutputPath->setText(qsOutputPathDefault);
    return false;
  }

  m_pOutputPath->setText(QDir(m_pOutputPath->text()).absolutePath());
  return true;
}

void Window::pressedStartStopButton()
{
  if (!m_pCovertor)
    return;

  if (m_pCovertor->isBusy())
  {
    m_pCovertor->stopConvertion();
    return;
  }

  QString qsInputPath = m_pInputPath->text(), 
          qsOutputPath = m_pOutputPath->text(),
          qsInputFileWildcard = m_pInputFileFilter->currentText();

  QVariant qvData = m_pOutputVersion->itemData(m_pOutputVersion->currentIndex());
  OdDb::DwgVersion versionSaveAs = (OdDb::DwgVersion) qvData.toUInt();

  OdDb::SaveType typeSaveAs = OdDb::kDwg;
  QString qsVersion = m_pOutputVersion->currentText();
  if (qsVersion.indexOf("ASCII DXF", Qt::CaseInsensitive) >= 0)
    typeSaveAs = OdDb::kDxf;
  else if (qsVersion.indexOf("Binary DXF", Qt::CaseInsensitive) >= 0)
    typeSaveAs = OdDb::kDxb;
  else
    ODA_ASSERT_ONCE(qsVersion.indexOf("DWG", Qt::CaseInsensitive) >= 0);

  m_pStartStopButton->setText(tr("Stop"));
  m_pCovertor->startConvertion(qsInputPath, qsInputFileWildcard,
                               m_pRecurseFoldersCheckBox->checkState() != Qt::Unchecked,
                               m_pAuditCheckBox->checkState() != Qt::Unchecked,
                               qsOutputPath,
                               versionSaveAs, typeSaveAs, 
                               m_pStateBar, m_pProgressBar,
                               NULL,
                               m_enState == state_CmdLine); // silent mode

  m_pStartStopButton->setText(tr("Start"));
  if (m_enState == state_CmdLine)
    close();
}

void Window::closeEvent(QCloseEvent *pEvent)
{
  //QWidget::closeEvent(pEvent);

  if (m_pCovertor->isBusy())
  {
    //m_pCovertor->stopConvertion(true); // force
    m_pCovertor->stopConvertion();
    pEvent->ignore();
    return;
  }
  pEvent->accept();
}

void Window::changedBannerSource(const QUrl& url)
{
  //QString qsPath = url.path();
  QString qsText = m_pBannerBrowser->toHtml();
  QString qsCopyrightDD = toQString(TD_COPYRIGHT_W);
  qsCopyrightDD.replace("Copyright ", "Copyright"); // remove useless space
  QString qsCompanyNameDD = qsCopyrightDD;
  qsCompanyNameDD.remove(" ('Open Design')", Qt::CaseInsensitive);
  qsText.replace("$(TD_COMPANY_NAME)", qsCompanyNameDD, Qt::CaseInsensitive);
  //qsCopyrightDD.replace("\xA9", "&copy;");
  //qsCopyrightDD.replace(" Inc. ('Open Design')", ".");
  //qsCopyrightDD.replace("2012", "2013"); // temp solution // TODO: remove after realese v3.? !!!
  qsText.replace("$(TD_COPYRIGHT)", qsCopyrightDD, Qt::CaseInsensitive);

  qsText.replace("$(TD_PRODUCT_NAME)", toQString(TD_PRODUCT_NAME_W), Qt::CaseInsensitive);
  qsText.replace("$(TD_PRODUCT_NAME_START)", toQString(TD_PRODUCT_NAME_START_W), Qt::CaseInsensitive);
  // In the About box, the TM symbol for ODA SDK needs to change to ®. // From: Allison Angus February 21, 2011
  //qsText.replace("$(TD_PRODUCT_NAME)", QString(TD_PRODUCT_NAME_S).replace("ODA SDK", "ODA SDK&reg;"), Qt::CaseInsensitive);
  //qsText.replace("$(TD_PRODUCT_NAME_START)", QString(TD_PRODUCT_NAME_START_S).replace("ODA SDK™", "ODA SDK&reg;"), Qt::CaseInsensitive);

  //QString qsVerTD =  TD_PRODUCT_VER_STR_S; // TD_SHORT_STRING_VER_S // "3.02"
  //qsVerTD.replace(", ", "."); // for common style with Qt (same with DD short ver style)
  QString qsVerTD = QString("%1.%2.%3").arg(TD_MAJOR_VERSION).arg(TD_MINOR_VERSION).arg(TD_MAJOR_BUILD);
 #if TD_MINOR_BUILD > 0
  qsVerTD += QString(".%1").arg(TD_MINOR_BUILD)
 #endif
  qsText.replace("$(TD_VER_STR)", qsVerTD, Qt::CaseInsensitive);
  qsText.replace("$(TD_SHORT_VER_STR)", TD_SHORT_STRING_VER_S, Qt::CaseInsensitive);
  qsText.replace("$(QT_VER_STR)", QT_VERSION_STR, Qt::CaseInsensitive); // "4.5.3"

  // set invisible background color
  // (unfortunately m_pBannerBrowser->setTextBackgroundColor is not usable for it)
  QPalette palette = qApp->palette();
  QColor colorSysGrey = palette.color(QPalette::Normal, QPalette::Window); //Base);
  int red = colorSysGrey.red(), // 236
      green = colorSysGrey.green(), // 233
      blue = colorSysGrey.blue(); // 216
  QString qsSysGrey = QString("bgcolor=\"#%1%2%3\"").arg(red, 2, 16, QLatin1Char('0'))
                                                    .arg(green, 2, 16, QLatin1Char('0'))
                                                    .arg(blue, 2, 16, QLatin1Char('0'));
  qsSysGrey = qsSysGrey.toUpper();
  qsText.replace("bgcolor=\"#ffffff\"", qsSysGrey, Qt::CaseInsensitive);
  m_pBannerBrowser->setHtml(qsSysGrey);

  m_pBannerBrowser->setHtml(qsText);
}

void Window::clickedBannerAnchor(const QUrl& url)
{
  m_pBannerBrowser->setSource(m_pBannerBrowser->source());
  ODA_ASSERT_ONCE(url.scheme() == "http"); // TODO

  // open with standard OS browser
  if (!QDesktopServices::openUrl(url))
  {
    ODA_FAIL_ONCE(); 
  }
}

//static 
QString Window::getInitialDocPath()
{
  QString qsInitialPath;
#ifndef QT_DEBUG
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
  qsInitialPath = QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation);
#else
  qsInitialPath = QStandardPaths::displayName(QStandardPaths::DocumentsLocation);
#endif
 #ifdef ANDROID
  if (qsInitialPath.length() <= 1)
 #endif
    qsInitialPath = "/sdcard";
#else
  qsInitialPath = QCoreApplication::applicationDirPath();
  qsInitialPath.replace("\\", "/");
  int pos = qsInitialPath.lastIndexOf("Oda/", -1, Qt::CaseInsensitive);
  if (pos > 0)
  {
    if (QDir(qsInitialPath.left(pos + 3)).cd("data"))
      qsInitialPath = qsInitialPath.left(pos + 3) + "/data";
  }
#endif
  return qsInitialPath;
}
