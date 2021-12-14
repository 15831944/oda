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
// GLES2 device rendition Id registrator implementation

#include "OdaCommon.h"
#include "Gs/GsFiler.h"
#include "TrVisIdMap.h"
#include "TrXmlIdRegistratorImpl.h"

#define DEBUG_IDREGISTRATOR

OdTrXmlRegistratorImpl::OdTrXmlRegistratorImpl()
{
}

OdTrXmlRegistratorImpl::~OdTrXmlRegistratorImpl()
{
  killAll();
}

// Registrator methods

void OdTrXmlRegistratorImpl::reg(OdTrVisRendition::DataType type, OdTrVisId id, OdDbStub *pPersId)
{
  if (pPersId)
  {
    OdTrVisUniqueID uId;
    uId.setId(pPersId); uId.setPersistent();
    uId.setData(m_linkMap[genType(type)][RegType(uId)].m_nLowFreeId);
    reg(type, id, uId);
  }
}

void OdTrXmlRegistratorImpl::reg(OdTrVisRendition::DataType type, OdTrVisId id, const OdGiDrawable *pTransId)
{
  OdTrVisUniqueID uId;
  uId.setId(pTransId); uId.setTransient();
  uId.setData(m_linkMap[genType(type)][RegType(uId)].m_nLowFreeId);
  reg(type, id, uId);
}

void OdTrXmlRegistratorImpl::reg(OdTrVisRendition::DataType type, OdTrVisId id, const OdTrVisUniqueID &uId)
{
  OdTrVisUniqueID &re = m_regMap[genType(type)][id];
#ifdef DEBUG_IDREGISTRATOR
  ODA_ASSERT_ONCE(!re.isValid());
#endif
  re = uId;
  // Reg data id
  RegLink &rl = m_linkMap[genType(type)][RegType(re)];
#ifdef DEBUG_IDREGISTRATOR
  ODA_ASSERT_ONCE(rl.m_linkIds.find(re.getData()) == rl.m_linkIds.end());
  ODA_ASSERT_ONCE(re.getData() >= rl.m_nLowFreeId);
#endif
  IdSet::iterator it = rl.m_linkIds.insert(re.getData()).first;
  if (re.getData() <= rl.m_nLowFreeId)
  {
    OdUInt64 counter = re.getData();
    do {
      if (counter == *it)
        counter++;
      else
        break;
      it++;
    } while (it != rl.m_linkIds.end());
    rl.m_nLowFreeId = counter;
  }
}

void OdTrXmlRegistratorImpl::killImpl(OdTrVisRendition::DataType type, OdTrVisId id) const
{ const OdUInt32 nGenType = genType(type);
  RegMap::iterator itReg = m_regMap[nGenType].find(id);
#ifdef DEBUG_IDREGISTRATOR
  ODA_ASSERT_ONCE(itReg != m_regMap[nGenType].end());
#endif
  if (itReg != m_regMap[nGenType].end())
  {
    LinkMap::iterator itLink = m_linkMap[nGenType].find(RegType(itReg->second));
    ODA_ASSERT_ONCE(itLink != m_linkMap[nGenType].end());
    if (itLink != m_linkMap[nGenType].end())
    {
      RegLink &rl = itLink->second;
      rl.m_linkIds.erase(itReg->second.getData());
      if (rl.isEmpty())
        m_linkMap[nGenType].erase(itLink);
      else
      {
        if (itReg->second.getData() < rl.m_nLowFreeId)
          rl.m_nLowFreeId = itReg->second.getData();
      }
    }
    m_regMap[nGenType].erase(itReg);
  }
}

void OdTrXmlRegistratorImpl::kill(OdTrVisRendition::DataType type, OdTrVisId id)
{
  killImpl(type, id);
}

OdTrVisUniqueID *OdTrXmlRegistratorImpl::get(OdTrVisRendition::DataType type, OdTrVisId id, bool bKill) const
{
  if (!id)
    return NULL;
  const OdUInt32 nGenType = genType(type);
  RegMap::iterator it = m_regMap[nGenType].find(id);
  if (it != m_regMap[nGenType].end())
  {
    if (!bKill)
      return &it->second;
    m_tmpId = it->second;
    killImpl(type, id);
    return &m_tmpId;
  }
  return NULL;
}

void OdTrXmlRegistratorImpl::killAll()
{
  for (OdUInt32 nDataType = 0; nDataType < OdTrVisRendition::kNGenericDataTypes; nDataType++)
  {
    m_regMap[nDataType].clear();
    m_linkMap[nDataType].clear();
  }
}

void OdTrXmlRegistratorImpl::traverse(Traverser *pTraverser)
{
  if (!pTraverser) return;
  for (OdUInt32 nDataType = 0; nDataType < OdTrVisRendition::kNGenericDataTypes; nDataType++)
  {
    RegMap::iterator it = m_regMap[nDataType].begin();
    while (it != m_regMap[nDataType].end())
    {
      if (!pTraverser->idRegTraverseCallback(OdTrVisRendition::MtComplex::fromGenericDataType(nDataType), it->first, &it->second))
        return;
      it++;
    }
  }
}

bool OdTrXmlRegistratorImpl::save(OdGsFiler *pFiler) const
{
  // Save only persistent and utilitary id's
  for (OdUInt32 nDataType = 0; nDataType < OdTrVisRendition::kNGenericDataTypes; nDataType++)
  {
    RegMap::const_iterator itReg = m_regMap[nDataType].begin();
    while (itReg != m_regMap[nDataType].end())
    {
      if (!itReg->second.isTransient())
      {
        pFiler->wrBool(true);
        pFiler->wrUInt64((OdUInt64)itReg->first);
        pFiler->wrUInt32(itReg->second.getType());
        if (itReg->second.isPersistent())
          pFiler->wrHandle(itReg->second.persistentId());
        else
          pFiler->wrUInt64(itReg->second.getId());
        pFiler->wrUInt64(itReg->second.getData());
      }
      itReg++;
    }
    pFiler->wrBool(false);
    LinkMap::const_iterator itLink = m_linkMap[nDataType].begin();
    while (itLink != m_linkMap[nDataType].end())
    {
      OdTrVisUniqueID uId = itLink->first.asUID();
      if (!uId.isTransient())
      {
        pFiler->wrBool(true);
        pFiler->wrUInt32(uId.getType());
        if (uId.isPersistent())
          pFiler->wrHandle(uId.persistentId());
        else
          pFiler->wrUInt64(uId.getId());
        pFiler->wrUInt64(itLink->second.m_nLowFreeId);
        pFiler->wrUInt32((OdUInt32)itLink->second.m_linkIds.size());
        IdSet::const_iterator itSet = itLink->second.m_linkIds.begin();
        while (itSet != itLink->second.m_linkIds.end())
        {
          pFiler->wrUInt64(*itSet);
          itSet++;
        }
      }
      itLink++;
    }
    pFiler->wrBool(false);
  }
  return true;
}

bool OdTrXmlRegistratorImpl::load(OdGsFiler *pFiler, OdTrVisIdMap *pIdMap)
{
  for (OdUInt32 nDataType = 0; nDataType < OdTrVisRendition::kNGenericDataTypes; nDataType++)
  {
    while (pFiler->rdBool())
    {
      OdTrVisId id = (OdTrVisId)pFiler->rdUInt64();
      pIdMap->get(OdTrVisRendition::MtComplex::fromGenericDataType(nDataType), id, id);
      OdTrVisUniqueID uID;
      uID.setUtilitary(pFiler->rdUInt32());
      if (uID.isPersistent())
        uID.setId(pFiler->rdHandle());
      else
        uID.setId(pFiler->rdUInt64());
      uID.setData(pFiler->rdUInt64());
      m_regMap[nDataType][id] = uID;
    }
    while (pFiler->rdBool())
    {
      OdTrVisUniqueID uID;
      uID.setUtilitary(pFiler->rdUInt32());
      if (uID.isPersistent())
        uID.setId(pFiler->rdHandle());
      else
        uID.setId(pFiler->rdUInt64());
      RegLink &rl = m_linkMap[nDataType][RegType(uID)];
      rl.m_nLowFreeId = pFiler->rdUInt64();
      const OdUInt32 nSets = pFiler->rdUInt32();
      for (OdUInt32 nSet = 0; nSet < nSets; nSet++)
        rl.m_linkIds.insert(pFiler->rdUInt64());
    }
  }
  return true;
}

//
