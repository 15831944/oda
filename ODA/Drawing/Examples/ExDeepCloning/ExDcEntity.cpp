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
/* ExDcEntity.cpp: implementation of the ExDcEntity class               */
/************************************************************************/
#include "OdaCommon.h"
#include "ExDcEntity.h"
#include "DbDatabase.h"
#include "DbProxyEntity.h"
#include "DbFiler.h"
#include "DbIdMapping.h"
#include "CloneFilers.h"
#include "DbBlockTableRecord.h"

ODRX_DXF_DEFINE_MEMBERS(ExDcEntity,                                                       // ClassName
                        OdDbCircle,                                                       // ParentClass
                        DBOBJECT_CONSTR,                                                  // DOCREATE
                        OdDb::vAC15,                                                      // DwgVer
                        OdDb::kMRelease0,                                                 // MaintVer
                        OdDbProxyEntity::kAllAllowedBits,                                 // NProxyFlags
                        EXDCENTITY,                                                       // DxfName
                        ExDeepCloning|Description: Teigha Run-time Extension Example)  // AppName

const OdInt16 ExDcEntity::CurrentVersion = 1;

ExDcEntity::ExDcEntity()
{
}

ExDcEntity::~ExDcEntity()
{
}

/************************************************************************/
/* Overridden deep cloning methods.                                     */
/************************************************************************/
OdDbObjectPtr ExDcEntity::subDeepClone(OdDbIdMapping& idMap, OdDbObject*, bool bPrimary) const
{
  // You should always return NULL if, for any reason,
  // you do not actually clone this object during this call.
  //
  OdDbObjectPtr pClone;

  // Verify database pointer
  //
  if (!idMap.destDb())
  {
    throw OdError(eNoDatabase);
  }

  // If this object is in the idMap and is already cloned, then return.
  //
  OdDbIdPair idPair(objectId());
  if (idMap.compute(idPair) && (idPair.isCloned() || !idPair.value().isNull()))
  {
    return OdDbObjectPtr();
  }

  // Step 1
  // Create the clone and add it to the database.
  //
  pClone = isA()->create();
  idMap.destDb()->addOdDbObject(pClone);

  // Step 2
  // The OdDbDeepCloneFiler makes a list of HardOwnership and SoftOwnership IDs.
  // These references must be cloned during a deepclone operation.
  //
  OdDbDeepCloneFilerPtr pFiler = OdDbDeepCloneFiler::createObject(&idMap);
  dwgOut(pFiler);

  // Step 3
  // Rewind the filer and read the data into the clone.
  //
  pFiler->seek(0L, OdDb::kSeekFromStart);
  pClone->dwgIn(pFiler);

  // Step 4
  // Add the new information to the ID map.
  idPair.setValue(pClone->objectId());
  idPair.setCloned(true);
  idPair.setPrimary(bPrimary);
  idMap.assign(idPair);

  // Step 5
  // Set flag that the cloned object's IDs have not yet been translated
  // and thus cannot be used in any operations on the object.
  // The flag is turned off after the object has translated 
  // the references to their new values.
  //
  pClone->setOdDbObjectIdsInFlux();

  // Step 6
  // Using the filer list created above, clone any owned references.
  //
  pFiler->defaultProcessReferences();
  // Where default processing iterates through owned references
  // and call deepClone() method.
  //
  //OdDbObjectId hardId;
  //pFiler->start();
  //while (pFiler->nextReference(hardId))
  //{
  //  hardId.safeOpenObject()->deepClone(idMap);
  //}

  // Return the cloned object.
  //
  return pClone;
}

OdDbObjectPtr ExDcEntity::subWblockClone(OdDbIdMapping& idMap, OdDbObject* /*owner*/, bool bPrimary) const
{
  // do not use the owner - override appendToOwner() virtual function 
  // if you need some special ownership processing

  // You should always return NULL if, for any reason,
  // you do not actually clone this object during this call.
  //
  OdDbObjectPtr pClone;

  // Verify database pointer
  //
  if (!idMap.destDb())
  {
    throw OdError(eNoDatabase);
  }

  // If this is an xref bind operation and this entity is in paper space, then return,
  // as xref bind doesn't support cloning entities in paper space.
  //
  OdDbDatabase* pDb = database();
  if (idMap.deepCloneContext() == OdDb::kDcXrefBind && ownerId() == pDb->getPaperSpaceId())
  {
    return OdDbObjectPtr();
  }

  // If this object is in the idMap and is already cloned, then return.
  //
  OdDbIdPair idPair(objectId());
  if (idMap.compute(idPair) && (idPair.isCloned() || !idPair.value().isNull()))
  {
    return OdDbObjectPtr();
  }

  // Step 1
  // Create the clone and add it to the database.
  //
  pClone = isA()->create();
  idMap.destDb()->addOdDbObject(pClone);

  // Step 2
  // The OdDbWblockCloneFiler makes a list of HardOwnership and HardPointer IDs.
  // These references must be cloned during a wblock operation.
  //
  OdDbWblockCloneFilerPtr pFiler = OdDbWblockCloneFiler::createObject(&idMap);
  dwgOut(pFiler);

  // Step 3
  // Rewind the filer and read the data into the clone.
  //
  pFiler->seek(0L, OdDb::kSeekFromStart);
  pClone->dwgIn(pFiler);

  // Step 4
  // Add the new information to the ID map.
  idPair.setValue(pClone->objectId());
  idPair.setCloned(true);
  idPair.setPrimary(bPrimary);
  idMap.assign(idPair);

  // Step 5
  // Set flag that the cloned object's IDs have not yet been translated
  // and thus cannot be used in any operations on the object.
  // The flag is turned off after the object has translated 
  // the references to their new values.
  //
  pClone->setOdDbObjectIdsInFlux();

  // Step 6
  // Using the filer list created above, clone any hard references.
  //
  pFiler->defaultProcessReferences();
  // Where default processing iterates through hard references
  // and call wblockClone() method.
  //
  //OdDbObjectId hardId;
  //pFiler->start();
  //while (pFiler->nextReference(hardId))
  //{
  //  hardId.safeOpenObject()->wblockClone(idMap);
  //}

  // Return the cloned object.
  //
  return pClone;
}

/************************************************************************/
/* Overridden IO methods.                                               */
/************************************************************************/
OdResult ExDcEntity::dwgInFields(OdDbDwgFiler* pFiler)
{
  OdResult res = OdDbCircle::dwgInFields(pFiler);
  if (res != eOk)
  {
    return res;
  }

  int nVersion = pFiler->rdInt16();
  if (nVersion > CurrentVersion)
  {
    return eMakeMeProxy;
  }

  OdUInt32 nCount = pFiler->rdInt32();
  m_ownedIds.resize(nCount);
  for (unsigned i = 0; i < m_ownedIds.size(); ++i)
  {
    m_ownedIds[i]= pFiler->rdSoftOwnershipId();
  }
  m_refId = pFiler->rdHardPointerId();

  return eOk;
}

void ExDcEntity::dwgOutFields(OdDbDwgFiler* pFiler) const
{
  OdDbCircle::dwgOutFields(pFiler);

  pFiler->wrInt16(CurrentVersion);
  pFiler->wrInt32(m_ownedIds.size());
  for (unsigned i = 0; i < m_ownedIds.size(); ++i)
  {
    pFiler->wrSoftOwnershipId(m_ownedIds[i]);
  }
  pFiler->wrHardPointerId(m_refId);
}

OdResult ExDcEntity::dxfInFields(OdDbDxfFiler* pFiler)
{
  OdResult res = OdDbCircle::dxfInFields(pFiler);
  if (res != eOk)
    return res;
  if (!pFiler->atSubclassData(desc()->name()))
    return eBadDxfSequence;

  m_ownedIds.resize(0);
  m_refId = OdDbObjectId::kNull;

  while(!pFiler->atEOF())
  {
    int nCode = pFiler->nextItem();
    switch (nCode)
    {
    case 70:
      {
        int nVersion = pFiler->rdInt16();
        if (nVersion >  CurrentVersion)
        {
          return eMakeMeProxy;
        }
      }
      break;
    case 90:
      m_ownedIds.reserve(pFiler->rdInt32());
      break;
    case 350:
      m_ownedIds.push_back(pFiler->rdObjectId());
      break;
    case 340:
      m_refId = pFiler->rdObjectId();
      break;
    }
  }
  return eOk;
}

void ExDcEntity::dxfOutFields(OdDbDxfFiler* pFiler) const
{
  OdDbCircle::dxfOutFields(pFiler);
  pFiler->wrSubclassMarker(desc()->name());
  pFiler->wrInt16(70, CurrentVersion);
  pFiler->wrInt32(90, m_ownedIds.size());
  for (unsigned i = 0; i < m_ownedIds.size(); ++i)
  {
    pFiler->wrObjectId(350, m_ownedIds[i]);
  }
  pFiler->wrObjectIdOpt(340, m_refId);
}

/************************************************************************/
/* Overridden IO methods.                                               */
/************************************************************************/
OdDbObjectIdArray ExDcEntity::getOwnedIds() const
{
  assertReadEnabled();
  return m_ownedIds;
}

void ExDcEntity::setOwnedIds(const OdDbObjectIdArray& ownedIds)
{
  assertWriteEnabled();
  m_ownedIds = ownedIds;
}

void ExDcEntity::addOwnedId(const OdDbObjectId& ownedId)
{
  assertWriteEnabled();
  m_ownedIds.push_back(ownedId);
}

OdDbObjectId ExDcEntity::getRefId() const
{
  assertReadEnabled();
  return m_refId;
}

void ExDcEntity::setRefId(const OdDbObjectId& refId)
{
  assertWriteEnabled();
  m_refId = refId;
}
