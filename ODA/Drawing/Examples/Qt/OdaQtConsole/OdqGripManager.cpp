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
// OdqGripManager.cpp
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
#include "RxVariantValue.h"

#include "OdqGripManager.h"
#include "OdqConsoleTab.h"
#include "ExtDbModule.h"

//////////////////////////////////////////////////////////////////////////

ODRX_NO_CONS_DEFINE_MEMBERS(OdqGripManager, OdEdPointTracker);

//static 
OdqGripManagerPtr OdqGripManager::createObject(OdqConsoleTab& tabConsole)
{
  OdqGripManagerPtr pGripManager = OdRxObjectImpl<OdqGripManager>::createObject();
  pGripManager->init(tabConsole);
  return pGripManager;
}

OdqGripManager::OdqGripManager()
  : m_pTabConsole(NULL)
  , m_isDragModel2d(getIAppProps()->toBool(L"Vars/dragmodel2d", false))
{
  m_GRIPCOLOR.setColorIndex(160);
  m_GRIPHOVER.setColorIndex(3);
  m_GRIPHOT.setColorIndex(1);

  //m_cDbReactor.m_pOwner = this;
}

OdqGripManager::~OdqGripManager()
{
  endHover();
  uninit();
}

void OdqGripManager::init(OdqConsoleTab& tabConsole)
{
  m_pTabConsole = &tabConsole;
  
  OdDbHostAppServices* pAppSvcs = NULL;
  OdDbDatabase* pDwgDb = OdDbDatabase::cast(tabConsole.getRxDatabase()).get();
  if (pDwgDb)
    pAppSvcs = pDwgDb->appServices();
  if (!pAppSvcs)
  {
    // TODO (after the same variables will be added to DgRegVarDefs.h) :
    //
    // get GRIP constants via OdDgHostAppServices
    //                 or via valueToString of OdSysVarPE
    // OdSysVarPEPtr pSysVarPE = OdSysVarPE::cast(tabConsole.getRxDatabase());
    // ... = pSysVarPE->valueToString(...);
    //
    //pAppSvcs = OdDbHostAppServices::cast(getIApp()->getBaseHostAppServices()).get();
    // hardcoded colors for DGN  :
    m_GRIPCOLOR.setColorIndex(1);  // blue
    m_GRIPHOVER.setColorIndex(2); // green
    m_GRIPHOT.setColorIndex(3); // red
  }
  else
  {
    m_GRIPSIZE = pAppSvcs->getGRIPSIZE();
    m_GRIPOBJLIMIT = pAppSvcs->getGRIPOBJLIMIT();
    m_GRIPCOLOR.setColorIndex(pAppSvcs->getGRIPCOLOR());
    m_GRIPHOVER.setColorIndex(pAppSvcs->getGRIPHOVER());
    m_GRIPHOT.setColorIndex(pAppSvcs->getGRIPHOT());
  }

  disable(false);
}

void OdqGripManager::uninit()
{
  disable(true);
}

void OdqGripManager::selectionSetChanged(OdSelectionSet* pSS)
{
  ODA_ASSERT_ONCE(!pSS || pSS == m_pTabConsole->workingSelectionSet()); // TODO

  OdSelectionSet* pSSet = m_pTabConsole->workingSelectionSet();
  ODA_ASSERT_ONCE(pSSet);

  OdBaseGripManager::selectionSetChanged(pSSet);
}

void OdqGripManager::updateAllGrips()
{
  if (m_bDisabled)
    return;
  OdSelectionSet* pSSet = m_pTabConsole->workingSelectionSet();
  ODA_ASSERT_ONCE(pSSet);

  OdRxObject* pRxDb = m_pTabConsole->getRxDatabase().get();
  OdExtDbModule* pExtDb = NULL;
  if (OdDbDatabase::cast(pRxDb).isNull())
    pExtDb = getIApp()->getExtDbModule(pRxDb);

  OdDbStubPtrArray idsToRemove;
  for (OdSelectionSetIteratorPtr pIter = pSSet->newIterator();
       !pIter->done(); pIter->next())
  {
    OdDbStub* id = pIter->id();
    ODA_ASSERT_ONCE(id);
    if (!id)
      continue;

    bool bErased = true;
    if (!pExtDb)
      bErased = OdDbObjectId(id).isErased();
    else
      bErased = pExtDb->isErased(id);

    if (bErased)
    {
      removeEntityGrips(id, true);
      idsToRemove.push_back(id);
      continue;
    }

    updateEntityGrips(id);
  }
  if (!idsToRemove.isEmpty())
  {
    // clear in SSet
    for (unsigned index = 0; index < idsToRemove.size(); index++)
      pSSet->remove(idsToRemove[index]);
  }

  updateInvisibleGrips();
}

OdUInt32 OdqGripManager::numGripPoints()
{
  return (OdUInt32) m_aGripData.size();
}

OdUInt32 OdqGripManager::numGripPoints(enum OdDbGripOperations::DrawType eStatus)
{
  if (!m_aGripData.size())
    return 0;
  OdExGripDataPtrArray aResult;
  locateGripsByStatus(eStatus, aResult);
  return aResult.size();
}

void OdqGripManager::showGrip(OdExGripData* pGrip, 
                              bool) // bModel
{
  IqViewSystem* iVS = getIViewSystem();
  if (iVS)
    iVS->highlightGripPoint(true, pGrip, m_pTabConsole->getRxDatabase());
}

void OdqGripManager::hideGrip(OdExGripData* pGrip, 
                              bool) // bModel
{
  IqViewSystem* iVS = getIViewSystem();
  if (iVS)
    iVS->highlightGripPoint(false, pGrip, m_pTabConsole->getRxDatabase());
}

int OdqGripManager::addDrawables(OdGsView* pView)
{
  OdUInt32 i, iSize = m_aDrags.size();
  if (!iSize)
    return 0;
  OdGsModelPtr pGsModel = getIViewSystem()->getView(
                                m_pTabConsole->getRxDatabase())->getGsModel(m_isDragModel2d);
  for (i = 0; i < iSize; i++)
    pView->add(m_aDrags[i].get(), pGsModel);
  return iSize;
}

void OdqGripManager::removeDrawables(OdGsView* pView)
{
  OdUInt32 i, iSize = m_aDrags.size();
  for (i = 0; i < iSize; i++)
    pView->erase(m_aDrags[i].get());
}

void OdqGripManager::onModified(OdGsView* pView)
{
  OdUInt32 i, iSize = m_aDrags.size();
  for (i = 0; i < iSize; i++)
  {
    if (OdGsModel* pModel = pView->getModel(m_aDrags[i].get()))
      pModel->onModified(m_aDrags[i].get(), (OdGiDrawable*) NULL);
  }
}

void OdqGripManager::onModified(OdGiDrawable* pGrip)
{
  ODA_ASSERT_ONCE(pGrip);
  IqViewSystem* iVS = getIViewSystem();
  if (!iVS || !pGrip)
    return;
  //iVS->highlightGripPoint(false, pGrip, m_pTabConsole->getRxDatabase());
  //iVS->highlightGripPoint(true, pGrip, m_pTabConsole->getRxDatabase());
  iVS->onModifiedGrip(pGrip, m_pTabConsole->getRxDatabase());
}

OdSelectionSetPtr OdqGripManager::workingSelectionSet() const
{
  if (!m_pTabConsole)
  {
    ODA_FAIL_ONCE();
    return OdSelectionSetPtr();
  }

  return m_pTabConsole->workingSelectionSet();
}

OdGsView* OdqGripManager::activeGsView() const
{
  //OdGsView* pView = m_pDevice->activeView();
  IqView* iView = getIViewSystem()->getView(m_pTabConsole->getRxDatabase());
  if (!iView)
    return NULL;
  OdGsView* pView = iView->getActiveGsView();
  ODA_ASSERT_ONCE(pView);
  return pView;
}

OdGePoint3d OdqGripManager::eyeToUcsPlane(const OdGePoint3d& pt, const OdGePoint3d& ptBase) const
{
  IqView* iView = getIViewSystem()->getView(m_pTabConsole->getRxDatabase());
  ODA_ASSERT_ONCE(iView);
  OdGePlane plane;
  if (!iView->getUcsPlane(plane))
  {
    ODA_FAIL_ONCE(); 
  }

  OdGeLine3d line(pt, activeViewDirection());
  OdGePoint3d newPoint;
  if (!plane.intersectWith(line, newPoint))
    newPoint = ptBase;
  return newPoint;
}

bool OdqGripManager::checkGripSetAt(int x, int y, bool& bShift)
{
  bShift = false;
  OdqConsoleTab::OdEdBaseIoStateKey keyState = m_pTabConsole->getKeyState();
  if (keyState & OdEdBaseIO::kShiftIsDown)
    bShift = true;
  bool res = onMouseDown(x, y, bShift);
  //if (bShift)
  //{
  // TODO for way in OdExGripManager::onMouseDown()
  //}
  return res;
}

OdGiDrawablePtr OdqGripManager::cloneEntity(OdDbStub* id)
{
  OdRxObject* pRxDb = m_pTabConsole->getRxDatabase();

  OdExtDbModule* pExtDb = NULL;
  if (OdDbDatabase::cast(pRxDb).isNull())
    pExtDb = getIApp()->getExtDbModule(pRxDb);
  if (pExtDb)
    return pExtDb->cloneEntity(id);

  OdDbEntityPtr pEntity = OdDbObjectId(id).safeOpenObject();

  OdDbEntityPtr pClone;
  if (pEntity->cloneMeForDragging())
    pClone = OdDbEntity::cast(pEntity->clone());

  OdGiDrawablePtr pRes;
  if (pClone.get())
  {
    pClone->disableUndoRecording(true);
    pClone->setPropertiesFrom(pEntity.get(), false);
    pRes = OdGiDrawable::cast(pClone);
  }
  return pRes;
}

OdGiDrawablePtr OdqGripManager::openObject(OdDbStub* id, 
                                           bool isForWriteMode) // = false
{
  OdRxObject* pRxDb = m_pTabConsole->getRxDatabase();
  OdExtDbModule* pExtDb = NULL;
  if (OdDbDatabase::cast(pRxDb).isNull())
    pExtDb = getIApp()->getExtDbModule(pRxDb);
  if (pExtDb)
    return OdGiDrawable::cast(pExtDb->openObject(id, isForWriteMode).get());

  OdGiDrawablePtr pRes = OdGiDrawable::cast(OdDbObjectId(id).safeOpenObject(isForWriteMode ? OdDb::kForWrite : OdDb::kForRead, true));
  return pRes;
}

OdResult OdqGripManager::getGripPointsAtSubentPath(OdGiDrawable* pEntity, const OdDbBaseFullSubentPath& path, 
                                                   OdDbGripDataPtrArray& grips, double curViewUnitSize, 
                                                   int gripSize, const OdGeVector3d& curViewDir, const OdUInt32 bitflags) const
{
  OdRxObject* pRxDb = m_pTabConsole->getRxDatabase();
  OdExtDbModule* pExtDb = NULL;
  if (OdDbDatabase::cast(pRxDb).isNull())
    pExtDb = getIApp()->getExtDbModule(pRxDb);
  if (pExtDb)
    return pExtDb->getGripPointsAtSubentPath(pRxDb, pEntity, path, grips, curViewUnitSize, 
                                             gripSize, curViewDir, bitflags);
  return OdDbEntity::cast(pEntity)->getGripPointsAtSubentPath(*((const OdDbFullSubentPath*) &path), 
                                                              grips, curViewUnitSize, 
                                                              gripSize, curViewDir, bitflags);
}

OdResult OdqGripManager::getGripPoints(OdGiDrawable* pEntity, OdDbGripDataPtrArray& grips, double curViewUnitSize, 
                                       int gripSize, const OdGeVector3d& curViewDir, int bitFlags) const
{
  OdRxObject* pRxDb = m_pTabConsole->getRxDatabase();
  OdExtDbModule* pExtDb = NULL;
  if (OdDbDatabase::cast(pRxDb).isNull())
    pExtDb = getIApp()->getExtDbModule(pRxDb);
  if (pExtDb)
    return pExtDb->getGripPoints(pRxDb, pEntity, grips, curViewUnitSize, gripSize, curViewDir, bitFlags);
  return OdDbEntity::cast(pEntity)->getGripPoints(grips, curViewUnitSize, gripSize, curViewDir, bitFlags);
}

OdResult OdqGripManager::getGripPoints(OdGiDrawable* pEntity, OdGePoint3dArray& gripPoints) const
{
  OdRxObject* pRxDb = m_pTabConsole->getRxDatabase();
  OdExtDbModule* pExtDb = NULL;
  if (OdDbDatabase::cast(pRxDb).isNull())
    pExtDb = getIApp()->getExtDbModule(pRxDb);
  if (pExtDb)
    return pExtDb->getGripPoints(pRxDb, pEntity, gripPoints);
  return OdDbEntity::cast(pEntity)->getGripPoints(gripPoints);
}

OdResult OdqGripManager::moveGripPointsAtSubentPaths(OdGiDrawable* pEntity, const OdDbBaseFullSubentPathArray& paths, 
                                                     const OdDbVoidPtrArray& gripAppData, const OdGeVector3d& offset,
                                                     OdUInt32 bitflags)
{
  OdRxObject* pRxDb = m_pTabConsole->getRxDatabase();
  OdExtDbModule* pExtDb = NULL;
  if (OdDbDatabase::cast(pRxDb).isNull())
    pExtDb = getIApp()->getExtDbModule(pRxDb);
  if (pExtDb)
    return pExtDb->moveGripPointsAtSubentPaths(pRxDb, pEntity, paths, gripAppData, offset, bitflags);
  ODA_ASSERT_ONCE(sizeof(OdDbFullSubentPath) == sizeof(OdDbBaseFullSubentPath));
  return OdDbEntity::cast(pEntity)->moveGripPointsAtSubentPaths(*((const OdDbFullSubentPathArray*) &paths), 
                                                                gripAppData, offset, bitflags);
}

OdResult OdqGripManager::moveGripPointsAt(OdGiDrawable* pEntity, const OdDbVoidPtrArray& gripAppData, const OdGeVector3d& offset, int bitFlags)
{
  OdRxObject* pRxDb = m_pTabConsole->getRxDatabase();
  OdExtDbModule* pExtDb = NULL;
  if (OdDbDatabase::cast(pRxDb).isNull())
    pExtDb = getIApp()->getExtDbModule(pRxDb);
  if (pExtDb)
  {
    OdResult res = pExtDb->moveGripPointsAt(pRxDb, pEntity, gripAppData, offset, bitFlags);
    if (res == eOk && OdDbBaseDatabasePEPtr(pRxDb)->getId(OdRxObject::cast(pEntity))) // is not a clone (DGN way)
      m_ptBasePoint += offset;
    return res;
  }
  return OdDbEntity::cast(pEntity)->moveGripPointsAt(gripAppData, offset, bitFlags);
}

OdResult OdqGripManager::moveGripPointsAt(OdGiDrawable* pEntity, const OdIntArray& indices, const OdGeVector3d& offset)
{
  OdRxObject* pRxDb = m_pTabConsole->getRxDatabase();
  OdExtDbModule* pExtDb = NULL;
  if (OdDbDatabase::cast(pRxDb).isNull())
    pExtDb = getIApp()->getExtDbModule(pRxDb);
  if (pExtDb)
  {
    OdResult res = pExtDb->moveGripPointsAt(pRxDb, pEntity, indices, offset);
    if (res == eOk && OdDbBaseDatabasePEPtr(pRxDb)->getId(OdRxObject::cast(pEntity))) // is not a clone (DGN way)
      m_ptBasePoint += offset;
    return res;
  }
  return OdDbEntity::cast(pEntity)->moveGripPointsAt(indices, offset);
}

void OdqGripManager::subentGripStatus(OdGiDrawable* pEntity, OdDb::GripStat status, const OdDbBaseFullSubentPath& subentity)
{
  OdRxObject* pRxDb = m_pTabConsole->getRxDatabase();
  OdExtDbModule* pExtDb = NULL;
  if (OdDbDatabase::cast(pRxDb).isNull())
    pExtDb = getIApp()->getExtDbModule(pRxDb);
  if (pExtDb)
  {
    pExtDb->subentGripStatus(pRxDb, pEntity, status, subentity);
    return; 
  }
  OdDbEntity::cast(pEntity)->subentGripStatus(status, *((const OdDbFullSubentPath*) &subentity));
}

void OdqGripManager::gripStatus(OdGiDrawable* pEntity, OdDb::GripStat st)
{
  OdRxObject* pRxDb = m_pTabConsole->getRxDatabase();
  OdExtDbModule* pExtDb = NULL;
  if (OdDbDatabase::cast(pRxDb).isNull())
    pExtDb = getIApp()->getExtDbModule(pRxDb);
  if (pExtDb)
  {
    pExtDb->gripStatus(pRxDb, pEntity, st);
    return; 
  }
  OdDbEntity::cast(pEntity)->gripStatus(st);
}

void OdqGripManager::dragStatus(OdGiDrawable* pEntity, OdDb::DragStat st)
{
  OdRxObject* pRxDb = m_pTabConsole->getRxDatabase();
  OdExtDbModule* pExtDb = NULL;
  if (OdDbDatabase::cast(pRxDb).isNull())
    pExtDb = getIApp()->getExtDbModule(pRxDb);
  if (pExtDb)
  {
    pExtDb->dragStatus(pRxDb, pEntity, st);
    return; 
  }
  OdDbEntity::cast(pEntity)->dragStatus(st);
}

bool OdqGripManager::isModel(OdGiDrawable* pEntity)
{
  OdRxObject* pRxDb = m_pTabConsole->getRxDatabase();
  OdDbDatabase* pDwgDb = OdDbDatabase::cast(pRxDb).get();
  if (pDwgDb)
    return pDwgDb->getTILEMODE();
  OdExtDbModule* pExtDb = getIApp()->getExtDbModule(pRxDb);
  ODA_ASSERT_ONCE(pExtDb);
  if (pExtDb)
    return pExtDb->isModel(pRxDb, pEntity);
  return true;
}

static void setDrawDraggingProp(OdRxObject* pRxDb, bool bVal)
{
  IqViewSystem* iVS = getIViewSystem();
  if (!iVS)
    return;
  QList<IqView*> lst = iVS->getOpenViews(pRxDb);
  foreach (IqView* iView, lst)
  {
    ODA_ASSERT_ONCE(iView);
    if (!iView)
      continue;
    OdGsDevice* pDevice = iView->getGsDevice();
    ODA_ASSERT_ONCE(pDevice);
    if (!pDevice)
      continue;
    OdRxDictionaryPtr pProps = pDevice->properties();
    ODA_ASSERT_ONCE(pProps.get());
    if (pProps.isNull())
      continue;
    if (!pProps->has(L"DrawDragging"))
      return;
    pProps->putAt(L"DrawDragging", OdRxVariantValue(bVal));
  }
}

void OdqGripManager::draggingStarted()
{
  setDrawDraggingProp(m_pTabConsole->getRxDatabase(), true);
}

void OdqGripManager::draggingStopped()
{
  setDrawDraggingProp(m_pTabConsole->getRxDatabase(), false);
}

// Menu animation flags
//#ifndef TPM_VERPOSANIMATION
//static const UINT TPM_VERPOSANIMATION = 0x1000L;
//#endif
//#ifndef TPM_NOANIMATION
//static const UINT TPM_NOANIMATION = 0x4000L;
//#endif
//
//
//bool OdqGripManager::handleMappedRtClk(OdExGripDataPtrArray &aActiveKeys, int x, int y)
//{
//  OdUInt32 iSize = aActiveKeys.size();
//  int rtClkIndex = -1;
//  for (OdUInt32 i = 0; i < iSize; i++)
//  {
//    if (!aActiveKeys[i]->data().isNull() && 0 != aActiveKeys[i]->data()->rtClk()
//      && aActiveKeys[i]->data()->mapGripHotToRtClk() && !aActiveKeys[i]->isShared())
//    {
//      rtClkIndex = i;
//      break;
//    }
//  }
//  if (rtClkIndex != -1)
//  {
//    OdDbStubPtrArray ents;
//    OdDbGripDataArray hotGrips;
//    for (OdUInt32 i = 0; i < iSize; i++)
//    {
//      hotGrips.append(*aActiveKeys[i]->data());
//      if (!ents.contains(aActiveKeys[i]->id()))
//        ents.append(aActiveKeys[i]->id());
//    }
//    OdString menuName;
//    ODHMENU menu = 0;
//    ContextMenuItemIndexPtr cb = 0;
//    OdResult eRet = (*aActiveKeys[rtClkIndex]->data()->rtClk())(hotGrips, ents, menuName, menu, cb);
//    if (eRet == eOk && menu != 0 && cb != 0)
//    {
//      HWND wnd = ::GetActiveWindow();
//      POINT pt = {x, y};
//      ::ClientToScreen(wnd, &pt);
//      (*cb)(::TrackPopupMenu((HMENU)menu, 
//        TPM_LEFTALIGN|TPM_TOPALIGN|TPM_NONOTIFY|TPM_RETURNCMD|TPM_LEFTBUTTON|TPM_NOANIMATION, pt.x, pt.y, 0, wnd, 0));
//      ::DestroyMenu((HMENU)menu);
//      for (OdUInt32 i = 0; i < iSize; i++)
//        aActiveKeys[i]->setStatus(OdDbGripOperations::kWarmGrip);
//      updateEntityGrips(aActiveKeys[rtClkIndex]->id());
//      return true;
//    }
//  }
//  return false;
//}
