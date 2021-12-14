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
// GLES2 dynamic unique ID mapping declaration

#ifndef ODTRVISUNIQUEIDMAPPING
#define ODTRVISUNIQUEIDMAPPING

#include "TD_PackPush.h"

#include "TrVisRendition.h"
#include "TrVisUniqueId.h"

#define STL_USING_MAP
#include "OdaSTL.h"

/** \details
    <group ExRender_Windows_Classes>
*/
template<class T>
class OdTrVisUniqueIDMappingImpl
{
  typedef std::map<T, OdTrVisId> IdMap;
  mutable struct IdMapEntry
  {
    IdMap m_idMap;
    OdTrVisId m_idMapCounter;
    void clear()
    {
      m_idMap.clear();
      m_idMapCounter = 101;
    }
    IdMapEntry() : m_idMapCounter(101) { }
  } m_idMapEntry[OdTrVisRendition::kNGenericDataTypes];
  inline OdUInt32 genType(OdTrVisRendition::DataType type) const
  { return OdTrVisRendition::MtComplex::toGenericDataType(type); }
  protected:
    OdTrVisId inc(OdTrVisId &counter, OdTrVisRendition::DataType type) const
    {
      if (counter == kTrVisNegativeId)
      {
        OdTrVisId id = 101;
        do
        {
          if (!findUID(id, type))
            return id;
          id++;
        } while (id < kTrVisNegativeId);
        counter = 101;
      }
      return counter++;
    }
  public:
    OdTrVisUniqueIDMappingImpl() { }

    void clean(OdTrVisRendition::DataType type = OdTrVisRendition::kNDataTypes)
    {
      const OdUInt32 nFrom = (type != OdTrVisRendition::kNDataTypes) ? genType(type) : 0;
      const OdUInt32 nTo = (type != OdTrVisRendition::kNDataTypes) ? (genType(type) + 1) : OdTrVisRendition::kNGenericDataTypes;
      for (OdUInt32 nType = nFrom; nType < nTo; nType++)
        m_idMapEntry[nType].clear();
    }

    OdTrVisId getTrVisId(const T& uid, OdTrVisRendition::DataType type) const
    { IdMapEntry &entry = m_idMapEntry[genType(type)];
      OD_TYPENAME IdMap::const_iterator it = entry.m_idMap.find(uid);
      if (it != entry.m_idMap.end())
        return it->second;
      entry.m_idMap[uid] = entry.m_idMapCounter;
      return inc(entry.m_idMapCounter, type);
    }

    void killTrVisId(const T& uid, OdTrVisRendition::DataType type)
    { IdMapEntry &entry = m_idMapEntry[genType(type)];
      OD_TYPENAME IdMap::iterator it = entry.m_idMap.find(uid);
      if (it != entry.m_idMap.end())
        entry.m_idMap.erase(it);
    }

    const T* findUID(OdTrVisId id, OdTrVisRendition::DataType type) const
    { const IdMapEntry &entry = m_idMapEntry[genType(type)];
      OD_TYPENAME IdMap::const_iterator it = entry.m_idMap.begin();
      while (it != entry.m_idMap.end())
      {
        if (it->second == id)
          return &it->first;
        it++;
      }
      return NULL;
    }

    bool hasUID(const T& uid, OdTrVisRendition::DataType type) const
    { const IdMapEntry &entry = m_idMapEntry[genType(type)];
      OD_TYPENAME IdMap::const_iterator it = entry.m_idMap.find(uid);
      return it != entry.m_idMap.end();
    }

    void setSpecialId(const T& uid, OdTrVisId id, OdTrVisRendition::DataType type)
    {
      m_idMapEntry[genType(type)].m_idMap[uid] = id;
    }

    static OdAnsiString asString(const OdTrVisUniqueID &id) { return id.transform(); }
    static OdTrVisUniqueID asUID(const OdAnsiString &str) { return OdTrVisUniqueID().untransform(str); }
};

/** \details
    <group ExRender_Windows_Classes>
*/
typedef OdTrVisUniqueIDMappingImpl<OdTrVisUniqueID> OdTrVisUniqueIDMapping;

/** \details
    <group ExRender_Windows_Classes>
*/
typedef OdTrVisUniqueIDMappingImpl<OdAnsiString> OdTrVisUniqueIDMappingStr;

/*
static bool chk()
{
  OdTrVisUniqueIDMapping idMap;
  OdTrVisUniqueID uid(22, 0, 707);
  OdTrVisId id = idMap.getTrVisId(uid, OdTrVisRendition::kMetafileData);
  // id == 100
  id = idMap.getTrVisId(uid, OdTrVisRendition::kMetafileData);
  // check back
  OdTrVisUniqueID chk = *idMap.findUID(id, OdTrVisRendition::kMetafileData);
  // For string
  OdTrVisUniqueIDMappingStr idMap2;
  id = idMap2.getTrVisId(idMap2.asString(uid), OdTrVisRendition::kMetafileData);
  // id == 100
  id = idMap2.getTrVisId(idMap2.asString(uid), OdTrVisRendition::kMetafileData);
  // check back
  chk = idMap2.asUID(*idMap2.findUID(id, OdTrVisRendition::kMetafileData));
  return (chk == uid);
}
*/

#include "TD_PackPop.h"

#endif // ODTRVISUNIQUEIDMAPPING
