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

#ifndef ODGSVIEWIMPL_INC
#define ODGSVIEWIMPL_INC

#include "TD_PackPush.h"
#include "Gs/Gs.h"
#include "Gs/GsBaseModule.h"
#include "Gs/GsDCRectArray.h"
#include "DbStubPtrArray.h"
#include "Ge/GePoint3d.h"
#include "Ge/GeVector3d.h"
#include "Ge/GeMatrix3d.h"
#include "UInt8Array.h"
#include "SharedPtr.h"
#include "Gs/GsDrawableHolder.h"
#include "Gi/GiCommonDraw.h"
#include "Gs/GsProperties.h"
#include "Gs/GsCullingVolume.h"
#include "Gs/GsViewLocalId.h"
#include "Gs/GsOverlayDefs.h"
#include "Gi/GiAbstractClipBoundary.h"
#include "ThreadsCounter.h"

class OdGsBaseVectorizeDevice;
class OdGeBoundBlock3d;
class OdGiPathNode;

struct OdSiShape;
typedef OdArray<const OdSiShape*, OdMemoryAllocator<const OdSiShape*> > OdSiShapeConstPtrArray;

/** \details
    This class implements Viewport objects in GUI display windows.
    
    Corresponding C++ library: Gs
    
    <group OdGs_Classes> 
*/
class GS_TOOLKIT_EXPORT OdGsViewImpl : public OdGsView
{
public:
  ODRX_DECLARE_MEMBERS(OdGsViewImpl);

  /** \details
    Default constructor for the OdGsViewImpl class. Sets the following data members:
    <table>
    Data member                             Value                                             Description
    m_gsViewImplFlags                       kDefaultLightingEnabled                           View implementation specific flags
    m_pDevice                               0                                                 Owning device
    m_borderColor                           ODRGB(0,0,0)                                      View border color
    m_borderWeight                          0                                                 View border width
    m_frontClipDist                         1.0                                               Front clipping distance
    m_backClipDist                          0.0                                               Back clipping distance
    m_position                              OdGePoint3d::kOrigin - OdGeVector3d::kZAxis       Camera position
    m_target                                OdGePoint3d::kOrigin                              Camera target
    m_upVector                              OdGeVector3d::kYAxis                              Camera "up" vector
    m_dcUpperRight                          1.0,1.0                                           Upper right corner of window in device coordinates
    m_dcScreenMax                           1.0,1.0                                           Maximum screen dimensions in device coordinates
    m_screenInvertedX                       false                                             Screen inverted by x axis
    m_screenInvertedY                       true                                              Screen inverted by x axis
    m_fieldWidth                            1.0                                               Camera field width
    m_fieldHeight                           1.0                                               Camera field height
    m_lensLength                            50.                                               Camera lens length
    m_eyeVecLength                          1.                                                Camera eye vector length
    m_linetypeScaleMultiplier               1.0                                               Linetype scale multiplier
    m_linetypeAlternateScaleMultiplier      1.0                                               Alternative linetype scale multiplier
    m_lineweightToDcScale                   1.0                                               Lineweight scale relative to device coordinates
    m_nCachedDrawables                      0                                                 Number of cached drawables
    m_pBackgroundId                         NULL                                              Identifier of a view background
    m_pVisualStyleId                        NULL                                              Identifier of a visual style
    m_defaultLightingType                   OdGsView::kTwoLights                              Default lighting type
    m_regenerationType                      kOdGiStandardDisplay                              Regeneration type
    m_renderMode                            k2DOptimized                                      Render mode
    m_xVector                               OdGeVector3d::kXAxis                              Vector that defines x axis.
    m_eyeVector                             OdGeVector3d::kZAxis                              Camera eye vector
    m_bInInteractivityMode                  false                                             Flag whether to use interactivity mode
    m_dInteractivityFrameRate               1.0                                               Frame rate for the interactivity mode
    </table>
    This constructor also sets this view as invalid and sets clear color to kPaletteBackground.
  */
  OdGsViewImpl();
  
  /** \details
    Destructor for the OdGsViewImpl class. Calls onFinalRelease(), eraseAll() and frees device slot manager slot from viewport id, if necessary.
  */
  ~OdGsViewImpl();

  // OdGsView methods

  /** \details
    Initializes this Viewport object.
    
    \param pDevice    [in]  Pointer to the OdGsBaseVectorizeDevice associated with this Viewport object.
    \param pViewInfo  [in]  Pointer to the Client View Information for this Viewport object.
    \param enableLayerVisibilityPerView [in]  Layer visibility per viewport is supported, if and only if true.
  */
  virtual void init(OdGsBaseVectorizeDevice* pDevice, 
    const OdGsClientViewInfo* pViewInfo = 0, 
    bool enableLayerVisibilityPerView = false);

  /** \details
    Returns the GsDevice object associated with this Viewport object.
    \sa
    <link tv_working_with_base_classes_device, Work with Visualize Graphics Devices>
  */
  virtual OdGsDevice* device() const;

  /** \details
    Returns the ClientViewInfo for this Viewport object.
    
    \param clientViewInfo [out]  Receives the ClientViewInfo.
  */
  virtual void clientViewInfo(OdGsClientViewInfo& viewInfo) const;

  
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
  virtual RenderMode mode() const;
  
  /** \details
    Sets the render mode override for this Viewport object.
    
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
  bool setModeOverride(RenderMode mode);
  
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
  virtual void setMode(RenderMode mode);

  
  /** \details
    Freezes the specified Layer object in this Viewport object.
    \param layerID [in]  Layer to freeze.
    \remarks
    All layers are thawed by default.
    \sa
    <link tv_working_with_attributes_layers, Work with Layers>
  */
  virtual void freezeLayer(OdDbStub* layerID);
  
  /** \details
    Thaws the specified Layer object in this Viewport object.
    \param layerID [in]  Layer to thaw.
    \remarks
    All layers are thawed by default.
    \sa
    <link tv_working_with_attributes_layers, Work with Layers>
  */
  virtual void thawLayer(OdDbStub* layerID);
  
  /** \details
    Thaws all Layer objects in this Viewport object.
    \remarks
    All layers are thawed by default.
    \sa
    <link tv_working_with_attributes_layers, Work with Layers>
  */
  virtual void clearFrozenLayers();

  
  /** \details
    Sets the color and width of the border for this Viewport object.
    \param color [in]  Border color.
    \param width [in]  Border width in pixels.
  */
  virtual void setViewportBorderProperties(ODCOLORREF color, int width);
  
  /** \details
    Returns the color and width of the border for this Viewport object.
    \param color [out]  Receives the border color.
    \param width [out]  Receives the border width in pixels.
  */
  virtual void getViewportBorderProperties(ODCOLORREF& color, int& width) const;
  
  /** \details
    Controls the border visibility for this Viewport object.
    \param visible [in]  Controls visibility.
  */
  virtual void setViewportBorderVisibility(bool visible);
  
  /** \details
    Checks whether border is visible for this Viewport object.
    \returns 
    true if the border is visible for this Viewport object, false otherwise.
  */
  virtual bool isViewportBorderVisible() const;

  
  /** \details
    Controls the front clipping of this Viewport object.
    \param enable [in]  Enables or disables front clipping.
  */
  virtual void setEnableFrontClip(bool enable);
  
  /** \details
    Checks whether front clipping is enabled for this Viewport object.
    \returns 
    true if front clipping is enabled for this Viewport object, false otherwise.
  */
  virtual bool isFrontClipped() const;
  
  /** \details
    Sets the front clip distance from the target of this Viewport object.
    \param distance [in]  Front clip distance.
  */
  virtual void setFrontClip(double distance);
  
  /** \details
    Returns the front clip distance from the target of this Viewport object.
  */
  virtual double frontClip() const;

  /** \details
    Controls the back clipping of this Viewport object.
    \param enable [in]  Enables or disables back clipping.
  */
  virtual void setEnableBackClip(bool enable);
  
  /** \details
    Checks whether back clipping is enabled for this Viewport object.
    \returns 
    true if back clipping is enabled for this Viewport object, false otherwise.
  */
  virtual bool isBackClipped() const;
  
  /** \details
    Sets the back clip distance from the target of this Viewport object.
    \param distance [in]  Back clip distance.
  */
  virtual void setBackClip(double distance);
  
  /** \details
    Returns the back clip distance from the target of this Viewport object.
  */
  virtual double backClip() const;

  
  
  /** \details
    Returns the WCS camera position for this Viewport object.
  */
  virtual OdGePoint3d position() const;
  
  /** \details
    Returns the WCS camera target for this Viewport object.
  */
  virtual OdGePoint3d target() const;
  
  /** \details
    Returns the WCS camera up vector for this Viewport object.
  */
  virtual OdGeVector3d upVector() const;
  
  /** \details
    Returns the WCS projection plane (field) width for this Viewport object.
  */
  virtual double fieldWidth() const;
  
  /** \details
    Returns the WCS projection plane (field) height for this Viewport object.
  */
  virtual double fieldHeight() const;
  /** \details
    Returns the width to height ratio for this view object.
  */
  double windowAspect() const;

  /** \details
    Returns the perspective lens length for this Viewport object.
  */
  virtual double lensLength() const;
  
  /** \details
    Sets the perspective lens length for this Viewport object.
    \param lensLength [in]  Perspective lens length.
  */
  virtual void setLensLength(double lensLength);

  /** \details
    Returns true if and only if the projection type for this Viewport object is kPerspective. 
  */
  virtual bool isPerspective() const;

  
  
  /** \details
    Puts this Viewport object into interactive state.
    
    \param frameRateInHz [in]  Frames/second.
    
    \remarks
    A frame rate of 0.0 specifies the default frame rate.
    \sa
    <link gs_views_interactivity_mode, Work with Interactivity Mode for Views>
  */
  virtual void beginInteractivity(double frameRateInHz);
  
  /** \details
    Checks whether this Viewport object is in interactive state.
    \returns
    true if this View is in interactivity mode, false otherwise.
    \sa
    <link gs_views_interactivity_mode, Work with Interactivity Mode for Views>
  */
  virtual bool isInInteractivity() const;
  
  /** \details
    Returns interactivity frame rate.
    \sa
    <link gs_views_interactivity_mode, Work with Interactivity Mode for Views>
  */
  virtual double interactivityFrameRate() const;
  
  /** \details
    Removes this Viewport object from the interactive state.
    \param frameRateInHz [in]  Frames/second.
    \sa
    <link gs_views_interactivity_mode, Work with Interactivity Mode for Views>
  */
  virtual void endInteractivity();
  
  /** \details
    Flushes the graphic pipeline and renders this Viewport object.
    
    \remarks
    This function should be called only during interactivty. 
  */
  virtual void flush();

  
  
  /** \details
    Hides this Viewport object.
  */
  virtual void hide();
  
  /** \details
    Un-hides this Viewport object.
  */
  virtual void show();
  
  /** \details
    Checks whether this Viewport object is visible.
    \returns 
    true if this Viewport object is not hidden, false otherwise.
  */
  virtual bool isVisible();

  
  
  /** \details
    Clones this view object.

    \param cloneViewParameters  [in]  If this flag is set, view parameters will be cloned.
    \param cloneGeometry        [in]  If this flag is set, geometry will be cloned.
    \returns
    Returns a SmartPointer to the newly created object.
  */
  virtual OdGsViewPtr cloneView(bool cloneViewParameters = true, bool cloneGeometry = false);
  
  /** \details
    Checks whether view parameters have exceeded their boundaries since the last call to this function.
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
  virtual bool exceededBounds();
  
  /** \details
    Controls stereo viewing for this Viewport object.
    
    \param enable [in]  Controls stereo viewing.
  */
  virtual void enableStereo(bool enabled);
  
  /** \details
    Checks whether stereo viewing is enabled for this Viewport object.
    \returns
    true if stereo viewing is enabled for this Viewport object, false otherwise.
  */
  virtual bool isStereoEnabled() const;
  
  /** \details
    Sets the stereo parameters for this Viewport Object.
    
    \param magnitude  [in]  View separation [0.0 .. 2.0].
    \param parallax   [in]  Adjusts the plane of zero parallax [0.0 .. 2.0].
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
  virtual void setStereoParameters(double magnitude, double parallax);
  
  /** \details
    Returns the stereo parameters for this Viewport Object.
    
    \param magnitude  [out]  Receives the view separation [0.0 .. 2.0].
    \param parallax   [out]  Receives the plane of zero parallax adjustment [0.0 .. 2.0].
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
  virtual void getStereoParameters(double& magnitude, double& parallax) const;
  
  /** \details
    Returns an iterator to traverse the lights attached to this Viewport object.
    \param pLightsIterator [in]  Pointer to the lights iterator.
  */
  virtual void initLights(OdRxIterator* pLightsIterator);
  
  /** \details
    Returns image of current view buffer.

    \param pImage [out]  Pointer for image to return.
    \param region [in]  Rectangle specifies region to return.
    \remarks
    Implementation can throw eNotImplementedYet exception if current vectorization module doesn't support snap shots.
  */
  virtual void getSnapShot(OdGiRasterImagePtr &pImage, const OdGsDCRect &region);

  
  
  /** \details
    Sets the size and position of this Viewport object.
    
    \param lowerLeft  [in]  Lower-left corner in nomalized device coordinates [0.0 .. 1.0].
    \param upperRight [in]  Upper-right corner in nomalized device coordinates [0.0 .. 1.0].   
    \remarks
    (0,0) is the lower-left corner of the owning Device object. Coordinates increase upward and to the right.
    The View object can be partly or completely off screen.    
  */
  virtual void setViewport(const OdGePoint2d& lowerLeft, const OdGePoint2d& upperRight);
  
  /** \details
    Sets the size and position of this Viewport object.
    
    \param screenRect [in]  Screen rectangle in device coordinates (pixels).    
    \remarks
    (0,0) is the lower-left corner of the owning Device object. Coordinates increase upward and to the right. 
    \remarks
    The View object can be partly or completely off screen.    
  */
  virtual void setViewport(const OdGsDCRect& screenRect);
  
  /** \details
    Sets the size and position of this Viewport object.
    
    \param screenRect [in]  Screen rectangle in device coordinates (pixels).    
    \remarks
    (0,0) is the lower-left corner of the owning Device object. Coordinates increase upward and to the right.
    The View object can be partly or completely off screen.    
  */
  virtual void setViewport(const OdGsDCRectDouble& screenRec);

  /** \details
    Returns the size and position of this Viewport object.
    
    \param lowerLeft  [out]  Receives the lower-left corner in nomalized device coordinates [0.0 .. 1.0].
    \param upperRight [out]  Receives the upper-right corner in nomalized device coordinates [0.0 .. 1.0].
    \param screenRect [out]  Receives the screen rectangle in device coordinates (pixels).
    \remarks
    (0,0) is the lower-left corner of the owning Device object.  Coordinates increase upward and to the right.    
    \remarks
    The View object may be partly or completely off screen.    
  */
  virtual void getViewport(OdGePoint2d& lowerLeft, OdGePoint2d& upperRight) const;
  
  /** \details
    Returns the size and position of this Viewport object.
    
    \param lowerLeft  [out]  Receives the lower-left corner in nomalized device coordinates [0.0 .. 1.0].
    \param upperRight [out]  Receives the upper-right corner in nomalized device coordinates [0.0 .. 1.0].
    \param screenRect [out]  Receives the screen rectangle in device coordinates (pixels).
    \remarks
    (0,0) is the lower-left corner of the owning Device object.  Coordinates increase upward and to the right.
    \remarks
    The View object may be partly or completely off screen.    
  */
  virtual void getViewport(OdGsDCRect& screenRect) const;
  
  /** \details
    Returns the size and position of this Viewport object.
    
    \param screenRect [out]  Receives the screen rectangle in device coordinates (pixels).
    \remarks
    (0,0) is the lower-left corner of the owning Device object.  Coordinates increase upward and to the right.
    The View object may be partly or completely off screen.    
  */
  virtual void getViewport(OdGsDCRectDouble& screenRec) const;

  
  
  /** \details
    Returns the matrix that transforms normalized device space to screen space for this Viewport object.
  */
  virtual OdGeMatrix3d screenMatrix() const;
  
  /** \details
    Returns a matrix that transforms coordinates from world space to screen space for this Viewport object. 
    
    \remarks
    This is equivalent to the concatenation of the viewingMatrix, 
    projectionMatrix, and screenMatrix.
  */
  virtual OdGeMatrix3d worldToDeviceMatrix() const;
  
  /** \details
    Returns the matrix that transforms coordinates from model space to screen space for this Viewport object.
  */
  virtual OdGeMatrix3d objectToDeviceMatrix() const;
  
  /** \details
    Returns the matrix that transforms world space to view space for this Viewport object.
  */
  virtual OdGeMatrix3d viewingMatrix() const;
  
  /** \details
    Returns the matrix that transforms view space to normalized device space for this Viewport object.
  */
  virtual OdGeMatrix3d projectionMatrix() const;

  /** \details
    Sets the view parameters of the specified OdGsView object to that of this Viewport object.
    \param pView [out]  Receives the view parameters.
  */
  virtual void viewParameters(OdGsView* pView) const;

  
  
  /** \details
    Returns the scale factor for displaying Lineweights in this Viewport object.
    \sa
    <link tv_working_with_attributes_lineweights, Work with Lineweights>
  */
  virtual double lineweightToDcScale() const;
  
  /** \details
    Sets the scale factor for displaying Lineweights in this Viewport object.
    \param scale [in]  Scale factor.
    \sa
    <link tv_working_with_attributes_lineweights, Work with Lineweights>
  */
  virtual void setLineweightToDcScale(double scale);
  
  /** \details
    Sets the lineweights for this Viewport object.
    \param numLineweights [in]  Number of Lineweights.
    \param lineweights [in]  Array of Lineweights.
    \sa
    <link tv_working_with_attributes_lineweights, Work with Lineweights>
  */
  virtual void setLineweightEnum(int numLineweights, const OdUInt8* lineweights);
  
  /** \details
    Sets a multiplier that is used to scale all linetypes in this Viewport object.
    
    \param linetypeScaleMultiplier [in]  Linetype scale multiplier.
    \sa
    <exref target="https://docs.opendesign.com/td/db_linetype.html">Working with Linetypes</exref>
  */
  virtual void setLinetypeScaleMultiplier(double linetypeScaleMultiplier);
  
  /** \details
    Sets a multiplier that could be used as an alternate scale factor for linetypes in this Viewport object.
    
    \param linetypeAlternateScaleMultiplier [in]  Alternate linetype scale multiplier.
    \sa
    <exref target="https://docs.opendesign.com/td/db_linetype.html">Working with Linetypes</exref>
  */
  virtual void setAlternateLinetypeScaleMultiplier(double linetypeAlternateScaleMultiplier);
  
  /** \details
    Returns current linetype scale multiplier value.
    \sa
    <exref target="https://docs.opendesign.com/td/db_linetype.html">Working with Linetypes</exref>
  */
  virtual double linetypeScaleMultiplier() const;



  /** \details
    Translates the camera position and target by the specified Camera Coordinate dolly amount.
     
    \param dollyVector [in]  Camera Coordinate dolly vector. 
    \remarks
    Camera Coordinates are relative to the view of a target from a camera. 
    * The Y-axis is along up vector. 
    * The Z-axis is along the vector from camera position to the target. 
    * The X-axis is the cross product Y-axis x Z-axis 
  */
  virtual void dolly(const OdGeVector3d& dollyVector);
  
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
  virtual void dolly(double xDolly, double yDolly, double zDolly);
  
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
  virtual void roll(double rollAngle);
  
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
  virtual void orbit(double xOrbit, double yOrbit);
  
  /** \details
    Scales the focal length of the camera by the specified amount.
     
    \param zoomFactor [in]  Zoom factor.
  */
  virtual void zoom(double zoomFactor);
  
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
  virtual void pan(double xPan, double yPan);
  
  /** \details
    Sets the camera parameters for this Viewport object.
    
    \param position       [in]  Camera position.
    \param target         [in]  Camera target.
    \param upVector       [in]  Camera up vector.
    \param fieldWidth     [in]  Projection plane (field) width.
    \param fieldHeight    [in]  Projection plane (field) height.
    \param projectionType [in]  Projection type.    
    \remarks
    This method defines the transformation from WCS coordinates to normalized device coordinates.
    All parameters are in WCS coordinates.
    projectionType must be one of the following:
    
    <table>
    Name           Value
    kParallel      0
    kPerspective   1
    </table>
  */
  virtual void setView(const OdGePoint3d & position,
    const OdGePoint3d& target,
    const OdGeVector3d& upVector,
    double fieldWidth,
    double fieldHeight,
    Projection projection = kParallel);

  
  
  /** \details
    Scales the camera to completely include specified WCS box inside view frustum.

    \param minPt [in]  minimal WCS box point.
    \param maxPt [in]  maximal WCS box point.
  */
  virtual void zoomExtents(const OdGePoint3d &minPt, const OdGePoint3d &maxPt);
  
  /** \details
    Scales the camera to completely include specified screen area.

    \param lowerLeft  [in]  Lower-left corner in nomalized device coordinates [0.0 .. 1.0].
    \param upperRight [in]  Upper-right corner in nomalized device coordinates [0.0 .. 1.0].
  */
  virtual void zoomWindow(const OdGePoint2d &lowerLeft, const OdGePoint2d &upperRight);
  
  /** \details
    Returns true if and only if specified point is visible inside current view frustum.

    \param pt [in]  WCS point for check.
  */
  virtual bool pointInView(const OdGePoint3d &pt) const;
  
  /** \details
    Returns true if and only if specified WCS box is visible completely or partially inside current view frustum.

    \param minPt [in]  minimal WCS box point.
    \param maxPt [in]  maximal WCS box point.
  */
  virtual bool extentsInView(const OdGePoint3d &minPt, const OdGePoint3d &maxPt) const;

  
  
  /** \details
    Returns a polygonal clip region for this Viewport object.
    
    \param counts   [out]  Array of the number of vertices in each polygonal contour.
    \param vertices [out]  Array of device coordinate (pixel) vertices defining the polygonal contours.
  */
  virtual void viewportClipRegion(OdIntArray& counts, OdGePoint2dArray& vertices) const;
  
  /** \details
    Returns a polygonal clip region for this Viewport object.
    
    \param counts   [out]  Array of the number of vertices in each polygonal contour.
    \param vertices [out]  Array of device coordinate (pixel) vertices defining the polygonal contours.
  */
  virtual void viewportClipRegion(OdIntArray& counts, OdGsDCPointArray& dcPts) const;
  
  /** \details
    Defines a polygonal clip region for this Viewport object.
    
    \param numCoutours  [in]  Number of polygonal contours.
    \param numVertices  [in]  Array of the number of vertices in each polygonal contour.
    \param vertices     [in]  Array of device coordinate (pixel) vertices defining the polygonal contours.
  */
  virtual void setViewportClipRegion(int numContours, const int* numVertices, const OdGePoint2d* vertices);
  
  /** \details
    Defines a polygonal clip region for this Viewport object.
    
    \param numCoutours  [in]  Number of polygonal contours.
    \param numVertices  [in]  Array of the number of vertices in each polygonal contour.
    \param vertices     [in]  Array of device coordinate (pixel) vertices defining the polygonal contours.
  */
  virtual void setViewportClipRegion(int numContours, int const* numVertices, OdGsDCPoint const* vertices);

  bool isDependentViewportView() const;
  bool isDependentGeometryView() const;
  bool isHelperView() const;

  
  /** \details
    Retrieves pointer to the associated GS module.
  */
  const OdGsBaseModule *getGsModulePtr() const;

  /** \details
    Returns pointer to context-dependent colors storage.

    \remarks
    This pointer could be null if contextual colors doesn't specified by context.
  */
  const OdGiContextualColors *contextualColors() const;
  
  /** \details
    Sets contextual colors.

    \param pColors [in]  Pointer to the context-dependent colors storage.
  */
  void setContextualColors(OdGiContextualColors *pColors);

  /** \details
    Checks whether the specified point is inside viewport area.

    \param screenPoint [in]  Check point in device coordinate space.
    \returns
    true if the passed point is inside the viewport, false otherwise.
  */
  virtual bool pointInViewport(const OdGePoint2d& screenPoint) const;

  /** \details
    Sets the clear color for this view.

    \param color [in]  The clear color.
  */
  virtual void setClearColor(ClearColor color);
  
  
  
  /** \details
    Marks the entire area of this View for refreshing.
  */
  virtual void invalidate();
  
  /** \details
    Marks for refresh the specified region of this Viewport object.
     
    \param rect [in]  Screen rectangle in device coordinates (pixels).
   */
  virtual void invalidate(const OdGsDCRect &rect);
  
  /** \details
    Marks for regeneration all subViewportDraw() geometry for this Viewport object.
  */
  virtual void invalidateCachedViewportGeometry();
  /** \details
    Evokes the specified OdGsSelectionReactor object for the specified polygon in this VectorizeView object.
    \param points     [in]  Array of points.
    \param numPoints  [in]  Number of points.
    \param pReactor   [in]  Pointer to the reactor. 
    \param mode       [in]  Selection mode. 
  */
  virtual void select(const OdGsDCPoint* points, int numPoints, OdGsSelectionReactor* pReactor,
    SelectionMode mode = kCrossing);

  /** \details
    Adds the specified scene graph to this Viewport object.
    
    \param pSceneGraph  [in]  Pointer to the root of the scene graph.
    \param pModel       [in]  Pointer to the Model object to which all scene graph notifications are to be directed.
    
    \remarks
    This function is intended to be used as follows:
    
    1.  Construct a scene graph of OdGiDrawable objects. 
        Each object draws its children with OdGiGeometry::draw().
    
    2.  With this function, add the root of the scene graph and the 
        OdGsModel that handles notifications to this Viewport object.
  */
  virtual bool add(OdGiDrawable* sceneGraph, OdGsModel* model);
  
  /** \details
    Retrieves a quantity of root drawables for this viewport.
  */
  virtual int numRootDrawables() const;
  
  /** \details
    Retrieves a persistent object identifier of the root drawable at the specified index.
    \param i [in]  Index of the drawable to return.
    \param pModelReturn [in/out]  If not NULL, receives a pointer to model of associated drawable holder.
  */
  virtual OdDbStub* rootDrawableIdAt(int i, OdGsModelPtr* pModelReturn = 0) const;
  
  /** \details
    Retrieves a smart pointer to transient root drawable at the specified index.
    \param i [in]  Index of the drawable to return.
    \param pModelReturn [in/out]  If not NULL, receives a pointer to model of associated drawable holder.
  */
  virtual OdGiDrawablePtr rootDrawableAt(int i, OdGsModelPtr* pModelReturn = 0) const;
  
  /** \details
    Removes the specified scene graph from this Viewport object.
    
    \param sceneGraph [in]  Pointer to the root of the scene graph.
  */
  virtual bool erase(OdGiDrawable* sceneGraph);
  
  /** \details
    Removes all scene graphs from this Viewport object.
    
    \param sceneGraph [in]  Root of scene graph.
  */
  virtual void eraseAll();

  
  
  /** \details
    Returns OdGsModel object associated with specified OdGiDrawable object in this Viewport object.

    \param pDrawable [in]  Pointer onto OdGiDrawable object.
  */
  virtual OdGsModel *getModel(const OdGiDrawable *pDrawable) const;
  
  /** \details
    Returns array of OdGsModel objects associated with this Viewport object.
  */
  virtual OdGsModelArray getModelList() const;

  /** \details
    Checks whether this Viewport shows the correct image.
    \returns
    true if this Viewport object is showing the correct image, false otherwise.

    \remarks
    This implies the following:
    * No part of GUI area is flagged for refresh.
    * All rasterizations are complete
    * The rendering pipeline has been flushed.
  */
  virtual bool isValid() const;
  
  /** \details
    Returns the display pixel density at the specified point for this Viewport object.

    \param givenWorldpt         [in]  WCS center of the unit square.
    \param pixelArea            [out]  Receives the pixel density.
    \param includePerspective   [in]  Enable perspective inclusion into pixel density calculation.
    \remarks
    Pixel density is measured in pixels per WCS unit.
    This function can be used to determine if the geometry generated for an object will 
    be smaller than the size of a pixel.
  */
  virtual void getNumPixelsInUnitSquare(const OdGePoint3d& givenWorldpt,
    OdGePoint2d& pixelArea, bool includePerspective = true) const;

  /** \details
    Sets background object for this view.

    \param backgroundId [in]  New background object ID.
    \sa
    <link tv_working_with_attributes_view_backgrounds, Work with View Backgrounds>
  */
  virtual void setBackground(OdDbStub *backgroundId);
  
  /** \details
    Returns current background object ID for this view.
    \sa
    <link tv_working_with_attributes_view_backgrounds, Work with View Backgrounds>
  */
  virtual OdDbStub *background() const;
  
  /** \details
    Sets visual style object for this view.

    \param visualStyleId [in]  New visual style object ID.
    \sa
    <link tv_working_with_attributes_visual_styles, Work with Visual Styles>
  */
  virtual void setVisualStyle(OdDbStub *visualStyleId);
  
  /** \details
    Returns current visual style object ID for this view.
    \sa
    <link tv_working_with_attributes_visual_styles, Work with Visual Styles>
  */
  virtual OdDbStub *visualStyle() const;
  
  /** \details
    Sets visual style for this view.

    \param visualStyle [in]  New visual style data.
    \sa
    <link tv_working_with_attributes_visual_styles, Work with Visual Styles>
  */
  virtual void setVisualStyle(const OdGiVisualStyle &visualStyle);
  
  /** \details
    Returns current visual style for this view.

    \param visualStyle [out]  OdGiVisualStyle structure to fill by current visual style data.

    \returns
    true if OdGiVisualStyle structure is filled by visual style data.
    \sa
    <link tv_working_with_attributes_visual_styles, Work with Visual Styles>
  */
  virtual bool visualStyle(OdGiVisualStyle &vs) const;
  
  /** \details
    Enables default lighting for this view.

    \param bEnable    [in]  New default lighting state.
    \param lightType  [in]  New default lighting type.
    \sa
    <link tv_working_with_lights, Work with Lights>
  */
  virtual void enableDefaultLighting(bool bEnable, DefaultLightingType lightType = kTwoLights);
  
  /** \details
    Checks whether default lighting is enabled.
    \returns 
    true if default lighting is enabled, false otherwise.
    \sa
    <link tv_working_with_lights, Work with Lights>
  */
  bool defaultLightingEnabled() const;
  
  /** \details
    Returns default lighting type which can be one of the following:
    <table>
    Name                            Value      Description
    kOneLight                       0          Single front light.
    kTwoLights                      1          Front and back lights.
    kBackLight                      2          Single back light.
    </table>
    \sa
    <link tv_working_with_lights, Work with Lights>
  */
  DefaultLightingType defaultLightingType() const;

  // OdGsViewImpl methods

  /** \details
    Returns the specified OdGsView pointer 
    as a pointer to an OdGsViewImpl object.
    \param pView [in]  Pointer to an OdGsView object.
  */
  static OdGsViewImpl* safeCast(OdGsView* pView);
  
  /** \details
    Returns the specified OdGsView pointer 
    as a pointer to an OdGsViewImpl object.
    \param pView [in]  Pointer to an OdGsView object.
  */
  static const OdGsViewImpl* safeCast(const OdGsView* pView);

  /** \details
    Returns the OdGi Viewport ID for this vectorizer.
    \remarks
    The value returned has no relationship to the CVPORT system variable.
  */
  OdUInt32 viewportId() const { return m_giViewportId; }
  
  /** \details
    Checks whether the passed viewport identifier is valid.
    
    \param vpId [in]  Viewport ID.
    \returns
    true if the specified viewport identifier is valid, false otherwise.
  */
  bool isValidViewportId(const OdUInt32 vpId) const;

  //NS TODO: rename to viewportObjectId()
  /** \details
    Returns the Object ID of the OdDbViewport object associated with this object.
  */
  OdDbStub* getViewportObjectId() const { return m_viewInfo.viewportObjectId; }

  /** \details
    Returns the OdGsBaseVectorizeDevice associated with this Viewport object.
  */
  OdGsBaseVectorizeDevice* baseDevice() { return m_pDevice; }
  
  /** \details
    Returns the OdGsBaseVectorizeDevice associated with this Viewport object.
  */
  const OdGsBaseVectorizeDevice* baseDevice() const { return m_pDevice; }

  /** \details
    Retrieves a pointer a persistent object ID for annotation scale object associated with this view.
  */
  OdDbStub* annotationScaleId() const;
  
  /** \details
    Retrieves internal drawable filtration function ID.
  */
  OdIntPtr drawablesFilterFunction() const;

  /** \details
    Returns the eye coordinate system extents of this view object.
    
    \param extents [out]  Receives the extents.

    \returns
    Returns true if and only if the extents are defined.
  */
  virtual bool viewExtents(OdGeBoundBlock3d& extents) const = 0;

  // Own methods
  
  
  /** \details
    Runs selection procedure inside this Viewport object.

    \param aPtDc      [in]  Selection points in device coordinate space.
    \param numPoints  [in]  Count of selection points.
    \param pReactor   [in]  Selection callback pointer.
    \param mode       [in]  Selection mode.
    \sa
    <link tv_working_with_features_selections, Work with Selections>
  */
  virtual void select(const OdGePoint2d* aPtDc, int numPoints, OdGsSelectionReactor* pReactor,
    SelectionMode mode = kCrossing) = 0;

  bool isSnapping() const;
  void setSnapping(bool val);
  bool isRegenOnDrawForbidden() const;
  void setRegenOnDrawForbidden(bool val);
  bool isAccumulateVpChanges() const;
  
  /** \details
    Clears linetype cache.   
  */
  virtual void clearLinetypeCache() = 0;
  
  /** \details
    Retrieves alternate linetype scale multiplier.   
  */
  double linetypeAlternateScaleMultiplier() const;

  // Legacy plot modes (#12611)
  // Legacy Wireframe
  virtual bool isSupportLegacyWireframeMode() const;
  virtual bool isLegacyWireframeMode() const;
  virtual void setLegacyWireframeMode(bool bSet);
  // Legacy Hidden
  virtual bool isSupportLegacyHiddenMode() const;
  virtual bool isLegacyHiddenMode() const;
  virtual void setLegacyHiddenMode(bool bSet);
  
  /** \details
    Checks whether this view supports plot styles rendering.
    \returns
    true if current view supports plot styles rendering, false otherwise.
  */
  bool isSupportPlotStyles() const;
  // Transparencies plotting
  
  /** \details
    Checks transparency is plotted for this view.
    \returns
    true if transparency is plotted for this view, false otherwise.
  */
  virtual bool isPlotTransparency() const;
  
  /** \details
    Specifies whether to plot transparency in this view.
    \param bSet       [in]  Flag that specifies whether to plot transparency.
  */
  virtual void setPlotTransparency(bool bSet);

  /** \details
    Returns count of frozen layers in this view. 
  */
  OdUInt32 frozenLayers() const { return m_frozenLayers.size(); }
  
  /** \details
    Returns the frozen layers for this view object.
    
    \param frozenLayers [out]  Receives an array of frozen layer Object IDs.
    
    \remarks
    If called without the frozenLayers argument, returns true if and only if there
    are any frozen layers.    
  */
  void frozenLayers(OdDbStubPtrArray& frozenLayers) const { frozenLayers = m_frozenLayers; }
  
  /** \details
    Checks whether the specified layer is visibile in this view.
    
    \param layerId  [in]  Persistent object identifier of a layer.
    \returns
    true if the specified layer is visibile, false otherwise.
  */
  bool isLayerVisible(OdDbStub* layerId) const;

  /** \details
    Returns the number of pixels corresponding to the specified lineweight for this view object.
    \param lineweight [in]  Lineweight.
  */
  int lineweightToPixels(OdDb::LineWeight lineweight) const;
  
  /** \details
    Returns the number of pixels corresponding to the specified lineweight for this view object.
    \param lineweight [in]  Lineweight.
  */
  double lineweightToPixels(double lineweight) const;

  /** \details
    Retrieves screen points that represent a DCS rectangle.
    \param lowerLeft  [out]  Receives lower-left corner.
    \param upperRight [out]  Receives upper-right corner.
  */
  void screenRect(OdGsDCPoint &lowerLeft, OdGsDCPoint &upperRight) const;
  
  /** \details
    Retrieves screen points that represent a DCS rectangle.
    \param lowerLeft  [out]  Receives lower-left corner.
    \param upperRight [out]  Receives upper-right corner.
  */
  void screenRect(OdGePoint2d &lowerLeft, OdGePoint2d &upperRight) const;
  
  /** \details
    Retrieves screen DCS rectangle.
    \param normalizedRect  [out]  Receives a rectangle in normalized device coordinates.
  */
  void screenRectNorm(OdGsDCRect& normalizedRect) const;
  
  /** \details
    Retrieves viewport points in normalized device coordinates.
    \param lowerLeft  [out]  Receives lower-left corner.
    \param upperRight [out]  Receives upper-right corner.
  */
  void viewportDcCorners(OdGePoint2d& lower_left, OdGePoint2d& upper_right) const;
  
  /** \details
    Applies the current perspective transformation to the specified point.

    \param point [in/out] Any 3D point.
    \remarks
    The point is transformed from EyeSpace coordinates to normalized device coordinates.
    \returns 
    true if the point was transformed; i.e., a perspective transform
    is active, and the point was neither too close or behind the camera.
  */
  bool doPerspectivePt(OdGePoint3d& point) const;
  
  /** \details
    Applies the inverse of the current perspective transformation to the specified point.

    \param point [in/out] Any 3D point.
    \remarks
    The point is transformed from normalized device coordinates to EyeSpace coordinates
    \returns 
    true the point was transformed; i.e., a perspective transform
    is active, and the point was neither too close or behind the camera.
  */
  bool doInversePerspectivePt(OdGePoint3d& point) const;

  /** \details
    Returns the transformation matrix from EyeSpace to WCS for this Viewport object.
  */
  const OdGeMatrix3d& eyeToWorldMatrix() const;
  
  /** \details
    Returns the transformation matrix from WCS to EyeSpace for this Viewport object.
  */
  const OdGeMatrix3d& worldToEyeMatrix() const;
  
  /** \details
    Returns the transformation matrix from EyeSpace to ScreenSpace for this Viewport object.
    
    \param nOverlay [in] Graphics system overlay index.
    \sa
    <link tv_graphSysOverlays, Work with Graphic System Overlays>
  */
  OdGeMatrix3d eyeToScreenMatrix(OdGsOverlayId nOverlay = kGsMainOverlay) const;
  
  /** \details
      Returns a matrix that transforms coordinates from world space to screen space for this Viewport object. 
      
      \param nOverlay [in] Graphics system overlay index.
      \remarks
      This is equivalent to the concatenation of the viewingMatrix, 
      projectionMatrix, and screenMatrix.
      \sa
      <link tv_graphSysOverlays, Work with Graphic System Overlays>
  */
  OdGeMatrix3d worldToDeviceMatrix(OdGsOverlayId nOverlay) const;
  
  /** \details
      Returns the matrix that transforms coordinates from model space to screen space for this Viewport object.
      
      \param nOverlay [in] Graphics system overlay index.
      \sa
      <link tv_graphSysOverlays, Work with Graphic System Overlays>
  */
  OdGeMatrix3d objectToDeviceMatrix(OdGsOverlayId nOverlay) const;
  
  /** \details
      Defines which components of a projection matrix are calculated.
  */
  enum ProjectionMatrixFlags
  {
    kProjectionIncludeDept        = (1 << 0),
    kProjectionIncludeClip        = (1 << 1),
    kProjectionIncludePerspective = (1 << 2),
    kProjectionIncludeRotation    = (1 << 3),
    kProjectionIncludeAll         = 0x0F,
    kProjectionIncludeAllNoDept   = kProjectionIncludeAll & ~kProjectionIncludeDept
  };
  
  /** \details
      Returns the matrix that transforms view space to normalized device space for this Viewport object.
      This method takes into account which components should be included for the matrix
      and for which overlay to compute the matrix.

      \param nIncludes  [in] Specifies which data should be included in the matrix, see ProjectionMatrixFlags.
      \param nOverlay   [in] Graphics system overlay index.
      \sa
      <link tv_graphSysOverlays, Work with Graphic System Overlays>
  */
  OdGeMatrix3d projectionMatrix(OdUInt32 nIncludes, OdGsOverlayId nOverlay = kGsMainOverlay) const;

  /** \details
    Retrieves the clear color of this view.
  */
  ClearColor clearColor() const;

  OdGsOverlayId extentsToPixels(OdGsDCRect &rc, const OdGeExtents3d& worldExt, const OdGsBaseModel *pModel, OdDb::LineWeight extendByLineweight = OdDb::kLnWt000) const;
  virtual void invalidate(const OdGeExtents3d& worldExt, const OdGsBaseModel *pModel, OdDb::LineWeight extendByLineweight = OdDb::kLnWt000);

  /** \details
    Marks a part of area (represented by provided extents) of this view as invalid.
    
    \param extents  [in] Part of view for invalidation.
    \param pModel   [in] Pointer to a model.
  */
  void invalidate(const OdGsUpdateExtents& extents, const OdGsBaseModel *pModel);
  
  /** \details
    Marks a part of area (represented by provided path node) of this view as invalid.
    
    \param path     [in] Path node for invalidation.
  */
  void invalidateRegion(const OdGiPathNode& path);

  OdGsDCRectArray invalidRects(OdGsOverlayId nOverlay = kGsMainOverlay) const;
  /** \details
    Check whether this viewport is displayed on screen.
    \returns 
    true if this view object is not off-screen.
  */
  bool isViewportOnScreen() const;

  bool isCompletelyVisible() const;
  void setCompletelyVisible(bool val);

  /** \details
    Specifies whether view should be inverted by x- or y- axis.
    
    \param invertedX   [in] Specifies whether to invert the x-axis.
    \param invertedY   [in] Specifies whether to invert the y-axis.
    \remarks 
    Since layout view can be rotated to 90, 180 or 270 degrees (for printing),
    views linked to viewports of this layout must be rotated accordingly.
    To determine viewport's rotation angle by coordinates it's corner points 
    it's necessary to know whether overall layout's window coordinates are inverted.     
    For example, overall layout's output rectangle can be (0,0)-(100,100)
    or (0,100)-(100,0).
    This means invertedX == invertedY == false in the first case and
    invertedX = false, invertedY == true in the second.
    Vieport with coordinates (10,10)-(90,90) inside this layout will be 
    considered unrotated in the first case or rotated to 270 degrees 
    in the second case.
  */
  void setInversion(bool invertedX, bool invertedY);

  /** \details
    Retrieves regeneration type of this view.
  */
  virtual OdGiRegenType getRegenType() const { return m_regenerationType; }

  /** \details
    Returns the WCS (ECS) focal length of the lens for this view object.
  */
  double focalLength() const;

  /** \details
    Retrieves eye vector of the camera. Eye vector is the vector from camera position to the target.
  */
  const OdGeVector3d& eyeVector() const { return m_eyeVector; }
  
  /** \details
    Retrieves x vector that defines camera x-axis.
  */
  const OdGeVector3d& xVector() const { return m_xVector; }
  
  /** \details
    Retrieves up vector of the camera (y-axis). Up vector defines which orientation is "upward" for camera. 
  */
  const OdGeVector3d& getUpVector() const { return m_upVector; }

  /** \details
    Retrieves a multiplier that is used to scale all linetypes in this Viewport object.
    \sa
    <exref target="https://docs.opendesign.com/td/db_linetype.html">Working with Linetypes</exref>
  */
  double getLinetypeScaleMultiplier() const;
  
  /** \details
    Returns the WCS value below which linetype generation is not used.

    \remarks
    If the WCS length an entire linetype pattern is less than the returned value, then
    the linetype will be rendered as continuous.
  */
  double getLinetypeGenerationCriteria() const;

  /** \details
    Retrieves the recommended maximum deviation of the
    current vectorization for the specified deviation type.

    \param deviationType    [in] Deviation type.
    \param pointOnCurve     [in] Point on curve. Used only if 'bRecalculate' is true.
    \param bRecalculate     [in] Flag that specifies whether to recalculate deviation value with the provided point on curve.
    
    \remarks
    deviationType must be one of the following:
    
    <table>
    Name                       Value
    kOdGiMaxDevForCircle       0      
    kOdGiMaxDevForCurve        1      
    kOdGiMaxDevForBoundary     2      
    kOdGiMaxDevForIsoline      3
    kOdGiMaxDevForFacet        4
    </table>
  */
  virtual double getDeviation(const OdGiDeviationType deviationType,
      const OdGePoint3d& pointOnCurve, bool bRecalculate = false) const;
  
  /** \details
    Starts calculation of initial deviation values for all deviation types.
  */
  void initDeviation();

  /** \details
    Returns the number of cached drawables for this view object.
  */
  int cachedDrawables() const { return m_nCachedDrawables!=0; }

  // Local viewport ID section
  /** \details
    Returns local viewport ID from the passed model.
    
    \param pModel     [in] Pointer to GS model.
  */
  OdUInt32 localViewportId(const OdGsBaseModel *pModel) const
  {
    return m_localId.localViewportId(pModel);
  }
  virtual bool isLocalViewportIdCompatible(const OdGsViewImpl *pView) const;
  OdGsViewLocalId &getViewLocalId()
  {
    return m_localId;
  }

  /** \details
    Checks whether view is regenerated.
    \returns
    true if view is regenerated, false otherwise.
  */
  virtual bool isViewRegenerated() const;

  const OdGePoint2d& center() const;
  void setCenter(const OdGePoint2d& center);

  /** \details
    Returns focus point of the camera.
  */
  OdGePoint3d focusPoint() const;
protected:
  void getUnrotatedFieldSize(double& fieldWidth, double& fieldHeight) const;
  void getClippingPlanes(double& nearClipPlaneDist, double& farClipPlaneDist
    , OdGsOverlayId overlayId, OdUInt32 projectionFlags) const;
  OdGeMatrix3d projectionMatrix(double nearClipPlaneDist, double farClipPlaneDist
    , OdUInt32 projectionFlags) const;
  OdGeMatrix3d eyeToScreenMatrix(double nearClipPlaneDist, double farClipPlaneDist
    , OdGsOverlayId overlayId) const;
  OdGeMatrix3d screenMatrix(double nearClipPlaneDist, double farClipPlaneDist) const;

  /** \details
    Returns the minimum and maximum z-values for this view object.
    \param zNear [out]  Receives the minimum z-value.
    \param zFar [out]  Receives the maximum z-value.
    \param nOverlay [in]  Overlay buffer index.
    \returns
    Returns true if and only if this view object has z-values associated with it.
  */
  virtual bool sceneDept(double& zNear, double& zFar, OdGsOverlayId nOverlay) const;
  // This is old version of sceneDept overrideable method, which is still provided for compatibility with
  // old vectorizers. Don't override it in new vectorizers, use new sceneDept method version even if
  // your vectorizer doesn't support overlay buffers. This method will be removed in one of next ODA Platform versions.
  virtual bool sceneDept(double& zNear, double& zFar) const;

  /** \details
    Returns the minimum and maximum depth of this view object.
    \param pMinDeptSupported [out]  Receives the minimum depth supported.
    \param pMaxDeptSupported [out]  Receives the maximum depth supported.
    \returns
    Returns true if and only if this view object supports depth.    

    \remarks
    The default implementation of this function does nothing but return false.
  */
  virtual bool usesDept(double* pMinDeptSupported = 0, double* pMaxDeptSupported = 0) const;
protected:
  /** \details
    Notification function called whenever the World to Eye transformation matrix has been changed.
  */
  void onWorldToEyeChanged();

  /** \details
    Notification function called whenever this ViewImpl object is about to be released.
  */
  void onFinalRelease();

  virtual void updateViewProps(bool bFullUpdate = true);

  inline bool isCheckValid() const;
  inline void setCheckValid(bool bValue) const;
  inline bool isInvalid() const;
  inline void setInvalid(bool bValue) const;
  inline void setPerspectiveEnabled(bool bValue) const;
  inline void setAccumulateVpChanges( bool bSet );
public:
  /** \details
    Checks whether there is non-rectangular clipping region.
    \returns
    true if array of points for non-rectangular clipping region is not empty, false otherwise.
  */
  inline bool isNonRectClipped() const;
  
  /** \details
    Checks whether this object has lineweights.
    \returns
    true if an array of lineweights is not empty, false otherwise.
  */
  inline bool hasLweights() const { return !m_lweights.empty(); }
  
  /** \details
    Retrieves an array of lineweights.
  */
  inline const OdUInt8Array &getLweightsEnum() const { return m_lweights; }

  /** \details
    Retrieves screen minimum point in device coordinates.
  */
  const OdGePoint2d& dcScreenMin() const { return m_dcScreenMin; }
  
  /** \details
    Retrieves screen minimum point in device coordinates.
  */
  const OdGePoint2d& dcScreenMax() const { return m_dcScreenMax; }
  
  /** \details
    Retrieves lower left point of this viewport in device coordinates.
  */
  const OdGePoint2d& dcLowerLeft() const { return m_dcLowerLeft; }
  
  /** \details
    Retrieves upper right point of this viewport in device coordinates.
  */
  const OdGePoint2d& dcUpperRight() const { return m_dcUpperRight; }

  /** \details
    Returns the perspective transformation matrix for this view object.
  */
  OdGeMatrix3d perspectiveMatrix() const;

  /** \details
    Utility function to recompute lens length into field of view angle.
    \param lensLength [in]  Lens length in millimeters.
    \returns
    Returns field of view angle in radians.
  */
  static double lensLengthToFOV(double lensLength);
  /** \details
    Utility function to recompute field of view angle into lens length.
    \param fovAngle [in]  Field of view angle in radians.
    \returns
    Returns lens length in millimeters.
    \remarks
    Input angle must be in range between 4 and 179 degrees inclusively.
  */
  static double lensLengthFromFOV(double fovAngle);

  /** \details
    Marks subViewportDraw geometry for regeneration.
    
    \param mask [in]  Mask.
  */
  void invalidateCachedViewportGeometry(OdUInt32 mask);

  /** \details
    Register the overlay for the specified model.

    \param pModel [in] Model which overlay should be registered.
  */
  virtual void registerOverlay(const OdGsModel *pModel);

  /** \details
    Unregister the overlay for the specified model.

    \param pModel [in] Model which overlay should be unregistered.
  */
  virtual void unregisterOverlay(const OdGsModel *pModel);

  /** \details
    Returns partial update rectange extension in pixels.
    \remarks
    This extension required for devices which support visual styles to avoid artifacts
    caused by silhouettes or line extensions.
  */
  virtual int partialUpdateExtentsEnlargement() const;
protected:
  /** \details
    Returns the OdGiDrawable associated with the specified DrawableHolder object.
    \param drawableHolder [in]  Drawable holder.
  */
  OdGiDrawablePtr drawableAt(DrawableHolder& drawableHolder);
  OdGsNode* getRootNode(DrawableHolder& holder);
  virtual void propagateInvalidVpFlag();
  virtual void propagateLayersChanges();

public:
  /** \details
    Calculates and returns recommended maximum deviation of the
    current vectorization for the specified deviation type.
    
    \param deviationType    [in] Deviation type.
    \param pointOnCurve     [in] Point on curve. Used only if 'bRecalculate' is true.
    \remarks
    deviationType must be one of the following:
    
    <table>
    Name                       Value
    kOdGiMaxDevForCircle       0      
    kOdGiMaxDevForCurve        1      
    kOdGiMaxDevForBoundary     2      
    kOdGiMaxDevForIsoline      3
    kOdGiMaxDevForFacet        4
    </table>
  */
  double calcDeviation(const OdGiDeviationType type, const OdGePoint3d& pt) const;

  // Viewport properties (backgrounds, render settings, visual styles) management
  
  /** \details
    Retrieves viewport properties.
  */
  OdGsPropertiesPtr getViewportPropertiesRoot() const;
  
  /** \details
    Updates viewport properties specified by the passed flags.
    
    \param incFlags    [in] Flags that specify which properties to update.
    \returns
    true if updated properties are not NULL, false otherwise.
  */
  bool updateViewportProperties(OdUInt32 incFlags);
  
  /** \details
    Retrieves viewport properties by the specified property type.
    
    \param type    [in] Property type to get.
  */
  OdGsPropertiesPtr getViewportPropertiesForType(OdGsProperties::PropertiesType type) const;

  // Printing support methods

  /** \details
    Retrieves viewport rotation matrix depending on viewport rotation.
    This method returns matrix for viewport rotation by 0, 90, 180 and 270 degrees.
  */
  const OdGeMatrix3d& rotationMatrix() const;
  
  /** \details
    Checks how viewport is rotated.
    \returns
    a value that represents degrees of viewport rotation. This method returns
    one of the following values: 0, 90, 180, 270.
  */
  short viewportRotation() const;
  
  /** \details
    Checks whether viewport is rotated.
    \returns
    true if viewport rotation is 90 or 270 degrees, false otherwise.
  */
  inline bool isRotated() const;

  /** \details
   Returns field height of unrotated viewport considering window aspect ration.
   
   \remarks
   When viewport is not rotated (or rotated 180)
   <code>
   +-----------------* m_dcScreenMax
   |                 |
   |                 |
   |                 | fieldHeight
   |                 | unrotatedFieldHeight
   |                 |
   |m_dcScreenMin    |
   *-----------------+
        fieldWidth
    unrotatedFieldWidth
   </code>

   When viewport is rotated to 90 or 270:
   <code>
   m_dcScreenMin
   *-----------------+
   |                 |
   |                 |
   |                 | fieldHeight
   |                 | unrotatedFieldWidth
   |                 |
   |                 |
   +-----------------* m_dcScreenMax
        fieldWidth
    unrotatedFieldHeight
    </code>
  */
  double unrotatedFieldHeight() const;
  
  /** \details
   Returns field width of unrotated viewport considering window aspect ration.
   
   \remarks
   When viewport is not rotated (or rotated 180)
   <code>
   +-----------------* m_dcScreenMax
   |                 |
   |                 |
   |                 | fieldHeight
   |                 | unrotatedFieldHeight
   |                 |
   |m_dcScreenMin    |
   *-----------------+
        fieldWidth
    unrotatedFieldWidth
   </code>

   When viewport is rotated to 90 or 270:
   <code>
   m_dcScreenMin
   *-----------------+
   |                 |
   |                 |
   |                 | fieldHeight
   |                 | unrotatedFieldWidth
   |                 |
   |                 |
   +-----------------* m_dcScreenMax
        fieldWidth
    unrotatedFieldHeight
    </code>
  */
  double unrotatedFieldWidth() const;

  /** \details
    Returns actual field width variable (without considering aspect ratio).
  */
  double actualFieldWidth() const;
  /** \details
    Returns actual field height variable (without considering aspect ratio).
  */
  double actualFieldHeight() const;

  // Primitives culling routines

  /** \details
    Initialize culling volume, associated with current view.

    \remarks
    Default implementation initializes culling volume for overall view, and transforms overall view culling volume
    for all viewports inside overall view space (only if viewports has orthogonal projection). 
    This method can be overrided into user device for adjust described behaviour.
    If this method overrided as empty function (not create culling volume inside) - all culling functionality
    will be disabled.

    \sa
    *  isCullingVolumeInitialized
    *  cullingVolumeIntersectWithOpt
    *  cullingVolumeIntersectWith
  */
  virtual void initCullingVolume();
  /** \details
    Sets culling volume associated with current view.
    \param newVol [in]  Smart pointer to new culling volume object.
    \sa
    *  cullingVolume
    *  initCullingVolume
  */
  void setCullingVolume(OdGsCullingVolumePtr newVol);
  /** \details
    Returns culling volume associated with current view.
    \sa
    *  initCullingVolume
    *  setCullingVolume
  */
  OdGsCullingVolumePtr cullingVolume() const;
  /** \details
    Checks whether culling volume associated with current view is initialized.
    \returns
    true if culling volume associated with current view is initialized, false otherwise.
    \remarks
    Culling volume consider associated with current view if pointer to culling volume is not null.
    \sa
    *  initCullingVolume
    *  setCullingVolume
    *  cullingVolume
  */
  virtual bool isCullingVolumeInitialized() const;
  /** \details
    Optimized intersection test between culling volume associated with current view and culling primitive.
    \returns
    Returns true if culling volume is not initialized.
    \param prim [in]  Input primitive for culling detection.
    \sa
    *  isCullingVolumeInitialized
    *  cullingVolumeIntersectWith
  */
  virtual bool cullingVolumeIntersectWithOpt(const OdGsCullingPrimitive &prim) const;
  /** \details
    Complete intersection test between culling volume associated with current view and culling primitive.

    \returns
    Returns kIntersectIn intersection status if culling volume is not initialized.
    \remarks
    Return value intersection statuses:
    <table>
    Name               Value    Description
    kIntersectNot      0        No intersection detected.
    kIntersectOk       1        Intersects with culling volume boundary.
    kIntersectIn       2        Culling volume primitive is completely inside culling volume boundary.
    </table>

    \param prim [in]  Input primitive for culling detection.

    \sa
    *  isCullingVolumeInitialized
    *  cullingVolumeIntersectWithOpt
  */
  virtual OdGsCullingVolume::IntersectionStatus cullingVolumeIntersectWith(const OdGsCullingPrimitive &prim) const;
  /** \details
    Applies transformation for culling volume associated with current view.
    \param xfm [in]  Transformation matrix.
    \sa
    *  initCullingVolume
  */
  virtual void cullingVolumeTransformBy(const OdGeMatrix3d &xfm);

  /** \details
    Defines the 3d clipping for this Viewport object.

    \param pBoundary [in]  3d clipping boundary.
    \param pClipInfo [in]  Optional 3d clipping boundary extended data.
  */
  virtual void setViewport3dClipping(const OdGiClipBoundary* pBoundary, const OdGiAbstractClipBoundary* pClipInfo = NULL);
  /** \details
    Returns the current 3d clipping for this Viewport object.

    \param ppClipInfo [out]  Optional 3d clipping boundary extended data.
  */
  virtual const OdGiClipBoundary *viewport3dClipping(const OdGiAbstractClipBoundary** ppClipInfo = NULL) const;
  /** \details
    Returns the current 3d clipping for this Viewport object as single object.
  */
  OdGiClipBoundaryWithAbstractData *viewport3dClippingObject();
  /** \details
    Returns true if and only if 3d clipping set for this Viewport object.
  */
  bool hasViewport3dClipping() const;

  /** \details
    Stores current view state into filer object.

    \param pFiler [out]  Filer object.
    \returns
    true if view state is successfully saved, false if metafile or client view could not be saved.
  */
  virtual bool saveViewState(OdGsFiler *pFiler) const;
  
  /** \details
    Restores view state from filer object.

    \param pFiler [in]  Filer object.
    \returns
    true if view state is successfully loaded, false if metafile or client view could not be read.
  */
  virtual bool loadViewState(OdGsFiler *pFiler);
  
  /** \details
    Stores client view state into filer object. Must be implemented in client views.

    \param pFiler [out]  Filer object.
    \returns
    true.
  */
  virtual bool saveClientViewState(OdGsFiler *pFiler) const;
  
  /** \details
    Restores client view state from filer object. Must be implemented in client views.

    \param pFiler [in]  Filer object.
    \returns
    true.
  */
  virtual bool loadClientViewState(OdGsFiler *pFiler);

  /** \details
    Returns array of calculated during propagateLayersChanges() call frozen layer differences.
  */
  virtual const OdDbStubPtrArray& getFrozenLayersDiff() const;

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

    \param nLevel [in]  Anti-aliasing mode (set as 3 by default). Set as 0 to disable anti-aliasing.
  */
  virtual void enableAntiAliasing(OdUInt32 nMode);

  /** \details
    Returns anti-aliasing mode for current Viewport object.
  */
  virtual OdUInt32 antiAliasingMode() const;

  /** \details
    Adds a clipping shape to the array of clipping shapes.

    \param pClippingShape [in]  Pointer to the clipping shape to add.
  */
  void addClippingShape(const OdSiShape*);
  
  /** \details
    Removes a clipping shape from the array of clipping shapes.

    \param pClippingShape [in]  Pointer to the clipping shape to remove.
  */
  void removeClippingShape(const OdSiShape*);
  
  /** \details
    Retrieves the array of pointers to clipping shapes.
  */
  const OdSiShapeConstPtrArray& clippingShapes() const;
  
  /** \details
    Sets the array of pointers to clipping shapes. All previously set elements in the array are overrided.
    
    \param s [in]  Array of pointers to clipping shapes.
  */
  void setClippingShapes(const OdSiShapeConstPtrArray& s);
private:
  void setExtents(OdGsOverlayId nOverlay, const OdGeExtents3d& ext, bool bAdd, bool bVisible = true);
  void getExtents(OdGsOverlayId nOverlay, OdGeExtents3d& ext, bool bVisible = false) const;

  void addInvalidRectThisView(OdGsOverlayId nOverlay, const OdGsDCRect& rc, const OdGsDCRect& vpRc);

private:
  enum EGsViewImplFlags
  {
    kFirstViewImplFlag       = 1,

    kInvalid                 = kFirstViewImplFlag << 0, // device surface entirely invalid
    kCheckValid              = kFirstViewImplFlag << 1, // device surface entirely valid
    kBorderVisible           = kFirstViewImplFlag << 2,
    kDoFrontClip             = kFirstViewImplFlag << 3,
    kDoBackClip              = kFirstViewImplFlag << 4,
    kPerspectiveEnabled      = kFirstViewImplFlag << 5,
    kHidden                  = kFirstViewImplFlag << 6, // view is hidden
    kTransparentClearColor   = kFirstViewImplFlag << 7,
    kDevBgClearColor         = kFirstViewImplFlag << 8,
    kEyeToWorldValid         = kFirstViewImplFlag << 9,
    kWorldToEyeValid         = kFirstViewImplFlag << 10,
    kSnapping                = kFirstViewImplFlag << 11,
    kForbidDrawableRegenDraw = kFirstViewImplFlag << 12,
    kDepViewInside           = kFirstViewImplFlag << 13,
    kLegacyWireframe         = kFirstViewImplFlag << 14,
    // there are two modes with hidden lines removed:
    // - normal, set by SHADEMODE command in CAD for display and
    //   represented by 'Hidden' visual style in shade plot viewport/layout props for printing, and
    // - legacy, set by HIDE command in CAD for display and
    //   according to 'OdDbViewport/OdDbPlotSettings::kHidden' shade plot for printing;
    // they both use 'kHiddenLine' render mode, but have some differences, e.g. in plot style usage;
    // this flag is to distinguish these two cases
    kLegacyHidden            = kFirstViewImplFlag << 15,
    kPlotTransparency        = kFirstViewImplFlag << 16,
    kAccumulateVpChanges     = kFirstViewImplFlag << 17,
    kDefaultLightingEnabled  = kFirstViewImplFlag << 18
  };
protected:
  OdDbStubPtrArray m_lastFrozenLayers;
private:
  OdGsBaseModulePtr m_pModule;
  OdDbStubPtrArray m_frozenLayersDiff;

  mutable OdUInt32 m_gsViewImplFlags;
  ODCOLORREF m_borderColor;
  int m_borderWeight;
  double m_frontClipDist;
  double m_backClipDist;
  bool m_screenInvertedX;
  bool m_screenInvertedY;
  OdGePoint2d m_dcScreenMin;
  OdGePoint2d m_dcScreenMax;
  OdGePoint2d m_dcLowerLeft;
  OdGePoint2d m_dcUpperRight;

  double m_fieldWidth;
  double m_fieldHeight;
  double m_lensLength;
  double m_eyeVecLength;

  OdGePoint3d m_position;
  OdGePoint3d m_target;
  OdGeVector3d m_upVector;
  OdGeVector3d m_eyeVector;
  OdGeVector3d m_xVector;
  OdGePoint2d m_center;
  mutable OdGeMatrix3d m_eyeToWorld;
  mutable OdGeMatrix3d m_worldToEye;

  OdIntArray m_nrcCounts;
  OdGePoint2dArray m_nrcPoints;
  double m_linetypeScaleMultiplier;
  double m_linetypeAlternateScaleMultiplier;
  double m_lineweightToDcScale;
  OdUInt8Array m_lweights;
  OdGsCullingVolumePtr m_pCullingVolume;
  OdSharedPtr<OdGiClipBoundaryWithAbstractData> m_p3dClip;
  OdSiShapeConstPtrArray m_clippingShapes;

  double m_deviation[5];

  int m_nCachedDrawables;
  mutable OdMutexPtr m_mtExtents;

  OdGsPropertiesPtr m_pProperties;
  OdDbStub *m_pBackgroundId;
  OdDbStub *m_pVisualStyleId;
  OdGiDrawablePtr m_pViewportTrans;
  DefaultLightingType m_defaultLightingType;
  OdUInt32 m_antiAliasingMode;

protected:
  struct GsViewOverlayData
  {
    enum OverlayFlags
    {
      kWorldToDeviceValid = (1 << 0),
      kSceneDeptInvalid   = (1 << 1),
      kViewToScreenValid  = (1 << 2)
    };
    mutable OdUInt32 m_nOverlayFlags;
    mutable OdGeMatrix3d m_worldToDeviceMatrix;
    mutable OdGeMatrix3d m_viewToScreenMat;
    mutable double m_nearClipPlaneDist1;
    mutable double m_farClipPlaneDist1;
    mutable double m_nearClipPlaneDist2;
    mutable double m_farClipPlaneDist2;
    OdGeExtents3d m_extents, m_viewExtents;
    OdGsDCRectArray m_invalidRects;
    GsViewOverlayData()
      : m_nOverlayFlags(0)
      , m_nearClipPlaneDist1(0.0)
      , m_farClipPlaneDist1(0.0)
      , m_nearClipPlaneDist2(0.0)
      , m_farClipPlaneDist2(0.0)
    {}
  };
  class GS_TOOLKIT_EXPORT GsViewOverlayDataContainer : public OdGsOverlayDataContainer<GsViewOverlayData>
  {
    public:
      void setWorldToDeviceInvalid();
      void setSceneDeptInvalid(OdGsOverlayId nOverlay, bool bSet) const;
      void clearInvalidRects(OdUInt32 nOverlays = kGsAllOverlays);
      bool hasInvalidRects(OdUInt32 nOverlays = kGsAllOverlays) const;
  };
  mutable GsViewOverlayDataContainer m_overlayData;
protected:
  OdGsBaseVectorizeDevice* m_pDevice;
  OdGsClientViewInfo m_viewInfo;
  OdUInt32 m_giViewportId;
  OdGsViewLocalId m_localId;
  OdGiRegenType m_regenerationType;
  // Reserved pointer for internal use (binary incompatible changes), do not delete.
  OdRxObjectPtr m_pGsReserved;

  // Made protected for access in OdGiViewport methods
  RenderMode m_renderMode;

  DrawableHolderArray m_drawables;

  OdDbStubPtrArray m_frozenLayers;

  bool m_bInInteractivityMode;
  double m_dInteractivityFrameRate;

  friend class GsViewImplHelper;
  friend class OdGiBaseVectorizerImpl;
  friend class OdGsBaseVectorizer;
  friend class OdGsBaseModule;
  friend class OdGsBaseVectorizeDevice;
};

inline bool OdGsViewImpl::isDependentViewportView() const
{
  return GETBIT(m_viewInfo.viewportFlags, OdGsClientViewInfo::kDependentViewport);
}

inline bool OdGsViewImpl::isDependentGeometryView() const
{
  return GETBIT(m_viewInfo.viewportFlags, OdGsClientViewInfo::kDependentGeometry);
}

inline bool OdGsViewImpl::isHelperView() const
{
  return GETBIT(m_viewInfo.viewportFlags, OdGsClientViewInfo::kHelperView);
}

inline const OdGsBaseModule *OdGsViewImpl::getGsModulePtr() const
{
  return m_pModule.get();
}

inline const OdGiContextualColors *OdGsViewImpl::contextualColors() const
{
  return m_viewInfo.contextColors.get();
}

inline void OdGsViewImpl::setContextualColors(OdGiContextualColors *pColors)
{
  m_viewInfo.contextColors = pColors;
  invalidate();
}

inline bool OdGsViewImpl::isSupportPlotStyles() const
{
  return (mode() == k2DOptimized) || isLegacyHiddenMode() || isLegacyWireframeMode();
}

inline bool OdGsViewImpl::isCheckValid() const
{
  return GETBIT(m_gsViewImplFlags, kCheckValid);
}

inline void OdGsViewImpl::setCheckValid(bool bValue) const
{
  SETBIT(m_gsViewImplFlags, kCheckValid, bValue);
}

inline bool OdGsViewImpl::isInvalid() const
{
  return GETBIT(m_gsViewImplFlags, kInvalid);
}

inline void OdGsViewImpl::setInvalid(bool bValue) const
{
  SETBIT(m_gsViewImplFlags, kInvalid, bValue);
}

inline void OdGsViewImpl::setPerspectiveEnabled(bool bValue) const
{
  SETBIT(m_gsViewImplFlags, kPerspectiveEnabled, bValue);
  if (bValue != isPerspective())
    m_overlayData.setWorldToDeviceInvalid();
}

inline bool OdGsViewImpl::defaultLightingEnabled() const
{
  return GETBIT(m_gsViewImplFlags, kDefaultLightingEnabled);
}

inline OdGsView::DefaultLightingType OdGsViewImpl::defaultLightingType() const
{
  return m_defaultLightingType;
}

inline void OdGsViewImpl::enableAntiAliasing(OdUInt32 nMode)
{
  if (antiAliasingMode() != nMode)
  {
    m_antiAliasingMode = nMode;
    invalidate();
  }
}

inline OdUInt32 OdGsViewImpl::antiAliasingMode() const
{
  return m_antiAliasingMode;
}

inline bool OdGsViewImpl::isNonRectClipped() const
{
  return m_nrcCounts.size() != 0;
}

inline bool OdGsViewImpl::isRotated() const
{
  short rotation = viewportRotation();
  return rotation == 90 || rotation == 270;
}

inline void OdGsViewImpl::invalidate(const OdGsUpdateExtents& extents, const OdGsBaseModel *pModel)
{
  invalidate(extents.getExtents(), pModel, extents.getLineweight());
}

inline const OdGeMatrix3d& OdGsViewImpl::eyeToWorldMatrix() const
{
  if(!GETBIT(m_gsViewImplFlags, kEyeToWorldValid))
  {
    m_eyeToWorld.setCoordSystem(target(), xVector(), upVector(), eyeVector());
    SETBIT(m_gsViewImplFlags, kEyeToWorldValid, true);
  }
  return m_eyeToWorld;
}

inline const OdGeMatrix3d& OdGsViewImpl::worldToEyeMatrix() const
{
  if(!GETBIT(m_gsViewImplFlags, kWorldToEyeValid))
  {
    m_worldToEye = eyeToWorldMatrix().inverse();
    SETBIT(m_gsViewImplFlags, kWorldToEyeValid, true);
  }
  return m_worldToEye;
}

inline OdGsViewImpl* OdGsViewImpl::safeCast(OdGsView* pView)
{
  return OdSmartPtr<OdGsViewImpl>(pView).get();
}

inline const OdGsViewImpl* OdGsViewImpl::safeCast(const OdGsView* pView)
{
  return OdSmartPtr<OdGsViewImpl>(pView).get();
}

inline bool OdGsViewImpl::isSnapping() const
{
  return GETBIT(m_gsViewImplFlags, kSnapping);
}

inline void OdGsViewImpl::setSnapping(bool val)
{
  SETBIT(m_gsViewImplFlags, kSnapping, val);
}

inline bool OdGsViewImpl::isRegenOnDrawForbidden() const
{
  return GETBIT(m_gsViewImplFlags, kForbidDrawableRegenDraw);
}

inline void OdGsViewImpl::setRegenOnDrawForbidden(bool val)
{
  SETBIT(m_gsViewImplFlags, kForbidDrawableRegenDraw, val);
}

inline bool OdGsViewImpl::isAccumulateVpChanges() const
{
  return GETBIT(m_gsViewImplFlags, kAccumulateVpChanges);
}

inline void OdGsViewImpl::setAccumulateVpChanges( bool bSet )
{
  SETBIT( m_gsViewImplFlags, kAccumulateVpChanges, bSet );
}

inline bool OdGsViewImpl::isCompletelyVisible() const
{
  return GETBIT(m_gsViewImplFlags, kDepViewInside);
}

inline void OdGsViewImpl::setCompletelyVisible(bool val)
{
  SETBIT(m_gsViewImplFlags, kDepViewInside, val);
}

inline OdGiClipBoundaryWithAbstractData *OdGsViewImpl::viewport3dClippingObject()
{
  return m_p3dClip.get();
}

inline bool OdGsViewImpl::hasViewport3dClipping() const
{
  return !m_p3dClip.isNull();
}

inline double OdGsViewImpl::actualFieldWidth() const
{
  return m_fieldWidth;
}

inline double OdGsViewImpl::actualFieldHeight() const
{
  return m_fieldHeight;
}

inline void OdGsViewImpl::addClippingShape(const OdSiShape* p)
{
  if (!m_clippingShapes.contains(p))
    m_clippingShapes.append(p);
}

inline void OdGsViewImpl::removeClippingShape(const OdSiShape* p)
{
  OdSiShapeConstPtrArray::size_type i;
  if (m_clippingShapes.find(p, i, 0))
    m_clippingShapes.removeAt(i);
}

inline const OdSiShapeConstPtrArray& OdGsViewImpl::clippingShapes() const
{
  return m_clippingShapes;
}

inline void OdGsViewImpl::setClippingShapes(const OdSiShapeConstPtrArray& s)
{
  m_clippingShapes = s;
}

GS_TOOLKIT_EXPORT double calcFocalLength(double lensLength, double fieldWidth, double fieldHeight);

#include "TD_PackPop.h"

#endif // ODGSVIEWIMPL_INC
