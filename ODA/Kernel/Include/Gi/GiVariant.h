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

// OdGiVariant type for generic procedural material.

#ifndef __ODGIVARIANT_H__
#define __ODGIVARIANT_H__

#include "RootExport.h"
#include "GiExport.h"

#include "CmColorBase.h"
#include "RxVariant.h"
#include "RxObject.h"

#include "TD_PackPush.h"

/** \details
    This class is used as the basis for the generic data storage used for procedural material definitions.

    \sa
    TD_Gi

    <group OdGi_Classes>
*/
class FIRSTDLL_EXPORT OdGiVariant : public OdRxObject
{
  static void copyDataFrom(OdGiVariant &vTo, const OdGiVariant &vFrom);
  public:
    /** \details
    Defines the current type set to an OdGiVariant object.
    */
    enum VariantType
    {
      kUndefined = 0, // Variant type is undefined (the default type). 
      kBoolean,       // An OdGiVariant is of a boolean type (bool).
      kInt,           // An OdGiVariant is of a long type (OdInt32).
      kDouble,        // An OdGiVariant is of a double type (double).
      kColor,         // An OdGiVariant is of a color type (OdCmEntityColor).
      kString,        // An OdGiVariant is of a string type (OdString).
      kTable          // An OdGiVariant is a table of key/value pairs.
    };
  public:
    /** \details
      Represents a generic variant type for the OdGiVariant object.
    */
    class EnumType
    {
      public:
        /** \details
          Constructor for the EnumType class. Initializes object with a given value.
          \param value [in]  Input integer value.
        */
        explicit EnumType(int value) : m_value(value) { }
        template <typename T> operator T() { return static_cast<T>(m_value); }
        /** \details
          Equality operator for the EnumType class.
          \param right [in]  Value to compare.
          \returns
          true if the casted (to integer) right value equals to the value of this object, false otherwise.
        */
        template <typename T> bool operator ==(T right) const { return static_cast<int>(right) == m_value; }
        /** \details
          Inequality operator for the EnumType class.
          \param right [in]  Value to compare.
          \returns
          true if the casted (to integer) right value does not equal to the value of this object, false otherwise.
        */
        template <typename T> bool operator !=(T right) const { return !(*this == right); }
      protected:
        int m_value;
    };
  public:
    ODRX_DECLARE_MEMBERS(OdGiVariant);

    // Constructors
    //OdGiVariant();
    //OdGiVariant(const OdGiVariant &value);
    //OdGiVariant(bool value);
    //OdGiVariant(OdInt32 value);
    //OdGiVariant(double value);
    //OdGiVariant(const OdCmEntityColor &value);
    //OdGiVariant(const OdString &value);
    //OdGiVariant(const OdChar *value);
  protected:
    OdGiVariant();
  public:
    /** \details
    This constructor creates an empty OdGiVariant instance, copies an input OdGiVariant into it and returns a smart pointer to the created object.

    \param value [in]  Reference to an existing OdGiVariant to copy.
    */
      static OdSmartPtr<OdGiVariant> createObject(const OdGiVariant &value);
    
    /** \details
    Creates a new OdGiVariant object of the kBoolean type, initializes it with an input boolean value and returns a smart pointer to the created object.

    \param value [in]  Input initial boolean value.
    */
     static OdSmartPtr<OdGiVariant> createObject(bool value);

    /** \details
    Creates a new OdGiVariant object of the kInt type, initializes it with an input long value and returns a smart pointer to the created object.

    \param value [in]  Input initial long value.
    */
     static OdSmartPtr<OdGiVariant> createObject(OdInt32 value);

    /** \details
    Creates a new OdGiVariant object of the kDouble type, initializes it with an input floating point value and returns a smart pointer to the created object.

    \param value [in]  Input initial floating point value.
    */
    static OdSmartPtr<OdGiVariant> createObject(double value);

    /** \details
    Creates a new OdGiVariant object of the kColor type, initializes it with an input color value and returns a smart pointer to the created object.

    \param value [in]  Input initial color value.
    */
    static OdSmartPtr<OdGiVariant> createObject(const OdCmEntityColor &value);

    /** \details
    Creates a new OdGiVariant object of the kString type, initializes it with an input string and returns a smart pointer to the created object.

    \param value [in]  Input initial string value.
    */
    static OdSmartPtr<OdGiVariant> createObject(const OdString &value);

    /** \details
    Creates a new OdGiVariant object of the kString type, initializes it with a string of an input char array and returns a smart pointer to the created object.

    \param value [in]  Input initial char array.
    */
    static OdSmartPtr<OdGiVariant> createObject(const OdChar *value);

    /** \details
    Destructor for the OdGiVariant class.
    */
    ~OdGiVariant();

    /** \details
      Equality operator for the OdGiVariant class.
      \param value [in]  Variant to compare.
      \returns
      true if all of the following conditions are met:
      * Neither of the objects are NULL;
      * Both objects have the same type;
      * Values of the objects extracted via asBoolean/asInt/etc.(other corresponding methods for various types) are equal.
      \remarks
      For table elements, verifies equivalence of each element by its key.
    */
    bool operator ==(const OdGiVariant& value) const;

    /** \details
      Assignment operator for the OdGiVariant class. Returns a reference to this object with a new assigned value.
      \param value [in]  Variant to assign.
    */
    OdGiVariant &operator =(const OdGiVariant& value);

    /** \details
    Copies the contents of the specified object into this object.
    
    \param pSource [in]  Pointer to the source object.
    
    Throws:
    <table>
    Exception             Cause
    eNotApplicable        The incoming object isn't of type OdGiVariant.
    eNullObjectPointer    The incoming object is null.
    </table>
    */
    virtual void copyFrom(const OdRxObject* pSource);
    
    /** \details
    Returns the current type of the OdGiVariant object. All available types are defined in the VariantType enumeration.
    */
    VariantType type() const;

    /** \details
    Changes the OdGiVariant's type to kBoolean and assigns a new value.

    \param value [in]  Input new boolean value to set.
    */
    void set(bool value);

    /** \details
    Changes the OdGiVariant's type to kInt and assigns a new value.

    \param value [in]  Input new long value to set.
    */
    void set(OdInt32 value);

    /** \details
    Changes the OdGiVariant's type to kDouble and assigns a new value.

    \param value [in]  Input new floating point value to set.
    */
    void set(double value);

    /** \details
    Changes the OdGiVariant's type to kColor and assigns a new value.

    \param value [in]  Input new color value to set.
    */
    void set(const OdCmEntityColor &value);

    /** \details
    Changes the OdGiVariant's type to kString and assigns a new value.

    \param value [in]  Input new string value to set.
    */
    void set(const OdString &value);
    
    /** \details
    Changes the OdGiVariant's type to kString and assigns a new value.

    \param value [in]  Input new char array to set as a string value.
    */
    void set(const OdChar *value);

    /** \details
    Returns the current value if the variant type is kBoolean. For other types the method throws an exception.
    */
    bool asBoolean() const;

    /** \details
    Returns the current value if the variant type is kInt. For other types the method throws an exception.
    */
    OdInt32 asInt() const;

    /** \details
    Returns the current value if the variant type is kDouble. For other types the method throws an exception.
    */
    double asDouble() const;

    /** \details
    Returns the current value if the variant type is kColor. For other types the method throws an exception.
    */
    const OdCmEntityColor &asColor() const;

    /** \details
    Returns the current value if the variant type is kString. For other types the method throws an exception.
    */
    const OdString &asString() const;

    /** \details
    Returns the current value as a float. The method calls the asDouble() and casts the result to a float.
    */
    float asFloat() const;

    /** \details
    Returns the current value as a char. The method calls the asInt() and casts the result to a char.
    */
    OdInt8 asChar() const;

    /** \details
    Returns the current value as an unsigned char. The method calls the asInt() and casts the result to an unsigned char.
    */
    OdUInt8 asUchar() const;

    /** \details
    Returns the current value as a short. The method calls the asInt() and casts the result to a short.
    */
    OdInt16 asShort() const;
    
    /** \details
    Returns the current value as an unsigned short. The method calls the asInt() and casts the result to an unsigned short.
    */
    OdUInt16 asUshort() const;
    
    /** \details
    Returns the current value as an unsigned long. The method calls the asInt() and casts the result to an unsigned long.
    */
    OdUInt32 asUint() const;
    
    /** \details
    Returns the current value as a long. The method is equivalent to asInt().
    */
    OdInt32 asLong() const;

    /** \details
    Returns the current value as an unsigned long. The method is equivalent to asUint().
    */
    OdUInt32 asUlong() const;

    /** \details
    Returns the current value as an EnumType. The method calls the asInt() and creates an OdGiVariant::EnumType from the result.
    */
    EnumType asEnum() const;

    /** \details
    If the variant's type is kTable, gets the value paired with the specified string key. For other types the method is invalid.
    
    \param value [in]  Input reference to an OdGiVariant object to be filled with the retrieved value.
    \param elem [in]  Input string key of an element in the table. 

    \returns
    true if value is successfully retrieved, false - otherwise.
    */
    bool getElem(const OdString &elem, OdGiVariant &value) const;
    
    /** \details
    If the variant's type is kTable, gets the value paired with the specified string key. For other types the method is invalid.
    
    \param elem [in]  Input string key of an element in the table.

    \returns
    A pointer to the obtained OdGiVariant object. If the value isn't retrieved, returns NULL.
    */
    const OdGiVariant *getElem(const OdString &elem) const;
    
    /** \details
    Adds the specified value to the table with the specified string key. If the object isn't of the kTable type, the method converts the type to kTable.
    
    \param value [in]  Value to be added with the elem key.
    \param elem [in]  Input string key with which to add the value. 
    */
    void setElem(const OdString &elem, const OdGiVariant &value);

    /** \details
    If the variant's type is kTable, deletes the element with the specified string key from the table. For other types the method is invalid.
    
    \param elem [in]  Input string key with which to add the value. 
    */
    void deleteElem(const OdString &elem);

    /** \details
    If the variant's type is kTable, returns the number of table's elements. For other types the method is invalid.
    */
    OdInt32 getElemCount() const;

    /** \details
    If the variant's type is kTable, gets the key and OdGiVariant value from the table by the specified index. For other types the method is invalid.
    
    \param nElem [in]  Input index. Should be within the limits of the table size.
    \param value [out]  Output OdGiVariant value at the nElem index.
    \param elem [out]  Output string key at the nElem index.

    \returns
    true - if the key and value are retrieved successfully. 
    false - otherwise. 
    */
    bool getElemAt(OdInt32 nElem, OdString &elem, OdGiVariant &value) const;

    /** \details
    If the variant's type is kTable, gets the key and a pointer to the OdGiVariant value from the table by the specified index. For other types the method is invalid.
    
    \param nElem [in]  Input index. Should be within the limits of the table size.
    \param elem [out]  Output string key at the nElem index.

    \returns
    A pointer to the obtained OdGiVariant value. If the value isn't retrieved, returns NULL. 
    */
    const OdGiVariant *getElemAt(OdInt32 nElem, OdString &elem) const;

    /** \details
    Determines whether two OdGiVariant objects are equivalent by checking the types and values. For table elements, verifies equivalence of each element by its key. 


    v1 - first input OdGiVariant object.
    v2 - second input OdGiVariant object.

    \returns
    true if all of the following conditions are met:
    * Neither of the objects are NULL;
    * Both objects have the same type;
    * Values of the objects extracted via asBoolean/asInt/etc.(other corresponding methods for various types) are equal.
    */
    static bool isEquivalent(const OdGiVariant *v1, const OdGiVariant *v2);
  private:
    VariantType m_type;
    OdVariant m_variant;
};

template <typename T>
inline bool operator ==(T left, const OdGiVariant::EnumType right)
{
  return (right == left);
}

template <typename T>
inline bool operator !=(T left, const OdGiVariant::EnumType right)
{
  return (right != left);
}

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGiVariant object pointers.
		
	<group Type Definitions>
*/
typedef OdSmartPtr<OdGiVariant> OdGiVariantPtr;

#include "TD_PackPop.h"

#endif // __ODGIVARIANT_H__
