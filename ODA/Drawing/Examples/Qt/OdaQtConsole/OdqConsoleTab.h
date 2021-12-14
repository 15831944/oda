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
// OdqConsoleTab.h
//

#ifndef ODA_QT_CONSOLE_TAB_H_
#define ODA_QT_CONSOLE_TAB_H_

#include <QObject>
QT_BEGIN_NAMESPACE
  class QInputEvent;
  class QLabel;
  class QTextEdit;
  class QScrollBar;
QT_END_NAMESPACE

#include "OdaCommon.h"
#include "RxVariant.h"

#include "SharedPtr.h"

#include "DbBaseDatabase.h"
#include "DbBaseHostAppServices.h"

#include "DbCommandContext.h"
#include "Ed/EdCommandStack.h"

// ExServices :
//#include "ExStringIO.h"

//#include "OdqGripManager.h"
//#include "ExSnapPointsSet.h"

#include "OdqInterfaces.h"
#include "OdqCommandLineEdit.h"
#include "CommandContextImpl.h"


enum OdTouchEvent {
  TouchUnknownEvent        = 0x01,
  TouchZoomEvent           = 0x02,
  TouchPanEvent            = 0x04,
  Touch3dOrbitEvent        = 0x08
};
//////////////////////////////////////////////////////////////////////////

class OdqConsoleTab : public QWidget
{
  Q_OBJECT
public:
  OdqConsoleTab(QWidget* pParent, 
                OdRxObject* pRxDatabase,
                const TCmdMap& mapCmds);

  virtual ~OdqConsoleTab();

  OdVariant executeCommand(const QString& qsCmd,
                           bool bEcho = true, // less priority than echo... commands
                           bool bSubCommand = false);
  OdVariant executeCommandScript(const QString& qsScriptFileName, // [.scr]
                                 bool* pbWasFound = NULL);

  OdVariant executeCommand(const OdString& sCmd,
                           bool bEcho = true, // less priority than echo... commands
                           bool bSubCommand = false);
  OdVariant executeCommandScript(const OdString& sScriptFileName, // [.scr]
                                 bool* pbWasFound = NULL);

  // -----

  static class OdqConsole& getConsole();

  void setArbitraryData(const OdString &sPathName, OdRxObject *pDataObj);
  OdRxObjectPtr arbitraryData(const OdString &sPathName) const;

  OdSelectionSet* workingSelectionSet();
  void setWorkingSelectionSet(OdSelectionSet* pRxSSet);
  void workingSelectionSetChanged();
  void clearSelectionSet();

  //OdqGripManagerPtr gripPointsSet();
  //OdqSnapManagerPtr snapPointsSet(); // ExSnapPointsSetPtr 

  void putMsgText(const QString &qsText, TMsgType type = enMsgTextNormal);
  void putMsgText(const OdString &sText, TMsgType type = enMsgTextNormal);
  OdString& traceBuf();

  void putResultAsText(const OdVariant &vResult);

  OdString getCommandPrompt();
  QString getCommandQPrompt();
  QString getQPromptWithLinks();

  OdString getPromptedString(int options, OdEdStringTracker* pTracker = NULL);
  OdGePoint3d getPromptedPoint(int options); // throw OdEdOtherInput by string

  OdEdBaseIOPtr getCmdIO();

  OdRxObjectPtr getRxDatabase();

  bool isDatabaseModified() const;
  void setDatabaseModified(bool bValue);

  OdString getFilePathName(); // associated name
  void setFilePathName(const OdString& sFilePathName); // is used for command new and qnew

  QString getQTitle();
  QString updateQTitle();

  static CommandContextImpl &getCmdContext(OdEdCommandContext *pCmdCtx);
  CommandContextImpl &getCmdContext(); 

  CommandState &getTopCmdState();

  OdGePoint3d lastGetPoint();

  OdqCommandLineEdit& getCommandLineEdit();

  Q_DECLARE_FLAGS(OdEdBaseIoStateKey, OdEdBaseIO::MouseEventFlags)
  OdEdBaseIoStateKey getKeyState();

  bool awaitForInput();

public slots:
  void slotMouseMove(IqView* iView, QEvent* pEvent, bool& bAccepted);
  void slotMouseButtonPress(IqView* iView, QEvent* pEvent, bool& bAccepted);
  void slotMouseButtonRelease(IqView* iView, QEvent* pEvent, bool& bAccepted);
  void slotMouseDoubleClick(IqView* iView, QEvent* pEvent, bool& bAccepted);
  void slotMouseWheel(IqView* iView, QEvent* pEvent, bool& bAccepted);
  void slotKeyPress(IqView* iView, QEvent* pEvent, bool& bAccepted);
  void slotKeyRelease(IqView* iView, QEvent* pEvent, bool& bAccepted);
  void slotTouchPressed(IqView* iView, QEvent* pEvent, bool& bAccepted);
  void slotTouchMove(IqView* iView, QEvent* pEvent, bool& bAccepted);
  void slotTouchReleased(IqView* iView, QEvent* pEvent, bool& bAccepted);
private slots:
  void executeCommandByInput();
  void newCommand();
  void openCommand();
  void cancelCommand();
  void eraseCommand();
  void undoCommand();
  void redoCommand();
  void selectAllCommand();
  void keywordByParam(const QString &qsParam); // ...ByMenu or ...ByLabelLink
  void resetTimeout();
  void timeTextBlink();

public:
  static void snapPoint(CommandState& stateCmd, IqView* iView,
                        OdGePoint3d& pt, // in / out
                        enum QEvent::Type typeEvent = QEvent::None);
  bool getPoint(IqView* iView, const QPoint& ptEyes, // in
                OdGePoint3d& pt, // out
                enum QEvent::Type typeEvent = QEvent::None); // in
protected:

  // Returns a SmartPointer to the arbitrary data dictionary 
  // (A dictionary is created if a dictionary does not exist)
  OdRxDictionaryPtr arbDataDic() const;

  bool checkToCancelCommandWithUndo(const OdString &sCmd);

  void updateKeyboardModifiers(QInputEvent* pEvent);

  // used as common for getPromptedString and getPromptedPoint
  OdGePoint3d getPromptedData(); // throw OdEdOtherInput by string

  OdSharedPtr<QTextEdit> m_pOut;
  OdSharedPtr<QScrollBar> m_pOutHorScrollBar;

  OdSharedPtr<QLabel> m_pLabel;
  OdSharedPtr<OdqCommandLineEdit> m_pIn;

  OdEdBaseIOPtr m_pCmdIO;
  OdEdBaseIoStateKey m_stateKey; // is used for for OdEdBaseIO::getKeyState()
  OdTouchEvent m_touchEvent;

  //CommandContextImplPtr m_pCmdContext;
  //ExDbCommandContextPtr m_pCmdContext;
  // fix  ambiguous conversions from 'OdRxObject *' via get() * ->
  OdDbCommandContextPtr m_pCmdContext;

  // CExCommandContext::m_pDb is not smart pointer
  // therefore the next is used as database holder
  OdRxObjectPtr m_pRxDatabase;
  OdString m_sFilePathName; // override for command new and qnew

  // to get modification state
  // TODO 
  //OdDbDatabaseReactorPtr m_pBaseDbReactor; // bool m_bDatabaseModified;
  //OdDbBaseDatabaseReactorPtr m_pBaseDbReactor;

  // instead ExDbCommandContext::m_pArbDataDic
  // holds common SelectionSet and so on
  mutable OdRxDictionaryPtr m_pArbDataDic;

  // used to remove internal calls of blockUndoRecording
  bool m_doubleTapTimer;
  int m_levelUndoBlock; 
  friend class OdqCommandReactor; 
  friend class OdqCmd_Undo;
  friend class OdqCmd_Redo;

  bool prepareCmdToExecute(const QPoint& ptEyes, IqView* iView, CommandState& stateCmd, OdGePoint3d& pt, bool &bAccepted);
  void setCmdOption(CommandState& stateCmd, const QPoint& ptEyes, OdGePoint3d& pt);

  friend class SaveViewStringTracker;
  bool m_bBlinkTextState; // switching via slot timeTextBlink()

  mutable OdString m_sTraceBuf;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(OdqConsoleTab::OdEdBaseIoStateKey)

#endif // ODA_QT_CONSOLE_TAB_H_
