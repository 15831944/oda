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
/* OdDHatch  entities                                                   */
/************************************************************************/
#include "StdAfx.h"
#include "ExHatchWatcher.h"
#include "RxObjectImpl.h"

OdExHatchWatcherPE::OdExHatchWatcherPE()
  : _openCnt(0)
{
}

void OdExHatchWatcherPE::modified(OdDbObject* pObj, const OdDbObject* pAssocObj)
{
  if (_openCnt)
  {
    OdDbObjectId objId = pObj->objectId(),
      assocId = pAssocObj->objectId();
    IdAssocIdsMap::iterator it = _assocMap.find(objId);
    if (it == _assocMap.end())
    {
      it = _assocMap.insert(_assocMap.end(), IdAssocIdsMap::value_type(objId, OdDbObjectIdArray()));
    }
    if (!it->second.contains(assocId,0))
    {
      it->second.push_back(assocId);
    }
  }
}

void OdExHatchWatcherPE::modifiedItself(OdDbHatch* pHatch)
{
  if (_openCnt)
  {
    OdDbObjectId hatchId = pHatch->objectId();
    IdAssocIdsMap::iterator it = _assocMap.find(hatchId);
    if (it != _assocMap.end())
    {
      _assocMap.erase(it);
    }
    if (!_modifiedHatches.contains(hatchId,0))
    {
      _modifiedHatches.push_back(hatchId);
    }
  }
}

void OdExHatchWatcherPE::update()
{
  IdAssocIdsMap::iterator it = _assocMap.begin();
  for (; it !=  _assocMap.end(); ++it)
  {
    OdDbHatchPtr pHatch = OdDbHatch::cast(it->first.openObject(OdDb::kForWrite));
    if (!pHatch.isNull())
    {
      evaluate(pHatch, it->second);
    }
  }
  OdDbObjectIdArray::iterator ith = _modifiedHatches.begin();
  for (; ith !=  _modifiedHatches.end(); ++ith)
  {
    OdDbHatchPtr pHatch = OdDbHatch::cast(ith->openObject(OdDb::kForWrite));
    if (!pHatch.isNull())
    {
      pHatch->setAssociative(false);
    }
  }
}

void OdExHatchWatcherPE::clear()
{
  _assocMap.erase(_assocMap.begin(), _assocMap.end());
  _modifiedHatches.clear();
}

void OdExHatchWatcherPE::open()
{
  if (_openCnt == 0)
  {
    clear();
  }
  ++_openCnt;
}

void OdExHatchWatcherPE::close(bool bUpdate)
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

