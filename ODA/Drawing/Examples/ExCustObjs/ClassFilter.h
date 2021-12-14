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

#pragma once

#include "RxObject.h"
#include "DbUserIO.h"


class ClassFilter : public OdStaticRxObject<OdEdSSetTracker>
{
  const OdRxClass* m_pClass;
public:
  ClassFilter()
    : m_pClass(OdRxObject::desc())
  {
  }
  ClassFilter(const OdRxClass* pClass)
    : m_pClass(pClass)
  {
  }
  void setClass(const OdRxClass* pClass)
  {
    m_pClass = pClass;
  }

  int addDrawables(OdGsView*) { return 0; }
  void removeDrawables(OdGsView*) { }

  bool check(const OdDbObjectId& entId)
  {
    try
    {
      OdDbObjectPtr pObj = entId.safeOpenObject();
      return pObj->isKindOf(m_pClass);
    }
    catch (const OdError&)
    {
    }
    return false;
  }

  bool append(const OdDbObjectId& entId, const OdDbSelectionMethod*)
  {
    return check(entId);
  }

  bool remove(const OdDbObjectId& entId, const OdDbSelectionMethod*)
  {
    return check(entId);
  }
};
