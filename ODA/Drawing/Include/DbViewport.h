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

#ifndef _OD_DB_VIEWPORT_
#define _OD_DB_VIEWPORT_

#include "TD_PackPush.h"

#include "DbEntity.h"
#include "DbSymbolTable.h"
#include "ViewportDefs.h"
#include "DbXrefObjectId.h"

#include "Gi/GiViewportTraits.h"
#include "DwgDeclareMembers.h"

class OdGeExtents3d;
class OdGeMatrix3d;
class OdGsView;
class OdDbSun;

/** \details
    This class represents PaperSpace Viewport entities in an OdDbDatabase instance.
  
        <group OdDb_Classes>
*/
class TOOLKIT_EXPORT OdDbViewport : public OdDbEntity
{
protected:
  /* void dxfOutXData(OdDbDxfFiler* pFiler) const;
  */
public:
  DWGMAP_DECLARE_MEMBERS(OdDbViewport);

  OdDbViewport();

  /** \details
    Returns the height of this Viewport object (DXF 41).

    \remarks
    If this Viewport object is an overall viewport,
    the values returned by width() and height() must be divided by a
    factor of 1.058, and the parameters of setWidth and setHeight() 
    must be multiplied by a like factor.                        
  */
  double height() const;

  /** \details
    Sets the height of this Viewport object (DXF 41).
    \param height [in]  Height.

    \remarks
    If this Viewport object is an overall viewport,
    the values returned by width() and height() must be divided by a
    factor of 1.058, and the parameters of setWidth and setHeight() 
    must be multiplied by a like factor.                        
  */
  void setHeight(
    double height);

  /** \details
    Returns the width of this Viewport object (DXF 40).

    \remarks
    If this Viewport object is an overall viewport,
    the values returned by width() and height() must be divided by a
    factor of 1.058, and the parameters of setWidth and setHeight() 
    must be multiplied by a like factor.                        
  */
  double width() const;

  /** \details
    Sets the width of this Viewport object (DXF 40).
    \param width [in]  Width.

    \remarks
    If this Viewport object is an overall viewport,
    the values returned by width() and height() must be divided by a
    factor of 1.058, and the parameters of setWidth and setHeight() 
    must be multiplied by a like factor.                        
  */
  void setWidth(
    double width);

  /** \details
    Returns the WCS center point of this Viewport object (DXF 10).
  */
  OdGePoint3d centerPoint() const;

  /** \details
    Sets the WCS center point of this Viewport object (DXF 10).
    \param centerPoint [in]  Center point.
  */
  void setCenterPoint(
    const OdGePoint3d& centerPoint);

  /** \details
    Returns the ID number of this Viewport object.
    \returns
    Returns -1 if this Viewport object is inactive.
  */
  OdInt16 number() const;

  /** \details
    Returns true if and only if this Viewport object is on (DXF 90, bit 0x20000).
  */
  bool isOn() const;

  /** \details
    Turns on this Viewport object (DXF 90, bit 0x20000, DXF 68 non-zero).
  */
  void setOn();

  /** \details
    Turns off this Viewport object (DXF 90, bit 0x20000, DXF 68 zero).
  */
  void setOff();

  /** \details
    Returns the WCS view target of this Viewport object (DXF 17).
  */
  OdGePoint3d viewTarget() const;

  /** \details
    Sets the WCS view target of this Viewport object (DXF 17).
    \param viewTarget [in]  View target.
  */
  void setViewTarget(
    const OdGePoint3d& viewTarget);

  /** \details
    Returns the WCS view direction of this Viewport object (DXF 16).
  */
  OdGeVector3d viewDirection() const;

  /** \details
    Sets the WCS view direction of this Viewport object (DXF 16).
    \param viewDirection [in]  View direction.
  */
  void setViewDirection(
    const OdGeVector3d& viewDirection);

  /** \details
    Returns the DCS view height of this Viewport object (DXF 45).
  */
  double viewHeight() const;

  /** \details
    Sets the DCS view height of this Viewport object (DXF 45).
    \param viewHeight [in]  View height.
  */
  void setViewHeight(
    double viewHeight);

  /** \details
    Returns the DCS view center of this Viewport object (DXF 12).
  */
  OdGePoint2d viewCenter() const;

  /** \details
    Sets the DCS view center of this Viewport object (DXF 12).
    \param viewCenter [in]  View center.
  */
  void setViewCenter(
    const OdGePoint2d& viewCenter);

  /** \details
    Returns the DCS twist angle of this Viewport object (DXF 51).
    
    \remarks
    All angles are expressed in radians.
  */
  double twistAngle() const;

  /** \details
    Sets the DCS twist angle of this Viewport object (DXF 51).
    \param twistAngle [in]  Twist angle.
    \remarks
    All angles are expressed in radians.
  */
  void setTwistAngle(
    double twistAngle);

  /** \details
    Returns the perspective mode lens length (in mm) of this Viewport object (DXF 42).
  */
  double lensLength() const;

  /** \details
    Sets the perspective mode lens length of this Viewport object (DXF 42).
    \param lensLength [in]  Lens length (mm).
  */
  void setLensLength(
    double lensLength);

  /** \details
    Returns true if and only if front clipping is on for this Viewport object (DXF 90, bit 0x02).
  */
  bool isFrontClipOn() const;

  /** \details
    Turns on front clipping for this Viewport object (DXF 90, bit 0x02).
  */
  void setFrontClipOn();

  /** \details
    Turns off front clipping for this Viewport object (DXF 90, bit 0x02).
  */
  void setFrontClipOff();

  /** \details
    Returns true if and only if back clipping is on for this Viewport object (DXF 90, bit 0x04).
  */
  bool isBackClipOn() const;

  /** \details
    Turns on back clipping for this Viewport object (DXF 90, bit 0x04).
  */
  void setBackClipOn();

  /** \details
    Turns off back clipping for this Viewport object (DXF 90, bit 0x04).
  */
  void setBackClipOff();

  /** \details
    Returns true if and only if the front clipping plane passes through the camera (DXF 90, bit 0x10).
    \remarks
    If true, the front clipping plane ignores the front clip distance.
  */
  bool isFrontClipAtEyeOn() const;

  /** \details
    Sets the front clipping plane to pass through the camera (DXF 90, bit 0x10).
    \remarks
    The front clipping plane ignores the front clip distance.
  */
  void setFrontClipAtEyeOn();

  /** \details
    Sets the front clipping plane to utilize the front clip distance (DXF 90, bit 0x10).
  */
  void setFrontClipAtEyeOff();

  /** \details
      Returns the front clip distance of this Viewport object (DXF 43).
  */
  double frontClipDistance() const;

  /** \details
    Sets the front clip distance of this Viewport object (DXF 43).
    \param frontClipDistance [in]  Front clip distance.
  */
  void setFrontClipDistance(
    double frontClipDistance);

  /** \details
    Returns the back clip distance of this Viewport object (DXF 44).
  */
  double backClipDistance() const;

  /** \details
    Sets the back distance of this Viewport object (DXF 44).
    \param backClipDistance [in]  Back clip distance.
  */
  void setBackClipDistance(
    double backClipDistance);

  /** \details
     Returns true if and only if perspective is on for this Viewport object (DXF 90, bit 0x01).
  */
  bool isPerspectiveOn() const;

  /** \details
    Sets perspective on for this Viewport object (DXF 90, bit 0x01).
  */
  void setPerspectiveOn();

  /** \details
    Sets perspective off for this Viewport object (DXF 90, bit 0x01).
  */
  void setPerspectiveOff();

  /** \details
    Returns true if and only if UCS follow mode is on for this Viewport object (DXF 90, bit 0x08).
  */
  bool isUcsFollowModeOn() const;

  /** \details
    Sets UCS follow mode on for this Viewport object (DXF 90, bit 0x08).
  */
  void setUcsFollowModeOn();

  /** \details
    Sets UCS follow mode off for this Viewport object (DXF 90, bit 0x08).
  */
  void setUcsFollowModeOff();

  /** \details
    Returns true if and only if the UCS icon is visible for this Viewport object (DXF 90, bit 0x20).
  */
  bool isUcsIconVisible() const;

  /** \details
    Sets UCS icon visible on for this Viewport object (DXF 90, bit 0x20).
  */
  void setUcsIconVisible();

  /** \details
    Sets UCS icon visible false for this Viewport object (DXF 90, bit 0x20).
  */
  void setUcsIconInvisible();

  /** \details
    Returns true if and only if the UCS icon is at the UCS origin for this Viewport object (DXF 90, bit 0x40).
  */
  bool isUcsIconAtOrigin() const;

  /** \details
    Sets the UCS icon to the UCS origin for this Viewport object (DXF 90, bit 0x40).
  */
  void setUcsIconAtOrigin();

  /** \details
    Sets the UCS icon to the corner of this Viewport object (DXF 90, bit 0x40).
  */
  void setUcsIconAtCorner();

  /** \details
    Returns true if and only if fast zooms are on for this Viewport object (DXF 90, bit 0x80).
  */
  bool isFastZoomOn() const;

  /** \details
    Sets fast zooms on for this Viewport object (DXF 90, bit 0x80).
  */
  void setFastZoomOn();

  /** \details
    Sets fast zooms off for this Viewport object (DXF 90, bit 0x80).
  */
  void setFastZoomOff();

  /** \details
    Returns the circle zoom percent of this Viewport object (DXF 72).
    \remarks
    circleSides() has a range of [1..20000]
  */
  OdUInt16 circleSides() const;

  /** \details
    Sets the circle zoom percent of this Viewport object (DXF 72).
    \param circleSides [in]  circle zoom percent [1,20000].
  */
  void setCircleSides(
    OdUInt16 circleSides);

  /** \details
    Returns true if and only if the snap mode is on for this Viewport object (DXF 90, bit 0x100).
  */
  bool isSnapOn() const;

  /** \details
    Sets the snap mode on for this Viewport object (DXF 90, bit 0x100).
  */
  void setSnapOn();

  /** \details
    Sets the snap mode off for this Viewport object (DXF 90, bit 0x100).
  */
  void setSnapOff();

  /** \details
    Returns true if and only if isometric snap style is on for this Viewport object (DXF 90, bit 0x400).
  */
  bool isSnapIsometric() const;

  /** \details
    Sets the isometric snap style on for this Viewport object (DXF 90, bit 0x400).
  */
  void setSnapIsometric();

  /** \details
    Sets the isometric snap style off for this Viewport object (DXF 90, bit 0x400).
  */
  void setSnapStandard();

  /** \details
    Returns the UCS snap angle of this Viewport object (DXF 50).
  */
  double snapAngle() const;

  /** \details
    Sets the UCS snap angle of this Viewport object (DXF 50).
    \param snapAngle [in]  Snap angle.
    \remarks
    All angles are expressed in radians.
  */
  void setSnapAngle(
    double snapAngle);

  /** \details
    Returns the UCS snap base point of this Viewport object (DXF 13).
  */
  OdGePoint2d snapBasePoint() const;

  /** \details
    Sets the UCS snap base point of this Viewport object (DXF 13).
    \param snapBasePoint [in]  Snap base point.
  */
  void setSnapBasePoint(
    const OdGePoint2d& snapBasePoint); 
  
  /** \details
    Returns the snap increment of this Viewport object (DXF 14).
  */
  OdGeVector2d snapIncrement() const;

  /** \details
    Sets the snap increment of this Viewport object (DXF 14).
    \param snapIncrement [in]  Snap increment.
  */
  void setSnapIncrement(
    const OdGeVector2d& snapIncrement);
  
  /** \details
    Returns the snap IsoPair of this Viewport object (DXF 14).
    \remarks
    snapIsoPair() returns one of the following:
    
    <table>
    Value    Description
    0        Left isoplane
    1        Top isoplane
    2        Right isoplane
    </table>
  */
  OdUInt16 snapIsoPair() const;

  /** \details
    Sets the snap IsoPair of this Viewport object (DXF 14).
    \param snapIsoPair [in]  Snap IsoPair.
    \remarks
    snapIsoPair must be one of the following:
    
    <table>
    Value    Description
    0        Left isoplane
    1        Top isoplane
    2        Right isoplane
    </table>
  */
  void setSnapIsoPair(
    OdUInt16 snapIsoPair);
  
  /** \details
    Returns true if and only if the grid is on for this Viewport object (DXF 90, bit 0x200).
  */
  bool isGridOn() const;

  /** \details
    Sets the grid on for this Viewport object (DXF 90, bit 0x200).
  */
  void setGridOn();

  /** \details
    Sets the grid off for this Viewport object (DXF 90, bit 0x200).
  */
  void setGridOff();
  
  /** \details
    Returns the grid increment of this Viewport object (DXF 15).
  */
  OdGeVector2d gridIncrement() const;

  /** \details
    Sets the grid increment of this Viewport object (DXF 15).
    \param gridIncrement [in]  Grid increment.
  */
  void setGridIncrement(
    const OdGeVector2d& gridIncrement);
  
  /** \details
    Returns true if and only if this Viewport object will have hidden lines removed during plotting. (DXF 90, bit 0x800).
  */
  bool hiddenLinesRemoved() const;

  /** \details
    Sets this Viewport object to have hidden shown lines during plotting. (DXF 90, bit 0x800).  
  */
  void showHiddenLines();

  /** \details
    Sets this Viewport object to have hidden lines removed during plotting. (DXF 90, bit 0x800).  
  */
  void removeHiddenLines();
  
  /** \details
    Freezes the specified layers in this Viewport object (DXF 341)
    
    \param layerIds [in]  Object IDs of the layers to be frozen.

    \remarks
    Other viewports are unaffected by this function.
  */
  void freezeLayersInViewport(
    const OdDbObjectIdArray& layerIds);

  /** \details
    Thaws the specified layers in this Viewport object (DXF 341)
    
    \param layerIds [in]  Object IDs of the layers to be thawed.

    \remarks
    Other viewports are unaffected by this function.
  */
  void thawLayersInViewport(
    const OdDbObjectIdArray& layerIds);

  /** \details
      Thaws all layers in this Viewport object.
  */
  void thawAllLayersInViewport();

  /** \details
    Returns true and only if the specified layer is frozen in this Viewport object.
    \param layerId [in]  Layer ID of the layer to be queried.
  */
  bool isLayerFrozenInViewport(
    const OdDbObjectId& layerId) const;

  /** \details
    Returns all layers that are frozen in this Viewport object.
    \param layerIds [out]  Receives the Object IDs of the frozen layers.
  */
  void getFrozenLayerList(
    OdDbObjectIdArray& layerIds) const;
  
  /** \details
     Updates the display to reflect changes in this Viewport object.
     
     \remarks
     Closing this Viewport object automatically calls this function. 
     
     Throws:
     eNotImplemented
  */
  void updateDisplay() const;
  
  
  virtual OdUInt32 subSetAttributes(
    OdGiDrawableTraits* pTraits) const;

  virtual bool subWorldDraw(
    OdGiWorldDraw* pWd) const;
  
  /** \details
    Returns true if and only if scale factor of this Viewport object is locked (DXF 90, bit 0x4000).
  */
  bool isLocked() const;

  /** \details
    Locks the scale factor of this Viewport object (DXF 90, bit 0x4000).
  */
  void setLocked();

  /** \details
    Unlocks the scale factor of this Viewport object (DXF 90, bit 0x4000).
  */
  void setUnlocked();
  
  /** \details
    Not implemented. The value returned is not saved to file. Always returns true.
  */
  bool isTransparent() const;

  /** \details
    Not implemented. The set value is not saved to file.
  */
  void setTransparent();

  /** \details
    Not implemented. The set value is not saved to file.
  */
  void setOpaque();
  
  enum StandardScaleType
  {
    kScaleToFit,    // Scaled to Fit
    kCustomScale,   // Scale is not a standard scale
    k1_1,           // 1:1
    k1_2,           // 1:2
    k1_4,           // 1:4
    k1_5,           // 1:5
    k1_8,           // 1:8
    k1_10,          // 1:10
    k1_16,          // 1:16
    k1_20,          // 1:20
    k1_30,          // 1:30
    k1_40,          // 1:40
    k1_50,          // 1:50
    k1_100,         // 1:100
    k2_1,           // 2:1
    k4_1,           // 4:1
    k8_1,           // 8:1
    k10_1,          // 10:1
    k100_1,         // 100:1
    k1_128in_1ft,   // 1/128"= 1'
    k1_64in_1ft,    // 1/64"= 1'
    k1_32in_1ft,    // 1/32"= 1'
    k1_16in_1ft,    // 1/16"= 1'
    k3_32in_1ft,    // 3/32"= 1'
    k1_8in_1ft,     // 1/8" = 1'
    k3_16in_1ft,    // 3/16"= 1'
    k1_4in_1ft,     // 1/4" = 1'
    k3_8in_1ft,     // 3/8" = 1'
    k1_2in_1ft,     // 1/2" = 1'
    k3_4in_1ft,     // 3/4" = 1'
    k1in_1ft,       // 1" = 1'
    k1and1_2in_1ft, // 1 1/2"= 1'
    k3in_1ft,       // 3" = 1'
    k6in_1ft,       // 6" = 1'
    k1ft_1ft        // 1' = 1'
  };
  
  enum ShadePlotType
  {
      kAsDisplayed  = 0,     // As displayed
      kWireframe    = 1,     // Wireframe
      kHidden       = 2,     // Hidden
      kRendered     = 3,     // Rendered
      kVisualStyle  = 4,     // Visual style
      kRenderPreset = 5      // Render preset
  };

  /** \details
    Returns the custom scale of this Viewport object.
  */
  double customScale() const;

  /** \details
    Sets the custom scale of this Viewport object.
    \param customScale [in]  Custom scale.
  */
  void setCustomScale(
    double customScale);
  
  /** \details
    Returns the standard scale type of this Viewport object/
  */
  StandardScaleType standardScale() const;

  /** \details
    Sets the standard scale type of this Viewport object.
    \param standardScale [in]  Standard scale type.
  */
  void setStandardScale(
    const StandardScaleType standardScale);
  
  /** \details
    Returns the name of the plot style sheet applied to objects in this Viewport object (DXF 1).
  */
  OdString plotStyleSheet() const;

  /** \details
    Returns the plot style sheet name associated with this Viewport object.
  */
  OdString effectivePlotStyleSheet() const;

  /** \details
   Sets the plot style sheet name associated with this Viewport object.
  */
  void setPlotStyleSheet(
    const OdString& plotStyleSheetName);
  
  /** \details
    Returns true if and only if non-rectangular clipping is enabled for this Viewport object (DXF 90, bit 0x10000).
  */
  bool isNonRectClipOn() const;

  /** \details
    Sets non-rectangular clipping on for this Viewport object (DXF 90, bit 0x10000).
  */
  void setNonRectClipOn();

  /** \details
    Sets non-rectangular clipping off for this Viewport object (DXF 90, bit 0x10000).
  */
  void setNonRectClipOff();
  
  /** \details
    Returns the Object ID of the clipping entity for this Viewport object (DXF 340).
  */
  OdDbObjectId nonRectClipEntityId() const;

  /** \details
    Sets the Object ID of the clipping entity for this Viewport object (DXF 340).
    
    \param clipEntityId [in]  Object ID of the clipping entity.
    
    \remarks
    The following entity types are acceptable clipping entities:

    @untitled table
    OdDb2dPolyline
    OdDb3dPolyline
    OdDbCircle
    OdDbEllipse
    OdDbFace
    OdDbPolyline
    OdDbRegion
    OdDbSpline
    
    \remarks
    A clipping entity must be in the same PaperSpace as this Viewport.
  */
  void setNonRectClipEntityId(
    OdDbObjectId clipEntityId);
  
  /*
     virtual void erased(const OdDbObject* , bool);
     virtual void modified(const OdDbObject *);
     virtual void copied(const OdDbObject* pDbObj,const OdDbObject* pNewObj);
     virtual void subObjModified(const OdDbObject* pDbObj, const OdDbObject* pSubObj);
  */
  
  /** \details
    Returns the origin, X-axis, and Y-Axis of the UCS associated with this Viewport object.

    \param origin [out]  Receives the UCS origin (DXF 110).
    \param xAxis [out]  Receives the UCS X-axis (DXF 111).
    \param yAxis [out]  Receives the UCS Y-axis (DXF 112).
  */
  void getUcs(OdGePoint3d& origin, 
    OdGeVector3d& xAxis, 
    OdGeVector3d& yAxis) const;

  /** \details
    True if and only if the UCS associated with this Viewport object
    is orthographic with respect to UCSBASE (DXF 79).

    \param viewType [out]  Receives the orthographic view type.
    \param pDb [in]  Working database. If object is a database resident this parameter is ignored.

    \returns
    Returns the type of orthographic view.
    \remarks
    viewType must be one of the following:
    
    <table>
    Name                  Value   View type
    OdDb::kNonOrthoView   0       Non-orthographic with respect to the UCS 
    OdDb::kTopView        1       Top view with respect to the UCS 
    OdDb::kBottomView     2       Bottom view with respect to the UCS 
    OdDb::kFrontView      3       Front view with respect to the UCS 
    OdDb::kBackView       4       Back view with respect to the UCS 
    OdDb::kLeftView       5       Left view with respect to the UCS 
    OdDb::kRightView      6       Right view with respect to the UCS 
    </table>
  */
  bool isUcsOrthographic(
    OdDb::OrthographicView& viewType,
    const OdDbDatabase* pDb = 0) const;

  /** \details
    Returns the Object ID of the UCS associated with this Viewport object (DXF 345 or 346).
  */
  OdDbObjectId ucsName() const;

  /** \details
    Returns the elevation of the UCS plane of this entity (DXF 146).
    
    \remarks
    The elevation is the distance from the WCS origin to the UCS plane of this entity.
  */
  double elevation() const;
  
  /** \details
    Sets the UCS associated with this Viewport object 

    \param origin [in]  The WCS origin of the UCS (DXF 110).
    \param xAxis [in]  The WCS X-axis of the UCS (DXF 111).
    \param yAxis [in]  The WCS Y-axis of the UCS(DXF 112).
  */
  void setUcs(
    const OdGePoint3d& origin, 
    const OdGeVector3d& xAxis, 
    const OdGeVector3d& yAxis);

  /** \details
    Sets the UCS associated with this Viewport object 

    \param viewType [in]  Orthographic view type (DXF 79).
    \param pDb [in]  Working database. If object is a database resident this parameter is ignored.

    \remarks
    viewType must be one of the following:
    
    <table>
    Name            Value   View type
    OdDb::kNonOrthoView   0       Non-orthographic with respect to the UCS 
    OdDb::kTopView        1       Top view with respect to the UCS 
    OdDb::kBottomView     2       Bottom view with respect to the UCS 
    OdDb::kFrontView      3       Front view with respect to the UCS 
    OdDb::kBackView       4       Back view with respect to the UCS 
    OdDb::kLeftView       5       Left view with respect to the UCS 
    OdDb::kRightView      6       Right view with respect to the UCS 
    </table>
  */
  void setUcs(
    OdDb::OrthographicView viewType, const OdDbDatabase* pDb = 0);

  /** \details
    Sets the UCS associated with this Viewport object 

    \param ucsId [in]  Object ID of the UCS (DXF 345 or 346).
  */
  void setUcs(
    const OdDbObjectId& ucsId);

  /** \details
    Sets the UCS associated with this Viewport object to the WCS. 
  */
  void setUcsToWorld();

  /** \details
    Sets the elevation of the UCS plane of this Viewport object (DXF 146).
    \param elevation [in]  Elevation.
    \remarks
    The elevation is the distance from the WCS origin to the plane of this Viewport object.
  */
  void setElevation(
    double elevation);
  
  /*
  bool isViewOrthographic(OdDb::OrthographicView& view) const;

  void setViewDirection(OdDb::OrthographicView view);
  */
  
  /** \details
    Returns true if and only if the UCS that is associated with this Viewport object will become active
    with the activation of this Viewport object.
  */
  bool isUcsSavedWithViewport() const;

  /** \details
    Controls the activation of the UCS that is associated with this Viewport object 
    with the activation of this Viewport object.
    
    \param ucsPerViewport [in]  Controls activation of the UCS.
  */
  void setUcsPerViewport(
    bool ucsPerViewport);
  
  /** \details
    Set the render mode of this Viewport object (DXF 281).
    \param renderMode [in]  Render mode.
    \remarks
    renderMode must be one of the following:
    
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
  void setRenderMode(
    OdDb::RenderMode renderMode);

  /** \details
    Returns the render mode of this Viewport object (DXF 281).

    \remarks
    renderMode() returns one of the following:
    
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
  OdDb::RenderMode renderMode() const;

  /** \details
    Retrieves the shade plot mode for this viewport. 
  */
  ShadePlotType shadePlot() const;

  /** \details
    Set the shade plot type of this viewport object (DXF 281).
    \param shadePlot [in]  Shade plot type.
    
    \remarks
    shadePlot determines how this viewport object will plot.
  */
  void setShadePlot(
    const ShadePlotType shadePlot);

  // New in R21 functions

  /** \details
    Retrieves the shade object (visual style or render preset) for the plotting 
    associated with this viewport. 
  */
  OdDbObjectId shadePlotId() const;
  
 /** \details
    Sets the shade plot type of this viewport object (DXF 281).
    
    \param type [in]  Shade plot type.
    
    \remarks
    type determines how this viewport object will plot.
  */
  void         setShadePlot(const ShadePlotType type, const OdDbObjectId& shadePlotId);

  /** \details
    Checks whether the viewport will plot vector output based on the viewport's current shade plot settings. 
    
    \returns
    true if at least one of the following conditions is true:
    * Render mode is kWireframe or k2DOptimized.
    * Visual style type is k2DWireframe or k3DWireframe.
    * Visual style has an OdGiFaceStyle::kInvisible lighting model for faces and an OdGiEdgeStyle::kIsolines edge model for edges.
  */
  bool         plotWireframe() const;
  
  /** \details
    Checks whether the viewport will plot raster output based on the viewport's current shade plot settings. 
    
    \returns
    true if all of the following conditions are false:
    * Render mode is kWireframe or k2DOptimized.
    * Visual style type is k2DWireframe or k3DWireframe.
    * Visual style has an OdGiFaceStyle::kInvisible lighting model for faces and an OdGiEdgeStyle::kIsolines edge model for edges.
  */
  bool         plotAsRaster() const;

  /** \details
    Retrieves an identifier of the view background for this viewport.
  */
  OdDbObjectId background() const;
  
  /** \details
    Sets the view background for this viewport.
    
    \param backgroundId [in]  Identifier of the background object.
  */
  void         setBackground(const OdDbObjectId& backgroundId);

  // Visual Styles

  /** \details
    Retrieves an identifier of the visual style object that is used for this viewport.
  */
  OdDbObjectId visualStyle() const;
  
  /** \details
    Sets the visual style for this viewport.
    
    \param visualStyle [in]  Identifier of the visual style object.
  */
  void         setVisualStyle(const OdDbObjectId& visualStyle);

  // Viewport Lighting

  /** \details
    Checks whether default lighting is enabled for this viewport.
    
    \returns
    Returns true if default lighting is turned on, false otherwise.
  */
  bool                isDefaultLightingOn() const;
  
  /** \details
    Enables or disables default lighting for this viewport.
    
    \param on [in]  Flag that switches on or off default lighting.
  */
  void                setDefaultLightingOn(bool on);
  
  /** \details
    Retrieves the default lighting type for this viewport.
  */
  OdGiViewportTraits::DefaultLightingType defaultLightingType() const;
  
  /** \details
    Sets the default lighting type for this viewport.
    
    \param typ [in]  Type of default lighting.
  */
  void                setDefaultLightingType(OdGiViewportTraits::DefaultLightingType typ);

  // Brightness controls the relative intensity of lights.

  /** \details
    Retrieves the brightness value which affects the relative intensity of lights in this viewport.
  */
  double              brightness() const;
  
  /** \details
    Sets the brightness value which affects the relative intensity of lights in this viewport.
    
    \param brightness [in]  Viewport lighting brightness.
  */
  void                setBrightness(double);

  // Contrast controls intensity of ambient light, relative to other lights.

  /** \details
    Retrieves the contrast value which affects the intensity of ambient lights on other lights in this viewport.
  */
  double              contrast() const;
  
  /** \details
    Sets the contrast value which affects the intensity of ambient lights on other lights in this viewport.
    
    \param contrast [in]  Viewport lighting contrast.
  */
  void                setContrast(double);

  /** \details
    Retrieves the color of ambient light in this viewport.
  */
  OdCmColor           ambientLightColor() const;
  
  /** \details
    Sets the color of ambient light in this viewport.
    
    \param clr [in]  Ambient light color.
  */
  void                setAmbientLightColor(const OdCmColor& clr);

  // A single sun (distant light) can be associated with each viewport.

  /** \details
    Retrieves the object identifier of the sun (distant light) associated with this viewport.
  */
  OdDbObjectId        sunId() const;
  
  /** \details
    Sets the sun (distant light) for this viewport.
    
    \param pSun [in]  Pointer to a sun object.
    \returns
    Object identifier of the sun in the database.
  */
  OdDbObjectId        setSun(OdDbSun* pSun);

  /** \details
    Retrieves tone parameters for this viewport.
    \param params [out]   Receives tone operator parameters.
  */
  void toneOperatorParameters(OdGiToneOperatorParameters &params) const;
  
  /** \details
    Sets the tone parameters for this viewport.
    \param params [in]   Tone operator parameters.
  */
  void setToneOperatorParameters(const OdGiToneOperatorParameters &params);

  /** \details
    Sets the annotation scale object for this viewport.
    
    \param pScaleObj [in]   Pointer to an object that affects the annotation scale in this viewport.
  */
  OdResult setAnnotationScale(const OdDbAnnotationScale* pScaleObj);
  
  /** \details
    Retrieves a smart pointer to the annotation scale object of this viewport.
  */
  OdDbAnnotationScalePtr annotationScale() const;
  
  /** \details
    Checks whether a grid is diplayed only inside the limits for the WCS and UCS in this viewport.
    \returns
    true if the grid is displayed only within the limits, false otherwise.
  */
  bool isGridBoundToLimits() const;
  
  /** \details
    Specifies whether to display a grid only inside the limits for the WCS and UCS in this viewport.
    
    \param bNewVal [in]   Flag that disables (if set to true) or enables (if set to false) grid display beyond the limits.
  */
  void setGridBoundToLimits(bool bNewVal);

  /** \details
    Checks whether the grid adapts to display fewer or more lines in the viewport than the value specified by the GRIDUNIT setting.
    An adaptive grid can be useful to avoid issues when a viewport is zoomed out too much and the grid becomes too dense, or on the contrary,
    when a viewport is zoomed in too much, there can be too few lines to display the grid.
    \returns
    true if the grid is adaptive, false otherwise.
  */
  bool isGridAdaptive() const;
  
  /** \details
    Specifies whether to adapt the grid display. An adaptive grid can be useful to avoid issues when a viewport is zoomed out too much 
    and the grid becomes too dense, or on the contrary, when a viewport is zoomed in too much, there can be too few lines to display the grid.
    
    \param bNewVal [in]   Flag that specifies whether the grid should be adaptive.
  */
  void setGridAdaptive(bool bNewVal);

  /** \details
    Checks whether grid subdivision is not allowed below the minor grid spacing in this viewport. 
    \returns
    true if the grid subdivision is restricted, false otherwise.
    \remarks
    The grid subdivision setting is ignored if an isGridAdaptive() call returns false.
  */
  bool isGridSubdivisionRestricted() const;
  
  /** \details
    Specifies whether to restrict grid subdivision below the minor grid spacing in this viewport.
    
    \param bNewVal [in]   Flag that specifies whether to restrict grid subdivision.
    \remarks
    The grid subdivision setting is ignored if an isGridAdaptive() call returns false.
  */
  void setGridSubdivisionRestricted(bool bNewVal);

  /** \details
    Checks whether the grid responds to dynamic UCS changes in this viewport. 
    \returns
    true if the grid follows UCS changes, false otherwise.
  */
  bool isGridFollow() const;
  
  /** \details
    Specifies whether the grid should be redrawn according to dynamic UCS changes in this viewport. 
    
    \param bNewVal [in]   Flag that specifies whether to redraw the grid for dynamic UCS changes.
  */
  void setGridFollow(bool bNewVal);

  /** \details
    Retrieves the number of minor grid lines between each major grid line.
  */
  OdUInt16 gridMajor() const;
  
  /** \details
    Specifies the number of minor grid lines between each major grid line.
    For example, if the passed value is 10, a major grid line is displayed at
    every 10-th minor grid line.
    
    \param nGridLines [in]   Number of minor grid lines between each major grid line in the range [1..100].
  */
  void setGridMajor(OdUInt16);

  // View association methods

  /** \details
    Associates a model view (defined by the passed external reference 'objId') with this viewport.
    
    \param objId [in]   External reference object identifier that corresponds to the model view in the exterally referenced database.
    \remarks
    This method can throw the following errors:
    * eInvalidObjectId - The 'objId' is not a valid OdDbObjectId in this database.
    * eInvalidXrefObjectId - The 'objId' is not a valid OdDbXrefObjectId.
    * eUnableToSetViewAssociation - Application cannot associate the view with this viewport.
  */
  void setModelView(const OdDbXrefObjectId &objId);
  
  /** \details
    Retrieves the exterally referenced object identifier of the model view associated with this viewport.
    \remarks
    This method can throw the 'eUnableToGetViewAssociation' error if it fails to get the view association.
  */
  OdDbXrefObjectId getModelView() const;
  
  /** \details
    Removes the model view association from this viewport.
  */
  void removeModelView();
  
  /** \details
    Updates (syncronizes) parameters of the OdDbViewport object with parameters in the associated view.
    \returns
    Returns one of the following values that represents a result of method execution:
    * eOk - Updated successfully.
    * eUnableToSyncModelView - Unable to get the model view, or its resolved external reference ID (resolved to OdDbObjectId) is not valid.
    * eInvalidView - Smart pointer to the view table record of the resolved ID is NULL.
  */
  OdResult syncModelView();

  /** \details
    Associates a sheet view with this viewport.
    
    \param objId [in]   Object ID of the sheet view to be associated with this viewport.
    This method can the eInvalidObjectId error if one of the following cases is true:
    * The passed 'objId' is not valid.
    * Smart pointer to the view table record of the 'objId' is NULL.
  */
  void setSheetView(const OdDbObjectId &objId);
  
  /** \details
    Retrieves an object identifier of a sheet view associated with this viewport.
    \remarks
    This method can throw the eUnableToGetViewAssociation error if it fails to get the associated view.
  */
  OdDbObjectId getSheetView() const;
  
  /** \details
    Removes the sheet view association from this viewport.
  */
  void removeSheetView();

  /** \details
    Associates a label block (OdDbBlockReference) with this viewport.
    
    \param objId [in]   Object ID of the label block to be associated with this viewport.
  */
  void setLabelBlock(const OdDbObjectId objId);
  
  /** \details
    Retrieves an object identifier of the label block (OdDbBlockReference) associated with this viewport.
    \remarks
    This method can throw the 'eUnableToGetViewAssociation' error if it fails to get the view association.
  */
  OdDbObjectId getLabelBlock() const;
  
  /** \details
    Removes the label block (OdDbBlockReference) associated with this viewport.
  */
  void removeLabelBlock();

  // OdGsView association methods

  /** \details
    Retrieves a pointer to a GS view associated with this viewport.
  */
  OdGsView* gsView() const;
  
  /** \details
    Associates a GS view with this viewport.
    
    \param view [in]   Pointer to a GS view to be associated with this viewport.
  */
  void setGsView(OdGsView*);

  // OdDbEntity overrides

  /** \details
    Called by setDatabaseDefaults() after the values are set.

    \param pDb [in]  Pointer to the database whose default values are to be used.
    \param doSubents [in]  Set to true if subentities should be processed.

    \remarks
    If pDb is null, the database containing this entity is used.

    This function allows custom classes to inspect and modify the values set by setDatabaseDefaults.
  */
  virtual void subSetDatabaseDefaults(
    OdDbDatabase* pDb, 
    bool doSubents);

  /** \details
    Reads the .dwg file data of this object. 
    
    \param pFiler [in] Pointer to a filer object from which to read the data.
    \returns
    Returns the filer status.
    \remarks
    This function is called by dwgIn() to allow the object to read its data. 
    When overriding this function: 
    <ol>
    1. Call assertWriteEnabled(). 
    2. Call the parent class's dwgInFields(pFiler). 
    3. If it returns eOK, continue; otherwise return whatever the parent's dwgInFields(pFiler) returned. 
    4. Call the OdDbDwgFiler(pFiler) methods to read each of the object's data items in the order they were written. 
    5. Return pFiler->filerStatus().
    </ol>
  */
  virtual OdResult dwgInFields(
    OdDbDwgFiler* pFiler);

  /** \details
    Writes the .dwg file data of this object. 
    
    \param pFiler [out] Pointer to a filer object to which to write the data.
    \returns
    Returns the filer status.
    \remarks
    This function is called by dwgOut() to allow the object to write its data. 
    When overriding this function: 
    <ol>
    1. Call assertReadEnabled(). 
    2. Call the parent class's dwgOutFields(pFiler). 
    3. Call the OdDbDwgFiler(pFiler) methods to write each of the object's data items in the order they were written. 
    </ol>
  */
  virtual void dwgOutFields(
    OdDbDwgFiler* pFiler) const;

  /** \details
    Reads the .dxf file data of this object. 
    
    \param pFiler [in] Pointer to a filer object from which to read the data.
    \returns
    Returns the filer status.
    \remarks
    This function is called by dxfIn() to allow the object to read its data. 
    When overriding this function: 
    <ol>
    1. Call assertWriteEnabled(). 
    2. Call the parent class's dwgInFields(pFiler). 
    3. If it returns eOK, continue; otherwise return whatever the parent's dxfInFields(pFiler) returned. 
    4. Call the OdDbDxfFiler(pFiler) methods to read each of the object's data items in the order they were written. 
    5. Return pFiler->filerStatus().
    </ol>
  */
  virtual OdResult dxfInFields(
    OdDbDxfFiler* pFiler);

  /** \details
    Writes the .dxf file data of this object. 
    
    \param pFiler [out] Pointer to a filer object to which to write the data.
    \returns
    Returns the filer status.
    \remarks
    This function is called by dxfOut() to allow the object to write its data. 
    When overriding this function: 
    <ol>
    1. Call assertReadEnabled(). 
    2. Call the parent class's dxfOutFields(pFiler). 
    3. Call the OdDbDxfFiler(pFiler) methods to write each of the object's data items in the order they were written. 
    </ol>
  */
  virtual void dxfOutFields(
    OdDbDxfFiler* pFiler) const;

  /** \details
    Reads the .dxf file data specific to the R12 version of this object. 
    
    \param pFiler [in] Pointer to a filer object from which to read the data.
    \returns
    Returns the filer status.
    \remarks
    This function is called by dxfIn() to allow the object to read its data. 
  */
  virtual OdResult dxfInFields_R12(
    OdDbDxfFiler* pFiler);

  /** \details
    Writes the .dxf file data specific to the R12 version of this object. 
    
    \param pFiler [out] Pointer to a filer object to which to write the data.
    \returns
    Returns the filer status.
    \remarks
    This function is called by dxfOut() to allow the object to write its data. 
  */
  virtual void dxfOutFields_R12(
    OdDbDxfFiler* pFiler) const;

  /** \details
    Returns the CLSID value associated with this object.
    
    \param pClsid [out]  Pointer to the CLSID value.
    
    \remarks
    Valid only on Windows with Drawings SDK built as a DLL.
  */
  virtual OdResult subGetClassID(
    void* pClsid) const;

  /** \details
    Called as the first operation when this object is being closed; for
    database-resident objects only. 
    
    \remarks
    This function is notified just before the current open operation is to be closed, giving this function
    the ability to perform necessary operations.
  */
  void subClose();

  /** \details
    Called as the first operation when this object is being erased or unerased. 

    \param erasing [in]  A copy of the erasing argument passed to erase().

    \remarks
    This function is notified just before the current object is to be erased, giving this function
    the ability to cancel the erase.
    \returns 
    eOk if erase() is to continue.
  */
  OdResult subErase(
    bool erasing);

  /** \details
    Adjusts the parameters of this viewport object such that the view is zoomed 
    to the extents of the drawing.
  */
  void zoomExtents();

  virtual OdResult subGetGeomExtents(
    OdGeExtents3d& extents) const;

  /** \details
    Applies the specified 3D transformation matrix to this entity. 
    
    \param xfm [in] 3D transformation matrix. 
    \returns
    eOk if successful, or an appropriate error code otherwise.
  */
  virtual OdResult subTransformBy(
  const OdGeMatrix3d& xfm) ODRX_OVERRIDE;

  /** \details
    Notification function called whenever an object is opened in the OdDb::kForWrite mode, a function
    is called that could modify the contents of this object, and this object is now being
    closed.

    \param pObject [in]  Pointer to the object that is being closed after being modified.
  */
  void modified(const OdDbObject* pObj);

  /** \details
    Notification function called whenever an object is erased or unerased.

    \param pObject [in]  Pointer to the object that is being erased/unerased.
    \param erasing [in]  True if and only if this object is being erased.
  */
  void erased(const OdDbObject* pObj, bool pErasing);

  /** \details
    Retrieves the copied version of this viewport transformed by the specified transformation matrix.

    \param mat [in]  Transformation matrix to apply to the copy object.
    \param pCopy [out]  Receives a transformed copy of this viewport.
    \returns
    Result of the transformation to the copy viewport. If the method returns a result that is
    not eOk, the pCopy is NULL.
  */
  virtual OdResult subGetTransformedCopy(const OdGeMatrix3d& mat, OdDbEntityPtr& pCopy) const;

  /** \details
    Explodes viewport geometry into a set of simpler entities.

    \param entitySet [in/out]  Receives an array of pointers to the new entities. 
    \returns
    The newly created entities are not database residents. 
    Entities resulting from the explosion are appended to the specified array.
  */
  virtual OdResult explodeGeometry(OdRxObjectPtrArray& entitySet) const;
};

/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbViewport object pointers.
*/
typedef OdSmartPtr<OdDbViewport> OdDbViewportPtr;

#include "TD_PackPop.h"

#endif
