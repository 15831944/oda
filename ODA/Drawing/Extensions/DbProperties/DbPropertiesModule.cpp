///////////////////////////////////////////////////////////////////////////////
// THIS FILE IS MACHINE GENERATED!!!!                                        //
///////////////////////////////////////////////////////////////////////////////

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
#include "RxProperty.h"
#include "RxDynamicModule.h"
#include "DynamicLinker.h"
#include "DbProperties.h"
#include "DbValueTypes.h"
#include "DbBackground.h"
#include "DbRenderSettings.h"
#include "DbDatabase.h"
#include "DbObject.h"
#include "DbSymbolTable.h"
#include "DbBody.h"
#include "DbLine.h"
#include "DbCurve.h"
#include "DbEntity.h"
#include "DbDictionary.h"
#include "Db3dSolid.h"
#include "DbArc.h"
#include "DbCircle.h"
#include "DbEllipse.h"
#include "DbPolyline.h"
#include "DbBlockReference.h"
#include "DbMLeader.h"
#include "DbTable.h"
#include "DbMInsertBlock.h"
#include "Db2dPolyline.h"
#include "Db3dPolyline.h"
#include "DbRay.h"
#include "DbLeader.h"
#include "DbSpline.h"
#include "DbHelix.h"
#include "DbXline.h"
#include "DbDimension.h"
#include "Db2LineAngularDimension.h"
#include "Db3PointAngularDimension.h"
#include "DbAlignedDimension.h"
#include "DbArcDimension.h"
#include "DbDiametricDimension.h"
#include "DbOrdinateDimension.h"
#include "DbRadialDimension.h"
#include "DbRadialDimensionLarge.h"
#include "DbRotatedDimension.h"
#include "DbFace.h"
#include "DbFcf.h"
#include "DbGeoPositionMarker.h"
#include "DbHatch.h"
#include "DbMline.h"
#include "DbMText.h"
#include "DbOle2Frame.h"
#include "DbPoint.h"
#include "DbPolyFaceMesh.h"
#include "DbPolygonMesh.h"
#include "DbProxyEntity.h"
#include "DbRegion.h"
#include "DbSection.h"
#include "DbShape.h"
#include "DbSolid.h"
#include "DbSubDMesh.h"
#include "DbLight.h"
#include "DbSurface.h"
#include "DbExtrudedSurface.h"
#include "DbLoftedSurface.h"
#include "DbNurbSurface.h"
#include "DbRevolvedSurface.h"
#include "DbSweptSurface.h"
#include "DbSun.h"
#include "DbText.h"
#include "DbAttribute.h"
#include "DbAttributeDefinition.h"
#include "DbTrace.h"
#include "DbUnderlayReference.h"
#include "Db2dVertex.h"
#include "Db3dPolylineVertex.h"
#include "DbViewport.h"
#include "DbGroup.h"
#include "DbMaterial.h"
#include "DbMlineStyle.h"
#include "DbModelDocViewStyle.h"
#include "DbPlotSettings.h"
#include "DbLayout.h"
#include "DbProxyObject.h"
#include "DbSymbolTableRecord.h"
#include "DbAbstractViewTableRecord.h"
#include "DbViewportTableRecord.h"
#include "DbViewTableRecord.h"
#include "DbBlockTableRecord.h"
#include "DbDimStyleTableRecord.h"
#include "DbLayerTableRecord.h"
#include "DbLinetypeTableRecord.h"
#include "DbTextStyleTableRecord.h"
#include "DbUCSTableRecord.h"
#include "DbTableStyle.h"
#include "DbUnderlayDefinition.h"
#include "DbVisualStyle.h"
#include "DbMLeaderStyle.h"
#include "DbBlockTable.h"

FIRSTDLL_EXPORT void odrxSetMemberConstructor(OdRxClass* pClass, OdRxMemberCollectionConstructorPtr pc, void* data = 0);

class OdDbPropertiesModule : public OdRxModule
{
  static OdArray<OdRxMemberPtr> properties;
  static void constructOdDbDatabaseProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDbDatabaseProperties(properties, b, data);
  };
  static void constructOdDbObjectProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDbObjectProperties(properties, b, data);
  };
  static void constructOdDbSymbolTableProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDbSymbolTableProperties(properties, b, data);
  };
  static void constructOdDbBodyProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDbBodyProperties(properties, b, data);
  };
  static void constructOdDbLineProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDbLineProperties(properties, b, data);
  };
  static void constructOdDbCurveProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDbCurveProperties(properties, b, data);
  };
  static void constructOdDbEntityProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDbEntityProperties(properties, b, data);
  };
  static void constructOdDbIBLBackgroundProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDbIBLBackgroundProperties(properties, b, data);
  };
  static void constructOdDbSkyBackgroundProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDbSkyBackgroundProperties(properties, b, data);
  };
  static void constructOdDbDictionaryProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDbDictionaryProperties(properties, b, data);
  };
  static void constructOdDb3dSolidProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDb3dSolidProperties(properties, b, data);
  };
  static void constructOdDbArcProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDbArcProperties(properties, b, data);
  };
  static void constructOdDbCircleProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDbCircleProperties(properties, b, data);
  };
  static void constructOdDbEllipseProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDbEllipseProperties(properties, b, data);
  };
  static void constructOdDbPolylineProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDbPolylineProperties(properties, b, data);
  };
  static void constructOdDbBlockReferenceProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDbBlockReferenceProperties(properties, b, data);
  };
  static void constructOdDbMLeaderProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDbMLeaderProperties(properties, b, data);
  };
  static void constructOdDbTableProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDbTableProperties(properties, b, data);
  };
  static void constructOdDbMInsertBlockProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDbMInsertBlockProperties(properties, b, data);
  };
  static void constructOdDb2dPolylineProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDb2dPolylineProperties(properties, b, data);
  };
  static void constructOdDb3dPolylineProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDb3dPolylineProperties(properties, b, data);
  };
  static void constructOdDbRayProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDbRayProperties(properties, b, data);
  };
  static void constructOdDbLeaderProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDbLeaderProperties(properties, b, data);
  };
  static void constructOdDbSplineProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDbSplineProperties(properties, b, data);
  };
  static void constructOdDbHelixProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDbHelixProperties(properties, b, data);
  };
  static void constructOdDbXlineProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDbXlineProperties(properties, b, data);
  };
  static void constructOdDbDimensionProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDbDimensionProperties(properties, b, data);
  };
  static void constructOdDb2LineAngularDimensionProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDb2LineAngularDimensionProperties(properties, b, data);
  };
  static void constructOdDb3PointAngularDimensionProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDb3PointAngularDimensionProperties(properties, b, data);
  };
  static void constructOdDbAlignedDimensionProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDbAlignedDimensionProperties(properties, b, data);
  };
  static void constructOdDbArcDimensionProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDbArcDimensionProperties(properties, b, data);
  };
  static void constructOdDbDiametricDimensionProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDbDiametricDimensionProperties(properties, b, data);
  };
  static void constructOdDbOrdinateDimensionProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDbOrdinateDimensionProperties(properties, b, data);
  };
  static void constructOdDbRadialDimensionProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDbRadialDimensionProperties(properties, b, data);
  };
  static void constructOdDbRadialDimensionLargeProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDbRadialDimensionLargeProperties(properties, b, data);
  };
  static void constructOdDbRotatedDimensionProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDbRotatedDimensionProperties(properties, b, data);
  };
  static void constructOdDbFaceProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDbFaceProperties(properties, b, data);
  };
  static void constructOdDbFcfProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDbFcfProperties(properties, b, data);
  };
  static void constructOdDbGeoPositionMarkerProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDbGeoPositionMarkerProperties(properties, b, data);
  };
  static void constructOdDbHatchProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDbHatchProperties(properties, b, data);
  };
  static void constructOdDbMlineProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDbMlineProperties(properties, b, data);
  };
  static void constructOdDbMTextProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDbMTextProperties(properties, b, data);
  };
  static void constructOdDbOle2FrameProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDbOle2FrameProperties(properties, b, data);
  };
  static void constructOdDbPointProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDbPointProperties(properties, b, data);
  };
  static void constructOdDbPolyFaceMeshProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDbPolyFaceMeshProperties(properties, b, data);
  };
  static void constructOdDbPolygonMeshProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDbPolygonMeshProperties(properties, b, data);
  };
  static void constructOdDbProxyEntityProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDbProxyEntityProperties(properties, b, data);
  };
  static void constructOdDbRegionProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDbRegionProperties(properties, b, data);
  };
  static void constructOdDbSectionProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDbSectionProperties(properties, b, data);
  };
  static void constructOdDbShapeProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDbShapeProperties(properties, b, data);
  };
  static void constructOdDbSolidProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDbSolidProperties(properties, b, data);
  };
  static void constructOdDbSubDMeshProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDbSubDMeshProperties(properties, b, data);
  };
  static void constructOdDbLightProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDbLightProperties(properties, b, data);
  };
  static void constructOdDbSurfaceProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDbSurfaceProperties(properties, b, data);
  };
  static void constructOdDbExtrudedSurfaceProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDbExtrudedSurfaceProperties(properties, b, data);
  };
  static void constructOdDbLoftedSurfaceProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDbLoftedSurfaceProperties(properties, b, data);
  };
  static void constructOdDbNurbSurfaceProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDbNurbSurfaceProperties(properties, b, data);
  };
  static void constructOdDbRevolvedSurfaceProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDbRevolvedSurfaceProperties(properties, b, data);
  };
  static void constructOdDbSweptSurfaceProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDbSweptSurfaceProperties(properties, b, data);
  };
  static void constructOdDbSunProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDbSunProperties(properties, b, data);
  };
  static void constructOdDbSolidBackgroundProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDbSolidBackgroundProperties(properties, b, data);
  };
  static void constructOdDbTextProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDbTextProperties(properties, b, data);
  };
  static void constructOdDbAttributeProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDbAttributeProperties(properties, b, data);
  };
  static void constructOdDbAttributeDefinitionProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDbAttributeDefinitionProperties(properties, b, data);
  };
  static void constructOdDbTraceProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDbTraceProperties(properties, b, data);
  };
  static void constructOdDbUnderlayReferenceProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDbUnderlayReferenceProperties(properties, b, data);
  };
  static void constructOdDb2dVertexProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDb2dVertexProperties(properties, b, data);
  };
  static void constructOdDb3dPolylineVertexProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDb3dPolylineVertexProperties(properties, b, data);
  };
  static void constructOdDbViewportProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDbViewportProperties(properties, b, data);
  };
  static void constructOdDbGroupProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDbGroupProperties(properties, b, data);
  };
  static void constructOdDbMaterialProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDbMaterialProperties(properties, b, data);
  };
  static void constructOdDbMlineStyleProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDbMlineStyleProperties(properties, b, data);
  };
  static void constructOdDbModelDocViewStyleProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDbModelDocViewStyleProperties(properties, b, data);
  };
  static void constructOdDbPlotSettingsProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDbPlotSettingsProperties(properties, b, data);
  };
  static void constructOdDbLayoutProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDbLayoutProperties(properties, b, data);
  };
  static void constructOdDbProxyObjectProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDbProxyObjectProperties(properties, b, data);
  };
  static void constructOdDbRenderEnvironmentProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDbRenderEnvironmentProperties(properties, b, data);
  };
  static void constructOdDbRenderGlobalProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDbRenderGlobalProperties(properties, b, data);
  };
  static void constructOdDbRenderSettingsProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDbRenderSettingsProperties(properties, b, data);
  };
  static void constructOdDbMentalRayRenderSettingsProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDbMentalRayRenderSettingsProperties(properties, b, data);
  };
  static void constructOdDbRapidRTRenderSettingsProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDbRapidRTRenderSettingsProperties(properties, b, data);
  };
  static void constructOdDbSymbolTableRecordProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDbSymbolTableRecordProperties(properties, b, data);
  };
  static void constructOdDbAbstractViewTableRecordProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDbAbstractViewTableRecordProperties(properties, b, data);
  };
  static void constructOdDbViewportTableRecordProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDbViewportTableRecordProperties(properties, b, data);
  };
  static void constructOdDbViewTableRecordProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDbViewTableRecordProperties(properties, b, data);
  };
  static void constructOdDbBlockTableRecordProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDbBlockTableRecordProperties(properties, b, data);
  };
  static void constructOdDbDimStyleTableRecordProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDbDimStyleTableRecordProperties(properties, b, data);
  };
  static void constructOdDbLayerTableRecordProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDbLayerTableRecordProperties(properties, b, data);
  };
  static void constructOdDbLinetypeTableRecordProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDbLinetypeTableRecordProperties(properties, b, data);
  };
  static void constructOdDbTextStyleTableRecordProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDbTextStyleTableRecordProperties(properties, b, data);
  };
  static void constructOdDbUCSTableRecordProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDbUCSTableRecordProperties(properties, b, data);
  };
  static void constructOdDbTableStyleProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDbTableStyleProperties(properties, b, data);
  };
  static void constructOdDbUnderlayDefinitionProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDbUnderlayDefinitionProperties(properties, b, data);
  };
  static void constructOdDbDgnDefinitionProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDbDgnDefinitionProperties(properties, b, data);
  };
  static void constructOdDbDwfDefinitionProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDbDwfDefinitionProperties(properties, b, data);
  };
  static void constructOdDbVisualStyleProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDbVisualStyleProperties(properties, b, data);
  };
  static void constructOdDbMLeaderStyleProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDbMLeaderStyleProperties(properties, b, data);
  };
  static void constructOdDbBlockTableProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdDbBlockTableProperties(properties, b, data);
  };
public:
  void initApp()
  {
    ::odrxDynamicLinker()->loadModule(L"RxProperties");
    ::odrxSetMemberConstructor(OdDbDatabase::desc(), constructOdDbDatabaseProperties);
    ::odrxSetMemberConstructor(OdDbObject::desc(), constructOdDbObjectProperties);
    ::odrxSetMemberConstructor(OdDbSymbolTable::desc(), constructOdDbSymbolTableProperties);
    ::odrxSetMemberConstructor(OdDbBody::desc(), constructOdDbBodyProperties);
    ::odrxSetMemberConstructor(OdDbLine::desc(), constructOdDbLineProperties);
    ::odrxSetMemberConstructor(OdDbCurve::desc(), constructOdDbCurveProperties);
    ::odrxSetMemberConstructor(OdDbEntity::desc(), constructOdDbEntityProperties);
    ::odrxSetMemberConstructor(OdDbIBLBackground::desc(), constructOdDbIBLBackgroundProperties);
    ::odrxSetMemberConstructor(OdDbSkyBackground::desc(), constructOdDbSkyBackgroundProperties);
    ::odrxSetMemberConstructor(OdDbDictionary::desc(), constructOdDbDictionaryProperties);
    ::odrxSetMemberConstructor(OdDb3dSolid::desc(), constructOdDb3dSolidProperties);
    ::odrxSetMemberConstructor(OdDbArc::desc(), constructOdDbArcProperties);
    ::odrxSetMemberConstructor(OdDbCircle::desc(), constructOdDbCircleProperties);
    ::odrxSetMemberConstructor(OdDbEllipse::desc(), constructOdDbEllipseProperties);
    ::odrxSetMemberConstructor(OdDbPolyline::desc(), constructOdDbPolylineProperties);
    ::odrxSetMemberConstructor(OdDbBlockReference::desc(), constructOdDbBlockReferenceProperties);
    ::odrxSetMemberConstructor(OdDbMLeader::desc(), constructOdDbMLeaderProperties);
    ::odrxSetMemberConstructor(OdDbTable::desc(), constructOdDbTableProperties);
    ::odrxSetMemberConstructor(OdDbMInsertBlock::desc(), constructOdDbMInsertBlockProperties);
    ::odrxSetMemberConstructor(OdDb2dPolyline::desc(), constructOdDb2dPolylineProperties);
    ::odrxSetMemberConstructor(OdDb3dPolyline::desc(), constructOdDb3dPolylineProperties);
    ::odrxSetMemberConstructor(OdDbRay::desc(), constructOdDbRayProperties);
    ::odrxSetMemberConstructor(OdDbLeader::desc(), constructOdDbLeaderProperties);
    ::odrxSetMemberConstructor(OdDbSpline::desc(), constructOdDbSplineProperties);
    ::odrxSetMemberConstructor(OdDbHelix::desc(), constructOdDbHelixProperties);
    ::odrxSetMemberConstructor(OdDbXline::desc(), constructOdDbXlineProperties);
    ::odrxSetMemberConstructor(OdDbDimension::desc(), constructOdDbDimensionProperties);
    ::odrxSetMemberConstructor(OdDb2LineAngularDimension::desc(), constructOdDb2LineAngularDimensionProperties);
    ::odrxSetMemberConstructor(OdDb3PointAngularDimension::desc(), constructOdDb3PointAngularDimensionProperties);
    ::odrxSetMemberConstructor(OdDbAlignedDimension::desc(), constructOdDbAlignedDimensionProperties);
    ::odrxSetMemberConstructor(OdDbArcDimension::desc(), constructOdDbArcDimensionProperties);
    ::odrxSetMemberConstructor(OdDbDiametricDimension::desc(), constructOdDbDiametricDimensionProperties);
    ::odrxSetMemberConstructor(OdDbOrdinateDimension::desc(), constructOdDbOrdinateDimensionProperties);
    ::odrxSetMemberConstructor(OdDbRadialDimension::desc(), constructOdDbRadialDimensionProperties);
    ::odrxSetMemberConstructor(OdDbRadialDimensionLarge::desc(), constructOdDbRadialDimensionLargeProperties);
    ::odrxSetMemberConstructor(OdDbRotatedDimension::desc(), constructOdDbRotatedDimensionProperties);
    ::odrxSetMemberConstructor(OdDbFace::desc(), constructOdDbFaceProperties);
    ::odrxSetMemberConstructor(OdDbFcf::desc(), constructOdDbFcfProperties);
    ::odrxSetMemberConstructor(OdDbGeoPositionMarker::desc(), constructOdDbGeoPositionMarkerProperties);
    ::odrxSetMemberConstructor(OdDbHatch::desc(), constructOdDbHatchProperties);
    ::odrxSetMemberConstructor(OdDbMline::desc(), constructOdDbMlineProperties);
    ::odrxSetMemberConstructor(OdDbMText::desc(), constructOdDbMTextProperties);
    ::odrxSetMemberConstructor(OdDbOle2Frame::desc(), constructOdDbOle2FrameProperties);
    ::odrxSetMemberConstructor(OdDbPoint::desc(), constructOdDbPointProperties);
    ::odrxSetMemberConstructor(OdDbPolyFaceMesh::desc(), constructOdDbPolyFaceMeshProperties);
    ::odrxSetMemberConstructor(OdDbPolygonMesh::desc(), constructOdDbPolygonMeshProperties);
    ::odrxSetMemberConstructor(OdDbProxyEntity::desc(), constructOdDbProxyEntityProperties);
    ::odrxSetMemberConstructor(OdDbRegion::desc(), constructOdDbRegionProperties);
    ::odrxSetMemberConstructor(OdDbSection::desc(), constructOdDbSectionProperties);
    ::odrxSetMemberConstructor(OdDbShape::desc(), constructOdDbShapeProperties);
    ::odrxSetMemberConstructor(OdDbSolid::desc(), constructOdDbSolidProperties);
    ::odrxSetMemberConstructor(OdDbSubDMesh::desc(), constructOdDbSubDMeshProperties);
    ::odrxSetMemberConstructor(OdDbLight::desc(), constructOdDbLightProperties);
    ::odrxSetMemberConstructor(OdDbSurface::desc(), constructOdDbSurfaceProperties);
    ::odrxSetMemberConstructor(OdDbExtrudedSurface::desc(), constructOdDbExtrudedSurfaceProperties);
    ::odrxSetMemberConstructor(OdDbLoftedSurface::desc(), constructOdDbLoftedSurfaceProperties);
    ::odrxSetMemberConstructor(OdDbNurbSurface::desc(), constructOdDbNurbSurfaceProperties);
    ::odrxSetMemberConstructor(OdDbRevolvedSurface::desc(), constructOdDbRevolvedSurfaceProperties);
    ::odrxSetMemberConstructor(OdDbSweptSurface::desc(), constructOdDbSweptSurfaceProperties);
    ::odrxSetMemberConstructor(OdDbSun::desc(), constructOdDbSunProperties);
    ::odrxSetMemberConstructor(OdDbSolidBackground::desc(), constructOdDbSolidBackgroundProperties);
    ::odrxSetMemberConstructor(OdDbText::desc(), constructOdDbTextProperties);
    ::odrxSetMemberConstructor(OdDbAttribute::desc(), constructOdDbAttributeProperties);
    ::odrxSetMemberConstructor(OdDbAttributeDefinition::desc(), constructOdDbAttributeDefinitionProperties);
    ::odrxSetMemberConstructor(OdDbTrace::desc(), constructOdDbTraceProperties);
    ::odrxSetMemberConstructor(OdDbUnderlayReference::desc(), constructOdDbUnderlayReferenceProperties);
    ::odrxSetMemberConstructor(OdDb2dVertex::desc(), constructOdDb2dVertexProperties);
    ::odrxSetMemberConstructor(OdDb3dPolylineVertex::desc(), constructOdDb3dPolylineVertexProperties);
    ::odrxSetMemberConstructor(OdDbViewport::desc(), constructOdDbViewportProperties);
    ::odrxSetMemberConstructor(OdDbGroup::desc(), constructOdDbGroupProperties);
    ::odrxSetMemberConstructor(OdDbMaterial::desc(), constructOdDbMaterialProperties);
    ::odrxSetMemberConstructor(OdDbMlineStyle::desc(), constructOdDbMlineStyleProperties);
    ::odrxSetMemberConstructor(OdDbModelDocViewStyle::desc(), constructOdDbModelDocViewStyleProperties);
    ::odrxSetMemberConstructor(OdDbPlotSettings::desc(), constructOdDbPlotSettingsProperties);
    ::odrxSetMemberConstructor(OdDbLayout::desc(), constructOdDbLayoutProperties);
    ::odrxSetMemberConstructor(OdDbProxyObject::desc(), constructOdDbProxyObjectProperties);
    ::odrxSetMemberConstructor(OdDbRenderEnvironment::desc(), constructOdDbRenderEnvironmentProperties);
    ::odrxSetMemberConstructor(OdDbRenderGlobal::desc(), constructOdDbRenderGlobalProperties);
    ::odrxSetMemberConstructor(OdDbRenderSettings::desc(), constructOdDbRenderSettingsProperties);
    ::odrxSetMemberConstructor(OdDbMentalRayRenderSettings::desc(), constructOdDbMentalRayRenderSettingsProperties);
    ::odrxSetMemberConstructor(OdDbRapidRTRenderSettings::desc(), constructOdDbRapidRTRenderSettingsProperties);
    ::odrxSetMemberConstructor(OdDbSymbolTableRecord::desc(), constructOdDbSymbolTableRecordProperties);
    ::odrxSetMemberConstructor(OdDbAbstractViewTableRecord::desc(), constructOdDbAbstractViewTableRecordProperties);
    ::odrxSetMemberConstructor(OdDbViewportTableRecord::desc(), constructOdDbViewportTableRecordProperties);
    ::odrxSetMemberConstructor(OdDbViewTableRecord::desc(), constructOdDbViewTableRecordProperties);
    ::odrxSetMemberConstructor(OdDbBlockTableRecord::desc(), constructOdDbBlockTableRecordProperties);
    ::odrxSetMemberConstructor(OdDbDimStyleTableRecord::desc(), constructOdDbDimStyleTableRecordProperties);
    ::odrxSetMemberConstructor(OdDbLayerTableRecord::desc(), constructOdDbLayerTableRecordProperties);
    ::odrxSetMemberConstructor(OdDbLinetypeTableRecord::desc(), constructOdDbLinetypeTableRecordProperties);
    ::odrxSetMemberConstructor(OdDbTextStyleTableRecord::desc(), constructOdDbTextStyleTableRecordProperties);
    ::odrxSetMemberConstructor(OdDbUCSTableRecord::desc(), constructOdDbUCSTableRecordProperties);
    ::odrxSetMemberConstructor(OdDbTableStyle::desc(), constructOdDbTableStyleProperties);
    ::odrxSetMemberConstructor(OdDbUnderlayDefinition::desc(), constructOdDbUnderlayDefinitionProperties);
    ::odrxSetMemberConstructor(OdDbDgnDefinition::desc(), constructOdDbDgnDefinitionProperties);
    ::odrxSetMemberConstructor(OdDbDwfDefinition::desc(), constructOdDbDwfDefinitionProperties);
    ::odrxSetMemberConstructor(OdDbVisualStyle::desc(), constructOdDbVisualStyleProperties);
    ::odrxSetMemberConstructor(OdDbMLeaderStyle::desc(), constructOdDbMLeaderStyleProperties);
    ::odrxSetMemberConstructor(OdDbBlockTable::desc(), constructOdDbBlockTableProperties);
  }
  void uninitApp()
  {
    for (unsigned i = 0; i < properties.size(); ++i)
      OdRxMember::deleteMember(properties[i]);
    properties.clear();
  }
};
OdArray<OdRxMemberPtr> OdDbPropertiesModule::properties;

ODRX_DEFINE_DYNAMIC_MODULE(OdDbPropertiesModule);
DISABLE_THREAD_LIBRARY_CALLS()

