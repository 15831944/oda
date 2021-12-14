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


#ifndef _DBVALUETYPES_H_INCLUDED_
#define _DBVALUETYPES_H_INCLUDED_

#include "DbRootValueTypes.h"
#include "DbDatabase.h"
#include "DbObjectId.h"
#include "CmColor.h"
#include "DbDate.h"
#include "DbBlockTableRecord.h"
#include "DbMLeaderStyle.h"
#include "DbMText.h"
#include "Db2dPolyline.h"
#include "Db3dPolyline.h"
#include "Db3dPolylineVertex.h"
#include "DbLeader.h"
#include "DbTable.h"
#include "DbSpline.h"
#include "DbHelix.h"
#include "DbGeoPositionMarker.h"
#include "DbHatch.h"
#include "DbLight.h"
#include "DbLoftOptions.h"
#include "DbSweepOptions.h"
#include "DbOle2Frame.h"
#include "DbPolygonMesh.h"
#include "DbSection.h"
#include "Db2dVertex.h"
#include "ViewportDefs.h"
#include "DbViewport.h"
#include "DbPlotSettings.h"
#include "DbRenderSettings.h"
#include "DbOle2Frame.h"

/** \details
  This class is a specialization of OdDbObjectId for OdDbBlockTableRecord.

  \remarks
  An OdDbBlockTableRecordId reference to OdDbBlockTableRecord for creating a
  ValueType with subproperties.
  Corresponding C++ library: TD_DbEntities

  <group OdDb_Classes>
*/
class TOOLKIT_EXPORT OdDbBlockTableRecordId : public OdDbObjectId
{
public:

  /** \details
    Default constructor for objects of the OdDbBlockTableRecordId class.
  */
  OdDbBlockTableRecordId() {}

  /** \details
    Copy constructor for objects of the OdDbBlockTableRecordId class. Clones an
    existing instance of the class.

    \param objectId [in] Object to be cloned.
  */
  OdDbBlockTableRecordId(
    const OdDbObjectId& objectId) : OdDbObjectId(objectId) {}

//DOM-IGNORE-BEGIN
  OdDbBlockTableRecordId(
    OdDbStub* objectId) : OdDbObjectId(objectId) {}
//DOM-IGNORE-END

  /** \details
    Assignment operator.

    \param objectId [in] Object ID to be assigned to this OdDbBlockTableRecordId.
  */
  OdDbBlockTableRecordId& operator = (
    const OdDbBlockTableRecordId& objectId) {
    OdDbObjectId::operator= (objectId); return *this;
  }

  /** \details
    Assignment operator.

    \param objectId [in] Object ID to be assigned to this OdDbBlockTableRecordId.
  */
  OdDbBlockTableRecordId& operator = (
    const OdDbObjectId& objectId) {
    OdDbObjectId::operator= (objectId); return *this;
  }

//DOM-IGNORE-BEGIN
  OdDbBlockTableRecordId& operator = (
    OdDbStub* objectId) {
    m_Id = (OdDbStub*)objectId; return *this;
  }
//DOM-IGNORE-END

  /** \details
    Negative comparison operator.

    \param objectId [in] Object ID to compare with this OdDbBlockTableRecordId.

    \returns
    True if object IDs are not equal; false otherwise.
  */
  bool operator != (
    const OdDbObjectId& objectId) const {
    return OdDbObjectId::operator!= (objectId);
  }

//DOM-IGNORE-BEGIN
  bool operator != (
    OdDbStub* objectId) const {
    return OdDbObjectId::operator!= (objectId);
  }
//DOM-IGNORE-END

  /** \details
    Comparison operator.

    \param objectId [in] Object ID to compare with this OdDbBlockTableRecordId.

    \returns
    True if object IDs are equal; false otherwise.
  */
  bool operator == (
    const OdDbObjectId& objectId) const {
    return OdDbObjectId::operator== (objectId);
  }

//DOM-IGNORE-BEGIN
  bool operator == (
    OdDbStub* objectId) const {
    return OdDbObjectId::operator== (objectId);
  }
//DOM-IGNORE-END

};

/** \details
  OdRxValueType specialization for 'OdDbBlockTableRecordId' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdDbBlockTableRecordId, DBENT_EXPORT);

/** \details
  OdRxValueType specialization for 'OdDbObjectId' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdDbObjectId, DBENT_EXPORT);

/** \details
  OdRxValueType specialization for 'OdCmColor' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdCmColor, DBENT_EXPORT);

/** \details
  OdRxValueType specialization for 'OdDbSweepOptions' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdDbSweepOptions, DBENT_EXPORT);

/** \details
  OdRxValueType specialization for 'OdDbDate' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdDbDate, DBENT_EXPORT);

/** \details
  OdRxValueType specialization for 'OdCellRange' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdCellRange, DBENT_EXPORT);

/** \details
  OdRxValueType specialization for 'OdDb3dPolylineVertexPtr' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdDb3dPolylineVertexPtr, DBENT_EXPORT);

/** \details
  OdRxValueType specialization for 'OdRectangle3d' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdRectangle3d, DBENT_EXPORT);

/** \details
  OdRxValueType specialization for 'OdDb::DuplicateRecordCloning' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdDb::DuplicateRecordCloning, DBENT_EXPORT);

/** \details
  OdRxValueType specialization for 'OdDb::LineSpacingStyle' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdDb::LineSpacingStyle, DBENT_EXPORT);

/** \details
  OdRxValueType specialization for 'OdDb::JoinStyle' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdDb::JoinStyle, DBENT_EXPORT);

/** \details
  OdRxValueType specialization for 'OdDb::EndCaps' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdDb::EndCaps, DBENT_EXPORT);

/** \details
  OdRxValueType specialization for 'OdDb::DwgVersion' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdDb::DwgVersion, DBENT_EXPORT);

/** \details
  OdRxValueType specialization for 'OdDb::LoftNormalsType' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdDb::LoftNormalsType, DBENT_EXPORT);

/** \details
  OdRxValueType specialization for 'OdDb::MeasurementValue' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdDb::MeasurementValue, DBENT_EXPORT);

/** \details
  OdRxValueType specialization for 'OdDb::MaintReleaseVer' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdDb::MaintReleaseVer, DBENT_EXPORT);

/** \details
  OdRxValueType specialization for 'OdDb::Visibility' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdDb::Visibility, DBENT_EXPORT);

/** \details
  OdRxValueType specialization for 'OdDb::CollisionType' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdDb::CollisionType, DBENT_EXPORT);

/** \details
  OdRxValueType specialization for 'OdDb::Poly2dType' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdDb::Poly2dType, DBENT_EXPORT);

/** \details
  OdRxValueType specialization for 'OdDb::Poly3dType' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdDb::Poly3dType, DBENT_EXPORT);

/** \details
  OdRxValueType specialization for 'OdDb::TableBreakFlowDirection' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdDb::TableBreakFlowDirection, DBENT_EXPORT);

/** \details
  OdRxValueType specialization for 'OdDb::TableBreakOption' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdDb::TableBreakOption, DBENT_EXPORT);

/** \details
  OdRxValueType specialization for 'OdDb::FlowDirection' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdDb::FlowDirection, DBENT_EXPORT);

/** \details
  OdRxValueType specialization for 'OdDb::PolyMeshType' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdDb::PolyMeshType, DBENT_EXPORT);

/** \details
  OdRxValueType specialization for 'OdDb::TextHorzMode' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdDb::TextHorzMode, DBENT_EXPORT);

/** \details
  OdRxValueType specialization for 'OdDb::TextVertMode' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdDb::TextVertMode, DBENT_EXPORT);

/** \details
  OdRxValueType specialization for 'OdDb::Vertex2dType' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdDb::Vertex2dType, DBENT_EXPORT);

/** \details
  OdRxValueType specialization for 'OdDb::Vertex3dType' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdDb::Vertex3dType, DBENT_EXPORT);

/** \details
  OdRxValueType specialization for 'OdDb::OrthographicView' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdDb::OrthographicView, DBENT_EXPORT);

/** \details
  OdRxValueType specialization for 'OdDb::XrefStatus' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdDb::XrefStatus, DBENT_EXPORT);

/** \details
  OdRxValueType specialization for 'OdDbBlockTableRecord::BlockScaling' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdDbBlockTableRecord::BlockScaling, DBENT_EXPORT);

/** \details
  OdRxValueType specialization for 'OdDbMLeaderStyle::ContentType' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdDbMLeaderStyle::ContentType, DBENT_EXPORT);

/** \details
  OdRxValueType specialization for 'OdDbMLeaderStyle::LeaderType' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdDbMLeaderStyle::LeaderType, DBENT_EXPORT);

/** \details
  OdRxValueType specialization for 'OdDbMLeaderStyle::TextAngleType' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdDbMLeaderStyle::TextAngleType, DBENT_EXPORT);

/** \details
  OdRxValueType specialization for 'OdDbMLeaderStyle::BlockConnectionType' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdDbMLeaderStyle::BlockConnectionType, DBENT_EXPORT);

/** \details
  OdRxValueType specialization for 'OdDbMLeaderStyle::DrawLeaderOrderType' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdDbMLeaderStyle::DrawLeaderOrderType, DBENT_EXPORT);

/** \details
  OdRxValueType specialization for 'OdDbMLeaderStyle::DrawMLeaderOrderType' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdDbMLeaderStyle::DrawMLeaderOrderType, DBENT_EXPORT);

/** \details
  OdRxValueType specialization for 'OdDbMLeaderStyle::TextAttachmentDirection' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdDbMLeaderStyle::TextAttachmentDirection, DBENT_EXPORT);

/** \details
  OdRxValueType specialization for 'OdDbMLeaderStyle::TextAlignmentType' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdDbMLeaderStyle::TextAlignmentType, DBENT_EXPORT);

/** \details
  OdRxValueType specialization for 'OdDbMLeaderStyle::TextAttachmentType' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdDbMLeaderStyle::TextAttachmentType, DBENT_EXPORT);

/** \details
  OdRxValueType specialization for 'OdDbMLeaderStyle::SegmentAngleType' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdDbMLeaderStyle::SegmentAngleType, DBENT_EXPORT);

/** \details
  OdRxValueType specialization for 'OdDbMText::FlowDirection' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdDbMText::FlowDirection, DBENT_EXPORT);

/** \details
  OdRxValueType specialization for 'OdDbMText::AttachmentPoint' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdDbMText::AttachmentPoint, DBENT_EXPORT);

/** \details
  OdRxValueType specialization for 'OdDbMText::ColumnType' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdDbMText::ColumnType, DBENT_EXPORT);

/** \details
  OdRxValueType specialization for 'OdDbMTextPtr' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdDbMTextPtr, DBENT_EXPORT);

/** \details
  OdRxValueType specialization for 'OdDbLeader::AnnoType' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdDbLeader::AnnoType, DBENT_EXPORT);

/** \details
  OdRxValueType specialization for 'OdDbSpline::SplineType' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdDbSpline::SplineType, DBENT_EXPORT);

/** \details
  OdRxValueType specialization for 'OdDbHelix::ConstrainType' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdDbHelix::ConstrainType, DBENT_EXPORT);

/** \details
  OdRxValueType specialization for 'OdDbGeoPositionMarker::OdTextAlignmentType' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdDbGeoPositionMarker::OdTextAlignmentType, DBENT_EXPORT);

/** \details
  OdRxValueType specialization for 'OdDbHatch::GradientPatternType' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdDbHatch::GradientPatternType, DBENT_EXPORT);

/** \details
  OdRxValueType specialization for 'OdDbHatch::HatchObjectType' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdDbHatch::HatchObjectType, DBENT_EXPORT);

/** \details
  OdRxValueType specialization for 'OdDbHatch::HatchStyle' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdDbHatch::HatchStyle, DBENT_EXPORT);

/** \details
  OdRxValueType specialization for 'OdDbHatch::HatchPatternType' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdDbHatch::HatchPatternType, DBENT_EXPORT);

/** \details
  OdRxValueType specialization for 'OdDbLight::LampColorType' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdDbLight::LampColorType, DBENT_EXPORT);

/** \details
  OdRxValueType specialization for 'OdDbLight::LampColorPreset' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdDbLight::LampColorPreset, DBENT_EXPORT);

/** \details
  OdRxValueType specialization for 'OdDbLight::GlyphDisplayType' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdDbLight::GlyphDisplayType, DBENT_EXPORT);

/** \details
  OdRxValueType specialization for 'OdDbLight::PhysicalIntensityMethod' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdDbLight::PhysicalIntensityMethod, DBENT_EXPORT);

/** \details
  OdRxValueType specialization for 'OdDbLoftOptions::NormalOption' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdDbLoftOptions::NormalOption, DBENT_EXPORT);

/** \details
  OdRxValueType specialization for 'OdDbLoftOptions' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdDbLoftOptions, DBENT_EXPORT);

/** \details
  OdRxValueType specialization for 'OdDbOle2Frame::Type' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdDbOle2Frame::Type, DBENT_EXPORT);

/** \details
  OdRxValueType specialization for 'OdDbOle2Frame::PlotQuality' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdDbOle2Frame::PlotQuality, DBENT_EXPORT);

/** \details
  OdRxValueType specialization for 'OdDbSection::State' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdDbSection::State, DBENT_EXPORT);

/** \details
  OdRxValueType specialization for 'OdDbViewport::ShadePlotType' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdDbViewport::ShadePlotType, DBENT_EXPORT);

/** \details
  OdRxValueType specialization for 'OdDbViewport::StandardScaleType' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdDbViewport::StandardScaleType, DBENT_EXPORT);

/** \details
  OdRxValueType specialization for 'OdDbPlotSettings::PlotRotation' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdDbPlotSettings::PlotRotation, DBENT_EXPORT);

/** \details
  OdRxValueType specialization for 'OdDbPlotSettings::ShadePlotResLevel' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdDbPlotSettings::ShadePlotResLevel, DBENT_EXPORT);

/** \details
  OdRxValueType specialization for 'OdDbPlotSettings::ShadePlotType' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdDbPlotSettings::ShadePlotType, DBENT_EXPORT);

/** \details
  OdRxValueType specialization for 'OdDbPlotSettings::PlotPaperUnits' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdDbPlotSettings::PlotPaperUnits, DBENT_EXPORT);

/** \details
  OdRxValueType specialization for 'OdDbPlotSettings::PlotType' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdDbPlotSettings::PlotType, DBENT_EXPORT);

/** \details
  OdRxValueType specialization for 'OdDbPlotSettings::StdScaleType' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdDbPlotSettings::StdScaleType, DBENT_EXPORT);

/** \details
  OdRxValueType specialization for 'OdDbRenderGlobal::Procedure' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdDbRenderGlobal::Procedure, DBENT_EXPORT);

/** \details
  OdRxValueType specialization for 'OdDbRenderGlobal::Destination' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdDbRenderGlobal::Destination, DBENT_EXPORT);

/** \details
  OdRxValueType specialization for 'OdDbMentalRayRenderSettings::ShadowSamplingMultiplier' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdDbMentalRayRenderSettings::ShadowSamplingMultiplier, DBENT_EXPORT);

/** \details
  OdRxValueType specialization for 'OdDbRapidRTRenderSettings::RenderTarget' value type.
*/
ODRX_DECLARE_VALUE_TYPE(OdDbRapidRTRenderSettings::RenderTarget, DBENT_EXPORT);


#endif
