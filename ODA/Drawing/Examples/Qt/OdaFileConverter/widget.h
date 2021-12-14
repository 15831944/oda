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
// widget.h - renamed from window.h as temp solution for #8614
//

#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>

QT_BEGIN_NAMESPACE
  class QLineEdit;
  class QCheckBox;
  class QComboBox;
  class QGroupBox;
  class QPushButton;
  class QProgressBar;
  class QUrl;
  class QTextBrowser;
QT_END_NAMESPACE

class Window : public QWidget
{
  Q_OBJECT

public:
  Window(const QStringList& args);
  virtual ~Window();

  void setConvertor(class CConvertor* pCovertor);
  bool isValid() const;
  static QString getInitialDocPath(); 

protected:
 virtual void closeEvent(QCloseEvent *);

private slots:
  void selectInputDirectory();
  bool changedInputDirectory();
  void selectOutputDirectory();
  bool changedOutputDirectory();
  void pressedStartStopButton();

  void changedBannerSource(const QUrl & url);
  void clickedBannerAnchor(const QUrl& url);

private:
  QLineEdit* m_pInputPath; 
  QLineEdit* m_pOutputPath;
  QLineEdit* m_pStateBar;
  QCheckBox* m_pRecurseFoldersCheckBox;
  QCheckBox* m_pAuditCheckBox;
  QComboBox* m_pInputFileFilter;
  QComboBox* m_pOutputVersion;
  QPushButton* m_pStartStopButton;
  QProgressBar* m_pProgressBar;
  QTextBrowser* m_pBannerBrowser;

  enum {
    state_Invalid
  , state_Normal
  , state_CmdLine
  } m_enState;
  class CConvertor* m_pCovertor;
};

#endif
