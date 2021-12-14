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

#ifndef _DbBlockPropertiesTable_h_Included_
#define _DbBlockPropertiesTable_h_Included_

#include "DbBlock1PtParameter.h"
#include "DbBlockGrips.h"

/** \details
    Corresponding C++ library:  TD_DynBlocks
    <group OdDb_Classes> 
*/
class DYNBLOCKS_EXPORT OdDbBlockUserParameter : public OdDbBlock1PtParameter
{
public:
  ODDB_EVAL_DECLARE_MEMBERS(OdDbBlockUserParameter);
  OdDbBlockUserParameter();
  enum UserParameterType 
  {
    kDistance = 0,
    kArea = 1,
    kVolume = 2,
    kReal = 3,
    kAngle = 4,
    kString = 5
  };

  OdDbObjectId assocVarId() const;
  void setAssocVarId(OdDbObjectId);

  OdString userVarName() const;
  //OdResult setUserVarName(const OdString&);

  /*

  virtual OdString expression()const;
  virtual void setExpression(const OdString&);

  virtual UserParameterType userParameterType()const;
  virtual void setUserParameterType(UserParameterType t);

  virtual OdString userVarDescription()const;
  virtual void setUserVarDescription(const OdString&);


  OdResult convertValue(OdResBuf*)const;
*/
};
typedef OdSmartPtr<OdDbBlockUserParameter> OdDbBlockUserParameterPtr;

/** \details
    Corresponding C++ library:  TD_Db
    <group OdDb_Classes>
*/
class OdDbBPTAuditError
{
public:
  enum BPTAuditErrorType {
    empty,
    noMatchingRow,
    invalidCell,
    notInValueSet,
    nonConstAttDef,
    invalidUnmatchedValue,
    duplicateRows,
    exprExternRef,
    cellEvalError
  };
  BPTAuditErrorType errorType() const { return m_Type; }
  void getCellIndex(int& row, int& col)const { row = m_nRow; col = m_nCol; }
  int getColumnIndex()const { return m_nCol; }
  OdIntArray getRowIndex() const { return m_Rows; }
  int getUnmatchedValueIndex()const { return m_nRow; }
protected:
  BPTAuditErrorType m_Type;
  int m_nRow;
  int m_nCol;
  OdIntArray m_Rows;
};

/** \details
    Corresponding C++ library:  TD_Db
    <group OdDb_Classes> 
*/
class DYNBLOCKS_EXPORT OdDbBlockPropertiesTable : public OdDbBlock1PtParameter
{
public:
  ODDB_EVAL_DECLARE_MEMBERS(OdDbBlockPropertiesTable);
  OdDbBlockPropertiesTable();
  
  /** \details
    Returns the number of columns in the table.
  */
  int numberOfColumns() const;
  
  /** \details
    Returns the number of rows in the table.
  */
  int numberOfRows() const;
  
  /** \details
    Returns the value of the specified cell.

    \param column [in] Column index.
    \param row [in] Row index.
    \param rb [out] Receives a copy of the cell value.
    \returns Returns eOk if the indices are valid, eInvalidIndex otherwise.
  */
  OdResult getCellValue(int column, int row, OdResBufPtr &rb) const;
  OdResult setCellValue(int column, int row, const OdResBuf*);

  OdResult getCellValueString(int column, int row, OdString &) const;

  // column flags
  bool constant(int column) const;
  OdResult setConstant(int column, bool);

  bool editable(int column) const;
  OdResult setEditable(int column, bool);
  
  bool removable(int column) const;
  OdResult setRemovable(int column, bool);
  
  bool isAngular(int column) const;
  bool isAttribute(int) const;

  // column properties
  OdDbObjectId customProperties(int column) const;
  void setCustomProperties(int column, const OdDbObjectId& id);
 
  OdResBufPtr defaultValue(int column) const;
  void setDefaultValue(int column, const OdResBuf*);
  
  OdString format(int column) const;
  void setFormat(int column, const OdString&);

  // if the values of the parameters must match some rows
  bool mustMatch() const;
  void setMustMatch(bool);

  // what is used in the context menu if parameter values do not match anything
  void setUnmatchedValue(int column, const OdResBuf*);
  OdResBufPtr unmatchedValue(int column) const;

  OdString getColumnName(int column) const;

  // Table description (used as a property description)
  OdString tableDescription() const;
  void setTableDescription(const OdString&);

  // Table name (used as a property name)
  OdString tableName() const;
  void setTableName(const OdString&);

  // rows may be reordered, but each row has unique (within the table) id that will not change
  int idOfRow(int index) const;
  int indexOfRow(int id) const;

  void insertColumn(const OdDbObjectId& param, const OdString& connection, int after);
  void insertRow(int after);
  void removeColumn(int column);
  void removeRow(int row);
  void appendColumn(const OdDbObjectId& param, const OdString& connection);
  void appendRow();
  void moveColumn(int from, int to);
  void moveRow(int from, int to);
  
  // BPT node id is stored in the eval graph xdata, reactors keep it updated
  static OdDbEvalNodeId bptNodeId(const OdDbEvalGraph*);
  virtual void addedToGraph(OdDbEvalGraph* ) ODRX_OVERRIDE;
  virtual void erased(const OdDbObject* , bool) ODRX_OVERRIDE;
  virtual void removedFromGraph(OdDbEvalGraph* gr) ODRX_OVERRIDE;
  
  // Look for a row matching default parameter values
  int getDefaultActiveRow() const;
  // this function actually changes default values of the parameters used in the table
  void setDefaultActiveRow(int row);
  
  bool columnValuesAreUnique(int col)const;
  // (not implemented, no idea what it does)
  bool disabledInDrawingEditor() const;
  
  // doesn't seem to have any effect besides the flag
  bool runtimeParametersOnly()const;
  void setRuntimeParametersOnly(bool );
  
  // look at the OdDbBPTAuditError::BPTAuditErrorType for possible error types 
  void tableAudit(OdArray<OdDbBPTAuditError>&);
  // sort the rows (duh)
  void sortRows(int column, bool ascending);
};
typedef OdSmartPtr<OdDbBlockPropertiesTable> OdDbBlockPropertiesTablePtr;

/** \details
    Corresponding C++ library:  TD_Db
    <group OdDb_Classes> 
*/
class DYNBLOCKS_EXPORT OdDbBlockPropertiesTableGrip : public OdDbBlockGrip
{
public:
  ODDB_DECLARE_MEMBERS(OdDbBlockPropertiesTableGrip);
  OdDbBlockPropertiesTableGrip();
};
typedef OdSmartPtr<OdDbBlockPropertiesTableGrip> OdDbBlockPropertiesTableGripPtr;

#endif //_DbBlockPropertiesTable_h_Included_
