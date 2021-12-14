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

#ifndef _ODRXVALUETYPEUTIL_INCLUDED_
#define _ODRXVALUETYPEUTIL_INCLUDED_

#include "RxMember.h"
#include "RxValue.h"
#include "OdArray.h"

/** \details
  Represents 'plain old data' type, which can be copied with memcpy, w/o calling
  a constructor. May be inline or non-inline, depending on size (inline if fits
  in 24 bytes).

  <group OdRx_Classes>
 */
template<typename ValueType>
class OdRxValueTypePOD : public OdRxValueType
{
public:

  /** \details
    Constructor for objects of the OdRxValueTypePOD class. Creates an instance
    that represents a POD type.

    \param name            [in] Type name.
    \param memberConstruct [in] Class member collection constructor.
    \param userData        [in] Optional user data.
  */
  OdRxValueTypePOD(const OdChar* name, OdRxMemberCollectionConstructorPtr memberConstruct, void* userData = NULL) :OdRxValueType(name, sizeof(ValueType), memberConstruct, userData) {}

  /** \details
    Convert value to string.

    \param instance [in] Instance of the value to convert.
    \param format   [in] Defines the format of the resulting string.

    \returns
    Converted value.

    \remarks
    Mostly for debug purposes.
  */
  virtual OdString subToString(const void* instance, OdRxValueType::StringFormat format) const ODRX_OVERRIDE;

  /** \details
    Compares two values of the underlying C++ type.

    \param a [in] First value to compare.
    \param b [in] Second value to compare.

    \returns
    True if compared values are equal; false otherwise.
  */
  virtual bool subEqualTo(const void* a, const void* b) const ODRX_OVERRIDE;

  /** \details
    Create a new wrapped value of this type.

    \returns
    Created wrapped value of this type.
  */
  virtual OdRxValue createValue() const ODRX_OVERRIDE { return createOdRxValue<ValueType>(); }
};

//DOM-IGNORE-BEGIN
template<typename ValueType>
OdString OdRxValueTypePOD<ValueType>::subToString(const void* instance, OdRxValueType::StringFormat format) const
{
  return OdString::kEmpty;
}
template<typename ValueType>
bool OdRxValueTypePOD<ValueType>::subEqualTo(const void* a, const void* b) const
{
  return false;
}
//DOM-IGNORE-END

/** \details
  This class dynamically specifies an underlying OdRxValueType by the type path.
  The instances of this class aren't registered in the class dictionary.
  Overloaded methods which return values of subValueType are needed for correct
  OdRxValue copying in appropriate OdRxValue(type, value) constructor.

  <group OdRx_Classes>
*/
class OdRxSpecifiedValueType : public OdRxValueType
{
public:

  /** \details
    Constructor which takes an original ValueType and a string that contains
    additional specification of the type in "Folder/SubFolder/Type" format.

    \param underlyingValueType [in] A pointer to the value type for specification.
    \param typePath            [in] A string for the value type specification.
  */
  OdRxSpecifiedValueType(const OdRxValueType &underlyingValueType, const char *typePath)
    : OdRxValueType(NULL, underlyingValueType.size(), 0)
    , m_underlyingValueType(underlyingValueType)
    , m_typePath(typePath)
  {}

  /** \details
    Retrieves a pointer to the underlying value type (which coincide with the
    actual ValueType of OdRxValue.m_value).

    \returns
    Pointer to the underlying value type.
  */
  const OdRxValueType& underlyingValueType() const { return m_underlyingValueType; }

  //DOM-IGNORE-BEGIN
  virtual const IOdRxNonBlittableType* nonBlittable() const { return m_underlyingValueType.nonBlittable(); }

  /** \details
    Implementation of the base class 'enumeration'.

    \returns
    Interface to the enumeration.
  */
  virtual const IOdRxEnumeration* enumeration() const { return m_underlyingValueType.enumeration(); }

  /** \details
    Returns the interface, that describes references (handlers).

    \returns
    Interface, describing references. NULL for non-reference types.
  */
  virtual const IOdRxReferenceType* reference() const { return m_underlyingValueType.reference(); }

  /** \details
    Returns the interface, that describes OdRxObject-descendant pointers.

    \returns
    Interface, describing OdRxObject-descendant pointers. NULL for types other
    than OdRxObject-descendant pointers.
  */
  virtual const IOdRxObjectValue* rxObjectValue() const { return m_underlyingValueType.rxObjectValue(); }

  /** \details
    Convert value to string.

    \param instance [in] Instance of the value to convert.
    \param format   [in] Defines the format of the resulting string.

    \returns
    Converted value.

    \remarks
    Mostly for debug purposes.
  */
  virtual OdString subToString(const void* instance, OdRxValueType::StringFormat format) const { return m_underlyingValueType.toString(instance, format); };

  /** \details
    Compares two values of the underlying C++ type.

    \param a [in] First value to compare.
    \param b [in] Second value to compare.

    \returns
    True if compared values are equal; false otherwise.
  */
  virtual bool subEqualTo(const void* a, const void* b) const { return m_underlyingValueType.equalTo(a, b); };

  /** \details
    Create a new wrapped value of this type.

    \returns
    Created wrapped value of this type.
  */
  virtual OdRxValue createValue() const ODRX_OVERRIDE { return m_underlyingValueType.createValue(); }

  /** \details
    Retreives an ANSI string in format of path that elaborates the sense of
    value of the value type.

    \returns
    ANSI string with clarification of value sense.
  */
  const OdAnsiString typePath() const
  {
    return m_typePath;
  }

  /** \details
    Comparison operator.

    \param rhs [in] Value type to compare with the current value type.

    \returns
    True if value types are equal; false otherwise.
  */
  bool operator==(const OdRxSpecifiedValueType& rhs) const
  {
    return (underlyingValueType() == rhs.underlyingValueType()) &&
      (m_typePath == rhs.typePath());
  }

  /** \details
    Negative comparison operator.

    \param rhs [in] Value type to compare with the current value type.

    \returns
    True if value types are not equal; false otherwise.
  */
  bool operator!=(const OdRxSpecifiedValueType& rhs) const
  {
    return !(operator == (rhs));
  }

  /** \details
  Enable operator new.
  */
  void* operator new(size_t s)
  {
    void* p = ::odrxAlloc(s);
    if (!p) NEW_HANDLER()
      return p;
  };
  /** \details
    Enable use delete operator.
  */
  void operator delete(void* p) { ::odrxFree(p); };
private:
  virtual const OdAnsiString subTypePath(const OdRxValue& instance) const ODRX_OVERRIDE { return m_typePath; };
protected:
  const OdRxValueType &m_underlyingValueType;
  const OdAnsiString m_typePath;
  //DOM-IGNORE-END
};

/** <group OdRx_Classes>
  \details
  Represents 'plain old data' types, that have underlying C++ types.
*/
template<typename ValueType>
class OdRxValueWithUnderlyingTypePOD : public OdRxValueTypePOD<ValueType>
{
public:

  /** \details
    Constructor for objects of the OdRxValueWithUnderlyingTypePOD class.
    Creates an instance that represents a POD type.

    \param name            [in] Type name.
    \param memberConstruct [in] Class member collection constructor.
    \param userData        [in] Optional user data.
  */
  OdRxValueWithUnderlyingTypePOD(const OdChar* name, OdRxMemberCollectionConstructorPtr memberConstruct, void* userData = NULL)
    : OdRxValueTypePOD<ValueType>(name, memberConstruct, userData) {};

  /** \details
    Convert value to the specified type from this type.

    \param vt       [in] Type to convert to.
    \param instance [in] Value of "this" type.
    \param subVal  [out] Resulting value of the type specified in vt.

    \returns
    True if conversion is possible. False otherwise.
  */
  virtual bool subToValueType(const OdRxValueType &vt, const OdRxValue &instance, OdRxValue &subVal) const ODRX_OVERRIDE;

  /** \details
    Convert value from the specified type (type of the "from" value) to this
    type.

    \param from      [in] Value of type to convert from.
    \param instance [out] Resulting value of "this" type.

    \returns
    True if conversion is possible. False otherwise.
  */
  virtual bool subFromValueType(const OdRxValue &from, OdRxValue &instance) const ODRX_OVERRIDE;
};

/** <group OdRx_Classes>
  \details
  Represents a 'plain old data' reference type.
*/
template<typename ValueType>
class OdRxValueWithReferenceTypePOD : public OdRxValueTypePOD<ValueType>
{
public:

  /** \details
    Constructor for objects of the OdRxValueWithReferenceTypePOD class.
    Creates an instance that represents a POD type.

    \param name            [in] Type name.
    \param memberConstruct [in] Class member collection constructor.
    \param userData        [in] Optional user data.
  */
  OdRxValueWithReferenceTypePOD(const OdChar* name, OdRxMemberCollectionConstructorPtr memberConstruct, void* userData = NULL)
    : OdRxValueTypePOD<ValueType>(name, memberConstruct, userData) {};

  /** \details
    Returns the interface, that describes references (handlers).

    \returns
    Interface, describing references. NULL for non-reference types.
  */
  virtual const IOdRxReferenceType* reference() const ODRX_OVERRIDE;

  /** \details
    Convert value to string.

    \param instance [in] Instance of the value to convert.
    \param format   [in] Defines the format of the resulting string.

    \returns
    Converted value.

    \remarks
    Mostly for debug purposes.
  */
  virtual OdString subToString(const void* instance, OdRxValueType::StringFormat format) const ODRX_OVERRIDE;

  /** \details
    Compares two values of the underlying C++ type.

    \param a [in] First value to compare.
    \param b [in] Second value to compare.

    \returns
    True if compared values are equal; false otherwise.
  */
  virtual bool subEqualTo(const void* a, const void* b) const ODRX_OVERRIDE;
};

#pragma warning(push)
#pragma warning(disable: 4355) 

/** \details
  Describes enumeration type for C++ enum to be stored in OdRxValue.
  Instances of this type should create tags in constructor (or pseudo-constructor) for every enum element.
  
  <group OdRx_Classes>
*/
template<typename ValueType>
class OdRxEnumType : public OdRxValueTypePOD<ValueType>, public IOdRxEnumeration
{
  OdArray<OdRxEnumTagPtr> m_tags; //Array of pointers to enumeration element descriptors.
public:
  
  /** \details
    This is the default constructor for OdRxEnumType class objects.

    \param name [in] Enumeration name.
    \param memberConstruct [in] Pointer to the enumeration member constructor.
    \param userData [in] Optional user data.
  */
  OdRxEnumType(const OdChar* name, OdRxMemberCollectionConstructorPtr memberConstruct, void* userData = NULL)
    :OdRxValueTypePOD<ValueType>(name, memberConstruct, userData) {}

  /** \details
    Calculates the number of elements in the enumeration.

    \returns
    Number of elements in the enumeration.
  */
  virtual int count() const ODRX_OVERRIDE
  {
    return m_tags.length();
  }

  /** \details
    Returns the enumeration element descriptor at the given index.

    \param i [in] Index of the enumeration element.

    \returns
    Descriptor of the enumeration element.
  */
  virtual const OdRxEnumTag& getAt(int i) const ODRX_OVERRIDE
  {
    return *m_tags[i];
  }

  /** \details
    Append a new enumeration element descriptor.

    \param tag [in] Enumeration element descriptor to be added to the enumeration.
  */
  void append(OdRxEnumTag* tag)
  {
    m_tags.append(tag);
  }

  /** \details
    Implementation of the base class 'enumeration'.

    \returns
    Interface to the enumeration.
  */
  virtual const IOdRxEnumeration* enumeration() const ODRX_OVERRIDE { return this; }

  /** \details
    This method is not defined in the base template, instance must declare a specialization.
    
    \param instance [in] Instance of the enumeration element.
    \param format   [in] Return value format.
    
    \returns
    Resulting value.
  */
  virtual OdString subToString(const void* instance, OdRxValueType::StringFormat format = OdRxValueType::kStringFormatGlobal) const ODRX_OVERRIDE;

  /** \details
    This method is not defined in the base template, instance must declare a specialization.
    
    \param a [in] First element to compare.
    \param b [in] Second element to compare.
    
    \returns
    Comparation result.
  */
  virtual bool subEqualTo(const void* a, const void* b) const ODRX_OVERRIDE;
};

/** \details
  Describes enumeration type for C++ enumeration with underying type to be
  stored in OdRxValue.
  Instances of this type should create tags in the constructor
  (or pseudo-constructor) for every enum element.

  <group OdRx_Classes>
*/
template<typename ValueType>
class OdRxEnumWithUnderlyingType : public OdRxEnumType<ValueType>
{
public:

  /** \details
    This is the default constructor for OdRxEnumWithUnderlyingType class objects.

    \param name [in] Enumeration name.
    \param memberConstruct [in] Pointer to the enumeration member constructor.
    \param userData [in] Optional user data.
  */
  OdRxEnumWithUnderlyingType(const OdChar* name, OdRxMemberCollectionConstructorPtr memberConstruct, void* userData = NULL)
    : OdRxEnumType<ValueType>(name, memberConstruct, userData) {};

  /** \details
    This method is not defined in the base template, instance must declare a specialization.

    \param vt [in] Value type.
    \param instance [in] Instance of the enumeration element.
    \param subVal [out] Resulting value.
    
    \returns
    The result of conversion.
  */
  virtual bool subToValueType(const OdRxValueType &vt, const OdRxValue &instance, OdRxValue &subVal) const ODRX_OVERRIDE;
};

#pragma warning(pop)

/** \details
 Helper macro to be used in enumeration types pseudo-constructors to add tags.
*/
#define ODRX_APPEND_VALUE_TAG(global, value) global->append(OdRxEnumTag::createObject( __OD_T(#value),  OdRxValue(*global, OdRxValue((int)value)), global).get())

/** \details
  Helper base type template describing C++ types with non-trivial
  constructor/destructor to be stored in OdRxValue, like string, arrays, etc.
  Can be inline or non-inline, depending on size.

  <group OdRx_Classes>
*/
template<typename T>
class OdRxNonBlittableType : public OdRxValueType
{
  //DOM-IGNORE-BEGIN
  class NonBlittable : public IOdRxNonBlittableType
  {
    virtual void construct(void* dest, const void* source) const ODRX_OVERRIDE
    {
      ::new ((OdRxValueStorage*)dest) T(*(T*)source);
    }
    virtual void assign(void* dest, const void* source) const ODRX_OVERRIDE
    {
      ((T*)dest)->operator =(*(T*)source);
    }
    virtual void destruct(const void* instance) const ODRX_OVERRIDE
    {
      ((T*)instance)->~T();
    }
  } m_nonBlittable;
  virtual const IOdRxNonBlittableType* nonBlittable() const ODRX_OVERRIDE { return &m_nonBlittable; }

  /** \details
    Convert value to string.

    \param instance [in] Instance of the enumeration element.
    \param format   [in] Return value format.

    \returns
    Resulting value.

    \remarks
    This method is not defined in the base template, instance must declare a specialization.
  */
  virtual OdString subToString(const void* instance, StringFormat format = kStringFormatGlobal) const ODRX_OVERRIDE;

  /** \details
    Compares two values of the underlying C++ type.

    \param a [in] First element to compare.
    \param b [in] Second element to compare.

    \returns
    Comparation result.

    \remarks
    This method is not defined in the base template, instance must declare a specialization.
  */
  virtual bool subEqualTo(const void* a, const void* b) const ODRX_OVERRIDE;

  /** \details
    Create a new wrapped value of this type.

    \returns
    Created wrapped value of this type.

    \remarks
    This method is not defined in the base template, instance must declare a specialization.
  */
  virtual OdRxValue createValue() const ODRX_OVERRIDE { return createOdRxValue<T>(); }
public:
  //DOM-IGNORE-END
  /** \details
    Constructor for objects of the OdRxNonBlittableType class.

    \param name            [in] Type name.
    \param memberConstruct [in] Class member collection constructor.
    \param userData        [in] Optional user data.
  */
  OdRxNonBlittableType(const OdChar* name, OdRxMemberCollectionConstructorPtr memberConstruct, void* userData = NULL) :OdRxValueType(name, sizeof(T), memberConstruct, userData) {}
};

/** <group OdRx_Classes>
  \details
  Represents non-blittable data types, that have underlying C++ types.
*/
template<typename ValueType>
class OdRxNonBlittableWithUnderlyingType : public OdRxNonBlittableType<ValueType>
{
public:

  /** \details
    Constructor for objects of the OdRxNonBlittableWithUnderlyingType class.

    \param name            [in] Type name.
    \param memberConstruct [in] Class member collection constructor.
    \param userData        [in] Optional user data.
  */
  OdRxNonBlittableWithUnderlyingType(const OdChar* name, OdRxMemberCollectionConstructorPtr memberConstruct, void* userData = NULL)
    : OdRxNonBlittableType<ValueType>(name, memberConstruct, userData) {};

  /** \details
    Convert value to the specified type from this type.

    \param vt       [in] Type to convert to.
    \param instance [in] Value of "this" type.
    \param subVal  [out] Resulting value of the type specified in vt.

    \returns
    True if conversion is possible. False otherwise.
  */
  virtual bool subToValueType(const OdRxValueType &vt, const OdRxValue &instance, OdRxValue &in) const ODRX_OVERRIDE;

  /** \details
    Convert value from the specified type (type of the "from" value) to this
    type.

    \param from      [in] Value of type to convert from.
    \param instance [out] Resulting value of "this" type.

    \returns
    True if conversion is possible. False otherwise.
  */
  virtual bool subFromValueType(const OdRxValue &from, OdRxValue &instance) const ODRX_OVERRIDE;
};


#define DEFINE_VALUE_TYPE_VAR_NAME(type) m_g ## type ## Type

/** \details
 Helper macro for defining OdRxObject-derived smart pointer types (OdSmartPtr<?>)
 e.g. ODRX_DEFINE_RXPTR_VALUE_TYPE(OdRxObjectPtr, DEFINE_VALUE_TYPE_VAR_NAME(OdRxObjectPtr), FIRSTDLL_EXPORT);
 
 <group OdRx_Classes>
*/
#define ODRX_DEFINE_RXPTR_VALUE_TYPE(Name, g_var, attrib)\
static OdRxValueType* g_var = 0;\
\
class Od ## Name ## Type : public OdRxNonBlittableType<Name>\
{\
public:\
  struct IOdRxObjectValueImpl : IOdRxObjectValue\
  {\
    virtual const OdRxObject* getRxObject(const OdRxValue& value) const\
    {\
      return rxvalue_cast<Name>(&value)->get();\
    }\
  }\
  m_rxObjectImpl;\
  virtual const IOdRxObjectValue* rxObjectValue() const { return &m_rxObjectImpl; }\
  Od ## Name ## Type() : OdRxNonBlittableType<Name>(__OD_T(#Name) , 0) {}\
};\
template<> attrib OdRxValue::OdRxValue(const Name& val) throw() : m_type(OdRxValueType::Desc<Name>::value()) { type().nonBlittable()->construct(inlineValuePtr(), &val); } \
template<> attrib OdString OdRxNonBlittableType<Name>::subToString(const void* instance, OdRxValueType::StringFormat format) const \
{\
  return OdString().format(L"%p", ((Name*)instance)->get());\
}\
template<> attrib bool OdRxNonBlittableType<Name>::subEqualTo(const void* a, const void* b) const \
{\
  return ((Name*)a)->get() == ((Name*)b)->get();\
}\
attrib const OdRxValueType& OdRxValueType::Desc<Name>::value() throw()\
{\
  if (g_var == 0)\
  {\
    static OdMutex m;\
    TD_AUTOLOCK(m);\
    if (g_var == 0)\
      g_var = new Od ## Name ## Type();\
  }\
  return *g_var;\
}\
attrib void OdRxValueType::Desc<Name>::del()\
{\
  if (g_var)\
  {\
    ::odrxClassDictionary()->remove(__OD_T(#Name));\
    delete g_var;\
    g_var = 0;\
  }\
}


#endif
