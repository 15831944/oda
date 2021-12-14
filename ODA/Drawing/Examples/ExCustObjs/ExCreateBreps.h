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

#ifndef _EX_CREATEBREPS_COMMAND_H_
#define _EX_CREATEBREPS_COMMAND_H_

class OdDbHostAppServices;

#include "DbBody.h"
#include "DbDatabase.h"
#include "Db3dSolid.h"

namespace ExCreateBreps
{
  void addPlane(OdDbHostAppServices* pHostApp, OdDbDatabasePtr pDb, bool useReverse, const OdGeVector3d& offset, OdDbBodyPtr* pBodyStore = NULL);
  void addRegionWithHole(OdDbHostAppServices* pHostApp, OdDbDatabasePtr pDb, const OdGeVector3d& offset, OdDbRegionPtr* pRegionStore = NULL);
  void addCylinder(OdDbHostAppServices* pHostApp, OdDbDatabasePtr pDb, bool useReverse, const OdGeVector3d& offset, OdDbBodyPtr* pBodyStore = NULL);
  void addNurbSurface(OdDbHostAppServices* pHostApp, OdDbDatabasePtr pDb, bool useReverse, const OdGeVector3d& offset, OdDbBodyPtr* pBodyStore = NULL);
  void addCone(OdDbHostAppServices* pHostApp, OdDbDatabasePtr pDb, bool cutBeforeApex, bool useReverse, const OdGeVector3d& offset, OdDbBodyPtr* pBodyStore = NULL);

  enum TorusType
  {
    ttDoughnut,
    ttApple,
    ttVortex,
    ttLemon,
  };
  void addTorus(OdDbHostAppServices* pHostApp, OdDbDatabasePtr pDb, TorusType torusType, const OdGeVector3d& offset, OdDb3dSolidPtr* pSolidStore = NULL);
  void addTorusQuarter(OdDbHostAppServices* pHostApp, OdDbDatabasePtr pDb, const OdGeVector3d& offset, OdDb3dSolidPtr* pSolidStore = NULL);
  void addSphere(OdDbHostAppServices* pHostApp, OdDbDatabasePtr pDb, const OdGeVector3d& offset, OdDb3dSolidPtr* pSolidStore = NULL);
  void addHemiSphere(OdDbHostAppServices* pHostApp, OdDbDatabasePtr pDb, const OdGeVector3d& offset, OdDb3dSolidPtr* pSolidStore = NULL);
  void addPyramid(OdDbHostAppServices* pHostApp, OdDbDatabasePtr pDb, const OdGeVector3d& offset, OdDb3dSolidPtr* pSolidStore = NULL);
  void addBox(OdDbHostAppServices* pHostApp, OdDbDatabasePtr pDb, const OdGeVector3d& offset, OdDb3dSolidPtr* pSolidStore = NULL);
  void addSeamedCylinder(OdDbHostAppServices* pHostApp, OdDbDatabasePtr pDb, const OdGeVector3d& offset, OdDb3dSolidPtr* pBodyStore = NULL);
  void addRotSurface1(OdDbHostAppServices* pHostApp, OdDbDatabasePtr pDb, const OdGeVector3d& offset, bool bUseSpunSurf = false, OdDb3dSolidPtr* pSolidStore = NULL);
  void addRotSurface2(OdDbHostAppServices* pHostApp, OdDbDatabasePtr pDb, const OdGeVector3d& offset, bool bUseSpunSurf = false, OdDb3dSolidPtr* pSolidStore = NULL);
  void addRotSurface3(OdDbHostAppServices* pHostApp, OdDbDatabasePtr pDb, const OdGeVector3d& offset, bool bUseSpunSurf = false, OdDb3dSolidPtr* pSolidStore = NULL);
  void addRotSurface4(OdDbHostAppServices* pHostApp, OdDbDatabasePtr pDb, const OdGeVector3d& offset, bool bUseSpunSurf = false, OdDb3dSolidPtr* pSolidStore = NULL);
  void addRotSurface5(OdDbHostAppServices* pHostApp, OdDbDatabasePtr pDb, const OdGeVector3d& offset, bool bUseSpunSurf = false, OdDb3dSolidPtr* pSolidStore = NULL);
  void addEllipCylinder(OdDbHostAppServices* pHostApp, OdDbDatabasePtr pDb, const OdGeVector3d& offset, OdDb3dSolidPtr* pSolidStore = NULL);
  void addEllipCone(OdDbHostAppServices* pHostApp, OdDbDatabasePtr pDb, const OdGeVector3d& offset, OdDb3dSolidPtr* pSolidStore = NULL);

  void addTCoedgeEllipCylinder(OdDbHostAppServices* pHostApp, OdDbDatabasePtr pDb, const OdGeVector3d& offset, OdDb3dSolidPtr* pSolidStore = NULL, double deltaRadius = 1e-3);
  void addTCoedgeFourthSphere(OdDbHostAppServices* pHostApp, OdDbDatabasePtr pDb, const OdGeVector3d& offset, OdDb3dSolidPtr* pSolidStore = NULL, double deltaRadius = 1e-3);
}

#endif // _EX_CREATEBREPS_COMMAND_H_
