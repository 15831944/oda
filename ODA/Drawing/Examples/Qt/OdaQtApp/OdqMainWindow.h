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
// OdqMainWindow.h
//

#ifndef ODQ_MAIN_WINDOW_H_
#define ODQ_MAIN_WINDOW_H_

#include <QMainWindow>

QT_BEGIN_NAMESPACE
  class QTextBrowser;
  class QUrl;
  //class QProgressBar;
QT_END_NAMESPACE

#include "OdaCommon.h"
#include "SharedPtr.h"

class OdqMainWindow : public QMainWindow
{
  Q_OBJECT

public:
  OdqMainWindow();
  virtual ~OdqMainWindow();

  bool isValid() const;
  bool isClosed() const; // by closeEvent

  class OdRxModule* getAppModule();

  bool showUrl(const QString& qsUrl,
               const QString& qsTitle = "%1", // by app name
               bool bModal = true);
  bool isBrowserClosed() const;

  void saveWidgetPlacement(QWidget* pWidget, 
                           const QString qsKey = "") const;  // for QMainWindow
  void restoreWidgetPlacement(QWidget* pWidget, 
                              const QString qsKey = "", // for QMainWindow
                              int widthDefault = 600,
                              int heightDefault = 200) const;

  bool saveQObjectProperties(QObject* pObj,
                             const QString& qsProps = "", // an empty for all
                             const QString& qsSkipProps = "",
                             const QString& qsDelim = " ") const;
  bool restoreQObjectProperties(QObject* pObj,
                                const QString& qsProps = "", // an empty for all
                                const QString& qsSkipProps = "",
                                const QString& qsDelim = " ") const;
private:
  bool initAppModule();
  void uninitAppModule();

  //class OdqServices* getServices() const;

protected:
  //virtual void showEvent(QShowEvent* pEvent);
  //virtual void paintEvent(QPaintEvent* pEvent);

  virtual void closeEvent(QCloseEvent* pEvent);

  virtual void dragEnterEvent(QDragEnterEvent* pEvent);
  //virtual void dragMoveEvent(QDragMoveEvent* pEvent);
  ////virtual void dragLeaveEvent(QDragLeaveEvent* pEvent);
  virtual void dropEvent(QDropEvent* pEvent);

public slots:
  void revertConsoleVisibility();
  void showHelp();
  void changedBrowserSource(const QUrl& url);
  void clickedBrowserAnchor(const QUrl& url);
  void clickedBrowserCloseButton();
  void unlockCommandViaBrowser();

private:
  bool m_bValid;
  OdRxModule* m_pAppModule;
  OdSharedPtr<QTextBrowser> m_pBrowser;
  // is used to block duplicate commands via Url 
  // (cleared via unlockCommandViaBrowser by QTimer::singleShot)
  QString m_qsBrowserLastCommand; 
  bool m_bDragging;
};

#endif // ODQ_MAIN_WINDOW_H_
