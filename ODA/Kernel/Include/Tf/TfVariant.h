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

#ifndef __TF_VARIANT_H__
#define __TF_VARIANT_H__

#include "RxVariant.h"
#include "Tf/TfExport.h"
#include "OdGUID.h"

class OdDbStub;

class OdTfVariant;

/** \details
A data type that represents an array of OdTfVariant objects. All objects in the array have a similar data type.
<group OdTf_Types>
*/
typedef OdArray<OdTfVariant> OdTfVariantArray;

/** \details
A data type that represents a tuple of OdTfVariant objects. In contrast to an array, a tuple can contain objects of different data types.
<group OdTf_Types>
*/
typedef OdArray<OdTfVariant> OdTfVariantTuple;

/** \details
A class that implements an extension of the <exref target="https://docs.opendesign.com/tkernel/OdVariant.html">OdVariant</exref> 
class that can be used as an universal storage of typified data.
Corresponding C++ library: Od_Tf
<group OdTf_Classes>
*/
class TFCORE_EXPORT OdTfVariant : public OdVariant {
public:

  /** \details
  Creates a factory (if it was not created previously) for variant objects of a specified data type.
  \param type [in] A data type of objects that are created by the factory.
  \returns Returns a raw pointer to a factory object.
  */
  static const OdVariant::TypeFactory* typeFactory(int type);
  
  /** \details
  Data types for Tf variant objects.
  <group OdTf_Types>
  */
  typedef enum {
    kGUID = OdVariant::kNextType, // A Global Unique Identifier (GUID) data type.
    kEmpty,                       // An empty variant value.
    kDEPRECARED0,                 // A deprecated data type (for compatibility only). Do not use it. Note that there can be other deprecated data types in future releases.
    kDbStubPtr,                   // A raw pointer to a stub.
    kRxObjectWeakPtr,             // A weak pointer to an OdRxObject.
    kTuple,                       // A tuple of variant values of one data type.
    kNextType                     // A system data type (for internal use only).
  } Type;

  /** \details
  Dynamically generates a variant data type that represents a multi-dimensional array of values with a specified data type.
  \param nDims    [in] A quantity of the array's dimensions.
  \param itemType [in] A data type.
  \returns Returns the requested data type.
  */
  static Type kVariantArray(OdUInt16 nDims, OdUInt32 itemType);
  
  /** \details
  Dynamically generates a variant data type that represents a multi-dimensional array of values with a specified data type.
  \param nDims    [in] A quantity of the array's dimensions.
  \param itemType [in] A data type.
  \returns Returns the requested data type.
  */
  static Type kVariantArray(OdUInt16 nDims, OdTfVariant::Type itemType);
  
  /** \details
  Dynamically generates a variant data type that represents an array of values with a specified data type.
  \param itemType [in] A data type.
  \returns Returns the requested data type.
  */
  static Type kVariantArray(OdUInt32 itemType);
  
  /** \details
  Dynamically generates a variant data type that represents an array of values with a specified data type.
  \param itemType [in] A data type.
  \returns Returns the requested data type.
  */
  static Type kVariantArray(OdTfVariant::Type type);

  /** \details
  Retrieves the data type of an element of a multi-dimensional array defined with its data type.
  \param type [in] A data type that represents a multi-dimensional array.
  \returns Returns the data type that any element of specified multi-dimension array has.
  */
  static Type getItemType(OdTfVariant::Type type);
  
  /**
  Retrieves the dimension quantity for a specified data type.
  \param type [in] A data type.
  \returns Returns dimensions count.
  */
  static OdUInt16 getDimsCount(OdTfVariant::Type type);
  
  /** \details 
  Creates a new Tf variant object with an empty value. 
  The created variant object has the OdTfVriantType::kEmpty type.
  \returns Returns a new Tf variant object with an empty value. 
  */
  static OdTfVariant empty();

  /** \details
  Retrieves whether the variant object is empty. 
  \returns Returns true if the variant object is empty; otherwise returns false.
  */
  bool isEmpty() const;
  
  /** \details
  Retrieves whether the variant object is an instance of a variant array. 
  \returns Returns true if the variant object is a variant arrey; otherwise returns false.
  */
  bool isVariantArray() const;

  /** \details
  Creates an empty Tf variant object. 
  */
  OdTfVariant();
  
  /** \details
  Creates a new Tf variant object that is a copy of another variant object represented with the OdVariant class instance. 
  \param value [in] An OdVariant object that is copied to a created Tf variant object.
  */
  OdTfVariant(const OdVariant& value);
  
  /** \details
  Creates a new Tf variant object that is a copy of another Tf variant object (copy constructor). 
  \param value [in] An OdTfVariant object that is copied to a created Tf variant object.
  */
  OdTfVariant(const OdTfVariant& value);
  
  /** \details
  Creates a new Tf variant object with a specified GUID. 
  \param value [in] A GUID value.
  */
  OdTfVariant(const OdGUID& value);
  
  /** \details
  Creates a new tuple of Tf variant objects from an existing variant object tuple. 
  \param value [in] An original variant object tuple that is copied to the created one.
  */
  OdTfVariant(const OdTfVariantTuple& value);
  
  /** \details
  Creates a new array of Tf variant objects of a specified data type from an existing array. 
  \param type  [in] A data type.
  \param value [in] An original variant object array that is copied to the created one.
  */
  OdTfVariant(OdTfVariant::Type type, const OdTfVariantArray& value);
  
  /** \details
  Creates a new Tf variant object from a specified raw data stub.
  \param value [in] A pointer to a raw stub.
  */
  OdTfVariant(OdDbStub* const value);

  /** \details
  The assignment operator for two OdTfVariant objects.
  \param value [in] A right operand of the assignment operation.
  \returns Returns this variant object after the assignment operation.
  */
  OdTfVariant& operator =(const OdTfVariant& value);

  /** \details
  Destroys the variant object.
  */
  virtual ~OdTfVariant();
  
  /** \details
  Retrieves the current GUID of the variant object.
  \returns Returns the current GUID of the variant object.
  */
  const OdGUID& getGUID() const;
  
  /** \details
  Retrieves the current array of variant values from the object.
  \returns Returns the array of variant values.
  */
  const OdTfVariantArray& getVariantArray() const;
  
  /** \details
  Retrieves the current raw data stub associated with the variant object.
  \returns Returns a pointer to the raw data stub.
  */
  OdDbStub* getDbStubPtr() const;
  
  /** \details
  Retrieves the weak pointer to an OdRxObject class instance associated with the variant object.
  \returns Returns a raw pointer to the OdRxObject class instance.
  */
  OdRxObject* getRxObjectWeakPtr() const;
  
  /** \details
  Retrieves the current tuple of variant values from the object.
  \returns Returns the tuple of variant values.
  */
  const OdTfVariantTuple& getVariantTuple() const;

  /** \details
  Retrieves the current variant data as a variant array.
  \returns Returns the array of variant values.
  \remarks
  In the variant object has a data type that differs from the array data type, the method throws an exception.
  */
  OdTfVariantArray& asVariantArray();
  
  /** \details
  Retrieves the current variant data as a variant tuple.
  \returns Returns the tuple of variant values.
  \remarks
  In the variant object has a data type that differs from the tuple data type, the method throws an exception.
  */
  OdTfVariantTuple& asVariantTuple();

  /** \details
  Sets a new GUID for the variant object.
  \param value [in] A new GUID value.
  \returns Returns the reference to the modified variant object.
  */
  OdTfVariant& setGUID(const OdGUID& value);
  
  /** \details
  Sets a new data type and variant values array for the variant object.
  \param type  [in] A new data type value.
  \param value [in] A new values array.
  \returns Returns the reference to the modified variant object.
  */
  OdTfVariant& setVariantArray(OdTfVariant::Type type, const OdTfVariantArray& value);
  
  /** \details
  Sets a new variant value tuple for the variant object.
  \param value [in] A new values tuple.
  \returns Returns the reference to the modified variant object.
  */
  OdTfVariant& setVariantTuple(const OdTfVariantTuple& value);
  
  /** \details
  Sets a new raw data stub for the variant object.
  \param value [in] A new pointer to the stub.
  \returns Returns the reference to the modified variant object.
  */
  OdTfVariant& setDbStubPtr(OdDbStub* const value);
  
  /** \details
  Sets a new weak pointer to the OdRxObject class instance associated with the variant object.
  \param value [in] A pointer to the OdRxObject class instance.
  \returns Returns the reference to the modified variant object.
  */
  OdTfVariant& setRxObjectWeakPtr(OdRxObject* const value);

//DOM-IGNORE-BEGIN
protected:
  void setVarType(int newType, int& type, void* data);
  void makeEmpty();
//DOM-IGNORE-END
};

/** \details
The comparison operator for two Tf variant data types.
\param lhs [in] A left-hand operand for the comparison operation.
\param rhs [in] A right-hand operand for the comparison operation.
\returns Returns true if the specified data types can be cast to each other; otherwise returns false.
*/
inline bool operator==(OdTfVariant::Type lhs, OdVariant::Type rhs) {
  return lhs == static_cast<OdTfVariant::Type>(rhs);
}

/** \details
The non-equality operator for two Tf variant data types.
\param lhs [in] A left-hand operand.
\param rhs [in] A right-hand operand.
\returns Returns true if the specified data types are not equal; otherwise returns false.
*/
inline bool operator!=(OdTfVariant::Type lhs, OdVariant::Type rhs) {
  return !(lhs == rhs);
}

/** \details
The comparison operator for two OdVariant data types.
\param lhs [in] A left-hand operand for the comparison operation.
\param rhs [in] A right-hand operand for the comparison operation.
\returns Returns true if the specified data types are equal; otherwise returns false.
*/
inline bool operator==(OdVariant::Type lhs, OdTfVariant::Type rhs) {
  return rhs == lhs;
}

/** \details
The non-equality operator for two OdVariant data types.
\param lhs [in] A left-hand operand.
\param rhs [in] A right-hand operand.
\returns Returns true if the specified data types are not equal; otherwise returns false.
*/
inline bool operator!=(OdVariant::Type lhs, OdTfVariant::Type rhs) {
  return rhs != lhs;
}

#endif
