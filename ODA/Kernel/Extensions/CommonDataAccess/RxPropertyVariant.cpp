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

#undef _CRTDBG_MAP_ALLOC
#include "OdaCommon.h"
#include "RxPropertyVariant.h"

#if defined(_MSC_VER)
#pragma warning (push)
#pragma warning ( disable : 4065 ) //  switch statement contains 'default' but no 'case' labels
#pragma warning ( disable : 4100 ) //  unreferenced formal parameter
#endif


void* OdRxPropertyVariant::data() const
{
  return (void*)m_uData.uint8;
}

int OdRxPropertyVariant::varType() const
{
  return m_type;
}

OdRxPropertyVariant::Type OdRxPropertyVariant::type() const
{
  return Type(m_type & (0x3F));
}

bool OdRxPropertyVariant::isArray() const
{
  return (m_type & kArray) != 0;
}

struct _void_factory : OdRxPropertyVariant::TypeFactory
{
  _void_factory() {}
  virtual void construct(void*) const { }
  virtual void destroy(void*) const { }
};

template<class T>
struct _mem_factory : OdRxPropertyVariant::TypeFactory
{
  _mem_factory() {}
  virtual void construct(void* pData) const { ::memset(pData, 0, OdRxPropertyVariantDataSize); }
  virtual void destroy(void*) const { }
};

template<class T>
struct _obj_factory : OdRxPropertyVariant::TypeFactory
{
  _obj_factory() {}
  virtual void construct(void* pData) const
  {
    new ((void*)(pData)) T;
  }
  virtual void destroy(void* pData) const
  {
    ((T*)pData)->~T();
  }
};

template <class T>
struct _ptr_factory : OdRxPropertyVariant::TypeFactory {
  _ptr_factory() {}
  virtual ~_ptr_factory() {}
  virtual void construct(void* pData) const {
    ::memset(pData, 0, OdRxPropertyVariantDataSize);
    T** ppT = reinterpret_cast<T**>(pData);
    (*ppT) = new T;
  }

  virtual void destroy(void* pData) const {
    T** ppT = reinterpret_cast<T**>(pData);
    delete (*ppT);
    ::memset(pData, 0, OdRxPropertyVariantDataSize);
  }
};

#define TYPE_CONTAINER \
  VARTYPE(String,   obj)\
  VARTYPE(AnsiString,  obj)\
  VARTYPE(Bool,     mem)\
  VARTYPE(Int8,     mem)\
  VARTYPE(Int16,    mem)\
  VARTYPE(Int32,    mem)\
  VARTYPE(Int64,    mem)\
  VARTYPE(Double,   mem)\
  VARTYPE(CmEntityColor,obj)\
  VARTYPE(CmTransparency, obj)\
  PTRTYPE(GePoint2d, ptr)\
  PTRTYPE(GePoint3d, ptr)\
  PTRTYPE(GeVector2d, ptr)\
  PTRTYPE(GeVector3d, ptr)\
  PTRTYPE(GeMatrix2d,  ptr)\
  PTRTYPE(GeMatrix3d, ptr)\
  PTRTYPE(GeScale3d, ptr)\
  PTRTYPE(TimeStamp,ptr)\
  PTRTYPE(GeExtents2d,ptr)\
  PTRTYPE(GeExtents3d,ptr)\
  PTRTYPE(GeQuaternion,ptr)\
  PTRTYPE(GiLightAttenuation,ptr)\
  PTRTYPE(GiSkyParameters,ptr)\
  PTRTYPE(GiShadowParameters,ptr)\
  PTRTYPE(GiColorRGB,ptr)\
  VARTYPE(GiMrExposureType,mem)\
  VARTYPE(GiMrShadowMode,mem)\
  VARTYPE(GiMrFinalGatheringMode,mem)\
  VARTYPE(GiMrDiagnosticMode,mem)\
  VARTYPE(GiMrDiagnosticPhotonMode,mem)\
  VARTYPE(GiMrDiagnosticBSPMode,mem)\
  VARTYPE(GiMrExportMIMode,mem)\
  VARTYPE(GiMrTileOrder,mem)\
  VARTYPE(GiLightingMode,mem)\
  VARTYPE(GiFilterType,mem)\
  PTRTYPE(GiMaterialColor,ptr)\
  VARTYPEENUM(CmEntityColor,ColorMethod,mem)\
  VARTYPEENUM(Db,LineWeight,mem)\
  VARTYPEENUM(Db,UnitsValue,mem)\
  VARTYPEENUM(GiDgLinetypeModifiers,WidthMode,mem)\
  VARTYPEENUM(GiDgLinetypeModifiers,ShiftMode,mem)\
  VARTYPEENUM(GiDgLinetypeModifiers,CornersMode,mem)\
  VARTYPEENUM(Ge,OdGeKnotParameterization,mem)\
  VARTYPEENUM(GiLightAttenuation,AttenuationType,mem)\
  VARTYPEENUM(GiDrawable,DrawableType,mem)\
  VARTYPEENUM(GiShadowParameters,ShadowType,mem)\
  VARTYPEENUM(GiShadowParameters,ExtendedLightShape,mem)\
  VARTYPEENUM(GiViewportTraits,DefaultLightingType,mem)\
  VARTYPEENUM(GiMaterialTraits,LuminanceMode,mem)\
  VARTYPEENUM(GiMaterialTraits,GlobalIlluminationMode,mem)\
  VARTYPEENUM(GiMaterialTraits,FinalGatherMode,mem)\
  VARTYPEENUM(GiMaterialTraits,ChannelFlags,mem)\
  VARTYPEENUM(GiMaterialTraits,Mode,mem)\
  VARTYPEENUM(GiMaterialTraits,IlluminationModel,mem)\
  VARTYPEENUM(GiVisualStyle,Type,mem)\
  VARTYPEENUM(GiVisualStyleProperties,FaceColorMode,mem)\
  VARTYPEENUM(GiVisualStyleProperties,EdgeModel,mem)\
  VARTYPEENUM(GiFaceStyle,FaceColorMode,mem)\
  VARTYPEENUM(GiEdgeStyle,EdgeModel,mem)\
  VARTYPEENUM(GiToneOperatorParameters,ExteriorDaylightMode,mem)


#undef VARTYPE
#define VARTYPE(Type, storage) \
ODA_ASSUME(sizeof(Od##Type)      <= OdRxPropertyVariantDataSize)\
ODA_ASSUME(sizeof(Od##Type##Array) <= OdRxPropertyVariantDataSize)

#undef PTRTYPE
#define PTRTYPE(Type, storage) \
ODA_ASSUME(sizeof(Od##Type*)      <= OdRxPropertyVariantDataSize)\
ODA_ASSUME(sizeof(Od##Type##Array) <= OdRxPropertyVariantDataSize)

#undef VARTYPEENUM
#define VARTYPEENUM(NameSp, Type, storage) \
ODA_ASSUME(sizeof(Od##NameSp::Type)      <= OdRxPropertyVariantDataSize)\
ODA_ASSUME(sizeof(Od##NameSp##_##Type##Array) <= OdRxPropertyVariantDataSize)

TYPE_CONTAINER 

const _void_factory g_void_factory;

#undef VARTYPE
#define VARTYPE(Type, storage) \
const _##storage##_factory<Od##Type> g_##Type##_factory;

#undef PTRTYPE
#define PTRTYPE(Type, storage) VARTYPE(Type, storage)

#undef VARTYPEENUM
#define VARTYPEENUM(NameSp, Type, storage) \
const _##storage##_factory<Od##NameSp::Type> g_##NameSp##_##Type##_factory;

TYPE_CONTAINER

#undef VARTYPE
#define VARTYPE(Type, Allocator) \
const _obj_factory<Od##Type##Array> g_##Type##Array_factory;

#undef VARTYPEENUM
#define VARTYPEENUM(NameSp, Type, storage) \
const _obj_factory<Od##NameSp::Type> g_##NameSp##_##Type##Array_factory;

TYPE_CONTAINER

const OdRxPropertyVariant::TypeFactory* OdRxPropertyVariant::typeFactory(int type)
{
  switch(type)
  {
  case OdRxPropertyVariant::kVoid:
    return &g_void_factory;

#undef VARTYPE
#define VARTYPE(Type, storage) \
  case OdRxPropertyVariant::k##Type:\
    return &g_##Type##_factory;

#undef VARTYPEENUM
#define VARTYPEENUM(NameSp, Type, storage) \
  case OdRxPropertyVariant::k##NameSp##_##Type:\
    return &g_##NameSp##_##Type##_factory;

  TYPE_CONTAINER

#undef VARTYPE
#define VARTYPE(Type, storage) \
  case OdRxPropertyVariant::k##Type | OdRxPropertyVariant::kArray:\
    return &g_##Type##Array_factory;

#undef VARTYPEENUM
#define VARTYPEENUM(NameSp, Type, storage) \
  case OdRxPropertyVariant::k##NameSp##_##Type | OdRxPropertyVariant::kArray:\
    return &g_##NameSp##_##Type##Array_factory;

  TYPE_CONTAINER

  };
  return 0;
}

void OdRxPropertyVariant::setVarType(int newType, int& type, void* data)
{
  const OdRxPropertyVariant::TypeFactory* pOldFactory = OdRxPropertyVariant::typeFactory(type);
  const OdRxPropertyVariant::TypeFactory* pNewFactory = OdRxPropertyVariant::typeFactory(newType);
  if(pOldFactory!=pNewFactory)
  {
    pOldFactory->destroy(data);
    pNewFactory->construct(data);
  }
  type = newType;
}

inline bool isInt(int type)
{
  return (type >= OdRxPropertyVariant::kBool && type <= OdRxPropertyVariant::kUInt64);
}

OdError_InvalidRxPropertyVariantType::OdError_InvalidRxPropertyVariantType() : OdError(eInvalidResBuf) {}

inline void assertIntType(int to, int from)
{
  if (to != from && !(to > from&& isInt(to) && isInt(from)))
    throw OdError_InvalidRxPropertyVariantType();
}

inline void assertType(int type1, int type2)
{
  if (type1 != type2)
    throw OdError_InvalidRxPropertyVariantType();
}

OdRxPropertyVariant::OdRxPropertyVariant()
  :m_type(OdRxPropertyVariant::kVoid)
{
}

OdRxPropertyVariant::OdRxPropertyVariant(const OdRxPropertyVariant& val)
  :m_type(OdRxPropertyVariant::kVoid)
{
  OdRxPropertyVariant::operator =(val);
}

OdRxPropertyVariant& OdRxPropertyVariant::operator =(const OdRxPropertyVariant& val)
{
  
  switch (val.varType())
  {
  case OdRxPropertyVariant::kVoid:
    setVarType(kVoid, m_type, data());
    return *this;

#undef VARTYPE
#define VARTYPE(Type, storage) \
  case OdRxPropertyVariant::k##Type:\
    return set##Type(val.get##Type());\

#undef VARTYPEENUM
#define VARTYPEENUM(NameSp, Type, storage) \
  case OdRxPropertyVariant::k##NameSp##_##Type:\
    return set##NameSp##_##Type(val.get##NameSp##_##Type());

    TYPE_CONTAINER

#undef VARTYPE
#define VARTYPE(Type, storage) \
  case OdRxPropertyVariant::k##Type | OdRxPropertyVariant::kArray:\
    return set##Type##Array(val.get##Type##Array());\

#undef VARTYPEENUM
#define VARTYPEENUM(NameSp, Type, storage) \
  case OdRxPropertyVariant::k##NameSp##_##Type | OdRxPropertyVariant::kArray:\
    return set##NameSp##_##Type##Array(val.get##NameSp##_##Type##Array());

    TYPE_CONTAINER

  default:
    throw OdError_InvalidRxPropertyVariantType();
    break;
  }
  //	pmk 08.27.2004 -- Unreachable Code
#if !defined(ODA_WINDOWS)
  return *this;
#endif
}

OdRxPropertyVariant::~OdRxPropertyVariant()
{
  setVarType(kVoid, m_type, data());
}

#undef TYPE_CONTAINER
#define TYPE_CONTAINER \
  VCRTYPE(String )\
  VCRTYPE(AnsiString )\
  VARTYPE(Bool )\
  VARTYPE(Int8 )\
  VARTYPE(Int16 )\
  VARTYPE(Int32 )\
  VARTYPE(Int64 )\
  VARTYPE(UInt8  )\
  VARTYPE(UInt16 )\
  VARTYPE(UInt32 )\
  VARTYPE(UInt64 )\
  VCRTYPE(Double )\
  VCRTYPE(CmEntityColor )\
  VCRTYPE(CmTransparency )\
  PTRTYPE(GePoint2d )\
  PTRTYPE(GePoint3d )\
  PTRTYPE(GeVector2d )\
  PTRTYPE(GeVector3d )\
  PTRTYPE(GeMatrix2d )\
  PTRTYPE(GeMatrix3d )\
  PTRTYPE(GeScale3d )\
  PTRTYPE(TimeStamp)\
  PTRTYPE(GeExtents2d)\
  PTRTYPE(GeExtents3d)\
  PTRTYPE(GeQuaternion)\
  PTRTYPE(GiLightAttenuation)\
  PTRTYPE(GiSkyParameters)\
  PTRTYPE(GiShadowParameters)\
  PTRTYPE(GiColorRGB)\
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
  PTRTYPE(GiMaterialColor)\
  VARTYPEENUM(CmEntityColor,ColorMethod)\
  VARTYPEENUM(Db,LineWeight)\
  VARTYPEENUM(Db,UnitsValue)\
  VARTYPEENUM(GiDgLinetypeModifiers,WidthMode)\
  VARTYPEENUM(GiDgLinetypeModifiers,ShiftMode)\
  VARTYPEENUM(GiDgLinetypeModifiers,CornersMode)\
  VARTYPEENUM(Ge,OdGeKnotParameterization)\
  VARTYPEENUM(GiLightAttenuation,AttenuationType)\
  VARTYPEENUM(GiDrawable,DrawableType)\
  VARTYPEENUM(GiShadowParameters,ShadowType)\
  VARTYPEENUM(GiShadowParameters,ExtendedLightShape)\
  VARTYPEENUM(GiViewportTraits,DefaultLightingType)\
  VARTYPEENUM(GiMaterialTraits,LuminanceMode)\
  VARTYPEENUM(GiMaterialTraits,GlobalIlluminationMode)\
  VARTYPEENUM(GiMaterialTraits,FinalGatherMode)\
  VARTYPEENUM(GiMaterialTraits,ChannelFlags)\
  VARTYPEENUM(GiMaterialTraits,Mode)\
  VARTYPEENUM(GiMaterialTraits,IlluminationModel)\
  VARTYPEENUM(GiVisualStyle,Type)\
  VARTYPEENUM(GiVisualStyleProperties,FaceColorMode)\
  VARTYPEENUM(GiVisualStyleProperties,EdgeModel)\
  VARTYPEENUM(GiFaceStyle,FaceColorMode)\
  VARTYPEENUM(GiEdgeStyle,EdgeModel)\
  VARTYPEENUM(GiToneOperatorParameters,ExteriorDaylightMode)

#define ODTYPE(Type) Od##Type

// defining OdRxPropertyVariant explicit constructors

#undef FUNC_HEADER_VCRTYPE
#define FUNC_HEADER_VCRTYPE(Type) \
OdRxPropertyVariant::OdRxPropertyVariant(const ODTYPE(Type)& val)

#undef FUNC_HEADER_VARTYPE
#define FUNC_HEADER_VARTYPE(Type) \
OdRxPropertyVariant::OdRxPropertyVariant(const ODTYPE(Type) val)

#undef BODY
#define BODY(Type, value) { setVarType(k##Type, m_type, data()); *((Od##Type *)data()) = value; }

#undef VARTYPE
#define VARTYPE(Type) \
FUNC_HEADER_VARTYPE(Type) BODY(Type, val)

#undef VCRTYPE
#define VCRTYPE(Type) \
FUNC_HEADER_VCRTYPE(Type) BODY(Type, val)

#undef PTRTYPE
#define PTRTYPE(Type) \
FUNC_HEADER_VCRTYPE(Type) { setVarType(k##Type, m_type, data()); **(reinterpret_cast<Od##Type**>(data())) = val; }

#undef VARTYPEENUM
#define VARTYPEENUM(NameSp, Type) \
  OdRxPropertyVariant::OdRxPropertyVariant(const Od##NameSp::Type value)\
  { setVarType(k##NameSp##_##Type, m_type, data()); *((Od##NameSp::Type *)data()) = value; }

TYPE_CONTAINER

// defining OdRxPropertyVariant::setXxxx()

#undef FUNC_HEADER_VCRTYPE
#define FUNC_HEADER_VCRTYPE(Type) \
OdRxPropertyVariant& OdRxPropertyVariant::set##Type(const ODTYPE(Type)& val)

#undef FUNC_HEADER_VARTYPE
#define FUNC_HEADER_VARTYPE(Type) \
OdRxPropertyVariant& OdRxPropertyVariant::set##Type(const ODTYPE(Type) val)

#undef BODY
#define BODY(Type, value) { setVarType(k##Type, m_type, data()); *((Od##Type *)data()) = value; return *this; }

#undef VARTYPE
#define VARTYPE(Type) \
FUNC_HEADER_VARTYPE(Type) BODY(Type, val)

#undef VCRTYPE
#define VCRTYPE(Type) \
FUNC_HEADER_VCRTYPE(Type) BODY(Type, val)

#undef PTRTYPE
#define PTRTYPE(Type) \
FUNC_HEADER_VCRTYPE(Type) { setVarType(k##Type, m_type, data()); **(reinterpret_cast<Od##Type**>(data())) = val; return *this; }

#undef VARTYPEENUM
#define VARTYPEENUM(NameSp, Type) \
  OdRxPropertyVariant& OdRxPropertyVariant::set##NameSp##_##Type(const Od##NameSp::Type value)\
  { setVarType(k##NameSp##_##Type, m_type, data()); *((Od##NameSp::Type *)data()) = value; return *this;}

TYPE_CONTAINER

// defining OdRxPropertyVariant::setXxxxArray()

#undef BODY
#define BODY(Type, value) { setVarType(k##Type | kArray, m_type, data()); *((Od##Type##Array*)data()) = value; return *this; }

#undef VARTYPE
#define VARTYPE(Type) \
OdRxPropertyVariant& OdRxPropertyVariant::set##Type##Array(const Od##Type##Array& array) BODY(Type, array)

#undef VCRTYPE
#define VCRTYPE(Type) VARTYPE(Type)

#undef PTRTYPE
#define PTRTYPE(Type) VCRTYPE(Type)

#undef VARTYPEENUM
#define VARTYPEENUM(NameSp, Type) \
  OdRxPropertyVariant& OdRxPropertyVariant::set##NameSp##_##Type##Array(const Od##NameSp##_##Type##Array& array)\
{ setVarType(k##NameSp##_##Type | kArray, m_type, data()); *((Od##NameSp##_##Type##Array*)data()) = array; return *this; }

TYPE_CONTAINER

// defining OdRxPropertyVariant::getXxxx()

#undef BODY
#define BODY(Type) const { assertType(k##Type, m_type); return *((Od##Type *)data()); }

#undef VCRTYPE
#define VCRTYPE(Type) \
const ODTYPE(Type)& OdRxPropertyVariant::get##Type() BODY(Type)

#undef VARTYPE
#define VARTYPE(Type) \
ODTYPE(Type) OdRxPropertyVariant::get##Type() BODY(Type)

#undef PTRTYPE
#define PTRTYPE(Type)\
const ODTYPE(Type)& OdRxPropertyVariant::get##Type() const { assertType(k##Type, m_type); return **(reinterpret_cast<Od##Type**>(data())); }

#undef VARTYPEENUM
#define VARTYPEENUM(NameSp, Type) \
  Od##NameSp::Type OdRxPropertyVariant::get##NameSp##_##Type() const\
  { assertType(k##NameSp##_##Type, m_type); return *((Od##NameSp::Type *)data()); }

TYPE_CONTAINER

// defining OdRxPropertyVariant::getXxxxArray()

#undef VARTYPE
#define VARTYPE(Type) \
const Od##Type##Array& OdRxPropertyVariant::get##Type##Array() const { assertType(k##Type | kArray, m_type); return *((Od##Type##Array*)data()); }

#undef VCRTYPE
#define VCRTYPE(Type) VARTYPE(Type)

#undef PTRTYPE
#define PTRTYPE(Type) VCRTYPE(Type)

#undef VARTYPEENUM
#define VARTYPEENUM(NameSp, Type) \
  const Od##NameSp##_##Type##Array& OdRxPropertyVariant::get##NameSp##_##Type##Array() const\
{ assertType(k##NameSp##_##Type | kArray, m_type); return *((Od##NameSp##_##Type##Array*)data()); }

TYPE_CONTAINER

// defining OdRxPropertyVariant::asXxxxArray

#undef VARTYPE
#define VARTYPE(Type) \
Od##Type##Array& OdRxPropertyVariant::as##Type##Array()\
{ setVarType(k##Type | kArray, m_type, data()); return *((Od##Type##Array*)data()); }

#undef VARTYPEENUM
#define VARTYPEENUM(NameSp, Type) \
  Od##NameSp##_##Type##Array& OdRxPropertyVariant::as##NameSp##_##Type##Array()\
{ setVarType(k##NameSp##_##Type | kArray, m_type, data()); return *((Od##NameSp##_##Type##Array*)data()); }

TYPE_CONTAINER

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
  PTRTYPE(GePoint2d      )\
  PTRTYPE(GePoint3d      )\
  PTRTYPE(GeVector2d     )\
  PTRTYPE(GeVector3d     )\
  VARTYPE(CmEntityColor  )\
  VARTYPE(CmTransparency )\
  PTRTYPE(GeMatrix2d )\
  PTRTYPE(GeMatrix3d )\
  PTRTYPE(GeScale3d )\
  PTRTYPE(TimeStamp)\
  PTRTYPE(GeExtents2d)\
  PTRTYPE(GeExtents3d)\
  PTRTYPE(GeQuaternion)\
  PTRTYPE(GiLightAttenuation)\
  PTRTYPE(GiSkyParameters)\
  PTRTYPE(GiShadowParameters)\
  PTRTYPE(GiColorRGB)\
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
  PTRTYPE(GiMaterialColor)\
  VARTYPEENUM(CmEntityColor,ColorMethod)\
  VARTYPEENUM(Db,LineWeight)\
  VARTYPEENUM(Db,UnitsValue)\
  VARTYPEENUM(GiDgLinetypeModifiers,WidthMode)\
  VARTYPEENUM(GiDgLinetypeModifiers,ShiftMode)\
  VARTYPEENUM(GiDgLinetypeModifiers,CornersMode)\
  VARTYPEENUM(Ge,OdGeKnotParameterization)\
  VARTYPEENUM(GiLightAttenuation,AttenuationType)\
  VARTYPEENUM(GiDrawable,DrawableType)\
  VARTYPEENUM(GiShadowParameters,ShadowType)\
  VARTYPEENUM(GiShadowParameters,ExtendedLightShape)\
  VARTYPEENUM(GiViewportTraits,DefaultLightingType)\
  VARTYPEENUM(GiMaterialTraits,LuminanceMode)\
  VARTYPEENUM(GiMaterialTraits,GlobalIlluminationMode)\
  VARTYPEENUM(GiMaterialTraits,FinalGatherMode)\
  VARTYPEENUM(GiMaterialTraits,ChannelFlags)\
  VARTYPEENUM(GiMaterialTraits,Mode)\
  VARTYPEENUM(GiMaterialTraits,IlluminationModel)\
  VARTYPEENUM(GiVisualStyle,Type)\
  VARTYPEENUM(GiVisualStyleProperties,FaceColorMode)\
  VARTYPEENUM(GiVisualStyleProperties,EdgeModel)\
  VARTYPEENUM(GiFaceStyle,FaceColorMode)\
  VARTYPEENUM(GiEdgeStyle,EdgeModel)\
  VARTYPEENUM(GiToneOperatorParameters,ExteriorDaylightMode)

#define ODTYPE(Type) Od##Type

// defining OdRxPropertyVariant::operator =

#undef FUNC_HEADER
#define FUNC_HEADER(Type) \
OdRxPropertyVariant& OdRxPropertyVariant::operator =(const ODTYPE(Type)& val)

#undef VARTYPE
#define VARTYPE(Type) \
FUNC_HEADER(Type) { setVarType(k##Type, m_type, data()); *((Od##Type *)data()) = val; return *this;}

#undef PTRTYPE
#define PTRTYPE(Type) \
FUNC_HEADER(Type) { setVarType(k##Type, m_type, data()); **(reinterpret_cast<Od##Type**>(data())) = val; return *this; }

#undef VARTYPEENUM
#define VARTYPEENUM(NameSp, Type) \
  OdRxPropertyVariant& OdRxPropertyVariant::operator =(const Od##NameSp::Type& value)\
  { setVarType(k##NameSp##_##Type, m_type, data()); *((Od##NameSp::Type *)data()) = value; return *this;}

TYPE_CONTAINER




