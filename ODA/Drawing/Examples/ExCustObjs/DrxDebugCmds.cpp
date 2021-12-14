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
#include "Ed/EdUserIO.h"
#include "Ed/EdCommandStack.h"
#include "DbCommandContext.h"
#include "DbSSet.h"
#include "DbDatabase.h"
#include "DbViewport.h"
#include "Ge/GeScale3d.h"
#include "Ge/GeExtents2d.h"
#include "RxObject.h"

#include "DbBlockReference.h"
#include "DbPageController.h"
#define STL_USING_LIMITS
#define STL_USING_ALGORITHM
#define STL_USING_MAP
#include "OdaSTL.h"

#include "DbLayout.h"
#include "AbstractViewPE.h"
#include "DbSpatialFilter.h"
#include "DbIndex.h"
#include "Gi/GiClipBoundary.h"
#include "DbBlockTableRecord.h"
#include "Ge/GeBoundBlock3d.h"
#include "DbUserIO.h"
#include "StaticRxObject.h"
#include "DbPolygonMesh.h"
#include "Db3dPolyline.h"
#include "DbVbaProject.h"
#include "DbHostAppServices.h"
#include "DbRotatedDimension.h"
#include "DbAlignedDimension.h"
#include "DbSortentsTable.h"
#include "DbTable.h"
#include "DbSweptSurface.h"
#include "Ed/EdFunctionIO.h"
#include "FdField.h"
#include "DbRegAppTable.h"

#include "DbViewportTable.h"
#include "DbViewportTableRecord.h"
#include "DbSubentId.h"
#include "DbPolyline.h"
#include "DbCompoundObjectId.h"
#include "DbAttribute.h"
#include "DbTrace.h"
#include "OdDToStr.h"
#include "../Extensions/PdfUnderlayCommon/PdfUnderlay.h"
#include "DbSubDMesh.h"
#include "DbNurbSurface.h"
#include "DbPoint.h"
#include "DbLine.h"
#include "DbCircle.h"
#include "DbEllipse.h"
#include "DbArc.h"
#include "DbSpline.h"
#include "DbRegion.h"
#include "Db3dSolid.h"
#include "DbBody.h"
#include "DbPlaneSurface.h"
#include "Ge/GeNurbCurve3d.h"
#include "Ge/GeCircArc3d.h"
#include "Ge/GeEllipArc3d.h"
#include "Ge/GeKnotVector.h"
#include "Ge/GeLineSeg2d.h"
#include "Ge/GeRay2d.h"
#include "DbLayerTableRecord.h"
#include "ModelerMarkers.h"
#include "DbObjectContextInterface.h"
#include "DbObjectContextManager.h"
#include "DbObjectContextCollection.h"
#include "DbAssocArrayActionBody.h"
#include "DbAssocManager.h"
#include "DbAssocArrayCommonParameters.h"
#include "DbCenterLineActionBody.h"
#include "DbCenterMarkActionBody.h"
#include "DbDictionaryVar.h"
#include "DbUnderlayDefinition.h"
#include "ExSelectionUtils.h"
#include "Tf/TfRevisionControl.h"
#include "DbBlockTable.h"
#include "DbSection.h"
#include "DbDimAssoc.h"
#include "ModelerGeometryDefs.h" 
#include "RxMember.h"
#include "RxProperty.h"
#include "RxValue.h"
#include "RxAttribute.h"
#include "Gi/GiOrthoClipperEx.h" //required for ExTrackers
#include "Gi/GiDrawableImpl.h" // required for ExTrackers
#include "../Extensions/ExServices/ExTrackers.h"

#include "ClassFilter.h"

DBENT_EXPORT OdModelerGeometryPtr getModelerGeometry(const OdDbEntity* pEnt);

OdDbObjectId activeBlockTableRecord(const OdDbDatabase* pDb)
{
  OdDbObjectId idSpace = pDb->getActiveLayoutBTRId();
  if(idSpace == pDb->getPaperSpaceId())
  {
    OdDbViewportPtr pVp = pDb->activeViewportId().safeOpenObject();
    if(pVp->number()>1)
      idSpace = pDb->getModelSpaceId();
  }
  return idSpace;
}


void _thawlayers_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();

  OdDbObjectIdArray layers;

  OdRxClassPtr pClass = odrxClassDictionary()->getAt(OD_T("AcDbLayerTableRecord"));
  ClassFilter fltr;
  OdDbSelectionSetPtr pSSet = pIO->select("Select a layer <done>:", 0, 0, OdString(), &fltr);
  OdDbSelectionSetIteratorPtr pIter = pSSet->newIterator();
  while(!pIter->done())
  {
    layers.append(pIter->objectId());
    pIter->next();
  }
  fltr.setClass(OdDbViewport::desc());
  pSSet = pIO->select("Select a viewport:", 0, 0, OdString(), &fltr);
  while(!pIter->done())
  {
    OdDbViewportPtr pVp = pIter->objectId().safeOpenObject();
    pVp->thawLayersInViewport(layers);
    pIter->next();
  }
}

void _ForceErase_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();

  OdDbSelectionSetPtr pSSet = pIO->select(OdString::kEmpty,
    OdEd::kSelAllowInactSpaces|OdEd::kSelAllowObjects|OdEd::kSelAllowPSVP);

  OdDbSelectionSetIteratorPtr pIter = pSSet->newIterator();
  while(!pIter->done())
  {
    OdDbObjectPtr pObj = pIter->objectId().safeOpenObject(OdDb::kForWrite);
    pObj->erase();
    pIter->next();
  }
}

OdEdCommandPtr pPrevErase;

void _ERASE_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();
  OdString msg;
  msg.format(OD_T("This custom DrxDebug:ERASE command overrides %s:ERASE one."), pPrevErase->groupName().c_str());
  pIO->putString(msg);

  odedRegCmds()->executeCommand(pPrevErase, pCmdCtx);
}

#if 0
//
// Tests unloading.
//
#include "DbBlockReference.h"
#include "DbPageController.h"

void extern_debugcommand(COdaMfcAppDoc* pDoc, CDwgView* pView)
{
  OdDbDatabase* pDb = pDoc->database();
  pDb->disableUndoRecording(false);
  pDb->startUndoRecord();
  OdDbEntityPtr pEnt;

  OdDbBlockTableRecordPtr pMs = pDb->getModelSpaceId().safeOpenObject();
  OdDbObjectIteratorPtr pIter = pMs->newIterator();
  while (!pIter->done())
  {
    OdDbObjectId id = pIter->objectId();
    pEnt = id.openObject();

    if (pEnt->isA() == OdDbBlockReference::desc())
    {
      pEnt->upgradeOpen();
      pEnt->setColorIndex(5);
    }

    pEnt = 0;

    pIter->step();
  }

  pDb->pageObjects();
}
#endif

#if 0
//
// Tests proxy flags.
//
#include "DbProxyEntity.h"

void extern_debugcommand(COdaMfcAppDoc* pDoc, CDwgView* pView)
{
  OdDbObjectId  objId = pView->getCurObjectId();
  OdDbObjectPtr obj = objId.openObject(OdDb::kForWrite);

  OdResult res;
  OdString msg;
  OdDbProxyEntityPtr pProxy = OdDbProxyEntity::cast(obj);
  if (!pProxy.isNull())
  {
    res = pProxy->setColorIndex(5);
    msg = "setColorIndex()\n";
    msg += pDoc->appServices()->getErrorDescription(res);
    pDoc->appServices()->warning(msg);
  }
}
#endif

#if 1
//
// Tests replacing run-time classes used by DD by altering pseudo constructor.
// It needs to add next functions to test functionality:
// - rxInitExPseudo() to _My_Module::initApp()
// - rxUninitExPseudo() to _My_Module::uninitApp()
//
#include "DbClass.h"
#include "DbFiler.h"
#include "DbCircle.h"
#include "DbText.h"
#include "DbWipeout.h"
#include "DbProxyEntity.h"

class ExCustProxyEntity : public OdDbProxyEntity
{
  OdString m_Name;
public:
  ODDB_PSEUDO_DECLARE_MEMBERS(ExCustProxyEntity);

  ExCustProxyEntity() : m_Name(OD_T("Custom proxy")) {}
  virtual ~ExCustProxyEntity() {}

  bool subWorldDraw(OdGiWorldDraw * pWd) const
  {
    OdDbProxyEntity::subWorldDraw(pWd);
    OdDbTextPtr pText = OdDbText::createObject();
    pText->setPropertiesFrom(this);
    pText->setTextString(m_Name);
    pText->worldDraw(pWd);
    return true;
  }
};

ODDB_PSEUDO_DEFINE_MEMBERS(ExCustProxyEntity,
                           OdDbProxyEntity,
                           OdDbProxyEntity,
                           DBOBJECT_CONSTR);

class ExCustBaseCircle : public OdDbCircle
{
public:
  ODDB_DECLARE_MEMBERS(ExCustBaseCircle);

  ExCustBaseCircle() : m_BaseName(OD_T("Custom circle")) {}
  virtual ~ExCustBaseCircle() {}
protected:
  OdString m_BaseName;
};

ODRX_CONS_DEFINE_MEMBERS(ExCustBaseCircle,
                         OdDbCircle,
                         DBOBJECT_CONSTR);

class ExCustCircle : public ExCustBaseCircle
{
  OdString m_Name;
public:
  ODDB_PSEUDO_DECLARE_MEMBERS(ExCustCircle);

  ExCustCircle() : m_Name(OD_T("Custom circle")) {}
  virtual ~ExCustCircle() {}

  OdResult dwgInFields(OdDbDwgFiler* pFiler)
  {
    OdResult res = OdDbCircle::dwgInFields(pFiler);
    if (pFiler->filerType() != OdDbFiler::kFileFiler)
    {
      m_Name = pFiler->rdString();
    }
    return res;
  }
  void dwgOutFields(OdDbDwgFiler* pFiler) const
  {
    OdDbCircle::dwgOutFields(pFiler);
    if (pFiler->filerType() != OdDbFiler::kFileFiler)
    {
      pFiler->wrString(m_Name);
    }
  }
  bool subWorldDraw(OdGiWorldDraw * pWd) const
  {
    OdDbCircle::subWorldDraw(pWd);
    OdDbTextPtr pText = OdDbText::createObject();
    pText->setPropertiesFrom(this);
    pText->setTextString(m_Name);
    pText->worldDraw(pWd);
    return true;
  }
};

ODDB_PSEUDO_DEFINE_MEMBERS(ExCustCircle,
                           ExCustBaseCircle,
                           OdDbCircle,
                           DBOBJECT_CONSTR);

class ExCustWipeout : public OdDbWipeout
{
  OdString m_Name;
public:
  ODDB_PSEUDO_DECLARE_MEMBERS(ExCustWipeout);

  ExCustWipeout() : m_Name(OD_T("Custom wipeout")) {}
  virtual ~ExCustWipeout() {}

  OdResult dwgInFields(OdDbDwgFiler* pFiler)
  {
    OdResult res = OdDbWipeout::dwgInFields(pFiler);
    if (pFiler->filerType() != OdDbFiler::kFileFiler)
    {
      m_Name = pFiler->rdString();
    }
    return res;
  }
  void dwgOutFields(OdDbDwgFiler* pFiler) const
  {
    OdDbWipeout::dwgOutFields(pFiler);
    if (pFiler->filerType() != OdDbFiler::kFileFiler)
    {
      pFiler->wrString(m_Name);
    }
  }
  bool subWorldDraw(OdGiWorldDraw * pWd) const
  {
    OdDbWipeout::subWorldDraw(pWd);
    OdDbTextPtr pText = OdDbText::createObject();
    pText->setPropertiesFrom(this);
    pText->setTextString(m_Name);
    pText->worldDraw(pWd);
    return true;
  }
};

ODDB_PSEUDO_DEFINE_MEMBERS(ExCustWipeout,
                           OdDbWipeout,
                           OdDbWipeout,
                           DBOBJECT_CONSTR);

void rxInitExPseudo()
{
  ExCustProxyEntity::rxInit();
  ExCustBaseCircle::rxInit();
  ExCustCircle::rxInit();
  ExCustWipeout::rxInit();
}

void rxUninitExPseudo()
{
  ExCustWipeout::rxUninit();
  ExCustCircle::rxUninit();
  ExCustBaseCircle::rxUninit();
  ExCustProxyEntity::rxUninit();
}

//
// OdDbRegion::createFromCurves test
//

extern OdRxClassPtr getModelerGeometryCreatorService();

void CreateRegions(OdRxObjectPtrArray &regions)
{
#if defined(_TOOLKIT_IN_DLL_) && defined(WIN32)
  OdGePoint3d center(0, 0, 0);
  OdGeVector3d step(10, 0, 0);
  double d = 3.;
  OdRxObjectPtrArray curves;
  OdResult res;

  // 1.
  OdDbLinePtr pLine1 = OdDbLine::createObject();
  OdDbLinePtr pLine2 = OdDbLine::createObject();
  OdDbLinePtr pLine3 = OdDbLine::createObject();
  OdDbLinePtr pLine4 = OdDbLine::createObject();

  pLine1->setStartPoint(center); pLine1->setEndPoint(center + OdGeVector3d(d,0,0));
  pLine2->setStartPoint(center + OdGeVector3d(d,0,0)); pLine2->setEndPoint(center + OdGeVector3d(d,d,0));
  pLine3->setStartPoint(center + OdGeVector3d(d,d,0)); pLine3->setEndPoint(center + OdGeVector3d(0,d,0));
  pLine4->setStartPoint(center + OdGeVector3d(0,d,0)); pLine4->setEndPoint(center);

  curves.push_back(pLine1.get());
  curves.push_back(pLine2.get());
  curves.push_back(pLine3.get());
  curves.push_back(pLine4.get());

  res = OdDbRegion::createFromCurves(curves, regions);

  // 2.
  curves.clear(); center += step;

  OdDbCirclePtr pCircle = OdDbCircle::createObject();
  pCircle->setCenter(center);
  pCircle->setRadius(d);
  pCircle->setNormal(OdGeVector3d(0,0,1));
  curves.push_back(pCircle.get());
  res = OdDbRegion::createFromCurves(curves, regions);

  // 3.
  curves.clear(); center += step;

  OdDbEllipsePtr pEllipse = OdDbEllipse::createObject();
  pEllipse->set(center, OdGeVector3d(0,0,1), OdGeVector3d(0,d,0), 0.5);
  curves.push_back(pEllipse.get());
  res = OdDbRegion::createFromCurves(curves, regions);

  // 4.
  curves.clear(); center += step;

  OdGePoint2d c(center.x,center.y);
  OdDbPolylinePtr pPoly = OdDbPolyline::createObject();
  pPoly->addVertexAt(0, c);
  pPoly->addVertexAt(1, c + OdGeVector2d(d,d));
  pPoly->addVertexAt(2, c + OdGeVector2d(0,d));
  pPoly->addVertexAt(3, c);

  curves.push_back(pPoly.get());


  res = OdDbRegion::createFromCurves(curves, regions);

  // 5.
  curves.clear(); center += step;

  OdDbCirclePtr pArc = OdDbCircle::createObject();
  pArc->setCenter(center);
  pArc->setRadius(d);
  pArc->setNormal(OdGeVector3d(0,0,1));
  curves.push_back(pArc.get());
  res = OdDbRegion::createFromCurves(curves, regions);

  // 6.
  curves.clear(); center += step;

  OdDbSplinePtr pSpline = OdDbSpline::createObject();
  OdGeNurbCurve3d nurbell( OdGeEllipArc3d(center, OdGeVector3d(0,-1,0), OdGeVector3d(1,0,0), d, 1.0, 0.0, Oda2PI ), 0);

  int degree;
  bool rational, periodic;
  OdGeKnotVector knots;
  OdGePoint3dArray controlPoints;
  OdGeDoubleArray weights;

  nurbell.getDefinitionData(degree, rational, periodic, knots, controlPoints, weights);
  pSpline->setNurbsData(degree, rational, nurbell.isClosed(), periodic, controlPoints, knots, weights, 1e-8);
  curves.push_back(pSpline.get());
  res = OdDbRegion::createFromCurves(curves, regions);

  // 7.
  curves.clear(); center += step;

  {
  pSpline = OdDbSpline::createObject();
  OdGeNurbCurve3d nurbell((OdGeEllipArc3d(OdGeCircArc3d(center, OdGeVector3d(0,0,1), d))));

  int degree;
  bool rational, periodic;
  OdGeKnotVector knots;
  OdGePoint3dArray controlPoints;
  OdGeDoubleArray weights;

  nurbell.getDefinitionData(degree, rational, periodic, knots, controlPoints, weights);
  pSpline->setNurbsData(degree, rational, nurbell.isClosed(), periodic, controlPoints, knots, weights, 1e-8);
  curves.push_back(pSpline.get());
  res = OdDbRegion::createFromCurves(curves, regions);

  // 8.
  curves.clear(); center += step;
  }

  {
    pSpline = OdDbSpline::createObject();
    OdGeNurbCurve3d nurbell((OdGeEllipArc3d(OdGeCircArc3d(center, OdGeVector3d(0,0,1), OdGeVector3d(0,1,0), d, 0, OdaPI))));

    int degree;
    bool rational, periodic;
    OdGeKnotVector knots;
    OdGePoint3dArray controlPoints;
    OdGeDoubleArray weights;

    nurbell.getDefinitionData(degree, rational, periodic, knots, controlPoints, weights);
    pSpline->setNurbsData(degree, rational, nurbell.isClosed(), periodic, controlPoints, knots, weights, 1e-8);
    curves.push_back(pSpline.get());

    OdDbLinePtr pLine1 = OdDbLine::createObject();
    pLine1->setStartPoint(nurbell.endPoint()); pLine1->setEndPoint(nurbell.startPoint());
    curves.push_back(pLine1.get());

    res = OdDbRegion::createFromCurves(curves, regions);

    // 9.
    curves.clear(); center += step;
  }

  {
    {
    OdDbSplinePtr pSpline1 = OdDbSpline::createObject();
    OdGeNurbCurve3d nurbell((OdGeEllipArc3d(OdGeCircArc3d(center, OdGeVector3d(0,0,1), OdGeVector3d(0,1,0), d, 0, OdaPI))));

    int degree;
    bool rational, periodic;
    OdGeKnotVector knots;
    OdGePoint3dArray controlPoints;
    OdGeDoubleArray weights;

    nurbell.getDefinitionData(degree, rational, periodic, knots, controlPoints, weights);
    pSpline1->setNurbsData(degree, rational, nurbell.isClosed(), periodic, controlPoints, knots, weights, 1e-8);
    curves.push_back(pSpline1.get());
    }

    OdDbSplinePtr pSpline2 = OdDbSpline::createObject();
    OdGeNurbCurve3d nurbell((OdGeEllipArc3d(OdGeCircArc3d(center, OdGeVector3d(0,0,1), OdGeVector3d(0,1,0), d, OdaPI, Oda2PI))));

    int degree;
    bool rational, periodic;
    OdGeKnotVector knots;
    OdGePoint3dArray controlPoints;
    OdGeDoubleArray weights;

    nurbell.getDefinitionData(degree, rational, periodic, knots, controlPoints, weights);
    pSpline2->setNurbsData(degree, rational, nurbell.isClosed(), periodic, controlPoints, knots, weights, 1e-8);
    curves.push_back(pSpline2.get());

    res = OdDbRegion::createFromCurves(curves, regions);

    curves.clear(); center += step;
  }
#endif
}


void _CreateRegionsFromCurves_func(OdEdCommandContext* pCmdCtx)
{
  // Create regions
  OdRxObjectPtrArray regions;
  CreateRegions(regions);

  // Add regions to database
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();

  OdDbBlockTableRecordPtr pMs = pDb->getModelSpaceId().safeOpenObject(OdDb::kForWrite);

  OdRxObjectPtrArray::iterator pIt = regions.begin();
  OdRxObjectPtrArray::iterator pEnd = regions.end();

  while(pIt != pEnd)
  {
    pMs->appendOdDbEntity(OdDbEntity::cast(*pIt));
    ++pIt;
  }
}


//
//
//
#include "MemoryStream.h"
#include "ModelerGeometry.h"
#include "ModelerGeometryCreator.h"

extern OdRxClassPtr getModelerGeometryCreatorService();

#if 0 //Moved to ExCommandsModule
void _Sphere_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabase* pDb = pDbCmdCtx->database();
  OdDbBlockTableRecordPtr pMs = pDb->getModelSpaceId().safeOpenObject(OdDb::kForWrite);
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();

  OdGePoint3d center = pIO->getPoint(OD_T("Specify sphere center point:"));
  OdDb3dSolidPtr pSolid  = OdDb3dSolid::createObject();
  pSolid->setDatabaseDefaults(pDb);

  struct RadiusTracker : OdStaticRxObject<OdEdRealTracker>
  {
    OdDb3dSolidPtr _ent;
    OdGePoint3d _center;
    RadiusTracker(OdDb3dSolidPtr pSolid, const OdGePoint3d& center,OdDbDatabase* /*pDb*/)
      : _ent(pSolid)
      , _center(center)
    {
      RadiusTracker::setValue(1.);
    }

    virtual void setValue(double r)
    {
      _ent->createSphere( r );
      _ent->transformBy( OdGeMatrix3d::translation( _center.asVector() ) );
    }

    virtual int addDrawables(OdGsView* pView)
    {
      pView->add(_ent, 0);
      return 1;
    }

    virtual void removeDrawables(OdGsView* pView)
    {
      pView->erase(_ent);
    }
  } tracker(pSolid, center, pDb);

  pIO->getDist(OD_T("Specify sphere radius:"), OdEd::kGdsFromLastPoint, 0.0, OdString::kEmpty, &tracker );

  pMs->appendOdDbEntity(pSolid);
}


void _Box_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabase* pDb = pDbCmdCtx->database();
  OdDbBlockTableRecordPtr pMs = pDb->getModelSpaceId().safeOpenObject(OdDb::kForWrite);
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();

  OdGePoint3d center = pIO->getPoint(OD_T("Specify height center point:"));
  OdDb3dSolidPtr pSolid  = OdDb3dSolid::createObject();
  pSolid->setDatabaseDefaults(pDb);

  struct RadiusTracker : OdStaticRxObject<OdEdRealTracker>
  {
    OdDb3dSolidPtr _ent;
    OdGePoint3d _center;
    RadiusTracker(OdDb3dSolidPtr pSolid, const OdGePoint3d& center,OdDbDatabase* /*pDb*/)
      : _ent(pSolid)
      , _center(center)
    {
      RadiusTracker::setValue(1.);
    }

    virtual void setValue(double r)
    {
      r *= 2;
      _ent->createBox( r, r, r );
      _ent->transformBy( OdGeMatrix3d::translation( _center.asVector() ) );
    }

    virtual int addDrawables(OdGsView* pView)
    {
      pView->add(_ent, 0);
      return 1;
    }

    virtual void removeDrawables(OdGsView* pView)
    {
      pView->erase(_ent);
    }
  } tracker(pSolid, center, pDb);

  pIO->getDist(OD_T("Specify box height:"), OdEd::kGdsFromLastPoint, 0.0, OdString::kEmpty, &tracker );

  pMs->appendOdDbEntity(pSolid);
}
#endif //#if 0 //Moved to ExCommandsModule

void _SomePyramid_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabase* pDatabase = pDbCmdCtx->database();
  OdDbBlockTableRecordPtr pMs = pDatabase->getModelSpaceId().safeOpenObject(OdDb::kForWrite);

  OdDb3dSolidPtr pSolid;
  pSolid = OdDb3dSolid::createObject();
  if (pSolid->createPyramid(7., 4, 3.) == eOk)
    pMs->appendOdDbEntity(pSolid);
/*
  pSolid = OdDb3dSolid::createObject();
  pSolid->createFrustum(5., 6., 4., 3.);
  pMs->appendOdDbEntity(pSolid);

  pSolid = OdDb3dSolid::createObject();
  pSolid->createTorus(15., 2.);
  pMs->appendOdDbEntity(pSolid);

  pSolid = OdDb3dSolid::createObject();
  pSolid->createWedge(5.,5.,5.);
  pMs->appendOdDbEntity(pSolid);
*/
}

void _CreateNurbSurface_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabase* pDatabase = pDbCmdCtx->database();
  OdDbBlockTableRecordPtr pMs = pDatabase->getModelSpaceId().safeOpenObject(OdDb::kForWrite);

  OdGePoint3dArray ctrlPts;
  ctrlPts.append(OdGePoint3d(5,   5, 7));
  ctrlPts.append(OdGePoint3d(5,  10, 7));
  ctrlPts.append(OdGePoint3d(10,  5, 7));
  ctrlPts.append(OdGePoint3d(10, 10, 7));
  OdGeDoubleArray weights;
  weights.append(0);
  weights.append(0);
  weights.append(0);
  weights.append(0);
  OdGeKnotVector knots;
  knots.append(-1);
  knots.append(-1);
  knots.append(1);
  knots.append(1);
  OdDbNurbSurfacePtr pNurbSurf  = OdDbNurbSurface::createObject();
  pNurbSurf->set(1, 1, false, 2, 2, ctrlPts, weights, knots, knots);

  pMs->appendOdDbEntity(pNurbSurf);
}

void _SomeWedge_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabase* pDatabase = pDbCmdCtx->database();
  OdDbBlockTableRecordPtr pMs = pDatabase->getModelSpaceId().safeOpenObject(OdDb::kForWrite);

  OdDb3dSolidPtr pSolid  = OdDb3dSolid::createObject();
  pSolid->createWedge(3., 4., 5.);

  pMs->appendOdDbEntity(pSolid);
}

void _intersectWith_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabase* pDatabase = pDbCmdCtx->database();
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();
  OdDbBlockTableRecordPtr pMs = pDatabase->getModelSpaceId().safeOpenObject(OdDb::kForWrite);

  OdDbSelectionSetPtr pFirstSel = pIO->select(OD_T("Select region"), OdEd::kSelSingleEntity/*, 0, OdString::kEmpty, &filter*/);
  if (pFirstSel->numEntities() == 0)
  {
    pIO->putString(OD_T("Nothing selected."));
    return;
  }
  OdDbSelectionSetPtr pSecondSel = pIO->select(OD_T("Select entity for test"), OdEd::kSelSingleEntity/*, 0, OdString::kEmpty, &filter*/);
  if (pSecondSel->numEntities() == 0)
  {
    pIO->putString(OD_T("Nothing selected."));
    return;
  }

  OdDbSelectionSetIteratorPtr pIter  = pFirstSel->newIterator();
  OdDbSelectionSetIteratorPtr pIter2 = pSecondSel->newIterator();
  OdDbObjectId objId  = pIter->objectId();
  OdDbEntityPtr pEnt  = objId.openObject();
  OdDbObjectId objId2 = pIter2->objectId();
  OdDbEntityPtr pEnt2 = objId2.openObject();
  if (!pEnt.isNull() && pEnt->isKindOf(OdDbRegion::desc()) && !pEnt2.isNull())
  {
    OdDb::Intersect intersect = OdDb::kExtendThis;
    OdGePoint3dArray array;
    OdGePoint3d origin(0,0,-10);
    OdGeVector3d normal(0.1,0.1,1);
    OdGePlane plane(origin, normal);
    OdDbRegionPtr(pEnt)->intersectWith(pEnt2, intersect, array);

    /*
    unsigned int nPoints = array.size();
    if (nPoints > 0)
    {
      for (int i = 0; i < nPoints; ++i)
      {
        OdDbPointPtr pPoint = OdDbPoint::createObject();
        pPoint->setPosition(array.at(i));
        pMs->appendOdDbEntity(pPoint);
      }
    }*/
  }
}


static OdDbRegionPtr createCircularRegion(OdDbDatabase* pDatabase)
{
  OdRxObjectPtrArray curveSegments;

  // create dummy OdDbCircle
  OdDbCirclePtr pC = OdDbCircle::createObject();
  pC->setFromOdGeCurve(OdGeCircArc3d());
  pC->setDatabaseDefaults(pDatabase);

  curveSegments.append(pC);

  OdRxObjectPtrArray regions;
  OdDbRegion::createFromCurves(curveSegments, regions);
  if (regions.isEmpty())
    return OdDbRegionPtr();

  return regions.first();
}

static void applyTransform(OdDb3dSolid *pSolid, OdGeVector3d translate, double dAngle = 0.)
{
  if (!pSolid)
    return;

  if (dAngle != 0.)
  {
    pSolid->transformBy(OdGeMatrix3d::rotation(dAngle, OdGeVector3d::kYAxis));
  }

  if (!translate.isZeroLength())
  {
    pSolid->transformBy(OdGeMatrix3d::translation(translate));
  }
}


static OdDb3dSolidPtr create90degree(OdDbDatabase* pDatabase, OdGeVector3d translate, double dAngle = 0.)
{
  OdDbRegionPtr pBase = createCircularRegion(pDatabase);

  OdDb3dSolidPtr pSolid = OdDb3dSolid::createObject();
  pSolid->setDatabaseDefaults(pDatabase);
  pSolid->setColorIndex(2);
  OdResult res = pSolid->revolve(pBase, OdGePoint3d(10, 0, 0), OdGeVector3d::kYAxis, OdaPI2);
  if (res != eOk)
  {
    return OdDb3dSolidPtr();
  }

  applyTransform(pSolid, translate, dAngle);
  return pSolid;
}

static OdDb3dSolidPtr createCylinder(OdDbDatabase* pDatabase, OdGeVector3d translate, double dAngle = 0.)
{
  OdDbRegionPtr pBase = createCircularRegion(pDatabase);

  OdDb3dSolidPtr pSolid = OdDb3dSolid::createObject();
  pSolid->setDatabaseDefaults(pDatabase);
  pSolid->setColorIndex(1);
  OdResult res = pSolid->extrude(pBase, 10);
  if (res != eOk)
  {
    return OdDb3dSolidPtr();
  }

  applyTransform(pSolid, translate, dAngle);
  return pSolid;
}

static void addToModelSpace(OdDbDatabase* pDatabase, OdDbEntity *pEntity)
{
  if (pDatabase && pEntity)
  {
    OdDbBlockTableRecordPtr pMs = pDatabase->getModelSpaceId().safeOpenObject(OdDb::kForWrite);
    pMs->appendOdDbEntity(pEntity);
  }
}

class BooleanAutoState
{
  bool m_DisablePE;

public:
  static bool enableBoolean();
  static void disableBoolean();

  BooleanAutoState()
    : m_DisablePE(enableBoolean())
  {
  }

  ~BooleanAutoState()
  {
    if (m_DisablePE)
    {
      disableBoolean();
    }
  }
};

void BooleanAutoState::disableBoolean()
{
  // Try to find class in dictionary by name string
  OdRxClass* pClass = static_cast<OdRxClass*>(odrxClassDictionary()->getAt(OD_T("OdDbBooleanEvaluationPE")).get());
  if (!pClass)
  {
    return;
  }

  // Unregister
  OdDb3dSolid::desc()->delX(pClass);
}

bool BooleanAutoState::enableBoolean()
{
  // Try to find class in dictionary by name string
  OdRxClass* pClass = static_cast<OdRxClass*>(odrxClassDictionary()->getAt(OD_T("OdDbBooleanEvaluationPE")).get());
  if (!pClass)
  {
    return false;
  }

  OdRxObjectPtr ext = OdDb3dSolid::desc()->getX(pClass);
  if (ext.isNull())
  {
    // Try to create object
    OdRxObjectPtr pBooleanPE = pClass->createObject();
    if (pBooleanPE.isNull())
    {
      return false;
    }

    // Register
    OdDb3dSolid::desc()->addX(pClass, pBooleanPE);

    return true;
  }
  return false;
}

OdRxClassPtr odrxGetModelerGeometryCreatorService();
static void checkModeler()
{
  // we should check if some modeler was loaded and if not - load ModelerGeometry.
  // ModelerGeometry will register BooleanPE.
  OdRxClassPtr pService = odrxGetModelerGeometryCreatorService();
  if (!pService.get())
  {
    ::odrxDynamicLinker()->loadApp(OdModelerGeometryModuleName, false);
  }
}


void _PipeDemo_func(OdEdCommandContext* pCmdCtx)
{
  // init 3d operations support
  checkModeler();

  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabase* pDatabase = pDbCmdCtx->database();

  // Lets create pipe from 8 pieces. 4 x 90 degree revolved solids and 4x extruded cylinders.

  OdDb3dSolidPtr pSolids[8];

  // create 90 degree revolved solid
  pSolids[0] = create90degree(pDatabase, OdGeVector3d(0, 0, 10));
  pSolids[2] = create90degree(pDatabase, OdGeVector3d(20, 0, 20), OdaPI2);
  pSolids[4] = create90degree(pDatabase, OdGeVector3d(30, 0, 0), OdaPI);
  pSolids[6] = create90degree(pDatabase, OdGeVector3d(10, 0, -10), OdaPI + OdaPI2);

  // create extruded cylinder
  pSolids[1] = createCylinder(pDatabase, OdGeVector3d::kIdentity);
  pSolids[3] = createCylinder(pDatabase, OdGeVector3d(30, 0, 0));
  pSolids[5] = createCylinder(pDatabase, OdGeVector3d(10, 0, 20), OdaPI2);
  pSolids[7] = createCylinder(pDatabase, OdGeVector3d(10, 0, -10), OdaPI2);

  // add copies of all 8 solids to model space
  OdUInt32 f;
  for (f = 0; f < 8; ++f)
  {
    pSolids[f]->transformBy(OdGeMatrix3d::rotation(OdaPI2, OdGeVector3d::kXAxis));
    addToModelSpace(pDatabase, OdDb3dSolidPtr(pSolids[f]->clone()));
  }

  // Boolean part:
  // enable evaluation of boolean operations
  BooleanAutoState autoPE;

  // Union all solids into single pipe 
  for (f = 1; f < 8; ++f)
  {
    OdResult res = pSolids[0]->booleanOper(OdDb::BoolOperType::kBoolUnite, pSolids[f]);
    if (res != eOk)
      return;
  }

  // add pipe to model space
  pSolids[0]->transformBy(OdGeMatrix3d::translation(OdGeVector3d(40, 0, 0)));
  addToModelSpace(pDatabase, pSolids[0]);
}

void _CheckInterference_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabase* pDatabase = pDbCmdCtx->database();
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();

  OdDbSelectionSetPtr pFirstSel = pIO->select(OD_T("Select first set of objects"), OdEd::kSelSingleEntity/*, 0, OdString::kEmpty, &filter*/);

  if (pFirstSel->numEntities() == 0)
  {
    pIO->putString(OD_T("Nothing selected."));
    return;
  }

  OdDbSelectionSetPtr pSecondSel = pIO->select(OD_T("Select second set of objects"), OdEd::kSelSingleEntity/*, 0, OdString::kEmpty, &filter*/);

  if (pSecondSel->numEntities() == 0)
  {
    pIO->putString(OD_T("Nothing selected."));
    return;
  }

  OdDbSelectionSetIteratorPtr pIter = pFirstSel->newIterator();
  bool notBrack = true;
  while ( !pIter->done() && notBrack)
  {
    OdDbObjectId objId = pIter->objectId();
    OdDbEntityPtr pEnt = objId.openObject();
    if (!pEnt.isNull() && pEnt->isKindOf(OdDb3dSolid::desc()))
    {
      OdDbSelectionSetIteratorPtr pSecondIter = pSecondSel->newIterator();
      while ( !pSecondIter->done() && pIter != pSecondIter )
      {
        objId = pSecondIter->objectId();
        OdDbEntityPtr pSecondEnt = objId.openObject();
        if (!pSecondEnt.isNull() && pSecondEnt->isKindOf(OdDb3dSolid::desc()))
        {
          bool bIsInterfere = false;
          OdDb3dSolidPtr pSolid;
          OdResult res = OdDb3dSolidPtr(pSecondEnt)->checkInterference(NULL, true, bIsInterfere, pSolid);
          ODA_ASSERT(res == eOk);
          if (res == eOk)
          {
            OdDbBlockTableRecordPtr pMs = pDatabase->getModelSpaceId().safeOpenObject(OdDb::kForWrite);
            pMs->appendOdDbEntity(pSolid);
            notBrack = false;
          }
          break;
        }
        pSecondIter->next();
      }
    }
    pIter->next();
  }
}

void _ViewSection_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDatabase = pDbCmdCtx->database();
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();
  OdDbBlockTableRecordPtr pMs = pDatabase->getModelSpaceId().safeOpenObject(OdDb::kForWrite);
  OdDbSelectionSetIteratorPtr pIter;
  OdDbEntityPtr pEnt;
  bool loopExit = false;
  OdGePoint3d ptFirst, ptStart, ptEnd, ptSegmentStart;
  OdGePoint3dArray pp, ppInverse;
  int nLines = 0;
  OdGeVector3d Normal;
  OdDbBlockTableRecordPtr pSpace = activeBlockTableRecord(pDatabase).openObject(OdDb::kForWrite);
  bool isModelSpaceView = pSpace == pMs;
  OdDbObjectIdArray linesId;
  OdDbObjectId objId;
  OdGeMatrix3d mViewTrans;
  OdGeMatrix3d inverseViewTrans;
  OdArray<OdDbEntityPtr> pEnts;

  OdDbObjectId sourceEntity;

  while (true)
  {
    OdGePoint3dArray ptsSelected;
    OdDbSelectionSetPtr pSSet;
    try
    {
      OdGePoint3d ptTemp = pIO->getPoint(L"Select 3d object or press ENTER to finish ", OdEd::kInpThrowEmpty, NULL);
      ptsSelected.append(ptTemp);
    }
    catch (const OdEdCancel&)
    {
      if (pEnts.length() > 0)
        loopExit = true;
      else
        pIO->putString(OD_T("Nothing selected."));

      break;
    }

    if (0 == ptsSelected.length())
      break;

    OdDbFullSubentPath path = selectedFullSubentPath(pIO, pDatabase, L"Select 3d object", ptsSelected, pSSet, true);
    OdDbObjectIdArray pathArray = path.objectIds();

    if (pathArray.length() >= 1)
    {
      OdDbViewportPtr viewportV = OdDbViewport::cast(pathArray[0].openObject());

      if (!viewportV.isNull())
      {
        mViewTrans = OdDbPointRef::mswcsToPswcs(viewportV);
        inverseViewTrans = mViewTrans.inverse();
      }

      sourceEntity = pathArray[pathArray.length() - 1];
      pEnt = sourceEntity.openObject(OdDb::kForWrite);

      if (0 == pEnts.length() && !pEnt.isNull() && (pEnt->isKindOf(OdDb3dSolid::desc()) || pEnt->isKindOf(OdDbSurface::desc())))// && eOk == pEnt->getGripPoints(aPts))
      {
        OdDbViewportTablePtr vTable = OdDbViewportTable::cast(pDatabase->getViewportTableId().openObject());
        OdDbViewportTableRecordPtr vRec = OdDbViewportTableRecord::cast(vTable->getAt(OD_T("*Active"), OdDb::kForRead));
        OdGsView* tempgsView = vRec->gsView();

        if (tempgsView != NULL)
          OdGeMatrix3d matr = tempgsView->viewingMatrix();

        Normal = vRec->viewDirection();
        Normal.normalize();

        //OdGePoint2d center = vRec->centerPoint();
        //break;
      }

      pEnts.append(pEnt);
    }

    if (loopExit)
      break;
  }

  if (0 == pEnts.length())
    return;

  OdInt16 nTangentVisibility = -1;
  OdEdgeTypeFlags edgeVis = kRegular;

  try
  {
    nTangentVisibility = pIO->getKeyword(L"Visibility of tangent edges[Yes/No/Shortens]", L"Yes No Shortens", 0);

    switch (nTangentVisibility)
    {
    case 0:
      break;
    case 1:
      edgeVis = kTangent;
      break;
    case 2:
      edgeVis = kTangentShortend;
      break;
    default:
      return;
    }
  }
  catch (...)
  {
    bool bOk = false;
  }

  SectionType nSectionType = kFull_ST;

  try
  {
    //nSectionType = (SectionType)pIO->getKeyword(L"The type of section[Full/HalfFirst/SecondHalf/Offset/Aligned/ByObject/Circular/Rectangular]", L"Full HalfFirst SecondHalf Offset Aligned ByObject Circular Rectangular", 0); //TODO
    nSectionType = (SectionType)pIO->getKeyword(L"The type of section[Full/HalfFirst/SecondHalf/Offset/Rectangular]", L"Full HalfFirst SecondHalf Offset Rectangular", 0);
    // TODO Should be deleted after finish of all types
    if (nSectionType == kAligned_ST)
      nSectionType = kRectangular_ST;
  }
  catch (...)
  {
    bool bOk = false;
  }
  //Create DbSection  
  if (kRectangular_ST == nSectionType)
  {
    try
    {
      ptStart = pIO->getPoint(OD_T("Specify first corner:"), OdEd::kGptNoLimCheck | OdEd::kGptNoUCS);
      ptEnd = pIO->getPoint(OD_T("Specify opposite corner:"), OdEd::kGptNoLimCheck | OdEd::kGptNoUCS | OdEd::kGptRectFrame);
      ODA_ASSERT(ptStart.z == ptEnd.z);
      OdGePoint3d p1(ptStart.x, ptEnd.y, ptEnd.z);
      OdGePoint3d p2(ptEnd.x, ptStart.y, ptEnd.z);
      pp.append(ptStart);

      OdGePoint3d testPP = ptStart;
      testPP.transformBy(inverseViewTrans);
      ppInverse.insertAt(0, testPP);

      pp.append(p1);
      testPP = p1;
      testPP.transformBy(inverseViewTrans);
      ppInverse.insertAt(0, testPP);

      pp.append(ptEnd);
      testPP = ptEnd;
      testPP.transformBy(inverseViewTrans);
      ppInverse.insertAt(0, testPP);

      pp.append(p2);
      testPP = p2;
      testPP.transformBy(inverseViewTrans);
      ppInverse.insertAt(0, testPP);
    }
    catch (const OdEdCancel)
    {
      pIO->putString(OD_T("Canceled."));
    }
  }
  else while (true)
  {
    try
    {
      if (pp.length() == 0)
        ptSegmentStart = ptStart = ptEnd = pIO->getPoint(L"Specify first point of section");
      else if (pp.length() == 1)
        ptEnd = pIO->getPoint(L"Specify second point of section ", OdEd::kGptRubberBand | OdEd::kInpThrowEmpty, NULL);
      else
      {
        OdEdPointTrackerPtr pTracker;;

        if (nSectionType == kSecondHalf_ST || nSectionType == kOffset_ST || nSectionType == kHalfFirst_ST)
        {
          pTracker = OrthoRubberBand::create(ptSegmentStart, ptEnd);
          ptSegmentStart = ptEnd;
        }

        ptEnd = pIO->getPoint(L"Specify next point of section or press ENTER to finish ", OdEd::kGptRubberBand | OdEd::kInpThrowEmpty, NULL, OdString::kEmpty, pTracker.get());

        if (!pTracker.isNull())
          ptEnd = ((OrthoRubberBand *)pTracker.get())->getValue();
      }

    }
    catch (const OdEdCancel&)
    {
      break;
    }

    OdGePoint3d testPP = ptEnd;
    testPP.transformBy(inverseViewTrans);
    pp.append(testPP);
    ppInverse.insertAt(0, testPP);
    // create temporary lines to pSection visualization
    if (pp.length() > 1)
    {
      if (pp.length() > 3 && kPolygon_ST == nSectionType)
      {
        OdDbObjectId delId = linesId.getAt(linesId.length() - 1);
        OdDbEntityPtr pDelEnt = delId.openObject(OdDb::kForWrite);
        ODA_ASSERT(!pDelEnt.isNull());
        pDelEnt->erase();
      }

      OdDbLinePtr pNewLine = OdDbLine::createObject();
      pNewLine->setDatabaseDefaults(pDatabase);
      pNewLine->setNormal(Normal);
      pNewLine->setStartPoint(ptStart);
      pNewLine->setEndPoint(ptEnd);
      pSpace->appendOdDbEntity(pNewLine);
      linesId.append(pNewLine->objectId());
      ///////////////////////
      if (pp.length() > 2 && kPolygon_ST == nSectionType)
      {
        OdDbLinePtr pNewLine = OdDbLine::createObject();
        pNewLine->setDatabaseDefaults(pDatabase);
        pNewLine->setNormal(Normal);
        pNewLine->setStartPoint(ptEnd);
        pNewLine->setEndPoint(pp.getAt(0));
        pSpace->appendOdDbEntity(pNewLine);
        linesId.append(pNewLine->objectId());
      }

      if (kFull_ST == nSectionType)
        break;

      if (pp.length() > 2 && (kHalfFirst_ST == nSectionType || kSecondHalf_ST == nSectionType))
      {
        if (kHalfFirst_ST == nSectionType)
        {
          pp.removeLast();
          ppInverse.removeFirst();
        }

        if (kSecondHalf_ST == nSectionType)
        {
          pp.removeFirst();
          ppInverse.removeLast();
        }
        break;
      }

      ptStart = ptEnd;
      ++nLines;
    }
  }

  if (pp.length() > 1)
  {
    // delete tempopary lines
    for (unsigned int ier = 0; ier < linesId.length(); ier++)
    {
      OdDbObjectId delId = linesId.getAt(ier);/// ->erase();
      OdDbEntityPtr pDelEnt = delId.openObject(OdDb::kForWrite);

      if (!pDelEnt.isNull())
        pDelEnt->erase();
    }

    linesId.clear();

    if (pp.length() > 2 && kPolygon_ST == nSectionType)
      pp.append(pp.getAt(0));

    OdArray<OdDbEntityPtr> newEnts;
    OdArray<OdDbEntityPtr> intHatchEnts;
    OdArray<OdDbEntityPtr> backgroundEnts;
    OdArray<OdDbEntityPtr> curveVisibilityEdges;
    OdArray<OdDbEntityPtr> intBoundaryEnts;
    OdDbEntityPtr pEnt = pEnts.getAt(0);
    OdModelerGeometry* pGeom = getModelerGeometry(pEnt);

    OdResult isViewInfo = pGeom->getSectionViewInfo(pEnts, pp, Normal, backgroundEnts, intHatchEnts, intBoundaryEnts, curveVisibilityEdges, nSectionType, edgeVis);

    ODA_ASSERT(eOk == isViewInfo);
    OdGeMatrix3d rotateMatrix;
    OdGeVector3d segmentFirst;// = perpVector();
    OdGeVector3d perpToSegment;// = segmentFirst.perpVector();
    OdGeMatrix3d translateMatrix;

    if (pp.length() > 1)
    {
      OdGePoint3d center(pp[0].x + (pp[1].x - pp[0].x) / 2, pp[0].y + (pp[1].y - pp[0].y) / 2, pp[0].z = (pp[1].z - pp[0].z) / 2);

      segmentFirst = pp[1] - pp[0];
      perpToSegment = segmentFirst.perpVector();

      double length = perpToSegment.length() * 10;

      perpToSegment.setLength(length);
      rotateMatrix.setToRotation(-OdaPI / 2, segmentFirst, center);
      //caleMatrix.setToScaling(scale);
      translateMatrix.setToTranslation(perpToSegment);
    }

    for (unsigned int entNum = 0; entNum < intBoundaryEnts.length(); entNum++)
    {
      OdDbEntityPtr tempBackGr = intBoundaryEnts.getAt(entNum);

      tempBackGr->transformBy(rotateMatrix);
      tempBackGr->transformBy(translateMatrix);
      //newEnts.append(tempBackGr);
    }

    for (unsigned int entNum = 0; entNum < curveVisibilityEdges.length(); entNum++)
    {
      OdDbEntityPtr tempBackGr = curveVisibilityEdges.getAt(entNum);

      //tempBackGr->transformBy(rotateMatrix);
      //tempBackGr->transformBy(translateMatrix);
      newEnts.append(tempBackGr);
    }

    for (unsigned int entNum = 0; entNum < backgroundEnts.length(); entNum++)
    {
      OdDbEntityPtr tempBackGr = backgroundEnts.getAt(entNum);

      tempBackGr->transformBy(rotateMatrix);
      tempBackGr->transformBy(translateMatrix);
      newEnts.append(tempBackGr);
    }

    for (unsigned int entNum = 0; entNum < intHatchEnts.length(); entNum++)
    {
      OdDbEntityPtr tempFillEnt = intHatchEnts.getAt(entNum);

      tempFillEnt->transformBy(rotateMatrix);
      tempFillEnt->transformBy(translateMatrix);
      newEnts.append(tempFillEnt);
    }

    intHatchEnts.clear();
    backgroundEnts.clear();
    curveVisibilityEdges.clear();
    intBoundaryEnts.clear();

    isViewInfo = pGeom->getSectionViewInfo(pEnts, ppInverse, Normal, backgroundEnts, intHatchEnts, intBoundaryEnts, curveVisibilityEdges, nSectionType, edgeVis);
    OdGeVector3d segmentLast;

    if (ppInverse.length() > 1)
    {
      segmentLast = ppInverse[1] - ppInverse[0];
      perpToSegment = segmentLast.perpVector();
      double length = perpToSegment.length() * 10;
      perpToSegment.setLength(length);

      OdGePoint3d center(ppInverse[0].x + (ppInverse[1].x - ppInverse[0].x) / 2, ppInverse[0].y + (ppInverse[1].y - ppInverse[0].y) / 2, ppInverse[0].z = (ppInverse[1].z - ppInverse[0].z) / 2);

      rotateMatrix.setToRotation(-OdaPI / 2, segmentLast, center);
      //caleMatrix.setToScaling(scale);
      translateMatrix.setToTranslation(perpToSegment);
    }

    for (unsigned int entNum = 0; entNum < intBoundaryEnts.length(); entNum++)
    {
      OdDbEntityPtr tempBackGr = intBoundaryEnts.getAt(entNum);

      tempBackGr->transformBy(rotateMatrix);
      tempBackGr->transformBy(translateMatrix);
      //newEnts.append(tempBackGr);
    }

    for (unsigned int entNum = 0; entNum < curveVisibilityEdges.length(); entNum++)
    {
      OdDbEntityPtr tempBackGr = curveVisibilityEdges.getAt(entNum);

      //tempBackGr->transformBy(rotateMatrix);
      //tempBackGr->transformBy(translateMatrix);
      newEnts.append(tempBackGr);
    }

    for (unsigned int entNum = 0; entNum < backgroundEnts.length(); entNum++)
    {
      OdDbEntityPtr tempBackGr = backgroundEnts.getAt(entNum);

      tempBackGr->transformBy(rotateMatrix);
      tempBackGr->transformBy(translateMatrix);
      newEnts.append(tempBackGr);
    }

    for (unsigned int entNum = 0; entNum < intHatchEnts.length(); entNum++)
    {
      OdDbEntityPtr tempFillEnt = intHatchEnts.getAt(entNum);

      tempFillEnt->transformBy(rotateMatrix);
      tempFillEnt->transformBy(translateMatrix);
      newEnts.append(tempFillEnt);
    }

    ODA_ASSERT(eOk == isViewInfo);

    for (unsigned i = 0; i < newEnts.size(); ++i)
    {
      pMs->appendOdDbEntity(newEnts[i]);
    }

    //if ( isModelSpaceView && !pEnt.isNull() && newEnts.size() > 0 )
    //{
    //	OdDbBlockTableRecordPtr pBlk = pEnt->ownerId().openObject(OdDb::kForWrite);
    //	if (!pBlk.isNull())
    //	{
    //		pEnt = sourceEntity.openObject(OdDb::kForWrite);
    //		pEnt->erase();
    //	}
    //}
  }
  //Create DbSection End
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void _GetSection_func(OdEdCommandContext* pCmdCtx)
{
	OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
	OdDbDatabase* pDatabase = pDbCmdCtx->database();
	OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();
	OdDbBlockTableRecordPtr pMs = pDatabase->getModelSpaceId().safeOpenObject(OdDb::kForWrite);

	OdDbSelectionSetPtr pFirstSel = pIO->select(OD_T("Select first set of objects"), OdEd::kSelSingleEntity/*, 0, OdString::kEmpty, &filter*/);

	if (pFirstSel->numEntities() == 0)
	{
		pIO->putString(OD_T("Nothing selected."));
		return;
	}

	OdDbSelectionSetIteratorPtr pIter = pFirstSel->newIterator();
	while (!pIter->done())
	{
		OdDbObjectId objId = pIter->objectId();

		OdDbEntityPtr pEnt = objId.openObject();
		OdGePoint3dArray aPts;
		if (!pEnt.isNull() && pEnt->isKindOf(OdDb3dSolid::desc()) && eOk == pEnt->getGripPoints(aPts))
		{
			OdDbViewportTablePtr vTable = OdDbViewportTable::cast(pDatabase->getViewportTableId().openObject());
			OdDbViewportTableRecordPtr vRec = OdDbViewportTableRecord::cast(vTable->getAt(OD_T("*Active"), OdDb::kForRead));
			OdGeVector3d Normal = vRec->viewDirection();
			Normal.normalize();

			OdGePlane plane(aPts[0], Normal);
			OdDbRegionPtr pRegion;
			OdResult res = OdDb3dSolidPtr(pEnt)->getSection(plane, pRegion);
			ODA_ASSERT(res == eOk);
			if (res == eOk)
			{
				pMs->appendOdDbEntity(pRegion);
			}
			break;
		}
		pIter->next();
	}
}

void _thicken_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabase* pDatabase = pDbCmdCtx->database();
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();
  OdDbBlockTableRecordPtr pMs = pDatabase->getModelSpaceId().safeOpenObject(OdDb::kForWrite);

  OdDbSelectionSetPtr pFirstSel = pIO->select(OD_T("Select first set of objects"), OdEd::kSelSingleEntity/*, 0, OdString::kEmpty, &filter*/);

  if (pFirstSel->numEntities() == 0)
  {
    pIO->putString(OD_T("Nothing selected."));
    return;
  }

  OdDbSelectionSetIteratorPtr pIter = pFirstSel->newIterator();
  while ( !pIter->done() )
  {
    OdDbObjectId objId = pIter->objectId();

    OdDbEntityPtr pEnt = objId.openObject();
    OdGePoint3dArray aPts;
    if (!pEnt.isNull() && pEnt->isKindOf(OdDbSurface::desc()))
    {
      OdDb3dSolidPtr pSolid;
      OdResult res = OdDbSurfacePtr(pEnt)->thicken(0, false, pSolid);
      if(res == eOk && !pSolid.isNull())
        pMs->appendOdDbEntity(pSolid);
      break;
    }
    pIter->next();
  }
}

void _GetSlice_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabase* pDatabase = pDbCmdCtx->database();
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();
  OdDbBlockTableRecordPtr pMs = pDatabase->getModelSpaceId().safeOpenObject(OdDb::kForWrite);

  OdDbSelectionSetPtr pFirstSel = pIO->select(OD_T("Select first set of objects"), OdEd::kSelSingleEntity/*, 0, OdString::kEmpty, &filter*/);

  if (pFirstSel->numEntities() == 0)
  {
    pIO->putString(OD_T("Nothing selected."));
    return;
  }

  OdDbSelectionSetIteratorPtr pIter = pFirstSel->newIterator();
  while ( !pIter->done() )
  {
    OdDbObjectId objId = pIter->objectId();

    OdDbEntityPtr pEnt = objId.openObject(OdDb::kForWrite);
    if (!pEnt.isNull() && (pEnt->isKindOf(OdDb3dSolid::desc()) || pEnt->isKindOf(OdDbSurface::desc())))
    {
      OdGePoint3d ptBase  = pIO->getPoint(OD_T("Specify base point of plane:"));
      OdGePoint3d ptDraft = pIO->getPoint(OD_T("Specify point normal:"));
      OdGeVector3d Normal(ptDraft - ptBase);
      Normal.normalize();

      OdGePlane plane(ptBase, Normal);
      if (pEnt->isKindOf(OdDb3dSolid::desc()))
      {
        OdDb3dSolidPtr pSolid;
        OdResult res = OdDb3dSolidPtr(pEnt)->getSlice(plane, true, pSolid);
        ODA_ASSERT(res == eOk);
        if (res == eOk && !pSolid.isNull())
        {
          pMs->appendOdDbEntity(pSolid);
        }
      }
      if (pEnt->isKindOf(OdDbSurface::desc()))
      {
        OdDbSurfacePtr pSurf, pNewSurf;
        OdResult res = OdDbSurfacePtr(pEnt)->sliceByPlane(plane, pSurf, pNewSurf);
        ODA_ASSERT(res == eOk);
        if (res == eOk && !pSurf.isNull())
        {
          pMs->appendOdDbEntity(pSurf);
          if (!pNewSurf.isNull())
            pMs->appendOdDbEntity(pSurf);
        }
      }
      break;
    }
    pIter->next();
  }
}

void _GetSliceSurf_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabase* pDatabase = pDbCmdCtx->database();
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();
  OdDbBlockTableRecordPtr pMs = pDatabase->getModelSpaceId().safeOpenObject(OdDb::kForWrite);

  OdDbSelectionSetPtr pFirstSel = pIO->select(OD_T("Select 3dSolid for slice"), OdEd::kSelSingleEntity/*, 0, OdString::kEmpty, &filter*/);

  if (pFirstSel->numEntities() == 0)
  {
    pIO->putString(OD_T("no solids selected."));
    return;
  }

  OdDbSelectionSetPtr pSecondSel = pIO->select(OD_T("Select surface"), OdEd::kSelSingleEntity/*, 0, OdString::kEmpty, &filter*/);
  if (pSecondSel->numEntities() == 0)
  {
    pIO->putString(OD_T("no surfaces selected."));
    return;
  }

  OdDbSelectionSetIteratorPtr pIter = pFirstSel->newIterator(),
                              pIterSec = pSecondSel->newIterator();
  OdDbObjectId objId = pIter->objectId();
  OdDbEntityPtr pObj = objId.openObject(OdDb::kForWrite);
  OdDbObjectId objId2 = pIterSec->objectId();
  OdDbEntityPtr pSurf = objId2.openObject(OdDb::kForWrite);

  if (!pObj.isNull() && !pSurf.isNull() && (pObj->isKindOf(OdDb3dSolid::desc()) || pObj->isKindOf(OdDbSurface::desc())) && pSurf->isKindOf(OdDbSurface::desc()))
  {
    if (pObj->isKindOf(OdDb3dSolid::desc()))
    {
      OdDb3dSolidPtr pNegRetSolid;
      OdResult res = OdDb3dSolidPtr(pObj)->getSlice(OdDbSurfacePtr(pSurf), true, pNegRetSolid);
      ODA_ASSERT(res == eOk);
      if (res == eOk && !pNegRetSolid.isNull())
      {
        pMs->appendOdDbEntity(pNegRetSolid);
      }
    }
    if (pObj->isKindOf(OdDbSurface::desc()))
    {
      OdDbSurfacePtr pSurf2, pNewSurf;
      OdResult res = OdDbSurfacePtr(pObj)->sliceBySurface(OdDbSurfacePtr(pSurf), pSurf2, pNewSurf);
      ODA_ASSERT(res == eOk);
      if (res == eOk && !pSurf2.isNull())
      {
        pMs->appendOdDbEntity(pSurf2);
        if (!pNewSurf.isNull())
          pMs->appendOdDbEntity(pNewSurf);
      }
    }
  }
}

#include "DbMaterial.h"
#include "DbDictionary.h"
void _SetSubentMaterial_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();
  OdDbSelectionSetPtr pSel = pIO->select(OD_T("Select objects:"), OdEd::kSelSingleEntity |
    OdEd::kSelAllowSubents );

  OdDbDatabasePtr pDb    = pDbCmdCtx->database();
  OdDbDictionaryPtr pMatDic = pDb->getMaterialDictionaryId().safeOpenObject();

  if (pMatDic->has(OdString(OD_T("TestMaterial"))))
  {
    OdDbObjectId matId = pMatDic->getAt(OdString(OD_T("TestMaterial")));

    if (pSel->numEntities() != 0)
    {
      OdDbSelectionSetIteratorPtr pIter = pSel->newIterator();
      while ( !pIter->done() )
      {
        OdDbObjectId objId  = pIter->objectId();
        OdDbEntityPtr pEnt  = objId.openObject( OdDb::kForWrite );

        if( !pEnt.isNull() && pEnt->isKindOf(OdDb3dSolid::desc()))
        {
          if( pIter->subentCount() )
          {
            OdDbFullSubentPath subentPath;
            int count = pIter->subentCount();
            for( int i = 0; i < count; i++ )
            {
              if(pIter->getSubentity(i,subentPath))
              {
                OdDbObjectId matIdGet;

                OdGiMapper mapper;
                OdResult res = OdDb3dSolidPtr(pEnt)->getSubentMaterialMapper(subentPath.subentId(), mapper);
                res = OdDb3dSolidPtr(pEnt)->getSubentMaterial(subentPath.subentId(), matIdGet);

                OdGiMapper mapperSet;
                mapperSet.setAutoTransform(OdGiMapper::kObject);
                mapperSet.setProjection(OdGiMapper::kBox);
                mapperSet.setUTiling(OdGiMapper::kTile);
                res = OdDb3dSolidPtr(pEnt)->setSubentMaterialMapper(subentPath.subentId(), mapperSet);
                res = OdDb3dSolidPtr(pEnt)->setSubentMaterial(subentPath.subentId(), matId);
                pEnt->objectId().safeOpenObject();
              }
            }
          }
        }
        pIter->next();
      }
    }
  }
}

void _SeparateBody_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabase* pDatabase = pDbCmdCtx->database();
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();
  OdDbBlockTableRecordPtr pMs = pDatabase->getModelSpaceId().safeOpenObject(OdDb::kForWrite);

  OdDbSelectionSetPtr pFirstSel = pIO->select(OD_T("Select 3dSolid for slice"), OdEd::kSelSingleEntity);

  if (pFirstSel->numEntities() == 0)
  {
    pIO->putString(OD_T("no solid selected."));
    return;
  }

  OdDbSelectionSetIteratorPtr pIter = pFirstSel->newIterator();
  OdDbObjectId objId = pIter->objectId();
  OdDbEntityPtr p3dSolid = objId.openObject(OdDb::kForWrite);

  if (!p3dSolid.isNull() && p3dSolid->isKindOf(OdDb3dSolid::desc()))
  {
    OdDb3dSolidPtrArray newSolids;
    OdResult res = OdDb3dSolidPtr(p3dSolid)->separateBody(newSolids);
    if (res == eOk)
    {
      int size = newSolids.size();
      for (int i = 0; i < size; ++i)
      {
        pMs->appendOdDbEntity(newSolids[i]);
      }
    }
  }
}

void _createFromCurves_func(OdEdCommandContext* pCmdCtx)
{
#if defined(_TOOLKIT_IN_DLL_) && defined(WIN32)
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabase* pDatabase = pDbCmdCtx->database();
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();
  OdDbBlockTableRecordPtr pMs = pDatabase->getModelSpaceId().safeOpenObject(OdDb::kForWrite);

  OdDbSelectionSetPtr pFirstSel = pIO->select(OD_T("Select first set of objects"), OdEd::kSelSingleEntity/*, 0, OdString::kEmpty, &filter*/);

  if (pFirstSel->numEntities() == 0)
  {
    pIO->putString(OD_T("Nothing selected."));
    return;
  }

  OdRxObjectPtrArray curveSegments;

  OdDbSelectionSetIteratorPtr pIter = pFirstSel->newIterator();
  while ( !pIter->done() )
  {
    OdDbObjectId objId = pIter->objectId();

    OdRxObjectPtr pEnt(objId.openObject());
    if (!pEnt.isNull())
    {
      curveSegments.push_back(pEnt);
    }
    pIter->next();
  }

  OdRxObjectPtrArray regions;
  OdResult res = OdDbRegion::createFromCurves(curveSegments, regions);
  if (res == eOk)
  {
    for (unsigned int i = 0; i<regions.size(); ++i)
    {
      pMs->appendOdDbEntity(OdDbEntityPtr(regions[i]));
    }
  }
#endif
}

void _convertToRegion_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabase* pDatabase = pDbCmdCtx->database();
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();
  OdDbBlockTableRecordPtr pMs = pDatabase->getModelSpaceId().safeOpenObject(OdDb::kForWrite);

  OdDbSelectionSetPtr pFirstSel = pIO->select(OD_T("Select first set of objects"), OdEd::kSelSingleEntity/*, 0, OdString::kEmpty, &filter*/);

  if (pFirstSel->numEntities() == 0)
  {
    pIO->putString(OD_T("Nothing selected."));
    return;
  }

  OdDbSelectionSetIteratorPtr pIter = pFirstSel->newIterator();
  while ( !pIter->done() )
  {
    OdDbObjectId objId = pIter->objectId();

    OdDbEntityPtr pEnt = objId.openObject();
    if (!pEnt.isNull() && pEnt->isKindOf(OdDbSurface::desc()))
    {
      OdDbEntityPtrArray regions;
      OdResult res = OdDbSurfacePtr(pEnt)->convertToRegion(regions);
      ODA_ASSERT(res == eOk);
      if (res == eOk)
      {
        for (unsigned int i = 0; i<regions.size(); ++i)
        {
          pMs->appendOdDbEntity(regions[i]);
        }
      }
      break;
    }
    pIter->next();
  }
}

void _createFrom_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabase* pDatabase = pDbCmdCtx->database();
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();
  OdDbBlockTableRecordPtr pMs = pDatabase->getModelSpaceId().safeOpenObject(OdDb::kForWrite);

  OdDbSelectionSetPtr pFirstSel = pIO->select(OD_T("Select first set of objects"), OdEd::kSelSingleEntity/*, 0, OdString::kEmpty, &filter*/);

  if (pFirstSel->numEntities() == 0)
  {
    pIO->putString(OD_T("Nothing selected."));
    return;
  }

  OdDbSelectionSetIteratorPtr pIter = pFirstSel->newIterator();
  while ( !pIter->done() )
  {
    OdDbObjectId objId = pIter->objectId();

    OdDbEntityPtr pEnt = objId.openObject();
    if (!pEnt.isNull())
    {
      OdDbSurfacePtr pSurface;
      OdDbSurfacePtr pSurface2;
      OdResult res = pSurface2->createFrom(pEnt, pSurface);
      ODA_ASSERT(res == eOk);
      if (res == eOk)
      {
        pMs->appendOdDbEntity(pSurface);
      }
      break;
    }
    pIter->next();
  }
}

void _getMassProp_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabase* pDatabase = pDbCmdCtx->database();
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();
  OdDbBlockTableRecordPtr pMs = pDatabase->getModelSpaceId().safeOpenObject(OdDb::kForWrite);

  OdDbSelectionSetPtr pFirstSel = pIO->select(OD_T("Select first set of objects"), OdEd::kSelSingleEntity/*, 0, OdString::kEmpty, &filter*/);

  if (pFirstSel->numEntities() == 0)
  {
    pIO->putString(OD_T("Nothing selected."));
    return;
  }

  OdDbSelectionSetIteratorPtr pIter = pFirstSel->newIterator();
  while ( !pIter->done() )
  {
    OdDbObjectId objId = pIter->objectId();
    OdDbEntityPtr pEnt = objId.openObject();
    if (!pEnt.isNull() && pEnt->isKindOf(OdDb3dSolid::desc()))
    {
      double volume;
      OdGePoint3d centroid;
      double momInertia[3];
      double prodInertia[3];
      double prinMoments[3];
      OdGeVector3d prinAxes[3]; 
      double radiiGyration[3];
      OdGeExtents3d extents;
      OdDb3dSolidPtr(pEnt)->getMassProp(volume, centroid, momInertia, prodInertia, prinMoments, 
                                        prinAxes, radiiGyration, extents);
      OdString sLog;
      sLog.format(L"\n\nVolume: %s\nCentroid: %s %s %s\nMom. Of Inertia: %s %s %s\nProd. Of Inertia: %s %s %s\n", odDToStr(volume, 'f', 6).c_str(),
        odDToStr(centroid.x, 'f', 6).c_str(), odDToStr(centroid.y, 'f', 6).c_str(), odDToStr(centroid.z, 'f', 6).c_str(),
        odDToStr(momInertia[0], 'f', 6).c_str(), odDToStr(momInertia[1], 'f', 6).c_str(), odDToStr(momInertia[2], 'f', 6).c_str(),
        odDToStr(prodInertia[0], 'f', 6).c_str(), odDToStr(prodInertia[1], 'f', 6).c_str(), odDToStr(prodInertia[2], 'f', 6).c_str());
      pIO->putString(sLog);

      sLog.format(L"Priciple Moments: %s %s %s\nRadii Gyration: %s %s %s\nPrinciple Axes: \n   X Axis: %s %s %s\n",
        odDToStr(prinMoments[0], 'f', 6).c_str(), odDToStr(prinMoments[1], 'f', 6).c_str(), odDToStr(prinMoments[2], 'f', 6).c_str(),
        odDToStr(radiiGyration[0], 'f', 6).c_str(), odDToStr(radiiGyration[1], 'f', 6).c_str(), odDToStr(radiiGyration[2], 'f', 6).c_str(),
        odDToStr(prinAxes[0].x, 'f', 6).c_str(), odDToStr(prinAxes[0].y, 'f', 6).c_str(), odDToStr(prinAxes[0].z, 'f', 6).c_str());
      pIO->putString(sLog);

      sLog.format(L"   Y Axis: %s %s %s\n   Z Axis: %s %s %s\nExtmin: %s %s %s\nExtmax: %s %s %s\n",
        odDToStr(prinAxes[1].x, 'f', 6).c_str(), odDToStr(prinAxes[1].y, 'f', 6).c_str(), odDToStr(prinAxes[1].z, 'f', 6).c_str(),
        odDToStr(prinAxes[2].x, 'f', 6).c_str(), odDToStr(prinAxes[2].y, 'f', 6).c_str(), odDToStr(prinAxes[2].z, 'f', 6).c_str(),
        odDToStr(extents.minPoint().x, 'f', 6).c_str(), odDToStr(extents.minPoint().y, 'f', 6).c_str(), odDToStr(extents.minPoint().z, 'f', 6).c_str(),
        odDToStr(extents.maxPoint().x, 'f', 6).c_str(), odDToStr(extents.maxPoint().y, 'f', 6).c_str(), odDToStr(extents.maxPoint().z, 'f', 6).c_str());
      pIO->putString(sLog);
    }
    pIter->next();
  }
}

void _getAreaProp_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabase* pDatabase = pDbCmdCtx->database();
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();
  OdDbBlockTableRecordPtr pMs = pDatabase->getModelSpaceId().safeOpenObject(OdDb::kForWrite);

  OdDbSelectionSetPtr pFirstSel = pIO->select(OD_T("Select object"), OdEd::kSelSingleEntity/*, 0, OdString::kEmpty, &filter*/);

  if (pFirstSel->numEntities() == 0)
  {
    pIO->putString(OD_T("Nothing selected."));
    return;
  }

  OdDbSelectionSetIteratorPtr pIter = pFirstSel->newIterator();
  while ( !pIter->done() )
  {
    OdDbObjectId objId = pIter->objectId();
    OdDbEntityPtr pEnt = objId.openObject();
    if (!pEnt.isNull() && pEnt->isKindOf(OdDbRegion::desc()))
    {
      OdGePoint3d origin;
      OdGeVector3d xAxis,yAxis;
      double perimeter, area, momInertia[2], prodInertia, prinMoments[2], radiiGyration[2];
      OdGeVector2d prinAxes[2];
      OdGePoint2d extentsLow, centroid, extentsHigh;

      OdGePlane plane;
      OdDb::Planarity planarity;

      OdDbRegionPtr(pEnt)->getPlane(plane, planarity);
      plane.getCoordSystem(origin, xAxis, yAxis);
      //Acad "MASSPROP" uses UCS so we don't need pass plane parameters in getAreaProp
      OdDbRegionPtr(pEnt)->getAreaProp(OdGePoint3d(0, 0, 0), OdGeVector3d(1, 0, 0), OdGeVector3d(0, 1, 0), perimeter, area, centroid, momInertia, prodInertia, prinMoments,
        prinAxes, radiiGyration, extentsLow, extentsHigh );

      OdString sLog;
      sLog.format(L"\norigin: %s %s %s\nxAxis: %s %s %s\nyAxis: %s %s %s\nperimeter: %s\ncentroid: %s %s\n", 
        odDToStr(origin[0], 'f', 6).c_str(), odDToStr(origin[1], 'f', 6).c_str(), odDToStr(origin[2], 'f', 6).c_str(),
        odDToStr(xAxis[0], 'f', 6).c_str(), odDToStr(xAxis[1], 'f', 6).c_str(), odDToStr(xAxis[2], 'f', 6).c_str(),
        odDToStr(yAxis[0], 'f', 6).c_str(), odDToStr(yAxis[1], 'f', 6).c_str(), odDToStr(yAxis[2], 'f', 6).c_str(),
        odDToStr(perimeter, 'f', 6).c_str(), odDToStr(centroid.x, 'f', 6).c_str(), odDToStr(centroid.y, 'f', 6).c_str());
      pIO->putString(sLog);

      sLog.format(L"area: %s\nmomInertia: %s %s\nprodInertia: %s\nprinMoments: %s %s\nradiiGyration: %s %s\n",
        odDToStr(area, 'f', 6).c_str(), odDToStr(momInertia[0], 'f', 6).c_str(), odDToStr(momInertia[1], 'f', 6).c_str(),
        odDToStr(prodInertia, 'f', 6).c_str(), odDToStr(prinMoments[0], 'f', 6).c_str(), odDToStr(prinMoments[1], 'f', 6).c_str(),
        odDToStr(radiiGyration[0], 'f', 6).c_str(),  odDToStr(radiiGyration[1], 'f', 6).c_str());
      pIO->putString(sLog);

      sLog.format(L"Principle Axes: \n   X Axis: %s %s\n   Y Axis: %s %s\nExtmin: %s %s\nExtmax: %s %s\n",
        odDToStr(prinAxes[0].x, 'f', 6).c_str(), odDToStr(prinAxes[0].y, 'f', 6).c_str(),
        odDToStr(prinAxes[1].x, 'f', 6).c_str(), odDToStr(prinAxes[1].y, 'f', 6).c_str(),
        odDToStr(extentsLow.x, 'f', 6).c_str(),  odDToStr(extentsLow.y, 'f', 6).c_str(),
        odDToStr(extentsHigh.x, 'f', 6).c_str(), odDToStr(extentsHigh.y, 'f', 6).c_str());
      pIO->putString(sLog);
    }
    pIter->next();
  }
}

//
// acisout of all ModelSpace entities.
//

#include "DbBody.h"
#include "DbRegion.h"
#include "Db3dSolid.h"
#include "DbSurface.h"

TOOLKIT_EXPORT OdRxClassPtr odrxGetModelerGeometryCreatorService();

void _AcisOutModelSpace_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabase* pDatabase = pDbCmdCtx->database();
  OdArray<OdModelerGeometryPtr> models;
  OdRxClassPtr pService = odrxGetModelerGeometryCreatorService();
  if (!pService.get())    
    return;

  OdModelerGeometryCreatorPtr pCreator = pService->create();
  if (!pCreator.get())
    return;

  OdDbBlockTableRecordPtr pMs = pDatabase->getModelSpaceId().safeOpenObject(OdDb::kForWrite);

  OdDbEntityPtrArray entityArray;

  OdDbObjectIteratorPtr pIter = pMs->newIterator();
  while (!pIter->done())
  {
    OdDbEntityPtr pE = OdDbEntityPtr(pIter->objectId().safeOpenObject());
    if (!pE.isNull())
      entityArray.append(pE);

    if (pE->isKindOf(OdDbBody::desc()))
    {
      OdSmartPtr<OdStreamBuf> pMemBuf = OdMemoryStream::createNew();
      OdResult res = OdDbBodyPtr(pE)->acisOut(pMemBuf);
      ODA_ASSERT(res == eOk);
      if (res == eOk)
      {
        pMemBuf->rewind();
        res = pCreator->createModeler(models, pMemBuf);
        ODA_ASSERT(res == eOk);
      }
    } else if (pE->isKindOf(OdDbBody::desc()))
    {
      OdSmartPtr<OdStreamBuf> pMemBuf = OdMemoryStream::createNew();
      OdResult res = OdDbBodyPtr(pE)->acisOut(pMemBuf);
      ODA_ASSERT(res == eOk);
      if (res == eOk)
      {
        pMemBuf->rewind();
        res = pCreator->createModeler(models, pMemBuf);
        ODA_ASSERT(res == eOk);
      }
    } else if (pE->isKindOf(OdDb3dSolid::desc()))
    {
      OdSmartPtr<OdStreamBuf> pMemBuf = OdMemoryStream::createNew();
      OdResult res = OdDb3dSolidPtr(pE)->acisOut(pMemBuf);
      ODA_ASSERT(res == eOk);
      if (res == eOk)
      {
        pMemBuf->rewind();
        res = pCreator->createModeler(models, pMemBuf);
        ODA_ASSERT(res == eOk);
      }
    } else if (pE->isKindOf(OdDbRegion::desc()))
    {
      OdSmartPtr<OdStreamBuf> pMemBuf = OdMemoryStream::createNew();
      OdResult res = OdDbRegionPtr(pE)->acisOut(pMemBuf);
      ODA_ASSERT(res == eOk);
      if (res == eOk)
      {
        pMemBuf->rewind();
        res = pCreator->createModeler(models, pMemBuf);
        ODA_ASSERT(res == eOk);
      }
    } else if (pE->isKindOf(OdDbSurface::desc()))
    {
      OdSmartPtr<OdStreamBuf> pMemBuf = OdMemoryStream::createNew();
      OdResult res = OdDbSurfacePtr(pE)->acisOut(pMemBuf);
      ODA_ASSERT(res == eOk);
      if (res == eOk)
      {
        pMemBuf->rewind();
        res = pCreator->createModeler(models, pMemBuf);
        ODA_ASSERT(res == eOk);
      }
    }

    pIter->step();
  }

  OdResult result;
  result = OdDbBody::acisOut("c:\\testsat400.sat", entityArray, kAf_ASCII_400|kAfTypeIndexed);
  ODA_ASSERT(result == eOk);
  
  result = OdDbBody::acisOut("c:\\testsat21200.sat", entityArray, kAf_ASCII_21200|kAfTypeIndexed);
  ODA_ASSERT(result == eOk);

  result = OdDbBody::acisOut("c:\\testsat21500.sat", entityArray, kAf_ASCII_21500|kAfTypeIndexed);
  ODA_ASSERT(result == eOk);

  OdStreamBufPtr wrFile 
    = odSystemServices()->createFile("c:\\testsat400_M.sat", Oda::kFileWrite, Oda::kShareDenyNo, Oda::kCreateAlways);
  result = pCreator->createSat(models, wrFile, kAf_ASCII_400|kAfTypeIndexed);
  ODA_ASSERT(result == eOk);

  OdStreamBufPtr wrFile2
    = odSystemServices()->createFile("c:\\testsat21200_M.sat", Oda::kFileWrite, Oda::kShareDenyNo, Oda::kCreateAlways);
  result = pCreator->createSat(models, wrFile2, kAf_ASCII_21200|kAfTypeIndexed);
  ODA_ASSERT(result == eOk);

  OdStreamBufPtr wrFile3
    = odSystemServices()->createFile("c:\\testsat21500_M.sat", Oda::kFileWrite, Oda::kShareDenyNo, Oda::kCreateAlways);
  result = pCreator->createSat(models, wrFile3, kAf_ASCII_21500|kAfTypeIndexed);
  ODA_ASSERT(result == eOk);
}

//
// set color of all ModelSpace solids.
//
void _Set3dSolidsColor_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabase* pDatabase = pDbCmdCtx->database();

  OdDbBlockTableRecordPtr pMs = pDatabase->getModelSpaceId().safeOpenObject(OdDb::kForWrite);

  OdDbObjectIteratorPtr pIter = pMs->newIterator();
  while (!pIter->done())
  {
    OdDbObjectPtr pObj = pIter->objectId().safeOpenObject(OdDb::kForWrite);
    if (pObj->isKindOf(OdDb3dSolid::desc()))
    {
      /*OdCmColor col;
      col.setRGB(125,15,100);
      OdDb3dSolidPtr(pObj)->setColor(col);*/
      OdDb3dSolidPtr(pObj)->setColorIndex(212);
    }
    else if (pObj->isKindOf(OdDbBody::desc()))
    {
      OdDbBodyPtr(pObj)->setColorIndex(212);
    }
    else if (pObj->isKindOf(OdDbRegion::desc()))
    {
      OdDbRegionPtr(pObj)->setColorIndex(212);
    }

    pIter->step();
  }
}

#endif


#if 0
#include "DbMline.h"
#include "Db2dPolyline.h"
#include "Db2dVertex.h"
void extern_debugcommand(COdaMfcAppDoc* pDoc, CDwgView* pView)
{
  OdDbObjectId id;
  HTREEITEM hItem = pView->m_ObjectTree.GetSelectedItem();
  if (hItem)
  {
    id = (OdDbStub*)(pView->m_ObjectTree.GetItemData(hItem));
  }
  OdDbMlinePtr pEnt = OdDbMline::cast(id.openObject(OdDb::kForWrite));
  if(pEnt.get())
  {
    OdRxObjectPtrArray entitySet;
    OdResult result = pEnt->explode(entitySet);

    for (unsigned i=0; i<entitySet.size(); ++i)
    {
      OdDb2dPolylinePtr pPline = OdDb2dPolyline::cast(entitySet[i]);
      if (!pPline.isNull())
      {
        OdDbObjectIteratorPtr pIter = pPline->vertexIterator();
        while (!pIter->done())
        {
          OdDb2dVertexPtr pVert = pIter->entity();
          bool isErased  = pVert->isErased();
          ODA_ASSERT(!isErased);
          OdResult res = pVert->erase(!isErased);
          ODA_ASSERT(res == eNoDatabase);
          pIter->step();
        }
      }
    }
  }
}
#endif


/*
    AcDbTable creation test
*/
#include "DbTable.h"
#include "DbMText.h"
#include "DbLine.h"
#include "DbSolid.h"
#include "DbBlockReference.h"
#include "DbBlockTableRecord.h"
#include "DbAttributeDefinition.h"
#include "DbDictionary.h"

#define TABLE_COLUMN_WIDTH  15.0
#define TABLE_ROW_HEIGHT    8.0
#define TABLE_NUM_COLUMN    2
#define TABLE_NUM_ROW       3

void copyTableOverides(OdDbTablePtr source, OdDbTablePtr target, OdDb::RowType rType)
{
  target->setTextStyle(source->textStyle(rType), rType);
  target->setAlignment(source->alignment(rType), rType);
  target->setTextHeight(source->textHeight(rType), rType);
  target->setContentColor(source->contentColor(rType), rType);
  target->setBackgroundColorNone(source->isBackgroundColorNone(rType), rType);
  target->setBackgroundColor(source->backgroundColor(rType), rType);

  target->setGridColor(source->gridColor(OdDb::kHorzTop, rType), OdDb::kHorzTop, rType );
  target->setGridColor(source->gridColor(OdDb::kHorzInside, rType), OdDb::kHorzInside,rType );
  target->setGridColor(source->gridColor(OdDb::kHorzBottom, rType), OdDb::kHorzBottom, rType );
  target->setGridColor(source->gridColor(OdDb::kVertLeft, rType), OdDb::kVertLeft, rType );
  target->setGridColor(source->gridColor(OdDb::kVertInside, rType), OdDb::kVertInside, rType );
  target->setGridColor(source->gridColor(OdDb::kVertRight, rType), OdDb::kVertRight, rType );


  // target->setgridLineWeight()
  target->setGridLineWeight(source->gridLineWeight(OdDb::kHorzTop, rType), OdDb::kHorzTop, rType );
  target->setGridLineWeight(source->gridLineWeight(OdDb::kHorzInside, rType), OdDb::kHorzInside,rType );
  target->setGridLineWeight(source->gridLineWeight(OdDb::kHorzBottom, rType), OdDb::kHorzBottom, rType );
  target->setGridLineWeight(source->gridLineWeight(OdDb::kVertLeft, rType), OdDb::kVertLeft, rType );
  target->setGridLineWeight(source->gridLineWeight(OdDb::kVertInside, rType), OdDb::kVertInside, rType );
  target->setGridLineWeight(source->gridLineWeight(OdDb::kVertRight, rType), OdDb::kVertRight, rType );

}

void _testTable_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbSelectionSetPtr ss = pDbCmdCtx->dbUserIO()->select(L"", OdEd::kSelAllowObjects|OdEd::kSelAllowInactSpaces|OdEd::kSelSingleEntity);
  if (ss->numEntities()==0)
    return;
  OdDbSelectionSetIteratorPtr pIter = ss->newIterator();
  OdDbObjectId  objId = pIter->objectId();
  OdDbObjectPtr obj = objId.openObject(OdDb::kForWrite);

  if ( obj->isKindOf(OdDbBlockTableRecord::desc()) )
  { // dump table block
    OdDbBlockTableRecordPtr  pRecord = OdDbBlockTableRecord::cast(obj);
    OdString name = pRecord->getName();
    if ( name.getAt(0) == '*' && name.getAt(1) == 'T' )
    {
      OdDbObjectIteratorPtr pIter = pRecord->newIterator();
      while (!pIter->done())
      {
        OdDbObjectPtr obj1 = pIter->objectId().safeOpenObject();
        if ( obj1->isKindOf(OdDbLine::desc()) )
        {
          OdDbLinePtr line = OdDbLine::cast(obj1);
          ODA_TRACE2("_line  %6.2f, %6.2f, 0.0", line->startPoint().x, line->startPoint().y);
          ODA_TRACE2(" %6.2f, %6.2f, 0.0 \n", line->endPoint().x, line->endPoint().y);
          ODA_TRACE2("color - %d Lw - %d\n", line->color().colorIndex(), line->lineWeight());
        }
        else if ( obj1->isKindOf(OdDbMText::desc()) )
        {
          OdDbMTextPtr mtext = OdDbMText::cast(obj1);
          ODA_TRACE1("mText (%ls)", mtext->contents().c_str());
          ODA_TRACE2(" position  %6.2f, %6.2f, 0.0\n", mtext->location().x, mtext->location().y);
          ODA_TRACE2("Vert Mode %d, horiz mode %d\n", mtext->verticalMode(), mtext->horizontalMode() );
          ODA_TRACE2("color - %d H - %6.2f\n", mtext->color().colorIndex(), mtext->textHeight());
        }
        else if ( obj1->isKindOf(OdDbSolid::desc()) )
        {
          OdDbSolidPtr solid = OdDbSolid::cast(obj1);
          OdGePoint3d pntRes;
          ODA_TRACE0("_solid\n");
          for ( int i = 0; i < 4; i ++)
          {
            solid->getPointAt(i, pntRes);
            ODA_TRACE2(" %6.2f, %6.2f, 0.0\n", pntRes.x, pntRes.y);
          }
        } else if ( obj1->isKindOf(OdDbBlockReference::desc()) )
        {
          OdDbBlockReferencePtr blk = OdDbBlockReference::cast(obj1);
          ODA_TRACE0("_blockreference \n");
          ODA_TRACE2("_position   %6.2f, %6.2f,0.0 \n", blk->position().x, blk->position().y);
        }
        pIter->step();
      }
    }
  } else if ( obj->isKindOf(OdDbTable::desc()) )
  { // Create new table from current table
    OdDbTablePtr table = OdDbTable::cast(obj);
    //table->recomputeTableBlock();
    try
    {
      OdDbBlockTableRecordPtr  pRecord = pDbCmdCtx->database()->getModelSpaceId().safeOpenObject(OdDb::kForWrite);
      OdDbObjectId  nObjId;
      OdDbTablePtr  pNewTable;
      pNewTable = OdDbTable::createObject();

      pNewTable->setDatabaseDefaults(pRecord->database());
      pNewTable->setTableStyle(table->tableStyle());
      pNewTable->setColor(table->color());
      pNewTable->setNumColumns(table->numColumns());
      pNewTable->setNumRows(table->numRows());
      pNewTable->generateLayout();
      OdUInt32 i, j;
      for ( i = 0; i < table->numRows(); i++)
        pNewTable->setRowHeight(i, table->rowHeight(i));

      for ( i = 0; i < table->numColumns(); i++)
        pNewTable->setColumnWidth(i, table->columnWidth(i));
      OdGePoint3d pos = table->position();
      pos.y += table->height() + 2.0;
      pNewTable->setPosition(pos);
      pNewTable->setDirection(table->direction());
      pNewTable->setNormal(table->normal());
	    // Merge Title row. Note that when you create table using UI the title row is merged
	    // automatically but when adding using API, we will have to explicitly merge the cells
  	  // pNewTable->mergeCells(0,0,0,pNewTable->numColumns() - 1);

      // table  overrides
      copyTableOverides(table, pNewTable, OdDb::kTitleRow);
      copyTableOverides(table, pNewTable, OdDb::kHeaderRow);
      copyTableOverides(table, pNewTable, OdDb::kDataRow);

	    for ( i = 0; i < table->numRows(); i++ )
	    {
		    for ( j = 0; j < table->numColumns(); j++ )
		    {
          OdUInt32 minRow, maxRow, minCol, maxCol;
			    bool b = table->isMergedCell(i, j, &minRow, &maxRow, &minCol, &maxCol );
          if ( b )
          {
            pNewTable->mergeCells(minRow, maxRow, minCol, maxCol);
          }
          pNewTable->setContentColor(i, j, table->contentColor(i, j));
          pNewTable->setBackgroundColorNone(i, j, table->isBackgroundColorNone(i, j));
          pNewTable->setBackgroundColor(i, j, table->backgroundColor(i, j));
          pNewTable->setAlignment(i, j, table->alignment(i,j));
          if ( table->cellType(i, j) == OdDb::kTextCell )
          {
            pNewTable->setTextStyle(i, j, table->textStyle(i,j));
            pNewTable->setTextString(i, j, table->textString(i, j));
            pNewTable->setTextHeight(i, j, table->textHeight(i, j));
            pNewTable->setTextRotation(i, j, table->textRotation(i, j));
          } else
          {
            pNewTable->setCellType(i,j, OdDb::kBlockCell);
            pNewTable->setBlockTableRecordId(i, j, table->blockTableRecordId(i, j) );
            pNewTable->setBlockScale(i, j, table->blockScale(i, j));
            pNewTable->setAutoScale(i, j, table->isAutoScale(i, j));
            pNewTable->setBlockRotation(i, j, table->blockRotation(i, j));

            OdDbBlockTableRecordPtr pInsBlock = OdDbBlockTableRecord::cast(table->blockTableRecordId(i,j).safeOpenObject());
            if ( pInsBlock->hasAttributeDefinitions() )
            {
              OdDbObjectIteratorPtr pIt = pInsBlock->newIterator();
              while (!pIt->done())
              {
                OdDbEntityPtr pEntity = pIt->entity();
                if ( pEntity->isKindOf(OdDbAttributeDefinition::desc()) )
                {
                  OdDbAttributeDefinitionPtr  pAttDef;
                  pAttDef = pEntity;
                  OdString str;
                  table->getBlockAttributeValue(i,j,pAttDef->objectId(), str);
                  pNewTable->setBlockAttributeValue(i,j,pAttDef->objectId(), str);
                }
                pIt->step();
              }
            }
          }

          pNewTable->setGridColor(i, j, OdDb::kTopMask, table->gridColor(i, j, OdDb::kTopMask));
          pNewTable->setGridColor(i, j, OdDb::kRightMask, table->gridColor(i, j, OdDb::kRightMask));
          pNewTable->setGridColor(i, j, OdDb::kBottomMask, table->gridColor(i, j, OdDb::kBottomMask));
          pNewTable->setGridColor(i, j, OdDb::kLeftMask, table->gridColor(i, j, OdDb::kLeftMask));

          pNewTable->setGridLineWeight(i, j, OdDb::kTopMask, table->gridLineWeight(i, j, OdDb::kTopMask));
          pNewTable->setGridLineWeight(i, j, OdDb::kRightMask,table->gridLineWeight(i, j, OdDb::kRightMask));
          pNewTable->setGridLineWeight(i, j, OdDb::kBottomMask, table->gridLineWeight(i, j, OdDb::kBottomMask));
          pNewTable->setGridLineWeight(i, j, OdDb::kLeftMask, table->gridLineWeight(i, j, OdDb::kLeftMask));

          pNewTable->setGridVisibility(i, j, OdDb::kTopMask, table->gridVisibility(i, j, OdDb::kTopMask));
          pNewTable->setGridVisibility(i, j, OdDb::kRightMask,table->gridVisibility(i, j, OdDb::kRightMask));
          pNewTable->setGridVisibility(i, j, OdDb::kBottomMask, table->gridVisibility(i, j, OdDb::kBottomMask));
          pNewTable->setGridVisibility(i, j, OdDb::kLeftMask, table->gridVisibility(i, j, OdDb::kLeftMask));
        }
	    }

      pNewTable->setFlowDirection(table->flowDirection());
      nObjId = pRecord->appendOdDbEntity(pNewTable);
      // pNewTable->recomputeTableBlock();

      pNewTable->recomputeTableBlock();
    }
    catch (OdError &e)
    {
      OdString msg = e.description();
      pDbCmdCtx->database()->appServices()->warning(msg);
    }
    /*
    pNewTable->setWidth(table->width()+30.0);
    pNewTable->setHeight(table->height()+20.0);
    pNewTable->recomputeTableBlock();
    */
  } else // Create new table directly
  {
    OdDbObjectId  nObjId;
    OdDbTablePtr  pNewTable;

    // extern_debugcommand1(pDoc, pView);

    // return;
    //  open model space for write
    try
    {
      OdDbBlockTableRecordPtr  pRecord = pDbCmdCtx->database()->getModelSpaceId().safeOpenObject(OdDb::kForWrite);

      //  create object itself
      pNewTable = OdDbTable::createObject();

      OdDbDictionaryPtr pDict = pDbCmdCtx->database()->getNamedObjectsDictionaryId().safeOpenObject();
      OdDbObjectId tblId = pDict->getAt(OdString(OD_T("ACAD_TABLESTYLE")));
      OdDbDictionaryPtr pTblSt =  tblId.safeOpenObject();
      OdDbObjectId stId = pTblSt->getAt(OD_T("Standard"));
      pNewTable->setTableStyle(stId);
      pNewTable->setDatabaseDefaults(pRecord->database());
      pNewTable->setNumColumns(TABLE_NUM_COLUMN);
      pNewTable->setNumRows(TABLE_NUM_ROW);
      pNewTable->generateLayout();

      pNewTable->setColumnWidth(TABLE_COLUMN_WIDTH);
      pNewTable->setRowHeight(TABLE_ROW_HEIGHT);
      OdGePoint3d insPoint(1.4, 1.4, 0);
      pNewTable->setPosition(insPoint);


      // pNewTable->recomputeTableBlock();

      nObjId = pRecord->appendOdDbEntity(pNewTable);

      pNewTable->recomputeTableBlock();

      pNewTable->insertRows(3, 20, 2);
      pNewTable->insertColumns(2, 10, 2);
      pNewTable->generateLayout();
      pNewTable->mergeCells(0,0,0,pNewTable->numColumns() - 1);
      pNewTable->recomputeTableBlock();
    }
    catch (OdError &e)
    {
      OdString msg = e.description();
      pDbCmdCtx->database()->appServices()->warning(msg);
    }
  }

}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////    custom commands are below
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "DbBlockTable.h"
#include "DbIdMapping.h"

void _XrefStatus_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  for (OdDbSelectionSetIteratorPtr it = pDbCmdCtx->dbUserIO()->select(L"Select xref block", 
    OdEd::kSelAllowObjects|OdEd::kSelAllowInactSpaces)->newIterator(); !it->done(); it->next())
  {
    OdDbObjectId id = it->objectId();
    try
    {
      OdDbBlockTableRecordPtr pBlock = OdDbBlockTableRecord::cast(it->objectId().safeOpenObject());

      OdString sStatus;
      switch(pBlock->xrefStatus())
      {
      case OdDb::kXrfUnloaded :
        sStatus = OD_T("Unloaded");
        break;
      case OdDb::kXrfNotAnXref:
        sStatus = OD_T("NotAnXref");
        break;
      case OdDb::kXrfResolved :
        sStatus = OD_T("Resolved");
        break;
      case OdDb::kXrfUnreferenced:
        sStatus = OD_T("Unreferenced");
        break;
      case OdDb::kXrfFileNotFound :
        sStatus = OD_T("FileNotFound");
        break;
      case OdDb::kXrfUnresolved:
        sStatus = OD_T("Unresolved");
        break;
      }
      OdString sMsg;
      sMsg.format(L"name: \"%ls\"\n\nisUnloaded() = %ls\npathName()   = %ls\nxrefStatus() = %ls\n"
        , pBlock->getName().c_str()
        , pBlock->isUnloaded() ? OD_T("true") : OD_T("false")
        , pBlock->pathName().c_str()
        , sStatus.c_str());
      pDbCmdCtx->dbUserIO()->putString(sMsg);
    }
    catch (OdError &e)
    {
      OdString msg = e.description();
      pDbCmdCtx->dbUserIO()->putString(msg);
    }
  }
}

#if 0
#include "DbPolygonMesh.h"
void extern_debugcommand(COdaMfcAppDoc* pDoc, CDwgView* pView)
{
  // This procedure performs AutoCAD EXPLODE command for selected entity
  HTREEITEM hItem = pView->m_ObjectTree.GetSelectedItem();
  if (hItem)
  {
    OdDbObjectId id = (OdDbStub*)(pView->m_ObjectTree.GetItemData(hItem));
    OdDbPolygonMeshPtr pMesh = OdDbEntity::cast(id.openObject(OdDb::kForWrite));
    if (!pMesh.isNull())
    {
//      pMesh->database()->startUndoRecord();
      pMesh->straighten();
      pView->fillTree();
    }
  }
}
#endif

#if 0
#include "DbHatch.h"
#include "DbArc.h"
#include "DbLine.h"
void extern_debugcommand(COdaMfcAppDoc* pDoc, CDwgView* pView)
{
  OdDbHatchPtr  pHatch;
  OdDbArcPtr    pArc;
  OdDbLinePtr   pLine;
  OdGePoint3d   point;
  OdDbObjectId  oId;

  try
  {
    OdDbObjectIdArray       aObjIds;
    OdDbBlockTableRecordPtr pMs = pDoc->getModelSpaceId().safeOpenObject(OdDb::kForWrite);

    //  create two elements which will be  hatch boundary
    pArc = OdDbArc::createObject();
    pArc->setDatabaseDefaults(pMs->database());

    point.x = 5.0; point.y = 3.0; point.z = 0.0;
    pArc->setCenter(point);
    pArc->setRadius(30.0);
    pArc->setStartAngle(0.0);
    pArc->setEndAngle(OdaPI);

    oId = pMs->appendOdDbEntity(pArc);
    aObjIds.push_back(oId);

    pLine = OdDbLine::createObject();
    pLine->setDatabaseDefaults(pMs->database());

    pArc->getEndPoint(point);
    pLine->setStartPoint(point);

    pArc->getStartPoint(point);
    pLine->setEndPoint(point);

    oId = pMs->appendOdDbEntity(pLine);
    aObjIds.push_back(oId);

    pHatch = OdDbHatch::createObject();
    pHatch->setDatabaseDefaults(pMs->database());
    pHatch->setPattern(OdDbHatch::kPreDefined, OD_T("ANGLE"));
    pHatch->setAssociative(true);
    pHatch->setPatternScale(2.0);

    pMs->appendOdDbEntity(pHatch);

    pHatch->appendLoop(OdDbHatch::kExternal, aObjIds);
    pHatch->evaluateHatch();
  }
  catch (OdError &e)
  {
    OdString msg = e.description();
    pDoc->appServices()->warning(msg);
  }
  return;
}

#endif

#if 0
#include "DbLayout.h"
void extern_debugcommand(COdaMfcAppDoc* pDoc, CDwgView* pView)
{
  OdDbDictionaryPtr pLayoutDict = pDoc->getLayoutDictionaryId().safeOpenObject();
  OdDbDictionaryIteratorPtr pIter = pLayoutDict->newIterator();

  while (!pIter->done())
  {
    OdDbObjectId LayoutId = pIter->objectId();
    OdDbLayoutPtr pLayout = LayoutId.safeOpenObject();

    //TRACE (OD_T("%s\n"), (const char *) pLayout->getLayoutName());

    OdDbObjectId viewportId;

    try
    {
      viewportId = pLayout->overallVportId();

      TRACE (OD_T("   Id: <%p>\n"), (void*) viewportId);
    }
    catch(OdError& e)
    {
      //TRACE (OD_T("   Error: <%s>\n"), (const char *) e.description());
    }

    pIter->next();
  }
}
#endif


#if 0
void extern_debugcommand(COdaMfcAppDoc* pDoc, CDwgView* pView)
{
  OdString str = pDoc->getPROJECTNAME();
  int i = pDoc->getINDEXCTL();
  bool b = pDoc->getXCLIPFRAME();

  pDoc->setPROJECTNAME(OD_T("My Project"));
  pDoc->setINDEXCTL(2);
  pDoc->setXCLIPFRAME(true);

  str = pDoc->getPROJECTNAME();
  i = pDoc->getINDEXCTL();
  b = pDoc->getXCLIPFRAME();

  pDoc->setPROJECTNAME(OD_T("My Project Next"));
  pDoc->setINDEXCTL(3);
  pDoc->setXCLIPFRAME(false);
}
#endif

#if 0
#include "DbMText.h"
#include "DbText.h"

int getTextFrag(OdDbMTextFragment *frag, void *param)
{
  ODA_TRACE0("---\n");
  ODA_TRACE1(":%ls:\n", frag->text.c_str());
  ODA_TRACE2("   PosX = %f, PosY = %f", frag->location.x, frag->location.y);
  ODA_TRACE2("   ExtX = %f, ExtY = %f\n", frag->extents.x, frag->extents.y);
  ODA_TRACE2("   Name = %ls, FontName = %ls\n", frag->font.c_str(), frag->fontname.c_str());
  // if ( !frag->fontname.isEmpty())
    ODA_TRACE1("CharSet = %d\n", frag->charset );
  if ( frag->text.getLength() == 1 && frag->text.getAt(0) == 0x9 )
    ODA_TRACE0("Tab\n");
  if ( frag->lineBreak )
    ODA_TRACE0("Last frag in line.\n");
  if ( frag->newParagraph )
    ODA_TRACE0("\nNew paragraph.\n");

  return 1;
}

void extern_debugcommand(COdaMfcAppDoc* pDoc, CDwgView* pView)
{
  OdDbObjectId  objId = pView->getCurObjectId();
  OdDbObjectPtr obj = objId.openObject(OdDb::kForWrite);
  OdDbMTextPtr mText = OdDbMText::cast(obj);
  if ( !mText.isNull() )
  {
    OdString content =  mText->contents();

    mText->explodeFragments(getTextFrag, NULL);
/*
    OdDbMTextIndents indents;
    mText->getParagraphsIndent(indents);
    ODA_TRACE1("Tab step %f \n", mText->textHeight()*4.0 );
    for ( OdUInt32 i = 0; i < indents.size(); i++)
    {
      ODA_TRACE2("\nParagraph %f  first line %f\n", indents[i].paragraphInd, indents[i].firstLineInd) ;
      ODA_TRACE0("Tabs:\n");
      for ( OdUInt32 j = 0; j < indents[i].tabs.size(); j++)
        ODA_TRACE1("  %f\n", indents[i].tabs[j] );
    }
*/
#if 0
    OdGePoint3dArray array;
    ODA_TRACE2("Actual height %6.4f width %6.4f\n", mText->actualHeight(), mText->actualWidth());
    mText->getBoundingPoints(array);
    ODA_TRACE2("top left     %6.4f  %6.4f ", array[0].x, array[0].y );
    ODA_TRACE1(" %6.4f\n", array[0].z );
    ODA_TRACE2("top right    %6.4f  %6.4f ", array[1].x, array[1].y );
    ODA_TRACE1(" %6.4f\n", array[1].z );
    ODA_TRACE2("bottom left  %6.4f  %6.4f ", array[2].x, array[2].y );
    ODA_TRACE1(" %6.4f\n", array[2].z );
    ODA_TRACE2("bottom right %6.4f  %6.4f ", array[3].x, array[3].y );
    ODA_TRACE1(" %6.4f\n", array[3].z );

    // OdDbMText::AttachmentPoint
    OdUInt8 attPoint = mText->attachment();
    OdGePoint3d loc = mText->location();
    if ( (OdDbMText::AttachmentPoint)attPoint < OdDbMText::AttachmentPoint::kBottomRight )
    {
      attPoint += 1;
      ODA_TRACE2("Location befor X %6.4f  Y %6.4f\n", loc.x, loc.y );
      mText->setAttachmentMovingLocation((OdDbMText::AttachmentPoint)attPoint);
      loc = mText->location();
      ODA_TRACE2("Location  after X %6.4f  Y %6.4f\n", loc.x, loc.y );
      ODA_TRACE0("----------------------------------\n");
    }
#endif
  }
}
#endif

#if 0
#include "DbLeader.h"
#include "DbMText.h"
#include "DbBlockReference.h"
#include "DbFcf.h"
#include "DbArc.h"

// Test notification for Leader and ArcAlignedText
void extern_debugcommand(COdaMfcAppDoc* pDoc, CDwgView* pView)
{
  bool isChange = false;
  OdDbObjectId  objId = pView->getCurObjectId();
  OdDbObjectPtr obj = objId.openObject(OdDb::kForWrite);
  if ( obj->isKindOf(OdDbMText::desc()) )
  {
    OdDbMTextPtr mText = OdDbMText::cast(obj);
    if ( !mText.isNull() )
    {
      /*
      OdGePoint3d loc = mText->location();
      loc.x -= 0.5;
      loc.y -= 0.5;
      mText->setLocation(loc);
      */
      mText->setRotation(mText->rotation()+0.2);
      isChange = true;
    }
  } else if ( obj->isKindOf(OdDbFcf::desc()) )
  {
    OdDbFcfPtr mFcf = OdDbFcf::cast(obj);
    if ( !mFcf.isNull() )
    {
      OdGePoint3d loc = mFcf->location();
      loc.x -= 0.5;
      loc.y -= 0.5;
      mFcf->setLocation(loc);
      isChange = true;
    }
  } else if ( obj->isKindOf(OdDbBlockReference::desc()) )
  {
    OdDbBlockReferencePtr block = OdDbBlockReference::cast(obj);
    if ( !block.isNull() )
    {
      OdGePoint3d pos = block->position();
      pos.x += 0.5;
      pos.y += 0.5;
      block->setPosition(pos) ;
      isChange = true;
    }
  } else if ( obj->isKindOf(OdDbLeader::desc()) )
  {
    OdDbLeaderPtr mLeader = OdDbLeader::cast(obj);
    if ( !mLeader.isNull() )
    {
      mLeader->evaluateLeader();
      isChange = true;
    }
  } else if ( obj->isKindOf(OdDbArc::desc()) )
  {
    OdDbArcPtr mArc = OdDbArc::cast(obj);
    if ( !mArc.isNull() )
    {
      mArc->setRadius(mArc->radius()+0.2);
      isChange = true;
    }
  }
  if (isChange)
    pDoc->UpdateAllViews(pView);
}
#endif

#if 0
#include "XRefMan.h"

void extern_debugcommand(COdaMfcAppDoc* pDoc, CDwgView* pView)
{
  OdDbObjectId  objId = pView->getCurObjectId();
  OdDbObjectPtr obj = objId.openObject(OdDb::kForWrite);
  if ( obj->isKindOf(OdDbBlockTableRecord::desc()) )
  {
    OdDbBlockTableRecordPtr   pBlock = obj;
    if (pBlock->isFromExternalReference())
    {
      OdDbXRefMan::detach(pBlock, false);
      //OdDbXRefMan::unload(pBlock, false);

      pDoc->UpdateAllViews(pView);
    }
  }
  return;
}
#endif


#if 0
#include "XRefMan.h"

void extern_debugcommand(COdaMfcAppDoc* pDoc, CDwgView* pView)
{
  OdDbObjectId  objId = pView->getCurObjectId();
  OdDbObjectPtr obj = objId.openObject(OdDb::kForWrite);
  if ( obj->isKindOf(OdDbBlockTableRecord::desc()) )
  {
    OdDbBlockTableRecordPtr   pBlock = obj;
    if (pBlock->isFromExternalReference() && pBlock->isResolved())
    {
      OdDbXRefMan::bind(pBlock, true, false);

      pDoc->UpdateAllViews(pView);
    }
  }
  return;
}
#endif


#if 0
#include "DbLeader.h"
#include "DbMText.h"
#include "DbBlockReference.h"
#include "DbFcf.h"
#include "DbText.h"

static OdDbObjectId mAnnoID;

void extern_debugcommand(COdaMfcAppDoc* pDoc, CDwgView* pView)
{
  OdDbObjectId  objId = pView->getCurObjectId();
  OdDbObjectPtr obj = objId.openObject(OdDb::kForWrite);

  if ( obj->isKindOf(OdDbMText::desc()) ) // ||

    // obj->isKindOf(OdDbFcf::desc()) ||
    // obj->isKindOf(OdDbBlockReference::desc()) &&
    // mAnnoID.isNull() )
  {
    OdDbMTextPtr mText = OdDbMText::cast(obj);
    OdGePoint3dArray points;
    OdGePoint3d point;
    point = mText->location();
    point.x += 0.001;
    mText->setLocation(point);
    // mText->getBoundingPoints(points);
    // mAnnoID =  objId;
  } else if ( obj->isKindOf(OdDbLeader::desc()) ) // &&
    // !mAnnoID.isNull() )
  {
    OdDbLeaderPtr mLeader = OdDbLeader::cast(obj);
    /* if ( !mLeader.isNull() )
    {
      mLeader->evaluateLeader(); // attachAnnotation(mAnnoID);
    }
    mAnnoID.setNull(); */
    OdGePoint3d point;
    point = mLeader->vertexAt(0);
    point.x += 0.001;
    mLeader->setVertexAt(0, point);
  } else if ( obj->isKindOf(OdDbText::desc()) )
  {
    OdDbTextPtr mText = OdDbText::cast(obj);
    mText->adjustAlignment();
    // mText->setPosition(OdGePoint3d(17.62, 10.31, 0.0));
    // mText->setAlignmentPoint(OdGePoint3d(17.62, 10.11, 0.0));
    // mText->adjustAlignment(mText->database());
    // mText->setHorizontalMode(OdDb::kTextAlign); //kTextRight);
    // mText->setHorizontalMode(OdDb::kTextCenter); //kTextRight);
    OdGePoint3dArray points;
    mText->getBoundingPoints(points);
  }
}
#endif

//////////    dimension test  //////////////////////

#if 0
#include "DbDimStyleTable.h"
#include "DbDimStyleTableRecord.h"

void extern_debugcommand(COdaMfcAppDoc* pDoc, CDwgView* pView)
{
  OdDbObjectId    id;
  OdDbObjectPtr   pObject;

  HTREEITEM hItem = pView->m_ObjectTree.GetSelectedItem();
  if (hItem)
  {
    id = (OdDbStub*)(pView->m_ObjectTree.GetItemData(hItem));
    pObject = id.openObject();

    if ( pObject->isKindOf(OdDbDimension::desc()) )
    {
      OdDbDimensionPtr           pDim = pObject;

      ODA_TRACE1("%ls\n", OdDmUtil::arrowName(pDim->dimblk1()).c_str());


      OdDbDimStyleTableRecordPtr pRec =  OdDbDimStyleTableRecord::createObject();
      OdDbDimStyleTablePtr       pTable = pDoc->getDimStyleTableId().safeOpenObject(OdDb::kForWrite);

      pRec->setName(OD_T("testStyle02"));
      pDim->getDimstyleData(pRec);
      pTable->add(pRec);

    }
  }
  return;
}
#endif


#if 0
#include "DbFcf.h"

void extern_debugcommand(COdaMfcAppDoc* pDoc, CDwgView* pView)
{
  OdDbObjectId  objId = pView->getCurObjectId();
  OdDbObjectPtr obj = objId.openObject(OdDb::kForWrite);

  if ( obj->isKindOf(OdDbFcf::desc()) )
  {
    OdDbFcfPtr mFcf = OdDbFcf::cast(obj);
    OdGePoint3dArray array;

    mFcf->getBoundingPline(array);
    ODA_TRACE0("Bounding PLine\n");
    for ( int i = 0; i < array.size(); i++ )
    {
      ODA_TRACE2("%6.4f  %6.4f", array[i].x, array[i].y );
      ODA_TRACE1("  %6.4f\n", array[i].z );
    }
    array.clear();
    mFcf->getBoundingPoints(array);
    ODA_TRACE0("Bounding Points\n");
    ODA_TRACE2("top left     %6.4f  %6.4f ", array[0].x, array[0].y );
    ODA_TRACE1(" %6.4f\n", array[0].z );
    ODA_TRACE2("top right    %6.4f  %6.4f ", array[1].x, array[1].y );
    ODA_TRACE1(" %6.4f\n", array[1].z );
    ODA_TRACE2("bottom left  %6.4f  %6.4f ", array[2].x, array[2].y );
    ODA_TRACE1(" %6.4f\n", array[2].z );
    ODA_TRACE2("bottom right %6.4f  %6.4f ", array[3].x, array[3].y );
    ODA_TRACE1(" %6.4f\n", array[3].z );

  }

}
#endif

#if 0
#include "DbXrecord.h"
#define RB_LENGTH 100000

void extern_debugcommand(COdaMfcAppDoc* pDoc, CDwgView* /*pView*/)
{
  OdDbDatabase* pDb = pDoc->database();
  if (!pDb)
    return;

  // Create a new xrecord object.
  OdDbDictionaryPtr pMain = pDb->getNamedObjectsDictionaryId().safeOpenObject(OdDb::kForWrite);
  OdDbDictionaryPtr pOdtDic = OdDbDictionary::createObject();
  OdDbObjectId dicId = pMain->setAt(OD_T("TEIGHA_OBJECTS"), pOdtDic);
  OdDbXrecordPtr pXRec = OdDbXrecord::createObject();
  OdDbObjectId xrId = pOdtDic->setAt(OD_T("PROPERTIES_1"), pXRec);

  // Create Rb chain.
  OdResBufPtr pStartRb = OdResBuf::newRb(1000, OD_T("Sample XRecord Data"));
  OdResBufPtr pRb = pStartRb;
  for (int i=0; i<RB_LENGTH; ++i)
  {
    OdResBufPtr pRb1 = OdResBuf::newRb(OdResBuf::kDxfText, OD_T("TEXT"));
    pRb = pRb->setNext(pRb1);
  }

  // Set & Append
  pXRec->setFromRbChain(pStartRb);
  pXRec->appendRbChain(pStartRb);

  return;
}
#endif

#if 0
#include "DbRasterImage.h"
#include "DbRasterImageDef.h"
#include "DbRasterVariables.h"

/*  create raster image and add it into the database  */
void extern_debugcommand(COdaMfcAppDoc* pDoc, CDwgView* )
{
  double ScaleFactor = 1.0;
  double RotationAngle = 0.0;
  OdUInt32  nWidth, nHeight;
  OdString  ImageName;

  // get image to add
  ImageName = OD_T("D:\\Teigha\\Test\\test.jpg");

  // open model space for write
  OdDbBlockTableRecordPtr  pRecord = pDoc->getModelSpaceId().safeOpenObject(OdDb::kForWrite);

  // Ensure that this drawing has an image dictionary
  OdDbObjectId imageDictId = OdDbRasterImageDef::createImageDictionary(pRecord->database());

  // Open the image dictionary.
  OdDbDictionaryPtr pImageDict = imageDictId.safeOpenObject(OdDb::kForWrite);

  // check the dictionary for specified image definition name
  int ind1 = ImageName.reverseFind('\\');
  int ind2 = ImageName.reverseFind('.');
  OdString key;
  if ( ind1 != -1 )
  {
    if ( ind2 != -1 )
      key = ImageName.mid(ind1+1, ind2 - ind1 - 1);
    else
      key = ImageName.mid(ind1+1);
  }
  else
  {
    if ( ind2 != -1 )
      key = ImageName.mid(0, ind2);
    else
      key = ImageName;
  }

  nWidth =  1;
  nHeight = 1;
  // try to load image and detect it size
  OdGiRasterImagePtr rast = odSystemServices()->loadRasterImage(ImageName);
  if ( !rast.isNull() )
    rast->size(nWidth, nHeight);

  OdDbObjectId imageDefId = pImageDict->getAt(key);
  if (imageDefId.isNull())
  {
      // an image definition was not find, so create new one
      OdDbRasterImageDefPtr pImageDef = OdDbRasterImageDef::createObject();
      imageDefId = pImageDict->setAt(key, pImageDef);

      // Set remaining imagedef attributes.
      pImageDef->setSourceFileName(ImageName);
      //pImageDef->setImage(OdGiRasterImageDesc::createObject(nWidth, nHeight, OdGiRasterImage::kInch));
      pImageDef->setImage(rast);

      // check raster variables
      OdDbRasterVariablesPtr pRVars = OdDbRasterVariables::openRasterVariables(pRecord->database(), OdDb::kForWrite);
      if (OdGiRasterImage::kNone == pRVars->userScale())
      {
        pRVars->setUserScale(OdGiRasterImage::kMillimeter);
        pRVars->setImageFrame(OdDbRasterVariables::kImageFrameAbove);
        pRVars->setImageQuality(OdDbRasterVariables::kImageQualityHigh);
      }
  }

  // Add image entity, which references the above imagedef.  An imagedef
  // can be used by multiple image entities.
  OdDbRasterImagePtr pdbRaster = OdDbRasterImage::createObject();

  // Add image entity to block.
  pRecord->appendOdDbEntity(pdbRaster);

  // Set remaining image attributes.
  pdbRaster->setImageDefId(imageDefId);

  OdGeVector3d nU(OdGeVector3d::kXAxis), nV(OdGeVector3d::kYAxis);
  nU.rotateBy(RotationAngle, OdGeVector3d::kZAxis);
  nU *= ScaleFactor;

  nV.y = nV.y * nHeight / nWidth;     //  take into account aspect ratio
  nV.rotateBy(RotationAngle, OdGeVector3d::kZAxis);
  nV *= ScaleFactor;

  OdGePoint3d  pt(10.0, 10.0, 0);
  pdbRaster->setOrientation(pt, nU, nV);

  pdbRaster->setDisplayOpt(OdDbRasterImage::kShow, true);
  pdbRaster->setDisplayOpt(OdDbRasterImage::kShowUnAligned, true);
  pdbRaster->setBrightness(OdUInt8(50));
  pdbRaster->setContrast(OdUInt8(50));

  OdGePoint2dArray pts;

  // as a second point we have to use image size
  pts.push_back(OdGePoint2d(2, 2));
  pts.push_back(OdGePoint2d(nWidth/2, 2));
  pts.push_back(OdGePoint2d(nWidth/2, nHeight/2));
  pts.push_back(OdGePoint2d(2, nHeight/2));
  pts.push_back(OdGePoint2d(2, 2));
  pdbRaster->setClipBoundary(pts);

  pdbRaster->setDisplayOpt(OdDbRasterImage::kClip, true);
  pdbRaster->setClipped(true);
  return;
}
#endif


#if 0
#include "DbTable.h"
void tableOverrides(OdDbTablePtr& mTable)
{
  OdArray<OdUInt32> over;
  mTable->tableStyleOverrides(over);
  ODA_TRACE1("Size overrides %d\n", over.size() );
  for ( OdUInt32 k = 0; k < over.size(); k++)
  {
  	ODA_TRACE1("\n %d!", over[k]);
  }
	for ( OdUInt32 i = 0; i < mTable->numRows(); i++ )
	{
		for ( OdUInt32 j = 0; j < mTable->numColumns(); j++ )
		{
			OdArray<OdUInt32> overridesCell;
			mTable->cellStyleOverrides(i, j, overridesCell);
			ODA_TRACE2("\nSize overrides Cell(%d, %d) - ", i, j );
			ODA_TRACE1(" %d!", overridesCell.length());
			for ( OdUInt32 i = 0; i < overridesCell.length(); i++ )
      {
				ODA_TRACE1("\n %d!", overridesCell[i]);
			}
		}
	}
}

void testMergedCell(OdDbTablePtr& mTable)
{
	for ( OdUInt32 i = 0; i < mTable->numRows(); i++ )
	{
		for ( OdUInt32 j = 0; j < mTable->numColumns(); j++ )
		{
			ODA_TRACE2("Cell(%d, %d) -  \n", i, j );
      OdUInt32 minRow, maxRow, minCol, maxCol;
			bool b = mTable->isMergedCell(i, j, &minRow, &maxRow, &minCol, &maxCol );
      if ( b )
      {
				ODA_TRACE0("Merged!\n");
  			ODA_TRACE2("Min - (%d, %d)", minRow, minCol );
  			ODA_TRACE2("Max - (%d, %d)\n", maxRow, maxCol );
        // mTable->unmergeCells(minRow, maxRow, minCol, maxCol);
      } else
      {
				ODA_TRACE0("No merged!\n");
      }
		}
	}

  mTable->unmergeCells(0, 2, 0, 2);
  mTable->unmergeCells(3, 5, 3, 5);

  mTable->mergeCells(0, 2, 3, 5);
  mTable->mergeCells(3, 5, 0, 2);
}

void testGridProperty(OdDbTablePtr& mTable)
{
	for ( OdUInt32 i = 0; i < mTable->numRows(); i++ )
	{
		for ( OdUInt32 j = 0; j < mTable->numColumns(); j++ )
		{
			ODA_TRACE2("Cell(%d, %d)\n", i, j );
			ODA_TRACE2("kTopMask    - %d\nkRightMask  - %d\n", mTable->gridColor(i,j, OdDb::kTopMask).colorIndex(),
					mTable->gridColor(i,j, OdDb::kRightMask).colorIndex() );
			ODA_TRACE2("kBottomMask - %d\nkLeftMask   - %d\n", mTable->gridColor(i,j, OdDb::kBottomMask).colorIndex(),
					mTable->gridColor(i,j, OdDb::kLeftMask).colorIndex() );
		}
	}

}

void extern_debugcommand(COdaMfcAppDoc* pDoc, CDwgView* pView)
{
  OdDbObjectId  objId = pView->getCurObjectId();
  OdDbObjectPtr obj = objId.openObject(OdDb::kForWrite);

  if ( obj->isKindOf(OdDbTable::desc()) )
  {
    ODA_TRACE0("\n================\nTable:\n");

    OdDbTablePtr mTable = OdDbTable::cast(obj);
    // testMergedCell(mTable);
    tableOverrides(mTable);
    // mTable->recomputeTableBlock();
    // testGridProperty(mTable);
    return;

    // mTable->recomputeTableBlock();
    double w = mTable->width();
    double h = mTable->height();

    ODA_TRACE2(" width - %f, height - %f\n", w, h);

    double hCm = mTable->horzCellMargin();
    double vCm = mTable->vertCellMargin();

    ODA_TRACE2(" horzCellMargin - %f, vertCellMargin - %f\n", hCm, vCm);

    OdUInt32 rows = mTable->numRows();
    OdUInt32 cols = mTable->numColumns();

    ODA_TRACE2(" rows - %d, cols - %d\n", rows, cols);

    // ==== Table Overrides ====
    OdDb::FlowDirection fDir = mTable->flowDirection();

    ODA_TRACE1(" FlowDirection %ls\n", fDir == OdDb::kTtoB ? OD_T("kTtoB") : OD_T("kBtoT"));

    bool Tsup = mTable->isTitleSuppressed();
    bool Hsup = mTable->isHeaderSuppressed();

    ODA_TRACE2(" %ls, %ls\n", Tsup ? OD_T("Title is suppressed ") : OD_T("Title "),
                          Hsup ? OD_T("Header is suppressed ") : OD_T("Header "));

    // by rowType
    // alignment()
    // isBackgroundColorNone
    // backgroundColor
    // contentColor
    // textHeight
    // gridLineWeight
    // gridColor
    // gridVisibility

    for ( OdUInt32 i = 0; i < rows; i++)
    {
      double rH = mTable->rowHeight(i);
      OdDb::RowType rType = mTable->rowType(i);
      ODA_TRACE2("Row %d  type %x", i, rType);
      ODA_TRACE1(" heigth %f\n", rH);
    }
    for (  OdUInt32 j = 0; j < cols; j++ )
    {
      double cW = mTable->columnWidth(j);
      ODA_TRACE1("Col %d  ", j);
      ODA_TRACE1(" width %f\n", cW);
    }
    tableOverrides(mTable);
    ODA_TRACE0("\n========= End Table ============\n");
  }
}
#endif

#if 0
//
// Tests reading/writing of AveRender options
//
#include "AveRender.h"

void extern_debugcommand(COdaMfcAppDoc* pDoc, CDwgView* pView)
{
  OdDbDatabase* pDb = pDoc->database();

  OdAveRenderOption renderOpt;
  renderOpt.getRenderOption(pDb);
  renderOpt.writeRenderOption(pDb);
}
#endif

//////////////////////////////////////////////////////////////////////
//
// copyFrom/clone
//
//////////////////////////////////////////////////////////////////////
//
//  copyFrom test for Plot Settings
//
#include "DbPlotSettings.h"
#include "DbPlotSettingsValidator.h"
#include "DbLayout.h"

void _copyFromPlotSettings_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);

  //  create new plot Settings
  OdDbPlotSettingsPtr pNewPlot = OdDbPlotSettings::createObject();
  OdString sPlotName(L"the Test settings");
  pNewPlot->setPlotSettingsName(sPlotName);

  // some plot setting are set through OdDbPlotSettingsValidator
  OdDbPlotSettingsValidator* pPlotValidator = pDbCmdCtx->database()->appServices()->plotSettingsValidator();
  pPlotValidator->setPlotCfgName(pNewPlot, L"DWF6 ePlot.pc3", sPlotName);
  pPlotValidator->setPlotCentered(pNewPlot, true);
  pNewPlot->setModelType(false);
  pNewPlot->setShadePlot(OdDbPlotSettings::kWireframe);
  pNewPlot->setDrawViewportsFirst(true);
  pNewPlot->setShadePlotResLevel(OdDbPlotSettings::kPresentation);

  pNewPlot->addToPlotSettingsDict(pDbCmdCtx->database());

  pNewPlot->setPaperImageOrigin(OdGePoint2d(3.0, 4.0));

  OdResBufPtr pRbuf = OdResBuf::newRb(OdResBuf::kDxfRegAppName,L"ACAD_PSEXT");
  pRbuf->setNext(OdResBuf::newRb(1000,L"None"));
  pRbuf->last()->setNext(OdResBuf::newRb(1000,L"None"));
  pRbuf->last()->setNext(OdResBuf::newRb(1000,L"Not applicable"));
  pRbuf->last()->setNext(OdResBuf::newRb(1000,L"The layout will  not be plotted"));
  pRbuf->last()->setNext(OdResBuf::newRb(1070,OdInt16(0)));
  pNewPlot->setXData(pRbuf);

  // copy settings from plotconfig to paperspace layout
  OdDbBlockTableRecordPtr  pPaper = pDbCmdCtx->database()->getPaperSpaceId().safeOpenObject();
  OdDbObjectId idPaperLayout =  pPaper->getLayoutId();

  OdDbLayoutPtr pLayout =  idPaperLayout.safeOpenObject(OdDb::kForWrite);

  pLayout->copyFrom(pNewPlot);
}

//
// Tests clone() for proxy entity
//
#include "DbProxyEntity.h"

void _proxyCloneTest_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbObjectIdArray objectIdsArray = OdDbSelectionSet::cast(pDbCmdCtx->dbUserIO()->select().get())->objectIdArray();
  if (objectIdsArray.isEmpty())
    return;
  OdDbObjectPtr pObject = objectIdsArray[0].openObject();
  try
  {
    OdDbProxyEntityPtr pProxy = OdDbProxyEntity::cast(pObject);
    if (!pProxy.isNull())
    {
      OdDbDatabasePtr pTmpDatabase = pDbCmdCtx->database()->appServices()->createDatabase();

      OdDbBlockTableRecordPtr  pMs = pTmpDatabase->getModelSpaceId().safeOpenObject(OdDb::kForWrite);
      OdDbProxyEntityPtr pProxyClone;
      OdDbObjectId objId;

      // Put clone of proxy object in DB
      //pProxyClone = pProxy->clone();
      //objId = pMs->appendOdDbEntity(pProxyClone);

      // Fill NonDBRO proxy and put in DB
      pProxyClone = OdDbProxyEntity::createObject();
      pProxyClone->copyFrom(pProxy);
      objId = pMs->appendOdDbEntity(pProxyClone);

      // Adding emplty proxy to DB throws exception
      //pProxyClone = OdDbProxyEntity::createObject();
      //objId = pMs->appendOdDbEntity(pProxyClone);

      pProxyClone = 0;
      pMs = 0;

      pTmpDatabase->writeFile (OD_T("c:\\DDTest\\TestProxyClone.dwg"), OdDb::kDwg, OdDb::kDHL_CURRENT, true, 0);
    }
  }
  catch (OdError &e)
  {
    OdString msg = e.description();
    pDbCmdCtx->database()->appServices()->warning(msg);
  }
}

//////////////////////////////////////////////////////////////////////
//
// DEEPCLONE/WBLOCK/INSERT
//
//////////////////////////////////////////////////////////////////////

//
// Tests deepCloneObjects() for selected item (entity or BlockRecord)
//
#include "DbBlockTable.h"
#include "DbBlockTableRecord.h"
#include "DbIdMapping.h"
#include "DbEntity.h"
#include "DbHostAppServices.h"

void _deepclone_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();

  OdDbObjectIdArray objectIdsArray = OdDbSelectionSet::cast(pIO->select(L"", OdEd::kSelAllowObjects|OdEd::kSelAllowInactSpaces).get())->objectIdArray();

  if (objectIdsArray.size() == 0)
    return;

  pDb->disableUndoRecording(false);
  pDb->startUndoRecord();

  OdDbIdMappingPtr pMapping = OdDbIdMapping::createObject();
  pMapping->setDestDb(pDb);

  if (!OdDbEntity::cast(objectIdsArray[0].openObject()).isNull())
  {
    pDb->deepCloneObjects(objectIdsArray, pDb->getModelSpaceId(), *pMapping);
  }
  else if (!OdDbBlockTableRecord::cast(objectIdsArray[0].openObject()).isNull())
  {
    pDb->deepCloneObjects(objectIdsArray, pDb->getBlockTableId(), *pMapping);
  }
}

//
// Tests wblockCloneObjects() for selected item (entity or BlockRecord)
//

#include "DbIdMapping.h"

void _wblockclone_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();

  OdDbSelectionSetIteratorPtr pIter = pIO->select(L"",OdEd::kSelAllowObjects|OdEd::kSelAllowInactSpaces)->newIterator();
  if (pIter->done())
    return;
  OdDbObjectIdArray objects;
  for (;!pIter->done();pIter->next())
    objects.push_back(pIter->objectId());
  OdStreamBufPtr pFile = ::odSystemServices()->createFile(
      pIO->getFilePath(OdString::kEmpty, OdEd::kGfpForSave, OdString::kEmpty, OD_T("dwg")),
      Oda::kFileWrite,
      Oda::kShareDenyReadWrite,
      Oda::kCreateAlways);

  OdDbDatabasePtr pNewDb = pDb->appServices()->createDatabase(true, pDb->getMEASUREMENT());

  OdDbIdMappingPtr pMap = OdDbIdMapping::createObject();
  pMap->setDestDb(pNewDb);

  if (!OdDbEntity::cast(objects[0].openObject()).isNull())
  {
    pDb->wblockCloneObjects(objects, pNewDb->getModelSpaceId(), *pMap, OdDb::kDrcReplace);
  }
  else if (!OdDbLayerTableRecord::cast(objects[0].openObject()).isNull())
  {
    pDb->wblockCloneObjects(objects, pNewDb->getLayerTableId(), *pMap, OdDb::kDrcIgnore);
  }
  else if (!OdDbBlockTableRecord::cast(objects[0].openObject()).isNull())
  {
    pDb->wblockCloneObjects(objects, pNewDb->getBlockTableId(), *pMap, OdDb::kDrcReplace);
  }

  pNewDb->writeFile(pFile, OdDb::kDwg, OdDb::kDHL_CURRENT);
}

//
// Tests wblockCloneObjects() for entities in a selected record
//
#include "DbBlockTable.h"

void _wblockCloneRecord_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();

  OdDbSelectionSetIteratorPtr pSsIter = pIO->select()->newIterator();
  if (pSsIter->done())
    return;
  OdDbObjectId id = pSsIter->objectId();

  OdStreamBufPtr pFile = ::odSystemServices()->createFile(
    pIO->getFilePath(OdString::kEmpty, OdEd::kGfpForSave, OdString::kEmpty, OD_T("dwg")), Oda::kFileWrite
    );

  OdDbDatabasePtr pNewDb = pDb->appServices()->createDatabase(true, pDb->getMEASUREMENT());

  OdDbIdMappingPtr pMap = OdDbIdMapping::createObject();
  pMap->setDestDb(pNewDb);
  OdDbObjectIdArray ids;

  OdDbBlockTableRecordPtr pBlock = id.openObject();
  OdDbObjectIteratorPtr pIter = pBlock->newIterator();
  while (!pIter->done())
  {
    ids.push_back(pIter->objectId());
    pIter->step();
  }
  pBlock.release();

  OdDbBlockTableRecordPtr pRec =  OdDbBlockTableRecord::createObject();
  OdDbBlockTablePtr pTable = pNewDb->getBlockTableId().safeOpenObject(OdDb::kForWrite);

  OdString name = OD_T("ClonedBlock_");
  name += pDb->handseed().ascii();
  pRec->setName(name);
  OdDbObjectId recId = pTable->add(pRec);
  pRec.release();
  pTable.release();

  OdDbIdMappingPtr pMapping = OdDbIdMapping::createObject();
  pMapping->setDestDb(pNewDb);
  pDb->wblockCloneObjects(ids, recId, *pMapping.get(), OdDb::kDrcIgnore);
  //pDb->wblockCloneObjects(ids, recId, *pMapping.get(), OdDb::kDrcMangleName);

  pNewDb->writeFile(pFile, OdDb::kDwg, OdDb::kDHL_CURRENT);
}

//
// Tests deepCloneObjects() for entities in a selected record
//
#include "DbBlockTable.h"
#include "DbIdMapping.h"

void _deepclone1_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();

  OdDbSelectionSetIteratorPtr pSsIter = pIO->select()->newIterator();
  if (pSsIter->done())
    return;
  OdDbObjectId  objId = pSsIter->objectId();
  pDb->disableUndoRecording(false);
  pDb->startUndoRecord();

  try
  {
    OdDbBlockTableRecordPtr pBlock = objId.openObject();
    OdDbObjectIdArray objectIdsArray;
    OdDbObjectIteratorPtr pIter = pBlock->newIterator();
    while (!pIter->done())
    {
      objectIdsArray.push_back(pIter->objectId());
      pIter->step();
    }
    pBlock.release();

    OdDbIdMappingPtr pMapping = OdDbIdMapping::createObject();
    pMapping->setDestDb(pDb);

    pBlock = OdDbBlockTableRecord::createObject();
    OdString blockName;
    blockName.format(OD_T("ClonedBlock_%ls"), pDb->handseed().ascii().c_str());
    pBlock->setName(blockName);
    OdDbBlockTablePtr pBlocks = pDb->getBlockTableId().safeOpenObject(OdDb::kForWrite);
    OdDbObjectId blockId = pBlocks->add(pBlock);

    pDb->deepCloneObjects(objectIdsArray, blockId, *pMapping);
  }
  catch (OdError &e)
  {
    OdString msg = e.description();
    pDb->appServices()->warning(msg);
  }
}

#if 0
//
// Tests wblockCloneObjects() for entities in a selected record
//
#include "DbBlockTable.h"
#include "DbIdMapping.h"
#include "OdFileBuf.h"

void extern_debugcommand(COdaMfcAppDoc* pDoc, CDwgView* pView)
{
  OdDbDatabase* pDb = pDoc->database();
  OdDbObjectId  id = pView->getCurObjectId();
  if (id.isNull())
    return;

  OdDbDatabasePtr pNewDb = pDoc->appServices()->createDatabase(true, pDoc->getMEASUREMENT());

  try
  {
    OdDbBlockTableRecordPtr pBlock = id.openObject();
    OdDbObjectIdArray ids;
    OdDbObjectIteratorPtr pIter = pBlock->newIterator();
    while (!pIter->done())
    {
      ids.push_back(pIter->objectId());
      pIter->step();
    }
    pBlock.release();

    OdDbBlockTableRecordPtr pRec =  OdDbBlockTableRecord::createObject();
    OdDbBlockTablePtr pTable = pNewDb->getBlockTableId().safeOpenObject(OdDb::kForWrite);

    OdString name = OD_T("ClonedBlock_");
    name += pDb->handseed().ascii();
    pRec->setName(name);
    OdDbObjectId recId = pTable->add(pRec);
    pRec.release();
    pTable.release();

    OdDbIdMappingPtr pMapping = OdDbIdMapping::createObject();
    pMapping->setDestDb(pNewDb);
    pDb->wblockCloneObjects(ids, recId, *pMapping.get(), OdDb::kDrcIgnore);

    OdWrFileBuf fileBuffer (OD_T("c:\\DDTest\\wblockCloneObjects_1_13.dwg"));
    pNewDb->writeFile(&fileBuffer, OdDb::kDwg, OdDb::kDHL_CURRENT);
  }
  catch (OdError &e)
  {
    OdString msg = e.description();
    pDoc->appServices()->warning(msg);
  }
}
#endif

#if 0
//
// Tests wblock of all layouts to another database thru wblockCloneObjects()
//
#include "DbBlockTable.h"
#include "DbLayout.h"
#include "OdFileBuf.h"
#include "DbIdMapping.h"

void extern_debugcommand(COdaMfcAppDoc* pDoc, CDwgView* pView)
{

  try
  {
    OdDbDatabasePtr pSrcDb = pDoc;

    // Get layouts BTRs
    OdDbObjectIdArray objects;
    OdDbDictionaryPtr pLayoutDict = pSrcDb->getLayoutDictionaryId().safeOpenObject();
    OdDbDictionaryIteratorPtr pIter = pLayoutDict->newIterator();
    while (!pIter->done())
    {
      OdDbObjectId layoutId = pIter->objectId();
      OdDbLayoutPtr pLayout = layoutId.safeOpenObject();
      OdDbObjectId recordId = pLayout->getBlockTableRecordId();
      if (!recordId.isNull())
      {
        objects.push_back(recordId);
      }
      pIter->next();
    }

    // Wblock
    OdDbDatabasePtr pNewDb = pSrcDb->appServices()->createDatabase(true, pSrcDb->getMEASUREMENT());
    OdDbIdMappingPtr pMap = OdDbIdMapping::createObject();
    pMap->setDestDb(pNewDb);
    pSrcDb->wblockCloneObjects(objects, pNewDb->getBlockTableId(), *pMap, OdDb::kDrcReplace);

    // Save to file
    OdWrFileBuf fileBuffer(OD_T("c:\\DDTest\\wblockClonePaperSpaces_1_13.dwg"));
    pNewDb->writeFile(&fileBuffer, OdDb::kDwg, OdDb::kDHL_CURRENT);
  }
  catch(const OdError& e)
  {
    theApp.reportError(OD_T("Wblock..."), e);
  }
}
#endif

//
// wblock() for selected item (entity, BlockRecord, database)
//
void _wblock_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbUserIO* pIO = OdDbUserIO::cast(pDbCmdCtx->userIO());
  if (!pIO)
    return;

  OdUInt32 nKeyword = pIO->getKeyword(OD_T("Enter type of record [Block/Database/Entities]:"), OD_T("Block Database Entities"));
  OdDbDatabasePtr pDbClone;
  switch (nKeyword)
  {
  case 0: //Block
    {
      OdDbSelectionSetIteratorPtr pIter = pIO->select(L"", OdEd::kSelAllowObjects)->newIterator();
      if (!pIter->done())
      {
        OdDbObjectId objId = pIter->objectId();
        if (!OdDbBlockTableRecord::cast(objId.openObject()).isNull())
        {
          pDbClone = pDb->wblock(objId);
        }
      }
    }
    break;
  case 1: //Database
    pDbClone = pDb->wblock();
    break;
  case 2: //Entities
    {
      OdDbObjectIdArray ids = OdDbSelectionSet::cast(pIO->select().get())->objectIdArray();
      if (ids.size())
      {
        OdGePoint3d pt = OdGePoint3d::kOrigin;
        pIO->getPoint(OD_T("Enter base point <Origin>:"), 0, &pt);
        pDbClone = pDb->wblock(ids, pt);
      }
    }
    break;
  }

  if (!pDbClone.isNull())
  {
    OdStreamBufPtr pFile = ::odSystemServices()->createFile(
      pIO->getFilePath(OdString::kEmpty, OdEd::kGfpForSave, OdString::kEmpty, OD_T("dwg")),
      Oda::kFileWrite,
      Oda::kShareDenyReadWrite,
      Oda::kCreateAlways);
    pDbClone->writeFile(pFile, OdDb::kDwg, OdDb::kDHL_CURRENT);
  }
}

#if 0
//
// Tests wblock() for blocks in database
//
#include "DbBlockTable.h"
#include "OdFileBuf.h"

void extern_debugcommand(COdaMfcAppDoc* pDoc, CDwgView* pView)
{
  OdDbDatabase* pDb = pDoc->database();

  OdDbBlockTablePtr pBlocks =  pDb->getBlockTableId().safeOpenObject(OdDb::kForRead);
  OdDbSymbolTableIteratorPtr pBlkIter = pBlocks->newIterator();
  for (pBlkIter->start(); ! pBlkIter->done(); pBlkIter->step())
  {
    OdDbBlockTableRecordPtr pBlock = pBlkIter->getRecordId().safeOpenObject(OdDb::kForWrite);
    if ( !pBlock.isNull()
      && !pBlock->isLayout()
      && !pBlock->isFromExternalReference())
    {
      OdString fname;
      fname.format(OD_T("c:\\DDTest\\TestWblock_%ls.dwg"), odDbGetHandleName(pBlock->objectId().getHandle()).c_str());
      OdWrFileBuf fbuff(fname.c_str());
      OdDbDatabasePtr pTmpDb = pDb->wblock(pBlock->objectId());
      pTmpDb->writeFile(&fbuff, OdDb::kDwg, pDb->originalFileVersion(), true, 0);
    }
  }
}
#endif

//
// Tests insert database
//
void _testDbInsert_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbUserIO* pIO = OdDbUserIO::cast(pDbCmdCtx->userIO());
  if (!pIO)
    return;

  OdString fname = pIO->getFilePath(OD_T("Enter file name to insert:"),
                                       OdEd::kGfpForOpen,
                                       OD_T("Select drawing to insert"),
                                       OD_T("dwg"),
                                       OdString::kEmpty,
                                       OD_T("TD drawing (*.dwg)|*.dwg||"));
  OdDbDatabasePtr pSrcDb = pDb->appServices()->readFile(fname);
  if (pSrcDb.get())
  {
    pDb->insert(OD_T("InsertedBlock1"), pSrcDb);
    //pDb->insert(OD_T("*Model_Space"), OD_T("InsertedBlock2"), pSrcDb);
    //pDb->insert(OdGeScale3d(0.75, 1.25, 1.0), pSrcDb);
    //pDb->insert(OdGeMatrix3d::kIdentity, pSrcDb);
  }
}

//////////////////////////////////////////////////////////////////////
//
// EXPLODE
//
//////////////////////////////////////////////////////////////////////

//
// Tests wblockClone in explodeToBlock() for all BlockReferences in ModelSpace
//
#include "DbBlockReference.h"

void _ExplodeToBlockTest_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pSrcDb = pDbCmdCtx->database();
  OdDbDatabasePtr pNewDb = pSrcDb->appServices()->createDatabase(true, pSrcDb->getMEASUREMENT());
  {
    OdDbBlockTableRecordPtr pNewDbMs = pNewDb->getModelSpaceId().safeOpenObject(OdDb::kForWrite);
    OdDbBlockTableRecordPtr pMs = pSrcDb->getModelSpaceId().safeOpenObject();
    OdDbObjectIteratorPtr pMsIter = pMs->newIterator();
    OdDbObjectIdArray resIds;
    resIds.append(OdDbObjectId::kNull); // test that explodeToBlock does not clear input array
    while (!pMsIter->done())
    {
      OdDbObjectId entId = pMsIter->objectId();
      OdDbEntityPtr pMsEnt = OdDbEntity::cast(entId.openObject(OdDb::kForWrite));
      if (pMsEnt->isKindOf(OdDbBlockReference::desc()))
      {
        pMsEnt->explodeToBlock(pNewDbMs, &resIds);
      }
      pMsIter->step();
    }
    ODA_ASSERT(resIds[0] == OdDbObjectId::kNull);
  }
  // Save to file
  pNewDb->writeFile("c:\\explodeBlockReferences_3_01.dwg", OdDb::kDwg, OdDb::kDHL_CURRENT);
}


//////////////////////////////////////////////////////////////////////
//
// XREF
//
//////////////////////////////////////////////////////////////////////
#include "XRefMan.h"

static OdDbBlockTableRecordPtr selectBlock(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  class BlockTracker : public OdStaticRxObject<OdEdSSetTracker>
  {
    int addDrawables(OdGsView* ) { return 0; }
    void removeDrawables(OdGsView* ) { }
    virtual bool append(const OdDbObjectId& entId, const OdDbSelectionMethod* )
    {
      OdDbObjectPtr obj = entId.openObject();
      return !obj.isNull() && (obj->isKindOf(OdDbBlockTableRecord::desc()) || obj->isKindOf(OdDbBlockReference::desc()));
    }
    virtual bool remove(const OdDbObjectId&, const OdDbSelectionMethod*)
    {
      return true;
    }
  }
  tr;
  OdDbSelectionSetIteratorPtr pIter = pDbCmdCtx->dbUserIO()->select(L"Select a block <done>:", 
    OdEd::kSelSingleEntity | OdEd::kSelAllowObjects | OdEd::kSelAllowInactSpaces, 0, OdString::kEmpty, &tr)->newIterator();
  for (; !pIter->done(); pIter->next())
  {
    OdDbObjectPtr pObj = pIter->objectId().safeOpenObject();
    if (pObj->isKindOf(OdDbBlockTableRecord::desc()))
    {
      OdDbBlockTableRecordPtr pBlock = pObj;
      pBlock->upgradeOpen();
      return pBlock;
    }
    else if (pObj->isKindOf(OdDbBlockReference::desc()))
    {
      OdDbBlockTableRecordPtr pBlock = OdDbBlockReferencePtr(pObj)->blockTableRecord().safeOpenObject(OdDb::kForWrite);
      return pBlock;
    }
    else
    {
      ODA_FAIL_ONCE(); // filter failed?
      continue;
    }
  }
  return OdDbBlockTableRecordPtr();
}

void _XrefBind_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();
  OdString msg;
  
  OdDbBlockTableRecordPtr pBlock = selectBlock(pCmdCtx);
  if (pBlock.isNull())
    return;

  if (!pBlock->isFromExternalReference())
  {
    msg.format(L"Selected block \"%ls\" is not from external reference.", pBlock->getName().c_str());
    pIO->putString(msg);
    return;
  }

  if (!pBlock->isResolved())
  {
    msg.format(L"Selected block \"%ls\" is not resolved.", pBlock->getName().c_str());
    pIO->putString(L"Selected block is not resolved.");
    return;
  }

  msg.format(L"Select BindType for \"%ls\" block (\"Bind\" or \"Insert\" ) or \"Cancel\" operation? <Bind>:", pBlock->getName().c_str());
  int bindType = pIO->getKeyword(msg, L"Bind Insert Cancel", 0);
  if (bindType == 2)
  {
    msg.format(L"\"%ls\" block is skipped.", pBlock->getName().c_str());
    pIO->putString(msg);
    return;
  }

  msg.format(L"Used \"%ls\" for \"%ls\" block.", bindType == 0 ? L"Bind" : L"Insert", pBlock->getName().c_str());
  pIO->putString(msg);

  OdDbXRefMan::bind(pBlock, bindType != 0);
}

void _XrefDetach_func(OdEdCommandContext* pCmdCtx)
{
  OdDbBlockTableRecordPtr pBlock = selectBlock(pCmdCtx);
  if (pBlock.isNull())
    return;
  OdResult res = OdDbXRefMan::detach(pBlock);
  if (res != eOk)
    throw OdError(res);
}

void _testRedirectedId_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();

  OdDbSelectionSetIteratorPtr pIter = pIO->select()->newIterator();
  if (pIter->done())
    return;
  OdDbObjectId objId = pIter->objectId();
  if (objId.isNull())
    return;

  OdDbEntityPtr pEnt = OdDbEntity::cast(objId.openObject());
  if (pEnt.get())
  {
    objId = pEnt->layerId();
  }

  OdDbDatabase* pDb1 =  objId.database();
  OdDbDatabase* pDb2 =  objId.originalDatabase();
  OdDbHandle h1 = objId.getHandle();
  OdDbHandle h2 = objId.getNonForwardedHandle();

  ODA_TRACE2("\n OdDbObjectId::database()              : <% 8X> , %ls", pDb1, pDb1->getFilename().c_str());
  ODA_TRACE2("\n OdDbObjectId::originalDatabase()      : <% 8X> , %ls", pDb2, pDb2->getFilename().c_str());
  ODA_TRACE1("\n OdDbObjectId::getHandle()             : <% 8X>", (OdUInt64)h1);
  ODA_TRACE1("\n OdDbObjectId::getNonForwardedHandle() : <% 8X>", (OdUInt64)h2);

  ODA_TRACE0("\n>>>Apply OdDbObjectId::convertToRedirectedId()");
  objId.convertToRedirectedId();

  pDb1 =  objId.database();
  pDb2 =  objId.originalDatabase();
  h1 = objId.getHandle();
  h2 = objId.getNonForwardedHandle();

  ODA_TRACE2("\n OdDbObjectId::database()              : <% 8X> , %ls", pDb1, pDb1->getFilename().c_str());
  ODA_TRACE2("\n OdDbObjectId::originalDatabase()      : <% 8X> , %ls", pDb2, pDb2->getFilename().c_str());
  ODA_TRACE1("\n OdDbObjectId::getHandle()             : <% 8X>", (OdUInt64)h1);
  ODA_TRACE1("\n OdDbObjectId::getNonForwardedHandle() : <% 8X>", (OdUInt64)h2);
}


#include "DbHatch.h"
#include "DbViewport.h"
#include "DbFcf.h"
// This example has some limitation :
// - working in model space;
// - active viewport direction is used for hatch normal.

void _ExCreateHatch_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();

  OdDbSelectionSetIteratorPtr pIter = pDbCmdCtx->dbUserIO()->select(L"Select entity to hatch", OdEd::kSelAllowSubents)->newIterator();
  OdDbBlockTableRecordPtr pBTR = pDb->getActiveLayoutBTRId().safeOpenObject(OdDb::kForWrite);

  OdDbViewportTablePtr vTable =  OdDbViewportTable::cast(pDb->getViewportTableId().openObject());
  OdDbViewportTableRecordPtr vRec = OdDbViewportTableRecord::cast(vTable->getAt(OD_T("*Active"), OdDb::kForRead));
  OdGeVector3d hatchNormal = vRec->viewDirection();
  hatchNormal.normalize();

  OdDbHatchPtr      pHatch = OdDbHatch::createObject();
  pHatch->setDatabaseDefaults(pDb);
  pHatch->setPattern(OdDbHatch::kPreDefined, OD_T("ANSI31")); //OD_T("SOLID")); //OD_T("ANSI31")); //OD_T("ANGLE")); OD_T("ANSI31")
  OdCmColor color;
  color.setColorIndex(1);
  pHatch->setColor(color);
  pHatch->setAssociative(true);
  pHatch->setPatternScale(0.25);
  pHatch->setNormal(hatchNormal);
  OdDbObjectId hatchId = pBTR->appendOdDbEntity(pHatch);
  
  OdGePlane hatchPlane(OdGePoint3d::kOrigin, hatchNormal); // hatch elevation is zero
  // Each entity as single loop
  int loopIndex = 0;
  while ( !pIter->done() )
  {
    OdDbObjectId objId = pIter->objectId();
    if (objId.isNull())
      continue;
    OdDbObjectIdArray loopIds;
    loopIds.append(objId);
    if (OdUInt32 subents = pIter->subentCount())
    {
      OdDbEntityPtr pMainEnt = objId.safeOpenObject(OdDb::kForWrite);
      EdgeArray edges;
      bool bTextBox = false;
      for (OdUInt32 i = 0; i < subents; ++i)
      {
        OdDbFullSubentPath subEntPath;
        if (pIter->getSubentity(i, subEntPath))
        {
          OdDbEntityPtr subent = pMainEnt->subentPtr(subEntPath);
          if (subent.isNull())
            continue;
          if (i == 0)
            bTextBox = subent->isKindOf(OdDbText::desc()) || subent->isKindOf(OdDbMText::desc()) || subent->isKindOf(OdDbFcf::desc());
          edges.append(oddbCreateEdgesFromEntity(subent, hatchPlane));
        }
      }
      int loopType = OdDbHatch::kExternal;
      if (bTextBox)
        loopType |= OdDbHatch::kTextbox;
      pHatch->appendLoop(loopType, edges);
      pHatch->setAssocObjIdsAt(loopIndex, loopIds);
      pMainEnt->addPersistentReactor(hatchId);
    }
    else
    {
      pHatch->appendLoop(OdDbHatch::kExternal, loopIds);
    }
    ++loopIndex;
    pIter->next();
  }
  pHatch->evaluateHatch();
}

#include "DbRegAppTableRecord.h"
#include "DbViewTableRecord.h"
#include "DbViewportTableRecord.h"
#include "DbDimStyleTableRecord.h"

void _testSymRecordUnErase_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();

  OdUInt32 nKeyword = pIO->getKeyword(OD_T("Enter type of record [Block/Dimstyle/LAyer/LType/Regapp/textSTyle/View/VPort]:"),
        OD_T("Block Dimstyle LAyer LType Regapp textSTyle View VPort"));
  OdRxClass* pRecClass = 0;
  OdDbObjectId objId;
  switch (nKeyword)
  {
  case 0: //Block
    pRecClass = OdDbBlockTableRecord::desc();
    objId = pDb->getBlockTableId();
    break;
  case 1: //Dimstyle
    pRecClass = OdDbDimStyleTableRecord::desc();
    objId = pDb->getDimStyleTableId();
    break;
  case 2: //LAyer
    pRecClass = OdDbLayerTableRecord::desc();
    objId = pDb->getLayerTableId();
    break;
  case 3: //LType
    pRecClass = OdDbLinetypeTableRecord::desc();
    objId = pDb->getLinetypeTableId();
    break;
  case 4: //Regapp
    pRecClass = OdDbRegAppTableRecord::desc();
    objId = pDb->getRegAppTableId();
    break;
  case 5: //textSTyle
    pRecClass = OdDbTextStyleTableRecord::desc();
    objId = pDb->getTextStyleTableId();
    break;
  case 6: //View
    pRecClass = OdDbViewTableRecord::desc();
    objId = pDb->getViewTableId();
    break;
  case 7: //VPort
    pRecClass = OdDbViewportTableRecord::desc();
    objId = pDb->getViewportTableId();
    break;
  }

  if (!pRecClass)
    return;
  OdDbSymbolTablePtr pTable = objId.safeOpenObject(OdDb::kForWrite);

  // Note: without Undo the objects are erased from database permanently
  pDb->disableUndoRecording(false);
  pDb->startUndoRecord();

  OdResult res;
  OdString blockName = OD_T("1");
  OdDbObjectId testId;

  // add record
  OdDbSymbolTableRecordPtr pRec = pRecClass->create();
  pRec->setName(blockName);
  OdDbObjectId recId1 = pTable->add(pRec);
  ODA_ASSERT(pTable->has(blockName));
  testId = pTable->getAt(blockName, false);
  ODA_ASSERT(testId == recId1);
  testId = pTable->getAt(blockName, true);
  ODA_ASSERT(testId == recId1);
  pTable.release();
  pRec.release();
  pTable = objId.openObject();

  // erase 1st record
  pRec = recId1.openObject(OdDb::kForWrite);
  res = pRec->erase();
  ODA_ASSERT(res==eOk);
  ODA_ASSERT(!pTable->has(blockName));
  testId = pTable->getAt(blockName, false);
  ODA_ASSERT(testId == OdDbObjectId::kNull);
  testId = pTable->getAt(blockName, true);
  ODA_ASSERT(testId == recId1);

  // add 2nd record with same name
  pTable->upgradeOpen();
  pRec = pRecClass->create();
  pRec->setName(blockName);
  OdDbObjectId recId2 = pTable->add(pRec);
  pRec.release();
  pTable->downgradeOpen();
  ODA_ASSERT(pTable->has(blockName));
  testId = pTable->getAt(blockName, false);
  ODA_ASSERT(testId == recId2);
  testId = pTable->getAt(blockName, true);
  ODA_ASSERT(testId == recId1 || testId == recId2);

  // try un-erase 1st record
  pRec = recId1.safeOpenObject(OdDb::kForWrite, true);
  res = pRec->erase(false);
  ODA_ASSERT(res==eDuplicateRecordName); // assert for Viewport as it does not need a name unicity

  // erase 2nd record
  pRec = recId2.safeOpenObject(OdDb::kForWrite);
  res = pRec->erase();
  ODA_ASSERT(res==eOk);
  ODA_ASSERT(!pTable->has(blockName));
  testId = pTable->getAt(blockName, false);
  ODA_ASSERT(testId == OdDbObjectId::kNull);
  testId = pTable->getAt(blockName, true);
  ODA_ASSERT(testId == recId1 || testId == recId2);

  // try un-erase 1st record again
  pRec = recId1.safeOpenObject(OdDb::kForWrite, true);
  res = pRec->erase(false);
  ODA_ASSERT(res==eOk);
  ODA_ASSERT(pTable->has(blockName));
  testId = pTable->getAt(blockName, false);
  ODA_ASSERT(testId == recId1);
  testId = pTable->getAt(blockName, true);
  ODA_ASSERT(testId == recId1 || testId == recId2);
}

void _explodeGeometryToBlock_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();

  OdDbSelectionSetPtr pSSet = pIO->select( L"Select objects to explode:", OdEd::kSelAllowInactSpaces|OdEd::kSelAllowLocked );

  OdDbSelectionSetIteratorPtr pIter = pSSet->newIterator();

  while ( !pIter->done() )
  {
    OdDbObjectId objId = pIter->objectId();
    OdDbEntityPtr pEnt = objId.openObject(OdDb::kForWrite);
    if (!pEnt.isNull())
    {
      OdDbBlockTableRecordPtr pBlk = pEnt->ownerId().openObject(OdDb::kForWrite);
      if (!pBlk.isNull())
      {
        if (pEnt->explodeGeometryToBlock(pBlk) == eOk)
        {
          pEnt->erase();
        }
      }
    }
    pIter->next();
  }
}

void _explodeGeometry_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();

  OdDbSelectionSetPtr pSSet = pIO->select( L"Select objects to explode:", OdEd::kSelAllowInactSpaces|OdEd::kSelAllowLocked );

  OdDbSelectionSetIteratorPtr pIter = pSSet->newIterator();

  while ( !pIter->done() )
  {
    OdDbObjectId objId = pIter->objectId();
    OdDbEntityPtr pEnt = objId.openObject(OdDb::kForWrite);
    if (!pEnt.isNull())
    {
      OdDbBlockTableRecordPtr pBlk = pEnt->ownerId().openObject(OdDb::kForWrite);
      if (!pBlk.isNull())
      {
        OdRxObjectPtrArray arrRes;
        if (pEnt->explodeGeometry(arrRes) == eOk)
        {
          int nSize = arrRes.size();
          if (nSize)
          {
            OdRxObjectPtr* pObj = arrRes.asArrayPtr();
            while (nSize--)
            {
              OdDbEntity* pResEnt = (OdDbEntity*)((OdRxObject*)(*pObj++));
              pBlk->appendOdDbEntity(pResEnt);
            }
            pEnt->erase();
          }
        }
      }
    }
    pIter->next();
  }
}

void _explodeToBlock_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();

  OdDbSelectionSetPtr pSSet = pIO->select( L"Select objects to explode:", OdEd::kSelAllowInactSpaces|OdEd::kSelAllowLocked );

  OdDbSelectionSetIteratorPtr pIter = pSSet->newIterator();

  while ( !pIter->done() )
  {
    OdDbObjectId objId = pIter->objectId();
    OdDbEntityPtr pEnt = objId.openObject(OdDb::kForWrite);
    if (!pEnt.isNull())
    {
      OdDbBlockTableRecordPtr pBlk = pEnt->ownerId().openObject(OdDb::kForWrite);
      if (!pBlk.isNull())
      {
        OdDbObjectIdArray arrEnts;
        OdResult res = pEnt->explodeToBlock(pBlk, &arrEnts);
        if ( res == eOk )
        {
          OdDbSortentsTablePtr pSortEntsTable = pBlk->getSortentsTable();
          pSortEntsTable->moveBelow(arrEnts, objId);
          pEnt->erase();
        }
        else
        {
          pIO->putString( pDb->appServices()->getErrorDescription(res) );
        }
      }
    }
    pIter->next();
  }
}

void _explode_test_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();

  OdDbSelectionSetPtr pSSet = pIO->select( L"Select objects to explode:", OdEd::kSelAllowInactSpaces|OdEd::kSelAllowLocked );

  OdDbSelectionSetIteratorPtr pIter = pSSet->newIterator();

  while ( !pIter->done() )
  {
    OdDbObjectId objId = pIter->objectId();
    OdDbEntityPtr pEnt = objId.openObject(OdDb::kForWrite);
    if (!pEnt.isNull())
    {
      OdDbBlockTableRecordPtr pBlk = pEnt->ownerId().openObject(OdDb::kForWrite);
      if (!pBlk.isNull())
      {
        OdRxObjectPtrArray arrRes;
        OdResult res = pEnt->explode(arrRes);
        if (res == eOk)
        {
          int nSize = arrRes.size();
          if (nSize)
          {
            OdRxObjectPtr* pObj = arrRes.asArrayPtr();
            while (nSize--)
            {
              OdDbEntity* pResEnt = (OdDbEntity*)((OdRxObject*)(*pObj++));
              pBlk->appendOdDbEntity(pResEnt);
            }
            pEnt->erase();
          }
        }
        else
        {
          pIO->putString( pDb->appServices()->getErrorDescription(res) );
        }
      }
    }
    pIter->next();
  }
}

#include "RxModule.h"

void _GetClassDictionary_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();

  OdRxDictionaryIteratorPtr pClassIter = odrxClassDictionary()->newIterator(OdRx::kDictSorted);
  for ( ; !pClassIter->done(); pClassIter->next())
  {
    OdRxClassPtr pClass = pClassIter->object();
    OdString sName = pClass->name();
    OdString sAppName = pClass->appName();
    OdString sDxfName = pClass->dxfName();
    OdUInt32 flags = pClass->proxyFlags();
    OdString sParentName;
    OdRxClass* pParent = pClass->myParent();
    if (pParent) sParentName = pParent->name();
    OdString sModuleName;
    OdRxModule* pModule = pClass->module();
    if (pModule) sModuleName = pModule->moduleName();
    if (!sName.isEmpty())
    {
      OdString s;
      s.format(OD_T("%-40s, %-40s, %-40s, %08x, %-40s, %-40s"), sName.c_str(), sDxfName.c_str(), sParentName.c_str(), flags, sModuleName.c_str(), sAppName.c_str());
      pIO->putString(s);
      ODA_TRACE1("%ls\n", s.c_str());
    }
  }
}

void _getPointAtParam_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);

  OdDbSelectionSetIteratorPtr pIter = pDbCmdCtx->dbUserIO()->select()->newIterator();
  for ( ; !pIter->done(); pIter->next() )
  {
    OdDbCurvePtr curve = OdDbCurve::cast( pIter->objectId().openObject() );
    if ( curve.isNull() )
      continue;
    double p1 = -std::numeric_limits<double>::max();
    double p2 = std::numeric_limits<double>::max();
    OdString s;
    if ( curve->getStartParam( p1 ) == eOk && curve->getEndParam(p2) == eOk )
      s.format(OD_T("Enter parameter [%g .. %g]"), p1, p2 );
    else s = OD_T("Enter parameter");
    double p = pDbCmdCtx->userIO()->getReal( s );
    if ( p < p1 || p > p2 )
      pDbCmdCtx->userIO()->putString(OD_T("Invalid parameter value"));
    else
    {
      OdGePoint3d point;
      OdResult res = curve->getPointAtParam( p, point );
      if ( res != eOk )
      {
        pDbCmdCtx->userIO()->putString( 
          pDbCmdCtx->database()->appServices()->getErrorDescription(res) );
        return;
      }
      s.format( OD_T("Point: (%g,%g,%g)"), point.x, point.y, point.z );
      pDbCmdCtx->userIO()->putString( s );
    }
  }
}

void _getFirstDeriv_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  
  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();

  OdDbSelectionSetPtr pSSet = pIO->select( OD_T("Select curve to test") );

  OdDbSelectionSetIteratorPtr pIter = pIO->select()->newIterator();
  for ( ; !pIter->done(); pIter->next() )
  {
    OdDbCurvePtr curve = OdDbCurve::cast( pIter->objectId().openObject() );
    if ( curve.isNull() )
      continue;
    double p1 = -std::numeric_limits<double>::max();
    double p2 = std::numeric_limits<double>::max();
    OdString s;
    if ( curve->getStartParam( p1 ) == eOk && curve->getEndParam(p2) == eOk )
      s.format(OD_T("Enter parameter [%g,%g]"), p1, p2 );
    else s = OD_T("Enter parameter");
    double p = pIO->getReal( s );
    if ( p < p1 || p > p2 )
      pIO->putString(OD_T("Invalid parameter value"));
    else
    {
      OdGeVector3d v;
      OdResult res = curve->getFirstDeriv( p, v );
      if ( res != eOk )
      {
        pDbCmdCtx->userIO()->putString( 
          pDbCmdCtx->database()->appServices()->getErrorDescription(res) );
      }
      else
      {
        s.format( OD_T("First derivative: (%g,%g,%g)"), v.x, v.y, v.z );
        pIO->putString( s );
      }
      res = curve->getSecondDeriv( p, v );
      if ( res != eOk )
      {
        pIO->putString( 
          pDbCmdCtx->database()->appServices()->getErrorDescription(res) );
      }
      else
      {
        s.format( OD_T("Second derivative: (%g,%g,%g)"), v.x, v.y, v.z );
        pIO->putString( s );
      }
    }
  }
}

void _getParamAtDist_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);

  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();

  OdDbSelectionSetPtr pSSet = pIO->select( OD_T("Select curve to test") );

  OdDbSelectionSetIteratorPtr pIter = pIO->select()->newIterator();
  for ( ; !pIter->done(); pIter->next() )
  {
    OdDbCurvePtr curve = OdDbCurve::cast( pIter->objectId().openObject() );
    if ( curve.isNull() ) continue;
    double dist = pDbCmdCtx->userIO()->getReal( OD_T("Enter distance") );
    double param = 0;
    OdResult res = curve->getParamAtDist( dist, param );
    if ( res != eOk )
    {
      pDbCmdCtx->userIO()->putString( 
        pDbCmdCtx->database()->appServices()->getErrorDescription(res) );
      return;
    }
    OdString s;
    s.format( OD_T("Parameter: %g"), param );
    pDbCmdCtx->userIO()->putString( s );
  }
}

void _getDistAtParam_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);

  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();

  OdDbSelectionSetPtr pSSet = pIO->select( OD_T("Select curve to test"), OdEd::kSelSingleEntity|OdEd::kSelSinglePass );

  OdDbSelectionSetIteratorPtr pIter = pIO->select()->newIterator();
  for ( ; !pIter->done(); pIter->next() )
  {
    OdDbCurvePtr curve = OdDbCurve::cast( pIter->objectId().openObject() );
    if ( curve.isNull() )
      continue;
    double p1 = -std::numeric_limits<double>::max();
    double p2 = std::numeric_limits<double>::max();
    OdString s;
    if ( curve->getStartParam( p1 ) == eOk && curve->getEndParam(p2) == eOk )
      s.format(OD_T("Enter parameter [%g,%g]"), p1, p2 );
    else s = OD_T("Enter parameter");
    double p = pDbCmdCtx->userIO()->getReal( s );
    if ( p < p1 || p > p2 )
      pDbCmdCtx->userIO()->putString(OD_T("Invalid parameter value"));
    else
    {
      double dist = 0;
      OdResult res = curve->getDistAtParam( p, dist );
      if ( res != eOk )
      {
        pDbCmdCtx->userIO()->putString( 
          pDbCmdCtx->database()->appServices()->getErrorDescription(res) );
        return;
      }
      s.format( OD_T("Distance: %g"), dist );
      pDbCmdCtx->userIO()->putString( s );
   }
  }
}

void _getOffsetCurves_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);

  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();

  ::odrxDynamicLinker()->loadApp(OdCurveFunctionsModuleName);
  OdDbSelectionSetPtr pSSet = pIO->select( OD_T("Select curve to test") );
  OdDbSelectionSetIteratorPtr pIter = pIO->select()->newIterator();
  for ( ; !pIter->done(); pIter->next() )
  {
    OdDbCurvePtr curve = OdDbCurve::cast( pIter->objectId().openObject() );
    if ( curve.isNull() )
      continue;
    double offset = pIO->getReal( OD_T("Enter offset") );
    OdRxObjectPtrArray curves; 
    if ( eOk == curve->getOffsetCurves( offset, curves ) )
    {
      OdDbBlockTableRecordPtr btr = curve->ownerId().safeOpenObject(OdDb::kForWrite);
      for ( unsigned i = 0; i < curves.size(); i++ )
        btr->appendOdDbEntity( (OdDbEntity*)curves[i].get() );
    }
  }
}

void _getArea_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);

  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();

  OdDbSelectionSetPtr pSSet = pIO->select( OD_T("Select curve to test") );
  ::odrxDynamicLinker()->loadApp(OdCurveFunctionsModuleName);
  OdDbSelectionSetIteratorPtr pIter = pIO->select()->newIterator();
  for ( ; !pIter->done(); pIter->next() )
  {
    double area;
    OdDbEntityPtr ent = pIter->objectId().openObject();
    if (ent->isKindOf(OdDbCurve::desc()))
    { 
      OdDbCurvePtr curve = ent;
      if (eOk == curve->getArea(area))
      {
        OdString s; s.format(OD_T("Area: %g"), area);
        pIO->putString(s);
      }
    }
    else if (ent->isKindOf(OdDbHatch::desc()))
    {
      OdDbHatchPtr h = ent;
      if (eOk == h->getArea(area))
        pIO->putString(OdString().format(OD_T("Area: %10.9f"), area));
      else 
        pIO->putString(L"Hatch has non-closed or self-intersecting boundaries");
    }
  }
}

#include <DbSpline.h>

void _getSpline_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);

  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();

  OdDbSelectionSetPtr pSSet = pIO->select( OD_T("Select curve to test") );
  ::odrxDynamicLinker()->loadApp(OdCurveFunctionsModuleName);
  OdDbSelectionSetIteratorPtr pIter = pIO->select()->newIterator();
  for ( ; !pIter->done(); pIter->next() )
  {
    OdDbCurvePtr curve = OdDbCurve::cast( pIter->objectId().openObject() );
    if ( curve.isNull() )
      continue;
    OdDbSplinePtr spline;
    if ( eOk == curve->getSpline( &spline ) )
    {
      OdDbBlockTableRecordPtr btr = curve->ownerId().safeOpenObject(OdDb::kForWrite);
      btr->appendOdDbEntity( spline );
    }
  }
}

void _extend_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);

  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();

  OdDbSelectionSetPtr pSSet = pIO->select( OD_T("Select curve to extend") );
  ::odrxDynamicLinker()->loadApp(OdCurveFunctionsModuleName);
  OdDbSelectionSetIteratorPtr pIter = pIO->select()->newIterator();
  for ( ; !pIter->done(); pIter->next() )
  {
    OdDbCurvePtr curve = OdDbCurve::cast( pIter->objectId().openObject(OdDb::kForWrite) );
    if ( curve.isNull() )
      continue;
    double p1,p2;
    if ( curve->getStartParam( p1 ) != eOk || curve->getEndParam(p2) != eOk )
    {
      pCmdCtx->userIO()->putString(OD_T("Error obtaining curve bounds"));
      continue;
    }
    OdString s; s.format(OD_T("Enter parameter [%g,%g]"), p1, p2 );
    double p;
    try
    {
      p = pIO->getReal( s );
    }
    catch(const OdEdEmptyInput)
    {
      p = p1;
    }
    if ( eOk != curve->extend( p ) )
      pCmdCtx->userIO()->putString(OD_T("Error extending curve"));
  }
  pIO->select()->clear();
}

void _reverseCurve_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);

  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();

  OdDbSelectionSetPtr pSSet = pIO->select( OD_T("Select curve(s) to reverse") );
  OdDbSelectionSetIteratorPtr pIter = pIO->select()->newIterator();
  OdUInt32 nProc = 0, nFail = 0;
  for ( ; !pIter->done(); pIter->next() )
  {
    OdDbCurvePtr curve = OdDbCurve::cast( pIter->objectId().openObject(OdDb::kForWrite) );
    if ( curve.isNull() )
    {
      nFail++;
      continue;
    }
    curve->reverseCurve();
    nProc++;
  }
  OdString procFmt;
  procFmt.format(OD_T("%u curve(s) reversed (%u ignored)"), nProc, nFail);
  pIO->putString(procFmt);
}

void _setBlockInsertUnits_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbSelectionSetIteratorPtr pIter = pDbCmdCtx->dbUserIO()->select(L"", OdEd::kSelAllowObjects)->newIterator();
  for ( ; !pIter->done(); pIter->next() )
  {
    OdDbBlockTableRecordPtr block = OdDbBlockTableRecord::cast( pIter->objectId().openObject(OdDb::kForWrite) );
    if ( block.isNull() )
      continue;
    OdDb::UnitsValue units = block->blockInsertUnits();
    OdString s; s.format( OD_T("Enter block insert units number [%d]"), (int)units );
    units = (OdDb::UnitsValue)pDbCmdCtx->userIO()->getInt( s, OdEd::kInpDefault, units );
    block->setBlockInsertUnits(units);
  }
}

#include "DbAttributeDefinition.h"

void _setLockPositionInBlock_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();
  for ( OdDbSelectionSetIteratorPtr pIter = pIO->select(L"Select attribute definition", OdEd::kSelAllowObjects)->newIterator();
    !pIter->done(); pIter->next() )
  {
    OdDbAttributeDefinitionPtr attr = OdDbAttributeDefinition::cast( pIter->objectId().openObject(OdDb::kForWrite) );
    if ( attr.isNull() )
      continue;
    bool value = attr->lockPositionInBlock();
    OdString s; s.format( OD_T("Enter attribute definition \"lock position\" flag [%d]"),(int)value);
    value = pDbCmdCtx->userIO()->getInt(s, OdEd::kInpDefault, (int)value) != 0;
    attr->setLockPositionInBlock(value);
  }
}

#include "DbDatabaseCollection.h"

void _ListOpenDatabases_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  for (OdRxIteratorPtr it = oddbDatabaseCollection()->newIterator(); !it->done(); it->next())
    pCmdCtx->userIO()->putString( OdDbDatabasePtr(it->object())->getFilename() );
}

#include "DbGroup.h"
#include "DbDictionary.h"
#include "DbLine.h"
#include "DbAudit.h"

void _testGroup_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();

  // Create test DbGroup entry
  OdDbDictionaryPtr pGroupDic = pDb->getGroupDictionaryId().safeOpenObject(OdDb::kForWrite);
  OdDbGroupPtr pGroup = OdDbGroup::createObject();
  pGroupDic->setAt(OD_T("OdaTestGroup"), pGroup);
  pGroup->setName(OD_T("OdaTestGroup"));
  pGroup->setSelectable(true);

  // Create entities and append them to database
  int n = 6;
  OdDbObjectIdArray entIds1, entIds2;
  OdDbBlockTableRecordPtr pBlock = pDb->getModelSpaceId().safeOpenObject(OdDb::kForWrite);
  OdDbLinePtr pLine;
  OdGePoint3d start, end(1.0, 1.0, 0.0);
  int i;
  for (i = 0; i < n; ++i)
  {
    pLine = OdDbLine::createObject();
    pLine->setStartPoint(start);
    pLine->setEndPoint(end);
    entIds1.push_back(pBlock->appendOdDbEntity(pLine));
    start = end; end.x += 1.0; end.y = -end.y;
  }
  for (i = 0; i < n; ++i)
  {
    pLine = OdDbLine::createObject();
    pLine->setStartPoint(start);
    pLine->setEndPoint(end);
    entIds2.push_back(pBlock->appendOdDbEntity(pLine));
    start = end; end.x += 1.0; end.y = -end.y;
  }

  // Append entities to group
  pGroup->append(entIds1);
  OdDbObjectIdArray ids1, ids2;
  pGroup->allEntityIds(ids1);

  // Test transform():
  // - non intersected ranges
  pGroup->transfer(1,n/3*2,n/3); pGroup->allEntityIds(ids2); ODA_ASSERT(ids2[0]==ids1[0] && ids2[1]==ids1[4] && ids2[2]==ids1[5] && ids2[3]==ids1[3] && ids2[4]==ids1[1] && ids2[5]==ids1[2]);
  pGroup->transfer(n/3*2,1,n/3); pGroup->allEntityIds(ids2); ODA_ASSERT(ids2==ids1);
  pGroup->transfer(0,n/2,n/2); pGroup->allEntityIds(ids2); ODA_ASSERT(ids2[0]==ids1[3] && ids2[1]==ids1[4] && ids2[2]==ids1[5] && ids2[3]==ids1[0] && ids2[4]==ids1[1] && ids2[5]==ids1[2]);
  pGroup->transfer(n/2,0,n/2); pGroup->allEntityIds(ids2); ODA_ASSERT(ids2==ids1);
  // - intersected ranges
  pGroup->transfer(0,n/3,n/3*2); pGroup->allEntityIds(ids2); ODA_ASSERT(ids2[0]==ids1[4] && ids2[1]==ids1[5] && ids2[2]==ids1[0] && ids2[3]==ids1[1] && ids2[4]==ids1[2] && ids2[5]==ids1[3]);
  pGroup->transfer(n/3,0,n/3*2); pGroup->allEntityIds(ids2); ODA_ASSERT(ids2==ids1);

  // Test insertAt() and removeAt() for array of entities
  pGroup->insertAt(0, entIds2); pGroup->allEntityIds(ids2); ODA_ASSERT(ids2[0]==entIds2[0] && ids2[entIds2.size()]==ids1[0]);
  pGroup->removeAt(0, entIds2); pGroup->allEntityIds(ids2); ODA_ASSERT(ids2==ids1);

  // Test insertAt() and removeAt() for entity
  pGroup->insertAt(0, entIds2[0]); pGroup->allEntityIds(ids2); ODA_ASSERT(ids2[0]==entIds2[0] && ids2[1]==ids1[0]);
  pGroup->removeAt(0); pGroup->allEntityIds(ids2); ODA_ASSERT(ids2==ids1);
  pGroup->insertAt(n/2, entIds2[0]); pGroup->allEntityIds(ids2); ODA_ASSERT(ids2[3]==entIds2[0] && ids2[2]==ids1[2] && ids2[4]==ids1[3]);
  pGroup->removeAt(n/2); pGroup->allEntityIds(ids2); ODA_ASSERT(ids2==ids1);
  pGroup->insertAt(n, entIds2[0]); pGroup->allEntityIds(ids2); ODA_ASSERT(ids2.size()==ids1.size()+1 && ids2[ids2.size()-2]==ids1[ids1.size()-1] && ids2[ids2.size()-1]==entIds2[0]);
  pGroup->removeAt(n); pGroup->allEntityIds(ids2); ODA_ASSERT(ids2==ids1);

  OdDbGroupIteratorPtr pGrIter = pGroup->newIterator();
  while (!pGrIter->done())
  {
    OdDbObjectId objId = pGrIter->objectId();
    OdUInt32 index;
    pGroup->getIndex(objId, index);
    ODA_TRACE2("\n%ls <%d>", odDbGetObjectIdName(objId).c_str(), index);
    pGrIter->next();
  }
}

#include "Gi/GiGeometrySimplifier.h"
#include "Gi/GiBaseVectorizer.h"
#include "GiContextForDbDatabase.h"
struct DumpVectorizer : OdGiGeometrySimplifier, OdGiBaseVectorizer
{
  OdDbCommandContext* _ctx;
  void polylineOut(OdInt32 n, const OdGePoint3d* pts)
  {
    OdString s;s.format(OD_T("%d points:"),n);
    _ctx->userIO()->putString(s);
    for ( const OdGePoint3d* pp = pts; pp != pts + n; ++pp )
    {
      s.format( OD_T("(%g,%g,%g)"), pp->x, pp->y, pp->z );
      _ctx->userIO()->putString(s);
    }
  }
};

void _dumpText_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();
  for ( OdDbSelectionSetIteratorPtr pIter = pIO->select()->newIterator();
    !pIter->done(); pIter->next() )
  {
    OdDbEntityPtr pEnt = OdDbEntity::cast( pIter->objectId().openObject() );
    if ( pEnt.isNull() )
      continue;
    OdStaticRxObject<DumpVectorizer> dv;
    OdStaticRxObject<OdGiContextForDbDatabase> giContext;
    giContext.setDatabase( pDbCmdCtx->database(), false );
    dv.setDrawContext( &dv );
    dv.setContext( &giContext );
    dv._ctx = pDbCmdCtx;
    dv.output().setDestGeometry( dv );
    dv.draw( pEnt );
  }
}

#include "DbDimension.h"

void _testFormatMeasurement_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);

  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();

  OdDbSelectionSetPtr pSSet = pIO->select( OD_T("Select dimensions") );

  OdString res, dimTxt;
  double measurement;
  try {
    measurement = pIO->getReal(OD_T("Enter measurement <1.0>"));
  }
  catch(const OdEdEmptyInput) {
    measurement = 1.0;
  }

  OdDbSelectionSetIteratorPtr it = pSSet->newIterator();
  for (; !it->done(); it->next())
  {
    OdDbDimensionPtr pDim = OdDbDimension::cast(it->objectId().openObject());
    if (pDim.get())
    {
      dimTxt = pDim->dimensionText();
      pDim->formatMeasurement(res, measurement, dimTxt);

    }
    OdString s;
    s.format(OD_T("\n%ls string is \"%ls\" "), odDbGetObjectName(pDim).c_str(), res.c_str());
    pDbCmdCtx->userIO()->putString(s);
  }
  pIO->select()->clear();
}

static void getUCS(OdDbDatabase* pDb,OdGeMatrix3d& m)
{
  OdGePoint3d ucsOrigin;
  OdGeVector3d ucsXAxis, ucsYAxis;
  if(pDb->getTILEMODE())
  {
    ucsOrigin = pDb->getUCSORG();
    ucsXAxis  = pDb->getUCSXDIR();
    ucsYAxis  = pDb->getUCSYDIR();
  }
  else
  {
    ucsOrigin = pDb->getPUCSORG();
    ucsXAxis  = pDb->getPUCSXDIR();
    ucsYAxis  = pDb->getPUCSYDIR();
  }
  m.setCoordSystem(ucsOrigin, ucsXAxis, ucsYAxis, ucsXAxis.crossProduct(ucsYAxis).normalize());
}
static OdGePoint3d selectByRect( const OdGePoint3d& origin, OdDbUserIO* pIO, OdDbDatabase* pDb, const OdGeMatrix3d& x )
{
  struct StretchPointSelector : OdStaticRxObject<OdEdPointTracker>
  {
    OdDbPolylinePtr _ent;
    const OdGeMatrix3d& _x;
    StretchPointSelector(const OdGePoint3d& origin,OdDbDatabase* pDb,const OdGeMatrix3d& x):_x(x)
    {

      _ent = OdDbPolyline::createObject();
      _ent->setDatabaseDefaults(pDb);
      OdGePoint2d org = (_x*origin).convert2d();
      _ent->addVertexAt( 0, org );
      _ent->addVertexAt( 1, org );
      _ent->addVertexAt( 2, org );
      _ent->addVertexAt( 3, org );
      _ent->setClosed( true );
    }
    virtual void setValue(const OdGePoint3d& p)
    {
      OdGePoint2d p1 = (_x*p).convert2d();
      OdGePoint2d p0;_ent->getPointAt(0,p0);
      _ent->setPointAt(2,p1);
      _ent->setPointAt(1,OdGePoint2d(p0.x,p1.y));
      _ent->setPointAt(3,OdGePoint2d(p1.x,p0.y));
    }
    virtual int addDrawables(OdGsView* pView)
    {
      pView->add(_ent, 0);
      return 1;
    }
    virtual void removeDrawables(OdGsView* pView)
    {
      pView->erase(_ent);
    }
  }
  selector( origin, pDb, x );

  return pIO->getPoint(OD_T("Select stretch points"),
    OdEd::kGptDefault, &origin, OdString::kEmpty, &selector );
}
void trackStretchPoints( OdGePoint3dArray& pts, const OdIntArray& ia, OdDbEntity* curve, OdDbUserIO* pIO )
{
  OdGePoint3d p = pts[ia[0]];
  struct StretchTracker : OdStaticRxObject<OdEdPointTracker>
  {
    OdDbEntity* _crv;
    const OdIntArray& _indices;
    const OdGePoint3dArray& _pts;
    OdGePoint3d _origin;
  public:
    StretchTracker( OdDbEntity* crv, const OdIntArray& indices, 
      const OdGePoint3dArray& pp, const OdGePoint3d& o )
      : _crv(crv), _indices(indices), _pts(pp), _origin(o)
    {
    }
    virtual void setValue(const OdGePoint3d& p)
    {
      _crv->upgradeOpen();
      _crv->moveStretchPointsAt( _indices, p - _origin );
      _origin = p;
      _crv->downgradeOpen();
    }
    virtual int addDrawables(OdGsView* pView)
    {
      pView->add(_crv, 0);
      return 1;
    }
    virtual void removeDrawables(OdGsView* pView)
    {
      pView->erase(_crv);
    }
  } tracker( curve, ia, pts, p );
  pIO->getPoint( OD_T("Enter displacement"), OdEd::kGptDefault, &p, OdString::kEmpty, &tracker);
}

void _STRETCH_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbUserIO* pIO = (OdDbUserIO*)pDbCmdCtx->userIO();

  OdDbSelectionSetPtr pSSet = pIO->select( OD_T("Select entities") );
  OdGeMatrix3d x; getUCS( pDb, x );
  OdGePoint3d origin = pIO->getPoint(OD_T("Select stretch points"));
  OdGePoint3d res = selectByRect( origin, pIO, pDb, x );
  OdGePoint2d p0 = (x*origin).convert2d();
  OdGePoint2d p1 = (x*res).convert2d();
  if ( p0.x > p1.x )
    std::swap( p0.x, p1.x );
  if ( p0.y > p1.y )
    std::swap( p0.y, p1.y );

  OdDbSelectionSetIteratorPtr it = pSSet->newIterator();
  for (; !it->done(); it->next())
  {
    OdDbEntityPtr curve = OdDbEntity::cast( it->objectId().openObject() );
    if ( curve.isNull() )
      continue;
    OdGePoint3dArray pts;
    if ( curve->getStretchPoints( pts ) != eOk )
      continue;
    OdIntArray ia;
    for ( unsigned i = 0 ;i < pts.size(); i++ )
    {
      OdGePoint2d p = (x*pts[i]).convert2d();
      if ( p.x <= p1.x && p.x >= p0.x && p.y >= p0.y && p.y <= p1.y )
        ia.push_back(i);
    }
    if ( ia.empty() )
      continue;
    trackStretchPoints( pts, ia, curve, pIO );
    break;
  }
}
void _selectByLayer_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdString layer = pDbCmdCtx->userIO()->getString(OD_T("Input layer name <0>:"), OdEd::kGstNoSpaces, OD_T("0"));
  OdDbSelectionSetPtr pSSet = OdDbSelectionSet::select(pDbCmdCtx->database(), OdResBuf::newRb(8, layer));
  OdString s;
  s.format(OD_T("%d entities on layer %ls"), pSSet->numEntities(), layer.c_str());
  pDbCmdCtx->userIO()->putString(s);
}

void _getParamAtPoint_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);

  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();

  OdDbSelectionSetPtr pSSet = pIO->select( OD_T("Select curve to test") );
  for ( OdDbSelectionSetIteratorPtr pIter = pSSet->newIterator(); !pIter->done(); pIter->next() )
  {
    OdString s;
    OdDbCurvePtr curve = OdDbCurve::cast( pIter->objectId().openObject() );
    if ( curve.isNull() ) continue;
    double start, end;

    OdResult res = curve->getStartParam(start);
    if (eOk != res)
    {
      pDbCmdCtx->userIO()->putString(OD_T("getStartParam failed"));
      throw OdError(res);
    }
    res = curve->getEndParam(end);
    if (eOk != res)
    {
      pDbCmdCtx->userIO()->putString(OD_T("getEndParam failed"));
      throw OdError(res);
    }
    double delta = (end - start)/12;
    for (int i = 0; i <= 12; ++i)
    {
      OdGePoint3d p;
      double param = start + delta*i;
      res = curve->getPointAtParam(param, p);
      if (eOk != res)
      {
        pDbCmdCtx->userIO()->putString(OD_T("getPointAtParam failed"));
        throw OdError(res);
      }
      else
      {
        s.format(OD_T("getPointAtParam(%g) = (%g,%g,%g)"), param, p.x, p.y, p.z );
        pDbCmdCtx->userIO()->putString( s );
      }
      double param1 = 0;
      res = curve->getParamAtPoint(p, param1);
      if (eOk != res)
      {
        pDbCmdCtx->userIO()->putString(OD_T("getParamAtPoint failed"));
        throw OdError(res);
      }
      else
      {
        s.format(OD_T("getParamAtPoint((%g,%g,%g)) = %g"), p.x, p.y, p.z, param1);
        pDbCmdCtx->userIO()->putString( s );
      }
      if ( i < 12 && !OdEqual(param, param1, 1.e-7 ))
      {
        pDbCmdCtx->userIO()->putString(OD_T("Test failed"));
      }
      else
      {
        pDbCmdCtx->userIO()->putString(OD_T("Test passed"));
      }
    }
  }
}

#ifndef OD_T
#define OD_T(x) x
#endif
#define TEST_SRC_DIR OD_T("e:\\Test")
#if defined(WIN32)
class DwgLoader
{
public:
  DwgLoader(OdDbHostAppServices* pHostApp, OdString srcDir = TEST_SRC_DIR)
    : _pHostApp(pHostApp)
    , _srcDir(srcDir)
    , _num(0)
  {
  }
  OdDbHostAppServices* _pHostApp;
  OdString  _srcDir;
  int       _num;

  void run(const OdString& dir = OD_T(""))
  {
    OdString oldSrcDir = _srcDir;
    if (dir.getLength() > 0)
    {
      _srcDir += OD_T("\\");
      _srcDir += dir;
    }

    ::SetCurrentDirectory(_srcDir);
    ODA_TRACE1("\n>>> Start Folder :  %ls <<<\n", _srcDir.c_str());

    WIN32_FIND_DATA findFileData;
    memset(&findFileData, 0, sizeof(WIN32_FIND_DATA));
    HANDLE hFind = FindFirstFile(OD_T("*.*"), &findFileData);
    BOOL bFind = (hFind != INVALID_HANDLE_VALUE);
    while (bFind)
    {
      if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
      {
        if (OdString(findFileData.cFileName).right(4).makeUpper() == OD_T(".DWG"))
        {
          ++_num;
          ODA_TRACE2("\n(%4d) Try converting  :  %ls", _num, findFileData.cFileName);
          try
          {
            OdDbDatabasePtr pDb = _pHostApp->readFile(findFileData.cFileName);
            ODA_TRACE0(" -> OK\n");
          }
          catch(...)
          {
            ODA_TRACE0(" -> ERROR\n");
          }
        }
      }
      else
      {
        OdString nextDir = findFileData.cFileName;
        if ( nextDir.getLength() > 0 
          && nextDir[0] != '.'
          && nextDir[0] != '\\' )
        {
          run(findFileData.cFileName);
        }
      }
      bFind = ::FindNextFile(hFind, &findFileData);
    }

    ODA_TRACE1("\n>>> End Folder   :  %ls <<<\n", _srcDir.c_str());
    _srcDir = oldSrcDir;
    ::SetCurrentDirectory(_srcDir);
  }
};
#endif

void _testBatchRead_func(OdEdCommandContext* pCmdCtx)
{
#if defined(WIN32)
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();

  DwgLoader loader(pDb->appServices());
  loader.run();
  return;
#endif
}



//////////////////////////////////////////////////////////////////////////
void _PEDIT_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();

  OdDbSelectionSetPtr pSSet = pIO->select( OD_T("Select 2dPolyline:") );

  OdDbSelectionSetIteratorPtr pIter = pSSet->newIterator();

  while ( !pIter->done() )
  {
    OdDbObjectId objId = pIter->objectId();
    OdDbEntityPtr pEnt = objId.openObject(OdDb::kForWrite);
    OdDb2dPolylinePtr p2dPoly = OdDb2dPolyline::cast(pEnt);
    OdDb3dPolylinePtr p3dPoly = OdDb3dPolyline::cast(pEnt);
    OdDbPolygonMeshPtr pMesh  = OdDbPolygonMesh::cast(pEnt);
	  OdDbPolylinePtr pPoly	  = OdDbPolyline::cast(pEnt);

    if (!p2dPoly.isNull())
    {
      OdUInt32 nKeyword = pIO->getKeyword(L"Enter Fit Type [None/Curve/Spline]:", L"None Curve Spline");
      switch (nKeyword) {
      case 0:
        p2dPoly->straighten();
        break;
      case 1:
        p2dPoly->curveFit();
        break;
      case 2:
        p2dPoly->splineFit();
        break;
      }
    }
	  else if( !pPoly.isNull() )
	  {
		  OdDb2dPolylinePtr p2DpLine = OdDb2dPolyline::createObject();
		  pPoly->convertTo( p2DpLine.get(), false );
      OdUInt32 nKeyword = pIO->getKeyword(L"Enter Fit Type [Curve/Spline]:", L"Curve Spline");
      switch (nKeyword) {
      case 0:
        p2DpLine->curveFit();
        break;
      case 1:
        p2DpLine->splineFit();
        break;
      }
		  OdDbBlockTableRecordPtr pMs = pDb->getModelSpaceId().safeOpenObject(OdDb::kForWrite);
		  pMs->appendOdDbEntity(p2DpLine);
		  pPoly->erase();
	  }
    else if (!p3dPoly.isNull())
    {
      OdUInt32 nKeyword = pIO->getKeyword(L"Enter Fit Type [None/Spline]:", L"None Spline");
      switch (nKeyword) {
      case 0:
        p3dPoly->straighten();
        break;
      case 1:
        p3dPoly->splineFit();
        break;
      }
    }
    else if (!pMesh.isNull())
    {
      // pMesh->surfaceFit(OdDb::kQuadSurfaceMesh, 7, 7);
      pMesh->surfaceFit();
    }

    pIter->next();
  }
}

//////////////////////////////////////////////////////////////////////////////
// 'reduce' command removes from database all the entities and blocks that 
// do not refer to selected, and not referred by them, directly or indirectly.
// It is used to isolate problems in large drawings.

static void appendAllHardReferencesTo( OdDbDatabase* db, const OdDbObjectId& id, OdDbObjectIdArray& refs )
{
  class RefFiler : public OdStaticRxObject<OdIdFiler>
  {
    OdDbObjectId m_id;
    bool m_bFound;
    OdDbDatabase* m_pDb;
  public:
    bool isFound() const { return m_bFound;}
    void reset() { m_bFound = false; }
    RefFiler(const OdDbObjectId& id, OdDbDatabase* db) : m_id(id), m_bFound(false), m_pDb(db){}
    virtual OdDbDatabase* database() const
    {
      return m_pDb;
    }
    virtual void wrHardOwnershipId(const OdDbObjectId& value)
    {
      if ( value == m_id )
        m_bFound = true;
    }
    virtual void wrSoftOwnershipId(const OdDbObjectId&){}
    virtual void wrSoftPointerId(const OdDbObjectId&){}
    virtual void wrHardPointerId(const OdDbObjectId& value)
    {
      if ( value == m_id ) 
        m_bFound = true;
    }
  }
  filer(id,db);

  OdDbBlockTablePtr pBlocks = db->getBlockTableId().safeOpenObject();
  for (OdDbSymbolTableIteratorPtr si = pBlocks->newIterator();!si->done(); si->step())
  {
    OdDbBlockTableRecordPtr pBlock = si->getRecord();
    for (OdDbObjectIteratorPtr oi = pBlock->newIterator(); !oi->done();oi->step())
    {
      oi->entity()->dwgOut(&filer);
      if (filer.isFound())
      {
        refs.append(oi->objectId());
        filer.reset();
      }
    }
  }
}

static void appendAllHardReferences( OdDbObject* obj, OdDbObjectIdArray& refs )
{
  if (!obj)
    return;
  class RefFiler : public OdStaticRxObject<OdIdFiler>
  {
    OdDbObjectIdArray& m_refs;
    OdDbDatabase* m_pDb;
  public:
    virtual OdDbDatabase* database() const
    {
      return m_pDb;
    }
    RefFiler(OdDbObjectIdArray& refs, OdDbDatabase* db) : m_refs(refs),m_pDb(db){}
    virtual void wrHardOwnershipId(const OdDbObjectId& value)
    {
      m_refs.append(value);
    }
    virtual void wrSoftOwnershipId(const OdDbObjectId&){}
    virtual void wrSoftPointerId(const OdDbObjectId&){}
    virtual void wrHardPointerId(const OdDbObjectId& value)
    {
      m_refs.append(value);
    }
  }
  filer(refs, obj->database());
  obj->dwgOut(&filer);
}

void _reduce_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbSelectionSetPtr sset = pDbCmdCtx->dbUserIO()->select(OdString::kEmpty, OdEd::kSelAllowInactSpaces);
  if (!sset->numEntities())
    return;
  bool preserve_VP = (0 == pDbCmdCtx->userIO()->getKeyword(OD_T("Preserve all viewports?"), OD_T("Yes No")));
  bool deepReduce = (0 == pDbCmdCtx->userIO()->getKeyword(OD_T("Deep reduce?"), OD_T("Yes No"), 0));
  // all objects referencing && referenced by selected ones
  OdDbObjectIdArray markedObjects;

  // collect all what we are referencing
  if (deepReduce)
  {
    for (OdDbSelectionSetIteratorPtr it = sset->newIterator(); !it->done(); it->next())
      markedObjects.append(it->objectId());
    unsigned int size = 0;
    for(;;)
    {
      OdDbObjectIdArray newObjs;
      for (unsigned i = size; i < markedObjects.size(); ++i)
      {
        OdDbObjectIdArray tmp;
        appendAllHardReferences(markedObjects[i].openObject(),tmp);
        for (unsigned j = 0; j < tmp.size(); ++j)
        {
          if (!markedObjects.contains(tmp[j]))
            newObjs.append(tmp[j]);
        }
      }
      
      if (newObjs.size() != 0)
      {
        size = markedObjects.size();
        markedObjects.append(newObjs);
      }
      else
        break;
    }
  }
  else
  {
    for (OdDbSelectionSetIteratorPtr it = sset->newIterator(); !it->done(); it->next())
    {
      OdDbObjectPtr pObj = it->objectId().openObject();
      appendAllHardReferences(pObj,markedObjects);
    }
  }

    // blocks containing selected objects
  OdDbObjectIdArray blocksToKeep;
  for (OdDbSelectionSetIteratorPtr it = sset->newIterator(); !it->done(); it->next())
  {
    OdDbObjectPtr pObj = it->objectId().openObject();
    // reduce works only for entities
    if (!pObj->isKindOf(OdDbEntity::desc())) continue;
    OdDbBlockTableRecordPtr pBlock = pObj->ownerId().safeOpenObject();
    blocksToKeep.append(pBlock->objectId());
    OdDbObjectIdArray objs;
    pBlock->getBlockReferenceIds( objs, false );
    markedObjects.append(objs);
    for (unsigned i = 0; i < objs.size(); ++i)
    {
      blocksToKeep.append(objs[i].safeOpenObject()->ownerId());
    }
    appendAllHardReferencesTo(pDb,pBlock->objectId(),markedObjects);
  }
  // append collected id's to selection
  for (unsigned i = 0; i < markedObjects.size(); ++i)
    sset->append(markedObjects[i]);

  try
  {
    pDb->startUndoRecord();
    pDb->startTransaction();
    // erase non-referenced blocks
    OdDbBlockTablePtr pBlocks = pDb->getBlockTableId().safeOpenObject(OdDb::kForWrite);
    OdDbSymbolTableIteratorPtr si = pBlocks->newIterator();
    for ( ;!si->done(); si->step())
    {
      if ( !blocksToKeep.contains(si->getRecordId()) && !sset->isMember(si->getRecordId()) )
      {
        OdDbBlockTableRecordPtr pBlock = si->getRecord(OdDb::kForWrite);
        if ( pBlock->isLayout() ) continue;
        pBlock->erase();
      }
    }
    pBlocks->downgradeOpen();
    si = pBlocks->newIterator();
    // erase non-selected objects in remaining blocks
    for ( ;!si->done(); si->step())
    {
      OdDbBlockTableRecordPtr pBlock = si->getRecord(OdDb::kForWrite);
      // if block is in sset => reference in selected
      if ( pBlock.isNull() || sset->isMember(pBlock->objectId()) ) continue; 
      OdDbObjectPtr pSortents = pBlock->getSortentsTable(false);
      if (pSortents.get())
        pSortents->erase();
      OdDbObjectIteratorPtr pIter = pBlock->newIterator();
      while (!pIter->done())
      {
        OdDbObjectPtr obj = pIter->objectId().openObject(OdDb::kForWrite);
        pIter->step();
        if (!sset->isMember(obj->objectId()))
        {
          if (obj->isKindOf(OdDbViewport::desc()))
          {
            if (preserve_VP) 
              continue;
            // keep active viewport in layout
            OdDbLayoutPtr l = pBlock->getLayoutId().openObject();
            if (!l.isNull() && l->activeViewportId() == obj->objectId())
              continue;
          }
          obj->erase();
        }
      }
    }
    pDb->endTransaction();
    sset->clear();
  }
  catch(const OdError& e)
  {
    pDb->appServices()->warning(e.description());
    pDb->abortTransaction();
    sset->clear();
  }
}
// end of 'reduce' functionality
//////////////////////////////////////////////////////////////////////////////
void _addSection_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdGePoint3dArray pp;
  pp.append(pDbCmdCtx->dbUserIO()->getPoint(L"Pick first point"));
  pp.append(pDbCmdCtx->dbUserIO()->getPoint(L"Pick second point"));
  OdDbSectionPtr s = OdDbSection::createObject(pp, OdGeVector3d::kZAxis);
  OdDbBlockTableRecordPtr btr = pDb->getActiveLayoutBTRId().safeOpenObject(OdDb::kForWrite);
  btr->appendOdDbEntity(s);
}

void _toggleLiveSection_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();
  OdDbSelectionSetPtr pSSet = pIO->select(OD_T("Select objects:"),
    OdEd::kSelAllowObjects |
    OdEd::kSelAllowSubents |
    OdEd::kSelLeaveHighlighted);
  if (pSSet->numEntities() == 0)
    return;

  OdDbSelectionSetIteratorPtr pIter = pSSet->newIterator();
  for (OdDbSelectionSetIteratorPtr i = pIter; !i->done(); i->next())
  {
    OdDbSectionPtr pSection = i->objectId().safeOpenObject(OdDb::kForWrite);
    try
    {
      pSection->enableLiveSection(!pSection->isLiveSectionEnabled());
    }
    catch (...)
    {
      ODA_ASSERT(!"_toggleLiveSection_func failed!!");
    }
  }
}


void _renameLayout_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbSelectionSetPtr sset = pDbCmdCtx->dbUserIO()->select(L"Select layout", OdEd::kSelAllowObjects);
  for (OdDbSelectionSetIteratorPtr it = sset->newIterator(); !it->done(); it->next())
  {
    OdDbLayoutPtr l = it->objectId().safeOpenObject(OdDb::kForWrite);
    if (l.isNull())
      continue;
    try
    {
      l->setLayoutName(pDbCmdCtx->userIO()->getString(L"Enter new layout name:", OdEd::kInpDefault, l->getLayoutName()));
    }
    catch (const OdError& e)
    {
    	pDbCmdCtx->userIO()->putString(e.description());
    }
  }
}

#if 0 // Moved to ExCommands
void _TransformByMirr_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();
  OdDbSelectionSetPtr pSSet = pIO->select(OD_T("Select objects:"), 
    OdEd::kSelAllowObjects | 
    OdEd::kSelAllowSubents |
    OdEd::kSelLeaveHighlighted);
  if( pSSet->numEntities() == 0)
    return;

  OdGePoint3d ptBasePoint = pIO->getPoint(OD_T("Specify base point:"));
  OdGePoint3d ptEndPoint  = pIO->getPoint(OD_T("Specify second point:"));

  OdGeLine3d mirrorLine(ptBasePoint, ptEndPoint);
  OdGeVector3d v = ptEndPoint - ptBasePoint;
  v.normalize();
  v.set(v.y, -v.x, 0);
  OdGePlane pln(ptBasePoint, v);
  OdGeMatrix3d trans = OdGeMatrix3d::mirroring(pln);

  if (ptBasePoint.isEqualTo(ptEndPoint))
    return;

  OdDbSelectionSetIteratorPtr pIter = pSSet->newIterator();
  for ( OdDbSelectionSetIteratorPtr i = pIter; !i->done(); i->next() )
  {
    OdDbEntityPtr pEntity = i->objectId().safeOpenObject(OdDb::kForWrite);
    try
    {
      pEntity->transformBy( trans );
    }
    catch(...)
    {
      ODA_ASSERT(!"Mirror_func failed!!");
    }
  }
}
void _TransformCopyMirr_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();
  OdDbSelectionSetPtr pSSet = pIO->select(OD_T("Select objects:"), 
    OdEd::kSelAllowObjects | 
    OdEd::kSelAllowSubents |
    OdEd::kSelLeaveHighlighted);
  if( pSSet->numEntities() == 0)
    return;

  OdGePoint3d ptBasePoint = pIO->getPoint(OD_T("Specify base point:"));
  OdGePoint3d ptEndPoint  = pIO->getPoint(OD_T("Specify second point:"));

  OdGeLine3d mirrorLine(ptBasePoint, ptEndPoint);
  OdGeVector3d v = ptEndPoint - ptBasePoint;
  v.normalize();
  v.set(v.y, -v.x, 0);
  OdGePlane pln(ptBasePoint, v);
  OdGeMatrix3d trans = OdGeMatrix3d::mirroring(pln);

  if (ptBasePoint.isEqualTo(ptEndPoint))
    return;

  OdDbSelectionSetIteratorPtr pIter = pSSet->newIterator();
  for ( OdDbSelectionSetIteratorPtr i = pIter; !i->done(); i->next() )
  {
    OdDbEntityPtr pEntity = i->objectId().safeOpenObject(OdDb::kForWrite);
    try
    {
      OdDbEntityPtr newEntity;
      if ( eOk == pEntity->getTransformedCopy(trans, newEntity))
      {
        OdDbBlockTableRecordPtr ms = pDb->getModelSpaceId().safeOpenObject(OdDb::kForWrite);
        ms->appendOdDbEntity(newEntity);
      }
    }
    catch(...)
    {
      ODA_ASSERT(!"Mirror_func failed!!");
    }
  }
}
#endif

void _ADJUSTTEXT_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  pDb->startUndoRecord();
  OdDbBlockTablePtr pBlockTbl = pDb->getBlockTableId().openObject();
  OdDbSymbolTableIteratorPtr  block_it(pBlockTbl->newIterator());
  while (!block_it->done())
  {
    OdDbBlockTableRecordPtr pBlock(block_it->getRecordId().safeOpenObject());
    block_it->step();
    if (!pBlock->isAnonymous())
    {
      OdDbObjectIteratorPtr pEntIter;
      for(pEntIter = pBlock->newIterator(); !pEntIter->done(); pEntIter->step())
      {
        OdDbEntityPtr pEnt = pEntIter->entity(OdDb::kForWrite);
        OdDbText* pText = (OdDbText*)pEnt->queryX(OdDbText::desc());
        if(pText)
        {
          pText->adjustAlignment();
          pText->release();
        } else 
        {
          OdDbBlockReferencePtr pBlRef = OdDbBlockReference::cast(pEnt);
          if ( !pBlRef.isNull() )
          {
            OdDbObjectIteratorPtr attrIter =  pBlRef->attributeIterator();
            while (!attrIter->done())
            {
              OdDbObjectId id = attrIter->objectId();
              OdDbTextPtr pTextAttr = id.openObject(OdDb::kForWrite);
              pTextAttr->adjustAlignment();
              attrIter->step();
            }
          }
        }
      }
    }
  }
}

double maxColWidth(OdDbTablePtr mTable, OdUInt32 col)
{
  OdUInt32 rows = mTable->numRows();
  double maxW = 0.0;
  for (OdUInt32 row = 0; row < rows; row++)
  {
    OdString str = mTable->textString(row, col);
    if ( !str.isEmpty() )
    { 
      OdDbMTextPtr mtext;
      mtext = OdDbMText::createObject();
      OdInt32 curCont = 0;
      mtext->setTextStyle(mTable->textStyle(row, col, -1));
      mtext->setTextHeight(mTable->textHeight(row, col, -1));
      mtext->setRotation(mTable->rotation(row, col, curCont));

      mtext->setContents(mTable->textString(row, col));

      maxW = odmax(mtext->actualWidth(), maxW);
    }
  }
  return maxW;
}

void _CreateLinkedTable_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();

  OdRxModulePtr module = ::odrxDynamicLinker()->loadModule(OdDataLinkUpdate);

  OdDbTextStyleTableRecordPtr ts = pDb->getTextStyleStandardId().safeOpenObject(OdDb::kForWrite);
  ts->setFont(L"Arial", false, false, 0, 0);
  OdDbDataLinkManager* man = odDbGetDataLinkManager(pDb);
  int cnt = man->dataLinkCount();

  OdGePoint3d pos = pIO->getPoint(OD_T("Specify table position:"));

  OdString fname = pIO->getFilePath(OD_T("Enter file name to link:"),
    OdEd::kGfpForOpen,
    OD_T("Select Excel file to insert"),
    OD_T("xls"),
    OdString::kEmpty,
    OD_T("XLS table (*.xls,*.xlsx)|*.xlsx;*.xls||"));

  OdString connectStr = fname + "!Sheet1";
  OdString nameLink;
  nameLink.format(OD_T("DataLinkDemo%d"), cnt);

//  OdDbTablePtr table = odDbCreateDataLinkAndTable(pDb, nameLink, OdString(OD_T("ODA created")), connectStr);


//   OdDb::UpdateDirection dirUpdate = OdDb::kUpdateDirectionSourceToData;
//   table->updateDataLink(dirUpdate, (OdDb::UpdateOption) 0);

  OdDbObjectId dlId = man->createDataLink(OD_T("AcExcel"), nameLink, OdString(OD_T("Teigha created")), connectStr);

  // OdDbDataLinkPtr dtLink = man->getDataLink(OdString(OD_T("DataLinkDemo")), OdDb::kForWrite);
  if (!dlId.isNull())
  {
//    dtLink->update(OdDb::kUpdateDirectionSourceToData, OdDb::kUpdateOptionAllowSourceUpdate);

    OdDbTablePtr table = OdDbTable::createObject();
    table->setSize(2, 2);
    table->setPosition(pos);
    table->setDataLink(1, 1, dlId, true);
  
    table->setPosition(pos);

    OdDbBlockTableRecordPtr ms = pDbCmdCtx->database()->getModelSpaceId().safeOpenObject(OdDb::kForWrite);
    ms->appendOdDbEntity(table);

    for ( OdUInt32 c = 0; c < table->numColumns(); c++ )
    { 
      double w = maxColWidth(table, c);
      if ( OdNonZero(w) )
      { 
        w += table->horzCellMargin() * 2.0;
        table->setColumnWidth(c, w*1.05);
      }
    }
  }
}

void _SubSelectTable_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();

  OdDbSelectionSetPtr sset = pIO->select(L"Select table:", OdEd::kSelDefault, 0, OdString::kEmpty);
  if (sset->numEntities() != 0)
  {
    OdDbTablePtr pTable;
    for (OdDbSelectionSetIteratorPtr it = sset->newIterator(); !it->done(); it->next())
    {
      pTable = it->objectId().safeOpenObject(OdDb::kForWrite);
      if (!pTable.isNull())
      {
        pTable->clearSubSelection();
        break;
      }
    }
    OdGePoint3d pnt = pIO->getPoint(OD_T("\nPick point in cell:"));
    OdDbViewportTableRecordPtr viewPort = pDb->activeViewportId().safeOpenObject();

    OdInt32 row = -1;
    OdInt32 col = -1;
    OdInt32 contentIndex = 0;
    OdDb::TableHitItem nItem = OdDb::kTableHitNone;

    OdString tmp;
    tmp.format(OD_T("\n Point (%f, %f, %f)"), pnt.x, pnt.y, pnt.z);
    pIO->putString(tmp);

    // OdDbTablePtr pTable = id.safeOpenObject();
    bool res = pTable->hitTest(pnt, viewPort->viewDirection(), 0, 0, row, col, contentIndex, nItem);

    if (res == true)
    {
      tmp.format(OD_T("\nSuccess! row=%d, col=%d, content=%d item=%d"), row, col, contentIndex, nItem);
      pIO->putString(tmp);
    }
    else
    {
      return;
    }
    pnt = pIO->getPoint(OD_T("\nPick point in cell:"));
    OdInt32 row1 = -1;
    OdInt32 col1 = -1;
    res = pTable->hitTest(pnt, viewPort->viewDirection(), 0, 0, row1, col1, contentIndex, nItem);
    if (res)
    {
      tmp.format(OD_T("\nSuccess! row=%d, col=%d, content=%d item=%d"), row1, col1, contentIndex, nItem);
      pIO->putString(tmp);

      pTable->setSubSelection(row, row1, col, col1);
    }
  }

}

void _RECOMPUTETABLE_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();

  pDb->startUndoRecord();

  OdDbBlockTablePtr pBlockTbl = pDb->getBlockTableId().openObject();
  OdDbSymbolTableIteratorPtr  block_it(pBlockTbl->newIterator());

  bool bUpdateLink = (0 == pIO->getKeyword(OD_T("Update Data links [Yes/No]?"), OD_T("Yes No")));

  OdDb::UpdateDirection dirUpdate = OdDb::kUpdateDirectionSourceToData;
  if ( bUpdateLink )
  {
    OdUInt32 nKeyword = pIO->getKeyword(OD_T("Download changes from source file or Upload Users changes [Download/Upload]:"), OD_T("Download Upload"));
    if ( nKeyword != 0 )
      dirUpdate = OdDb::kUpdateDirectionDataToSource;
    ::odrxDynamicLinker()->loadModule(OdDataLinkUpdate);
  }
  while (!block_it->done())
  {
    OdDbBlockTableRecordPtr pBlock(block_it->getRecordId().safeOpenObject());
    block_it->step();
    if (!pBlock->isAnonymous())
    {
      OdDbObjectIteratorPtr pEntIter;
      for(pEntIter = pBlock->newIterator(); !pEntIter->done(); pEntIter->step())
      {
        OdDbEntityPtr pEnt = pEntIter->entity(OdDb::kForWrite);
        OdDbTable* pTable = (OdDbTable*)pEnt->queryX(OdDbTable::desc());
        if(pTable)
        {
          if ( bUpdateLink )
            pTable->updateDataLink(dirUpdate, (OdDb::UpdateOption) 0);
          pTable->recomputeTableBlock();
          pTable->release();
        } 
      }
    }
  }
}

void _ISEMR_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  pDbCmdCtx->dbUserIO()->putString(pDbCmdCtx->database()->isEMR() ? L"isEMR() = true" : L"isEMR() = false");
}

void _VBAEXTRACT_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDictionaryPtr rootDic = pDbCmdCtx->database()->getNamedObjectsDictionaryId().safeOpenObject();
  if (rootDic->has(L"ACAD_VBA"))
  {
    OdDbDictionaryPtr vbaDic = rootDic->getAt(L"ACAD_VBA").safeOpenObject();
    OdDbVbaProjectPtr vbaProj = vbaDic->getAt(L"VBAProject").safeOpenObject();
    if (!vbaProj.isNull())
    {
      OdBinaryData bin;
      vbaProj->getVbaProject(bin);
      OdString path = pDbCmdCtx->userIO()->getFilePath("Select path to save vba project contents",OdEd::kGfpForSave);
      OdStreamBufPtr str = odrxSystemServices()->createFile(path, Oda::kFileWrite, Oda::kShareDenyNo, Oda::kCreateAlways);
      str->putBytes(bin.asArrayPtr(), bin.size());
    }
  }
}

#include "DbRotatedDimension.h"

struct DimTracker : public OdEdPointTracker
{
  OdDbRotatedDimensionPtr pNewDim;
  virtual int addDrawables(OdGsView* pView)
  {
    pView->add(pNewDim, 0);
    return 1;
  }
  virtual void removeDrawables(OdGsView* pView)
  {
    pView->erase(pNewDim);
  }
  virtual void setValue(const OdGePoint3d& value)
  {
    pNewDim->setDimLinePoint(value);
    pNewDim->recomputeDimBlock();
  }
};
#include <DbAnnotativeObjectPE.h>

void _TESTDIM_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();

  OdDbBlockTableRecordPtr pSpace = pDb->getActiveLayoutBTRId().safeOpenObject(OdDb::kForWrite);

  OdStaticRxObject<DimTracker> tracker;
  tracker.pNewDim = OdDbRotatedDimension::createObject();
  tracker.pNewDim->setDatabaseDefaults(pDb);

  pSpace->appendOdDbEntity(tracker.pNewDim);
  tracker.pNewDim->recomputeDimBlock();
  OdDbAnnotativeObjectPEPtr(tracker.pNewDim)->setAnnotative(tracker.pNewDim, true);

  tracker.pNewDim->setXLine1Point( pIO->getPoint(OD_T("Specify first extension line origin:")) );

  tracker.pNewDim->setXLine2Point( pIO->getPoint(OD_T("Specify second extension line origin:"), OdEd::kGptRubberBand) );

  pIO->getPoint(OD_T("Specify dimension line location:"), 0, 0, OdString::kEmpty, &tracker);

  tracker.pNewDim->recordGraphicsModified(false);
}

void _DIMINSPECT_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();
  class DimSelectTracker : public OdStaticRxObject<OdEdSSetTracker>
  {
    virtual bool append(const OdDbObjectId& entId, const OdDbSelectionMethod* )
    {
      return entId.safeOpenObject()->isKindOf(OdDbDimension::desc());
    }
    virtual bool remove(const OdDbObjectId& , const OdDbSelectionMethod* ){return true;}
    virtual int addDrawables(OdGsView*){return 0;}
    virtual void removeDrawables(OdGsView*){}
  }
  tr;
  if ( 0 == pIO->getKeyword( L"Add inspection data or [Remove] <Add>:", L"Add Remove", 0))
  {
    OdDbSelectionSetPtr sset = pIO->select(L"Select dimensions:",OdEd::kSelDefault,0,OdString::kEmpty,&tr);
    if (sset->numEntities()!=0)
    {
      int frame = pIO->getKeyword(L"Enter shape option [Round/Angular/None] <Round>:", L"Round Angular None", 0);
      OdString label = pIO->getString( L"Enter label data or <None>:" );
      OdString rate = pIO->getString(L"Enter inspection rate <100%>:", OdEd::kInpDefault, L"100%");
      for (OdDbSelectionSetIteratorPtr it = sset->newIterator(); !it->done(); it->next())
      {
        OdDbDimensionPtr dim = it->objectId().safeOpenObject(OdDb::kForWrite);
        dim->setInspection(true);
        dim->setInspectionFrame( (1 << frame) | (label.isEmpty() ? 0 : 0x10) | (rate.isEmpty() ? 0 : 0x20));
        dim->setInspectionLabel(label);
        dim->setInspectionRate(rate);
      }
    }
  }
  else
  {
    OdDbSelectionSetPtr sset = pIO->select(L"Select dimensions:",OdEd::kSelDefault,0,OdString::kEmpty,&tr);
    for (OdDbSelectionSetIteratorPtr it = sset->newIterator(); !it->done(); it->next())
    {
      OdDbDimensionPtr dim = it->objectId().safeOpenObject(OdDb::kForWrite);
      dim->setInspection(false);
    }
  }
}

void _DIMJOGLINE_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();
  class DimSelectTracker : public OdStaticRxObject<OdEdSSetTracker>
  {
    virtual bool append(const OdDbObjectId& entId, const OdDbSelectionMethod* )
    {
      return entId.safeOpenObject()->isKindOf(OdDbDimension::desc());
    }
    virtual bool remove(const OdDbObjectId& , const OdDbSelectionMethod* ){return true;}
    virtual int addDrawables(OdGsView*){return 0;}
    virtual void removeDrawables(OdGsView*){}
  }
  tr;
  try
  {
    OdDbSelectionSetPtr sset = pIO->select(L"Select dimension to add jog or [Remove]:",OdEd::kSelSingleEntity, 0, L"Remove", &tr);
    for (OdDbSelectionSetIteratorPtr it = sset->newIterator(); !it->done(); it->next())
    {
      OdGePoint3d p = pIO->getPoint(L"Specify jog location (or press ENTER):");
      OdDbDimensionPtr dim = it->objectId().safeOpenObject(OdDb::kForWrite);
      if (dim->isKindOf(OdDbRotatedDimension::desc()))
      {
        static_cast<OdDbRotatedDimension*>(dim.get())->setJogSymbolOn(true);
        static_cast<OdDbRotatedDimension*>(dim.get())->setJogSymbolPosition(p);
      }
      else if (dim->isKindOf(OdDbAlignedDimension::desc()))
      {
        static_cast<OdDbAlignedDimension*>(dim.get())->setJogSymbolOn(false);
        static_cast<OdDbAlignedDimension*>(dim.get())->setJogSymbolPosition(p);
      }
    }
  }
  catch( const OdEdKeyword& kwd )
  {
    if (kwd.keywordIndex() == 0)
    {
      OdDbSelectionSetPtr sset = pIO->select(L"Select jog to remove:", OdEd::kSelSingleEntity, 0, OdString::kEmpty, &tr);
      for (OdDbSelectionSetIteratorPtr it = sset->newIterator(); !it->done(); it->next())
      {
        OdDbDimensionPtr dim = it->objectId().safeOpenObject(OdDb::kForWrite);
        if (dim->isKindOf(OdDbRotatedDimension::desc()))
        {
          static_cast<OdDbRotatedDimension*>(dim.get())->setJogSymbolOn(false);
        }
        else if (dim->isKindOf(OdDbAlignedDimension::desc()))
        {
          static_cast<OdDbAlignedDimension*>(dim.get())->setJogSymbolOn(false);
        }
      }
    }
  }
}

int fragmentFn(OdDbMTextFragment *frag, void *out)
{
  OdDbUserIO* pIO = (OdDbUserIO*)out;
  OdString msg;
  msg.format(OD_T("Text - <%s>"), frag->text.c_str(), frag->fontname.c_str());
  pIO->putString(msg);
  // pIO->putString(frag->text);
  msg.format(OD_T("x - %-10.4f y - %-10.4f"), frag->location.x, frag->location.y);
  pIO->putString(msg);
  msg.format(OD_T("Property - %s %s"), frag->italic ? OD_T("Ital") : OD_T(""), frag->bold ? OD_T("Bold") : OD_T(""));
  pIO->putString(msg);
  msg.format(OD_T("Font - %s FontFile - %s"), frag->font.c_str(), frag->fontname.c_str());
  pIO->putString(msg);
  return 1;
}

void _DUMPMTEXTFRAGMENTS_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();
  for ( OdDbSelectionSetIteratorPtr pIter = pIO->select()->newIterator();
    !pIter->done(); pIter->next() )
  {
    OdDbEntityPtr pEnt = OdDbEntity::cast( pIter->objectId().openObject() );
    if ( pEnt.isNull() )
      continue;
    OdDbMTextPtr mText = OdDbMText::cast(pEnt);
    if ( mText.isNull() )
      continue;
    mText->explodeFragments(fragmentFn, pIO);
  }  
}

namespace ADS_TEST
{
  OdResBuf* assoc(OdResBufPtr& elist, short groupCode)
  {
    OdResBuf* od_Assoc = elist.get();
    if (od_Assoc == NULL)
      return NULL;
    while(od_Assoc != NULL)
    {
      if (od_Assoc->restype() == groupCode)
        return od_Assoc;
      od_Assoc = od_Assoc->next();
    }
    return NULL;
  }
}

void _ADS_TEST_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbObjectId id;
  OdString output;
  OdResBuf *tmp[3] = {NULL, NULL, NULL};
  for (;;)
  {
    id = oddbEntNext(id, pDb);
    if (!id.isValid())
      break;
    OdResBufPtr pRb = oddbEntGet(id);

    tmp[0] = ADS_TEST::assoc(pRb, 5);
    tmp[1] = ADS_TEST::assoc(pRb, 410);
    tmp[2] = ADS_TEST::assoc(pRb, 0);
    output.format(L"Handle - (5 . %s), Space - (410 . %s), Type - (0 .  %s)", tmp[0]->getHandle().ascii().c_str(),
      tmp[1]->getString().c_str(),
      tmp[2]->getString().c_str());
    pCmdCtx->userIO()->putString(output);
  }
}

void _SORTENTS_TEST_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdString s;
  OdDbBlockTableRecordPtr ms = pDbCmdCtx->database()->getModelSpaceId().safeOpenObject(OdDb::kForWrite);
  OdDbObjectIteratorPtr it = ms->newIterator();
  if (it->done())
    return;
  OdDbObjectId id1 = it->objectId();
  it->step();
  if (it->done())
    return;
  OdDbObjectId id2 = it->objectId();
  ms->getSortentsTable()->swapOrder(id1, id2);
  HandlePairsArray ha;
  ms->getSortentsTable()->getAbsoluteDrawOrder(ha);
  for (int i = 0; i < (int)ha.size(); ++i)
  {
    s.format( L"%" PRIX64W L",%" PRIX64W, (OdUInt64)ha[i].first, (OdUInt64)ha[i].second.getHandle());
    pCmdCtx->userIO()->putString(s);
  }
}

void _ANNOTATION_TEST_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();
  OdString s;
  for ( OdDbSelectionSetIteratorPtr pIter = pIO->select()->newIterator(); !pIter->done(); pIter->next() )
  {
    OdDbTextPtr pText = OdDbText::cast( pIter->objectId().openObject(OdDb::kForWrite) );
    if ( pText.isNull() )
      continue;
    OdGePoint3d ap = pText->alignmentPoint();
    pIO->putString(s.format(L"alignmentPoint() = {%g,%g,%g}", ap.x, ap.y, ap.z));
    pIO->putString(s.format(L"rotation()=%g",pText->rotation()));
    pIO->putString(s.format(L"height()=%g",pText->height()));
    pIO->putString(s.format(L"horizontalMode()=%d",(int)pText->horizontalMode()));
    pText->adjustAlignment(pDbCmdCtx->database());
    ap = pText->alignmentPoint();
    pIO->putString(s.format(L"alignmentPoint() = {%g,%g,%g}", ap.x, ap.y, ap.z));
  }  
}

void _ANNOTATION_ADD_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  for (OdDbSelectionSetIteratorPtr pIter = pDbCmdCtx->dbUserIO()->select()->newIterator(); !pIter->done(); pIter->next())
  {
    OdDbEntityPtr pEnt = pIter->objectId().openObject(OdDb::kForWrite);
    if (pEnt.isNull())
      continue;
    OdDbAnnotativeObjectPEPtr pA = OdDbAnnotativeObjectPE::cast(pEnt);
    if (pA.isNull())
      continue;
    pA->setAnnotative(pEnt, true);
    OdDbObjectContextInterfacePtr(pEnt)->addContext(pEnt, *pDbCmdCtx->database()->getCANNOSCALE());
  }
}

void _ANNORESET_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  for (OdDbSelectionSetIteratorPtr pIter = pDbCmdCtx->dbUserIO()->select()->newIterator(); !pIter->done(); pIter->next())
  {
    OdDbEntityPtr pEnt = pIter->objectId().openObject(OdDb::kForWrite);
    if (pEnt.isNull())
      continue;
    OdDbAnnotativeObjectPEPtr pA = OdDbAnnotativeObjectPE::cast(pEnt);
    if (pA.isNull())
      continue;
    pA->resetScaleDependentProperties(pEnt);
  }
}

#if defined(WIN32)
// extract file name from path
static OdString fileName(const OdString& path)
{
  OdChar drive[_MAX_DRIVE], dir[_MAX_DIR], ext[_MAX_EXT];
  OdString fname;
  ::_wsplitpath( path, drive, dir, fname.getBuffer(_MAX_FNAME), ext);
  fname.releaseBuffer();
  return fname;
}
class OdDbUtils
{
public:
  static TOOLKIT_EXPORT void appendAttributeToBlockRef(OdDbBlockReference *pBlkRef, OdDbObjectId insBlkId);
};
#endif
// insert file as block
void _InsertEx_func(OdEdCommandContext* pCmdCtx)
{
  #if defined(WIN32)
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbUserIOPtr pIO = pDbCmdCtx->userIO();

  OdString fname = pIO->getFilePath(OD_T("Enter file name to insert:"),
    OdEd::kGfpForOpen,
    OD_T("Select drawing to insert"),
    OD_T("dwg"),
    OdString::kEmpty,
    OD_T("DWG drawing (*.dwg)|*.dwg||"));

  OdDbObjectId blockId = pDb->insert(fileName(fname), pDb->appServices()->readFile(fname, false, true), false);
  // Create the new insert entity.
  OdDbBlockReferencePtr pdbBlkRef = OdDbBlockReference::createObject();
  pdbBlkRef->setDatabaseDefaults(pDb);
  pdbBlkRef->setBlockTableRecord(blockId);
  
  pdbBlkRef->setPosition(pIO->getPoint(L"Specify insertion point:"));
  double scale = pIO->getReal(L"Specify scale factor for XYZ axes <1>", OdEd::kInpDefault, 1.0);
  pdbBlkRef->setScaleFactors(OdGeScale3d(scale, scale, scale));
  pdbBlkRef->setRotation(pIO->getReal(L"Specify rotation angle <0>", OdEd::kInpDefault, 0.0));

  // here we should iterate through attribute definitions and add corresponding attributes
  OdDbUtils::appendAttributeToBlockRef(pdbBlkRef, blockId);

  // Add the entity to the parent block.
  OdDbBlockTableRecordPtr activeLayout = pDb->getActiveLayoutBTRId().safeOpenObject(OdDb::kForWrite);
  activeLayout->appendOdDbEntity(pdbBlkRef);
#endif
}

void _UPDATEALLFIELDS_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  int numFound = 0, numEvaluated = 0;
  OdResult res = oddbEvaluateFields(pDbCmdCtx->database(), OdDbField::kDemand, 0, 0, OdString::kEmpty, OdFd::kEvalRecursive, &numFound, &numEvaluated);
  if (eOk == res)
    pCmdCtx->userIO()->putString(OdString().format(L"%d fields found, %d fields evaluated", numFound, numEvaluated));
  else
    pCmdCtx->userIO()->putError(pDbCmdCtx->database()->appServices()->formatMessage(res));

}

#include "ApLongTransactions.h"
#include "DbLongTransaction.h"
#include "OdValue.h"

void _REFEDITEX_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbObjectId transId = odapLongTransactionManager()->currentLongTransactionFor(pDbCmdCtx->database());
  if (!transId.isNull())
  {
    pDbCmdCtx->dbUserIO()->putString(L"REFEDIT session is active, use REFCLOSE");
    return;
  }
  OdDbSelectionSetIteratorPtr it = pDbCmdCtx->dbUserIO()->select(L"Select reference",OdEd::kSelSingleEntity)->newIterator();
  if (it->done())
  {
    pDbCmdCtx->dbUserIO()->putString(L"Nothing selected");
    return;
  }
  OdDbBlockReferencePtr b = it->objectId().safeOpenObject();
  if (b.isNull())
  {
    pDbCmdCtx->dbUserIO()->putString(L"Selected object is not a block reference");
    return;
  }
  OdDbBlockTableRecordPtr btr = b->blockTableRecord().safeOpenObject();
  OdGeMatrix3d xform = b->blockTransform();
  OdDbObjectId blockId = b->objectId();
  b = 0; // close reference, to allow it to be opened in transaction checkout (to lock it)
  OdDbObjectIdArray ia;
  for (OdDbObjectIteratorPtr i = btr->newIterator(); !i->done(); i->step())
    ia.append(i->objectId());
  ;
  OdDbIdMappingPtr errrorMap = OdDbIdMapping::createObject();
  OdResult res = odapLongTransactionManager()->checkOut(transId, ia, pDbCmdCtx->database()->getActiveLayoutBTRId(), *errrorMap, blockId);
  if (res == eOk)
  {
    OdValuePtr v = OdValue::createObject();
    *v = blockId;
    pDbCmdCtx->setArbitraryData(L"REFEDIT_BLOCK_ID", v);
    pDbCmdCtx->dbUserIO()->putString(L"Use REFCLOSE to end reference editing session.");
    // transform workset objects by insert matrix
    pDbCmdCtx->database()->startTransaction();
    OdDbLongTransactionPtr lt = transId.safeOpenObject();
    for (OdDbLongTransWorkSetIteratorPtr wi = lt->newWorkSetIterator(); !wi->done(); wi->step())
    {
      OdDbEntityPtr(wi->objectId().openObject(OdDb::kForWrite))->transformBy(xform);
    }
    pDbCmdCtx->database()->endTransaction();
    ::odedRegCmds()->executeCommand(L"REGEN", pCmdCtx);
  }
  else
  {
    pDbCmdCtx->dbUserIO()->putString(L"REFEDIT failed. Reason: " + pDbCmdCtx->database()->appServices()->formatMessage(res));
  }
}

void _REFCLOSEEX_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbObjectId transId = odapLongTransactionManager()->currentLongTransactionFor(pDbCmdCtx->database());
  if (transId.isNull())
  {
    pDbCmdCtx->dbUserIO()->putString(L"No active REFEDIT session");
    return;
  }
  if (pDbCmdCtx->dbUserIO()->getKeyword(L"Enter option [Save/Discard reference changes] <Save>:", "Save Discard", 0) == 0)
  {
    OdDbIdMappingPtr errrorMap = OdDbIdMapping::createObject();
    // transform workset objects back to original state, to be cloned to block
    OdDbLongTransactionPtr lt = transId.safeOpenObject();
    OdValuePtr v = pDbCmdCtx->arbitraryData(L"REFEDIT_BLOCK_ID");
    OdDbBlockReferencePtr b = ((OdDbObjectId)*v).safeOpenObject(OdDb::kForWrite);
    OdDbObjectId xrefBlockId;
    if(lt->type() == OdDbLongTransaction::kXrefDb)
      xrefBlockId = lt->originBlock();
    OdGeMatrix3d xform = b->blockTransform();
    xform.invert();
    for (OdDbLongTransWorkSetIteratorPtr wi = lt->newWorkSetIterator(); !wi->done(); wi->step())
    {
      OdDbEntityPtr ent(wi->objectId().openObject(OdDb::kForWrite));
      if (!ent.isNull())
        ent->transformBy(xform);
    }
    OdResult res = odapLongTransactionManager()->checkIn(transId, *errrorMap);
    if (res != eOk)
    {
      pDbCmdCtx->dbUserIO()->putString(L"REFCLOSE failed. Reason: " + pDbCmdCtx->database()->appServices()->formatMessage(res));
    }
    b->highlight(false);
  }
  else
  {
    odapLongTransactionManager()->abortLongTransaction(transId);
  }
  pDbCmdCtx->setArbitraryData(L"REFEDIT_BLOCK_ID", 0);
  ::odedRegCmds()->executeCommand(L"REGEN", pCmdCtx);
}

void _REFSETEX_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbObjectId transId = odapLongTransactionManager()->currentLongTransactionFor(pDbCmdCtx->database());
  if (transId.isNull())
  {
    pDbCmdCtx->dbUserIO()->putString(L"No active REFEDIT session");
    return;
  }
  OdDbLongTransactionPtr lt = transId.safeOpenObject(OdDb::kForWrite);
  if (pDbCmdCtx->dbUserIO()->getKeyword(L"Enter an option [Add/Remove] <Add>:", "Add Remove", 0) == 0)
  {
    for (OdDbSelectionSetIteratorPtr it = pDbCmdCtx->dbUserIO()->select()->newIterator(); !it->done(); it->next())
    {
      if (!lt->workSetHas(it->objectId()))
      {
        OdResult res = lt->addToWorkSet(it->objectId());
        if (res != eOk)
          pDbCmdCtx->userIO()->putString(pDbCmdCtx->database()->appServices()->formatMessage(res));
      }
    }
  }
  else
  {
    for (OdDbSelectionSetIteratorPtr it = pDbCmdCtx->dbUserIO()->select()->newIterator(); !it->done(); it->next())
    {
      if (lt->workSetHas(it->objectId()))
      {
        OdResult res = lt->removeFromWorkSet(it->objectId());
        if (res != eOk)
          pDbCmdCtx->userIO()->putString(pDbCmdCtx->database()->appServices()->formatMessage(res));
      }
    }
  }
}

#include "Reactors/DbgLongTransactionReactor.h"

static OdString formatIdPair(OdDbIdMappingIter* i)
{
  OdDbIdPair p; i->getMap(p);
  OdDbObjectPtr obj = p.key().openObject(OdDb::kForRead, true);
  OdRxClass* pClass = obj.isNull() ? 0 : obj->isA();
  OdString tmp;
  tmp.format(L"(%ls) => (%ls), <%ls> %ls, %ls", 
    p.key().getNonForwardedHandle().ascii().c_str(),
    p.value().getNonForwardedHandle().ascii().c_str(),
    pClass ? pClass->name().c_str() : L"?",
    p.isCloned()? L"isCloned = true" : L"isCloned = false", p.isOwnerXlated() ? L"isOwnerXlated = true" : L"isOwnerXlated = false");
  return tmp;
}

void OdDbgLongTransactionReactor::dumpTransaction(OdDbLongTransaction& tr)
{
  printMessage(OdString().format(L"Long transaction \"%ls\"", tr.getLongTransactionName().c_str()));
  printMessage(OdString().format(L"IdMap: deepCloneContext() = %d, duplicateRecordCloning() = %d", 
    tr.activeIdMap()->deepCloneContext(), tr.activeIdMap()->duplicateRecordCloning()));
  for (OdDbIdMappingIterPtr i = tr.activeIdMap()->newIterator(); !i->done(); i->next())
    printMessage(formatIdPair(i));
}

void OdDbgLongTransactionReactor::beginCheckOut(OdDbLongTransaction& tr, OdDbObjectIdArray& originList)
{
  printMessage(L"DrxDbgLongTransactionReactor::beginCheckOut");
  dumpTransaction(tr);
  printMessage(OdString().format(L"Original checkout list:"));
  for (unsigned int i = 0; i < originList.length(); ++i)
  {
    OdDbObjectId id = originList[i];
    OdDbObjectPtr pObj = id.openObject(OdDb::kForRead, true);
    printMessage(OdString().format(L"<%ls> (%ls)", pObj->isA()->name().c_str(), id.getHandle().ascii().c_str()));
  }
}

void OdDbgLongTransactionReactor::endCheckOut(OdDbLongTransaction& tr)
{ 
  printMessage(L"DrxDbgLongTransactionReactor::endCheckOut");
  dumpTransaction(tr);
}

void OdDbgLongTransactionReactor::beginCheckIn(OdDbLongTransaction& tr)
{
  printMessage(L"DrxDbgLongTransactionReactor::beginCheckIn");
  dumpTransaction(tr);
}

void OdDbgLongTransactionReactor::endCheckIn(OdDbLongTransaction& tr)
{
  printMessage(L"DrxDbgLongTransactionReactor::endCheckIn");
  dumpTransaction(tr);
}

void OdDbgLongTransactionReactor::abortLongTransaction (OdDbLongTransaction& tr)
{
  printMessage(L"DrxDbgLongTransactionReactor::abortLongTransaction");
  dumpTransaction(tr);
}
#if defined(WIN32) 
static OdString createUniqueName(OdDbDictionary* pDict, const TCHAR* prefix)
{
  OdString newName;
  int i = 1;
  do
  {
    newName.format(OD_T("%s%d"), prefix, i++);
  }
  while(pDict->has(newName));
  return newName;
}

static OdDbObjectId latestId;
#endif
void _NEWLAYOUT_func(OdEdCommandContext* pCmdCtx)
{
  #if defined(WIN32)
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbDictionaryPtr pDict = pDb->getLayoutDictionaryId().openObject();
  ODA_ASSERT(pDict.get());
  OdString newLayoutName = createUniqueName(pDict, OD_T("NewLayout"));
  OdDbObjectId newLayoutId = pDb->createLayout(newLayoutName);
  ODA_ASSERT(newLayoutId);
  latestId = newLayoutId;
#endif
}

void _SETLAYOUT_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbBlockTableRecordPtr pBlockTableRecord = pDb->getModelSpaceId().safeOpenObject();
  OdDbObjectIteratorPtr it = pBlockTableRecord->newIterator();
  for (;!it->done();it->step())
  {
    OdDbEntityPtr ent = it->entity();
    if (ent->isKindOf(OdDbBlockReference::desc()))
    {
      OdDbBlockReference* r = (OdDbBlockReference*)ent.get();
      OdRxObjectPtrArray a;
      r->explode(a);
      for (unsigned int i = 0; i < a.length(); ++i)
      {
        pCmdCtx->userIO()->putString(a[i]->isA()->name());
      }
    }
  }
}

void _SelectionFilterTest_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbUserIO* const pIO = pDbCmdCtx->dbUserIO();

  // select an entity to use for test.
  OdDbSelectionSetPtr pSS;
  try {
    pSS = pIO->select(OD_T("select object: "), OdEd::kSelSingleEntity);
  }
  catch (OdEdCancel) {
    return;
  }
  OdDbObjectIdArray const idEnts = pSS->objectIdArray();
  pSS.release();
  if (idEnts.size() != 1) {
    return;
  }
  OdDbObjectId const idEnt = idEnts.at(0);

  // confirm the application name registerd.
  OdString const strAppName = OD_T("DENKI");
  OdDbDatabase * const pDb = idEnt.database();
  OdDbRegAppTablePtr pRegAppTable = pDb->getRegAppTableId().safeOpenObject();
  if (!pRegAppTable->has(strAppName)) {
    pRegAppTable->upgradeOpen();
    OdDbRegAppTableRecordPtr pRegAppTableRecord;
    pRegAppTableRecord = OdDbRegAppTableRecord::createObject();
    pRegAppTableRecord->setName(strAppName);
    pRegAppTable->add(pRegAppTableRecord);
  }
  pRegAppTable.release();

  // add xdata.
  OdResBufPtr pRb = OdResBuf::newRb(OdResBuf::kDxfRegAppName, strAppName);
  OdInt16 const nDummy = 0;
  pRb->setNext(OdResBuf::newRb(OdResBuf::kDxfXdInteger16, nDummy));
  idEnt.safeOpenObject(OdDb::kForWrite)->setXData(pRb);
  pRb.release();

  // make a filter to select entities which have the the specified application name.
/*  OdResBufPtr pFilter;
  pFilter = OdResBuf::newRb(OdResBuf::kDxfXDataStart);
  pFilter->setNext(OdResBuf::newRb(OdResBuf::kDxfRegAppName, strAppName));
*/
  // make conditional filter
  OdResBufPtr pFilter;
  pFilter = OdResBuf::newRb(OdResBuf::kDxfOperator, OD_T("<and"));
  pFilter->last()->setNext(OdResBuf::newRb(OdResBuf::kDxfStart, OD_T("LINE")));
  pFilter->last()->setNext(OdResBuf::newRb(OdResBuf::kDxfOperator, OD_T("*")));
  pFilter->last()->setNext(OdResBuf::newRb(67, OdInt16(0)));
  pFilter->last()->setNext(OdResBuf::newRb(OdResBuf::kDxfOperator, OD_T("and>")));

  // select all entities with the filter.
  try {
    pSS = OdDbSelectionSet::select(pDb, pFilter);
  }
  catch (OdEdCancel) {
    return;
  }

  // report number of entities selected by the filter.
  OdUInt32 const nNumEntities = pSS->numEntities();
  OdString s;
  s.format(OD_T("selected: %d.\n"), nNumEntities);
  pIO->putString(s);
}

void _MoveModelToPaperspace_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabase* pDb = pDbCmdCtx->database();
  OdDbBlockTableRecordPtr ms = pDb->getModelSpaceId().safeOpenObject(OdDb::kForRead);
  // collect model space entities
  OdDbObjectIdArray ids;
  for (OdDbObjectIteratorPtr it = ms->newIterator(); !it->done(); it->step())
    ids.append(it->objectId());
  ms = 0; // close model space
  OdDbLayoutManagerPtr lm = pDb->appServices()->layoutManager();
  OdDbLayoutPtr psLayout = lm->findLayoutNamed(pDb, lm->findActiveLayout(pDb, false)).safeOpenObject();
  OdDbBlockTableRecordPtr ps = psLayout->getBlockTableRecordId().safeOpenObject(OdDb::kForWrite);
  // move MS contents to PS
  ps->assumeOwnershipOf(ids);
}

void _NumHatchLines_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbBlockTableRecordPtr blockRecord = pDbCmdCtx->database()->getModelSpaceId().safeOpenObject();
  for (OdDbObjectIteratorPtr objectIterator = blockRecord->newIterator(); !objectIterator->done(); objectIterator->step())
  {
    OdDbObjectPtr obj = objectIterator->objectId().safeOpenObject();
    if (obj->isKindOf(OdDbHatch::desc()))
    {
      OdDbHatchPtr hatch = OdDbHatch::cast(obj);
      int n = hatch->numHatchLines();
      OdString s;
      s.format(L"Hatch %X has %d lines", (OdUInt32)(OdUInt64)obj->handle(), n);
      pDbCmdCtx->userIO()->putString(s);
    }
  }  
}

void _REFEDIT_XREF_NESTED_BLOCK_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbSelectionSetIteratorPtr it = pDbCmdCtx->dbUserIO()->select(L"Select reference",OdEd::kSelSingleEntity)->newIterator();
  if (it->done())
  {
    pDbCmdCtx->dbUserIO()->putString(L"Nothing selected");
    return;
  }
  OdDbBlockReferencePtr b = it->objectId().safeOpenObject();
  if (b.isNull())
  {
    pDbCmdCtx->dbUserIO()->putString(L"Selected object is not a block reference");
    return;
  }
  OdDbBlockTableRecordPtr btr = b->blockTableRecord().safeOpenObject();
  if(!btr->isFromExternalReference())
  {
    pDbCmdCtx->dbUserIO()->putString(L"Selected object is not an external reference");
    return;
  }

  //Get id of nested block
  OdDbObjectId nestedBlockId, nestedBlockRef;
  {
    for(OdDbObjectIteratorPtr it = btr->newIterator(); !it->done(); it->step())
    {
      OdDbBlockReferencePtr pRef = OdDbBlockReference::cast(it->entity());
      if(pRef.get())
      {
        nestedBlockId = pRef->blockTableRecord();
        nestedBlockRef = pRef->objectId();
        break;
      }
    }
  }

  if(!nestedBlockId)
  {
    pDbCmdCtx->dbUserIO()->putString(L"Nested block not found");
    return;
  }

  b = 0; // close reference, to allow it to be opened in transaction checkout (to lock it)
  OdDbObjectIdArray ia;
  {
    OdDbBlockTableRecordPtr pNestedBlock = nestedBlockId.openObject();
    for (OdDbObjectIteratorPtr i = pNestedBlock->newIterator(); !i->done(); i->step())
      ia.append(i->objectId());
  }
#ifdef ODA_DIAGNOSTICS
  const int nSubents = ia.size();
#endif // ODA_DIAGNOSTICS

  OdDbObjectId transId;
  {
    OdDbIdMappingPtr errrorMap = OdDbIdMapping::createObject();
    OdResult res = odapLongTransactionManager()->checkOut(transId, ia, pDbCmdCtx->database()->getActiveLayoutBTRId(), *errrorMap, nestedBlockRef);
    if(res != eOk)
    {
      pDbCmdCtx->dbUserIO()->putString(L"Failed to checkout");
      return;
    }
    ::odedRegCmds()->executeCommand(L"REGEN", pCmdCtx);
  }

  OdDbLongTransactionPtr pTrans = transId.openObject();
  OdDbLongTransWorkSetIteratorPtr it2 = pTrans->newWorkSetIterator();
  pTrans = NULL;

  OdGeMatrix3d tf;
  tf.setToTranslation(OdGeVector3d(100, 200, 300));
  int nModified = 0;
  for(; !it2->done(); it2->step())
  {
    if(!it2->curObjectIsPrimary())
      continue;
    OdDbEntityPtr pEnt = it2->objectId().openObject(OdDb::kForWrite);
    pEnt->transformBy(tf);
    ++nModified;
  }
  ODA_ASSERT(nModified == nSubents);

  //Check in now
  OdDbIdMappingPtr errrorMap = OdDbIdMapping::createObject();
  // transform workset objects back to original state, to be cloned to block
  OdResult res = odapLongTransactionManager()->checkIn(transId, *errrorMap);
  if (res != eOk)
  {
    pDbCmdCtx->dbUserIO()->putString(L"REFCLOSE failed. Reason: " + pDbCmdCtx->database()->appServices()->formatMessage(res));
    return;
  }

  {
    int nSubentsAfterCheckin = 0;
    OdDbBlockTableRecordPtr pNestedBlock = nestedBlockId.openObject();
    for(OdDbObjectIteratorPtr i = pNestedBlock->newIterator(); !i->done(); i->step())
      ++nSubentsAfterCheckin;
    ODA_ASSERT(nSubentsAfterCheckin == nSubents);
  }
}

void _REFEDIT_SAVE_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbSelectionSetIteratorPtr it = pDbCmdCtx->dbUserIO()->select(L"Select reference",OdEd::kSelSingleEntity)->newIterator();
  if (it->done())
  {
    pDbCmdCtx->dbUserIO()->putString(L"Nothing selected");
    return;
  }
  OdDbBlockReferencePtr b = OdDbBlockReference::cast(it->objectId().openObject());
  if (b.isNull())
  {
    pDbCmdCtx->dbUserIO()->putString(L"Selected object is not a block reference");
    return;
  }
  OdDbBlockTableRecordPtr btr = b->blockTableRecord().safeOpenObject();
  if(!btr->isFromExternalReference())
  {
    pDbCmdCtx->dbUserIO()->putString(L"Selected object is not an external reference");
    return;
  }
  btr->xrefDatabase()->restoreOriginalXrefSymbols();
  btr->xrefDatabase()->writeFile(btr->xrefDatabase()->getFilename(),
    btr->xrefDatabase()->originalFileType(), btr->xrefDatabase()->originalFileVersion());
  btr->xrefDatabase()->restoreForwardingXrefSymbols();
}

void _CopySubent_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();
  OdDbDatabase* pDatabase = pDbCmdCtx->database();
  OdDbBlockTableRecordPtr pMs = pDatabase->getModelSpaceId().safeOpenObject(OdDb::kForWrite);
  OdDbSelectionSetPtr pSel = pIO->select(OD_T("Select objects:"), OdEd::kSelSingleEntity |
                                                                  OdEd::kSelAllowSubents );
  if (pSel->numEntities() != 0)
  {
    OdDbSelectionSetIteratorPtr pIter = pSel->newIterator();
    while ( !pIter->done() )
    {
      OdDbObjectId objId  = pIter->objectId();
      OdDbEntityPtr pEnt  = objId.openObject( OdDb::kForWrite );

      if( !pEnt.isNull() && pEnt->isKindOf(OdDb3dSolid::desc()))
      {
        if( pIter->subentCount() )
        {
          OdDbFullSubentPath subentPath;
          int count = pIter->subentCount();
          for( int i = 0; i < count; i++ )
          {
            if(pIter->getSubentity(i,subentPath))
            {
              OdResult res = eOk;
              OdDbEntityPtr newEntity;
              if (subentPath.subentId().type() == OdDb::kEdgeSubentType)
                res = OdDb3dSolidPtr(pEnt)->copyEdge(subentPath.subentId(), newEntity);
              if (subentPath.subentId().type() == OdDb::kFaceSubentType)
                res = OdDb3dSolidPtr(pEnt)->copyFace(subentPath.subentId(), newEntity);
              if (res == eOk && !newEntity.isNull())
              {
                pMs->appendOdDbEntity(newEntity);
                pIO->putString(OD_T("eOk"));
              }
              else
                pIO->putString(OD_T("Error"));
            }
          }
        }
      }
      pIter->next();
    }
  }
}

void _RemoveFaces_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();
  OdDbSelectionSetPtr pSel = pIO->select(OD_T("Select objects:"), OdEd::kSelSingleEntity |
                                                                  OdEd::kSelAllowSubents );
  if (pSel->numEntities() != 0)
  {
    OdDbSelectionSetIteratorPtr pIter = pSel->newIterator();
    while ( !pIter->done() )
    {
      OdDbObjectId objId  = pIter->objectId();
      OdDbEntityPtr pEnt  = objId.openObject( OdDb::kForWrite );

      if( !pEnt.isNull() && pEnt->isKindOf(OdDb3dSolid::desc()))
      {
        if( pIter->subentCount() )
        {
          OdDbFullSubentPath subentPath;
          int count = pIter->subentCount();
          OdArray<OdDbSubentId*> arr;
          OdArray<OdDbFullSubentPath> arrPath;
          arrPath.resize(count);
          for( int i = 0; i < count; i++ )
          {
            if(pIter->getSubentity(i,arrPath[i]))
            {
              arr.push_back(&arrPath[i].subentId());
            }
          }
          OdResult res = OdDb3dSolidPtr(pEnt)->removeFaces(arr);
          if (res == eOk)
            pIO->putString(OD_T("eOk"));
          else
            pIO->putString(OD_T("Error"));
        }
      }
      pIter->next();
    }
  }
}

void _OffsetFaces_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();
  OdDbSelectionSetPtr pSel = pIO->select(OD_T("Select objects:"), OdEd::kSelSingleEntity |
                                                                  OdEd::kSelAllowSubents );
  if (pSel->numEntities() != 0)
  {
    OdDbSelectionSetIteratorPtr pIter = pSel->newIterator();
    while ( !pIter->done() )
    {
      OdDbObjectId objId  = pIter->objectId();
      OdDbEntityPtr pEnt  = objId.openObject( OdDb::kForWrite );

      if( !pEnt.isNull() && pEnt->isKindOf(OdDb3dSolid::desc()))
      {
        if( pIter->subentCount() )
        {
          OdDbFullSubentPath subentPath;
          int count = pIter->subentCount();
          OdArray<OdDbSubentId*> arr;
          OdArray<OdDbFullSubentPath> arrPath;
          arrPath.resize(count);
          for( int i = 0; i < count; i++ )
          {
            if(pIter->getSubentity(i,arrPath[i]))
            {
              arr.push_back(&arrPath[i].subentId());
            }
          }
          double offsetDist   = pIO->getReal(OD_T("Specify offset distance:"));
          OdResult res = OdDb3dSolidPtr(pEnt)->offsetFaces(arr, offsetDist);
          if (res == eOk)
            pIO->putString(OD_T("eOk"));
          else
            pIO->putString(OD_T("Error"));
        }
      }
      pIter->next();
    }
  }
}

void _ChamferEdges_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();
  OdDbSelectionSetPtr pSel = pIO->select(OD_T("Select objects:"), OdEd::kSelSingleEntity |
                                                                  OdEd::kSelAllowSubents );
  if (pSel->numEntities() != 0)
  {
    OdDbSelectionSetIteratorPtr pIter = pSel->newIterator();
    while ( !pIter->done() )
    {
      OdDbObjectId objId  = pIter->objectId();
      OdDbEntityPtr pEnt  = objId.openObject( OdDb::kForWrite );

      if( !pEnt.isNull() && (pEnt->isKindOf(OdDb3dSolid::desc()) || pEnt->isKindOf(OdDbSurface::desc())))
      {
        if( pIter->subentCount() )
        {
          OdDbFullSubentPath subentPath;
          int count = pIter->subentCount();
          OdArray<OdDbSubentId*> arr;
          OdArray<OdDbFullSubentPath> arrPath;
          arrPath.resize(count);
          for( int i = 0; i < count; i++ )
          {
            if(pIter->getSubentity(i,arrPath[i]))
            {
              arr.push_back(&arrPath[i].subentId());
            }
          }

          OdDbSubentId faceId(OdDb::kFaceSubentType, kFirstFaceMarker);
          double baseDist = pIO->getReal(OD_T("Specify base distance:"));
          double otherDist = pIO->getReal(OD_T("Specify other distance:"));

          OdResult res = eInvalidInput;
          if (pEnt->isKindOf(OdDb3dSolid::desc()))
            res = OdDb3dSolidPtr(pEnt)->chamferEdges(arr, faceId, baseDist, otherDist);
          else if (pEnt->isKindOf(OdDbSurface::desc()))
            res = OdDbSurfacePtr(pEnt)->chamferEdges(arr, faceId, baseDist, otherDist);

          if (res == eOk)
            pIO->putString(OD_T("eOk"));
          else
            pIO->putString(OD_T("Error"));
        }
      }
      pIter->next();
    }
  }
}

void _ShellBody_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();
  OdDbSelectionSetPtr pSel = pIO->select(OD_T("Select objects:"), OdEd::kSelSingleEntity |
                                                                  OdEd::kSelAllowSubents );

  if (pSel->numEntities() != 0)
  {
    OdDbSelectionSetIteratorPtr pIter = pSel->newIterator();
    while ( !pIter->done() )
    {
      OdDbObjectId objId  = pIter->objectId();
      OdDbEntityPtr pEnt  = objId.openObject( OdDb::kForWrite );

      if( !pEnt.isNull() && pEnt->isKindOf(OdDb3dSolid::desc()))
      {
        if( pIter->subentCount() )
        {
          OdDbFullSubentPath subentPath;
          int count = pIter->subentCount();
          OdArray<OdDbSubentId*> arr;
          OdArray<OdDbFullSubentPath> arrPath;
          arrPath.resize(count);
          for( int i = 0; i < count; i++ )
          {
            if(pIter->getSubentity(i,arrPath[i]))
            {
              arr.push_back(&arrPath[i].subentId());
            }
          }
          double offsetDist   = pIO->getReal(OD_T("Specify offset distance:"));
          OdResult res = OdDb3dSolidPtr(pEnt)->shellBody(arr, offsetDist);
          if (res == eOk)
            pIO->putString(OD_T("eOk"));
          else
            pIO->putString(OD_T("Error"));
        }
      }
      pIter->next();
    }
  }
}

void _NumChanges_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();

  OdDbSelectionSetPtr pFirstSel = pIO->select(OD_T("Select first entity"), OdEd::kSelSingleEntity);
  if (pFirstSel->numEntities() == 0)
    return;

  OdDbSelectionSetIteratorPtr pIter = pFirstSel->newIterator();
  OdDbObjectPtr pObj  = pIter->objectId().openObject();
  if (!pObj.isNull() && pObj->isKindOf(OdDb3dSolid::desc()))
  {
    OdUInt32 changes = OdDb3dSolidPtr(pObj)->numChanges();
    OdString sLog;
    sLog.format(L"   NumChanges: %i", changes);
    pIO->putString(sLog);
  }
}

void _TransformFaces_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();

  OdDbSelectionSetPtr pFirstSel = pIO->select(OD_T("Select first entity"), OdEd::kSelSingleEntity);
  if (pFirstSel->numEntities() == 0)
    return;

  OdDbSelectionSetIteratorPtr pIter = pFirstSel->newIterator();
  OdDbObjectPtr pObj  = pIter->objectId().openObject(OdDb::kForWrite);
  if (!pObj.isNull() && pObj->isKindOf(OdDb3dSolid::desc()))
  {
    OdArray<OdDbSubentId*> arr;
    OdDbSubentId subId(OdDb::kFaceSubentType, kFirstFaceMarker);
    arr.push_back(&subId);
    OdGeMatrix3d matrix;
    matrix.setCoordSystem(OdGePoint3d(1,1,1),
                          OdGeVector3d(1,0,0),
                          OdGeVector3d(0,1,0),
                          OdGeVector3d(0,0,1));

    OdResult res = OdDb3dSolidPtr(pObj)->transformFaces(arr, matrix);
    if (res == eOk)
      pIO->putString(OD_T("eOk"));
    else
      pIO->putString(OD_T("Error"));
  }
}

void _TaperFaces_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();
  OdDbSelectionSetPtr pSel = pIO->select(OD_T("Select objects:"), OdEd::kSelSingleEntity |
    OdEd::kSelAllowSubents );

  if (pSel->numEntities() != 0)
  {
    OdDbSelectionSetIteratorPtr pIter = pSel->newIterator();
    while ( !pIter->done() )
    {
      OdDbObjectId objId  = pIter->objectId();
      OdDbEntityPtr pEnt  = objId.openObject( OdDb::kForWrite );

      if( !pEnt.isNull() && pEnt->isKindOf(OdDb3dSolid::desc()) )
      {
        if( pIter->subentCount() )
        {
          OdDbFullSubentPath subentPath;
          int count = pIter->subentCount();
          OdArray<OdDbSubentId*> arr;
          OdArray<OdDbFullSubentPath> arrPath;
          arrPath.resize(count);
          for( int i = 0; i < count; i++ )
          {
            if(pIter->getSubentity(i,arrPath[i]))
            {
              arr.push_back(&arrPath[i].subentId());
            }
          }
          OdGePoint3d ptBase  = pIO->getPoint(OD_T("Specify base point:"));
          OdGePoint3d ptDraft = pIO->getPoint(OD_T("Specify draft point:"));
          double draftAngle   = pIO->getReal(OD_T("Specify draft angle:"));
          OdResult res = OdDb3dSolidPtr(pEnt)->taperFaces(arr, ptBase, ptDraft.asVector(), draftAngle);
          if (res == eOk)
            pIO->putString(OD_T("eOk"));
          else
            pIO->putString(OD_T("Error"));
        }
      }
      pIter->next();
    }
  }
}

namespace{
  void blockAddInStack(OdDbEntityPtr pEnt, OdDbObjectIdArray* pEntAndInsertStack)
  {
    pEntAndInsertStack->append(pEnt->objectId());
    OdDbBlockReferencePtr blk = OdDbBlockReference::cast(pEnt);
    OdDbBlockTableRecordPtr btr = blk->blockTableRecord().safeOpenObject();
    OdDbObjectIteratorPtr It = btr->newIterator();
    if(!It.isNull())
    {
      OdDbObjectId id = It->objectId();
      OdDbEntityPtr pEntTmp = id.openObject();
      if (pEntTmp->isA() == OdDbBlockReference::desc())
      {
        blockAddInStack(pEntTmp, pEntAndInsertStack);
      }
      else
      {
        pEntAndInsertStack->append(id);
      }
    }
  }
}

void _SubentPaths_GsMarker_Block_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabase* pDatabase = pDbCmdCtx->database();
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();
  OdDbBlockTableRecordPtr pMs = pDatabase->getModelSpaceId().safeOpenObject();

  OdDbSelectionSetPtr pFirstSel = pIO->select(OD_T("Select first entity"), OdEd::kSelSingleEntity);
  if (pFirstSel->numEntities() == 0)
    return;

  OdDbSelectionSetIteratorPtr pIter = pFirstSel->newIterator();
  OdDbObjectId objId  = pIter->objectId();
  OdDbEntityPtr pEnt  = objId.openObject();
  if (!pEnt.isNull() && pEnt->isKindOf(OdDbBlockReference::desc()))
  {
    OdGePoint3d pickPoint;
    OdGeMatrix3d viewXform; 
    OdDbFullSubentPathArray subentPaths; 
    OdDbObjectIdArray pEntAndInsertStack;
    OdDbBlockReferencePtr blk = OdDbBlockReference::cast(pEnt);
    blockAddInStack(pEnt, &pEntAndInsertStack);
    pEntAndInsertStack.reverse();
    OdResult res = blk->getSubentPathsAtGsMarker(OdDb::kFaceSubentType, kFirstFaceMarker, pickPoint, viewXform, subentPaths, &pEntAndInsertStack);

    if (res == eOk)
    {
      pIO->putString(OD_T("eOk"));
      OdGsMarkerArray gsMarkArr;
      blk->getGsMarkersAtSubentPath(subentPaths[0], gsMarkArr);
    }
    else
      pIO->putString(OD_T("Error"));
  }
  pEnt->objectId().safeOpenObject();
}


void _SubentPaths_GsMarker_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabase* pDatabase = pDbCmdCtx->database();
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();
  OdDbBlockTableRecordPtr pMs = pDatabase->getModelSpaceId().safeOpenObject(OdDb::kForWrite);

  OdDbSelectionSetPtr pFirstSel = pIO->select(OD_T("Select first entity"), OdEd::kSelSingleEntity);
  if (pFirstSel->numEntities() == 0)
    return;

  OdDbSelectionSetIteratorPtr pIter = pFirstSel->newIterator();
  OdDbObjectId objId  = pIter->objectId();
  OdDbEntityPtr pEnt  = objId.openObject(OdDb::kForWrite);
  if (!pEnt.isNull() && pEnt->isKindOf(OdDb3dSolid::desc()))
  {
    OdGePoint3d pickPoint;
    OdGeMatrix3d viewXform; 
    OdDbFullSubentPathArray subentPaths; 

    OdResult res = OdDb3dSolidPtr(pEnt)->getSubentPathsAtGsMarker(OdDb::kFaceSubentType, kFirstFaceMarker, pickPoint, viewXform, subentPaths);

    OdGsMarkerArray gsMarkArr;
    OdDb3dSolidPtr(pEnt)->getGsMarkersAtSubentPath(subentPaths[0], gsMarkArr);
    if (res == eOk)
    {
      pIO->putString(OD_T("eOk"));
    }
    else
      pIO->putString(OD_T("Error"));
  }
}

void _SetSubentColor_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();
  OdDbSelectionSetPtr pSel = pIO->select(OD_T("Select objects:"), OdEd::kSelSingleEntity |
                                                                  OdEd::kSelAllowSubents );
  //OdCmColor newColor;
  //newColor.setRGB(200, 200, 0);
  OdCmColor newColor = pIO->getColor(OD_T("Specify new color:"));

  if (pSel->numEntities() != 0)
  {
    OdDbSelectionSetIteratorPtr pIter = pSel->newIterator();
    while ( !pIter->done() )
    {
      OdDbObjectId objId  = pIter->objectId();
      OdDbEntityPtr pEnt  = objId.openObject( OdDb::kForWrite );

      if( !pEnt.isNull() && pEnt->isKindOf(OdDb3dSolid::desc()))
      {
        if( pIter->subentCount() )
        {
          OdDbFullSubentPath subentPath;
          int count = pIter->subentCount();
          for( int i = 0; i < count; i++ )
          {
            if(pIter->getSubentity(i,subentPath))
            {
              OdCmColor retColor;
              OdResult res = OdDb3dSolidPtr(pEnt)->getSubentColor(subentPath.subentId(), retColor);

              res = OdDb3dSolidPtr(pEnt)->setSubentColor(subentPath.subentId(), newColor);
              pEnt->objectId().safeOpenObject();
            }
          }
        }
      }
      pIter->next();
    }
  }
}

///// Viewports

#if 1 // 3.4
class ViewportSelFilter : public ClassFilter
{
  public:
    ViewportSelFilter() : ClassFilter(OdDbViewport::desc()) { }

    static OdDbObjectId vptId(const OdDbObjectId &id)
    {
      return id;
    }
};
#else
class ViewportSelFilter : public OdStaticRxObject<OdEdSSetTracker>
{
public:
  ViewportSelFilter()
  {
  }

  int addDrawables(OdGsView* ) { return 0; }
  void removeDrawables(OdGsView* ) { }

  static OdDbObjectId vptId(const OdDbObjectId &id)
  {
    OdDbObjectPtr pObj = id.safeOpenObject();
    if (pObj->isKindOf(OdDbViewport::desc()))
      return id;
    OdDbBlockTableRecordPtr pBTR = OdDbBlockTableRecord::cast(pObj->ownerId().openObject());
    if (!pBTR.isNull())
    {
      OdDbObjectIteratorPtr pIt = pBTR->newIterator();
      while (!pIt->done())
      {
        if (pIt->objectId().openObject()->isKindOf(OdDbViewport::desc()))
        {
          if (pObj->hasPersistentReactor(pIt->objectId()))
            return pIt->objectId();
        }
        pIt->step();
      }
    }
    return OdDbObjectId::kNull;
  }

  bool check(const OdDbObjectId& entId)
  {
    try
    {
      return !vptId(entId).isNull();
    }
    catch(const OdError& )
    {
    }
    return false;
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
#endif

void _ViewportsOnOff_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();

  ViewportSelFilter fltr;
  OdDbSelectionSetPtr pSSet = pIO->select("Select a viewport(s):", 0, 0, OdString(), &fltr);
  if (pSSet->numEntities() == 0)
  {
    pIO->putString(OD_T("Nothing selected."));
    return;
  }

  OdString outStr;
  outStr.format(OD_T("Selected %u viewports."), pSSet->numEntities());
  pIO->putString(outStr);

  int backValue = pIO->getKeyword(OD_T("[On]/[Off] viewports or [List] current values?"), OD_T("On Off List"));
  bool enableVPs = backValue == 0;
  bool listVPs = backValue == 2;

  //const bool regen = true;
  /*if (!listVPs)
  {
    regen = pIO->getKeyword(OD_T("Regen?"), OD_T("Yes No")) == 0;
  }*/

  OdDbSelectionSetIteratorPtr pIter = pSSet->newIterator();

  OdInt32 n = 0;
  while(!pIter->done())
  {
    OdDbViewportPtr pVp = ViewportSelFilter::vptId(pIter->objectId()).safeOpenObject((listVPs) ? OdDb::kForRead : OdDb::kForWrite);
    if (listVPs)
    {
      outStr.format(OD_T("Viewport %d: %s."), n, (pVp->isOn()) ? OD_T("On") : OD_T("Off"));
      pIO->putString(outStr);
    }
    else
    {
      if (enableVPs)
      {
        pVp->setOn();
      }
      else
      {
        pVp->setOff();
      }
    }
    pIter->next();
    n++;
  }

  pIO->putString(OD_T("Completed."));

  if (!listVPs/* && regen*/)
  {
    ::odedRegCmds()->executeCommand(L"REGEN", pCmdCtx);
  }
}

void _CreateViewport_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();
  OdDbDatabasePtr pDb = pDbCmdCtx->database();

  // Check if we are inside paperspace
  if (pDb->getActiveLayoutBTRId() == pDb->getModelSpaceId())
  {
    pIO->putString(OD_T("Can't create viewport in *ModelSpace."));
    return;
  }

  int backValue = pIO->getKeyword(OD_T("Create [Rect] viewport, viewport from [Object] or [Clone] viewport?"), OD_T("Rect Object Clone"));
  switch (backValue)
  {
    case 0:
      { // Rectangle viewport
        // Get location
        OdGePoint3d pt[2];
        try
        {
          pt[0] = pIO->getPoint(OD_T("Specify first corner:"), OdEd::kGptNoLimCheck | OdEd::kGptNoUCS);
          pt[1] = pIO->getPoint(OD_T("Specify opposite corner:"), OdEd::kGptNoLimCheck | OdEd::kGptNoUCS | OdEd::kGptRectFrame);
        }
        catch (const OdEdCancel)
        {
          pIO->putString(OD_T("Canceled."));
          return;
        }
        OdGeExtents2d ptExt;
        for (int i = 0; i < 2; i++)
          ptExt.addPoint(pt[i].convert2d());

        // Create viewport
        OdDbViewportPtr pViewport = OdDbViewport::createObject();
        pViewport->setDatabaseDefaults(pDb);
        OdDbBlockTableRecord::cast(pDb->getActiveLayoutBTRId().openObject(OdDb::kForWrite))->appendOdDbEntity(pViewport);
        // Location in paperspace
        pViewport->setWidth(ptExt.maxPoint().x - ptExt.minPoint().x);
        pViewport->setHeight(ptExt.maxPoint().y - ptExt.minPoint().y);
        pViewport->setCenterPoint(OdGePoint3d(OdGePlane::kXYPlane, ptExt.minPoint() + ((ptExt.maxPoint() - ptExt.minPoint()) * 0.5)));
        // Location in modelspace
        pViewport->zoomExtents();
      }
    break;
    case 1: // Viewport from object
      {
        OdDbSelectionSetPtr pEntSel = pIO->select(OD_T("Select an entity:"), OdEd::kSelSingleEntity);
        if (pEntSel->numEntities() == 0)
        {
          pIO->putString(OD_T("Nothing selected."));
          return;
        }

        // Create viewport
        OdDbViewportPtr pViewport = OdDbViewport::createObject();
        pViewport->setDatabaseDefaults(pDb);
        OdDbBlockTableRecord::cast(pDb->getActiveLayoutBTRId().openObject(OdDb::kForWrite))->appendOdDbEntity(pViewport);
        // Location in paperspace
        pViewport->setNonRectClipEntityId(pEntSel->objectIdArray().first());
        // Location in modelspace
        pViewport->zoomExtents();
      }
    break;
    case 2: // Clone
      {
        ViewportSelFilter fltr;
        OdDbSelectionSetPtr pSSet = pIO->select("Select a viewport:", OdEd::kSelSingleEntity, 0, OdString::kEmpty, &fltr);
        if (pSSet->numEntities() == 0)
        {
          pIO->putString(OD_T("Nothing selected."));
          return;
        }

        OdGePoint3d pt = pIO->getPoint(OD_T("Specify insertion point:"), OdEd::kGptNoLimCheck | OdEd::kGptNoUCS);

        // Create viewport
        OdDbViewportPtr pViewport = OdDbViewport::createObject();
        pViewport->setDatabaseDefaults(pDb);
        OdDbBlockTableRecord::cast(pDb->getActiveLayoutBTRId().openObject(OdDb::kForWrite))->appendOdDbEntity(pViewport);
        pViewport->copyFrom(ViewportSelFilter::vptId(pSSet->objectIdArray().first()).openObject());
        // Location in paperspace
        pViewport->setCenterPoint(OdGePoint3d(pt.x + pViewport->width() * 0.5, pt.y + pViewport->height() * 0.5, pt.z));
      }
    break;
    default:
      pIO->putString(OD_T("Bad input."));
      return;
  }

  pIO->putString(OD_T("Completed."));

  ::odedRegCmds()->executeCommand(OD_T("REGEN"), pCmdCtx);
}

void _SetActiveViewport_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();
  OdDbDatabasePtr pDb = pDbCmdCtx->database();

  // Check if we are inside paperspace
  if (pDb->getActiveLayoutBTRId() != pDb->getModelSpaceId())
  {
    int backValue = pIO->getKeyword(OD_T("[Select] OdDbViewport or set [Overall] viewport?"), OD_T("Select Overall"));
    bool bOverall = backValue == 1;

    OdDbSelectionSetPtr pSSet;

    if (!bOverall)
    {
      ViewportSelFilter fltr;
      pSSet = pIO->select("Select a viewport:", OdEd::kSelSingleEntity, 0, OdString::kEmpty, &fltr);
      if (pSSet->numEntities() == 0)
      {
        pIO->putString(OD_T("Nothing selected."));
        return;
      }
    }

    OdDbLayoutPtr pLayout = OdDbLayout::cast(OdDbBlockTableRecord::cast(pDb->getActiveLayoutBTRId().safeOpenObject())->getLayoutId().safeOpenObject(OdDb::kForWrite));
    if (pLayout.isNull())
    {
      pIO->putString(OD_T("Not a layout."));
      return;
    }
    OdDbSelectionSetIteratorPtr pIter;
    if (!bOverall) pIter = pSSet->newIterator();
    pLayout->setActiveViewportId((!bOverall) ? ViewportSelFilter::vptId(pIter->objectId()) : pLayout->overallVportId());
  }
  else
  {
    OdDbViewportTablePtr pVPT = OdDbViewportTable::cast(pDb->getViewportTableId().openObject(OdDb::kForWrite));
    OdDbObjectId id = pVPT->getActiveViewportId();
    OdDbObjectId nextId;
    OdDbSymbolTableIteratorPtr pIt = pVPT->newIterator();
    while (!pIt->done())
    {
      if (pIt->getRecordId() != nextId)
      {
        if (!nextId.isNull())
        {
          nextId = pIt->getRecordId();
          break;
        }
        else if (pIt->getRecordId() == id)
        {
          nextId = id;
        }
      }
      pIt->step();
    }
    if (nextId.isNull() || nextId == id)
    {
      pIt->start();
      if (!pIt->done())
        id = pIt->getRecordId();
    }
    if (nextId.isNull() || nextId == id)
    {
      return;
    }
    pVPT->SetActiveViewport(nextId);
  }

  pIO->putString(OD_T("Completed."));

  ::odedRegCmds()->executeCommand(OD_T("REGEN"), pCmdCtx);
}

void _CVPORTTest_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();
  OdDbDatabasePtr pDb = pDbCmdCtx->database();

  // Check if we are inside paperspace
  if (pDb->getActiveLayoutBTRId() == pDb->getModelSpaceId())
  {
    pIO->putString(OD_T("Not implemented for *ModelSpace."));
    return;
  }

  OdDbLayoutPtr pLayout = OdDbLayout::cast(OdDbBlockTableRecord::cast(pDb->getActiveLayoutBTRId().safeOpenObject())->getLayoutId().safeOpenObject(OdDb::kForWrite));
  if (pLayout.isNull())
  {
    pIO->putString(OD_T("No layout."));
    return;
  }

  OdDbBlockTableRecordPtr pBTR = OdDbBlockTableRecord::cast(pDb->getActiveLayoutBTRId().safeOpenObject());
  OdDbObjectIteratorPtr pIt = pBTR->newIterator(true, false);
  // Iteration for number
  OdString str;
  long nid = 0;
  pIO->putString(OD_T("<<< number"));
  while (!pIt->done())
  {
    OdDbViewportPtr pVp = OdDbViewport::cast(pIt->entity(OdDb::kForRead, true));
    if (!pVp.isNull())
    {
      OdDbObjectId id = pVp->id();
      str.format(OD_T("%d number(handle: %s, erased: %d) %d"), nid, id.getHandle().ascii().c_str(), id.isErased() ? 1L : 0L, (long)pVp->number());
      pIO->putString(str);
      nid++;
    }
    pIt->step(true, false);
  }
  // Iteration for cvport
  nid = 0;
  pIO->putString(OD_T("<<< cvport"));
  OdDbObjectId actId = pLayout->activeViewportId();
  pIt->start(true, false);
  while (!pIt->done())
  {
    OdDbViewportPtr pVp = OdDbViewport::cast(pIt->entity(OdDb::kForRead, true));
    if (!pVp.isNull())
    {
      OdDbObjectId id = pVp->id();
      if (id.isErased())
      {
        str.format(OD_T("%d number(handle: %s, erased: %d) ERASED"), nid, id.getHandle().ascii().c_str(), id.isErased() ? 1L : 0L);
      }
      else
      {
        pLayout->setActiveViewportId(id);
        str.format(OD_T("%d number(handle: %s, erased: %d) %d"), nid, id.getHandle().ascii().c_str(), id.isErased() ? 1L : 0L, (long)pDb->getSysVar(OD_T("CVPORT"))->getInt16());
      }
      pIO->putString(str);
      nid++;
    }
    pIt->step(true, false);
  }
  pLayout->setActiveViewportId(actId);

  pIO->putString(OD_T("Completed."));
}

#include "Ge/GeRay3d.h"
#include "DbRay.h"

static void UnprojectRay(const OdGeMatrix3d &xForm, OdGeRay3d &rayOut, long int ptX, long int ptY,
                         bool bPerspective, const OdGePoint3d &basePt, const OdGeVector3d &target,
                         double perspCoef)
{
  if (bPerspective)
  {
    OdGePoint3d dir;
    dir.set((double)ptX, (double)ptY, perspCoef);
    dir.transformBy(xForm);
    rayOut.set(basePt, dir - basePt);
  }
  else
  {
    OdGePoint3d origin, direction;
    origin.set((double)ptX, (double)ptY, 0.0);
    direction = origin + OdGeVector3d::kZAxis; // @@@TODO: length to far side
    origin.transformBy(xForm);
    direction.transformBy(xForm);
    rayOut.set(origin - target, direction - origin);
  }
}

static void GeRayToDatabaseConv(const OdGeRay3d &ray, OdDbDatabase *pDb)
{
  OdDbLinePtr pRay = OdDbLine::createObject();
  pRay->setStartPoint(ray.pointOnLine());
  pRay->setEndPoint(ray.pointOnLine() + ray.direction() * 100.0);
  //OdDbRayPtr pRay = OdDbRay::createObject();
  //pRay->setBasePoint(ray.pointOnLine());
  //pRay->setUnitDir(ray.direction());
  OdDbBlockTableRecord::cast(pDb->getActiveLayoutBTRId().safeOpenObject(OdDb::kForWrite))->appendOdDbEntity(pRay);
}

static void GeRayToDatabaseConv(const OdGeRay3d &ray1, const OdGeRay3d &ray2, OdDbDatabase *pDb, bool bPerspective)
{
  OdDbLinePtr pLine = OdDbLine::createObject();
  double mult = (bPerspective) ? 100.0 : 0.0;
  pLine->setStartPoint(ray1.pointOnLine() + ray1.direction() * mult);
  pLine->setEndPoint(ray2.pointOnLine() + ray2.direction() * mult);
  OdDbBlockTableRecord::cast(pDb->getActiveLayoutBTRId().safeOpenObject(OdDb::kForWrite))->appendOdDbEntity(pLine);
}

static void EmitRayQuad(OdDbDatabase *pDb, const OdGeMatrix3d &xForm, long int minX, long int minY, long int maxX, long int maxY,
                        bool bPerspective, const OdGePoint3d &basePt, const OdGeVector3d &target, double perspCoef)
{
  OdGeRay3d rays[4];
  UnprojectRay(xForm, rays[0], minX, minY, bPerspective, basePt, target, perspCoef);
  UnprojectRay(xForm, rays[1], maxX, minY, bPerspective, basePt, target, perspCoef);
  UnprojectRay(xForm, rays[2], maxX, maxY, bPerspective, basePt, target, perspCoef);
  UnprojectRay(xForm, rays[3], minX, maxY, bPerspective, basePt, target, perspCoef);
  GeRayToDatabaseConv(rays[0], pDb);
  GeRayToDatabaseConv(rays[1], pDb);
  GeRayToDatabaseConv(rays[2], pDb);
  GeRayToDatabaseConv(rays[3], pDb);
  GeRayToDatabaseConv(rays[0], rays[1], pDb, bPerspective);
  GeRayToDatabaseConv(rays[1], rays[2], pDb, bPerspective);
  GeRayToDatabaseConv(rays[2], rays[3], pDb, bPerspective);
  GeRayToDatabaseConv(rays[3], rays[0], pDb, bPerspective);
}

void _RayEmitter_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  // Check if we are inside paperspace
  OdGsView *pView = NULL;
  if (pDb->getActiveLayoutBTRId() != pDb->getModelSpaceId())
  {
    OdDbLayoutPtr pLayout = OdDbLayout::cast(OdDbBlockTableRecord::cast(pDb->getActiveLayoutBTRId().safeOpenObject())->getLayoutId().safeOpenObject());
    OdDbViewportPtr pVpt = OdDbViewport::cast(pLayout->activeViewportId().safeOpenObject());
    pView = pVpt->gsView();
  }
  else
  {
    OdDbViewportTablePtr pVPT = OdDbViewportTable::cast(pDb->getViewportTableId().openObject(OdDb::kForWrite));
    OdDbViewportTableRecordPtr pVpt = OdDbViewportTableRecord::cast(pVPT->getActiveViewportId().safeOpenObject());
    pView = pVpt->gsView();
  }
  if (!pView)
  {
    pIO->putString(OD_T("No associated GsView found."));
    return;
  }
  OdGeMatrix3d xForm = pView->worldToDeviceMatrix(); // product of view/projection/screen matrices
  OdGeMatrix3d invXForm = xForm.inverse();
  OdGsDCRect rect;
  pView->getViewport(rect);
  long int width = rect.m_max.x - rect.m_min.x;
  long int height = rect.m_min.y - rect.m_max.y;
  EmitRayQuad(pDb, invXForm, rect.m_min.x, rect.m_max.y, rect.m_max.x, rect.m_min.y, pView->isPerspective(), pView->position(),
              (pView->target() - pView->position()).normal(), pView->projectionMatrix()(2,3));
  EmitRayQuad(pDb, invXForm, rect.m_min.x + (width >> 2), rect.m_max.y + (height >> 2),
    rect.m_max.x - (width >> 2), rect.m_min.y - (height >> 2), pView->isPerspective(), pView->position(),
    (pView->target() - pView->position()).normal(), pView->projectionMatrix()(2,3));
  // control ray
  OdGeRay3d geRay;
  geRay.set(pView->position(), pView->target() - pView->position());
  GeRayToDatabaseConv(geRay, pDb);
}

void _DrawViewportFirst_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  if (pDb->getActiveLayoutBTRId() == pDb->getModelSpaceId())
  {
    pIO->putString(OD_T("Works only in PS."));
    return;
  }
  OdDbLayoutPtr pLayout = OdDbLayout::cast(OdDbBlockTableRecord::cast(pDb->getActiveLayoutBTRId().safeOpenObject())->getLayoutId().safeOpenObject(OdDb::kForWrite));
  if (pLayout.isNull())
  {
    pIO->putString(OD_T("Not a layout."));
    return;
  }
  OdString acc;
  acc.format(OD_T("New value [True/False] <%ls>:"), pLayout->drawViewportsFirst() ? OD_T("true") : OD_T("false"));
  int backValue = pIO->getKeyword(acc, OD_T("True False"), pLayout->drawViewportsFirst() ? 0 : 1);
  pLayout->setDrawViewportsFirst(backValue == 0);

  pIO->putString(OD_T("Completed."));

  ::odedRegCmds()->executeCommand(OD_T("REGEN"), pCmdCtx);
}

void _XClipVolumeIntersectionTest_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();

  ClassFilter fltr(OdDbBlockReference::desc());
  OdDbSelectionSetPtr pSSet = pIO->select(OD_T("Select block reference:"), OdEd::kSelSingleEntity, 0, OdString::kEmpty, &fltr);
  OdDbSelectionSetIteratorPtr pIter = pSSet->newIterator();
  unsigned nCountTotal = 0, nCountClipped = 0;
  while(!pIter->done())
  {
    OdDbBlockReferencePtr pBR = pIter->objectId().safeOpenObject(OdDb::kForWrite);
    OdDbDictionaryPtr pDict = OdDbDictionary::cast(pBR->extensionDictionary().openObject());
    if (pDict.isNull())
    {
      pIO->putString(OD_T("No extension dictionary found."));
      return;
    }
    OdDbDictionaryPtr pFDict = OdDbDictionary::cast(pDict->getAt(OD_T("ACAD_FILTER"), OdDb::kForRead));
    if (pFDict.isNull())
    {
      pIO->putString(OD_T("No filters dictionary found."));
      return;
    }
    OdDbSpatialFilterPtr pSp = OdDbSpatialFilter::cast(pFDict->getAt(OD_T("SPATIAL"), OdDb::kForWrite));
    if (pSp.isNull())
    {
      pIO->putString(OD_T("No spatial filter found."));
      return;
    }

    OdGeExtents3d entExt;
    for (OdDbObjectIteratorPtr pEntIt = OdDbBlockTableRecord::cast(pBR->blockTableRecord().safeOpenObject())->newIterator(); !pEntIt->done(); pEntIt->step())
    {
      OdDbEntityPtr pEnt = pEntIt->entity();
      if (pEnt->getGeomExtents(entExt) == eOk)
      { nCountTotal++;
        if (!pSp->clipVolumeIntersectsExtents(entExt))
        { nCountClipped++;
          pEnt->upgradeOpen();
          pEnt->setColorIndex(1);
        }
      }
    }
    pIter->next();
  }
  pIO->putString(OdString().format(OD_T("Total entities: %u, Intersects: %u, Disjoint: %u"), nCountTotal, nCountTotal - nCountClipped, nCountClipped).c_str());
}

////// eof Viewports

////// Transient Manager

#include <Gi/GiTransientManager.h>

static OdArray<OdGiDrawablePtr> _Transients_Array;

void _ClearTransients_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();
  OdDbDatabasePtr pDb = pDbCmdCtx->database();

  OdGsView *pView = _GetGsView_func(pDb);
  if (pView == NULL)
  {
    pIO->putString(OD_T("No GsView available!"));
    return;
  }
  OdGiTransientManagerPtr pTM = ::odgiGetTransientManager(pView->device());
  if (pTM.isNull())
  {
    pIO->putString(OD_T("No Transient Manager available!"));
    return;
  }

  OdUInt32 nTransients = _Transients_Array.size();
  for (OdUInt32 nTransient = 0; nTransient < nTransients; nTransient++)
  {
    pTM->eraseTransient(_Transients_Array[nTransient], OdUInt32Array());
  }
  _Transients_Array.setPhysicalLength(0);
}

void _PlineRayIntersect_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();
  OdDbDatabasePtr pDb = pDbCmdCtx->database();

  OdGsView *pView = _GetGsView_func(pDb);
  if (pView == NULL)
  {
    pIO->putString(OD_T("No GsView available!"));
    return;
  }
  OdGiTransientManagerPtr pTM = ::odgiGetTransientManager(pView->device());
  if (pTM.isNull())
  {
    pIO->putString(OD_T("No Transient Manager available!"));
    return;
  }

  ClassFilter fltr(OdDbPolyline::desc());
  OdDbSelectionSetPtr pSSet = pIO->select(OD_T("Select a polyline:"), OdEd::kSelSingleEntity, 0, OdString(), &fltr);
  OdDbSelectionSetIteratorPtr pIt = pSSet->newIterator();
  for (; !pIt->done(); pIt->next())
  {
    OdGePoint3d point = pIO->getPoint(OD_T("Select an internal point:"));
    double angle = pIO->getAngle(OD_T("Specify ray direction:"));
    OdGePoint3d tempPoint = point + OdGeVector3d::kXAxis;
    tempPoint.rotateBy(angle, OdGeVector3d::kZAxis, point);
    OdGeVector3d rayDir = tempPoint - point;
    _ClearTransients_func(pCmdCtx);
    OdDbPolylinePtr pPl = OdDbPolyline::cast(pIt->objectId().openObject());
    unsigned int nSegs = pPl->numVerts();
    if (!pPl->isClosed()) nSegs--;
    OdGeRay2d lRay(point.convert2d(), rayDir.convert2d());
    for (unsigned int nSeg = 0; nSeg < nSegs; nSeg++)
    {
      OdGeLineSeg2d lSeg;
      pPl->getLineSegAt(nSeg, lSeg);
      OdGePoint2d intPt;
      if (lSeg.intersectWith(lRay, intPt))
      {
        OdDbCirclePtr pMarker = OdDbCircle::createObject();
        pMarker->setCenter(OdGePoint3d(intPt.x, intPt.y, 0.));
        pMarker->setRadius(100.0);
        pMarker->setColorIndex(2);
        _Transients_Array.push_back(pMarker);
        pTM->addTransient(pMarker, OdGiTransientManager::kOdGiDirectShortTerm, 0, OdUInt32Array());
      }
    }
  }
}

void _AddTransientMode_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();
  OdDbDatabasePtr pDb = pDbCmdCtx->database();

  OdGsView *pView = _GetGsView_func(pDb);
  if (pView == NULL)
  {
    pIO->putString(OD_T("No GsView available!"));
    return;
  }
  OdGiTransientManagerPtr pTM = ::odgiGetTransientManager(pView->device());
  if (pTM.isNull())
  {
    pIO->putString(OD_T("No Transient Manager available!"));
    return;
  }

  OdGePoint3d point = pIO->getPoint(OD_T("Select a point:"));
  int mode = pIO->getInt(OD_T("Enter mode:"));
  if (mode >= OdGiTransientManager::kOdGiDrawingModeCount || mode < 0) mode = OdGiTransientManager::kOdGiDrawingModeCount - 1;
  int subMode = pIO->getInt(OD_T("Enter subMode:"));
  OdDbCirclePtr pMarker = OdDbCircle::createObject();
  pMarker->setCenter(point);
  pMarker->setRadius(100.0);
  pMarker->setColorIndex(OdUInt16(1 + mode));
  _Transients_Array.push_back(pMarker);
  pTM->addTransient(pMarker, (OdGiTransientManager::OdGiTransientDrawingMode)mode, subMode, OdUInt32Array());
  OdDbLinePtr pMarker2 = OdDbLine::createObject();
  pMarker2->setStartPoint(point - OdGeVector3d::kXAxis * 100.0 - OdGeVector3d::kYAxis * 100.0);
  pMarker2->setEndPoint(point + OdGeVector3d::kXAxis * 100.0 + OdGeVector3d::kYAxis * 100.0);
  pMarker2->setColorIndex(pMarker->colorIndex());
  _Transients_Array.push_back(pMarker2);
  pTM->addTransient(pMarker2, (OdGiTransientManager::OdGiTransientDrawingMode)mode, subMode, OdUInt32Array());
  pMarker2 = OdDbLine::createObject();
  pMarker2->setStartPoint(point - OdGeVector3d::kXAxis * 100.0 + OdGeVector3d::kYAxis * 100.0);
  pMarker2->setEndPoint(point + OdGeVector3d::kXAxis * 100.0 - OdGeVector3d::kYAxis * 100.0);
  pMarker2->setColorIndex(pMarker->colorIndex());
  _Transients_Array.push_back(pMarker2);
  pTM->addTransient(pMarker2, (OdGiTransientManager::OdGiTransientDrawingMode)mode, subMode, OdUInt32Array());
}

void _EraseTransientMode_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();
  OdDbDatabasePtr pDb = pDbCmdCtx->database();

  OdGsView *pView = _GetGsView_func(pDb);
  if (pView == NULL)
  {
    pIO->putString(OD_T("No GsView available!"));
    return;
  }
  OdGiTransientManagerPtr pTM = ::odgiGetTransientManager(pView->device());
  if (pTM.isNull())
  {
    pIO->putString(OD_T("No Transient Manager available!"));
    return;
  }

  int mode = pIO->getInt(OD_T("Enter mode:"));
  if (mode >= OdGiTransientManager::kOdGiDrawingModeCount || mode < 0) mode = OdGiTransientManager::kOdGiDrawingModeCount - 1;
  int subMode = pIO->getInt(OD_T("Enter subMode:"));

  pTM->eraseTransients((OdGiTransientManager::OdGiTransientDrawingMode)mode, subMode, OdUInt32Array());
}

void _FreeTransientMode_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();
  OdDbDatabasePtr pDb = pDbCmdCtx->database();

  OdGsView *pView = _GetGsView_func(pDb);
  if (pView == NULL)
  {
    pIO->putString(OD_T("No GsView available!"));
    return;
  }
  OdGiTransientManagerPtr pTM = ::odgiGetTransientManager(pView->device());
  if (pTM.isNull())
  {
    pIO->putString(OD_T("No Transient Manager available!"));
    return;
  }

  int mode = pIO->getInt(OD_T("Enter mode:"));
  if (mode >= OdGiTransientManager::kOdGiDrawingModeCount || mode < 0) mode = OdGiTransientManager::kOdGiDrawingModeCount - 1;
  OdInt32 subMode = pIO->getInt(OD_T("Enter subMode:"));

  OdInt32 nRet = pTM->getFreeSubDrawingMode((OdGiTransientManager::OdGiTransientDrawingMode)mode, subMode, OdUInt32Array());
  pIO->putString(OdString().format(OD_T("Returned: %d (subMode: %d)\n"), nRet, subMode));
}

void _UpdateTransientMode_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();
  OdDbDatabasePtr pDb = pDbCmdCtx->database();

  OdGsView *pView = _GetGsView_func(pDb);
  if (pView == NULL)
  {
    pIO->putString(OD_T("No GsView available!"));
    return;
  }
  OdGiTransientManagerPtr pTM = ::odgiGetTransientManager(pView->device());
  if (pTM.isNull())
  {
    pIO->putString(OD_T("No Transient Manager available!"));
    return;
  }

  OdUInt32 nTransients = _Transients_Array.size();
  for (OdUInt32 nTransient = 0; nTransient < nTransients; nTransient++)
  {
    OdDbEntityPtr pEnt = OdDbEntity::cast(_Transients_Array[nTransient]);
    if (!pEnt.isNull())
    {
      pEnt->setColorIndex(pEnt->colorIndex() + 1);
      pTM->updateTransient(pEnt, OdUInt32Array());
    }
  }
}

////// eof Transient Manager

void _LinesByCurvePath_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbSelectionSetPtr pSSet = pIO->select(OD_T("Select curve to test"));
  int nCount = pIO->getInt(OD_T("Set count of output lines: "));
  double dLength = pIO->getDist(OD_T("Set line length: "));
  OdDbSelectionSetIteratorPtr pIter = pSSet->newIterator();
  for (; !pIter->done(); pIter->next())
  {
    OdDbCurvePtr pCurve = OdDbCurve::cast(pIter->objectId().openObject());
    if (pCurve.isNull())
      continue;
    double startParam, endParam;
    pCurve->getStartParam(startParam);
    pCurve->getEndParam(endParam);
    double delta = (endParam - startParam) / (nCount - 1);
    for (int i = 0; i < nCount; i++)
    {
      double param = startParam + delta * i;
      OdGePoint3d curvePt;
      pCurve->getPointAtParam(param, curvePt);
      OdGeVector3d perp;
      //pCurve->getSecondDeriv(param, perp);
      pCurve->getFirstDeriv(param, perp);
      perp = perp.perpVector();
      if (!perp.isZeroLength())
      {
        OdDbLinePtr pLine = OdDbLine::createObject();
        pLine->setStartPoint(curvePt - perp.normal() * (dLength * 0.5));
        pLine->setEndPoint(curvePt + perp.normal() * (dLength * 0.5));
        OdDbBlockTableRecord::cast(pDb->getActiveLayoutBTRId().safeOpenObject(OdDb::kForWrite))->appendOdDbEntity(pLine);
      }
    }
  }
}

#include "DbMline.h"
#include "DbPoint.h"

static void _makePt(OdDbDatabase *pDb, const OdGePoint3d *pt, OdUInt16 color)
{
  pDb->setPDMODE(99);
  OdDbPointPtr pPt = OdDbPoint::createObject();
  pPt->setPosition(*pt);
  pPt->setColorIndex(color);
  OdDbBlockTableRecord::cast(pDb->getActiveLayoutBTRId().safeOpenObject(OdDb::kForWrite))->appendOdDbEntity(pPt);
}

void _MLineGetClosestPointTo_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbSelectionSetPtr pSSet = pIO->select(OD_T("Select MLine to test"));
  OdGePoint3d wPt = pIO->getPoint(OD_T("Select point"));
  OdDbSelectionSetIteratorPtr pIter = pSSet->newIterator();
  for (; !pIter->done(); pIter->next())
  {
    OdDbMlinePtr pCurve = OdDbMline::cast(pIter->objectId().openObject());
    if (pCurve.isNull())
      continue;
    OdGePoint3d ptOut;
    if (pCurve->getClosestPointTo(wPt, ptOut, false, false) == eOk)
      _makePt(pDb, &ptOut, 4);
    if (pCurve->getClosestPointTo(wPt, ptOut, true, false) == eOk)
      _makePt(pDb, &ptOut, 5);
    if (pCurve->getClosestPointTo(wPt, ptOut, false, true) == eOk)
      _makePt(pDb, &ptOut, 6);
    if (pCurve->getClosestPointTo(wPt, ptOut, true, true) == eOk)
      _makePt(pDb, &ptOut, 8);
  }
}

void _MLineGetClosestPointToProj_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbSelectionSetPtr pSSet = pIO->select(OD_T("Select MLine to test"));
  OdGePoint3d wPt = pIO->getPoint(OD_T("Select point"));
  OdDbSelectionSetIteratorPtr pIter = pSSet->newIterator();
  OdGeVector3d viewdir = pDb->getSysVar(OD_T("VIEWDIR"))->getVector3d();
  for (; !pIter->done(); pIter->next())
  {
    OdDbMlinePtr pCurve = OdDbMline::cast(pIter->objectId().openObject());
    if (pCurve.isNull())
      continue;
    OdGePoint3d ptOut;
    if (pCurve->getClosestPointTo(wPt, viewdir, ptOut, false, false) == eOk)
      _makePt(pDb, &ptOut, 4);
    if (pCurve->getClosestPointTo(wPt, viewdir, ptOut, true, false) == eOk)
      _makePt(pDb, &ptOut, 5);
    if (pCurve->getClosestPointTo(wPt, viewdir, ptOut, false, true) == eOk)
      _makePt(pDb, &ptOut, 6);
    if (pCurve->getClosestPointTo(wPt, viewdir, ptOut, true, true) == eOk)
      _makePt(pDb, &ptOut, 8);
  }
}

void _MLineAppendSeg_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbSelectionSetPtr pSSet = pIO->select(OD_T("Select MLine to test"), OdEd::kSelSingleEntity);
  if (pSSet->numEntities() == 0)
  {
    pIO->putString(OD_T("Nothing selected."));
    return;
  }
  OdDbMlinePtr pMLine = OdDbMline::cast(OdDbObjectId(pSSet->newIterator()->id()).safeOpenObject(OdDb::kForWrite));
  if (pMLine.isNull())
  {
    pIO->putString(OD_T("Invalid object selected."));
    return;
  }
  bool bStop = false;
  do
  {
    OdGePoint3d pt;
    try
    {
      pt = pIO->getPoint(OD_T("Select point:"));
    }
    catch (const OdEdCancel &)
    {
      pIO->putString(OD_T("Quit."));
      bStop = true;
    }
    if (!bStop)
    {
      pMLine->appendSeg(pt);
      pMLine->downgradeOpen();
      pMLine->database()->flushGraphics();
      pMLine->upgradeOpen();
    }
  } while (!bStop);
}

void _MLineRemoveLastSeg_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbSelectionSetPtr pSSet = pIO->select(OD_T("Select MLine to test"), OdEd::kSelSingleEntity);
  if (pSSet->numEntities() == 0)
  {
    pIO->putString(OD_T("Nothing selected."));
    return;
  }
  OdDbMlinePtr pMLine = OdDbMline::cast(OdDbObjectId(pSSet->newIterator()->id()).safeOpenObject(OdDb::kForWrite));
  if (pMLine.isNull())
  {
    pIO->putString(OD_T("Invalid object selected."));
    return;
  }
  OdGePoint3d pt;
  pMLine->removeLastSeg(pt);
}

void _MLineSetScale_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbSelectionSetPtr pSSet = pIO->select(OD_T("Select MLine to test"), OdEd::kSelSingleEntity);
  if (pSSet->numEntities() == 0)
  {
    pIO->putString(OD_T("Nothing selected."));
    return;
  }
  OdDbMlinePtr pMLine = OdDbMline::cast(OdDbObjectId(pSSet->newIterator()->id()).safeOpenObject(OdDb::kForWrite));
  if (pMLine.isNull())
  {
    pIO->putString(OD_T("Invalid object selected."));
    return;
  }
  OdString s;
  s.format(L"Enter new multiline scale <%g>: ", pMLine->scale());
  pMLine->setScale(pIO->getReal(s, OdEd::kInpNonNeg|OdEd::kInpNonZero, pMLine->scale()));
}

#include "DbFace.h"

void _getGeomExtents_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();
  OdDbDatabasePtr pDb = pDbCmdCtx->database();

  OdDbSelectionSetPtr pSel = pIO->select(OD_T("Select entity for test"), OdEd::kSelSingleEntity);
  if (pSel->numEntities() == 0)
  {
    pIO->putString(OD_T("Nothing selected."));
    return;
  }

  OdDbSelectionSetIteratorPtr pIter = pSel->newIterator();
  OdDbObjectId objId = pIter->objectId();
  OdDbEntityPtr pEnt = objId.openObject();

  OdGeExtents3d bestFit;
  bool bBlockRef = pEnt->isKindOf(OdDbBlockReference::desc());
  if (!bBlockRef || OdDbBlockReference::cast(pEnt)->geomExtentsBestFit(bestFit) != eOk)
    bBlockRef = false;

  OdGeExtents3d extents;
  if (pEnt->getGeomExtents(extents) == eOk)
  {
    if (extents.isValidExtents())
    {
      OdString sFormat;
      sFormat.format(OD_T("EXTMIN: %.4f, %.4f, %.4f"), extents.minPoint().x, extents.minPoint().y, extents.minPoint().z);
      pIO->putString(sFormat);
      sFormat.format(OD_T("EXTMAX: %.4f, %.4f, %.4f"), extents.maxPoint().x, extents.maxPoint().y, extents.maxPoint().z);
      pIO->putString(sFormat);
      if (bBlockRef)
      {
        sFormat.format(OD_T("EXTMIN (BestFit): %.4f, %.4f, %.4f"), bestFit.minPoint().x, bestFit.minPoint().y, bestFit.minPoint().z);
        pIO->putString(sFormat);
        sFormat.format(OD_T("EXTMAX (BestFit): %.4f, %.4f, %.4f"), bestFit.maxPoint().x, bestFit.maxPoint().y, bestFit.maxPoint().z);
        pIO->putString(sFormat);
      }
      OdGePoint3d points[8];
      OdGeVector3d extHalf = extents.maxPoint() - extents.center();
      points[0].set(-extHalf.x, -extHalf.y, -extHalf.z);
      points[1].set( extHalf.x, -extHalf.y, -extHalf.z);
      points[2].set( extHalf.x,  extHalf.y, -extHalf.z);
      points[3].set(-extHalf.x,  extHalf.y, -extHalf.z);
      points[4].set(-extHalf.x, -extHalf.y,  extHalf.z);
      points[5].set( extHalf.x, -extHalf.y,  extHalf.z);
      points[6].set( extHalf.x,  extHalf.y,  extHalf.z);
      points[7].set(-extHalf.x,  extHalf.y,  extHalf.z);
      OdDbBlockTableRecordPtr pRec = OdDbBlockTableRecord::createObject();
      pRec->setPathName(OdString::kEmpty);
      pRec->setName(OD_T("*U"));
      OdDbBlockTable::cast(pDb->getBlockTableId().openObject(OdDb::kForWrite))->add(pRec);
      OdUInt8 colorId = 7;
      //if (bBlockRef)
      //  colorId = 6;
      OdDbFacePtr tmpFace;
#define addFace(p0, p1, p2, p3) \
      tmpFace = OdDbFace::createObject(); \
      tmpFace->setVertexAt(0, p0); \
      tmpFace->setVertexAt(1, p1); \
      tmpFace->setVertexAt(2, p2); \
      tmpFace->setVertexAt(3, p3); \
      tmpFace->setColorIndex(colorId); \
      pRec->appendOdDbEntity(tmpFace)
      addFace(points[0], points[1], points[2], points[3]);
      addFace(points[4], points[5], points[6], points[7]);
      addFace(points[0], points[1], points[5], points[4]);
      addFace(points[1], points[2], points[6], points[5]);
      addFace(points[2], points[3], points[7], points[6]);
      addFace(points[3], points[0], points[4], points[7]);
      if (bBlockRef)
      {
        colorId = 4;
        extHalf = bestFit.maxPoint() - bestFit.center();
        points[0].set(-extHalf.x, -extHalf.y, -extHalf.z);
        points[1].set( extHalf.x, -extHalf.y, -extHalf.z);
        points[2].set( extHalf.x,  extHalf.y, -extHalf.z);
        points[3].set(-extHalf.x,  extHalf.y, -extHalf.z);
        points[4].set(-extHalf.x, -extHalf.y,  extHalf.z);
        points[5].set( extHalf.x, -extHalf.y,  extHalf.z);
        points[6].set( extHalf.x,  extHalf.y,  extHalf.z);
        points[7].set(-extHalf.x,  extHalf.y,  extHalf.z);
        OdGeVector3d diffCtr = bestFit.center() - extents.center();
        for (int i = 0; i < 8; i++) points[i] += diffCtr;
        addFace(points[0], points[1], points[2], points[3]);
        addFace(points[4], points[5], points[6], points[7]);
        addFace(points[0], points[1], points[5], points[4]);
        addFace(points[1], points[2], points[6], points[5]);
        addFace(points[2], points[3], points[7], points[6]);
        addFace(points[3], points[0], points[4], points[7]);
      }
#undef addFace
      OdDbBlockReferencePtr pRef = OdDbBlockReference::createObject();
      pRef->setBlockTableRecord(pRec->id());
      pRef->setPosition(extents.center());
      pRef->setDatabaseDefaults(pDb);
      OdDbBlockTableRecordPtr pSpace = OdDbBlockTableRecord::cast(pDb->getActiveLayoutBTRId().openObject(OdDb::kForWrite));
      pSpace->appendOdDbEntity(pRef);

      pIO->putString(OD_T("Completed."));

      ::odedRegCmds()->executeCommand(L"REGEN", pCmdCtx);
    }
    else
    {
      pIO->putString(OD_T("Invalid extents."));
    }
  }
  else
  {
    pIO->putString(OD_T("getGeomExtents() failed."));
  }
}

#if 0 // TxtExp command added to ExCommands module instead
#include "Gi/GiConveyorNode.h"
#include "Gi/GiConveyorEntryPoint.h"

class TextContourExplode : public OdStaticRxObject<OdGiConveyorNode>, private OdGiEmptyGeometry,
                           public OdGiConveyorInput, public OdGiConveyorOutput
{
  bool m_bExplode;
  OdDbDatabasePtr m_pDb;
//  OdGiContext *m_pCtx;
  OdGiBaseVectorizer *m_pVectorizer;
  int m_iplCount;
protected:
  void addSourceNode(OdGiConveyorOutput& sourceNode)
  {
    sourceNode.setDestGeometry(*this);
  }
  void removeSourceNode(OdGiConveyorOutput& /*sourceNode*/)
  {
  }
  void setDestGeometry(OdGiConveyorGeometry& destGeometry)
  {
  }
  OdGiConveyorGeometry& destGeometry() const
  {
    return OdGiEmptyGeometry::kVoid;
  }
  OdGiConveyorInput& input() { return *this; }
  OdGiConveyorOutput& output() { return *this; }
public:
  TextContourExplode(const OdDbDatabase *pDb, int iplCount)
    : m_bExplode(false)
    , m_pDb(pDb)
    , m_pVectorizer(NULL)
    , m_iplCount(iplCount)
  {
  }
  void setVectorizer(OdGiBaseVectorizer *pVect) { m_pVectorizer = pVect; }

  void appendEntity(OdDbEntity * pEnt)
  {
    // Set Properties
    const OdGiSubEntityTraitsData& traits = m_pVectorizer->effectiveTraits();

    pEnt->setLayer(traits.layer());

    //Color
    OdCmColor cmColor;
    cmColor.setColor(traits.trueColor().color());
    pEnt->setColor(cmColor);

    pEnt->setLinetype(traits.lineType(), false);
    pEnt->setLinetypeScale(traits.lineTypeScale(), false);
    pEnt->setLineWeight(traits.lineWeight(), false);
    pEnt->setPlotStyleName(traits.plotStyleNameType(), traits.plotStyleNameId(), false);
    pEnt->setMaterial(traits.material(), false);
    pEnt->setMaterialMapper(traits.mapper(), false);

    double dThickness = traits.thickness();
    if (!OdZero(dThickness))
    {
      OdDbPolylinePtr pPline = OdDbPolyline::cast(pEnt);
      if (!pPline.isNull())
      {
        pPline->setThickness(dThickness);
      }
      else
      {
        OdDb2dPolylinePtr pPline = OdDb2dPolyline::cast(pEnt);
        if (!pPline.isNull())
        {
          pPline->setThickness(dThickness);
        }
        else
        {
          OdDbCirclePtr pCirc = OdDbCircle::cast(pEnt);
          if (!pCirc.isNull())
          {
            pCirc->setThickness(dThickness);
          }
          else
          {
            OdDbLinePtr pLine = OdDbLine::cast(pEnt);
            if (!pLine.isNull())
            {
              pLine->setThickness(dThickness);
            }
            else
            {
              OdDbArcPtr pArc = OdDbArc::cast(pEnt);
              if (!pArc.isNull())
              {
                pArc->setThickness(dThickness);
              }
            }
          }
        }
      }
    }
    OdDbBlockTableRecord::cast(m_pDb->getActiveLayoutBTRId().openObject(OdDb::kForWrite))->appendOdDbEntity(pEnt);
  }

  void polylineProc(
    OdInt32 numPoints, const OdGePoint3d* vertexList,
    const OdGeVector3d* pNormal,
    const OdGeVector3d* pExtrusion, OdGsMarker baseSubEntMarker)
  {
//    if (m_bExplode)
    {
      OdDbEntityPtr pEnt;
      if (numPoints == 2)
      {
        OdDbLinePtr pLine = OdDbLine::createObject();
        pLine->setStartPoint(*vertexList++);
        pLine->setEndPoint(*vertexList);
        if (pNormal)
        {
          pLine->setNormal(*pNormal);
        }
        pEnt = pLine;
      } else
      {
        bool bClosed = (*vertexList == vertexList[numPoints-1]);
        if (bClosed)
        {
          --numPoints;
        }
        OdGeVector3d vNormal;
        if (pNormal)
          vNormal = *pNormal;
        else if (geCalculateNormal(vertexList, numPoints, &vNormal) != OdGe::kOk)
        {
          ODA_FAIL(); // shoul not be 3d polyline for text
          return;
        }
        OdGeMatrix3d matrix;
        if (vNormal.isParallelTo(OdGeVector3d::kZAxis))
        {
          vNormal = OdGeVector3d::kZAxis;
        }
        matrix.setToWorldToPlane(vNormal);

        OdDbPolylinePtr pLine = OdDbPolyline::createObject();
        pEnt = pLine;

        pLine->setNormal(vNormal);
        OdGePoint3d pnt = matrix * vertexList[0];
        pLine->setElevation(pnt.z);
        matrix.setTranslation(OdGeVector3d(0., 0., -pnt.z));

        for(OdInt32 f=0; f<numPoints; ++f)
        {
          pnt = matrix * vertexList[f];
          ODA_ASSERT(OdZero(pnt.z / 2.));
          pLine->addVertexAt(f, OdGePoint2d(pnt.x, pnt.y));
        }
        if (bClosed)
        {
          pLine->setClosed(true);
        }
      }
      appendEntity(pEnt);
    }
  }

  virtual void circleProc(
    const OdGePoint3d& center, 
    double radius, 
    const OdGeVector3d& normal, 
    const OdGeVector3d* pExtrusion)
  {
//    if (m_bExplode)
    {
      OdDbCirclePtr pCirc = OdDbCircle::createObject();
      pCirc->setCenter(center);
      pCirc->setRadius(radius);
      pCirc->setNormal(normal);
      appendEntity(pCirc);
    }
  }
  void circularArcProc(
    const OdGePoint3d& firstPoint,
    const OdGePoint3d& secondPoint,
    const OdGePoint3d& thirdPoint,
    OdGiArcType arcType,
    const OdGeVector3d* pExtrusion)
  {
//    if (m_bExplode)
    {
      OdGeCircArc3d arc(firstPoint, secondPoint, thirdPoint);
      OdDbArcPtr pArc = OdDbArc::createObject();
      pArc->setFromOdGeCurve(arc, (OdGeVector3d*)pExtrusion);
      appendEntity(pArc);
    }
  }

  void ellipArcProc(const OdGeEllipArc3d& ellipse,
                                     const OdGePoint3d* , OdGiArcType , const OdGeVector3d* pExtrusion)
  {
//    if (m_bExplode)
    {
      OdDbEntityPtr pEnt;
      if (ellipse.isCircular())
      {
        OdGeCircArc3d arc(ellipse.center(), ellipse.normal(), ellipse.majorAxis(), ellipse.majorRadius(),
          ellipse.startAng(), ellipse.endAng());
        if (arc.isClosed())
        {
          OdDbCirclePtr pCirc = OdDbCircle::createObject();
          pCirc->setFromOdGeCurve(arc);
          pEnt = pCirc;
        }
        else
        {
          OdDbArcPtr pArc = OdDbArc::createObject();
          pArc->setFromOdGeCurve(arc, (OdGeVector3d*)pExtrusion);
          pEnt = pArc;
        }
      }
      else
      {
        OdDbEllipsePtr pArc = OdDbEllipse::createObject();
        pArc->setFromOdGeCurve(ellipse, (OdGeVector3d*)pExtrusion);
        pEnt = pArc;
      }
      appendEntity(pEnt);
    }
  }

  void shellProc(
    OdInt32 /*numVertices*/,
    const OdGePoint3d* vertexList,
    OdInt32 faceListSize,
    const OdInt32* faceList,
    const OdGiEdgeData* /*pEdgeData*/ = 0,
    const OdGiFaceData* /*pFaceData*/ = 0,
    const OdGiVertexData* /*pVertexData*/ = 0)
  {
    if (m_bExplode)
    {
      OdInt32 it = 0;
      while (it < faceListSize)
      {
        OdDb2dPolylinePtr pPl = OdDb2dPolyline::createObject();
        for (OdInt32 id = 1; id < abs(faceList[it]) + 1; id++)
        {
          OdDb2dVertexPtr pVert = OdDb2dVertex::createObject();
          pVert->setPosition(vertexList[faceList[id + it]]);
          pPl->appendVertex(pVert);
        }
        OdDbBlockTableRecord::cast(m_pDb->getActiveLayoutBTRId().openObject(OdDb::kForWrite))->appendOdDbEntity(pPl);
        it += abs(faceList[it]) + 1;
      }
    }
  }
  enum pdFlags
  {
    kpdCloseFigure = 0x01,
    kpdLineTo      = 0x02,
    kpdBezierTo    = 0x04,
    kpdMoveTo      = 0x06
  };
  void ttfPolyDrawProc(OdInt32 numVertices, const OdGePoint3d* vertexList,
                       OdInt32 /*faceListSize*/, const OdInt32* /*faceList*/,
                       const OdUInt8* pBezierTypes, const OdGiFaceData* /*pFaceData*/ = 0)
  {
//    if (m_bExplode)
    {
      OdGeVector3d vNormal;
      if (geCalculateNormal(vertexList, numVertices, &vNormal) != OdGe::kOk)
      {
        ODA_FAIL(); // shoul not be 3d polyline for text
        return;
      }
      OdGeMatrix3d matrix;
      if (vNormal.isParallelTo(OdGeVector3d::kZAxis))
      {
        vNormal = OdGeVector3d::kZAxis;
      }
      matrix.setToWorldToPlane(vNormal);
      OdGePoint3d pnt = matrix * vertexList[0];
      double dElevation = pnt.z;
      matrix.setTranslation(OdGeVector3d(0., 0., -dElevation));
      int nVerts = 0;

      OdDbPolylinePtr pPl;
      int bzCnt = 0;
      OdGePoint3d bzSpl[4];
      for (OdInt32 i = 0; i < numVertices; i++)
      {
        if (pBezierTypes[i] == kpdMoveTo)
        {
          pPl = OdDbPolyline::createObject();
          pPl->setNormal(vNormal);
          pPl->setElevation(dElevation);
          nVerts = 0;
          pnt = matrix * vertexList[i];
          ODA_ASSERT(OdZero(pnt.z / 2.));
          pPl->addVertexAt(nVerts++, OdGePoint2d(pnt.x, pnt.y));
        }
        else
        {
          if (GETBIT(pBezierTypes[i], kpdLineTo))
          {
            pnt = matrix * vertexList[i];
            ODA_ASSERT(OdZero(pnt.z / 2.));
            pPl->addVertexAt(nVerts++, OdGePoint2d(pnt.x, pnt.y));
          }
          else if (GETBIT(pBezierTypes[i], kpdBezierTo))
          {
            bzCnt++;
            bzSpl[bzCnt] = vertexList[i];
            if (bzCnt == 3)
            {
              const int nSegs = m_iplCount;
              // This is sequence of 3 bezierTo points + previous point, so when we collect 3rd bezierTo,
              // we could contruct cubic spline based on 4 points.
              pPl->getEndPoint(bzSpl[0]);
              double d = 1. / nSegs;
              for (int n = 1; n < nSegs; n++)
              {
                double t = d * n;
                double u = 1.0 - t;
                double tt = t * t, uu = u * u;
                double uuu = uu * u, ttt = tt * t;
                double uut3 = uu * t * 3, utt3 = u * tt * 3;
            
                pnt = matrix * OdGePoint3d(
                          bzSpl[0].x * uuu + bzSpl[1].x * uut3 + bzSpl[2].x * utt3 + bzSpl[3].x * ttt,
                          bzSpl[0].y * uuu + bzSpl[1].y * uut3 + bzSpl[2].y * utt3 + bzSpl[3].y * ttt,
                          bzSpl[0].z * uuu + bzSpl[1].z * uut3 + bzSpl[2].z * utt3 + bzSpl[3].z * ttt);
                ODA_ASSERT(OdZero(pnt.z / 2.));
                pPl->addVertexAt(nVerts++, OdGePoint2d(pnt.x, pnt.y));
              }

              pnt = matrix * vertexList[i];
              ODA_ASSERT(OdZero(pnt.z / 2.));
              pPl->addVertexAt(nVerts++, OdGePoint2d(pnt.x, pnt.y));
              bzCnt = 0;
            }
          }
        }
        if (GETBIT(pBezierTypes[i], kpdCloseFigure))
        {

          appendEntity(pPl);

          pPl = OdDbPolyline::createObject();
          pPl->setNormal(vNormal);
          pPl->setElevation(dElevation);
          nVerts = 0;

          OdGePoint3d ptPos(matrix * vertexList[i]);
          ODA_ASSERT(OdZero(ptPos.z / 2.));
          pPl->addVertexAt(nVerts++, OdGePoint2d(ptPos.x, ptPos.y));
        }
      }
    }
  }
  virtual void textProc(
    const OdGePoint3d& position,
    const OdGeVector3d& direction, 
    const OdGeVector3d& upVector,
    const OdChar* msg, 
    OdInt32 numBytes, 
    bool raw, 
    const OdGiTextStyle* pTextStyle,
    const OdGeVector3d* /*pExtrusion*/ = 0)
  {
    m_bExplode = true;
    m_pVectorizer->context()->drawText(this, position, direction, upVector, msg, numBytes, raw, pTextStyle, 0);
    m_bExplode = false;
  }
};
class OdGiDrawTextForContourExplode : public OdGiBaseVectorizer
                                    , public OdGiContextForDbDatabase
{
  public:
    ODRX_USING_HEAP_OPERATORS(OdGiBaseVectorizer);

    OdGiDrawTextForContourExplode()
    {
      OdGiBaseVectorizer::m_flags |= (kDrawInvisibleEnts|kDrawLayerOff|kDrawLayerFrozen);
      setContext(this); 
      m_pModelToEyeProc->setDrawContext(OdGiBaseVectorizer::drawContext());
    }
    void attachOutputNode(OdGiConveyorNode *pNode)
    {
      pNode->input().addSourceNode(output());
    }
    OdGiRegenType regenType() const
    {
      return kOdGiForExplode;
    }
    void draw(const OdGiDrawable* pDrawable)
    {
      pDrawable->setAttributes(this);
      pDrawable->worldDraw(this);
    }
    const OdGiSubEntityTraitsData& effectiveTraits() const
    {
      return m_entityTraitsData;
    }
    void affectTraits(const OdGiSubEntityTraitsData* , OdGiSubEntityTraitsData& ) const
    {
    }
};
void _textContourExplode_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbSelectionSetPtr pSel = pIO->select(OD_T("Select entity for test"), OdEd::kSelSingleEntity);
  if (pSel->numEntities() == 0)
  {
    pIO->putString(OD_T("Nothing selected."));
    return;
  }
  OdDbSelectionSetIteratorPtr pIter = pSel->newIterator();
  int typeValue = pIO->getKeyword(OD_T("Use [Default] render or TTF [Polydraw] mode?"), OD_T("Default Polydraw"));
  bool bPolyDraw = typeValue == 1;
  int iplCount = 10;
  if (bPolyDraw)
  {
    iplCount = pIO->getInt(OD_T("Set count of points for tesselate cubic bezier curves <10>: "), OdEd::kInpDefault, iplCount);
  }
  TextContourExplode textExp(pDb, iplCount);
  OdStaticRxObject<OdGiDrawTextForContourExplode> vect;
  vect.setDatabase(pDb);
  textExp.setVectorizer(&vect);
  vect.attachOutputNode(&textExp);
  vect.setTtfPolyDrawMode(true);

  for ( ; !pIter->done(); pIter->next() )
  {
    OdDbEntityPtr pEnt = OdDbEntity::cast( pIter->objectId().openObject() );
    vect.draw(pEnt);
    pEnt->upgradeOpen();
    pEnt->erase();
  }
  pIO->putString(OD_T("Completed."));
//  ::odedRegCmds()->executeCommand(L"REGEN", pCmdCtx);
}
#endif

#if 1 // ExClipTest command
#include "Gi/GiConveyorNode.h" // include for OdGiConveyorNode class
#include "Gi/GiConveyorEntryPoint.h" // include for OdGiConveyorInput and OdGiConveyorOutput classes
#include "Gi/GiGeometrySimplifier.h" // include for OdGiGeometrySimplifier class
#include "Gi/GiOrthoClipperEx.h" // include clipper
#include "Gi/GiDrawableImpl.h" // required for ExTrackers
#include "../Extensions/ExServices/ExTrackers.h" // include for PolygonSelectionTracker tracker
#include "OdStack.h" // For saved trackers container

// This class accumulate clipped geometry after clipper conveyor node processing and add clipped geometry back
// into database.
class ClippedGeometryCollector : public OdStaticRxObject<OdGiConveyorNode>, public OdGiGeometrySimplifier,
                                 public OdGiConveyorInput, public OdGiConveyorOutput
{
  OdDbDatabasePtr m_pDb;
  OdGiBaseVectorizer *m_pVectorizer;
protected:
  void addSourceNode(OdGiConveyorOutput& sourceNode)
  {
    sourceNode.setDestGeometry(*this);
  }
  void removeSourceNode(OdGiConveyorOutput& /*sourceNode*/)
  {
  }
  void setDestGeometry(OdGiConveyorGeometry& destGeometry)
  {
  }
  OdGiConveyorGeometry& destGeometry() const
  {
    return OdGiEmptyGeometry::kVoid;
  }
  OdGiConveyorInput& input() { return *this; }
  OdGiConveyorOutput& output() { return *this; }
public:
  ClippedGeometryCollector(const OdDbDatabase *pDb, OdGiBaseVectorizer *pVect)
    : m_pDb(pDb)
    , m_pVectorizer(pVect)
  {
  }

  void setDrawContext(OdGiConveyorContext* pDrawCtx)
  {
     OdGiGeometrySimplifier::setDrawContext(pDrawCtx);
     m_renderMode = OdGsView::kGouraudShadedWithWireframe;
  }

  void appendEntity(OdDbEntity * pEnt)
  {
    // Set Properties
    const OdGiSubEntityTraitsData& traits = m_pVectorizer->effectiveTraits();

    pEnt->setLayer(traits.layer());

    //Color
    OdCmColor cmColor;
    cmColor.setColor(traits.trueColor().color());
    pEnt->setColor(cmColor);
    // Other properties
    pEnt->setLinetype(traits.lineType(), false);
    pEnt->setLinetypeScale(traits.lineTypeScale(), false);
    pEnt->setLineWeight(traits.lineWeight(), false);
    pEnt->setPlotStyleName(traits.plotStyleNameType(), traits.plotStyleNameId(), false);
    pEnt->setMaterial(traits.material(), false);
    pEnt->setMaterialMapper(traits.mapper(), false);
    // Insert entity into current block
    OdDbBlockTableRecord::cast(m_pDb->getActiveLayoutBTRId().openObject(OdDb::kForWrite))->appendOdDbEntity(pEnt);
  }

  void polylineOut(OdInt32 numPoints, const OdGePoint3d* vertexList)
  {
    OdDbEntityPtr pEnt;
    if (numPoints == 2)
    { // Output as OdDbLine entity
      OdDbLinePtr pLine = OdDbLine::createObject();
      pLine->setStartPoint(*vertexList++);
      pLine->setEndPoint(*vertexList);
      pEnt = pLine;
    }
    else
    { // Output as OdDbPolyline entity
      bool bClosed = (*vertexList == vertexList[numPoints-1]);
      if (bClosed)
        --numPoints;
      OdGeVector3d vNormal;
      bool b3dPolyline = false;
      if (geCalculateNormal(vertexList, numPoints, &vNormal) != OdGe::kOk)
        b3dPolyline = true;
      OdGeMatrix3d matrix;
      if (vNormal.isParallelTo(OdGeVector3d::kZAxis))
        vNormal = OdGeVector3d::kZAxis;
      matrix.setToWorldToPlane(vNormal);
      if (!b3dPolyline)
      {
        OdDbPolylinePtr pLine = OdDbPolyline::createObject();
        pEnt = pLine;
        pLine->setNormal(vNormal);
        OdGePoint3d pnt = matrix * vertexList[0];
        pLine->setElevation(pnt.z);
        matrix.setTranslation(OdGeVector3d(0., 0., -pnt.z));
        for (OdInt32 f = 0; f < numPoints; ++f)
        {
          pnt = matrix * vertexList[f];
          ODA_ASSERT(OdZero(pnt.z / 2.));
          pLine->addVertexAt(f, OdGePoint2d(pnt.x, pnt.y));
        }
        if (bClosed)
          pLine->setClosed(true);
      }
      else
      {
        // @@@TODO: implement for 3d polylines
        ODA_FAIL();
      }
    }
    appendEntity(pEnt);
  }

  void polygonOut(OdInt32 nbPoints, const OdGePoint3d* pVertexList, const OdGeVector3d* pNormal)
  {
    if (nbPoints <= 4)
    { // Output triangles and quarilaterals as OdDbSolid entity
      OdDbSolidPtr pSolid = OdDbSolid::createObject();
      pSolid->setPointAt(0, pVertexList[0]);
      pSolid->setPointAt(1, pVertexList[1]);
      pSolid->setPointAt(2, pVertexList[2]);
      if (nbPoints == 4)
        pSolid->setPointAt(3, pVertexList[3]);
      else
        pSolid->setPointAt(3, pVertexList[2]);
      if (pNormal)
        pSolid->setNormal(*pNormal);
      appendEntity(pSolid);
    }
    else
    {
      ODA_FAIL();
    }
  }

  virtual void circleProc(
    const OdGePoint3d& center, 
    double radius, 
    const OdGeVector3d& normal, 
    const OdGeVector3d* pExtrusion)
  {
    OdDbCirclePtr pCirc = OdDbCircle::createObject();
    pCirc->setCenter(center);
    pCirc->setRadius(radius);
    pCirc->setNormal(normal);
    appendEntity(pCirc);
  }
  void circularArcProc(
    const OdGePoint3d& firstPoint,
    const OdGePoint3d& secondPoint,
    const OdGePoint3d& thirdPoint,
    OdGiArcType arcType,
    const OdGeVector3d* pExtrusion)
  {
    OdGeCircArc3d arc(firstPoint, secondPoint, thirdPoint);
    OdDbArcPtr pArc = OdDbArc::createObject();
    pArc->setFromOdGeCurve(arc, (OdGeVector3d*)pExtrusion);
    appendEntity(pArc);
  }

  void ellipArcProc(const OdGeEllipArc3d& ellipse,
                    const OdGePoint3d* , OdGiArcType , const OdGeVector3d* pExtrusion)
  {
    OdDbEntityPtr pEnt;
    if (ellipse.isCircular())
    {
      OdGeCircArc3d arc(ellipse.center(), ellipse.normal(), ellipse.majorAxis(), ellipse.majorRadius(),
        ellipse.startAng(), ellipse.endAng());
      if (arc.isClosed())
      {
        OdDbCirclePtr pCirc = OdDbCircle::createObject();
        pCirc->setFromOdGeCurve(arc);
        pEnt = pCirc;
      }
      else
      {
        OdDbArcPtr pArc = OdDbArc::createObject();
        pArc->setFromOdGeCurve(arc, (OdGeVector3d*)pExtrusion);
        pEnt = pArc;
      }
    }
    else
    {
      OdDbEllipsePtr pArc = OdDbEllipse::createObject();
      pArc->setFromOdGeCurve(ellipse, (OdGeVector3d*)pExtrusion);
      pEnt = pArc;
    }
    appendEntity(pEnt);
  }
};
// This class accumulate clipped geometry sections after clipper conveyor node processing and add clipped section geometry back
// into database.
class ClippedSectionGeometryCollector : public OdStaticRxObject<OdGiConveyorNode>, public OdGiGeometrySimplifier,
                                        public OdGiConveyorInput, public OdGiConveyorOutput
{
  OdDbDatabasePtr m_pDb;
  OdGiBaseVectorizer *m_pVectorizer;
protected:
  void addSourceNode(OdGiConveyorOutput& sourceNode)
  {
    sourceNode.setDestGeometry(*this);
  }
  void removeSourceNode(OdGiConveyorOutput& /*sourceNode*/)
  {
  }
  void setDestGeometry(OdGiConveyorGeometry& destGeometry)
  {
  }
  OdGiConveyorGeometry& destGeometry() const
  {
    return OdGiEmptyGeometry::kVoid;
  }
  OdGiConveyorInput& input() { return *this; }
  OdGiConveyorOutput& output() { return *this; }
public:
  ClippedSectionGeometryCollector(const OdDbDatabase *pDb, OdGiBaseVectorizer *pVect)
    : m_pDb(pDb)
    , m_pVectorizer(pVect)
  {
  }

  void setDrawContext(OdGiConveyorContext* pDrawCtx)
  {
     OdGiGeometrySimplifier::setDrawContext(pDrawCtx);
     m_renderMode = OdGsView::kGouraudShadedWithWireframe;
  }

  void appendEntity(OdDbEntity * pEnt)
  {
    // Set Properties
    const OdGiSubEntityTraitsData& traits = m_pVectorizer->effectiveTraits();

    pEnt->setLayer(traits.layer());

    //Color
    OdCmColor cmColor;
    cmColor.setRGB(255, 0 ,0);
    pEnt->setColor(cmColor);
    // Other properties
    pEnt->setLinetype(traits.lineType(), false);
    pEnt->setLinetypeScale(traits.lineTypeScale(), false);
    pEnt->setLineWeight(traits.lineWeight(), false);
    pEnt->setPlotStyleName(traits.plotStyleNameType(), traits.plotStyleNameId(), false);
    pEnt->setMaterial(traits.material(), false);
    pEnt->setMaterialMapper(traits.mapper(), false);
    // Insert entity into current block
    OdDbBlockTableRecord::cast(m_pDb->getActiveLayoutBTRId().openObject(OdDb::kForWrite))->appendOdDbEntity(pEnt);
  }

  void polylineOut(OdInt32 numPoints, const OdGePoint3d* vertexList)
  {
    OdDbEntityPtr pEnt;
    if (numPoints == 2)
    { // Output as OdDbLine entity
      OdDbLinePtr pLine = OdDbLine::createObject();
      pLine->setStartPoint(*vertexList++);
      pLine->setEndPoint(*vertexList);
      pEnt = pLine;
    }
    else
    { // Output as OdDbPolyline entity
      bool bClosed = (*vertexList == vertexList[numPoints-1]);
      if (bClosed)
        --numPoints;
      OdGeVector3d vNormal;
      bool b3dPolyline = false;
      if (geCalculateNormal(vertexList, numPoints, &vNormal) != OdGe::kOk)
        b3dPolyline = true;
      OdGeMatrix3d matrix;
      if (vNormal.isParallelTo(OdGeVector3d::kZAxis))
        vNormal = OdGeVector3d::kZAxis;
      
      OdGeTol tol;
      if ( vNormal.length() > tol.equalPoint() * 10. )
        matrix.setToWorldToPlane(vNormal);

      if (!b3dPolyline)
      {
        OdDbPolylinePtr pLine = OdDbPolyline::createObject();
        pEnt = pLine;
        pLine->setNormal(vNormal);
        OdGePoint3d pnt = matrix * vertexList[0];
        pLine->setElevation(pnt.z);
        matrix.setTranslation(OdGeVector3d(0., 0., -pnt.z));
        for (OdInt32 f = 0; f < numPoints; ++f)
        {
          pnt = matrix * vertexList[f];
          ODA_ASSERT(OdZero(pnt.z / 2.));
          pLine->addVertexAt(f, OdGePoint2d(pnt.x, pnt.y));
        }
        if (bClosed)
          pLine->setClosed(true);
      }
      else
      {
        // @@@TODO: implement for 3d polylines
        ODA_FAIL();
      }
    }
    appendEntity(pEnt);
  }

  void polygonOut(OdInt32 nbPoints, const OdGePoint3d* pVertexList, const OdGeVector3d* pNormal)
  {
    if (nbPoints <= 4)
    { // Output triangles and quarilaterals as OdDbSolid entity

      OdDbSolidPtr pSolid = OdDbSolid::createObject();
      pSolid->setPointAt(0, pVertexList[0]);
      pSolid->setPointAt(1, pVertexList[1]);
      pSolid->setPointAt(2, pVertexList[2]);
      if (nbPoints == 4)
        pSolid->setPointAt(3, pVertexList[3]);
      else
        pSolid->setPointAt(3, pVertexList[2]);
      if (pNormal)
        pSolid->setNormal(*pNormal);
      
      appendEntity(pSolid);
    }
    else
    {
      ODA_FAIL();
    }
  }

};
// This is our vectorizer which will be used to initiate clipping process.
class OdGiDrawEntityForClip : public OdGiBaseVectorizer
                            , public OdGiContextForDbDatabase
{
  public:
    ODRX_USING_HEAP_OPERATORS(OdGiBaseVectorizer);

    OdGiDrawEntityForClip()
    {
      OdGiBaseVectorizer::m_flags |= (kDrawInvisibleEnts|kDrawLayerOff|kDrawLayerFrozen);
      setContext(this); 
      m_pModelToEyeProc->setDrawContext(OdGiBaseVectorizer::drawContext());
    }
    void attachOutputNode(OdGiConveyorNode *pNode)
    {
      pNode->input().addSourceNode(output());
    }
    OdGiRegenType regenType() const
    {
      //return kOdGiStandardDisplay;
      return kOdGiRenderCommand;
    }
    void draw(const OdGiDrawable* pDrawable)
    {
      pDrawable->setAttributes(this);
      if (!pDrawable->worldDraw(this))
        pDrawable->viewportDraw(this);
    }
    const OdGiSubEntityTraitsData& effectiveTraits() const
    {
      return m_entityTraitsData;
    }
    void affectTraits(const OdGiSubEntityTraitsData* , OdGiSubEntityTraitsData& ) const
    {
    }
};
// Storage class for graphic trackers. We use it to keep drawn contours visible onto screen until command isn't completed.
class GraphTrackersStorage
{
  protected:
    struct TrackerEntry
    {
      OdEdPointTrackerPtr m_pTracker;
      OdGePoint3dArray m_contourPoints;
    };
    OdStack<TrackerEntry> m_trackers;
    OdGsView *m_pView;
  public:
    GraphTrackersStorage(OdDbDatabase *pDb)
    {
      OdDbObjectPtr pVpObj = pDb->activeViewportId().safeOpenObject();
      OdDbAbstractViewportDataPtr pVp(pVpObj);
      m_pView = pVp->gsView(pVpObj);
    }
    ~GraphTrackersStorage()
    {
      freeTrackers();
    }

    static GraphTrackerBase *asGraphTracker(OdEdPointTracker *pTracker)
    {
      return static_cast<GraphTrackerBase*>(pTracker);
    }

    void addTracker(const OdGePoint3dArray &points, bool bPlaneTracker = false)
    {
      if (!m_pView)
        return;
      m_trackers.push();
      m_trackers.top()->m_contourPoints = points;
      if ( !bPlaneTracker )
        m_trackers.top()->m_pTracker = PolygonSelectionTracker::create(&m_trackers.top()->m_contourPoints);
      else
        m_trackers.top()->m_pTracker = PlaneSelectionTracker::create(&m_trackers.top()->m_contourPoints);
      asGraphTracker(m_trackers.top()->m_pTracker)->addDrawables(m_pView);
      m_trackers.top()->m_pTracker->setValue(points.last());
    }
    void freeTrackers()
    {
      while (!m_trackers.empty())
      {
        asGraphTracker(m_trackers.top()->m_pTracker)->removeDrawables(m_pView);
        m_trackers.pop();
      }
    }
};
// getClipPolygon function is used to get clipping polygons from UI.
static void getClipPolygon(OdDbUserIO* pIO, OdGePoint3dArray &pts)
{
  pts.clear();
  OdEdPointTrackerPtr pTracker = PolygonSelectionTracker::create(&pts);
  for (;;)
  {
    try
    {
      if (pts.size() == 0)
        pts.push_back(pIO->getPoint(OD_T("First polygon point:"), OdEd::kGptNoLimCheck | OdEd::kGptNoUCS | OdEd::kInpThrowEmpty));
      OdGePoint3d newPoint = pIO->getPoint(OD_T("Specify endpoint of line:"),
        OdEd::kGptNoLimCheck | OdEd::kGptNoUCS | OdEd::kInpThrowEmpty,
        0, OD_T("Undo"), pTracker);
      pts.append(newPoint);
      GraphTrackersStorage::asGraphTracker(pTracker.get())->invalidateGsModel();
    }
    catch (const OdEdKeyword& kw)
    {
      if (kw.keywordIndex() == 0)
      {
        if (pts.isEmpty())
          pts.removeLast();
        GraphTrackersStorage::asGraphTracker(pTracker.get())->invalidateGsModel();
      }
    }
    catch (const OdEdEmptyInput)
    {
      break;
    }
  }
}

void _ExClipTestCmd_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();
  OdDbDatabasePtr pDb = pDbCmdCtx->database();

  // Get entity for clip
  OdDbSelectionSetPtr pSel = pIO->select(OD_T("Select entity for clip"), OdEd::kSelSingleEntity);
  if (pSel->numEntities() == 0)
  {
    pIO->putString(OD_T("Nothing selected."));
    return;
  }

  // Collect clipping polygons
  OdGePoint3dArray points;
  OdIntArray polyCounts;
  OdGePoint3dArray polyVertices;
  GraphTrackersStorage savedTrackers(pDb);
  do
  {
    ::getClipPolygon(pIO, points);
    if (!points.empty())
    {
      polyCounts.push_back(points.size());
      polyVertices.insert(polyVertices.end(), points.begin(), points.end());
      savedTrackers.addTracker(points);
    }
  } while (!points.empty());

  const bool bInvert = pIO->getKeyword(OD_T("[Invert] polygon contours or use [Normal] mode?"), OD_T("Invert Normal")) == 0;

  // Contruct vectorization conveyor for entities clipping
  OdStaticRxObject<OdGiDrawEntityForClip> vect; // Create vectorizer
  ClippedGeometryCollector geomCollector(pDb, &vect); // Create geometry collector conveyor node
  geomCollector.setDrawContext(&vect); // Setup our vectorizer conveyor context

  ClippedSectionGeometryCollector sectionGemCollector(pDb, &vect); // Create section geometry collector conveyor node
  sectionGemCollector.setDrawContext(&vect);

  OdGiOrthoClipperExPtr pGeomClipper = OdGiOrthoClipperEx::createObject(); // Create geometry clipper
  pGeomClipper->setDrawContext(&vect); // Setup our vectorizer conveyor context
  vect.setDatabase(pDb); // Set database into vectorizer
  pGeomClipper->output().setDestGeometry(geomCollector); // Attach geometry collector conveyor node to geometry clipper output

  //pGeomClipper->sectionOutput().setDestGeometry(sectionGemCollector); // Attach geometry collector conveyor node to geometry clipper output

  vect.attachOutputNode(pGeomClipper); // Attach clipper to vectorizer output

  // Setup clipping contours
  pGeomClipper->pushClipStage(polyCounts.size(), polyCounts.getPtr(), polyVertices.size(), polyVertices.getPtr(), NULL,
                              (bInvert) ? OdGiOrthoClipperEx::kCSInverted : OdGiOrthoClipperEx::kCSNoFlags);

  // Draw selected objects to clip
  OdDbSelectionSetIteratorPtr pIter = pSel->newIterator();
  for (; !pIter->done(); pIter->next())
  {
    OdDbEntityPtr pEnt = OdDbEntity::cast( pIter->objectId().openObject() );
    vect.draw(pEnt);
   // pGeomClipper->flushSection(true, true);
    pEnt->upgradeOpen();
    pEnt->erase();
  }

  // Remove clipping contours
  pGeomClipper->popClipStage();

  pIO->putString(OD_T("Completed."));
}

bool getPolygonNormal(OdGePoint3dArray &pts, OdGeVector3d& normal)
{
  normal.set(0., 0., 0.);

  OdUInt32 nPoints = pts.size();
  if ( nPoints < 3 )
    return false;

  OdGePoint3d* pPoints = pts.asArrayPtr();

  const OdGePoint3d *pA = NULL, *pB = pPoints  + (nPoints - 2), *pC = pPoints  + (nPoints - 1);
  normal.set(0., 0., 0.);
  
  for (OdUInt32 nPt = 0; nPt < nPoints; nPt++)
  {
    pA = pB; pB = pC; pC = pPoints  + nPt;
    normal.x += pB->y * (pC->z - pA->z);
    normal.y += pB->z * (pC->x - pA->x);
    normal.z += pB->x * (pC->y - pA->y);
  }

   double length = normal.length();
   if (OdNonZero(length))
   {
      normal /= length;
   }
   else
     return false;

   return true;
}

// getClipPlanen function is used to get clipping plane from UI.
static void getClipPlane(OdDbUserIO* pIO, OdGePoint3dArray &pts)
{
  pts.clear();
  OdEdPointTrackerPtr pTracker = PlaneSelectionTracker::create(&pts);
  for (;;)
  {
    try
    {
      if (pts.size() == 0)
        pts.push_back(pIO->getPoint(OD_T("First line point:"), OdEd::kGptNoLimCheck | OdEd::kGptNoUCS | OdEd::kInpThrowEmpty));
      else if ( pts.size() == 1 )
      {
        OdGePoint3d newPoint = pIO->getPoint(OD_T("Specify endpoint of line:"),
          OdEd::kGptNoLimCheck | OdEd::kGptNoUCS | OdEd::kInpThrowEmpty,
          0, OD_T("Undo"), pTracker);
        pts.append(newPoint);
      }
      else if ( pts.size() == 2)
      {
        pts.push_back(pIO->getPoint(OD_T("Specify plane's normal direction:"), OdEd::kGptNoLimCheck | OdEd::kGptNoUCS | OdEd::kInpThrowEmpty, 0, OdString::kEmpty, pTracker));
      }
      GraphTrackersStorage::asGraphTracker(pTracker.get())->invalidateGsModel();

      if ( pts.size() == 3 )
        break;
    }
    catch (const OdEdKeyword& kw)
    {
      if (kw.keywordIndex() == 0)
      {
        if (pts.isEmpty())
          pts.removeLast();
        GraphTrackersStorage::asGraphTracker(pTracker.get())->invalidateGsModel();
      }
    }
    catch (const OdEdEmptyInput)
    {
      break;
    }
  }
}

bool normalizePlaneNormal(OdGePoint3dArray& points)
{
  if (points.size() != 3 )
    return false;

  OdGeTol tol;

  OdGeVector3d currentPseudoNormal(points[2] - points[1]);
  double length = currentPseudoNormal.length();
  if ( length < tol.equalPoint() )
    length = 1.;

  OdGeVector3d polygonNormal;
  if ( getPolygonNormal(points, polygonNormal) )
  {    
    OdGePoint3d* pPoints = points.asArrayPtr();

    const OdGePoint3d * pA = pPoints  + 0;
    const OdGePoint3d * pB = pPoints  + 1;

    OdGeVector3d dirP(pB->x - pA->x, pB->y - pA->y, pB->z - pA->z); 

    OdGeVector3d planeNormal = polygonNormal.crossProduct(dirP);
    planeNormal.normalize();

    planeNormal *= length;

    points[2] = points[1] + planeNormal;
  }
  else
    return false;

  return true;
}

void _ExClipPlaneTestCmd_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();
  OdDbDatabasePtr pDb = pDbCmdCtx->database();

  //1. Get entity for clip
  OdDbSelectionSetPtr pSel = pIO->select(OD_T("Select entity for clip"), OdEd::kSelSingleEntity);
  if (pSel->numEntities() == 0)
  {
    pIO->putString(OD_T("Nothing selected."));
    return;
  }

  //2. Get the choosing mode (via single plane or via multiplanes using polygon)
  bool bSinglePlaneMode = pIO->getKeyword(OD_T("Use [Single] or [Multi via polygon] plane choosing mode?"), OD_T("Single Multi via polygon")) == 0;

  //3. Define the clipping planes via UI
  OdGePoint3dArray points;
  OdIntArray polyCounts;
  OdGePoint3dArray polyVertices;

  if ( bSinglePlaneMode )
  {
    // Collect clipping planes
    GraphTrackersStorage savedTrackers(pDb);
    do
    {
      ::getClipPlane(pIO, points);
      if (!points.empty())
      {
        if ( !::normalizePlaneNormal(points) )
           pIO->putString(OD_T("Incorrect normal direction"));
        else
        {
          polyCounts.push_back(points.size());
          polyVertices.insert(polyVertices.end(), points.begin(), points.end());
          savedTrackers.addTracker(points, true);
        }
      }
    } while (!points.empty());
  }
  else // planes via polygon mode
  {
    // Collect clipping polygons
    GraphTrackersStorage savedTrackers(pDb);
  
    // get polygon from UI
    ::getClipPolygon(pIO, points);
    if (!points.empty())
    {
      polyCounts.push_back(points.size());
      polyVertices.insert(polyVertices.end(), points.begin(), points.end());
      savedTrackers.addTracker(points);
    }
  }

  //4. Get planes from polygon's
  OdGiOrthoClipperEx::ClipPlane* planes = NULL;
  OdInt32 nPlanes = 0;
  if ( bSinglePlaneMode )
  {
    OdUInt32 nPolygons = polyCounts.size();
    planes = new OdGiOrthoClipperEx::ClipPlane[nPolygons];
    nPlanes = nPolygons;

    OdGePoint3d* pPoints = polyVertices.asArrayPtr();

    OdUInt32 nPt = 0;
    for (OdUInt32 nPlg = 0; nPlg < nPolygons; nPlg++)
    {
      OdGeVector3d planeNormal = *(pPoints + nPt + 2) - *(pPoints + nPt + 1);
      planes[nPlg].m_normal = planeNormal.normalize();
      planes[nPlg].m_origin = *(pPoints + nPt);

      nPt += polyCounts[nPlg];
    }
  }
  else
  {
    OdUInt32 nPoints = points.size();
    
    if ( nPoints == 1 )
    {
      pIO->putString(OD_T("It is need to enter at least two points"));
      return;
    }

    OdGeVector3d polygonNormal;
    if ( getPolygonNormal(points, polygonNormal) )
    {
      planes = new OdGiOrthoClipperEx::ClipPlane[nPoints];
      nPlanes = nPoints;
    
      OdGePoint3d* pPoints = points.asArrayPtr();

      for (OdUInt32 nPt = 0; nPt < nPoints-1; nPt++)
      {
        const OdGePoint3d * pA = pPoints  + nPt;
        const OdGePoint3d * pB = pPoints  + nPt + 1;

        OdGeVector3d dirP(pB->x - pA->x, pB->y - pA->y, pB->z - pA->z); 

        OdGeVector3d planeNormal = polygonNormal.crossProduct(dirP);
        planeNormal.normalize();

        planes[nPt].m_normal = planeNormal;
        planes[nPt].m_origin = *pA;
      }

      // last segment
      const OdGePoint3d * pA = pPoints  + nPoints-1;
      const OdGePoint3d * pB = pPoints;

      OdGeVector3d dirP(pB->x - pA->x, pB->y - pA->y, pB->z - pA->z); 

      OdGeVector3d planeNormal = polygonNormal.crossProduct(dirP);
      planeNormal.normalize();

      planes[nPoints-1].m_normal = planeNormal;
      planes[nPoints-1].m_origin = *pA;
    }
    else
    {
      pIO->putString(OD_T("Error polygon"));
      return;
    }
  }

  //5. Base checking
  if ( planes == NULL )
  {
    pIO->putString(OD_T("It is impossible to define the clipping planes"));
    return;
  }

  pIO->putString(OD_T("Processing clipping..."));

  //6. Contruct vectorization conveyor for entities clipping
  OdStaticRxObject<OdGiDrawEntityForClip> vect; // Create vectorizer
  ClippedGeometryCollector geomCollector(pDb, &vect); // Create geometry collector conveyor node

  ClippedSectionGeometryCollector sectionGemCollector(pDb, &vect); // Create section geometry collector conveyor node
  sectionGemCollector.setDrawContext(&vect);

  geomCollector.setDrawContext(&vect); // Setup our vectorizer conveyor context
  OdGiOrthoClipperExPtr pGeomClipper = OdGiOrthoClipperEx::createObject(); // Create geometry clipper
  pGeomClipper->setDrawContext(&vect); // Setup our vectorizer conveyor context
  vect.setDatabase(pDb); // Set database into vectorizer
  pGeomClipper->output().setDestGeometry(geomCollector); // Attach geometry collector conveyor node to geometry clipper output

  pGeomClipper->sectionOutput().setDestGeometry(sectionGemCollector); // Attach geometry collector conveyor node to geometry clipper output

  vect.attachOutputNode(pGeomClipper); // Attach clipper to vectorizer output

  // 7. Setup clipping contours via planes
  pGeomClipper->pushClipStage(nPlanes, planes, NULL, OdGiOrthoClipperEx::kCSEnableSections);
  delete planes; 

  //8. Draw selected objects to clip
  OdDbSelectionSetIteratorPtr pIter = pSel->newIterator();
  for (; !pIter->done(); pIter->next())
  {
    OdDbEntityPtr pEnt = OdDbEntity::cast( pIter->objectId().openObject() );
    vect.draw(pEnt);
    pGeomClipper->flushSection(true, true);
    pEnt->upgradeOpen();
    pEnt->erase();
  }

  // Remove clipping contours
  pGeomClipper->popClipStage();

  pIO->putString(OD_T("Completed."));

}
#endif

#include "DbSymUtl.h"

void _wblockTest_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbObjectId blockId = OdDbSymUtil::getBlockId(OD_T("block_30"), pDb);
  if(!blockId)
    return;
  OdDbDatabasePtr pNewDb = pDb->wblock(blockId);
  OdDbBlockTableRecordPtr pBlock1 = blockId.openObject();
  OdDbBlockTableRecordPtr pBlock2 = pNewDb->getModelSpaceId().openObject();

  OdDbObjectIteratorPtr it1 = pBlock1->newIterator();
  OdDbObjectIteratorPtr it2 = pBlock2->newIterator();
  for(; !it1->done(); it1->step(), it2->step())
  {
    ODA_ASSERT((!it2->done()) && OD_T("Incorrect number of entities in MS"));
    OdDbEntityPtr pEnt1 = it1->entity();
    OdDbEntityPtr pEnt2 = it2->entity();
    ODA_ASSERT((pEnt1->isA() == pEnt2->isA())&& OD_T("Incorrect entity type"));
  }
  ODA_ASSERT(it2->done() && OD_T("Incorrect number of entities in MS"));
}

#include "STLModule.h"
using namespace TD_STL_EXPORT;
void _STLOut_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);

  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();

  OdDbSelectionSetPtr pSSet = pIO->select( OD_T("Select entity to export") );
  
  OdDbSelectionSetIteratorPtr pIter = pSSet->newIterator();
  for ( ; !pIter->done(); pIter->next() )
  {
    OdDbEntityPtr pEntity = OdDbEntity::cast( pIter->objectId().openObject() );
    if ( pEntity.isNull() )
      continue;

    STLModulePtr module = odrxDynamicLinker()->loadModule(OdSTLExportModuleName);
    if (module.isNull())
    {
      pIO->putString( L"Cannot load module : " OdSTLExportModuleName );
      return;
    }

    OdString fileName = pIO->getFilePath(
      OD_T("Enter OUTPUT .stl file name:"), OdEd::kGfpForSave,
      OD_T("Select .stl file"), OD_T("stereolithography"),
      OdString::kEmpty,
      OD_T("Stereolithography files (*.stl)|*.stl|"));
    OdStreamBufPtr pFile = odSystemServices()->createFile(fileName, Oda::kFileWrite, Oda::kShareDenyNo, Oda::kCreateAlways);
    if (pFile.isNull())
    {
      OdString tmp = OD_T("Cannot create file : ") + fileName;
      pIO->putString(tmp);
      return;
    }

    int key = pIO->getKeyword(OD_T("Export in binary mode [Y\\N] :"), "Y N", 0);
    
    double dDeviation = 0;

    OdGeExtents3d extents;
    if (pEntity->getGeomExtents(extents) == eOk)
      dDeviation = extents.minPoint().distanceTo(extents.maxPoint()) / 100.;

    OdResult res = module->exportSTL(pEntity->database(), *pEntity, *pFile, key != 0, dDeviation);
    if (eOk == res)
    {
      pIO->putString( "Successfully exported." );
    }
    else
    {
      OdString tmp = OD_T("Error : ") + OdError(res).description();
      pIO->putString( tmp );
      return;
    }
  }
}

#ifdef STLIMPORT_ENABLED

#include "STLImport.h"

using namespace STLFileImport;

static OdString toStr(OdStlImportRes stlres)
{
  switch (stlres)
  {
  case eNoError: return "eNoError";
  case eNoFile: return "eNoFile";
  case eFileTooShort: return "eFileTooShort";
  case eEndSolidNotFound: return "eEndSolidNotFound";
  case eFailOnReadingFacet: return "eFailOnReadingFacet";
  case eNormalNotFound: return "eNormalNotFound";
  case eEndFacetNotFound: return "eEndFacetNotFound";
  case eFailOnReadingOuter: return "eFailOnReadingOuter";
  case eLoopWordNotFound: return "eLoopWordNotFound";
  case eEndLoopNotFound: return "eEndLoopNotFound";
  case eFailOnReadingVertex: return "eFailOnReadingVertex";
  case eFailOnReadingNormal: return "eFailOnReadingNormal";
  case eInvalidNumber: return "eInvalidNumber";
  case eImportCallReturnError: return "eImportCallReturnError";
  case eMultiOuterFace: return "eMultiOuterFace";
  case eOuterNotATriangle: return "eOuterNotATriangle";
  };
  return "unknown";
}

void _STLIn_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();
  
  //try to get STL importer
  OdStlImportPtr pStlImporter = createStlImporter();
  if (pStlImporter.isNull())
  {
    pIO->putString(OD_T("createStlImporter() failed."));
    return;
  }

  // ask for STL file
  OdString filePath = pDbCmdCtx->dbUserIO()->getFilePath(L"Select STL file", OdEd::kGfpForOpen, L"Select STL", L"stl", L"", "STL files (*.stl)|*.stl");

  //try to perform import
  OdStlImportRes stlres = pStlImporter->import(filePath);
  if (stlres != STLFileImport::eNoError)
  {
    pIO->putString(OD_T("pStlImporter->import(filePath) error: "));
    pIO->putString(toStr(stlres));
    return;
  }

  //try to get the data about the loaded STL file
  OdGePoint3dArray vertices;
  OdUInt32Array faces;
  OdGeVector3dArray normals;

  OdUInt8 flags = 0;
  SETBIT(flags, OdStlImport::kUnifyDuplicatedVertices, true);

  stlres = pStlImporter->getShell(vertices, faces, normals, flags);
  if (stlres != STLFileImport::eNoError)
  {
    pIO->putString(OD_T("pStlImporter->getShell error: "));
    pIO->putString(toStr(stlres));
    return;
  }

  // create SubDMesh
  OdDbSubDMeshPtr pMesh = OdDbSubDMesh::createObject();
  pMesh->setDatabaseDefaults(pDb);
  OdResult res = pMesh->setSubDMesh(vertices, (const OdInt32Array &)faces, 0);
  if (res != eOk)
  {
    pIO->putString(OD_T("pMesh->setSubDMesh failed: "));
    pIO->putString(OdError(res).description());
    return;
  }

  // ask for conversion mode
  int kwd = pDbCmdCtx->dbUserIO()->getKeyword(
    L"Convert SubDMesh to 3dSolid(S), to Surface(U), to SubDMesh(N)? (S\\U\\N)", L"S U N", 2);

  // Convert to solid, surface or don't convert
  OdDbEntityPtr pEntity(pMesh);
  bool bConvert = kwd != 2;
  if (bConvert)
  {
    bool bConvertToSolid = kwd == 0;
    if (bConvertToSolid)
    {
      OdDb3dSolidPtr pSolid;
      res = pMesh->convertToSolid(false, false, pSolid);
      pEntity = pSolid;
    }
    else
    {
      OdDbSurfacePtr pSurface;
      res = pMesh->convertToSurface(false, false, pSurface);
      pEntity = pSurface;
    }

    if (res != eOk)
    {
      pIO->putString(OD_T("pMesh->convertTo error: "));
      pIO->putString(OdError(res).description());
      return;
    }
  }
  
  // put entity to ModelSpace
  OdDbBlockTableRecordPtr ms = pDb->getModelSpaceId().safeOpenObject(OdDb::kForWrite);
  ms->appendOdDbEntity(pEntity);
  pIO->putString(OD_T("Success."));
}

#endif

#include "../Exports/DwfExport/Include/DwfExport.h"
using namespace TD_DWF_EXPORT;

#include "TDVersion.h"

//typedef void (*ExportDwfFn)(const DwExportParams& pParams);
//
//static void ExportDwfWrapper(const DwExportParams& params)
//{
//#ifdef __BORLANDC__
//#define TD_DLL_VERSION_SUFFIX_STR ""
//#endif
//
//#if defined(_TOOLKIT_IN_DLL_) && defined(WIN32)
//
//  HINSTANCE hinstLib = LoadLibrary(OdString(OdDwf7ExportModuleName) + OdString(TD_DLL_VERSION_SUFFIX_STR) + OdString(_T(".dll")));
//
//  ExportDwfFn exportFn = 0;
//  if (hinstLib != NULL) 
//  { 
//#ifdef _WIN64
//    exportFn = (ExportDwfFn)GetProcAddress(hinstLib, "?exportDwf@@YAXAEBVDwExportParams@@@Z");
//#else
//    exportFn = (ExportDwfFn)GetProcAddress(hinstLib, "?exportDwf@@YAXABVDwExportParams@@@Z");
//#endif
//  }
//  if (exportFn != NULL) 
//  { 
//    exportFn(params);
//    FreeLibrary(hinstLib);
//  }
//  else
//  {
//    throw OdError(OdString(OdDwf7ExportModuleName) + OdString(TD_DLL_VERSION_SUFFIX_STR) + OdString(L".dll"));
//  }
//
//#else // #ifdef _TOOLKIT_IN_DLL_
//
//#if defined(WIN32)
//  exportDwf(params);
//#endif
//
//#endif // #ifdef _TOOLKIT_IN_DLL_
//}

void _XrefExport_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  // insert xref block
  OdString xrefPath = pDbCmdCtx->dbUserIO()->getFilePath(L"Select xref", OdEd::kGfpForOpen, L"Select xref", L"dwg", L"", "DWG files (*.dwg)|*.dwg");
  OdDbDatabasePtr xrefDb = pDb->appServices()->readFile(xrefPath);
  OdDbObjectId  blkId = pDb->insert(L"Xref", xrefDb);
  // add reference to it
  OdDbBlockTableRecordPtr ms = pDb->getModelSpaceId().safeOpenObject(OdDb::kForWrite);
  OdDbBlockReferencePtr pdbBlkRef = OdDbBlockReference::createObject();
  pdbBlkRef->setDatabaseDefaults(pDb);
  pdbBlkRef->setBlockTableRecord(blkId);
  ms->appendOdDbEntity(pdbBlkRef);
  // get active viewport
  OdDbObjectPtr pVpObj;
  OdDbViewportTablePtr pVT = pDb->getViewportTableId().safeOpenObject();
  for (OdDbSymbolTableIteratorPtr pVpIter = pVT->newIterator(); !pVpIter->done(); pVpIter->step())
  {
    OdDbViewportTableRecordPtr pVp = pVpIter->getRecordId().openObject();
    if (!pVp.isNull() && OdDbSymUtil::isViewportActiveName(pVp->getName()))
    {
      pVpObj = pVp->objectId().openObject(OdDb::kForWrite);
    }
  }
  // zoom extents
  OdGeBoundBlock3d fullExtents;
  OdAbstractViewPEPtr pVpPE(pVpObj);
  if (pVpPE->viewExtents(pVpObj, fullExtents))
  {
    pVpPE->zoomExtents(pVpObj, &fullExtents);
    fullExtents.transformBy(pVpPE->eyeToWorld(pVpObj));
    fullExtents.setToBox(true);
    pDb->setEXTMIN(fullExtents.minPoint());
    pDb->setEXTMAX(fullExtents.maxPoint());
  }
  // export to DWF
  OdDwfExportModulePtr pModule = ::odrxDynamicLinker()->loadApp(OdDwf7ExportModuleName);
  OdDwfExportPtr exporter = pModule->create();

  DwExportParams params;

  params.setDatabase(pDb);
  params.setDwfFileName(pDbCmdCtx->dbUserIO()->getFilePath(L"DWF path", OdEd::kGfpForSave,L"DWF path", L"DWF", L"", L"DWF files (*.dwf)|*.dwf"));
  params.setBackground(0);
  //params.pPalette = theApp.curPalette();
  //params.sSourceFileName = m_strPathName;
  params.setInkedArea(false);                                                        // MKU 1/21/2004
  params.setColorMapOptimize(false);                                                 // MKU 1/21/2004
  params.setExportInvisibleText(true);

  DwfPageData pageData;
  params.setFormat(DW_UNCOMPRESSED_BINARY);
  params.setVersion(nDwf_v60);

  pageData.sLayout = pDb->findActiveLayout(true);
  params.pageData().append(pageData);
  exporter->exportDwf(params);
}

void _FlushGraphicsTest_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbObjectId id1, id2, id3;
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  {
    OdDbBlockTableRecordPtr ms = pDb->getModelSpaceId().safeOpenObject(OdDb::kForWrite);
    OdDbLinePtr l1 = OdDbLine::createObject();
    l1->setStartPoint(OdGePoint3d::kOrigin);
    l1->setEndPoint(OdGePoint3d(1,1,0));
    id1 = ms->appendOdDbEntity(l1);

    l1 = OdDbLine::createObject();
    l1->setStartPoint(OdGePoint3d::kOrigin);
    l1->setEndPoint(OdGePoint3d(2,1,0));
    id2 = ms->appendOdDbEntity(l1);
    
    l1 = OdDbLine::createObject();
    l1->setStartPoint(OdGePoint3d::kOrigin);
    l1->setEndPoint(OdGePoint3d(1,2,0));
    id3 = ms->appendOdDbEntity(l1);
  }
  pDb->flushGraphics();
  pDbCmdCtx->userIO()->getString(L"pause");
  pDb->startTransaction();
  {
    OdDbLinePtr l1 = id1.safeOpenObject(OdDb::kForWrite);
    l1->setStartPoint(OdGePoint3d(3,3, 0));
  }
  {
    OdDbLinePtr l1 = id2.safeOpenObject(OdDb::kForWrite);
    l1->setStartPoint(OdGePoint3d(3,3, 0));
  }
  pDb->flushGraphics();
  {
    OdDbLinePtr l1 = id3.safeOpenObject(OdDb::kForWrite);
    l1->setStartPoint(OdGePoint3d(3,3, 0));
  }
  pDbCmdCtx->userIO()->getString(L"pause");
  pDb->endTransaction();
}

static OdDbSortentsTablePtr getSortentsTable(OdDbDatabase* db, bool createIfNotFound)
{
  OdDbSortentsTablePtr pTbl;
  OdDbObjectId blockId = db->getActiveLayoutBTRId();
  OdDbBlockTableRecordPtr pBlock = blockId.openObject(OdDb::kForWrite);
  if(pBlock.get())
  {
    pTbl = pBlock->getSortentsTable(createIfNotFound);
  }
  return pTbl;
}

static OdDbObjectId createLine(OdDbDatabase* db, const OdGePoint3d& start, const OdGePoint3d& end)
{
  OdDbBlockTableRecordPtr pOwner = db->getActiveLayoutBTRId().safeOpenObject(OdDb::kForWrite);
  OdDbLinePtr pLine = OdDbLine::createObject();
  pLine->setDatabaseDefaults(db);
  pLine->setStartPoint(start);
  pLine->setEndPoint(end);
  return pOwner->appendOdDbEntity(pLine);
}

static bool setFrontBack(OdDbDatabase* db, OdDbObjectIdArray& aIds, bool bFront)
{
  OdDbSortentsTablePtr pTbl = getSortentsTable(db, true);
  if(!pTbl.get())
    return false;
  if(bFront)
    pTbl->moveToTop(aIds);
  else
    pTbl->moveToBottom(aIds);
  return true;
}

static bool setAboveUnder(OdDbDatabase* db, OdDbObjectIdArray& aIds,
                          const OdDbObjectId& refId, bool bAbove)
{
  OdDbSortentsTablePtr pTbl = getSortentsTable(db, true);
  if(!pTbl.get())
    return false;
  if(bAbove)
    pTbl->moveAbove(aIds, refId);
  else
    pTbl->moveBelow(aIds, refId);
  return true;
}

static void createEnts(OdDbDatabase* db, OdDbObjectIdArray& aIds, int nEnts)
{
  OdGePoint3d start(0,0,0), end(1,1,0);
  for(int i = 0; i < nEnts; ++i)
    aIds.push_back(createLine(db, start, end));
}

static bool checkSortentsTable(OdDbDatabase* db, const HandlePairsArray& pattern)
{
  OdDbSortentsTablePtr pTbl = getSortentsTable(db, true);
  HandlePairsArray order;
  pTbl->getAbsoluteDrawOrder(order);
  if(order.size() != pattern.size())
  {
    ODA_ASSERT("Incorrect sortents table size" && 0);
    return false;
  }
  for(unsigned i = 0, n = order.size(); i < n; ++i)
  {
    if(pattern[i].first != order[i].first || pattern[i].second != order[i].second)
    {
      ODA_ASSERT("Incorrect sortents table data" && 0);
      return false;
    }
  }
  return true;
}

void _SORTENTS_TEST_Front_func(OdEdCommandContext* pCmdCtx)
{
  OdDbDatabase* db = ((OdDbCommandContext*)pCmdCtx)->database();
  OdDbObjectIdArray ids; //to keep created ents, nothing else
  createEnts(db, ids, 3);

  OdDbObjectIdArray aArg;//operation arguments
  HandlePairsArray pattern; //correct draworder

  //1. Move 0, 1 ents to front
  aArg.push_back(ids[0]);
  aArg.push_back(ids[1]);
  setFrontBack(db, aArg, true);
  aArg.clear();
  // Checking...Form correct pattern, must be (2, 0, 1)
  pattern.push_back(HandlePair(ids[0].getHandle(), ids[2]));
  pattern.push_back(HandlePair(ids[1].getHandle(), ids[0]));
  pattern.push_back(HandlePair(ids[2].getHandle(), ids[1]));
  checkSortentsTable(db, pattern);
  pattern.clear();


  //2. Move 2 to front
  aArg.push_back(ids[2]);
  setFrontBack(db, aArg, true);
  aArg.clear();
  // Checking...Form correct pattern, must be (0, 1, 2)
  pattern.push_back(HandlePair(ids[0].getHandle(), ids[0]));
  pattern.push_back(HandlePair(ids[1].getHandle(), ids[1]));
  pattern.push_back(HandlePair(ids[2].getHandle(), ids[2]));
  checkSortentsTable(db, pattern);
  pattern.clear();

  //Create extra entity
  createEnts(db, ids, 1);

  //3. Move 0,3 to front
  aArg.push_back(ids[0]);
  aArg.push_back(ids[3]);
  setFrontBack(db, aArg, true);
  aArg.clear();
  // Checking...Form correct pattern, must be (1, 2, 0, 3)
  pattern.push_back(HandlePair(ids[0].getHandle(), ids[1]));
  pattern.push_back(HandlePair(ids[1].getHandle(), ids[2]));
  pattern.push_back(HandlePair(ids[2].getHandle(), ids[0]));
  pattern.push_back(HandlePair(ids[3].getHandle(), ids[3]));
  checkSortentsTable(db, pattern);  //DD bug, it implements (1, 2, 3, 0)
  pattern.clear();
}

void _SORTENTS_TEST_Back_func(OdEdCommandContext* pCmdCtx)
{
  OdDbDatabase* db = ((OdDbCommandContext*)pCmdCtx)->database();
  OdDbObjectIdArray ids; //to keep created ents, nothing else
  createEnts(db, ids, 3);

  OdDbObjectIdArray aArg;//operation arguments
  HandlePairsArray pattern; //correct draworder

  //1. Move 0, 1 ents to back
  aArg.push_back(ids[0]);
  aArg.push_back(ids[1]);
  setFrontBack(db, aArg, false);
  aArg.clear();
  // Checking...Form correct pattern, must be (0, 1, 2)
  pattern.push_back(HandlePair(ids[0].getHandle(), ids[0]));
  pattern.push_back(HandlePair(ids[1].getHandle(), ids[1]));
  pattern.push_back(HandlePair(ids[2].getHandle(), ids[2]));
  checkSortentsTable(db, pattern); //DD bug, it implements (1, 0, 2)
  pattern.clear();
}

void _SORTENTS_TEST_Above_func(OdEdCommandContext* pCmdCtx)
{
  OdDbDatabase* db = ((OdDbCommandContext*)pCmdCtx)->database();
  OdDbObjectIdArray ids; //to keep created ents, nothing else
  createEnts(db, ids, 4);

  OdDbObjectIdArray aArg;//operation arguments
  HandlePairsArray pattern; //correct draworder

  //1. Move 0, 1 above 2
  aArg.push_back(ids[0]);
  aArg.push_back(ids[1]);
  setAboveUnder(db, aArg, ids[2], true);
  aArg.clear();
  // Checking...Form correct pattern, must be (2, 0, 1, 3)
  pattern.push_back(HandlePair(ids[0].getHandle(), ids[2]));
  pattern.push_back(HandlePair(ids[1].getHandle(), ids[0]));
  pattern.push_back(HandlePair(ids[2].getHandle(), ids[1]));
  pattern.push_back(HandlePair(ids[3].getHandle(), ids[3]));
  checkSortentsTable(db, pattern);
  pattern.clear();


  //2. Move 2 above 1
  aArg.push_back(ids[2]);
  setAboveUnder(db, aArg, ids[1], true);
  aArg.clear();
  // Checking...Form correct pattern, must be (0, 1, 2, 3)
  pattern.push_back(HandlePair(ids[0].getHandle(), ids[0]));
  pattern.push_back(HandlePair(ids[1].getHandle(), ids[1]));
  pattern.push_back(HandlePair(ids[2].getHandle(), ids[2]));
  pattern.push_back(HandlePair(ids[3].getHandle(), ids[3]));
  checkSortentsTable(db, pattern);
  pattern.clear();

  //create one more entity
  createEnts(db, ids, 1);

  //3. Move 1, 4 above 2
  aArg.push_back(ids[1]);
  aArg.push_back(ids[4]);
  setAboveUnder(db, aArg, ids[2], true);
  aArg.clear();
  // Checking...Form correct pattern, must be (0, 2, 1, 4, 3)
  pattern.push_back(HandlePair(ids[0].getHandle(), ids[0]));
  pattern.push_back(HandlePair(ids[1].getHandle(), ids[2]));
  pattern.push_back(HandlePair(ids[2].getHandle(), ids[1]));
  pattern.push_back(HandlePair(ids[3].getHandle(), ids[4]));
  pattern.push_back(HandlePair(ids[4].getHandle(), ids[3]));
  checkSortentsTable(db, pattern); //DD bug
  pattern.clear();
}

void _SORTENTS_TEST_Under_func(OdEdCommandContext* pCmdCtx)
{
  OdDbDatabase* db = ((OdDbCommandContext*)pCmdCtx)->database();
  OdDbObjectIdArray ids; //to keep created ents, nothing else
  createEnts(db, ids, 4);

  OdDbObjectIdArray aArg;//operation arguments
  HandlePairsArray pattern; //correct draworder

  //1. Move 0, 1 under 2
  aArg.push_back(ids[0]);
  aArg.push_back(ids[1]);
  setAboveUnder(db, aArg, ids[2], false);
  aArg.clear();
  // Checking...Form correct pattern, must be (0, 1, 2, 3)
  pattern.push_back(HandlePair(ids[0].getHandle(), ids[0]));
  pattern.push_back(HandlePair(ids[1].getHandle(), ids[1]));
  pattern.push_back(HandlePair(ids[2].getHandle(), ids[2]));
  pattern.push_back(HandlePair(ids[3].getHandle(), ids[3]));
  checkSortentsTable(db, pattern);
  pattern.clear();


  //2. Move 2 under 1
  aArg.push_back(ids[2]);
  setAboveUnder(db, aArg, ids[1], false);
  aArg.clear();
  // Checking...Form correct pattern, must be (0, 2, 1, 3)
  pattern.push_back(HandlePair(ids[0].getHandle(), ids[0]));
  pattern.push_back(HandlePair(ids[1].getHandle(), ids[2]));
  pattern.push_back(HandlePair(ids[2].getHandle(), ids[1]));
  pattern.push_back(HandlePair(ids[3].getHandle(), ids[3]));
  checkSortentsTable(db, pattern);
  pattern.clear();

  //create one more entity
  createEnts(db, ids, 1);

  //3. Move 1, 4 under 2
  aArg.push_back(ids[1]);
  aArg.push_back(ids[4]);
  setAboveUnder(db, aArg, ids[2], false);
  aArg.clear();
  // Checking...Form correct pattern, must be (0, 1, 4, 2, 3)
  pattern.push_back(HandlePair(ids[0].getHandle(), ids[0]));
  pattern.push_back(HandlePair(ids[1].getHandle(), ids[1]));
  pattern.push_back(HandlePair(ids[2].getHandle(), ids[4]));
  pattern.push_back(HandlePair(ids[3].getHandle(), ids[2]));
  pattern.push_back(HandlePair(ids[4].getHandle(), ids[3]));
  checkSortentsTable(db, pattern);
  pattern.clear();

  //create one more entity
  createEnts(db, ids, 1);

  //4. Move 2, 4 under 5
  aArg.push_back(ids[2]);
  aArg.push_back(ids[4]);
  setAboveUnder(db, aArg, ids[5], false);
  aArg.clear();
  // Checking...Form correct pattern, must be (0, 1, 3, 4, 2, 5)
  pattern.push_back(HandlePair(ids[0].getHandle(), ids[0]));
  pattern.push_back(HandlePair(ids[1].getHandle(), ids[1]));
  pattern.push_back(HandlePair(ids[2].getHandle(), ids[3]));
  pattern.push_back(HandlePair(ids[3].getHandle(), ids[4]));
  pattern.push_back(HandlePair(ids[4].getHandle(), ids[2]));
  pattern.push_back(HandlePair(ids[5].getHandle(), ids[5]));
  checkSortentsTable(db, pattern); //DD bug
  pattern.clear();
}

void _SORTENTS_TEST_MoveToBottom_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbSelectionSetPtr pSSet = pDbCmdCtx->dbUserIO()->select();
  OdDbObjectIdArray ids;
  for (OdDbSelectionSetIteratorPtr pIter = pSSet->newIterator();!pIter->done();pIter->next())
    ids.append(pIter->objectId());
  OdDbSortentsTablePtr pTbl = getSortentsTable(pDbCmdCtx->database(), true);
  pTbl->moveToBottom(ids);
}

void _SORTENTS_TEST_MoveToTop_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbSelectionSetPtr pSSet = pDbCmdCtx->dbUserIO()->select();
  OdDbObjectIdArray ids;
  for (OdDbSelectionSetIteratorPtr pIter = pSSet->newIterator();!pIter->done();pIter->next())
    ids.append(pIter->objectId());
  OdDbSortentsTablePtr pTbl = getSortentsTable(pDbCmdCtx->database(), true);
  pTbl->moveToTop(ids);
}

void _EvaluateNetwork_func(OdEdCommandContext* pCmdCtx)
{
}

static bool s_isBeditActive = false;
static OdDbObjectId s_blockId;
static OdDbObjectId s_tmpMsId;

static void collectBlockContents(OdDbObjectId blockId, OdDbObjectIdArray& ids)
{
  OdDbBlockTableRecordPtr pBlock = blockId.safeOpenObject();
  for (OdDbObjectIteratorPtr i = pBlock->newIterator(); !i->done(); i->step(true, false))
    ids.append(i->objectId());
}

void _BEDIT_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();

  if (s_isBeditActive)
  {
    pIO->putString(L"BEDIT session is active, use BCLOSE");
    return;
  }
  s_isBeditActive = true;
  s_blockId = OdDbObjectId::kNull;
  s_tmpMsId = OdDbObjectId::kNull;

  // Select a block to edit
  {
    OdDbSelectionSetPtr pSSet = pIO->select(
      OD_T("Select object <done>:"),
      OdEd::kSelAllowInactSpaces|OdEd::kSelAllowObjects);
    OdDbSelectionSetIteratorPtr pIter = pSSet->newIterator();
    if (pIter->done())
    {
      throw OdError(eInvalidInput);
    }
    s_blockId = pIter->objectId();
  }
  {
    OdDbObjectPtr pObj = s_blockId.safeOpenObject();
    if (pObj->isKindOf(OdDbBlockReference::desc()))
      s_blockId = OdDbBlockReferencePtr(pObj)->blockTableRecord();
  }
  
  // Save model space contents to the temp block
  {
    OdDbBlockTableRecordPtr pBlock = OdDbBlockTableRecord::createObject();
    pBlock->setName(L"*U");
    OdDbBlockTablePtr bt = pDb->getBlockTableId().safeOpenObject(OdDb::kForWrite);
    s_tmpMsId = bt->add(pBlock);
    OdDbObjectIdArray ids;
    collectBlockContents(pDb->getModelSpaceId(), ids);
    pBlock->assumeOwnershipOf(ids);
  }

  // Clone block contents to model space
  {
    OdDbObjectIdArray ids;
    collectBlockContents(s_blockId, ids);
    OdDbIdMappingPtr m = OdDbIdMapping::createObject();
    pDb->deepCloneObjects(ids, pDb->getModelSpaceId(), *m);
  }

  // Here one should also save current view parameters, and zoom to extents

  ::odedRegCmds()->executeCommand(L"REGEN", pCmdCtx);
}

void _BCLOSE_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();

  if (!s_isBeditActive)
  {
    pIO->putString(L"BEDIT session is not active, use BEDIT");
    return;
  }
  s_isBeditActive = false;

  OdDbObjectIdArray msIds, oldBlockIds, newBlockIds;
  collectBlockContents(pDb->getModelSpaceId(), newBlockIds);
  collectBlockContents(s_tmpMsId, msIds);
  OdDbBlockTableRecordPtr pTmpMs = s_tmpMsId.safeOpenObject(OdDb::kForWrite);
  OdDbBlockTableRecordPtr pMs = pDb->getModelSpaceId().safeOpenObject(OdDb::kForWrite);
  // Save block changes
  if (0 == pIO->getKeyword(OD_T("Save changes to block?"), OD_T("Yes No")))
  {
    collectBlockContents(s_blockId, oldBlockIds);
    OdDbBlockTableRecordPtr pOldBlock = s_blockId.safeOpenObject(OdDb::kForWrite);
    pMs->assumeOwnershipOf(msIds);
    pOldBlock->assumeOwnershipOf(newBlockIds);
    pTmpMs->assumeOwnershipOf(oldBlockIds);
  }
  else
  {
    pMs->assumeOwnershipOf(msIds);
    pTmpMs->assumeOwnershipOf(newBlockIds);
  }
  pTmpMs->erase();
  pTmpMs = 0;
  pMs = 0;

  // This is a good place to restore saved view

  ::odedRegCmds()->executeCommand(L"REGEN", pCmdCtx);
}

void _CompoundIdTest_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbBlockTablePtr bt = pDb->getBlockTableId().safeOpenObject(OdDb::kForWrite);
  // LINE
  OdDbLinePtr line = OdDbLine::createObject();
  line->setStartPoint(OdGePoint3d::kOrigin);
  line->setEndPoint(OdGePoint3d(1,1,0));
  // block that contains the line
  OdDbBlockTableRecordPtr btr1 = OdDbBlockTableRecord::createObject();
  btr1->setName(L"BLOCK1");
  OdDbObjectId btr1Id = bt->add(btr1);
  OdDbObjectId lineid = btr1->appendOdDbEntity(line);
  // block that contains the first insert
  OdDbBlockTableRecordPtr btr2 = OdDbBlockTableRecord::createObject();
  btr2->setName(L"BLOCK2");
  OdDbObjectId btr2Id = bt->add(btr2);
  // FIRST insert
  OdDbBlockReferencePtr brf1 = OdDbBlockReference::createObject();
  brf1->setBlockTableRecord(btr1Id);
  OdGeMatrix3d m1 = OdGeMatrix3d::rotation(1.0, OdGeVector3d::kZAxis);
  m1.preMultBy(OdGeMatrix3d::scaling(2));
  m1.preMultBy(OdGeMatrix3d::translation(OdGeVector3d(10,0,0)));
  brf1->setBlockTransform(m1);
  OdDbObjectId brf1id = btr2->appendOdDbEntity(brf1);
  
  OdDbBlockReferencePtr brf2 = OdDbBlockReference::createObject();
  brf2->setBlockTableRecord(btr2Id);
  OdGeMatrix3d m2 = OdGeMatrix3d::rotation(-0.7, OdGeVector3d::kZAxis);
  m2.preMultBy(OdGeMatrix3d::scaling(3));
  m2.preMultBy(OdGeMatrix3d::translation(OdGeVector3d(0,5,0)));
  brf2->setBlockTransform(m2);
  OdDbBlockTableRecordPtr ms = pDb->getModelSpaceId().safeOpenObject(OdDb::kForWrite);
  OdDbObjectId brf2id = ms->appendOdDbEntity(brf2);

  OdDbObjectIdArray ia;
  ia.append(brf2id);
  ia.append(brf1id);
  OdDbCompoundObjectId id1;
  id1.set(lineid, ia, pDb);
  OdDbCompoundObjectId id2;
  ia.append(lineid);
  id2.setFullPath(ia);
  OdDbCompoundObjectId id3;
  id3.set(id1);
  ODA_ASSERT(id1 == id2);
  ODA_ASSERT(id2 == id3);
  ODA_ASSERT(id1 == id3);
  OdGeMatrix3d m;
  id3.getTransform(m);
  ODA_ASSERT(m == (m1 * m2));
  ODA_ASSERT( OdDbCompoundObjectId().isSimpleObjectId());
  ODA_ASSERT( !id3.isSimpleObjectId());
  id3.set(lineid);
  ODA_ASSERT( id3.isSimpleObjectId());
}

void _CONVERTTOMTEXTATTRIBUTE_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbSelectionSetPtr ss = pDbCmdCtx->dbUserIO()->select(L"select block reference", OdEd::kSelAllowObjects);
  for (OdDbSelectionSetIteratorPtr i = ss->newIterator(); !i->done();i->next())
  {
    OdDbObjectPtr o = i->objectId().openObject();
    if (!o.isNull() && o->isKindOf(OdDbBlockReference::desc()))
    {
      int kwd = pDbCmdCtx->dbUserIO()->getKeyword(
        L"Convert to multiline attribute (M)/Convert to single line attribute (S)", L"M S", 0);
      OdDbBlockReferencePtr br(o);
      for (OdDbObjectIteratorPtr ai = br->attributeIterator(); !ai->done(); ai->step())
      {
        OdDbAttributePtr a(ai->entity(OdDb::kForWrite));
        a->convertIntoMTextAttribute(kwd == 0);
      }
    }
  }
}

void _TransactionPerformanceTest_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pOdDb = pDbCmdCtx->database();

  OdDbObjectId idLayout = OdDbBlockTableRecordPtr(pOdDb->getActiveLayoutBTRId().safeOpenObject())->getLayoutId();
  OdDbLayoutPtr pActiveLayout = idLayout.safeOpenObject(OdDb::kForWrite);
  OdDbObjectId idOwner = pActiveLayout->getBlockTableRecordId();

  OdDbLinePtr pNewLine;
  pNewLine = OdDbLine::createObject();
  pNewLine->setDatabaseDefaults(pOdDb.get());

  pNewLine->setStartPoint(OdGePoint3d(0.,0.,0.));
  pNewLine->setEndPoint(OdGePoint3d(1.,1.,0.));

  OdDbObjectId newEntId = pOdDb->addOdDbObject(pNewLine);

  OdDbObjectIdArray ids;
  ids.append(newEntId);

  pOdDb->startTransaction();

  for (int i=0; i<50000; i++)
  {
    OdDbIdMappingPtr entityToEntityMap = OdDbIdMapping::createObject();
    entityToEntityMap->setDestDb(pOdDb);

    pOdDb->deepCloneObjects(ids, idOwner, *entityToEntityMap.get());
  }

  pOdDb->endTransaction();
}

#include "summinfo.h"
void _AddCustomProperty_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();
  OdDbDatabasePtr pDb = pDbCmdCtx->database();

  OdString newSIKey   = pIO->getString("Specify new CustomProperty key:");
  OdString newSIValue = pIO->getString("Specify new CustomProperty value:");
  OdDbDatabaseSummaryInfoPtr pSI = oddbGetSummaryInfo(pDb);
  pSI->addCustomSummaryInfo(newSIKey, newSIValue);
  oddbPutSummaryInfo(pSI);
}

#include "DbUnderlayReference.h"
#include "RxObjectImpl.h"
#include "Gi/GiViewport.h"
#include "Gi/GiWorldDraw.h"
#include "Gi/GiDummyGeometry.h"
#include "DbGrip.h"
#include "TDVersion.h"
#include "StaticRxObject.h"
#include "DbPoint.h"
#include "Db3dPolyline.h"
#include "Db3dPolylineVertex.h"
#include "DbSpline.h"
#include "../Extensions/PdfUnderlayCommon/PdfUnderlay.h"

void _showPdfSnaps_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();

  // get Modelspace block table record
  OdDbBlockTableRecordPtr pMS = pDb->getModelSpaceId().openObject(OdDb::kForWrite);

  // Step ower only model space block entities.
  OdDbObjectIteratorPtr pEntIter = pMS->newIterator();
  while(!pEntIter->done())
  {
    // next entitys
    OdDbEntityPtr pEnt = pEntIter->entity(OdDb::kForWrite);

    // step should be here. Operations like handOverTo\erase is unsafe for iteration entity sequence.
    pEntIter->step();

    // is OdDbPdfReference
    if (!pEnt.isNull() 
      && (pEnt->isKindOf(OdDbPdfReference::desc())))
    {
      OdDbPdfReferencePtr pPdfRef = pEnt;

      // There are two implementation of PdfUnderlay available.
      // Check which one is used.

      OdString pdfModuleName;

      // FLYSDK version
      //PdfUnderlayModulePtr pModule = odrxDynamicLinker()->loadModule(OdPdfModuleVIModuleName);
      //
      PdfUnderlayModulePtr pModule = OdDbPdfDefinition::loadPdfUnderlayModule();
      if (pModule.isNull())
      {
        pIO->putString(OD_T("PdfUnderlay support module missed."));
        return;
      }

      {
        {
          class PdfSnapGrabberImpl : public OdStaticRxObject<OdGiDummyGeometry<OdGiGeometry> >
          {
          public:
            OdGeMatrix3d mx;
            OdDbBlockTableRecordPtr pMS;

            void polyline(OdInt32 numVertices,
              const OdGePoint3d* vertexList,
              const OdGeVector3d* /*pNormal*/,
              OdGsMarker /*baseSubEntMarker*/) 
            {
              OdDb3dPolylinePtr pP = OdDb3dPolyline::createObject();
              pP->setColorIndex(1);
              pP->setLineWeight(OdDb::kLnWt020);
              for(int f=0; f<numVertices; ++f)
              {

                OdDb3dPolylineVertexPtr pV = OdDb3dPolylineVertex::createObject();
                pV->setPosition(OdGePoint3d(vertexList[f]).transformBy(mx));
                pP->appendVertex(pV );
              }
              pMS->appendOdDbEntity(pP);
            }

            void nurbs(const OdGeNurbCurve3d& nurbsCurve)
            {
              OdDbSplinePtr pS = OdDbSpline::createObject();
              pS->setColorIndex(1);
              pS->setLineWeight(OdDb::kLnWt020);
              int degree;
              bool rational, periodic;
              OdGeKnotVector aKt;
              OdGePoint3dArray aCP;
              OdGeDoubleArray aWt;
              nurbsCurve.getDefinitionData(degree, rational, periodic, aKt, aCP, aWt);
              pS->setNurbsData(degree, rational, false, periodic, aCP, aKt, aWt, 0.001);
              pMS->appendOdDbEntity(pS);
              pS->transformBy(mx);
            }

            void circularArc(const OdGePoint3d& start, const OdGePoint3d& point, const OdGePoint3d& end, OdGiArcType arcType = kOdGiArcSimple)
            {
              OdDbArcPtr pArc = OdDbArc::createObject();
              pArc->setColorIndex(3);
              pArc->setLineWeight(OdDb::kLnWt020);
              OdGeCircArc3d arc(start, point, end);
              pArc->setCenter(arc.center());
              pArc->setRadius(arc.radius());
              OdGeVector3d ref = start - arc.center();
              bool bSide = OdGeVector3d(1., 0., 0.).crossProduct(ref).z > 0;
              double angBeg = OdGeVector3d(1., 0., 0.).angleTo(ref);
              if(!bSide)
                angBeg = Oda2PI - angBeg;
              ref = end - arc.center();
              bSide = OdGeVector3d(1., 0., 0.).crossProduct(ref).z > 0;
              double angEnd = OdGeVector3d(1., 0., 0.).angleTo(ref);
              if(!bSide)
                angEnd = Oda2PI - angEnd;
              ref = point - arc.center();
              bSide = OdGeVector3d(1., 0., 0.).crossProduct(ref).z > 0;
              double angMid = OdGeVector3d(1., 0., 0.).angleTo(ref);
              if(!bSide)
                angMid = Oda2PI - angMid;
              if(angBeg > angEnd)
              {
                double tmp = angBeg;
                angBeg = angEnd;
                angEnd = tmp;
              }
              if(angMid < angBeg || angMid > angEnd)
              {
                double tmp = angBeg;
                angBeg = angEnd;
                angEnd = tmp;
              }
              pArc->setStartAngle(angBeg);
              pArc->setEndAngle(angEnd);
              pMS->appendOdDbEntity(pArc);
              pArc->transformBy(mx);
            }

            void circle(const OdGePoint3d& center, double radius, const OdGeVector3d& normal)
            {
              OdDbCirclePtr pCirc = OdDbCircle::createObject();
              pCirc->setColorIndex(2);
              pCirc->setLineWeight(OdDb::kLnWt020);
              pCirc->setCenter(center);
              pCirc->setRadius(radius);
              pMS->appendOdDbEntity(pCirc);
              pCirc->transformBy(mx);
            }
          };

          PdfSnapGrabberImpl PdfSnapGrabber;
          PdfSnapGrabber.pMS = pMS;
          //////////////////////////////////////////////////////////////////////////
          OdGePoint2dArray pnts2d;
          if (pPdfRef->isClipped() && pPdfRef->clipBoundary().size())
          { // fill from clip boundary
            pnts2d.insert(pnts2d.begin(), pPdfRef->clipBoundary().asArrayPtr(),
              pPdfRef->clipBoundary().asArrayPtr() + pPdfRef->clipBoundary().size());
          }
          else
          { // fill from extents
            OdDbUnderlayDefinitionPtr pDef = pPdfRef->definitionId().openObject();
            if (!pDef.isNull())
            {
              OdDbUnderlayItemPtr pItem = pDef->getUnderlayItem();
              if (!pItem.isNull())
              {
                pnts2d.resize(2);
                pItem->getExtents(pnts2d[0], pnts2d[1]);
              }
            }
          }

          if (pnts2d.size() == 2)
          {
            if (pnts2d[0].x > pnts2d[1].x)
              std::swap(pnts2d[0].x, pnts2d[1].x);
            if (pnts2d[0].y > pnts2d[1].y)
              std::swap(pnts2d[0].y, pnts2d[1].y);
          
            OdGePoint2d pt0 = pnts2d[0], pt1 = pnts2d[1];
            pnts2d.resize(5);
            pnts2d[0].set(pt0.x, pt0.y);
            pnts2d[1].set(pt0.x, pt1.y);
            pnts2d[2].set(pt1.x, pt1.y);
            pnts2d[3].set(pt1.x, pt0.y);
            pnts2d[4] = pt0;
          }

          OdGePoint3dArray pnts;
          pnts.resize(pnts2d.size());
          for (OdUInt32 f = 0; f < pnts2d.size(); ++f)
          {
            pnts[f].set(pnts2d[f].x, pnts2d[f].y, 0);
            pnts[f].transformBy(pPdfRef->transform());
          }
          if (!pnts.isEmpty() && !pnts.first().isEqualTo(pnts.last()))
            pnts.push_back(pnts.first());
          if (!pnts.isEmpty())
            PdfSnapGrabber.polyline(pnts.size(), pnts.asArrayPtr(), 0, -1);
          //////////////////////////////////////////////////////////////////////////
          PdfSnapGrabber.mx = pPdfRef->transform();

          pModule->getSnapGeometry(pPdfRef, &PdfSnapGrabber, 0);
        }
      }
    }
  }
}

void _showPdfGrips_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();

  // There are two implementation of PdfUnderlay available.
  // Check which one is used.

  OdString pdfModuleName;

  // FLYSDK version
  //PdfUnderlayModulePtr pModule = odrxDynamicLinker()->loadModule(OdPdfModuleVIModuleName);
  PdfUnderlayModulePtr pModule = OdDbPdfDefinition::loadPdfUnderlayModule();
  if (pModule.isNull())
  {
    pIO->putString(OD_T("PdfUnderlay support module missed."));
    return;
  }  
  
  int kwd = pDbCmdCtx->dbUserIO()->getKeyword(
    L"Make all underlays monochrome? (Y\\N)", L"Y N", 0);

  bool bMakeMono = kwd == 0;

  // get Modelspace block table record
  OdDbBlockTableRecordPtr pMS = pDb->getActiveLayoutBTRId().openObject(OdDb::kForWrite);

  // Step ower only model space block entities.
  OdDbObjectIteratorPtr pEntIter = pMS->newIterator();
  while(!pEntIter->done())
  {
    // next entity
    OdDbEntityPtr pEnt = pEntIter->entity(OdDb::kForWrite);

    // step should be here. Operations like handOverTo\erase is unsafe for iteration entity sequence.
    pEntIter->step();

    // is OdDbPdfReference
    if (!pEnt.isNull() 
      && (pEnt->isKindOf(OdDbPdfReference::desc())))
    {
      OdDbPdfReferencePtr pPdfRef = pEnt;

      if (bMakeMono)
      {
        pPdfRef->setContrast(0);
        pPdfRef->setIsMonochrome(true);
      }

      OdGePoint3dArray gripPoints;
      if (pPdfRef->getGripPoints(gripPoints) != eOk)
      {
        pIO->putString(OD_T("pPdfRef->getGripPoints return Error."));
      }
      else
      {
        OdGeExtents3d extents;
        if (pPdfRef->getGeomExtents(extents) == eOk)
        {
          if (extents.isValidExtents())
          {
            for(unsigned int i = 0; i<gripPoints.length(); ++i)
            {
              double radius = extents.maxPoint().distanceTo(extents.minPoint()) / 100.;
              OdGePoint3d center = gripPoints[i];

              OdDbTracePtr pTrace = OdDbTrace::createObject();
              pTrace->setPointAt(0, OdGePoint3d(center.x-radius, center.y+radius, center.z));
              pTrace->setPointAt(1, OdGePoint3d(center.x+radius, center.y+radius, center.z));
              pTrace->setPointAt(2, OdGePoint3d(center.x-radius, center.y-radius, center.z));
              pTrace->setPointAt(3, OdGePoint3d(center.x+radius, center.y-radius, center.z));
              pTrace->setColorIndex(1);
              pMS->appendOdDbEntity(pTrace);
            }
          }
          else
          {
            pIO->putString(OD_T("extents.isValidExtents return false."));
          }
        }
        else
        {
          pIO->putString(OD_T("pPdfRef->getGeomExtents return Error."));
        }
      }
    }
  }
}

void _InvertImageClipBoundary_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();

  ClassFilter fltr(OdDbRasterImage::desc());
  OdDbSelectionSetPtr pSSet = pIO->select(OD_T("Select raster images:"), 0, 0, OdString::kEmpty, &fltr);
  OdDbSelectionSetIteratorPtr pIter = pSSet->newIterator();
  while(!pIter->done())
  {
    OdDbRasterImagePtr pRI = pIter->objectId().safeOpenObject(OdDb::kForWrite);
    pRI->setClipInverted(!pRI->isClipInverted());
    pIter->next();
  }
}

void _UnderlayClip_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();

  ClassFilter fltr(OdDbUnderlayReference::desc());
  OdDbSelectionSetPtr pSSet = pIO->select(OD_T("Select underlay:"), OdEd::kSelSingleEntity, 0, OdString::kEmpty, &fltr);
  OdDbSelectionSetIteratorPtr pIter = pSSet->newIterator();
  while(!pIter->done())
  {
    OdDbUnderlayReferencePtr pUR = pIter->objectId().safeOpenObject(OdDb::kForWrite);
    if (pIO->getKeyword(OD_T("What to test [Invert/gen from Pline]:"), OD_T("Invert Pline")) == 1)
    {
      OdDbSelectionSetPtr pSSet = pIO->select(OD_T("Select polyline:"), OdEd::kSelSingleEntity, 0, OdString::kEmpty);
      OdDbSelectionSetIteratorPtr pIter2 = pSSet->newIterator();
      while(!pIter2->done())
      {
        if (pUR->generateClipBoundaryFromPline(pIter2->objectId()) == eOk)
        {
          pUR->setIsClipped(true);
          pIO->putString(OD_T("ok"));
        }
        else
        {
          pUR->setIsClipped(false);
          pIO->putString(OD_T("error"));
        }
        pIter2->next();
      }
    }
    else
    {
      pUR->setClipInverted(!pUR->isClipInverted());
    }
    pIter->next();
  }
}

void _BlockRefClip_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();

  ClassFilter fltr(OdDbBlockReference::desc());
  OdDbSelectionSetPtr pSSet = pIO->select(OD_T("Select block reference:"), OdEd::kSelSingleEntity, 0, OdString::kEmpty, &fltr);
  OdDbSelectionSetIteratorPtr pIter = pSSet->newIterator();
  while(!pIter->done())
  {
    OdDbBlockReferencePtr pBR = pIter->objectId().safeOpenObject(OdDb::kForWrite);
    OdDbDictionaryPtr pDict = OdDbDictionary::cast(pBR->extensionDictionary().openObject());
    if (pDict.isNull())
      return;
    OdDbDictionaryPtr pFDict = OdDbDictionary::cast(pDict->getAt(OD_T("ACAD_FILTER"), OdDb::kForRead));
    if (pFDict.isNull())
      return;
    OdDbSpatialFilterPtr pSp = OdDbSpatialFilter::cast(pFDict->getAt(OD_T("SPATIAL"), OdDb::kForWrite));
    if (pSp.isNull())
      return;
    if (pIO->getKeyword(OD_T("What to test [Invert/gen from Pline]:"), OD_T("Invert Pline")) == 1)
    {
      OdDbSelectionSetPtr pSSet = pIO->select(OD_T("Select polyline:"), OdEd::kSelSingleEntity, 0, OdString::kEmpty);
      OdDbSelectionSetIteratorPtr pIter2 = pSSet->newIterator();
      while(!pIter2->done())
      {
        if (pSp->generateClipBoundaryFromPline(pIter2->objectId()) == eOk)
        {
          pIO->putString(OD_T("ok"));
        }
        else
        {
          pIO->putString(OD_T("error"));
        }
        pIter2->next();
      }
    }
    else
    {
      pSp->setFilterInverted(!pSp->isFilterInverted());
    }
    pIter->next();
  }
}

void _BlockRefHighlightTest_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();
  bool bStop = false;
  int nMethod = 0;
  do
  {
    switch (nMethod)
    {
      case 0: 
        pIO->putString(OD_T("Highlighting method selected"));
      break;
      case 1: 
        pIO->putString(OD_T("Unhighlighting method selected"));
      break;
      case 2: 
        pIO->putString(OD_T("Subentities highlighting method selected"));
      break;
      case 3: 
        pIO->putString(OD_T("Subentities unhighlighting method selected"));
      break;
      default:
        pIO->putString(OD_T("No method selected"));
      return;
    }
    OdGePoint3d pt;
    try
    {
      pt = pIO->getPoint(OD_T("Selection point (or type selection method [En Dis SubEn SubDis]):"), OdEd::kGptDefault, NULL, OD_T("En Dis SubEn SubDis"));
    }
    catch (const OdEdKeyword &kwd)
    {
      nMethod = kwd.keywordIndex();
      continue;
    }
    catch (const OdEdCancel &)
    {
      pIO->putString(OD_T("Quit."));
      break;
    }
    OdGsView *pView = _GetGsView_func(pDb);
    if (pView == NULL)
    {
      pIO->putString(OD_T("No GsView available!"));
      return;
    }
    pt.transformBy(pView->worldToDeviceMatrix());
    OdGsDCPoint pts[2];
    OdInt32 pb = pDb->getSysVar(OD_T("PICKBOX"))->getInt16();
    pts[0].x = (long)pt.x - pb;
    pts[0].y = (long)pt.y - pb;
    pts[1].x = (long)pt.x + pb;
    pts[1].y = (long)pt.y + pb;
    _SimplifiedSelectionReactor selRe(nMethod >= 2);
    pView->select(pts, 2, &selRe);
    if (selRe.m_pPath)
    {
      const OdGiPathNode *pLastPath;
      { // path dump
        const OdGiPathNode *pPath = pLastPath = selRe.m_pPath;
        OdString pdStr;
        pdStr.format(OD_T("--> Filtered selection path dump (selmark: %d):"), (OdInt32)pPath->selectionMarker());
        pIO->putString(pdStr);
        while (pPath)
        {
          pdStr.format(OD_T("-> %s : %s"), OdDbObjectId(pPath->persistentDrawableId()).getHandle().ascii().c_str(),
                                           OdDbObjectPtr(OdDbObjectId(pPath->persistentDrawableId()).safeOpenObject())->isA()->name().c_str());
          pIO->putString(pdStr);
          pLastPath = pPath;
          pPath = pPath->parent();
        }
      }
      // make hlt operation
      OdGsCache *pCache = OdDbObjectPtr(OdDbObjectId(pLastPath->persistentDrawableId()).safeOpenObject())->gsNode();
      if (pCache == NULL)
      {
        pIO->putString(OD_T("No GsCache available!"));
        return;
      }
      OdGsModel *pGsModel = pCache->model();
      if (pGsModel == NULL)
      {
        pIO->putString(OD_T("No GsModel available!"));
        return;
      }
      pGsModel->highlight(*selRe.m_pPath, !(nMethod & 1));
      pView->device()->invalidate();
    }
    else
      pIO->putString(OD_T("Nothing selected"));
  } while(!bStop);
}

#include "GeometryFromProxy.h"
void _ConvertAcAdParts_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  //OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();

  OdDbBlockTablePtr pBlockTbl = pDb->getBlockTableId().openObject();
  OdDbSymbolTableIteratorPtr  block_it(pBlockTbl->newIterator());
  while (!block_it->done())
  {
    OdDbBlockTableRecordPtr pBlock(block_it->getRecordId().safeOpenObject());
    block_it->step();
    OdDbObjectIteratorPtr pEntIter;
    for(pEntIter = pBlock->newIterator(); !pEntIter->done(); pEntIter->step())
    {
      OdDbProxyEntityPtr pProxy = OdDbProxyEntity::cast(pEntIter->entity(OdDb::kForWrite) );
      if (!pProxy.isNull())
      {
        if (pProxy->originalClassName() == L"AcAdPart")
        {
          OdModelerGeometryPtr pModelerGeometry;
          if (odGetSatFromProxy(pProxy, pModelerGeometry))
          {
            OdSmartPtr<OdStreamBuf> pOut = OdMemoryStream::createNew();
            pModelerGeometry->out(pOut, kAfTypeAny);
            pOut->rewind();
            OdDb3dSolidPtr p3dSolid = OdDb3dSolid::createObject();
            if (p3dSolid->acisIn(pOut.get()) == eOk)
            {
              pProxy->handOverTo(p3dSolid);
            }
          }
        }
      }
    }
  }
}
#include "DbMLeader.h"

static OdDbObjectId createMleader(OdDbDatabase* pDb, OdDbBlockTableRecord* ms, OdDbMLeaderStyle::TextAttachmentType ta)
{
  OdDbMLeaderPtr ml = OdDbMLeader::createObject();
  ml->setLandingGap(3.0);
  ml->setDoglegLength(10.0);
  ml->setDatabaseDefaults(pDb);
  // ml->setMLeaderStyle(createMleaderStyleTextHorz(pDb));
  ml->setTextAttachmentType(ta, OdDbMLeaderStyle::kLeftLeader);
  ml->setTextAttachmentType(ta, OdDbMLeaderStyle::kRightLeader);
  OdDbMTextPtr pMText = OdDbMText::createObject();
  pMText->setDatabaseDefaults(pDb);
  pMText->setContents( L"{\\Ftxt|c0;MText MText MText}" );
  const double textHeight = 100;
  const double textWidth = 50;

  pMText->setLocation(OdGePoint3d(-200, -155, 0));
  pMText->setTextHeight(textHeight);
  pMText->setWidth(textWidth);
  pMText->setAttachment(OdDbMText::kTopLeft);
  ml->setTextHeight(100);
  ml->setMText(pMText); 
  OdDbObjectId id = ms->appendOdDbEntity(ml);
  int lindex;
  ml->addLeader(lindex);
  ml->addLeaderLine(OdGePoint3d(800,0,0), lindex);
  OdDbAnnotativeObjectPEPtr(ml)->setAnnotative(ml, true);
  return id;
}

static OdDbObjectId createMleader1(OdDbDatabase* pDb, OdDbBlockTableRecord* ms)
{
  OdDbMLeaderPtr destMLeader = OdDbMLeader::createObject();
  destMLeader->setDatabaseDefaults(pDb);
  destMLeader->setEnableLanding(false);
  destMLeader->setEnableDogleg(true);
  destMLeader->setLandingGap(2);
  destMLeader->setDoglegLength(10);

  // Set up attached object
  destMLeader->setContentType(OdDbMLeaderStyle::kMTextContent);
  OdGePoint3d attachPoint(47, -70, 0);

  destMLeader->setTextAttachmentDirection(OdDbMLeaderStyle::kAttachmentVertical);

  // Now export out actual text entity
  OdDbMTextPtr pMTextOut = OdDbMText::createObject();
  pMTextOut->setDatabaseDefaults(pDb);
  pMTextOut->setAttachment(OdDbMText::kTopRight);
  pMTextOut->setRotation(0);
  pMTextOut->setLocation(attachPoint);
  pMTextOut->setTextHeight(5);
  pMTextOut->setWidth(35.32);
  pMTextOut->setContents( L"Default Text" );

  destMLeader->setMText(pMTextOut);

  OdCmColor textColour;
  textColour.setColorIndex(2);
  destMLeader->setTextColor(textColour);
  destMLeader->setTextHeight(5);
  destMLeader->setTextAlignmentType(OdDbMLeaderStyle::kRightAlignment);			// ?
  destMLeader->setTextAngleType(OdDbMLeaderStyle::kHorizontalAngle);
  destMLeader->setTextAttachmentType(OdDbMLeaderStyle::kAttachmentBottomOfTop, OdDbMLeaderStyle::kLeftLeader);
  destMLeader->setTextAttachmentType(OdDbMLeaderStyle::kAttachmentBottomOfTop, OdDbMLeaderStyle::kRightLeader);
  destMLeader->setTextAttachmentType(OdDbMLeaderStyle::kAttachmentCenter, OdDbMLeaderStyle::kTopLeader);
  destMLeader->setTextAttachmentType(OdDbMLeaderStyle::kAttachmentCenter, OdDbMLeaderStyle::kBottomLeader);
  destMLeader->setEnableFrameText(true);
  destMLeader->setTextLocation(attachPoint);
  OdDbObjectId id = ms->appendOdDbEntity(destMLeader);
  int lindex;
  destMLeader->addLeader(lindex);
  destMLeader->addLeaderLine(OdGePoint3d(200,0,0), lindex);
  //OdDbAnnotativeObjectPEPtr(destMLeader)->setAnnotative(ml, true);
  return id;
}
static OdDbObjectId createMleader2(OdDbDatabase* pDb, OdDbBlockTableRecord* ms)
{
  OdDbMLeaderPtr destMLeader = OdDbMLeader::createObject();
  destMLeader->setDatabaseDefaults(pDb);
  destMLeader->setArrowSize(4.0);
  destMLeader->setEnableLanding(true);
  destMLeader->setEnableDogleg(true);
  destMLeader->setLandingGap(2.0);
  destMLeader->setDoglegLength(8.0);
  destMLeader->setContentType(OdDbMLeaderStyle::kBlockContent);

  OdDbObjectId blockId;
  OdDbBlockTablePtr pBlocks = OdDbBlockTable::cast(pDb->getBlockTableId().openObject(OdDb::kForWrite));
  if (!pBlocks->has(L"AnnoBlock2"))
  {
    OdDbBlockTableRecordPtr pBlock = OdDbBlockTableRecord::createObject();
    pBlock->setName(OD_T("AnnoBlock2"));
    blockId = pBlocks->add(pBlock);
    OdDbCirclePtr pCircle = OdDbCircle::createObject();
    pCircle->setDatabaseDefaults(pDb);
    pBlock->appendOdDbEntity(pCircle);
    pBlock->setOrigin(OdGePoint3d(1000, 1000, 0));
    pCircle->setCenter(OdGePoint3d::kOrigin);
    pCircle->setRadius(0.5);
    OdDbAttributeDefinitionPtr pAttr = OdDbAttributeDefinition::createObject();
    pAttr->setDatabaseDefaults(pDb);
    pBlock->appendOdDbEntity(pAttr);
    pAttr->setPrompt(OD_T("Enter Attr: "));
    pAttr->setTag(OD_T("Attr"));
    pAttr->setHorizontalMode(OdDb::kTextCenter);
    pAttr->setHeight(0.2);
    pAttr->setTextString(OD_T("Block"));
  }

  destMLeader->setBlockContentId(blockId);
  destMLeader->setBlockRotation(0.5);
  destMLeader->setBlockPosition(OdGePoint3d(2090.284, 1792.082, 0.0));
  destMLeader->setBlockConnectionType(OdDbMLeaderStyle::kConnectExtents);
  //destMLeader->setBlockScale(1.5);

  // Set leader vertices and line info
  int leaderIndex = 0;
  destMLeader->addLeaderLine(OdGePoint3d(2017.618, 1730.847, 0.0), leaderIndex);
  // no extra vertices added to this leader line
  return ms->appendOdDbEntity(destMLeader);
}
static OdDbObjectId createMleader3(OdDbDatabase* pDb, OdDbBlockTableRecord* ms)
{
  OdDbObjectId contentBlockId;
  OdDbBlockTablePtr pBlocks = OdDbBlockTable::cast(pDb->getBlockTableId().openObject(OdDb::kForWrite));
  if (!pBlocks->has(L"AnnoBlock2"))
  {
    OdDbBlockTableRecordPtr pBlock = OdDbBlockTableRecord::createObject();
    pBlock->setName(L"AnnoBlock2");
    contentBlockId = pBlocks->add(pBlock);

    OdDbPolylinePtr pPl = OdDbPolyline::createObject();
    pPl->setDatabaseDefaults(pDb);
    pBlock->appendOdDbEntity(pPl);

    pBlock->setOrigin(OdGePoint3d(0.0,0.3,0));

    pPl->addVertexAt(0, OdGePoint2d(0.0, 0.0));
    pPl->addVertexAt(1, OdGePoint2d(1.0, 0.0));
    pPl->addVertexAt(2, OdGePoint2d(1.0, 1.0));
    pPl->addVertexAt(3, OdGePoint2d(0.0, 1.0));
    pPl->addVertexAt(4, OdGePoint2d(0.0, 0.0));
    //pPl->transformBy(OdGeMatrix3d::translation(OdGeVector3d(10.0, 0, 0)));

/*    OdDbAttributeDefinitionPtr pAttr = OdDbAttributeDefinition::createObject();
    pAttr->setDatabaseDefaults(pDb);
    pBlock->appendOdDbEntity(pAttr);
    pAttr->setPrompt(L"Enter Attr: ");
    pAttr->setTag(L"Attr");
    pAttr->setHorizontalMode(OdDb::kTextCenter);
    pAttr->setHeight(0.2);
    pAttr->setTextString(L"Block");*/
  }

  OdDbObjectId arrowBlockId;
  if (!pBlocks->has(L"ArrowBlock2"))
  {
    OdDbBlockTableRecordPtr pBlock = OdDbBlockTableRecord::createObject();
    pBlock->setName(L"ArrowBlock2");
    pBlock->setComments(L"Block for MLeader arrow heads.");
    arrowBlockId = pBlocks->add(pBlock);
    OdDbPolylinePtr pPl = OdDbPolyline::createObject();
    pPl->setDatabaseDefaults(pDb);
    pBlock->appendOdDbEntity(pPl);
    pPl->addVertexAt(0, OdGePoint2d(0.0, -0.5));
    pPl->addVertexAt(1, OdGePoint2d(0.5, 0.0));
    pPl->addVertexAt(2, OdGePoint2d(0.0, 0.5));
    pPl->addVertexAt(3, OdGePoint2d(-0.5, 0.0));
    pPl->addVertexAt(4, OdGePoint2d(0.0, -0.5));
    pPl->transformBy(OdGeMatrix3d::scaling(2.0));
  }

  OdDbMLeaderPtr pMLeader = OdDbMLeader::createObject();
  OdDbObjectId styleId2;
  {
    OdDbDictionaryPtr pMLeaderStyleDic = pDb->getMLeaderStyleDictionaryId().safeOpenObject(OdDb::kForWrite);
    OdDbMLeaderStylePtr pMLeaderStyle = OdDbMLeaderStyle::createObject();
    pMLeaderStyle->setDatabaseDefaults(pDb);
    pMLeaderStyle->setContentType(OdDbMLeaderStyle::kBlockContent);
    styleId2 = pMLeaderStyleDic->setAt(L"BlockMLeaderStyle2", pMLeaderStyle);

    pMLeaderStyle->setBlockId(contentBlockId);
    pMLeaderStyle->setBlockScale(OdGeScale3d(0.5, 0.5, 0.5));
    //pMLeaderStyle->setBlockRotation(0.1);
    pMLeaderStyle->setArrowSymbolId(arrowBlockId);
    pMLeaderStyle->setDoglegLength(0.4);
    pMLeaderStyle->setArrowSize(0.5);
    pMLeaderStyle->setLandingGap(0.0);
  }
  pMLeader->setDatabaseDefaults(pDb);
  ms->appendOdDbEntity(pMLeader);
  pMLeader->setMLeaderStyle(styleId2);
  pMLeader->setBlockScale(OdGeScale3d(0.5, 0.5, 0.5));
  //pMLeader->setBlockRotation(0.1);
  pMLeader->setBlockPosition(OdGePoint3d(12.2, 1.0, 0.0));
  int llIndex;
  pMLeader->addLeaderLine(OdGePoint3d(12.0, 1.5, 0.0), llIndex);
  pMLeader->addFirstVertex(llIndex, OdGePoint3d(12.7, 1.9, 0.0));
  pMLeader->addFirstVertex(llIndex, OdGePoint3d(12.3, 2.3, 0.0));

  pMLeader->addLeaderLine(OdGePoint3d(13.0, 1.65, 0.0), llIndex);
  pMLeader->addFirstVertex(llIndex, OdGePoint3d(13.0, 2.3, 0.0));

  pMLeader->setArrowSize(pMLeader->arrowSize() * 0.5);
  return pMLeader->objectId();
}

static OdDbObjectId createMleader4(OdDbDatabase* pDb, OdDbBlockTableRecord* ms)
{
  OdDbMLeaderPtr destMLeader = OdDbMLeader::createObject();
  destMLeader->setDatabaseDefaults(pDb);
  destMLeader->setEnableLanding(false);
  destMLeader->setEnableDogleg(true);
  destMLeader->setLandingGap(2);
  destMLeader->setDoglegLength(10);

  // Set up attached object
  destMLeader->setContentType(OdDbMLeaderStyle::kMTextContent);
  destMLeader->setTextAttachmentDirection(OdDbMLeaderStyle::kAttachmentHorizontal);

  // Now export out actual text entity
  OdDbMTextPtr pMTextOut = OdDbMText::createObject();
  pMTextOut->setDatabaseDefaults(pDb);
  pMTextOut->setAttachment(OdDbMText::kTopRight);
  pMTextOut->setRotation(0);
  pMTextOut->setLocation(OdGePoint3d(-100, 50, 0));
  pMTextOut->setTextHeight(10);
  pMTextOut->setWidth(35.32);
  pMTextOut->setContents( L"Default Text" );

  destMLeader->setMText(pMTextOut);

  OdCmColor textColour;
  textColour.setColorIndex(2);
  destMLeader->setTextColor(textColour);
  destMLeader->setTextHeight(10);
  destMLeader->setTextAlignmentType(OdDbMLeaderStyle::kRightAlignment);			// ?
  destMLeader->setTextAngleType(OdDbMLeaderStyle::kHorizontalAngle);
  destMLeader->setTextAttachmentType(OdDbMLeaderStyle::kAttachmentBottomLine, OdDbMLeaderStyle::kLeftLeader);
  destMLeader->setTextAttachmentType(OdDbMLeaderStyle::kAttachmentBottomLine, OdDbMLeaderStyle::kRightLeader);
  destMLeader->setTextAttachmentType(OdDbMLeaderStyle::kAttachmentCenter, OdDbMLeaderStyle::kTopLeader);
  destMLeader->setTextAttachmentType(OdDbMLeaderStyle::kAttachmentCenter, OdDbMLeaderStyle::kBottomLeader);
  destMLeader->setEnableFrameText(false);
  destMLeader->setTextLocation(OdGePoint3d(-100, 50, 0));
  OdDbObjectId id = ms->appendOdDbEntity(destMLeader);
  int lindex;
  destMLeader->addLeader(lindex);
  destMLeader->addLeaderLine(OdGePoint3d(0,0,0), lindex);
  //OdDbAnnotativeObjectPEPtr(destMLeader)->setAnnotative(ml, true);
  return id;
}

static OdDbObjectId createMleader5(OdDbDatabase* pDb, OdDbBlockTableRecord* ms)
{
  OdDbMTextPtr pMText = OdDbMText::createObject();
  pMText->setDatabaseDefaults(pDb);
  pMText->setTextHeight(10.5);
  pMText->setContents("This is a callout\\Pwith 2 lines of text\\P");
  pMText->setLocation( OdGePoint3d( 101.43502758599440, 360.94155581764551, 0 ) );
  pMText->setAttachment(OdDbMText::kTopLeft);
  OdDbMLeaderPtr pLeader = OdDbMLeader::createObject();
  pLeader->setDatabaseDefaults( pDb );
  OdDbObjectId id = ms->appendOdDbEntity(pLeader);

  pLeader->setTextAttachmentType(OdDbMLeaderStyle::kAttachmentBottomLine,OdDbMLeaderStyle::kRightLeader);

  OdGePoint3d startPoint;
  startPoint.x = 277.26944188591835;
  startPoint.y = 192.65679303518613;
  int llIndex;
  pLeader->addLeaderLine( startPoint, llIndex );
  pLeader->setMText( pMText );
  return id;
}
static OdDbObjectId createMleader6(OdDbDatabase* pDb, OdDbBlockTableRecord* ms)
{
  OdDbMTextPtr pMText = OdDbMText::createObject();
  pMText->setDatabaseDefaults(pDb);
  pMText->setTextHeight(3);
  pMText->setContents("6-%HC D.H\\Panother line");
  pMText->setAttachment(OdDbMText::kTopCenter);
  pMText->setLocation(OdGePoint3d(575.45407882601262, 517.47073178890116,0) );
  OdDbMLeaderPtr pLeader = OdDbMLeader::createObject();
  pLeader->setDatabaseDefaults( pDb );
  OdDbObjectId id = ms->appendOdDbEntity(pLeader);

  pLeader->setTextAttachmentType(OdDbMLeaderStyle::kAttachmentBottomLine,OdDbMLeaderStyle::kLeftLeader);

  OdGePoint3d startPoint;
  startPoint.x = 549.78001105575754;
  startPoint.y = 486.96800216249574;
  int llIndex;
  pLeader->addLeaderLine( startPoint, llIndex );
  pLeader->setMText( pMText );
  return id;
}

static OdDbObjectId createMleader7(OdDbDatabase* pDb, OdDbBlockTableRecord* ms)
{
  OdDbMTextPtr pMText = OdDbMText::createObject();
  pMText->setDatabaseDefaults(pDb);
  pMText->setTextHeight(3);
  pMText->setContents("6-%HC D.H\\Panother line");
  pMText->setAttachment(OdDbMText::kTopCenter);
  pMText->setLocation(OdGePoint3d(575.45407882601262, 517.47073178890116,0) );
  OdDbMLeaderPtr pLeader = OdDbMLeader::createObject();
  pLeader->setDatabaseDefaults( pDb );
  OdDbObjectId id = ms->appendOdDbEntity(pLeader);

  pLeader->setTextAttachmentDirection(OdDbMLeaderStyle::kAttachmentVertical);
  pLeader->setTextAttachmentType(OdDbMLeaderStyle::kAttachmentBottomLine,OdDbMLeaderStyle::kLeftLeader);
  pLeader->setTextAttachmentType(OdDbMLeaderStyle::kAttachmentLinedCenter,OdDbMLeaderStyle::kBottomLeader);

  OdGePoint3d startPoint;
  startPoint.x = 549.78001105575754;
  startPoint.y = 486.96800216249574;
  int llIndex;
  pLeader->addLeaderLine( startPoint, llIndex );
  pLeader->setMText( pMText );
  return id;
}

static OdDbObjectId createMleader8(OdDbDatabase* pDb, OdDbBlockTableRecord* ms)
{
  OdDbMLeaderPtr pLeader = OdDbMLeader::createObject();
  pLeader->setDatabaseDefaults(pDb);
  OdDbObjectId id = ms->appendOdDbEntity(pLeader);
  pLeader->setContentType(OdDbMLeaderStyle::kNoneContent);
  OdGePoint3d startPoint;
  startPoint.x = 10;
  startPoint.y = 10;
  int llIndex;
  pLeader->addLeaderLine(startPoint, llIndex);
  startPoint.x += 20;
  pLeader->addFirstVertex(llIndex, startPoint);
  startPoint.x += 20;
  startPoint.y += 20;
  pLeader->addFirstVertex(llIndex, startPoint);
  return id;
}


void _AddMleader_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbBlockTableRecordPtr pBTR = pDb->getModelSpaceId().safeOpenObject(OdDb::kForWrite);
  //createMleader(pDb, pBTR, OdDbMLeaderStyle::kAttachmentTopOfTop);
  //createMleader2(pDb, pBTR);
  //createMleader3(pDb, pBTR);
  //createMleader4(pDb, pBTR);
  //createMleader7(pDb, pBTR);
/*  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();
  OdDbMLeaderPtr pLeader = OdDbObjectId(pIO->select()->newIterator()->id()).safeOpenObject();
  OdIntArray leaderLineIndexes;
  pLeader->getLeaderLineIndexes(leaderLineIndexes);
  pIO->putString(OdString().format(L"leaderLineIndexes.len = %d", leaderLineIndexes.length()));*/
  OdDbMLeaderPtr ml = createMleader8(pDb, pBTR).safeOpenObject(OdDb::kForWrite);
//  OdDbAnnotativeObjectPEPtr(ml)->setAnnotative(ml, true);
//  OdDbObjectContextInterfacePtr(ml)->addContext(ml, *pDb->objectContextManager()->contextCollection(ODDB_ANNOTATIONSCALES_COLLECTION)->getContext(L"1:2"));
}
void _DimsScale_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();
  ClassFilter fltr(OdDbDimension::desc());
  OdDbSelectionSetPtr pFirstSel = pIO->select(OD_T("Select dimension entity"), OdEd::kSelSingleEntity, 0, OdString::kEmpty, &fltr);
  if (pFirstSel->numEntities() == 0)
  {
    pIO->putString(OD_T("Nothing selected."));
    return;
  }
  OdDbDimensionPtr d = pFirstSel->objectIdArray().first().safeOpenObject();
  pIO->putString(OdString().format(L"dimscale = %g", d->dimscale()));
}

void _SubDMeshCode_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabase* pDatabase = pDbCmdCtx->database();
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();
  ClassFilter fltr(OdDbSubDMesh::desc());
  OdAnsiString tmp;

  OdDbSelectionSetPtr pFirstSel = pIO->select(OD_T("Select SubDMesh entity"), OdEd::kSelSingleEntity, 0, OdString::kEmpty, &fltr);
  if (pFirstSel->numEntities() == 0)
  {
    pIO->putString(OD_T("Nothing selected."));
    return;
  }

  OdString txtPath = pDbCmdCtx->dbUserIO()->getFilePath(L"Select file to save", OdEd::kGfpForSave, L"Select file to save", L"txt", L"", "TXT files (*.txt)|*.txt");
  if (txtPath.isEmpty())
  {
    pIO->putString(OD_T("No file selected."));
    return;
  }

  OdDbSubDMeshPtr pMesh = pFirstSel->objectIdArray().first().safeOpenObject();
  OdStreamBufPtr pOut = odrxSystemServices()->createFile(txtPath, Oda::kFileWrite ,Oda::kShareDenyNo, Oda::kCreateAlways);

  tmp.format("// created from file : %ls\n"
             "// entity handle 0x%ls\n\n"
             "OdDbSubDMeshPtr createMesh()\n"
             "{\n"
             "  OdDbSubDMeshPtr pSubDMesh = OdDbSubDMesh::createObject();\n\n"
             "  OdGePoint3dArray vertexArray;\n"
             "  OdInt32Array faceArray;\n"
             , pDatabase->getFilename().c_str(), pMesh->objectId().getHandle().ascii().c_str());
  pOut->putBytes(tmp.c_str(), tmp.getLength());

  OdGePoint3dArray vertexArray;
  OdInt32Array faceArray;
  OdInt32 result;
  pMesh->getVertices(vertexArray);
  pMesh->getFaceArray(faceArray);
  pMesh->subdLevel(result);


  tmp.format("  const double vertexes[] = {\n");
  pOut->putBytes(tmp.c_str(), tmp.getLength());
  OdUInt32 f;
  for(f=0; f<vertexArray.size(); ++f){
    tmp.format("% .16f,% .16f,% .16f%s%s", vertexArray[f].x, vertexArray[f].y, vertexArray[f].z, (f==vertexArray.size()-1)?"":",", (f+1)%3 ? "" : "\n");
    pOut->putBytes(tmp.c_str(), tmp.getLength());
  }

  tmp.format("  };\n\n"
             "  OdUInt32 f, k = sizeof(vertexes)/sizeof(vertexes[0]);\n"
             "  vertexArray.resize(k/3);\n"
             "  for(f=0; f<k; f+=3) vertexArray[f/3].set(vertexes[f], vertexes[f+1], vertexes[f+2]);\n\n"
             "  const OdInt32 faces[] = {\n");
  pOut->putBytes(tmp.c_str(), tmp.getLength());

  for(f=0; f<faceArray.size(); ++f){
    tmp.format("%3d%s%s", faceArray[f], (f==faceArray.size()-1)?"":",", (f+1)%20 ? "" : "\n");
    pOut->putBytes(tmp.c_str(), tmp.getLength());
  }

  tmp.format(
    "  };\n\n"
    "  k = sizeof(faces)/sizeof(faces[0]);\n"
    "  faceArray.resize(k);\n"
    "  for(f=0; f<k; ++f) faceArray[f] = faces[f];\n\n"
    "  pSubDMesh->setSubDMesh(vertexArray, faceArray, %d);\n\n"
    "  const double creases[] = {\n"
    , result);
  pOut->putBytes(tmp.c_str(), tmp.getLength());

  OdDbSubentId id(OdDb::kEdgeSubentType, 0);
  double dResult;
  for(f = 0;eOk == pMesh->getCrease(id, dResult); ++f){
    tmp.format("%s%g%s", (f==0)?" ":",", dResult, (f+1)%20 ? "" : "\n");
    pOut->putBytes(tmp.c_str(), tmp.getLength());
    id.setIndex(f+1);
  }

  tmp.format(
    "  };\n\n"
    "  k = sizeof(creases)/sizeof(creases[0]);\n"
    "  for(f=0; f<k; ++f) {\n"
    "    OdDbFullSubentPathArray subentPaths;\n"
    "    subentPaths.append(OdDbFullSubentPath(OdDb::kEdgeSubentType, f));\n"
    "    pSubDMesh->setCrease(subentPaths, creases[f]);\n"
    "  }\n"
    "\n  return pSubDMesh;\n}\n");
  pOut->putBytes(tmp.c_str(), tmp.getLength());
}


#include "CommonTests.h"

typedef int (*CommonTestFunc) (OdDbObject* pObj);



void executeCommonTest(OdEdCommandContext* pCmdCtx, CommonTestFunc pFunc, const OdChar* pTestName)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  //OdDbDatabase* pDatabase = pDbCmdCtx->database();
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();

  OdDbSelectionSetPtr pSSet = pIO->select(OdString(L"Select objects"),
    OdEd::kSelAllowInactSpaces|OdEd::kSelAllowObjects|OdEd::kSelAllowPSVP);

  OdDbSelectionSetIteratorPtr pIter = pSSet->newIterator();

  if (pIter->done())
  {
    pIO->putString(L"Nothing selected.");
    return;
  }
  while(!pIter->done())
  {
    OdDbObjectId idObj = pIter->objectId();
    OdString strMessage(pTestName);
    strMessage += odDbGetHandleName(idObj.getHandle()).c_str();
    strMessage += L' ';
    try
    {
      OdDbObjectPtr pObj = idObj.safeOpenObject(OdDb::kForWrite);
      strMessage += pObj->isA()->name().c_str();
      int nRes = (*pFunc)(pObj);
      if (nRes < 0)
        strMessage += L" - OK";
      else
      {
        OdString strErr;
        strErr.format(L" - Failed at %d", nRes);
        strMessage += strErr;
      }
    }
    catch (const OdError& err)
    {
      strMessage += L" - OdError: ";
      strMessage += err.description().c_str();
    }
    catch (...)
    {
      strMessage += L" - Unknown error";
    }
    pIO->putString(strMessage);

    pIter->next();
  }
}


void _DXF_IN_OUT_func(OdEdCommandContext* pCmdCtx)
{
  executeCommonTest(pCmdCtx, dxfIoTest, L"Test DXF In/Out ");
}

void _DWG_IN_OUT_UNDO_func(OdEdCommandContext* pCmdCtx)
{
  executeCommonTest(pCmdCtx, dwgIoUndoTest, L"Test DWG In/Out (Undo) ");
}

void _TRANSFORM_BY_func(OdEdCommandContext* pCmdCtx)
{
  executeCommonTest(pCmdCtx, transformByTest, L"Test tranformBy ");
}

/////////////////////////////////////////////////////////////////////////////

class DwgToResBufFiler : public OdStaticRxObject<OdDbDwgFiler>
{
  OdResBufPtr m_ResBuf;
  OdResBufPtr m_CurResBuf;
  bool m_bFirstStep;
  mutable OdDbDatabasePtr m_pDb;
private:
  OdResBufPtr next()
  {
    if (m_bFirstStep)
    {
      m_bFirstStep = false;
      return m_CurResBuf;
    }

    m_CurResBuf = m_CurResBuf->setNext(OdResBuf::createObject());
    return m_CurResBuf;
  }
public:
  DwgToResBufFiler(OdDbDatabasePtr pDb)
  {
    m_pDb = pDb;
    m_ResBuf = OdResBuf::createObject();
    m_CurResBuf = m_ResBuf;
    m_bFirstStep= true;
  }
  OdResBufPtr getResults()
  {
    return m_ResBuf;
  }

  // Filer methods implementation
  virtual void seek(
    OdInt64 offset, 
    OdDb::FilerSeekType seekType) {
      throw OdResult(eNotImplemented);
  }

  /** Description:
  Returns the current I/O pointer position of this Filer object.
  */
  virtual OdUInt64 tell() const {
    throw OdResult(eNotImplemented);
    return 0;
  }

  /** Description:
  Returns the bool *value* of the current item of this Filer object.
  */
  virtual bool rdBool() {
    throw OdResult(eNotImplemented);
    return false;
  }

  /** Description:
  Returns the *string* *value* of the current item of this Filer object.
  Arguments:
  */
  virtual OdString rdString() {
    throw OdResult(eNotImplemented);
    return "";
  }


  /** Description:
  Returns the specified number of *bytes* from this Filer object.
  Arguments:
  buffer (O) Receives the bytes.
  numBytes (I) Number of *bytes*.
  */
  virtual void rdBytes(
    void* buffer, 
    OdUInt32 numBytes) {
      throw OdResult(eNotImplemented);
  }


  /** Description:
  Returns the Int8 *value* of the current item of this Filer object.
  */
  virtual OdInt8 rdInt8() {
    throw OdResult(eNotImplemented);
    return 0;
  }

  /** Description:
  Returns the UInt8 *value* of the current item of this Filer object.
  */
  virtual OdUInt8 rdUInt8() {
    throw OdResult(eNotImplemented);
    return 0;
  }

  /** Description:
  Returns the Int16 *value* of the current item of this Filer object.
  */
  virtual OdInt16 rdInt16() {
    throw OdResult(eNotImplemented);
    return 0;
  }

  /** Description:
  Returns the Int32 *value* of the current item of this Filer object.
  */
  virtual OdInt32 rdInt32() {
    throw OdResult(eNotImplemented);
  }

  /** Description:
  Returns the Int64 *value* of the current item of this Filer object.
  */
  virtual OdInt64 rdInt64() {
    throw OdResult(eNotImplemented);
    return 0;
  }

  /** Description:
  Returns the memory address *value* of the current item of this Filer object.
  */
  //virtual void* rdAddress();

  /** Description:
  Returns the double *value* of the current item of this Filer object.
  */
  virtual double rdDouble() {
    throw OdResult(eNotImplemented);
    return 0;
  }

  /** Description:
  Returns the *handle* *value* of the current item of this Filer object.
  */
  virtual OdDbHandle rdDbHandle() {
    throw OdResult(eNotImplemented);
    return 0;
  }

  /** Description:
  Returns the SoftOwnershipId *value* of the current item of this Filer object.
  */
  virtual OdDbObjectId rdSoftOwnershipId() {
    throw OdResult(eNotImplemented);
    return 0;
  }

  /** Description:
  Returns the HardOwnershipId *value* of the current item of this Filer object.
  */
  virtual OdDbObjectId rdHardOwnershipId() {
    throw OdResult(eNotImplemented);
    return 0;
  }

  /** Description:
  Returns the HardPointerId *value* of the current item of this Filer object.
  */
  virtual OdDbObjectId rdHardPointerId() {
    throw OdResult(eNotImplemented);
    return 0;
  }

  /** Description:
  Returns the SoftPointerId *value* of the current item of this Filer object.
  */
  virtual OdDbObjectId rdSoftPointerId() {
    throw OdResult(eNotImplemented);
    return 0;
  }

  /** Description:
  Returns the 2D *point* *value* of the current item of this Filer object.
  */
  virtual OdGePoint2d rdPoint2d() {
    throw OdResult(eNotImplemented);
    return OdGePoint2d::kOrigin;
  }

  /** Description:
  Returns the 3D *point* *value* of the current item of this Filer object.
  */
  virtual OdGePoint3d rdPoint3d() {
    throw OdResult(eNotImplemented);
  }

  /** Description:
  Returns the 2D *vector* *value* of the current item of this Filer object.
  */
  virtual OdGeVector2d rdVector2d() {
    throw OdResult(eNotImplemented);
    return OdGeVector2d::kIdentity;
  }

  /** Description:
  Returns the 3D vector *value* of the current item of this Filer object.
  */
  virtual OdGeVector3d rdVector3d() {
    throw OdResult(eNotImplemented);
    return OdGeVector3d::kIdentity;
  }

  /** Description:
  Returns the *scale* *value* of the current item of this Filer object.
  */
  virtual OdGeScale3d rdScale3d() {
    throw OdResult(eNotImplemented);
    return OdGeScale3d::kIdentity;
  }

  /** Description:
  Writes the specified bool *value* to this Filer object.
  Arguments:
  value (I) Value.
  */  
  virtual void wrBool(
    bool value) {
      OdResBufPtr cur= next();
      cur->setRestype(OdResBuf::kRtBool);
      cur->setBool(value);
  }

  /** Description:
  Writes the specified *string* *value* to this Filer object.
  Arguments:
  value (I) Value.
  */  
  virtual void wrString(
    const OdString &value) {
      OdResBufPtr cur= next();
      cur->setRestype(OdResBuf::kRtString);
      cur->setString(value);
  }

  /** Description:
  Writes the specified number of *bytes* to this Filer object.
  Arguments:
  buffer (I) array of *bytes*.
  numBytes (I) Number of *bytes*.
  */
  virtual void wrBytes(
    const void* buffer, 
    OdUInt32 numBytes) {
      OdResBufPtr cur= next();
      cur->setRestype(OdResBuf::kRtBinaryChunk);
      OdBinaryData data;
      for(OdUInt32 f=0; f<numBytes; ++f)
        data.append(((OdUInt8*)buffer)[f]);
      cur->setBinaryChunk(data);
  }

  /** Description:
  Writes the specified Int8 *value* to this Filer object.
  Arguments:
  value (I) Value.
  */  
  virtual void wrInt8(
    OdInt8 value) {
      OdResBufPtr cur= next();
      cur->setRestype(OdResBuf::kRtInt8);
      cur->setInt8(value);
  }

  /** Description:
  Writes the specified UInt8 *value* to this Filer object.
  Arguments:
  value (I) Value.
  */  
  virtual void wrUInt8(
    OdUInt8 value) {
      OdResBufPtr cur= next();
      cur->setRestype(OdResBuf::kRtInt8);
      cur->setInt8(value);
  }

  /** Description:
  Writes the specified Int16 *value* to this Filer object.
  Arguments:
  value (I) Value.
  */  
  virtual void wrInt16(
    OdInt16 value) {
      OdResBufPtr cur= next();
      cur->setRestype(OdResBuf::kRtInt16);
      cur->setInt16(value);
  }

  /** Description:
  Writes the specified Int32 *value* to this Filer object.
  Arguments:
  value (I) Value.
  */  
  virtual void wrInt32(
    OdInt32 value) {
      OdResBufPtr cur= next();
      cur->setRestype(OdResBuf::kRtInt32);
      cur->setInt32(value);
  }

  /** Description:
  Writes the specified Int64 *value* to this Filer object.
  Arguments:
  value (I) Value.
  */  
  virtual void wrInt64(
    OdInt64 value) {
      OdResBufPtr cur= next();
      cur->setRestype(OdResBuf::kDxfInt64);
      cur->setInt64(value);
  }

  /** Description:
  Writes the specified memory address *value* to this Filer object.
  Arguments:
  value (I) Value.
  */  
  //virtual void wrAddress(
  //  const void* value);

  /** Description:
  Writes the specified double *value* to this Filer object.
  Arguments:
  value (I) Value.
  */  
  virtual void wrDouble(double value) {
    OdResBufPtr cur= next();
    cur->setRestype(OdResBuf::kRtDouble);
    cur->setDouble(value);
  }

  /** Description:
  Writes the specified *handle* *value* to this Filer object.
  Arguments:
  value (I) Value.
  */  
  virtual void wrDbHandle(
    const OdDbHandle& value) {
      OdResBufPtr cur= next();
      cur->setRestype(OdResBuf::kRtHandle);
      cur->setHandle(value);
  }

  /** Description:
  Writes the specified SoftOwnershipId *value* to this Filer object.
  Arguments:
  value (I) Value.
  */  
  virtual void wrSoftOwnershipId(
    const OdDbObjectId& value) {
      OdResBufPtr cur= next();
      cur->setRestype(OdResBuf::kRtSoftOwnershipId);
      cur->setObjectId(value);
  }

  /** Description:
  Writes the specified HardOwnershipId *value* to this Filer object.
  Arguments:
  value (I) Value.
  */  
  virtual void wrHardOwnershipId(
    const OdDbObjectId& value) {
      OdResBufPtr cur= next();
      cur->setRestype(OdResBuf::kRtHardOwnershipId);
      cur->setObjectId(value);
  }

  /** Description:
  Writes the specified SoftPointerId *value* to this Filer object.
  Arguments:
  value (I) Value.
  */  
  virtual void wrSoftPointerId(
    const OdDbObjectId& value) {
      OdResBufPtr cur= next();
      cur->setRestype(OdResBuf::kRtSoftPointerId);
      cur->setObjectId(value);
  }

  /** Description:
  Writes the specified HardPointerId *value* to this Filer object.
  Arguments:
  value (I) Value.
  */  
  virtual void wrHardPointerId(
    const OdDbObjectId& value) {
      OdResBufPtr cur= next();
      cur->setRestype(OdResBuf::kRtHardPointerId);
      cur->setObjectId(value);
  }

  /** Description:
  Writes the specified 2D *point* *value* to this Filer object.
  Arguments:
  value (I) Value.
  */  
  virtual void wrPoint2d(
    const OdGePoint2d& value) {
      OdResBufPtr cur= next();
      cur->setRestype(OdResBuf::kRtPoint2d);
      cur->setPoint2d(value);
  }

  /** Description:
  Writes the specified 3D *point* *value* to this Filer object.
  Arguments:
  value (I) Value.
  */  
  virtual void wrPoint3d(
    const OdGePoint3d& value) {
      OdResBufPtr cur= next();
      cur->setRestype(OdResBuf::kRtPoint3d);
      cur->setPoint3d(value);
  }

  /** Description:
  Writes the specified 2D *vector* *value* to this Filer object.
  Arguments:
  value (I) Value.
  */  
  virtual void wrVector2d(
    const OdGeVector2d& value) {
      OdResBufPtr cur= next();
      cur->setRestype(OdResBuf::kRtVector2d);
      cur->setVector2d(value);
  }

  /** Description:
  Writes the specified 3D *vector* *value* to this Filer object.
  Arguments:
  value (I) Value.
  */  
  virtual void wrVector3d(
    const OdGeVector3d& value) {
      OdResBufPtr cur= next();
      cur->setRestype(OdResBuf::kRtVector3d);
      cur->setVector3d(value);
  }

  /** Description:
  Writes the specified 3D *scale* *value* to this Filer object.
  Arguments:
  value (I) Value.
  */  
  virtual void wrScale3d(
    const OdGeScale3d& value) {
      OdResBufPtr cur= next();
      cur->setRestype(OdResBuf::kRtVector3d);
      cur->setVector3d(OdGeVector3d(value.sx, value.sy, value.sz));
  }

  OdDbFiler::FilerType filerType(void) const
  {
    return OdDbFiler::kFileFiler;
  }

  virtual OdDbDatabase* database() const
  {
    return m_pDb.get();
  }
};

OdResBufPtr dwgToResBuf(const OdDbObject *pObj)
{
  if (!pObj)
    return OdResBufPtr();

  DwgToResBufFiler filer(pObj->database());

  pObj->dwgOutFields(&filer);

  return filer.getResults();
}

void executeRoundTripTest(OdEdCommandContext* pCmdCtx, const OdDbSelectionSet* pSSet, 
                          OdDb::SaveType type, OdDb::DwgVersion ver, bool bWithPause = true,
                          bool bWithAssertAtPlace = false)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabase* pDb = pDbCmdCtx->database();
  if (!pDb)
    throw OdError (eNoDatabase);

  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();

  OdDbSelectionSetIteratorPtr pIter = pSSet->newIterator();

  OdDbHandle hFailed;
  bool bFailed = false;

  if (pIter->done())
  {
    pIO->putString(L"Nothing selected.");
    return;
  }
  // Save drawing and read it back into a copy
  OdDbHostAppServices* pHostApp = pDb->appServices();
  OdMemoryStreamPtr pStream = OdMemoryStream::createNew();
  pDb->writeFile(pStream, type, ver);
  pStream->seek(0, OdDb::kSeekFromStart);
  OdDbDatabasePtr pDbCopy = pHostApp->readFile(pStream);

  int nSuccess = 0;
  int nFailed = 0;
  OdString strMessage;

  while(!pIter->done())
  {
    OdDbObjectId idObj = pIter->objectId();
    if (!bFailed)
      hFailed = idObj.getHandle();
    strMessage = L"Round trip for ";
    strMessage += odDbGetHandleName(idObj.getHandle()).c_str();
    strMessage += L' ';
    ++nFailed;
    try
    {
      OdDbObjectPtr pObj = idObj.safeOpenObject(OdDb::kForWrite);
      strMessage += pObj->isA()->name().c_str();

      OdDbObjectPtr pCopy = pDbCopy->getOdDbObjectId(idObj.getHandle()).safeOpenObject();

      int nRes = 0;
      if (type == OdDb::kDxf)
      {
        OdResBufPtr pRb1 = oddbEntGet(pObj, L"*");
        OdResBufPtr pRb2 = oddbEntGet(pCopy, L"*");
        // Skip ent name
        pRb1 = pRb1->next();
        pRb2 = pRb2->next();

        if (type != OdDb::kDwg)
        {
          // fix for class OdDbViewRep in dataFiles/dwg/ACADSamples/Budweiser/2013/budweiser2013.dwg
          OdGeTol svTol = OdGeContext::gTol;
          OdGeContext::gTol = OdGeTol(1.0e-9, 1.0e-9); 
          nRes = compareResBufs(pRb1, pRb2);
          OdGeContext::gTol = svTol; // restore
        }
        else
          nRes = compareResBufs(pRb1, pRb2);
      }
      else
      {
        OdResBufPtr pRb1 = dwgToResBuf(pObj);
        OdResBufPtr pRb2 = dwgToResBuf(pCopy);
        // Skip ent name
        //pRb1 = pRb1->next();
        //pRb2 = pRb2->next();
        
        nRes = compareResBufs(pRb1, pRb2);
      }

      if (nRes < 0)
      {
        strMessage += L" - OK";
        --nFailed;
        ++nSuccess;
        if (!bFailed)
          hFailed = 0;
      }
      else
      {
        OdString strErr;
        strErr.format(L" - Failed at %d", nRes);
        strMessage += strErr;
       #ifdef ODA_DIAGNOSTICS
        if (bWithAssertAtPlace)
        {
          // get assertion with needed call stack
          OdResBuf::setAssertIndexByNewRb(nRes + 1);
          oddbEntGet(pCopy, L"*"); 
          OdResBuf::setAssertIndexByNewRb(nRes + 1);
          oddbEntGet(pObj, L"*"); 
        }
       #endif
      }
    }
    catch (const OdError& err)
    {
      strMessage += L" - OdError: ";
      strMessage += err.description().c_str();
    }
    catch (...)
    {
      strMessage += L" - Unknown error";
    }
    pIO->putString(strMessage);

    if (!bFailed && hFailed)
      bFailed = true;

    pIter->next();
  }

  strMessage.format(L"Success: %d", nSuccess);
  pIO->putString(strMessage);
  strMessage.format(L"Failed: %d", nFailed);
  pIO->putString(strMessage);
  if (bWithPause)
    pIO->getString(L"Press Enter to continue");

  if (bFailed)
  {
    OdEdCommandStackPtr pCommands = ::odedRegCmds();
    if (!pCommands.isNull())
    {
      OdEdCommandPtr pCmd = pCommands->lookupCmd(L"DISPLAY_DIFFS");
      if (!pCmd.isNull())
      {
        pCmdCtx->setArbitraryData(L"OdDbDatabasePtr", pDbCopy);

        OdValuePtr pHandle = OdValue::createObject();
        *pHandle = (OdInt64)hFailed;
        pCmdCtx->setArbitraryData(L"Handle", pHandle);
        pCommands->executeCommand(L"DISPLAY_DIFFS", pCmdCtx);
      }
    }
  }
}

void executeRoundTripTest(OdEdCommandContext* pCmdCtx, OdDb::SaveType type, OdDb::DwgVersion ver)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabase* pDb = pDbCmdCtx->database();
  if (!pDb)
    throw OdError (eNoDatabase);

  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();

  OdDbSelectionSetPtr pSSet = pIO->select(OdString(L"Select objects"),
    OdEd::kSelAllowInactSpaces|OdEd::kSelAllowObjects|OdEd::kSelAllowPSVP);

  executeRoundTripTest(pCmdCtx, pSSet, type, ver);
}

///////////////////////////////////////////////////////
void _ROUNDTRIP_DXF14_func(OdEdCommandContext* pCmdCtx)
{
  executeRoundTripTest(pCmdCtx, OdDb::kDxf, OdDb::vAC14);
}

void _ROUNDTRIP_DXF15_func(OdEdCommandContext* pCmdCtx)
{
  executeRoundTripTest(pCmdCtx, OdDb::kDxf, OdDb::vAC15);
}

void _ROUNDTRIP_DXF2004_func(OdEdCommandContext* pCmdCtx)
{
  executeRoundTripTest(pCmdCtx, OdDb::kDxf, OdDb::vAC18);
}

void _ROUNDTRIP_DXF2007_func(OdEdCommandContext* pCmdCtx)
{
  executeRoundTripTest(pCmdCtx, OdDb::kDxf, OdDb::vAC21);
}

void _ROUNDTRIP_DXF2010_func(OdEdCommandContext* pCmdCtx)
{
  executeRoundTripTest(pCmdCtx, OdDb::kDxf, OdDb::vAC24);
}

void _ROUNDTRIP_DXF2013_func(OdEdCommandContext* pCmdCtx)
{
  executeRoundTripTest(pCmdCtx, OdDb::kDxf, OdDb::vAC27);
}

//////////////
void _ROUNDTRIP_DWG14_func(OdEdCommandContext* pCmdCtx)
{
  executeRoundTripTest(pCmdCtx, OdDb::kDwg, OdDb::vAC14);
}

void _ROUNDTRIP_DWG15_func(OdEdCommandContext* pCmdCtx)
{
  executeRoundTripTest(pCmdCtx, OdDb::kDwg, OdDb::vAC15);
}

void _ROUNDTRIP_DWG2004_func(OdEdCommandContext* pCmdCtx)
{
  executeRoundTripTest(pCmdCtx, OdDb::kDwg, OdDb::vAC18);
}

void _ROUNDTRIP_DWG2007_func(OdEdCommandContext* pCmdCtx)
{
  executeRoundTripTest(pCmdCtx, OdDb::kDwg, OdDb::vAC21);
}

void _ROUNDTRIP_DWG2010_func(OdEdCommandContext* pCmdCtx)
{
  executeRoundTripTest(pCmdCtx, OdDb::kDwg, OdDb::vAC24);
}

void _ROUNDTRIP_DWG2013_func(OdEdCommandContext* pCmdCtx)
{
  executeRoundTripTest(pCmdCtx, OdDb::kDwg, OdDb::vAC27);
}

void _ArrayExplode_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabase* pDatabase = pDbCmdCtx->database();
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();

  OdDbSelectionSetPtr pArraySel = pIO->select(OD_T("Select array"), OdEd::kSelSingleEntity);
  
  OdDbEntityPtr pEnt = OdDbEntity::cast( pArraySel->objectIdArray().first().openObject( OdDb::kForWrite ) ); 

  if ( !OdDbAssocArrayActionBody::isAssociativeArray( pEnt ) )
    return;

  OdDbObjectIdArray newEnt;

  pDatabase->startTransaction();
  bool bOk = true;
  try
  {
    OdDbAssocArrayActionBody::explode( pEnt, newEnt );
  }
  catch ( ... )
  {
    bOk = false;
  }

  if ( bOk )
  {
    pDatabase->endTransaction();
    OdDbAssocManager::evaluateTopLevelNetwork( pDatabase );
    pIO->putString( L"Ready" );
  }
  else
  {
    pDatabase->abortTransaction();
    pIO->putString( L"Failed" );
  }
}

void _EvaluateTopLevelNetwork_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabase* pDatabase = pDbCmdCtx->database();
  //pAction->setStatus(kChangedDirectlyAssocStatus); // TODO Select entity and so on
  OdDbAssocManager::evaluateTopLevelNetwork( pDatabase );
}

void _setBasePlane_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabase* pDatabase = pDbCmdCtx->database();
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();

  OdDbSelectionSetPtr pArraySel = pIO->select(OD_T("Select array"), OdEd::kSelSingleEntity);

  OdDbEntityPtr pEnt = OdDbEntity::cast( pArraySel->objectIdArray().first().openObject( OdDb::kForWrite ) ); 

  if ( !OdDbAssocArrayActionBody::isAssociativeArray( pEnt ) )
    return;

  OdGePoint3d Origin = pIO->getPoint( OD_T("Specify \"Origin\"") );
  OdGePoint3d BaseNormal = pIO->getPoint( OD_T("Specify \"BaseNormal\"") );

  OdDbObjectId BodyId = OdDbAssocArrayActionBody::getControllingActionBody( pEnt );
  OdDbAssocArrayActionBodyPtr pBody = BodyId.openObject( OdDb::kForWrite );

  OdDbVertexRefPtr pOriginVertex = OdDbVertexRef::createObject( Origin );

  static_cast<OdDbAssocArrayCommonParameters *>
    ( pBody->parameters().get() )->setBasePlane( *pOriginVertex, BaseNormal - OdGePoint3d(0,0,0));

  OdDbAssocManager::evaluateTopLevelNetwork( pDatabase );

  pIO->putString(OD_T("Ready"));
}

void _setBasePoint_func( OdEdCommandContext* pCmdCtx )
{
  OdDbCommandContextPtr pDbCmdCtx( pCmdCtx );
  OdDbDatabase* pDatabase = pDbCmdCtx->database();
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();

  OdDbSelectionSetPtr pArraySel = pIO->select( OD_T( "Select array" ), OdEd::kSelSingleEntity );

  OdDbEntityPtr pEnt = OdDbEntity::cast( pArraySel->objectIdArray().first().openObject( OdDb::kForWrite ) );

  if ( !OdDbAssocArrayActionBody::isAssociativeArray( pEnt ) )
    return;

  OdGePoint3d BasePoint = pIO->getPoint( OD_T( "Specify \"Base point\"" ) );

  OdDbObjectId BodyId = OdDbAssocArrayActionBody::getControllingActionBody( pEnt );
  OdDbAssocArrayActionBodyPtr pBody = BodyId.openObject( OdDb::kForWrite );
  OdDbVertexRefPtr pBasePointVertex = OdDbVertexRef::createObject( BasePoint );

  pBody->setSourceBasePoint( *pBasePointVertex.get() );

  OdDbAssocActionPtr pAction = pBody->parentAction().openObject( OdDb::kForWrite );
  pAction->setStatus( kChangedDirectlyAssocStatus, true );

  OdDbAssocManager::evaluateTopLevelNetwork( pDatabase );

  pIO->putString( OD_T( "Ready" ) );
}

////////////////////////////////////////////// CENTER MARK start
//void _ExplodeCenterMark_func(OdEdCommandContext* pCmdCtx)
//{
//	OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
//	OdDbDatabase* pDatabase = pDbCmdCtx->database();
//	OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();
//	OdDbSelectionSetPtr pArraySel = pIO->select(OD_T("Select center mark"), OdEd::kSelSingleEntity);
//
//	int numselected = pArraySel->numEntities();
//	
//	if (numselected == 0)
//	{
//		pIO->putString(OD_T("Nothing selected."));
//		return;
//	}
//	
//	OdDbSelectionSetIteratorPtr pIter = pArraySel->newIterator();
//
//	while (!pIter->done())
//	{
//		OdDbEntityPtr pEnt = OdDbEntity::cast(pIter->objectId().openObject(OdDb::kForWrite));
//		OdDbObjectId BodyId = OdDbAssocArrayActionBody::getControllingActionBody(pEnt); //  getControllingActionBody(pEnt) is the same as for OdDbCenterMarkActionBodyPtr
//		OdDbCenterMarkActionBodyPtr pBody;
//
//		try
//		{
//			pBody = BodyId.openObject(OdDb::kForWrite);
//		}
//		catch (const OdError&)
//		{
//	
//		}
//
//		pIter->next();
//	}
//
//	_EvaluateOverrideCenterMark_func(pCmdCtx);
//	pIO->putString(OD_T("Ready"));
//}

void SetValueParamForCenterMark(OdEdCommandContext* pCmdCtx, const OdString& paramName, const OdString& prompt, bool evaluateOnly = false)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabase* pDatabase = pDbCmdCtx->database();
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();
  OdDbSelectionSetPtr pArraySel = pIO->select(OD_T("Select center mark"), OdEd::kSelSingleEntity);

  int numselected = pArraySel->numEntities();

  if (numselected == 0)
  {
    pIO->putString(OD_T("Nothing selected."));
    return;
  }

  OdDbSelectionSetIteratorPtr pIter = pArraySel->newIterator();
  double newValue = 0;

  if (!evaluateOnly)
    newValue = pIO->getReal(prompt);

  while (!pIter->done())
  {
    OdDbEntityPtr pEnt = OdDbEntity::cast(pIter->objectId().openObject(OdDb::kForWrite));
    OdDbObjectId BodyId = OdDbAssocArrayActionBody::getControllingActionBody(pEnt); //  getControllingActionBody(pEnt) is the same as for OdDbCenterMarkActionBodyPtr
    OdDbCenterMarkActionBodyPtr pBody;

    if (!BodyId.isNull())
    {
      try
      {
        pBody = BodyId.openObject(OdDb::kForWrite);
        
        if (!evaluateOnly)
          pBody->setGeometryParam(newValue, paramName);
      }
      catch (const OdError&)
      {

      }

      if (!pBody.isNull())
      {
        OdDbAssocActionPtr pAction = pBody->parentAction().openObject(OdDb::kForWrite);

        pAction->setStatus(kChangedDirectlyAssocStatus, true);
      }

      //OdDbAssocManager::evaluateTopLevelNetwork(pDatabase);
    }

    pIter->next();
  }

  //_EvaluateOverrideCenterMark_func(pCmdCtx);
  OdDbAssocManager::evaluateTopLevelNetwork(pDatabase);

  pIO->putString(OD_T("Ready"));
}

void _EvaluateOverrideCenterMark_func(OdEdCommandContext* pCmdCtx)
{
  SetValueParamForCenterMark(pCmdCtx, OD_T(""), OD_T(""), true);
  //_EvaluateTopLevelNetwork_func(pCmdCtx);
}

void SetStringValueParamForCenterMark(OdEdCommandContext* pCmdCtx, const OdString& paramName, const OdString& prompt)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabase* pDatabase = pDbCmdCtx->database();
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();
  OdDbSelectionSetPtr pArraySel = pIO->select(OD_T("Select center mark"), OdEd::kSelSingleEntity);

  int numselected = pArraySel->numEntities();

  if (numselected == 0)
  {
    pIO->putString(OD_T("Nothing selected."));
    return;
  }

  OdString newStr = pIO->getString(prompt);
  int length = newStr.getLength();
  OdChar lastCh = newStr.getAt(length - 1);
  bool correctString = true;
  newStr.replace('X', 'x');
  int indPoint = newStr.find('.');
  int revIndPoint = newStr.reverseFind('.');

  while (length > 1 && (newStr.getAt(0) == '0' && newStr.getAt(1) != '.'))
  {
    newStr = newStr.right(length - 1);
    length = newStr.getLength();
  }

  if (length == 1 && ((lastCh >= '0' && lastCh <= '9')))
  {
    correctString = true;
  }
  else if ((lastCh == 'x' || lastCh == 'X' || lastCh == '.' || (lastCh >= '0' && lastCh <= '9')) && indPoint == revIndPoint)
  {
    if (lastCh == 'x' || lastCh == 'X')
    {
      length--;
    }

    for (int i = 0; i < length; i++)
    {
      OdChar tempCh = newStr.getAt(i);
      
      if (tempCh == '.' || (tempCh >= '0' && tempCh <= '9'))
      {

      }
      else
      {
        correctString = false;
        break;
      }
    }
  }
  else
    correctString = false;

  OdDbSelectionSetIteratorPtr pIter = pArraySel->newIterator();

  while (!pIter->done() && correctString)
  {
    OdDbEntityPtr pEnt = OdDbEntity::cast(pIter->objectId().openObject(OdDb::kForWrite));
    OdDbObjectId BodyId = OdDbAssocArrayActionBody::getControllingActionBody(pEnt); //  getControllingActionBody(pEnt) is the same as for OdDbCenterMarkActionBodyPtr
    OdDbCenterMarkActionBodyPtr pBody;

    if (!BodyId.isNull())
    {
      try
      {
        pBody = BodyId.openObject(OdDb::kForWrite);
        pBody->setStringGeometryParam(newStr, paramName);
      }
      catch (const OdError&)
      {

      }

      if (!pBody.isNull())
      {
        OdDbAssocActionPtr pAction = pBody->parentAction().openObject(OdDb::kForWrite);

        pAction->setStatus(kChangedDirectlyAssocStatus, true);
      }
      //OdDbAssocManager::evaluateTopLevelNetwork(pDatabase);
    }

    pIter->next();
  }

  //_EvaluateOverrideCenterMark_func(pCmdCtx);
  OdDbAssocManager::evaluateTopLevelNetwork(pDatabase);

  pIO->putString(OD_T("Ready"));
}

void _CrossHairGap_func(OdEdCommandContext* pCmdCtx)
{
  SetStringValueParamForCenterMark(pCmdCtx, OD_T("CrossHairGapExpression"), OD_T("Insert the gap at the intersection"));
}

void _CrossHairLength_func(OdEdCommandContext* pCmdCtx)
{
  SetStringValueParamForCenterMark(pCmdCtx, OD_T("CrossHairLengthExpression"), OD_T("Insert the size of the intersection"));
}

void _LeftLineOvershoot_func(OdEdCommandContext* pCmdCtx)
{
  SetValueParamForCenterMark(pCmdCtx, OD_T("HorizontalStartOvershoot"), OD_T("Insert left overshoot"));
}

void _RightLineOvershoot_func(OdEdCommandContext* pCmdCtx)
{
  SetValueParamForCenterMark(pCmdCtx, OD_T("HorizontalEndOvershoot"), OD_T("Insert right overshoot"));
}

void _TopLineOvershoot_func(OdEdCommandContext* pCmdCtx)
{
  SetValueParamForCenterMark(pCmdCtx, OD_T("VerticalEndOvershoot"), OD_T("Insert top overshoot"));
}

void _BottomLineOvershoot_func(OdEdCommandContext* pCmdCtx)
{
  SetValueParamForCenterMark(pCmdCtx, OD_T("VerticalStartOvershoot"), OD_T("Insert bottom overshoot"));
}

void _ExtentionLinesAreVisible_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabase* pDatabase = pDbCmdCtx->database();
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();
  OdDbSelectionSetPtr pArraySel = pIO->select(OD_T("Select center mark"), OdEd::kSelSingleEntity);

  int numselected = pArraySel->numEntities();

  if (numselected == 0)
  {
    pIO->putString(OD_T("Nothing selected."));
    return;
  }

  OdDbSelectionSetIteratorPtr pIter = pArraySel->newIterator();
  bool newValue = (pIO->getInt(OD_T("Insert 1 for visible or 0 instead")) == 1);
  OdDb::Visibility newVis = newValue ? OdDb::kVisible: OdDb::kInvisible;

  while (!pIter->done())
  {
    OdDbEntityPtr pEnt = OdDbEntity::cast(pIter->objectId().openObject(OdDb::kForWrite));
    OdDbObjectId BodyId = OdDbAssocArrayActionBody::getControllingActionBody(pEnt); //  getControllingActionBody(pEnt) is the same as for OdDbCenterMarkActionBodyPtr
    OdDbCenterMarkActionBodyPtr pBody;

    try
    {
      pBody = BodyId.openObject(OdDb::kForWrite);
      pBody->setExtensionLinesVisibility(newVis);
    }
    catch (const OdError&)
    {

    }

    if (!pBody.isNull())
    {
      OdDbAssocActionPtr pAction = pBody->parentAction().openObject(OdDb::kForWrite);

      pAction->setStatus(kChangedDirectlyAssocStatus, true);

      //OdDbAssocManager::evaluateTopLevelNetwork(pDatabase);
    }

    pIter->next();
  }

  //_EvaluateOverrideCenterMark_func(pCmdCtx);
  OdDbAssocManager::evaluateTopLevelNetwork(pDatabase);

  pIO->putString(OD_T("Ready"));
}

void _SetRotationCenterMark_func(OdEdCommandContext* pCmdCtx)
{
	OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
	OdDbDatabase* pDatabase = pDbCmdCtx->database();
	OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();
	OdDbSelectionSetPtr pArraySel = pIO->select(OD_T("Select center mark"), OdEd::kSelSingleEntity);

	int numselected = pArraySel->numEntities();

	if (numselected == 0)
	{
		pIO->putString(OD_T("Nothing selected."));
		return;
	}

	OdDbSelectionSetIteratorPtr pIter = pArraySel->newIterator();

	OdString odPrompt = OdString(OD_T("Insert rotation of Center Mark in degrees"));
	double newValue = pIO->getReal(odPrompt);//degrees
	while(abs(newValue) > 360)
	{
		newValue = newValue > 0 ? newValue - 360 : newValue + 360;
	}
	
	newValue = newValue > 0 ? newValue : newValue + 360;
	newValue = newValue * OdaPI / 180.0;

	while (!pIter->done())
	{
		OdDbEntityPtr pEnt = OdDbEntity::cast(pIter->objectId().openObject(OdDb::kForWrite));
		OdDbObjectId BodyId = OdDbAssocArrayActionBody::getControllingActionBody(pEnt); //  getControllingActionBody(pEnt) is the same as for OdDbCenterMarkActionBodyPtr
		OdDbCenterMarkActionBodyPtr pBody;

		try
		{
			pBody = BodyId.openObject(OdDb::kForWrite);
			pBody->setRotation(newValue);
		}
		catch (const OdError&)
		{

		}

		//if (!pBody.isNull())
		//{
		//	OdDbAssocActionPtr pAction = pBody->parentAction().openObject(OdDb::kForWrite);

		//	pAction->setStatus(kChangedDirectlyAssocStatus, true);
		//}

		pIter->next();
	}

	OdDbAssocManager::evaluateTopLevelNetwork(pDatabase);

	pIO->putString(OD_T("Ready"));
}

////////////////////////////////////////////// CENTER MARK end


////////////////////////////////////////////// CENTER Line start
void SetValueParamForCenterLine(OdEdCommandContext* pCmdCtx, const OdString& paramName, const OdString& prompt, bool evaluateOnly = false)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabase* pDatabase = pDbCmdCtx->database();
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();
  OdDbSelectionSetPtr pArraySel = pIO->select(OD_T("Select center line"), OdEd::kSelSingleEntity);

  int numselected = pArraySel->numEntities();

  if (numselected == 0)
  {
    pIO->putString(OD_T("Nothing selected."));
    return;
  }

  OdDbSelectionSetIteratorPtr pIter = pArraySel->newIterator();
  double newValue = 0;
  
  if (!evaluateOnly)
    newValue = pIO->getReal(prompt);
  
  while (!pIter->done())
  {
    OdDbEntityPtr pEnt = OdDbEntity::cast(pIter->objectId().openObject(OdDb::kForWrite));
    OdDbObjectId BodyId = OdDbAssocArrayActionBody::getControllingActionBody(pEnt); //  getControllingActionBody(pEnt) is the same as for OdDbCenterMarkActionBodyPtr
    OdDbCenterLineActionBodyPtr pBody;

    try
    {
      pBody = BodyId.openObject(OdDb::kForWrite);

      if (!evaluateOnly)
        pBody->setGeometryParam(newValue, paramName); 
    }
    catch (const OdError&)
    {

    }

    if (!pBody.isNull())
    {
      OdDbAssocActionPtr pAction = pBody->parentAction().openObject(OdDb::kForWrite);

      pAction->setStatus(kChangedDirectlyAssocStatus, true);

      //OdDbAssocManager::evaluateTopLevelNetwork(pDatabase);
    }

    pIter->next();
  }

  //_EvaluateOverrideCenterMark_func(pCmdCtx);
  OdDbAssocManager::evaluateTopLevelNetwork(pDatabase);

  pIO->putString(OD_T("Ready"));
}

void _StartOvershoot_func(OdEdCommandContext* pCmdCtx)
{
  SetValueParamForCenterLine(pCmdCtx, OD_T("StartOvershoot"), OD_T("Insert start overshoot"));
}

void _EndOvershoot_func(OdEdCommandContext* pCmdCtx)
{
  SetValueParamForCenterLine(pCmdCtx, OD_T("EndOvershoot"), OD_T("Insert end overshoot"));
}

void _EvaluateOverrideCenterLine_func(OdEdCommandContext* pCmdCtx)
{
  SetValueParamForCenterLine(pCmdCtx, OD_T("EndOvershoot"), OD_T("Insert end overshoot"), true);
  //_EvaluateTopLevelNetwork_func(pCmdCtx);
}
////////////////////////////////////////////// CENTER LINE end

void _XrefUnloadAll_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbXRefMan::unloadAll(pDbCmdCtx->database());
}

void _xrefAttach_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbUserIO* const pIO = pDbCmdCtx->dbUserIO();
  OdDbDatabase* pDb = pDbCmdCtx->database();
  OdString xrefPath = pIO->getFilePath(L"Select file to attach", OdEd::kGfpForOpen, L"Attach Xref", OdString::kEmpty, OdString::kEmpty, L"*.dwg|*.dwg");
  xrefPath.replace('/', '\\');
  OdString blockName = xrefPath.mid(xrefPath.reverseFind('\\') + 1);
  OdDbBlockTableRecordPtr pBlock = OdDbXRefManExt::addNewXRefDefBlock(pDb, xrefPath, blockName, false);
  if (!pBlock.isNull() && eOk == OdDbXRefMan::load(pBlock))
  {
    OdDbBlockReferencePtr pRef = OdDbBlockReference::createObject();
    pRef->setDatabaseDefaults(pDb);
    pRef->setNormal(OdGeVector3d::kZAxis);
    pRef->setRotation(0.0);
    OdGePoint3d defaultInsertionPoint;
    pRef->setPosition(pIO->getPoint("Insertion point:", 0, &defaultInsertionPoint));
    pRef->setScaleFactors(OdGeScale3d::kIdentity);
    pRef->setBlockTableRecord(pBlock->objectId());

    OdDbBlockTableRecordPtr pModelSpace = pDb->getModelSpaceId().safeOpenObject(OdDb::kForWrite);
    pModelSpace->appendOdDbEntity(pRef);
  }
}


#include "ExCustEntity.h"
void _PROXY_XTEST_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCtx = pCmdCtx;
  OdDbDatabase* pDb = const_cast<OdDbDatabase*>(pDbCtx->database());
  OdDbLinePtr l = OdDbLine::createObject();
  l->setDatabaseDefaults(pDb);
  OdDbBlockTableRecordPtr ms = pDb->getModelSpaceId().safeOpenObject(OdDb::kForWrite);
  ms->appendOdDbEntity(l);
  l->createExtensionDictionary();
  l->setStartPoint(OdGePoint3d(0, 0, 0));
  l->setEndPoint(OdGePoint3d(1, 0, 0));
  OdDbDictionaryPtr pDic = l->extensionDictionary().safeOpenObject(OdDb::kForWrite);
  OdSmartPtr<ExCustEntity> ce = ExCustEntity::createObject();
  ce->setDatabaseDefaults(pDb);
  pDic->setAt(L"P", ce);
}

void _PROXY_XTEST1_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCtx = pCmdCtx;
  OdDbDatabase* pDb = const_cast<OdDbDatabase*>(pDbCtx->database());
  OdDbLinePtr l = OdDbLine::createObject();
  l->setDatabaseDefaults(pDb);
  OdDbBlockTableRecordPtr ms = pDb->getModelSpaceId().safeOpenObject(OdDb::kForWrite);
  ms->appendOdDbEntity(l);
  l->createExtensionDictionary();
  l->setStartPoint(OdGePoint3d(0, 0, 0));
  l->setEndPoint(OdGePoint3d(1, 0, 0));

  OdSmartPtr<ExCustEntity> ce = ExCustEntity::createObject();
  ce->setDatabaseDefaults(pDb);
  ce->setCenter(OdGePoint3d(0, 0, 0));
  ce->setRadius(0.5);
  ce->setCount(4);
  ms->appendOdDbEntity(ce);
//  pDic->setAt(L"P", ce);
}


void _Intersect_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();

  OdDbObjectIdArray objectIdsArray = OdDbSelectionSet::cast(pIO->select(L"", OdEd::kSelAllowObjects|OdEd::kSelAllowInactSpaces).get())->objectIdArray();

  if (objectIdsArray.size() < 2)
    return;

  OdDbEntityPtr pEnt1 = objectIdsArray[0].openObject();
  OdDbEntityPtr pEnt2 = objectIdsArray[1].openObject();

  OdGePoint3dArray pts;
  OdResult res = pEnt1->intersectWith(pEnt2, OdDb::kExtendBoth, pts);
  int n = pts.size();
  if (n)
  {
    OdDbBlockTableRecordPtr pSpace = activeBlockTableRecord(pDb).openObject(OdDb::kForWrite);
    pDb->setPDMODE(96);
    while (n--)
    {
      OdDbPointPtr pPoint = OdDbPoint::createObject();
      pPoint->setPosition(pts[n]);
      pPoint->setColorIndex(1);
      pPoint->setDatabaseDefaults(pDb);
      pSpace->appendOdDbEntity(pPoint);
    }
  }
  return;
}

void _ATTSYNC_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();
  // this version of attsync emulates Acad "ATTSYNC S" command
  for (OdDbSelectionSetIteratorPtr i = OdDbSelectionSet::cast(pIO->select(L"Select block reference").get())->newIterator(); !i->done(); i->next())
  {
    OdDbBlockReferencePtr br = OdDbBlockReference::cast(i->objectId().openObject(OdDb::kForWrite));
    if (br.isNull())
      continue;
    OdGeMatrix3d xform = br->blockTransform();
    typedef std::map<OdString, OdDbObjectId> ObjectMap;
    ObjectMap attributes, definitions;
    for (OdDbObjectIteratorPtr ai = br->attributeIterator(); !ai->done(); ai->step())
    {
      OdDbAttributePtr a = ai->entity();
      attributes[a->tag()] = a->objectId();
    }
    OdDbBlockTableRecordPtr btr = br->blockTableRecord().safeOpenObject();
    for (OdDbObjectIteratorPtr oi = btr->newIterator(); !oi->done(); oi->step())
    {
      if (!oi->entity()->isKindOf(OdDbAttributeDefinition::desc()))
        continue;
      OdDbAttributeDefinitionPtr a = oi->entity();
      if (a->isConstant())
        continue;
      definitions[a->tag()] = a->objectId();
    }
    for (ObjectMap::iterator it = definitions.begin(); it != definitions.end(); ++it)
    {
      ObjectMap::iterator it2 = attributes.find(it->first);
      if (it2 == attributes.end()) // new definition
      {
        OdDbAttributePtr atr = OdDbAttribute::createObject();
        br->appendAttribute(atr);
        OdDbAttributeDefinitionPtr ad = it->second.safeOpenObject();
        atr->setAttributeFromBlock(ad, xform);
      }
      else // save value, update all else
      {
        OdDbAttributePtr atr = it2->second.safeOpenObject(OdDb::kForWrite);
        OdString s = atr->textString();
        OdDbMTextPtr mt = atr->getMTextAttribute();
        OdString content;
        if ( !mt.isNull() )
          content = mt->contents();
        OdDbAttributeDefinitionPtr ad = it->second.safeOpenObject();
        atr->setAttributeFromBlock(ad, xform);
        atr->setTextString(s);
        if (!mt.isNull())
        {
          OdDbMTextPtr mt1 = atr->getMTextAttribute();
          if (!mt1.isNull())
            mt1->setContents(content); // mt->contents());
        }
      }
    }
    for (ObjectMap::iterator it = attributes.begin(); it != attributes.end(); ++it)
    {
      if (definitions.find(it->first) == definitions.end())
        it->second.safeOpenObject(OdDb::kForWrite)->erase();
    }
  }
}

TOOLKIT_EXPORT OdDbDictionaryVarPtr odDbGetDictionaryVar(const OdDbDatabase * pDb,  const OdString& name,  OdDb::OpenMode mode);

void _POLYDISPLAY_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();
  OdDbDictionaryVarPtr v = odDbGetDictionaryVar(pDb, L"POLYDISPLAY", OdDb::kForRead);
  OdString val = v.isNull() ? L"0" : v->value().c_str();
  OdString newval = pIO->getString(OdString("POLYDISPLAY <") + val + L">: ", 0, val);
  if (newval != val)
  {
    if (v.isNull())
      v = odDbGetDictionaryVar(pDb, L"POLYDISPLAY", OdDb::kForWrite);
    else
      v->upgradeOpen();
    v->setValue(newval);
  }
}

#include "DbOle2Frame.h"
#include "OleItemHandler.h"
#include "MemoryStream.h"

void _SaveStorage_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();

  OdDbSelectionSetIteratorPtr pIter = pIO->select(L"Select Ole2Frame entity",OdEd::kSelAllowObjects|OdEd::kSelAllowInactSpaces)->newIterator();
  if (pIter->done())
    return;
  OdDbObjectIdArray objects;
  for (;!pIter->done();pIter->next())
  {
    OdDbOle2FramePtr pOle = OdDbOle2Frame::cast(pIter->objectId().openObject());
    if (pOle.isNull())
      continue;

    const OdOleItemHandler* pHandler = pOle->itemHandler();
    if (!pHandler)
    {
      pIO->putString(L"No handler\n");
      continue;
    }

    OdMemoryStreamPtr pStr = OdMemoryStream::createNew();
    pHandler->getCompoundDocument(*pStr.get());
    pStr->rewind();
    const OdUInt8 magic[] = {0xD0, 0xCF, 0x11, 0xE0, 0xA1, 0xB1, 0x1A, 0xE1};
    OdUInt8 actMagic[sizeof(magic)];
    pStr->getBytes(actMagic, sizeof(magic));

    OdUInt64 nStart = 0;
    if (memcmp(magic, actMagic, sizeof(magic)))
    {
      nStart = 22;
      pStr->seek(22, OdDb::kSeekFromStart);
      pStr->getBytes(actMagic, sizeof(magic));
      if (memcmp(magic, actMagic, sizeof(magic)))
      {
        pIO->putString(L"Failed\n");
        continue;
      }
    }
    OdStreamBufPtr pFile = ::odSystemServices()->createFile(
      pIO->getFilePath( L"Enter file name to save", OdEd::kGfpForSave,
                        L"Save Compound Document", L"ole",
                        OdString::kEmpty, // Default name
                        L"Compound Document (*.ole)|*.ole||"),
      Oda::kFileWrite,
      Oda::kShareDenyReadWrite,
      Oda::kCreateAlways);

    pStr->copyDataTo(pFile, nStart);
  }
}

void _Commit_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  if (!OdTfRevisionControl::getRepository(pDb))
    return;
  OdTfRevisionControl::commit(pDb, OdTfCommitAnnotation(L"Debug", pCmdCtx->userIO()->getString(L"Enter commit message:")));
}

void _StepBack_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdString branch = OdTfRevisionControl::getLocalBranch(pDb);
  if (branch.isEmpty())
    return;
  OdTfRepository* repo = OdTfRevisionControl::getRepository(pDb);
  OdTfDigest sha;
  repo->getReference(branch, sha);
  OdTfCommitInfo ci = repo->getCommitInfo(sha);
  repo->setReference(branch, ci.parents[0]);
  OdTfRevisionControl::switchBranch(pDb, branch);
}

void _CreateBranch_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();

  if (OdTfRepository* repo = OdTfRevisionControl::getRepository(pDb))
  {
    OdTfDigest sha;
    repo->getReference(OdTfRevisionControl::getLocalBranch(pDb), sha);

    OdString branch = pCmdCtx->userIO()->getString(L"Enter new branch name:");
    repo->setReference(branch, sha);
    OdTfRevisionControl::switchBranch(pDb, branch, true);
  }
}

void _SwitchBranch_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();

  if (!OdTfRevisionControl::getRepository(pDb))
    return;

  OdString branch = pCmdCtx->userIO()->getString(L"Enter branch name:");
  OdTfRevisionControl::switchBranch(pDb, branch, true);
}

void _MergeBranch_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();

  OdString currentBranch = OdTfRevisionControl::getLocalBranch(pDb);
  if (currentBranch.isEmpty())
    return;

  OdString branchFrom = pCmdCtx->userIO()->getString(L"Enter branch name to merge from:");
  if (branchFrom == currentBranch)
  {
    pCmdCtx->userIO()->putString(L"Can't merge from a branch to itself\n");
    return;
  }

  bool bNoFastForward = (pDbCmdCtx->dbUserIO()->getKeyword(L"No FastForward? (Y\\N)", L"Y N", 1) == 0);
  
  bool bRes = false;
  try
  {
    OdTfFixedMergeHandler<OdTfRevisionControl::kMergeMineFull> mergeHandler  = OdTfFixedMergeHandler<OdTfRevisionControl::kMergeMineFull>();
    bRes = OdTfRevisionControl::merge(pDb, branchFrom, mergeHandler, bNoFastForward);

    if (bRes)
      pCmdCtx->userIO()->putString(L"Branch merged successfuly.\n");
    else
      pCmdCtx->userIO()->putString(L"Merge error.\n");

    if (bRes)
    {
      OdString sDescr;
      sDescr.format(L"Merge branch \"%ls\" into \"%ls\"", branchFrom.c_str(), currentBranch.c_str());
      pCmdCtx->userIO()->putString(sDescr);

      OdTfRevisionControl::commit(pDb, OdTfCommitAnnotation(OD_T("Debug"), sDescr));
      pCmdCtx->userIO()->putString(L"Merge commit was created");
    }
  }
  catch (OdError& err)
  {
    pCmdCtx->userIO()->putString(err.description());
  }
}

static void addCommitParents(OdTfRepository* repo, std::map<OdTfDigest, OdTfCommitInfo> &mapCommits, OdTfCommitInfo &cp)
{
  int nParents = cp.parents.size();
  for (int i = 0; i < nParents; ++i)
  {
    OdTfDigest parentDigest = cp.parents[i];
    if (mapCommits.find(parentDigest) == mapCommits.end())
    {
      OdTfCommitInfo cp = repo->getCommitInfo(parentDigest);
      mapCommits[parentDigest] = cp;
      addCommitParents(repo, mapCommits, cp);
    }
  }
}

void _ExplodeStorage_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();

  OdTfRepository* repo = OdTfRevisionControl::getRepository(pDb);
  if (!repo)
    return;

  OdTfRevisionControl::ReferenceList branchList;
  repo->getReferences(branchList);

  std::map<OdTfDigest, OdTfCommitInfo> mapCommits;

  std::map<OdString, OdTfDigest>::iterator it = branchList.begin();
  while (it != branchList.end())
  {
    OdString branchName = it->first;
    OdTfDigest top = it->second;

    if (mapCommits.find(top) == mapCommits.end())
    {
      OdTfCommitInfo ct = repo->getCommitInfo(top);
      mapCommits[top] = ct;

      addCommitParents(repo, mapCommits, ct);
    }
    ++it;
  }

  if (mapCommits.size() == 0)
    pCmdCtx->userIO()->putString(L"No commits in the storage.");

  std::multimap<OdInt64, OdTfDigest> mapCommitsChrono;
  for (std::map<OdTfDigest, OdTfCommitInfo>::iterator itCommits = mapCommits.begin();
    itCommits != mapCommits.end();
    ++itCommits)
  {
    mapCommitsChrono.insert(std::pair<OdInt64, OdTfDigest>(itCommits->second.annotation.date.packedValue(), itCommits->first));
  }

  OdString outputPath = pCmdCtx->userIO()->getString(L"Enter output folder path:");
  if (outputPath.isEmpty())
  {
    pCmdCtx->userIO()->putString(L"Output folder path isn't defined\n");
    return;
  }

  if (outputPath.reverseFind('\\') != outputPath.getLength() - 1)
  {
    outputPath += L"\\";
  }

  int counter = 0;
  for (
    std::multimap<OdInt64, OdTfDigest>::iterator itCommits = mapCommitsChrono.begin();
    itCommits != mapCommitsChrono.end();
    ++itCommits)
  {
    OdTfCommitInfo ci = mapCommits[itCommits->second];
    try
    {
      OdString revName = ci.annotation.message;
      if (revName.isEmpty())
        revName = OD_T("Unnamed_revision");
      revName.replace(" ", "_");
      revName.replace("\"", "_");
      revName.replace("\\", "_");

      OdString fileName;
      fileName.format(OD_T("%d_%s.dwg"), ++counter, revName.c_str());
      fileName = outputPath + fileName;

      pCmdCtx->userIO()->putString(OD_T("Start exporting into ") + fileName);
      repo->retrieve(itCommits->second)->writeFile(fileName, OdDb::kDwg, OdDb::vAC32);
      pCmdCtx->userIO()->putString(OD_T("Export into ") + fileName + OD_T(" finished"));
    }
    catch (OdError &err)
    {
      pCmdCtx->userIO()->putString(err.description());
    }
  }
}

#ifdef OBJTOOLKIT_ENABLED

#include "OBJToolkit.h"
using namespace OBJToolkit;
void _OBJExport_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();

  OdString fileName = pIO->getString(OD_T("Enter file name :"));
  if (fileName.isEmpty())
  {
    pIO->putString(OD_T("Incorrect filename entered : ") + fileName);
    return;
  }

  fileName.replace(OdChar('\\'), OdChar('/'));
  if (fileName.find(OD_T(".obj")) == -1)
    fileName += ".obj";

  int key = pIO->getKeyword(OD_T("Export current space [Y\\N] :"), "Y N", 0);

  OdDbSelectionSetPtr pSSet;
  if (key == 1)
  {
    pSSet = pIO->select(OD_T("Select entities to export in single file"), 0);
  }

  OdObjToolkitModulePtr module = loadObjToolkitModule();
  if (module.isNull())
  {
    pIO->putString(OD_T("Cannot load module : ") + OdString(OdObjToolkitModuleName));
    return;
  }

  OdDbDatabase *pDb = pDbCmdCtx->database();

  OdResult res;
  if (key == 0)
  {
    OdObjExportOptions options;
    options.m_applicationComment = OD_T("Teigha OdaMfcApp");

    //OdDbLayoutPtr pLayout = pDb->currentLayoutId().safeOpenObject(OdDb::kForRead);
    //OdDbBlockTableRecordPtr pSpace = pLayout->getBlockTableRecordId().safeOpenObject(OdDb::kForRead);
    //res = module->exportObj(pDb, pSpace, fileName, &options);
    res = module->exportObj(pDb, fileName, &options);
  }
  else
  {
    OdObjExportOptions options;
    options.m_applicationComment = OD_T("Teigha OdaMfcApp selection export");
    options.m_bFillDb = true;

    int cntExported = 0;
    res = eSubSelectionSetEmpty;
    OdDbSelectionSetIteratorPtr pIter = pSSet->newIterator();
    while (!pIter->done())
    {
      OdDbObjectPtr pObject = pIter->objectId().openObject();

      // To collect entities in OdObjDb pass fileName empty:
      OdResult expRes = module->exportObj(pDb, pObject, OD_T(""), &options);
      if (expRes == eOk)
        ++cntExported;
      pIter->next();
    }

    if (cntExported > 0)
    {
      OdObjDbPtr pObjDb = options.m_pObjDb;
      if (!pObjDb.isNull() && !pObjDb->isEmpty())
      {
        OdMtlDbPtr pMtlDb = pObjDb->getMtlDb();
        if (!pMtlDb.isNull() && pMtlDb->getMaterialsCount() > 0)
        {
          int slashPos = fileName.reverseFind(L'/') + 1;
          OdString sPath = fileName.mid(0, slashPos);
          module->updateTexturesPaths(pMtlDb, pDb, sPath, &options);
        }

        OdObjExportPtr pObjExporter = createObjExporter();
        pObjExporter->setObjDb(pObjDb);
        res = pObjExporter->exportFile(fileName, options.m_applicationComment);
      }
      else
        res = eNoDatabase;
    }
  }

  if (res == eOk)
  {
    pIO->putString(OD_T("Exported successfully."));
  }
  else
  {
    OdString tmp = OD_T("Export error: ") + OdError(res).description();
    pIO->putString(tmp);
  }
}

#else

void _OBJExport_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();

  pIO->putString(OD_T("Components should be enabled for export into OBJ format..."));
}

#endif

// CreateBreps
#include "ExCreateBreps.h"

namespace ExCreateBreps
{
  void setColor(OdDbEntityPtr pEnt, OdUInt8 r, OdUInt8 g, OdUInt8 b)
  {
    if (pEnt.isNull())
    {
      throw OdError(eNullPtr);
    }

    OdCmColor color;
    color.setRGB(r, g, b);
    pEnt->setColor(color);
  }

  struct GridPos
  {
    OdGeVector3d current;
    OdGeVector3d prev;
    double dx;
    double dy;

    GridPos(double dx, double dy)
      : current(OdGeVector3d::kIdentity)
      , prev(current)
      , dx(dx)
      , dy(dy)
    {}

    OdGeVector3d operator()() { current.x += dx; return current; }
    void nextRow() { prev.y += dy; current = prev; }
  };
}


void _CreateBreps_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();

  OdRxClassPtr pCreatorService = odrxGetModelerGeometryCreatorService();
  if (pCreatorService.isNull())
  {
    pIO->putString(OD_T("Creator service not loaded"));
    return;
  }

  OdDbHostAppServices* pHostApp = pDb->appServices();

  try
  {
    OdDbBodyPtr pBody;
    OdDb3dSolidPtr pSolid;
    OdDbRegionPtr pRegion;

    ExCreateBreps::GridPos pos(10., 10.);

    ExCreateBreps::addCylinder(pHostApp, pDb, false, pos(), &pBody);
    ExCreateBreps::setColor(pBody, 0x00, 0xff, 0x00);

    ExCreateBreps::addNurbSurface(pHostApp, pDb, false, pos(), &pBody);
    ExCreateBreps::setColor(pBody, 0xff, 0x00, 0x00);

    ExCreateBreps::addCone(pHostApp, pDb, false, false, pos(), &pBody);
    ExCreateBreps::setColor(pBody, 0x00, 0x00, 0xff);
    ExCreateBreps::addCone(pHostApp, pDb, true, false, pos(), &pBody);
    ExCreateBreps::setColor(pBody, 0x00, 0x00, 0xff);

    ExCreateBreps::addRegionWithHole(pHostApp, pDb, pos(), &pRegion);
    ExCreateBreps::setColor(pRegion, 0xff, 0x00, 0x00);

    pos.nextRow();
    ExCreateBreps::addTorus(pHostApp, pDb, ExCreateBreps::ttDoughnut, pos(), &pSolid);
    ExCreateBreps::setColor(pSolid, 0x00, 0xff, 0xff);
    ExCreateBreps::addTorus(pHostApp, pDb, ExCreateBreps::ttApple, pos(), &pSolid);
    ExCreateBreps::setColor(pSolid, 0x00, 0xff, 0xff);
    ExCreateBreps::addTorus(pHostApp, pDb, ExCreateBreps::ttVortex, pos(), &pSolid);
    ExCreateBreps::setColor(pSolid, 0x00, 0xff, 0xff);
    ExCreateBreps::addTorus(pHostApp, pDb, ExCreateBreps::ttLemon, pos(), &pSolid);
    ExCreateBreps::setColor(pSolid, 0x00, 0xff, 0xff);
    ExCreateBreps::addTorusQuarter(pHostApp, pDb, pos(), &pSolid);
    ExCreateBreps::setColor(pSolid, 0x00, 0xff, 0xff);

    pos.nextRow();
    ExCreateBreps::addSphere(pHostApp, pDb, pos(), &pSolid);
    ExCreateBreps::setColor(pSolid, 0x00, 0x00, 0xff);

    ExCreateBreps::addHemiSphere(pHostApp, pDb, pos(), &pSolid);
    ExCreateBreps::setColor(pSolid, 0x00, 0x00, 0xff);

    ExCreateBreps::addPyramid(pHostApp, pDb, pos());

    ExCreateBreps::addBox(pHostApp, pDb, pos(), &pSolid);
    ExCreateBreps::setColor(pSolid, 0xff, 0x00, 0xff);

    ExCreateBreps::addSeamedCylinder(pHostApp, pDb, pos(), &pSolid);
    ExCreateBreps::setColor(pSolid, 0xff, 0x00, 0xff);

    ExCreateBreps::addEllipCylinder(pHostApp, pDb, pos(), &pSolid);
    ExCreateBreps::setColor(pSolid, 0xff, 0xff, 0xcc);

    ExCreateBreps::addEllipCone(pHostApp, pDb, pos(), &pSolid);
    ExCreateBreps::setColor(pSolid, 0xff, 0xff, 0xcc);

    pos.nextRow();//revolve
    ExCreateBreps::addRotSurface1(pHostApp, pDb, pos(), false, &pSolid);
    ExCreateBreps::setColor(pSolid, 0x00, 0xa0, 0xa0);
    ExCreateBreps::addRotSurface2(pHostApp, pDb, pos(), false, &pSolid);
    ExCreateBreps::setColor(pSolid, 0xa0, 0xa0, 0x00);

    ExCreateBreps::addRotSurface3(pHostApp, pDb, pos(), false, &pSolid);
    ExCreateBreps::setColor(pSolid, 0x00, 0xa0, 0x00);
    ExCreateBreps::addRotSurface4(pHostApp, pDb, pos(), false, &pSolid);
    ExCreateBreps::setColor(pSolid, 0x00, 0x00, 0xa0);

    ExCreateBreps::addRotSurface5(pHostApp, pDb, pos(), false, &pSolid);
    ExCreateBreps::setColor(pSolid, 0xa0, 0x00, 0x00);

    pos.nextRow();//spun
    ExCreateBreps::addRotSurface1(pHostApp, pDb, pos(), true, &pSolid);
    ExCreateBreps::setColor(pSolid, 0x00, 0xa0, 0xa0);
    ExCreateBreps::addRotSurface2(pHostApp, pDb, pos(), true, &pSolid);
    ExCreateBreps::setColor(pSolid, 0xa0, 0xa0, 0x00);

    ExCreateBreps::addRotSurface3(pHostApp, pDb, pos(), true, &pSolid);
    ExCreateBreps::setColor(pSolid, 0x00, 0xa0, 0x00);
    ExCreateBreps::addRotSurface4(pHostApp, pDb, pos(), true, &pSolid);
    ExCreateBreps::setColor(pSolid, 0x00, 0x00, 0xa0);

    ExCreateBreps::addRotSurface5(pHostApp, pDb, pos(), true, &pSolid);
    ExCreateBreps::setColor(pSolid, 0xa0, 0x00, 0x00);

    pos.nextRow();
    ExCreateBreps::addTCoedgeEllipCylinder(pHostApp, pDb, pos(), &pSolid);
    ExCreateBreps::setColor(pSolid, 0x00, 0xc0, 0x50);

    ExCreateBreps::addTCoedgeFourthSphere(pHostApp, pDb, pos(), &pSolid);
    ExCreateBreps::setColor(pSolid, 0x00, 0xc0, 0x50);

  }
  catch (const OdError& e)
  {
    OdString msg = e.description();
    pIO->putString(
      OdString().format(OD_T("Failed with message: %s"), msg.c_str())
    );
  }
  catch (const wchar_t* s)
  {
    pIO->putString(
      OdString().format(OD_T("Failed with message: %ls"), s)
    );
  }
  catch (const char* s)
  {
    pIO->putString(
      OdString().format(OD_T("Failed with message: %hs"), s)
    );
  }
  catch (...)
  {
    pIO->putString(OD_T("Failed"));
  }
}
// CreateBreps

void _InsertClone_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();
  
  // Read db with clipboard contents
  OdDbDatabasePtr tempDb = pDb->appServices()->readFile(pIO->getFilePath("Dynamic block", OdEd::kGfpForOpen, L"", L"dwg", L"", L"*.dwg|*.dwg"));
  // Insert the copied db as a whole anonymous block
  OdDbObjectId tempBlockId = pDb->insert(L"*A", tempDb);
  // Build a set of objects to clone
  OdDbBlockTableRecordPtr pBlockDef = tempBlockId.safeOpenObject();
  OdDbObjectIteratorPtr pIterator = pBlockDef->newIterator();
  OdDbObjectIdArray blockIds;
  for (pIterator->start(); !pIterator->done(); pIterator->step())
    blockIds.append(pIterator->objectId());

  OdDbIdMappingPtr idMap = OdDbIdMapping::createObject();
  pDb->deepCloneObjects(blockIds, pDb->getModelSpaceId(), *idMap);
}

static void dumpProperty(const OdRxObject* pObj, OdRxProperty* pProp,  OdEdUserIO* pIO)
{
  OdString s = pProp->name() + L" = ";
  OdRxValue v, converted;
  if (eOk == pProp->getValue(pObj, v))
  {
    if (v.type() == OdRxValueType::Desc<OdString>::value())
      s += *rxvalue_cast<OdString>(&v);
    else if (v.type().toValueType(OdRxValueType::Desc<OdString>::value(), v, converted))
      s += *rxvalue_cast<OdString>(&converted);
    else if (v.type().isEnum())
      s += v.getEnumTag()->name();
    else if (OdRxDisplayAsAttribute* dsa = (OdRxDisplayAsAttribute*)pProp->attributes().get(OdRxDisplayAsAttribute::desc()))
      s += dsa->getDisplayValue(v);
    else if (v.type() == OdRxValueType::Desc<int>::value())
      s += v.toString();
    else if(v.type().toValueType(OdRxValueType::Desc<int>::value(), v, converted))
      s += converted.toString();
    else // fallback
      s += v.toString();
  }
  pIO->putString(s);
}

static void dumpCollectionProperty(const OdRxObject* pObj, OdRxCollectionProperty* pProp, OdEdUserIO* pIO)
{

}

void _Props_func(OdEdCommandContext* pCmdCtx)
{
  odrxDynamicLinker()->loadModule(DbPropertiesModuleName);
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();
  OdDbSelectionSetPtr pSSet = pIO->select(OdString::kEmpty, OdEd::kSelAllowInactSpaces | OdEd::kSelAllowObjects | OdEd::kSelAllowPSVP);
  for (OdDbSelectionSetIteratorPtr pIter = pSSet->newIterator(); !pIter->done(); pIter->next())
  {
    OdDbObjectPtr pObj = pIter->objectId().openObject();
    if (pObj.isNull())
      continue;
    pIO->putString(OdString().format(L"Dumping properties for %ls <%llX>", pObj->isA()->name().c_str(), (OdUInt64)pObj->objectId().getHandle()));
    pIO->putString(L"");
    for (OdRxMemberIteratorPtr pMi = OdRxMemberQueryEngine::theEngine()->newMemberIterator(pObj); !pMi->done(); pMi->next())
    {
      OdRxPropertyPtr pProp = OdRxProperty::cast(pMi->current());
      if (!pProp.isNull())
        dumpProperty(pObj, pProp, pIO);
      else
      {
        OdRxCollectionPropertyPtr pColl = OdRxCollectionProperty::cast(pMi->current());
        if (!pColl.isNull())
          dumpCollectionProperty(pObj, pColl, pIO);
      }
    }
  }
}


void _getRegionArea_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();
  ::odrxDynamicLinker()->loadApp(OdCurveFunctionsModuleName);
  ::odrxDynamicLinker()->loadApp(OdSpaModelerModuleName, false);
  OdDbSelectionSetIteratorPtr pIter = pIO->select()->newIterator();
  for (; !pIter->done(); pIter->next())
  {
    double area;
    OdDbEntityPtr ent = pIter->objectId().openObject();
    if (ent->isKindOf(OdDbHatch::desc()))
    {
      OdDbHatchPtr h = ent;
      OdDbRegion* pReg = h->getRegionArea();
      double d = 0.0;
      if (pReg != 0)
      {
        if (eOk == pReg->getPerimeter(area))
        {
          //ODA_ASSERT(OdEqual(d, area));
          pIO->putString(OdString().format(OD_T("getPerimeter: %10.9f"), area));
        }
        else
          pIO->putString(L"Region has non-closed or self-intersecting boundaries");
      }
    }
  }
}

class ChildsFiler : public OdStaticRxObject<OdIdFiler>
{
  OdDbDatabase* m_pDb;
  OdDbObjectIdArray m_childs;
public:
  ChildsFiler(const OdDbObjectId& id) : m_pDb(id.database())
  {
    OdDbObjectPtr pObj = id.openObject();
    pObj->dwgOut(this);
    // Recursion. On each step array grows
    for (unsigned i = 0; i < m_childs.size(); ++i)
    {
      pObj = m_childs[i].openObject(OdDb::kForRead, true); // Open erased(
      pObj->dwgOut(this);
    }
  }

  OdDbObjectIdArray getChilds() { return m_childs; };

  virtual OdDbDatabase* database() const
  {
    return m_pDb;
  }
  virtual void wrHardOwnershipId(const OdDbObjectId& value)
  {
    m_childs.append(value);
  }
  virtual void wrSoftOwnershipId(const OdDbObjectId& value)
  {
    m_childs.append(value);
  }
  virtual void wrSoftPointerId(const OdDbObjectId&) {}
  virtual void wrHardPointerId(const OdDbObjectId& value) {}
};

void _ListChilds_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();
  OdDbSelectionSetIteratorPtr pIter = pIO->select(L"Select object", OdEd::kSelAllowInactSpaces | OdEd::kSelAllowObjects)->newIterator();
  for (; !pIter->done(); pIter->next())
  {
    ChildsFiler filer(pIter->objectId());
    OdDbObjectIdArray childs = filer.getChilds();
    for (unsigned i = 0; i < childs.size(); ++i)
    {
      OdString s;
      s.format(OD_T("%ls\n"), odDbGetObjectName(childs[i].openObject()).c_str());
      pDbCmdCtx->userIO()->putString(s);
    }
  }
}
