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
#define STL_USING_MAP
#define STL_USING_ALGORITHM
#include "OdaSTL.h"
#include "UInt32Array.h"
#include "Ge/GePoint3d.h"
#include "Gi/GiDrawableImpl.h"
#include "Gi/GiWorldDraw.h"
#include "Gi/GiViewportDraw.h"
#include "DbHostAppServices.h"
#include "DbCommandContext.h"
#include "DbEntity.h"
#include "DbAbstractViewportData.h"
#include "RxVariantValue.h"
#include "ExGripManager.h"
#include "Gs/GsModel.h"

// Menu animation flags
#if !defined(ODA_UNIXOS) //EMCC
#ifndef TPM_VERPOSANIMATION
static const UINT TPM_VERPOSANIMATION = 0x1000L;
#endif
#ifndef TPM_NOANIMATION
static const UINT TPM_NOANIMATION = 0x4000L;
#endif
#endif //#ifndef EMCC
//

#define GM_PAGE_EACH_OBJECT 200

namespace
{

static OdSelectionSetIteratorPtr searchObjectSSetIterator(OdSelectionSetPtr pSSet, OdDbStub* id)
{
  OdSelectionSetIteratorPtr pIter = pSSet->newIterator();
  while (!pIter->done())
  {
    if (pIter->id() == id)
    {
      return pIter;
    }
    pIter->next();
  }
  return OdSelectionSetIteratorPtr();
}

static OdBaseGripManager::OdExGripDataSubent& getSubentGripData(OdBaseGripManager::OdExGripDataExt &ext, OdDbBaseFullSubentPath entPath)
{
  for (OdUInt32 i = 0; i < ext.m_pDataSub.size(); i++)
  {
    if (ext.m_pDataSub.at(i).m_entPath == entPath)
      return ext.m_pDataSub.at(i);
  }
  ODA_FAIL();
  return ext.m_pDataSub.at(0);
}

}

OdExGripDragPtr OdExGripDrag::createObject(OdDbStub* id, OdBaseGripManager* pOwner)
{
  OdExGripDragPtr pRes = RXIMPL_CONSTR(OdExGripDrag);
  //pRes->m_entPath = OdDbBaseFullSubentPath();
  pRes->m_entPath.objectIds().append(id);
  pRes->m_pOwner = pOwner;
  return pRes;
}

OdExGripDragPtr OdExGripDrag::createObject(OdDbBaseFullSubentPath entPath, OdBaseGripManager* pOwner)
{
  OdExGripDragPtr pRes = RXIMPL_CONSTR(OdExGripDrag);
  pRes->m_entPath = entPath;
  pRes->m_pOwner = pOwner;
  return pRes;
}

OdExGripDrag::OdExGripDrag()
{
  //m_entPath = OdDbBaseFullSubentPath();
  m_pClone = 0;
  m_pOwner = 0;
}

OdExGripDrag::~OdExGripDrag()
{
}

OdDbStub* OdExGripDrag::entityId() const 
{ 
  return m_entPath.objectIds().last(); 
}

bool OdExGripDrag::entPath(OdDbBaseFullSubentPath *pPath) const // = NULL
{
  if (pPath)
    *pPath = m_entPath;
  return m_entPath.subentId() != OdDbSubentId();
}

bool OdExGripDrag::locateActiveGrips( OdIntArray& aIndices )
{
  const OdExGripDataPtrArray& rData = (entPath()) ? getSubentGripData(m_pOwner->m_aGripData[entityId()], m_entPath).m_pSubData
                                                  : m_pOwner->m_aGripData[ entityId() ].m_pDataArray;

  bool bExMethod = true;
  aIndices.clear();
  OdUInt32 i, iSize = rData.size();
  for( i = 0; i < iSize; i++ )
  {
    if ( rData[ i ]->data().isNull() )
        bExMethod = false;

    if ( OdDbGripOperations::kDragImageGrip == rData[ i ]->status() )
        aIndices.push_back( i );
  }
  ODA_ASSERT( iSize == 0 || !aIndices.empty() );
  return bExMethod;
}

void OdExGripDrag::cloneEntity()
{
  m_pClone = m_pOwner ? m_pOwner->cloneEntity(entityId()) : OdGiDrawablePtr();
}
OdGiDrawablePtr OdExGripManager::cloneEntity(OdDbStub* id)
{
  OdDbEntityPtr pEntity = OdDbEntity::cast(OdDbObjectId(id).openObject());
  if (pEntity.isNull())
    return OdGiDrawablePtr();

  OdDbEntityPtr pClone;
  if (pEntity->cloneMeForDragging())
  {
    pClone = OdDbEntity::cast( pEntity->clone() );
  }

  if (pClone.get())
  {
    pClone->disableUndoRecording( true );
    pClone->setPropertiesFrom( pEntity.get(), false );
  }

  return OdGiDrawable::cast(pClone);
}

void OdExGripDrag::cloneEntity(const OdGePoint3d& ptMoveAt)
{
  cloneEntity();
  if (m_pClone.isNull())
    return;

  OdIntArray aIndices;
  bool bExMethod = locateActiveGrips( aIndices );

  OdGeVector3d vOffset =
    ptMoveAt - m_pOwner->m_ptBasePoint;

  if ( bExMethod )
  {
    OdDbGripDataPtrArray aCloneData;
    if (entPath())
    {
      m_pOwner->getGripPointsAtSubentPath(m_pClone, m_entPath, aCloneData,
        m_pOwner->activeViewUnitSize(), m_pOwner->m_GRIPSIZE, m_pOwner->activeViewDirection(), 0);
    }
    else
    {
      m_pOwner->getGripPoints(m_pClone, aCloneData,
        m_pOwner->activeViewUnitSize(), m_pOwner->m_GRIPSIZE, m_pOwner->activeViewDirection(), 0);
    }

    OdDbVoidPtrArray aIds;
    OdUInt32 i, iSize = aIndices.size();
    for ( i = 0; i < iSize; i++ )
    {
      if ( aIndices[ i ] < (OdInt32)aCloneData.size() )
      {
        aIds.push_back( aCloneData[ aIndices[ i ] ]->appData() );
      }
      else
      {
        ODA_ASSERT( 0 );
      }
    }
    for (i = 0; i < aCloneData.size(); ++i)
      if (aCloneData[i]->gripOpStatFunc())
        (aCloneData[i]->gripOpStatFunc())(aCloneData[i], OdDbObjectId::kNull, OdDbGripOperations::kGripStart);

    if (entPath())
    {
      OdDbBaseFullSubentPathArray aPaths;
      aPaths.append(m_entPath);
      m_pOwner->moveGripPointsAtSubentPaths(m_pClone, aPaths, aIds, vOffset, 0);
      m_pOwner->subentGripStatus(m_pClone, OdDb::kGripsToBeDeleted, m_entPath);
    }
    else
    {
      m_pOwner->moveGripPointsAt(m_pClone, aIds, vOffset, 0);
      m_pOwner->gripStatus(m_pClone, OdDb::kGripsToBeDeleted);
      //m_pClone->gripStatus( OdDb::kDimDataToBeDeleted );
    }
    for (i = 0; i < aCloneData.size(); ++i)
      if (aCloneData[i]->gripOpStatFunc())
        (aCloneData[i]->gripOpStatFunc())(aCloneData[i], NULL, OdDbGripOperations::kGripEnd);
  }
  else
  {
    //OdGePoint3dArray aPts;
    //m_pClone->getGripPoints( aPts );

    //OdUInt32 i, iSize = aIndices.size();
    //for( i = 0; i < iSize; i++ )
    //{
    //  if ( aIndices[ i ] < (OdInt32)aPts.size() ) 
    //  {
    //    aPts[ aIndices[ i ] ] += vOffset;
    //  }
    //  else
    //  {
    //    ODA_ASSERT( 0 );
    //  }
    //}

    m_pOwner->moveGripPointsAt(m_pClone, aIndices, vOffset);
    m_pOwner->gripStatus(m_pClone, OdDb::kGripsToBeDeleted);
  }
  m_pOwner->onModified(this);
}
void OdExGripManager::onModified(OdGiDrawable* pGrip)
{
  if (getGsModel())
    getGsModel()->onModified(pGrip, (OdGiDrawable*)0);
  else if (getGsLayoutHelper())
    getGsLayoutHelper()->invalidate();
}

void OdExGripDrag::moveEntity( const OdGePoint3d& ptMoveAt )
{
  OdIntArray aIndices;
  bool bExMethod = locateActiveGrips( aIndices );

  OdGeVector3d vOffset =
    ptMoveAt - m_pOwner->m_ptBasePoint;

  OdGiDrawablePtr pEntity = m_pOwner->openObject(entityId(), OdDb::kForWrite);
  ODA_ASSERT(pEntity.get());

  const OdExGripDataPtrArray& rData =
    (entPath()) ? getSubentGripData(m_pOwner->m_aGripData[entityId()], m_entPath).m_pSubData
                : m_pOwner->m_aGripData[ entityId() ].m_pDataArray;

  if ( bExMethod )
  {
    OdDbVoidPtrArray aIds;
    OdUInt32 i, iSize = aIndices.size();
    for ( i = 0; i < iSize; i++ )
    {
      if ( aIndices[ i ] < (OdInt32)rData.size() )
      {
        aIds.push_back( rData[ aIndices[ i ] ]->data()->appData() );
      }
      else
      {
        ODA_ASSERT( 0 );
      }
    }

    if (entPath())
    {
      OdDbBaseFullSubentPathArray aPaths;
      aPaths.append(m_entPath);
      m_pOwner->moveGripPointsAtSubentPaths(pEntity, aPaths, aIds, vOffset, 0);
    }
    else
    {
      m_pOwner->moveGripPointsAt(pEntity, aIds, vOffset, 0);
    }
  }
  else
  {
    //OdGePoint3dArray aPts;
    //OdUInt32 i, iSize = rData.size();
    //aPts.resize( iSize );
    //for ( i = 0; i < iSize; i++ )
    //  aPts[ i ] = rData[ i ]->point();

    //iSize = aIndices.size();
    //for( i = 0; i < iSize; i++ )
    //{
    //  if ( aIndices[ i ] < (OdInt32)rData.size() ) 
    //  {
    //    aPts[ aIndices[ i ] ] += vOffset;
    //  }
    //  else
    //  {
    //    ODA_ASSERT( 0 );
    //  }
    //}

    m_pOwner->moveGripPointsAt(pEntity, aIndices, vOffset);
  }
}

void OdExGripDrag::notifyDragStarted()
{
  if (!m_pOwner)
    return;
  OdGiDrawablePtr pEntity = m_pOwner->openObject(entityId());
  if (pEntity.get())
    m_pOwner->dragStatus(pEntity, OdDb::kDragStart);
  m_pOwner->draggingStarted();
}

void OdExGripDrag::notifyDragEnded()
{
  if (!m_pOwner)
    return;
  OdGiDrawablePtr pEntity = m_pOwner->openObject(entityId());
  if (pEntity.get())
    m_pOwner->dragStatus(pEntity, OdDb::kDragEnd);
  m_pOwner->draggingStopped();
}

void OdExGripDrag::notifyDragAborted()
{
  if (!m_pOwner)
    return;
  OdGiDrawablePtr pEntity = m_pOwner->openObject(entityId());
  if (pEntity.get())
    m_pOwner->dragStatus(pEntity, OdDb::kDragAbort);
  m_pOwner->draggingStopped();
}

OdUInt32 OdExGripDrag::subSetAttributes(OdGiDrawableTraits* pTraits) const
{
  if (m_pClone.isNull())
    return kDrawableIsInvisible;

  OdUInt32 iRet = m_pClone->setAttributes(pTraits);

  OdGiSubEntityTraitsPtr pEntityTraits = OdGiSubEntityTraits::cast(pTraits);
  if (pEntityTraits.get())
    pEntityTraits->setFillType(kOdGiFillNever);

  return iRet;
}

bool OdExGripDrag::subWorldDraw(OdGiWorldDraw* pWorldDraw) const
{
  if (m_pClone.isNull())
    return true;
  return m_pClone->worldDraw(pWorldDraw);
}

void OdExGripDrag::subViewportDraw(OdGiViewportDraw* pViewportDraw) const
{
  if (m_pClone.get())
    m_pClone->viewportDraw(pViewportDraw);
}

OdExGripDataPtr OdExGripData::createObject(OdDbStub* id, OdDbGripDataPtr pData, const OdGePoint3d& pt, OdBaseGripManager* pOwner)
{ 
  OdExGripDataPtr pRes = RXIMPL_CONSTR(OdExGripData);
  //pRes->m_entPath = OdDbBaseFullSubentPath();
  pRes->m_entPath.objectIds().append(id);
  pRes->m_pData = pData;
  pRes->m_pOwner = pOwner;
  pRes->m_point = pt;
  return pRes;
}

OdExGripDataPtr OdExGripData::createObject(OdDbBaseFullSubentPath entPath, OdDbGripDataPtr pData, const OdGePoint3d& pt, OdBaseGripManager* pOwner)
{ 
  OdExGripDataPtr pRes = RXIMPL_CONSTR(OdExGripData);
  pRes->m_entPath = entPath;
  pRes->m_pData = pData;
  pRes->m_pOwner = pOwner;
  pRes->m_point = pt;
  return pRes;
}

OdExGripData::OdExGripData()
{
  m_status = OdDbGripOperations::kWarmGrip;
  m_bInvisible = false;
  m_bShared = false;
  m_point = OdGePoint3d::kOrigin;
  //m_entPath = OdDbBaseFullSubentPath();
  //m_pData = 0;
  m_pOwner = 0;
}

OdExGripData::~OdExGripData()
{
  if(m_pData.get() && m_pData->alternateBasePoint())
  {
    delete m_pData->alternateBasePoint();
    m_pData->setAlternateBasePoint(0);
  }
}

bool OdExGripData::computeDragPoint( OdGePoint3d& ptOverride ) const
{
  OdGePoint3d ptBase = point();
  if (data().get() && data()->alternateBasePoint())
    ptBase = *(data()->alternateBasePoint());

  bool bOverride = false;
  ptOverride = ptBase;

  if (status() == OdDbGripOperations::kDragImageGrip && data().get() && data()->drawAtDragImageGripPoint())
  {
    ptOverride = ptBase + (m_pOwner->m_ptLastPoint - m_pOwner->m_ptBasePoint);
    bOverride = true;
  }
  return bOverride;
}

OdUInt32 OdExGripData::subSetAttributes( OdGiDrawableTraits* pTraits ) const
{
  if (isInvisible())
    return kDrawableIsInvisible;

  OdGiSubEntityTraitsPtr pEntityTraits = OdGiSubEntityTraits::cast(pTraits);
  if (!pEntityTraits.get())
    return kDrawableNone;

  switch (status())
  {
  case OdDbGripOperations::kWarmGrip:
    pEntityTraits->setTrueColor( m_pOwner->m_GRIPCOLOR );
    break;
    
  case OdDbGripOperations::kHotGrip:
  case OdDbGripOperations::kDragImageGrip:
    pEntityTraits->setTrueColor( m_pOwner->m_GRIPHOT );
    break;
    
  case OdDbGripOperations::kHoverGrip:
    pEntityTraits->setTrueColor( m_pOwner->m_GRIPHOVER );
    break;
  }
  pEntityTraits->setMaterial(NULL/*entityId().database()->byBlockMaterialId()*/);
  pEntityTraits->setLineWeight(OdDb::kLnWt000);
  return kDrawableRegenDraw;
}

bool OdExGripData::subWorldDraw( OdGiWorldDraw* pWorldDraw ) const
{
  double dGripSize = m_pOwner->m_GRIPSIZE;
  if (!pWorldDraw->context() || !pWorldDraw->context()->database())
  {
    dGripSize = m_pOwner->m_GRIPSIZE;
  }
  //else
  //{
  //  // Here is the design flaw:
  //  // ARX help says that grip size passed in callback below should be
  //  // calculated individually for each viewport.
  //}

  if (data().get() && data()->worldDraw())
  {
    OdGePoint3d ptComputed;
    OdGePoint3d* pDrawAtDrag = 0;
    if ( computeDragPoint( ptComputed ) )
        pDrawAtDrag = &ptComputed;

    OdGiDrawFlagsHelper _dfh(pWorldDraw->subEntityTraits(), OdGiSubEntityTraits::kDrawNoPlotstyle);
    return( (*data()->worldDraw())(
        (OdDbGripData*)data().get(), pWorldDraw, entityId(),  status(), pDrawAtDrag, dGripSize ) );
  }

  return false;
}

void OdExGripData::subViewportDraw( OdGiViewportDraw* pViewportDraw ) const
{
  OdGePoint3d ptComputed;
  OdGePoint3d* pDrawAtDrag = 0;
  if ( computeDragPoint( ptComputed ) )
    pDrawAtDrag = &ptComputed;
  OdGiDrawFlagsHelper _dfh(pViewportDraw->subEntityTraits(), OdGiSubEntityTraits::kDrawNoPlotstyle);

  bool bDefault = true;
  if (data().get() && data()->viewportDraw())
  {
    (*data()->viewportDraw())(
        (OdDbGripData*)data().get(), pViewportDraw, entityId(), status(), pDrawAtDrag, m_pOwner->m_GRIPSIZE );
    bDefault = false;
  }

  if ( bDefault )
  {
    //if (m_pOwner->getGsModel() && m_pOwner->getGsModel()->renderType() < OdGsModel::kDirect) {
    //  // Commented since renderTypes implemented, so no need to translate objects for kDirect renderType
    //  OdGeVector3d vpDirection(pViewportDraw->viewport().viewDir());
    //  OdGePoint3d vpOrigin(pViewportDraw->viewport().getCameraLocation());
    //  double ptLength = (ptComputed - vpOrigin).dotProduct(vpDirection);
    //  ptComputed -= vpDirection * ptLength;
    //}

    double dGripSize;
    {
      OdGePoint2d ptDim;
      pViewportDraw->viewport().getNumPixelsInUnitSquare( point(), ptDim );
      OdGeVector3d v( m_pOwner->m_GRIPSIZE / ptDim.x, 0.0, 0.0 );
      v.transformBy( pViewportDraw->viewport().getWorldToEyeTransform() );
      //if (pViewportDraw->viewport().isPerspective())
      //{
      //  OdGePoint3d perspFix(v.length(), 0.0, OdGePoint3d(ptComputed).transformBy(pViewportDraw->viewport().getWorldToEyeTransform()).z);
      //  pViewportDraw->viewport().doInversePerspective(perspFix);
      //  v.x = perspFix.x;
      //  v.y = v.z = 0.0;
      //}
      dGripSize = v.length();
    }

    OdGePoint3d ptOnScreen = ptComputed; //+ 1E10 * pViewportDraw->viewport().viewDir();
    ptOnScreen.transformBy( pViewportDraw->viewport().getWorldToEyeTransform() );

    pViewportDraw->subEntityTraits().setFillType( kOdGiFillAlways );
    pViewportDraw->subEntityTraits().setDrawFlags( OdGiSubEntityTraits::kDrawSolidFill | OdGiSubEntityTraits::kDrawPolygonFill );

    //ptOnScreen.z = 0.;
        
    OdGePoint3d aPoly[ 4 ];
    aPoly[ 0 ].set( ptOnScreen.x - dGripSize, ptOnScreen.y - dGripSize, ptOnScreen.z );
    aPoly[ 1 ].set( ptOnScreen.x + dGripSize, ptOnScreen.y - dGripSize, ptOnScreen.z );
    aPoly[ 2 ].set( ptOnScreen.x + dGripSize, ptOnScreen.y + dGripSize, ptOnScreen.z );
    aPoly[ 3 ].set( ptOnScreen.x - dGripSize, ptOnScreen.y + dGripSize, ptOnScreen.z );
    pViewportDraw->geometry().polygonEye( 4, aPoly );
  }
}

OdBaseGripManager::OdBaseGripManager()
{
  m_aGripData.clear();
  m_aHoverGrips.clear();
  clockStartHover = 0;

  m_ptBasePoint = OdGePoint3d::kOrigin;
  m_ptLastPoint = OdGePoint3d::kOrigin;
  m_aDrags.clear();

  m_bDisabled = true;

  m_GRIPSIZE     = 5;
  m_GRIPOBJLIMIT = 100;
}

OdExGripManager::OdExGripManager()
{
  m_pDevice = 0;
  m_pCmdCtx = 0;
  m_pGsModel = 0;

  m_cDbReactor.m_pOwner = this;

  m_pGetSelectionSetPtr = 0;
}

OdBaseGripManager::~OdBaseGripManager()
{
  endHover();
}

OdExGripManager::~OdExGripManager()
{
}

void OdExGripManager::init( OdGsDevice* pDevice, OdGsModel* pGsModel, OdDbCommandContext* pCmdCtx, GetSelectionSetPtr pGetSSet )
{
  m_pDevice = pDevice;
  m_pGsModel = pGsModel;
  m_pCmdCtx = pCmdCtx;

  if(m_pCmdCtx->baseDatabase()) {
    OdDbDatabase* pDb = m_pCmdCtx->database();
    disable(false);

    OdDbHostAppServices* pAppSvcs = pDb->appServices();
    m_GRIPSIZE     = pAppSvcs->getGRIPSIZE();
    m_GRIPOBJLIMIT = pAppSvcs->getGRIPOBJLIMIT();
    m_GRIPCOLOR.setColorIndex(pAppSvcs->getGRIPCOLOR());
    m_GRIPHOVER.setColorIndex(pAppSvcs->getGRIPHOVER());
    m_GRIPHOT.setColorIndex(pAppSvcs->getGRIPHOT());
  }
  m_pGetSelectionSetPtr = pGetSSet;
  m_gripStretchCommand.m_parent = this;
}

void OdExGripManager::uninit()
{
  if(m_pCmdCtx)
  {
    disable(true);
    m_pCmdCtx = 0;
  }
  m_pDevice = 0;
}

void OdExGripManager::OdExGripCommand::execute(OdEdCommandContext* pCmdCtx)
{
  bool bOk = true;
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabase* pDb = pDbCmdCtx->database();
  try
  {
    OdGePoint3d ptFinal = m_parent->m_pCmdCtx->dbUserIO()->getPoint(
      OD_T("Specify stretch point or [eXit]:"),
      OdEd::kGptNoLimCheck | OdEd::kGptDefault | OdEd::kGptNoUCS,
      &m_parent->m_ptBasePoint,
      OD_T("eXit"),
      m_parent);

    pDb->startTransaction();
    for (unsigned i = 0; i < m_parent->m_aDrags.size(); i++)
      m_parent->m_aDrags[i]->moveEntity(m_parent->eyeToUcsPlane(ptFinal, m_parent->m_ptBasePoint));
    pDb->endTransaction();
  }
  catch (const OdEdCancel&)
  {
    pDb->endTransaction();
    bOk = false;
    for (unsigned i = 0; i < m_parent->m_aDrags.size(); i++)
      m_parent->m_aDrags[i]->notifyDragAborted();

  }

  for (unsigned i = 0; i < m_parent->m_aDrags.size(); i++)
  {
    if (bOk)
    {
      m_parent->m_aDrags[i]->notifyDragEnded();
      m_parent->updateEntityGrips(m_parent->m_aDrags[i]->entityId());
    }
    else
    {
      m_parent->m_aDrags[i]->notifyDragAborted();
    }
  }

  m_parent->m_aDrags.clear();

  if (bOk)
    m_parent->updateInvisibleGrips();

}

bool OdBaseGripManager::onMouseDown(int x, int y, bool bShift)
{
  endHover();

  OdExGripDataPtrArray aKeys;
  locateGripsAt( x, y, aKeys );

  if (aKeys.empty())
    return false;
  //{
  if (bShift)
  {
    // Modify Grip  status().
    OdDbGripOperations::DrawType eNewStatus = OdDbGripOperations::kHotGrip;
    OdUInt32 i, iSize = aKeys.size();
    for( i = 0; i < iSize; i++ )
    {
      if ( OdDbGripOperations::kHotGrip == aKeys[ i ]->status() )
      {
        eNewStatus = OdDbGripOperations::kWarmGrip;
        break;
      }
    }

    for ( i = 0; i < iSize; i++ )
    {
      OdDbGripOperations::DrawType eCurStatus = eNewStatus;
      OdExGripDataPtr pGrip = aKeys[ i ];
      if ( !pGrip->data().isNull() )
      {
        if ( pGrip->data()->triggerGrip() )
        {
          eCurStatus = OdDbGripOperations::kWarmGrip;
        }
        else
        {
          if (pGrip->data()->hotGripFunc())
          {
            int iFlags = OdDbGripOperations::kMultiHotGrip;
            if ( pGrip->isShared() )
              iFlags |= OdDbGripOperations::kSharedGrip;

            OdResult eRet =
              (*pGrip->data()->hotGripFunc())( pGrip->data(), pGrip->entityId(), iFlags );
            switch (eRet)
            {
            case eGripOpGripHotToWarm :
              eCurStatus = OdDbGripOperations::kWarmGrip;
              break;
            default:
              // no op
              break;
            }
          }
        }
      }
      aKeys[ i ]-> setStatus( eCurStatus );
    }
  }
  else // of if ( bShift )
  {
    // Launch Grip Edit.
    bool bMakeHot = true;
    {
      GripDataMap::const_iterator it = m_aGripData.begin();
      while( (it != m_aGripData.end()) && bMakeHot )
      {
        const OdExGripDataPtrArray& aData = it->second.m_pDataArray;

        OdUInt32 i, iSize = aData.size();
        for ( i = 0; i < iSize; i++ )
        {
          if ( OdDbGripOperations::kHotGrip == aData[ i ]->status() )
          {
            bMakeHot = false;
            break;
          }
        }
        for (i = 0; (i < it->second.m_pDataSub.size()) && bMakeHot; i++)
        {
          const OdExGripDataPtrArray& aData = it->second.m_pDataSub.at(i).m_pSubData;
          OdUInt32 j, iSize = aData.size();
          for ( j = 0; j < iSize; j++ )
          {
            if (OdDbGripOperations::kHotGrip == aData[j]->status())
            {
              bMakeHot = false;
              break;
            }
          }
        }
        it++;
      }
    }

    bool bGetNew = false;
    OdDbObjectId idEntityToUpdate;
    if ( bMakeHot )
    {
      OdUInt32 i, iSize = aKeys.size();
      for ( i = 0; i < iSize; i++ )
      {
        OdExGripDataPtr pGrip = aKeys[ i ];

        OdDbGripOperations::DrawType eNew = OdDbGripOperations::kHotGrip;

        if (!pGrip->data().isNull() && pGrip->data()->hotGripFunc())
        {
          int iFlags = 0;
          if ( pGrip->isShared() )
            iFlags |= OdDbGripOperations::kSharedGrip;

          if (pGrip->data()->triggerGrip())
          {
            if (!pGrip->isShared())
            {
              OdResult eRet =
                (*pGrip->data()->hotGripFunc())( pGrip->data(), pGrip->entityId(), iFlags );
              switch (eRet)
              {
              case eOk :
              case eGripOpGripHotToWarm :
                eNew = OdDbGripOperations::kWarmGrip;
                break;
              case eGripOpGetNewGripPoints :
                bGetNew = true;
                idEntityToUpdate = pGrip->entityId();
                break;
              default:
                // no op
                break;
              }
            }
          }
          else // of if (pGrip->data()->triggerGrip())
          {
            OdResult eRet =
              (*pGrip->data()->hotGripFunc())( pGrip->data(), pGrip->entityId(), iFlags );
            if (!pGrip->isShared())
            {
              switch ( eRet )
              {
              case eGripOpGripHotToWarm :
                eNew = OdDbGripOperations::kWarmGrip;
                break;
              case eGripOpGetNewGripPoints :
                bGetNew = true;
                idEntityToUpdate = pGrip->entityId();
                break;
              default:
                // no op
                break;
              }
            }
          } // else // of if (pGrip->data()->triggerGrip())
        }

        pGrip->setStatus( eNew );
      }
    }

    if ( bGetNew )
    {
      updateEntityGrips( idEntityToUpdate );
      //return true;
    }
  //////////////////////////////////////////////////////////
  } // of else of if (bShift)
  return true;
}

void OdExGripManager::addToDrag(OdExGripDataPtrArray& aActiveKeys)
{
  OdUInt32 i, iSize = aActiveKeys.size();
  for (i = 0; i < iSize; i++)
    aActiveKeys[i]->setStatus(OdDbGripOperations::kDragImageGrip);

  GripDataMap::const_iterator it = m_aGripData.begin();
  while (it != m_aGripData.end())
  {
    bool bActive = false;
    OdExGripDragPtr pDrag;
    {
      const OdExGripDataPtrArray& aData = it->second.m_pDataArray;
      OdUInt32 i, iSize = aData.size();
      for (i = 0; i < iSize; i++)
      {
        if (OdDbGripOperations::kDragImageGrip == aData[i]->status())
        {
          bActive = true;
          pDrag = OdExGripDrag::createObject(it->first, this);
          break;
        }
      }
      for (i = 0; (i < it->second.m_pDataSub.size()) && !bActive; i++)
      {
        const OdExGripDataPtrArray& aData = it->second.m_pDataSub.at(i).m_pSubData;
        OdUInt32 j, iSize = aData.size();
        for (j = 0; j < iSize; j++)
        {
          if (OdDbGripOperations::kDragImageGrip == aData[j]->status())
          {
            bActive = true;
            pDrag = OdExGripDrag::createObject(it->second.m_pDataSub.at(i).m_entPath, this);
            break;
          }
        }
      }
    }
    if (bActive)
    {
      //pDrag->entityId = it->first;
      //pDrag->m_pOwner = this;
      m_aDrags.push_back(pDrag);
    }
    it++;
  }

  iSize = m_aDrags.size();
  for (i = 0; i < iSize; i++)
  {
    m_aDrags[i]->notifyDragStarted();
    m_aDrags[i]->cloneEntity();
  }
}

bool OdExGripManager::onMouseDown(int x, int y, bool bShift)
{
  if (!OdBaseGripManager::onMouseDown(x, y, bShift))
    return false;
  if (bShift)
    return true;

  OdExGripDataPtrArray aKeys;
  locateGripsAt(x, y, aKeys);
  if (aKeys.empty())
    return true;

  //////////////////////////////////////////////////////////
  OdExGripDataPtrArray aActiveKeys;
  locateGripsByStatus( OdDbGripOperations::kHotGrip, aActiveKeys );
  if ( aActiveKeys.empty() )
  {
    // Valid situation.
    // If trigger grip performed entity modification and returned eGripHotToWarm
    // then nothing is to be done cause entity modification will cause reactor to regen grips.
    return false;
  }

  if (handleMappedRtClk(aActiveKeys, x, y))
    return true;

  addToDrag(aActiveKeys);
  
  //////////////////////////////////////////////////////////
  m_ptBasePoint = aKeys.first()->point();
  m_ptLastPoint = m_ptBasePoint;
  {
    // Use alternative point if needed.
    OdDbGripDataPtr pFirstData = aKeys.first()->data();
    if ( 0 != pFirstData.get() )
    {
      if ( 0 != pFirstData->alternateBasePoint() )
      {
        m_ptBasePoint = *(pFirstData->alternateBasePoint());
      }
    }
  }
  m_pCmdCtx->database()->startUndoRecord();
  ::odedRegCmds()->executeCommand(&m_gripStretchCommand, m_pCmdCtx);
  
  OdUInt32 i, iSize = aActiveKeys.size();
  for( i = 0; i < iSize; i++ )
    aActiveKeys[ i ]->setStatus( OdDbGripOperations::kWarmGrip );
  //////////////////////////////////////////////////////////
  //  } // of else of if ( bShift )
  //}
  //else
  //{
  //  return false;
  //}

  return true;
}

OdResult OdBaseGripManager::startHover(int x, int y, bool bShift)
{
  OdResult eRet = eOk;

  if (!endHover())
  {
    eRet = eGripOpFailure;
    clockStartHover = 0;
  }

  OdExGripDataPtrArray aKeys;
  locateGripsAt( x, y, aKeys );
  if (!aKeys.empty())
  {
    m_aHoverGrips = aKeys;

    OdUInt32 i, iSize = m_aHoverGrips.size();
    for( i = 0; i < iSize; i++ )
    {
      OdExGripDataPtr pGrip = m_aHoverGrips[ i ];
      if ( pGrip->status() == OdDbGripOperations::kWarmGrip )
      {
        pGrip->setStatus( OdDbGripOperations::kHoverGrip );

        if ( !pGrip->data().isNull() )
        {
          if ((0 != pGrip->data()->hoverFunc()) && !bShift)
          {
            if (!clockStartHover)
              clockStartHover = clock();

            // 300 ms delay before hoverFunc() 
            if ((clock() - clockStartHover) * 1000 / CLOCKS_PER_SEC > 300)
            {
              int iFlags = 0;
              if (pGrip->isShared())
                iFlags = OdDbGripOperations::kSharedGrip;
              eRet = (*pGrip->data()->hoverFunc())(pGrip->data(), pGrip->entityId(), iFlags);
              if (eRet == eGripOpGetNewGripPoints)
              {
                clockStartHover = 0;
                aKeys[i]->setStatus(OdDbGripOperations::kHotGrip);

                m_ptBasePoint = aKeys.first()->point();
                m_ptLastPoint = m_ptBasePoint;
                {
                  // Use alternative point if needed.
                  OdDbGripDataPtr pFirstData = aKeys.first()->data();
                  if (0 != pFirstData.get())
                  {
                    if (0 != pFirstData->alternateBasePoint())
                    {
                      m_ptBasePoint = *(pFirstData->alternateBasePoint());
                    }
                  }
                }
              }
            }
          }
        }
        onModified(pGrip);
      }
    }
  }
  return eRet;
}

bool OdBaseGripManager::endHover()
{
  if ( m_aHoverGrips.empty() )
    return( false );

  OdUInt32 i, iSize = m_aHoverGrips.size();
  for( i = 0; i < iSize; i++ )
  {
    OdExGripDataPtr pGrip = m_aHoverGrips[ i ];
    if ( pGrip->status() == OdDbGripOperations::kHoverGrip )
    {
      pGrip->setStatus( OdDbGripOperations::kWarmGrip );
      onModified(pGrip);
    }
  }
  m_aHoverGrips.clear();
  return( true );
}

bool OdExGripManager::onMouseMove(int x, int y, bool bShift)
{
  // restart hover operation
  OdResult eRet = startHover(x, y, bShift);
  if (eRet == eGripOpFailure)
    return false;
  else if (eRet == eGripOpGetNewGripPoints)
  {
    OdExGripDataPtrArray aActiveKeys;
    locateGripsByStatus(OdDbGripOperations::kHotGrip, aActiveKeys);
    if (aActiveKeys.empty())
    {
      // Valid situation.
      // If trigger grip performed entity modification and returned eGripHotToWarm
      // then nothing is to be done cause entity modification will cause reactor to regen grips.
      return false;
    }

    addToDrag(aActiveKeys);

    m_pCmdCtx->database()->startUndoRecord();
    ::odedRegCmds()->executeCommand(&m_gripStretchCommand, m_pCmdCtx);

    int iSize = aActiveKeys.size();
    for (int i = 0; i < iSize; i++)
      aActiveKeys[i]->setStatus(OdDbGripOperations::kWarmGrip);
  }
  
  return true;
}

bool OdExGripManager::onControlClick()
{
  if ( m_aDrags.empty() )
    return false;

  // TODO: Notify active grips.

  // AEC grips use CTRL key to change mode,
  // but how to pass it throw standard interface is currently unknown.

  return true;
}

void OdBaseGripManager::selectionSetChanged(OdSelectionSet* pSSet)
{
  bool bRestoreOld = false;
  if ( pSSet->numEntities() > (unsigned)m_GRIPOBJLIMIT )
  {
    disable(true);
  }
  else
  {
    if ( isDisabled() )
    {
      bRestoreOld = true;
    }
    disable(false);
  }

  OdDbDatabase* pDb = OdDbDatabase::cast(pSSet->baseDatabase()).get();
  // Old Entities.
  {
    OdDbStubPtrArray aOld;
    GripDataMap::iterator it = m_aGripData.begin();
    while( it != m_aGripData.end() )
    {
      if ( isDisabled() )
      {
        aOld.push_back( it->first );
      }
      else
      {
        if (!pSSet->isMember(it->first))
        {
          aOld.push_back( it->first );
        }
        else
        {
          // Remove if subentities changed
          bool bRemoved = false;
          OdUInt32 se;
          for (se = 0; se < it->second.m_pDataSub.size(); se++)
          {
            if (!pSSet->isMember(it->second.m_pDataSub[se].m_entPath))
            {
              aOld.push_back( it->first );
              bRemoved = true;
              break;
            }
          }
          // Remove if new paths added also (workaround. tehnically new pathes must be added on second step)
          if (!bRemoved)
          {
            OdSelectionSetIteratorPtr ssIt = searchObjectSSetIterator(pSSet, it->first);
            for (se = 0; se < ssIt->subentCount(); se++)
            {
              OdDbBaseFullSubentPath tmpPath;
              ssIt->getSubentity(se, tmpPath);
              OdUInt32 searchPath = 0;
              bool bFound = false;
              for ( ; searchPath < it->second.m_pDataSub.size(); searchPath++)
              {
                if (it->second.m_pDataSub.at(searchPath).m_entPath == tmpPath)
                {
                  bFound = true;
                  break;
                }
              }
              if (!bFound)
              {
                aOld.push_back( it->first );
                break;
              }
            }
          }
        }
      }
      it++;
    }

    OdUInt32 i, iSize = aOld.size();
    for( i = 0; i < iSize; i++ )
    {
      removeEntityGrips( aOld[ i ], true ); 
      if ((i % GM_PAGE_EACH_OBJECT) && pDb)
        pDb->pageObjects();
    }
  }

  // New Entities.
  {
    OdDbStubPtrArray aNew;
    OdSelectionSetIteratorPtr pIter = pSSet->newIterator();
    while (!pIter->done())
    {
      if (!isDisabled() && m_aGripData.end() == m_aGripData.find(pIter->id()))
      {
        aNew.push_back(pIter->id());
      }

      pIter->next();
    }
    OdUInt32 i, iSize = aNew.size();
    for( i = 0; i < iSize; i++ )
    {
      updateEntityGrips( aNew[ i ] ); 
      if ((i % GM_PAGE_EACH_OBJECT) && pDb)
        pDb->pageObjects();
    }
  }

  updateInvisibleGrips();
}

void OdBaseGripManager::updateEntityGrips(OdDbStub* id)
{
  removeEntityGrips( id, false );

  OdSelectionSetPtr pSSet = workingSelectionSet();
  if ( pSSet.isNull() || !pSSet->isMember( id ) )
    return;

  OdGiDrawablePtr pEntity = openObject(id);
  if ( pEntity.isNull() )
    return;

  OdExGripDataPtrArray aExt;
  OdDbGripDataPtrArray aPts;

  OdSelectionSetIteratorPtr pObjIt = searchObjectSSetIterator(pSSet, id);
  if (pObjIt->subentCount() > 0)
  {
    for (OdUInt32 se = 0; se < pObjIt->subentCount(); se++)
    {
      OdDbBaseFullSubentPath subEntPath;
      pObjIt->getSubentity(se, subEntPath);
      aPts.clear();
      if (getGripPointsAtSubentPath(pEntity, subEntPath, aPts, activeViewUnitSize(), m_GRIPSIZE, activeViewDirection(), 0) == eOk)
      {
        OdUInt32 prevSize = aExt.size();
        aExt.resize(prevSize + aPts.size());
        for (OdUInt32 i = 0; i < aPts.size(); i++)
        {
          aExt[i + prevSize] = OdExGripData::createObject(
            subEntPath, aPts[ i ], aPts[ i ]->gripPoint(), this);
        }
      }
    }
  }
  else
  {
    if ( eOk == getGripPoints(pEntity,
      aPts, activeViewUnitSize(), m_GRIPSIZE, activeViewDirection(), 0 ) )
    {
      aExt.resize( aPts.size() );
      OdUInt32 i, iSize = aExt.size();
      for( i = 0; i < iSize; i++ )
      {
        aExt[ i ] = OdExGripData::createObject(
          id, aPts[ i ], aPts[ i ]->gripPoint(), this);
      }
    }
    else
    {
      OdGePoint3dArray aOldPts;
      if ( eOk == getGripPoints(pEntity, aOldPts) )
      {
        aExt.resize( aOldPts.size() );
        OdUInt32 i, iSize = aExt.size();
        for( i = 0; i < iSize; i++ )
        {
          aExt[ i ] = OdExGripData::createObject(
            id, 0, aOldPts[ i ], this);
        }
      }
    }
  }

  bool bModel = isModel(pEntity);
  if ( !aExt.empty() )
  {
    OdUInt32 i, iSize = aExt.size();
    OdExGripDataExt dExt;
    for (i = 0; i < iSize; i++)
    {
      OdDbBaseFullSubentPath entPath;
      if (aExt[i]->entPath(&entPath))
      {
        bool bFound = false;
        for (OdUInt32 j = 0; j < dExt.m_pDataSub.size(); j++)
        {
          if (dExt.m_pDataSub[j].m_entPath == entPath)
          {
            bFound = true;
            dExt.m_pDataSub[j].m_pSubData.append(aExt[i]);
            break;
          }
        }
        if (!bFound)
        {
          OdExGripDataSubent se;
          se.m_entPath = entPath;
          se.m_pSubData.append(aExt[i]);
          dExt.m_pDataSub.append(se);
        }
      }
      else
      {
        dExt.m_pDataArray.append(aExt[i]);
      }
    }

    //dExt.m_pDataArray = aExt;
    m_aGripData.insert( std::make_pair( id, dExt ) );

    for( i = 0; i < iSize; i++ )
      showGrip( aExt[ i ], bModel );
  }
}

void OdBaseGripManager::removeEntityGrips(OdDbStub* id, bool bFireDone)
{
  GripDataMap::iterator it = m_aGripData.find( id );
  if ( it != m_aGripData.end() )
  {
    OdGiDrawablePtr pEntity = openObject(id);
    if ( pEntity.get() )
      gripStatus(pEntity, OdDb::kGripsToBeDeleted);

    bool bModel = isModel(pEntity);
    OdUInt32 i, iSize = it->second.m_pDataArray.size();
    for ( i = 0; i < iSize; i++ )
    {
      OdExGripDataPtr pData = it->second.m_pDataArray[ i ];
      hideGrip( pData, bModel );
      if (!it->second.m_pDataArray[i]->data().isNull() && it->second.m_pDataArray[i]->data()->gripOpStatFunc())
        (*it->second.m_pDataArray[i]->data()->gripOpStatFunc())(it->second.m_pDataArray[i]->data(), id, OdDbGripOperations::kGripEnd);
      it->second.m_pDataArray[i] = 0;
    }
    for (i = 0; i < it->second.m_pDataSub.size(); i++)
    {
      for (OdUInt32 j = 0; j < it->second.m_pDataSub.at(i).m_pSubData.size(); j++)
      {
        OdExGripDataPtr pData = it->second.m_pDataSub.at(i).m_pSubData[j];
        hideGrip( pData, bModel );
        it->second.m_pDataSub.at(i).m_pSubData[j] = 0;
      }
    }

    if ( bFireDone )
    {
      if (pEntity.get())
        gripStatus(pEntity, OdDb::kGripsDone);
    }
    m_aGripData.erase( it );
  }
}

void OdBaseGripManager::locateGripsAt( int x, int y, OdExGripDataPtrArray& aResult )
{
  aResult.clear();

  double dX = x;
  double dY = y;

  OdGePoint3d ptFirst;
  GripDataMap::const_iterator it = m_aGripData.begin();
  while( it != m_aGripData.end() )
  {
    for (OdUInt32 se = 0; se < it->second.m_pDataSub.size() + 1; se++)
    {
      const OdExGripDataPtrArray& aData = (se == 0) ? it->second.m_pDataArray : it->second.m_pDataSub[se - 1].m_pSubData;

      OdUInt32 i, iSize = aData.size();
      for ( i = 0; i < iSize; i++ )
      {
        const OdGePoint3d& ptCurrent = aData[ i ]->point();

        if ( aResult.empty() )
        {
          // First grip is obtained by comparing
          // grip point device position with cursor position.
          OdGePoint3d ptDC = ptCurrent;
          ptDC.transformBy( activeGsView()->worldToDeviceMatrix() );

          double dDeltaX = ::fabs( dX - ptDC.x );
          double dDeltaY = ::fabs( dY - ptDC.y );
          bool bOk = (dDeltaX <= m_GRIPSIZE) && (dDeltaY <= m_GRIPSIZE);
          if ( bOk )
          {
            ptFirst = ptCurrent;
            aResult.push_back( aData[ i ] );
          }
        }
        else
        {
          // Other grips are obtained by comparing world coordinates.
          // The approach here is quite raw.
          if ( ptCurrent.isEqualTo( ptFirst, 1E-4 ) )
          {
            aResult.push_back( aData[ i ] );
          }
        }
      }
    }
    it++;
  }
}

void OdBaseGripManager::locateGripsByStatus( OdDbGripOperations::DrawType eStatus, OdExGripDataPtrArray& aResult )
{
    aResult.clear();

    GripDataMap::const_iterator it = m_aGripData.begin();
    while( it != m_aGripData.end() )
    {
      for (OdUInt32 se = 0; se < it->second.m_pDataSub.size() + 1; se++)
      {
        const OdExGripDataPtrArray& aData = (se == 0) ? it->second.m_pDataArray : it->second.m_pDataSub[se - 1].m_pSubData;
        OdUInt32 i, iSize = aData.size();
        for ( i = 0; i < iSize; i++ )
        {
            if ( eStatus == aData[ i ]->status() )
            {
                aResult.push_back( aData[ i ] );
            }
        }
      }
      it++;
    }
}

namespace
{

struct SortGripsAlongXAxis
{
  bool operator()( const OdExGripDataPtr& grA, const OdExGripDataPtr& grB )
  {
    return OdPositive( grA->point().x, grB->point().x );
  }
};

}

void OdBaseGripManager::updateInvisibleGrips()
{
  OdExGripDataPtrArray aOverall;
  GripDataMap::const_iterator it = m_aGripData.begin();
  while( it != m_aGripData.end() )
  {
    aOverall.insert( aOverall.end(), it->second.m_pDataArray.begin(), it->second.m_pDataArray.end() );
    for (OdUInt32 i = 0; i < it->second.m_pDataSub.size(); i++)
    {
      aOverall.insert(aOverall.end(), it->second.m_pDataSub[i].m_pSubData.begin(), it->second.m_pDataSub[i].m_pSubData.end());
    }
    it++;
  }

  OdUInt32 i, iSize = aOverall.size();
  for( i = 0; i < iSize; i++ )
  {
    aOverall[ i ]->setInvisible(false);
    aOverall[ i ]->setShared(false);
  }

  // Not the best approach for sorting.
  // Just for demonstration.
  std::sort( aOverall.begin(), aOverall.end(), SortGripsAlongXAxis() );

  iSize = aOverall.size();
  for ( i = 0; i < iSize; i++ )
  {
    if ( aOverall[ i ]->isShared() )
      continue;

    OdUInt32Array aEq;
    aEq.push_back( i );

    OdGePoint3d ptIni = aOverall[ i ]->point();

    OdUInt32 iNext = i + 1;
    while( iNext < iSize )
    {
      OdGePoint3d ptCur = aOverall[ iNext ]->point();

      if ( OdEqual( ptIni.x, ptCur.x, 1E-6 ) )
      {
        if ( ptIni.isEqualTo( ptCur, 1E-6 ) )
        {
          aEq.push_back( iNext );
        }
        iNext++;
      }
      else
      {
        break;
      }
    }

    if ( aEq.size() >= 2 )
    {
      OdUInt32 iVisible = 0;
      OdUInt32 j, jSize = aEq.size();
      for ( j = 0; j < jSize; j++ )
      {
        OdExGripDataPtr pGrip = aOverall[ aEq[ j ] ];

        bool bOk = true;
        if ( !pGrip->data().isNull() )
        {
          if ( pGrip->data()->skipWhenShared() )
            bOk = false;
        }
        else
        {
          bOk = false;
        }

        if ( bOk )
        {
          iVisible = j;
          break;
        }
      }

      for ( j = 0; j < jSize; j++ )
      {
        OdExGripDataPtr pGrip = aOverall[ aEq[ j ] ];

        pGrip->setShared(true);
        pGrip->setInvisible(j != iVisible);
      }
    }
  }
}

void OdExGripManager::showGrip( OdExGripData* pGrip, bool /*bModel*/ )
{
  OdGsPaperLayoutHelperPtr pPaperHelper = OdGsPaperLayoutHelper::cast(m_pDevice);
  OdUInt32 i, iSize = m_pDevice->numViews();
  if(pPaperHelper.get())
  {
    //for( i = 0; i < iSize; i++ )
    //  if(bModel==(pPaperHelper->viewAt(i) != pPaperHelper->overallView()))
    //    pPaperHelper->viewAt( i )->add( pGrip, m_pGsModel );
    OdDbObjectPtr pVpObj = m_pCmdCtx->database()->activeViewportId().openObject();
    OdDbAbstractViewportDataPtr pAVD = OdDbAbstractViewportDataPtr(pVpObj);
    if (!pAVD.isNull() && pAVD->gsView(pVpObj))
      pAVD->gsView(pVpObj)->add( pGrip, m_pGsModel );
    else
      pPaperHelper->overallView()->add( pGrip, m_pGsModel );
  }
  else
  {
    for( i = 0; i < iSize; i++ )
      m_pDevice->viewAt( i )->add( pGrip, m_pGsModel );
  }
}

void OdExGripManager::hideGrip( OdExGripData* pGrip, bool /*bModel*/ )
{
  OdGsPaperLayoutHelperPtr pPaperHelper = OdGsPaperLayoutHelper::cast(m_pDevice);
  OdUInt32 i, iSize = m_pDevice->numViews();
  if(pPaperHelper.get())
  {
    for( i = 0; i < iSize; i++ )
      m_pDevice->viewAt( i )->erase( pGrip );
  }
  else
  {
    for( i = 0; i < iSize; i++ )
      m_pDevice->viewAt( i )->erase( pGrip );
  }
}

void OdBaseGripManager::setValue( const OdGePoint3d& ptValue )
{
  OdUInt32 i, iSize = m_aDrags.size();
  OdGePoint3d newPoint = eyeToUcsPlane(ptValue, m_ptBasePoint);
  for ( i = 0; i < iSize; i++ )
    m_aDrags[ i ]->cloneEntity( newPoint );
  m_ptLastPoint = newPoint;
}

int OdExGripManager::addDrawables( OdGsView* pView )
{
  ODA_ASSERT(pView->device()==m_pDevice->underlyingDevice().get());

  OdUInt32 i, iSize = m_aDrags.size();
  for ( i = 0; i < iSize; i++ )
    pView->add( m_aDrags[ i ].get(), /*m_pGsModel*/ 0 );
  return iSize;
}

void OdExGripManager::removeDrawables( OdGsView* pView )
{
  OdUInt32 i, iSize = m_aDrags.size();
  for ( i = 0; i < iSize; i++ )
    pView->erase( m_aDrags[ i ].get() );
}

inline void resetDragging(OdGsDevice *pDevice, bool bOp)
{
  if (!pDevice)
    return;
  OdRxDictionaryPtr pProps = pDevice->properties();
  if (pProps.isNull())
    return;
  if (!pProps->has(OD_T("DrawDragging")))
    return;
  pProps->putAt(OD_T("DrawDragging"), OdRxVariantValue(bOp));
}
void OdExGripManager::draggingStarted()
{
  ::resetDragging(m_pDevice, true);
}
void OdExGripManager::draggingStopped()
{
  ::resetDragging(m_pDevice, false);
}

OdSelectionSetPtr OdExGripManager::workingSelectionSet() const
{
  if (m_pGetSelectionSetPtr)
  {
    return OdSelectionSet::cast(m_pGetSelectionSetPtr(m_pCmdCtx));
  }
  return OdSelectionSetPtr();
}

OdGsView* OdExGripManager::activeGsView() const
{
  return m_pDevice->activeView();
}

OdDbStub* OdBaseGripManager::activeVpId() const
{
  OdGsClientViewInfo vi;
  activeGsView()->clientViewInfo(vi);
  return vi.viewportObjectId;
}

double OdBaseGripManager::activeViewUnitSize() const
{
  OdGsView* pView = activeGsView();

  // Do not have access to getNumPixelsInUnitSquare here.
  double dRes;
  {
    OdGePoint2d ptDim; // getNumPixelsInUnitSquare
    {
      OdGePoint2d ll, ur;
      pView->getViewport(ll, ur);

      OdGsDCRect scrRect;
      pView->getViewport( scrRect );
      ptDim.x = fabs(double(scrRect.m_max.x - scrRect.m_min.x) / pView->fieldWidth()  * (ur.x-ll.x));
      ptDim.y = fabs(double(scrRect.m_max.y - scrRect.m_min.y) / pView->fieldHeight() * (ur.y-ll.y));
    }
    OdGeVector3d v( m_GRIPSIZE / ptDim.x, 0, 0 );
    v.transformBy( pView->viewingMatrix() );
    dRes = v.length() / m_GRIPSIZE;
  }
  return dRes;
}

OdGeVector3d OdBaseGripManager::activeViewDirection() const
{
  OdGsView* pView = activeGsView();
  return (pView->position() - pView->target()).normal();
}

OdGePoint3d OdExGripManager::eyeToUcsPlane(const OdGePoint3d &pPoint, const OdGePoint3d &pBasePoint) const
{
  // #8043
  OdDbObjectPtr pVpObj = OdDbObjectId(activeVpId()).safeOpenObject();
  OdDbAbstractViewportDataPtr pAVD(pVpObj);
  OdGePoint3d ucsOrigin;
  OdGeVector3d ucsXAxis, ucsYAxis;
  pAVD->getUcs(pVpObj, ucsOrigin, ucsXAxis, ucsYAxis);
  OdGePlane plane(/*ucsOrigin*/pBasePoint, ucsXAxis, ucsYAxis);
  OdGeLine3d line(pPoint, activeViewDirection());
  OdGePoint3d newPoint;
  if (!plane.intersectWith(line, newPoint))
  {
    line.set(pPoint, ucsXAxis.crossProduct(ucsYAxis));
    if (!plane.intersectWith(line, newPoint)) // #7727
      newPoint = pBasePoint;
  }
  return newPoint;
}

bool OdExGripManager::handleMappedRtClk( OdExGripDataPtrArray &aActiveKeys, int x, int y )
{
#if !defined(ODA_UNIXOS) && !(defined(ODA_WINDOWS) && !defined(OD_WINDOWS_DESKTOP))
  OdUInt32 iSize = aActiveKeys.size();
  int rtClkIndex = -1;
  for (OdUInt32 i = 0; i < iSize; i++)
  {
    if (!aActiveKeys[i]->data().isNull() && 0 != aActiveKeys[i]->data()->rtClk()
      && aActiveKeys[i]->data()->mapGripHotToRtClk() && !aActiveKeys[i]->isShared())
    {
      rtClkIndex = i;
      break;
    }
  }
  if (rtClkIndex != -1)
  {
    OdDbStubPtrArray ents;
    OdDbGripDataArray hotGrips;
    for (OdUInt32 i = 0; i < iSize; i++ )
    {
      hotGrips.append(*aActiveKeys[i]->data());
      if (!ents.contains(aActiveKeys[i]->entityId()))
        ents.append(aActiveKeys[i]->entityId());
    }
    OdString menuName;
    ODHMENU menu = 0;
    ContextMenuItemIndexPtr cb = 0;
    OdResult eRet = (*aActiveKeys[rtClkIndex]->data()->rtClk())( hotGrips, ents, menuName, menu, cb);
    if (eRet == eOk && menu != 0 && cb != 0)
    {
      HWND wnd = ::GetActiveWindow();
      POINT pt = { x, y };
      ::ClientToScreen(wnd, &pt);
      (*cb)(::TrackPopupMenu((HMENU)menu,
        TPM_LEFTALIGN | TPM_TOPALIGN | TPM_NONOTIFY | TPM_RETURNCMD | TPM_LEFTBUTTON | TPM_NOANIMATION, pt.x, pt.y, 0, wnd, 0));
      ::DestroyMenu((HMENU)menu);
      for (OdUInt32 i = 0; i < iSize; i++)
        aActiveKeys[i]->setStatus(OdDbGripOperations::kWarmGrip);
      /*for (OdUInt32 i = 0; i < iSize; i++)
      {
        if (aActiveKeys[i]->data()->hotAfterMenuChoose())
          aActiveKeys[i]->setStatus(OdDbGripOperations::kHotGrip);
        else
          aActiveKeys[ i ]->setStatus( OdDbGripOperations::kWarmGrip );
      }
      if (aActiveKeys[rtClkIndex]->data()->hotAfterMenuChoose())
        return false;*/

      updateEntityGrips(aActiveKeys[rtClkIndex]->entityId());
      return true;
    }
  }
#endif // ODA_UNIXOS
  return false;
}

void OdBaseGripManager::disable(bool bDisable)
{
  m_bDisabled = bDisable;
}
void OdExGripManager::disable(bool bDisable)
{
  if (m_bDisabled != bDisable)
  {
    OdDbDatabase* pDb = m_pCmdCtx->database();
    m_bDisabled = bDisable;
    if (bDisable)
      pDb->removeReactor( &m_cDbReactor );
    else
      pDb->addReactor( &m_cDbReactor );
  }
}

OdGiDrawablePtr OdExGripManager::openObject(OdDbStub* id, 
                                            bool isForWriteMode) // = false
{
  OdGiDrawablePtr pObj;
  if (!id)
    return pObj;
  pObj = OdGiDrawable::cast(OdDbObjectId(id).openObject(isForWriteMode ? OdDb::kForWrite : OdDb::kForRead));
  return pObj;
}

OdResult OdExGripManager::getGripPointsAtSubentPath(OdGiDrawable* pEntity, const OdDbBaseFullSubentPath& path, 
                                                    OdDbGripDataPtrArray& grips, double curViewUnitSize, 
                                                    int gripSize, const OdGeVector3d& curViewDir, const OdUInt32 bitflags) const
{
  return OdDbEntity::cast(pEntity)->getGripPointsAtSubentPath(*((const OdDbFullSubentPath*) &path), 
                                                              grips, curViewUnitSize, 
                                                              gripSize, curViewDir, bitflags);
}

OdResult OdExGripManager::getGripPoints(OdGiDrawable* pEntity, OdDbGripDataPtrArray& grips, double curViewUnitSize, 
                                        int gripSize, const OdGeVector3d& curViewDir, int bitFlags) const
{
  OdDbEntity* pEnt = OdDbEntity::cast(pEntity);
  if (!pEnt)
    return eNotApplicable;
  return pEnt->getGripPoints(grips, curViewUnitSize, gripSize, curViewDir, bitFlags);
}

OdResult OdExGripManager::getGripPoints(OdGiDrawable* pEntity, OdGePoint3dArray& gripPoints) const
{
  OdDbEntity* pEnt = OdDbEntity::cast(pEntity);
  if (!pEnt)
    return eNotApplicable;
  return pEnt->getGripPoints(gripPoints);
}

OdResult OdExGripManager::moveGripPointsAtSubentPaths(OdGiDrawable* pEntity, const OdDbBaseFullSubentPathArray& paths, 
                                                      const OdDbVoidPtrArray& gripAppData, const OdGeVector3d& offset,
                                                      OdUInt32 bitflags)
{
  ODA_ASSERT_ONCE(sizeof(OdDbFullSubentPath) == sizeof(OdDbBaseFullSubentPath));
  return OdDbEntity::cast(pEntity)->moveGripPointsAtSubentPaths(*((const OdDbFullSubentPathArray*) &paths), 
                                                                gripAppData, offset, bitflags);
}

OdResult OdExGripManager::moveGripPointsAt(OdGiDrawable* pEntity, const OdDbVoidPtrArray& gripAppData, const OdGeVector3d& offset, int bitFlags)
{
  return OdDbEntity::cast(pEntity)->moveGripPointsAt(gripAppData, offset, bitFlags);
}

OdResult OdExGripManager::moveGripPointsAt(OdGiDrawable* pEntity, const OdIntArray& indices, const OdGeVector3d& offset)
{
  return OdDbEntity::cast(pEntity)->moveGripPointsAt(indices, offset);
}

void OdExGripManager::subentGripStatus(OdGiDrawable* pEntity, OdDb::GripStat status, const OdDbBaseFullSubentPath& subentity)
{
  OdDbEntity::cast(pEntity)->subentGripStatus(status, *((const OdDbFullSubentPath*) &subentity));
}

void OdExGripManager::gripStatus(OdGiDrawable* pEntity, OdDb::GripStat st)
{
  OdDbEntity::cast(pEntity)->gripStatus(st);
}

void OdExGripManager::dragStatus(OdGiDrawable* pEntity, OdDb::DragStat st)
{
  OdDbEntity::cast(pEntity)->dragStatus(st);
}

bool OdExGripManager::isModel(OdGiDrawable* pEntity)
{
	OdDbEntity* pEnt = OdDbEntity::cast(pEntity);
	return !pEnt || pEnt->database()->getTILEMODE();
}

OdExGripDbReactor::OdExGripDbReactor()
  : m_pOwner(0)
{
}

void OdExGripDbReactor::objectAppended( const OdDbDatabase* , const OdDbObject* )
{
  // New object.
}

void OdExGripDbReactor::objectModified( const OdDbDatabase*, const OdDbObject* pDbObj )
{
  m_pOwner->updateEntityGrips( pDbObj->objectId() );
  m_pOwner->updateInvisibleGrips();
}

void OdExGripDbReactor::objectErased( const OdDbDatabase*, const OdDbObject* pDbObj, bool pErased )
{
  if ( pErased )
  {
    m_pOwner->removeEntityGrips( pDbObj->objectId(), true );
    m_pOwner->updateInvisibleGrips();
  }
}

#undef GM_PAGE_EACH_OBJECT
