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

/************************************************************************/
/* Implements notification functions for the evaluation of              */
/* OdDbDimension  entities                                              */
/************************************************************************/

#include "StdAfx.h"
#include "ExDimAssocWatcher.h"
#include "RxObjectImpl.h"
#include "DbAudit.h"

#include "DbDimAssoc.h"
#include "DbDimension.h"
#include "DbDictionary.h"

OdExDimAssocWatcherPE::OdExDimAssocWatcherPE()
  : _openCnt(0)
{
}

void OdExDimAssocWatcherPE::modified(OdDbObject* pObj, const OdDbObject* pAssocObj)
{
  ODA_TRACE2("OdExDimAssocWatcherPE::modified: %ls is notified by %ls", odDbGetObjectName(pObj).c_str(), odDbGetObjectName(pAssocObj).c_str());

  if (_openCnt)
  {
    OdDbDimAssocPtr pDimAssoc = OdDbDimAssoc::cast(pObj);
    if (pDimAssoc.get())
    {
      if (pDimAssoc->dimObjId() == pAssocObj->objectId())
      {
        _removeAssocIds.push_back(pDimAssoc->objectId());
      }
    }
  }
}

void OdExDimAssocWatcherPE::copied(OdDbObject* pObj, const OdDbObject* pAssocObj, const OdDbObject* /*pNewObj*/)
{
  ODA_TRACE2("OdExDimAssocWatcherPE::copied: %ls is notified by %ls", odDbGetObjectName(pObj).c_str(), odDbGetObjectName(pAssocObj).c_str());
}

void OdExDimAssocWatcherPE::erased(OdDbObject* pObj, const OdDbObject* pAssocObj, bool /*erasing*/)
{
  ODA_TRACE2("OdExDimAssocWatcherPE::erased: %ls is notified by %ls", odDbGetObjectName(pObj).c_str(), odDbGetObjectName(pAssocObj).c_str());
}

void OdExDimAssocWatcherPE::open()
{
  if (_openCnt == 0)
  {
    clear();
  }
  ++_openCnt;
}

void OdExDimAssocWatcherPE::close(bool bUpdate)
{
  if (_openCnt > 0)
  {
    if (--_openCnt == 0)
    {
      if (bUpdate)
        update();
      clear();
    }
  }
}

void OdExDimAssocWatcherPE::clear()
{
  _removeAssocIds.resize(0);
}

void OdExDimAssocWatcherPE::update()
{
  unsigned i = 0;
  for (; i < _removeAssocIds.size(); ++i)
  {
    OdDbDimAssocPtr pDimAssoc = _removeAssocIds[i].safeOpenObject(OdDb::kForWrite);
    pDimAssoc->removeAssociativity();
  }
}

