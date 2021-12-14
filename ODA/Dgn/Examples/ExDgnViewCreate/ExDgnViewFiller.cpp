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
#include "DgDatabase.h"
#include "DgHostAppServices.h"

#include "DgModel.h"

#include "DgEllipse.h"
#include "DgLineString.h"
#include "DgLine.h"

#include "DgTable.h"
#include "DgView.h"
#include "DgLevelMask.h"

#include "ExDgnViewFiller.h"

#define STL_USING_IOSTREAM
#include "OdaSTL.h"
#define  STD(a)  std:: a
#ifndef _tcout
#ifdef TD_UNICODE
#define _tcout wcout
#else
#define _tcout cout
#endif
#endif

OdExDgnViewFiller::OdExDgnViewFiller()
{
}

void OdExDgnViewFiller::fillDatabase(OdDgDatabase* pDb)
{

  /********************************************************************/
  /* Fill the model                                                   */
  /********************************************************************/
  m_pModel1 = pDb->getActiveModelId().safeOpenObject(OdDg::kForWrite);

  //initialize measure units
  OdDgModel::UnitDescription description;
  OdDgModel::fillUnitDescriptor( OdDgModel::kMeters, description );
  m_pModel1->setMasterUnit( description );
  OdDgModel::fillUnitDescriptor( OdDgModel::kMillimeters, description );
  m_pModel1->setSubUnit( description );
  m_pModel1->setWorkingUnit( OdDgModel::kWuMasterUnit );
  m_pModel1->setName("Meters model");

  OdDgModel::StorageUnitDescription storageUnits;

  m_pModel1->getStorageUnit( storageUnits );
  storageUnits.m_uorPerStorageUnit *= 0.9;
  m_pModel1->setStorageUnit( storageUnits );

  // create additional Model
  OdDgModelTablePtr pModelTable = pDb->getModelTable();
  m_pModel2 = OdDgModel::createObject();
  m_pModel2->setModelIs3dFlag(true);
  m_pModel2->setName("Feet model");
  OdDgModel::fillUnitDescriptor( OdDgModel::kFeet, description );
  m_pModel2->setMasterUnit( description );
  m_pModel2->setWorkingUnit( OdDgModel::kWuMasterUnit );
  pModelTable->add( m_pModel2 );

  m_pModel2->getStorageUnit( storageUnits );
  storageUnits.m_uorPerStorageUnit *= 2.1;
  m_pModel2->setStorageUnit( storageUnits );

  fillModels();
}

void OdExDgnViewFiller::fillModels()
{
  addDefaultGeometry(m_pModel1);
  addDefaultGeometry(m_pModel2);

  m_pModel1->fitToView();
  m_pModel2->fitToView();

  addViewGroup( m_pModel1, OD_T("View group 1") );
  addViewGroup( m_pModel2, OD_T("View group 2") );
  addViewGroup( m_pModel1, OD_T("Exchange1") );
  addViewGroup( m_pModel2, OD_T("Exchange2") );
  exchangeViewGroups( m_pModel1, m_pModel2, OD_T("Exchange1"), OD_T("Exchange2") );

  addNamedView( m_pModel1, OD_T("NamedView1") );
  addNamedView( m_pModel2, OD_T("Named view 2") );
  addNamedView( m_pModel1, OD_T("Named Exchange1") );
  addNamedView( m_pModel2, OD_T("Named Exchange2") );
  exchangeNamedViews( m_pModel1, m_pModel2, OD_T("Named Exchange1"), OD_T("Named Exchange2") );
}

void OdExDgnViewFiller::addDefaultGeometry( OdDgModelPtr pModel )
{
  OdDgLineString3dPtr pConturLine = OdDgLineString3d::createObject();

  pConturLine->addVertex( OdGePoint3d( -20.0,-15.0, 0 ) );
  pConturLine->addVertex( OdGePoint3d( 20.0,-15.0, 0 ) );
  pConturLine->addVertex( OdGePoint3d( 20.0,15.0, 0 ) );
  pConturLine->addVertex( OdGePoint3d( -20.0,15.0, 0 ) );
  pConturLine->addVertex( OdGePoint3d( -20.0,-15.0, 0 ) );

  pConturLine->setColorIndex(2);

  pModel->addElement( pConturLine );

  OdDgLineString3dPtr pInternalLine = OdDgLineString3d::createObject();

  pInternalLine->addVertex( OdGePoint3d( -18.0,-13.0, 0 ) );
  pInternalLine->addVertex( OdGePoint3d( 18.0,-13.0, 0 ) );
  pInternalLine->addVertex( OdGePoint3d( 18.0,13.0, 0 ) );
  pInternalLine->addVertex( OdGePoint3d( -18.0,13.0, 0 ) );
  pInternalLine->addVertex( OdGePoint3d( -18.0,-13.0, 0 ) );

  pInternalLine->setColorIndex(3);
  pInternalLine->setLineStyleEntryId(3);

  pModel->addElement( pInternalLine );

  OdDgEllipse3dPtr pEllipse = OdDgEllipse3d::createObject();

  pEllipse->setOrigin( OdGePoint3d(0,0,0) );
  pEllipse->setPrimaryAxis( 5 );
  pEllipse->setSecondaryAxis( 5 );
  pEllipse->setColorIndex( 3 );

  pModel->addElement(pEllipse);

  OdDgLine3dPtr pVerLine = OdDgLine3d::createObject();

  pVerLine->setStartPoint( OdGePoint3d(0,-2,0) );
  pVerLine->setEndPoint( OdGePoint3d(0,2,0) );

  pVerLine->setColorIndex(4);
  pVerLine->setLineStyleEntryId(5);

  pModel->addElement( pVerLine );

  OdDgLine3dPtr pHorLine = OdDgLine3d::createObject();

  pHorLine->setStartPoint( OdGePoint3d(-2,0,0) );
  pHorLine->setEndPoint( OdGePoint3d(2,0,0) );

  pHorLine->setColorIndex(4);
  pHorLine->setLineStyleEntryId(5);

  pModel->addElement( pHorLine );
}

void OdExDgnViewFiller::addViewGroup( OdDgModelPtr pModel, OdString strGroupName )
{
  OdString strView1Name;
  OdString strView2Name;
  strView1Name.format(OD_T("View 1 for %s"), strGroupName.c_str() );
  strView1Name.format(OD_T("View 2 for %s"), strGroupName.c_str() );

  OdDgViewGroupPtr pViewGroup = OdDgViewGroup::createObject();

  pViewGroup->setName(strGroupName);

  OdDgDatabase* pDb = pModel->database();

  OdDgViewGroupTablePtr pViewGroupTable = pDb->getViewGroupTable(OdDg::kForWrite);

  pViewGroupTable->add( pViewGroup );

  OdDgElementIteratorPtr pIter = pViewGroupTable->createIterator();

  OdDgViewGroupPtr pDefaultViewGroup = pIter->item().openObject(OdDg::kForRead);

  OdDgElementIteratorPtr pViewIter = pDefaultViewGroup->createIterator();

  OdDgViewPtr pDefaultView1 = pViewIter->item().openObject(OdDg::kForRead);

  OdGeExtents3d extBox;

  pDefaultView1->getOrthographyVisibleBox(extBox);

  OdDgViewPtr pView1 = OdDgView::createObject();
  pView1->setName(strView1Name);
  pView1->setViewRectangle( pDefaultView1->getViewRectangle() );
  pView1->setOrthographyVisibleBox( extBox );
  pView1->setUseCameraFlag(true);

  OdDgViewPtr pView2 = OdDgView::createObject();
  pView2->setName(strView2Name);
  pView2->setViewRectangle( pDefaultView1->getViewRectangle() );
  pView2->setOrthographyVisibleBox( extBox );
  pView2->setUseCameraFlag(false);

  pViewGroup->add(pView1);
  pViewGroup->add(pView2);

  pViewGroup->setModelId( pModel->id() );

  OdDgLevelMaskPtr  levelMask = OdDgLevelMask::createObject();

  levelMask->setLevelIsVisible( 1, true );

  pView1->add( levelMask );

  OdDgLevelMaskPtr  levelMask2 = OdDgLevelMask::createObject();

  levelMask2->setLevelIsVisible( 1, true );

  pView2->add( levelMask2 );
}

void OdExDgnViewFiller::exchangeViewGroups( OdDgModelPtr pModel1, OdDgModelPtr pModel2,  OdString strGroupName1,  OdString strGroupName2 )
{
  OdDgDatabase* pDb = pModel1->database();

  OdDgViewGroupTablePtr pViewGroupTable = pDb->getViewGroupTable(OdDg::kForWrite);

  OdDgViewGroupPtr pModel1VG = pViewGroupTable->getAt( strGroupName1 ).openObject(OdDg::kForWrite);
  OdDgViewGroupPtr pModel2VG = pViewGroupTable->getAt( strGroupName2 ).openObject(OdDg::kForWrite);

  pModel1VG->setModelId( pModel2->id() );
  pModel2VG->setModelId( pModel1->id() );
}

void OdExDgnViewFiller::addNamedView( OdDgModelPtr pModel, OdString strViewName )
{
  OdDgDatabase* pDb = pModel->database();

  OdDgViewGroupTablePtr pViewGroupTable = pDb->getViewGroupTable(OdDg::kForWrite);

  OdDgElementIteratorPtr pIter = pViewGroupTable->createIterator();

  OdDgViewGroupPtr pDefaultViewGroup = pIter->item().openObject(OdDg::kForRead);

  OdDgElementIteratorPtr pViewIter = pDefaultViewGroup->createIterator();

  OdDgViewPtr pDefaultView1 = pViewIter->item().openObject(OdDg::kForRead);

  OdGeExtents3d extBox;

  pDefaultView1->getOrthographyVisibleBox(extBox);

  OdDgNamedViewTablePtr pNamedViewTable = pDb->getNamedViewTable( OdDg::kForWrite );

  OdDgViewPtr pView = OdDgView::createObject();
  pView->setName(strViewName);
  pView->setViewRectangle( pDefaultView1->getViewRectangle() );
  pView->setOrthographyVisibleBox( extBox );
  pView->setUseCameraFlag(false);
  pView->setIndex((OdUInt16)(-1));
  pView->setNamedFlag(true);

  pNamedViewTable->add( pView );

  pView->setModelId( pModel->id() );

  OdDgLevelMaskPtr  levelMask = OdDgLevelMask::createObject();

  levelMask->setLevelIsVisible( 1, true );

  pView->add( levelMask );
}

void OdExDgnViewFiller::exchangeNamedViews( OdDgModelPtr pModel1, OdDgModelPtr pModel2,  OdString strViewName1,  OdString strViewName2 )
{
  OdDgDatabase* pDb = pModel1->database();
  OdDgNamedViewTablePtr pNamedViewTable = pDb->getNamedViewTable( OdDg::kForWrite );

  OdDgViewPtr pView1 = pNamedViewTable->getAt( strViewName1 ).openObject(OdDg::kForWrite);
  OdDgViewPtr pView2 = pNamedViewTable->getAt( strViewName2 ).openObject(OdDg::kForWrite);

  pView1->setModelId( pModel2->id() );
  pView2->setModelId( pModel1->id() );
}
