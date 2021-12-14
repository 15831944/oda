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
// OdTableCreator.h
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _PDF_PUBLISH_TABLE_CREATOR_INCLUDED_
#define _PDF_PUBLISH_TABLE_CREATOR_INCLUDED_

#include "PdfPublishCommon.h"
#include "PdfPublishTable.h"
#include "UInt32Array.h"
#include "DoubleArray.h"
#define STL_USING_MAP
#include "OdaSTL.h"

/** \details
  Contains declarations for Publish SDK.
  <group TD_Namespaces>
*/
namespace OdPdfPublish{

//DOM-IGNORE-BEGIN
  class OdTableCreatorImpl;
//DOM-IGNORE-END

/** \details
  This class implements the table creator interface for Publish SDK.
  The table creator is used for creating <link OdPdfPublish::OdTable, tables> with specified parameters.
*/
class PDFPUBLISH_TOOLKIT OdTableCreator : public OdRxObject
{
  
//DOM-IGNORE-BEGIN
  friend class OdTableCreatorImpl;
  OdTableCreatorImpl* m_pImpl;
protected:
  OdTableCreator();
//DOM-IGNORE-END

public:

  //DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdTableCreator);
  ODRX_HEAP_OPERATORS();
  //DOM-IGNORE-END
  
  
  /** \details
    Determines whether the table creator object is empty.
    \returns Returns true if the object is empty, otherwise returns false.
  */
  bool isEmpty() const;
  
  /** \details
    Clears the table creator object.
  */
  void clear();
  
  /** \details
    Determines whether the table creator object is valid.
    \returns Returns true if the object is valid, otherwise returns false.
  */
  bool isValid() const;

  /** \details
    Destroys the table creator object.
  */
  virtual ~OdTableCreator();

  /** \details
    Sets the column count for the table to be created.
    \param column_count [in] A column count.
    
    \remarks
    By default, all table columns have the same width.
  */
  void setColumnCount(const OdUInt32 column_count);
  
  /** \details
    Sets the row count for the table to be created.
    \param row_count [in] A row count.
    
    \remarks
    By default, all table rows have the same height.
  */
  void setRowCount(const OdUInt32 row_count);

  /** \details
    Sets a new width for a particular table column.
    \param col          [in] A column index (from 0 to column count -1).
    \param column_width [in] A column width value.
  */
  void setParticularColumnWidth(const OdUInt32 col, const OdUInt32 column_width);

  /** \details
    Sets a new height for a particular table row.
    \param row        [in] A row index (from 0 to row count -1).
    \param row_height [in] A row height value.
  */
  void setParticularRowHeight(const OdUInt32 row, const OdUInt32 row_height);

  /** \details
    Sets a new color for a table cell.
    \param row          [in] A row index (from 0 to row count -1).
    \param column       [in] A column index (from 0 to column count -1).
    \param fill_color   [in] A new cell color.
  */
  void setCellFillColor(const OdUInt32 row, const OdUInt32 column, const ODCOLORREF fill_color);

  /** \details
    Sets a new border color for a table cell.
    \param row          [in] A row index (from 0 to row count -1).
    \param column       [in] A column index (from 0 to column count -1).
    \param border_color [in] A new cell border color.
  */
  void setCellBorderColor(const OdUInt32 row, const OdUInt32 column, const ODCOLORREF border_color);
  
  /** \details
    Sets a new border thickness for a table cell.
    \param row              [in] A row index (from 0 to row count -1).
    \param column           [in] A column index (from 0 to column count -1).
    \param border_thickness [in] A new cell border thickness.
  */
  void setCellBorderThickness(const OdUInt32 row, const OdUInt32 column, const Border::Thickness border_thickness);

  /** \details
    Returns the current column count for the table to be created.
    \param column_count [out] A placeholder for the column count.
    \remarks 
    The method accepts a passed value, fills it and returns it to a calling subroutine.
  */
  void getColumnCount(OdUInt32& column_count) const;
  
  /** \details
    Returns the current row count for the table to be created.
    \param row_count [out] A placeholder for the row count.
    \remarks 
    The method accepts a passed value, fills it and returns it to a calling subroutine.
  */
  void getRowCount(OdUInt32& row_count) const;
  
  /** \details
    Returns the current width for a particular table column.
    \param col          [in] A column index (from 0 to column count -1).
    \param column_width [out] A placeholder for the column width value.

    \returns Returns the width of the specified column.

    \remarks 
    The method accepts a passed value, fills it and returns it to a calling subroutine.
  */
  bool getParticularColumnWidth(const OdUInt32 col, OdUInt32& column_width) const;
  
  /** \details
    Returns the current height for a particular table row.
    \param row        [in] A row index (from 0 to row count -1).
    \param row_height [out] A placeholder for the row height value.

    \returns Returns the height of the specified row.
    \remarks 
    The method accepts a passed value, fills it and returns it to a calling subroutine.
  */
  bool getParticularRowHeight(const OdUInt32 row, OdUInt32& row_height) const;
  
  /** \details
    Returns the current color of a table cell.
    \param row          [in] A row index (from 0 to row count -1).
    \param column       [in] A column index (from 0 to column count -1).
    \param fill_color   [out] A placeholder for the cell color.
    \remarks 
    The method accepts a passed color value, fills it and returns it to a calling subroutine.
  */
  void getCellFillColor(const OdUInt32 row, const OdUInt32 column, ODCOLORREF& fill_color) const;
  
  /** \details
    Returns the current border color of a table cell.
    \param row          [in] A row index (from 0 to row count -1).
    \param column       [in] A column index (from 0 to column count -1).
    \param border_color [out] A placeholder for the cell border color.
    \remarks 
    The method accepts a passed color value, fills it and returns it to a calling subroutine.
  */
  void getCellBorderColor(const OdUInt32 row, const OdUInt32 column, ODCOLORREF& border_color) const;
  
  /** \details
    Returns the current border thickness for a table cell.
    \param row              [in] A row index (from 0 to row count -1).
    \param column           [in] A column index (from 0 to column count -1).
    \param border_thickness [out] A placeholder for the cell border thickness.
    \remarks 
    The method accepts a passed border thickness value, fills it and returns it to a calling subroutine.
  */
  void getCellBorderThickness(const OdUInt32 row, const OdUInt32 column, Border::Thickness& border_thickness) const;

  /** \details
    Creates a new table according to options of the table creator.
    \returns Returns a smart pointer to an <link OdPdfPublish::OdTable, OdTable> object. 
  */
  OdTablePtr createTable() const;

};

/** \details
  A data type that represents a smart pointer to an <link OdPdfPublish::OdTableCreator, OdTableCreator> object.
*/
SMARTPTR(OdTableCreator);

}
#endif // _PDF_PUBLISH_TABLE_CREATOR_INCLUDED_

