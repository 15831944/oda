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

#ifndef __OD_GI_LIGHT_TRAITS_DATA__
#define __OD_GI_LIGHT_TRAITS_DATA__

#include "TD_PackPush.h"

#include "Gi/GiLightTraits.h"

/** \details
    Corresponding C++ library: TD_Gi
    <group OdGi_Classes>
    \sa
    <link tv_working_with_lights, Work with Lights>
*/
class OdGiLightTraitsData
{
  bool                       m_bOn;
  double                     m_dIntensity;
  OdCmEntityColor            m_color;
  OdGiShadowParameters       m_shadowParameters;

public:
  /** \details
    Represents light types.
  */
  enum LightType
  {
    /** Invalid light. */
    kInvalidLight = -1,
    /** Point light. */
    kPointLight   = OdGiDrawable::kPointLight,
    /** Spot light. */
    kSpotLight    = OdGiDrawable::kSpotLight,
    /** Distant light. */
    kDistantLight = OdGiDrawable::kDistantLight,
    /** Web light. */
    kWebLight     = OdGiDrawable::kWebLight
  };

  /** \details
    Retrieves a light type.
    
    \returns
    light type as a value from the LightType enumeration.
  */
  static LightType drawableLightType(const OdGiDrawable *pDrawable);
protected:
  LightType m_type;

protected:
  OdGiLightTraitsData()
    : m_bOn(false)
    , m_dIntensity(0.0)
    , m_type(kInvalidLight)
  {}

public:
  
  /** \details
    Checks whether light is enabled.
    
    \returns
    true if light is enabled, false otherwise.
  */
  bool isOn() const { return m_bOn; }
  
  /** \details
    Specifies whether light is enabled.
    
    \param value [in]  Flag that specifies whether light is enabled.
  */
  void setOn(bool value) { m_bOn = value; }

  /** \details
    Retrieves intensity of a light source.
    
    \returns
    the intensity of the light source as a value of the double type.
  */
  double intensity() const { return m_dIntensity; }
  
  /** \details
    Specifies intensity of a light source.
    
    \param dIntensity  [in]  Light source intensity.
  */
  void setIntensity(double dIntensity) { m_dIntensity = dIntensity; }

  /** \details
    Retrieves the color of the emitted light.
    
    \returns
    the color of the emitted light as OdCmEntityColor instance.
  */
  OdCmEntityColor color() const { return m_color; }
  
  /** \details
    Specifies the color of the emitted light.
    
    \param color  [in]  Color of the emitted light.
  */
  void setColor(const OdCmEntityColor& color) { m_color = color; }

  /** \details
    Retrieves shadow parameters of this light source.
    
    \returns
    shadow parameters as OdGiShadowParameters instance.
    \sa
    <link gi_shadows_rendering, Shadows Visualization>
  */
  OdGiShadowParameters shadowParameters() const { return m_shadowParameters; }
  
  /** \details
    Specifies shadow parameters for this light source.
    
    \param params  [in]  Shadow parameters.
    \sa
    <link gi_shadows_rendering, Shadows Visualization>
  */
  void setShadowParameters(const OdGiShadowParameters& params) { m_shadowParameters = params; }

  /** \details
    Retrieves a light type.
    
    \returns
    light type as a value from the LightType enumeration.
  */
  LightType type() const { return m_type; }

  /** \details
    Saves the light traits to a specified filer.
    
    \param pFiler  [in]  Pointer to a filer.
  */
  void save(OdGsFiler * pFiler) const;
  
  /** \details
    Loads the light traits from a specified filer.
    
    \param pFiler  [in]  Pointer to a filer.
  */
  void load(OdGsFiler * pFiler);

  /** \details
    Deletes the specified light traits.
    
    \param pLightTraits  [in]  Pointer to light traits to delete.
  */
  static void deleteLightTraitsData(OdGiLightTraitsData* pLightTraits);
};

inline OdGiLightTraitsData::LightType OdGiLightTraitsData::drawableLightType(const OdGiDrawable *pDrawable)
{
  const LightType lightType = (pDrawable) ? (LightType)pDrawable->drawableType() : kInvalidLight;
  switch (lightType)
  {
    case kPointLight:
    case kSpotLight:
    case kDistantLight:
    case kWebLight:
      return lightType;
    default:
      return kInvalidLight;
  }
}

inline void OdGiLightTraitsData::save(OdGsFiler *pFiler) const {
  pFiler->wrBool(isOn());
  pFiler->wrDouble(intensity());
  pFiler->wrUInt32(color().color());
  m_shadowParameters.save(pFiler);
}

inline void OdGiLightTraitsData::load(OdGsFiler *pFiler) {
  setOn(pFiler->rdBool());
  setIntensity(pFiler->rdDouble());
  { OdCmEntityColor lightColor; lightColor.setColor(pFiler->rdUInt32()); setColor(lightColor); }
  m_shadowParameters.load(pFiler);
}

/** \details
    Corresponding C++ library: TD_Gi
    <group OdGi_Classes>
    \sa
    <link tv_working_with_lights, Work with Lights>
*/
class OdGiPointLightTraitsData : public OdGiLightTraitsData
{
  OdGePoint3d          m_position;
  OdGiLightAttenuation m_attenuation;
  // Photometric data
  double               m_dPhysIntensity;
  OdGiColorRGB         m_lampColor;
  bool                 m_bHasTarget;
  OdGePoint3d          m_target;
  bool                 m_bHemisphere;

public:
  
  /** \details
    Default constructor for the OdGiPointLightTraitsData class. Creates an object with physical intensity 1500, white lamp color, without target and hemispherical distribution.
    Also sets type of the light to kPointLight.
  */
  OdGiPointLightTraitsData()
    : m_dPhysIntensity(1500.0)
    , m_lampColor(1.0, 1.0, 1.0)
    , m_bHasTarget(false)
    , m_bHemisphere(false)
  {
    m_type = kPointLight;
  }

  /** \details
    Retrieves position of the point light source.
    
    \returns
    point light position as OdGePoint3d instance.
  */
  OdGePoint3d position() const { return m_position; }
  
  /** \details
    Specifies position of the point light source.
    
    \param pos  [in]  Point light source position.
  */
  void setPosition(const OdGePoint3d& pos) { m_position = pos; }

  
  
  /** \details
    Retrieves light attenuation parameters for this light source.
    
    \returns
    light attenuation parameters as OdGiLightAttenuation instance.
  */
  OdGiLightAttenuation attenuation() const { return m_attenuation; }
  
  /** \details
    Specifies attenuation light parameters for the point light source.
    
    \param atten  [in]  Point light source position.
  */
  void setAttenuation(const OdGiLightAttenuation& atten) { m_attenuation = atten; }

  
  
  /** \details
    Retrieves physical intensity for this light source in candela.
    
    \returns
    physical intensity of the light as a value of the double type.
  */
  double physicalIntensity() const { return m_dPhysIntensity; }
  
  /** \details
    Specifies physical intensity for this light source in candela.
    
    \param fIntensity  [in]  Physical intensity of the light in candela.
  */
  void setPhysicalIntensity(double intensity) { m_dPhysIntensity = intensity; }

  
  
  /** \details
    Retrieves color of the light lamp.
    
    \returns
    color of the light lamp as OdGiColorRGB instance.
  */
  OdGiColorRGB lampColor() const { return m_lampColor; }
  
  /** \details
    Specifies color for the light lamp.
    
    \param color  [in]  RGB color value.
  */
  void setLampColor(const OdGiColorRGB& color) { m_lampColor = color; }

  
  
  /** \details
    Checks whether this point has an associated target grip displayed.
    
    \returns
    true if light displays a target grip, false otherwise.
  */
  bool hasTarget() const { return m_bHasTarget; }
  
  /** \details
    Specifies whether this point should have an associated target grip displayed.
    
    \param bTarget  [in]  Flag that specifies whether to display a target grip for orienting the light.
  */
  void setHasTarget(bool bTarget) { m_bHasTarget = bTarget; }

  
  
  /** \details
    Retrieves target location of the light.
    
    \returns
    target location of the light as OdGePoint3d instance.
  */
  OdGePoint3d targetLocation() const { return m_target; }
  
  /** \details
    Specifies target location of the light.
    
    \param loc  [in]  Location of the target. Must differ from the point light position.
  */
  void setTargetLocation(const OdGePoint3d &loc) { m_target = loc; }

  void setHemisphericalDistribution(bool bHemisphere) { m_bHemisphere = bHemisphere; }
  bool hemisphericalDistribution() const { return m_bHemisphere; }

  
  
  /** \details
    Saves the light traits to a specified filer.
    
    \param pFiler  [in]  Pointer to a filer.
  */
  inline void save(OdGsFiler *pFiler) const {
    OdGiLightTraitsData::save(pFiler);
    pFiler->wrPoint3d(position());
    attenuation().save(pFiler);
    pFiler->wrDouble(physicalIntensity());
    pFiler->wrDouble(m_lampColor.red);
    pFiler->wrDouble(m_lampColor.green);
    pFiler->wrDouble(m_lampColor.blue);
    pFiler->wrBool(hasTarget());
    pFiler->wrPoint3d(targetLocation());
    pFiler->wrBool(hemisphericalDistribution());
  }

  /** \details
    Loads the light traits from a specified filer.
    
    \param pFiler  [in]  Pointer to a filer.
  */
  inline void load(OdGsFiler *pFiler) {
    OdGiLightTraitsData::load(pFiler);
    pFiler->rdPoint3d(m_position);
    m_attenuation.load(pFiler);
    setPhysicalIntensity(pFiler->rdDouble());
    m_lampColor.red   = pFiler->rdDouble();
    m_lampColor.green = pFiler->rdDouble();
    m_lampColor.blue  = pFiler->rdDouble();
    setHasTarget(pFiler->rdBool());
    pFiler->rdPoint3d(m_target);
    setHemisphericalDistribution(pFiler->rdBool());
  }
};

/** \details
    Corresponding C++ library: TD_Gi
    <group OdGi_Classes>
    \sa
    <link tv_working_with_lights, Work with Lights>
*/
class OdGiSpotLightTraitsData : public OdGiLightTraitsData
{
  OdGePoint3d          m_position, m_target;
  OdGiLightAttenuation m_attenuation;
  double               m_dHotspot, m_dFalloff;
  // Photometric data
  double               m_dPhysIntensity;
  OdGiColorRGB         m_lampColor;

public:
  /** \details
    Default constructor for the OdGiSpotLightTraitsData class. Creates an object with physical intensity 1500, white lamp color, hotspot and falloff cone angles set to 0.
    Also sets type of the light to kSpotLight.
  */
  OdGiSpotLightTraitsData()
    : m_dHotspot(0.0)
    , m_dFalloff(0.0)
    , m_dPhysIntensity(1500.0)
    , m_lampColor(1.0, 1.0, 1.0)
  {
    m_type = kSpotLight;
  }

  /** \details
    Retrieves position of the spot light source.
    
    \returns
     position of the spot light source as OdGePoint3d instance.
  */
  OdGePoint3d position() const { return m_position; }
  
  /** \details
    Specifies position of the spot light source.
    
    \param pos  [in]  Spot light source position.
  */
  void setPosition(const OdGePoint3d& pos) { m_position = pos; }

  
  
  /** \details
    Retrieves target location of the light.
    
    \returns
    target location of the light as OdGePoint3d instance.
  */
  OdGePoint3d target() const { return m_target; }
  
  /** \details
    Specifies target location of the light.
    
    \param loc  [in]  Location of the target. Must differ from the spot light position.
  */
  void setTarget(const OdGePoint3d& trg) { m_target = trg; }

  
  
  /** \details
    Retrieves light attenuation parameters for this light source.
    
    \returns
    light attenuation parameters as OdGiLightAttenuation instance.
  */
  OdGiLightAttenuation attenuation() const { return m_attenuation; }
  
  /** \details
    Specifies attenuation light parameters for the point light source.
    
    \param atten  [in]  Point light source position.
  */
  void setAttenuation(const OdGiLightAttenuation& atten) { m_attenuation = atten; }

  
  
  /** \details
    Specifies target location of the light.
    
    \param hotspot  [in]  Hotspot cone angle, in radians. Defines the brightest part of the spot light beam.
    \param falloff  [in]  Falloff angle, in radians. Defines the full cone of light.
  */
  bool setHotspotAndFalloff(double hotSpot, double falloff);
  
  /** \details
    Retrieves the cone angle that produces the brightest part of the beam (hotspot) of this light.
    
    \returns
    cone of the hotspot as a value of the double type.
  */
  double hotspot() const { return m_dHotspot; }
  
  /** \details
    Retrieves the cone angle that produces the light falloff.
    
    \returns
    cone of the falloff as a value of the double type.
  */
  double falloff() const { return m_dFalloff; }

  
  
  /** \details
    Retrieves physical intensity for this light source in candela.
    
    \returns
    physical intensity of the light as a value of the double type.
  */
  double physicalIntensity() const { return m_dPhysIntensity; }
  
  /** \details
    Specifies physical intensity for this light source in candela.
    
    \param fIntensity  [in]  Physical intensity of the light in candela.
  */
  void setPhysicalIntensity(double intensity) { m_dPhysIntensity = intensity; }

  
  
  /** \details
    Retrieves color of the light lamp.
    
    \returns
    color of the light lamp as OdGiColorRGB instance.
  */
  OdGiColorRGB lampColor() const { return m_lampColor; }
  
  /** \details
    Specifies color for the light lamp.
    
    \param color  [in]  RGB color value.
  */
  void setLampColor(const OdGiColorRGB& color) { m_lampColor = color; }



  /** \details
    Saves the light traits to a specified filer.
    
    \param pFiler  [in]  Pointer to a filer.
  */
  void save(OdGsFiler *pFiler) const {
    OdGiLightTraitsData::save(pFiler);
    pFiler->wrPoint3d(position());
    pFiler->wrPoint3d(target());
    attenuation().save(pFiler);
    pFiler->wrDouble(physicalIntensity());
    pFiler->wrDouble(hotspot());
    pFiler->wrDouble(falloff());
    pFiler->wrDouble(m_lampColor.red);
    pFiler->wrDouble(m_lampColor.green);
    pFiler->wrDouble(m_lampColor.blue);
  }

  /** \details
    Loads the light traits from a specified filer.
    
    \param pFiler  [in]  Pointer to a filer.
  */
  void load(OdGsFiler *pFiler) {
    OdGiLightTraitsData::load(pFiler);
    pFiler->rdPoint3d(m_position);
    pFiler->rdPoint3d(m_target);
    m_attenuation.load(pFiler);
    setPhysicalIntensity(pFiler->rdDouble());
    m_dHotspot        = pFiler->rdDouble();
    m_dFalloff        = pFiler->rdDouble();
    m_lampColor.red   = pFiler->rdDouble();
    m_lampColor.green = pFiler->rdDouble();
    m_lampColor.blue  = pFiler->rdDouble();
  }
};

/** \details
    Corresponding C++ library: TD_Gi
    <group OdGi_Classes>
*/
inline bool OdGiSpotLightTraitsData::setHotspotAndFalloff(double hotspot, double falloff)
{ 
  if((hotspot > falloff) || 
     (hotspot < 0.0) || (falloff < 0.0) || 
     (OdaToDegree(hotspot) > 160.0) || (OdaToDegree(falloff) > 160.0))
    return false;
  m_dHotspot = hotspot;
  m_dFalloff = falloff; 
  return true;
}

/** \details
    Corresponding C++ library: TD_Gi
    <group OdGi_Classes>
    \sa
    <link tv_working_with_lights, Work with Lights>
*/
class OdGiDistantLightTraitsData : public OdGiLightTraitsData
{
  OdGeVector3d m_direction;
  bool         m_bIsSunlight;
  // Photometric data
  double       m_dPhysIntensity;
  OdGiColorRGB m_lampColor;
  //OdGiSkyParameters m_skyParams; // Unused

public:
  /** \details
    Default constructor for the OdGiDistantLightTraitsData class. Creates an object with physical intensity 1500, white lamp color, and flag that specifies whether this light is rendered as a sunlight.
    Also sets type of the light to kDistantLight.
  */
  OdGiDistantLightTraitsData()
    : m_bIsSunlight(false)
    , m_dPhysIntensity(1500.0)
    , m_lampColor(1.0, 1.0, 1.0)
  {
    m_type = kDistantLight;
  }

  
  
  /** \details
    Retrieves direction of the distant light.
    
    \returns
    direction of the distant light as OdGeVector3d instance.
  */
  OdGeVector3d direction() const { return m_direction; }
  
  /** \details
    Specifies direction of the distant light.
    
    \param dir  [in]  Light direction.
  */
  void setDirection(const OdGeVector3d& dir) { m_direction = dir; }

  
  
  /** \details
    Checks whether this light is rendered as sunlight.
    
    \returns
    true if this light is rendered as sunlight, false otherwise.
  */
  bool isSunlight() const { return m_bIsSunlight; }
  
  /** \details
    Specifies whether this light is rendered as sunlight.
    
    \param isSunlight  [in]  Flag that specifies whether to render this light as sunlight.
  */
  void setSunlight(bool val) { m_bIsSunlight = val; }

  
  
  /** \details
    Retrieves physical intensity for this light source in candela.
    
    \returns
    physical intensity of the light as a value of the double type.
  */
  double physicalIntensity() const { return m_dPhysIntensity; }
  
  /** \details
    Specifies physical intensity for this light source in candela.
    
    \param fIntensity  [in]  Physical intensity of the light in candela.
  */
  void setPhysicalIntensity(double intensity) { m_dPhysIntensity = intensity; }

  
  
  /** \details
    Retrieves color of the light lamp.
    
    \returns
    color of the light lamp as OdGiColorRGB instance.
  */
  OdGiColorRGB lampColor() const { return m_lampColor; }
  
  /** \details
    Specifies color for the light lamp.
    
    \param color  [in]  RGB color value.
  */
  void setLampColor(const OdGiColorRGB& color) { m_lampColor = color; }

  
  
  /** \details
    Retrieves sky parameters of this light. Currently not implemented.
  */
  void skyParameters(OdGiSkyParameters & /*params*/) const { /*params = m_skyParams;*/ }
  
  /** \details
    Specifies sky parameters for this light. Currently not implemented.
  */
  void setSkyParameters(const OdGiSkyParameters & /*params*/) { /*m_skyParams = params;*/ }

  
  
  /** \details
    Saves the light traits to a specified filer.
    
    \param pFiler  [in]  Pointer to a filer.
  */
  void save(OdGsFiler *pFiler) const {
    OdGiLightTraitsData::save(pFiler);
    pFiler->wrVector3d(direction());
    pFiler->wrBool(isSunlight());
    pFiler->wrDouble(physicalIntensity());
    pFiler->wrDouble(m_lampColor.red);
    pFiler->wrDouble(m_lampColor.green);
    pFiler->wrDouble(m_lampColor.blue);
  }

  /** \details
    Loads the light traits from a specified filer.
    
    \param pFiler  [in]  Pointer to a filer.
  */
  void load(OdGsFiler *pFiler) {
    OdGiLightTraitsData::load(pFiler);
    pFiler->rdVector3d(m_direction);
    setSunlight(pFiler->rdBool());
    setPhysicalIntensity(pFiler->rdDouble());
    m_lampColor.red   = pFiler->rdDouble();
    m_lampColor.green = pFiler->rdDouble();
    m_lampColor.blue  = pFiler->rdDouble();
  }
};

/** \details
    Corresponding C++ library: TD_Gi
    <group OdGi_Classes>
*/
class OdGiWebLightTraitsData : public OdGiPointLightTraitsData
{
  OdString                        m_webFile;
  OdGeVector3d                    m_webRotate;
  double                          m_dFlux;
  OdGiWebLightTraits::WebFileType m_webFileType;
  OdGiWebLightTraits::WebSymmetry m_webSymmetry;
  bool                            m_bHorzAng90to270;

public:
  OdGiWebLightTraitsData()
    : OdGiPointLightTraitsData()
    , m_dFlux(0.0)
    , m_webFileType(OdGiWebLightTraits::kTypeA)
    , m_webSymmetry(OdGiWebLightTraits::kNoSymmetry)
    , m_bHorzAng90to270(false)
  {
    m_type = kWebLight;
  }

  const OdString &webFile() const { return m_webFile; }
  void setWebFile(const OdString &fileName) { m_webFile = fileName; }

  const OdGeVector3d& webRotation() const { return m_webRotate; }
  void setWebRotation(const OdGeVector3d& rot) { m_webRotate = rot; }

  double webFlux() const { return m_dFlux; }
  void setWebFlux(double flux) { m_dFlux = flux; }

  OdGiWebLightTraits::WebFileType webFileType() const { return m_webFileType; }
  void setWebFileType(OdGiWebLightTraits::WebFileType type) { m_webFileType = type; }

  OdGiWebLightTraits::WebSymmetry webSymmetry() const { return m_webSymmetry; }
  void setWebSymmetry(OdGiWebLightTraits::WebSymmetry sym) { m_webSymmetry = sym; }

  bool webHorzAng90to270() const { return m_bHorzAng90to270; }
  void setWebHorzAng90to270(bool bHA) { m_bHorzAng90to270 = bHA; }

  inline void save(OdGsFiler *pFiler) const {
    OdGiPointLightTraitsData::save(pFiler);
    pFiler->wrString(webFile());
    pFiler->wrVector3d(webRotation());
    pFiler->wrDouble(webFlux());
    pFiler->wrUInt8((OdUInt8)webFileType());
    pFiler->wrUInt8((OdUInt8)webSymmetry());
    pFiler->wrBool(webHorzAng90to270());
  }

  inline void load(OdGsFiler *pFiler) {
    OdGiPointLightTraitsData::load(pFiler);
    pFiler->rdString(m_webFile);
    pFiler->rdVector3d(m_webRotate);
    setWebFlux(pFiler->rdDouble());
    setWebFileType((OdGiWebLightTraits::WebFileType)pFiler->rdUInt8());
    setWebSymmetry((OdGiWebLightTraits::WebSymmetry)pFiler->rdUInt8());
    setWebHorzAng90to270(pFiler->rdBool());
  }
};

inline void OdGiLightTraitsData::deleteLightTraitsData(OdGiLightTraitsData* pLightTraits)
{
  switch(pLightTraits->type())
  {
  case OdGiLightTraitsData::kDistantLight:
    delete static_cast<OdGiDistantLightTraitsData*>(pLightTraits);
    break;
  case OdGiLightTraitsData::kPointLight:
    delete static_cast<OdGiPointLightTraitsData*>(pLightTraits);
    break;
  case OdGiLightTraitsData::kSpotLight:
    delete static_cast<OdGiSpotLightTraitsData*>(pLightTraits);
    break;
  case OdGiLightTraitsData::kWebLight:
    delete static_cast<OdGiWebLightTraitsData*>(pLightTraits);
    break;
  default:
    {
      ODA_FAIL_ONCE();
    }
    break;
  }
}

/** \details
    Corresponding C++ library: TD_Gi
    <group OdGi_Classes>
*/
class OdGiLightTraitsVpDepData : public OdGiLightTraitsData
{
  OdUInt32    m_nViewportId;
  OdDbStub*   m_pViewportObjectId;
  bool        m_bVpDepOn;
  double      m_dVpDepDimming;

public:
  OdGiLightTraitsVpDepData() : OdGiLightTraitsData(),
    m_nViewportId( 0 ),
    m_pViewportObjectId( NULL ),
    m_bVpDepOn( false ),
    m_dVpDepDimming( 0.0 )
  {
  }

  OdUInt32 viewportId() const { return m_nViewportId; }
  void setViewportId( OdUInt32 id ) { m_nViewportId = id; }

  OdDbStub* viewportObjectId() const { return m_pViewportObjectId; }
  void setViewportObjectId( OdDbStub* id ) { m_pViewportObjectId = id; }

  bool vpDepOn() const { return m_bVpDepOn; }
  void setVpDepOn( bool on ) { m_bVpDepOn = on; }

  double vpDepDimming() const { return m_dVpDepDimming; }
  void setVpDepDimming( double dimming ) { m_dVpDepDimming = dimming; }

  inline void save(OdGsFiler *pFiler) const {
    OdGiLightTraitsData::save(pFiler);
    pFiler->wrBool( vpDepOn() );
    pFiler->wrDouble( vpDepDimming() );
    pFiler->wrUInt32( viewportId() );
    pFiler->wrHandle( viewportObjectId() );
  }

  inline void load(OdGsFiler *pFiler) {
    OdGiLightTraitsData::load(pFiler);
    setVpDepOn( pFiler->rdBool() );
    setVpDepDimming( pFiler->rdDouble() );
    setViewportId( pFiler->rdUInt32() );
    setViewportObjectId( pFiler->rdHandle() );
  }
};

template <class LightTraitsDataType>
inline void transformLightTraitsDataPosition(LightTraitsDataType &traitsData, const OdGeMatrix3d &modelTransform)
{
  OdGePoint3d newPos(traitsData.position());
  newPos.transformBy(modelTransform);
  traitsData.setPosition(newPos);
}
// For Point and Web lights
template <class LightTraitsDataType>
inline void transformLightTraitsData(LightTraitsDataType &traitsData, const OdGeMatrix3d &modelTransform)
{
  transformLightTraitsDataPosition(traitsData, modelTransform);
  if (traitsData.hasTarget())
  {
    OdGePoint3d newTarget = traitsData.targetLocation();
    newTarget.transformBy(modelTransform);
    traitsData.setTargetLocation(newTarget);
  }
}
// Specialization for Spot lights
template <>
inline void transformLightTraitsData<OdGiSpotLightTraitsData>(OdGiSpotLightTraitsData &traitsData, const OdGeMatrix3d &modelTransform)
{
  transformLightTraitsDataPosition(traitsData, modelTransform);
  OdGePoint3d newTarget = traitsData.target();
  newTarget.transformBy(modelTransform);
  traitsData.setTarget(newTarget);
}
// Specialization for Direction lights
template <>
inline void transformLightTraitsData<OdGiDistantLightTraitsData>(OdGiDistantLightTraitsData &traitsData, const OdGeMatrix3d &modelTransform)
{
  OdGeVector3d newDirection = traitsData.direction();
  newDirection.transformBy(modelTransform);
  traitsData.setDirection(newDirection);
}

#include "TD_PackPop.h"

#endif // __OD_GI_LIGHT_TRAITS_DATA__
