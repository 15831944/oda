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
// ExtDbUtils.h
//

#ifndef __EXTDB_UTILS__H__
#define __EXTDB_UTILS__H__

#include "DbBaseDatabase.h"
#include "RxDictionary.h"
#include "OdString.h"

namespace OdExtDbUtils
{
  enum DbType
  {
    kDbUnknown = 0,
    kDbDwg,
    kDbDwf,
    kDbDgn,
    kDbPrc,
    kDbIfc,
    kDbBim,
    kDbVis,
  };

  inline bool checkType(const OdRxObject* pObject, const OdString& dbClassName)
  {
    OdRxDictionaryPtr pClassDict = odrxClassDictionary();
    if (pClassDict.isNull())
    {
      throw OdError(eNotInitializedYet);
    }
    OdRxClassPtr pClass = OdRxClass::cast(pClassDict->getAt(dbClassName));
    if (pClass.isNull())
    {
      return false; // ok. Db lib was not loaded.
    }
    return pObject->isKindOf(pClass);
  }

  /**
    Get database product in run-time.
    The database should extend base product database class with the help of RxObject-methods.
  */
  inline DbType getProduct(const OdDbBaseDatabase* pDb)
  {
    if (!pDb) {
      ODA_FAIL_ONCE();
      return kDbUnknown;
    }

    if (checkType(pDb, OD_T("OdDbDatabaseTv"))) return kDbVis;
    if (checkType(pDb, OD_T("OdDbDatabase"))) return kDbDwg;
    if (checkType(pDb, OD_T("OdDwfDatabase"))) return kDbDwf;
    if (checkType(pDb, OD_T("OdDgDatabase"))) return kDbDgn;
    if (checkType(pDb, OD_T("OdPrcFile"))) return kDbPrc;
    if (checkType(pDb, OD_T("OdIfcFile"))) return kDbIfc;
    if (checkType(pDb, OD_T("OdBmDatabase"))) return kDbBim;

    ODA_FAIL_ONCE();
    return kDbUnknown;
  }

}

#endif // __EXTDB_UTILS__H__
