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
#include "RxObjectImpl.h"
#include "DbObject.h"
#include "DbgObjectReactor.h"

ODRX_NO_CONS_DEFINE_MEMBERS(OdDbgObjectReactor, OdDbObjectReactor)

void OdDbgObjectReactor::cancelled(const OdDbObject* pObj)
{
  OdString msg;
  msg.format(OD_T("[OBJECT_REACTOR ] :  <%-24ls %3I64X>"), pObj->isA()->name().c_str(), (OdUInt64)pObj->getDbHandle());
  printMessage(msg);
}

void OdDbgObjectReactor::copied(const OdDbObject* pObj, const OdDbObject* pNewObj)
{
  OdString msg;
  msg.format(OD_T("[OBJECT_REACTOR ] : copied <%-24ls %3I64X> / new object <%-24ls %3I64X>"), pObj->isA()->name().c_str(), (OdUInt64)pObj->getDbHandle(), pNewObj->isA()->name().c_str(), (OdUInt64)pNewObj->getDbHandle());
  printMessage(msg);
}

void OdDbgObjectReactor::erased(const OdDbObject* pObj, bool bErasing)
{
  OdString msg;
  msg.format(bErasing ? OD_T("[OBJECT_REACTOR ] : erased <%-24ls %3I64X>") : OD_T("[OBJECT_REACTOR ] : unerased <%-24ls %3I64X>")
    , pObj->isA()->name().c_str(), (OdUInt64)pObj->getDbHandle());
  printMessage(msg);
}

void OdDbgObjectReactor::goodbye(const OdDbObject* pObj)
{
  OdString msg;
  msg.format(OD_T("[OBJECT_REACTOR ] : goodbye <%-24ls %3I64X>"), pObj->isA()->name().c_str(), (OdUInt64)pObj->getDbHandle());
  printMessage(msg);
}

void OdDbgObjectReactor::openedForModify(const OdDbObject* pObj)
{
  OdString msg;
  msg.format(OD_T("[OBJECT_REACTOR ] : openedForModify <%-24ls %3I64X>"), pObj->isA()->name().c_str(), (OdUInt64)pObj->getDbHandle());
  printMessage(msg);
}

void OdDbgObjectReactor::modified(const OdDbObject* pObj)
{
  OdString msg;
  msg.format(OD_T("[OBJECT_REACTOR ] : modified <%-24ls %3I64X>"), pObj->isA()->name().c_str(), (OdUInt64)pObj->getDbHandle());
  printMessage(msg);
}

void OdDbgObjectReactor::modifiedGraphics(const OdDbObject* pObj)
{
  OdString msg;
  msg.format(OD_T("[OBJECT_REACTOR ] : modifiedGraphics <%-24ls %3I64X>"), pObj->isA()->name().c_str(), (OdUInt64)pObj->getDbHandle());
  printMessage(msg);
}

void OdDbgObjectReactor::subObjModified(const OdDbObject* pObj, const OdDbObject* /*pSubObj*/)
{
  OdString msg;
  msg.format(OD_T("[OBJECT_REACTOR ] : subObjModified <%-24ls %3I64X>"), pObj->isA()->name().c_str(), (OdUInt64)pObj->getDbHandle());
  printMessage(msg);
}

void OdDbgObjectReactor::modifyUndone(const OdDbObject* pObj)
{
  OdString msg;
  msg.format(OD_T("[OBJECT_REACTOR ] : modifyUndone <%-24ls %3I64X>"), pObj->isA()->name().c_str(), (OdUInt64)pObj->getDbHandle());
  printMessage(msg);
}

void OdDbgObjectReactor::modifiedXData(const OdDbObject* pObj)
{
  OdString msg;
  msg.format(OD_T("[OBJECT_REACTOR ] : modifiedXData <%-24ls %3I64X>"), pObj->isA()->name().c_str(), (OdUInt64)pObj->getDbHandle());
  printMessage(msg);
}

void OdDbgObjectReactor::unappended(const OdDbObject* pObj)
{
  OdString msg;
  msg.format(OD_T("[OBJECT_REACTOR ] : unappended <%-24ls %3I64X>"), pObj->isA()->name().c_str(), (OdUInt64)pObj->getDbHandle());
  printMessage(msg);
}

void OdDbgObjectReactor::reappended(const OdDbObject* pObj)
{
  OdString msg;
  msg.format(OD_T("[OBJECT_REACTOR ] : reappended <%-24ls %3I64X>"), pObj->isA()->name().c_str(), (OdUInt64)pObj->getDbHandle());
  printMessage(msg);
}

void OdDbgObjectReactor::objectClosed(const OdDbObjectId& objId)
{
  OdString msg;
  msg.format(OD_T("[OBJECT_REACTOR ] : objectClosed <%3I64X>"), (OdUInt64)objId.getHandle());
  printMessage(msg);
}
