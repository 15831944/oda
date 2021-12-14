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



#ifndef OD_DBFORMATTEDTABLEDATA_H
#define OD_DBFORMATTEDTABLEDATA_H

#include "DbObject.h"
#include "DbLinkedTableData.h"

#include "TD_PackPush.h"

/** \details
  This class represents FormattedTableData objects in an OdDbDatabase instance.

  \remarks
  Work in progress!

  
  <group OdDb_Classes>
*/
class DBENT_EXPORT OdDbFormattedTableData : public OdDbLinkedTableData
{
public:
  //DOM-IGNORE-BEGIN
  ODDB_DECLARE_MEMBERS(OdDbFormattedTableData);
  //DOM-IGNORE-END

  /** \details
    Default constructor. Creates an object of the OdDbFormattedTableData class.
  */
  OdDbFormattedTableData();

  /** \details
    Inserts rows at the specified index and inherits the row properties for the
    inserted rows from the reference row.

    \param nIndex       [in]  Index to insert rows at.
    \param nInheritFrom [in]  Index of the reference row to inherit the format from.
    \param nNumRows     [in]  Number of inserted rows.
    \returns Starting index of the inserted rows.
  */
  virtual OdInt32 insertRowAndInherit(
    OdInt32 nIndex, 
    OdInt32 nInheritFrom,
    OdInt32 nNumRows);

  /** \details
    Inserts columns at the specified index and inherits the column properties
    for the inserted columns from the reference column.

    \param nIndex       [in]  Index.
    \param nInheritFrom [in]  Index of the reference column to inherit the format from.
    \param nNumCols     [in]  Number of inserted columns.
    \returns Starting index of the inserted columns.
    \remarks
    Method generates the eInvalidInput exception when the number of rows or columns is zero.
  */
  virtual OdInt32 insertColumnAndInherit(
    OdInt32 nIndex, 
    OdInt32 nInheritFrom,
    OdInt32 nNumCols);

  /** \details
    Sets a field ID to the zero cell content.

    \param nRow    [in]  Row index of the cell.
    \param nCol    [in]  Column index of the cell.
    \param idField [in]  Field ID.
    \param nFlag   [in]  Cell option.

    \remarks
    Method generates the eInvalidInput exception when input row number and
    column number are less than 0 or greater than table size.
    Method generates the eIsWriteProtected when content is not editable
    (!isContentEditable()).
  */
  virtual void setFieldId(
    OdInt32 nRow, 
    OdInt32 nCol, 
    const OdDbObjectId& idField, 
    OdDb::CellOption nFlag);

  /** \details
    Sets a field ID to the specified cell content.

    \param nRow     [in]  Row index of the cell.
    \param nCol     [in]  Column index of the cell.
    \param nContent [in]  Number of content.
    \param idField  [in]  Field ID.
    \param nFlag    [in]  Cell option.

    \remarks
    Method generates the eInvalidInput exception when input row number and
    column number are less than 0 or greater than table size.
    Method generates the eIsWriteProtected when content is not editable
    (!isContentEditable()).
  */
  virtual void setFieldId(
    OdInt32 nRow, 
    OdInt32 nCol, 
    OdUInt32 nContent, 
    const OdDbObjectId& idField, 
    OdDb::CellOption nFlag);

  /** \details
    Merges a rectangular range of cells in this table object.

    \param range [in]  Input range to merge.

    \remarks
    Method generates the eInvalidInput exception when the minimum row index is
    more than the maximum row index, the minimum column index is more than the
    maximum column index, the maximum row index is more than the number of rows,
    or the maximum column index is more than the number of columns.
  */
  virtual void merge(
    const OdCellRange& range);

  /** \details
    Unmerges a rectangular range of cells in this table object.

    \param range [in]  Input range to unmerge.

    \remarks
    Method generates the eInvalidInput exception when the minimum row index is
    more than the maximum row index, the minimum column index is more than the
    maximum column index, the maximum row index is more than the number of rows,
    or the maximum column index is more than the number of columns.
  */
  virtual void unmerge(
    const OdCellRange& range);

  /** \details
    Returns true if and only if the specified cell has been merged.

    \param nRow [in]  Row index.
    \param nCol [in]  Column index.
    \returns True if the specified cell has been merged; false otherwise.
    \remarks
    Method generates the eInvalidInput exception when the row argument is more
    than the number of rows or the column argument is more than the number of
    columns.
  */
  virtual bool isMerged(
    OdInt32 nRow, 
    OdInt32 nCol) const;

  /** \details
    Returns merge range of the cell as an object of the OdCellRange class.

    \param nRow [in]  Row index of the cell.
    \param nCol [in]  Column index of the cell.
    \returns Merge range of the specified cell.
    \remarks
    Returns invalid merge range if the cell is not part of the merge range.
  */
  virtual OdCellRange getMergeRange(
    OdInt32 nRow, 
    OdInt32 nCol) const;

  /** \details
    Checks whether the merge-all flag is enabled for the cell, row, or column.

    \param nRow [in]  Row index.
    \param nCol [in]  Column index.
    \returns True if the merge-all flag is enabled or false if it is not.
    \remarks
    Method generates the eNotImplementedYet exception.
  */
  virtual bool isMergeAllEnabled(
    OdInt32 nRow, 
    OdInt32 nCol) const;
  
  /** \details
    Sets the merge-all flag for the cell, row, or column.

    \param nRow    [in]  Row index.
    \param nCol    [in]  Column index.
    \param bEnable [in]  Merge-all flag value.

    \remarks
    Method generates the eNotImplementedYet exception.
  */
  virtual void enableMergeAll(
    OdInt32 nRow, 
    OdInt32 nCol,
    bool bEnable);

  /** \details
    Returns "editing format flag" cell property.

    \param nRow [in]  Row index of the cell.
    \param nCol [in]  Column index of the cell.
    \returns True if cell format can be edited or false if not.
  */
  virtual bool isFormatEditable(
    OdInt32 nRow, 
    OdInt32 nCol) const;

  /** \details
    Returns the zero content rotation angle.

    \param nRow [in]  Row index of the cell.
    \param nCol [in]  Column index of the cell.
    \returns Zero content rotation angle value in radians.
    \remarks
    Method generates the eInvalidInput exception when input row number and
    column number are less than 0 or greater than table size.
  */
  virtual double rotation(
    OdInt32 nRow, 
    OdInt32 nCol) const;

  /** \details
    Returns the specified content rotation angle.

    \param nRow     [in]  Row index of the cell.
    \param nCol     [in]  Column index of the cell.
    \param nContent [in]  Content index.
    \returns Specified content rotation angle value in radians.
    \remarks
    Method generates the eInvalidInput exception when input row number and
    column number are less than 0 or greater than table size.
  */
  virtual double rotation(
    OdInt32 nRow, 
    OdInt32 nCol,
    OdInt32 nContent) const;

  /** \details
    Sets the zero cell content rotation angle.

    \param nRow      [in]  Row index of the cell.
    \param nCol      [in]  Column index of the cell.
    \param fRotation [in]  Rotation angle value in radians.

    \remarks
    Method generates the eInvalidInput exception when input row number and
    column number are less than 0 or greater than table size.
    Method generates the eIsWriteProtected when format is not editable
    (!isFormatEditable()).
  */
  virtual void setRotation(
    OdInt32 nRow, 
    OdInt32 nCol, 
    double fRotation);

  /** \details
    Sets the specified cell content rotation angle.

    \param nRow      [in]  Row index of the cell.
    \param nCol      [in]  Column index of the cell.
    \param nContent  [in]  Content index.
    \param fRotation [in]  Rotation angle value in radians.

    \remarks
    Method generates the eInvalidInput exception when input row number and
    column number are less than 0 or greater than table size.
    Method generates the eIsWriteProtected when format is not editable
    (!isFormatEditable()).
  */
  virtual void setRotation(
    OdInt32 nRow, 
    OdInt32 nCol, 
    OdInt32 nContent,
    double fRotation);

  /** \details
    Returns the scale value of the zero content of a cell.

    \param nRow [in]  Row index of the cell.
    \param nCol [in]  Column index of the cell.
    \returns Zero content scale value.
    \remarks
    Method generates the eInvalidInput exception when input row number and
    column number are less than 0 or greater than table size.
  */
  virtual double scale(
    OdInt32 nRow, 
    OdInt32 nCol) const;

  /** \details
    Returns the scale value of the specified content of a cell.

    \param nRow     [in]  Row index of the cell.
    \param nCol     [in]  Column index of the cell.
    \param nContent [in]  Content index.
    \returns Specified content scale value.
    \remarks
    Method generates the eInvalidInput exception when input row number and
    column number are less than 0 or greater than table size.
  */
  virtual double scale(
    OdInt32 nRow, 
    OdInt32 nCol,
    OdInt32 nContent) const;

  /** \details
    Sets the scale value for the zero content of a cell.

    \param nRow   [in]  Row index of the cell.
    \param nCol   [in]  Column index of the cell.
    \param fScale [in]  Scale value.

    \remarks
    Method generates the eInvalidInput exception when input row number and
    column number are less than 0 or greater than table size.
  */
  virtual void setScale(
    OdInt32 nRow, 
    OdInt32 nCol, 
    double fScale);

  /** \details
    Sets the scale value for the specified content of a cell.

    \param nRow     [in]  Row index of the cell.
    \param nCol     [in]  Column index of the cell.
    \param nContent [in]  Content index.
    \param fScale   [in]  Scale value.

    \remarks
    Method generates the eInvalidInput exception when input row number and
    column number are less than 0 or greater than table size.
  */
  virtual void setScale(
    OdInt32 nRow, 
    OdInt32 nCol, 
    OdInt32 nContent,
    double fScale);

  /** \details
    Indicates whether the data in the zero content of a cell is auto-scaled or
    not.

    \param nRow [in]  Row index of the cell.
    \param nCol [in]  Column index of the cell.
    \returns True if the data in the zero content of the cell is auto-scaled;
    false otherwise.
  */
  virtual bool isAutoScale(
    OdInt32 nRow, 
    OdInt32 nCol) const;

  /** \details
    Indicates whether the data in the specified content of a cell is auto-scaled
    or not. Returns the auto-scaled flag as a boolean value.

    \param nRow     [in]  Row index of the cell.
    \param nCol     [in]  Column index of the cell.
    \param nContent [in]  Content index.
    \returns True if the data in the specified content of the cell is
    auto-scaled; false otherwise.
  */
  virtual bool isAutoScale(
    OdInt32 nRow, 
    OdInt32 nCol,
    OdInt32 nContent) const;

  /** \details
    Sets the auto-scaled flag to the zero content of a cell.

    \param nRow       [in]  Row index of the cell.
    \param nCol       [in]  Column index of the cell.
    \param bAutoScale [in]  Auto-scaled flag value.

    \remarks
    Method generates the eInvalidInput exception when input row number and
    column number are less than 0 or greater than table size.
    Method generates the eIsWriteProtected when format is not editable
    (!isFormatEditable()).
  */
  virtual void setAutoScale(
    OdInt32 nRow, 
    OdInt32 nCol, 
    bool bAutoScale);

  /** \details
    Sets the auto-scaled flag to the specified content of a cell.

    \param nRow       [in]  Row index of the cell.
    \param nCol       [in]  Column index of the cell.
    \param nContent   [in]  Content index.
    \param bAutoScale [in]  Auto-scaled flag value.

    \remarks
    Method generates the eInvalidInput exception when input row number and
    column number are less than 0 or greater than table size.
    Method generates the eIsWriteProtected when format is not editable
    (!isFormatEditable()).
  */
  virtual void setAutoScale(
    OdInt32 nRow, 
    OdInt32 nCol, 
    OdInt32 nContent,
    bool bAutoScale);

  /** \details
    Returns the cell alignment for the specified cell in this table object.

    \param nRow [in]  Row index of the cell.
    \param nCol [in]  Column index of the cell.
    \returns Cell alignment for the specified cell.
    \remarks
    alignment() returns one of the following:

    <table>
    Name                    Value
    OdDb::kTopLeft          1
    OdDb::kTopCenter        2
    OdDb::kTopRight         3
    OdDb::kMiddleLeft       4
    OdDb::kMiddleCenter     5
    OdDb::kMiddleRight      6
    OdDb::kBottomLeft       7
    OdDb::kBottomCenter     8
    OdDb::kBottomRight      9
    </table>
  */
  virtual OdDb::CellAlignment alignment(
    OdInt32 nRow, 
    OdInt32 nCol) const;

  /** \details
    Sets the cell alignment for the specified cell in this table entity.

    \param nRow       [in]  Row index of the cell.
    \param nCol       [in]  Column index of the cell.
    \param nAlignment [in]  Alignment.
    \remarks
    Alignment must be one of the following:

    <table>
    Name                    Value
    OdDb::kTopLeft          1
    OdDb::kTopCenter        2
    OdDb::kTopRight         3
    OdDb::kMiddleLeft       4
    OdDb::kMiddleCenter     5
    OdDb::kMiddleRight      6
    OdDb::kBottomLeft       7
    OdDb::kBottomCenter     8
    OdDb::kBottomRight      9
    </table>

    Method generates the eInvalidInput exception when the alignment is out of
    the available range 1 to 9, when the row argument is grater than the number
    of rows, or when the column argument is grater than the number of columns.
  */
  virtual void setAlignment(
    OdInt32 nRow, 
    OdInt32 nCol, 
    OdDb::CellAlignment nAlignment);

  /** \details
    Returns the color of the cell content as an object of the OdCmColor class.

    \param nRow [in]  Row index of the cell.
    \param nCol [in]  Column index of the cell.
    \param nContent [in]  Content index.

    \remarks
    Method generates the eInvalidInput exception when input row number and column number are less than 0 or greater than table size.
  */
  virtual OdCmColor contentColor(
    OdInt32 nRow, 
    OdInt32 nCol) const;

  /** \details
    Returns the color of the specified content of a cell as an object of the
    OdCmColor class.

    \param nRow     [in]  Row index of the cell.
    \param nCol     [in]  Column index of the cell.
    \param nContent [in]  Content index.
    \returns Color of the specified content of the cell.
    \remarks
    Method generates the eInvalidInput exception when input row number and
    column number are less than 0 or greater than table size.
  */
  virtual OdCmColor contentColor(
    OdInt32 nRow, 
    OdInt32 nCol,
    OdInt32 nContent) const;

  /** \details
    Sets the color of the zero content of a cell.

    \param nRow  [in]  Row index of the cell.
    \param nCol  [in]  Column index of the cell.
    \param color [in]  Color of the content.

    \remarks
    Method generates the eInvalidInput exception when input row number and
    column number are less than 0 or greater than table size.
    Method generates the eIsWriteProtected when format is not editable
    (!isFormatEditable()).
  */
  virtual void setContentColor(
    OdInt32 nRow, 
    OdInt32 nCol, 
    const OdCmColor& color);

  /** \details
    Sets the color of the specified content of a cell.

    \param nRow     [in]  Row index of the cell.
    \param nCol     [in]  Column index of the cell.
    \param nContent [in]  Content index.
    \param color    [in]  Color of the content.

    \remarks
    Method generates the eInvalidInput exception when input row number and
    column number are less than 0 or greater than table size.
    Method generates the eIsWriteProtected when format is not editable
    (!isFormatEditable()).
  */
  virtual void setContentColor(
    OdInt32 nRow, 
    OdInt32 nCol, 
    OdInt32 nContent,
    const OdCmColor& color);

  /** \details
    Returns the text style of the zero content of a cell.

    \param nRow [in]  Row index of the cell.
    \param nCol [in]  Column index of the cell.
    \returns Text style object ID.
    \remarks
    Method generates the eInvalidInput exception when input row number and
    column number are less than 0 or greater than table size.
  */
  virtual OdDbObjectId textStyle(
    OdInt32 nRow, 
    OdInt32 nCol) const;

  /** \details
    Returns the text style of the specified content of a cell.

    \param nRow     [in]  Row index of the cell.
    \param nCol     [in]  Column index of the cell.
    \param nContent [in]  Content index.
    \returns Text style object ID.
    \remarks
    Method generates the eInvalidInput exception when input row number and
    column number are less than 0 or greater than table size.
  */
  virtual OdDbObjectId textStyle(
    OdInt32 nRow, 
    OdInt32 nCol,
    OdInt32 nContent) const;

  /** \details
    Sets the text style for the zero content of a cell.

    \param nRow        [in]  Row index of the cell.
    \param nCol        [in]  Column index of the cell.
    \param idTextStyle [in]  Text style object ID.

    \remarks
    Method generates the eInvalidInput exception when input row number and
    column number are less than 0 or greater than table size.
    Method generates the eIsWriteProtected when format is not editable
    (!isFormatEditable()).
  */
  virtual void setTextStyle(
    OdInt32 nRow, 
    OdInt32 nCol, 
    const OdDbObjectId& idTextStyle);

  /** \details
    Sets the text style for the specified content of a cell.

    \param nRow        [in]  Row index of the cell.
    \param nCol        [in]  Column index of the cell.
    \param nContent    [in]  Content index.
    \param idTextStyle [in]  Text style object ID.

    \remarks
    Method generates the eInvalidInput exception when input row number and
    column number are less than 0 or greater than table size.
    Method generates the eIsWriteProtected when format is not editable
    (!isFormatEditable()).
  */
  virtual void setTextStyle(
    OdInt32 nRow, 
    OdInt32 nCol, 
    OdInt32 nContent,
    const OdDbObjectId& idTextStyle);

  /** \details
    Returns the text height for the zero content of a cell in this table object.

    \param nRow [in]  Row index of the cell.
    \param nCol [in]  Column index of the cell.
    \returns Text height value.
  */
  virtual double textHeight(
    OdInt32 nRow, 
    OdInt32 nCol) const;

  /** \details
    Returns the text height of the specified content of a cell.

    \param nRow     [in]  Row index of the cell.
    \param nCol     [in]  Column index of the cell.
    \param nContent [in]  Content index.
    \returns Text height value.
    \remarks
    Method generates the eInvalidInput exception when input row number and
    column number are less than 0 or greater than table size.
  */
  virtual double textHeight(
    OdInt32 nRow, 
    OdInt32 nCol,
    OdInt32 nContent) const;

  /** \details
    Sets the text height of the zero content of a cell.

    \param nRow        [in]  Row index of the cell.
    \param nCol        [in]  Column index of the cell.
    \param fTextHeight [in]  Text height value.

    \remarks
    Method generates the eInvalidInput exception when input row number and
    column number are less than 0 or greater than table size.
    Method generates the eIsWriteProtected when format is not editable
    (!isFormatEditable()).
  */
  virtual void setTextHeight(
    OdInt32 nRow, 
    OdInt32 nCol, 
    double fTextHeight);

  /** \details
    Sets the text height of the specified content of a cell.

    \param nRow        [in]  Row index of the cell.
    \param nCol        [in]  Column index of the cell.
    \param nContent    [in]  Content index.
    \param fTextHeight [in]  Text height value.

    \remarks
    Method generates the eInvalidInput exception when input row number and
    column number are less than 0 or greater than table size.
    Method generates the eIsWriteProtected when format is not editable
    (!isFormatEditable()).
  */
  virtual void setTextHeight(
    OdInt32 nRow, 
    OdInt32 nCol,
    OdInt32 nContent,
    double fTextHeight);

  /** \details
    Returns the background color for the specified cell in this table object.

    \param nRow [in]  Row index of the cell.
    \param nCol [in]  Column index of the cell.
    \returns Background color value.
  */
  virtual OdCmColor backgroundColor(
    OdInt32 nRow, 
    OdInt32 nCol) const;

  /** \details
    Sets the background color for the specified cell in this table object.

    \param nRow  [in]  Row index of the cell.
    \param nCol  [in]  Column index of the cell.
    \param color [in]  Background color value.
    \remarks
    Method generates the eInvalidInput exception when the row type mask is more
    than 7, the row argument is grater than the number of rows, or the column
    argument is grater than the number of columns.
  */
  virtual void setBackgroundColor(
    OdInt32 nRow, 
    OdInt32 nCol, 
    const OdCmColor& color);

  /** \details
    Returns the content layout of the specified cell.

    \param nRow [in]  Row index of the cell.
    \param nCol [in]  Column index of the cell.
    \returns Content layout of the specified cell.
    \remarks
    Returning value should be one of following values:

    <table>
    Name                                  Value
    kCellContentLayoutFlow                0x1
    kCellContentLayoutStackedHorizontal   0x2
    kCellContentLayoutStackedVertical     0x4
    </table>
  */
  virtual OdDb::CellContentLayout contentLayout(
    OdInt32 nRow, 
    OdInt32 nCol) const;

  /** \details
    Sets the content layout for the specified cell.

    \param nRow    [in]  Row index of the cell.
    \param nCol    [in]  Column index of the cell.
    \param nLayout [in]  Content layout.

    \remarks
    Content layout should take one of following values:

    <table>
    Name                                  Value
    kCellContentLayoutFlow                0x1
    kCellContentLayoutStackedHorizontal   0x2
    kCellContentLayoutStackedVertical     0x4
    </table>

    Method generates the eInvalidInput exception when input row number and
    column number are less than 0 or greater than table size.
    Method generates the eIsWriteProtected when format is not editable
    (!isFormatEditable()).
  */
  virtual void setContentLayout(
    OdInt32 nRow, 
    OdInt32 nCol, 
    OdDb::CellContentLayout nLayout);

  /** \details
    Returns the direction that this table entity flows from its first row to the
    last.
    \returns Flow direction value.
    \remarks
    flowDirection() returns one of the following:

    <table>
    Name          Value   Description
    OdDb::kTtoB   0       Top to Bottom
    OdDb::kBtoT   1       Bottom to Top
    </table>
  */
  virtual OdDb::FlowDirection flowDirection() const;
  
  /** \details
    Sets the direction that this table entity flows from its first row to the
    last.

    \param nDir [in]  Flow direction value.

    \remarks
    flowDirection must be one of the following:

    <table>
    Name          Value   Description
    OdDb::kTtoB   0       Top to Bottom
    OdDb::kBtoT   1       Bottom to Top
    </table>

    Method generates the eInvalidInput exception when the flow direction is not equal to 0 or 1.
  */
  virtual void setFlowDirection(
    OdDb::FlowDirection nDir);

  /** \details
    Returns the margin of the cell, row or column as a double value.
    Use a valid row index and pass the column index '-1' to get the margin of the row.
    Use a valid column index and pass the row index '-1' to get the margin of the column.

    \param nRow    [in]  Row index.
    \param nCol    [in]  Column index.
    \param nMargin [in]  Margin type.
    \returns Margin value.
    \remarks
    Margin type should be one of following values:

    <table>
    Name                     Value
    kCellMarginTop           0x01
    kCellMarginLeft          0x02
    kCellMarginBottom        0x04
    kCellMarginRight         0x08
    kCellMarginHorzSpacing   0x10
    kCellMarginVertSpacing   0x20
    </table>
  */
  virtual double margin(
    OdInt32 nRow, 
    OdInt32 nCol, 
    OdDb::CellMargin nMargin) const;

  /** \details
    Sets the margin to the cell, row or column.
    Use a valid row index and pass the column index '-1' to set the margin for the row.
    Use a valid column index and pass the row index '-1' to set the margin for the column.

    \param nRow     [in]  Row index.
    \param nCol     [in]  Column index.
    \param nMargins [in]  Margin type.
    \param fMargin  [in]  Margin value.

    \remarks
    Margin type should be a combination of the following values:

    <table>
    Name                     Value
    kCellMarginTop           0x01
    kCellMarginLeft          0x02
    kCellMarginBottom        0x04
    kCellMarginRight         0x08
    kCellMarginHorzSpacing   0x10
    kCellMarginVertSpacing   0x20
    </table>
  */
  virtual void setMargin(
    OdInt32 nRow, 
    OdInt32 nCol, 
    OdDb::CellMargin nMargins, 
    double fMargin);

  /** \details
    Returns the grid linestyle of the cell, row or column.
    Use a valid row index and pass the column index '-1' to get the grid linestyle of the row.
    Use a valid column index and pass the row index '-1' to get the grid linestyle of the column.

    \param nRow          [in]  Row index.
    \param nCol          [in]  Column index.
    \param nGridLineType [in]  Grid linetype.
    \returns Grid linestyle.
    \remarks
    Grid linetype should take one of the following values:

    <table>
    Name                  Value
    kInvalidGridLine      0x00
    kHorzTop              0x01
    kHorzInside           0x02
    kHorzBottom           0x04
    kVertLeft             0x08
    kVertInside           0x10
    kVertRight            0x20
    kHorzGridLineTypes    kHorzTop|kHorzBottom|kHorzInside
    kVertGridLineTypes    kVertLeft|kVertRight|kVertInside
    kOuterGridLineTypes   kHorzTop|kHorzBottom|kVertLeft|kVertRight
    kInnerGridLineTypes   kHorzInside|kVertInside
    kAllGridLineTypes     kOuterGridLineTypes|kInnerGridLineTypes
    </table>

    Grid line style should take one of the following values:

    <table>
    Name                   Value
    kGridLineStyleSingle   1
    kGridLineStyleDouble   2
    </table>
  */
  virtual OdDb::GridLineStyle gridLineStyle(
    OdInt32 nRow, 
    OdInt32 nCol, 
    OdDb::GridLineType nGridLineType) const;

  /** \details
    Sets the grid line style for the cell, row or column.
    Use a valid row index and pass the column index '-1' to set the row grid linestyle.
    Use a valid column index and pass the row index '-1' to set the column grid linestyle.

    \param nRow           [in]  Row index.
    \param nCol           [in]  Column index.
    \param nGridLineTypes [in]  Grid linetype.
    \param nLineStyle     [in]  Grid linestyle.

    \remarks
    Grid linetype should take one of the following values:

    <table>
    Name                  Value
    kInvalidGridLine      0x00
    kHorzTop              0x01
    kHorzInside           0x02
    kHorzBottom           0x04
    kVertLeft             0x08
    kVertInside           0x10
    kVertRight            0x20
    kHorzGridLineTypes    kHorzTop|kHorzBottom|kHorzInside
    kVertGridLineTypes    kVertLeft|kVertRight|kVertInside
    kOuterGridLineTypes   kHorzTop|kHorzBottom|kVertLeft|kVertRight
    kInnerGridLineTypes   kHorzInside|kVertInside
    kAllGridLineTypes     kOuterGridLineTypes|kInnerGridLineTypes
    </table>

    Grid linestyle should take one of the following values:

    <table>
    Name                   Value
    kGridLineStyleSingle   1
    kGridLineStyleDouble   2
    </table>
  */
  virtual void setGridLineStyle(
    OdInt32 nRow, 
    OdInt32 nCol, 
    OdDb::GridLineType nGridLineTypes,
    OdDb::GridLineStyle nLineStyle);

  /** \details
    Returns the grid lineweight of the cell, row or column.
    Use a valid row index and pass the column index '-1' to get the row grid lineweight.
    Use a valid column index and pass the row index '-1' to get the column grid lineweight.

    \param nRow          [in]  Row index.
    \param nCol          [in]  Column index.
    \param nGridLineType [in]  Grid linetype.
    \returns Lineweight value.
    \remarks
    Grid linetype should take one of the following values:

    <table>
    Name                  Value
    kInvalidGridLine      0x00
    kHorzTop              0x01
    kHorzInside           0x02
    kHorzBottom           0x04
    kVertLeft             0x08
    kVertInside           0x10
    kVertRight            0x20
    kHorzGridLineTypes    kHorzTop|kHorzBottom|kHorzInside
    kVertGridLineTypes    kVertLeft|kVertRight|kVertInside
    kOuterGridLineTypes   kHorzTop|kHorzBottom|kVertLeft|kVertRight
    kInnerGridLineTypes   kHorzInside|kVertInside
    kAllGridLineTypes     kOuterGridLineTypes|kInnerGridLineTypes
    </table>

    Grid lineweight should take one of the following values:

    <table>
    Name               Value
    kLnWt000           0
    kLnWt005           5
    kLnWt009           9
    kLnWt013           13
    kLnWt015           15
    kLnWt018           18
    kLnWt020           20
    kLnWt025           25
    kLnWt030           30
    kLnWt035           35
    kLnWt040           40
    kLnWt050           50
    kLnWt053           53
    kLnWt060           60
    kLnWt070           70
    kLnWt080           80
    kLnWt090           90
    kLnWt100           100
    kLnWt106           106
    kLnWt120           120
    kLnWt140           140
    kLnWt158           158
    kLnWt200           200
    kLnWt211           211
    kLnWtByLayer       -1
    kLnWtByBlock       -2
    kLnWtByLwDefault   -3
    </table>
  */
  virtual OdDb::LineWeight gridLineWeight(
    OdInt32 nRow, 
    OdInt32 nCol, 
    OdDb::GridLineType nGridLineType) const;

  /** \details
    Sets the grid lineweight of the cell, row or column.
    Use a valid row index and pass the column index '-1' to set the row grid lineweight.
    Use a valid column index and pass the row index '-1' to set the column grid lineweight.

    \param nRow           [in]  Row index.
    \param nCol           [in]  Column index.
    \param nGridLineTypes [in]  Grid linetype.
    \param nLineWeight    [in]  Grid lineweight.

    \remarks
    Grid linetype should take one of the following values:

    <table>
    Name                  Value
    kInvalidGridLine      0x00
    kHorzTop              0x01
    kHorzInside           0x02
    kHorzBottom           0x04
    kVertLeft             0x08
    kVertInside           0x10
    kVertRight            0x20
    kHorzGridLineTypes    kHorzTop|kHorzBottom|kHorzInside
    kVertGridLineTypes    kVertLeft|kVertRight|kVertInside
    kOuterGridLineTypes   kHorzTop|kHorzBottom|kVertLeft|kVertRight
    kInnerGridLineTypes   kHorzInside|kVertInside
    kAllGridLineTypes     kOuterGridLineTypes|kInnerGridLineTypes
    </table>

    Grid lineweight should take one of the following values:

    <table>
    Name               Value
    kLnWt000           0
    kLnWt005           5
    kLnWt009           9
    kLnWt013           13
    kLnWt015           15
    kLnWt018           18
    kLnWt020           20
    kLnWt025           25
    kLnWt030           30
    kLnWt035           35
    kLnWt040           40
    kLnWt050           50
    kLnWt053           53
    kLnWt060           60
    kLnWt070           70
    kLnWt080           80
    kLnWt090           90
    kLnWt100           100
    kLnWt106           106
    kLnWt120           120
    kLnWt140           140
    kLnWt158           158
    kLnWt200           200
    kLnWt211           211
    kLnWtByLayer       -1
    kLnWtByBlock       -2
    kLnWtByLwDefault   -3
    </table>
  */
  virtual void setGridLineWeight(
    OdInt32 nRow, 
    OdInt32 nCol, 
    OdDb::GridLineType nGridLineTypes, 
    OdDb::LineWeight nLineWeight);

  /** \details
    Returns the grid linetype of the cell, row or column.
    Use a valid row index and pass the column index '-1' to get the row grid linetype.
    Use a valid column index and pass the row index '-1' to get the column grid linetype.

    \param nRow          [in]  Row index.
    \param nCol          [in]  Column index.
    \param nGridLineType [in]  Grid linetypes.
    \returns Grid linetype object ID.
    \remarks
    Grid linetype should take one of the following values:

    <table>
    Name                  Value
    kInvalidGridLine      0x00
    kHorzTop              0x01
    kHorzInside           0x02
    kHorzBottom           0x04
    kVertLeft             0x08
    kVertInside           0x10
    kVertRight            0x20
    kHorzGridLineTypes    kHorzTop|kHorzBottom|kHorzInside
    kVertGridLineTypes    kVertLeft|kVertRight|kVertInside
    kOuterGridLineTypes   kHorzTop|kHorzBottom|kVertLeft|kVertRight
    kInnerGridLineTypes   kHorzInside|kVertInside
    kAllGridLineTypes     kOuterGridLineTypes|kInnerGridLineTypes
    </table>
  */
  virtual OdDbObjectId gridLinetype(
    OdInt32 nRow, 
    OdInt32 nCol, 
    OdDb::GridLineType nGridLineType) const;

  /** \details
    Sets the grid linetype of the cell, row or column.
    Use a valid row index and pass the column index '-1' to set the row grid linetype.
    Use a valid column index and pass the row index '-1' to set the column grid linetype.

    \param nRow           [in]  Row index.
    \param nCol           [in]  Column index.
    \param nGridLineTypes [in]  Grid linetypes.
    \param idLinetype     [in]  Object ID of the grid linetype to set.

    \remarks
    Grid linetypes should take one of the following values:

    <table>
    Name                  Value
    kInvalidGridLine      0x00
    kHorzTop              0x01
    kHorzInside           0x02
    kHorzBottom           0x04
    kVertLeft             0x08
    kVertInside           0x10
    kVertRight            0x20
    kHorzGridLineTypes    kHorzTop|kHorzBottom|kHorzInside
    kVertGridLineTypes    kVertLeft|kVertRight|kVertInside
    kOuterGridLineTypes   kHorzTop|kHorzBottom|kVertLeft|kVertRight
    kInnerGridLineTypes   kHorzInside|kVertInside
    kAllGridLineTypes     kOuterGridLineTypes|kInnerGridLineTypes
    </table>
  */
  virtual void setGridLinetype(
    OdInt32 nRow, 
    OdInt32 nCol, 
    OdDb::GridLineType nGridLineTypes, 
    const OdDbObjectId& idLinetype);

  /** \details
    Returns the grid line color of the cell, row or column as an object of the
    OdCmColor class.
    Use a valid row index and pass the column index '-1' to get the row grid line color.
    Use a valid column index and pass the row index '-1' to get the column grid line color.

    \param nRow          [in]  Row index.
    \param nCol          [in]  Column index.
    \param nGridLineType [in]  Grid linetype.
    \returns Grid line color.
    \remarks
    Grid linetype should take one of the following values:

    <table>
    Name                  Value
    kInvalidGridLine      0x00
    kHorzTop              0x01
    kHorzInside           0x02
    kHorzBottom           0x04
    kVertLeft             0x08
    kVertInside           0x10
    kVertRight            0x20
    kHorzGridLineTypes    kHorzTop|kHorzBottom|kHorzInside
    kVertGridLineTypes    kVertLeft|kVertRight|kVertInside
    kOuterGridLineTypes   kHorzTop|kHorzBottom|kVertLeft|kVertRight
    kInnerGridLineTypes   kHorzInside|kVertInside
    kAllGridLineTypes     kOuterGridLineTypes|kInnerGridLineTypes
    </table>
  */
  virtual OdCmColor gridColor(
    OdInt32 nRow, 
    OdInt32 nCol, 
    OdDb::GridLineType nGridLineType) const;

  /** \details
    Sets the grid line color of the cell, row or column.
    Use a valid row index and pass the column index '-1' to set the grid line color for the row.
    Use a valid column index and pass the row index '-1' to set the grid line color for the column.

    \param nRow           [in]  Row index.
    \param nCol           [in]  Column index.
    \param nGridlineTypes [in]  Grid linetypes.
    \param color          [in]  Grid line color.

    \remarks
    Grid linetype should take one of the following values:

    <table>
    Name                  Value
    kInvalidGridLine      0x00
    kHorzTop              0x01
    kHorzInside           0x02
    kHorzBottom           0x04
    kVertLeft             0x08
    kVertInside           0x10
    kVertRight            0x20
    kHorzGridLineTypes    kHorzTop|kHorzBottom|kHorzInside
    kVertGridLineTypes    kVertLeft|kVertRight|kVertInside
    kOuterGridLineTypes   kHorzTop|kHorzBottom|kVertLeft|kVertRight
    kInnerGridLineTypes   kHorzInside|kVertInside
    kAllGridLineTypes     kOuterGridLineTypes|kInnerGridLineTypes
    </table>
  */
  virtual void setGridColor(
    OdInt32 nRow, 
    OdInt32 nCol, 
    OdDb::GridLineType nGridLineTypes, 
    const OdCmColor& color);

  /** \details
    Returns the grid visibility of the cell, row or column.
    Use a valid row index and pass the column index '-1' to get the grid visibility of the row.
    Use a valid column index and pass the row index '-1' to get the grid visibility of the column.

    \param nRow          [in]  Row index.
    \param nCol          [in]  Column index.
    \param nGridLineType [in]  Grid linetype.

    \remarks
    Grid linetype should take one of the following values:

    <table>
    Name                  Value
    kInvalidGridLine      0x00
    kHorzTop              0x01
    kHorzInside           0x02
    kHorzBottom           0x04
    kVertLeft             0x08
    kVertInside           0x10
    kVertRight            0x20
    kHorzGridLineTypes    kHorzTop|kHorzBottom|kHorzInside
    kVertGridLineTypes    kVertLeft|kVertRight|kVertInside
    kOuterGridLineTypes   kHorzTop|kHorzBottom|kVertLeft|kVertRight
    kInnerGridLineTypes   kHorzInside|kVertInside
    kAllGridLineTypes     kOuterGridLineTypes|kInnerGridLineTypes
    </table>

    Grid visibility should take one of the following values:

    <table>
    Name         Value
    kInvisible   1
    kVisible     0
    </table>
  */
  virtual OdDb::Visibility gridVisibility(
    OdInt32 nRow, 
    OdInt32 nCol, 
    OdDb::GridLineType nGridLineType) const;

  /** \details
    Sets the grid visibility for the cell, row or column.
    Use a valid row index and pass the column index '-1' to set the grid visibility for the row.
    Use a valid column index and pass the row index '-1' to set the grid visibility for the column.

    \param nRow           [in]  Row index.
    \param nCol           [in]  Column index.
    \param nGridLineTypes [in]  Grid linetypes.
    \param nVisibility    [in]  Grid line visibility.

    \remarks
    Grid linetypes should take one of the following values:

    <table>
    Name                  Value
    kInvalidGridLine      0x00
    kHorzTop              0x01
    kHorzInside           0x02
    kHorzBottom           0x04
    kVertLeft             0x08
    kVertInside           0x10
    kVertRight            0x20
    kHorzGridLineTypes    kHorzTop|kHorzBottom|kHorzInside
    kVertGridLineTypes    kVertLeft|kVertRight|kVertInside
    kOuterGridLineTypes   kHorzTop|kHorzBottom|kVertLeft|kVertRight
    kInnerGridLineTypes   kHorzInside|kVertInside
    kAllGridLineTypes     kOuterGridLineTypes|kInnerGridLineTypes
    </table>

    Grid visibility should take one of the following values:

    <table>
    Name         Value
    kInvisible   1
    kVisible     0
    </table>
  */
  virtual void setGridVisibility(
    OdInt32 nRow, 
    OdInt32 nCol, 
    OdDb::GridLineType nGridLineTypes, 
    OdDb::Visibility nVisibility);

  /** \details
    Returns the grid double line spacing of the cell, row or column as a double value.
    Use a valid row index and pass column index -1 to get row grid double line spacing.
    Use a valid column index and pass row index -1 to get column grid double line spacing.

    \param nRow          [in]  Row index.
    \param nCol          [in]  Column index.
    \param nGridLineType [in]  Grid linetype.
    \returns Grid double line spacing value.
    \remarks
    Grid linetype should take one of the following values:

    <table>
    Name                  Value
    kInvalidGridLine      0x00
    kHorzTop              0x01
    kHorzInside           0x02
    kHorzBottom           0x04
    kVertLeft             0x08
    kVertInside           0x10
    kVertRight            0x20
    kHorzGridLineTypes    kHorzTop|kHorzBottom|kHorzInside
    kVertGridLineTypes    kVertLeft|kVertRight|kVertInside
    kOuterGridLineTypes   kHorzTop|kHorzBottom|kVertLeft|kVertRight
    kInnerGridLineTypes   kHorzInside|kVertInside
    kAllGridLineTypes     kOuterGridLineTypes|kInnerGridLineTypes
    </table>
  */
  virtual double gridDoubleLineSpacing(
    OdInt32 nRow, 
    OdInt32 nCol, 
    OdDb::GridLineType nGridLineType) const;

  /** \details
    Sets the grid double line spacing for the cell, row or column.
    Use a valid row index and pass the column index '-1' to set the grid double line spacing for the row.
    Use a valid column index and pass the row index '-1' to set the grid double line spacing for the column.

    \param nRow           [in]  Row index.
    \param nCol           [in]  Column index.
    \param nGridLineTypes [in]  Grid linetypes.
    \param fSpacing       [in]  Grid double line spacing value.

    \remarks
    Grid linetypes should take one of the following values:

    <table>
    Name                  Value
    kInvalidGridLine      0x00
    kHorzTop              0x01
    kHorzInside           0x02
    kHorzBottom           0x04
    kVertLeft             0x08
    kVertInside           0x10
    kVertRight            0x20
    kHorzGridLineTypes    kHorzTop|kHorzBottom|kHorzInside
    kVertGridLineTypes    kVertLeft|kVertRight|kVertInside
    kOuterGridLineTypes   kHorzTop|kHorzBottom|kVertLeft|kVertRight
    kInnerGridLineTypes   kHorzInside|kVertInside
    kAllGridLineTypes     kOuterGridLineTypes|kInnerGridLineTypes
    </table>
  */
  virtual void setGridDoubleLineSpacing(
    OdInt32 nRow, 
    OdInt32 nCol, 
    OdDb::GridLineType nGridLineTypes, 
    double fSpacing);

  /** \details
    Returns the grid line property of the cell, row or column.
    Use a valid row index and pass the column index '-1' to get the grid property of the row.
    Use a valid column index and pass the row index '-1' to get the grid property of the column.

    \param nRow          [in]  Row index.
    \param nCol          [in]  Column index.
    \param nGridLineType [in]  Grid linetype.
    \param gridProp     [out]  Grid line property.

    \remarks
    Grid linetype should take one of the following values:

    <table>
    Name                  Value
    kInvalidGridLine      0x00
    kHorzTop              0x01
    kHorzInside           0x02
    kHorzBottom           0x04
    kVertLeft             0x08
    kVertInside           0x10
    kVertRight            0x20
    kHorzGridLineTypes    kHorzTop|kHorzBottom|kHorzInside
    kVertGridLineTypes    kVertLeft|kVertRight|kVertInside
    kOuterGridLineTypes   kHorzTop|kHorzBottom|kVertLeft|kVertRight
    kInnerGridLineTypes   kHorzInside|kVertInside
    kAllGridLineTypes     kOuterGridLineTypes|kInnerGridLineTypes
    </table>
  */
  virtual void getGridProperty(
    OdInt32 nRow, 
    OdInt32 nCol, 
    OdDb::GridLineType nGridLineType, 
    OdGridProperty& gridProp) const;

  /** \details
    Sets the grid line property for the cell, row or column.
    Use a valid row index and pass the column index '-1' to set the grid property for the row.
    Use a valid column index and pass the row index '-1' to set the grid property for the column.

    \param nRow           [in]  Row index.
    \param nCol           [in]  Column index.
    \param nGridLineTypes [in]  Grid linetypes.
    \param gridProp       [in]  Grid line property.

    \remarks
    Grid linetypes should take one of the following values:

    <table>
    Name                  Value
    kInvalidGridLine      0x00
    kHorzTop              0x01
    kHorzInside           0x02
    kHorzBottom           0x04
    kVertLeft             0x08
    kVertInside           0x10
    kVertRight            0x20
    kHorzGridLineTypes    kHorzTop|kHorzBottom|kHorzInside
    kVertGridLineTypes    kVertLeft|kVertRight|kVertInside
    kOuterGridLineTypes   kHorzTop|kHorzBottom|kVertLeft|kVertRight
    kInnerGridLineTypes   kHorzInside|kVertInside
    kAllGridLineTypes     kOuterGridLineTypes|kInnerGridLineTypes
    </table>
  */
  virtual void setGridProperty(
    OdInt32 nRow, 
    OdInt32 nCol, 
    OdDb::GridLineType nGridLineTypes, 
    const OdGridProperty& gridProp);

  /** \details
    Sets the grid line property for the cell, row or column.

    \param range          [in]  Cell range, row range or column range.
    \param nGridLineTypes [in]  Grid linetypes.
    \param gridProp       [in]  Grid line property.

    \remarks
    Grid linetypes should take one of the following values:

    <table>
    Name                  Value
    kInvalidGridLine      0x00
    kHorzTop              0x01
    kHorzInside           0x02
    kHorzBottom           0x04
    kVertLeft             0x08
    kVertInside           0x10
    kVertRight            0x20
    kHorzGridLineTypes    kHorzTop|kHorzBottom|kHorzInside
    kVertGridLineTypes    kVertLeft|kVertRight|kVertInside
    kOuterGridLineTypes   kHorzTop|kHorzBottom|kVertLeft|kVertRight
    kInnerGridLineTypes   kHorzInside|kVertInside
    kAllGridLineTypes     kOuterGridLineTypes|kInnerGridLineTypes
    </table>
  */
  virtual void setGridProperty(
    const OdCellRange& range,
    OdDb::GridLineType nGridLineTypes, 
    const OdGridProperty& gridProp);

  /** \details
    Returns the override of the cell, row or column.
    Use a valid row index and pass the column index '-1' to get the row property overrides.
    Use a valid column index and pass the row index '-1' to get the column property overrides.

    \param nRow     [in]  Row index.
    \param nCol     [in]  Column index.
    \param nContent [in]  Content index.
    \returns Override of the cell, row or column.
    \remarks
    Cell property should take one of the next values:

    <table>
    Name                         Value
    kCellPropInvalid             0x00000
    kCellPropDataType            0x00001
    kCellPropDataFormat          0x00002
    kCellPropRotation            0x00004
    kCellPropScale               0x00008
    kCellPropAlignment           0x00010
    kCellPropContentColor        0x00020
    kCellPropTextStyle           0x00040
    kCellPropTextHeight          0x00080
    kCellPropAutoScale           0x00100
    kCellPropBackgroundColor     0x00200
    kCellPropMarginLeft          0x00400
    kCellPropMarginTop           0x00800
    kCellPropMarginRight         0x01000
    kCellPropMarginBottom        0x02000
    kCellPropContentLayout       0x04000
    kCellPropMergeAll            0x08000
    kCellPropFlowDirBtoT         0x10000
    kCellPropMarginHorzSpacing   0x20000
    kCellPropMarginVertSpacing   0x40000
    kCellPropDataTypeAndFormat   (kCellPropDataType | kCellPropDataFormat)
    kCellPropContent             (kCellPropDataType | kCellPropDataFormat |
                                 kCellPropRotation | kCellPropScale |
                                 kCellPropContentColor | kCellPropTextStyle |
                                 kCellPropTextHeight | kCellPropAutoScale)
    kCellPropBitProperties       (kCellPropAutoScale | kCellPropMergeAll |
                                 kCellPropFlowDirBtoT)
    kCellPropAll                 (kCellPropDataType | kCellPropDataFormat |
                                 kCellPropRotation | kCellPropScale |
                                 kCellPropAlignment | kCellPropContentColor |
                                 kCellPropBackgroundColor | kCellPropTextStyle |
                                 kCellPropTextHeight | kCellPropMarginLeft |
                                 kCellPropMarginTop | kCellPropMarginRight |
                                 kCellPropMarginBottom | kCellPropMarginHorzSpacing |
                                 kCellPropMarginVertSpacing | kCellPropAutoScale |
                                 kCellPropMergeAll | kCellPropFlowDirBtoT |
                                 kCellPropContentLayout)
    </table>
  */
  virtual OdDb::CellProperty getOverride(
    OdInt32 nRow, 
    OdInt32 nCol, 
    OdInt32 nContent) const;

  /** \details
    Returns the override in the grid line of the cell, row, or column.
    Use a valid row index and pass the column index '-1' to get the row property overrides.
    Use a valid column index and pass the row index '-1' to get the column property overrides.

    \param nRow          [in]  Row index.
    \param nCol          [in]  Column index.
    \param nGridLineType [in]  Grid linetype.

    \remarks
    Grid property should take one of the following values:

    <table>
    Name                         Value
    kGridPropInvalid             0x00
    kGridPropLineStyle           0x01
    kGridPropLineWeight          0x02
    kGridPropLinetype            0x04
    kGridPropColor               0x08
    kGridPropVisibility          0x10
    kGridPropDoubleLineSpacing   0x20
    kGridPropAll                 (kGridPropLineStyle | kGridPropLineWeight |
                                 kGridPropLinetype | kGridPropColor |
                                 kGridPropVisibility | kGridPropDoubleLineSpacing)
    </table>

    Grid linetype should take one of the following values:

    <table>
    Name                  Value
    kInvalidGridLine      0x00
    kHorzTop              0x01
    kHorzInside           0x02
    kHorzBottom           0x04
    kVertLeft             0x08
    kVertInside           0x10
    kVertRight            0x20
    kHorzGridLineTypes    kHorzTop | kHorzBottom | kHorzInside
    kVertGridLineTypes    kVertLeft | kVertRight | kVertInside
    kOuterGridLineTypes   kHorzTop | kHorzBottom | kVertLeft | kVertRight
    kInnerGridLineTypes   kHorzInside | kVertInside
    kAllGridLineTypes     kOuterGridLineTypes | kInnerGridLineTypes
    </table>
  */
  virtual OdDb::GridProperty getOverride(
    OdInt32 nRow, 
    OdInt32 nCol, 
    OdDb::GridLineType nGridLineType) const;

  /** \details
    Sets the override in the cell, row, column, content or grid line in the
    cell, row, or column.
    Use a valid row index and pass the column index '-1' to set the row property overrides.
    Use a valid column index and pass the row index '-1' to set the column property overrides.

    \param nRow      [in]  Row index.
    \param nCol      [in]  Column index.
    \param nContent  [in]  Content index.
    \param nOverride [in]  Override.

    \remarks
    Cell property should take one of the following values:

    <table>
    Name                         Value
    kCellPropInvalid             0x00000
    kCellPropDataType            0x00001
    kCellPropDataFormat          0x00002
    kCellPropRotation            0x00004
    kCellPropScale               0x00008
    kCellPropAlignment           0x00010
    kCellPropContentColor        0x00020
    kCellPropTextStyle           0x00040
    kCellPropTextHeight          0x00080
    kCellPropAutoScale           0x00100
    kCellPropBackgroundColor     0x00200
    kCellPropMarginLeft          0x00400
    kCellPropMarginTop           0x00800
    kCellPropMarginRight         0x01000
    kCellPropMarginBottom        0x02000
    kCellPropContentLayout       0x04000
    kCellPropMergeAll            0x08000
    kCellPropFlowDirBtoT         0x10000
    kCellPropMarginHorzSpacing   0x20000
    kCellPropMarginVertSpacing   0x40000
    kCellPropDataTypeAndFormat   (kCellPropDataType | kCellPropDataFormat)
    kCellPropContent             (kCellPropDataType | kCellPropDataFormat |
                                 kCellPropRotation | kCellPropScale |
                                 kCellPropContentColor | kCellPropTextStyle |
                                 kCellPropTextHeight | kCellPropAutoScale)
    kCellPropBitProperties       (kCellPropAutoScale | kCellPropMergeAll |
                                 kCellPropFlowDirBtoT)
    kCellPropAll                 (kCellPropDataType | kCellPropDataFormat |
                                 kCellPropRotation | kCellPropScale |
                                 kCellPropAlignment | kCellPropContentColor |
                                 kCellPropBackgroundColor | kCellPropTextStyle |
                                 kCellPropTextHeight | kCellPropMarginLeft |
                                 kCellPropMarginTop | kCellPropMarginRight |
                                 kCellPropMarginBottom | kCellPropMarginHorzSpacing |
                                 kCellPropMarginVertSpacing | kCellPropAutoScale |
                                 kCellPropMergeAll | kCellPropFlowDirBtoT |
                                 kCellPropContentLayout)
    </table>
  */
  virtual void setOverride(
    OdInt32 nRow, 
    OdInt32 nCol, 
    OdInt32 nContent, 
    OdDb::CellProperty nOverride);
  
  /** \details
    Sets the override in the cell, row, column, content or grid line in the
    cell, row, or column.
    Use a valid row index and pass the column index '-1' to set the row property overrides.
    Use a valid column index and pass the row index '-1' to set the column property overrides.

    \param nRow          [in]  Row index.
    \param nCol          [in]  Column index.
    \param nGridLineType [in]  Grid linetype.
    \param nOverride     [in]  Override.

    \remarks
    Grid linetype should take one of the next values:

    <table>
    Name                  Value
    kInvalidGridLine      0x00
    kHorzTop              0x01
    kHorzInside           0x02
    kHorzBottom           0x04
    kVertLeft             0x08
    kVertInside           0x10
    kVertRight            0x20
    kHorzGridLineTypes    kHorzTop | kHorzBottom | kHorzInside
    kVertGridLineTypes    kVertLeft | kVertRight | kVertInside
    kOuterGridLineTypes   kHorzTop | kHorzBottom | kVertLeft | kVertRight
    kInnerGridLineTypes   kHorzInside | kVertInside
    kAllGridLineTypes     kOuterGridLineTypes | kInnerGridLineTypes
    </table>
  */
  virtual void setOverride(
    OdInt32 nRow, 
    OdInt32 nCol, 
    OdDb::GridLineType nGridLineType, 
    OdDb::GridProperty nOverride);

  /** \details
    Removes all overrides in the cell, row, or column.
    Use a valid row index and pass the column index '-1' to remove the row property overrides.
    Use a valid column index and pass the row index '-1' to remove the column property overrides.

    \param nRow [in]  Row index.
    \param nCol [in]  Column index.
  */
  virtual void removeAllOverrides(
    OdInt32 nRow, 
    OdInt32 nCol);

  /** \details
    OdDbObject override.
  */
  virtual OdResult dwgInFields(
    OdDbDwgFiler* pFiler);

  /** \details
    OdDbObject override.
  */
  virtual void dwgOutFields(
    OdDbDwgFiler* pFiler) const;

  /** \details
    OdDbObject override.
  */
  virtual OdResult dxfInFields(
    OdDbDxfFiler* pFiler);

  /** \details
    OdDbObject override.
  */
  virtual void dxfOutFields(
    OdDbDxfFiler* pFiler) const;
};

/** \details
  This template class is a specialization of the OdSmartPtr class for smart
  pointers to objects of the OdDbFormattedTableData class.
*/
typedef OdSmartPtr<OdDbFormattedTableData> OdDbFormattedTableDataPtr;

#include "TD_PackPop.h"

#endif // OD_DBFORMATTEDTABLEDATA_H
