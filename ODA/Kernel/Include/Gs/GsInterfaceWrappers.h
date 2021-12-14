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

#ifndef __GSINTERFACEWRAPPERS_H_INCLUDED_
#define __GSINTERFACEWRAPPERS_H_INCLUDED_

#include "Gs/Gs.h"
#include "Ge/GePoint3d.h"
#include "Ge/GeVector3d.h"
#include "Ge/GeMatrix3d.h"
#include "Gs/GsModel.h"
#include "Gs/GsUpdateManager.h"

#include "UInt8Array.h"
#include "IntArray.h"
#include "Ge/GePoint2dArray.h"

#include "TD_PackPush.h"

/** \details
    Default wrapper base class for Gs interface classes.

    Corresponding C++ library: TD_Gs

    <group OdGs_Classes> 
*/
template <typename RedirectionBaseInterface, typename RedirectionInterface>
class OdGsDefaultRedirectionBase : public RedirectionInterface
{
  private:
    RedirectionBaseInterface *m_pRedirectionObj;
  public:
    /** \details
      Default constructor for the OdGsDefaultRedirectionBase class. Sets the redirection object.
      
      \param pRedirectionObj  [in]  Pointer to a redirection object.
    */
    OdGsDefaultRedirectionBase(RedirectionBaseInterface *pRedirectionObj = NULL)
      : m_pRedirectionObj(pRedirectionObj)
    {}

    /** \details
      Checks whether this object has redirection object.
      \returns
      true if there is a redirection object, false otherwise.
    */
    bool hasRedirection() const { return m_pRedirectionObj != NULL; }
    
    /** \details
      Retrieves a pointer to a redirection object.
    */
    const RedirectionBaseInterface *redirection() const { return m_pRedirectionObj; }
    
    /** \details
      Retrieves a pointer to a redirection object.
    */
    RedirectionBaseInterface *redirection() { return m_pRedirectionObj; }
    
    /** \details
      Sets the redirection object.
      
      \param pRedirectionObj  [in]  Pointer to a redirection object.
    */
    void setRedirection(RedirectionBaseInterface *pRedirectionObj) { m_pRedirectionObj = pRedirectionObj; }
    
    /** \details
      Resets the redirection object to NULL.
    */
    void resetRedirection() { m_pRedirectionObj = NULL; }
};

/** \details
    Abstract wrapper base class for Gs interface classes.

    Corresponding C++ library: TD_Gs

    <group OdGs_Classes> 
*/
template <typename RedirectionBaseInterface, typename RedirectionInterface>
class OdGsAbstractRedirectionBase : public RedirectionInterface
{
  public:
    /** \details
      Constructor for the OdGsAbstractRedirectionBase.
    */
    OdGsAbstractRedirectionBase(RedirectionBaseInterface * /*pRedirectionObj*/ = NULL)
    {}

    /** \details
      Checks whether this object has redirection object.
      \returns
      true if there is a redirection object, false otherwise.
    */
    virtual bool hasRedirection() const = 0;
    
    /** \details
      Retrieves a pointer to a redirection object.
    */
    virtual const RedirectionBaseInterface *redirection() const = 0;
    
    /** \details
      Retrieves a pointer to a redirection object.
    */
    virtual RedirectionBaseInterface *redirection() = 0;
    
    /** \details
      Sets the redirection object.
      
      \param pRedirectionObj  [in]  Pointer to a redirection object.
    */
    virtual void setRedirection(RedirectionBaseInterface * /*pRedirectionObj*/) {}
    
    /** \details
      Resets the redirection object to NULL.
    */
    virtual void resetRedirection() {}
};

/** \details
    Base template for OdGsView class wrappers.

    Corresponding C++ library: TD_Gs

    <group OdGs_Classes> 
    \sa
    <link tv_working_with_base_classes_view, Work with Visualize Graphical Views>
    \sa
    <link gs_OdGsView, Work with Views>
*/
template <typename GsViewInterface = OdGsView, typename RedirectionBase = OdGsDefaultRedirectionBase<OdGsView, GsViewInterface> >
class OdGsViewWrapper : public RedirectionBase
{
  private:
    bool trHas() const { return this->hasRedirection(); }
    const OdGsView *trCall() const { return this->redirection(); }
    OdGsView *trCall() { return this->redirection(); }
  public:
    /** \details
      Constructor for the OdGsViewWrapper class. Sets the redirection view.
      
      \param pRedirectionView  [in]  Redirection view.
    */
    OdGsViewWrapper(OdGsView *pRedirectionView = NULL)
      : RedirectionBase(pRedirectionView)
    {}

    // OdGsView wrappers

    /** \details
      Retrieves a pointer to a device that owns the wrapped view.
      This method returns NULL if there is no redirection view.
    */
    OdGsDevice* device() const
    {
      if (trHas())
        return trCall()->device();
      return NULL;
    }

    /** \details
      Retrieves a pointer to the vectorization context.
      This method returns NULL if there is no redirection view.
    */
    OdGiContext* userGiContext() const
    {
      if (trHas())
        return trCall()->userGiContext();
      return NULL;
    }
    /** \details
      Sets current vectorization context.
      
      \param pUserGiContext  [in]  Pointer to vectorization context.
    */
    void setUserGiContext(OdGiContext* pUserGiContext)
    {
      if (trHas())
        trCall()->setUserGiContext(pUserGiContext);
    }

    /** \details
      Returns the scale factor for displaying Lineweights from the wrapped Viewport object.
      If there is no wrapped view, this method returns 1.0.
      \sa
      <link tv_working_with_attributes_lineweights, Work with Lineweights>
    */
    double lineweightToDcScale() const
    {
      if (trHas())
        return trCall()->lineweightToDcScale();
      return 1.0;
    }
    
    /** \details
      Sets the scale factor for displaying Lineweights for the wrapped Viewport object.
      \param scale [in]  Scale factor.
      \sa
      <link tv_working_with_attributes_lineweights, Work with Lineweights>
    */
    void setLineweightToDcScale(double scale)
    {
      if (trHas())
        trCall()->setLineweightToDcScale(scale);
    }
    
    /** \details
      Sets the lineweights for the wrapped Viewport object.
      \param numLineweights [in]  Number of Lineweights.
      \param lineweights    [in]  Array of Lineweights.
      \sa
      <link tv_working_with_attributes_lineweights, Work with Lineweights>
    */
    void setLineweightEnum(int numLineweights, const OdUInt8* lineweights)
    {
      if (trHas())
        trCall()->setLineweightEnum(numLineweights, lineweights);
    }

    /** \details
      Sets the size and position for the wrapped Viewport object.
      
      \param lowerLeft [in]  Lower-left corner in nomalized device coordinates [0.0 .. 1.0].
      \param upperRight [in]  Upper-right corner in nomalized device coordinates [0.0 .. 1.0].   
      \remarks
      (0,0) is the lower-left corner of the owning Device object. Coordinates increase upward and to the right.
      The View object can be partly or completely off screen.
    */
    void setViewport(const OdGePoint2d& lowerLeft, const OdGePoint2d& upperRight)
    {
      if (trHas())
        trCall()->setViewport(lowerLeft, upperRight);
    }
    
    /** \details
      Sets the size and position of the wrapped Viewport object.
      
      \param screenRect [in]  Screen rectangle in device coordinates (pixels).    
      \remarks
      (0,0) is the lower-left corner of the owning Device object. Coordinates increase upward and to the right. 
      \remarks
      The View object can be partly or completely off screen.    
    */
    void setViewport(const OdGsDCRect& screenRect)
    {
      if (trHas())
        trCall()->setViewport(screenRect);
    }
    
    /** \details
      Sets the size and position of the wrapped viewport object.
      
      \param screenRect [in]  Screen rectangle in device coordinates (pixels).    
      \remarks
      (0,0) is the lower-left corner of the owning Device object. Coordinates increase upward and to the right.
      The View object can be partly or completely off screen.    
    */
    void setViewport(const OdGsDCRectDouble& screenRect)
    {
      if (trHas())
        trCall()->setViewport(screenRect);
    }
    
    /** \details
      Returns the size and position of the wrapped viewport object.
      \param lowerLeft [out]  Receives the lower-left corner in nomalized device coordinates [0.0 .. 1.0].
      \param upperRight [out]  Receives the upper-right corner in nomalized device coordinates [0.0 .. 1.0].
      \param screenRect [out]  Receives the screen rectangle in device coordinates (pixels).
      
      \remarks
      (0,0) is the lower-left corner of the owning Device object.  Coordinates increase upward and to the right.
      
      \remarks
      The View object may be partly or completely off screen.    
    */
    void getViewport(OdGePoint2d& lowerLeft, OdGePoint2d& upperRight) const
    {
      if (trHas())
        trCall()->getViewport(lowerLeft, upperRight);
    }
    
    /** \details
      Returns the size and position of the wrapped viewport object.
      \param screenRect [out]  Receives the screen rectangle in device coordinates (pixels).
      
      \remarks
      (0,0) is the lower-left corner of the owning Device object.  Coordinates increase upward and to the right.
      
      \remarks
      The View object may be partly or completely off screen.    
    */
    void getViewport(OdGsDCRect& screenRect) const
    {
      if (trHas())
        trCall()->getViewport(screenRect);
    }
    
    /** \details
      Returns the size and position of the wrapped viewport object.
      \param screenRect [out]  Receives the screen rectangle in device coordinates (pixels).
      \remarks
      (0,0) is the lower-left corner of the owning Device object.  Coordinates increase upward and to the right.
      The View object may be partly or completely off screen.    
    */
    void getViewport(OdGsDCRectDouble& screenRect) const
    {
      if (trHas())
        trCall()->getViewport(screenRect);
    }

    /** \details
      Defines a polygonal clip region for the wrapped viewport object.
      
      \param numCoutours [in]  Number of polygonal contours.
      \param numVertices [in]  Array of the number of vertices in each polygonal contour.
      \param vertices [in]  Array of device coordinate (pixel) vertices defining the polygonal contours.
    */
    void setViewportClipRegion(int numContours, int const* numVertices, OdGsDCPoint const* vertices)
    {
      if (trHas())
        trCall()->setViewportClipRegion(numContours, numVertices, vertices);
    }
    
    /** \details
      Defines a polygonal clip region for the wrapped viewport object.
      
      \param numCoutours [in]  Number of polygonal contours.
      \param numVertices [in]  Array of the number of vertices in each polygonal contour.
      \param vertices [in]  Array of device coordinate (pixel) vertices defining the polygonal contours.
    */
    void setViewportClipRegion(int numContours, int const* numVertices, OdGePoint2d const* vertices)
    {
      if (trHas())
        trCall()->setViewportClipRegion(numContours, numVertices, vertices);
    }
    
    /** \details
      Returns a polygonal clip region for the wrapped viewport object.
      
      \param counts [out]  Array of the number of vertices in each polygonal contour.
      \param vertices [out]  Array of device coordinate (pixel) vertices defining the polygonal contours.
    */
    void viewportClipRegion(OdIntArray& counts, OdGsDCPointArray& vertices) const
    {
      if (trHas())
        trCall()->viewportClipRegion(counts, vertices);
    }
    
    /** \details
      Returns a polygonal clip region for the wrapped viewport object.
      
      \param counts [out]  Array of the number of vertices in each polygonal contour.
      \param vertices [out]  Array of device coordinate (pixel) vertices defining the polygonal contours.
    */
    void viewportClipRegion(OdIntArray& counts, OdGePoint2dArray& vertices) const
    {
      if (trHas())
        trCall()->viewportClipRegion(counts, vertices);
    }

    /** \details
      Defines the 3d clipping for the wrapped viewport object.

      \param pBoundary [in]  3d clipping boundary.
      \param pClipInfo [in]  Optional 3d clipping boundary extended data.
    */
    void setViewport3dClipping(const OdGiClipBoundary* pBoundary, const OdGiAbstractClipBoundary* pClipInfo = NULL)
    {
      if (trHas())
        trCall()->setViewport3dClipping(pBoundary, pClipInfo);
    }
    
    /** \details
      Returns the current 3d clipping for the wrapped viewport object.

      \param ppClipInfo [out]  Optional 3d clipping boundary extended data.

      \remarks
      Returns NULL if 3d clipping boundary isn't set or if there is no redirection view.
    */
    const OdGiClipBoundary *viewport3dClipping(const OdGiAbstractClipBoundary** ppClipInfo = NULL) const
    {
      if (trHas())
        return trCall()->viewport3dClipping(ppClipInfo);
      return NULL;
    }
    
    /** \details
      Sets the color and width of the border for the wrapped viewport object.
      \param color [in]  Border color.
      \param width [in]  Border width in pixels.
    */
    void setViewportBorderProperties(ODCOLORREF color, int width)
    {
      if (trHas())
        trCall()->setViewportBorderProperties(color, width);
    }
    
    /** \details
      Returns the color and width of the border for the wrapped viewport object.
      \param color [out]  Receives the border color.
      \param width [out]  Receives the border width in pixels.
    */
    void getViewportBorderProperties(ODCOLORREF& color, int& width) const
    {
      if (trHas())
        trCall()->getViewportBorderProperties(color, width);
    }
    
    /** \details
      Controls the border visibility for the wrapped viewport object.
      \param visible [in]  Controls visibility.
    */
    void setViewportBorderVisibility(bool visible)
    {
      if (trHas())
        trCall()->setViewportBorderVisibility(visible);
    }
    
    /** \details
      Checks whether viewport border is visibile, false otherwise.
      \returns 
      true the border is visible for the wrapped viewport object, false otherwise.
      This method returns false if there is no redirection view.
    */
    bool isViewportBorderVisible() const
    {
      if (trHas())
        return trCall()->isViewportBorderVisible();
      return false;
    }

    /** \details
      Sets the camera parameters for the wrapped viewport object.
      
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
    void setView(const OdGePoint3d& position, const OdGePoint3d& target, const OdGeVector3d& upVector,
                 double fieldWidth, double fieldHeight, OdGsView::Projection projectionType = OdGsView::kParallel)
    {
      if (trHas())
        trCall()->setView(position, target, upVector, fieldWidth, fieldHeight, projectionType);
    }
    
    /** \details
      Returns the WCS camera position for the wrapped viewport object.
      This method returns a 3D point at the origin of world coordinate system if there is no redirection view.
    */
    OdGePoint3d position() const
    {
      if (trHas())
        return trCall()->position();
      return OdGePoint3d::kOrigin;
    }
    
    /** \details
      Returns the WCS camera target for the wrapped viewport object.
      This method returns (0,0,1) vector if there is no redirection view.
    */
    OdGePoint3d target() const
    {
      if (trHas())
        return trCall()->target();
      return OdGePoint3d::kOrigin + OdGeVector3d::kZAxis;
    }
    
    /** \details
      Returns the WCS camera up vector for the wrapped viewport object.
      This method returns (0,1,0) vector if there is no redirection view.
    */
    OdGeVector3d upVector() const
    {
      if (trHas())
        return trCall()->upVector();
      return OdGeVector3d::kYAxis;
    }

    /** \details
      Returns the perspective lens length for the wrapped viewport object.
      This method returns 50.0 if there is no redirection view.
    */
    double lensLength() const
    {
      if (trHas())
        return trCall()->lensLength();
      return 50.0;
    }
    
    /** \details
      Sets the perspective lens length for the wrapped viewport object.
      \param lensLength [in]  Perspective lens length.
    */
    void setLensLength(double lensLength)
    {
      if (trHas())
        trCall()->setLensLength(lensLength);
    }
    
    /** \details
      Checks whether the projection type for the wrapped viewport object is kPerspective.
      \returns
      true if projection type for the wrapped viewport object is kPerspective, false otherwise.
      This method returns false if there is no redirection view.
    */
    bool isPerspective() const
    {
      if (trHas())
        return trCall()->isPerspective();
      return false;
    }

    /** \details
      Returns the WCS projection plane (field) width for the wrapped viewport object.
      This method returns 1.0 if there is no redirection view.
    */
    double fieldWidth() const
    {
      if (trHas())
        return trCall()->fieldWidth();
      return 1.0;
    }
    
    /** \details
      Returns the WCS projection plane (field) height for the wrapped viewport object.
      This method returns 1.0 if there is no redirection view.
    */
    double fieldHeight() const
    {
      if (trHas())
        return trCall()->fieldHeight();
      return 1.0;
    }

    /** \details
      Controls the front clipping of the wrapped viewport object.
      \param enable [in]  Enable front clipping.
    */
    void setEnableFrontClip(bool enable)
    {
      if (trHas())
        trCall()->setEnableFrontClip(enable);
    }
    
    /** \details
      Checks whether front clipping is enabled for the wrapped viewport object.
      \returns
      true if front clipping is enabled for the wrapped viewport object, false otherwise.
      This method returns false if there is no redirection view.
    */
    bool isFrontClipped() const
    {
      if (trHas())
        return trCall()->isFrontClipped();
      return false;
    }
    
    /** \details
      Sets the front clip distance from the target of the wrapped viewport object.
      \param frontClip [in]  Front clip distance.
    */
    void setFrontClip(double frontClip)
    {
      if (trHas())
        trCall()->setFrontClip(frontClip);
    }
    
    /** \details
      Returns the front clip distance from the target of the wrapped viewport object.
      This method returns 0.0 if there is no redirection view.
    */
    double frontClip() const
    {
      if (trHas())
        return trCall()->frontClip();
      return 0.0;
    }
    
    /** \details
      Controls the back clipping of the wrapped viewport object.
      \param enable [in]  Enable back clipping.
    */
    void setEnableBackClip(bool enable)
    {
      if (trHas())
        trCall()->setEnableBackClip(enable);
    }
    
    /** \details
      Checks whether back clipping is enabled for the wrapped viewport object.
      \returns
      true if back clipping is enabled for the wrapped viewport object, false otherwise.
      This method returns false if there is no redirection view.
    */
    bool isBackClipped() const
    {
      if (trHas())
        return trCall()->isBackClipped();
      return false;
    }
    
    /** \details
      Sets the back clip distance from the target of the wrapped viewport object.
      \param backClip [in]  Back clip distance.
    */
    void setBackClip(double backClip)
    {
      if (trHas())
        trCall()->setBackClip(backClip);
    }
    
    /** \details
      Returns the back clip distance from the target of the wrapped viewport object.
      This method returns 0.0 if there is no redirection view.
    */
    double backClip() const
    {
      if (trHas())
        return trCall()->backClip();
      return 0.0;
    }

    /** \details
      Returns the matrix that transforms world space to view space for the wrapped viewport object.
      This method returns identity matrix if there is no redirection view.
    */
    OdGeMatrix3d viewingMatrix() const
    {
      if (trHas())
        return trCall()->viewingMatrix();
      return OdGeMatrix3d::kIdentity;
    }
    
    /** \details
      Returns the matrix that transforms view space to normalized device space for the wrapped viewport object.
      This method returns identity matrix if there is no redirection view.
    */
    OdGeMatrix3d projectionMatrix() const
    {
      if (trHas())
        return trCall()->projectionMatrix();
      return OdGeMatrix3d::kIdentity;
    }
    
    /** \details
      Returns the matrix that transforms normalized device space to screen space for the wrapped viewport object.
      This method returns identity matrix if there is no redirection view.
    */
    OdGeMatrix3d screenMatrix() const
    {
      if (trHas())
        return trCall()->screenMatrix();
      return OdGeMatrix3d::kIdentity;
    }
    
    /** \details
      Returns a matrix that transforms coordinates from world space to screen space for the wrapped viewport object. 
      
      \remarks
      This is equivalent to the concatenation of the viewingMatrix, 
      projectionMatrix, and screenMatrix.
      This method returns identity matrix if there is no redirection view.
    */
    OdGeMatrix3d worldToDeviceMatrix() const
    {
      if (trHas())
        return trCall()->worldToDeviceMatrix();
      return OdGeMatrix3d::kIdentity;
    }
    
    /** \details
      Returns the matrix that transforms coordinates from model space to screen space for the wrapped viewport object.
      This method returns identity matrix if there is no redirection view.
    */
    OdGeMatrix3d objectToDeviceMatrix() const
    {
      if (trHas())
        return trCall()->objectToDeviceMatrix();
      return OdGeMatrix3d::kIdentity;
    }

    /** \details
      Sets the render mode for the wrapped viewport object.
      
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
    void setMode(OdGsView::RenderMode mode)
    {
      if (trHas())
        trCall()->setMode(mode);
    }
    
    /** \details
      Sets the render mode for the wrapped viewport object.
      
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
      This method OdGsView::kNone.
    */
    OdGsView::RenderMode mode() const
    {
      if (trHas())
        return trCall()->mode();
      return OdGsView::kNone;
    }

    /** \details
      Adds the specified scene graph to the wrapped viewport object.
      
      \param pSceneGraph [in]  Pointer to the root of the scene graph.
      \param pModel [in]  Pointer to the Model object to which all scene graph notifications are to be directed.
      \returns
      false.
      \remarks
      This function is intended to be used as follows:
      
      1.  Construct a scene graph of OdGiDrawable objects. 
          Each object draws its children with OdGiGeometry::draw().
      
      2.  With this function, add the root of the scene graph and the 
          OdGsModel that handles notifications to the wrapped viewport object.
    */
    bool add(OdGiDrawable* pSceneGraph, OdGsModel* pModel)
    {
      if (trHas())
        return trCall()->add(pSceneGraph, pModel);
      return false;
    }
    
    /** \details
      Retrieves a quantity of root drawables for the wrapped viewport.
      This method returns 0 if there is no redirection view.
    */
    int numRootDrawables() const {
      if (trHas())
        return trCall()->numRootDrawables();
      return 0;
    }
    
    /** \details
      Retrieves a persistent object identifier of the root drawable at the specified index.
      This method returns 0 if there is no redirection view.
      \param i [in]  Index of the drawable to return.
      \param pModelReturn [in]  Index of the drawable to return.
    */
    OdDbStub* rootDrawableIdAt(int i, OdGsModelPtr* pModelReturn /*= 0*/) const {
      if (trHas())
        return trCall()->rootDrawableIdAt(i, pModelReturn);
      return 0;
    }
    
    /** \details
      Retrieves a transient object identifier of the root drawable at the specified index.
      This method returns a not valid smart pointer to a drawable if there is no redirection view.
      \param i [in]  Index of the drawable to return.
      \param pModelReturn [in]  Index of the drawable to return.
    */
    OdGiDrawablePtr rootDrawableAt(int i, OdGsModelPtr* pModelReturn /*= 0*/) const {
      if (trHas())
        return trCall()->rootDrawableAt(i, pModelReturn);
      return OdGiDrawablePtr();
    }
    
    /** \details
      Removes the specified scene graph from the wrapped viewport object.
      
      \param sceneGraph [in]  Pointer to the root of the scene graph.
      \returns
      true if scene graph is successfully erased, false otherwise.
      This method returns false if there is no redirection view.
    */
    bool erase(OdGiDrawable* sceneGraph)
    {
      if (trHas())
        return trCall()->erase(sceneGraph);
      return false;
    }
    
    /** \details
      Removes all scene graphs from the wrapped viewport object.
      
      \param sceneGraph [in]  Root of scene graph.
    */
    void eraseAll()
    {
      if (trHas())
        trCall()->eraseAll();
    }

    /** \details
      Returns OdGsModel object associated with specified OdGiDrawable object in the wrapped viewport object.
      This method returns NULL if there is no redirection view.

      \param pDrawable [in]  Pointer onto OdGiDrawable object.
    */
    OdGsModel *getModel(const OdGiDrawable *pDrawable) const
    {
      if (trHas())
        return trCall()->getModel(pDrawable);
      return NULL;
    }
    
    /** \details
      Returns array of OdGsModel objects associated with the wrapped viewport object.
      This method returns an empty model array if there is no redirection view.
    */
    OdGsModelArray getModelList() const
    {
      if (trHas())
        return trCall()->getModelList();
      return OdGsModelArray();
    }

    /** \details
      Marks the entire area of the wrapped view for refreshing.
    */
    void invalidate()
    {
      if (trHas())
        trCall()->invalidate();
    }
    
    /** \details
      Marks for refresh the specified region of the wrapped viewport object.
       
      \param screenRect [in]  Screen rectangle in device coordinates (pixels).
   */
    void invalidate(const OdGsDCRect &screenRect)
    {
      if (trHas())
        trCall()->invalidate(screenRect);
    }
    
    /** \details
      Checks whether the wrapped viewport shows the correct image.
      \returns
      true if the wrapped viewport object is showing the correct image, false otherwise.
      This method returns false if there is no redirection view.

      \remarks
      This implies the follow:
      * No part of GUI area is flagged for refresh.
      * All rasterizations are complete
      * The rendering pipeline has been flushed.
    */
    bool isValid() const
    {
      if (trHas())
        return trCall()->isValid();
      return false;
    }

    /** \details
      Flushes any queued graphics to the display device.
      
      \remarks
      If called while the wrapped viewport object is in the interactive state, this function immediately returns.
    */
    void update()
    {
      if (trHas())
        trCall()->update();
    }

    /** \details
      Puts the wrapped viewport object into interactive state.
      
      \param frameRateInHz [in]  Frames/second.
      
      \remarks
      A frame rate of 0.0 specifies the default frame rate.
      \sa
      <link gs_views_interactivity_mode, Work with Interactivity Mode for Views>
    */
    void beginInteractivity(double frameRateInHz)
    {
      if (trHas())
        trCall()->beginInteractivity(frameRateInHz);
    }
    
    /** \details
      Removes the wrapped viewport object from the interactive state.
      \param frameRateInHz [in]  Frames/second.
      \sa
      <link gs_views_interactivity_mode, Work with Interactivity Mode for Views>
    */
    void endInteractivity()
    {
      if (trHas())
        trCall()->endInteractivity();
    }
    
    /** \details
      Checks whether the wrapped viewport object is in interactive state.
      \returns
      true if the wrapped view is in interactivity mode, false otherwise.
      This method returns false if there is no redirection view.
      \sa
      <link gs_views_interactivity_mode, Work with Interactivity Mode for Views>
    */
    bool isInInteractivity() const
    {
      if (trHas())
        return trCall()->isInInteractivity();
      return false;
    }
    
    /** \details
      Returns interactivity frame rate.
      This method returns 0.0 if there is no redirection view.
      \sa
      <link gs_views_interactivity_mode, Work with Interactivity Mode for Views>
    */
    double interactivityFrameRate() const
    {
      if (trHas())
        return trCall()->interactivityFrameRate();
      return 0.0;
    }

    /** \details
      Flushes the graphic pipeline and renders the wrapped viewport object.
      
      \remarks
      This function should be called only during interactivty. 
    */
    void flush()
    {
      if (trHas())
        trCall()->flush();
    }

    /** \details
      Hides the wrapped viewport object.
    */
    void hide()
    {
      if (trHas())
        trCall()->hide();
    }
    
    /** \details
      Un-hides the wrapped viewport object.
    */
    void show()
    {
      if (trHas())
        trCall()->show();
    }
    
    /** \details
      Checks whether the wrapped viewport object is not hidden.
      This method returns false if there is no redirection view.
      \returns
      true if the wrapped viewport object is visible, false otherwise.
    */
    bool isVisible()
    {
      if (trHas())
        return trCall()->isVisible();
      return false;
    }

    /** \details
      Freezes the specified Layer object in the wrapped viewport object.
      \param layerID [in]  Layer to freeze.
      \remarks
      All layers are thawed by default.
      \sa
      <link tv_working_with_attributes_layers, Work with Layers>
    */
    void freezeLayer(OdDbStub* layerID)
    {
      if (trHas())
        trCall()->freezeLayer(layerID);
    }
    
    /** \details
      Thaws the specified Layer object in the wrapped viewport object.
      \param layerID [in]  Layer to thaw.
      \remarks
      All layers are thawed by default.
      \sa
      <link tv_working_with_attributes_layers, Work with Layers>
    */
    void thawLayer(OdDbStub* layerID)
    {
      if (trHas())
        trCall()->thawLayer(layerID);
    }
    
    /** \details
      Thaws all Layer objects in the wrapped viewport object.
      \remarks
      All layers are thawed by default.
      \sa
      <link tv_working_with_attributes_layers, Work with Layers>
    */
    void clearFrozenLayers()
    {
      if (trHas())
        trCall()->clearFrozenLayers();
    }

    /** \details
      Marks for regeneration all subViewportDraw() geometry for the wrapped viewport object.
    */
    void invalidateCachedViewportGeometry()
    {
      if (trHas())
        trCall()->invalidateCachedViewportGeometry();
    }

    /** \details
      Runs selection procedure inside the wrapped viewport object.

      \param pts [in]  Selection points in device coordinate space.
      \param nPoints [in]  Count of selection points.
      \param pReactor [in]  Selection callback pointer.
      \param mode [in]  Selection mode.
      \sa
      <link tv_working_with_features_selections, Work with Selections>
    */
    void select(const OdGsDCPoint* pts, int nPoints, OdGsSelectionReactor* pReactor,
                OdGsView::SelectionMode mode = OdGsView::kCrossing)
    {
      if (trHas())
        trCall()->select(pts, nPoints, pReactor, mode);
    }

    /** \details
      Translates the camera position and target by the specified Camera Coordinate dolly amount.
       
      \param dollyVector [in]  Camera Coordinate dolly vector. 
      \remarks
      Camera Coordinates are relative to the view of a target from a camera. 
      * The Y-axis is along up vector. 
      * The Z-axis is along the vector from camera position to the target. 
      * The X-axis is the cross product Y-axis x Z-axis 
    */
    void dolly(const OdGeVector3d& dollyVector)
    {
      if (trHas())
        trCall()->dolly(dollyVector);
    }
    
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
    void dolly(double xDolly, double yDolly, double zDolly)
    {
      if (trHas())
        trCall()->dolly(xDolly, yDolly, zDolly);
    }
    
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
    void roll(double rollAngle)
    {
      if (trHas())
        trCall()->roll(rollAngle);
    }
    
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
    void orbit(double xOrbit, double yOrbit)
    {
      if (trHas())
        trCall()->orbit(xOrbit, yOrbit);
    }
    
    /** \details
      Scales the focal length of the camera by the specified amount.
       
      \param zoomFactor [in]  Zoom factor.
    */
    void zoom(double zoomFactor)
    {
      if (trHas())
        trCall()->zoom(zoomFactor);
    }
    
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
    void pan(double xPan, double yPan)
    {
      if (trHas())
        trCall()->pan(xPan, yPan);
    }

    /** \details
      Scales the camera to completely include specified WCS box inside view frustum.

      \param minPt [in]  minimal WCS box point.
      \param maxPt [in]  maximal WCS box point.
    */
    void zoomExtents(const OdGePoint3d &minPt, const OdGePoint3d &maxPt)
    {
      if (trHas())
        trCall()->zoomExtents(minPt, maxPt);
    }
    
    /** \details
      Scales the camera to completely include specified screen area.

      \param lowerLeft [in]  Lower-left corner in nomalized device coordinates [0.0 .. 1.0].
      \param upperRight [in]  Upper-right corner in nomalized device coordinates [0.0 .. 1.0].
    */
    void zoomWindow(const OdGePoint2d &lowerLeft, const OdGePoint2d &upperRight)
    {
      if (trHas())
        trCall()->zoomWindow(lowerLeft, upperRight);
    }

    /** \details
      Checks whether specified point is visible inside current view frustum.
      This method returns false if there is no redirection view.

      \param pt [in]  WCS point for check.
      \returns
      true if the specified point is visible in the wrapped viewport, false otherwise.
    */
    bool pointInView(const OdGePoint3d &pt) const
    {
      if (trHas())
        return trCall()->pointInView(pt);
      return false;
    }
    
    /** \details
      Checks whether specified WCS box is visible completely or partially inside current view frustum.
      This method returns false if there is no redirection view.

      \param minPt [in]  minimal WCS box point.
      \param maxPt [in]  maximal WCS box point.
      \returns
      true if the specified extents box is visible in the current viewport frustum of the wrapped view, false otherwise.
    */
    bool extentsInView(const OdGePoint3d &minPt, const OdGePoint3d &maxPt) const
    {
      if (trHas())
        return trCall()->extentsInView(minPt, maxPt);
      return false;
    }

    /** \details
      Clones the wrapped view object and returns a smart pointer to it.
      This method returns NULL view smart pointer valid if there is no redirection view.

      \param cloneViewParameters [in]  If and only if true, view parameters will be cloned.
      \param cloneGeometry [in]  If and only if true, geometry will be cloned.
      \returns
      smart pointer to the newly created object.
    */
    OdGsViewPtr cloneView(bool cloneViewParameters = true, bool cloneGeometry = false)
    {
      if (trHas())
        return trCall()->cloneView(cloneViewParameters, cloneGeometry);
      return OdGsViewPtr();
    }
    
    /** \details
      Sets the view parameters of the specified OdGsView object to that of the wrapped viewport object.
      \param pView [out]  Receives the view parameters.
    */
    void viewParameters(OdGsView* pView) const
    {
      if (trHas())
        trCall()->viewParameters(pView);
    }

    /** \details
      Checks whether any view parameters have exceeded their boundaries since the last
      call to this function.
      \returns 
      true if any view parameters have exceeded their boundaries since the last call to this function, false otherwise.
      This method returns false if there is no redirection view.
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
    bool exceededBounds()
    {
      if (trHas())
        return trCall()->exceededBounds();
      return false;
    }

    /** \details
      Controls stereo viewing for the wrapped viewport object.
      
      \param enable [in]  Controls stereo viewing.
    */
    void enableStereo(bool enabled)
    {
      if (trHas())
        trCall()->enableStereo(enabled);
    }
    
    /** \details
      Checks whether stereo viewing is enabled for the wrapped viewport object.
      \returns
      true if and only if stereo viewing is enabled for the wrapped viewport object, false otherwise.
      This method returns false if there is no redirection view.
    */
    bool isStereoEnabled() const
    {
      if (trHas())
        return trCall()->isStereoEnabled();
      return false;
    }
    
    /** \details
      Sets the stereo parameters for the wrapped viewport Object.
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
    void setStereoParameters(double magnitude, double parallax)
    {
      if (trHas())
        trCall()->setStereoParameters(magnitude, parallax);
    }
    
    /** \details
      Returns the stereo parameters for the wrapped viewport Object.
      
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
    void getStereoParameters(double& magnitude, double& parallax) const
    {
      if (trHas())
        trCall()->getStereoParameters(magnitude, parallax);
    }

    /** \details
      Returns an iterator to traverse the lights attached to the wrapped viewport object.
      \param pLightsIterator [in]  Pointer to the lights iterator.
    */
    void initLights(OdRxIterator* pLightsIterator)
    {
      if (trHas())
        trCall()->initLights(pLightsIterator);
    }

    /** \details
      Sets a multiplier that is used to scale all linetypes in the wrapped viewport object.
      
      \param linetypeScaleMultiplier [in]  Linetype scale multiplier.
    */
    void setLinetypeScaleMultiplier(double linetypeScaleMultiplier)
    {
      if (trHas())
        trCall()->setLinetypeScaleMultiplier(linetypeScaleMultiplier);
    }
    
    /** \details
      Returns current linetype scale multiplier value.
      This method returns 1.0 if there is no redirection view.
      \sa
      <exref target="https://docs.opendesign.com/td/db_linetype.html">Working with Linetypes</exref>
    */
    double linetypeScaleMultiplier() const
    {
      if (trHas())
        return trCall()->linetypeScaleMultiplier();
      return 1.0;
    }
    
    /** \details
      Sets a multiplier that could be used as an alternate scale factor for linetypes in the wrapped viewport object.
      
      \param linetypeAlternateScaleMultiplier [in]  Alternate linetype scale multiplier.
      \sa
      <exref target="https://docs.opendesign.com/td/db_linetype.html">Working with Linetypes</exref>
    */
    void setAlternateLinetypeScaleMultiplier(double linetypeAlternateScaleMultiplier)
    {
      if (trHas())
        trCall()->setAlternateLinetypeScaleMultiplier(linetypeAlternateScaleMultiplier);
    }
    
    /** \details
      Returns current alternate linetype scale multiplier value.
      This method returns 1.0 if there is no redirection view.
      \sa
      <exref target="https://docs.opendesign.com/td/db_linetype.html">Working with Linetypes</exref>
    */
    double linetypeAlternateScaleMultiplier() const
    {
      if (trHas())
        return trCall()->linetypeAlternateScaleMultiplier();
      return 1.0;
    }

    /** \details
      Returns the ClientViewInfo for the wrapped viewport object.
      
      \param clientViewInfo [out]  Receives the ClientViewInfo.
    */
    void clientViewInfo(OdGsClientViewInfo& clientViewInfo) const
    {
      if (trHas())
        trCall()->clientViewInfo(clientViewInfo);
    }

    /** \details
      Sets the clear color for the wrapped view.

      \param color [in]  The clear color.
    */
    void setClearColor(OdGsView::ClearColor color)
    {
      if (trHas())
        trCall()->setClearColor(color);
    }

    /** \details
      Checks whether the specified point is inside viewport area.
      This method returns false if there is no redirection view.

      \param screenPoint [in]  Check point in device coordinate space.
      \returns
      true if the passed point is inside the viewport, false otherwise.
    */
    bool pointInViewport(const OdGePoint2d& screenPoint) const
    {
      if (trHas())
        return trCall()->pointInViewport(screenPoint);
      return false;
    }

    /** \details
      Returns the display pixel density at the specified point for the wrapped viewport object.

      \param point [in]  WCS center of the unit square.
      \param pixelDensity [out]  Receives the pixel density.
      \param bUsePerspective [in]  Enable perspective inclusion into pixel density calculation.
      \remarks
      Pixel density is measured in pixels per WCS unit.
      This function can be used to determine if the geometry generated for an object will 
      be smaller than the size of a pixel.
    */
    void getNumPixelsInUnitSquare(const OdGePoint3d& point,
                                  OdGePoint2d& pixelDensity, bool bUsePerspective = true) const
    {
      if (trHas())
        trCall()->getNumPixelsInUnitSquare(point, pixelDensity, bUsePerspective);
    }

    /** \details
      Sets background object for the wrapped view.

      \param backgroundId [in]  New background object ID.
      \sa
      <link tv_working_with_attributes_view_backgrounds, Work with View Backgrounds>
    */
    void setBackground(OdDbStub *backgroundId)
    {
      if (trHas())
        trCall()->setBackground(backgroundId);
    }
    
    /** \details
      Returns current background object ID for the wrapped view.
      This method returns NULL if there is no redirection view.
      \sa
      <link tv_working_with_attributes_view_backgrounds, Work with View Backgrounds>
    */
    OdDbStub *background() const
    {
      if (trHas())
        return trCall()->background();
      return NULL;
    }

    /** \details
      Sets visual style object for the wrapped view.

      \param visualStyleId [in]  New visual style object ID.
      \sa
      <link tv_working_with_attributes_visual_styles, Work with Visual Styles>
    */
    void setVisualStyle(OdDbStub *visualStyleId)
    {
      if (trHas())
        trCall()->setVisualStyle(visualStyleId);
    }
    
    /** \details
      Returns current visual style object ID for the wrapped view.
      This method returns NULL if there is no redirection view.
      \sa
      <link tv_working_with_attributes_visual_styles, Work with Visual Styles>
    */
    OdDbStub *visualStyle() const
    {
      if (trHas())
        return trCall()->visualStyle();
      return NULL;
    }
    
    /** \details
      Sets visual style for the wrapped view.

      \param visualStyle [in]  New visual style data.
      \sa
      <link tv_working_with_attributes_visual_styles, Work with Visual Styles>
    */
    void setVisualStyle(const OdGiVisualStyle &visualStyle)
    {
      if (trHas())
        trCall()->setVisualStyle(visualStyle);
    }
    
    /** \details
      Returns current visual style for the wrapped view.
      This method returns false if there is no redirection view.

      \param visualStyle [out]  OdGiVisualStyle structure to fill by current visual style data.

      \returns
      Returns true if and only if OdGiVisualStyle structure is filled by visual style data.
      \sa
      <link tv_working_with_attributes_visual_styles, Work with Visual Styles>
    */
    bool visualStyle(OdGiVisualStyle &visualStyle) const
    {
      if (trHas())
        return trCall()->visualStyle(visualStyle);
      return false;
    }

    /** \details
      Enables default lighting for the wrapped view.

      \param bEnable [in]  New default lighting state.
      \param lightType [in]  New default lighting type.
      \sa
      <link tv_working_with_lights, Work with Lights>
    */
    void enableDefaultLighting(bool bEnable, OdGsView::DefaultLightingType lightType = OdGsView::kTwoLights)
    {
      if (trHas())
        trCall()->enableDefaultLighting(bEnable, lightType);
    }

    /** \details
      Returns image of current view buffer.

      \param pImage [out]  Pointer for image to return.
      \param region [in]  Rectangle specifies region to return.
      \remarks
      Implementation can throw eNotImplementedYet exception if current vectorization module doesn't support snap shots.
    */
    void getSnapShot(OdGiRasterImagePtr &pImage, const OdGsDCRect &region)
    {
      if (trHas())
        trCall()->getSnapShot(pImage, region);
    }

    /** \details
      Runs collision detection procedure inside the wrapped viewport object.

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
    void collide( OdGiPathNode const*const* pInputList, OdUInt32 nInputListSize, OdGsCollisionDetectionReactor* pReactor,
                  OdGiPathNode const*const* pCollisionWithList = NULL, OdUInt32 nCollisionWithListSize = 0,
                  const OdGsCollisionDetectionContext* pCtx = NULL )
    {
      if( trHas())
        trCall()->collide( pInputList, nInputListSize, pReactor, pCollisionWithList, nCollisionWithListSize, pCtx );
    }

    /** \details
      Enable or disable anti-aliasing in current Viewport object.

      \param nLevel [in]  Anti-aliasing mode (set as 3 by default). Set as 0 to disable anti-aliasing.
    */
    void enableAntiAliasing(OdUInt32 nMode)
    {
      if (trHas())
        trCall()->enableAntiAliasing(nMode);
    }

    /** \details
      Returns anti-aliasing mode for current Viewport object.
    */
    OdUInt32 antiAliasingMode() const
    {
      if (trHas())
        return trCall()->antiAliasingMode();
      return 3;
    }
};

/** \details
    Base template for OdGsDevice class wrappers.

    Corresponding C++ library: TD_Gs

    <group OdGs_Classes> 
*/
template <typename GsDeviceInterface = OdGsDevice, typename RedirectionBase = OdGsDefaultRedirectionBase<OdGsDevice, GsDeviceInterface> >
class OdGsDeviceWrapper : public RedirectionBase
{
  private:
    bool trHas() const { return this->hasRedirection(); }
    const OdGsDevice *trCall() const { return this->redirection(); }
    OdGsDevice *trCall() { return this->redirection(); }
  public:
    
    /** \details
      Default constructor for the OdGsDeviceWrapper class. Sets the redirection (wrapped) device.
      
      \param pRedirectionDevice    [in]  Pointer to the redirection (wrapped) device.
    */
    OdGsDeviceWrapper(OdGsDevice *pRedirectionDevice = NULL)
      : RedirectionBase(pRedirectionDevice)
    {}

    // OdGsDevice wrappers

    /** \details
      Returns a SmartPointer to the dictionary containing the properties for this Device object.
      This method returns smart pointer to an empty dictionary if there is no redirection device.
      
      \sa
      <link gs_DeviceProps, ODA Vectorization Devices Properties>
    */
    OdRxDictionaryPtr properties()
    {
      if (trHas())
        return trCall()->properties();
      return OdRxDictionaryPtr();
    }

    /** \details
      Returns the User Context object associated with this Device object.
      This method returns NULL if there is no redirection device.
    */
    OdGiContext* userGiContext() const
    {
      if (trHas())
        return trCall()->userGiContext();
      return NULL;
    }
    
    /** \details
      Sets the User Context object associated with this Device object.
      \param pUserGiContext [in]  Pointer to the User Context.
    */
    void setUserGiContext(OdGiContext* pUserGiContext)
    {
      if (trHas())
        trCall()->setUserGiContext(pUserGiContext);
    }

    /** \details
      Marks for refresh entire region of the GUI window of this Device object.
    */
    void invalidate()
    {
      if (trHas())
        trCall()->invalidate();
    }
    
    /** \details
      Marks for refresh the specified region of the GUI window of this Device object.
       
      \param screenRect [in]  Rectangular area.
    */
    void invalidate(const OdGsDCRect &screenRect)
    {
      if (trHas())
        trCall()->invalidate(screenRect);
    }
    
    /** \details
      Checks whether the GUI window for this Device object is showing the correct image.
      This method returns NULL if there is no redirection device.
      
      \returns 
      true if the GUI window for this Device object is showing the correct image, false otherwise.
      \remarks
      This implies the following:
      * No part of GUI area is flagged for refresh.
      * All rasterizations are complete
      * The rendering pipeline has been flushed.
    */
    bool isValid() const
    {
      if (trHas())
        return trCall()->isValid();
      return false;
    }

    /** \details
      Updates the GUI window for this Device object.
      
      \param pUpdatedRect [out]  Pointer to the a rectangle to receive the region updated by this function.

      \remarks
      This function call is propagated to all OdGsView objects owned by this Device object,
      thus displaying the correct image on the GUI window of this Device object.
    */
    void update(OdGsDCRect* pUpdatedRect = 0)
    {
      if (trHas())
        trCall()->update(pUpdatedRect);
    }

    /** \details
      Notification function called whenever the size of the GUI window for this Device object has changed.
      
      \param outputRect [in]  New size of the output rectangle in device coordinates (pixels).

      \remarks
      This function should not be called if this Device object has a width and/or height of 0, .
    */
    void onSize(const OdGsDCRect& outputRect)
    {
      if (trHas())
        trCall()->onSize(outputRect);
    }
    
    /** \details
      Notification function called whenever the size of the GUI window for this Device object has changed.
      
      \param outputRect [in]  New size of the output rectangle in device coordinates (pixels).

      \remarks
      This function should not be called if this Device object has a width and/or height of 0, .
    */
    void onSize(const OdGsDCRectDouble& outputRect)
    {
      if (trHas())
        trCall()->onSize(outputRect);
    }

    /** \details
      Returns device surface output rectangle.

      \param outputRect [out]  Obtains size of the output rectangle in device coordinates (pixels).
    */
    void getSize(OdGsDCRect& outputRect) const
    {
      if (trHas())
        trCall()->getSize(outputRect);
    }
    
    /** \details
      Returns device surface output rectangle.

      \param outputRect [out]  Obtains size of the output rectangle in device coordinates (pixels).
    */
    void getSize(OdGsDCRectDouble& outputRect) const
    {
      if (trHas())
        trCall()->getSize(outputRect);
    }

    /** \details
      Causes this Device object to realize its foreground palette.
    */
    void onRealizeForegroundPalette()
    {
      if (trHas())
        trCall()->onRealizeForegroundPalette();
    }
    
    /** \details
      Causes this Device object to realize its background palette.
    */
    void onRealizeBackgroundPalette()
    {
      if (trHas())
        trCall()->onRealizeBackgroundPalette();
    }
    
    /** \details
      Notification function called whenever the OS screen resolution and/or color depth have changed.
      
      \param bitsPerPixel [in]  Color depth.
      \param xPixels [in]  X pixels.
      \param yPixels [in]  Y pixels.
    
      \remarks
      This function is called after the operation.
    */
    void onDisplayChange(int bitsPerPixel, int xPixels, int yPixels)
    {
      if (trHas())
        trCall()->onDisplayChange(bitsPerPixel, xPixels, yPixels);
    }

    /** \details
      Creates a new OdGsView object, and associates it with this Device object.
      
      \param pViewInfo [in]  Pointer to the Client View Information for this Device object.
      \param enableLayerVisibilityPerView [in]  Layer visibility per viewport is supported, if and only if true.
      
      \returns
      Returns a SmartPointer to the newly created object.
      This method returns a NULL smart pointer to a view if there is no redirection device.
      pViewInfo contains information returned by OdGiViewport::acadWindowId() and OdGiViewportDraw::viewportObjectId()
    */
    OdGsViewPtr createView(const OdGsClientViewInfo* pViewInfo = 0, bool enableLayerVisibilityPerView = false)
    {
      if (trHas())
        return trCall()->createView(pViewInfo, enableLayerVisibilityPerView);
      return OdGsViewPtr();
    }
    
    /** \details
      Associates the specified Viewport object with this Device object.
      
      \param pView [in]  Pointer to the OdGsView object.
    */
    void addView(OdGsView* pView)
    {
      if (trHas())
        trCall()->addView(pView);
    }

    /** \details
      Creates a new OdGsModel object, and associates it with this Device object.
      This method returns a NULL smart pointer to a model if there is no redirection device.
      
      \returns
      Returns a SmartPointer to the newly created object.
    */
    OdGsModelPtr createModel()
    {
      if (trHas())
        return trCall()->createModel();
      return OdGsModelPtr();
    }
    
    /** \details
      Checks compatibility between specified OdGsModel and this Device object.

      \param pModel [in]  Model object pointer to check compatibility.
      \returns
      true if the provided model is compatible with this device, false otherwise.
      This method returns false if there is no redirection device.
      \remarks
      Use this method to check compatibility of exist Model object and newly created device
      to be sure the exist Model object could be used with this Device object.
      By default this always returns false. It is must be implemented in inherited classes
      to enable Model object reuse support. If Device object use overriden Model object
      it should return false if input Model object doesn't overriden.
      Typical way to check compatibility is using of RTTI.
      If your metafiles cache doesn't support per-device sharing override and return false.
    */
    bool isModelCompatible(OdGsModel* pModel) const
    {
      if (trHas())
        return trCall()->isModelCompatible(pModel);
      return false;
    }

    /** \details
      Stores current device state into filer object.

      \param pFiler [out]  Filer object.
      \returns
      true if device state is successfully saved, false if device state saving is not supported.
      This method returns false if there is no redirection device.
    */
    bool saveDeviceState(OdGsFiler *pFiler) const
    {
      if (trHas())
        return trCall()->saveDeviceState(pFiler);
      return false;
    }
    
    /** \details
      Restores device state from filer object.

      \param pFiler [in]  Filer object.
      \returns
      true if device state is successfully loaded, false if device state loading is not supported.
      This method returns false if there is no redirection device.
    */
    bool loadDeviceState(OdGsFiler *pFiler)
    {
      if (trHas())
        return trCall()->loadDeviceState(pFiler);
      return false;
    }

    /** \details
      Inserts the specified Viewport object to the specified position in this Device object.

      \param viewIndex [in]  View index.
      \param pView [in]  Pointer to the OdGsView object.
    */
    void insertView(int viewIndex, OdGsView* pView)
    {
      if (trHas())
        trCall()->insertView(viewIndex, pView);
    }
    
    /** \details
      Erases the specified View object.
      \param pView [in]  Pointer to the OdGsView object.

      \returns
      true if view is erased successfully false otherwise.
      This method returns false if there is no redirection device.
    */
    bool eraseView(OdGsView* pView)
    {
      if (trHas())
        return trCall()->eraseView(pView);
      return false;
    }
    
    /** \details
      Erases the specified View object.

      \param viewIndex [in]  View index.
      \returns
      true if view is erased successfully false otherwise.
      This method returns false if there is no redirection device.
    */
    bool eraseView(int viewIndex)
    {
      if (trHas())
        return trCall()->eraseView(viewIndex);
      return false;
    }
    
    /** \details
      Erases all views associated with this Device object.
    */
    void eraseAllViews()
    {
      if (trHas())
        trCall()->eraseAllViews();
    }
    
    /** \details
      Returns the number of views associated with this Device object.
      This method returns 0 if there is no redirection device.
    */
    int numViews() const
    {
      if (trHas())
        return trCall()->numViews();
      return 0;
    }
    
    /** \details
      Returns the specified OdGsView object associated with this Device object.
      This method returns NULL if there is no redirection device.
      \param viewIndex [in]  View index.
    */
    OdGsView* viewAt(int viewIndex)
    {
      if (trHas())
        return trCall()->viewAt(viewIndex);
      return NULL;
    }

    /** \details
      Sets the Background Color of the GUI window of this Device object.
      \param backgroundColor [in]  Background color.
      \returns
      true if background color is set successfully, false otherwise.
      This method returns false if there is no redirection device.
    */
    bool setBackgroundColor(ODCOLORREF backgroundColor)
    {
      if (trHas())
        return trCall()->setBackgroundColor(backgroundColor);
      return false;
    }
    
    /** \details
      Returns the Background Color of the GUI window of this Device object.
      This method returns ODRGB(0, 0, 0) if there is no redirection device.
    */
    ODCOLORREF getBackgroundColor()
    {
      if (trHas())
        return trCall()->getBackgroundColor();
      return ODRGB(0, 0, 0);
    }

    /** \details
      Sets the logical palette to be used by this Device object.
      
      \param logicalPalette [in]  Logical palette.
      \param numColors [in]  Number of colors in palette.
      
      \remarks
      The logical palette is used with calls to OdGiSubEntityTraits::setColor().
    */
    void setLogicalPalette(const ODCOLORREF* logicalPalette, int numColors)
    {
      if (trHas())
        trCall()->setLogicalPalette(logicalPalette, numColors);
    }
    
    /** \details
      Returns the logical palette used by this Device object.
      This method returns NULL if there is no redirection device.

      \param numColors [out]  Number of colors in palette.
    */
    const ODCOLORREF* getLogicalPalette(int &numColors) const
    {
      if (trHas())
        return trCall()->getLogicalPalette(numColors);
      return NULL;
    }

    /** \details
      Returns image of current device buffer.

      \param pImage [out]  Pointer for image to return.
      \param region [in]  Rectangle specifies region to return.

      \remarks
      Implementation can throw eNotImplementedYet exception if current vectorization module doesn't support snap shots.
    */
    void getSnapShot(OdGiRasterImagePtr &pImage, const OdGsDCRect &region)
    {
      if (trHas())
        trCall()->getSnapShot(pImage, region);
    }

    /** \details
      Returns OdGsUpdateManager associated with current device.
      This method returns not valid smart pointer to update manager if there is no redirection device.

      \param createIfNotExist [in] Creates instance of OdGsUpdateManager if no one associated with device
    */
    OdGsUpdateManagerPtr getUpdateManager( bool createIfNotExist )
    {
      if( trHas())
      {
        return trCall()->getUpdateManager( createIfNotExist );
      }
      OdGsUpdateManagerPtr res;
      return res;
    }

    /** \details
      Associates OdGsUpdateManager with device

      \param pManager [in] Pointer to the OdGsUpdateManager
    */
    void setUpdateManager( OdGsUpdateManagerPtr pManager )
    {
      if( trHas() )
      {
        trCall()->setUpdateManager( pManager );
      }
    }
};

/** \details
    Minimal implementation for OdGsView class wrappers.

    Corresponding C++ library: TD_Gs

    <group OdGs_Classes> 
*/
template <typename GsViewInterface = OdGsView, typename RedirectionBase = OdGsDefaultRedirectionBase<OdGsView, GsViewInterface> >
class OdGsViewWrapperMinimalImpl : public OdGsViewWrapper<GsViewInterface, RedirectionBase>
{
  protected:
    OdGsDevice *m_pDevice;
    OdGiContext *m_pContext;
    double m_lwdDcScale;
    OdGePoint2d m_llVpt, m_urVpt;
    OdGsDCRectDouble m_dcRect;
    OdIntArray m_nrcClipCounts;
    OdGePoint2dArray m_nrcClipPoints;
    ODCOLORREF m_borderColor;
    int m_borderWidth;
    bool m_bBorderVisible;
    OdGePoint3d m_position, m_target;
    OdGeVector3d m_upVector;
    double m_fieldsWidth, m_fieldsHeight;
    OdGsView::Projection m_projection;
    double m_lensLength;
    bool m_bFrontClip, m_bBackClip;
    double m_frontClip, m_backClip;
    OdGsView::RenderMode m_renderMode;
    bool m_bHidden;
    double m_ltpMult1, m_ltpMult2;
    OdGsClientViewInfo m_viewInfo;
    OdDbStub *m_bkgnd, *m_visualStyle;
    OdUInt32 m_antiAliasingMode;
  public:
    
    /** \details
      Default constructor for the OdGsViewWrapperMinimalImpl class. Sets the following data members:
      <table>
      Data member                             Value                                             Description
      m_pDevice                               0                                                 Owning device
      m_pContext                              NULL                                              Vectorization context
      m_lwdDcScale                            0.0                                               Scale factor for displaying lineweights in the Viewport
      m_borderColor                           0                                                 View border color
      m_borderWidth                           0                                                 View border width
      m_borderVisible                         false                                             View border visibility
      m_target                                OdGePoint3d::kOrigin + OdGeVector3d::kZAxis       Camera target
      m_upVector                              OdGeVector3d::kYAxis                              Camera "up" vector
      m_fieldWidth                            1.0                                               Camera field width
      m_fieldHeight                           1.0                                               Camera field height
      m_projection                            OdGsView::kParallel                               Camera projection
      m_lensLength                            50.0                                              Camera lens length
      m_bFrontClip                            false                                             Flag that specifies whether front clipping is enabled
      m_bBackClip                             false                                             Flag that specifies whether back clipping is enabled
      m_frontClip                             0.0                                               Front clipping distance
      m_backClip                              0.0                                               Back clipping distance
      m_renderMode                            OdGsView::k2DOptimized                            Render mode
      m_bHidden                               false                                             Flag that specifies whether viewport is hidden
      m_ltpMult1                              1.0                                               Multiplier that is used to scale all linetypes in the Viewport
      m_ltpMult2                              1.0                                               Alternate multiplier that is used to scale all linetypes in the Viewport
      m_bkgnd                                 NULL                                              Identifier of a view background object
      m_visualStyle                           NULL                                              Identifier of a visual style object
      m_antiAliasingMode                      3                                                 Anti-aliasing mode
      </table>
    */
    OdGsViewWrapperMinimalImpl(OdGsView *pRedirectionView = NULL)
      : OdGsViewWrapper<GsViewInterface, RedirectionBase>(pRedirectionView)
      , m_pDevice(NULL)
      , m_pContext(NULL)
      , m_lwdDcScale(0.0)
      , m_borderColor(0)
      , m_borderWidth(1)
      , m_bBorderVisible(false)
      , m_target(OdGePoint3d::kOrigin + OdGeVector3d::kZAxis)
      , m_upVector(OdGeVector3d::kYAxis)
      , m_fieldsWidth(1.0)
      , m_fieldsHeight(1.0)
      , m_projection(OdGsView::kParallel)
      , m_lensLength(50.0)
      , m_bFrontClip(false)
      , m_bBackClip(false)
      , m_frontClip(0.0)
      , m_backClip(0.0)
      , m_renderMode(OdGsView::k2DOptimized)
      , m_bHidden(false)
      , m_ltpMult1(1.0)
      , m_ltpMult2(1.0)
      , m_bkgnd(NULL)
      , m_visualStyle(NULL)
      , m_antiAliasingMode(3)
    {}

    // OdGsView wrappers

    /** \details
      Returns the GsDevice object associated with the wrapped viewport object.
      \sa
      <link tv_working_with_base_classes_device, Work with Visualize Graphics Devices>
    */
    OdGsDevice* device() const { return m_pDevice; }
    
    /** \details
      Sets the owning device for the wrapped viewport.
      \sa
      <link tv_working_with_base_classes_device, Work with Visualize Graphics Devices>
    */
    void setDevice(OdGsDevice *pDevice) { m_pDevice = pDevice; }

    /** \details
      Returns pointer to the GiContext object associated with the wrapped viewport object.
    */
    OdGiContext* userGiContext() const { return m_pContext; }
    
    /** \details
      Sets the User Context object associated with the wrapped viewport object.
      
      \param pUserGiContext [in]  Pointer to the User Context.
    */
    void setUserGiContext(OdGiContext* pUserGiContext) { m_pContext = pUserGiContext; }

    /** \details
      Returns the scale factor for displaying Lineweights in the wrapped viewport object.
      \sa
      <link tv_working_with_attributes_lineweights, Work with Lineweights>
    */
    double lineweightToDcScale() const { return m_lwdDcScale; }
    
    /** \details
      Sets the scale factor for displaying Lineweights in the wrapped viewport object.
      \param scale [in]  Scale factor.
      \sa
      <link tv_working_with_attributes_lineweights, Work with Lineweights>
    */
    void setLineweightToDcScale(double scale) { m_lwdDcScale = scale; }

    /** \details
      Sets the size and position of the wrapped viewport object.
      
      \param lowerLeft [in]  Lower-left corner in nomalized device coordinates [0.0 .. 1.0].
      \param upperRight [in]  Upper-right corner in nomalized device coordinates [0.0 .. 1.0].   
      \remarks
      (0,0) is the lower-left corner of the owning Device object. Coordinates increase upward and to the right.
      The View object can be partly or completely off screen.    
    */
    void setViewport(const OdGePoint2d& lowerLeft, const OdGePoint2d& upperRight)
    { m_llVpt = lowerLeft; m_urVpt = upperRight; }
    
    /** \details
      Sets the size and position of the wrapped viewport object.
      
      \param screenRect [in]  Screen rectangle in device coordinates (pixels).    
      \remarks
      (0,0) is the lower-left corner of the owning Device object. Coordinates increase upward and to the right. 
      \remarks
      The View object can be partly or completely off screen.    
    */
    void setViewport(const OdGsDCRect& screenRect) { m_dcRect = OdGsDCRectDouble(screenRect); }
    
    /** \details
      Sets the size and position of the wrapped viewport object.
      
      \param screenRect [in]  Screen rectangle in device coordinates (pixels).    
      \remarks
      (0,0) is the lower-left corner of the owning Device object. Coordinates increase upward and to the right.
      The View object can be partly or completely off screen.    
    */
    void setViewport(const OdGsDCRectDouble& screenRect) { m_dcRect = screenRect; }
    
    /** \details
      Returns the size and position of the wrapped viewport object.
      \param lowerLeft [out]  Receives the lower-left corner in nomalized device coordinates [0.0 .. 1.0].
      \param upperRight [out]  Receives the upper-right corner in nomalized device coordinates [0.0 .. 1.0].
      
      \remarks
      (0,0) is the lower-left corner of the owning Device object.  Coordinates increase upward and to the right.
      
      \remarks
      The View object may be partly or completely off screen.    
    */
    void getViewport(OdGePoint2d& lowerLeft, OdGePoint2d& upperRight) const
    { lowerLeft = m_llVpt; upperRight = m_urVpt; }
    
    /** \details
      Returns the size and position of the wrapped viewport object.
      \param screenRect [out]  Receives the screen rectangle in device coordinates (pixels).
      
      \remarks
      (0,0) is the lower-left corner of the owning Device object.  Coordinates increase upward and to the right.
      
      \remarks
      The View object may be partly or completely off screen.    
    */
    void getViewport(OdGsDCRect& screenRect) const { screenRect = m_dcRect.round(); }
    
    /** \details
      Returns the size and position of the wrapped viewport object.
      \param screenRect [out]  Receives the screen rectangle in device coordinates (pixels).
      \remarks
      (0,0) is the lower-left corner of the owning Device object.  Coordinates increase upward and to the right.
      The View object may be partly or completely off screen.    
    */
    void getViewport(OdGsDCRectDouble& screenRect) const { screenRect = m_dcRect; }

    /** \details
      Defines a polygonal clip region for the wrapped viewport object.
      
      \param numCoutours [in]  Number of polygonal contours.
      \param numVertices [in]  Array of the number of vertices in each polygonal contour.
      \param vertices [in]  Array of device coordinate (pixel) vertices defining the polygonal contours.
    */
    void setViewportClipRegion(int numContours, int const* numVertices, OdGsDCPoint const* vertices)
    {
      m_nrcClipCounts.resize(numContours);
      int numVertexes = 0;
      for (int numContour = 0; numContour < numContours; numContour++)
        numVertexes += numVertices[numContour], m_nrcClipCounts[numContour] = numVertices[numContour];
      m_nrcClipPoints.resize(numVertexes);
      for (int numVertex = 0; numVertex < numVertexes; numVertex++)
        m_nrcClipPoints[numVertex].set(vertices[numVertex].x, vertices[numVertex].y);
    }
    
    /** \details
      Defines a polygonal clip region for the wrapped viewport object.
      
      \param numCoutours [in]  Number of polygonal contours.
      \param numVertices [in]  Array of the number of vertices in each polygonal contour.
      \param vertices [in]  Array of device coordinate (pixel) vertices defining the polygonal contours.
    */
    void setViewportClipRegion(int numContours, int const* numVertices, OdGePoint2d const* vertices)
    {
      m_nrcClipCounts.resize(numContours);
      int numVertexes = 0;
      for (int numContour = 0; numContour < numContours; numContour++)
        numVertexes += numVertices[numContour], m_nrcClipCounts[numContour] = numVertices[numContour];
      m_nrcClipPoints.resize(numVertexes);
      for (int numVertex = 0; numVertex < numVertexes; numVertex++)
        m_nrcClipPoints[numVertex] = vertices[numVertex];
    }
    
    /** \details
      Returns a polygonal clip region for the wrapped viewport object.
      
      \param counts   [out]  Array of the number of vertices in each polygonal contour.
      \param vertices [out]  Array of device coordinate (pixel) vertices defining the polygonal contours.
    */
    void viewportClipRegion(OdIntArray& counts, OdGsDCPointArray& vertices) const
    {
      counts = m_nrcClipCounts;
      const OdUInt32 nPoints = m_nrcClipPoints.size();
      vertices.resize(nPoints);
      OdGsDCPoint *pPt = vertices.asArrayPtr();
      for (OdUInt32 nPt = 0; nPt < nPoints; nPt++)
        pPt[nPt].x = (long)m_nrcClipPoints[nPt].x, pPt[nPt].y = (long)m_nrcClipPoints[nPt].y;
    }
    
    /** \details
      Returns a polygonal clip region for the wrapped viewport object.
      
      \param counts [out]  Array of the number of vertices in each polygonal contour.
      \param vertices [out]  Array of device coordinate (pixel) vertices defining the polygonal contours.
    */
    void viewportClipRegion(OdIntArray& counts, OdGePoint2dArray& vertices) const
    { counts = m_nrcClipCounts; vertices = m_nrcClipPoints; }

    /** \details
      Sets the color and width of the border for the wrapped viewport object.
      \param color [in]  Border color.
      \param width [in]  Border width in pixels.
    */
    void setViewportBorderProperties(ODCOLORREF color, int width)
    { m_borderColor = color; m_borderWidth = width; }
    
    /** \details
      Returns the color and width of the border for the wrapped viewport object.
      \param color [out]  Receives the border color.
      \param width [out]  Receives the border width in pixels.
    */
    void getViewportBorderProperties(ODCOLORREF& color, int& width) const
    { color = m_borderColor; width = m_borderWidth; }
    
    /** \details
      Controls the border visibility for the wrapped viewport object.
      \param visible [in]  Controls visibility.
    */
    void setViewportBorderVisibility(bool visible) { m_bBorderVisible = visible; }
    
    /** \details
      Checks whether the border is visible.
      \returns 
      true if the border is visible for the wrapped viewport object, false otherwise.
    */
    bool isViewportBorderVisible() const { return m_bBorderVisible; }

    /** \details
      Sets the camera parameters for the wrapped viewport object.
      
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
    void setView(const OdGePoint3d& position, const OdGePoint3d& target, const OdGeVector3d& upVector,
                 double fieldWidth, double fieldHeight, OdGsView::Projection projectionType = OdGsView::kParallel)
    {
      m_position = position; m_target = target; m_upVector = upVector;
      m_fieldsWidth = fieldWidth; m_fieldsHeight = fieldHeight; m_projection = projectionType;
    }
    
    /** \details
      Returns the WCS camera position for the wrapped viewport object.
    */
    OdGePoint3d position() const { return m_position; }
    
    /** \details
      Returns the WCS camera target for the wrapped viewport object.
    */
    OdGePoint3d target() const { return m_target; }
    
    /** \details
      Returns the WCS camera up vector for the wrapped viewport object.
    */
    OdGeVector3d upVector() const { return m_upVector; }
    
    /** \details
      Returns the perspective lens length for the wrapped viewport object.
    */
    double lensLength() const { return m_lensLength; }
    
    /** \details
      Sets the perspective lens length for the wrapped viewport object.
      \param lensLength [in]  Perspective lens length.
    */
    void setLensLength(double lensLength) { m_lensLength = lensLength; }
    
    /** \details
      Checks whether the projection type for the wrapped viewport object is kPerspective.
      \returns
      true if projection type is kPerspective, false otherwise.
    */
    bool isPerspective() const { return m_projection == OdGsView::kPerspective; }
    
    /** \details
      Returns the WCS projection plane (field) width for the wrapped viewport object.
    */
    double fieldWidth() const { return m_fieldsWidth; }
    
    /** \details
      Returns the WCS projection plane (field) height for the wrapped viewport object.
    */
    double fieldHeight() const { return m_fieldsHeight; }

    /** \details
      Controls the front clipping of the wrapped viewport object.
      \param enable [in]  Enable front clipping.
    */
    void setEnableFrontClip(bool enable) { m_bFrontClip = enable; }
    
    /** \details
      Checks whether front clipping is enabled for the wrapped viewport object.
      \returns
      true if front clipping is enabled, false otherwise.
    */
    bool isFrontClipped() const { return m_bFrontClip; }
    
    /** \details
      Sets the front clip distance from the target of the wrapped viewport object.
      \param frontClip [in]  Front clip distance.
    */
    void setFrontClip(double frontClip) { m_frontClip = frontClip; }
    
    /** \details
      Returns the front clip distance from the target of the wrapped viewport object.
    */
    double frontClip() const { return m_frontClip; }
    
    /** \details
      Controls the back clipping of the wrapped viewport object.
      \param enable [in]  Enable back clipping.
    */
    void setEnableBackClip(bool enable) { m_bBackClip = enable; }
    
    /** \details
      Checks whether back clipping is enabled for the wrapped viewport object.
      \returns
      true if back clipping is enabled, false otherwise.
    */
    bool isBackClipped() const { return m_bBackClip; }
    
    /** \details
      Sets the back clip distance from the target of the wrapped viewport object.
      \param backClip [in]  Back clip distance.
    */
    void setBackClip(double backClip) { m_backClip = backClip; }
    
    /** \details
      Returns the back clip distance from the target of the wrapped viewport object.
    */
    double backClip() const { return m_backClip; }

    /** \details
      Sets the render mode for the wrapped viewport object.
      
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
    void setMode(OdGsView::RenderMode mode) { m_renderMode = mode; }
    
    /** \details
      Sets the render mode for the wrapped viewport object.
      
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
    OdGsView::RenderMode mode() const { return m_renderMode; }

    /** \details
      Hides the wrapped viewport object.
    */
    void hide() { m_bHidden = true; }
    
    /** \details
      Un-hides the wrapped viewport object.
    */
    void show() { m_bHidden = false; }
    
    /** \details
      Checks whether the wrapped viewport object is not hidden.
      \returns
      true if the wrapped viewport is visible, false otherwise.
    */
    bool isVisible() { return !m_bHidden; }

    /** \details
      Sets a multiplier that is used to scale all linetypes in the wrapped viewport object.
      
      \param linetypeScaleMultiplier [in]  Linetype scale multiplier.
    */
    void setLinetypeScaleMultiplier(double linetypeScaleMultiplier) { m_ltpMult1 = linetypeScaleMultiplier; }
    
    /** \details
      Returns current linetype scale multiplier value.
      \sa
      <exref target="https://docs.opendesign.com/td/db_linetype.html">Working with Linetypes</exref>
    */
    double linetypeScaleMultiplier() const { return m_ltpMult1; }
    
    /** \details
      Sets a multiplier that could be used as an alternate scale factor for linetypes in the wrapped viewport object.
      
      \param linetypeAlternateScaleMultiplier [in]  Alternate linetype scale multiplier.
      \sa
      <exref target="https://docs.opendesign.com/td/db_linetype.html">Working with Linetypes</exref>
    */
    void setAlternateLinetypeScaleMultiplier(double linetypeAlternateScaleMultiplier) { m_ltpMult2 = linetypeAlternateScaleMultiplier; }
    
    /** \details
      Returns current alternate linetype scale multiplier value.
      \sa
      <exref target="https://docs.opendesign.com/td/db_linetype.html">Working with Linetypes</exref>
    */
    double linetypeAlternateScaleMultiplier() const { return m_ltpMult2; }

    /** \details
      Returns the ClientViewInfo for the wrapped viewport object.
      
      \param clientViewInfo [out]  Receives the ClientViewInfo.
    */
    void clientViewInfo(OdGsClientViewInfo& clientViewInfo) const { clientViewInfo = m_viewInfo; }
    
    /** \details
      Sets the ClientViewInfo for the wrapped viewport object.
      
      \param clientViewInfo [out]  Receives the ClientViewInfo.
    */
    void setViewInfo(const OdGsClientViewInfo& clientViewInfo) { m_viewInfo = clientViewInfo; }

    /** \details
      Sets background object for the wrapped view.

      \param backgroundId [in]  New background object ID.
      \sa
      <link tv_working_with_attributes_view_backgrounds, Work with View Backgrounds>
    */
    void setBackground(OdDbStub *backgroundId) { m_bkgnd = backgroundId; }
    
    /** \details
      Returns current background object ID for the wrapped view.
      \sa
      <link tv_working_with_attributes_view_backgrounds, Work with View Backgrounds>
    */
    OdDbStub *background() const { return m_bkgnd; }

    /** \details
      Sets visual style object for the wrapped view.

      \param visualStyleId [in]  New visual style object ID.
      \sa
      <link tv_working_with_attributes_visual_styles, Work with Visual Styles>
    */
    void setVisualStyle(OdDbStub *visualStyleId) { m_visualStyle = visualStyleId; }
    
    /** \details
      Returns current visual style object ID for the wrapped view.
      \sa
      <link tv_working_with_attributes_visual_styles, Work with Visual Styles>
    */
    OdDbStub *visualStyle() const { return m_visualStyle; }

    /** \details
      Enable or disable anti-aliasing in current Viewport object.

      \param nLevel [in]  Anti-aliasing mode (set as 3 by default). Set as 0 to disable anti-aliasing.
    */
    void enableAntiAliasing(OdUInt32 nMode) { m_antiAliasingMode = nMode; }

    /** \details
      Returns anti-aliasing mode for current Viewport object.
    */
    OdUInt32 antiAliasingMode() const { return m_antiAliasingMode; }
};

/** \details
    Base template for OdGsDevice class wrappers.

    Corresponding C++ library: TD_Gs

    <group OdGs_Classes> 
*/
template <typename GsDeviceInterface = OdGsDevice, typename RedirectionBase = OdGsDefaultRedirectionBase<OdGsDevice, GsDeviceInterface> >
class OdGsDeviceWrapperMinimalImpl : public OdGsDeviceWrapper<GsDeviceInterface, RedirectionBase>
{
  protected:
    mutable OdSmartPtr<OdGiContext> m_pContext;
    ODCOLORREF m_backColor;
    OdArray<OdGsViewPtr> m_views;
    OdGsDCRectDouble m_size;
    OdArray<ODCOLORREF, OdMemoryAllocator<ODCOLORREF> > m_palette;
  public:
    /** \details
      Constructor for the OdGsDeviceWrapperMinimalImpl class. Sets redirection device and background color for it.
      
      \param pRedirectionDevice [in]  Pointer to the redirection (wrapped) device.
    */
    OdGsDeviceWrapperMinimalImpl(OdGsDevice *pRedirectionDevice = NULL)
      : OdGsDeviceWrapper<GsDeviceInterface, RedirectionBase>(pRedirectionDevice)
      , m_backColor(0)
    {}

    // OdGsDevice wrappers

    /** \details
      Sets the User Context object associated with this Device object.
      \param pUserGiContext [in]  Pointer to the User Context.
    */
    OdGiContext* userGiContext() const { return m_pContext.get(); }
    
    /** \details
      Sets the User Context object associated with this Device object.
      \param pUserGiContext [in]  Pointer to the User Context.
    */
    void setUserGiContext(OdGiContext* pUserGiContext)
    { m_pContext = pUserGiContext;
      for (OdUInt32 nView = 0; nView < m_views.size(); nView++)
        m_views[nView]->setUserGiContext(pUserGiContext);
    }

    /** \details
      Notification function called whenever the size of the GUI window for this Device object has changed.
      
      \param outputRect [in]  New size of the output rectangle in device coordinates (pixels).

      \remarks
      This function should not be called if this Device object has a width and/or height of 0, .
    */
    void onSize(const OdGsDCRect& outputRect) { m_size = OdGsDCRectDouble(outputRect); }
    
    /** \details
      Notification function called whenever the size of the GUI window for this Device object has changed.
      
      \param outputRect [in]  New size of the output rectangle in device coordinates (pixels).

      \remarks
      This function should not be called if this Device object has a width and/or height of 0, .
    */
    void onSize(const OdGsDCRectDouble& outputRect) { m_size = outputRect; }
    
    /** \details
      Returns device surface output rectangle.

      \param outputRect [out]  Obtains size of the output rectangle in device coordinates (pixels).
    */
    void getSize(OdGsDCRect& outputRect) const { outputRect = m_size.round(); }
    
    /** \details
      Returns device surface output rectangle.

      \param outputRect [out]  Obtains size of the output rectangle in device coordinates (pixels).
    */
    void getSize(OdGsDCRectDouble& outputRect) const { outputRect = m_size; }

    /** \details
      Creates a new OdGsView object, and associates it with this Device object.
      
      \param pViewInfo [in]  Pointer to the Client View Information for this Device object.
      \param enableLayerVisibilityPerView [in]  Layer visibility per viewport is supported, if and only if true.
      
      \returns
      Returns a SmartPointer to the newly created object.

      pViewInfo contains information returned by OdGiViewport::acadWindowId() and OdGiViewportDraw::viewportObjectId()
    */
    OdGsViewPtr createView(const OdGsClientViewInfo* pViewInfo = 0, bool /*enableLayerVisibilityPerView*/ = false)
    { OdSmartPtr<OdGsViewWrapperMinimalImpl<> > pView = OdRxObjectImpl<OdGsViewWrapperMinimalImpl<> >::createObject();
      if (pViewInfo) pView->setViewInfo(*pViewInfo);
      pView->setUserGiContext(m_pContext.get());
      pView->setDevice(this); return pView;
    }

    /** \details
      Creates a new OdGsView object, and associates it with this Device object.
      
      \param pViewInfo [in]  Pointer to the Client View Information for this Device object.
      \param enableLayerVisibilityPerView [in]  Layer visibility per viewport is supported, if and only if true.
      
      \returns
      Returns a SmartPointer to the newly created object.

      pViewInfo contains information returned by OdGiViewport::acadWindowId() and OdGiViewportDraw::viewportObjectId()
    */
    void addView(OdGsView* pView) { m_views.push_back(pView); }
    
    /** \details
      Inserts the specified Viewport object to the specified position in this Device object.

      \param viewIndex [in]  View index.
      \param pView [in]  Pointer to the OdGsView object.
    */
    void insertView(int viewIndex, OdGsView* pView) { m_views.insertAt(viewIndex, pView); }
    
    /** \details
      Erases the specified View object.
      \param pView [in]  Pointer to the OdGsView object.

      \returns
      Returns true if and only if successful.
    */
    bool eraseView(OdGsView* pView) { m_views.remove(pView); return true; }
    
    /** \details
      Erases the specified View object.

      \param viewIndex [in]  View index.
      \returns
      Returns true if and only if successful.
    */
    bool eraseView(int viewIndex) { m_views.removeAt(viewIndex); return true; }
    
    /** \details
      Erases all views associated with this Device object.
    */
    void eraseAllViews() { m_views.clear(); }
    
    /** \details
      Returns the number of views associated with this Device object.
    */
    int numViews() const { return m_views.size(); }
    
    /** \details
      Returns the specified OdGsView object associated with this Device object.
      \param viewIndex [in]  View index.
    */
    OdGsView* viewAt(int viewIndex) { return m_views.at(viewIndex); }

    /** \details
      Sets the Background Color of the GUI window of this Device object.
      \param backgroundColor [in]  Background color.
      \returns
      true.
    */
    bool setBackgroundColor(ODCOLORREF backgroundColor) { m_backColor = backgroundColor; return true; }
    
    /** \details
      Returns the Background Color of the GUI window of this Device object.
    */
    ODCOLORREF getBackgroundColor() { return m_backColor; }

    /** \details
      Sets the logical palette to be used by this Device object.
      
      \param logicalPalette [in]  Logical palette.
      \param numColors [in]  Number of colors in palette.
      
      \remarks
      The logical palette is used with calls to OdGiSubEntityTraits::setColor().
    */
    void setLogicalPalette(const ODCOLORREF* logicalPalette, int numColors)
    {
      m_palette.setPhysicalLength(0);
      if (numColors)
        m_palette.insert(m_palette.end(), logicalPalette, logicalPalette + numColors);
    }
    
    /** \details
      Returns the logical palette used by this Device object.

      \param numColors [out]  Number of colors in palette.
    */
    const ODCOLORREF* getLogicalPalette(int &numColors) const
    {
      numColors = (int)m_palette.size();
      return m_palette.getPtr();
    }
};

#include "TD_PackPop.h"

#endif // __GSINTERFACEWRAPPERS_H_INCLUDED_
