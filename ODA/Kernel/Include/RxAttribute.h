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

#ifndef _ODRXATTRIBUTE_INCLUDED_
#define _ODRXATTRIBUTE_INCLUDED_

#include "OdString.h"
#include "OdArray.h"
#include "RxValue.h"
#include "RxProperty.h"

  //DOM-IGNORE-BEGIN
class OdRxAttributeImpl;
class OdRxAttributeCollectionImpl;
class OdRxMember;
class OdRxPropertyBase;
//DOM-IGNORE-END

/** <group OdRx_Classes>
  \details
  This is the base interface class for all attributes.
*/
class ODRX_ABSTRACT FIRSTDLL_EXPORT OdRxAttribute : public OdRxObject
{
public:
  //DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdRxAttribute);
  //DOM-IGNORE-END

  /** \details
    Default destructor for objects of the OdRxAttribute class.
  */
  virtual ~OdRxAttribute();
protected:
  //DOM-IGNORE-BEGIN
  OdRxAttribute(OdRxAttributeImpl*);
  friend class OdRxAttributeImpl;
  OdRxAttributeImpl* m_pImpl;
  //DOM-IGNORE-END
};

/** \details
  Smart pointer to objects of the <link OdRxAttribute, OdRxAttribute> class.
*/
typedef OdSmartPtr<OdRxAttribute>OdRxAttributePtr;

/** <group OdRx_Classes>
  \details
  Container for the attributes of a member (OdRxMember).
  Attribute collection may have only one attribute of the given class.
*/
class FIRSTDLL_EXPORT OdRxAttributeCollection
{
  //DOM-IGNORE-BEGIN
  friend class OdRxAttributeCollectionImpl;
  OdRxAttributeCollectionImpl* m_pImpl;
  //DOM-IGNORE-END
public:
  /** \details
    Default constructor for objects of the OdRxAttributeCollection class.
  */
  OdRxAttributeCollection();

  /** \details
    Default destructor for objects of the OdRxAttributeCollection class.
  */
  ~OdRxAttributeCollection();
  
  /** \details
    Get the number of elements in the collection.

    \returns
    Number of elements in the collection.
  */
  int count() const;
  
  /** \details
    Retrieve the attribute at the index (constant version).

    \param index [in]  Index of the attribute to be retrieved.

    \returns
    Pointer to the attribute at the given index.
  */
  const OdRxAttribute* getAt(int index) const;
  
  /** \details
    Retrieve the attribute at the index (non-constant version).

    \param index [in]  Index of the attribute to be retrieved.

    \returns
    Pointer to the attribute at the given index.
  */
  OdRxAttribute* getAt(int index);

  /** \details
    Finds the attribute of the given class in the collection (constant version).

    \param type [in]  Class of the attribute to be found.

    \returns
    Pointer to the attribute of the given class. Returns 0 if not found.
  */
  const OdRxAttribute* get(const OdRxClass* type) const;

  /** \details
    Finds the attribute of the given class in the collection.

    \param type [in]  Class of the attribute to be found.

    \returns
    Pointer to the attribute of the given class. Returns 0 if not found.
  */
  OdRxAttribute* get(const OdRxClass* type);

  /** \details
    Add an attribute to the collection. If the collection already has an
    attribute of this class, it will not be replaced.

    \param attribute [in]  Attribute to be added to the collection.

    \returns
    eOk if operation was successful; eInvalidInput if the attribute parameter is
    invalid; eDuplicateKey if the collection already has an attribute of this
    type.
  */
  OdResult add(const OdRxAttribute* attribute);
  
  /** \details
    Add an attribute to the collection. If the collection already has an
    attribute of this class, it will be replaced.

    \param attribute [in]  Attribute to be added to the collection.

    \returns
    eOk if operation was successful; eInvalidInput if the attribute parameter is
    invalid.
  */
  OdResult override(const OdRxAttribute* attribute);

  /** \details
    Removes the attribute of that type from the collection.

    \param attribute [in]  Attribute to be removed from the collection.

    \returns
    eOk if successful or eKeyNotFound otherwise.
  */
  OdResult remove(const OdRxAttribute* attribute);
};

/** <group OdRx_Classes>
  \details
  Protocol extension to handle loading strings from resources.
  (E.g. for localization.)
*/
class ODRX_ABSTRACT FIRSTDLL_EXPORT OdRxResourceLoader : public OdRxObject
{
protected:

  /** \details
    Load the string identified by the ID and a hint. Implementation is up to the
    application.

    \param id         [in]  Resource ID of the string.
    \param sourceHint [in]  Resource ID hint.
    \param result    [out]  Loaded string.

    \returns
    Result of the operation. Depends on implementation.
  */
  virtual OdResult subLoadString(unsigned int id, unsigned int sourceHint, OdString& result) = 0;
public:
  //DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdRxResourceLoader);
  //DOM-IGNORE-END

  /** \details
    Load the string identified by the ID and a hint. Queries the class for
    OdRxResourceLoader and calls subLoadString() to do the actual loading.

    \param pClass     [in]  Class to be queried.
    \param id         [in]  Resource ID of the string.
    \param sourceHint [in]  Resource ID hint.
    \param result    [out]  Loaded string.

    \returns
    Result of the subLoadString() call for the queried class or eNotImplemented
    if the subLoadString() is not implemented in the queried class.
  */
  static OdResult loadString(const OdRxObject* pClass, unsigned int id, unsigned int sourceHint, OdString& result);
};

//DOM-IGNORE-BEGIN
class OdRxLocalizedNameAttributeImpl;
//DOM-IGNORE-END

/** <group OdRx_Classes>
  \details
  Provides a human readable name of the member for UI.
  Uses OdRxResourceLoader to get the actual name.
*/
class FIRSTDLL_EXPORT OdRxLocalizedNameAttribute : public OdRxAttribute
{
public:
  //DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdRxLocalizedNameAttribute);
  //DOM-IGNORE-END
  
  /** \details
    Construct an instance of the attribute to store a string identifier.

    \param id         [in]  ID of the string.
    \param sourceHint [in]  Resource ID hint.

    \returns
    Smart pointer to a constructed attribute object.
  */
  static OdRxAttributePtr createObject(unsigned int id, unsigned int sourceHint = 0);
  
  /** \details
    Looks up OdRxLocalizedNameAttribute on the object if it is a member, (and in
    the object class attribute list if it is not), and uses OdRxResourceLoader
    to get the string from resources identified by ID and hint.

    \param pO [in]  Object to get localized name of.

    \returns
    Localized name of the object if operation was successful; empty string
    otherwise.
  */
  static OdString getLocalizedName(const OdRxObject* pO);
  
  /** \details
    Retrieves a name string resource identifier.

    \returns
    Name string resource identifier.
  */
  unsigned int id() const;
  
  /** \details
    Retrieves a name string resource identifier hint.

    \returns
    Name string resource identifier hint. Zero if not set.
  */
  unsigned int sourceHint() const;
protected:
  //DOM-IGNORE-BEGIN
  OdRxLocalizedNameAttribute(OdRxLocalizedNameAttributeImpl*);
  //DOM-IGNORE-END
};

//DOM-IGNORE-BEGIN
class OdRxAlternateLocalizedNameAttributeImpl;
//DOM-IGNORE-END

/** <group OdRx_Classes>
  \details
  Same as OdRxLocalizedNameAttribute, alternative for different contexts (up to
  the application).
*/
class FIRSTDLL_EXPORT OdRxAlternateLocalizedNameAttribute : public OdRxLocalizedNameAttribute
{
public:
  //DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdRxAlternateLocalizedNameAttribute);
  //DOM-IGNORE-END

  /** \details
    Constructor. Creates an instance of the attribute to store a string
    identifier.

    \param id         [in]  Resource ID of the string.
    \param sourceHint [in]  Resource ID hint.

    \returns
    Smart pointer to a constructed attribute object.
  */
  static OdRxAttributePtr createObject(unsigned int id, unsigned int sourceHint = 0);

  //DOM-IGNORE-BEGIN
  OdRxAlternateLocalizedNameAttribute(OdRxAlternateLocalizedNameAttributeImpl* impl);
  //DOM-IGNORE-END
};

//DOM-IGNORE-BEGIN
class OdRxUiPlacementAttributeImpl;
//DOM-IGNORE-END

/** <group OdRx_Classes>
  \details
  Used by property palettes, to group properties by category and sort by weight.
*/
class FIRSTDLL_EXPORT OdRxUiPlacementAttribute : public OdRxAttribute
{
public:
  //DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdRxUiPlacementAttribute);
  OdRxUiPlacementAttribute(OdRxUiPlacementAttributeImpl*);
  //DOM-IGNORE-END

  /** \details
    Constructor. Creates the attribute to store UI category and weight.

    \param category [in]  Category name.
    \param weight   [in]  Weight.

    \returns
    Smart pointer to the created attribute.
  */
  static OdRxAttributePtr createObject(const OdString& category, unsigned int weight);

  /** \details
    Retrieves the UI category for this member.

    \param member [in]  Member to get category of.

    \returns
    Category name.
  */
  static OdString getCategory(const OdRxObject* member);

  /** \details
    Returns the UI weight for this member.

    \param member [in]  Member to get weight of.

    \returns
    Weight as float value.
  */
  static float getWeight(const OdRxObject* member);
};

//DOM-IGNORE-BEGIN
class OdRxLMVAttributeImpl;
class FIRSTDLL_EXPORT OdRxLMVAttribute : public OdRxAttribute
{
public:
  ODRX_DECLARE_MEMBERS(OdRxLMVAttribute);
  OdRxLMVAttribute(OdRxLMVAttributeImpl*);
};
//DOM-IGNORE-END

//DOM-IGNORE-BEGIN
class OdRxDescriptionAttributeImpl;
//DOM-IGNORE-END

/** <group OdRx_Classes>
  \details
  This attribute provides a human readable description of the member for UI.
*/
class FIRSTDLL_EXPORT OdRxDescriptionAttribute : public OdRxAttribute
{
public:
  //DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdRxDescriptionAttribute);
  OdRxDescriptionAttribute(OdRxDescriptionAttributeImpl*);
  //DOM-IGNORE-END

  /** \details
    Constructor. Creates an attribute to store a description string resource ID
    and a resource ID hint.

    \param description [in]  Description string.
    \param id          [in]  Resource ID of the string.
    \param sourceHint  [in]  Resource ID hint.

    \returns
    Smart pointer to a constructed attribute object.
  */
  static OdRxAttributePtr createObject(const OdString& description, unsigned int id = 0, unsigned int sourceHint = 0);
  
  /** \details
    Looks up OdRxDescriptionAttribute on the object if it is a member, (and in
    the object class attribute list if it is not), and uses OdRxResourceLoader
    to get the string from resources identified by ID and hint.

    \param pO [in]  Object to be queried for a description.

    \returns
    A description string if found; empty string otherwise.
  */
  static OdString getDescription(const OdRxObject* pO);
  
  /** \details
    Gets the description string resource identifier.

    \returns
    Description string resource identifier.
  */
  unsigned int id() const;

  /** \details
    Gets the description string resource identifier hint.

    \returns 
    Description string resource identifier hint. Zero if not set.
  */
  unsigned int sourceHint() const;
};

/** \details
  Callback type to customize property retrieval in
  OdRxRefersToAttribute::parseReference().
*/
typedef OdRxPropertyBasePtr (*FindPropertyCallback)(const OdRxObject* pObject, const OdString& pszPropName);

/** \details
  Callback type to optionally convert some known property values in
  OdRxRefersToAttribute::parseReference().
*/
typedef bool(*ConvertValueCallback)(const OdRxPropertyBase* pProperty, OdRxValue& value);

//DOM-IGNORE-BEGIN
class OdRxRefersToAttributeImpl;
//DOM-IGNORE-END

/** <group OdRx_Classes>
  \details
  This attribute is used to provide additional information about properties
  linked with this one. Usually contained item with the container,
  like "/BlockTableId/Items" for OdDbEntity "BlockId" property.
*/
class FIRSTDLL_EXPORT OdRxRefersToAttribute : public OdRxAttribute
{
public:
//DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdRxRefersToAttribute);
//DOM-IGNORE-END

  /** \details
    Constructor. Creates an attribute containing the provided path.

    \param path [in]  Path to the related property (starting with database).
                      E.g. "/BlockTableId/Items" for OdDbEntity "BlockId" property.
    \returns
    Smart pointer to a constructed attribute object.
  */
  static OdRxAttributePtr createObject(const OdString& path);

  /** \details
    Gets the path to the related property.

    \returns
    Path to the related property (starting with database).
    E.g. "/BlockTableId/Items" for OdDbEntity "BlockId" property.
  */
  OdString path() const;

  /** \details
    Parses the reference path returning the last property. E.g. for
    "/BlockTableId/Items" it would be "Items" property of the block table.

    \param path    [in]  Path to the property. Intermediate names are separated by '/'.
                         All the properties but the last should be dereferenceable (type().isReference() == true).
    \param pObject [in]  The object to start with. Usually database, for paths starting with '/'.
    \param find    [in]  Optional callback to customize the property retrieval. OdRxMemberQueryEngine::find() is used by default.
    \param convert [in]  Optional callback to convert the property value to another type.

    \returns
    The last property in the path.
  */
  static const OdRxPropertyBasePtr parseReference(const OdString& path, const OdRxObject* pObject, FindPropertyCallback find = NULL, ConvertValueCallback convert = NULL);
protected:
  //DOM-IGNORE-BEGIN
  OdRxRefersToAttribute(OdRxRefersToAttributeImpl*);
  //DOM-IGNORE-END
};

//DOM-IGNORE-BEGIN
class OdRxCOMAttributeImpl;
//DOM-IGNORE-END

/** <group OdRx_Classes>
  \details
  Provides the name of the corresponding COM property if it is different.
  For example: "Transparency" (non-COM) / "EntityTransparency" (COM).
*/
class FIRSTDLL_EXPORT OdRxCOMAttribute : public OdRxAttribute
{
public:
  //DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdRxCOMAttribute);
  //DOM-IGNORE-END
  /** \details
    Constructor. Creates an attribute containing the corresponding COM property.

    \param name [in]  Name of the corresponding COM property.

    \returns
    Smart pointer to a constructed attribute object.
  */
  static OdRxAttributePtr createObject(const OdString& name);

  /** \details
    Gets the name of the corresponding COM property.

    \returns
    Name of the corresponding COM property.
  */
  OdString name() const;
protected:
  //DOM-IGNORE-BEGIN
  OdRxCOMAttribute(OdRxCOMAttributeImpl*);
  //DOM-IGNORE-END
};

//DOM-IGNORE-BEGIN
class OdRxFilePathAttributeImpl;
//DOM-IGNORE-END

/** <group OdRx_Classes>
  \details
  Marks properties containing a file path.
*/
class FIRSTDLL_EXPORT OdRxFilePathAttribute : public OdRxAttribute
{
public:
  //DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdRxFilePathAttribute);
protected:
  OdRxFilePathAttribute(OdRxFilePathAttributeImpl*);
  //DOM-IGNORE-END
};

//DOM-IGNORE-BEGIN
class OdRxFlagsAttributeImpl;
//DOM-IGNORE-END

/** <group OdRx_Classes>
  \details
  This attributes marks the properties that contain a bitwise flag collection.
*/
class FIRSTDLL_EXPORT OdRxFlagsAttribute : public OdRxAttribute
{
public:
  //DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdRxFlagsAttribute);
protected:
  OdRxFlagsAttribute(OdRxFlagsAttributeImpl*);
  //DOM-IGNORE-END
};

//DOM-IGNORE-BEGIN
class OdRxUnitTypeAttributeImpl;
//DOM-IGNORE-END

/** <group OdRx_Classes>
  \details
  This attribute represents unit types for properties (value type would be just
  'double').
*/
class FIRSTDLL_EXPORT OdRxUnitTypeAttribute : public OdRxAttribute
{
public:

  /** \details
    Defines possible unit types.
  */
  enum UnitType
  {
    kUnitless = 0,                      //Unitless
    kDistance = (0x1 << 0),             //Distance
    kAngle = (0x1 << 1),                //Angle
    kArea = (0x1 << 2),                 //Area
    kVolume = (0x1 << 3),               //Volume
    kCurrency = (0x1 << 4),             //Currency
    kPercentage = (0x1 << 5),           //Percentage
    kAngleNotTransformed = (0x1 << 16), //Not transformed angle
  };

  //DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdRxUnitTypeAttribute);
  //DOM-IGNORE-END
  
  /** \details
    Constructor. Creates an attribute containing a unit type for properties.

    \param unitType [in]  Unit type.

    \returns
    Smart pointer to a constructed attribute object.
  */
  static OdRxAttributePtr createObject(UnitType unitType);

  /** \details
    Gets the property unit type.

    \returns
    Unit type.
  */
  UnitType unitType() const;
protected:
  //DOM-IGNORE-BEGIN
  OdRxUnitTypeAttribute(OdRxUnitTypeAttributeImpl*);
  //DOM-IGNORE-END
};

//DOM-IGNORE-BEGIN
class OdRxUseDialogForReferredCollectionAttributeImpl;

/**
  <group OdRx_Classes>
*/
class FIRSTDLL_EXPORT OdRxUseDialogForReferredCollectionAttribute : public OdRxAttribute
{
public:
  ODRX_DECLARE_MEMBERS(OdRxUseDialogForReferredCollectionAttribute);
protected:
  OdRxUseDialogForReferredCollectionAttribute(OdRxUseDialogForReferredCollectionAttributeImpl*);
};

class OdRxUiCascadingContextMenuAttributeImpl;
/**
  <group OdRx_Classes>
*/
class FIRSTDLL_EXPORT OdRxUiCascadingContextMenuAttribute : public OdRxAttribute
{
public:
  ODRX_DECLARE_MEMBERS(OdRxUiCascadingContextMenuAttribute);
protected:
  OdRxUiCascadingContextMenuAttribute(OdRxUiCascadingContextMenuAttributeImpl*);
};
//DOM-IGNORE-END

//DOM-IGNORE-BEGIN
class OdRxCumulativeAttributeImpl;
//DOM-IGNORE-END

/** <group OdRx_Classes>
  \details
  This attribute marks properties which values can be accumulated in case of
  multiple selection, like area, length, etc.
*/
class FIRSTDLL_EXPORT OdRxCumulativeAttribute : public OdRxAttribute
{
public:
  //DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdRxCumulativeAttribute);
  //DOM-IGNORE-END

  /** \details
    Constructor. Creates an attribute that marks properties with cumulative
    values.

    \param type [in]  String specifying the type of cumulative value ("area", "length").

    \returns
    Smart pointer to a constructed attribute object.
  */
  static OdRxAttributePtr createObject(const OdString& type);

  /** \details
    Gets the string specifying the type of cumulative value.

    \returns
    String specifying the type of cumulative value ("area", "length").
  */
  OdString type() const;
protected:
  //DOM-IGNORE-BEGIN
  OdRxCumulativeAttribute(OdRxCumulativeAttributeImpl*);
  //DOM-IGNORE-END
};

//DOM-IGNORE-BEGIN
class OdRxAffinityAttributeImpl;
//DOM-IGNORE-END

/** <group OdRx_Classes>
  \details
  This attribute defines which additional objects need to be packed with the
  main object to get a propery, marked by this attribute. E.g. if the size of
  the object depends on the viewport, the viewport should be packed with the
  main object.

  \remarks
  This attribute is for internal use.
*/
class FIRSTDLL_EXPORT OdRxAffinityAttribute : public OdRxAttribute
{
public:
  //DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdRxAffinityAttribute);
  //DOM-IGNORE-END

  /**
    \details
    Constructor. Creates an attribute of the OdRxAffinityAttribute type.

    \param name [in]  Names of additional objects.

    \returns
    Smart pointer to a constructed attribute object.
  */
  static OdRxAttributePtr createObject(const OdString& name);

  /**
    \details
    Gets the list of names of additional objects.

    \returns
    List of names of additional objects.
  */
  OdString name() const;
protected:
  //DOM-IGNORE-BEGIN
  OdRxAffinityAttribute(OdRxAffinityAttributeImpl*);
  //DOM-IGNORE-END
};

//DOM-IGNORE-BEGIN
class OdRxTypePromotionAttributeImpl;
//DOM-IGNORE-END

/** <group OdRx_Classes>
  \details
  This attribute defines which subproperties to show besides the property in the
  promotion mode with OdRxPromotingQueryContext.
*/
class FIRSTDLL_EXPORT OdRxTypePromotionAttribute : public OdRxAttribute
{
public:
  //DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdRxTypePromotionAttribute);
  //DOM-IGNORE-END

  /** \details
    Constructor. Creates an attribute that defines which subproperties to show
    besides the property in the promotion mode with OdRxPromotingQueryContext.

    \param properties [in]  List of subproperties.

    \returns
    Smart pointer to a constructed attribute object.
  */
  static OdRxAttributePtr createObject(const OdString& properties);

  /** \details
    Gets the list of subproperties.

    \returns
    List of subproperties.
  */
  OdString properties() const;
protected:
  //DOM-IGNORE-BEGIN
  OdRxTypePromotionAttribute(OdRxTypePromotionAttributeImpl*);
  //DOM-IGNORE-END
};

//DOM-IGNORE-BEGIN
class OdRxUnionTypeAttributeImpl;
//DOM-IGNORE-END

/** <group OdRx_Classes>
  \details
  This attribute defines possible types of a property, if it returns values of
  types void or OdRxObjectPtr.
*/
class FIRSTDLL_EXPORT OdRxUnionTypeAttribute : public OdRxAttribute
{
public:
  //DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdRxUnionTypeAttribute);
  //DOM-IGNORE-END

  /** \details
    Constructor. Creates an attribute that defines possible types of a property,
    if it returns values of types void or OdRxObjectPtr.

    \param typenames [in]  Semicolon separated list of possible types.

    \returns
    Smart pointer to a constructed attribute object.
  */
  static OdRxAttributePtr createObject(const OdString& typenames);

  /** \details
    Gets the list of possible types.

    \returns
    Semicolon separated list of possible types.
  */
  OdString typenames() const;
protected:
  //DOM-IGNORE-BEGIN
  OdRxUnionTypeAttribute(OdRxUnionTypeAttributeImpl*);
  //DOM-IGNORE-END
};

//DOM-IGNORE-BEGIN
class OdRxDefaultValueAttributeImpl;
//DOM-IGNORE-END

/** \details
  Default value of this property, e.g. for resetting. Optional.

  <group OdRx_Classes>
*/
class FIRSTDLL_EXPORT OdRxDefaultValueAttribute : public OdRxAttribute
{
public:
  //DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdRxDefaultValueAttribute);
  //DOM-IGNORE-END

  /** \details
    Constructor. Creates an attribute containing the default value for the
    property.

    \param value [in]  Default value of the property.

    \returns
    Smart pointer to a constructed attribute object.
  */
  static OdRxAttributePtr createObject(const OdRxValue& value);

  /** \details
    Gets the default value of the property.

    \returns
    Default value of the property.
  */
  const OdRxValue& value() const;
protected:
  //DOM-IGNORE-BEGIN
  OdRxDefaultValueAttribute(OdRxDefaultValueAttributeImpl*);
  //DOM-IGNORE-END
};

//DOM-IGNORE-BEGIN
class OdRxHierarchyLevelAttributeImpl;
//DOM-IGNORE-END

/** \details
  Marks this property as a tree node in 'Common Data Access' API data structure.

  <group OdRx_Classes>
*/
class FIRSTDLL_EXPORT OdRxHierarchyLevelAttribute : public OdRxAttribute
{
public:
  //DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdRxHierarchyLevelAttribute);
  //DOM-IGNORE-END

  /** \details
    Constructor. Creates an attribute of the OdRxHierarchyLevelAttribute type.

    \param value [in]  String describing CDA hierarchy level of this node.

    \returns
    Smart pointer to a constructed attribute object.
  */
  static OdRxAttributePtr createObject(const OdString& value);

  /** \details
    Retrieves a string describing CDA hierarchy level of this node.

    \returns
    String describing CDA hierarchy level of this node (e.g. 'Model', 'Block').
  */
  OdString defaultValue();

  /** \details
    Retrieves a string describing CDA hierarchy level of the specified value.

    \returns
    String describing CDA hierarchy level of this node (e.g. 'Model', 'Block').
  */
  virtual OdString value(OdRxValue& value);

  //DOM-IGNORE-BEGIN
  OdRxHierarchyLevelAttribute(OdRxHierarchyLevelAttributeImpl*);
  OdRxHierarchyLevelAttribute();
  //DOM-IGNORE-END
};

//DOM-IGNORE-BEGIN
class OdRxDisplayAsAttributeImpl;
//DOM-IGNORE-END

/** \details
  This attribute can be used by UI to represent properties of dereferenceable
  types in human readable form (instead of displaying just a handle).

  <group OdRx_Classes>
*/
class FIRSTDLL_EXPORT OdRxDisplayAsAttribute : public OdRxAttribute
{
public:
  //DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdRxDisplayAsAttribute);
  //DOM-IGNORE-END

  /** \details
    Constructor. Creates an attribute of the OdRxDisplayAsAttribute type.

    \param property_name [in]  The name of the property of the dereferenced
                               object to use instead of the object ID.

    \returns
    Smart pointer to a constructed attribute object.
  */
  static OdRxAttributePtr createObject(const OdString& property_name);

  /** \details
    Retrieves the name of the property of the dereferenced object to use instead
    of the object ID.

    \returns
    Name of the property of the dereferenced object to use instead of the object
    ID.
  */
  OdString propertyName();

  /** \details
    By default this function dereferences the value and retrieves the property
    which name is stored in the attribute.

    \param value                [in]  Value to dereference and retrieve the property of.
    \param useDynamicProperties [in]  Flag to optimize getting the displayed value.
                                      It can be set to false only if all properties
                                      of database objects are statically dependent
                                      on the object type. Default value is true.

    \returns
    Name of the property of the dereferenced object to use instead of the object
    ID. If the value type is not a reference or the referenced object has no
    such property, returns an empty string.

    \remarks
     Can be overloaded in custom implementations.
  */
  virtual OdString getDisplayValue(OdRxValue& value, bool useDynamicProperties = true);

  //DOM-IGNORE-BEGIN
  OdRxDisplayAsAttribute(OdRxDisplayAsAttributeImpl* pImpl);
  OdRxDisplayAsAttribute();
  //DOM-IGNORE-END
};

//DOM-IGNORE-BEGIN
class OdRxDynamicAttributeImpl;
//DOM-IGNORE-END

/** \details
  This attribute can be used to store various values (values attached to dynamic property etc).

  <group OdRx_Classes>
*/

class FIRSTDLL_EXPORT OdRxDynamicAttribute : public OdRxAttribute
{
public:
  //DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdRxDynamicAttribute);
  //DOM-IGNORE-END

  /** \details
    Constructor. Creates an attribute of the OdRxDynamicAttribute type.

    \param name [in]  The name of value.
    \param value [in]  The value to stored in attribute.

    \returns
    Smart pointer to a constructed attribute object.
  */
  static OdRxAttributePtr createObject(const OdString& name, const OdRxValue& value);

  /** \details
    Retrieves the name of the value stored in attribute.

    \returns
    Name of the value stored in attribute.
  */
  OdString getName() const;

  /** \details
    Retrieves value stored in attribute.

    \returns
    Value stored in attribute.
  */
  const OdRxValue& getValue() const;

  /** \details
    Retrieves value stored in attribute (non - constant version).

    \returns
    Value stored in attribute.
  */
  OdRxValue& getValue();

protected:
  //DOM-IGNORE-BEGIN
  OdRxDynamicAttribute(OdRxDynamicAttributeImpl*);
  //DOM-IGNORE-END
};

/** \details
  Smart pointer to objects of the <link OdRxDynamicAttribute, OdRxDynamicAttribute> class.
*/
typedef OdSmartPtr<OdRxDynamicAttribute> OdRxDynamicAttributePtr;

//DOM-IGNORE-BEGIN
class OdRxDynamicCollectionAttributeImpl;
//DOM-IGNORE-END

/** <group OdRx_Classes>
  \details
  Container for the dynamic attributes.
*/
class FIRSTDLL_EXPORT OdRxDynamicCollectionAttribute : public OdRxAttribute
{
public:
  //DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdRxDynamicCollectionAttribute);
  //DOM-IGNORE-END
  /** \details
    Constructor. Creates an attribute of the OdRxDynamicCollectionAttribute type.

    \returns
    Smart pointer to a constructed attribute object.
  */
  //static OdRxAttributePtr createObject();

  /** \details
    Get the number of elements in the collection.

    \returns
    Number of elements in the collection.
  */
  int count() const;

  /** \details
    Retrieve the attribute at the index (constant version).

    \param index [in]  Index of the attribute to be retrieved.

    \returns
    Pointer to the attribute at the given index.
  */
  const OdRxDynamicAttribute* getAt(int index) const;

  /** \details
    Retrieve the attribute at the index (non-constant version).

    \param index [in]  Index of the attribute to be retrieved.

    \returns
    Pointer to the attribute at the given index.
  */
  OdRxDynamicAttribute* getAt(int index);

  /** \details
    Retrieve the attribute by its name.

    \param name [in]  Name of the attribute to be retrieved.
    \param caseSensitive [in] Case sensitive string comparison flag. 

    \returns
    Pointer to the attribute at the given index.
  */
  const OdRxDynamicAttribute* getByName(const OdString name, bool caseSensitive) const;

  /** \details
    Retrieve the attribute by its name (non-constant version).

    \param name [in]  Name of the attribute to be retrieved.
    \param caseSensitive [in] Case sensitive string comparison flag.

    \returns
    Pointer to the attribute at the given index.
  */
  OdRxDynamicAttribute* getByName(const OdString name, bool caseSensitive);

  /** \details
    Add an attribute to the collection.

    \param attribute [in]  Attribute to be added to the collection.

    \returns
    eOk if operation was successful; eInvalidInput if the attribute parameter is
    invalid.
  */
  OdResult add(const OdRxDynamicAttribute* attribute);

  /** \details
    Removes the attribute from the collection.

    \param attribute [in]  Attribute to be removed from the collection.

    \returns
    eOk if successful or eKeyNotFound otherwise.
  */
  OdResult remove(const OdRxDynamicAttribute* attribute);

protected:
  //DOM-IGNORE-BEGIN
  OdRxDynamicCollectionAttribute(OdRxDynamicCollectionAttributeImpl*);
  //DOM-IGNORE-END
};

/** \details
  Smart pointer to objects of the <link OdRxDynamicCollectionAttribute, OdRxDynamicCollectionAttribute> class.
*/
typedef OdSmartPtr<OdRxDynamicCollectionAttribute> OdRxDynamicCollectionAttributePtr;

#endif
