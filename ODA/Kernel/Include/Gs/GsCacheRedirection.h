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

#ifndef __OD_GS_NODE_REDIRECTION__
#define __OD_GS_NODE_REDIRECTION__

#include "TD_PackPush.h"

#include "Gs/GsModel.h"
#include "OdVector.h"
#include "ChunkAllocator.h"
#include "Ge/GeMatrix3d.h"
#include "Ge/GeExtents3d.h"
#define STL_USING_SET
#include "OdaSTL.h"

// Forward declarations
class OdGsCacheRedirectionManager;

/** \details
    This class used to redirect OdGsModel interface calls into set of underlying OdGsModel's.

    Corresponding C++ library: TD_Gs
    <group OdGs_Classes> 
*/
template <typename Client, typename Base = OdGsModel>
class OdGsModelRedirectionWrapper : public Base
{
  protected:
    Client *m_pRedirectionClient;
  public:
    OdGsModelRedirectionWrapper(Client *pClient = NULL) : m_pRedirectionClient(pClient) { }

    /** \details
      Sets redirection client.

      \param pClient [in]  Redirection client.
    */
    void setRedirectionClient(Client *pClient) { m_pRedirectionClient = pClient; }
  public:
    /** \details
      Specifies the function called for drawable object open by underlying Model objects.

      \param openDrawableFn [in]  Function to be called.
    */
    virtual void setOpenDrawableFn(OdGiOpenDrawableFn openDrawableFn)
    {
      for (OdUInt32 nModel = 0; nModel < m_pRedirectionClient->numModels(); nModel++)
        m_pRedirectionClient->gsModelAt(nModel)->setOpenDrawableFn(openDrawableFn);
    }

    /** \details
      Notification function called whenever a drawable object is added to underlying Model objects.

      \param pAdded [in]  Pointer to the added Drawable object.
      \param pParent [in]  Pointer to the parent of the added Drawable object.
    */
    virtual void onAdded(OdGiDrawable* pAdded, OdGiDrawable* pParent)
    {
      for (OdUInt32 nModel = 0; nModel < m_pRedirectionClient->numModels(); nModel++)
        m_pRedirectionClient->gsModelAt(nModel)->onAdded(pAdded, pParent);
    }

    /** \details
      Notification function called whenever a drawable object is added to underlying Model objects.

      \param pAdded [in]  Pointer to the added Drawable object.
      \param parentID [in]  Object ID of the parent of the added Drawable object.
    */
    virtual void onAdded(OdGiDrawable* pAdded, OdDbStub* parentID)
    {
      for (OdUInt32 nModel = 0; nModel < m_pRedirectionClient->numModels(); nModel++)
        m_pRedirectionClient->gsModelAt(nModel)->onAdded(pAdded, parentID);
    }

    /** \details
      Notification function called whenever a drawable object is modified in underlying Model objects.

      \param pModified [in]  Pointer to the modified Drawable object.
      \param pParent [in]  Pointer to the parent of the modified Drawable object.
    */
    virtual void onModified(OdGiDrawable* pModified, OdGiDrawable* pParent)
    {
      for (OdUInt32 nModel = 0; nModel < m_pRedirectionClient->numModels(); nModel++)
        m_pRedirectionClient->gsModelAt(nModel)->onModified(pModified, pParent);
    }

    /** \details
      Notification function called whenever a drawable object is modified in underlying Model objects.

      \param pModified [in]  Pointer to the modified Drawable object.
      \param parentID [in]  Object ID of the parent of the modified Drawable object.
    */
    virtual void onModified(OdGiDrawable* pModified, OdDbStub* parentID)
    {
      for (OdUInt32 nModel = 0; nModel < m_pRedirectionClient->numModels(); nModel++)
        m_pRedirectionClient->gsModelAt(nModel)->onModified(pModified, parentID);
    }

    /** \details
      Function to call directly whenever a drawable object's graphics has been modified
      but not the drawable itself

      \param pModified [in]  Pointer to the Drawable object with modified graphics
      \param parentID [in]   Object ID of the parent of the modified Drawable object.
    */
    virtual void onModifiedGraphics(const OdGiDrawable* pModified, OdDbStub* parentID)
    {
      for (OdUInt32 nModel = 0; nModel < m_pRedirectionClient->numModels(); nModel++)
        m_pRedirectionClient->gsModelAt(nModel)->onModifiedGraphics(pModified, parentID);
    }

    /** \details
      Notification function called whenever a drawable object is erased from underlying Model objects.

      \param pErased [in]  Pointer to the erased Drawable object.
      \param pParent [in]  Pointer to the parent of the erased Drawable object.
    */
    virtual void onErased(OdGiDrawable* pErased, OdGiDrawable* pParent)
    {
      for (OdUInt32 nModel = 0; nModel < m_pRedirectionClient->numModels(); nModel++)
        m_pRedirectionClient->gsModelAt(nModel)->onErased(pErased, pParent);
    }

    /** \details
      Notification function called whenever a drawable object is erased from underlying Model objects.

      \param pErased [in]  Pointer to the erased Drawable object.
      \param parentID [in]  Object ID of the parent of the erased Drawable object.
    */
    virtual void onErased(OdGiDrawable* pErased, OdDbStub* parentID)
    {
      for (OdUInt32 nModel = 0; nModel < m_pRedirectionClient->numModels(); nModel++)
        m_pRedirectionClient->gsModelAt(nModel)->onErased(pErased, parentID);
    }

    /** \details
      Notification function called whenever a drawable object is unerased in underlying Model objects.

      \param pUnerased [in]  Pointer to the unerased Drawable object.
      \param pParent [in]  Pointer to the parent of the unerased Drawable object.
    */
    virtual void onUnerased(OdGiDrawable* pUnerased, OdGiDrawable* pParent)
    {
      for (OdUInt32 nModel = 0; nModel < m_pRedirectionClient->numModels(); nModel++)
        m_pRedirectionClient->gsModelAt(nModel)->onUnerased(pUnerased, pParent);
    }

    /** \details
      Notification function called whenever a drawable object is unerased in underlying Model objects.

      \param pUnerased [in]  Pointer to the unerased Drawable object.
      \param parentID [in]  Object ID of the parent of the unerased Drawable object.
    */ 
    virtual void onUnerased(OdGiDrawable* pUnerased, OdDbStub* parentID)
    {
      for (OdUInt32 nModel = 0; nModel < m_pRedirectionClient->numModels(); nModel++)
        m_pRedirectionClient->gsModelAt(nModel)->onUnerased(pUnerased, parentID);
    }

    /** \details
      Invalidates the specified cached data contained in underlying Model objects.

      \param hint [in]  Invalidation hint.

      \remarks
      Hint must be one of the following:

      <table>
      Name                        Value
      kInvalidateIsolines         0
      kInvalidateViewportCache    1
      kInvalidateAll              2
      kInvalidateMaterials        3
      kInvalidateLinetypes        4
      </table>
    */
    virtual void invalidate(OdGsModel::InvalidationHint hint)
    {
      for (OdUInt32 nModel = 0; nModel < m_pRedirectionClient->numModels(); nModel++)
        m_pRedirectionClient->gsModelAt(nModel)->invalidate(hint);
    }

    /** \details
      Invalidates cached data associated with the specified View object.

      \param pView [in]  Pointer to the OdGsView object for which data is to be invalidated.
    */
    virtual void invalidate(OdGsView* pView)
    { // @@@TODO: check that this view registered in underlying model
      for (OdUInt32 nModel = 0; nModel < m_pRedirectionClient->numModels(); nModel++)
        m_pRedirectionClient->gsModelAt(nModel)->invalidate(pView);
    }

    /** \details
      Invalidates cached data that visible on the device.

      \param pView [in]  Pointer to the OdGsDevice object for which data is to be invalidated.
    */
    virtual void invalidateVisible(OdGsDevice* pDevice)
    { // @@@TODO: check that this device registered in underlying model
      for (OdUInt32 nModel = 0; nModel < m_pRedirectionClient->numModels(); nModel++)
        m_pRedirectionClient->gsModelAt(nModel)->invalidateVisible(pDevice);
    }

    /** \details
      Transforms underlying Model objects by applying the specified transformation matrix to the model contents.

      \param xForm [in]  Transformation matrix.
    */
    virtual void setTransform(const OdGeMatrix3d& xForm)
    {
      for (OdUInt32 nModel = 0; nModel < m_pRedirectionClient->numModels(); nModel++)
        m_pRedirectionClient->gsModelAt(nModel)->setTransform(xForm);
    }

    /** \details
      Obtains the current transformation matrix of underlying Model object.
    */
    virtual OdGeMatrix3d transform() const
    { // ! This getter can return conflicting states
      return m_pRedirectionClient->gsModelAt(0)->transform();
    }

    /** \details
      Highlights the specified entity in underlying Model objects.

      \param path [in]  Path to the entity for highlighting.
      \param bDoIt [in]  Flag that indicates whether highlighting is enabled (true value) or disabled (false value).
      \param nStyle [in]  Selection style index.
      \param pView [in]  Pointer to the view where the specified entity should be highlighted.
    */
    virtual void highlight(const OdGiPathNode& path, bool bDoIt = true, OdUInt32 nStyle = 0, const OdGsView* pView = NULL)
    {
      for (OdUInt32 nModel = 0; nModel < m_pRedirectionClient->numModels(); nModel++)
        m_pRedirectionClient->gsModelAt(nModel)->highlight(path, bDoIt, nStyle, pView);
    }

    /** \details
      Highlights the specified subentities of entity in underlying Model objects.

      \param path [in]  Path to the entity 
      \param pMarkers [in] Array of subentity markers
      \param nMarkers [in] Count of subentity markers
      \param bDoIt [in]  Flag that indicates whether highlighting is enabled (true value) or disabled (false value).
      \param nStyle [in]  Selection style index.
      \param pView [in]  Pointer to the view where the specified entity should be highlighted.
    */
    virtual void highlight(const OdGiPathNode& path, const OdGsMarker* pMarkers, OdUInt32 nMarkers, bool bDoIt = true,
                           OdUInt32 nStyle = 0, const OdGsView* pView = NULL)
    {
      for (OdUInt32 nModel = 0; nModel < m_pRedirectionClient->numModels(); nModel++)
        m_pRedirectionClient->gsModelAt(nModel)->highlight(path, pMarkers, nMarkers, bDoIt, nStyle, pView);
    }

    /** \details
      Hides the specified entity in underlying Model objects.

      \param path [in]  Path to the entity for hide.
      \param bDoIt [in]  Flag that indicates whether hiding is enabled (true value) or disabled (false value).
      \param bSelectHidden [in] Flag that indicates whether hidden object is selectable or not.
      \param pView [in]  Pointer to the view where the specified entity should be hidden.
    */
    virtual void hide(const OdGiPathNode& path, bool bDoIt = true, bool bSelectHidden = false, const OdGsView* pView = NULL)
    {
      for (OdUInt32 nModel = 0; nModel < m_pRedirectionClient->numModels(); nModel++)
        m_pRedirectionClient->gsModelAt(nModel)->hide(path, bDoIt, bSelectHidden, pView);
    }

    /** \details
      Hides the specified subentities of entity in underlying Model objects.

      \param path [in]  Path to the entity for hide.
      \param pMarkers [in] Array of subentity markers.
      \param nMarkers [in] Count of subentity markers.
      \param bDoIt [in]  Flag that indicates whether hiding is enabled (true value) or disabled (false value).
      \param bSelectHidden [in] Flag that indicates whether hidden object is selectable or not.
      \param pView [in]  Pointer to the view where the specified entity should be hidden.
    */
    virtual void hide(const OdGiPathNode& path, const OdGsMarker* pMarkers, OdUInt32 nMarkers, bool bDoIt = true,
                      bool bSelectHidden = false, const OdGsView* pView = NULL)
    {
      for (OdUInt32 nModel = 0; nModel < m_pRedirectionClient->numModels(); nModel++)
        m_pRedirectionClient->gsModelAt(nModel)->hide(path, pMarkers, nMarkers, bDoIt, bSelectHidden, pView);
    }

    /** \details
      Sets the render type for underlying Model objects.

      \param renderType [in]  Render type.
    */
    virtual void setRenderType(OdGsModel::RenderType renderType)
    {
      for (OdUInt32 nModel = 0; nModel < m_pRedirectionClient->numModels(); nModel++)
        m_pRedirectionClient->gsModelAt(nModel)->setRenderType(renderType);
    }

    /** \details
      Returns the render type of underlying Model object.
    */
    virtual OdGsModel::RenderType renderType() const
    { // ! This getter can return conflicting states
      return m_pRedirectionClient->gsModelAt(0)->renderType();
    }

    //virtual bool addSceneGraphRoot(OdGiDrawable* pRoot) = 0;
    //virtual bool eraseSceneGraphRoot(OdGiDrawable* pRoot) = 0;

    /** \details
      Sets the specified rendering mode override for underlying Model objects.

      \param mode [in]  Render mode override.

      \remarks
      Rendering mode specified as OdGsView::kNone cancels currently set rendering mode override.
    */
    virtual void setRenderModeOverride(OdGsView::RenderMode mode = OdGsView::kNone)
    {
      for (OdUInt32 nModel = 0; nModel < m_pRedirectionClient->numModels(); nModel++)
        m_pRedirectionClient->gsModelAt(nModel)->setRenderModeOverride(mode);
    }

    /** \details
      Returns rendering mode override, specified for underlying Model object.

      \remarks
      Returns OdGsView::kNone in case if rendering mode overide doesn't specified.
    */
    virtual OdGsView::RenderMode renderModeOverride() const
    { // ! This getter can return conflicting states
      return m_pRedirectionClient->gsModelAt(0)->renderModeOverride();
    }

    /** \details
      Enables underlying model's geometry to ignore the view's front and back clipping planes (if any) and viewport 3d clipping (if set).
      If the method's input value is false, the view's clipping planes and viewport 3d clipping are allowed by underlying Model objects.

      \param bEnable [in]  Boolean flag which indicates that the model ignores the view's clipping planes and viewport 3d clipping (true value).
    */
    virtual void setViewClippingOverride(bool bEnable)
    {
      for (OdUInt32 nModel = 0; nModel < m_pRedirectionClient->numModels(); nModel++)
        m_pRedirectionClient->gsModelAt(nModel)->setViewClippingOverride(bEnable);
    }

    /** \details
      Returns true in case if view clipping override enabled for underlying Model objects, elsewhere returns false.
    */
    virtual bool viewClippingOverride() const
    { bool bRVal = false;
      for (OdUInt32 nModel = 0; nModel < m_pRedirectionClient->numModels(); nModel++)
        bRVal |= m_pRedirectionClient->gsModelAt(nModel)->viewClippingOverride();
      return bRVal;
    }

    /** \details
      Sets the background of underlying Model objects.

      \param backgroundId [in]  OdDbBackground object ID.
    */
    virtual void setBackground(OdDbStub *backgroundId)
    {
      for (OdUInt32 nModel = 0; nModel < m_pRedirectionClient->numModels(); nModel++)
        m_pRedirectionClient->gsModelAt(nModel)->setBackground(backgroundId);
    }

    /** \details
      Returns the OdDbBackground object ID if the background was set to underlying Model object.
    */
    virtual OdDbStub *background() const
    { // ! This getter can return conflicting states
      return m_pRedirectionClient->gsModelAt(0)->background();
    }

    /** \details
      Sets the visual style for underlying Model objects. This method is used to direct views 
      to display the model in the specified visual style. By default, models are displayed 
      in the visual style assigned to the view.

      \param visualStyleId [in]  Visual style object ID used by objects in underlying models

      \remarks
      Specify the OdDbObjectId::kNull as an input value to clear the model's visual style
      and use the view visual style for displaying the model's objects.
    */
    virtual void setVisualStyle(OdDbStub *visualStyleId)
    {
      for (OdUInt32 nModel = 0; nModel < m_pRedirectionClient->numModels(); nModel++)
        m_pRedirectionClient->gsModelAt(nModel)->setVisualStyle(visualStyleId);
    }

    /** \details
      Returns the visual style of underlying Model object.
    */
    virtual OdDbStub *visualStyle() const
    { // ! This getter can return conflicting states
      return m_pRedirectionClient->gsModelAt(0)->visualStyle();
    }

    /** \details
      Sets the visual style for underlying Model objects. This method is used to direct views 
      to display the model in the specified visual style. By default, models are displayed 
      in the visual style assigned to the view.

      \param visualStyle [in]  Visual style object
    */
    virtual void setVisualStyle(const OdGiVisualStyle &visualStyle)
    {
      for (OdUInt32 nModel = 0; nModel < m_pRedirectionClient->numModels(); nModel++)
        m_pRedirectionClient->gsModelAt(nModel)->setVisualStyle(visualStyle);
    }

    /** \details
      Returns the visual style of underlying Model object. 

      \param visualStyle [out]  Visual style

      \returns Returns true if the visual style is set for the model, false otherwise.
    */
    virtual bool visualStyle(OdGiVisualStyle &visualStyle) const
    { // ! This getter can return conflicting states
      return m_pRedirectionClient->gsModelAt(0)->visualStyle(visualStyle);
    }

    /** \details
      Adds a model reactor for underlying Model objects. 

      \param pReactor [in]  Model reactor
    */
    virtual void addModelReactor(OdGsModelReactor *pReactor)
    {
      for (OdUInt32 nModel = 0; nModel < m_pRedirectionClient->numModels(); nModel++)
        m_pRedirectionClient->gsModelAt(nModel)->addModelReactor(pReactor);
    }

    /** \details
      Removes the specified model reactor from underlying Model objects. 

      \param pReactor [in]  Pointer to the model reactor that should be removed
    */
    virtual void removeModelReactor(OdGsModelReactor *pReactor)
    {
      for (OdUInt32 nModel = 0; nModel < m_pRedirectionClient->numModels(); nModel++)
        m_pRedirectionClient->gsModelAt(nModel)->removeModelReactor(pReactor);
    }

    /** \details
      Enables the currently specified sectioning in the views displaying underlying Model objects.

      \param bEnable [in]  Boolean flag which indicates whether sectioning should be enabled
    */
    virtual void setEnableSectioning(bool bEnable)
    {
      for (OdUInt32 nModel = 0; nModel < m_pRedirectionClient->numModels(); nModel++)
        m_pRedirectionClient->gsModelAt(nModel)->setEnableSectioning(bEnable);
    }

    /** \details
      Returns true if the sectioning is active in the views displaying underlying Model objects.
      Returns false in the other case.
    */
    virtual bool isSectioningEnabled() const
    { bool bRVal = false;
      for (OdUInt32 nModel = 0; nModel < m_pRedirectionClient->numModels(); nModel++)
        bRVal |= m_pRedirectionClient->gsModelAt(nModel)->isSectioningEnabled();
      return bRVal;
    }

    /** \details
      Sets the active sectioning for underlying Model objects. After specifying the sectioning, 
      it should be enabled by the setEnableSectioning() method. 

      \param points [in]  Array of co-planar points specifying a sectioning region
      \param upVector [in]  Vector that specifies the orientation of the plane that contains points

      \remarks
      If the last point of the points array is equal to the first point of this array, the sectioning region is closed.
      The upVector is used to specify the clipped and unclipped sides of the sectioning. The unclipped side 
      is determined by the direction of the cross product of the first line segment defined by the point array, and the upVector.
    */
    virtual bool setSectioning(const OdGePoint3dArray &points, const OdGeVector3d &upVector)
    { bool bRVal = false;
      for (OdUInt32 nModel = 0; nModel < m_pRedirectionClient->numModels(); nModel++)
        bRVal |= m_pRedirectionClient->gsModelAt(nModel)->setSectioning(points, upVector);
      return bRVal;
    }

    /** \details
      Sets the active sectioning for underlying Model objects. The sectioning is capped vertically using 
      the specified top and bottom heights. After specifying the sectioning, it should be enabled 
      by the setEnableSectioning() method.

      \param points [in]  Array of co-planar points specifying a sectioning region
      \param upVector [in]  Vector that specifies the orientation of the plane that contains points
      \param dTop [in]  Specifies the top capping height of the sectioning
      \param dBottom [in]  Specifies the bottom capping height of the sectioning

      \returns Returns true if the specified sectioning is valid. Returns false 
      if the specified sectioning is invalid or malformed.

      \remarks
      If the last point of the points array is equal to the first point of this array,
      the sectioning region is closed. The upVector is used to specify the clipped and unclipped
      sides of the sectioning. The unclipped side is determined by the direction of the
      cross product of the first line segment defined by the point array, and the upVector.
    */
    virtual bool setSectioning(const OdGePoint3dArray &points, const OdGeVector3d &upVector,
                               double dTop, double dBottom)
    { bool bRVal = false;
      for (OdUInt32 nModel = 0; nModel < m_pRedirectionClient->numModels(); nModel++)
        bRVal |= m_pRedirectionClient->gsModelAt(nModel)->setSectioning(points, upVector, dTop, dBottom);
      return bRVal;
    }

    /** \details
      Sets the visual style for the objects on the clipped side of the sectioning. 

      \param visualStyleId [in]  Visual style object ID. Null by default.

      \remarks
      If the input parameter is a null object ID, the clipped side should not be drawn.
    */
    virtual void setSectioningVisualStyle(OdDbStub *visualStyleId)
    {
      for (OdUInt32 nModel = 0; nModel < m_pRedirectionClient->numModels(); nModel++)
        m_pRedirectionClient->gsModelAt(nModel)->setSectioningVisualStyle(visualStyleId);
    }

    /** \details
      Enables or disables linetype display in any OdGsView object that views underlying Model objects.  

      \param bEnable [in]  Boolean flag which enables (true value) or disables (false value) linetype display.
    */
    virtual void setEnableLinetypes(bool bEnable)
    {
      for (OdUInt32 nModel = 0; nModel < m_pRedirectionClient->numModels(); nModel++)
        m_pRedirectionClient->gsModelAt(nModel)->setEnableLinetypes(bEnable);
    }

    /** \details
      Returns true if the linetype display is enabled or returns false in the other case.
    */
    virtual bool isLinetypesEnabled() const
    { bool bRVal = false;
      for (OdUInt32 nModel = 0; nModel < m_pRedirectionClient->numModels(); nModel++)
        bRVal |= m_pRedirectionClient->gsModelAt(nModel)->isLinetypesEnabled();
      return bRVal;
    }

    /** \details
      Enables or disables the ability to select entities inside underlying Model objects. If the input value 
      is true, entities inside underlying Model objects is selectable. If the input value is false, 
      entities inside underlying Model objects is not selectable.

      \param bEnable [in]  Boolean flag which indicates whether underlying Model objects is selectable.
    */
    virtual void setSelectable(bool bEnable)
    {
      for (OdUInt32 nModel = 0; nModel < m_pRedirectionClient->numModels(); nModel++)
        m_pRedirectionClient->gsModelAt(nModel)->setSelectable(bEnable);
    }

    /** \details
      Checks whether entities inside underlying Model objects is selectable. If the returning value is true,
      entities inside underlying Model objects is selectable. If the returning value is false,
      entities inside underlying Model objects is not selectable.
    */
    virtual bool isSelectable() const
    { bool bRVal = false;
      for (OdUInt32 nModel = 0; nModel < m_pRedirectionClient->numModels(); nModel++)
        bRVal |= m_pRedirectionClient->gsModelAt(nModel)->isSelectable();
      return bRVal;
    }

    /** \details
      Enables or disables extents calculation for entities inside underlying Model objects. If the input value
      is true, entities inside underlying Model objects will be used during view extents calculation.
      If the input value is false, entities inside underlying Model objects will be not used in view extents calculations.

      \param bEnable [in]  Boolean flag which indicates that Model entities which will be used during view extents calculation.
    */
    virtual void setEnableViewExtentsCalculation(bool bEnable)
    {
      for (OdUInt32 nModel = 0; nModel < m_pRedirectionClient->numModels(); nModel++)
        m_pRedirectionClient->gsModelAt(nModel)->setEnableViewExtentsCalculation(bEnable);
    }

    /** \details
      Checks whether entities inside underlying Model objects is used for view extents calculation. If the returning value is true,
      entities inside underlying Model objects will be used in view extents calculation. If the returning value is false,
      entities inside underlying Model objects will be not used in view extents calculation.
    */
    virtual bool isViewExtentsCalculationEnabled() const
    { bool bRVal = false;
      for (OdUInt32 nModel = 0; nModel < m_pRedirectionClient->numModels(); nModel++)
        bRVal |= m_pRedirectionClient->gsModelAt(nModel)->isViewExtentsCalculationEnabled();
      return bRVal;
    }

    /** \details
      Enables or disables lights in blocks.

      \param bEnable [in]  Boolean flag which enables (true value) or disables (false value) lights in blocks.
    */
    virtual void setEnableLightsInBlocks(bool bEnable)
    {
      for (OdUInt32 nModel = 0; nModel < m_pRedirectionClient->numModels(); nModel++)
        m_pRedirectionClient->gsModelAt(nModel)->setEnableLightsInBlocks(bEnable);
    }

    /** \details
      Returns true if lights in blocks are enabled or returns false in the other case.
    */
    virtual bool isLightsInBlocksEnabled() const
    { bool bRVal = false;
      for (OdUInt32 nModel = 0; nModel < m_pRedirectionClient->numModels(); nModel++)
        bRVal |= m_pRedirectionClient->gsModelAt(nModel)->isLightsInBlocksEnabled();
      return bRVal;
    }

    /** \details
      Enables or disables view sectioning override. In case if view sectioning override enabled, sectioning, which is enabled
      for View object (which is represents underlying Model objects), will be ignored for Drawable objects in underlying Model objects.

      \param bEnable [in]  Boolean flag which enabled (true value) or disables (false value) view sectioning override in underlying Model objects.
    */
    virtual void setViewSectioningOverride(bool bEnable)
    {
      for (OdUInt32 nModel = 0; nModel < m_pRedirectionClient->numModels(); nModel++)
        m_pRedirectionClient->gsModelAt(nModel)->setViewSectioningOverride(bEnable);
    }

    /** \details
      Returns current state of view sectioning override (true value in case if view sectioning override enabled or false elsewhere).
    */
    virtual bool viewSectioningOverride() const
    { bool bRVal = false;
      for (OdUInt32 nModel = 0; nModel < m_pRedirectionClient->numModels(); nModel++)
        bRVal |= m_pRedirectionClient->gsModelAt(nModel)->viewSectioningOverride();
      return bRVal;
    }
};

/** \details
    Abstract references counter helper.

    Corresponding C++ library: TD_Gs
    <group OdGs_Classes> 
*/
template <typename BaseClass>
class OdGsRedirectionHelper : public BaseClass
{
  protected:
    OdRefCounter m_refCounter;
  public:
    /** \details
      Constructor sets the reference counter to one.
    */
    OdGsRedirectionHelper() : m_refCounter(1) { }
    /** \details
      Increments the reference counter.
    */
    void addRef() { ++m_refCounter; }
    /** \details
      Decrements the reference counter, checks the reference counter, and destroys the instance 
      when the reference counter reaches zero.
    */
    void release()
    {
      ODA_ASSERT((m_refCounter > 0));
      if (!(--m_refCounter))
        delete this;
    }
    /** \details
      Returns the number of references to an instance of this class.
    */
    long numRefs() const { return m_refCounter; }
};

/** \details
    This class used to redirect OdGsModel interface calls into set of underlying OdGsModel's.

    Corresponding C++ library: TD_Gs
    <group OdGs_Classes> 
*/
class GS_TOOLKIT_EXPORT OdGsModelRedirectionHandler : public OdGsRedirectionHelper<OdGsModelRedirectionWrapper<OdGsModelRedirectionHandler> >
{
  protected:
    OdUInt32 m_nModels;
    OdRefCounter m_nReferences;
    OdGsCacheRedirectionManager *m_pManager;
    OdGsModel *m_pModel[1];
    friend OdGsCacheRedirectionManager;
  protected:
    static OdGsModelRedirectionHandler *allocate(OdUInt32 nModels);
  public:
    OdGsModelRedirectionHandler() : m_nModels(0), m_nReferences(0), m_pManager(NULL) { setRedirectionClient(this); }
    ~OdGsModelRedirectionHandler() { }
    ODRX_DECLARE_MEMBERS(OdGsModelRedirectionHandler);

    /** \details
      Create new OdGsModelRedirectionHandler object for set of models.

      \param nModels [in]  Number of underlying OdGsModel's.
      \param pModels [in]  Array of underlying OdGsModel's.
      \param pManager [in]  Owning cache redirection manager.
    */
    static OdGsModelPtr createObject(OdUInt32 nModels, const OdGsModelPtr *pModels, OdGsCacheRedirectionManager *pManager);
    static OdGsModelPtr createObject(OdUInt32 nModels, const OdGsModel   **pModels, OdGsCacheRedirectionManager *pManager);

    /** \details
      Returns number of underlying OdGsModel's.
    */
    OdUInt32 numModels() const { return m_nModels; }

    /** \details
      Return underlying OdGsModel by index.

      \param nModel [in]  Index of model.
    */
    OdGsModel *gsModelAt(OdUInt32 nModel) { return m_pModel[nModel]; }
    const OdGsModel *gsModelAt(OdUInt32 nModel) const { return m_pModel[nModel]; }

    /** \details
      Sets underlying OdGsModel by specified index.

      \param nModel [in]  Index of model.
      \param pModel [in]  OdGsModel pointer.
    */
    void setGsModelAt(OdUInt32 nModel, const OdGsModel *pModel) { m_pModel[nModel] = OdGsModelPtr(pModel); }

    /** \details
      Returns owning cache redirection manager.
    */
    OdGsCacheRedirectionManager *cacheRedirectionManager() const { return m_pManager; }
};

/** \details
    This class used to redirect OdGsCache interface calls into set of underlying OdGsCache's.

    Corresponding C++ library: TD_Gs
    <group OdGs_Classes> 
*/
class GS_TOOLKIT_EXPORT OdGsCacheRedirectionNode : public OdGsRedirectionHelper<OdGsCache>
{
  protected:
    GS_STATIC_EXPORT static AllocatorArray s_aAlloc;
    OdGsModelRedirectionHandler *m_pModels;
    OdGsCache                   *m_pNode[1];
  protected:
    static OdGsCacheRedirectionNode *allocate(OdGsModel *pModel);
  public:
    OdGsCacheRedirectionNode() : m_pModels(NULL) { }
    ~OdGsCacheRedirectionNode() { }
    ODRX_DECLARE_MEMBERS(OdGsCacheRedirectionNode);

    void* operator new(size_t);
    void operator delete(void *);

    /** \details
      Create new OdGsCacheRedirectionNode object for set of GsCache's.

      \param pModel [in]  Encapsulates underlying set of OdGsModel's.
      \param pNodes [in]  Array of underlying OdGsCache's.
    */
    static OdSmartPtr<OdGsCache> createObject(const OdGsModel *pModel, OdGsCache **pNodes);

    /** \details
      Returns number of underlying OdGsCache's.
    */
    OdUInt32 numNodes() const { return m_pModels->numModels(); }

    /** \details
      Return underlying OdGsCache by index.

      \param nModel [in]  Index of model.
    */
    OdGsCache *gsNodeAt(OdUInt32 nNode) const { return m_pNode[nNode]; }

    /** \details
      Sets underlying OdGsCache by specified index.

      \param nNode [in]  Index of cache.
      \param pNode [in]  OdGsCache pointer.
    */
    void setGsNodeAt(OdUInt32 nNode, OdGsCache *pNode) { m_pNode[nNode] = pNode; }

    /** \details
      Pointer to the OdGsModel object associated with this Cache object.
    */
    virtual OdGsModel* model() const { return m_pModels; }

    /** \details
      Reset pointer to the underlying GiDrawable object associated with this Cache object.
    */
    virtual void setDrawableNull();

    /** \details
      Returns the extents of the underlying GiDrawable object(s) associated with this Cache object.

      \param extents [out]  Receives the extents.

      \returns
      Returns true if and only if the GiDrawable object(s) have extents.
    */
    virtual bool extents(OdGeExtents3d& extents) const
    { OdGeExtents3d exts; bool bRVal = false;
      for (OdUInt32 nNode = 0; nNode < m_pModels->numModels(); nNode++)
      { bRVal |= m_pNode[nNode]->extents(exts);
        if (exts.isValidExtents())
          extents.addExt(exts);
      }
      return bRVal;
    }
};

/** \details
    This class provides management functionality for multiple OdGsCache instances.

    Corresponding C++ library: TD_Gs
    <group OdGs_Classes> 
*/
class GS_TOOLKIT_EXPORT OdGsCacheRedirectionManager : public OdRxObject
{
  protected:
    struct ModelsListPredicate
    {
      bool operator()(const OdGsModelPtr &lhs, const OdGsModelPtr &rhs) const
      { const OdGsModelRedirectionHandler *pLeft  = static_cast<const OdGsModelRedirectionHandler*>(lhs.get());
        const OdGsModelRedirectionHandler *pRight = static_cast<const OdGsModelRedirectionHandler*>(rhs.get());
        if (pLeft->numModels() != pRight->numModels())
          return pLeft->numModels() < pRight->numModels();
        for (OdUInt32 nCache = 0; nCache < pLeft->numModels(); nCache++)
        { if (pLeft->gsModelAt(nCache) != pRight->gsModelAt(nCache))
            return pLeft->gsModelAt(nCache) < pRight->gsModelAt(nCache);
        }
        return false;
      }
    };
    std::set<OdGsModelPtr, ModelsListPredicate> m_rModelsList;
    OdRefCounter m_nReferences;
    class FinderEntry : public OdGsModelRedirectionHandler
    { public:
        static FinderEntry *createMe(OdUInt32 nModels) { return static_cast<FinderEntry*>(allocate(nModels)); }
    } *m_pFinderEntry;
    OdVector<OdGsCache*, OdMemoryAllocator<OdGsCache*> > m_tmpNodesList;
    mutable OdMutex m_mutex;
  public:
    OdGsCacheRedirectionManager() : m_nReferences(0), m_pFinderEntry(NULL) { }
    ~OdGsCacheRedirectionManager() { if (m_pFinderEntry) delete m_pFinderEntry; }
    ODRX_DECLARE_MEMBERS(OdGsCacheRedirectionManager);

    /** \details
      Assigns the specified OdGsCache to Drawable object.

      \param pDrawable [in]  Drawable object pointer.
      \param pGsNode [in]  Pointer to the OdGsCache to be assigned.
    */
    void setGsNode(OdGiDrawable *pDrawable, OdGsCache* pGsNode);

    /** \details
      Removes the specified OdGsCache from Drawable object.

      \param pDrawable [in]  Drawable object pointer.
      \param pModel [in]  Model for which OdGsCache will be removed.
    */
    void resetGsNode(OdGiDrawable *pDrawable, OdGsModel* pModel);

    /** \details
      Assigns or removes the specified OdGsCache in Drawable object.

      \param pDrawable [in]  Drawable object pointer.
      \param pGsNode [in]  Pointer to the OdGsCache to be assigned.
      \param pModel [in]  Model for which OdGsCache will be removed.
    */
    void setGsNode(OdGiDrawable *pDrawable, OdGsCache* pGsNode, OdGsModel* pModel)
    {
      if (pGsNode)
        setGsNode(pDrawable, pGsNode);
      else
        resetGsNode(pDrawable, pModel);
    }

    /** \details
      Retrieves the OdGsCache associated with Drawable object.
    
      \param pDrawable [in]  Drawable object pointer.
      \param pModel [in]  Model object for which OdGsCache object will be retrieved.
      \returns
      Pointer to the OdGsCache that represents cache associated with Drawable object.
    */
    OdGsCache* gsNode(const OdGiDrawable *pDrawable, OdGsModel *pModel) const;

    /** \details
      Detach node from drawable.

      \param pNode [in]  Node to detach.
    */
    void setDrawableNull(OdGsCacheRedirectionNode *pNode);

    /** \details
      Retrieves the protection mutex.
    */
    OdMutex& getProtectionMutex() const
    {
      return m_mutex;
    }

  protected:
    friend OdGsCacheRedirectionManager *odgsCreateGsCacheRedirectionManager();
    friend void odgsDestroyGsCacheRedirectionManager();
};

inline void OdGsCacheRedirectionNode::setDrawableNull()
{
  m_pModels->cacheRedirectionManager()->setDrawableNull(this);
  for (OdUInt32 nNode = 0; nNode < m_pModels->numModels(); nNode++)
    m_pNode[nNode]->setDrawableNull();
}

/** \details
  Create global instance of OdGsCacheRedirectionManager.
*/
OdGsCacheRedirectionManager *odgsCreateGsCacheRedirectionManager();
/** \details
  Destroy global instance of OdGsCacheRedirectionManager.
*/
void odgsDestroyGsCacheRedirectionManager();
/** \details
  Aquire global instance of OdGsCacheRedirectionManager.
*/
GS_TOOLKIT_EXPORT OdGsCacheRedirectionManager *odgsGetGsCacheRedirectionManager();

#include "TD_PackPop.h"

#endif // __OD_GS_NODE_REDIRECTION__
