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


#ifndef _DGVALUETYPES_H_INCLUDED_
#define _DGVALUETYPES_H_INCLUDED_

#include "DbRootValueTypes.h"
#include "DgElementId.h"
#include "DgElement.h"
#include "DgGraphicsElement.h"
#include "RxValueType.h"
#include "DgHatchPattern.h"
#include "DgItemTypeLibrary.h"
#include "RxAttribute.h"

//-----------------------------------------------------------------------------------------

#undef  ODDG_DEFINE_RX_VALUE_ARRAY_ITERATOR
#define ODDG_DEFINE_RX_VALUE_ARRAY_ITERATOR(Type) \
struct OdRx ## Type ## ValueIterator : OdRxValueIterator \
{ \
  OdArray<Type> m_arrData;   \
  OdUInt32      m_uCurIndex; \
  OdRx ## Type ## ValueIterator() { m_uCurIndex = 0; } \
  virtual void init( const OdArray<Type>& arrData, OdUInt32 uStartIndex ) \
  { \
    m_arrData = arrData; \
    m_uCurIndex = uStartIndex; \
  } \
  virtual bool done() \
  { \
    if( m_arrData.isEmpty() )\
      return true;\
    return m_uCurIndex == m_arrData.size();\
  }\
  virtual bool next()\
  {\
    if( m_uCurIndex < m_arrData.size() )\
      m_uCurIndex++;\
    return !done();\
  }\
  virtual OdRxValue current() const\
  {\
    if( m_uCurIndex >= m_arrData.size() )\
      throw OdError(eInvalidIndex);\
    return m_arrData[m_uCurIndex];\
  }\
}

//-----------------------------------------------------------------------------------------

ODRX_DECLARE_VALUE_TYPE(OdDgElementId, DGENT_EXPORT);
ODRX_DECLARE_VALUE_TYPE(OdDgElement::ElementTypes, DGENT_EXPORT);
ODRX_DECLARE_VALUE_TYPE(OdDgGraphicsElement::Class, DGENT_EXPORT);
ODRX_DECLARE_VALUE_TYPE(OdDgGradientFill::OdDgGradientType, DGENT_EXPORT);
ODRX_DECLARE_VALUE_TYPE(OdDgGradientKey, DGENT_EXPORT);
ODRX_DECLARE_VALUE_TYPE(OdDgGradientFill, DGENT_EXPORT);
ODRX_DECLARE_VALUE_TYPE(DWGHatchLine, DGENT_EXPORT);
ODRX_DECLARE_VALUE_TYPE(OdDgHatchPattern::PatternType, DGENT_EXPORT);
ODRX_DECLARE_VALUE_TYPE(OdDgECPropertyInstanceFullPath, DGENT_EXPORT);

//-----------------------------------------------------------------------------------------

class OdDgRxDisplayAsAttribute;
typedef OdSmartPtr<OdDgRxDisplayAsAttribute> OdDgRxDisplayAsAttributePtr;

/** \details
    Corresponding C++ library: TG_Db
    <group OdDg_Classes>
*/
class TG_EXPORT OdDgRxDisplayAsAttribute : public OdRxDisplayAsAttribute
{
public:
  ODRX_DECLARE_MEMBERS(OdDgRxDisplayAsAttribute);
  static OdRxAttributePtr createObject(bool bSetIndex, bool bSetName);
  virtual OdString getDisplayValue(OdRxValue& value, bool useDynamicProperties);
  void setShowName(bool val);
  void setShowIndex(bool val);
private:
  bool m_bShowIndex;
  bool m_bShowName;
};

//-----------------------------------------------------------------------------------------

class OdDgRxHierarchyLevelAttribute;
typedef OdSmartPtr<OdDgRxHierarchyLevelAttribute> OdDgRxHierarchyLevelAttributePtr;

/** \details
    Corresponding C++ library: TG_Db
    <group OdDg_Classes>
*/
class TG_EXPORT OdDgRxHierarchyLevelAttribute : public OdRxHierarchyLevelAttribute
{
public:
  ODRX_DECLARE_MEMBERS(OdDgRxHierarchyLevelAttribute);
  virtual OdString value(OdRxValue& value);
};

//-----------------------------------------------------------------------------------------

#endif
