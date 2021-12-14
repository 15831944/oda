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


#ifndef __OD_GI_ENVIRONMENT_TRAITS_DATA__
#define __OD_GI_ENVIRONMENT_TRAITS_DATA__

#include "TD_PackPush.h"

#include "Gi/GiEnvironment.h"
#include "Gi/GiLightTraits.h" // Define OdGiSkyParameters

/** \details
    Corresponding C++ library: TD_Gi
    <group OdGi_Classes>
*/
class OdGiBackgroundTraitsData
{
protected:
  OdGiDrawable::DrawableType m_type;

protected:
  OdGiBackgroundTraitsData()
  {
  }

public:
  /** \details
    Retrieves drawable type.
    
    \returns
    drawable type as value from the OdGiDrawable::DrawableType enumeration.
  */
  OdGiDrawable::DrawableType type() const { return m_type; }

  /** \details
    Removes the specified background traits.
    
    \param pBackgroundTraits [in]  Pointer to background traits.
  */
  static void deleteBackgroundTraitsData(OdGiBackgroundTraitsData* pBackgroundTraits);

  /** \details
    Checks whether the passed background traits have the same type as these background traits.
    
    \param data2 [in]  Background traits to compare.
    
    \returns
    true if background traits are equal, false otherwise.
  */
  bool operator ==(const OdGiBackgroundTraitsData &data2) const
  {
    return m_type == data2.m_type;
  }
  
  /** \details
    Checks whether the passed background traits do not have the same type as these background traits.
    
    \param data2 [in]  Background traits to compare.
    
    \returns
    true if background traits are not equal, false otherwise.
  */
  bool operator !=(const OdGiBackgroundTraitsData &data2) const
  {
    return m_type != data2.m_type;
  }
};

/** \details
    Corresponding C++ library: TD_Gi
    <group OdGi_Classes>
*/
class OdGiSolidBackgroundTraitsData : public OdGiBackgroundTraitsData
{
  OdCmEntityColor      m_color;

public:
  /** \details
    Default constructor for the OdGiSolidBackgroundTraitsData class. Sets the 
    OdGiDrawable::kSolidBackground type for this object.
  */
  OdGiSolidBackgroundTraitsData() { m_type = OdGiDrawable::kSolidBackground; }

  /** \details
    Retrieves color of this solid background.
    
    \returns
    color of this solid background as OdCmEntityColor instance.
  */
  OdCmEntityColor color() const { return m_color; }
  
  /** \details
    Specifies color for this solid background.
    
    \param color [in]  Color for this solid background.
  */
  void setColor(const OdCmEntityColor& color) { m_color = color; }

  /** \details
    Checks whether the passed traits for solid backgrounds are equal to these traits.
    
    \param data2 [in]  Background traits to compare.
    
    \returns
    true if background traits are equal, false otherwise.
  */
  bool operator ==(const OdGiSolidBackgroundTraitsData &data2) const
  {
    return ((*static_cast<const OdGiBackgroundTraitsData*>(this) == static_cast<const OdGiBackgroundTraitsData&>(data2)) &&
           (m_color == data2.m_color));
  }
  
  /** \details
    Checks whether the passed traits for solid backgrounds are not equal to these traits.
    
    \param data2 [in]  Background traits to compare.
    
    \returns
    true if background traits are not equal, false otherwise.
  */
  bool operator !=(const OdGiSolidBackgroundTraitsData &data2) const
  {
    return !(*this == data2);
  }
};

/** \details
    Corresponding C++ library: TD_Gi
    <group OdGi_Classes>
*/
class OdGiGradientBackgroundTraitsData : public OdGiBackgroundTraitsData
{
  OdCmEntityColor      m_colorTop;
  OdCmEntityColor      m_colorMiddle;
  OdCmEntityColor      m_colorBottom;
  double               m_dHorizon;
  double               m_dHeight;
  double               m_dRotation;

public:
  /** \details
    Default constructor for the OdGiGradientBackgroundTraitsData class. Sets the 
    OdGiDrawable::kSolidBackground type for this object and horizon, height and rotation parameters to 0.
  */
  OdGiGradientBackgroundTraitsData() : m_dHorizon(0.0), m_dHeight(0.0), m_dRotation(0.0) { m_type = OdGiDrawable::kGradientBackground; }

  /** \details
    Retrieves a top color of this background.
    
    \returns
    top color as instance of the OdCmEntityColor class.
  */
  OdCmEntityColor colorTop() const { return m_colorTop; }
  
  /** \details
    Specifies a top color for this background.
    
    \param color [in]  Top color.
  */
  void setColorTop(const OdCmEntityColor& color) { m_colorTop = color; }
  
  /** \details
    Retrieves a middle color of this background.
    
    \returns
    middle color as instance of the OdCmEntityColor class.
  */
  OdCmEntityColor colorMiddle() const { return m_colorMiddle; }
  
  /** \details
    Specifies a middle color for this background.
    
    \param color [in]  Middle color.
  */
  void setColorMiddle(const OdCmEntityColor& color) { m_colorMiddle = color; }
  
  /** \details
    Retrieves a bottom color of this background.
    
    \returns
    bottom color as instance of the OdCmEntityColor class.
  */
  OdCmEntityColor colorBottom() const { return m_colorBottom; }
  
  /** \details
    Specifies a bottom color for this background.
    
    \param color [in]  Bottom color.
  */
  void setColorBottom(const OdCmEntityColor& color) { m_colorBottom = color; }
  
  
  /** \details
    Retrieves a position of a middle color relative to top and bottom colors.
    
    \returns
    horizon that represents position of a middle color relative to top and bottom colors.
  */
  double horizon() const { return m_dHorizon; }
  
  /** \details
    Receives height of the middle color.
    
    \returns
    height of a middle color.
  */
  double height() const { return m_dHeight; }
  
  /** \details
    Receives rotation of the gradient.
    
    \returns
    degrees of counter clock-wise gradient rotation.
  */
  double rotation() const { return m_dRotation; }

  
  /** \details
    Specifies a position of a middle color relative to top and bottom colors.
    
    \param horizon [in]  Position of a middle color in range where 0 is the beginning of device coordinate system and 1 is end of of device coordinate system.
    
    \remarks
    For example, if you call this method and pass 0.5, a middle color will reside in the middle of a window (if no rotation applied). The passed value may lie out of [0..1] range,
    which means that position for middle color is set off-screen.
  */
  void setHorizon(double value) { m_dHorizon = value; }
  
  /** \details
    Specifies height of the middle color.
    
    \param height [in]  Height of the middle color.
    
    \remarks
    For example, if you call the setHorizon() method and pass 0.5 and then call this method with 0.33 value then all 3 gradient
    colors are equally distributed inside scene window (if no rotation applied).
  */
  void setHeight(double value) { m_dHeight = value; }
  
  /** \details
    Specifies rotation of the gradient.
    
    \param rotation [in]  Rotation in degrees.
    
    \remarks
    Rotation is performed the counter clock-wise direction, relative to window coordinates. 
    For example, if you call this method with the 45.0 value the gradient background is so 
    the beginning of gradient is in lower-left corner of a window and the end is in the upper-right corner.
  */
  void setRotation(double value) { m_dRotation = value; }

  
  /** \details
    Checks whether the passed traits for gradient backgrounds are equal to these traits.
    Traits are considered equal if:
    * Top colors are equal;
    * Middle colors are equal;
    * Bottom colors are equal;
    * Horizons are equal;
    * Horizon heights are equal;
    * Rotations are equal.
    
    \param data2 [in]  Background traits to compare.
    
    \returns
    true if background traits are equal, false otherwise.
  */
  bool operator ==(const OdGiGradientBackgroundTraitsData &data2) const
  {
    return ((*static_cast<const OdGiBackgroundTraitsData*>(this) == static_cast<const OdGiBackgroundTraitsData&>(data2)) &&
           (m_colorTop == data2.m_colorTop) &&
           (m_colorMiddle == data2.m_colorMiddle) &&
           (m_colorBottom == data2.m_colorBottom) &&
           OdEqual(m_dHorizon, data2.m_dHorizon) &&
           OdEqual(m_dHeight, data2.m_dHeight) &&
           OdEqual(m_dRotation, data2.m_dRotation));
  }
  
  /** \details
    Checks whether the passed traits for gradient backgrounds are not equal to these traits.
    Traits are considered not equal if one of the following conditions fails:
    * Top colors are equal;
    * Middle colors are equal;
    * Bottom colors are equal;
    * Horizons are equal;
    * Horizon heights are equal;
    * Rotations are equal.
    
    \param data2 [in]  Background traits to compare.
    
    \returns
    true if background traits are not equal, false otherwise.
  */
  bool operator !=(const OdGiGradientBackgroundTraitsData &data2) const
  {
    return !(*this == data2);
  }
};

/** \details
    Corresponding C++ library: TD_Gi
    <group OdGi_Classes>
*/
class OdGiImageBackgroundTraitsData : public OdGiBackgroundTraitsData
{
  OdString             m_imageFileName;
  bool                 m_bFitToScreen;
  bool                 m_bMaintainAspect;
  bool                 m_bUseTiling;
  OdGeVector2d         m_vOffset;
  OdGeVector2d         m_vScale;

public:
  OdGiImageBackgroundTraitsData() : m_bFitToScreen(false), m_bMaintainAspect(false), m_bUseTiling(false) { m_type = OdGiDrawable::kImageBackground; }

  /** \details
    Retrieves image file name.
    
    \returns
    full path to an image file.
  */
  const OdString& imageFilename() const { return m_imageFileName; }
  
  /** \details
    Specifies image file name.
    
    \param filename [in]  Full path to an image file.
  */
  void setImageFilename(const OdString& filename) { m_imageFileName = filename; }

  /** \details
    Checks whether image-based background should be fit to screen.
    
    \returns
    true if background is fit to screen, false otherwise.
  */
  bool fitToScreen() const { return m_bFitToScreen; }
  
  /** \details
    Checks whether image-based background maintains aspect ratio.
    
    \returns
    true if aspect ratio is maintained, false otherwise.
  */
  bool maintainAspectRatio() const { return m_bMaintainAspect; }
  
  /** \details
    Checks whether image-based background is tiled.
    
    \returns
    true if background is tiled, false otherwise.
  */
  bool useTiling() const { return m_bUseTiling; }

  /** \details
    Specifies whether image-based background should be fit to screen.
    
    \param bFitToScreen [in]  Flag that specifies whether to fit this background to screen.
  */
  void setFitToScreen(bool flag) { m_bFitToScreen = flag; }
  
  /** \details
    Specifies whether image-based background should maintain aspect ratio. Usually called together with setFitToScreen() methods
    or other methods which can modify background dimensions.
    
    \param bMaintainAspectRatio [in]  Flag that specifies whether to maintain aspect ratio of a background image.
  */
  void setMaintainAspectRatio(bool flag) { m_bMaintainAspect = flag; }
  
  /** \details
    Specifies whether to tile image-based background.
    
    \param bUseTiling [in]  Flag that specifies whether to tile image-based background.
    
    \remarks
    Note that background cannot be tiled if it is set to fit to screen.
  */
  void setUseTiling(bool flag) { m_bUseTiling = flag; }

  /** \details
    Specifies offset of a background by both axes.
    
    \param x [in]  Offset distance by x-axis.
    \param y [in]  Offset distance by y-axis.
  */
  void setOffset(double x, double y);

  /** \details
    Retrieves offset of a background on the x-axis.
    
    \returns
    offset value on the x-axis.
  */
  double xOffset() const { return m_vOffset.x; }
  
  /** \details
    Retrieves offset of a background on the y-axis.
    
    \returns
    offset value on the y-axis.
  */
  double yOffset() const { return m_vOffset.y; }

  /** \details
    Specifies offset of a background on the x-axis.
    
    \param xOffset [in]  Offset distance. Negative values mean that background is offset in the opposite direction of x-axis. 
  */
  void setXOffset(double xOffset) { m_vOffset.x = xOffset; }
  
  /** \details
    Specifies offset of a background on the y-axis.
    
    \param yOffset [in]  Offset distance. Negative values mean that background is offset in the opposite direction of y-axis. 
  */
  void setYOffset(double yOffset) { m_vOffset.y = yOffset; }

  /** \details
    Specifies scale of a background by both axes.
    
    \param x [in]  Scale factor by x-axis.
    \param y [in]  Scale factor by y-axis.
  */
  void setScale(double x, double y);

  /** \details
    Retrieves scale of a background on the x-axis.
    
    \returns
    Scale of the x-axis for this background. Values between 0 and 1 mean that background is shrunk on x-axis, values greater than 1 mean than background is stretched
    and negative values mean that background is flipped on x-axis.
  */
  double xScale() const { return m_vScale.x; }
  
  /** \details
    Retrieves scale of a background on the y-axis.
    
    \returns
    Scale of the y-axis for this background. Values between 0 and 1 mean that background is shrunk on y-axis, values greater than 1 mean than background is stretched
    and negative values mean that background is flipped on y-axis.
  */
  double yScale() const { return m_vScale.y; }

  /** \details
    Specifies scale of a background on the x-axis.
    
    \param xScale [in]  Scale factor for the x-axis.
  */
  void setXScale(double xScale) { m_vScale.x = xScale; }
  
  /** \details
    Specifies scale of a background on the y-axis.
    
    \param yScale [in]  Scale factor for the y-axis.
  */
  void setYScale(double yScale) { m_vScale.y = yScale; }

  /** \details
    Checks whether the passed traits for image backgrounds are equal to these traits.
    Traits are considered equal if:
    * Full file paths are equal;
    * Fit to screen flags are equal;
    * Maintain aspect ratio flags are equal;
    * Use tiling flags are equal;
    * Offset distances are equal by both axes;
    * Scale factors are equal by both axes;
    
    \param data2 [in]  Background traits to compare.
    
    \returns
    true if background traits are equal, false otherwise.
  */
  bool operator ==(const OdGiImageBackgroundTraitsData &data2) const
  {
    return ((*static_cast<const OdGiBackgroundTraitsData*>(this) == static_cast<const OdGiBackgroundTraitsData&>(data2)) &&
           (m_imageFileName == data2.m_imageFileName) &&
           (m_bFitToScreen == data2.m_bFitToScreen) &&
           (m_bMaintainAspect == data2.m_bMaintainAspect) &&
           (m_bUseTiling == data2.m_bUseTiling) &&
           (m_vOffset == data2.m_vOffset) &&
           (m_vScale == data2.m_vScale));
  }
  
  /** \details
    Checks whether the passed traits for image backgrounds are not equal to these traits.
    Traits are considered not equal if one of the following conditions fails:
    * Full file paths are equal;
    * Fit to screen flags are equal;
    * Maintain aspect ratio flags are equal;
    * Use tiling flags are equal;
    * Offset distances are equal by both axes;
    * Scale factors are equal by both axes;
    
    \param data2 [in]  Background traits to compare.
    
    \returns
    true if background traits are not equal, false otherwise.
  */
  bool operator !=(const OdGiImageBackgroundTraitsData &data2) const
  {
    return !(*this == data2);
  }
};

inline void OdGiImageBackgroundTraitsData::setOffset(double x, double y)
{ 
  m_vOffset.set(x, y);
}

inline void OdGiImageBackgroundTraitsData::setScale(double x, double y)
{ 
  m_vScale.set(x, y);
}

/** \details
    Corresponding C++ library: TD_Gi
    <group OdGi_Classes>
*/
class OdGiGroundPlaneBackgroundTraitsData : public OdGiBackgroundTraitsData
{
  OdCmEntityColor      m_colorSkyZenith;
  OdCmEntityColor      m_colorSkyHorizon;
  OdCmEntityColor      m_colorUndergroundHorizon;
  OdCmEntityColor      m_colorUndergroundAzimuth;
  OdCmEntityColor      m_colorGroundPlaneNear;
  OdCmEntityColor      m_colorGroundPlaneFar;

public:
  /** \details
    Default constructor for the OdGiGroundPlaneBackgroundTraitsData class. Sets the 
    OdGiDrawable::kGroundPlaneBackground type for this object.
  */
  OdGiGroundPlaneBackgroundTraitsData() { m_type = OdGiDrawable::kGroundPlaneBackground; }

  
  /** \details
    Retrieves a color of the sky zenith.
    
    \returns
    color of sky zenith as OdCmEntityColor instance.
  */
  OdCmEntityColor colorSkyZenith() const { return m_colorSkyZenith; }
  
  /** \details
    Specifies a color of the sky zenith.
    
    \param color [in]  Color of the sky zenith.
  */
  void setColorSkyZenith(const OdCmEntityColor& color) { m_colorSkyZenith = color; }
  
  /** \details
    Retrieves a color of the sky horizon.
    
    \returns
    color of sky horizon as OdCmEntityColor instance.
  */
  OdCmEntityColor colorSkyHorizon() const { return m_colorSkyHorizon; }
  
  /** \details
    Specifies a color of the sky horizon.
    
    \param color [in]  Color of the sky horizon.
  */
  void setColorSkyHorizon(const OdCmEntityColor& color) { m_colorSkyHorizon = color; }
  
  /** \details
    Retrieves a color of the underground horizon.
    
    \returns
    color of underground horizon as OdCmEntityColor instance.
  */
  OdCmEntityColor colorUndergroundHorizon() const { return m_colorUndergroundHorizon; }
  
  /** \details
    Specifies a color of the underground horizon.
    
    \param color [in]  Color of the underground horizon.
  */
  void setColorUndergroundHorizon(const OdCmEntityColor& color) { m_colorUndergroundHorizon = color; }
  
  /** \details
    Retrieves a color of the underground azimuth.
    
    \returns
    color of underground azimuth as OdCmEntityColor instance.
  */
  OdCmEntityColor colorUndergroundAzimuth() const { return m_colorUndergroundAzimuth; }
  
  /** \details
    Specifies a color of the underground azimuth.
    
    \param color [in]  Color of the underground azimuth.
  */
  void setColorUndergroundAzimuth(const OdCmEntityColor& color) { m_colorUndergroundAzimuth = color; }
  
  /** \details
    Retrieves a color of the near ground plane.
    
    \returns
    color of the near ground plane as OdCmEntityColor instance.
  */
  OdCmEntityColor colorGroundPlaneNear() const { return m_colorGroundPlaneNear; }
  
  /** \details
    Specifies a color of the near ground plane.
    
    \param color [in]  Color of the near ground plane.
  */
  void setColorGroundPlaneNear(const OdCmEntityColor& color) { m_colorGroundPlaneNear = color; }
  
  /** \details
    Retrieves a color of the far ground plane.
    
    \returns
    color of the far ground plane as OdCmEntityColor instance.
  */
  OdCmEntityColor colorGroundPlaneFar() const { return m_colorGroundPlaneFar; }
  
  /** \details
    Specifies a color of the far ground plane.
    
    \param color [in]  Color of the far ground plane.
  */
  void setColorGroundPlaneFar(const OdCmEntityColor& color) { m_colorGroundPlaneFar = color; }

  /** \details
    Checks whether the passed traits for ground plane backgrounds are equal to these traits.
    Traits are considered equal if:
    * Colors of sky zeniths are equal;
    * Colors of sky horizons are equal;
    * Colors of underground horizons are equal;
    * Colors of underground azimuths are equal;
    * Colors of near ground planes are equal;
    * Colors of far ground planes are equal;
    
    \param data2 [in]  Background traits to compare.
    
    \returns
    true if background traits are equal, false otherwise.
  */
  bool operator ==(const OdGiGroundPlaneBackgroundTraitsData &data2) const
  {
    return ((*static_cast<const OdGiBackgroundTraitsData*>(this) == static_cast<const OdGiBackgroundTraitsData&>(data2)) &&
           (m_colorSkyZenith == data2.m_colorSkyZenith) &&
           (m_colorSkyHorizon == data2.m_colorSkyHorizon) &&
           (m_colorUndergroundHorizon == data2.m_colorUndergroundHorizon) &&
           (m_colorUndergroundAzimuth == data2.m_colorUndergroundAzimuth) &&
           (m_colorGroundPlaneNear == data2.m_colorGroundPlaneNear) &&
           (m_colorGroundPlaneFar == data2.m_colorGroundPlaneFar));
  }
  
  /** \details
    Checks whether the passed traits for ground plane backgrounds are not equal to these traits.
    Traits are not equal if one of the following conditions fails:
    * Colors of sky zeniths are equal;
    * Colors of sky horizons are equal;
    * Colors of underground horizons are equal;
    * Colors of underground azimuths are equal;
    * Colors of near ground planes are equal;
    * Colors of far ground planes are equal;
    
    \param data2 [in]  Background traits to compare.
    
    \returns
    true if background traits are not equal, false otherwise.
  */
  bool operator !=(const OdGiGroundPlaneBackgroundTraitsData &data2) const
  {
    return !(*this == data2);
  }
};

/** \details
    Corresponding C++ library: TD_Gi
    <group OdGi_Classes>
*/
class OdGiSkyBackgroundTraitsData : public OdGiBackgroundTraitsData
{
  bool            m_status;
  double          m_intensityFactor;
  double          m_haze;
  double          m_horizonHeight;
  double          m_horizonBlur;
  OdCmEntityColor m_groundColor;
  OdCmEntityColor m_nightColor;
  bool            m_aerialPerspective;
  double          m_visibilityDistance;
  double          m_diskScale;
  double          m_glowIntensity;
  double          m_diskIntensity;
  OdUInt16        m_solarDiskSamples;
  OdGeVector3d    m_sunDirection;
  double          m_redBlueShift;
  double          m_saturation;
public:
  /** \details
    Default constructor for the OdGiSkyBackgroundTraitsData class. Sets the 
    OdGiDrawable::kSkyBackground type for this object.
  */
  OdGiSkyBackgroundTraitsData() { m_type = OdGiDrawable::kSkyBackground; }

  /** \details
    Retrieves sky parameters of this background.
    
    \param params [out]  Receives sky parameters for this parameters.
  */
  void skyParameters(OdGiSkyParameters& params) const;
  
  /** \details
    Specifies sky parameters of this background.
    
    \param params [in]  Sky parameters.
  */
  void setSkyParameters(const OdGiSkyParameters& params);

  /** \details
    Checks whether the passed traits for sky backgrounds are equal to these traits.
    Traits are considered equal if:
    * Statuses are equal;
    * Intensity factors are equal;
    * Haze (or turbulence) values are equal;
    * Horizon heights are equal;
    * Horizon blur values are equal;
    * Ground colors are equal.
    * Night colors are equal.
    * Aerial perspective flags are equal.
    * Visibility distances are equal.
    * Sun disk scales are equal.
    * Sun glow intensity values are equal.
    * Sun disk intensity values are equal.
    * Solar disk samples are equal;
    * Sun direction vectors are equal;
    * The red-blue shift factors are equal;
    * Sky saturation values are equal;
    
    \param data2 [in]  Background traits to compare.
    
    \returns
    true if background traits are equal, false otherwise.
  */
  bool operator ==(const OdGiSkyBackgroundTraitsData &data2) const
  {
    return ((*static_cast<const OdGiBackgroundTraitsData*>(this) == static_cast<const OdGiBackgroundTraitsData&>(data2)) &&
           (m_status == data2.m_status) &&
           OdEqual(m_intensityFactor, data2.m_intensityFactor) &&
           OdEqual(m_haze, data2.m_haze) &&
           OdEqual(m_horizonHeight, data2.m_horizonHeight) &&
           OdEqual(m_horizonBlur, data2.m_horizonBlur) &&
           (m_groundColor == data2.m_groundColor) &&
           (m_nightColor == data2.m_nightColor) &&
           (m_aerialPerspective == data2.m_aerialPerspective) &&
           OdEqual(m_visibilityDistance, data2.m_visibilityDistance) &&
           OdEqual(m_diskScale, data2.m_diskScale) &&
           OdEqual(m_glowIntensity, data2.m_glowIntensity) &&
           OdEqual(m_diskIntensity, data2.m_diskIntensity) &&
           (m_solarDiskSamples == data2.m_solarDiskSamples) &&
           (m_sunDirection == data2.m_sunDirection) &&
           OdEqual(m_redBlueShift, data2.m_redBlueShift) &&
           OdEqual(m_saturation, data2.m_saturation));
  }
  
  /** \details
    Checks whether the passed traits for sky backgrounds are not equal to these traits.
    Traits are not equal if one of the following conditions fails:
    * Statuses are equal;
    * Intensity factors are equal;
    * Haze (or turbulence) values are equal;
    * Horizon heights are equal;
    * Horizon blur values are equal;
    * Ground colors are equal.
    * Night colors are equal.
    * Aerial perspective flags are equal.
    * Visibility distances are equal.
    * Sun disk scales are equal.
    * Sun glow intensity values are equal.
    * Sun disk intensity values are equal.
    * Solar disk samples are equal;
    * Sun direction vectors are equal;
    * The red-blue shift factors are equal;
    * Sky saturation values are equal;
    
    \param data2 [in]  Background traits to compare.
    
    \returns
    true if background traits are not equal, false otherwise.
  */
  bool operator !=(const OdGiSkyBackgroundTraitsData &data2) const
  {
    return !(*this == data2);
  }
};

inline void OdGiSkyBackgroundTraitsData::skyParameters(OdGiSkyParameters& params) const
{
  params.setIllumination(m_status);
  params.setIntensityFactor(m_intensityFactor);
  params.setHaze(m_haze);
  params.setHorizonHeight(m_horizonHeight);
  params.setHorizonBlur(m_horizonBlur);
  params.setGroundColor(m_groundColor);
  params.setNightColor(m_nightColor);
  params.setAerialPerspective(m_aerialPerspective);
  params.setVisibilityDistance(m_visibilityDistance);
  params.setDiskScale(m_diskScale);
  params.setGlowIntensity(m_glowIntensity);
  params.setDiskIntensity(m_diskIntensity);
  params.setSolarDiskSamples(m_solarDiskSamples);
  params.setSunDirection(m_sunDirection);
  params.setRedBlueShift(m_redBlueShift);
  params.setSaturation(m_saturation);
}

inline void OdGiSkyBackgroundTraitsData::setSkyParameters(const OdGiSkyParameters& params)
{
  m_status = params.illumination();
  m_intensityFactor = params.intensityFactor();
  m_haze = params.haze();
  m_horizonHeight = params.horizonHeight();
  m_horizonBlur = params.horizonBlur();
  m_groundColor = params.groundColor();
  m_nightColor = params.nightColor();
  m_aerialPerspective = params.aerialPerspective();
  m_visibilityDistance = params.visibilityDistance();
  m_diskScale = params.diskScale();
  m_glowIntensity = params.glowIntensity();
  m_diskIntensity = params.diskIntensity();
  m_solarDiskSamples = params.solarDiskSamples();
  m_sunDirection = params.sunDirection();
  m_redBlueShift = params.redBlueShift();
  m_saturation = params.saturation();
}

/** \details
    Corresponding C++ library: TD_Gi
    <group OdGi_Classes>
*/
class OdGiIBLBackgroundTraitsData : public OdGiBackgroundTraitsData
{
  bool      m_bEnable;
  OdString  m_iblFileName;
  double    m_rotation;
  bool      m_bDisplay;
  OdDbStub *m_secBkgndId;

public:
  
  /** \details
    Default constructor for the OdGiIBLBackgroundTraitsData class. Sets the 
    OdGiDrawable::kImageBasedLightingBackground type for this object.
  */
  OdGiIBLBackgroundTraitsData() { m_type = OdGiDrawable::kImageBasedLightingBackground; }

  /** \details
    Checks whether image-based lighting is enabled.
    
    \returns
    true if image-based lighting is enabled, false otherwise.
  */
  bool isEnabled() const { return m_bEnable; }
  
  /** \details
    Specifies whether image-based lighting background should be enabled.
    
    \param bSet [in]  Flag that specifies whether to activate IBL.
  */
  void enable(bool bSet) { m_bEnable = bSet; }

  
  
  /** \details
    Retrieves image file name for this image-based lighting background.
    
    \returns
    full path to an image file.
  */
  const OdString &IBLFileName() const { return m_iblFileName; }
  
  /** \details
    Specifies image file name for the image-based lighting background.
    
    \param iblFileName [in]  Full path to an image file.
  */
  void setIBLFileName(const OdString &iblFileName) { m_iblFileName = iblFileName; }

  
  
  /** \details
    Retrieves rotation value for the IBL image.
    
    \returns
    rotation value for the IBL image in degrees.
  */
  double rotation() const { return m_rotation; }
  
  /** \details
    Specifies rotation value for the IBL image in range [-180..180].
    
    \param angle [in]  Rotation value in degrees.
  */
  void setRotation(double angle) { m_rotation = angle; }

  
  
  /** \details
    Checks whether the IBL image is displayed as the background.
    
    \returns
    true if the IBL image is displayed as the background, false otherwise.
  */
  bool displayImage() const { return m_bDisplay; }
  
  /** \details
    Specifies whether to display the IBL image as the background.
    
    \param bSet [in]  Flag that specifies whether to display IBL image as a background.
  */
  void setDisplayImage(bool bSet) { m_bDisplay = bSet; }

  
  
  /** \details
    Retrieves a background identifier of a secondary background.
    
    \returns
    Pointer to an identifier of a secondary background.
  */
  OdDbStub *secondaryBackground() const { return m_secBkgndId; }
  
  /** \details
    Specifies a background to display when false is passed to the DisplayImage() method.
    
    \param secBkgndId [in]  Pointer to a background identifier.
  */
  void setSecondaryBackground(OdDbStub *secBkgndId) { m_secBkgndId = secBkgndId; }

  
  
  /** \details
    Checks whether the passed traits for image-based lighting backgrounds are equal to these traits.
    Traits are considered equal if:
    * Image-based background lighting enable flags are equal;
    * Full file paths are equal;
    * Rotation values are equal;
    * Display flags are equal;
    * Secondary background identifiers are equal;
    
    \param data2 [in]  Background traits to compare.
    
    \returns
    true if background traits are equal, false otherwise.
  */
  bool operator ==(const OdGiIBLBackgroundTraitsData &data2) const
  {
    return ((*static_cast<const OdGiBackgroundTraitsData*>(this) == static_cast<const OdGiBackgroundTraitsData&>(data2)) &&
           (m_bEnable == data2.m_bEnable) &&
           (m_iblFileName == data2.m_iblFileName) &&
           OdEqual(m_rotation, data2.m_rotation) &&
           (m_bDisplay == data2.m_bDisplay) &&
           (m_secBkgndId == data2.m_secBkgndId));
  }
  
  /** \details
    Checks whether the passed traits for image-based lighting backgrounds are not equal to these traits.
    Traits are not equal if one of the following conditions fails:
    * Image-based background lighting enable flags are equal;
    * Full file paths are equal;
    * Rotation values are equal;
    * Display flags are equal;
    * Secondary background identifiers are equal;
    
    \param data2 [in]  Background traits to compare.
    
    \returns
    true if background traits are not equal, false otherwise.
  */
  bool operator !=(const OdGiIBLBackgroundTraitsData &data2) const
  {
    return !(*this == data2);
  }
};

// Delete background traits data by background type

inline void OdGiBackgroundTraitsData::deleteBackgroundTraitsData(OdGiBackgroundTraitsData* pBackgroundTraits)
{
  switch (pBackgroundTraits->type())
  {
    case OdGiDrawable::kSolidBackground:
      delete static_cast<OdGiSolidBackgroundTraitsData*>(pBackgroundTraits);
    break;
    case OdGiDrawable::kGradientBackground:
      delete static_cast<OdGiGradientBackgroundTraitsData*>(pBackgroundTraits);
    break;
    case OdGiDrawable::kImageBackground:
      delete static_cast<OdGiImageBackgroundTraitsData*>(pBackgroundTraits);
    break;
    case OdGiDrawable::kGroundPlaneBackground:
      delete static_cast<OdGiGroundPlaneBackgroundTraitsData*>(pBackgroundTraits);
    break;
    case OdGiDrawable::kSkyBackground:
      delete static_cast<OdGiSkyBackgroundTraitsData*>(pBackgroundTraits);
    break;
    case OdGiDrawable::kImageBasedLightingBackground:
      delete static_cast<OdGiIBLBackgroundTraitsData*>(pBackgroundTraits);
    break;
    default:
      ODA_FAIL();
  }
}

/** \details
    Corresponding C++ library: Gi
    <group OdGi_Classes>
*/
class OdGiRenderEnvironmentTraitsData
{
  bool                   m_bEnable;
  bool                   m_bIsBackground;
  OdCmEntityColor        m_FogColor;
  double                 m_dNearDist;
  double                 m_dFarDist;
  double                 m_dNearPerc;
  double                 m_dFarPerc;
  OdGiMaterialTexturePtr m_envMap;
public:
  
  /** \details
    Default constructor for the OdGiRenderEnvironmentTraitsData class. Sets fog display and background flags,
    fog color, fog render distances and percentage.
  */
  OdGiRenderEnvironmentTraitsData()
    : m_bEnable(false)
    , m_bIsBackground(false)
    , m_FogColor(128, 128, 128)
    , m_dNearDist(0.0)
    , m_dFarDist(100.0)
    , m_dNearPerc(0.0)
    , m_dFarPerc(100.0)
  {
  }

  /** \details
    Enables or disables the fog rendering.
      
    \param bEnable [in]  Flag that specifies whether to enable fog rendering.
  */
  void setEnable(bool bEnable) { m_bEnable = bEnable; }
  
  /** \details
    Checks whether fog rendering is enabled.
      
    \returns
    true if fog rendering is enabled, false otherwise.
  */
  bool enable() const { return m_bEnable; }
  
  /** \details
    Specifies whether fog should be treated as a background.
      
    \param bEnable [in]  Flag that specifies whether to treat fog as background.
  */
  void setIsBackground(bool bEnable) { m_bIsBackground = bEnable; }
  
  /** \details
    Checks whether fog is treated background.
      
    \returns
    true if fog is treated as background, false otherwise.
  */
  bool isBackground() const { return m_bIsBackground; }
  
  /** \details
    Sets background color.
      
    \param color [in]  Fog color.
  */
  void setFogColor(const OdCmEntityColor &color) { m_FogColor = color; }
  
  /** \details
    Retrieves fog color.
      
    \returns
    fog color as a reference to OdCmEntityColor instance.
  */
  const OdCmEntityColor &fogColor() const { return m_FogColor; }
  
  /** \details
    Specifies near distance for the fog rendering.
      
    \param nearDist [in]  Near distance of the fog.
  */
  void setNearDistance(double nearDist) { m_dNearDist = nearDist; }
  
  /** \details
    Retrieves near distance of fog rendering.
      
    \returns
    fog rendering near distance.
  */
  double nearDistance() const { return m_dNearDist; }
  
  /** \details
    Specifies far distance for the fog rendering.
      
    \param farDist [in]  Far distance of the fog.
  */
  void setFarDistance(double farDist) { m_dFarDist = farDist; }
  
  /** \details
    Retrieves far distance of fog rendering.
      
    \returns
    fog rendering far distance.
  */
  double farDistance() const { return m_dFarDist; }
  
  /** \details
    Specifies near percentage of the fog.
      
    \param nearPct [in]  Near percentage of the fog.
  */
  void setNearPercentage(double nearPct) { m_dNearPerc = nearPct; }
  
  /** \details
    Retrieves near percentage of the fog.
      
    \returns
    near percentage of the fog.
  */
  double nearPercentage() const { return m_dNearPerc; }
  
  /** \details
    Specifies far percentage of the fog.
      
    \param farPct [in]  Far percentage of the fog.
  */
  void setFarPercentage(double farPct) { m_dFarPerc = farPct; }
  
  /** \details
    Retrieves far percentage of the fog.
      
    \returns
    far percentage of the fog.
  */
  double farPercentage() const { return m_dFarPerc; }
  
  /** \details
    Sets the environment map.
      
    \param m [in]  Pointer to the environment map.
  */
  void setEnvironmentMap(const OdGiMaterialTexture *map)
  {
    m_envMap.release();
    if (map)
    {
      m_envMap = OdGiMaterialTexture::cast(map->clone());
    }
  }
  
  /** \details
    Retrieves the environment map.
      
    \returns
    smart pointer to the OdGiMaterialTexture instance that represents environment map.
  */
  OdGiMaterialTexturePtr environmentMap() const { return m_envMap; }

  /** \details
    Checks whether the passed traits for render environment traits are equal to these render traits.
    Traits are considered equal if:
    * The enable flags are equal;
    * The background flags are equal;
    * Fog colors are equal;
    * Near and far distances are equal;
    * Near and far percentages are equal;
    * Either both object have environment maps absent or have the same environment maps;
    
    \param data2 [in]  Traits to compare.
    
    \returns
    true if traits are equal, false otherwise.
  */
  bool operator ==(const OdGiRenderEnvironmentTraitsData &data2) const
  {
    return ((m_bEnable == data2.m_bEnable) &&
           (m_bIsBackground == data2.m_bIsBackground) &&
           (m_FogColor == data2.m_FogColor) &&
           OdEqual(m_dNearDist, data2.m_dNearDist) &&
           OdEqual(m_dFarDist, data2.m_dFarDist) &&
           OdEqual(m_dNearPerc, data2.m_dNearPerc) &&
           OdEqual(m_dFarPerc, data2.m_dFarPerc) &&
           ((m_envMap.isNull() && data2.m_envMap.isNull()) ||
            (!m_envMap.isNull() && !data2.m_envMap.isNull() && (*m_envMap == *data2.m_envMap))));
  }
  
  /** \details
    Checks whether the passed traits for render environment traits are not equal to these render traits.
    Traits are not equal if one of the following conditions fails:
    * The enable flags are equal;
    * The background flags are equal;
    * Fog colors are equal;
    * Near and far distances are equal;
    * Near and far percentages are equal;
    * Either both object have environment maps absent or have the same environment maps;
    
    \param data2 [in]  Traits to compare.
    
    \returns
    true if traits are not equal, false otherwise.
  */
  bool operator !=(const OdGiRenderEnvironmentTraitsData &data2) const
  {
    return !(*this == data2);
  }
};

/** \details
    Corresponding C++ library: Gi
    <group OdGi_Classes>
*/
class OdGiRenderSettingsTraitsData
{
  bool   m_bMatEnable;
  bool   m_bTextureSampling;
  bool   m_bBackFaces;
  bool   m_bShadows;
  bool   m_bDiagnosticBack;
  double m_dScaleFactor;
public:
  /** \details
    Default constructor for the OdGiRenderSettingsTraitsData class. Sets the following parameters:
    <table>
    Data member           Value     Description
    m_bMatEnable          true      Flag that shows if materials are enabled
    m_bTextureSampling    true      Flag that shows if texture sampling is enabled
    m_bBackFaces          true      Flag that shows if back faces are displayed
    m_bShadows            true      Flag that shows if shadows are enabled
    m_bDiagnosticBack     false     Flag that shows if diagnostic background is enabled
    m_dScaleFactor        1.0       Scale factor
    </table>
  */
  OdGiRenderSettingsTraitsData()
    : m_bMatEnable(true)
    , m_bTextureSampling(true)
    , m_bBackFaces(true)
    , m_bShadows(true)
    , m_bDiagnosticBack(false)
    , m_dScaleFactor(1.0)
  {
  }
  
  /** \details
    Destructor for the OdGiRenderSettingsTraitsData class.
  */
  virtual ~OdGiRenderSettingsTraitsData() { } // Exclude warnings in some compilers

  /** \details
    Specifies whether per-object materials should be used. 
      
    \param enabled [in]  Flag that specifies whether per-object materials should be used.
    
    \remarks
    If set to false, the global material is used for all objects.
  */
  void setMaterialEnabled(bool enabled) { m_bMatEnable = enabled; }
  
  /** \details
    Checks whether per-object materials are used.
      
    \returns
    true if per-object materials are used.
  */
  bool materialEnabled() const { return m_bMatEnable; }
  
  /** \details
    Specifies whether texture sampling (filtering) should be used for image textures. 
      
    \param enabled [in]  Flag that specifies whether sampling should be used.
  */
  void setTextureSampling(bool enabled) { m_bTextureSampling = enabled; }
  
  /** \details
    Checks whether texture sampling (filtering) is used for image textures.
      
    \returns
    true if texture sampling is used, false otherwise.
    
    \remarks
    texture sampling is not applied to textured created in a procedural way.
  */
  bool textureSampling() const { return m_bTextureSampling; }
  
  /** \details
    Specifies whether back faces should be rendered. 
      
    \param enabled [in]  Flag that specifies whether back faces should be rendered.
  */
  void setBackFacesEnabled(bool enabled) { m_bBackFaces = enabled; }
  
  /** \details
    Checks whether back faces rendering is enabled.
      
    \returns
    true if back faces rendering is enabled, false otherwise.
  */
  bool backFacesEnabled() const { return m_bBackFaces; }
  
  /** \details
    Specifies whether shadows should be rendered. 
      
    \param enabled [in]  Flag that specifies whether shadows should be rendered.
  */
  void setShadowsEnabled(bool enabled) { m_bShadows = enabled; }
  
  /** \details
    Checks whether shadows rendering is enabled.
      
    \returns
    true if shadows rendering is enabled, false otherwise.
  */
  bool shadowsEnabled() const { return m_bShadows; }
  
  /** \details
    Specifies whether checkerboard (diagnostic) background should be used for rendering. 
      
    \param enabled [in]  Flag that specifies whether checkerboard background should be used.
  */
  void setDiagnosticBackgroundEnabled(bool enabled) { m_bDiagnosticBack = enabled; }
  
  /** \details
    Checks whether checkerboard (diagnostic) background is enabled.
      
    \returns
    true if diagnostic background rendering is enabled, false otherwise.
  */
  bool diagnosticBackgroundEnabled() const { return m_bDiagnosticBack; }
  
  /** \details
    Specifies model scale related to meters. 
      
    \param scaleFactor [in]  Scale factor for a model.
    
    \remarks
    For example, if the model is drawn in kilometers, the scale factor is 1000.0. This is later used for the global illumination.
  */
  void setModelScaleFactor(double scaleFactor) { m_dScaleFactor = scaleFactor; }
  
  /** \details
    Retrieves model scale related to meters.
      
    \returns
    scale factor as a value of the double type. For example if model is drawn in kilometers, the returned value is 1000.0.
  */
  double modelScaleFactor() const { return m_dScaleFactor; }

  /** \details
    Checks whether the passed render settings traits are equal to these traits.
    Traits are considered equal if:
    * Material flags are equal;
    * Texture sampling flags are equal;
    * Back faces rendering flags are equal;
    * Shadows rendering flags are equal;
    * Diagnostic background flags are equal;
    * Scale factors are equal.
    
    \param data2 [in]  Traits to compare.
    
    \returns
    true if traits are equal, false otherwise.
  */
  bool operator ==(const OdGiRenderSettingsTraitsData &data2) const
  {
    return ((m_bMatEnable == data2.m_bMatEnable) &&
           (m_bTextureSampling == data2.m_bTextureSampling) &&
           (m_bBackFaces == data2.m_bBackFaces) &&
           (m_bShadows == data2.m_bShadows) &&
           (m_bDiagnosticBack == data2.m_bDiagnosticBack) &&
           OdEqual(m_dScaleFactor, data2.m_dScaleFactor));
  }
  
  /** \details
    Checks whether the passed render settings traits are not equal to these traits.
    Traits are considered not equal if one of the following conditions fails:
    * Material flags are equal;
    * Texture sampling flags are equal;
    * Back faces rendering flags are equal;
    * Shadows rendering flags are equal;
    * Diagnostic background flags are equal;
    * Scale factors are equal.
    
    \param data2 [in]  Traits to compare.
    
    \returns
    true if traits are equal, false otherwise.
  */
  bool operator !=(const OdGiRenderSettingsTraitsData &data2) const
  {
    return !(*this == data2);
  }
};

/** \details
    Corresponding C++ library: Gi
    <group OdGi_Classes>
*/
class OdGiMentalRayRenderSettingsTraitsData : public virtual OdGiRenderSettingsTraitsData
{
  int                        m_iSamplingMin;
  int                        m_iSamplingMax;
  OdGiMrFilter               m_SamplingFilter;
  double                     m_dSamplingWidth;
  double                     m_dSamplingHeight;
  float                      m_fContrastR;
  float                      m_fContrastG;
  float                      m_fContrastB;
  float                      m_fContrastA;
  OdGiMrShadowMode           m_ShadowMode;
  bool                       m_bShadowMapEnabled;
  bool                       m_bRayTraceEnabled;
  int                        m_iRTReflection;
  int                        m_iRTRefraction;
  int                        m_iRTSum;
  bool                       m_bGlobalIllum;
  int                        m_iGISampleCount;
  bool                       m_bSampleRadEnabled;
  double                     m_dSampleRad;
  int                        m_iPhotonsPerLight;
  int                        m_iPhotonsReflection;
  int                        m_iPhotonsRefraction;
  int                        m_iPhotonsSum;
  bool                       m_bFGEnabled;
  int                        m_iFGRayCount;
  bool                       m_bFGRadMin;
  bool                       m_bFGRadMax;
  bool                       m_bFGPixels;
  double                     m_dFGSRadMin;
  double                     m_dFGSRadMax;
  double                     m_dLightLumScale;
  OdGiMrDiagnosticMode       m_DiagMode;
  OdGiMrDiagnosticGridMode   m_DiagGridMode;
  float                      m_fDiagGridSize;
  OdGiMrDiagnosticPhotonMode m_DiagPhtMode;
  OdGiMrDiagnosticBSPMode    m_DiagBSPMode;
  bool                       m_bExportMIEnabled;
  OdString                   m_sMIFile;
  int                        m_iTileSize;
  OdGiMrTileOrder            m_TileOrder;
  int                        m_iMemLimit;
  float                      m_fEnergyMult;
  void*                      m_pProgressMonitor;
  OdGiMrExposureType         m_ExposureType;
  OdGiMrFinalGatheringMode   m_FGMode;
  double                     m_dShadowMult;
  OdGiMrExportMIMode         m_MIMode;
public:
  
  /** \details
    Default constructor for the OdGiRenderSettingsTraitsData class. Sets the following parameters:
    <table>
    <B>Data member</B>            <B>Value</B>               <B>Description</B>
    m_iSamplingMin                -1                         Minimum number of samples for pixel shading.
    m_iSamplingMax                0                          Maximum number of samples for pixel shading.
    m_SamplingFilter              krBox                      Filter for merging multiple samples into a pixel color.
    m_dSamplingWidth              1.0                        Sample width in pixels.
    m_dSamplingHeight             1.0                        Sample height in pixels.
    m_fContrastR                  0.1f                       Threshold value for red color channel of samples.
    m_fContrastG                  0.1f                       Threshold value for green color channel of samples.
    m_fContrastB                  0.1f                       Threshold value for blue color channel of samples.
    m_fContrastA                  0.1f                       Threshold value for alpha channel of samples.
    m_ShadowMode                  krSimple                   Mode for processing shadows.
    m_bShadowMapEnabled           true                       Flag that shows if shadow maps are enabled
    m_bRayTraceEnabled            true                       Flag that shows if ray tracing is enabled
    m_iRTReflection               2                          Level of recursion for computing reflection rays.
    m_iRTRefraction               2                          Level of recursion for computing refraction rays.
    m_iRTSum                      4                          Maximum recursion level of reflection and refraction rays for ray tracing, combined.
    m_bGlobalIllum                false                      Flag that shows if global illumination is enabled.
    m_iGISampleCount              500                        Maximum number of photons surrounding a render sample point to use for calculation of global illumination.
    m_bSampleRadEnabled           false                      Flag that shows if user-defined photon radius should be used.
    m_dSampleRad                  1.0                        Sample area of photons used for global illumination.
    m_iPhotonsPerLight            10000                      Average number of photons to emit for each light source.
    m_iPhotonsReflection          5                          Reflection tracing depth for photons.
    m_iPhotonsRefraction          5                          Refraction tracing depth for photons.
    m_iPhotonsSum                 5                          Maximum recursion level of reflection and refraction rays for photons, combined.
    m_bFGEnabled                  false                      Flag that shows if final gathering is enabled.
    m_iFGRayCount                 1000                       Amount of final gather rays to use for each final gather point.
    m_bFGRadMin                   false                      Flag that shows if the user-defined minimum radius is used.
    m_bFGRadMax                   false                      Flag that shows if the user-defined maximum radius is used.
    m_bFGPixels                   false                      Flag that shows if the user-defined radii are measured in pixel units.
    m_dFGSRadMin                  1.0                        Minimum final gather sample radius.
    m_dFGSRadMax                  1.0                        Maximum final gather sample radius.
    m_dLightLumScale              1.0                        Luminance of a light with intensity 1.0 (in candelas).
    m_DiagMode                    krOff                      Mode for processing diagnostic images.
    m_DiagGridMode                krObject                   Coordinate system for the diagnostic grid.
    m_fDiagGridSize               10.0f                      Distance between grid lines (size).
    m_DiagPhtMode                 krDensity                  Type of photon information to use in the photon diagnostic mode.
    m_DiagBSPMode                 krDepth                    Type of BSP information to visualize with the BSP diagnostic mode.
    m_bExportMIEnabled            false                      Flag that shows if MI file is exported after image is rendered.
    m_sMIFile                     (empty)                    Name of the exported MI file.
    m_iTileSize                   32                         Tile size to use during rendering.
    m_TileOrder                   krHilbert                  Tile order to use during rendering.
    m_iMemLimit                   1048                       Memory limit to allocate for rendering (in megabytes).
    m_fEnergyMult                 1.0f                       Energy multiplier to control the magnitude of indirect illumination.
    m_pProgressMonitor            NULL                       Interface pointer for the render driver to callback.
    m_ExposureType                krAutomatic                Exposure type.
    m_FGMode                      krFinalGatherAuto          Final gathering mode.
    m_dShadowMult                 1.0                        Shadows sampling multiplier.
    m_MIMode                      krExportMIOff              Export mode for the MI file.
    </table>
  */
  OdGiMentalRayRenderSettingsTraitsData()
    : OdGiRenderSettingsTraitsData()
    , m_iSamplingMin(-1)
    , m_iSamplingMax(0)
    , m_SamplingFilter(krBox)
    , m_dSamplingWidth(1.0)
    , m_dSamplingHeight(1.0)
    , m_fContrastR(0.1f)
    , m_fContrastG(0.1f)
    , m_fContrastB(0.1f)
    , m_fContrastA(0.1f)
    , m_ShadowMode(krSimple)
    , m_bShadowMapEnabled(true)
    , m_bRayTraceEnabled(true)
    , m_iRTReflection(2)
    , m_iRTRefraction(2)
    , m_iRTSum(4)
    , m_bGlobalIllum(false)
    , m_iGISampleCount(500)
    , m_bSampleRadEnabled(false)
    , m_dSampleRad(1.0)
    , m_iPhotonsPerLight(10000)
    , m_iPhotonsReflection(5)
    , m_iPhotonsRefraction(5)
    , m_iPhotonsSum(5)
    , m_bFGEnabled(false)
    , m_iFGRayCount(1000)
    , m_bFGRadMin(false)
    , m_bFGRadMax(false)
    , m_bFGPixels(false)
    , m_dFGSRadMin(1.0)
    , m_dFGSRadMax(1.0)
    , m_dLightLumScale(1.0)
    , m_DiagMode(krOff)
    , m_DiagGridMode(krObject)
    , m_fDiagGridSize(10.0f)
    , m_DiagPhtMode(krDensity)
    , m_DiagBSPMode(krDepth)
    , m_bExportMIEnabled(false)
    , m_sMIFile()
    , m_iTileSize(32)
    , m_TileOrder(krHilbert)
    , m_iMemLimit(1048)
    , m_fEnergyMult(1.0f)
    , m_pProgressMonitor(NULL)
    , m_ExposureType(krAutomatic)
    , m_FGMode(krFinalGatherAuto)
    , m_dShadowMult(1.0)
    , m_MIMode(krExportMIOff)
  {
  }

  /** \details
    Specifies the minimum and maximum numbers of samples that are used for pixel shading process during rendering.
      
    \param min [in]  Minimum sampling rate [-3..5]. Values less than zero enable subsampling. -3 corresponds to one sample for every 64 pixel (1/64) and 5 corresponds to 1024 samples per pixel. This parameter must be less or equal to the max parameter.
    \param max [in]  Maximal sampling rate [-3..5].  This parameter must be equal or greater than to the mix parameter. 
  */
  void setSampling(int min, int max)
  {
    m_iSamplingMin = min;
    m_iSamplingMax = max;
  }
  
  /** \details
    Retrieves the minimum and maximum numbers of samples that are used for pixel shading process during rendering.
      
    \param min [out]  Minimum sampling rate [-3..5]. Values less than zero enable subsampling. -3 corresponds to one sample for every 64 pixel (1/64) and 5 corresponds to 1024 samples per pixel. This parameter cannot be greater than the max parameter.
    \param max [out]  Maximal sampling rate [-3..5]. This parameter cannot be less than the min parameter.
  */
  void sampling(int& min, int& max) const
  {
    min = m_iSamplingMin;
    max = m_iSamplingMax;
  }
  
  /** \details
    Specifies the filtering parameters for merging multiple samples into a pixel color.
    
    \param filter [in]  Filtering parameters for merging multiple samples into a pixel color.
    \param width [in]  Width of a filter area for neighboring pixels in the range of [0..8]. 
    \param height [in]  Width of a filter area for neighboring pixels in the range of [0..8]. 
  */
  void setSamplingFilter(OdGiMrFilter filter, double width, double height)
  {
    m_SamplingFilter = filter;
    m_dSamplingWidth = width;
    m_dSamplingHeight = height;
  }
  
  /** \details
    Retrieves the filtering parameters for merging multiple samples into a pixel color.
      
    \param filter [out]  Filtering parameters for merging multiple samples into a pixel color.
    \param width [out]  Width of a filter area for neighboring pixels in the range of [0..8]. 
    \param height [out]  Width of a filter area for neighboring pixels in the range of [0..8]. 
  */
  void samplingFilter(OdGiMrFilter& filter, double& width, double& height) const
  {
    filter = m_SamplingFilter;
    width = m_dSamplingWidth;
    height = m_dSamplingHeight;
  }
  
  /** \details
    Specifies the threshold values for color channels below which further samples are not accepted.
    
    \param r [in]  Red channel.
    \param g [in]  Green channel.
    \param b [in]  Blue channel.
    \param a [in]  Alpha (transparency) channel.
    
    \remarks
    Each color channel value can be in the range [0.0..1.0].
  */
  void setSamplingContrastColor(float r, float g, float b, float a)
  {
    m_fContrastR = r;
    m_fContrastG = g;
    m_fContrastB = b;
    m_fContrastA = a;
  }
  
  /** \details
    Retrieves the threshold values for color channels below which further samples are not accepted.
    
    \param r [out]  Red channel.
    \param g [out]  Green channel.
    \param b [out]  Blue channel.
    \param a [out]  Alpha (transparency) channel.
    
    \remarks
    Each color channel value can be in the range [0.0..1.0].
  */
  void samplingContrastColor(float& r, float& g, float& b, float& a) const
  {
    r = m_fContrastR;
    g = m_fContrastG;
    b = m_fContrastB;
    a = m_fContrastA;
  }
  
  /** \details
    Specifies the method used to compute ray-traced shadows. Affects only lights using ray-traced shadows.
    
    \param mode [in]  Shadow mode.
  */
  void setShadowMode(OdGiMrShadowMode mode)
  {
    m_ShadowMode = mode;
  }
  
  /** \details
    Retrieves the method used to compute ray-traced shadows.
    
    \returns
    a value from the OdGiMrShadowMode which represents a shadow mode used for processing ray-traced shadows.
  */
  OdGiMrShadowMode shadowMode() const
  {
    return m_ShadowMode;
  }
  
  /** \details
    Specifies whether shadow maps should be enabled. Affects only lights using mapped shadows.
    
    \param enabled [in]  Flag that specifies whether shadow maps should be enabled.
  */
  void setShadowMapEnabled(bool enabled)
  {
    m_bShadowMapEnabled = enabled;
  }
  
  /** \details
    Checks whether shadow maps is enabled.
    
    \returns
    true if shadow mapping is enabled, false otherwise.
  */
  bool shadowMapEnabled() const
  {
    return m_bShadowMapEnabled;
  }
  
  /** \details
    Specifies whether ray tracing should be enabled.
    
    \param enabled [in]  Flag that specifies whether ray tracing should be enabled.
  */
  void setRayTraceEnabled(bool enabled)
  {
    m_bRayTraceEnabled = enabled;
  }
  
  /** \details
    Checks whether ray tracing is enabled.
    
    \returns
    true if ray tracing is enabled, false otherwise.
  */
  bool rayTraceEnabled() const
  {
    return m_bRayTraceEnabled;
  }
  
  /** \details
    Specifies the maximum depth (recursion depth) for traced rays.
    
    \param reflection [in]  Maximum recursion level of reflection rays, greater than or equal to 0.
    \param refraction [in]  Maximum recursion level of refraction rays, greater than or equal to 0.
    \param sum [in]  Maximum recursion level of reflection and refraction rays, combined.
  */
  void setRayTraceDepth(int reflection, int refraction, int sum)
  {
    m_iRTReflection = reflection;
    m_iRTRefraction = refraction;
    m_iRTSum = sum;
  }
  
  /** \details
    Retrieves the maximum depth (recursion depth) for traced rays.
    
    \param reflection [out]  Maximum recursion level of reflection rays, greater than or equal to 0.
    \param refraction [out]  Maximum recursion level of refraction rays, greater than or equal to 0.
    \param sum [out]  Maximum recursion level of reflection and refraction rays, combined.
  */
  void rayTraceDepth(int& reflection, int& refraction, int& sum) const
  {
    reflection = m_iRTReflection;
    refraction = m_iRTRefraction;
    sum = m_iRTSum;
  }
  
  /** \details
    Specifies whether global illumination effects (for example, color bleeding) should be enabled.
    
    \param enabled [in]  Flag that specifies whether global illumination should be enabled.
  */
  void setGlobalIlluminationEnabled(bool enabled)
  {
    m_bGlobalIllum = enabled;
  }
  
  /** \details
    Checks whether global illumination effects are enabled.
    
    \returns
    true if ray tracing is enabled, false otherwise.
  */
  bool globalIlluminationEnabled() const
  {
    return m_bGlobalIllum;
  }
  
  /** \details
    Specifies the maximum number of photons surrounding a render sample point to use for calculation of global illumination. 
    
    \param sum [in]  Number of photons around each render sample. Must be greater than zero.
  */
  void setGISampleCount(int num)
  {
    m_iGISampleCount = num;
  }
  
  /** \details
    Retrieves the maximum number of photons surrounding a render sample point to use for calculation of global illumination.
    
    \returns
    integer value that represents a number of photons around each render sample.
  */
  int giSampleCount() const
  {
    return m_iGISampleCount;
  }
  
  /** \details
    Specifies whether user-defined photon radius should be used. 
    
    \param enabled [in]  Flag that specifies whether to enable user-defined photon sampling radius.
  */
  void setGISampleRadiusEnabled(bool enabled)
  {
    m_bSampleRadEnabled = enabled;
  }
  
  /** \details
    Checks whether user-defined photon radius is enabled.
    
    \returns
    true if user-defined photon radius is enabled, false otherwise.
  */
  bool giSampleRadiusEnabled() const
  {
    return m_bSampleRadEnabled;
  }
  
  /** \details
    Specifies the radius of the sample area of photons used for global illumination. 
    
    \param radius [in]  Radius in model units. Must be greater than or equal to zero.
    
    \remarks
    Photons are searched for the area indicated around each render sample, up to the number of photons defined by setGIPhotonCount() method.
  */
  void setGISampleRadius(double radius)
  {
    m_dSampleRad = radius;
  }
  
  /** \details
    Retrieves the radius of the sample area of photons used for global illumination. 
    
    \returns
    radius of sample photons as a value of the double type.
  */
  double giSampleRadius() const
  {
    return m_dSampleRad;
  }
  
  /** \details
    Specifies the average number of photons to emit for each light source. 
    
    \param num [in]  Average number of photons to emit for each light. Must be greater than 0.
  */
  void setGIPhotonsPerLight(int num)
  {
    m_iPhotonsPerLight = num;
  }
  
  /** \details
    Retrieves the average number of photons that are emitted for each light source. 
    
    \returns
    average number of photons that are emitted for each light source.
  */
  int giPhotonsPerLight() const
  {
    return m_iPhotonsPerLight;
  }
  
  /** \details
    Specifies the maximum depth (recursion level) for traced photons. 
    
    \param reflection [in]  Maximum recursion level of reflection photons, greater than or equal to 0.
    \param refraction [in]  Maximum recursion level of refraction photons, greater than or equal to 0.
    \param sum [in]  Maximum recursion level of reflection and refraction photons, combined.
  */
  void setPhotonTraceDepth(int reflection, int refraction, int sum)
  {
    m_iPhotonsReflection = reflection;
    m_iPhotonsRefraction = refraction;
    m_iPhotonsSum = sum;
  }
  
  /** \details
  Retrieves the maximum depth (recursion depth) for traced photons.
    
    \param reflection [out]  Maximum recursion level of reflection photons, greater than or equal to 0.
    \param refraction [out]  Maximum recursion level of refraction photons, greater than or equal to 0.
    \param sum [out]  Maximum recursion level of reflection and refraction photons, combined.
  */
  void photonTraceDepth(int& reflection, int& refraction, int& sum) const
  {
    reflection = m_iPhotonsReflection;
    refraction = m_iPhotonsRefraction;
    sum = m_iPhotonsSum;
  }
  
  /** \details
    Specifies whether final gathering should be enabled.
    
    \param enabled [in]  Flag that specifies whether global illumination should be enabled.
    
    \remarks
    Final gathering enables indirect illumination effects such as color bleeding and can be combined with global illumination.
  */
  void setFinalGatheringEnabled(bool enabled)
  {
    m_bFGEnabled = enabled;
  }
  
  /** \details
    Checks whether final gathering is enabled.
    
    \returns
    true if final gathering is enabled, false otherwise.
    
    \remarks
    Final gathering enables indirect illumination effects such as color bleeding and can be combined with global illumination.
  */
  bool finalGatheringEnabled() const
  {
    return m_bFGEnabled;
  }
  
  /** \details
    Specifies the amount of final gather rays to use for each final gather point.
    
    \param enabled [in]  Amount of final gather rays to use for each final gather point. Must be greater than 0.
    
    \remarks
    Each final gather point receives the specified amount of rays to calculate indirect illumination.
  */
  void setFGRayCount(int num)
  {
    m_iFGRayCount = num;
  }
  
  /** \details
    Retrieves the amount of final gather rays used for each final gather point.
    
    \returns
    a number of final gather rays.
    
    \remarks
    Each final gather point receives the specified amount of rays to calculate indirect illumination.
  */
  int fgRayCount() const
  {
    return m_iFGRayCount;
  }
  
  /** \details
    Specifies whether user-defined radii are used for final gathering and the measurement units for the radii.
    
    \param bMin [in]  Flag that specifies whether the user-defined minimum radius is used. If set to false, a default radius is used.
    \param bMax [in]  Flag that specifies whether the user-defined maximum radius is used. If set to false, a default radius is used.
    \param bPixels [in]  Flag that specifies whether the user-defined radii are measured in pixel units.
  */
  void setFGRadiusState(bool bMin, bool bMax, bool bPixels)
  {
    m_bFGRadMin = bMin;
    m_bFGRadMax = bMax;
    m_bFGPixels = bPixels;
  }
  
  /** \details
    Checks whether user-defined radii are used for final gathering and their measurement units.
    
    \param bMin [out]  Flag that shows whether the user-defined minimum radius is used. If set to false, a default radius is used.
    \param bMax [out]  Flag that shows whether the user-defined maximum radius is used. If set to false, a default radius is used.
    \param bPixels [out]  Flag that shows whether the user-defined radii are measured in pixel units.
  */
  void fgSampleRadiusState(bool& bMin, bool& bMax, bool& bPixels)
  {
    bMin = m_bFGRadMin;
    bMax = m_bFGRadMax;
    bPixels = m_bFGPixels;
  }
  
  /** \details
    Specifies the minimum and maximum radii of the area used to sample final gather points to process indirect illumination.
    
    \param min [in]  Minimum final gather sample radius. To define measurement units for this parameter, use the setFGRadiusState() method. Must be greater than 0 but less than or equal to the max parameter.
    \param max [in]  Maximum final gather sample radius. To define measurement units for this parameter, use the setFGRadiusState() method. Must be greater than 0 and greater than or equal to the min parameter.
  */
  void setFGSampleRadius(double min, double max)
  {
    m_dFGSRadMin = min;
    m_dFGSRadMax = max;
  }
  
  /** \details
    Retrieves the minimum and maximum radii of the area used to sample final gather points to process indirect illumination.
    
    \param min [out]  Receives minimum final gather sample radius. Measurement units for this parameter can be retrieved using the setFGRadiusState() method.
    \param max [out]  Receives maximum final gather sample radius. Measurement units for this parameter can be retrieved using the setFGRadiusState() method.
  */
  void fgSampleRadius(double& min, double& max) const
  {
    min = m_dFGSRadMin;
    max = m_dFGSRadMax;
  }
  
  /** \details
    Specifies the physical luminance of a light with intensity 1.0 (in candelas).
    
    \param luminance [in]  Physical luminance of a light with intensity 1.0 (in candelas), used as a physical scaling factor.
  */
  void setLightLuminanceScale(double luminance)
  {
    m_dLightLumScale = luminance;
  }
  
  /** \details
    Retrieves the physical luminance of a light with intensity 1.0 (in candelas).
    
    \returns
    physical luminance of a light.
  */
  double lightLuminanceScale() const
  {
    return m_dLightLumScale;
  }
  
  /** \details
    Specifies a mode used to process diagnostic images.
    
    \param mode [in]  Mode used to process diagnostic images.
  */
  void setDiagnosticMode(OdGiMrDiagnosticMode mode)
  {
    m_DiagMode = mode;
  }
  
  /** \details
    Retrieves a mode used to process diagnostic images.
    
    \returns
    physical luminance of a light.
  */
  OdGiMrDiagnosticMode diagnosticMode() const
  {
    return m_DiagMode;
  }
  
  /** \details
    Specifies the coordinate system for the diagnostic grid and the distance between grid lines (size). 
    
    \param mode [in]  Type of a diagnostic grid.
    \param fSize [in]  Distance between grid lines. Must be greater than 0.
  */
  void setDiagnosticGridMode(OdGiMrDiagnosticGridMode mode, float fSize)
  {
    m_DiagGridMode = mode;
    m_fDiagGridSize = fSize;
  }
  
  /** \details
    Retrieves the coordinate system for the diagnostic grid and the distance between grid lines (size). 
    
    \param mode [out]  Receives the type of a diagnostic grid.
    \param fSize [out]  Receives the distance between grid lines. Must be greater than 0.
  */
  void diagnosticGridMode(OdGiMrDiagnosticGridMode& mode, float& fSize) const
  {
    mode = m_DiagGridMode;
    fSize = m_fDiagGridSize;
  }
  
  /** \details
    Specifies the type of photon information to render images within the photon diagnostic mode.
    
    \param mode [in]  Photon diagnostic mode.
  */
  void setDiagnosticPhotonMode(OdGiMrDiagnosticPhotonMode mode)
  {
    m_DiagPhtMode = mode;
  }
  
  /** \details
    Retrieves the type of photon information to render images within the photon diagnostic mode.
    
    \returns
    the type of photon information to to process images within the photon diagnostic mode as OdGiMrDiagnosticPhotonMode value.
  */
  OdGiMrDiagnosticPhotonMode diagnosticPhotonMode() const
  {
    return m_DiagPhtMode;
  }
  
  /** \details
    Specifies the type of BSP information to render images within the BSP diagnostic mode.
    
    \param mode [in]  BSP diagnostic mode.
  */
  void setDiagnosticBSPMode(OdGiMrDiagnosticBSPMode mode)
  {
    m_DiagBSPMode = mode;
  }
  
  /** \details
    Retrieves the type of BSP information to render images within the BSP diagnostic mode.
    
    \returns
    the type of BSP information to to process images within the BSP diagnostic mode as OdGiMrDiagnosticBSPMode value.
  */
  OdGiMrDiagnosticBSPMode diagnosticBSPMode() const
  {
    return m_DiagBSPMode;
  }
  
  /** \details
    Specifies whether to export MI file after image is rendered.
    
    \param mode [in]  Flag that specifies whether to export MI file after image is rendered.
  */
  void setExportMIEnabled(bool enabled)
  {
    m_bExportMIEnabled = enabled;
  }
  
  /** \details
    Checks whether MI file is enabled after image rendering.
    
    \returns
    flag that specifies whether MI file is enabled after image rendering.
  */
  bool exportMIEnabled() const
  {
    return m_bExportMIEnabled;
  }
  
  /** \details
    Specifies a file name for an exported MI file.
    
    \param miName [in]  Name for an exported MI file.
  */
  void setExportMIFileName(const OdString& miName)
  {
    m_sMIFile = miName;
  }
  
  /** \details
    Retrieves a file name for an exported MI file.
    
    \returns
    file name for an exported MI file as OdString value.
  */
  const OdString &exportMIFileName() const
  {
    return m_sMIFile;
  }
  
  /** \details
    Specifies the size of the image tiles used during rendering. 
    
    \param size [in]  Size of image tiles used during rendering.
    
    \remarks
    Small values increase rendering time, however provide more frequent updates during rendering. 
    The values must be in range [4..512]. 
  */
  void setTileSize(int size)
  {
    m_iTileSize = size;
  }
  
  /** \details
    Retrieves the size of the image tiles used during rendering. 
    
    \returns
    size of the image tiles used during rendering in range of [4..512].
  */
  int tileSize() const
  {
    return m_iTileSize;
  }
  
  /** \details
    Specifies the order used to render image tiles. 
    
    \param order [in]  Order used to render image tiles.
  */
  void setTileOrder(OdGiMrTileOrder order)
  {
    m_TileOrder = order;
  }
  
  /** \details
    Retrieves the order used to render image tiles. 
    
    \returns
    order used to render image tiles as OdGiMrTileOrder value.
  */
  OdGiMrTileOrder tileOrder() const
  {
    return m_TileOrder;
  }
  
  /** \details
    Specifies the maximum amount of memory that can be allocated for rendering. 
    
    \param limit [in]  maximum amount of memory for rendering in MB.
  */
  void setMemoryLimit(int limit)
  {
    m_iMemLimit = limit;
  }
  
  /** \details
    Retrieves the maximum amount of memory that can be allocated for rendering. 
    
    \returns
    maximum amount of memory that can be allocated for rendering as integer value.
  */
  int memoryLimit() const
  {
    return m_iMemLimit;
  }
  
  /** \details
    Specifies the energy multiplier to control the magnitude of indirect illumination. 
    
    \param fScale [in]  Scale factor.
  */
  void setEnergyMultiplier(float fScale)
  {
    m_fEnergyMult = fScale;
  }
  
  /** \details
    Receives the energy multiplier to control the magnitude of indirect illumination. 
    
    \returns
    scale factor to control magnitude of indirect illumination.
  */
  float energyMultiplier() const
  {
    return m_fEnergyMult;
  }
  
  /** \details
    Specifies the interface pointer for the render driver to callback. 
    
    \param pMonitor [in]  Void pointer that caller provides for the render driver to callback.
  */
  void setProgressMonitor(void* pMonitor)
  {
    m_pProgressMonitor = pMonitor;
  }
  
  /** \details
    Retrieves a pointer to the progress monitor. 
    
    \returns
    a pointer to a void type that can be casted later to a type of a corresponding process monitor.
  */
  const void* progressMonitor() const
  {
    return m_pProgressMonitor;
  }
  
  /** \details
    Specifies the exposure type. 
    
    \param type [in]  Exposure type.
  */
  void setExposureType(OdGiMrExposureType type)
  {
    m_ExposureType = type;
  }
  
  /** \details
    Retrieves the exposure type. 
    
    \returns
    exposure type as a value from the OdGiMrExposureType enumeration.
  */
  OdGiMrExposureType exposureType() const
  {
    return m_ExposureType;
  }
  
  /** \details
    Specifies the final gathering mode. 
    
    \param mode [in]  Final gathering mode. 
    
    \remarks
    Final gathering enables indirect illumination effects such as color bleeding and can be combined with global illumination.
  */
  void setFinalGatheringMode(OdGiMrFinalGatheringMode mode)
  {
    m_FGMode = mode;
  }
  
  /** \details
    Retrieves the final gathering mode. 
    
    \returns
    final gathering mode as a value from OdGiMrFinalGatheringMode enumeration.
    
    \remarks
    Final gathering enables indirect illumination effects such as color bleeding and can be combined with global illumination.
  */
  OdGiMrFinalGatheringMode finalGatheringMode() const
  {
    return m_FGMode;
  }
  
  /** \details
    Specifies shadow sampling multiplier value.
    
    \param multiplier [in]  Multiplier value for shadows. 
  */
  void setShadowSamplingMultiplier(double multiplier)
  {
    m_dShadowMult = multiplier;
  }
  
  /** \details
    Retrieves shadow sampling multiplier value.
    
    \returns
    multiplier value for shadows. 
  */
  double shadowSamplingMultiplier() const
  {
    return m_dShadowMult;
  }
  
  /** \details
    Specifies an export type for MI file.
    
    \param mode [in]  Defines export mode for MI files. 
  */
  void setExportMIMode(OdGiMrExportMIMode mode)
  {
    m_MIMode = mode;
  }
  
  /** \details
    Retrieves an export type for MI file.
    
    \returns
    an export mode for MI file as a value from the OdGiMrExportMIMode enumeration.
  */
  OdGiMrExportMIMode exportMIMode() const
  {
    return m_MIMode;
  }

  /** \details
    Checks whether the passed render traits are not equal to these traits.
    
    \param data2 [in]  Traits to compare.
    \returns
    true if traits are not equal, false otherwise.
  */
  bool operator !=(const OdGiMentalRayRenderSettingsTraitsData &data2) const
  {
    return ((*static_cast<const OdGiRenderSettingsTraitsData*>(this) != static_cast<const OdGiRenderSettingsTraitsData&>(data2)) ||
           (m_iSamplingMin != data2.m_iSamplingMin) ||
           (m_iSamplingMax != data2.m_iSamplingMax) ||
           (m_SamplingFilter != data2.m_SamplingFilter) ||
           !OdEqual(m_dSamplingWidth, data2.m_dSamplingWidth) ||
           !OdEqual(m_dSamplingHeight, data2.m_dSamplingHeight) ||
           (m_fContrastR != data2.m_fContrastR) ||
           (m_fContrastG != data2.m_fContrastG) ||
           (m_fContrastB != data2.m_fContrastB) ||
           (m_fContrastA != data2.m_fContrastA) ||
           (m_ShadowMode != data2.m_ShadowMode) ||
           (m_bShadowMapEnabled != data2.m_bShadowMapEnabled) ||
           (m_bRayTraceEnabled != data2.m_bRayTraceEnabled) ||
           (m_iRTReflection != data2.m_iRTReflection) ||
           (m_iRTRefraction != data2.m_iRTRefraction) ||
           (m_iRTSum != data2.m_iRTSum) ||
           (m_bGlobalIllum != data2.m_bGlobalIllum) ||
           (m_iGISampleCount != data2.m_iGISampleCount) ||
           (m_bSampleRadEnabled != data2.m_bSampleRadEnabled) ||
           !OdEqual(m_dSampleRad, data2.m_dSampleRad) ||
           (m_iPhotonsPerLight != data2.m_iPhotonsPerLight) ||
           (m_iPhotonsReflection != data2.m_iPhotonsReflection) ||
           (m_iPhotonsRefraction != data2.m_iPhotonsRefraction) ||
           (m_iPhotonsSum != data2.m_iPhotonsSum) ||
           (m_bFGEnabled != data2.m_bFGEnabled) ||
           (m_iFGRayCount != data2.m_iFGRayCount) ||
           (m_bFGRadMin != data2.m_bFGRadMin) ||
           (m_bFGRadMax != data2.m_bFGRadMax) ||
           (m_bFGPixels != data2.m_bFGPixels) ||
           !OdEqual(m_dFGSRadMin, data2.m_dFGSRadMin) ||
           !OdEqual(m_dFGSRadMax, data2.m_dFGSRadMax) ||
           !OdEqual(m_dLightLumScale, data2.m_dLightLumScale) ||
           (m_DiagMode != data2.m_DiagMode) ||
           (m_DiagGridMode != data2.m_DiagGridMode) ||
           (m_fDiagGridSize != data2.m_fDiagGridSize) ||
           (m_DiagPhtMode != data2.m_DiagPhtMode) ||
           (m_DiagBSPMode != data2.m_DiagBSPMode) ||
           (m_bExportMIEnabled != data2.m_bExportMIEnabled) ||
           (m_sMIFile != data2.m_sMIFile) ||
           (m_iTileSize != data2.m_iTileSize) ||
           (m_TileOrder != data2.m_TileOrder) ||
           (m_iMemLimit != data2.m_iMemLimit) ||
           (m_fEnergyMult != data2.m_fEnergyMult) ||
           (m_pProgressMonitor != data2.m_pProgressMonitor) ||
           (m_ExposureType != data2.m_ExposureType) ||
           (m_FGMode != data2.m_FGMode) ||
           !OdEqual(m_dShadowMult, data2.m_dShadowMult) ||
           (m_MIMode != data2.m_MIMode));
  }
  
  /** \details
    Checks whether the passed render traits are equal to these traits.
    
    \param data2 [in]  Traits to compare.
    \returns
    true if traits are not equal, false otherwise.
  */
  bool operator ==(const OdGiMentalRayRenderSettingsTraitsData &data2) const
  {
    return !(*this != data2);
  }
};

/** \details
    Corresponding C++ library: TD_Gi
    <group OdGi_Classes> 
*/
class OdGiRapidRTRenderSettingsTraitsData : public virtual OdGiRenderSettingsTraitsData
{
  OdGiQuitCondition          m_QuitCondition;
  int                        m_iRenderLevel;
  int                        m_iRenderTime;
  OdGiLightingMode           m_LightingModel;
  OdGiFilterType             m_FilterType;
  float                      m_fFilterWidth;
  float                      m_fFilterHeight;
public:
  
  /** \details
    Default constructor for the OdGiRapidRTRenderSettingsTraitsData class. Sets the following parameters:
    <table>
    <B>Data member</B>       <B>Value</B>           <B>Description</B>
    m_QuitCondition           krEQuitByRenderLevel  Rendering quit condition.
    m_iRenderLevel            1                     Render level required to quit the rendering process.
    m_iRenderTime             1                     Render time required to quit the rendering process (in minutes).
    m_LightingModel           krEBasic              Light mode.
    m_FilterType              krEBox                Filter type.
    m_fFilterWidth            1.0f                  Filter width in pixels.
    m_fFilterHeight           1.0f                  Filter height in pixels.
    </table>
  */
  OdGiRapidRTRenderSettingsTraitsData()
    : OdGiRenderSettingsTraitsData()
    , m_QuitCondition(krEQuitByRenderLevel)
    , m_iRenderLevel(1)
    , m_iRenderTime(1)
    , m_LightingModel(krEBasic)
    , m_FilterType(krEBox)
    , m_fFilterWidth(1.0f)
    , m_fFilterHeight(1.0f)
  {
  }

  /** \details
    Specifies quit condition for rendering process. 
    
    \param condition [in]  Quit condition.
  */
  void setQuitCondition(OdGiQuitCondition condition)
  {
    m_QuitCondition = condition;
  }
  
  /** \details
    Retrieves a condition that is used to quit the rendering process. 
    
    \returns
    quit condition as a value from the OdGiQuitCondition enumeration.
  */
  OdGiQuitCondition quitCondition() const
  {
    return m_QuitCondition;
  }
  
  /** \details
    Specifies the desired render level. Rendering quits once desired render quality is reached. This works only when the OdGiQuitCondition::krEQuitByRenderLevel value was passed to the setQuitCondition() method.
    
    \param level [in]  Desired render level in range [1..50].
  */
  void setDesiredRenderLevel(int level)
  {
    m_iRenderLevel = level;
  }
  
  /** \details
    Retrieves the desired render level. Rendering quits once desired render quality is reached. This works only when the OdGiQuitCondition::krEQuitByRenderLevel value was passed to the setQuitCondition() method.
    
    \returns
    desired render level in range [1..50].
  */
  int desiredRenderLevel() const
  {
    return m_iRenderLevel;
  }
  
  /** \details
    Specifies the desired render time. Rendering quits once rendering time limit is exceeded. This works only when the OdGiQuitCondition::krEQuitByRenderTime value was passed to the setQuitCondition() method.
    
    \param time [in]  Desired render time in range [1..1440]. Render time is set in minutes.
  */
  void setDesiredRenderTime(int time)
  {
    m_iRenderTime = time;
  }
  
  /** \details
    Retrieves the desired render time. Rendering quits once rendering time limit is exceeded. This works only when the OdGiQuitCondition::krEQuitByRenderTime value was passed to the setQuitCondition() method.
    
    \returns
    desired render time in minutes.
  */
  int desiredRenderTime() const
  {
    return m_iRenderTime;
  }
  
  /** \details
    Specifies a lighting mode.
    
    \param mode [in]  Lighting mode.
    
    \remarks
    You can set whether a simplistic, basic or advanced Lighting mode. When simplistic mode is used, the Global illumination and Glossy reflection/refraction is turned off. 
    Basic lighting uses ambient occlusion and a simpler model for ambient light and image-based lighting (IBL). 
    Advanced lighting uses full global illumination and a more advanced model for ambient light and IBL.
  */
  void setLightingMode(OdGiLightingMode mode)
  {
    m_LightingModel = mode;
  }
  
  /** \details
    Retrieves a lighting mode.
    
    \returns
    a lighting mode as a value from the OdGiLightingMode enumeration.
    
    \remarks
    There are a simplistic, basic or advanced Lighting mode. When simplistic mode is used, the Global illumination and Glossy reflection/refraction is turned off.
    Basic lighting uses ambient occlusion and a simpler model for ambient light and image-based lighting (IBL). 
    Advanced lighting uses full global illumination and a more advanced model for ambient light and IBL.
  */
  OdGiLightingMode lightingMode() const
  {
    return m_LightingModel;
  }
  
  /** \details
    Specifies the filter type applied to the image samples during sampling.
    
    \param type [in]  Filter type.
  */
  void setFilterType(OdGiFilterType type)
  {
    m_FilterType = type;
  }
  
  /** \details
    Retrieves the filter type applied to the image samples during sampling.
    
    \returns
    a filter type.
  */
  OdGiFilterType filterType() const
  {
    return m_FilterType;
  }
  
  /** \details
    Specifies the filter width.
    
    \param width [in]  Filter width in pixels (image samples).
    
    \remarks
    There are different default sizes, recommended to different filter types:
    <table>
    Filter            Value
    box               1.0
    triangle          2.0
    Gaussian          3.0
    Lanczos           4.0
    Mitchell          4.0
    </table>
  */
  void setFilterWidth(float width)
  {
    m_fFilterWidth = width;
  }
  
  /** \details
    Retrieves the filter width in pixels.
    
    \returns
    filter width.
  */
  float filterWidth() const
  {
    return m_fFilterWidth;
  }
  
  /** \details
    Specifies the filter height.
    
    \param height [in]  Filter height in pixels (image samples).
    
    \remarks
    There are different default sizes, recommended to different filter types:
    <table>
    Filter            Value
    box               1.0
    triangle          2.0
    Gaussian          3.0
    Lanczos           4.0
    Mitchell          4.0
    </table>
  */
  void setFilterHeight(float height)
  {
    m_fFilterHeight = height;
  }
  
  /** \details
    Retrieves the filter height in pixels.
    
    \returns
    filter height.
  */
  float filterHeight() const
  {
    return m_fFilterHeight;
  }

  /** \details
    Checks whether the passed traits are not equal to these traits.
    Traits are not equal if one of the following conditions fails:
    * Quit conditions are equal;
    * Levels to quit rendering are equal;
    * Times to quit rendering are equal;
    * Light models are equal;
    * Filter types are equal;
    * Filter width values are equal;
    * Filter height values are equal;
    
    \param data2 [in]  Traits to compare.
    
    \returns
    true if background traits are not equal, false otherwise.
  */
  bool operator !=(const OdGiRapidRTRenderSettingsTraitsData &data2) const
  {
    return ((*static_cast<const OdGiRenderSettingsTraitsData*>(this) != static_cast<const OdGiRenderSettingsTraitsData&>(data2)) ||
      (m_QuitCondition != data2.m_QuitCondition) ||
      (m_iRenderLevel != data2.m_iRenderLevel) ||
      (m_iRenderTime != data2.m_iRenderTime) ||
      (m_LightingModel != data2.m_LightingModel) ||
      (m_FilterType != data2.m_FilterType) ||
      (m_fFilterWidth != data2.m_fFilterWidth) ||
      (m_fFilterHeight != data2.m_fFilterHeight));
  }
  
  /** \details
    Checks whether the passed traits are equal to these traits.
    Traits are equal if one of the following conditions fails:
    * Quit conditions are equal;
    * Levels to quit rendering are equal;
    * Times to quit rendering are equal;
    * Light models are equal;
    * Filter types are equal;
    * Filter width values are equal;
    * Filter height values are equal;
    
    \param data2 [in]  Traits to compare.
    
    \returns
    true if background traits are equal, false otherwise.
  */
  bool operator ==(const OdGiRapidRTRenderSettingsTraitsData &data2) const
  {
    return !(*this != data2);
  }
};

/** \details
    Corresponding C++ library: TD_Gi
    <group OdGi_Classes> 
*/
class OdGiCombinedRenderSettingsTraitsData : public OdGiMentalRayRenderSettingsTraitsData
                                           , public OdGiRapidRTRenderSettingsTraitsData
{
  OdUInt32 m_kindOfRenderSettings;
public:
  /** \details
    Default constructor for the OdGiCombinedRenderSettingsTraitsData class.
  */
  OdGiCombinedRenderSettingsTraitsData()
    : m_kindOfRenderSettings(0)
  {
  }

  /** \details
    Represents render settings that are used for rendering.
  */
  enum KindOfRenderSettings
  {
    /** Mental ray renderer settings. */
    kMentalRaySettings = 1,
    /** RapidRT renderer settings. */
    kRapidRTSettings   = 2
  };
  
  /** \details
    Specifies which render settings are used.
    
    \param nFlags [in]  Value to define render settings. 1 - Mental Ray renderer, 2 - Rapid RT renderer.
  */
  void setKindOfRenderSettings(OdUInt32 nFlags)
  {
    m_kindOfRenderSettings = nFlags;
  }
  
  /** \details
    Retrieves a value that represents a type of render settings.
    
    \returns
    value that defines render settings. 1 - Mental Ray renderer, 2 - Rapid RT renderer.
  */
  OdUInt32 kindOfRenderSettings() const
  {
    return m_kindOfRenderSettings;
  }
  
  /** \details
    Modifies which render settings are currently used.
    
    \param nFlags [in]  Value to define render settings. 1 - Mental Ray renderer, 2 - Rapid RT renderer.
    \param bOp [in]  Flag that specifies whether the passed render should be used.
  */
  void modifyKindOfRenderSettings(OdUInt32 nFlags, bool bOp = true)
  {
    SETBIT(m_kindOfRenderSettings, nFlags, bOp);
  }

  /** \details
    Checks whether mental ray render settings are used.
    
    \returns
    true if mental ray settings are used, false otherwise.
  */
  bool hasMentalRayRenderSettings() const
  {
    return GETBIT(m_kindOfRenderSettings, kMentalRaySettings);
  }
  
  /** \details
    Checks whether RapidRT render settings are used.
    
    \returns
    true if RapidRT render settings are used, false otherwise.
  */
  bool hasRapidRTRenderSettings() const
  {
    return GETBIT(m_kindOfRenderSettings, kRapidRTSettings);
  }

  /** \details
    Checks whether the passed render traits are not equal to these render traits.
    Traits are not equal if one of the following conditions fails:
    * Mental Ray render settings of the passed object are equal to Mental Ray render settings of the this object;
    * RapidRT render settings of the passed object are equal to RapidRT render settings of this object;
    * Types of render settings are equal;
    
    \param data2 [in]  Traits to compare.
    
    \returns
    true if traits are not equal, false otherwise.
  */
  bool operator !=(const OdGiCombinedRenderSettingsTraitsData &data2) const
  {
    return ((*static_cast<const OdGiMentalRayRenderSettingsTraitsData*>(this) != static_cast<const OdGiMentalRayRenderSettingsTraitsData&>(data2)) ||
            (*static_cast<const OdGiRapidRTRenderSettingsTraitsData*>(this) != static_cast<const OdGiRapidRTRenderSettingsTraitsData&>(data2)) ||
            (m_kindOfRenderSettings != data2.m_kindOfRenderSettings));
  }
  
  /** \details
    Checks whether the passed render traits are equal to these render traits.
    Traits are equal if:
    * Mental Ray render settings of the passed object are equal to Mental Ray render settings of the this object;
    * RapidRT render settings of the passed object are equal to RapidRT render settings of this object;
    * Types of render settings are equal;
    
    \param data2 [in]  Traits to compare.
    
    \returns
    true if traits are equal, false otherwise.
  */
  bool operator ==(const OdGiCombinedRenderSettingsTraitsData &data2) const
  {
    return !(*this != data2);
  }
};

#include "TD_PackPop.h"

#endif // __OD_GI_ENVIRONMENT_TRAITS_DATA__
