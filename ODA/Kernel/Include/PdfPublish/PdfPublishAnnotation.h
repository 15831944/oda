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
// PdfPublishAnnotation.h
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _PDF_PUBLISH_ANNOTATION_INCLUDED_
#define _PDF_PUBLISH_ANNOTATION_INCLUDED_

#include "PdfPublishBaseObject.h"
#include "PdfPublishArtwork.h"
#include "PdfPublishImage.h"
#include "PdfPublishModel.h"
#include "PdfPublishListBox.h"
#include "PdfPublishTextField.h"
#include "UInt32Array.h"
#include "StringArray.h"
#include "DoubleArray.h"
 

/** \details
  Contains declarations for Publish SDK.
  <group TD_Namespaces>
*/
namespace OdPdfPublish{

/** \details
  This class implements an annotation entity for .pdf documents created with Publish SDK.
  An annotation is a user space object that stores a set of objects of different types (acts like a container for other objects and controls).
  Annotations can contain compressed and uncompressed .prc tessellations, .prc boundary representation data, .prc attributes, and carousel buttons 
  which allow users to interactively switch between different views. An annotation entity is based on a <link OdPdfPublish::OdCADModel, CAD model>. 
  The <link OdPdfPublish::OdCADModel, CAD model> represents any drawing in a format supported by ODA Software (for example, a .dwg drawing), exported to PRC data format.
*/
class PDFPUBLISH_TOOLKIT OdAnnotation : public OdObject
{
  //DOM-IGNORE-BEGIN
protected:
  OdAnnotation();
  //DOM-IGNORE-END
public:
  //DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdAnnotation);
  ODRX_HEAP_OPERATORS();
  //DOM-IGNORE-END
  
  /** \details
    Destroys an annotation object.
  */
  virtual ~OdAnnotation();

  /** \details
    Sets the annotation source.
    The source contains drawing data that should be exported to the annotation.
    
    \param cad_model [in] A smart pointer to an <link OdPdfPublish::OdCADModel, OdCADModel> object to be set as the annotation source.
    
    \sa
    <link OdPdfPublish::OdCADModel, OdCADModel>
  */
  void setSource(const OdCADModelPtr& cad_model);
  
  /** \details
    Sets the annotation name.
    
    \param name [in] A name value.

    \remarks
    The name is mandatory and must be unique among the document object names.
  */  
  void setName(const OdString& name);
  
  /** \details
    Sets an <link OdPdfPublish::OdArtwork, artwork> object of the annotation.
    
    \param artwork [in] An <link OdPdfPublish::OdArtwork, artwork> object.
  */  
  void setArtwork(const OdArtworkPtr& artwork);

  /** \details
    Sets the visibility flag of the annotation toolbar.
    If the flag is true, the toolbar is diplayed when the annotation becomes active, otherwise the toolbar stays hidden.
    
    \param state [in] A visibility flag value.
  */  
  void setToolbarVisibility(bool state);
  
  /** \details
    Sets the visibility flag of the annotation model tree.
    If the flag is true, the model tree is diplayed when the annotation becomes active, otherwise the model tree stays hidden.
    
    \param state [in] A visibility flag value.
  */
  void setModelTreeVisibility(bool state);
  
  /** \details
    Sets the interactivity flag of the annotation.
    If the flag is true, annotation interactivity is enabled, otherwise interactivity is disabled.
    
    \param state [in] An interactivity flag value.
  */
  void setInteractivity(bool state);
  
  /** \details
    Sets the border width of the annotation.
    Border width is measured in points.
    
    \param width [in] A border width value (in points).
  */
  void setBorderWidth(OdUInt16 width);
  
  /** \details
    Sets the transparency flag of the annotation.
    If the flag is true, the annotation background is transparent, otherwise the annotation background is opaque.
    
    \param state [in] A transparency flag value.
  */
  void setTransparentBackground(bool state);
  
  /** \details
    Sets the annotation poster <link OdPdfPublish::OdImage, image>.
    
    \param image [in] A smart pointer to an <link OdPdfPublish::OdImage, image> object that should be used as the poster.
    
    \sa 
    <link OdPdfPublish::OdImage, OdImage class>.
  */
  void setPosterImage(const OdImagePtr& image);

  /** \details
    Sets the annotation activation type.
    The activation type determines when the annotation becomes active.
    
    \param when [in] An activation type value.
    
    \sa 
    <link OdPdfPublish::Activation::When, Activation::When enumeration>.
  */
  void setActivation(Activation::When when);
  
  /** \details
    Sets the annotation deactivation type.
    The deactivation type determines when the annotation becomes inactive.
    
    \param when [in] A deactivation type value.
    
    \sa
    <link OdPdfPublish::Deactivation::When, Deactivation::When enumeration>.
  */
  void setDeactivation(Deactivation::When when);

  /** \details
    Sets the annotation carousel buttons.

    \param button_names         [in] An array of button names.
    \param previous_button_name [in] A name for the previous button.
    \param next_button_name     [in] A name for the next button.
    \param scroll_size          [in] A scroll size value.
    
    \remarks
    
    A carousel buttons array includes a collection of buttons to switch between different views (each button contains a small preview) 
    and two specific buttons for switching to a previous or next view relative to the view that is currently displayed.
    View indices and small previews are set using the setCarouselViews function.
    The scroll size property determines how many buttons are scrolled when the user presses the next or previous button.
    
    It is necessary to set carousel buttons to generate a carousel view.
  */
  void setCarouselButtons(const OdStringArray& button_names, const OdString& previous_button_name, const OdString& next_button_name, OdUInt16 scroll_size);
  
  /** \details
    Sets the carousel views of the annotation.

    \param indices [in] An array of view indices.
    \param images  [in] An array of smart pointers to image objects that are used as button preview images.
    
    \remarks
    A carousel view array includes a collection of views that represent different points of view to a published drawing. 
    The number of view indices and image must be equal to the number of button names set by the setCarouselButtons function.
    To switch from one view to another, a set of carousel buttons is used. Each button (except the previous and next buttons) is associated with a particular view. 
  */
  void setCarouselViews(const OdUInt32Array& indices, const OdImagePtrArray& images);
  
  /** \details
    Sets the images for carousel scroll buttons.

    \param previous_button [in] A smart pointer to image objects that are used as previous button image.
    \param next_button  [in] A smart pointer to image objects that are used as next button image.
  */
  void setCarouselScrollButtonsImages(const OdImagePtr& previous_button, const OdImagePtr& next_button);

  /** \details
    Sets a color that is used for filling the carousel buttons.

    \param color [in] A new value of the fill color.
  */
  void setCarouselButtonsFillColor(const ODCOLORREF color);

  /** \details
    Sets a offset between annotations and the carousel buttons.

    \param color [in] A new value of the offset.
  */
  void setCarouselButtonsOffset(const OdUInt32 value);

  /** \details
    Sets the carousel buttons font name and style.

    \param fontName  [in] A new font name.
    \param fontStyle [in] A new font style.

    \remarks
    This method can set a font that is not represented in the list of
    <link OdPdfPublish::Text::StandardFontsType, standard fonts>.
    By default the <link OdPdfPublish::Text::StandardFontsType::kTimesRoman, normal Times New Roman font> is used.

    \sa
    <link OdPdfPublish::Text::FontStyle, Text::FontStyle enumeration>
    <link OdPdfPublish::Text::StandardFontsType, StandardFontsType enumeration>
  */
  void setCarouselButtonsFont(const OdString& fontName, Text::FontStyle fontStyle = Text::kRegular);

  /** \details
    Sets the carousel buttons font type from the list of <link OdPdfPublish::Text::StandardFontsType,  predefined fonts>.

    \param fontType [in] A new font type value.

    \sa
    <link OdPdfPublish::Text::StandardFontsType, Text::StandardFontsType enumeration>.
  */
  void setCarouselButtonsFont(const Text::StandardFontsType fontType);

  /** \details
    Sets the carousel buttons font size.

    \param size [in] A new font size value.
  */
  void setCarouselButtonsFontSize(OdUInt16 size);

  /** \details
    Creates a new list of view names combined in a single string.

    \param field [out] A string containing view names.

    \remarks 
    The name is mandatory and must be unique among the document object names.
  */
  void setViewListByField(const OdString& field);
  
  /** \details
    Adds a new list box object in a specified area and creates a list of view names in it.

    \param location [out] A list box location.
    \param list		  [out] A list box object.
  */
  void setViewList(const OdRect& location, const OdListBoxPtr& list);
  
  /** \details 
  Sets the table that contains the list of annotation parts. 
  
  \param node_names   [in] A string array that contains the distinct node names.
  \param location     [in] A location of the parts list table. 
  \param headers      [in] The parts list table header titles (number, name, quantity). If the array is empty, the table will not have headers. 
  \param columns      [in] A double array that contains column width proportions. The sum of proportion values must be equal to 1.0. 
  \param text_style   [in] A text field object that is used to represent table cells. It means that cells will have the same text style as the specified text field object. 
  \param header_style  [in] A text field object that is used to represent column headers. It means that the headers will have the same text style as the specified text field object. 
  
  \remarks 
  A user can scroll the list parts table. The table consists of three columns: 
  * Part number
  * Part name 
  * Quantity of the part appearence in the annotation
  
  The annotation must have an OdCADModel object as its source.  
  The OdCADModel object must contain data in PRC format. 
  
  The columns parameter allows you to specify how the whole table width should be divided between three columns (number, name, quantity). 
  For example if the column array contains values 0.1, 0.75 and 0.15, the number column occupies 10%, 
  name column occupies 75%, and quantity column occupies 15% of the table width respectively. 
  */  
  void setPartsList(const OdStringArray node_names, const OdRect& location, const OdStringArray& headers, const OdDoubleArray& columns, 
    const OdTextFieldPtr text_style = OdTextFieldPtr(), const OdTextFieldPtr header_style = OdTextFieldPtr());

  /** \details
    Creates a new OdList object in a specified location and fills it with property values of the node 
    selected at the annotation object. If the node has no property, the list object remains empty.
    \param location [in] A rectangle that determines the location.
  */
  void setPropertyList(const OdRect& location);

  /** \details
    Displays the property value of the selected node to a specified text field. 
    The property to be displayed is defined by its name.
    \param property_name [in] A property name to be displayed.
    \param text_fields   [in] A smart pointer to a text field that displays the property value.
    \remarks 
    If the selected node of the annotation does not have a property with the specified name, the text box object remains empty.
  */
  void setPropertyToTextField(const OdString& property_name, const OdTextFieldPtr text_fields);

  /** \details
    Returns the annotation source.
    The method fills a passed smart pointer to the model object and returns it to a calling subroutine.
    
    \param cad_model [out] A placeholder for the smart pointer to the model.    
    
    \remarks
    The source contains drawing data that has been exported to the annotation.
    
    \sa
    <link OdPdfPublish::OdCADModel, OdCADModel class>.
  */
  void getSource(OdCADModelPtr& cad_model) const;
  
  /** \details
    Returns the annotation name.
    The method fills a passed string object and returns it to a calling subroutine.
    
    \param name [out] A placeholder for the annotation name.
  */
  void getName(OdString& name) const;
  
  /** \details
    Returns a smart pointer to the annotation <link OdPdfPublish::OdArtwork, artwork> object.
    The method fills a passed smart pointer value and returns it to a calling subroutine.
    
    \param artwork [out] A placeholder for the smart pointer to the annotation artwork.
    
    \sa
    <link OdPdfPublish::OdArtwork, OdArtwork class>.
  */
  void getArtwork(OdArtworkPtr& artwork) const;

  /** \details
    Returns the current value of the visibility flag of the annotation toolbar.
    The method fills a passed boolean value and returns it to a calling subroutine.
    
    \param state [out] A placeholder for the visibility flag value.
    
    \remarks
    If the flag is true, the toolbar is diplayed when the annotation becomes active, otherwise the toolbar stays hidden.
  */  
  void getToolbarVisibility(bool& state) const;
  
  /** \details
    Returns the current value of the visibility flag of the annotation model tree.
    The method fills a passed boolean value and returns it to a calling subroutine.
    
    \param state [out] A placeholder for the visibility flag value.
    
    \remarks
    If the flag is true, the model tree is diplayed when the annotation becomes active, otherwise the model tree stays hidden.
  */
  void getModelTreeVisibility(bool& state) const;
  
  /** \details
    Returns the current value of the interactivity flag of the annotation.
    The method fills a passed boolean value and returns it to a calling subroutine.

    \param state [out] A placeholder for the interactivity flag value.
    
    \remarks
    If the flag is true, the annotation interactiviy is enabled, otherwise interactivity is disabled.
  */
  void getInteractivity(bool& state) const;
  
  /** \details
    Returns the current border width of the annotation.
    The method fills a passed value and returns it to a calling subroutine.
    
    \param width [out] A placeholder for the border width value (in points).
    
    \remarks 
    Border width is measured in points.
  */
  void getBorderWidth(OdUInt16& width) const;
  
  /** \details
    Returns the current value of the transparency flag of the annotation.
    The method fills a passed boolean value and returns it to a calling subroutine.
    
    \param state [out] A placeholder for the transparency flag value.    
    
    \remarks 
    If the flag is true, the annotation background is transparent, otherwise the annotation background is opaque.
  */
  void getTransparentBackground(bool& state) const;
  
  /** \details
    Returns the annotation poster image.
    The method fills a passed smart pointer to an <link OdPdfPublish::OdImage, image> object and returns it to a calling subroutine.
    
    \param image [out] A placeholder for the smart pointer to an image object.
    
    \sa 
    <link OdPdfPublish::OdImage, OdImage class>.
  */
  void getPosterImage(OdImagePtr& image) const;

  /** \details
    Returns the annotation activation type.
    The method fills a passed activation type value and returns it to a calling subroutine.
    
    \param when [out] A placeholder for the activation type value.
    
    \remarks
    The activation type determines when the annotation becomes active.
    
    \sa
    <link OdPdfPublish::Activation::When, Activation::When enumeration>.
  */
  void getActivation(Activation::When& when) const;
  
  /** \details
    Returns the annotation deactivation type.
    The method fills a passed deactivation type value and returns it to a calling subroutine.

    \param when [out] A placeholder for the deactivation type value.
    
    \remarks
    The dectivation type determines when the annotation becomes inactive.
    
    \sa 
    <link OdPdfPublish::Deactivation::When, Deactivation::When enumeration>.
  */
  void getDeactivation(Deactivation::When& when) const;

  /** \details
    Returns the annotation carousel buttons information.
    The information contains button names, including previous and next button names, and scroll size value.
    The method fills the passed objects with button information and returns them to a calling subroutine.
    
    \param button_names         [out] A placeholder for the array of button names.
    \param previous_button_name [out] A placeholder for the name of the previous button.
    \param next_button_name     [out] A placeholder for the name of the next button.
    \param scroll_size          [out] A placeholder for the scroll size value.
    
    \remarks
    A carousel buttons array includes a collection of buttons to switch between different views (each button contains a small preview) 
    and two specific buttons for switching to a previous or next view relative to the view that is currently displayed. 
    The scroll size property determines how many buttons are scrolled when the user presses the next or previous button.
    
    It is necessary to set carousel buttons to generate a carousel view.
  */
  void getCarouselButtons(OdStringArray& button_names, OdString& previous_button_name, OdString& next_button_name, OdUInt16& scroll_size) const;
  
  /** \details
    Returns the carousel views of the annotation.
    The view information contains an array of view indices and an array of button icons.
    The method fills the passed objects with data about the views and returns them to a calling subroutine.

    \param indices [out] A placeholder for the array of view indices.
    \param images   [out] A placeholder for the array of smart pointers to the image objects that are used as button preview images.
    
    \remarks
    A carousel view array includes a collection of views that represent different points of view to a published drawing.
    To switch from one view to another, a set of carousel buttons is used. Each button (except the previous and next buttons) is associated with a particular view. 
  */
  void getCarouselViews(OdUInt32Array& indices, OdImagePtrArray& images) const;
  
  /** \details
    Returns the images for carousel scroll buttons.

    \param previous_button [out] A placeholder for the smart pointer to image objects that are used as previous button image.
    \param next_button  [out] A placeholder for the smart pointer to image objects that are used as next button image.
  */
  void getCarouselScrollButtonsImages(OdImagePtr& previous_button, OdImagePtr& next_button) const;

  /** \details
    Returns a color that is used for filling the carousel buttons.

    \param color [out] A placeholder for the value of the fill color.
  */
  void getCarouselButtonsFillColor(ODCOLORREF& color) const;

  /** \details
    Returns a offset between annotations and the carousel buttons.

    \param color [out] A placeholder for the value of the offset.
  */
  void getCarouselButtonsOffset(OdUInt32& value) const;

  /** \details
    Returns the current carousel buttons font parameters.
    The method fills passed values and returns them to a calling subroutine.

    \param storageType  [out] A placeholder for the storage type.
    \param fontType     [out] A placeholder for the font type.
    \param fontName     [out] A placeholder for the font name.
    \param fontStyle    [out] A placeholder for the font style.

    \sa
    <link OdPdfPublish::Text::StorageType, Text::StorageType enumeration>
    <link OdPdfPublish::Text::StandardFontsType, Text::StandardFontsType enumeration>
    <link OdPdfPublish::Text::FontStyle, Text::FontStyle enumeration>
  */
  void getCarouselButtonsFont(Text::StorageType& storageType, Text::StandardFontsType& fontType, OdString& fontName, Text::FontStyle& fontStyle) const;

  /** \details
    Returns the carousel buttons font size.
    The method fills a passed value and returns it to a calling subroutine.

    \param size [out] A placeholder for the font size value.
  */
  void getCarouselButtonsFontSize(OdUInt16& size) const;

  /** \details
    Returns a list of view names combined in a single string.
    The method fills the passed string objects with view names and returns it to a calling subroutine.

    \param field [out] A placeholder for the string containing view names.
  */
  void getViewListField(OdString& field) const;

  /** \details
    Returns the list box object that contains the list of view names.
    The method fills the passed location and list box objects, then returns them to a calling subroutine.

    \param location [out] A placeholder for the list box location.
    \param list		[out] A placeholder for the list box object.
  */
  void getViewList(OdRect& location, OdListBoxPtr& list) const;
  
  /** \details 
  Returns the table that contains the list of annotation parts. 
  The method accepts placeholders for the parts list table, fills them and returns to a calling subroutine.
  
  \param node_names   [out] A placeholder for the array of node names.
  \param location     [out] A placeholder for the location of the parts list table. 
  \param headers      [out] A placeholder for the header titles (number, name, quantity). If the array is empty, the table will not have headers. 
  \param columns      [out] A placeholder for the column width proportions. The sum of proportion values must be equal to 1.0. 
  \param text_style   [out] A placeholder for the text styles of the table cells.
  \param header_style  [out] A placeholder for the column header styles.
  
  \remarks 
  A user can scroll the list parts table. The table consists of three columns: 
  * Part number
  * Part name 
  * Quantity of the part appearence in the annotation
  
  The annotation must have an OdCADModel object as its source.  
  The OdCADModel object must contain data in PRC format. 
  
  The columns parameter allows you to specify how the whole table width should be divided between three columns (number, name, quantity). 
  For example, if the column array contains values 0.1, 0.75 and 0.15, the number column occupies 10%, 
  name column occupies 75%, and quantity column occupies 15% of the table width respectively. 
  */
  void getPartsList(OdStringArray& node_names, OdRect& location, OdStringArray& headers, OdDoubleArray& columns, 
    OdTextFieldPtr& text_style, OdTextFieldPtr& header_style) const;

  /** \details
  Returns the current location of the property list label.
  \param location [in] A placehloder for the property list location.
  */
  void getPropertyList(OdRect& location) const;

  /** \details
  Returns the current text field property with a specified name.
  \param property_name [in] A property name.
  \param text_field   [in] A smart pointer to a text field array.
  */
  void getPropertyToTextField(const OdString& property_name, OdTextFieldPtrArray& text_field) const;
};

/** \details
  A data type that represents a smart pointer to an <link OdPdfPublish::OdAnnotation, OdAnnotation> object.
*/
SMARTPTR(OdAnnotation);

}
#endif // _PDF_PUBLISH_ANNOTATION_INCLUDED_

