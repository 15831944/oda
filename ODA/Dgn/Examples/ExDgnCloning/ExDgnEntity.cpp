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
/* ExDgnEntity.cpp: implementation of the ExDgnEntity class               */
/************************************************************************/
#include "OdaCommon.h"
#include "ExDgnEntity.h"
#include "DgDatabase.h"
//#include "DbProxyEntity.h"
#include "DgFiler.h"
#include "DgIdMapping.h"
#include "DgCloneFilers.h"
//#include "DgBlockTableRecord.h"

ODRX_NO_CONS_DEFINE_MEMBERS( ExDgnEntity, OdDgEllipse3d )


ExDgnEntity::ExDgnEntity()
{
}

ExDgnEntity::~ExDgnEntity()
{
}

/************************************************************************/
/* Overridden deep cloning methods.                                     */
/************************************************************************/
OdDgElementPtr ExDgnEntity::subDeepClone(OdDgIdMapping& idMap) const
{
  // You should always return NULL if, for any reason,
  // you do not actually clone this object during this call.
  //
  OdDgElementPtr pClone;

  // Verify database pointer
  //
  if (!idMap.destDb())
  {
    throw OdError(eNoDatabase);
  }

  // If this object is in the idMap and is already cloned, then return.
  //
  OdDgIdPair idPair(elementId());
  if (idMap.compute(idPair) && (idPair.isCloned() || !idPair.value().isNull()))
  {
    return OdDgElementPtr();
  }

  // Step 1
  // Create the clone and add it to the database.
  //
  pClone = isA()->create();
  idMap.destDb()->addOdDgElement( pClone);

  // Step 2
  // The OdDbDeepCloneFiler makes a list of HardOwnership and SoftOwnership IDs.
  // These references must be cloned during a deepclone operation.
  //
  OdDgDeepCloneFilerPtr pFiler = OdDgDeepCloneFiler::createObject(&idMap);
  dgnOut(pFiler);

  // Step 3
  // Rewind the filer and read the data into the clone.
  //
  pFiler->seek(0L, OdDb::kSeekFromStart);
  pClone->dgnIn(pFiler);

  // Step 4
  // Add the new information to the ID map.
  idPair.setValue(pClone->elementId());
  idPair.setCloned(true);
  idMap.assign(idPair);

  // Step 5
  // Set flag that the cloned object's IDs have not yet been translated
  // and thus cannot be used in any operations on the object.
  // The flag is turned off after the object has translated 
  // the references to their new values.
  //
  pClone->setOdDgElementIdsInFlux();

  // Step 6
  // Using the filer list created above, clone any owned references.
  //
  pFiler->defaultProcessReferences();
  // Where default processing iterates through owned references
  // and call deepClone() method.
  //
  //OdDgElement hardId;
  //pFiler->start();
  //while (pFiler->nextReference(hardId))
  //{
  //  hardId.safeOpenObject()->deepClone(idMap);
  //}

  // Return the cloned object.
  //
  return pClone;
}

//OdDgElementPtr ExDgnEntity::subWblockClone(OdDgIdMapping& idMap) const
//{
  // You should always return NULL if, for any reason,
  // you do not actually clone this object during this call.
  //
//  OdDgElementPtr pClone;
//
//  // Verify database pointer
//  //
//  if (!idMap.destDb())
//  {
//    throw OdError(eNoDatabase);
//  }
//
//  // If this is an xref bind operation and this entity is in paper space, then return,
//  // as xref bind doesn't support cloning entities in paper space.
//  //
//  OdDbDatabase* pDb = database();
//  if (idMap.deepCloneContext() == OdDb::kDcXrefBind && ownerId() == pDb->getPaperSpaceId())
//  {
//    return OdDbObjectPtr();
//  }
//
//  // If this object is in the idMap and is already cloned, then return.
//  //
//  OdDbIdPair idPair(objectId());
//  if (idMap.compute(idPair) && (idPair.isCloned() || !idPair.value().isNull()))
//  {
//    return OdDbObjectPtr();
//  }
//
//  // Step 1
//  // Create the clone and add it to the database.
//  //
//  pClone = isA()->create();
//  idMap.destDb()->addOdDbObject(pClone);
//
//  // Step 2
//  // The OdDbWblockCloneFiler makes a list of HardOwnership and HardPointer IDs.
//  // These references must be cloned during a wblock operation.
//  //
//  OdDbWblockCloneFilerPtr pFiler = OdDbWblockCloneFiler::createObject(&idMap);
//  dwgOut(pFiler);
//
//  // Step 3
//  // Rewind the filer and read the data into the clone.
//  //
//  pFiler->seek(0L, OdDb::kSeekFromStart);
//  pClone->dwgIn(pFiler);
//
//  // Step 4
//  // Add the new information to the ID map.
//  idPair.setValue(pClone->objectId());
//  idPair.setCloned(true);
//  idMap.assign(idPair);
//
//  // Step 5
//  // Set flag that the cloned object's IDs have not yet been translated
//  // and thus cannot be used in any operations on the object.
//  // The flag is turned off after the object has translated 
//  // the references to their new values.
//  //
//  pClone->setOdDgElementsInFlux();
//
//  // Step 6
//  // Using the filer list created above, clone any hard references.
//  //
//  pFiler->defaultProcessReferences();
//  // Where default processing iterates through hard references
//  // and call wblockClone() method.
//  //
//  //OdDgElement hardId;
//  //pFiler->start();
//  //while (pFiler->nextReference(hardId))
//  //{
//  //  hardId.safeOpenObject()->wblockClone(idMap);
//  //}
//
//  // Return the cloned object.
//  //
//  return pClone;
//}

/************************************************************************/
/* Overridden IO methods.                                               */
/************************************************************************/
OdResult ExDgnEntity::dgnInFields(OdDgFiler* pFiler)
{
  OdResult res = OdDgEllipse3d::dgnInFields(pFiler);
  if (res != eOk)
  {
    return res;
  }

  //int nVersion = pFiler->rdInt16();
  //if (nVersion > CurrentVersion)
  //{
  //  return eMakeMeProxy;
  //}

  //OdUInt32 nCount = pFiler->rdInt32();
  //m_ownedIds.resize(nCount);
  //for (unsigned i = 0; i < m_ownedIds.size(); ++i)
  //{
  //  m_ownedIds[i]= pFiler->rdSoftOwnershipId();
  //}
  //m_refId = pFiler->rdHardPointerId();

  return eOk;
}

void ExDgnEntity::dgnOutFields(OdDgFiler* pFiler) const
{
  OdDgEllipse3d::dgnOutFields(pFiler);

  //pFiler->wrInt16(CurrentVersion);
  //pFiler->wrInt32(m_ownedIds.size());
  //for (unsigned i = 0; i < m_ownedIds.size(); ++i)
  //{
  //  pFiler->wrSoftOwnershipId(m_ownedIds[i]);
  //}
  //pFiler->wrHardPointerId(m_refId);
}


/************************************************************************/
/* Overridden IO methods.                                               */
/************************************************************************/
OdDgElementIdArray ExDgnEntity::getOwnedIds() const
{
  assertReadEnabled();
  return m_ownedIds;
}

void ExDgnEntity::setOwnedIds(const OdDgElementIdArray& ownedIds)
{
  assertWriteEnabled();
  m_ownedIds = ownedIds;
}

void ExDgnEntity::addOwnedId(const OdDgElementId& ownedId)
{
  assertWriteEnabled();
  m_ownedIds.push_back(ownedId);
}

OdDgElementId ExDgnEntity::getRefId() const
{
  assertReadEnabled();
  return m_refId;
}

void ExDgnEntity::setRefId(const OdDgElementId& refId)
{
  assertWriteEnabled();
  m_refId = refId;
}
