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

// ExLine.cpp: implementation of the ExLine function.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"

#include "DbCurve.h"
#include "DbAssocManager.h"
#include "DbAssocAction.h"
#include "DbAssocArrayActionBody.h"
#include "DbAssocArrayModifyActionBody.h"
#include "DbAssocArrayRectangularParameters.h"
#include "DbAssocArrayPathParameters.h"
#include "DbAssocArrayPolarParameters.h"
#include "DbAssocGeomDependency.h"
#include "Ed/EdCommandStack.h"
#include "../Editor/OSnapManager.h"

#include "OdDbAssocCurvePersSubentIdPE.h"
#include "DbCurve.h"
#include "DbCircle.h"
#include "DbLine.h"
#include "DbArc.h"


void _ArrayEdit_Replace(OdDbUserIO* pIO, OdDbObjectId &arrayId)
{
  OdDbSelectionSetPtr pSSet = pIO->select( L"Select objects:",
      OdEd::kSelAllowObjects );
  OdGePoint3d basePoint;

  int nVar = -1;
  try
  {
    basePoint = pIO->getPoint( L"Specify base point or [Key point]",OdEd::kGptDefault, 0, OD_T("Key point") );
  }
  catch ( const OdEdKeyword &kwd )
  {
    nVar = kwd.keywordIndex();
  }

  OdDbItemLocator firstIdx( 0, 0, 0 );
  OdArray<OdDbItemLocator> indicies;
  indicies.append( firstIdx );
  OdDbObjectId bodyId;


  OdDbVertexRefPtr pBasePointRef;

  switch (nVar)
  {
  case -1:
    pBasePointRef = OdDbVertexRef::createObject(basePoint);
    break;

  case 0:
    {
      OdEdPointTrackerWithSnapInfo centroidFilter( pSSet->objectIdArray() );
      OdEdPointTrackerWithSnapInfo::SnapContext &snapContext = centroidFilter.m_SnapContext;

      do
      {

        pIO->getPoint( L"Select centroid:",
                       OdEd::kGptNoUCS |
                       OdEd::kGptNoLimCheck |
                       OdEd::kInpThrowEmpty |
                       OdEd::kInpThrowOther
                       , 0, OdString::kEmpty, &centroidFilter );



      } while ( !snapContext.bValid );

      OdDbSubentId subId;
      OdDbCurvePtr pCurve = snapContext.entId.openObject();
      OdGePoint3d point;

      if ( pCurve->isA()->isEqualTo( OdDbLine::desc() ) 
        || pCurve->isA()->isEqualTo( OdDbArc::desc() ))
      {
        subId.setType( OdDb::kVertexSubentType );
        switch ( snapContext.mode )
        {
          case OdDb::kOsModeEnd:
            pCurve->getStartPoint( point );
            if ( snapContext.point.isEqualTo( point ) )
              subId.setIndex( OdDbAssocCurvePersSubentIdPE::kStart );
            else
              subId.setIndex( OdDbAssocCurvePersSubentIdPE::kEnd );
            break;
          case OdDb::kOsModeMid:
            subId.setIndex( OdDbAssocCurvePersSubentIdPE::kMiddle );
            break;
        }
      }

      if ( pCurve->isA()->isEqualTo( OdDbCircle::desc() ) )
      {
        subId.setType( OdDb::kVertexSubentType );
        switch ( snapContext.mode )
        {
          case OdDb::kOsModeCen:
            subId.setIndex( OdDbAssocCurvePersSubentIdPE::kCenter );
            break;
        }
      }

      pBasePointRef = OdDbVertexRef::createObject( snapContext.entId, subId, snapContext.point );  
    }
    break;
  }

  OdDbAssocArrayModifyActionBody::createInstance( arrayId,
                                                    indicies,
                                                    pSSet->objectIdArray(),
                                                    *pBasePointRef.get(),
                                                    bodyId );
}

void _ArrayEdit_Path( OdDbUserIO* pIO, OdDbAssocArrayActionBodyPtr &pBody, OdDbObjectId &arrayId )
{
  OdDbAssocArrayPathParametersPtr pPathParam = 
    OdDbAssocArrayPathParameters::cast( pBody->parameters() );

  switch ( pIO->getKeyword( 
      OD_T("Enter an option[Items/Rows/Levels/Method/Align/Zdirection/RESet/REPlace]"), 
      OD_T("Items Rows Levels Method Align Zdir RESet REPlace") ) )
  {
  case 0:
    {
      switch ( pPathParam->method() )
      {
      case OdDbAssocArrayPathParameters::kDivide:
        {
          int itemCount = pIO->getInt( OD_T("Enter number of items or [Expression]"),
                                       OdEd::kInpDefault,
                                       pPathParam->itemCount() );
          pPathParam->setItemCount( itemCount );
        }
        break;

      case OdDbAssocArrayPathParameters::kMeasure:
        {                     
          double itemSpacing = pIO->getReal( OD_T("Specify the distance between items or [Expression]"),
                                             OdEd::kInpDefault,
                                             pPathParam->itemSpacing() );
          int maxItemCount = pPathParam->path().curve()->length() / itemSpacing;
          OdString confirmTXT = OdString().format(OD_T("Maximum items %d. Specify number of items or [Fillentirepath Expression]"), maxItemCount);           
          int itemCount;
          do
          {
            itemCount = pIO->getInt( confirmTXT, OdEd::kInpDefault, maxItemCount );
          }
          while ( itemCount > maxItemCount );

          pPathParam->setItemSpacing( itemSpacing );
          pPathParam->setItemCount( itemCount );
        }
        break;
      }           
    }
    break;

  case 1:
    {
      int rowCount = pIO->getInt( OD_T("Enter number of rows or [Expression]"),
                                  OdEd::kInpDefault,
                                  pPathParam->rowCount() );
      pPathParam->setRowCount( rowCount );

      double rowSpacing = pIO->getReal( OD_T("Specify the distance between rows or [Total Expression]"),
                                        OdEd::kInpDefault,
                                        pPathParam->rowSpacing() );
      pPathParam->setRowSpacing( rowSpacing );

      double rowElevation = pIO->getReal( OD_T("Specify the incrementing elevation between rows or [Expression]"),
                                          OdEd::kInpDefault,
                                          pPathParam->rowElevation());
      pPathParam->setRowElevation( rowElevation );
    }
    break;

  case 2:
    {
      int lvlCount = pIO->getInt( OD_T("Enter number of levels or [Expression]"),
                                  OdEd::kInpDefault,
                                  pPathParam->levelCount() );
      pPathParam->setLevelCount( lvlCount );

      double lvlSpacing = pIO->getReal( OD_T("Specify the distance between levels or [Total Expression]"),  
                                        OdEd::kInpDefault,
                                        pPathParam->levelSpacing() );
      pPathParam->setLevelSpacing( lvlSpacing );
    }
    break;

  case 3:
    {
      switch ( pIO->getKeyword( OD_T("Method[Divide/Measure]"), OD_T("Divide Measure"), pPathParam->method() ) )
      {
      case 0:
        pPathParam->setMethod( OdDbAssocArrayPathParameters::kDivide );
        break;

      case 1:
        pPathParam->setMethod( OdDbAssocArrayPathParameters::kMeasure );
        break;
      }
    }
    break;

  case 4:
    {
    bool alignItems = !!pIO->getKeyword( OD_T("Align arrayed items[Yes No]"), OD_T("No Yes"), pPathParam->alignItems() );
    pPathParam->setAlignItems( alignItems );
    }
    break;

  case 5:
  {
    OdDbEvalVariantPtr value = OdDbEvalVariant::init(  double(1) );
    OdValue::UnitType unitType;
    OdString MaintainZvalueParamName = OD_T( "MaintainZ" );
    OdString errorMSG, kEmpty;
    pPathParam->getValueParam( MaintainZvalueParamName, *value.get(), kEmpty, kEmpty, unitType );          
    bool Zdirection = !!pIO->getKeyword( OD_T( "Keep Z direction[Yes No]" ), OD_T( "No Yes" ), ! OdZero( value->getDouble() ) );
    value->setDouble( Zdirection ? 1.0 : 0.0 );
    pPathParam->setValueParam( MaintainZvalueParamName, *value.get(), kEmpty, kEmpty, errorMSG, &unitType );  
  }
    break;

  case 6:
    {
      OdArray <OdDbItemLocator> itemIndexes;
      OdDbAssocArrayActionBody::resetArrayItems( arrayId, itemIndexes, true );
    }       
    break;

  case 7:
    _ArrayEdit_Replace( pIO, arrayId );
    break;
  }
}

void _ArrayEdit_Polar( OdDbUserIO* pIO, OdDbAssocArrayActionBodyPtr &pBody, OdDbObjectId &arrayId )
{
  OdDbAssocArrayPolarParametersPtr pPolarParam = 
    OdDbAssocArrayPolarParameters::cast( pBody->parameters() );

  switch ( pIO->getKeyword( 
    OD_T("Enter an option[Items/Rows/Levels/Angle/Fillangle/ROTate/RESet/REPlace]"), 
    OD_T("Items Rows Levels Angle Fillangle ROTate RESet REPlace") ) )
  {
  case 0:
    {
      int itemCount = pIO->getInt( OD_T("Enter number of items or [Expression]"),
                                   OdEd::kInpDefault,
                                   pPolarParam->itemCount() );
      pPolarParam->setItemCount( itemCount );

      
      pPolarParam->setAngleBetweenItems( pPolarParam->fillAngle()/(itemCount) );

    }
    break;

  case 1:
    {
      int rowCount = pIO->getInt( OD_T("Enter number of rows or [Expression]"),
                                  OdEd::kInpDefault,
                                  pPolarParam->rowCount() );
      pPolarParam->setRowCount( rowCount );

      double rowSpacing = pIO->getReal( OD_T("Specify the distance between rows or [Total Expression]"),
                                        OdEd::kInpDefault,
                                        pPolarParam->rowSpacing() );
      pPolarParam->setRowSpacing( rowSpacing );

      double rowElevation = pIO->getReal( OD_T("Specify the incrementing elevation between rows or [Expression]"),
                                          OdEd::kInpDefault,
                                          pPolarParam->rowElevation() );
      pPolarParam->setRowElevation( rowElevation );
    }
    break;

  case 2:
    {
      int lvlCount = pIO->getInt( OD_T("Enter number of levels or [Expression]"), 
                                  OdEd::kInpDefault,
                                  pPolarParam->levelCount() );
      pPolarParam->setLevelCount( lvlCount );

      double lvlSpacing = pIO->getReal( OD_T("Specify the distance between levels or [Total Expression]"),
                                        OdEd::kInpDefault,
                                        pPolarParam->levelSpacing() );
      pPolarParam->setLevelSpacing( lvlSpacing );
    }
    break;

  case 3:
    {
      double AngleBetweenItem = pIO->getReal( OD_T("Specify angle between items or [Expression]"),
                                              OdEd::kInpDefault,
                                              pPolarParam->angleBetweenItems() );
      int itemcnt = pPolarParam->itemCount();
      double FillAngle = AngleBetweenItem * itemcnt;
      if ( FillAngle > 360.0 )
      {
        AngleBetweenItem = 360.0 / itemcnt; 
        FillAngle = 360.0;
      }
      pPolarParam->setFillAngle(FillAngle);
      pPolarParam->setAngleBetweenItems(AngleBetweenItem);
    }
    break;

  case 4:
    {
      double FillAngle = pIO->getReal( OD_T("Specify fill angle or [Expression]"),
                                       OdEd::kInpDefault,
                                       pPolarParam->fillAngle() );
      double AngleBetweenItem;
      if ( FillAngle > 360.0 )
      {        
        FillAngle = 360.0;
      }

      AngleBetweenItem = FillAngle / pPolarParam->itemCount(); 

      pPolarParam->setFillAngle(FillAngle);
      pPolarParam->setAngleBetweenItems(AngleBetweenItem);
    }
    break;

  case 5:
    {
      bool rotateItems = !!pIO->getKeyword( OD_T("Rotate arrayed items[Yes No]"), 
                                          OD_T("No Yes"), 
                                          pPolarParam->rotateItems() );
      pPolarParam->setRotateItems( rotateItems );
    }
    break;

  case 6:
    {
      OdArray <OdDbItemLocator> itemIndexes;
      OdDbAssocArrayActionBody::resetArrayItems( arrayId, itemIndexes, true );
    }       
    break;
  case 7:
    _ArrayEdit_Replace( pIO, arrayId );
    break;
  }
}

void _ArrayEdit_Rectangular( OdDbUserIO* pIO, OdDbAssocArrayActionBodyPtr &pBody, OdDbObjectId &arrayId )
{
  OdDbAssocArrayRectangularParametersPtr pRectParam = 
    OdDbAssocArrayRectangularParameters::cast( pBody->parameters() );

  switch ( pIO->getKeyword( OD_T("Enter an option[Columns/Rows/Levels/RESet/REPlace]"), OD_T("Columns Rows Levels RESet REPlace") ) )
  {
  case 0:
    {
      int colCount = pIO->getInt( OD_T("Enter number of columns or [Expression]"),
                                  OdEd::kInpDefault,
                                  pRectParam->columnCount() );
      pRectParam->setColumnCount( colCount );

      double colSpacing = 
        pIO->getReal( OD_T("Specify the distance between columns or [Total Expression]"), 
                      OdEd::kInpDefault,
                      pRectParam->columnSpacing() );
      pRectParam->setColumnSpacing( colSpacing );
    }
    break;

  case 1:
    {
      int rowCount = pIO->getInt( OD_T("Enter number of rows or [Expression]"),
                                  OdEd::kInpDefault,
                                  pRectParam->rowCount() );
      pRectParam->setRowCount( rowCount );

      double rowSpacing = pIO->getReal( OD_T("Specify the distance between rows or [Total Expression]"),
                                        OdEd::kInpDefault,
                                        pRectParam->rowSpacing() );
      pRectParam->setRowSpacing( rowSpacing );

      double rowElevation = pIO->getReal( OD_T("Specify the incrementing elevation between rows or [Expression]"),
                                          OdEd::kInpDefault,
                                          pRectParam->rowElevation() );
      pRectParam->setRowElevation( rowElevation );
    }
    break;

  case 2:
    {
      int lvlCount = pIO->getInt( OD_T("Enter number of levels or [Expression]"),
                                  OdEd::kInpDefault,
                                  pRectParam->levelCount() );
      pRectParam->setLevelCount( lvlCount );

      double lvlSpacing = pIO->getReal( OD_T("Specify the distance between levels or [Total Expression]"),      
                                        OdEd::kInpDefault,
                                        pRectParam->levelSpacing() );
      pRectParam->setLevelSpacing( lvlSpacing );
    }
    break;

  case 3:
    {
      OdArray <OdDbItemLocator> itemIndexes;
      OdDbAssocArrayActionBody::resetArrayItems( arrayId, itemIndexes, true );
    }       
    break;

  case 4:
    _ArrayEdit_Replace(pIO,arrayId);
    break;

  }
}

int _ArrayEdit_getArrayType( const OdDbObjectId arrayId, OdDbAssocArrayActionBodyPtr &pBody )
{  
  OdDbEntityPtr pArray = OdDbEntity::cast( arrayId.safeOpenObject( OdDb::kForRead ) );
  
  OdDbObjectId bodyId = OdDbAssocArrayActionBody::getControllingActionBody( pArray );  

  pBody = OdDbAssocArrayActionBody::cast( bodyId.safeOpenObject( OdDb::kForWrite )  );

  OdDbAssocArrayParametersPtr pParameters = pBody->parameters();

  if ( pParameters->isKindOf( OdDbAssocArrayRectangularParameters::desc() ) )
  {

    return 0;
  }
  else if ( pParameters->isKindOf( OdDbAssocArrayPolarParameters::desc() ) )
  {

    return 1;
  }
  else if ( pParameters->isKindOf( OdDbAssocArrayPathParameters::desc() ) )
  {

    return 2;
  }

  return -1;
}

bool _ArrayEdit_selectArray( OdDbUserIO* pIO, OdDbSelectionSetPtr &pSSet )
{
  struct ArrayFilter : public OdStaticRxObject<OdEdSSetTracker>
  {  
    virtual int addDrawables(OdGsView*){return 0;}
    virtual void removeDrawables(OdGsView*){}

    virtual bool append(const OdDbObjectId& entId, const OdDbSelectionMethod* )
    {             
      OdDbEntityPtr pEnt = OdDbEntity::cast( entId.safeOpenObject( OdDb::kForRead ) );
      if ( pEnt.isNull() )
        return false;

      return OdDbAssocArrayActionBody::isAssociativeArray( pEnt );
    }
    virtual bool remove(const OdDbObjectId& , const OdDbSelectionMethod* )
    {
      return true;
    }
  } arrayFilter;

  do 
  {
    pSSet = pIO->select(L"Select array:", OdEd::kSelSinglePass, 0, OdString::kEmpty, &arrayFilter );
  } 
  while ( !pSSet->numEntities() );

  return true;
}


void _ArrayEdit_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();

  OdDbObjectId spaceId = ExCommandsUtils::activeBlockTableRecord(pDb);
  OdDbBlockTableRecordPtr pSpace = spaceId.safeOpenObject(OdDb::kForWrite);

  OdGeVector3d vNormal(ExCommandsUtils::ucsNormal(pDb));

  OdDbSelectionSetPtr pSArray;

  if ( false == _ArrayEdit_selectArray( pIO, pSArray ) )
    return;

  OdDbAssocArrayActionBodyPtr pBody;

  pDb->startTransaction();
  bool bOk = true;
  try
  {
    OdDbObjectId arrayId = pSArray->objectIdArray().first(); 
    switch ( _ArrayEdit_getArrayType( arrayId, pBody ) )
    {
    case 0:
      _ArrayEdit_Rectangular( pIO, pBody, arrayId );
      break;

    case 1:
      _ArrayEdit_Polar( pIO, pBody, arrayId );
      break;

    case 2:
      _ArrayEdit_Path( pIO, pBody, arrayId );
      break;

    default:
      ODA_FAIL_ONCE();
      bOk = false;
    }
  }
  catch (...)
  {
    bOk = false;
  }

 /* pBody->evaluateOverride();*/

  if (bOk)
  {
    {
      OdDbAssocActionPtr pAction = pBody->parentAction().openObject( OdDb::kForWrite );
      pAction->setStatus( kChangedDirectlyAssocStatus );
      pAction->downgradeOpen();
    }
     pDb->endTransaction();
    OdDbAssocManager::evaluateTopLevelNetwork( pDb );
    ::odedRegCmds()->executeCommand(OD_T("REGEN"), pCmdCtx);

    pIO->putString(OD_T("Ready"));
  }
  else
  {
    pDb->abortTransaction();
    pIO->putString(OD_T("Failed"));
  }
}
