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
// Xml rendition Id registrator implementation

#ifndef ODTRXMLREGISTRATORIMPL
#define ODTRXMLREGISTRATORIMPL

#include "TD_PackPush.h"

#include "TrVisRendition.h"
#include "TrVisUniqueId.h"

#define STL_USING_MAP
#define STL_USING_SET
#include "OdaSTL.h"

/** \details
    <group ExRender_Windows_Classes> 
*/
class OdTrXmlRegistratorImpl : public OdTrVisRendition::IdRegistrator
{
  protected:
    typedef std::map<OdTrVisId, OdTrVisUniqueID> RegMap;
    mutable RegMap m_regMap[OdTrVisRendition::kNGenericDataTypes];
    mutable OdTrVisUniqueID m_tmpId;
    struct RegType
    {
      OdUInt64 m_entryId;
      OdUInt32 m_entryType;

      RegType(const OdTrVisUniqueID& id)
        : m_entryId(id.getId())
        , m_entryType(id.getType())
      {}

      bool operator <(const RegType &r2) const
      {
        if (m_entryType != r2.m_entryType)
          return m_entryType < r2.m_entryType;
        return m_entryId < r2.m_entryId;
      }

      OdTrVisUniqueID asUID() const
      {
        return OdTrVisUniqueID(m_entryId, m_entryType);
      }
    };
    typedef std::set<OdUInt64> IdSet;
    struct RegLink
    {
      OdUInt64 m_nLowFreeId;
      IdSet m_linkIds;
      RegLink()
        : m_nLowFreeId(0)
      {
      }
      bool isEmpty() const { return m_linkIds.empty(); }
    };
    typedef std::map<RegType, RegLink> LinkMap;
    mutable LinkMap m_linkMap[OdTrVisRendition::kNGenericDataTypes];
    inline OdUInt32 genType(OdTrVisRendition::DataType type) const
    { return OdTrVisRendition::MtComplex::toGenericDataType(type); }
  public:
    struct Traverser
    {
      virtual bool idRegTraverseCallback(OdTrVisRendition::DataType type, OdTrVisId id, OdTrVisUniqueID *pRegId) = 0;
    };
  public:
    OdTrXmlRegistratorImpl();
    ~OdTrXmlRegistratorImpl();

    // Registrator methods

    virtual void reg(OdTrVisRendition::DataType type, OdTrVisId id, OdDbStub *pPersId);
    virtual void reg(OdTrVisRendition::DataType type, OdTrVisId id, const OdGiDrawable *pTransId);
    virtual void reg(OdTrVisRendition::DataType type, OdTrVisId id, const OdTrVisUniqueID &uId);
    virtual void kill(OdTrVisRendition::DataType type, OdTrVisId id);
    virtual OdTrVisUniqueID *get(OdTrVisRendition::DataType type, OdTrVisId id, bool bKill = false) const;

    void killImpl(OdTrVisRendition::DataType type, OdTrVisId id) const;
    void killAll();

    void traverse(Traverser *pTraverser);

    bool save(OdGsFiler *pFiler) const;
    bool load(OdGsFiler *pFiler, OdTrVisIdMap *pIdMap);
};

#include "TD_PackPop.h"

#endif // ODTRXMLREGISTRATORIMPL
