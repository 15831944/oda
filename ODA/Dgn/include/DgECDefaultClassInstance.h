/////////////////////////////////////////////////////////////////////////////// 
// Copyright (C) 2002-2020, Open Design Alliance (the "Alliance"). 
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
//   Open Design Alliance Copyright (C) 2002-2020 by Open Design Alliance. 
//   All rights reserved.
//
// By use of this software, its documentation or related materials, you 
// acknowledge and accept the above terms.
///////////////////////////////////////////////////////////////////////////////
#ifndef __DG_ECDEFAULTCLASSINSTANCE_H__
#define __DG_ECDEFAULTCLASSINSTANCE_H__

#include "TD_PackPush.h"

#include "DgECSchema.h"


//=============================================================================== 
//                EditorCustomAttributes
//=============================================================================== 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: EditorCustomAttributes, Version: 1.3, Class: AlwaysExpand
    Available properties: "ArrayMembers"(bool)
    Description: Indicates if a property should always be expanded on ECPropertyPane
*/
class TG_EXPORT OdDgAlwaysExpandClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgAlwaysExpandClassInstance);

  virtual bool getArrayMembersFlag() const = 0;
  virtual void setArrayMembersFlag(bool bValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgAlwaysExpandClassInstance> OdDgAlwaysExpandClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: EditorCustomAttributes, Version: 1.3, Class: ArrayBehaviorAttributes
    Available properties: "OneElementAsArray"(bool), "ShowEmpty"(bool), "SupportsAddAndRemove"(bool)
    Description: Specifies how UI should handle the display of array properties
*/
class TG_EXPORT OdDgArrayBehaviorAttributesClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgArrayBehaviorAttributesClassInstance);

  virtual bool getOneElementAsArrayFlag() const = 0;
  virtual void setOneElementAsArrayFlag(bool bValue) = 0;

  virtual bool getShowEmptyFlag() const = 0;
  virtual void setShowEmptyFlag(bool bValue) = 0;

  virtual bool getSupportsAddAndRemoveFlag() const = 0;
  virtual void setSupportsAddAndRemoveFlag(bool bValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgArrayBehaviorAttributesClassInstance> OdDgArrayBehaviorAttributesClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: EditorCustomAttributes, Version: 1.3, Class: ArrayMemberNameFromValue
    Available properties: ---
*/
class TG_EXPORT OdDgArrayMemberNameFromValueClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgArrayMemberNameFromValueClassInstance);

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgArrayMemberNameFromValueClassInstance> OdDgArrayMemberNameFromValueClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: EditorCustomAttributes, Version: 1.3, Class: Localizable
    Available properties: ---
    Description: Mark a string type property of a custom attribute as localizable
*/
class TG_EXPORT OdDgLocalizableClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgLocalizableClassInstance);

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgLocalizableClassInstance> OdDgLocalizableClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: EditorCustomAttributes, Version: 1.3, Class: BooleanDisplay
    Available properties: "TrueString"(string), "FalseString"(string)
    Description: Specifies custom strings for True and False
*/
class TG_EXPORT OdDgBooleanDisplayClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgBooleanDisplayClassInstance);

  virtual OdString getTrueString() const = 0;
  virtual void setTrueString(const OdString& strValue) = 0;

  virtual OdString getFalseString() const = 0;
  virtual void setFalseString(const OdString& strValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgBooleanDisplayClassInstance> OdDgBooleanDisplayClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: EditorCustomAttributes, Version: 1.3, Class: Category
    Available properties: "Standard"(integer), "Name"(string), "DisplayLabel"(string), "Description"(string), "Priority"(integer), "Expand"(bool)
    Description: Specifies category information to be used by ECPropertyPane
*/
class TG_EXPORT OdDgCategoryClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgCategoryClassInstance);

  virtual OdInt32 getStandard() const = 0;
  virtual void setStandard(OdInt32 iValue) = 0;

  virtual OdString getCategoryName() const = 0;
  virtual void setCategoryName(const OdString& strValue) = 0;

  virtual OdString getCategoryDisplayLabel() const = 0;
  virtual void setCategoryDisplayLabel(const OdString& strValue) = 0;

  virtual OdString getCategoryDescription() const = 0;
  virtual void setCategoryDescription(const OdString& strValue) = 0;

  virtual OdInt32 getPriority() const = 0;
  virtual void setPriority(OdInt32 iValue) = 0;

  virtual bool getExpandFlag() const = 0;
  virtual void setExpandFlag(bool bValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgCategoryClassInstance> OdDgCategoryClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: EditorCustomAttributes, Version: 1.3, Class: StringLength
    Available properties: "Min"(long), "Max"(long)
    Description: Specifies minimum and maxmimum lengths for a string property
*/
class TG_EXPORT OdDgStringLengthClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgStringLengthClassInstance);

  virtual OdInt64 getMin() const = 0;
  virtual void setMin(OdInt64 iValue) = 0;

  virtual OdInt64 getMax() const = 0;
  virtual void setMax(OdInt64 iValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgStringLengthClassInstance> OdDgStringLengthClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: EditorCustomAttributes, Version: 1.3, Class: ClassPriority
    Available properties: "Priority"(integer)
*/
class TG_EXPORT OdDgClassPriorityClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgClassPriorityClassInstance);

  virtual OdInt32 getPriority() const = 0;
  virtual void setPriority(OdInt32 iValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgClassPriorityClassInstance> OdDgClassPriorityClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: EditorCustomAttributes, Version: 1.3, Class: DontLoadProperty
    Available properties: ---
    Description: Indicates that this property is expensive to calculate and it should not be loaded by default
*/
class TG_EXPORT OdDgDontLoadPropertyClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgDontLoadPropertyClassInstance);

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgDontLoadPropertyClassInstance> OdDgDontLoadPropertyClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: EditorCustomAttributes, Version: 1.3, Class: DontShowNullProperties
    Available properties: ---
    Description: Indicates if null properties should be displayed on ECPropertyPane
*/
class TG_EXPORT OdDgDontShowNullPropertiesClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgDontShowNullPropertiesClassInstance);

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgDontShowNullPropertiesClassInstance> OdDgDontShowNullPropertiesClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: EditorCustomAttributes, Version: 1.3, Class: DontShowNullProperty
    Available properties: ---
*/
class TG_EXPORT OdDgDontShowNullPropertyClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgDontShowNullPropertyClassInstance);

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgDontShowNullPropertyClassInstance> OdDgDontShowNullPropertyClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: EditorCustomAttributes, Version: 1.3, Class: DWGFormat
    Available properties: "FormatString"(string)
    Description: Specifies the AutoCAD format string to use when creating a new value from this property
*/
class TG_EXPORT OdDgDWGFormatClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgDWGFormatClassInstance);

  virtual OdString getFormatString() const = 0;
  virtual void setFormatString(const OdString& strValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgDWGFormatClassInstance> OdDgDWGFormatClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: EditorCustomAttributes, Version: 1.3, Class: DWGProperty
    Available properties: "Name"(string)
    Description: Specifies the AutoCAD property name
*/
class TG_EXPORT OdDgDWGPropertyClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgDWGPropertyClassInstance);

  virtual OdString getDWGPropertyName() const = 0;
  virtual void setDWGPropertyName(const OdString& strValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgDWGPropertyClassInstance> OdDgDWGPropertyClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: EditorCustomAttributes, Version: 1.3, Class: ExtendType
    Available properties: "Standard"(integer), "Name"(string)
    Description: Specifies the extended type for a property
*/
class TG_EXPORT OdDgExtendTypeClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgExtendTypeClassInstance);

  virtual OdInt32 getStandard() const = 0;
  virtual void setStandard(OdInt32 iValue) = 0;

  virtual OdString getExtendTypeName() const = 0;
  virtual void setExtendTypeName(const OdString& strValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgExtendTypeClassInstance> OdDgExtendTypeClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: EditorCustomAttributes, Version: 1.3, Class: FilePickerAttributes
    Available properties: "Title"(string), "FilterList"(string), "FilterIndex"(integer), "InitialDirectory"(string), "MultiSelect"(bool), "UseSaveFile"(bool), "CreatePrompt"(bool), "OverwritePrompt"(bool)
    Description: Specifies file dialog settings
*/
class TG_EXPORT OdDgFilePickerAttributesClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgFilePickerAttributesClassInstance);

  virtual OdString getTitle() const = 0;
  virtual void setTitle(const OdString& strValue) = 0;

  virtual OdString getFilterList() const = 0;
  virtual void setFilterList(const OdString& strValue) = 0;

  virtual OdInt32 getFilterIndex() const = 0;
  virtual void setFilterIndex(OdInt32 iValue) = 0;

  virtual OdString getInitialDirectory() const = 0;
  virtual void setInitialDirectory(const OdString& strValue) = 0;

  virtual bool getMultiSelectFlag() const = 0;
  virtual void setMultiSelectFlag(bool bValue) = 0;

  virtual bool getUseSaveFileFlag() const = 0;
  virtual void setUseSaveFileFlag(bool bValue) = 0;

  virtual bool getCreatePromptFlag() const = 0;
  virtual void setCreatePromptFlag(bool bValue) = 0;

  virtual bool getOverwritePromptFlag() const = 0;
  virtual void setOverwritePromptFlag(bool bValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgFilePickerAttributesClassInstance> OdDgFilePickerAttributesClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: EditorCustomAttributes, Version: 1.3, Class: Format
    Available properties: "FormatString"(string)
    Description: Specifies the format string to be used by UI to format values of an ECProperty
*/
class TG_EXPORT OdDgFormatClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgFormatClassInstance);

  virtual OdString getFormatString() const = 0;
  virtual void setFormatString(const OdString& strValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgFormatClassInstance> OdDgFormatClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: EditorCustomAttributes, Version: 1.3, Class: HideMembers
    Available properties: "ArrayMembers"(bool), "If2D"(bool), "If3D"(bool)
    Description: Indicates if members of a property should be shown on ECPropertyPane
*/
class TG_EXPORT OdDgHideMembersClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgHideMembersClassInstance);

  virtual bool getArrayMembersFlag() const = 0;
  virtual void setArrayMembersFlag(bool bValue) = 0;

  virtual bool getIf2DFlag() const = 0;
  virtual void setIf2DFlag(bool bValue) = 0;

  virtual bool getIf3DFlag() const = 0;
  virtual void setIf3DFlag(bool bValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgHideMembersClassInstance> OdDgHideMembersClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: EditorCustomAttributes, Version: 1.3, Class: HideProperty
    Available properties: "If2D"(bool), "If3D"(bool), "If"(string)
    Description: Indicates if this property should be hidden from the user
*/
class TG_EXPORT OdDgHidePropertyClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgHidePropertyClassInstance);

  virtual bool getIf2DFlag() const = 0;
  virtual void setIf2DFlag(bool bValue) = 0;

  virtual bool getIf3DFlag() const = 0;
  virtual void setIf3DFlag(bool bValue) = 0;

  virtual OdString getIf() const = 0;
  virtual void setIf(const OdString& strValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgHidePropertyClassInstance> OdDgHidePropertyClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: EditorCustomAttributes, Version: 1.3, Class: IgnoreZ
    Available properties: ---
    Description: Indicates if the Z value of a DPoint3D property should not be displayed in the property pane
*/
class TG_EXPORT OdDgIgnoreZClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgIgnoreZClassInstance);

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgIgnoreZClassInstance> OdDgIgnoreZClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: EditorCustomAttributes, Version: 1.3, Class: ItemTypeSelectorOptIn
    Available properties: "Hidden"(bool)
*/
class TG_EXPORT OdDgItemTypeSelectorOptInClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgItemTypeSelectorOptInClassInstance);

  virtual bool getHiddenFlag() const = 0;
  virtual void setHiddenFlag(bool bValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgItemTypeSelectorOptInClassInstance> OdDgItemTypeSelectorOptInClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: EditorCustomAttributes, Version: 1.3, Class: MemberExtendedType
    Available properties: "Standard"(integer), "Name"(string)
    Description: Specifies the extended type for member properties of an array type property
*/
class TG_EXPORT OdDgMemberExtendedTypeClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgMemberExtendedTypeClassInstance);

  virtual OdInt32 getStandard() const = 0;
  virtual void setStandard(OdInt32 iValue) = 0;

  virtual OdString getMemberExtendedTypeName() const = 0;
  virtual void setMemberExtendedTypeName(const OdString& strValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgMemberExtendedTypeClassInstance> OdDgMemberExtendedTypeClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: EditorCustomAttributes, Version: 1.3, Class: MembersIndependent
    Available properties: ---
    Description: Indicates if the members of a struct or array property should appear in ECPropertyPane as if they are in the parent structure
*/
class TG_EXPORT OdDgMembersIndependentClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgMembersIndependentClassInstance);

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgMembersIndependentClassInstance> OdDgMembersIndependentClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: EditorCustomAttributes, Version: 1.3, Class: PropertyPriority
    Available properties: "Priority"(integer)
    Description: Higher values appear first in GUI. Leave large gaps (e.g. 1000). See ECPropertyPane.PropertySortPriority
*/
class TG_EXPORT OdDgPropertyPriorityClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgPropertyPriorityClassInstance);

  virtual OdInt32 getPriority() const = 0;
  virtual void setPriority(OdInt32 iValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgPropertyPriorityClassInstance> OdDgPropertyPriorityClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: EditorCustomAttributes, Version: 1.3, Class: RequiresRefresh
    Available properties: ---
    Description: Indicates if a change in the property value requires a refresh of ECPropertyPane.  If this is set on a property and the property value changes, the PropertyPane refreshes
*/
class TG_EXPORT OdDgRequiresRefreshClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgRequiresRefreshClassInstance);

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgRequiresRefreshClassInstance> OdDgRequiresRefreshClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: EditorCustomAttributes, Version: 1.3, Class: RequiresReload
    Available properties: ---
    Description: Indicates if a change in the property value requires a reload of ECPropertyPane.  If the property value changes, descriptors are regenerated and the ECPropertyPane is reloaded with the instances
*/
class TG_EXPORT OdDgRequiresReloadClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgRequiresReloadClassInstance);

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgRequiresReloadClassInstance> OdDgRequiresReloadClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: EditorCustomAttributes, Version: 1.3, Class: SchemaPriority
    Available properties: "Priority"(integer)
*/
class TG_EXPORT OdDgSchemaPriorityClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgSchemaPriorityClassInstance);

  virtual OdInt32 getPriority() const = 0;
  virtual void setPriority(OdInt32 iValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgSchemaPriorityClassInstance> OdDgSchemaPriorityClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: EditorCustomAttributes, Version: 1.3, Class: ValueMap
    Available properties: "DisplayString"(string), "Value"(integer)
*/
class TG_EXPORT OdDgValueMapClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgValueMapClassInstance);

  virtual OdString getDisplayString() const = 0;
  virtual void setDisplayString(const OdString& strValue) = 0;

  virtual OdInt32 getValue() const = 0;
  virtual void setValue(OdInt32 iValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgValueMapClassInstance> OdDgValueMapClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: EditorCustomAttributes, Version: 1.3, Class: StandardValues
    Available properties: "MustBeFromList"(bool), "ValueMap"(array of ValueMap)
    Description: Sets the display strings for standard integer values for an IECProperty
*/
class TG_EXPORT OdDgStandardValuesClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgStandardValuesClassInstance);

  virtual bool getMustBeFromListFlag() const = 0;
  virtual void setMustBeFromListFlag(bool bValue) = 0;

  virtual OdUInt32 getValueMapItemCount() const = 0;
  virtual OdDgValueMapClassInstancePtr getValueMapItem(OdUInt32 uIndex) const = 0;
  virtual void setValueMapItem(OdUInt32 uIndex, const OdDgValueMapClassInstancePtr& value) = 0;
  virtual void addValueMapItem(const OdDgValueMapClassInstancePtr& value) = 0;
  virtual void deleteValueMapItem(OdUInt32 uIndex) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgStandardValuesClassInstance> OdDgStandardValuesClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: EditorCustomAttributes, Version: 1.3, Class: StoresUnitsAsUors
    Available properties: ---
*/
class TG_EXPORT OdDgStoresUnitsAsUorsClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgStoresUnitsAsUorsClassInstance);

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgStoresUnitsAsUorsClassInstance> OdDgStoresUnitsAsUorsClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: EditorCustomAttributes, Version: 1.3, Class: StrikethroughSpecification
    Available properties: "StrikethroughECProperty"(string)
    Description: Used by the ECGrid to determine whether IECInstances should have a red strike through (typically indicated a deleted state).
*/
class TG_EXPORT OdDgStrikethroughSpecificationClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgStrikethroughSpecificationClassInstance);

  virtual OdString getStrikethroughECProperty() const = 0;
  virtual void setStrikethroughECProperty(const OdString& strValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgStrikethroughSpecificationClassInstance> OdDgStrikethroughSpecificationClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: EditorCustomAttributes, Version: 1.3, Class: UseBooleanInExpressions
    Available properties: ---
    Description: Indicates that values of boolean properties should not be converted to strings when evaluating ECExpressions.
*/
class TG_EXPORT OdDgUseBooleanInExpressionsClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgUseBooleanInExpressionsClassInstance);

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgUseBooleanInExpressionsClassInstance> OdDgUseBooleanInExpressionsClassInstancePtr;

//------------------------------------------------------------------------------- 

//=============================================================================== 
//                Bentley_Common_Classes
//=============================================================================== 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: Bentley_Common_Classes, Version: 1.1, Class: Comment
    Available properties: "Comment"(string), "Reviewer"(string), "Date"(date), "Source"(string), "Reference"(string)
    Description: Standard Comment Class
*/
class TG_EXPORT OdDgCommentClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgCommentClassInstance);

  virtual OdString getComment() const = 0;
  virtual void setComment(const OdString& strValue) = 0;

  virtual OdString getReviewer() const = 0;
  virtual void setReviewer(const OdString& strValue) = 0;

  virtual OdTimeStamp getDate() const = 0;
  virtual void setDate(const OdTimeStamp& timeValue) = 0;

  virtual OdString getSource() const = 0;
  virtual void setSource(const OdString& strValue) = 0;

  virtual OdString getReference() const = 0;
  virtual void setReference(const OdString& strValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgCommentClassInstance> OdDgCommentClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: Bentley_Common_Classes, Version: 1.1, Class: RgbColor
    Available properties: "Red"(integer), "Blue"(integer), "Green"(integer)
    Description: Standard RGB Element Color Class
*/
class TG_EXPORT OdDgRgbColorClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgRgbColorClassInstance);

  virtual OdInt32 getRed() const = 0;
  virtual void setRed(OdInt32 iValue) = 0;

  virtual OdInt32 getBlue() const = 0;
  virtual void setBlue(OdInt32 iValue) = 0;

  virtual OdInt32 getGreen() const = 0;
  virtual void setGreen(OdInt32 iValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgRgbColorClassInstance> OdDgRgbColorClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: Bentley_Common_Classes, Version: 1.1, Class: DgnElementColor
    Available properties: "Source"(integer), "Type"(integer), "Index"(integer), "ColorBook"(string), "ColorName"(string)
    Description: Standard Dgn Element Color Class
*/
class TG_EXPORT OdDgDgnElementColorClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgDgnElementColorClassInstance);

  virtual OdInt32 getSource() const = 0;
  virtual void setSource(OdInt32 iValue) = 0;

  virtual OdInt32 getType() const = 0;
  virtual void setType(OdInt32 iValue) = 0;

  virtual OdInt32 getIndex() const = 0;
  virtual void setIndex(OdInt32 iValue) = 0;

  virtual OdString getColorBook() const = 0;
  virtual void setColorBook(const OdString& strValue) = 0;

  virtual OdString getColorName() const = 0;
  virtual void setColorName(const OdString& strValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgDgnElementColorClassInstance> OdDgDgnElementColorClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: Bentley_Common_Classes, Version: 1.1, Class: GradientKeyColor
    Available properties: "Location"(double)
    Description: Standard Gradient Key Color Class
*/
class TG_EXPORT OdDgGradientKeyColorClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgGradientKeyColorClassInstance);

  virtual double getLocation() const = 0;
  virtual void setLocation(double dValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgGradientKeyColorClassInstance> OdDgGradientKeyColorClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: Bentley_Common_Classes, Version: 1.1, Class: GradientInfo
    Available properties: "GradientAngle"(double), "GradientTint"(double), "GradientShift"(double), "GradientOptions"(integer), "GradientMode"(integer), "FillOptions"(integer), "GradientKeyColors"(array of GradientKeyColor)
    Description: Gradient color info
*/
class TG_EXPORT OdDgGradientInfoClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgGradientInfoClassInstance);

  virtual double getGradientAngle() const = 0;
  virtual void setGradientAngle(double dValue) = 0;

  virtual double getGradientTint() const = 0;
  virtual void setGradientTint(double dValue) = 0;

  virtual double getGradientShift() const = 0;
  virtual void setGradientShift(double dValue) = 0;

  virtual OdInt32 getGradientOptions() const = 0;
  virtual void setGradientOptions(OdInt32 iValue) = 0;

  virtual OdInt32 getGradientMode() const = 0;
  virtual void setGradientMode(OdInt32 iValue) = 0;

  virtual OdInt32 getFillOptions() const = 0;
  virtual void setFillOptions(OdInt32 iValue) = 0;

  virtual OdUInt32 getGradientKeyColorsItemCount() const = 0;
  virtual OdDgGradientKeyColorClassInstancePtr getGradientKeyColorsItem(OdUInt32 uIndex) const = 0;
  virtual void setGradientKeyColorsItem(OdUInt32 uIndex, const OdDgGradientKeyColorClassInstancePtr& value) = 0;
  virtual void addGradientKeyColorsItem(const OdDgGradientKeyColorClassInstancePtr& value) = 0;
  virtual void deleteGradientKeyColorsItem(OdUInt32 uIndex) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgGradientInfoClassInstance> OdDgGradientInfoClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: Bentley_Common_Classes, Version: 1.1, Class: DgnElementFillColor
    Available properties: ---
    Description: Standard Dgn Element Fill Color Class
*/
class TG_EXPORT OdDgDgnElementFillColorClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgDgnElementFillColorClassInstance);

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgDgnElementFillColorClassInstance> OdDgDgnElementFillColorClassInstancePtr;

//=============================================================================== 
//                Bentley_Standard_Classes
//=============================================================================== 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: Bentley_Standard_Classes, Version: 1.1, Class: AnyClass
    Available properties: ---
    Description: A special class to be added, during deserialization of schemas, as a constraint class to all such relationship classes that have no constrainedClasses defined in their source or target
*/
class TG_EXPORT OdDgAnyClassClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgAnyClassClassInstance);

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgAnyClassClassInstance> OdDgAnyClassClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: Bentley_Standard_Classes, Version: 1.1, Class: ECSchemaInfo
    Available properties: "Name"(string), "VersionMajor"(integer), "VersionMinor"(integer)
    Description: A class to store information about an ECSchema
*/
class TG_EXPORT OdDgECSchemaInfoClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgECSchemaInfoClassInstance);

  virtual OdString getECSchemaInfoName() const = 0;
  virtual void setECSchemaInfoName(const OdString& strValue) = 0;

  virtual OdInt32 getVersionMajor() const = 0;
  virtual void setVersionMajor(OdInt32 iValue) = 0;

  virtual OdInt32 getVersionMinor() const = 0;
  virtual void setVersionMinor(OdInt32 iValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgECSchemaInfoClassInstance> OdDgECSchemaInfoClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: Bentley_Standard_Classes, Version: 1.1, Class: InstanceCount
    Available properties: "ECSchemaName"(string), "ECClassName"(string), "Count"(integer)
    Description: Metaclass used to query the instance count of EC classes
*/
class TG_EXPORT OdDgInstanceCountClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgInstanceCountClassInstance);

  virtual OdString getECSchemaName() const = 0;
  virtual void setECSchemaName(const OdString& strValue) = 0;

  virtual OdString getECClassName() const = 0;
  virtual void setECClassName(const OdString& strValue) = 0;

  virtual OdInt32 getCount() const = 0;
  virtual void setCount(OdInt32 iValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgInstanceCountClassInstance> OdDgInstanceCountClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: Bentley_Standard_Classes, Version: 1.1, Class: AdHocContainer
    Available properties: "Name"(string), "DisplayLabel"(string), "Value"(string), "Type"(integer), "Unit"(string), "ExtendedType"(string), "IsReadOnly"(bool), "IsHidden"(bool)
*/
class TG_EXPORT OdDgAdHocContainerClassInstance : public OdDgECClassInstance
{
public:
  enum OdDgAdHocContainerType
  {
    kString       = 0,
    kInteger      = 1,
    kLongInteger  = 2,
    kDouble       = 3,
    kDateTime     = 4,
    kBoolean      = 5,
    kBinary       = 6,
    kPoint2d      = 7,
    kPoint3d      = 8
  };
public:
  ODRX_DECLARE_MEMBERS(OdDgAdHocContainerClassInstance);

  virtual OdString getAdHocContainerName() const = 0;
  virtual void setAdHocContainerName(const OdString& strValue) = 0;

  virtual OdString getDisplayLabel() const = 0;
  virtual void setDisplayLabel(const OdString& strValue) = 0;

  virtual OdString getValue() const = 0;
  virtual void setValue(const OdString& strValue) = 0;

  virtual OdDgAdHocContainerType getType() const = 0;
  virtual void setType(OdDgAdHocContainerType iValue) = 0;

  virtual OdString getUnit() const = 0;
  virtual void setUnit(const OdString& strValue) = 0;

  virtual OdString getExtendedType() const = 0;
  virtual void setExtendedType(const OdString& strValue) = 0;

  virtual bool getIsReadOnlyFlag() const = 0;
  virtual void setIsReadOnlyFlag(bool bValue) = 0;

  virtual bool getIsHiddenFlag() const = 0;
  virtual void setIsHiddenFlag(bool bValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgAdHocContainerClassInstance> OdDgAdHocContainerClassInstancePtr;

//-------------------------------------------------------------------------------

//=============================================================================== 
//                Bentley_Standard_CustomAttributes
//=============================================================================== 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: Bentley_Standard_CustomAttributes, Version: 1.13, Class: AllowDuplicateLocalizedValues
    Available properties: "Label"(bool), "Description"(bool)
    Description: Keeps the localizable values from this element from being combined with those from another element if duplication occurs.  If combined a GUID is used as the key for the localizable value.
*/
class TG_EXPORT OdDgAllowDuplicateLocalizedValuesClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgAllowDuplicateLocalizedValuesClassInstance);

  virtual bool getLabelFlag() const = 0;
  virtual void setLabelFlag(bool bValue) = 0;

  virtual bool getDescriptionFlag() const = 0;
  virtual void setDescriptionFlag(bool bValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgAllowDuplicateLocalizedValuesClassInstance> OdDgAllowDuplicateLocalizedValuesClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: Bentley_Standard_CustomAttributes, Version: 1.13, Class: PropertyReferenceProperties
    Available properties: "PropertiesThatReferenceAProperty"(array of string)
    Description: For Internal Use Only.  Applied to a custom attribute class defiition to indicate which string properties refer to a property in the class where this attribute is applied.
*/
class TG_EXPORT OdDgPropertyReferencePropertiesClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgPropertyReferencePropertiesClassInstance);

  virtual OdUInt32 getPropertiesThatReferenceAPropertyItemCount() const = 0;
  virtual OdString getPropertiesThatReferenceAPropertyItem(OdUInt32 uIndex) const = 0;
  virtual void setPropertiesThatReferenceAPropertyItem(OdUInt32 uIndex, const OdString& strValue) = 0;
  virtual void addPropertiesThatReferenceAPropertyItem(const OdString& strValue) = 0;
  virtual void deletePropertiesThatReferenceAPropertyItem(OdUInt32 uIndex) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgPropertyReferencePropertiesClassInstance> OdDgPropertyReferencePropertiesClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: Bentley_Standard_CustomAttributes, Version: 1.13, Class: BusinessKeySpecification
    Available properties: "PropertyName"(string)
    Description: Business Key Specification
*/
class TG_EXPORT OdDgBusinessKeySpecificationClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgBusinessKeySpecificationClassInstance);

  virtual OdString getPropertyName() const = 0;
  virtual void setPropertyName(const OdString& strValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgBusinessKeySpecificationClassInstance> OdDgBusinessKeySpecificationClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: Bentley_Standard_CustomAttributes, Version: 1.13, Class: CalculatedECPropertySpecification
    Available properties: "ECExpression"(string), "FailureValue"(string), "ParserRegularExpression"(string), "IsDefaultValueOnly"(bool), "UseLastValidValueOnFailure"(bool), "DoNotUseECMAScript"(bool), "RequiredSymbolSets"(array of string), "SuppressTypeConversions"(bool), "EnforceUnits"(bool)
    Description: See Bentley.ECObjects.Standards.CalculatedECPropertySpecification. ECCustomAttributeClass for specifying an ECExpression to be used for calculating the value of a given ECProperty.
*/
class TG_EXPORT OdDgCalculatedECPropertySpecificationClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgCalculatedECPropertySpecificationClassInstance);

  virtual OdString getECExpression() const = 0;
  virtual void setECExpression(const OdString& strValue) = 0;

  virtual OdString getFailureValue() const = 0;
  virtual void setFailureValue(const OdString& strValue) = 0;

  virtual OdString getParserRegularExpression() const = 0;
  virtual void setParserRegularExpression(const OdString& strValue) = 0;

  virtual bool getIsDefaultValueOnlyFlag() const = 0;
  virtual void setIsDefaultValueOnlyFlag(bool bValue) = 0;

  virtual bool getUseLastValidValueOnFailureFlag() const = 0;
  virtual void setUseLastValidValueOnFailureFlag(bool bValue) = 0;

  virtual bool getDoNotUseECMAScriptFlag() const = 0;
  virtual void setDoNotUseECMAScriptFlag(bool bValue) = 0;

  virtual OdUInt32 getRequiredSymbolSetsItemCount() const = 0;
  virtual OdString getRequiredSymbolSetsItem(OdUInt32 uIndex) const = 0;
  virtual void setRequiredSymbolSetsItem(OdUInt32 uIndex, const OdString& strValue) = 0;
  virtual void addRequiredSymbolSetsItem(const OdString& strValue) = 0;
  virtual void deleteRequiredSymbolSetsItem(OdUInt32 uIndex) = 0;

  virtual bool getSuppressTypeConversionsFlag() const = 0;
  virtual void setSuppressTypeConversionsFlag(bool bValue) = 0;

  virtual bool getEnforceUnitsFlag() const = 0;
  virtual void setEnforceUnitsFlag(bool bValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgCalculatedECPropertySpecificationClassInstance> OdDgCalculatedECPropertySpecificationClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: Bentley_Standard_CustomAttributes, Version: 1.13, Class: TabDisplaySpecification
    Available properties: "DisplayLabel"(string), "NewDisplayLabel"(string), "Show"(bool)
*/
class TG_EXPORT OdDgTabDisplaySpecificationClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgTabDisplaySpecificationClassInstance);

  virtual OdString getDisplayLabel() const = 0;
  virtual void setDisplayLabel(const OdString& strValue) = 0;

  virtual OdString getNewDisplayLabel() const = 0;
  virtual void setNewDisplayLabel(const OdString& strValue) = 0;

  virtual bool getShowFlag() const = 0;
  virtual void setShowFlag(bool bValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgTabDisplaySpecificationClassInstance> OdDgTabDisplaySpecificationClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: Bentley_Standard_CustomAttributes, Version: 1.13, Class: CEContentPaneCustomization
    Available properties: "TabSpecifications"(array of TabDisplaySpecification)
*/
class TG_EXPORT OdDgCEContentPaneCustomizationClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgCEContentPaneCustomizationClassInstance);

  virtual OdUInt32 getTabSpecificationsItemCount() const = 0;
  virtual OdDgTabDisplaySpecificationClassInstancePtr getTabSpecificationsItem(OdUInt32 uIndex) const = 0;
  virtual void setTabSpecificationsItem(OdUInt32 uIndex, const OdDgTabDisplaySpecificationClassInstancePtr& value) = 0;
  virtual void addTabSpecificationsItem(const OdDgTabDisplaySpecificationClassInstancePtr& value) = 0;
  virtual void deleteTabSpecificationsItem(OdUInt32 uIndex) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgCEContentPaneCustomizationClassInstance> OdDgCEContentPaneCustomizationClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: Bentley_Standard_CustomAttributes, Version: 1.13, Class: ChangeManagement
    Available properties: "Supported"(bool)
    Description: Defines change management features for this ECClass
*/
class TG_EXPORT OdDgChangeManagementClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgChangeManagementClassInstance);

  virtual bool getSupportedFlag() const = 0;
  virtual void setSupportedFlag(bool bValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgChangeManagementClassInstance> OdDgChangeManagementClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: Bentley_Standard_CustomAttributes, Version: 1.13, Class: ClassReferenceProperties
    Available properties: "PropertiesThatReferenceAClass"(array of string)
    Description: For Internal Use Only.  Applied to a custom attribute class definition to indicate which string properties refer to a class in the schema where the custom attribute class is applied.
*/
class TG_EXPORT OdDgClassReferencePropertiesClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgClassReferencePropertiesClassInstance);

  virtual OdUInt32 getPropertiesThatReferenceAClassItemCount() const = 0;
  virtual OdString getPropertiesThatReferenceAClassItem(OdUInt32 uIndex) const = 0;
  virtual void setPropertiesThatReferenceAClassItem(OdUInt32 uIndex, const OdString& strValue) = 0;
  virtual void addPropertiesThatReferenceAClassItem(const OdString& strValue) = 0;
  virtual void deletePropertiesThatReferenceAClassItem(OdUInt32 uIndex) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgClassReferencePropertiesClassInstance> OdDgClassReferencePropertiesClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: Bentley_Standard_CustomAttributes, Version: 1.13, Class: CustomImageSpecification
    Available properties: "Moniker_Default"(string), "Moniker_Expanded"(string), "Moniker_Collapsed"(string)
*/
class TG_EXPORT OdDgCustomImageSpecificationClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgCustomImageSpecificationClassInstance);

  virtual OdString getMoniker_Default() const = 0;
  virtual void setMoniker_Default(const OdString& strValue) = 0;

  virtual OdString getMoniker_Expanded() const = 0;
  virtual void setMoniker_Expanded(const OdString& strValue) = 0;

  virtual OdString getMoniker_Collapsed() const = 0;
  virtual void setMoniker_Collapsed(const OdString& strValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgCustomImageSpecificationClassInstance> OdDgCustomImageSpecificationClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: Bentley_Standard_CustomAttributes, Version: 1.13, Class: CustomStructSerializer
    Available properties: "SerializerName"(string)
    Description: Identifies a struct that should have a custom XML string generated
*/
class TG_EXPORT OdDgCustomStructSerializerClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgCustomStructSerializerClassInstance);

  virtual OdString getSerializerName() const = 0;
  virtual void setSerializerName(const OdString& strValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgCustomStructSerializerClassInstance> OdDgCustomStructSerializerClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: Bentley_Standard_CustomAttributes, Version: 1.13, Class: DefinitionMetaData
    Available properties: "DefinitionBackReference"(string)
*/
class TG_EXPORT OdDgDefinitionMetaDataClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgDefinitionMetaDataClassInstance);

  virtual OdString getDefinitionBackReference() const = 0;
  virtual void setDefinitionBackReference(const OdString& strValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgDefinitionMetaDataClassInstance> OdDgDefinitionMetaDataClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: Bentley_Standard_CustomAttributes, Version: 1.13, Class: DgnECRelationshipOptions
    Available properties: "PointerPersistence"(integer), "BidirectionallyTraversable"(bool), "SameFileDifferentModelBackPointer"(bool), "UseNewLocalKeyPersistence"(bool)
    Description: Specifies how relationships should be persisted.
*/
class TG_EXPORT OdDgDgnECRelationshipOptionsClassInstance : public OdDgECClassInstance
{
public:
  enum OdDgDgnECRelationshipOptionsPointerPersistenceType
  {
    kBothEnds  = 0,
    kSourceEnd = 1,
    kTargetEnd = 2
  };
public:
  ODRX_DECLARE_MEMBERS(OdDgDgnECRelationshipOptionsClassInstance);

  virtual OdDgDgnECRelationshipOptionsPointerPersistenceType getPointerPersistence() const = 0;
  virtual void setPointerPersistence(OdDgDgnECRelationshipOptionsPointerPersistenceType iValue) = 0;

  virtual bool getBidirectionallyTraversableFlag() const = 0;
  virtual void setBidirectionallyTraversableFlag(bool bValue) = 0;

  virtual bool getSameFileDifferentModelBackPointerFlag() const = 0;
  virtual void setSameFileDifferentModelBackPointerFlag(bool bValue) = 0;

  virtual bool getUseNewLocalKeyPersistenceFlag() const = 0;
  virtual void setUseNewLocalKeyPersistenceFlag(bool bValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgDgnECRelationshipOptionsClassInstance> OdDgDgnECRelationshipOptionsClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: Bentley_Standard_CustomAttributes, Version: 1.13, Class: DGNPersistedGeometry
    Available properties: "NameOfGeometryProperty"(string)
    Description: See Bentley.ECObjects.Standards.DGNPersistedGeometry. ECCustomAttributeClass for specifying that an IGeometry ECProperty is to be translated to DGN geometry when the instance is written to a DGN file with the default persistence strategy of the DgnECPlugin
*/
class TG_EXPORT OdDgDGNPersistedGeometryClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgDGNPersistedGeometryClassInstance);

  virtual OdString getNameOfGeometryProperty() const = 0;
  virtual void setNameOfGeometryProperty(const OdString& strValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgDGNPersistedGeometryClassInstance> OdDgDGNPersistedGeometryClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: Bentley_Standard_CustomAttributes, Version: 1.13, Class: DictionaryCustomAttributes
    Available properties: "IsClassDictionary"(bool), "IsPropertyDictionary"(bool), "IsUnitsDictionary"(bool), "IsDimensionDictionary"(bool), "IsKindOfQuantityDictionary"(bool)
*/
class TG_EXPORT OdDgDictionaryCustomAttributesClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgDictionaryCustomAttributesClassInstance);

  virtual bool getIsClassDictionaryFlag() const = 0;
  virtual void setIsClassDictionaryFlag(bool bValue) = 0;

  virtual bool getIsPropertyDictionaryFlag() const = 0;
  virtual void setIsPropertyDictionaryFlag(bool bValue) = 0;

  virtual bool getIsUnitsDictionaryFlag() const = 0;
  virtual void setIsUnitsDictionaryFlag(bool bValue) = 0;

  virtual bool getIsDimensionDictionaryFlag() const = 0;
  virtual void setIsDimensionDictionaryFlag(bool bValue) = 0;

  virtual bool getIsKindOfQuantityDictionaryFlag() const = 0;
  virtual void setIsKindOfQuantityDictionaryFlag(bool bValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgDictionaryCustomAttributesClassInstance> OdDgDictionaryCustomAttributesClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: Bentley_Standard_CustomAttributes, Version: 1.13, Class: DisplayOptions
    Available properties: "Hidden"(bool), "HideInstances"(bool), "HideRelated"(bool)
*/
class TG_EXPORT OdDgDisplayOptionsClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgDisplayOptionsClassInstance);

  virtual bool getHiddenFlag() const = 0;
  virtual void setHiddenFlag(bool bValue) = 0;

  virtual bool getHideInstancesFlag() const = 0;
  virtual void setHideInstancesFlag(bool bValue) = 0;

  virtual bool getHideRelatedFlag() const = 0;
  virtual void setHideRelatedFlag(bool bValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgDisplayOptionsClassInstance> OdDgDisplayOptionsClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: Bentley_Standard_CustomAttributes, Version: 1.13, Class: ItemsSchemaIdentifier
    Available properties: "IsItemsSchema"(bool), "StoresExtrinsicItems"(bool)
*/
class TG_EXPORT OdDgItemsSchemaIdentifierClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgItemsSchemaIdentifierClassInstance);

  virtual bool getIsItemsSchemaFlag() const = 0;
  virtual void setIsItemsSchemaFlag(bool bValue) = 0;

  virtual bool getStoresExtrinsicItemsFlag() const = 0;
  virtual void setStoresExtrinsicItemsFlag(bool bValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgItemsSchemaIdentifierClassInstance> OdDgItemsSchemaIdentifierClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: Bentley_Standard_CustomAttributes, Version: 1.13, Class: FileDependentProperties
    Available properties: "FileName"(string), "FileSize"(string), "DateCreated"(string), "DateModified"(string), "IsReadOnly"(string), "IsHidden"(string)
*/
class TG_EXPORT OdDgFileDependentPropertiesClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgFileDependentPropertiesClassInstance);

  virtual OdString getFileName() const = 0;
  virtual void setFileName(const OdString& strValue) = 0;

  virtual OdString getFileSize() const = 0;
  virtual void setFileSize(const OdString& strValue) = 0;

  virtual OdString getDateCreated() const = 0;
  virtual void setDateCreated(const OdString& strValue) = 0;

  virtual OdString getDateModified() const = 0;
  virtual void setDateModified(const OdString& strValue) = 0;

  virtual OdString getIsReadOnly() const = 0;
  virtual void setIsReadOnly(const OdString& strValue) = 0;

  virtual OdString getIsHidden() const = 0;
  virtual void setIsHidden(const OdString& strValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgFileDependentPropertiesClassInstance> OdDgFileDependentPropertiesClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: Bentley_Standard_CustomAttributes, Version: 1.13, Class: FolderDependentProperties
    Available properties: "FolderName"(string), "DateCreated"(string), "DateModified"(string), "IsHidden"(string)
*/
class TG_EXPORT OdDgFolderDependentPropertiesClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgFolderDependentPropertiesClassInstance);

  virtual OdString getFolderName() const = 0;
  virtual void setFolderName(const OdString& strValue) = 0;

  virtual OdString getDateCreated() const = 0;
  virtual void setDateCreated(const OdString& strValue) = 0;

  virtual OdString getDateModified() const = 0;
  virtual void setDateModified(const OdString& strValue) = 0;

  virtual OdString getIsHidden() const = 0;
  virtual void setIsHidden(const OdString& strValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgFolderDependentPropertiesClassInstance> OdDgFolderDependentPropertiesClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: Bentley_Standard_CustomAttributes, Version: 1.13, Class: GlobalIdSpecification
    Available properties: "PropertyName"(string)
    Description: Global ID Specification
*/
class TG_EXPORT OdDgGlobalIdSpecificationClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgGlobalIdSpecificationClassInstance);

  virtual OdString getPropertyName() const = 0;
  virtual void setPropertyName(const OdString& strValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgGlobalIdSpecificationClassInstance> OdDgGlobalIdSpecificationClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: Bentley_Standard_CustomAttributes, Version: 1.13, Class: IdentificationOfActivityProperties
    Available properties: "Name"(string), "Description"(string)
*/
class TG_EXPORT OdDgIdentificationOfActivityPropertiesClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgIdentificationOfActivityPropertiesClassInstance);

  virtual OdString getIdentificationOfActivityPropertiesName() const = 0;
  virtual void setIdentificationOfActivityPropertiesName(const OdString& strValue) = 0;

  virtual OdString getDescription() const = 0;
  virtual void setDescription(const OdString& strValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgIdentificationOfActivityPropertiesClassInstance> OdDgIdentificationOfActivityPropertiesClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: Bentley_Standard_CustomAttributes, Version: 1.13, Class: IdentificationOfStandardClasses
    Available properties: "ProjectBaseClass"(string), "FolderBaseClass"(string), "DocumentBaseClass"(string), "FolderParentRelationshipClass"(string), "ProjectParentRelationshipClass"(string), "DocumentParentRelationshipClass"(string), "UserBaseClass"(string), "UserGroupBaseClass"(string), "ActivityBaseClass"(string), "UserParentRelationshipClass"(string), "UserGroupParentRelationshipClass"(string), "ActivityApproverRelationshipClass"(string)
*/
class TG_EXPORT OdDgIdentificationOfStandardClassesClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgIdentificationOfStandardClassesClassInstance);

  virtual OdString getProjectBaseClass() const = 0;
  virtual void setProjectBaseClass(const OdString& strValue) = 0;

  virtual OdString getFolderBaseClass() const = 0;
  virtual void setFolderBaseClass(const OdString& strValue) = 0;

  virtual OdString getDocumentBaseClass() const = 0;
  virtual void setDocumentBaseClass(const OdString& strValue) = 0;

  virtual OdString getFolderParentRelationshipClass() const = 0;
  virtual void setFolderParentRelationshipClass(const OdString& strValue) = 0;

  virtual OdString getProjectParentRelationshipClass() const = 0;
  virtual void setProjectParentRelationshipClass(const OdString& strValue) = 0;

  virtual OdString getDocumentParentRelationshipClass() const = 0;
  virtual void setDocumentParentRelationshipClass(const OdString& strValue) = 0;

  virtual OdString getUserBaseClass() const = 0;
  virtual void setUserBaseClass(const OdString& strValue) = 0;

  virtual OdString getUserGroupBaseClass() const = 0;
  virtual void setUserGroupBaseClass(const OdString& strValue) = 0;

  virtual OdString getActivityBaseClass() const = 0;
  virtual void setActivityBaseClass(const OdString& strValue) = 0;

  virtual OdString getUserParentRelationshipClass() const = 0;
  virtual void setUserParentRelationshipClass(const OdString& strValue) = 0;

  virtual OdString getUserGroupParentRelationshipClass() const = 0;
  virtual void setUserGroupParentRelationshipClass(const OdString& strValue) = 0;

  virtual OdString getActivityApproverRelationshipClass() const = 0;
  virtual void setActivityApproverRelationshipClass(const OdString& strValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgIdentificationOfStandardClassesClassInstance> OdDgIdentificationOfStandardClassesClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: Bentley_Standard_CustomAttributes, Version: 1.13, Class: IdentificationOfUserGroupProperties
    Available properties: "Name"(string), "Description"(string), "IsSecurityGroup"(string), "SecurityContext"(string)
*/
class TG_EXPORT OdDgIdentificationOfUserGroupPropertiesClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgIdentificationOfUserGroupPropertiesClassInstance);

  virtual OdString getIdentificationOfUserGroupPropertiesName() const = 0;
  virtual void setIdentificationOfUserGroupPropertiesName(const OdString& strValue) = 0;

  virtual OdString getDescription() const = 0;
  virtual void setDescription(const OdString& strValue) = 0;

  virtual OdString getIsSecurityGroup() const = 0;
  virtual void setIsSecurityGroup(const OdString& strValue) = 0;

  virtual OdString getSecurityContext() const = 0;
  virtual void setSecurityContext(const OdString& strValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgIdentificationOfUserGroupPropertiesClassInstance> OdDgIdentificationOfUserGroupPropertiesClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: Bentley_Standard_CustomAttributes, Version: 1.13, Class: IdentificationOfUserProperties
    Available properties: "Name"(string), "Description"(string), "SecurityContext"(string), "LoginIdentifier"(string), "PrimaryEmail"(string), "SecondaryEmail"(string), "HomePhone"(string), "WorkPhone"(string), "MobilePhone"(string), "TextMessagingNumber"(string)
*/
class TG_EXPORT OdDgIdentificationOfUserPropertiesClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgIdentificationOfUserPropertiesClassInstance);

  virtual OdString getIdentificationOfUserPropertiesName() const = 0;
  virtual void setIdentificationOfUserPropertiesName(const OdString& strValue) = 0;

  virtual OdString getDescription() const = 0;
  virtual void setDescription(const OdString& strValue) = 0;

  virtual OdString getSecurityContext() const = 0;
  virtual void setSecurityContext(const OdString& strValue) = 0;

  virtual OdString getLoginIdentifier() const = 0;
  virtual void setLoginIdentifier(const OdString& strValue) = 0;

  virtual OdString getPrimaryEmail() const = 0;
  virtual void setPrimaryEmail(const OdString& strValue) = 0;

  virtual OdString getSecondaryEmail() const = 0;
  virtual void setSecondaryEmail(const OdString& strValue) = 0;

  virtual OdString getHomePhone() const = 0;
  virtual void setHomePhone(const OdString& strValue) = 0;

  virtual OdString getWorkPhone() const = 0;
  virtual void setWorkPhone(const OdString& strValue) = 0;

  virtual OdString getMobilePhone() const = 0;
  virtual void setMobilePhone(const OdString& strValue) = 0;

  virtual OdString getTextMessagingNumber() const = 0;
  virtual void setTextMessagingNumber(const OdString& strValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgIdentificationOfUserPropertiesClassInstance> OdDgIdentificationOfUserPropertiesClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: Bentley_Standard_CustomAttributes, Version: 1.13, Class: InstanceLabelSpecification
    Available properties: "PropertyName"(string)
    Description: Specifies which ECProperty to use for the InstanceLabel for tree nodes and other GUI
*/
class TG_EXPORT OdDgInstanceLabelSpecificationClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgInstanceLabelSpecificationClassInstance);

  virtual OdString getPropertyName() const = 0;
  virtual void setPropertyName(const OdString& strValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgInstanceLabelSpecificationClassInstance> OdDgInstanceLabelSpecificationClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: Bentley_Standard_CustomAttributes, Version: 1.13, Class: ClassLabelIsInstanceLabel
    Available properties: ---
    Description: Specifies that the display label of the ECClass is always used as the InstanceLabel for instances of that class. This is used to override the default behavior
        when no InstanceLabelSpecification exists, which is to use the value of any property called Name or NAME as the instance label.
        If both an ClassLabelIsInstanceLabel and InstanceLabelSpecification custom attribute exist on the same ECClass, the InstanceLabelSpecification is used.
*/
class TG_EXPORT OdDgClassLabelIsInstanceLabelClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgClassLabelIsInstanceLabelClassInstance);

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgClassLabelIsInstanceLabelClassInstance> OdDgClassLabelIsInstanceLabelClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: Bentley_Standard_CustomAttributes, Version: 1.13, Class: InstanceSortingOptions
    Available properties: "SortingType"(integer), "SortingDirection"(integer), "SortingProperty"(string)
*/
class TG_EXPORT OdDgInstanceSortingOptionsClassInstance : public OdDgECClassInstance
{
public:
  enum OdDgInstanceSortingOptionsSortingType
  {
    kNoSorting                  = 0,
    kInstanceLableSorting       = 1,
    kSpecifiedPropertySorting   = 2
  };

  enum OdDgInstanceSortingOptionsSortingDirection
  {
    kAscendingSorting   = 0,
    kDescendingSorting  = 1
  };

public:
  ODRX_DECLARE_MEMBERS(OdDgInstanceSortingOptionsClassInstance);

  virtual OdDgInstanceSortingOptionsSortingType getSortingType() const = 0;
  virtual void setSortingType(OdDgInstanceSortingOptionsSortingType iValue) = 0;

  virtual OdDgInstanceSortingOptionsSortingDirection getSortingDirection() const = 0;
  virtual void setSortingDirection(OdDgInstanceSortingOptionsSortingDirection iValue) = 0;

  virtual OdString getSortingProperty() const = 0;
  virtual void setSortingProperty(const OdString& strValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgInstanceSortingOptionsClassInstance> OdDgInstanceSortingOptionsClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: Bentley_Standard_CustomAttributes, Version: 1.13, Class: ISO15926MetaData
    Available properties: "EntityType"(string), "IdPCA"(string), "Designation"(string), "CreationDate"(date), "SubmitterOrg"(string), "Creator"(string), "Status"(string), "Note"(string), "Example"(string)
    Description: ISO 15926 metadata back reference
*/
class TG_EXPORT OdDgISO15926MetaDataClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgISO15926MetaDataClassInstance);

  virtual OdString getEntityType() const = 0;
  virtual void setEntityType(const OdString& strValue) = 0;

  virtual OdString getIdPCA() const = 0;
  virtual void setIdPCA(const OdString& strValue) = 0;

  virtual OdString getDesignation() const = 0;
  virtual void setDesignation(const OdString& strValue) = 0;

  virtual OdTimeStamp getCreationDate() const = 0;
  virtual void setCreationDate(const OdTimeStamp& timeValue) = 0;

  virtual OdString getSubmitterOrg() const = 0;
  virtual void setSubmitterOrg(const OdString& strValue) = 0;

  virtual OdString getCreator() const = 0;
  virtual void setCreator(const OdString& strValue) = 0;

  virtual OdString getStatus() const = 0;
  virtual void setStatus(const OdString& strValue) = 0;

  virtual OdString getNote() const = 0;
  virtual void setNote(const OdString& strValue) = 0;

  virtual OdString getExample() const = 0;
  virtual void setExample(const OdString& strValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgISO15926MetaDataClassInstance> OdDgISO15926MetaDataClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: Bentley_Standard_CustomAttributes, Version: 1.13, Class: ItemsBrowserTreePresentation
    Available properties: "ShowAtTopLevel"(bool)
*/
class TG_EXPORT OdDgItemsBrowserTreePresentationClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgItemsBrowserTreePresentationClassInstance);

  virtual bool getShowAtTopLevelFlag() const = 0;
  virtual void setShowAtTopLevelFlag(bool bValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgItemsBrowserTreePresentationClassInstance> OdDgItemsBrowserTreePresentationClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: Bentley_Standard_CustomAttributes, Version: 1.13, Class: LocalizationData
    Available properties: "Key"(string), "Value"(string), "IsGUID"(bool)
    Description: Contains a localized value and a key that identifies it.  The key may be a GUID or the original value.  If the value is a GUID 'IsGUID' is true.
*/
class TG_EXPORT OdDgLocalizationDataClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgLocalizationDataClassInstance);

  virtual OdString getKey() const = 0;
  virtual void setKey(const OdString& strValue) = 0;

  virtual OdString getValue() const = 0;
  virtual void setValue(const OdString& strValue) = 0;

  virtual bool getIsGUIDFlag() const = 0;
  virtual void setIsGUIDFlag(bool bValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgLocalizationDataClassInstance> OdDgLocalizationDataClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: Bentley_Standard_CustomAttributes, Version: 1.13, Class: LocalizationSpecification
    Available properties: "Locale"(string), "Resource"(array of LocalizationData)
    Description: Specifies the locale and an array of resources consisting of key-value pairs that contain localization data.
*/
class TG_EXPORT OdDgLocalizationSpecificationClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgLocalizationSpecificationClassInstance);

  virtual OdString getLocale() const = 0;
  virtual void setLocale(const OdString& strValue) = 0;

  virtual OdUInt32 getResourceItemCount() const = 0;
  virtual OdDgLocalizationDataClassInstancePtr getResourceItem(OdUInt32 uIndex) const = 0;
  virtual void setResourceItem(OdUInt32 uIndex, const OdDgLocalizationDataClassInstancePtr& value) = 0;
  virtual void addResourceItem(const OdDgLocalizationDataClassInstancePtr& value) = 0;
  virtual void deleteResourceItem(OdUInt32 uIndex) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgLocalizationSpecificationClassInstance> OdDgLocalizationSpecificationClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: Bentley_Standard_CustomAttributes, Version: 1.13, Class: PreviousName
    Available properties: "OldName"(string), "SchemaFullNameKey"(string), "Date"(date), "IsNewInThisVersion"(bool)
    Description: For Schema Evolution.  Holds a previous name for an element, the last released version when this name was used and if the current element should be saved on rename.
*/
class TG_EXPORT OdDgPreviousNameClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgPreviousNameClassInstance);

  virtual OdString getOldName() const = 0;
  virtual void setOldName(const OdString& strValue) = 0;

  virtual OdString getSchemaFullNameKey() const = 0;
  virtual void setSchemaFullNameKey(const OdString& strValue) = 0;

  virtual OdTimeStamp getDate() const = 0;
  virtual void setDate(const OdTimeStamp& timeValue) = 0;

  virtual bool getIsNewInThisVersionFlag() const = 0;
  virtual void setIsNewInThisVersionFlag(bool bValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgPreviousNameClassInstance> OdDgPreviousNameClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: Bentley_Standard_CustomAttributes, Version: 1.13, Class: PreviousNameArray
    Available properties: "PreviousNames"(array of PreviousName), "PreviousNameAlreadySaved"(bool)
    Description: For Schema Evolution.  Holds previous name entries for the current element.
*/
class TG_EXPORT OdDgPreviousNameArrayClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgPreviousNameArrayClassInstance);

  virtual OdUInt32 getPreviousNamesItemCount() const = 0;
  virtual OdDgPreviousNameClassInstancePtr getPreviousNamesItem(OdUInt32 uIndex) const = 0;
  virtual void setPreviousNamesItem(OdUInt32 uIndex, const OdDgPreviousNameClassInstancePtr& value) = 0;
  virtual void addPreviousNamesItem(const OdDgPreviousNameClassInstancePtr& value) = 0;
  virtual void deletePreviousNamesItem(OdUInt32 uIndex) = 0;

  virtual bool getPreviousNameAlreadySavedFlag() const = 0;
  virtual void setPreviousNameAlreadySavedFlag(bool bValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgPreviousNameArrayClassInstance> OdDgPreviousNameArrayClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: Bentley_Standard_CustomAttributes, Version: 1.13, Class: PrimarySchemaMetaData
    Available properties: "ContainsUnits"(bool)
    Description: Defined at the top level of a schema to indicate that this schema may have supplemental schemas
*/
class TG_EXPORT OdDgPrimarySchemaMetaDataClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgPrimarySchemaMetaDataClassInstance);

  virtual bool getContainsUnitsFlag() const = 0;
  virtual void setContainsUnitsFlag(bool bValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgPrimarySchemaMetaDataClassInstance> OdDgPrimarySchemaMetaDataClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: Bentley_Standard_CustomAttributes, Version: 1.13, Class: PropertyTemplateMetadata
    Available properties: "DataType"(string)
*/
class TG_EXPORT OdDgPropertyTemplateMetadataClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgPropertyTemplateMetadataClassInstance);

  virtual OdString getDataType() const = 0;
  virtual void setDataType(const OdString& strValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgPropertyTemplateMetadataClassInstance> OdDgPropertyTemplateMetadataClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: Bentley_Standard_CustomAttributes, Version: 1.13, Class: PropertyTransformationExpression
    Available properties: "ParserRegularExpression"(string), "TransformationECExpression"(string), "RequiredSymbolSets"(array of string)
    Description: When applied to a property it allows type changes requiring a transformation to pass through Schema Evolution and transformations using an ECExpression for any property transform.
*/
class TG_EXPORT OdDgPropertyTransformationExpressionClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgPropertyTransformationExpressionClassInstance);

  virtual OdString getParserRegularExpression() const = 0;
  virtual void setParserRegularExpression(const OdString& strValue) = 0;

  virtual OdString getTransformationECExpression() const = 0;
  virtual void setTransformationECExpression(const OdString& strValue) = 0;

  virtual OdUInt32 getRequiredSymbolSetsItemCount() const = 0;
  virtual OdString getRequiredSymbolSetsItem(OdUInt32 uIndex) const = 0;
  virtual void setRequiredSymbolSetsItem(OdUInt32 uIndex, const OdString& strValue) = 0;
  virtual void addRequiredSymbolSetsItem(const OdString& strValue) = 0;
  virtual void deleteRequiredSymbolSetsItem(OdUInt32 uIndex) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgPropertyTransformationExpressionClassInstance> OdDgPropertyTransformationExpressionClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: Bentley_Standard_CustomAttributes, Version: 1.13, Class: ReferenceTypeRelationship
    Available properties: ---
    Description: Custom attribute showing that the relationship is of reference type. This means that the relationship is just reference, but not an object. IDs for such relationship are not meaningful.
*/
class TG_EXPORT OdDgReferenceTypeRelationshipClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgReferenceTypeRelationshipClassInstance);

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgReferenceTypeRelationshipClassInstance> OdDgReferenceTypeRelationshipClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: Bentley_Standard_CustomAttributes, Version: 1.13, Class: RelatedItemsDisplaySpecification
    Available properties: "ParentClass"(string), "RelationshipPath"(string), "DerivedClasses"(array of string)
    Description: A specification of related items to display with the parent
*/
class TG_EXPORT OdDgRelatedItemsDisplaySpecificationClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgRelatedItemsDisplaySpecificationClassInstance);

  virtual OdString getParentClass() const = 0;
  virtual void setParentClass(const OdString& strValue) = 0;

  virtual OdString getRelationshipPath() const = 0;
  virtual void setRelationshipPath(const OdString& strValue) = 0;

  virtual OdUInt32 getDerivedClassesItemCount() const = 0;
  virtual OdString getDerivedClassesItem(OdUInt32 uIndex) const = 0;
  virtual void setDerivedClassesItem(OdUInt32 uIndex, const OdString& strValue) = 0;
  virtual void addDerivedClassesItem(const OdString& strValue) = 0;
  virtual void deleteDerivedClassesItem(OdUInt32 uIndex) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgRelatedItemsDisplaySpecificationClassInstance> OdDgRelatedItemsDisplaySpecificationClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: Bentley_Standard_CustomAttributes, Version: 1.13, Class: RelatedItemsDisplaySpecifications
    Available properties: "Specifications"(array of RelatedItemsDisplaySpecification)
    Description: Allows the display of related item's properties with the current parent
*/
class TG_EXPORT OdDgRelatedItemsDisplaySpecificationsClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgRelatedItemsDisplaySpecificationsClassInstance);

  virtual OdUInt32 getSpecificationsItemCount() const = 0;
  virtual OdDgRelatedItemsDisplaySpecificationClassInstancePtr getSpecificationsItem(OdUInt32 uIndex) const = 0;
  virtual void setSpecificationsItem(OdUInt32 uIndex, const OdDgRelatedItemsDisplaySpecificationClassInstancePtr& value) = 0;
  virtual void addSpecificationsItem(const OdDgRelatedItemsDisplaySpecificationClassInstancePtr& value) = 0;
  virtual void deleteSpecificationsItem(OdUInt32 uIndex) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgRelatedItemsDisplaySpecificationsClassInstance> OdDgRelatedItemsDisplaySpecificationsClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: Bentley_Standard_CustomAttributes, Version: 1.13, Class: RelatesToExtendedPropertyObject
    Available properties: ---
*/
class TG_EXPORT OdDgRelatesToExtendedPropertyObjectClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgRelatesToExtendedPropertyObjectClassInstance);

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgRelatesToExtendedPropertyObjectClassInstance> OdDgRelatesToExtendedPropertyObjectClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: Bentley_Standard_CustomAttributes, Version: 1.13, Class: SchemaFinalizationInfo
    Available properties: "FinalizationDate"(date), "SchemaFullNameKey"(string)
*/
class TG_EXPORT OdDgSchemaFinalizationInfoClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgSchemaFinalizationInfoClassInstance);

  virtual OdTimeStamp getFinalizationDate() const = 0;
  virtual void setFinalizationDate(const OdTimeStamp& timeValue) = 0;

  virtual OdString getSchemaFullNameKey() const = 0;
  virtual void setSchemaFullNameKey(const OdString& strValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgSchemaFinalizationInfoClassInstance> OdDgSchemaFinalizationInfoClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: Bentley_Standard_CustomAttributes, Version: 1.13, Class: SchemaChangeTrackingState
    Available properties: "IsFinalizedForRelease"(bool), "SchemaFullNameAtEditingStart"(string), "TrackingEnabled"(bool), "FinalizationHistory"(array of SchemaFinalizationInfo)
    Description: Holds some important change tracking information about a schema.
*/
class TG_EXPORT OdDgSchemaChangeTrackingStateClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgSchemaChangeTrackingStateClassInstance);

  virtual bool getIsFinalizedForReleaseFlag() const = 0;
  virtual void setIsFinalizedForReleaseFlag(bool bValue) = 0;

  virtual OdString getSchemaFullNameAtEditingStart() const = 0;
  virtual void setSchemaFullNameAtEditingStart(const OdString& strValue) = 0;

  virtual bool getTrackingEnabledFlag() const = 0;
  virtual void setTrackingEnabledFlag(bool bValue) = 0;

  virtual OdUInt32 getFinalizationHistoryItemCount() const = 0;
  virtual OdDgSchemaFinalizationInfoClassInstancePtr getFinalizationHistoryItem(OdUInt32 uIndex) const = 0;
  virtual void setFinalizationHistoryItem(OdUInt32 uIndex, const OdDgSchemaFinalizationInfoClassInstancePtr& value) = 0;
  virtual void addFinalizationHistoryItem(const OdDgSchemaFinalizationInfoClassInstancePtr& value) = 0;
  virtual void deleteFinalizationHistoryItem(OdUInt32 uIndex) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgSchemaChangeTrackingStateClassInstance> OdDgSchemaChangeTrackingStateClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: Bentley_Standard_CustomAttributes, Version: 1.13, Class: SupplementalSchemaMetaData
    Available properties: "PrimarySchemaName"(string), "PrimarySchemaMajorVersion"(integer), "PrimarySchemaMinorVersion"(integer), "Precedence"(integer), "Purpose"(string), "IsUserSpecific"(bool)
    Description: Custom Attribute Class that is applied to a supplemental schema
*/
class TG_EXPORT OdDgSupplementalSchemaMetaDataClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgSupplementalSchemaMetaDataClassInstance);

  virtual OdString getPrimarySchemaName() const = 0;
  virtual void setPrimarySchemaName(const OdString& strValue) = 0;

  virtual OdInt32 getPrimarySchemaMajorVersion() const = 0;
  virtual void setPrimarySchemaMajorVersion(OdInt32 iValue) = 0;

  virtual OdInt32 getPrimarySchemaMinorVersion() const = 0;
  virtual void setPrimarySchemaMinorVersion(OdInt32 iValue) = 0;

  virtual OdInt32 getPrecedence() const = 0;
  virtual void setPrecedence(OdInt32 iValue) = 0;

  virtual OdString getPurpose() const = 0;
  virtual void setPurpose(const OdString& strValue) = 0;

  virtual bool getIsUserSpecificFlag() const = 0;
  virtual void setIsUserSpecificFlag(bool bValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgSupplementalSchemaMetaDataClassInstance> OdDgSupplementalSchemaMetaDataClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: Bentley_Standard_CustomAttributes, Version: 1.13, Class: SupportsOrderedRelationships
    Available properties: "OrderIdSourceProperty"(string), "OrderIdTargetProperty"(string)
    Description: Custom attribute that indicates if a relationship class supports ordered relationships.
*/
class TG_EXPORT OdDgSupportsOrderedRelationshipsClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgSupportsOrderedRelationshipsClassInstance);

  virtual OdString getOrderIdSourceProperty() const = 0;
  virtual void setOrderIdSourceProperty(const OdString& strValue) = 0;

  virtual OdString getOrderIdTargetProperty() const = 0;
  virtual void setOrderIdTargetProperty(const OdString& strValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgSupportsOrderedRelationshipsClassInstance> OdDgSupportsOrderedRelationshipsClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: Bentley_Standard_CustomAttributes, Version: 1.13, Class: OrderedRelationshipsConstraint
    Available properties: "OrderIdProperty"(string), "OrderIdStorageMode"(integer)
    Description: Custom attribute that indicates if a relationship constraint supports ordered relationships.
*/
class TG_EXPORT OdDgOrderedRelationshipsConstraintClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgOrderedRelationshipsConstraintClassInstance);

  virtual OdString getOrderIdProperty() const = 0;
  virtual void setOrderIdProperty(const OdString& strValue) = 0;

  virtual OdInt32 getOrderIdStorageMode() const = 0;
  virtual void setOrderIdStorageMode(OdInt32 iValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgOrderedRelationshipsConstraintClassInstance> OdDgOrderedRelationshipsConstraintClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: Bentley_Standard_CustomAttributes, Version: 1.13, Class: AssociatedStringConstraint
    Available properties: "AssociatedStringProperty"(string)
    Description: Custom attribute that indicates if a relationship source or target supports associated string.
*/
class TG_EXPORT OdDgAssociatedStringConstraintClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgAssociatedStringConstraintClassInstance);

  virtual OdString getAssociatedStringProperty() const = 0;
  virtual void setAssociatedStringProperty(const OdString& strValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgAssociatedStringConstraintClassInstance> OdDgAssociatedStringConstraintClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: Bentley_Standard_CustomAttributes, Version: 1.13, Class: SupportsRelationshipProperties
    Available properties: ---
    Description: Custom attribute that indicates if a relationship class supports storing properties.
*/
class TG_EXPORT OdDgSupportsRelationshipPropertiesClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgSupportsRelationshipPropertiesClassInstance);

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgSupportsRelationshipPropertiesClassInstance> OdDgSupportsRelationshipPropertiesClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: Bentley_Standard_CustomAttributes, Version: 1.13, Class: SyncIDSpecification
    Available properties: "Property"(string)
    Description: Custom attribute that points on property that should be used as synchronization ID
*/
class TG_EXPORT OdDgSyncIDSpecificationClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgSyncIDSpecificationClassInstance);

  virtual OdString getProperty() const = 0;
  virtual void setProperty(const OdString& strValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgSyncIDSpecificationClassInstance> OdDgSyncIDSpecificationClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: Bentley_Standard_CustomAttributes, Version: 1.13, Class: TreeColorCustomAttributes
    Available properties: "NodeBackColor"(string), "NodeForeColor"(string)
*/
class TG_EXPORT OdDgTreeColorCustomAttributesClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgTreeColorCustomAttributesClassInstance);

  virtual OdString getNodeBackColor() const = 0;
  virtual void setNodeBackColor(const OdString& strValue) = 0;

  virtual OdString getNodeForeColor() const = 0;
  virtual void setNodeForeColor(const OdString& strValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgTreeColorCustomAttributesClassInstance> OdDgTreeColorCustomAttributesClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: Bentley_Standard_CustomAttributes, Version: 1.13, Class: ValueMapEntry
    Available properties: "Key"(string), "Value"(string)
*/
class TG_EXPORT OdDgValueMapEntryClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgValueMapEntryClassInstance);

  virtual OdString getKey() const = 0;
  virtual void setKey(const OdString& strValue) = 0;

  virtual OdString getValue() const = 0;
  virtual void setValue(const OdString& strValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgValueMapEntryClassInstance> OdDgValueMapEntryClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: Bentley_Standard_CustomAttributes, Version: 1.13, Class: TransformationValueMap
    Available properties: "Name"(string), "Entries"(array of ValueMapEntry)
*/
class TG_EXPORT OdDgTransformationValueMapClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgTransformationValueMapClassInstance);

  virtual OdString getTransformationValueMapName() const = 0;
  virtual void setTransformationValueMapName(const OdString& strValue) = 0;

  virtual OdUInt32 getEntriesItemCount() const = 0;
  virtual OdDgValueMapEntryClassInstancePtr getEntriesItem(OdUInt32 uIndex) const = 0;
  virtual void setEntriesItem(OdUInt32 uIndex, const OdDgValueMapEntryClassInstancePtr& value) = 0;
  virtual void addEntriesItem(const OdDgValueMapEntryClassInstancePtr& value) = 0;
  virtual void deleteEntriesItem(OdUInt32 uIndex) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgTransformationValueMapClassInstance> OdDgTransformationValueMapClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: Bentley_Standard_CustomAttributes, Version: 1.13, Class: TransformationValueMaps
    Available properties: "ValueMaps"(array of TransformationValueMap)
*/
class TG_EXPORT OdDgTransformationValueMapsClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgTransformationValueMapsClassInstance);

  virtual OdUInt32 getValueMapsItemCount() const = 0;
  virtual OdDgTransformationValueMapClassInstancePtr getValueMapsItem(OdUInt32 uIndex) const = 0;
  virtual void setValueMapsItem(OdUInt32 uIndex, const OdDgTransformationValueMapClassInstancePtr& value) = 0;
  virtual void addValueMapsItem(const OdDgTransformationValueMapClassInstancePtr& value) = 0;
  virtual void deleteValueMapsItem(OdUInt32 uIndex) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgTransformationValueMapsClassInstance> OdDgTransformationValueMapsClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: Bentley_Standard_CustomAttributes, Version: 1.13, Class: StrictComparisonOnly
    Available properties: ---
*/
class TG_EXPORT OdDgStrictComparisonOnlyClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgStrictComparisonOnlyClassInstance);

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgStrictComparisonOnlyClassInstance> OdDgStrictComparisonOnlyClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: Bentley_Standard_CustomAttributes, Version: 1.13, Class: DateTimeInfo
    Available properties: "DateTimeKind"(string), "DateTimeComponent"(string)
    Description: Optional additional meta data for ECProperties of type DateTime.
*/
class TG_EXPORT OdDgDateTimeInfoClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgDateTimeInfoClassInstance);

  virtual OdString getDateTimeKind() const = 0;
  virtual void setDateTimeKind(const OdString& strValue) = 0;

  virtual OdString getDateTimeComponent() const = 0;
  virtual void setDateTimeComponent(const OdString& strValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgDateTimeInfoClassInstance> OdDgDateTimeInfoClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: Bentley_Standard_CustomAttributes, Version: 1.13, Class: ECDbSchemaHint
    Available properties: "TablePrefix"(string), "DefaultPropertyStrategy"(string), "DefaultClassMapStrategy"(string)
    Description: DEPRECATED: Use ECDbMap ECSchema instead
*/
class TG_EXPORT OdDgECDbSchemaHintClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgECDbSchemaHintClassInstance);

  virtual OdString getTablePrefix() const = 0;
  virtual void setTablePrefix(const OdString& strValue) = 0;

  virtual OdString getDefaultPropertyStrategy() const = 0;
  virtual void setDefaultPropertyStrategy(const OdString& strValue) = 0;

  virtual OdString getDefaultClassMapStrategy() const = 0;
  virtual void setDefaultClassMapStrategy(const OdString& strValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgECDbSchemaHintClassInstance> OdDgECDbSchemaHintClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: Bentley_Standard_CustomAttributes, Version: 1.13, Class: ECDbPropertyHint
    Available properties: "ColumnName"(string), "MapStrategy"(string), "IsNullable"(bool), "IsUnique"(bool), "Collate"(string), "Precision"(string)
    Description: DEPRECATED: Use ECDbMap ECSchema instead
*/
class TG_EXPORT OdDgECDbPropertyHintClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgECDbPropertyHintClassInstance);

  virtual OdString getColumnName() const = 0;
  virtual void setColumnName(const OdString& strValue) = 0;

  virtual OdString getMapStrategy() const = 0;
  virtual void setMapStrategy(const OdString& strValue) = 0;

  virtual bool getIsNullableFlag() const = 0;
  virtual void setIsNullableFlag(bool bValue) = 0;

  virtual bool getIsUniqueFlag() const = 0;
  virtual void setIsUniqueFlag(bool bValue) = 0;

  virtual OdString getCollate() const = 0;
  virtual void setCollate(const OdString& strValue) = 0;

  virtual OdString getPrecision() const = 0;
  virtual void setPrecision(const OdString& strValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgECDbPropertyHintClassInstance> OdDgECDbPropertyHintClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: Bentley_Standard_CustomAttributes, Version: 1.13, Class: ECDbIndex
    Available properties: "Name"(string), "IsUnique"(bool), "Properties"(array of string), "Where"(string)
    Description: DEPRECATED: Use ECDbMap ECSchema instead
*/
class TG_EXPORT OdDgECDbIndexClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgECDbIndexClassInstance);

  virtual OdString getECDbIndexName() const = 0;
  virtual void setECDbIndexName(const OdString& strValue) = 0;

  virtual bool getIsUniqueFlag() const = 0;
  virtual void setIsUniqueFlag(bool bValue) = 0;

  virtual OdUInt32 getPropertiesItemCount() const = 0;
  virtual OdString getPropertiesItem(OdUInt32 uIndex) const = 0;
  virtual void setPropertiesItem(OdUInt32 uIndex, const OdString& strValue) = 0;
  virtual void addPropertiesItem(const OdString& strValue) = 0;
  virtual void deletePropertiesItem(OdUInt32 uIndex) = 0;

  virtual OdString getWhere() const = 0;
  virtual void setWhere(const OdString& strValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgECDbIndexClassInstance> OdDgECDbIndexClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: Bentley_Standard_CustomAttributes, Version: 1.13, Class: ECDbForeignKeyConstraint
    Available properties: "EnforceReferentialIntegrityCheck"(bool), "OnDeleteAction"(string), "OnUpdateAction"(string), "MatchType"(string)
    Description: Specify database forign key constraint.
*/
class TG_EXPORT OdDgECDbForeignKeyConstraintClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgECDbForeignKeyConstraintClassInstance);

  virtual bool getEnforceReferentialIntegrityCheckFlag() const = 0;
  virtual void setEnforceReferentialIntegrityCheckFlag(bool bValue) = 0;

  virtual OdString getOnDeleteAction() const = 0;
  virtual void setOnDeleteAction(const OdString& strValue) = 0;

  virtual OdString getOnUpdateAction() const = 0;
  virtual void setOnUpdateAction(const OdString& strValue) = 0;

  virtual OdString getMatchType() const = 0;
  virtual void setMatchType(const OdString& strValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgECDbForeignKeyConstraintClassInstance> OdDgECDbForeignKeyConstraintClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: Bentley_Standard_CustomAttributes, Version: 1.13, Class: ECDbRelationshipConstraintHint
    Available properties: "ECClassIdColumn"(string), "ECIdColumn"(string), "GenerateDefaultIndex"(bool), "ForeignKeyConstraint"(ECDbForeignKeyConstraint)
    Description: Optional hint to tell how to map relationship constraint
*/
class TG_EXPORT OdDgECDbRelationshipConstraintHintClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgECDbRelationshipConstraintHintClassInstance);

  virtual OdString getECClassIdColumn() const = 0;
  virtual void setECClassIdColumn(const OdString& strValue) = 0;

  virtual OdString getECIdColumn() const = 0;
  virtual void setECIdColumn(const OdString& strValue) = 0;

  virtual bool getGenerateDefaultIndexFlag() const = 0;
  virtual void setGenerateDefaultIndexFlag(bool bValue) = 0;

  virtual OdDgECDbForeignKeyConstraintClassInstancePtr getForeignKeyConstraint() const = 0;
  virtual void setForeignKeyConstraint(const OdDgECDbForeignKeyConstraintClassInstancePtr& value) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgECDbRelationshipConstraintHintClassInstance> OdDgECDbRelationshipConstraintHintClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: Bentley_Standard_CustomAttributes, Version: 1.13, Class: ECDbClassHint
    Available properties: "MapStrategy"(string), "ECIdColumn"(string), "TableName"(string), "MapToExistingTable"(bool), "ReplaceEmptyTableWithEmptyView"(bool), "Indexes"(array of ECDbIndex), "NamedGroupIsAssembly"(bool), "MapStrategyOption"(string)
    Description: DEPRECATED: Use ECDbMap ECSchema instead
*/
class TG_EXPORT OdDgECDbClassHintClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgECDbClassHintClassInstance);

  virtual OdString getMapStrategy() const = 0;
  virtual void setMapStrategy(const OdString& strValue) = 0;

  virtual OdString getECIdColumn() const = 0;
  virtual void setECIdColumn(const OdString& strValue) = 0;

  virtual OdString getTableName() const = 0;
  virtual void setTableName(const OdString& strValue) = 0;

  virtual bool getMapToExistingTableFlag() const = 0;
  virtual void setMapToExistingTableFlag(bool bValue) = 0;

  virtual bool getReplaceEmptyTableWithEmptyViewFlag() const = 0;
  virtual void setReplaceEmptyTableWithEmptyViewFlag(bool bValue) = 0;

  virtual OdUInt32 getIndexesItemCount() const = 0;
  virtual OdDgECDbIndexClassInstancePtr getIndexesItem(OdUInt32 uIndex) const = 0;
  virtual void setIndexesItem(OdUInt32 uIndex, const OdDgECDbIndexClassInstancePtr& value) = 0;
  virtual void addIndexesItem(const OdDgECDbIndexClassInstancePtr& value) = 0;
  virtual void deleteIndexesItem(OdUInt32 uIndex) = 0;

  virtual bool getNamedGroupIsAssemblyFlag() const = 0;
  virtual void setNamedGroupIsAssemblyFlag(bool bValue) = 0;

  virtual OdString getMapStrategyOption() const = 0;
  virtual void setMapStrategyOption(const OdString& strValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgECDbClassHintClassInstance> OdDgECDbClassHintClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: Bentley_Standard_CustomAttributes, Version: 1.13, Class: ECDbRelationshipClassHint
    Available properties: "PreferredDirection"(string), "AllowDuplicateRelationships"(bool), "SourceECIdColumn"(string), "SourceECClassIdColumn"(string), "TargetECClassIdColumn"(string), "TargetECIdColumn"(string)
    Description: Optional hint to provide indexing information to optimize relationship querying.
*/
class TG_EXPORT OdDgECDbRelationshipClassHintClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgECDbRelationshipClassHintClassInstance);

  virtual OdString getPreferredDirection() const = 0;
  virtual void setPreferredDirection(const OdString& strValue) = 0;

  virtual bool getAllowDuplicateRelationshipsFlag() const = 0;
  virtual void setAllowDuplicateRelationshipsFlag(bool bValue) = 0;

  virtual OdString getSourceECIdColumn() const = 0;
  virtual void setSourceECIdColumn(const OdString& strValue) = 0;

  virtual OdString getSourceECClassIdColumn() const = 0;
  virtual void setSourceECClassIdColumn(const OdString& strValue) = 0;

  virtual OdString getTargetECClassIdColumn() const = 0;
  virtual void setTargetECClassIdColumn(const OdString& strValue) = 0;

  virtual OdString getTargetECIdColumn() const = 0;
  virtual void setTargetECIdColumn(const OdString& strValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgECDbRelationshipClassHintClassInstance> OdDgECDbRelationshipClassHintClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: Bentley_Standard_CustomAttributes, Version: 1.13, Class: DynamicSchema
    Available properties: ---
    Description: ECSchema level attribute that mark schema as dynamic. A Dynamic schema is generated by applications.
*/
class TG_EXPORT OdDgDynamicSchemaClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgDynamicSchemaClassInstance);

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgDynamicSchemaClassInstance> OdDgDynamicSchemaClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: Bentley_Standard_CustomAttributes, Version: 1.13, Class: SystemSchema
    Available properties: ---
    Description: ECSchema level attribute that mark schema as system schema. A system schema is internal to application or product for managing and storing its internal data.
*/
class TG_EXPORT OdDgSystemSchemaClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgSystemSchemaClassInstance);

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgSystemSchemaClassInstance> OdDgSystemSchemaClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: Bentley_Standard_CustomAttributes, Version: 1.13, Class: PersistStringsAsUtf8
    Available properties: ---
    Description: Applied to any ECClass to indicate that if instances of the class are persisted in ECD format, all strings in the ECD buffer should be persisted as Utf-8
*/
class TG_EXPORT OdDgPersistStringsAsUtf8ClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgPersistStringsAsUtf8ClassInstance);

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgPersistStringsAsUtf8ClassInstance> OdDgPersistStringsAsUtf8ClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: Bentley_Standard_CustomAttributes, Version: 1.13, Class: SchemaNameAndPurpose
    Available properties: "SchemaName"(string), "Purpose"(string)
    Description: Helper class for the SupplementalProvenance custom attribute.  Used to define a supplemental schema and its purpose
*/
class TG_EXPORT OdDgSchemaNameAndPurposeClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgSchemaNameAndPurposeClassInstance);

  virtual OdString getECSchemaName() const = 0;
  virtual void setECSchemaName(const OdString& strValue) = 0;

  virtual OdString getPurpose() const = 0;
  virtual void setPurpose(const OdString& strValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgSchemaNameAndPurposeClassInstance> OdDgSchemaNameAndPurposeClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: Bentley_Standard_CustomAttributes, Version: 1.13, Class: SupplementalProvenance
    Available properties: "SupplementalSchemaNamesAndPurposes"(array of SchemaNameAndPurpose)
    Description: Provenance defining what supplemental schemas were used to create a compiled, supplemented schema
*/
class TG_EXPORT OdDgSupplementalProvenanceClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgSupplementalProvenanceClassInstance);

  virtual OdUInt32 getSupplementalSchemaNamesAndPurposesItemCount() const = 0;
  virtual OdDgSchemaNameAndPurposeClassInstancePtr getSupplementalSchemaNamesAndPurposesItem(OdUInt32 uIndex) const = 0;
  virtual void setSupplementalSchemaNamesAndPurposesItem(OdUInt32 uIndex, const OdDgSchemaNameAndPurposeClassInstancePtr& value) = 0;
  virtual void addSupplementalSchemaNamesAndPurposesItem(const OdDgSchemaNameAndPurposeClassInstancePtr& value) = 0;
  virtual void deleteSupplementalSchemaNamesAndPurposesItem(OdUInt32 uIndex) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgSupplementalProvenanceClassInstance> OdDgSupplementalProvenanceClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: Bentley_Standard_CustomAttributes, Version: 1.13, Class: SearchOptions
    Available properties: "ShowWhenDerivedClassIsShown"(bool), "SearchPolymorphically"(bool), "Hidden"(bool)
    Description: ECClass level attribute used to specify search options for a class.
*/
class TG_EXPORT OdDgSearchOptionsClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgSearchOptionsClassInstance);

  virtual bool getShowWhenDerivedClassIsShownFlag() const = 0;
  virtual void setShowWhenDerivedClassIsShownFlag(bool bValue) = 0;

  virtual bool getSearchPolymorphicallyFlag() const = 0;
  virtual void setSearchPolymorphicallyFlag(bool bValue) = 0;

  virtual bool getHiddenFlag() const = 0;
  virtual void setHiddenFlag(bool bValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgSearchOptionsClassInstance> OdDgSearchOptionsClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: Bentley_Standard_CustomAttributes, Version: 1.13, Class: AdhocPropertyContainerDefinition
    Available properties: "NameProperty"(string), "DisplayLabelProperty"(string), "ValueProperty"(string), "TypeProperty"(string), "UnitProperty"(string), "ExtendTypeProperty"(string), "IsReadOnlyProperty"(string), "IsHiddenProperty"(string)
*/
class TG_EXPORT OdDgAdhocPropertyContainerDefinitionClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgAdhocPropertyContainerDefinitionClassInstance);

  virtual OdString getNameProperty() const = 0;
  virtual void setNameProperty(const OdString& strValue) = 0;

  virtual OdString getDisplayLabelProperty() const = 0;
  virtual void setDisplayLabelProperty(const OdString& strValue) = 0;

  virtual OdString getValueProperty() const = 0;
  virtual void setValueProperty(const OdString& strValue) = 0;

  virtual OdString getTypeProperty() const = 0;
  virtual void setTypeProperty(const OdString& strValue) = 0;

  virtual OdString getUnitProperty() const = 0;
  virtual void setUnitProperty(const OdString& strValue) = 0;

  virtual OdString getExtendTypeProperty() const = 0;
  virtual void setExtendTypeProperty(const OdString& strValue) = 0;

  virtual OdString getIsReadOnlyProperty() const = 0;
  virtual void setIsReadOnlyProperty(const OdString& strValue) = 0;

  virtual OdString getIsHiddenProperty() const = 0;
  virtual void setIsHiddenProperty(const OdString& strValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgAdhocPropertyContainerDefinitionClassInstance> OdDgAdhocPropertyContainerDefinitionClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: Bentley_Standard_CustomAttributes, Version: 1.13, Class: AdhocPropertySpecification
    Available properties: "AdhocPropertyContainer"(string)
*/
class TG_EXPORT OdDgAdhocPropertySpecificationClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgAdhocPropertySpecificationClassInstance);

  virtual OdString getAdhocPropertyContainer() const = 0;
  virtual void setAdhocPropertyContainer(const OdString& strValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgAdhocPropertySpecificationClassInstance> OdDgAdhocPropertySpecificationClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: Bentley_Standard_CustomAttributes, Version: 1.13, Class: ClassHasCurrentTimeStampProperty
    Available properties: "PropertyName"(string)
*/
class TG_EXPORT OdDgClassHasCurrentTimeStampPropertyClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgClassHasCurrentTimeStampPropertyClassInstance);

  virtual OdString getPropertyName() const = 0;
  virtual void setPropertyName(const OdString& strValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgClassHasCurrentTimeStampPropertyClassInstance> OdDgClassHasCurrentTimeStampPropertyClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: Bentley_Standard_CustomAttributes, Version: 1.13, Class: IsSymmetricalRelationship
    Available properties: ---
    Description: Indicates that the direction of the ECRelationship is meaningless. For example, a relationship that relates two ECInstances of class 'A' by virtue of residing on the same element
*/
class TG_EXPORT OdDgIsSymmetricalRelationshipClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgIsSymmetricalRelationshipClassInstance);

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgIsSymmetricalRelationshipClassInstance> OdDgIsSymmetricalRelationshipClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: Bentley_Standard_CustomAttributes, Version: 1.13, Class: PresentationFilterMetadata
    Available properties: "PropertyName"(string)
*/
class TG_EXPORT OdDgPresentationFilterMetadataClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgPresentationFilterMetadataClassInstance);

  virtual OdString getPropertyName() const = 0;
  virtual void setPropertyName(const OdString& strValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgPresentationFilterMetadataClassInstance> OdDgPresentationFilterMetadataClassInstancePtr;

//------------------------------------------------------------------------------- 


//=============================================================================== 
//                ECv3ConversionAttributes
//=============================================================================== 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: ECv3ConversionAttributes, Version: 1.0, Class: ForceAbstract
    Available properties: ---
*/
class TG_EXPORT OdDgForceAbstractClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgForceAbstractClassInstance);

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgForceAbstractClassInstance> OdDgForceAbstractClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: ECv3ConversionAttributes, Version: 1.0, Class: ForceCustomAttributeClass
    Available properties: ---
*/
class TG_EXPORT OdDgForceCustomAttributeClassClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgForceCustomAttributeClassClassInstance);

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgForceCustomAttributeClassClassInstance> OdDgForceCustomAttributeClassClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: ECv3ConversionAttributes, Version: 1.0, Class: ForceEntityClass
    Available properties: ---
*/
class TG_EXPORT OdDgForceEntityClassClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgForceEntityClassClassInstance);

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgForceEntityClassClassInstance> OdDgForceEntityClassClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: ECv3ConversionAttributes, Version: 1.0, Class: ForceSealed
    Available properties: ---
*/
class TG_EXPORT OdDgForceSealedClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgForceSealedClassInstance);

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgForceSealedClassInstance> OdDgForceSealedClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: ECv3ConversionAttributes, Version: 1.0, Class: ForceStructClass
    Available properties: ---
*/
class TG_EXPORT OdDgForceStructClassClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgForceStructClassClassInstance);

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgForceStructClassClassInstance> OdDgForceStructClassClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: ECv3ConversionAttributes, Version: 1.0, Class: DropClass
    Available properties: ---
*/
class TG_EXPORT OdDgDropClassClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgDropClassClassInstance);

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgDropClassClassInstance> OdDgDropClassClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: ECv3ConversionAttributes, Version: 1.0, Class: ResolvePropertyNameConflicts
    Available properties: ---
*/
class TG_EXPORT OdDgResolvePropertyNameConflictsClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgResolvePropertyNameConflictsClassInstance);

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgResolvePropertyNameConflictsClassInstance> OdDgResolvePropertyNameConflictsClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: ECv3ConversionAttributes, Version: 1.0, Class: ResolveClassNameConflicts
    Available properties: ---
*/
class TG_EXPORT OdDgResolveClassNameConflictsClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgResolveClassNameConflictsClassInstance);

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgResolveClassNameConflictsClassInstance> OdDgResolveClassNameConflictsClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: ECv3ConversionAttributes, Version: 1.0, Class: OverwriteCardinality
    Available properties: "Cardinality"(string)
*/
class TG_EXPORT OdDgOverwriteCardinalityClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgOverwriteCardinalityClassInstance);

  virtual OdString getCardinality() const = 0;
  virtual void setCardinality(const OdString& strValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgOverwriteCardinalityClassInstance> OdDgOverwriteCardinalityClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: ECv3ConversionAttributes, Version: 1.0, Class: IgnoreBaseClass
    Available properties: ---
*/
class TG_EXPORT OdDgIgnoreBaseClassClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgIgnoreBaseClassClassInstance);

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgIgnoreBaseClassClassInstance> OdDgIgnoreBaseClassClassInstancePtr;

//------------------------------------------------------------------------------- 

//=============================================================================== 
//                iip_mdb_customAttributes
//=============================================================================== 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: iip_mdb_customAttributes, Version: 1.0, Class: MDB_Attributes
    Available properties: "colNumber"(integer)
*/
class TG_EXPORT OdDgMDB_AttributesClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgMDB_AttributesClassInstance);

  virtual OdInt32 getcolNumber() const = 0;
  virtual void setcolNumber(OdInt32 iValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgMDB_AttributesClassInstance> OdDgMDB_AttributesClassInstancePtr;

//------------------------------------------------------------------------------- 

//=============================================================================== 
//                rdl_customAttributes
//=============================================================================== 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: rdl_customAttributes, Version: 1.0, Class: RDL_Attributes
    Available properties: "INST_UOM"(string), "META_SID"(string), "META_NAME"(string), "META_TYPE"(string), "INST_SID"(string), "INST_TYPE"(string), "INST_NAME"(string), "INST_ALTERNATIVE_NAME"(string), "INST_DEFINITION"(string), "INST_REMARKS"(string), "INST_ABBREVIATION"(string), "INST_SYMBOL"(string), "INST_PC_ID"(string), "INST_SUBJECT_AREA_SID"(string), "INST_SUBJECT_AREA"(string), "INST_STATUS_SID"(string), "INST_STATUS"(string), "INST_DT_MODIFIED"(string), "INST_MODIFIED_BY"(string), "INST_FACT_EXPONENT"(string), "INST_LIN_MULTIPLIER"(string), "INST_LIN_OFFSET"(string), "INST_LOG_A"(string), "INST_LOG_B"(string), "INST_LOG_C"(string), "INST_CLASS_SID"(string), "INST_CLASS_NAME"(string), "INST_CHCN_ASS_SID"(string), "INST_DESCRIPTION"(string), "INST_VALUE"(string), "INST_UOM_SID"(string), "SELECTION"(string), "HierNumber"(string)
*/
class TG_EXPORT OdDgRDL_AttributesClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgRDL_AttributesClassInstance);

  virtual OdString getINST_UOM() const = 0;
  virtual void setINST_UOM(const OdString& strValue) = 0;

  virtual OdString getMETA_SID() const = 0;
  virtual void setMETA_SID(const OdString& strValue) = 0;

  virtual OdString getMETA_NAME() const = 0;
  virtual void setMETA_NAME(const OdString& strValue) = 0;

  virtual OdString getMETA_TYPE() const = 0;
  virtual void setMETA_TYPE(const OdString& strValue) = 0;

  virtual OdString getINST_SID() const = 0;
  virtual void setINST_SID(const OdString& strValue) = 0;

  virtual OdString getINST_TYPE() const = 0;
  virtual void setINST_TYPE(const OdString& strValue) = 0;

  virtual OdString getINST_NAME() const = 0;
  virtual void setINST_NAME(const OdString& strValue) = 0;

  virtual OdString getINST_ALTERNATIVE_NAME() const = 0;
  virtual void setINST_ALTERNATIVE_NAME(const OdString& strValue) = 0;

  virtual OdString getINST_DEFINITION() const = 0;
  virtual void setINST_DEFINITION(const OdString& strValue) = 0;

  virtual OdString getINST_REMARKS() const = 0;
  virtual void setINST_REMARKS(const OdString& strValue) = 0;

  virtual OdString getINST_ABBREVIATION() const = 0;
  virtual void setINST_ABBREVIATION(const OdString& strValue) = 0;

  virtual OdString getINST_SYMBOL() const = 0;
  virtual void setINST_SYMBOL(const OdString& strValue) = 0;

  virtual OdString getINST_PC_ID() const = 0;
  virtual void setINST_PC_ID(const OdString& strValue) = 0;

  virtual OdString getINST_SUBJECT_AREA_SID() const = 0;
  virtual void setINST_SUBJECT_AREA_SID(const OdString& strValue) = 0;

  virtual OdString getINST_SUBJECT_AREA() const = 0;
  virtual void setINST_SUBJECT_AREA(const OdString& strValue) = 0;

  virtual OdString getINST_STATUS_SID() const = 0;
  virtual void setINST_STATUS_SID(const OdString& strValue) = 0;

  virtual OdString getINST_STATUS() const = 0;
  virtual void setINST_STATUS(const OdString& strValue) = 0;

  virtual OdString getINST_DT_MODIFIED() const = 0;
  virtual void setINST_DT_MODIFIED(const OdString& strValue) = 0;

  virtual OdString getINST_MODIFIED_BY() const = 0;
  virtual void setINST_MODIFIED_BY(const OdString& strValue) = 0;

  virtual OdString getINST_FACT_EXPONENT() const = 0;
  virtual void setINST_FACT_EXPONENT(const OdString& strValue) = 0;

  virtual OdString getINST_LIN_MULTIPLIER() const = 0;
  virtual void setINST_LIN_MULTIPLIER(const OdString& strValue) = 0;

  virtual OdString getINST_LIN_OFFSET() const = 0;
  virtual void setINST_LIN_OFFSET(const OdString& strValue) = 0;

  virtual OdString getINST_LOG_A() const = 0;
  virtual void setINST_LOG_A(const OdString& strValue) = 0;

  virtual OdString getINST_LOG_B() const = 0;
  virtual void setINST_LOG_B(const OdString& strValue) = 0;

  virtual OdString getINST_LOG_C() const = 0;
  virtual void setINST_LOG_C(const OdString& strValue) = 0;

  virtual OdString getINST_CLASS_SID() const = 0;
  virtual void setINST_CLASS_SID(const OdString& strValue) = 0;

  virtual OdString getINST_CLASS_NAME() const = 0;
  virtual void setINST_CLASS_NAME(const OdString& strValue) = 0;

  virtual OdString getINST_CHCN_ASS_SID() const = 0;
  virtual void setINST_CHCN_ASS_SID(const OdString& strValue) = 0;

  virtual OdString getINST_DESCRIPTION() const = 0;
  virtual void setINST_DESCRIPTION(const OdString& strValue) = 0;

  virtual OdString getINST_VALUE() const = 0;
  virtual void setINST_VALUE(const OdString& strValue) = 0;

  virtual OdString getINST_UOM_SID() const = 0;
  virtual void setINST_UOM_SID(const OdString& strValue) = 0;

  virtual OdString getSELECTION() const = 0;
  virtual void setSELECTION(const OdString& strValue) = 0;

  virtual OdString getHierNumber() const = 0;
  virtual void setHierNumber(const OdString& strValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgRDL_AttributesClassInstance> OdDgRDL_AttributesClassInstancePtr;

//=============================================================================== 
//                SIUnitSystemDefaults
//=============================================================================== 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: SIUnitSystemDefaults, Version: 1.0, Class: UseSIUnitSystemDefaults
    Available properties: ---
*/
class TG_EXPORT OdDgUseSIUnitSystemDefaultsClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgUseSIUnitSystemDefaultsClassInstance);

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgUseSIUnitSystemDefaultsClassInstance> OdDgUseSIUnitSystemDefaultsClassInstancePtr;

//------------------------------------------------------------------------------- 


//=============================================================================== 
//                Unit_Attributes
//=============================================================================== 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: Unit_Attributes, Version: 1.0, Class: Dimension_Attributes
    Available properties: "DimensionName"(string), "DisplayName"(string), "ShortLabel"(string), "DimensionalDescription"(string), "DimensionalDerivation"(string)
*/
class TG_EXPORT OdDgDimension_AttributesClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgDimension_AttributesClassInstance);

  virtual OdString getDimensionName() const = 0;
  virtual void setDimensionName(const OdString& strValue) = 0;

  virtual OdString getDisplayName() const = 0;
  virtual void setDisplayName(const OdString& strValue) = 0;

  virtual OdString getShortLabel() const = 0;
  virtual void setShortLabel(const OdString& strValue) = 0;

  virtual OdString getDimensionalDescription() const = 0;
  virtual void setDimensionalDescription(const OdString& strValue) = 0;

  virtual OdString getDimensionalDerivation() const = 0;
  virtual void setDimensionalDerivation(const OdString& strValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgDimension_AttributesClassInstance> OdDgDimension_AttributesClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: Unit_Attributes, Version: 1.0, Class: DisplayUnitSpecification
    Available properties: "DisplayFormatString"(string), "DisplayFormatKey"(string), "DisplayUnitName"(string)
*/
class TG_EXPORT OdDgDisplayUnitSpecificationClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgDisplayUnitSpecificationClassInstance);

  virtual OdString getDisplayFormatString() const = 0;
  virtual void setDisplayFormatString(const OdString& strValue) = 0;

  virtual OdString getDisplayFormatKey() const = 0;
  virtual void setDisplayFormatKey(const OdString& strValue) = 0;

  virtual OdString getDisplayUnitName() const = 0;
  virtual void setDisplayUnitName(const OdString& strValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgDisplayUnitSpecificationClassInstance> OdDgDisplayUnitSpecificationClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: Unit_Attributes, Version: 1.0, Class: DisplayUnitSpecifications
    Available properties: "DisplayUnitSpecificationList"(array of DisplayUnitSpecification)
*/
class TG_EXPORT OdDgDisplayUnitSpecificationsClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgDisplayUnitSpecificationsClassInstance);

  virtual OdUInt32 getDisplayUnitSpecificationListItemCount() const = 0;
  virtual OdDgDisplayUnitSpecificationClassInstancePtr getDisplayUnitSpecificationListItem(OdUInt32 uIndex) const = 0;
  virtual void setDisplayUnitSpecificationListItem(OdUInt32 uIndex, const OdDgDisplayUnitSpecificationClassInstancePtr& value) = 0;
  virtual void addDisplayUnitSpecificationListItem(const OdDgDisplayUnitSpecificationClassInstancePtr& value) = 0;
  virtual void deleteDisplayUnitSpecificationListItem(OdUInt32 uIndex) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgDisplayUnitSpecificationsClassInstance> OdDgDisplayUnitSpecificationsClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: Unit_Attributes, Version: 1.0, Class: IsUnitSystemSchema
    Available properties: ---
*/
class TG_EXPORT OdDgIsUnitSystemSchemaClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgIsUnitSystemSchemaClassInstance);

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgIsUnitSystemSchemaClassInstance> OdDgIsUnitSystemSchemaClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: Unit_Attributes, Version: 1.0, Class: KindOfQuantity_Attributes
    Available properties: "DisplayName"(string), "ShortLabel"(string), "Description"(string), "Dimension"(string), "DimensionDisplayName"(string), "DimensionalDescription"(string)
*/
class TG_EXPORT OdDgKindOfQuantity_AttributesClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgKindOfQuantity_AttributesClassInstance);

  virtual OdString getDisplayName() const = 0;
  virtual void setDisplayName(const OdString& strValue) = 0;

  virtual OdString getShortLabel() const = 0;
  virtual void setShortLabel(const OdString& strValue) = 0;

  virtual OdString getDescription() const = 0;
  virtual void setDescription(const OdString& strValue) = 0;

  virtual OdString getDimension() const = 0;
  virtual void setDimension(const OdString& strValue) = 0;

  virtual OdString getDimensionDisplayName() const = 0;
  virtual void setDimensionDisplayName(const OdString& strValue) = 0;

  virtual OdString getDimensionalDescription() const = 0;
  virtual void setDimensionalDescription(const OdString& strValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgKindOfQuantity_AttributesClassInstance> OdDgKindOfQuantity_AttributesClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: Unit_Attributes, Version: 1.0, Class: Mixed_UnitSystem
    Available properties: ---
*/
class TG_EXPORT OdDgMixed_UnitSystemClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgMixed_UnitSystemClassInstance);

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgMixed_UnitSystemClassInstance> OdDgMixed_UnitSystemClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: Unit_Attributes, Version: 1.0, Class: SI_UnitSystem
    Available properties: ---
*/
class TG_EXPORT OdDgSI_UnitSystemClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgSI_UnitSystemClassInstance);

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgSI_UnitSystemClassInstance> OdDgSI_UnitSystemClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: Unit_Attributes, Version: 1.0, Class: Unit_Attributes
    Available properties: "Dimension"(string), "UnitSystem"(string), "Label"(string), "ShortLabel"(string), "ConversionType"(string), "BaseUnit"(string), "ConversionFactor"(double), "ConversionOffset"(double)
*/
class TG_EXPORT OdDgUnit_AttributesClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgUnit_AttributesClassInstance);

  virtual OdString getDimension() const = 0;
  virtual void setDimension(const OdString& strValue) = 0;

  virtual OdString getUnitSystem() const = 0;
  virtual void setUnitSystem(const OdString& strValue) = 0;

  virtual OdString getLabel() const = 0;
  virtual void setLabel(const OdString& strValue) = 0;

  virtual OdString getShortLabel() const = 0;
  virtual void setShortLabel(const OdString& strValue) = 0;

  virtual OdString getConversionType() const = 0;
  virtual void setConversionType(const OdString& strValue) = 0;

  virtual OdString getBaseUnit() const = 0;
  virtual void setBaseUnit(const OdString& strValue) = 0;

  virtual double getConversionFactor() const = 0;
  virtual void setConversionFactor(double dValue) = 0;

  virtual double getConversionOffset() const = 0;
  virtual void setConversionOffset(double dValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgUnit_AttributesClassInstance> OdDgUnit_AttributesClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: Unit_Attributes, Version: 1.0, Class: UnitSpecification
    Available properties: "DimensionName"(string), "KindOfQuantityName"(string), "UnitName"(string), "AllowableUnits"(array of string)
*/
class TG_EXPORT OdDgUnitSpecificationClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgUnitSpecificationClassInstance);

  virtual OdString getDimensionName() const = 0;
  virtual void setDimensionName(const OdString& strValue) = 0;

  virtual OdString getKindOfQuantityName() const = 0;
  virtual void setKindOfQuantityName(const OdString& strValue) = 0;

  virtual OdString getUnitName() const = 0;
  virtual void setUnitName(const OdString& strValue) = 0;

  virtual OdUInt32 getAllowableUnitsItemCount() const = 0;
  virtual OdString getAllowableUnitsItem(OdUInt32 uIndex) const = 0;
  virtual void setAllowableUnitsItem(OdUInt32 uIndex, const OdString& strValue) = 0;
  virtual void addAllowableUnitsItem(const OdString& strValue) = 0;
  virtual void deleteAllowableUnitsItem(OdUInt32 uIndex) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgUnitSpecificationClassInstance> OdDgUnitSpecificationClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: Unit_Attributes, Version: 1.0, Class: UnitSpecifications
    Available properties: "UnitSpecificationList"(array of UnitSpecification)
*/
class TG_EXPORT OdDgUnitSpecificationsClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgUnitSpecificationsClassInstance);

  virtual OdUInt32 getUnitSpecificationListItemCount() const = 0;
  virtual OdDgUnitSpecificationClassInstancePtr getUnitSpecificationListItem(OdUInt32 uIndex) const = 0;
  virtual void setUnitSpecificationListItem(OdUInt32 uIndex, const OdDgUnitSpecificationClassInstancePtr& value) = 0;
  virtual void addUnitSpecificationListItem(const OdDgUnitSpecificationClassInstancePtr& value) = 0;
  virtual void deleteUnitSpecificationListItem(OdUInt32 uIndex) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgUnitSpecificationsClassInstance> OdDgUnitSpecificationsClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: Unit_Attributes, Version: 1.0, Class: US_UnitSystem
    Available properties: ---
*/
class TG_EXPORT OdDgUS_UnitSystemClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgUS_UnitSystemClassInstance);

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgUS_UnitSystemClassInstance> OdDgUS_UnitSystemClassInstancePtr;

//=============================================================================== 
//                USCustomaryUnitSystemDefaults
//=============================================================================== 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: USCustomaryUnitSystemDefaults, Version: 1.0, Class: UseUSCustomaryUnitSystemDefaults
    Available properties: ---
*/
class TG_EXPORT OdDgUseUSCustomaryUnitSystemDefaultsClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgUseUSCustomaryUnitSystemDefaultsClassInstance);

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgUseUSCustomaryUnitSystemDefaultsClassInstance> OdDgUseUSCustomaryUnitSystemDefaultsClassInstancePtr;


//=============================================================================== 
//                DgnCustomAttributes
//=============================================================================== 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: DgnCustomAttributes, Version: 01.00, Class: IsDgnPlatformDeliveredSchema
    Available properties: "CanBeStored"(bool)
*/
class TG_EXPORT OdDgIsDgnPlatformDeliveredSchemaClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgIsDgnPlatformDeliveredSchemaClassInstance);

  virtual bool getCanBeStoredFlag() const = 0;
  virtual void setCanBeStoredFlag(bool bValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgIsDgnPlatformDeliveredSchemaClassInstance> OdDgIsDgnPlatformDeliveredSchemaClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: DgnCustomAttributes, Version: 01.00, Class: RelationshipIsNotStored
    Available properties: ---
    Description: Custom Attribute applied to relationships like the ones defined in DgnContentRelationshipSchema which are not stored in the repository but generated on demand. This is primarily intended to prevent DgnECPlugin from trying to do delete/update operations on these relationship instances
*/
class TG_EXPORT OdDgRelationshipIsNotStoredClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgRelationshipIsNotStoredClassInstance);

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgRelationshipIsNotStoredClassInstance> OdDgRelationshipIsNotStoredClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: DgnCustomAttributes, Version: 01.00, Class: DgnECClassAttributes
    Available properties: "HostType"(integer)
*/
class TG_EXPORT OdDgDgnECClassAttributesClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgDgnECClassAttributesClassInstance);

  virtual OdInt32 getHostType() const = 0;
  virtual void setHostType(OdInt32 iValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgDgnECClassAttributesClassInstance> OdDgDgnECClassAttributesClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: DgnCustomAttributes, Version: 01.00, Class: DgnECRelationshipAttributes
    Available properties: "SourceHostType"(integer), "TargetHostType"(integer), "SourceQueryFlags"(integer), "TargetQueryFlags"(integer)
*/
class TG_EXPORT OdDgDgnECRelationshipAttributesClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgDgnECRelationshipAttributesClassInstance);

  virtual OdInt32 getSourceHostType() const = 0;
  virtual void setSourceHostType(OdInt32 iValue) = 0;

  virtual OdInt32 getTargetHostType() const = 0;
  virtual void setTargetHostType(OdInt32 iValue) = 0;

  virtual OdInt32 getSourceQueryFlags() const = 0;
  virtual void setSourceQueryFlags(OdInt32 iValue) = 0;

  virtual OdInt32 getTargetQueryFlags() const = 0;
  virtual void setTargetQueryFlags(OdInt32 iValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgDgnECRelationshipAttributesClassInstance> OdDgDgnECRelationshipAttributesClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: DgnCustomAttributes, Version: 01.00, Class: DgnSymbologyOptions
    Available properties: "AllowNone"(bool), "NoneDisplayString"(string), "AllowByCell"(bool), "ByCellDisplayString"(string)
*/
class TG_EXPORT OdDgDgnSymbologyOptionsClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgDgnSymbologyOptionsClassInstance);

  virtual bool getAllowNoneFlag() const = 0;
  virtual void setAllowNoneFlag(bool bValue) = 0;

  virtual OdString getNoneDisplayString() const = 0;
  virtual void setNoneDisplayString(const OdString& strValue) = 0;

  virtual bool getAllowByCellFlag() const = 0;
  virtual void setAllowByCellFlag(bool bValue) = 0;

  virtual OdString getByCellDisplayString() const = 0;
  virtual void setByCellDisplayString(const OdString& strValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgDgnSymbologyOptionsClassInstance> OdDgDgnSymbologyOptionsClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: DgnCustomAttributes, Version: 01.00, Class: UnitDefinitionExtendedTypeOptions
    Available properties: "SupportsBaseMeter"(bool), "SupportsBaseDegree"(bool), "SupportsBaseNone"(bool), "SupportsSystemMetric"(bool), "SupportsSystemEnglish"(bool), "SupportsSystemUSSurvey"(bool), "SupportsSystemUndefined"(bool)
*/
class TG_EXPORT OdDgUnitDefinitionExtendedTypeOptionsClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgUnitDefinitionExtendedTypeOptionsClassInstance);

  virtual bool getSupportsBaseMeterFlag() const = 0;
  virtual void setSupportsBaseMeterFlag(bool bValue) = 0;

  virtual bool getSupportsBaseDegreeFlag() const = 0;
  virtual void setSupportsBaseDegreeFlag(bool bValue) = 0;

  virtual bool getSupportsBaseNoneFlag() const = 0;
  virtual void setSupportsBaseNoneFlag(bool bValue) = 0;

  virtual bool getSupportsSystemMetricFlag() const = 0;
  virtual void setSupportsSystemMetricFlag(bool bValue) = 0;

  virtual bool getSupportsSystemEnglishFlag() const = 0;
  virtual void setSupportsSystemEnglishFlag(bool bValue) = 0;

  virtual bool getSupportsSystemUSSurveyFlag() const = 0;
  virtual void setSupportsSystemUSSurveyFlag(bool bValue) = 0;

  virtual bool getSupportsSystemUndefinedFlag() const = 0;
  virtual void setSupportsSystemUndefinedFlag(bool bValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgUnitDefinitionExtendedTypeOptionsClassInstance> OdDgUnitDefinitionExtendedTypeOptionsClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: DgnCustomAttributes, Version: 01.00, Class: DgnStyleOptions
    Available properties: "AllowNone"(integer), "NoneDisplayString"(string)
*/
class TG_EXPORT OdDgDgnStyleOptionsClassInstance : public OdDgECClassInstance
{
public:
  enum OdDgDgnStyleOptionsAllowNoneType
  {
    kDefault = 0,
    kAlways  = 1,
    kNever   = 2
  };
public:
  ODRX_DECLARE_MEMBERS(OdDgDgnStyleOptionsClassInstance);

  virtual OdDgDgnStyleOptionsAllowNoneType getAllowNone() const = 0;
  virtual void setAllowNone(OdDgDgnStyleOptionsAllowNoneType iValue) = 0;

  virtual OdString getNoneDisplayString() const = 0;
  virtual void setNoneDisplayString(const OdString& strValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgDgnStyleOptionsClassInstance> OdDgDgnStyleOptionsClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: DgnCustomAttributes, Version: 01.00, Class: ItemDisplayAttribute
    Available properties: "ItemIsGraphical"(bool)
*/
class TG_EXPORT OdDgItemDisplayAttributeClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgItemDisplayAttributeClassInstance);

  virtual bool getItemIsGraphicalFlag() const = 0;
  virtual void setItemIsGraphicalFlag(bool bValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgItemDisplayAttributeClassInstance> OdDgItemDisplayAttributeClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: DgnCustomAttributes, Version: 01.00, Class: PropertyDependencyCondition
    Available properties: ---
*/
class TG_EXPORT OdDgPropertyDependencyConditionClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgPropertyDependencyConditionClassInstance);

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgPropertyDependencyConditionClassInstance> OdDgPropertyDependencyConditionClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: DgnCustomAttributes, Version: 01.00, Class: AnyAreBoundPropertyDependencyCondition
    Available properties: "PropertyNames"(array of string)
*/
class TG_EXPORT OdDgAnyAreBoundPropertyDependencyConditionClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgAnyAreBoundPropertyDependencyConditionClassInstance);

  virtual OdUInt32 getPropertyNamesItemCount() const = 0;
  virtual OdString getPropertyNamesItem(OdUInt32 uIndex) const = 0;
  virtual void setPropertyNamesItem(OdUInt32 uIndex, const OdString& strValue) = 0;
  virtual void addPropertyNamesItem(const OdString& strValue) = 0;
  virtual void deletePropertyNamesItem(OdUInt32 uIndex) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgAnyAreBoundPropertyDependencyConditionClassInstance> OdDgAnyAreBoundPropertyDependencyConditionClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: DgnCustomAttributes, Version: 01.00, Class: DoesNotSupportPropertyDependency
    Available properties: "Conditions"(array of PropertyDependencyCondition)
*/
class TG_EXPORT OdDgDoesNotSupportPropertyDependencyClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgDoesNotSupportPropertyDependencyClassInstance);

  virtual OdUInt32 getConditionsItemCount() const = 0;
  virtual OdDgPropertyDependencyConditionClassInstancePtr getConditionsItem(OdUInt32 uIndex) const = 0;
  virtual void setConditionsItem(OdUInt32 uIndex, const OdDgPropertyDependencyConditionClassInstancePtr& value) = 0;
  virtual void addConditionsItem(const OdDgPropertyDependencyConditionClassInstancePtr& value) = 0;
  virtual void deleteConditionsItem(OdUInt32 uIndex) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgDoesNotSupportPropertyDependencyClassInstance> OdDgDoesNotSupportPropertyDependencyClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: DgnCustomAttributes, Version: 01.00, Class: UndisplayedCustomProperty
    Available properties: ---
*/
class TG_EXPORT OdDgUndisplayedCustomPropertyClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgUndisplayedCustomPropertyClassInstance);

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgUndisplayedCustomPropertyClassInstance> OdDgUndisplayedCustomPropertyClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: DgnCustomAttributes, Version: 01.00, Class: ItemTypeLibraryComponentAttributes
    Available properties: "ID"(integer)
*/
class TG_EXPORT OdDgItemTypeLibraryComponentAttributesClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgItemTypeLibraryComponentAttributesClassInstance);

  virtual OdInt32 getID() const = 0;
  virtual void setID(OdInt32 iValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgItemTypeLibraryComponentAttributesClassInstance> OdDgItemTypeLibraryComponentAttributesClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: DgnCustomAttributes, Version: 01.00, Class: UpgradedTagSetAttributes
    Available properties: "SetName"(string)
*/
class TG_EXPORT OdDgUpgradedTagSetAttributesClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgUpgradedTagSetAttributesClassInstance);

  virtual OdString getSetName() const = 0;
  virtual void setSetName(const OdString& strValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgUpgradedTagSetAttributesClassInstance> OdDgUpgradedTagSetAttributesClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: DgnCustomAttributes, Version: 01.00, Class: UpgradedTagDefAttributes
    Available properties: "ID"(integer)
*/
class TG_EXPORT OdDgUpgradedTagDefAttributesClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgUpgradedTagDefAttributesClassInstance);

  virtual OdInt32 getID() const = 0;
  virtual void setID(OdInt32 iValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgUpgradedTagDefAttributesClassInstance> OdDgUpgradedTagDefAttributesClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: DgnCustomAttributes, Version: 01.00, Class: DisplayRulesOptions
    Available properties: "SuppressGenerateByRange"(bool), "GenerateByRangeDefaultMin"(double), "GenerateByRangeDefaultMax"(double)
*/
class TG_EXPORT OdDgDisplayRulesOptionsClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgDisplayRulesOptionsClassInstance);

  virtual bool getSuppressGenerateByRangeFlag() const = 0;
  virtual void setSuppressGenerateByRangeFlag(bool bValue) = 0;

  virtual double getGenerateByRangeDefaultMin() const = 0;
  virtual void setGenerateByRangeDefaultMin(double dValue) = 0;

  virtual double getGenerateByRangeDefaultMax() const = 0;
  virtual void setGenerateByRangeDefaultMax(double dValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgDisplayRulesOptionsClassInstance> OdDgDisplayRulesOptionsClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: DgnCustomAttributes, Version: 01.00, Class: ItemIdentifiesHost
    Available properties: ---
*/
class TG_EXPORT OdDgItemIdentifiesHostClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgItemIdentifiesHostClassInstance);

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgItemIdentifiesHostClassInstance> OdDgItemIdentifiesHostClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: DgnCustomAttributes, Version: 01.00, Class: PresentationCapabilityRequirement
    Available properties: "Capability"(integer), "ShowReadOnly"(bool), "DefaultModelSpecialBehavior"(bool), "AdditionalCapabilities"(array of integer)
*/
class TG_EXPORT OdDgPresentationCapabilityRequirementClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgPresentationCapabilityRequirementClassInstance);

  virtual OdInt32 getCapability() const = 0;
  virtual void setCapability(OdInt32 iValue) = 0;

  virtual bool getShowReadOnlyFlag() const = 0;
  virtual void setShowReadOnlyFlag(bool bValue) = 0;

  virtual bool getDefaultModelSpecialBehaviorFlag() const = 0;
  virtual void setDefaultModelSpecialBehaviorFlag(bool bValue) = 0;

  virtual OdUInt32 getAdditionalCapabilitiesItemCount() const = 0;
  virtual OdInt32 getAdditionalCapabilitiesItem(OdUInt32 uIndex) const = 0;
  virtual void setAdditionalCapabilitiesItem(OdUInt32 uIndex, OdInt32 iValue) = 0;
  virtual void addAdditionalCapabilitiesItem(OdInt32 iValue) = 0;
  virtual void deleteAdditionalCapabilitiesItem(OdUInt32 uIndex) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgPresentationCapabilityRequirementClassInstance> OdDgPresentationCapabilityRequirementClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: DgnCustomAttributes, Version: 01.00, Class: ECXDPersistenceHandler
    Available properties: "HandlerMajorId"(integer), "HandlerMinorId"(integer)
*/
class TG_EXPORT OdDgECXDPersistenceHandlerClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgECXDPersistenceHandlerClassInstance);

  virtual OdInt32 getHandlerMajorId() const = 0;
  virtual void setHandlerMajorId(OdInt32 iValue) = 0;

  virtual OdInt32 getHandlerMinorId() const = 0;
  virtual void setHandlerMinorId(OdInt32 iValue) = 0;

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgECXDPersistenceHandlerClassInstance> OdDgECXDPersistenceHandlerClassInstancePtr;

//------------------------------------------------------------------------------- 

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>

    \remarks
    Schema: DgnCustomAttributes, Version: 01.00, Class: IsDwgAttributeDefinitionsSchema
    Available properties: ---
*/
class TG_EXPORT OdDgIsDwgAttributeDefinitionsSchemaClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgIsDwgAttributeDefinitionsSchemaClassInstance);

public:
  static OdDgECClassReference getDefaultClassReference();
};

//------------------------------------------------------------------------------- 

typedef OdSmartPtr<OdDgIsDwgAttributeDefinitionsSchemaClassInstance> OdDgIsDwgAttributeDefinitionsSchemaClassInstancePtr;

//------------------------------------------------------------------------------- 

#include "TD_PackPop.h"

#endif // __DG_ECDEFAULTCLASSINSTANCE_H__
