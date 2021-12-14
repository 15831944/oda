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

#ifdef _DEBUG

#include "OdaCommon.h"
#include "ExTraceDbReactor.h"
#include "DbHostAppServices.h"

OdStaticRxObject<ExTraceDbReactor> g_traceDbReactor;

void ExTraceDbReactor::attachTo(OdDbDatabase* pDb) {
  if (pDb) {
    pDb->addReactor(&g_traceDbReactor);
    pDb->appServices()->layoutManager()->addReactor(&g_traceDbReactor);
  }
}

void ExTraceDbReactor::detach(OdDbDatabase* pDb) {
  if (pDb)   {
    pDb->removeReactor(&g_traceDbReactor);
    pDb->appServices()->layoutManager()->removeReactor(&g_traceDbReactor);
  }
}

void ExTraceDbReactor::objectAppended(const OdDbDatabase* /*db*/, const OdDbObject* obj)
{
  ODA_TRACE2("<objectAppended         : %ls, %ls>\n", obj->isA()->name().c_str(), obj->objectId().getHandle().ascii().c_str());
}
void ExTraceDbReactor::objectUnAppended(const OdDbDatabase* /*db*/, const OdDbObject* obj)
{
  ODA_TRACE2("<objectUnAppended       : %ls, %ls>\n", obj->isA()->name().c_str(), obj->objectId().getHandle().ascii().c_str());
}
void ExTraceDbReactor::objectReAppended(const OdDbDatabase* /*db*/, const OdDbObject* obj)
{
  ODA_TRACE2("<objectReAppended       : %ls, %ls>\n", obj->isA()->name().c_str(), obj->objectId().getHandle().ascii().c_str());
}
void ExTraceDbReactor::objectOpenedForModify(const OdDbDatabase* /*db*/, const OdDbObject* obj)
{
  ODA_TRACE2("<objectOpenedForModify  : %ls, %ls>\n", obj->isA()->name().c_str(), obj->objectId().getHandle().ascii().c_str());
}
void ExTraceDbReactor::objectModified(const OdDbDatabase* /*db*/, const OdDbObject* obj)
{
  ODA_TRACE2("<objectModified         : %ls, %ls>\n", obj->isA()->name().c_str(), obj->objectId().getHandle().ascii().c_str());
}
void ExTraceDbReactor::objectErased(const OdDbDatabase* /*db*/, const OdDbObject* obj, bool /*bErased*/)
{
  ODA_TRACE3("<objectErased      %ls: %ls, %ls>\n", OdString(obj->isErased() ? OD_T("TRUE ") : OD_T("FALSE")).c_str(), obj->isA()->name().c_str(), obj->objectId().getHandle().ascii().c_str());
}
void ExTraceDbReactor::headerSysVarWillChange(const OdDbDatabase* /*db*/, const OdString& sysVar)
{
  ODA_TRACE1("<headerSysVarWillChange : %ls>\n", sysVar.c_str());
}
void ExTraceDbReactor::headerSysVarChanged(const OdDbDatabase* /*db*/, const OdString& sysVar)
{
  ODA_TRACE1("<headerSysVarChanged    : %ls>\n", sysVar.c_str());
}
void ExTraceDbReactor::layoutCreated(const OdString&, const OdDbObjectId&)
{
}
void ExTraceDbReactor::layoutToBeRemoved(const OdString&, const OdDbObjectId&)
{
}
void ExTraceDbReactor::layoutRemoved(const OdString&, const OdDbObjectId&)
{
}
void ExTraceDbReactor::abortLayoutRemoved(const OdString&, const OdDbObjectId&)
{
}
void ExTraceDbReactor::layoutToBeCopied(const OdString&, const OdDbObjectId&)
{
}
void ExTraceDbReactor::layoutCopied(const OdString&, const OdDbObjectId&, const OdString&, const OdDbObjectId&)
{
}
void ExTraceDbReactor::abortLayoutCopied(const OdString&, const OdDbObjectId&)
{
}
void ExTraceDbReactor::layoutToBeRenamed(const OdString&, const OdString&, const OdDbObjectId&)
{
}
void ExTraceDbReactor::layoutRenamed(const OdString&, const OdString&, const OdDbObjectId&)
{
}
void ExTraceDbReactor::abortLayoutRename(const OdString&, const OdString&, const OdDbObjectId&)
{
}
void ExTraceDbReactor::layoutSwitched(const OdString& name, const OdDbObjectId&)
{
  ODA_TRACE1("<layoutSwitched         : %ls>\n", name.c_str());
}
void ExTraceDbReactor::plotStyleTableChanged(const OdString&, const OdDbObjectId&)
{
}

#endif //#ifdef _DEBUG


