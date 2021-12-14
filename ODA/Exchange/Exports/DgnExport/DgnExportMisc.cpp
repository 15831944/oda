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

#include <OdaCommon.h>
#include "DgnExportMisc.h"
#include "DgnExportCommon.h"
#include "DgnExportContext.h"
#include "DgnExportBlockRef.h"
#include "DgnExportText.h"
#include <DgTableElement.h>
#include <DbTable.h>
#include <DbMText.h>
#include <DgTextNode.h>
#include <DbTextStyleTableRecord.h>
#include <DbLight.h>
#include <DgLight.h>
#include <DbBlockTableRecord.h>

namespace TD_DGN_EXPORT {

//---------------------------------------------------------------------------------------------------

void recomputeTeble(OdDbTable* pDbTable)
{
  ::odrxDynamicLinker()->loadModule(OdExFieldEvaluatorModuleName);
  pDbTable->upgradeOpen();
  pDbTable->recomputeTableBlock(true);
  pDbTable->downgradeOpen();
  ::odrxDynamicLinker()->unloadModule(OdExFieldEvaluatorModuleName);
}

//---------------------------------------------------------------------------------------------------

void OdDbTableExportPE::subExportElement(OdDbEntity* pDbEntity, OdDgElement* owner)
{
  OdDbTable* pDbTable = (OdDbTable*)(pDbEntity);

  if( !pDbTable || !owner )
    return;

  bool bExportAsBlock = false;
  bool bRecomputeTable = pDbTable->hasFields();
  OdUInt32 i, j;

  for ( i = 0; i < pDbTable->numRows(); i++)
  {
    for( j = 0; j < pDbTable->numColumns(); j++ )
    {
      if(pDbTable->numContents(i, j) > 1 ||
        ((pDbTable->numContents(i, j) == 1) && (pDbTable->contentType(i, j, 0) == OdDb::kCellContentTypeBlock)) ||
        (pDbTable->flowDirection() == OdDb::kBtoT)
        )
      {
        bExportAsBlock = true;
        break;
      }

      if (!bRecomputeTable && (pDbTable->numContents(i, j) == 1) && pDbTable->hasFormula(i, j, 0))
        bRecomputeTable = true;
    }
  }

  if( bExportAsBlock )
  {
    if (!exportBlockReference(pDbTable, owner))
    {
      OdDgnExportPE::subExportElement(pDbTable, owner);
    }
    return;
  }

  if( bRecomputeTable )
    recomputeTeble(pDbTable);

  // Main table

  OdDgTableElementPtr pDgTable = OdDgTableElement::createObject();
  pDgTable->setDatabaseDefaults(owner->database());

  for (i = 0; i < pDbTable->numRows(); i++)
  {
    pDgTable->addRow();
    pDgTable->getRow(i)->setHeight(pDbTable->rowHeight(i));
  }

  for (i = 0; i < pDbTable->numColumns(); i++)
  {
    pDgTable->addColumn();
    pDgTable->getColumn(i)->setWidth(pDbTable->columnWidth(i));
  }

  OdGePoint3d ptOrigin = pDbTable->position();

  OdGeVector3d vrNormal    = pDbTable->normal();
  OdGeVector3d vrRowDir    = pDbTable->direction();
  OdGeVector3d vrColumnDir = vrNormal.crossProduct(vrRowDir);

  if( !vrNormal.isZeroLength() )
    vrNormal.normalize();

  if (!vrRowDir.isZeroLength())
    vrRowDir.normalize();

  if (!vrColumnDir.isZeroLength())
    vrColumnDir.normalize();

  OdGeMatrix3d matRotation;
  matRotation.setCoordSystem(OdGePoint3d::kOrigin, vrRowDir, vrColumnDir, vrNormal);

  pDgTable->setOrigin(pDbTable->position());
  pDgTable->setRotation(matRotation);

  if( pDbTable->isHeaderSuppressed() )
    pDgTable->setHeaderRowCount(0);
  else
    pDgTable->setHeaderRowCount(1);

  if( pDbTable->isTitleSuppressed() )
    pDgTable->setTitleRowCount(0);
  else
    pDgTable->setTitleRowCount(1);

  pDgTable->setFooterRowCount(0);
  pDgTable->setFirstColumnsCount(0);
  pDgTable->setLastColumnsCount(0);

  OdDbObjectId idDbBodyTextStyle   = pDbTable->textStyle(OdDb::kDataRow);
  OdDbObjectId idDbHeaderTextStyle = pDbTable->textStyle(OdDb::kHeaderRow);
  OdDbObjectId idDbTitleTextStyle  = pDbTable->textStyle(OdDb::kTitleRow);

  OdUInt32 uBodyTextStyle   = getDgnTextStyleIndex(idDbBodyTextStyle);
  OdUInt32 uHeaderTextStyle = getDgnTextStyleIndex(idDbHeaderTextStyle);
  OdUInt32 uTitleTextStyle  = getDgnTextStyleIndex(idDbTitleTextStyle);

  pDgTable->setBodyTextStyle(uBodyTextStyle);

  if( uHeaderTextStyle != 0 )
    pDgTable->setHeaderRowTextStyle(uHeaderTextStyle);

  if (uTitleTextStyle != 0)
    pDgTable->setTitleRowTextStyle(uTitleTextStyle);

  OdDb::TableBreakOption breakData = pDbTable->breakOption();

  if (breakData & OdDb::kTableBreakEnableBreaking)
  {
    pDgTable->setBreakType(OdDgTableElement::kHorizontalBreak);
    pDgTable->setRepeatHeadersFlag(breakData & OdDb::kTableBreakRepeatTopLabels ? true : false);
    pDgTable->setRepeatFootersFlag(breakData & OdDb::kTableBreakRepeatBottomLabels ? true : false);
  }
  else
    pDgTable->setBreakType(OdDgTableElement::kNoBreak);

  pDgTable->setSubTableSpacing(pDbTable->breakSpacing());
  pDgTable->setBreakHeight(pDbTable->breakHeight(0));

  if( breakData & OdDb::kTableBreakAllowManualPositions )
  {
    pDgTable->setSubTablePosition(OdDgTableElement::kManual);
  }
  else
  {
    switch (pDbTable->breakFlowDirection())
    {
      case OdDb::kTableBreakFlowRight: pDgTable->setSubTablePosition(OdDgTableElement::kRight); break;
      case OdDb::kTableBreakFlowDownOrUp: pDgTable->setSubTablePosition(OdDgTableElement::kBelow); break;
      case OdDb::kTableBreakFlowLeft: pDgTable->setSubTablePosition(OdDgTableElement::kLeft); break;
    }
  }

  OdArray<OdArray<bool> >         arrMergeStatus;

  for (i = 0; i < pDbTable->numRows(); i++)
  {
    OdArray<bool> arrMergeRow;
    arrMergeRow.resize(pDbTable->numColumns(), false);
    arrMergeStatus.push_back(arrMergeRow);
  }

  for (i = 0; i < pDbTable->numRows(); i++)
  {
    for (j = 0; j < pDbTable->numColumns(); j++)
    {
      if (arrMergeStatus[i][j])
        continue;

      OdUInt32 minRow, maxRow, minCol, maxCol;

      if( pDbTable->isMergedCell(i, j, &minRow, &maxRow, &minCol, &maxCol) )
      {
        for( OdUInt32 k = minRow; k <= maxRow; k++ )
        {
          for (OdUInt32 l = minCol; l <= maxCol; l++)
            arrMergeStatus[k][l] = true;
        }

        OdDgTableElementMerge mergeData;
        mergeData.setBaseRowIndex(minRow);
        mergeData.setBaseColumnIndex(minCol);
        mergeData.setRowCount(maxRow - minRow + 1);
        mergeData.setColumnCount(maxCol - minCol + 1);

        pDgTable->addMerge(mergeData);
      }
    }
  }

  // Cells

  for (i = 0; i < pDbTable->numRows(); i++)
  {
    for (j = 0; j < pDbTable->numColumns(); j++)
    {
      OdDgTableCellElementPtr pDgCell = pDgTable->getCell(i, j);

      pDgCell->setShowTextFlag(true);
      pDgCell->setFillFlag(false);

      if( !pDbTable->isBackgroundColorNone(i, j))
      {
        pDgCell->setFillFlag(true);
        pDgCell->setFillColor(getDgnColorIndex(pDbTable->backgroundColor(i, j), owner->database()));
      }

      OdCmColor        topColor      = pDbTable->gridColor(i, j, OdDb::kHorzTop);
      OdDb::LineWeight topLineWeight = pDbTable->gridLineWeight(i, j, OdDb::kHorzTop);
      OdDbObjectId     topLineType   = pDbTable->gridLinetype(i, j, OdDb::kHorzTop);
      bool             topVisibility = !pDbTable->gridVisibility(i, j, OdDb::kHorzTop);

      OdCmColor        bottomColor = pDbTable->gridColor(i, j, OdDb::kHorzBottom);
      OdDb::LineWeight bottomLineWeight = pDbTable->gridLineWeight(i, j, OdDb::kHorzBottom);
      OdDbObjectId     bottomLineType = pDbTable->gridLinetype(i, j, OdDb::kHorzBottom);
      bool             bottomVisibility = !pDbTable->gridVisibility(i, j, OdDb::kHorzBottom);

      OdCmColor        leftColor = pDbTable->gridColor(i, j, OdDb::kVertLeft);
      OdDb::LineWeight leftLineWeight = pDbTable->gridLineWeight(i, j, OdDb::kVertLeft);
      OdDbObjectId     leftLineType = pDbTable->gridLinetype(i, j, OdDb::kVertLeft);
      bool             leftVisibility = !pDbTable->gridVisibility(i, j, OdDb::kVertLeft);

      OdCmColor        rightColor = pDbTable->gridColor(i, j, OdDb::kVertRight);
      OdDb::LineWeight rightLineWeight = pDbTable->gridLineWeight(i, j, OdDb::kVertRight);
      OdDbObjectId     rightLineType = pDbTable->gridLinetype(i, j, OdDb::kVertRight);
      bool             rightVisibility = !pDbTable->gridVisibility(i, j, OdDb::kVertRight);

      OdDbObjectId idLTByBlock = pDbTable->database()->getLinetypeByBlockId();

      OdDgTableCellSymbology topSymbology;
      OdDgTableCellSymbology bottomSymbology;
      OdDgTableCellSymbology leftSymbology;
      OdDgTableCellSymbology rightSymbology;

      topSymbology.setLineVisibility(topVisibility);

      if( !topColor.isByBlock() )
      {
        topSymbology.setUseTableColorFlag(false);
        topSymbology.setColorIndex(getDgnColorIndex(topColor, owner->database()));
      }
      else
        topSymbology.setUseTableColorFlag(true);

      if ( topLineWeight != OdDb::kLnWtByBlock )
      {
        topSymbology.setUseTableLineWeightFlag(false);
        topSymbology.setLineWeight(odDgnExportLineweightMapping(topLineWeight));
      }
      else
        topSymbology.setUseTableLineWeightFlag(true);

      if (topLineType != idLTByBlock)
      {
        topSymbology.setUseTableLineStyleFlag(false);
        topSymbology.setLineStyle(getDgnLineStyleEntryId(topLineType));
      }
      else
        topSymbology.setUseTableLineStyleFlag(true);

      bottomSymbology.setLineVisibility(bottomVisibility);

      if (!bottomColor.isByBlock())
      {
        bottomSymbology.setUseTableColorFlag(false);
        bottomSymbology.setColorIndex(getDgnColorIndex(bottomColor, owner->database()));
      }
      else
        bottomSymbology.setUseTableColorFlag(true);

      if (bottomLineWeight != OdDb::kLnWtByBlock)
      {
        bottomSymbology.setUseTableLineWeightFlag(false);
        bottomSymbology.setLineWeight(odDgnExportLineweightMapping(bottomLineWeight));
      }
      else
        bottomSymbology.setUseTableLineWeightFlag(true);

      if (bottomLineType != idLTByBlock)
      {
        bottomSymbology.setUseTableLineStyleFlag(false);
        bottomSymbology.setLineStyle(getDgnLineStyleEntryId(bottomLineType));
      }
      else
        bottomSymbology.setUseTableLineStyleFlag(true);

      leftSymbology.setLineVisibility(leftVisibility);

      if (!leftColor.isByBlock())
      {
        leftSymbology.setUseTableColorFlag(false);
        leftSymbology.setColorIndex(getDgnColorIndex(leftColor, owner->database()));
      }
      else
        leftSymbology.setUseTableColorFlag(true);

      if (leftLineWeight != OdDb::kLnWtByBlock)
      {
        leftSymbology.setUseTableLineWeightFlag(false);
        leftSymbology.setLineWeight(odDgnExportLineweightMapping(leftLineWeight));
      }
      else
        leftSymbology.setUseTableLineWeightFlag(true);

      if (leftLineType != idLTByBlock)
      {
        leftSymbology.setUseTableLineStyleFlag(false);
        leftSymbology.setLineStyle(getDgnLineStyleEntryId(leftLineType));
      }
      else
        leftSymbology.setUseTableLineStyleFlag(true);

      rightSymbology.setLineVisibility(rightVisibility);

      if (!rightColor.isByBlock())
      {
        rightSymbology.setUseTableColorFlag(false);
        rightSymbology.setColorIndex(getDgnColorIndex(rightColor, owner->database()));
      }
      else
        rightSymbology.setUseTableColorFlag(true);

      if (rightLineWeight != OdDb::kLnWtByBlock)
      {
        rightSymbology.setUseTableLineWeightFlag(false);
        rightSymbology.setLineWeight(odDgnExportLineweightMapping(rightLineWeight));
      }
      else
        rightSymbology.setUseTableLineWeightFlag(true);

      if (rightLineType != idLTByBlock)
      {
        rightSymbology.setUseTableLineStyleFlag(false);
        rightSymbology.setLineStyle(getDgnLineStyleEntryId(rightLineType));
      }
      else
        rightSymbology.setUseTableLineStyleFlag(true);

      pDgCell->setTopBorder(topSymbology);
      pDgCell->setBottomBorder(bottomSymbology);
      pDgCell->setLeftBorder(leftSymbology);
      pDgCell->setRightBorder(rightSymbology);

      pDgCell->setCellMarginType(OdDgTableCellElement::kNone);

      switch (pDbTable->textRotation(i, j))
      {
        case OdDb::kDegrees000:
        case OdDb::kDegrees180: pDgCell->setTextOrientation(OdDgTableCellElement::kHorizontal); break;
        case OdDb::kDegrees090: pDgCell->setTextOrientation(OdDgTableCellElement::kRotateRight); break;
        case OdDb::kDegrees270: pDgCell->setTextOrientation(OdDgTableCellElement::kRotateLeft); break;
      }

      switch( pDbTable->alignment(i, j) )
      {
        case OdDb::kTopLeft: pDgCell->setTextAlignment(OdDgTableCellElement::kLeftTop); break;
        case OdDb::kMiddleLeft: pDgCell->setTextAlignment(OdDgTableCellElement::kLeftMiddle); break;
        case OdDb::kBottomLeft: pDgCell->setTextAlignment(OdDgTableCellElement::kLeftBottom); break;
        case OdDb::kTopCenter: pDgCell->setTextAlignment(OdDgTableCellElement::kCenterTop); break;
        case OdDb::kMiddleCenter: pDgCell->setTextAlignment(OdDgTableCellElement::kCenterMiddle); break;
        case OdDb::kBottomCenter: pDgCell->setTextAlignment(OdDgTableCellElement::kCenterBottom); break;
        case OdDb::kTopRight: pDgCell->setTextAlignment(OdDgTableCellElement::kRightTop); break;
        case OdDb::kMiddleRight: pDgCell->setTextAlignment(OdDgTableCellElement::kRightMiddle); break;
        case OdDb::kBottomRight: pDgCell->setTextAlignment(OdDgTableCellElement::kRightBottom); break;
      }

      OdString strTextContent = pDbTable->textString(i, j);

      if( !strTextContent.isEmpty() )
      {
        OdDbMTextPtr pMText = OdDbMText::createObject();
        pMText->setDatabaseDefaults(pDbTable->database());
        pMText->setTextStyle(pDbTable->textStyle(i, j, 0));
        pMText->setTextHeight(pDbTable->textHeight(i, j, 0));
        pMText->setColor(pDbTable->contentColor(i, j, 0));
        pMText->setRotation(pDbTable->rotation(i, j, 0));
        pMText->setAttachment((OdDbMText::AttachmentPoint)pDbTable->alignment(i, j));
        pMText->setFlowDirection(OdDbMText::kByStyle);

        bool bIsVertical = false;
        OdDbObjectId id = pDbTable->textStyle(i, j, 0);
        if (!id.isNull())
        {
          OdDbTextStyleTableRecordPtr txtStyle = pDbTable->textStyle(i, j, 0).safeOpenObject();
          if (!txtStyle.isNull())
            bIsVertical = txtStyle->isVertical();
        }

        double dCellWidth  = pDgCell->getWidth();
        double dCellHeight = pDgCell->getHeight();

        OdUInt32 minRow, minColumn, maxRow, maxColumn;

        if (pDbTable->isMergedCell(i, j, &minRow, &maxRow, &minColumn, &maxColumn))
        {
          dCellWidth  = 0;
          dCellHeight = 0;
          OdUInt32 k;

          for( k = minRow; k <= maxRow; k++ )
            dCellHeight += pDbTable->rowHeight(k);

          for (k = minColumn; k <= maxColumn; k++)
            dCellWidth += pDbTable->columnWidth(k);
        }

        double wText = dCellWidth - pDbTable->margin(i, j, OdDb::kCellMarginLeft) * 2.0;

        if (pDbTable->textRotation(i,j) == OdDb::kDegrees090 || pDbTable->textRotation(i, j) == OdDb::kDegrees270)
          wText = dCellHeight - pDbTable->margin(i, j, OdDb::kCellMarginTop) * 2.0;

        if (bIsVertical)
          pMText->setWidth(odmax(0.0, (dCellHeight - pDbTable->margin(i, j, OdDb::kCellMarginTop) * 2.0)));
        else
          pMText->setWidth(odmax(0.0, wText));

        pMText->setContents(strTextContent);

        bool b3dConvert = OdDgnExportContext::is3d();
        OdDgnExportContext::set3d(true);

        OdDgGraphicsElementPtr pTextNodeElm = convertMText(pMText, owner->database(), true);

        OdDgnExportContext::set3d(b3dConvert);

        if( !pTextNodeElm.isNull() )
        {
          OdDgTextNode3dPtr pTextNode = pTextNodeElm;
          pDgCell->setCellText(pTextNode);
        }
      }
    }
  }

  addElementToDatabase(pDgTable, owner);
  appendOdDgElementToOwner(owner, pDgTable);
  OdDgnExportContext::addDgnElement(pDbTable->objectId(), pDgTable->elementId());
}

//---------------------------------------------------------------------------------------------------

double getLightGeometrySize(OdDbLight* pDbLight)
{
  double dRet = 1;

  OdGeExtents3d ext3d;

  OdDbObjectId idOwner = pDbLight->ownerId();

  while( !idOwner.isNull() )
  {
    OdDbObjectPtr pOwner = idOwner.openObject(OdDb::kForRead, true);

    if( pOwner->isKindOf(OdDbBlockTableRecord::desc()) )
    {
      pOwner->getGeomExtents(ext3d);
      break;
    }

    idOwner = pOwner->ownerId();
  }

  if (ext3d.isValidExtents())
    dRet = ext3d.maxPoint().convert2d().distanceTo(ext3d.minPoint().convert2d()) / 15000.0;

  return dRet;
}

//---------------------------------------------------------------------------------------------------

OdDgGraphicsElementPtr exportDistantLight(OdDbLight* pDbLight, OdDgElement* owner)
{
  OdDgLightDistantPtr pDgLight = OdDgLightDistant::createObject();
  pDgLight->setDatabaseDefaults(owner->database());
  pDgLight->setDefaultValues(true, getLightGeometrySize(pDbLight));

  pDgLight->setLightName(pDbLight->name());

  OdGeVector3d vrXAxis = pDbLight->lightDirection();
  OdGeVector3d vrYAxis = vrXAxis.perpVector();
  OdGeVector3d vrZAxis = vrXAxis.crossProduct(vrYAxis);
  OdGeMatrix3d matTransform;
  matTransform.setCoordSystem(pDbLight->position(), vrXAxis, vrYAxis, vrZAxis);
  pDgLight->setIntensity(pDbLight->intensity());
  pDgLight->setOnFlag(pDbLight->isOn());
  
  const OdGiShadowParameters& shadowParam = pDbLight->shadowParameters();
  pDgLight->setShadowOnFlag(shadowParam.shadowsOn());
  pDgLight->setShadowResolution(shadowParam.shadowMapSize());

  ODCOLORREF lightColor = OdCmColorToRGBLayerBlock(pDbLight->lightColor(), pDbLight->layerId(), owner->database());

  pDgLight->setColorRed(ODGETRED(lightColor) / 255.0);
  pDgLight->setColorGreen(ODGETGREEN(lightColor) / 255.0);
  pDgLight->setColorBlue(ODGETBLUE(lightColor) / 255.0);

  pDgLight->transformBy(matTransform);

  addElementToDatabase(pDgLight, owner);
  appendOdDgElementToOwner(owner, pDgLight);
  OdDgnExportContext::addDgnElement(pDbLight->objectId(), pDgLight->elementId());
  return pDgLight;
}

//---------------------------------------------------------------------------------------------------

OdDgGraphicsElementPtr exportPointLight(OdDbLight* pDbLight, OdDgElement* owner)
{
  OdDgLightPointPtr pDgLight = OdDgLightPoint::createObject();
  pDgLight->setDatabaseDefaults(owner->database());
  pDgLight->setDefaultValues(true, getLightGeometrySize(pDbLight));

  pDgLight->setLightName(pDbLight->name());

  OdGeVector3d vrXAxis = pDbLight->lightDirection();

  if (vrXAxis.isZeroLength())
    vrXAxis = OdGeVector3d::kXAxis;

  OdGeVector3d vrYAxis = vrXAxis.perpVector();
  OdGeVector3d vrZAxis = vrXAxis.crossProduct(vrYAxis);
  OdGeMatrix3d matTransform;
  matTransform.setCoordSystem(pDbLight->position(), vrXAxis, vrYAxis, vrZAxis);
  pDgLight->setIntensity(pDbLight->intensity());
  pDgLight->setOnFlag(pDbLight->isOn());

  const OdGiShadowParameters& shadowParam = pDbLight->shadowParameters();
  pDgLight->setShadowOnFlag(shadowParam.shadowsOn());
  pDgLight->setShadowResolution(shadowParam.shadowMapSize());

  ODCOLORREF lightColor = OdCmColorToRGBLayerBlock(pDbLight->lightColor(), pDbLight->layerId(), owner->database());

  pDgLight->setColorRed(ODGETRED(lightColor) / 255.0);
  pDgLight->setColorGreen(ODGETGREEN(lightColor) / 255.0);
  pDgLight->setColorBlue(ODGETBLUE(lightColor) / 255.0);

  const OdGiLightAttenuation& attenuation = pDbLight->lightAttenuation();

  pDgLight->setAttenuateOnFlag(attenuation.attenuationType() == OdGiLightAttenuation::kInverseLinear);

  if( attenuation.useLimits() )
    pDgLight->setAttenuationDistance(attenuation.endLimit() - attenuation.startLimit());

  pDgLight->transformBy(matTransform);

  addElementToDatabase(pDgLight, owner);
  appendOdDgElementToOwner(owner, pDgLight);
  OdDgnExportContext::addDgnElement(pDbLight->objectId(), pDgLight->elementId());
  return pDgLight;
}

//---------------------------------------------------------------------------------------------------

OdDgGraphicsElementPtr exportSpotLight(OdDbLight* pDbLight, OdDgElement* owner)
{
  OdDgLightSpotPtr pDgLight = OdDgLightSpot::createObject();
  pDgLight->setDatabaseDefaults(owner->database());

  double dScale = 1.0;

  if (pDbLight->hasTarget())
    dScale = pDbLight->position().distanceTo(pDbLight->targetLocation());

  pDgLight->setDefaultValues(true, getLightGeometrySize(pDbLight)/dScale);

  OdGePoint3d ptOrigin = pDbLight->position();

  OdGeVector3d vrXAxis = pDbLight->lightDirection();
  OdGeVector3d vrYAxis = vrXAxis.perpVector();
  OdGeVector3d vrZAxis = vrXAxis.crossProduct(vrYAxis);
  OdGeMatrix3d matTransform;
  matTransform.setCoordSystem(ptOrigin, vrXAxis * dScale, vrYAxis * dScale, vrZAxis * dScale);

  pDgLight->setLightName(pDbLight->name());
  pDgLight->setIntensity(pDbLight->intensity());
  pDgLight->setOnFlag(pDbLight->isOn());

  double dSpotAngle = pDbLight->hotspotAngle();
  double dFalloffAngle = pDbLight->falloffAngle();

  pDgLight->setConeAngle(dSpotAngle / 2.0);
  pDgLight->setDeltaAngle((dFalloffAngle - dSpotAngle) / 2.0);

  const OdGiShadowParameters& shadowParam = pDbLight->shadowParameters();
  pDgLight->setShadowOnFlag(shadowParam.shadowsOn());
  pDgLight->setShadowResolution(shadowParam.shadowMapSize());

  ODCOLORREF lightColor = OdCmColorToRGBLayerBlock(pDbLight->lightColor(), pDbLight->layerId(), owner->database());

  pDgLight->setColorRed(ODGETRED(lightColor) / 255.0);
  pDgLight->setColorGreen(ODGETGREEN(lightColor) / 255.0);
  pDgLight->setColorBlue(ODGETBLUE(lightColor) / 255.0);

  const OdGiLightAttenuation& attenuation = pDbLight->lightAttenuation();

  pDgLight->setAttenuateOnFlag(attenuation.attenuationType() == OdGiLightAttenuation::kInverseLinear);

  if (attenuation.useLimits())
    pDgLight->setAttenuationDistance(attenuation.endLimit() - attenuation.startLimit());

  pDgLight->transformBy(matTransform);

  addElementToDatabase(pDgLight, owner);
  appendOdDgElementToOwner(owner, pDgLight);
  OdDgnExportContext::addDgnElement(pDbLight->objectId(), pDgLight->elementId());

  return pDgLight;
}

//---------------------------------------------------------------------------------------------------

void OdDbLightExportPE::subExportElement(OdDbEntity* pDbEntity, OdDgElement* owner)
{
  OdDbLight* pDbLight = (OdDbLight*)(pDbEntity);

  if (!pDbLight || !owner)
    return;

  OdDgGraphicsElementPtr pDgLight;

  switch( pDbLight->drawableType() )
  {
    case OdGiDrawable::kDistantLight: pDgLight = exportDistantLight(pDbLight, owner); break;
    case OdGiDrawable::kPointLight:   pDgLight = exportPointLight(pDbLight, owner); break;
    case OdGiDrawable::kSpotLight:    pDgLight = exportSpotLight(pDbLight, owner); break;
  }

  if (!pDgLight.isNull())
    OdDgnExportPE::subExportElement(pDbLight, pDgLight);
}

//---------------------------------------------------------------------------------------------------

}
