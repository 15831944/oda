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
#ifndef __DG_ECSCHEMA_H__
#define __DG_ECSCHEMA_H__

#include "TD_PackPush.h"

#include "DgElement.h"
#include "OdBinaryData.h"
#include "DgTagSetDefinition.h"
#include "OdTimeStamp.h"
#include "DgGraphicsElement.h"

//------------------------------------------------------

/** 
  <group OdDg_Classes>
*/
class TG_EXPORT OdDgECSchemaReference
{
public:
  OdDgECSchemaReference();
  OdDgECSchemaReference(const OdString& strName, const OdString& strVersion, const OdString& strPrefix);
  ~OdDgECSchemaReference();

  OdString getName() const;
  void     setName(const OdString& strName);

  OdString getVersion() const;
  void     setVersion(const OdString& strVersion);

  OdString getPrefix() const;
  void     setPrefix(const OdString& strPrefix);

  bool operator< (const OdDgECSchemaReference& data) const;
  bool operator==(const OdDgECSchemaReference& data) const;
  bool isEmpty() const;

private:

  OdString m_strName;
  OdString m_strPrefix;
  OdString m_strVersion;
};

//------------------------------------------------------

/** 
  <group OdDg_Classes>
*/
class TG_EXPORT OdDgECClassReference
{
public:
  OdDgECClassReference();
  OdDgECClassReference(const OdString& strClassName, const OdDgECSchemaReference& schemaRef);
  OdDgECClassReference(const OdString& strClassName, const OdString& strSchemaName, 
    const OdString& strSchemaVersion, const OdString& strSchemaPrefix);

  OdString getClassName() const;
  void     setClassName(const OdString& strName);

  OdString getSchemaName() const;
  OdString getSchemaVersion() const;
  OdString getSchemaPrefix() const;

  OdDgECSchemaReference getSchemaReference() const;
  void     setSchemaReference(const OdDgECSchemaReference& schemaRef);

  bool operator< (const OdDgECClassReference& data) const;
  bool operator==(const OdDgECClassReference& data) const;

  static OdString createName(const OdDgECClassReference& classRef);

private:
  OdDgECSchemaReference m_ecSchemaRef;
  OdString              m_strClassName;
};

//------------------------------------------------------

class OdDgECClass;
typedef OdSmartPtr<OdDgECClass> OdDgECClassPtr;

//------------------------------------------------------

class OdDgECProperty;
typedef OdSmartPtr<OdDgECProperty> OdDgECPropertyPtr;

//------------------------------------------------------

class OdDgECArrayProperty;
typedef OdSmartPtr<OdDgECArrayProperty> OdDgECArrayPropertyPtr;

//------------------------------------------------------

class OdDgECStructProperty;
typedef OdSmartPtr<OdDgECStructProperty> OdDgECStructPropertyPtr;

//------------------------------------------------------

class OdDgECClassInstance;
typedef OdSmartPtr<OdDgECClassInstance> OdDgECClassInstancePtr;

//------------------------------------------------------

class OdDgECCustomClassInstance;
typedef OdSmartPtr<OdDgECCustomClassInstance> OdDgECCustomClassInstancePtr;

//------------------------------------------------------

/** \details
    Corresponding C++ library: TG_Db
    <group OdDg_Classes>
*/
class TG_EXPORT OdDgECPropertyInstance
{
public:
  enum OdDgECPropertyInstanceType
  {
    kUndefined = 0,
    kBool = 1,
    kBoolArray = 2,
    kByte = 3,
    kByteArray = 4,
    kShort = 5,
    kShortArray = 6,
    kInt32 = 7,
    kInt32Array = 8,
    kInt64 = 9,
    kInt64Array = 10,
    kDouble = 11,
    kDoubleArray = 12,
    kString = 13,
    kStringArray = 14,
    kPoint3d = 15,
    kPoint3dArray = 16,
    kDateAndTime = 17,
    kDateTimeArray = 18,
    kBinary = 19,
    kBinaryArray = 20,
    kECClassInstance = 21,
    kECClassInstanceArray = 22
  };

public:
  OdDgECPropertyInstance();
  OdDgECPropertyInstance(const OdDgECPropertyInstance& obj );
  OdDgECPropertyInstance(const OdString& strName, bool bValue);
  OdDgECPropertyInstance(const OdString& strName, const OdArray<bool>& arrBoolValue);
  OdDgECPropertyInstance(const OdString& strName, OdInt8 iValue);
  OdDgECPropertyInstance(const OdString& strName, const OdArray<OdInt8>& arrByteValue);
  OdDgECPropertyInstance(const OdString& strName, OdInt16 iValue);
  OdDgECPropertyInstance(const OdString& strName, const OdArray<OdInt16>& arrShortValue);
  OdDgECPropertyInstance(const OdString& strName, OdInt32 iValue);
  OdDgECPropertyInstance(const OdString& strName, const OdArray<OdInt32>& arrIntValue);
  OdDgECPropertyInstance(const OdString& strName, OdInt64 iValue);
  OdDgECPropertyInstance(const OdString& strName, const OdArray<OdInt64>& arrInt64Value);
  OdDgECPropertyInstance(const OdString& strName, double dValue);
  OdDgECPropertyInstance(const OdString& strName, const OdArray<double>& arrDoubleValue);
  OdDgECPropertyInstance(const OdString& strName, const OdString& strValue);
  OdDgECPropertyInstance(const OdString& strName, const OdArray<OdString>& arrStrValue);
  OdDgECPropertyInstance(const OdString& strName, const OdGePoint3d& ptValue);
  OdDgECPropertyInstance(const OdString& strName, const OdArray<OdGePoint3d>& arrPt3dValue);
  OdDgECPropertyInstance(const OdString& strName, const OdTimeStamp& timeValue);
  OdDgECPropertyInstance(const OdString& strName, const OdArray<OdTimeStamp>& arrDateTimeValue);
  OdDgECPropertyInstance(const OdString& strName, const OdBinaryData& binData);
  OdDgECPropertyInstance(const OdString& strName, const OdArray<OdBinaryData>& arrBinData);
  OdDgECPropertyInstance(const OdString& strName, const OdDgECClassInstancePtr& classProperty);
  OdDgECPropertyInstance(const OdString& strName, const OdArray< OdDgECClassInstancePtr>& arrClassProperty);

  virtual ~OdDgECPropertyInstance();

  bool isUndefined() const;

  OdString getName() const;
  void   setName(const OdString& strName);

  void setByDefault(const OdRxObjectPtr& pProperty, OdDgDatabase* pDb);

  OdDgECPropertyInstance::OdDgECPropertyInstanceType getType() const;

  bool getBoolValue() const;
  void setBoolValue(bool bValue);

  OdArray<bool> getBoolArrayValue() const;
  void setBoolArrayValue(const OdArray<bool>& arrBoolValue);

  OdInt8 getByteValue() const;
  void   setByteValue(OdInt8 iValue);

  OdArray<OdInt8> getByteArrayValue() const;
  void setByteArrayValue(const OdArray<OdInt8>& arrByteValue);

  OdBinaryData getBinaryValue() const;
  void   setBinaryValue(const OdBinaryData& binValue);

  OdArray<OdBinaryData> getBinaryArrayValue() const;
  void   setBinaryArrayValue(const OdArray<OdBinaryData>& arrBinValue);

  OdInt16 getShortValue() const;
  void   setShortValue(OdInt16 iValue);

  OdArray<OdInt16> getShortArrayValue() const;
  void setShortArrayValue(const OdArray<OdInt16>& arrShortValue);

  OdInt32 getIntValue() const;
  void   setIntValue(OdInt32 iValue);

  OdArray<OdInt32> getIntArrayValue() const;
  void setIntArrayValue(const OdArray<OdInt32>& arrIntValue);

  OdInt64 getInt64Value() const;
  void   setInt64Value(OdInt64 iValue);

  OdArray<OdInt64> getInt64ArrayValue() const;
  void setInt64ArrayValue(const OdArray<OdInt64>& arrInt64Value);

  double getDoubleValue() const;
  void   setDoubleValue(double dValue);

  OdArray<double> getDoubleArrayValue() const;
  void setDoubleArrayValue(const OdArray<double>& arrDoubleValue);

  OdString getStringValue() const;
  void   setStringValue(const OdString& strValue);

  OdArray<OdString> getStringArrayValue() const;
  void setStringArrayValue(const OdArray<OdString>& arrStringValue);

  OdGePoint3d getPoint3dValue() const;
  void   setPoint3dValue(const OdGePoint3d& ptValue);

  OdArray<OdGePoint3d> getPoint3dArrayValue() const;
  void setPoint3dArrayValue(const OdArray<OdGePoint3d>& arrPoint3dValue);

  OdTimeStamp getDateTimeValue() const;
  void   setDateTimeValue(const OdTimeStamp& timeValue);

  OdArray<OdTimeStamp> getDateTimeArrayValue() const;
  void setDateTimeArrayValue(const OdArray<OdTimeStamp>& arrDateTimeValue);

  OdDgECClassInstancePtr getClassInstanceValue() const;
  void setClassInstanceValue(const OdDgECClassInstancePtr& classProperty);

  OdArray<OdDgECClassInstancePtr> getClassInstanceArrayValue() const;
  void setClassInstanceArrayValue(const OdArray<OdDgECClassInstancePtr>& arrClassInstance);

  bool isEqualTo(const OdDgECPropertyInstance& val2, bool bCompareNames = true) const;

  OdDgECPropertyInstance& operator=(const OdDgECPropertyInstance& obj);

private:

  OdDgECPropertyInstanceType                m_uType;
  bool                                      m_bValue;
  OdArray<bool>                             m_arrBoolValue;
  OdInt8                                    m_iInt8Value;
  OdArray<OdInt8>                           m_arrInt8Value;
  OdInt16                                   m_iInt16Value;
  OdArray<OdInt16>                          m_arrInt16Value;
  OdInt32                                   m_iInt32Value;
  OdArray<OdInt32>                          m_arrInt32Value;
  OdInt64                                   m_iInt64Value;
  OdArray<OdInt64>                          m_arrInt64Value;
  double                                    m_dValue;
  OdArray<double>                           m_arrDoubleValue;
  OdString                                  m_strValue;
  OdArray<OdString>                         m_arrStringValue;
  OdGePoint3d                               m_ptValue;
  OdArray<OdGePoint3d>                      m_arrPoint3dValue;
  OdTimeStamp                               m_timeValue;
  OdArray<OdTimeStamp>                      m_arrDateTimeValue;
  OdBinaryData                              m_binDataValue;
  OdArray<OdBinaryData>                     m_arrBinDataValue;
  OdDgECClassInstancePtr                    m_pClassInstance;
  OdArray<OdDgECClassInstancePtr>           m_arrClassInstancePtr;
  OdString                                  m_strName;
};

//------------------------------------------------------

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgECClassInstance : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdDgECClassInstance);

  virtual OdString               getName() const = 0;
  virtual OdString               getClassName() const = 0;
  virtual OdString               getSchemaName() const = 0;

  virtual bool                   isDefault() const = 0;

  virtual OdDgECClassReference   getECClassReference() const = 0;
  virtual OdUInt64               getECSchemaId() const = 0;

  virtual OdUInt32               getPropertyInstanceCount() const = 0;
  virtual bool                   addPropertyInstance(const OdDgECPropertyInstance& pAttribute) = 0;
  virtual OdDgECPropertyInstance getPropertyInstance(OdUInt32 uIndex) const = 0;
  virtual OdDgECPropertyInstance getPropertyInstance(const OdString& strPropertyName) const = 0;
  virtual bool                   setPropertyInstance(const OdDgECPropertyInstance& pPropertyInstance) = 0;
  virtual bool                   setPropertyInstance(OdUInt32 uIndex, const OdDgECPropertyInstance& pPropertyInstance) = 0;
  virtual bool                   deletePropertyInstance(const OdString& strPropertyName) = 0;
  virtual bool                   deletePropertyInstance(OdUInt32 uIndex) = 0;

  virtual bool                   isEqualTo(const OdDgECCustomClassInstancePtr& pObj) const = 0;
  virtual OdRxObjectPtr          clone() const = 0;
  virtual void                   copyFrom(const OdRxObject* pSource) = 0;

  static OdDgECClassInstancePtr  createObject(const OdDgECClassReference& classRef, OdDgDatabase* pDb, bool bAllowToCreateDefaultInstances = true);
};

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgECCustomClassInstance : public OdDgECClassInstance
{
public:
  ODRX_DECLARE_MEMBERS(OdDgECCustomClassInstance);

  virtual void                   setByDefault(const OdRxObjectPtr& pClass, OdDgDatabase* pDb, bool bSetAllProperties = true) = 0;
};

//------------------------------------------------------

/** \details
    Corresponding C++ library: TG_Db
    <group OdDg_Classes>
*/
class TG_EXPORT OdDgECProperty : public OdRxObject
{
  ODDG_DECLARE_MEMBERS(OdDgECProperty);
public:
  enum OdDgECPropertyType
  {
    kECClassReference = 0,
    kString           = 1,
    kDouble           = 2,
    kInteger          = 3,
    kLong             = 4,
    kBoolean          = 5,
    kDateTime         = 6,
    kPoint3d          = 7,
    kBinary           = 8
  };
public:
  virtual OdString getName() const = 0;
  virtual void     setName(const OdString& strName) = 0;

  virtual bool     isArrayProperty() const = 0;
  virtual bool     isStructProperty() const = 0;

  virtual OdDgECPropertyType getPropertyType( OdDgECClassReference& typeRef ) const = 0;
  virtual OdDgECPropertyType getPropertyType() const = 0;
  virtual void setPropertyType(OdDgECPropertyType uType, const OdDgECClassReference& typeRef ) = 0;

  virtual OdString getDisplayName() const = 0;
  virtual void     setDisplayName(const OdString& strDisplayName) = 0;

  virtual OdString getDescription() const = 0;
  virtual void     setDescription(const OdString& strDescription) = 0;

  virtual bool     getReadOnlyFlag() const = 0;
  virtual void     setReadOnlyFlag(bool bSet) = 0;

  virtual OdUInt32               getCustomAttributeCount() const = 0;
  virtual OdDgECClassInstancePtr getCustomAttribute(const OdDgECClassReference& classRef) const = 0;
  virtual OdDgECClassInstancePtr getCustomAttribute(OdUInt32 uIndex) const = 0;
  virtual bool                   addCustomAttribute(OdDgECClassInstance* pAttribute) = 0;
  virtual bool                   setCustomAttribute(OdDgECClassInstance* pAttribute) = 0;
  virtual bool                   setCustomAttribute(OdUInt32 uIndex, OdDgECClassInstance* pAttribute) = 0;
  virtual bool                   deleteCustomAttribute(const OdDgECClassReference& classRef) = 0;
  virtual bool                   deleteCustomAttribute(OdUInt32 uIndex) = 0;

  virtual OdRxObjectPtr          clone() const = 0;
  virtual void                   copyFrom(const OdRxObject* pSource) = 0;

  static OdDgECPropertyPtr       createObject( bool bStructProperty, bool bArrayProperty);
};

//------------------------------------------------------

/** \details
    Corresponding C++ library: TG_Db
    <group OdDg_Classes>
*/
class TG_EXPORT OdDgECArrayProperty : public OdDgECProperty
{
  ODDG_DECLARE_MEMBERS(OdDgECArrayProperty);
public:
  virtual OdString getMaxOccurs() const = 0;
  virtual void     setMaxOccurs(const OdString& strValue) = 0;

  virtual OdString getMinOccurs() const = 0;
  virtual void     setMinOccurs(const OdString& strValue) = 0;

  virtual void     setStructFlag(bool bSet) = 0;

  virtual OdString getZeroString() const = 0;
  virtual OdString getUnboundedString() const = 0;
};

//------------------------------------------------------

/** \details
    Corresponding C++ library: TG_Db
    <group OdDg_Classes>
*/
class TG_EXPORT OdDgECStructProperty : public OdDgECProperty
{
  ODDG_DECLARE_MEMBERS(OdDgECStructProperty);
};

//------------------------------------------------------

/** \details
    Corresponding C++ library: TG_Db
    <group OdDg_Classes>
*/
class TG_EXPORT OdDgECClass : public OdRxObject
{
  ODDG_DECLARE_MEMBERS(OdDgECClass);
public:
  enum OdDgECClassType
  {
    kClassValue = 0,
    kClassDomain = 1,
    kClassStructValue = 2,
    kClassStructDomain = 3,
    kCustomAttribute = 4
  };

public:

  virtual OdString getName() const = 0;
  virtual void     setName(const OdString& strName) = 0;

  virtual OdDgECClassReference getClassReference() const = 0;

  virtual OdString getDisplayName() const = 0;
  virtual void     setDisplayName(const OdString& strDisplayName) = 0;

  virtual OdString getDescription() const = 0;
  virtual void     setDescription(const OdString& strDescription) = 0;

  virtual OdDgECClassType        getType() const = 0;
  virtual void                   setType(const OdDgECClassType& uType) = 0;

  virtual OdUInt32               getCustomAttributeCount() const = 0;
  virtual OdDgECClassInstancePtr getCustomAttribute(const OdDgECClassReference& classRef) const = 0;
  virtual OdDgECClassInstancePtr getCustomAttribute(OdUInt32 uIndex) const = 0;
  virtual bool                   addCustomAttribute(OdDgECClassInstance* pAttribute) = 0;
  virtual bool                   setCustomAttribute(OdDgECClassInstance* pAttribute) = 0;
  virtual bool                   setCustomAttribute(OdUInt32 uIndex, OdDgECClassInstance* pAttribute) = 0;
  virtual bool                   deleteCustomAttribute(const OdDgECClassReference& classRef) = 0;
  virtual bool                   deleteCustomAttribute(OdUInt32 uIndex) = 0;

  static OdDgECClassPtr          createObject(bool bRelationshipClass);

public:
  virtual OdUInt32                getBaseClassCount() const = 0;
  virtual OdDgECClassReference    getBaseClass(OdUInt32 uIndex) const = 0;
  virtual bool                    setBaseClass(OdUInt32 uIndex, const OdDgECClassReference& classRef) = 0;
  virtual bool                    addBaseClass(const OdDgECClassReference& classRef) = 0;
  virtual bool                    deleteBaseClass(OdUInt32 uIndex) = 0;

  virtual OdUInt32                getPropertyCount() const = 0;
  virtual OdDgECPropertyPtr       getProperty(OdUInt32 uIndex) const = 0;
  virtual OdDgECPropertyPtr       getProperty(const OdString& strPropertyName) const = 0;
  virtual void                    addProperty(OdDgECProperty* pProperty) = 0;
  virtual void                    setProperty(OdDgECProperty* pProperty) = 0;
  virtual void                    setProperty(OdUInt32 uIndex, OdDgECProperty* pProperty) = 0;
  virtual bool                    deleteProperty(const OdString& strPropertyName) = 0;
  virtual bool                    deleteProperty(OdUInt32 uIndex) = 0;

  virtual OdRxObjectPtr           clone() const = 0;
  virtual void                    copyFrom(const OdRxObject* pSource) = 0;
};

//------------------------------------------------------

class OdDgECRelationship;
typedef OdSmartPtr<OdDgECRelationship> OdDgECRelationshipPtr;

//------------------------------------------------------
/** \details
    Corresponding C++ library: TG_Db
    <group OdDg_Classes>
*/
class TG_EXPORT OdDgECRelationship : public OdRxObject
{
  ODDG_DECLARE_MEMBERS(OdDgECRelationship);
public:
  virtual OdString getRoleLabel() const = 0;
  virtual void     setRoleLabel(const OdString& strLabel) = 0;

  virtual bool     getPolymorphicFlag() const = 0;
  virtual void     setPolymorphicFlag(bool bSet) = 0;

  virtual OdString getCardinality() const = 0;
  virtual void     setCardinality(const OdString& strCardinality) = 0;

  virtual OdString getCardinality01Value() const = 0;
  virtual OdString getCardinality0NValue() const = 0;

  virtual OdUInt32               getCustomAttributeCount() const = 0;
  virtual OdDgECClassInstancePtr getCustomAttribute(const OdDgECClassReference& classRef) const = 0;
  virtual OdDgECClassInstancePtr getCustomAttribute(OdUInt32 uIndex) const = 0;
  virtual bool                   addCustomAttribute(OdDgECClassInstance* pAttribute) = 0;
  virtual bool                   setCustomAttribute(OdDgECClassInstance* pAttribute) = 0;
  virtual bool                   setCustomAttribute(OdUInt32 uIndex, OdDgECClassInstance* pAttribute) = 0;
  virtual bool                   deleteCustomAttribute(const OdDgECClassReference& classRef) = 0;
  virtual bool                   deleteCustomAttribute(OdUInt32 uIndex) = 0;

  virtual OdUInt32               getClassCount() const = 0;
  virtual OdDgECClassReference   getClass(OdUInt32 uIndex) const = 0;
  virtual bool                   setClass(OdUInt32 uIndex, const OdDgECClassReference& classRef) = 0;
  virtual bool                   addClass(const OdDgECClassReference& classRef) = 0;
  virtual bool                   deleteClass(OdUInt32 uIndex) = 0;
};

//------------------------------------------------------

class OdDgECRelationshipClass;
typedef OdSmartPtr<OdDgECRelationshipClass> OdDgECRelationshipClassPtr;

//------------------------------------------------------
/** \details
    Corresponding C++ library: TG_Db
    <group OdDg_Classes>
*/
class TG_EXPORT OdDgECRelationshipClass : public OdDgECClass
{
  ODDG_DECLARE_MEMBERS(OdDgECRelationshipClass);
public:
  enum OdDgECRelationshipDirection
  {
    kForward  = 0,
    kBackward = 1
  };

  enum OdDgECRelationshipStrength
  {
    kReferencing  = 0,
    kEmbedding    = 1,
    kHolding      = 2
  };
public:

  virtual OdDgECRelationshipStrength getStrength() const = 0;
  virtual void            setStrength(const OdDgECRelationshipStrength& uStrength) = 0;

  virtual OdDgECRelationshipDirection getStrengthDirection() const = 0;
  virtual void            setStrengthDirection(const OdDgECRelationshipDirection& uDirection) = 0;

  virtual OdDgECRelationshipPtr  getSource() const = 0;
  virtual void  setSource(const OdDgECRelationshipPtr& relationship) = 0;

  virtual OdDgECRelationshipPtr  getTarget() const = 0;
  virtual void  setTarget(const OdDgECRelationshipPtr& relationship) = 0;
};

//------------------------------------------------------

/** \details
    Corresponding C++ library: TG_Db
    <group OdDg_Classes>
*/
class TG_EXPORT OdDgECSchema : public OdDgElement
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgECSchema);
public:
  enum DgECSchemeVerificationResult
  {
    kOk           = 0,
    kUnknownError = 1
  };

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

  virtual bool     getExtraXmlnsOnXmlDataFlag() const;
  virtual void     setExtraXmlnsOnXmlDataFlag( bool bSet );

  virtual OdUInt32              getReferenceCount() const;
  virtual OdDgECSchemaReference getReference(OdUInt32 uIndex) const;
  virtual bool                  setReference(OdUInt32 uIndex, const OdDgECSchemaReference& newRef );
  virtual void                  addReference(const OdDgECSchemaReference& newRef);
  virtual bool                  deleteReference(OdUInt32 uIndex);

  virtual OdUInt32               getCustomAttributeCount() const ;
  virtual OdDgECClassInstancePtr getCustomAttribute(const OdDgECClassReference& classRef) const;
  virtual OdDgECClassInstancePtr getCustomAttribute(OdUInt32 uIndex) const;
  virtual bool                   addCustomAttribute(OdDgECClassInstance* pAttribute);
  virtual bool                   setCustomAttribute(OdDgECClassInstance* pAttribute);
  virtual bool                   setCustomAttribute(OdUInt32 uIndex, OdDgECClassInstance* pAttribute);
  virtual bool                   deleteCustomAttribute(const OdDgECClassReference& classRef);
  virtual bool                   deleteCustomAttribute(OdUInt32 uIndex);

  virtual OdUInt32               getClassCount() const;
  virtual OdDgECClassPtr         getClass(const OdString& strClassName) const;
  virtual OdDgECClassPtr         getClass(OdUInt32 uIndex) const;
  virtual void                   addClass(OdDgECClass* pClass);
  virtual void                   setClass(OdDgECClass* pClass);
  virtual void                   setClass(OdUInt32 uIndex, OdDgECClass* pClass, bool bDeleteInstances = true);
  virtual bool                   deleteClass(const OdString& strClassName, bool bDeleteInstances = true);
  virtual bool                   deleteClass(OdUInt32 uIndex, bool bDeleteInstances = true);

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

  virtual DgECSchemeVerificationResult verifyAndAudit(OdDgDatabase* pDb, OdString& errorDescription);
  virtual void                   loadFromXMLString(const OdString& strXML);
  virtual OdString               saveToXMLString() const;

  virtual bool                   getCorruptedDataFlag() const;
  virtual OdBinaryData           getDataOfCorruptedSchema() const;
  virtual OdDgElementIteratorPtr createIterator(bool atBeginning = true, bool skipDeleted = true) const;

  virtual bool                   getECSchemaForItemTypeFlag() const;
  virtual void                   setECSchemaForItemTypeFlag( bool bSet);

  virtual bool                   getUseECXAProviderFlag() const;
  virtual void                   setUseECXAProviderFlag(bool bSet);

  static  OdDgECPropertyPtr      getECPropertyByName(const OdDgECClassReference& classRef, const OdString& strPropName, const OdDgDatabase* pDb);
  static  OdDgECClassPtr         getECClassByReference(const OdDgECClassReference& classRef, const OdDgDatabase* pDb);
  static  OdRxObjectPtr          getClassByReference(const OdDgECClassReference& classRef, const OdDgDatabase* pDb);
  static  OdDgECClassReference   getECClassReference(const OdRxObjectPtr& pClass);

  static  OdDgElementId          getECSchemaIdByName(const OdString& strName, const OdDgDatabase* pDb);
  static  OdDgElementId          getECSchemaIdByReference(const OdDgECSchemaReference& schemaRef, const OdDgDatabase* pDb);
  static  OdDgECSchemaReference  getECSchemaReferenceById(const OdDgElementId& idSchema);
  static  OdDgECSchemaReference  getECSchemaReferenceByName(const OdString& strName, const OdDgDatabase* pDb);
};

//------------------------------------------------------

typedef OdSmartPtr<OdDgECSchema> OdDgECSchemaPtr;

#include "TD_PackPop.h"

#endif // __DG_ECSCHEMA_H__
