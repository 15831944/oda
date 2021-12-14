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
// OdqInterfaces.h
//

#ifndef OD_QT_INTERFACES_H_
#define OD_QT_INTERFACES_H_

#include <QObject>
#if QT_VERSION < 0x050000
#include <QApplication>
#else
#include <QtWidgets>
#endif

#include <QPointer>
#include <QList>
#include <QHash>

QT_BEGIN_NAMESPACE
  class QMainWindow;
  class QModelIndex;
  class QStringList;
QT_END_NAMESPACE

#include "OdaCommon.h"
#include "DbBaseDatabase.h"
#include "RxVariant.h"
#include "ResBuf.h"
#include "Gs/GsBaseVectorizeDevice.h" // for ODGSPALETTE only

class OdString;
class OdRxObject;
class OdDbBaseHostAppServices;
class OdGePoint2d;
class OdGePoint3d;
class OdGePlane;
class OdGsDevice;
class OdGsView;
class OdGsModel;
//#include "OdArrayPreDef.h" // template <class T, class A = OdObjectsAllocator<T> > class OdArray;

typedef QHash<QString, // low case
              QString> TCmdMap; // group names
const QChar cqchCmdGroupDelim = '|';

//////////////////////////////////////////////////////////////////////////

class IqApplication
{
public:
  virtual QString getAppName() const = 0;
  virtual QMainWindow* getMainWindow() const = 0;
  virtual OdDbBaseHostAppServices* getBaseHostAppServices() const = 0;

  virtual bool registerQObject(const QString& qsKeys, 
                               QObject* pObj) = 0;
  virtual bool unregisterQObject(const QString& qsKeys, 
                                 QObject* pObj = NULL) = 0; // all
  virtual QList<QPointer<QObject> > getQObjects(const QString& qsKey) const = 0;
  virtual QObject* getSingle(const QString& qsKey) const = 0;

  virtual void refreshQObjetcts(const QString& qsKey,
                                const QString& qsArg = "",
                                const char* pcszMember = SLOT(refresh(const QString&, const QString&))) = 0;

  virtual void toStatusBar(const QString& qsText = "", int timeout = 0) = 0;
  virtual void setMeterLimit(unsigned int max = 100) = 0; // specify the maximum number of times to increment
  virtual void startMeter(const QString& qsText = "") = 0; // show progress bar (zero level)
  virtual void stopMeter(int timeout = 0) = 0; // hide progress meter
  virtual void stepMeter() = 0; // increment the progress meter

  virtual OdString toOdString(const QString& qsStr) const = 0;
  virtual QString toQString(const OdString& sStr) const = 0;

  virtual QString encodeToXmlAttrib(const QString& qsStr) const = 0;
  virtual QString decodeFromXmlAttrib(const QString& qsStr) const = 0;

  virtual QString getExtensionByFilter(const QString& qsSelectedFilter,
                                       const QString& qsDelim = "") const = 0; // return only 1st if empty

  virtual OdStringArray getExtDbModuleNames(OdRxObject* pRxDb = NULL,
                                            bool bNamesOnly = true) const = 0;
  virtual class OdExtDbModule* getExtDbModule(OdRxObject* pRxDb) const = 0;
  virtual OdExtDbModule* getExtDbModuleByExtension(const OdString& csExt) const = 0;

  virtual const QStringList& getAcadRegProfileGeneralKeys() const = 0;
  virtual QString& substituteEnvVariables(QString& qsPath) const = 0;
  virtual QString getInitialDocumentsPath() const = 0;

  virtual void saveWidgetPlacement(QWidget* pWidget, const QString cqsKey) const = 0;
  virtual void restoreWidgetPlacement(QWidget* pWidget, const QString cqsKey,
                                      int widthDefault = 600, int heightDefault = 200) const = 0;

  virtual QString getLoadedCuiCustomGroupNameByFileName(const QString& qsCuiFileName) const = 0;

  // is needed to add path for findFile set
  virtual bool importFromFile(OdExtDbModule* pExtDb, class OdEdCommandContext* pCmdCtx,
                              const OdString& sFileName, ODCOLORREF& clrBackground) = 0;
};
Q_DECLARE_INTERFACE(IqApplication, "oda.IqApplication")

class IqAppProps
{
public:
  typedef enum  {
    prpNone = OdResBuf::kRtNone
  , prpString = OdResBuf::kRtString
  , prpCombo = OdResBuf::kRtString + 1000 // string is edited via combo only
  , prpFolder = OdResBuf::kRtString + 1001
  , prpInt = OdResBuf::kRtInt32
  , prpBool = OdResBuf::kRtBool
  , prpBoolPictured = OdResBuf::kRtBool + 1000
  , prpBoolNamed = OdResBuf::kRtBool + 1001 // button
  , prpRadio = OdResBuf::kRtBool + 1002
  , prpDouble = OdResBuf::kRtDouble
  , prpDistance = OdResBuf::kDxfXdDist
  , prpAngle = OdResBuf::kDxfAngle // or OdResBuf::kRtAngle
  , prpOdColor = OdResBuf::kRtColor
  , prpColorref = OdResBuf::kRtColor + 1000
  , prpLinetype = OdResBuf::kRtColor + 1001
  , prpLineweight = OdResBuf::kRtColor + 1002
  , prpTransparency = OdResBuf::kRtColor + 1003
  , prpGrid = 1200
  , prpTree
  , prpPalette // special tree view in prop palette
  , prpFileDialog
  , prpImage
  , prpCategory
  , prpReference
  } TPropType;

  virtual QList<OdString> subpaths(const OdString& sParentPath, 
                                   bool bSkipHidden = true) = 0; 

  virtual TPropType type(const OdString& sPropPath) = 0;
  virtual bool isHidden(const OdString& sPropPath) = 0;
  virtual bool set(const OdString& sPropPath, const OdVariant& vValue,
                   const OdString& sTypeToCreate = OdString::kEmpty) = 0; // only override is enabled

  virtual OdString toString(const OdString& sPropPath, 
                            const OdString& sDefValue = OdString::kEmpty) = 0;
  virtual bool toBool(const OdString& sPropPath, 
                      bool bDefValue = false) = 0;
  virtual int toInt(const OdString & sPropPath, 
                    int iDefValue = 0) = 0;
  virtual double toDouble(const OdString& sPropPath, 
                           double dDefValue = 0.0) = 0;
  virtual ODCOLORREF toColorref(const OdString& sPropPath, 
                                ODCOLORREF clrDefValue = ODRGB(0, 0, 0)) = 0;
};
#define IqAppProps_iid "oda.IqAppProps"
Q_DECLARE_INTERFACE(IqAppProps, IqAppProps_iid)

class IqPropSystem
{
public:
  virtual bool isPropVisible() const = 0;
  virtual ODGSPALETTE& cacheColors(OdRxObject* pRxDatabase) const = 0; // for color palette via ExtDb
};
#define IqPropSystem_iid "oda.IqPropSystem"
Q_DECLARE_INTERFACE(IqPropSystem, IqPropSystem_iid)

class IqRevSystem
{
public:
  virtual class IqRevDiff* getIRevDiff(class IqView* iView) = 0;
  virtual bool isRevVisible() const = 0;
  virtual bool isUnder(OdRxObject* pRxDb) const = 0;
};
#define IqRevSystem_iid "oda.IqRevSystem"
Q_DECLARE_INTERFACE(IqRevSystem, IqRevSystem_iid)

class IqRevDiff
{
public:
  virtual OdSmartPtr<OdGsDevice> setupLayoutDiff(OdRxObject* pRxDb, OdGsDevice* pDevice, OdGiDefaultContext* pGiCtx) = 0;
  virtual void syncView(OdRxObject* pRxDb, OdGsDevice* pDevice) = 0;
  virtual void closeDiffView(OdRxObject* pRxDb) = 0;
  virtual void setVisualStyle( const OdString& ) = 0;
};
#define IqRevDiff_iid "oda.IqRevDiff"
Q_DECLARE_INTERFACE(IqRevDiff, IqRevDiff_iid)

class IqConsole
{
public:
  virtual OdVariant executeCommand(const QString& qsCmd,
                                   bool bEcho = true,
                                   OdRxObject* pRxDatabase = NULL) = 0; // current
  virtual void postponeCommand(const QString& qsCmd,
                               bool bEcho = true,
                               OdRxObject* pRxDatabase = NULL, // current (use ((OdRxObject*) -1) to avoid setCurrentDatabase at execution moment)
                               int timeStep = 0) = 0; // milliseconds (0 - via Qt::QueuedConnection)
  virtual QString findPostponedCommand(const QString &qsCmdPref, 
                                       OdRxObject* pRxDatabase = NULL) = 0; // current
  virtual OdVariant executeCommandScript(const QString& qsScriptFileName, // [.scr] // for current database
                                         bool* pbWasFound = NULL) = 0;
  virtual OdVariant executeCommandScript(const QString& qsScriptFileName, // [.scr]
                                         OdRxObject* pRxDatabase,
                                         bool* pbWasFound = NULL) = 0;
  virtual QString evaluateDieselExpression(const QString& qsExpression,
                                           OdRxObject* pRxDatabase = NULL) = 0; // current
  virtual OdString evaluateDieselExpression(const OdString& sExpression,
                                            OdRxObject* pRxDatabase = NULL) = 0; // current
  virtual void updateAllGrips(OdRxObject* pRxDatabase = NULL) = 0; // current
  virtual class OdSelectionSet* workingSelectionSet(OdRxObject* pRxDatabase = NULL) = 0; // current
  virtual void clearSelectionSet(OdRxObject* pRxDatabase = NULL) = 0; // current

  virtual void putString(const QString& qsMsg,
                         OdRxObject* pRxDatabase = NULL) = 0; // current
  virtual void putString(const OdString& sMsg,
                         OdRxObject* pRxDatabase = NULL) = 0; // current
  virtual void putTrace(const OdString& sMsg,
                        OdRxObject* pRxDatabase = NULL) = 0; // current
  virtual bool isInputStringReady() const = 0; // for current database
  virtual bool isInputStringReady(OdRxObject* pRxDatabase) const = 0;
  virtual bool isInputStringReady(class OdEdCommandContext* pCmdCtx) const = 0;
  virtual const TCmdMap& getCommandMap() const = 0;

  virtual OdRxObject* getCurrentDatabase() const = 0;
  virtual QList<OdRxObject*> getOpenDatabases() const = 0;
  virtual OdEdCommandContext* getCommandContext() const = 0; // current
  virtual OdEdCommandContext* getCommandContext(OdRxObject* pRxDatabase) const = 0;
  virtual OdRxObject* getDatabaseDragSource() const = 0;

  virtual bool setCurrentDatabase(OdRxObject* pRxDatabase) = 0;
  virtual bool closeDatabase(OdRxObject* pRxDatabase) = 0;
  virtual bool isMarkedToClose(OdRxObject* pRxDatabase) = 0;

  virtual OdString getFilePathName(OdRxObject* pRxDatabase) = 0;
  virtual QString getQTitle(OdRxObject* pRxDatabase) = 0;
  virtual bool getSlotData(QEvent::Type eventType, // in
                           OdRxObject* pRxDatabase, // in
                           QObject*& pReceiver, // out
                           const char*& pcszMember) = 0; // out

  virtual bool isConsoleVisible() const = 0;
 
  virtual OdString activeCommandName(OdRxObject* pRxDatabase = NULL) = 0; // current
  // system variable CMDACTIVE returns the sum of the following values:
  enum CmdActiveState_t {
    enCmdActive_No = 0 // No active command
    , enCmdActive_Ordinary = 1 // Ordinary command is active
    , enCmdActive_Transparent = 2 // Transparent command is active
    , enCmdActive_Script = 4 // Script is active
    , enCmdActive_DialogBox = 8 // Dialog box is active
  //, enCmdActive_DDE = 16 // DDE is active
  //, enCmdActive_AutoLISP = 32 // AutoLISP is active (only visible to an ObjectARX-defined command)
  //, enCmdActive_ObjectARX = 64 // ObjectARX command is active
  };
  Q_DECLARE_FLAGS(TCmdActiveState, CmdActiveState_t)
  virtual TCmdActiveState activeCommandState(OdRxObject* pRxDatabase = NULL) = 0; // current
  virtual bool markToCancel(OdRxObject* pRxDatabase = NULL) = 0; // current
};
#define IqConsole_iid "oda.IqConsole"
Q_DECLARE_INTERFACE(IqConsole, IqConsole_iid)

class IqView
{
public:
  virtual OdRxObject* getRxDatabase() = 0;
  virtual void updateLayouts(bool bForce = false) = 0; // force means to renew render area widget
  virtual bool emitClose() = 0;

  virtual OdGsDevice* getGsDevice() = 0;
  virtual OdGsView* getActiveGsView() = 0;
  virtual OdGsView* getActiveTopGsView() = 0;
  virtual OdGsModel* getGsModel(bool b2d = false) = 0;

  virtual QCursor cursor() = 0; 
  virtual void setCursor(const QCursor& cursor) = 0; 

  virtual bool getUcsPlane(OdGePlane& plane,
                           const OdGeVector3d* pvHintDirToIntersect = NULL) = 0;
    // convert eyes point to UCS(by empty OdEd::kGptNoUCS flag in option) or to WCS in eye plane
  virtual bool eyesTo(const QPoint& ptEyes, // in 
                      int options, // in 
                      OdGePoint3d& ptOut, // out
                      const OdGePlane* pUcsPlane = NULL) = 0; // in
  virtual bool wcsToScreen(const OdGePoint3d& wcsPt,
                           OdGePoint2d& ptScr) = 0; // out

  virtual ODCOLORREF backgroundColor() const = 0;
  virtual bool setBackgroundColor(ODCOLORREF clrBackground) = 0;
  virtual void setViewportBorderProperties() = 0;

  virtual QWidget* getWidget() = 0; // of render area
  virtual void updateGL() = 0; // repaint
  virtual void setActive() = 0;
  // for Android only
  virtual bool isFullScreenMode() = 0; 
  virtual bool isFullScreenActive() = 0; 
};
Q_DECLARE_INTERFACE(IqView, "oda.IqView")

class IqViewSystem
{
public:
  virtual OdSmartPtr<class OdGsModule> getGsModule(OdRxObject* pRxDatabase) = 0;
  virtual IqView* createViews(OdRxObject* pRxDatabase) = 0;
  virtual IqView* getView(OdRxObject* pRxDatabase, bool bDoActive = false, bool* pIsSingle = NULL) = 0;
  virtual QList<IqView*> getOpenViews(OdRxObject* pRxDatabase) = 0;
  // return NULL if point out of view widget
  virtual IqView* getViewByScreenPoint(QPoint& point, // in[screen] // out[view] 
                                       bool bDoActive = false,
                                       bool bDwgOnly = true) = 0; 
  virtual bool updateViews(OdRxObject* pRxDatabase) = 0;
  virtual bool closeViews(OdRxObject* pRxDatabase) = 0;
  virtual bool closeAllViews(QList<OdRxObject*>* pLstExceptDatabases = NULL) = 0;
  virtual bool markToCloseAppByLastView() = 0; // return false if opened views are missing
  virtual void checkMarkToCloseApp() = 0;
  virtual void updateViewTitles(OdRxObject* pRxDatabase,
                                bool bForce = false) = 0;
  virtual bool setActiveViewbyTitle(const OdString& sTitle) = 0;
  virtual bool isMaximized() = 0; // state of QMdiSubWindow

  // repaint
  virtual void updateGL(OdRxObject* pRxDatabase, bool bActiveOrFirstOnly = false) = 0; 

  virtual bool highlightObject(bool bValue, // on / off
                               class OdDbStub* id, 
                               const class OdDbBaseFullSubentPath* pPath = NULL, 
                               OdRxObject* pRxDatabase = NULL) = 0; // via baseDatabaseBy(id)
  virtual bool onModifiedGrip(OdGiDrawablePtr pGripData, 
                              OdRxObject* pRxDatabase) = 0;
  virtual bool highlightGripPoint(bool bValue, OdGiDrawablePtr pGripData,
                                  OdRxObject* pRxDatabase) = 0;
};
#define IqViewSystem_iid "oda.IqViewSystem"
Q_DECLARE_INTERFACE(IqViewSystem, IqViewSystem_iid)

// used via FIX_MAC_APP_TROW to prevent exceptions from application to dylib on Mac OS X
// (such exception crashes the application)
#if defined(Q_WS_MAC) || defined(Q_OS_MAC) || defined(__APPLE__)
class IqFixPlatform
{
public:
  virtual void fix_mac_throw(const OdError& err) = 0;
};
#define IqFixPlatform_iid "oda.IqFixPlatform"
Q_DECLARE_INTERFACE(IqFixPlatform, IqFixPlatform_iid)
#endif

//////////////////////////////////////////////////////////////////////////

inline IqApplication* getIApp()
{
  IqApplication* iApp = qobject_cast<IqApplication*>(qApp);
  ODA_ASSERT_ONCE(iApp);
  return iApp;
}

inline IqAppProps* getIAppProps()
{
  IqAppProps* iAppProps = qobject_cast<IqAppProps*>(getIApp()->getSingle(IqAppProps_iid));
  ODA_ASSERT_ONCE(iAppProps);
  return iAppProps;
}

inline IqConsole* getIConsole()
{
  IqConsole* iConsole = qobject_cast<IqConsole*>(getIApp()->getSingle(IqConsole_iid));
  //ODA_ASSERT_ONCE(iConsole);
  return iConsole;
}

inline IqViewSystem* getIViewSystem()
{
  IqViewSystem* iVS = qobject_cast<IqViewSystem*>(getIApp()->getSingle(IqViewSystem_iid));
  //ODA_ASSERT_ONCE(iVS);
  return iVS;
}

inline IqPropSystem* getIPropSystem()
{
  IqPropSystem* iPS = qobject_cast<IqPropSystem*>(getIApp()->getSingle(IqPropSystem_iid));
  //ODA_ASSERT_ONCE(iPS);
  return iPS;
}

inline IqRevSystem* getIRevSystem()
{
  IqRevSystem* iRS = qobject_cast<IqRevSystem*>(getIApp()->getSingle(IqRevSystem_iid));
  //ODA_ASSERT_ONCE(iRS);
  return iRS;
}

#if defined(Q_OS_WIN) || defined(Q_WS_WIN) // Q_OS_WIN32 _MSC_VER
  #define chPathSeparator ';'
#else
  #define chPathSeparator ':'
#endif

#endif // OD_QT_INTERFACES_H_
