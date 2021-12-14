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


#ifndef _OD_TF_Included_
#define _OD_TF_Included_

#include "DbHandle.h"
#include "Ge/GePoint2d.h"
#include "Ge/GePoint3d.h"
#include "Ge/GeScale3d.h"

#include "TD_PackPush.h"

#ifdef TF_DLL_EXPORTS
#define TFDLL_EXPORT           OD_TOOLKIT_EXPORT
#define TFDLL_EXPORT_STATIC    OD_STATIC_EXPORT
#else
#define TFDLL_EXPORT           OD_TOOLKIT_IMPORT
#define TFDLL_EXPORT_STATIC    OD_STATIC_IMPORT
#endif

class OdDbStub;

  /** \details
    This structure is the base for structures that are used for file I/O
    operations with revision control database.
  */
struct OdTfFiler
{
  /** \details
    Returns the database associated with this Filer object.
  */
  virtual OdRxObject* database() const = 0;

  /** \details
    Moves the current position of the file pointer for
    this Filer object to the specified location.

    \param offset [in]  Number of bytes.
    \param seekType [in]  Seek type.

    \remarks
    seekType value must be one of the following:

    <table>
    Name                    Value       Description.
    OdDb::kSeekFromStart    SEEK_SET    Start of file.
    OdDb::kSeekFromCurrent  SEEK_CUR    Current position of file pointer.
    OdDb::kSeekFromEnd      SEEK_END    End of file.
    </table>
  */
  virtual void seek(OdInt64 offset, OdDb::FilerSeekType seekType) = 0;

  /** \details
    Returns the current I/O pointer position of this Filer object.
  */
  virtual OdUInt64 tell() const = 0;

  /** \details
    Reads the boolean value from the current position of the file
    pointer for this Filer object, and increments the pointer.

    \returns
    Read boolean value.
  */
  virtual bool rdBool() = 0;

  /** \details
    Reads the String value from the current position of the file
    pointer for this Filer object, and increments the pointer.

    \returns
    Read String value.
  */
  virtual OdString rdString() = 0;
  
  /** \details
    Reads the specified number of bytes, starting from the current position of
    the file pointer for this Filer object, and increments the pointer by
    numBytes.

    \param buffer  [out]  Read bytes buffer.
    \param numBytes [in]  Number of bytes.
  */
  virtual void rdBytes(
    void* buffer, 
    OdUInt32 numBytes) = 0;

  /** \details
    Reads the Int8 value from the current position of the file
    pointer for this Filer object, and increments the pointer.

    \returns
    Read Int8 value.
  */
  virtual OdInt8 rdInt8() = 0;
  
  /** \details
    Reads the UInt8 value from the current position of the file
    pointer for this Filer object, and increments the pointer.

    \returns
    Read UInt8 value.
  */
  virtual OdUInt8 rdUInt8() = 0;

  /** \details
    Reads the Int16 value from the current position of the file
    pointer for this Filer object, and increments the pointer.

    \returns
    Read Int16 value.
  */
  virtual OdInt16 rdInt16() = 0;

  /** \details
    Reads the Int32 value from the current position of the file
    pointer for this Filer object, and increments the pointer.

    \returns
    Read Int32 value.
  */
  virtual OdInt32 rdInt32() = 0;

  /** \details
    Reads the Int64 value from the current position of the file
    pointer for this Filer object, and increments the pointer.

    \returns
    Read Int64 value.
  */
  virtual OdInt64 rdInt64() = 0;

  /** \details
    Reads the Double value from the current position of the file
    pointer for this Filer object, and increments the pointer.

    \returns
    Read Double value.
  */
  virtual double rdDouble() = 0;

  /** \details
    Reads the DbHandle value from the current position of the file
    pointer for this Filer object, and increments the pointer.

    \returns
    Read DbHandle value.
  */
  virtual OdDbHandle rdDbHandle() = 0;

  /** \details
    Reads the OwnershipId value from the current position of the file
    pointer for this Filer object, and increments the pointer.

    \returns
    Read OwnershipId value.
  */
  virtual OdDbStub* rdSoftOwnershipId() = 0;

  /** \details
    Reads the HardOwnershipId value from the current position of the file
    pointer for this Filer object, and increments the pointer.

    \returns
    Read HardOwnershipId value.
  */
  virtual OdDbStub* rdHardOwnershipId() = 0;

  /** \details
    Reads the HardPointerId value from the current position of the file
    pointer for this Filer object, and increments the pointer.

    \returns
    Read HardPointerId value.
  */
  virtual OdDbStub* rdHardPointerId() = 0;

  /** \details
    Reads the SoftPointerId value from the current position of the file
    pointer for this Filer object, and increments the pointer.

    \returns
    Read SoftPointerId value.
  */
  virtual OdDbStub* rdSoftPointerId() = 0;

  /** \details
    Reads the Point2d value from the current position of the file
    pointer for this Filer object, and increments the pointer.

    \returns
    Read Point2d value.
  */
  virtual OdGePoint2d rdPoint2d() = 0;
    
  /** \details
    Reads the Point3d value from the current position of the file
    pointer for this Filer object, and increments the pointer.

    \returns
    Read Point3d value.
  */
  virtual OdGePoint3d rdPoint3d() = 0;

  /** \details
    Reads the Vector2d value from the current position of the file
    pointer for this Filer object, and increments the pointer.

    \returns
    Read Vector2d value.
  */
  virtual OdGeVector2d rdVector2d() = 0;

  /** \details
    Reads the Vector3d value from the current position of the file
    pointer for this Filer object, and increments the pointer.

    \returns
    Read Vector3d value.
  */
  virtual OdGeVector3d rdVector3d() = 0;
  
  /** \details
    Reads the Scale3d value from the current position of the file
    pointer for this Filer object, and increments the pointer.

    \returns
    Read Scale3d value.
  */
  virtual OdGeScale3d rdScale3d() = 0;

  /** \details
    Writes the specified boolean value at the current position of the file
    pointer for this Filer object, and increments the pointer.

    \param value [in] Value to write.
  */
  virtual void wrBool(
    bool value) = 0;

  /** \details
    Writes the specified String value at the current position of the file
    pointer for this Filer object, and increments the pointer.

    \param value [in] Value to write.
  */
  virtual void wrString(
    const OdString &value) = 0;
    
  /** \details
    Writes the specified number of bytes, starting from the current position of
    the file pointer for this Filer object, and increments the pointer by
    numBytes.

    \param buffer  [in]  Array of bytes to write.
    \param numBytes [in]  Number of bytes.
  */
  virtual void wrBytes(
    const void* buffer,
    OdUInt32 numBytes) = 0;

  /** \details
    Writes the specified Int8 value at the current position of the file
    pointer for this Filer object, and increments the pointer.

    \param value [in] Value to write.
  */
  virtual void wrInt8(
    OdInt8 value) = 0;
    
  /** \details
    Writes the specified UInt8 value at the current position of the file
    pointer for this Filer object, and increments the pointer.

    \param value [in] Value to write.
  */
  virtual void wrUInt8(
    OdUInt8 value) = 0;

  /** \details
    Writes the specified Int16 value at the current position of the file
    pointer for this Filer object, and increments the pointer.

    \param value [in] Value to write.
  */
  virtual void wrInt16(
    OdInt16 value) = 0;
    
  /** \details
    Writes the specified Int32 value at the current position of the file
    pointer for this Filer object, and increments the pointer.

    \param value [in] Value to write.
  */
  virtual void wrInt32(
    OdInt32 value) = 0;

  /** \details
    Writes the specified Int64 value at the current position of the file
    pointer for this Filer object, and increments the pointer.

    \param value [in] Value to write.
  */
  virtual void wrInt64(
    OdInt64 value) = 0;

  /** \details
    Writes the specified Double value at the current position of the file
    pointer for this Filer object, and increments the pointer.

    \param value [in] Value to write.
  */
  virtual void wrDouble(double value) = 0;

  /** \details
    Writes the specified DbHandle value at the current position of the file
    pointer for this Filer object, and increments the pointer.

    \param value [in] Value to write.
  */
  virtual void wrDbHandle(
    const OdDbHandle& value) = 0;

  /** \details
    Writes the specified SoftOwnershipId value at the current position of the
    file pointer for this Filer object, and increments the pointer.

    \param value [in] Value to write.
  */
  virtual void wrSoftOwnershipId(
    OdDbStub* value) = 0;
    
  /** \details
    Writes the specified HardOwnershipId value at the current position of the
    file pointer for this Filer object, and increments the pointer.

    \param value [in] Value to write.
  */
  virtual void wrHardOwnershipId(
    OdDbStub* value) = 0;

  /** \details
    Writes the specified SoftPointerId value at the current position of the
    file pointer for this Filer object, and increments the pointer.

    \param value [in] Value to write.
  */
  virtual void wrSoftPointerId(
    OdDbStub* value) = 0;

  /** \details
    Writes the specified HardPointerId value at the current position of the
    file pointer for this Filer object, and increments the pointer.

    \param value [in] Value to write.
  */
  virtual void wrHardPointerId(
    OdDbStub* value) = 0;

  /** \details
    Writes the specified Point2d value at the current position of the
    file pointer for this Filer object, and increments the pointer.

    \param value [in] Value to write.
  */
  virtual void wrPoint2d(
    const OdGePoint2d& value) = 0;

  /** \details
    Writes the specified Point3d value at the current position of the
    file pointer for this Filer object, and increments the pointer.

    \param value [in] Value to write.
  */
  virtual void wrPoint3d(
    const OdGePoint3d& value) = 0;
  
  /** \details
    Writes the specified Vector2d value at the current position of the
    file pointer for this Filer object, and increments the pointer.

    \param value [in] Value to write.
  */
  virtual void wrVector2d(
    const OdGeVector2d& value) = 0;
  
  /** \details
    Writes the specified Vector3d value at the current position of the
    file pointer for this Filer object, and increments the pointer.

    \param value [in] Value to write.
  */
  virtual void wrVector3d(
    const OdGeVector3d& value) = 0;
  
  /** \details
    Writes the specified Scale3d value at the current position of the
    file pointer for this Filer object, and increments the pointer.

    \param value [in] Value to write.
  */
  virtual void wrScale3d(
    const OdGeScale3d& value) = 0;

  /** \details
    Writes the specified Subobject data at the current position of the file
    pointer for this Filer object, and increments the pointer.

    \param id   [in] Subobject ID.
    \param name [in] Subobject name.
  */
  virtual void wrSubobject(int id, const char* name) = 0;

  /** \details
    Reads the specified Subobject data from the current position of the file
    pointer for this Filer object, and increments the pointer.

    \param id [in] Subobject ID.

    \returns
    Subobject name.
  */
  virtual const char* rdSubobject(int& id) = 0;

  /** \details
    Writes the specified TimeStamp value at the current position of the file
    pointer for this Filer object, and increments the pointer.

    \param value [in] Value to write.
  */
  virtual void wrDateTime(OdTimeStamp) = 0;

  /** \details
    Reads the TimeStamp value from the current position of the file
    pointer for this Filer object, and increments the pointer.

    \param value [out] Read value.
  */
  virtual void rdDateTime(OdTimeStamp&) = 0;
};


struct TFDLL_EXPORT OdTfObjectPE : OdRxObject
{
  ODRX_DECLARE_MEMBERS(OdTfObjectPE);
  virtual int schemaVersion(OdRxObject*) = 0;
  virtual OdString schema(OdRxObject*) = 0;
  virtual bool in(OdRxObject*, OdTfFiler*) = 0;
  virtual void out(OdRxObject*, OdTfFiler*) = 0;
};

typedef OdSmartPtr<OdTfObjectPE> OdTfObjectPEPtr;

namespace OdTf
{
  enum TagsEnum {
    Element = 1,
    Condition = 2,
    Array = 4,
    version = 8,
    EndClass = 16,
    Mark = 32,

    EndTagMask = 64
  };

  enum ClassVersionEnum
  {
    originalVersion = 0,
    addInVersion = 1,
    deletedInVersion = 2
  };

  enum ElementAttributesEnum {
    isCacheValue = 1,
    isBreak = 2,
    isTableName = 4,
    isContextData = 8,
    isTableQueue = 16,
    isTableCache = 32,
    TableQueue = 64,
    isSum = 128
  };

  enum ConditionAttributesEnum {
    isMarkCondition = 1,
    isOwner = 2,
    dwgVer = 4
  };

  enum ArrayAttributesEnum {
    breakValue = 1,
    increaseOn = 2,
    decreaseOn = 3,
  };

  enum ConditionOperatorsEnum {
    kTrue = 1,
    kFalse = 2,
    kNot = 4,
    kAnd = 8,
    kLessThan = 16,
    kGreaterThan = 32
  };

  enum HexBinaryAttributesEnum {
    increaseIn = 1,
    reduceIn = 2,
    limitLen = 4,
    countBytes = 8,
    mul = 16,
    value = 32,
    cacheValue = 64
  };

  enum TypesEnum
  {
    Subclass,
    Bool,
    Byte,
    Short,
    Int,
    UnsignedByte,
    UnsignedShort,
    UnsignedInt,
    Long,
    Double,
    HexBinary,
    String,
    Handle,
    SoftOwnershipId,
    HardOwnershipId,
    SoftPointerId,
    HardPointerId,
    Point2d,
    Point3d,
    Vector2d,
    Vector3d,
    Scale3d,
    DateTime,
    DataEnd,
    Table,
    Empty,
    DoubleWithDefault
  };
}


#include "TD_PackPop.h"

#endif // _OD_TF_Included_
