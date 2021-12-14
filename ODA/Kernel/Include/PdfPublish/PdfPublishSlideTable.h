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
// PdfPublishSlideTable.h
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _PDF_PUBLISH_SLIDETABLE_INCLUDED_
#define _PDF_PUBLISH_SLIDETABLE_INCLUDED_

#include "PdfPublishBaseObject.h"
#include "PdfPublishTextField.h"
#include "DoubleArray.h"

/** \details
  Contains declarations for Publish SDK.
  <group TD_Namespaces>
*/
namespace OdPdfPublish{

/** \details
  This class implements a slide table entity for .pdf documents created with Publish SDK.
  A slide table contains a scrollbar (by default) or two specific buttons (if their names are specified with the <link OdPdfPublish::OdSlideTable::setButtons@OdString&@OdString&, setButtons() method>):
* Next button. When a user presses this button, the table content is scrolled one row downwards.
* Previous button. When a user presses this button, the the table content is scrolled one row upwards.
*/
class PDFPUBLISH_TOOLKIT OdSlideTable : public OdObject
{
  //DOM-IGNORE-BEGIN
protected:
  OdSlideTable();
  //DOM-IGNORE-END
public:
  //DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdSlideTable);
  ODRX_HEAP_OPERATORS();
  //DOM-IGNORE-END
 
  /** \details
    Destroys a slide table object.
  */
  virtual ~OdSlideTable();

  /** \details 
    Sets column width proportions and row height for the slide table.
    
    \param column_proportions [in] An array that contains column width proportions. The array size should match the count of the slide table columns.
    \param row_height         [in] A new row height value.
    
    \remarks
    The column_proportions parameter defines the width of columns in proportional units with a range between 0.0 and 1.0.
    The sum of column width proportions must be equal to 1.0.
    The size of the slide table is determined by the location rectangle passed to the <link OdPdfPublish::OdPage::addSlideTable@OdSlideTablePtr&@OdRect&, addSlideTable()> method of the 
    <link OdPdfPublish::OdPage> page object. 
  */
  void setDimensions(const OdDoubleArray& column_proportions, const OdUInt32 row_height);  

  /** \details
    Sets HTML markup source code for the slide table.
    The HTML markup source code is represented with a string in UTF8 format.
        
    \param source [in] New HTML markup source code.
    
    \remarks
    This functionality is not implemented yet.
  */
  void setHTML(const OdString& source);

  /** \details
    Sets style options for the HTML markup of the slide table.
    The HTML markup style options are represented with a string in UTF8 format.
        
    \param source [in] New HTML markup style options.
    
    \remarks
    This functionality is not implemented yet.
  */
  void setHTMLStyle(const OdString& source);

  /** \details
    Sets special buttons for sliding rows upwards and downwards in the slide table.
        
    \param previous_button_name [in] A name for the Previous button that slides rows upwards.
    \param next_button_name     [in] A name for the Next button that slides rows downwards.
    
    \remarks
    The method allows you to set only names of buttons and not captions (text strings, which are displayed at buttons). 
    If you set Next and Previous buttons names, scrolling is implemented with these buttons. 
    If the names of buttons are empty (by default), scrolling is implemented with a scrollbar.
  */
  void setButtons(const OdString& previous_button_name, const OdString& next_button_name);

  /** \details
    Adds text fields for slide table cells.
        
    \param rows     [in] A quantity of slide table rows that should be filled with text fields.
    \param columns  [in] A quantity of slide table columns that should be filled with text fields.
    \param text     [in] An array of smart pointers to text fields that should be added to the slide table cells.
    
    \remarks
    The text for the table is specified with an array of <link OdPdfPublish::OdTextField, text fields>, which contain text and styles for appropriate table cells.
    The quantity of the text field array elements must be equal to the product of the rows and columns parameter values. 
  */
  void setText(const OdUInt32 rows, const OdUInt32 columns, const OdTextFieldPtrArray& text);

  /** \details
    Sets a header flag value. The header flag determines whether the slide table has a header (if set to true).
        
    \param state [in] A new value of the header flag.
  */
  void setHeader(bool state);

  /** \details 
    Returns the current column width proportions and row height for the slide table.
    The method fills the passed double array and integer value with the current column width proportions and row height value, 
    then returns them to a calling subroutine. 
    
    \param column_proportions [out] A placeholder for column width proportions.
    \param row_height         [out] A placeholder for the row height value.
    
    \remarks
    The column_proportions parameter defines the width of columns in proportional units with a range between 0.0 and 1.0.
    The sum of column width proportions must be equal to 1.0.
    The size of the slide table is determined by the location rectangle passed to the <link OdPdfPublish::OdPage::addSlideTable@OdSlideTablePtr&@OdRect&, addSlideTable()> method of the 
    <link OdPdfPublish::OdPage> page object. 
  */
  void getDimensions(OdDoubleArray& column_proportions, OdUInt32& row_height) const;

  /** \details
    Returns the current HTML markup source code for the slide table.
    The HTML markup source code is represented with a string in UTF8 format.
    The method fills a passed string object with HTML source code and returns it to a calling subroutine.
        
    \param source [out] A placeholder for the HTML markup source code.
    
    \remarks
    This functionality is not implemented yet.
  */
  void getHTML(OdString& source) const;

  /** \details
    Returns the current style options for the HTML markup of the slide table.
    The HTML markup style options are represented with a string in UTF8 format.
    The method fills a passed string object and returns it to a calling subroutine.
        
    \param source [in] A placeholder for the HTML markup style options.
    
    \remarks
    This functionality is not implemented yet.
  */
  void getHTMLStyle(OdString& source) const;

  /** \details
    Returns the current names of the previous and next buttons contained in the slide table.
    The method fills the passed string objects with previous and next button names and returns them to a calling subroutine.
        
    \param previous_button_name [out] A placeholder for the previous button name.
    \param next_button_name     [out] A placeholder for the next button name.
  */
  void getButtons(OdString& previous_button_name, OdString& next_button_name) const;

  /** \details
    Returns the text fields of the slide table.
    The method fills the passed values with the quantity of rows and columns and adds text field smart pointers into a passed array, 
    then returns the filled values and array to a calling subroutine.
        
    \param rows     [out] A placeholder for the quantity of slide table rows.
    \param columns  [out] A placeholder for the quantity of slide table columns.
    \param text     [out] A placeholder for smart pointers to text fields.
    
    \remarks
    The method fills the passed array of <link OdPdfPublish::OdTextField, text fields> that contain text and styles for the appropriate table cells.
    The quantity of the text field array elements is equal to the product of the rows and columns parameter values. 
  */
  void getText(OdUInt32 & rows, OdUInt32 & columns, OdTextFieldPtrArray& text) const;

  /** \details
    Returns the current header flag value. The header flag determines whether the slide table has a header (if set to true).
    The method fills a passed boolean value with a header flag value and returns it to a calling subroutine.
        
    \param state [in] A placeholder for the header flag value.
  */
  void getHeader(bool& state) const;

};

/** \details
  A data type that represents a smart pointer to an <link OdPdfPublish::OdSlideTable, OdSlideTable> object.
*/
SMARTPTR(OdSlideTable);

}
#endif // _PDF_PUBLISH_SLIDETABLE_INCLUDED_

