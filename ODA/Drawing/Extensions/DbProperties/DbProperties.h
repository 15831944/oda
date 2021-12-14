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

#pragma once

#include "RxMember.h"

extern void createOdDbDatabaseProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDbObjectProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDbSymbolTableProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDbBodyProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDbLineProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDbCurveProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDbEntityProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDbIBLBackgroundProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDbSkyBackgroundProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDbDictionaryProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDb3dSolidProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDbArcProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDbCircleProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDbEllipseProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDbPolylineProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDbBlockReferenceProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDbMLeaderProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDbTableProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDbMInsertBlockProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDb2dPolylineProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDb3dPolylineProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDbRayProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDbLeaderProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDbSplineProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDbHelixProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDbXlineProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDbDimensionProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDb2LineAngularDimensionProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDb3PointAngularDimensionProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDbAlignedDimensionProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDbArcDimensionProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDbDiametricDimensionProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDbOrdinateDimensionProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDbRadialDimensionProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDbRadialDimensionLargeProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDbRotatedDimensionProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDbFaceProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDbFcfProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDbGeoPositionMarkerProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDbHatchProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDbMlineProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDbMTextProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDbOle2FrameProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDbPointProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDbPolyFaceMeshProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDbPolygonMeshProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDbProxyEntityProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDbRegionProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDbSectionProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDbShapeProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDbSolidProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDbSubDMeshProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDbLightProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDbSurfaceProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDbExtrudedSurfaceProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDbLoftedSurfaceProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDbNurbSurfaceProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDbRevolvedSurfaceProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDbSweptSurfaceProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDbSunProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDbSolidBackgroundProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDbTextProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDbAttributeProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDbAttributeDefinitionProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDbTraceProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDbUnderlayReferenceProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDb2dVertexProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDb3dPolylineVertexProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDbViewportProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDbGroupProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDbMaterialProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDbMlineStyleProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDbModelDocViewStyleProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDbPlotSettingsProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDbLayoutProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDbProxyObjectProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDbRenderEnvironmentProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDbRenderGlobalProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDbRenderSettingsProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDbMentalRayRenderSettingsProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDbRapidRTRenderSettingsProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDbSymbolTableRecordProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDbAbstractViewTableRecordProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDbViewportTableRecordProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDbViewTableRecordProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDbBlockTableRecordProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDbDimStyleTableRecordProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDbLayerTableRecordProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDbLinetypeTableRecordProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDbTextStyleTableRecordProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDbUCSTableRecordProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDbTableStyleProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDbUnderlayDefinitionProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDbDgnDefinitionProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDbDwfDefinitionProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDbVisualStyleProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDbMLeaderStyleProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
extern void createOdDbBlockTableProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*);
