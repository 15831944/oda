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
#include "DgnExportText.h"
#include "DgnExportBlockRef.h"
#include "DgnExportImpl.h"
#include "DgnExportCommon.h"
#include "DgnExportContext.h"
#include "DgnExportImages.h"
#include <DgSharedCellReference.h>
#include <DbBlockReference.h>
#include <DbBlockTableRecord.h>
#include <DbAttributeDefinition.h>
#include <DbAttribute.h>
#include <DgTagSetDefinition.h>
#include <DgTagElement.h>
#include <DgText.h>
#include <DgTextNode.h>
#include <DbDictionary.h>
#include <DbRasterImage.h>
#include <DbSpatialFilter.h>
#include <DgReferenceAttach.h>
#include <DgLevelTableRecord.h>
#include <DbViewport.h>
#include <DbLayerTable.h>
#include <DbLayerTableRecord.h>
#include <DgLevelMask.h>
#include <ModelDocObj/DbViewRepBlockReference.h>

namespace TD_DGN_EXPORT {

//---------------------------------------------------------------------------------------------------

OdDbObjectId getAttrDefBlockId(OdDbAttributeDefinition* pDbAttrDef)
{
  OdDbObjectId idBlock = pDbAttrDef->ownerId();
  OdDbObjectPtr pBlockObj = idBlock.safeOpenObject();

  while (!pBlockObj.isNull() && !pBlockObj->isKindOf(OdDbBlockTableRecord::desc()))
  {
    idBlock = pBlockObj->ownerId();
    pBlockObj = idBlock.safeOpenObject();
  }

  return idBlock;
}

//---------------------------------------------------------------------------------------------------

void OdDbAttributeDefinitionExportPE::subExportElement(OdDbEntity* pDbEntity, OdDgElement* owner)
{
  OdDbAttributeDefinition* pDbAttrDef = (OdDbAttributeDefinition*)(pDbEntity);

  OdDbObjectId idAttrDefBlock = getAttrDefBlockId(pDbAttrDef);

  if (idAttrDefBlock.isNull())
    return;

  OdDgElementId idDgAttrDefSet = OdDgnExportContext::getAttrDefSetForBlock(idAttrDefBlock);

  if( idDgAttrDefSet.isNull() )
  {
    OdDbBlockTableRecordPtr pBlock = idAttrDefBlock.openObject(OdDb::kForRead);
    OdDgTagDefinitionSetTablePtr pTagSetTable = owner->database()->getTagDefinitionSetTable(OdDg::kForWrite);

    OdDgTagDefinitionSetPtr pNewSet = OdDgTagDefinitionSet::createObject();

    OdString strDefSetNameBase = pBlock->getName() + L"_Attrs";

    OdString strDefSetName = strDefSetNameBase;

    if (strDefSetName.getLength() > 32)
      strDefSetNameBase.left(32);

    if (strDefSetNameBase.getLength() > 29)
      strDefSetNameBase.left(29);

    OdUInt32 uCount = 0;

    while( !pTagSetTable->getAt(strDefSetName).isNull() )
    {
      uCount++;
      strDefSetName.format(L"_%d", uCount);
      strDefSetName = strDefSetNameBase + strDefSetName;
    }

    pNewSet->setName(strDefSetName);
    pTagSetTable->add(pNewSet);
    OdDgnExportContext::bindAttrDefSetToBlock(pNewSet->elementId(), idAttrDefBlock);
    idDgAttrDefSet = pNewSet->elementId();
  }


  if (idDgAttrDefSet.isNull())
    return;

  OdDgTagDefinitionSetPtr pDgBlockSet = idDgAttrDefSet.openObject(OdDg::kForWrite);

  // Apppend tag definition

  OdString strKey = pDbAttrDef->tag();

  if( !pDgBlockSet->getByName(strKey).isNull() )
  {
    OdString strNewKey;
    OdUInt32 uIndex = 1;
    do
    {
      strNewKey.format(L"_%d", uIndex);
      strNewKey = strKey + strNewKey;
      uIndex++;
    } 
    while (!pDgBlockSet->getByName(strNewKey).isNull());

    strKey = strNewKey;
  }

  pDgBlockSet->addDefinition(OdDgTagDefinition::kChar, strKey);

  OdDgTagDefinitionPtr pTagDef = pDgBlockSet->getByName(strKey);

  if (pTagDef.isNull())
    return;

  pTagDef->setPrompt(pDbAttrDef->prompt());

  if (pDbAttrDef->isMTextAttributeDefinition())
  {
    OdDbMTextPtr pMText = pDbAttrDef->getMTextAttributeDefinition();

    if( pMText.isNull() )
      pTagDef->setString(pMText->contents());
    else
      pTagDef->setString(pDbAttrDef->textString());
  }
  else
    pTagDef->setString(pDbAttrDef->textString());

  pTagDef->setDisplayTagFlag(!pDbAttrDef->isInvisible());
  pTagDef->setVariableFlag(!pDbAttrDef->isConstant());
  pTagDef->setConfirmFlag(pDbAttrDef->isVerifiable());
  pTagDef->setDefaultFlag(true);

  // Append tag

  OdDgTagElementPtr pTag = OdDgTagElement::createObject();
  pTag->setDatabaseDefaults(owner->database());
  pTag->set3dFlag(OdDgnExportContext::is3d());
  pTag->setTagDefinitionId(pDgBlockSet->elementId(), pTagDef->getEntryId());
  pTag->setAssociatedFlag(false);
  pTag->setOffsetUsedFlag(false);
  pTag->setJustificationUsedFlag(true);
  pTag->setHideForSharedCellFlag(true);
  pTag->setOrigin(pDbAttrDef->position());

  if( OdDgnExportContext::is3d() )
  {
    OdGeMatrix3d matRotation;
    OdGeVector3d vrZAxis = pDbAttrDef->normal();
    OdGeVector3d vrXAxis = OdGeVector3d::kXAxis;
    double dRotation = pDbAttrDef->rotation();

    if (fabs(dRotation) < 1.e16)
      vrXAxis.rotateBy(dRotation, OdGeVector3d::kZAxis);

    vrXAxis.transformBy(OdGeMatrix3d::planeToWorld(vrZAxis));
    matRotation.setCoordSystem(OdGePoint3d::kOrigin, vrXAxis, vrZAxis.crossProduct(vrXAxis), vrZAxis);
    OdGeQuaternion quatRot;
    quatRot.set(matRotation);
    pTag->setRotation3d(quatRot);
  }
  else
  {
    pTag->setRotation2d(pDbAttrDef->rotation());
  }

  pTag->setString(pTagDef->getString());

  OdDgElementId idDgTextStyle = OdDgnExportContext::getElementId(pDbAttrDef->textStyle());

  if (!idDgTextStyle.isNull())
    pTag->applyTextStyle(idDgTextStyle);

  pTag->setHeightMultiplier(pDbAttrDef->height());
  pTag->setLengthMultiplier(pTag->getHeightMultiplier() * pDbAttrDef->widthFactor());
  pTag->setSlant(pDbAttrDef->oblique());

  OdDg::TextJustification uDgJust = getDgTextJustification(pDbAttrDef->horizontalMode(), pDbAttrDef->verticalMode());
  pTag->setJustification(uDgJust);

  if (pDbAttrDef->isMirroredInX())
    pTag->setBackwardsFlag(true);

  if (pDbAttrDef->isMirroredInY())
    pTag->setUpsideDownFlag(true);

  if( !pTag.isNull() )
  {
    addElementToDatabase(pTag, owner);
    copyElementProperties(pTag, pDbAttrDef, owner->database());
    appendOdDgElementToOwner(owner, pTag);

    OdDgnExportContext::addDgnElement(pDbAttrDef->objectId(), pTag->elementId());
  }
}

//---------------------------------------------------------------------------------------------------

OdDbAttributePtr convertConstDbAttrDefToAttribute(const OdDbAttributeDefinitionPtr& pAttrDef, const OdGeMatrix3d& matBlockTransform )
{
  OdDbAttributePtr pRet = OdDbAttribute::createObject();
  pRet->setDatabaseDefaults(pAttrDef->database());
  pRet->setAttributeFromBlock(pAttrDef, matBlockTransform);
  pRet->adjustAlignment();

  return pRet;
}

//---------------------------------------------------------------------------------------------------

void getBlockAttributes(OdDbBlockReference* pDbRef,
                          OdArray<OdDbAttributePtr>& arrAttributes,
                            OdArray<OdDbObjectId>& arrRasterImages,
                              OdArray<OdDbObjectId>& arrAttributeOwners,
                                OdArray<OdGeMatrix3d>& arrTransforms,
                                  OdArray<OdGeMatrix3d>& arrRasterTransforms,
                                    const OdGeMatrix3d& matBaseTransform)
{
  OdDbObjectId idDbBlock = pDbRef->blockTableRecord();

  if (idDbBlock.isNull())
    return;

  OdGeMatrix3d matRefTransform = pDbRef->blockTransform();

  OdDbObjectIteratorPtr pAttrIter = pDbRef->attributeIterator();

  OdArray<OdDbAttributePtr> arrCurBlockTags;

  for(; !pAttrIter->done(); pAttrIter->step())
  {
    OdDbAttributePtr pAttribute = pAttrIter->entity();

    if (pAttribute.isNull())
      continue;

    arrAttributes.push_back(pAttribute);
    arrCurBlockTags.push_back(pAttribute);
    arrAttributeOwners.push_back(idDbBlock);
    arrTransforms.push_back(matBaseTransform);
  }

  OdDbBlockTableRecordPtr pBlock = idDbBlock.openObject(OdDb::kForRead, true);

  OdDbObjectIteratorPtr pBlockIter = pBlock->newIterator();

  for (; !pBlockIter->done(); pBlockIter->step())
  {
    OdDbEntityPtr pItem = pBlockIter->entity();

    if( pItem.isNull() )
      continue;

    if( pItem->isKindOf(OdDbBlockReference::desc()) )
    {
      OdDbBlockReferencePtr pSubBlockRef = pItem;
      getBlockAttributes(pSubBlockRef.get(), arrAttributes, arrRasterImages, arrAttributeOwners, arrTransforms, arrRasterTransforms, matRefTransform*matBaseTransform);
    }
    else if (pItem->isKindOf(OdDbRasterImage::desc()))
    {
      arrRasterImages.push_back(pItem->id());
      arrRasterTransforms.push_back(matRefTransform*matBaseTransform);
    }
    else if( pItem->isKindOf(OdDbAttributeDefinition::desc()) )
    {
      OdDbAttributeDefinitionPtr pAttrDef = pItem;

      if( pAttrDef->isConstant() )
      {
        bool bAddAttribute = true;

        OdString strAttrDefName = pAttrDef->tag();
        strAttrDefName.makeUpper();

        for( OdUInt32 k = 0; k < arrCurBlockTags.size(); k++ )
        {
          OdString strTagName = arrCurBlockTags[k]->tag();
          strTagName.makeUpper();

          if( strAttrDefName == strTagName )
          {
            bAddAttribute = false;
            break;
          }
        }

        if( bAddAttribute )
        {
          OdDbAttributePtr pAttribute = convertConstDbAttrDefToAttribute(pAttrDef, matRefTransform);

          if( !pAttribute.isNull() )
          {
            arrAttributes.push_back(pAttribute);
            arrAttributeOwners.push_back(idDbBlock);
            arrTransforms.push_back(matBaseTransform);
          }
        }
      }
    }
  }
}

//---------------------------------------------------------------------------------------------------

OdUInt32 getNextFileNumber(const OdDgModelPtr& pModel)
{
  OdUInt32 uRet = 0;

  OdDgElementIteratorPtr pIter = pModel->createControlElementsIterator();

  for(; !pIter->done(); pIter->step() )
  {
    OdDgElementPtr pItem = pIter->item().openObject(OdDg::kForRead);

    if( !pItem.isNull() && pItem->isKindOf(OdDgReferenceAttachmentHeader::desc()) )
    {
      OdDgReferenceAttachmentHeaderPtr pXRef = pItem;

      if (pXRef->getFileNumber() > uRet)
        uRet = pXRef->getFileNumber();
    }
  }

  return uRet + 1;
}

//---------------------------------------------------------------------------------------------------

void exportBlockReferenceThroughExplode(OdDbBlockTableRecord* pDbBlock, OdDbBlockReference* pDbRef, OdDgElement* owner)
{
  OdGeMatrix3d matTransform = pDbRef->blockTransform();
  OdGeMatrix3d matUniScale  = matTransform;

  if( !matTransform.isUniScaledOrtho() )
  {
    OdGePoint3d ptOrigin;
    OdGeVector3d vrX, vrY, vrZ;
    matTransform.getCoordSystem(ptOrigin, vrX, vrY, vrZ);
    double dScale = vrX.length();

    vrY = vrZ.crossProduct(vrX);
    vrX = vrY.crossProduct(vrZ);

    vrX.setLength(dScale);
    vrY.setLength(dScale);
    vrZ.setLength(dScale);

    matUniScale.setCoordSystem(ptOrigin, vrX, vrY, vrZ);
  }

  OdDbObjectIteratorPtr pIter = pDbBlock->newIterator();

  OdUInt32 uColorByBlock       = OdDgnExportContext::getColorByBlockIndex();
  OdUInt32 uLineStyleByBlock   = OdDgnExportContext::getLineStyleByBlockIndex();
  OdUInt32 uLineWeightByBlock  = OdDgnExportContext::getLineWeightByBlockIndex();

  OdCmColor         curColor   = pDbRef->color();
  OdDbObjectId      idLineType = pDbRef->linetypeId();
  OdDb::LineWeight  lineWeight = pDbRef->lineWeight();

  OdDgnExportContext::setColorByBlockIndex(getDgnColorIndex(curColor, owner->database()));
  OdDgnExportContext::setLineStyleByBlockIndex(getDgnLineStyleEntryId(idLineType));
  OdDgnExportContext::setLineWeightByBlockIndex(odDgnExportLineweightMapping(lineWeight));

  for(; !pIter->done(); pIter->step())
  {
    OdDbEntityPtr pEnt = pIter->entity();
    OdDbEntityPtr pEntClone = pEnt->clone();

    if( pEntClone->transformBy(matTransform) == eCannotScaleNonUniformly )
    {
      pEntClone->transformBy(matUniScale);
    }

    if (!pEntClone.isNull())
    {
      OdDgnExportPE* pExportPE = (OdDgnExportPE*)pEntClone->queryX(OdDgnExportPE::desc());

      if (pExportPE)
        pExportPE->exportElement(pEntClone, owner);
    }
  }

  OdDgnExportContext::setColorByBlockIndex(uColorByBlock);
  OdDgnExportContext::setLineStyleByBlockIndex(uLineStyleByBlock);
  OdDgnExportContext::setLineWeightByBlockIndex(uLineWeightByBlock);
}

//---------------------------------------------------------------------------------------------------

bool exportBlockReference(OdDbBlockReference* pDbRef, OdDgElement* owner)
{
  if (!pDbRef)
    return true;

  OdDbObjectId idDbBlock = pDbRef->blockTableRecord();

  if (idDbBlock.isNull())
    return true;

  OdDbBlockTableRecordPtr pDbBlock = idDbBlock.openObject(OdDb::kForRead);

  if (pDbBlock.isNull())
    return true;

  if (OdDgnExportContext::isBlockOnProcessing(pDbBlock->id()))
  {
    ODA_ASSERT_ONCE(!"Circular block references.");
    return true;
  }

  if (OdDgnExportContext::isBlockWithXRefInsert(idDbBlock))
  {
    exportBlockReferenceThroughExplode(pDbBlock, pDbRef, owner);
    return true;
  }

  if (isXRef(pDbBlock)) // X-Ref
  {
    OdDgElementId idModel = getOwnerModelId(owner);

    if( idModel.isNull() )
    {
      ODA_ASSERT_ONCE(!"Can't get dgn model.");
      return true;
    }

    OdDgModelPtr pModel = idModel.openObject(OdDg::kForWrite);

    OdDgReferenceAttachmentHeaderPtr pDgXRef = OdDgReferenceAttachmentHeader::createObject();
    OdString strFileName = pDbBlock->pathName();
    pDgXRef->setFullFileName(strFileName);
    strFileName.replace(L'\\', L'/');

    if (strFileName.reverseFind(L'/') != -1)
      strFileName = strFileName.right(strFileName.getLength() - strFileName.reverseFind(L'/') - 1);
    else
    {
      OdDbHostAppServices* pAppServices = pDbRef->database()->appServices();

      if (pAppServices)
      {
        OdString strFullFileName = pAppServices->findFile(strFileName, pDbRef->database(), OdDbBaseHostAppServices::kXRefDrawing);

        if( !strFullFileName.isEmpty() )
          pDgXRef->setFullFileName(strFullFileName);
      }
    }

    pDgXRef->setFileName(strFileName);
    OdGeMatrix3d matTransform = pDbRef->blockTransform();

    OdGeVector3d vrXAxis = matTransform.getCsXAxis();
    OdGeVector3d vrYAxis = matTransform.getCsYAxis();
    OdGeVector3d vrZAxis = matTransform.getCsZAxis();

    double dScale = vrXAxis.length();

    if( !vrZAxis.isParallelTo(OdGeVector3d::kZAxis))
      dScale = matTransform.scale();
    else
    {
      if (vrYAxis.length() > dScale)
        dScale = vrYAxis.length();
    }

    dScale /= pModel->getMeasuresConversion(OdDgModel::kWuWorldUnit, pModel->getWorkingUnit());

    vrXAxis.normalize();
    vrYAxis.normalize();
    vrZAxis.normalize();

    OdGeMatrix3d matRotation;
    matRotation.setCoordSystem(OdGePoint3d::kOrigin, vrXAxis, vrYAxis, vrZAxis);

    pDgXRef->setTransformation(matRotation);
    pDgXRef->setScale(dScale);
    pDgXRef->setMasterOrigin(pDbRef->position());

    pDgXRef->setDisplayFlag(true);
    pDgXRef->setNestDepth(99);
    pDgXRef->setFileNumber(getNextFileNumber(pModel));

    OdDgElementId idDgLayer = OdDgnExportContext::getElementId(pDbRef->layerId());

    if (!idDgLayer.isNull())
    {
      OdDgLevelTableRecordPtr pLevel = idDgLayer.openObject(OdDg::kForRead);
      pDgXRef->setLevelEntryId(pLevel->getEntryId());
    }
    else
      pDgXRef->setLevelEntryId(0);

    // set clipping
    
    OdDbObjectId idDictionary = pDbRef->extensionDictionary();

    if (!idDictionary.isNull() && owner->isKindOf(OdDgModel::desc()))
    {
      OdDbDictionaryPtr pXDic = OdDbDictionary::cast(OdDbObjectId(idDictionary).openObject(OdDb::kForRead));

      if (!pXDic.isNull())
      {
        OdDbDictionaryPtr pFilterDic = pXDic->getAt(OD_T("ACAD_FILTER"), OdDb::kForRead);

        if (!pFilterDic.isNull())
        {
          OdDbSpatialFilterPtr pSpFilter = pFilterDic->getAt(OD_T("SPATIAL"), OdDb::kForRead);

          if (!pSpFilter.isNull() && pSpFilter->isEnabled())
          {
            OdGiClipBoundary boundary;
            bool bEnabled;
            pSpFilter->getDefinition(boundary, bEnabled);

            if (bEnabled)
            {
              OdGeVector2d vrOffset = pDbRef->position().convert2d().asVector();

              OdUInt32 uNumPts = boundary.m_Points.size();

              if (!boundary.m_Points.first().isEqualTo(boundary.m_Points.last()))
                uNumPts++;

              pDgXRef->setClipPointsCount(uNumPts);
              
              for( OdUInt32 k = 0; k < boundary.m_Points.size(); k++ )
                pDgXRef->setClipPoint(k, boundary.m_Points[k] - vrOffset);

              if (!boundary.m_Points.first().isEqualTo(boundary.m_Points.last()))
                pDgXRef->setClipPoint(boundary.m_Points.size(), boundary.m_Points[0] - vrOffset);
            }
          }
        }
      }
    }

    pModel->addElement(pDgXRef);
  }
  else
  {
    OdDgElementId idDgBlock = OdDgnExportContext::getElementId(idDbBlock);

    if (idDgBlock.isNull())
    {
      OdDgSharedCellDefinitionTablePtr pDgBlockTable = owner->database()->getSharedCellDefinitionTable(OdDg::kForWrite);

      OdDgSharedCellDefinitionPtr pDgBlockDef = OdDgSharedCellDefinition::createObject();
      pDgBlockDef->setName(pDbBlock->getName());
      pDgBlockDef->setDescription(pDbBlock->comments());
      pDgBlockDef->setOrigin(pDbBlock->origin());
      pDgBlockDef->set3dFormatFlag(OdDgnExportContext::is3d());
      pDgBlockTable->add(pDgBlockDef);

      double dOldScaleFactor = OdDgnExportContext::getLineStyleScaleCorrectionFactor();
      OdDgnExportContext::setLineStyleScaleCorrectionFactor(1.0 / OdDgnExportContext::getMasterToUORsScale());

      OdDgnExportContext::addProcessingBlockId(pDbBlock->id());
      DgnExporter::copyBlock(pDbBlock, pDgBlockDef);
      OdDgnExportContext::removeProcessingBlockId(pDbBlock->id());

      OdDgnExportContext::setLineStyleScaleCorrectionFactor(dOldScaleFactor);

      OdDgnExportContext::addDgnElement(pDbBlock->objectId(), pDgBlockDef->elementId());

      pDgBlockDef->transformBy(OdGeMatrix3d::scaling(OdDgnExportContext::getMasterToUORsScale()));

      idDgBlock = pDgBlockDef->elementId();
    }

    if (!idDgBlock.isNull())
    {
      OdDgSharedCellReferencePtr pCellRef = OdDgSharedCellReference::createObject();

      OdDgElementId idOwnerModel = getOwnerModelId(owner);
      pCellRef->setOrigin(pDbRef->position());
      pCellRef->setTransformation(pDbRef->blockTransform()*OdGeMatrix3d::scaling(1.0 / OdDgnExportContext::getMasterToUORsScale()));
      pCellRef->setDefinitionName(pDbBlock->getName());

      addElementToDatabase(pCellRef, owner);
      copyElementProperties(pCellRef, pDbRef, owner->database());
      appendOdDgElementToOwner(owner, pCellRef);

      OdDgnExportContext::addDgnElement(pDbRef->objectId(), pCellRef->elementId());

      OdUInt32 uOldColorByBlock   = OdDgnExportContext::getColorByBlockIndex();
      OdUInt32 uOldLineWeightByBlock = OdDgnExportContext::getLineWeightByBlockIndex();

      OdDgnExportContext::setColorByBlockIndex(pCellRef->getColorIndex());
      OdDgnExportContext::setLineWeightByBlockIndex(pCellRef->getLineWeight());

      // copy attributes and raster images of the block

      if( !idOwnerModel.isNull() )
      {
        OdDgModelPtr pModel = idOwnerModel.openObject(OdDg::kForRead);
        bool b3dTag = pModel->getModelIs3dFlag();

        OdArray<OdDbAttributePtr> arrAttributes;
        OdArray<OdDbObjectId> arrRasterImages;
        OdArray<OdDbObjectId> arrAttributeOwners;
        OdArray<OdGeMatrix3d> arrRasterTransforms;
        OdArray<OdGeMatrix3d> arrTransforms;

        getBlockAttributes(pDbRef, arrAttributes, arrRasterImages, arrAttributeOwners, arrTransforms, arrRasterTransforms, OdGeMatrix3d::kIdentity);

        for (OdUInt32 k = 0; k < arrAttributes.size(); k++)
        {
          OdDgElementId idAttrSetDef = OdDgnExportContext::getAttrDefSetForBlock(arrAttributeOwners[k]);

          if (idAttrSetDef.isNull())
            continue;

          OdDbAttributePtr pAttribute = arrAttributes[k];

          if (pAttribute.isNull())
            continue;

          OdDgTagDefinitionPtr pTagDef;

          if (!idAttrSetDef.isNull())
          {
            OdDgTagDefinitionSetPtr pTagSet = idAttrSetDef.openObject(OdDg::kForRead);

            pTagDef = pTagSet->getByName(pAttribute->tag());

            if( pTagDef.isNull() )
            {
              OdString strAttrName = pAttribute->tag();
              strAttrName.makeUpper();

              for (OdUInt32 n = 0; n < pTagSet->getCount(); n++)
              {
                OdDgTagDefinitionPtr pDef = pTagSet->getByEntryId(n);
                
                if( !pDef.isNull())
                {
                  OdString strDefName = pDef->getName();
                  strDefName.makeUpper();

                  if (strAttrName == strDefName)
                  {
                    pTagDef = pDef;
                    break;
                  }
                }
              }
            }
          }

          if (pTagDef.isNull())
            continue;

          OdDgTagElementPtr pTag = OdDgTagElement::createObject();
          pTag->setDatabaseDefaults(owner->database());
          pTag->set3dFlag(b3dTag);
          pTag->setTagDefinitionId(idAttrSetDef, pTagDef->getEntryId());
          pTag->setAssociatedFlag(true);
          pTag->setOffsetUsedFlag(true);
          pTag->setJustificationUsedFlag(true);
          pTag->setHideForSharedCellFlag(true);
          pTag->setOrigin(pAttribute->position());

          if (b3dTag)
          {
            OdGeMatrix3d matRotation;
            OdGeVector3d vrZAxis = pAttribute->normal();
            OdGeVector3d vrXAxis = OdGeVector3d::kXAxis;
            double dRotation = pAttribute->rotation();

            if (fabs(dRotation) < 1.e16)
              vrXAxis.rotateBy(dRotation, OdGeVector3d::kZAxis);

            vrXAxis.transformBy(OdGeMatrix3d::planeToWorld(vrZAxis));
            matRotation.setCoordSystem(OdGePoint3d::kOrigin, vrXAxis, vrZAxis.crossProduct(vrXAxis), vrZAxis);
            OdGeQuaternion quatRot;
            quatRot.set(matRotation);
            pTag->setRotation3d(quatRot);
          }
          else
          {
            pTag->setRotation2d(pAttribute->rotation());
          }

          if (pAttribute->isMTextAttribute())
            pTag->setString(OdString::kEmpty);
          else
            pTag->setString(pAttribute->textString());

          OdDgElementId idDgTextStyle = OdDgnExportContext::getElementId(pAttribute->textStyle());

          if (!idDgTextStyle.isNull())
            pTag->applyTextStyle(idDgTextStyle);

          pTag->setHeightMultiplier(pAttribute->height());
          pTag->setLengthMultiplier(pTag->getHeightMultiplier() * pAttribute->widthFactor());
          pTag->setSlant(pAttribute->oblique());

          //OdDg::TextJustification uDgJust = getDgTextJustification(pAttribute->horizontalMode(), pAttribute->verticalMode());
          pTag->setJustification(OdDg::kLeftBottom);

          if (pAttribute->isMirroredInX())
            pTag->setBackwardsFlag(true);

          if (pAttribute->isMirroredInY())
            pTag->setUpsideDownFlag(true);

          if (!pTag.isNull())
          {
            addElementToDatabase(pTag, owner);
            copyElementProperties(pTag, pAttribute, owner->database());
            pTag->setInvisibleFlag(pAttribute->isInvisible());
            appendOdDgElementToOwner(owner, pTag);

            OdDgnExportContext::addDgnElement(pAttribute->objectId(), pTag->elementId());

            pTag->transformBy(arrTransforms[k]);

            if (pAttribute->isMTextAttribute())
            {
              OdDbMTextPtr pDbMText = pAttribute->getMTextAttribute();
              OdDgGraphicsElementPtr pDgMText = convertMText(pDbMText, owner->database());

              pDgMText->transformBy(arrTransforms[k]);

              if (!pDgMText.isNull())
              {
                if (pDgMText->isKindOf(OdDgText2d::desc()))
                {
                  OdDgText2dPtr pDgText = pDgMText;
                  pTag->setString(pDgText->getText());
                }
                else if (pDgMText->isKindOf(OdDgText3d::desc()))
                {
                  OdDgText3dPtr pDgText = pDgMText;
                  pTag->setString(pDgText->getText());
                }
                else
                {
                  addElementToDatabase(pDgMText, owner);
                  appendOdDgElementToOwner(owner, pDgMText);
                }
              }
            }

            pTag->setAssociationId(pCellRef->elementId(), true);
          }
        }

        // Add raster images from blocks

        for (OdUInt32 l = 0; l < arrRasterImages.size(); l++)
        {
          OdDbEntityPtr pDbImage = arrRasterImages[l].openObject(OdDb::kForRead);

          OdDgnExportPE* pImageExportPE = (OdDgnExportPE*)pDbImage->queryX(OdDgnExportPE::desc());

          if (pImageExportPE)
          {
            pImageExportPE->exportElement(pDbImage, owner);
            OdDgElementId idDgRaster = OdDgnExportContext::getElementId(pDbImage->objectId());

            if (!idDgRaster.isNull())
            {
              OdDgGraphicsElementPtr pRasterFrame = idDgRaster.openObject(OdDg::kForWrite);

              if (!pRasterFrame.isNull())
                pRasterFrame->transformBy(arrRasterTransforms[l]);
            }
          }
        }
      }

      OdDgnExportContext::setColorByBlockIndex(uOldColorByBlock);
      OdDgnExportContext::setLineWeightByBlockIndex(uOldLineWeightByBlock);

      // set clipping

      OdDbObjectId idDictionary = pDbRef->extensionDictionary();

      if (!idDictionary.isNull() && owner->isKindOf(OdDgModel::desc()))
      {
        OdDbDictionaryPtr pXDic = OdDbDictionary::cast(OdDbObjectId(idDictionary).openObject(OdDb::kForRead));

        if (!pXDic.isNull())
        {
          OdDbDictionaryPtr pFilterDic = pXDic->getAt(OD_T("ACAD_FILTER"), OdDb::kForRead);

          if (!pFilterDic.isNull())
          {
            OdDbSpatialFilterPtr pSpFilter = pFilterDic->getAt(OD_T("SPATIAL"), OdDb::kForRead);

            if (!pSpFilter.isNull() && pSpFilter->isEnabled())
            {
              OdGiClipBoundary boundary;
              bool bEnabled;
              pSpFilter->getDefinition(boundary, bEnabled);

              if (bEnabled)
                pCellRef->setClipBoundary(boundary);
            }
          }
        }
      }

      // Investigate annotation scale using
    }
  }

  return true;
}

//---------------------------------------------------------------------------------------------------

void OdDbBlockReferenceExportPE::subExportElement(OdDbEntity* pDbEntity, OdDgElement* owner)
{
  OdDbBlockReference* pDbRef = (OdDbBlockReference*)(pDbEntity);

  OdDbViewRepBlockReferencePtr pViewRepBlockRef = OdDbViewRepBlockReference::cast(pDbRef);

  if( !pViewRepBlockRef.isNull() && !pViewRepBlockRef->ownerViewportId().isNull() )
  {
    // To Do: support OdDbViewRepBlockReference for different viewports.
    return;
  }

  if (!exportBlockReference(pDbRef, owner))
    OdDgnExportPE::subExportElement(pDbEntity, owner);
}

//---------------------------------------------------------------------------------------------------

void setRenderMode(OdDgReferenceAttachmentViewFlags& viewFlags, OdDb::RenderMode uRenderMode)
{
  switch( uRenderMode )
  {
    case OdDb::k2DOptimized:
    {
      viewFlags.setDisplayMode(OdDgView::kWireFrame);
    } break;

    case OdDb::kWireframe:
    {
      viewFlags.setDisplayMode(OdDgView::kWireMesh);
    } break;

    case OdDb::kHiddenLine:
    {
      viewFlags.setDisplayMode(OdDgView::kHiddenLine);
    } break;

    case OdDb::kFlatShaded:
    {
      viewFlags.setDisplayMode(OdDgView::kFilledHiddenLine);
    } break;

    case OdDb::kGouraudShaded:
    {
      viewFlags.setDisplayMode(OdDgView::kPhongShade);
    } break;

    case OdDb::kFlatShadedWithWireframe:
    {
      viewFlags.setDisplayMode(OdDgView::kFilledHiddenLine);
      viewFlags.setDisplayEdgesFlag(true);
    } break;

    case OdDb::kGouraudShadedWithWireframe:
    {
      viewFlags.setDisplayMode(OdDgView::kPhongShade);
      viewFlags.setDisplayEdgesFlag(true);
    } break;
  }
}

//---------------------------------------------------------------------------------------------------

void OdDbViewportExportPE::subExportElement(OdDbEntity* pDbEntity, OdDgElement* owner)
{
  OdDbViewport* pViewport = (OdDbViewport*)(pDbEntity);

  OdDgElementId idMainModel = OdDgnExportContext::getDgnModelSpaceModelId();

  if( idMainModel.isNull() )
    return;

  OdDgElementId idModel = getOwnerModelId(owner);

  if (idModel.isNull())
  {
    ODA_ASSERT_ONCE(!"Can't get dgn model.");
    return;
  }

  OdDgModelPtr pModel     = idModel.openObject(OdDg::kForWrite);
  OdDgModelPtr pMainModel = idMainModel.openObject(OdDg::kForRead);
  OdAbstractViewPEPtr pDbAbstractView = OdAbstractViewPEPtr(pViewport);

  double dScale = pViewport->height() / pViewport->viewHeight();

  OdGePoint3d ptOrigin = pDbAbstractView->target(pViewport);
  OdGePoint3d ptCameraPos = ptOrigin + pViewport->viewDirection();

  ptOrigin    *= OdDgnExportContext::getMasterToUORsScale();
  ptCameraPos *= OdDgnExportContext::getMasterToUORsScale();
  double dFocalLength = pViewport->viewDirection().length() * OdDgnExportContext::getMasterToUORsScale();
  double dZFront = pViewport->frontClipDistance() * OdDgnExportContext::getMasterToUORsScale();
  double dZBack  = pViewport->backClipDistance() * OdDgnExportContext::getMasterToUORsScale();

  OdGeMatrix3d matTransform = pDbAbstractView->worldToEye(pViewport);
  matTransform.setTranslation(OdGeVector3d::kIdentity);

  OdGeVector3d vrXAxis = matTransform.getCsXAxis();
  OdGeVector3d vrYAxis = matTransform.getCsYAxis();
  OdGeVector3d vrZAxis = matTransform.getCsZAxis();

  vrXAxis.normalize();
  vrYAxis.normalize();
  vrZAxis.normalize();

  OdGeMatrix3d matRotation;
  matRotation.setCoordSystem(OdGePoint3d::kOrigin, vrXAxis, vrYAxis, vrZAxis);

  OdDgReferenceAttachmentHeaderPtr pDgXRef = OdDgReferenceAttachmentHeader::createObject();
  pDgXRef->setFileName(OdString::kEmpty);
  pDgXRef->setModelName(pMainModel->getName());
  pDgXRef->setMasterOrigin(pViewport->centerPoint());
  pDgXRef->setReferenceOrigin(ptOrigin);
  pDgXRef->setTransformation(matRotation);
  pDgXRef->setScale(dScale);

  OdInt16 uNumber = pViewport->number();

  if(uNumber < 0 )
  {
    pDgXRef->setDisplayFlag(true);
    pDgXRef->setFileNumber(1);
    uNumber = 1;
  }
  else
  {
    pDgXRef->setDisplayFlag(pViewport->isOn() && !pViewport->visibility());
    pDgXRef->setFileNumber(uNumber);
  }
  pDgXRef->setNestDepth(99);
  pDgXRef->setCameraPosition(ptCameraPos);
  pDgXRef->setCameraFocalLength(dFocalLength);
  pDgXRef->setZFront(dZFront);
  pDgXRef->setZBack(dZBack);
  pDgXRef->setClipBackFlag(false);
  pDgXRef->setClipFrontFlag(false);
  pDgXRef->setRotateClippingFlag(true);
  pDgXRef->setLineStyleScaleFlag(true);
  
  OdDgElementId idDgLayer = OdDgnExportContext::getElementId(pViewport->layerId());

  if (!idDgLayer.isNull())
  {
    OdDgLevelTableRecordPtr pLevel = idDgLayer.openObject(OdDg::kForRead);
    pDgXRef->setLevelEntryId(pLevel->getEntryId());
  }
  else
    pDgXRef->setLevelEntryId(0);

  OdString strName;
  strName.format(L"Viewport %d", uNumber);

  pDgXRef->setLogicalName(strName);
  pDgXRef->setViewportFlag(true);

  OdGePoint2dArray arrClipPts;

  OdGePoint2d ptCenter = OdGePoint2d::kOrigin;
  double dHeight = pViewport->height();
  double dWidth  = pViewport->width();

  pDgXRef->setClipPointsCount(5);
  pDgXRef->setClipPoint(0, ptCenter - OdGeVector2d::kXAxis * dWidth / 2.0 - OdGeVector2d::kYAxis * dHeight / 2.0);
  pDgXRef->setClipPoint(1, ptCenter + OdGeVector2d::kXAxis * dWidth / 2.0 - OdGeVector2d::kYAxis * dHeight / 2.0);
  pDgXRef->setClipPoint(2, ptCenter + OdGeVector2d::kXAxis * dWidth / 2.0 + OdGeVector2d::kYAxis * dHeight / 2.0);
  pDgXRef->setClipPoint(3, ptCenter - OdGeVector2d::kXAxis * dWidth / 2.0 + OdGeVector2d::kYAxis * dHeight / 2.0);
  pDgXRef->setClipPoint(4, ptCenter - OdGeVector2d::kXAxis * dWidth / 2.0 - OdGeVector2d::kYAxis * dHeight / 2.0);

  pDgXRef->setClipRotationMatrix(OdGeMatrix3d::kIdentity);

  OdDgReferenceAttachmentViewFlags viewFlags = pDgXRef->getViewFlags(0);
  setRenderMode(viewFlags, pViewport->renderMode());
  pDgXRef->setViewFlags(0, viewFlags);

  pModel->addElement(pDgXRef);

  if( !pViewport->objectId().isNull() )
    OdDgnExportContext::addDgnElement(pViewport->objectId(), pDgXRef->elementId());

  //Create level mask

  OdDbLayerTablePtr pLayerTable = pDbEntity->database()->getLayerTableId().openObject(OdDb::kForRead);
  OdDbSymbolTableIteratorPtr pIter = pLayerTable->newIterator();

  OdDgLevelMaskPtr pLevelMask = OdDgLevelMask::createObject();
  pLevelMask->setViewIndex(0);
  pLevelMask->setReferenceAttachId(pDgXRef->elementId());
  pDgXRef->add(pLevelMask);

  for(; !pIter->done(); pIter->step())
  {
    OdDbObjectId idLayer = pIter->getRecordId();
    bool bLayerIsFrozen = pViewport->isLayerFrozenInViewport(idLayer);
    OdDgElementId idLevel = OdDgnExportContext::getElementId(idLayer);

    if( idLevel.isNull() )
      continue;

    OdDgLevelTableRecordPtr pLevel = idLevel.openObject(OdDg::kForRead);

    pLevelMask->setLevelIsVisible(pLevel->getEntryId(), pLevel->getName(), !bLayerIsFrozen);
  }
}

//---------------------------------------------------------------------------------------------------

}
