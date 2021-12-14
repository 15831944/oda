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


#ifndef _DBROOTVALUETYPES_H_INCLUDED_
#define _DBROOTVALUETYPES_H_INCLUDED_

#include "RxValue.h"
#include "Ge/Ge.h"
#include "Ge/GePoint2d.h"
#include "Ge/GePoint3d.h"
#include "Ge/GeScale3d.h"
#include "Ge/GeVector2d.h"
#include "Ge/GeVector3d.h"
#include "Ge/GeMatrix3d.h"
#include "DbHandle.h"
#include "OdTimeStamp.h"
#include "DbRootExport.h"
#include "Ge/GeExtents2d.h"
#include "Ge/GeExtents3d.h"
#include "Gi/GiDgLinetype.h"
#include "Ge/GeQuaternion.h"
#include "Ge/GeMatrix2d.h"
#include "Ge/GeMatrix3d.h"
#include "Gi/GiLightTraits.h"
#include "Gi/GiDrawable.h"
#include "Gi/GiViewportTraits.h"
#include "Gi/GiMaterial.h"
#include "Gi/GiEnvironment.h"
#include "Gi/GiVisualStyle.h"

/**  \details
    OdRxValueType specialization for 'ODRECT' value type
*/
ODRX_DECLARE_VALUE_TYPE(ODRECT, DBROOT_EXPORT);

/**  \details
    OdRxValueType specialization for 'OdGePoint2d' value type
*/
ODRX_DECLARE_VALUE_TYPE(OdGePoint2d, DBROOT_EXPORT);

/**  \details
    OdRxValueType specialization for 'OdGePoint3d' value type
*/
ODRX_DECLARE_VALUE_TYPE(OdGePoint3d, DBROOT_EXPORT);

/**  \details
    OdRxValueType specialization for 'OdGeScale3d' value type
*/
ODRX_DECLARE_VALUE_TYPE(OdGeScale3d, DBROOT_EXPORT);

/**  \details
    OdRxValueType specialization for 'OdGeVector2d' value type
*/
ODRX_DECLARE_VALUE_TYPE(OdGeVector2d, DBROOT_EXPORT);

/**  \details
    OdRxValueType specialization for 'OdGeVector3d' value type
*/
ODRX_DECLARE_VALUE_TYPE(OdGeVector3d, DBROOT_EXPORT);

/**  \details
    OdRxValueType specialization for 'OdTimeStamp' value type
*/
ODRX_DECLARE_VALUE_TYPE(OdTimeStamp, DBROOT_EXPORT);

/**  \details
    OdRxValueType specialization for 'OdCmEntityColor::ColorMethod' value type
*/
ODRX_DECLARE_VALUE_TYPE(OdCmEntityColor::ColorMethod, DBROOT_EXPORT);

/**  \details
    OdRxValueType specialization for 'OdCmEntityColor' value type
*/
ODRX_DECLARE_VALUE_TYPE(OdCmEntityColor, DBROOT_EXPORT);

/**  \details
    OdRxValueType specialization for 'OdDb::LineWeight' value type
*/
ODRX_DECLARE_VALUE_TYPE(OdDb::LineWeight, DBROOT_EXPORT);

/**  \details
    OdRxValueType specialization for 'OdDb::UnitsValue' value type
*/
ODRX_DECLARE_VALUE_TYPE(OdDb::UnitsValue, DBROOT_EXPORT);

/**  \details
    OdRxValueType specialization for 'OdGeExtents2d' value type
*/
ODRX_DECLARE_VALUE_TYPE(OdGeExtents2d, DBROOT_EXPORT);

/**  \details
    OdRxValueType specialization for 'OdGeExtents3d' value type
*/
ODRX_DECLARE_VALUE_TYPE(OdGeExtents3d, DBROOT_EXPORT);

/**  \details
    OdRxValueType specialization for 'OdGeMatrix2d' value type
*/
ODRX_DECLARE_VALUE_TYPE(OdGeMatrix2d, DBROOT_EXPORT);

/**  \details
    OdRxValueType specialization for 'OdGeMatrix3d' value type
*/
ODRX_DECLARE_VALUE_TYPE(OdGeMatrix3d, DBROOT_EXPORT);

/**  \details
    OdRxValueType specialization for 'OdGeQuaternion' value type
*/
ODRX_DECLARE_VALUE_TYPE(OdGeQuaternion, DBROOT_EXPORT);

/**  \details
    OdRxValueType specialization for 'OdGiDgLinetypeModifiers::WidthMode' value type
*/
ODRX_DECLARE_VALUE_TYPE(OdGiDgLinetypeModifiers::WidthMode, DBROOT_EXPORT);

/**  \details
    OdRxValueType specialization for 'OdGiDgLinetypeModifiers::ShiftMode' value type
*/
ODRX_DECLARE_VALUE_TYPE(OdGiDgLinetypeModifiers::ShiftMode, DBROOT_EXPORT);

/**  \details
    OdRxValueType specialization for 'OdGiDgLinetypeModifiers::CornersMode' value type
*/
ODRX_DECLARE_VALUE_TYPE(OdGiDgLinetypeModifiers::CornersMode, DBROOT_EXPORT);

/**  \details
    OdRxValueType specialization for 'OdGiDgLinetypeModifiers' value type
*/
ODRX_DECLARE_VALUE_TYPE(OdGiDgLinetypeModifiers, DBROOT_EXPORT);

/**  \details
    OdRxValueType specialization for 'OdArray<OdGePoint3d>' value type
*/
ODRX_DECLARE_VALUE_TYPE(OdArray<OdGePoint3d>, DBROOT_EXPORT);

/**  \details
    OdRxValueType specialization for 'OdArray<OdTimeStamp>' value type
*/
ODRX_DECLARE_VALUE_TYPE(OdArray<OdTimeStamp>, DBROOT_EXPORT);

/**  \details
    OdRxValueType specialization for 'OdGe::OdGeKnotParameterization' value type
*/
ODRX_DECLARE_VALUE_TYPE(OdGe::OdGeKnotParameterization, DBROOT_EXPORT);

/**  \details
    OdRxValueType specialization for 'OdGiLightAttenuation::AttenuationType' value type
*/
ODRX_DECLARE_VALUE_TYPE(OdGiLightAttenuation::AttenuationType, DBROOT_EXPORT);

/**  \details
    OdRxValueType specialization for 'OdGiLightAttenuation' value type
*/
ODRX_DECLARE_VALUE_TYPE(OdGiLightAttenuation, DBROOT_EXPORT);

/**  \details
    OdRxValueType specialization for 'OdGiSkyParameters' value type
*/
ODRX_DECLARE_VALUE_TYPE(OdGiSkyParameters, DBROOT_EXPORT);

/**  \details
    OdRxValueType specialization for 'OdGiDrawable::DrawableType' value type
*/
ODRX_DECLARE_VALUE_TYPE(OdGiDrawable::DrawableType, DBROOT_EXPORT);

/**  \details
    OdRxValueType specialization for 'OdGiShadowParameters::ShadowType' value type
*/
ODRX_DECLARE_VALUE_TYPE(OdGiShadowParameters::ShadowType, DBROOT_EXPORT);

/**  \details
    OdRxValueType specialization for 'OdGiShadowParameters::ExtendedLightShape' value type
*/
ODRX_DECLARE_VALUE_TYPE(OdGiShadowParameters::ExtendedLightShape, DBROOT_EXPORT);

/**  \details
    OdRxValueType specialization for 'OdGiShadowParameters' value type
*/
ODRX_DECLARE_VALUE_TYPE(OdGiShadowParameters, DBROOT_EXPORT);

/**  \details
    OdRxValueType specialization for 'OdGiColorRGB' value type
*/
ODRX_DECLARE_VALUE_TYPE(OdGiColorRGB, DBROOT_EXPORT);

/**  \details
    OdRxValueType specialization for 'OdGiViewportTraits::DefaultLightingType' value type
*/
ODRX_DECLARE_VALUE_TYPE(OdGiViewportTraits::DefaultLightingType, DBROOT_EXPORT);

/**  \details
    OdRxValueType specialization for 'OdGiMaterialTraits::LuminanceMode' value type
*/
ODRX_DECLARE_VALUE_TYPE(OdGiMaterialTraits::LuminanceMode, DBROOT_EXPORT);

/**  \details
    OdRxValueType specialization for 'OdGiMaterialTraits::GlobalIlluminationMode' value type
*/
ODRX_DECLARE_VALUE_TYPE(OdGiMaterialTraits::GlobalIlluminationMode, DBROOT_EXPORT);

/**  \details
    OdRxValueType specialization for 'OdGiMaterialTraits::FinalGatherMode' value type
*/
ODRX_DECLARE_VALUE_TYPE(OdGiMaterialTraits::FinalGatherMode, DBROOT_EXPORT);

/**  \details
    OdRxValueType specialization for 'OdGiMaterialTraits::ChannelFlags' value type
*/
ODRX_DECLARE_VALUE_TYPE(OdGiMaterialTraits::ChannelFlags, DBROOT_EXPORT);

/**  \details
    OdRxValueType specialization for 'OdGiMaterialTraits::Mode' value type
*/
ODRX_DECLARE_VALUE_TYPE(OdGiMaterialTraits::Mode, DBROOT_EXPORT);

/**  \details
    OdRxValueType specialization for 'OdGiMaterialTraits::IlluminationModel' value type
*/
ODRX_DECLARE_VALUE_TYPE(OdGiMaterialTraits::IlluminationModel, DBROOT_EXPORT);

/**  \details
    OdRxValueType specialization for 'OdGiMrExposureType' value type
*/
ODRX_DECLARE_VALUE_TYPE(OdGiMrExposureType, DBROOT_EXPORT);

/**  \details
    OdRxValueType specialization for 'OdGiMrShadowMode' value type
*/
ODRX_DECLARE_VALUE_TYPE(OdGiMrShadowMode, DBROOT_EXPORT);

/**  \details
    OdRxValueType specialization for 'OdGiMrFinalGatheringMode' value type
*/
ODRX_DECLARE_VALUE_TYPE(OdGiMrFinalGatheringMode, DBROOT_EXPORT);

/**  \details
    OdRxValueType specialization for 'OdGiMrDiagnosticMode' value type
*/
ODRX_DECLARE_VALUE_TYPE(OdGiMrDiagnosticMode, DBROOT_EXPORT);

/**  \details
    OdRxValueType specialization for 'OdGiMrDiagnosticPhotonMode' value type
*/
ODRX_DECLARE_VALUE_TYPE(OdGiMrDiagnosticPhotonMode, DBROOT_EXPORT);

/**  \details
    OdRxValueType specialization for 'OdGiMrDiagnosticBSPMode' value type
*/
ODRX_DECLARE_VALUE_TYPE(OdGiMrDiagnosticBSPMode, DBROOT_EXPORT);

/**  \details
    OdRxValueType specialization for 'OdGiMrExportMIMode' value type
*/
ODRX_DECLARE_VALUE_TYPE(OdGiMrExportMIMode, DBROOT_EXPORT);

/**  \details
    OdRxValueType specialization for 'OdGiMrTileOrder' value type
*/
ODRX_DECLARE_VALUE_TYPE(OdGiMrTileOrder, DBROOT_EXPORT);

/**  \details
    OdRxValueType specialization for 'OdGiLightingMode' value type
*/
ODRX_DECLARE_VALUE_TYPE(OdGiLightingMode, DBROOT_EXPORT);

/**  \details
    OdRxValueType specialization for 'OdGiFilterType' value type
*/
ODRX_DECLARE_VALUE_TYPE(OdGiFilterType, DBROOT_EXPORT);

/**  \details
    OdRxValueType specialization for 'OdGiVisualStyle::Type' value type
*/
ODRX_DECLARE_VALUE_TYPE(OdGiVisualStyle::Type, DBROOT_EXPORT);

/**  \details
    OdRxValueType specialization for 'OdGiVisualStyleProperties::FaceColorMode' value type
*/
ODRX_DECLARE_VALUE_TYPE(OdGiVisualStyleProperties::FaceColorMode, DBROOT_EXPORT);

/**  \details
    OdRxValueType specialization for 'OdGiVisualStyleProperties::EdgeModel' value type
*/
ODRX_DECLARE_VALUE_TYPE(OdGiVisualStyleProperties::EdgeModel, DBROOT_EXPORT);

/**  \details
    OdRxValueType specialization for 'OdGiFaceStyle::FaceColorMode' value type
*/
ODRX_DECLARE_VALUE_TYPE(OdGiFaceStyle::FaceColorMode, DBROOT_EXPORT);

/**  \details
    OdRxValueType specialization for 'OdGiEdgeStyle::EdgeModel' value type
*/
ODRX_DECLARE_VALUE_TYPE(OdGiEdgeStyle::EdgeModel, DBROOT_EXPORT);

/**  \details
    OdRxValueType specialization for 'OdGiToneOperatorParameters::ExteriorDaylightMode' value type
*/
ODRX_DECLARE_VALUE_TYPE(OdGiToneOperatorParameters::ExteriorDaylightMode, DBROOT_EXPORT);

/**  \details
    OdRxValueType specialization for 'OdGiToneOperatorParametersPtr' value type
*/
ODRX_DECLARE_VALUE_TYPE(OdGiToneOperatorParametersPtr, DBROOT_EXPORT);

/**  \details
    OdRxValueType specialization for 'OdGiMaterialColor' value type
*/
ODRX_DECLARE_VALUE_TYPE(OdGiMaterialColor, DBROOT_EXPORT);

#endif
