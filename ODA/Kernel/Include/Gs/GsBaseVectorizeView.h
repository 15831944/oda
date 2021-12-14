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

#ifndef ODGSBASEVECTORIZEVIEW_INC
#define ODGSBASEVECTORIZEVIEW_INC

#include "TD_PackPush.h"

#include <stdlib.h>
#include <utility>

#include "Gs/GsViewImpl.h"
#include "Ge/GeExtents3d.h"
#include "Gs/GsViewPropsDef.h"
#include "Gs/GsBaseVectorizeDevice.h"

/** \details
    This class is a pattern implementation of the OdGsView interface. 
    
    \remarks
    This class implements most of the OdGsView class.
    It creates simplified eye coordinate system (ECS) output graphics,
    which could be redirected by a client to a renderer to screen or file.
    
    \sa
    DWF/SVG exports
    
    Corresponding C++ library: Gs

    <group OdGs_Classes> 
*/
class GS_TOOLKIT_EXPORT OdGsBaseVectorizeView : public OdGsViewImpl
{
public:
  /** \details
    Default constructor for the OdGsBaseVectorizeView class.
  */
  OdGsBaseVectorizeView();
  
  /** \details
    Destructor for the OdGsBaseVectorizeView class.
  */
  ~OdGsBaseVectorizeView();

public:
  /** OdGsBaseVectorizeView methods *
  */

  /** \details
    Returns the specified OdGsView pointer 
    as a pointer to an OdGsBaseVectorizeView object.
    \param pView [in]  Pointer to an OdGsView object.
  */
  static OdGsBaseVectorizeView* safeCast(OdGsView* pView);

  /** OdGsViewImpl methods *
  */
  
  /** \details
    Retrieves context associated with this object.
    
    \returns
    Pointer to the associated context.
  */
  virtual OdGiContext* userGiContext() const;
  
  /** \details
    Sets context for this object.
    
    \param pUserGiContext [in]  User Gi context.
  */
  virtual void setUserGiContext(OdGiContext* pUserGiContext);
  
  /** \details
    Updates this vectorization view (propagates layer changes, propagates invalid viewport flag, updates view properties, updates viewpport and sets invalid and check for validation flags depending on the regenAbort() call).
  */
  virtual void update();

  /** \details
    Runs the selection procedure inside the view object.
    \param aPtDc      [in] A pointer to an array of selection points in the device coordinate space (pixels).
    \param numPoints  [in] A quantity of selection points.
    \param pReactor   [in] A pointer to a selection reactor object to process selections.
    \param mode       [in] Selection mode.
    \sa
    <link tv_working_with_features_selections, Work with Selections>
  */
  virtual void select(const OdGePoint2d* aPtDc, int numPoints, OdGsSelectionReactor* pReactor,
    OdGsView::SelectionMode mode = OdGsView::kCrossing);
  TD_USING(OdGsViewImpl::select);

  /** \details
    Returns the eye coordinate system extents of this view object.
    
    \param extents [out]  Receives the extents.
    \returns
    Returns true if the extents are defined.
  */
  virtual bool viewExtents(OdGeBoundBlock3d& extents) const;

  /** \details
    Returns the minimum and maximum z-values for this view object.
    \param zNear [out]  Receives the minimum z-value.
    \param zFar [out]  Receives the maximum z-value.
    \param nOverlay [in]  Overlay buffer index.
    \returns
    Returns true if this view object has z-values associated with it.
  */
  bool sceneDept(double& zNear, double& zFar, OdGsOverlayId nOverlay) const;
  
  /** \details
    Returns the minimum and maximum z-values for this view object.
    \param zNear [out]  Receives the minimum z-value.
    \param zFar [out]  Receives the maximum z-value.
    \returns
    Returns true if this view object has z-values associated with it.
    \remarks
    This is old version of sceneDept overrideable method, which is still provided for compatibility with
    old vectorizers. Don't override it in new vectorizers, use new sceneDept method version even if
    your vectorizer doesn't support overlay buffers. This method will be removed in future ODA releases.
  */
  bool sceneDept(double& zNear, double& zFar) const;

  /** \details
    Runs the collision detection procedure inside the view object.
    
    \param pInputList [in]  List of GiPathNode that represents input entities; can be NULL.
    \param nInputListSize [in]  Size of input list; can be 0.
    \param pReactor [in]  Pointer to the reactor that processes detected collisions; cannot be NULL.
    \param pCollisionWithList [in]  List of GiPathNode that represents entities to check with the input list for collisions; can be NULL.
    \param nCollisionWithListSize [in]  Size of list; can be NULL.
    \param pCtx [in]  Pointer to the context that provides additional collision detection properties; can be NULL.
    
    \sa
    <link gs_collision_detection, Work with Collision Detection>
    \sa
    <link tv_working_with_features_collision_detection, Work with Collision Detection>
  */
  virtual void collide( OdGiPathNode const*const* pInputList, OdUInt32 nInputListSize, OdGsCollisionDetectionReactor* pReactor, OdGiPathNode const*const* pCollisionWithList = NULL, OdUInt32 nCollisionWithListSize = 0, const OdGsCollisionDetectionContext* pCtx = NULL );
  TD_USING(OdGsViewImpl::collide);
protected:

  virtual void updateGeometryByDevice();

  /** OdGsBaseVectorizeView methods *
  */
public:
  /** \details
    Retrieves a number of vectorization threads.
  */
  virtual OdUInt32 numVectorizers() const = 0;

protected:
  virtual OdGsBaseVectorizer* getVectorizer(bool bDisplay) = 0;
  virtual void releaseVectorizer(OdGsBaseVectorizer* pVect) = 0;

  virtual void updateGeometry();
  virtual void updateScreen();

private:
  void setVectThreadIndex(OdGsBaseVectorizer* pVect, bool bAssignIndex, int idx = -1);

  //data
protected:
  OdSmartPtr<OdGiContext> m_userGiContext;

private:
  friend class OdGsBaseVectorizeDevice;
  friend class VectorizerAutoPtr;
  friend class BaseVectScheduler;
};

inline OdGsBaseVectorizeView* OdGsBaseVectorizeView::safeCast(OdGsView* pView)
{
  return static_cast<OdGsBaseVectorizeView*>(OdSmartPtr<OdGsViewImpl>(pView).get());
}

/** \details
        
    Corresponding C++ library: TD_Gs

    <group OdGs_Classes> 
*/
class VectorizerAutoPtr
{
public:
  /** \details
    Constructor for the VectorizerAutoPtr class. Creates an auto ptr based on the passed view vectorizer.
    
    \param view     [in]  View vectorizer for which to create an auto ptr.
    \param bDisplay [in]  Not used.
  */
  VectorizerAutoPtr(OdGsBaseVectorizeView& view, bool bDisplay): m_view(view), m_obj(NULL)
  {
    m_obj = m_view.getVectorizer(bDisplay);
    if(m_obj && bDisplay)
        m_view.setVectThreadIndex(m_obj, true);
  }
  
  /** \details
    Destructor for the VectorizerAutoPtr class. Creates an auto ptr based on the passed view vectorizer.
    
    \param view     [in]  View vectorizer for which to create an auto ptr.
    \param bDisplay [in]  Not used.
  */
  ~VectorizerAutoPtr()
  {
    if(m_obj)
    {
      m_view.setVectThreadIndex(m_obj, false);
      m_view.releaseVectorizer(m_obj);
    }
  }
  
  /** \details
    Arrow operator for the VectorizerAutoPtr class. Returns a pointer to a view vectorizer.
  */
  OdGsBaseVectorizer* operator ->()
  {
    return static_cast<OdGsBaseVectorizer*>(m_obj);
  }
  
  /** \details
    Arrow operator for the VectorizerAutoPtr class. Returns a pointer to a view vectorizer.
  */
  const OdGsBaseVectorizer* operator ->() const
  {
    return const_cast<const OdGsBaseVectorizer*>(m_obj);
  }

  /** \details
    Returns a pointer to a view vectorizer.
  */
  const OdGsBaseVectorizer* get() const
  {
    return const_cast<const OdGsBaseVectorizer*>(m_obj);
  }
  
  /** \details
    Returns a pointer to a view vectorizer.
  */
  OdGsBaseVectorizer* get()
  {
    return static_cast<OdGsBaseVectorizer*>(m_obj);
  }

protected:
  OdGsBaseVectorizeView& m_view;
  OdGsBaseVectorizer* m_obj;
};

#include "TD_PackPop.h"

#define IMPL_VECTORIZER_DEFAULT()\
  virtual OdUInt32 numVectorizers() const { return 1; }\
  virtual OdGsBaseVectorizer* getVectorizer(bool /*bDisplay*/)\
  { return this; }\
  virtual void releaseVectorizer(OdGsBaseVectorizer* pVect)\
  {ODA_ASSERT(pVect == (OdGsBaseVectorizer*)this); }

#define IMPL_VECTORIZER_DEFAULT_EX()\
  IMPL_VECTORIZER_DEFAULT()\
  virtual void clearLinetypeCache(){ OdGiBaseVectorizerImpl::clearLinetypeCache(); }\
  ODRX_USING_HEAP_OPERATORS(OdGiBaseVectorizer);

#endif // ODGSBASEVECTORIZEVIEW_INC
