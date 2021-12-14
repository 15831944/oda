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

#ifndef __OD_GS_MATERIAL_NODE__
#define __OD_GS_MATERIAL_NODE__

#include "TD_PackPush.h"

#include "Gs/GsNode.h"
#include "Gi/GiMaterial.h"
#include "Gi/GiMaterialTraitsData.h"
#include "ThreadsCounter.h"
#define STL_USING_MAP
#include "OdaSTL.h"

// Materials cached in GsModel, but in separate cache, instead all geometry.

// GsModel caches hierarchy:
// OdGsBaseModel
//   OdGsContainerNode <-- OdGsNode - geometry cache
//     OdGsEntityNode <-- OdGsNode
//       OdGsLightNode - has separate array, but stored into geometry cache too
//   OdGsLayerNode <-- OdGsNode - layers cache
// OdGsBaseMode::OdGsMaterialCache
//   OdGsMaterialNode <-- OdGsNode - materials cache

/** <group OdGs_Classes> 
*/
class GS_TOOLKIT_EXPORT OdGsMaterialNode : public OdGsNode
{
protected:
  /** \details
      No operation.
  */
  void update(OdGsUpdateContext& ctx, OdGsContainerNode* pParent,
      OdSiSpatialIndex* pParentIndex);

  /** \details
      No operation.
  */
  void display(OdGsDisplayContext& ctx);

  /** \details
      No operation.
  */
  bool extents(OdGeExtents3d& ) const;

  /** \details
      No operation.
  */
  virtual void propagateLayerChanges(OdGsViewImpl& view);

  /** \details
      Material traits.
  */
  OdGiMaterialTraitsData m_materialTraits;
  
  /** \details
      Flag that shows whether data is initialized.
  */
  bool m_dataInitialized;
  
  /** \details
      Flag that shows whether data is modified.
  */
  bool m_dataModified;
  
  /** \details
      Represents a material data entry.
  */
  struct DataEntry
  {
    /** \details
      Flag that shows whether cached material data is modified.
    */
    bool m_cachedDataModified;
    
    /** \details
      Smart point to cached material data.
    */
    OdRxObjectPtr m_pCachedData;

    /** \details
      Default constructor for the DataEntry structure.
    */
    DataEntry() : m_cachedDataModified(true) { }
  };
  /** \details
    Data type that represents a cache map.
  */
  typedef std::map<const void *, DataEntry> CacheMap;
  
  /** \details
    Cache map.
  */
  CacheMap m_cacheMap;
  
  /** \details
    Cache mutex.
  */
  OdMutexPtr m_pCacheMutex;

  /** \details
    Pointer to previous material node.
  */
  mutable OdGsMaterialNode* m_pMatPrev;
  
  /** \details
    Pointer to next material node.
  */
  mutable OdGsMaterialNode* m_pMatNext;
public:
  ODRX_DECLARE_MEMBERS(OdGsMaterialNode);

  /** \details
    Constructor for the OdGsMaterialNode class.
    
    \param pModel [in]  Pointer to the BaseModel object.
    \param pUnderlyingDrawable [in]  Pointer to the underlying Drawable object.
    \param bSetGsNode [in]  Flag that controls whether to set GS node to the underlying drawable.
  */
  OdGsMaterialNode(OdGsBaseModel* pModel, const OdGiDrawable* pUnderlyingDrawable, bool bSetGsNode = true);
  
  /** \details
    Destructor for the OdGsMaterialNode class.
  */
  ~OdGsMaterialNode();

  void destroy() {}

  /** \details
    Retrieves material traits associated with this material node.
  */
  OdGiMaterialTraitsData& materialTraits();
  
  /** \details
    Retrieves material traits associated with this material node.
  */
  const OdGiMaterialTraitsData& materialTraits() const;

  /** \details
    Notification method called from OdGsNode if material data changed at update.
    May be used by inherited nodes to update device-dependent information.
    
    \param view [in]  Vectorizer view.
  */
  virtual void onModified(OdGsBaseVectorizer& view);

  /** \details
      No operation.
  */
  void invalidate(OdGsContainerNode* pParent, OdGsViewImpl* pView, OdUInt32 mask);
  
  /** \details
    Erases cache map for the passed module.
    
    \param pModule [in]  Pointer to a module for which to erase a cache map.
  */
  void invalidateModule(const void *pModule);

  /** \details
    Updates this material node.
    
    \param view [in]  Vectorizer view.
  */
  virtual void update(OdGsBaseVectorizer& view);

  /** \details
    Retrieves a pointer to the next material node.
  */
  OdGsMaterialNode *nextNode() const;
  
  /** \details
    Retrieves a pointer to the previous material node.
  */
  OdGsMaterialNode *prevNode() const;
  
  /** \details
    Sets the next material node.
    
    \param newNode [in]  Pointer to new material node.
    \returns
    pointer to the currently next material node that is replaced with new passed material node.
  */
  OdGsMaterialNode *setNextNode(OdGsMaterialNode *newNode);
  
  /** \details
    Sets the previous material node.
    
    \param newNode [in]  Pointer to new material node.
    \returns
    pointer to the currently previous material node that is replaced with new passed material node.
  */
  OdGsMaterialNode *setPrevNode(OdGsMaterialNode *newNode);

  /** \details
    Checks whether material data is modified for this object.
    
    \returns
    true if data has been modified, false otherwise.
  */
  bool isDataModified() const;

  /** \details
    Checks whether cached material data is modified for this object.
    
    \returns
    true if cached data has been modified, false otherwise.
  */
  bool isCachedDataModified(OdGsBaseVectorizer& view) const;
  
  /** \details
    Specifies that cached data of the passed view's module is updated (not modified in other words).
    
    \param view [in]  Vectorizer view.
  */
  void setCachedDataUpdated(OdGsBaseVectorizer& view);

  /** \details
    Retrieves pointer to the material cache data object.
    
    \param view [in]  Vectorizer view.
  */
  const OdRxObject *cachedData(OdGsBaseVectorizer& view) const;
  
  /** \details
    Sets material cache data for the passed view.
    
    \param view           [in]  Vectorizer view.
    \param newCachedData  [in]  Cache data to set.
  */
  void setCachedData(OdGsBaseVectorizer& view, OdRxObject *newCachedData);

  /** \details
    Retrieves cache mutex.
  */
  OdMutexPtr &cacheMutex() { return m_pCacheMutex; }

  /** \details
    Retrieves node type of this object.
    
    \returns
    kMaterialNode.
  */
  virtual ENodeType nodeType() const { return kMaterialNode; }

  /** \details
    Stores current node state into filer object.

    \param pFiler       [in]  Filer object.
    \param pVectorizer  [in]  Pointer to a vectorizer. May be required for saving client node state.
    \returns
    true state of this node is successfully saved, false otherwise.
  */
  bool saveNodeState(OdGsFiler *pFiler, OdGsBaseVectorizer *pVectorizer = NULL) const;
  
  /** \details
    Loads current node state from filer object.

    \param pFiler       [in]  Filer object.
    \param pVectorizer  [in]  Pointer to a vectorizer. May be required for saving client node state.
    \returns
    true state of this node is successfully loaded, false otherwise.
  */
  bool loadNodeState(OdGsFiler *pFiler, OdGsBaseVectorizer *pVectorizer = NULL);
};

// Cache for material nodes
#include "GsMaterialCache.h"

#include "TD_PackPop.h"

#endif // __OD_GS_MATERIAL_NODE__
