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
///////////////////////////////////////////////////////////////////////////////
// Examples:
//
// R/W Property
// ODRX_DECLARE_RX_PROPERTY_START(OdDbDatabase, Splinetype, int)
//   ODRX_DECLARE_RX_PROPERTY_GET_BY_VAL(OdDbDatabase, getSPLINETYPE, int);
//   ODRX_DECLARE_RX_PROPERTY_SET_BY_VAL(OdDbDatabase, setPLINETYPE, int);
// ODRX_DECLARE_RX_PROPERTY_END;
//
// R/O Property
// ODRX_DECLARE_RX_PROPERTY_START(OdDbDatabase, Splinetype, int)
//   ODRX_DECLARE_RX_PROPERTY_GET_BY_VAL(OdDbDatabase, getPLINETYPE, int);
//   ODRX_DECLARE_RX_PROPERTY_READONLY;
// ODRX_DECLARE_RX_PROPERTY_END;
//
// R/W Property with subproperty
// ODRX_DECLARE_RX_PROPERTY_START(OdDbDatabase, Pextmin, OdGePoint3d)
//   ODRX_DECLARE_RX_PROPERTY_GET_BY_VAL(OdDbDatabase, getPEXTMIN, OdGePoint3d);
//   ODRX_DECLARE_RX_PROPERTY_SET_BY_VAL(OdDbDatabase, setPEXTMIN, OdGePoint3d);
//   ODRX_DECLARE_RX_PROPERTY_SUBCHILDREN_START
//     ODRX_DECLARE_RX_PROPERTY_SUBCHILDREN_ADD(X, double);
//     ODRX_DECLARE_RX_PROPERTY_SUBCHILDREN_ADD(Y, double);
//     ODRX_DECLARE_RX_PROPERTY_SUBCHILDREN_ADD(Z, double);
//   ODRX_DECLARE_RX_PROPERTY_SUBCHILDREN_END;
// ODRX_DECLARE_RX_PROPERTY_END;
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _RXPROPERTY_H_INCLUDED_
#define _RXPROPERTY_H_INCLUDED_

#include "RxMember.h"
#include "RxObjectImpl.h"

class OdRxPropertyBaseImpl;
class OdRxValueIteratorImpl;
class OdRxPropertyIteratorImpl;
class OdRxDictionaryPropertyImpl;
class OdRxPropertyImpl;
class OdRxCollectionPropertyImpl;
class OdRxIndexedPropertyImpl;

/**  \details
   Base class for scalar and collection properties
   
  <group OdRx_Classes>
*/
class FIRSTDLL_EXPORT OdRxPropertyBase : public OdRxMember
{
public:
  ODRX_DECLARE_MEMBERS(OdRxPropertyBase);
  virtual bool isReadOnly(const OdRxObject* pO) const;
protected:
  OdRxPropertyBase(OdRxPropertyBaseImpl*);
};

typedef OdSmartPtr<OdRxPropertyBase> OdRxPropertyBasePtr;

/** \details
  <group OdRx_Classes>
  Scalar property class. Represent a proxy access to some object property. Usable in property palettes, fields evaluation etc.
*/
class FIRSTDLL_EXPORT OdRxProperty : public OdRxPropertyBase
{
public:
  ODRX_DECLARE_MEMBERS(OdRxProperty);
  OdRxProperty();
  
  /** \details
    Get the property value. 
    \param pO [in] The object which property to retrieve. 
    \param value [out] Resulting value.
    \returns Returns eOk if successful.
  */
  OdResult getValue(const OdRxObject* pO, OdRxValue& value) const;
  
  /** \details
    Set the property value.
    \param pO [in] The object which property to set.
    \param value [in] Property value to set.
    \returns Returns eOk if successful. If it returns eNotApplicable the property is considered read-only.
  */
  OdResult setValue(OdRxObject* pO, const OdRxValue& value) const;
protected:
  /** \details
    Method called by getValue() to actually get the value. To be overloaded by the property implementation.
  */
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const;

  /** \details
    Method called by setValue() to actually set the value. To be overloaded by the property implementation.
  */
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const;
  friend class OdRxPropertyImpl;
  friend class OdRxPropertyOverrule;
  OdRxProperty(OdRxPropertyImpl*);
};

typedef OdSmartPtr<OdRxProperty> OdRxPropertyPtr;

/**   
  <group OdRx_Classes>
  Not implemented yet.
*/
class FIRSTDLL_EXPORT OdRxPropertyOverrule : public OdRxMemberOverrule
{
public:
  ODRX_DECLARE_MEMBERS(OdRxPropertyOverrule);
  virtual OdResult getValue(const OdRxProperty* pProp, const OdRxObject* pO, OdRxValue& value) const;
  virtual OdResult setValue(const OdRxProperty* pProp, OdRxObject* pO, const OdRxValue& value) const;
};


/** \details
  <group OdRx_Classes>
  See OdRxMemberQueryEngine::promotingContext for details.
  Users may derive from this class to create user specific promoting contexts.
*/
class FIRSTDLL_EXPORT OdRxPromotingQueryContext : public OdRxMemberQueryContext
{
public:
  ODRX_DECLARE_MEMBERS(OdRxPromotingQueryContext);
};

/** \details
  <group OdRx_Classes>
  Generic iterator interface for the collection properties.
*/
struct ODRX_ABSTRACT FIRSTDLL_EXPORT OdRxValueIterator : OdRxObject
{
  ODRX_DECLARE_MEMBERS(OdRxValueIterator);
  /** \details
    Returns true if the iteration is ended.
  */
  virtual bool done() = 0;
  /** \details
    Step to the next item. Returns true if there actually was a next item.
  */
  virtual bool next() = 0;
  /** \details
    Returns the current item.
  */
  virtual OdRxValue current() const = 0;
};

typedef OdSmartPtr<OdRxValueIterator>OdRxValueIteratorPtr;

/**   
  <group OdRx_Classes>
*/
class FIRSTDLL_EXPORT OdRxCollectionProperty : public OdRxPropertyBase
{
public:
  ODRX_DECLARE_MEMBERS(OdRxCollectionProperty);
  OdRxValueIteratorPtr newValueIterator(const OdRxObject* pO) const;
  OdResult tryGetCount(const OdRxObject* pO, int& count) const;
  OdRxCollectionProperty();
protected:
  virtual OdRxValueIteratorPtr subNewValueIterator(const OdRxObject* pO) const;
  virtual OdResult subTryGetCount(const OdRxObject* pO, int& count) const;
  using OdRxMember::init;
  friend class OdRxCollectionPropertyImpl;
  friend class OdRxCollectionPropertyOverrule;
  OdRxCollectionProperty(OdRxCollectionPropertyImpl* pImpl);
};

typedef OdSmartPtr<OdRxCollectionProperty>OdRxCollectionPropertyPtr;

/**   
  <group OdRx_Classes>
*/
class FIRSTDLL_EXPORT OdRxCollectionPropertyOverrule : public OdRxMemberOverrule
{
public:
  ODRX_DECLARE_MEMBERS(OdRxCollectionPropertyOverrule);
  virtual OdRxValueIterator* newValueIterator(const OdRxCollectionProperty* pProp, const OdRxObject* pO) const;
  virtual OdResult tryGetCount(const OdRxCollectionProperty* pProp, const OdRxObject* pO, int& count) const;
};

/**   
  <group OdRx_Classes>
*/
class FIRSTDLL_EXPORT OdRxIndexedProperty : public OdRxCollectionProperty
{
public:
  ODRX_DECLARE_MEMBERS(OdRxIndexedProperty);
  OdResult getValue(const OdRxObject* pO, int index, OdRxValue& value) const;
  OdResult setValue(OdRxObject* pO, int index, const OdRxValue& value) const;
  OdResult insertValue(OdRxObject* pO, int index, const OdRxValue& value) const;
  OdResult removeValue(OdRxObject* pO, int index) const;
  OdRxIndexedProperty();
protected:
  virtual OdResult subGetValue(const OdRxObject* pO, int index, OdRxValue& value) const { return eNotApplicable; }
  virtual OdResult subSetValue(OdRxObject* pO, int index, const OdRxValue& value) const { return eNotApplicable; }
  virtual OdResult subInsertValue(OdRxObject* pO, int index, const OdRxValue& value) const { return eNotApplicable; }
  virtual OdResult subRemoveValue(OdRxObject* pO, int index) const { return eNotApplicable; }
  static OdRxCollectionPropertyPtr createObject(const OdString& name, const OdRxValueType& type, const OdRxObject* owner = NULL);
  friend class OdRxIndexedPropertyImpl;
  OdRxIndexedProperty(OdRxIndexedPropertyImpl* pImpl);
};


typedef OdSmartPtr<OdRxIndexedProperty> OdRxIndexedPropertyPtr;

/**   
  <group OdRx_Classes>
*/
class FIRSTDLL_EXPORT OdRxDictionaryProperty : public OdRxCollectionProperty
{
public:
  ODRX_DECLARE_MEMBERS(OdRxDictionaryProperty);
  OdResult getValue(const OdRxObject* pO, const OdString& key, OdRxValue& value) const;
  OdResult setValue(OdRxObject* pO, const OdString& key, const OdRxValue& value) const;
  OdRxDictionaryProperty();
protected:
  virtual OdResult subGetValue(const OdRxObject* pO, const OdString& key, OdRxValue& value) const { return eNotApplicable; }
  virtual OdResult subSetValue(OdRxObject* pO, const OdString& key, const OdRxValue& value) const { return eNotApplicable; }
  static OdRxCollectionPropertyPtr createObject(const OdString& name, const OdRxValueType& type, const OdRxObject* owner = NULL);
  friend class OdRxDictionaryPropertyImpl;
  OdRxDictionaryProperty(OdRxDictionaryPropertyImpl* pImpl);
};

#define ODRX_DECLARE_RX_PROPERTY_RO(Object, Name, Type) struct Object ## Name ## Property : OdRxProperty {\
  static OdRxMemberPtr createObject(const OdRxClass* owner)\
  {\
    OdRxMemberPtr res = OdRxObjectImpl<Object ## Name ## Property>::createObject();\
    ((Object ## Name ## Property*)res.get())->init(__OD_T(#Name), &OdRxValueType::Desc<Type>::value(), owner);\
    return res;\
  }\
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;\
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }\
}

#define ODRX_DECLARE_RX_PROPERTY(Object, Name, Type) struct Object ## Name ## Property : OdRxProperty {\
  static OdRxMemberPtr createObject(const OdRxClass* owner)\
  {\
    OdRxMemberPtr res = OdRxObjectImpl<Object ## Name ## Property>::createObject();\
    ((Object ## Name ## Property*)res.get())->init(__OD_T(#Name), &OdRxValueType::Desc<Type>::value(), owner);\
    return res;\
  }\
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;\
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;\
}

#define ODRX_DECLARE_RX_INDEXED_PROPERTY(Object, Name, Type) \
struct Object ## Name ## Property : OdRxIndexedProperty {\
  static OdRxMemberPtr createObject(const OdRxClass* owner)\
  {\
    OdRxMemberPtr res = OdRxObjectImpl<Object ## Name ## Property>::createObject();\
    ((Object ## Name ## Property*)res.get())->init(__OD_T(#Name), &OdRxValueType::Desc<Type>::value(), owner);\
    return res;\
  }\
  virtual OdResult subGetValue(const OdRxObject* pO, int index, OdRxValue& value) const ODRX_OVERRIDE; \
  virtual OdResult subSetValue(OdRxObject* pO, int index, const OdRxValue& value) const ODRX_OVERRIDE; \
  virtual OdResult subInsertValue(OdRxObject* pO, int index, const OdRxValue& value) const ODRX_OVERRIDE; \
  virtual OdResult subRemoveValue(OdRxObject* pO, int index) const ODRX_OVERRIDE; \
  virtual OdRxValueIteratorPtr subNewValueIterator(const OdRxObject* pO) const ODRX_OVERRIDE;\
  virtual OdResult subTryGetCount(const OdRxObject* pO, int& count) const ODRX_OVERRIDE;\
}

#define ODRX_DECLARE_RX_CPLX_INDEXED_PROPERTY(Object, Name, Type) \
struct Object ## Name ## Property : OdRxIndexedProperty {\
  static OdRxMemberPtr createObject(const OdRxClass* owner)\
  {\
    OdRxMemberPtr res = OdRxObjectImpl<Object ## Name ## Property>::createObject();\
    ((Object ## Name ## Property*)res.get())->init(__OD_T(#Name), &OdRxValueType::Desc<Type>::value(), owner);\
    return res;\
  }\
  virtual OdResult subGetValue(const OdRxObject* pO, int index, OdRxValue& value) const ODRX_OVERRIDE; \
  virtual OdResult subSetValue(OdRxObject* pO, int index, const OdRxValue& value) const ODRX_OVERRIDE; \
  virtual OdResult subInsertValue(OdRxObject* pO, int index, const OdRxValue& value) const ODRX_OVERRIDE; \
  virtual OdResult subRemoveValue(OdRxObject* pO, int index) const ODRX_OVERRIDE; \
  virtual OdRxValueIteratorPtr subNewValueIterator(const OdRxObject* pO) const ODRX_OVERRIDE;\
  virtual OdResult subTryGetCount(const OdRxObject* pO, int& count) const ODRX_OVERRIDE;\
  virtual OdResult subChildren(OdArray<OdRxMemberPtr>& children) const ODRX_OVERRIDE;\
}

#define ODRX_DECLARE_RX_PROPERTY_START(Object, Name, Type) struct Object ## Name ## Property : OdRxProperty {\
  static OdRxMemberPtr createObject(const OdRxClass* owner)\
  {\
    OdRxMemberPtr res = OdRxObjectImpl<Object ## Name ## Property>::createObject();\
    ((Object ## Name ## Property*)res.get())->init(__OD_T(#Name), &OdRxValueType::Desc<Type>::value(), owner);\
    return res;\
  }

#define ODRX_DECLARE_RX_PROPERTY_END }

#define ODRX_DECLARE_RX_PROPERTY_READONLY virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }

#define ODRX_DECLARE_RX_PROPERTY_GET_BY_VAL(Object, ObjGetMemberName, Type) \
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE\
  {\
    Object ## Ptr pObj = Object::cast(pO);\
    if (pObj.isNull()) return eNotApplicable;\
    value = static_cast<Type>(pObj->ObjGetMemberName());\
    return eOk;\
  }

#define ODRX_DECLARE_RX_PROPERTY_SET_BY_VAL(Object, ObjSetMemberName, Type) \
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE\
  {\
    Object ## Ptr pObj = Object::cast(pO);\
    if (pObj.isNull()) return eNotApplicable;\
    pObj->ObjSetMemberName(*rxvalue_cast<Type>(&value));\
    return eOk;\
  }

#define ODRX_DECLARE_RX_PROPERTY_SUBCHILDREN_START \
  virtual OdResult subChildren(OdArray<OdRxMemberPtr>& children) const {

#define ODRX_DECLARE_RX_PROPERTY_SUBCHILDREN_END return eOk; }

#define ODRX_DECLARE_RX_PROPERTY_SUBCHILDREN_ADD(Name, Type) \
  children.append(OdRxSubProperty::createObject(__OD_T(#Name), OdRxValueType::Desc<Type>::value(), this))

// Subproperty allows access to the individual components of a complex type

/**   
  <group OdRx_Classes>
*/
class FIRSTDLL_EXPORT OdRxSubProperty : public OdRxProperty
{
protected:
  OdRxSubProperty();
public:
  static OdRxMemberPtr createObject(const OdString& memberName, const OdRxValueType& type, const OdRxProperty* owner);
  virtual bool isReadOnly(const OdRxObject* pO) const ODRX_OVERRIDE;
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

/**   
  <group OdRx_Classes>
*/
class FIRSTDLL_EXPORT OdRxSubPropertyOfIndexedProperty : public OdRxIndexedProperty
{
protected:
	OdRxSubPropertyOfIndexedProperty();
public:
	static OdRxMemberPtr createObject(const OdString& memberName, const OdRxValueType& type, const OdRxIndexedProperty* owner);
	virtual OdResult subGetValue(const OdRxObject* pO, int index, OdRxValue& value) const ODRX_OVERRIDE;
	virtual OdResult subSetValue(OdRxObject* pO, int index, const OdRxValue& value) const ODRX_OVERRIDE;
};

/**   
  <group OdRx_Classes>
*/
class FIRSTDLL_EXPORT OdRxIndexedSubProperty : public OdRxIndexedProperty
{
protected:
	OdRxIndexedSubProperty();
public:
	static OdRxMemberPtr createObject(const OdString& memberName, const OdRxValueType& type, const OdRxProperty* owner);
	virtual OdResult subGetValue(const OdRxObject* pO, int index, OdRxValue& value) const ODRX_OVERRIDE;
	virtual OdResult subSetValue(OdRxObject* pO, int index, const OdRxValue& value) const ODRX_OVERRIDE;
	virtual OdResult subInsertValue(OdRxObject* pO, int index, const OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subRemoveValue(OdRxObject* pO, int index) const ODRX_OVERRIDE;
	virtual OdRxValueIteratorPtr subNewValueIterator(const OdRxObject* pO) const ODRX_OVERRIDE; 
	virtual OdResult subTryGetCount(const OdRxObject* pO, int& count) const ODRX_OVERRIDE;
};



#endif
