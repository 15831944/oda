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




#ifndef _ODRXPROPERTYVARIANT_INCLUDED_
#define _ODRXPROPERTYVARIANT_INCLUDED_

#include "TD_PackPush.h"

#include "OdPlatformSettings.h"

#include "StringArray.h"
#include "BoolArray.h"
#include "Int8Array.h"
#include "Int16Array.h"
#include "Int32Array.h"
#include "Int64Array.h"
#include "UInt8Array.h"
#include "UInt16Array.h"
#include "UInt32Array.h"
#include "UInt64Array.h"
#include "DoubleArray.h"
#include "OdString.h"
#include "OdAnsiString.h"

#include "CDAExport.h"
#include "Ge/GePoint3d.h"
#include "Ge/GePoint2d.h"
#include "CmColorBase.h"
#include "Ge/GeVector2d.h"
#include "Ge/GeVector3d.h"
#include "Ge/GeVector3dArray.h"
#include "Ge/GeVector2dArray.h"
#include "Gs/GsFiler.h"
#include "CmEntityColorArray.h"
#include "CmTransparencyArray.h"
#include "Ge/GeMatrix2d.h"
#include "Ge/GeMatrix3d.h"
#include "Ge/GeMatrix2dArray.h"
#include "Ge/GeMatrix3dArray.h"
#include "OdTimeStamp.h"
#include "Ge/GeScale3d.h"
#include "Ge/GeExtents2d.h"
#include "Ge/GeExtents3d.h"
#include "Ge/GeQuaternion.h"
#include "Gi/GiDgLinetype.h"
#include "Gi/GiLightTraits.h"
#include "OdPlatform.h"
#include "Gi/GiDgLinetype.h"
#include "Gi/GiEnvironment.h"
#include "Gi/GiMaterial.h"
#include "Gi/GiViewportTraits.h"
#include "Gi/GiVisualStyle.h"


//DOM-IGNORE-BEGIN
const int OdRxPropertyVariantDataSize = 8;
//DOM-IGNORE-END

/** \details
  This class implements OdError objects with Result codes of
  eInvalidVariantType.

  Corresponding C++ library: TD_Root
  <group Error_Classes>
*/
class ODCDA_EXPORT OdError_InvalidRxPropertyVariantType : public OdError
{
public:
  /** \details
    Default constructor for objects of the OdError_InvalidRxPropertyVariantType
    class.
  */
  OdError_InvalidRxPropertyVariantType();
};

/** \details
  This template class is a specialization of the OdArray class for OdGeScale3d
  objects.
*/
typedef OdArray<OdGeScale3d, OdMemoryAllocator<OdGeScale3d> > OdGeScale3dArray;

/** \details
  This template class is a specialization of the OdArray class for OdTimeStamp
  objects.
*/
typedef OdArray<OdTimeStamp, OdMemoryAllocator<OdTimeStamp> > OdTimeStampArray;

/** \details
  This template class is a specialization of the OdArray class for OdGeExtents2d
  objects.
*/
typedef OdArray<OdGeExtents2d, OdMemoryAllocator<OdGeExtents2d> > OdGeExtents2dArray;

/** \details
  This template class is a specialization of the OdArray class for OdGeExtents3d
  objects.
*/
typedef OdArray<OdGeExtents3d, OdMemoryAllocator<OdGeExtents3d> > OdGeExtents3dArray;

/** \details
  This template class is a specialization of the OdArray class for
  OdGeQuaternion objects.
*/
typedef OdArray<OdGeQuaternion, OdMemoryAllocator<OdGeQuaternion> > OdGeQuaternionArray;

/** \details
  This template class is a specialization of the OdArray class for
  OdGiLightAttenuation objects.
*/
typedef OdArray<OdGiLightAttenuation, OdMemoryAllocator<OdGiLightAttenuation> > OdGiLightAttenuationArray;

/** \details
  This template class is a specialization of the OdArray class for
  OdGiSkyParameters objects.
*/
typedef OdArray<OdGiSkyParameters, OdMemoryAllocator<OdGiSkyParameters> > OdGiSkyParametersArray;

/** \details
  This template class is a specialization of the OdArray class for
  OdGiShadowParameters objects.
*/
typedef OdArray<OdGiShadowParameters, OdMemoryAllocator<OdGiShadowParameters> > OdGiShadowParametersArray;

/** \details
  This template class is a specialization of the OdArray class for OdGiColorRGB
  objects.
*/
typedef OdArray<OdGiColorRGB, OdMemoryAllocator<OdGiColorRGB> > OdGiColorRGBArray;

/** \details
  This template class is a specialization of the OdArray class for
  OdGiMrExposureType objects.
*/
typedef OdArray<OdGiMrExposureType, OdMemoryAllocator<OdGiMrExposureType> > OdGiMrExposureTypeArray;

/** \details
  This template class is a specialization of the OdArray class for
  OdGiMrShadowMode objects.
*/
typedef OdArray<OdGiMrShadowMode, OdMemoryAllocator<OdGiMrShadowMode> > OdGiMrShadowModeArray;

/** \details
  This template class is a specialization of the OdArray class for
  OdGiMrFinalGatheringMode objects.
*/
typedef OdArray<OdGiMrFinalGatheringMode, OdMemoryAllocator<OdGiMrFinalGatheringMode> > OdGiMrFinalGatheringModeArray;

/** \details
  This template class is a specialization of the OdArray class for
  OdGiMrDiagnosticMode objects.
*/
typedef OdArray<OdGiMrDiagnosticMode, OdMemoryAllocator<OdGiMrDiagnosticMode> > OdGiMrDiagnosticModeArray;

/** \details
  This template class is a specialization of the OdArray class for
  OdGiMrDiagnosticPhotonMode objects.
*/
typedef OdArray<OdGiMrDiagnosticPhotonMode, OdMemoryAllocator<OdGiMrDiagnosticPhotonMode> > OdGiMrDiagnosticPhotonModeArray;

/** \details
  This template class is a specialization of the OdArray class for
  OdGiMrDiagnosticBSPMode objects.
*/
typedef OdArray<OdGiMrDiagnosticBSPMode, OdMemoryAllocator<OdGiMrDiagnosticBSPMode> > OdGiMrDiagnosticBSPModeArray;

/** \details
  This template class is a specialization of the OdArray class for
  OdGiMrExportMIMode objects.
*/
typedef OdArray<OdGiMrExportMIMode, OdMemoryAllocator<OdGiMrExportMIMode> > OdGiMrExportMIModeArray;

/** \details
  This template class is a specialization of the OdArray class for
  OdGiMrTileOrder objects.
*/
typedef OdArray<OdGiMrTileOrder, OdMemoryAllocator<OdGiMrTileOrder> > OdGiMrTileOrderArray;

/** \details
  This template class is a specialization of the OdArray class for
  OdGiLightingMode objects.
*/
typedef OdArray<OdGiLightingMode, OdMemoryAllocator<OdGiLightingMode> > OdGiLightingModeArray;

/** \details
  This template class is a specialization of the OdArray class for
  OdGiFilterType objects.
*/
typedef OdArray<OdGiFilterType, OdMemoryAllocator< OdGiFilterType> > OdGiFilterTypeArray;

/** \details
  This template class is a specialization of the OdArray class for
  OdGiMaterialColor objects.
*/
typedef OdArray<OdGiMaterialColor, OdMemoryAllocator<OdGiMaterialColor> > OdGiMaterialColorArray;

/** \details
  This template class is a specialization of the OdArray class for
  OdCmEntityColor::OdCmEntityColor objects.
*/
typedef OdArray<OdCmEntityColor::ColorMethod, OdMemoryAllocator<OdCmEntityColor::ColorMethod> > OdCmEntityColor_ColorMethodArray;

/** \details
  This template class is a specialization of the OdArray class for
  OdDb::LineWeight objects.
*/
typedef OdArray<OdDb::LineWeight, OdMemoryAllocator<OdDb::LineWeight> > OdDb_LineWeightArray;

/** \details
  This template class is a specialization of the OdArray class for
  OdDb::UnitsValue objects.
*/
typedef OdArray<OdDb::UnitsValue, OdMemoryAllocator<OdDb::UnitsValue> > OdDb_UnitsValueArray;

/** \details
  This template class is a specialization of the OdArray class for
  OdGiDgLinetypeModifiers::WidthMode objects.
*/
typedef OdArray<OdGiDgLinetypeModifiers::WidthMode, OdMemoryAllocator<OdGiDgLinetypeModifiers::WidthMode> > OdGiDgLinetypeModifiers_WidthModeArray;

/** \details
  This template class is a specialization of the OdArray class for
  OdGiDgLinetypeModifiers::ShiftMode objects.
*/
typedef OdArray<OdGiDgLinetypeModifiers::ShiftMode, OdMemoryAllocator<OdGiDgLinetypeModifiers::ShiftMode> > OdGiDgLinetypeModifiers_ShiftModeArray;

/** \details
  This template class is a specialization of the OdArray class for
  OdGiDgLinetypeModifiers::CornersMode objects.
*/
typedef OdArray<OdGiDgLinetypeModifiers::CornersMode, OdMemoryAllocator<OdGiDgLinetypeModifiers::CornersMode> > OdGiDgLinetypeModifiers_CornersModeArray;

/** \details
  This template class is a specialization of the OdArray class for
  OdGe::OdGeKnotParameterization objects.
*/
typedef OdArray<OdGe::OdGeKnotParameterization, OdMemoryAllocator<OdGe::OdGeKnotParameterization> > OdGe_OdGeKnotParameterizationArray;

/** \details
  This template class is a specialization of the OdArray class for
  OdGiLightAttenuation::AttenuationType objects.
*/
typedef OdArray<OdGiLightAttenuation::AttenuationType, OdMemoryAllocator<OdGiLightAttenuation::AttenuationType> > OdGiLightAttenuation_AttenuationTypeArray;

/** \details
  This template class is a specialization of the OdArray class for
  OdGiDrawable::DrawableType objects.
*/
typedef OdArray<OdGiDrawable::DrawableType, OdMemoryAllocator<OdGiDrawable::DrawableType> > OdGiDrawable_DrawableTypeArray;

/** \details
  This template class is a specialization of the OdArray class for
  OdGiShadowParameters::ShadowType objects.
*/
typedef OdArray<OdGiShadowParameters::ShadowType, OdMemoryAllocator<OdGiShadowParameters::ShadowType> > OdGiShadowParameters_ShadowTypeArray;

/** \details
  This template class is a specialization of the OdArray class for
  OdGiShadowParameters::ExtendedLightShape objects.
*/
typedef OdArray<OdGiShadowParameters::ExtendedLightShape, OdMemoryAllocator<OdGiShadowParameters::ExtendedLightShape> > OdGiShadowParameters_ExtendedLightShapeArray;

/** \details
  This template class is a specialization of the OdArray class for
  OdGiViewportTraits::DefaultLightingType objects.
*/
typedef OdArray<OdGiViewportTraits::DefaultLightingType, OdMemoryAllocator<OdGiViewportTraits::DefaultLightingType> > OdGiViewportTraits_DefaultLightingTypeArray;

/** \details
  This template class is a specialization of the OdArray class for
  OdGiMaterialTraits::LuminanceMode objects.
*/
typedef OdArray<OdGiMaterialTraits::LuminanceMode, OdMemoryAllocator<OdGiMaterialTraits::LuminanceMode> > OdGiMaterialTraits_LuminanceModeArray;

/** \details
  This template class is a specialization of the OdArray class for
  OdGiMaterialTraits::GlobalIlluminationMode objects.
*/
typedef OdArray<OdGiMaterialTraits::GlobalIlluminationMode, OdMemoryAllocator<OdGiMaterialTraits::GlobalIlluminationMode> > OdGiMaterialTraits_GlobalIlluminationModeArray;

/** \details
  This template class is a specialization of the OdArray class for
  OdGiMaterialTraits::FinalGatherMode objects.
*/
typedef OdArray<OdGiMaterialTraits::FinalGatherMode, OdMemoryAllocator<OdGiMaterialTraits::FinalGatherMode> > OdGiMaterialTraits_FinalGatherModeArray;

/** \details
  This template class is a specialization of the OdArray class for
  OdGiMaterialTraits::ChannelFlags objects.
*/
typedef OdArray<OdGiMaterialTraits::ChannelFlags, OdMemoryAllocator<OdGiMaterialTraits::ChannelFlags> > OdGiMaterialTraits_ChannelFlagsArray;

/** \details
  This template class is a specialization of the OdArray class for
  OdGiMaterialTraits::Mode objects.
*/
typedef OdArray<OdGiMaterialTraits::Mode, OdMemoryAllocator<OdGiMaterialTraits::Mode> > OdGiMaterialTraits_ModeArray;

/** \details
  This template class is a specialization of the OdArray class for
  OdGiMaterialTraits::IlluminationModel objects.
*/
typedef OdArray<OdGiMaterialTraits::IlluminationModel, OdMemoryAllocator<OdGiMaterialTraits::IlluminationModel> > OdGiMaterialTraits_IlluminationModelArray;

/** \details
  This template class is a specialization of the OdArray class for
  dGiVisualStyle::Type objects.
*/
typedef OdArray<OdGiVisualStyle::Type, OdMemoryAllocator<OdGiVisualStyle::Type> > OdGiVisualStyle_TypeArray;

/** \details
  This template class is a specialization of the OdArray class for
  OdGiVisualStyleProperties::FaceColorMode objects.
*/
typedef OdArray<OdGiVisualStyleProperties::FaceColorMode, OdMemoryAllocator<OdGiVisualStyleProperties::FaceColorMode> > OdGiVisualStyleProperties_FaceColorModeArray;

/** \details
  This template class is a specialization of the OdArray class for
  OdGiVisualStyleProperties::EdgeModel objects.
*/
typedef OdArray<OdGiVisualStyleProperties::EdgeModel, OdMemoryAllocator<OdGiVisualStyleProperties::EdgeModel> > OdGiVisualStyleProperties_EdgeModelArray;

/** \details
  This template class is a specialization of the OdArray class for
  OdGiFaceStyle::FaceColorMode objects.
*/
typedef OdArray<OdGiFaceStyle::FaceColorMode, OdMemoryAllocator<OdGiFaceStyle::FaceColorMode> > OdGiFaceStyle_FaceColorModeArray;

/** \details
  This template class is a specialization of the OdArray class for
  OdGiEdgeStyle::EdgeModel objects.
*/
typedef OdArray<OdGiEdgeStyle::EdgeModel, OdMemoryAllocator<OdGiEdgeStyle::EdgeModel> > OdGiEdgeStyle_EdgeModelArray;

/** \details
  This template class is a specialization of the OdArray class for
  OdGiToneOperatorParameters::ExteriorDaylightMode objects.
*/
typedef OdArray<OdGiToneOperatorParameters::ExteriorDaylightMode, OdMemoryAllocator<OdGiToneOperatorParameters::ExteriorDaylightMode> > OdGiToneOperatorParameters_ExteriorDaylightModeArray;

/** \details
  This class defines objects to store property values in the property cache for
  the Common Data Access API.

  <group OdRx_Classes>
*/
class ODCDA_EXPORT OdRxPropertyVariant
{
protected:
  //DOM-IGNORE-BEGIN
  int m_type;
  union   // Union is required to have proper data alignment which is important
  {       // for some platforms
    OdUInt8 uint8[OdRxPropertyVariantDataSize];
    double  dbl;
    void* ptr;
  } m_uData;
  void* data() const;

  void setVarType(int newType, int& type, void* data);
  //DOM-IGNORE-END
public:

  /** \details
    This structure represents factory objects for different data types.
  */
  struct TypeFactory
  {

    /** \details
      Type factory structure constructor.

      \param pData [in] Data.
    */
    virtual void construct(void* pData) const = 0;

    /** \details
      Type factory structure destructor.

      \param pData [in] Data.
    */
    virtual void destroy(void* pData) const = 0;
  };

  /** \details
    Gets the pointer to the factory object for the corresponding type.

    \param type [in] Type value.

    \returns Pointer to the factory object for the corresponding type.
  */
  static const TypeFactory* typeFactory(int type);

  /** \details
    Defines additional types of stored values.
  */
  typedef enum
  {

    /** The value type is void */
    kVoid = 0x0000,

    /** The value type is OdString */
    kString = 0x0001,

    /** The value type is OdBool */
    kBool = 0x0002,

    /** The value type is OdInt8 */
    kInt8 = 0x0003,

    /** The value type is OdUInt 8*/
    kUInt8 = 0x0003,

    /** The value type is OdInt16 */
    kInt16 = 0x0004,

    /** The value type is OdUInt16 */
    kUInt16 = 0x0004,

    /** The value type is OdInt32 */
    kInt32 = 0x0005,

    /** The value type is OdUInt32 */
    kUInt32 = 0x0005,

    /** The value type is OdInt64 */
    kInt64 = 0x0006,

    /** The value type is OdUInt64 */
    kUInt64 = 0x0006,

    /** The value type is OdDouble */
    kDouble = 0x0007,

    /** The value type is OdAnsiString */
    kAnsiString = 0x0008,

    /** The value type is OdGePoint2d */
    kGePoint2d = 0x000A,

    /** The value type is OdGePoint3d */
    kGePoint3d = 0x000B,

    /** The value type is OdGeVector2d */
    kGeVector2d = 0x000C,

    /** The value type is OdGeVector3d */
    kGeVector3d = 0x000D,

    /** The value type is OdCmEntityColor */
    kCmEntityColor = 0x000E,

    /** The value type is OdCmTransparency */
    kCmTransparency = 0x000F,

    /** The value type is OdGeMatrix2d */
    kGeMatrix2d = 0x0010,

    /** The value type is OdGeMatrix3d */
    kGeMatrix3d = 0x0011,

    /** The value type is OdGeScale3d */
    kGeScale3d = 0x0012,

    /** The value type is OdTimeStamp */
    kTimeStamp = 0x0013,

    /** The value type is OdGeExtents2d */
    kGeExtents2d = 0x0014,

    /** The value type is OdGeExtents3d */
    kGeExtents3d = 0x0015,

    /** The value type is OdGeQuaternion */
    kGeQuaternion = 0x0016,

    /** The value type is OdGiLightAttenuation */
    kGiLightAttenuation = 0x0017,

    /** The value type is OdGiSkyParameters */
    kGiSkyParameters = 0x0018,

    /** The value type is OdGiShadowParameters */
    kGiShadowParameters = 0x0019,

    /** The value type is OdGiColorRGB */
    kGiColorRGB = 0x001A,

    /** The value type is OdGiMaterialColor */
    kGiMaterialColor = 0x001B,

    /** The value type is OdGiMrExposureType */
    kGiMrExposureType = 0x001C,

    /** The value type is OdGiMrShadowMode */
    kGiMrShadowMode = 0x001D,

    /** The value type is OdGiMrFinalGatheringMode */
    kGiMrFinalGatheringMode = 0x001E,

    /** The value type is OdGiMrDiagnosticMode */
    kGiMrDiagnosticMode = 0x001F,

    /** The value type is OdGiMrDiagnosticPhotonMode */
    kGiMrDiagnosticPhotonMode = 0x0020,

    /** The value type is OdGiMrDiagnosticBSPMode */
    kGiMrDiagnosticBSPMode = 0x0021,

    /** The value type is OdGiMrExportMIMode */
    kGiMrExportMIMode = 0x0022,

    /** The value type is OdGiMrTileOrder */
    kGiMrTileOrder = 0x0023,

    /** The value type is OdGiLightingMode */
    kGiLightingMode = 0x0024,

    /** The value type is OdGiFilterType */
    kGiFilterType = 0x0025,

    /** The value type is OdCmEntityColor::ColorMethod */
    kCmEntityColor_ColorMethod = 0x0026,

    /** The value type is OdDb::LineWeight */
    kDb_LineWeight = 0x0027,

    /** The value type is OdDb::UnitsValue */
    kDb_UnitsValue = 0x0028,

    /** The value type is OdGiDgLinetypeModifiers::WidthMode */
    kGiDgLinetypeModifiers_WidthMode = 0x0029,

    /** The value type is OdGiDgLinetypeModifiers::ShiftMode */
    kGiDgLinetypeModifiers_ShiftMode = 0x002A,

    /** The value type is OdGiDgLinetypeModifiers::CornersMode */
    kGiDgLinetypeModifiers_CornersMode = 0x002B,

    /** The value type is OdGe::OdGeKnotParameterization */
    kGe_OdGeKnotParameterization = 0x002C,

    /** The value type is OdGiLightAttenuation::AttenuationType */
    kGiLightAttenuation_AttenuationType = 0x002D,

    /** The value type is OdGiDrawable::DrawableType */
    kGiDrawable_DrawableType = 0x002E,

    /** The value type is OdGiShadowParameters::ShadowType */
    kGiShadowParameters_ShadowType = 0x002F,

    /** The value type is OdGiShadowParameters::ExtendedLightShape */
    kGiShadowParameters_ExtendedLightShape = 0x0030,

    /** The value type is OdGiViewportTraits::DefaultLightingType */
    kGiViewportTraits_DefaultLightingType = 0x0031,

    /** The value type is OdGiMaterialTraits::LuminanceMode */
    kGiMaterialTraits_LuminanceMode = 0x0032,

    /** The value type is OdGiMaterialTraits::GlobalIlluminationMode */
    kGiMaterialTraits_GlobalIlluminationMode = 0x0033,

    /** The value type is OdGiMaterialTraits::FinalGatherMode */
    kGiMaterialTraits_FinalGatherMode = 0x0034,

    /** The value type is OdGiMaterialTraits::ChannelFlags */
    kGiMaterialTraits_ChannelFlags = 0x0035,

    /** The value type is OdGiMaterialTraits::Mode */
    kGiMaterialTraits_Mode = 0x0036,

    /** The value type is OdGiMaterialTraits::IlluminationModel */
    kGiMaterialTraits_IlluminationModel = 0x0037,

    /** The value type is OdGiVisualStyle::Type */
    kGiVisualStyle_Type = 0x0038,

    /** The value type is OdGiVisualStyleProperties::FaceColorMode */
    kGiVisualStyleProperties_FaceColorMode = 0x0039,

    /** The value type is OdGiVisualStyleProperties::EdgeModel */
    kGiVisualStyleProperties_EdgeModel = 0x003A,

    /** The value type is OdGiFaceStyle::FaceColorMode */
    kGiFaceStyle_FaceColorMode = 0x003B,

    /** The value type is OdGiEdgeStyle::EdgeModel */
    kGiEdgeStyle_EdgeModel = 0x003C,

    /** The value type is OdGiToneOperatorParameters::ExteriorDaylightMode */
    kGiToneOperatorParameters_ExteriorDaylightMode = 0x003D,

    /** The value type is Array flag */
    kArray = 0x0080
  } Type;

  /** \details
    Default constructor for instances of the OdRxPropertyVariant class.
  */
  OdRxPropertyVariant();

  /** \details
    Copy constructor for instances of the OdRxPropertyVariant class.

    \param val [in] Constant reference to the OdRxPropertyVariant value.
  */
  OdRxPropertyVariant(const OdRxPropertyVariant& val);

  /** \details
    Returns the type of this OdRxPropertyVariant object, including ByRef and
    Array flags.

    \returns Type of this OdRxPropertyVariant object.
  */
  int varType() const;

  /** \details
    Returns the type of this Variant object, excluding ByRef and Array flags.

    \returns Type of this Variant object.
  */
  Type type() const;

  /** \details
    Returns true if and only if this OdRxPropertyVariant object is an Array.

    \returns True if this OdRxPropertyVariant object is an Array; false
    otherwise.
  */
  bool isArray() const;

  // Copy constructors.

  /** \details
    Copy constructor from an OdString value for instances of the
    OdRxPropertyVariant class.

    \param val [in] Constant reference to the OdString value.
  */
  OdRxPropertyVariant(const OdString& val);

  /** \details
    Copy constructor from an OdAnsiString value for instances of the
    OdRxPropertyVariant class.

    \param val [in] Constant reference to the OdAnsiString value.
  */
  OdRxPropertyVariant(const OdAnsiString& val);

  /** \details
    Copy constructor from a boolean value for instances of the
    OdRxPropertyVariant class.

    \param val [in] Constant boolean value.
  */
  OdRxPropertyVariant(const bool val);

  /** \details
    Copy constructor from an OdInt8 value for instances of the
    OdRxPropertyVariant class.

    \param val [in] Constant OdInt8 value.
  */
  OdRxPropertyVariant(const OdInt8 val);

  /** \details
    Copy constructor from an OdUInt8 value for instances of the
    OdRxPropertyVariant class.

    \param val [in] Constant OdUInt8 value.
  */
  OdRxPropertyVariant(const OdUInt8 val);

  /** \details
    Copy constructor from an OdInt16 value for instances of the
    OdRxPropertyVariant class.

    \param val [in] Constant OdInt16 value.
  */
  OdRxPropertyVariant(const OdInt16 val);

  /** \details
    Copy constructor from an OdUInt16 value for instances of the
    OdRxPropertyVariant class.

    \param val [in] Constant OdUInt16 value.
  */
  OdRxPropertyVariant(const OdUInt16 val);

  /** \details
    Copy constructor from an OdInt32 value for instances of the
    OdRxPropertyVariant class.

    \param val [in] Constant OdInt32 value.
  */
  OdRxPropertyVariant(const OdInt32 val);

  /** \details
    Copy constructor from an OdUInt32 value for instances of the
    OdRxPropertyVariant class.

    \param val [in] Constant OdUInt32 value.
  */
  OdRxPropertyVariant(const OdUInt32 val);

  /** \details
    Copy constructor from an OdInt64 value for instances of the
    OdRxPropertyVariant class.

    \param val [in] Constant OdInt64 value.
  */
  OdRxPropertyVariant(const OdInt64 val);

  /** \details
    Copy constructor from an OdUInt64 value for instances of the
    OdRxPropertyVariant class.

    \param val [in] Constant OdUInt64 value.
  */
  OdRxPropertyVariant(const OdUInt64 val);

  /** \details
    Copy constructor from a double value for instances of the
    OdRxPropertyVariant class.

    \param val [in] Constant reference to double value.
  */
  OdRxPropertyVariant(const double& val);

  /** \details
    Copy constructor from an OdGePoint3d value for instances of the
    OdRxPropertyVariant class.

    \param val [in] Constant reference to the OdGePoint3d value.
  */
  OdRxPropertyVariant(const OdGePoint3d& val);

  /** \details
    Copy constructor from an OdGePoint2d value for instances of the
    OdRxPropertyVariant class.

    \param val [in] Constant reference to the OdGePoint2d value.
  */
  OdRxPropertyVariant(const OdGePoint2d& val);

  /** \details
    Copy constructor from an OdGeVector2d value for instances of the
    OdRxPropertyVariant class.

    \param val [in] Constant reference to the OdGeVector2d value.
  */
  OdRxPropertyVariant(const OdGeVector2d& val);

  /** \details
    Copy constructor from an OdGeVector3d value for instances of the
    OdRxPropertyVariant class.

    \param val [in] Constant reference to the OdGeVector3d value.
  */
  OdRxPropertyVariant(const OdGeVector3d& val);

  /** \details
    Copy constructor from an OdCmEntityColor value for instances of the
    OdRxPropertyVariant class.

    \param val [in] Constant reference to the OdCmEntityColor value.
  */
  OdRxPropertyVariant(const OdCmEntityColor& val);

  /** \details
    Copy constructor from an OdCmTransparency value for instances of the
    OdRxPropertyVariant class.

    \param val [in] Constant reference to the OdCmTransparency value.
  */
  OdRxPropertyVariant(const OdCmTransparency& val);

  /** \details
    Copy constructor from an OdGeMatrix2d value for instances of the
    OdRxPropertyVariant class.

    \param val [in] Constant reference to the OdGeMatrix2d value.
  */
  OdRxPropertyVariant(const OdGeMatrix2d& val);

  /** \details
    Copy constructor from an OdGeMatrix3d value for instances of the
    OdRxPropertyVariant class.

    \param val [in] Constant reference to the OdGeMatrix3d value.
  */
  OdRxPropertyVariant(const OdGeMatrix3d& val);

  /** \details
    Copy constructor from an OdGeScale3d value for instances of the
    OdRxPropertyVariant class.

    \param val [in] Constant reference to the OdGeScale3d value.
  */
  OdRxPropertyVariant(const OdGeScale3d& val);

  /** \details
    Copy constructor from an OdTimeStamp value for instances of the
    OdRxPropertyVariant class.

    \param val [in] Constant reference to the OdTimeStamp value.
  */
  OdRxPropertyVariant(const OdTimeStamp& val);

  /** \details
    Copy constructor from an OdGeExtents2d value for instances of the
    OdRxPropertyVariant class.

    \param val [in] Constant reference to the OdGeExtents2d value.
  */
  OdRxPropertyVariant(const OdGeExtents2d& val);

  /** \details
    Copy constructor from an OdGeExtents3d value for instances of the
    OdRxPropertyVariant class.

    \param val [in] Constant reference to the OdGeExtents2d value.
  */
  OdRxPropertyVariant(const OdGeExtents3d& val);

  /** \details
    Copy constructor from an OdGeQuaternion value for instances of the
    OdRxPropertyVariant class.

    \param val [in] Constant reference to the OdGeQuaternion value.
  */
  OdRxPropertyVariant(const OdGeQuaternion& val);

  /** \details
    Copy constructor from an OdGiLightAttenuation value for instances of the
    OdRxPropertyVariant class.

    \param val [in] Constant reference to the OdGiLightAttenuation value.
  */
  OdRxPropertyVariant(const OdGiLightAttenuation& val);

  /** \details
    Copy constructor from an OdGiSkyParameters value for instances of the
    OdRxPropertyVariant class.

    \param val [in] Constant reference to the OdGiSkyParameters value.
  */
  OdRxPropertyVariant(const OdGiSkyParameters& val);

  /** \details
    Copy constructor from an OdGiShadowParameters value for instances of the
    OdRxPropertyVariant class.

    \param val [in] Constant reference to the OdGiShadowParameters value.
  */
  OdRxPropertyVariant(const OdGiShadowParameters& val);

  /** \details
    Copy constructor from an OdGiColorRGB value for instances of the
    OdRxPropertyVariant class.

    \param val [in] Constant reference to the OdGiColorRGB value.
  */
  OdRxPropertyVariant(const OdGiColorRGB& val);

  /** \details
    Copy constructor from an OdGiMrExposureType value for instances of the
    OdRxPropertyVariant class.

    \param val [in] Constant OdGiMrExposureType value.
  */
  OdRxPropertyVariant(const OdGiMrExposureType val);

  /** \details
    Copy constructor from an OdGiMrShadowMode value for instances of the
    OdRxPropertyVariant class.

    \param val [in] Constant OdGiMrShadowMode value.
  */
  OdRxPropertyVariant(const OdGiMrShadowMode val);

  /** \details
    Copy constructor from an OdGiMrFinalGatheringMode value for instances of the
    OdRxPropertyVariant class.

    \param val [in] Constant OdGiMrFinalGatheringMode value.
  */
  OdRxPropertyVariant(const OdGiMrFinalGatheringMode val);

  /** \details
    Copy constructor from an OdGiMrDiagnosticMode value for instances of the
    OdRxPropertyVariant class.

    \param val [in] Constant OdGiMrDiagnosticMode value.
  */
  OdRxPropertyVariant(const OdGiMrDiagnosticMode val);

  /** \details
    Copy constructor from an OdGiMrDiagnosticPhotonMode value for instances of
    the OdRxPropertyVariant class.

    \param val [in] Constant OdGiMrDiagnosticPhotonMode value.
  */
  OdRxPropertyVariant(const OdGiMrDiagnosticPhotonMode val);

  /** \details
    Copy constructor from an OdGiMrDiagnosticBSPMode value for instances of the
    OdRxPropertyVariant class.

    \param val [in] Constant OdGiMrDiagnosticBSPMode value.
  */
  OdRxPropertyVariant(const OdGiMrDiagnosticBSPMode val);

  /** \details
    Copy constructor from an OdGiMrExportMIMode value for instances of the
    OdRxPropertyVariant class.

   \param val [in] Constant OdGiMrExportMIMode value.
  */
  OdRxPropertyVariant(const OdGiMrExportMIMode val);

  /** \details
    Copy constructor from an OdGiMrTileOrder value for instances of the
    OdRxPropertyVariant class.

    \param val [in] Constant OdGiMrTileOrder value.
  */
  OdRxPropertyVariant(const OdGiMrTileOrder val);

  /** \details
    Copy constructor from an OdGiLightingMode value for instances of the
    OdRxPropertyVariant class.

    \param val [in] Constant OdGiLightingMode value.
  */
  OdRxPropertyVariant(const OdGiLightingMode val);

  /** \details
    Copy constructor from an OdGiFilterType value for instances of the
    OdRxPropertyVariant class.

    \param val [in] Constant OdGiFilterType value.
  */
  OdRxPropertyVariant(const OdGiFilterType val);

  /** \details
    Copy constructor from an OdGiMaterialColor value for instances of the
    OdRxPropertyVariant class.

    \param val [in] Constant reference to the OdGiMaterialColor value.
  */
  OdRxPropertyVariant(const OdGiMaterialColor& val);

  /** \details
    Copy constructor from an OdCmEntityColor::ColorMethod value for instances of
    the OdRxPropertyVariant class.

    \param val [in] Constant reference to the OdCmEntityColor::ColorMethod value.
  */
  OdRxPropertyVariant(const OdCmEntityColor::ColorMethod val);

  /** \details
    Copy constructor from an OdDb::LineWeight value for instances of the
    OdRxPropertyVariant class.

    \param val [in] Constant reference to the OdDb::LineWeight value.
  */
  OdRxPropertyVariant(const OdDb::LineWeight val);

  /** \details
    Copy constructor from an OdDb::UnitsValue value for instances of the
    OdRxPropertyVariant class.

    \param val [in] Constant reference to the OdDb::UnitsValue value.
  */
  OdRxPropertyVariant(const OdDb::UnitsValue val);

  /** \details
    Copy constructor from an OdGiDgLinetypeModifiers::WidthMode value for
    instances of the OdRxPropertyVariant class.

    \param val [in] Constant OdGiDgLinetypeModifiers::WidthMode value.
  */
  OdRxPropertyVariant(const OdGiDgLinetypeModifiers::WidthMode val);

  /** \details
    Copy constructor from an OdGiDgLinetypeModifiers::ShiftMode value for
    instances of the OdRxPropertyVariant class.

    \param val [in] Constant OdGiDgLinetypeModifiers::ShiftMode value.
  */
  OdRxPropertyVariant(const OdGiDgLinetypeModifiers::ShiftMode val);

  /** \details
    Copy constructor from an OdGiDgLinetypeModifiers::CornersMode value for
    instances of the OdRxPropertyVariant class.

    \param val [in] Constant OdGiDgLinetypeModifiers::CornersMode value.
  */
  OdRxPropertyVariant(const OdGiDgLinetypeModifiers::CornersMode val);

  /** \details
    Copy constructor from an OdGe::OdGeKnotParameterization value for instances
    of the OdRxPropertyVariant class.

    \param val [in] Constant OdGe::OdGeKnotParameterization value.
  */
  OdRxPropertyVariant(const OdGe::OdGeKnotParameterization val);

  /** \details
    Copy constructor from an OdGiLightAttenuation::AttenuationType value for
    instances of the OdRxPropertyVariant class.

    \param val [in] Constant OdGiLightAttenuation::AttenuationType value.
  */
  OdRxPropertyVariant(const OdGiLightAttenuation::AttenuationType val);

  /** \details
    Copy constructor from an OdGiDrawable::DrawableType value for instances of
    the OdRxPropertyVariant class.

    \param val [in] Constant OdGiDrawable::DrawableType value.
  */
  OdRxPropertyVariant(const OdGiDrawable::DrawableType val);

  /** \details
    Copy constructor from an OdGiShadowParameters::ShadowType value for
    instances of the OdRxPropertyVariant class.

    \param val [in] Constant OdGiShadowParameters::ShadowType value.
  */
  OdRxPropertyVariant(const OdGiShadowParameters::ShadowType val);

  /** \details
    Copy constructor from an OdGiShadowParameters::ExtendedLightShape value for
    instances of the OdRxPropertyVariant class.

    \param val [in] Constant OdGiShadowParameters::ExtendedLightShape value.
  */
  OdRxPropertyVariant(const OdGiShadowParameters::ExtendedLightShape val);

  /** \details
    Copy constructor from an OdGiViewportTraits::DefaultLightingType value for
    instances of the OdRxPropertyVariant class.

    \param val [in] Constant OdGiViewportTraits::DefaultLightingType value.
  */
  OdRxPropertyVariant(const OdGiViewportTraits::DefaultLightingType val);

  /** \details
    Copy constructor from an OdGiMaterialTraits::LuminanceMode value for
    instances of the OdRxPropertyVariant class.

    \param val [in] Constant OdGiMaterialTraits::LuminanceMode value.
  */
  OdRxPropertyVariant(const OdGiMaterialTraits::LuminanceMode val);

  /** \details
    Copy constructor from an OdGiMaterialTraits::GlobalIlluminationMode value
    for instances of the OdRxPropertyVariant class.

    \param val [in] Constant OdGiMaterialTraits::GlobalIlluminationMode value.
  */
  OdRxPropertyVariant(const OdGiMaterialTraits::GlobalIlluminationMode val);

  /** \details
    Copy constructor from an OdGiMaterialTraits::FinalGatherMode value for
    instances of the OdRxPropertyVariant class.

    \param val [in] Constant OdGiMaterialTraits::FinalGatherMode value.
  */
  OdRxPropertyVariant(const OdGiMaterialTraits::FinalGatherMode val);

  /** \details
    Copy constructor from an OdGiMaterialTraits::ChannelFlags value for
    instances of the OdRxPropertyVariant class.

    \param val [in] Constant OdGiMaterialTraits::ChannelFlags value.
  */
  OdRxPropertyVariant(const OdGiMaterialTraits::ChannelFlags val);

  /** \details
    Copy constructor from an OdGiMaterialTraits::Mode value for instances of the
    OdRxPropertyVariant class.

    \param val [in] Constant OdGiMaterialTraits::Mode value.
  */
  OdRxPropertyVariant(const OdGiMaterialTraits::Mode val);

  /** \details
    Copy constructor from an OdGiMaterialTraits::IlluminationModel value for
    instances of the OdRxPropertyVariant class.

    \param val [in] Constant OdGiMaterialTraits::IlluminationModel value.
  */
  OdRxPropertyVariant(const OdGiMaterialTraits::IlluminationModel val);

  /** \details
    Copy constructor from an OdGiVisualStyle::Type value for instances of the
    OdRxPropertyVariant class.

    \param val [in] Constant OdGiVisualStyle::Type value.
  */
  OdRxPropertyVariant(const OdGiVisualStyle::Type val);

  /** \details
    Copy constructor from an OdGiVisualStyleProperties::FaceColorMode value for
    instances of the OdRxPropertyVariant class.

    \param val [in] Constant OdGiVisualStyleProperties::FaceColorMode value.
  */
  OdRxPropertyVariant(const OdGiVisualStyleProperties::FaceColorMode val);

  /** \details
    Copy constructor from an OdGiVisualStyleProperties::EdgeModel value for
    instances of the OdRxPropertyVariant class.

    \param val [in] Constant OdGiVisualStyleProperties::EdgeModel value.
  */
  OdRxPropertyVariant(const OdGiVisualStyleProperties::EdgeModel val);

  /** \details
    Copy constructor from an OdGiFaceStyle::FaceColorMode value for instances of
    the OdRxPropertyVariant class.

    \param val [in] Constant OdGiFaceStyle::FaceColorMode value.
  */
  OdRxPropertyVariant(const OdGiFaceStyle::FaceColorMode val);

  /** \details
    Copy constructor from an OdGiEdgeStyle::EdgeModel value for instances of the
    OdRxPropertyVariant class.

    \param val [in] Constant OdGiEdgeStyle::EdgeModel value.
  */
  OdRxPropertyVariant(const OdGiEdgeStyle::EdgeModel val);

  /** \details
    Copy constructor from an OdGiToneOperatorParameters::ExteriorDaylightMode
    value for instances of the OdRxPropertyVariant class.

    \param val [in] Constant OdGiToneOperatorParameters::ExteriorDaylightMode value.
  */
  OdRxPropertyVariant(const OdGiToneOperatorParameters::ExteriorDaylightMode val);

  // Assignment operators.

  /** \details
    Assignment operator. Assigns a new value to this OdRxPropertyVariant object,
    replacing its current contents.

    \param val [in] New OdRxPropertyVariant value.

    \returns Reference to this object.
  */
  OdRxPropertyVariant& operator =(const OdRxPropertyVariant& val);

  /** \details
    Assignment operator. Assigns a new boolean value to this OdRxPropertyVariant
    object, replacing its current contents.

    \param val [in] New boolean value.

    \returns Reference to this object.
  */
  OdRxPropertyVariant& operator =(const bool& val);

  /** \details
    Assignment operator. Assigns a new double value to this OdRxPropertyVariant
    object, replacing its current contents.

    \param val [in] New double value.

    \returns Reference to this object.
  */
  OdRxPropertyVariant& operator =(const double& val);

  /** \details
    Assignment operator. Assigns a new OdInt8 value to this OdRxPropertyVariant
    object, replacing its current contents.

    \param val [in] New OdInt8 value.

    \returns Reference to this object.
  */
  OdRxPropertyVariant& operator =(const OdInt8& val);

  /** \details
    Assignment operator. Assigns a new OdUInt8 value to this OdRxPropertyVariant
    object, replacing its current contents.

    \param val [in] New OdUInt8 value.

    \returns Reference to this object.
  */
  OdRxPropertyVariant& operator =(const OdUInt8& val);

  /** \details
    Assignment operator. Assigns a new OdInt16 value to this OdRxPropertyVariant
    object, replacing its current contents.

    \param val [in] New OdInt16 value.

    \returns Reference to this object.
  */
  OdRxPropertyVariant& operator =(const OdInt16& val);

  /** \details
    Assignment operator. Assigns a new OdUInt16 value to this
    OdRxPropertyVariant object, replacing its current contents.

    \param val [in] New OdUInt16 value.

    \returns Reference to this object.
  */
  OdRxPropertyVariant& operator =(const OdUInt16& val);

  /** \details
    Assignment operator. Assigns a new OdInt32 value to this OdRxPropertyVariant
    object, replacing its current contents.

    \param val [in] New OdInt32 value.

    \returns Reference to this object.
  */
  OdRxPropertyVariant& operator =(const OdInt32& val);

  /** \details
    Assignment operator. Assigns a new OdUInt32 value to this
    OdRxPropertyVariant object, replacing its current contents.

    \param val [in] New OdUInt32 value.

    \returns Reference to this object.
  */
  OdRxPropertyVariant& operator =(const OdUInt32& val);

  /** \details
    Assignment operator. Assigns a new OdInt64 value to this OdRxPropertyVariant
    object, replacing its current contents.

    \param val [in] New OdInt64 value.

    \returns Reference to this object.
  */
  OdRxPropertyVariant& operator =(const OdInt64& val);

  /** \details
    Assignment operator. Assigns a new OdUInt64 value to this
    OdRxPropertyVariant object, replacing its current contents.

    \param val [in] New OdUInt64 value.

    \returns Reference to this object.
  */
  OdRxPropertyVariant& operator =(const OdUInt64& val);

  /** \details
    Assignment operator. Assigns a new OdString value to this
    OdRxPropertyVariant object, replacing its current contents.

    \param val [in] New OdString value.

    \returns Reference to this object.
  */
  OdRxPropertyVariant& operator =(const OdString& val);

  /** \details
    Assignment operator. Assigns a new OdAnsiString value to this
    OdRxPropertyVariant object, replacing its current contents.

    \param val [in] New OdAnsiString value.

    \returns Reference to this object.
  */
  OdRxPropertyVariant& operator =(const OdAnsiString& val);

  /** \details
    Assignment operator. Assigns a new OdGePoint2d value to this
    OdRxPropertyVariant object, replacing its current contents.

    \param val [in] New OdGePoint2d value.

    \returns Reference to this object.
  */
  OdRxPropertyVariant& operator =(const OdGePoint2d& val);

  /** \details
    Assignment operator. Assigns a new OdGePoint3d value to this
    OdRxPropertyVariant object, replacing its current contents.

    \param val [in] New OdGePoint3d value.

    \returns Reference to this object.
  */
  OdRxPropertyVariant& operator =(const OdGePoint3d& val);

  /** \details
    Assignment operator. Assigns a new OdGeVector2d value to this
    OdRxPropertyVariant object, replacing its current contents.

    \param val [in] New OdGeVector2d value.

    \returns Reference to this object.
  */
  OdRxPropertyVariant& operator =(const OdGeVector2d& val);

  /** \details
    Assignment operator. Assigns a new OdGeVector3d value to this
    OdRxPropertyVariant object, replacing its current contents.

    \param val [in] New OdGeVector3d value.

    \returns Reference to this object.
  */
  OdRxPropertyVariant& operator =(const OdGeVector3d& val);

  /** \details
    Assignment operator. Assigns a new OdCmEntityColor value to this
    OdRxPropertyVariant object, replacing its current contents.

    \param val [in] New OdCmEntityColor value.

    \returns Reference to this object.
  */
  OdRxPropertyVariant& operator =(const OdCmEntityColor& val);

  /** \details
    Assignment operator. Assigns a new OdCmTransparency value to this
    OdRxPropertyVariant object, replacing its current contents.

    \param val [in] New OdCmTransparency value.

    \returns Reference to this object.
  */
  OdRxPropertyVariant& operator =(const OdCmTransparency& val);

  /** \details
    Assignment operator. Assigns a new OdGeMatrix2d value to this
    OdRxPropertyVariant object, replacing its current contents.

    \param val [in] New OdGeMatrix2d value.

    \returns Reference to this object.
  */
  OdRxPropertyVariant& operator =(const OdGeMatrix2d& val);

  /** \details
    Assignment operator. Assigns a new OdGeMatrix3d value to this
    OdRxPropertyVariant object, replacing its current contents.

    \param val [in] New OdGeMatrix3d value.

    \returns Reference to this object.
  */
  OdRxPropertyVariant& operator =(const OdGeMatrix3d& val);

  /** \details
    Assignment operator. Assigns a new OdGeScale3d value to this
    OdRxPropertyVariant object, replacing its current contents.

    \param val [in] New OdGeScale3d value.

    \returns Reference to this object.
  */
  OdRxPropertyVariant& operator =(const OdGeScale3d& val);

  /** \details
    Assignment operator. Assigns a new OdTimeStamp value to this
    OdRxPropertyVariant object, replacing its current contents.

    \param val [in] New OdTimeStamp value.

    \returns Reference to this object.
  */
  OdRxPropertyVariant& operator =(const OdTimeStamp& val);

  /** \details
    Assignment operator. Assigns a new OdGeExtents2d value to this
    OdRxPropertyVariant object, replacing its current contents.

    \param val [in] New OdGeExtents2d value.

    \returns Reference to this object.
  */
  OdRxPropertyVariant& operator =(const OdGeExtents2d& val);

  /** \details
    Assignment operator. Assigns a new OdGeExtents3d value to this
    OdRxPropertyVariant object, replacing its current contents.

    \param val [in] New OdGeExtents3d value.

    \returns Reference to this object.
  */
  OdRxPropertyVariant& operator =(const OdGeExtents3d& val);

  /** \details
    Assignment operator. Assigns a new OdGeQuaternion value to this
    OdRxPropertyVariant object, replacing its current contents.

    \param val [in] New OdGeQuaternion value.

    \returns Reference to this object.
  */
  OdRxPropertyVariant& operator =(const OdGeQuaternion& val);

  /** \details
    Assignment operator. Assigns a new OdGiLightAttenuation value to this
    OdRxPropertyVariant object, replacing its current contents.

    \param val [in] New OdGiLightAttenuation value.

    \returns Reference to this object.
  */
  OdRxPropertyVariant& operator =(const OdGiLightAttenuation& val);

  /** \details
    Assignment operator. Assigns a new OdGiSkyParameters value to this
    OdRxPropertyVariant object, replacing its current contents.

    \param val [in] New OdGiSkyParameters value.

    \returns Reference to this object.
  */
  OdRxPropertyVariant& operator =(const OdGiSkyParameters& val);

  /** \details
    Assignment operator. Assigns a new OdGiShadowParameters value to this
    OdRxPropertyVariant object, replacing its current contents.

    \param val [in] New OdGiShadowParameters value.

    \returns Reference to this object.
  */
  OdRxPropertyVariant& operator =(const OdGiShadowParameters& val);

  /** \details
    Assignment operator. Assigns a new OdGiColorRGB value to this
    OdRxPropertyVariant object, replacing its current contents.

    \param val [in] New OdGiColorRGB value.

    \returns Reference to this object.
  */
  OdRxPropertyVariant& operator =(const OdGiColorRGB& val);

  /** \details
    Assignment operator. Assigns a new OdGiMrExposureType value to this
    OdRxPropertyVariant object, replacing its current contents.

    \param val [in] New OdGiMrExposureType value.

    \returns Reference to this object.
  */
  OdRxPropertyVariant& operator =(const OdGiMrExposureType& val);

  /** \details
    Assignment operator. Assigns a new OdGiMrShadowMode value to this
    OdRxPropertyVariant object, replacing its current contents.

    \param val [in] New OdGiMrShadowMode value.

    \returns Reference to this object.
  */
  OdRxPropertyVariant& operator =(const OdGiMrShadowMode& val);

  /** \details
    Assignment operator. Assigns a new OdGiMrFinalGatheringMode value to this
    OdRxPropertyVariant object, replacing its current contents.

    \param val [in] New OdGiMrFinalGatheringMode value.

    \returns Reference to this object.
  */
  OdRxPropertyVariant& operator =(const OdGiMrFinalGatheringMode& val);

  /** \details
    Assignment operator. Assigns a new OdGiMrDiagnosticMode value to this
    OdRxPropertyVariant object, replacing its current contents.

    \param val [in] New OdGiMrDiagnosticMode value.

    \returns Reference to this object.
  */
  OdRxPropertyVariant& operator =(const OdGiMrDiagnosticMode& val);

  /** \details
    Assignment operator. Assigns a new OdGiMrDiagnosticPhotonMode value to this
    OdRxPropertyVariant object, replacing its current contents.

    \param val [in] New OdGiMrDiagnosticPhotonMode value.

    \returns Reference to this object.
  */
  OdRxPropertyVariant& operator =(const OdGiMrDiagnosticPhotonMode& val);

  /** \details
    Assignment operator. Assigns a new OdGiMrDiagnosticBSPMode value to this
    OdRxPropertyVariant object, replacing its current contents.

    \param val [in] New OdGiMrDiagnosticBSPMode value.

    \returns Reference to this object.
  */
  OdRxPropertyVariant& operator =(const OdGiMrDiagnosticBSPMode& val);

  /** \details
    Assignment operator. Assigns a new OdGiMrExportMIMode value to this
    OdRxPropertyVariant object, replacing its current contents.

    \param val [in] New OdGiMrExportMIMode value.

    \returns Reference to this object.
  */
  OdRxPropertyVariant& operator =(const OdGiMrExportMIMode& val);

  /** \details
    Assignment operator. Assigns a new OdGiMrTileOrder value to this
    OdRxPropertyVariant object, replacing its current contents.

    \param val [in] New OdGiMrTileOrder value.

    \returns Reference to this object.
  */
  OdRxPropertyVariant& operator =(const OdGiMrTileOrder& val);

  /** \details
    Assignment operator. Assigns a new OdGiLightingMode value to this
    OdRxPropertyVariant object, replacing its current contents.

    \param val [in] New OdGiLightingMode value.

    \returns Reference to this object.
  */
  OdRxPropertyVariant& operator =(const OdGiLightingMode& val);

  /** \details
    Assignment operator. Assigns a new OdGiFilterType value to this
    OdRxPropertyVariant object, replacing its current contents.

    \param val [in] New OdGiFilterType value.

    \returns Reference to this object.
  */
  OdRxPropertyVariant& operator =(const OdGiFilterType& val);

  /** \details
    Assignment operator. Assigns a new OdGiMaterialColor value to this
    OdRxPropertyVariant object, replacing its current contents.

    \param val [in] New OdGiMaterialColor value.

    \returns Reference to this object.
  */
  OdRxPropertyVariant& operator =(const OdGiMaterialColor& val);

  /** \details
    Assignment operator. Assigns a new OdCmEntityColor::ColorMethod value to
    this OdRxPropertyVariant object, replacing its current contents.

    \param val [in] New OdCmEntityColor::ColorMethod value.

    \returns Reference to this object.
  */
  OdRxPropertyVariant& operator =(const OdCmEntityColor::ColorMethod& val);

  /** \details
    Assignment operator. Assigns a new OdDb::LineWeight value to this
    OdRxPropertyVariant object, replacing its current contents.

    \param val [in] New OdDb::LineWeight value.

    \returns Reference to this object.
  */
  OdRxPropertyVariant& operator =(const OdDb::LineWeight& val);

  /** \details
    Assignment operator. Assigns a new OdDb::UnitsValue value to this
    OdRxPropertyVariant object, replacing its current contents.

    \param val [in] New OdDb::UnitsValue value.

    \returns Reference to this object.
  */
  OdRxPropertyVariant& operator =(const OdDb::UnitsValue& val);

  /** \details
    Assignment operator. Assigns a new OdGiDgLinetypeModifiers::WidthMode value
    to this OdRxPropertyVariant object, replacing its current contents.

    \param val [in] New OdGiDgLinetypeModifiers::WidthMode value.

    \returns Reference to this object.
  */
  OdRxPropertyVariant& operator =(const OdGiDgLinetypeModifiers::WidthMode& val);

  /** \details
    Assignment operator. Assigns a new OdGiDgLinetypeModifiers::ShiftMode value
    to this OdRxPropertyVariant object, replacing its current contents.

    \param val [in] New OdGiDgLinetypeModifiers::ShiftMode value.

    \returns Reference to this object.
  */
  OdRxPropertyVariant& operator =(const OdGiDgLinetypeModifiers::ShiftMode& val);

  /** \details
    Assignment operator. Assigns a new OdGiDgLinetypeModifiers::CornersMode
    value to this OdRxPropertyVariant object, replacing its current contents.

    \param val [in] New OdGiDgLinetypeModifiers::CornersMode value.

    \returns Reference to this object.
  */
  OdRxPropertyVariant& operator =(const OdGiDgLinetypeModifiers::CornersMode& val);

  /** \details
    Assignment operator. Assigns a new OdGe::OdGeKnotParameterization value to
    this OdRxPropertyVariant object, replacing its current contents.

    \param val [in] New OdGe::OdGeKnotParameterization value.

    \returns Reference to this object.
  */
  OdRxPropertyVariant& operator =(const OdGe::OdGeKnotParameterization& val);

  /** \details
    Assignment operator. Assigns a new OdGiLightAttenuation::AttenuationType
    value to this OdRxPropertyVariant object, replacing its current contents.

    \param val [in] New OdGiLightAttenuation::AttenuationType value.

    \returns Reference to this object.
  */
  OdRxPropertyVariant& operator =(const OdGiLightAttenuation::AttenuationType& val);

  /** \details
    Assignment operator. Assigns a new OdGiDrawable::DrawableType value to this
    OdRxPropertyVariant object, replacing its current contents.

    \param val [in] New OdGiDrawable::DrawableType value.

    \returns Reference to this object.
  */
  OdRxPropertyVariant& operator =(const OdGiDrawable::DrawableType& val);

  /** \details
    Assignment operator. Assigns a new OdGiShadowParameters::ShadowType value to
    this OdRxPropertyVariant object, replacing its current contents.

    \param val [in] New OdGiShadowParameters::ShadowType value.

    \returns Reference to this object.
  */
  OdRxPropertyVariant& operator =(const OdGiShadowParameters::ShadowType& val);

  /** \details
    Assignment operator. Assigns a new OdGiShadowParameters::ExtendedLightShape
    value to this OdRxPropertyVariant object, replacing its current contents.

    \param val [in] New OdGiShadowParameters::ExtendedLightShape value.

    \returns Reference to this object.
  */
  OdRxPropertyVariant& operator =(const OdGiShadowParameters::ExtendedLightShape& val);

  /** \details
    Assignment operator. Assigns a new OdGiViewportTraits::DefaultLightingType
    value to this OdRxPropertyVariant object, replacing its current contents.

    \param val [in] New OdGiViewportTraits::DefaultLightingType value.

    \returns Reference to this object.
  */
  OdRxPropertyVariant& operator =(const OdGiViewportTraits::DefaultLightingType& val);

  /** \details
    Assignment operator. Assigns a new OdGiMaterialTraits::LuminanceMode value
    to this OdRxPropertyVariant object, replacing its current contents.

    \param val [in] New OdGiMaterialTraits::LuminanceMode value.

    \returns Reference to this object.
  */
  OdRxPropertyVariant& operator =(const OdGiMaterialTraits::LuminanceMode& val);

  /** \details
    Assignment operator. Assigns a new OdGiMaterialTraits::GlobalIlluminationMode
    value to this OdRxPropertyVariant object, replacing its current contents.

    \param val [in] New OdGiMaterialTraits::GlobalIlluminationMode value.

    \returns Reference to this object.
  */
  OdRxPropertyVariant& operator =(const OdGiMaterialTraits::GlobalIlluminationMode& val);

  /** \details
    Assignment operator. Assigns a new OdGiMaterialTraits::FinalGatherMode value
    to this OdRxPropertyVariant object, replacing its current contents.

    \param val [in] New OdGiMaterialTraits::FinalGatherMode value.

    \returns Reference to this object.
  */
  OdRxPropertyVariant& operator =(const OdGiMaterialTraits::FinalGatherMode& val);

  /** \details
    Assignment operator. Assigns a new OdGiMaterialTraits::ChannelFlags value to
    this OdRxPropertyVariant object, replacing its current contents.

    \param val [in] New OdGiMaterialTraits::ChannelFlags value.

    \returns Reference to this object.
  */
  OdRxPropertyVariant& operator =(const OdGiMaterialTraits::ChannelFlags& val);

  /** \details
    Assignment operator. Assigns a new OdGiMaterialTraits::Mode value to this
    OdRxPropertyVariant object, replacing its current contents.

    \param val [in] New OdGiMaterialTraits::Mode value.

    \returns Reference to this object.
  */
  OdRxPropertyVariant& operator =(const OdGiMaterialTraits::Mode& val);

  /** \details
    Assignment operator. Assigns a new OdGiMaterialTraits::IlluminationModel
    value to this OdRxPropertyVariant object, replacing its current contents.

    \param val [in] New OdGiMaterialTraits::IlluminationModel value.

    \returns Reference to this object.
  */
  OdRxPropertyVariant& operator =(const OdGiMaterialTraits::IlluminationModel& val);

  /** \details
    Assignment operator. Assigns a new OdGiVisualStyle::Type value to this
    OdRxPropertyVariant object, replacing its current contents.

    \param val [in] New OdGiVisualStyle::Type value.

    \returns Reference to this object.
  */
  OdRxPropertyVariant& operator =(const OdGiVisualStyle::Type& val);

  /** \details
    Assignment operator. Assigns a new OdGiVisualStyleProperties::FaceColorMode
    value to this OdRxPropertyVariant object, replacing its current contents.

    \param val [in] New OdGiVisualStyleProperties::FaceColorMode value.

    \returns Reference to this object.
  */
  OdRxPropertyVariant& operator =(const OdGiVisualStyleProperties::FaceColorMode& val);

  /** \details
    Assignment operator. Assigns a new OdGiVisualStyleProperties::EdgeModel
    value to this OdRxPropertyVariant object, replacing its current contents.

    \param val [in] New OdGiVisualStyleProperties::EdgeModel value.

    \returns Reference to this object.
  */
  OdRxPropertyVariant& operator =(const OdGiVisualStyleProperties::EdgeModel& val);

  /** \details
    Assignment operator. Assigns a new OdGiFaceStyle::FaceColorMode value to
    this OdRxPropertyVariant object, replacing its current contents.

    \param val [in] New OdGiFaceStyle::FaceColorMode value.

    \returns Reference to this object.
  */
  OdRxPropertyVariant& operator =(const OdGiFaceStyle::FaceColorMode& val);

  /** \details
    Assignment operator. Assigns a new OdGiEdgeStyle::EdgeModel value to this
    OdRxPropertyVariant object, replacing its current contents.

    \param val [in] New OdGiEdgeStyle::EdgeModel value.

    \returns Reference to this object.
  */
  OdRxPropertyVariant& operator =(const OdGiEdgeStyle::EdgeModel& val);

  /** \details
    Assignment operator. Assigns a new
    OdGiToneOperatorParameters::ExteriorDaylightMode value to this
    OdRxPropertyVariant object, replacing its current contents.

    \param val [in] New OdGiToneOperatorParameters::ExteriorDaylightMode value.

    \returns Reference to this object.
  */
  OdRxPropertyVariant& operator =(const OdGiToneOperatorParameters::ExteriorDaylightMode& val);

  /** \details
    Destroys an instance of the OdRxPropertyVariant class.
  */
  ~OdRxPropertyVariant();

  // Single-value getters.

  /** \details
    Gets the value of this OdRxPropertyVariant object as OdString.

    \returns Constant reference to the OdString value.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  const OdString& getString() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as OdAnsiString.

    \returns Constant reference to the OdAnsiString value.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  const OdAnsiString& getAnsiString() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as boolean.

    \returns Boolean value.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  bool getBool() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as OdInt8.

    \returns OdInt8 value.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  OdInt8 getInt8() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as OdUInt8.

    \returns OdUInt8 value.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  OdUInt8 getUInt8() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as OdInt16.

    \returns OdInt16 value.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  OdInt16 getInt16() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as OdUInt16.

    \returns OdUInt16 value.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  OdUInt16 getUInt16() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as OdInt32.

    \returns OdInt32 value.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  OdInt32 getInt32() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as OdUInt32.

    \returns OdUInt32 value.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  OdUInt32 getUInt32() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as OdInt64.

    \returns OdInt64 value.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  OdInt64 getInt64() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as OdUInt64.

    \returns OdUInt64 value.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  OdUInt64 getUInt64() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as double.

    \returns Constant reference to the double value.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  const double& getDouble() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as OdGePoint3d.

    \returns Constant reference to the OdGePoint3d object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  const OdGePoint3d& getGePoint3d() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as OdGePoint2d.

    \returns Constant reference to the OdGePoint2d object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  const OdGePoint2d& getGePoint2d() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as OdGeVector2d.

    \returns Constant reference to the OdGeVector2d object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  const OdGeVector2d& getGeVector2d() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as OdGeVector3d.

    \returns Constant reference to the OdGeVector3d object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  const OdGeVector3d& getGeVector3d() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as OdCmEntityColor.

    \returns Constant reference to the OdCmEntityColor object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  const OdCmEntityColor& getCmEntityColor() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as OdCmTransparency.

    \returns Constant reference to the OdCmTransparency object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  const OdCmTransparency& getCmTransparency() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as OdGeMatrix2d.

    \returns Constant reference to the OdGeMatrix2d object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  const OdGeMatrix2d& getGeMatrix2d() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as OdGeMatrix3d.

    \returns Constant reference to the OdGeMatrix3d object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  const OdGeMatrix3d& getGeMatrix3d() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as OdGeScale3d.

    \returns Constant reference to the OdGeScale3d object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  const OdGeScale3d& getGeScale3d() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as OdTimeStamp.

    \returns Constant reference to the OdTimeStamp object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  const OdTimeStamp& getTimeStamp() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as OdGeExtents2d.

    \returns Constant reference to the OdGeExtents2d object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  const OdGeExtents2d& getGeExtents2d() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as OdGeExtents3d.

    \returns Constant reference to the OdGeExtents3d object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  const OdGeExtents3d& getGeExtents3d() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as OdGeQuaternion.

    \returns Constant reference to the OdGeQuaternion object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  const OdGeQuaternion& getGeQuaternion() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as OdGiLightAttenuation.

    \returns Constant reference to the OdGiLightAttenuation object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  const OdGiLightAttenuation& getGiLightAttenuation() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as OdGiSkyParameters.

    \returns Constant reference to the OdGiSkyParameters object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  const OdGiSkyParameters& getGiSkyParameters() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as OdGiShadowParameters.

    \returns Constant reference to the OdGiShadowParameters object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  const OdGiShadowParameters& getGiShadowParameters() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as OdGiColorRGB.

    \returns Constant reference to the OdGiColorRGB object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  const OdGiColorRGB& getGiColorRGB() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as OdGiMaterialColor.

    \returns Constant reference to the OdGiMaterialColor object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  const OdGiMaterialColor& getGiMaterialColor() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as OdGiMrExposureType.

    \returns OdGiMrExposureType object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  OdGiMrExposureType getGiMrExposureType() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as OdGiMrShadowMode.

    \returns OdGiMrShadowMode object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  OdGiMrShadowMode getGiMrShadowMode() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as
    OdGiMrFinalGatheringMode.

    \returns OdGiMrFinalGatheringMode object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  OdGiMrFinalGatheringMode getGiMrFinalGatheringMode() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as OdGiMrDiagnosticMode.

    \returns OdGiMrDiagnosticMode object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  OdGiMrDiagnosticMode getGiMrDiagnosticMode() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as
    OdGiMrDiagnosticPhotonMode.

    \returns OdGiMrDiagnosticPhotonMode object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  OdGiMrDiagnosticPhotonMode getGiMrDiagnosticPhotonMode() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as
    OdGiMrDiagnosticBSPMode.

    \returns OdGiMrDiagnosticBSPMode object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  OdGiMrDiagnosticBSPMode getGiMrDiagnosticBSPMode() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as OdGiMrExportMIMode.

    \returns OdGiMrExportMIMode object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  OdGiMrExportMIMode getGiMrExportMIMode() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as OdGiMrTileOrder.

    \returns OdGiMrTileOrder object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  OdGiMrTileOrder getGiMrTileOrder() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as OdGiLightingMode.

    \returns OdGiLightingMode object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  OdGiLightingMode getGiLightingMode() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as OdGiFilterType.

    \returns OdGiFilterType object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  OdGiFilterType getGiFilterType() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as
    OdCmEntityColor::ColorMethod.

    \returns OdCmEntityColor::ColorMethod object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  OdCmEntityColor::ColorMethod getCmEntityColor_ColorMethod() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as OdDb::LineWeight.

    \returns OdDb::LineWeight object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  OdDb::LineWeight getDb_LineWeight() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as OdDb::UnitsValue.

    \returns OdDb::UnitsValue object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  OdDb::UnitsValue getDb_UnitsValue() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as
    OdGiDgLinetypeModifiers::WidthMode.

    \returns OdGiDgLinetypeModifiers::WidthMode object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  OdGiDgLinetypeModifiers::WidthMode getGiDgLinetypeModifiers_WidthMode() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as
    OdGiDgLinetypeModifiers::ShiftMode.

    \returns OdGiDgLinetypeModifiers::ShiftMode object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  OdGiDgLinetypeModifiers::ShiftMode getGiDgLinetypeModifiers_ShiftMode() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as
    OdGiDgLinetypeModifiers::CornersMode.

    \returns OdGiDgLinetypeModifiers::CornersMode object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  OdGiDgLinetypeModifiers::CornersMode getGiDgLinetypeModifiers_CornersMode() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as
    OdGe::OdGeKnotParameterization.

    \returns OdGe::OdGeKnotParameterization object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  OdGe::OdGeKnotParameterization getGe_OdGeKnotParameterization() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as
    OdGiLightAttenuation::AttenuationType.

    \returns OdGiLightAttenuation::AttenuationType object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  OdGiLightAttenuation::AttenuationType getGiLightAttenuation_AttenuationType() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as
    OdGiDrawable::DrawableType.

    \returns OdGiDrawable::DrawableType object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  OdGiDrawable::DrawableType getGiDrawable_DrawableType() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as
    OdGiShadowParameters::ShadowType.

    \returns OdGiShadowParameters::ShadowType object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  OdGiShadowParameters::ShadowType getGiShadowParameters_ShadowType() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as
    OdGiShadowParameters::ExtendedLightShape.

    \returns OdGiShadowParameters::ExtendedLightShape object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  OdGiShadowParameters::ExtendedLightShape getGiShadowParameters_ExtendedLightShape() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as
    OdGiViewportTraits::DefaultLightingType.

    \returns OdGiViewportTraits::DefaultLightingType object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  OdGiViewportTraits::DefaultLightingType getGiViewportTraits_DefaultLightingType() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as
    OdGiMaterialTraits::LuminanceMode.

    \returns OdGiMaterialTraits::LuminanceMode object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  OdGiMaterialTraits::LuminanceMode getGiMaterialTraits_LuminanceMode() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as
    OdGiMaterialTraits::GlobalIlluminationMode.

    \returns OdGiMaterialTraits::GlobalIlluminationMode object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  OdGiMaterialTraits::GlobalIlluminationMode getGiMaterialTraits_GlobalIlluminationMode() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as
    OdGiMaterialTraits::FinalGatherMode.

    \returns OdGiMaterialTraits::FinalGatherMode object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  OdGiMaterialTraits::FinalGatherMode getGiMaterialTraits_FinalGatherMode() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as
    OdGiMaterialTraits::ChannelFlags.

    \returns OdGiMaterialTraits::ChannelFlags object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  OdGiMaterialTraits::ChannelFlags getGiMaterialTraits_ChannelFlags() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as
    OdGiMaterialTraits::Mode.

    \returns OdGiMaterialTraits::Mode object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  OdGiMaterialTraits::Mode getGiMaterialTraits_Mode() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as
    OdGiMaterialTraits::IlluminationModel.

    \returns OdGiMaterialTraits::IlluminationModel object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  OdGiMaterialTraits::IlluminationModel getGiMaterialTraits_IlluminationModel() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as OdGiVisualStyle::Type.

    \returns OdGiVisualStyle::Type object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  OdGiVisualStyle::Type getGiVisualStyle_Type() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as
    OdGiVisualStyleProperties::FaceColorMode.

    \returns OdGiVisualStyleProperties::FaceColorMode object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  OdGiVisualStyleProperties::FaceColorMode getGiVisualStyleProperties_FaceColorMode() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as
    OdGiVisualStyleProperties::EdgeModel.

    \returns OdGiVisualStyleProperties::EdgeModel object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  OdGiVisualStyleProperties::EdgeModel getGiVisualStyleProperties_EdgeModel() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as
    OdGiFaceStyle::FaceColorMode.

    \returns OdGiFaceStyle::FaceColorMode object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  OdGiFaceStyle::FaceColorMode getGiFaceStyle_FaceColorMode() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as
    OdGiEdgeStyle::EdgeModel.

    \returns OdGiEdgeStyle::EdgeModel object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  OdGiEdgeStyle::EdgeModel getGiEdgeStyle_EdgeModel() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as
    OdGiToneOperatorParameters::ExteriorDaylightMode.

    \returns OdGiToneOperatorParameters::ExteriorDaylightMode object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  OdGiToneOperatorParameters::ExteriorDaylightMode getGiToneOperatorParameters_ExteriorDaylightMode() const;

  // Array-value getters.

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of OdString.

    \returns Constant reference to the OdStringArray object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  const OdStringArray& getStringArray() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdAnsiString.

    \returns Constant reference to the OdAnsiStringArray object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  const OdAnsiStringArray& getAnsiStringArray() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of boolean
    values.

    \returns Constant reference to the OdBoolArray object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  const OdBoolArray& getBoolArray() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of OdInt8
    values.

    \returns Constant reference to the OdInt8Array object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  const OdInt8Array& getInt8Array() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of OdUInt8
    values.

    \returns Constant reference to the OdUInt8Array object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  const OdUInt8Array& getUInt8Array() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of OdInt16
    values.

    \returns Constant reference to the OdInt16Array object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  const OdInt16Array& getInt16Array() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of OdUInt16
    values.

    \returns Constant reference to the OdUInt16Array object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  const OdUInt16Array& getUInt16Array() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of OdInt32
    values.

    \returns Constant reference to the OdInt32Array object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  const OdInt32Array& getInt32Array() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of OdUInt32
    values.

    \returns Constant reference to the OdUInt32Array object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  const OdUInt32Array& getUInt32Array() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of OdInt64
    values.

    \returns Constant reference to the OdInt64Array object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  const OdInt64Array& getInt64Array() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of OdUInt64
    values.

    \returns Constant reference to the OdUInt64Array object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  const OdUInt64Array& getUInt64Array() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of double
    values.

    \returns Constant reference to the OdDoubleArray object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  const OdDoubleArray& getDoubleArray() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of OdGePoint3d
    objects.

    \returns Constant reference to the OdGePoint3dArray object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  const OdGePoint3dArray& getGePoint3dArray() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of OdGePoint2d
    objects.

    \returns Constant reference to the OdGePoint2dArray object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  const OdGePoint2dArray& getGePoint2dArray() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdGeVector2d objects.

    \returns Constant reference to the OdGeVector2dArray object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  const OdGeVector2dArray& getGeVector2dArray() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdGeVector3d objects.

    \returns Constant reference to the OdGeVector3dArray object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  const OdGeVector3dArray& getGeVector3dArray() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdCmEntityColor objects.

    \returns Constant reference to the OdCmEntityColorArray object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  const OdCmEntityColorArray& getCmEntityColorArray() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdCmTransparency objects.

    \returns Constant reference to the OdCmTransparencyArray object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  const OdCmTransparencyArray& getCmTransparencyArray() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdGeMatrix2d objects.

    \returns Constant reference to the OdGeMatrix2dArray object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  const OdGeMatrix2dArray& getGeMatrix2dArray() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdGeMatrix3d objects.

    \returns Constant reference to the OdGeMatrix3dArray object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  const OdGeMatrix3dArray& getGeMatrix3dArray() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of OdGeScale3d
    objects.

    \returns Constant reference to the OdGeScale3dArray object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  const OdGeScale3dArray& getGeScale3dArray() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of OdTimeStamp
    objects.

    \returns Constant reference to the OdTimeStampArray object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  const OdTimeStampArray& getTimeStampArray() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdGeExtents2d objects.

    \returns Constant reference to the OdGeExtents2dArray object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  const OdGeExtents2dArray& getGeExtents2dArray() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdGeExtents3d objects.

    \returns Constant reference to the OdGeExtents3dArray object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  const OdGeExtents3dArray& getGeExtents3dArray() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdGeQuaternion objects.

    \returns Constant reference to the OdGeQuaternionArray object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  const OdGeQuaternionArray& getGeQuaternionArray() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdGiLightAttenuation objects.

    \returns Constant reference to the OdGiLightAttenuationArray object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  const OdGiLightAttenuationArray& getGiLightAttenuationArray() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdGiSkyParameters objects.

    \returns Constant reference to the OdGiSkyParametersArray object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  const OdGiSkyParametersArray& getGiSkyParametersArray() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdGiShadowParameters objects.

    \returns Constant reference to the OdGiShadowParametersArray object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  const OdGiShadowParametersArray& getGiShadowParametersArray() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdGiColorRGB objects.

    \returns Constant reference to the OdGiColorRGBArray object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  const OdGiColorRGBArray& getGiColorRGBArray() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdGiMrExposureType objects.

    \returns Constant reference to the OdGiMrExposureTypeArray object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  const OdGiMrExposureTypeArray& getGiMrExposureTypeArray() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdGiMrShadowMode objects.

    \returns Constant reference to the OdGiMrShadowModeArray object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  const OdGiMrShadowModeArray& getGiMrShadowModeArray() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdGiMrFinalGatheringMode objects.

    \returns Constant reference to the OdGiMrFinalGatheringModeArray object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  const OdGiMrFinalGatheringModeArray& getGiMrFinalGatheringModeArray() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdGiMrDiagnosticMode objects.

    \returns Constant reference to the OdGiMrDiagnosticModeArray object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  const OdGiMrDiagnosticModeArray& getGiMrDiagnosticModeArray() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdGiMrDiagnosticPhotonMode objects.

    \returns Constant reference to the OdGiMrDiagnosticPhotonModeArray object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  const OdGiMrDiagnosticPhotonModeArray& getGiMrDiagnosticPhotonModeArray() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdGiMrDiagnosticBSPMode objects.

    \returns Constant reference to the OdGiMrDiagnosticBSPModeArray object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  const OdGiMrDiagnosticBSPModeArray& getGiMrDiagnosticBSPModeArray() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdGiMrExportMIMode objects.

    \returns Constant reference to the OdGiMrExportMIModeArray object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  const OdGiMrExportMIModeArray& getGiMrExportMIModeArray() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdGiMrTileOrder objects.

    \returns Constant reference to the OdGiMrTileOrderArray object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  const OdGiMrTileOrderArray& getGiMrTileOrderArray() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdGiLightingMode objects.

    \returns Constant reference to the OdGiLightingModeArray object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  const OdGiLightingModeArray& getGiLightingModeArray() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdGiFilterType objects.

    \returns Constant reference to the OdGiFilterTypeArray object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  const OdGiFilterTypeArray& getGiFilterTypeArray() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdGiMaterialColor objects.

    \returns Constant reference to the OdGiMaterialColorArray object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  const OdGiMaterialColorArray& getGiMaterialColorArray() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdCmEntityColor_ColorMethod objects.

    \returns Constant reference to the OdCmEntityColor_ColorMethodArray object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  const OdCmEntityColor_ColorMethodArray& getCmEntityColor_ColorMethodArray() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdDb_LineWeight objects.

    \returns Constant reference to the OdDb_LineWeightArray object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  const OdDb_LineWeightArray& getDb_LineWeightArray() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdDb_UnitsValue objects.

    \returns Constant reference to the OdDb_UnitsValueArray object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  const OdDb_UnitsValueArray& getDb_UnitsValueArray() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdGiDgLinetypeModifiers_WidthMode objects.

    \returns Constant reference to the OdGiDgLinetypeModifiers_WidthModeArray
    object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  const OdGiDgLinetypeModifiers_WidthModeArray& getGiDgLinetypeModifiers_WidthModeArray() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdGiDgLinetypeModifiers_ShiftMode objects.

    \returns Constant reference to the OdGiDgLinetypeModifiers_ShiftModeArray
    object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  const OdGiDgLinetypeModifiers_ShiftModeArray& getGiDgLinetypeModifiers_ShiftModeArray() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdGiDgLinetypeModifiers_CornersMode objects.

    \returns Constant reference to the OdGiDgLinetypeModifiers_CornersModeArray
    object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  const OdGiDgLinetypeModifiers_CornersModeArray& getGiDgLinetypeModifiers_CornersModeArray() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdGe_OdGeKnotParameterization objects.

    \returns Constant reference to the OdGe_OdGeKnotParameterizationArray
    object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  const OdGe_OdGeKnotParameterizationArray& getGe_OdGeKnotParameterizationArray() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdGiLightAttenuation_AttenuationType objects.

    \returns Constant reference to the OdGiLightAttenuation_AttenuationTypeArray
    object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  const OdGiLightAttenuation_AttenuationTypeArray& getGiLightAttenuation_AttenuationTypeArray() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdGiDrawable_DrawableType objects.

    \returns Constant reference to the OdGiDrawable_DrawableTypeArray object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  const OdGiDrawable_DrawableTypeArray& getGiDrawable_DrawableTypeArray() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdGiShadowParameters_ShadowType objects.

    \returns Constant reference to the OdGiShadowParameters_ShadowTypeArray object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  const OdGiShadowParameters_ShadowTypeArray& getGiShadowParameters_ShadowTypeArray() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdGiShadowParameters_ExtendedLightShape objects.

    \returns Constant reference to the
    OdGiShadowParameters_ExtendedLightShapeArray object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  const OdGiShadowParameters_ExtendedLightShapeArray& getGiShadowParameters_ExtendedLightShapeArray() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdGiViewportTraits_DefaultLightingType objects.

    \returns Constant reference to the
    OdGiViewportTraits_DefaultLightingTypeArray object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  const OdGiViewportTraits_DefaultLightingTypeArray& getGiViewportTraits_DefaultLightingTypeArray() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdGiMaterialTraits_LuminanceMode objects.

    \returns Constant reference to the
    OdGiMaterialTraits_LuminanceModeArray object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  const OdGiMaterialTraits_LuminanceModeArray& getGiMaterialTraits_LuminanceModeArray() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdGiMaterialTraits_GlobalIlluminationMode objects.

    \returns Constant reference to the
    OdGiMaterialTraits_GlobalIlluminationModeArray object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  const OdGiMaterialTraits_GlobalIlluminationModeArray& getGiMaterialTraits_GlobalIlluminationModeArray() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdGiMaterialTraits_FinalGatherMode objects.

    \returns Constant reference to the
    OdGiMaterialTraits_FinalGatherModeArray object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  const OdGiMaterialTraits_FinalGatherModeArray& getGiMaterialTraits_FinalGatherModeArray() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdGiMaterialTraits_ChannelFlags objects.

    \returns Constant reference to the
    OdGiMaterialTraits_ChannelFlagsArray object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  const OdGiMaterialTraits_ChannelFlagsArray& getGiMaterialTraits_ChannelFlagsArray() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdGiMaterialTraits_Mode objects.

    \returns Constant reference to the OdGiMaterialTraits_ModeArray object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  const OdGiMaterialTraits_ModeArray& getGiMaterialTraits_ModeArray() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdGiMaterialTraits_IlluminationModel objects.

    \returns Constant reference to the OdGiMaterialTraits_IlluminationModelArray
    object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  const OdGiMaterialTraits_IlluminationModelArray& getGiMaterialTraits_IlluminationModelArray() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdGiVisualStyle_Type objects.

    \returns Constant reference to the OdGiVisualStyle_TypeArray object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  const OdGiVisualStyle_TypeArray& getGiVisualStyle_TypeArray() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdGiVisualStyleProperties_FaceColorMode objects.

    \returns Constant reference to the
    OdGiVisualStyleProperties_FaceColorModeArray object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  const OdGiVisualStyleProperties_FaceColorModeArray& getGiVisualStyleProperties_FaceColorModeArray() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdGiVisualStyleProperties_EdgeModel objects.

    \returns Constant reference to the OdGiVisualStyleProperties_EdgeModelArray
    object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  const OdGiVisualStyleProperties_EdgeModelArray& getGiVisualStyleProperties_EdgeModelArray() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdGiFaceStyle_FaceColorMode objects.

    \returns Constant reference to the OdGiFaceStyle_FaceColorModeArray object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  const OdGiFaceStyle_FaceColorModeArray& getGiFaceStyle_FaceColorModeArray() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdGiEdgeStyle_EdgeModel objects.

    \returns Constant reference to the OdGiEdgeStyle_EdgeModelArray object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  const OdGiEdgeStyle_EdgeModelArray& getGiEdgeStyle_EdgeModelArray() const;

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdGiToneOperatorParameters_ExteriorDaylightMode objects.

    \returns Constant reference to the
    OdGiToneOperatorParameters_ExteriorDaylightModeArray object.

    \remarks
    No check is made to determine that this OdRxPropertyVariant object is of the
    specified type.
  */
  const OdGiToneOperatorParameters_ExteriorDaylightModeArray& getGiToneOperatorParameters_ExteriorDaylightModeArray() const;

  // Array-value cast methods.

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of OdString.

    \returns Reference to the OdStringArray object.
  */
  OdStringArray& asStringArray();

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdAnsiString.

    \returns Reference to the OdAnsiStringArray object.
  */
  OdAnsiStringArray& asAnsiStringArray();

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of boolean
    values.

    \returns Reference to the OdBoolArray object.
  */
  OdBoolArray& asBoolArray();

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of OdInt8
    values.

    \returns Reference to the OdInt8Array object.
  */
  OdInt8Array& asInt8Array();

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of OdUInt8
    values.

    \returns Reference to the OdUInt8Array object.
  */
  OdUInt8Array& asUInt8Array();

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of OdInt16
    values.

    \returns Reference to the OdInt16Array object.
  */
  OdInt16Array& asInt16Array();

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of OdUInt16
    values.

    \returns Reference to the OdUInt16Array object.
  */
  OdUInt16Array& asUInt16Array();

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of OdInt32
    values.

    \returns Reference to the OdInt32Array object.
  */
  OdInt32Array& asInt32Array();

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of OdUInt32
    values.

    \returns Reference to the OdUInt32Array object.
  */
  OdUInt32Array& asUInt32Array();

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of OdInt64
    values.

    \returns Reference to the OdInt64Array object.
  */
  OdInt64Array& asInt64Array();

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of OdUInt64
    values.

    \returns Reference to the OdUInt64Array object.
  */
  OdUInt64Array& asUInt64Array();

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of OdDouble
    values.

    \returns Reference to the OdDoubleArray object.
  */
  OdDoubleArray& asDoubleArray();

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of OdGePoint3d
    objects.

    \returns Reference to the OdGePoint3dArray object.
  */
  OdGePoint3dArray& asGePoint3dArray();

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of OdGePoint2d
    objects.

    \returns Reference to the OdGePoint2dArray object.
  */
  OdGePoint2dArray& asGePoint2dArray();

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdGeVector2d objects.

    \returns Reference to the OdGeVector2dArray object.
  */
  OdGeVector2dArray& asGeVector2dArray();

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdGeVector3d objects.

    \returns Reference to the OdGeVector3dArray object.
  */
  OdGeVector3dArray& asGeVector3dArray();

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdCmEntityColor objects.

    \returns Reference to the OdCmEntityColorArray object.
  */
  OdCmEntityColorArray&  asCmEntityColorArray();

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdCmTransparency objects.

    \returns Reference to the OdCmTransparencyArray object.
  */
  OdCmTransparencyArray& asCmTransparencyArray();

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdGeMatrix2d objects.

    \returns Reference to the OdGeMatrix2dArray object.
  */
  OdGeMatrix2dArray& asGeMatrix2dArray();

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdGeMatrix3d objects.

    \returns Reference to the OdGeMatrix3dArray object.
  */
  OdGeMatrix3dArray& asGeMatrix3dArray();

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of OdGeScale3d
    objects.

    \returns Reference to the OdGeScale3dArray object.
  */
  OdGeScale3dArray& asGeScale3dArray();

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of OdTimeStamp
    objects.

    \returns Reference to the OdTimeStampArray object.
  */
  OdTimeStampArray& asTimeStampArray();

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdGeExtents2d objects.

    \returns Reference to the OdGeExtents2dArray object.
  */
  OdGeExtents2dArray& asGeExtents2dArray();

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdGeExtents3d objects.

    \returns Reference to the OdGeExtents3dArray object.
  */
  OdGeExtents3dArray& asGeExtents3dArray();

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdGeQuaternion objects.

    \returns Reference to the OdGeQuaternionArray object.
  */
  OdGeQuaternionArray& asGeQuaternionArray();

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdGiLightAttenuation objects.

    \returns Reference to the OdGiLightAttenuationArray object.
  */
  OdGiLightAttenuationArray& asGiLightAttenuationArray();

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdGiSkyParameters objects.

    \returns Reference to the OdGiSkyParametersArray object.
  */
  OdGiSkyParametersArray& asGiSkyParametersArray();

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdGiShadowParameters objects.

    \returns Reference to the OdGiShadowParametersArray object.
  */
  OdGiShadowParametersArray& asGiShadowParametersArray();

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdGiColorRGB objects.

    \returns Reference to the OdGiColorRGBArray object.
  */
  OdGiColorRGBArray& asGiColorRGBArray();

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdGiMrExposureType objects.

    \returns Reference to the OdGiMrExposureTypeArray object.
  */
  OdGiMrExposureTypeArray& asGiMrExposureTypeArray();

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdGiMrShadowMode objects.

    \returns Reference to the OdGiMrShadowModeArray object.
  */
  OdGiMrShadowModeArray& asGiMrShadowModeArray();

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdGiMrFinalGatheringMode objects.

    \returns Reference to the OdGiMrFinalGatheringModeArray object.
  */
  OdGiMrFinalGatheringModeArray& asGiMrFinalGatheringModeArray();

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdGiMrDiagnosticMode objects.

    \returns Reference to the OdGiMrDiagnosticModeArray object.
  */
  OdGiMrDiagnosticModeArray& asGiMrDiagnosticModeArray();

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdGiMrDiagnosticPhotonMode objects.

    \returns Reference to the OdGiMrDiagnosticPhotonModeArray object.
  */
  OdGiMrDiagnosticPhotonModeArray& asGiMrDiagnosticPhotonModeArray();

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdGiMrDiagnosticBSPMode objects.

    \returns Reference to the OdGiMrDiagnosticBSPModeArray object.
  */
  OdGiMrDiagnosticBSPModeArray& asGiMrDiagnosticBSPModeArray();

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdGiMrExportMIMode objects.

    \returns Reference to the OdGiMrExportMIModeArray object.
  */
  OdGiMrExportMIModeArray& asGiMrExportMIModeArray();

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdGiMrTileOrder objects.

    \returns Reference to the OdGiMrTileOrderArray object.
  */
  OdGiMrTileOrderArray& asGiMrTileOrderArray();

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdGiLightingMode objects.

    \returns Reference to the OdGiLightingModeArray object.
  */
  OdGiLightingModeArray& asGiLightingModeArray();

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdGiFilterType objects.

    \returns Reference to the OdGiFilterTypeArray object.
  */
  OdGiFilterTypeArray& asGiFilterTypeArray();

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdGiMaterialColor objects.

    \returns Reference to the OdGiMaterialColorArray object.
  */
  OdGiMaterialColorArray& asGiMaterialColorArray();

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdCmEntityColor_ColorMethod objects.

    \returns Reference to the OdCmEntityColor_ColorMethodArray object.
  */
  OdCmEntityColor_ColorMethodArray& asCmEntityColor_ColorMethodArray();

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdDb_LineWeight objects.

    \returns Reference to the OdDb_LineWeightArray object.
  */
  OdDb_LineWeightArray& asDb_LineWeightArray();

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdDb_UnitsValue objects.

    \returns Reference to the OdDb_UnitsValueArray object.
  */
  OdDb_UnitsValueArray& asDb_UnitsValueArray();

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdGiDgLinetypeModifiers_WidthMode objects.

    \returns Reference to the OdGiDgLinetypeModifiers_WidthModeArray object.
  */
  OdGiDgLinetypeModifiers_WidthModeArray& asGiDgLinetypeModifiers_WidthModeArray();

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdGiDgLinetypeModifiers_ShiftMode objects.

    \returns Reference to the OdGiDgLinetypeModifiers_ShiftModeArray object.
  */
  OdGiDgLinetypeModifiers_ShiftModeArray& asGiDgLinetypeModifiers_ShiftModeArray();

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdGiDgLinetypeModifiers_CornersMode objects.

    \returns Reference to the OdGiDgLinetypeModifiers_CornersModeArray object.
  */
  OdGiDgLinetypeModifiers_CornersModeArray& asGiDgLinetypeModifiers_CornersModeArray();

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdGe_OdGeKnotParameterization objects.

    \returns Reference to the OdGe_OdGeKnotParameterizationArray object.
  */
  OdGe_OdGeKnotParameterizationArray& asGe_OdGeKnotParameterizationArray();

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdGiLightAttenuation_AttenuationType objects.

    \returns Reference to the OdGiLightAttenuation_AttenuationTypeArray object.
  */
  OdGiLightAttenuation_AttenuationTypeArray& asGiLightAttenuation_AttenuationTypeArray();

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdGiDrawable_DrawableType objects.

    \returns Reference to the OdGiDrawable_DrawableTypeArray object.
  */
  OdGiDrawable_DrawableTypeArray& asGiDrawable_DrawableTypeArray();

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdGiShadowParameters_ShadowType objects.

    \returns Reference to the OdGiShadowParameters_ShadowTypeArray object.
  */
  OdGiShadowParameters_ShadowTypeArray& asGiShadowParameters_ShadowTypeArray();

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdGiShadowParameters_ExtendedLightShape objects.

    \returns Reference to the OdGiShadowParameters_ExtendedLightShapeArray
    object.
  */
  OdGiShadowParameters_ExtendedLightShapeArray& asGiShadowParameters_ExtendedLightShapeArray();

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdGiViewportTraits_DefaultLightingType objects.

    \returns Reference to the OdGiViewportTraits_DefaultLightingTypeArray
    object.
  */
  OdGiViewportTraits_DefaultLightingTypeArray& asGiViewportTraits_DefaultLightingTypeArray();

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdGiMaterialTraits_LuminanceMode objects.

    \returns Reference to the OdGiMaterialTraits_LuminanceModeArray object.
  */
  OdGiMaterialTraits_LuminanceModeArray& asGiMaterialTraits_LuminanceModeArray();

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdGiMaterialTraits_GlobalIlluminationMode objects.

    \returns Reference to the OdGiMaterialTraits_GlobalIlluminationModeArray
    object.
  */
  OdGiMaterialTraits_GlobalIlluminationModeArray& asGiMaterialTraits_GlobalIlluminationModeArray();

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdGiMaterialTraits_FinalGatherMode objects.

    \returns Reference to the OdGiMaterialTraits_FinalGatherModeArray object.
  */
  OdGiMaterialTraits_FinalGatherModeArray& asGiMaterialTraits_FinalGatherModeArray();

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdGiMaterialTraits_ChannelFlags objects.

    \returns Reference to the OdGiMaterialTraits_ChannelFlagsArray object.
  */
  OdGiMaterialTraits_ChannelFlagsArray& asGiMaterialTraits_ChannelFlagsArray();

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdGiMaterialTraits_Mode objects.

    \returns Reference to the OdGiMaterialTraits_ModeArray object.
  */
  OdGiMaterialTraits_ModeArray& asGiMaterialTraits_ModeArray();

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdGiMaterialTraits_IlluminationModel objects.

    \returns Reference to the OdGiMaterialTraits_IlluminationModelArray object.
  */
  OdGiMaterialTraits_IlluminationModelArray& asGiMaterialTraits_IlluminationModelArray();

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdGiVisualStyle_Type objects.

    \returns Reference to the OdGiVisualStyle_TypeArray object.
  */
  OdGiVisualStyle_TypeArray& asGiVisualStyle_TypeArray();

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdGiVisualStyleProperties_FaceColorMode objects.

    \returns Reference to the OdGiVisualStyleProperties_FaceColorModeArray
    object.
  */
  OdGiVisualStyleProperties_FaceColorModeArray& asGiVisualStyleProperties_FaceColorModeArray();

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdGiVisualStyleProperties_EdgeModel objects.

    \returns Reference to the OdGiVisualStyleProperties_EdgeModelArray object.
  */
  OdGiVisualStyleProperties_EdgeModelArray& asGiVisualStyleProperties_EdgeModelArray();

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdGiFaceStyle_FaceColorMode objects.

    \returns Reference to the OdGiFaceStyle_FaceColorModeArray object.
  */
  OdGiFaceStyle_FaceColorModeArray& asGiFaceStyle_FaceColorModeArray();

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdGiEdgeStyle_EdgeModel objects.

    \returns Reference to the OdGiEdgeStyle_EdgeModelArray object.
  */
  OdGiEdgeStyle_EdgeModelArray& asGiEdgeStyle_EdgeModelArray();

  /** \details
    Gets the value of this OdRxPropertyVariant object as an array of
    OdGiToneOperatorParameters_ExteriorDaylightMode objects.

    \returns Reference to the
    OdGiToneOperatorParameters_ExteriorDaylightModeArray object.
  */
  OdGiToneOperatorParameters_ExteriorDaylightModeArray& asGiToneOperatorParameters_ExteriorDaylightModeArray();

  // Single-value setters.

  /** \details
    Sets the value of this OdRxPropertyVariant object from an OdString object.

    \param val [in] Constant reference to OdString.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setString(const OdString& val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an OdAnsiString object.

    \param val [in] Constant reference to OdAnsiString.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setAnsiString(const OdAnsiString& val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from a boolean value.

    \param val [in] Constant boolean value.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setBool(const bool val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an OdInt8 object.

    \param val [in] Constant OdInt8 object.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setInt8(const OdInt8 val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an OdUInt8 object.

    \param val [in] Constant OdUInt8 object.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setUInt8(const OdUInt8 val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an OdInt16 object.

    \param val [in] Constant OdInt16 object.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setInt16(const OdInt16 val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an OdUInt16 object.

    \param val [in] Constant OdUInt16 object.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setUInt16(const OdUInt16 val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an OdInt32 object.

    \param val [in] Constant OdInt32 object.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setInt32(const OdInt32 val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an OdUInt32 object.

    \param val [in] Constant OdUInt32.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setUInt32(const OdUInt32 val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an OdInt64 object.

    \param val [in] Constant OdInt64 object.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setInt64(const OdInt64 val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an OdUInt64 object.

    \param val [in] Constant OdUInt64 object.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setUInt64(const OdUInt64 val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from a double value.

    \param val [in] Constant reference to a double value.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setDouble(const double& val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an OdGePoint3d
    object.

    \param val [in] Constant reference to the OdGePoint3d object.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setGePoint3d(const OdGePoint3d& val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an OdGePoint2d
    object.

    \param val [in] Constant reference to the OdGePoint2d object.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setGePoint2d(const OdGePoint2d& val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an OdGeVector2d
    object.

    \param val [in] Constant reference to the OdGeVector2d object.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setGeVector2d(const OdGeVector2d& val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an OdGeVector3d
    object.

    \param val [in] Constant reference to the OdGeVector3d object.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setGeVector3d(const OdGeVector3d& val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an OdCmEntityColor
    object.

    \param val [in] Constant reference to the OdCmEntityColor object.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setCmEntityColor(const OdCmEntityColor& val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an OdCmTransparency
    object.

    \param val [in] Constant reference to the OdCmTransparency object.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setCmTransparency(const OdCmTransparency& val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an OdGeMatrix2d
    object.

    \param val [in] Constant reference to the OdGeMatrix2d object.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setGeMatrix2d(const OdGeMatrix2d& val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an OdGeMatrix3d
    object.

    \param val [in] Constant reference to the OdGeMatrix3d object.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setGeMatrix3d(const OdGeMatrix3d& val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an OdGeScale3d
    object.

    \param val [in] Constant reference to the OdGeScale3d object.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setGeScale3d(const OdGeScale3d& val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an OdTimeStamp
    object.

    \param val [in] Constant reference to the OdTimeStamp object.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setTimeStamp(const OdTimeStamp& val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an OdGeExtents2d
    object.

    \param val [in] Constant reference to the OdGeExtents2d object.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setGeExtents2d(const OdGeExtents2d& val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an OdGeExtents3d
    object.

    \param val [in] Constant reference to the OdGeExtents3d object.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setGeExtents3d(const OdGeExtents3d& val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an OdGeQuaternion
    object.

    \param val [in] Constant reference to the OdGeQuaternion object.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setGeQuaternion(const OdGeQuaternion& val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an
    OdGiLightAttenuation object.

    \param val [in] Constant reference to the OdGiLightAttenuation object.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setGiLightAttenuation(const OdGiLightAttenuation& val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an OdGiSkyParameters
    object.

    \param val [in] Constant reference to the OdGiSkyParameters object.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setGiSkyParameters(const OdGiSkyParameters& val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an
    OdGiShadowParameters object.

    \param val [in] Constant reference to the OdGiShadowParameters object.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setGiShadowParameters(const OdGiShadowParameters& val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an OdGiColorRGB
    object.

    \param val [in] Constant reference to the OdGiColorRGB object.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setGiColorRGB(const OdGiColorRGB& val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an OdGiMaterialColor
    object.

    \param val [in] Constant reference to the OdGiMaterialColor object.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setGiMaterialColor(const OdGiMaterialColor& val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an OdGiMrExposureType
    object.

    \param val [in] Constant OdGiMrExposureType object.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setGiMrExposureType(const OdGiMrExposureType val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an OdGiMrShadowMode
    object.

    \param val [in] Constant OdGiMrShadowMode object.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setGiMrShadowMode(const OdGiMrShadowMode val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an
    OdGiMrFinalGatheringMode object.

    \param val [in] Constant OdGiMrFinalGatheringMode object.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setGiMrFinalGatheringMode(const OdGiMrFinalGatheringMode val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an
    OdGiMrDiagnosticMode object.

    \param val [in] Constant OdGiMrDiagnosticMode object.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setGiMrDiagnosticMode(const OdGiMrDiagnosticMode val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an
    OdGiMrDiagnosticPhotonMode object.

    \param val [in] Constant OdGiMrDiagnosticPhotonMode object.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setGiMrDiagnosticPhotonMode(const OdGiMrDiagnosticPhotonMode val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an
    OdGiMrDiagnosticBSPMode object.

    \param val [in] Constant OdGiMrDiagnosticBSPMode object.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setGiMrDiagnosticBSPMode(const OdGiMrDiagnosticBSPMode val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an OdGiMrExportMIMode
    object.

    \param val [in] Constant OdGiMrExportMIMode object.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setGiMrExportMIMode(const OdGiMrExportMIMode val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an OdGiMrTileOrder
    object.

    \param val [in] Constant OdGiMrTileOrder object.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setGiMrTileOrder(const OdGiMrTileOrder val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an OdGiLightingMode
    object.

    \param val [in] Constant OdGiLightingMode object.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setGiLightingMode(const OdGiLightingMode val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an OdGiFilterType
    object.

    \param val [in] Constant OdGiFilterType object.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setGiFilterType(const OdGiFilterType val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an
    OdCmEntityColor::ColorMethod object.

    \param val [in] Constant OdCmEntityColor::ColorMethod object.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setCmEntityColor_ColorMethod(const OdCmEntityColor::ColorMethod val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an OdDb::LineWeight
    object.

    \param val [in] Constant OdDb::LineWeight object.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setDb_LineWeight(const OdDb::LineWeight val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an OdDb::UnitsValue
    object.

    \param val [in] Constant OdDb::UnitsValue object.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setDb_UnitsValue(const OdDb::UnitsValue val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an
    OdGiDgLinetypeModifiers::WidthMode object.

    \param val [in] Constant OdGiDgLinetypeModifiers::WidthMode object.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setGiDgLinetypeModifiers_WidthMode(const OdGiDgLinetypeModifiers::WidthMode val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an
    OdGiDgLinetypeModifiers::ShiftMode object.

    \param val [in] Constant OdGiDgLinetypeModifiers::ShiftMode object.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setGiDgLinetypeModifiers_ShiftMode(const OdGiDgLinetypeModifiers::ShiftMode val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an
    OdGiDgLinetypeModifiers::CornersMode object.

    \param val [in] Constant OdGiDgLinetypeModifiers::CornersMode object.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setGiDgLinetypeModifiers_CornersMode(const OdGiDgLinetypeModifiers::CornersMode val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an
    OdGe::OdGeKnotParameterization object.

    \param val [in] Constant OdGe::OdGeKnotParameterization object.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setGe_OdGeKnotParameterization(const OdGe::OdGeKnotParameterization val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an
    OdGiLightAttenuation::AttenuationType object.

    \param val [in] Constant OdGiLightAttenuation::AttenuationType object.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setGiLightAttenuation_AttenuationType(const OdGiLightAttenuation::AttenuationType val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an
    OdGiDrawable::DrawableType object.

    \param val [in] Constant OdGiDrawable::DrawableType object.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setGiDrawable_DrawableType(const OdGiDrawable::DrawableType val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an
    OdGiShadowParameters::ShadowType object.

    \param val [in] Constant OdGiShadowParameters::ShadowType object.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setGiShadowParameters_ShadowType(const OdGiShadowParameters::ShadowType val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an
    OdGiShadowParameters::ExtendedLightShape object.

    \param val [in] Constant OdGiShadowParameters::ExtendedLightShape object.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setGiShadowParameters_ExtendedLightShape(const OdGiShadowParameters::ExtendedLightShape val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an
    OdGiViewportTraits::DefaultLightingType object.

    \param val [in] Constant OdGiViewportTraits::DefaultLightingType object.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setGiViewportTraits_DefaultLightingType(const OdGiViewportTraits::DefaultLightingType val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an
    OdGiMaterialTraits::LuminanceMode object.

    \param val [in] Constant OdGiMaterialTraits::LuminanceMode object.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setGiMaterialTraits_LuminanceMode(const OdGiMaterialTraits::LuminanceMode val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an
    OdGiMaterialTraits::GlobalIlluminationMode object.

    \param val [in] Constant OdGiMaterialTraits::GlobalIlluminationMode object.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setGiMaterialTraits_GlobalIlluminationMode(const OdGiMaterialTraits::GlobalIlluminationMode val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an
    OdGiMaterialTraits::FinalGatherMode object.

    \param val [in] Constant OdGiMaterialTraits::FinalGatherMode object.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setGiMaterialTraits_FinalGatherMode(const OdGiMaterialTraits::FinalGatherMode val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an
    OdGiMaterialTraits::ChannelFlags object.

    \param val [in] Constant OdGiMaterialTraits::ChannelFlags object.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setGiMaterialTraits_ChannelFlags(const OdGiMaterialTraits::ChannelFlags val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an
    OdGiMaterialTraits::Mode object.

    \param val [in] Constant OdGiMaterialTraits::Mode object.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setGiMaterialTraits_Mode(const OdGiMaterialTraits::Mode val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an
    OdGiMaterialTraits::IlluminationModel object.

    \param val [in] Constant OdGiMaterialTraits::IlluminationModel object.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setGiMaterialTraits_IlluminationModel(const OdGiMaterialTraits::IlluminationModel val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an
    OdGiVisualStyle::Type object.

    \param val [in] Constant OdGiVisualStyle::Type object.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setGiVisualStyle_Type(const OdGiVisualStyle::Type val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an
    OdGiVisualStyleProperties::FaceColorMode object.

    \param val [in] Constant OdGiVisualStyleProperties::FaceColorMode object.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setGiVisualStyleProperties_FaceColorMode(const OdGiVisualStyleProperties::FaceColorMode val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an
    OdGiVisualStyleProperties::EdgeModel object.

    \param val [in] Constant OdGiVisualStyleProperties::EdgeModel object.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setGiVisualStyleProperties_EdgeModel(const OdGiVisualStyleProperties::EdgeModel val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an
    OdGiFaceStyle::FaceColorMode object.

    \param val [in] Constant OdGiFaceStyle::FaceColorMode object.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setGiFaceStyle_FaceColorMode(const OdGiFaceStyle::FaceColorMode val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an
    OdGiEdgeStyle::EdgeModel object.

    \param val [in] Constant OdGiEdgeStyle::EdgeModel object.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setGiEdgeStyle_EdgeModel(const OdGiEdgeStyle::EdgeModel val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an
    OdGiToneOperatorParameters::ExteriorDaylightMode object.

    \param val [in] Constant OdGiToneOperatorParameters::ExteriorDaylightMode object.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setGiToneOperatorParameters_ExteriorDaylightMode(const OdGiToneOperatorParameters::ExteriorDaylightMode val);

  // Array-value setters.

  /** \details
    Sets the value of this OdRxPropertyVariant object from an array of OdString
    objects.

    \param val [in] Constant reference to the array of OdString.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setStringArray(const OdStringArray& val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an array of
    OdAnsiString objects.

    \param val [in] Constant reference to the array of OdAnsiString.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setAnsiStringArray(const OdAnsiStringArray& val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an array of boolean
    values.

    \param val [in] Constant reference to the array of boolean.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setBoolArray(const OdBoolArray& val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an array of OdInt8
    objects.

    \param val [in] Constant reference to the array of OdInt8.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setInt8Array(const OdInt8Array& val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an array of OdUInt8
    objects.

    \param val [in] Constant reference to the array of OdUInt8.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setUInt8Array(const OdUInt8Array& val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an array of OdInt16
    objects.

    \param val [in] Constant reference to the array of OdInt16.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setInt16Array(const OdInt16Array& val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an array of OdUInt16
    objects.

    \param val [in] Constant reference to the array of OdUInt16.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setUInt16Array(const OdUInt16Array& val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an array of OdInt32
    objects.

    \param val [in] Constant reference to the array of OdInt32.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setInt32Array(const OdInt32Array& val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an array of OdUInt32
    objects.

    \param val [in] Constant reference to the array of OdUInt32.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setUInt32Array(const OdUInt32Array& val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an array of OdInt64
    objects.

    \param val [in] Constant reference to the array of OdInt64.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setInt64Array(const OdInt64Array& val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an array of OdUInt64
    objects.

    \param val [in] Constant reference to the array of OdUInt64.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setUInt64Array(const OdUInt64Array& val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an array of double
    values.

    \param val [in] Constant reference to the array of double.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setDoubleArray(const OdDoubleArray& val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an array of
    OdGePoint3d objects.

    \param val [in] Constant reference to the array of OdGePoint3d.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setGePoint3dArray(const OdGePoint3dArray& val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an array of
    OdGePoint2d objects.

    \param val [in] Constant reference to the array of OdGePoint2d.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setGePoint2dArray(const OdGePoint2dArray& val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an array of
    OdGeVector2d objects.

    \param val [in] Constant reference to the array of OdGeVector2d.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setGeVector2dArray(const OdGeVector2dArray& val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an array of
    OdGeVector3d objects.

    \param val [in] Constant reference to the array of OdGeVector3d.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setGeVector3dArray(const OdGeVector3dArray& val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an array of
    OdCmEntityColor objects.

    \param val [in] Constant reference to the array of OdCmEntityColor.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setCmEntityColorArray(const OdCmEntityColorArray& val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an array of
    OdCmTransparency objects.

    \param val [in] Constant reference to the array of OdCmTransparency.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setCmTransparencyArray(const OdCmTransparencyArray& val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an array of
    OdGeMatrix2d objects.

    \param val [in] Constant reference to the array of OdGeMatrix2d.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setGeMatrix2dArray(const OdGeMatrix2dArray& val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an array of
    OdGeMatrix3d objects.

    \param val [in] Constant reference to the array of OdGeMatrix3d.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setGeMatrix3dArray(const OdGeMatrix3dArray& val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an array of
    OdGeScale3d objects.

    \param val [in] Constant reference to the array of OdGeScale3d.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setGeScale3dArray(const OdGeScale3dArray& val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an array of
    OdTimeStamp objects.

    \param val [in] Constant reference to the array of OdTimeStamp.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setTimeStampArray(const OdTimeStampArray& val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an array of
    OdGeExtents2d objects.

    \param val [in] Constant reference to the array of OdGeExtents2d.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setGeExtents2dArray(const OdGeExtents2dArray& val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an array of
    OdGeExtents3d objects.

    \param val [in] Constant reference to the array of OdGeExtents3d.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setGeExtents3dArray(const OdGeExtents3dArray& val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an array of
    OdGeQuaternion objects.

    \param val [in] Constant reference to the array of OdGeQuaternion.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setGeQuaternionArray(const OdGeQuaternionArray& val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an array of
    OdGiLightAttenuation objects.

    \param val [in] Constant reference to the array of OdGiLightAttenuation.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setGiLightAttenuationArray(const OdGiLightAttenuationArray& val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an array of
    OdGiSkyParameters objects.

    \param val [in] Constant reference to the array of OdGiSkyParameters.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setGiSkyParametersArray(const OdGiSkyParametersArray& val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an array of
    OdGiShadowParameters objects.

    \param val [in] Constant reference to the array of OdGiShadowParameters.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setGiShadowParametersArray(const OdGiShadowParametersArray& val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an array of
    OdGiColorRGB objects.

    \param val [in] Constant reference to the array of OdGiColorRGB.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setGiColorRGBArray(const OdGiColorRGBArray& val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an array of
    OdGiMaterialColor objects.

    \param val [in] Constant reference to the array of OdGiMaterialColor.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setGiMaterialColorArray(const OdGiMaterialColorArray& val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an array of
    OdGiMrExposureType objects.

    \param val [in] Constant reference to the array of OdGiMrExposureType.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setGiMrExposureTypeArray(const OdGiMrExposureTypeArray& val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an array of
    OdGiMrShadowMode objects.

    \param val [in] Constant reference to the array of OdGiMrShadowMode.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setGiMrShadowModeArray(const OdGiMrShadowModeArray& val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an array of
    OdGiMrFinalGatheringMode objects.

    \param val [in] Constant reference to the array of OdGiMrFinalGatheringMode.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setGiMrFinalGatheringModeArray(const OdGiMrFinalGatheringModeArray& val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an array of
    OdGiMrDiagnosticMode objects.

    \param val [in] Constant reference to the array of OdGiMrDiagnosticMode.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setGiMrDiagnosticModeArray(const OdGiMrDiagnosticModeArray& val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an array of
    OdGiMrDiagnosticPhotonMode objects.

    \param val [in] Constant reference to the array of OdGiMrDiagnosticPhotonMode.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setGiMrDiagnosticPhotonModeArray(const OdGiMrDiagnosticPhotonModeArray& val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an array of
    OdGiMrDiagnosticBSPMode objects.

    \param val [in] Constant reference to the array of OdGiMrDiagnosticBSPMode.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setGiMrDiagnosticBSPModeArray(const OdGiMrDiagnosticBSPModeArray& val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an array of
    OdGiMrExportMIMode objects.

    \param val [in] Constant reference to the array of OdGiMrExportMIMode.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setGiMrExportMIModeArray(const OdGiMrExportMIModeArray& val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an array of
    OdGiMrTileOrder objects.

    \param val [in] Constant reference to the array of OdGiMrTileOrder.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setGiMrTileOrderArray(const OdGiMrTileOrderArray& val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an array of
    OdGiLightingMode objects.

    \param val [in] Constant reference to the array of OdGiLightingMode.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setGiLightingModeArray(const OdGiLightingModeArray& val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an array of
    OdGiFilterType objects.

    \param val [in] Constant reference to the array of OdGiFilterType.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setGiFilterTypeArray(const OdGiFilterTypeArray& val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an array of
    OdCmEntityColor::ColorMethod objects.

    \param val [in] Constant reference to the array of OdCmEntityColor::ColorMethod.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setCmEntityColor_ColorMethodArray(const OdCmEntityColor_ColorMethodArray& val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an array of
    OdDb::LineWeight objects.

    \param val [in] Constant reference to the array of OdDb::LineWeight.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setDb_LineWeightArray(const OdDb_LineWeightArray& val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an array of
    OdDb::UnitsValue objects.

    \param val [in] Constant reference to the array of OdDb::UnitsValue.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setDb_UnitsValueArray(const OdDb_UnitsValueArray& val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an array of
    OdGiDgLinetypeModifiers::WidthMode objects.

    \param val [in] Constant reference to the array of OdGiDgLinetypeModifiers::WidthMode.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setGiDgLinetypeModifiers_WidthModeArray(const OdGiDgLinetypeModifiers_WidthModeArray& val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an array of
    OdGiDgLinetypeModifiers::ShiftMode objects.

    \param val [in] Constant reference to the array of OdGiDgLinetypeModifiers::ShiftMode.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setGiDgLinetypeModifiers_ShiftModeArray(const OdGiDgLinetypeModifiers_ShiftModeArray& val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an array of
    OdGiDgLinetypeModifiers::CornersMode objects.

    \param val [in] Constant reference to the array of OdGiDgLinetypeModifiers::CornersMode.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setGiDgLinetypeModifiers_CornersModeArray(const OdGiDgLinetypeModifiers_CornersModeArray& val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an array of
    OdGe::OdGeKnotParameterization objects.

    \param val [in] Constant reference to the array of OdGe::OdGeKnotParameterization.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setGe_OdGeKnotParameterizationArray(const OdGe_OdGeKnotParameterizationArray& val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an array of
    OdGiLightAttenuation::AttenuationType objects.

    \param val [in] Constant reference to the array of OdGiLightAttenuation::AttenuationType.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setGiLightAttenuation_AttenuationTypeArray(const OdGiLightAttenuation_AttenuationTypeArray& val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an array of
    OdGiDrawable::DrawableType objects.

    \param val [in] Constant reference to the array of OdGiDrawable::DrawableType.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setGiDrawable_DrawableTypeArray(const OdGiDrawable_DrawableTypeArray& val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an array of
    OdGiShadowParameters::ShadowType objects.

    \param val [in] Constant reference to the array of OdGiShadowParameters::ShadowType.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setGiShadowParameters_ShadowTypeArray(const OdGiShadowParameters_ShadowTypeArray& val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an array of
    OdGiShadowParameters::ExtendedLightShape objects.

    \param val [in] Constant reference to the array of OdGiShadowParameters::ExtendedLightShape.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setGiShadowParameters_ExtendedLightShapeArray(const OdGiShadowParameters_ExtendedLightShapeArray& val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an array of
    OdGiViewportTraits::DefaultLightingType objects.

    \param val [in] Constant reference to the array of OdGiViewportTraits::DefaultLightingType.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setGiViewportTraits_DefaultLightingTypeArray(const OdGiViewportTraits_DefaultLightingTypeArray& val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an array of
    OdGiMaterialTraits::LuminanceMode objects.

    \param val [in] Constant reference to the array of OdGiMaterialTraits::LuminanceMode.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setGiMaterialTraits_LuminanceModeArray(const OdGiMaterialTraits_LuminanceModeArray& val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an array of
    OdGiMaterialTraits::GlobalIlluminationMode objects.

    \param val [in] Constant reference to the array of OdGiMaterialTraits::GlobalIlluminationMode.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setGiMaterialTraits_GlobalIlluminationModeArray(const OdGiMaterialTraits_GlobalIlluminationModeArray& val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an array of
    OdGiMaterialTraits::FinalGatherMode objects.

    \param val [in] Constant reference to the array of OdGiMaterialTraits::FinalGatherMode.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setGiMaterialTraits_FinalGatherModeArray(const OdGiMaterialTraits_FinalGatherModeArray& val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an array of
    OdGiMaterialTraits::ChannelFlags objects.

    \param val [in] Constant reference to the array of OdGiMaterialTraits::ChannelFlags.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setGiMaterialTraits_ChannelFlagsArray(const OdGiMaterialTraits_ChannelFlagsArray& val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an array of
    OdGiMaterialTraits::Mode objects.

    \param val [in] Constant reference to the array of OdGiMaterialTraits::Mode.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setGiMaterialTraits_ModeArray(const OdGiMaterialTraits_ModeArray& val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an array of
    OdGiMaterialTraits::IlluminationModel objects.

    \param val [in] Constant reference to the array of OdGiMaterialTraits::IlluminationModel.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setGiMaterialTraits_IlluminationModelArray(const OdGiMaterialTraits_IlluminationModelArray& val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an array of
    OdGiVisualStyle::Type objects.

    \param val [in] Constant reference to the array of OdGiVisualStyle::Type.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setGiVisualStyle_TypeArray(const OdGiVisualStyle_TypeArray& val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an array of
    OdGiVisualStyleProperties::FaceColorMode objects.

    \param val [in] Constant reference to the array of OdGiVisualStyleProperties::FaceColorMode.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setGiVisualStyleProperties_FaceColorModeArray(const OdGiVisualStyleProperties_FaceColorModeArray& val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an array of
    OdGiVisualStyleProperties::EdgeModel objects.

    \param val [in] Constant reference to the array of OdGiVisualStyleProperties::EdgeModel.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setGiVisualStyleProperties_EdgeModelArray(const OdGiVisualStyleProperties_EdgeModelArray& val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an array of
    OdGiFaceStyle::FaceColorMode objects.

    \param val [in] Constant reference to the array of OdGiFaceStyle::FaceColorMode.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setGiFaceStyle_FaceColorModeArray(const OdGiFaceStyle_FaceColorModeArray& val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an array of
    OdGiEdgeStyle::EdgeModel objects.

    \param val [in] Constant reference to the array of OdGiEdgeStyle::EdgeModel.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setGiEdgeStyle_EdgeModelArray(const OdGiEdgeStyle_EdgeModelArray& val);

  /** \details
    Sets the value of this OdRxPropertyVariant object from an array of
    OdGiToneOperatorParameters::ExteriorDaylightMode objects.

    \param val [in] Constant reference to the array of OdGiToneOperatorParameters::ExteriorDaylightMode.

    \returns Reference to the OdRxPropertyVariant object.
  */
  OdRxPropertyVariant& setGiToneOperatorParameters_ExteriorDaylightModeArray(const OdGiToneOperatorParameters_ExteriorDaylightModeArray& val);

};


#include "TD_PackPop.h"

#endif //_ODRXPROPERTYVARIANT_INCLUDED_


