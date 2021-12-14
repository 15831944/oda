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
//
// OdqRevDiffHelper.cpp
// 

#include "OdaCommon.h"
#include "OdqRevDiffHelper.h"
#include "Gs/GsViewImpl.h"
#include "Gi/GiVisualStyle.h"
#include "GiDefaultContext.h"
#include "DbVisualStyle.h"
#include "DbViewportTable.h"
#include "DbSymbolTable.h"
#include "DbViewportTableRecord.h"
#include "DbDatabase.h"
#include "DbGsManager.h"
#include "DbEntity.h"
#include "DbDictionary.h"
#include "AbstractViewPE.h"
#include "DbBaseDatabase.h"

void modelLWeights(OdUInt8Array& lweights, double scale)
{
  lweights.clear();
  lweights.reserve(24);
  *lweights.append() = OdUInt8(  2. * scale + 0.5 );
  *lweights.append() = OdUInt8(  2. * scale + 0.5 );
  *lweights.append() = OdUInt8(  2. * scale + 0.5 );
  *lweights.append() = OdUInt8(  2. * scale + 0.5 );
  *lweights.append() = OdUInt8(  2. * scale + 0.5 );
  *lweights.append() = OdUInt8(  2. * scale + 0.5 );
  *lweights.append() = OdUInt8(  2. * scale + 0.5 );
  *lweights.append() = OdUInt8(  2. * scale + 0.5 );
  *lweights.append() = OdUInt8(  6. * scale + 0.5 );
  *lweights.append() = OdUInt8(  6. * scale + 0.5 );
  *lweights.append() = OdUInt8(  8. * scale + 0.5 );
  *lweights.append() = OdUInt8( 10. * scale + 0.5 );
  *lweights.append() = OdUInt8( 10. * scale + 0.5 );
  *lweights.append() = OdUInt8( 12. * scale + 0.5 );
  *lweights.append() = OdUInt8( 14. * scale + 0.5 );

  *lweights.append() = OdUInt8( 16. * scale + 0.5 );
  *lweights.append() = OdUInt8( 18. * scale + 0.5 );
  *lweights.append() = OdUInt8( 20. * scale + 0.5 );
  *lweights.append() = OdUInt8( 20. * scale + 0.5 );
  *lweights.append() = OdUInt8( 24. * scale + 0.5 );

  *lweights.append() = OdUInt8( 26. * scale + 0.5 );
  *lweights.append() = OdUInt8( 28. * scale + 0.5 );
  *lweights.append() = OdUInt8( 30. * scale + 0.5 );
  *lweights.append() = OdUInt8( 30. * scale + 0.5 );
}

OdqRevDiffHelper::OdqRevDiffHelper()
{
  clear();
}


OdqRevDiffHelper::~OdqRevDiffHelper()
{
  clear();
}


void OdqRevDiffHelper::setVS( OdDbObjectId v1, OdDbObjectId v2 )
{
  m_firstVS = v1;
  m_secondVS = v2;
}

OdDbObjectId OdqRevDiffHelper::getVS( bool bFirst )
{
  if( bFirst ) return m_firstVS;
  return m_secondVS;
}


void OdqRevDiffHelper::setDB( OdDbDatabasePtr db1, OdDbDatabasePtr db2 )
{
  m_firstDB = db1;
  m_secondDB = db2;
}


OdDbDatabasePtr OdqRevDiffHelper::getDatabase( bool first )
{
  if( first ) return m_firstDB;
  return m_secondDB;
}


void OdqRevDiffHelper::setDevice( OdGsDevice* d )
{
  m_device = d;
}


OdGsDevice* OdqRevDiffHelper::getDevice()
{
  return m_device;
}


void OdqRevDiffHelper::setModel( OdGsModel* m )
{
  m_model = m;
}


OdGsModel* OdqRevDiffHelper::getModel()
{
  return m_model;
}

void OdqRevDiffHelper::setViews( OdGsView* v1, OdGsView* v2 )
{
  m_mainView = v1;
  m_secondView = v2;
}


OdGsView* OdqRevDiffHelper::getView( bool first )
{
  if( first ) return m_mainView;
  return m_secondView;
}


 void OdqRevDiffHelper::setHandles( const OdDbHandleList& handles )
 {
   m_handles = handles;
 }


void OdqRevDiffHelper::clear()
{
  if( !m_firstDB.isNull() )
  {
    m_firstDB.release();
  }
  if( !m_secondDB.isNull() )
  {
    m_secondDB.release();
  }
  m_firstVS.setNull();
  m_secondVS.setNull();
  m_device = NULL;
  m_model = NULL;
  m_mainView = NULL;
  m_secondView = NULL;

  m_bShowAll = true;
  m_dOpac1 = 1.0;
  m_dOpac2 = 1.0;

  m_handles.clear();
  m_firstVSMap.clear();
  m_secondVSMap.clear();
}


void OdqRevDiffHelper::setupDefaultDifferenceView( bool bPlotGeneration )
{
  OdDbViewportTablePtr pVT = m_secondDB->getViewportTableId().safeOpenObject();
  OdDbSymbolTableIteratorPtr  pVpIter = pVT->newIterator();
  OdGsClientViewInfo viewInfo;
  OdGsViewPtr pView1;

  OdUInt8Array lweights;
  double dLineweightToDcScale = 0.;
  bool bLwDisplay = false;
  if( bPlotGeneration )
  {
    dLineweightToDcScale = 0.;
  }
  else
  {
    bLwDisplay = m_secondDB->getLWDISPLAY();
    //modelLWeights(lweights, pDb->appServices()->getLWDISPSCALE());
  }

  OdGsView* pView = m_device->viewAt( 0 );

  for ( ; !pVpIter->done(); pVpIter->step())
  {
    OdDbViewportTableRecordPtr pVp = pVpIter->getRecordId().openObject();
    pView->clientViewInfo( viewInfo );
    viewInfo.viewportObjectId = pVpIter->getRecordId();
    pView1 = m_device->createView( &viewInfo );
    m_device->addView( pView1 );
    OdAbstractViewPEPtr(pView1)->setView(pView1, pVp);

    if(bLwDisplay)
      pView1->setLineweightEnum(lweights.size(), lweights.getPtr());
    else
      pView1->setLineweightToDcScale(dLineweightToDcScale);
    break;
  }
  setViews( pView, pView1.get() );
}


void OdqRevDiffHelper::initViews( const OdDbHandleList& handles )
{
  setHandles( handles );
  m_bShowAll = true;
  loadSecondView();
}


void OdqRevDiffHelper::reloadMainView( bool bShowAll )
{
  if( bShowAll == m_bShowAll ) return;
  m_mainView->eraseAll();
  m_bShowAll = bShowAll;
  if( !bShowAll )
  {
    for( OdUInt32 i = 0; i < m_handles.size(); ++i )
    {
      OdDbObjectId id1 = m_firstDB->getOdDbObjectId( m_handles[i], false );
      if( id1.isValid() )
      {
        try
        {
          OdDbObjectPtr objPtr = id1.safeOpenObject( OdDb::kForWrite );
          if( !objPtr.isNull() )
          {
            OdDbEntityPtr ent = OdDbEntity::cast( objPtr );
            if( !ent.isNull() )
            {
              {
                m_mainView->add( ent, m_model );
              }
            }
          }
        }
        catch( ... )
        {
          //Nothing
        }
      }
    }
  }
  else //load model space
  {
    m_mainView->add( m_firstDB->getModelSpaceId().safeOpenObject(), m_model );
  }
}


void OdqRevDiffHelper::loadSecondView()
{
  for( OdUInt32 i = 0; i < m_handles.size(); ++i )
  {
    OdDbObjectId id2 = m_secondDB->getOdDbObjectId( m_handles[i], false );
    if( id2.isValid() )
    {
      try
      {
        OdDbObjectPtr objPtr = id2.safeOpenObject( OdDb::kForWrite );
        if( !objPtr.isNull() )
        {
          OdDbEntityPtr ent = OdDbEntity::cast( objPtr );
          if( !ent.isNull() )
          {
            {
              m_secondView->add( ent, m_model );
            }
          }
        }
      }
      catch( ... )
      {
        //Nothing
      }
    }
  }
}


OdDbStub* getVsIdByName(OdDbBaseDatabase* pRxDb, const OdString& sName, OdDbStub* idVsDef = NULL)
{
  OdDbBaseDatabasePE* pDbPE = OdDbBaseDatabasePE::cast(pRxDb).get();
  ODA_ASSERT_ONCE(pRxDb && pDbPE && !sName.isEmpty());

  for (OdRxIteratorPtr pVsItr = pDbPE->visualStyles(pRxDb); 
       pVsItr.get() && !pVsItr->done(); pVsItr->next())
  {
    OdRxObjectPtr pVsObject = pVsItr->object();
    OdDbBaseVisualStylePE* pVsPE = OdDbBaseVisualStylePE::cast(pVsObject).get();
    ODA_ASSERT_ONCE(pVsPE);
    if (!pVsPE || sName != pVsPE->name(pVsObject) || pVsPE->isInternal(pVsObject))
      continue;
    
    OdGiDrawable* pDrw = OdGiDrawable::cast(pVsObject).get();
    ODA_ASSERT_ONCE(pDrw && pDrw->id());

    return pDrw->id();
  }
  return idVsDef;
}


OdDbObjectId OdqRevDiffHelper::createVisualStyleBasedOn( const OdString& base, const OdString& name )
{
  OdDbDictionaryPtr pDic = m_firstDB->getVisualStyleDictionaryId().safeOpenObject(OdDb::kForWrite);
  OdDbVisualStylePtr pStyle = OdDbVisualStyle::createObject();
  //OdDbObjectId styleId = pDic->getAt( base );//OdDb::kszVSShaded );//kszVSGouraudWithEdges );
  OdDbObjectId styleId = getVsIdByName( m_firstDB, base );
  OdDbVisualStylePtr style = OdDbVisualStyle::cast( styleId.safeOpenObject( OdDb::kForWrite ) );
  pStyle->copyFrom( style.get() );
  pStyle->setDescription(name);
  //pStyle->setType( OdGiVisualStyle::Type::kCustom );
  pStyle->setTrait( OdGiVisualStyleProperties::kUseDrawOrder, false );
  pStyle->setTraitFlag( OdGiVisualStyleProperties::kEdgeModifiers, OdGiVisualStyleProperties::kEdgeOpacityFlag, false );
  pStyle->setTrait( OdGiVisualStyleProperties::kEdgeOpacity, 1.0 );
  pStyle->setTraitFlag( OdGiVisualStyleProperties::kFaceModifiers, OdGiVisualStyleProperties::kFaceOpacityFlag, false );
  pStyle->setTrait( OdGiVisualStyleProperties::kFaceOpacity, 1.0 );
  OdDbObjectId styleId1 = pDic->setAt(pStyle->description(), pStyle.get());
  return styleId1;
}


 OdDbObjectId OdqRevDiffHelper::createVisualStyleBasedOn( OdDbStub* orig, const OdString& name )
 {
    OdDbDictionaryPtr pDic = m_firstDB->getVisualStyleDictionaryId().safeOpenObject(OdDb::kForWrite);
    OdDbVisualStylePtr pStyle = OdDbVisualStyle::createObject();
    //OdDbObjectId styleId = pDic->getAt( base );//OdDb::kszVSShaded );//kszVSGouraudWithEdges );
    OdDbObjectId styleId = orig;
    OdDbVisualStylePtr style = OdDbVisualStyle::cast( styleId.safeOpenObject( OdDb::kForWrite ) );
    pStyle->copyFrom( style.get() );
    pStyle->setDescription(name);
    //pStyle->setType( OdGiVisualStyle::Type::kCustom );
    pStyle->setTrait( OdGiVisualStyleProperties::kUseDrawOrder, false );
    pStyle->setTraitFlag( OdGiVisualStyleProperties::kEdgeModifiers, OdGiVisualStyleProperties::kEdgeOpacityFlag, false );
    pStyle->setTrait( OdGiVisualStyleProperties::kEdgeOpacity, 1.0 );
    pStyle->setTraitFlag( OdGiVisualStyleProperties::kFaceModifiers, OdGiVisualStyleProperties::kFaceOpacityFlag, false );
    pStyle->setTrait( OdGiVisualStyleProperties::kFaceOpacity, 1.0 );
    OdDbObjectId styleId1 = pDic->setAt(pStyle->description(), pStyle.get());
    return styleId1;
 }


void OdqRevDiffHelper::applyVisualStyles()
{
  m_mainView->setVisualStyle( m_firstVS );
  OdGsClientViewInfo clientViewInfo1;
  m_mainView->clientViewInfo(clientViewInfo1);
  OdDbBaseDatabasePE* pDbPE1 = OdDbBaseDatabasePE::cast(m_firstDB).get();
  OdRxObjectPtr pViewport1 = pDbPE1->openObject(clientViewInfo1.viewportObjectId);
  OdAbstractViewPEPtr pViewPE1 = OdAbstractViewPE::cast(pViewport1);
  //pViewPE1->setRenderMode( pViewport1, OdDb::kGouraudShadedWithWireframe );
  pViewPE1->setVisualStyle( pViewport1, m_firstVS );
  pViewPE1->setRenderMode( pViewport1, OdDb::kGouraudShaded );

  m_secondView->setVisualStyle( m_secondVS );
  OdGsClientViewInfo clientViewInfo2;
  m_secondView->clientViewInfo(clientViewInfo2);
  OdDbBaseDatabasePE* pDbPE2 = OdDbBaseDatabasePE::cast(m_secondDB).get();
  OdRxObjectPtr pViewport2 = pDbPE2->openObject(clientViewInfo2.viewportObjectId);
  OdAbstractViewPEPtr pViewPE2 = OdAbstractViewPE::cast(pViewport2);
  //pViewPE1->setRenderMode( pViewport1, OdDb::kGouraudShadedWithWireframe );
  pViewPE2->setVisualStyle( pViewport2, m_secondVS );
  pViewPE2->setRenderMode( pViewport2, OdDb::kGouraudShaded );
}


void OdqRevDiffHelper::syncViews()
{
  m_secondView->setView( m_mainView->position(), m_mainView->target(), m_mainView->upVector(), m_mainView->fieldWidth(), 
    m_mainView->fieldHeight(), m_mainView->isPerspective() ? OdGsView::kPerspective : OdGsView::kParallel  );
  OdGsDCRectDouble rc;
  m_mainView->getViewport( rc );
  m_secondView->setViewport( rc );
}


void OdqRevDiffHelper::setViewsVisible( bool bMainVisible, bool bSecondVisible )
{
  if( bMainVisible && !m_mainView->isVisible() ) m_mainView->show();
  else if( !bMainVisible && m_mainView->isVisible() ) m_mainView->hide();

  if( bSecondVisible && !m_secondView->isVisible() ) m_secondView->show();
  else if( !bSecondVisible && m_secondView->isVisible() ) m_secondView->hide();
}


void OdqRevDiffHelper::setVSOpacity( double op, bool bFirst )
{
  if( bFirst )
  {
    if( fabs( op - m_dOpac1 ) < 1e-6 ) return;
    m_dOpac1 = op;
    OdDbVisualStylePtr pStyle = m_firstVS.openObject( OdDb::kForWrite );
    if( fabs( 1.0 - op ) < 1e-6 )
    {
      pStyle->setTraitFlag( OdGiVisualStyleProperties::kEdgeModifiers, OdGiVisualStyleProperties::kEdgeOpacityFlag, false );
      pStyle->setTraitFlag( OdGiVisualStyleProperties::kFaceModifiers, OdGiVisualStyleProperties::kFaceOpacityFlag, false );
    }
    else
    {
      pStyle->setTraitFlag( OdGiVisualStyleProperties::kEdgeModifiers, OdGiVisualStyleProperties::kEdgeOpacityFlag, true );
      pStyle->setTraitFlag( OdGiVisualStyleProperties::kFaceModifiers, OdGiVisualStyleProperties::kFaceOpacityFlag, true );
      pStyle->setTrait( OdGiVisualStyleProperties::kEdgeOpacity, m_dOpac1 );
      pStyle->setTrait( OdGiVisualStyleProperties::kFaceOpacity, m_dOpac1 );
    }
  }
  else
  {
    if( fabs( op - m_dOpac2 ) < 1e-6 ) return;
    m_dOpac2 = op;
    OdDbVisualStylePtr pStyle = m_secondVS.openObject( OdDb::kForWrite );
    if( fabs( 1.0 - op ) < 1e-6 )
    {
      pStyle->setTraitFlag( OdGiVisualStyleProperties::kEdgeModifiers, OdGiVisualStyleProperties::kEdgeOpacityFlag, false );
      pStyle->setTraitFlag( OdGiVisualStyleProperties::kFaceModifiers, OdGiVisualStyleProperties::kFaceOpacityFlag, false );
    }
    else
    {
      pStyle->setTraitFlag( OdGiVisualStyleProperties::kEdgeModifiers, OdGiVisualStyleProperties::kEdgeOpacityFlag, true );
      pStyle->setTraitFlag( OdGiVisualStyleProperties::kFaceModifiers, OdGiVisualStyleProperties::kFaceOpacityFlag, true );
      pStyle->setTrait( OdGiVisualStyleProperties::kEdgeOpacity, m_dOpac2 );
      pStyle->setTrait( OdGiVisualStyleProperties::kFaceOpacity, m_dOpac2 );
    }
  }
}


bool OdqRevDiffHelper::isValid() const
{
  if( m_firstDB.isNull() ) return false;
  if( m_secondDB.isNull() ) return false;
  if( !m_mainView || !m_secondView || !m_device || !m_model ) return false;
  if( !m_firstVS.isValid() || !m_secondVS.isValid() ) return false;
  return true;
}


void OdqRevDiffHelper::setVSbyVS( const OdString& name )
{
  OdDbStub* orig = getOrigVisualStyle( name );
  visualStylesMap::iterator it;
  OdDbObjectId vs1, vs2;
  static int nameMod = 1;
  it = m_firstVSMap.find( orig );
  if( it == m_firstVSMap.end() )
  {
    OdString name;
    name = name.format(L"AutoCustomStyle%d", nameMod++ );
    vs1 = createVisualStyleBasedOn( orig, name );
    m_firstVSMap[ orig ] = vs1;
  }
  else
  {
    vs1 = it->second;
  }

  if( vs1 != m_firstVS )
  {
    m_firstVS = vs1;
    double op = m_dOpac1;
    m_dOpac1 = 1.0;
    setVSOpacity( op, true );
  }

  it = m_secondVSMap.find( orig );
  if( it == m_secondVSMap.end() )
  {
    OdString name;
    name = name.format(L"AutoCustomStyle%d", nameMod++ );
    vs2 = createVisualStyleBasedOn( orig, name );
    m_secondVSMap[ orig ] = vs1;
  }
  else
  {
    vs2 = it->second;
  }

  if( vs2 != m_secondVS )
  {
    m_secondVS = vs2;
    double op = m_dOpac2;
    m_dOpac2 = 1.0;
    setVSOpacity( op, false );
  }

  applyVisualStyles();
}

OdDbStub* OdqRevDiffHelper::getOrigVisualStyle( const OdString& sName )
{
  return getVsIdByName( m_firstDB, sName );
}

void OdqRevDiffHelper::setReColorOption(bool bFirstRevision, bool doReColor, const OdCmColor& color, bool b2DOnly)
{
  if (bFirstRevision)
  {
    m_bReColorFirst = doReColor;
    m_clReColorFirst = color;
    m_b2DOnlyFirst = b2DOnly;
  }
  else
  {
    m_bReColorSecond = doReColor;
    m_clReColorSecond = color;
    m_b2DOnlySecond = b2DOnly;
  }
}

//RevisionDiffSingleViewHelper

RevisionDiffSingleViewHelper::RevisionDiffSingleViewHelper()
{
  m_pView = NULL;
  clear();
}

RevisionDiffSingleViewHelper::~RevisionDiffSingleViewHelper()
{
  clear();
}

void RevisionDiffSingleViewHelper::setDB( OdDbDatabasePtr db1, OdDbDatabasePtr db2 )
{
  m_firstDB = db1;
  m_secondDB = db2;
}


OdDbDatabasePtr RevisionDiffSingleViewHelper::getDatabase( bool first )
{
  if( first ) return m_firstDB;
  return m_secondDB;
}


void RevisionDiffSingleViewHelper::setDevice( OdGsDevicePtr d )
{
  m_device = d;
}


OdGsDevice* RevisionDiffSingleViewHelper::getDevice()
{
  return m_device;
}


void RevisionDiffSingleViewHelper::setModel( OdGsModel* m )
{
  m_model = m;
}


OdGsModel* RevisionDiffSingleViewHelper::getModel()
{
  return m_model;
}

OdGsView* RevisionDiffSingleViewHelper::getView()
{
  return m_pView;
}

void RevisionDiffSingleViewHelper::setHandles( const OdDbHandleList& handles )
{
  m_handles = handles;
}

void RevisionDiffSingleViewHelper::clear()
{
  clearVS();
  m_bShowAll = true;
  m_dOpac1 = 1.0;
  m_dOpac2 = 1.0;
  m_firstVSMap.clear();
  m_secondVSMap.clear();
  m_firstVS.setNull();
  m_secondVS.setNull();
  m_origVS.setNull();
  if( m_pView )
  {
    m_pView->eraseAll();
  }
  m_device.release();
  if( !m_secondDB.isNull() )
  {
    m_secondDB.release();
  }
  if( !m_firstDB.isNull() )
  {
    m_firstDB.release();
  }
  m_model = NULL;
  m_pView = NULL;
  m_handles.clear();

  m_bReColorFirst = false;
  m_clReColorFirst.setRGB( 255, 0, 0 );
  m_b2DOnlyFirst = true;
  m_bReColorSecond = false;
  m_clReColorSecond.setRGB( 0, 255, 0 );
  m_b2DOnlySecond = true;
}

bool RevisionDiffSingleViewHelper::isValid() const
{
  if( m_firstDB.isNull() ) return false;
  if( m_secondDB.isNull() ) return false;
  return true;
}

void RevisionDiffSingleViewHelper::setupView( bool bShowAll, const OdDbHandleList& handles )
{
  if( !isValid() ) return;
  if( !m_pView )
  {
    m_pView = m_device->viewAt( 0 );
  }
  ODA_ASSERT( m_pView != NULL );
  m_pView->eraseAll();
  setHandles( handles );
  m_bShowAll = bShowAll;
  loadFirstEntities();
  loadSecondEntities();
}

void RevisionDiffSingleViewHelper::reloadView( bool bShowAll )
{
  if( !isValid() ) return;
  if( m_bShowAll == bShowAll ) return;
  m_pView->eraseAll();
  m_bShowAll = bShowAll;
  loadFirstEntities();
  loadSecondEntities();
}
#include "DbBlockTableRecord.h"
#include "Gs/GsModel.h"
void RevisionDiffSingleViewHelper::loadFirstEntities()
{
  if( m_bShowAll )
  {
    OdDbBlockTableRecordPtr rec = OdDbBlockTableRecord::cast( m_firstDB->getModelSpaceId().safeOpenObject( OdDb::kForWrite, true ) );
    //if( !m_firstVS.isNull() )
    {
      OdDbObjectIteratorPtr pIter = rec->newIterator( true, false, true );
      for (;  !pIter->done(); pIter->step() )
      {
        OdDbEntityPtr ent = pIter->entity( OdDb::kForWrite, true );
        if( !m_firstVS.isNull() )
        {
          ent->setVisualStyle( m_firstVS );
        }
        if( m_bReColorFirst )
        {
          bool bDO = !m_b2DOnlyFirst;
          if( !bDO )
          {
            OdUInt32 flags = ent->setAttributes( NULL );
            bDO = !(flags & OdGiDrawable::kDrawableRegenTypeDependantGeometry);
          }
          if( bDO ) ent->setColor( m_clReColorFirst );
        }
        m_pView->add( ent, m_model );
      }
    }
    //m_pView->add( rec, m_model );
  }
  else
  {
    for( OdUInt32 i = 0; i < m_handles.size(); ++i )
    {
      OdDbObjectId id1 = m_firstDB->getOdDbObjectId( m_handles[i], false );
      if( id1.isValid() )
      {
        try
        {
          OdDbObjectPtr objPtr = id1.safeOpenObject( OdDb::kForWrite );
          if( !objPtr.isNull() )
          {
            OdDbEntityPtr ent = OdDbEntity::cast( objPtr );
            if( !ent.isNull() )
            {
              if( !m_firstVS.isNull() )
              {
                ent->setVisualStyle( m_firstVS );
              }
              if( m_bReColorFirst )
              {
                bool bDO = !m_b2DOnlyFirst;
                if( !bDO )
                {
                  OdUInt32 flags = ent->setAttributes( NULL );
                  bDO = !(flags & OdGiDrawable::kDrawableRegenTypeDependantGeometry);
                }
                if( bDO ) ent->setColor( m_clReColorFirst );
              }
              m_pView->add( ent, m_model );
            }
          }
        }
        catch( ... )
        {
          //Nothing
        }
      }
    }
  }
}


void RevisionDiffSingleViewHelper::clearVS()
{
  for( OdUInt32 i = 0; i < m_handles.size(); ++i )
  {
    OdDbObjectId id1 = m_firstDB->getOdDbObjectId( m_handles[i], false );
    if( id1.isValid() )
    {
      try
      {
        OdDbObjectPtr objPtr = id1.safeOpenObject( OdDb::kForWrite );
        if( !objPtr.isNull() )
        {
          OdDbEntityPtr ent = OdDbEntity::cast( objPtr );
          if( !ent.isNull() )
          {
            ent->setVisualStyle( NULL );
          }
        }
      }
      catch( ... )
      {
        //Nothing
      }
    }
  }
}

void RevisionDiffSingleViewHelper::loadSecondEntities()
{
  m_secondEntitiesList.clear();
  OdCmColor color;
  color.setRGB(255, 0, 0);
  for( OdUInt32 i = 0; i < m_handles.size(); ++i )
  {
    OdDbObjectId id1 = m_secondDB->getOdDbObjectId( m_handles[i], false );
    if( id1.isValid() )
    {
      try
      {
        OdDbObjectPtr objPtr = id1.safeOpenObject( OdDb::kForWrite );
        if( !objPtr.isNull() )
        {
          OdDbEntityPtr ent = OdDbEntity::cast( objPtr );
          if( !ent.isNull() )
          {
            if( m_bReColorSecond )
            {
              bool bDO = !m_b2DOnlySecond;
              if( !bDO )
              {
                OdUInt32 flags = ent->setAttributes( NULL );
                bDO = !(flags & OdGiDrawable::kDrawableRegenTypeDependantGeometry);
              }
              if( bDO ) ent->setColor( m_clReColorSecond );
            }
            m_secondEntitiesList.push_back( m_handles[i] );
            if( !m_secondVS.isNull() )
            {
              //ent->setVisualStyle( m_secondVS );
            }
            m_pView->add( ent, m_model );
          }
        }
      }
      catch( ... )
      {
        //Nothing
      }
    }
  }
}

OdDbObjectId RevisionDiffSingleViewHelper::createVisualStyleBasedOn( const OdString& base, const OdString& name, bool bSecondDB )
{
  OdDbDictionaryPtr pDic;
  if( !bSecondDB ) pDic = m_firstDB->getVisualStyleDictionaryId().safeOpenObject(OdDb::kForWrite);
  else pDic = m_secondDB->getVisualStyleDictionaryId().safeOpenObject(OdDb::kForWrite);
  OdDbVisualStylePtr pStyle = OdDbVisualStyle::createObject();
  OdDbObjectId styleId;
  if( !bSecondDB ) styleId = getVsIdByName( m_firstDB, base );
  else styleId = getVsIdByName( m_secondDB, base );
  OdDbVisualStylePtr style = OdDbVisualStyle::cast( styleId.safeOpenObject( OdDb::kForWrite ) );
  pStyle->copyFrom( style.get() );
  pStyle->setDescription(name);
  pStyle->setTrait( OdGiVisualStyleProperties::kUseDrawOrder, false );
  pStyle->setTraitFlag( OdGiVisualStyleProperties::kEdgeModifiers, OdGiVisualStyleProperties::kEdgeOpacityFlag, false );
  pStyle->setTrait( OdGiVisualStyleProperties::kEdgeOpacity, 1.0 );
  pStyle->setTraitFlag( OdGiVisualStyleProperties::kFaceModifiers, OdGiVisualStyleProperties::kFaceOpacityFlag, false );
  pStyle->setTrait( OdGiVisualStyleProperties::kFaceOpacity, 1.0 );
  OdDbObjectId styleId1 = pDic->setAt(pStyle->description(), pStyle.get());
  return styleId1;
}


 OdDbObjectId RevisionDiffSingleViewHelper::createVisualStyleBasedOn( OdDbStub* orig, const OdString& name, bool bSecondDB )
 {
    OdDbDictionaryPtr pDic;
    if( !bSecondDB ) pDic = m_firstDB->getVisualStyleDictionaryId().safeOpenObject(OdDb::kForWrite);
    else pDic = m_secondDB->getVisualStyleDictionaryId().safeOpenObject(OdDb::kForWrite);
    OdDbVisualStylePtr pStyle = OdDbVisualStyle::createObject();
    OdDbObjectId styleId = orig;
    OdDbVisualStylePtr style = OdDbVisualStyle::cast( styleId.safeOpenObject( OdDb::kForWrite ) );
    pStyle->copyFrom( style.get() );
    pStyle->setDescription(name);
    pStyle->setTrait( OdGiVisualStyleProperties::kUseDrawOrder, false );
    pStyle->setTraitFlag( OdGiVisualStyleProperties::kEdgeModifiers, OdGiVisualStyleProperties::kEdgeOpacityFlag, false );
    pStyle->setTrait( OdGiVisualStyleProperties::kEdgeOpacity, 1.0 );
    pStyle->setTraitFlag( OdGiVisualStyleProperties::kFaceModifiers, OdGiVisualStyleProperties::kFaceOpacityFlag, false );
    pStyle->setTrait( OdGiVisualStyleProperties::kFaceOpacity, 1.0 );
    OdDbObjectId styleId1 = pDic->setAt(pStyle->description(), pStyle.get());
    return styleId1;
 }


void RevisionDiffSingleViewHelper::applyVisualStyles()
{
  m_pView->setVisualStyle( m_origVS );
  //m_pView->setVisualStyle( m_secondVS );
  //m_pView->setVisualStyle( m_firstVS );
  OdGsClientViewInfo clientViewInfo1;
  m_pView->clientViewInfo(clientViewInfo1);
  OdDbBaseDatabasePE* pDbPE1 = OdDbBaseDatabasePE::cast(m_firstDB).get();
  OdRxObjectPtr pViewport1 = pDbPE1->openObject(clientViewInfo1.viewportObjectId);
  OdAbstractViewPEPtr pViewPE1 = OdAbstractViewPE::cast(pViewport1);
  //pViewPE1->setRenderMode( pViewport1, OdDb::kGouraudShadedWithWireframe );
  pViewPE1->setVisualStyle( pViewport1, m_origVS );
  //pViewPE1->setVisualStyle( pViewport1, m_secondVS );
  //pViewPE1->setVisualStyle( pViewport1, m_firstVS );
  pViewPE1->setRenderMode( pViewport1, OdDb::kGouraudShaded );

  m_bShowAll = ! m_bShowAll;
  reloadView( !m_bShowAll );
}


void RevisionDiffSingleViewHelper::setViewsVisible( bool bMainVisible, bool bSecondVisible )
{
/*  if( bMainVisible && !m_mainView->isVisible() ) m_mainView->show();
  else if( !bMainVisible && m_mainView->isVisible() ) m_mainView->hide();

  if( bSecondVisible && !m_secondView->isVisible() ) m_secondView->show();
  else if( !bSecondVisible && m_secondView->isVisible() ) m_secondView->hide();*/
}


void RevisionDiffSingleViewHelper::setVSOpacity( double op, bool bFirst )
{
  if( bFirst )
  {
    if( fabs( op - m_dOpac1 ) < 1e-6 ) return;
    m_dOpac1 = op;
    OdDbVisualStylePtr pStyle = m_firstVS.openObject( OdDb::kForWrite );
    if( fabs( 1.0 - op ) < 1e-6 )
    {
      pStyle->setTraitFlag( OdGiVisualStyleProperties::kEdgeModifiers, OdGiVisualStyleProperties::kEdgeOpacityFlag, false );
      pStyle->setTraitFlag( OdGiVisualStyleProperties::kFaceModifiers, OdGiVisualStyleProperties::kFaceOpacityFlag, false );
    }
    else
    {
      pStyle->setTraitFlag( OdGiVisualStyleProperties::kEdgeModifiers, OdGiVisualStyleProperties::kEdgeOpacityFlag, true );
      pStyle->setTraitFlag( OdGiVisualStyleProperties::kFaceModifiers, OdGiVisualStyleProperties::kFaceOpacityFlag, true );
      pStyle->setTrait( OdGiVisualStyleProperties::kEdgeOpacity, m_dOpac1 );
      pStyle->setTrait( OdGiVisualStyleProperties::kFaceOpacity, m_dOpac1 );
    }
  }
  else
  {
    if( fabs( op - m_dOpac2 ) < 1e-6 ) return;
    m_dOpac2 = op;
    OdDbVisualStylePtr pStyle = m_secondVS.openObject( OdDb::kForWrite );
    if( fabs( 1.0 - op ) < 1e-6 )
    {
      pStyle->setTraitFlag( OdGiVisualStyleProperties::kEdgeModifiers, OdGiVisualStyleProperties::kEdgeOpacityFlag, false );
      pStyle->setTraitFlag( OdGiVisualStyleProperties::kFaceModifiers, OdGiVisualStyleProperties::kFaceOpacityFlag, false );
    }
    else
    {
      pStyle->setTraitFlag( OdGiVisualStyleProperties::kEdgeModifiers, OdGiVisualStyleProperties::kEdgeOpacityFlag, true );
      pStyle->setTraitFlag( OdGiVisualStyleProperties::kFaceModifiers, OdGiVisualStyleProperties::kFaceOpacityFlag, true );
      pStyle->setTrait( OdGiVisualStyleProperties::kEdgeOpacity, m_dOpac2 );
      pStyle->setTrait( OdGiVisualStyleProperties::kFaceOpacity, m_dOpac2 );
    }
  }
}

void RevisionDiffSingleViewHelper::setVSbyVS( const OdString& name )
{
  OdDbStub* orig = getOrigVisualStyle( name );
  if( orig == m_origVS ) return;
  m_origVS = orig;
  OdDbVisualStylePtr pStyle = m_origVS.openObject( OdDb::kForWrite );
  {
    pStyle->setTraitFlag( OdGiVisualStyleProperties::kEdgeModifiers, OdGiVisualStyleProperties::kEdgeOpacityFlag, true );
    pStyle->setTraitFlag( OdGiVisualStyleProperties::kFaceModifiers, OdGiVisualStyleProperties::kFaceOpacityFlag, true );
    pStyle->setTrait( OdGiVisualStyleProperties::kEdgeOpacity, 0.5 );
    pStyle->setTrait( OdGiVisualStyleProperties::kFaceOpacity, 0.5 );
  }
  visualStylesMap::iterator it;
  OdDbObjectId vs1, vs2;
  static int nameMod = 1;
  it = m_firstVSMap.find( orig );
  if( it == m_firstVSMap.end() )
  {
    OdString name;
    name = name.format(L"AutoCustomStyle%d", nameMod++ );
    vs1 = createVisualStyleBasedOn( orig, name );
    m_firstVSMap[ orig ] = vs1;
  }
  else
  {
    vs1 = it->second;
  }

  if( vs1 != m_firstVS )
  {
    m_firstVS = vs1;
    double op = m_dOpac1;
    m_dOpac1 = 1.0;
    setVSOpacity( op, true );
  }

  it = m_secondVSMap.find( orig );
  if( it == m_secondVSMap.end() )
  {
    OdString name;
    name = name.format(L"AutoCustomStyle%d", nameMod++ );
    OdString origName;
    pStyle->name( origName );
    vs2 = createVisualStyleBasedOn( origName, name );
    m_secondVSMap[ orig ] = vs1;
  }
  else
  {
    vs2 = it->second;
  }

  if( vs2 != m_secondVS )
  {
    m_secondVS = vs2;
    double op = m_dOpac2;
    m_dOpac2 = 1.0;
    setVSOpacity( op, false );
  }

  applyVisualStyles();
}

OdDbStub* RevisionDiffSingleViewHelper::getOrigVisualStyle( const OdString& sName )
{
  return getVsIdByName( m_firstDB, sName );
}

void RevisionDiffSingleViewHelper::setReColorOption( bool bFirstRevision, bool doReColor, const OdCmColor& color, bool b2DOnly )
{
  if( bFirstRevision )
  {
    m_bReColorFirst = doReColor;
    m_clReColorFirst = color;
    m_b2DOnlyFirst = b2DOnly;
  }
  else
  {
    m_bReColorSecond = doReColor;
    m_clReColorSecond = color;
    m_b2DOnlySecond = b2DOnly;
  }
}
