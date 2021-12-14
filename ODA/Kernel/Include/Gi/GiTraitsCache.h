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

#ifndef _ODGITRAITSCACHE_INCLUDED_
#define _ODGITRAITSCACHE_INCLUDED_

#include "GiCommonDraw.h"

#define STL_USING_MAP
#include "OdaSTL.h"

class OdDbStub;

#include "TD_PackPush.h"

/** \details
    
    \sa
    TD_Gi
   
    <group OdGi_Classes>
*/
template <class traitsContainer, class traitsTaker, OdUInt32 byMask = 0>
class OdGiTraitsCache : public traitsTaker
{
  public:
    /** \details
      Represents cache types.
    */
    enum CacheType
    {
      /** No caching will be performed.*/
      kCacheNone   = 0,
      /** Cache traits by mask.*/
      kCacheByMask = 1,
      /** Cache all traits.*/
      kCacheAll    = 2
    };
  protected:
    struct CacheEntry
    {
      traitsContainer m_data;
      OdUInt32        m_flags;
      void set(const traitsContainer& data, OdUInt32 flags) { m_data = data; m_flags = flags; }
    };
   
  protected:
    CacheType           m_cacheType;
    typedef std::map<OdDbStub*, CacheEntry> _mmap;
    _mmap m_cacheData;
    OdUInt32            m_traitsAttrib;
  public:
    /** \details
      Default constructor for the OdGiTraitsCache class. Initializes parent object properties with default values,
      specifies that no caching will be performed and sets traits attributes to 0.
    */
    OdGiTraitsCache()
      : traitsTaker()
      , m_cacheType(kCacheNone)
      , m_traitsAttrib(0)
    {
    }
    /** \details
      Constructor for the OdGiTraitsCache class. Initializes parent object properties with default values,
      sets the passed traits type and sets traits attributes to 0.
    */
    explicit OdGiTraitsCache(CacheType cacheType)
      : traitsTaker()
      , m_cacheType(cacheType)
      , m_traitsAttrib(0)
    {
    }
    /** \details
      Destructor for the OdGiTraitsCache class.
    */
    ~OdGiTraitsCache()
    {
    }

    /** \details
      Sets the cache type for caching. This method allows you to specify whether all traits are cached or specific traits are 
      cached by mask or traits are not cached at all.
      \param cacheType [in]   Cache type.
    */
    void setCacheType(CacheType cacheType)
    {
      m_cacheType = cacheType;
    }
    
    /** \details
      Retrieves cache type that is used for caching traits.
    */
    CacheType cacheType() const
    {
      return m_cacheType;
    }

    /** \details
      Sets attributes for caching.
      \param objId [in]   Pointer to the identifier of the drawable, traits of which to cache.
      \param giCtx [in]   Drawable context.
      \param forceCaching [in]   Flag that specifies whether to force traits caching.
    */
    void setAttributes(OdDbStub* objId, OdGiContext &giCtx, bool forceCaching = false)
    {
      if (m_cacheType != kCacheNone && m_cacheData.size() > 0)
      {
        // Note: iterator won't compile in this context on all platforms.
        typename _mmap::iterator it = m_cacheData.find(objId);
        if(it != m_cacheData.end())
        {
          m_traitsAttrib = it->second.m_flags;
          if(!(m_traitsAttrib & byMask))
            traitsTaker::setData(it->second.m_data);
          return;
        }
      }
      OdGiDrawablePtr drawable = giCtx.openDrawable(objId);
      if (!drawable.isNull())
      {
        m_traitsAttrib = drawable->setAttributes(this);
        if (m_cacheType != kCacheNone)
        {
          if (m_cacheType == kCacheAll || forceCaching || ((m_traitsAttrib & byMask) != 0))
          {
            m_cacheData[objId].set(this->getData(), m_traitsAttrib);
          }
        }
      }
    }

    /** \details
      Sets a set of flags that represents attributes to cache.
      \param uAttrib [in]   Attributes to cache.
    */
    void setFlags(OdUInt32 uAttrib)
    {
      m_traitsAttrib = uAttrib;
    }
    
    /** \details
      Retrieves a set of flags that represents attributes to cache.
    */
    OdUInt32 getFlags() const
    {
      return m_traitsAttrib;
    }

    /** \details
      Clears the cached data.
    */
    void clear()
    {
      m_cacheData.clear();
    }
};

// traitsContainer - must have operator =.
// traitsTaker - must have setData(traitsContainer), getData() methods.

#include "TD_PackPop.h"

#endif // #ifndef _ODGITRAITSCACHE_INCLUDED_
