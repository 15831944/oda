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
// PdfPublishDropDownList.h
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _PDF_PUBLISH_DROPDOWN_INCLUDED_
#define _PDF_PUBLISH_DROPDOWN_INCLUDED_

#include "PdfPublishBaseObject.h"
#include "StringArray.h"

/** \details
  Contains declarations for Publish SDK.
  <group TD_Namespaces>
*/
namespace OdPdfPublish{

/** \details
  This class implements a drop-down list control contained in a .pdf document created with Publish SDK.
  A drop-down list contains a list of predefined items that can be selected. Only one of the predefined items can be selected. 
  A user can also enter a custom value to a drop-down list if allowed (see <link OdPdfPublish::OdDropDownList::getCustomText@bool&@const, setCustomText()> 
  and <link OdPdfPublish::OdDropDownList::setCustomText@bool, getCustomText()> methods).
*/
class PDFPUBLISH_TOOLKIT OdDropDownList : public OdObject
{
  //DOM-IGNORE-BEGIN
protected:
  OdDropDownList();
  //DOM-IGNORE-END
public:
  //DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdDropDownList);
  ODRX_HEAP_OPERATORS();
  //DOM-IGNORE-END
  
  /** \details
    Destroys the drop-down list control.
  */
  virtual ~OdDropDownList();

  /** \details
    Sets the drop-down list name.
    
    \param name [in] A new drop-down list name.

    \remarks
    The name is mandatory and must be unique among the document object names.
  */
  void setName(const OdString& name);

  /** \details
    Sets the drop-down list font type from the list of <link OdPdfPublish::Text::StandardFontsType,  predefined fonts>. 
    
    \param fontType [in] A new font type value.
    
    \sa 
    <link OdPdfPublish::Text::StandardFontsType, Text::StandardFontsType enumeration>.
  */
  void setFont(Text::StandardFontsType fontType);
  
  /** \details
    Sets the drop-down list font name and style. 
    
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
    Sets a font size for the drop-down list.
    
    \param size [in] A new font size value of the drop-down list.
  */
  void setFontSize(OdUInt16 size);

  /** \details
    Sets a text color for the drop-down list.
    
    \param color [in] A new color value.
  */
  void setTextColor(ODCOLORREF color);

  /** \details
    Sets tooltip text for the drop-down list.
    Tooltip text is text that appears when a user hovers the mouse pointer over the drop-down list.
    
    \param tooltip [in] A new value of the tooltip text.
  */
  void setTooltip(const OdString& tooltip);

  /** \details
    Sets a new value of the visibility flag for the drop-down list.
    The visibility flag determines whether the drop-down list is visible in a document (if true) or is hidden (if false).
    
    \param state [in] A new flag value.
  */
  void setVisibility(bool state);

  /** \details
    Sets a new value of the flag that determines whether the drop-down list should be printed.
    If the drop-down list should be printed, the flag value is equal to true, otherwise it is equal to false.
    
    \param state [in] A new flag value.
  */
  void setPrintability(bool state);

  /** \details
    Sets a new value for the drop-down list text rotation.
    
    \param rotation [in] A new text rotation value.
    
    \sa
    <link OdPdfPublish::Text::Rotation, Text::Rotation enumeration>.
  */
  void setTextRotation(Text::Rotation rotation);

  /** \details
    Sets a new value of the flag that determines whether the drop-down list is locked.
    If the flag value is equal to true, the drop-down list is locked and becomes disabled, otherwise it is enabled and unlocked.
    
    \param state [in] A new flag value.
  */
  void setLock(bool state);

  /** \details
    Sets a new value of the flag that determines whether the drop-down list has a border.
    If the flag value is equal to true, the drop-down list has a border, otherwise it does not.
    
    \param state [in] A new flag value.
  */
  void setBorder(bool state);

  /** \details
    Sets a drop-down list border color.
    
    \param color [in] A new color value.
  */
  void setBorderColor(ODCOLORREF color);

  /** \details
    Sets a new value for the drop-down list border thickness.
    
    \param thickness [in] A new thickness value.
    
    \sa
    <link OdPdfPublish::Border::Thickness, Border::Thickness enumeration>.
  */
  void setBorderThickness(Border::Thickness thickness);

  /** \details
    Sets a new border style for the drop-down list.
    
    \param style [in] A new border style.
    
    \sa
    <link OdPdfPublish::Border::Style, Border::Style enumeration>.
  */
  void setBorderStyle(Border::Style style);

  /** \details
    Sets a drop-down list fill color.
    
    \param color [in] A new color value.
  */
  void setFillColor(ODCOLORREF color);

  /** \details
    Sets a new value of the custom text flag for the drop-down list.
    The custom text flag determines whether a user can add a custom value to the drop-down list. 
    If the flag is equal to true, a user can enter a custom value, otherwise the user can only select one of the predefined values from the drop-down list.
    
    \param state [in] A new flag value.
  */
  void setCustomText(bool state);

  /** \details
    Sets a new value of the spell-checking flag for the drop-down list.
    The spell-checking flag determines whether user text entered in the drop-down list should be checked. 
    If the flag is equal to true, the custom value in the drop-down list is spell-checked, otherwise the drop-down list does not spell-check the user text.
    
    \param state [in] A new flag value.
  */
  void setSpellChecking(bool state);

  /** \details
    Sets a new value of the immediate commit flag for the drop-down list.
    The immediate commit flag determines whether user text entered in the drop-down list is committed immediately (if set to true). 
    
    \param state [in] A new flag value.
  */
  void setImmediateCommit(bool state);

  /** \details
    Sets the drop-down list content.
    The method fills the drop-down list with an array of items. Each item is a string value.
    
    \param displayed_values [in] An array of string items to be displayed as the content of the drop-down list control.
    \param export_values    [in] An array of string items to be exported from the drop-down list control.
  */
  void setContents(const OdStringArray& displayed_values, const OdStringArray& export_values);
  
  /** \details
    Returns the current name of the drop-down list.
    The method fills a passed string object with the drop-down list name and returns it to a calling subroutine.
    
    \param name [out] A placeholder for the drop-down list name.
  */
  void getName(OdString& name) const;

  /** \details
    Returns the current font name used for displaying drop-down list items.
    The method fills a passed string object and Text::FontStyle value with the drop-down list font name and font style, then returns them to a calling subroutine.
    
    \param storageType  [out] A placeholder for the font storage type.
    \param fontType     [out] A placeholder for the font type that should be used for displaying drop-down list items.
    \param fontName     [out] A placeholder for the font name that should be used for displaying drop-down list items.
    \param fontStyle    [out] A placeholder for the font style that should be used for displaying drop-down list items.
    
    \sa
    <link OdPdfPublish::Text::StorageType, Text::StorageType enumeration>
    <link OdPdfPublish::Text::StandardFontsType, Text::StandardFontsType enumeration>
    <link OdPdfPublish::Text::FontStyle, Text::FontStyle enumeration>
  */
  void getFont(Text::StorageType& storageType, Text::StandardFontsType& fontType, OdString& fontName, Text::FontStyle& fontStyle) const;

  /** \details
    Returns the current font size used for displaying drop-down list items.
    The method fills a passed integer value with the drop-down list font size and returns it to a calling subroutine.
    
    \param size [out] A placeholder for the font size that should be used for displaying drop-down list items.
  */
  void getFontSize(OdUInt16& size) const;

  /** \details
    Returns the current text color used for displaying drop-down list items.
    The method fills a passed ODCOLORREF value with the drop-down list caption color and returns it to a calling subroutine.
    
    \param color [out] A placeholder for the text color that should be used for displaying drop-down list items.
  */
  void getTextColor(ODCOLORREF& color) const;

  /** \details
    Returns the current tooltip text for the drop-down list.
    Tooltip text is text that appears when a user hovers the mouse pointer over the drop-down list.
    
    The method fills a passed string object with the drop-down list tooltip text and returns it to a calling subroutine.
    
    \param tooltip [out] A placeholder for the tooltip text.
  */
  void getTooltip(OdString& tooltip) const;

  /** \details
    Returns the current value of the visibility flag for the drop-down list.
    The visibility flag determines whether the drop-down list is visible in a document (if true) or is hidden (if false).
    The method fills a passed boolean value with the drop-down list visibility flag value and returns it to a calling subroutine.
    
    \param state [out] A placeholder for the flag value.
  */
  void getVisibility(bool& state) const;

  /** \details
    Returns the current value of the flag that determines whether the drop-down list should be printed.
    If the drop-down list should be printed, the flag value is equal to true, otherwise it is equal to false.
    The method fills a passed boolean value with the drop-down list printability flag value and returns it to a calling subroutine.
    
    \param state [out] A placeholder for the flag value.
  */
  void getPrintability(bool& state) const;

  /** \details
    Returns the current rotation of the drop-down list text.
    The method fills a passed value with the current text rotation value and returns it to a calling subroutine.
    
    \param rotation [out] A placeholder for the text rotation value.
    
    \sa
    <link OdPdfPublish::Text::Rotation, Text::Rotation enumeration>.
  */
  void getTextRotation(Text::Rotation& rotation) const;

  /** \details
    Returns the current value of the flag that determines whether the drop-down list is locked.
    If the flag value is equal to true, the drop-down list is locked and becomes disabled, otherwise it is enabled and unlocked.
    The method fills a passed boolean value with the drop-down list lock flag value and returns it to a calling subroutine.
    
    \param state [out] A placeholder for the flag value.
  */
  void getLock(bool& state) const;

  /** \details
    Returns the current value of the flag that determines whether the drop-down list has a border.
    If the flag value is equal to true, the drop-down list has a border, otherwise it does not.
    The method fills a passed boolean value with the drop-down list border flag value and returns it to a calling subroutine.
    
    \param state [out] A placeholder for the flag value.
  */
  void getBorder(bool& state) const;

  /** \details
    Returns the current drop-down list border color.
    The method fills a passed ODCOLORREF value with the drop-down list border color value and returns it to a calling subroutine.
    
    \param color [out] A placeholder for the color value.
  */
  void getBorderColor(ODCOLORREF& color) const;

  /** \details
    Returns the current drop-down list border thickness.
    The method fills a passed value with the current border thickness and returns it to a calling subroutine.
    
    \param thickness [out] A placeholder for the border thickness.

    \sa
    <link OdPdfPublish::Border::Thickness, Border::Thickness enumeration>.
  */
  void getBorderThickness(Border::Thickness& thickness) const;

  /** \details
    Returns the current drop-down list border style.
    The method fills a passed value with the current border style value and returns it to a calling subroutine.
    
    \param style [out] A placeholder for the border style value.
    
    \sa
    <link OdPdfPublish::Border::Style, Border::Style enumeration>.
  */
  void getBorderStyle(Border::Style& style) const;

  /** \details
    Returns the current drop-down list fill color.
    The method fills a passed ODCOLORREF value with the drop-down list fill color value and returns it to a calling subroutine.
    
    \param color [out] A placeholder for the color value.
  */
  void getFillColor(ODCOLORREF& color) const;

  /** \details
    Returns the current value of the custom text flag of the drop-down list.
    The custom text flag determines whether a user can add a custom value to the drop-down list. 
    If the flag is equal to true, a user can enter a custom value, otherwise the user can only select one of the predefined values from the drop-down list.
    The method fills a passed boolean value with the custom text flag value and returns it to a calling subroutine.
    
    \param state [out] A placeholder for the flag value.
  */
  void getCustomText(bool& state) const;

  /** \details
    Returns the current value of the spell-checking flag of the drop-down list.
    The spell-checking flag determines whether user text entered in the drop-down list should be checked. 
    If the flag is equal to true, the custom value of the drop-down list is spell-checked, otherwise the drop-down list does not spell-check the user text.
    The method fills a passed boolean value with the spell-checking flag value and returns it to a calling subroutine.
    
    \param state [out] A placeholder for the flag value.
  */
  void getSpellChecking(bool& state) const;

  /** \details
    Returns the current value of the immediate commit flag of the drop-down list.
    The immediate commit flag determines whether user text entered in the drop-down list is committed immediately (if set to true). 
    The method fills a passed boolean value with the immediate commit flag value and returns it to a calling subroutine.
    
    \param state [out] A placeholder of the flag value.
  */
  void getImmediateCommit(bool& state) const;

  /** \details
    Returns the current drop-down list content.
    The method fills the passed string arrays with display and export items respectively and returns them to a calling subroutine.
    
    \param displayed_values [out] A placeholder for the array of string items to be displayed as the content of the drop-down list.
    \param export_values    [out] A placeholder for the array of string items to be exported as the content of the drop-down list.
  */
  void getContents(OdStringArray& displayed_values, OdStringArray& export_values) const;

};

/** \details
  A data type that represents a smart pointer to an <link OdPdfPublish::OdDropDownList, OdDropDownList> object.
*/
SMARTPTR(OdDropDownList);

}
#endif // _PDF_PUBLISH_DROPDOWN_INCLUDED_

