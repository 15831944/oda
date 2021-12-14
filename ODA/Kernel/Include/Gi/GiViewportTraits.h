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

#ifndef __OD_GI_VIEWPORT_TRAITS__
#define __OD_GI_VIEWPORT_TRAITS__

#include "TD_PackPush.h"

#include "Gi.h"
#include "CmColorBase.h"
#include "StaticRxObject.h"

class OdGiShadowParameters;

/** \details
    Class that contains all tone parameters. 
    Corresponding C++ library: TD_Gi
    <group OdGi_Classes>
*/
class FIRSTDLL_EXPORT OdGiToneOperatorParameters : public OdRxObject
{
public:
  /** \details
    Represents exterior daylight modes.
  */
  enum ExteriorDaylightMode
  {
    /** Daylight is off.*/
    kDaylightOff  = 0,
    /** Daylight is on.*/
    kDaylightOn   = 1,
    /** Daylight depends on the sun status.*/
    kDaylightAuto = 2
  };
public:
  ODRX_DECLARE_MEMBERS(OdGiToneOperatorParameters);

  /** \details
    Default constructor for the OdGiToneOperatorParameters class. Initializes the following data members:
    <table>
    Name                      Value               Description
    m_bIsActive               true                The tone operator state. When this flag is set to false, some methods may not apply the passed parameters for this tone operator (for example, contrast, daylight exterior mode, etc.).
    m_bChromaticAdaptation    false               Chromatic adaptation.
    m_bColorDifferentiation   false               Color differentiation.
    m_clrWhiteColor           0xC2FFFFFF          Reference white color.
    m_bProcessBackground      false               Flag that indicates whether background should be processed by exposure control at render time.
    m_fBrightness             65.0                Brightness in range 0..200. 
    m_fContrast               50.0                Contrast in range 0..100.
    m_fMidTones               1.0                 Midtones in range 0.01..20.0.
    m_eExteriorDaylight       kDaylightAuto       Daylight exterior mode (on, off, or depends on the sun state).
    </table>
  */
  OdGiToneOperatorParameters();
  
  /** \details
    Copy constructor for the OdGiToneOperatorParameters class. Copies the data members of the passed object.
    \param params [in]  Tone parameters to copy.
  */
  OdGiToneOperatorParameters(const OdGiToneOperatorParameters &params);
  
  /** \details
    Destructor for the OdGiToneOperatorParameters class.
  */
  virtual ~OdGiToneOperatorParameters() { }

  /** \details
    Equality operator for the OdGiToneOperatorParameters class.
    The parameters are considered equal if:
    * The active state flags are equal;
    * Chromatic adaptation flags are equal;
    * Color differentiation flags are equal;
    * Reference white colors are equal;
    * The process background flags are equal;
    * Brightness values are equal.
    * Midtone values are equal.
    * Contrast values are equal.
    * Daylight exterior modes are equal.
    \param params [in]  Tone operator parameters to compare.
    \returns
    true if parameters are equal, false otherwise.
  */
  bool operator ==(const OdGiToneOperatorParameters &params) const;
  
  /** \details
    Inequality operator for the OdGiToneOperatorParameters class.
    The parameters are considered not equal if one of the following condition fails:
    * The active state flags are equal;
    * Chromatic adaptation flags are equal;
    * Color differentiation flags are equal;
    * Reference white colors are equal;
    * The process background flags are equal;
    * Brightness values are equal.
    * Midtone values are equal.
    * Contrast values are equal.
    * Daylight exterior modes are equal.
    \param params [in]  Tone operator parameters to compare.
    \returns
    true if parameters are not equal, false otherwise.
  */
  bool operator !=(const OdGiToneOperatorParameters &params) const;
  
  /** \details
    Assignment operator for the OdGiToneOperatorParameters class. Returns a reference to this object with new assigned parameters.
    \param params [in]  Parameters to assign.
  */
  OdGiToneOperatorParameters &operator =(const OdGiToneOperatorParameters &params);

  /** \details
    Specifies whether this tone is active.
    \param bActive [in]  Flag that specifies whether tone is active.
  */
  virtual void setIsActive(bool bActive);
  
  /** \details
    Checks whether this tone is active.
    \returns
    true if this tone is active, false otherwise.
  */
  virtual bool isActive() const;

  
  /** \details
    Specifies whether to enable chromatic adaptation for this tone.
    \param bEnable [in]  Flag that specifies whether to enable chromatic adaptation.
  */
  virtual void setChromaticAdaptation(bool bEnable);
  
  /** \details
    Checks whether chromatic adaptation is enabled for this tone.
    \returns
    true if chromatic adaptation is enabled, false otherwise.
  */
  virtual bool chromaticAdaptation() const;

  
  /** \details
    Specifies whether to enable color differentiation for this tone.
    \param bEnable [in]  Flag that specifies whether to enable color differentiation.
  */
  virtual void setColorDifferentiation(bool bEnable);
  
  /** \details
    Checks whether color differentiation is enabled for this tone.
    \returns
    true if color differentiation is enabled, false otherwise.
  */
  virtual bool colorDifferentiation() const;

  /** \details
    Sets the reference white color.
    \param color [in]  Reference white color.
  */
  virtual void setWhiteColor(const OdCmEntityColor &color);
  
  /** \details
    Retrieves the reference white color.
  */
  virtual const OdCmEntityColor &whiteColor() const;

  
  /** \details
    Specifies whether to enable background processing by exposure control at render time.
    \param bProcessBg [in]  Flag that specifies whether to enable background processing.
  */
  virtual void setProcessBackground(bool bProcessBg);
  
  /** \details
    Checks whether background processing by exposure control at render time is enabled.
    \returns
    true if color background processing by exposure control at render time is enabled, false otherwise.
  */
  virtual bool processBackground() const;

  
  /** \details
    Sets the brightness for this tone. Value is applied only if the tone is active.
    \param fBrightness [in]  Brightness in range 0..200.
    \returns
    true if a value is successfully applied, false otherwise.
  */
  virtual bool setBrightness(double fBrightness);
  
  /** \details
    Retrieves the brightness for this tone.
  */
  virtual double brightness() const;

  
  /** \details
    Sets the contrast for this tone. Value is applied only if the tone is active.
    \param fContrast [in]  Contrast in range 0..200.
    \returns
    true if a value is successfully applied, false otherwise.
  */
  virtual bool setContrast(double fContrast);
  
  /** \details
    Retrieves the contrast for this tone.
  */
  virtual double contrast() const;

  
  /** \details
    Sets the midtones for this tone. Value is applied only if the tone is active.
    \param fMidTones [in]  Midtones in range 0.01..20.0.
    \returns
    true if a value is successfully applied, false otherwise.
  */
  virtual bool setMidTones(double fMidTones);
  
  /** \details
    Retrieves the midtones for this tone.
  */
  virtual double midTones() const;

  
  /** \details
    Sets the exterior daylight mode for this tone. Value is applied only if the tone is active.
    \param mode [in]  Daylight exterior mode.
    \returns
    true if the mode is successfully applied, false otherwise.
  */
  virtual bool setExteriorDaylight(ExteriorDaylightMode mode);
  
  /** \details
    Retrieves the daylight exterior mode for this tone.
  */
  virtual ExteriorDaylightMode exteriorDaylight() const;
private:
  bool m_bIsActive;
  bool m_bChromaticAdaptation;
  bool m_bColorDifferentiation;
  OdCmEntityColor m_clrWhiteColor;
  bool m_bProcessBackground;
  double m_fBrightness;
  double m_fContrast;
  double m_fMidTones;
  ExteriorDaylightMode m_eExteriorDaylight;
};

/** \details
    Corresponding C++ library: TD_Gi
    <group OdGi_Classes>
*/
class OdGiToneOperatorParametersStatic : public OdStaticRxObject<OdGiToneOperatorParameters>
{
  public:
    /** \details
      Default constructor for the OdGiToneOperatorParametersStatic class.
    */
    OdGiToneOperatorParametersStatic() : OdStaticRxObject<OdGiToneOperatorParameters>() { }
    /** \details
      Equality operator for the OdGiToneOperatorParametersStatic class.
      The parameters are considered equal if:
      * The active state flags are equal;
      * Chromatic adaptation flags are equal;
      * Color differentiation flags are equal;
      * Reference white colors are equal;
      * The process background flags are equal;
      * Brightness values are equal.
      * Midtone values are equal.
      * Contrast values are equal.
      * Daylight exterior modes are equal.
      \param params [in]  Tone operator parameters to compare.
      \returns
      true if parameters are equal, false otherwise.
    */
    bool operator ==(const OdGiToneOperatorParameters &params) const { return params == *this; }
    
    /** \details
      Inequality operator for the OdGiToneOperatorParametersStatic class.
      The parameters are considered not equal if one of the following condition fails:
      * The active state flags are equal;
      * Chromatic adaptation flags are equal;
      * Color differentiation flags are equal;
      * Reference white colors are equal;
      * The process background flags are equal;
      * Brightness values are equal.
      * Midtone values are equal.
      * Contrast values are equal.
      * Daylight exterior modes are equal.
      \param params [in]  Object to compare.
      \returns
      true if parameters are not equal, false otherwise.
    */
    bool operator !=(const OdGiToneOperatorParameters &params) const { return params != *this; }
    OdGiToneOperatorParameters &operator =(const OdGiToneOperatorParameters &params) { return (static_cast<OdGiToneOperatorParameters&>(*this) = params); }
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGiToneOperatorParameters object pointers.
*/
typedef OdSmartPtr<OdGiToneOperatorParameters> OdGiToneOperatorParametersPtr;

/** \details
    Class that contains photographic exposure parameters. 
    Corresponding C++ library: TD_Gi
    <group OdGi_Classes>
*/
class FIRSTDLL_EXPORT OdGiPhotographicExposureParameters : public OdGiToneOperatorParameters
{
public:
  ODRX_DECLARE_MEMBERS(OdGiPhotographicExposureParameters);

  /** \details
    Default constructor for the OdGiPhotographicExposureParameters class. Sets default properties of OdGiToneOperatorParameters and initializes the following data members:
    <table>
    Name                      Value               Description
    m_fExposure               true                Exposure value.
    m_fWhitePoint             false               White point in Kelvins which affects the chromaticity of a scene.
    </table>
  */
  OdGiPhotographicExposureParameters();
  
  /** \details
    Copy constructor for the OdGiPhotographicExposureParameters class. Copies the data members of the passed object.
    \param params [in]  Photographic exposure parameters to copy.
  */
  OdGiPhotographicExposureParameters(const OdGiPhotographicExposureParameters &params);
  
  /** \details
    Destructor for the OdGiPhotographicExposureParameters class.
  */
  virtual ~OdGiPhotographicExposureParameters() { }

  /** \details
    Equality operator for the OdGiPhotographicExposureParameters class.
    \param params [in]  Tone operator parameters to compare.
    \returns
    true if parameters are equal, false otherwise.
  */
  bool operator ==(const OdGiPhotographicExposureParameters &params) const;
  
  /** \details
    Inequality operator for the OdGiPhotographicExposureParameters class.
    \param params [in]  Tone operator parameters to compare.
    \returns
    true if parameters are not equal, false otherwise.
  */
  bool operator !=(const OdGiPhotographicExposureParameters &params) const;
  
  /** \details
    Assignment operator for the OdGiPhotographicExposureParameters class. Returns a reference to this object with new assigned parameters.
    \param params [in]  Parameters to assign.
  */
  OdGiPhotographicExposureParameters &operator =(const OdGiPhotographicExposureParameters &params);

  
  /** \details
    Sets the exposure value. This value is a combination of a camera shutter speed and f-number (depth of field).
    \param fExposure [in]  Photographic exposure in range -6.0..21.0.
  */
  virtual bool setExposure(double fExposure);
  
  /** \details
    Retrieves the exposure value.
  */
  virtual double exposure() const;

  
  /** \details
    Sets the white point which affects the chromaticity of a scene.
    \param fWhitePoint [in]  White point in kelvins in range 1000.0..20000.0.
  */
  virtual bool setWhitePoint(double fWhitePoint);
  
  /** \details
    Retrieves the white point in kelvins.
  */
  virtual double whitePoint() const;

  /** \details
    Sets the brightness for the parent OdGiToneOperatorParameters.
    \param fBrightness [in]  Brightness in range 0.0..200.0.
  */
  virtual bool setBrightness(double fBrightness);

  /** \details
    Returns the converted white point to a color. 
  */
  OdCmEntityColor whitePointToColor() const;

  /** \details
    Returns the converted exposure to a brightness value.
    \param fExposure [in]  Exposure value to convert.
  */
  static double convertExposureToBrightness(double fExposure);
  
  /** \details
    Returns the converted brightness value to an exposure.
    \param fBrightness [in]  Brightness value to convert.
  */
  static double convertBrightnessToExposure(double fBrightness);
private:
  double m_fExposure;
  double m_fWhitePoint;
};

/** \details
    Corresponding C++ library: TD_Gi
    <group OdGi_Classes>
*/
class OdGiPhotographicExposureParametersStatic : public OdStaticRxObject<OdGiPhotographicExposureParameters>
{
public:
  /** \details
    Default constructor for the OdGiPhotographicExposureParametersStatic class.
  */
  OdGiPhotographicExposureParametersStatic() : OdStaticRxObject<OdGiPhotographicExposureParameters>() { }
  /** \details
    Equality operator for the OdGiPhotographicExposureParametersStatic class.
    \param params [in]  Parameters to compare.
    \returns
    true if parameters are equal, false otherwise.
  */
  bool operator ==(const OdGiPhotographicExposureParameters &params) const { return params == *this; }
  
  /** \details
    Inequality operator for the OdGiPhotographicExposureParametersStatic class.
    \param params [in]  Parameters to compare.
    \returns
    true if parameters are not equal, false otherwise.
  */
  bool operator !=(const OdGiPhotographicExposureParameters &params) const { return params != *this; }
  
  /** \details
    Assignment operator for the OdGiPhotographicExposureParametersStatic class. Returns a reference to the OdGiPhotographicExposureParameters object with new assigned parameters.
    \param params [in]  Parameters to assign.
  */
  OdGiPhotographicExposureParameters &operator =(const OdGiPhotographicExposureParameters &params) { return (static_cast<OdGiPhotographicExposureParameters&>(*this) = params); }
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGiPhotographicExposureParameters object pointers.
*/
typedef OdSmartPtr<OdGiPhotographicExposureParameters> OdGiPhotographicExposureParametersPtr;

/** \details
    Corresponding C++ library: TD_Gi
    <group OdGi_Classes>
    \sa
    <link tv_working_with_lights, Work with Lights>
*/
class FIRSTDLL_EXPORT OdGiViewportTraits : public OdGiSubEntityTraits
{
public:
  ODRX_DECLARE_MEMBERS(OdGiViewportTraits);

  /** \details
    Represents default lighting types for a viewport.
  */
  enum DefaultLightingType 
  {
    /** One distant light (that is located infinitely far from a scene). */
    kOneDistantLight  = 0,
    /** Two distant lights (that are located infinitely far from a scene). */
    kTwoDistantLights = 1,
    /** Background is the source of default lighting. */
    kBackLighting     = 2,
    /** Default lighting is defined by user. */
    kUserDefinedLight = 3
  };

  
  /** \details
    Checks whether default lighting is turned on.
    \returns
    true if default lighting is on, false otherwise.
  */
  virtual bool isDefaultLightingOn() const = 0;
  /** \details
    Enables or disables the default lighting.
    \param b [in]   Flag that specifies whether to enable the default lighting.
  */
  virtual void setDefaultLightingOn(bool b) = 0;

  
  /** \details
    Retrieves the default lighting type.
  */
  virtual DefaultLightingType defaultLightingType() const = 0;
  /** \details
    Sets the default lighting type.
    \param DefaultLightingType [in]   One of the default lighting types.
  */
  virtual void setDefaultLightingType(DefaultLightingType) = 0;

  
  /** \details
    Retrieves the user-defined light direction for distant lights.
  */
  virtual OdGeVector3d userDefinedLightDirection() const = 0;
  /** \details
    Sets the user-defined light direction for distant lights.
    \param lightDirection [in]   Vector that specifies the light direction.
  */
  virtual void setUserDefinedLightDirection(const OdGeVector3d& lightDirection) = 0;

  
  /** \details
    Retrieves the intensity of default lighting.
  */
  virtual double defaultLightingIntensity() const = 0;
  /** \details
    Sets the intensity of default lighting.
    \param dIntensity [in]   Default lighting intensity.
  */
  virtual void setDefaultLightingIntensity(double dIntensity) = 0;

  
  /** \details
    Retrieves the default lighting color.
  */
  virtual OdCmEntityColor defaultLightingColor() const = 0;
  /** \details
    Sets the intensity of default lighting.
    \param color [in]   Default lighting color.
  */
  virtual void setDefaultLightingColor(const OdCmEntityColor& color) = 0;

  
  /** \details
    Retrieves the default lighting shadow parameters.
    \param params [out]   Receives default shadow parameters.
  */
  virtual void defaultLightingShadowParameters(OdGiShadowParameters &params) const = 0;
  /** \details
    Sets the default lighting shadow parameters.
    \param params [in]   Shadow parameters.
  */
  virtual void setDefaultLightingShadowParameters(const OdGiShadowParameters &params) = 0;

  
  /** \details
    Sets the ambient color (color in the shadows) for this viewport.
    \param color [in]   Ambient color.
  */
  virtual void setAmbientLightColor(const OdCmEntityColor& color) = 0;
  /** \details
    Retrieves the ambient color (color in the shadows).
  */
  virtual OdCmEntityColor ambientLightColor() const = 0;

  
  /** \details
    Sets the overall contrast for this viewport.
    \param contrast [in]   Light contrast value.
  */
  virtual void setContrast(double contrast) = 0;
  /** \details
    Retrieves the overall contrast for this viewport.
  */
  virtual double contrast() const = 0;

  
  /** \details
    Sets the brightness for this viewport.
    \param brightness [in]   Brightness value.
  */
  virtual void setBrightness(double brightness) = 0;
  /** \details
    Retrieves the brightness for this viewport.
  */
  virtual double brightness() const = 0;

  
  /** \details
    Sets the background for this viewport.
    \param bg [in]   Pointer to the background.
  */
  virtual void setBackground(OdDbStub* bg) = 0;
  /** \details
    Retrieves a pointer to the background for this viewport.
  */
  virtual OdDbStub* background() const = 0;

  
  /** \details
    Sets the render environment for this viewport. Used only in full renders.
    \param re [in]   Pointer to the render environment.
  */
  virtual void setRenderEnvironment(OdDbStub* re) = 0;
  /** \details
    Retrieves a pointer to the render environment for this viewport. Used only in full renders.
  */
  virtual OdDbStub* renderEnvironment () const = 0;

  
  /** \details
    Sets the render settings for this viewport. Used only in full renders.
    \param rs [in]   Pointer to the render settings.
  */
  virtual void setRenderSettings(OdDbStub* rs) = 0;
  /** \details
    Retrieves a pointer to the render settings for this viewport. Used only in full renders.
  */
  virtual OdDbStub* renderSettings() const = 0;

  
  /** \details
    Sets the tone parameters for this viewport.
    \param params [in]   Tone operator parameters.
  */
  virtual void setToneOperatorParameters(const OdGiToneOperatorParameters &params) = 0;
  /** \details
    Retrieves tone parameters for this viewport.
    \param params [out]   Receives tone operator parameters.
  */
  virtual void toneOperatorParameters(OdGiToneOperatorParameters &params) = 0;
};

/** \details
  A data type that represents a smart pointer to an <link OdGiViewportTraits, OdGiViewportTraits> object.
*/
typedef OdSmartPtr<OdGiViewportTraits> OdGiViewportTraitsPtr;

#include "TD_PackPop.h"

#endif // __OD_GI_VIEWPORT_TRAITS__
