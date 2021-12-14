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
#ifndef ODA_CMBASE_COLOR
#define ODA_CMBASE_COLOR

#include "OdString.h"
#include "Gi/Gi.h"

#include "TD_PackPush.h"

//DOM-IGNORE-BEGIN
class OdDbDwgFiler;
//DOM-IGNORE-END

/** \details
  <group OdCm_Classes>

  This class implements the entity color object that represents the unnamed
  colors specified by the byLayer, byBlock, byColor, byACI, byPen, Foreground,
  byDgnIndex, or None color methods.

  \remarks
  The entity color object can specify any of the following color methods:

  <table>
  <b>Name</b>    <b>Value</b>    <b>Description</b>
  kByLayer       0xC0            Color is specified by the layer object on which the object resides.
  kByBlock       0xC1            Color is specified by the block object in which the object is contained.
  kByColor       0xC2            Color is specified by an RGB value.
  kByACI         0xC3            Color is specified by an index (ACI) of a color palette.
  kByPen         0xC4            Color is specified by an index of a pen color table.
  kForeground    0xC5            Color is the same as the foreground color.
  kByDgnIndex    0xC7            Color is specified by an index of a .dgn color table.
  kNone          0xC8            Color is absent (object is clear).
  </table>

  An entity has no color from the time it is first instantiated until it is
  assigned one or added to a database. The ColorMethod enumeration defines the
  constants for color methods.

  \sa
  <exref target="https://docs.opendesign.com/td/cm_color_sample_entity.html">Example of Working with the Entity Color</exref>

  <exref target="https://docs.opendesign.com/td/cm_overview.html">Overview of Classes that Implement Color and Transparency</exref>

  <exref target="https://docs.opendesign.com/td/cm_color_method.html">Methods of the Color Definition</exref>

  <link OdCmColorBase, OdCmColorBase class>

  <exref target="https://docs.opendesign.com/td/OdCmColor.html">OdCmColor class</exref>
*/
class FIRSTDLL_EXPORT OdCmEntityColor
{
public:
//DOM-IGNORE-BEGIN
  ODRX_HEAP_OPERATORS();
//DOM-IGNORE-END

  /** \details
    Represents the RGB color components.
  */
  enum Color
  {
    kRed,    // Red component.
    kGreen,  // Green component.
    kBlue    // Blue component.
  };

  /** \details
    Represents the color method.

    \sa
    <exref target="https://docs.opendesign.com/td/cm_color_method.html">Methods of the Color Definition</exref>
  */
  enum ColorMethod
  {
    /**Color of the layer on which it resides.*/
    kByLayer    = 0xC0,
    /**Color of the block reference in which it is contained.*/
    kByBlock    = 0xC1,
    /**Color specified by an RGB value.*/
    kByColor    = 0xC2,
    /**Color specified by an index (ACI) of a color palette.*/
    kByACI      = 0xC3,
    /**Color specified by an index of a pen color table.*/
    kByPen      = 0xC4,
    /**Editor foreground color.*/
    kForeground = 0xC5,
    /**Color specified by an index of a .dgn color table.*/
    kByDgnIndex = 0xC7,
    /**No color.*/
    kNone       = 0xC8
  };

  /** \details
    Represents color indices for the ACI color method.

    \sa
    <exref target="https://docs.opendesign.com/td/cm_color_method.html">Methods of the Color Definition</exref>

    <exref target="https://docs.opendesign.com/td/cm_color_index.html">Color Functionality as an ACI-value</exref>
  */
  enum ACIcolorMethod
  {
    kACIbyBlock     = 0,    // byBlock
    kACIforeground  = 7,    // Foreground
    kACIbyLayer     = 256,  // byLayer

    kACIclear       = 0,    // Clear

    kACIRed         = 1,    // Red
    kACIYellow      = 2,    // Yellow
    kACIGreen       = 3,    // Green
    kACICyan        = 4,    // Cyan
    kACIBlue        = 5,    // Blue
    kACIMagenta     = 6,    // Magenta
    kACIWhite       = 7,    // White

    kACIstandard    = 7,    // Standard color
    kACImaximum     = 255,  // Maximum index value
    kACInone        = 257,  // None
    kACIminimum     = -255  // Minimum index value
  };

  /** \details
    Represents the combined value of the color and color method.

    \sa
    <exref target="https://docs.opendesign.com/td/cm_color_method.html">Methods of the Color Definition</exref>
  */
  typedef OdUInt32 RGBM;

  /** \details
    Creates an entity color object with default properties.

    \remarks
    Default ColorMethod is kByColor.
  */
  OdCmEntityColor() : m_RGBM(0) { setColorMethod(kByColor); }

  /** \details
    Creates a copy of an existing entity color object.

    \param color [in]  The entity color object to be copied.
  */
  OdCmEntityColor(
    const OdCmEntityColor & color) : m_RGBM(color.m_RGBM) { }

  /** \details
    Creates an entity color object and sets the specified color method for the
    newly created entity color object.

    \param colorMethod [in]  Color method.

    \sa
    <exref target="https://docs.opendesign.com/td/cm_color_method.html">Methods of the Color Definition</exref>
  */
  OdCmEntityColor(
    ColorMethod colorMethod) : m_RGBM(0) { setColorMethod(colorMethod); }

  /** \details
    Creates an entity color object and sets the red, green and blue color
    components and the color method to byColor for the newly created entity
    color object.

    \param red   [in] Red component.
    \param green [in] Green component.
    \param blue  [in] Blue component.

    \sa
    <exref target="https://docs.opendesign.com/td/cm_color_RGB.html">Color Functionality as an RGB-value</exref>
  */
  OdCmEntityColor(
    OdUInt8 red,
    OdUInt8 green,
    OdUInt8 blue)
  {
    setColorMethod(kByColor);
    setRGB(red, green, blue);
  }

  /** \details
    Copies an entity color object specified on the right of the operator to the entity color
    object specified on the left.

    \sa
    <exref target="https://docs.opendesign.com/td/cm_color_sample_entity.html">Example of Working with the Entity Color</exref>
  */
  OdCmEntityColor& operator =(
    const OdCmEntityColor& color)
  {
    m_RGBM = color.m_RGBM;
    return *this;
  }

  /** \details
    Compares two entity color objects as integer values, and returns true when
    their values are equal or false when their values are not equal.

    \sa
    <exref target="https://docs.opendesign.com/td/cm_color_sample_entity.html">Example of Working with the Entity Color</exref>

    <exref target="https://docs.opendesign.com/td/cm_color_integer.html">Color Functionality as an Integer-value</exref>
  */
  bool operator ==(
    const OdCmEntityColor& color) const
  {
    return m_RGBM == color.m_RGBM;
  }

  /** \details
    Compares two entity color objects as integer values, and returns true when
    their values are not equal or false when their values are equal.

    \sa
    <exref target="https://docs.opendesign.com/td/cm_color_sample_entity.html">Example of Working with the Entity Color</exref>

    <exref target="https://docs.opendesign.com/td/cm_color_integer.html">Color Functionality as an Integer-value</exref>
  */
  bool operator !=(
    const OdCmEntityColor& color) const
  {
    return m_RGBM != color.m_RGBM;
  }

  /** \details
    Sets the color method for the entity color object.

    \param colorMethod [in]  color method as the ColorMethod enumeration.

    \remarks
    The color method can be one of the following:

    <table>
    <b>Name</b>    <b>Value</b>    <b>Description</b>
    kByLayer       0xC0            Color is specified by the layer object on which the object resides.
    kByBlock       0xC1            Color is specified by the block object in which the object is contained.
    kByColor       0xC2            Color is specified by an RGB value.
    kByACI         0xC3            Color is specified by an index (ACI) of a color palette.
    kByPen         0xC4            Color is specified by an index of a pen color table.
    kForeground    0xC5            Color is the same as the foreground color.
    kByDgnIndex    0xC7            Color is specified by an index of a .dgn color table.
    kNone          0xC8            Color is absent (object is clear).
    </table>

    An entity does not have a color from the time it is first instantiated until
    it is assigned one or added to a database.

    \sa
    <exref target="https://docs.opendesign.com/td/cm_color_sample_entity.html">Example of Working with the Entity Color</exref>

    <exref target="https://docs.opendesign.com/td/cm_color_method.html">Methods of the Color Definition</exref>
  */
  void setColorMethod(
    ColorMethod colorMethod) { OdCmEntityColor::setColorMethod(&m_RGBM, colorMethod); }

  /** \details
    Returns the color method of the entity color object as a value of the ColorMethod enumeration.

    \remarks
    The color method can be one of the following:

    <table>
    <b>Name</b>    <b>Value</b>    <b>Description</b>
    kByLayer       0xC0            Color is specified by the layer object on which the object resides.
    kByBlock       0xC1            Color is specified by the block object in which the object is contained.
    kByColor       0xC2            Color is specified by an RGB value.
    kByACI         0xC3            Color is specified by an index (ACI) of a color palette.
    kByPen         0xC4            Color is specified by an index of a pen color table.
    kForeground    0xC5            Color is the same as the foreground color.
    kByDgnIndex    0xC7            Color is specified by an index of a .dgn color table.
    kNone          0xC8            Color is absent (object is clear).
    </table>

    An entity does not have a color from the time it is first instantiated until
    it is assigned one or added to a database.

    \sa
    <exref target="https://docs.opendesign.com/td/cm_color_sample_entity.html">Example of Working with the Entity Color</exref>

    <exref target="https://docs.opendesign.com/td/cm_color_method.html">Methods of the Color Definition</exref>
  */
  ColorMethod colorMethod() const
  {
    return colorMethod(&m_RGBM);
  }

  /** \details
    Sets the color method and color components for the entity color object as an
    integer value.

    \param color [in]  A packed 32-bit integer value that specifies the color
      method and color components.

    \sa
    <exref target="https://docs.opendesign.com/td/cm_color_sample_entity.html">Example of Working with the Entity Color</exref>

    <exref target="https://docs.opendesign.com/td/cm_color_method.html">Methods of the Color Definition</exref>

    <exref target="https://docs.opendesign.com/td/cm_color_integer.html">Color Functionality as an Integer-value</exref>
  */
  void setColor(
    OdUInt32 color);

  /** \details
    Returns the packed 32-bit integer value that stores the color method and
    color components of the entity color object.

    \sa
    <exref target="https://docs.opendesign.com/td/cm_color_sample_entity.html">Example of Working with the Entity Color</exref>

    <exref target="https://docs.opendesign.com/td/cm_color_method.html">Methods of the Color Definition</exref>

    <exref target="https://docs.opendesign.com/td/cm_color_integer.html">Color Functionality as an Integer-value</exref>
  */
  OdUInt32 color() const { return m_RGBM; }

  /** \details
    Sets the color index (ACI) of a color palette and sets the color method
    to byACI for the entity color object.

    \param colorIndex [in]  An integer value that is the index of the color in a palette.

    \remarks
    The color index can be one of the following:

    <table>
    <b>Name</b>       <b>Value</b>    <b>Description</b>
    kACIbyBlock       0               Sets the color method to byBlock.
    kACIRed           1               Red.
    kACIYellow        2               Yellow.
    kACIGreen         3               Green.
    kACICyan          4               Cyan.
    kACIBlue          5               Blue.
    kACIMagenta       6               Magenta.
    kACIforeground    7               Sets the color method to Foreground.
    ..                8-255           Defined by display device.
    kACIbyLayer       256             Sets the color method to byLayer.
    kACInone          257             Sets the color method to None.
    </table>

    An entity does not have a color from the time it is first instantiated until
    it is assigned one or added to a database.

    \sa
    <exref target="https://docs.opendesign.com/td/cm_color_sample_entity.html">Example of Working with the Entity Color</exref>

    <exref target="https://docs.opendesign.com/td/cm_color_method.html">Methods of the Color Definition</exref>

    <exref target="https://docs.opendesign.com/td/cm_color_index.html">Color Functionality as an ACI-value</exref>
  */
  void setColorIndex(
    OdInt16 colorIndex) { setColorIndex(&m_RGBM, colorIndex); }

  /** \details
    Returns the color index (ACI) of the entity color object.

    \remarks
    The color index can be one of the following:

    <table>
    <b>Name</b>       <b>Value</b>    <b>Description</b>
    kACIbyBlock       0               Sets the color method to byBlock.
    kACIRed           1               Red.
    kACIYellow        2               Yellow.
    kACIGreen         3               Green.
    kACICyan          4               Cyan.
    kACIBlue          5               Blue.
    kACIMagenta       6               Magenta.
    kACIforeground    7               Sets the color method to Foreground.
    ..                8-255           Defined by display device.
    kACIbyLayer       256             Sets the color method to byLayer.
    kACInone          257             Sets the color method to None.
    </table>

    An entity does not have a color from the time it is first instantiated until
    it is assigned one or added to a database.

    \sa
    <exref target="https://docs.opendesign.com/td/cm_color_sample_entity.html">Example of Working with the Entity Color</exref>

    <exref target="https://docs.opendesign.com/td/cm_color_method.html">Methods of the Color Definition</exref>

    <exref target="https://docs.opendesign.com/td/cm_color_index.html">Color Functionality as an ACI-value</exref>
  */
  OdInt16 colorIndex() const { return OdCmEntityColor::colorIndex(&m_RGBM); }

  /** \details
    Sets the red, green, and blue color components and the color method to byColor
    for the entity color object.

    \param red   [in] Red component as an integer value in the range from 0 to 255.
    \param green [in] Green component as an integer value in the range from 0 to 255.
    \param blue  [in] Blue component as an integer value in the range from 0 to 255.

    \sa
    <exref target="https://docs.opendesign.com/td/cm_color_sample_entity.html">Example of Working with the Entity Color</exref>

    <exref target="https://docs.opendesign.com/td/cm_color_method.html">Methods of the Color Definition</exref>

    <exref target="https://docs.opendesign.com/td/cm_color_RGB.html">Color Functionality as an RGB-value</exref>
  */
  void setRGB(
    OdUInt8 red,
    OdUInt8 green,
    OdUInt8 blue)
  {
    setRGB(&m_RGBM, red, green, blue);
  }

  /** \details
    Sets the red color component for the entity color object.

    \param red [in] Red component as an integer value in the range from 0 to 255.

    \sa
    <exref target="https://docs.opendesign.com/td/cm_color_sample_entity.html">Example of Working with the Entity Color</exref>

    <exref target="https://docs.opendesign.com/td/cm_color_method.html">Methods of the Color Definition</exref>

    <exref target="https://docs.opendesign.com/td/cm_color_RGB.html">Color Functionality as an RGB-value</exref>
  */
  void setRed(
    OdUInt8 red) { setRed(&m_RGBM, red); }

  /** \details
    Sets the green color component for the entity color object.

    \param green [in] Green component as an integer value in the range from 0 to 255.

    \sa
    <exref target="https://docs.opendesign.com/td/cm_color_sample_entity.html">Example of Working with the Entity Color</exref>

    <exref target="https://docs.opendesign.com/td/cm_color_method.html">Methods of the Color Definition</exref>

    <exref target="https://docs.opendesign.com/td/cm_color_RGB.html">Color Functionality as an RGB-value</exref>
  */
  void setGreen(
    OdUInt8 green) { setGreen(&m_RGBM, green); }

  /** \details
    Sets the blue color component for the entity color object.

    \param blue [in] Blue component as an integer value in the range from 0 to 255.

    \sa
    <exref target="https://docs.opendesign.com/td/cm_color_sample_entity.html">Example of Working with the Entity Color</exref>

    <exref target="https://docs.opendesign.com/td/cm_color_method.html">Methods of the Color Definition</exref>

    <exref target="https://docs.opendesign.com/td/cm_color_RGB.html">Color Functionality as an RGB-value</exref>
  */
  void setBlue(
    OdUInt8 blue) { setBlue(&m_RGBM, blue); }

  /** \details
    Returns the red color component of the entity color object as a value in the
    range from 0 to 255.

    \sa
    <exref target="https://docs.opendesign.com/td/cm_color_sample_entity.html">Example of Working with the Entity Color</exref>

    <exref target="https://docs.opendesign.com/td/cm_color_RGB.html">Color Functionality as an RGB-value</exref>
  */
  OdUInt8 red() const { return red(&m_RGBM); }

  /** \details
    Returns the green color component of the entity color object as a value in
    the range from 0 to 255.

    \sa
    <exref target="https://docs.opendesign.com/td/cm_color_sample_entity.html">Example of Working with the Entity Color</exref>

    <exref target="https://docs.opendesign.com/td/cm_color_RGB.html">Color Functionality as an RGB-value</exref>
  */
  OdUInt8 green() const { return green(&m_RGBM); }

  /** \details
    Returns the blue color component of the entity color object as a value in
    the range from 0 to 255.

    \sa
    <exref target="https://docs.opendesign.com/td/cm_color_sample_entity.html">Example of Working with the Entity Color</exref>

    <exref target="https://docs.opendesign.com/td/cm_color_RGB.html">Color Functionality as an RGB-value</exref>
  */
  OdUInt8 blue() const { return blue(&m_RGBM); }

  // Method check

  /** \details
    Checks whether the color method is byColor for the entity color object and
    returns true if and only if the color method is set to kByColor, otherwise
    it returns false.

    \sa
    <exref target="https://docs.opendesign.com/td/cm_color_sample_entity.html">Example of Working with the Entity Color</exref>

    <exref target="https://docs.opendesign.com/td/cm_color_method.html">Methods of the Color Definition</exref>
  */
  bool isByColor() const { return isByColor(&m_RGBM); }

  /** \details
    Checks whether the color method is byLayer for the entity color object and
    returns true if and only if the color method is set to kByLayer or was set
    to kACIbyLayer, otherwise it returns false.

    \sa
    <exref target="https://docs.opendesign.com/td/cm_color_sample_entity.html">Example of Working with the Entity Color</exref>

    <exref target="https://docs.opendesign.com/td/cm_color_method.html">Methods of the Color Definition</exref>
  */
  bool isByLayer() const { return isByLayer(&m_RGBM); }

  /** \details
    Checks whether the color method is byBlock for the entity color object and
    returns true if and only if the color method is set to kByBlock or was set
    to kACIbyBlock, otherwise it returns false.

    \sa
    <exref target="https://docs.opendesign.com/td/cm_color_sample_entity.html">Example of Working with the Entity Color</exref>

    <exref target="https://docs.opendesign.com/td/cm_color_method.html">Methods of the Color Definition</exref>
  */
  bool isByBlock() const { return isByBlock(&m_RGBM); }

  /** \details
    Checks whether the color method is byACI for the entity color object and
    returns true if and only if the color method is set to kByACI, otherwise it
    returns false.

    \remarks
    This method returns true for ACI values of 0 (ByBlock), 7 (ByForeground),
    256 (ByLayer), and 257 (None).

    \sa
    <exref target="https://docs.opendesign.com/td/cm_color_sample_entity.html">Example of Working with the Entity Color</exref>

    <exref target="https://docs.opendesign.com/td/cm_color_method.html">Methods of the Color Definition</exref>
  */
  bool isByACI() const { return isByACI(&m_RGBM); }

  /** \details
    Checks whether the color method is Foreground for the entity color object
    and returns true if and only if the color method is set to kForeground or
    was set to kACIforeground, otherwise it returns false.

    \sa
    <exref target="https://docs.opendesign.com/td/cm_color_sample_entity.html">Example of Working with the Entity Color</exref>

    <exref target="https://docs.opendesign.com/td/cm_color_method.html">Methods of the Color Definition</exref>
  */
  bool isForeground() const { return isForeground(&m_RGBM); }

  /** \details
    Checks whether the color method is byDgnIndex for the entity color object
    and returns true if and only if the color method is set to kByDgnIndex,
    otherwise it returns false.

    \sa
    <exref target="https://docs.opendesign.com/td/cm_color_sample_entity.html">Example of Working with the Entity Color</exref>

    <exref target="https://docs.opendesign.com/td/cm_color_method.html">Methods of the Color Definition</exref>
  */
  bool isByDgnIndex() const { return isByDgnIndex(&m_RGBM); }

  /** \details
    Checks whether the color method is None (invisible) for the entity color
    object.

    \returns
    True if and only if the color method is set to kNone or kACInone, otherwise
    returns false.

    \sa
    <exref target="https://docs.opendesign.com/td/cm_color_sample_entity.html">Example of Working with the Entity Color</exref>

    <exref target="https://docs.opendesign.com/td/cm_color_method.html">Methods of the Color Definition</exref>
  */
  bool isNone() const  { return isNone(&m_RGBM); }
  
  /** \details
    Returns the TrueColor value of the entity color object.

    \sa
    <exref target="https://docs.opendesign.com/td/cm.html">Working with Colors and Transparency</exref>
  */
  OdUInt32 trueColor() const;

  /** \details
    Sets the color of the calling entity with the entity color object.

    \sa
    <exref target="https://docs.opendesign.com/td/cm.html">Working with Colors and Transparency</exref>
  */
  void setTrueColor();

  /** \details
    Converts the color method from ACIcolorMethod to ColorMethod and returns the
    ColorMethod value.

    \remarks
    Assumes the current color method is ACIcolorMethod.

    \sa
    <exref target="https://docs.opendesign.com/td/cm_color_index.html">Color Functionality as an ACI-value</exref>
  */
  OdUInt8 trueColorMethod() const;

  /** \details
    Converts the color method from ACIcolorMethod to ColorMethod and
    assumes the current color method is ACIcolorMethod.

    \sa
    <exref target="https://docs.opendesign.com/td/cm_color_index.html">Color Functionality as an ACI-value</exref>
  */
  void setTrueColorMethod();

  /** \details
    Sets the color method for the entity color object.

    \param pRGBM [in]  Pointer to the m_RGBM member of the entity color object.
    \param colorMethod [in]  color method as the ColorMethod enumeration.

    \remarks
    The color method can be one of the following:

    <table>
    <b>Name</b>    <b>Value</b>    <b>Description</b>
    kByLayer       0xC0            Color is specified by the layer object on which the object resides.
    kByBlock       0xC1            Color is specified by the block object in which the object is contained.
    kByColor       0xC2            Color is specified by an RGB value.
    kByACI         0xC3            Color is specified by an index (ACI) of a color palette.
    kByPen         0xC4            Color is specified by an index of a pen color table.
    kForeground    0xC5            Color is the same as the foreground color.
    kByDgnIndex    0xC7            Color is specified by an index of a .dgn color table.
    kNone          0xC8            Color is absent (object is clear).
    </table>

    An entity does not have a color from the time it is first instantiated until
    it is assigned one or added to a database.

    \sa
    <exref target="https://docs.opendesign.com/td/cm_color_sample_entity.html">Example of Working with the Entity Color</exref>

    <exref target="https://docs.opendesign.com/td/cm_color_method.html">Methods of the Color Definition</exref>
  */
  static void setColorMethod(
    RGBM* pRGBM, 
    ColorMethod colorMethod);

  /** \details
    Returns the color method of the entity color object as a value of the ColorMethod enumeration.

    \param pRGBM [in]  Pointer to the m_RGBM member of the entity color object.

    \remarks
    The color method can be one of the following:

    <table>
    <b>Name</b>    <b>Value</b>    <b>Description</b>
    kByLayer       0xC0            Color is specified by the layer object on which the object resides.
    kByBlock       0xC1            Color is specified by the block object in which the object is contained.
    kByColor       0xC2            Color is specified by an RGB value.
    kByACI         0xC3            Color is specified by an index (ACI) of a color palette.
    kByPen         0xC4            Color is specified by an index of a pen color table.
    kForeground    0xC5            Color is the same as the foreground color.
    kByDgnIndex    0xC7            Color is specified by an index of a .dgn color table.
    kNone          0xC8            Color is absent (object is clear).
    </table>

    An entity does not have a color from the time it is first instantiated until
    it is assigned one or added to a database.

    \sa
    <exref target="https://docs.opendesign.com/td/cm_color_sample_entity.html">Example of Working with the Entity Color</exref>

    <exref target="https://docs.opendesign.com/td/cm_color_method.html">Methods of the Color Definition</exref>
  */
  static ColorMethod colorMethod(
    const RGBM* pRGBM)
  {
    return OdCmEntityColor::ColorMethod((*pRGBM >> 24) & 0xFF);
  }

  /** \details
    Sets the color method and color components for the entity color object as an
    integer value.

    \param pRGBM [in]  Pointer to the m_RGBM member of the entity color object.
    \param color [in]  A packed 32-bit integer value that specifies the color method and color components.

    \sa
    <exref target="https://docs.opendesign.com/td/cm_color_sample_entity.html">Example of Working with the Entity Color</exref>

    <exref target="https://docs.opendesign.com/td/cm_color_method.html">Methods of the Color Definition</exref>

    <exref target="https://docs.opendesign.com/td/cm_color_integer.html">Color Functionality as an Integer-value</exref>
  */
  static void setColor(
    RGBM* pRGBM,
    OdUInt32 color);

  /** \details
    Returns the packed 32-bit integer value that stores the color method and
    color components of the entity color object.

    \param pRGBM [in]  Pointer to the m_RGBM member of the entity color object.

    \sa
    <exref target="https://docs.opendesign.com/td/cm_color_sample_entity.html">Example of Working with the Entity Color</exref>

    <exref target="https://docs.opendesign.com/td/cm_color_method.html">Methods of the Color Definition</exref>

    <exref target="https://docs.opendesign.com/td/cm_color_integer.html">Color Functionality as an Integer-value</exref>
  */
  static OdUInt32 color(
    const RGBM* pRGBM);

  /** \details
    Sets the color index (ACI) of a color palette and sets the color method
    to byACI for the entity color object.

    \param pRGBM [in]  Pointer to the m_RGBM member of the entity color object.
    \param colorIndex [in]  An integer value that is the index of the color in a palette.

    \remarks
    The color index can be one of the following:

    <table>
    <b>Name</b>       <b>Value</b>    <b>Description</b>
    kACIbyBlock       0               Sets the color method to byBlock.
    kACIRed           1               Red.
    kACIYellow        2               Yellow.
    kACIGreen         3               Green.
    kACICyan          4               Cyan.
    kACIBlue          5               Blue.
    kACIMagenta       6               Magenta.
    kACIforeground    7               Sets the color method to Foreground.
    ..                8-255           Defined by display device.
    kACIbyLayer       256             Sets the color method to byLayer.
    kACInone          257             Sets the color method to None.
    </table>

    An entity does not have a color from the time it is first instantiated until
    it is assigned one or added to a database.

    \sa
    <exref target="https://docs.opendesign.com/td/cm_color_sample_entity.html">Example of Working with the Entity Color</exref>

    <exref target="https://docs.opendesign.com/td/cm_color_method.html">Methods of the Color Definition</exref>

    <exref target="https://docs.opendesign.com/td/cm_color_index.html">Color Functionality as an ACI-value</exref>
  */
  static void setColorIndex(
    RGBM* pRGBM,
    OdInt16 colorIndex);

    /** \details
    Sets the .dgn color index (DgnIndex) of a color palette and sets the
    color method to byDgnIndex for the entity color object.

    \param pRGBM [in] Pointer to the m_RGBM member of the entity color object.
    \param colorIndex [in] An integer value that is the index of the color in a palette.
  */
  static void setDgnColorIndex(
    RGBM* pRGBM,
    OdInt16 colorIndex);

  /** \details
    Returns the color index (ACI) of the entity color object.

    \param pRGBM [in]  Pointer to the m_RGBM member of the entity color object.

    \remarks
    The color index can be one of the following:

    <table>
    <b>Name</b>       <b>Value</b>    <b>Description</b>
    kACIbyBlock       0               Sets the color method to byBlock.
    kACIRed           1               Red.
    kACIYellow        2               Yellow.
    kACIGreen         3               Green.
    kACICyan          4               Cyan.
    kACIBlue          5               Blue.
    kACIMagenta       6               Magenta.
    kACIforeground    7               Sets the color method to Foreground.
    ..                8-255           Defined by display device.
    kACIbyLayer       256             Sets the color method to byLayer.
    kACInone          257             Sets the color method to None.
    </table>

    An entity does not have a color from the time it is first instantiated until
    it is assigned one or added to a database.

    \sa
    <exref target="https://docs.opendesign.com/td/cm_color_sample_entity.html">Example of Working with the Entity Color</exref>

    <exref target="https://docs.opendesign.com/td/cm_color_method.html">Methods of the Color Definition</exref>

    <exref target="https://docs.opendesign.com/td/cm_color_index.html">Color Functionality as an ACI-value</exref>
  */
  static OdInt16 colorIndex(
    const RGBM* pRGBM);

  /** \details
    Sets the red, green, and blue color components and the color method to byColor
    for the entity color object.

    \param pRGBM [in] Pointer to the m_RGBM member of the entity color object.
    \param red   [in] Red component as an integer value in the range from 0 to 255.
    \param green [in] Green component as an integer value in the range from 0 to 255.
    \param blue  [in] Blue component as an integer value in the range from 0 to 255.

    \sa
    <exref target="https://docs.opendesign.com/td/cm_color_sample_entity.html">Example of Working with the Entity Color</exref>

    <exref target="https://docs.opendesign.com/td/cm_color_method.html">Methods of the Color Definition</exref>

    <exref target="https://docs.opendesign.com/td/cm_color_RGB.html">Color Functionality as an RGB-value</exref>
  */
  static void setRGB(
    RGBM* pRGBM,
    OdUInt8 red,
    OdUInt8 green,
    OdUInt8 blue)
  {
    setColorMethod(pRGBM, kByColor);
    setRed(pRGBM, red);
    setGreen(pRGBM, green);
    setBlue(pRGBM, blue);
  }

  /** \details
    Sets the red color component for the entity color object.

    \param pRGBM [in] Pointer to the m_RGBM member of the entity color object.
    \param red   [in] Red component as an integer value in the range from 0 to 255.

    \sa
    <exref target="https://docs.opendesign.com/td/cm_color_sample_entity.html">Example of Working with the Entity Color</exref>

    <exref target="https://docs.opendesign.com/td/cm_color_method.html">Methods of the Color Definition</exref>

    <exref target="https://docs.opendesign.com/td/cm_color_RGB.html">Color Functionality as an RGB-value</exref>
  */
  static void setRed(
    RGBM* pRGBM,
    OdUInt8 red)
  {
    ODA_ASSERT(colorMethod(pRGBM) == kByColor);
    *pRGBM &= 0xFF00FFFF;
    *pRGBM |= RGBM(((OdUInt32)red << 16) & 0x00FF0000);
  }

  /** \details
    Sets the green color component for the entity color object.

    \param pRGBM [in]  Pointer to the m_RGBM member of the entity color object.
    \param green [in]  Green component as an integer value in the range from 0 to 255.

    \sa
    <exref target="https://docs.opendesign.com/td/cm_color_sample_entity.html">Example of Working with the Entity Color</exref>

    <exref target="https://docs.opendesign.com/td/cm_color_method.html">Methods of the Color Definition</exref>

    <exref target="https://docs.opendesign.com/td/cm_color_RGB.html">Color Functionality as an RGB-value</exref>
  */
  static void setGreen(
    RGBM* pRGBM,
    OdUInt8 green)
  {
    ODA_ASSERT(colorMethod(pRGBM) == kByColor);
    *pRGBM &= 0xFFFF00FF;
    *pRGBM |= RGBM(((OdUInt32)green << 8) & 0x0000FF00);
  }

  /** \details
    Sets the blue color component for the entity color object.

    \param pRGBM [in]  Pointer to the m_RGBM member of the entity color object.
    \param blue  [in]  Blue component as an integer value in the range from 0 to 255.

    \sa
    <exref target="https://docs.opendesign.com/td/cm_color_sample_entity.html">Example of Working with the Entity Color</exref>

    <exref target="https://docs.opendesign.com/td/cm_color_method.html">Methods of the Color Definition</exref>

    <exref target="https://docs.opendesign.com/td/cm_color_RGB.html">Color Functionality as an RGB-value</exref>
  */
  static void setBlue(
    RGBM* pRGBM,
    OdUInt8 blue)
  {
    ODA_ASSERT(colorMethod(pRGBM) == kByColor);
    *pRGBM &= 0xFFFFFF00;
    *pRGBM |= RGBM((blue) & 0x000000FF);
  }

  /** \details
    Returns the red color component of the entity color object as a value in the
    range from 0 to 255.

    \param pRGBM [in]  Pointer to the m_RGBM member of the entity color object.

    \sa
    <exref target="https://docs.opendesign.com/td/cm_color_sample_entity.html">Example of Working with the Entity Color</exref>

    <exref target="https://docs.opendesign.com/td/cm_color_RGB.html">Color Functionality as an RGB-value</exref>
  */
  static OdUInt8 red(
    const RGBM* pRGBM)
  {
    ODA_ASSERT_ONCE(colorMethod(pRGBM) == kByColor);
    return OdUInt8((*pRGBM >> 16) & 0xFF);
  }

  /** \details
    Returns the green color component of the entity color object as a value in
    the range from 0 to 255.

    \param pRGBM [in]  Pointer to the m_RGBM member of the entity color object.

    \sa
    <exref target="https://docs.opendesign.com/td/cm_color_sample_entity.html">Example of Working with the Entity Color</exref>

    <exref target="https://docs.opendesign.com/td/cm_color_RGB.html">Color Functionality as an RGB-value</exref>
  */
  static OdUInt8 green(
    const RGBM* pRGBM)
  {
    ODA_ASSERT_ONCE(colorMethod(pRGBM) == kByColor);
    return OdUInt8((*pRGBM >> 8) & 0xFF);
  }

  /** \details
    Returns the blue color component of the entity color object as a value in
    the range from 0 to 255.

    \param pRGBM [in]  Pointer to the m_RGBM member of the entity color object.

    \sa
    <exref target="https://docs.opendesign.com/td/cm_color_sample_entity.html">Example of Working with the Entity Color</exref>

    <exref target="https://docs.opendesign.com/td/cm_color_RGB.html">Color Functionality as an RGB-value</exref>
  */
  static OdUInt8 blue(
    const RGBM* pRGBM)
  {
    ODA_ASSERT_ONCE(colorMethod(pRGBM) == kByColor);
    return OdUInt8(*pRGBM & 0xFF);
  }

  /** \details
    Checks whether the color method is byColor for the entity color object and
    returns true if and only if the color method is set to kByColor, otherwise
    it returns false.

    \param pRGBM [in]  Pointer to the m_RGBM member of the entity color object.

    \sa
    <exref target="https://docs.opendesign.com/td/cm_color_sample_entity.html">Example of Working with the Entity Color</exref>

    <exref target="https://docs.opendesign.com/td/cm_color_method.html">Methods of the Color Definition</exref>
  */
  static bool isByColor(
    const RGBM* pRGBM) { return colorMethod(pRGBM) == kByColor; }

  /** \details
    Checks whether the color method is byLayer for the entity color object and
    returns true if and only if the color method is set to kByLayer or
    kACIbyLayer, otherwise it returns false.

    \param pRGBM [in]  Pointer to the m_RGBM member of the entity color object.

    \sa
    <exref target="https://docs.opendesign.com/td/cm_color_sample_entity.html">Example of Working with the Entity Color</exref>

    <exref target="https://docs.opendesign.com/td/cm_color_method.html">Methods of the Color Definition</exref>
  */
  static bool isByLayer(
    const RGBM* pRGBM)
  {
    return (colorMethod(pRGBM) == kByLayer || (colorMethod(pRGBM) == kByACI && indirect(pRGBM) == kACIbyLayer));
  }

  /** \details
    Checks whether the color method is byBlock for the entity color object and
    returns true if and only if the color method is set to kByBlock, otherwise
    it returns false.

    \param pRGBM [in] Pointer to the m_RGBM member of the entity color object.

    \sa
    <exref target="https://docs.opendesign.com/td/cm_color_sample_entity.html">Example of Working with the Entity Color</exref>

    <exref target="https://docs.opendesign.com/td/cm_color_method.html">Methods of the Color Definition</exref>
  */
  static bool isByBlock(
    const RGBM* pRGBM)
  {
    return (colorMethod(pRGBM) == kByBlock || (colorMethod(pRGBM) == kByACI && indirect(pRGBM) == kACIbyBlock));
  }

  /** \details
    Checks whether the color method is byACI for the entity color object and
    returns true if and only if the color method is set to kByACI, otherwise it
    returns false.

    \param pRGBM [in] Pointer to the m_RGBM member of the entity color object.

    \remarks
    This method returns true for ACI values of 0 (ByBlock), 7 (ByForeground),
    256 (ByLayer), and 257 (None).

    \sa
    <exref target="https://docs.opendesign.com/td/cm_color_sample_entity.html">Example of Working with the Entity Color</exref>

    <exref target="https://docs.opendesign.com/td/cm_color_method.html">Methods of the Color Definition</exref>
  */
  static bool isByACI(
    const RGBM* pRGBM)
  {
    return colorMethod(pRGBM) == kByACI;
  }

  /** \details
    Checks whether the color method is Foreground for the entity color object
    and returns true if and only if the color method is set to kForeground or
    was set to kACIforeground, otherwise it returns false.

    \param pRGBM [in] Pointer to the m_RGBM member of the entity color object.

    \sa
    <exref target="https://docs.opendesign.com/td/cm_color_sample_entity.html">Example of Working with the Entity Color</exref>

    <exref target="https://docs.opendesign.com/td/cm_color_method.html">Methods of the Color Definition</exref>
  */
  static bool isForeground(
    const RGBM* pRGBM)
  {
    return (colorMethod(pRGBM) == kForeground || (colorMethod(pRGBM) == kByACI && indirect(pRGBM) == kACIforeground));
  }

  /** \details
    Checks whether the color method is byDgnIndex for the entity color object
    and returns true if and only if the color method is set to kByDgnIndex,
    otherwise it returns false.

    \param pRGBM [in] Pointer to the m_RGBM member of the entity color object.

    \sa
    <exref target="https://docs.opendesign.com/td/cm_color_sample_entity.html">Example of Working with the Entity Color</exref>

    <exref target="https://docs.opendesign.com/td/cm_color_method.html">Methods of the Color Definition</exref>
  */
  static bool isByDgnIndex(
    const RGBM* pRGBM)
  {
    return colorMethod(pRGBM) == kByDgnIndex;
  }

  /** \details
    Checks whether the color method is None (invisible) for the entity color
    object and returns true if and only if the color method is set to kNone or
    was set to kACInone, otherwise it returns false.

    \param pRGBM [in]  Pointer to the m_RGBM member of the entity color object.

    \sa
    <exref target="https://docs.opendesign.com/td/cm_color_sample_entity.html">Example of Working with the Entity Color</exref>

    <exref target="https://docs.opendesign.com/td/cm_color_method.html">Methods of the Color Definition</exref>
  */
  static bool isNone(
    const RGBM* pRGBM)
  {
    return (colorMethod(pRGBM) == kNone || (colorMethod(pRGBM) == kByACI && indirect(pRGBM) == kACInone));
  }

  /** \details
    Returns the TrueColor value of the entity color object.

    \param pRGBM [in]  Pointer to the m_RGBM member of the entity color object.

    \sa
    <exref target="https://docs.opendesign.com/td/cm.html">Working with Colors and Transparency</exref>
  */
  static OdUInt32 trueColor(
    const RGBM* pRGBM);

  /** \details
    Converts the color method from ACIcolorMethod to ColorMethod and returns the ColorMethod value.

    \param aciColorMethod [in]  ACIcolorMethod value.

    \remarks
    Assumes the current color method is ACIcolorMethod.

    \sa
    <exref target="https://docs.opendesign.com/td/cm_color_index.html">Color Functionality as an ACI-value</exref>
  */
  static OdUInt8 trueColorMethod(
    OdInt16 aciColorMethod);

  /** \details
    Sets the color of the calling entity with the entity color object.

    \param pRGBM [in]  Pointer to the m_RGBM member of the entity color object.

    \sa
    <exref target="https://docs.opendesign.com/td/cm.html">Working with Colors and Transparency</exref>
  */
  static void setTrueColor(
    RGBM* pRGBM);

  /** \details
    Converts the color method from ACIcolorMethod to ColorMethod and assumes the
    current color method is ACIcolorMethod.

    \param pRGBM [in]  Pointer to the m_RGBM member of the entity color object.

    \sa
    <exref target="https://docs.opendesign.com/td/cm_color_index.html">Color Functionality as an ACI-value</exref>
  */
  static void setTrueColorMethod(
    RGBM* pRGBM);

  /** \details
    Converts the specified color index (ACI value) to an RGB value.

    \remarks
    For converting, this method uses the default palette specified by the mLUT static array
    and returns the RGB value without color method as a packed integer value in which:

    <table>
    <b>Byte</b>    <b>Description</b>
    0              Blue component.
    1              Green component.
    2              Red component.
    </table>

    \sa
    <exref target="https://docs.opendesign.com/td/cm_color_sample_entity.html">Example of Working with the Entity Color</exref>

    <exref target="https://docs.opendesign.com/td/cm_color_RGB.html">Color Functionality as an RGB-value</exref>

    <exref target="https://docs.opendesign.com/td/cm_color_index.html">Color Functionality as an ACI-value</exref>
  */
  static OdUInt32 lookUpRGB(
    OdUInt8 colorIndex);

  /** \details
    Converts the RGB value specified by the red, green, and blue components to an
    ACI value.

    \param red   [in]  Red component an an integer value in the range from 0 to 255.
    \param green [in]  Green component an an integer value in the range from 0 to 255.
    \param blue  [in]  Blue component an an integer value in the range from 0 to 255.

    \remarks
    For converting, this method uses the default palette specified by the mLUT
    static array and returns the color index without color method as an integer
    value in the range from 0 to 257.

    \sa
    <exref target="https://docs.opendesign.com/td/cm_color_sample_entity.html">Example of Working with the Entity Color</exref>

    <exref target="https://docs.opendesign.com/td/cm_color_RGB.html">Color Functionality as an RGB-value</exref>

    <exref target="https://docs.opendesign.com/td/cm_color_index.html">Color Functionality as an ACI-value</exref>
  */
  static OdUInt8 lookUpACI(
    OdUInt8 red,
    OdUInt8 green,
    OdUInt8 blue);

protected:
//DOM-IGNORE-BEGIN
  static OdInt16 indirect(
    const RGBM* pRGBM) 
  {
    ODA_ASSERT(colorMethod(pRGBM) != kByColor);
    return OdInt16((*pRGBM) & 0x0000FFFF);
  }

  static void setIndirect(
    RGBM* pRGBM, 
    OdInt16 indirect)
  {
    ODA_ASSERT(colorMethod(pRGBM) != kByColor); 
    *pRGBM = ((*pRGBM & 0xFF000000) | (OdUInt32(indirect) & 0x0000FFFF));
  }

  OdInt16 indirect() const { return indirect(&m_RGBM); }

  void setIndirect(OdInt16 indirect) { setIndirect(&m_RGBM, indirect); }
//DOM-IGNORE-END

  /** \details
    The combined value of the color method and color components.

    \sa
    <exref target="https://docs.opendesign.com/td/cm_color_method.html">Methods of the Color Definition</exref>
  */
  RGBM m_RGBM;

public:

  /** \details
    Maps the colors between ACI values and RGB values.
  */
  FIRSTDLL_EXPORT_STATIC static const OdUInt8 mLUT[256][3];
};

/** <group OdCm_Classes>
  \details
  This class is a virtual interface for classes that represent the unnamed and
  named colors specified by the byLayer, byBlock, byColor, byACI, byPen,
  Foreground, byDgnIndex, or None color methods.

  \sa
  <exref target="https://docs.opendesign.com/td/cm_color_sample_base.html">Example of Working with the Database Color</exref>

  <exref target="https://docs.opendesign.com/td/cm_overview.html">Overview of Classes that Implement Color and Transparency</exref>

  <link OdCmEntityColor, OdCmEntityColor class>

  <exref target="https://docs.opendesign.com/td/OdCmColor.html">OdCmColor class</exref>
*/
class FIRSTDLL_EXPORT OdCmColorBase
{
public:
//DOM-IGNORE-BEGIN
  ODRX_HEAP_OPERATORS();
  virtual ~OdCmColorBase() {}
//DOM-IGNORE-END

  /** \details
    Returns the color method of the database color object as a value of the
    ColorMethod enumeration.

    \remarks
    The color method can be one of the following:

    <table>
    <b>Name</b>    <b>Value</b>    <b>Description</b>
    kByLayer       0xC0            Color is specified by the layer object on which the object resides.
    kByBlock       0xC1            Color is specified by the block object in which the object is contained.
    kByColor       0xC2            Color is specified by an RGB value.
    kByACI         0xC3            Color is specified by an index (ACI) of a color palette.
    kByPen         0xC4            Color is specified by an index of a pen color table.
    kForeground    0xC5            Color is the same as the foreground color.
    kByDgnIndex    0xC7            Color is specified by an index of a .dgn color table.
    kNone          0xC8            Color is absent (object is clear).
    </table>

    Use of this method by third-party applications is neither supported nor
    recommended.

    \sa
    <exref target="https://docs.opendesign.com/td/cm_color_sample_base.html">Example of Working with the Database Color</exref>

    <exref target="https://docs.opendesign.com/td/cm_color_method.html">Methods of the Color Definition</exref>
  */
  virtual OdCmEntityColor::ColorMethod colorMethod() const = 0;

  /** \details
    Sets the color method for the database color object as a value of the
    ColorMethod enumeration.

    \param colorMethod [in]  color method as the ColorMethod enumeration.

    \remarks
    The color method can be one of the following:

    <table>
    <b>Name</b>    <b>Value</b>    <b>Description</b>
    kByLayer       0xC0            Color is specified by the layer object on which the object resides.
    kByBlock       0xC1            Color is specified by the block object in which the object is contained.
    kByColor       0xC2            Color is specified by an RGB value.
    kByACI         0xC3            Color is specified by an index (ACI) of a color palette.
    kByPen         0xC4            Color is specified by an index of a pen color table.
    kForeground    0xC5            Color is the same as the foreground color.
    kByDgnIndex    0xC7            Color is specified by an index of a .dgn color table.
    kNone          0xC8            Color is absent (object is clear).
    </table>

    \remarks
    Use of this method by third-party applications is neither supported nor
    recommended.

    \sa
    <exref target="https://docs.opendesign.com/td/cm_color_sample_base.html">Example of Working with the Database Color</exref>

    <exref target="https://docs.opendesign.com/td/cm_color_method.html">Methods of the Color Definition</exref>
  */
  virtual void  setColorMethod(
    OdCmEntityColor::ColorMethod colorMethod) = 0;

  /** \details
    Checks whether the color method is byColor for the database color object and
    returns true if and only if the color method is set to kByColor, otherwise
    it returns false.

    \sa
    <exref target="https://docs.opendesign.com/td/cm_color_sample_base.html">Example of Working with the Database Color</exref>

    <exref target="https://docs.opendesign.com/td/cm_color_method.html">Methods of the Color Definition</exref>
  */
  virtual bool isByColor() const = 0;

  /** \details
    Checks whether the color method is byLayer for the database color object and
    returns true if and only if the color method is set to kByLayer or was set
    to kACIbyLayer, otherwise it returns false.

    \sa
    <exref target="https://docs.opendesign.com/td/cm_color_sample_base.html">Example of Working with the Database Color</exref>

    <exref target="https://docs.opendesign.com/td/cm_color_method.html">Methods of the Color Definition</exref>
  */
  virtual bool isByLayer() const = 0;

  /** \details
    Checks whether the color method is byBlock for the database color object and
    returns true if and only if the color method is set to kByBlock or was set
    to kACIbyBlock, otherwise it returns false.

    \sa
    <exref target="https://docs.opendesign.com/td/cm_color_sample_base.html">Example of Working with the Database Color</exref>

    <exref target="https://docs.opendesign.com/td/cm_color_method.html">Methods of the Color Definition</exref>
  */
  virtual bool isByBlock() const = 0;

  /** \details
    Checks whether the color method is byACI for the database color object and
    returns true if and only if the color method is set to kByACI, otherwise it
    returns false.

    \remarks
    This method returns true for ACI values of 0 (ByBlock), 7 (ByForeground),
    256 (ByLayer), and 257 (None).

    \sa
    <exref target="https://docs.opendesign.com/td/cm_color_sample_base.html">Example of Working with the Database Color</exref>

    <exref target="https://docs.opendesign.com/td/cm_color_method.html">Methods of the Color Definition</exref>
  */
  virtual bool isByACI() const = 0;

  /** \details
    Checks whether the color method is Foreground for the database color object
    and returns true if and only if the color method is set to kForeground or
    kACIforeground, otherwise it returns false.

    \sa
    <exref target="https://docs.opendesign.com/td/cm_color_sample_base.html">Example of Working with the Database Color</exref>

    <exref target="https://docs.opendesign.com/td/cm_color_method.html">Methods of the Color Definition</exref>
  */
  virtual bool isForeground() const = 0;

  /** \details
    Checks whether the color method is byDgnIndex for the database color object
    and returns true if and only if the color method is set to kByDgnIndex,
    otherwise it returns false.

    \sa
    <exref target="https://docs.opendesign.com/td/cm_color_sample_base.html">Example of Working with the Database Color</exref>

    <exref target="https://docs.opendesign.com/td/cm_color_method.html">Methods of the Color Definition</exref>
  */
  virtual bool isByDgnIndex() const = 0;

  /** \details
    Returns the packed 32-bit integer value that stores the color method and
    color components of the database color object.

    \sa
    <exref target="https://docs.opendesign.com/td/cm_color_sample_base.html">Example of Working with the Database Color</exref>

    <exref target="https://docs.opendesign.com/td/cm_color_method.html">Methods of the Color Definition</exref>

    <exref target="https://docs.opendesign.com/td/cm_color_integer.html">Color Functionality as an Integer-value</exref>
  */
  virtual OdUInt32  color() const = 0;

  /** \details
    Sets the color method and color components for the database color object as
    an integer value.

    \param color [in] A packed 32-bit integer value that specifies the color method and color components.

    \sa
    <exref target="https://docs.opendesign.com/td/cm_color_sample_base.html">Example of Working with the Database Color</exref>

    <exref target="https://docs.opendesign.com/td/cm_color_method.html">Methods of the Color Definition</exref>

    <exref target="https://docs.opendesign.com/td/cm_color_integer.html">Color Functionality as an Integer-value</exref>
  */
  virtual void setColor(
    OdUInt32 color) = 0;

  /** \details
    Sets the red, green, and blue color components and the color method to byColor
    for the database color object.

    \param red   [in] Red component as an integer value in the range from 0 to 255.
    \param green [in] Green component as an integer value in the range from 0 to 255.
    \param blue  [in] Blue component as an integer value in the range from 0 to 255.

    \sa
    <exref target="https://docs.opendesign.com/td/cm_color_sample_base.html">Example of Working with the Database Color</exref>

    <exref target="https://docs.opendesign.com/td/cm_color_method.html">Methods of the Color Definition</exref>

    <exref target="https://docs.opendesign.com/td/cm_color_RGB.html">Color Functionality as an RGB-value</exref>
  */
  virtual void setRGB(
    OdUInt8 red,
    OdUInt8 green,
    OdUInt8 blue) = 0;

  /** \details
    Sets the red color component for the database color object.

    \param red [in]  Red component as an integer value in the range from 0 to 255.

    \sa
    <exref target="https://docs.opendesign.com/td/cm_color_sample_base.html">Example of Working with the Database Color</exref>

    <exref target="https://docs.opendesign.com/td/cm_color_RGB.html">Color Functionality as an RGB-value</exref>
  */
  virtual void setRed(
    OdUInt8 red) = 0;

  /** \details
    Sets the green color component for the database color object.

    \param green [in] Green component as an integer value in the range from 0 to 255.

    \sa
    <exref target="https://docs.opendesign.com/td/cm_color_sample_base.html">Example of Working with the Database Color</exref>

    <exref target="https://docs.opendesign.com/td/cm_color_RGB.html">Color Functionality as an RGB-value</exref>
  */
  virtual void setGreen(
    OdUInt8 green) = 0;

  /** \details
    Sets the blue color component for the database color object.

    \param blue [in] Blue component as an integer value in the range from 0 to 255.

    \sa
    <exref target="https://docs.opendesign.com/td/cm_color_sample_base.html">Example of Working with the Database Color</exref>

    <exref target="https://docs.opendesign.com/td/cm_color_RGB.html">Color Functionality as an RGB-value</exref>
  */
  virtual void setBlue(
    OdUInt8 blue) = 0;

  /** \details
    Returns the red color component of the database color object as a value in
    the range from 0 to 255.

    \sa
    <exref target="https://docs.opendesign.com/td/cm_color_sample_base.html">Example of Working with the Database Color</exref>

    <exref target="https://docs.opendesign.com/td/cm_color_RGB.html">Color Functionality as an RGB-value</exref>
  */
  virtual OdUInt8 red() const = 0;

   /** \details
    Returns the green color component of the database color object as a value in
    the range from 0 to 255.

    \sa
    <exref target="https://docs.opendesign.com/td/cm_color_sample_base.html">Example of Working with the Database Color</exref>

    <exref target="https://docs.opendesign.com/td/cm_color_RGB.html">Color Functionality as an RGB-value</exref>
   */
  virtual OdUInt8 green() const = 0;

  /** \details
    Returns the blue color component of the database color object as a value in
    the range from 0 to 255.

    \sa
    <exref target="https://docs.opendesign.com/td/cm_color_sample_base.html">Example of Working with the Database Color</exref>

    <exref target="https://docs.opendesign.com/td/cm_color_RGB.html">Color Functionality as an RGB-value</exref>
  */
  virtual OdUInt8 blue() const = 0;

  /** \details
    Returns the color index (ACI) of the database color object.

    \remarks
    The color index can be one of the following:

    <table>
    <b>Name</b>       <b>Value</b>    <b>Description</b>
    kACIbyBlock       0               Sets the color method to byBlock.
    kACIRed           1               Red.
    kACIYellow        2               Yellow.
    kACIGreen         3               Green.
    kACICyan          4               Cyan.
    kACIBlue          5               Blue.
    kACIMagenta       6               Magenta.
    kACIforeground    7               Sets the color method to Foreground.
    ..                8-255           Defined by display device.
    kACIbyLayer       256             Sets the color method to byLayer.
    kACInone          257             Sets the color method to None.
    </table>

    \sa
    <exref target="https://docs.opendesign.com/td/cm_color_sample_base.html">Example of Working with the Database Color</exref>

    <exref target="https://docs.opendesign.com/td/cm_color_method.html">Methods of the Color Definition</exref>

    <exref target="https://docs.opendesign.com/td/cm_color_index.html">Color Functionality as an ACI-value</exref>
  */
  virtual OdUInt16  colorIndex() const = 0;

  /** \details
    Sets the color index (ACI) of a color palette and sets the color method
    to byACI for the database color object.

    \param colorIndex [in]  An integer value that is the index of the color in a palette.

    \remarks
    The color index can be a one of the following:

    <table>
    <p>Name</p>       <b>Value</b>    <b>Description</b>
    kACIbyBlock       0               Sets the color method to byBlock.
    kACIRed           1               Red.
    kACIYellow        2               Yellow.
    kACIGreen         3               Green.
    kACICyan          4               Cyan.
    kACIBlue          5               Blue.
    kACIMagenta       6               Magenta.
    kACIforeground    7               Sets the color method to Foreground.
    ..                8-255           Defined by display device.
    kACIbyLayer       256             Sets the color method to byLayer.
    kACInone          257             Sets the color method to None.
    </table>

    \sa
    <exref target="https://docs.opendesign.com/td/cm_color_sample_base.html">Example of Working with the Database Color</exref>

    <exref target="https://docs.opendesign.com/td/cm_color_method.html">Methods of the Color Definition</exref>

    <exref target="https://docs.opendesign.com/td/cm_color_index.html">Color Functionality as an ACI-value</exref>
  */
  virtual void setColorIndex(
    OdUInt16 colorIndex) = 0;

  /** \details
    Converts to the named color and sets the color name and book name for the
    database color object.
    If the book name is an empty string, the method sets the book name to
    UNNAMED.
    If the color name is an empty string, the method ignores the specified
    values.
    If the book name and the color name are empty strings, the method
    converts the database color object to the unnamed color.

    \param colorName [in] Color name as a string value.
    \param bookName  [in] Book name as a string value.

    \returns
    Returns true if and only if the conversion is successful.

    \sa
    <exref target="https://docs.opendesign.com/td/cm_color_sample_base.html">Example of Working with the Database Color</exref>

    <exref target="https://docs.opendesign.com/td/cm_color_book.html">Color Functionality as a Book Name</exref>

    <exref target="https://docs.opendesign.com/td/cm_color_integer.html">Color Functionality as an Integer-value</exref>
  */
  virtual bool  setNames(
    const OdString& colorName,
    const OdString& bookName = OdString::kEmpty) = 0;

  /** \details
    Returns the color name of the database color object.

    \sa
    <exref target="https://docs.opendesign.com/td/cm_color_sample_base.html">Example of Working with the Database Color</exref>

    <exref target="https://docs.opendesign.com/td/cm_color_book.html">Color Functionality as a Book Name</exref>
  */
  virtual OdString colorName() const = 0;

  /** \details
    Returns the book name of the database color object.

    \sa
    <exref target="https://docs.opendesign.com/td/cm_color_sample_base.html">Example of Working with the Database Color</exref>

    <exref target="https://docs.opendesign.com/td/cm_color_book.html">Color Functionality as a Book Name</exref>
  */
  virtual OdString bookName() const = 0;

  /** \details
    Returns the display color name of the database color object.

    \remarks
    For named colors, this is the same as colorName(). For unnamed colors, it is
    an 'appropriate' name.

    \sa
    <exref target="https://docs.opendesign.com/td/cm_color_sample_base.html">Example of Working with the Database Color</exref>

    <exref target="https://docs.opendesign.com/td/cm_color_book.html">Color Functionality as a Book Name</exref>
  */
  virtual OdString colorNameForDisplay() const = 0;
};

/** <group OdCm_Classes>
  \details
  This class implements a transparency object, which provides the transparency
  information about OdGiDrawable objects.

  \sa
  <exref target="https://docs.opendesign.com/td/cm_transparency_sample.html">Example of Working with Transparency</exref>

  <exref target="https://docs.opendesign.com/td/cm_overview.html">Overview of Classes that Implement Color and Transparency</exref>

  <exref target="https://docs.opendesign.com/td/cm_transparency_method.html">Methods of the Transparency Definition</exref>
*/
class OdCmTransparency
{
public:
//DOM-IGNORE-BEGIN
  ODRX_HEAP_OPERATORS();
//DOM-IGNORE-END

  /** \details
    Represents the transparency method.

    \sa
    <exref target="https://docs.opendesign.com/td/cm_transparency_method.html">Methods of the Transparency Definition</exref>
  */
  enum transparencyMethod
  {
    kByLayer    = 0, // Use the setting for the layer.
    kByBlock    = 1, // Use the setting for the block.
    kByAlpha    = 2, // Use the Alpha-value in this object.
    kErrorValue = 3  // Error value.
  };

  /** \details Creates a transparency object with default properties.

    \remarks
    The default transparency method is kByLayer.
  */
  OdCmTransparency() { setMethod(kByLayer); }

  /** \details Creates a transparency object and sets the specified transparency
    method for it.

    \param method [in] Transparency method.

    \sa
    <exref target="https://docs.opendesign.com/td/cm_transparency_method.html">Methods of the Transparency Definition</exref>
  */
  OdCmTransparency(transparencyMethod method) { setMethod(method); }

  /** \details Creates a transparency object and sets its transparency property
    using a specified alpha value.

    \param alpha [in] An alpha value of transparency as an integer value in the
      range from 0 to 255.

    \remarks
    The transparency method is set to kByAlpha.
    Alpha = 0 corresponds to full transparency. Alpha = 255 corresponds to full opacity.

    \sa
    <exref target="https://docs.opendesign.com/td/cm_transparency_alpha.html">Transparency Functionality as an Alpha-value</exref>
  */
  OdCmTransparency(OdUInt8 alpha) { setAlpha(alpha); }

  /** \details Creates a transparency object and sets its transparency property
    using a specified alpha value as a percentage.

    \param alphaPercent [in] An alpha value of transparency in percent as a
      double value in the range from 0.0 to 1.0.

    \remarks
    The transparency method is set to kByAlpha.
    Alpha = 0.0 corresponds to full transparency. Alpha = 1.0 corresponds to full opacity.

    \sa
    <exref target="https://docs.opendesign.com/td/cm_transparency_alpha.html">Transparency Functionality as an Alpha-value</exref>
  */
  OdCmTransparency(double alphaPercent) { setAlphaPercent(alphaPercent); }

  /** \details Creates a copy of an existing transparency object.

    \param transparency [in] The transparency object to be copied.
  */
  OdCmTransparency(
    const OdCmTransparency& transparency) { m_AM = transparency.m_AM; }

//DOM-IGNORE-BEGIN
  ~OdCmTransparency() {}
//DOM-IGNORE-END

  /** \details
    Copies a transparency object specified on the right of the operator to the transparency object specified on the left.

    \sa
    <exref target="https://docs.opendesign.com/td/cm_transparency_sample.html">Example of Working with Transparency</exref>

    <exref target="https://docs.opendesign.com/td/cm_transparency_integer.html">Transparency Functionality as an Integer-value</exref>
  */
  OdCmTransparency& operator=(
    const OdCmTransparency& transparency) { m_AM = transparency.m_AM; return *this; }

  /** \details
    Compares two transparency objects as integer values, and returns true when
    their values are equal or false when their values are not equal.

    \sa
    <exref target="https://docs.opendesign.com/td/cm_transparency_sample.html">Example of Working with Transparency</exref>

    <exref target="https://docs.opendesign.com/td/cm_transparency_integer.html">Transparency Functionality as an Integer-value</exref>
  */
  bool operator==(
    const OdCmTransparency& transparency) const { return (m_AM == transparency.m_AM); }

  /** \details
    Compares two transparency objects as integer values, and returns true when
    their values are not equal or false when their values are equal.

    \sa
    <exref target="https://docs.opendesign.com/td/cm_transparency_sample.html">Example of Working with Transparency</exref>

    <exref target="https://docs.opendesign.com/td/cm_transparency_integer.html">Transparency Functionality as an Integer-value</exref>
  */
  bool operator!=(
    const OdCmTransparency& transparency) const { return (m_AM != transparency.m_AM); }

  /** \details
    Sets the alpha value and switches the transparency method to kByAlpha for the transparency object.

    \param alpha [in]  Alpha value as an integer value in the range 0 to 255.

    \remarks
    An alpha value defines the degree of transparency.
    Alpha = 0 corresponds to full transparency. Alpha = 255 corresponds to full opacity.

    \sa
    <exref target="https://docs.opendesign.com/td/cm_transparency_sample.html">Example of Working with Transparency</exref>

    <exref target="https://docs.opendesign.com/td/cm_transparency_alpha.html">Transparency Functionality as an Alpha-value</exref>
  */
  void setAlpha(
    OdUInt8 alpha)
  {
    m_AM = ((OdUInt32)kByAlpha << 24) | alpha;
  }

  /** \details
    Returns the alpha value in the range from 0 to 255 of the transparency object.

    \remarks
    An alpha value defines the degree of transparency.
    Alpha = 0 corresponds to full transparency. Alpha = 255 corresponds to full opacity.

    \sa
    <exref target="https://docs.opendesign.com/td/cm_transparency_sample.html">Example of Working with Transparency</exref>

    <exref target="https://docs.opendesign.com/td/cm_transparency_alpha.html">Transparency Functionality as an Alpha-value</exref>
  */
  OdUInt8 alpha() const
  {
    if (!isByAlpha())
      return 255;
    return OdUInt8(m_AM & 0xFF);
  }

  /** \details
    Sets the alpha value as a percentage and switches the transparency method to
    kByAlpha for the transparency object.

    \param alphaPercent [in]  Alpha as a double value in the range from 0.0 to 1.0.

    \remarks
    An alpha value defines the degree of transparency.
    Alpha = 0.0 corresponds to full transparency. Alpha = 1.0 corresponds to full opacity.

    \sa
    <exref target="https://docs.opendesign.com/td/cm_transparency_sample.html">Example of Working with Transparency</exref>

    <exref target="https://docs.opendesign.com/td/cm_transparency_alpha.html">Transparency Functionality as an Alpha-value</exref>
  */
  void setAlphaPercent(
    double alphaPercent)
  {
    if (alphaPercent < 0.0)
      setAlpha(0);
    else if (alphaPercent > 1.0)
      setAlpha(0xFF);
    else
      setAlpha((OdUInt8)(alphaPercent * 255));
  }

  /** \details
    Returns the alpha value in the range from 0.0 to 1.0 of the transparency object.

    \remarks
    An alpha value defines the degree of transparency.
    Alpha = 0.0 corresponds to full transparency. Alpha = 1.0 corresponds to full opacity.

    \sa
    <exref target="https://docs.opendesign.com/td/cm_transparency_sample.html">Example of Working with Transparency</exref>

    <exref target="https://docs.opendesign.com/td/cm_transparency_alpha.html">Transparency Functionality as an Alpha-value</exref>
  */
  double alphaPercent() const
  {
    return (double)alpha() / 255;
  }

  /** \details
    Returns the transparency method of the transparency object as a value of the transparencyMethod enumeration.

    \remarks
    The transparency method can be a one of the following:

    <table>
    <b>Name</b>    <b>Value</b>    <b>Description</b>
    kByLayer       0               Transparency is specified by the layer object that the object is assigned to.
    kByBlock       1               Transparency is specified by the block object that the object is contained in.
    kByAlpha       2               Transparency is specified by an alpha value.
    </table>

    \sa
    <exref target="https://docs.opendesign.com/td/cm_transparency_sample.html">Example of Working with Transparency</exref>

    <exref target="https://docs.opendesign.com/td/cm_transparency_method.html">Methods of the Transparency Definition</exref>
  */
  transparencyMethod method() const
  {
    return transparencyMethod(OdUInt8(m_AM >> 24));
  }

  /** \details
    Sets the transparency method for the transparency object.

    \param method [in]  Transparency method as the transparencyMethod enumeration.

    \remarks
    The transparency method can be one of the following:

    <table>
    <b>Name</b>    <b>Value</b>    <b>Description</b>
    kByLayer       0               Transparency is specified by the layer object that the object is assigned to.
    kByBlock       1               Transparency is specified by the block object that the object is contained in.
    kByAlpha       2               Transparency is specified by an alpha value.
    </table>

    \sa
    <exref target="https://docs.opendesign.com/td/cm_transparency_sample.html">Example of Working with Transparency</exref>

    <exref target="https://docs.opendesign.com/td/cm_transparency_method.html">Methods of the Transparency Definition</exref>
  */
  void setMethod(
    transparencyMethod method)
  {
    m_AM = (OdUInt32)method << 24;
  }

  /** \details
    Checks whether the transparency method is byAlpha for the transparency
    object and returns true if and only if the transparency method is set to
    kByAlpha, otherwise it returns false.

    \sa
    <exref target="https://docs.opendesign.com/td/cm_transparency_sample.html">Example of Working with Transparency</exref>

    <exref target="https://docs.opendesign.com/td/cm_transparency_method.html">Methods of the Transparency Definition</exref>
  */
  bool isByAlpha() const { return (method() == kByAlpha); }

  /** \details
    Checks whether the transparency method is byBlock for the transparency
    object and returns true if and only if the transparency method is set to
    kByBlock, otherwise it returns false.

    \sa
    <exref target="https://docs.opendesign.com/td/cm_transparency_sample.html">Example of Working with Transparency</exref>

    <exref target="https://docs.opendesign.com/td/cm_transparency_method.html">Methods of the Transparency Definition</exref>
  */
  bool isByBlock() const { return (method() == kByBlock); }

  /** \details
    Checks whether the transparency method is byLayer for the transparency
    object and returns true if and only if the transparency method is set to
    kByLayer, otherwise it returns false.

    \sa
    <exref target="https://docs.opendesign.com/td/cm_transparency_sample.html">Example of Working with Transparency</exref>

    <exref target="https://docs.opendesign.com/td/cm_transparency_method.html">Methods of the Transparency Definition</exref>
  */
  bool isByLayer() const { return (method() == kByLayer); }

  /** \details
    Checks whether the transparency method is an error value for the transparency
    object and returns true if and only if the transparency method is set to
    kErrorValue, otherwise it returns false.

    \sa
    <exref target="https://docs.opendesign.com/td/cm_transparency_sample.html">Example of Working with Transparency</exref>

    <exref target="https://docs.opendesign.com/td/cm_transparency_method.html">Methods of the Transparency Definition</exref>
  */
  bool isInvalid() const { return (method() == kErrorValue); }

  /** \details
    Checks whether the transparency method is byAlpha and the transparency is
    set to full for this transparency object. It returns true if and only if the
    transparency method is set to kByAlpha and Alpha = 0, otherwise it returns
    false.

    \sa
    <exref target="https://docs.opendesign.com/td/cm_transparency_sample.html">Example of Working with Transparency</exref>

    <exref target="https://docs.opendesign.com/td/cm_transparency_method.html">Methods of the Transparency Definition</exref>

    <exref target="https://docs.opendesign.com/td/cm_transparency_alpha.html">Transparency Functionality as an Alpha-value</exref>
  */
  bool isClear() const   { return (method() == kByAlpha) && (alpha() == 0);}
  
  /** \details
    Checks whether the transparency method is byAlpha and the opacity is set to
    full for this transparency object. It returns true if and only if the
    transparency method is set to kByAlpha and Alpha = 255, otherwise it returns
    false.

    \sa
    <exref target="https://docs.opendesign.com/td/cm_transparency_sample.html">Example of Working with Transparency</exref>

    <exref target="https://docs.opendesign.com/td/cm_transparency_method.html">Methods of the Transparency Definition</exref>

    <exref target="https://docs.opendesign.com/td/cm_transparency_alpha.html">Transparency Functionality as an Alpha-value</exref>
  */
  bool isSolid() const   { return (method() == kByAlpha) && (alpha() == 255);}

  /** \details
    Returns the packed 32-bit integer value that stores the transparency method
    and transparency components.

    \sa
    <exref target="https://docs.opendesign.com/td/cm_transparency_sample.html">Example of Working with Transparency</exref>

    <exref target="https://docs.opendesign.com/td/cm_transparency_method.html">Methods of the Transparency Definition</exref>

    <exref target="https://docs.opendesign.com/td/cm_transparency_integer.html">Transparency Functionality as an Integer-value</exref>
  */
  OdUInt32 serializeOut() const { return m_AM; }

  /** \details
    Sets the transparency method and transparency components as an integer value
    for the transparency object.

    \param transparency [in]  A packed 32-bit integer value that specifies the
      transparency method and transparency components.

    \sa
    <exref target="https://docs.opendesign.com/td/cm_transparency_sample.html">Example of Working with Transparency</exref>

    <exref target="https://docs.opendesign.com/td/cm_transparency_method.html">Methods of the Transparency Definition</exref>

    <exref target="https://docs.opendesign.com/td/cm_transparency_integer.html">Transparency Functionality as an Integer-value</exref>
  */
  void serializeIn(
    OdUInt32 transparency) { m_AM = transparency; }

  /** \details
    Reads from the specified .dwg filer data for this object.

    \param pFiler [in]  Pointer to the filer that the data is to be read from.
  */
  void dwgIn(
    OdDbDwgFiler* pFiler);

  /** \details
    Writes to the specified .dwg filer data of this object.

    \param pFiler [in]  Pointer to the filer that the data is to be written to.
  */
  void dwgOut(
    OdDbDwgFiler* pFiler) const;

private: 

  /** \details
    The combined value of the transparency method and transparency components.

    \sa
    <exref target="https://docs.opendesign.com/td/cm_transparency_method.html">Methods of the Transparency Definition</exref>
  */
  OdUInt32 m_AM;
};

#include "TD_PackPop.h"

#endif //ODA_CMBASE_COLOR
