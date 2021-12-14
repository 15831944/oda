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
// OdqDragTracker.cpp
//

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
#include "Gs/GsModel.h"

#include "OdqDragTracker.h"
#include "OdqConsoleTab.h"

//////////////////////////////////////////////////////////////////////////

ODRX_NO_CONS_DEFINE_MEMBERS(OdqDragTracker, OdEdPointTracker); // OdEdPointTrackerWithSnapInfo);

//static
OdqDragTrackerPtr OdqDragTracker::createObject(OdqGripManagerPtr pGripManager,
                                               const OdGePoint3d& ptBase,
                                               bool bUseClones) // = true
{
  ODA_ASSERT_ONCE(pGripManager.get()); // test
  OdqDragTrackerPtr pDragTracker = OdRxObjectImpl<OdqDragTracker>::createObject();
  pDragTracker->init(pGripManager, ptBase, bUseClones);
  return pDragTracker;
}

OdqDragTracker::OdqDragTracker()
  : m_bCanceled(true) // : OdEdPointTrackerWithSnapInfo(OdDbObjectIdArray()), m_bCanceled(true)
  , m_pDrawablesWereAddedToGsView(NULL)
{
}

OdqDragTracker::~OdqDragTracker()
{
  uninit();
}

bool OdqDragTracker::init(OdqGripManagerPtr pGripManager,
                          const OdGePoint3d& ptBase,
                          bool bUseClones) // = true
{
  ODA_ASSERT_ONCE(!pGripManager.isNull());
  m_pGripManager = pGripManager;

  //OdDbDatabase* pDwgDb = OdDbDatabase::cast(m_pGripManager->m_pTabConsole->getRxDatabase()).get();
  //if (pDgwDb)
  //  pDwgDb->addReactor(&m_cDbReactor);
  
  // see also OdExGripManager::onMouseDown after call OdBaseGripManager::onMouseDown

  OdExGripDataPtrArray aActiveKeys;
  m_pGripManager->locateGripsByStatus(OdDbGripOperations::kHotGrip, aActiveKeys);
  if (aActiveKeys.empty())
  {
    ODA_FAIL_ONCE();
    return false;
  }

  //if (handleMappedRtClk(aActiveKeys, x, y))
  //  return true;

  OdUInt32 i, j, iSize = aActiveKeys.size();
  for (i = 0; i < iSize; i++)
    aActiveKeys[i]->setStatus(OdDbGripOperations::kDragImageGrip);

  for (OdqGripManager::GripDataMap::const_iterator it = m_pGripManager->m_aGripData.begin();
       it != m_pGripManager->m_aGripData.end(); it++)
  {
    bool bActive = false;
    OdExGripDragPtr pDrag;
    {
      const OdExGripDataPtrArray& aData = it->second.m_pDataArray;
      iSize = aData.size();
      for (i = 0; i < iSize; i++)
      {
        if (OdDbGripOperations::kDragImageGrip == aData[i]->status())
        {
          bActive = true;
          pDrag = OdExGripDrag::createObject(it->first, m_pGripManager.get());
          break;
        }
      }
      for (i = 0; (i < it->second.m_pDataSub.size()) && !bActive; i++)
      {
        const OdExGripDataPtrArray& aData = it->second.m_pDataSub.at(i).m_pSubData;
        iSize = aData.size();
        for (j = 0; j < iSize; j++)
        {
          if (OdDbGripOperations::kDragImageGrip == aData[j]->status())
          {
            bActive = true;
            pDrag = OdExGripDrag::createObject(it->second.m_pDataSub.at(i).m_entPath, m_pGripManager.get());
            break;
          }
        }
      }
    }
    if (bActive)
    {
      ////pDrag->entityId = it->first;
      ////pDrag->m_pOwner = this;
      m_pGripManager->m_aDrags.push_back(pDrag);
    }
  }

  iSize = m_pGripManager->m_aDrags.size();
  for (i = 0; i < iSize; i++)
  {
    m_pGripManager->m_aDrags[i]->notifyDragStarted();
    if (bUseClones)
      m_pGripManager->m_aDrags[i]->cloneEntity();
  }

  // will be set via finishAt call //m_bCanceled = false; // bool bOk = true;

  //m_pGripManager->m_ptBasePoint = aActiveKeys.first()->point();
  //{
  //  // Use alternative point if needed.
  //  OdDbGripDataPtr pFirstData = aActiveKeys.first()->data();
  //  if (pFirstData.get() && pFirstData->alternateBasePoint())
  //    m_pGripManager->m_ptBasePoint = *(pFirstData->alternateBasePoint());
  //}
  //
  // usially is near by not equal //ODA_ASSERT_ONCE(ptBase == aActiveKeys.first()->point()); // test
  //m_pGripManager->m_ptBasePoint = aActiveKeys.first()->point();
  m_pGripManager->m_ptBasePoint = ptBase; 
  m_pGripManager->m_ptLastPoint = m_pGripManager->m_ptBasePoint;

  return true;
}

void OdqDragTracker::uninit()
{
  OdUInt32 i, iSize;

  // TODO maybe enough do the next under (if (m_bCanceled)) only
  //
  // should be earlier than m_pGripManager->updateEntityGrips 
  // (recreate OdExGripData via OdExGripDataImpl::createObject)
  OdExGripDataPtrArray aActiveKeys;
  m_pGripManager->locateGripsByStatus(OdDbGripOperations::kDragImageGrip, aActiveKeys);
  iSize = aActiveKeys.size();
  ODA_ASSERT_ONCE(iSize);
  for (i = 0; i < iSize; i++)
    aActiveKeys[i]->setStatus(OdDbGripOperations::kWarmGrip);

  iSize = m_pGripManager->m_aDrags.size();
  if (m_bCanceled) // (!bOk)
  {
    for (i = 0; i < iSize; i++)
      m_pGripManager->m_aDrags[i]->notifyDragAborted();
  }
  else
  {
    for (i = 0; i < iSize; i++)
    {
      m_pGripManager->m_aDrags[i]->notifyDragEnded();
      m_pGripManager->updateEntityGrips(m_pGripManager->m_aDrags[i]->id());
    }
  }

  m_pGripManager->m_aDrags.clear();

  if (!m_bCanceled) // (bOk)
    m_pGripManager->updateInvisibleGrips();

  //OdDbDatabase* pDwgDb = OdDbDatabase::cast(m_pGripManager->m_pTabConsole->getRxDatabase()).get();
  //if (pDwgDb)
  //  pDwgDb->removeReactor(&m_cDbReactor);
}

bool OdqDragTracker::finishAt(const OdGePoint3d& ptFinal)
{
  ODA_ASSERT_ONCE(   m_bCanceled  // default
                  && m_pGripManager->m_ptLastPoint.distanceTo(ptFinal) < 1.0); // test
  OdUInt32 i, iSize = m_pGripManager->m_aDrags.size();
  ODA_ASSERT_ONCE(iSize);
  if (iSize)
    m_bCanceled = false;

  for (i = 0; i < iSize; i++)
    m_pGripManager->m_aDrags[i]->moveEntity(m_pGripManager->eyeToUcsPlane(ptFinal, // m_pGripManager->m_ptLastPoint
                                                                          m_pGripManager->m_ptBasePoint));
  return !m_bCanceled;
}

void OdqDragTracker::setValue(const OdGePoint3d& ptValue)
{
  m_pGripManager->setValue(ptValue);
  ODA_ASSERT_ONCE(m_pDrawablesWereAddedToGsView); // test
  if (m_pDrawablesWereAddedToGsView)
  {
    //PROD-149
    m_pGripManager->onModified(m_pDrawablesWereAddedToGsView); // m_pDrawablesWereAddedToGsView->onModified(this, NULL); // is a way of invalidateGsModel()
    // or renew
    //m_pGripManager->removeDrawables(m_pDrawablesWereAddedToGsView);
    //m_pGripManager->addDrawables(m_pDrawablesWereAddedToGsView);
  }
}

int OdqDragTracker::addDrawables(OdGsView* pView)
{
  ODA_ASSERT_ONCE(!m_pDrawablesWereAddedToGsView);
  m_pDrawablesWereAddedToGsView = pView;
  return m_pGripManager->addDrawables(pView);
}

void OdqDragTracker::removeDrawables(OdGsView* pView)
{
  ODA_ASSERT_ONCE(m_pDrawablesWereAddedToGsView);
  m_pDrawablesWereAddedToGsView = NULL;
  m_pGripManager->removeDrawables(pView);
}

// TODO
//OdExGripDbReactor::OdExGripDbReactor()
//  : m_pOwner(0)
//{
//}
//
//void OdExGripDbReactor::objectAppended(const OdDbDatabase* , const OdDbObject*)
//{
//  // New object.
//}
//
//void OdExGripDbReactor::objectModified(const OdDbDatabase*, const OdDbObject* pDbObj)
//{
//  m_pOwner->updateEntityGrips(pDbObj->objectId());
//  m_pOwner->updateInvisibleGrips();
//}
//
//void OdExGripDbReactor::objectErased(const OdDbDatabase*, const OdDbObject* pDbObj, bool pErased)
//{
//  if (pErased)
//  {
//    m_pOwner->removeEntityGrips(pDbObj->objectId(), true);
//    m_pOwner->updateInvisibleGrips();
//  }
//}
