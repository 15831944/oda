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
#ifndef __DG_ITEMTYPELIBRARY_H__
#define __DG_ITEMTYPELIBRARY_H__


#include "TD_PackPush.h"

#include "DgElement.h"
#include "OdBinaryData.h"
#include "DgTagSetDefinition.h"
#include "OdTimeStamp.h"
#include "DgGraphicsElement.h"
#include "DgECSchema.h"

//------------------------------------------------------

OdString TG_EXPORT convertStringToECSchemaItemName( const OdString& strName, bool bDisplayLabelConversion = false );
OdString TG_EXPORT convertECSchemaItemNameToString( const OdString& strSchemaItemName );

//------------------------------------------------------

/** \details
   
    Corresponding C++ library: TG_Db 

    <group OdDg_Classes>
*/

//------------------------------------------------------

class OdDgItemTypeProperty;
typedef OdSmartPtr<OdDgItemTypeProperty> OdDgItemTypePropertyPtr;

//------------------------------------------------------

class OdDgECClassInstance;
typedef OdSmartPtr<OdDgECClassInstance> OdDgECClassInstancePtr;

//------------------------------------------------------

/** \details
   
    Corresponding C++ library: TG_Db 

    <group OdDg_Classes>
*/
class OdDgItemTypePropertyUnits;

/** \details
   
    Corresponding C++ library: TG_Db 

    <group OdDg_Classes>
*/
class OdDgItemTypePropertyValue;

//------------------------------------------------------

/** \details
   
    Corresponding C++ library: TG_Db 

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgItemTypeProperty : public OdRxObject
{
public:
  enum OdDgItemTypePropertyType
  {
    kCustom   = 0,
    kString   = 1,
    kDouble   = 2,
    kInteger  = 3,
    kLong     = 4,
    kBoolean  = 5,
    kDateTime = 6,
    kPoint3d  = 7
  };

public:
  ODRX_DECLARE_MEMBERS(OdDgItemTypeProperty);

  virtual OdUInt32             getEntryId() const = 0;

  virtual OdDgItemTypePropertyType getType() const = 0;
  virtual void                     setType( const OdDgItemTypePropertyType& uType ) = 0;

  virtual OdString             getTypeName() const = 0;
  virtual void                 setTypeName( const OdString& strTypeName ) = 0;

  virtual OdDgECClassReference getOwnerClassReference() const = 0;

  virtual OdString             getName() const = 0;
  virtual bool                 setName( const OdString& strName ) = 0;

  virtual OdString             getDisplayLabel() const = 0;
  virtual void                 setDisplayLabel( const OdString& strDisplayLabel ) = 0;

  virtual OdString             getDescription() const = 0;
  virtual void                 setDescription( const OdString& strDescription ) = 0;

  virtual bool                 getReadOnlyFlag() const = 0;
  virtual void                 setReadOnlyFlag( bool bSet ) = 0;

  virtual bool                 getArrayFlag() const = 0;
  virtual void                 setArrayFlag( bool bSet ) = 0;

  virtual bool                 getUsePropertyTypeFlag() const = 0;
  virtual void                 setUsePropertyTypeFlag( bool bSet ) = 0;

  virtual bool                 hasDefaultValue() const = 0;
  virtual void                 clearDefaultValue() = 0;

  virtual OdDgItemTypePropertyValue getDefaultValue() const = 0;
  virtual void                 setDefaultValue( const OdDgItemTypePropertyValue& defaultValue ) = 0;

  virtual OdDgItemTypePropertyUnits getUnits() const = 0;
  virtual void                 setUnits( const OdDgItemTypePropertyUnits& unitsValue ) = 0;

  virtual OdDgItemTypePropertyPtr createClone() const = 0;
};

//------------------------------------------------------

/** \details
   
    Corresponding C++ library: TG_Db 

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgItemTypePropertyUnits
{
public:
  enum OdDgItemTypePropertyUnitsType
  {
    kUndefined           = 0,
    kCustomUnits         = 1,
    kWorkingUnits        = 2,
    kSquareWorkingUnits  = 3,
    kCubicWorkingUnits   = 4,
    kAngularWorkingUnits = 5
  };

public:
  OdDgItemTypePropertyUnits();
  virtual ~OdDgItemTypePropertyUnits();

  OdString getUnitsName() const;
  void     setUnitsName( const OdString& strUnitsName );

  OdDgItemTypePropertyUnitsType getUnitsType() const;
  void setUnitsType( const OdDgItemTypePropertyUnitsType& uType );

private:
  OdDgItemTypePropertyUnitsType m_uType;
  OdString                      m_strUnitsName;
};

//------------------------------------------------------

/** \details
   
    Corresponding C++ library: TG_Db 

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgItemTypePropertyValue
{
public:
  OdDgItemTypePropertyValue();
  virtual ~OdDgItemTypePropertyValue();

  virtual OdDgItemTypeProperty::OdDgItemTypePropertyType getPropertyType() const;

  virtual OdString getStringValue() const;
  virtual void     setStringValue( const OdString& strValue );

  virtual double   getDoubleValue() const;
  virtual void     setDoubleValue( double dValue );

  virtual OdInt32  getIntegerValue() const;
  virtual void     setIntegerValue( OdInt32 iValue );

  virtual OdInt64  getLongValue() const;
  virtual void     setLongValue(OdInt64 iValue);

  virtual bool     getBooleanValue() const;
  virtual void     setBooleanValue( bool bValue );

  virtual OdTimeStamp getDateTimeValue() const;
  virtual void     setDateTimeValue( const OdTimeStamp& timeValue );

  virtual OdGePoint3d getPointValue() const;
  virtual void     setPointValue( const OdGePoint3d& ptValue );

private:

  OdDgItemTypeProperty::OdDgItemTypePropertyType m_valueType;
  OdString    m_strValue;
  double      m_dValue;
  OdInt32     m_iValue;
  OdInt64     m_iLongValue;
  bool        m_bValue;
  OdTimeStamp m_dateTimeValue;
  OdGePoint3d m_ptValue;
};

/** \details
   
    Corresponding C++ library: TG_Db 

    <group OdDg_Classes>
*/
class OdDgItemType;
typedef OdSmartPtr<OdDgItemType> OdDgItemTypePtr;

//------------------------------------------------------

/** \details
   
    Corresponding C++ library: TG_Db 

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgItemType : public OdRxObject
{
public:
  enum OdDgItemTypeCategory
  {
    kItemType        = 0,
    kPropertyType    = 1,
    kCustomAttribute = 2
  };

public:
  ODRX_DECLARE_MEMBERS(OdDgItemType);

  virtual OdUInt32             getEntryId() const = 0;

  virtual OdDgItemTypeCategory getCategory() const = 0;
  virtual void                 setCategory( const OdDgItemTypeCategory& uCategory ) = 0;

  virtual OdString             getName() const = 0;
  virtual bool                 setName( const OdString& strName ) = 0;

  virtual OdDgECClassReference getClassReference() const = 0;

  virtual OdString             getDisplayLabel() const = 0;
  virtual void                 setDisplayLabel( const OdString& strDisplayLabel ) = 0;

  virtual OdString             getDescription() const = 0;
  virtual void                 setDescription( const OdString& strDescription ) = 0;

  virtual bool                 getUseNameForElementFlag() const = 0;
  virtual void                 setUseNameForElementFlag( bool bSet ) = 0;

  virtual OdUInt32             getPropertyCount() const = 0;
  virtual OdDgItemTypePropertyPtr getProperty( OdUInt32 uIndex ) const = 0;
  virtual OdDgItemTypePropertyPtr getProperty( const OdString& strPropertyName ) const = 0;
  virtual void setProperty( OdUInt32 uIndex, const OdDgItemTypePropertyPtr& pProp ) = 0;
  virtual void setProperty( const OdDgItemTypePropertyPtr& pProp ) = 0;
  virtual void addProperty( const OdDgItemTypePropertyPtr& pProp ) = 0;
  virtual void deleteProperty( OdUInt32 uIndex ) = 0;
  virtual void deleteProperty(  const OdString& strPropertyName ) = 0;

  virtual OdDgItemTypePtr      createClone() const = 0;
};

//------------------------------------------------------

/** \details
   
    Corresponding C++ library: TG_Db 

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgItemTypeLibrary : public OdDgElement
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgItemTypeLibrary);

public:
  virtual OdString getName() const;
  virtual bool     setName(const OdString& name);

  virtual OdDgECSchemaReference getReferenceToSchema() const;

  virtual OdString getDescription() const;
  virtual void setDescription( const OdString& strDescription );

  virtual OdString getDisplayLabel() const;
  virtual void setDisplayLabel( const OdString& strDisplayLabel );

  virtual OdUInt32 getItemTypeCount() const;
  virtual OdDgItemTypePtr getItemType( OdUInt32 uIndex ) const;
  virtual OdDgItemTypePtr getItemType( const OdString& strItemName ) const;
  virtual void setItemType( OdUInt32 uIndex, const OdDgItemTypePtr& pItem );
  virtual void setItemType( const OdDgItemTypePtr& pItem );
  virtual void addItemType( const OdDgItemTypePtr& pItem );
  virtual void deleteItemType( OdUInt32 uIndex );
  virtual void deleteItemType(  const OdString& strItemName );

  /** \details
  Called as the first operation as this object is being erased or unerased.

  \param erasing [in]  A copy of the erasing argument passed to erase().

  \remarks
  This function is notified just before the current object is to be erased, giving this function
  the ability to cancel the erase.

  Returns eOk if and only if erase() is to continue.

  When overriding this function:

  1)  If the OdDgElement's state is incorrect, return
      something other than eOk.

  2)  If the parent class's subErase() returns anything
      other than eOk, immediately return it.

  3)  If other actions are required before erase, do them.

  4)  Return eOk.

  If you must make changes to this object's state, either make them after
  step 2, or roll them back if step 2 returns other than eOk.

  The default implementation of this function does nothing but return eOk.  This function can be
  overridden in custom classes.

*/
  virtual OdResult subErase(bool erasing);

// If item type is corrupted.
  virtual bool getIsProxyFlag() const;
  virtual void setIsProxyFlag( bool bSet );

  virtual OdUInt32 getBinarySize() const;
  virtual void     getBinaryData( OdBinaryData& data ) const;
  virtual void     setBinaryData( const OdBinaryData& data );
};

//------------------------------------------------------

typedef OdSmartPtr<OdDgItemTypeLibrary> OdDgItemTypeLibraryPtr;

//------------------------------------------------------

/** \details
    Corresponding C++ library: TG_Db 
    <group OdDg_Classes>
*/
class TG_EXPORT OdDgXMLSchemaContainer : public OdDgItemTypeLibrary
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgXMLSchemaContainer);
public:
  virtual bool setName(const OdString& name);
  virtual void setDescription( const OdString& strDescription );
  virtual void setDisplayLabel( const OdString& strDisplayLabel );

  virtual OdDgECSchemaReference getReferenceToSchema() const;

  virtual OdDgItemTypePtr getItemType( OdUInt32 uIndex ) const;
  virtual OdDgItemTypePtr getItemType( const OdString& strItemName ) const;
  virtual void setItemType( OdUInt32 uIndex, const OdDgItemTypePtr& pItem );
  virtual void setItemType( const OdDgItemTypePtr& pItem );
  virtual void addItemType( const OdDgItemTypePtr& pItem );
  virtual void deleteItemType( OdUInt32 uIndex );
  virtual void deleteItemType(  const OdString& strItemName );

  virtual void setIsProxyFlag( bool bSet );
  virtual void setBinaryData( const OdBinaryData& data );

  virtual OdString getXmlString() const;
  virtual void     setXmlString( const OdString& strData );
};

//------------------------------------------------------

typedef OdSmartPtr<OdDgXMLSchemaContainer> OdDgXMLSchemaContainerPtr;

//------------------------------------------------------

/** \details
    Corresponding C++ library: TG_Db 
    <group OdDg_Classes>
*/
class OdDgXMLProperty;

//------------------------------------------------------

/** \details
    Corresponding C++ library: TG_Db 
    <group OdDg_Classes>
*/
class OdDgCustomItemTypeProperty;
typedef OdSmartPtr<OdDgCustomItemTypeProperty> OdDgCustomItemTypePropertyPtr;

//------------------------------------------------------

/** \details
    Corresponding C++ library: TG_Db 
    <group OdDg_Classes>
*/
class TG_EXPORT OdDgCustomItemTypeProperty : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdDgCustomItemTypeProperty);

  virtual OdDgItemTypeProperty::OdDgItemTypePropertyType getType() const = 0;
  virtual void                 setType( const OdDgItemTypeProperty::OdDgItemTypePropertyType& uType ) = 0;

  virtual OdString             getTypeName() const = 0;
  virtual void                 setTypeName( const OdString& strTypeName ) = 0;

  virtual OdString             getName() const = 0;
  virtual void                 setName( const OdString& strName ) = 0;

  virtual OdDgECClassReference getOwnerClassReference() const = 0;

  virtual OdString             getDisplayLabel() const = 0;
  virtual void                 setDisplayLabel( const OdString& strDisplayLabel ) = 0;

  virtual OdString             getDescription() const = 0;
  virtual void                 setDescription( const OdString& strDescription ) = 0;

  virtual bool                 getReadOnlyFlag() const = 0;
  virtual void                 setReadOnlyFlag( bool bSet ) = 0;

  virtual bool                 getArrayFlag() const = 0;
  virtual void                 setArrayFlag( bool bSet ) = 0;

  virtual OdUInt32 getCustomAttributePropertiesCount() const = 0;
  virtual OdDgXMLProperty getCustomAttributeProperty( OdUInt32 uIndex ) const = 0;
  virtual void     setCustomAttributeProperty( OdUInt32 uIndex, const OdDgXMLProperty& property ) = 0;
  virtual void     addCustomAttributeProperty( const OdDgXMLProperty& property ) = 0;
  virtual void     deleteCustomAttributeProperty( OdUInt32 uIndex ) = 0;

  virtual OdUInt32 getExtraPropertiesCount() const = 0;
  virtual OdDgXMLProperty getExtraProperty( OdUInt32 uIndex ) const = 0;
  virtual void     setExtraProperty( OdUInt32 uIndex, const OdDgXMLProperty& property ) = 0;
  virtual void     addExtraProperty( const OdDgXMLProperty& property ) = 0;
  virtual void     deleteExtraProperty( OdUInt32 uIndex ) = 0;

  virtual OdDgCustomItemTypePropertyPtr createClone() const = 0;
};

//------------------------------------------------------

/** \details
    Corresponding C++ library: TG_Db 
    <group OdDg_Classes>
*/
class OdDgCustomItemType;
typedef OdSmartPtr<OdDgCustomItemType> OdDgCustomItemTypePtr;

//------------------------------------------------------

/** \details
    Corresponding C++ library: TG_Db 
    <group OdDg_Classes>
*/
class TG_EXPORT OdDgCustomItemType : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdDgCustomItemType);

  virtual OdString             getName() const = 0;
  virtual void                 setName( const OdString& strName ) = 0;

  virtual OdDgECClassReference getClassReference() const = 0;

  virtual bool                 getIsCustomAttributeClassFlag() const = 0;
  virtual void                 setIsCustomAttributeClassFlag( bool bSet ) = 0;

  virtual bool                 getIsDomainClassFlag() const = 0;
  virtual void                 setIsDomainClassFlag( bool bSet ) = 0;

  virtual bool                 getIsStructFlag() const = 0;
  virtual void                 setIsStructFlag( bool bSet ) = 0;

  virtual OdString             getDisplayLabel() const = 0;
  virtual void                 setDisplayLabel( const OdString& strDisplayLabel ) = 0;

  virtual OdString             getDescription() const = 0;
  virtual void                 setDescription( const OdString& strDescription ) = 0;

  virtual OdUInt32 getCustomAttributePropertiesCount() const = 0;
  virtual OdDgXMLProperty getCustomAttributeProperty( OdUInt32 uIndex ) const = 0;
  virtual void     setCustomAttributeProperty( OdUInt32 uIndex, const OdDgXMLProperty& property ) = 0;
  virtual void     addCustomAttributeProperty( const OdDgXMLProperty& property ) = 0;
  virtual void     deleteCustomAttributeProperty( OdUInt32 uIndex ) = 0;

  virtual OdUInt32             getPropertyCount() const = 0;
  virtual OdDgCustomItemTypePropertyPtr getProperty( OdUInt32 uIndex ) const = 0;
  virtual OdDgCustomItemTypePropertyPtr getProperty( const OdString& strPropertyName ) const = 0;
  virtual void setProperty( OdUInt32 uIndex, const OdDgCustomItemTypePropertyPtr& pProp ) = 0;
  virtual void setProperty( const OdDgCustomItemTypePropertyPtr& pProp ) = 0;
  virtual void addProperty( const OdDgCustomItemTypePropertyPtr& pProp ) = 0;
  virtual void deleteProperty( OdUInt32 uIndex ) = 0;
  virtual void deleteProperty(  const OdString& strPropertyName ) = 0;

  virtual OdUInt32 getExtraPropertiesCount() const = 0;
  virtual OdDgXMLProperty getExtraProperty( OdUInt32 uIndex ) const = 0;
  virtual void     setExtraProperty( OdUInt32 uIndex, const OdDgXMLProperty& property ) = 0;
  virtual void     addExtraProperty( const OdDgXMLProperty& property ) = 0;
  virtual void     deleteExtraProperty( OdUInt32 uIndex ) = 0;

  virtual OdDgCustomItemTypePtr      createClone() const = 0;
};

//------------------------------------------------------

/** \details
    Corresponding C++ library: TG_Db 
    <group OdDg_Classes>
*/
class TG_EXPORT OdDgXMLCustomSchemaContainer : public OdDgElement
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgXMLCustomSchemaContainer);

public:
  virtual OdString getName() const;
  virtual void     setName(const OdString& strName);

  virtual OdString getNameSpacePrefix() const;
  virtual void     setNameSpacePrefix(const OdString& strNameSpacePrefix);

  virtual OdString getVersion() const;
  virtual void     setVersion(const OdString& strVersion);

  virtual OdDgECSchemaReference getReferenceToSchema() const;

  virtual OdString getDisplayName() const;
  virtual void     setDisplayName(const OdString& strDisplayName);

  virtual OdString getDescription() const;
  virtual void     setDescription(const OdString& strDescription);

  virtual OdArray<OdDgECSchemaReference> getXMLReferences() const;
  virtual void setXMLReferences( const OdArray<OdDgECSchemaReference>& arrRefs );

  /** \details
  Called as the first operation as this object is being erased or unerased.

  \param erasing [in]  A copy of the erasing argument passed to erase().

  \remarks
  This function is notified just before the current object is to be erased, giving this function
  the ability to cancel the erase.

  Returns eOk if and only if erase() is to continue.

  When overriding this function:

  1)  If the OdDgElement's state is incorrect, return
      something other than eOk.

  2)  If the parent class's subErase() returns anything
      other than eOk, immediately return it.

  3)  If other actions are required before erase, do them.

  4)  Return eOk.

  If you must make changes to this object's state, either make them after
  step 2, or roll them back if step 2 returns other than eOk.

  The default implementation of this function does nothing but return eOk.  This function can be
  overridden in custom classes.

  */
  virtual OdResult subErase(bool erasing);

  virtual OdUInt32 getCustomItemCount() const;

  virtual OdDgCustomItemTypePtr getCustomItem( OdUInt32 uIndex ) const;
  virtual OdDgCustomItemTypePtr getCustomItem( const OdString& strItemName ) const;
  virtual void setCustomItem( OdUInt32 uIndex, const OdDgCustomItemTypePtr& pItem );
  virtual void setCustomItem( const OdDgCustomItemTypePtr& pItem );
  virtual void addCustomItem( const OdDgCustomItemTypePtr& pItem );
  virtual void deleteCustomItem( OdUInt32 uIndex );
  virtual void deleteCustomItem(  const OdString& strItemName );

  virtual OdUInt32 getCustomAttributePropertiesCount() const;
  virtual OdDgXMLProperty getCustomAttributeProperty( OdUInt32 uIndex ) const;
  virtual void     setCustomAttributeProperty( OdUInt32 uIndex, const OdDgXMLProperty& property );
  virtual void     addCustomAttributeProperty( const OdDgXMLProperty& property );
  virtual void     deleteCustomAttributeProperty( OdUInt32 uIndex );

  virtual OdUInt32 getExtraPropertiesCount() const;
  virtual OdDgXMLProperty getExtraProperty( OdUInt32 uIndex ) const;
  virtual void     setExtraProperty( OdUInt32 uIndex, const OdDgXMLProperty& property );
  virtual void     addExtraProperty( const OdDgXMLProperty& property );
  virtual void     deleteExtraProperty( OdUInt32 uIndex );
};

//------------------------------------------------------

typedef OdSmartPtr<OdDgXMLCustomSchemaContainer> OdDgXMLCustomSchemaContainerPtr;

//------------------------------------------------------

/** \details
   
    Corresponding C++ library: TG_Db 

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgECPropertyInstanceFullPath
{
	public:
    OdDgECPropertyInstanceFullPath();
		virtual ~OdDgECPropertyInstanceFullPath();

		OdString getECSchemaName() const;
		void     setECSchemaName(const OdString& strName);

		OdString getECClassName() const;
		void     setECClassName(const OdString& strName);

		OdDgECPropertyInstance getPropertyInstance() const;
		void setPropertyInstance(const OdDgECPropertyInstance& value);

	private:
		OdString m_strSchemaName;
		OdString m_strClassName;
		OdDgECPropertyInstance m_propertyInstance;
};

//------------------------------------------------------

/** \details

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgECClassInstanceElementPE : public OdRxObject
{
  ODRX_DECLARE_MEMBERS(OdDgECClassInstanceElementPE);
  long        m_references;     //used by each object separately
public:

  OdDgECClassInstanceElementPE();
  ~OdDgECClassInstanceElementPE();

  //OdRxObject overridden
  virtual void addRef();
  virtual void release();
  virtual long numRefs() const;

  virtual OdUInt32 getECClassInstanceCount(const OdDgElement* obj);
  virtual OdDgECClassInstancePtr getECClassInstance(const OdDgElement* obj, OdUInt32 uIndex);
  virtual OdDgECClassInstancePtr getECClassInstance(const OdDgElement* obj, const OdDgECClassReference& classRef );
  virtual void setECClassInstance(OdDgElement* obj, OdUInt32 uIndex, const OdDgECClassInstancePtr& pClassInstance);
  virtual void setECClassInstance(OdDgElement* obj, const OdDgECClassInstancePtr& pClassInstance);
  virtual void addECClassInstance(OdDgElement* obj, const OdDgECClassInstancePtr& pClassInstance);
  virtual void deleteECClassInstance(OdDgElement* obj, OdUInt32 uIndex);
  virtual void deleteECClassInstance(OdDgElement* obj, const OdDgECClassReference& classRef );
};

//------------------------------------------------------

typedef OdSmartPtr<OdDgECClassInstanceElementPE> OdDgECClassInstanceElementPEPtr;

//------------------------------------------------------

#include "TD_PackPop.h"

#endif // __DG_ITEMTYPELIBRARY_H__
