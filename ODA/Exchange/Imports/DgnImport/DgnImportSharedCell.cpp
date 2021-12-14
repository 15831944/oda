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
#include "DgnImportSharedCell.h"
#include "DgnImportCommon.h"
#include "DgnImportContext.h"
#include "DbSymUtl.h"

#include <DbObjectId.h>
#include <DbBlockReference.h>
#include <DgSharedCellReference.h>
#include <DbBlockTable.h>
#include <DbSpatialFilter.h>

namespace TD_DGN_IMPORT {

//---------------------------------------------------------------------------------------------------

OdDbObjectId copyBlockZeroLayer(const OdString& strBlockName, const OdDgElementId& idDgBlockDef, OdDbDatabase* pDb)
{
  OdDbObjectId retVal;

  if (idDgBlockDef.isNull() || !pDb)
    return retVal;

  OdDbBlockTablePtr bt = pDb->getBlockTableId().safeOpenObject(OdDb::kForWrite);

  OdDgSharedCellDefinitionPtr pSharedCellDef = idDgBlockDef.openObject(OdDg::kForRead);

  OdDbBlockTableRecordPtr block = OdDbBlockTableRecord::createObject();

  block->setComments(pSharedCellDef->getDescription());
  block->setOrigin(pSharedCellDef->getOrigin());
  pDb->addOdDbObject(block, bt->objectId());

  if( !bt->has(strBlockName) )
    block->setName(strBlockName);
  else
    block->setName(L"*U");

  bt->add(block);

  for( OdDgElementIteratorPtr pIter = pSharedCellDef->createIterator(); !pIter->done(); pIter->step())
  {
    OdDgElementPtr pDgElement = pIter->item().openObject();

    if (!pDgElement.isNull())
    {
      ((OdDgnImportPE*)pDgElement->queryX(OdDgnImportPE::desc()))->importElement(pDgElement, block);
    }
  }

  retVal = block->id();

  return retVal;
}

//---------------------------------------------------------------------------------------------------

OdString getBlockOverrideKeyString(const OdString& strBlockName)
{
  OdString strRet = strBlockName;

  if( OdDgnImportContext::getLockColorMode() )
  {
    OdString strColor;
    strColor.format(L"_%d", OdDgnImportContext::getLockColor());
    strRet += strColor;
  }
  else
    strRet += L"_X";

  if (OdDgnImportContext::getLockLineStyleMode())
  {
    OdString strLS;
    strLS.format(L"_%d", OdDgnImportContext::getLockLineStyle());
    strRet += strLS;
  }
  else
    strRet += L"_X";

  if (OdDgnImportContext::getLockLineWeightMode())
  {
    OdString strLW;
    strLW.format(L"_%d", OdDgnImportContext::getLockLineWeight());
    strRet += strLW;
  }
  else
    strRet += L"_X";

  if( OdDgnImportContext::getLayerByBlockMode() )
    strRet += L"_0";
  else
    strRet += L"_X";

  return strRet;
}

//---------------------------------------------------------------------------------------------------

void OdDgSharedCellReferenceImportPE::subImportElement(OdDgElement* e, OdDbBlockTableRecord* owner)
{
  OdDgSharedCellReference* r = (OdDgSharedCellReference*)e;
  OdDbBlockTablePtr bt = owner->database()->getBlockTableId().safeOpenObject();
  OdDgSharedCellDefinitionPtr pDef = r->findDefinition();

  OdDgElementId idCellDef;

  if( !pDef.isNull() )
  {
    idCellDef = pDef->elementId();
  }

  OdDbObjectId blockId = OdDgnImportContext::getObjectId( idCellDef );

  if (blockId.isNull())
  {
    ODA_FAIL_ONCE();
    return;
  }

  bool bSkipTransform = false;

  bool bLockColor = OdDgnImportContext::getLockColorMode();
  bool bLockLineWeight = OdDgnImportContext::getLockLineWeightMode();
  bool bLockLineStyle = OdDgnImportContext::getLockLineStyleMode();
  bool bLayerByBlockMode = OdDgnImportContext::getLayerByBlockMode();

  if( r->getColorOverrideFlag() || bLockColor || 
      r->getLevelOverrideFlag() || bLayerByBlockMode ||
      r->getStyleOverrideFlag() || bLockLineStyle ||
      r->getWeightOverrideFlag() || bLockLineWeight )
  {
    OdUInt32 uLockColorIndex = r->getColorIndex();

    if( bLockColor )
      uLockColorIndex = OdDgnImportContext::getLockColor();

    OdUInt32 uLockWeight = r->getLineWeight();

    if (bLockLineWeight)
      uLockWeight = OdDgnImportContext::getLockLineWeight();

    OdUInt32 uLockStyle = r->getLineStyleEntryId();

    if (bLockLineStyle)
      uLockWeight = OdDgnImportContext::getLockLineStyle();

    if( !bLockColor && r->getColorOverrideFlag() )
    {
      OdDgnImportContext::setLockColorMode(true);
      OdDgnImportContext::setLockColor(uLockColorIndex);
    }

    if (!bLockLineWeight && r->getWeightOverrideFlag())
    {
      OdDgnImportContext::setLockLineWeightMode(true);
      OdDgnImportContext::setLockLineWeight(uLockWeight);
    }

    if (!bLockLineStyle && r->getStyleOverrideFlag())
    {
      OdDgnImportContext::setLockLineStyleMode(true);
      OdDgnImportContext::setLockLineStyle(uLockStyle);
    }

    if( !bLayerByBlockMode && r->getLevelOverrideFlag() )
    {
      OdDgnImportContext::setLayerByBlockMode(true);
    }

    OdString strBlockName = pDef->getName();

    if( strBlockName.isEmpty() )
      strBlockName = L"*U";

    OdString strKey = getBlockOverrideKeyString(strBlockName);

    blockId = OdDgnImportContext::getBlockForSharedCellOverride(strKey);

    if( blockId.isNull() )
    {
      blockId = copyBlockZeroLayer(strBlockName, pDef->elementId(), owner->database());

      if( !blockId.isNull() )
        OdDgnImportContext::addBlockForSharedCellOverride(strKey, blockId);
    }

    OdDgnImportContext::setLockColorMode(bLockColor);
    OdDgnImportContext::setLockLineWeightMode(bLockLineWeight);
    OdDgnImportContext::setLockLineStyleMode(bLockLineStyle);
    OdDgnImportContext::setLayerByBlockMode(bLayerByBlockMode);
  }

  OdDbBlockReferencePtr dbRef = OdDbBlockReference::createObject();
  dbRef->setDatabaseDefaults(owner->database());
  owner->database()->addOdDbObject(dbRef, owner->objectId());
  copyEntityProperties(r, dbRef);
  owner->appendOdDbEntity(dbRef);
  dbRef->setBlockTableRecord(blockId);

  if( bSkipTransform )
  {
    dbRef->setBlockTransform(OdGeMatrix3d::kIdentity);
  }
  else
  {
    OdGeMatrix3d matTransform = OdGeMatrix3d::translation(r->getOrigin().asVector()) * r->getTransformation();

    prepareMatrixToBlockTransform( matTransform );

    dbRef->setBlockTransform(matTransform);
  }

  OdDgCellBoundaryClipPEPtr pCellBoundaryClip = OdDgCellBoundaryClipPEPtr(OdRxObjectPtr(e));

  if( !pCellBoundaryClip.isNull() )
  {
    OdGiClipBoundary clipBoundary;

    if( pCellBoundaryClip->getBoundaryClip(e, clipBoundary) )
    {
      clipBoundary.m_xInverseBlockRefXForm = dbRef->blockTransform().invert();

      if( dbRef->extensionDictionary().isNull() )
      {
        dbRef->createExtensionDictionary();
      }

      OdDbDictionaryPtr pDict = OdDbDictionary::cast(dbRef->extensionDictionary().openObject(OdDb::kForWrite));

      if( !pDict.isNull() )
      {
        OdDbDictionaryPtr pFDict = OdDbDictionary::cast(pDict->getAt(OD_T("ACAD_FILTER"), OdDb::kForWrite));

        if( pFDict.isNull() )
        {
          pFDict = OdDbDictionary::createObject();
          pDict->setAt( OD_T("ACAD_FILTER"), pFDict );
        }

        if( !pFDict.isNull() )
        {
          OdDbSpatialFilterPtr pSp = OdDbSpatialFilter::cast(pFDict->getAt(OD_T("SPATIAL"), OdDb::kForWrite));

          if( pSp.isNull() )
          {
            pSp = OdDbSpatialFilter::createObject();
            pFDict->setAt( OD_T("SPATIAL"), pSp );
          }

          if( !pSp.isNull() )
          {
            double dFrontClip = ODDB_INFINITE_XCLIP_DEPTH;
            double dBackClip = ODDB_INFINITE_XCLIP_DEPTH;

            if( clipBoundary.m_bClippingFront )
            {
              dFrontClip = clipBoundary.m_dFrontClipZ;
            }

            if( clipBoundary.m_bClippingBack )
            {
              dBackClip = clipBoundary.m_dBackClipZ;
            }
            pSp->setDefinition( clipBoundary );
          }
        }
      }
    }
  }


  OdDgnImportPathToDwgObject dwgPath;
  dwgPath.m_idPath.objectIds().push_back( dbRef->objectId() );
  dwgPath.m_bExists = true;
  OdDgnImportContext::addObjectPath( e->elementId(), dwgPath );
}

//---------------------------------------------------------------------------------------------------

}
