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
// ObjectsCommands.cpp
//

#include "StdAfx.h"
#include "Db3dSolid.h"

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
