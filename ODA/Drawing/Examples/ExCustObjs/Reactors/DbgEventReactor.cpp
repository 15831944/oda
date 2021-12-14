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
#include "DbgEventReactor.h"

/************************************************************************/
/*  DWG file Open/Save Events                                           */
/************************************************************************/
void OdDbgEditorReactor::dwgFileOpened(OdDbDatabase* pDb, const OdString& fileName)
{
  OdString msg;
  msg.format(OD_T("[RXEVENT_REACTOR] : dwgFileOpened DB = 0x%08X, fileName = %ls"), pDb, fileName.c_str());
  printMessage(msg);
}
void OdDbgEditorReactor::initialDwgFileOpenComplete(OdDbDatabase* pDb)
{
  OdString msg;
  msg.format(OD_T("[RXEVENT_REACTOR] : initialDwgFileOpenComplete DB = 0x%08X"), pDb);
  printMessage(msg);
}
void OdDbgEditorReactor::databaseConstructed(OdDbDatabase* pDb)
{
  OdString msg;
  msg.format(OD_T("[RXEVENT_REACTOR] : databaseConstructed DB = 0x%08X"), pDb);
  printMessage(msg);
}
void OdDbgEditorReactor::databaseToBeDestroyed(OdDbDatabase* pDb)
{
  OdString msg;
  msg.format(OD_T("[RXEVENT_REACTOR] : databaseToBeDestroyed DB = 0x%08X"), pDb);
  printMessage(msg);
}
void OdDbgEditorReactor::beginSave(OdDbDatabase* pDb, const OdString& pIntendedName)
{
  OdString msg;
  msg.format(OD_T("[RXEVENT_REACTOR] : beginSave DB = 0x%08X, fileName = %ls"), pDb, pIntendedName.c_str());
  printMessage(msg);
}
void OdDbgEditorReactor::saveComplete(OdDbDatabase* pDb, const OdString& pActualName)
{
  OdString msg;
  msg.format(OD_T("[RXEVENT_REACTOR] : saveComplete DB = 0x%08X, fileName = %ls"), pDb, pActualName.c_str());
  printMessage(msg);
}
void OdDbgEditorReactor::abortSave(OdDbDatabase* pDb)
{
  OdString msg;
  msg.format(OD_T("[RXEVENT_REACTOR] : abortSave DB = 0x%08X"), pDb);
  printMessage(msg);
}

/************************************************************************/
/* DXF In/Out Events                                                    */
/************************************************************************/
void OdDbgEditorReactor::beginDxfIn(OdDbDatabase* pDb)
{
  OdString msg;
  msg.format(OD_T("[RXEVENT_REACTOR] : beginDxfIn DB = 0x%08X"), pDb);
  printMessage(msg);
}
void OdDbgEditorReactor::abortDxfIn(OdDbDatabase* pDb)
{
  OdString msg;
  msg.format(OD_T("[RXEVENT_REACTOR] : abortDxfIn DB = 0x%08X"), pDb);
  printMessage(msg);
}
void OdDbgEditorReactor::dxfInComplete(OdDbDatabase* pDb)
{
  OdString msg;
  msg.format(OD_T("[RXEVENT_REACTOR] : dxfInComplete DB = 0x%08X"), pDb);
  printMessage(msg);
}
void OdDbgEditorReactor::beginDxfOut(OdDbDatabase* pDb)
{
  OdString msg;
  msg.format(OD_T("[RXEVENT_REACTOR] : beginDxfOut DB = 0x%08X"), pDb);
  printMessage(msg);
}
void OdDbgEditorReactor::abortDxfOut(OdDbDatabase* pDb)
{
  OdString msg;
  msg.format(OD_T("[RXEVENT_REACTOR] : abortDxfOut DB = 0x%08X"), pDb);
  printMessage(msg);
}
void OdDbgEditorReactor::dxfOutComplete(OdDbDatabase* pDb)
{
  OdString msg;
  msg.format(OD_T("[RXEVENT_REACTOR] : dxfOutComplete DB = 0x%08X"), pDb);
  printMessage(msg);
}

/************************************************************************/
/* Insert Events                                                        */
/************************************************************************/
void OdDbgEditorReactor::beginInsert(OdDbDatabase* pTo, const OdString& pBlockName, OdDbDatabase* pFrom)
{
  OdString msg;
  msg.format(OD_T("[RXEVENT_REACTOR] : beginInsert DB_TO = 0x%08X, BlockName = %ls, DB_FROM = 0x%08X"), pTo, pBlockName.c_str(), pFrom);
  printMessage(msg);
}
void OdDbgEditorReactor::beginInsert(OdDbDatabase* pTo, const OdGeMatrix3d& /*xform*/, OdDbDatabase* pFrom)
{
  OdString msg;
  msg.format(OD_T("[RXEVENT_REACTOR] : beginInsert DB_TO = 0x%08X, <xform>, DB_FROM = 0x%08X"), pTo, pFrom);
  printMessage(msg);
}
void OdDbgEditorReactor::otherInsert(OdDbDatabase* pTo, OdDbIdMapping& idMap, OdDbDatabase* pFrom)
{
  OdString msg;
  msg.format(OD_T("[RXEVENT_REACTOR] : otherInsert DB_TO = 0x%08X, IdMap = 0x%08X, DB_FROM = 0x%08X"), pTo, &idMap, pFrom);
  printMessage(msg);
}
void OdDbgEditorReactor::abortInsert(OdDbDatabase* pTo)
{
  OdString msg;
  msg.format(OD_T("[RXEVENT_REACTOR] : abortInsert DB = 0x%08X"), pTo);
  printMessage(msg);
}
void OdDbgEditorReactor::endInsert(OdDbDatabase* pTo)
{
  OdString msg;
  msg.format(OD_T("[RXEVENT_REACTOR] : endInsert DB = 0x%08X"), pTo);
  printMessage(msg);
}

/************************************************************************/
/* Wblock Events                                                        */
/************************************************************************/
void OdDbgEditorReactor::wblockNotice(OdDbDatabase* pDb)
{
  OdString msg;
  msg.format(OD_T("[RXEVENT_REACTOR] : wblockNotice DB = 0x%08X"), pDb);
  printMessage(msg);
}
void OdDbgEditorReactor::beginWblock(OdDbDatabase* pTo, OdDbDatabase* pFrom, const OdGePoint3d& /*insertionPoint*/)
{
  OdString msg;
  msg.format(OD_T("[RXEVENT_REACTOR] : beginWblock DB_TO = 0x%08X, DB_FROM = 0x%08X, <insertionPoint>"), pTo, pFrom);
  printMessage(msg);
}
void OdDbgEditorReactor::beginWblock(OdDbDatabase* pTo, OdDbDatabase* pFrom, OdDbObjectId /*blockId*/)
{
  OdString msg;
  msg.format(OD_T("[RXEVENT_REACTOR] : beginWblock DB_TO = 0x%08X, DB_FROM = 0x%08X, <blockId>"), pTo, pFrom);
  printMessage(msg);
}
void OdDbgEditorReactor::beginWblock(OdDbDatabase* pTo, OdDbDatabase* pFrom)
{
  OdString msg;
  msg.format(OD_T("[RXEVENT_REACTOR] : beginWblock DB_TO = 0x%08X, <xform>, DB_FROM = 0x%08X"), pTo, pFrom);
  printMessage(msg);
}
void OdDbgEditorReactor::otherWblock(OdDbDatabase* pTo, OdDbIdMapping& idMap, OdDbDatabase* pFrom)
{
  OdString msg;
  msg.format(OD_T("[RXEVENT_REACTOR] : otherWblock DB_TO = 0x%08X, IdMap = 0x%08X, DB_FROM = 0x%08X"), pTo, &idMap, pFrom);
  printMessage(msg);
}
void OdDbgEditorReactor::abortWblock(OdDbDatabase* pTo)
{
  OdString msg;
  msg.format(OD_T("[RXEVENT_REACTOR] : abortWblock DB = 0x%08X"), pTo);
  printMessage(msg);
}
void OdDbgEditorReactor::endWblock(OdDbDatabase* pTo)
{
  OdString msg;
  msg.format(OD_T("[RXEVENT_REACTOR] : endWblock DB = 0x%08X"), pTo);
  printMessage(msg);
}
void OdDbgEditorReactor::beginWblockObjects(OdDbDatabase* pFrom, OdDbIdMapping& idMap)
{
  OdString msg;
  msg.format(OD_T("[RXEVENT_REACTOR] : beginWblockObjects DB_FROM = 0x%08X, IdMap = 0x%08X"), pFrom, &idMap);
  printMessage(msg);
}

/************************************************************************/
/* Deep Clone Events                                                    */
/************************************************************************/
void OdDbgEditorReactor::beginDeepClone(OdDbDatabase* pTo, OdDbIdMapping& idMap)
{
  OdString msg;
  msg.format(OD_T("[RXEVENT_REACTOR] : beginDeepClone DB = 0x%08X, IdMap = 0x%08X"), pTo, &idMap);
  printMessage(msg);
}
void OdDbgEditorReactor::beginDeepCloneXlation(OdDbIdMapping& idMap)
{
  OdString msg;
  msg.format(OD_T("[RXEVENT_REACTOR] : beginDeepCloneXlation IdMap = 0x%08X"), &idMap);
  printMessage(msg);
}
void OdDbgEditorReactor::abortDeepClone(OdDbIdMapping& idMap)
{
  OdString msg;
  msg.format(OD_T("[RXEVENT_REACTOR] : abortDeepClone IdMap = 0x%08X"), &idMap);
  printMessage(msg);
}
void OdDbgEditorReactor::endDeepClone(OdDbIdMapping& idMap)
{
  OdString msg;
  msg.format(OD_T("[RXEVENT_REACTOR] : endDeepClone IdMap = 0x%08X"), &idMap);
  printMessage(msg);
}

/************************************************************************/
/* Partial Open Events                                                  */
/************************************************************************/
void OdDbgEditorReactor::partialOpenNotice(OdDbDatabase* pDb)
{
  OdString msg;
  msg.format(OD_T("[RXEVENT_REACTOR] : partialOpenNotice DB = 0x%08X"), pDb);
  printMessage(msg);
}

/************************************************************************/
/* Xref-related Events                                                  */
/************************************************************************/
void OdDbgEditorReactor::beginAttach(OdDbDatabase* pToDb, const OdString& filename, OdDbDatabase* pFromDb)
{
  OdString msg;
  msg.format(OD_T("[EDITOR_REACTOR ] : beginAttach HostDB = 0x%08X, XrefDB = 0x%08X, fileName = %ls"), pToDb, pFromDb, filename.c_str());
  printMessage(msg);
}

void OdDbgEditorReactor::abortAttach(OdDbDatabase* pFromDb)
{
  OdString msg;
  msg.format(OD_T("[EDITOR_REACTOR ] : abortAttach DB = 0x%08X"), pFromDb);
  printMessage(msg);
}

void OdDbgEditorReactor::endAttach(OdDbDatabase* pToDb)
{
  OdString msg;
  msg.format(OD_T("[EDITOR_REACTOR ] : endAttach DB = 0x%08X"), pToDb);
  printMessage(msg);
}
