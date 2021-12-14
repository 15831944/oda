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

#include "DgElementProperties.h"
#include "DgDatabaseProperties.h"
#include "DgPropertiesModule.h"
#include "DgGraphicsElmProperties.h"
#include "DgTableProperties.h"
#include "DgCurveElementProperties.h"
#include "DgLevelTableRecord.h"
#include "DgSharedCellDefinition.h"
#include "DgExtraProperties.h"
#include "DgComplexCurve.h"
#include "DgSharedCellReference.h"
#include "DgReferenceAttach.h"

//-----------------------------------------------------------------------------------------------------------------------

void OdDgPropertiesModule::constructOdDgElementProperties(OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDgElementElementIdProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgElementODAUniqueIDProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgElementElementTypeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgElementElementSubTypeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgElementNameProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgElementModificationTimeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgElementNewProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgElementModifiedProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgElementLockedProperty::createObject(b.owner()));
  b.add(properties.last());
}

//-----------------------------------------------------------------------------------------------------------------------

void OdDgPropertiesModule::constructOdDgDatabaseProperties(OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDgDatabaseLevelTableIdProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgDatabaseLevelFilterTableIdProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgDatabaseFontTableIdProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgDatabaseTextStyleTableIdProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgDatabaseLineStyleTableIdProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgDatabaseModelsProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgDatabaseMajorVersionProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgDatabaseMinorVersionProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgDatabaseFileNameProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgDatabaseColorPaletteProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgDatabaseMaterialTableIdProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgDatabaseDisplayStyleTableIdProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgDatabaseSharedCellDefinitionsProperty::createObject(b.owner()));
  b.add(properties.last());
}

//-----------------------------------------------------------------------------------------------------------------------

void OdDgPropertiesModule::constructOdDgGraphicsElementProperties(OdRxMemberCollectionBuilder& b, void*)
{
  //properties.append(OdDgGraphicsElementColorIndexProperty::createObject(b.owner()));
  //b.add(properties.last());
  properties.append(OdDgGraphicsElementColorProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgGraphicsElementGraphicsGroupIndexProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgGraphicsElementClassProperty::createObject(b.owner()));
  b.add(properties.last());
  //properties.append(OdDgGraphicsElementLevelIndexProperty::createObject(b.owner()));
  //b.add(properties.last());
  properties.append(OdDgGraphicsElementLevelProperty::createObject(b.owner()));
  b.add(properties.last());
  //properties.append(OdDgGraphicsElementLevelNameProperty::createObject(b.owner()));
  //b.add(properties.last());
  //properties.append(OdDgGraphicsElementLineStyleIndexProperty::createObject(b.owner()));
  //b.add(properties.last());
  properties.append(OdDgGraphicsElementLineStyleProperty::createObject(b.owner()));
  b.add(properties.last());
  //properties.append(OdDgGraphicsElementLineStyleNameProperty::createObject(b.owner()));
  //b.add(properties.last());
  properties.append(OdDgGraphicsElementLineWeightProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgGraphicsElementThicknessProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgGraphicsElementPriorityProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgGraphicsElementInvisibleProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgGraphicsElement3DProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgGraphicsElementViewIndependentProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgGraphicsElementNonPlanarProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgGraphicsElementNotSnappableProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgGraphicsElementHBitProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgGraphicsElementTransparencyProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgGraphicsElementExtentsProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgGraphicsElementDisplayStyleProperty::createObject(b.owner()));
  b.add(properties.last());
  //properties.append(OdDgGraphicsElementDisplayStyleNameProperty::createObject(b.owner()));
  //b.add(properties.last());
	properties.append(OdDgGraphicsElementItemTypeInstancesProperty::createObject(b.owner()));
	b.add(properties.last());
}

//-----------------------------------------------------------------------------------------------------------------------

void OdDgPropertiesModule::constructOdDgLevelTableProperties(OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDgLevelTableItemsProperty::createObject(b.owner()));
  b.add(properties.last());
}

//-----------------------------------------------------------------------------------------------------------------------

void OdDgPropertiesModule::constructOdDgLineStyleTableProperties(OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDgLineStyleTableItemsProperty::createObject(b.owner()));
  b.add(properties.last());
}

//-----------------------------------------------------------------------------------------------------------------------

void OdDgPropertiesModule::constructOdDgMaterialTableProperties(OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDgMaterialTableItemsProperty::createObject(b.owner()));
  b.add(properties.last());
}

//-----------------------------------------------------------------------------------------------------------------------

void OdDgPropertiesModule::constructOdDgModelTableProperties(OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDgModelTableItemsProperty::createObject(b.owner()));
  b.add(properties.last());
}

//-----------------------------------------------------------------------------------------------------------------------

void OdDgPropertiesModule::constructOdDgDisplayStyleTableProperties(OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDgDisplayStyleTableItemsProperty::createObject(b.owner()));
  b.add(properties.last());
}

//-----------------------------------------------------------------------------------------------------------------------

void OdDgPropertiesModule::constructOdDgLevelProperties(OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDgLevelTableRecordIndexProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgLevelTableRecordNameProperty::createObject(b.owner()));
  b.add(properties.last());
}

//-----------------------------------------------------------------------------------------------------------------------

void OdDgPropertiesModule::constructOdDgLineStyleProperties(OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDgLineStyleTableRecordIndexProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgLineStyleTableRecordNameProperty::createObject(b.owner()));
  b.add(properties.last());
}

//-----------------------------------------------------------------------------------------------------------------------

void OdDgPropertiesModule::constructOdDgMaterialProperties(OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDgMaterialTableRecordNameProperty::createObject(b.owner()));
  b.add(properties.last());
}

//-----------------------------------------------------------------------------------------------------------------------

void OdDgPropertiesModule::constructOdDgDisplayStyleProperties(OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDgDisplayStyleTableRecordIndexProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgDisplayStyleTableRecordNameProperty::createObject(b.owner()));
  b.add(properties.last());
}

//-----------------------------------------------------------------------------------------------------------------------

void OdDgPropertiesModule::constructOdDgCurveElement2dProperties(OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDgCurveElement2dLineStyleScaleProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgCurveElement2dLineStyleModifiersProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgCurveElement2dClosedProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgCurveElement2dPeriodicProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgCurveElement2dStartParamProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgCurveElement2dEndParamProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgCurveElement2dStartPointProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgCurveElement2dEndPointProperty::createObject(b.owner()));
  b.add(properties.last());
}

//-----------------------------------------------------------------------------------------------------------------------

void OdDgPropertiesModule::constructOdDgCurveElement3dProperties(OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDgCurveElement3dLineStyleScaleProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgCurveElement3dLineStyleModifiersProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgCurveElement3dClosedProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgCurveElement3dPeriodicProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgCurveElement3dStartParamProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgCurveElement3dEndParamProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgCurveElement3dStartPointProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgCurveElement3dEndPointProperty::createObject(b.owner()));
  b.add(properties.last());
}

//-----------------------------------------------------------------------------------------------------------------------

void OdDgPropertiesModule::constructOdDgLine2dProperties(OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDgLine2dStartPointProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgLine2dEndPointProperty::createObject(b.owner()));
  b.add(properties.last());
}

//-----------------------------------------------------------------------------------------------------------------------

void OdDgPropertiesModule::constructOdDgLine3dProperties(OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDgLine3dStartPointProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgLine3dEndPointProperty::createObject(b.owner()));
  b.add(properties.last());
}

//-----------------------------------------------------------------------------------------------------------------------

void OdDgPropertiesModule::constructOdDgArc2dProperties(OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDgArc2dPrimaryAxisProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgArc2dSecondaryAxisProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgArc2dPrimaryAxisProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgArc2dRotationAngleProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgArc2dOriginProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgArc2dStartAngleProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgArc2dSweepAngleProperty::createObject(b.owner()));
  b.add(properties.last());
}

//-----------------------------------------------------------------------------------------------------------------------

void OdDgPropertiesModule::constructOdDgArc3dProperties(OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDgArc3dPrimaryAxisProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgArc3dSecondaryAxisProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgArc3dPrimaryAxisProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgArc3dRotationProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgArc3dNormalProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgArc3dOriginProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgArc3dStartAngleProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgArc3dSweepAngleProperty::createObject(b.owner()));
  b.add(properties.last());
}

//-----------------------------------------------------------------------------------------------------------------------

void OdDgPropertiesModule::constructOdDgCellHeader2dProperties(OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDgCellHeader2dNameProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgCellHeader2dColorProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgCellHeader2dLevelProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgCellHeader2dLineStyleProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgCellHeader2dLineWeightProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgCellHeader2dOriginProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgCellHeader2dTransformationProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgCellHeader2dElementsProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgCellHeader2dGroupedHoleProperty::createObject(b.owner()));
  b.add(properties.last());
}

//-----------------------------------------------------------------------------------------------------------------------

void OdDgPropertiesModule::constructOdDgCellHeader3dProperties(OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDgCellHeader3dNameProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgCellHeader3dColorProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgCellHeader3dLevelProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgCellHeader3dLineStyleProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgCellHeader3dLineWeightProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgCellHeader3dOriginProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgCellHeader3dTransformationProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgCellHeader3dElementsProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgCellHeader3dGroupedHoleProperty::createObject(b.owner()));
  b.add(properties.last());
}

//-----------------------------------------------------------------------------------------------------------------------

void OdDgPropertiesModule::constructOdDgShape2dProperties(OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDgShape2dVerticesProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgShape2dMaterialProperty::createObject(b.owner()));
  b.add(properties.last());
  //properties.append(OdDgShape2dMaterialNameProperty::createObject(b.owner()));
  //b.add(properties.last());
  properties.append(OdDgShape2dFillProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgShape2dHatchPatternProperty::createObject(b.owner()));
  b.add(properties.last());
}

//-----------------------------------------------------------------------------------------------------------------------

void OdDgPropertiesModule::constructOdDgShape3dProperties(OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDgShape3dVerticesProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgShape3dMaterialProperty::createObject(b.owner()));
  b.add(properties.last());
  //properties.append(OdDgShape3dMaterialNameProperty::createObject(b.owner()));
  //b.add(properties.last());
  properties.append(OdDgShape3dFillProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgShape3dHatchPatternProperty::createObject(b.owner()));
  b.add(properties.last());
}

//-----------------------------------------------------------------------------------------------------------------------

void OdDgPropertiesModule::constructOdDgSharedCellDefinitionProperties(OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDgSharedCellDefinitionNameProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgSharedCellDefinitionElementsProperty::createObject(b.owner()));
  b.add(properties.last());
}

//-----------------------------------------------------------------------------------------------------------------------

void OdDgPropertiesModule::constructOdDgSharedCellReferenceProperties(OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDgSharedCellReferenceBlockDefinitionProperty::createObject(b.owner()));
  b.add(properties.last());
}

//-----------------------------------------------------------------------------------------------------------------------

void OdDgPropertiesModule::constructOdDgReferenceAttachmentHeaderProperties(OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDgReferenceAttachmentHeaderReferencedModelProperty::createObject(b.owner()));
  b.add(properties.last());
}

//-----------------------------------------------------------------------------------------------------------------------

void OdDgPropertiesModule::constructOdDgComplexCurveProperties(OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDgComplexCurveElementsProperty::createObject(b.owner()));
  b.add(properties.last());
}

//-----------------------------------------------------------------------------------------------------------------------

void OdDgPropertiesModule::constructOdDgModelProperties(OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDgModelNameProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgModelElementsProperty::createObject(b.owner()));
  b.add(properties.last());
}

//-----------------------------------------------------------------------------------------------------------------------

void OdDgPropertiesModule::constructOdDgHatchPatternProperties(OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDgHatchPatternPatternTypeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgHatchPatternToleranceProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgHatchPatternUseToleranceProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgHatchPatternRotationProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgHatchPattern2dRotationProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgHatchPatternUseRotationProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgHatchPatternLowMultilineIndexProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgHatchPatternHighMultilineIndexProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgHatchPatternUseMultilineIndicesProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgHatchPatternOffsetProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgHatchPatternUseOffsetProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgHatchPatternSnappableProperty::createObject(b.owner()));
  b.add(properties.last());
}

//-----------------------------------------------------------------------------------------------------------------------

void OdDgPropertiesModule::constructOdDgLinearHatchPatternProperties(OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDgLinearHatchPatternSpaceProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgLinearHatchPatternAngleProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgLinearHatchPatternLineStyleEntryIdProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgLinearHatchPatternUseLineStyleProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgLinearHatchPatternLineWeightProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgLinearHatchPatternUseLineWeightProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgLinearHatchPatternLineColorProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgLinearHatchPatternUseLineColorProperty::createObject(b.owner()));
  b.add(properties.last());
}

//-----------------------------------------------------------------------------------------------------------------------

void OdDgPropertiesModule::constructOdDgCrossHatchPatternProperties(OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDgCrossHatchPatternSpace1Property::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgCrossHatchPatternSpace2Property::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgCrossHatchPatternAngle1Property::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgCrossHatchPatternAngle2Property::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgCrossHatchPatternLineStyleEntryIdProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgCrossHatchPatternUseLineStyleProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgCrossHatchPatternLineWeightProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgCrossHatchPatternUseLineWeightProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgCrossHatchPatternLineColorProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgCrossHatchPatternUseLineColorProperty::createObject(b.owner()));
  b.add(properties.last());
}

//-----------------------------------------------------------------------------------------------------------------------

void OdDgPropertiesModule::constructOdDgSymbolHatchPatternProperties(OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDgSymbolHatchPatternSpace1Property::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgSymbolHatchPatternSpace2Property::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgSymbolHatchPatternAngle1Property::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgSymbolHatchPatternAngle2Property::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgSymbolHatchPatternLineStyleEntryIdProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgSymbolHatchPatternUseLineStyleProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgSymbolHatchPatternLineWeightProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgSymbolHatchPatternUseLineWeightProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgSymbolHatchPatternColorProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgSymbolHatchPatternUseColorProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgSymbolHatchPatternTrueScaleProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgSymbolHatchPatternSymbolIdProperty::createObject(b.owner()));
  b.add(properties.last());
}

//-----------------------------------------------------------------------------------------------------------------------

void OdDgPropertiesModule::constructOdDgDWGHatchPatternProperties(OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDgDWGHatchPatternPatternNameProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDgDWGHatchPatternDwgLinesProperty::createObject(b.owner()));
  b.add(properties.last());
}

//-----------------------------------------------------------------------------------------------------------------------

void OdDgPropertiesModule::initApp()
{
  ::odrxDynamicLinker()->loadModule(L"RxProperties");
  ::odrxSetMemberConstructor(OdDgElement::desc(), constructOdDgElementProperties);
  ::odrxSetMemberConstructor(OdDgDatabase::desc(), constructOdDgDatabaseProperties);
  ::odrxSetMemberConstructor(OdDgGraphicsElement::desc(), constructOdDgGraphicsElementProperties);
  ::odrxSetMemberConstructor(OdDgLevelTable::desc(), constructOdDgLevelTableProperties);
  ::odrxSetMemberConstructor(OdDgLineStyleTable::desc(), constructOdDgLineStyleTableProperties);
  ::odrxSetMemberConstructor(OdDgMaterialTable::desc(), constructOdDgMaterialTableProperties);
  ::odrxSetMemberConstructor(OdDgModelTable::desc(), constructOdDgModelTableProperties);
  ::odrxSetMemberConstructor(OdDgDisplayStyleTable::desc(), constructOdDgDisplayStyleTableProperties);
  ::odrxSetMemberConstructor(OdDgLevelTableRecord::desc(), constructOdDgLevelProperties);
  ::odrxSetMemberConstructor(OdDgLineStyleTableRecord::desc(), constructOdDgLineStyleProperties);
  ::odrxSetMemberConstructor(OdDgMaterialTableRecord::desc(), constructOdDgMaterialProperties);
  ::odrxSetMemberConstructor(OdDgDisplayStyleTableRecord::desc(), constructOdDgDisplayStyleProperties);
  ::odrxSetMemberConstructor(OdDgCurveElement2d::desc(), constructOdDgCurveElement2dProperties);
  ::odrxSetMemberConstructor(OdDgCurveElement3d::desc(), constructOdDgCurveElement3dProperties);
  ::odrxSetMemberConstructor(OdDgLine2d::desc(), constructOdDgLine2dProperties);
  ::odrxSetMemberConstructor(OdDgLine3d::desc(), constructOdDgLine3dProperties);
  ::odrxSetMemberConstructor(OdDgArc2d::desc(), constructOdDgArc2dProperties);
  ::odrxSetMemberConstructor(OdDgArc3d::desc(), constructOdDgArc3dProperties);
  ::odrxSetMemberConstructor(OdDgCellHeader2d::desc(), constructOdDgCellHeader2dProperties);
  ::odrxSetMemberConstructor(OdDgCellHeader3d::desc(), constructOdDgCellHeader3dProperties);
  ::odrxSetMemberConstructor(OdDgShape2d::desc(), constructOdDgShape2dProperties);
  ::odrxSetMemberConstructor(OdDgShape3d::desc(), constructOdDgShape3dProperties);
  ::odrxSetMemberConstructor(OdDgSharedCellDefinition::desc(), constructOdDgSharedCellDefinitionProperties);
  ::odrxSetMemberConstructor(OdDgHatchPattern::desc(), constructOdDgHatchPatternProperties);
  ::odrxSetMemberConstructor(OdDgLinearHatchPattern::desc(), constructOdDgLinearHatchPatternProperties);
  ::odrxSetMemberConstructor(OdDgCrossHatchPattern::desc(), constructOdDgCrossHatchPatternProperties);
  ::odrxSetMemberConstructor(OdDgSymbolHatchPattern::desc(), constructOdDgSymbolHatchPatternProperties);
  ::odrxSetMemberConstructor(OdDgDWGHatchPattern::desc(), constructOdDgDWGHatchPatternProperties);
  ::odrxSetMemberConstructor(OdDgModel::desc(), constructOdDgModelProperties);
  ::odrxSetMemberConstructor(OdDgSharedCellReference::desc(), constructOdDgSharedCellReferenceProperties);
  ::odrxSetMemberConstructor(OdDgReferenceAttachmentHeader::desc(), constructOdDgReferenceAttachmentHeaderProperties);
  ::odrxSetMemberConstructor(OdDgComplexCurve::desc(), constructOdDgComplexCurveProperties);
}

//-----------------------------------------------------------------------------------------------------------------------

void OdDgPropertiesModule::uninitApp()
{
  for (unsigned i = 0; i < properties.size(); ++i)
    OdRxMember::deleteMember(properties[i]);
  properties.clear();
}

//-----------------------------------------------------------------------------------------------------------------------


