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

#ifndef _GIENVIRONMENT_H_INCLUDED_
#define _GIENVIRONMENT_H_INCLUDED_

#include "Gi.h"
#include "GiMaterial.h"

class OdGiSkyParameters;

/** \details
    Represents the render environment which is fog and environment map in the display system. 
    
    Corresponding C++ library: TD_Gi
    <group OdGi_Classes> 
*/
class ODRX_ABSTRACT FIRSTDLL_EXPORT OdGiRenderEnvironmentTraits : public OdGiDrawableTraits
{
public:
  ODRX_DECLARE_MEMBERS(OdGiRenderEnvironmentTraits);

  /** \details
    Enables or disables the fog rendering.
      
    \param bEnable [in]  Flag that specifies whether to enable fog rendering.
  */
  virtual void setEnable(bool bEnable) = 0;
  
  /** \details
    Checks whether fog rendering is enabled.
      
    \returns
    true if fog rendering is enabled, false otherwise.
  */
  virtual bool enable() const = 0;
  
  /** \details
    Specifies whether fog should be treated as a background.
      
    \param bEnable [in]  Flag that specifies whether to treat fog as background.
  */
  virtual void setIsBackground(bool bEnable) = 0;
  
  /** \details
    Checks whether fog is treated background.
      
    \returns
    true if fog is treated as background, false otherwise.
  */
  virtual bool isBackground() const = 0;
  
  /** \details
    Sets background color.
      
    \param color [in]  Fog color.
  */
  virtual void setFogColor(const OdCmEntityColor& color) = 0;
  
  /** \details
    Retrieves fog color.
      
    \returns
    fog color as OdCmEntityColor instance.
  */
  virtual OdCmEntityColor fogColor() const = 0;
  
  /** \details
    Specifies near distance for the fog rendering.
      
    \param nearDist [in]  Near distance of the fog.
  */
  virtual void setNearDistance(double nearDist) = 0;
  
  /** \details
    Retrieves near distance of fog rendering.
      
    \returns
    fog rendering near distance.
  */
  virtual double nearDistance() const = 0;
  
  /** \details
    Specifies far distance for the fog rendering.
      
    \param farDist [in]  Far distance of the fog.
  */
  virtual void setFarDistance(double farDist) = 0;
  
  /** \details
    Retrieves far distance of fog rendering.
      
    \returns
    fog rendering far distance.
  */
  virtual double farDistance() const = 0;
  
  /** \details
    Specifies near percentage of the fog.
      
    \param nearPct [in]  Near percentage of the fog.
  */
  virtual void setNearPercentage(double nearPct) = 0;
  
  /** \details
    Retrieves near percentage of the fog.
      
    \returns
    near percentage of the fog.
  */
  virtual double nearPercentage() const = 0;
  
  /** \details
    Specifies far percentage of the fog.
      
    \param farPct [in]  Far percentage of the fog.
  */
  virtual void setFarPercentage(double farPct) = 0;
  
  /** \details
    Retrieves far percentage of the fog.
      
    \returns
    far percentage of the fog.
  */
  virtual double farPercentage() const = 0;
  
  /** \details
    Sets the environment map.
      
    \param m [in]  Pointer to the environment map.
  */
  virtual void setEnvironmentMap(const OdGiMaterialTexture* m) = 0;
  
  /** \details
    Retrieves the environment map.
      
    \returns
    pointer to the OdGiMaterialTexture instance that represents environment map.
  */
  virtual OdGiMaterialTexture* environmentMap() const = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGiRenderEnvironmentTraits object pointers.
*/
typedef OdSmartPtr<OdGiRenderEnvironmentTraits> OdGiRenderEnvironmentTraitsPtr;


/** \details
    Represents the render settings for the display system.

    Corresponding C++ library: TD_Gi
    <group OdGi_Classes> 
*/
class ODRX_ABSTRACT FIRSTDLL_EXPORT OdGiRenderSettingsTraits : public OdGiDrawableTraits
{
public:
  ODRX_DECLARE_MEMBERS(OdGiRenderSettingsTraits);

  /** \details
    Specifies whether per-object materials should be used. 
      
    \param enabled [in]  Flag that specifies whether per-object materials should be used.
    
    \remarks
    If set to false, the global material is used for all objects.
  */
  virtual void setMaterialEnabled(bool enabled) = 0;
  
  /** \details
    Checks whether per-object materials are used.
      
    \returns
    true if per-object materials are used.
  */
  virtual bool materialEnabled() const = 0;
  
  /** \details
    Specifies whether texture sampling (filtering) should be used for image textures. 
      
    \param enabled [in]  Flag that specifies whether sampling should be used.
  */
  virtual void setTextureSampling(bool enabled) = 0;
  
  /** \details
    Checks whether texture sampling (filtering) is used for image textures.
      
    \returns
    true if texture sampling is used, false otherwise.
    
    \remarks
    texture sampling is not applied to textured created in a procedural way.
  */
  virtual bool textureSampling() const = 0;
  
  /** \details
    Specifies whether back faces should be rendered. 
      
    \param enabled [in]  Flag that specifies whether back faces should be rendered.
  */
  virtual void setBackFacesEnabled(bool enabled) = 0;
  
  /** \details
    Checks whether back faces rendering is enabled.
      
    \returns
    true if back faces rendering is enabled, false otherwise.
  */
  virtual bool backFacesEnabled() const = 0;
  
  /** \details
    Specifies whether shadows should be rendered. 
      
    \param enabled [in]  Flag that specifies whether shadows should be rendered.
  */
  virtual void setShadowsEnabled(bool enabled) = 0;
  
  /** \details
    Checks whether shadows rendering is enabled.
      
    \returns
    true if shadows rendering is enabled, false otherwise.
  */
  virtual bool shadowsEnabled() const = 0;
  
  /** \details
    Specifies whether checkerboard (diagnostic) background should be used for rendering. 
      
    \param enabled [in]  Flag that specifies whether checkerboard background should be used.
  */
  virtual void setDiagnosticBackgroundEnabled(bool enabled) = 0;
  
  /** \details
    Checks whether checkerboard (diagnostic) background is enabled.
      
    \returns
    true if diagnostic background rendering is enabled, false otherwise.
  */
  virtual bool diagnosticBackgroundEnabled() const = 0;
  
  /** \details
    Specifies model scale related to meters. 
      
    \param scaleFactor [in]  Scale factor for a model.
    
    \remarks
    For example, if the model is drawn in kilometers, the scale factor is 1000.0. This is later used for the global illumination.
  */
  virtual void setModelScaleFactor(double scaleFactor) = 0;
  
  /** \details
    Retrieves model scale related to meters.
      
    \returns
    scale factor as a value of the double type. For example if model is drawn in kilometers, the returned value is 1000.0.
  */
  virtual double modelScaleFactor() const = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGiRenderSettingsTraits object pointers.
*/
typedef OdSmartPtr<OdGiRenderSettingsTraits> OdGiRenderSettingsTraitsPtr;


typedef enum OdGiMrFilter_
{
  /** Box filter. */
  krBox         = 0,
  /** Triangle filter. */
  krTriangle    = 1,
  /** Gauss filter. */
  krGauss       = 2,
  /** Mitchell filter. */
  krMitchell    = 3,
  /** Lanczos filter. */
  krLanczos     = 4
} 
OdGiMrFilter; // Represents filtering methods for merging samples into a pixel color.

typedef enum OdGiMrShadowMode_
{
  /** Simple shadows, no sorting of shadow rays is performed. */
  krSimple      = 0,
  /** Shadow sorting is performed, from sample point to light. */
  krSorted      = 1,
  /** Shadow segmenting is performed, from light to sample point. */
  krSegments    = 2
} 
OdGiMrShadowMode; // Represents available methods for processing ray-traced shadows.

typedef enum OdGiMrDiagnosticMode_
{
  /** Render normally.*/
  krOff         = 0,
  /** Render a grid on top of all objects in the scene, in the coordinate system specified with a value from OdGiMrDiagnosticGridMode enum. */
  krGrid        = 1,
  /** Render a false-color image of photon density or photon irradiance on all surfaces, as specified with value from OdGiMrDiagnosticPhotonMode enum. Higher density or irradiance is shown red, and lower values are represented with blue.*/
  krPhoton      = 2,
  /** Render a grayscale image illustrating sample density, which is controlled with the OdGiMentalRayRenderSettingsTraits::setSampling() method. Higher levels of sampling are shown with brighter shades of gray.*/
  krSamples     = 3,
  /** Render a false-color image illustrating the cost of creating and traversing the BSP tree used for ray-tracing. Both the depth and the leaf size can be visualized, as specified with value from OdGiMrDiagnosticBSPMode enum.*/
  krBSP         = 4
} 
OdGiMrDiagnosticMode; // Represents available diagnostic visualization modes.

typedef enum OdGiMrDiagnosticGridMode_
{
  /** Object-space coordinate system.*/
  krObject      = 0,
  /** World-space coordinate system.*/
  krWorld       = 1,
  /** Camera-space (view-space) coordinate system.*/
  krCamera      = 2
}
OdGiMrDiagnosticGridMode; // Represents available coordinate systems to use for the diagnostic grid.

typedef enum OdGiMrDiagnosticPhotonMode_
{
  /** Photon density is displayed, where higher density values are shown red and lower values are shown in red*/
  krDensity     = 0,
  /** Photon irradiance is displayed, where higher irradiance values are shown red and lower values are shown in red.*/
  krIrradiance  = 1
} 
OdGiMrDiagnosticPhotonMode; // Represents the type of photon information that is used in the photon diagnostic mode.

typedef enum OdGiMrDiagnosticBSPMode_
{
  /** BSP depth: the depth of the BSP tree reached.*/
  krDepth       = 0,
  /** BSP leaf size: the leaf size in the BSP tree reached.*/
  krSize        = 1
}
OdGiMrDiagnosticBSPMode; // Represents the available types of BSP information to visualize with the BSP diagnostic mode. 

typedef enum OdGiMrTileOrder_
{
  /** Hilbert order.*/
  krHilbert       = 0,
  /** Spiral order.*/
  krSpiral        = 1,
  /** Left-to-right order.*/
  krLeftToRight   = 2,
  /** Right-to-left order.*/
  krRightToLeft   = 3,
  /** Top-to-bottom order.*/
  krTopToBottom   = 4,
  /** Bottom-to-top order.*/
  krBottomToTop   = 5
}
OdGiMrTileOrder; // Represents the available sequences for rendered image tiles that can be followed.

typedef enum OdGiMrExposureType_
{
  /** The exposure type is determined with an automatic exposure.*/
  krAutomatic     = 0,
  /** The exposure type is determined on a logarithmic scale.*/
  krLogarithmic   = 1
}
OdGiMrExposureType; // Represents the available exposure types.

typedef enum OdGiMrFinalGatheringMode_
{
  krFinalGatherOff   = 0,
  krFinalGatherOn    = 1,
  krFinalGatherAuto  = 2
}
OdGiMrFinalGatheringMode;

typedef enum OdGiMrExportMIMode_
{
  /** Mi file is not exported.*/
  krExportMIOff        = 0,
  /** Mi file is exported and rendered.*/
  krExportMIWithRender = 1,
  /** Mi file is exported but not rendered.*/
  krExportMIOnly       = 2
}
OdGiMrExportMIMode; // Represents values for MI mode exporting.

/** \details
    Represents settings that are specific to the mental ray render.
    
    Corresponding C++ library: TD_Gi
    <group OdGi_Classes> 
*/
class ODRX_ABSTRACT FIRSTDLL_EXPORT OdGiMentalRayRenderSettingsTraits : public OdGiRenderSettingsTraits
{
public:
  ODRX_DECLARE_MEMBERS(OdGiMentalRayRenderSettingsTraits);

  /** \details
    Specifies the minimum and maximum numbers of samples that are used for pixel shading process during rendering.
      
    \param min [in]  Minimum sampling rate [-3..5]. Values less than zero enable subsampling. -3 corresponds to one sample for every 64 pixel (1/64) and 5 corresponds to 1024 samples per pixel. This parameter must be less or equal to the max parameter.
    \param max [in]  Maximal sampling rate [-3..5].  This parameter must be equal or greater than to the mix parameter. 
  */
  virtual void setSampling(int min, int max) = 0;
  
  /** \details
    Retrieves the minimum and maximum numbers of samples that are used for pixel shading process during rendering.
      
    \param min [out]  Minimum sampling rate [-3..5]. Values less than zero enable subsampling. -3 corresponds to one sample for every 64 pixel (1/64) and 5 corresponds to 1024 samples per pixel. This parameter cannot be greater than the max parameter.
    \param max [out]  Maximal sampling rate [-3..5]. This parameter cannot be less than the min parameter.
  */
  virtual void sampling(int& min, int& max) const = 0;

  
  
  /** \details
    Specifies the filtering parameters for merging multiple samples into a pixel color.
    
    \param filter [in]  Filtering parameters for merging multiple samples into a pixel color.
    \param width [in]  Width of a filter area for neighboring pixels in the range of [0..8]. 
    \param height [in]  Width of a filter area for neighboring pixels in the range of [0..8]. 
  */
  virtual void setSamplingFilter(OdGiMrFilter filter, double width, double height) = 0;
  
  /** \details
    Retrieves the filtering parameters for merging multiple samples into a pixel color.
      
    \param filter [out]  Filtering parameters for merging multiple samples into a pixel color.
    \param width [out]  Width of a filter area for neighboring pixels in the range of [0..8]. 
    \param height [out]  Width of a filter area for neighboring pixels in the range of [0..8]. 
  */
  virtual void SamplingFilter(OdGiMrFilter& filter, double& width, double& height) const = 0;

  
  
  /** \details
    Specifies the threshold values for color channels below which further samples are not accepted.
    
    \param r [in]  Red channel.
    \param g [in]  Green channel.
    \param b [in]  Blue channel.
    \param a [in]  Alpha (transparency) channel.
    
    \remarks
    Each color channel value can be in the range [0.0..1.0].
  */
  virtual void setSamplingContrastColor(float r, float g, float b, float a) = 0;
  
  /** \details
    Retrieves the threshold values for color channels below which further samples are not accepted.
    
    \param r [out]  Red channel.
    \param g [out]  Green channel.
    \param b [out]  Blue channel.
    \param a [out]  Alpha (transparency) channel.
    
    \remarks
    Each color channel value can be in the range [0.0..1.0].
  */
  virtual void samplingContrastColor(float& r, float& g, float& b, float& a) const = 0;

  
  
  /** \details
    Specifies the method used to compute ray-traced shadows. Affects only lights using ray-traced shadows.
    
    \param mode [in]  Shadow mode.
  */
  virtual void setShadowMode(OdGiMrShadowMode mode) = 0;
  
  /** \details
    Retrieves the method used to compute ray-traced shadows.
    
    \returns
    a value from the OdGiMrShadowMode which represents a shadow mode used for processing ray-traced shadows.
  */
  virtual OdGiMrShadowMode shadowMode() const = 0;

  
  
  /** \details
    Specifies whether shadow maps should be enabled. Affects only lights using mapped shadows.
    
    \param enabled [in]  Flag that specifies whether shadow maps should be enabled.
  */
  virtual void setShadowMapEnabled(bool enabled) = 0;
  
  /** \details
    Checks whether shadow maps is enabled.
    
    \returns
    true if shadow mapping is enabled, false otherwise.
  */
  virtual bool shadowMapEnabled() const = 0;

  
  
  /** \details
    Specifies whether ray tracing should be enabled.
    
    \param enabled [in]  Flag that specifies whether ray tracing should be enabled.
  */
  virtual void setRayTraceEnabled(bool enabled) = 0;
  
  /** \details
    Checks whether ray tracing is enabled.
    
    \returns
    true if ray tracing is enabled, false otherwise.
  */
  virtual bool rayTraceEnabled() const = 0;

  
  
  /** \details
    Specifies the maximum depth (recursion depth) for traced rays.
    
    \param reflection [in]  Maximum recursion level of reflection rays, greater than or equal to 0.
    \param refraction [in]  Maximum recursion level of refraction rays, greater than or equal to 0.
    \param sum [in]  Maximum recursion level of reflection and refraction rays, combined.
  */
  virtual void setRayTraceDepth(int reflection, int refraction, int sum) = 0;
  
  /** \details
    Retrieves the maximum depth (recursion depth) for traced rays.
    
    \param reflection [out]  Maximum recursion level of reflection rays, greater than or equal to 0.
    \param refraction [out]  Maximum recursion level of refraction rays, greater than or equal to 0.
    \param sum [out]  Maximum recursion level of reflection and refraction rays, combined.
  */
  virtual void rayTraceDepth(int& reflection, int& refraction, int& sum) const = 0;

  
  
  /** \details
    Specifies whether global illumination effects (for example, color bleeding) should be enabled.
    
    \param enabled [in]  Flag that specifies whether global illumination should be enabled.
  */
  virtual void setGlobalIlluminationEnabled(bool enabled) = 0;
  
  /** \details
    Checks whether global illumination effects are enabled.
    
    \returns
    true if ray tracing is enabled, false otherwise.
  */
  virtual bool globalIlluminationEnabled() const = 0;

  
  
  /** \details
    Specifies the maximum number of photons surrounding a render sample point to use for calculation of global illumination. 
    
    \param sum [in]  Number of photons around each render sample. Must be greater than zero.
  */
  virtual void setGISampleCount(int num) = 0;
  
  /** \details
    Retrieves the maximum number of photons surrounding a render sample point to use for calculation of global illumination.
    
    \returns
    integer value that represents a number of photons around each render sample.
  */
  virtual int giSampleCount() const = 0;

  
  
  /** \details
    Specifies whether user-defined photon radius should be used. 
    
    \param enabled [in]  Flag that specifies whether to enable user-defined photon sampling radius.
  */
  virtual void setGISampleRadiusEnabled(bool enabled) = 0;
  
  /** \details
    Checks whether user-defined photon radius is enabled.
    
    \returns
    true if user-defined photon radius is enabled, false otherwise.
  */
  virtual bool giSampleRadiusEnabled() const = 0;

  
  
  /** \details
    Specifies the radius of the sample area of photons used for global illumination. 
    
    \param radius [in]  Radius in model units. Must be greater than or equal to zero.
    
    \remarks
    Photons are searched for the area indicated around each render sample, up to the number of photons defined by setGIPhotonCount() method.
  */
  virtual void setGISampleRadius(double radius) = 0;
  
  /** \details
    Retrieves the radius of the sample area of photons used for global illumination. 
    
    \returns
    radius of sample photons as a value of the double type.
  */
  virtual double giSampleRadius() const = 0;

  
  
  /** \details
    Specifies the average number of photons to emit for each light source. 
    
    \param num [in]  Average number of photons to emit for each light. Must be greater than 0.
  */
  virtual void setGIPhotonsPerLight(int num) = 0;
  
  /** \details
    Retrieves the average number of photons that are emitted for each light source. 
    
    \returns
    average number of photons that are emitted for each light source.
  */
  virtual int giPhotonsPerLight() const = 0;

  
  
  /** \details
    Specifies the maximum depth (recursion level) for traced photons. 
    
    \param reflection [in]  Maximum recursion level of reflection photons, greater than or equal to 0.
    \param refraction [in]  Maximum recursion level of refraction photons, greater than or equal to 0.
    \param sum [in]  Maximum recursion level of reflection and refraction photons, combined.
  */
  virtual void setPhotonTraceDepth(int reflection, int refraction, int sum) = 0;
  
  /** \details
  Retrieves the maximum depth (recursion depth) for traced photons.
    
    \param reflection [out]  Maximum recursion level of reflection photons, greater than or equal to 0.
    \param refraction [out]  Maximum recursion level of refraction photons, greater than or equal to 0.
    \param sum [out]  Maximum recursion level of reflection and refraction photons, combined.
  */
  virtual void photonTraceDepth(int& reflection, int& refraction, int& sum) const = 0;

  
  
  /** \details
    Specifies whether final gathering should be enabled.
    
    \param enabled [in]  Flag that specifies whether global illumination should be enabled.
    
    \remarks
    Final gathering enables indirect illumination effects such as color bleeding and can be combined with global illumination.
  */
  virtual void setFinalGatheringEnabled(bool enabled) = 0;
  
  /** \details
    Checks whether final gathering is enabled.
    
    \returns
    true if final gathering is enabled, false otherwise.
    
    \remarks
    Final gathering enables indirect illumination effects such as color bleeding and can be combined with global illumination.
  */
  virtual bool finalGatheringEnabled() const = 0;

  
  
  /** \details
    Specifies the amount of final gather rays to use for each final gather point.
    
    \param enabled [in]  Amount of final gather rays to use for each final gather point. Must be greater than 0.
    
    \remarks
    Each final gather point receives the specified amount of rays to calculate indirect illumination.
  */
  virtual void setFGRayCount(int num) = 0;
  
  /** \details
    Retrieves the amount of final gather rays used for each final gather point.
    
    \returns
    a number of final gather rays.
    
    \remarks
    Each final gather point receives the specified amount of rays to calculate indirect illumination.
  */
  virtual int fgRayCount() const = 0;

  
  
  /** \details
    Specifies whether user-defined radii are used for final gathering and the measurement units for the radii.
    
    \param bMin [in]  Flag that specifies whether the user-defined minimum radius is used. If set to false, a default radius is used.
    \param bMax [in]  Flag that specifies whether the user-defined maximum radius is used. If set to false, a default radius is used.
    \param bPixels [in]  Flag that specifies whether the user-defined radii are measured in pixel units.
  */
  virtual void setFGRadiusState(bool bMin, bool bMax, bool bPixels) = 0;
  
  /** \details
    Checks whether user-defined radii are used for final gathering and their measurement units.
    
    \param bMin [out]  Flag that shows whether the user-defined minimum radius is used. If set to false, a default radius is used.
    \param bMax [out]  Flag that shows whether the user-defined maximum radius is used. If set to false, a default radius is used.
    \param bPixels [out]  Flag that shows whether the user-defined radii are measured in pixel units.
  */
  virtual void fgSampleRadiusState(bool& bMin, bool& bMax, bool& bPixels) = 0;

  
  
  /** \details
    Specifies the minimum and maximum radii of the area used to sample final gather points to process indirect illumination.
    
    \param min [in]  Minimum final gather sample radius. To define measurement units for this parameter, use the setFGRadiusState() method. Must be greater than 0 but less than or equal to the max parameter.
    \param max [in]  Maximum final gather sample radius. To define measurement units for this parameter, use the setFGRadiusState() method. Must be greater than 0 and greater than or equal to the min parameter.
  */
  virtual void setFGSampleRadius(double min, double max) = 0;
  
  /** \details
    Retrieves the minimum and maximum radii of the area used to sample final gather points to process indirect illumination.
    
    \param min [out]  Receives minimum final gather sample radius. Measurement units for this parameter can be retrieved using the setFGRadiusState() method.
    \param max [out]  Receives maximum final gather sample radius. Measurement units for this parameter can be retrieved using the setFGRadiusState() method.
  */
  virtual void fgSampleRadius(double& min, double& max) const = 0;

  
  
  /** \details
    Specifies the physical luminance of a light with intensity 1.0 (in candelas).
    
    \param luminance [in]  Physical luminance of a light with intensity 1.0 (in candelas), used as a physical scaling factor.
  */
  virtual void setLightLuminanceScale(double luminance) = 0;
  
  /** \details
    Retrieves the physical luminance of a light with intensity 1.0 (in candelas).
    
    \returns
    physical luminance of a light.
  */
  virtual double lightLuminanceScale() const = 0;

  
  
  /** \details
    Specifies a mode used to process diagnostic images.
    
    \param mode [in]  Mode used to process diagnostic images.
  */
  virtual void setDiagnosticMode(OdGiMrDiagnosticMode mode) = 0;
  
  /** \details
    Retrieves a mode used to process diagnostic images.
    
    \returns
    physical luminance of a light.
  */
  virtual OdGiMrDiagnosticMode diagnosticMode() const = 0;

  
  
  /** \details
    Specifies the coordinate system for the diagnostic grid and the distance between grid lines (size). 
    
    \param mode [in]  Type of a diagnostic grid.
    \param fSize [in]  Distance between grid lines. Must be greater than 0.
  */
  virtual void setDiagnosticGridMode(OdGiMrDiagnosticGridMode mode, float fSize) = 0;
  
  /** \details
    Retrieves the coordinate system for the diagnostic grid and the distance between grid lines (size). 
    
    \param mode [out]  Receives the type of a diagnostic grid.
    \param fSize [out]  Receives the distance between grid lines. Must be greater than 0.
  */
  virtual void diagnosticGridMode(OdGiMrDiagnosticGridMode& mode, float& fSize) const = 0;

  
  
  /** \details
    Specifies the type of photon information to render images within the photon diagnostic mode.
    
    \param mode [in]  Photon diagnostic mode.
  */
  virtual void setDiagnosticPhotonMode(OdGiMrDiagnosticPhotonMode mode) = 0;
  
  /** \details
    Retrieves the type of photon information to render images within the photon diagnostic mode.
    
    \returns
    the type of photon information to to process images within the photon diagnostic mode as OdGiMrDiagnosticPhotonMode value.
  */
  virtual OdGiMrDiagnosticPhotonMode diagnosticPhotonMode() const = 0;

  
  
  /** \details
    Specifies the type of BSP information to render images within the BSP diagnostic mode.
    
    \param mode [in]  BSP diagnostic mode.
  */
  virtual void setDiagnosticBSPMode(OdGiMrDiagnosticBSPMode mode) = 0;
  
  /** \details
    Retrieves the type of BSP information to render images within the BSP diagnostic mode.
    
    \returns
    the type of BSP information to to process images within the BSP diagnostic mode as OdGiMrDiagnosticBSPMode value.
  */
  virtual OdGiMrDiagnosticBSPMode diagnosticBSPMode() const = 0;

  
  
  /** \details
    Specifies whether to export MI file after image is rendered.
    
    \param mode [in]  Flag that specifies whether to export MI file after image is rendered.
  */
  virtual void setExportMIEnabled(bool enabled) = 0;
  
  /** \details
    Checks whether MI file is enabled after image rendering.
    
    \returns
    flag that specifies whether MI file is enabled after image rendering.
  */
  virtual bool exportMIEnabled() const = 0;

  
  
  /** \details
    Specifies a file name for an exported MI file.
    
    \param miName [in]  Name for an exported MI file.
  */
  virtual void setExportMIFileName(const OdString& miName) = 0;
  
  /** \details
    Retrieves a file name for an exported MI file.
    
    \returns
    file name for an exported MI file as OdString value.
  */
  virtual OdString exportMIFileName() const = 0;

  
  
  /** \details
    Specifies the size of the image tiles used during rendering. 
    
    \param size [in]  Size of image tiles used during rendering.
    
    \remarks
    Small values increase rendering time, however provide more frequent updates during rendering. 
    The values must be in range [4..512]. 
  */
  virtual void setTileSize(int size) = 0;
  
  /** \details
    Retrieves the size of the image tiles used during rendering. 
    
    \returns
    size of the image tiles used during rendering in range of [4..512].
  */
  virtual int tileSize() const = 0;

  
  
  /** \details
    Specifies the order used to render image tiles. 
    
    \param order [in]  Order used to render image tiles.
  */
  virtual void setTileOrder(OdGiMrTileOrder order) = 0;
  
  /** \details
    Retrieves the order used to render image tiles. 
    
    \returns
    order used to render image tiles as OdGiMrTileOrder value.
  */
  virtual OdGiMrTileOrder tileOrder() const = 0;

  
  
  /** \details
    Specifies the maximum amount of memory that can be allocated for rendering. 
    
    \param limit [in]  maximum amount of memory for rendering in MB.
  */
  virtual void setMemoryLimit(int limit) = 0;
  
  /** \details
    Retrieves the maximum amount of memory that can be allocated for rendering. 
    
    \returns
    maximum amount of memory that can be allocated for rendering as integer value.
  */
  virtual int memoryLimit() const = 0;

  
  
  /** \details
    Specifies the energy multiplier to control the magnitude of indirect illumination. 
    
    \param fScale [in]  Scale factor.
  */
  virtual void setEnergyMultiplier(float fScale) = 0;
  
  /** \details
    Receives the energy multiplier to control the magnitude of indirect illumination. 
    
    \returns
    scale factor to control magnitude of indirect illumination.
  */
  virtual float energyMultiplier() const = 0;

  
  
  /** \details
    Specifies the interface pointer for the render driver to callback. 
    
    \param pMonitor [in]  Void pointer that caller provides for the render driver to callback.
  */
  virtual void setProgressMonitor(void* pMonitor) = 0;
  
  /** \details
    Retrieves a pointer to the progress monitor. 
    
    \returns
    a pointer to a void type that can be casted later to a type of a corresponding process monitor.
  */
  virtual const void* progressMonitor() const = 0;

  
  /** \details
    Specifies the exposure type. 
    
    \param type [in]  Exposure type.
  */
  virtual void setExposureType(OdGiMrExposureType type) = 0;
  
  /** \details
    Retrieves the exposure type. 
    
    \returns
    exposure type as a value from the OdGiMrExposureType enumeration.
  */
  virtual OdGiMrExposureType exposureType() const = 0;

  
  
  /** \details
    Specifies the final gathering mode. 
    
    \param mode [in]  Final gathering mode. 
    
    \remarks
    Final gathering enables indirect illumination effects such as color bleeding and can be combined with global illumination.
  */
  virtual void setFinalGatheringMode(OdGiMrFinalGatheringMode mode) = 0;
  
  /** \details
    Retrieves the final gathering mode. 
    
    \returns
    final gathering mode as a value from OdGiMrFinalGatheringMode enumeration.
    
    \remarks
    Final gathering enables indirect illumination effects such as color bleeding and can be combined with global illumination.
  */
  virtual OdGiMrFinalGatheringMode finalGatheringMode() const = 0;

  
  
  /** \details
    Specifies shadow sampling multiplier value.
    
    \param multiplier [in]  Multiplier value for shadows. 
  */
  virtual void setShadowSamplingMultiplier(double multiplier) = 0;
  
  /** \details
    Retrieves shadow sampling multiplier value.
    
    \returns
    multiplier value for shadows. 
  */
  virtual double shadowSamplingMultiplier() const = 0;

  
  
  /** \details
    Specifies an export type for MI file.
    
    \param mode [in]  Defines export mode for MI files. 
  */
  virtual void setExportMIMode(OdGiMrExportMIMode mode) = 0;
  
  /** \details
    Retrieves an export type for MI file.
    
    \returns
    an export mode for MI file as a value from the OdGiMrExportMIMode enumeration.
  */
  virtual OdGiMrExportMIMode exportMIMode() const = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGiMentalRayRenderSettingsTraits object pointers.
*/
typedef OdSmartPtr<OdGiMentalRayRenderSettingsTraits> OdGiMentalRayRenderSettingsTraitsPtr;


typedef enum OdGiQuitCondition_
{
  /** Quit by render level.*/
  krEQuitByRenderLevel = 0,
  /** Quit by exceeding render time limit.*/
  krEQuitByRenderTime  = 1
}
OdGiQuitCondition; // Represents the quit condition.

typedef enum OdGiLightingMode_
{
  /** Global illumination and Glossy reflection/refraction is turned off. The most simple way to reduce noise.*/
  krESimplistic = 0,
  /** Ambient Occlusion and a simpler model for ambient light and image-based lighting (IBL) are introduced. Provides faster results.*/
  krEBasic      = 1,
  /** Full Global Illumination and a more advanced model for ambient light and IBL are introduced. Provides more realistic results.*/
  krEAdvanced   = 2
}
OdGiLightingMode; // Represents light modes.

typedef enum OdGiFilterType_
{
  /** Box filter type.*/
  krEBox      = 0,
  /** Triangle filter type.*/
  krETriangle = 1,
  /** Gaussian filter type.*/
  krEGaussian = 2,
  /** Lanczos filter type.*/
  krELanczos  = 3,
  /** Mitchell filter type.*/
  krEMitchell = 4
}
OdGiFilterType; // Represents filter types.

/** \details

    Corresponding C++ library: TD_Gi
    <group OdGi_Classes>
*/
class ODRX_ABSTRACT FIRSTDLL_EXPORT OdGiRapidRTRenderSettingsTraits : public OdGiRenderSettingsTraits
{
public:
  ODRX_DECLARE_MEMBERS(OdGiRapidRTRenderSettingsTraits);

  /** \details
    Specifies quit condition for rendering process. 
    
    \param condition [in]  Quit condition.
  */
  virtual void setQuitCondition(OdGiQuitCondition condition) = 0;
  
  /** \details
    Retrieves a condition that is used to quit the rendering process. 
    
    \returns
    quit condition as a value from the OdGiQuitCondition enumeration.
  */
  virtual OdGiQuitCondition quitCondition() const = 0;

  
  
  /** \details
    Specifies the desired render level. Rendering quits once desired render quality is reached. This works only when the OdGiQuitCondition::krEQuitByRenderLevel value was passed to the setQuitCondition() method.
    
    \param level [in]  Desired render level in range [1..50].
  */
  virtual void setDesiredRenderLevel(int level) = 0;
  
  /** \details
    Retrieves the desired render level. Rendering quits once desired render quality is reached. This works only when the OdGiQuitCondition::krEQuitByRenderLevel value was passed to the setQuitCondition() method.
    
    \returns
    desired render level in range [1..50].
  */
  virtual int desiredRenderLevel() const = 0;

  
  
  /** \details
    Specifies the desired render time. Rendering quits once rendering time limit is exceeded. This works only when the OdGiQuitCondition::krEQuitByRenderTime value was passed to the setQuitCondition() method.
    
    \param time [in]  Desired render time in range [1..1440]. Render time is set in minutes.
  */
  virtual void setDesiredRenderTime(int time) = 0;
  
  /** \details
    Retrieves the desired render time. Rendering quits once rendering time limit is exceeded. This works only when the OdGiQuitCondition::krEQuitByRenderTime value was passed to the setQuitCondition() method.
    
    \returns
    desired render time in minutes.
  */
  virtual int desiredRenderTime() const = 0;

  
  /** \details
    Specifies a lighting mode.
    
    \param mode [in]  Lighting mode.
    
    \remarks
    You can set whether a simplistic, basic or advanced Lighting mode. When simplistic mode is used, the Global illumination and Glossy reflection/refraction is turned off. 
    Basic lighting uses ambient occlusion and a simpler model for ambient light and image-based lighting (IBL). 
    Advanced lighting uses full global illumination and a more advanced model for ambient light and IBL.
  */
  virtual void setLightingMode(OdGiLightingMode mode) = 0;
  
  /** \details
    Retrieves a lighting mode.
    
    \returns
    a lighting mode as a value from the OdGiLightingMode enumeration.
    
    \remarks
    There are a simplistic, basic or advanced Lighting mode. When simplistic mode is used, the Global illumination and Glossy reflection/refraction is turned off.
    Basic lighting uses ambient occlusion and a simpler model for ambient light and image-based lighting (IBL). 
    Advanced lighting uses full global illumination and a more advanced model for ambient light and IBL.
  */
  virtual OdGiLightingMode lightingMode() const = 0;

  
  
  /** \details
    Specifies the filter type applied to the image samples during sampling.
    
    \param type [in]  Filter type.
  */
  virtual void setFilterType(OdGiFilterType type) = 0;
  
  /** \details
    Retrieves the filter type applied to the image samples during sampling.
    
    \returns
    a filter type.
  */
  virtual OdGiFilterType filterType() const = 0;

  
  
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
  virtual void setFilterWidth(float width) = 0;
  
  /** \details
    Retrieves the filter width in pixels.
    
    \returns
    filter width.
  */
  virtual float filterWidth() const = 0;

  
  
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
  virtual void setFilterHeight(float height) = 0;
  
  /** \details
    Retrieves the filter height in pixels.
    
    \returns
    filter height.
  */
  virtual float filterHeight() const = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGiRapidRTRenderSettingsTraits object pointers.
*/
typedef OdSmartPtr<OdGiRapidRTRenderSettingsTraits> OdGiRapidRTRenderSettingsTraitsPtr;


/** \details
    This class represents traits that are related to the image-based lighting (IBL) backgrounds.
    
    Corresponding C++ library: TD_Gi
    <group OdGi_Classes> 
*/
class ODRX_ABSTRACT FIRSTDLL_EXPORT OdGiSolidBackgroundTraits : public OdGiDrawableTraits
{
public:
  ODRX_DECLARE_MEMBERS(OdGiSolidBackgroundTraits);

  /** \details
    Specifies a solid color for this background.
    
    \param color [in]  Solid color.
  */
  virtual void setColorSolid(const OdCmEntityColor& color) = 0;
  
  /** \details
    Retrieves a solid color of this background.
    
    \returns
    solid color as instance of the OdCmEntityColor class.
  */
  virtual OdCmEntityColor colorSolid() const = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGiSolidBackgroundTraits object pointers.
*/
typedef OdSmartPtr<OdGiSolidBackgroundTraits> OdGiSolidBackgroundTraitsPtr;


/** \details
    This class represents traits that are related to the gradient backgrounds.

    Corresponding C++ library: TD_Gi
    <group OdGi_Classes> 
*/
class ODRX_ABSTRACT FIRSTDLL_EXPORT OdGiGradientBackgroundTraits : public OdGiDrawableTraits
{
public:
  ODRX_DECLARE_MEMBERS(OdGiGradientBackgroundTraits);

  /** \details
    Specifies a top color for this background.
    
    \param color [in]  Top color.
  */
  virtual void setColorTop(const OdCmEntityColor& color) = 0;
  
  /** \details
    Retrieves a top color of this background.
    
    \returns
    top color as instance of the OdCmEntityColor class.
  */
  virtual OdCmEntityColor colorTop() const = 0;

  
  
  /** \details
    Specifies a middle color for this background.
    
    \param color [in]  Middle color.
  */
  virtual void setColorMiddle(const OdCmEntityColor& color) = 0;
  
  /** \details
    Retrieves a middle color of this background.
    
    \returns
    middle color as instance of the OdCmEntityColor class.
  */
  virtual OdCmEntityColor colorMiddle() const  = 0;

  
  
  /** \details
    Specifies a bottom color for this background.
    
    \param color [in]  Bottom color.
  */
  virtual void setColorBottom(const OdCmEntityColor& color) = 0;
  
  /** \details
    Retrieves a bottom color of this background.
    
    \returns
    bottom color as instance of the OdCmEntityColor class.
  */
  virtual OdCmEntityColor colorBottom() const  = 0;

  
  
  /** \details
    Specifies a position of a middle color relative to top and bottom colors.
    
    \param horizon [in]  Position of a middle color in range where 0 is the beginning of device coordinate system and 1 is end of of device coordinate system.
    
    \remarks
    For example, if you call this method and pass 0.5, a middle color will reside in the middle of a window (if no rotation applied). The passed value may lie out of [0..1] range,
    which means that position for middle color is set off-screen.
  */
  virtual void setHorizon(double horizon) = 0;
  
  /** \details
    Retrieves a position of a middle color relative to top and bottom colors.
    
    \returns
    horizon that represents position of a middle color relative to top and bottom colors.
  */
  virtual double horizon() const = 0;

  
  
  /** \details
    Specifies height of the middle color.
    
    \param height [in]  Height of the middle color.
    
    \remarks
    For example, if you call the setHorizon() method and pass 0.5 and then call this method with 0.33 value then all 3 gradient
    colors are equally distributed inside scene window (if no rotation applied).
  */
  virtual void setHeight(double height) = 0;
  
  /** \details
    Receives height of the middle color.
    
    \returns
    height of a middle color.
  */
  virtual double height() const = 0;

  
  
  /** \details
    Specifies rotation of the gradient.
    
    \param rotation [in]  Rotation in degrees.
    
    \remarks
    Rotation is performed the counter clock-wise direction, relative to window coordinates. 
    For example, if you call this method with the 45.0 value the gradient background is so 
    the beginning of gradient is in lower-left corner of a window and the end is in the upper-right corner.
  */
  virtual void setRotation(double rotation) = 0;
  
  /** \details
    Receives rotation of the gradient.
    
    \returns
    degrees of counter clock-wise gradient rotation.
  */
  virtual double rotation() const = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGiGradientBackgroundTraits object pointers.
*/
typedef OdSmartPtr<OdGiGradientBackgroundTraits> OdGiGradientBackgroundTraitsPtr;


/** \details
    This class represents traits that are related to the image-based backgrounds.

    Corresponding C++ library: TD_Gi
    <group OdGi_Classes> 
*/
class ODRX_ABSTRACT FIRSTDLL_EXPORT OdGiImageBackgroundTraits : public OdGiDrawableTraits
{
public:
  ODRX_DECLARE_MEMBERS(OdGiImageBackgroundTraits);

  /** \details
    Specifies image file name.
    
    \param filename [in]  Full path to an image file.
  */
  virtual void setImageFilename(const OdString& filename) = 0;
  
  /** \details
    Retrieves image file name.
    
    \returns
    full path to an image file.
  */
  virtual const OdString& imageFilename() const = 0;

  
  
  /** \details
    Specifies whether image-based background should be fit to screen.
    
    \param bFitToScreen [in]  Flag that specifies whether to fit this background to screen.
  */
  virtual void setFitToScreen(bool bFitToScreen) = 0;
  
  /** \details
    Checks whether image-based background should be fit to screen.
    
    \returns
    true if background is fit to screen, false otherwise.
  */
  virtual bool fitToScreen() const = 0;

  
  
  /** \details
    Specifies whether image-based background should maintain aspect ratio. Usually called together with setFitToScreen() methods
    or other methods which can modify background dimensions.
    
    \param bMaintainAspectRatio [in]  Flag that specifies whether to maintain aspect ratio of a background image.
  */
  virtual void setMaintainAspectRatio(bool bMaintainAspectRatio) = 0;
  
  /** \details
    Checks whether image-based background maintains aspect ratio.
    
    \returns
    true if aspect ratio is maintained, false otherwise.
  */
  virtual bool maintainAspectRatio() const = 0;

  
  
  /** \details
    Specifies whether to tile image-based background.
    
    \param bUseTiling [in]  Flag that specifies whether to tile image-based background.
    
    \remarks
    Note that background cannot be tiled if it is set to fit to screen.
  */
  virtual void setUseTiling(bool bUseTiling) = 0;
  
  /** \details
    Checks whether image-based background is tiled.
    
    \returns
    true if background is tiled, false otherwise.
  */
  virtual bool useTiling() const = 0;

  
  
  /** \details
    Specifies offset of a background on the x-axis.
    
    \param xOffset [in]  Offset distance. Negative values mean that background is offset in the opposite direction of x-axis. 
  */
  virtual void setXOffset(double xOffset) = 0;
  
  /** \details
    Retrieves offset of a background on the x-axis.
    
    \returns
    offset value on the x-axis.
  */
  virtual double xOffset() const = 0;

  
  
  /** \details
    Specifies offset of a background on the y-axis.
    
    \param yOffset [in]  Offset distance. Negative values mean that background is offset in the opposite direction of y-axis. 
  */
  virtual void setYOffset(double yOffset) = 0;
  
  /** \details
    Retrieves offset of a background on the y-axis.
    
    \returns
    offset value on the y-axis.
  */
  virtual double yOffset() const = 0;

  
  
  /** \details
    Specifies scale of a background on the x-axis.
    
    \param xScale [in]  Scale factor for the x-axis.
  */
  virtual void setXScale(double xScale) = 0;
  
  /** \details
    Retrieves scale of a background on the x-axis.
    
    \returns
    Scale of the x-axis for this background. Values between 0 and 1 mean that background is shrunk on x-axis, values greater than 1 mean than background is stretched
    and negative values mean that background is flipped on x-axis.
  */
  virtual double xScale() const = 0;

  
  
  /** \details
    Specifies scale of a background on the y-axis.
    
    \param yScale [in]  Scale factor for the y-axis.
  */
  virtual void setYScale(double yScale) = 0;
  
  /** \details
    Retrieves scale of a background on the y-axis.
    
    \returns
    Scale of the y-axis for this background. Values between 0 and 1 mean that background is shrunk on y-axis, values greater than 1 mean than background is stretched
    and negative values mean that background is flipped on y-axis.
  */
  virtual double yScale() const = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGiImageBackgroundTraits object pointers.
*/
typedef OdSmartPtr<OdGiImageBackgroundTraits> OdGiImageBackgroundTraitsPtr;


/** \details
    This class represents traits that are related to the ground plane backgrounds.

    Corresponding C++ library: TD_Gi
    <group OdGi_Classes> 
*/
class ODRX_ABSTRACT FIRSTDLL_EXPORT OdGiGroundPlaneBackgroundTraits : public OdGiDrawableTraits
{
public:
  ODRX_DECLARE_MEMBERS(OdGiGroundPlaneBackgroundTraits);

  /** \details
    Specifies a color of the sky zenith.
    
    \param color [in]  Color of the sky zenith.
  */
  virtual void setColorSkyZenith(const OdCmEntityColor& color) = 0;
  
  /** \details
    Retrieves a color of the sky zenith.
    
    \returns
    color of sky zenith as OdCmEntityColor instance.
  */
  virtual OdCmEntityColor colorSkyZenith() const = 0;

  
  
  /** \details
    Specifies a color of the sky horizon.
    
    \param color [in]  Color of the sky horizon.
  */
  virtual void setColorSkyHorizon(const OdCmEntityColor& color) = 0;
  
  /** \details
    Retrieves a color of the sky horizon.
    
    \returns
    color of sky horizon as OdCmEntityColor instance.
  */
  virtual OdCmEntityColor colorSkyHorizon() const = 0;

  
  
  /** \details
    Specifies a color of the underground horizon.
    
    \param color [in]  Color of the underground horizon.
  */
  virtual void setColorUndergroundHorizon(const OdCmEntityColor& color) = 0;
  
  /** \details
    Retrieves a color of the underground horizon.
    
    \returns
    color of underground horizon as OdCmEntityColor instance.
  */
  virtual OdCmEntityColor colorUndergroundHorizon() const = 0;

  
  
  /** \details
    Specifies a color of the underground azimuth.
    
    \param color [in]  Color of the underground azimuth.
  */
  virtual void setColorUndergroundAzimuth(const OdCmEntityColor& color) = 0;
  
  /** \details
    Retrieves a color of the underground azimuth.
    
    \returns
    color of underground azimuth as OdCmEntityColor instance.
  */
  virtual OdCmEntityColor colorUndergroundAzimuth() const = 0;

  
  
  /** \details
    Specifies a color of the near ground plane.
    
    \param color [in]  Color of the near ground plane.
  */
  virtual void setColorGroundPlaneNear(const OdCmEntityColor& color) = 0;
  
  /** \details
    Retrieves a color of the near ground plane.
    
    \returns
    color of the near ground plane as OdCmEntityColor instance.
  */
  virtual OdCmEntityColor colorGroundPlaneNear() const = 0;

  
  
  /** \details
    Specifies a color of the far ground plane.
    
    \param color [in]  Color of the far ground plane.
  */
  virtual void setColorGroundPlaneFar(const OdCmEntityColor& color) = 0;
  
  /** \details
    Retrieves a color of the far ground plane.
    
    \returns
    color of the far ground plane as OdCmEntityColor instance.
  */
  virtual OdCmEntityColor colorGroundPlaneFar() const = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGiGroundPlaneBackgroundTraits object pointers.
*/
typedef OdSmartPtr<OdGiGroundPlaneBackgroundTraits> OdGiGroundPlaneBackgroundTraitsPtr;


/** \details
    This class allows Gi classes to implement sky backgrounds.

    Corresponding C++ library: TD_Gi
    <group OdGi_Classes> 
*/
class ODRX_ABSTRACT FIRSTDLL_EXPORT OdGiSkyBackgroundTraits : public OdGiDrawableTraits
{
public:
  ODRX_DECLARE_MEMBERS(OdGiSkyBackgroundTraits);

  /** \details
    Specifies sky parameters of this background.
    
    \param params [in]  Sky parameters.
  */
  virtual void setSkyParameters(const OdGiSkyParameters& params) = 0;
  
  /** \details
    Retrieves sky parameters of this background.
    
    \param params [out]  Receives sky parameters for this parameters.
  */
  virtual void skyParameters(OdGiSkyParameters& params) const = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGiSkyBackgroundTraits object pointers.
*/
typedef OdSmartPtr<OdGiSkyBackgroundTraits> OdGiSkyBackgroundTraitsPtr;


/** \details
    This class allows Gi classes to implement image base lighting backgrounds.

    Corresponding C++ library: TD_Gi
    <group OdGi_Classes>
*/
class ODRX_ABSTRACT FIRSTDLL_EXPORT OdGiIBLBackgroundTraits : public OdGiDrawableTraits
{
public:
  ODRX_DECLARE_MEMBERS(OdGiIBLBackgroundTraits);

  /** \details
    Specifies whether image-based lighting background should be enabled.
    
    \param bEnable [in]  Flag that specifies whether to activate IBL.
  */
  virtual void setEnable(bool bEnable) = 0;
  
  /** \details
    Checks whether image-based lighting is enabled.
    
    \returns
    true if image-based lighting is enabled, false otherwise.
  */
  virtual bool enable() const = 0;

  
  
  /** \details
    Specifies image file name for the image-based lighting background.
    
    \param filename [in]  Full path to an image file.
  */
  virtual void setIBLImageName(const OdString& filename) = 0;
  
  /** \details
    Retrieves image file name for this image-based lighting background.
    
    \returns
    full path to an image file.
  */
  virtual const OdString& IBLImageName() const = 0;

  
  
  /** \details
    Specifies rotation value for the IBL image in range [-180..180].
    
    \param rotation [in]  Rotation value in degrees.
  */
  virtual void setRotation(double rotation) = 0;
  
  /** \details
    Retrieves rotation value for the IBL image.
    
    \returns
    rotation value for the IBL image in degrees.
  */
  virtual double rotation() const = 0;

  
  
  /** \details
    Specifies whether to display the IBL image as the background.
    
    \param bDisplay [in]  Flag that specifies whether to display IBL image as a background.
  */
  virtual void setDisplayImage(bool bDisplay) = 0;
  
  /** \details
    Checks whether the IBL image is displayed as the background.
    
    \returns
    true if the IBL image is displayed as the background, false otherwise.
  */
  virtual bool displayImage() const = 0;

  
  
  /** \details
    Specifies a background to display when false is passed to the DisplayImage() method.
    
    \param bgId [in]  Pointer to a background identifier.
  */
  virtual void setSecondaryBackground(OdDbStub* bgId) = 0;
  
  /** \details
    Retrieves a background identifier of a secondary background.
    
    \returns
    Pointer to an identifier of a secondary background.
  */
  virtual OdDbStub* secondaryBackground() const = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGiIBLBackgroundTraits object pointers.
*/
typedef OdSmartPtr<OdGiIBLBackgroundTraits> OdGiIBLBackgroundTraitsPtr;

#endif // _GIENVIRONMENT_H_INCLUDED_
