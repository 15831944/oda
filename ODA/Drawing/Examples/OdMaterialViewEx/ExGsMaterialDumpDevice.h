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

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef ODExGsMaterialDumpDevice
#define ODExGsMaterialDumpDevice

// Include OdGsBaseMaterialView class.
#include "Gs/GsBaseMaterialView.h"
// Include OdGiGeometrySimplifier class.
#include "Gi/GiGeometrySimplifier.h"

#include <iostream>

// Comment this define, if you don't want use delay cache for texture coordinates.
#define OD_GSMAT_USEDELAYCACHE

/************************************************************************/
/* Simple data to console output helper class.                          */
/************************************************************************/
class ExDumpOutput : public OdRxObject
{
  int   m_indentLevel;
public:
  static OdSmartPtr<ExDumpOutput> createObject();

  ExDumpOutput(); // it isn't possible to create object using this constructor, use 
                  // above createObject method for such purpose
  ~ExDumpOutput();

  void output(const OdString& str);
  void output(const OdString& str1, const OdString& str2);

  void pushIndent() { m_indentLevel += 2; }
  void popIndent() { m_indentLevel -= 2; }
};

/************************************************************************/
/* This template class is a specialization of the OdSmartPtr class for  */
/* ExDumpOutput object pointers                                         */
/************************************************************************/
typedef OdSmartPtr<ExDumpOutput> ExDumpOutputPtr;

/************************************************************************/
/* OdGsBaseVectorizeDevice objects own, update, and refresh one or more */
/* OdGsView objects.                                                    */
/************************************************************************/
class ExGsMaterialDumpDevice : 
  public OdGsBaseVectorizeDevice
{
  ExDumpOutputPtr m_pDumper;

public:
  ExGsMaterialDumpDevice();

  /**********************************************************************/
  /* Creates vectorization device.                                      */
  /**********************************************************************/
  static OdGsDevicePtr createObject();

  /**********************************************************************/
  /* Called by the ODA vectorization framework to update                */
  /* the GUI window for this Device object                              */
  /*                                                                    */
  /* pUpdatedRect specifies a rectangle to receive the region updated   */
  /* by this function.                                                  */
  /*                                                                    */
  /* The override should call the parent version of this function,      */
  /* OdGsBaseVectorizeDevice::update().                                 */
  /**********************************************************************/
  void update(OdGsDCRect* pUpdatedRect);

  /**********************************************************************/
  /* Creates a new OdGsView object, and associates it with this Device  */
  /* object.                                                            */
  /*                                                                    */
  /* pInfo is a pointer to the Client View Information for this Device  */
  /* object.                                                            */
  /*                                                                    */
  /* bEnableLayerVisibilityPerView specifies that layer visibility      */
  /* per viewport is to be supported.                                   */
  /**********************************************************************/  
  OdGsViewPtr createView(const OdGsClientViewInfo* pInfo = 0, 
                         bool bEnableLayerVisibilityPerView = false);
  
  /**********************************************************************/
  /* Returns the geometry dumper associated with this object.           */
  /**********************************************************************/  
  ExDumpOutputPtr dumper();
}; // end ExGsMaterialDumpDevice

/************************************************************************/
/* This template class is a specialization of the OdSmartPtr class for  */
/* ExGsMaterialDumpDevice object pointers                               */
/************************************************************************/
typedef OdSmartPtr<ExGsMaterialDumpDevice> ExGsMaterialDumpDevicePtr;

/************************************************************************/
/* Example client view class that demonstrates how to receive           */
/* texture coordinates from the vectorization process.                  */
/************************************************************************/
class ExGsMaterialDumpView
  : public OdGsBaseMaterialView
  , public OdGiGeometrySimplifier
{
protected:
  bool m_bEnableDump; // Enable coordinates output
  bool m_bDumpTextureCoordinates; // Enable texture coordinates output
  OdUInt32 m_uNTriangle; // Count of currently vectorized triangles
#ifndef OD_GSMAT_USEDELAYCACHE
  OdGeExtents3d m_DrawableExtents; // Drawable extents computed before object rendering
#endif
protected:

  /**********************************************************************/
  /* Called by the ODA vectorization framework in beginning of          */
  /* vectorization process.                                             */
  /**********************************************************************/
  virtual void beginViewVectorization();

  /**********************************************************************/
  /* Returns the ExGsMaterialDumpDevice instance that owns this view.   */
  /**********************************************************************/
  ExGsMaterialDumpDevice* device()
  { 
    return (ExGsMaterialDumpDevice*)OdGsBaseVectorizeView::device(); 
  }

  ExGsMaterialDumpView()
    /*************************************************************************************************************/
    /* We initialize material view for process mappers and materials. kEnableDelayCache used to enable cache,    */
    /* which can be used to store texture coordinates which must be recalculated if entity extents is not        */
    /* known on first rendering step, so they calculation can be delayed and calculcated after entity rendering. */
    /*************************************************************************************************************/
#ifdef OD_GSMAT_USEDELAYCACHE
    : OdGsBaseMaterialView(kProcessMappersAndMaterials | kEnableDelayCache)
#else
    // We compute extents manually before entity drawing
    : OdGsBaseMaterialView(kProcessMappersAndMaterials)
#endif
    , m_bEnableDump(false)
    , m_bDumpTextureCoordinates(false)
    , m_uNTriangle(0)
  {
  }

  friend class ExGsMaterialDumpDevice;

  /**********************************************************************/
  /* PseudoConstructor                                                  */
  /**********************************************************************/
  static OdGsViewPtr createObject()
  {
    return OdRxObjectImpl<ExGsMaterialDumpView,OdGsView>::createObject();
  }
public:
  /**********************************************************************/
  /* Called by the ODA vectorization framework to vectorize             */
  /* each entity in this view.  This override allows a client           */
  /* application to examine each entity before it is vectorized.        */
  /* The override should call the parent version of this function,      */
  /* OdGsBaseVectorizeView::draw().                                     */
  /**********************************************************************/
  void draw(const OdGiDrawable* pDrawable);

  /**********************************************************************/
  /* Called by the OAD vectorization framework to give the              */
  /* client application a chance to terminate the current               */
  /* vectorization process.  Returning true from this function will     */
  /* stop the current vectorization operation.                          */
  /**********************************************************************/
  bool regenAbort() const;

  /**********************************************************************/
  /* Flushes any queued graphics to the display device.                 */
  /**********************************************************************/
  void update();
  
  TD_USING(OdGsBaseMaterialView::update);

  /**********************************************************************/
  /* Notification function called by the vectorization framework        */
  /* whenever the rendering attributes have changed.                    */
  /*                                                                    */
  /* Retrieves the current vectorization traits (color,                 */
  /* lineweight, etc.) and sets them in this device.                    */
  /**********************************************************************/
  virtual void onTraitsModified();

  /**********************************************************************/
  /* Called by OdGiGeometrySimplifier for each output triangle.         */
  /**********************************************************************/
  virtual void triangleOut(const OdInt32* vertices,
                           const OdGeVector3d* pNormal);
  /**********************************************************************/
  /* Called by OdGiGeometrySimplifier for each output polyline.         */
  /**********************************************************************/
  virtual void polylineOut(OdInt32 nVertices,
                           const OdGePoint3d* pPoints);

#ifdef OD_GSMAT_USEDELAYCACHE
  /**********************************************************************/
  /* This callback method called by OdGsBaseMaterialView for delayed    */
  /* entries in cache by default.                                       */
  /**********************************************************************/
  virtual void playDelayCacheEntry(const DelayCacheEntry *pEntry);
#else
  /**********************************************************************/
  /* This callback method called by OdGsBaseMaterialView for calculate  */
  /* entity extents when they are needed for texture mapping if delay   */
  /* cache disabled.                                                    */
  /**********************************************************************/
  virtual bool computeDrawableExtents(const OdGiDrawable *pDrawable, OdGeExtents3d &extents);
#endif

  /**********************************************************************/
  /* Called by OdGsBaseMaterialView for fill cache for each material.   */
  /* prevCache - Previous cache pointer if it available.                */
  /* materialId - Id of currently processing material.                  */
  /* materialData - Current material traits container.                  */
  /**********************************************************************/
  virtual OdGiMaterialItemPtr fillMaterialCache(OdGiMaterialItemPtr prevCache, OdDbStub *materialId, const OdGiMaterialTraitsData &materialData);
  /**********************************************************************/
  /* Called by OdGsBaseMaterialView for render currently selected       */
  /* material cache.                                                    */
  /* pCache - Pointer to currently processing cache.                    */
  /* materialId - Id of currently processing material.                  */
  /**********************************************************************/
  virtual void renderMaterialCache(OdGiMaterialItemPtr pCache, OdDbStub *materialId);
}; // end ExGsMaterialDumpView 

#endif 
