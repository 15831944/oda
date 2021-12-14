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

#ifndef __OD_GS_VIEW_PROPS__
#define __OD_GS_VIEW_PROPS__

#include "GsViewPropsDef.h"
#include "Ge/GeMatrix3d.h"
#include "DbStubPtrArray.h"
#include "GsViewLocalId.h"

#include "TD_PackPush.h"

class OdGsViewImpl;

/** \details
    
    Corresponding C++ library: TD_Gs

    <group !!RECORDS_TD_APIRef> 
*/
struct GS_TOOLKIT_EXPORT ViewProps
{
public:
    /** \details
      Default constructor for the ViewProps class. Sets the following data members:
      <table>
      Data member                 Value                             Description
      m_vpId                      0                                 Viewport ID.
      m_vpObjectId                NULL                              Persistent viewport object ID.
      m_regenType                 eOdGiRegenTypeInvalid             Regeneration type of the view.
      m_renderMode                OdGsView::k2DOptimized            View render mode.
      m_frontClip                 .0                                Front clip distance from the target of the viewport.
      m_backClip                  .0                                Back clip distance from the target of the viewport.
      m_linetypeScaleMultiplier   .0                                Linetype scale multiplier.
      m_renderModule              NULL                              Module that is used for rendering.
      m_annoScale                 NULL                              Persistent annotation scale object ID.
      m_filterFunction            0                                 Drawables filtration function.
      m_visualStyle               NULL                              Visual style.
      m_nViewChanges              0                                 View changes flags.
      </table>
      
      The constructor also sets deviation values for all deviation types that are used for current vectorization to 0.
    */
    ViewProps();
    
    /** Viewport ID. */
    OdUInt32              m_vpId;
    
    /** Persistent viewport object ID. */
    OdDbStub*             m_vpObjectId;
    
    /** Regeneration type. */
    OdGiRegenType         m_regenType;
    
    /** View render mode. */
    OdGsView::RenderMode  m_renderMode;
    
    /** World to eye transformation matrix. */
    OdGeMatrix3d          m_worldToEye;
    
    /** Camera location. */
    OdGePoint3d           m_cameraLocation;
    
    /** Camera target. */
    OdGePoint3d           m_cameraTarget;
    
    /** Camera "up" vector. */
    OdGeVector3d          m_cameraUpVector;
    
    /** Vector from the camera target to the camera location. */
    OdGeVector3d          m_viewDir;
    
    /** Viewport lower left point. */
    OdGePoint2d           m_vpLowerLeft;
    
    /** Viewport upper right point. */
    OdGePoint2d           m_vpUpperRight;
    
    /** Valuee for deviation types. */
    double                m_deviation[5];
    
    /** Front clip distance from the target of the viewport. */
    double                m_frontClip;
    
    /** Back clip distance from the target of the viewport. */
    double                m_backClip;
    
    /** Array of frozen layers. */
    OdDbStubPtrArray      m_frozenLayers;
    
    /** Linetype scale multiplier. */
    double                m_linetypeScaleMultiplier;
    
    /** Module that is used for rendering. */
    const void*           m_renderModule;
    
    /** Persistent annotation scale object ID. */
    OdDbStub*             m_annoScale;
    
    /** Pointer to the drawables filtration function. */
    OdIntPtr              m_filterFunction;
    
    /** Visual style. */
    OdDbStub*             m_visualStyle;
    
    /** View changes flags. */
    OdUInt32              m_nViewChanges;
    
    /** \details
      Sets view properties from the specified view.
      \param view [in]  View.
    */
    void set(const OdGsViewImpl& view);
    
    /** \details
      Retrieves a set of flags that are different from the specified view properties. 
      See enum located in GsViewPropsDef.h.
      
      \param props [in]  View properties to compare.
    */
    OdUInt32 difference(const ViewProps& props) const;
    
    /** \details
      Checks whether view the passed view properties are compatible with this view properties. 
      
      \param props                [in]  View properties to compare.
      \param nFlags               [in]  Mask that defines which properties to compare.
      \param bCompareFrozenLayers [in]  Flag that specifies whether to compare frozen layers.
      \returns
      true if view the passed view properties are compatible with this view properties, false otherwise.
    */
    bool isCompatibleWith(const ViewProps& props, OdUInt32 nFlags,
        bool bCompareFrozenLayers = true) const;
    
    /** \details
      Checks whether view properties are invalid. 
      
      \returns
      true if view properties are invalid, false otherwise.
    */
    bool isInvalid() const { return GETBIT(m_nViewChanges, 0x80000000); }
    
    /** \details
      Specifies whether view properties are invalid. 
      
      \param bValue  [in]  Flag that specifies whether view properties are invalid.
    */
    void setInvalid(bool bValue) { SETBIT(m_nViewChanges, 0x80000000, bValue); }
};
/** \details
  A data type that represents an array of ViewProps objects.
*/
typedef OdArray<ViewProps> ViewPropsArray;

inline bool isInvalid(const ViewPropsArray& props)
{
  unsigned i = 0;
  for(; (i < props.size()) && props.getAt(i).isInvalid(); ++i);
  return (i == props.size()); //all are invalid
}

/** \details
    
    Corresponding C++ library: TD_Gs

    <group OdGs_Classes> 
*/
class GS_TOOLKIT_EXPORT ViewRefs
{
public:
  ViewRefs() : m_nViews(0), m_nMaxVpId(-1) {}
  OdUInt32 numViews() const { return m_nViews; }
  void add(OdUInt32 viewportId)
  {
    if(viewportId < m_data.size())
    {
      if(!m_data[viewportId])
        ++m_nViews;
    }
    else
    {
      m_data.insert(m_data.end(), viewportId + 1 - m_data.size(), 0);
      ++m_nViews;
    }
    ++m_data[viewportId];
    m_nMaxVpId = -1;
  }
  void remove(OdUInt32 viewportId)
  {
    ODA_ASSERT(m_nViews);
    ODA_ASSERT(m_data.size() > viewportId);
    ODA_ASSERT(m_data[viewportId] > 0);
    if((--m_data[viewportId]) == 0)
    {
      --m_nViews;
      if(!m_nViews)
        m_data.clear();
    }
    m_nMaxVpId = -1;
  }
  int maxViewportId() const
  {
    if (m_nMaxVpId < 0)
    {
      for(unsigned i = m_data.size(); i; )
        if(m_data[--i])
          return m_nMaxVpId = (int)i;
    }
    return m_nMaxVpId;
  }
  unsigned numDifferentViewports() const
  {
    unsigned count = 0;
    for(unsigned i = 0; i < m_data.size(); ++i)
      if(m_data[i])
        ++count;
    return count;
  }
  void clear() { m_data.clear(); m_nViews = 0; m_nMaxVpId = -1; }
  void set(const OdGsBaseModel *pModel);
  bool contains(OdUInt32 viewportId) const
  {
    return (viewportId < m_data.size()) && (m_data[viewportId] != 0);
  }

  class const_iterator
  {
  public:
    const_iterator(const ViewRefs& refs): m_refs(refs), m_pos(-1){}
    const_iterator(const const_iterator& c): m_refs(c.m_refs), m_pos(c.m_pos){}
    void start() { m_pos = -1; findNext(); }
    bool done() const { return m_pos < 0 || m_pos >= (int)(m_refs.m_data.size()); }
    OdUInt32 viewportId() const { ODA_ASSERT(!done()); return m_pos; }
    void step() { findNext(); }

  protected:
    void findNext()
    {
      ODA_ASSERT(m_pos > -2);
      for(++m_pos; (m_pos < (int)m_refs.m_data.size()) && !m_refs.m_data[m_pos]; ++m_pos);
    }
  protected:
    const ViewRefs& m_refs;
    int m_pos;
  };

  friend class const_iterator;
protected:
  typedef OdIntArray _marray;
  _marray m_data;
  OdUInt32 m_nViews;
  mutable int m_nMaxVpId; // cache maxViewportId
};

/** \details
        
    Corresponding C++ library: TD_Gs

    <group OdGs_Classes> 
*/
class StockProps
{
public:
  StockProps(): m_deviceClass(0){}
  StockProps(const StockProps& c): m_viewRefs(c.m_viewRefs),
      m_viewProps(c.m_viewProps), m_deviceClass(c.m_deviceClass){}

  const ViewProps& viewProps(unsigned i) const { return m_viewProps.getAt(i); }
  unsigned numViewProps() const { return m_viewProps.size(); }
  const ViewRefs& viewRefs() const { return m_viewRefs; }
  const OdRxClass* deviceClass() const { return m_deviceClass; }

  bool set(const ViewRefs& refs, const ViewPropsArray& props,
      const OdRxClass* deviceClass)
  {
      if(!deviceClass)
          return false;//no device to identity cached data
      ODA_ASSERT(!isInvalid(props));
      m_viewRefs = refs;
      m_viewProps = props;
      m_deviceClass = deviceClass;
      return true;
  }
  void clear() { m_viewRefs.clear(); m_viewProps.clear(); }

protected:
  ViewRefs m_viewRefs;
  ViewPropsArray m_viewProps;
  const OdRxClass* m_deviceClass;
};

inline bool sameSortedArrays(const OdDbStubPtrArray& ar1, const OdDbStubPtrArray& ar2)
{
  if (ar1.size() == ar2.size())
  {
    if (ar1.getPtr() != ar2.getPtr())
      return ::memcmp(ar1.getPtr(), ar2.getPtr(), sizeof(OdDbStub*) * ar2.size()) == 0;
    else
      return true;
  }
  return false;
}

inline bool ViewProps::isCompatibleWith(
    const ViewProps& props, OdUInt32 nMask
    ,bool bCompareFrozenLayers
    ) const
{
  //NS: optimization
  if (!(nMask & ~kVpForceEraseMf) && !bCompareFrozenLayers)
    return true;
  if(GETBIT(nMask, kVpID))
  {
    if((m_vpId!=props.m_vpId) || (m_vpObjectId!=props.m_vpObjectId))
      return false;
  }
  if(GETBIT(nMask, kVpRegenType))
  {
    if(m_regenType!=props.m_regenType)
      return false;
  }
  if(GETBIT(nMask, kVpRenderMode))
  {
    if(m_renderMode!=props.m_renderMode)
      return false;
  }
  if(bCompareFrozenLayers && (GETBIT(nMask, kVpFrozenLayers) ||
     !sameSortedArrays(m_frozenLayers, props.m_frozenLayers)))
  {
    return false;
  }
  if(GETBIT(nMask, kVpFrontBack))
  {
    if(m_frontClip!=props.m_frontClip || m_backClip!=props.m_backClip)
      return false;
  }
  if(GETBIT(nMask, kVpCamLocation))
  {
    if(m_cameraLocation!=props.m_cameraLocation)
      return false;
  }
  if(GETBIT(nMask, kVpCamTarget))
  {
    if(m_cameraTarget!=props.m_cameraTarget)
      return false;
  }
  for (OdUInt32 nDeviation = kOdGiMaxDevForCircle; nDeviation <= kOdGiMaxDevForCurve; nDeviation++)
  {
    if(GETBIT(nMask, kVpMaxDevForCircle << nDeviation))
    {
      if(OdNegative(m_deviation[nDeviation] - props.m_deviation[nDeviation]))
        return false;
    }
  }
  //skip kOdGiMaxDevForBoundary and kOdGiMaxDevForIsoline
  if(GETBIT(nMask, kVpMaxDevForFacet))
  {
    if(OdNegative(m_deviation[kOdGiMaxDevForFacet] - props.m_deviation[kOdGiMaxDevForFacet]))
      return false;
  }
  if(GETBIT(nMask, kVpCamUpVector))
  {
    if(m_cameraUpVector!=props.m_cameraUpVector)
      return false;
  }
  if(GETBIT(nMask, kVpCamViewDir))
  {
    if(m_viewDir!=props.m_viewDir)
      return false;
  }
  if(GETBIT(nMask, kVpViewport))
  {
    if(m_vpLowerLeft!=props.m_vpLowerLeft || m_vpUpperRight!=props.m_vpUpperRight)
      return false;
  }
  if(GETBIT(nMask, kVpWorldToEye))
  {
    if(m_worldToEye!=props.m_worldToEye)
      return false;
  }
  if(GETBIT(nMask, kVpLtypeScaleMult))
  {
    if(!OdEqual(m_linetypeScaleMultiplier, props.m_linetypeScaleMultiplier))
      return false;
  }
  if(GETBIT(nMask, kVpRenderModule))
  {
    if(m_renderModule!=props.m_renderModule)
      return false;
  }
  if(GETBIT(nMask, kVpAnnoScale))
  {
    if(m_annoScale!=props.m_annoScale)
      return false;
  }
  if(GETBIT(nMask, kVpFilterFunction))
  {
    if(m_filterFunction!=props.m_filterFunction)
      return false;
  }
  if(GETBIT(nMask, kVpVisualStyle))
  {
    if(m_visualStyle!=props.m_visualStyle)
      return false;
  }
  return true;
}

#include "TD_PackPop.h"

#endif // __OD_GS_VIEW_PROPS__
