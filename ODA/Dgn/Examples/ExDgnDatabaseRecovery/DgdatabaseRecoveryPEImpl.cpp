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

#include "StdAfx.h"
#include "DgDatabaseRecoveryPEImpl.h"
#include "DgDatabase.h"
#include "DgDimension.h"
#include "DgCellHeader.h"
#include "DgDetailingSymbol.h"

//----------------------------------------------------------
//
// OdDgDatabaseDimensionsRecoveryPE
//
//----------------------------------------------------------

#define DIMPOINT_MAX_COORD 1e14

void OdDgDatabaseDimensionsRecoveryPE::recoverDimension( OdDgElementPtr& pDimElement )
{
  OdDgDimensionPtr pDim = pDimElement;

  if( pDim.isNull() )
  {
    return;
  }

  bool bCorrectDimension = true;

  for( OdUInt32 i = 0; i < pDim->getPointsCount(); i++ )
  {
    OdDgDimPoint curPoint = pDim->getPoint( i );
    OdGePoint3d  ptCheck = curPoint.getPoint();

    if( pDim->get3dFormatFlag() && 
       ( fabs(ptCheck.x) > DIMPOINT_MAX_COORD || fabs(ptCheck.y) > DIMPOINT_MAX_COORD || fabs(ptCheck.z) > DIMPOINT_MAX_COORD) )
    {
      bCorrectDimension = false;
    }
    else if( !pDim->get3dFormatFlag() && ( fabs(ptCheck.x) > DIMPOINT_MAX_COORD || fabs(ptCheck.y) > DIMPOINT_MAX_COORD ) )
    {
      bCorrectDimension = false;
    }

    if( !bCorrectDimension )
    {
      break;
    }
  }

  if( !bCorrectDimension )
  {
    pDimElement->erase();
  }
}

//----------------------------------------------------------

void OdDgDatabaseDimensionsRecoveryPE::recoverDimensionsIntoCellHeader( OdDgElementPtr& pElement )
{
  OdDgElementIteratorPtr pIter;

  if( pElement->isKindOf( OdDgDetailingSymbolCellHeader2d::desc()) || 
      pElement->isKindOf( OdDgDetailingSymbolCellHeader3d::desc())
    )
  {
    return;
  }

  if( !pElement.isNull() && pElement->isKindOf(OdDgCellHeader3d::desc() ) )
  {
    OdDgCellHeader3dPtr pCell3d = pElement;
    pIter = pCell3d->createIterator();
  }
  else if( !pElement.isNull() && pElement->isKindOf(OdDgCellHeader2d::desc() ) )
  {
    OdDgCellHeader2dPtr pCell2d = pElement;
    pIter = pCell2d->createIterator();
  }

  if( pIter.isNull() )
  {
    return;
  }

  for(; !pIter->done(); pIter->step() )
  {
    OdDgElementPtr pChildElement = pIter->item().openObject(OdDg::kForWrite);

    if (pChildElement.isNull())
      continue;

    if( pChildElement->isKindOf(OdDgDimension::desc()) )
    {
      recoverDimension( pChildElement );
    }
    else if( pChildElement->getElementType() == OdDgElement::kTypeCellHeader )
    {
      recoverDimensionsIntoCellHeader( pChildElement );
    }
  }
}

//----------------------------------------------------------

OdResult OdDgDatabaseDimensionsRecoveryPE::recoveryDatabase( OdDgDatabase* pDb )
{
  if( !pDb )
  {
    return eInvalidInput;
  }

  // 1. Check all dimensions from model tables. (Try to investigate both graphics and control elements. ) 

  OdDgModelTablePtr pModelTable = pDb->getModelTable( OdDg::kForRead );

  if( !pModelTable.isNull() )
  {
    OdDgElementIteratorPtr pModelIter = pModelTable->createIterator();

    for(; !pModelIter->done(); pModelIter->step() )
    {
      OdDgModelPtr pModel = pModelIter->item().openObject(OdDg::kForWrite );

      if( !pModel.isNull() )
      {
        OdDgElementIteratorPtr pGrElmIter = pModel->createGraphicsElementsIterator();

        for(; !pGrElmIter->done(); pGrElmIter->step() )
        {
          OdDgElementPtr pElement = pGrElmIter->item().openObject(OdDg::kForWrite);

          if( pElement.isNull() )
            continue;

          if( pElement->isKindOf(OdDgDimension::desc()) )
          {
            recoverDimension( pElement );
          }
          else if( pElement->getElementType() == OdDgElement::kTypeCellHeader )
          {
            recoverDimensionsIntoCellHeader( pElement );
          }
        }

        OdDgElementIteratorPtr pCtrlElmIter = pModel->createControlElementsIterator();

        for(; !pCtrlElmIter->done(); pCtrlElmIter->step() )
        {
          OdDgElementPtr pElement = pCtrlElmIter->item().openObject(OdDg::kForWrite);

          if( pElement.isNull() )
            continue;

          if( pElement->isKindOf(OdDgDimension::desc()) )
          {
            recoverDimension( pElement );
          }
          else if( pElement->getElementType() == OdDgElement::kTypeCellHeader )
          {
            recoverDimensionsIntoCellHeader( pElement );
          }
        }
      }
    }
  }

  // 2. Check all dimensions from shared cell definition table.

  OdDgSharedCellDefinitionTablePtr pSharedCellTable = pDb->getSharedCellDefinitionTable(OdDg::kForRead);

  OdDgElementIteratorPtr pSharedCellIter = pSharedCellTable->createIterator();

  for(; !pSharedCellIter->done(); pSharedCellIter->step() )
  {
    OdDgElementPtr pSharedCellElm = pSharedCellIter->item().openObject(OdDg::kForRead);

    if( pSharedCellElm->isKindOf( OdDgSharedCellDefinition::desc() ) )
    {
      OdDgSharedCellDefinitionPtr pSharedCellDef = pSharedCellElm;
      OdDgElementIteratorPtr      pCellElmIter   = pSharedCellDef->createIterator();

      for(; !pCellElmIter->done(); pCellElmIter->step() )
      {
        OdDgElementPtr pElement = pCellElmIter->item().openObject(OdDg::kForWrite);

        if( pElement->isKindOf(OdDgDimension::desc()) )
        {
          recoverDimension( pElement );
        }
        else if( pElement->getElementType() == OdDgElement::kTypeCellHeader )
        {
          recoverDimensionsIntoCellHeader( pElement );
        }
      }
    }
  }

  return eOk;
}

//----------------------------------------------------------


