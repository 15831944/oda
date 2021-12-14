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
// DbselectionUtils.h: Declarations of the selection utility functions.
//
//////////////////////////////////////////////////////////////////////

#ifndef EXTRACEDBREACTOR_INCLUDED
#define EXTRACEDBREACTOR_INCLUDED

#ifdef _DEBUG

#include "DbLayoutManager.h"
#include "DbDatabaseReactor.h"

class ExDocReactorsJoint : public OdDbLayoutManagerReactor
  , public OdDbDatabaseReactor
{
  ODRX_NO_HEAP_OPERATORS();
};

class ExTraceDbReactor : public ExDocReactorsJoint {
public:
  static void attachTo(OdDbDatabase* pDb);
  static void detach(OdDbDatabase* pDb);

  void objectAppended(const OdDbDatabase* db, const OdDbObject* obj);
  void objectUnAppended(const OdDbDatabase* db, const OdDbObject* obj);
  void objectReAppended(const OdDbDatabase* db, const OdDbObject* obj);
  void objectOpenedForModify(const OdDbDatabase* db, const OdDbObject* obj);
  void objectModified(const OdDbDatabase* db, const OdDbObject* obj);
  void objectErased(const OdDbDatabase* db, const OdDbObject* obj, bool erased);
  void headerSysVarWillChange(const OdDbDatabase* db, const OdString& sysVar);
  void headerSysVarChanged(const OdDbDatabase* db, const OdString& sysVar);
  void layoutCreated(const OdString&, const OdDbObjectId&);
  void layoutToBeRemoved(const OdString&, const OdDbObjectId&);
  void layoutRemoved(const OdString&, const OdDbObjectId&);
  void abortLayoutRemoved(const OdString&, const OdDbObjectId&);
  void layoutToBeCopied(const OdString&, const OdDbObjectId&);
  void layoutCopied(const OdString&, const OdDbObjectId&, const OdString&, const OdDbObjectId&);
  void abortLayoutCopied(const OdString&, const OdDbObjectId&);
  void layoutToBeRenamed(const OdString&, const OdString&, const OdDbObjectId&);
  void layoutRenamed(const OdString&, const OdString&, const OdDbObjectId&);
  void abortLayoutRename(const OdString&, const OdString&, const OdDbObjectId&);
  void layoutSwitched(const OdString& name, const OdDbObjectId&);
  void plotStyleTableChanged(const OdString&, const OdDbObjectId&);
};

#define TRACE_DB_EVENTS(pDb) ExTraceDbReactor::attachTo(pDb)
#define UNTRACE_DB_EVENTS(pDb) ExTraceDbReactor::detach(pDb)
#else
#define TRACE_DB_EVENTS(pDb)
#define UNTRACE_DB_EVENTS(pDb)
#endif


#endif //#ifdef _DEBUG

