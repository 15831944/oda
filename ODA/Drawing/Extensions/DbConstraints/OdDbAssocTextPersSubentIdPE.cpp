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

#include "OdDbAssocTextPersSubentIdPE.h"

#include "DbMText.h"
#include "DbText.h"

ODRX_NO_CONS_DEFINE_MEMBERS_ALTNAME(OdDbAssocTextPersSubentIdPE,
                                    OdDbAssocPointBasedPersSubentIdPE,
                                    L"AcDbAssocTextPersSubentIdPE");

ODRX_NO_CONS_DEFINE_MEMBERS_ALTNAME(OdDbAssocMTextPersSubentIdPE,
                                    OdDbAssocPointBasedPersSubentIdPE,
                                    L"AcDbAssocMTextPersSubentIdPE");

static OdDbSubentId textInsertionSubent()
{
  return OdDbSubentId(OdDb::kVertexSubentType, -1);
}

static OdGePoint3d getPosition(const OdDbText& text)
{
  return text.position();
}

static OdGePoint3d getPosition(const OdDbMText& mtext)
{
  return mtext.location();
}

static void setPosition(OdDbText& text, const OdGePoint3d& position)
{
  text.setPosition(position);
}

static void setPosition(OdDbMText& mtext, const OdGePoint3d& position)
{
  mtext.setLocation(position);
}

template<typename T>
class OdDbImpAssocTextPersSubentIdPE
{
public:
  static bool isValid(const OdDbEntity* pEnt) { return pEnt && pEnt->isKindOf(T::desc()); }

  static OdDbAssocPersSubentIdPtr
  createNewPersSubent(OdDbEntity* pEntity, const OdDbCompoundObjectId&, const OdDbSubentId& subentId)
  {
    OdDbAssocSimplePersSubentIdPtr res;
    if (isValid(pEntity) && subentId == textInsertionSubent())
    {
      res = OdDbAssocSimplePersSubentId::createObject();
      res->init(subentId);
    }
    return res;
  }

  static OdResult
  getAllSubentities(const OdDbEntity* pEntity, OdDb::SubentType subentType, OdArray<OdDbSubentId>& allSubentIds)
  {
    OdResult res = eBadObjType;
    if (subentType == OdDb::kVertexSubentType && isValid(pEntity))
    {
      allSubentIds.push_back(textInsertionSubent());
      res = eOk;
    }
    return res;
  }

  static OdResult
  getVertexSubentityGeometry(const OdDbEntity* pEntity, const OdDbSubentId& vertexSubentId, OdGePoint3d& vertexPosition)
  {
    OdResult res = eBadObjType;
    if (isValid(pEntity) && vertexSubentId == textInsertionSubent())
    {
      vertexPosition = getPosition(static_cast<const T&>(*pEntity));
      res = eOk;
    }
    return res;
  }

  static OdResult setVertexSubentityGeometry(OdDbEntity* pEntity,
                                             const OdDbSubentId& vertexSubentId,
                                             const OdGePoint3d& newVertexPosition)
  {
    if (isValid(pEntity) && vertexSubentId == textInsertionSubent())
    {
      setPosition(static_cast<T&>(*pEntity), newVertexPosition);
      return eOk;
    }
    return eBadObjType;
  }

  static OdResult
  setSubentityGeometry(OdDbEntity* pEntity, const OdDbSubentId& subentId, const OdDbEntity* pNewSubentityGeometry)
  {
    if (isValid(pEntity) && isValid(pNewSubentityGeometry) && subentId == textInsertionSubent())
    {
      OdGePoint3d vert = getPosition(static_cast<const T&>(*pNewSubentityGeometry));
      setPosition(static_cast<T&>(*pEntity), vert);
      return eOk;
    }
    return eBadObjType;
  }
};

OdDbAssocPersSubentIdPtr OdDbAssocTextPersSubentIdPE::createNewPersSubent(OdDbEntity* pEntity,
                                                                          const OdDbCompoundObjectId& compId,
                                                                          const OdDbSubentId& subentId)
{
  return OdDbImpAssocTextPersSubentIdPE<OdDbText>::createNewPersSubent(pEntity, compId, subentId);
}

OdResult OdDbAssocTextPersSubentIdPE::getAllSubentities(const OdDbEntity* pEntity,
                                                        OdDb::SubentType subentType,
                                                        OdArray<OdDbSubentId>& allSubentIds)
{
  return OdDbImpAssocTextPersSubentIdPE<OdDbText>::getAllSubentities(pEntity, subentType, allSubentIds);
}

OdResult OdDbAssocTextPersSubentIdPE::getVertexSubentityGeometry(const OdDbEntity* pEntity,
                                                                 const OdDbSubentId& vertexSubentId,
                                                                 OdGePoint3d& vertexPosition)
{
  return OdDbImpAssocTextPersSubentIdPE<OdDbText>::getVertexSubentityGeometry(pEntity, vertexSubentId, vertexPosition);
}

OdResult OdDbAssocTextPersSubentIdPE::setVertexSubentityGeometry(OdDbEntity* pEntity,
                                                                 const OdDbSubentId& vertexSubentId,
                                                                 const OdGePoint3d& newVertexPosition)
{
  return OdDbImpAssocTextPersSubentIdPE<OdDbText>::setVertexSubentityGeometry(pEntity, vertexSubentId,
                                                                              newVertexPosition);
}

OdResult OdDbAssocTextPersSubentIdPE::setSubentityGeometry(OdDbEntity* pEntity,
                                                           const OdDbSubentId& subentId,
                                                           const OdDbEntity* pNewSubentityGeometry)
{
  return OdDbImpAssocTextPersSubentIdPE<OdDbText>::setSubentityGeometry(pEntity, subentId, pNewSubentityGeometry);
}

OdResult OdDbAssocTextPersSubentIdPE::getTransientSubentIds(const OdDbEntity* pEntity,
                                                            const OdDbAssocPersSubentId* pPerSubentId,
                                                            OdArray<OdDbSubentId>& subents) const
{
  return OdDbAssocPointBasedPersSubentIdPE::getTransientSubentIds(pEntity, pPerSubentId, subents);
}

OdDbAssocPersSubentIdPtr OdDbAssocMTextPersSubentIdPE::createNewPersSubent(OdDbEntity* pEntity,
                                                                           const OdDbCompoundObjectId& compId,
                                                                           const OdDbSubentId& subentId)
{
  return OdDbImpAssocTextPersSubentIdPE<OdDbMText>::createNewPersSubent(pEntity, compId, subentId);
}

OdResult OdDbAssocMTextPersSubentIdPE::getAllSubentities(const OdDbEntity* pEntity,
                                                         OdDb::SubentType subentType,
                                                         OdArray<OdDbSubentId>& allSubentIds)
{
  return OdDbImpAssocTextPersSubentIdPE<OdDbMText>::getAllSubentities(pEntity, subentType, allSubentIds);
}

OdResult OdDbAssocMTextPersSubentIdPE::getVertexSubentityGeometry(const OdDbEntity* pEntity,
                                                                  const OdDbSubentId& vertexSubentId,
                                                                  OdGePoint3d& vertexPosition)
{
  return OdDbImpAssocTextPersSubentIdPE<OdDbMText>::getVertexSubentityGeometry(pEntity, vertexSubentId, vertexPosition);
}

OdResult OdDbAssocMTextPersSubentIdPE::setVertexSubentityGeometry(OdDbEntity* pEntity,
                                                                  const OdDbSubentId& vertexSubentId,
                                                                  const OdGePoint3d& newVertexPosition)
{
  return OdDbImpAssocTextPersSubentIdPE<OdDbMText>::setVertexSubentityGeometry(pEntity, vertexSubentId,
                                                                               newVertexPosition);
}

OdResult OdDbAssocMTextPersSubentIdPE::setSubentityGeometry(OdDbEntity* pEntity,
                                                            const OdDbSubentId& subentId,
                                                            const OdDbEntity* pNewSubentityGeometry)
{
  return OdDbImpAssocTextPersSubentIdPE<OdDbMText>::setSubentityGeometry(pEntity, subentId, pNewSubentityGeometry);
}

OdResult OdDbAssocMTextPersSubentIdPE::getTransientSubentIds(const OdDbEntity* pEntity,
                                                             const OdDbAssocPersSubentId* pPerSubentId,
                                                             OdArray<OdDbSubentId>& subents) const
{
  return OdDbAssocPointBasedPersSubentIdPE::getTransientSubentIds(pEntity, pPerSubentId, subents);
}
