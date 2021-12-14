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

#ifndef _ODGILINEWEIGHTOVERRIDE_INCLUDED_
#define _ODGILINEWEIGHTOVERRIDE_INCLUDED_

#include "Gi/Gi.h"
#include "Ps/PlotStyles.h"

#include "TD_PackPush.h"

/** \details
    This class provides an interface to override OdGsView lineweight settings.
  
    \sa
    <link tv_working_with_attributes_lineweights, Work with Lineweights>
    
    <group OdGi_Classes> 
*/
struct OdGiLineweightOverride
{
  protected:
    enum Flags
    {
      kLwdOverrideScale = (1 << 0),
      kLwdOverrideStyle = (1 << 1)
    };
    OdUInt32            m_uFlags;
    double              m_dLwdScale;
    double              m_dPixScale;
    OdPs::LineEndStyle  m_lineCapStyle;
    OdPs::LineJoinStyle m_lineJoinStyle;
  public:
    /** \details
      Default constructor for the OdGiLineweightOverride class. Sets the following data members:
      <table>
      Data member           Value             Description
      m_uFlags              0                 Lineweight flags
      m_dLwdScale           1.0               Lineweight scale
      m_dPixScale           1.0               Pixel scale
      m_lineCapStyle        OdPs::kLesRound   Style for line caps
      m_lineJoinStyle       OdPs::kLjsRound   Style for line joining
      </table>
    */
    OdGiLineweightOverride()
      : m_uFlags(0)
      , m_dLwdScale(1.0)
      , m_dPixScale(1.0)
      , m_lineCapStyle(OdPs::kLesRound)
      , m_lineJoinStyle(OdPs::kLjsRound)
    { }
    /** \details
      Overrides scale properties.
      
      \param dLwdScale [in]  Lineweight scale.
    */
    void setScaleOverride(double dLwdScale)
    {
      SETBIT_1(m_uFlags, kLwdOverrideScale);
      m_dLwdScale = m_dPixScale = dLwdScale;
    }
    
    /** \details
      Resets scale overrides.
    */
    void resetScaleOverride()
    {
      SETBIT_0(m_uFlags, kLwdOverrideScale);
      m_dLwdScale = m_dPixScale = 1.0;
    }
    
    /** \details
      Checks whether this lineweight has a scale override.
      
      \returns
      true if lineweight override is present, false otherwise.
    */
    bool hasScaleOverride() const
    {
      return GETBIT(m_uFlags, kLwdOverrideScale);
    }
    
    /** \details
      Retrieves the lineweight scale override value.
      
      \returns
      lineweight scale override as a value of the double type.
    */
    double scaleOverride() const
    {
      return m_dLwdScale;
    }
    // Helpers
    /** \details
      Specifies pixel scale property.
      
      \param dPixScale [in]  Pixel scale value.
    */
    void setPixelScale(double dPixScale)
    {
      m_dPixScale = dPixScale;
    }
    
    /** \details
      Retrieves the pixel scale value.
      
      \returns
      pixel scale as a value of the double type.
    */
    double pixelScale() const
    {
      return m_dPixScale;
    }
    // Style override
    /** \details
      Overrides line style properties.
      
      \param lineCapStyle [in]  Style for a line cap.
      \param lineJoinStyle [in]  Style for a line joining.
    */
    void setStyleOverride(OdPs::LineEndStyle lineCapStyle, OdPs::LineJoinStyle lineJoinStyle)
    {
      SETBIT_1(m_uFlags, kLwdOverrideStyle);
      m_lineCapStyle = lineCapStyle;
      m_lineJoinStyle = lineJoinStyle;
    }
    /** \details
      Resets line style overrides.
    */
    void resetStyleOverride()
    {
      SETBIT_0(m_uFlags, kLwdOverrideStyle);
      m_lineCapStyle = OdPs::kLesRound;
      m_lineJoinStyle = OdPs::kLjsRound;
    }
    /** \details
      Checks whether this lineweight has line style overrides.
      
      \returns
      true if lineweight has line style overrides, false otherwise.
    */
    bool hasStyleOverride() const
    {
      return GETBIT(m_uFlags, kLwdOverrideStyle);
    }
    /** \details
      Retrieves the line cap style override.
      
      \returns
      lineweight cap style override as a value from the OdPs::LineEndStyle enumeration.
    */
    OdPs::LineEndStyle endStyleOverride() const
    {
      return m_lineCapStyle;
    }
    /** \details
      Retrieves the line joining style override.
      
      \returns
      lineweight joining style override as a value from the OdPs::LineJoinStyle enumeration.
    */
    OdPs::LineJoinStyle joinStyleOverride() const
    {
      return m_lineJoinStyle;
    }
    // Helpers
    
    /** \details
      Checks whether this lineweight has any overrides.
      
      \returns
      true if lineweight has overrides, false otherwise.
    */
    bool hasOverrides() const
    {
      return m_uFlags != 0;
    }
    
    /** \details
      Equality operator for this lineweight override. Lineweight overrides
      are equal if:
      * Lineweight override flags are equal;
      * Lineweight override scales are equal;
      * Lineweight override line cap styles are equal;
      * Lineweight override line join styles are equal;
      
      \param lwdO2 [in]  Lineweight override to compare.
      
      \returns
      true if this lineweight override is equal to the passed lineweight override, false otherwise.
    */
    bool operator ==(const OdGiLineweightOverride &lwdO2) const
    {
      if (lwdO2.m_uFlags != m_uFlags)
        return false;
      if (hasScaleOverride())
      {
        if (!OdEqual(lwdO2.m_dLwdScale, m_dLwdScale))
          return false;
      }
      if (hasStyleOverride())
      {
        if (lwdO2.m_lineCapStyle != m_lineCapStyle ||
            lwdO2.m_lineJoinStyle != m_lineJoinStyle)
          return false;
      }
      return true;
    }
    /** \details
      Inequality operator for this lineweight override. Lineweight overrides
      are not equal if one of the following conditions fails:
      * Lineweight override flags are equal;
      * Lineweight override scales are equal;
      * Lineweight override line cap styles are equal;
      * Lineweight override line join styles are equal;
      
      \param lwdO2 [in]  Lineweight override to compare.
      
      \returns
      true if this lineweight override is not equal to the passed lineweight override, false otherwise.
    */
    bool operator !=(const OdGiLineweightOverride &lwdO2) const
    {
      if (lwdO2.m_uFlags != m_uFlags)
        return true;
      if (hasScaleOverride())
      {
        if (!OdEqual(lwdO2.m_dLwdScale, m_dLwdScale))
          return true;
      }
      if (hasStyleOverride())
      {
        if (lwdO2.m_lineCapStyle != m_lineCapStyle ||
            lwdO2.m_lineJoinStyle != m_lineJoinStyle)
          return true;
      }
      return false;
    }
};

/** \details
    Helper class to simplify lineweight override setting.
  
    \sa
    TD_Gi
    
    <group OdGi_Classes> 
*/
class ODGI_EXPORT OdGiLineweightOverrideHelper
{
  protected:
    OdGiSubEntityTraits *m_pTraits;
  public:
    /** \details
      Default constructor for the OdGiLineweightOverrideHelper class. Sets the lineweight override traits to NULL.
    */
    OdGiLineweightOverrideHelper() : m_pTraits(NULL) {}
    
    /** \details
      Constructor for the OdGiLineweightOverrideHelper class. Sets the object with the specified traits  and underlaying override object.
      \param pTraits [in]  Entity traits.
      \param pOverride [in]  Lineweight override object to wrap.
    */
    OdGiLineweightOverrideHelper(OdGiSubEntityTraits &pTraits, const OdGiLineweightOverride &pOverride) : m_pTraits(NULL)
    { setLineweightOverride(pTraits, pOverride); }
    
    /** \details
      Destructor for the OdGiLineweightOverrideHelper class.
    */
    ~OdGiLineweightOverrideHelper()
    {
      if (m_pTraits)
        m_pTraits->popLineweightOverride();
    }
    
    /** \details
      Sets the specified traits and lineweight object.
      \param pTraits [in]  Entity traits.
      \param pOverride [in]  Lineweight override object to wrap.
    */
    void setLineweightOverride(OdGiSubEntityTraits &pTraits, const OdGiLineweightOverride &pOverride)
    {
      m_pTraits = &pTraits;
      m_pTraits->pushLineweightOverride(&pOverride);
    }
};

#include "TD_PackPop.h"

#endif //_ODGILINEWEIGHTOVERRIDE_INCLUDED_
