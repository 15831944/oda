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
#include "StdAfx.h"

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
#include "DbPlaneSurface.h"


class ExtrudePathFilter : public OdStaticRxObject<OdEdSSetTracker>
{
public:

  ExtrudePathFilter( )
  {}

  int addDrawables(OdGsView* ) { return 0; }
  void removeDrawables(OdGsView* ) { }

  bool check(const OdDbObjectId& entId)
  {
    OdDbEntityPtr pEnt = entId.openObject();
    return true;//pEnt->isKindOf(OdDbRegion::desc()) || pEnt->isKindOf(OdDb3dSolid::desc());
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

class ExtrudeFilter : public OdStaticRxObject<OdEdSSetTracker>
{
public:

  ExtrudeFilter( )
  {}

  int addDrawables(OdGsView* ) { return 0; }
  void removeDrawables(OdGsView* ) { }

  bool check(const OdDbObjectId& entId)
  {
    OdDbEntityPtr pEnt = entId.openObject();
    return true;//pEnt->isKindOf(OdDbRegion::desc()) || pEnt->isKindOf(OdDb3dSolid::desc());
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


//
static void addEntityToDb(OdDbDatabase* pDb, OdDbEntity* pEntity)
{
  OdDbBlockTableRecordPtr pMs = pDb->getModelSpaceId().safeOpenObject(OdDb::kForWrite);
  pMs->appendOdDbEntity(pEntity);
}

struct SolidCreator
{
  OdDb3dSolidPtr m_pSolid;
  OdDbSweepOptions m_options;

  SolidCreator(OdDbDatabase* pDb)
    : m_pSolid(OdDb3dSolid::createObject())
  {
    m_pSolid->setDatabaseDefaults(pDb);
  }

  //
  int addDrawables(OdGsView* pView)
  {
    pView->add(m_pSolid, 0);
    return 1;
  }
  void removeDrawables(OdGsView* pView)
  {
    if (!pView) return;
    pView->erase(m_pSolid);
  }

  //
  void setHeight(OdDbEntity* pEnt, const OdGeVector3d& dir, double height, double taper) {
    m_options.setDraftAngle(taper);
    OdResult res = m_pSolid->createExtrudedSolid(pEnt, dir * height, m_options);
    if (eOk != res && m_pSolid->body()) {
      m_pSolid->setBody(NULL);
    }
  }
  void setHeight(OdDbRegion* pRegion, const OdGeVector3d& /*dir*/, double height, double taper) {
    OdResult res = m_pSolid->extrude(pRegion, height, taper);
    if (eOk != res && m_pSolid->body()) {
      m_pSolid->setBody(NULL);
    }
  }

  //
  OdResult createEntity(OdDbDatabase* pDb, OdDbEntity* pEnt, const OdGeVector3d& dir, double taper)
  {
    m_options.setDraftAngle(taper);

    OdDb3dSolidPtr pNewSolid = OdDb3dSolid::createObject();
    pNewSolid->setDatabaseDefaults(pDb);

    OdResult res = pNewSolid->createExtrudedSolid(pEnt, dir, m_options);
    if (eOk != res) {
      return res;
    }

    addEntityToDb(pDb, pNewSolid);
    return eOk;
  }

  OdResult createEntity(OdDbDatabase* pDb, OdDbEntity* pEnt, const OdGeVector3d& dir, double height, double taper)
  {
    return createEntity(pDb, pEnt, dir * height, taper);
  }

  OdResult createEntity(OdDbDatabase* pDb, OdDbEntity* pEnt,
    const OdGePoint3d& ptStart, const OdGePoint3d& ptEnd, double taper)
  {
    return createEntity(pDb, pEnt, ptEnd - ptStart, taper);
  }

  OdResult createEntity(OdDbDatabase* pDb, OdDbEntity* pEnt, OdDbCurve* pCurve, double taper)
  {
    m_options.setAlign(OdDbSweepOptions::kTranslatePathToSweepEntity);
    m_options.setDraftAngle(taper);

    OdDb3dSolidPtr pNewSolid = OdDb3dSolid::createObject();
    pNewSolid->setDatabaseDefaults(pDb);

    OdResult res = pNewSolid->createSweptSolid(pEnt, pCurve, m_options);
    if (eOk != res) {
      return res;
    }

    addEntityToDb(pDb, pNewSolid);
    return eOk;
  }

  // special case for region
  OdResult createEntity(OdDbDatabase* pDb, OdDbRegion* pRegion, const OdGeVector3d& /*dir*/, double height, double taper)
  {
    OdDb3dSolidPtr pNewSolid = OdDb3dSolid::createObject();
    pNewSolid->setDatabaseDefaults(pDb);

    OdResult res = pNewSolid->extrude(pRegion, height, taper);
    if (eOk != res) {
      return res;
    }

    addEntityToDb(pDb, pNewSolid);
    return eOk;
  }
};
struct SurfaceCreator
{
  OdDbExtrudedSurfacePtr m_pSurface;
  OdDbSweepOptions m_options;

  SurfaceCreator(OdDbDatabase* pDb)
    : m_pSurface(OdDbExtrudedSurface::createObject())
  {
    m_pSurface->setDatabaseDefaults(pDb);
  }

  int addDrawables(OdGsView* pView)
  {
    pView->add(m_pSurface, 0);
    return 1;
  }
  void removeDrawables(OdGsView* pView)
  {
    if (!pView) return;
    pView->erase(m_pSurface);
  }

  void setHeight(OdDbEntity* pEnt, const OdGeVector3d& dir, double height, double taper) {
    m_options.setDraftAngle(taper);
    OdResult res = m_pSurface->createExtrudedSurface(pEnt, dir * height, m_options);
    if (eOk != res && m_pSurface->body()) {
      m_pSurface->setBody(NULL);
    }
  }

  //
  OdResult createEntity(OdDbDatabase* pDb, OdDbEntity* pEnt, const OdGeVector3d& dir, double taper)
  {
    m_options.setDraftAngle(taper);

    OdDbExtrudedSurfacePtr pNewSurface = OdDbExtrudedSurface::createObject();
    pNewSurface->setDatabaseDefaults(pDb);

    OdResult res = pNewSurface->createExtrudedSurface(pEnt, dir, m_options);
    if (eOk != res) {
      return res;
    }

    addEntityToDb(pDb, pNewSurface);
    return eOk;
  }

  OdResult createEntity(OdDbDatabase* pDb, OdDbEntity* pEnt, const OdGeVector3d& dir, double height, double taper)
  {
    return createEntity(pDb, pEnt, dir * height, taper);
  }

  OdResult createEntity(OdDbDatabase* pDb, OdDbEntity* pEnt,
    const OdGePoint3d& ptStart, const OdGePoint3d& ptEnd, double taper)
  {
    return createEntity(pDb, pEnt, ptEnd - ptStart, taper);
  }

  OdResult createEntity(OdDbDatabase* pDb, OdDbEntity* pEnt, OdDbCurve* pCurve, double taper)
  {
    m_options.setAlign(OdDbSweepOptions::kTranslatePathToSweepEntity);
    m_options.setDraftAngle(taper);

    OdDbSweptSurfacePtr pNewSurface = OdDbSweptSurface::createObject();
    pNewSurface->setDatabaseDefaults(pDb);

    OdResult res = pNewSurface->createSweptSurface(pEnt, pCurve, m_options);
    if (eOk != res) {
      return res;
    }

    addEntityToDb(pDb, pNewSurface);
    return eOk;
  }
};

class ExtrudeInit
{
  enum ExtrudeMode
  {
    eSolid = 0,
    eSurface = 1
  };

  OdDbDatabasePtr m_pDb;
  OdDbUserIOPtr m_pIO;

  ExtrudeMode m_mode;
  OdDbEntityPtr m_pEntity;

  template <class EntCreator>
  friend class ExtrudeBaseHelper;

public:
  ExtrudeInit(OdDbDatabasePtr pDb, OdDbUserIOPtr pIO)
    : m_pDb(pDb)
    , m_pIO(pIO)
    , m_mode(eSolid)
  {}

  void run();

private:
  template <template<class> class Helper>
  bool runMain();

  bool selectItemToExtrude();
  void changeCraetionMode();
  void forceSetupMode();

};
// ExtrudeConext


template <class EntCreator>
class ExtrudeBaseHelper : public OdStaticRxObject<OdEdPointTracker>
{
protected:
  OdDbDatabasePtr m_pDb;
  OdDbUserIOPtr m_pIO;

  EntCreator m_creator;
  OdGePoint3d m_basePoint;
  OdGeVector3d m_planeNormal;
  double m_taper;

  OdGsView* m_pGsView;

  void initBasePoint(const OdDbEntity* pEntity);
  void initPlane(const OdDbEntity* pEntity);

  virtual OdDbEntity* getEntity() = 0;

public:
  static OdGeTol tol;

  ExtrudeBaseHelper(ExtrudeInit& context);

private:
  double getDist(const OdGePoint3d& pt);
  void setTaper(double taper) { m_taper = taper; }

  // creation
  OdResult createEntity(const OdGePoint3d& ptStart, const OdGePoint3d& ptEnd)
  {
    return m_creator.createEntity(m_pDb, getEntity(), ptStart, ptEnd, m_taper);
  }
  OdResult createEntity(OdDbCurve* pCurve)
  {
    return m_creator.createEntity(m_pDb, getEntity(), pCurve, m_taper);
  }
  virtual OdResult createEntity(double height)
  {
    return m_creator.createEntity(m_pDb, getEntity(), m_planeNormal, height, m_taper);
  }
  //

  // real-time extrude
  virtual void setHeight(double height)
  {
    if (OdZero(height, tol.equalPoint())) return;
    m_creator.setHeight(getEntity(), m_planeNormal, height, m_taper);
  }

  int addDrawables(OdGsView* pView) ODRX_OVERRIDE
  {
    m_pGsView = pView;
    return m_creator.addDrawables(pView);
  }
  void removeDrawables(OdGsView* pView) ODRX_OVERRIDE
  {
    // do not reset m_pGsView, need little later
    m_creator.removeDrawables(pView);
  }
  void invalidate();
  void setValue(const OdGePoint3d& pt) ODRX_OVERRIDE
  {
    setHeight(getDist(pt));
  }
  //

public:
  void run();

private:
  void performCmdDirection();
  void performCmdPath();
  void performCmdSetTaper();
  void performCmdHeight(double dHeight);

};
template <class EntCreator>
/*static*/ OdGeTol ExtrudeBaseHelper<EntCreator>::tol(1e-6, 1e-10);
// ExtrudeBaseHelper


template <class EntCreator>
class ExtrudeRegion : public ExtrudeBaseHelper<EntCreator>
{
  OdDbRegionPtr m_pRegion;

  using ExtrudeBaseHelper<EntCreator>::m_pDb;
  using ExtrudeBaseHelper<EntCreator>::m_creator;
  using ExtrudeBaseHelper<EntCreator>::m_planeNormal;
  using ExtrudeBaseHelper<EntCreator>::m_taper;
  using ExtrudeBaseHelper<EntCreator>::tol;

public:
  typedef OdDbRegion EntType;

  ExtrudeRegion(ExtrudeInit& context, OdDbRegionPtr pRegion)
    : ExtrudeBaseHelper<EntCreator>(context)
    , m_pRegion(pRegion)
  {
    this->initBasePoint(m_pRegion);
    this->initPlane(m_pRegion);
  }

  OdDbEntity* getEntity() ODRX_OVERRIDE { return m_pRegion; }

  void setHeight(double height) ODRX_OVERRIDE
  {
    if (OdZero(height, tol.equalPoint())) return;
    m_creator.setHeight(m_pRegion, m_planeNormal, height, m_taper);
  }

  OdResult createEntity(double height) ODRX_OVERRIDE
  {
    return m_creator.createEntity(m_pDb, m_pRegion, m_planeNormal, height, m_taper);
  }
};
// ExtrudeRegion


template <class EntCreator>
class ExtrudeCurve : public ExtrudeBaseHelper<EntCreator>
{
  OdDbCurvePtr m_pCurve;

  using ExtrudeBaseHelper<EntCreator>::m_planeNormal;
  using ExtrudeBaseHelper<EntCreator>::tol;

public:
  typedef OdDbCurve EntType;

  ExtrudeCurve(ExtrudeInit& context, OdDbCurvePtr pCurve)
    : ExtrudeBaseHelper<EntCreator>(context)
    , m_pCurve(pCurve)
  {
    this->initBasePoint(m_pCurve);
    if (initPlaneFromDbLine(m_pCurve)) {
      return;
    }
    this->initPlane(m_pCurve);
  }

  OdDbEntity* getEntity() ODRX_OVERRIDE { return m_pCurve; }

private:
  bool initPlaneFromDbLine(OdDbCurvePtr pCurve);
};
// ExtrudeCurve


template <class EntCreator>
class ExtrudePlane : public ExtrudeBaseHelper<EntCreator>
{
  OdDbPlaneSurfacePtr m_pPlane;

public:
  typedef OdDbPlaneSurface EntType;

  ExtrudePlane(ExtrudeInit& context, OdDbPlaneSurfacePtr pPlane)
    : ExtrudeBaseHelper<EntCreator>(context)
    , m_pPlane(pPlane)
  {
    this->initBasePoint(m_pPlane);
    this->initPlane(m_pPlane);
  }

  OdDbEntity* getEntity() ODRX_OVERRIDE { return m_pPlane; }
};
// ExtrudePlane


//
void ExtrudeInit::run()
{
  // 0. Init mode: solid/surface (if there is no selected entity)
  // 1. Init extrude profile entity (if not closed curve => force setup mode to surface)
  if (!selectItemToExtrude()) {
    return;
  }
  forceSetupMode();

  // 2. Collect extrude params for selected method and make extrude
  bool rc = runMain<ExtrudeRegion>()
    || runMain<ExtrudeCurve>()
    || runMain<ExtrudePlane>();

  if (!rc) {
    ODA_FAIL_ONCE();
  }
}

template <template<class> class Helper>
bool ExtrudeInit::runMain()
{
  typedef typename Helper<SolidCreator>::EntType EntType;
  OdSmartPtr<EntType> pEnt = EntType::cast(m_pEntity);
  if (pEnt.isNull()) {
    return false;
  }

  if (eSolid == m_mode) {
    Helper<SolidCreator> helper(*this, pEnt);
    helper.run();
  } else {
    Helper<SurfaceCreator> helper(*this, pEnt);
    helper.run();
  }

  return true;
}

bool ExtrudeInit::selectItemToExtrude()
{
  ExtrudeFilter filter;

  OdDbSelectionSetPtr pSet;
  while (true)
  {
    // Here we select only one entity for simplification
    try
    {
      pSet = m_pIO->select(OD_T("Select objects to extrude or [MOde]"),
        OdEd::kSelSingleEntity | OdEd::kSelAllowEmpty,
        0, OD_T("MOde"), &filter);
      break;
    }
    catch (const OdEdKeyword&) {
      changeCraetionMode();
    }
  }

  if (pSet->numEntities() == 0) {
    m_pIO->putString(OD_T("Nothing selected."));
    return false;
  }
  if (pSet->numEntities() != 1) {
    m_pIO->putString(OD_T("Single entity selection supported only."));
    return false;
  }

  OdDbSelectionSetIteratorPtr pIt = pSet->newIterator();
  m_pEntity = OdDbEntity::cast(pIt->objectId().safeOpenObject(OdDb::kForWrite));
  return !m_pEntity.isNull();
}

void ExtrudeInit::changeCraetionMode()
{
  m_mode = ExtrudeMode(m_pIO->getKeyword(OD_T("Closed profiles creation mode [SOlid/SUrface]"), OD_T("SOlid SUrface"), int(m_mode), 0));
}

void ExtrudeInit::forceSetupMode()
{
  if (eSolid != m_mode) {
    return;
  }

  OdDbCurvePtr pCurve = OdDbCurve::cast(m_pEntity);
  if (pCurve.isNull()) {
    return;
  }

  if (!pCurve->isClosed()) {
    m_mode = eSurface;
  }
}
// ExtrudeInit


template <class EntCreator>
ExtrudeBaseHelper<EntCreator>::ExtrudeBaseHelper(ExtrudeInit& context)
  : m_pDb(context.m_pDb)
  , m_pIO(context.m_pIO)
  , m_creator(context.m_pDb)
  , m_taper(0.)
  , m_pGsView(NULL)
{}

template <class EntCreator>
void ExtrudeBaseHelper<EntCreator>::initBasePoint(const OdDbEntity* pEntity)
{
  OdGeExtents3d extents;
  OdResult res = pEntity->getGeomExtents(extents);
  ODA_ASSERT_ONCE(eOk == res);
  m_basePoint = extents.center();
}
template <class EntCreator>
void ExtrudeBaseHelper<EntCreator>::initPlane(const OdDbEntity* pEntity)
{
  // plane
  OdDb::Planarity planarity;
  OdGePlane plane;
  OdResult res = pEntity->getPlane(plane, planarity);
  ODA_ASSERT_ONCE(res == eOk);
  ODA_ASSERT_ONCE(OdDb::kPlanar == planarity || OdDb::kLinear == planarity);

  // plane normal
  m_planeNormal = plane.normal();
}

template <class EntCreator>
double ExtrudeBaseHelper<EntCreator>::getDist(const OdGePoint3d& pt)
{
  if (!m_pGsView) return 0.;

  OdGePoint3d ptPos = m_pGsView->position(),
    ptTarget = m_pGsView->target();
  OdGeVector3d vDirection = ptPos - ptTarget;
  vDirection.normalize();
  OdGeVector3d vUp = m_pGsView->upVector();
  vUp.normalize();
  OdGeVector3d vLeft = vUp.crossProduct(vDirection);

  OdGeVector3d normalLU = vUp * m_planeNormal.dotProduct(vUp) + vLeft * m_planeNormal.dotProduct(vLeft);
  if (normalLU.isZeroLength(tol)) return 0.;//view dir is parallel to noraml
  normalLU.normalize();

  OdGeVector3d tmp = pt - m_basePoint;
  OdGeVector3d vect = vUp * tmp.dotProduct(vUp) + vLeft * tmp.dotProduct(vLeft);
  bool isNegative = vect.dotProduct(normalLU) < 0;
  //ODA_ASSERT_ONCE(vect.isPerpendicularTo(vDirection, tol));

  double dAng = m_planeNormal.angleTo(normalLU);
  double dCoef = cos(dAng);
  if (!OdZero(dCoef, tol.equalPoint())) vect = m_planeNormal * vect.length() / dCoef;

  double dist = vect.length();
  if (isNegative) dist = -dist;

  return dist;
}

// reset view
template <class EntCreator>
void ExtrudeBaseHelper<EntCreator>::invalidate()
{
  if (!m_pGsView) return;

  OdGsDevice* pDevice = m_pGsView->device();
  if (pDevice)
  {
    pDevice->invalidate();
    pDevice->update();
  }
}

template <class EntCreator>
void ExtrudeBaseHelper<EntCreator>::run()
{
  bool isStep = false;

  OdUInt32 nKeyword = 4;

  do
  {
    isStep = false;

    double dHeight = 0.;

    try
    {
      nKeyword = 4;

      OdGePoint3d point = m_pIO->getPoint(OD_T("Specify height of extrusion or [Direction/Path/Taper angle]:"),
        OdEd::kGptNoUCS | OdEd::kInpThrowOther | OdEd::kInpThrowEmpty,
        0, OD_T("Direction Path TaperAngle"), this);

      dHeight = getDist(point);
    }
    catch (const OdEdKeyword& key)
    {
      invalidate();
      nKeyword = key.keywordIndex();
    }
    catch (const OdEdOtherInput& other)
    {
      invalidate();
      if (other.string().isEmpty()) return;
      dHeight = m_pDb->formatter().unformatLinear(other.string());
    }

    switch (nKeyword)
    {
    case 0://Direction
      performCmdDirection();
      break;
    case 1://Path
      performCmdPath();
      break;
    case 2://TaperAngle
      performCmdSetTaper();
      isStep = true;
      break;
    case 4://height
      performCmdHeight(dHeight);
      break;
    }
  } while (isStep);
}

template <class EntCreator>
void ExtrudeBaseHelper<EntCreator>::performCmdDirection()
{
  OdGePoint3d ptStart = m_pIO->getPoint(OD_T("Specify start point of direction:"));
  OdGePoint3d ptEnd = m_pIO->getPoint(OD_T("Specify end point of direction:"));

  OdResult res = createEntity(ptStart, ptEnd);
  if (eOk != res) {
    m_pIO->putString(OdString().format(OD_T("Error : %s"), OdError(res).description().c_str()));
    return;
  }

  m_pIO->putString(OD_T("Extrude success."));
}

template <class EntCreator>
void ExtrudeBaseHelper<EntCreator>::performCmdPath()
{
  OdDbSelectionSetPtr pPathSet;

  const int selOptions = OdEd::kSelAllowEmpty | OdEd::kSelSingleEntity;
  try
  {
    ExtrudePathFilter filter;
    pPathSet = m_pIO->select(OD_T("Select extrusion path or [Taper angle]:"), selOptions, 0, OD_T("TaperAngle"), &filter);
  }
  catch (const OdEdKeyword&)
  {
    performCmdSetTaper();

    ExtrudePathFilter filter;
    pPathSet = m_pIO->select(OD_T("Select extrusion path:"), selOptions, 0, OdString::kEmpty, &filter);
  }

  if (pPathSet->numEntities() == 0)
  {
    m_pIO->putString(OD_T("Nothing selected."));
    return;
  }
  else if (pPathSet->numEntities() > 1)
  {
    m_pIO->putString(OD_T("More than one entities was selected."));
    return;
  }

  OdDbSelectionSetIteratorPtr pIter = pPathSet->newIterator();
  OdDbCurvePtr pCurve = OdDbCurve::cast(pIter->objectId().safeOpenObject(OdDb::kForRead));
  if (pCurve.isNull())
  {
    m_pIO->putString(OD_T("Selected entity is not a curve."));
    return;
  }

  OdResult res = createEntity(pCurve.get());
  if (res != eOk)
  {
    m_pIO->putString(OdString().format(OD_T("Error : %s"), OdError(res).description().c_str()));
    return;
  }

  m_pIO->putString(OD_T("Extrude success."));
}

template <class EntCreator>
void ExtrudeBaseHelper<EntCreator>::performCmdSetTaper()
{
  double dTaper = m_pIO->getReal(OD_T("Specify angle of taper for extrusion :"), OdEd::kInpDefault, 0.);
  setTaper(dTaper);
}

template <class EntCreator>
void ExtrudeBaseHelper<EntCreator>::performCmdHeight(double dHeight)
{
  OdResult res = createEntity(dHeight);
  if (res != eOk)
  {
    m_pIO->putString(OdString().format(OD_T("Error : %s"), OdError(res).description().c_str()));
    return;
  }

  m_pIO->putString(OD_T("Extrude success."));
}
// ExtrudeHelper


template <class EntCreator>
bool ExtrudeCurve<EntCreator>::initPlaneFromDbLine(OdDbCurvePtr pCurve)
{
  OdResult res = eOk;

  OdDbLinePtr pLine = OdDbLine::cast(pCurve);
  if (pLine.isNull()) {
    return false;
  }

  OdGePoint3d startPnt;
  OdGePoint3d endPnt;

  res = pLine->getStartPoint(startPnt);
  ODA_ASSERT_ONCE(eOk == res);
  res = pLine->getEndPoint(endPnt);
  ODA_ASSERT_ONCE(eOk == res);

  OdGeVector3d dir = endPnt - startPnt;
  if (OdZero(dir.z, tol.equalPoint())) {
    return false;
  }

  OdGeVector3d extrudeDir = dir.crossProduct(OdGeVector3d::kZAxis);
  if (extrudeDir.isZeroLength(tol)) {
    m_planeNormal = OdGeVector3d::kXAxis;
    return true;
  }

  m_planeNormal = extrudeDir.normal();
  return true;
}
// ExtrudeCurve



void _Extrude_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbUserIOPtr pIO = pDbCmdCtx->userIO();
  if (pIO.isNull())
    return;

  ExtrudeInit extrudeHelper(pDb, pIO);
  try
  {
    extrudeHelper.run();
  }
  catch (const OdEdCancel&)
  {
    pIO->putString(OD_T("Canceled."));
  }
  // catch  all exceptions
  catch (...)
  {
    pIO->putString(OD_T("Extrude failed."));
  }
}
