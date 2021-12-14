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


///////////////////////////////////////////////////////////////////////////////
//
// PdfPublishRadioButton.h
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _PDF_PUBLISH_RADIOBUTTON_INCLUDED_
#define _PDF_PUBLISH_RADIOBUTTON_INCLUDED_

#include "PdfPublishBaseObject.h"

/** \details
  Contains declarations for Publish SDK.
  <group TD_Namespaces>
*/
namespace OdPdfPublish{

/** \details
  This class implements a radio button control contained in a .pdf document created with Publish SDK.
  A radio button represent a dependent option to be selected.
  An option is called dependent when several options are joined into a group and only one option from the group can be selected. 
  Then the other options become unselected.
*/
class PDFPUBLISH_TOOLKIT OdRadioButton : public OdObject
{
  //DOM-IGNORE-BEGIN
protected:
  OdRadioButton();
  //DOM-IGNORE-END
public:
  //DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdRadioButton);
  ODRX_HEAP_OPERATORS();
  //DOM-IGNORE-END
  
  /** \details
    Destroys the radio button.
  */
  virtual ~OdRadioButton();

/** \details
    Sets the radio button name.
    
    \param name [in] A new radio button name.

    \remarks
    The name is mandatory and must be unique among the document object names.
  */
  void setName(const OdString& name);

  /** \details
    Sets the radio button font type from the list of <link OdPdfPublish::Text::StandardFontsType, predefined fonts>. 
    
    \param fontType [in] A new font type value.
    
    \sa 
    <link OdPdfPublish::Text::StandardFontsType, Text::StandardFontsType enumeration>.
  */
  void setFont(Text::StandardFontsType fontType);

  /** \details
    Sets the radio button font name and style. 
    
    \param fontName  [in] A new font name.
    \param fontStyle [in] A new font style.
    
    \remarks
    This method can set a font that is not in the list of 
    <link OdPdfPublish::Text::StandardFontsType, standard fonts>.
    By default the <link OdPdfPublish::Text::StandardFontsType::kTimesRoman, normal Times New Roman font> is used.
    
    \sa 
    <link OdPdfPublish::Text::FontStyle, Text::FontStyle enumeration>
    <link OdPdfPublish::Text::StandardFontsType, StandardFontsType enumeration>
  */
  void setFont(const OdString& fontName, Text::FontStyle fontStyle = Text::kRegular);

  /** \details
    Sets a font size for the radio button.
    
    \param size [in] A new font size value of the radio button.
  */
  void setFontSize(OdUInt16 size);

  /** \details
    Sets a text color for the radio button.
    
    \param color [in] A new color value.
  */
  void setTextColor(ODCOLORREF color);

  /** \details
    Sets tooltip text for the radio button.
    Tooltip text is text that appears when a user hovers the mouse pointer over the radio button.
    
    \param tooltip [in] A new value of the tooltip text.
  */
  void setTooltip(const OdString& tooltip);

  /** \details
    Sets a new value for the visibility flag for the radio button.
    The visibility flag determines whether the radio button is visible in a document (if true) or is hidden (if false).
    
    \param state [in] A new flag value.
  */
  void setVisibility(bool state);

  /** \details
    Sets a new value for the flag that determines whether the radio button should be printed.
    If the radio button should be printed, the flag value is equal to true, otherwise it is equal to false.
    
    \param state [in] A new flag value.
  */
  void setPrintability(bool state);

  /** \details
    Sets a new value for the radio button text rotation.
    
    \param rotation [in] A new text rotation value.
    
    \sa 
    <link OdPdfPublish::Text::Rotation, Text::Rotation enumeration>.
  */
  void setTextRotation(Text::Rotation rotation);

  /** \details
    Sets a new value for the flag that determines whether the radio button is locked.
    If the flag value is equal to true, the radio button is locked and becomes disabled, otherwise it is enabled and unlocked.
    
    \param state [in] A new flag value.
  */
  void setLock(bool state);

  /** \details
    Sets a new value for the flag that determines whether the radio button has a border.
    If the flag value is equal to true, the radio button has a border, otherwise it does not.
    
    \param state [in] A new flag value.
  */
  void setBorder(bool state);

  /** \details
    Sets the radio button border color.
    
    \param color [in] A new color value.
  */
  void setBorderColor(ODCOLORREF color);

  /** \details
    Sets the radio button border thickness.
    
    \param thickness [in] A new border thickness value.
    
    \sa
    <link OdPdfPublish::Border::Thickness, Border::Thickness enumeration>.
  */
  void setBorderThickness(Border::Thickness thickness);

  /** \details
    Sets the radio button border style.
    
    \param style [in] A new border style.
    
    \sa
    <link OdPdfPublish::Border::Style, Border::Style enumeration>.
  */
  void setBorderStyle(Border::Style style);

  /** \details
    Sets the radio button fill color.
    
    \param color [in] A new color value.
  */
  void setFillColor(ODCOLORREF color);

  /** \details
    Sets an export value for the radio button.
    
    \param export_value [in] A new export value.
  */  
  void setExportValue(const OdString& export_value);

  /** \details
    Sets the radio button default state.
    If the default state is equal to true, the radio button is switched on when it is displayed in a document, otherwise the radio button is switched off.
    
    \param state [in] A new default state.
  */
  void setDefaultState(bool state);

  /** \details
    Sets a new value for the flag that determines whether the radio button is included in a group of radio buttons.
    If the flag value is equal to true, the radio button is in a group, otherwise it is a standalone radio button.
    
    \param state [in] A new flag value.
  */
  void setGrouping(bool state);

  /** \details
    Returns the current name of the radio button.
    The method fills a passed string object with the radio button name and returns it to a calling subroutine.
    
    \param name [out] A placeholder for the radio button name.
  */
  void getName(OdString& name) const;

  /** \details
    Returns current font parameters used for displaying the radio button caption.
    The method fills passed values and returns them to a calling subroutine.
    
    \param storageType  [out] A placeholder for the storage type value.
    \param fontType     [out] A placeholder for the font type that should be used for displaying the radio button caption.
    \param fontName     [out] A placeholder for the font name that should be used for displaying the radio button caption.
    \param fontStyle    [out] A placeholder for the font style that should be used for displaying the radio button caption.
    
    \sa
    <link OdPdfPublish::Text::StorageType, Text::StorageType enumeration>
    <link OdPdfPublish::Text::StandardFontsType, Text::StandardFontsType enumeration>
    <link OdPdfPublish::Text::FontStyle, Text::FontStyle enumeration>
  */
  void getFont(Text::StorageType& storageType, Text::StandardFontsType& fontType, OdString& fontName, Text::FontStyle& fontStyle) const;

  /** \details
    Returns the current font size used for displaying the radio button caption.
    The method fills a passed integer value with the radio button caption font size and returns it to a calling subroutine.
    
    \param size [out] A placeholder for the font size that should be used for the radio button caption.
  */
  void getFontSize(OdUInt16& size) const;

  /** \details
    Returns the current text color used for displaying the radio button caption.
    The method fills a passed ODCOLORREF value with the radio button caption color and returns it to a calling subroutine.
    
    \param color [out] A placeholder for the text color that should be used for the radio button caption.
  */
  void getTextColor(ODCOLORREF& color) const;

  /** \details
    Returns the current tooltip text for the radio button.
    Tooltip text is text that appears when a user hovers the mouse pointer over the radio button.
    
    The method fills a passed string object with the radio button tooltip text and returns it to a calling subroutine.
    
    \param tooltip [out] A placeholder for the tooltip text.
  */
  void getTooltip(OdString& tooltip) const;

  /** \details
    Returns the current value of the visibility flag for the radio button.
    The visibility flag determines whether the radio button is visible in a document (if true) or is hidden (if false).
    The method fills a passed boolean value with the radio button visibility flag value and returns it to a calling subroutine.    
    
    \param state [out] A placeholder for the flag value.
  */
  void getVisibility(bool& state) const;

  /** \details
    Returns the current value of the flag that determines whether the radio button should be printed.
    If the radio button should be printed, the flag value is equal to true, otherwise it is equal to false.
    The method fills a passed boolean value with the radio button printability flag value and returns it to a calling subroutine.
    
    \param state [out] A placeholder for the flag value.
  */
  void getPrintability(bool& state) const;

  /** \details
    Returns the current value of the radio text rotation.
    The method fills a passed value with the current text rotation value and returns it to a calling subroutine.
    
    \param rotation [out] A placeholder for the text rotation value.
    
    \sa 
    <link OdPdfPublish::Text::Rotation, Text::Rotation enumeration>.
  */
  void getTextRotation(Text::Rotation& rotation) const;

  /** \details
    Returns the current value of the flag that determines whether the radio button is locked.
    If the flag value is equal to true, the radio button is locked and becomes disabled, otherwise it is enabled and unlocked.
    The method fills a passed boolean value with the radio button lock flag value and returns it to a calling subroutine.
    
    \param state [out] A placeholder for the flag value.
  */
  void getLock(bool& state) const;

  /** \details
    Returns the current value of the flag that determines whether the radio button has a border.
    If the flag value is equal to true, the radio button has a border, otherwise it does not.
    The method fills a passed boolean value with the radio button border flag value and returns it to a calling subroutine.
    
    \param state [out] A placeholder for the flag value.
  */
  void getBorder(bool& state) const;

  /** \details
    Returns the current radio button border color.
    The method fills a passed ODCOLORREF value with the radio button border color value and returns it to a calling subroutine.
    
    \param color [out] A placeholder for the color value.
  */
  void getBorderColor(ODCOLORREF& color) const;

  /** \details
    Returns the current radio button border thickness.
    The method fills a passed value with the current radio button border thickness and returns it to a calling subroutine.
    
    \param thickness [out] A placeholder for the border thickness.
    
    \sa
    <link OdPdfPublish::Border::Thickness, Border::Thickness enumeration>.
  */
  void getBorderThickness(Border::Thickness& thickness) const;

  /** \details
    Returns the current radio button border style.
    The method fills a passed value with the current radio button border style and returns it to a calling subroutine.
    
    \param style [out] A placeholder for the border style.
    
    \sa
    <link OdPdfPublish::Border::Style, Border::Style enumeration>.
  */
  void getBorderStyle(Border::Style& style) const;

  /** \details
    Returns the current radio button fill color.
    The method fills a passed ODCOLORREF value with the radio button fill color value and returns it to a calling subroutine.
    
    \param color [out] A placeholder for the color value.
  */
  void getFillColor(ODCOLORREF& color) const;

  /** \details
    Returns the current export value for the radio button.
    The method fills a passed string object with the radio button export value and returns it to a calling subroutine.

    \param export_value [out] A placeholder for the export value.
  */
  void getExportValue(OdString& export_value) const;

  /** \details
    Returns the current radio button default state.
    If the default state is equal to true, the radio button is switched on when it is displayed in a document, otherwise the radio button is switched off.
    The method fills a boolean object with the radio button default state value and returns it to a calling subroutine.
    
    \param default_state [out] A placeholder for the default state value.
  */
  void getDefaultState(bool& default_state) const;

  /** \details
    Returns the current value of the group flag that determines whether the radio button is included in a group of radio buttons.
    If the flag value is equal to true, the radio button is in a group, otherwise it is a standalone radio button.
    The method fills a passed boolean value with the current flag value and returns it to a calling subroutine.
    
    \param grouping [out] A new flag value.
  */
  void getGrouping(bool& grouping) const;

};

/** \details
  A data type that represents a smart pointer to an <link OdPdfPublish::OdRadioButton, OdRadioButton> object.
*/
SMARTPTR(OdRadioButton);

}
#endif // _PDF_PUBLISH_RADIOBUTTON_INCLUDED_

