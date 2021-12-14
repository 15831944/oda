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
#include "DgDatabase.h"
#include "ExDgDatabaseReactor.h"

void OdExDgDatabaseReactor::elementAppended(const OdDgDatabase* pDb, const OdDgElement* pElm)
{
  OdString msg;
  msg.format(OD_T("DB 0x%08X : elementAppended        <%-24ls %3ls>\n"), pDb, pElm->isA()->name().c_str(), pElm->elementId().getHandle().ascii().c_str());
  printMessage(msg);
}

void OdExDgDatabaseReactor::elementUnAppended(const OdDgDatabase* pDb, const OdDgElement* pElm)
{
  OdString msg;
  msg.format(OD_T("DB 0x%08X : elementUnAppended      <%-24ls %3ls>\n"), pDb, pElm->isA()->name().c_str(), pElm->elementId().getHandle().ascii().c_str());
  printMessage(msg);
}

void OdExDgDatabaseReactor::elementReAppended(const OdDgDatabase* pDb, const OdDgElement* pElm)
{
  OdString msg;
  msg.format(OD_T("DB 0x%08X : elementReAppended      <%-24ls %3ls>\n"), pDb, pElm->isA()->name().c_str(), pElm->elementId().getHandle().ascii().c_str());
  printMessage(msg);
}

void OdExDgDatabaseReactor::elementOpenedForModify(const OdDgDatabase* pDb, const OdDgElement* pElm)
{
  OdString msg;
  msg.format(OD_T("DB 0x%08X : elementOpenedForModify <%-24ls %3ls>\n"), pDb, pElm->isA()->name().c_str(), pElm->elementId().getHandle().ascii().c_str());
  printMessage(msg);
}

void OdExDgDatabaseReactor::elementModified(const OdDgDatabase* pDb, const OdDgElement* pElm)
{
  OdString msg;
  msg.format(OD_T("DB 0x%08X : elementModified        <%-24ls %3ls>\n"), pDb, pElm->isA()->name().c_str(), pElm->elementId().getHandle().ascii().c_str());
  printMessage(msg);
}

void OdExDgDatabaseReactor::elementErased(const OdDgDatabase* pDb, const OdDgElement* pElm, bool erased)
{
  OdString msg;
  msg.format(OD_T("DB 0x%08X : elementErased(%ls)   <%-24ls %3ls>\n"), pDb, erased ? L"True " : L"False", pElm->isA()->name().c_str(), pElm->elementId().getHandle().ascii().c_str());
  printMessage(msg);
}

void OdExDgDatabaseReactor::headerSysVarWillChange(const OdDgDatabase* pDb, const OdString& name)
{
  OdString msg;

  msg.format(OD_T("DB 0x%08X : headerSysVarWillChange <%ls>\n"), pDb, name.c_str());
  printMessage(msg);
}

void OdExDgDatabaseReactor::headerSysVarChanged(const OdDgDatabase* pDb, const OdString& name)
{
  OdString msg;
  msg.format(OD_T("DB 0x%08X : headerSysVarChanged    <%ls>\n"), pDb, name.c_str());
  printMessage(msg);
}

void OdExDgDatabaseReactor::goodbye(const OdDgDatabase* /*pDb*/)
{
}
