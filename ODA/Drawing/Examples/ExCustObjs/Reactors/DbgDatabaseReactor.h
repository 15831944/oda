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


#ifndef _ODEX_DBGDATABASEREACTOR_INCLUDED_
#define _ODEX_DBGDATABASEREACTOR_INCLUDED_

#include "DbDatabaseReactor.h"
#include "../ExCustObjExport.h"

class EXCUSTOBJEXPORT OdDbgDatabaseReactor : public OdDbDatabaseReactor
{
public:
  virtual void printMessage(const OdString& msg) const = 0;

  // OdDbDatabaseReactor overridden
  //
  virtual void objectAppended(const OdDbDatabase* pDb, const OdDbObject* pDbObj);
  virtual void objectUnAppended(const OdDbDatabase* pDb, const OdDbObject* pDbObj);
  virtual void objectReAppended(const OdDbDatabase* pDb, const OdDbObject* pDbObj);
  virtual void objectOpenedForModify(const OdDbDatabase* pDb, const OdDbObject* pDbObj);
  virtual void objectModified(const OdDbDatabase* pDb, const OdDbObject* pDbObj);
  virtual void objectErased(const OdDbDatabase* pDb, const OdDbObject* pDbObj, bool pErased = true);
  virtual void headerSysVarWillChange(const OdDbDatabase* pDb, const OdString& name);
  virtual void headerSysVarChanged(const OdDbDatabase* pDb, const OdString& name);
};

#endif //_ODEX_DBGDATABASEREACTOR_INCLUDED_
