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

#ifndef _COLLADA_LIGHTTRAITS_INCLUDED_
#define _COLLADA_LIGHTTRAITS_INCLUDED_
#pragma once

#include "Gi/GiDrawable.h"
#include "DbBaseDatabase.h"
#include "Ge/GePoint3d.h"
#include "SmartPtr.h"
#include "Gi/GiLightTraitsData.h"

/** \details
  Provides a set of classes, structures and enumerations for working with Collada export.
*/
namespace TD_COLLADA_EXPORT
{
  /** \details
    This class implements the distant light traits for Collada.
  */
  class ColladaDistantLightTraits : public OdGiDistantLightTraits
  {
    /** Pointer of distant light traits. */
    OdGiDistantLightTraitsData* m_pData;
  public:

    /** \details
      \returns the intensity of the light source as a value of the double type.
    */
    virtual double intensity() const;

    /** \details
      Sets the distant light intensity.

      \param inten [in] The distant light intensity.
    */
    virtual void setIntensity( double inten );

    /** \details
      \returns shadow parameters as OdGiShadowParameters instance.
    */
    virtual OdGiShadowParameters shadowParameters() const;

    /** \details
      Sets the shadow parameters.

      \param params [in] The shadow parameters.
    */
    virtual void setShadowParameters( const OdGiShadowParameters& params );

    /** \details
      \returns the distant light color.
    */
    virtual OdCmEntityColor lightColor() const;

    /** \details
      Sets the distant light color.

      \param color [in] The distant light color.
    */
    virtual void setLightColor( const OdCmEntityColor& color );

    /** \details
      Checks whether the distant light is on or off.

      \returns true if light is enabled, false otherwise.
    */
    virtual bool isOn() const;
    
    /** \details
      Switches the distant light to the on or off.

      \param on [in] The distant light switch value.
    */
    virtual void setOn(bool on);

    /** \details
       \returns the distant light direction.
    */
    virtual OdGeVector3d lightDirection() const;

    /** \details
      Sets the distant light direction.

      \param vec [in] The distant light direction.
    */
    virtual void setLightDirection( const OdGeVector3d& vec );

    /** \details
      Checks whether the sun light is on.

      \returns true if this light is rendered as sunlight, false otherwise.
    */
    virtual bool isSunlight() const;

    /** \details
      Switches the sun light to the on or off.
      
      \param isSunlight [in] The sun light value.
    */
    virtual void setIsSunlight( bool isSunlight );

    /** \details
      Sets the distant light physical intensity.
    
      \param fIntensity [in] The distant light physical intensity.
    */
    virtual void setPhysicalIntensity(double fIntensity);

    /** \details
      \returns physical intensity of the distant light as a value of the double type.
    */
    virtual double physicalIntensity() const;

    /** \details
      Sets color for the distant light lamp.

      \param color  [in]  RGB color value.
    */
    virtual void setLampColor(const OdGiColorRGB& color);

    /** \details
      Returns color of the light lamp.
    
      \returns color of the distant light lamp as OdGiColorRGB instance.
    */
    virtual OdGiColorRGB lampColor() const;

    /** \details
      Sets sky parameters for this distant light. Currently not implemented.

      \param params [in] The sky parameters for this distant light.
    */
    virtual void setSkyParameters(const OdGiSkyParameters &params);

    /** \details
      Returns sky parameters of this distant light. Currently not implemented.

      \returns sky parameters of this distant light.
    */
    virtual void skyParameters(OdGiSkyParameters &params) const;

    /** \details
      Sets distant lights traits data.
    
      \param data [in] The distant lights traits data.
    */
    void setData(OdGiDistantLightTraitsData& data);
  };


  //-------------------------------------------------------------------------------------------------------------------

  /** \details
    This class implements the point light traits for Collada.
  */
  class ColladaPointLightTraits : public OdGiPointLightTraits
  {
    /** Pointer of point light traits. */
    OdGiPointLightTraitsData* m_pData;
  public:

    /** \details
       \returns the point light intensity.
    */
    virtual double intensity() const;

    /** \details
       Sets the point light intensity.

       \param inten [in] The point light intensity.
    */
    virtual void setIntensity( double inten );

    /** \details
       \returns the shadow parameters.
    */
    virtual OdGiShadowParameters shadowParameters() const;

    /** \details
       Sets the shadow parameters.

       \param params [in] The shadow parameters.
    */
    virtual void setShadowParameters( const OdGiShadowParameters& params );

    /** \details
       \returns the point light color.
    */
    virtual OdCmEntityColor lightColor() const;

    /** \details
       Sets the point light color.

       \param color [in] The point light color.
    */
    virtual void setLightColor( const OdCmEntityColor& color );

    /** \details
       Checks whether the point light is on or off.

       \returns true if point light is enabled, false otherwise.
    */
    virtual bool isOn() const;

    /** \details
       Switches the point light to the on or off.

       \param on [in] The point light value.
    */
    virtual void setOn(bool on);

    /** \details
       Returns the point light position.
    */
    virtual OdGePoint3d position() const;

    /** \details
       Sets the point light position.
    */
    virtual void setPosition( const OdGePoint3d& pos );

    /** \details
      \returns the point light attenuation.
    */
    virtual OdGiLightAttenuation lightAttenuation() const;


    /** \details
      Sets the point light attenuation.
    
      \param atten [in] The point light attenuation.
    */
    virtual void setAttenuation( const OdGiLightAttenuation& atten );

    /** \details
      Sets physical intensity for this point light source in candela.
    
      \param fIntensity  [in]  Physical intensity of the point light in candela.
    */
    virtual void setPhysicalIntensity(double fIntensity);
    
    /** \details
      Returns physical intensity for this point light source in candela.
    
      \returns physical intensity of the point light as a value of the double type.
    */
    virtual double physicalIntensity() const;

    /** \details
      Sets color for the point light lamp.
    
      \param color  [in]  RGB color value.
    */
    virtual void setLampColor(const OdGiColorRGB& color);

    /** \details
      Retrieves color of the point light lamp.
    
      \returns color of the point light lamp as OdGiColorRGB instance.
    */
    virtual OdGiColorRGB lampColor() const;

    /** \details
      Sets whether this point should have an associated target grip displayed.
    
      \param bTarget  [in]  Flag that specifies whether to display a target grip for orienting the light.
    */
    virtual void setHasTarget(bool bTarget);

    /** \details
      Checks whether this point has an associated target grip displayed.
    
      \returns
      true if light displays a target grip, false otherwise.
    */
    virtual bool hasTarget() const;

    /** \details
      Sets target location of the light.
     
      \param loc  [in]  Location of the target. Must differ from the point light position.
    */
    virtual void setTargetLocation(const OdGePoint3d &loc);

    /** \details
      Returns target location of the light.
    
      \returns
      target location of the light as OdGePoint3d instance.
    */
    virtual OdGePoint3d targetLocation() const;

    /** \details
      Sets hemi spherical distribution.
 
      \param loc  [in]  hemi spherical distribution.
    */
    virtual void setHemisphericalDistribution(bool bHemisphere);

    /** \details
      Returns hemi spherical distribution.
    */
    virtual bool hemisphericalDistribution() const;

    /** \details
      Sets point lights traits data.
    
      \param data [in] The point lights traits data.
    */
    void setData(OdGiPointLightTraitsData& data);
  };

  //-------------------------------------------------------------------------------------------------------------------

  /** \details
    This class implements the spot light traits for Collada.
  */
  class ColladaSpotLightTraits : public OdGiSpotLightTraits
  {
    /** Pointer of spot light traits. */
    OdGiSpotLightTraitsData* m_pData;
  public:
    /** \details
       Returns the spot light intensity.
    */
    virtual double intensity() const;

    /** \details
       Sets the spot light intensity.
    */
    virtual void setIntensity( double inten );

    /** \details
       Returns the shadow parameters.
    */
    virtual OdGiShadowParameters shadowParameters() const;

    /** \details
       Sets the shadow parameters.
    */
    virtual void setShadowParameters( const OdGiShadowParameters& params );

    /** \details
       Returns the spot light color.
    */
    virtual OdCmEntityColor lightColor() const;

    /** \details
       Sets the spot light color.
    */
    virtual void setLightColor( const OdCmEntityColor& color );

    /** \details
       Checks whether the spot light is on or off.
    */
    virtual bool isOn() const;

    /** \details
       Switches the spot light to the on or off.

       \param on [in] The point light switch value.
    */
    virtual void setOn(bool on);

    /** \details
       \returns the spot light target point.
    */
    virtual OdGePoint3d targetLocation() const;

    /** \details
       Sets the spot light target point.

       \param loc [in] The spot light target  point.
    */
    virtual void setTargetLocation( const OdGePoint3d& loc );

    /** \details
       \returns the spot light position.
    */
    virtual OdGePoint3d position() const;

    /** \details
       Sets the spot light position.

       \param pos [in] The spot light position.
    */
    virtual void setPosition( const OdGePoint3d& pos );

    /** \details
      Sets the spot light attenuation.

      \param atten [in] The spot light attenuation.
    */
    virtual void setAttenuation( const OdGiLightAttenuation& atten );

    /** \details
      \returns the spot light attenuation.
    */
    virtual OdGiLightAttenuation lightAttenuation() const;

    /** \details
      Returns the cone angle that produces the spot light falloff.
    
      \returns
      cone of the falloff as a value of the double type.
    */
    virtual double falloff() const;

    /** \details
      Sets target location of the spot light.
    
      \param hotspot  [in]  Hotspot cone angle, in radians. Defines the brightest part of the spot light beam.
      \param falloff  [in]  Falloff angle, in radians. Defines the full cone of light.
    */
    virtual bool setHotspotAndFalloff( double hotspot, double falloff );

    /** \details
       \returns the hotspot.
    */
    virtual double hotspot() const;

    /** \details
      Sets physical intensity for this spot light source in candela.
    
      \param fIntensity  [in]  Physical intensity of the spot light in candela.
    */
    virtual void setPhysicalIntensity(double fIntensity);

    /** \details
      Returns physical intensity for this spot light source in candela.
    
      \returns
      physical intensity of the spot light as a value of the double type.
    */
    virtual double physicalIntensity() const;

    /** \details
      Sets color for the spot light lamp.
    
      \param color  [in]  RGB color value.
    */
    virtual void setLampColor(const OdGiColorRGB& color);

    /** \details
      Returns color of the spot light lamp.
    
      \returns
      color of the spot light lamp as OdGiColorRGB instance.
    */
    virtual OdGiColorRGB lampColor() const;

    /** \details
      Sets spot lights traits data.
    
      \param data [in] The spot traits data.
    */
    void setData(OdGiSpotLightTraitsData& data);
  };
};

#endif // _COLLADA_LIGHTTRAITS_INCLUDED_

