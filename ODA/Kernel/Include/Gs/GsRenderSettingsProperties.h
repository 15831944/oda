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

#ifndef __OD_GS_RENDERSETTINGS_PROPERTIES__
#define __OD_GS_RENDERSETTINGS_PROPERTIES__

#include "TD_PackPush.h"

#include "Gs/GsProperties.h"
#include "Gi/GiEnvironmentTraitsData.h"

// Manages:
// OdRenderEnvironment
// OdRenderSettings
// OdMentalRayRenderSettings

/** <group OdGs_Classes> 
*/
class GS_TOOLKIT_EXPORT OdGsRenderEnvironmentProperties : public OdGsProperties
{
  OdGiRenderEnvironmentTraitsData* m_pRenderEnvironmentTraits;

public:
  ODRX_DECLARE_MEMBERS(OdGsRenderEnvironmentProperties);

  /** \details
    Default constructor for the OdGsRenderEnvironmentProperties class. Creates object with OdGiRenderEnvironmentTraitsData object pointer that points to NULL.
  */
  OdGsRenderEnvironmentProperties();
  
  /** \details
    Destructor for the OdGsRenderEnvironmentProperties class. Clears underlying OdGiRenderEnvironmentTraitsData object and destroys this object.
  */
  ~OdGsRenderEnvironmentProperties();

  /** \details
    Returns pointer to the OdGiRenderEnvironmentTraitsData object that represents render environment traits.
  */
  OdGiRenderEnvironmentTraitsData* renderEnvironmentTraitsData() const { return m_pRenderEnvironmentTraits; }

  /** \details
    Checks whether render environment traits are modified.
    \returns
    true if at least one of the following conditions is true:
    * Pointer to render environment traits (OdGiRenderEnvironmentTraitsData) is NULL
    * This object has no underlying drawable
    * Render environment traits data has been modified
    In other cases this method returns false.
  */
  bool isTraitsModified();

  /** \details
    Clears render environment traits of the underlying OdGiRenderEnvironmentTraitsData object.
  */
  void clearTraits();

  // OdGsProperties virtual overrides
  
  /** \details
    Retrieves the property type.
  */
  virtual PropertiesType propertiesType() const
  {
    return kRenderEnvironment;
  }

  /** \details
    Retrieves smart pointer to the properties by the specified type.
    
    \param type  [in]  Property type.
    \returns
    smart pointer to this object is the passed type is kRenderEnvironment, otherwise returns smart pointer to generic OdGsProperties object.
  */
  virtual OdSmartPtr<OdGsProperties> propertiesForType(PropertiesType type) const;

  virtual void update(const OdGiDrawable* pUnderlyingDrawable, OdGsViewImpl* view, OdUInt32 incFlags = kAll);

  virtual void display(OdGsBaseVectorizer& view, OdGsPropertiesDirectRenderOutput *pdro = NULL, OdUInt32 incFlags = kAll);
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGsRenderEnvironmentProperties object pointers.
*/
typedef OdSmartPtr<OdGsRenderEnvironmentProperties> OdGsRenderEnvironmentPropertiesPtr;

/** <group OdGs_Classes> 
*/
class GS_TOOLKIT_EXPORT OdGsRenderSettingsProperties : public OdGsProperties
{
  OdGiCombinedRenderSettingsTraitsData* m_pRenderSettingsTraits;

public:
  ODRX_DECLARE_MEMBERS(OdGsRenderSettingsProperties);

  /** \details
    Default constructor for the OdGsRenderSettingsProperties class. Creates object with OdGiCombinedRenderSettingsTraitsData object pointer that points to NULL.
  */
  OdGsRenderSettingsProperties();
  
  /** \details
    Destructor for the OdGsRenderSettingsProperties class. Clears underlying OdGiCombinedRenderSettingsTraitsData object and destroys this object.
  */
  ~OdGsRenderSettingsProperties();

  
  /** \details
    Returns pointer to the OdGiCombinedRenderSettingsTraitsData object that represents render settings traits.
  */
  OdGiRenderSettingsTraitsData* renderSettingsTraitsData() const { return m_pRenderSettingsTraits; }
  
  /** \details
    Checks whether this object has Mental Ray render settings.
    \returns
    true if this object has Mental Ray render settings, false otherwise.
  */
  bool hasMentalRayRenderSettings() const { return (m_pRenderSettingsTraits) ? m_pRenderSettingsTraits->hasMentalRayRenderSettings() : false; }
  
  /** \details
     Returns pointer to the Mental Ray render settings.
  */
  OdGiMentalRayRenderSettingsTraitsData* mentalRayRenderSettingsTraitsData() const { return m_pRenderSettingsTraits; }
  
  /** \details
    Checks whether this object has Rapid RT render settings.
    \returns
    true if this object has Rapid RT render settings, false otherwise.
  */
  bool hasRapidRTRenderSettings() const { return (m_pRenderSettingsTraits) ? m_pRenderSettingsTraits->hasRapidRTRenderSettings() : false; }
  
  /** \details
     Returns pointer to the Rapid RT render settings.
  */
  OdGiRapidRTRenderSettingsTraitsData* rapidRTRenderSettingsTraitsData() const { return m_pRenderSettingsTraits; }

  /** \details
    Checks whether render settings traits are modified.
    \returns
    true if render settings traits are modified, false otherwise.
  */
  bool isTraitsModified();

  /** \details
    Clears render settings traits of the underlying OdGiCombinedRenderSettingsTraitsData object.
  */
  void clearTraits();

  // OdGsProperties virtual overrides
  
  /** \details
    Retrieves the property type.
  */
  virtual PropertiesType propertiesType() const
  {
    return kRenderSettings;
  }

  /** \details
    Retrieves smart pointer to the properties by the specified type.
    
    \param type  [in]  Property type.
    \returns
    smart pointer to this object is the passed type is kRenderSettings, otherwise returns smart pointer to generic OdGsProperties object.
  */
  virtual OdSmartPtr<OdGsProperties> propertiesForType(PropertiesType type) const;

  virtual void update(const OdGiDrawable* pUnderlyingDrawable, OdGsViewImpl* view, OdUInt32 incFlags = kAll);

  virtual void display(OdGsBaseVectorizer& view, OdGsPropertiesDirectRenderOutput *pdro = NULL, OdUInt32 incFlags = kAll);
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGsRenderSettingsProperties object pointers.
*/
typedef OdSmartPtr<OdGsRenderSettingsProperties> OdGsRenderSettingsPropertiesPtr;

#include "TD_PackPop.h"

#endif // __OD_GS_RENDERSETTINGS_PROPERTIES__
