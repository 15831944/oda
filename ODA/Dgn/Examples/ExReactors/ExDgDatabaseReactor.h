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

#ifndef __EX_DGDATABASEREACTOR_H__
#define __EX_DGDATABASEREACTOR_H__

#include "DgDatabaseReactor.h"

class OdExDgDatabaseReactor : public OdDgDatabaseReactor
{
protected:
  OdExDgDatabaseReactor() {}
public:
  virtual void printMessage(const OdString& msg) const = 0;

  //
  // OdDgDatabaseReactor overridden
  //
  virtual void elementAppended(const OdDgDatabase* pDb, const OdDgElement* pElm);
  virtual void elementUnAppended(const OdDgDatabase* pDb, const OdDgElement* pElm);
  virtual void elementReAppended(const OdDgDatabase* pDb, const OdDgElement* pElm);
  virtual void elementOpenedForModify(const OdDgDatabase* pDb, const OdDgElement* pElm);
  virtual void elementModified(const OdDgDatabase* pDb, const OdDgElement* pElm);
  virtual void elementErased(const OdDgDatabase* pDb, const OdDgElement* pElm, bool erased);
  virtual void headerSysVarWillChange(const OdDgDatabase* pDb, const OdString& name);
  virtual void headerSysVarChanged(const OdDgDatabase* pDb, const OdString& name);
  virtual void goodbye(const OdDgDatabase* pDb);
};

#endif //__EX_DGDATABASEREACTOR_H__
