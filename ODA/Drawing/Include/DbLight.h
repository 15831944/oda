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


#ifndef __OD_DB_LIGHT__
#define __OD_DB_LIGHT__

#include "TD_PackPush.h"

#include "DbEntity.h"
#include "Gi/GiLightTraits.h"

/** \details
    This class represents a placeholder for all generic light properties.
    Corresponding C++ library: SCENEOE

    <group OdDb_Classes>
*/
class SCENEOE_EXPORT OdDbLight : public OdDbEntity
{
public:
  ODDB_DECLARE_MEMBERS(OdDbLight);

  /** \details
    Default constructor for the OdDbLight class. Creates an object with default properties of the OdDbLightImpl object. 
  */
  OdDbLight();

  enum PhysicalIntensityMethod
  {
    kPeakIntensity = 0,
    kFlux,
    kIlluminance
  };

  /** \details
    The lamp color type. 
  */
  enum LampColorType
  {
    /** Color as Kelvin color temperature. */
    kKelvin = 0,
    /** Color from a lamp color preset. */
    kPreset
  };

  /** \details
    Lamp color presets. 
  */
  enum LampColorPreset
  {
    /** D65White lamp color (255, 255, 255). */
    kD65White = 0,
    /** Fluorescent lamp color (255, 253, 229). */
    kFluorescent,
    /** Cool white lamp color (255, 253, 229). */
    kCoolWhite,
    /** White fluorescent lamp color (255, 249, 221). */
    kWhiteFluorescent,
    /** Daylight fluorescent lamp color (248, 255, 248). */
    kDaylightFluorescent,
    /** Incandescent lamp color (255, 244, 214). */
    kIncandescent,
    /** Xenon lamp color (255, 252, 255). */
    kXenon,
    /** Halogen lamp color (255, 247, 219). */
    kHalogen,
    /** Quartz lamp color (255, 247, 219).*/
    kQuartz,
    /** Metal halide lamp color (255, 250, 225). */
    kMetalHalide,
    /** Mercury lamp color (233, 255, 233). */
    kMercury,
    /** Phosphor mercury lamp color (222, 255, 255). */
    kPhosphorMercury,
    /** High pressure sodium lamp color (255, 236, 204). */
    kHighPressureSodium,
    /** Low pressure sodium lamp color (255, 230, 204). */
    kLowPressureSodium,
    /** Custom lamp color. */
    kCustom
  };

  /** \details
    Display types for glyphs. 
  */
  enum GlyphDisplayType
  {
    /** Automatic glyph display. */
    kGlyphDisplayAuto = 0,
    /** Glyph display enabled. */
    kGlyphDisplayOn,
    /** Glyph display disabled. */
    kGlyphDisplayOff
  };

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
    Applies the specified 3D transformation matrix to this entity. 
    
    \param xfm [in] 3D transformation matrix. 
    \returns
    eOk if successful, or an appropriate error code otherwise.
  */
  virtual OdResult subTransformBy(
    const OdGeMatrix3d& xfm);

  /** \details
    Called as the first operation when this object is being erased or unerased.
    
    \param erasing  [in] A copy of the erasing argument passed to erase(). 
    \returns
    Returns eOk if and only if erase() is to continue. 
    \remarks
    This function is notified just before the current object is to be erased, giving this function the ability to cancel the erase. 
    When overriding this function: 
    <ol>
    1. If the OdDbObject's state is incorrect, return something other than eOk.  
    2. If the parent class's subErase() returns anything other than eOk, immediately return it. 
    3. If other actions are required before erase, do them. 
    4. Return eOk. 
    </ol>
    If you must make changes to this object's state, either make them after step 2, or roll them back if step 2 returns anything other than eOk.
    Also the m_bHighlighted member of this object is set to false if 'erasing' is true.
  */
  virtual OdResult subErase(bool erasing);
  
  /** \details
    Highlights this object.
    
    \param bDoIt  [in] A flag value that determines whether highlighting is enabled (if true) or disabled (if false). This flag can switch the highlighting on or off.
    \param pSubId  [in] A full subentity path that identifies the subentity to be highlighted.    
    \param highlightAll  [in] Indicates whether to highlight in all viewports.
  */
  virtual void subHighlight(bool bDoIt = true, const OdDbFullSubentPath* pSubId = 0, bool highlightAll = false) const;

  /** \details
    Retrieves the type of this drawable.
  */
  virtual DrawableType drawableType() const;
  
  /** \details
    Sets the vectorization attributes of this object, and returns its attribute flags.  

    \param pTraits [in]  Pointer to the OdGiDrawableTraits object from and to which the attributes are to be set.

    \remarks
    This function is called by the vectorization framework, prior to calling subWorldDraw() or
    subViewportDraw(), to set the attributes for an object.

    \sa
    <link OdDbLight::subSetAttributes@OdGiDrawableTraits*@const, subSetAttributesFlags() method> 
  */
  virtual OdUInt32 subSetAttributes(OdGiDrawableTraits* pTraits) const;

  /** \details
    Creates a viewport-independent geometric representation of this object.
    
    \param pWd [in]  Pointer to the OdGiWorldDraw interface.
    \returns
    Returns true if and only if the geometric representation can be generated in a
    viewport-independent manner. 
  */
  virtual bool subWorldDraw(OdGiWorldDraw* pWd) const;
  
  /** \details
    Creates a viewport-dependent geometric representation of this object.
    
    \param pVd [in]  Pointer to the OdGiViewportDraw interface.
    \remarks
    Causes OdGiDrawable to describe its geometry to the specified OdGiViewportDraw object. 
  */
  virtual void subViewportDraw(OdGiViewportDraw* pVd) const;

  /** \details
    Retrieves the light name.
  */
  OdString name() const;
  
  /** \details
    Sets a light name.
    
    \param name [in]  Light name.
  */
  void setName(OdString name);

  /** \details
    Retrieves the type of this light.
  */
  OdGiDrawable::DrawableType lightType() const;
  
  /** \details
    Sets the type of this light.
    
    \param type [in]  Light type.
  */
  void setLightType(OdGiDrawable::DrawableType type);

  /** \details
    Checks whether this light is enabled.
    
    \returns
    true if this light is turned on, false otherwise.
  */
  bool isOn() const;
  
  /** \details
    Switches this light on or off.
    
    \param value [in]  Flag that turns this light on or off.
  */
  void setOn(bool value);

  /** \details
    Checks whether this light is shown during plotting.
    
    \returns
    true if this light is plottable, false otherwise.
  */
  bool isPlottable() const;
  
  /** \details
    Specifies whether this light is shown during plotting.
    
    \param plot [in]  Flag that specifies whether this light is plottable.
  */
  void setIsPlottable(bool plot);

  /** \details
    Retrieves the color of this light.
  */
  OdCmColor lightColor() const;
  
  /** \details
    Specifies the color of this light.
    
    \param color [in]  Light color.
  */
  void setLightColor(const OdCmColor& color);

  /** \details
    Retrieves the intensity of this light.
  */
  double intensity() const;
  
  /** \details
    Specifies the intensity of this light.
    
    \param intensity [in]  Light intensity.
  */
  void setIntensity(double intensity);

  /** \details
    Specifies the shadow parameters for this light.
    
    \param params  [in]  Shadow parameters.
  */
  void setShadowParameters(const OdGiShadowParameters& params);
  
  /** \details
    Retrieves the shadow parameters of this light.
  */
  const OdGiShadowParameters &shadowParameters() const;

  /** \details
    Specifies the attenuation parameters for this light.
    
    \param params  [in]  Attenuation parameters.
  */
  void setLightAttenuation(const OdGiLightAttenuation& params);
  
  /** \details
    Retrieves the attenuation parameters of this light.
  */
  const OdGiLightAttenuation &lightAttenuation() const;

  /** \details
    Specifies the position for point light and spot light sources.
    
    \param pos  [in]  Light source position.
  */
  void setPosition(const OdGePoint3d& pos);
  
  /** \details
    Retrieves the position for point light and spot light sources.
  */
  OdGePoint3d position() const;

  /** \details
    Retrieves the target location of a light.
    
    \returns
    Target location of the light as an OdGePoint3d instance.
  */
  OdGePoint3d targetLocation() const;
  
  /** \details
    Specifies the target location of a light.
    
    \param newVal  [in]  Location of the target. Must differ from the light position.
  */
  void setTargetLocation(const OdGePoint3d& newVal);

  /** \details
    Specifies the hotspot and falloff for the cone of the directional light.
    
    \param hotspot  [in]  Hotspot cone angle, in radians. Defines the brightest part of the spot light beam.
    \param falloff  [in]  Falloff angle, in radians. Defines the full cone of light.
  */
  void setHotspotAndFalloff(double hotspot, double falloff);
  
  /** \details
    Retrieves the cone angle that produces the brightest part of the beam (hotspot) of this light.
  */
  double hotspotAngle() const;
  
  /** \details
    Retrieves the cone angle that produces the light falloff.
  */
  double falloffAngle() const;

  /** \details
    Retrieves the direction of a distant light.
  */
  OdGeVector3d lightDirection() const;
  
  /** \details
    Specifies the direction of a distant light.
    
    \param vec  [in]  Light direction.
  */
  void setLightDirection(const OdGeVector3d& lightvec);

  // Photometric properties

  /** \details
    Retrieves the physical intensity method of this light that indicates how the physical intensity of the light is calculated (by peak intensity, by flux or by illuminance).
  */
  PhysicalIntensityMethod physicalIntensityMethod() const;
  
  /** \details
    Specifies the physical intensity method for this light that indicates how the physical intensity of the light is specified.
    
    \param physIntMethod  [in]  Physical intensity method.
  */
  void setPhysicalIntensityMethod(PhysicalIntensityMethod physIntMethod);

  /** \details
    Retrieves the physical intensity of the light. The units are based on the physical intensity method (candela, lumen or lux).
  */
  double physicalIntensity() const;
  
  /** \details
    Specifies the physical intensity of the light.
    
    \param physIntensity  [in]  Physical intensity of the light. The units are based on the physical intensity method (candela, lumen or lux).
  */
  void setPhysicalIntensity(double physIntensity);

  /** \details
    Retrieves the illuminance distance of the light. 
  */
  double illuminanceDistance() const;
  
  /** \details
    Specifies the illuminance distance of the light.
    
    \param illumDistance  [in]  Illuminance distance of the light.
  */
  void setIlluminanceDistance(double illumDistance);

  /** \details
    Retrieves the lamp color type of the light that indicates how the lamp color of the light is specified 
    (Kelvin color temperature or a preset lamp color).
  */
  LampColorType lampColorType() const;
  
  /** \details
    Specifies the lamp color type of the light that indicates how the lamp color of the light is specified
    (Kelvin color temperature or a preset lamp color).
    
    \param colorType  [in]  Lamp color type.
  */
  void setLampColorType(LampColorType colorType);

  /** \details
    Retrieves the Kelvin color temperature of the light if the lamp color type is kKelvin.
  */
  double lampColorTemp() const;
  
  /** \details
    Sets the Kelvin color temperature of the light.
    
    \param colorTemp  [in]  Lamp color temperature in the range [1000 .. 20000] Kelvin.
  */
  void setLampColorTemp(double colorTemp);

  /** \details
    Retrieves the preset lamp color of this light.
  */
  LampColorPreset lampColorPreset() const;
  
  /** \details
    Sets the lamp color of this light from a preset.
    
    \param colorPreset  [in]  Preset lamp color.
  */
  void setLampColorPreset(LampColorPreset colorPreset);

  /** \details
    Retrieves the RGB value of the light's lamp color.
  */
  OdGiColorRGB lampColorRGB() const;
  
  /** \details
    Sets the lamp color of this light as an RGB color value.
    
    \param color  [in]  RGB lamp color.
  */
  void setLampColorRGB(const OdGiColorRGB &color);
  
  /** \details
    Retrieves the final color of the light as a result of mixing the lamp color and the filter color.
  */
  OdCmColor resultingColor() const;

  /** \details
    Retrieves the full path to the file that stores the light's goniometric data (.ies file).
  */
  OdString webFile() const;
  
  /** \details
    Specifies the full path to the file that stores the light's goniometric data (.ies file).
    
    \param webFile  [in]  Full path to a web file.
  */
  void setWebFile(const OdString &webFile);

  /** \details
    Retrieves the rotation offset used to orient the goniometric data relative to the light orientation.
  */
  OdGeVector3d webRotation() const;
  
  /** \details
    Specifies the rotation offset used to orient the goniometric data relative to the light orientation. The angle for each axis is 0.0. 
    
    \param rotation  [in]  Vector that defines the rotation offset.
  */
  void setWebRotation(const OdGeVector3d &rotation);

  /** \details
    Checks whether this light displays a target grip to orient this light.
  */
  bool hasTarget() const;
  
  /** \details
    Specifies whether this light should display a target grip to orient this light.
    
    \param bTarget  [in]  Flag that specifies whether this light should display a target grip.
  */
  void setHasTarget(bool bTarget);

  /** \details
    Retrieves the glyph display type of this light.
  */
  GlyphDisplayType glyphDisplay() const;
  
  /** \details
    Specifies the display type of the glyph for this light.
    
    \param type  [in]  Glyph display type.
  */
  void setGlyphDisplay(GlyphDisplayType type);

  OdResult subGetClassID(void* pClsid) const;
};

/** \details
  A data type that represents a smart pointer to an <link OdDbLight, OdDbLight> object.
*/
typedef OdSmartPtr<OdDbLight> OdDbLightPtr;

#include "TD_PackPop.h"

#endif // __OD_DB_LIGHT__
