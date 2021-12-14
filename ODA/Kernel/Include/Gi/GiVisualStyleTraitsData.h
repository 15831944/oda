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

#ifndef __OD_GI_VISUALSTYLE_TRAITS_DATA__
#define __OD_GI_VISUALSTYLE_TRAITS_DATA__

#include "TD_PackPush.h"

#include "Gi/GiVisualStyle.h"

/** \details
    Corresponding C++ library: TD_Gi
    <group OdGi_Classes>
    \sa
    <link tv_working_with_attributes_visual_styles, Work with Visual Styles>
*/
class OdGiVisualStyleTraitsData
{
  const OdGiVisualStyle*    m_visualStylePtr;

public:

  /** \details
    Default constructor for the OdGiVisualStyleTraitsData class. Initializes the smart pointer to the associated visual style to NULL.
  */
  OdGiVisualStyleTraitsData()
    : m_visualStylePtr(NULL)
  {}
  
  /** \details
    Retrieves the drawable type of this object which is kGeometry.
  */
  OdGiDrawable::DrawableType type() const { return OdGiDrawable::kGeometry; }

  /** \details
    Sets visual style.
    \param visualStyle [in]  Visual style to set.
  */
  void setOdGiVisualStyle(const OdGiVisualStyle& visualStyle)
  {
    m_visualStylePtr = &visualStyle;
  }
  
  /** \details
    Retrieves the visual style.
  */
  const OdGiVisualStyle& odgiVisualStyle() const
  {
    return *m_visualStylePtr;
  }

  /** \details
    Checks whether visual style is set.
    \returns
    true if pointer to the associated visual style is not NULL, false otherwise.
  */
  bool isOdGiVisualStyleSet() const
  {
    return m_visualStylePtr != NULL;
  }
  /** \details
    Resets visual style.
  */
  void resetOdGiVisualStyle()
  {
    m_visualStylePtr = NULL;
  }

  /** \details
    Equality operator for the OdGiVisualStyleTraitsData class.
    The objects are considered equal if both visual styles are set
    and refer to the same visual style, or if both are not set.
    
    \param data2 [in]  Object to compare.
    \returns
    true if object are equal, false otherwise.
  */
  bool operator ==(const OdGiVisualStyleTraitsData &data2) const
  {
    return (isOdGiVisualStyleSet() == data2.isOdGiVisualStyleSet()) &&
           (!isOdGiVisualStyleSet() || (odgiVisualStyle() == data2.odgiVisualStyle()));
  }
  
  /** \details
    Inequality operator for the OdGiVisualStyleTraitsData class.
    The objects are considered not equal if the equality operator returns false.
    
    \param data2 [in]  Object to compare.
    \returns
    true if objects are not equal, false otherwise.
  */
  bool operator !=(const OdGiVisualStyleTraitsData &data2) const
  {
    return !(*this == data2);
  }
};                          

#include "TD_PackPop.h"

#endif // __OD_GI_VISUALSTYLE_TRAITS_DATA__
