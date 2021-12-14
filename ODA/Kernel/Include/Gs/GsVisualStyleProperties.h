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

#ifndef __OD_GS_VISUALSTYLE_PROPERTIES__
#define __OD_GS_VISUALSTYLE_PROPERTIES__

#include "TD_PackPush.h"

#include "Gs/GsProperties.h"
#include "Gi/GiVisualStyle.h"
#include "Gi/GiVisualStyleTraitsData.h"

// VisualStyles is not entities or nodes
// VisualStyles can be dynamicly modified outside viewport's:
//   OdGsPropertiesPtr vs = props->propertiesForType(kVisualStyle);
//   vs->update(newVS, view, kVisualStyle);
// Then may be putted to view:
//   vs->display(this->view, NULL, kVisualStyle);
// Or accesed directly from viewport (on onTraitsModified for example):
//   OdGsVisualStylePropertiesPtr pVS = props->propertiesForType(kVisualStyle);
//   OdGiVisualStyleTraitsData* pVSData = pVS->visualStyleTraitsData();
//   pVSData->(useOfData());

/** <group OdGs_Classes> 
*/
class GS_TOOLKIT_EXPORT OdGsVisualStyleProperties : public OdGsProperties
{
  OdGiVisualStyleTraitsData* m_pVisualStyleTraits;

public:
  ODRX_DECLARE_MEMBERS(OdGsVisualStyleProperties);

  /** \details
    Default constructor for the OdGsVisualStyleProperties class. Sets visual style traits (pointer to OdGiVisualStyleTraitsData object) to NULL.
  */
  OdGsVisualStyleProperties();
  
  /** \details
    Destructor for the OdGsVisualStyleProperties class. Clears visual style traits.
  */
  ~OdGsVisualStyleProperties();

  /** \details
    Retrieves pointer to visual style traits data object.
  */
  OdGiVisualStyleTraitsData* visualStyleTraitsData() const { return m_pVisualStyleTraits; }

  /** \details
    Checks whether visual style traits are modified.
    
    \returns
    true if visual style traits are modified, false otherwise.
  */
  bool isTraitsModified();

  /** \details
    Clears viewport traits (background, visual style, render environment and render settings) if they are not NULL.
  */
  void clearTraits();

  // OdGsProperties virtual overrides
  
  /** \details
    Retrieves type of properties for this object with is 'kVisualStyle'.
  */
  virtual PropertiesType propertiesType() const
  {
    return kVisualStyle;
  }

  /** \details
    Retrieves smart pointer to properties specified by the passed type.
    
    \param type [in]  Property type.
    \remarks
    If you pass the 'kVisualStyle' style the method returns a smart pointer to this object.
    In all other case method returns generic OdGsPropertiesPtr smart pointer.
  */
  virtual OdSmartPtr<OdGsProperties> propertiesForType(PropertiesType type) const;

  virtual void update(const OdGiDrawable* pUnderlyingDrawable,
      OdGsViewImpl* view, OdUInt32 incFlags = kAll);

  virtual void display(OdGsBaseVectorizer& view,
      OdGsPropertiesDirectRenderOutput *pdro = NULL, OdUInt32 incFlags = kAll);
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGsVisualStyleProperties object pointers.
*/
typedef OdSmartPtr<OdGsVisualStyleProperties> OdGsVisualStylePropertiesPtr;

#include "TD_PackPop.h"

#endif // __OD_GS_VISUALSTYLE_PROPERTIES__
