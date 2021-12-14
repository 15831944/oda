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

#ifndef _ODRXVALUE_INCLUDED_
#define _ODRXVALUE_INCLUDED_

#include "RxValueType.h"
#include "RxMember.h"
#include "StringArray.h"
#include "IntArray.h"
#include "Ge/GeDoubleArray.h"
#include "StaticRxObject.h"
#include "CmColorBase.h"
#include "DbHandle.h"

/** \details
  Safe cast. Returns non-NULL only if ValueType is contained in this value.

  \param value [in] Value which to cast a value type from.

  \returns
  Value type if ValueType is contained in the specified value. NULL otherwise.
*/
template<typename ValueType> ValueType* rxvalue_cast(OdRxValue* value) throw();

/** \details
  Safe cast for enumerations. Returns non-NULL only if ValueType is contained in
  this value.

  \param value [in] Enumeration value which to cast a value type from.

  \returns
  Value type if ValueType is contained in the specified enumeration value. NULL
  otherwise.
*/
template<typename ValueType> ValueType * rxenum_cast(OdRxValue * value) throw();

/** \details
  Const correct safe cast. Returns non-NULL only if ValueType is contained in
  this value.

  \param value [in] Value which to cast a value type from.

  \returns
  Value type if ValueType is contained in the specified value. NULL otherwise.
*/
template<typename ValueType> const ValueType * rxvalue_cast(const OdRxValue * value) throw();

/** \details
  Const correct safe cast for enumerations. Returns non-NULL only if ValueType
  is contained in this value.

  \param value [in] Enumeration value which to cast a value type from.

  \returns
  Value type if ValueType is contained in the specified enumeration value. NULL
  otherwise.
*/
template<typename ValueType> const ValueType * rxenum_cast(const OdRxValue * value) throw();

#include "TD_PackPush.h"

/** <group OdRx_Classes>
  \details
  Generic variant type. Can contain any C++ type. Contained type is described by
  OdRxValueType.
*/
class FIRSTDLL_EXPORT OdRxValue
{
public:
  /** \details
    Default constructor for objects of the OdRxValue class.
  */
  OdRxValue() throw() : m_type(OdRxValueType::Desc<void>::value())
  {
    memset(&m_value, 0, sizeof(m_value));
  }

  /** \details
    Copy constructor for objects of the OdRxValue class. Clones an existing
    instance of the OdRxValue class.

    \param rhs [in] Object of the OdRxValue class to be cloned.
  */
  OdRxValue(const OdRxValue& rhs) throw() : m_type(rhs.m_type)
  {
    init(rhs, false);
  }

  /** \details
    Constructor for objects of the OdRxValue class. Creates an OdRxValue object
    with the specified type and value. Source value should be compatible with
    the target type, e.g. enumeration can be constructed from integer (like
    reinterpret_cast<> for underlying C++ type).

    \param type  [in] Value type of the object to be created.
    \param value [in] Value to be assigned to the created object.
  */
  OdRxValue(const OdRxValueType& type, const OdRxValue& value) throw() : m_type(type)
  {
    init(value, false);
  }

  /** \details
    Assignment operator.

    \param rhs [in] Value to be assigned.
  */
  const OdRxValue& operator=(const OdRxValue& rhs) throw()
  {
    if (this == &rhs)
      return *this;
    if (type() != rhs.type())
    {
      if (!type().isBlittable())
        type().nonBlittable()->destruct(valuePtr());
      if (!isInlined() && rhs.isInlined())
        deallocate(m_value.m_ptr);
#ifdef _MSC_VER
#pragma push_macro("new")
#undef new
#endif
      ::new ((OdRxValueStorage*)this) OdRxValue(rhs, !isInlined() && !rhs.isInlined());
#ifdef _MSC_VER
#pragma pop_macro("new")
#endif
      return *this;
    }
    bool blittable = rhs.type().isBlittable();
    bool inlined = rhs.isInlined();
    if (blittable && inlined)
    {
      memcpy(this, &rhs, sizeof(OdRxValue));
      return *this;
    }
    /*
    inlined,non-blittable
    non-inlined, non-blittable
    non-inlined, blittable
    */
    if (inlined)
      type().nonBlittable()->assign(inlineValuePtr(), rhs.inlineValuePtr());
    else
      setNonInlineValue(rhs.nonInlineValuePtr(), blittable, true, true);
    return *this;
  }

  /** \details
    Default destructor for objects of the OdRxValue class.
  */
  ~OdRxValue() throw()
  {
    //if the type is non-blittable then we must call destructor
    if (!type().isBlittable())
      type().nonBlittable()->destruct(valuePtr());
    //finally free memory if necessary
    if (!isInlined())
      deallocate(m_value.m_ptr);
  }

  /** \details
    Get metaclass describing C++ type of the stored value.

    \returns
    Metaclass describing C++ type of the stored value.
  */
  const OdRxValueType& type() const throw()
  {
    return m_type;
  }

  /** \details
    Checks if the value is empty.

    \returns
    True if the value is empty; false otherwise.
  */
  bool isEmpty() const throw() { return *this == empty(); }

  /** \details
    Returns canonical empty value.

    \returns
    Canonical empty value.
  */
  static const OdRxValue& empty() throw();

  /** \details
    Returns true if the value is undefined. This is represented by assigning
    special singleton value "varies".

    \returns
    True if the value is undefined; false otherwise.
  */
  bool isVaries() const throw() { return *this == varies(); }

  /** \details
    Returns singleton, representing undefined value.

    \returns
    Singleton, representing undefined value.
  */
  static const OdRxValue& varies() throw();

  /** \details
    Convert value to string.

    \param format [in] Defines the format of the resulting string.

    \returns
    Converted value.

    \remarks
    Mostly for debug purposes.
  */
  OdString toString(OdRxValueType::StringFormat format = OdRxValueType::kStringFormatGlobal) const throw()
  {
    return m_type.toString(valuePtr(), format);
  }

  /** \details
    Comparison operator.

    \param value [in] Value to compare with the current value.

    \returns
    True if values are equal; false otherwise.
  */
  bool operator==(const OdRxValue& value) const throw()
  {
    if (type() != value.type())
      return false;
    return type().equalTo(valuePtr(), value.valuePtr());
  }

  /** \details
    Negative comparison operator.

    \param value [in] Value to compare with the current value.

    \returns
    True if values are not equal; false otherwise.
  */
  bool operator!=(const OdRxValue& value) const throw()
  {
    return !(operator == (value));
  }

  /** \details
    Retreives an ANSI string in format of path that elaborates the sense of
    value of the value type. Can be interpreted additionally when working with
    RxValue.

    \returns
    ANSI string with clarification of value sense.
  */
  const OdAnsiString typePath() const
  {
    return m_type.typePath(*this);
  }

  /** \details
    This constructor can be used for blittable (POD) types. For non-blittable
    types specialization should be declared.

    \param value [in] Value type for the constructed value object.
  */
  template <typename ValueType>
  OdRxValue(const ValueType& value) throw()
    :m_type(OdRxValueType::Desc<ValueType>::value())
  {
    //this should have been specialized otherwise
    ODA_ASSERT(m_type.isBlittable());
    initBlittable<sizeof(ValueType) <= 24>(&value, sizeof(ValueType));
  }

  /** \details
    Safe cast to the contained C++ type. Returns non-null only if ValueType is
    contained in this value.

    \param value [in] Value to be casted.

    \returns
    Value type if it is contained in value; NULL otherwise.
  */
  template<typename ValueType>
  friend ValueType* rxvalue_cast(OdRxValue* value) throw()
  {
    return value && OdRxValueType::Desc<ValueType>::value() == value->type() ? (ValueType*)(value->valuePtr__<sizeof(ValueType) <= 24>()) : 0;
  }

  /** \details
    Safe cast for enumerations. Returns non-null only if ValueType is contained
    in this value.

    \param value [in] Value to be casted.

    \returns
    Value type if it is contained in value; NULL otherwise.
  */
  template<typename ValueType>
  friend ValueType * rxenum_cast(OdRxValue * value) throw()
  {
    ODA_ASSERT(value == NULL || value->isVaries() || value->type().isEnum());
    return value &&
      value->type().isEnum() &&
      OdRxValueType::Desc<ValueType>::value() == value->type().enumeration()->getAt(0).type() ? (ValueType*)(value->valuePtr__<sizeof(ValueType) <= 24>()) : 0;
  }

  /** \details
    This assignment operator can be optionally specialized.

    \param rhs [in] Value type to be assigned to this value.

    \returns
    This value.
  */
  template<typename ValueType>
  OdRxValue& operator=(const ValueType & rhs) throw()
  {
    *this = OdRxValue(rhs);
    return *this;
  }

  /** \details
    Constant correct safe cast version. Returns non-null only if ValueType is
    contained in this value.

    \param value [in] Value to be casted.

    \returns
    Value type if it is contained in value; NULL otherwise.
  */
  template<typename ValueType>
  friend inline const ValueType * rxvalue_cast(const OdRxValue * value) throw()
  {
    return rxvalue_cast<ValueType>(const_cast<OdRxValue*>(value));
  }

  /** \details
    Constant correct safe cast for enumerations. Returns non-null only if
    ValueType is contained in this value.

    \param value [in] Value to be casted.

    \returns
    Value type if it is contained in value; NULL otherwise.
  */
  template<typename ValueType>
  friend inline const ValueType * rxenum_cast(const OdRxValue * value) throw()
  {
    return rxenum_cast<ValueType>(const_cast<OdRxValue*>(value));
  }

  /** \details
    Unwraps RxValue contained in OdRxBoxedValue.

    \param pO [in] OdRxBoxedValue to be unboxed.

    \returns
    Unwrapped value.
  */
  static const OdRxValue* unbox(const OdRxObject* pO) throw();

  /** \details
    Unwraps RxValue contained in OdRxBoxedValue.

    \param pO [in] OdRxBoxedValue to be unboxed.

    \returns
    Unwrapped value.
  */
  static OdRxValue* unbox(OdRxObject* pO) throw();

  /** \details
    Get the corresponding descriptor, if this value contains an enumeration
    item.

    \returns
    Enumeration item descriptor.
  */
  const OdRxEnumTag* getEnumTag() const throw();

  /** \details
    Store value in the byte array.

    \param pBytes         [out] Pointer to the byte array.
    \param maxBytesToWrite [in] Maximum number of bytes to write.

    \returns
    Actual number of bytes written.
  */
  size_t serializeOut(void* pBytes, size_t& maxBytesToWrite) const;

  /** \details
    Read value from the byte array.

    \param pBytes        [out] Pointer to the byte array.
    \param maxBytesToRead [in] Maximum number of bytes to read.

    \returns
    Actual number of bytes read.
  */
  size_t serializeIn(const void* pBytes, size_t maxBytesToRead);

  /** \details
    Assignment operator. Assigns a value from a C++ variable to this value.

    \param val [in] Value type of the corresponding C++ type to be assigned to
                    this value.
  */
  template<typename ValueType>
  void operator << (const ValueType &val)
  {
    *this = val;
  }

  /** \details
    Output this value into the C++ variable. Unlike rxvalue_cast<> takes into
    account type propagations supported by type().toValueType or
    OdRxValueType::Desc<ValueType>::value().fromValueType.

    \param val [out] Value type of the corresponding C++ type.

    \returns
    True if the operation was successful; false otherwise.
  */
  template<typename ValueType>
  bool operator >> (ValueType &val) const
  {
    if (m_type == OdRxValueType::Desc<ValueType>::value())
    {
      const ValueType *pVal = rxvalue_cast<ValueType>(this);
      if (pVal)
      {
        val = *pVal;
        return true;
      }
    }
    else
    {
      OdRxValue subVal;
      if (m_type.toValueType(OdRxValueType::Desc<ValueType>::value(), *this, subVal) ||
        OdRxValueType::Desc<ValueType>::value().fromValueType(*this, subVal))
      {
        const ValueType *pVal = rxvalue_cast<ValueType>(&subVal);
        if (pVal)
        {
          val = *pVal;
          return true;
        }
      }
    }
    return false;
  }

  //DOM-IGNORE-BEGIN
#ifdef _MSC_VER
#pragma region private
#endif
private:
  bool isInlined() const
  {
    return type().size() <= sizeof(m_value);
  }
  const void* nonInlineValuePtr() const { return m_value.m_ptr; }
  void* nonInlineValuePtr() { return m_value.m_ptr; }
  const void* inlineValuePtr() const { return &m_value; }
  void* inlineValuePtr() { return &m_value; }
  const void* valuePtr() const
  {
    if (isInlined())
      return inlineValuePtr();
    else
      return nonInlineValuePtr();
  }
  template <bool Inlined>
  void* valuePtr__();
  template <bool Inlined>
  void initBlittable(const void* value, size_t size);

  template<typename T, bool inlined>
  class InitNonBlittable
  {
  public:
    static void init(OdRxValue& rxValue, const T& value);
  };
  template<typename T>
  class InitNonBlittable<T, true>
  {
  public:
    static void init(OdRxValue& rxValue, const T& value);
  };
  template<typename T>
  class InitNonBlittable<T, false>
  {
  public:
    static void init(OdRxValue& rxValue, const T& value);
  };

  template <typename T>
  void initNonBlittable(const T& value)
  {
    InitNonBlittable<T, sizeof(value) <= sizeof(m_value) >::init(*this, value);
  }

  void init(const OdRxValue& rhs, bool realloc)
  {
    bool blittable = type().isBlittable();
    bool inlined = isInlined();
    if (blittable && inlined)
    {
      memcpy(&m_value, &rhs.m_value, sizeof(m_value));
      return;
    }
    /*
    inlined,non-blittable
    non-inlined, non-blittable
    non-inlined, blittable
    */
    if (inlined)
      type().nonBlittable()->construct(inlineValuePtr(), rhs.inlineValuePtr());
    else
      setNonInlineValue(rhs.nonInlineValuePtr(), blittable, false, realloc);
  }
  void setNonInlineValue(const void* value, bool blittable, bool assignment, bool realloc)
  {
    ODA_ASSERT(blittable == type().isBlittable());
    ODA_ASSERT(!isInlined());
    unsigned int newSize = type().size();
    realloc = realloc || assignment;
    if (realloc)
    {
      size_t oldsize = *(((size_t*)m_value.m_ptr) - 1);
      if (oldsize != newSize)
      {
        m_value.m_ptr = reallocate(newSize, m_value.m_ptr);
        assignment = false;
      }
    }
    else
      m_value.m_ptr = allocate(newSize);

    if (blittable)
      memcpy(nonInlineValuePtr(), value, newSize);
    else if (assignment)
      type().nonBlittable()->assign(nonInlineValuePtr(), value);
    else
      type().nonBlittable()->construct(nonInlineValuePtr(), value);
  }
  OdRxValue(const OdRxValue& rhs, bool realloc)
    :m_type(rhs.m_type)
  {
    init(rhs, realloc);
  }
  const OdRxValueType& m_type;
#if (OD_SIZEOF_PTR == 4)
  OdInt32 padding;
#endif
  union InlineStorage
  {
    double m_point[3];
    void* m_ptr;
    char  m_int8;
    short m_int16;
    int   m_int32;
    OdInt64 m_int64;
    float m_real32;
    double m_real64;
  } m_value;
  void* allocate(size_t size) const;
  void* reallocate(size_t size, void* p) const;
  void  deallocate(void* p) const;
};
#ifdef _MSC_VER
#pragma endregion private
#endif
//DOM-IGNORE-END

/** <group OdRx_Classes>
  \details
  Create an empty value of the specific type.

  \returns
  OdRxValue object that has the specified value type and does not contain a
  value.
*/
template <class T> OdRxValue createOdRxValue() { T val; return OdRxValue(val); }

ODA_ASSUME(sizeof(OdRxValue) == 32); //each OdRxValue instance is 32 bytes long (on both 32 and 64 bit)

//DOM-IGNORE-BEGIN
/**   
  <group OdRx_Classes>
*/
template<>
inline const void* rxvalue_cast<void>(const OdRxValue * value) throw()
{
  return value ? value->valuePtr() : 0;
}

template<>
inline void* OdRxValue::valuePtr__<true>()
{
  ODA_ASSERT(isInlined());
  return inlineValuePtr();
}

/**   
  <group OdRx_Classes>
*/
template <>
inline void* OdRxValue::valuePtr__<false>()
{
  ODA_ASSERT(!isInlined());
  return nonInlineValuePtr();
}

/**   
  <group OdRx_Classes>
*/
template <>
inline void OdRxValue::initBlittable<true>(const void* value, size_t size)
{
  ODA_ASSERT(type().isBlittable());
  ODA_ASSERT(isInlined());
  memcpy(inlineValuePtr(), value, size);
}
template <>
inline void OdRxValue::initBlittable<false>(const void* value, size_t size)
{
  ODA_ASSERT(type().isBlittable());
  ODA_ASSERT(!isInlined());
  m_value.m_ptr = allocate(size);
  memcpy(nonInlineValuePtr(), value, size);
}

/**   
  <group OdRx_Classes>
*/
template<typename T>
inline void OdRxValue::InitNonBlittable< T, true>::init(OdRxValue& rxValue, const T& value)
{
  //call global placement new defined above so that we can call copy constructor
  ::new ((OdRxValueStorage*)(rxValue.inlineValuePtr())) T(value);
}

/**   
  <group OdRx_Classes>
*/
template<typename T>
inline void OdRxValue::InitNonBlittable< T, false>::init(OdRxValue& rxValue, const T& value)
{
  rxValue.setNonInlineValue(&value, false, false, false);
}
//DOM-IGNORE-END

class OdRxBoxedValue;

/** \details
  Smart pointer to objects of the OdRxBoxedValue class.
*/
typedef OdSmartPtr<OdRxBoxedValue> OdRxBoxedValuePtr;

/**  <group OdRx_Classes>
  \details
  Base class for wrapping OdRxValue in OdRxObject.
*/
class ODRX_ABSTRACT FIRSTDLL_EXPORT OdRxBoxedValue : public OdRxObject
{
public:
  //DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdRxBoxedValue);
  //DOM-IGNORE-END

  /**  \details
    Returns contained OdRxValue.

    \returns
    Contained OdRxValue.
  */
  virtual const OdRxValue* value() const = 0;
  
  /**  \details
    Returns contained OdRxValue.

    \returns
    Contained OdRxValue.
  */
  virtual OdRxValue* value() = 0;

  /**  \details
    Create a new boxed value on the heap. Returns a reference-counted smart
    pointer.

    \param value [in] Object of the OdRxValue to be wrapped.

    \returns
    Reference-counted smart pointer to the created OdRxBoxedValue object.
  */
  static OdRxBoxedValuePtr newBoxedValueOnHeap(const OdRxValue& value);

  /**  \details
    Clone this value.

    \returns
    Reference to the cloned object.
  */
  virtual OdRxObjectPtr clone() const ODRX_OVERRIDE;
 
  /**  \details
    Copy contents from another value.

    \param other [in] Object to copy value from.
  */
  virtual void copyFrom(const OdRxObject* other) ODRX_OVERRIDE;
  
  /**  \details
    Compare with another value.

    \param other [in] Object which to compare value with.

    \returns
    True if values of both objects are equal; false otherwise.
  */
  virtual bool isEqualTo(const OdRxObject * other) const ODRX_OVERRIDE;
  
  /**  \details
    Establish order relation with another value.

    \returns
    OdRx::Ordering::kLessThan if this object is less then the specified object;
    OdRx::Ordering::kEqual if this object is equal to the specified object;
    OdRx::Ordering::kGreaterThan if this object is grater then the specified object;
    OdRx::Ordering::kNotOrderable if these objects cannot be ordered.
  */
  virtual OdRx::Ordering comparedTo(const OdRxObject * other) const ODRX_OVERRIDE;
};

/** <group OdRx_Classes>
  \details
  Class wrapping OdRxValue in OdRxObject in-place, w/o additional allocations.
*/
class FIRSTDLL_EXPORT OdRxBoxedValueOnStack : public OdStaticRxObject<OdRxBoxedValue>
{
  OdRxValue& m_value;
public:
  //DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdRxBoxedValueOnStack);
  //DOM-IGNORE-END

  /** \details
    Default constructor.

    \param value [in] Value to be wrapped.
  */
  OdRxBoxedValueOnStack(OdRxValue& value);

  /** \details
    Returns contained OdRxValue (const version).

    \returns
    Contained value.
  */
  virtual const OdRxValue* value() const ODRX_OVERRIDE { return &m_value; }

  /**  \details
    Returns contained OdRxValue.

    \returns
    Contained value.
  */
  virtual OdRxValue* value() ODRX_OVERRIDE { return &m_value; }
};

// Type declarations follow:
//

#define ODRX_DECLARE_VALUE_TYPE(type, attribute)\
template<> struct OdRxValueType::Desc<type>\
{\
  attribute static const OdRxValueType& value() throw();\
  attribute static void del();\
};\
template<> OdRxValue::OdRxValue(const type&) throw();

/** \details
  OdRxValueType specialization for 'OdArray<OdRxValue>' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdArray<OdRxValue>, FIRSTDLL_EXPORT)

/** \details
  OdRxValueType specialization for 'char' value type.
*/
ODRX_DECLARE_VALUE_TYPE(char, FIRSTDLL_EXPORT)

/** \details
  OdRxValueType specialization for 'signed char' value type.
*/
ODRX_DECLARE_VALUE_TYPE(signed char, FIRSTDLL_EXPORT)

/** \details
  OdRxValueType specialization for 'unsigned char' value type.
*/
ODRX_DECLARE_VALUE_TYPE(unsigned char, FIRSTDLL_EXPORT)

/** \details
  OdRxValueType specialization for 'bool' value type.
*/
ODRX_DECLARE_VALUE_TYPE(bool, FIRSTDLL_EXPORT)

/** \details
  OdRxValueType specialization for 'double' value type.
*/
ODRX_DECLARE_VALUE_TYPE(double, FIRSTDLL_EXPORT)

/** \details
  OdRxValueType specialization for 'short' value type.
*/
ODRX_DECLARE_VALUE_TYPE(short, FIRSTDLL_EXPORT)

/** \details
  OdRxValueType specialization for 'unsigned short' value type.
*/
ODRX_DECLARE_VALUE_TYPE(unsigned short, FIRSTDLL_EXPORT)

/** \details
  OdRxValueType specialization for 'int' value type.
*/
ODRX_DECLARE_VALUE_TYPE(int, FIRSTDLL_EXPORT)

/** \details
  OdRxValueType specialization for 'unsigned int' value type.
*/
ODRX_DECLARE_VALUE_TYPE(unsigned int, FIRSTDLL_EXPORT)

/** \details
  OdRxValueType specialization for 'long' value type.
*/
ODRX_DECLARE_VALUE_TYPE(long, FIRSTDLL_EXPORT)

/** \details
  OdRxValueType specialization for 'unsigned long' value type.
*/
ODRX_DECLARE_VALUE_TYPE(unsigned long, FIRSTDLL_EXPORT)

/** \details
  OdRxValueType specialization for 'long long' value type.
*/
ODRX_DECLARE_VALUE_TYPE(long long, FIRSTDLL_EXPORT)

/** \details
  OdRxValueType specialization for 'unsigned long long' value type.
*/
ODRX_DECLARE_VALUE_TYPE(unsigned long long, FIRSTDLL_EXPORT)

/** \details
  OdRxValueType specialization for 'float' value type.
*/
ODRX_DECLARE_VALUE_TYPE(float, FIRSTDLL_EXPORT)

/** \details
  OdRxValueType specialization for 'const OdChar*' value type.
*/
template<> struct OdRxValueType::Desc<const OdChar*>
{
  FIRSTDLL_EXPORT static const OdRxValueType& value() throw();
  FIRSTDLL_EXPORT static void del();
};
template<> OdRxValue::OdRxValue(const OdChar* const &) throw();

/** \details
  OdRxValueType specialization for 'OdArray<bool>' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdArray<bool>, FIRSTDLL_EXPORT)

/** \details
  OdRxValueType specialization for 'OdArray<double>' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdArray<double>, FIRSTDLL_EXPORT)

/** \details
  OdRxValueType specialization for 'OdArray<float>' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdArray<float>, FIRSTDLL_EXPORT)

/** \details
  OdRxValueType specialization for 'OdArray<signed char>' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdArray<signed char>, FIRSTDLL_EXPORT)

/** \details
  OdRxValueType specialization for 'OdArray<char>' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdArray<char>, FIRSTDLL_EXPORT)

/** \details
  OdRxValueType specialization for 'OdArray<short>' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdArray<short>, FIRSTDLL_EXPORT)

/** \details
  OdRxValueType specialization for 'OdArray<int>' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdArray<int>, FIRSTDLL_EXPORT)

/** \details
  OdRxValueType specialization for 'OdArray<long>' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdArray<long>, FIRSTDLL_EXPORT)

/** \details
  OdRxValueType specialization for 'OdArray<unsigned char>' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdArray<unsigned char>, FIRSTDLL_EXPORT)

/** \details
  OdRxValueType specialization for 'OdArray<unsigned short>' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdArray<unsigned short>, FIRSTDLL_EXPORT)

/** \details
  OdRxValueType specialization for 'OdArray<unsigned int>' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdArray<unsigned int>, FIRSTDLL_EXPORT)

/** \details
  OdRxValueType specialization for 'OdArray<unsigned long>' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdArray<unsigned long>, FIRSTDLL_EXPORT)

/** \details
  OdRxValueType specialization for 'OdArray<long long>' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdArray<long long>, FIRSTDLL_EXPORT)

/** \details
  OdRxValueType specialization for 'OdArray<unsigned long long>' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdArray<unsigned long long>, FIRSTDLL_EXPORT)

/** \details
  OdRxValueType specialization for 'OdArray<OdArray<double> >' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdArray<OdArray<double> >, FIRSTDLL_EXPORT)

/** \details
  OdRxValueType specialization for 'OdArray<OdArray<int> >' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdArray<OdArray<int> >, FIRSTDLL_EXPORT)

class OdRxClass;

/** \details
  OdRxValueType specialization for 'OdRxClass*' value type.
*/
template<> struct OdRxValueType::Desc<OdRxClass*>
{
  FIRSTDLL_EXPORT static const OdRxValueType& value() throw(); 
  FIRSTDLL_EXPORT static void del(); 
};

class OdAnsiString;

/** \details
  OdRxValueType specialization for 'OdAnsiString' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdAnsiString, FIRSTDLL_EXPORT)

class OdString;

/** \details
  OdRxValueType specialization for 'OdString' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdString, FIRSTDLL_EXPORT)

/** \details
  OdRxValueType specialization for 'OdCmTransparency' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdCmTransparency, FIRSTDLL_EXPORT);

/** \details
  OdRxValueType specialization for 'OdRxObjectPtr' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdRxObjectPtr, FIRSTDLL_EXPORT)

/** \details
  OdRxValueType specialization for 'OdDbHandle' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdDbHandle, FIRSTDLL_EXPORT)

/** \details
  OdRxValueType specialization for 'OdArray<OdAnsiString>' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdArray<OdAnsiString>, FIRSTDLL_EXPORT)

/** \details
  OdRxValueType specialization for 'OdArray<OdString>' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdArray<OdString>, FIRSTDLL_EXPORT)

/** \details
  OdRxValueType specialization for 'const char*' value type.
*/
template<> struct OdRxValueType::Desc<const char*>
{
  FIRSTDLL_EXPORT static const OdRxValueType& value() throw();
  FIRSTDLL_EXPORT static void del();
};
template<> OdRxValue::OdRxValue(const char* const &) throw();

#include "TD_PackPop.h"
#endif // _ODRXVALUE_INCLUDED_
