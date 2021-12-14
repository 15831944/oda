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

#include <OdaCommon.h>
#include "OverrulingSample.h"
#include "Ed/EdCommandContext.h"
#include "Ed/EdUserIO.h"
#include "RxDynamicModule.h"
#include "DbCommandContext.h"
#include "DbLine.h"
#include "Db3dSolid.h"

inline bool isSampleOverruleApplicable(const OdRxObject* pOverruledSubject)
{
  // we don't perform any actual checking here
  // first ASSERT should be always true because we only attach our overrules to OdDbLine class
  ODA_ASSERT(pOverruledSubject->isKindOf(OdDbLine::desc())); 
  // second condition will be checked in the overrule implementation, so we avoid redundant XData unwinding
  ODA_ASSERT(!((OdDbLine*)pOverruledSubject)->xData(OVERRULE_SAMPLE_APPNAME).isNull());
  return true;
}

bool SampleDrawableOverrule::isApplicable(const OdRxObject* pOverruledSubject) const
{
  // the implementation is common
  return isSampleOverruleApplicable(pOverruledSubject);
}

OdUInt32 SampleDrawableOverrule::setAttributes(const OdGiDrawable* pSubject, OdGiDrawableTraits * traits)
{
  // we might change line rendering attributes here
  return OdGiDrawableOverrule::setAttributes(pSubject, traits);
}

// We overrule line rendering, to draw pipe with a given radius
OdDb3dSolidPtr createPipe(const OdDbLine* pLine)
{
  // check if we overruled this line
  OdResBufPtr xdata = pLine->xData(OVERRULE_SAMPLE_APPNAME);
  if (xdata.isNull() || xdata->next().isNull())
    return OdDb3dSolidPtr();
  // extract pipe radius
  double r = xdata->next()->getDouble();
  if (OdZero(r))
    return OdDb3dSolidPtr();
  // cache line start...
  OdGePoint3d p1 = pLine->startPoint();
  // ... and end points
  OdGePoint3d p2 = pLine->endPoint();
  // line length
  double len = p1.distanceTo(p2);
  // zero length lines are not overruled
  if (OdZero(len))
    return OdDb3dSolidPtr();
  // new empty 3d solid object
  OdDb3dSolidPtr solid = OdDb3dSolid::createObject();
  // crash in getGeomExtents\worldDraw is possible without the next line.
  solid->setDatabaseDefaults(pLine->database());

  // create cylinder (height = line length, constant radius)
  solid->createFrustum(len, r, r, r);
  // cylinder is created centered in WCS origin => place it on the line: set orientation first...
  solid->transformBy(OdGeMatrix3d::planeToWorld(p2 - p1));
  // ...then move to the line center
  solid->transformBy(OdGeMatrix3d::translation((p1.asVector() + p2.asVector())/2));
  // set yellow color
  //solid->setColorIndex(2);
  return solid;
}

bool SampleDrawableOverrule::worldDraw (const OdGiDrawable* pSubject, OdGiWorldDraw * wd)
{
  // we use this overruling only on line class => blind cast is ok
  const OdDbLine* pLine = static_cast<const OdDbLine*>(pSubject);
  // create the pipe
  OdDb3dSolidPtr solid = createPipe(pLine);
  if (solid.isNull())
    return OdGiDrawableOverrule::worldDraw (pSubject, wd);
  solid->worldDraw(wd);
  // don't call viewportDraw 
  return true;
}

bool SampleGeometryOverrule::isApplicable(const OdRxObject* pOverruledSubject) const
{
  // the implementation is common
  return isSampleOverruleApplicable(pOverruledSubject);
}

OdResult SampleGeometryOverrule::getGeomExtents(const OdDbEntity* pSubject, OdGeExtents3d& extents)
{
  // we use this overruling only on line class => blind cast is ok
  const OdDbLine* pLine = static_cast<const OdDbLine*>(pSubject);
  // that implementation is wildly inefficient, but it is short and clear :)
  OdDb3dSolidPtr solid = createPipe(pLine);
  if (solid.isNull())
    return OdDbGeometryOverrule::getGeomExtents(pSubject, extents);
  return solid->getGeomExtents(extents);
}

bool SampleTransformOverrule::isApplicable(const OdRxObject* pOverruledSubject) const
{
  // the implementation is common
  return isSampleOverruleApplicable(pOverruledSubject);
}

OdResult SampleTransformOverrule::explode(const OdDbEntity* pSubject, OdRxObjectPtrArray& entitySet)
{
  // we use this overruling only on line class => blind cast is ok
  const OdDbLine* pLine = static_cast<const OdDbLine*>(pSubject);
  // create the pipe
  OdDb3dSolidPtr solid = createPipe(pLine);
  if (solid.isNull())
    return OdDbTransformOverrule::explode(pSubject, entitySet);
  // append the pipe as an explosion result
  entitySet.append(solid.get());
  return eOk;
}

bool SampleDbObjectOverrule::isApplicable(const OdRxObject* pOverruledSubject) const
{
  // the implementation is common
  return isSampleOverruleApplicable(pOverruledSubject);
}
 
OdDbObjectPtr SampleDbObjectOverrule::deepClone(const OdDbObject* pSubject, OdDbIdMapping& idMap, OdDbObject* owner, bool bPrimary)
{
  // we might remove pipe radius from the cloned object here
  return OdDbObjectOverrule::deepClone(pSubject, idMap, owner, bPrimary);
}

OdDbObjectPtr SampleDbObjectOverrule::wblockClone(const OdDbObject* pSubject, OdDbIdMapping& idMap, OdDbObject* owner, bool bPrimary)
{
  // we might remove pipe radius from the cloned object here
  return OdDbObjectOverrule::wblockClone(pSubject, idMap, owner, bPrimary);
}

const OdString SampleOverruleCommand::groupName() const
{
  return L"ODA Example Commands";
}

const OdString SampleOverruleCommand::globalName() const
{
  // less typing
  return L"ovr";
}

void SampleOverruleCommand::execute(OdEdCommandContext* pCmdCtx)
{
  try
  {
    // our overrule makes sense only for DWG database
    OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
    // register regapp, for XData operations
    pDbCmdCtx->database()->newRegApp(OVERRULE_SAMPLE_APPNAME);
    // ask user for 'A' or 'R', i.e. Add or Remove overule 
    OdDbSelectionSetPtr ss = pDbCmdCtx->dbUserIO()->select(L"Select a line");
    for (OdDbSelectionSetIteratorPtr si = ss->newIterator(); !si->done(); si->next())
    {
      OdDbEntityPtr ent = si->objectId().safeOpenObject(OdDb::kForWrite);
      if (!ent->isKindOf(OdDbLine::desc()))
      {
        pCmdCtx->userIO()->putString(L"Non line object skipped");
        continue;
      }
      OdDbLinePtr line(ent);
      if (0 == pCmdCtx->userIO()->getKeyword(L"Add/Remove overrule from selected line", L"A R", 0))
      {
        double r = pCmdCtx->userIO()->getReal(L"Enter pipe radius: <1.0>", OdEd::kInpNonNeg, 1.0);
        OdResBufPtr xdata = OdResBuf::newRb(OdResBuf::kDxfRegAppName, OVERRULE_SAMPLE_APPNAME);
        xdata->setNext(OdResBuf::newRb(OdResBuf::kDxfXdReal, r));
        line->setXData(xdata);
      }
      else
      {
        line->setXData(OdResBuf::newRb(OdResBuf::kDxfRegAppName, OVERRULE_SAMPLE_APPNAME));
      }
    }
  }
  catch (const OdError&) // just ignore the errors
  {
  }
}

void OverrulingSampleModule::initApp()
{
  OdEdCommandStackPtr pCommands = odedRegCmds();
  OdRxOverrule::setIsOverruling(true);
  pCommands->addCommand(&m_SampleOverruleCommand);
  OdRxOverrule::addOverrule(OdDbLine::desc(), &m_SampleDrawableOverrule);
  OdRxOverrule::addOverrule(OdDbLine::desc(), &m_SampleTransformOverrule);
  OdRxOverrule::addOverrule(OdDbLine::desc(), &m_SampleDbObjectOverrule);
  OdRxOverrule::addOverrule(OdDbLine::desc(), &m_SampleGeometryOverrule);
}

void OverrulingSampleModule::uninitApp()
{
  OdEdCommandStackPtr pCommands = odedRegCmds();
  pCommands->removeCmd(m_SampleOverruleCommand.groupName(), m_SampleOverruleCommand.globalName());
  OdRxOverrule::removeOverrule(OdDbLine::desc(), &m_SampleDrawableOverrule);
  OdRxOverrule::removeOverrule(OdDbLine::desc(), &m_SampleTransformOverrule);
  OdRxOverrule::removeOverrule(OdDbLine::desc(), &m_SampleDbObjectOverrule);
  OdRxOverrule::removeOverrule(OdDbLine::desc(), &m_SampleGeometryOverrule);
}

// define TX module entry point
ODRX_DEFINE_DYNAMIC_MODULE(OverrulingSampleModule);

// the stuff below is irrelevant to the sample
#if defined(_TOOLKIT_IN_DLL_) && defined(_MSC_VER)


extern "C" int APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID )
{
  switch ( dwReason )
  {
  case DLL_PROCESS_ATTACH:
    // remove this if you need per-thread initialization
    DisableThreadLibraryCalls( (HMODULE)hInstance );
    break;
  }
  return TRUE;
}
#endif //_TOOLKIT_IN_DLL_
