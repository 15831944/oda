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
#ifndef __DGPROPS__H__
#define __DGPROPS__H__

#include "OdaCommon.h"
#include "DgValueTypes.h"
#include "RxProperty.h"
#include "RxDynamicModule.h"
#include "RxSysRegistry.h"
#include "RxObjectImpl.h"
#include "RxValueTypeUtil.h"
#include "RxAttribute.h"
#include "DgElement.h"
#include "RxCategory.h"
#include "DynamicLinker.h"
#include "DgElement.h"
#include "DgElementIterator.h"

//----------------------------------------------------------------------------------------------------------------------
/** \details
  Corresponding C++ library: TG_Db 
  <group OdDg_Classes>
*/
struct OdDgElementIdValueIterator : OdRxValueIterator
{
  OdDgElementIteratorPtr m_pIter;
  bool                   m_bAllowNull;

  virtual bool done() 
  { 
    if( m_pIter.isNull() )
      return true;
    return m_pIter->done(); 
  }
  virtual bool next()
  {
    if( m_bAllowNull )
    {
      m_bAllowNull = false;
      return true;
    }
    else if( m_pIter.isNull() )
      return false;
    else if(m_pIter->done())
      return false;
    m_pIter->step();
      return true;
  }
  virtual OdRxValue current() const
  {
    if( m_bAllowNull )
      return OdDgElementId();
    else
      return m_pIter->item();
  }
};

//----------------------------------------------------------------------------------------------------------------------

#undef ODRX_DECLARE_RX_PROPERTY_RO
#define ODRX_DECLARE_RX_PROPERTY_RO(Object, Name, Type, Category, Weight, Description) \
struct Object ## Name ## Property : OdRxProperty {\
  static OdRxMemberPtr createObject(const OdRxClass* owner)\
  {\
    OdRxMemberPtr res = OdRxObjectImpl<Object ## Name ## Property>::createObject();\
    ((Object ## Name ## Property*)res.get())->init(__OD_T(#Name), &OdRxValueType::Desc<Type>::value(), owner);\
    if( OdString::kEmpty != Description ) \
      ((Object ## Name ## Property*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(OD_T(Description))); \
    if( OdString::kEmpty != Category ) \
      ((Object ## Name ## Property*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(OD_T(Category), Weight)); \
    return res;\
  }\
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;\
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }\
}

//----------------------------------------------------------------------------------------------------------------------

#undef ODRX_DECLARE_RX_CPLX_PROPERTY_RO
#define ODRX_DECLARE_RX_CPLX_PROPERTY_RO(Object, Name, Type, Category, Weight, Description) \
struct Object ## Name ## Property : OdRxProperty {\
  static OdRxMemberPtr createObject(const OdRxClass* owner)\
  {\
    OdRxMemberPtr res = OdRxObjectImpl<Object ## Name ## Property>::createObject();\
    ((Object ## Name ## Property*)res.get())->init(__OD_T(#Name), &OdRxValueType::Desc<Type>::value(), owner);\
    if( OdString::kEmpty != Description ) \
      ((Object ## Name ## Property*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(OD_T(Description))); \
    if( OdString::kEmpty != Category ) \
      ((Object ## Name ## Property*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(OD_T(Category), Weight)); \
    return res;\
  }\
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;\
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }\
  virtual OdResult subChildren(OdArray<OdRxMemberPtr>& children) const; \
}

//----------------------------------------------------------------------------------------------------------------------

#undef ODRX_DECLARE_RX_PROPERTY
#define ODRX_DECLARE_RX_PROPERTY(Object, Name, Type, Category, Weight, Description, CollectionPath) \
struct Object ## Name ## Property : OdRxProperty {\
  static OdRxMemberPtr createObject(const OdRxClass* owner)\
  {\
    OdRxMemberPtr res = OdRxObjectImpl<Object ## Name ## Property>::createObject();\
    ((Object ## Name ## Property*)res.get())->init(__OD_T(#Name), &OdRxValueType::Desc<Type>::value(), owner);\
    if( OdString::kEmpty != Description ) \
      ((Object ## Name ## Property*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(OD_T(Description))); \
    if( OdString::kEmpty != Category ) \
      ((Object ## Name ## Property*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(OD_T(Category), Weight)); \
    if( OdString::kEmpty != CollectionPath ) \
      ((Object ## Name ## Property*)res.get())->attributes().add(OdRxRefersToAttribute::createObject(OD_T(CollectionPath))); \
    return res;\
  }\
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;\
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;\
}

//----------------------------------------------------------------------------------------------------------------------

#undef ODRX_DECLARE_RX_PROPERTY_TREE
#define ODRX_DECLARE_RX_PROPERTY_TREE(Object, Name, Type, Category, Weight, Description, CollectionPath, HierarchyLevel) \
struct Object ## Name ## Property : OdRxProperty {\
  static OdRxMemberPtr createObject(const OdRxClass* owner)\
  {\
    OdRxMemberPtr res = OdRxObjectImpl<Object ## Name ## Property>::createObject();\
    ((Object ## Name ## Property*)res.get())->init(__OD_T(#Name), &OdRxValueType::Desc<Type>::value(), owner);\
    ((Object ## Name ## Property*)res.get())->attributes().add(OdDgRxDisplayAsAttribute::createObject(false, true)); \
    if( OdString::kEmpty != Description ) \
      ((Object ## Name ## Property*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(OD_T(Description))); \
    if( OdString::kEmpty != Category ) \
      ((Object ## Name ## Property*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(OD_T(Category), Weight)); \
    if( OdString::kEmpty != CollectionPath ) \
      ((Object ## Name ## Property*)res.get())->attributes().add(OdRxRefersToAttribute::createObject(OD_T(CollectionPath))); \
    if( OdString::kEmpty != HierarchyLevel ) \
      ((Object ## Name ## Property*)res.get())->attributes().add(OdDgRxHierarchyLevelAttribute::createObject()); \
    return res;\
  }\
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;\
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;\
}

//----------------------------------------------------------------------------------------------------------------------

#undef ODRX_DECLARE_RX_PROPERTY_TREE_RO
#define ODRX_DECLARE_RX_PROPERTY_TREE_RO(Object, Name, Type, Category, Weight, Description, CollectionPath, HierarchyLevel) \
struct Object ## Name ## Property : OdRxProperty {\
  static OdRxMemberPtr createObject(const OdRxClass* owner)\
  {\
    OdRxMemberPtr res = OdRxObjectImpl<Object ## Name ## Property>::createObject();\
    ((Object ## Name ## Property*)res.get())->init(__OD_T(#Name), &OdRxValueType::Desc<Type>::value(), owner);\
    ((Object ## Name ## Property*)res.get())->attributes().add(OdDgRxDisplayAsAttribute::createObject(false, true)); \
    if( OdString::kEmpty != Description ) \
      ((Object ## Name ## Property*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(OD_T(Description))); \
    if( OdString::kEmpty != Category ) \
      ((Object ## Name ## Property*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(OD_T(Category), Weight)); \
    if( OdString::kEmpty != CollectionPath ) \
      ((Object ## Name ## Property*)res.get())->attributes().add(OdRxRefersToAttribute::createObject(OD_T(CollectionPath))); \
    if( OdString::kEmpty != HierarchyLevel ) \
      ((Object ## Name ## Property*)res.get())->attributes().add(OdDgRxHierarchyLevelAttribute::createObject()); \
    return res;\
  }\
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;\
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }\
}

//----------------------------------------------------------------------------------------------------------------------

#undef ODRX_DECLARE_RX_CPLX_PROPERTY
#define ODRX_DECLARE_RX_CPLX_PROPERTY(Object, Name, Type, Category, Weight, Description, CollectionPath) \
struct Object ## Name ## Property : OdRxProperty {\
  static OdRxMemberPtr createObject(const OdRxClass* owner)\
  {\
    OdRxMemberPtr res = OdRxObjectImpl<Object ## Name ## Property>::createObject();\
    ((Object ## Name ## Property*)res.get())->init(__OD_T(#Name), &OdRxValueType::Desc<Type>::value(), owner);\
    if( OdString::kEmpty != Description ) \
      ((Object ## Name ## Property*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(OD_T(Description))); \
    if( OdString::kEmpty != Category ) \
      ((Object ## Name ## Property*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(OD_T(Category), Weight)); \
    if( OdString::kEmpty != CollectionPath ) \
      ((Object ## Name ## Property*)res.get())->attributes().add(OdRxRefersToAttribute::createObject(OD_T(CollectionPath))); \
    return res;\
  }\
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;\
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;\
  virtual OdResult subChildren(OdArray<OdRxMemberPtr>& children) const; \
}


//----------------------------------------------------------------------------------------------------------------------

#undef ODRX_DECLARE_RX_PROPERTY_UNIONTYPE
#define ODRX_DECLARE_RX_PROPERTY_UNIONTYPE(Object, Name, Type, Category, Weight, Description, CollectionPath, UnionType) \
struct Object ## Name ## Property : OdRxProperty {\
  static OdRxMemberPtr createObject(const OdRxClass* owner)\
  {\
    OdRxMemberPtr res = OdRxObjectImpl<Object ## Name ## Property>::createObject();\
    ((Object ## Name ## Property*)res.get())->init(__OD_T(#Name), &OdRxValueType::Desc<Type>::value(), owner);\
    if( OdString::kEmpty != Description ) \
      ((Object ## Name ## Property*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(OD_T(Description))); \
    if( OdString::kEmpty != Category ) \
      ((Object ## Name ## Property*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(OD_T(Category), Weight)); \
    if( OdString::kEmpty != CollectionPath ) \
      ((Object ## Name ## Property*)res.get())->attributes().add(OdRxRefersToAttribute::createObject(OD_T(CollectionPath))); \
    if( OdString::kEmpty != UnionType ) \
      ((Object ## Name ## Property*)res.get())->attributes().add(OdRxUnionTypeAttribute::createObject(OD_T(UnionType))); \
    return res;\
  }\
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;\
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;\
}

//----------------------------------------------------------------------------------------------------------------------

#undef ODRX_DECLARE_RX_PROPERTY_ANGLE
#define ODRX_DECLARE_RX_PROPERTY_ANGLE(Object, Name, Type, Category, Weight, Description, CollectionPath) \
struct Object ## Name ## Property : OdRxProperty {\
  static OdRxMemberPtr createObject(const OdRxClass* owner)\
  {\
    OdRxMemberPtr res = OdRxObjectImpl<Object ## Name ## Property>::createObject();\
    ((Object ## Name ## Property*)res.get())->init(__OD_T(#Name), &OdRxValueType::Desc<Type>::value(), owner);\
    if( OdString::kEmpty != Description ) \
      ((Object ## Name ## Property*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(OD_T(Description))); \
    if( OdString::kEmpty != Category ) \
      ((Object ## Name ## Property*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(OD_T(Category), Weight)); \
    if( OdString::kEmpty != CollectionPath ) \
      ((Object ## Name ## Property*)res.get())->attributes().add(OdRxRefersToAttribute::createObject(OD_T(CollectionPath))); \
    ((Object ## Name ## Property*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(OdRxUnitTypeAttribute::kAngle)); \
    return res;\
  }\
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;\
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;\
}

//----------------------------------------------------------------------------------------------------------------------

#undef ODRX_DECLARE_RX_PROPERTY_COLLECTION
#define ODRX_DECLARE_RX_PROPERTY_COLLECTION(Object, Name, Type, Category, Weight, Description) \
struct Object ## Name ## Property : OdRxCollectionProperty {\
  static OdRxMemberPtr createObject(const OdRxClass* owner)\
  {\
    OdRxMemberPtr res = OdRxObjectImpl<Object ## Name ## Property>::createObject();\
    ((Object ## Name ## Property*)res.get())->init(__OD_T(#Name), &OdRxValueType::Desc<Type>::value(), owner);\
    if( OdString::kEmpty != Description ) \
      ((Object ## Name ## Property*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(OD_T(Description))); \
    if( OdString::kEmpty != Category ) \
      ((Object ## Name ## Property*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(OD_T(Category), Weight)); \
    return res;\
  }\
  virtual OdRxValueIteratorPtr subNewValueIterator(const OdRxObject* pO) const ODRX_OVERRIDE;\
  virtual OdResult subTryGetCount(const OdRxObject* pO, int& count) const ODRX_OVERRIDE;\
}

//----------------------------------------------------------------------------------------------------------------------

#undef ODRX_DECLARE_RX_PROPERTY_COLLECTION_NAMED
#define ODRX_DECLARE_RX_PROPERTY_COLLECTION_NAMED(Object, Name, Type, Category, Weight, Description, ShowIndex) \
struct Object ## Name ## Property : OdRxCollectionProperty {\
  static OdRxMemberPtr createObject(const OdRxClass* owner)\
  {\
    OdRxMemberPtr res = OdRxObjectImpl<Object ## Name ## Property>::createObject();\
    ((Object ## Name ## Property*)res.get())->init(__OD_T(#Name), &OdRxValueType::Desc<Type>::value(), owner);\
    ((Object ## Name ## Property*)res.get())->attributes().add(OdDgRxDisplayAsAttribute::createObject(ShowIndex, true)); \
    if( OdString::kEmpty != Description ) \
      ((Object ## Name ## Property*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(OD_T(Description))); \
    if( OdString::kEmpty != Category ) \
      ((Object ## Name ## Property*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(OD_T(Category), Weight)); \
    return res;\
  }\
  virtual OdRxValueIteratorPtr subNewValueIterator(const OdRxObject* pO) const ODRX_OVERRIDE;\
  virtual OdResult subTryGetCount(const OdRxObject* pO, int& count) const ODRX_OVERRIDE;\
}

//----------------------------------------------------------------------------------------------------------------------

#undef ODRX_DECLARE_RX_PROPERTY_COLLECTION_TREE
#define ODRX_DECLARE_RX_PROPERTY_COLLECTION_TREE(Object, Name, Type, Category, Weight, Description, HierarchyLevel) \
struct Object ## Name ## Property : OdRxCollectionProperty {\
  static OdRxMemberPtr createObject(const OdRxClass* owner)\
  {\
    OdRxMemberPtr res = OdRxObjectImpl<Object ## Name ## Property>::createObject();\
    ((Object ## Name ## Property*)res.get())->init(__OD_T(#Name), &OdRxValueType::Desc<Type>::value(), owner);\
    ((Object ## Name ## Property*)res.get())->attributes().add(OdDgRxDisplayAsAttribute::createObject(false, true)); \
    if( OdString::kEmpty != Description ) \
      ((Object ## Name ## Property*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(OD_T(Description))); \
    if( OdString::kEmpty != Category ) \
      ((Object ## Name ## Property*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(OD_T(Category), Weight)); \
    if( OdString::kEmpty != HierarchyLevel ) \
      ((Object ## Name ## Property*)res.get())->attributes().add(OdDgRxHierarchyLevelAttribute::createObject()); \
    return res;\
  }\
  virtual OdRxValueIteratorPtr subNewValueIterator(const OdRxObject* pO) const ODRX_OVERRIDE;\
  virtual OdResult subTryGetCount(const OdRxObject* pO, int& count) const ODRX_OVERRIDE;\
}

//----------------------------------------------------------------------------------------------------------------------

#undef ODRX_DECLARE_RX_PROPERTY_INDEXED
#define ODRX_DECLARE_RX_PROPERTY_INDEXED(Object, Name, Type, Category, Weight, Description) \
struct Object ## Name ## Property : OdRxIndexedProperty {\
  static OdRxMemberPtr createObject(const OdRxClass* owner)\
  {\
    OdRxMemberPtr res = OdRxObjectImpl<Object ## Name ## Property>::createObject();\
    ((Object ## Name ## Property*)res.get())->init(__OD_T(#Name), &OdRxValueType::Desc<Type>::value(), owner);\
    if( OdString::kEmpty != Description ) \
      ((Object ## Name ## Property*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(OD_T(Description))); \
    if( OdString::kEmpty != Category ) \
      ((Object ## Name ## Property*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(OD_T(Category), Weight)); \
    return res;\
  }\
  virtual OdResult subGetValue(const OdRxObject* pO, int index, OdRxValue& value) const ODRX_OVERRIDE; \
  virtual OdResult subSetValue(OdRxObject* pO, int index, const OdRxValue& value) const ODRX_OVERRIDE; \
  virtual OdResult subInsertValue(OdRxObject* pO, int index, const OdRxValue& value) const ODRX_OVERRIDE; \
  virtual OdResult subRemoveValue(OdRxObject* pO, int index) const ODRX_OVERRIDE; \
  virtual OdRxValueIteratorPtr subNewValueIterator(const OdRxObject* pO) const ODRX_OVERRIDE;\
  virtual OdResult subTryGetCount(const OdRxObject* pO, int& count) const ODRX_OVERRIDE;\
}

//----------------------------------------------------------------------------------------------------------------------

#undef ODRX_DECLARE_RX_CPLX_PROPERTY_INDEXED
#define ODRX_DECLARE_RX_CPLX_PROPERTY_INDEXED(Object, Name, Type, Category, Weight, Description) \
struct Object ## Name ## Property : OdRxIndexedProperty {\
  static OdRxMemberPtr createObject(const OdRxClass* owner)\
  {\
    OdRxMemberPtr res = OdRxObjectImpl<Object ## Name ## Property>::createObject();\
    ((Object ## Name ## Property*)res.get())->init(__OD_T(#Name), &OdRxValueType::Desc<Type>::value(), owner);\
    if( OdString::kEmpty != Description ) \
      ((Object ## Name ## Property*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(OD_T(Description))); \
    if( OdString::kEmpty != Category ) \
      ((Object ## Name ## Property*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(OD_T(Category), Weight)); \
    return res;\
  }\
  virtual OdResult subGetValue(const OdRxObject* pO, int index, OdRxValue& value) const ODRX_OVERRIDE; \
  virtual OdResult subSetValue(OdRxObject* pO, int index, const OdRxValue& value) const ODRX_OVERRIDE; \
  virtual OdResult subInsertValue(OdRxObject* pO, int index, const OdRxValue& value) const ODRX_OVERRIDE; \
  virtual OdResult subRemoveValue(OdRxObject* pO, int index) const ODRX_OVERRIDE; \
  virtual OdRxValueIteratorPtr subNewValueIterator(const OdRxObject* pO) const ODRX_OVERRIDE;\
  virtual OdResult subTryGetCount(const OdRxObject* pO, int& count) const ODRX_OVERRIDE;\
  virtual OdResult subChildren(OdArray<OdRxMemberPtr>& children) const; \
}

//----------------------------------------------------------------------------------------------------------------------

#endif // __DGELEMENTPROPS__H__
