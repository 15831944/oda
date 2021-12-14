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

#ifndef __OD_GI_VIEWPORT_TRAITS_DATA__
#define __OD_GI_VIEWPORT_TRAITS_DATA__

#include "TD_PackPush.h"

#include "Ge/GeVector3d.h"
#include "Gi/GiViewportTraits.h"
#include "Gi/GiLightTraits.h"

/** \details
    Corresponding C++ library: TD_Gi
    <group OdGi_Classes>
    \sa
    <link tv_working_with_lights, Work with Lights>
*/
class OdGiViewportTraitsData
{
  OdDbStub*                               m_backgroundId;
  bool                                    m_bDefLightOn;
  OdGiViewportTraits::DefaultLightingType m_lightType;
  OdGeVector3d                            m_userDefLightDir;
  double                                  m_defLightIntensity;
  OdCmEntityColor                         m_defLightColor;
  OdGiShadowParameters                    m_defShadowParams;
  OdCmEntityColor                         m_colorAmbient;
  double                                  m_dBrightness;
  double                                  m_dContrast;
  OdDbStub*                               m_renderEnvId;
  OdDbStub*                               m_renderSettings;
  OdDbStub*                               m_visualStyle;
  OdGiToneOperatorParametersPtr           m_pToneOperators;

public:

  /** \details
    Default constructor for the OdGiToneOperatorParameters class. Initializes the following data members:
    <table>
    Name                      Value                                   Description
    m_backgroundId            NULL                                    Viewport background identifier.
    m_bDefLightOn             false                                   Default lighting usage (on/off).
    m_lightType               OdGiViewportTraits::kOneDistantLight    Default Lighting type.
    m_userDefLightDir         OdGeVector3d::kZAxis                    User-defined light direction.
    m_defLightIntensity       0.5                                     Default lighting intensity.
    m_defLightColor           (255, 255, 255)                         Default lighting color. 
    m_defShadowParams         setShadowsOn(false)                     Shadow parameters for default lighting.
    m_colorAmbient            -                                        Ambient color.
    m_dBrightness             50.0                                    Overall viewport brightness.
    m_dContrast               50.0                                    Overall viewport contrast.
    m_renderEnvId             NULL                                    Render environment identifier.
    m_renderSettings          NULL                                    Render settings identifier.
    m_visualStyle             NULL                                    Visual style identifier.
    m_pToneOperators          default settings*                       Tone operator parameters. Default settings are set internally with the createObject method.
    </table>
  */
  OdGiViewportTraitsData()
    : m_backgroundId(NULL)
    , m_bDefLightOn(false)
    , m_lightType(OdGiViewportTraits::kOneDistantLight)
    , m_userDefLightDir(OdGeVector3d::kZAxis)
    , m_defLightIntensity(0.5)
    , m_defLightColor(255, 255, 255)
    , m_defShadowParams()
    , m_colorAmbient()
    , m_dBrightness(50.0)
    , m_dContrast(50.0)
    , m_renderEnvId(NULL)
    , m_renderSettings(NULL)
    , m_visualStyle(NULL)
  {
    m_defShadowParams.setShadowsOn(false);
    m_pToneOperators = OdGiToneOperatorParameters::createObject();
  }

  /** \details
    Retrieves drawable type which is OdGiDrawable::kViewport for this object.
  */
  OdGiDrawable::DrawableType type() const { return OdGiDrawable::kViewport; }

  /** \details
    Sets the background for this viewport.
    \param backgroundId [in]   Pointer to the background.
    \sa
    <link tv_working_with_attributes_view_backgrounds, Work with View Backgrounds> 
  */
  void setBackground(OdDbStub* backgroundId)
  {
    m_backgroundId = backgroundId;
  }
  /** \details
    Retrieves a pointer to the background for this viewport.
    \sa
    <link tv_working_with_attributes_view_backgrounds, Work with View Backgrounds> 
  */
  OdDbStub* background() const
  {
    return m_backgroundId;
  }
  
  
  /** \details
    Enables or disables the default lighting.
    \param on [in]   Flag that specifies whether to enable the default lighting.
  */
  void setDefaultLightingOn(bool on)
  {
    m_bDefLightOn = on;
  }
  /** \details
    Checks whether default lighting is turned on.
    \returns
    true if default lighting is on, false otherwise.
  */
  bool isDefaultLightingOn() const
  {
    return m_bDefLightOn;
  }

  
  /** \details
    Sets the default lighting type.
    \param typ [in]   One of the default lighting types.
  */
  void setDefaultLightingType(OdGiViewportTraits::DefaultLightingType typ)
  {
    m_lightType = typ;
  }
  /** \details
    Retrieves the default lighting type.
  */
  OdGiViewportTraits::DefaultLightingType defaultLightingType() const
  {
    return m_lightType;
  }

  
  /** \details
    Retrieves the user-defined light direction for distant lights.
  */
  OdGeVector3d userDefinedLightDirection() const
  {
    return m_userDefLightDir;
  }
  /** \details
    Sets the user-defined light direction for distant lights.
    \param lightDirection [in]   Vector that specifies the light direction.
  */
  void setUserDefinedLightDirection(const OdGeVector3d& lightDirection)
  {
    m_userDefLightDir = lightDirection;
  }

  
  /** \details
    Retrieves the intensity of default lighting.
  */
  double defaultLightingIntensity() const
  {
    return m_defLightIntensity;
  }
  /** \details
    Sets the intensity of default lighting.
    \param dIntensity [in]   Default lighting intensity.
  */
  void setDefaultLightingIntensity(double dIntensity)
  {
    m_defLightIntensity = dIntensity;
  }

  
  /** \details
    Retrieves the default lighting color.
  */
  const OdCmEntityColor &defaultLightingColor() const
  {
    return m_defLightColor;
  }
  /** \details
    Sets the color of default lighting.
    \param color [in]   Default lighting color.
  */
  void setDefaultLightingColor(const OdCmEntityColor& color)
  {
    m_defLightColor = color;
  }

  
  /** \details
    Retrieves the default lighting shadow parameters.
    \sa
    <link gi_shadows_rendering, Shadows Visualization>
  */
  const OdGiShadowParameters &defaultLightingShadowParameters() const
  {
    return m_defShadowParams;
  }
  /** \details
    Sets the default lighting shadow parameters.
    \param params [in]   Shadow parameters.
    \sa
    <link gi_shadows_rendering, Shadows Visualization>
  */
  void setDefaultLightingShadowParameters(const OdGiShadowParameters &params)
  {
    m_defShadowParams = params;
  }

  
  /** \details
    Sets the ambient color (color in the shadows) for this viewport.
    \param clr [in]   Ambient color.
  */
  void setAmbientLightColor(const OdCmEntityColor& clr)
  {
    m_colorAmbient = clr;
  }
  /** \details
    Retrieves the ambient color (color in the shadows).
  */
  const OdCmEntityColor &ambientLightColor() const
  {
    return m_colorAmbient;
  }

  
  /** \details
    Sets the brightness for this viewport.
    \param brightness [in]   Brightness value.
  */
  void setBrightness(double brightness)
  {
    m_dBrightness = brightness;
  }
  /** \details
    Retrieves the brightness for this viewport.
  */
  double brightness() const
  {
    return m_dBrightness;
  }

  
  /** \details
    Sets the overall contrast for this viewport.
    \param contrast [in]   Light contrast value.
  */
  void setContrast(double contrast)
  {
    m_dContrast = contrast;
  }
  /** \details
    Retrieves the overall contrast for this viewport.
  */
  double contrast() const
  {
    return m_dContrast;
  }

  
  /** \details
    Sets the render environment for this viewport. Used only in full renders.
    \param renderEnvId [in]   Pointer to the render environment.
  */
  void setRenderEnvironment(OdDbStub* renderEnvId)
  {
    m_renderEnvId = renderEnvId;
  }
  /** \details
    Retrieves a pointer to the render environment for this viewport. Used only in full renders.
  */
  OdDbStub* renderEnvironment() const
  {
    return m_renderEnvId;
  }

  
  /** \details
    Sets the render settings for this viewport. Used only in full renders.
    \param renderSettingsId [in]   Pointer to the render settings.
  */
  void setRenderSettings(OdDbStub* renderSettingsId)
  {
    m_renderSettings = renderSettingsId;
  }
  /** \details
    Retrieves a pointer to the render settings for this viewport. Used only in full renders.
  */
  OdDbStub* renderSettings() const
  {
    return m_renderSettings;
  }

  
  /** \details
    Sets the visual style for this viewport. Used only in full renders.
    \param visualStyleId [in]   Pointer to the render settings.
    \sa
    <link tv_working_with_attributes_visual_styles, Work with Visual Styles>
  */
  void setVisualStyle(OdDbStub* visualStyleId)
  {
    m_visualStyle = visualStyleId;
  }
  
  /** \details
    Retrieves a pointer to the visual style for this viewport. Used only in full renders.
    \sa
    <link tv_working_with_attributes_visual_styles, Work with Visual Styles>
  */
  OdDbStub* visualStyle() const
  {
    return m_visualStyle;
  }

  
  /** \details
    Sets the tone parameters for this viewport.
    \param params [in]   Tone operator parameters.
  */
  void setToneOperatorParameters(const OdGiToneOperatorParameters &params)
  {
    if (params.isKindOf(OdGiPhotographicExposureParameters::desc()))
    {
      if (!m_pToneOperators->isKindOf(OdGiPhotographicExposureParameters::desc()))
        m_pToneOperators = OdGiPhotographicExposureParameters::createObject();
      *static_cast<OdGiPhotographicExposureParameters*>(m_pToneOperators.get()) = static_cast<const OdGiPhotographicExposureParameters&>(params);
    }
    else
    {
      if (m_pToneOperators->isKindOf(OdGiPhotographicExposureParameters::desc()))
        m_pToneOperators = OdGiToneOperatorParameters::createObject();
      *m_pToneOperators = params;
    }
  }
  /** \details
    Retrieves tone parameters for this viewport.
    \param params [out]   Receives tone operator parameters.
  */
  void toneOperatorParameters(OdGiToneOperatorParameters &params) const
  {
    if (m_pToneOperators->isKindOf(OdGiPhotographicExposureParameters::desc()) &&
        params.isKindOf(OdGiPhotographicExposureParameters::desc()))
      static_cast<OdGiPhotographicExposureParameters&>(params) = *static_cast<const OdGiPhotographicExposureParameters*>(m_pToneOperators.get());
    else
      params = *m_pToneOperators;
  }
  /** \details
    Retrieves tone parameters for this viewport.
  */
  const OdGiToneOperatorParameters &toneOperatorParameters() const
  {
    return *m_pToneOperators;
  }

  /** \details
    Compares tone parameters (static and non-static) for the viewport.
    \param op1 [in]   First tone operator to compare.
    \param op2 [in]   Second tone operator to compare.
    \returns
    true if tone operators are equal, false otherwise.
  */
  static bool compareToneOps(const OdGiToneOperatorParametersPtr &op1, const OdGiToneOperatorParametersPtr &op2)
  {
    const bool bPhoto1 = op1->isKindOf(OdGiPhotographicExposureParameters::desc());
    const bool bPhoto2 = op2->isKindOf(OdGiPhotographicExposureParameters::desc());
    if (bPhoto1 && bPhoto2)
      return *static_cast<const OdGiPhotographicExposureParameters*>(op1.get()) == *static_cast<const OdGiPhotographicExposureParameters*>(op2.get());
    else if (!bPhoto1 && !bPhoto2)
      return *op1 == *op2;
    return false;
  }

  /** \details
    Equality operator for the OdGiViewportTraitsData class.
    Traits are considered equal if:
    * Background identifiers are equal;
    * Default lighting state flags are equal;
    * User-defined light directions are equal;
    * Default lighting intensity values are equal;
    * Default lighting colors are equal;
    * Ambient lighting colors are equal;
    * Brightness values are equal.
    * Contrast values are equal.
    * Render environment identifiers are equal.
    * Render settings identifiers are equal.
    * Render environment identifiers are equal.
    * Visual style identifiers are equal.
    * Tone operators are equal.
    
    \param data2 [in]  Viewport traits to compare.
    \returns
    true if viewport traits are equal, false otherwise.
  */
  bool operator ==(const OdGiViewportTraitsData &data2) const
  {
    return ((m_backgroundId == data2.m_backgroundId) &&
            (m_bDefLightOn == data2.m_bDefLightOn) &&
            (m_lightType == data2.m_lightType) &&
            m_userDefLightDir.isEqualTo(data2.m_userDefLightDir) &&
            OdEqual(m_defLightIntensity, data2.m_defLightIntensity) &&
            (m_defLightColor == data2.m_defLightColor) &&
            (m_colorAmbient == data2.m_colorAmbient) &&
            OdEqual(m_dBrightness, data2.m_dBrightness) &&
            OdEqual(m_dContrast, data2.m_dContrast) &&
            (m_renderEnvId == data2.m_renderEnvId) &&
            (m_renderSettings == data2.m_renderSettings) &&
            (m_visualStyle == data2.m_visualStyle) &&
            compareToneOps(m_pToneOperators, data2.m_pToneOperators));
  }
  /** \details
    Inequality operator for the OdGiViewportTraitsData class.
    Traits are considered not equal if at least one of the following condition fails:
    * Background identifiers are equal;
    * Default lighting state flags are equal;
    * User-defined light directions are equal;
    * Default lighting intensity values are equal;
    * Default lighting colors are equal;
    * Ambient lighting colors are equal;
    * Brightness values are equal.
    * Contrast values are equal.
    * Render environment identifiers are equal.
    * Render settings identifiers are equal.
    * Render environment identifiers are equal.
    * Visual style identifiers are equal.
    * Tone operators are equal.
    
    \param data2 [in]  Viewport traits to compare.
    \returns
    true if viewport traits are not equal, false otherwise.
  */
  bool operator !=(const OdGiViewportTraitsData &data2) const
  {
    return !(*this == data2);
  }
};

#include "TD_PackPop.h"

#endif // __OD_GI_VIEWPORT_TRAITS_DATA__
