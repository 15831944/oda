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

#ifndef EX_H_METAFILETRANSFORMSTACK
#define EX_H_METAFILETRANSFORMSTACK

#include "TD_PackPush.h"

#include "DebugStuff.h"
#include "Ge/GeMatrix3d.h"
#include "OdStack.h"

/** \details
  This class provides metafile transformations stack functionality.
  Library: Source code provided.
  <group ExRender_Windows_Classes> 
*/
class MetafileTransformStack
{
  public:
    typedef bool (*IdentityCheckFunc)(const OdGeMatrix3d &mtx);
    static bool defIdentityCheckFull(const OdGeMatrix3d &mtx);
    static bool defIdentityCheckFast(const OdGeMatrix3d &mtx);
    static bool defIdentityCheckPtr(const OdGeMatrix3d &mtx);

    enum TopLevelMgdType
    {
      TopLevelMgdType_Exclusive = 0,
      TopLevelMgdType_Inclusive
    };
  protected:
    IdentityCheckFunc m_pIdentityCheckFunc;
    TopLevelMgdType m_tpLevelType;

    struct MTEntry
    {
      OdGeMatrix3d m_transform, m_transformExclusive;
      bool m_bIdentity, m_bIdentityExclusive;

      MTEntry() : m_bIdentity(true), m_bIdentityExclusive(true) { }
    };
    mutable MTEntry m_output;
    OdStack<MTEntry> m_stack;
  public:
    MetafileTransformStack(IdentityCheckFunc pIdentityCheckFunc = defIdentityCheckFull, TopLevelMgdType tpType = TopLevelMgdType_Exclusive)
      : m_pIdentityCheckFunc(pIdentityCheckFunc)
      , m_tpLevelType(tpType)
    {
    }

    bool isTopLevelMgdExclusive() const;

    void setOutputTransform(const OdGeMatrix3d &mtx);
    const OdGeMatrix3d &outputTransform() const;
    bool isOutputTransformIdentity() const;

    void pushMetafileTransform(const OdGeMatrix3d &mtx);
    void popMetafileTransform();
    const OdGeMatrix3d &metafileTransform() const;
    OdGeMatrix3d metafileTransformExclusive() const;
    const OdGeMatrix3d &inverseMetafileTransform(bool bExclusive = false) const;
    bool isMetafileTransformIdentity(bool bExclusive = false) const;
    OdGeMatrix3d getCompositeTransform() const;
    OdGeMatrix3d getInverseCompositeTransform() const;
    void checkMetafileTransform();
    bool hasStack() const;

    void clear(bool bStack = true, bool bReset = false);
};

inline bool MetafileTransformStack::defIdentityCheckFull(const OdGeMatrix3d &mtx)
{
  return mtx.isEqualTo(OdGeMatrix3d::kIdentity);
}

inline bool MetafileTransformStack::defIdentityCheckFast(const OdGeMatrix3d &mtx)
{
  return !::memcmp(&mtx, &OdGeMatrix3d::kIdentity, sizeof(OdGeMatrix3d));
}

inline bool MetafileTransformStack::defIdentityCheckPtr(const OdGeMatrix3d &mtx)
{
  return &mtx == &OdGeMatrix3d::kIdentity;
}

inline bool MetafileTransformStack::isTopLevelMgdExclusive() const
{
  return m_tpLevelType == TopLevelMgdType_Exclusive;
}

inline void MetafileTransformStack::setOutputTransform(const OdGeMatrix3d &mtx)
{
  m_output.m_transform = mtx;
  m_output.m_bIdentity = m_pIdentityCheckFunc(mtx);
}

inline const OdGeMatrix3d &MetafileTransformStack::outputTransform() const
{
  return m_output.m_transform;
}

inline bool MetafileTransformStack::isOutputTransformIdentity() const
{
  return m_output.m_bIdentity;
}

inline void MetafileTransformStack::pushMetafileTransform(const OdGeMatrix3d &mtx)
{ const bool bStackEmpty = m_stack.empty();
  MTEntry *mt = m_stack.push();
  if (bStackEmpty || m_stack.beforeTop()->m_bIdentity)
  {
    if (isTopLevelMgdExclusive() || isOutputTransformIdentity())
    {
      mt->m_transform = mtx;
      mt->m_bIdentity = m_pIdentityCheckFunc(mtx);
    }
    else
    {
      mt->m_transform = m_output.m_transform * mtx;
      mt->m_bIdentity = m_pIdentityCheckFunc(mt->m_transform);
    }
  }
  else
  {
    mt->m_transform = m_stack.beforeTop()->m_transform * mtx;
    mt->m_bIdentity = m_pIdentityCheckFunc(mt->m_transform);
  }
  if (!isTopLevelMgdExclusive())
  {
    if (isOutputTransformIdentity())
      mt->m_transformExclusive = mt->m_transform, mt->m_bIdentityExclusive = mt->m_bIdentity;
    else if (bStackEmpty || m_stack.beforeTop()->m_bIdentityExclusive)
      mt->m_transformExclusive = mtx, mt->m_bIdentityExclusive = m_pIdentityCheckFunc(mtx);
    else
      mt->m_transformExclusive = m_stack.beforeTop()->m_transformExclusive * mtx,
      mt->m_bIdentityExclusive = m_pIdentityCheckFunc(mt->m_transformExclusive);
  }
  m_output.m_bIdentityExclusive = true; // Mark that inverse matrix transform is not computed.
}

inline void MetafileTransformStack::popMetafileTransform()
{
  m_stack.pop();
  m_output.m_bIdentityExclusive = true; // Mark that inverse matrix transform is not computed.
}

inline const OdGeMatrix3d &MetafileTransformStack::metafileTransform() const
{
  if (m_stack.empty())
  {
    if (isTopLevelMgdExclusive())
      return OdGeMatrix3d::kIdentity;
    return m_output.m_transform;
  }
  return m_stack.top()->m_transform;
}

inline OdGeMatrix3d MetafileTransformStack::metafileTransformExclusive() const
{
  if (m_stack.empty())
    return OdGeMatrix3d::kIdentity;
  if (isTopLevelMgdExclusive())
    return m_stack.top()->m_transform;
  else
    return m_stack.top()->m_transformExclusive;
    //return m_output.m_transform.inverse() * m_stack.top()->m_transform;
}

inline const OdGeMatrix3d &MetafileTransformStack::inverseMetafileTransform(bool bExclusive) const
{
  if (m_output.m_bIdentityExclusive)
  {
    if (m_stack.empty())
    {
      if (isTopLevelMgdExclusive() || bExclusive)
        m_output.m_transformExclusive = OdGeMatrix3d::kIdentity;
      else
        m_output.m_transformExclusive = m_output.m_transform.inverse();
    }
    else
    {
      if (!bExclusive || isOutputTransformIdentity())
        m_output.m_transformExclusive = m_stack.top()->m_transform.inverse();
      else if (bExclusive && !isTopLevelMgdExclusive())
        m_output.m_transformExclusive = m_stack.top()->m_transformExclusive.inverse();
      else
        m_output.m_transformExclusive = m_stack.top()->m_transform.inverse() * m_output.m_transform;
    }
    m_output.m_bIdentityExclusive = false;
  }
  return m_output.m_transformExclusive;
}

inline bool MetafileTransformStack::isMetafileTransformIdentity(bool bExclusive) const
{
  if (m_stack.empty())
  {
    if (isTopLevelMgdExclusive() || bExclusive)
      return true;
    return m_output.m_bIdentity;
  }
  if (isTopLevelMgdExclusive() || !bExclusive)
    return m_stack.top()->m_bIdentity;
  else if (bExclusive && !isTopLevelMgdExclusive())
    return m_stack.top()->m_bIdentityExclusive;
  return m_pIdentityCheckFunc(m_output.m_transform.inverse() * m_stack.top()->m_transform);
}

inline OdGeMatrix3d MetafileTransformStack::getCompositeTransform() const
{
  if (isMetafileTransformIdentity())
  {
    return m_output.m_transform;
  }
  else if (m_output.m_bIdentity)
  {
    return m_stack.top()->m_transform;
  }
  else
  {
    if (isTopLevelMgdExclusive())
      return m_output.m_transform * m_stack.top()->m_transform;
    return metafileTransform();
  }
}

inline OdGeMatrix3d MetafileTransformStack::getInverseCompositeTransform() const
{
  if (isMetafileTransformIdentity())
  {
    return m_output.m_transform;
  }
  else if (m_output.m_bIdentity)
  {
    return m_stack.top()->m_transform;
  }
  else
  {
    if (isTopLevelMgdExclusive())
      return m_stack.top()->m_transform * m_output.m_transform;
    return m_output.m_transform.inverse() * m_stack.top()->m_transform * m_output.m_transform;
  }
}

inline void MetafileTransformStack::checkMetafileTransform()
{
  ODA_ASSERT(m_stack.empty());
}

inline bool MetafileTransformStack::hasStack() const
{
  return !m_stack.empty();
}

inline void MetafileTransformStack::clear(bool bStack, bool bReset)
{
  if (bStack)
  {
    while (m_stack.top())
      m_stack.pop();
  }
  if (bReset)
    setOutputTransform(OdGeMatrix3d::kIdentity);
}

#include "TD_PackPop.h"

#endif // EX_H_METAFILETRANSFORMSTACK
