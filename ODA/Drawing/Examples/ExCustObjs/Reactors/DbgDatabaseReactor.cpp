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

#include "OdaCommon.h"
#include "DbDatabase.h"
#include "DbgDatabaseReactor.h"

void OdDbgDatabaseReactor::objectAppended(const OdDbDatabase* pDb, const OdDbObject* pDbObj)
{
  OdString msg;
  msg.format(OD_T("[DB 0x%08X  ] : Appended        <%-24ls %3I64X>"), pDb, pDbObj->isA()->name().c_str(), (OdUInt64)pDbObj->getDbHandle());
  printMessage(msg);
}

void OdDbgDatabaseReactor::objectUnAppended(const OdDbDatabase* pDb, const OdDbObject* pDbObj)
{
  OdString msg;
  msg.format(OD_T("[DB 0x%08X  ] : UnAppended      <%-24ls %3I64X>"), pDb, pDbObj->isA()->name().c_str(), (OdUInt64)pDbObj->getDbHandle());
  printMessage(msg);
}

void OdDbgDatabaseReactor::objectReAppended(const OdDbDatabase* pDb, const OdDbObject* pDbObj)
{
  OdString msg;
  msg.format(OD_T("[DB 0x%08X  ] : ReAppended      <%-24ls %3I64X>"), pDb, pDbObj->isA()->name().c_str(), (OdUInt64)pDbObj->getDbHandle());
  printMessage(msg);
}

void OdDbgDatabaseReactor::objectOpenedForModify(const OdDbDatabase* pDb, const OdDbObject* pDbObj)
{
  OdString msg;
  msg.format(OD_T("[DB 0x%08X  ] : OpenedForModify <%-24ls %3I64X>"), pDb, pDbObj->isA()->name().c_str(), (OdUInt64)pDbObj->getDbHandle());
  printMessage(msg);
}

void OdDbgDatabaseReactor::objectModified(const OdDbDatabase* pDb, const OdDbObject* pDbObj)
{
  OdString msg;
  msg.format(OD_T("[DB 0x%08X  ] : Modified        <%-24ls %3I64X>"), pDb, pDbObj->isA()->name().c_str(), (OdUInt64)pDbObj->getDbHandle());
  printMessage(msg);
}

void OdDbgDatabaseReactor::objectErased(const OdDbDatabase* pDb, const OdDbObject* pDbObj, bool pErased)
{
  OdString msg;
  msg.format(OD_T("[DB 0x%08X  ] : %ls        <%-24ls %3I64X>"), pDb, pErased ? OD_T("Erased  ") : OD_T("Unerased"),
    pDbObj->isA()->name().c_str(), (OdUInt64)pDbObj->getDbHandle());
  printMessage(msg);
}

void OdDbgDatabaseReactor::headerSysVarWillChange(const OdDbDatabase* pDb, const OdString& name)
{
  OdString msg;
  msg.format(OD_T("[DB 0x%08X  ] : Will Change     <%ls>"), pDb, name.c_str());
  printMessage(msg);
}

void OdDbgDatabaseReactor::headerSysVarChanged(const OdDbDatabase* pDb, const OdString& name)
{
  OdString msg;
  msg.format(OD_T("[DB 0x%08X  ] : Changed         <%ls>"), pDb, name.c_str());
  printMessage(msg);
}
