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

#ifndef __GS_H_INCLUDED_
#define __GS_H_INCLUDED_

#include "RxObject.h"
#include "RxIterator.h"
#include "Gs/GsExport.h"

class OdGiDrawable;

/** \details
  This template class is a specialization of the OdSmartPtr class for OdGiDrawable object pointers. 
*/
typedef OdSmartPtr<OdGiDrawable> OdGiDrawablePtr;

class OdGeMatrix3d;
class OdGePoint3d;
class OdGeVector3d;
class OdGePoint2d;
class OdGiEdgeData;
class OdGiFaceData;
class OdGiVertexData;
struct OdGiClipBoundary;
class OdGiAbstractClipBoundary;
class OdDbStub;
class OdGeExtents3d;
class OdGiVisualStyle;

#include "TD_PackPush.h"

#include "OdPlatform.h"
#include "RxModule.h"
#include "IntArray.h"
#include "GsDCPointArray.h"
#include "Ge/GePoint2dArray.h"

class OdGsSelectionReactor;

class OdGsView;

/** \details
  This template class is a specialization of the OdSmartPtr class for OdGsView object pointers. 
*/
typedef OdSmartPtr<OdGsView> OdGsViewPtr;

class OdGsModel;

/** \details
  This template class is a specialization of the OdSmartPtr class for OdGsModel object pointers. 
*/
typedef OdSmartPtr<OdGsModel> OdGsModelPtr;

/** \details
  This template class is a specialization of the OdArray class for OdGsModel object pointers.
*/
typedef OdArray<OdGsModel*, OdMemoryAllocator<OdGsModel*> > OdGsModelArray;

class OdGsDevice;

/** \details
  This template class is a specialization of the OdSmartPtr class for OdGsDevice object pointers. 
*/
typedef OdSmartPtr<OdGsDevice> OdGsDevicePtr;

class OdRxDictionary;

/** \details
  This template class is a specialization of the OdSmartPtr class for OdRxDictionary object pointers. 
*/
typedef OdSmartPtr<OdRxDictionary> OdRxDictionaryPtr;

class OdGsReactor;

/** \details
  This template class is a specialization of the OdSmartPtr class for OdGsReactor object pointers. 
*/
typedef OdSmartPtr<OdGsReactor> OdGsReactorPtr;

class OdGiRasterImage;

/** \details
  This template class is a specialization of the OdSmartPtr class for OdGiRasterImage object pointers. 
*/
typedef OdSmartPtr<OdGiRasterImage> OdGiRasterImagePtr;

class OdGsUpdateManager;
/** \details
  This template class is a specialization of the OdSmartPtr class for OdGsUpdateManager object pointers. 
*/
typedef OdSmartPtr<OdGsUpdateManager> OdGsUpdateManagerPtr;

#include "GsDefs.h"

class OdGiContext;
struct OdGsClientViewInfo;
class OdGsFiler;

class OdGsCollisionDetectionReactor;
class OdGsCollisionDetectionContext;
class OdGiPathNode;

#include "Gi/GiContextualColors.h"

/** \details
    This class is the abstract base class for classes that implement Viewport objects in GUI display windows.

    \remarks
    Each Viewport objects consists of a collection of objects to be viewed, and camera parameters that define the view.
    
    Corresponding C++ library: TD_Gs
    <group OdGs_Classes> 
    \sa
    <link tv_working_with_base_classes_view, Work with Visualize Graphical Views>
    \sa
    <link gs_OdGsView, Work with Views>
*/
class FIRSTDLL_EXPORT OdGsView : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdGsView);
  
  /** \details
    Render modes for the view.
  */
  enum RenderMode
  {
    kBoundingBox                  = -1,     // Bounding box. For internal use only.
    k2DOptimized                  = 0,      // Standard *display*. Optimized for 2D.
    kWireframe                    = 1,      // Standard *display*. Uses 3D pipeline.
    kHiddenLine                   = 2,      // Wireframe *display*. Hidden lines removed.
    kFlatShaded                   = 3,      // Faceted *display*. One *color* per face.
    kGouraudShaded                = 4,      // Smooth shaded *display*. Colors interpolated between *vertices*.
    kFlatShadedWithWireframe      = 5,      // Faceted *display* with wireframe overlay.
    kGouraudShadedWithWireframe   = 6,      // Smooth shaded *display* with wireframe overlay.
    kNone
  };

  /** \details
    Projection types.
  */
  enum Projection
  {
    kParallel     = 0, // Orthogonal projection.
    kPerspective  = 1  // Perspective projection.
  };

  /** \details
    Selection modes.
  */
  enum SelectionMode
  {
    kWindow       = 0, // Selection by two points (represents rectangle). Select only objects which is entirely inside selection area.
    kCrossing     = 1, // Selection by two points (represents rectangle). Select objects which is entirely inside or intersects selection area.
    kFence        = 2, // Selection by two or more points (represents polyline). Select object which is intersects selection polyline.
    kWPoly        = 3, // Selection by three or more points (represents polygon). Select only objects which is entirely inside selection area.
    kCPoly        = 4, // Selection by three or more points (represents polygon). Select objects which is entirely inside or intersects selection area.

    kPoint        = 16 // Same as kCrossing, but returns only single selected object: top for 3D render modes or first for 2D render mode.
  };

  /** \details
    Default lighting modes.
  */
  enum DefaultLightingType
  {
    kOneLight = 0, // Single front light.
    kTwoLights,    // Front and back lights.
    kBackLight     // Single back light.
  };

  /** \details
    View update flags.
  */
  enum ViewUpdateFlags
  {
    kCameraChanged = 1
  };

  /** \details
    Returns the GsDevice object associated with this Viewport object.
    \sa
    <link tv_working_with_base_classes_device, Work with Visualize Graphics Devices>
  */
  virtual OdGsDevice* device() const = 0;

  /** \details
    Returns the GiContext object associated with this Viewport object.
  */
  virtual OdGiContext* userGiContext() const = 0;

  /** \details
    Sets the User Context object associated with this Viewport object.
    \param pUserGiContext [in]  Pointer to the User Context.
  */
  virtual void setUserGiContext(OdGiContext* pUserGiContext) = 0;

  /** \details
    Returns the scale factor for displaying Lineweights in this Viewport object.
    \sa
    <link tv_working_with_attributes_lineweights, Work with Lineweights>
  */
  virtual double lineweightToDcScale() const = 0;
  /** \details
    Sets the scale factor for displaying Lineweights in this Viewport object.
    \param scale [in]  Scale factor.
    \sa
    <link tv_working_with_attributes_lineweights, Work with Lineweights>
  */
  virtual void setLineweightToDcScale(double scale) = 0;

  /** \details
    Sets the lineweights for this Viewport object.
    \param numLineweights [in]  Number of Lineweights.
    \param lineweights [in]  Array of Lineweights.
    \sa
    <link tv_working_with_attributes_lineweights, Work with Lineweights>
  */
  virtual void setLineweightEnum(int numLineweights, const OdUInt8* lineweights) = 0;

  /** \details
    Sets the size and position of this Viewport object.
    
    \param lowerLeft [in]  Lower-left corner in nomalized device coordinates [0.0 .. 1.0].
    \param upperRight [in]  Upper-right corner in nomalized device coordinates [0.0 .. 1.0].   
    \remarks
    (0,0) is the lower-left corner of the owning Device object. Coordinates increase upward and to the right.
    The View object can be partly or completely off screen.    
  */
  virtual void setViewport(const OdGePoint2d& lowerLeft, const OdGePoint2d& upperRight) = 0;
  
  /** \details
    Sets the size and position of this Viewport object.
    
    \param screenRect [in]  Screen rectangle in device coordinates (pixels).    
    \remarks
    (0,0) is the lower-left corner of the owning Device object. Coordinates increase upward and to the right. 
    \remarks
    The View object can be partly or completely off screen.    
  */
  virtual void setViewport(const OdGsDCRect& screenRect) = 0;
  
  /** \details
    Sets the size and position of this Viewport object.
    
    \param screenRect [in]  Screen rectangle in device coordinates (pixels).    
    \remarks
    (0,0) is the lower-left corner of the owning Device object. Coordinates increase upward and to the right.
    The View object can be partly or completely off screen.    
  */
  virtual void setViewport(const OdGsDCRectDouble& screenRect);

  /** \details
    Returns the size and position of this Viewport object.
    \param lowerLeft [out]  Receives the lower-left corner in nomalized device coordinates [0.0 .. 1.0].
    \param upperRight [out]  Receives the upper-right corner in nomalized device coordinates [0.0 .. 1.0].
    
    \remarks
    (0,0) is the lower-left corner of the owning Device object.  Coordinates increase upward and to the right.
    
    \remarks
    The View object may be partly or completely off screen.    
  */
  virtual void getViewport(OdGePoint2d& lowerLeft, OdGePoint2d& upperRight) const = 0;
  
  /** \details
    Returns the size and position of this Viewport object.
    \param screenRect [out]  Receives the screen rectangle in device coordinates (pixels).
    
    \remarks
    (0,0) is the lower-left corner of the owning Device object.  Coordinates increase upward and to the right.
    
    \remarks
    The View object may be partly or completely off screen.    
  */
  virtual void getViewport(OdGsDCRect& screenRect) const = 0;
  
  /** \details
    Returns the size and position of this Viewport object.
    \param screenRect [out]  Receives the screen rectangle in device coordinates (pixels).
    \remarks
    (0,0) is the lower-left corner of the owning Device object.  Coordinates increase upward and to the right.
    The View object may be partly or completely off screen.    
  */
  virtual void getViewport(OdGsDCRectDouble& screenRect) const;

  /** \details
    Defines a polygonal clip region for this Viewport object.
    
    \param numCoutours [in]  Number of polygonal contours.
    \param numVertices [in]  Array of the number of vertices in each polygonal contour.
    \param vertices [in]  Array of device coordinate (pixel) vertices defining the polygonal contours.
  */
  virtual void setViewportClipRegion(int numContours, int const* numVertices, OdGsDCPoint const* vertices) = 0;
  
  /** \details
    Defines a polygonal clip region for this Viewport object.
    
    \param numCoutours [in]  Number of polygonal contours.
    \param numVertices [in]  Array of the number of vertices in each polygonal contour.
    \param vertices [in]  Array of device coordinate (pixel) vertices defining the polygonal contours.
  */
  virtual void setViewportClipRegion(int numContours, int const* numVertices, OdGePoint2d const* vertices) = 0;

  /** \details
    Returns a polygonal clip region for this Viewport object.
    
    \param counts [out]  Array of the number of vertices in each polygonal contour.
    \param vertices [out]  Array of device coordinate (pixel) vertices defining the polygonal contours.
  */
  virtual void viewportClipRegion(OdIntArray& counts, OdGsDCPointArray& vertices) const = 0;
  
  /** \details
    Returns a polygonal clip region for this Viewport object.
    
    \param counts [out]  Array of the number of vertices in each polygonal contour.
    \param vertices [out]  Array of device coordinate (pixel) vertices defining the polygonal contours.
  */
  virtual void viewportClipRegion(OdIntArray& counts, OdGePoint2dArray& vertices) const = 0;

  /** \details
    Removes the polygonal clip region for this Viewport object.
  */
  void removeViewportClipRegion();

  /** \details
    Defines the 3d clipping for this Viewport object.

    \param pBoundary [in]  3d clipping boundary.
    \param pClipInfo [in]  Optional 3d clipping boundary extended data.
  */
  virtual void setViewport3dClipping(const OdGiClipBoundary* pBoundary, const OdGiAbstractClipBoundary* pClipInfo = NULL);

  /** \details
    Returns the current 3d clipping for this Viewport object.

    \param ppClipInfo [out]  Optional 3d clipping boundary extended data.

    \remarks
    Returns null if 3d clipping boundary doesn't set.
  */
  virtual const OdGiClipBoundary *viewport3dClipping(const OdGiAbstractClipBoundary** ppClipInfo = NULL) const;

  /** \details
    Removes the 3d clipping from this Viewport object.
  */
  void removeViewport3dClipping();

  /** \details
    Sets the color and width of the border for this Viewport object.
    \param color [in]  Border color.
    \param width [in]  Border width in pixels.
  */
  virtual void setViewportBorderProperties(ODCOLORREF color, int width) = 0;

  /** \details
    Returns the color and width of the border for this Viewport object.
    \param color [out]  Receives the border color.
    \param width [out]  Receives the border width in pixels.
  */
  virtual void getViewportBorderProperties(ODCOLORREF& color, int& width) const = 0;

  /** \details
    Controls the border visibility for this Viewport object.
    \param visible [in]  Controls visibility.
  */
  virtual void setViewportBorderVisibility(bool visible) = 0;

  /** \details
    Checks whether viewport border is visible.
    \returns
    true if the border is visible for this Viewport object, false otherwise.
  */
  virtual bool isViewportBorderVisible() const = 0;

  /** \details
    Sets the camera parameters for this Viewport object.
    
    \param position [in]  Camera position.
    \param target [in]  Camera target.
    \param upVector [in]  Camera up vector.
    \param fieldWidth [in]  Projection plane (field) width.
    \param fieldHeight [in]  Projection plane (field) height.
    \param projectionType [in]  Projection type.    
    \remarks
    This method defines the transfomation from WCS coordinates to normalized device coordinates.
    All parameters are in WCS coordinates.
    projectionType must be one of the following:
    
    <table>
    Name           Value
    kParallel      0
    kPerspective   1
    </table>
  */
  virtual void setView(
    const OdGePoint3d& position,
    const OdGePoint3d& target,
    const OdGeVector3d& upVector,
    double fieldWidth,
    double fieldHeight,
    Projection projectionType = kParallel) = 0;

  /** \details
    Returns the WCS camera position for this Viewport object.
  */
  virtual OdGePoint3d position() const = 0;

  /** \details
    Returns the WCS camera target for this Viewport object.
  */
  virtual OdGePoint3d target() const = 0;

  /** \details
    Returns the WCS camera up vector for this Viewport object.
  */
  virtual OdGeVector3d upVector() const = 0;

  /** \details
    Returns the perspective lens length for this Viewport object.
  */
  virtual double lensLength() const = 0;

  /** \details
    Sets the perspective lens length for this Viewport object.
    \param lensLength [in]  Perspective lens length.
  */
  virtual void setLensLength(double lensLength) = 0;

  /** \details
    Checks whether the projection type for the viewport object is kPerspective.
    \returns
    true if projection type for the viewport object is kPerspective, false otherwise.
  */
  virtual bool isPerspective() const = 0;

  /** \details
    Returns the WCS projection plane (field) width for this Viewport object.
  */
  virtual double fieldWidth() const = 0;
  /** \details
    Returns the WCS projection plane (field) height for this Viewport object.
  */
  virtual double fieldHeight() const = 0;

  /** \details
    Controls the front clipping of this Viewport object.
    \param enable [in]  Enable front clipping.
  */
  virtual void setEnableFrontClip(bool enable) = 0;

  /** \details
    Checks whether front clipping is enabled for the viewport object.
    \returns
    true if front clipping is enabled for the viewport object, false otherwise.
  */
  virtual bool isFrontClipped() const = 0;

  /** \details
    Sets the front clip distance from the target of this Viewport object.
    \param frontClip [in]  Front clip distance.
  */
  virtual void setFrontClip(double frontClip) = 0;

  /** \details
    Returns the front clip distance from the target of this Viewport object.
  */
  virtual double frontClip() const = 0;

  /** \details
    Controls the back clipping of this Viewport object.
    \param enable [in]  Enable back clipping.
  */
  virtual void setEnableBackClip(bool enable) = 0;

  /** \details
    Checks whether back clipping is enabled for the viewport object.
    \returns
    true if back clipping is enabled for the viewport object, false otherwise.
  */
  virtual bool isBackClipped() const = 0;

  /** \details
    Sets the back clip distance from the target of this Viewport object.
    \param backClip [in]  Back clip distance.
  */
  virtual void setBackClip(double backClip) = 0;
  /** \details
    Returns the back clip distance from the target of this Viewport object.
  */
  virtual double backClip() const = 0;

  /** \details
      Returns the matrix that transforms world space to view space for this Viewport object.

      \sa
      Coordinate Systems.
  */
  virtual OdGeMatrix3d viewingMatrix() const = 0;

  /** \details
      Returns the matrix that transforms view space to normalized device space for this Viewport object.

      \sa
      Coordinate Systems.
  */
  virtual OdGeMatrix3d projectionMatrix() const = 0;

  /** \details
      Returns the matrix that transforms normalized device space to screen space for this Viewport object.

      \sa
      Coordinate Systems.
  */
  virtual OdGeMatrix3d screenMatrix() const = 0;

  /** \details
      Returns a matrix that transforms coordinates from world space to screen space for this Viewport object. 
      
      \remarks
      This is equivalent to the concatenation of the viewingMatrix, 
      projectionMatrix, and screenMatrix.

      \sa
      Coordinate Systems.
  */
  virtual OdGeMatrix3d worldToDeviceMatrix() const = 0;

  /** \details
      Returns the matrix that transforms coordinates from model space to screen space for this Viewport object.

      \sa
      Coordinate Systems.
  */
  virtual OdGeMatrix3d objectToDeviceMatrix() const = 0;

  /** \details
    Sets the render mode for this Viewport object.
    
    \param mode [in]  Render mode.
    
    \remarks
    mode must be one of the following:
    
    <table>
    Name                            Value      Description
    kBoundingBox                    -1         Bounding box. For internal use only.
    k2DOptimized                    0          Standard display. Optimized for 2D.
    kWireframe                      1          Standard display. Uses 3D pipeline.
    kHiddenLine                     2          Wireframe display. Hidden lines removed.
    kFlatShaded                     3          Faceted display. One color per face.
    kGouraudShaded                  4          Smooth shaded display. Colors interpolated between vertices.
    kFlatShadedWithWireframe        5          Faceted display with wireframe overlay.
    kGouraudShadedWithWireframe     6          Smooth shaded display with wireframe overlay.
    </table>
  */
  virtual void setMode(RenderMode mode) = 0;

  /** \details
    Sets the render mode for this Viewport object.
    
    \remarks
    mode must be one of the following:
    
    <table>
    Name                            Value      Description
    kBoundingBox                    -1         Bounding box. For internal use only.
    k2DOptimized                    0          Standard display. Optimized for 2D.
    kWireframe                      1          Standard display. Uses 3D pipeline.
    kHiddenLine                     2          Wireframe display. Hidden lines removed.
    kFlatShaded                     3          Faceted display. One color per face.
    kGouraudShaded                  4          Smooth shaded display. Colors interpolated between vertices.
    kFlatShadedWithWireframe        5          Faceted display with wireframe overlay.
    kGouraudShadedWithWireframe     6          Smooth shaded display with wireframe overlay.
    </table>
  */
  virtual RenderMode mode() const = 0;

  /** \details
    Adds the specified scene graph to this Viewport object.
    
    \param pSceneGraph [in]  Pointer to the root of the scene graph.
    \param pModel [in]  Pointer to the Model object to which all scene graph notifications are to be directed.
    
    \remarks
    This function is intended to be used as follows:
    
    1.  Construct a scene graph of OdGiDrawable objects. 
        Each object draws its children with OdGiGeometry::draw().
    
    2.  With this function, add the root of the scene graph and the 
        OdGsModel that handles notifications to this Viewport object.
  */
  virtual bool add(OdGiDrawable* pSceneGraph, OdGsModel* pModel) = 0;

  /** \details
    Retrieves a quantity of root drawables for this viewport.
  */
  virtual int numRootDrawables() const = 0;

  /** \details
    Retrieves a persistent object identifier of the root drawable at the specified index.
    \param i [in]  Index of the drawable to return.
    \param pModelReturn [in]  Index of the drawable to return.
  */
  virtual OdDbStub* rootDrawableIdAt(int i, OdGsModelPtr* pModelReturn = 0) const = 0;

  virtual OdGiDrawablePtr rootDrawableAt(int i, OdGsModelPtr* pModelReturn = 0) const = 0;

  /** \details
    Removes the specified scene graph from this Viewport object.
    
    \param sceneGraph [in]  Pointer to the root of the scene graph.
    \returns
    true if scene graph is successfully erased, false otherwise.
  */
  virtual bool erase(OdGiDrawable* sceneGraph) = 0;
  /** \details
    Removes all scene graphs from this Viewport object.
    
    \param sceneGraph [in]  Root of scene graph.
  */
  virtual void eraseAll() = 0;

  /** \details
    Returns OdGsModel object associated with specified OdGiDrawable object in this Viewport object.

    \param pDrawable [in]  Pointer onto OdGiDrawable object.
  */
  virtual OdGsModel *getModel(const OdGiDrawable *pDrawable) const = 0;
  /** \details
    Returns array of OdGsModel objects associated with this Viewport object.
  */
  virtual OdGsModelArray getModelList() const = 0;

   /** \details
    Marks the entire area of this View for refreshing.
   */
  virtual void invalidate() = 0;
  
  /** \details
    Marks for refresh the specified region of this Viewport object.
     
    \param screenRect [in]  Screen rectangle in device coordinates (pixels).
   */
  virtual void invalidate(const OdGsDCRect &screenRect) = 0;

  /** \details
    Checks whether this Viewport shows the correct image.
    \returns
    true if this Viewport object is showing the correct image, false otherwise.

    \remarks
    This implies the follow:
    * No part of GUI area is flagged for refresh.
    * All rasterizations are complete
    * The rendering pipeline has been flushed.
  */
  virtual bool isValid() const = 0;

  /** \details
    Flushes any queued graphics to the display device.
    
    \remarks
    If called while this Viewport object is in the interactive state, this function immediately returns.
  */
  virtual void update() = 0;

  /** \details
    Puts this Viewport object into interactive state.
    
    \param frameRateInHz [in]  Frames/second.
    
    \remarks
    A frame rate of 0.0 specifies the default frame rate.
    \sa
    <link gs_views_interactivity_mode, Work with Interactivity Mode for Views>
  */
  virtual void beginInteractivity(double frameRateInHz) = 0;
  /** \details
    Checks whether this Viewport object is in interactive state.
    \returns
    true if this View is in interactivity mode, false otherwise.
    \sa
    <link gs_views_interactivity_mode, Work with Interactivity Mode for Views>
  */
  virtual bool isInInteractivity() const = 0;
  /** \details
    Returns interactivity frame rate.
    \sa
    <link gs_views_interactivity_mode, Work with Interactivity Mode for Views>
  */
  virtual double interactivityFrameRate() const = 0;
  /** \details
    Removes this Viewport object from the interactive state.
    \sa
    <link gs_views_interactivity_mode, Work with Interactivity Mode for Views>
  */
  virtual void endInteractivity() = 0;
  /** \details
    Flushes the graphic pipeline and renders this Viewport object.
    
    \remarks
    This function should be called only during interactivty. 
  */
  virtual void flush() = 0;

  /** \details
    Hides this Viewport object.
  */
  virtual void hide() = 0;

  /** \details
    Un-hides this Viewport object.
  */
  virtual void show() = 0;

  /** \details
    Checks whether the viewport object is not hidden.
    \returns
    true if the wrapped viewport object is visible, false otherwise.
  */
  virtual bool isVisible() = 0;

  /** \details
    Freezes the specified Layer object in this Viewport object.
    \param layerID [in]  Layer to freeze.
    \remarks
    All layers are thawed by default.
    \sa
    <link tv_working_with_attributes_layers, Work with Layers>
  */
  virtual void freezeLayer(OdDbStub* layerID) = 0;

  /** \details
    Thaws the specified Layer object in this Viewport object.
    \param layerID [in]  Layer to thaw.
    \remarks
    All layers are thawed by default.
    \sa
    <link tv_working_with_attributes_layers, Work with Layers>
  */
  virtual void thawLayer(OdDbStub* layerID) = 0;

  /** \details
    Thaws all Layer objects in this Viewport object.
    \remarks
    All layers are thawed by default.
    \sa
    <link tv_working_with_attributes_layers, Work with Layers>
  */
  virtual void clearFrozenLayers() = 0;

  /** \details
    Marks for regeneration all subViewportDraw() geometry for this Viewport object.
  */
  virtual void invalidateCachedViewportGeometry() = 0;

  /** \details
    Runs selection procedure inside this Viewport object.

    \param pts [in]  Selection points in device coordinate space.
    \param nPoints [in]  Count of selection points.
    \param pReactor [in]  Selection callback pointer.
    \param mode [in]  Selection mode.
    \sa
    <link tv_working_with_features_selections, Work with Selections>
  */
  virtual void select(const OdGsDCPoint* pts, int nPoints,
    OdGsSelectionReactor* pReactor,
    OdGsView::SelectionMode mode = OdGsView::kCrossing) = 0;

  /** \details
    Translates the camera position and target by the specified Camera Coordinate dolly amount.
     
    \param dollyVector [in]  Camera Coordinate dolly vector. 
    \remarks
    Camera Coordinates are relative to the view of a target from a camera. 
    * The Y-axis is along up vector. 
    * The Z-axis is along the vector from camera position to the target. 
    * The X-axis is the cross product Y-axis x Z-axis 
  */
  virtual void dolly(const OdGeVector3d& dollyVector) = 0;
  
  /** \details
    Translates the camera position and target by the specified Camera Coordinate dolly amount.
    
    \param xDolly [in]  Camera Coordinate X dolly amount.
    \param yDolly [in]  Camera Coordinate Y dolly amount.
    \param zDolly [in]  Camera Coordinate Z dolly amount.
    \remarks
    Camera Coordinates are relative to the view of a target from a camera.   
    * The Y-axis is along up vector. 
    * The Z-axis is along the vector from camera position to the target. 
    * The X-axis is the cross product Y-axis x Z-axis 
  */
  virtual void dolly(double xDolly, double yDolly, double zDolly) = 0;

  /** \details
    Rotates the camera by the specified amount about the eye vector.
     
    \param rollAngle [in]  Camera CCW roll angle.
    \remarks
    Positive angles rotate the camera counterclockwise about the eye vector
    when viewed from the camera position to the target. This corresponds
    to a clockwise rotation of the up vector.
    Camera Coordinates are relative to the view of a target from a camera. 
    * The Y-axis is along up-vector. 
    * The Z-axis (eye vector) is along the vector from camera position to the target. 
    * The X-axis is the cross product Y-axis x Z-axis.
    All angles are expressed in radians.
  */
  virtual void roll(double rollAngle) = 0;

  /** \details
    Orbits the camera about the target by the specified amounts.
     
    \param xOrbit [in]  X-axis Orbit.
    \param yOrbit [in]  Y-axis Orbit.
    \remarks
    the camera moves on the surface a sphere whose center the camera target and whose
    radius is the distance from the camera position to its target 
    * The camera is first rotated about an axis parallel to the X-axis and passing through the target.  
    * The camera is next rotated about an axis parallel to the Y-axis and passing through the target.
    Camera Coordinates are relative to the view of a target from a camera.  
    * The Y-axis is along up vector. 
    * The Z-axis is along the vector from camera position to the target. 
    * The X-axis is the cross product Y-axis x Z-axis 
    All angles are expressed in radians.
  */
  virtual void orbit(double xOrbit, double yOrbit) = 0;

  /** \details
    Scales the focal length of the camera by the specified amount.
     
    \param zoomFactor [in]  Zoom factor.
  */
  virtual void zoom(double zoomFactor) = 0;

  /** \details
    Rotates the target about the camera the specified amounts.
     
    \param xPan [in]  X-axis pan.
    \param yPan [in]  Y-axis pan.   
    \remarks
    The target moves on the surface a sphere whose center the camera position and whose
    radius is the distance from the camera position to its target.
    * The target is first rotated about an axis parallel to the X-axis and passing through the camera.  
    * The target is next rotated about an axis parallel to the Y-axis and passing through the camera.  
    Camera Coordinates are relative to the view of a target from a camera. 
    * The Y-axis is along up vector. 
    * The Z-axis is along the vector from camera position to the target. 
    * The X-axis is the cross product Y-axis x Z-axis 
    All angles are expressed in radians.
  */
  virtual void pan(double xPan, double yPan) = 0;

  /** \details
    Scales the camera to completely include specified WCS box inside view frustum.

    \param minPt [in]  minimal WCS box point.
    \param maxPt [in]  maximal WCS box point.
  */
  virtual void zoomExtents(const OdGePoint3d &minPt, const OdGePoint3d &maxPt) = 0;

  /** \details
    Scales the camera to completely include specified screen area.

    \param lowerLeft [in]  Lower-left corner in nomalized device coordinates [0.0 .. 1.0].
    \param upperRight [in]  Upper-right corner in nomalized device coordinates [0.0 .. 1.0].
  */
  virtual void zoomWindow(const OdGePoint2d &lowerLeft, const OdGePoint2d &upperRight) = 0;

  /** \details
    Checks whether specified point is visible inside current view frustum.

    \param pt [in]  WCS point for check.
    \returns
    true if the specified point is visible in the wrapped viewport, false otherwise.
  */
  virtual bool pointInView(const OdGePoint3d &pt) const = 0;

  /** \details
    Checks whether specified WCS box is visible completely or partially inside current view frustum.

    \param minPt [in]  minimal WCS box point.
    \param maxPt [in]  maximal WCS box point.
    \returns
    true if the specified extents box is visible in the current viewport frustum of the wrapped view, false otherwise.
  */
  virtual bool extentsInView(const OdGePoint3d &minPt, const OdGePoint3d &maxPt) const = 0;

  /** \details
    Clones this view object.

    \param cloneViewParameters [in]  If and only if true, view parameters will be cloned.
    \param cloneGeometry [in]  If and only if true, geometry will be cloned.
    \returns
    Returns a SmartPointer to the newly created object.
  */
  virtual OdGsViewPtr cloneView(bool cloneViewParameters = true, bool cloneGeometry = false) = 0;

  /** \details
    Sets the view parameters of the specified OdGsView object to that of this Viewport object.
    \param pView [out]  Receives the view parameters.
  */
  virtual void viewParameters(OdGsView* pView) const = 0;

  /** \details
    Checks whether any view parameters have exceeded their boundaries since the last
    call to this function.
    \returns 
    true if any view parameters have exceeded their boundaries since the last call to this function, false otherwise.
    \remarks
    The following calls are monitored:
    
    * dolly()
    * orbit()
    * pan() 
    * roll()
    * setBackClip()
    * setFrontClip()
    * setView()
    * zoom()
  */
  virtual bool exceededBounds() = 0;

  /** \details
    Controls stereo viewing for this Viewport object.
    
    \param enable [in]  Controls stereo viewing.
  */
  virtual void enableStereo(bool enabled) = 0;

  /** \details
    Checks whether stereo viewing is enabled for this Viewport object.
    \returns
    true if and only if stereo viewing is enabled for this Viewport object, false otherwise.
  */
  virtual bool isStereoEnabled() const = 0;

  /** \details
    Sets the stereo parameters for this Viewport Object.
    \param magnitude [in]  View separation [0.0 .. 2.0].
    \param parallax [in]  Adjusts the plane of zero parallax [0.0 .. 2.0].
    
    \remarks
    magnitude controls view eye separation, and hence the stereo effect.
    * A magnitude of 1.0 is the default value.
    * A magnitude of 0.0 produces no stereo effect.
    * A magnitude of 2.0 produces a double stereo effect.
    parallax adjusts the zero parallax plane.
    * A parallax of 1.0 is the default value. An object centered at the target will be half-in and half-out of the screen.
    * A parallax of 0.0 moves the object into the screen.
    * A parallax of 2.0 move the object out of the screen.
  */
  virtual void setStereoParameters(double magnitude, double parallax) = 0;

  /** \details
    Returns the stereo parameters for this Viewport Object.
    
    \param magnitude [out]  Receives the view separation [0.0 .. 2.0].
    \param parallax [out]  Receives the plane of zero parallax adjustment [0.0 .. 2.0].
    \remarks
    magnitude controls view eye separation, and hence the stereo effect.
    * A magnitude of 1.0 is the default value.
    * A magnitude of 0.0 produces no stereo effect.
    * A magnitude of 2.0 produces a double stereo effect.
    parallax adjusts the zero parallax plane.
    * A parallax of 1.0 is the default value. An object centered at the target will be half-in and half-out of the screen.
    * A parallax of 0.0 moves the object into the screen.
    * A parallax of 2.0 move the object out of the screen.
  */
  virtual void getStereoParameters(double& magnitude, double& parallax) const = 0;

  /** \details
    Returns an iterator to traverse the lights attached to this Viewport object.
    \param pLightsIterator [in]  Pointer to the lights iterator.
  */
  virtual void initLights(OdRxIterator* pLightsIterator) = 0;

  /** \details
    Sets a multiplier that is used to scale all linetypes in this Viewport object.
    
    \param linetypeScaleMultiplier [in]  Linetype scale multiplier.
  */
  virtual void setLinetypeScaleMultiplier(double linetypeScaleMultiplier) = 0;

  /** \details
    Returns current linetype scale multiplier value.
    \sa
    <exref target="https://docs.opendesign.com/td/db_linetype.html">Working with Linetypes</exref>
  */
  virtual double linetypeScaleMultiplier() const = 0;

  /** \details
    Sets a multiplier that could be used as an alternate scale factor for linetypes in this Viewport object.
    
    \param linetypeAlternateScaleMultiplier [in]  Alternate linetype scale multiplier.
    \sa
    <exref target="https://docs.opendesign.com/td/db_linetype.html">Working with Linetypes</exref>
  */
  virtual void setAlternateLinetypeScaleMultiplier(double linetypeAlternateScaleMultiplier) = 0;

  /** \details
    Returns current alternate linetype scale multiplier value.
    \sa
    <exref target="https://docs.opendesign.com/td/db_linetype.html">Working with Linetypes</exref>
  */
  virtual double linetypeAlternateScaleMultiplier() const = 0;

  /** \details
    Returns the ClientViewInfo for this Viewport object.
    
    \param clientViewInfo [out]  Receives the ClientViewInfo.
  */
  virtual void clientViewInfo(OdGsClientViewInfo& clientViewInfo) const;

  /** \details
    Clear color for this view.
  */
  enum ClearColor
  {
    kTransparent,         // Transparent color
    kDeviceBackground,    // Color from device background
    kPaletteBackground    // Palette background color
  };

  /** \details
    Sets the clear color for this view

    \param color [in]  The clear color.
  */
  virtual void setClearColor(ClearColor color) = 0;

  /** \details
    Checks whether the specified point is inside viewport area.

    \param screenPoint [in]  Check point in device coordinate space.
    \returns
    true if the passed point is inside the viewport, false otherwise.
  */
  virtual bool pointInViewport(const OdGePoint2d& screenPoint) const = 0;

  /** \details
    Returns the display pixel density at the specified point for this Viewport object.

    \param point [in]  WCS center of the unit square.
    \param pixelDensity [out]  Receives the pixel density.
    \param bUsePerspective [in]  Enable perspective inclusion into pixel density calculation.
    \remarks
    Pixel density is measured in pixels per WCS unit.
    This function can be used to determine if the geometry generated for an object will 
    be smaller than the size of a pixel.
  */
  virtual void getNumPixelsInUnitSquare(const OdGePoint3d& point,
                                        OdGePoint2d& pixelDensity, bool bUsePerspective = true) const = 0;

  /** \details
    Sets background object for this view.

    \param backgroundId [in]  New background object ID.
    \sa
    <link tv_working_with_attributes_view_backgrounds, Work with View Backgrounds>
  */
  virtual void setBackground(OdDbStub *backgroundId) = 0;
  /** \details
    Returns current background object ID for this view.
    \sa
    <link tv_working_with_attributes_view_backgrounds, Work with View Backgrounds>
  */
  virtual OdDbStub *background() const = 0;

  /** \details
    Sets visual style object for this view.

    \param visualStyleId [in]  New visual style object ID.
    \sa
    <link tv_working_with_attributes_visual_styles, Work with Visual Styles>
  */
  virtual void setVisualStyle(OdDbStub *visualStyleId) = 0;
  /** \details
    Returns current visual style object ID for this view.
    \sa
    <link tv_working_with_attributes_visual_styles, Work with Visual Styles>
  */
  virtual OdDbStub *visualStyle() const = 0;
  /** \details
    Sets visual style for this view.

    \param visualStyle [in]  New visual style data.
    \sa
    <link tv_working_with_attributes_visual_styles, Work with Visual Styles>
  */
  virtual void setVisualStyle(const OdGiVisualStyle &visualStyle) = 0;
  /** \details
    Returns current visual style for this view.

    \param visualStyle [out]  OdGiVisualStyle structure to fill by current visual style data.

    \returns
    Returns true if and only if OdGiVisualStyle structure is filled by visual style data.
    \sa
    <link tv_working_with_attributes_visual_styles, Work with Visual Styles>
  */
  virtual bool visualStyle(OdGiVisualStyle &visualStyle) const = 0;

  /** \details
    Enables default lighting for this view.

    \param bEnable [in]  New default lighting state.
    \param lightType [in]  New default lighting type.
    \sa
    <link tv_working_with_lights, Work with Lights>
  */
  virtual void enableDefaultLighting(bool bEnable, DefaultLightingType lightType = kTwoLights) = 0;

  /** \details
    Returns image of current view buffer.

    \param pImage [out]  Pointer for image to return.
    \param region [in]  Rectangle specifies region to return.
    \remarks
    Implementation can throw eNotImplementedYet exception if current vectorization module doesn't support snap shots.
  */
  virtual void getSnapShot(OdGiRasterImagePtr &pImage, const OdGsDCRect &region) = 0;

  /** \details
    Runs collision detection procedure inside this Viewport object.

    \param pInputList [in]  Pointer to the list of objects to check
    \param nInputListSize [in]  Size of the list of objects to check
    \param pCollisionWithList [in]  Pointer to the list of possible colliding objects
    \param nCollisionWithListSize [in]  Size of the list of possible colliding objects
    \param pCtx [in] Pointer to the OdGsCollisionDetectionContext that provide additional Collision Detection options
    \remarks
    If pCollisionWithList is NULL pInputList will be checked with all other drawables.
    If pCollisionWithList is not a NULL pInputList will be checked with pCollisionWithList only.
    If pInputList is NULL all drawables will be checked on collisions with each other.
    \sa
    <link gs_collision_detection, Work with Collision Detection>
  */
  virtual void collide( OdGiPathNode const*const* pInputList, OdUInt32 nInputListSize, OdGsCollisionDetectionReactor* pReactor,
                        OdGiPathNode const*const* pCollisionWithList = NULL, OdUInt32 nCollisionWithListSize = 0,
                        const OdGsCollisionDetectionContext* pCtx = NULL ) = 0;

  /** \details
    Enable or disable anti-aliasing in current Viewport object.

    \param nMode [in]  Anti-aliasing mode (set as 3 by default). Set as 0 to disable anti-aliasing.
  */
  virtual void enableAntiAliasing(OdUInt32 nMode) = 0;
  /** \details
    Returns anti-aliasing mode for current Viewport object.
  */
  virtual OdUInt32 antiAliasingMode() const = 0;
};

/** \details
    This stucture contains the Window ID and Viewport Object ID.
    
    Corresponding C++ library: TD_Gs

    <group !!RECORDS_TD_APIRef>
*/
struct OdGsClientViewInfo 
{
  enum ViewportFlags
  {
    kDependentViewport = 1,
    kDependentGeometry = 2,
    kHelperView        = 4,
    kSetViewportId     = 8
  };

  long      viewportId;        // Returned by OdGiViewport::viewportId()
  long      acadWindowId;      // Returned by OdGiViewport::acadWindowId()
  OdDbStub* viewportObjectId;  // Returned by OdGiViewportDraw::viewportObjectId()
  OdGiContextualColorsPtr contextColors; // Returned by OdGiViewport::contextualColors()
  OdUInt32  viewportFlags;     // Optional view flags

  OdGsClientViewInfo()
    : viewportId(-1)
    , acadWindowId(0)
    , viewportObjectId(NULL)
    , viewportFlags(0)
  {
  }
};

/** \details
    This class is the base class for custom classes that receive notification
    of OdGs Collision Detection events.
    
    \remarks
    The default implementations of all methods in this class do nothing but return.

    <group OdGs_Classes> 
    \sa
    <link gs_collision_detection, Work with Collision Detection>
*/
class OdGsCollisionDetectionReactor
{
public:
  enum
  {
    kNotImplemented = 0x80000000,
    kContinue       = 0,
    kBreak          = 1
  };
  /** \details
    Notification function called whenever an collision detected.

    \param pPathNode1 [in]  Pointer to the OdGiPathNode object from OdGsView::collide input list
    \param pPathNode2 [in]  Pointer to the OdGiPathNode object that collides with pathNode1

    \remarks
    Returns kContinue if the operation is to continue or kBreak to stop collision detection
  */
  virtual OdUInt32 collisionDetected(const OdGiPathNode* /*pPathNode1*/, const OdGiPathNode* /*pPathNode2*/)
  {
    return OdUInt32(kNotImplemented);
  }

  /** \details
  Notification function called whenever an collision detected if distance calsulation is specified

  \param pPathNode1 [in]  Pointer to the OdGiPathNode object from OdGsView::collide input list
  \param pPathNode2 [in]  Pointer to the OdGiPathNode object that collides with pathNode1
  \param dDistace   [in]  Collision distance. If negative, it is the penetration depth. If positive, it is the clearance.

  \remarks
  Returns kContinue if the operation is to continue or kBreak to stop collision detection
*/
  virtual OdUInt32 collisionDetected( const OdGiPathNode* pPathNode1, const OdGiPathNode* pPathNode2, double dDistance )
  {
    return collisionDetected( pPathNode1, pPathNode2 );
  }
};

/** \details
    This class is the context for Collision Detection
    
    \sa
    TD_Gs
  
    <group OdGs_Classes> 
*/
class OdGsCollisionDetectionContext
{
//DOM-IGNORE-BEGIN
protected:
  OdUInt8 m_flags;
  OdGeTol m_tolerance;
  enum Options
  {
    kIntersectionOnly = 1,
    kIntersectionWithModel = 1 << 1,
    kIgnoreViewExtents = 1 << 2,
    kCombineSubEntities = 1 << 3,
    kOverrideTolerance = 1 << 4,
    kCalculateDistance = 1 << 5,
    kCheckAll = 1 << 6,
    kProcessSingleList = 1 << 7
  };
  double m_clearance;
//DOM-IGNORE-END
public:
  /** \details
    Default constructor for the OdGsCollisionDetectionContext class. Sets default tolerance value for geometric operations.
  */
  OdGsCollisionDetectionContext() : m_flags( kCalculateDistance ) { m_tolerance = OdGeTol(); m_clearance = 0.0; };

  /** \details
    Sets flag that specifies whether collisions are detected only when objects intersect.
    \param bSet    [in]  Flag that specifies whether to detect collisions only when objects intersect. If set to false, collisions can be detected via touches (no intersections).
  */
  void setIntersectionOnly( bool bSet ) { SETBIT( m_flags, kIntersectionOnly, bSet ); }
  
  /** \details
    Retrieves a flag that specifies whether collisions are detected only when objects intersect. 
    \returns
    true if collisions are detected only when objects intersect or false when collisions can be detected via touches.
  */
  bool intersectionOnly() const { return GETBIT( m_flags, kIntersectionOnly ); }

  /** \details
    Sets flag that specifies whether collisions should be detected with a model.
    \param bSet    [in]  Flag that specifies whether to detected collisions with a model.
  */
  void setIntersectionWithModel( bool bSet ) { SETBIT( m_flags, kIntersectionWithModel, bSet ); }
  
  /** \details
    Retrieves a flag that specifies whether collisions are detected only when objects intersect. 
    \returns
    true if collisions are detected only when objects intersect or false when collisions can be detected via touches.
  */
  bool intersectionWithModel() const { return GETBIT( m_flags, kIntersectionWithModel ); }

  /** \details
    Specifies whether collisions should be detected despite objects are outside view extents.
    \param bSet    [in]  Flag that specifies whether to detected collisions even if objects are beyond view extents.
  */
  void setIgnoreViewExtents( bool bSet ) { SETBIT( m_flags, kIgnoreViewExtents, bSet ); }
  
  /** \details
    Checks whether collisions are detected even if objects are outside view extents. 
    \returns
    true if collisions are detected even if objects are beyond view extents, false otherwise.
  */
  bool ignoreViewExtents() const { return GETBIT( m_flags, kIgnoreViewExtents ); }
  /** \details
  Specifies subentity markers ignoring.
  \param bSet [in] If true, subentities markers will be ignored.
  */
  void setCombineSubEntities( bool bSet ) { SETBIT( m_flags, kCombineSubEntities, bSet ); }
  /** \details
  Retrives subentity markers ignoring.
  \returns Returns flag that indicates should subentities markers be ignored or not.
  */
  bool combineSubEntities() const { return GETBIT( m_flags, kCombineSubEntities ); }
  /** \details
  Specifies custom collision tolerance.
  \param tol [in] Collision tolerance
  */
  void setToleranceOverride( const OdGeTol& tol )
  {
    SETBIT_1( m_flags, kOverrideTolerance );
    m_tolerance = tol;
  }
  /** \details
  Checks that collision detection should use custom tolerance.
  \returns Returns flag that indicates should collision detection use custom tolerance or not.
  */
  bool isToleranceOverride() const { return GETBIT( m_flags, kOverrideTolerance ); }
  /** \details
  Retrives custom collision detection tolerance.
  \returns Returns custom collision detection tolerance.
  */
  const OdGeTol& toleranceOverride() const { return m_tolerance; }
  /** \details
  Specifies that collision detection should also calculate distances (penetration depth or distance between objects)
  \param bSet [in] If true, distances will be calculated.
  */
  void setCalculateDistance( bool bSet ) { SETBIT( m_flags, kCalculateDistance, bSet ); }
  /** \details
  Checks should collision detection calculate distances (penetration depth or distance between objects)
  \returns Returns flag that indicates should collision detection calculate distances or not.
  */
  bool getCalculateDistance() const { return GETBIT( m_flags, kCalculateDistance ); }
  /** \details
  Specifies that collision detection should check all faces instead of detecting first collision.
  \param bSet [in] If true, collision detection will check all faces instead of detecting first collision.
  */
  void setCheckAll( bool bSet ) { SETBIT( m_flags, kCheckAll, bSet ); }
  /** \details
  Checks that collision detection should check all faces instead of detecting first collision.
  \returns Returns flag that indicates should collision detection check all faces instead of detecting first collision.
  */
  bool getCheckAll() const { return GETBIT( m_flags, kCheckAll ); }
  /** \details
  Specifies collision detection clearance value.
  \param c [in] Collision detection clearance value.
  */
  void setClearance( double c ) { m_clearance = c; }
  /** \details
  Retrives collision detection clearance value.
  \returns Returns collision detection clearance value.
  */
  double getClearance() const { return m_clearance; }
  /** \details
  Specifies should collision detection detects collisions between list and all other entities (false)
  or it should detect collisions between entities in list (true).
  \param bSet [in] If true, collision detection will detect collisions between entities in list instead of between list and other entities.
  */
  void setProcessSignleList( bool bSet ) { SETBIT( m_flags, kProcessSingleList, bSet ); }
  /** \details
  Checks that collision detection should detects collisions between list and all other entities (false)
  or it should detect collisions between entities in list (true).
  \returns Returns flag that indicates should collision detection detect collisions between entities in list instead of between list and other entities.
  */
  bool getProcessSignleList() const { return GETBIT( m_flags, kProcessSingleList ); }

};

class OdGiContext;

/** \details
    This class is the abstract base class for classes that encapsulate GUI display windows.

    \remarks
    OdGsDevice objects own, update, and refresh one or more OdGsView objects.
    In addition, they responds to operating system notifications,
    such as window resize, forwarded by the graphics system client.
    Corresponding C++ library: TD_Gs
        
    <group OdGs_Classes>
    \sa
    <link tv_working_with_base_classes_device, Work with Visualize Graphics Devices>
    \sa
    <link gs_OdGsDevice, Work with Graphics Devices>
*/
class FIRSTDLL_EXPORT OdGsDevice : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdGsDevice);

  /** \details
    Returns a SmartPointer to the dictionary containing the 
    properties for this Device object.
    \sa
    <link gs_DeviceProps, ODA Vectorization Devices Properties>
  */
  virtual OdRxDictionaryPtr properties() = 0;

  /** \details
    Returns the User Context object associated with this Device object.
  */
  virtual OdGiContext* userGiContext() const = 0;

  /** \details
    Sets the User Context object associated with this Device object.
    \param pUserGiContext [in]  Pointer to the User Context.
  */
  virtual void setUserGiContext(OdGiContext* pUserGiContext) = 0;

  /** \details
    Marks for refresh entire region of the GUI window of this Device object.
  */
  virtual void invalidate() = 0;

  /** \details
    Marks for refresh the specified region of the GUI window of this Device object.
     
    \param screenRect [in]  Rectangular area.
  */
  virtual void invalidate(const OdGsDCRect &screenRect) = 0;

  /** \details
    Checks whether the GUI window for this Device object is showing the correct image.
    
    \returns 
    true if the GUI window for this Device object is showing the correct image, false otherwise.
    \remarks
    This implies the following:
    * No part of GUI area is flagged for refresh.
    * All rasterizations are complete
    * The rendering pipeline has been flushed.
  */
  virtual bool isValid() const = 0;

  /** \details
    Updates the GUI window for this Device object.
    
    \param pUpdatedRect [out]  Pointer to the a rectangle to receive the region updated by this function.

    \remarks
    This function call is propagated to all OdGsView objects owned by this Device object,
    thus displaying the correct image on the GUI window of this Device object.
  */
  virtual void update(OdGsDCRect* pUpdatedRect = 0) = 0;

  /** \details
    Notification function called whenever the size of the GUI window for this Device object has changed.
    
    \param outputRect [in]  New size of the output rectangle in device coordinates (pixels).

    \remarks
    This function should not be called if this Device object has a width and/or height of 0, .
  */
  virtual void onSize(const OdGsDCRect& outputRect) = 0;
  
  /** \details
    Notification function called whenever the size of the GUI window for this Device object has changed.
    
    \param outputRect [in]  New size of the output rectangle in device coordinates (pixels).

    \remarks
    This function should not be called if this Device object has a width and/or height of 0, .
  */
  virtual void onSize(const OdGsDCRectDouble& outputRect);

  /** \details
    Returns device surface output rectangle.

    \param outputRect [out]  Obtains size of the output rectangle in device coordinates (pixels).
  */
  virtual void getSize(OdGsDCRect& outputRect) const = 0;
  
  /** \details
    Returns device surface output rectangle.

    \param outputRect [out]  Obtains size of the output rectangle in device coordinates (pixels).
  */
  virtual void getSize(OdGsDCRectDouble& outputRect) const;

  /** \details
    
    \remarks
    Causes this Device object to realize its foreground palette.
  */
    virtual void onRealizeForegroundPalette() = 0;

  /** \details
    Notification function typically called whenever the GUI window for this Device object loses focus.
    
    \remarks
    Causes this Device object to realize its background palette.
  */
    virtual void onRealizeBackgroundPalette() = 0;

  /** \details
    Notification function called whenever the OS screen resolution and/or color depth have changed.
    
    \param bitsPerPixel [in]  Color depth.
    \param xPixels [in]  X pixels.
    \param yPixels [in]  Y pixels.
  
    \remarks
    This function is called after the operation.  
  */
  virtual void onDisplayChange(int bitsPerPixel, int xPixels, int yPixels) = 0;

  /** \details
    Creates a new OdGsView object, and associates it with this Device object.
    
    \param pViewInfo [in]  Pointer to the Client View Information for this Device object.
    \param enableLayerVisibilityPerView [in]  Layer visibility per viewport is supported, if and only if true.
    
    \returns
    Returns a SmartPointer to the newly created object.

    pViewInfo contains information returned by OdGiViewport::acadWindowId() and OdGiViewportDraw::viewportObjectId()
  */
  virtual OdGsViewPtr createView(
    const OdGsClientViewInfo* pViewInfo = 0, 
    bool enableLayerVisibilityPerView = false) = 0;

  /** \details
    Associates the specified Viewport object with this Device object.
    
    \param pView [in]  Pointer to the OdGsView object.
  */
  virtual void addView(OdGsView* pView) = 0;

  /** \details
    Creates a new OdGsModel object, and associates it with this Device object.
    
    \returns
    Returns a SmartPointer to the newly created object.
  */
  virtual OdGsModelPtr createModel() = 0;

  /** \details
    Checks compatibility between specified OdGsModel and this Device object.

    \param pModel [in]  Model object pointer to check compatibility.
    \returns
    true if the provided model is compatible with this device, false otherwise.
    \remarks
    Use this method to check compatibility of exist Model object and newly created device
    to be sure the exist Model object could be used with this Device object.
    By default this always returns false. It is must be implemented in inherited classes
    to enable Model object reuse support. If Device object use overriden Model object
    it should return false if input Model object doesn't overriden.
    Typical way to check compatibility is using of RTTI.
    If your metafiles cache doesn't support per-device sharing override and return false.
  */
  virtual bool isModelCompatible(OdGsModel* pModel) const;

  /** \details
    Stores current device state into filer object.

    \param pFiler [out]  Filer object.
    \returns
    true if device state is successfully saved, false if device state saving is not supported.
  */
  virtual bool saveDeviceState(OdGsFiler *pFiler) const;
  /** \details
    Restores device state from filer object.

    \param pFiler [in]  Filer object.
    \returns
    true if device state is successfully loaded, false if device state loading is not supported.
  */
  virtual bool loadDeviceState(OdGsFiler *pFiler);

  /** \details
    Inserts the specified Viewport object to the specified position in this Device object.

    \param viewIndex [in]  View index.
    \param pView [in]  Pointer to the OdGsView object.
  */
  virtual void insertView(int viewIndex, OdGsView* pView) = 0;

  /** \details
    Erases the specified View object.
    \param pView [in]  Pointer to the OdGsView object.

    \returns
    Returns true if and only if successful.
  */
  virtual bool eraseView(OdGsView* pView) = 0;
  
  /** \details
    Erases the specified View object.

    \param viewIndex [in]  View index.
    \returns
    Returns true if and only if successful.
  */
  virtual bool eraseView(int viewIndex) = 0;

  /** \details
    Erases all views associated with this Device object.
  */
  virtual void eraseAllViews() = 0;

  /** \details
    Returns the number of views associated with this Device object.
  */
  virtual int numViews() const = 0;

  /** \details
    Returns the specified OdGsView object associated with this Device object.
    \param viewIndex [in]  View index.
  */
  virtual OdGsView* viewAt(int viewIndex) = 0;

  /** \details
    Sets the Background Color of the GUI window of this Device object.
    \param backgroundColor [in]  Background color.
  */
  virtual bool setBackgroundColor(ODCOLORREF backgroundColor) = 0;

  /** \details
    Returns the Background Color of the GUI window of this Device object.
  */
  virtual ODCOLORREF getBackgroundColor() = 0;

  /** \details
    Sets the logical pallete to be used by this Device object.
    
    \param logicalPalette [in]  Logical palette.
    \param numColors [in]  Number of colors in palette.
    
    \remarks
    The logical pallete is used with calls to OdGiSubEntityTraits::setColor().
  */
  virtual void setLogicalPalette(const ODCOLORREF* logicalPalette, int numColors) = 0;

  /** \details
    Returns the logical pallete used by this Device object.

    \param numColors [out]  Number of colors in palette.
  */
  virtual const ODCOLORREF* getLogicalPalette(int &numColors) const = 0;

  /** \details
    Returns image of current device buffer.

    \param pImage [out]  Pointer for image to return.
    \param region [in]  Rectangle specifies region to return.

    \remarks
    Implementation can throw eNotImplementedYet exception if current vectorization module doesn't support snap shots.
  */
  virtual void getSnapShot(OdGiRasterImagePtr &pImage, const OdGsDCRect &region) = 0;

  /** \details
    Returns OdGsUpdateManager associated with current device

    \param createIfNotExist [in] Creates instance of OdGsUpdateManager if no one associated with device
  */
  virtual OdGsUpdateManagerPtr getUpdateManager( bool createIfNotExist = false );

  /** \details
    Associates OdGsUpdateManager with device

    \param pManager [in] Pointer to the OdGsUpdateManager
  */
  virtual void setUpdateManager( OdGsUpdateManagerPtr pManager );
};

class OdGsModule;

/** \details
    This class is the base class for custom classes that receive notification
    of OdGs events.
    
    \remarks
    The default implementations of all methods in this class do nothing but return.

    \sa
    TD_Gs
  
    <group OdGs_Classes> 
*/
class FIRSTDLL_EXPORT OdGsReactor : public OdRxObject
{
public:
  /** \details
    Notification function called whenever a OdGsView object has been created.
    \param pView [in]  Pointer to the OdGsView object.

    \remarks
    This function is called after the operation.
  */
  virtual void viewWasCreated(OdGsView* pView);

  /** \details
    Notification function called whenever an OdGsView object is about to be destroyed.
    \param pView [in]  Pointer to the OdGsView object.

    \remarks
    This function is called before the operation.
  */
  virtual void viewToBeDestroyed(OdGsView* pView);

  /** \details
    Notification function called before OdGsView properties are updated.
    \param pView [in]  Pointer to the OdGsView object.
    \param flags [in]  Bitcoded flags of type ViewUpdateFlags depending on which properties are effected.
  */
  virtual void viewToBeUpdated(OdGsView* pView, int flags);

  /** \details
    Notification function called after OdGsView properties are updated.
    \param pView [in]  Pointer to the OdGsView object.
    \param flags [in]  Bitcoded flags of type ViewUpdateFlags depending on which properties are effected.
  */
  virtual void viewWasUpdated(OdGsView* pView, int flags);

  /** \details
    Notification function called whenever an OdGsModule object is about to be unloaded.
    \param pModule [in]  Pointer to the OdGsModule object.

    \remarks
    This function is called after the operation.
  */
  virtual void gsToBeUnloaded(OdGsModule* pModule);
};

/** \details
    This class provides management of OdGsDevice classes.
    Corresponding C++ library: TD_Gs 
    <group OdGs_Classes> 
*/
class FIRSTDLL_EXPORT OdGsModule : public OdRxModule
{
public:
  ODRX_DECLARE_MEMBERS(OdGsModule);

  /** \details
    Creates a new OdGsDevice object, and associates it with this Module object.
    \returns
    Returns a SmartPointer to the newly created object.
  */
  virtual OdGsDevicePtr createDevice() = 0;

  /** \details
    Creates a new OdGsBaseVectorizeDevice object, and associates it with this Module object.
    \returns
    Returns a SmartPointer to the newly created object.
  */
  virtual OdGsDevicePtr createBitmapDevice();

  /** \details
    Adds the specified reactor to this object's reactor list.
    \param pReactor [in]  Pointer to the reactor object.
  */
  virtual void addReactor(OdGsReactor* pReactor) = 0;

  /** \details
    Removes the specified reactor from this object's reactor list.
    \param pReactor [in]  Pointer to the reactor object.
  */
  virtual void removeReactor(OdGsReactor* pReactor) = 0;
};

/** \details
  This template class is a specialization of the OdSmartPtr class for OdGsModule object pointers. 
*/
typedef OdSmartPtr<OdGsModule> OdGsModulePtr;

/** \details
  Default Gs Marker value for highlight entire entity.
*/
const OdGsMarker kNullSubentIndex = 0;

#include "TD_PackPop.h"

#endif // __GS_H_INCLUDED_
