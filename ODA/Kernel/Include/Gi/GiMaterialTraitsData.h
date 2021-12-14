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

#ifndef _ODGIMATERIALTRAITSDATA_INCLUDED_
#define _ODGIMATERIALTRAITSDATA_INCLUDED_

#include "Gi/GiMaterial.h"

class OdDbStub;

#include "TD_PackPush.h"

/** \details

    <group !!RECORDS_TD_APIRef>
    \sa
    <link tv_working_with_attributes_materials, Work with Materials>
*/
struct OdGiMaterialTraitsData
{
protected:
  OdGiMaterialColor                          m_ambientColor;
  OdGiMaterialColor                          m_diffuseColor;
  OdGiMaterialMap                            m_diffuseMap;
  OdGiMaterialColor                          m_specularColor;
  OdGiMaterialMap                            m_specularMap;
  double                                     m_glossFactor;
  OdGiMaterialMap                            m_reflectionMap;
  double                                     m_opacity;
  OdGiMaterialMap                            m_opacityMap;
  OdGiMaterialMap                            m_bumpMap;
  double                                     m_refractionIndex;
  OdGiMaterialMap                            m_refractionMap;
  double                                     m_translucenceVal;
  double                                     m_selfIllumVal;
  double                                     m_reflectivityVal;
  OdGiMaterialTraits::IlluminationModel      m_illumModel;
  OdGiMaterialTraits::ChannelFlags           m_channelFlags;
  OdGiMaterialTraits::Mode                   m_mode;
  // Extended material properties
  double                                     m_bleedScale;
  double                                     m_indirectBumpScale;
  double                                     m_reflectScale;
  double                                     m_transmitScale;
  bool                                       m_twoSided;
  OdGiMaterialTraits::LuminanceMode          m_lumiMode;
  double                                     m_lumiVal;
  OdGiMaterialMap                            m_normalMap;
  OdGiMaterialTraits::NormalMapMethod        m_normalMethod;
  double                                     m_normalStrength;
  OdGiMaterialTraits::GlobalIlluminationMode m_globIllum;
  OdGiMaterialTraits::FinalGatherMode        m_fgMode;
  // BIM extensions
  OdGiMaterialColor                          m_emissionColor;
  OdGiMaterialMap                            m_emissionMap;
  OdGiMaterialColor                          m_tintColor;
  OdGiMaterialColor                          m_shadingAmbientColor;
  OdGiMaterialColor                          m_shadingDiffuseColor;
  OdGiMaterialColor                          m_shadingSpeclrColor;
  double                                     m_shadingOpacity;
  OdGiMaterialTraits::DiffuseColorMode       m_diffuseColorMode;
  // Visualize extensions
  double                                     m_shadowsOpacity;
  double                                     m_specularOverride;
  double                                     m_nslmEffect;

public:
  /** \details
    Default constructor for the OdGiMaterialTraitsData class. Sets object with default properties.
  */
  OdGiMaterialTraitsData()
    : m_ambientColor()
    , m_diffuseColor()
    , m_diffuseMap()
    , m_specularColor()
    , m_specularMap()
    , m_glossFactor(0.0)
    , m_reflectionMap()
    , m_opacity(1.0)
    , m_opacityMap()
    , m_bumpMap()
    , m_refractionIndex(0.0)
    , m_refractionMap()
    , m_translucenceVal(0.0)
    , m_selfIllumVal(0.0)
    , m_reflectivityVal(0.0)
    , m_illumModel(OdGiMaterialTraits::kBlinnShader)
    , m_channelFlags(OdGiMaterialTraits::kUseDiffuse)
    , m_mode(OdGiMaterialTraits::kRealistic)
    // Extended material properties
    , m_bleedScale(100.0)
    , m_indirectBumpScale(100.0)
    , m_reflectScale(100.0)
    , m_transmitScale(100.0)
    , m_twoSided(true)
    , m_lumiMode(OdGiMaterialTraits::kSelfIllumination)
    , m_lumiVal(0.0)
    , m_normalMap()
    , m_normalMethod(OdGiMaterialTraits::kTangentSpace)
    , m_normalStrength(1.0)
    , m_globIllum(OdGiMaterialTraits::kGlobalIlluminationCastAndReceive)
    , m_fgMode(OdGiMaterialTraits::kFinalGatherCastAndReceive)
    // BIM extensions
    , m_emissionColor(OdCmEntityColor(0, 0, 0))
    , m_emissionMap()
    , m_tintColor(OdCmEntityColor(255, 255, 255))
    , m_shadingAmbientColor()
    , m_shadingDiffuseColor()
    , m_shadingSpeclrColor()
    , m_shadingOpacity(1.0)
    , m_diffuseColorMode(OdGiMaterialTraits::kDiffuseRealisticColor)
    // Visualize extensions
    , m_shadowsOpacity(1.0)
    , m_specularOverride(0.0)
    , m_nslmEffect(1.0)
  {
  }
  
  /** \details
    Assignment operator for the OdGiMaterialColor class. Assigns properties of the specified material traits
    to this material traits object.
    
    \param other  [in]  Object that stores properties to be assigned.
  */
  void operator =(const OdGiMaterialTraitsData& other)
  {
    m_ambientColor        = other.m_ambientColor;
    m_diffuseColor        = other.m_diffuseColor;
    m_diffuseMap          = other.m_diffuseMap;
    m_specularColor       = other.m_specularColor;
    m_specularMap         = other.m_specularMap;
    m_glossFactor         = other.m_glossFactor;
    m_reflectionMap       = other.m_reflectionMap;
    m_opacity             = other.m_opacity;
    m_opacityMap          = other.m_opacityMap;
    m_bumpMap             = other.m_bumpMap;
    m_refractionIndex     = other.m_refractionIndex;
    m_refractionMap       = other.m_refractionMap;
    m_translucenceVal     = other.m_translucenceVal;
    m_selfIllumVal        = other.m_selfIllumVal;
    m_reflectivityVal     = other.m_reflectivityVal;
    m_illumModel          = other.m_illumModel;
    m_channelFlags        = other.m_channelFlags;
    m_mode                = other.m_mode;
    // Extended material properties
    m_bleedScale          = other.m_bleedScale;
    m_indirectBumpScale   = other.m_indirectBumpScale;
    m_reflectScale        = other.m_reflectScale;
    m_transmitScale       = other.m_transmitScale;
    m_twoSided            = other.m_twoSided;
    m_lumiMode            = other.m_lumiMode;
    m_lumiVal             = other.m_lumiVal;
    m_normalMap           = other.m_normalMap;
    m_normalMethod        = other.m_normalMethod;
    m_normalStrength      = other.m_normalStrength;
    m_globIllum           = other.m_globIllum;
    m_fgMode              = other.m_fgMode;
    // BIM extensions
    m_emissionColor       = other.m_emissionColor;
    m_emissionMap         = other.m_emissionMap;
    m_tintColor           = other.m_tintColor;
    m_shadingAmbientColor = other.m_shadingAmbientColor;
    m_shadingDiffuseColor = other.m_shadingDiffuseColor;
    m_shadingSpeclrColor  = other.m_shadingSpeclrColor;
    m_shadingOpacity      = other.m_shadingOpacity;
    m_diffuseColorMode    = other.m_diffuseColorMode;
    // Visualize extensions
    m_shadowsOpacity      = other.m_shadowsOpacity;
    m_specularOverride    = other.m_specularOverride;
    m_nslmEffect          = other.m_nslmEffect;
  }
  
  /** \details
    Equality operator for the OdGiMaterialTraitsData class. Compares properties of the specified material traits
    to properties of this material traits object.
    
    \param other  [in] Other object to be compared.
    
    \returns
    true if all properties of the passed object are equal to properties pf this object, false otherwise.
  */
  bool operator ==(const OdGiMaterialTraitsData& other) const
  {
    if (m_ambientColor        != other.m_ambientColor)        return false;
    if (m_diffuseColor        != other.m_diffuseColor)        return false;
    if (m_diffuseMap          != other.m_diffuseMap)          return false;
    if (m_specularColor       != other.m_specularColor)       return false;
    if (m_specularMap         != other.m_specularMap)         return false;
    if (m_glossFactor         != other.m_glossFactor)         return false;
    if (m_reflectionMap       != other.m_reflectionMap)       return false;
    if (m_opacity             != other.m_opacity)             return false;
    if (m_opacityMap          != other.m_opacityMap)          return false;
    if (m_bumpMap             != other.m_bumpMap)             return false;
    if (m_refractionIndex     != other.m_refractionIndex)     return false;
    if (m_refractionMap       != other.m_refractionMap)       return false;
    if (m_translucenceVal     != other.m_translucenceVal)     return false;
    if (m_selfIllumVal        != other.m_selfIllumVal)        return false;
    if (m_reflectivityVal     != other.m_reflectivityVal)     return false;
    if (m_illumModel          != other.m_illumModel)          return false;
    if (m_channelFlags        != other.m_channelFlags)        return false;
    if (m_mode                != other.m_mode)                return false;
    // Extended material properties
    if (m_bleedScale          != other.m_bleedScale)          return false;
    if (m_indirectBumpScale   != other.m_indirectBumpScale)   return false;
    if (m_reflectScale        != other.m_reflectScale)        return false;
    if (m_transmitScale       != other.m_transmitScale)       return false;
    if (m_twoSided            != other.m_twoSided)            return false;
    if (m_lumiMode            != other.m_lumiMode)            return false;
    if (m_lumiVal             != other.m_lumiVal)             return false;
    if (m_normalMap           != other.m_normalMap)           return false;
    if (m_normalMethod        != other.m_normalMethod)        return false;
    if (m_normalStrength      != other.m_normalStrength)      return false;
    if (m_globIllum           != other.m_globIllum)           return false;
    if (m_fgMode              != other.m_fgMode)              return false;
    // BIM extensions
    if (m_emissionColor       != other.m_emissionColor)       return false;
    if (m_emissionMap         != other.m_emissionMap)         return false;
    if (m_tintColor           != other.m_tintColor)           return false;
    if (m_shadingAmbientColor != other.m_shadingAmbientColor) return false;
    if (m_shadingDiffuseColor != other.m_shadingDiffuseColor) return false;
    if (m_shadingSpeclrColor  != other.m_shadingSpeclrColor)  return false;
    if (m_shadingOpacity      != other.m_shadingOpacity)      return false;
    if (m_diffuseColorMode    != other.m_diffuseColorMode)    return false;
    // Visualize extensions
    if (m_shadowsOpacity      != other.m_shadowsOpacity)      return false;
    if (m_specularOverride    != other.m_specularOverride)    return false;
    if (m_nslmEffect          != other.m_nslmEffect)          return false;
    return true;
  }
  
  /** \details
    Inequality operator for the OdGiMaterialTraitsData class. Compares properties of the specified material traits
    to properties of this material traits object.
    
    \param other  [in] Other object to be compared.
    
    \returns
    true if at least one property of the passed object differs from the corresponding property of this object, false if all properties are equal.
  */
  bool operator !=(const OdGiMaterialTraitsData& other) const
  {
    if (m_ambientColor        != other.m_ambientColor)        return true;
    if (m_diffuseColor        != other.m_diffuseColor)        return true;
    if (m_diffuseMap          != other.m_diffuseMap)          return true;
    if (m_specularColor       != other.m_specularColor)       return true;
    if (m_specularMap         != other.m_specularMap)         return true;
    if (m_glossFactor         != other.m_glossFactor)         return true;
    if (m_reflectionMap       != other.m_reflectionMap)       return true;
    if (m_opacity             != other.m_opacity)             return true;
    if (m_opacityMap          != other.m_opacityMap)          return true;
    if (m_bumpMap             != other.m_bumpMap)             return true;
    if (m_refractionIndex     != other.m_refractionIndex)     return true;
    if (m_refractionMap       != other.m_refractionMap)       return true;
    if (m_translucenceVal     != other.m_translucenceVal)     return true;
    if (m_selfIllumVal        != other.m_selfIllumVal)        return true;
    if (m_reflectivityVal     != other.m_reflectivityVal)     return true;
    if (m_illumModel          != other.m_illumModel)          return true;
    if (m_channelFlags        != other.m_channelFlags)        return true;
    if (m_mode                != other.m_mode)                return true;    
    // Extended material properties
    if (m_bleedScale          != other.m_bleedScale)          return true;
    if (m_indirectBumpScale   != other.m_indirectBumpScale)   return true;
    if (m_reflectScale        != other.m_reflectScale)        return true;
    if (m_transmitScale       != other.m_transmitScale)       return true;
    if (m_twoSided            != other.m_twoSided)            return true;
    if (m_lumiMode            != other.m_lumiMode)            return true;
    if (m_lumiVal             != other.m_lumiVal)             return true;
    if (m_normalMap           != other.m_normalMap)           return true;
    if (m_normalMethod        != other.m_normalMethod)        return true;
    if (m_normalStrength      != other.m_normalStrength)      return true;
    if (m_globIllum           != other.m_globIllum)           return true;
    if (m_fgMode              != other.m_fgMode)              return true;
    // BIM extensions
    if (m_emissionColor       != other.m_emissionColor)       return true;
    if (m_emissionMap         != other.m_emissionMap)         return true;
    if (m_tintColor           != other.m_tintColor)           return true;
    if (m_shadingAmbientColor != other.m_shadingAmbientColor) return true;
    if (m_shadingDiffuseColor != other.m_shadingDiffuseColor) return true;
    if (m_shadingSpeclrColor  != other.m_shadingSpeclrColor)  return true;
    if (m_shadingOpacity      != other.m_shadingOpacity)      return true;
    if (m_diffuseColorMode    != other.m_diffuseColorMode)    return true;
    // Visualize extensions
    if (m_shadowsOpacity      != other.m_shadowsOpacity)      return true;
    if (m_specularOverride    != other.m_specularOverride)    return true;
    if (m_nslmEffect          != other.m_nslmEffect)          return true;
    return false;
  }

  /** \details
    Sets the ambient color.
    \param ambientColor [in]  Ambient color.
  */
  void ambient(OdGiMaterialColor& ambientColor) const
  {
    ambientColor = m_ambientColor;
  }
  
  /** \details
    Sets the diffuse color.
    \param diffuseColor [in]  Diffuse color.
    \param diffuseMap [in]  Diffuse map.
  */
  void diffuse(OdGiMaterialColor& diffuseColor, OdGiMaterialMap& diffuseMap) const
  {
    diffuseColor = m_diffuseColor;
    diffuseMap = m_diffuseMap;
  }
  
  /** \details
    Sets the specular (shine) color.
    \param specularColor [in]  Specular color.
    \param specularMap [in]  Specular map.
    \param glossFactor [in]  Glossiness factor.
  */
  void specular(OdGiMaterialColor& specularColor, OdGiMaterialMap& specularMap, double& glossFactor) const
  {
    specularColor = m_specularColor;
    specularMap = m_specularMap;
    glossFactor = m_glossFactor;
  }
  
  /** \details
    Sets the reflection map.
    \param reflectionMap [in]  Reflection map.
  */
  void reflection(OdGiMaterialMap& reflectionMap) const
  {
    reflectionMap = m_reflectionMap;
  }
  
  /** \details
    Sets the opacity properties.
    \param opacity [in]  Opacity factor.
    \param opacityMap [in]  Opacity map.
  */
  void opacity(double& opacity, OdGiMaterialMap& opacityMap) const
  {
    opacity = m_opacity;
    opacityMap = m_opacityMap;
  }
  
  /** \details
    Sets the bump map.
    \param bumpMap [in]  Bump map.
  */
  void bump(OdGiMaterialMap& bumpMap) const
  {
    bumpMap = m_bumpMap;
  }
  
  /** \details
    Sets the refraction properties.
    \param refractionIndex [in]  Index of refraction for a physical material. See online tables with values for specific physical materials.
    \param refractionMap [in]  Reflection map.
  */
  void refraction(double& refractionIndex, OdGiMaterialMap& refractionMap) const
  {
    refractionIndex = m_refractionIndex;
    refractionMap = m_refractionMap;
  }
  
  /** \details
    Retrieves the translucence value.
    \returns
    translucence as a value of the double type.
  */
  double translucence() const
  {
    return m_translucenceVal;
  }
  
  /** \details
    Retrieves the self-illumination amount.
    \returns
    self-illumination as a value of the double type.
  */
  double selfIllumination() const
  {
    return m_selfIllumVal;
  }
  
  /** \details
    Retrieves the reflectivity value.
    \returns
    reflectivity as a value of the double type.
  */
  double reflectivity() const
  {
    return m_reflectivityVal;
  }
  /** \details
    Retrieves the illumination model.
    \returns
    illumination model represented by the value from the IlluminationModel enumeration.
  */
  OdGiMaterialTraits::IlluminationModel illuminationModel() const
  {
    return m_illumModel;
  }
  
  /** \details
    Retrieves the material channels that are enabled.
    \returns
    channel flags of a material represented by a value from the ChannelFlags enumeration.
  */
  OdGiMaterialTraits::ChannelFlags channelFlags() const
  {
    return m_channelFlags;
  }
  
  /** \details
    Retrieves the material model.
    \returns
    material model represented by a value from the Mode enumeration.
  */
  OdGiMaterialTraits::Mode mode() const
  {
    return m_mode;
  }

  /** \details
    Sets the ambient color component.

    \param ambientColor [in]  Ambient color.

    \remarks
    The ambient color component is most apparent when there is no direct illumination on the entity.
  */
  void setAmbient(const OdGiMaterialColor& ambientColor)
  {
    m_ambientColor = ambientColor;
  }
  
  /** \details
    Sets the diffuse component of this object.

    \param diffuseColor [in]  Diffuse color.
    \param diffuseMap [in]  Diffuse map.

    \remarks
    The diffuse component is most apparent when there is direct illumination on the entity.
  */
  void setDiffuse(const OdGiMaterialColor& diffuseColor, const OdGiMaterialMap& diffuseMap)
  {
    m_diffuseColor = diffuseColor;
    m_diffuseMap = diffuseMap;
  }
  
  /** \details
    Sets the specular component of this object.

    \param specularColor [in]  Specular color.
    \param specularMap [in]  Specular map.
    \param glossFactor [in]  Gloss factor.

    \remarks
    The specular component depends on the viewer's eye position and represents light source reflection highlights on the entity surface.
  */
  void setSpecular(const OdGiMaterialColor& specularColor, const OdGiMaterialMap& specularMap, double glossFactor)
  {
    m_specularColor = specularColor;
    m_specularMap = specularMap;
    m_glossFactor = glossFactor;
  }
  
  /** \details
    Sets the reflection component of this object.

    \param reflectionMap [in]  Reflection map.

    \remarks
    The reflection component creates a mirror finish on the entity.
  */
  void setReflection(const OdGiMaterialMap& reflectionMap)
  {
    m_reflectionMap = reflectionMap;
  }
  
  /** \details
    Sets the opacity component of this object.

    \param opacityPercentage [in]  Opacity percentage.
    \param opacityMap [in]  Opacity map.
  */
  void setOpacity(double opacity, const OdGiMaterialMap& opacityMap)
  {
    m_opacity = opacity;
    m_opacityMap = opacityMap;
  }
  
  /** \details
    Sets the bump component of this object.

    \param bumpMap [in]  Bump map.
  */
  void setBump(const OdGiMaterialMap& bumpMap)
  {
    m_bumpMap = bumpMap;
  }
  
  /** \details
    Sets the refraction component of this object.

    \param refractionIndex [in]  Refraction index.
    \param refractionMap [in]  Refraction map.
  */
  void setRefraction(double refractionIndex, const OdGiMaterialMap& refractionMap)
  {
    m_refractionIndex = refractionIndex;
    m_refractionMap = refractionMap;
  }
  
  /** \details
    Sets the translucence of this object.

    \param value [in]  Translucence value.
  */
  void setTranslucence(double value)
  {
    m_translucenceVal = value;
  }
  
  /** \details
    Sets the self illumination of this object.

    \param value [in]  Self illumination level.
  */
  void setSelfIllumination(double value)
  {
    m_selfIllumVal = value;
  }
  
  /** \details
    Sets the reflectivity of this object.

    \param value [in]  Reflectivity value.
  */
  void setReflectivity(double value)
  {
    m_reflectivityVal = value;
  }
  
  /** \details
    Sets the illumination model of this object.

    \param model [in]  Illumination model.
  */
  void setIlluminationModel(OdGiMaterialTraits::IlluminationModel model)
  {
    m_illumModel = model;
  }
  
  /** \details
    Sets the material channels to be used by this MaterialTraits object.

    \param value [in]  Channels to be used.
  */
  void setChannelFlags(OdGiMaterialTraits::ChannelFlags flags)
  {
    m_channelFlags = flags;
  }
  
  /** \details
    Sets the mode of this object.

    \param value [in]  Mode value.
  */
  void setMode(OdGiMaterialTraits::Mode value)
  {
    m_mode = value;
  }

  // Extended material properties

  
  /** \details
    Sets the color bleed scale of this object.

    \param scale [in]  Color bleed scale.
  */
  void setColorBleedScale(double scale)
  {
    m_bleedScale = scale;
  }
  
  /** \details
    Retrieves the color bleed scale of this object.
    
    \returns
    color bleed scale as a value of the double type.
  */
  double colorBleedScale() const
  {
    return m_bleedScale;
  }

  /** \details
    Sets the indirect bump scale of this object.

    \param scale [in]  Indirect bump scale.
  */
  void setIndirectBumpScale(double scale)
  {
    m_indirectBumpScale = scale;
  }
  
  /** \details
    Retrieves the indirect bump scale of this object.
    
    \returns
    indirect bump scale as a value of the double type.
  */
  double indirectBumpScale() const
  {
    return m_indirectBumpScale;
  }

  /** \details
    Sets the reflectance scale of this object.

    \param scale [in]  Reflectance scale.
  */
  void setReflectanceScale(double scale)
  {
    m_reflectScale = scale;
  }
  
  /** \details
    Retrieves the reflectance scale of this object.
    
    \returns
    reference scale as a value of the double type.
  */
  double reflectanceScale() const
  {
    return m_reflectScale;
  }

  /** \details
    Sets the transmittance scale of this object.

    \param scale [in]  Transmittance scale.
  */
  void setTransmittanceScale(double scale)
  {
    m_transmitScale = scale;
  }
  
  /** \details
    Retrieves the transmittance scale of this object.
    
    \returns
    transmittance scale as a value of the double type.
  */
  double transmittanceScale() const
  {
    return m_transmitScale;
  }

  /** \details
    Sets the two-sided (individual materials for front and back faces) mode of this object.

    \param flag [in]  Two-sided mode flag.
  */
  void setTwoSided(bool flag)
  {
    m_twoSided = flag;
  }
  
  /** \details
    Retrieves the two-sided (individual materials for front and back faces) mode of this object.
    \returns
    true if the two-sided mode is enabled, false otherwise.
  */
  bool twoSided() const
  {
    return m_twoSided;
  }

  /** \details
    Sets the luminance mode of this object.

    \param mode [in]  Luminance mode.
  */
  void setLuminanceMode(OdGiMaterialTraits::LuminanceMode mode)
  {
    m_lumiMode = mode;
  }
  
  /** \details
    Retrieves the luminance mode of this object.
    
    \returns
    a value from the LuminanceMode enumeration that represents one of the luminance modes.
  */
  OdGiMaterialTraits::LuminanceMode luminanceMode() const
  {
    return m_lumiMode;
  }

  /** \details
    Sets the luminance of this object.

    \param value [in]  Luminance value.
  */
  void setLuminance(double value)
  {
    m_lumiVal = value;
  }
  
  /** \details
    Retrieves the luminance of this object.
    
    \returns
    luminance amount as a value of the double type.
  */
  double luminance() const
  {
    return m_lumiVal;
  }

  /** \details
    Sets the normalMap component of this object.

    \param normalMap [in]  Normal map.
    \param method [in]  Normal map method.
    \param strength [in]  Strength factor.
  */
  void setNormalMap(const OdGiMaterialMap &normalMap, OdGiMaterialTraits::NormalMapMethod method, double strength)
  {
    m_normalMap = normalMap;
    m_normalMethod = method;
    m_normalStrength = strength;
  }
  
  /** \details
    Retrieves the normalMap component of this object.

    \param normalMap [out]  Receives the normal map.
    \param method [out]  Receives the normal map method.
    \param strength [out]  Receives the normal map strength factor.
  */
  void normalMap(OdGiMaterialMap &normalMap, OdGiMaterialTraits::NormalMapMethod &method, double &strength) const
  {
    normalMap = m_normalMap;
    method = m_normalMethod;
    strength = m_normalStrength;
  }

  /** \details
    Sets the global illumination mode of this object.

    \param mode [in]  Global illumination mode.
  */
  void setGlobalIllumination(OdGiMaterialTraits::GlobalIlluminationMode mode)
  {
    m_globIllum = mode;
  }
  
  /** \details
    Retrieves the global illumination mode of this object.
    
    \returns
    global illumination mode as a value from the GlobalIlluminationMode enumeration.
  */
  OdGiMaterialTraits::GlobalIlluminationMode globalIllumination() const
  {
    return m_globIllum;
  }

  /** \details
    Sets the final gather mode of this object.

    \param mode [in]  Final gather mode.
  */
  void setFinalGather(OdGiMaterialTraits::FinalGatherMode mode)
  {
    m_fgMode = mode;
  }
  
  /** \details
    Retrieves the final gather mode of this object.
    
    \returns
    final gathering mode as a value from the FinalGatherMode enumeration.
  */
  OdGiMaterialTraits::FinalGatherMode finalGather() const
  {
    return m_fgMode;
  }

  // BIM extensions

  
  /** \details
    Sets the emission component of this object.

    \param emissionColor [in]  Emission color.
    \param emissionMap [in]  Emission map.
  */
  void setEmission(const OdGiMaterialColor& emissionColor, const OdGiMaterialMap& emissionMap)
  {
    m_emissionColor = emissionColor;
    m_emissionMap = emissionMap;
  }
  
  /** \details
    Retrieves the emission component of this object.

    \param emissionColor [out]  Receives the emission color.
    \param emissionMap [out]  Receives the emission map.
  */
  void emission(OdGiMaterialColor& emissionColor, OdGiMaterialMap& emissionMap) const
  {
    emissionMap = m_emissionMap;
    emissionColor = m_emissionColor;
  }

  /** \details
    Sets the tint component of this object.

    \param tintColor [in]  Tint color.
  */
  void setTint(const OdGiMaterialColor& tintColor)
  {
    m_tintColor = tintColor;
  }
  
  /** \details
    Retrieves the tint component of this object.

    \param tintColor [out]  Receives the tint color.
  */
  void tint(OdGiMaterialColor& tintColor) const
  {
    tintColor = m_tintColor;
  }

  /** \details
    Sets the shading ambient color component of this object.

    \param ambientColor [in]  Shading ambient color.
  */
  void setShadingAmbient(const OdGiMaterialColor& ambientColor)
  {
    m_shadingAmbientColor = ambientColor;
  }
  
  /** \details
    Retrieves the shading ambient color component of this object.

    \param ambientColor [out]  Receives the shading ambient color.
  */
  void shadingAmbient(OdGiMaterialColor& ambientColor) const
  {
    ambientColor = m_shadingAmbientColor;
  }

  /** \details
    Sets the shading diffuse color component of this object.

    \param diffuseColor [in]  Shading diffuse color.
  */
  void setShadingDiffuse(const OdGiMaterialColor& diffuseColor)
  {
    m_shadingDiffuseColor = diffuseColor;
  }
  
  /** \details
    Retrieves the shading diffuse color component of this object.

    \param diffuseColor [out]  Receives the shading diffuse color.
  */
  void shadingDiffuse(OdGiMaterialColor& diffuseColor) const
  {
    diffuseColor = m_shadingDiffuseColor;
  }

  /** \details
    Sets the shading specular color component of this object.

    \param specularColor [in]  Shading specular color.
  */
  void setShadingSpecular(const OdGiMaterialColor& specularColor)
  {
    m_shadingSpeclrColor = specularColor;
  }
  
  /** \details
    Retrieves the shading specular color component of this object.

    \param specularColor [out]  Receives the shading specular color.
  */
  void shadingSpecular(OdGiMaterialColor& specularColor) const
  {
    specularColor = m_shadingSpeclrColor;
  }

  /** \details
    Sets the shading opacity component of this object.

    \param opacityPercentage [in]  Shading opacity percentage.
  */
  void setShadingOpacity(double opacityPercentage)
  {
    m_shadingOpacity = opacityPercentage;
  }
  
  /** \details
    Retrieves the shading opacity component of this object.

    \param opacityPercentage [out]  Receives the shading opacity percentage.
  */
  void shadingOpacity(double &opacityPercentage) const
  {
    opacityPercentage = m_shadingOpacity;
  }

  /** \details
    Sets diffuse, ambient, specual color and opacity behavior.

    \param diffuseColorMode [in]  New setting for diffuse color and opacity mode.
  */
  void setDiffuseColorMode(OdGiMaterialTraits::DiffuseColorMode diffuseColorMode)
  {
    m_diffuseColorMode = diffuseColorMode;
  }
  
  /** \details
    Retrieves current diffuse, ambient, specular color and opacity behavior.
    \returns
    diffuse color mode as a value from the DiffuseColorMode enumeration.
  */
  OdGiMaterialTraits::DiffuseColorMode diffuseColorMode() const
  {
    return m_diffuseColorMode;
  }

  // Visualize extensions

  void setShadowsOpacityEffect(double shadowsOpacity, double nonShadowLightsMergeEffect)
  {
    m_shadowsOpacity = shadowsOpacity;
    m_nslmEffect = nonShadowLightsMergeEffect;
  }
  double shadowsOpacityEffect(double &nonShadowLightsMergeEffect) const
  {
    nonShadowLightsMergeEffect = m_nslmEffect;
    return m_shadowsOpacity;
  }

  void setSpecularHighlightingOverride(double overrideVal)
  {
    m_specularOverride = overrideVal;
  }
  double specularHighlightingOverride() const
  {
    return m_specularOverride;
  }
};

/** \details
    
    \sa
    <link tv_working_with_attributes_materials, Work with Materials>
   
    <group OdGi_Classes>
*/
class ODGI_EXPORT OdGiMaterialTraitsTaker : public OdGiMaterialTraits
{
protected:
  OdGiMaterialTraitsData m_data;
public:
  //ODRX_DECLARE_MEMBERS(OdGiMaterialTraitsTaker);
  ODRX_USING_HEAP_OPERATORS(OdGiMaterialTraits);

  /** \details
    Adds one reference to this object. Does nothing in current implementation.
  */
  void addRef() { }
  
  /** \details
    Removes one reference to this object. Does nothing in current implementation.
  */
  void release() { }

  /** \details
    Sets the ambient color.
    \param ambientColor [in]  Ambient color.
  */
  virtual void ambient(OdGiMaterialColor& ambientColor) const
  {
    m_data.ambient(ambientColor);
  }
  
  /** \details
    Sets the diffuse color.
    \param diffuseColor [in]  Diffuse color.
    \param diffuseMap [in]  Diffuse map.
  */
  virtual void diffuse(OdGiMaterialColor& diffuseColor, OdGiMaterialMap& diffuseMap) const
  {
    m_data.diffuse(diffuseColor, diffuseMap);
  }
  
  /** \details
    Sets the specular (shine) color.
    \param specularColor [in]  Specular color.
    \param specularMap [in]  Specular map.
    \param glossFactor [in]  Glossiness factor.
  */
  virtual void specular(OdGiMaterialColor& specularColor, OdGiMaterialMap& specularMap, double& glossFactor) const
  {
    m_data.specular(specularColor, specularMap, glossFactor);
  }
  
  /** \details
    Sets the reflection map.
    \param reflectionMap [in]  Reflection map.
  */
  virtual void reflection(OdGiMaterialMap& reflectionMap) const
  {
    m_data.reflection(reflectionMap);
  }
  
  /** \details
    Sets the opacity properties.
    \param opacity [in]  Opacity factor.
    \param opacityMap [in]  Opacity map.
  */
  virtual void opacity(double& opacityPercentage, OdGiMaterialMap& opacityMap) const
  {
    m_data.opacity(opacityPercentage, opacityMap);
  }
  
  /** \details
    Sets the bump map.
    \param bumpMap [in]  Bump map.
  */
  virtual void bump(OdGiMaterialMap& bumpMap) const
  {
    m_data.bump(bumpMap);
  }
  
  /** \details
    Sets the refraction properties.
    \param refractionIndex [in]  Index of refraction for a physical material. See online tables with values for specific physical materials.
    \param refractionMap [in]  Reflection map.
  */
  virtual void refraction(double& refractionIndex, OdGiMaterialMap& refractionMap) const
  {
    m_data.refraction(refractionIndex, refractionMap);
  }
  
  /** \details
    Retrieves the translucence value.
    \returns
    translucence as a value of the double type.
  */
  virtual double translucence() const
  {
    return m_data.translucence();
  }
  
  /** \details
    Retrieves the self-illumination amount.
    \returns
    self-illumination as a value of the double type.
  */
  virtual double selfIllumination() const
  {
    return m_data.selfIllumination();
  }
  
  /** \details
    Retrieves the reflectivity value.
    \returns
    reflectivity as a value of the double type.
  */
  virtual double reflectivity() const
  {
    return m_data.reflectivity();
  }
  
  /** \details
    Retrieves the illumination model.
    \returns
    illumination model represented by the value from the IlluminationModel enumeration.
  */
  virtual IlluminationModel illuminationModel() const
  {
    return m_data.illuminationModel();
  }
  
  /** \details
    Retrieves the material channels that are enabled.
    \returns
    channel flags of a material represented by a value from the ChannelFlags enumeration.
  */
  virtual ChannelFlags channelFlags() const
  {
    return m_data.channelFlags();
  }
  
  /** \details
    Retrieves the material model.
    \returns
    material model represented by a value from the Mode enumeration.
  */
  virtual Mode mode() const
  {
    return m_data.mode();
  }

  /** \details
    Sets the ambient color component.

    \param ambientColor [in]  Ambient color.

    \remarks
    The ambient color component is most apparent when there is no direct illumination on the entity.
  */
  virtual void setAmbient(const OdGiMaterialColor& ambientColor)
  {
    m_data.setAmbient(ambientColor);
  }
  
  /** \details
    Sets the diffuse component of this object.

    \param diffuseColor [in]  Diffuse color.
    \param diffuseMap [in]  Diffuse map.

    \remarks
    The diffuse component is most apparent when there is direct illumination on the entity.
  */
  virtual void setDiffuse(const OdGiMaterialColor& diffuseColor, const OdGiMaterialMap& diffuseMap)
  {
    m_data.setDiffuse(diffuseColor, diffuseMap);
  }
  
  /** \details
    Sets the specular component of this object.

    \param specularColor [in]  Specular color.
    \param specularMap [in]  Specular map.
    \param glossFactor [in]  Gloss factor.

    \remarks
    The specular component depends on the viewer's eye position and represents light source reflection highlights on the entity surface.
  */
  virtual void setSpecular(const OdGiMaterialColor& specularColor, const OdGiMaterialMap& specularMap, double glossFactor)
  {
    m_data.setSpecular(specularColor, specularMap, glossFactor);
  }
  
  /** \details
    Sets the reflection component of this object.

    \param reflectionMap [in]  Reflection map.

    \remarks
    The reflection component creates a mirror finish on the entity.
  */
  virtual void setReflection(const OdGiMaterialMap& reflectionMap)
  {
    m_data.setReflection(reflectionMap);
  }
  
  /** \details
    Sets the opacity component of this object.

    \param opacityPercentage [in]  Opacity percentage.
    \param opacityMap [in]  Opacity map.
  */
  virtual void setOpacity(double opacityPercentage, const OdGiMaterialMap& opacityMap)
  {
    m_data.setOpacity(opacityPercentage, opacityMap);
  }
  
  /** \details
    Sets the bump component of this object.

    \param bumpMap [in]  Bump map.
  */
  virtual void setBump(const OdGiMaterialMap& bumpMap)
  {
    m_data.setBump(bumpMap);
  }
  
  /** \details
    Sets the refraction component of this object.

    \param refractionIndex [in]  Refraction index.
    \param refractionMap [in]  Refraction map.
  */
  virtual void setRefraction(double refractionIndex, const OdGiMaterialMap& refractionMap)
  {
    m_data.setRefraction(refractionIndex, refractionMap);
  }
  
  /** \details
    Sets the translucence of this object.

    \param value [in]  Translucence value.
  */
  virtual void setTranslucence(double value)
  {
    m_data.setTranslucence(value);
  }
  
  /** \details
    Sets the self illumination of this object.

    \param value [in]  Self illumination level.
  */
  virtual void setSelfIllumination(double value)
  {
    m_data.setSelfIllumination(value);
  }
  
  /** \details
    Sets the reflectivity of this object.

    \param value [in]  Reflectivity value.
  */
  virtual void setReflectivity(double value)
  {
    m_data.setReflectivity(value);
  }
  
  /** \details
    Sets the illumination model of this object.

    \param model [in]  Illumination model.
  */
  virtual void setIlluminationModel(IlluminationModel model)
  {
    m_data.setIlluminationModel(model);
  }
  
  /** \details
    Sets the material channels to be used by this MaterialTraits object.

    \param value [in]  Channels to be used.
  */
  virtual void setChannelFlags(ChannelFlags flags)
  {
    m_data.setChannelFlags(flags);
  }
  
  /** \details
    Sets the mode of this object.

    \param value [in]  Mode value.
  */
  virtual void setMode(Mode value)
  {
    m_data.setMode(value);
  }

  /** \details
    Sets the color bleed scale of this object.

    \param scale [in]  Color bleed scale.
  */
  virtual void setColorBleedScale(double scale)
  {
    m_data.setColorBleedScale(scale);
  }
  
  /** \details
    Retrieves the color bleed scale of this object.
    
    \returns
    color bleed scale as a value of the double type.
  */
  virtual double colorBleedScale() const
  {
    return m_data.colorBleedScale();
  }

  /** \details
    Sets the indirect bump scale of this object.

    \param scale [in]  Indirect bump scale.
  */
  virtual void setIndirectBumpScale(double scale)
  {
    m_data.setIndirectBumpScale(scale);
  }
  
  /** \details
    Retrieves the indirect bump scale of this object.
    
    \returns
    indirect bump scale as a value of the double type.
  */
  virtual double indirectBumpScale() const
  {
    return m_data.indirectBumpScale();
  }

  /** \details
    Sets the reflectance scale of this object.

    \param scale [in]  Reflectance scale.
  */
  virtual void setReflectanceScale(double scale)
  {
    m_data.setReflectanceScale(scale);
  }
  
  /** \details
    Retrieves the reflectance scale of this object.
    
    \returns
    reference scale as a value of the double type.
  */
  virtual double reflectanceScale() const
  {
    return m_data.reflectanceScale();
  }

  /** \details
    Sets the transmittance scale of this object.

    \param scale [in]  Transmittance scale.
  */
  virtual void setTransmittanceScale(double scale)
  {
    m_data.setTransmittanceScale(scale);
  }
  
  /** \details
    Retrieves the transmittance scale of this object.
    
    \returns
    transmittance scale as a value of the double type.
  */
  virtual double transmittanceScale() const
  {
    return m_data.transmittanceScale();
  }

  /** \details
    Sets the two-sided (individual materials for front and back faces) mode of this object. 

    \param flag [in]  Two-sided mode flag.
  */
  virtual void setTwoSided(bool flag)
  {
    m_data.setTwoSided(flag);
  }
  
  /** \details
    Checks whether two-sided mode (individual materials for front and back faces) is enabled.
    \returns
    true if two-sided mode is enabled, false otherwise.
  */
  virtual bool twoSided() const
  {
    return m_data.twoSided();
  }

  /** \details
    Sets the luminance mode of this object.

    \param mode [in]  Luminance mode.
  */
  virtual void setLuminanceMode(LuminanceMode mode)
  {
    m_data.setLuminanceMode(mode);
  }
  
  /** \details
    Retrieves the luminance mode of this object.
    
    \returns
    a value from the LuminanceMode enumeration that represents one of the luminance modes.
  */
  virtual LuminanceMode luminanceMode() const
  {
    return m_data.luminanceMode();
  }

  /** \details
    Sets the luminance of this object.

    \param value [in]  Luminance value.
  */
  virtual void setLuminance(double value)
  {
    m_data.setLuminance(value);
  }
  
  /** \details
    Retrieves the luminance of this object.
    
    \returns
    luminance amount as a value of the double type.
  */
  virtual double luminance() const
  {
    return m_data.luminance();
  }

  /** \details
    Sets the normalMap component of this object.

    \param normalMap [in]  Normal map.
    \param method [in]  Normal map method.
    \param strength [in]  Strength factor.
  */
  virtual void setNormalMap(const OdGiMaterialMap &normalMap, NormalMapMethod method, double strength)
  {
    m_data.setNormalMap(normalMap, method, strength);
  }
  
  /** \details
    Retrieves the normalMap component of this object.

    \param normalMap [out]  Receives the normal map.
    \param method [out]  Receives the normal map method.
    \param strength [out]  Receives the normal map strength factor.
  */
  virtual void normalMap(OdGiMaterialMap &normalMap, NormalMapMethod &method, double &strength) const
  {
    m_data.normalMap(normalMap, method, strength);
  }

  /** \details
    Sets the global illumination mode of this object.

    \param mode [in]  Global illumination mode.
  */
  virtual void setGlobalIllumination(GlobalIlluminationMode mode)
  {
    m_data.setGlobalIllumination(mode);
  }
  
  /** \details
    Retrieves the global illumination mode of this object.
    
    \returns
    global illumination mode as a value from the GlobalIlluminationMode enumeration.
  */
  virtual GlobalIlluminationMode globalIllumination() const
  {
    return m_data.globalIllumination();
  }

  /** \details
    Sets the final gather mode of this object.

    \param mode [in]  Final gather mode.
  */
  virtual void setFinalGather(FinalGatherMode mode)
  {
    m_data.setFinalGather(mode);
  }
  
  /** \details
    Retrieves the final gather mode of this object.
    
    \returns
    final gathering mode as a value from the FinalGatherMode enumeration.
  */
  virtual FinalGatherMode finalGather() const
  {
    return m_data.finalGather();
  }

  /** \details
    Sets the emission component of this object.

    \param emissionColor [in]  Emission color.
    \param emissionMap [in]  Emission map.
  */
  virtual void setEmission(const OdGiMaterialColor& emissionColor, const OdGiMaterialMap& emissionMap)
  {
    m_data.setEmission(emissionColor, emissionMap);
  }
  
  /** \details
    Retrieves the emission component of this object.

    \param emissionColor [out]  Receives the emission color.
    \param emissionMap [out]  Receives the emission map.
  */
  virtual void emission(OdGiMaterialColor& emissionColor, OdGiMaterialMap& emissionMap) const
  {
    m_data.emission(emissionColor, emissionMap);
  }

  /** \details
    Sets the tint component of this object.

    \param tintColor [in]  Tint color.
  */
  virtual void setTint(const OdGiMaterialColor& tintColor)
  {
    m_data.setTint(tintColor);
  }
  
  /** \details
    Retrieves the tint component of this object.

    \param tintColor [out]  Receives the tint color.
  */
  virtual void tint(OdGiMaterialColor& tintColor) const
  {
    m_data.tint(tintColor);
  }

  /** \details
    Sets the shading ambient color component of this object.

    \param ambientColor [in]  Shading ambient color.
  */
  virtual void setShadingAmbient(const OdGiMaterialColor& ambientColor)
  {
    m_data.setShadingAmbient(ambientColor);
  }
  
  /** \details
    Retrieves the shading ambient color component of this object.

    \param ambientColor [out]  Receives the shading ambient color.
  */
  virtual void shadingAmbient(OdGiMaterialColor& ambientColor) const
  {
    m_data.shadingAmbient(ambientColor);
  }

  /** \details
    Sets the shading diffuse color component of this object.

    \param diffuseColor [in]  Shading diffuse color.
  */
  virtual void setShadingDiffuse(const OdGiMaterialColor& diffuseColor)
  {
    m_data.setShadingDiffuse(diffuseColor);
  }
  
  /** \details
    Retrieves the shading diffuse color component of this object.

    \param diffuseColor [out]  Receives the shading diffuse color.
  */
  virtual void shadingDiffuse(OdGiMaterialColor& diffuseColor) const
  {
    m_data.shadingDiffuse(diffuseColor);
  }

  /** \details
    Sets the shading specular color component of this object.

    \param specularColor [in]  Shading specular color.
  */
  virtual void setShadingSpecular(const OdGiMaterialColor& specularColor)
  {
    m_data.setShadingSpecular(specularColor);
  }
  
  /** \details
    Retrieves the shading specular color component of this object.

    \param specularColor [out]  Receives the shading specular color.
  */
  virtual void shadingSpecular(OdGiMaterialColor& specularColor) const
  {
    m_data.shadingSpecular(specularColor);
  }

  
  /** \details
    Sets the shading opacity component of this object.

    \param opacityPercentage [in]  Shading opacity percentage.
  */
  virtual void setShadingOpacity(double opacityPercentage)
  {
    m_data.setShadingOpacity(opacityPercentage);
  }
  
  /** \details
    Retrieves the shading opacity component of this object.

    \param opacityPercentage [out]  Receives the shading opacity percentage.
  */
  virtual void shadingOpacity(double &opacityPercentage) const
  {
    m_data.shadingOpacity(opacityPercentage);
  }

  /** \details
    Sets diffuse, ambient, specual color and opacity behavior.

    \param diffuseColorMode [in]  New setting for diffuse color and opacity mode.
  */
  virtual void setDiffuseColorMode(DiffuseColorMode diffuseColorMode)
  {
    m_data.setDiffuseColorMode(diffuseColorMode);
  }
  
  /** \details
    Retrieves current diffuse, ambient, specular color and opacity behavior.
    \returns
    diffuse color mode as a value from the DiffuseColorMode enumeration.
  */
  virtual DiffuseColorMode diffuseColorMode() const
  {
    return m_data.diffuseColorMode();
  }

virtual void setShadowsOpacityEffect(double effectPercentage, double nonShadowLightsMergeEffect)
  {
    m_data.setShadowsOpacityEffect(effectPercentage, nonShadowLightsMergeEffect);
  }
  virtual double shadowsOpacityEffect(double &nonShadowLightsMergeEffect) const
  {
    return m_data.shadowsOpacityEffect(nonShadowLightsMergeEffect);
  }

  void setSpecularHighlightingOverride(double overrideVal)
  {
    m_data.setSpecularHighlightingOverride(overrideVal);
  }
  double specularHighlightingOverride() const
  {
    return m_data.specularHighlightingOverride();
  }  /** \details
    Retrieves material traits data from this object.
    \param data [out]  Placeholder for material traits.
  */  void getData(OdGiMaterialTraitsData& data) const { data = m_data; }
  
  /** \details
    Retrieves material traits data from this object.
    \returns
    reference to the OdGiMaterialTraitsData that represents material traits data for this object.
  */
  const OdGiMaterialTraitsData &getData() const { return m_data; }

  void setData(const OdGiMaterialTraitsData& data) { m_data = data; }
};

/** \details
  A data type that represents a smart pointer to an <link OdGiMaterialTraitsTaker, OdGiMaterialTraitsTaker> object.
*/
typedef OdSmartPtr<OdGiMaterialTraitsTaker> OdGiMaterialTraitsTakerPtr;

/** \details
    
    \sa
    <link tv_working_with_attributes_materials, Work with Materials>
   
    <group OdGi_Classes>
*/
class ODGI_EXPORT OdGiMaterialTraits_Stub : public OdGiMaterialTraits
{
public:
  ODRX_USING_HEAP_OPERATORS(OdGiMaterialTraits);

  OdGiMaterialTraits_Stub() { }

  /** \details
    Adds one reference to this object. Does nothing in current implementation.
  */
  void addRef() { }
  
  /** \details
    Removes one reference to this object. Does nothing in current implementation.
  */
  void release() { }

  void ambient(OdGiMaterialColor& /*ambientColor*/) const { }
  void diffuse(OdGiMaterialColor& /*diffuseColor*/, OdGiMaterialMap& /*diffuseMap*/) const { }
  void specular(OdGiMaterialColor& /*specularColor*/, OdGiMaterialMap& /*specularMap*/, double& /*glossFactor*/) const { }
  void reflection(OdGiMaterialMap& /*reflectionMap*/) const { }
  void opacity(double& /*opacityPercentage*/, OdGiMaterialMap& /*opacityMap*/) const { }
  void bump(OdGiMaterialMap& /*bumpMap*/) const { }
  void refraction(double& /*refractionIndex*/, OdGiMaterialMap& /*refractionMap*/) const { }
  double translucence() const { return 0.0; }
  double selfIllumination() const { return 0.0; }
  double reflectivity() const { return 0.0; }
  IlluminationModel illuminationModel() const { return kBlinnShader; }
  ChannelFlags channelFlags() const { return kUseAll; }
  Mode mode() const { return kRealistic; }

  void setAmbient(const OdGiMaterialColor& /*ambientColor*/) { }
  void setDiffuse(const OdGiMaterialColor& /*diffuseColor*/, const OdGiMaterialMap& /*diffuseMap*/) { }
  void setSpecular(const OdGiMaterialColor& /*specularColor*/, const OdGiMaterialMap& /*specularMap*/, double /*glossFactor*/) { }
  void setReflection(const OdGiMaterialMap& /*reflectionMap*/) { }
  void setOpacity(double /*opacityPercentage*/, const OdGiMaterialMap& /*opacityMap*/) { }
  void setBump(const OdGiMaterialMap& /*bumpMap*/) { }
  void setRefraction(double /*refractionIndex*/, const OdGiMaterialMap& /*refractionMap*/) { }
  void setTranslucence(double /*value*/) { }
  void setSelfIllumination(double /*value*/) { }
  void setReflectivity(double /*value*/) { }
  void setIlluminationModel(IlluminationModel /*model*/) { }
  void setChannelFlags(ChannelFlags /*flags*/) { }
  void setMode(Mode /*value*/) { }

  void setColorBleedScale(double /*scale*/) { }
  double colorBleedScale() const { return 0.0; }

  void setIndirectBumpScale(double /*scale*/) { }
  double indirectBumpScale() const { return 0.0; }

  void setReflectanceScale(double /*scale*/) { }
  double reflectanceScale() const { return 0.0; }

  void setTransmittanceScale(double /*scale*/) { }
  double transmittanceScale() const { return 0.0; }

  void setTwoSided(bool /*flag*/) { }
  bool twoSided() const { return true; }

  void setLuminanceMode(LuminanceMode /*mode*/) { }
  LuminanceMode luminanceMode() const { return kSelfIllumination; }

  void setLuminance(double /*value*/) { }
  double luminance() const { return 0.0; }

  void setNormalMap(const OdGiMaterialMap &/*normalMap*/, NormalMapMethod /*method*/, double /*strength*/) { }
  void normalMap(OdGiMaterialMap &/*normalMap*/, NormalMapMethod &/*method*/, double &/*strength*/) const { }

  void setGlobalIllumination(GlobalIlluminationMode /*mode*/) { }
  GlobalIlluminationMode globalIllumination() const { return kGlobalIlluminationCastAndReceive; }

  void setFinalGather(FinalGatherMode /*mode*/) { }
  FinalGatherMode finalGather() const { return kFinalGatherCastAndReceive; }

  void setEmission(const OdGiMaterialColor& /*emissionColor*/, const OdGiMaterialMap& /*emissionMap*/) { }
  void emission(OdGiMaterialColor& /*emissionColor*/, OdGiMaterialMap& /*emissionMap*/) const { }

  void setTint(const OdGiMaterialColor& /*tintColor*/) { }
  void tint(OdGiMaterialColor& /*tintColor*/) const { }

  void setShadingAmbient(const OdGiMaterialColor& /*ambientColor*/) { }
  void shadingAmbient(OdGiMaterialColor& /*ambientColor*/) const { }

  void setShadingDiffuse(const OdGiMaterialColor& /*diffuseColor*/) { }
  void shadingDiffuse(OdGiMaterialColor& /*diffuseColor*/) const { }

  void setShadingSpecular(const OdGiMaterialColor& /*specularColor*/) { }
  void shadingSpecular(OdGiMaterialColor& /*specularColor*/) const { }

  void setShadingOpacity(double /*opacityPercentage*/) { }
  void shadingOpacity(double& /*opacityPercentage*/) const { }

  void setDiffuseColorMode(DiffuseColorMode /*diffuseColorMode*/) { }
  DiffuseColorMode diffuseColorMode() const { return kDiffuseRealisticColor; }

  void setShadowsOpacityEffect(double /*effectPercentage*/, double /*nonShadowLightsMergeEffect*/) { }
  double shadowsOpacityEffect(double& /*nonShadowLightsMergeEffect*/) const { return 1.0; }

  void setSpecularHighlightingOverride(double /*overrideVal*/) { }
  double specularHighlightingOverride() const { return 0.0; }
};

/** \details
  A data type that represents a smart pointer to an <link OdGiMaterialTraits_Stub, OdGiMaterialTraits_Stub> object.
*/
typedef OdSmartPtr<OdGiMaterialTraits_Stub> OdGiMaterialTraits_StubPtr;

#include "TD_PackPop.h"

#endif // #ifndef _ODGIMATERIALTRAITSDATA_INCLUDED_
