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
#include "ExDgnOverruling.h"
#include "Ed/EdCommandContext.h"
#include "Ed/EdUserIO.h"
#include "RxDynamicModule.h"
#include "DgCommandContext.h"
#include "DgLine.h"
#include "DgCone.h"
#include "DgXAttribute.h"

inline bool isSampleOverruleApplicable(const OdRxObject* pOverruledSubject)
{
  // we don't perform any actual checking here
  // first ASSERT should be always true because we only attach our overrules to OdDgLine3d class
  ODA_ASSERT(pOverruledSubject->isKindOf(OdDgLine3d::desc())); 
  // second condition will be checked in the overrule implementation, so we avoid redundant XAttribute unwinding
  OdRxObjectPtrArray arrXRefs;
  ((OdDgLine3d*)pOverruledSubject)->getXAttributes(OdDgAnnotationCellXAttribute::kType, arrXRefs);
  ODA_ASSERT(arrXRefs.size() == 0);
  return true;
}

bool ExDgnSampleDrawableOverrule::isApplicable(const OdRxObject* pOverruledSubject) const
{
  // the implementation is common
  return isSampleOverruleApplicable(pOverruledSubject);
}

OdUInt32 ExDgnSampleDrawableOverrule::setAttributes(const OdGiDrawable* pSubject, OdGiDrawableTraits * traits)
{
  // we might change line rendering attributes here
  return OdGiDrawableOverrule::setAttributes(pSubject, traits);
}

// We overrule line rendering, to draw pipe with a given radius
OdDgConePtr createCone(const OdDgLine3d* pLine)
{
    // Annotation Cell XAttribute is used just to illustarte 'overruling'.
    //  It is suitable to store required data to draw a pipe instead a line in case of 'overruling'.
    OdRxObjectPtrArray arrXRefs;
    pLine->getXAttributes(OdDgAnnotationCellXAttribute::kType, arrXRefs);
    if( arrXRefs.size() > 0 )
    {
      if( arrXRefs[0]->isKindOf(OdDgAnnotationCellXAttribute::desc()) )
      {
        OdDgAnnotationCellXAttributePtr pAnnotationXAttr = arrXRefs[0];
        double r = pAnnotationXAttr->getAnnotationScale();

        OdDgConePtr pCone = OdDgCone::createObject();
        // set pipe
        pCone->setCenter1( pLine->getStartPoint() );
        pCone->setCenter2( pLine->getEndPoint() );
        pCone->setRadius1( r );
        pCone->setRadius2( r );
        pCone->setColorIndex( pLine->getColorIndex() + 1 );

        return pCone;
      }
    }

    return OdDgConePtr();
}

bool ExDgnSampleDrawableOverrule::worldDraw (const OdGiDrawable* pSubject, OdGiWorldDraw * wd)
{
  // we use this overruling only on line class => blind cast is ok
  const OdDgLine3d* pLine = static_cast<const OdDgLine3d*>(pSubject);
  // create the pipe
  OdDgConePtr cone = createCone(pLine);
  if (cone.isNull())
    return OdGiDrawableOverrule::worldDraw (pSubject, wd);
  cone->worldDraw(wd);
  // don't call viewportDraw 
  return true;
}

bool ExDgnSampleGeometryOverrule::isApplicable(const OdRxObject* pOverruledSubject) const
{
  // the implementation is common
  return isSampleOverruleApplicable(pOverruledSubject);
}

OdResult ExDgnSampleGeometryOverrule::getGeomExtents(const OdDgElement* pSubject, OdGeExtents3d& extents)
{
  // we use this overruling only on line class => blind cast is ok
  const OdDgLine3d* pLine = static_cast<const OdDgLine3d*>(pSubject);
  // that implementation is wildly inefficient, but it is short and clear :)
  OdDgConePtr cone = createCone(pLine);
  if (cone.isNull())
    return OdDgGeometryOverrule::getGeomExtents(pSubject, extents);
  return cone->getGeomExtents(extents);
}

bool ExDgnSampleTransformOverrule::isApplicable(const OdRxObject* pOverruledSubject) const
{
  // the implementation is common
  return isSampleOverruleApplicable(pOverruledSubject);
}

OdResult ExDgnSampleTransformOverrule::explode(const OdDgElement* pSubject, OdRxObjectPtrArray& entitySet)
{
  // we use this overruling only on line class => blind cast is ok
  const OdDgLine3d* pLine = static_cast<const OdDgLine3d*>(pSubject);
  // create the pipe
  OdDgConePtr cone = createCone(pLine);
  if (cone.isNull())
    return OdDgTransformOverrule::explode(pSubject, entitySet);
  // append the pipe as an explosion result
  entitySet.append(cone.get());
  return eOk;
}

bool ExDgnSampleDgElementOverrule::isApplicable(const OdRxObject* pOverruledSubject) const
{
  // the implementation is common
  return isSampleOverruleApplicable(pOverruledSubject);
}
 
OdDgElementPtr ExDgnSampleDgElementOverrule::deepClone(const OdDgElement* pSubject, OdDgIdMapping& idMap)
{
  // we might remove pipe radius from the cloned object here
  return OdDgElementOverrule::deepClone(pSubject, idMap);
}

OdDgElementPtr ExDgnSampleDgElementOverrule::wblockClone(const OdDgElement* pSubject, OdDgIdMapping& idMap)
{
  // we might remove pipe radius from the cloned object here
  return OdDgElementOverrule::wblockClone(pSubject, idMap);
}

const OdString ExDgnSampleOverruleCommand::groupName() const
{
  return L"ODA Example Commands";
}

const OdString ExDgnSampleOverruleCommand::globalName() const
{
  // less typing
  return L"exdgnoverrule";
}

void ExDgnSampleOverruleCommand::execute(OdEdCommandContext* pCmdCtx)
{
  try
  {
    // our overrule makes sense only for DGN database
    OdDgCommandContextPtr pDgCmdCtx(pCmdCtx);
    OdEdUserIO* pIO = pDgCmdCtx->userIO();

    // ask user for 'A' or 'R', i.e. Add or Remove overule 
    OdDgSelectionSetPtr ss = pDgCmdCtx->dbUserIO()->select(NULL, L"Select a line");
    for (OdDgSelectionSetIteratorPtr si = ss->newIterator(); !si->done(); si->next())
    {
      OdDgElementPtr ent = si->objectId().safeOpenObject(OdDg::kForWrite);
      if (!ent->isKindOf(OdDgLine3d::desc()))
      {
        pCmdCtx->userIO()->putString(L"Non line object skipped");
        continue;
      }
      OdDgLine3dPtr line(ent);
      if (0 == pIO->getKeyword(L"Add/Remove overrule from selected line", L"A R", 0))
      {
        double r = pIO->getReal(L"Enter pipe radius: <1.0>", OdEd::kInpNonNeg, 1.0);

        // Annotation Cell XAttribute is used just to illustarte 'overruling'.
        //  It is suitable to store required data to draw a pipe instead a line in case of 'overruling'.
        OdDgAnnotationCellXAttributePtr pXAttr = OdDgAnnotationCellXAttribute::createObject();
        pXAttr->setAnnotationScale( r );
        line->addXAttribute( pXAttr->getHandlerId(), pXAttr.get());
      }
      else
      {
        OdUInt32 c = line->getColorIndex();
        line->removeXAttributes( OdDgAnnotationCellXAttribute::kType );
      }
    }
  }
  catch (const OdError&) // just ignore the errors
  {
  }
}

void ExDgnOverrulingModule::initApp()
{
  OdEdCommandStackPtr pCommands = odedRegCmds();
  OdRxOverrule::setIsOverruling(true);
  pCommands->addCommand(&m_SampleOverruleCommand);
  OdRxOverrule::addOverrule(OdDgLine3d::desc(), &m_SampleDrawableOverrule);
  OdRxOverrule::addOverrule(OdDgLine3d::desc(), &m_SampleTransformOverrule);
  OdRxOverrule::addOverrule(OdDgLine3d::desc(), &m_SampleDbObjectOverrule);
  OdRxOverrule::addOverrule(OdDgLine3d::desc(), &m_SampleGeometryOverrule);
}

void ExDgnOverrulingModule::uninitApp()
{
  OdEdCommandStackPtr pCommands = odedRegCmds();
  pCommands->removeCmd(m_SampleOverruleCommand.groupName(), m_SampleOverruleCommand.globalName());
  OdRxOverrule::removeOverrule(OdDgLine3d::desc(), &m_SampleDrawableOverrule);
  OdRxOverrule::removeOverrule(OdDgLine3d::desc(), &m_SampleTransformOverrule);
  OdRxOverrule::removeOverrule(OdDgLine3d::desc(), &m_SampleDbObjectOverrule);
  OdRxOverrule::removeOverrule(OdDgLine3d::desc(), &m_SampleGeometryOverrule);
}

// define TX module entry point
ODRX_DEFINE_DYNAMIC_MODULE(ExDgnOverrulingModule);

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
