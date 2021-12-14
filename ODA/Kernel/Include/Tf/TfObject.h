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

#ifndef __TF_OBJECT_H__
#define __TF_OBJECT_H__

#include "TD_PackPush.h"

#include "RxObject.h"
#include "RxIterator.h"
#include "TfVariant.h"
#include "TfObjectReactor.h"
#include "DbStubPtrArray.h"
#include "Gi/GiDrawable.h"

//DOM-IGNORE-BEGIN
#define ODTF_CREATE_ATTRIBUTE(Name, value) \
  result.push_back(::newOdTfAttribute(OD_T(#Name), pObject, value));

#define ODTF_DEFINE_ATTRIBUTES_CONSTRUCTOR(Name, ATTRIBUTES_LIST) \
  static OdArray<OdTfAttribute*> Name##AttributesConstructor(     \
      OdRxObject* pObject) {                                      \
    OdArray<OdTfAttribute*> result;                               \
    ATTRIBUTES_LIST(ODTF_CREATE_ATTRIBUTE)                        \
    return result;                                                \
  }

#define ODTF_CREATE_PROPERTY_NO_ATTRS(PropertyName, type, getterFn, setterFn, \
                                      defaultItemFn)                           \
  result.push_back(::newOdTfProperty(                                          \
      OD_T(#PropertyName), pClass, static_cast<OdTfVariant::Type>(type), NULL, \
      defaultItemFn, getterFn, setterFn));

#define ODTF_DEFINE_PROPERTIES_CONSTRUCTOR(ClassName, PROPERTIES_LIST) \
  static OdArray<OdTfProperty*> ClassName##propertiesConstructor(      \
      OdTfClass* pClass) {                                             \
    OdArray<OdTfProperty*> result;                                     \
    PROPERTIES_LIST(ODTF_CREATE_PROPERTY_NO_ATTRS);                    \
    return result;                                                     \
  }

#define ODTF_DECLARE_MEMBERS(ClassName)\
    ODRX_DECLARE_MEMBERS_GENERIC(OdTfClass, ClassName)

#define ODTF_DEFINE_RTTI_MEMBERS(ClassName, ParentClass) \
  ODRX_DEFINE_RTTI_MEMBERS_GENERIC(OdTfClass, ClassName, ParentClass)

#define ODTF_DEFINE_INIT_MEMBERS(ClassName, ParentClass, attrsConsFn,      \
                                 propsConsFn, pseudoConsFn, szClassName)   \
  ODRX_DEFINE_INIT_MEMBERS_GENERIC(                                        \
      ClassName, (::newOdTfClass(szClassName, ParentClass::desc(),         \
                                 attrsConsFn, propsConsFn, pseudoConsFn)), \
      (::newOdTfClass(szClassName, ParentClass::desc(), attrsConsFn,       \
                      propsConsFn, pseudoConsFn)))

#define ODTF_DEFINE_MEMBERS2(ClassName, ParentClass, attrsConsFn, propsConsFn, \
                             pseudoConsFn, szClassName)                        \
                                                                               \
  ODTF_DEFINE_RTTI_MEMBERS(ClassName, ParentClass)                             \
  ODTF_DEFINE_INIT_MEMBERS(ClassName, ParentClass, attrsConsFn, propsConsFn,   \
                           pseudoConsFn, szClassName)

#define ODTF_NO_CONS_NO_ATTRS_NO_PROPS_DEFINE_MEMBERS(ClassName, ParentClass) \
                                                                              \
  ODTF_DEFINE_MEMBERS2(ClassName, ParentClass, NULL, NULL, NULL,              \
                       OD_T(#ClassName))                                      \
                                                                              \
  ODRX_DEFINE_PSEUDOCONSTRUCTOR(ClassName, EMPTY_CONSTR)

#define ODTF_NO_CONS_NO_ATTRS_PROPS_DEFINE_MEMBERS(ClassName, ParentClass, \
                                                   PROPERTIES_LIST)        \
  ODTF_DEFINE_PROPERTIES_CONSTRUCTOR(ClassName, PROPERTIES_LIST)           \
  ODTF_DEFINE_MEMBERS2(ClassName, ParentClass, NULL,                       \
                       ClassName##propertiesConstructor, NULL,             \
                       OD_T(#ClassName))                                   \
                                                                           \
  ODRX_DEFINE_PSEUDOCONSTRUCTOR(ClassName, EMPTY_CONSTR)

#define ODTF_CONS_NO_ATTRS_PROPS_DEFINE_MEMBERS(ClassName, ParentClass,    \
                                                PROPERTIES_LIST, DOCREATE) \
  ODTF_DEFINE_PROPERTIES_CONSTRUCTOR(ClassName, PROPERTIES_LIST)           \
  ODTF_DEFINE_MEMBERS2(ClassName, ParentClass, NULL,                       \
                       ClassName##propertiesConstructor,                   \
                       ClassName::pseudoConstructor, OD_T(#ClassName))     \
                                                                           \
  ODRX_DEFINE_PSEUDOCONSTRUCTOR(ClassName, DOCREATE)

#define ODTF_CONS_NO_ATTRS_NO_PROPS_DEFINE_MEMBERS(ClassName, ParentClass, \
                                                   DOCREATE)               \
  ODTF_DEFINE_MEMBERS2(ClassName, ParentClass, NULL, NULL,                 \
                       ClassName::pseudoConstructor, OD_T(#ClassName))     \
                                                                           \
  ODRX_DEFINE_PSEUDOCONSTRUCTOR(ClassName, DOCREATE)

//DOM-IGNORE-END

class OdTfClass;
class OdTfProperty;
class OdTfAttribute;

/** \details 
An abstract interface class that implements storage for a set of attributes and provides access to attributes via their names.
<group OdTf_Classes>
*/
class ODRX_ABSTRACT TFCORE_EXPORT OdTfAttributesContainer : public OdRxObject {
  //DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdTfAttributesContainer);
  //DOM-IGNORE-END
public:
  /** \details 
  Retrieves the object that is the owner of the attributes container. 
  \returns Returns a pointer to the owner object represented as an instance of the OdRxObject class.
  */
  virtual OdRxObject* owner() const = 0;
  
  /** \details 
  Searches for an attribute in the container by its name.
  \param name [in] A string that contains the name of the attribute. 
  \returns Returns a pointer to the attribute object represented as an instance of the OdTfAttribute class. 
  \remarks 
  If the container contains more than one attribute with the specified name, the first found attribute is returned.
  */
  virtual const OdTfAttribute* find(const OdString& name) const = 0;
};

/** \details
A data type that represents a smart pointer to an OdTfAttributesContainer object.
<group OdTf_Types>
*/
typedef OdSmartPtr<OdTfAttributesContainer> OdTfAttributesContainerPtr;

/** \details 
An abstract interface class that implements storage for a set of properties and provides additional functionality: 
* Searching for a property by its index or name
* Checking property uniqueness 
* Sequential access to properties using an iterator object
<group OdTf_Classes>
*/
class ODRX_ABSTRACT TFCORE_EXPORT OdTfPropertiesContainer : public OdRxObject {
  //DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdTfPropertiesContainer);
  //DOM-IGNORE-END
public:

  /** \details
  A data type that represents a size of properties in bytes.
  <group OdTf_Types>
  */
  typedef OdInt32 size_type;

  /** \details 
  Retrieves the object that is the owner of the properties container. 
  \returns Returns a pointer to the owner object represented as an instance of the OdTfClass class.
  */
  virtual OdTfClass* owner() const = 0;
  
  /** \details 
  Retrieves the size of the properties container. 
  \returns Returns the quantity of bytes that are allocated by the properties container in the memory.
  */
  virtual size_type size() const = 0;
  
  /** \details 
  Retrieves whether the container contains a specified property. 
  \param pProp [in] A pointer to a property object to search for.
  \returns Returns true if the specified property is found in the container; otherwise returns false.
  */
  virtual bool contains(const OdTfProperty* pProp) const = 0;
  
  /** \details 
  Searches for a property in the container by its index, which specifies the property's position in the container. 
  \param index [in] An index of the property to be found.
  \returns Returns a pointer to a property object if it was found; otherwise returns a NULL pointer.
  */
  virtual const OdTfProperty* find(size_type index) const = 0;
  
  /** \details 
  Searches for a property in the container by its name. 
  \param name [in] A name of the property to be found.
  \returns Returns a pointer to a property object if it was found; otherwise returns a NULL pointer.
  \remarks 
  If the container contains more than one property with the specified name, the first found property is returned.
  */
  virtual const OdTfProperty* find(const OdString& name) const = 0;
  
  /** \details 
  Searches for a property in the container by its name and data type. 
  \param name [in] A name of the property to be found.
  \param type [in] A data type of the property to be found.
  \returns Returns a pointer to a property object if it was found; otherwise returns a NULL pointer.
  \remarks 
  A property in the container matches the search condition only if the name and data type match the search parameters.
  \remarks 
  If the container contains more than one property with the specified name and type, the first found property is returned.
  */
  virtual const OdTfProperty* find(const OdString& name,
                                   const OdTfVariant::Type type) const = 0;
                                   
  /** \details 
  Retrieves the index value for a specified property. The index value determines the position of the property in the container.
  \param pProp [in] A pointer to a property object.
  \returns Returns an index value if the specified property is found in the container; otherwise returns -1.
  */
  virtual size_type index(const OdTfProperty* pProp) const = 0;
  
  /** \details 
  Retrieves the uniqueness flag value for a property specified with its name. 
  The uniqueness flag value is equal to true if the property object is unique in the container; otherwise the flag value is equal to false.
  \param pProp [in] A property object's name.
  \returns Returns true if the property object is found in the container and is unique; otherwise returns false.
  */
  virtual bool unique(const OdString& name) const = 0;
  
  /** \details 
  Retrieves the uniqueness flag value for a property specified with its name and data type. 
  The uniqueness flag value is equal to true if the property object is unique in the container; otherwise the flag value is equal to false.
  \param pProp [in] A property object's name.
  \param type  [in] A property object's data type.
  \returns Returns true if the property object is found in the container and is unique; otherwise returns false.
  \remarks 
  A property in the container matches the search condition only if the name and data type match the search parameters.
  */
  virtual bool unique(const OdString& name,
                      const OdTfVariant::Type type) const = 0;
                      
  /** \details 
  Retrieves the properties container iterator object that provides sequential access to properties in the container. 
  \returns Returns a smart pointer to an iterator object represented as the OdRxIterator class instance.
  */
  virtual OdRxIteratorPtr iterator() const = 0;
};

/** \details
A data type that represents a smart pointer to an OdTfPropertiesContainer object.
<group OdTf_Types>
*/
typedef OdSmartPtr<OdTfPropertiesContainer> OdTfPropertiesContainerPtr;

class OdTfClassImpl;

/** \details
An abstract interface class that provides functionality for storing information about Tf classes. 
This interface provides an access to the class meta data of a Tf object.
Corresponding C++ library: Od_Tf
<group OdTf_Classes>
*/
class TFCORE_EXPORT OdTfClass : public OdRxClass {
  //DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdTfClass);
  //DOM-IGNORE-END
protected:
  OdTfClass(OdTfClassImpl*);
public:

  /** \details
  Retrieves the full list of class attributes. 
  \returns Returns a smart pointer to the class attributes container represented as an OdTfAttributesContainer class instance.
  */
  virtual const OdTfAttributesContainerPtr attributes() const;
  
  /** \details
  Retrieves the full list of class properties. 
  \returns Returns a smart pointer to the class properties container represented as an OdTfPropertiesContainer class instance.
  */
  virtual const OdTfPropertiesContainerPtr properties() const;
  
  /** \details
  Retrieves the full list of class properties, including properties of all base classes in the class hierarchy. 
  \returns Returns a smart pointer to the class properties container represented as an OdTfPropertiesContainer class instance.
  */
  virtual const OdTfPropertiesContainerPtr allProperties() const;
  
  /** \details
  Retrieves information about the base class that the class is inherited from. 
  \returns Returns a pointer to the base class object. 
  If the class is on the top of the class hierarchy, the method returns a NULL pointer. 
  If the base class is not an instance of the OdTfClass, the method also returns a NULL pointer.
  */
  virtual OdTfClass* tfParent() const;
};

/** \details
A data type that represents a smart pointer to an OdTfClass object.
<group OdTf_Types>
*/
typedef OdSmartPtr<OdTfClass> OdTfClassPtr;

/** \details
An abstract interface class that provides functionality for storing property data.
A property is defined with its name, data type, value and a set of property attributes.
Corresponding C++ library: Od_Tf
<group OdTf_Classes>
*/
class ODRX_ABSTRACT TFCORE_EXPORT OdTfProperty : public OdRxObject {
  //DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdTfProperty);
  //DOM-IGNORE-END
public:

  /** \details 
  Retrieves the owner class information for the property.
  \returns Returns a pointer to an OdTfClass object that owns the property.
  */
  virtual OdTfClass* owner() const = 0;
  
  /** \details 
  Retrieves the property data type.
  \returns Returns the property's data type represented with the OdTfVariant::Type enumeration value.
  */
  virtual OdTfVariant::Type type() const = 0;
  
  /** \details 
  Retrieves the property name.
  \returns Returns the property's name represented as an OdString object.
  */
  virtual const OdString& name() const = 0;
  
  /** \details 
  Retrieves the property attributes.
  \returns Returns a smart pointer to the property's attributes container.
  */
  virtual OdTfAttributesContainerPtr attributes() const = 0;
  
  /** \details
  Retrieves the current default value for the property. 
  \returns Returns the default value that should be used for the property instead of the OdTfVariant::kEmpty value.
  \remarks 
  For a single property, the method returns a value that should be used instead of the whole property.
  For an array property, the method returns a value that replaces all empty items.
  Note that the default value can also be equal to OdTfVariant::kEmpty.
  */
  virtual OdTfVariant defaultItemValue() const = 0;
  
  /** \details
  Retrieves the current value of the property. 
  \param pObject [in] A pointer to an object that encapsulates the property.
  \param value   [out] A placeholder for the value. 
  \returns Returns the result of retrieving the property value. 
  \remarks 
  If the value was successfully returned to a calling subroutine, the method returns the eOk value; otherwise it returns an appropriate error code.
  */
  virtual OdResult getValue(const OdTfObject* pObject,
                                     OdTfVariant& value) const = 0;

  /** \details
  Sets a new value of the property. 
  \param pObject [in] A pointer to an object that encapsulates the property.
  \param value   [out] A new value to be set. 
  \returns Returns the result of setting the property value. 
  \remarks 
  If the value was successfully set to a calling subroutine, the method returns the eOk value; otherwise it returns an appropriate error code.
  */
  virtual OdResult setValue(OdTfObject* pObject,
                                     const OdTfVariant& value) const = 0;
};

/** \details
A data type that represents a smart pointer to an OdTfProperty object.
<group OdTf_Types>
*/
typedef OdSmartPtr<OdTfProperty> OdTfPropertyPtr;

/** \details
An abstract interface class that provides functionality for storing attribute data.
An attribute is defined with its name and value.
Corresponding C++ library: Od_Tf
<group OdTf_Classes>
*/
class ODRX_ABSTRACT TFCORE_EXPORT OdTfAttribute : public OdRxObject {
  //DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdTfAttribute);
  //DOM-IGNORE-END
public:

  /** \details 
  Retrieves the owner object for the attribute.
  \returns Returns a pointer to the OdRxObject object that owns the attribute.
  */
  virtual OdRxObject* owner() const = 0;
  
  /** \details 
  Retrieves the attribute name.
  \returns Returns the attribute name represented as an OdString object.
  */
  virtual const OdString& name() const = 0;
  
  /** \details
  Retrieves the current value of the attribute. 
  \returns Returns the attribute value as an instance of the OdTfVariant class.
  */
  virtual OdTfVariant value() const = 0;
};

/** \details
A data type that represents a smart pointer to an OdTfAttribute object.
<group OdTf_Types>
*/
typedef OdSmartPtr<OdTfAttribute> OdTfAttributePtr;

/** \details
An abstract interface class that provides functionality for storing information about an object, allows  
to get an access to the internal object's data (properties) and process different events triggered within the object through adding reactor objects for it.
This class is an extension for the <exref target="https://docs.opendesign.com/tkernel/OdRxObject.html">OdRxObject class</exref>.

Corresponding C++ library: Od_Tf
<group OdTf_Classes>
\sa
* <exref target="https://docs.opendesign.com/tkernel/rtti.html">Run-Time Type Identification</exref> topics in ODA Kernel SDK documentation.
* <exref target="https://docs.opendesign.com/tbim/tbim_tf.html">Working with the Tf Interface</exref> topics in ODA BimRv SDK documentation.
*/
class TFCORE_EXPORT OdTfObject : public OdGiDrawable {
  //DOM-IGNORE-BEGIN
  ODTF_DECLARE_MEMBERS(OdTfObject);
  //DOM-IGNORE-END

public:

  /** \details
  A data type that represents a property index.
  <group OdTf_Types>
  */
  typedef OdTfPropertiesContainer::size_type PropertyIndexType;
  
  /** \details
  A data type that represents an item index.
  <group OdTf_Types>
  */
  typedef OdTfVariantArray::size_type ItemIndexType;

  /** \details 
  Retrieves the object's owner object. 
  \returns Returns a pointer to the object's owner represented as an instance of the OdTfObject class.
  */
  virtual const OdTfObject* getOwner() const = 0;

  /** \details 
  Retrieves the object's property value. 
  \param pPI        [in]  A pointer to the property object that has the value to be retrieved.
  \param value      [out] A placeholder for the property value.
  \param arrayIndex [in]  An index of the value from the array property to be set, if the property is an array.
  \returns Returns the result of retrieving the property's value. 
  \remarks 
  If the property specified with the pPI pointer is an array and the arrayIndex parameter is specified, 
  the method stores the value at the arrayIndex position in the value output parameter. 
  If the property is not an array, the value of the property object is stored in the value output parameter.
  If the property value was successfully returned to a calling subroutine, the method returns the eOk value; 
  otherwise it returns an appropriate error code (for example, if the pointer to the property is invalid, the value index is not correct, or any other error occurs).
  */
  virtual OdResult getProperty(
      const OdTfProperty* pPI, OdTfVariant& value,
      const ItemIndexType arrayIndex = -1) const = 0;

  /** \details 
  Sets a new value for the object's property. 
  \param pPI        [in] A pointer to the property object that has the value to be set.
  \param value      [in] A new property value.
  \param arrayIndex [in] An index of the value from the array property to be set, if the property is an array.
  \returns Returns the result of setting the property's value. 
  \remarks 
  If the property specified with the pPI pointer is an array and the arrayIndex parameter is specified, 
  the method sets the new value at the arrayIndex position in the property array. 
  If the property is not an array, the value of the property object is replaced with the new value.
  If the property value was successfully set, the method returns the eOk value; 
  otherwise it returns an appropriate error code (for example, if the pointer to the property is invalid, the value index is not correct, or any other error occurs).
  */
  virtual OdResult setProperty(
      const OdTfProperty* pPI, const OdTfVariant& value,
      const ItemIndexType arrayIndex = -1) = 0;

  /** \details
  Retrieves the object's property value. 
  \param name       [in]  A string that represents the property name.
  \param value      [out] A placeholder for the property value.
  \param arrayIndex [in]  An index of the value from the array property to be set, if the property is an array.
  \param pClass     [in]  A pointer to the class object that owns the property. 
  \returns Returns the result of retrieving the property's value. 
  \remarks 
  The method searches for a property object with the specified name in the array of class properties. 
  If the property is an array and the arrayIndex parameter is specified, the method stores the value at the arrayIndex position in the value output parameter. 
  If the property is not an array, the value of the found property object is stored in the value output parameter.
  If the pointer to the owner object is NULL, the method searches for the property in the hierarchy of the object's parent classes until the property with the specified name is found.
  If the pointer to the owner object is not NULL, the method searches for the property in the properties of the object that is specified with the pointer passed in the pClass parameter. 
  The found property value is stored in the value output parameter.
  If the property value was successfully returned to a calling subroutine, the method returns the eOk value; 
  otherwise it returns an appropriate error code (for example, if the property index or value index is not correct or any other error occurs).
  The method returns the eAmbiguousOutput value if there is more than one property with the specified name in the pClass owner object. 
  In this case the first found property's value is returned.
  */
  virtual OdResult getProperty(
      const OdString& name, OdTfVariant& value,
      const ItemIndexType arrayIndex = -1,
      const OdTfClass* pClass = NULL) const = 0;
      
  /** \details 
  Sets a new value for the object's property. 
  \param name       [in] A string that represents the property name.
  \param value      [in] A new property value.
  \param arrayIndex [in] An index of the value from the array property to be set, if the property is an array.
  \param pClass     [in] A pointer to the class object that owns the property. 
  \returns Returns the result of setting the property's value. 
  \remarks 
  The method searches for a property object with the specified name in the array of class properties. 
  If the property is an array and the arrayIndex parameter is specified, the method sets the new value at the arrayIndex position in the property array. 
  If the property is not an array, the value of the found property object is replaced with the new value.
  
  If the pointer to the owner object is NULL, the method searches for the property in the hierarchy of the object's parent classes until the property with the specified name is found.
  If the pointer to the owner object is not NULL, the method searches for the property in the properties of the object that is specified with the pointer passed in the pClass parameter. 
  If the property was found, the method modifies it with the new value.
  If the property value was successfully set, the method returns the eOk value; 
  otherwise it returns an appropriate error code (for example, if the property index or value index is not correct or if any other error occurs).
  The method returns the eAmbiguousOutput value if there is more than one property with the specified name in the pClass owner object. 
  In this case the first found property's value is modified.
  */
  virtual OdResult setProperty(const OdString& name,
                                        const OdTfVariant& value,
                                        const ItemIndexType arrayIndex = -1,
                                        const OdTfClass* pClass = NULL) = 0;
  /** \details
  Retrieves the object's property value. 
  \param propertyIndex  [in] An index of the property in the class field array.
  \param value          [out] A placeholder for the property value.
  \param arrayIndex     [in]  An index of the retrieved value from the array property, if the property is an array.
  \returns Returns the result of retrieving the property's value. 
  \remarks 
  The method searches for a property object allocated at the propertyIndex position in the array of class properties. 
  If the property is an array and the arrayIndex parameter is specified, the method stores the value at the arrayIndex position in the value output parameter. 
  If the property is not an array, the value of the found property object is stored in the value output parameter.
  If the property value was successfully returned to a calling subroutine, the method returns the eOk value; 
  otherwise it returns an appropriate error code (for example, if the property index or value index is not correct or if any other error occurs).
  */
  virtual OdResult getProperty(
      const PropertyIndexType propertyIndex, OdTfVariant& value,
      const ItemIndexType arrayIndex = -1) const = 0;
      
  /** \details
  Sets a new value for the object's property. 
  \param propertyIndex  [in] An index of the property in the class field array.
  \param value          [in] A new property value.
  \param arrayIndex     [in] An index of the value from the array property to be set, if the property is an array.
  \returns Returns the result of setting the property's value. 
  \remarks 
  The method searches for a property object allocated at the propertyIndex position in the array of class properties. 
  If the property is an array and the arrayIndex parameter is specified, the method sets the new value at the arrayIndex position in the property array. 
  If the property is not an array, the value of the found property object is replaced with the new value.
  If the property value was successfully set, the method returns the eOk value; 
  otherwise it returns an appropriate error code (for example, if the property index or value index is not correct or if any other error occurs).
  */
  virtual OdResult setProperty(
      const PropertyIndexType propertyIndex, const OdTfVariant& value,
      const ItemIndexType arrayIndex = -1) = 0;

  /** \details
  Adds a new transient reactor object to the element's reactor list.
  \param pReactor [in] A pointer to the transient reactor object.
  \remarks
  Before adding a transient reactor object, open the element either in the read or write mode. 
  */
  virtual void addReactor(OdTfObjectReactor* pReactor) = 0;

  /** \details
  Removes a specified transient reactor object from the element's reactor list.
  \param pReactor [in] A pointer to the transient reactor object.
  \remarks
  Before removing a transient reactor object, open the element either in the read or write mode. 
  */
  virtual void removeReactor(OdTfObjectReactor* pReactor) = 0;

  /** \details
  Adds a new persistent reactor object to the element's reactor list.
  \param pPersistentReactor [in] A pointer to the persistent reactor object.
  \remarks
  Before adding a persistent reactor object, open the element in the write mode. 
  If the persistent reactor does not have an owner object, it isn't saved within the drawing.
  */
  virtual void addPersistentReactor(OdTfPersistentReactor* pPersistentReactor) = 0;

  /** \details
  Removes a specified persistent reactor object from the element's reactor list.
  \param pPersistentReactor [in] A pointer to the persistent reactor object.
  \remarks
  Before removing a persistent reactor object, open the element in the write mode. 
  */
  virtual void removePersistentReactor(OdTfPersistentReactor* pPersistentReactor) = 0;

  /** \details
  Retrieves whether a specified persistent reactor object is attached to the element. 
  \param pPersistentReactor [in] A pointer to the persistent reactor object. 
  \returns Returns true if the specified persistent reactor object is attached to the element; otherwise returns false.
  */
  virtual bool hasPersistentReactor(OdTfPersistentReactor* pPersistentReactor) const = 0;

  /** \details
  Retrieves all the persistent reactors from the element. 
  \param elmIds [out] A placeholder for the array of reactor object identifiers.
  \remarks 
  The method fills the passed array with identifiers of persistent reactor objects for the element and returns it to a calling subroutine. 
  */
  virtual void getPersistentReactors(OdTfPersistentReactorArray& elmIds) = 0;

  /** \details
  Retrieves all the transient reactors from the element. 
  \param elmIds [out] A placeholder for the array of reactor objects identifiers.
  \remarks 
  The method fills the passed array with identifiers of transient reactor objects for the element and returns it to a calling subroutine. 
  */
  virtual void getTransientReactors(OdTfObjectReactorArray& reactors) = 0;
};

/** \details
A data type that represents a smart pointer to an OdTfObject object.
<group OdTf_Types>
*/
typedef OdSmartPtr<OdTfObject> OdTfObjectPtr;

//
//
//

/** \details
A data type that represents a pointer to a function that creates a new properties container derived from a specified class.
<group OdTf_Types>
*/
typedef OdArray<OdTfProperty*>(*OdTfPropertiesConstructorType)(OdTfClass*);

/** \details
A data type that represents a pointer to a function that creates a new attributes container derived from a specified class.
<group OdTf_Types>
*/
typedef OdArray<OdTfAttribute*>(*OdTfAttributesConstructorType)(OdRxObject*);

/** \details
A data type that represents a pointer to a function that returns a value of a property owned by a specified object.
<group OdTf_Types>
*/
typedef OdResult (*OdTfPropertyGetterType)(const OdTfObject*,
                                                    OdTfVariant&);
                                                    
/** \details
A data type that represents a pointer to a function that sets a new value of a property owned by a specified object.
<group OdTf_Types>
*/
typedef OdResult (*OdTfPropertySetterType)(OdTfObject*,
                                                    const OdTfVariant&);
                                                    
/** \details
A data type that represents a pointer to a function that creates a variant value.
<group OdTf_Types>
*/
typedef OdTfVariant (*OdTfVariantConstructorType)();

/** \details
This function creates and initializes a new Tf class object with specified parameters.
\param name         [in] A name of a class to be created.
\param pBaseClass   [in] A pointer to the base class.
\param pAttrsConstr [in] A pointer to the function that creates an attribute container object for instances of the class.
\param pPropsConstr [in] A pointer to the function that creates a property container object for instances of the class.
\param pConstr      [in] A pointer to the function that creates an instance of the class.
\param bRegister    [in] A flag value that determines whether the created class should be registered in the run-time type identification system (if equal to true) or not (if equal to false).
\returns Returns a raw pointer to the created and initialized class object. 
\remarks
The method creates an instance of the class by calling the createOdTfClass() function, then attribute and property containers are created 
using the pointers passed to appropriate constructor functions. 
For the created class instance, the initOdTfClass() function is called to initialize it. 
<group OdTf_Functions>
*/
TFCORE_EXPORT OdTfClass* newOdTfClass(
    const OdString& name, OdRxClass* pBaseClass,
    OdTfAttributesConstructorType pAttrsConstr,
    OdTfPropertiesConstructorType pPropsConstr, OdPseudoConstructorType pConstr,
    bool bRegister = true);

/** \details
This function creates a new Tf class information storage object with default parameters (acts as a default constructor for the OdTfClass).
\returns Returns a raw pointer to the created class. 
<group OdTf_Functions>
*/
TFCORE_EXPORT OdTfClass* createOdTfClass();

/** \details
This function initializes a previously created Tf class object with specified parameters: name, base class, constructor function, attributes and properties.
\param pClass     [in] A pointer to the previously created class instance.
\param name       [in] A name of the class.
\param pBaseClass [in] A pointer to the base class. After the method successfully runs, it refers to an instance of the base class.
\param attributes [in] An attributes container for the class instance.
\param properties [in] A properties container for the class instance.
\param pConstr    [in] A pointer to the function that creates an instance of the class.
\param bRegister  [in] A flag value that determines whether the class should be registered in the run-time type identification system (if equal to true) or not (if equal to false).
\returns Returns a pointer to the initialized class object. 
\remarks
The method creates an instance of the base class and initializes it with a constructor function, name, and a set of other default parameters. 
Then the method creates and initializes objects for storing attribute and property containers, and associates them with the class instance. 
As the last step of the initialization, the method associates the base class instance and property and attribute containers with the class object. 
<group OdTf_Functions>
*/
TFCORE_EXPORT OdTfClass* initOdTfClass(
    OdTfClass* pClass, const OdString& name, OdRxClass* pBaseClass,
    const OdArray<OdTfAttribute*>& attributes,
    const OdArray<OdTfProperty*>& properties, OdPseudoConstructorType pConstr,
    bool bRegister = true);

/** \details
This function creates and initializes a new property object with specified parameters.
\param name                     [in] A name of the property.
\param pOwner                   [in] A pointer to the owner object.
\param type                     [in] A data type of the property.
\param pAttrsConstr             [in] A pointer to the function that creates an attributes container object for the property.
\param pDefaultValueConstructor [in] A pointer to the function that creates a default value of the property.
\param pGetter                  [in] A pointer to the function that retrieves and returns the current value of the property.
\param pSetter                  [in] A pointer to the function that sets a new value of the property.
\returns Returns a raw pointer to the created and initialized property object. 
<group OdTf_Functions>
*/
TFCORE_EXPORT OdTfProperty* newOdTfProperty(
    const OdString& name, OdTfClass* pOwner, OdTfVariant::Type type,
    OdTfAttributesConstructorType pAttrsConstr,
    OdTfVariantConstructorType pDefaultValueConstructor,
    OdTfPropertyGetterType pGetter, OdTfPropertySetterType pSetter);

/** \details
This function creates a new property object with default parameters (acts as a default constructor for OdTfProperty).
\returns Returns a raw pointer to the created property object. 
<group OdTf_Functions>
*/
TFCORE_EXPORT OdTfProperty* createOdTfProperty();

/** \details
This function initializes a previously created property object.
\param pProp                    [in] A pointer to a property object that should be initialized.
\param name                     [in] A name of the property.
\param pOwner                   [in] A pointer to the owner object.
\param type                     [in] A data type of the property.
\param attributes               [in] An attributes container object for the property.
\param pDefaultValueConstructor [in] A pointer to the function that creates a default value of the property.
\param pGetter                  [in] A pointer to the function that retrieves and returns the current value of the property.
\param pSetter                  [in] A pointer to the function that sets a new value of the property.
\returns Returns a raw pointer to the initialized property object. 
<group OdTf_Functions>
*/
TFCORE_EXPORT OdTfProperty* initOdTfProperty(
    OdTfProperty* pProp, const OdString& name, OdTfClass* pOwner,
    OdTfVariant::Type type, const OdArray<OdTfAttribute*>& attributes,
    OdTfVariantConstructorType pDefaultValueConstructor,
    OdTfPropertyGetterType pGetter, OdTfPropertySetterType pSetter);

/** \details
This function creates and initializes a new attribute object with specified parameters.
\param name                     [in] A name of the attribute.
\param pOwner                   [in] A pointer to the owner object.
\param value                    [in] A data type of the property.
\returns Returns a raw pointer to the created and initialized attribute object. 
<group OdTf_Functions>
*/
TFCORE_EXPORT OdTfAttribute* newOdTfAttribute(const OdString& name,
                                              OdRxObject* pOwner,
                                              const OdTfVariant& value);

#endif  // __TF_OBJECT_H__
