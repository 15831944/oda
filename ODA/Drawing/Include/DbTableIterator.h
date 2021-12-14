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


#ifndef OD_DBTABLEITERATOR_H
#define OD_DBTABLEITERATOR_H

#include "RxObject.h"
#include "DbTableStyle.h"
#include "TD_PackPush.h"

/** 
  	<group OdDb_Classes>
*/
class DBENT_EXPORT OdDbCell
{
public:
  OdInt32  m_row;
  OdInt32  m_column;
  OdDbCell() { m_row = 0; m_column = 0; }
  OdDbCell(OdInt32 row, OdInt32 col) { m_row = row; m_column = col; }
};

class OdDbTableIteratorImpl;
class OdDbLinkedTableData;
class OdDbTable;

/** \details
  This class implements Iterator objects that traverse columns, rows and cells of tables.
  \remarks
  Instances of this class are returned by OdDbTable::getIterator() or OdDbLinkedTableData::getIterator() methods.

  <group OdDb_Classes>
*/
class DBENT_EXPORT OdDbTableIterator : public OdStaticRxObject<OdRxObject>
{
public:
  ODRX_DECLARE_MEMBERS(OdDbTableIterator);

  OdDbTableIterator(const OdDbTable* pTable);

  OdDbTableIterator(const OdDbTable* pTable, const OdCellRange& range, OdDb::TableIteratorOption nOption);

  OdDbTableIterator(const OdDbLinkedTableData* pTable);

  OdDbTableIterator(const OdDbLinkedTableData* pTable, const OdCellRange& range, OdDb::TableIteratorOption nOption);

  OdDbTableIterator(const OdCellRange& range);

  OdDbTableIterator(const OdCellRange& range, OdDb::TableIteratorOption nOption);

  ~OdDbTableIterator();

  static OdSmartPtr<OdDbTableIterator> createObject(const OdDbTable* pTable);
  static OdSmartPtr<OdDbTableIterator> createObject(const OdDbTable* pTable, const OdCellRange& range, OdDb::TableIteratorOption nOption);
  static OdSmartPtr<OdDbTableIterator> createObject(const OdDbLinkedTableData* pTable);
  static OdSmartPtr<OdDbTableIterator> createObject(const OdDbLinkedTableData* pTable, const OdCellRange& range, OdDb::TableIteratorOption nOption);
  static OdSmartPtr<OdDbTableIterator> createObject(const OdCellRange& range);
  static OdSmartPtr<OdDbTableIterator> createObject(const OdCellRange& range, OdDb::TableIteratorOption nOption);

  /** \details
    Sets this Iterator object to the beginning of the iterated range.
  */
  void start(void);

  /** \details
    Increments this Iterator object so that it refers to the next object of the iterated range.
  */
  void step(void);

  /** \details
    Returns a value indicating whether this Iterator object has reached the end of the iterated range.
    \returns True if the end of the iterating range is reached, false otherwise.
  */
  bool done(void);

  /** \details
    Positions this Iterator object at the specified cell.
    \param cell [in] The cell to set the Iterator object at.
    \returns True if the specified cell is found within the iterating range, false otherwise.
    \remarks If the specified cell is not found within the iterating range, the Iterator object remains unchanged.
  */
  bool seek(const OdDbCell& cell);

  /** \details
    Returns the cell this Iterator object currently refers to.
    \returns The object of the OdDbCell class.
  */
  OdDbCell getCell(void) const;

  /** \details
    Returns the index of the row of the current iteration position.
    \returns The index of the row within the iteration range. Returns -1 if columns are currently iterated.
  */
  OdInt32 getRow (void) const;

  /** \details
    Returns the index of the column of the current iteration position.
    \returns The index of the column within the iteration range. Returns -1 if rows are currently iterated.
  */
  OdInt32 getColumn (void) const;

protected:
  OdDbTableIterator(void);

  OdDbTableIterator(OdDbTableIteratorImpl* pImpl);

  OdDbTableIteratorImpl  *m_pImpl;
};

typedef OdSmartPtr<OdDbTableIterator> OdDbTableIteratorPtr;

#include "TD_PackPop.h"

#endif
