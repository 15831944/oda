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

#ifndef OdDbAssocTextPersSubentIdPE_INCLUDED_
#define OdDbAssocTextPersSubentIdPE_INCLUDED_  /*!DOM*/

#include "OdDbAssocPointPersSubentIdPE.h"

#include "TD_PackPush.h"

/** \details
This class provides the base protocol extension class that declares the protocol for
working with OdDbText persistent subentity identifier.

Library: Source code provided.
<group Contraint_Classes>
*/
class DBCONSTRAINTS_EXPORT OdDbAssocTextPersSubentIdPE : public OdDbAssocPointBasedPersSubentIdPE
{
public:
  ODRX_DECLARE_MEMBERS(OdDbAssocTextPersSubentIdPE);

  /** \details
  Creates new persistent subentity identifier.
  */
  OdDbAssocPersSubentIdPtr createNewPersSubent(OdDbEntity* pEntity,
                                               const class OdDbCompoundObjectId& compId,
                                               const OdDbSubentId& subentId) ODRX_OVERRIDE;

  /** \details
  Returns the transient subent ID of the dependent-on entity corresponding to the internally
  held associated subentity.
  */
  OdResult getTransientSubentIds(const OdDbEntity* pEntity,
                                 const OdDbAssocPersSubentId* pPerSubentId,
                                 OdArray<OdDbSubentId>& subents) const ODRX_OVERRIDE;

  /** \details
  Gets all subentity identifiers for a given subentity type.
  */
  OdResult getAllSubentities(const OdDbEntity* pEntity,
                             OdDb::SubentType subentType,
                             OdArray<OdDbSubentId>& allSubentIds) ODRX_OVERRIDE;

  /** \details
  Gets the coordinates of a vertex subentity corresponding to the associated subentity ID.
  */
  OdResult getVertexSubentityGeometry(const OdDbEntity* pEntity,
                                      const OdDbSubentId& vertexSubentId,
                                      OdGePoint3d& vertexPosition) ODRX_OVERRIDE;

  /** \details
  Sets the coordinates of a vertex subentity corresponding to the associated subentity ID.
  */
  OdResult setVertexSubentityGeometry(OdDbEntity* pEntity,
                                      const OdDbSubentId& vertexSubentId,
                                      const OdGePoint3d& newVertexPosition) ODRX_OVERRIDE;

  /** \details
  Modifiers the geometry of a subentity.
  */
  OdResult setSubentityGeometry(OdDbEntity* pEntity,
                                const OdDbSubentId& subentId,
                                const OdDbEntity* pNewSubentityGeometry) ODRX_OVERRIDE;
};

typedef OdSmartPtr<OdDbAssocTextPersSubentIdPE> OdDbAssocTextPersSubentIdPEPtr;

/** \details
This class provides the base protocol extension class that declares the protocol for
working with OdDbMText persistent subentity identifier.

Library: Source code provided.
<group Contraint_Classes>
*/
class DBCONSTRAINTS_EXPORT OdDbAssocMTextPersSubentIdPE : public OdDbAssocPointBasedPersSubentIdPE
{
public:
  ODRX_DECLARE_MEMBERS(OdDbAssocMTextPersSubentIdPE);

  /** \details
  Creates new persistent subentity identifier.
  */
  OdDbAssocPersSubentIdPtr createNewPersSubent(OdDbEntity* pEntity,
                                               const class OdDbCompoundObjectId& compId,
                                               const OdDbSubentId& subentId) ODRX_OVERRIDE;

  /** \details
  Returns the transient subent ID of the dependent-on entity corresponding to the internally
  held associated subentity.
  */
  OdResult getTransientSubentIds(const OdDbEntity* pEntity,
                                 const OdDbAssocPersSubentId* pPerSubentId,
                                 OdArray<OdDbSubentId>& subents) const ODRX_OVERRIDE;

  /** \details
  Gets all subentity identifiers for a given subentity type.
  */
  OdResult getAllSubentities(const OdDbEntity* pEntity,
                             OdDb::SubentType subentType,
                             OdArray<OdDbSubentId>& allSubentIds) ODRX_OVERRIDE;

  /** \details
  Gets the coordinates of a vertex subentity corresponding to the associated subentity ID.
  */
  OdResult getVertexSubentityGeometry(const OdDbEntity* pEntity,
                                      const OdDbSubentId& vertexSubentId,
                                      OdGePoint3d& vertexPosition) ODRX_OVERRIDE;

  /** \details
  Sets the coordinates of a vertex subentity corresponding to the associated subentity ID.
  */
  OdResult setVertexSubentityGeometry(OdDbEntity* pEntity,
                                      const OdDbSubentId& vertexSubentId,
                                      const OdGePoint3d& newVertexPosition) ODRX_OVERRIDE;

  /** \details
  Modifiers the geometry of a subentity.
  */
  OdResult setSubentityGeometry(OdDbEntity* pEntity,
                                const OdDbSubentId& subentId,
                                const OdDbEntity* pNewSubentityGeometry) ODRX_OVERRIDE;
};

typedef OdSmartPtr<OdDbAssocMTextPersSubentIdPE> OdDbAssocMTextPersSubentIdPEPtr;

#include "TD_PackPop.h"

#endif
