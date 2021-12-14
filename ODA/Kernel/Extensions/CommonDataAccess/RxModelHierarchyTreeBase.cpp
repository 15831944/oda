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

#include "OdaCommon.h"
#include "RxModelHierarchyTreeBase.h"
#include "RxValue.h"
#include "RxValueTypeUtil.h"
#include "RxMember.h"
#include "RxObjectImpl.h"
#include "RxAttribute.h"
#include "RxSysRegistry.h"
#include "RxModelHierarchyTreeBaseImpl.h"
#include "RxProperty.h"
#include "RxModelTreeBaseNodeImpl.h"


ODRX_CONS_DEFINE_MEMBERS(OdRxModelHierarchyTreeBase, OdRxObject, RXIMPL_CONSTR)

#define _GET_IMPL_ \
  \
  OdRxModelHierarchyTreeBaseImpl* pImpl = OdRxModelHierarchyTreeBaseImpl::getImpl(this); \
  if (NULL == pImpl)\
  { \
    throw OdError(::eNullObjectPointer); \
  } 

class OdRxModelHierarchyTreeImpl : public OdRxModelHierarchyTreeBaseImpl
{
public:
  OdRxModelHierarchyTreeImpl() : OdRxModelHierarchyTreeBaseImpl()
  {

  }

  OdRxModelTreeBaseNodePtr createNodeObject()
  {
    return OdRxModelTreeBaseNode::createObject();
  }
};

OdRxModelHierarchyTreeBase::OdRxModelHierarchyTreeBase()
  :m_pImpl(new OdRxModelHierarchyTreeImpl)
{

}

OdRxModelHierarchyTreeBase::OdRxModelHierarchyTreeBase(OdRxModelHierarchyTreeBaseImpl* impl)
  :m_pImpl(impl)
{

}

OdRxModelHierarchyTreeBase::~OdRxModelHierarchyTreeBase()
{
  delete m_pImpl;
}

OdRxModelTreeBaseNodePtr OdRxModelHierarchyTreeBase::createDatabaseHierarchyTree(const OdRxObject* pDb, 
  const std::set<OdString>* collected_properties, const bool class_dependent_hierarchy, const bool class_dependent_property)
{
  _GET_IMPL_
   return  pImpl->createDatabaseHierarchyTree(pDb, collected_properties, class_dependent_hierarchy, class_dependent_property);
}


OdRxModelTreeBaseNodePtr OdRxModelHierarchyTreeBase::createDatabaseHierarchyTree(const OdRxObject* pDb, 
  const bool create_properties_cache /*= true*/, const bool class_dependent_hierarchy /*= false*/, const bool class_dependent_property /*= false*/)
{
  _GET_IMPL_
  return  pImpl->createDatabaseHierarchyTree(pDb, create_properties_cache, class_dependent_hierarchy, class_dependent_property);
}

OdRxModelTreeBaseNodePtr OdRxModelHierarchyTreeBase::getDatabaseNode() const
{
  _GET_IMPL_
  return  pImpl->getDatabaseNode();
}


const OdArray<PropertyInfo>& OdRxModelHierarchyTreeBase::getNodeProperties(OdUInt64 unique_DB_ID, OdUInt64 unique_source_ID)
{
  _GET_IMPL_
  return  pImpl->getNodeProperties(unique_DB_ID, unique_source_ID);
}

void OdRxModelHierarchyTreeBase::serialize(OdBaseHierarchyTreeFiler* pFiler) const
{
  _GET_IMPL_
  return  pImpl->serialize(pFiler);
}

OdResult OdRxModelHierarchyTreeBase::deserialize(OdBaseHierarchyTreeFiler* pFiler)
{
  _GET_IMPL_
  return  pImpl->deserialize(pFiler);
}

struct SinglePropertyFunctor
{
  template <class ValueType>
  static bool processValue(OdRxPropertyVariant& variant, const OdRxValue& value)
  {
    const ValueType* val = rxvalue_cast<ValueType>(&value);
    if (val == 0)
      return false;
    variant = *val;
    return true;
  }
  static bool processDefaultValue(OdRxPropertyVariant& variant, const OdString& value)
  {
    variant = value;
    return true;
  }
};

template <>
bool SinglePropertyFunctor::processValue<OdAnsiString>(OdRxPropertyVariant& variant, const OdRxValue& value)
{
  const OdAnsiString* val = rxvalue_cast<OdAnsiString>(&value);
  if (val == 0)
    return false;
  variant.setAnsiString(*val);
  return true;
}

template<typename BaseType>
bool processBaseTypeSingle(OdRxPropertyVariant& variant, const OdRxValue& value)
{

  const BaseType* val = rxvalue_cast<BaseType>(&value);
  if (val == 0)
    return false;
  if (sizeof(BaseType) <= sizeof(OdInt32))
    variant.setInt32(*val);
  else
    variant.setInt64(*val);
  return true;
}

#if OD_SIZEOF_LONG == 4
template <>
bool SinglePropertyFunctor::processValue<int>(OdRxPropertyVariant& variant, const OdRxValue& value)
{
  return processBaseTypeSingle<int>(variant, value);
}

template <>
bool SinglePropertyFunctor::processValue<unsigned int>(OdRxPropertyVariant& variant, const OdRxValue& value)
{
  return processBaseTypeSingle<unsigned int>(variant, value);
}
#else 
#if OD_SIZEOF_LONG != 8
template <>
bool SinglePropertyFunctor::processValue<long>(OdRxPropertyVariant& variant, const OdRxValue& value)
{
  return processBaseTypeSingle<long>(variant, value);
}

template <>
bool SinglePropertyFunctor::processValue<unsigned long>(OdRxPropertyVariant& variant, const OdRxValue& value)
{
  return processBaseTypeSingle<unsigned long>(variant, value);
}
#endif
#endif

#undef TYPE_CONTAINER
#define TYPE_CONTAINER \
  VARTYPE(AnsiString )\
  VARTYPE(String )\
  VARTYPE(Bool   )\
  VARTYPE(Int8   )\
  VARTYPE(Int16  )\
  VARTYPE(Int32  )\
  VARTYPE(Int64  )\
  VARTYPE(UInt8  )\
  VARTYPE(UInt16 )\
  VARTYPE(UInt32 )\
  VARTYPE(UInt64 )\
  VARTYPE(Double )\
  VARTYPE(GePoint2d      )\
  VARTYPE(GePoint3d      )\
  VARTYPE(GeVector2d     )\
  VARTYPE(GeVector3d     )\
  VARTYPE(CmEntityColor  )\
  VARTYPE(CmTransparency )\
  VARTYPE(GeMatrix2d)\
  VARTYPE(GeMatrix3d)\
  VARTYPE(TimeStamp)\
  VARTYPE(GeExtents2d)\
  VARTYPE(GeExtents3d)\
  VARTYPE(GeQuaternion)\
  VARTYPE(GiLightAttenuation)\
  VARTYPE(GiSkyParameters)\
  VARTYPE(GiShadowParameters)\
  VARTYPE(GiColorRGB)\
  VARTYPE(GiMaterialColor)\
  VARTYPE(GiMrExposureType)\
  VARTYPE(GiMrShadowMode)\
  VARTYPE(GiMrFinalGatheringMode)\
  VARTYPE(GiMrDiagnosticMode)\
  VARTYPE(GiMrDiagnosticPhotonMode)\
  VARTYPE(GiMrDiagnosticBSPMode)\
  VARTYPE(GiMrExportMIMode)\
  VARTYPE(GiMrTileOrder)\
  VARTYPE(GiLightingMode)\
  VARTYPE(GiFilterType)\
  VARTYPEENUM(CmEntityColor, ColorMethod)\
  VARTYPEENUM(Db, LineWeight)\
  VARTYPEENUM(Db, UnitsValue)\
  VARTYPEENUM(GiDgLinetypeModifiers, WidthMode)\
  VARTYPEENUM(GiDgLinetypeModifiers, ShiftMode)\
  VARTYPEENUM(GiDgLinetypeModifiers, CornersMode)\
  VARTYPEENUM(Ge, OdGeKnotParameterization)\
  VARTYPEENUM(GiLightAttenuation, AttenuationType)\
  VARTYPEENUM(GiDrawable, DrawableType)\
  VARTYPEENUM(GiShadowParameters, ShadowType)\
  VARTYPEENUM(GiShadowParameters, ExtendedLightShape)\
  VARTYPEENUM(GiViewportTraits, DefaultLightingType)\
  VARTYPEENUM(GiMaterialTraits, LuminanceMode)\
  VARTYPEENUM(GiMaterialTraits, GlobalIlluminationMode)\
  VARTYPEENUM(GiMaterialTraits, FinalGatherMode)\
  VARTYPEENUM(GiMaterialTraits, ChannelFlags)\
  VARTYPEENUM(GiMaterialTraits, Mode)\
  VARTYPEENUM(GiMaterialTraits, IlluminationModel)\
  VARTYPEENUM(GiVisualStyle, Type)\
  VARTYPEENUM(GiVisualStyleProperties, FaceColorMode)\
  VARTYPEENUM(GiVisualStyleProperties, EdgeModel)\
  VARTYPEENUM(GiFaceStyle, FaceColorMode)\
  VARTYPEENUM(GiEdgeStyle, EdgeModel)\
  VARTYPEENUM(GiToneOperatorParameters, ExteriorDaylightMode)


#define ODTYPE(Type) Od##Type

typedef bool (*process_function) (OdRxPropertyVariant& variant, const OdRxValue& value);
static std::map<const OdRxValueType*, process_function> SinglePropsFuncMap;
static std::map<const OdRxValueType*, process_function> CollectionInitPropsFuncMap;
static std::map<const OdRxValueType*, process_function> CollectionPropsFuncMap;

struct CollectionInitPropertyFunctor
{
  template <class ValueType>
  static bool processValue(OdRxPropertyVariant& variant, const OdRxValue& value)
  {
    return false;
  }

  static bool processDefaultValue(OdRxPropertyVariant& variant, const OdString& value)
  {
    OdStringArray array;
    variant.setStringArray(array);
    return true;
  }
};


#undef VARTYPE
#define VARTYPE(Type) \
template <>\
bool CollectionInitPropertyFunctor::processValue<ODTYPE(Type)>(OdRxPropertyVariant& variant, const OdRxValue& value)\
{\
  Od##Type##Array array;\
  variant.set##Type##Array(array);\
  return true;\
}

#undef VARTYPEENUM
#define VARTYPEENUM(NameSp, Type) \
template <>\
bool CollectionInitPropertyFunctor::processValue<Od##NameSp::Type>(OdRxPropertyVariant& variant, const OdRxValue& value)\
{\
  Od##NameSp##_##Type##Array array;\
  variant.set##NameSp##_##Type##Array(array);\
  return true;\
}

TYPE_CONTAINER

template<typename BaseType>
void processBaseTypeInit(OdRxPropertyVariant& variant, const OdRxValue& value)
{
  if (sizeof(BaseType) <= sizeof(OdInt32))
  {
    OdInt32Array array;
    variant.setInt32Array(array);
  }
  else
  {
    OdInt64Array array;
    variant.setInt64Array(array);
  }
}

#if OD_SIZEOF_LONG == 4
template <>
bool CollectionInitPropertyFunctor::processValue<int>(OdRxPropertyVariant& variant, const OdRxValue& value)
{
  processBaseTypeInit<int>(variant, value);
  return true;
}

template <>
bool CollectionInitPropertyFunctor::processValue<unsigned int>(OdRxPropertyVariant& variant, const OdRxValue& value)
{
  processBaseTypeInit<unsigned int>(variant, value);
  return true;
}
#else 
#if OD_SIZEOF_LONG != 8
template <>
bool CollectionInitPropertyFunctor::processValue<long>(OdRxPropertyVariant& variant, const OdRxValue& value)
{
  processBaseTypeInit<long>(variant, value);
  return true;
}

template <>
bool CollectionInitPropertyFunctor::processValue<unsigned long>(OdRxPropertyVariant& variant, const OdRxValue& value)
{
  processBaseTypeInit<unsigned long>(variant, value);
  return true;
}
#endif
#endif

template <>
bool CollectionInitPropertyFunctor::processValue<float>(OdRxPropertyVariant& variant, const OdRxValue& value)
{
  OdDoubleArray array;
  variant.setDoubleArray(array);
  return true;
}

struct CollectionPropertyFunctor
{
  template <class ValueType>
  static bool processValue(OdRxPropertyVariant& variant, const OdRxValue& value)
  {
    return false;
  }

  static bool processDefaultValue(OdRxPropertyVariant& variant, const OdString& value)
  {
    if (OdRxPropertyVariant::kString == variant.varType())
      variant.asStringArray().append(value);
    return true;
  }
};

#define ODTYPE(Type) Od##Type

#undef VARTYPE
#define VARTYPE(Type) \
template <>\
bool CollectionPropertyFunctor::processValue<ODTYPE(Type)>(OdRxPropertyVariant& variant, const OdRxValue& value)\
{\
  const ODTYPE(Type)* val = rxvalue_cast<ODTYPE(Type)>(&value);\
  if (val != 0  && (OdRxPropertyVariant::k##Type | OdRxPropertyVariant::kArray) == variant.type())\
    variant.as##Type##Array().append(*val);\
  return true;\
}

#undef VARTYPEENUM
#define VARTYPEENUM(NameSp, Type) \
template <>\
bool CollectionPropertyFunctor::processValue<Od##NameSp::Type>(OdRxPropertyVariant& variant, const OdRxValue& value)\
{\
  const Od##NameSp::Type* val = rxvalue_cast<Od##NameSp::Type>(&value);\
  if (val != 0  && (OdRxPropertyVariant::k##NameSp##_##Type | OdRxPropertyVariant::kArray) == variant.type())\
    variant.as##NameSp##_##Type##Array().append(*val);\
  return true;\
}

TYPE_CONTAINER

template<typename BaseType>
void processBaseTypeCollect(OdRxPropertyVariant& variant, const OdRxValue& value)
{
  if (sizeof(BaseType) <= sizeof(OdInt32))
  {
    const BaseType* val = rxvalue_cast<BaseType>(&value);
    if (val != 0 && (OdRxPropertyVariant::kInt32 | OdRxPropertyVariant::kArray) == variant.type())
      variant.asInt32Array().append(*val);
  }
  else
  {
    const BaseType* val = rxvalue_cast<BaseType>(&value);
    if (val != 0 && (OdRxPropertyVariant::kInt64 | OdRxPropertyVariant::kArray) == variant.type())
      variant.asInt64Array().append(*val);
  }
}

#if OD_SIZEOF_LONG == 4
template <>
bool CollectionPropertyFunctor::processValue<int>(OdRxPropertyVariant& variant, const OdRxValue& value)
{
  processBaseTypeCollect<int>(variant, value);
  return true;
}

template <>
bool CollectionPropertyFunctor::processValue<unsigned int>(OdRxPropertyVariant& variant, const OdRxValue& value)
{
  processBaseTypeCollect<unsigned int>(variant, value);
  return true;
}
#else 
#if OD_SIZEOF_LONG != 8
template <>
bool CollectionPropertyFunctor::processValue<long>(OdRxPropertyVariant& variant, const OdRxValue& value)
{
  processBaseTypeCollect<long>(variant, value);
  return true;
}

template <>
bool CollectionPropertyFunctor::processValue<unsigned long>(OdRxPropertyVariant& variant, const OdRxValue& value)
{
  processBaseTypeCollect<unsigned long>(variant, value);
  return true;
}
#endif
#endif

template <>
bool CollectionPropertyFunctor::processValue<float>(OdRxPropertyVariant& variant, const OdRxValue& value)
{
  const float* val = rxvalue_cast<float>(&value);
  if (val != 0 && (OdRxPropertyVariant::kDouble | OdRxPropertyVariant::kArray) == variant.type())
    variant.asDoubleArray().append(*val);
  return true;
}

void init()
{

#undef VARTYPE
#define VARTYPE(Type) \
SinglePropsFuncMap.insert(std::make_pair(&OdRxValueType::Desc<ODTYPE(Type)>().value(), SinglePropertyFunctor::processValue<ODTYPE(Type)>));

#undef VARTYPEENUM
#define VARTYPEENUM(NameSp, Type) VARTYPE(NameSp::Type)
  
  TYPE_CONTAINER

  SinglePropsFuncMap.insert(std::make_pair(&OdRxValueType::Desc<float>().value(), SinglePropertyFunctor::processValue<float>));

#if OD_SIZEOF_LONG == 4
  SinglePropsFuncMap.insert(std::make_pair(&OdRxValueType::Desc<int>().value(), SinglePropertyFunctor::processValue<int>));
  SinglePropsFuncMap.insert(std::make_pair(&OdRxValueType::Desc<unsigned int>().value(), SinglePropertyFunctor::processValue<unsigned int>));
#else
#if OD_SIZEOF_LONG != 8
  SinglePropsFuncMap.insert(std::make_pair(&OdRxValueType::Desc<long>().value(), SinglePropertyFunctor::processValue<long>));
  SinglePropsFuncMap.insert(std::make_pair(&OdRxValueType::Desc<unsigned long>().value(), SinglePropertyFunctor::processValue<unsigned long>));
#endif
#endif

#undef VARTYPE
#define VARTYPE(Type) \
CollectionInitPropsFuncMap.insert(std::make_pair(&OdRxValueType::Desc<ODTYPE(Type)>().value(), CollectionInitPropertyFunctor::processValue<ODTYPE(Type)>));

#undef VARTYPEENUM
#define VARTYPEENUM(NameSp, Type) VARTYPE(NameSp::Type)

  TYPE_CONTAINER

  CollectionInitPropsFuncMap.insert(std::make_pair(&OdRxValueType::Desc<float>().value(), CollectionInitPropertyFunctor::processValue<float>));

#if OD_SIZEOF_LONG == 4
  CollectionInitPropsFuncMap.insert(std::make_pair(&OdRxValueType::Desc<int>().value(), CollectionInitPropertyFunctor::processValue<int>));
  CollectionInitPropsFuncMap.insert(std::make_pair(&OdRxValueType::Desc<unsigned int>().value(), CollectionInitPropertyFunctor::processValue<unsigned int>));
#else
#if OD_SIZEOF_LONG != 8
  CollectionInitPropsFuncMap.insert(std::make_pair(&OdRxValueType::Desc<long>().value(), CollectionInitPropertyFunctor::processValue<long>));
  CollectionInitPropsFuncMap.insert(std::make_pair(&OdRxValueType::Desc<unsigned long>().value(), CollectionInitPropertyFunctor::processValue<unsigned long>));
#endif
#endif



#undef VARTYPE
#define VARTYPE(Type) \
CollectionPropsFuncMap.insert(std::make_pair(&OdRxValueType::Desc<ODTYPE(Type)>().value(), CollectionPropertyFunctor::processValue<ODTYPE(Type)>));

#undef VARTYPEENUM
#define VARTYPEENUM(NameSp, Type) VARTYPE(NameSp::Type)

  TYPE_CONTAINER

  CollectionPropsFuncMap.insert(std::make_pair(&OdRxValueType::Desc<float>().value(), CollectionPropertyFunctor::processValue<float>));

#if OD_SIZEOF_LONG == 4
  CollectionPropsFuncMap.insert(std::make_pair(&OdRxValueType::Desc<int>().value(), CollectionPropertyFunctor::processValue<int>));
  CollectionPropsFuncMap.insert(std::make_pair(&OdRxValueType::Desc<unsigned int>().value(), CollectionPropertyFunctor::processValue<unsigned int>));
#else
#if OD_SIZEOF_LONG != 8
  CollectionPropsFuncMap.insert(std::make_pair(&OdRxValueType::Desc<long>().value(), CollectionPropertyFunctor::processValue<long>));
  CollectionPropsFuncMap.insert(std::make_pair(&OdRxValueType::Desc<unsigned long>().value(), CollectionPropertyFunctor::processValue<unsigned long>));
#endif
#endif


}

OdRxModelHierarchyTreeBaseImpl::OdRxModelHierarchyTreeBaseImpl()
{
  if (0 == SinglePropsFuncMap.size())
    init();
}

OdRxModelHierarchyTreeBaseImpl::~OdRxModelHierarchyTreeBaseImpl()
{

}

OdRxModelTreeBaseNodePtr OdRxModelHierarchyTreeBaseImpl::getDatabaseNode() const
{
  return m_DatabaseNode;
}

OdRxModelTreeBaseNodePtr OdRxModelHierarchyTreeBaseImpl::createDatabaseHierarchyTree(const OdRxObject* pDb, 
  const bool create_properties_cache /*= true*/, const bool class_dependent_hierarchy /*= false*/, const bool class_dependent_property /*= false*/)
{
  m_StringStorage.clear();

  m_DatabaseNode = createNodeObject();

  m_NodeCache.clear();
  processNode(m_DatabaseNode, pDb, m_NodeCache, create_properties_cache, class_dependent_hierarchy, class_dependent_property, (OdUInt64)pDb);

  if (m_DatabaseNode->getNodeName().isEmpty())
  {
    OdRxModelTreeBaseNodeImpl* pImpl = OdRxModelTreeBaseNodeImpl::getImpl(m_DatabaseNode);
    OdRxPropertyPtr pFileNameProp = OdRxMemberQueryEngine::theEngine()->find(pDb, L"FileName");
    if (!pFileNameProp.isNull())
    {
      OdRxValue valFileName;
      if (eOk == pFileNameProp->getValue(pDb, valFileName))
      {
        OdString file_path = *rxvalue_cast<OdString>(&valFileName);
        int i1 = file_path.reverseFind('\\');
        int i2 = file_path.reverseFind('/');
        int i = odmax(i1, i2);
        file_path = file_path.mid(i + 1);
        i = file_path.find(L'.');
        if (i != -1)
          file_path = file_path.left(i);
        pImpl->setNodeName(file_path);
      }
    }
    if (m_DatabaseNode->getNodeName().isEmpty())
    {
      pImpl->setNodeName(L"Database");
    }
  }
  m_CollectedProperties.clear();
  return m_DatabaseNode;
}

OdRxModelTreeBaseNodePtr OdRxModelHierarchyTreeBaseImpl::createDatabaseHierarchyTree(const OdRxObject* pDb, 
  const std::set<OdString>* collected_properties, const bool class_dependent_hierarchy /*= false*/, 
  const bool class_dependent_property /*= false*/)
{
  if (NULL != collected_properties)
    m_CollectedProperties = *collected_properties;

  return createDatabaseHierarchyTree(pDb, NULL == collected_properties, class_dependent_hierarchy, class_dependent_property);
}

void OdRxModelHierarchyTreeBaseImpl::processElement(OdRxModelTreeBaseNode* pParent, const AttributesInfo& attributes_info, 
  OdRxValue& prop_value, std::map < OdUInt64, std::map<OdUInt64, OdRxModelTreeBaseNodePtr> >& cache,
  const bool create_properties_cache, const bool class_dependent_hierarchy, const bool class_dependent_property, const OdUInt64 databaseID)
{
  OdRxObjectPtr pChildObj;

  if (prop_value.type().isReference())
  {
    const IOdRxReferenceType* pRef = prop_value.type().reference();
    if (NULL == pRef)
      return;

    pChildObj = pRef->dereference(prop_value, IOdRxReferenceType::kForRead);
  }
  else
  {
    const IOdRxObjectValue* pValue = prop_value.type().rxObjectValue();
    if (NULL == pValue)
      return;
    pChildObj = pValue->getRxObject(prop_value);
  }

  if (NULL == pChildObj)
    return;

  OdString hierarchy_value = attributes_info.pHierarchyAttribute->value(prop_value);
  static const OdString str_Model_key_name(L"Model");
  static const OdString str_BlockReference_key_name(L"BlockReference");
  static const OdString str_Block_key_name(L"Block");
  static const OdString str_Hidden_key_name(L"Hidden");
  static const OdString str_Entity_key_name(L"Entity");
  static const OdString str_DB_key_name(L"Database");
  static const OdString str_NU_key_name(L"UNUSED_NODE");

  HierarchyTreeObjectType node_type = eEntity;

  if (0 == str_Model_key_name.compare(hierarchy_value))
  {
    node_type = eModel;
  }
  else
  {
    if (0 == str_BlockReference_key_name.compare(hierarchy_value))
    {
      node_type = eBlockReference;
    }
    else
    {
      if (0 == str_Block_key_name.compare(hierarchy_value))
      {
        node_type = eBlock;
      }
      else
      {
        if (0 == str_DB_key_name.compare(hierarchy_value))
        {
          node_type = eDatabase;
        }
        else
        {
          if (0 == str_Hidden_key_name.compare(hierarchy_value))
          {
            node_type = eHidden;
          }
          else
          {
            if (0 == str_NU_key_name.compare(hierarchy_value))
            {
              return;
            }
          }
        }
      }
    }
  }

  const bool use_dynamic_properties = !(class_dependent_property || (class_dependent_hierarchy && (!create_properties_cache && (0 == m_CollectedProperties.size()))));

  bool is_unic_found(false);
  OdUInt64 uinic_ID(0);

  OdRxPropertyPtr pHandleProp = OdRxMemberQueryEngine::theEngine()->find(pChildObj, L"ODAUniqueID", 0, use_dynamic_properties);
  if (!pHandleProp.isNull())
  {
    OdRxValue valHandle;
    if (eOk == pHandleProp->getValue(pChildObj, valHandle))
    {
      is_unic_found = true;
      uinic_ID = *rxvalue_cast<OdUInt64>(&valHandle);
    }
  }

  OdRxModelTreeBaseNodePtr pChild;
  if (is_unic_found)
  {
    std::map<OdUInt64, OdRxModelTreeBaseNodePtr>::const_iterator iter = cache[databaseID].find(uinic_ID);
    if (cache[databaseID].end() != iter)
      pChild = iter->second;
  }
  if (pChild.isNull())
  {
    pChild = createNodeObject();
    cache[databaseID][uinic_ID] = pChild;
    OdRxModelTreeBaseNodeImpl* pImpl = OdRxModelTreeBaseNodeImpl::getImpl(pChild);
    if (NULL != attributes_info.pNameAttribute)
      pImpl->setNodeName(*m_StringStorage.insert(attributes_info.pNameAttribute->getDisplayValue(prop_value, use_dynamic_properties)).first);

    pImpl->setUniqueDBID(databaseID);

    pImpl->setNodeType(node_type);

    if (pImpl->getNodeName().isEmpty())
    {
      if (hierarchy_value.isEmpty())
        pImpl->setNodeName(str_Entity_key_name);
      else
        pImpl->setNodeName(hierarchy_value);
    }

    if (is_unic_found)
    {
      pImpl->setUniqueSourceID(uinic_ID);
    }

    processNode(pChild, pChildObj, cache, create_properties_cache, class_dependent_hierarchy, class_dependent_property, 
        eDatabase == pImpl->getNodeType()? (OdUInt64)pChildObj.get() : databaseID);
  }
  pParent->addChild(pChild);
}

void OdRxModelHierarchyTreeBaseImpl::processRxProperty(OdRxModelTreeBaseNode* pParent, const OdRxObject* pObj, OdRxProperty* pProperty, PropertyInfo* info,
  const AttributesInfo& attributes_info, std::map < OdUInt64, std::map<OdUInt64, OdRxModelTreeBaseNodePtr> >& cache,
  const bool create_properties_cache, const bool class_dependent_hierarchy, const bool class_dependent_property, const OdUInt64 databaseID)
{
  try
  {
    OdRxValue prop_value;
    if (eOk == pProperty->getValue(pObj, prop_value))
    {
      if (create_properties_cache && !prop_value.isEmpty())
      {
        info->name = *m_StringStorage.insert(pProperty->name()).first;

        if (NULL == attributes_info.pLocalizedNameAttribute)
        {
          info->localizedName = *m_StringStorage.insert(pProperty->localName()).first;
        }
        else
        {
          info->localizedName = *m_StringStorage.insert(attributes_info.pLocalizedNameAttribute->getLocalizedName(pProperty)).first;
        }

        if (NULL == attributes_info.pUiPlacementAttribute)
        {
          info->weight = 0.;
        }
        else
        {
          info->weight = attributes_info.pUiPlacementAttribute->getWeight(pProperty);
          info->uiPlacement = *m_StringStorage.insert(attributes_info.pUiPlacementAttribute->getCategory(pProperty)).first;
        }
        if ((prop_value.type().isReference() || NULL != prop_value.type().rxObjectValue()) && (NULL != attributes_info.pNameAttribute))
        {
          const bool use_dynamic_properties = !(class_dependent_property || (class_dependent_hierarchy && (!create_properties_cache && (0 == m_CollectedProperties.size()))));
          info->value = attributes_info.pNameAttribute->getDisplayValue(prop_value, use_dynamic_properties);
        }
        else
        {
          std::map<const OdRxValueType*, process_function>::iterator it = SinglePropsFuncMap.find(&prop_value.type());
          if (SinglePropsFuncMap.end() == it)
            SinglePropertyFunctor::processDefaultValue(info->value, prop_value.toString());
          else
            it->second(info->value, prop_value);
        }
      }
      if (NULL != attributes_info.pHierarchyAttribute)
      {
        processElement(pParent, attributes_info, prop_value, cache, create_properties_cache, class_dependent_hierarchy, class_dependent_property, databaseID);
      }
    }
  }
  catch (...)
  {
  }
}

void OdRxModelHierarchyTreeBaseImpl::processRxCollection(OdRxModelTreeBaseNode* pParent, const OdRxObject* pObj, 
  const OdRxCollectionProperty* pPropertyCollection, PropertyInfo* info, 
  const AttributesInfo& attributes_info, std::map < OdUInt64, std::map<OdUInt64, OdRxModelTreeBaseNodePtr> >& cache,
  const bool create_properties_cache, const bool class_dependent_hierarchy, const bool class_dependent_property, const OdUInt64 databaseID)
{

  OdRxValueIteratorPtr pPrIt = pPropertyCollection->newValueIterator(pObj);
  if (!pPrIt.isNull())
  {
    if (pPrIt->done())
      return;
    OdRxValue initial_prop_value = pPrIt->current();

    if (create_properties_cache && NULL != info)
    {
      info->name = *m_StringStorage.insert(pPropertyCollection->name()).first;

      if (NULL == attributes_info.pLocalizedNameAttribute)
      {
        info->localizedName = *m_StringStorage.insert(pPropertyCollection->localName()).first;
      }
      else
      {
        info->localizedName = *m_StringStorage.insert(attributes_info.pLocalizedNameAttribute->getLocalizedName(pPropertyCollection)).first;
      }

      if (NULL == attributes_info.pUiPlacementAttribute)
      {
        info->weight = 0.;
      }
      else
      {
        info->weight = attributes_info.pUiPlacementAttribute->getWeight(pPropertyCollection);
        info->uiPlacement = *m_StringStorage.insert(attributes_info.pUiPlacementAttribute->getCategory(pPropertyCollection)).first;
      }

      if ((initial_prop_value.type().isReference() || NULL != initial_prop_value.type().rxObjectValue()) && (NULL != attributes_info.pNameAttribute))
      {
        CollectionInitPropertyFunctor::processDefaultValue(info->value, OdString());
      }
      else
      {
        std::map<const OdRxValueType*, process_function>::iterator it = CollectionInitPropsFuncMap.find(&initial_prop_value.type());
        if (CollectionInitPropsFuncMap.end() == it)
          CollectionInitPropertyFunctor::processDefaultValue(info->value, initial_prop_value.toString());
        else
          it->second(info->value, initial_prop_value);
      }
    }

    while (!pPrIt->done())
    {
      OdRxValue prop_value = pPrIt->current();

      if (create_properties_cache)
      {
        if ((prop_value.type().isReference() || NULL != prop_value.type().rxObjectValue()) && (NULL != attributes_info.pNameAttribute))
        {
          const bool use_dynamic_properties = !(class_dependent_property || (class_dependent_hierarchy && (!create_properties_cache && (0 == m_CollectedProperties.size()))));
          info->value.asStringArray().append(attributes_info.pNameAttribute->getDisplayValue(prop_value, use_dynamic_properties));
        }
        else
        {
          std::map<const OdRxValueType*, process_function>::iterator it = CollectionPropsFuncMap.find(&prop_value.type());
          if (CollectionPropsFuncMap.end() == it)
            CollectionPropertyFunctor::processDefaultValue(info->value, prop_value.toString());
          else
            it->second(info->value, prop_value);
        }
      }

      if (NULL != attributes_info.pHierarchyAttribute)
      {
        processElement(pParent, attributes_info, prop_value, cache, create_properties_cache, class_dependent_hierarchy, class_dependent_property, databaseID);
      }

      pPrIt->next();
    }
  }
}

void OdRxModelHierarchyTreeBaseImpl::collectAttributes(const OdRxMember* pMember, AttributesInfo& attributes_info)
{
  const OdRxAttributeCollection& attributes = pMember->attributes();
  for (int i = 0; i < attributes.count(); i++)
  {
    const OdRxAttribute* pAttribute = attributes.getAt(i);

    OdRxClass* pRx = pAttribute->isA();

    if (NULL == pRx)
      continue;
    if (!attributes_info.pNameAttribute && pRx->isDerivedFrom(OdRxDisplayAsAttribute::desc()))
    {
      attributes_info.pNameAttribute = (OdRxDisplayAsAttribute*)pAttribute;
    }
    else if (!attributes_info.pHierarchyAttribute && pRx->isDerivedFrom(OdRxHierarchyLevelAttribute::desc()))
    {
      attributes_info.pHierarchyAttribute = (OdRxHierarchyLevelAttribute*)pAttribute;
    }
    else if (!attributes_info.pUiPlacementAttribute && pRx->isDerivedFrom(OdRxUiPlacementAttribute::desc()))
    {
      attributes_info.pUiPlacementAttribute = (OdRxUiPlacementAttribute*)pAttribute;
    }
    else if (!attributes_info.pLocalizedNameAttribute && pRx->isDerivedFrom(OdRxLocalizedNameAttribute::desc()))
    {
      attributes_info.pLocalizedNameAttribute = (OdRxLocalizedNameAttribute*)pAttribute;
    }
  }
  
  const OdRxClass* pRxMember = pMember->isA();
  attributes_info.is_property = pRxMember->isDerivedFrom(OdRxProperty::desc());
  if (!attributes_info.is_property)
    attributes_info.is_collection = pRxMember->isDerivedFrom(OdRxCollectionProperty::desc());
}

OdRxModelHierarchyTreeBaseImpl::ClassAttributeInfo* OdRxModelHierarchyTreeBaseImpl::buildAttributeMap(const OdRxObject* pObj)
{
  ClassAttributeInfo& ci = m_AttributeMap.insert(std::make_pair(pObj->isA(), ClassAttributeInfo())).first->second;
  for (OdRxMemberIteratorPtr it = OdRxMemberQueryEngine::theEngine()->newMemberIterator(pObj); !it->done(); it->next())
  {
    OdRxMember* pMember = it->current();
    if (!pMember || !pMember->isA())
      continue;
    AttributesInfo& attributes_info = ci.insert(std::make_pair(pMember, AttributesInfo())).first->second;
    collectAttributes(pMember, attributes_info);
  }
  return &ci;
}

void OdRxModelHierarchyTreeBaseImpl::processNode(OdRxModelTreeBaseNode* pParent, const OdRxObject* pObj, std::map < OdUInt64, std::map<OdUInt64,
  OdRxModelTreeBaseNodePtr> >& cache, const bool create_properties_cache, const bool class_dependent_hierarchy, const bool class_dependent_property, const OdUInt64 databaseID)
{
  const bool static_hierarchy = class_dependent_hierarchy && (!create_properties_cache && (0 == m_CollectedProperties.size()));

  OdArray<PropertyInfo>& props = OdRxModelTreeBaseNodeImpl::getImpl(pParent)->m_Properties;

  if (class_dependent_property || static_hierarchy)
  {
    AttributeMap::iterator ai = m_AttributeMap.find(pObj->isA());
    ClassAttributeInfo* cai = (ai == m_AttributeMap.end()) ? buildAttributeMap(pObj) : &ai->second;
    for (ClassAttributeInfo::const_iterator ci = cai->begin(); ci != cai->end(); ++ci)
    {
      const bool cache_property = create_properties_cache ? create_properties_cache : (m_CollectedProperties.end() != m_CollectedProperties.find(ci->first->name()));
      if (cache_property)
      {
        if (0 == props.size() || !props.last().name.isEmpty())
          props.resize(props.size() + 1);

        processPropertyInternal(pParent, pObj, ci->first, &props.last(), cache, cache_property, ci->second, class_dependent_hierarchy, class_dependent_property, databaseID);
      }
      else
        if (ci->second.pHierarchyAttribute)
        {
          if (ci->second.is_collection)
            processRxCollection(pParent, pObj, (OdRxCollectionProperty*)ci->first, 0, ci->second, cache, false, class_dependent_hierarchy, class_dependent_property, databaseID);
          else if (ci->second.is_property)
            processRxProperty(pParent, pObj, (OdRxProperty*)ci->first, 0, ci->second, cache, false, class_dependent_hierarchy, class_dependent_property, databaseID);
        }
    }
  }
  else
  {
    OdRxMemberIteratorPtr it = OdRxMemberQueryEngine::theEngine()->newMemberIterator(pObj);
    OdUInt64 props_size = it->size();
    props.reserve(create_properties_cache ? props_size : m_CollectedProperties.size());
    for (; !it->done(); it->next())
    {
      const OdRxMember* pRxMember = it->current();
      if (NULL != pRxMember && NULL != pRxMember->isA())
      {
        const bool cache_property = create_properties_cache ? create_properties_cache : (m_CollectedProperties.end() != m_CollectedProperties.find(pRxMember->name()));

        if (cache_property && (0 == props.size() || !props.last().name.isEmpty()))
          props.resize(props.size() + 1);

        processProperty(pParent, pObj, pRxMember, cache_property?&props.last():0, cache, cache_property, databaseID);

      }
    }
  }
  if (0 != props.size() && props.last().name.isEmpty())
    props.removeLast();
}

void OdRxModelHierarchyTreeBaseImpl::processProperty(OdRxModelTreeBaseNode* pParent, const OdRxObject* pObj, const OdRxMember* pRxMember, PropertyInfo* info,
  std::map < OdUInt64, std::map<OdUInt64, OdRxModelTreeBaseNodePtr> >& cache, const bool create_properties_cache, const OdUInt64 databaseID)
{
  AttributesInfo attributes_info;
  collectAttributes(pRxMember, attributes_info);

  if (!create_properties_cache && NULL == attributes_info.pHierarchyAttribute)
    return;

  processPropertyInternal(pParent, pObj, pRxMember, info, cache, create_properties_cache, attributes_info, false, false, databaseID);
}

void OdRxModelHierarchyTreeBaseImpl::processPropertyInternal(OdRxModelTreeBaseNode* pParent, const OdRxObject* pObj, const OdRxMember* pRxMember, PropertyInfo* info, 
  std::map < OdUInt64, std::map<OdUInt64, OdRxModelTreeBaseNodePtr> >& cache, const bool create_properties_cache, const AttributesInfo& attributes_info,
  const bool class_dependent_hierarchy, const bool class_dependent_property, const OdUInt64 databaseID)
{
  if (attributes_info.is_property)
  {
    processRxProperty(pParent, pObj, (OdRxProperty*)pRxMember, info, attributes_info, cache, create_properties_cache, class_dependent_hierarchy, class_dependent_property, databaseID);
  }
  if (attributes_info.is_collection)
  {
    processRxCollection(pParent, pObj, (OdRxCollectionProperty*)pRxMember, info, attributes_info, cache, create_properties_cache, class_dependent_hierarchy, class_dependent_property, databaseID);
  }
  if (NULL != info && (attributes_info.is_property || attributes_info.is_collection))
  {
    if (const OdArray<OdRxMemberPtr>* pChildern = pRxMember->children())
    {
      const OdArray<OdRxMemberPtr>::size_type length = pChildern->length();
      if (0 != length && NULL != pChildern->getAt(0).get() && !pChildern->getAt(0).get()->isA()->isDerivedFrom(OdRxSubPropertyOfIndexedProperty::desc())) // DGN specific
      {
        info->subProperties.resize(length);
        for (OdArray<OdRxMemberPtr>::size_type i = 0; i < length; ++i)
        {
          processProperty(pParent, pObj, pChildern->getAt(i).get(), &info->subProperties[i], cache, create_properties_cache, databaseID);
        }
        while (0 != info->subProperties.size() && info->subProperties.last().name.isEmpty())
          info->subProperties.removeLast();
      }
    }
  }
}

void OdRxModelHierarchyTreeBaseImpl::serialize(OdBaseHierarchyTreeFiler* pFiler) const
{
  OdArray<NodeCache> cache;
  std::map<OdRxModelTreeBaseNode*, OdUInt64> id_cahe;
  createNodeCache(m_DatabaseNode, cache, id_cahe);
  
  pFiler->wrUInt8((OdUInt8)eCURRENT);

  pFiler->wrInt64(cache.size());
  for (OdArray<NodeCache>::const_iterator elem_it = cache.begin(); elem_it != cache.end(); ++elem_it)
  {
    elem_it->m_pNode->outFields(pFiler);
    pFiler->wrInt64(elem_it->m_IDs.size());
    for (OdArray<OdUInt64>::const_iterator ids_it = elem_it->m_IDs.begin(); ids_it != elem_it->m_IDs.end(); ++ids_it)
    {
      pFiler->wrInt64(*ids_it);
    }
  }
}

OdResult OdRxModelHierarchyTreeBaseImpl::deserialize(OdBaseHierarchyTreeFiler* pFiler)
{
  m_NodeCache.clear();

  HierarchyTreeVersion version = (HierarchyTreeVersion)pFiler->rdUInt8();

  OdUInt64 node_count = pFiler->rdInt64();
  OdArray<OdRxModelTreeBaseNodePtr> cache(node_count);
  cache.resize(node_count);
  for (OdUInt64 node_num = 0; node_num < node_count; ++node_num)
  {
    cache[node_num] = createNodeObject();
    cache[node_num]->inFields(pFiler, m_StringStorage);
    OdUInt64 children_count = pFiler->rdInt64();
    for (OdUInt64 ch_num = 0; ch_num < children_count; ++ch_num)
    {
      OdUInt64 ch_index = pFiler->rdInt64();
      cache[node_num]->addChild(cache[ch_index]);
    }
    m_NodeCache[cache[node_num]->getUniqueDBID()][cache[node_num]->getUniqueSourceID()] = cache[node_num];
  }
  m_DatabaseNode = cache[node_count - 1];
  return eOk;
}

OdUInt64 OdRxModelHierarchyTreeBaseImpl::createNodeCache(const OdRxModelTreeBaseNodePtr& pNode, OdArray<NodeCache>& cache, std::map<OdRxModelTreeBaseNode*, OdUInt64>& id_cahe) const
{
  NodeCache elem;
  const OdRxModelTreeBaseNodePtrArray& children = pNode->getChildren();
  const OdRxModelTreeBaseNodePtrArray::size_type children_count = children.size();
  for (OdRxModelTreeBaseNodePtrArray::size_type i = 0; i < children_count; ++i)
  {
    OdRxModelTreeBaseNode* pChild = const_cast<OdRxModelTreeBaseNode*>(children[i].get());
    std::map<OdRxModelTreeBaseNode*, OdUInt64>::const_iterator it = id_cahe.find(pChild);
    if (it != id_cahe.end())
      elem.m_IDs.append(it->second);
    else
      elem.m_IDs.append(createNodeCache(children[i], cache, id_cahe));
  }
  elem.m_pNode = pNode;
  OdUInt64 ret = cache.append(elem);
  id_cahe.insert(std::make_pair(elem.m_pNode.get(), ret));
  return ret;
}

const OdArray<PropertyInfo>& OdRxModelHierarchyTreeBaseImpl::getNodeProperties(OdUInt64 unique_DB_ID, OdUInt64 unique_source_ID)
{
  std::map < OdUInt64, std::map<OdUInt64, OdRxModelTreeBaseNodePtr> >::const_iterator db_it = m_NodeCache.find(unique_DB_ID);
  if (m_NodeCache.end() == db_it)
  {
    static OdArray<PropertyInfo> ret;
    return ret;
  }
  std::map<OdUInt64, OdRxModelTreeBaseNodePtr>::const_iterator nodes_it = db_it->second.find(unique_source_ID);
  if (db_it->second.end() == nodes_it)
  {
    static OdArray<PropertyInfo> ret;
    return ret;
  }
  return nodes_it->second->getProperties();
}




