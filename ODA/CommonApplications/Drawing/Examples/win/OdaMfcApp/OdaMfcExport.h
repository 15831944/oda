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

#ifndef __ODA_MFC_EXPORT_H__
#define __ODA_MFC_EXPORT_H__

#ifdef ODAMFC_EXPORT_SYMBOL
#   define ODAMFC_EXPORT OD_TOOLKIT_EXPORT
#else
#   define ODAMFC_EXPORT OD_TOOLKIT_IMPORT
#endif

#include "DbGsManager.h"

class ODAMFC_EXPORT OdApplicationReactor : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS( OdApplicationReactor );

  // App events.
  virtual void OnBeginQuit(){}

  virtual void OnEnterModal(){}

  virtual void OnIdle( int ){}

  virtual void OnLeaveModal(){}

  virtual void OnPreTranslateMessage( MSG* ){}

  virtual void OnQuitAborted(){}

  virtual void OnQuitWillStart(){}

  // Document events.
  virtual void documentCreateStarted( CDocument* ){}

  virtual void documentCreated( CDocument* ){}

  virtual void documentToBeDestroyed( CDocument* ){}

  virtual void documentDestroyed( const OdString& ){}

  virtual void documentCreateCanceled( CDocument* ){}

  //virtual void documentLockModeWillChange(CDocument* ,
  //                        AcAp::DocLockMode myCurrentMode,
  //                        AcAp::DocLockMode myNewMode,
  //                        AcAp::DocLockMode currentMode,
  //                        const ACHAR* pGlobalCmdName);
  //virtual void documentLockModeChangeVetoed(CDocument* ,
  //                        const ACHAR* pGlobalCmdName);
  //virtual void documentLockModeChanged(CDocument* ,
  //                        AcAp::DocLockMode myPreviousMode,
  //                        AcAp::DocLockMode myCurrentMode,
  //                        AcAp::DocLockMode currentMode,
  //                        const ACHAR* pGlobalCmdName);

  virtual void documentBecameCurrent( CDocument* ){}

  virtual void documentToBeActivated( CDocument* ){}

  virtual void documentToBeDeactivated( CDocument* ){}

  virtual void documentActivationModified( bool ){}

  virtual void documentActivated( CDocument* ){}
};

typedef OdSmartPtr< OdApplicationReactor > OdApplicationReactorPtr;

ODAMFC_EXPORT void odAddAppReactor( OdApplicationReactor* pReactor );

class ODAMFC_EXPORT OdApDocument : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdApDocument);

  virtual OdString fileName() const = 0;

  virtual CDocument* cDoc() const = 0;

  //virtual AcTransactionManager* transactionManager() const = 0;

  virtual OdDbDatabasePtr database() const = 0;

  virtual void lockMode(bool bIncludeMyLocks ) const = 0;

  virtual void myLockMode() const = 0;

  virtual bool isQuiescent() const = 0;

  virtual void* contextPtr() const = 0;

  virtual OdEdBaseIO* cmdIO() = 0;

  virtual OdDbCommandContextPtr cmdCtx() = 0;

  virtual void ExecuteCommand(const OdString& sCmd, bool bEcho) = 0;

  virtual OdString recentCmd() = 0;

  virtual OdDbSelectionSetPtr selectionSet() const = 0;
};
typedef OdSmartPtr< OdApDocument > OdApDocumentPtr;

ODAMFC_EXPORT OdApDocumentPtr odGetAppDocument( CDocument* pDoc );

ODAMFC_EXPORT OdGsLayoutHelperPtr odGetDocDevice( CDocument* pDoc );

ODAMFC_EXPORT bool odGetDocOsnapPoint( CDocument* pDoc, OdGePoint3d& pt );

#endif // __ODA_MFC_EXPORT_H__
