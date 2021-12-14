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

#include "OdaCommon.h"
#include "ExDgnDumper.h"

//===================================================================================================

void OdExDgnDumper::writeFieldValue(const OdString& name, const OdDgECClassInstancePtr& pClass)
{
  // Dump default class instances.
  OdDgECClassReference classRef = pClass->getECClassReference();

  OdString strClassName = convertECSchemaItemNameToString(classRef.getClassName());
  OdString strSchemaName = convertECSchemaItemNameToString(classRef.getSchemaName());
  OdString strSchemaPrefix = convertECSchemaItemNameToString(classRef.getSchemaPrefix());

  writeFieldValue(name + strClassName, strSchemaName + L"." + classRef.getSchemaVersion() + L" ( " + strSchemaPrefix + L" )");

  OdString strPropPrefix;

  for (OdInt32 k = 0; k < name.getLength(); k++)
    strPropPrefix += L" ";

  strPropPrefix += L" = ";

  for( OdUInt32 i = 0; i < pClass->getPropertyInstanceCount(); i++ )
  {
    OdDgECPropertyInstance propInstance = pClass->getPropertyInstance(i);

    OdString strTypeName;
    OdDgECClassReference typeRef;

    switch (propInstance.getType())
    {
      case OdDgECPropertyInstance::kUndefined:
      {
        strTypeName = " <unknownType>";
        writeFieldValue(strPropPrefix + propInstance.getName() + strTypeName, OdString(L"--No data--"));
      } break;

      case OdDgECPropertyInstance::kBool:
      {
        strTypeName = " <bool>";
        writeFieldValue(strPropPrefix + propInstance.getName() + strTypeName, propInstance.getBoolValue());
      } break;

      case OdDgECPropertyInstance::kBoolArray:
      {
        strTypeName = " <boolArray>";
        writeFieldValue(strPropPrefix + propInstance.getName() + strTypeName, propInstance.getBoolArrayValue());
      } break;

      case OdDgECPropertyInstance::kByte:
      {
        strTypeName = " <byte>";
        writeFieldValue(strPropPrefix + propInstance.getName() + strTypeName, propInstance.getByteValue());
      } break;

      case OdDgECPropertyInstance::kByteArray:
      {
        strTypeName = " <byteArray>";
        writeFieldValue(strPropPrefix + propInstance.getName() + strTypeName, propInstance.getByteArrayValue());
      } break;

      case OdDgECPropertyInstance::kShort:
      {
        strTypeName = " <short>";
        writeFieldValue(strPropPrefix + propInstance.getName() + strTypeName, propInstance.getShortValue());
      } break;

      case OdDgECPropertyInstance::kShortArray:
      {
        strTypeName = " <shortArray>";
        writeFieldValue(strPropPrefix + propInstance.getName() + strTypeName, propInstance.getShortArrayValue());
      } break;

      case OdDgECPropertyInstance::kInt32:
      {
        strTypeName = " <int>";
        writeFieldValue(strPropPrefix + propInstance.getName() + strTypeName, propInstance.getIntValue());
      } break;

      case OdDgECPropertyInstance::kInt32Array:
      {
        strTypeName = " <intArray>";
        writeFieldValue(strPropPrefix + propInstance.getName() + strTypeName, propInstance.getIntArrayValue());
      } break;

      case OdDgECPropertyInstance::kInt64:
      {
        strTypeName = " <long>";
        writeFieldValue(strPropPrefix + propInstance.getName() + strTypeName, propInstance.getInt64Value());
      } break;

      case OdDgECPropertyInstance::kInt64Array:
      {
        strTypeName = " <longArray>";
        writeFieldValue(strPropPrefix + propInstance.getName() + strTypeName, propInstance.getInt64ArrayValue());
      } break;

      case OdDgECPropertyInstance::kDouble:
      {
        strTypeName = " <double>";
        writeFieldValue(strPropPrefix + propInstance.getName() + strTypeName, propInstance.getDoubleValue());
      } break;

      case OdDgECPropertyInstance::kDoubleArray:
      {
        strTypeName = " <doubleArray>";
        writeFieldValue(strPropPrefix + propInstance.getName() + strTypeName, propInstance.getDoubleArrayValue());
      } break;

      case OdDgECPropertyInstance::kBinary:
      {
        strTypeName = " <binary>";
        writeFieldValue(strPropPrefix + propInstance.getName() + strTypeName, propInstance.getBinaryValue());
      } break;

      case OdDgECPropertyInstance::kBinaryArray:
      {
        strTypeName = " <binaryArray>";
        writeFieldValue(strPropPrefix + propInstance.getName() + strTypeName, propInstance.getBinaryArrayValue());
      } break;

      case OdDgECPropertyInstance::kString:
      {
        strTypeName = " <string>";
        writeFieldValue(strPropPrefix + propInstance.getName() + strTypeName, convertECSchemaItemNameToString(propInstance.getStringValue()));
      } break;

      case OdDgECPropertyInstance::kStringArray:
      {
        strTypeName = " <stringArray>";
        writeFieldValue(strPropPrefix + propInstance.getName() + strTypeName, propInstance.getStringArrayValue());
      } break;

      case OdDgECPropertyInstance::kPoint3d:
      {
        strTypeName = " <point3d>";
        writeFieldValue(strPropPrefix + propInstance.getName() + strTypeName, propInstance.getPoint3dValue());
      } break;

      case OdDgECPropertyInstance::kPoint3dArray:
      {
        strTypeName = " <point3dArray>";
        writeFieldValue(strPropPrefix + propInstance.getName() + strTypeName, propInstance.getPoint3dArrayValue());
      } break;

      case OdDgECPropertyInstance::kDateAndTime:
      {
        strTypeName = " <dateTime>";
        writeFieldValue(strPropPrefix + propInstance.getName() + strTypeName, propInstance.getDateTimeValue());
      } break;

      case OdDgECPropertyInstance::kDateTimeArray:
      {
        strTypeName = " <dateTimeArray>";
        writeFieldValue(strPropPrefix + propInstance.getName() + strTypeName, propInstance.getDateTimeArrayValue());
      } break;

      case OdDgECPropertyInstance::kECClassInstance:
      {
        OdDgECClassInstancePtr pClassInst = propInstance.getClassInstanceValue();
        OdDgECClassReference classRef = pClassInst->getECClassReference();
        OdString strClassName  = convertECSchemaItemNameToString(classRef.getClassName());
        OdString strSchemaPrefix = convertECSchemaItemNameToString(classRef.getSchemaPrefix());
        writeFieldValue(strPropPrefix + convertECSchemaItemNameToString(propInstance.getName()), strSchemaPrefix + L":" + strClassName);

        OdString strClassInstPrefix;

        for (OdInt32 l = 0; l < strPropPrefix.getLength(); l++)
          strClassInstPrefix += L" ";

        strClassInstPrefix += L" + ";

        writeFieldValue(strClassInstPrefix, pClassInst);
      } break;
      case OdDgECPropertyInstance::kECClassInstanceArray:
      {
        OdArray<OdDgECClassInstancePtr> pClassInstArr = propInstance.getClassInstanceArrayValue();

        if (pClassInstArr.isEmpty())
        {
          writeFieldValue(strPropPrefix + propInstance.getName(), OdString(L"<--Empty Class Instance Array-->"));
        }
        else
        {
          OdDgECClassReference classRef = pClassInstArr[0]->getECClassReference();
          OdString strClassName = convertECSchemaItemNameToString(classRef.getClassName());
          OdString strSchemaPrefix = convertECSchemaItemNameToString(classRef.getSchemaPrefix());
          writeFieldValue(strPropPrefix + convertECSchemaItemNameToString(propInstance.getName()), L"Array of < " + strSchemaPrefix + L":" + strClassName + L" >");

          OdString strClassInstPrefix;

          for (OdInt32 l = 0; l < strPropPrefix.getLength(); l++)
            strClassInstPrefix += L" ";

          strClassInstPrefix += L" + ";

          for( OdUInt32 n = 0; n < pClassInstArr.size(); n++ )
            writeFieldValue(strClassInstPrefix, pClassInstArr[n]);
        }
      } break;
    }
  }
}

//===================================================================================================

void OdExDgnDumper::writeFieldValue(const OdString& name, const OdDgECClassPtr& pClass)
{
  if( !pClass.isNull() && pClass->isKindOf(OdDgECRelationshipClass::desc()) )
  {
    OdDgECRelationshipClassPtr pRelClass = pClass;
    dumpFieldName(L" ");
    writeFieldValue(name.left(name.getLength() - 3) + OdString(L"  Relationship Class : " + convertECSchemaItemNameToString(pRelClass->getName())), OdString(L" "));
    writeFieldValue(name + L"Name", convertECSchemaItemNameToString(pRelClass->getName()));
    writeFieldValue(name + L"DisplayName", pRelClass->getDisplayName());
    writeFieldValue(name + L"Description", pRelClass->getDescription());
    writeFieldValue(name + L"Type", pRelClass->getType());
    writeFieldValue(name + L"strength", pRelClass->getStrength());
    writeFieldValue(name + L"direction", pRelClass->getStrengthDirection());

    OdString strPrefix;

    for (OdInt32 l = 0; l < name.getLength(); l++)
      strPrefix += L" ";

    if (pClass->getBaseClassCount() > 0)
    {
      dumpFieldName(name + L"------ Parent classes ------------");

      for (OdUInt32 i = 0; i < pRelClass->getBaseClassCount(); i++)
      {
        OdDgECClassReference classRef = pRelClass->getBaseClass(i);
        OdString strClassName = convertECSchemaItemNameToString(classRef.getClassName());
        OdString strSchemaName = convertECSchemaItemNameToString(classRef.getSchemaName());
        OdString strSchemaPrefix = convertECSchemaItemNameToString(classRef.getSchemaPrefix());

        writeFieldValue(strPrefix + L" - " + strClassName,
          strSchemaName + L"." + classRef.getSchemaVersion() + L" ( " + strSchemaPrefix + L" )");
      }
    }

    if (pRelClass->getCustomAttributeCount() > 0)
    {
      dumpFieldName(name + L"----- Relationship Class Custom Attributes ------");

      for (OdUInt32 j = 0; j < pRelClass->getCustomAttributeCount(); j++)
        writeFieldValue(strPrefix + L" - ", pRelClass->getCustomAttribute(j));
    }

    if (pRelClass->getPropertyCount() > 0)
    {
      dumpFieldName(name + L"------ Properties ------------------------");

      for (OdUInt32 k = 0; k < pRelClass->getPropertyCount(); k++)
        writeFieldValue(strPrefix + L" - ", pRelClass->getProperty(k));
    }

    writeFieldValue(strPrefix + L" - ", pRelClass->getSource(), true);
    writeFieldValue(strPrefix + L" - ", pRelClass->getTarget(), false);
  }
  else   if (!pClass.isNull() && pClass->isKindOf(OdDgECClass::desc()))
  {
    dumpFieldName(L" ");
    writeFieldValue(name.left(name.getLength() - 3) + OdString(L"Class : " + convertECSchemaItemNameToString(pClass->getName())), OdString(L" "));
    writeFieldValue(name + L"Name", convertECSchemaItemNameToString(pClass->getName()));
    writeFieldValue(name + L"DisplayName", pClass->getDisplayName());
    writeFieldValue(name + L"Description", pClass->getDescription());
    writeFieldValue(name + L"Type", pClass->getType());

    OdString strPrefix;

    for (OdInt32 l = 0; l < name.getLength(); l++)
      strPrefix += L" ";

    if (pClass->getCustomAttributeCount() > 0)
    {
      dumpFieldName(name + L"----- Class Custom Attributes -------------");

      for (OdUInt32 j = 0; j < pClass->getCustomAttributeCount(); j++)
        writeFieldValue(strPrefix + L" - ", pClass->getCustomAttribute(j));
    }

    if (pClass->getPropertyCount() > 0)
    {
      dumpFieldName(name + L"------ Properties ------------------------");

      for (OdUInt32 k = 0; k < pClass->getPropertyCount(); k++)
        writeFieldValue(strPrefix + L" - ", pClass->getProperty(k));
    }
  }
}

//===================================================================================================

void OdExDgnDumper::writeFieldValue(const OdString& name, const OdDgRelationshipNodePtr& pNode)
{
  OdString strValue;

  if( pNode->isKindOf(OdDgRelationshipElementIdNode::desc()) )
  {
    OdDgRelationshipElementIdNodePtr pIdNode = pNode;

    strValue.format(L"Id = 0x%X", pIdNode->getElementId());

    if( pIdNode->getUseHandlerNameFlag() )
    {
      strValue += L", HandlerName = \"" + pIdNode->getHandlerName() + "\"";

      if (pIdNode->getExtendedUInt64Value())
      {
        OdString strFormat;
        strFormat.format(L", ExtInt64 = %d", pIdNode->getExtendedUInt64Value());
        strValue += strFormat;
      }

      if (pIdNode->getUseObjectNameFlag())
        strValue += L", ObjName = " + pIdNode->getObjectName();
    }
    else
    {
      OdString strExt;
      strExt.format(L", ExtInt32 = %d", pIdNode->getExtendedUInt32Value());
      strValue += strExt;

      if (pIdNode->getExtendedUInt64Value())
      {
        OdString strFormat;
        strFormat.format(L", ExtInt64 = %d", pIdNode->getExtendedUInt64Value());
        strValue += strFormat;
      }

      if (pIdNode->getUseObjectNameFlag())
        strValue += L", ExtStr = " + pIdNode->getExtendedString();
    }
  }
  else if (pNode->isKindOf(OdDgRelationshipElementIdExtNode::desc()))
  {
    OdDgRelationshipElementIdExtNodePtr pIdNode = pNode;

    strValue.format(L"Index = %d, Id = 0x%X", pIdNode->getIndex(), pIdNode->getElementId());

    if (pIdNode->getUseHandlerNameFlag())
    {
      strValue += L", HandlerName = \"" + pIdNode->getHandlerName() + "\"";

      if (pIdNode->getExtendedUInt64Value())
      {
        OdString strFormat;
        strFormat.format(L", ExtInt64 = %d", pIdNode->getExtendedUInt64Value());
        strValue += strFormat;
      }

      if (pIdNode->getUseObjectNameFlag())
        strValue += L", ObjName = " + pIdNode->getObjectName();
    }
    else
    {
      OdString strExt;
      strExt.format(L", ExtInt32 = %d", pIdNode->getExtendedUInt32Value());
      strValue += strExt;

      if (pIdNode->getExtendedUInt64Value())
      {
        OdString strFormat;
        strFormat.format(L", ExtInt64 = %d", pIdNode->getExtendedUInt64Value());
        strValue += strFormat;
      }

      if (pIdNode->getUseObjectNameFlag())
        strValue += L", ExtStr = " + pIdNode->getExtendedString();
    }
  }
  else if (pNode->isKindOf(OdDgRelationshipFarElementIdNode::desc()))
  {
    OdDgRelationshipFarElementIdNodePtr pIdNode = pNode;

    strValue.format(L"XRefId = 0x%X, Index = %d, Id = 0x%X", pIdNode->getXRefId(), pIdNode->getIndex(), pIdNode->getElementId());

    if (pIdNode->getUseHandlerNameFlag())
    {
      strValue += L", HandlerName = \"" + pIdNode->getHandlerName() + "\"";

      if (pIdNode->getExtendedUInt64Value())
      {
        OdString strFormat;
        strFormat.format(L", ExtInt64 = %d", pIdNode->getExtendedUInt64Value());
        strValue += strFormat;
      }

      if (pIdNode->getUseObjectNameFlag())
        strValue += L", ObjName = " + pIdNode->getObjectName();
    }
    else
    {
      OdString strExt;
      strExt.format(L", ExtInt32 = %d", pIdNode->getExtendedUInt32Value());
      strValue += strExt;

      if (pIdNode->getExtendedUInt64Value())
      {
        OdString strFormat;
        strFormat.format(L", ExtInt64 = %d", pIdNode->getExtendedUInt64Value());
        strValue += strFormat;
      }

      if (pIdNode->getUseObjectNameFlag())
        strValue += L", ExtStr = " + pIdNode->getExtendedString();
    }
  }
  else if (pNode->isKindOf(OdDgRelationshipFarElementIdExtNode::desc()))
  {
    OdDgRelationshipFarElementIdExtNodePtr pIdNode = pNode;

    strValue.format(L"XRefIndex = %d, XRefId = 0x%X, Index = %d, Id = 0x%X", pIdNode->getXRefIndex(), pIdNode->getXRefId(), pIdNode->getIndex(), pIdNode->getElementId());

    if (pIdNode->getUseHandlerNameFlag())
    {
      strValue += L", HandlerName = \"" + pIdNode->getHandlerName() + "\"";

      if (pIdNode->getExtendedUInt64Value())
      {
        OdString strFormat;
        strFormat.format(L", ExtInt64 = %d", pIdNode->getExtendedUInt64Value());
        strValue += strFormat;
      }

      if (pIdNode->getUseObjectNameFlag())
        strValue += L", ObjName = " + pIdNode->getObjectName();
    }
    else
    {
      OdString strExt;
      strExt.format(L", ExtInt32 = %d", pIdNode->getExtendedUInt32Value());
      strValue += strExt;

      if (pIdNode->getExtendedUInt64Value())
      {
        OdString strFormat;
        strFormat.format(L", ExtInt64 = %d", pIdNode->getExtendedUInt64Value());
        strValue += strFormat;
      }

      if (pIdNode->getUseObjectNameFlag())
        strValue += L", ExtStr = " + pIdNode->getExtendedString();
    }
  }
  else
  {
    OdDgRelationshipProxyNodePtr pProxyNode = pNode;
    strValue.format(L"NumItems = %d", pProxyNode->getSubNodeCount());
    OdBinaryData binData = pProxyNode->getProxyData(0);

    OdString strBinData;

    for(OdUInt32 j = 0; j < binData.size(); j++)
    {
      OdString strItem;

      if (j < binData.size() - 1)
        strItem.format(L"%02X,", binData[j]);
      else
        strItem.format(L"%02X", binData[j]);
        
      strBinData += strItem;
    }

    strValue += L",BinData = {" + strBinData + L"}";
  }

  writeFieldValue(name, strValue);
}

//===================================================================================================

void OdExDgnDumper::writeFieldValue(const OdString& name,
                                      OdDgECProperty::OdDgECPropertyType uType,
                                        const OdDgECClassReference& typeRef)
{
  switch (uType)
  {
  case OdDgECProperty::kECClassReference:
  {
    OdString strClassName    = convertECSchemaItemNameToString(typeRef.getClassName());
    OdString strSchemaPrefix = convertECSchemaItemNameToString(typeRef.getSchemaPrefix());
    writeFieldValue(name, strSchemaPrefix + L":" + strClassName);
  }break;
  case OdDgECProperty::kString:
    writeFieldValue(name, OdString(L"kString")); break;
  case OdDgECProperty::kDouble:
    writeFieldValue(name, OdString(L"kDouble")); break;
  case OdDgECProperty::kInteger:
    writeFieldValue(name, OdString(L"kInteger")); break;
  case OdDgECProperty::kLong:
    writeFieldValue(name, OdString(L"kLong")); break;
  case OdDgECProperty::kBoolean:
    writeFieldValue(name, OdString(L"kBoolean")); break;
  case OdDgECProperty::kDateTime:
    writeFieldValue(name, OdString(L"kDateTime")); break;
  case OdDgECProperty::kPoint3d:
    writeFieldValue(name, OdString(L"kPoint3d")); break;
  case OdDgECProperty::kBinary:
    writeFieldValue(name, OdString(L"kBinary")); break;
  }
}

//===================================================================================================

void OdExDgnDumper::writeFieldValue(const OdString& name, const OdDgECPropertyPtr& pProperty)
{
  OdDgECClassReference typeRef;
  OdDgECProperty::OdDgECPropertyType uType = pProperty->getPropertyType(typeRef);
  writeFieldValue(name.left(name.getLength() - 3) + OdString(L"Property : " + convertECSchemaItemNameToString(pProperty->getName())), OdString(L" "));
  writeFieldValue(name + L"Name", convertECSchemaItemNameToString(pProperty->getName()));
  writeFieldValue(name + L"DisplayName", pProperty->getDisplayName());
  writeFieldValue(name + L"Description", pProperty->getDescription());
  writeFieldValue(name + L"Type", uType, typeRef);
  writeFieldValue(name + L"ReadOnly", pProperty->getReadOnlyFlag());
  writeFieldValue(name + L"IsArray", pProperty->isArrayProperty());
  writeFieldValue(name + L"IsStruct", pProperty->isArrayProperty());

  if( pProperty->isKindOf(OdDgECArrayProperty::desc()))
  {
    const OdDgECArrayPropertyPtr pArrayProperty = pProperty;
    writeFieldValue(name + L"MinOccurs", pArrayProperty->getMinOccurs());
    writeFieldValue(name + L"MaxOccurs", pArrayProperty->getMaxOccurs());
  }

  OdString strPrefix;

  for (OdInt32 l = 0; l < name.getLength(); l++)
    strPrefix += L" ";

  if (pProperty->getCustomAttributeCount() > 0)
  {
    dumpFieldName(name + L"+++++ Property Custom Attributes +++++++++++++");

    for (OdUInt32 j = 0; j < pProperty->getCustomAttributeCount(); j++)
      writeFieldValue(strPrefix + L" + ", pProperty->getCustomAttribute(j));
  }
}

//===================================================================================================

void OdExDgnDumper::writeFieldValue(const OdString& name, const OdDgECRelationshipPtr& pRelationship, bool bSource)
{
  if (pRelationship.isNull())
    return;

  dumpFieldName(L" ");

  if( bSource )
    writeFieldValue(name.left(name.getLength() - 3) + OdString(L"  Source Relationship: "), OdString(L" "));
  else
    writeFieldValue(name.left(name.getLength() - 3) + OdString(L"  Target Relationship: "), OdString(L" "));

  dumpFieldName(L" ");

  writeFieldValue(name + L"Role Label", pRelationship->getRoleLabel());
  writeFieldValue(name + L"Cardinality", pRelationship->getCardinality());
  writeFieldValue(name + L"Polymorphic", pRelationship->getPolymorphicFlag());

  OdString strPrefix;

  for (OdInt32 l = 0; l < name.getLength(); l++)
    strPrefix += L" ";

  if (pRelationship->getClassCount() > 0)
  {
    dumpFieldName(name + L"------ Classes ------------");

    for (OdUInt32 i = 0; i < pRelationship->getClassCount(); i++)
    {
      OdDgECClassReference classRef = pRelationship->getClass(i);

      OdString strClassName  = convertECSchemaItemNameToString(classRef.getClassName());
      OdString strSchemaName = convertECSchemaItemNameToString(classRef.getSchemaName());
      OdString strSchemaPrefix = convertECSchemaItemNameToString(classRef.getSchemaPrefix());

      writeFieldValue(strPrefix + L" - " + strClassName,
        strSchemaName + L"." + classRef.getSchemaVersion() + L" ( " + strSchemaPrefix + L" )");
    }
  }

  if( pRelationship->getCustomAttributeCount() > 0 )
  {
    dumpFieldName(name + L"----- Class Custom Attributes -------------");

    for (OdUInt32 j = 0; j < pRelationship->getCustomAttributeCount(); j++)
      writeFieldValue(strPrefix + L" - ", pRelationship->getCustomAttribute(j));
  }
}

//===================================================================================================
