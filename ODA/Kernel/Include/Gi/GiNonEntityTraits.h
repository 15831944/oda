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

#ifndef __ODGI_NONENTITYTRAITS_H__
#define __ODGI_NONENTITYTRAITS_H__

#include "TD_PackPush.h"

#include "Gi/Gi.h"
#include "OdCodePage.h"
#include "Gi/GiDrawable.h"

class OdGiLinetypeDash;
class OdFont;
class OdTtfDescriptor;
class OdGiTextStyle;

/** \details
    This class defines the interface to the layer traits for the vectorization process.
    
    Corresponding C++ library: TD_Gi
    <group OdGi_Classes> 
    \sa
    <link tv_working_with_attributes_layers, Work with Layers>
*/
class FIRSTDLL_EXPORT OdGiLayerTraits : public OdGiDrawableTraits
{
public:
  ODRX_DECLARE_MEMBERS(OdGiLayerTraits);

  // returned via subSetAttributes()

  enum
  {
    /** Traits inherited by block.*/
    kByBlock    = (OdGiDrawable::kLastFlag << 1),
    /** Frozen layer.*/
    kFrozen     = (OdGiDrawable::kLastFlag << 2),
    /** Layer is switched off.*/
    kOff        = (OdGiDrawable::kLastFlag << 3),
    /** Layer is plottable.*/
    kPlottable  = (OdGiDrawable::kLastFlag << 4),
    /** Layer is locked.*/
    kLocked     = (OdGiDrawable::kLastFlag << 5)
  };
  
  /** \details
    Retrieves the color for this object.
    
    \returns
    color as OdCmEntityColor instance.
    \sa
    <exref target="https://docs.opendesign.com/td/cm.html">Working with Colors</exref>
  */
  virtual OdCmEntityColor color() const = 0;

  /** \details
    Retrieves the lineweight for this object.
    
    \returns
    lineweight as a value from the OdDb::LineWeight enumeration.
    \sa
    <link tv_working_with_attributes_lineweights, Work with Lineweights>
  */
  virtual OdDb::LineWeight lineweight() const = 0;

  /** \details
    Retrieves the Object ID of the linetype for this object.
    
    \returns
    pointer to OdDbStub that represents linetype identifier in database.
    \sa
    <exref target="https://docs.opendesign.com/td/db_linetype.html">Working with Linetypes</exref>
  */
  virtual OdDbStub* linetype() const = 0;
  
  /** \details
    Returns the Plot Style Name Type for this object. 
    \returns
    plotStyleNameType() returns one of the following:
    
    <table>
    Name                                 Value                         
    OdDb::kPlotStyleNameByLayer          0
    OdDb::kPlotStyleNameByBlock          1
    OdDb::kPlotStyleNameIsDictDefault    2
    OdDb::kPlotStyleNameById             3
    </table>
  */
  virtual OdDb::PlotStyleNameType plotStyleNameType() const = 0;
  /** \details
    Retrieves the Object ID of the Plot Style Name.
    \returns
    pointer to OdDbStub that represents a plotStyleName identifier in database.
  */
  virtual OdDbStub* plotStyleNameId() const = 0;
  /** \details
    Retrieves the Object ID of the material.
    \returns
    pointer to OdDbStub that represents a material identifier in database.
    \sa
    <link tv_working_with_attributes_materials, Work with Materials>
  */
  virtual OdDbStub* materialId() const = 0;
  /** \details
    Retrieves the transparency for this object.
    
    \returns
    transparency as OdCmTransparency instance.
    \sa
    <exref target="https://docs.opendesign.com/td/cm_transparency_alpha.html">Transparency Functionality as an Alpha-value</exref>
    \sa
    <exref target="https://docs.opendesign.com/td/cm_transparency_integer.html">Transparency Functionality as an Integer-value</exref>
  */
  virtual OdCmTransparency transparency() const = 0;

  /** \details
    Sets the color for this object.
    \param color [in]  Entity color.
    \sa
    <exref target="https://docs.opendesign.com/td/cm.html">Working with Colors</exref>
  */
  virtual void setColor(const OdCmEntityColor& color) = 0;
  /** \details
    Sets the lineweight for this object.
    \param lineweight [in]  Lineweight.
    \sa
    <link tv_working_with_attributes_lineweights, Work with Lineweights>
  */
  virtual void setLineweight(OdDb::LineWeight lineweight) = 0;
  /** \details
    Sets the linetype for this object.
    \param pLinetypeId [in]  Pointer to the Object ID of the linetype.
    \sa
    <exref target="https://docs.opendesign.com/td/db_linetype.html">Working with Linetypes</exref>
  */
  virtual void setLinetype(OdDbStub* pLinetypeId) = 0;
  
  /** \details
    Sets the Plot Style Name for this object.

    \param plotStyleNameType [in]  Plot Style Name Type.
    \param pPlotStyleNameId [in]  Pointer to the Object ID of the Plot Style Name.
    
    \remarks
    pPlotStyleNameId is used only when plotStyleNameType == kPlotStyleNameById.
    
    plotStyleNameType must be one of the following:
    
    <table>
    Name                                 Value                         
    OdDb::kPlotStyleNameByLayer          0
    OdDb::kPlotStyleNameByBlock          1
    OdDb::kPlotStyleNameIsDictDefault    2
    OdDb::kPlotStyleNameById             3
    </table>
  */
  virtual void setPlotStyleName(OdDb::PlotStyleNameType plotStyleNameType, OdDbStub* pPlotStyleNameId= 0) = 0;

  /** \details
    Sets the material ID for this object.
    \param pMaterialId [in]  Pointer to the Object ID of the material.
  */
  virtual void setMaterial(OdDbStub* pMaterialId) = 0;

  /** \details
    Sets the transparency for this object.
    
    \param transparency [in] OdCmTransparency object.
    \sa
    <exref target="https://docs.opendesign.com/td/cm_transparency_alpha.html">Transparency Functionality as an Alpha-value</exref>
    \sa
    <exref target="https://docs.opendesign.com/td/cm_transparency_integer.html">Transparency Functionality as an Integer-value</exref>
  */
  virtual void setTransparency(const OdCmTransparency &transparency) = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGiLayerTraits object pointers.
*/
typedef OdSmartPtr<OdGiLayerTraits> OdGiLayerTraitsPtr;


/** \details
    This class defines the interface to linetype traits for the vectorization process.
    Library Gi
    <group OdGi_Classes> 
    \sa
    <exref target="https://docs.opendesign.com/td/db_linetype.html">Working with Linetypes</exref>
*/
class FIRSTDLL_EXPORT OdGiLinetypeTraits : public OdGiDrawableTraits
{
public:
  ODRX_DECLARE_MEMBERS(OdGiLinetypeTraits);

  // returned via subSetAttributes()
  
  enum
  {
    /** Traits inherited by block.*/
    kByBlock    = (OdGiDrawable::kLastFlag << 1),
    /** Traits inherited by layer.*/
    kByLayer    = (OdGiDrawable::kLastFlag << 2),
    /** Linetype is continuous inherited by block.*/
    kContinuous = (OdGiDrawable::kLastFlag << 3),
    /** Scale linetype to fit.*/
    kScaleToFit = (OdGiDrawable::kLastFlag << 4),
    // Values specific to .dgn files
    /** Scalability for linetype.*/
    kScalability= (OdGiDrawable::kLastFlag << 5),
    /** Dgn linetype.*/
    kDgLinetype = (OdGiDrawable::kLastFlag << 6),
    //
    kGiLinetypeFlagMax = kDgLinetype
  };

  /** \details
    Retrieves the length of the linetype pattern for this LinetypeTraits object.
    \returns
    length of the linetype pattern as a value of double type.
    \remarks
    patternLength is the total length in drawing units of all the dashes in this Linetype pattern.
  */
  virtual double patternLength() const = 0;
  
  /** \details
    Returns an array of dashes for this LinetypeTraits object.
    \param dashes [out]  Receives the array of dashes.
  */
  virtual void dashes(OdArray<OdGiLinetypeDash>& dashes) = 0;

  /** \details
    Returns the scale for this LinetypeTraits object.
  */
  virtual double scale() const = 0;
  
  /** \details
    Sets the dashes for this LinetypeTraits object.
    \param dashes [in]  Array of dashes.
  */
  virtual void setDashes(const OdArray<OdGiLinetypeDash>& dashes) = 0;
  /** \details
    Sets the scale for this LinetypeTraits object.
    \param scale [in]  Scale factor for this linetype.
  */
  virtual void setScale(double scale) = 0;
  /** \details
    Sets the length of the linetype pattern for this LinetypeTraits object.
    
    \param patternLength [in]  Pattern length.
    
    \remarks
    patternLength is the total length in drawing units of all the dashes in this Linetype pattern.
  */
  virtual void setPatternLength(double patternLength) = 0;
};
/** \details
    This template class is a specialization of the OdSmartPtr class for OdGiLinetypeTraits object pointers.
*/
typedef OdSmartPtr<OdGiLinetypeTraits> OdGiLinetypeTraitsPtr;

/** \details
    This class defines the interface to text style traits for the vectorization process.
    Corresponding C++ library: TD_Gi
    <group OdGi_Classes> 
    \sa
    <link tv_working_with_attributes_text_style, Work with Text Styles>
*/
class FIRSTDLL_EXPORT OdGiTextStyleTraits : public  OdGiDrawableTraits
{
public:
  ODRX_DECLARE_MEMBERS(OdGiTextStyleTraits);

  /** \details
    Returns the text style of this TextStyleTraits object.
    \param giTextStyle [out]  Receives the text style.
  */
  virtual void textStyle(OdGiTextStyle& giTextStyle) const = 0;

  /** \details
    Sets the text style of this TextStyleTraits object.
    \param giTextStyle [in]  Text style.
  */
  virtual void setTextStyle(const OdGiTextStyle& giTextStyle) = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGiTextStyleTraits object pointers.
*/
typedef OdSmartPtr<OdGiTextStyleTraits> OdGiTextStyleTraitsPtr;

#include "TD_PackPop.h"

#endif
