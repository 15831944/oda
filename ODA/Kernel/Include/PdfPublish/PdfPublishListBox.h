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
// PdfPublishListBox.h
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _PDF_PUBLISH_LISTBOX_INCLUDED_
#define _PDF_PUBLISH_LISTBOX_INCLUDED_

#include "PdfPublishBaseObject.h"
#include "StringArray.h"

/** \details
  Contains declarations for Publish SDK.
  <group TD_Namespaces>
*/
namespace OdPdfPublish{

/** \details
  This class implements a list box control contained in a .pdf document created with Publish SDK.
  A list box contains a list of predefined items that can be selected (one or several depending on the multiple selection flag value).
*/
class PDFPUBLISH_TOOLKIT OdListBox : public OdObject
{
  //DOM-IGNORE-BEGIN
protected:
  OdListBox();
  //DOM-IGNORE-END
public:
  //DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdListBox);
  ODRX_HEAP_OPERATORS();
  //DOM-IGNORE-END

  /** \details
    Destroys the list box control.
  */
  virtual ~OdListBox();

  /** \details
    Sets the list box name.
    
    \param name [in] A new list box name.

    \remarks
    The name is mandatory and must be unique among the document object names.
  */
  void setName(const OdString& name);

  /** \details
    Sets the list box font name and style. 
    
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
    Sets the list box font type from the list of <link OdPdfPublish::Text::StandardFontsType,  predefined fonts>. 
    
    \param fontType [in] A new font type value.
    
    \sa 
    <link OdPdfPublish::Text::StandardFontsType, Text::StandardFontsType enumeration>.
  */
  void setFont(const Text::StandardFontsType fontType);

  /** \details
    Sets a font size for the list box.
    
    \param size [in] A new font size value of the list box.
  */
  void setFontSize(OdUInt16 size);

  /** \details
    Sets a text color for the list box.
    
    \param color [in] A new color value.
  */
  void setTextColor(ODCOLORREF color);

  /** \details
    Sets tooltip text for the list box.
    Tooltip text is text that appears when a user hovers the mouse pointer over the list box.
    
    \param tooltip [in] A new value of the tooltip text.
  */
  void setTooltip(const OdString& tooltip);

  /** \details
    Sets a new value of the visibility flag for the list box.
    The visibility flag determines whether the list box is visible in a document (if true) or is hidden (if false).
    
    \param state [in] A new flag value.
  */
  void setVisibility(bool state);

  /** \details
    Sets a new value of the flag that determines whether the list box should be printed.
    If the list box should be printed, the flag value is equal to true, otherwise it is equal to false.
    
    \param state [in] A new flag value.
  */
  void setPrintability(bool state);

  /** \details
    Sets a new value for the list box text rotation.
    
    \param rotation [in] A new text rotation value.
    
    \sa
    <link OdPdfPublish::Text::Rotation, Text::Rotation enumeration>.
  */
  void setTextRotation(Text::Rotation rotation);

  /** \details
    Sets a new value of the flag that determines whether the list box is locked.
    If the flag value is equal to true, the list box is locked and becomes disabled, otherwise it is enabled and unlocked.
    
    \param state [in] A new flag value.
  */
  void setLock(bool state);

  /** \details
    Sets a new value of the flag that determines whether the list box has a border.
    If the flag value is equal to true, the list box has a border, otherwise it does not.
    
    \param state [in] A new flag value.
  */
  void setBorder(bool state);

  /** \details
    Sets the list box border color.
    
    \param color [in] A new color value.
  */
  void setBorderColor(ODCOLORREF color);

  /** \details
    Sets the list box border thickness.
    
    \param thickness [in] A new border thickness.
    
    \sa
    <link OdPdfPublish::Border::Thickness, Border::Thickness enumeration>.
  */
  void setBorderThickness(Border::Thickness thickness);

  /** \details
    Sets the list box border style.
    
    \param style [in] A new border style.
    
    \sa
    <link OdPdfPublish::Border::Style, Border::Style enumeration>.
  */
  void setBorderStyle(Border::Style style);

  /** \details
    Sets the list box fill color.
    
    \param color [in] A new color value.
  */
  void setFillColor(ODCOLORREF color);

/** \details
    Sets the multiple selection flag of the list box.
    The multiple selection flag determines whether more than one item from the list box can be selected simultaneously.
    
    \param state [in] A new value of the multiple selection flag.
  */
  void setMultipleSelection(bool state);

  /** \details
    Sets the list box content.
    The method fills the list box with an array of items. Each item is a string value.
    
    \param displayed_values [in] An array of string items to be displayed as the content of the list box control.
    \param export_values    [in] An array of string items to be exported from the list box control.
  */
  void setContents(const OdStringArray& displayed_values, const OdStringArray& export_values);


  /** \details
    Returns the current name of the list box.
    The method fills a passed string object with the list box name and returns it to a calling subroutine.
    
    \param name [out] A placeholder for the list box name.
  */
  void getName(OdString& name) const;

  /** \details
    Returns current font parameters used for displaying list box items.
    The method fills passed values and returns them to a calling subroutine.
    
    \param storageType  [out] A placeholder for the storage type value.
    \param fontType     [out] A placeholder for the font type that should be used for displaying list box items.
    \param fontName     [out] A placeholder for the font name that should be used for displaying list box items.
    \param fontStyle    [out] A placeholder for the font style that should be used for displaying list box items.
    
    \sa
    <link OdPdfPublish::Text::StorageType, Text::StorageType enumeration>
    <link OdPdfPublish::Text::StandardFontsType, Text::StandardFontsType enumeration>
    <link OdPdfPublish::Text::FontStyle, Text::FontStyle enumeration>
  */
  void getFont(Text::StorageType& storageType, Text::StandardFontsType& fontType, OdString& fontName, Text::FontStyle& fontStyle) const;

  /** \details
    Returns the current font size used for displaying list box items.
    The method fills a passed integer value with the list box font size and returns it to a calling subroutine.
    
    \param size [out] A placeholder for the font size that should be used for displaying list box items.
  */
  void getFontSize(OdUInt16& size) const;

  /** \details
    Returns the current text color used for displaying list box items.
    The method fills a passed ODCOLORREF value with the list box caption color and returns it to a calling subroutine.
    
    \param color [out] A placeholder for the text color that should be used for displaying list box items.
  */
  void getTextColor(ODCOLORREF& color) const;

  /** \details
    Returns the current tooltip text for the list box.
    Tooltip text is text that appears when a user hovers the mouse pointer over the list box.
    
    The method fills a passed string object with the list box tooltip text and returns it to a calling subroutine.
    
    \param tooltip [out] A placeholder for the tooltip text.
  */
  void getTooltip(OdString& tooltip) const;

  /** \details
    Returns the current value of the visibility flag for the list box.
    The visibility flag determines whether the list box is visible in a document (if true) or is hidden (if false).
    The method fills a passed boolean value with the list box visibility flag value and returns it to a calling subroutine.    
    
    \param state [out] A placeholder for the flag value.
  */
  void getVisibility(bool& state) const;

  /** \details
    Returns the current value of the flag that determines whether the list box should be printed.
    If the list box should be printed, the flag value is equal to true, otherwise it is equal to false.
    The method fills a passed boolean value with the list box printability flag value and returns it to a calling subroutine.
    
    \param state [out] A placeholder for the flag value.
  */
  void getPrintability(bool& state) const;

  /** \details
    Returns the current text rotation for the list box.
    The method fills a passed value with the current text rotation value and returns it to a calling subroutine.
    
    \param rotation [out] A placeholder for the text rotation value.
    
    \sa
    <link OdPdfPublish::Text::Rotation, Text::Rotation enumeration>.
  */
  void getTextRotation(Text::Rotation& rotation) const;

  /** \details
    Returns the current value of the flag that determines whether the list box is locked.
    If the flag value is equal to true, the list box is locked and becomes disabled, otherwise it is enabled and unlocked.
    The method fills a passed boolean value with the list box lock flag value and returns it to a calling subroutine.
    
    \param state [out] A placeholder for the flag value.
  */
  void getLock(bool& state) const;

  /** \details
    Returns the current value of the flag that determines whether the list box has a border.
    If the flag value is equal to true, the list box has a border, otherwise it does not.
    The method fills a passed boolean value with the list box border flag value and returns it to a calling subroutine.
    
    \param state [out] A placeholder for the flag value.
  */
  void getBorder(bool& state) const;

  /** \details
    Returns the current list box border color.
    The method fills a passed ODCOLORREF value with the list box border color value and returns it to a calling subroutine.
    
    \param color [out] A placeholder for the color value.
  */
  void getBorderColor(ODCOLORREF& color) const;

  /** \details
    Returns the current list box border thickness.
    The method fills a passed value with the current list box border thickness and returns it to a calling subroutine.
    
    \param thickness [out] A placeholder for the border thickness value.
    
    \sa
    <link OdPdfPublish::Border::Thickness, Border::Thickness enumeration>.
  */
  void getBorderThickness(Border::Thickness& thickness) const;

  /** \details
    Returns the current list box border style.
    The method fills a passed value with the current list box border style and returns it to a calling subroutine.
    
    \param style [out] A placeholder for the border style value.
    
    \sa
    <link OdPdfPublish::Border::Style, Border::Style enumeration>.
  */
  void getBorderStyle(Border::Style& style) const;

  /** \details
    Returns the current list box fill color.
    The method fills a passed ODCOLORREF value with the list box fill color value and returns it to a calling subroutine.
    
    \param color [out] A placeholder for the color value.
  */
  void getFillColor(ODCOLORREF& color) const;

  /** \details
    Returns the current multiple selection flag of the list box.
    The multiple selection flag determines whether more than one item from the list box can be selected simultaneously.
    
    \param state [out] A placeholder for the multiple selection flag.
  */
  void getMultipleSelection(bool& state) const;

  /** \details
    Returns the list box content items.
    The method fills passed string arrays for displaying and exporting values of the list box and returns them to a calling subroutine.
    
    \param displayed_values [out] A placeholder for displayed values of the list box items.
    \param export_values    [out] A placeholder for exported values of the list box items.
  */
  void getContents(OdStringArray& displayed_values, OdStringArray& export_values) const;

};

/** \details
  A data type that represents a smart pointer to an <link OdPdfPublish::OdListBox, OdListBox> object.
*/
SMARTPTR(OdListBox);

}
#endif // _PDF_PUBLISH_LISTBOX_INCLUDED_

