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


#ifndef _ODEX_DBGOBJECTREACTOR_INCLUDED_
#define _ODEX_DBGOBJECTREACTOR_INCLUDED_

#include "DbObject.h"
#include "DbObjectReactor.h"
#include "../ExCustObjExport.h"

class EXCUSTOBJEXPORT OdDbgObjectReactor : public OdDbObjectReactor
{
protected:
  OdDbgObjectReactor() {}
public:
  ODRX_DECLARE_MEMBERS(OdDbgObjectReactor);

  virtual void printMessage(const OdString& msg) const = 0;

  virtual void cancelled(const OdDbObject* pDbObj);
  virtual void copied(const OdDbObject* pDbObj, const OdDbObject* newObj);
  virtual void erased(const OdDbObject* pDbObj, bool pErasing = true);
  virtual void goodbye(const OdDbObject* pDbObj);
  virtual void openedForModify(const OdDbObject* pDbObj); 
  virtual void modified(const OdDbObject* pDbObj);
  virtual void modifiedGraphics(const OdDbObject* pDbObj);
  virtual void subObjModified(const OdDbObject* pDbObj, const OdDbObject* subObj);
  virtual void modifyUndone(const OdDbObject* pDbObj);
  virtual void modifiedXData(const OdDbObject* pDbObj);
  virtual void unappended(const OdDbObject* pDbObj);
  virtual void reappended(const OdDbObject* pDbObj);
  virtual void objectClosed(const OdDbObjectId& objId);
};

#endif //_ODEX_DBGOBJECTREACTOR_INCLUDED_
