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
#include "BooleanCommand.h"

#include "DbDatabase.h"
#include "DbEntity.h"
#include "DbCommandContext.h"
#include "DbSSet.h"
#include "DbBlockTableRecord.h"
#include "DbBlockReference.h"
#include "DbHostAppServices.h"
#include "Ge/GeScale3d.h"
#include "DbErrorInvalidSysvar.h"
#include "SysVarInfo.h"
#include "StaticRxObject.h"
#include "DbUserIO.h"
#include "DbRegion.h"
#include "DbBody.h"
#include "Db3dSolid.h"
#include "DbSurface.h"
#include "Br/BrBrep.h"
#include "DbViewport.h"

class BooleanFilter : public OdStaticRxObject<OdEdSSetTracker>
{
public:
  bool allowDbBody;
  BooleanFilter()
    : allowDbBody(false)
  {}

  int addDrawables(OdGsView* ) { return 0; }
  void removeDrawables(OdGsView* ) { }

  bool check(const OdDbObjectId& entId)
  {
    OdDbEntityPtr pEnt = entId.openObject();
    return !pEnt.isNull() && (pEnt->isKindOf(OdDbRegion::desc()) || pEnt->isKindOf(OdDb3dSolid::desc()) ||
      pEnt->isKindOf(OdDbSurface::desc()) || allowDbBody && pEnt->isKindOf(OdDbBody::desc()));
  }

  bool append(const OdDbObjectId& entId, const OdDbSelectionMethod* )
  {
    return check(entId);
  }

  bool remove(const OdDbObjectId& entId, const OdDbSelectionMethod* )
  {
    return check(entId);
  }
};


void UnionCommand::execute(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbUserIOPtr pIO = pDbCmdCtx->dbUserIO();

  BooleanFilter filter;
  OdDbSelectionSetPtr pSet = pIO->select(OD_T("Select objects:"), OdEd::kSelAllowEmpty, 0, OdString::kEmpty, &filter);
  
  if (pSet->numEntities() >= 2)
  {
    OdDbSelectionSetIteratorPtr pIter = pSet->newIterator();
    OdDbRegionPtr pMainReg;
    OdDb3dSolidPtr pMainSolid;
    OdDbSurfacePtr pMainSurf, pMainRet;
    while ( !pIter->done() )
    {
      OdDbObjectId objId  = pIter->objectId();
      OdDbEntityPtr pEnt  = objId.openObject( OdDb::kForWrite );
      if( !pEnt.isNull())
      {
        if(pEnt->isKindOf(OdDb3dSolid::desc()))
        {
          if (pMainSolid.isNull())
            pMainSolid = OdDb3dSolidPtr(pEnt);
          else
          {
            pMainSolid->booleanOper(OdDb::kBoolUnite, OdDb3dSolidPtr(pEnt));
            pEnt->erase();
          }
        }
        if(pEnt->isKindOf(OdDbRegion::desc()))
        {
          if (pMainReg.isNull())
            pMainReg = OdDbRegionPtr(pEnt);
          else
          {
            pMainReg->booleanOper(OdDb::kBoolUnite, OdDbRegionPtr(pEnt));
            pEnt->erase();
          }
        }
        if(pEnt->isKindOf(OdDbSurface::desc()))
        {
          if (pMainSurf.isNull())
            pMainSurf = OdDbSurfacePtr(pEnt);
          else
          {
            OdDbDatabase* pDatabase = pDbCmdCtx->database();
            OdDbBlockTableRecordPtr pMs = pDatabase->getModelSpaceId().safeOpenObject(OdDb::kForWrite);

            pMainSurf->booleanUnion(OdDbSurfacePtr(pEnt), pMainRet);
            pMs->appendOdDbEntity(pMainRet);
            pEnt->erase();
            pMainSurf->erase();
          }
        }
      }
      pIter->next();
    }
  }
}

void IntersectSurfCommand::execute(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbUserIOPtr pIO = pDbCmdCtx->dbUserIO();
  OdDbDatabase* pDatabase = pDbCmdCtx->database();
  OdDbBlockTableRecordPtr pMs = pDatabase->getModelSpaceId().safeOpenObject(OdDb::kForWrite);

  BooleanFilter filter;
  OdDbSelectionSetPtr pSet = pIO->select(OD_T("Select surface:"), OdEd::kSelAllowEmpty, 0, OdString::kEmpty, &filter);
  OdDbSelectionSetPtr pSet2 = pIO->select(OD_T("Select surfase or 3dsolid:"), OdEd::kSelAllowEmpty, 0, OdString::kEmpty, &filter);
  
  if (pSet->numEntities() < 2 && pSet2->numEntities() < 2)
  {
    OdDbSelectionSetIteratorPtr pIter = pSet->newIterator();
    OdDbSelectionSetIteratorPtr pIter2 = pSet2->newIterator();

    OdResult res = eInvalidInput;
    if ( pIter->done() || pIter2->done())
      return;

    
    OdDbObjectId objId  = pIter->objectId();
    OdDbEntityPtr pEnt  = objId.openObject( OdDb::kForWrite );
    OdDbObjectId objId2  = pIter2->objectId();
    OdDbEntityPtr pEnt2  = objId2.openObject( OdDb::kForWrite );

    if( !pEnt.isNull() && !pEnt2.isNull() && pEnt->isKindOf(OdDbSurface::desc()) && (pEnt2->isKindOf(OdDbSurface::desc()) || pEnt2->isKindOf(OdDb3dSolid::desc())))
    {
      OdDbEntityPtrArray entArr;
      if(pEnt2->isKindOf(OdDb3dSolid::desc()))
        res = OdDbSurfacePtr(pEnt)->booleanIntersect(OdDb3dSolidPtr(pEnt2), entArr);
      if(pEnt2->isKindOf(OdDbSurface::desc()))
        res = OdDbSurfacePtr(pEnt)->booleanIntersect(OdDbSurfacePtr(pEnt2), entArr);
      else
        res = OdDbSurfacePtr(pEnt)->createInterferenceObjects(entArr, pEnt2, 0);

      int size = entArr.length();
      for (int i = 0; i < size; ++i)
      {
        pMs->appendOdDbEntity(entArr[i]);
      }
    }
  }
}

void IntersectCommand::execute(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbUserIOPtr pIO = pDbCmdCtx->dbUserIO();

  BooleanFilter filter;
  OdDbSelectionSetPtr pSet = pIO->select(OD_T("Select objects:"), OdEd::kSelAllowEmpty, 0, OdString::kEmpty, &filter);

  if (pSet->numEntities() >= 2)
  {
    OdDbSelectionSetIteratorPtr pIter = pSet->newIterator();
    OdDbRegionPtr pMainReg;
    OdDb3dSolidPtr pMainSolid;
    OdResult res = eOk;
    while (!pIter->done() && eOk == res)
    {
      OdDbObjectId objId = pIter->objectId();
      OdDbEntityPtr pEnt = objId.openObject(OdDb::kForWrite);
      if (!pEnt.isNull())
      {
        if (pEnt->isKindOf(OdDb3dSolid::desc()))
        {
          if (pMainSolid.isNull())
            pMainSolid = OdDb3dSolidPtr(pEnt);
          else
          {
            if (!pMainSolid->isNull())
              pMainSolid->booleanOper(OdDb::kBoolIntersect, OdDb3dSolidPtr(pEnt));
            pEnt->erase();
          }
        }
        if (pEnt->isKindOf(OdDbRegion::desc()))
        {
          if (pMainReg.isNull())
            pMainReg = OdDbRegionPtr(pEnt);
          else
          {
            if (!pMainReg->isNull())
              pMainReg->booleanOper(OdDb::kBoolIntersect, OdDbRegionPtr(pEnt));
            pEnt->erase();
          }
        }
      }
      pIter->next();
    }
  }
}

OdDbObjectId spaUnionActiveBlockTableRecord(const OdDbDatabase* pDb)
{
  OdDbObjectId idSpace = pDb->getActiveLayoutBTRId();
  if (idSpace == pDb->getPaperSpaceId())
  {
    OdDbViewportPtr pVp = pDb->activeViewportId().safeOpenObject();
    if (pVp->number() > 1)
      idSpace = pDb->getModelSpaceId();
  }
  return idSpace;
}

class BooleanExTracker : public OdStaticRxObject<OdEdPointTracker>
{
public:

  OdDbDatabasePtr m_pDb;
  OdGsView       *m_pGsView;

  OdDbEntityPtr     m_BooleanResult;
  OdDbEntityPtr     m_pEnt1;
  OdDbEntityPtr     m_pEnt2;
  OdGePoint3d        m_Ent2Point;
  OdDb::BoolOperType m_operation;

  OdGePoint3d getEnt2Point() const
  {
    OdGeExtents3d extents;
    m_pEnt2->getGeomExtents(extents);
    return extents.isValidExtents() ? extents.center() : OdGePoint3d::kOrigin;
  }

public:
  BooleanExTracker(OdDbDatabasePtr pDb, OdDbEntityPtr pEnt1, OdDbEntityPtr pEnt2, OdDb::BoolOperType operation)
    : m_pDb(pDb)
    , m_pGsView(NULL)
    , m_operation(operation)
  {
    if (pEnt1.isNull() || pEnt2.isNull()) return;

    m_pEnt1 = pEnt1->clone();
    m_pEnt2 = pEnt2->clone();

    m_Ent2Point = getEnt2Point();
  }

  void setValue(const OdGePoint3d& pt) ODRX_OVERRIDE
  {
    if (m_pEnt1.isNull()) return;

    if (m_pGsView)
    {
      removeDrawables(m_pGsView);
    }

    m_BooleanResult = m_pEnt1->clone();

    OdGePoint3d   basePoint = getEnt2Point();

    OdGeVector3d v = pt - basePoint;
    OdGeMatrix3d mx = OdGeMatrix3d::translation(v);
    m_pEnt2->transformBy(mx);
    OdResult res;
    OdDbEntityPtr ptmp = m_pEnt2->clone();
    if (m_BooleanResult->isKindOf(OdDb3dSolid::desc()) && ptmp->isKindOf(OdDb3dSolid::desc()))
    {
      res = ((OdDb3dSolidPtr)m_BooleanResult)->booleanOper(m_operation, (OdDb3dSolidPtr)(ptmp));
    }
    else if (m_BooleanResult->isKindOf(OdDbRegion::desc()) && ptmp->isKindOf(OdDbRegion::desc()))
    {
      res = ((OdDbRegionPtr)m_BooleanResult)->booleanOper(m_operation, (OdDbRegionPtr)(ptmp));
    }
    else
      return;
    if (res == eOk)
    {
      OdGeExtents3d ext;
      m_BooleanResult->getGeomExtents(ext);
      if (ext.isValidExtents())
      {
        m_BooleanResult->setColorIndex(4);
      }
      else
      {
        m_BooleanResult = OdDb3dSolidPtr();
      }
    }
    else
    {
      m_BooleanResult = OdDb3dSolidPtr();
    }

    if (m_pGsView)
    {
      removeDrawables(m_pGsView);
      addDrawables(m_pGsView);
    }
  }

  //
  int addDrawables(OdGsView* pView) ODRX_OVERRIDE
  {
    m_pGsView = pView;


    if (!m_pEnt2.isNull())
    {
      pView->add(m_pEnt2, 0);
    }

    if (!m_BooleanResult.isNull())
    {
      pView->add(m_BooleanResult, 0);
    }

    return 1;
  }

  void removeDrawables(OdGsView* pView) ODRX_OVERRIDE
  {
    if (!pView) return;

    // do not reset m_pGsView, need little later
    if (!m_BooleanResult.isNull()) pView->erase(m_BooleanResult);
    if (!m_pEnt2.isNull()) pView->erase(m_pEnt2);
  }

  void invalidate()
  {
    if (!m_pGsView) return;

    OdGsDevice* pDevice = m_pGsView->device();
    if (pDevice)
    {
      pDevice->invalidate();
      pDevice->update();
    }
  }
};

void IntersectExCommand::execute(OdEdCommandContext* pCmdCtx, OdDb::BoolOperType operationType)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();

  OdDbObjectIdArray objectIdsArray = OdDbSelectionSet::cast(pIO->select(L"", OdEd::kSelAllowObjects | OdEd::kSelAllowInactSpaces).get())->objectIdArray();

  if (objectIdsArray.size() < 2)
  {
    pIO->putString(OD_T("Two entities should be selected."));
    return;
  }

  OdDbEntityPtr pEnt1 = objectIdsArray[0].openObject(OdDb::kForWrite);
  OdDbEntityPtr pEnt2 = objectIdsArray[1].openObject(OdDb::kForWrite);

  {
    BooleanExTracker heightTracker(pDb, pEnt1, pEnt2, operationType);

    try
    {
      OdGePoint3d point = pIO->getPoint(OD_T("Move second entity."),
        OdEd::kGptNoUCS | OdEd::kInpThrowOther | OdEd::kInpThrowEmpty,
        0, OD_T(""), &heightTracker);
    }
    catch (const OdEdKeyword& )
    {
      heightTracker.invalidate();
    }
    catch (const OdEdOtherInput& other)
    {
      heightTracker.invalidate();
      if (other.string().isEmpty()) return;
    }
    catch (const OdEdCancel& )
    {
      pIO->putString(OD_T("Canceled."));
      return;
    }

    pEnt1->erase();
    pEnt2->erase();

    if (!heightTracker.m_BooleanResult.isNull())
    {
      OdDbBlockTableRecordPtr pSpace = spaUnionActiveBlockTableRecord(pDb).openObject(OdDb::kForWrite);
      pSpace->appendOdDbEntity(heightTracker.m_BooleanResult);
    }
  }
}

namespace{
  template <class tEntPtr, class tEnt> void unionEnt(OdDbSelectionSetPtr pSet, tEntPtr & pMain, tEntPtr & pWork)
  {
    OdResult res = eOk;
    OdDbSelectionSetIteratorPtr pIter = pSet->newIterator();
    while ( !pIter->done() )
    {
      OdDbObjectId objId  = pIter->objectId();
      OdDbEntityPtr pEnt  = objId.openObject( OdDb::kForWrite );
      if( !pEnt.isNull())
      {
        if(pEnt->isKindOf(tEnt::desc()))
        {
          if (pMain.isNull())
          {
            pWork = tEnt::createObject();
            pWork->copyFrom(pEnt);
            pMain = tEntPtr(pEnt);
          }
          else
          {
            res = pWork->booleanOper(OdDb::kBoolUnite, tEntPtr(pEnt));
          }
        }
      }
      pIter->next();
    }
  }

  template <class tEntPtr> void clearEnt(OdDbSelectionSetPtr pSet, OdDbObjectId pObjId)
  {
    OdDbSelectionSetIteratorPtr pIter = pSet->newIterator();
    while ( !pIter->done() )
    {
      OdDbObjectId objId  = pIter->objectId();
      OdDbEntityPtr pEnt  = objId.openObject( OdDb::kForWrite );
      if( !pEnt.isNull())
      {
        if(pEnt->isKindOf(tEntPtr::desc()))
        {
          if (pObjId != pEnt->objectId())
          {
            pEnt->erase();
          }
        }
      }
      pIter->next();
    }
  }
}


void SubtractCommand::execute(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);           
  OdDbUserIOPtr pIO = pDbCmdCtx->dbUserIO();

  BooleanFilter filter;
  OdDbSelectionSetPtr pSet = pIO->select(OD_T("Select objects:"), OdEd::kSelAllowEmpty, 0, OdString::kEmpty, &filter);
  OdDbSelectionSetPtr pSet2 = pIO->select(OD_T("Select objects:"), OdEd::kSelAllowEmpty, 0, OdString::kEmpty, &filter);

  if (pSet->numEntities() >= 1 && pSet2->numEntities() >= 1)
  {
    OdDbSelectionSetIteratorPtr pIter = pSet->newIterator();
    while ( !pIter->done())
    {
      OdDbObjectId objId  = pIter->objectId();
      OdDbSelectionSetIteratorPtr pIter2 = pSet2->newIterator();
      while ( !pIter2->done())
      {
        if (objId == pIter2->objectId())
        {
          pSet2->remove(pIter2->objectId());
          break;
        }
        pIter2->next();
      }
      pIter->next();
    }
  }

  if (pSet->numEntities() >= 1 && pSet2->numEntities() >= 1)
  {
    OdResult res = eOk;

    OdDb3dSolidPtr pMainSolid, pMainSolid2, pWorkSolid, pWorkSolid2;
    unionEnt<OdDb3dSolidPtr, OdDb3dSolid>(pSet, pMainSolid, pWorkSolid);
    unionEnt<OdDb3dSolidPtr, OdDb3dSolid>(pSet2, pMainSolid2, pWorkSolid2);
    if (!pWorkSolid.isNull() && !pWorkSolid2.isNull())
    {
      res = pWorkSolid->booleanOper(OdDb::kBoolSubtract, pWorkSolid2);
      pMainSolid->copyFrom(pWorkSolid);
      clearEnt<OdDb3dSolid>(pSet, pMainSolid->objectId());
      clearEnt<OdDb3dSolid>(pSet2, pMainSolid2->objectId());
      pMainSolid2->erase();
    }

    if (eOk == res)
    {
      OdDbRegionPtr pMainReg, pMainReg2, pWorkReg, pWorkReg2;
      unionEnt<OdDbRegionPtr, OdDbRegion>(pSet, pMainReg, pWorkReg);
      unionEnt<OdDbRegionPtr, OdDbRegion>(pSet2, pMainReg2, pWorkReg2);
      if (!pMainReg.isNull() && !pMainReg2.isNull())
      {
        pWorkReg->booleanOper(OdDb::kBoolSubtract, pWorkReg2);
        pMainReg->copyFrom(pWorkReg);
        clearEnt<OdDbRegion>(pSet, pMainReg->objectId());
        clearEnt<OdDbRegion>(pSet2, pMainReg2->objectId());
        pMainReg2->erase();
      }
    }

    if (eOk == res)
    {
      OdDbDatabase* pDatabase = pDbCmdCtx->database();
      OdDbBlockTableRecordPtr pMs = pDatabase->getModelSpaceId().safeOpenObject(OdDb::kForWrite);

      OdResult res = eOk;
      OdDbSurfacePtr pRetSurf;
      OdDbSelectionSetIteratorPtr pIter = pSet->newIterator();
      while ( !pIter->done() )
      {
        OdDbObjectId objId  = pIter->objectId();
        OdDbEntityPtr pEnt  = objId.openObject( OdDb::kForWrite );
        if( !pEnt.isNull() && pEnt->isKindOf(OdDbSurface::desc()))
        {
          OdDbSelectionSetIteratorPtr pIter2 = pSet2->newIterator();
          while ( !pIter2->done() )
          {
            OdDbObjectId objId2  = pIter2->objectId();
            OdDbEntityPtr pEnt2  = objId2.openObject( OdDb::kForWrite );
            if( !pEnt2.isNull())
            {
              res = eInvalidInput;
              if(pEnt2->isKindOf(OdDbSurface::desc()) )
                res = OdDbSurfacePtr(pEnt)->booleanSubtract(OdDbSurfacePtr(pEnt2), pRetSurf);
              if(pEnt2->isKindOf(OdDb3dSolid::desc()) )
                res = OdDbSurfacePtr(pEnt)->booleanSubtract(OdDb3dSolidPtr(pEnt2), pRetSurf);

              if (eOk == res && !pRetSurf.isNull())
              {
                pMs->appendOdDbEntity(pRetSurf);
              }
            }
            pIter2->next();
          }
        }
        pIter->next();
      }
    }
  }
}

void SculptCommand::execute(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbUserIOPtr pIO = pDbCmdCtx->dbUserIO();

  BooleanFilter filter;
  filter.allowDbBody = true;
  OdDbSelectionSetPtr pSet = pIO->select(OD_T("Select objects:"), OdEd::kSelAllowEmpty, 0, OdString::kEmpty, &filter);

  //ACAD 2020 doesn't perform sculpt command for a single solid with "Solid creation failed, no watertight volume detected."
  //However, this message is incorrect. Also createSculptedSolid makes sense and may be non trivial, e.g., for a solid with holes.
  if (pSet->numEntities() >= 1)
  {
    OdDbEntityPtrArray entities;
    OdDbSelectionSetIteratorPtr pIter = pSet->newIterator();
    while (!pIter->done())
    {
      OdDbObjectId objId = pIter->objectId();
      OdDbEntityPtr pEnt = objId.openObject(OdDb::kForWrite);
      //already filtered in BooleanFilter
      ODA_ASSERT(!pEnt.isNull() && (pEnt->isKindOf(OdDb3dSolid::desc()) || pEnt->isKindOf(OdDbRegion::desc()) ||
        pEnt->isKindOf(OdDbSurface::desc()) || pEnt->isKindOf(OdDbBody::desc())));
      entities.append(pEnt);
      pIter->next();
    }
    if (!entities.empty())
    {
      OdDb3dSolidPtr sculptSolid = OdDb3dSolid::createObject();
      OdGeIntArray limitingFlags;
      OdResult res = sculptSolid->createSculptedSolid(entities, limitingFlags);
      if (res != eOk)
      {
        pIO->putString(OD_T("Solid creation failed, no watertight volume created."));
        return;
      }

      for (unsigned entityIdx = 0; entityIdx < entities.size(); ++entityIdx)
      {
        entities[entityIdx]->erase();
      }
      OdDbDatabase* pDatabase = pDbCmdCtx->database();
      OdDbBlockTableRecordPtr pMs = pDatabase->getModelSpaceId().safeOpenObject(OdDb::kForWrite);
      pMs->appendOdDbEntity(sculptSolid);
    }
  }
}
