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
// Rendition interface property definition.

#ifndef ODTRVISPROPERTYDEF
#define ODTRVISPROPERTYDEF

#include "TD_PackPush.h"

#include "TrVisDefs.h"
#include "Ge/GeMatrix3d.h"

// Main property interface

/** \details
    <group ExRender_Windows_Classes> 
*/
struct OdTrVisPropertyDef
{
  enum PropertyType
  {
    kOwningProperty = 0,
    kXformProperty,
    kLayerProperty,
    kHlBranchProperty,
    kVsBranchProperty,
    kVisualStyleProperty,
    kSelStyleProperty,
    kNameProperty,
    kSysDefaultProperty,
    // Number of property types
    kNumPropTypes
  } m_propType;
  enum UnderlyingDataType
  {
    kBooleanProperty = 0, // Boolean
    kIdProperty,          // 64bit numeric
    kStringProperty,      // String
    kMatrixProperty,      // 4x4 doubles
    // Number of underlying data types
    kNumDataTypes
  };
  PropertyType propertyType() const { return m_propType; }
  UnderlyingDataType underlyingDataType() const
  {
    switch (m_propType)
    {
      case kOwningProperty:      return kIdProperty;
      case kXformProperty:       return kMatrixProperty;
      case kLayerProperty:       return kIdProperty;
      case kHlBranchProperty:    return kIdProperty;
      case kVsBranchProperty:    return kIdProperty;
      case kVisualStyleProperty: return kIdProperty;
      case kSelStyleProperty:    return kIdProperty;
      case kNameProperty:        return kStringProperty;
      case kSysDefaultProperty:  return kBooleanProperty;
      default:                   return kNumDataTypes;
    }
  }
  protected:
    explicit OdTrVisPropertyDef(PropertyType propType)
      : m_propType(propType) {}
};

// Property data types

/** \details
    <group ExRender_Windows_Classes> 
*/
struct OdTrVisBooleanPropertyDef : public OdTrVisPropertyDef
{
  bool m_boolVal;
  bool propertyValue() const { return m_boolVal; }
  protected:
    OdTrVisBooleanPropertyDef(PropertyType propType, bool bVal)
      : OdTrVisPropertyDef(propType), m_boolVal(bVal) {}
};

/** \details
    <group ExRender_Windows_Classes> 
*/
struct OdTrVisIdPropertyDef : public OdTrVisPropertyDef
{
  OdTrVisId m_idVal;
  OdTrVisId propertyValue() const { return m_idVal; }
  protected:
    OdTrVisIdPropertyDef(PropertyType propType, OdTrVisId idVal)
      : OdTrVisPropertyDef(propType), m_idVal(idVal) {}
};

/** \details
    <group ExRender_Windows_Classes> 
*/
struct OdTrVisStringPropertyDef : public OdTrVisPropertyDef
{
  OdString m_charVal;
  const OdString &propertyValue() const { return m_charVal; }
  protected:
    OdTrVisStringPropertyDef(PropertyType propType, const OdString &charVal)
      : OdTrVisPropertyDef(propType), m_charVal(charVal) {}
};

/** \details
    <group ExRender_Windows_Classes> 
*/
struct OdTrVisMatrixPropertyDef : public OdTrVisPropertyDef
{
  OdGeMatrix3d m_mtxVal;
  const OdGeMatrix3d &propertyValue() const { return m_mtxVal; }
  protected:
    OdTrVisMatrixPropertyDef(PropertyType propType, const OdGeMatrix3d &mtxVal)
      : OdTrVisPropertyDef(propType), m_mtxVal(mtxVal) {}
};

// Final property interfaces

/** \details
    <group ExRender_Windows_Classes> 
*/
struct OdTrVisOwningPropertyDef : public OdTrVisIdPropertyDef
{
  OdTrVisOwningPropertyDef(OdTrVisOwning owningPropVal)
    : OdTrVisIdPropertyDef(kOwningProperty, (OdTrVisId)owningPropVal) {}
  OdTrVisOwning propertyValue() const { return (OdTrVisOwning)OdTrVisIdPropertyDef::propertyValue(); }
};

/** \details
    <group ExRender_Windows_Classes> 
*/
struct OdTrVisXformPropertyDef : public OdTrVisMatrixPropertyDef
{
  OdTrVisXformPropertyDef(const OdGeMatrix3d &xformMatrix)
    : OdTrVisMatrixPropertyDef(kXformProperty, xformMatrix) {}
};

/** \details
    <group ExRender_Windows_Classes> 
*/
struct OdTrVisLayerPropertyDef : public OdTrVisIdPropertyDef
{
  OdTrVisLayerPropertyDef(OdTrVisLayerId layerId)
    : OdTrVisIdPropertyDef(kLayerProperty, (OdTrVisId)layerId) {}
  OdTrVisLayerId propertyValue() const { return (OdTrVisLayerId)OdTrVisIdPropertyDef::propertyValue(); }
};

/** \details
  <group ExRender_Windows_Classes>
  Property definition for state branches (highlighting or visibility).
*/
struct OdTrVisStateBranchPropertyDef : public OdTrVisIdPropertyDef
{
  enum BranchType { kHl, kVs, kNumBranches };
  static PropertyType branchTypeToPropertyType(BranchType branchType)
  { return (branchType == kHl) ? kHlBranchProperty : kVsBranchProperty; }
  OdTrVisStateBranchPropertyDef(OdTrVisStateBranchId branchId, BranchType branchType = kHl)
    : OdTrVisIdPropertyDef(branchTypeToPropertyType(branchType), (OdTrVisId)branchId) {}
  /** \details
    Retrives property value.
    \returns Returns property value.
  */
  OdTrVisStateBranchId propertyValue() const { return (OdTrVisStateBranchId)OdTrVisIdPropertyDef::propertyValue(); }
};

/** \details
    <group ExRender_Windows_Classes> 
*/
struct OdTrVisVisualStylePropertyDef : public OdTrVisIdPropertyDef
{
  OdTrVisVisualStylePropertyDef(OdTrVisVisualStyleId visualStyleId)
    : OdTrVisIdPropertyDef(kVisualStyleProperty, (OdTrVisId)visualStyleId) {}
  OdTrVisVisualStyleId propertyValue() const { return (OdTrVisVisualStyleId)OdTrVisIdPropertyDef::propertyValue(); }
};

/** \details
    <group ExRender_Windows_Classes> 
*/
struct OdTrVisSelStylePropertyDef : public OdTrVisIdPropertyDef
{
  OdTrVisSelStylePropertyDef(OdUInt32 selStyleIndex)
    : OdTrVisIdPropertyDef(kSelStyleProperty, (OdTrVisId)selStyleIndex) {}
  OdUInt32 propertyValue() const { return (OdUInt32)OdTrVisIdPropertyDef::propertyValue(); }
};

/** \details
    <group ExRender_Windows_Classes> 
*/
struct OdTrVisNamePropertyDef : public OdTrVisStringPropertyDef
{
  OdTrVisNamePropertyDef(const OdString &nameProp)
    : OdTrVisStringPropertyDef(kNameProperty, nameProp) {}
};

/** \details
    <group ExRender_Windows_Classes> 
*/
struct OdTrVisSysDefaultPropertyDef : public OdTrVisBooleanPropertyDef
{
  OdTrVisSysDefaultPropertyDef(bool bSystemDefault = true)
    : OdTrVisBooleanPropertyDef(kSysDefaultProperty, bSystemDefault) {}
};

/** \details
    <group ExRender_Windows_Classes> 
*/
template <typename BasicType>
class OdTrVisPropertyChangedMonitor
{
  protected:
    BasicType m_value;
    bool m_bChanged;
  public:
    OdTrVisPropertyChangedMonitor(const BasicType &initialState)
      : m_value(initialState), m_bChanged(false) { }
    void resetValue(const BasicType &newValue, bool bSetChanged = true)
    {
      if (bSetChanged)
      {
        if (m_value != newValue)
        {
          m_value = newValue;
          m_bChanged = true;
        }
        else
          m_bChanged = false;
      }
      else
      {
        m_value = newValue;
        m_bChanged = false;
      }
    }
    bool isChanged() const { return m_bChanged; }
    void setNoChange() { m_bChanged = false; }
    const BasicType &value() const { return m_value; }
};

#include "TD_PackPop.h"

#endif // ODTRVISPROPERTYDEF
