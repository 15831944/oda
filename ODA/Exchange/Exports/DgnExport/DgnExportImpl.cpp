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
#include <Ed/EdCommandStack.h>
#include <Ed/EdCommandContext.h>
#include <Ed/EdUserIO.h>
#include <DbCommandContext.h>
#include <OdModuleNames.h>
#include <StaticRxObject.h>
#include "DgnExportImpl.h"
#include <DgDatabase.h>
#include <ExDgnHostAppServices.h>
#include <ExHostAppServices.h>
#include <DbBlockTableRecord.h>
#include <DbLine.h>
#include <DbEllipse.h>
#include <DbCircle.h>
#include <DbArc.h>
#include <DbPolyline.h>
#include <Db2dPolyline.h>
#include <Db3dPolyline.h>
#include <DbSpline.h>
#include <DbText.h>
#include <DbHatch.h>
#include <DbSolid.h>
#include <DbRegion.h>
#include <DbFace.h>
#include <DbPolyFaceMesh.h>
#include <DbPolygonMesh.h>
#include <DbBlockTable.h>
#include <DbBlockTableRecord.h>
#include <DgSharedCellDefinition.h>
#include <DbBlockReference.h>
#include <DbAttributeDefinition.h>
#include <DbAttribute.h>
#include <DbMText.h>
#include <DbMline.h>
#include <Db3dSolid.h>
#include <DbSurface.h>
#include <DbSubDMesh.h>
#include <DbBody.h>
#include <DbOle2Frame.h>
#include <DbRasterImage.h>
#include <Db2LineAngularDimension.h>
#include <Db3PointAngularDimension.h>
#include <DbArcDimension.h>
#include <DbRadialDimension.h>
#include <DbDiametricDimension.h>
#include <DbOrdinateDimension.h>
#include <DbAlignedDimension.h>
#include <DbRadialDimensionLarge.h>
#include <DbRotatedDimension.h>
#include <DbLeader.h>
#include <DbMLeader.h>
#include <DbTable.h>
#include <DbViewport.h>
#include <DbPoint.h>
#include "DgnExportContext.h"
#include "DgnExportPE.h"
#include "DgnExportCurve.h"
#include "DgnExportText.h"
#include "DgnExportPolygon.h"
#include "DgnExportHatch.h"
#include "DgnExportMesh.h"
#include "DgnExportBlockRef.h"
#include "DgnExport3dObjects.h"
#include "DgnExportImages.h"
#include "DgnExportDimensions.h"
#include "DgnExportMisc.h"
#include <OdUtilAds.h>
#include <DbViewportTable.h>
#include <DbViewportTableRecord.h>
#include <DgReferenceAttach.h>
#include <DbLight.h>
#include <DbMPolygon.h>
#include <DbGroup.h>
#include <DgNamedGroup.h>
#include <DgLevelTableRecord.h>
#include <DgMultilineStyleTableRecord.h>
#include <DbWipeout.h>
#include <DbFcf.h>

namespace TD_DGN_EXPORT
{
  //========================================================================================================================

  void setActiveModel(OdDbDatabase* pDwgDb, OdDgDatabase* pDgnDb)
  {
    OdDbObjectId idCurLayout = pDwgDb->currentLayoutId();

    if( !idCurLayout.isNull() )
    {
      OdDbLayoutPtr pLayout = idCurLayout.openObject(OdDb::kForRead);

      if (!pLayout.isNull() && !pLayout->getBlockTableRecordId().isNull())
      {
        OdDgElementId idModel = OdDgnExportContext::getElementId(pLayout->getBlockTableRecordId());

        if (!idModel.isNull())
          pDgnDb->setActiveModelId(idModel);
      }
    }
  }

  //========================================================================================================================

  OdDgnExport::ExportResult DgnExporter::exportDb()
  {
    OdSmartPtr<OdDgHostAppServices> svc = _properties->get_DgnServices();

    if (svc.isNull())
      return OdDgnExport::fail;

    OdDgDatabasePtr pDgnDb = _properties->get_DgnDatabase();

    bool isInitialized = !pDgnDb.isNull();

    if (!isInitialized)
      pDgnDb = svc->createDatabase(OdDgHostAppServices::kDbDefaultModelIs3d);

    OdSmartPtr<OdDbHostAppServices> _services = _properties->get_DwgServices();

    if (_services.isNull())
      _services = OdRxObjectImpl<ExHostAppServices>::createObject();

    try
    {
      OdDbDatabasePtr pDwgDb = _properties->get_DwgDatabase();

      if (pDwgDb.isNull())
        pDwgDb = _services->readFile(_properties->get_DwgPath());

      if (pDwgDb.isNull())
        return OdDgnExport::bad_file;

      // choose active model and prepare export params

      double dMasterUnitsToUORsScale = 1.0;

      OdDgElementId idActiveModel = pDgnDb->getActiveModelId();

      if( idActiveModel.isNull() )
        idActiveModel = pDgnDb->getDefaultModelId();

      if( idActiveModel.isNull() )
      {
        OdDgModelTablePtr pModelTable = pDgnDb->getModelTable();
        OdDgElementIteratorPtr pModelIter = pModelTable->createIterator();

        if (!pModelIter->done())
          idActiveModel = pModelIter->item();
      }

      if( idActiveModel.isNull() )
        return OdDgnExport::fail;

      registerElementLoaders();

      OdDgnExportContext::prepareToExport(this);

      OdDgModelPtr pActiveModel = idActiveModel.openObject(OdDg::kForWrite);

      if (!pActiveModel.isNull())
      {
        setActiveModelSettings(pDwgDb, pActiveModel);

        dMasterUnitsToUORsScale = pActiveModel->convertWorkingUnitsToUORs(1.0);
        OdDgnExportContext::setMasterToUORsScale(dMasterUnitsToUORsScale);
        OdDgnExportContext::set3d(pActiveModel->getModelIs3dFlag());
        OdDgnExportContext::setDgnModelSpaceModelId(pActiveModel->elementId());
      }

      OdDgnExportContext::setBackgroundColor(_properties->get_BackgroundColor());
      OdDgnExportContext::setRecomputeDimensions(_properties->get_RecomputeDimensionsAfterExport());
      OdDgnExportContext::setAllowCloseCurvesFlag(false);

      // Copy table records
      setDwgColorTable(pDgnDb, !OdDgnExportContext::isDarkPalette(), OdDgnExportContext::getBackgroundColor());
      copyRegApps(pDwgDb, pDgnDb);
      copyTextStyles(pDwgDb, pDgnDb, dMasterUnitsToUORsScale); 
      copyMaterials(pDwgDb, pDgnDb);
      copyLineStyles(pDwgDb, pDgnDb, dMasterUnitsToUORsScale);
      copyLayers(pDwgDb, pDgnDb, !isInitialized );
      copyMultilineStyles(pDwgDb, pDgnDb);
      copyDimensionStyles(pDwgDb, pDgnDb);
      copyVisualStyles(pDwgDb, pDgnDb);
      copyNamedViews(pDwgDb, pDgnDb);
      copyUCSs(pDwgDb, pDgnDb);
      copyBlocks(pDwgDb, pDgnDb);
      copyDatabaseDefaults(pDwgDb, pDgnDb);
      //

      copyModelSpace(pDwgDb, pActiveModel);
      copyPaperSpaces(pDwgDb, pDgnDb);

      setActiveModel(pDwgDb, pDgnDb);

      copyGroups(pDwgDb, pDgnDb);

      restoreHatchAssociations();

      restoreDimensionAssociations();

      unregisterElementLoaders();

      m_pDwgDb = pDwgDb.get();
      m_pDgHostAppServices = svc.get();
    }
    catch (OdError& e)
    {
      OdDgnExportContext::restoreAfterExport();
      _services->warning(e);
      return OdDgnExport::fail;
    }
    catch (...)
    {
      OdDgnExportContext::restoreAfterExport();
      throw;
    }

    OdDgnExportContext::restoreAfterExport();

    _properties->put_DgnDatabase(pDgnDb);

    return OdDgnExport::success;
  }

  //========================================================================================================================

  OdDgModel::UnitDescription convertDwgUnits(OdDb::UnitsValue iBaseUnit)
  {
    OdDgModel::UnitDescription result;

    switch( iBaseUnit )
    {
      case OdDb::kUnitsInches:
      {
        result.m_base = OdDgModel::kMeter;
        result.m_system = OdDgModel::kEnglish;
        result.m_numerator = 1e4; 
        result.m_denominator = 254.; 
        result.m_name = "in";
      } break;

      case OdDb::kUnitsFeet:
      {
        result.m_base = OdDgModel::kMeter;
        result.m_system = OdDgModel::kEnglish;
        result.m_numerator = 1e4;
        result.m_denominator = 3048.;
        result.m_name = "ft";
      } break;

      case OdDb::kUnitsMiles:
      {
        result.m_base = OdDgModel::kMeter;
        result.m_system = OdDgModel::kEnglish;
        result.m_numerator = 1e4; 
        result.m_denominator = 16093440.; 
        result.m_name = "mi";
      } break;

      case OdDb::kUnitsMillimeters:
      {
        result.m_base = OdDgModel::kMeter;
        result.m_system = OdDgModel::kMetric;
        result.m_numerator = 1e3; 
        result.m_denominator = 1.; 
        result.m_name = "mm";
      } break;

      case OdDb::kUnitsCentimeters:
      {
        result.m_base = OdDgModel::kMeter;
        result.m_system = OdDgModel::kMetric;
        result.m_numerator = 1e2;
        result.m_denominator = 1.;
        result.m_name = "mm";
      } break;

      case OdDb::kUnitsKilometers:
      {
        result.m_base = OdDgModel::kMeter;
        result.m_system = OdDgModel::kMetric;
        result.m_numerator = 1.; 
        result.m_denominator = 1e3; 
        result.m_name = "km";
      } break;

      case OdDb::kUnitsMicroinches:
      {
        result.m_base = OdDgModel::kMeter;
        result.m_system = OdDgModel::kEnglish;
        result.m_numerator = 1e10; 
        result.m_denominator = 254.;
        result.m_name = "ui";
      } break;

      case OdDb::kUnitsMils:
      {
        result.m_base = OdDgModel::kMeter;
        result.m_system = OdDgModel::kEnglish;
        result.m_numerator = 1e7;
        result.m_denominator = 254.;
        result.m_name = "mil";
      } break;

      case OdDb::kUnitsYards:
      {
        result.m_base = OdDgModel::kMeter;
        result.m_system = OdDgModel::kEnglish;
        result.m_numerator = 1e4; 
        result.m_denominator = 9144.; 
        result.m_name = "yd";
      } break;

      case OdDb::kUnitsNanometers:
      {
        result.m_base = OdDgModel::kMeter;
        result.m_system = OdDgModel::kMetric;
        result.m_numerator = 1e9; 
        result.m_denominator = 1.; 
        result.m_name = "nm";
      } break;

      case OdDb::kUnitsMicrons:
      {
        result.m_base = OdDgModel::kMeter;
        result.m_system = OdDgModel::kMetric;
        result.m_numerator = 1e6; 
        result.m_denominator = 1.; 
        result.m_name = "um";
      } break;

      case OdDb::kUnitsDecimeters:
      {
        result.m_base = OdDgModel::kMeter;
        result.m_system = OdDgModel::kMetric;
        result.m_numerator = 10.;
        result.m_denominator = 1.;
        result.m_name = "dm";
      } break;

      case OdDb::kUnitsDekameters:
      case OdDb::kUnitsHectometers:
      case OdDb::kUnitsGigameters:
      case OdDb::kUnitsAstronomical:
      case OdDb::kUnitsLightYears:
      case OdDb::kUnitsParsecs:
      case OdDb::kUnitsAngstroms:
      {
        result.m_base = OdDgModel::kMeter;
        result.m_system = OdDgModel::kMetric;
        result.m_numerator = 1.0;
        result.m_denominator = oddbGetUnitsConversion(iBaseUnit, OdDb::kUnitsMeters);
      } break;

      case OdDb::kUnitsUSSurveyFeet:
      {
        result.m_base = OdDgModel::kMeter;
        result.m_system = OdDgModel::kEnglish;
        result.m_numerator = 39370.; 
        result.m_denominator = 12000.; 
        result.m_name = "sf";
      } break;

      case OdDb::kUnitsUSSurveyInch:
      case OdDb::kUnitsUSSurveyYard:
      case OdDb::kUnitsUSSurveyMile:
      {
        result.m_base = OdDgModel::kMeter;
        result.m_system = OdDgModel::kEnglish;
        result.m_numerator = 1.0;
        result.m_denominator = oddbGetUnitsConversion(iBaseUnit, OdDb::kUnitsMeters);
      } break;

      default:
      {
        result.m_base = OdDgModel::kMeter;
        result.m_system = OdDgModel::kMetric;
        result.m_numerator = 1.;
        result.m_denominator = 1.;
        result.m_name = "m";
      } break;
    }

    return result;
  }

  //========================================================================================================================

  void DgnExporter::setActiveModelSettings(const OdDbDatabase* pDwgDb, OdDgModel* pDgModel)
  {
    // Master units

    OdDb::UnitsValue iBaseUnit = pDwgDb->getINSUNITS();
    OdDgModel::UnitDescription masterUnits = convertDwgUnits(iBaseUnit);
    pDgModel->setMasterUnit(masterUnits);

    // View settings

    OdDbObjectId idVpTable = pDwgDb->getViewportTableId();

    if (!idVpTable.isNull())
    {
      OdDbViewportTablePtr pVpTable = idVpTable.openObject(OdDb::kForRead);
      OdDbViewportTableRecordPtr pVPR = OdDbViewportTableRecord::cast(pVpTable->getActiveViewportId().safeOpenObject(OdDb::kForWrite));

      if( !pVPR.isNull() )
      {
        OdDgElementId idActiveVG = pDgModel->getViewGroup();

        if( !idActiveVG.isNull() )
        {
          OdDgViewGroupPtr    pVG = idActiveVG.openObject(OdDg::kForWrite);
          OdDgViewPtr pActiveView = pVG->getAt(0).openObject(OdDg::kForWrite);

          OdAbstractViewPEPtr pDgAbstractView = pActiveView;
          OdAbstractViewPEPtr pDbAbstractView = pVPR;

          if (!pDgAbstractView.isNull() && !pDbAbstractView.isNull())
          {
            pDgAbstractView->setView(pActiveView, pVPR);
            pDgAbstractView->setView(pActiveView, pDbAbstractView->target(pVPR), pDbAbstractView->direction(pVPR),
              pDbAbstractView->upVector(pVPR), pDbAbstractView->fieldWidth(pVPR), pDbAbstractView->fieldHeight(pVPR),
              pDbAbstractView->isPerspective(pVPR));
          }

          pActiveView->setShowTextNodesFlag(false);
          pActiveView->setUseTransparencyFlag(true);
          pActiveView->setShowClipFrontFlag(false);
          pActiveView->setShowClipBackFlag(false);
          pActiveView->setShowLineWeightsFlag(pDwgDb->getLWDISPLAY());
        }
      }
    }
  }

  //========================================================================================================================

  bool hasInternalReferences(const OdDbBlockTableRecordPtr& pDbBlock)
  {
    if( OdDgnExportContext::isBlockWithXRefInsert(pDbBlock->id()) )
      return true;

    if( isXRef(pDbBlock) )
      return true;

    OdDbObjectIteratorPtr pIter = pDbBlock->newIterator();

    for(; !pIter->done(); pIter->step())
    {
      OdDbEntityPtr pEnt = pIter->entity();

      if( !pEnt.isNull() && pEnt->isKindOf(OdDbBlockReference::desc()) )
      {
        OdDbBlockReferencePtr pBlockRef = pEnt;

        if( !pBlockRef.isNull() && !pBlockRef->blockTableRecord().isNull() )
        {
          OdDbBlockTableRecordPtr pInternalBlock = pBlockRef->blockTableRecord().openObject(OdDb::kForRead);

          if (!pInternalBlock.isNull() && hasInternalReferences(pInternalBlock))
          {
            OdDgnExportContext::addBlockWithXRefInsert(pDbBlock->id());
            return true;
          }
        }
      }
    }

    return false;
  }

  //========================================================================================================================

  void DgnExporter::copyBlocks(const OdDbDatabase* pDwgDb, OdDgDatabase* pDgnDb)
  {
    if( !pDgnDb || !pDwgDb )
      return;

    OdDbObjectId idDbBlockTable = pDwgDb->getBlockTableId();

    if( idDbBlockTable.isNull() )
      return;

    OdDbBlockTablePtr pBlockTable = idDbBlockTable.openObject(OdDb::kForRead);

    OdDbSymbolTableIteratorPtr pDbIter = pBlockTable->newIterator();

    OdDgSharedCellDefinitionTablePtr pDgBlockTable = pDgnDb->getSharedCellDefinitionTable(OdDg::kForWrite);

    for(; !pDbIter->done(); pDbIter->step())
    {
      OdDbBlockTableRecordPtr pDbBlock = pDbIter->getRecord();

      if( pDbBlock.isNull() || isXRef(pDbBlock) )
        continue;

      if (OdDgnExportContext::isBlockOnProcessing(pDbBlock->id()))
        continue;

      if (!OdDgnExportContext::getElementId(pDbBlock->id()).isNull())
        continue;

      if( hasInternalReferences(pDbBlock) )
        continue;

      OdDgSharedCellDefinitionPtr pDgBlockDef = OdDgSharedCellDefinition::createObject();
      pDgBlockDef->setName(pDbBlock->getName());
      pDgBlockDef->setDescription(pDbBlock->comments());
      pDgBlockDef->setOrigin(pDbBlock->origin());
      pDgBlockDef->set3dFormatFlag(OdDgnExportContext::is3d());
      pDgBlockTable->add(pDgBlockDef);

      double dOldScaleFactor = OdDgnExportContext::getLineStyleScaleCorrectionFactor();
      OdDgnExportContext::setLineStyleScaleCorrectionFactor(1.0 / OdDgnExportContext::getMasterToUORsScale());

      OdDgnExportContext::addProcessingBlockId(pDbBlock->id());
      copyBlock(pDbBlock, pDgBlockDef);
      OdDgnExportContext::removeProcessingBlockId(pDbBlock->id());

      OdDgnExportContext::setLineStyleScaleCorrectionFactor(dOldScaleFactor);

      OdDgnExportContext::addDgnElement(pDbBlock->objectId(), pDgBlockDef->elementId());

      pDgBlockDef->transformBy(OdGeMatrix3d::scaling(OdDgnExportContext::getMasterToUORsScale()));
    }
  }

  //========================================================================================================================

  void getAllViewportClipObjects(const OdDbLayoutPtr& pLayout, 
                                   OdDbObjectIdArray& arrViewports,
                                     const OdDbObjectId& idOverallViewport )
  {
    OdDbObjectId idSpaceBlock = pLayout->getBlockTableRecordId();
    OdDbBlockTableRecordPtr pBlock = idSpaceBlock.openObject(OdDb::kForRead);

    OdDbObjectIteratorPtr pDbObjIter = pBlock->newIterator();

    for (; !pDbObjIter->done(); pDbObjIter->step())
    {
      OdDbEntityPtr pDbEntity = pDbObjIter->entity();

      if (!pDbEntity->isKindOf(OdDbViewport::desc()))
        continue;

      if( pDbEntity->id() == idOverallViewport )
        continue;

      OdDbViewportPtr pViewport = pDbEntity;

      if( pViewport->isNonRectClipOn() && !pViewport->nonRectClipEntityId().isNull() )
      {
        OdDgnExportContext::addViewportClipObject(pViewport->id(), pViewport->nonRectClipEntityId());
        arrViewports.push_back(pViewport->id());
      }
    }
  }

  //========================================================================================================================

  void bindClipElementsToViewports(const OdDbObjectIdArray& arrViewports)
  {
    for( OdUInt32 i = 0; i < arrViewports.size(); i++ )
    {
      OdDgElementId idXRef = OdDgnExportContext::getElementId(arrViewports[i]);

      if (idXRef.isNull())
        continue;

      OdDbObjectId idClipItem = OdDgnExportContext::getViewportClipObject(arrViewports[i]);

      if (idClipItem.isNull())
        continue;

      OdDgElementId idDgClipElm = OdDgnExportContext::getElementId(idClipItem);

      if (idDgClipElm.isNull())
        continue;

      OdDgReferenceAttachmentHeaderPtr pXRef = idXRef.openObject(OdDg::kForWrite);
      pXRef->setClipElement(idDgClipElm);
    }

    OdDgnExportContext::clearViewportClipObjects();
  }

  //========================================================================================================================

  void setPSCamera(OdDgSheetModel* pDgPaperSpace, const OdDbObjectId& idOverallViewport)
  {
    OdDgElementId idActiveVG = pDgPaperSpace->getViewGroup();

    if (!idActiveVG.isNull() )
    {
      if( !idOverallViewport.isNull() )
      {
        OdDbViewportPtr pViewport = idOverallViewport.openObject(OdDb::kForRead);
        OdDgViewGroupPtr    pVG = idActiveVG.openObject(OdDg::kForWrite);
        OdDgViewPtr pActiveView = pVG->getAt(0).openObject(OdDg::kForWrite);

        OdAbstractViewPEPtr pDgAbstractView = pActiveView;
        OdAbstractViewPEPtr pDbAbstractView = pViewport;

        if (!pDgAbstractView.isNull() && !pDbAbstractView.isNull())
        {
          pDgAbstractView->setView(pActiveView, pViewport);
          pDgAbstractView->setView(pActiveView, pDbAbstractView->target(pViewport), pDbAbstractView->direction(pViewport),
            pDbAbstractView->upVector(pViewport), pDbAbstractView->fieldWidth(pViewport), pDbAbstractView->fieldHeight(pViewport),
            pDbAbstractView->isPerspective(pViewport));
        }

        pActiveView->setShowTextNodesFlag(false);
        pActiveView->setShowClipFrontFlag(false);
        pActiveView->setShowClipBackFlag(false);
      }
      else
      {
        pDgPaperSpace->fitToView();
      }
    }
  }

  //========================================================================================================================

  OdDgModel::UnitDescription getSheetUnits(OdDbPlotSettings::PlotPaperUnits uDbUnits )
  {
    if (uDbUnits == OdDbPlotSettings::kMillimeters)
      return convertDwgUnits(OdDb::kUnitsMillimeters);
    else
      return convertDwgUnits(OdDb::kUnitsInches);
  }

  //========================================================================================================================

  void createDefaultViewport(OdDgSheetModelPtr& pDgModel, const OdDbLayout* pLayout)
  {
    OdDbViewportPtr pDefaultViewport = OdDbViewport::createObject();
    pDefaultViewport->setDatabaseDefaults(pLayout->database());

    double dPaperWidth, dPaperHeight;
    double dOffsetX, dOffsetY;
    pLayout->getPlotPaperSize(dPaperWidth, dPaperHeight);
    pLayout->getPlotOrigin(dOffsetX, dOffsetY);

    if( OdZero(dPaperWidth) || OdZero(dPaperHeight) )
    {
      dOffsetX = pLayout->getLIMMIN().x;
      dOffsetY = pLayout->getLIMMIN().y;
      dPaperWidth  = (pLayout->getLIMMAX().x - pLayout->getLIMMIN().x)*0.875;
      dPaperHeight = (pLayout->getLIMMAX().y - pLayout->getLIMMIN().y)*0.875;
    }
    else
    {
      double dWuToMMScale = pDgModel->getMeasuresConversion(OdDgModel::kWuWorldUnit, pDgModel->getWorkingUnit()) / 1000.0;

      double dDen, dNum;
      pLayout->getCustomPrintScale(dDen, dNum);

      double dPlotScale = dNum / dDen * dWuToMMScale;

      dPaperWidth *= dPlotScale;
      dPaperHeight *= dPlotScale;
      dOffsetX *= dPlotScale;
      dOffsetY *= dPlotScale;
    }

    pDefaultViewport->setCenterPoint(OdGePoint3d(dOffsetX + dPaperWidth / 2.0, dOffsetY + dPaperHeight / 2.0, 0.0));
    pDefaultViewport->setWidth(dPaperWidth * 0.8);
    pDefaultViewport->setHeight(dPaperHeight * 0.8);
    pDefaultViewport->setOn();

    OdDbDatabase* pDb = pLayout->database();

    OdDbObjectId idVpTable = pDb->getViewportTableId();

    if (!idVpTable.isNull())
    {
      OdDbViewportTablePtr pVpTable = idVpTable.openObject(OdDb::kForRead);
      OdDbViewportTableRecordPtr pVPR = OdDbViewportTableRecord::cast(pVpTable->getActiveViewportId().safeOpenObject(OdDb::kForWrite));

      if (!pVPR.isNull())
      {
        OdAbstractViewPEPtr pDbAbstractView = pVPR;
        OdAbstractViewPEPtr pViewportAbstractView = pDefaultViewport;

        if (!pViewportAbstractView.isNull() && !pDbAbstractView.isNull())
        {
          pViewportAbstractView->setView(pDefaultViewport, pVPR);

          OdDgnExportPE* pExportPE = (OdDgnExportPE*)pDefaultViewport->queryX(OdDgnExportPE::desc());

          if (pExportPE)
            pExportPE->exportElement(pDefaultViewport, pDgModel);
        }
      }
    }
  }

  //========================================================================================================================

  void DgnExporter::copyPaperSpaces(const OdDbDatabase* pDwgDb, OdDgDatabase* pDgnDb)
  {
    OdDbDictionaryPtr pLayoutDict = pDwgDb->getLayoutDictionaryId().safeOpenObject();
    OdDbDictionaryIteratorPtr pIt = pLayoutDict->newIterator();

    bool bOld3dState = OdDgnExportContext::is3d();
    OdDgnExportContext::set3d(false);

    OdUInt32 iSheetNumber = 1;

    ODCOLORREF uOldBgColor = OdDgnExportContext::getBackgroundColor();

    if( (ODGETRED(uOldBgColor) == 0) && (ODGETGREEN(uOldBgColor) == 0) && (ODGETBLUE(uOldBgColor) == 0) )
      OdDgnExportContext::setBackgroundColor( ODRGB(255,255,255) );

    while (!pIt->done())
    {
      OdDbLayoutPtr pLayout = pIt->getObject();
      pIt->next();

      OdDbObjectId idSpaceBlock = pLayout->getBlockTableRecordId();

      if( idSpaceBlock.isNull() || idSpaceBlock == pDwgDb->getModelSpaceId() )
        continue;

      OdDbBlockTableRecordPtr pDbPaperSpace = idSpaceBlock.openObject(OdDb::kForRead);

      OdDgSheetModelPtr pDgPaperSpace = OdDgSheetModel::createObject();
      pDgPaperSpace->setType(OdDgModel::kSheetModel);
      pDgPaperSpace->setName(pLayout->getLayoutName());
      pDgPaperSpace->setModelIs3dFlag(false);
      pDgPaperSpace->setBackground(OdDgnExportContext::getBackgroundColor());
      pDgPaperSpace->setUseBackgroundColorFlag(true);
      
      OdDgModelTablePtr pModelTable = pDgnDb->getModelTable(OdDg::kForWrite);
      pModelTable->add(pDgPaperSpace);

      // Units

      OdDb::UnitsValue iBaseUnit = pDwgDb->getINSUNITS();
      OdDgModel::UnitDescription masterUnits = convertDwgUnits(iBaseUnit);
      pDgPaperSpace->setMasterUnit(masterUnits);
      pDgPaperSpace->setWorkingUnit(OdDgModel::kWuMasterUnit);

      // Paper configuration

      pDgPaperSpace->setDrawBorderFlag(true);
      pDgPaperSpace->setDrawMarginsFlag(true);
      pDgPaperSpace->setSheetNumber(iSheetNumber++);
      double dPaperWidth, dPaperHeight;
      double dLeftMargin = pLayout->getLeftMargin();
      double dRightMargin = pLayout->getRightMargin(); 
      double dTopMargin = pLayout->getTopMargin(); 
      double dBottomMargin = pLayout->getBottomMargin();
      pLayout->getPlotPaperSize(dPaperWidth, dPaperHeight);
      OdGePoint2d ptPaperOrigin = -1 * pLayout->getPaperImageOrigin();

      if (OdZero(dPaperWidth) || OdZero(dPaperHeight))
      {
        dPaperWidth = 279.4;
        dPaperHeight = 215.9;
      }

      double dDen, dNum;
      pLayout->getCustomPrintScale(dDen, dNum);

      double dPlotScale = dNum / dDen;

      dPaperWidth *= dPlotScale;
      dPaperHeight *= dPlotScale;
      dLeftMargin *= dPlotScale;
      dRightMargin *= dPlotScale;
      dTopMargin *= dPlotScale;
      dBottomMargin *= dPlotScale;

      if( pLayout->plotPaperUnits() != OdDbPlotSettings::kMillimeters )
      {
        dPaperWidth  /= 25.4;
        dPaperHeight /= 25.4;
        dLeftMargin  /= 25.4;
        dRightMargin /= 25.4;
        dTopMargin /= 25.4;
        dBottomMargin /= 25.4;
        ptPaperOrigin /= 25.4;
      }

      switch( pLayout->plotRotation() )
      {
        case OdDbPlotSettings::k90degrees: 
        {
          double dTmp = 0;
          dTmp = dPaperWidth;
          dPaperWidth = dPaperHeight;
          dPaperHeight = dTmp;
          dTmp = dBottomMargin;
          dBottomMargin = dLeftMargin;
          dLeftMargin = dTmp;
          dTmp = dTopMargin;
          dTopMargin = dRightMargin;
          dRightMargin = dTmp;
        } break;

        case OdDbPlotSettings::k180degrees:
        {
          double dTmp = 0;
          dTmp = dBottomMargin;
          dBottomMargin = dTopMargin;
          dTopMargin = dTmp;
          dTmp = dLeftMargin;
          dLeftMargin = dRightMargin;
          dRightMargin = dTmp;
        } break;

        case OdDbPlotSettings::k270degrees:
        {
          double dTmp = 0;
          dTmp = dPaperWidth;
          dPaperWidth = dPaperHeight;
          dPaperHeight = dTmp;
          dTmp = dBottomMargin;
          dBottomMargin = dRightMargin;
          dRightMargin = dTmp;
          dTmp = dTopMargin;
          dTopMargin = dLeftMargin;
          dLeftMargin = dTmp;
        } break;
      }

      pDgPaperSpace->setSheetWidth(dPaperWidth - dLeftMargin - dRightMargin);
      pDgPaperSpace->setSheetHeight(dPaperHeight - dTopMargin - dBottomMargin);
      pDgPaperSpace->setSheetLeftMargin(dLeftMargin);
      pDgPaperSpace->setSheetRightMargin(dRightMargin);
      pDgPaperSpace->setSheetTopMargin(dTopMargin);
      pDgPaperSpace->setSheetBottomMargin(dBottomMargin);
      pDgPaperSpace->setSheetOffset(ptPaperOrigin);

      pDgPaperSpace->setSheetPaperName(pLayout->getCanonicalMediaName());

      OdDgModel::UnitDescription sheetUnits = getSheetUnits(pLayout->plotPaperUnits());
      pDgPaperSpace->setSheetUnits(sheetUnits);

      // View settings

      OdDbObjectId idOverallViewport = pLayout->overallVportId();

      setPSCamera(pDgPaperSpace, idOverallViewport);

      // Content

      pDgPaperSpace->setLinestyleScale(pDwgDb->getLTSCALE());

      OdDbObjectIdArray arrViewports;

      getAllViewportClipObjects(pLayout, arrViewports, idOverallViewport);

      if( !pDgPaperSpace.isNull() )
        copyBlock(pDbPaperSpace, pDgPaperSpace, &idOverallViewport);

      bindClipElementsToViewports(arrViewports);

      try
      {
        if (arrViewports.isEmpty() && idOverallViewport.isNull())
          createDefaultViewport(pDgPaperSpace, pLayout);
      }
      catch (...)
      {
        ODA_ASSERT_ONCE(!"Can't create default viewport")
      }

      OdDgnExportContext::addDgnElement(pDbPaperSpace->objectId(), pDgPaperSpace->elementId());
    }

    OdDgnExportContext::setBackgroundColor(uOldBgColor);
    OdDgnExportContext::set3d(bOld3dState);
  }

  //========================================================================================================================

  void DgnExporter::copyDatabaseDefaults(const OdDbDatabase* pDwgDb, OdDgDatabase* pDgnDb)
  {
    pDgnDb->setActiveColorIndex(getDgnColorIndex(pDwgDb->getCECOLOR(), pDgnDb));
    pDgnDb->setActiveLineStyleEntryId(getDgnLineStyleEntryId(pDwgDb->getCELTYPE()));
    pDgnDb->setActiveLineWeight(odDgnExportLineweightMapping(pDwgDb->getCELWEIGHT()));

    OdDgElementId idLevel = OdDgnExportContext::getElementId(pDwgDb->getCLAYER());

    if( !idLevel.isNull() )
    {
      OdDgLevelTableRecordPtr pLevel = idLevel.openObject(OdDg::kForRead);

      if (!pLevel.isNull())
        pDgnDb->setActiveLevelEntryId(pLevel->getEntryId());
    }

    OdDgElementId idDefaultDimStyle = OdDgnExportContext::getElementId(pDwgDb->getDIMSTYLE());

    if( !idDefaultDimStyle.isNull() )
    {
      OdDgDimStyleTablePtr pDimStyleTable = pDgnDb->getDimStyleTable(OdDg::kForWrite);
      OdDgDimStyleTableRecordPtr pDimStyle = idDefaultDimStyle.openObject(OdDg::kForRead);

      if (!pDimStyle.isNull())
        pDimStyleTable->setActiveDimensionStyle(pDimStyle);
    }

    OdDgElementId idMLStyleDefault = OdDgnExportContext::getElementId(pDwgDb->getCMLSTYLE());

    if( !idMLStyleDefault.isNull() )
    {
      OdDgMultilineStyleTableRecordPtr pMLStyle = idMLStyleDefault.openObject(OdDg::kForWrite);
      OdDgMultilineStyleTablePtr pMLStyleTable = pDgnDb->getMultilineStyleTable(OdDg::kForWrite);

      if( !pMLStyle.isNull() )
        pMLStyleTable->setActiveMultilineStyle(pMLStyle);
    }

    OdDgElementId idTextStyleDefault = OdDgnExportContext::getElementId(pDwgDb->getTEXTSTYLE());

    if( !idTextStyleDefault.isNull() )
    {
      OdDgTextStyleTableRecordPtr pTextStyle = idTextStyleDefault.openObject(OdDg::kForRead);
      pDgnDb->setActiveTextStyleEntryId(pTextStyle->getEntryId());
    }
  }

  //========================================================================================================================

  void DgnExporter::copyGroups(const OdDbDatabase* pDwgDb, OdDgDatabase* pDgnDb)
  {
    OdDbObjectId idGroupDict = pDwgDb->getGroupDictionaryId(false);

    if( idGroupDict.isNull() )
      return;

    OdDbDictionaryPtr pGroupDict = idGroupDict.safeOpenObject();

    if (pGroupDict.isNull())
      return;

    OdDbDictionaryIteratorPtr pGrIter = pGroupDict->newIterator();

    for(; !pGrIter->done(); pGrIter->next())
    {
      OdDbGroupPtr pDbGroup = pGrIter->getObject();

      if (pDbGroup.isNull())
        continue;

      OdDgNamedGroupHeaderPtr pDgGroup = OdDgNamedGroupHeader::createObject();
      pDgGroup->setName(pDbGroup->name());
      pDgGroup->setSelectMembersFlag(true);
      
      OdDbObjectIdArray arrDbItems;
      pDbGroup->allEntityIds(arrDbItems);

      bool bCreateGroup = true;
      OdDgElementId idModelOfGroup;

      for( OdUInt32 i = 0; i < arrDbItems.size(); i++ )
      {
        OdDgElementId idElm = OdDgnExportContext::getElementId(arrDbItems[i]);

        if (idElm.isNull())
        {
          bCreateGroup = false;
          break;
        }

        OdDgElementPtr pElm = idElm.openObject(OdDg::kForRead);

        if( idModelOfGroup.isNull() )
          idModelOfGroup = getOwnerModelId(pElm);

        OdDgNamedGroupItem newItem;
        OdArray<OdUInt64> arrPath;
        arrPath.push_back((OdUInt64)(idElm.getHandle()));
        newItem.setPath(arrPath);

        pDgGroup->addNamedGroupItem(newItem);
      }

      if( bCreateGroup && !idModelOfGroup.isNull() )
      {
        OdDgModelPtr pModel = idModelOfGroup.openObject(OdDg::kForWrite);
        pModel->addElement(pDgGroup);
      }
    }
  }

  //========================================================================================================================

  void DgnExporter::copyModelSpace(const OdDbDatabase* pDwgDb, OdDgModel* pActiveModel)
  {
    if (!pActiveModel || !pDwgDb)
      return;

    OdDbObjectId idModelSpace = pDwgDb->getModelSpaceId();

    if (idModelSpace.isNull())
      return;

    OdDbBlockTableRecordPtr pModelSpace = idModelSpace.openObject(OdDb::kForRead);

    pActiveModel->setLinestyleScale(pDwgDb->getLTSCALE());

    if (!pModelSpace.isNull())
      copyBlock(pModelSpace, pActiveModel);

    OdDgnExportContext::addDgnElement(pModelSpace->objectId(), pActiveModel->elementId());
  }

  //========================================================================================================================

  void DgnExporter::copyBlock(const OdDbBlockTableRecord* pDwgBlock, OdDgElement* pDgnBlock, OdDbObjectId* idOverallVP )
  {
    if (!pDwgBlock || !pDgnBlock)
      return;

    OdGePoint3d ptOffset = pDwgBlock->origin();

    OdDbObjectIteratorPtr pDbObjIter = pDwgBlock->newIterator(true, true, true);

    for(; !pDbObjIter->done(); pDbObjIter->step() )
    {
      OdDbEntityPtr pDbEntity = pDbObjIter->entity();

      if (idOverallVP && (pDbEntity->id() == *idOverallVP))
        continue;

      if( !pDbEntity.isNull() )
      {
        OdDgnExportPE* pExportPE = (OdDgnExportPE*)pDbEntity->queryX(OdDgnExportPE::desc());

        if (pExportPE)
          pExportPE->exportElement(pDbEntity, pDgnBlock);
      }
    }

    if (!ptOffset.isEqualTo(OdGePoint3d::kOrigin))
      pDgnBlock->transformBy(OdGeMatrix3d::translation(-ptOffset.asVector()));

  }

  //========================================================================================================================

  DgnExporter::DgnExporter() : _properties(DgnExportProperties::createObject())
  {
  }

  //========================================================================================================================

  void DgnExporter::addIdPair(const OdDbObjectId& idDwg, const OdDgElementId& idDgn)
  {
    m_idMap[idDwg] = idDgn;
  }

  //========================================================================================================================

  OdUInt32 DgnExporter::getDgnIndexByLineWeight(OdDb::LineWeight uLineWeight)
  {
    OdDgnExportLineWeightsMapPtr pLWMap = _properties->get_LineWeightsMap();
    return pLWMap->getDgnIndexByDwgLineWeight(uLineWeight);
  }

  //========================================================================================================================

  OdDgElementId DgnExporter::getDgnElementId(const OdDbObjectId& idDwg)
  {
    OdDgElementId retVal;

    IdDbToDgMap::const_iterator pIter = m_idMap.find(idDwg);

    if (pIter != m_idMap.end())
      retVal = pIter->second;

    return retVal;
  }

  //========================================================================================================================

  OdRxDictionaryPtr DgnExporter::properties() { return _properties; }

  //========================================================================================================================

  void DgnExporter::registerElementLoaders()
  {
    if( OdDgnExportContext::getInternalExportFlag())
      return;

    setExportPEToElements();
  }

  //========================================================================================================================

  void DgnExporter::unregisterElementLoaders()
  {
    if( OdDgnExportContext::getInternalExportFlag() )
      return;

    removeExportPEFromElements();
  }

  //========================================================================================================================

  void DgnExporter::setExportPEToElements()
  {
    static OdStaticRxObject<OdDgnExportPE> defaultExportPE;
    OdDbEntity::desc()->addX(OdDgnExportPE::desc(), &defaultExportPE);

    static OdStaticRxObject<OdDbLineExportPE> dbLineExportPE;
    OdDbLine::desc()->addX(OdDgnExportPE::desc(), &dbLineExportPE);

    static OdStaticRxObject<OdDbCircleExportPE> dbCircleExportPE;
    OdDbCircle::desc()->addX(OdDgnExportPE::desc(), &dbCircleExportPE);

    static OdStaticRxObject<OdDbEllipseExportPE> dbEllipseExportPE;
    OdDbEllipse::desc()->addX(OdDgnExportPE::desc(), &dbEllipseExportPE);

    static OdStaticRxObject<OdDbArcExportPE> dbArcExportPE;
    OdDbArc::desc()->addX(OdDgnExportPE::desc(), &dbArcExportPE);

    static OdStaticRxObject<OdDbPolylineExportPE> dbPolylineExportPE;
    OdDbPolyline::desc()->addX(OdDgnExportPE::desc(), &dbPolylineExportPE);

    static OdStaticRxObject<OdDb3dPolylineExportPE> db3dPolylineExportPE;
    OdDb3dPolyline::desc()->addX(OdDgnExportPE::desc(), &db3dPolylineExportPE);

    static OdStaticRxObject<OdDb2dPolylineExportPE> db2dPolylineExportPE;
    OdDb2dPolyline::desc()->addX(OdDgnExportPE::desc(), &db2dPolylineExportPE);

    static OdStaticRxObject<OdDbBSplineCurveExportPE> dbSplineExportPE;
    OdDbSpline::desc()->addX(OdDgnExportPE::desc(), &dbSplineExportPE);

    static OdStaticRxObject<OdDbRayExportPE> dbRayExportPE;
    OdDbRay::desc()->addX(OdDgnExportPE::desc(), &dbRayExportPE);

    static OdStaticRxObject<OdDbXLineExportPE> dbXLineExportPE;
    OdDbXline::desc()->addX(OdDgnExportPE::desc(), &dbXLineExportPE);

    static OdStaticRxObject<OdDbTextExportPE> dbTextExportPE;
    OdDbText::desc()->addX(OdDgnExportPE::desc(), &dbTextExportPE);

    static OdStaticRxObject<OdDbHatchExportPE> dbHatchExportPE;
    OdDbHatch::desc()->addX(OdDgnExportPE::desc(), &dbHatchExportPE);

    static OdStaticRxObject<OdDbSolidExportPE> dbSolidExportPE;
    OdDbSolid::desc()->addX(OdDgnExportPE::desc(), &dbSolidExportPE);

    static OdStaticRxObject<OdDbRegionExportPE> dbRegionExportPE;
    OdDbRegion::desc()->addX(OdDgnExportPE::desc(), &dbRegionExportPE);

    static OdStaticRxObject<OdDbFaceExportPE> dbFaceExportPE;
    OdDbFace::desc()->addX(OdDgnExportPE::desc(), &dbFaceExportPE);

    static OdStaticRxObject<OdDbPolyfaceMeshExportPE> dbPolyFaceMeshExportPE;
    OdDbPolyFaceMesh::desc()->addX(OdDgnExportPE::desc(), &dbPolyFaceMeshExportPE);

    static OdStaticRxObject<OdDbPolygonMeshExportPE> dbPolygonMeshExportPE;
    OdDbPolygonMesh::desc()->addX(OdDgnExportPE::desc(), &dbPolygonMeshExportPE);

    static OdStaticRxObject<OdDbBlockReferenceExportPE> dbBlockRefExportPE;
    OdDbBlockReference::desc()->addX(OdDgnExportPE::desc(), &dbBlockRefExportPE);

    static OdStaticRxObject<OdDbMTextExportPE> dbMTextExportPE;
    OdDbMText::desc()->addX(OdDgnExportPE::desc(), &dbMTextExportPE);

    static OdStaticRxObject<OdDbAttributeDefinitionExportPE> dbAttrDefExportPE;
    OdDbAttributeDefinition::desc()->addX(OdDgnExportPE::desc(), &dbAttrDefExportPE);

    static OdStaticRxObject<OdDbMLineExportPE> dbMLineExportPE;
    OdDbMline::desc()->addX(OdDgnExportPE::desc(), &dbMLineExportPE);

    static OdStaticRxObject<OdDb3dSolidExportPE> db3dSolidExportPE;
    OdDb3dSolid::desc()->addX(OdDgnExportPE::desc(), &db3dSolidExportPE);

    static OdStaticRxObject<OdDb3dSurfaceExportPE> db3dSurfaceExportPE;
    OdDbSurface::desc()->addX(OdDgnExportPE::desc(), &db3dSurfaceExportPE);

    static OdStaticRxObject<OdDbSubDMeshExportPE> dbSubDMeshExportPE;
    OdDbSubDMesh::desc()->addX(OdDgnExportPE::desc(), &dbSubDMeshExportPE);

    static OdStaticRxObject<OdDbBodyExportPE> dbBodyExportPE;
    OdDbBody::desc()->addX(OdDgnExportPE::desc(), &dbBodyExportPE);

    static OdStaticRxObject<OdDbNurbSurfaceExportPE> dbNurbSurfaceExportPE;
    OdDbNurbSurface::desc()->addX(OdDgnExportPE::desc(), &dbNurbSurfaceExportPE);

    static OdStaticRxObject<OdDbRasterImageExportPE> dbRasterImageExportPE;
    OdDbRasterImage::desc()->addX(OdDgnExportPE::desc(), &dbRasterImageExportPE);

    static OdStaticRxObject<OdDbOle2FrameExportPE> dbOle2FrameExportPE;
    OdDbOle2Frame::desc()->addX(OdDgnExportPE::desc(), &dbOle2FrameExportPE);

    static OdStaticRxObject<OdDb2LineAngularDimensionExportPE> db2LineAngularDimensionExportPE;
    OdDb2LineAngularDimension::desc()->addX(OdDgnExportPE::desc(), &db2LineAngularDimensionExportPE);

    static OdStaticRxObject<OdDb3PointAngularDimensionExportPE> db3PointAngularDimensionExportPE;
    OdDb3PointAngularDimension::desc()->addX(OdDgnExportPE::desc(), &db3PointAngularDimensionExportPE);

    static OdStaticRxObject<OdDbArcDimensionExportPE> dbArcDimensionExportPE;
    OdDbArcDimension::desc()->addX(OdDgnExportPE::desc(), &dbArcDimensionExportPE);

    static OdStaticRxObject<OdDbRadialDimensionExportPE> dbRadialDimensionExportPE;
    OdDbRadialDimension::desc()->addX(OdDgnExportPE::desc(), &dbRadialDimensionExportPE);

    static OdStaticRxObject<OdDbDiametricDimensionExportPE>  dbDiametricDimensionExportPE;
    OdDbDiametricDimension::desc()->addX(OdDgnExportPE::desc(), &dbDiametricDimensionExportPE);

    static OdStaticRxObject<OdDbOrdinateDimensionExportPE> dbOrdinateDimensionExportPE;
    OdDbOrdinateDimension::desc()->addX(OdDgnExportPE::desc(), &dbOrdinateDimensionExportPE);

    static OdStaticRxObject<OdDbAlignedDimensionExportPE> dbAlignedDimensionExportPE;
    OdDbAlignedDimension::desc()->addX(OdDgnExportPE::desc(), &dbAlignedDimensionExportPE);

    static OdStaticRxObject<OdDbRadialDimensionLargeExportPE> dbRadialDimensionLargeExportPE;
    OdDbRadialDimensionLarge::desc()->addX(OdDgnExportPE::desc(), &dbRadialDimensionLargeExportPE);

    static OdStaticRxObject<OdDbRotatedDimensionExportPE> dbRotatedDimensionExportPE;
    OdDbRotatedDimension::desc()->addX(OdDgnExportPE::desc(), &dbRotatedDimensionExportPE);

    static OdStaticRxObject<OdDbLeaderExportPE> dbLeaderExportPE;
    OdDbLeader::desc()->addX(OdDgnExportPE::desc(), &dbLeaderExportPE);

    static OdStaticRxObject<OdDbMLeaderExportPE> dbMLeaderExportPE;
    OdDbMLeader::desc()->addX(OdDgnExportPE::desc(), &dbMLeaderExportPE);

    static OdStaticRxObject<OdDbTableExportPE> dbTableExportPE;
    OdDbTable::desc()->addX(OdDgnExportPE::desc(), &dbTableExportPE);

    static OdStaticRxObject<OdDbViewportExportPE> dbViewportExportPE;
    OdDbViewport::desc()->addX(OdDgnExportPE::desc(), &dbViewportExportPE);

    static OdStaticRxObject<OdDbLightExportPE> dbLightExportPE;
    OdDbLight::desc()->addX(OdDgnExportPE::desc(), &dbLightExportPE);

    static OdStaticRxObject<OdDbPointExportPE> dbPointExportPE;
    OdDbPoint::desc()->addX(OdDgnExportPE::desc(), &dbPointExportPE);

    static OdStaticRxObject<OdDbMPolygonExportPE> dbMPolygonExportPE;
    OdDbMPolygon::desc()->addX(OdDgnExportPE::desc(), &dbMPolygonExportPE);
    
    static OdStaticRxObject<OdDbWipeoutExportPE> dbWipeoutExportPE;
    OdDbWipeout::desc()->addX(OdDgnExportPE::desc(), &dbWipeoutExportPE);

    static OdStaticRxObject<OdDbFcfExportPE> dbFcfExportPE;
    OdDbFcf::desc()->addX(OdDgnExportPE::desc(), &dbFcfExportPE);
  }

  //========================================================================================================================

  void DgnExporter::removeExportPEFromElements()
  {
    OdDbEntity::desc()->delX(OdDgnExportPE::desc());
    OdDbLine::desc()->delX(OdDgnExportPE::desc());
    OdDbCircle::desc()->delX(OdDgnExportPE::desc());
    OdDbEllipse::desc()->delX(OdDgnExportPE::desc());
    OdDbArc::desc()->delX(OdDgnExportPE::desc());
    OdDbPolyline::desc()->delX(OdDgnExportPE::desc());
    OdDb3dPolyline::desc()->delX(OdDgnExportPE::desc());
    OdDb2dPolyline::desc()->delX(OdDgnExportPE::desc());
    OdDbSpline::desc()->delX(OdDgnExportPE::desc());
    OdDbRay::desc()->delX(OdDgnExportPE::desc());
    OdDbXline::desc()->delX(OdDgnExportPE::desc());
    OdDbText::desc()->delX(OdDgnExportPE::desc());
    OdDbHatch::desc()->delX(OdDgnExportPE::desc());
    OdDbSolid::desc()->delX(OdDgnExportPE::desc());
    OdDbRegion::desc()->delX(OdDgnExportPE::desc());
    OdDbFace::desc()->delX(OdDgnExportPE::desc());
    OdDbPolyFaceMesh::desc()->delX(OdDgnExportPE::desc());
    OdDbPolygonMesh::desc()->delX(OdDgnExportPE::desc());
    OdDbBlockReference::desc()->delX(OdDgnExportPE::desc());
    OdDbAttributeDefinition::desc()->delX(OdDgnExportPE::desc());
    OdDbMText::desc()->delX(OdDgnExportPE::desc());
    OdDbMline::desc()->delX(OdDgnExportPE::desc());
    OdDb3dSolid::desc()->delX(OdDgnExportPE::desc());
    OdDbSurface::desc()->delX(OdDgnExportPE::desc());
    OdDbSubDMesh::desc()->delX(OdDgnExportPE::desc());
    OdDbBody::desc()->delX(OdDgnExportPE::desc());
    OdDbNurbSurface::desc()->delX(OdDgnExportPE::desc());
    OdDbRasterImage::desc()->delX(OdDgnExportPE::desc());
    OdDbOle2Frame::desc()->delX(OdDgnExportPE::desc());
    OdDb2LineAngularDimension::desc()->delX(OdDgnExportPE::desc());
    OdDb3PointAngularDimension::desc()->delX(OdDgnExportPE::desc());
    OdDbArcDimension::desc()->delX(OdDgnExportPE::desc());
    OdDbRadialDimension::desc()->delX(OdDgnExportPE::desc());
    OdDbDiametricDimension::desc()->delX(OdDgnExportPE::desc());
    OdDbOrdinateDimension::desc()->delX(OdDgnExportPE::desc());
    OdDbAlignedDimension::desc()->delX(OdDgnExportPE::desc());
    OdDbRadialDimensionLarge::desc()->delX(OdDgnExportPE::desc());
    OdDbRotatedDimension::desc()->delX(OdDgnExportPE::desc());
    OdDbLeader::desc()->delX(OdDgnExportPE::desc());
    OdDbMLeader::desc()->delX(OdDgnExportPE::desc());
    OdDbTable::desc()->delX(OdDgnExportPE::desc());
    OdDbViewport::desc()->delX(OdDgnExportPE::desc());
    OdDbLight::desc()->delX(OdDgnExportPE::desc());
    OdDbPoint::desc()->delX(OdDgnExportPE::desc());
    OdDbMPolygon::desc()->delX(OdDgnExportPE::desc());
    OdDbWipeout::desc()->delX(OdDgnExportPE::desc());
    OdDbFcf::desc()->delX(OdDgnExportPE::desc());
  }

  //========================================================================================================================

  OdRxDictionaryPtr DgnExportProperties::createObject()
  {
    return OdRxObjectImpl<DgnExportProperties, OdRxDictionary>::createObject();
  }

  ODRX_DECLARE_PROPERTY(DwgPath)
  ODRX_DECLARE_PROPERTY(DgnDatabase)
  ODRX_DECLARE_PROPERTY(DwgDatabase)
  ODRX_DECLARE_PROPERTY(DwgServices)
  ODRX_DECLARE_PROPERTY(DgnServices)
  ODRX_DECLARE_PROPERTY(BackgroundColor)
  ODRX_DECLARE_PROPERTY(RecomputeDimensionsAfterExport)
  ODRX_DECLARE_PROPERTY(LineWeightsMap)

  ODRX_DEFINE_PROPERTY(DwgPath, DgnExportProperties, getString)
  ODRX_DEFINE_PROPERTY_OBJECT(DgnDatabase, DgnExportProperties, get_DgnDatabase, put_DgnDatabase, OdDgDatabase)
  ODRX_DEFINE_PROPERTY_OBJECT(DwgDatabase, DgnExportProperties, get_DwgDatabase, put_DwgDatabase, OdDbDatabase)
  ODRX_DEFINE_PROPERTY_OBJECT(DwgServices, DgnExportProperties, get_DwgServices, put_DwgServices, OdDbHostAppServices)
  ODRX_DEFINE_PROPERTY_OBJECT(DgnServices, DgnExportProperties, get_DgnServices, put_DgnServices, OdDgHostAppServices)
  ODRX_DEFINE_PROPERTY(BackgroundColor, DgnExportProperties, getInt32)
  ODRX_DEFINE_PROPERTY(RecomputeDimensionsAfterExport, DgnExportProperties, getBool)
  ODRX_DEFINE_PROPERTY_OBJECT(LineWeightsMap, DgnExportProperties, get_LineWeightsMap, put_LineWeightsMap, OdDgnExportLineWeightsMap)

  ODRX_BEGIN_DYNAMIC_PROPERTY_MAP(DgnExportProperties);
    ODRX_GENERATE_PROPERTY(DwgPath)
    ODRX_GENERATE_PROPERTY(DgnDatabase)
    ODRX_GENERATE_PROPERTY(DwgDatabase)
    ODRX_GENERATE_PROPERTY(DwgServices)
    ODRX_GENERATE_PROPERTY(DgnServices)
    ODRX_GENERATE_PROPERTY(BackgroundColor)
    ODRX_GENERATE_PROPERTY(RecomputeDimensionsAfterExport)
    ODRX_GENERATE_PROPERTY(LineWeightsMap)
  ODRX_END_DYNAMIC_PROPERTY_MAP(DgnExportProperties);

#if defined(_MSC_VER) && (_MSC_VER >= 1300)
#pragma warning(disable:4355)
#endif

class DgnExportModule : public OdDgnExportModule
{

 public:
   virtual void initApp()
   {
     OdDgnExportPE::rxInit();
   }
   virtual void uninitApp()
   {
     OdDgnExportPE::rxUninit();
   }

   virtual OdDgnExportPtr create()
   {
     return OdRxObjectImpl<DgnExporter>::createObject();
   }
 };

//========================================================================================================================

}

//========================================================================================================================

using namespace TD_DGN_EXPORT;
ODRX_DEFINE_DYNAMIC_MODULE(DgnExportModule)

//========================================================================================================================

