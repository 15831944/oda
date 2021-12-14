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

#ifndef __OD_GS_LIGHT_NODE__
#define __OD_GS_LIGHT_NODE__

#include "TD_PackPush.h"

#include "Gs/GsEntityNode.h"
#include "Gi/GiLightTraitsData.h"

#define STL_USING_MAP
#include "OdaSTL.h"

// Lights are entities that have some special meaning

/** <group OdGs_Classes> 
*/
class GS_TOOLKIT_EXPORT OdGsLightNode : public OdGsEntityNode
{
protected:
  OdGeMatrix3d m_model2World; // In case light isn't in model space
  OdGiLightTraitsData* m_pLightTraits;
  OdGsNode *m_pLightParent;
  typedef OdVector<OdIntPtr, OdMemoryAllocator<OdIntPtr> > LightInsertPath;
  LightInsertPath m_lightInsertParents;
  std::map< OdUInt32, OdGiLightTraitsData* > m_mapVpDepTraitsData;

  enum
  {
    kViewportDependentLight      = (OdGsEntityNode::kLastFlag << 1),
    kViewportDependentNeedUpdate = (OdGsEntityNode::kLastFlag << 2),
    kLightDrawableIsInvisible    = (OdGsEntityNode::kLastFlag << 3),
    kLastFlag                    = kLightDrawableIsInvisible
  };
protected:
  void update();

  void contextualUpdate(OdGsUpdateContext& ctx);

  void adjustViewportDependentLightTraitsData( OdGiLightTraitsVpDepData* vdData, OdGiLightTraitsData* data );

  void clearVpDepTraitsMap();

public:
  ODRX_DECLARE_MEMBERS(OdGsLightNode);

  /** \details
    Destructor for the OdGsLightNode class.
  */
  ~OdGsLightNode();
  
  /** \details
    Constructor for the OdGsLightNode class.
    
    \param pModel [in]  Pointer to the BaseModel object.
    \param pUnderlyingDrawable [in]  Pointer to the underlying Drawable object.
    \param bSetGsNode [in]  Flag that controls whether to set GS node to the underlying drawable.
  */
  OdGsLightNode(OdGsBaseModel* pModel, const OdGiDrawable* pUnderlyingDrawable, bool bSetGsNode = true);
public:

  /** \details
    Sets modeling transformation matrix.
    
    \param xform [in]  Modeling transformation matrix.
  */
  void setModelTransform(const OdGeMatrix3d& xform) { m_model2World = xform; }
  
  /** \details
    Retrieves modeling transformation matrix.
  */
  OdGeMatrix3d modelTransform() const { return m_model2World; }

  /** \details
    Sets a parent object for the light.
    
    \param ctx [in]  Update context.
    \param pParent [in]  Parent container node of the light.
  */
  void setLightOwner(OdGsUpdateContext& ctx, OdGsContainerNode* pParent);
  
  /** \details
    Checks whether the passed container node is a parent of the light.
    
    \param ctx [in]  Update context.
    \param pParent [in]  Container node to check.
    \returns
    true if the passed container node owns this light node, false otherwise.
  */
  bool isOwnedBy(OdGsUpdateContext& ctx, OdGsContainerNode* pParent) const;
  
  /** \details
    Checks whether the passed container GS node is a parent of the light.

    \param pOwner [in]  Node to check.
    \returns
    true if the passed GS node owns this light node, false otherwise.
  */
  bool isOwnedBy(const OdGsNode *pOwner) const { return m_pLightParent == pOwner; }
  
  /** \details
    Retrieves a pointer to the node that owns this light node.
  */
  OdGsNode *getLightOwner() const { return m_pLightParent; }

  /** \details
    Retrieves light type of this node.
  */
  OdGiLightTraitsData::LightType lightType();
  
  /** \details
    Checks whether this node is light node.
    \returns
    true.
  */
  virtual bool isLight() const;

  /** \details
    Retrieves a pointer to the point light traits data.
    
    \param viewportID [in]  Viewport ID for viewport-dependent lights.
    \returns
    pointer to an object that stores point light traits data, or NULL if type of this light node is not kPointLight and kWebLight.
  */
  OdGiPointLightTraitsData* pointLightTraitsData( OdUInt32 viewportID = (OdUInt32)ULONG_MAX );
  
  /** \details
    Retrieves a pointer to the spot light traits data.
    
    \param viewportID [in]  Viewport ID for viewport-dependent lights.
    \returns
    pointer to an object that stores spot light traits data, or NULL if type of this light node is not kSpotLight.
  */
  OdGiSpotLightTraitsData* spotLightTraitsData( OdUInt32 viewportID = (OdUInt32)ULONG_MAX );
  
  /** \details
    Retrieves a pointer to the distant light traits data.
    
    \param viewportID [in]  Viewport ID for viewport-dependent lights.
    \returns
    pointer to an object that stores distant light traits data, or NULL if type of this light node is not kDistantLight.
  */
  OdGiDistantLightTraitsData* distantLightTraitsData( OdUInt32 viewportID = (OdUInt32)ULONG_MAX );
  
  /** \details
    Retrieves a pointer to the web light traits data.
    
    \param viewportID [in]  Viewport ID for viewport-dependent lights.
    \returns
    pointer to an object that stores web light traits data, or NULL if type of this light node is not kWebLight.
  */
  OdGiWebLightTraitsData* webLightTraitsData( OdUInt32 viewportID = (OdUInt32)ULONG_MAX );
  
  /** \details
    Retrieves a pointer to the generic light traits data object.
    
    \param viewportID [in]  Viewport ID for viewport-dependent lights.
  */
  OdGiLightTraitsData* lightTraitsData( OdUInt32 viewportID = (OdUInt32)ULONG_MAX );

  // OdGsNode virtual overrides
  
  /** \details
    Invalidates this object.
    
    \param pParent [in]  Pointer to the ContainerNode object for this Node object.
    \param pView [in]  Pointer to the OdGsViewImpl object for which data are to be invalidated.
    \param mask [in]  Mask for viewport properties.
  */
  virtual void invalidate(OdGsContainerNode* pParent, OdGsViewImpl* pView, OdUInt32 mask);
  
  /** \details
    Updates this light node.
    
    \param ctx          [in]  Current update context.
    \param pParent      [in]  Parent container node.
    \param parentIndex  [in]  Parent spatial index.
  */
  virtual void update(OdGsUpdateContext& ctx, OdGsContainerNode* pParent,
      OdSiSpatialIndex*);

  /** \details
    Saves data of a client light node to the specified filer.
    \param pFiler       [out]  Pointer to a filer where to save node state.
    \param pVectorizer  [in]  Pointer to the vectorizer.
    \returns
    true if the state of the client light node is saved successfully, false otherwise.
  */
  virtual bool saveClientNodeState(OdGsFiler *pFiler, OdGsBaseVectorizer *pVectorizer) const;
  
  /** \details
    Loads data of a client light node from the specified filer.
    \param pFiler       [in]  Pointer to a filer where to save node state.
    \param pVectorizer  [in]  Pointer to the vectorizer.
    \returns
    true if the state of the client light node is loaded successfully, false otherwise.
  */
  virtual bool loadClientNodeState(OdGsFiler *pFiler, OdGsBaseVectorizer *pVectorizer);

  /** \details
    Updates viewport-dependent properties.
    \param pView [in] View implementation.
  */
  void updateViewportDependent( OdGsViewImpl* pView );

  /** \details
    Checks whether the passed drawable is light.

    \param pDrawable [in]  Pointer to the Drawable object.
    \returns 
    true if the specified drawable is a light entity, false otherwise.
  */
  static bool drawableIsLight(const OdGiDrawable *pDrawable);
};

/** \details
  A data type that represents a smart pointer to an <link OdGsLightNode, OdGsLightNode> object.
*/
typedef OdSmartPtr<OdGsLightNode> OdGsLightNodePtr;

inline OdGiLightTraitsData::LightType OdGsLightNode::lightType()
{
  return OdGiLightTraitsData::drawableLightType(underlyingDrawable());
}

inline bool OdGsLightNode::isLight() const
{
  return true;
}

inline OdGiPointLightTraitsData* OdGsLightNode::pointLightTraitsData( OdUInt32 viewportID )
{
  if (!m_pLightTraits || ((m_pLightTraits->type() != OdGiLightTraitsData::kPointLight) && (m_pLightTraits->type() != OdGiLightTraitsData::kWebLight)))
    return NULL;
  if(GETBIT(m_flags, kViewportDependentLight))
  {
    std::map< OdUInt32, OdGiLightTraitsData* >::iterator it = m_mapVpDepTraitsData.find( viewportID );
    if( it != m_mapVpDepTraitsData.end() )
    {
      return (OdGiPointLightTraitsData*)(it->second);
    }
  }
  return (OdGiPointLightTraitsData*)m_pLightTraits;
}
inline OdGiSpotLightTraitsData* OdGsLightNode::spotLightTraitsData( OdUInt32 viewportID )
{
  if (!m_pLightTraits || (m_pLightTraits->type() != OdGiLightTraitsData::kSpotLight))
    return NULL;
  if(GETBIT(m_flags, kViewportDependentLight))
  {
    std::map< OdUInt32, OdGiLightTraitsData* >::iterator it = m_mapVpDepTraitsData.find( viewportID );
    if( it != m_mapVpDepTraitsData.end() )
    {
      return (OdGiSpotLightTraitsData*)(it->second);
    }
  }
  return (OdGiSpotLightTraitsData*)m_pLightTraits;
}
inline OdGiDistantLightTraitsData* OdGsLightNode::distantLightTraitsData( OdUInt32 viewportID )
{
  if (!m_pLightTraits || (m_pLightTraits->type() != OdGiLightTraitsData::kDistantLight))
    return NULL;
  if(GETBIT(m_flags, kViewportDependentLight))
  {
    std::map< OdUInt32, OdGiLightTraitsData* >::iterator it = m_mapVpDepTraitsData.find( viewportID );
    if( it != m_mapVpDepTraitsData.end() )
    {
      return (OdGiDistantLightTraitsData*)(it->second);
    }
  }
  return (OdGiDistantLightTraitsData*)m_pLightTraits;
}
inline OdGiWebLightTraitsData* OdGsLightNode::webLightTraitsData( OdUInt32 viewportID )
{
  if (!m_pLightTraits || (m_pLightTraits->type() != OdGiLightTraitsData::kWebLight))
    return NULL;
  if(GETBIT(m_flags, kViewportDependentLight))
  {
    std::map< OdUInt32, OdGiLightTraitsData* >::iterator it = m_mapVpDepTraitsData.find( viewportID );
    if( it != m_mapVpDepTraitsData.end() )
    {
      return (OdGiWebLightTraitsData*)(it->second);
    }
  }
  return (OdGiWebLightTraitsData*)m_pLightTraits;
}
inline OdGiLightTraitsData* OdGsLightNode::lightTraitsData( OdUInt32 viewportID )
{
  if(GETBIT(m_flags, kViewportDependentLight))
  {
    std::map< OdUInt32, OdGiLightTraitsData* >::iterator it = m_mapVpDepTraitsData.find( viewportID );
    if( it != m_mapVpDepTraitsData.end() )
    {
      return it->second;
    }
  }
  return m_pLightTraits;
}

inline void OdGsLightNode::adjustViewportDependentLightTraitsData( OdGiLightTraitsVpDepData* vdData, OdGiLightTraitsData* data )
{
  if( !GETBIT(m_flags, kViewportDependentLight) ) return;
  if( !data || !vdData ) return;
  data->setOn( vdData->vpDepOn() && !GETBIT(m_flags, kLightDrawableIsInvisible) );
  data->setIntensity( data->intensity() * ( 1.0 - vdData->vpDepDimming() ) );
}

inline bool OdGsLightNode::drawableIsLight(const OdGiDrawable *pDrawable)
{
  return (OdGiLightTraitsData::drawableLightType(pDrawable) != OdGiLightTraitsData::kInvalidLight);
}

inline void OdGsLightNode::clearVpDepTraitsMap()
{
  for( std::map< OdUInt32, OdGiLightTraitsData* >::iterator it = m_mapVpDepTraitsData.begin(); it != m_mapVpDepTraitsData.end(); ++it )
  {
    OdGiLightTraitsData::deleteLightTraitsData(it->second);
  }
  m_mapVpDepTraitsData.clear();
}

#include "TD_PackPop.h"

#endif // __OD_GS_LIGHT_NODE__
