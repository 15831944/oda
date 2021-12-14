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
#include "ExSweepCommand.h"

#include "DbHostAppServices.h"
#include "RxDynamicModule.h"
#include "StaticRxObject.h"

#include "OdErrorContext.h"
#include "Ed/EdUserIO.h"
#include "Gs/Gs.h"
#include "DbCommandContext.h"
#include "DbUnitsFormatter.h"

#include "DbRegion.h"
#include "Db3dSolid.h"
#include "DbExtrudedSurface.h"
#include "DbSweptSurface.h"
#include "DbSweepOptions.h"
#include "DbBlockTableRecord.h"
#include "DbCurve.h"
#include "DbLine.h"
#include "DbSpline.h"

#include "Ge/GeLineSeg3d.h"
#include "Ge/GeCircArc3d.h"
#include "Ge/GeNurbCurve3d.h"
#include "Ge/GeCompositeCurve3d.h"

#define STL_USING_LIMITS
#include "OdaSTL.h"

struct SweepPointTracker : public OdStaticRxObject<OdEdPointTracker>
{
  // Db entities
  OdDb3dSolidPtr m_ent;
  OdDbEntityPtr  m_base;
  OdDbCurvePtr   m_originalPath;

  // Ge cache
  OdGeInterval     m_Interval;
  OdGeCurve3dPtr   m_originalPathGe;
  OdGeCurve3dPtr   m_splinePathGe;
  OdGeDoubleArray  m_sampleParams;
  OdGePoint3dArray m_samplePts;
  OdInt32          m_curBestIndex;

  // Sweep options
  OdDbSweepOptions m_sweepParams;

  // View
  OdGsView* m_pGsView;

public:

  SweepPointTracker(OdDb3dSolidPtr entity, OdDbEntityPtr base, OdDbCurvePtr path, OdDbSweepOptions options)
    : m_ent(entity), m_base(base), m_originalPath(path), m_sweepParams(options)
  {
    // Get Ge curve
    OdGeCurve3d* geCurve;
    m_originalPath->getOdGeCurve(geCurve);
    m_originalPathGe = geCurve;

    // Init Ge cache
    m_splinePathGe = OdGeNurbCurve3d::convertFrom(m_originalPathGe);
    m_originalPathGe->getInterval(m_Interval);
    m_originalPathGe->getSamplePoints(100, m_samplePts, m_sampleParams);
    if (m_originalPathGe->isClosed())
    {
      OdGePoint3d endPt;
      m_originalPathGe->hasEndPoint(endPt);
      m_samplePts.push_back(endPt);
      m_sampleParams.push_back(m_Interval.upperBound());
    }

    // Unknown best point index
    m_curBestIndex = -1;
  }

  OdDb3dSolidPtr getSolid()
  {
    return m_ent;
  }

  OdResult updateSolid(const OdGePoint3d& value)
  {
    OdResult res = eOk;
    double param;

    // Find nearest parameter on path to user point
    if (!m_originalPathGe->isOn(value) && NULL != m_pGsView)
    {
      // Point of view unbounded normal
      OdGeLine3d line(value, m_pGsView->target() - m_pGsView->position());

      // Find best candidate from sample points
      double minDist = std::numeric_limits<double>::max();
      unsigned int iBest = 0;
      for (unsigned int i = 0; i < m_samplePts.length(); ++i)
      {
        double curDist = line.distanceTo(m_samplePts[i]);
        if (curDist < minDist)
        {
          minDist = curDist;
          iBest = i;
        }
      }

      // Near end "hack"
      if (m_originalPathGe->isClosed() && iBest < 2)
      {
        iBest = m_samplePts.length() - 1;
      }

      // Nothing to: solid is in actual state
      if (iBest == m_curBestIndex)
      {
        return res;
      }

      m_curBestIndex = iBest;

      param = m_sampleParams[iBest];
    }
    else
    {
      // UserPoint on the path
      OdGePoint3d endPt;
      if (m_samplePts.last().isEqualTo(value))
      {
        param = m_Interval.upperBound();
      }
      else
      {
        param = m_originalPathGe->paramOf(value);
      }
    }

    // Split path by param
    OdGeCurve3dPtr pResultGeCurve;
    if (m_originalPathGe->isKindOf(OdGe::kNurbCurve3d))
    {
      OdGeCurve3d* p1, * p2;
      m_originalPathGe->getSplitCurves(param, p1, p2);
      if (NULL == p1)
      {
        return eInvalidInput;
      }
      pResultGeCurve = p1;
      OdGeCurve3dPtr pSecond = p2;
    }
    else if (m_originalPathGe->isKindOf(OdGe::kCompositeCrv3d)) // Split manually
    {
      OdGeCompositeCurve3d* pCompCur = static_cast<OdGeCompositeCurve3d*>(m_originalPathGe.get());

      int iCur;
      double dLocalParam = pCompCur->globalToLocalParam(param, iCur);

      OdGeCurve3dPtrArray aCurvesPtrs;
      pCompCur->getCurveList(aCurvesPtrs);

      OdGeCurve3dPtrArray aNewCurves = OdGeCurve3dPtrArray::create(aCurvesPtrs.asArrayPtr(), iCur + 1);

      aNewCurves[iCur] = (OdGeCurve3d*)aNewCurves[iCur]->copy();

      OdGeInterval iv;
      aNewCurves.last()->getInterval(iv);
      iv.setUpper(dLocalParam);
      aNewCurves.last()->setInterval(iv);

      if (OdZero(iv.length()))
      {
        aNewCurves.removeLast();
      }

      if (aNewCurves.isEmpty())
      {
        m_ent->cleanBody();
        return eInvalidInput;
      }

      pResultGeCurve = new OdGeCompositeCurve3d(aNewCurves);
    }
    else
    {
      pResultGeCurve = (OdGeCurve3d*)m_originalPathGe->copy();
      OdGeInterval iv = m_Interval;

      if (OdEqual(m_Interval.upperBound(), param))
      {
        pResultGeCurve = m_originalPathGe;
      }
      else if (OdEqual(m_Interval.lowerBound(), param))
      {
        return eInvalidInput;
      }
      else
      {
        iv.setUpper(param);
        pResultGeCurve->setInterval(iv);
      }
    }

    // Create new DbCurve 
    OdDbCurve* dbCrv;
    res = OdDbCurve::createFromOdGeCurve(*pResultGeCurve, dbCrv);
    OdDbCurvePtr pNewDbCrv;
    pNewDbCrv.attach(dbCrv);

    if (eOk != res)
    {
      return res;
    }

    // Sweep
    res = m_ent->createSweptSolid(m_base, pNewDbCrv, m_sweepParams);
    if (eOk != res)
    {
      return res;
    }

    return res;
  }

  // Real-time listener method
  virtual void setValue(const OdGePoint3d& value) ODRX_OVERRIDE
  {
    updateSolid(value);
  }

  virtual int addDrawables(OdGsView* pView)
  {
    m_pGsView = pView;
    pView->add(m_ent, 0);
    return 1;
  }

  virtual void removeDrawables(OdGsView* pView)
  {
    pView->erase(m_ent);
  }

};

const OdString ExSweepCommand::groupName() const
{
  return OD_T("MODELER GEOMETRY");
}

const OdString ExSweepCommand::globalName() const
{
  return OD_T("sweep_ex");
}

void ExSweepCommand::execute(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabase* pDatabase = pDbCmdCtx->database();
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();
  //   OdDbBlockTableRecordPtr pMs = pDatabase->getModelSpaceId().safeOpenObject(OdDb::kForWrite);
  OdDbSelectionSetPtr pFirstSel = pIO->select(OD_T("Select objects to sweep"), OdEd::kSelSingleEntity/*, 0, OdString::kEmpty, &filter*/);

  if (pFirstSel->numEntities() == 0)
  {
    pIO->putString(OD_T("Nothing selected."));
    return;
  }

  OdGePoint3d ptBase = OdGePoint3d::kOrigin;
  ptBase = pIO->getPoint(OD_T("Specify base point"),
    OdEd::kGptNoUCS, &ptBase);

  OdDbSelectionSetPtr pSecondSel = pIO->select(OD_T("Select sweep path"), OdEd::kSelSingleEntity/*, 0, OdString::kEmpty, &filter*/);

  if (pSecondSel->numEntities() == 0)
  {
    pIO->putString(OD_T("Nothing selected."));
    return;
  }
  OdDbSelectionSetIteratorPtr pIter = pFirstSel->newIterator();
  bool notBrack = true;
  while (!pIter->done() && notBrack)
  {
    OdDbObjectId objId = pIter->objectId();
    OdDbEntityPtr pEnt = objId.openObject();
    OdDbSweepOptions sweep;
    OdDb::Planarity planarity;
    OdGePoint3d pnt;
    OdGeVector3d vec;
    bool closed;
    double approxArcLen;

    if (!pEnt.isNull() && sweep.checkSweepCurve(pEnt, planarity, pnt, vec, closed, approxArcLen) == eOk)
    {
      OdDbSelectionSetIteratorPtr pSecondIter = pSecondSel->newIterator();
      while (!pSecondIter->done() && pIter != pSecondIter)
      {
        objId = pSecondIter->objectId();
        OdDbEntityPtr pSecondEnt = objId.openObject();
        if (!pSecondEnt.isNull() && sweep.checkPathCurve(pSecondEnt) == eOk)
        {
          //ARX 2016 twistAngle is 0
//           sweep.setTwistAngle(2);
//           OdGeVector3d vec1;
//           vec1.set(3, 1, 0);
//           sweep.setTwistRefVec(vec1);
//           sweep.setDraftAngle(2);
          sweep.setAlign(OdDbSweepOptions::kAlignSweepEntityToPath);
          sweep.setBasePoint(ptBase);

          OdDb3dSolidPtr pSolid = OdDb3dSolid::createObject();
          pSolid->setPropertiesFrom(pEnt);
          SweepPointTracker tracker(pSolid, pEnt, pSecondEnt, sweep);

          OdGePoint3d point = pIO->getPoint(OD_T("Specify end point"),
            OdEd::kGptNoUCS | OdEd::kInpThrowOther | OdEd::kInpThrowEmpty,
            0, OdString::kEmpty, &tracker);

          OdResult res = tracker.updateSolid(point);
          ODA_ASSERT(res == eOk);
          if (res == eOk)
          {
            OdDbBlockTableRecordPtr pMs = pDatabase->getModelSpaceId().safeOpenObject(OdDb::kForWrite);
            pMs->appendOdDbEntity(tracker.getSolid());
            pIO->putString(OD_T("Sweep entity created successfully!"));
          }
          else
          {
            pIO->putString(OD_T("Sweep error!"));
          }
          notBrack = false;
          break;
        }
        pSecondIter->next();
      }
    }
    pIter->next();
  }
}
