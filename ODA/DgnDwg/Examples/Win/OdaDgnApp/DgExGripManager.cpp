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
#include "Ge/GePlane.h"
#include "Ge/GeLine3d.h"
#include "Gi/GiDrawableImpl.h"
#include "Gi/GiWorldDraw.h"
#include "Gi/GiViewportDraw.h"
#include "DgHostAppServices.h"
#include "DgCommandContext.h"
#include "DgElement.h"
//#include "DbAbstractViewportData.h"
#include "DgExGripManager.h"
#include "Gs/GsModel.h"
#include "RxObjectImpl.h"

// Menu animation flags
#ifndef TPM_VERPOSANIMATION
static const UINT TPM_VERPOSANIMATION = 0x1000L;
#endif
#ifndef TPM_NOANIMATION
static const UINT TPM_NOANIMATION = 0x4000L;
#endif
//

namespace
{

static OdDgSelectionSetIteratorPtr searchObjectSSetIterator(OdDgSelectionSetPtr pSSet, const OdDgElementId& id)
{
  OdDgSelectionSetIteratorPtr pIter = pSSet->newIterator();
  while (!pIter->done())
  {
    if (pIter->objectId() == id)
    {
      return pIter;
    }
    pIter->next();
  }
  return OdDgSelectionSetIteratorPtr();
}

static OdDgExGripManager::OdDgExGripDataSubent &getSubentGripData(OdDgExGripManager::OdDgExGripDataExt &ext, OdDgFullSubentPath entPath)
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

OdDgExGripDragPtr OdDgExGripDrag::createObject(OdDgElementId id, OdDgExGripManager* pOwner)
{
  OdDgExGripDragPtr pRes = RXIMPL_CONSTR(OdDgExGripDrag);
  pRes->m_entPath = OdDgFullSubentPath();
  pRes->m_entPath.objectIds().append(id);
  pRes->m_pOwner = pOwner;
  return pRes;
}

OdDgExGripDragPtr OdDgExGripDrag::createObject(OdDgFullSubentPath entPath, OdDgExGripManager* pOwner)
{
  OdDgExGripDragPtr pRes = RXIMPL_CONSTR(OdDgExGripDrag);
  pRes->m_entPath = entPath;
  pRes->m_pOwner = pOwner;
  return pRes;
}

OdDgExGripDrag::OdDgExGripDrag()
{
  m_entPath = OdDgFullSubentPath();
  m_pClone = 0;
  m_pOwner = 0;
}

OdDgExGripDrag::~OdDgExGripDrag()
{
}

bool OdDgExGripDrag::locateActiveGrips( OdIntArray& aIndices )
{
  const OdDgExGripDataPtrArray& rData = (entPath()) ? getSubentGripData(m_pOwner->m_aGripData[entityId()], m_entPath).m_pSubData
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
  ODA_ASSERT( !aIndices.empty() );
  return bExMethod;
}

void OdDgExGripDrag::cloneEntity()
{
  m_pClone = 0;

  OdDgElementPtr pEntity = OdDgElement::cast( entityId().openObject() );
  if ( pEntity.isNull() || 0 == m_pOwner )
  {
    return;
  }

  if( pEntity->cloneMeForDragging() )
  {
    m_pClone = OdDgElement::cast( pEntity->clone() );
  }

  if( false == m_pClone.isNull() )
  {
    m_pClone->setPropertiesFrom( pEntity.get(), false );
  }
}

void OdDgExGripDrag::cloneEntity( const OdGePoint3d& ptMoveAt )
{
  cloneEntity();

  if ( m_pClone.isNull() )
    return;

  OdIntArray aIndices;
  bool bExMethod = locateActiveGrips( aIndices );

  OdGeVector3d vOffset =
    ptMoveAt - m_pOwner->m_ptBasePoint;

  if ( bExMethod )
  {
    OdDgGripDataPtrArray aCloneData;

//     if( entPath() )
//     {
//       m_pClone->getGripPointsAtSubentPath(m_entPath, aCloneData,
//       m_pOwner->activeViewUnitSize(), m_pOwner->m_GRIPSIZE, m_pOwner->activeViewDirection(), 0 );
//     }
//     else
    {
      m_pClone->getGripPoints( aCloneData,
        m_pOwner->activeViewUnitSize(), m_pOwner->m_GRIPSIZE, m_pOwner->activeViewDirection(), 0 );
    }

    OdDgVoidPtrArray aIds;
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

//     if (entPath())
//     {
//       OdDgFullSubentPathArray aPaths;
//       aPaths.append(m_entPath);
//       m_pClone->moveGripPointsAtSubentPaths(aPaths, aIds, vOffset, 0 );
//       m_pClone->subentGripStatus(OdDb::kGripsToBeDeleted, m_entPath);
//     }
//     else
    {
      m_pClone->moveGripPointsAt( aIds, vOffset, 0 );
      m_pClone->gripStatus( OdDgElement::kGripsToBeDeleted );
      //m_pClone->gripStatus( OdDb::kDimDataToBeDeleted );
    }
    for (i = 0; i < aCloneData.size(); ++i)
    {
      if (aCloneData[i]->gripOpStatFunc())
      {
        (aCloneData[i]->gripOpStatFunc())(aCloneData[i], OdDgElementId::kNull, OdDbGripOperations::kGripEnd);
      }
    }
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

    m_pClone->moveGripPointsAt( aIndices, vOffset );
    m_pClone->gripStatus( OdDgElement::kGripsToBeDeleted );
  }
  if (m_pOwner->m_pGsModel)
    m_pOwner->m_pGsModel->onModified(this, (OdGiDrawable*)0);
  else
    m_pOwner->m_pDevice->invalidate();
}

void OdDgExGripDrag::moveEntity( const OdGePoint3d& ptMoveAt )
{
  OdIntArray aIndices;
  bool bExMethod = locateActiveGrips( aIndices );

  OdGeVector3d vOffset =
    ptMoveAt - m_pOwner->m_ptBasePoint;

  OdDgElementPtr pEntity =
    OdDgElement::cast( entityId().openObject( OdDg::kForWrite ) );
  ODA_ASSERT( !pEntity.isNull() );

  const OdDgExGripDataPtrArray& rData =
//     (entPath()) ?
//     getSubentGripData(m_pOwner->m_aGripData[entityId()], m_entPath).m_pSubData
//     :
    m_pOwner->m_aGripData[ entityId() ].m_pDataArray;

  if ( bExMethod )
  {
    OdDgVoidPtrArray aIds;
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

//     if (entPath())
//     {
//       OdDgFullSubentPathArray aPaths;
//       aPaths.append(m_entPath);
//       pEntity->moveGripPointsAtSubentPaths(aPaths, aIds, vOffset, 0);
//     }
//     else
    {
      pEntity->moveGripPointsAt( aIds, vOffset, 0 );
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

    pEntity->moveGripPointsAt( aIndices, vOffset );
  }
}

void OdDgExGripDrag::notifyDragStarted()
{
  OdDgElementPtr pEntity =
    OdDgElement::cast( entityId().openObject() );
  if ( false == pEntity.isNull() )
    pEntity->dragStatus( OdDgElement::kDragStart );
}

void OdDgExGripDrag::notifyDragEnded()
{
  OdDgElementPtr pEntity =
    OdDgElement::cast( entityId().openObject() );
  if ( false == pEntity.isNull() )
    pEntity->dragStatus( OdDgElement::kDragEnd );
}

void OdDgExGripDrag::notifyDragAborted()
{
  OdDgElementPtr pEntity =
    OdDgElement::cast( entityId().openObject() );
  if ( false == pEntity.isNull() )
    pEntity->dragStatus( OdDgElement::kDragAbort );
}

OdUInt32 OdDgExGripDrag::subSetAttributes( OdGiDrawableTraits* pTraits ) const
{
  if ( m_pClone.isNull() )
  {
    return( kDrawableIsInvisible );
  }
  else
  {
    OdUInt32 iRet = m_pClone->setAttributes( pTraits );

    iRet &= ~kDrawableIsCompoundObject | kDrawableUsesNesting;

    OdGiSubEntityTraitsPtr pEntityTraits =
      OdGiSubEntityTraits::cast( pTraits );
    if ( pEntityTraits.isNull() )
    {
      ODA_ASSERT( 0 );
      return( OdGiDrawable::kDrawableNone );
    }

    pEntityTraits->setFillType( kOdGiFillNever );

    return iRet;
  }
}

bool OdDgExGripDrag::subWorldDraw( OdGiWorldDraw* pWorldDraw ) const
{
  if ( m_pClone.isNull() )
  {
    return true;
  }
  else
  {
    pWorldDraw->geometry().draw(m_pClone);
    return true;
  }
}

void OdDgExGripDrag::subViewportDraw( OdGiViewportDraw* pViewportDraw ) const
{
  if ( false == m_pClone.isNull() )
  {
    m_pClone->viewportDraw( pViewportDraw );
  }
}

OdDgExGripDataPtr OdDgExGripData::createObject(OdDgElementId id, OdDgGripDataPtr pData, const OdGePoint3d& pt,  OdDgExGripManager* pOwner)
{ 
  OdDgExGripDataPtr pRes = RXIMPL_CONSTR(OdDgExGripData);
  pRes->m_entPath = OdDgFullSubentPath();
  pRes->m_entPath.objectIds().append(id);
  pRes->m_pData = pData;
  pRes->m_pOwner = pOwner;
  pRes->m_point = pt;
  return pRes;
}

OdDgExGripDataPtr OdDgExGripData::createObject(OdDgFullSubentPath entPath, OdDgGripDataPtr pData, const OdGePoint3d& pt,  OdDgExGripManager* pOwner)
{ 
  OdDgExGripDataPtr pRes = RXIMPL_CONSTR(OdDgExGripData);
  pRes->m_entPath = entPath;
  pRes->m_pData = pData;
  pRes->m_pOwner = pOwner;
  pRes->m_point = pt;
  return pRes;
}

OdDgExGripData::OdDgExGripData()
{
  m_status = OdDbGripOperations::kWarmGrip;
  m_bInvisible = false;
  m_bShared = false;
  m_point = OdGePoint3d::kOrigin;
  m_entPath = OdDgFullSubentPath();
  m_pData = 0;
  m_pOwner = 0;
}

OdDgExGripData::~OdDgExGripData()
{
  if(m_pData.get() && m_pData->alternateBasePoint())
  {
    delete m_pData->alternateBasePoint();
    m_pData->setAlternateBasePoint(0);
  }
}

bool OdDgExGripData::computeDragPoint( OdGePoint3d& ptOverride ) const
{
    OdGePoint3d ptBase = point();
    if ( !data().isNull() &&
         0 != data()->alternateBasePoint() )
    {
        ptBase = *(data()->alternateBasePoint());
    }

    bool bOverride = false;
    ptOverride = ptBase;

    if (  status() == OdDbGripOperations::kDragImageGrip )
    {
        if ( !data().isNull() )
        {
            if ( data()->drawAtDragImageGripPoint() )
            {
                ptOverride =
                    ptBase + (m_pOwner->m_ptLastPoint - m_pOwner->m_ptBasePoint);
                bOverride = true;
            }
        }
    }
    return bOverride;
}

OdUInt32 OdDgExGripData::subSetAttributes( OdGiDrawableTraits* pTraits ) const
{
  if ( isInvisible() )
    return kDrawableIsInvisible;

  OdGiSubEntityTraitsPtr pEntityTraits = OdGiSubEntityTraits::cast( pTraits );

  if( !pEntityTraits.isNull() )
  {
    switch(  status() )
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
  }

  return kDrawableRegenDraw;
}

bool OdDgExGripData::subWorldDraw( OdGiWorldDraw* pWorldDraw ) const
{
    double dGripSize = m_pOwner->m_GRIPSIZE;
    {
        if ( 0 == pWorldDraw->context() ||
             0 == pWorldDraw->context()->database() )
        {
            dGripSize = m_pOwner->m_GRIPSIZE;
        }
        else
        {
            // Here is the design flaw:
            // ARX help says that grip size passed in callback below should be
            // calculated individually for each viewport.
        }
    }

    if ( !data().isNull() )
    {
        if ( 0 != data()->worldDraw() )
        {
            OdGePoint3d ptComputed;
            OdGePoint3d* pDrawAtDrag = 0;
            if ( computeDragPoint( ptComputed ) )
                pDrawAtDrag = &ptComputed;

            return( (*data()->worldDraw())(
                (OdDbGripData*)data().get(), pWorldDraw, entityId(),  status(), pDrawAtDrag, dGripSize ) );
        }
    }

    return( false );
}

void OdDgExGripData::subViewportDraw( OdGiViewportDraw* pViewportDraw ) const
{
    OdGePoint3d ptComputed;
    OdGePoint3d* pDrawAtDrag = 0;
    if ( computeDragPoint( ptComputed ) )
        pDrawAtDrag = &ptComputed;

    bool bDefault = true;
    if ( !data().isNull() )
    {
        if ( 0 != data()->viewportDraw() )
        {
            (*data()->viewportDraw())(
                (OdDbGripData*)data().get(), pViewportDraw, entityId(),  status(), pDrawAtDrag, m_pOwner->m_GRIPSIZE );
            bDefault = false;
        }
    }

    if ( bDefault )
    {
        if (!m_pOwner->m_pGsModel || m_pOwner->m_pGsModel->renderType() < OdGsModel::kDirect) {
            // Commented since renderTypes implemented, so no need to translate objects for kDirect renderType
            OdGeVector3d vpDirection(pViewportDraw->viewport().viewDir());
            OdGePoint3d vpOrigin(pViewportDraw->viewport().getCameraLocation());
            double ptLength = (ptComputed - vpOrigin).dotProduct(vpDirection);
            ptComputed -= vpDirection * ptLength;
        }

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

        //ptOnScreen.z = 0.;
        
        OdGePoint3d aPoly[ 4 ];
        aPoly[ 0 ].set( ptOnScreen.x - dGripSize, ptOnScreen.y - dGripSize, ptOnScreen.z );
        aPoly[ 1 ].set( ptOnScreen.x + dGripSize, ptOnScreen.y - dGripSize, ptOnScreen.z );
        aPoly[ 2 ].set( ptOnScreen.x + dGripSize, ptOnScreen.y + dGripSize, ptOnScreen.z );
        aPoly[ 3 ].set( ptOnScreen.x - dGripSize, ptOnScreen.y + dGripSize, ptOnScreen.z );
        pViewportDraw->geometry().polygonEye( 4, aPoly );
    }
}


OdDgExGripManager::OdDgExGripManager()
{
  m_pDevice = 0;
  m_pCmdCtx = 0;
  m_aGripData.clear();

  m_aHoverGrips.clear();

  m_ptBasePoint = OdGePoint3d::kOrigin;
  m_ptLastPoint = OdGePoint3d::kOrigin;
  m_aDrags.clear();

  m_cDbReactor.m_pOwner = this;

  m_bDisabled = false;

  m_GRIPSIZE     = 5;
  m_GRIPOBJLIMIT = 100;

  m_pGetSelectionSetPtr = 0;
}

OdDgExGripManager::~OdDgExGripManager()
{
  endHover();
}

void OdDgExGripManager::init( OdGsDevice* pDevice, OdGsModel* pGsModel, OdDgCommandContext* pCmdCtx, GetSelectionSetPtr pGetSSet )
{
  m_pDevice = pDevice;
  m_pGsModel = pGsModel;
  m_pCmdCtx = pCmdCtx;

  OdDgDatabase* pDb = m_pCmdCtx->database();
  pDb->addReactor( &m_cDbReactor );

  OdDgHostAppServices* pAppSvcs = pDb->appServices();
  m_GRIPSIZE     = pAppSvcs->getDG_GRIPSIZE();
  m_GRIPOBJLIMIT = pAppSvcs->getDG_GRIPOBJLIMIT();
  m_GRIPCOLOR.setColorIndex(pAppSvcs->getDG_GRIPCOLOR());
  m_GRIPHOVER.setColorIndex(pAppSvcs->getDG_GRIPHOVER());
  m_GRIPHOT.setColorIndex(pAppSvcs->getDG_GRIPHOT());

  m_pGetSelectionSetPtr = pGetSSet;
}

void OdDgExGripManager::uninit()
{
  if(m_pCmdCtx)
  {
    m_pCmdCtx->database()->removeReactor( &m_cDbReactor );
    m_pCmdCtx = 0;
  }
  m_pDevice = 0;
}

bool OdDgExGripManager::onMouseDown( int x, int y, bool bShift )
{
    endHover();

    OdDgExGripDataPtrArray aKeys;
    locateGripsAt( x, y, aKeys );

    if ( false == aKeys.empty() )
    {
        if ( bShift )
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
                OdDgExGripDataPtr pGrip = aKeys[ i ];
                if ( !pGrip->data().isNull() )
                {
                    if ( pGrip->data()->triggerGrip() )
                    {
                        eCurStatus = OdDbGripOperations::kWarmGrip;
                    }
                    else
                    {
                        if ( 0 != pGrip->data()->hotGripFunc() )
                        {
                            int iFlags = OdDbGripOperations::kMultiHotGrip;
                            if ( pGrip->isShared() )
                                iFlags |= OdDbGripOperations::kSharedGrip;

                            OdResult eRet =
                                (*pGrip->data()->hotGripFunc())( pGrip->data(), pGrip->entityId(), iFlags );
                            switch( eRet )
                            {
                                case eGripOpGripHotToWarm :
                                {
                                    eCurStatus = OdDbGripOperations::kWarmGrip;
                                    break;
                                }
                            }
                        }
                    }
                }
                aKeys[ i ]-> setStatus( eCurStatus );
            }
        }
        else
        {
            // Launch Grip Edit.
            bool bMakeHot = true;
            {
                DgGripDataMap::const_iterator it = m_aGripData.begin();
                while( (it != m_aGripData.end()) && bMakeHot )
                {
                    const OdDgExGripDataPtrArray& aData = it->second.m_pDataArray;

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
                      const OdDgExGripDataPtrArray& aData = it->second.m_pDataSub.at(i).m_pSubData;
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
            OdDgElementId idEntityToUpdate;
            if ( bMakeHot )
            {
              OdUInt32 i, iSize = aKeys.size();
              for ( i = 0; i < iSize; i++ )
              {
                OdDgExGripDataPtr pGrip = aKeys[ i ];

                OdDbGripOperations::DrawType eNew = OdDbGripOperations::kHotGrip;

                if ( !pGrip->data().isNull() &&
                  0 != pGrip->data()->hotGripFunc() )
                {
                  int iFlags = 0;
                  if ( pGrip->isShared() )
                    iFlags |= OdDbGripOperations::kSharedGrip;

                  if ( pGrip->data()->triggerGrip() )
                  {
                    if ( false == pGrip->isShared() )
                    {
                      OdResult eRet =
                        (*pGrip->data()->hotGripFunc())( pGrip->data(), pGrip->entityId(), iFlags );
                      switch( eRet )
                      {
                      case eOk :
                      case eGripOpGripHotToWarm :
                        {
                          eNew = OdDbGripOperations::kWarmGrip;
                          break;
                        }
                      case eGripOpGetNewGripPoints :
                        {
                          bGetNew = true;
                          idEntityToUpdate = pGrip->entityId();
                          break;
                        }
                      }
                    }
                  }
                  else
                  {
                    OdResult eRet =
                      (*pGrip->data()->hotGripFunc())( pGrip->data(), pGrip->entityId(), iFlags );
                    if ( false == pGrip->isShared() )
                    {
                      switch( eRet )
                      {
                      case eGripOpGripHotToWarm :
                        {
                          eNew = OdDbGripOperations::kWarmGrip;
                          break;
                        }
                      case eGripOpGetNewGripPoints :
                        {
                          bGetNew = true;
                          idEntityToUpdate = pGrip->entityId();
                          break;
                        }
                      }
                    }
                  }
                }

                pGrip->setStatus( eNew );
              }
            }

            if ( bGetNew )
            {
              updateEntityGrips( idEntityToUpdate );
              return true;
            }

            OdDgExGripDataPtrArray aActiveKeys;
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

            OdUInt32 i, iSize = aActiveKeys.size();
            for (i = 0; i < iSize; i++)
              aActiveKeys[ i ]->setStatus( OdDbGripOperations::kDragImageGrip );

            DgGripDataMap::const_iterator it = m_aGripData.begin();
            while( it != m_aGripData.end() )
            {
                bool bActive = false;
                OdDgExGripDragPtr pDrag;
                {
                    const OdDgExGripDataPtrArray& aData = it->second.m_pDataArray;
                    OdUInt32 i, iSize = aData.size();
                    for ( i = 0; i < iSize; i++ )
                    {
                        if ( OdDbGripOperations::kDragImageGrip == aData[ i ]->status() )
                        {
                            bActive = true;
                            pDrag = OdDgExGripDrag::createObject(it->first, this);
                            break;
                        }
                    }
                    for (i = 0; (i < it->second.m_pDataSub.size()) && !bActive; i++)
                    {
                      const OdDgExGripDataPtrArray& aData = it->second.m_pDataSub.at(i).m_pSubData;
                      OdUInt32 j, iSize = aData.size();
                      for (j = 0; j < iSize; j++)
                      {
                        if (OdDbGripOperations::kDragImageGrip == aData[j]->status())
                        {
                          bActive = true;
                          pDrag = OdDgExGripDrag::createObject(it->second.m_pDataSub.at(i).m_entPath, this);
                          break;
                        }
                      }
                    }
                }
                if ( bActive )
                {
                    //pDrag->entityId = it->first;
                    //pDrag->m_pOwner = this;
                    m_aDrags.push_back( pDrag );
                }
                it++;
            }

            iSize = m_aDrags.size();
            for ( i = 0; i < iSize; i++ )
            {
                m_aDrags[ i ]->notifyDragStarted();
                m_aDrags[ i ]->cloneEntity();
            }

            bool bOk = true;

            m_ptBasePoint = aKeys.first()->point();
            m_ptLastPoint = m_ptBasePoint;
            {
                // Use alternative point if needed.
                OdDgGripDataPtr pFirstData = aKeys.first()->data();
                if ( 0 != pFirstData.get() )
                {
                    if ( 0 != pFirstData->alternateBasePoint() )
                    {
                        m_ptBasePoint = *(pFirstData->alternateBasePoint());
                    }
                }
            }

            try
            {
                OdGePoint3d ptFinal = m_pCmdCtx->dbUserIO()->getPoint(
                    OD_T("Specify stretch point or [Base point/Copy/Undo/eXit]:"),
                    OdEd::kGptNoLimCheck | OdEd::kGptDefault | OdEd::kGptNoUCS,
                    &m_ptBasePoint,
                    OD_T("Base Copy Undo eXit"),
                    this );

                //m_pCmdCtx->database()->startUndoRecord();

                iSize = m_aDrags.size();

                if( iSize > 0 )
                {
                  OdDgDatabase* pDgnDb = m_aDrags[ 0 ]->entityId().database();

                  if( pDgnDb )
                  {
                    pDgnDb->startUndoRecord();
                    pDgnDb->setUndoMark();
                    pDgnDb->startTransaction();
                  }

                  for( i = 0; i < iSize; i++ )
                    m_aDrags[ i ]->moveEntity( eyeToUcsPlane(ptFinal, m_ptBasePoint) );

                  if( pDgnDb )
                  {
                    pDgnDb->endTransaction();
                  }
                }
            }
            catch( const OdEdCancel& )
            {
                bOk = false;

                iSize = m_aDrags.size();
                for( i = 0; i < iSize; i++ )
                    m_aDrags[ i ]->notifyDragAborted();
            
            }

            iSize = m_aDrags.size();
            for( i = 0; i < iSize; i++ )
            {
                if ( bOk )
                {
                    m_aDrags[ i ]->notifyDragEnded();
                    updateEntityGrips( m_aDrags[ i ]->entityId() );
                }
                else
                {
                    m_aDrags[ i ]->notifyDragAborted();
                }
            }

            m_aDrags.clear();

            if ( bOk )
                updateInvisibleGrips();

            iSize = aActiveKeys.size();
            for( i = 0; i < iSize; i++ )
                aActiveKeys[ i ]->setStatus( OdDbGripOperations::kWarmGrip );
        }
    }
    else
    {
      return false;
    }

    return true;
}

bool OdDgExGripManager::startHover( int x, int y )
{
    bool bRet = endHover();

    OdDgExGripDataPtrArray aKeys;
    locateGripsAt( x, y, aKeys );
    if ( false == aKeys.empty() )
    {
        m_aHoverGrips = aKeys;

        OdUInt32 i, iSize = m_aHoverGrips.size();
        for( i = 0; i < iSize; i++ )
        {
            OdDgExGripDataPtr pGrip = m_aHoverGrips[ i ];
            if ( pGrip->status() == OdDbGripOperations::kWarmGrip )
            {
                pGrip->setStatus( OdDbGripOperations::kHoverGrip );

                if ( !pGrip->data().isNull() )
                {
                    if ( 0 != pGrip->data()->hoverFunc() )
                    {
                        int iFlags = 0;
                        if ( pGrip->isShared() )
                            iFlags = OdDbGripOperations::kSharedGrip;
                        //OdResult eRet =
                        (*pGrip->data()->hoverFunc())( pGrip->data(), pGrip->entityId(), iFlags );
                    }
                }
                if (m_pGsModel)
                  m_pGsModel->onModified( pGrip.get(), (OdGiDrawable*)0 );
                else
                  m_pDevice->invalidate();
            }
        }

        bRet = true;
    }
    return( bRet );
}

bool OdDgExGripManager::endHover()
{
  if ( m_aHoverGrips.empty() )
    return( false );

  OdUInt32 i, iSize = m_aHoverGrips.size();
  for( i = 0; i < iSize; i++ )
  {
    OdDgExGripDataPtr pGrip = m_aHoverGrips[ i ];
    if ( pGrip->status() == OdDbGripOperations::kHoverGrip )
    {
      pGrip->setStatus( OdDbGripOperations::kWarmGrip );
      if (m_pGsModel)
        m_pGsModel->onModified( pGrip.get(), (OdGiDrawable*)0 );
      else
        m_pDevice->invalidate();
    }
  }
  m_aHoverGrips.clear();
  return( true );
}

bool OdDgExGripManager::onMouseMove( int x, int y )
{
    // Restarts hover operation.
    return startHover( x, y );
}

bool OdDgExGripManager::onControlClick()
{
    if ( m_aDrags.empty() )
        return false;

    // TODO: Notify active grips.

    // AEC grips use CTRL key to change mode,
    // but how to pass it throw standard interface is currently unknown.

    return true;
}

void OdDgExGripManager::selectionSetChanged(OdDgSelectionSetPtr pSSet)
{
    bool bRestoreOld = false;
    if ( pSSet->numEntities() > (unsigned)m_GRIPOBJLIMIT )
    {
        m_bDisabled = true;
    }
    else
    {
        if ( m_bDisabled )
        {
            bRestoreOld = true;
        }
        m_bDisabled = false;
    }

    // Old Entities.
    {
        OdDgElementIdArray aOld;
        DgGripDataMap::iterator it = m_aGripData.begin();
        while( it != m_aGripData.end() )
        {
            if ( m_bDisabled )
            {
                aOld.push_back( it->first );
            }
            else
            {
                if ( false == pSSet->isMember( it->first ) )
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
                    OdDgSelectionSetIteratorPtr ssIt = searchObjectSSetIterator(pSSet, it->first);
                    for (se = 0; se < ssIt->subentCount(); se++)
                    {
                      OdDgFullSubentPath tmpPath;
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
            removeEntityGrips( aOld[ i ], true );
    }

    // New Entities.
    {
        OdDgElementIdArray aNew;
        OdDgSelectionSetIteratorPtr pIter = pSSet->newIterator();
        while ( false == pIter->done() )
        {
            if ( false == m_bDisabled )
            {
                if ( m_aGripData.end() == m_aGripData.find( pIter->objectId() ) )
                {
                    aNew.push_back( pIter->objectId() );
                }
            }

            pIter->next();
        }
        OdUInt32 i, iSize = aNew.size();
        for( i = 0; i < iSize; i++ )
            updateEntityGrips( aNew[ i ] );

    }

    updateInvisibleGrips();
}

void OdDgExGripManager::updateEntityGrips( const OdDgElementId& id )
{
    removeEntityGrips( id, false );

    OdDgSelectionSetPtr pSSet = workingSelectionSet();
    if ( pSSet.isNull() || !pSSet->isMember( id ) )
        return;

    OdDgElementPtr pEntity =
        OdDgElement::cast( id.openObject() );
    if ( pEntity.isNull() )
        return;

    OdDgExGripDataPtrArray aExt;
    OdDgGripDataPtrArray aPts;

    OdDgSelectionSetIteratorPtr pObjIt = searchObjectSSetIterator(pSSet, id);
//     if (pObjIt->subentCount() > 0)
//     {
//       for (OdUInt32 se = 0; se < pObjIt->subentCount(); se++)
//       {
//         OdDgFullSubentPath subEntPath;
//         pObjIt->getSubentity(se, subEntPath);
//         aPts.clear();
//         if (pEntity->getGripPointsAtSubentPath(subEntPath, aPts, activeViewUnitSize(), m_GRIPSIZE, activeViewDirection(), 0) == eOk)
//         {
//           OdUInt32 prevSize = aExt.size();
//           aExt.resize(prevSize + aPts.size());
//           for (OdUInt32 i = 0; i < aPts.size(); i++)
//           {
//             aExt[i + prevSize] = OdDgExGripData::createObject(
//               subEntPath, aPts[ i ], aPts[ i ]->gripPoint(), this);
//           }
//         }
//       }
//     }
//     else
    {
      if ( eOk == pEntity->getGripPoints(
          aPts, activeViewUnitSize(), m_GRIPSIZE, activeViewDirection(), 0 ) )
      {
          aExt.resize( aPts.size() );
          OdUInt32 i, iSize = aExt.size();
          for( i = 0; i < iSize; i++ )
          {
            aExt[ i ] = OdDgExGripData::createObject(
              pEntity->elementId(), aPts[ i ], aPts[ i ]->gripPoint(), this);
          }
      }
      else
      {
          OdGePoint3dArray aOldPts;
          if ( eOk == pEntity->getGripPoints( aOldPts ) )
          {
              aExt.resize( aOldPts.size() );
              OdUInt32 i, iSize = aExt.size();
              for( i = 0; i < iSize; i++ )
              {
                aExt[ i ] = OdDgExGripData::createObject(
                  pEntity->elementId(), 0, aOldPts[ i ], this);
              }
          }
      }
    }

  bool bModel = true; //pEntity->database()->getTILEMODE();
  if ( !aExt.empty() )
  {
    OdUInt32 i, iSize = aExt.size();
    OdDgExGripDataExt dExt;
    for (i = 0; i < iSize; i++)
    {
      OdDgFullSubentPath entPath;
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
          OdDgExGripDataSubent se;
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

void OdDgExGripManager::removeEntityGrips( const OdDgElementId& id, bool bFireDone )
{
  DgGripDataMap::iterator it = m_aGripData.find( id );
  if ( it != m_aGripData.end() )
  {
    OdDgElementPtr pEntity = OdDgElement::cast( id.openObject() );
    if ( pEntity.get() )
      pEntity->gripStatus( OdDgElement::kGripsToBeDeleted );

    bool bModel = true; //id.database()->getTILEMODE();
    OdUInt32 i, iSize = it->second.m_pDataArray.size();
    for ( i = 0; i < iSize; i++ )
    {
      OdDgExGripDataPtr pData = it->second.m_pDataArray[ i ];
      hideGrip( pData, bModel );
      if (!it->second.m_pDataArray[i]->data().isNull() && it->second.m_pDataArray[i]->data()->gripOpStatFunc())
        (*it->second.m_pDataArray[i]->data()->gripOpStatFunc())(it->second.m_pDataArray[i]->data(), id, OdDbGripOperations::kGripEnd);
      it->second.m_pDataArray[i] = 0;
    }
    for (i = 0; i < it->second.m_pDataSub.size(); i++)
    {
      for (OdUInt32 j = 0; j < it->second.m_pDataSub.at(i).m_pSubData.size(); j++)
      {
        OdDgExGripDataPtr pData = it->second.m_pDataSub.at(i).m_pSubData[j];
        hideGrip( pData, bModel );
        it->second.m_pDataSub.at(i).m_pSubData[j] = 0;
      }
    }

    if ( bFireDone )
    {
      if ( pEntity.get() )
        pEntity->gripStatus( OdDgElement::kGripsDone );
    }
    m_aGripData.erase( it );
  }
}

void OdDgExGripManager::locateGripsAt( int x, int y, OdDgExGripDataPtrArray& aResult )
{
    aResult.clear();

    double dX = x;
    double dY = y;

    OdGePoint3d ptFirst;
    DgGripDataMap::const_iterator it = m_aGripData.begin();
    while( it != m_aGripData.end() )
    {
      for (OdUInt32 se = 0; se < it->second.m_pDataSub.size() + 1; se++)
      {
        const OdDgExGripDataPtrArray& aData = (se == 0) ? it->second.m_pDataArray : it->second.m_pDataSub[se - 1].m_pSubData;

        OdUInt32 i, iSize = aData.size();
        for ( i = 0; i < iSize; i++ )
        {
            const OdGePoint3d& ptCurrent = aData[ i ]->point();

            if ( aResult.empty() )
            {
                // First grip is obtained by comparing
                // grip point device position with cursor position.
                OdGePoint3d ptDC = ptCurrent;
                ptDC.transformBy( m_pDevice->activeView()->worldToDeviceMatrix() );

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

void OdDgExGripManager::locateGripsByStatus( OdDbGripOperations::DrawType eStatus, OdDgExGripDataPtrArray& aResult )
{
    aResult.clear();

    DgGripDataMap::const_iterator it = m_aGripData.begin();
    while( it != m_aGripData.end() )
    {
      for (OdUInt32 se = 0; se < it->second.m_pDataSub.size() + 1; se++)
      {
        const OdDgExGripDataPtrArray& aData = (se == 0) ? it->second.m_pDataArray : it->second.m_pDataSub[se - 1].m_pSubData;
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

struct DgSortGripsAlongXAxis
{
    bool operator()( const OdDgExGripDataPtr& grA, const OdDgExGripDataPtr& grB )
    {
        return OdPositive( grA->point().x, grB->point().x );
    }
};

}

void OdDgExGripManager::updateInvisibleGrips()
{
    OdDgExGripDataPtrArray aOverall;
    DgGripDataMap::const_iterator it = m_aGripData.begin();
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
    std::sort( aOverall.begin(), aOverall.end(), DgSortGripsAlongXAxis() );

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
                OdDgExGripDataPtr pGrip = aOverall[ aEq[ j ] ];

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
                OdDgExGripDataPtr pGrip = aOverall[ aEq[ j ] ];

                pGrip->setShared(true);
                pGrip->setInvisible(j != iVisible);
            }
        }
    }
}

void OdDgExGripManager::showGrip( OdDgExGripData* pGrip, bool /*bModel*/ )
{
//  OdGsPaperLayoutHelperPtr pPaperHelper = OdGsPaperLayoutHelper::cast(m_pDevice);
  OdUInt32 i, iSize = m_pDevice->numViews();
//   if(pPaperHelper.get())
//   {
//     //for( i = 0; i < iSize; i++ )
//     //  if(bModel==(pPaperHelper->viewAt(i) != pPaperHelper->overallView()))
//     //    pPaperHelper->viewAt( i )->add( pGrip, m_pGsModel );
//     OdDbObjectPtr pVpObj = m_pCmdCtx->database()->activeViewportId().openObject();
//     OdDbAbstractViewportDataPtr pAVD = OdDbAbstractViewportDataPtr(pVpObj);
//     if (!pAVD.isNull() && pAVD->gsView(pVpObj))
//       pAVD->gsView(pVpObj)->add( pGrip, m_pGsModel );
//     else
//       pPaperHelper->overallView()->add( pGrip, m_pGsModel );
//   }
//   else
  {
    for( i = 0; i < iSize; i++ )
      m_pDevice->viewAt( i )->add( pGrip, m_pGsModel );
  }
}

void OdDgExGripManager::hideGrip( OdDgExGripData* pGrip, bool /*bModel*/ )
{
//  OdGsPaperLayoutHelperPtr pPaperHelper = OdGsPaperLayoutHelper::cast(m_pDevice);
  OdUInt32 i, iSize = m_pDevice->numViews();
//   if(pPaperHelper.get())
//   {
//     for( i = 0; i < iSize; i++ )
//       m_pDevice->viewAt( i )->erase( pGrip );
//   }
//   else
  {
    for( i = 0; i < iSize; i++ )
      m_pDevice->viewAt( i )->erase( pGrip );
  }
}

void OdDgExGripManager::setValue( const OdGePoint3d& ptValue )
{
    OdUInt32 i, iSize = m_aDrags.size();
    OdGePoint3d newPoint = eyeToUcsPlane(ptValue, m_ptBasePoint);
    for ( i = 0; i < iSize; i++ )
        m_aDrags[ i ]->cloneEntity( newPoint );
    m_ptLastPoint = newPoint;
}

int OdDgExGripManager::addDrawables( OdGsView* pView )
{
    ODA_ASSERT(pView->device()==m_pDevice->underlyingDevice().get());

    OdUInt32 i, iSize = m_aDrags.size();
    for ( i = 0; i < iSize; i++ )
        pView->add( m_aDrags[ i ].get(), /*m_pGsModel*/ 0 );
    return iSize;
}

void OdDgExGripManager::removeDrawables( OdGsView* pView )
{
  OdUInt32 i, iSize = m_aDrags.size();
  for ( i = 0; i < iSize; i++ )
    pView->erase( m_aDrags[ i ].get() );
}

OdDgSelectionSetPtr OdDgExGripManager::workingSelectionSet() const
{
  if (m_pGetSelectionSetPtr)
  {
    return m_pGetSelectionSetPtr(m_pCmdCtx);
  }
  return OdDgSelectionSetPtr();
}

OdDgElementId OdDgExGripManager::activeVpId() const
{
  OdGsClientViewInfo vi;
  m_pDevice->activeView()->clientViewInfo(vi);
  return OdDgElementId(vi.viewportObjectId);
}

double OdDgExGripManager::activeViewUnitSize() const
{
    OdGsView* pView = m_pDevice->activeView();

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

OdGeVector3d OdDgExGripManager::activeViewDirection() const
{
  OdGsView* pView = m_pDevice->activeView();
  return (pView->position() - pView->target()).normal();
}

OdGePoint3d OdDgExGripManager::eyeToUcsPlane(const OdGePoint3d &pPoint, const OdGePoint3d &pBasePoint) const
{
   OdGePoint3d ucsOrigin(0,0,0);
   OdGeVector3d ucsXAxis = OdGeVector3d::kXAxis;
   OdGeVector3d ucsYAxis = OdGeVector3d::kYAxis;
   OdGePlane plane(/*ucsOrigin*/pBasePoint, activeViewDirection());
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

bool OdDgExGripManager::handleMappedRtClk( OdDgExGripDataPtrArray &aActiveKeys, int x, int y )
{
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
      POINT pt = {x, y};
      ::ClientToScreen(wnd, &pt);
      (*cb)(::TrackPopupMenu((HMENU)menu, 
        TPM_LEFTALIGN|TPM_TOPALIGN|TPM_NONOTIFY|TPM_RETURNCMD|TPM_LEFTBUTTON|TPM_NOANIMATION, pt.x, pt.y, 0, wnd, 0));
      ::DestroyMenu((HMENU)menu);
      for (OdUInt32 i = 0; i < iSize; i++)
        aActiveKeys[ i ]->setStatus( OdDbGripOperations::kWarmGrip );
      updateEntityGrips(aActiveKeys[rtClkIndex]->entityId());
      return true;
    }
  }
  return false;
}


OdExGripDgReactor::OdExGripDgReactor()
  : m_pOwner(0)
{
}

void OdExGripDgReactor::objectAppended( const OdDgDatabase* , const OdDgElement* )
{
  // New object.
}

void OdExGripDgReactor::objectModified( const OdDgDatabase*, const OdDgElement* pDbObj )
{
  m_pOwner->updateEntityGrips( pDbObj->elementId() );
  m_pOwner->updateInvisibleGrips();
}

void OdExGripDgReactor::objectErased( const OdDgDatabase*, const OdDgElement* pDbObj, bool pErased )
{
  if ( pErased )
  {
    m_pOwner->removeEntityGrips( pDbObj->elementId(), true );
    m_pOwner->updateInvisibleGrips();
  }
}
