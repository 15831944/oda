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
// CommandContextImpl.h
//

#ifndef COMMAND_CONTEXT_IMPL_H_
#define COMMAND_CONTEXT_IMPL_H_

#include <QObject>
#include <QLinkedList>

#include "OdaCommon.h"
#include "RxVariant.h"

#include "OdErrorContext.h"

#include "Ed/EdCommandContext.h" // #include "DbCommandContext.h"
#include "Ed/EdCommandStack.h"

// ExServices :
#include "ExStringIO.h"
#include "ExDbCommandContext.h"

#include "OdqGripManager.h"
#include "OdqSnapManager.h"

//////////////////////////////////////////////////////////////////////////

#define MARKER_TO_CANCEL "*Cancel*"
#define MARKER_TO_SKIP_TRANSLATION "##"

//////////////////////////////////////////////////////////////////////////

class OdErrorExecuteCommandAfterCancelContext : public OdErrorContext
{
  OdString m_sCmdExecuteAfterCancel;
  bool m_bEchoExecuteAfterCancel;
  friend class ExCancelAndExecute;
public:

  ODRX_DECLARE_MEMBERS(OdErrorExecuteCommandAfterCancelContext);
  OdSmartPtr<OdErrorExecuteCommandAfterCancelContext> 
    init(const OdString &sCmdExecuteAfterCancel,
         bool bEchoExecuteAfterCancel);

  virtual OdResult code() const;
  virtual OdString description() const;

  OdString getCommand() const
  {
    return description();
  }

  bool getEcho() const
  {
    return m_bEchoExecuteAfterCancel;
  }
};

inline bool isErrorExecuteAfterCancel(const OdErrorContext* pErrContext)
{
  bool bRes = (   pErrContext 
               && pErrContext->isKindOf(OdErrorExecuteCommandAfterCancelContext::desc()));
  return bRes;
}

//////////////////////////////////////////////////////////////////////////

class ExCancelAndExecute : public OdException
{
public:
  ExCancelAndExecute(const OdString &sCmdAfterCancel,
    bool bEchoAfterCancel)
    : OdException(OdRxObjectImpl<OdErrorExecuteCommandAfterCancelContext>::createObject()
        ->init(sCmdAfterCancel, bEchoAfterCancel))
  {
  }
  ExCancelAndExecute(OdErrorContext* pErrorContext)
    : OdException(pErrorContext)
  {
  }

  virtual ~ExCancelAndExecute()
  {
  }

  //virtual void proceedThrow() // used to throw exception via pointer
  //{
  //  OdSharedPtr<ExCancelAndExecute> err(this); // to destroy this after throw
  //  throw *this;
  //}

  OdString getCommand() const
  {
    if (!context())
      return OdString::kEmpty;
    return description();
  }

  bool getEcho() const
  {
    ODA_ASSERT_ONCE(context());
    return ((OdErrorExecuteCommandAfterCancelContext *) context())->m_bEchoExecuteAfterCancel;
  }
};

//////////////////////////////////////////////////////////////////////////

//typedef class QMap<QString, // command global name (low case)
//                   OdSharedPtr<class CCommandRec> > TCommandMap;

typedef OdSmartPtr<OdEdBaseIO> OdEdBaseIOPtr;

class ConsoleIoImpl : public OdRxObjectImpl<OdEdBaseIO> // class OdEdBaseIO is : public OdRxObject    
{
  class OdqConsoleTab& m_tabConsole;

  ConsoleIoImpl(OdqConsoleTab& tabConsole);
public:

  virtual ~ConsoleIoImpl();

  // ----- OdEdBaseIO -----

  // prmpt (I) Prompt.
  // options (I) Input options.
  // pTracker (I) Pointer to a string tracker.
  virtual OdString getString(const OdString &prompt, 
                             int options, 
                             OdEdStringTracker *pTracker);
  virtual void putString(const OdString &string);

  virtual OdGePoint3d getPoint(const OdString &prompt, 
                               int options, 
                               OdEdPointTracker *pTracker);
  virtual OdUInt32 getKeyState();

  // Creates an instance of and returns SmartPointer to it
  static OdSmartPtr<OdEdBaseIO> createObject(OdqConsoleTab &tabConsole);
};

//////////////////////////////////////////////////////////////////////////

class CommandState
{
  friend class ConsoleIoImpl;
  friend class CommandContextImpl;
  friend class SaveViewParams;
  friend class OdqCmd_Stretch;
  friend class OdqCommandReactor;
  friend class OdqCommandLastReactor;
  friend class OdqConsoleTab;
  friend class OdqConsole;

  bool m_bInUse;
  OdInt32 m_flagsCmd; // set via commandWillStart

  QString m_qsLastInput;
  QString m_qsLastCommandName;

  int m_levelCmd; // instead m_nCmdActive;

  QString m_qsCommandPrompt;
  QString m_qsPromptAndKeywords; // as "prompt: keywords"
  QString m_qsCommandEcho;

  OdSmartPtr<ExStringIO> m_pStringIO;

  int m_optionsRequestToGet;

  bool m_bRequestToGetString,
       m_bGetStringReady;
  OdEdStringTracker* m_pStringTracker;
  QString m_qsGetString;

  bool m_bRequestToGetPoint,
       m_bGetPointReady;
  OdGePoint3d m_ptGetPoint;
  QPoint m_ptGetEyes;

  // TODO possible better create 
  // separate OdqGripManagerPtr m_pGripManager in CommandContextImpl
  OdEdPointTrackerPtr m_pPointTracker; // OdqGripManager, trackers via createRectFrame, createRubberBand ...
  OdqSnapManagerPtr m_pSnapManager; 

  OdSmartPtr<OdErrorContext> m_pErrCtx;

  OdVariant m_vValue; // is used to calculate and return boolean value and etc

public:
  CommandState();
  CommandState(const CommandState &from);
  CommandState &operator=(const CommandState &from);

  virtual ~CommandState();

  bool isAllocated() const;
  bool isEmpty() const;
  const OdVariant &getValue() const;
  void setValue(const OdVariant &vValue);

  bool isDefaultPrompt() const;  // is "Command"
  OdString getPrompt() const;
  const QString& getQPrompt() const;
  void setPrompt(const OdString& sPrompt);
  void setPrompt(const QString& qsPrompt);
  void setKeywordsAtPrompt(const OdString& sPrompt, const OdString& sKeywords);

  OdEdPointTrackerPtr pointTracker() const;
  void setPointTracker(OdEdPointTracker* pPointTracker);

  OdqSnapManagerPtr snapManager() const; 
  void setSnapManager(OdqSnapManager* pSnapManager); 

  const OdString& getInputString() const;
  void setInputString(const OdString& sVal);

  void putCmdStringEcho(OdEdCommandContext* pCmdCtx, OdEdCommand* pCmd = NULL);

  static OdString getDefaultPrompt();  // "Command"
  static QString getDefaultQPrompt();
};

//////////////////////////////////////////////////////////////////////////

typedef enum {
    enEchoByMessage = 1
  , enEchoByInput = 2
  , enEchoByMessageAndInput = (enEchoByMessage | enEchoByInput)
  , enEchoByExecution = 4
  , enEchoByMessageAndExecution = (enEchoByMessage | enEchoByExecution) 
  , enEchoFull = (enEchoByMessage | enEchoByInput | enEchoByExecution)
} TEchoType;

typedef enum {
    enMsgTextNormal = 0 // simple output text
  , enMsgTextPrompted = 1
  , enMsgTextByInput = 2 // full duplicate of input string
  , enMsgTextPromptedByInput = (enMsgTextPrompted | enMsgTextByInput)
} TMsgType;

typedef OdSmartPtr<OdDbCommandContext> OdDbCommandContextPtr;
//typedef OdSmartPtr<ExDbCommandContext> ExDbCommandContextPtr;

class CommandContextImpl : public ExDbCommandContext
{
  friend class OdqConsoleTab;

  ODRX_USING_HEAP_OPERATORS(ExDbCommandContext);
  //ODRX_USING_HEAP_OPERATORS(OdDbCommandContext);

  QLinkedList<CommandState> m_stackCommandStates;

  int m_levelCmdToOffEcho; // controlled by commands echo.on, echo.on etc
  TEchoType m_typeEcho;

  OdRxObject* m_pRxDb; // TODO change type of m_pDb in base class

  static QString m_qsScriptFileNameToRecord;
  static QString m_qsScriptDataToRecord;

  static OdRxObject* m_pRxDatabaseDragSource;

protected:
  CommandContextImpl();
  ~CommandContextImpl();

  // ----- ExDbCommandContext -----
protected:
  //virtual void merge(OdSelectionSet* pRes,
  //                   int options,
  //                   const OdSelectionSet* pSSet,
  //                   OdSSetTracker* pTracker,
  //                   OdEdUserIO* pIO = NULL) const;
  virtual bool isPickadd() const ODRX_OVERRIDE;
  //virtual void highlight(OdSelectionSet* pSSet, 
  //                       bool bDoIt = true) const;
  virtual void highlight(OdSelectionSetIterator* pIter, 
                         bool bDoIt = true) const ODRX_OVERRIDE;
  TD_USING(ExDbCommandContext::highlight); 
    // ----- 
public:
  static OdDbCommandContextPtr createObject(OdEdBaseIO* pIOStream, OdRxObject* pRxDb);

  // ----- OdEdCommandContext -----

  virtual void setArbitraryData(const OdString& sPathName, OdRxObject* pDataObj) ODRX_OVERRIDE;
  virtual OdRxObjectPtr arbitraryData(const OdString& sPathName) const ODRX_OVERRIDE;

  virtual OdRxObject* baseDatabase() ODRX_OVERRIDE;

  // ----- OdDbCommandContext -----

  // ACAD acdbHostApplicationServices()->workingDatabase()
  //OdDbDatabase* database(); // is not virtual now

  //OdDbUserIO* dbUserIO();

  // ----- ExCommandContext -----

  //virtual OdString getSubCommandString(bool bSubCmdOpenBracketWasGotten = false);
  //virtual OdVariant executeSubCommand(const OdString &sSubCmd);

  //virtual const OdVariant &getValue() const;
  //virtual void setValue(const OdVariant &vValue);

  //virtual void setValue(bool bValue);

  // -----

  // used for formatting exception
  //virtual void setException(const OdSharedPtr<OdException> &wrpException);

  // after QObject::tr
  //virtual OdString tr(const OdString &sText,
  //                        // true => result with prefix ## (to prevent double translation)
  //                    bool bMarkResultAsNonTraslated = false);

  // ----- OdEdUserIO -----

  // Returns the *index* of the entered keyword in the *keyword* list, 
  // or -1 it's not there.

  virtual int getKeyword(const OdString& sPrompt,
                         const OdString& sKeywords,
                         int defVal = -1,
                         int options = OdEd::kInpDefault,
                         OdEdIntegerTracker* pTracker = NULL) ODRX_OVERRIDE;

  virtual OdString getString(const OdString& sPrompt, 
                             int options = OdEd::kInpDefault, 
                             const OdString& pDefVal = OdString::kEmpty,
                             const OdString& sKeywords = OdString::kEmpty,
                             OdEdStringTracker* pTracker = NULL) ODRX_OVERRIDE;

  virtual int getInt(const OdString& sPrompt,
                     int options = OdEd::kInpDefault,
                     int defVal = 0,
                     const OdString& sKeywords = OdString::kEmpty,
                     OdEdIntegerTracker* pTracker = NULL) ODRX_OVERRIDE;

  virtual double getReal(const OdString& sPrompt, 
                         int options = OdEd::kInpDefault,
                         double defVal = 0.0,
                         const OdString& sKeywords = OdString::kEmpty,
                         OdEdRealTracker* pTracker = NULL) ODRX_OVERRIDE;

  virtual OdSelectionSetPtr box(const OdGePoint3d& firstCorner, 
                                OdDbVisualSelection::SubentSelectionMode ssm) ODRX_OVERRIDE;

  virtual bool try_lisp(const OdString& input) ODRX_OVERRIDE;

  // ACAD acutPrintf
  //virtual void putString(const OdString &sString);

  //  Returns user input of a filename obtained in some manner.

  virtual OdString getFilePath(const OdString& sPrompt, 
                               int options = OdEd::kGfpForOpen, // or OdEd::kGfpForSave
                               const OdString& sDialogCaption = OdString::kEmpty, 
                               const OdString& sDefExt = OdString::kEmpty,
                               const OdString& sFileName = OdString::kEmpty,
                               const OdString& sFilter = OdString::kEmpty,
                               const OdString& sKeywords = OdString::kEmpty,
                               OdEdStringTracker* pTracker = NULL) ODRX_OVERRIDE;

  // ----- OdDbUserIO -----

  //virtual OdDbUnitsFormatter& formatter();
  //virtual OdGePoint3d getLASTPOINT() const;
  //virtual void setLASTPOINT(const OdGePoint3d &pt);
  //virtual void setPickfirst(OdSelectionSet *pSSet);
  //virtual OdSelectionSetPtr pickfirst();
  //virtual OdEdPointDefTrackerPtr createRubberBand(const OdGePoint3d& base) const;
  //virtual OdEdPointDefTrackerPtr createRectFrame(const OdGePoint3d& base) const;

  // ACAD acedGetVar("VIEWDIR", resbuf *rb);   

  // ACAD acedGetPoint
  virtual OdGePoint3d getPoint(const OdString& sPrompt,
                               int options = OdEd::kGptDefault,
                               const OdGePoint3d* pDefVal = NULL,
                               const OdString& sKeywords = OdString::kEmpty,
                               OdEdPointTracker* pTracker = NULL) ODRX_OVERRIDE;

  // All angles are expressed in radians.
  virtual double getAngle(const OdString& sPrompt,
                          int options = OdEd::kInpDefault,
                          double defaultValue = 0.0,
                          const OdString& sKeywords = OdString::kEmpty,
                          OdEdRealTracker* pTracker = NULL) ODRX_OVERRIDE;

  virtual double getDist(const OdString& sPrompt,
                         int options = OdEd::kInpDefault, // or OdEd::kGdsFromLastPoint
                         double defVal = 0.0,
                         const OdString& sKeywords = OdString::kEmpty,
                         OdEdRealTracker* pTracker = NULL) ODRX_OVERRIDE;

  virtual OdSharedPtr<OdCmColorBase> getCmColor(const OdString& sPrompt,
                                                int options = OdEd::kInpDefault,
                                                const OdCmColorBase* pDefaultValue = NULL,
                                                const OdString& sKeywords = OdString::kEmpty,
                                                OdEdColorTracker* pTracker = NULL) ODRX_OVERRIDE;

  // Launches interactive object selection to fill selection set.
  // Each subsequent call to this function modifies current selection set.
  // Returned value is guaranteed to be not null.
  virtual OdSelectionSetPtr select(const OdString& sPrompt = OdString::kEmpty,
                                   int options = OdEd::kSelDefault,
                                   const OdSelectionSet* pDefaultValue = NULL,
                                   const OdString& sKeywords = OdString::kEmpty,
                                   OdSSetTracker* pTracker = NULL,
                                   OdGePoint3dArray* ptsPointer = NULL) ODRX_OVERRIDE;

  // ----- OdEdFunctionIO -----

  // implements non-geometric interactive user IO functionality 
  // for custom interactive command objects.
  // (implements via arbitraryData)

  // setParam, param
  // setResult, result

  // ----- OdDbUnitsFormatter -----

  // formatLinear, unformatLinear
  // formatAngle, unformatAngle
  // toUserAngle, fromUserAngle
  // formatPoint, unformatPoint
  //virtual OdGePoint3d toUCS(const OdGePoint3d& wcsPt) const;
  //virtual OdGePoint3d fromUCS(const OdGePoint3d& ucsPt) const;

  // ----- CommandContextImpl -----

  static OdRxObject* baseDatabaseDragSource();

  CommandState& getTopCmdState();
  const CommandState& getTopCmdState() const;

  bool isEchoOn(TMsgType typeMsg = enMsgTextNormal);

  TEchoType getEchoType() const;
  void setEchoType(TEchoType typeEcho);

  int getCmdLevelToOffEcho() const;
  void setCmdLevelToOffEcho(int level);

  bool hasCmdWithUndo(bool bIgnoreMarkedToCancel = false) const;
  // return false if there is nothing to cancel
  bool markToCancelAndExecute(const OdString &sCmdAfterCancel,
                              bool bEchoAfterCancel);
  // return true if it contains mark to execute with close command
  bool isMarkedToCloseWithDeferredCommand();
  
  void accumulateScriptData(const QString &qsText, TMsgType type); // check for script accumulation

  OdqGripManagerPtr gripManager() const;
  void setGripManager(OdqGripManager* pGripManager);

  OdqSnapManagerPtr snapManager() const;

  OdSelectionSetPtr createSelectionSet();
  OdSelectionSetPtr select(int nPoints,
                           const OdGePoint3d* wcsPts,
                           OdDbVisualSelection::Mode mode = OdDbVisualSelection::kCrossing,
                           OdDbVisualSelection::SubentSelectionMode sm = OdDbVisualSelection::kDisableSubents,
                           const OdRxObject* pFilter = NULL);
};
//typedef OdSmartPtr<CommandContextImpl> CommandContextImplPtr;

//class ExExStringIO : public ExStringIO { public: const OdString& initString() const { return m_sInput; } };
//inline const OdString& ExStringIO_initString(ExStringIO* pExStringIO)
//{
//  //struct Tmp : public OdEdBaseIO { OdString m_sInput; };
//  //return reinterpret_cast<Tmp*>(pExStringIO)->m_sInput;
//  return reinterpret_cast<ExExStringIO*>(pExStringIO)->initString();
//}

#endif // COMMAND_CONTEXT_IMPL_H_
