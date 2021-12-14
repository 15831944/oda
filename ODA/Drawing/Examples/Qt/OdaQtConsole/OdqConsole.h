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
// OdqConsole.h
//

#ifndef ODA_QT_CONSOLE_H_
#define ODA_QT_CONSOLE_H_

#include <QPointer>
#include <QMap>
#include <QQueue>

QT_BEGIN_NAMESPACE
  class QDockWidget;
  class QTabWidget;
QT_END_NAMESPACE

#include "OdqInterfaces.h"
#include "OdqConsoleTab.h"
#include "OdqCommandLineEdit.h"

//////////////////////////////////////////////////////////////////////////


class CmdInfo
{
public:
  CmdInfo()
  {
  }

  CmdInfo(const QString &sName, bool bEcho, OdRxObject* pRxDatabase) 
    : m_sName(sName)
    , m_bEcho(bEcho)
    , m_pRxDatabase(pRxDatabase)
  {
  }

  ~CmdInfo()
  {
  }

  const QString& getCommand() const
  {
    return m_sName;
  }

  bool getEcho() const
  {
    return m_bEcho;
  }

  OdRxObject* getRxDatabase() const
  {
    return m_pRxDatabase;
  }

  bool operator ==(const CmdInfo& with) const
  {
    bool bRes = (   m_sName == with.m_sName
      && (m_pRxDatabase == ((OdRxObject*) -1) || with.m_pRxDatabase == ((OdRxObject*) -1) || m_pRxDatabase == with.m_pRxDatabase)
      && m_bEcho == with.m_bEcho);
    return bRes;
  }

private:
  QString m_sName;
  bool m_bEcho;
  OdRxObject* m_pRxDatabase;
};

//////////////////////////////////////////////////////////////////////////

class OdqConsole : public QObject
                 , public IqConsole
{
  Q_OBJECT
  Q_INTERFACES(IqConsole)

  friend class OdqConsoleModule;
  OdqConsole(const TCmdMap& mapCmds);
  void saveSettings();

public:
  virtual ~OdqConsole();

  // ----- IqConsole -----

  virtual OdVariant executeCommand(const QString& qsCmd,
                                   bool bEcho = true,
                                   OdRxObject* pRxDatabase = NULL); // current
  virtual void postponeCommand(const QString& qsCmd,
                               bool bEcho = true,
                               OdRxObject* pRxDatabase = NULL, // current (use ((OdRxObject*) -1) to avoid setCurrentDatabase at execution moment)
                               int timeStep = 0); // milliseconds (0 - via Qt::QueuedConnection)
  virtual QString findPostponedCommand(const QString &qsCmdPref, 
                                       OdRxObject* pRxDatabase = NULL); // current
  virtual OdVariant executeCommandScript(const QString& qsScriptFileName, // [.scr] for current database
                                         bool* pbWasFound = NULL);
  virtual OdVariant executeCommandScript(const QString& qsScriptFileName, // [.scr]
                                         OdRxObject* pRxDatabase,
                                         bool* pbWasFound = NULL);
  virtual QString evaluateDieselExpression(const QString& qsExpression,
                                           OdRxObject* pRxDatabase = NULL); // current
  virtual OdString evaluateDieselExpression(const OdString& sExpression,
                                            OdRxObject* pRxDatabase = NULL); // current
  virtual void updateAllGrips(OdRxObject* pRxDatabase = NULL); // current
  virtual OdSelectionSet* workingSelectionSet(OdRxObject* pRxDatabase); // current
  virtual void clearSelectionSet(OdRxObject* pRxDatabase = NULL); // current

  virtual void putString(const QString& qsMsg,
                         OdRxObject* pRxDatabase = NULL); // current
  virtual void putString(const OdString& sMsg,
                         OdRxObject* pRxDatabase = NULL); // current
  virtual void putTrace(const OdString& sMsg,
                        OdRxObject* pRxDatabase = NULL); // current
  virtual bool isInputStringReady() const; // for current database
  virtual bool isInputStringReady(OdRxObject* pRxDatabase) const;
  virtual bool isInputStringReady(OdEdCommandContext* pCmdCtx) const;
  virtual const TCmdMap& getCommandMap() const;

  virtual OdRxObject* getCurrentDatabase() const;
  virtual QList<OdRxObject*> getOpenDatabases() const;
  virtual class OdEdCommandContext* getCommandContext() const; // current
  virtual class OdEdCommandContext* getCommandContext(OdRxObject* pRxDatabase) const;
  virtual OdRxObject* getDatabaseDragSource() const;

  virtual bool setCurrentDatabase(OdRxObject* pRxDatabase);
  virtual bool closeDatabase(OdRxObject* pRxDatabase);
  virtual bool isMarkedToClose(OdRxObject* pRxDatabase);

  virtual OdString getFilePathName(OdRxObject* pRxDatabase);
  virtual QString getQTitle(OdRxObject* pRxDatabase);
  virtual bool getSlotData(QEvent::Type eventType, // in
                           OdRxObject* pRxDatabase, // in
                           QObject*& pReceiver, // out
                           const char*& pcszMember); // out

  virtual bool isConsoleVisible() const;
  virtual OdString activeCommandName(OdRxObject* pRxDatabase = NULL); // current
  virtual TCmdActiveState activeCommandState(OdRxObject* pRxDatabase = NULL); // current
  virtual bool markToCancel(OdRxObject* pRxDatabase = NULL); // current

  // ----- OdqConsole -----

  static OdqConsole& getConsole();
  static QDockWidget* getDockPalette(); 

  static OdqConsoleTab* getConsoleTab(OdRxObject* pRxDatabase,
                                      bool bSetCurrent = false,
                                      const OdString& sFilePathNameOverride = OdString::kEmpty); // nono
  static bool removeConsoleTab(OdRxObject* pRxDatabase);
  static bool removeConsoleTab(OdqConsoleTab &tab);

private slots:
  void currentTabChanged(int index);
  void executePostponedCommands();

signals:
  void postponeCommandsAvailable();

private:
  const TCmdMap& m_mapCmds;
  QQueue<CmdInfo> m_cmdsPostponed;

  OdSharedPtr<QDockWidget> m_pDockPalette; //QPointer<QDockWidget> m_pDockPalette;

  QMap<OdRxObject*, OdSharedPtr<OdqConsoleTab> > m_mapTabs; // via rx database (base database) as key
  QPointer<QTabWidget> m_pTabWidget; // is m_pDockPalette->widget()
};

//////////////////////////////////////////////////////////////////////////
// used via FIX_MAC_APP_TROW to prevent exceptions from application to dylib on Mac OS X
// (such exception crashes the application)

#if defined(Q_WS_MAC) || defined(Q_OS_MAC) || defined(__APPLE__)
class OdqFixPlatform : public QObject
                     , public IqFixPlatform
{
  Q_OBJECT
  Q_INTERFACES(IqFixPlatform)

  friend class OdqConsoleModule;
  OdqFixPlatform();

public:
  virtual ~OdqFixPlatform();

  virtual void fix_mac_throw(const OdError& err);
};
#endif

//////////////////////////////////////////////////////////////////////////

#endif // ODA_QT_CONSOLE_H_
