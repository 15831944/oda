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

#ifndef __ODGSHIGHLIGHTDATA_H__
#define __ODGSHIGHLIGHTDATA_H__

#define STL_USING_SET
#include "OdaSTL.h"

#include "GsMarkerArray.h"
#include "Gi/GiDrawable.h"
#include "Gs.h"
#include "TPtr.h"

#include "TD_PackPush.h"

class OdGsStateBranch;
typedef TPtr<OdGsStateBranch> OdGsStateBranchPtr;
typedef OdArray<OdGsStateBranchPtr> OdGsStateBranchPtrArray;

typedef std::set<OdGsMarker> OdGsMarkerSet;

class OdGsStateBranchReactor;

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGsStateBranchReactor object pointers.
*/
typedef OdSmartPtr<OdGsStateBranchReactor> OdGsStateBranchReactorPtr;

/** \details
    This class is a service class to keep information about highlighted subentities and nested entities on OsGsEntityNode.

    Corresponding C++ library: TD_Gs

    <group OdGs_Classes> 
*/
class GS_TOOLKIT_EXPORT OdGsStateBranch
{
  protected:
    ODRX_HEAP_OPERATORS();

    OdRefCounter m_nRefCounter;        // shared ownership
    const void* m_pDrawableId;         // persistent id or transient drawable pointer (depends from flag)
    bool m_bPersistentId;              // true if m_pDrawableId stores OdDbStub pointer
    OdUInt32 m_nStyle;                 // Style index
    // @@@TODO: Add ability to set style index per selection marker.
    OdGsStateBranchReactorPtr m_pReactor; // OdGsStateBranch reactor
    OdGsMarkerSet  m_setMarkers;       // Set of markers 
    OdGsStateBranchPtrArray m_aChild;     // sorted array of nested entities
    // the sorting rule: first are transient entities with id 0, sorted by drw() value,
    // next are persistent entities with id != 0, sorted by id()
  public:
    OdGsStateBranch(const OdDbStub* pDrawableId)
      : m_nRefCounter(1)
      , m_pDrawableId(pDrawableId)
      , m_bPersistentId(true)
      , m_nStyle(0)
      , m_aChild(0, 1)
    {
    }
    OdGsStateBranch(const OdGiDrawable* pTransDrawable)
      : m_nRefCounter(1)
      , m_pDrawableId(pTransDrawable)
      , m_bPersistentId(false)
      , m_nStyle(0)
      , m_aChild(0, 1)
    {
    }

    static OdGsStateBranchPtr create(OdDbStub* pDrawableId, OdGsMarker marker);
    static OdGsStateBranchPtr create(OdGiDrawable* pTransDrawable, OdGsMarker marker);

    void addRef()
    {
      ++m_nRefCounter;
    }
    void release()
    {
      ODA_ASSERT((m_nRefCounter > 0));
      if (!(--m_nRefCounter))
        delete this;
    }

    // access to nested entities
    const OdGsStateBranchPtrArray& aChild() const { return m_aChild; }
    // access to subentities
    const OdGsMarkerSet& markers() const { return m_setMarkers; }
    OdUInt32 markersSize() const { return (OdUInt32)m_setMarkers.size(); }
    bool markersEmpty() const { return m_setMarkers.empty(); }

    // add/remove/find nested entities info
    OdGsStateBranch* addChild(OdDbStub* pDrawableId);
    OdGsStateBranch* addChild(const OdGiDrawable* pTransDrawable);
    void removeChild(const OdGsStateBranch* pChild);
    OdGsStateBranch* findChild(const OdDbStub* pDrawableId); // search in a sorted array, fast
    OdGsStateBranch* findChild(const OdGiDrawable* pTransDrawable); // search in a sorted array, fast

    // add/remove/find subentities info
    bool addMarker(OdGsMarker marker);
    bool removeMarker(OdGsMarker marker);
    bool hasMarker(OdGsMarker marker) const; // search in a sorted array, fast

    // access to the OdGsEntityNode data
    const OdDbStub* id() const
    {
      return (m_bPersistentId) ? reinterpret_cast<const OdDbStub*>(m_pDrawableId) : NULL;
    }
    const OdGiDrawable* drw() const
    {
      return (!m_bPersistentId) ? reinterpret_cast<const OdGiDrawable*>(m_pDrawableId) : NULL;
    }
    bool isEmpty() const
    {
      return m_setMarkers.empty() && aChild().isEmpty();
    }

    // access flags
    bool isPersistentId() const { return m_bPersistentId; }

    // reactor
    void setReactor(OdGsStateBranchReactor *pReactor);
    OdGsStateBranchReactor *reactor();

    // style
    void setStyle(OdUInt32 nStyle);
    OdUInt32 style() const { return m_nStyle; }
    bool hasStyle() const { return m_nStyle != 0; }

    static inline bool isValidMarker(OdGsMarker marker)
    {
      return (marker != kNullSubentIndex);
    }
};

/** \details
    This class is a reactor for OdGsStateBranch class.

    Corresponding C++ library: TD_Gs

    <group OdGs_Classes> 
*/
class GS_TOOLKIT_EXPORT OdGsStateBranchReactor : public OdRxObject
{
  public:
    virtual void onChildAdded(OdGsStateBranch *pHlBranch, OdGsStateBranch *pHlBranchAdded) = 0;
    virtual void onChildRemoved(OdGsStateBranch *pHlBranch, OdGsStateBranch *pHlBranchRemoved) = 0;
    virtual void onMarkerAdded(OdGsStateBranch *pHlBranch, OdGsMarker gsMarker) = 0;
    virtual void onMarkerRemoved(OdGsStateBranch *pHlBranch, OdGsMarker gsMarker) = 0;
    virtual void onStyleModified(OdGsStateBranch *pHlBranch, OdUInt32 nPrevStyle, OdUInt32 nStyle) = 0;
};

// gcc3x template instantiation must be done after class declaration
inline void OdGsStateBranch::setReactor(OdGsStateBranchReactor *pReactor) { m_pReactor = pReactor; }
inline OdGsStateBranchReactor *OdGsStateBranch::reactor() { return m_pReactor.get(); }

inline void OdGsStateBranch::setStyle(OdUInt32 nStyle)
{
  if (nStyle != m_nStyle)
  {
    const OdUInt32 nPrevStyle = m_nStyle;
    m_nStyle = nStyle;
    if (!m_pReactor.isNull())
      m_pReactor->onStyleModified(this, nPrevStyle, nStyle);
  }
}

/** \details
    This class is a multimodule reactor reactor implementation for OdGsStateBranch class.

    Corresponding C++ library: TD_Gs

    <group OdGs_Classes> 
*/
class GS_TOOLKIT_EXPORT OdGsStateBranchMultimoduleReactor
{
  public:
    static void attach(OdGsStateBranch *pHlBranch, const void *pModule, OdGsStateBranchReactor *pReactor);
    static void detach(OdGsStateBranch *pHlBranch, const void *pModule);
    static OdGsStateBranchReactor *getReactor(OdGsStateBranch *pHlBranch, const void *pModule);
    static void setReactor(OdGsStateBranch *pHlBranch, const void *pModule, OdGsStateBranchReactor *pReactor);
};

#include "TD_PackPop.h"

#endif // __ODGSHIGHLIGHTDATA_H__
