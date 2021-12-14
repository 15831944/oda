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



#ifndef _ODRXVALUETYPE_INCLUDED_
#define _ODRXVALUETYPE_INCLUDED_

#include <memory.h>
#include "RxObject.h"
#include "OdString.h"

class OdRxProperty;
class OdRxValue;
class OdRxEnumTag;

/** <group !!RECORDS_tkernel_apiref>
  \details
  This interface is implemented by the value types having non-trivial
  constructor and/or destructor.
*/
struct IOdRxNonBlittableType
{
  /** \details
    Called to construct a copy of the object.

    \param dest   [in] Memory location to construct an object at.
    \param source [in] Pointer to the source object.
  */
  virtual void construct(void* dest, const void* source) const = 0;

  /** \details
    Called to assign the object.

    \param dest   [in] Pointer to the destination object.
    \param source [in] Pointer to the source object.
  */
  virtual void assign(void* dest, const void* source) const = 0;

  /** \details
    Called to destroy the object.

    \param instance [in] Pointer to the object.
  */
  virtual void destruct(const void* instance) const = 0;
};

/** <group !!RECORDS_tkernel_apiref>
  \details
  This interface is implemented by the value types describing enumerations.
*/
struct IOdRxEnumeration
{
  /** \details
    Returns the number of items in the enum.

    \returns
    The number of items in the enum.
  */
  virtual int count() const = 0;

  /** \details
    Returns the descriptor of the given enum item.

    \param index [in] Index of the item.

    \returns
    Descriptor of the given enum item.
  */
  virtual const OdRxEnumTag& getAt(int index) const = 0;
};

/** <group !!RECORDS_tkernel_apiref>
  \details
  This interface is implemented by the value types describing objects that can
  be "opened"/"closed", like OdDbObjectId in Drawings project.
*/
struct IOdRxReferenceType
{
  /** \details
    This enumeration describes access modes for referenced objects.
  */
  enum OpenMode 
  {
    kForRead = 0,  //For read.
    kForWrite = 1, //For write.
    kForNotify = 2 //For notification.
  };

  /** \details
    Open the object in the specified mode.

    \param value [in] Value object to open.
    \param mode  [in] Mode to open the value object in.

    \returns
    Smart pointer to opened object.

    \remarks
    Returned object will be automatically closed when the smart pointer is
    released, as usual.
  */
  virtual OdRxObjectPtr dereference(const OdRxValue& value, OpenMode mode) const = 0;
};

/** <group !!RECORDS_tkernel_apiref>
  \details
  This interface is implemented by the value types describing pointers to
  OdRxObject descendants.
*/
struct IOdRxObjectValue
{
  /** \details
    Return contained pointer as an OdRxObject.
    \param value [in] Value containing pointer to an OdRxObject descendant.
    \returns
    Contained pointer as an OdRxObject.
  */
  virtual const OdRxObject* getRxObject(const OdRxValue& value) const = 0;
};

/** \details
  Definition of the OdRxValue pseudoconstructor.
*/
typedef OdRxValue (*OdRxValueConstructor)();


/** <group OdRx_Classes>
  \details
  Metaclass (description of the contained type) of the OdRxValue.
  Instances of this class are registered in the class dictionary.
  See RxValueTypeUtil.h for examples of specific value types and basic helper
  types.
*/
class FIRSTDLL_EXPORT OdRxValueType : public OdRxClass
{
  void addRef() {}
  void release() {}
public:
  //DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdRxValueType);
  //DOM-IGNORE-END

  /** \details
    Destructor for objects of the OdRxValueType class.
  */
  ~OdRxValueType();

  /** \details
    Comparison operator.

    \param rhs [in] Value type to compare with the current value type.

    \returns
    True if value types are equal; false otherwise.
  */
  bool operator==(const OdRxValueType& rhs) const
  {
    return this == &rhs;
  }

  /** \details
    Negative comparison operator.

    \param rhs [in] Value type to compare with the current value type.

    \returns
    True if value types are not equal; false otherwise.
  */
  bool operator!=(const OdRxValueType& rhs) const
  {
    return this != &rhs;
  }

  /** \details
    Returns the amount of memory to store the value of the type (like sizeof(T)).

    \returns
    Amount of memory to store the value of the type. (sizeof(T))
  */
  unsigned int size() const { return m_size; }

  /** \details
    Indicates whether the value type is blittable.

    \returns
    True for the POD types (value may be copied by memcpy). False for other types.
  */
  bool isBlittable() const { return nonBlittable() == 0; }

  /** \details
    Indicates whether the value type is enumeration.

    \returns
    True for the enumerations. False for other types.
  */
  bool isEnum() const { return enumeration() != 0; }

  /** \details
    Indicates whether the value type is handler.

    \returns
    True for object handlers value types, like OdDbObjectId. False for other
    value types.
  */
  bool isReference() const { return reference() != 0; }

  /** \details
    Indicates whether the value type is SDAI select data type.
    
    \returns
    True for SDAI select data type. False for other value types.
  */
  virtual bool isSelect() const { return false; }

  /** \details
    Indicates whether the value type is SDAI aggregate data type.

    \returns
    True for SDAI aggregate data type. False for other value types.
  */
  virtual bool isAggregate() const { return false; }

  /** \details
    Returns the interface, that handles copying/destroying values for non-POD
    types.

    \returns
    Interface, handling copying/destroying values for non-POD types. NULL for
    POD types.
  */
  virtual const IOdRxNonBlittableType* nonBlittable() const { return 0; }

  /** \details
    Returns the interface, that describes enumeration.

    \returns
    Interface, describing enumeration. NULL for non-enumeration types.
  */
  virtual const IOdRxEnumeration* enumeration() const { return 0; }

  /** \details
    Returns the interface, that describes references (handlers).

    \returns
    Interface, describing references. NULL for non-reference types.
  */
  virtual const IOdRxReferenceType* reference() const { return 0; }

  /** \details
    Returns the interface, that describes OdRxObject-descendant pointers.

    \returns
    Interface, describing OdRxObject-descendant pointers. NULL for types other
    than OdRxObject-descendant pointers.
  */
  virtual const IOdRxObjectValue* rxObjectValue() const { return 0; }

  /** \details
    Create a new wrapped value of this type.

    \returns
    Created wrapped value of this type.
  */
  virtual OdRxValue createValue() const;
  
  /** \details
    Create new OdRxValue of this type, wrapped in OdRxBoxedValue (if you need a
    smart pointer).

    \returns
    Created OdRxBoxedValue object containing an OdRxValue object of this type.
  */
  virtual OdRxObjectPtr create() const ODRX_OVERRIDE;

  /** \details
    Convert value to the specified type from this type.

    \param vt       [in] Type to convert to.
    \param instance [in] Value of "this" type.
    \param result  [out] Resulting value of the type specified in vt.

    \returns
    True if conversion is possible. False otherwise.
  */
  virtual bool toValueType(const OdRxValueType &vt, const OdRxValue& instance, OdRxValue &result) const
  {
    return subToValueType(vt, instance, result);
  }

  /** \details
    Convert value from the specified type (type of the "from" value) to this
    type.

    \param from      [in] Value of type to convert from.
    \param instance [out] Resulting value of "this" type.

    \returns
    True if conversion is possible. False otherwise.
  */
  virtual bool fromValueType(const OdRxValue &from, OdRxValue& instance) const
  {
    return subFromValueType(from, instance);
  }

  /** \details
    Retreives an ANSI string in format of path that elaborates the sense of
    value of the value type. Can be interpreted additionally when working with
    RxValue.

    \param instance [in] Value, the sense of which must be clarified.

    \returns
    ANSI string with clarification of value sense.
  */
  virtual const OdAnsiString typePath(const OdRxValue& instance) const
  {
    return subTypePath(instance);
  }

  /** \details
    This enumeration defines string formats for converting values, contained in
    OdRxValue objects to strings.
  */
  enum StringFormat
  {
    kStringFormatGlobal = 0,  //Global locale string format.
    kStringFormatCurrent = 1, //Current string format.
  };

  /** \details
    Convert value to string.

    \param instance [in] Instance of the value to convert.
    \param format   [in] Defines the format of the resulting string.

    \returns
    Converted value.

    \remarks
    Mostly for debug purposes.
  */
  virtual OdString toString(const void* instance, StringFormat format = kStringFormatGlobal) const
  {
    return subToString(instance, format);
  }

  /** \details
    Compares two values of the underlying C++ type.

    \param a [in] First value to compare.
    \param b [in] Second value to compare.

    \returns
    True if compared values are equal; false otherwise.
  */
  bool equalTo(const void* a, const void* b) const
  {
    ODA_ASSERT(a != NULL);
    if (a == NULL)
      return false;
    ODA_ASSERT(b != NULL);
    if (b == NULL)
      return false;
    return subEqualTo(a, b);
  }

  /** \details
    Specializations of this class connect C++ classes with corresponding
    OdRxValueType classes.
  */
  template<typename ValueType> struct Desc
  {
    /** \details
      Returns a singleton instance of the OdRxValueType representing ValueType.

      \returns
      Instance of the corresponding OdRxValueType class.

      \remarks
      First call of this function also registers the value type in the class
      dictionary.
    */
    static const OdRxValueType& value() throw();
    
    /** \details
      Unregister and delete the value type.
    */
    static void del();
  };
protected:
  OdRxValueType(const OdChar* name, unsigned int size, OdRxMemberCollectionConstructorPtr memberConstruct, void *userData = NULL);
private:
  virtual OdString subToString(const void* instance, StringFormat format) const = 0;
  virtual bool subEqualTo(const void* a, const void* b) const = 0;
  virtual bool subToValueType(const OdRxValueType &vt, const OdRxValue& instance, OdRxValue &in) const { return false; };
  virtual bool subFromValueType(const OdRxValue& from, OdRxValue &instance) const { return false; };
  virtual const OdAnsiString subTypePath(const OdRxValue& instance) const { return ""; };
  OdRxValueType(const OdRxValueType& rhs);
  OdRxValueType& operator=(const OdRxValueType& rhs);
  unsigned int m_size;
};

/** \details
  Smart pointer to objects of the OdRxValueType class.
*/
typedef OdSmartPtr<OdRxValueType> OdRxValueTypePtr;

/** \details
  Specialization for 'no type'.  

  <group OdRx_Classes>
*/
template<> struct OdRxValueType::Desc<void>
{
  /** \details
    Constructor for 'no type' objects.

    \returns
    Constructed object.
  */
  FIRSTDLL_EXPORT static const OdRxValueType& value() throw();

  /** \details
    Destructor for 'no type' objects.
  */
  FIRSTDLL_EXPORT static void del();
};

//DOM-IGNORE-BEGIN
class OdRxValueStorage; //this class is never defined, only used in placement new/delete
#ifdef _MSC_VER
#pragma push_macro("new")
#undef new
#endif

inline void* operator new(size_t, OdRxValueStorage* loc) { return loc; }

#ifdef _MSC_VER
#pragma pop_macro("new")
#pragma push_macro("delete")
#undef delete
#endif

inline void operator delete(void* , OdRxValueStorage* ) { }
#ifdef _MSC_VER
#pragma pop_macro("delete")
#endif
//DOM-IGNORE-END

#endif // _ODRXVALUETYPE_INCLUDED_
