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

#ifndef __OD_GS_MATERIAL_CACHE__
#define __OD_GS_MATERIAL_CACHE__

// Cache for material nodes

#include "TD_PackPush.h"

#include "RxObject.h"
#include "ThreadsCounter.h"

class OdGsBaseVectorizer;
class OdGsBaseModel;
class OdGsMaterialNode;
class OdGsBaseModule;
class OdGsFiler;

/** <group OdGs_Classes> 
*/
class GS_TOOLKIT_EXPORT OdGsMaterialCache : public OdRxObject
{
  protected:
    OdUInt32           m_nMaterials;
    OdGsMaterialNode*  m_pMaterials;
    OdGsBaseModel*     m_pModel;
    mutable OdMutexPtr m_pCacheMutex;
  public:
    
    /** \details
      Constructor for the OdGsLightNode class. Sets material cache and associated model to NULL.
    */
    OdGsMaterialCache();
    
    /** \details
      Destructor for the OdGsLightNode class.
    */
    ~OdGsMaterialCache();

    ODRX_DECLARE_MEMBERS(OdGsMaterialCache);

    /** \details
      Sets the base model for this object.

      \param pModel [in]  Pointer to base model.
    */
    void setBaseModel(OdGsBaseModel* pModel);

    /** \details
      Retrieves pointer to the base model associated with this object.
    */
    OdGsBaseModel* baseModel() { return m_pModel; }
    
    /** \details
      Retrieves pointer to the base model associated with this object.
    */
    const OdGsBaseModel* baseModel() const { return m_pModel; }

    /** \details
      Creates a material cache and returns a smart pointer to it.
      
      \param pModel [in]  Pointer to base model.
    */
    static OdSmartPtr<OdGsMaterialCache> createObject(OdGsBaseModel* pModel);

    // Search for already initialized node
    /** \details
      Searches for already initialized node by the persistent object ID.
      
      \param mtl [in]  Persistent object ID that represents a material.
    */
    OdGsMaterialNode *searchNode(OdDbStub *mtl) const;

    /** \details
      Initializes a new node, if not already initialized or changed and set's as current node and returns a pointer to the initialized node.
      
      \param view         [in]  Vectorization view.
      \param mtl          [in]  Persistent object ID that represents a material.
      \param bDontReinit  [in]  Flag that specifies whether initialization should not be performed if this node is previously initialized and not changed.
    */
    OdGsMaterialNode *setMaterial(OdGsBaseVectorizer& view, OdDbStub *mtl, bool bDontReinit = false);

    /** \details
      Removes material node on Erase.

      \param mtl          [in]  Persistent object ID that represents a material.
    */
    bool removeNode(OdDbStub *mtl);
    
    /** \details
      Removes material node on Erase.

      \param pCsh         [in]  Pointer to GS cache.
    */
    bool removeNode(OdGsCache *pCsh);

    // Cache accessors
    
    /** \details
      Retrieves a number of cache elements.
    */
    OdUInt32 getCacheSize() const;
    OdGsMaterialNode *getCacheNode(OdUInt32 n) const;

    // Aliases
    
    /** \details
      Adds a material node to cache.
      
      \param view         [in]  Vectorization view.
      \param mtl          [in]  Persistent object ID that represents a material.
    */
    inline void addNode(OdGsBaseVectorizer& view, OdDbStub *mtl)
    {
      setMaterial(view, mtl, true);
    }

    /** \details
      Clears cache.
    */
    void clearCache();

    /** \details
      Invalidates material cache.
      
      \param pModule         [in]  Pointer to module which is erased from the map cache.
    */
    void invalidateCache(const OdGsBaseModule *pModule = NULL);

    
    /** \details
      Saves material cache to the specified filer.
      \param pFiler [in]  Pointer to a filer where to save the material item.
      \returns
      true if material cache is successfully saved, false otherwise.
    */
    bool saveMaterialCache(OdGsFiler *pFiler) const;
    
    /** \details
      Loads material cache from the specified filer.
      \param pFiler [in]  Pointer to a filer where to load the material item from.
      \returns
      true if material cache is successfully loaded, false otherwise.
    */
    bool loadMaterialCache(OdGsFiler *pFiler);
};

typedef OdSmartPtr<OdGsMaterialCache> OdGsMaterialCachePtr;

/** <group OdGs_Classes> 
*/
class GS_TOOLKIT_EXPORT OdGsCurrentMaterialNode
{
  protected:
    OdGsMaterialNode* m_pCurrentNode;
  public:
    
    /** \details
      Constructor for the OdGsCurrentMaterialNode class. Sets current material node from the passed parameter.
      
      \param pCurrentNode [in]  Pointer to current material node.
    */
    OdGsCurrentMaterialNode(OdGsMaterialNode *pCurrentNode = NULL)
      : m_pCurrentNode(pCurrentNode) {}
    
    /** \details
      Destructor for the OdGsCurrentMaterialNode class.
    */
    ~OdGsCurrentMaterialNode() {}

    /** \details
      Retrieves pointer to current material node.
    */
    OdGsMaterialNode *currentNode() const
    {
      return m_pCurrentNode;
    }

     /** \details
      Sets a new current material node.
      
      \param pNode [in]  Pointer to current material node.
    */
    void setCurrentNode(OdGsMaterialNode *pNode = NULL)
    {
      m_pCurrentNode = pNode;
    }
    
    /** \details
      Initializes a new node, if not already initialized or changed and set's as current node and returns a pointer to the initialized node.
      
      \param pCache       [in]  Pointer to material cache.
      \param view         [in]  Vectorization view.
      \param mtl          [in]  Persistent object ID that represents a material.
      \param bDontReinit  [in]  Flag that specifies whether initialization should not be performed if this node is previously initialized and not changed.
    */
    OdGsMaterialNode *setMaterial(OdGsMaterialCache *pCache, OdGsBaseVectorizer& view, OdDbStub *mtl, bool bDontReinit = false);
};

#include "TD_PackPop.h"

#endif // __OD_GS_MATERIAL_CACHE__
