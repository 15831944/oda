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

#ifndef _ODGSBASELAYOUTHELPER_H_
#define _ODGSBASELAYOUTHELPER_H_

#include "Gs/Gs.h"
#include "IntArray.h"
#include "Ge/GePoint3d.h"
#include "StaticRxObject.h"
#include "ThreadsCounter.h"
#include "Gi/GiCommonDraw.h"
#include "Gs/GsModel.h"
#include "Gs/GsCacheRedirection.h"
#include "RxDictionary.h"

struct OdGsLayoutHelperIntCache : public OdGsCache
{
  struct CacheEntry
  {
    OdGsModelPtr m_pModel;
    bool m_bPlotGeneration; // Initial state of plot generation flag
    unsigned m_threadId; // In which thread vectorization was initiated
    OdRefCounter m_nRefs;
  };
  OdVector<CacheEntry> m_cacheEntries;
  OdGsModel *cast(const OdGsModel *pModel) const { return const_cast<OdGsModel*>(pModel); }
  OdUInt32 numModels() const { return m_cacheEntries.size(); }
  OdGsModel *gsModelAt(OdUInt32 nModel) { return m_cacheEntries.at(nModel).m_pModel; }
  OdGsModelRedirectionWrapper<OdGsLayoutHelperIntCache, OdStaticRxObject<OdGsModel> > m_modelRedirection;
  OdGsLayoutHelperIntCache() { m_modelRedirection.setRedirectionClient(this); }
  // gcc build
  ODRX_HEAP_OPERATORS();
  // OdGsCache overrides
  OdGsModel* model() const { return (m_cacheEntries.size() == 1) ? cast(m_cacheEntries.begin()->m_pModel)
                                                                 : cast(&m_modelRedirection); }
  void setDrawableNull() { }
  bool extents(OdGeExtents3d& /*extents*/) const { return false; }
  //
};

template <class TBaseDatabase, class TBaseObject>
class OdGsBaseLayoutHelperInt
{
 protected:
  void objectAppendedBase(const TBaseDatabase* , const TBaseObject* pDbObj)
  {
    if(m_pGsModel.isNull())
    {
      m_pDevice->invalidate();
      return;
    }

    baseModel()->onAdded(const_cast<TBaseObject*>(pDbObj), pDbObj->ownerId());
  }

  void objectModifiedBase(const TBaseDatabase* , const TBaseObject* pDbObj)
  {
    if(m_pGsModel.isNull())
    {
      m_pDevice->invalidate();
      return;
    }

    baseModel()->onModified(const_cast<TBaseObject*>(pDbObj), pDbObj->ownerId());
  }

  void objectErasedBase(const TBaseDatabase* pDb, const TBaseObject* pDbObj, bool bErased)
  {
    if(m_pGsModel.isNull())
    {
      m_pDevice->invalidate();
      return;
    }

    if (bErased)
      baseModel()->onErased(const_cast<TBaseObject*>(pDbObj), pDbObj->ownerId());
    else
      baseModel()->onUnerased(const_cast<TBaseObject*>(pDbObj), pDbObj->ownerId());
  }

  void goodbyeBase(const TBaseDatabase* )
  {
    m_pDevice.release();
    m_layoutId = NULL;
    m_nActive = 0;
    if (!m_pGsModel.isNull())
    {
      OdGsLayoutHelperIntCache *pModelCache = static_cast<OdGsLayoutHelperIntCache*>(m_pDb->gsNode());
      if (!pModelCache) throw OdError(eNullPtr);
      for (OdUInt32 nCacheEntry = 0; nCacheEntry < pModelCache->m_cacheEntries.size(); nCacheEntry++)
      { OdGsLayoutHelperIntCache::CacheEntry &pCacheEntry = pModelCache->m_cacheEntries.at(nCacheEntry);
        if (pCacheEntry.m_pModel.get() == m_pGsModel.get())
        {
          if (!--pCacheEntry.m_nRefs)
            pModelCache->m_cacheEntries.removeAt(nCacheEntry);
          break;
        }
      }
      if (pModelCache->m_cacheEntries.isEmpty())
      {
        m_pDb->setGsNode(0);
        pModelCache->release();
      }
    }
    m_pDb = 0;
    m_pGsModel.release();
  }
 
  OdGsModel *baseModel() const { return m_pDb->gsNode()->model(); }

  TBaseDatabase*   m_pDb;
public:
  TBaseDatabase* database() const { return const_cast<TBaseDatabase*>(m_pDb); }

  OdGsModelPtr initCache(TBaseDatabase *pDb, OdGiContext *pCtx, OdGiOpenDrawableFn openDrawableFn)
  {
    OdGsLayoutHelperIntCache *pModelCache = static_cast<OdGsLayoutHelperIntCache*>(pDb->gsNode());
    if (!pModelCache)
      pDb->setGsNode(pModelCache = OdRxObjectImpl<OdGsLayoutHelperIntCache>::createObject().detach());
    OdGsLayoutHelperIntCache::CacheEntry *pCacheEntry = NULL;
    for (OdUInt32 nCacheEntry = 0; nCacheEntry < pModelCache->m_cacheEntries.size(); nCacheEntry++)
    { OdGsLayoutHelperIntCache::CacheEntry &cacheEntry = pModelCache->m_cacheEntries.at(nCacheEntry);
      if (m_pDevice->isModelCompatible(cacheEntry.m_pModel) &&
          (cacheEntry.m_bPlotGeneration == pCtx->isPlotGeneration()) &&
          (cacheEntry.m_threadId == ::odGetCurrentThreadId()))
      {
        pCacheEntry = &cacheEntry;
        ++pCacheEntry->m_nRefs;
        break;
      }
    }
    if (!pCacheEntry)
    {
      OdGsLayoutHelperIntCache::CacheEntry newEntry;
      newEntry.m_pModel = m_pDevice->createModel();
      if (!newEntry.m_pModel.isNull())
      {
        newEntry.m_bPlotGeneration = pCtx->isPlotGeneration();
        newEntry.m_threadId = ::odGetCurrentThreadId();
        newEntry.m_nRefs = 1;
        newEntry.m_pModel->setOpenDrawableFn(openDrawableFn);
        pModelCache->m_cacheEntries.push_back(newEntry);
        pCacheEntry = &pModelCache->m_cacheEntries.last();
      }
    }
    if (!pCacheEntry)
    {
      if (pModelCache->m_cacheEntries.empty())
      {
        pDb->setGsNode(NULL);
        pModelCache->release();
      }
      return OdGsModelPtr();
    }
    m_pGsModel = pCacheEntry->m_pModel;
    return m_pGsModel;
  }

  mutable
  OdGsDevicePtr   m_pDevice;
  OdUInt32        m_nActive;
  OdGsModelPtr    m_pGsModel;
  OdDbStub*       m_layoutId;

  bool viewIndex(OdDbStub* vpId, OdUInt32& index)
  {
    OdUInt32 numViews = m_pDevice->numViews();
    for(OdUInt32 i = 0; i < numViews; i ++)
    {
      OdGsClientViewInfo viewInfo;
      m_pDevice->viewAt(i)->clientViewInfo(viewInfo);
      if(viewInfo.viewportObjectId == vpId)
      {
        index = i;
        return true;
      }
    }
    return false;
  }

  bool viewIndex(OdGsView* pView, OdUInt32& index)
  {
    OdUInt32 len = m_pDevice->numViews();
    if(len)
    {
      for(OdUInt32 i = 0; i<len; ++i)
      {
        if(pView == m_pDevice->viewAt(i))
        {
          index = i;
          return true;
        }
      }
    }
    return false;
  }

  OdGsBaseLayoutHelperInt()
    : m_pDb(NULL)
    , m_nActive(0)
    , m_layoutId(NULL)
  {}

  virtual ~OdGsBaseLayoutHelperInt()
  {
    if(m_pDb)
    {
      goodbyeBase(m_pDb);
    }
  }
};

template <class TBase, class TBaseLayoutHelper>
class OdGsBaseLayoutHelperImpl : public TBase
{
protected:
  TBaseLayoutHelper  m_int;

public:  
  OdGiContext* userGiContext() const
  {
    return m_int.m_pDevice->userGiContext();
  }

  void setUserGiContext(OdGiContext* pUserContext)
  {
    m_int.m_pDevice->setUserGiContext(pUserContext);
  }

  OdRxDictionaryPtr properties()
  {
    return m_int.m_pDevice->properties();
  }

  void invalidate()
  {
    m_int.m_pDevice->invalidate();
  }

  void invalidate(const OdGsDCRect &rect)
  {
    m_int.m_pDevice->invalidate(rect);
  }

  bool isValid() const
  {
    return m_int.m_pDevice->isValid();
  }

  void onRealizeForegroundPalette()
  {
    m_int.m_pDevice->onRealizeForegroundPalette();
  }

  void onRealizeBackgroundPalette()
  {
    m_int.m_pDevice->onRealizeBackgroundPalette();
  }

  void onDisplayChange(int nBitsPerPixel, int nXRes, int nYRes)
  {
    m_int.m_pDevice->onDisplayChange(nBitsPerPixel, nXRes, nYRes);
  }

  int numViews() const
  {
    return m_int.m_pDevice->numViews();
  }

  OdGsView* viewAt(int n)
  {
    return m_int.m_pDevice->viewAt(n);
  }

  bool setBackgroundColor(ODCOLORREF color)
  {
    return m_int.m_pDevice->setBackgroundColor(color);
  }

  ODCOLORREF getBackgroundColor()
  {
    return m_int.m_pDevice->getBackgroundColor();
  }

  void setLogicalPalette(const ODCOLORREF* palette, int nCount)
  {
    m_int.m_pDevice->setLogicalPalette(palette, nCount);
  }

  const ODCOLORREF* getLogicalPalette(int &numColors) const
  {
    return m_int.m_pDevice->getLogicalPalette(numColors);
  }

  void getSnapShot(OdGiRasterImagePtr &pImage, const OdGsDCRect &region)
  {
    m_int.m_pDevice->getSnapShot(pImage, region);
  }

  OdGsModelPtr createModel()
  {
    return m_int.m_pDevice->createModel();
  }

  OdGsViewPtr createView(const OdGsClientViewInfo* pInfo, bool bEnableLayerVisibilityPerView = false)
  {
    return m_int.m_pDevice->createView(pInfo, bEnableLayerVisibilityPerView);
  }

  void addView(OdGsView* pView)
  {
    m_int.m_pDevice->addView(pView);
  }

  void insertView(int viewIndex, OdGsView* pView)
  {
    m_int.m_pDevice->insertView(viewIndex, pView);
  }

  bool eraseView(OdGsView* pView)
  {
    return m_int.m_pDevice->eraseView(pView);
  }

  bool eraseView(int n)
  {
    return m_int.m_pDevice->eraseView(n);
  }

  void eraseAllViews()
  {
    m_int.m_pDevice->eraseAllViews();
  }

  void onSize(const OdGsDCRect& outputRect)
  {
    m_int.m_pDevice->onSize(outputRect);
  }

  void getSize(OdGsDCRect& outputRect) const
  {
    m_int.m_pDevice->getSize(outputRect);
  }

  bool isModelCompatible(OdGsModel* pModel) const
  {
    return m_int.m_pDevice->isModelCompatible(pModel);
  }

  OdDbStub* getLayoutId() const
  {
    return m_int.m_layoutId;
  }

  OdGsViewPtr activeView() const
  {
    return m_int.m_pDevice->viewAt(m_int.m_nActive);
  }

  void makeViewActive(OdGsView* pView)
  {
    if(!m_int.viewIndex(pView, m_int.m_nActive))
      throw OdError(eInvalidInput);
  }

  OdGsModel* gsModel()
  {
    return m_int.m_pGsModel;
  }

  OdGsDevicePtr underlyingDevice() const
  {
    return m_int.m_pDevice;
  }

  void enableLightsInBlocks(bool bEnable)
  {
    if (!m_int.m_pGsModel.isNull())
      m_int.m_pGsModel->setEnableLightsInBlocks(bEnable);
  }
  bool isLightsInBlocksEnabled() const
  {
    if (!m_int.m_pGsModel.isNull())
      return m_int.m_pGsModel->isLightsInBlocksEnabled();
    return true;
  }

  bool setActiveViewport(const OdGePoint2d& at, int iOverall, int iHelper)
  {
    int i, viewsNumber = numViews();
    //for(i = 0; i < viewsNumber; i ++) // Backward order must be here
    for (i = viewsNumber - 1; i >= 0; i--)
    {
      if(i != iOverall && i != iHelper)
      {
        OdGsView* pView = viewAt(i);
        if(pView->isVisible() && pView->pointInViewport(at))
        {
          makeViewActive(pView);
          return true;
        }
      }
    }
    if(iOverall >= 0)
    {
      makeViewActive(viewAt(iOverall));
      return true;
    }
    return false;
  }

  bool setActiveViewportStub(OdDbStub* id)
  {
    return m_int.viewIndex(id, m_int.m_nActive);
  }

  OdDbStub* getActiveViewportStub()
  {
    OdGsClientViewInfo viewInfo;
    m_int.m_pDevice->viewAt(m_int.m_nActive)->clientViewInfo(viewInfo);
    return viewInfo.viewportObjectId;
  }
};

#endif //#ifndef _ODGSBASELAYOUTHELPER_H_
