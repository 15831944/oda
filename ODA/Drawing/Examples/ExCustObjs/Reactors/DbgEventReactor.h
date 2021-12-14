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


#ifndef _ODEX_DBGEDITORREACTOR_INCLUDED_
#define _ODEX_DBGEDITORREACTOR_INCLUDED_

#include "Editor.h"
#include "../ExCustObjExport.h"

class EXCUSTOBJEXPORT OdDbgEditorReactor : public OdEditorReactor
{
public:
  virtual void printMessage(const OdString& msg) const = 0;

  //
  // OdRxEventReactor overridden
  //
  // DWG/Save Events.
  virtual void dwgFileOpened(OdDbDatabase* pDb, const OdString& filename);
  virtual void initialDwgFileOpenComplete(OdDbDatabase* pDb);
  virtual void databaseConstructed(OdDbDatabase* pDb);
  virtual void databaseToBeDestroyed(OdDbDatabase* pDb);
  virtual void beginSave(OdDbDatabase* pDb, const OdString& pIntendedName);
  virtual void saveComplete(OdDbDatabase* pDb, const OdString& pActualName);
  virtual void abortSave(OdDbDatabase* pDb);
  // DXF In/Out Events.
  virtual void beginDxfIn(OdDbDatabase* pDb);
  virtual void abortDxfIn(OdDbDatabase* pDb);
  virtual void dxfInComplete(OdDbDatabase* pDb);
  virtual void beginDxfOut(OdDbDatabase* pDb);
  virtual void abortDxfOut(OdDbDatabase* pDb);
  virtual void dxfOutComplete(OdDbDatabase* pDb);
  // Insert Events.
  virtual void beginInsert(OdDbDatabase* pTo, const OdString& pBlockName, OdDbDatabase* pFrom);
  virtual void beginInsert(OdDbDatabase* pTo, const OdGeMatrix3d& xform, OdDbDatabase* pFrom);
  virtual void otherInsert(OdDbDatabase* pTo, OdDbIdMapping& idMap, OdDbDatabase* pFrom);
  virtual void abortInsert(OdDbDatabase* pTo);
  virtual void endInsert(OdDbDatabase* pTo);
  // Wblock Events.
  virtual void wblockNotice(OdDbDatabase* pDb);
  virtual void beginWblock(OdDbDatabase* pTo, OdDbDatabase* pFrom, const OdGePoint3d& insertionPoint);
  virtual void beginWblock(OdDbDatabase* pTo, OdDbDatabase* pFrom, OdDbObjectId blockId);
  virtual void beginWblock(OdDbDatabase* pTo, OdDbDatabase* pFrom);
  virtual void otherWblock(OdDbDatabase* pTo, OdDbIdMapping& idMap, OdDbDatabase* pFrom);
  virtual void abortWblock(OdDbDatabase* pTo);
  virtual void endWblock(OdDbDatabase* pTo);
  virtual void beginWblockObjects(OdDbDatabase* pFrom, OdDbIdMapping& idMap);
  // Deep Clone Events.
  virtual void beginDeepClone(OdDbDatabase* pTo, OdDbIdMapping& idMap);
  virtual void beginDeepCloneXlation(OdDbIdMapping& idMap);
  virtual void abortDeepClone(OdDbIdMapping& idMap);
  virtual void endDeepClone(OdDbIdMapping& idMap);
  // Partial Open Events.
  virtual void partialOpenNotice(OdDbDatabase* pDb);

  //
  // OdEditorReactor overridden
  //
  // Xref-related Events.
  void beginAttach(OdDbDatabase* pToDb, const OdString& filename, OdDbDatabase* pFromDb);
  void abortAttach(OdDbDatabase* pFromDb);
  void endAttach(OdDbDatabase* pToDb);
};

#endif //_ODEX_DBGEDITORREACTOR_INCLUDED_
