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
#include <RxDispatchImpl.h>
#include <RxVariantValue.h>
#include <DbDictionary.h>
#include <RxDynamicModule.h>
#include "DgnExportImpl.h"
#include <DgDatabase.h>
#include <DgTable.h>
#include <DgRegAppTableRecord.h>
#include <DbRegAppTable.h>
#include <DbRegAppTableRecord.h>
#include <DgTextStyleTableRecord.h>
#include <DbTextStyleTable.h>
#include <DbTextStyleTableRecord.h>
#include <DgFontTableRecord.h>
#include <DgMaterialTableRecord.h>
#include <DbMaterial.h>
#include <DgLineStyleTableRecord.h>
#include <DbLinetypeTable.h>
#include <DbLinetypeTableRecord.h>
#include <DgnLS/DbLSMisc.h>
#include <DgnLS/DbLSDefinition.h>
#include <DgnLS/DbLSInternalComponent.h>
#include <DgnLS/DbLSCompoundComponent.h>
#include <DgnLS/DbLSPointComponent.h>
#include <DgnLS/DbLSStrokePatternComponent.h>
#include <DgnLS/DbLSSymbolComponent.h>
#include <DgnLS/DbLSStroke.h>
#include <DbLayerTable.h>
#include <DbLayerTableRecord.h>
#include <DgLevelTableRecord.h>
#include <DgText.h>
#include <DgCellHeader.h>
#include <DgSharedCellDefinition.h>
#include <DbBlockTableRecord.h>
#include "Gi/GiTextStyle.h"
#include "Gi/GiMaterial.h"
#include "DgnExportCommon.h"
#include "DgnExportContext.h"
#include "ColorMapping.h"
#include <DbMlineStyle.h>
#include <DgMultilineStyleTableRecord.h>
#include <DbDimStyleTableRecord.h>
#include <DbDimStyleTable.h>
#include <DgDimStyleTableRecord.h>
#include <DbVisualStyle.h>
#include <DbViewTable.h>
#include <DbViewTableRecord.h>
#include <DgACS.h>
#include <DbUCSTable.h>
#include <DbUCSTableRecord.h>

namespace TD_DGN_EXPORT 
{
  //========================================================================================================================

  void DgnExporter::setDwgColorTable(OdDgDatabase* pDgnDb, bool bLightColorSchema, ODCOLORREF bgColor )
  {
    OdDgColorTablePtr pColorTable = pDgnDb->getColorTable(OdDg::kForWrite);

    OdArray<ODCOLORREF> ACadColors;

    const ODCOLORREF* pColors;

    if( bLightColorSchema )
      pColors = odcmAcadLightPalette();
    else
      pColors = odcmAcadDarkPalette();

    ACadColors.push_back(ODRGB(255, 255, 255));

    for( OdUInt32 i = 1; i < 255; i++ )
      ACadColors.push_back(pColors[i]);

    ACadColors.push_back(bgColor);

    pColorTable->setPalette(ACadColors);
  }

  //========================================================================================================================

  void DgnExporter::copyRegApps(const OdDbDatabase* pDwgDb, OdDgDatabase* pDgnDb)
  {
    if (!pDgnDb || !pDwgDb)
      return;

    OdDbRegAppTablePtr pDbRegAppTable = pDwgDb->getRegAppTableId().openObject(OdDb::kForRead);

    if( pDbRegAppTable.isNull() )
      return;

    OdDbSymbolTableIteratorPtr pRegAppIter = pDbRegAppTable->newIterator();

    if( pRegAppIter.isNull() || pRegAppIter->done() )
      return;

    OdDgRegAppTablePtr pDgRegAppTable = pDgnDb->getRegAppTable(OdDg::kForWrite);

    for(; !pRegAppIter->done(); pRegAppIter->step())
    {
      OdDbRegAppTableRecordPtr pDbRegAppTableRecord = pRegAppIter->getRecord(OdDb::kForRead);

      if (pDbRegAppTableRecord.isNull())
        continue;

      OdString regAppName = pDbRegAppTableRecord->getName();

      if( regAppName.isEmpty() )
      {
        regAppName = OD_T("RegApp0");

        OdUInt32 iCount = 1;

        while( !pDgRegAppTable->getAt(regAppName).isNull() )
        {
          regAppName.format(OD_T("RegApp%d"), iCount);
          iCount++;
        }
      }

      if (!pDgRegAppTable->getAt(regAppName).isNull() )
      {
        OdDgRegAppTableRecordPtr pDgRegApp = OdDgRegAppTableRecord::createObject();

        pDgRegApp->setName(regAppName);

        OdDgElementId idItem = pDgnDb->getElementId(OdDbHandle((OdUInt64)(pDbRegAppTableRecord->objectId().getHandle())));

        if( idItem.isNull() )
          pDgnDb->addOdDgElement(pDgRegApp, pDgRegAppTable->elementId(), pDbRegAppTableRecord->objectId().getHandle());
        else
          pDgnDb->addOdDgElement(pDgRegApp, pDgRegAppTable->elementId());

        pDgRegAppTable->add(pDgRegApp);
      }
    }
  }

  //========================================================================================================================

  bool isEqualTextStyles(const OdDgTextStyleTableRecord* pRec1, const OdDgTextStyleTableRecord* pRec2)
  {
    if( !pRec1 || !pRec2 )
      return false;

    if( pRec1->getFontEntryId() != pRec2->getFontEntryId() )
      return false;

    if (pRec1->getShxBigFontId() != pRec2->getShxBigFontId())
      return false;

    if (pRec1->getBoldFlag() != pRec2->getBoldFlag())
      return false;

    if (pRec1->getItalicsFlag() != pRec2->getItalicsFlag())
      return false;

    if (pRec1->getTextDirection() != pRec2->getTextDirection())
      return false;

    if (pRec1->getUnderlineFlag() != pRec2->getUnderlineFlag())
      return false;

    if (pRec1->getOverlineFlag() != pRec2->getOverlineFlag())
      return false;

    if( !OdEqual(pRec1->getTextHeight(), pRec2->getTextHeight()) )
      return false;

    if (!OdEqual(pRec1->getTextWidth(), pRec2->getTextWidth()))
      return false;

    if (!OdEqual(pRec1->getSlant(), pRec2->getSlant()))
      return false;

    return true;
  }

  //========================================================================================================================

  void convertDwgTextureMappingToDgnTextureMapping(OdGiMaterialMap& matMapping)
  {
    if (matMapping.source() == OdGiMaterialMap::kFile)
    {
      OdGiMapper& texMapper = matMapping.mapper();

      if( texMapper.projection() == OdGiMapper::kPlanar )
      {
        texMapper.setProjection(OdGiMapper::kDgnPlanar);
        matMapping.setMapper(texMapper);
      }
      else if (texMapper.projection() == OdGiMapper::kSphere)
      {
        texMapper.setProjection(OdGiMapper::kDgnSphere);
        matMapping.setMapper(texMapper);
      }
      else if (texMapper.projection() == OdGiMapper::kCylinder )
      {
        texMapper.setProjection(OdGiMapper::kDgnCylinderCapped);
        matMapping.setMapper(texMapper);
      }
    }
  }

  //========================================================================================================================

  bool isMaterialEqual(const OdDgMaterialTableRecord* pMat1, const OdDgMaterialTableRecord* pMat2)
  {
    // Ambient color

    OdGiMaterialColor clrAmbient1;
    OdGiMaterialColor clrAmbient2;
    pMat1->getAmbient(clrAmbient1);
    pMat2->getAmbient(clrAmbient2);

    if( clrAmbient1 != clrAmbient2 )
      return false;

    // Diffuse color

    OdGiMaterialColor clrDiffuse1;
    OdDgMaterialMap   mapDiffuse1;
    OdGiMaterialColor clrDiffuse2;
    OdDgMaterialMap   mapDiffuse2;

    pMat1->getDiffuse(clrDiffuse1, mapDiffuse1);
    pMat2->getDiffuse(clrDiffuse2, mapDiffuse2);

    if( clrDiffuse1 != clrDiffuse2 )
      return false;

    if (mapDiffuse1 != mapDiffuse2)
      return false;

    // Specular color

    OdGiMaterialColor clrSpecular1;
    double            dGlossFactor1;
    OdGiMaterialColor clrSpecular2;
    double            dGlossFactor2;

    pMat1->getSpecular(clrSpecular1, dGlossFactor1);
    pMat2->getSpecular(clrSpecular2, dGlossFactor2);

    if( clrSpecular1 != clrSpecular2 )
      return false;

    if (!OdEqual(dGlossFactor1, dGlossFactor2))
      return false;

    // Reflection

    OdGiMaterialMap mapReflect1;
    OdGiMaterialMap mapReflect2;

    pMat1->getReflection(mapReflect1);
    pMat2->getReflection(mapReflect2);

    if (mapReflect1 != mapReflect2)
      return false;

    // Opacity

    if (!OdEqual(pMat1->getOpacity(), pMat2->getOpacity()))
      return false;

    // Refraction

    if (!OdEqual(pMat1->getRefraction(), pMat2->getRefraction()))
      return false;

    // Translucence

    if (!OdEqual(pMat1->getTranslucence(), pMat2->getTranslucence()))
      return false;

    // Luminance

    if (!OdEqual(pMat1->getShininess(), pMat2->getShininess()))
      return false;

    // Bump

    OdDgMaterialMap   mapBump1;
    OdDgMaterialMap   mapBump2;

    pMat1->getBump(mapBump1);
    pMat2->getBump(mapBump2);

    if( mapBump1 != mapBump2 )
      return false;

    // Global illumination.

    if (pMat1->getGlobalIlluminationFlag() != pMat2->getGlobalIlluminationFlag())
      return false;

    return true;
  }

  //========================================================================================================================

  OdDbLSDefinitionPtr getDgnDefinitionDictionary(const OdDbLinetypeTableRecord* pDbLineStyle)
  {
    OdDbLSDefinitionPtr pRet;

    if (!pDbLineStyle->extensionDictionary().isNull())
    {
      OdDbDictionaryPtr pDictionary = pDbLineStyle->extensionDictionary().openObject(OdDb::kForRead);

      if (!pDictionary.isNull() && !pDictionary->getAt(::oddbDgnLSGetDefinitionKeyName()).isNull())
        pRet = pDictionary->getAt(::oddbDgnLSGetDefinitionKeyName()).openObject(OdDb::kForRead);
    }

    return pRet;
  }

  //========================================================================================================================

  bool isLineStyleContinuous(const OdDgElementId& idLineStyleDef)
  {
    bool bRet = true;

    OdDgLineStyleDefTableRecordPtr pLineStyleDef = idLineStyleDef.openObject(OdDg::kForRead);

    if (!pLineStyleDef.isNull())
    {
      if (pLineStyleDef->getType() == OdDg::kLsTypeInternal)
        bRet = true;
      else if (pLineStyleDef->getType() == OdDg::kLsTypeLineCode)
      {
        OdDgLineCodeResourcePtr pCodeRes = pLineStyleDef->getResource();

        if (pCodeRes->getStrokeCount() > 1)
          bRet = false;
        else
        {
          OdDgLineCodeResourceStrokeData firstStroke;

          pCodeRes->getStroke(0, firstStroke);

          if (!firstStroke.getDashFlag())
            bRet = false;
        }
      }
      else
        bRet = false;
    }

    return bRet;
  }

  //========================================================================================================================

  OdDg::LineStyleType getLineStyleType(const OdDgElementId& idLineStyleDef)
  {
    OdDg::LineStyleType retVal = OdDg::kLsTypeUndefined;

    OdDgLineStyleDefTableRecordPtr pLineStyleDef = idLineStyleDef.openObject(OdDg::kForRead);

    if (!pLineStyleDef.isNull())
      retVal = pLineStyleDef->getType();

    return retVal;
  }

  //========================================================================================================================

  OdDgElementId addDwgLineStyleDefinition( const OdDbLinetypeTableRecord* pDbLineStyle,
                                             const OdDbDatabase* pDwgDb,
                                                OdDgLineStyleDefTable* pDgLineStyleDefTable,
                                                  const OdString& strLineStyleName,
                                                    double dMasterUnitsToUorsScale
                                         )
  {
    OdDgElementId idRet;

    OdString defName = strLineStyleName;
    OdUInt32 uCount = 1;

    while( !pDgLineStyleDefTable->getAt(defName).isNull() )
    {
      OdString strCount;
      strCount.format(L"_%d", uCount);
      defName = strLineStyleName + strCount;
      uCount++;
    }

    OdDgLineStyleDefTableRecordPtr pLinePointDef;
    OdDgLinePointResourcePtr pLinePointRes;
    OdDgLineStyleDefTableRecordPtr pLineCodeDef = OdDgLineStyleDefTableRecord::createObject(OdDg::kLsTypeLineCode);

    OdDgLineCodeResourcePtr pLineCode = pLineCodeDef->getResource();

    for( OdInt32 i = 0; i < pDbLineStyle->numDashes(); i++ )
    {
      double dDashLength = pDbLineStyle->dashLengthAt(i);

      OdDgLineCodeResourceStrokeData curDash;
      curDash.setLength(fabs(dDashLength) * dMasterUnitsToUorsScale);
      curDash.setDashFlag(dDashLength >= 0);
      curDash.setWidthMode(OdDgLineCodeResourceStrokeData::kLsWidthFull);
      pLineCode->addStroke(curDash);

      OdString strShape = pDbLineStyle->textAt(i);

      if( strShape.isEmpty() && (pDbLineStyle->shapeNumberAt(i) != 0) )
        strShape += OdChar(pDbLineStyle->shapeNumberAt(i));

      if( !strShape.isEmpty() )
      {
        OdGeVector2d vrShapeOffset  = pDbLineStyle->shapeOffsetAt(i) * dMasterUnitsToUorsScale;

        double dRotation            = pDbLineStyle->shapeRotationAt(i);
        double dScale               = pDbLineStyle->shapeScaleAt(i);
        OdDbObjectId idShapeStyle   = pDbLineStyle->shapeStyleAt(i);

        OdDgElementId idTextStyle = OdDgnExportContext::getElementId(idShapeStyle);

        if( !idTextStyle.isNull() )
        {
          // 1. Create shape cell definition.

          OdString strStyleId;
          strStyleId.format(L"_0x%X", (OdUInt32)(idTextStyle.getHandle()));
          OdString strCellName = L"Shape_" + strShape + strStyleId;

          OdDgElementId idShape = pDgLineStyleDefTable->getAt(strCellName);

          if( idShape.isNull() )
          {
            OdDgCellHeader2dPtr pCell2d = OdDgCellHeader2d::createObject();
            pCell2d->setName(strCellName);
            pDgLineStyleDefTable->addSymbol(pCell2d);
            OdDgText2dPtr pShapeText = OdDgText2d::createObject();
            pShapeText->setDatabaseDefaults(pDgLineStyleDefTable->database());
            pShapeText->setText(strShape);
            pCell2d->add(pShapeText);
            pShapeText->applyTextStyle(idTextStyle, false, 1.0);

            if( !OdZero(pShapeText->getHeightMultiplier()) )
            {
              double dHeightCorrection = dMasterUnitsToUorsScale / pShapeText->getHeightMultiplier();
              pShapeText->transformBy(OdGeMatrix3d::scaling(dHeightCorrection));
            }
            else
            {
              pShapeText->setHeightMultiplier(dMasterUnitsToUorsScale);
              pShapeText->setLengthMultiplier(dMasterUnitsToUorsScale);
            }

            idShape = pCell2d->elementId();
          }

          if( !idShape.isNull() )
          {
            if (pLineCodeDef->elementId().isNull())
            {
              pLineCodeDef->setName(defName + L"_LineCode");
              pDgLineStyleDefTable->add(pLineCodeDef);
            }

            // 2. Create line point resource.

            if( pLinePointDef.isNull() )
            {
              pLinePointDef = OdDgLineStyleDefTableRecord::createObject(OdDg::kLsTypeLinePoint);
              pLinePointDef->setName(defName + L"_LinePoint");
              pDgLineStyleDefTable->add(pLinePointDef);
              pLinePointRes = pLinePointDef->getResource();
              pLinePointRes->setBasePatternHandleId(pLineCodeDef->elementId().getHandle());
              pLinePointRes->setBasePatternType(OdDgLineStyleResource::kLsLineCodeRes);
            }

            // 3. Create line symbol resource.

            OdDgLineStyleDefTableRecordPtr pLineSymbolDef = OdDgLineStyleDefTableRecord::createObject(OdDg::kLsTypePointSymbol);
            OdString strSymbolShape;
            strSymbolShape.format(L"_Symbol_%d", i);
            pLineSymbolDef->setName(defName + strSymbolShape);
            pDgLineStyleDefTable->add(pLineSymbolDef);

            OdDgPointSymbolResourcePtr pSymbolRes = pLineSymbolDef->getResource();
            pSymbolRes->setSymbolOffset(OdGePoint3d::kOrigin);
            pSymbolRes->setSymbolScale(1.0 / dScale);
            pSymbolRes->setSymbol3DFlag(false);
            pSymbolRes->setDependedCellHeaderHandle(idShape.getHandle());

            OdGeExtents3d extSymbol;
            OdDgElementPtr pShapeCellElm = idShape.openObject(OdDg::kForRead);

            if( !pShapeCellElm.isNull() && pShapeCellElm->isKindOf(OdDgCellHeader2d::desc()) )
            {
              OdDgCellHeader2dPtr pShapeCell = pShapeCellElm;
              pShapeCell->getGeomExtents(extSymbol);
            }

            if (extSymbol.isValidExtents())
            {
              pSymbolRes->setSymbolExtents(extSymbol);
            }

            // 4. Register symbol on point symbol element

            OdDgLinePointResourceSymInfo symbolInfo;
            symbolInfo.setOffset(OdGePoint2d(vrShapeOffset.x, vrShapeOffset.y));
            symbolInfo.setAbsoluteRotationAngleFlag(pDbLineStyle->shapeIsUcsOrientedAt(i));
            symbolInfo.setRotationAngle(dRotation);
            symbolInfo.setPointSymbolHandleId(pLineSymbolDef->elementId().getHandle());
            symbolInfo.setSymbolType(OdDgLineStyleResource::kLsPointSymbolRes);
            symbolInfo.setSymbolStrokeNo(i);
            symbolInfo.setSymbolPosOnStroke(OdDgLinePointResourceSymInfo::kLsAtEndOfStroke);
            symbolInfo.setDoNotScaleElementFlag(true);

            pLinePointRes = pLinePointDef->getResource();
            pLinePointRes->addSymbol(symbolInfo);
          }
        }
      }
    }

    if( pDbLineStyle->numDashes() == 0 )
    {
      OdDgLineCodeResourceStrokeData curDash;
      curDash.setLength(1.0 * dMasterUnitsToUorsScale);
      curDash.setDashFlag(true);
      curDash.setWidthMode(OdDgLineCodeResourceStrokeData::kLsWidthFull);
      pLineCode->addStroke(curDash);
      pLineCode->setAutoPhaseFlag(false);
      pLineCode->setSingleSegmentModeFlag(true);
      pLineCode->setCenterStretchPhaseModeFlag(true);
      pLineCode->setUseIterationLimitFlag(false);
    }

    if( pLinePointRes.isNull() )
    {
      pLineCodeDef->setName(defName);
      pDgLineStyleDefTable->add(pLineCodeDef);
      idRet = pLineCodeDef->elementId();
    }
    else
    {
      OdDgLineStyleDefTableRecordPtr pLineCompoundDef = OdDgLineStyleDefTableRecord::createObject(OdDg::kLsTypeCompound);
      pLineCompoundDef->setName(defName);
      pDgLineStyleDefTable->add(pLineCompoundDef);
      OdDgCompoundLineStyleResourcePtr pCompoundRes = pLineCompoundDef->getResource();

      OdDgCompoundLineStyleComponentInfo codeComp;
      codeComp.setComponentType(OdDgLineStyleResource::kLsLineCodeRes);
      codeComp.setComponentOffset(0.0);
      codeComp.setComponentHandleId(pLineCodeDef->elementId().getHandle());
      pCompoundRes->addComponent(codeComp);
      OdDgCompoundLineStyleComponentInfo pointComp;
      pointComp.setComponentType(OdDgLineStyleResource::kLsLinePointRes);
      pointComp.setComponentOffset(0.0);
      pointComp.setComponentHandleId(pLinePointDef->elementId().getHandle());
      pCompoundRes->addComponent(pointComp);

      idRet = pLineCompoundDef->elementId();
    }

    return idRet;
  }

  //========================================================================================================================

  OdInt32 getDgnInternalStyleNumber(const OdDbObjectId& idComponent)
  {
    OdInt32 iRet = -1;

    if (!idComponent.isNull())
    {
      OdDbObjectPtr pObjComponent = idComponent.openObject(OdDb::kForRead);

      if (pObjComponent->isKindOf(OdDbLSInternalComponent::desc()))
      {
        OdDbLSInternalComponentPtr pInternalComponent = pObjComponent;
        iRet = pInternalComponent->lineCode();
      }
    }

    return iRet;
  }

  //========================================================================================================================

  OdString createLineStyleComponentName(const OdDbObjectId& idDbComponent, const OdString& strNameBase)
  {
    OdString strRet = strNameBase;

    OdDbLSComponentPtr pDgnComponent = idDbComponent.openObject(OdDb::kForRead);

    if (pDgnComponent.isNull())
      return strRet;

    if (pDgnComponent->isKindOf(OdDbLSCompoundComponent::desc()))
    {
      strRet += L"_compound";
    }
    else if (pDgnComponent->isKindOf(OdDbLSPointComponent::desc()))
    {
      strRet += L"_point";
    }
    else if (pDgnComponent->isKindOf(OdDbLSSymbolComponent::desc()))
    {
      strRet += L"_symbol";
    }
    else if (pDgnComponent->isKindOf(OdDbLSStrokePatternComponent::desc()))
    {
      strRet += L"_stroke";
    }

    return strRet;
  }

  //========================================================================================================================

  OdDgLineStyleResource::OdLsResourceType getLineStyleComponentType(const OdDgElementId& idDgComponent)
  {
    OdDgLineStyleResource::OdLsResourceType retVal = OdDgLineStyleResource::kLsUnknownRes;

    if( idDgComponent.isNull() )
      return retVal;

    OdDgLineStyleDefTableRecordPtr pDef = idDgComponent.openObject(OdDg::kForRead);
    
    switch( pDef->getType() )
    {
      case OdDg::kLsTypeCompound   : retVal = OdDgLineStyleResource::kLsCompoundRes; break;
      case OdDg::kLsTypeLinePoint  : retVal = OdDgLineStyleResource::kLsLinePointRes; break;
      case OdDg::kLsTypePointSymbol: retVal = OdDgLineStyleResource::kLsPointSymbolRes; break;
      case OdDg::kLsTypeInternal:    retVal = OdDgLineStyleResource::kLsInternalRes; break;
      case OdDg::kLsTypeLineCode:    retVal = OdDgLineStyleResource::kLsLineCodeRes; break;
    }

    return retVal;
  }

  //========================================================================================================================

  OdDgElementId convertDgnComponent(OdDbObjectId& idComponent, OdDgLineStyleDefTable* pDgLineStyleDefTable, double dComponentScale, const OdString& strComponentName);

  //========================================================================================================================

  OdDgElementId createCodeComponent(OdDbLSStrokePatternComponentPtr pDgnComponent,
                                      OdDgLineStyleDefTable* pDgLineStyleDefTable,
                                        double dComponentScale,
                                          const OdString& strComponentName)
  {
    OdDgElementId idRet;

    OdDgLineStyleDefTableRecordPtr pComponent = OdDgLineStyleDefTableRecord::createObject(OdDg::kLsTypeLineCode);
    pComponent->setName(strComponentName);
    pDgLineStyleDefTable->add(pComponent);

    idRet = pComponent->elementId();

    OdDgLineCodeResourcePtr pRes = pComponent->getResource();
    pRes->setDescription(pDgnComponent->description());

    switch( pDgnComponent->phaseMode() )
    {
      case OdDbLSStrokePatternComponent::kLSPhaseCentered:
      {
        pRes->setCenterStretchPhaseModeFlag(true);
        pRes->setAutoPhaseFlag(false);
      } break;

      case OdDbLSStrokePatternComponent::kLSPhaseDistance:
      {
        pRes->setCenterStretchPhaseModeFlag(false);
        pRes->setAutoPhaseFlag(false);
        pRes->setPhase(pDgnComponent->phase());
      } break;

      case OdDbLSStrokePatternComponent::kLSPhaseFraction:
      {
        pRes->setCenterStretchPhaseModeFlag(false);
        pRes->setAutoPhaseFlag(true);
        pRes->setPhase(pDgnComponent->autoPhase());
      } break;
    }

    pRes->setUseIterationLimitFlag(pDgnComponent->hasIterationLimit());
    pRes->setMaxIterations(pDgnComponent->iterationLimit());
    pRes->setSingleSegmentModeFlag(pDgnComponent->isSingleSegment());

    for( OdUInt32 i = 0; i < pDgnComponent->numberStrokes(); i++ )
    {
      OdDgLineCodeResourceStrokeData strokeInfo;
      OdDbLSStroke* pDbStroke = pDgnComponent->stroke(i);

      if( !pDbStroke )
        continue;

      strokeInfo.setDashFlag(pDbStroke->isDash());
      strokeInfo.setByPassCornerFlag(pDbStroke->bypassCorner());
      strokeInfo.setCanBeScaledFlag(pDbStroke->canBeScaled());
      strokeInfo.setCapsType((OdDgLineCodeResourceStrokeData::OdLsStrokeCapsType)(pDbStroke->capMode()));

      if( pDbStroke->length() < 1e10 )
        strokeInfo.setLength(pDbStroke->length() * dComponentScale);
      else 
        strokeInfo.setLength(1.0);

      strokeInfo.setEndWidth(pDbStroke->endWidth());
      strokeInfo.setInvertStrokeInFirstCodeFlag(pDbStroke->invertAtOrigin());
      strokeInfo.setInvertStrokeInLastCodeFlag(pDbStroke->invertAtEnd());
      strokeInfo.setStartWidth(pDbStroke->startWidth());
      strokeInfo.setWidthMode((OdDgLineCodeResourceStrokeData::OdLsStrokeWidthMode)pDbStroke->widthMode());

      pRes->addStroke(strokeInfo);
    }

    return idRet;
  }

  //========================================================================================================================

  OdDgElementId createSymbolComponent(OdDbLSSymbolComponentPtr pDgnComponent,
                                        OdDgLineStyleDefTable* pDgLineStyleDefTable,
                                          double dComponentScale,
                                            const OdString& strComponentName)
  {
    OdDgElementId idRet;

    OdDgLineStyleDefTableRecordPtr pPointSymbol = OdDgLineStyleDefTableRecord::createObject(OdDg::kLsTypePointSymbol);
    pPointSymbol->setName(strComponentName);
    pDgLineStyleDefTable->add(pPointSymbol);

    idRet = pPointSymbol->elementId();

    OdDgPointSymbolResourcePtr pRes = pPointSymbol->getResource();
    pRes->setDescription(pDgnComponent->description());

    pRes->setSymbol3DFlag(pDgnComponent->Is3d());

    if (pDgnComponent->hasUnitScale() && !OdZero(pDgnComponent->unitScale()) )
      pRes->setSymbolScale(OdDgnExportContext::getMasterToUORsScale() * dComponentScale / (pDgnComponent->unitScale()) );
    else if(!OdZero(pDgnComponent->storedUnitScale()))
      pRes->setSymbolScale( OdDgnExportContext::getMasterToUORsScale() * dComponentScale / (pDgnComponent->storedUnitScale()) );

    OdDbObjectId idSymbol = pDgnComponent->blockTableRecord();
    OdDbBlockTableRecordPtr pDbSymbol = idSymbol.openObject(OdDb::kForRead);

    if( !pDbSymbol.isNull() )
    {
      bool bOld3dFlag = OdDgnExportContext::is3d();
      OdDgnExportContext::set3d(pDgnComponent->Is3d());

      OdDgElementId idDgSymbol = OdDgnExportContext::getElementId(idSymbol);
      OdDgGraphicsElementPtr pDgSymbol;

      if(idDgSymbol.isNull() )
      {
        if (pDgnComponent->Is3d())
        {
          OdDgCellHeader3dPtr pCellHeader = OdDgCellHeader3d::createObject();
          pCellHeader->setName(pDbSymbol->getName());
          pDgLineStyleDefTable->addSymbol(pCellHeader);
          pDgSymbol = pCellHeader;
        }
        else
        {
          OdDgCellHeader2dPtr pCellHeader = OdDgCellHeader2d::createObject();
          pCellHeader->setName(pDbSymbol->getName());
          pDgLineStyleDefTable->addSymbol(pCellHeader);
          pDgSymbol = pCellHeader;
        }

        OdDgnExportContext::addProcessingBlockId(pDbSymbol->id());
        DgnExporter::copyBlock(pDbSymbol, pDgSymbol);
        OdDgnExportContext::removeProcessingBlockId(pDbSymbol->id());

        idDgSymbol = pDgSymbol->elementId();

        OdDgnExportContext::addDgnElement(idSymbol, pDgSymbol->elementId());
      }
      else
      {
        OdDgElementPtr pDgSymbol = idDgSymbol.openObject(OdDg::kForWrite);

        if( pDgSymbol->isKindOf(OdDgSharedCellDefinition::desc()) )
        {
          OdDgSharedCellDefinitionPtr pSharedCellDef = pDgSymbol;

          OdDgCellHeader3dPtr pCellHeader3d;
          OdDgCellHeader2dPtr pCellHeader2d;

          if (pDgnComponent->Is3d())
          {
            pCellHeader3d = OdDgCellHeader3d::createObject();
            pCellHeader3d->setName(pDbSymbol->getName());
            pDgLineStyleDefTable->addSymbol(pCellHeader3d);
            pDgSymbol = pCellHeader3d;
          }
          else
          {
            pCellHeader2d = OdDgCellHeader2d::createObject();
            pCellHeader2d->setName(pDbSymbol->getName());
            pDgLineStyleDefTable->addSymbol(pCellHeader2d);
            pDgSymbol = pCellHeader2d;
          }

          OdDgElementIteratorPtr pIter = pSharedCellDef->createIterator();

          for(; !pIter->done(); pIter->step())
          {
            OdDgElementPtr pItem = pIter->item().openObject(OdDg::kForRead);

            if( !pCellHeader2d.isNull())
            {
              OdDgElementPtr pItemClone;

              OdDg2d3dConversionGraphicsElementPEPtr p2dConvert = OdDg2d3dConversionGraphicsElementPE::cast(pItem);

              if (!p2dConvert.isNull())
                pItemClone = p2dConvert->convertTo2d(pItem);
              else
                pItemClone = pItem->clone();

              pCellHeader2d->add(pItemClone);
            }
            else
            {
              OdDgElementPtr pItemClone;

              OdDg2d3dConversionGraphicsElementPEPtr p3dConvert = OdDg2d3dConversionGraphicsElementPE::cast(pItem);

              if (!p3dConvert.isNull())
                pItemClone = p3dConvert->convertTo3d(pItem);
              else
                pItemClone = pItem->clone();

              pCellHeader3d->add(pItemClone);
            }
          }

          pSharedCellDef->erase(true);

          OdDgnExportContext::addDgnElement(idSymbol, pDgSymbol->elementId());
        }
        else if (pDgSymbol->ownerId() != pDgLineStyleDefTable->elementId())
        {
          ODA_ASSERT_ONCE(!"DgnExport: wrong placement of dgn line style shape symbol.");
          pDgSymbol = NULL;
          idDgSymbol = NULL;
        }
      }

      if( !idDgSymbol.isNull() )
      {
        pRes->setDependedCellHeaderHandle(idDgSymbol.getHandle());

        OdGeExtents3d extSymbol;
        pDgSymbol->getGeomExtents(extSymbol);

        if (extSymbol.isValidExtents())
          pRes->setSymbolExtents(extSymbol);
      }

      OdDgnExportContext::set3d(bOld3dFlag);
    }

    return idRet;
  }

  //========================================================================================================================

  OdDgElementId createPointComponent(OdDbLSPointComponentPtr pDgnComponent,
                                       OdDgLineStyleDefTable* pDgLineStyleDefTable,
                                         double dComponentScale,
                                           const OdString& strComponentName)
  {
    OdDgElementId idRet;

    OdDgLineStyleDefTableRecordPtr pLinePoint = OdDgLineStyleDefTableRecord::createObject(OdDg::kLsTypeLinePoint);
    pLinePoint->setName(strComponentName);
    pDgLineStyleDefTable->add(pLinePoint);

    idRet = pLinePoint->elementId();

    OdDgLinePointResourcePtr pRes = pLinePoint->getResource();
    pRes->setDescription(pDgnComponent->description());

    OdDbObjectId idDbLineCode = pDgnComponent->strokeComponent();

    OdInt32 iInternalLineStyle = getDgnInternalStyleNumber(idDbLineCode);

    if (iInternalLineStyle < 0)
    {
      OdDgElementId idDgComponent = convertDgnComponent(idDbLineCode, pDgLineStyleDefTable, dComponentScale, createLineStyleComponentName(idDbLineCode, strComponentName));

      if (!idDgComponent.isNull())
      {
        pRes->setBasePatternHandleId(idDgComponent.getHandle());
        pRes->setBasePatternType(getLineStyleComponentType(idDgComponent));
      }
    }
    else
    {
      pRes->setBasePatternEntryId(iInternalLineStyle);
      pRes->setBasePatternType(OdDgLineStyleResource::kLsInternalRes);
    }

    for( OdUInt32 i = 0; i < pDgnComponent->numberSymbols(); i++ )
    {
      OdDbLSSymbolReference* pDbSymbolDef = pDgnComponent->symbol(i);

      OdDgLinePointResourceSymInfo symInfo;
      symInfo.setNoPartialStrokesFlag(!pDbSymbolDef->partialStrokes());
      symInfo.setDoNotClipElementFlag(!pDbSymbolDef->clipPartial());
      symInfo.setDoNotScaleElementFlag(!pDbSymbolDef->allowStretch());
      symInfo.setPartialOriginBeyondEndFlag(pDbSymbolDef->partialProjected());
      symInfo.setUseSymbolColorFlag(pDbSymbolDef->useSymbolColor());
      symInfo.setUseSymbolWeightFlag(pDbSymbolDef->useSymbolLineweight());
      symInfo.setOffset(OdGePoint2d(pDbSymbolDef->xOffset()*dComponentScale, pDbSymbolDef->yOffset()*dComponentScale));
      symInfo.setRotationAngle(pDbSymbolDef->angle());

      if( pDbSymbolDef->strokeNumber() >= 0 )
        symInfo.setSymbolStrokeNo(pDbSymbolDef->strokeNumber());

      symInfo.setSymbolPosOnStroke((OdDgLinePointResourceSymInfo::OdLsSymbolPosOnStroke)(pDbSymbolDef->justify()));
      symInfo.setSymbolAtEachVertexFlag((pDbSymbolDef->vertexMask() & OdDbLSSymbolReference::kLSAtVertex) ? true : false);
      symInfo.setSymbolAtElementOriginFlag((pDbSymbolDef->vertexMask() & OdDbLSSymbolReference::kLSAtOrigin) ? true : false);
      symInfo.setSymbolAtElementEndFlag((pDbSymbolDef->vertexMask() & OdDbLSSymbolReference::kLSAtEnd) ? true : false);
      
      switch( pDbSymbolDef->rotationType() )
      {
        case OdDbLSSymbolReference::kLSAbsolute:
        {
          symInfo.setAbsoluteRotationAngleFlag(true);
          symInfo.setMirrorSymbolForReversedLinesFlag(false);
        } break;

        case OdDbLSSymbolReference::kLSAdjusted:
        {
          symInfo.setAbsoluteRotationAngleFlag(false);
          symInfo.setMirrorSymbolForReversedLinesFlag(true);
        } break;

        case OdDbLSSymbolReference::kLSRelative:
        {
          symInfo.setAbsoluteRotationAngleFlag(false);
          symInfo.setMirrorSymbolForReversedLinesFlag(false);
        } break;
      }

      symInfo.setSymbolType(OdDgLineStyleResource::kLsPointSymbolRes);

      OdDbObjectId idDbSymbolPoint = pDbSymbolDef->symbolComponent();

      OdDgElementId idDgSymbolPoint = convertDgnComponent(idDbSymbolPoint, pDgLineStyleDefTable, dComponentScale, createLineStyleComponentName(idDbSymbolPoint, strComponentName));

      if (!idDgSymbolPoint.isNull())
      {
        symInfo.setPointSymbolHandleId(idDgSymbolPoint.getHandle());
        pRes->addSymbol(symInfo);
      }
    }

    return idRet;
  }

  //========================================================================================================================

  OdDgElementId createCompoundComponent(OdDbLSCompoundComponentPtr pDgnComponent,
                                          OdDgLineStyleDefTable* pDgLineStyleDefTable,
                                            double dComponentScale,
                                              const OdString& strComponentName)
  {
    OdDgElementId idRet;

    OdDgLineStyleDefTableRecordPtr pCompound = OdDgLineStyleDefTableRecord::createObject(OdDg::kLsTypeCompound);
    pCompound->setName(strComponentName);
    pDgLineStyleDefTable->add(pCompound);

    idRet = pCompound->elementId();

    OdDgCompoundLineStyleResourcePtr pRes = pCompound->getResource();
    pRes->setDescription(pDgnComponent->description());

    for( OdUInt32 i = 0; i < pDgnComponent->numberComponents(); i++ )
    {
      OdDgCompoundLineStyleComponentInfo compoundItem;

      OdDbObjectId idDbComponent = pDgnComponent->component(i);
      double       dOffset = pDgnComponent->offsetToComponent(i);

      OdInt32 iInternalLineStyle = getDgnInternalStyleNumber(idDbComponent);

      if( iInternalLineStyle < 0 )
      {
        OdDgElementId idDgComponent = convertDgnComponent(idDbComponent, pDgLineStyleDefTable, dComponentScale, createLineStyleComponentName(idDbComponent, strComponentName));

        if (!idDgComponent.isNull())
        {
          compoundItem.setComponentType(getLineStyleComponentType(idDgComponent));
          compoundItem.setComponentOffset(dOffset * dComponentScale);
          compoundItem.setComponentHandleId(idDgComponent.getHandle());

          pRes->addComponent(compoundItem);
        }
      }
      else
      {
        compoundItem.setComponentType(OdDgLineStyleResource::kLsInternalRes);
        compoundItem.setComponentOffset(dOffset * dComponentScale);
        compoundItem.setComponentEntryId(iInternalLineStyle);

        pRes->addComponent(compoundItem);
      }
    }

    return idRet;
  }

  //========================================================================================================================

  OdDgElementId convertDgnComponent(OdDbObjectId& idComponent,
                                      OdDgLineStyleDefTable* pDgLineStyleDefTable,
                                        double dComponentScale,
                                          const OdString& strComponentName)
  {
    OdDgElementId idRet = OdDgnExportContext::getElementId(idComponent);

    if (!idRet.isNull())
      return idRet;

    OdDbLSComponentPtr pDgnComponent = idComponent.openObject(OdDb::kForRead);

    if (pDgnComponent.isNull())
      return idRet;

    if( pDgnComponent->isKindOf(OdDbLSCompoundComponent::desc()) )
    {
      idRet = createCompoundComponent(pDgnComponent, pDgLineStyleDefTable, dComponentScale, strComponentName);
    }
    else if ( pDgnComponent->isKindOf(OdDbLSPointComponent::desc()) )
    {
      idRet = createPointComponent(pDgnComponent, pDgLineStyleDefTable, dComponentScale, strComponentName);
    }
    else if ( pDgnComponent->isKindOf(OdDbLSSymbolComponent::desc()) )
    {
      idRet = createSymbolComponent(pDgnComponent, pDgLineStyleDefTable, dComponentScale, strComponentName);
    }
    else if ( pDgnComponent->isKindOf(OdDbLSStrokePatternComponent::desc()) )
    {
      idRet = createCodeComponent(pDgnComponent, pDgLineStyleDefTable, dComponentScale, strComponentName);
    }

    if (!idRet.isNull())
      OdDgnExportContext::addDgnElement(idComponent, idRet);

    return idRet;
  }

  //========================================================================================================================

  OdDgElementId addDgnLineStyleDefinition(const OdDbLSDefinition* pDbDgnDef,
                                            const OdDbDatabase* pDwgDb,
                                              OdDgLineStyleDefTable* pDgLineStyleDefTable,
                                                const OdString& strLineStyleName,
                                                  double dComponentScale)
  {
    OdDgElementId idRet;

    if( getDgnInternalStyleNumber(pDbDgnDef->component()) >= 0 )
      return idRet;

    OdDbObjectId idComponent = pDbDgnDef->component();

    if( !idComponent.isNull() )
      idRet = convertDgnComponent(idComponent, pDgLineStyleDefTable, dComponentScale, strLineStyleName );

    return idRet;
  }

  //========================================================================================================================

  void DgnExporter::copyLayers(const OdDbDatabase* pDwgDb, 
                                 OdDgDatabase* pDgnDb,
                                   bool bUpdateDefaultLevelInfo,
                                     bool bRenameIfEqual)
  {
    OdDbObjectId idDbLayerTable = pDwgDb->getLayerTableId();

    if (idDbLayerTable.isNull())
      return;

    OdDbLayerTablePtr pDbLayerTable = idDbLayerTable.openObject(OdDb::kForRead);

    if (pDbLayerTable.isNull())
      return;

    OdDbSymbolTableIteratorPtr pDbLayerIter = pDbLayerTable->newIterator();

    if( pDbLayerIter.isNull() || pDbLayerIter->done() )
      return;

    OdDgLevelTablePtr pDgLevelTable = pDgnDb->getLevelTable(OdDg::kForWrite);

    for(; !pDbLayerIter->done(); pDbLayerIter->step())
    {
      OdDbLayerTableRecordPtr pDbLayer = pDbLayerIter->getRecord();

      if( pDbLayer.isNull() )
        continue;

      OdDgLevelTableRecordPtr pDgLevel;

      OdString strLayerName = pDbLayer->getName();

      if (strLayerName == L"0")
      {
        if (!bUpdateDefaultLevelInfo)
          continue;
        else
          pDgLevel = pDgLevelTable->getAt(0x40).openObject(OdDg::kForWrite);
      }
      else if( !bRenameIfEqual && !pDgLevelTable->getAt(strLayerName).isNull() )
        continue;
      else
      {
        if( !pDgLevelTable->getAt(strLayerName).isNull() )
        {
          OdUInt32 uNameCount = 1;
          OdString strNewName;
          strNewName.format(L"_%d", uNameCount);
          strNewName = strLayerName + strNewName;

          while (!pDgLevelTable->getAt(strLayerName).isNull())
          {
            uNameCount++;
            strNewName.format(L"_%d", uNameCount);
            strNewName = strLayerName + strNewName;
          }

          strLayerName = strNewName;
        }

        pDgLevel = OdDgLevelTableRecord::createObject();
        pDgLevel->setName(strLayerName);
      }

      pDgLevel->setDescription(pDbLayer->description());

      // Line weight

      pDgLevel->setElementLineWeight(odDgnExportLineweightMapping(pDbLayer->lineWeight()));

      // Line style

      OdDbObjectId  idDbLineStyle = pDbLayer->linetypeObjectId();
      OdDgElementId idDgLineStyle = getDgnElementId(idDbLineStyle);

      OdUInt32 uLineStyleEntryId = 0;

      if (!idDgLineStyle.isNull())
      {
        OdDgElementPtr pDgLineStyleElm = idDgLineStyle.openObject(OdDg::kForRead);
        
        if( !pDgLineStyleElm.isNull() && pDgLineStyleElm->isKindOf(OdDgLineStyleTableRecord::desc()) )
        {
          OdDgLineStyleTableRecordPtr pDgLineStyle = pDgLineStyleElm;
          uLineStyleEntryId = pDgLineStyle->getEntryId();
        }
      }

      pDgLevel->setElementLineStyleEntryId(uLineStyleEntryId);

      // Color

      pDgLevel->setElementColorIndex(getDgnColorIndex(pDbLayer->color(), pDgnDb));

      // Transparency

      pDgLevel->setTransparency(pDbLayer->transparency());

      // Material

      OdDbObjectId idDbMeterial = pDbLayer->materialId();

      if (!idDbMeterial.isNull() && (idDbMeterial != pDwgDb->globalMaterialId()))
      {
        OdDgElementId idDgMaterial = getDgnElementId(idDbMeterial);

        if (!idDgMaterial.isNull())
          pDgLevel->setByLevelMaterial(idDgMaterial);
      }

      // Flags

      pDgLevel->setIsPlotFlag(pDbLayer->isPlottable());
      pDgLevel->setIsFrozenFlag(pDbLayer->isFrozen());
      pDgLevel->setIsDisplayedFlag(!pDbLayer->isOff());

      if (pDbLayer->isLocked())
        pDgLevel->setElementAccess(OdDgLevelTableRecord::kAccessLocked);

      if( !pDgLevel.isNull() )
      {
        if (!pDgLevel->isDBRO())
          pDgLevelTable->add(pDgLevel);
      }

      if (!pDgLevel.isNull())
        addIdPair(pDbLayer->objectId(), pDgLevel->elementId());
      else
        addIdPair(pDbLayer->objectId(), OdDgElementId());
    }
  }

  //========================================================================================================================

  void DgnExporter::copyLineStyles(const OdDbDatabase* pDwgDb, OdDgDatabase* pDgnDb, double dMasterUnitsToUorsScale )
  {
    OdDbObjectId idDbLineStyleTable = pDwgDb->getLinetypeTableId();

    if (idDbLineStyleTable.isNull())
      return;

    OdDbLinetypeTablePtr pDbLineStyleTable = idDbLineStyleTable.openObject(OdDb::kForRead);

    if (pDbLineStyleTable.isNull())
      return;

    OdDbSymbolTableIteratorPtr pDbLineStyleIter = pDbLineStyleTable->newIterator();

    if( pDbLineStyleIter.isNull() || pDbLineStyleIter->done() )
      return;

    OdDgLineStyleTablePtr     pDgLineStyleTable     = pDgnDb->getLineStyleTable(OdDg::kForWrite);
    OdDgLineStyleDefTablePtr  pDgLineStyleDefTable  = pDgnDb->getLineStyleDefTable(OdDg::kForWrite);

    for(; !pDbLineStyleIter->done(); pDbLineStyleIter->step())
    {
      OdDbLinetypeTableRecordPtr pDbLineStyle = pDbLineStyleIter->getRecord();

      if( pDbLineStyle.isNull() || (pDbLineStyle->objectId() == pDwgDb->getLinetypeByLayerId()) || (pDbLineStyle->objectId() == pDwgDb->getLinetypeByBlockId()) )
        continue;

      OdString strLineStyleName = pDbLineStyle->getName();

      if( strLineStyleName.isEmpty() )
      {
        OdUInt32 iCount = 1;
        OdString strCurNameBase = L"ACadLinetype";
        OdString strCurName = strCurNameBase;

        while( !pDgLineStyleTable->getAt(strCurName).isNull() )
        {
          strCurName.format(L"_%d", iCount);
          strCurName = strCurNameBase + strCurName;
          iCount++;
        }
       
        strLineStyleName = strCurName;
      }

      OdDgLineStyleTableRecordPtr pDgLineStyle;

      if (!pDgLineStyleTable->getAt(strLineStyleName).isNull())
      {
        addIdPair(pDbLineStyle->objectId(), pDgLineStyleTable->getAt(strLineStyleName));
        continue;
      }

      pDgLineStyle = OdDgLineStyleTableRecord::createObject();
      pDgLineStyle->setName(strLineStyleName);

      OdDbLSDefinitionPtr pDbDgnDictionary = getDgnDefinitionDictionary(pDbLineStyle);

      OdDgElementId idLineStyleDef;

      if( !pDbDgnDictionary.isNull() )
      {
        OdDgElementId idLineStyleDef = addDgnLineStyleDefinition(pDbDgnDictionary, pDwgDb, pDgLineStyleDefTable, strLineStyleName, dMasterUnitsToUorsScale * pDbDgnDictionary->unitScale());

        if( !idLineStyleDef.isNull() )
        {
          pDgLineStyle->setRefersToElementFlag(true);
          pDgLineStyle->setRefersToId(idLineStyleDef);
          pDgLineStyle->setNoWidthFlag(false);
          pDgLineStyle->setSharedCellScaleIndependentFlag(pDbDgnDictionary->isSCScaleIndependent());
          pDgLineStyle->setSnappableFlag(pDbDgnDictionary->isSnappable());
          pDgLineStyle->setPhysicalFlag(pDbDgnDictionary->isPhysical());
          pDgLineStyle->setUnitsType(OdDg::kLsUORS);
          pDgLineStyle->setContinuousFlag(pDbDgnDictionary->isContinuous());
          pDgLineStyle->setType(getLineStyleType(idLineStyleDef));

          pDgLineStyleTable->add(pDgLineStyle);
        }
        else
        {
          OdInt32 iDgnInternalStyle = getDgnInternalStyleNumber(pDbDgnDictionary->component());

          if( iDgnInternalStyle >= 0 && iDgnInternalStyle < 8 )
          {
            pDgLineStyle = pDgLineStyleTable->getAt((OdUInt32)(iDgnInternalStyle)).openObject(OdDg::kForRead);
          }
          else
            pDgLineStyle = NULL;
        }
      }
      else
      {
        OdDgElementId idLineStyleDef = addDwgLineStyleDefinition(pDbLineStyle, pDwgDb, pDgLineStyleDefTable, strLineStyleName, dMasterUnitsToUorsScale);

        if (!idLineStyleDef.isNull())
        {
          pDgLineStyle->setRefersToElementFlag(true);
          pDgLineStyle->setRefersToId(idLineStyleDef);
          pDgLineStyle->setNoWidthFlag(false);
          pDgLineStyle->setSharedCellScaleIndependentFlag(false);
          pDgLineStyle->setSnappableFlag(false);
          pDgLineStyle->setPhysicalFlag(false);
          pDgLineStyle->setUnitsType(OdDg::kLsUORS);
          pDgLineStyle->setContinuousFlag(isLineStyleContinuous(idLineStyleDef));
          pDgLineStyle->setType(getLineStyleType(idLineStyleDef));

          pDgLineStyleTable->add(pDgLineStyle);
        }
        else
          pDgLineStyle = NULL;
      }

      if( !pDgLineStyle.isNull() )
        addIdPair(pDbLineStyle->objectId(), pDgLineStyle->elementId());
      else
        addIdPair(pDbLineStyle->objectId(), OdDgElementId());
    }
  }

  //========================================================================================================================

  void DgnExporter::copyMaterials(const OdDbDatabase* pDwgDb, OdDgDatabase* pDgnDb, bool bRenameIfEqual )
  {
    OdDbObjectId idMatDictionary = pDwgDb->getMaterialDictionaryId(false);

    if( idMatDictionary.isNull() )
      return;

    OdDbDictionaryPtr pDbMatDictionary = idMatDictionary.openObject(OdDb::kForRead);

    if( pDbMatDictionary.isNull() )
      return;

    OdDbDictionaryIteratorPtr pMatDictIter = pDbMatDictionary->newIterator();

    if( pMatDictIter.isNull() || pMatDictIter->done() )
      return;

    OdDgMaterialTablePtr pDgMaterialTable = pDgnDb->getMaterialTable(OdDg::kForWrite);

    /*
      pMatTraits->setAmbient(pImpl->m_AmbientColor);
  pMatTraits->setDiffuse(pImpl->m_DiffuseColor, pImpl->m_DiffuseMapSync);
  pMatTraits->setSpecular(pImpl->m_SpecularColor, pImpl->m_SpecularMapSync, pImpl->m_SpecularGloss);
  pMatTraits->setReflection(pImpl->m_ReflectionMapSync);
  pMatTraits->setOpacity(pImpl->m_OpacityPercent, pImpl->m_OpacityMapSync);
  pMatTraits->setBump(pImpl->m_BumpMapSync);
  pMatTraits->setRefraction(pImpl->m_RefractionIndex, pImpl->m_RefractionMapSync);
  pMatTraits->setTranslucence(pImpl->m_Translucence);
  pMatTraits->setSelfIllumination(pImpl->m_SelfIllumination);
  pMatTraits->setReflectivity(pImpl->m_Reflectivity);
  pMatTraits->setIlluminationModel(pImpl->m_IlluminationModel);
  pMatTraits->setChannelFlags(pImpl->m_ChannelFlags);
  pMatTraits->setMode(pImpl->m_Mode);
  // Extended material properties
  pMatTraits->setColorBleedScale(pImpl->m_ColorBleedScale);
  pMatTraits->setIndirectBumpScale(pImpl->m_IndirectBumpScale);
  pMatTraits->setReflectanceScale(pImpl->m_ReflectanceScale);
  pMatTraits->setTransmittanceScale(pImpl->m_TransmitScale);
  pMatTraits->setTwoSided(pImpl->m_TwoSided);
  pMatTraits->setLuminanceMode(pImpl->m_LuminanceMode);
  pMatTraits->setLuminance(pImpl->m_Luminance);
  pMatTraits->setNormalMap(pImpl->m_NormalMap, pImpl->m_NormalMapMethod, pImpl->m_NormalMapStrength);
  pMatTraits->setGlobalIllumination(pImpl->m_GlobalIllumination);
  pMatTraits->setFinalGather(pImpl->m_FinalGather);
    */

    for(; !pMatDictIter->done(); pMatDictIter->next() )
    {
      OdDbMaterialPtr pDbMaterial = OdDbMaterial::cast(pMatDictIter->getObject());

      if( pDbMaterial.isNull() || (pDbMaterial->objectId() == pDwgDb->byLayerMaterialId()) || (pDbMaterial->objectId() == pDwgDb->byBlockMaterialId()))
        continue;

      OdDgMaterialTableRecordPtr pDgMaterial;

      OdString strMaterialName = pDbMaterial->name();

      if (strMaterialName.isEmpty())
      {
        OdUInt32 iCount = 1;
        OdString strCurNameBase = L"ACadMaterial";
        OdString strCurName = strCurNameBase;

        while (!pDgMaterialTable->getAt(strCurName).isNull())
        {
          strCurName.format(L"_%d", iCount);
          strCurName = strCurNameBase + strCurName;
          iCount++;
        }

        strMaterialName = strCurName;
      }

      if( pDgMaterialTable->getAt(strMaterialName).isNull() || bRenameIfEqual )
      {
        pDgMaterial = OdDgMaterialTableRecord::createObject();

        // name 

        pDgMaterial->setName(strMaterialName);

        // Ambient color

        OdGiMaterialColor clrAmbient;
        pDbMaterial->ambient(clrAmbient);
        pDgMaterial->setAmbientFactor(clrAmbient.factor());

        // Diffuse color

        OdGiMaterialColor clrDiffuse;
        OdGiMaterialMap   mapGiDiffuse;
        pDbMaterial->diffuse(clrDiffuse, mapGiDiffuse);

        if( mapGiDiffuse.source() == OdGiMaterialMap::kFile && mapGiDiffuse.sourceFileName().isEmpty() )
          mapGiDiffuse.setSource(OdGiMaterialMap::kScene);

        convertDwgTextureMappingToDgnTextureMapping(mapGiDiffuse);

        OdDgMaterialMap   mapDiffuse;
        mapDiffuse.setGiMaterialMap(mapGiDiffuse, true);

        pDgMaterial->setDiffuse(clrDiffuse, mapDiffuse);

        // Specular color

        OdGiMaterialColor clrSpecular;
        double            dGlossFactor;
        OdGiMaterialMap   mapSpecular;
        pDbMaterial->specular(clrSpecular, mapSpecular, dGlossFactor);
        mapSpecular.setSource(OdGiMaterialMap::kScene);

        OdDgMaterialTableRecord::SpecularColorType iSpecularType = OdDgMaterialTableRecord::kByElement;

        if( clrSpecular.method() == OdGiMaterialColor::kOverride )
          iSpecularType = OdDgMaterialTableRecord::kCustom;

        pDgMaterial->setSpecular(clrSpecular, iSpecularType, dGlossFactor, OdDgMaterialTableRecord::kGlossFactorCustom);

        // Reflection

        pDgMaterial->setReflectionFactor(pDbMaterial->reflectanceScale(), false);

        // Opacity

        OdGiMaterialMap   mapOpacity;
        double            dOpacityFactor;
        pDbMaterial->opacity(dOpacityFactor, mapOpacity);
        pDgMaterial->setOpacity(dOpacityFactor);

        // Refraction

        OdGiMaterialMap   mapRefraction;
        double            dRefrectionFactor;
        pDbMaterial->refraction(dRefrectionFactor, mapRefraction);
        pDgMaterial->setRefraction(dRefrectionFactor);

        // Translucence

        pDgMaterial->setTranslucence(pDbMaterial->translucence());

        // Luminance

        pDgMaterial->setShininess(pDbMaterial->selfIllumination());

        // Bump

        OdDgMaterialMap   mapBump;
        OdGiMaterialMap   mapGiBump;
        pDbMaterial->bump(mapGiBump);

        convertDwgTextureMappingToDgnTextureMapping(mapGiBump);

        if (mapGiBump.source() == OdGiMaterialMap::kFile && mapGiBump.sourceFileName().isEmpty())
          mapGiBump.setSource(OdGiMaterialMap::kScene);

        mapBump.setGiMaterialMap(mapGiBump);

        pDgMaterial->setBump(mapBump, false);

        // Global illumination.

        pDgMaterial->setGlobalIlluminationFlag(pDbMaterial->globalIllumination() == OdGiMaterialTraits::kGlobalIlluminationCastAndReceive);

        // add material

        if( pDgMaterialTable->getAt(strMaterialName).isNull() )
          pDgMaterialTable->add(pDgMaterial);
        else if( bRenameIfEqual )
        {
          OdDgElementId idDgMaterial = pDgMaterialTable->getAt(strMaterialName);
          OdDgMaterialTableRecordPtr pNativeMaterial = idDgMaterial.openObject(OdDg::kForRead, true);

          if( isMaterialEqual(pDgMaterial, pNativeMaterial) )
          {
            pDgMaterial = pNativeMaterial;
          }
          else
          {
            OdString strNewName;
            OdUInt32 uCount = 1;
            strNewName.format(L"_%d", uCount);
            strNewName = strMaterialName + strNewName;

            while (!pDgMaterialTable->getAt(strNewName).isNull())
            {
              uCount++;
              strNewName.format(L"_%d", uCount);
              strNewName = strMaterialName + strNewName;
            }

            pDgMaterial->setName(strNewName);
            pDgMaterialTable->add(pDgMaterial);
          }
        }
      }
      else
      {
        OdDgElementId idDgMaterial = pDgMaterialTable->getAt(strMaterialName);
        pDgMaterial = idDgMaterial.openObject(OdDg::kForRead, true);
      }

      if (!pDgMaterial.isNull())
        addIdPair(pDbMaterial->objectId(), pDgMaterial->elementId());
      else
        addIdPair(pDbMaterial->objectId(), OdDgElementId());
    }
  }

  //========================================================================================================================

  void DgnExporter::copyTextStyles(const OdDbDatabase* pDwgDb, OdDgDatabase* pDgnDb, double dMasterUnitsToUorsScale, bool bRenameIfEqual )
  {
    if (!pDgnDb || !pDwgDb)
      return;

    OdDbTextStyleTablePtr pDbTestStyleTable = pDwgDb->getTextStyleTableId().openObject(OdDb::kForRead);

    if (pDbTestStyleTable.isNull())
      return;

    OdDbSymbolTableIteratorPtr pTestStyleIter = pDbTestStyleTable->newIterator();

    if( pTestStyleIter.isNull() || pTestStyleIter->done() )
      return;

    OdDgTextStyleTablePtr pDgTextStyleTable = pDgnDb->getTextStyleTable(OdDg::kForWrite);

    for(; !pTestStyleIter->done(); pTestStyleIter->step() )
    {
      OdDbTextStyleTableRecordPtr pDbTextStyle = pTestStyleIter->getRecord(OdDb::kForRead);

      if( pDbTextStyle.isNull() )
        continue;

      OdString strTextStyleName = pDbTextStyle->getName();

      if (strTextStyleName.isEmpty())
      {
        OdUInt32 iCount = 1;
        OdString strCurNameBase = L"ACadTextStyle";
        OdString strCurName = strCurNameBase;

        while (!pDgTextStyleTable->getAt(strCurName).isNull())
        {
          strCurName.format(L"_%d", iCount);
          strCurName = strCurNameBase + strCurName;
          iCount++;
        }

        strTextStyleName = strCurName;
      }

      OdGiTextStyle giTextStyle;
      giFromDbTextStyle(pDbTextStyle, giTextStyle);

      OdDgTextStyleTableRecordPtr pDgTextStyle;

      if( pDgTextStyleTable->getAt(strTextStyleName).isNull() || bRenameIfEqual )
      {
        // set direction flags and underline/overline

        pDgTextStyle = OdDgTextStyleTableRecord::createObject();
        pDgTextStyle->setName(strTextStyleName);

        OdUInt16 uDirectionFlags = 0;

        if(giTextStyle.isVertical() )
          uDirectionFlags |= OdDg::kVertical;

        if(giTextStyle.isBackward() )
          uDirectionFlags |= OdDg::kBackwards;

        if(giTextStyle.isUpsideDown() )
          uDirectionFlags |= OdDg::kUpsideDown;

        pDgTextStyle->setTextDirection((OdDg::TextDirection)(uDirectionFlags));
        pDgTextStyle->setUnderlineFlag(giTextStyle.isUnderlined());
        pDgTextStyle->setOverlineFlag(giTextStyle.isOverlined());

        if (OdZero( giTextStyle.textSize()) )
          pDgTextStyle->setTextHeight(pDbTextStyle->priorSize() * dMasterUnitsToUorsScale);
        else
          pDgTextStyle->setTextHeight( giTextStyle.textSize() * dMasterUnitsToUorsScale );

        pDgTextStyle->setTextWidth( pDgTextStyle->getTextHeight() * giTextStyle.trackingPercent() );
        pDgTextStyle->setSlant(giTextStyle.obliquingAngle());

        if( !pDbTextStyle->fileName().isEmpty() )
        {
          OdString strTypeFace;
          bool     bBold = false;
          bool     bItalic = false;
          int      iCharset = 0;
          int      iPitchAndFamily = 0;

          bool bTTF = false;

          OdString strFileName = pDbTextStyle->fileName();

          giTextStyle.font(strTypeFace, bBold, bItalic, iCharset, iPitchAndFamily);

          if (!strTypeFace.isEmpty())
          {
            strFileName = strTypeFace;
            bTTF = true;
          }

          OdUInt32 uFontEntryId = getDgnFontEntryId(strFileName, pDgnDb, !bTTF);

          pDgTextStyle->setBoldFlag(bBold);
          pDgTextStyle->setItalicsFlag(bItalic);

          pDgTextStyle->setFontEntryId(uFontEntryId);
        }
        else if (giTextStyle.getFont())
        {
          OdUInt32 uFontEntryId = getDgnFontEntryId(giTextStyle.getFont(), pDgnDb);

          if (giTextStyle.isShxFont())
          {
            pDgTextStyle->setBoldFlag(false);
            pDgTextStyle->setItalicsFlag(false);
          }
          else
          {
            OdString strTypeFace;
            bool     bBold = false;
            bool     bItalic = false;
            int      iCharset = 0;
            int      iPitchAndFamily = 0;

            giTextStyle.font(strTypeFace, bBold, bItalic, iCharset, iPitchAndFamily);

            pDgTextStyle->setBoldFlag(bBold);
            pDgTextStyle->setItalicsFlag(bItalic);
          }

          pDgTextStyle->setFontEntryId(uFontEntryId);
        }
        else
          pDgTextStyle->setFontEntryId(0);

        if( !pDbTextStyle->bigFontFileName().isEmpty() )
        {
          OdUInt32 uBigFontEntryId = getDgnFontEntryId(pDbTextStyle->bigFontFileName(), pDgnDb, true);
          pDgTextStyle->setShxBigFontId(uBigFontEntryId);
        }

        if( pDgTextStyleTable->getAt(strTextStyleName).isNull() )
          pDgTextStyleTable->add(pDgTextStyle);
        else if( bRenameIfEqual )
        {
          OdDgElementId idNativeTextStyle = pDgTextStyleTable->getAt(strTextStyleName);

          OdDgTextStyleTableRecordPtr pNativeTextStyle = idNativeTextStyle.openObject(OdDg::kForRead, true);

          if( isEqualTextStyles(pNativeTextStyle, pDgTextStyle) )
            pDgTextStyle = pNativeTextStyle;
          else
          {
            OdString strNewName;
            OdUInt32 uCount = 1;
            strNewName.format(L"_%d", uCount);
            strNewName = strTextStyleName + strNewName;

            while( !pDgTextStyleTable->getAt(strNewName).isNull() )
            {
              uCount++;
              strNewName.format(L"_%d", uCount);
              strNewName = strTextStyleName + strNewName;
            }

            pDgTextStyle->setName(strNewName);
            pDgTextStyleTable->add(pDgTextStyle);
          }
        }
      }
      else
      {
        OdDgElementId idNativeTextStyle = pDgTextStyleTable->getAt(strTextStyleName);
        pDgTextStyle = idNativeTextStyle.openObject(OdDg::kForRead, true);
      }

      if (!pDgTextStyle.isNull())
        addIdPair(pDbTextStyle->objectId(), pDgTextStyle->elementId());
      else
        addIdPair(pDbTextStyle->objectId(), OdDgElementId());
    }

    OdDgTextStyleTableRecordPtr pDefaultTextStyle = pDgnDb->getTextStyleTable()->getDefaultData(OdDg::kForWrite);

    if( !pDwgDb->getTEXTSTYLE().isNull() )
    {
      OdDgElementId idDefaultTextStyle = OdDgnExportContext::getElementId(pDwgDb->getTEXTSTYLE());

      if (!idDefaultTextStyle.isNull())
      {
        OdDgTextStyleTableRecordPtr pDefTextStyle = idDefaultTextStyle.openObject(OdDg::kForRead);

        if (!pDefTextStyle.isNull())
          pDefaultTextStyle = pDefTextStyle->clone();
      }
    }

    if (!pDefaultTextStyle.isNull() )
    {
      if (!OdZero(pDwgDb->getTEXTSIZE()))
      {
        OdDgModelPtr pDefaultModel;

        if (!pDgnDb->getDefaultModelId().isNull())
          pDefaultModel = pDgnDb->getDefaultModelId().openObject(OdDg::kForRead);
        else if (!pDgnDb->getActiveModelId().isNull())
          pDefaultModel = pDgnDb->getActiveModelId().openObject(OdDg::kForRead);
        else
        {
          OdDgModelTablePtr pModelTable = pDgnDb->getModelTable();
          OdDgElementIteratorPtr pModelIter = pModelTable->createIterator();

          if (!pModelIter->done())
            pDefaultModel = pModelIter->item().openObject(OdDg::kForRead);
        }

        if (!pDefaultModel.isNull())
        {
          double dRatio = 1.0;
          
          if( !OdZero(pDefaultTextStyle->getTextHeight() ) )
            dRatio = pDefaultTextStyle->getTextWidth() / pDefaultTextStyle->getTextHeight();

          pDefaultTextStyle->setTextHeight(pDwgDb->getTEXTSIZE()*pDefaultModel->convertWorkingUnitsToUORs(1.0));
          pDefaultTextStyle->setTextWidth(pDefaultTextStyle->getTextHeight() * dRatio);
        }
      }

      pDgnDb->getTextStyleTable(OdDg::kForWrite)->setDefaultData(pDefaultTextStyle);
    }
  }

  //========================================================================================================================

  void convertDimStyleTableRecord(OdDbDimStyleTableRecord* pDbDimStyle, OdDgDimStyleTable* pDgDimStyleTable)
  {
    if (!pDbDimStyle || !pDgDimStyleTable)
      return;

    OdDgDimStyleTableRecordPtr pDgDimStyle = OdDgDimStyleTableRecord::createObject();

    OdString strName = pDbDimStyle->getName();

    pDgDimStyle->setName(strName);
    pDgDimStyle->setCenterMarkSize(pDbDimStyle->dimcen());

    // Angular data

    OdUInt32 uAccur = pDbDimStyle->dimadec();

    if( uAccur < 8 )
      pDgDimStyle->setAngleAccuracy((OdDgDimTextFormat::Accuracy)uAccur);
    else
      pDgDimStyle->setAngleAccuracy((OdDgDimTextFormat::Accuracy)7);

    pDgDimStyle->setAngleDisplayMode(OdDgDimTextFormat::kAngle_D);
    pDgDimStyle->setShowAngleFormatFlag(true);

    switch( pDbDimStyle->dimaunit() )
    {
      case 0: pDgDimStyle->setShowAngleFormatFlag(false); break;
      case 1: 
      {
        pDgDimStyle->setShowAngleFormatFlag(true);
        pDgDimStyle->setAngleDisplayMode(OdDgDimTextFormat::kAngle_DMS);
      } break;
      case 2: pDgDimStyle->setAngleDisplayMode(OdDgDimTextFormat::kAngle_C); break;
      case 3: pDgDimStyle->setAngleDisplayMode(OdDgDimTextFormat::kAngle_Radian); break;
    }

    pDgDimStyle->setAngleTrailingZerosFlag(pDbDimStyle->dimazin() < 2);
    pDgDimStyle->setAngleLeadingZeroFlag(pDbDimStyle->dimazin() == 2 || pDbDimStyle->dimazin() == 0);

    // Extension lines

    pDgDimStyle->setExtensionLinesPresentFlag(!pDbDimStyle->dimse1() || !pDbDimStyle->dimse2());

    // Dimension units

    bool     bUseSecondaryUnits = pDbDimStyle->dimalt();
    pDgDimStyle->setShowSecondaryUnitsFlag(bUseSecondaryUnits);

    // Dimension Primary units

    OdUInt16 uCommaSym                = pDbDimStyle->dimdsep();
    OdUInt32 uDimZinValue             = pDbDimStyle->dimzin();
    double   dPrimaryUnitsScaleFactor = fabs(pDbDimStyle->dimlfac());
    double   dSubUnitsScaleFactor = fabs(pDbDimStyle->dimmzf());
    OdString strPrefixSuffixMaster    = pDbDimStyle->dimpost();
    OdString strPrefixSuffixSub       = pDbDimStyle->dimmzs();
    OdUInt8  uDimPresission = pDbDimStyle->dimdec();
    OdUInt8  uDimUnitFormat = pDbDimStyle->dimlunit();

    pDgDimStyle->setDecimalCommaFlag(uCommaSym == L',');

    // 1. leading and trailing zeros

    pDgDimStyle->setPrimaryTrailingZerosFlag(uDimZinValue < 8);
    pDgDimStyle->setPrimaryLeadingZeroFlag(uDimZinValue & 0x00000004 ? true : false);

    // 2. Prefix and suffix for units and subunits

    OdString strPrefix;
    OdString strSuffix;

    int iPrefixEnd = strPrefixSuffixMaster.find(L"<>");

    if (iPrefixEnd != -1)
    {
      if (iPrefixEnd != 0)
        strPrefix = strPrefixSuffixMaster.left(iPrefixEnd);

      if (iPrefixEnd != strPrefixSuffixMaster.getLength() - 2)
        strPrefixSuffixMaster = strPrefixSuffixMaster.right(strPrefixSuffixMaster.getLength() - iPrefixEnd - 2);
      else
        strPrefixSuffixMaster.empty();
    }

    strSuffix = strPrefixSuffixMaster;

    if (strSuffix.right(2) == L"\\L")
    {
      pDgDimStyle->setTextUnderlineFlag(true);

      if (strSuffix.getLength() > 2)
        strSuffix = strSuffix.left(strSuffix.getLength() - 2);
      else
        strSuffix.empty();
    }

    if (strSuffix.right(2) == L"\\X")
    {
      if (strSuffix.getLength() > 2)
        strSuffix = strSuffix.left(strSuffix.getLength() - 2);
      else
        strSuffix.empty();
    }

    // 3. Scale factor of units and subunits

    OdDgModel::UnitDescription baseMasterUnits;
    OdDgModel::UnitDescription baseSubUnits;

    baseMasterUnits.m_numerator = 1.0;
    baseMasterUnits.m_denominator = 1.0;
    baseSubUnits.m_numerator = 1.0;
    baseSubUnits.m_denominator = 1.0;

    OdDgModelPtr pDimensionModel;

    OdDgElementId idModel = pDgDimStyleTable->database()->getActiveModelId();

    if (!idModel.isNull())
      pDimensionModel = idModel.openObject(OdDg::kForRead);

    double dBaseSubUnitsScaleFactor = 1.0;

    if (!pDimensionModel.isNull())
    {
      pDimensionModel->getMasterUnit(baseMasterUnits);
      pDimensionModel->getSubUnit(baseSubUnits);
      dBaseSubUnitsScaleFactor = baseMasterUnits.m_denominator / baseMasterUnits.m_numerator /
        baseSubUnits.m_denominator * baseSubUnits.m_numerator;
    }

    if( (strSuffix.getLength() < 2) && OdEqual(dPrimaryUnitsScaleFactor, 1.0) && OdEqual(dSubUnitsScaleFactor, dBaseSubUnitsScaleFactor) )
    {
      pDgDimStyle->setMasterFileUnitsFlag(true);
    }
    else
    {
      OdDgModel::UnitDescription masterUnits;
      OdDgModel::UnitDescription subUnits;

      pDgDimStyle->setMasterFileUnitsFlag(false);

      double dMasterNum = baseMasterUnits.m_numerator * dPrimaryUnitsScaleFactor / baseMasterUnits.m_denominator;
      double dSubNum = dMasterNum * dSubUnitsScaleFactor;

      fillUnitDescription(dMasterNum, masterUnits);
      fillUnitDescription(dSubNum, subUnits);

      pDgDimStyle->setPrimaryMasterUnits(masterUnits);
      pDgDimStyle->setPrimarySubUnits(subUnits);
    }

    pDgDimStyle->setPrimaryLabelDisplayMode(OdDgDimTextFormat::kMu);

    if (bUseSecondaryUnits)
    {
      if( strPrefix.getLength() == 1 )
        pDgDimStyle->setUpperPrefix(strPrefix.getAt(0));

      if( strSuffix.getLength() == 1 )
        pDgDimStyle->setUpperSuffix(strSuffix.getAt(0));
    }
    else
    {
      if (strPrefix.getLength() == 1)
        pDgDimStyle->setMainPrefix(strPrefix.getAt(0));

      if (strSuffix.getLength() == 1)
        pDgDimStyle->setMainSuffix(strSuffix.getAt(0));
    }

    // 4. Accuracy

    pDgDimStyle->setPrimaryAccuracy(convertAccuracyToDgnFormat(uDimUnitFormat, uDimPresission, pDbDimStyle->dimrnd()));

    // Dimension alternate units

    if( bUseSecondaryUnits )
    {
      double dSecondaryUnitsScaleFactor    = fabs(pDbDimStyle->dimaltf());
      double dSecondarySubUnitsScaleFactor = fabs(pDbDimStyle->dimaltmzf());
      uDimPresission        = pDbDimStyle->dimaltd();
      uDimUnitFormat        = pDbDimStyle->dimaltu();
      uDimZinValue          = pDbDimStyle->dimaltz();
      strPrefixSuffixMaster = pDbDimStyle->dimapost();
      strPrefixSuffixSub    = pDbDimStyle->dimaltmzs();

      // 1. leading and trailing zeros

      pDgDimStyle->setSecondaryTrailingZerosFlag(uDimZinValue < 8);
      pDgDimStyle->setSecondaryLeadingZeroFlag(uDimZinValue & 0x00000004 ? true : false);

      // 2. Prefix and suffix for units and subunits

      strPrefix.empty();
      strSuffix.empty();

      int iPrefixEnd = strPrefixSuffixMaster.find(L"[]");

      if (iPrefixEnd != -1)
      {
        if (iPrefixEnd != 0)
          strPrefix = strPrefixSuffixMaster.left(iPrefixEnd);

        if (iPrefixEnd != strPrefixSuffixMaster.getLength() - 2)
          strPrefixSuffixMaster = strPrefixSuffixMaster.right(strPrefixSuffixMaster.getLength() - iPrefixEnd - 2);
        else
          strPrefixSuffixMaster.empty();
      }

      strSuffix = strPrefixSuffixMaster;

      if( strSuffix.right(2) == L"\\L" || strSuffix.right(2) == L"\\X" )
      {
        if (strSuffix.getLength() > 2)
          strSuffix = strSuffix.left(strSuffix.getLength() - 2);
        else
          strSuffix.empty();
      }

      // 3. Scale factor of units and subunits

      OdDgModel::UnitDescription masterUnits;
      OdDgModel::UnitDescription subUnits;

      double dMasterNum = baseMasterUnits.m_numerator * dSecondaryUnitsScaleFactor / baseMasterUnits.m_denominator;
      double dSubNum = dMasterNum * dSecondarySubUnitsScaleFactor;

      fillUnitDescription(dMasterNum, masterUnits);
      fillUnitDescription(dSubNum, subUnits);

      pDgDimStyle->setSecondaryMasterUnits(masterUnits);
      pDgDimStyle->setSecondarySubUnits(subUnits);

      pDgDimStyle->setSecondaryLabelDisplayMode(OdDgDimTextFormat::kMu);

      if (strPrefix.getLength() == 1)
        pDgDimStyle->setLowerPrefix(strPrefix.getAt(0));

      if (strSuffix.getLength() == 1)
        pDgDimStyle->setLowerSuffix(strSuffix.getAt(0));

      // 4. Accuracy

      pDgDimStyle->setSecondaryAccuracy(convertAccuracyToDgnFormat(uDimUnitFormat, uDimPresission, pDbDimStyle->dimrnd()));
    }

    // Dimension Stacked fraction settings

    pDgDimStyle->setStackedFractionFlag(pDbDimStyle->dimfrac() != 2);
    pDgDimStyle->setUseStackedFractionFlag(pDbDimStyle->dimfrac() != 2);
    pDgDimStyle->setStackedFractionType(OdDgDimTextInfo::kFractionFromText);

    if( pDbDimStyle->dimfrac() == 0 )
      pDgDimStyle->setStackedFractionType(OdDgDimTextInfo::kFractionHorizontal);
    else if (pDbDimStyle->dimfrac() == 1)
      pDgDimStyle->setStackedFractionType(OdDgDimTextInfo::kFractionDiagonal);

    // Dimension Tolerances

    bool bDisplayTolerance = pDbDimStyle->dimtol();
    pDgDimStyle->setShowToleranceFlag(bDisplayTolerance);

    if (bDisplayTolerance)
    {
      pDgDimStyle->setToleranceTextScale(pDbDimStyle->dimtfac()*fabs(pDbDimStyle->dimtxt()));
      pDgDimStyle->setUseToleranceLimitModeFlag(pDbDimStyle->dimlim());

      double dLowTol = pDbDimStyle->dimtm();
      double dHighTol = pDbDimStyle->dimtp();

      pDgDimStyle->setLowerToleranceValue(dLowTol);
      pDgDimStyle->setUpperToleranceValue(dHighTol);
      pDgDimStyle->setToleranceStackIfEqualFlag( OdEqual(dLowTol, dHighTol));
      pDgDimStyle->setPrimaryToleranceAccuracy(convertAccuracyToDgnFormat(2, pDbDimStyle->dimtdec(), pDbDimStyle->dimrnd()));
      pDgDimStyle->setSecondaryToleranceAccuracy(convertAccuracyToDgnFormat(2, pDbDimStyle->dimalttd(), pDbDimStyle->dimrnd()));
    }

    // Dimension fit options.

    pDgDimStyle->setJoinerFlag(pDbDimStyle->dimtofl());

    if( pDbDimStyle->dimtix() )
    {
      pDgDimStyle->setFitOptions(OdDgDimTextInfo::kTextInside);
    }
    else
    {
      switch (pDbDimStyle->dimatfit())
      {
        case 0: pDgDimStyle->setFitOptions(OdDgDimTextInfo::kBothMoves); break;
        case 1: pDgDimStyle->setFitOptions(OdDgDimTextInfo::kTermMovesFirst); break;
        case 2: pDgDimStyle->setFitOptions(OdDgDimTextInfo::kTextMovesFirst); break;
        case 3: pDgDimStyle->setFitOptions(OdDgDimTextInfo::kSmallestMoves); break;
      }
    }

    pDgDimStyle->setDimLeaderEnableFlag(pDbDimStyle->dimtmove() != 0);

    if( pDbDimStyle->dimtmove() == 1 )
      pDgDimStyle->setDimLeaderChainType(OdDgDimOptionOffset::kLine);
    else if (pDbDimStyle->dimtmove() == 2)
      pDgDimStyle->setDimLeaderChainType(OdDgDimOptionOffset::kNone);

    // Dimension text

    // 1. Text style

    OdDgElementId idDgDimTextStyle = OdDgnExportContext::getElementId(pDbDimStyle->dimtxsty());

    if (!idDgDimTextStyle.isNull())
    {
      OdDgElementPtr pTextStyleElm = idDgDimTextStyle.openObject(OdDg::kForRead);

      if( !pTextStyleElm.isNull() && pTextStyleElm->isKindOf(OdDgTextStyleTableRecord::desc()))
      {
        OdDgTextStyleTableRecordPtr pTextStyle = pTextStyleElm;
        pDgDimStyle->setTextStyleEntryId(pTextStyle->getEntryId());
        pDgDimStyle->setTextFontId(pTextStyle->getFontEntryId());
      }
    }

    // 2.Text height

    double dTextHeight = pDbDimStyle->dimtxt();

    if( !OdZero(dTextHeight) )
    {
      pDgDimStyle->setTextHeightOverrideFlag(true);
      pDgDimStyle->setTextHeight(dTextHeight);
    }

    // 3.Text color

    OdCmColor textColor = pDbDimStyle->dimclrt();

    if( textColor.colorMethod() != OdCmEntityColor::kByBlock )
    {
      pDgDimStyle->setTextColorOverrideFlag(true);
      pDgDimStyle->setTextColor(getDgnColorIndex(textColor, pDgDimStyleTable->database()));
    }

    // 4.Text direction

    pDgDimStyle->setHorizontalTextFlag(pDbDimStyle->dimtih() || pDbDimStyle->dimtoh());

    // 5.Text offset and lift

    pDgDimStyle->setGeometryMargin(fabs(pDbDimStyle->dimgap()));
    pDgDimStyle->setInlineTextLift(pDbDimStyle->dimtvp());
    pDgDimStyle->setBoxTextFlag(pDbDimStyle->dimgap() < 0);

    // 6. Text alignments

    pDgDimStyle->setTextLocation((OdDgDimTextInfo::TextLocation)(pDbDimStyle->dimtad()));

    switch( pDbDimStyle->dimjust() )
    {
      case 0: pDgDimStyle->setTextJustification(OdDgDimTextInfo::kCenterLeftText); break;
      case 1:
      case 3: pDgDimStyle->setTextJustification(OdDgDimTextInfo::kLeftText); break;
      case 2:
      case 4: pDgDimStyle->setTextJustification(OdDgDimTextInfo::kRightText); break;
    }

    // Arrows and lines

    pDgDimStyle->setWitnessLineExtension(pDbDimStyle->dimexe());
    pDgDimStyle->setWitnessLineOffset(pDbDimStyle->dimexo());

    OdCmColor extColor    = pDbDimStyle->dimclre();
    OdDb::LineWeight  iExtWeight  = pDbDimStyle->dimlwe();
    OdDbObjectId lineTypeId = pDbDimStyle->dimltex1();

    if( extColor.colorMethod() != OdCmEntityColor::kByBlock )
    {
      pDgDimStyle->setExtensionLineColorOverrideFlag(true);
      pDgDimStyle->setExtensionLineColor(getDgnColorIndex(extColor, pDgDimStyleTable->database()));
    }

    if( iExtWeight != OdDb::kLnWtByBlock )
    {
      pDgDimStyle->setExtensionLineWeightOverrideFlag(true);
      pDgDimStyle->setExtensionLineWeight(odDgnExportLineweightMapping(iExtWeight));
    }

    OdUInt32 uDgLineTypeEntryId = getDgnLineStyleEntryId(lineTypeId);

    if( uDgLineTypeEntryId != OdDg::kLineStyleByCell )
    {
      pDgDimStyle->setExtensionLineStyleOverrideFlag(true);
      pDgDimStyle->setExtensionLineStyleId(uDgLineTypeEntryId);
    }

    extColor = pDbDimStyle->dimclrd();
    iExtWeight = pDbDimStyle->dimlwd();
    lineTypeId = pDbDimStyle->dimltype();

    if( extColor.colorMethod() != OdCmEntityColor::kByBlock )
      pDgDimStyle->setDimensionColor(getDgnColorIndex(extColor, pDgDimStyleTable->database()));

    if( iExtWeight != OdDb::kLnWtByBlock )
      pDgDimStyle->setDimensionLineWeight(odDgnExportLineweightMapping(iExtWeight));

    uDgLineTypeEntryId = getDgnLineStyleEntryId(lineTypeId);

    if( uDgLineTypeEntryId != OdDg::kLineStyleByCell )
      pDgDimStyle->setDimensionLineStyleId(uDgLineTypeEntryId);

    // Dimension arrow heads

    OdDbObjectId  idArrow     = pDbDimStyle->dimblk1();
    double        dArrowSize  = pDbDimStyle->dimasz();
    double        dArrowWidth = pDbDimStyle->dimtsz();

    pDgDimStyle->setTerminatorHeight(dArrowSize);
    pDgDimStyle->setTerminatorWidth(dArrowWidth);
    OdDgnExportDimArrowheadType arrowType = getArrowheadDwgType(idArrow);

    OdDgDimension::TerminatorArrowHeadType aType = OdDgDimension::kOpened;

    switch( arrowType )
    {
      case acArrowClosedBlank :
      case acArrowDotSmall:
      case acArrowBoxFilled:
      case acArrowDatumFilled:
      {
        aType = (OdDgDimension::TerminatorArrowHeadType)(OdDgDimension::kClosed | OdDgDimension::kFilled);
      } break;

      case acArrowClosed:
      case acArrowDotBlank:
      case acArrowBoxBlank:
      case acArrowDatumBlank:
      {
        aType = OdDgDimension::kClosed;
      } break;
    }

    pDgDimStyle->setTerminatorArrowHead(aType);

    pDgDimStyleTable->add(pDgDimStyle);
  }

  //========================================================================================================================

  void DgnExporter::copyDimensionStyles(const OdDbDatabase* pDwgDb, OdDgDatabase* pDgnDb)
  {
    if (!pDgnDb || !pDwgDb)
      return;

    OdDbDimStyleTablePtr pDbDimStyleTable = pDwgDb->getDimStyleTableId().openObject(OdDb::kForRead);
    OdDgDimStyleTablePtr pDgDimStyleTable = pDgnDb->getDimStyleTable(OdDg::kForWrite);

    OdDbSymbolTableIteratorPtr pDbDimStyleIter = pDbDimStyleTable->newIterator();

    for(; !pDbDimStyleIter->done(); pDbDimStyleIter->step())
    {
      OdDbDimStyleTableRecordPtr pDbDimStyle = pDbDimStyleIter->getRecord();

      convertDimStyleTableRecord(pDbDimStyle, pDgDimStyleTable);
    }
  }

  //========================================================================================================================

  void DgnExporter::copyMultilineStyles(const OdDbDatabase* pDwgDb, OdDgDatabase* pDgnDb)
  {
    if (!pDgnDb || !pDwgDb)
      return;

    OdDbObjectId idMLineDict = pDwgDb->getMLStyleDictionaryId();

    if (idMLineDict.isNull())
      return;

    OdDbDictionaryPtr pMLDic = idMLineDict.safeOpenObject(OdDb::kForRead);

    if( pMLDic.isNull() )
      return;

    OdDgMultilineStyleTablePtr pDgMLineTable = pDgnDb->getMultilineStyleTable(OdDg::kForWrite);

    OdDbDictionaryIteratorPtr pIt = pMLDic->newIterator();

    for (; !pIt->done(); pIt->next())
    {
      OdDbObjectId idMLineStyle = pIt->objectId();

      if( !idMLineStyle.isNull() )
      {
        OdDbMlineStylePtr pDbMLineStyle = idMLineStyle.openObject(OdDb::kForRead);

        if (pDbMLineStyle.isNull())
          continue;

        OdDgMultilineStyleTableRecordPtr pDgMLineStyle = OdDgMultilineStyleTableRecord::createObject();
        pDgMLineStyle->setName(pDbMLineStyle->name());
        pDgMLineStyle->setFillColorIndex( getDgnColorIndex(pDbMLineStyle->fillColor(), pDgnDb) );
        pDgMLineStyle->setUseFillColorFlag(pDbMLineStyle->filled());

        double    dOffset;
        OdCmColor clrColor;
        OdDbObjectId idDbLineType;

        for( OdInt32 i = 0; i < pDbMLineStyle->numElements(); i++ )
        {
          pDbMLineStyle->getElementAt(i, dOffset, clrColor, idDbLineType);

          OdDgMultilineProfile   dgMLineProfile;
          OdDgMultilineSymbology dgMLineSymbology;
          dgMLineSymbology.setColorIndex( getDgnColorIndex(clrColor, pDgnDb) );
          dgMLineSymbology.setLineStyleEntryId(getDgnLineStyleEntryId(idDbLineType));
          dgMLineSymbology.setUseColorFlag(true);
          dgMLineSymbology.setUseStyleFlag(true);

          dgMLineProfile.setDistance(dOffset);
          dgMLineProfile.setSymbology(dgMLineSymbology);

          pDgMLineStyle->addProfile(dgMLineProfile);
        }

        pDgMLineStyle->setOriginCapAngle(pDbMLineStyle->startAngle()*180/OdaPI);
        pDgMLineStyle->setEndCapAngle(pDbMLineStyle->endAngle()*180/OdaPI);

        OdDgMultilineSymbology symbStartCap;
        symbStartCap.setCapColorFromSegmentFlag(true);
        symbStartCap.setCapLineFlag(pDbMLineStyle->startSquareCap());
        symbStartCap.setCapOutArcFlag(pDbMLineStyle->startRoundCap());
        symbStartCap.setCapInArcFlag(pDbMLineStyle->startInnerArcs());

        pDgMLineStyle->setOriginCap(symbStartCap);

        OdDgMultilineSymbology symbEndCap;
        symbEndCap.setCapColorFromSegmentFlag(true);
        symbEndCap.setCapLineFlag(pDbMLineStyle->endSquareCap());
        symbEndCap.setCapOutArcFlag(pDbMLineStyle->endRoundCap());
        symbEndCap.setCapInArcFlag(pDbMLineStyle->endInnerArcs());

        pDgMLineStyle->setEndCap(symbEndCap);

        OdDgMultilineSymbology symbMidCap;
        symbMidCap.setCapColorFromSegmentFlag(true);
        symbMidCap.setCapLineFlag(pDbMLineStyle->showMiters());

        pDgMLineStyle->setMiddleCap(symbMidCap);

        pDgMLineTable->add(pDgMLineStyle);
      }
    }
  }

  //========================================================================================================================

  void copyVisualStyleProperties( const OdDbVisualStyle* pVS,
                                    OdDgDisplayStyleTableRecord* pDisplayStyle,
                                      OdDgDatabase* pDgnDb)
  {
    OdString strName;
    pVS->name(strName);
    pDisplayStyle->setName(strName);

    const OdGiDisplayStyle& giDisplayStyle = pVS->displayStyle();
    const OdGiFaceStyle&    giFaceStyle    = pVS->faceStyle();
    const OdGiEdgeStyle&    giEdgeStyle    = pVS->edgeStyle();

    pDisplayStyle->setDisplayShadowsFlag(giDisplayStyle.shadowType() != OdGiDisplayStyle::kShadowsNone);
    pDisplayStyle->setUseBackgroundColorOverrideFlag(giDisplayStyle.isDisplaySettingsFlagSet(OdGiDisplayStyle::kBackgrounds));
    pDisplayStyle->setSmoothIgnoreLightsFlag(!giDisplayStyle.isDisplaySettingsFlagSet(OdGiDisplayStyle::kLights));
    pDisplayStyle->setIgnorePatternMapsFlag(!giDisplayStyle.isDisplaySettingsFlagSet(OdGiDisplayStyle::kTextures));

    if( giFaceStyle.lightingModel() == OdGiFaceStyle::kInvisible )
    {
      pDisplayStyle->setDisplayMode(OdDgDisplayStyleTableRecord::kWireframe);
    }
    else if (giFaceStyle.lightingModel() == OdGiFaceStyle::kConstant)
    {
      if( giFaceStyle.faceColorMode() == OdGiFaceStyle::kBackgroundColor )
        pDisplayStyle->setDisplayMode(OdDgDisplayStyleTableRecord::kVisibleEdges);
      else
      {
        pDisplayStyle->setDisplayMode(OdDgDisplayStyleTableRecord::kFilledVisibleEdges);

        if( giFaceStyle.faceColorMode() == OdGiFaceStyle::kMono )
        {
          pDisplayStyle->setUseElementColorOverrideFlag(true);
          pDisplayStyle->setElementColor(getDgnColorIndex(giFaceStyle.monoColor(), pDgnDb));
        }
        else
        {
          pDisplayStyle->setUseElementColorOverrideFlag(false);
        }
      }
    }
    else
    {
      pDisplayStyle->setDisplayMode(OdDgDisplayStyleTableRecord::kShaded);

      if (giFaceStyle.faceColorMode() == OdGiFaceStyle::kMono)
      {
        pDisplayStyle->setUseElementColorOverrideFlag(true);
        pDisplayStyle->setElementColor(getDgnColorIndex(giFaceStyle.monoColor(), pDgnDb));
      }
      else
      {
        pDisplayStyle->setUseElementColorOverrideFlag(false);
      }

      if (giFaceStyle.lightingModel() == OdGiFaceStyle::kNoLighting)
        pDisplayStyle->setSmoothIgnoreLightsFlag(true);
      else
        pDisplayStyle->setSmoothIgnoreLightsFlag(false);

      if( giEdgeStyle.edgeModel() == OdGiEdgeStyle::kNoEdges )
        pDisplayStyle->setDisplayVisibleEdgesFlag(false);
      else
        pDisplayStyle->setDisplayVisibleEdgesFlag(true);
    }

    if (giFaceStyle.isFaceModifierFlagSet(OdGiFaceStyle::kOpacity))
    {
      pDisplayStyle->setUseElementTransparencyOverrideFlag(true);
      pDisplayStyle->setElementTransparency(1.0 - giFaceStyle.opacityLevel());
    }
    else
    {
      pDisplayStyle->setUseElementTransparencyOverrideFlag(false);
    }

    if (giEdgeStyle.isEdgeModifierFlagSet(OdGiEdgeStyle::kColor))
    {
      if (!pDisplayStyle->getUseElementColorOverrideFlag())
      {
        pDisplayStyle->setUseVisibleEdgeColorFlag(true);
        pDisplayStyle->setVisibleEdgeColor(getDgnColorIndex(giEdgeStyle.edgeColor(), pDgnDb));
      }
    }
    else
    {
      pDisplayStyle->setUseVisibleEdgeColorFlag(false);
      pDisplayStyle->setUseElementColorOverrideFlag(false);
    }

    if (giEdgeStyle.isEdgeModifierFlagSet(OdGiEdgeStyle::kWidth))
    {
      pDisplayStyle->setUseVisibleEdgeWeightFlag(true);
      pDisplayStyle->setUseElementLineWeightOverrideFlag(false);
      pDisplayStyle->setVisibleEdgeWeight(giEdgeStyle.edgeWidth());
    }
    else
    {
      pDisplayStyle->setUseVisibleEdgeWeightFlag(false);
      pDisplayStyle->setUseElementLineWeightOverrideFlag(false);
    }

    if( !giEdgeStyle.isEdgeStyleFlagSet(OdGiEdgeStyle::kObscured) )
    {
      pDisplayStyle->setUseHiddenEdgeLineStyleFlag(false);
    }
    else
    {
      pDisplayStyle->setUseHiddenEdgeLineStyleFlag(true);

      switch (giEdgeStyle.obscuredLinetype())
      {
        case OdGiEdgeStyle::kDashed:
          pDisplayStyle->setHiddenEdgeLineStyle(OdDgDisplayStyleTableRecord::kDashed);
        break;
        case OdGiEdgeStyle::kDotted:
        case OdGiEdgeStyle::kSparseDot:
          pDisplayStyle->setHiddenEdgeLineStyle(OdDgDisplayStyleTableRecord::kDotted);
        break;
        case OdGiEdgeStyle::kShortDash:
        case OdGiEdgeStyle::kMediumDash:
          pDisplayStyle->setHiddenEdgeLineStyle(OdDgDisplayStyleTableRecord::kShortDash);
        break;
        case OdGiEdgeStyle::kDoubleShortDash:
        case OdGiEdgeStyle::kDoubleMediumDash:
        case OdGiEdgeStyle::kDoubleLongDash:
        case OdGiEdgeStyle::kMediumLongDash:
          pDisplayStyle->setHiddenEdgeLineStyle(OdDgDisplayStyleTableRecord::kShortLongDash);
          break;
        case OdGiEdgeStyle::kLongDash:
          pDisplayStyle->setHiddenEdgeLineStyle(OdDgDisplayStyleTableRecord::kLongDash);
        break;
        default:
          pDisplayStyle->setHiddenEdgeLineStyle(OdDgDisplayStyleTableRecord::kDefault);
        break;
      }
    }

    pDisplayStyle->setHideInPickerFlag(pVS->isInternalUseOnly());
  }

  //========================================================================================================================

  void DgnExporter::copyVisualStyles(const OdDbDatabase* pDwgDb, OdDgDatabase* pDgnDb)
  {
    OdDbObjectId idVisualStyleDict = pDwgDb->getVisualStyleDictionaryId();

    if (idVisualStyleDict.isNull())
      return;

    OdDbDictionaryPtr pVSDict = idVisualStyleDict.openObject(OdDb::kForRead);

    OdDbDictionaryIteratorPtr pDictIter = pVSDict->newIterator();

    OdDgDisplayStyleTablePtr pDisplayStyleTable = pDgnDb->getDisplayStyleTable(OdDg::kForWrite);

    for(; !pDictIter->done(); pDictIter->next())
    {
      OdDbVisualStylePtr pVS = OdDbVisualStyle::cast(pDictIter->getObject());

      if (pVS.isNull())
        continue;

      try
      {
        OdDgDisplayStyleTableRecordPtr pDisplayStyle = OdDgDisplayStyleTableRecord::createObject();
        copyVisualStyleProperties(pVS, pDisplayStyle, pDgnDb);
        pDisplayStyleTable->add(pDisplayStyle);

        if (!pDisplayStyle.isNull())
          addIdPair(pVS->objectId(), pDisplayStyle->elementId());
        else
          addIdPair(pVS->objectId(), OdDgElementId());
      }
      catch (...)
      {
        ODA_ASSERT_ONCE(!"Display style can't be added.")
      }
    }
  }

  //========================================================================================================================

  void DgnExporter::copyUCSs(const OdDbDatabase* pDwgDb, OdDgDatabase* pDgnDb)
  {
    OdDgElementId idActiveModel = OdDgnExportContext::getDgnModelSpaceModelId();

    if( idActiveModel.isNull() )
      return;

    OdDbObjectId idUCSTable = pDwgDb->getUCSTableId();

    if( idUCSTable.isNull() )
      return;

    OdDbUCSTablePtr pDbUCSTable = idUCSTable.openObject(OdDb::kForRead);

    if( pDbUCSTable.isNull() )
      return;

    OdDbSymbolTableIteratorPtr pIter = pDbUCSTable->newIterator();

    OdDgModelPtr pActiveModel = idActiveModel.openObject(OdDg::kForWrite);

    for(; !pIter->done(); pIter->step())
    {
      OdDbUCSTableRecordPtr pDbUCS = pIter->getRecord();

      if( pDbUCS.isNull() )
        continue;

      OdDgACSPtr pDgACS = OdDgACS::createObject();
      pDgACS->setName(pDbUCS->getName());
      pDgACS->setOrigin(pDbUCS->origin());

      OdGeVector3d vrX = pDbUCS->xAxis();
      OdGeVector3d vrY = pDbUCS->yAxis();
      OdGeMatrix3d matRotation;
      matRotation.setCoordSystem(OdGePoint3d::kOrigin, vrX, vrY, vrX.crossProduct(vrY));
      pDgACS->setRotation(matRotation);
      pDgACS->setType(OdDgModel::kRectangular);

      pActiveModel->addElement(pDgACS);

      OdDgnExportContext::addDgnElement(pDbUCS->objectId(), pDgACS->elementId());
    }

    OdDbObjectId idCurUCS    = pDwgDb->getUCSNAME();
    OdGePoint3d  ptACSOrigin = pDwgDb->getUCSORG();
    OdGeVector3d vrACSvrX = pDwgDb->getUCSXDIR();
    OdGeVector3d vrACSvrY = pDwgDb->getUCSYDIR();

    OdGeMatrix3d matACS;
    matACS.setCoordSystem(OdGePoint3d::kOrigin, vrACSvrX, vrACSvrY, vrACSvrX.crossProduct(vrACSvrY));

    OdDgElementId idACS = OdDgnExportContext::getElementId(idCurUCS);

    pActiveModel->setAcsOrigin(ptACSOrigin);
    pActiveModel->setAcsRotation(matACS);
    pActiveModel->setAcsType(OdDgModel::kRectangular);

    if( !idACS.isNull() )
      pActiveModel->setAcsElementId(idACS);
  }

  //========================================================================================================================

  void DgnExporter::copyNamedViews(const OdDbDatabase* pDwgDb, OdDgDatabase* pDgnDb)
  {
    OdDbObjectId idViewTable = pDwgDb->getViewTableId();

    if (idViewTable.isNull())
      return;

    OdDbViewTablePtr pDbViewTable = idViewTable.openObject(OdDb::kForRead);

    if (pDbViewTable.isNull())
      return;

    OdDgNamedViewTablePtr pNamedViewTable = pDgnDb->getNamedViewTable(OdDg::kForWrite);

    OdDbSymbolTableIteratorPtr pIter = pDbViewTable->newIterator();

    for(; !pIter->done(); pIter->step())
    {
      OdDbViewTableRecordPtr pDbView = pIter->getRecord();

      if (pDbView.isNull())
        continue;

      OdDgViewPtr pDgView = OdDgView::createObject();
      pDgView->setNamedFlag(true);
      pDgView->setName(pDbView->getName());

      OdAbstractViewPEPtr pDbAbstractView = pDbView;
      OdAbstractViewPEPtr pDgAbstractView = pDgView;

      if( !pDgAbstractView.isNull() && !pDbAbstractView.isNull() )
      {
        pDgAbstractView->setView(pDgView, pDbView);
        pNamedViewTable->add(pDgView);
      }
    }
  }

  //========================================================================================================================

}
