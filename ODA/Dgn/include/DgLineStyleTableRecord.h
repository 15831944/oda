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
#ifndef __DG_LINESTYLETABLERECORD_H__
#define __DG_LINESTYLETABLERECORD_H__


#include "TD_PackPush.h"

#include "DgTableRecord.h"

/** \details
  
    <group OdDg_Classes>
*/
namespace OdDg
{
  /** \details
   
    Corresponding C++ library: TG_Db

    <group OdDg_Classes>
  */
  enum LineStyleType
  {
    kLsTypeInternal      = 0,
    kLsTypePointSymbol   = 1,
    kLsTypeCompound      = 2,
    kLsTypeLineCode      = 3,
    kLsTypeLinePoint     = 4,

    kLsTypeUndefined   = -1
  };

  /** \details
   
    Corresponding C++ library: TG_Db

    <group OdDg_Classes>
  */
  enum LineStyleUnitsType
  {
    kLsMasterUnits     = 0,
    kLsUORS            = 1,
    kLsDeviceUnits     = 2
  };
}

/** \details
    This class represents LineStyle records in the OdDgLineStyleTable of an OdDgDatabase instance.

    Corresponding C++ library: TG_Db
    
    <group OdDg_Classes>
*/
class TG_EXPORT OdDgLineStyleTableRecord : public OdDgTableRecord
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgLineStyleTableRecord);
public:

  /** \details
    Returns the entry ID of the linestyle table record.

   \remarks
    The first entry of a linestyle table record is 9. Entry IDs
    0-8 are reserved for internal linestyles, linestyle by level
    and linestyle by cell.
  */
  OdUInt32 getEntryId() const;

/** \details
  Returns the "refers to element" flag of the linestyle table record.

 \remarks
   If the flag is true, the linestyle definition of the style is stored 
   in the linestyle definition table of the file. The ID of the definition must 
   be retrieved using the getRefersToId() method. Otherwise if the flag is false, the linestyle 
   definition is stored in the RSC file.
*/
  bool getRefersToElementFlag() const;

/** \details
  Sets the "refers to element" flag of the linestyle table record.

  \param val [in] Value of the "refers to element" flag.

*/
  void setRefersToElementFlag(bool val);

/** \details
  Returns the snappable flag of the linestyle table record.

 \remarks
   If true, snap to geometry of a linestyle (dashes, symbols, etc.)
   is enabled. Otherwise the geometry of the linestyle is ignored during the snap process.
*/
  bool getSnappableFlag() const;

/** \details
  Sets the snappable flag of the linestyle table record.

  \param val [in] Value of the snappable flag.
*/
  void setSnappableFlag(bool val);

/** \details
  Returns the physical flag of the linestyle table record.

 \remarks
   If true, modifications to model and reference attachment linestyle scales 
   are ignored.
*/
  bool getPhysicalFlag() const;

/** \details
  Sets the physical flag of the linestyle table record.

  \param val [in] Value of the physical flag.
*/
  void setPhysicalFlag(bool val);

/** \details
  Returns the unit type of the linestyle.

  \remarks
    Defines the scale conversion from linestyle definition units to model units.
*/
  OdDg::LineStyleUnitsType getUnitsType() const;

/** \details
  Sets the unit type of the linestyle.

  \param val [in] Unit type of the linestyle.
*/
  void setUnitsType(OdDg::LineStyleUnitsType val);

/** \details
  Returns the no-snap flag of the linestyle table record.

 \remarks
   If true, snapping to geometry of elements with the linestyle is disabled. 
*/
  bool getNoSnapFlag() const;

/** \details
  Sets the no-snap flag of the linestyle table record.

  \param val [in] Value of the no-snap flag.
*/
  void setNoSnapFlag(bool val);

/** \details
  Returns the continuous flag of the linestyle table record.

 \remarks
   If true, the linestyle is drawn as continuous (a solid line).
*/
  bool getContinuousFlag() const;

/** \details
  Sets the continuous flag of the linestyle table record.

  \param val [in] Value of the continuous flag.
*/
  void setContinuousFlag(bool val);

/** \details
  Returns the no-range flag of the linestyle table record.

 \remarks
   If true, ranges of elements with the linestyle are not calculated.
*/
  bool getNoRangeFlag() const;

/** \details
  Sets the no-range flag of the linestyle table record.

  \param val [in] Value of the no-range flag.
*/
  void setNoRangeFlag(bool val);

/** \details
  Returns the "shared-cell scale independent" flag of the linestyle table record.

  \remarks
    If true, the shared-cell scale is independent of the linestyle scale.
*/
  bool getSharedCellScaleIndependentFlag() const;

/** \details
  Sets the "shared-cell scale independent" flag of the linestyle table record.

  \param val [in] Value of the "shared-cell scale independent" flag.
*/
  void setSharedCellScaleIndependentFlag(bool val);

/** \details
  Returns the no-width flag of the linestyle table record.

  \remarks
    If true, line width modifications are not applied to the linestyle.
*/
  bool getNoWidthFlag() const;

/** \details
  Sets the no-width flag of the linestyle table record.

  \param val [in] Value of the no-width flag.
*/
  void setNoWidthFlag(bool val);

/** \details
  Returns the ID of a linestyle definition table record for linestyles with the "refers to element" 
  flag set.
*/
  OdDgElementId getRefersToId() const;

/** \details
  Sets the ID of a linestyle definition table record that has the "refers to element" flag set.

  \param val [in] Element ID of the linestyle definition.
*/
  void setRefersToId(const OdDgElementId& val);

/** \details
  Returns the resource type of the linestyle definition of the linestyle.
*/
  OdDg::LineStyleType getType() const;

/** \details
  Sets the resource type of the linestyle definition of the linestyle.

  \param val [in] Resource type of the linestyle definition.
*/
  void setType(OdDg::LineStyleType val);

/** \details
  Returns the name of the source RSC file.
*/
  OdString getRscFileName() const;

/** \details
  Sets the name of the source RSC file.

  \param strFileName [in] Filename of the source RSC file.
*/
  void     setRscFileName( const OdString& strFileName );

  // OdGiDrawable overridden
  OdUInt32 subSetAttributes(OdGiDrawableTraits* pTraits) const;

  /** \details
    Resets the linestyle data cache and reloads the linestyle definition.
  */
  void updateLineStyleData();

  /** \details
  Called as the first operation as this object is being erased or unerased.

  \param erasing [in]  A copy of the erasing argument passed to erase().

  \remarks
  This function is notified just before the current object is to be erased, giving this function
  the ability to cancel the erase.

  Returns eOk if and only if erase() is to continue.

  When overriding this function:

  1)  If the OdDgElement's state is incorrect, return
      something other than eOk.

  2)  If the parent class's subErase() returns anything
      other than eOk, immediately return it.

  3)  If other actions are required before the erase, do them.

  4)  Return eOk.

  If you must make changes to this object's state, either make them after
  step 2, or roll them back if step 2 returns anything other than eOk.

  The default implementation of this function does nothing but return eOk. This function can be
  overridden in custom classes.

*/
  virtual OdResult subErase(bool erasing);
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDgLineStyleTableRecord object pointers.
*/
typedef OdSmartPtr<OdDgLineStyleTableRecord> OdDgLineStyleTableRecordPtr;


#include "TD_PackPop.h"

#endif // __DG_LINESTYLETABLERECORD_H__
