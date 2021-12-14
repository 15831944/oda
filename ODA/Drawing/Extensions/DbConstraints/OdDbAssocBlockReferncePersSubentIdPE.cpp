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
#include "DbBlockReference.h"
#include "OdDbAssocBlockReferncePersSubentIdPE.h"

ODRX_NO_CONS_DEFINE_MEMBERS_ALTNAME(OdDbAssocBlockReferncePersSubentIdPE, OdDbAssocPointBasedPersSubentIdPE, L"AcDbAssocBlockReferncePersSubentIdPE");

static OdDbSubentId getBrefSubent()
{
  return OdDbSubentId(OdDb::kVertexSubentType, OdGsMarker(-1));
}

OdDbAssocPersSubentIdPtr OdDbAssocBlockReferncePersSubentIdPE::createNewPersSubent(OdDbEntity* pEntity,
                                                                                   const OdDbCompoundObjectId&,
                                                                                   const OdDbSubentId& subentId)
{
  OdDbAssocSimplePersSubentIdPtr res;
  if (isTypeEqual(pEntity, OdDbBlockReference::desc()) && subentId == getBrefSubent())
  {
    res = OdDbAssocSimplePersSubentId::createObject();
    res->init(subentId);
  }
  return res;
}

OdResult OdDbAssocBlockReferncePersSubentIdPE::getAllSubentities(const OdDbEntity* pEntity,
                                                                 OdDb::SubentType subentType,
                                                                 OdArray<OdDbSubentId>& allSubentIds)
{
  if (subentType == OdDb::kVertexSubentType && isTypeEqual(pEntity, OdDbBlockReference::desc()))
  {
    allSubentIds.push_back(getBrefSubent());
    return eOk;
  }
  return eBadObjType;
}

OdResult OdDbAssocBlockReferncePersSubentIdPE::getVertexSubentityGeometry(const OdDbEntity* pEntity,
                                                                          const OdDbSubentId& vertexSubentId,
                                                                          OdGePoint3d& vertexPosition)
{
  if (isTypeEqual(pEntity, OdDbBlockReference::desc()) && vertexSubentId == getBrefSubent())
  {
    vertexPosition = static_cast<const OdDbBlockReference*>(pEntity)->position();
    return eOk;
  }
  return eBadObjType;
}

OdResult OdDbAssocBlockReferncePersSubentIdPE::setVertexSubentityGeometry(OdDbEntity* pEntity,
                                                                          const OdDbSubentId& vertexSubentId,
                                                                          const OdGePoint3d& newVertexPosition)
{
  if (isTypeEqual(pEntity, OdDbBlockReference::desc()) && vertexSubentId == getBrefSubent())
  {
    static_cast<OdDbBlockReference*>(pEntity)->setPosition(newVertexPosition);
    return eOk;
  }
  return eBadObjType;
}

OdResult OdDbAssocBlockReferncePersSubentIdPE::setSubentityGeometry(OdDbEntity* pEntity,
                                                                    const OdDbSubentId& subentId,
                                                                    const OdDbEntity* pNewSubentityGeometry)
{
  if (isTypeEqual(pEntity, OdDbBlockReference::desc()) &&
      isTypeEqual(pNewSubentityGeometry, OdDbBlockReference::desc()) && subentId == getBrefSubent())
  {
    OdGePoint3d vert = static_cast<const OdDbBlockReference*>(pNewSubentityGeometry)->position();
    static_cast<OdDbBlockReference*>(pEntity)->setPosition(vert);
    return eOk;
  }
  return eBadObjType;
}

OdResult OdDbAssocBlockReferncePersSubentIdPE::getTransientSubentIds(const OdDbEntity* pEntity,
  const OdDbAssocPersSubentId* pPerSubentId,
  OdArray<OdDbSubentId>& subents) const
{
  //m_pPersSubent = pPerSubentId;
  return OdDbAssocPointBasedPersSubentIdPE::getTransientSubentIds(pEntity, pPerSubentId, subents);
}
