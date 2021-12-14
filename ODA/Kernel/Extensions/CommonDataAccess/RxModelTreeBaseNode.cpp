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
#include "RxModelTreeBaseNode.h"
#include "RxValue.h"
#include "RxValueTypeUtil.h"
#include "RxMember.h"
#include "RxObjectImpl.h"
#include "RxAttribute.h"
#include "RxSysRegistry.h"
#include "RxModelTreeBaseNodeImpl.h"
#include "RxProperty.h"


ODRX_CONS_DEFINE_MEMBERS(OdRxModelTreeBaseNode, OdRxObject, RXIMPL_CONSTR)

#define _GET_IMPL_ \
  \
  OdRxModelTreeBaseNodeImpl* pImpl = OdRxModelTreeBaseNodeImpl::getImpl(this); \
  if (NULL == pImpl)\
  { \
    throw OdError(::eNullObjectPointer); \
  } 

OdRxModelTreeBaseNode::OdRxModelTreeBaseNode()
  :m_pImpl(new OdRxModelTreeBaseNodeImpl)
{

}

OdRxModelTreeBaseNode::OdRxModelTreeBaseNode(OdRxModelTreeBaseNodeImpl* impl)
  :m_pImpl(impl)
{

}

OdRxModelTreeBaseNode::~OdRxModelTreeBaseNode()
{
  delete m_pImpl;
}

const OdString& OdRxModelTreeBaseNode::getNodeName() const
{
  _GET_IMPL_
  return  pImpl->getNodeName();
}

OdUInt64 OdRxModelTreeBaseNode::getUniqueSourceID() const
{
  _GET_IMPL_
  return  pImpl->getUniqueSourceID();
}


OdUInt64 OdRxModelTreeBaseNode::getUniqueDBID() const
{
  _GET_IMPL_
  return  pImpl->getUniqueDBID();
}

HierarchyTreeObjectType OdRxModelTreeBaseNode::getNodeType() const
{
  _GET_IMPL_
  return  pImpl->getNodeType();
}

void OdRxModelTreeBaseNodeImpl::setNodeName(const OdString& name)
{
  m_Name = name;
}

void OdRxModelTreeBaseNodeImpl::setUniqueSourceID(const OdUInt64 id)
{
  m_UniqueSourceID = id;
}

void OdRxModelTreeBaseNodeImpl::setUniqueDBID(const OdUInt64 id)
{
  m_UniqueDBID = id;
}

void OdRxModelTreeBaseNodeImpl::setNodeType(const HierarchyTreeObjectType type)
{
  m_NodeType = type;
}

const OdRxModelTreeBaseNodeRawPtrArray& OdRxModelTreeBaseNode::getParents() const
{
  _GET_IMPL_
  return  pImpl->getParents();
}

const OdRxModelTreeBaseNodePtrArray& OdRxModelTreeBaseNode::getChildren() const
{
  _GET_IMPL_
  return  pImpl->getChildren();
}


const OdArray<PropertyInfo>& OdRxModelTreeBaseNode::getProperties() const
{
  _GET_IMPL_
  return  pImpl->getProperties();
}

bool OdRxModelTreeBaseNode::addChild(OdRxModelTreeBaseNodePtr& elem)
{
  _GET_IMPL_
  return pImpl->addChild(this, elem);
}

void OdRxModelTreeBaseNode::outFields(OdBaseHierarchyTreeFiler* pFiler) const
{
  _GET_IMPL_
  return pImpl->outFields(pFiler);
}

OdResult OdRxModelTreeBaseNode::inFields(OdBaseHierarchyTreeFiler* pFiler, std::set<OdString>& string_storage)
{
  _GET_IMPL_
    return pImpl->inFields(pFiler, string_storage);
}

OdRxModelTreeBaseNodeImpl::OdRxModelTreeBaseNodeImpl()
  : m_UniqueSourceID(0)
  , m_NodeType(eDatabase)
  , m_UniqueDBID(0)
{

}

OdRxModelTreeBaseNodeImpl::~OdRxModelTreeBaseNodeImpl()
{

}

const OdString& OdRxModelTreeBaseNodeImpl::getNodeName() const
{
  return m_Name;
}

OdUInt64 OdRxModelTreeBaseNodeImpl::getUniqueSourceID() const
{
  return m_UniqueSourceID;
}

OdUInt64 OdRxModelTreeBaseNodeImpl::getUniqueDBID() const
{
  return m_UniqueDBID;
}

HierarchyTreeObjectType OdRxModelTreeBaseNodeImpl::getNodeType() const
{
  return m_NodeType;
}

const OdRxModelTreeBaseNodeRawPtrArray& OdRxModelTreeBaseNodeImpl::getParents() const
{
  return m_Parents;
}

const OdRxModelTreeBaseNodePtrArray& OdRxModelTreeBaseNodeImpl::getChildren() const
{
  return m_Children;
}

bool findParent(const OdRxModelTreeBaseNode* current, const OdRxModelTreeBaseNodeRawPtrArray& parents)
{
  for (OdRxModelTreeBaseNodeRawPtrArray::const_iterator it = parents.begin(); it != parents.end(); ++it)
  {
    if (current == *it)
      return true;
  }
  return false;
}

bool OdRxModelTreeBaseNodeImpl::addChild(OdRxModelTreeBaseNode* parent, OdRxModelTreeBaseNodePtr& child)
{
  OdRxModelTreeBaseNodeImpl* pImpl = OdRxModelTreeBaseNodeImpl::getImpl(child);
  if(parent == child.get())
    return false;
  if (findParent(parent, pImpl->m_Parents))
    return false;
  m_Children.append(child);
  pImpl->m_Parents.append(parent);
  return true;
}

#undef TYPE_CONTAINER
#define TYPE_CONTAINER \
  VARTYPE(AnsiString )\
  VARTYPE(String )\
  VARTYPE(Bool   )\
  VARTYPE(Int8   )\
  VARTYPE(Int16  )\
  VARTYPE(Int32  )\
  VARTYPE(Int64  )\
  VARTYPE(Double )\
  VARTYPE(GePoint2d      )\
  VARTYPE(GePoint3d      )\
  VARTYPE(GeVector2d     )\
  VARTYPE(GeVector3d     )\
  VARTYPE(CmEntityColor  )\
  VARTYPE(CmTransparency )\
  VARTYPEINT(GiMrExposureType)\
  VARTYPEINT(GiMrShadowMode)\
  VARTYPEINT(GiMrFinalGatheringMode)\
  VARTYPEINT(GiMrDiagnosticMode)\
  VARTYPEINT(GiMrDiagnosticPhotonMode)\
  VARTYPEINT(GiMrDiagnosticBSPMode)\
  VARTYPEINT(GiMrExportMIMode)\
  VARTYPEINT(GiMrTileOrder)\
  VARTYPEINT(GiLightingMode)\
  VARTYPEINT(GiFilterType)\
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
  VARTYPEENUM(GiToneOperatorParameters, ExteriorDaylightMode)\
  PTRTYPE(GeMatrix2d)\
  PTRTYPE(GeMatrix3d)\
  PTRTYPE(TimeStamp)\
  PTRTYPE(GeExtents2d)\
  PTRTYPE(GeExtents3d)\
  PTRTYPE(GeQuaternion)\
  PTRTYPE(GiLightAttenuation)\
  PTRTYPE(GiSkyParameters)\
  PTRTYPE(GiShadowParameters)\
  PTRTYPE(GiColorRGB)\
  PTRTYPE(GiMaterialColor)


void wrGeMatrix2d(OdBaseHierarchyTreeFiler* pFiler, const OdGeMatrix2d& matrix)
{
  pFiler->wrDouble(matrix[0][0]);
  pFiler->wrDouble(matrix[0][1]);
  pFiler->wrDouble(matrix[0][2]);
  pFiler->wrDouble(matrix[1][0]);
  pFiler->wrDouble(matrix[1][1]);
  pFiler->wrDouble(matrix[1][2]);
  pFiler->wrDouble(matrix[2][0]);
  pFiler->wrDouble(matrix[2][1]);
  pFiler->wrDouble(matrix[2][2]);
}

OdGeMatrix2d rdGeMatrix2d(OdBaseHierarchyTreeFiler* pFiler)
{
  OdGeMatrix2d matrix;
  matrix[0][0] = pFiler->rdDouble();
  matrix[0][1] = pFiler->rdDouble();
  matrix[0][2] = pFiler->rdDouble();
  matrix[1][0] = pFiler->rdDouble();
  matrix[1][1] = pFiler->rdDouble();
  matrix[1][2] = pFiler->rdDouble();
  matrix[2][0] = pFiler->rdDouble();
  matrix[2][1] = pFiler->rdDouble();
  matrix[2][2] = pFiler->rdDouble();
  return matrix;
}

void wrGeMatrix3d(OdBaseHierarchyTreeFiler* pFiler, const OdGeMatrix3d& matrix)
{
  pFiler->wrDouble(matrix[0][0]);
  pFiler->wrDouble(matrix[0][1]);
  pFiler->wrDouble(matrix[0][2]);
  pFiler->wrDouble(matrix[0][3]);
  pFiler->wrDouble(matrix[1][0]);
  pFiler->wrDouble(matrix[1][1]);
  pFiler->wrDouble(matrix[1][2]);
  pFiler->wrDouble(matrix[1][3]);
  pFiler->wrDouble(matrix[2][0]);
  pFiler->wrDouble(matrix[2][1]);
  pFiler->wrDouble(matrix[2][2]);
  pFiler->wrDouble(matrix[2][3]);
  pFiler->wrDouble(matrix[3][0]);
  pFiler->wrDouble(matrix[3][1]);
  pFiler->wrDouble(matrix[3][2]);
  pFiler->wrDouble(matrix[3][3]);
}

OdGeMatrix3d rdGeMatrix3d(OdBaseHierarchyTreeFiler* pFiler)
{
  OdGeMatrix3d matrix;
  matrix[0][0] = pFiler->rdDouble();
  matrix[0][1] = pFiler->rdDouble();
  matrix[0][2] = pFiler->rdDouble();
  matrix[0][3] = pFiler->rdDouble();
  matrix[1][0] = pFiler->rdDouble();
  matrix[1][1] = pFiler->rdDouble();
  matrix[1][2] = pFiler->rdDouble();
  matrix[1][3] = pFiler->rdDouble();
  matrix[2][0] = pFiler->rdDouble();
  matrix[2][1] = pFiler->rdDouble();
  matrix[2][2] = pFiler->rdDouble();
  matrix[2][3] = pFiler->rdDouble();
  matrix[3][0] = pFiler->rdDouble();
  matrix[3][1] = pFiler->rdDouble();
  matrix[3][2] = pFiler->rdDouble();
  matrix[3][3] = pFiler->rdDouble();
  return matrix;
}

void wrTimeStamp(OdBaseHierarchyTreeFiler* pFiler, const OdTimeStamp& timestamp)
{
  pFiler->wrInt64(timestamp.packedValue());
}

OdTimeStamp rdTimeStamp(OdBaseHierarchyTreeFiler* pFiler)
{
  OdTimeStamp timestamp;
  timestamp.setPackedValue(pFiler->rdInt64());
  return timestamp;
}

void wrGeExtents2d(OdBaseHierarchyTreeFiler* pFiler, const OdGeExtents2d& extents)
{
  pFiler->wrGePoint2d(extents.minPoint());
  pFiler->wrGePoint2d(extents.maxPoint());
}

OdGeExtents2d rdGeExtents2d(OdBaseHierarchyTreeFiler* pFiler)
{
  OdGePoint2d min_point = pFiler->rdGePoint2d();
  OdGePoint2d max_point = pFiler->rdGePoint2d();
  OdGeExtents2d extents(min_point, max_point);
  return extents;
}

void wrGeExtents3d(OdBaseHierarchyTreeFiler* pFiler, const OdGeExtents3d& extents)
{
  pFiler->wrGePoint3d(extents.minPoint());
  pFiler->wrGePoint3d(extents.maxPoint());
}

OdGeExtents3d rdGeExtents3d(OdBaseHierarchyTreeFiler* pFiler)
{
  OdGePoint3d min_point = pFiler->rdGePoint3d();
  OdGePoint3d max_point = pFiler->rdGePoint3d();
  OdGeExtents3d extents(min_point, max_point);
  return extents;
}

void wrGeQuaternion(OdBaseHierarchyTreeFiler* pFiler, const OdGeQuaternion& quaternion)
{
  pFiler->wrDouble(quaternion.w);
  pFiler->wrDouble(quaternion.x);
  pFiler->wrDouble(quaternion.y);
  pFiler->wrDouble(quaternion.z);
}

OdGeQuaternion rdGeQuaternion(OdBaseHierarchyTreeFiler* pFiler)
{
  double w_val = pFiler->rdDouble();
  double x_val = pFiler->rdDouble();
  double y_val = pFiler->rdDouble();
  double z_val = pFiler->rdDouble();
  OdGeQuaternion quaternion(w_val, x_val, y_val, z_val);
  return quaternion;
}

void wrGiLightAttenuation(OdBaseHierarchyTreeFiler* pFiler, const OdGiLightAttenuation& value)
{
  pFiler->wrUInt32(value.attenuationType());
  pFiler->wrBool(value.useLimits());
  pFiler->wrDouble(value.startLimit());
  pFiler->wrDouble(value.endLimit());
}

OdGiLightAttenuation rdGiLightAttenuation(OdBaseHierarchyTreeFiler* pFiler)
{
  OdGiLightAttenuation value;
  value.setAttenuationType((OdGiLightAttenuation::AttenuationType)pFiler->rdUInt32());
  value.setUseLimits(pFiler->rdBool());
  value.setLimits(pFiler->rdDouble(), pFiler->rdDouble());
  return value;
}

void wrGiSkyParameters(OdBaseHierarchyTreeFiler* pFiler, const OdGiSkyParameters& value)
{
  pFiler->wrBool(value.illumination());
  pFiler->wrDouble(value.intensityFactor());
  pFiler->wrDouble(value.haze());
  pFiler->wrDouble(value.horizonHeight());
  pFiler->wrDouble(value.horizonBlur());
  pFiler->wrCmEntityColor(value.groundColor());
  pFiler->wrCmEntityColor(value.nightColor());
  pFiler->wrBool(value.aerialPerspective());
  pFiler->wrDouble(value.visibilityDistance());
  pFiler->wrDouble(value.diskScale());
  pFiler->wrDouble(value.glowIntensity());
  pFiler->wrDouble(value.diskIntensity());
  pFiler->wrUInt16(value.solarDiskSamples());
  pFiler->wrGeVector3d(value.sunDirection());
  pFiler->wrDouble(value.redBlueShift());
  pFiler->wrDouble(value.saturation());
}

OdGiSkyParameters rdGiSkyParameters(OdBaseHierarchyTreeFiler* pFiler)
{
  OdGiSkyParameters value;
  pFiler->wrBool(value.illumination());
  pFiler->wrDouble(value.intensityFactor());
  pFiler->wrDouble(value.haze());
  pFiler->wrDouble(value.horizonHeight());
  pFiler->wrDouble(value.horizonBlur());
  pFiler->wrCmEntityColor(value.groundColor());
  pFiler->wrCmEntityColor(value.nightColor());
  pFiler->wrBool(value.aerialPerspective());
  pFiler->wrDouble(value.visibilityDistance());
  pFiler->wrDouble(value.diskScale());
  pFiler->wrDouble(value.glowIntensity());
  pFiler->wrDouble(value.diskIntensity());
  pFiler->wrUInt16(value.solarDiskSamples());
  pFiler->wrGeVector3d(value.sunDirection());
  pFiler->wrDouble(value.redBlueShift());
  pFiler->wrDouble(value.saturation());
  return value;
}

void wrGiShadowParameters(OdBaseHierarchyTreeFiler* pFiler, const OdGiShadowParameters& value)
{
  pFiler->wrBool(value.shadowsOn());
  pFiler->wrUInt8((OdUInt8)value.shadowType());
  pFiler->wrUInt16(value.shadowMapSize());
  pFiler->wrUInt8(value.shadowMapSoftness());
  pFiler->wrBool(value.shapeVisibility());
  pFiler->wrUInt8((OdUInt8)value.extendedLightShape());
  pFiler->wrDouble(value.extendedLightLength());
  pFiler->wrDouble(value.extendedLightWidth());
  pFiler->wrDouble(value.extendedLightRadius());
}

OdGiShadowParameters rdGiShadowParameters(OdBaseHierarchyTreeFiler* pFiler)
{
  OdGiShadowParameters value;
  value.setShadowsOn(pFiler->rdBool());
  value.setShadowType((OdGiShadowParameters::ShadowType)pFiler->rdUInt8());
  value.setShadowMapSize(pFiler->rdUInt16());
  value.setShadowMapSoftness(pFiler->rdUInt8());
  value.setShapeVisibility(pFiler->rdBool());
  value.setExtendedLightShape((OdGiShadowParameters::ExtendedLightShape)pFiler->rdUInt8());
  value.setExtendedLightLength(pFiler->rdDouble());
  value.setExtendedLightWidth(pFiler->rdDouble());
  value.setExtendedLightRadius(pFiler->rdDouble());
  return value;
}

void wrGiColorRGB(OdBaseHierarchyTreeFiler* pFiler, const OdGiColorRGB& value)
{
  pFiler->wrDouble(value.red);
  pFiler->wrDouble(value.green);
  pFiler->wrDouble(value.blue);
}

OdGiColorRGB rdGiColorRGB(OdBaseHierarchyTreeFiler* pFiler)
{
  OdGiColorRGB value;
  value.red = pFiler->rdDouble();
  value.green = pFiler->rdDouble();
  value.blue = pFiler->rdDouble();
  return value;
}

void wrGiMaterialColor(OdBaseHierarchyTreeFiler* pFiler, const OdGiMaterialColor& value)
{
  pFiler->wrUInt8(value.method());
  pFiler->wrDouble(value.factor());
  pFiler->wrCmEntityColor(value.color());
}

OdGiMaterialColor rdGiMaterialColor(OdBaseHierarchyTreeFiler* pFiler)
{
  OdGiMaterialColor value;
  value.setMethod((OdGiMaterialColor::Method)pFiler->rdUInt8());
  value.setFactor(pFiler->rdDouble());
  value.setColor(pFiler->rdCmEntityColor());
  return value;
}

#define ODTYPE(Type) Od##Type

void OdRxModelTreeBaseNodeImpl::serializeProprties(OdBaseHierarchyTreeFiler* pFiler, const PropertyInfo& info) const
{
  pFiler->wrString(info.name);
  pFiler->wrString(info.localizedName);
  pFiler->wrString(info.uiPlacement);
  pFiler->wrDouble(info.weight);

  pFiler->wrInt64(info.value.varType());

  switch (info.value.varType())
  {
#undef VARTYPE
#define VARTYPE(Type) \
  case OdRxPropertyVariant::k##Type:\
    pFiler->wr##Type(info.value.get##Type());\
    break;

#undef VARTYPEINT
#define VARTYPEINT(Type) \
  case OdRxPropertyVariant::k##Type:\
    pFiler->wrUInt8(info.value.get##Type());\
    break;

#undef VARTYPEENUM
#define VARTYPEENUM(NameSp, Type) \
  case OdRxPropertyVariant::k##NameSp##_##Type:\
    pFiler->wrUInt8(info.value.get##NameSp##_##Type());\
    break;

#undef PTRTYPE
#define PTRTYPE(Type) \
  case OdRxPropertyVariant::k##Type:\
    wr##Type(pFiler, info.value.get##Type());\
    break;

    TYPE_CONTAINER

#undef VARTYPE
#define VARTYPE(Type) \
  case OdRxPropertyVariant::k##Type | OdRxPropertyVariant::kArray:\
    {\
    const Od##Type##Array& array = info.value.get##Type##Array();\
    const OdInt64 array_size = array.size();\
    pFiler->wrInt64(array_size);\
    for (OdInt64 i = 0; i < array_size; ++i)\
    {\
      pFiler->wr##Type(array[i]);\
    }\
    }\
    break;

#undef VARTYPEINT
#define VARTYPEINT(Type) \
  case OdRxPropertyVariant::k##Type | OdRxPropertyVariant::kArray:\
    {\
    const Od##Type##Array& array = info.value.get##Type##Array();\
    const OdInt64 array_size = array.size();\
    pFiler->wrInt64(array_size);\
    for (OdInt64 i = 0; i < array_size; ++i)\
    {\
      pFiler->wrUInt8(array[i]);\
    }\
    }\
    break;

#undef VARTYPEENUM
#define VARTYPEENUM(NameSp, Type) \
  case OdRxPropertyVariant::k##NameSp##_##Type | OdRxPropertyVariant::kArray:\
    {\
    const Od##NameSp##_##Type##Array& array = info.value.get##NameSp##_##Type##Array();\
    const OdInt64 array_size = array.size();\
    pFiler->wrInt64(array_size);\
    for (OdInt64 i = 0; i < array_size; ++i)\
    {\
      pFiler->wrUInt8(array[i]);\
    }\
    }\
    break;

#undef PTRTYPE
#define PTRTYPE(Type) \
  case OdRxPropertyVariant::k##Type | OdRxPropertyVariant::kArray:\
    {\
    const Od##Type##Array& array = info.value.get##Type##Array();\
    const OdInt64 array_size = array.size();\
    pFiler->wrInt64(array_size);\
    for (OdInt64 i = 0; i < array_size; ++i)\
    {\
      wr##Type(pFiler, array[i]);\
    }\
    }\
    break;

      TYPE_CONTAINER

  }

  const OdUInt64 prop_count = info.subProperties.size();
  pFiler->wrUInt64(prop_count);
  for (OdUInt64 i = 0; i < prop_count; ++i)
  {
    serializeProprties(pFiler, info.subProperties[i]);
  }
}

void OdRxModelTreeBaseNodeImpl::outFields(OdBaseHierarchyTreeFiler* pFiler) const
{
  pFiler->wrUInt8((OdUInt8)eCURRENT);

  pFiler->wrUInt8((OdUInt8)getNodeType());
  pFiler->wrInt64(getUniqueSourceID());
  pFiler->wrString(getNodeName());
  pFiler->wrInt64(getUniqueDBID());

  const OdUInt64 prop_count = m_Properties.size();
  pFiler->wrUInt64(prop_count);
  for (OdUInt64 i = 0; i < prop_count; ++i)
  {
    serializeProprties(pFiler, m_Properties[i]);
  }
}

void OdRxModelTreeBaseNodeImpl::deserializeProprties(OdBaseHierarchyTreeFiler* pFiler, PropertyInfo& info, std::set<OdString>& string_storage)
{
  info.name = *string_storage.insert(pFiler->rdString()).first;
  info.localizedName = *string_storage.insert(pFiler->rdString()).first;
  info.uiPlacement = *string_storage.insert(pFiler->rdString()).first;
  info.weight = pFiler->rdDouble();

  OdUInt64 vale_type = pFiler->rdInt64();

  switch (vale_type)
  {
#undef VARTYPE
#define VARTYPE(Type) \
  case OdRxPropertyVariant::k##Type:\
    info.value = pFiler->rd##Type();\
    break;

#undef VARTYPEINT
#define VARTYPEINT(Type) \
  case OdRxPropertyVariant::k##Type:\
    info.value.set##Type((ODTYPE(Type))pFiler->rdUInt8());\
    break;

#undef VARTYPEENUM
#define VARTYPEENUM(NameSp, Type) \
  case OdRxPropertyVariant::k##NameSp##_##Type:\
    info.value.set##NameSp##_##Type((Od##NameSp::Type)pFiler->rdUInt8());\
    break;

#undef PTRTYPE
#define PTRTYPE(Type) \
  case OdRxPropertyVariant::k##Type:\
    info.value = rd##Type(pFiler);\
    break;

    TYPE_CONTAINER

#undef VARTYPE
#define VARTYPE(Type) \
  case OdRxPropertyVariant::k##Type | OdRxPropertyVariant::kArray:\
    {\
    const OdInt64 array_size = pFiler->rdInt64();\
    Od##Type##Array& array = info.value.as##Type##Array();\
    array.resize(array_size);\
    for (OdInt64 i = 0; i < array_size; ++i)\
    {\
      array[i] = pFiler->rd##Type();\
    }\
    }\
    break;

#undef VARTYPEINT
#define VARTYPEINT(Type) \
  case OdRxPropertyVariant::k##Type | OdRxPropertyVariant::kArray:\
    {\
    const OdInt64 array_size = pFiler->rdInt64();\
    Od##Type##Array& array = info.value.as##Type##Array();\
    array.resize(array_size);\
    for (OdInt64 i = 0; i < array_size; ++i)\
    {\
      array[i] = (ODTYPE(Type))pFiler->rdUInt8();\
    }\
    }\
    break;

#undef VARTYPEENUM
#define VARTYPEENUM(NameSp, Type) \
  case OdRxPropertyVariant::k##NameSp##_##Type | OdRxPropertyVariant::kArray:\
    {\
    const OdInt64 array_size = pFiler->rdInt64();\
    Od##NameSp##_##Type##Array& array = info.value.as##NameSp##_##Type##Array();\
    array.resize(array_size);\
    for (OdInt64 i = 0; i < array_size; ++i)\
    {\
      array[i] = (Od##NameSp::Type)pFiler->rdUInt8();\
    }\
    }\
    break;

#undef PTRTYPE
#define PTRTYPE(Type) \
  case OdRxPropertyVariant::k##Type | OdRxPropertyVariant::kArray:\
    {\
    const OdInt64 array_size = pFiler->rdInt64();\
    Od##Type##Array& array = info.value.as##Type##Array();\
    array.resize(array_size);\
    for (OdInt64 i = 0; i < array_size; ++i)\
    {\
      array[i] = rd##Type(pFiler);\
    }\
    }\
    break;

      TYPE_CONTAINER
  }

  OdUInt64 prop_count = pFiler->rdUInt64();
  info.subProperties.resize(prop_count);
  for (OdUInt64 i = 0; i < prop_count; ++i)
  {
    deserializeProprties(pFiler, info.subProperties[i], string_storage);
  }
}

OdResult OdRxModelTreeBaseNodeImpl::inFields(OdBaseHierarchyTreeFiler* pFiler, std::set<OdString>& string_storage)
{
  HierarchyTreeVersion version = (HierarchyTreeVersion)pFiler->rdUInt8();

  m_NodeType = (HierarchyTreeObjectType)pFiler->rdUInt8();
  m_UniqueSourceID = pFiler->rdInt64();
  m_Name = pFiler->rdString();
  if (version > eHT2)
    m_UniqueDBID = pFiler->rdInt64();

  if (version > eHT1)
  {
    OdUInt64 prop_count = pFiler->rdUInt64();
    m_Properties.resize(prop_count);
    for (OdUInt64 i = 0; i < prop_count; ++i)
    {
      deserializeProprties(pFiler, m_Properties[i], string_storage);
    }
  }

  return eOk;
}

const OdArray<PropertyInfo>& OdRxModelTreeBaseNodeImpl::getProperties() const
{
  return m_Properties;
}


void OdRxModelTreeBaseNodeImpl::addProperty(PropertyInfo& info)
{
  m_Properties.append(info);
}



