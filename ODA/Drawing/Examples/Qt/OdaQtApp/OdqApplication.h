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
// OdqApplication.h
//

#ifndef ODA_QT_APPLICATION_H_
#define ODA_QT_APPLICATION_H_

#include <QApplication>
#include <QHash>
#include <QProgressBar>
#if defined(Q_WS_MAC) || defined(Q_OS_MAC) || defined(__APPLE__)
#include <QFileOpenEvent>
#endif //Q_OS_MAC

#include "OdaCommon.h"
#include "OdString.h"
#include "SharedPtr.h"

#include "OdqInterfaces.h"

#define STL_USING_MAP
#include "OdaSTL.h"

class OdqApplication : public QApplication
                     , public IqApplication
{
  Q_OBJECT
  Q_INTERFACES(IqApplication)

  int m_modeMeter;
  unsigned int m_curMeter, m_maxMeter, m_dumpedMeter;
  OdString m_sMeterPrefix, m_sMeterText, m_sMeterTextToHide;
  QPointer<QProgressBar> m_pMeter;

public:
  OdqApplication(int &argc, char **argv);
  virtual ~OdqApplication();

  // ----- IqApplication -----

  virtual QString getAppName() const;
  virtual QMainWindow* getMainWindow() const;
  virtual OdDbBaseHostAppServices* getBaseHostAppServices() const;

  virtual bool registerQObject(const QString& qsKeys,
                               QObject* pObj);
  virtual bool unregisterQObject(const QString& qsKeys,
                                 QObject* pObj = NULL); // all
  virtual QList<QPointer<QObject> > getQObjects(const QString& qsKey) const;
  virtual QObject* getSingle(const QString& qsKey) const;

  virtual void refreshQObjetcts(const QString& qsKey,
                                const QString& qsArg = "",
                                const char* pcszMember = SLOT(refresh(const QString&, const QString&)));

  virtual void toStatusBar(const QString& qsText = "", int timeout = 0);
  virtual void setMeterLimit(unsigned int max = 100); // specify the maximum number of times to increment
  virtual void startMeter(const QString& qsText = ""); // show progress bar (zero level)
  virtual void stopMeter(int timeout = 0); // hide progress meter
  virtual void stepMeter(); // increment the progress meter

  virtual OdString toOdString(const QString& qsStr) const;
  virtual QString toQString(const OdString& sStr) const;

  virtual QString encodeToXmlAttrib(const QString& qsStr) const;
  virtual QString decodeFromXmlAttrib(const QString& qsStr) const;

  virtual QString getExtensionByFilter(const QString& qsSelectedFilter,
                                       const QString& qsDelim = "") const; // return only 1st if empty
  
  virtual OdStringArray getExtDbModuleNames(OdRxObject* pRxDb = NULL,
                                            bool bNamesOnly = true) const;
  virtual OdExtDbModule* getExtDbModule(OdRxObject* pRxDb) const;
  virtual OdExtDbModule* getExtDbModuleByExtension(const OdString& csExt) const;

  virtual const QStringList& getAcadRegProfileGeneralKeys() const;
  virtual QString& substituteEnvVariables(QString& qsPath) const;

  virtual QString getInitialDocumentsPath() const;

  virtual void saveWidgetPlacement(QWidget* pWidget, const QString cqsKey) const;
  virtual void restoreWidgetPlacement(QWidget* pWidget, const QString cqsKey,
                                      int widthDefault = 600, int heightDefault = 200) const;

  virtual QString getLoadedCuiCustomGroupNameByFileName(const QString& qsCuiFileName) const;

  // is needed to add path for findFile set
  virtual bool importFromFile(OdExtDbModule* pExtDb, OdEdCommandContext* pCmdCtx,
                              const OdString& sFileName, ODCOLORREF& clrBackground);

  // ----- OdqApplication -----

  class OdqCustomization* getCustom() const;

  static QMainWindow* getMainWnd();

  static QString getLinuxInfo();
#if defined(Q_WS_MAC) || defined(Q_OS_MAC) || defined(__APPLE__)
  bool event(QEvent *event);
  QString getDoubleClickedFileName() const;
#endif

private:
  void saveTextToFile(const QString& qsText, const QString& qsFilePathName) const;

  mutable std::map<OdString, OdString> m_mapExtDbModuleNameByExtension;
  QMultiHash<QString, QPointer<QObject> > m_mapRegObjects;
  mutable OdSharedPtr<OdqCustomization> m_pCustom;
#if defined(Q_WS_MAC) || defined(Q_OS_MAC) || defined(__APPLE__)
  QString doubleClickedFileName;
#endif //Q_OS_MAC
  friend class OdqMainWindow;

signals:
  void refreshSignal(const QString&, const QString&);

private slots:
  void hideMeter();
};

#if defined(Q_WS_MAC) || defined(Q_OS_MAC) || defined(__APPLE__)

  // used to prevent exceptions from application to dylib on Mac OS X
  // (such exception crashes the application)
  #define FIX_MAC_APP_TROW(err) \
    do { \
      IqFixPlatform* iFix = qobject_cast<IqFixPlatform*>(getIApp()->getSingle(IqFixPlatform_iid)); \
      ODA_ASSERT_ONCE(iFix); \
      if (iFix) \
        iFix->fix_mac_throw(err); \
    } while (false)

#else

  #define FIX_MAC_APP_TROW(err) throw err

#endif

#endif // ODA_QT_APPLICATION_H_
