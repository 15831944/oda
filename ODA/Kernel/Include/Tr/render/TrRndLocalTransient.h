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
// Standalone renderer transient metafiles manager

#ifndef ODTRRNDLOCALTRANSIENT
#define ODTRRNDLOCALTRANSIENT

#include "TD_PackPush.h"

#include "TrRndLocalRenderer.h"
#include "../TrVisMetafileBuilder.h"

// Forward declarations
class OdTrRndLocalTransientManager;

/** \details
    <group ExRender_Windows_Classes>
*/
class OdTrRndLocalTransient : public OdRxObject
{
  protected:
    enum Flags
    {
      kInvalid         = 1,
      kMViewCompatible = 2,
      kAutoRegen       = 4
    };
    OdUInt16 m_flags, m_counter;

    friend class OdTrRndLocalTransientManagerImpl;
    void setInvalid() { SETBIT_1(m_flags, kInvalid); m_counter = 0; }
    void setValid() { SETBIT_0(m_flags, kInvalid); m_counter = 0; }
    bool isValid() const { return !GETBIT(m_flags, kInvalid); }

    void setMultiViewCompatible(bool bSet) { SETBIT(m_flags, kMViewCompatible, bSet); }
    bool isMultiViewCompatible() const { return GETBIT(m_flags, kMViewCompatible); }

    void setAutoRegen(bool bSet) { SETBIT(m_flags, kAutoRegen, bSet); }
    bool isAutoRegen() const { return GETBIT(m_flags, kAutoRegen); }

    typedef std::map<OdTrVisViewportId, OdRxObjectPtr> MetaDataMap;
    MetaDataMap m_metaData;
    OdTrRndLocalTransientManager *m_pOwner;
  public:
    OdTrRndLocalTransient()
      : m_flags(kInvalid)
      , m_counter(0)
      , m_pOwner(NULL)
    { }
    virtual ~OdTrRndLocalTransient() { }

    OdTrRndLocalTransientManager *owner() const { return m_pOwner; }

    void invalidate();
    void invalidate(OdTrVisViewportId vpId);

    bool isAttached(OdTrVisViewportId vpId) const;

    OdRxObject *viewportData(OdTrVisViewportId id);

    virtual void draw(OdTrVisMetafileBuilderPtr mfBuilder, const OdTrRndLocalRenderer* pRenderer) = 0;

    typedef OdSmartPtr<OdTrRndLocalTransient> Ptr;
};

/** \details
    <group ExRender_Windows_Classes>
*/
class OdTrRndLocalTransientManager
{
public:
  virtual ~OdTrRndLocalTransientManager() {}
  virtual void attachTransient(OdTrVisViewportId, OdTrRndLocalTransient *) {}
  virtual void detachTransient(OdTrVisViewportId, OdTrRndLocalTransient *) {}
  virtual void detachTransient(OdTrRndLocalTransient *) {}

  virtual void detachAll(OdTrVisViewportId ) {}
  virtual void detachAll() {}

  virtual void invalidateTransient(OdTrRndLocalTransient *) {}
  virtual void invalidateTransient(OdTrVisViewportId, OdTrRndLocalTransient *) {}
  virtual void invalidate(OdTrVisViewportId vpId) {}
  virtual void invalidate() {}

  virtual bool update(OdTrRndLocalRenderer *) { return false; }
  virtual void getExtents(OdTrVisViewportId, OdGeExtents3d &) {}

  virtual void draw(const OdTrRndLocalRenderer *, OdTrVisViewportId) {}
  virtual bool isDrawing() const { return false; }
};

// OdTrRndLocalTransient inlines section

inline
void OdTrRndLocalTransient::invalidate()
{
  if (m_pOwner)
    m_pOwner->invalidateTransient(this);
}

inline
void OdTrRndLocalTransient::invalidate(OdTrVisViewportId vpId)
{
  if (m_pOwner)
    m_pOwner->invalidateTransient(vpId, this);
}

inline
bool OdTrRndLocalTransient::isAttached(OdTrVisViewportId vpId) const
{
  return m_metaData.find(vpId) != m_metaData.end();
}

inline
OdRxObject *OdTrRndLocalTransient::viewportData(OdTrVisViewportId id)
{
  MetaDataMap::iterator it = m_metaData.find(id);
  if (it != m_metaData.end())
    return it->second.get();
  return NULL;
}

#include "TD_PackPop.h"

#endif // ODTRRNDLOCALTRANSIENT
