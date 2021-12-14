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
#include "CommonTests.h"

#include "ResBuf.h"
#include "DbEntity.h"
#include "DbDatabase.h"
#include "Ge/GeExtents3d.h"
#include "Ge/GeMatrix3d.h"
#include "MemoryStream.h"
#include "DbHostAppServices.h"

//#include "Ed/EdUserIO.h"
//#include "Ed/EdCommandStack.h"
//#include "DbCommandContext.h"
//#include "DbSSet.h"
//#include "DbDatabase.h"
//#include "DbViewport.h"
//#include "Ge/GeScale3d.h"
//#include "Ge/GeExtents2d.h"
//#include "RxObject.h"


int compareResBufs(const OdResBuf* pR1, const OdResBuf* pR2)
{
  int nRes = 0;
  while (pR1 && pR2)
  {
    if (*pR1 != *pR2)
      return nRes;
    pR1 = pR1->next();
    pR2 = pR2->next();
    ++nRes;
  }
  if (pR1 || pR2)
    return nRes;
  return -1;
}

int dxfIoTest(OdDbObject* pObj)
{
  OdResBufPtr pRb1 = oddbEntGet(pObj, L"*");
  OdResult res = oddbEntMod(pObj, pRb1);
  if (res != eOk)
    throw OdError(res);

  OdResBufPtr pRb2 = oddbEntGet(pObj, L"*");
  return compareResBufs(pRb1, pRb2);
}


int dwgIoUndoTest(OdDbObject* pObj)
{
  OdDbDatabase* pDb = pObj->database();
  if (!pDb)
    throw OdError (eNoDatabase);

  OdResBufPtr pRb1 = oddbEntGet(pObj, L"*");

  pDb->startUndoRecord();
  pObj->addPersistentReactor(pDb->getModelSpaceId()); // Change something
  OdResBufPtr pRb2 = oddbEntGet(pObj, L"*");
  if (compareResBufs(pRb1, pRb2) < 0)
    throw OdError(eInvalidInput);

  pDb->undo();

  pRb2 = oddbEntGet(pObj, L"*");
  return compareResBufs(pRb1, pRb2);
}

//////////////////////////////////////////////////////////////////
static bool extEqual(const OdGeExtents3d& ex1, const OdGeExtents3d& ex2)
{
  return ex1.contains(ex2) && ex2.contains(ex1);
}

int transformByTest(OdDbObject* pObj)
{
  OdDbDatabase* pDb = pObj->database();
  if (!pDb)
    throw OdError (eNoDatabase);

  OdDbEntity* pEnt = OdDbEntity::cast(pObj);
  if (pEnt)
  {
    OdGeExtents3d extInitial;
    OdResult res = pEnt->getGeomExtents(extInitial);
    if (res != eOk)
      throw OdError(res);

    OdGeMatrix3d mTrans;
    mTrans.setCoordSystem(OdGePoint3d(10., 20., 30.),
                          OdGeVector3d(2., 0., 0.),
                          OdGeVector3d(0., 2., 0.),
                          OdGeVector3d(0., 0., 2.));

    pDb->startUndoRecord();
    res = pEnt->transformBy(mTrans);
    if (res != eOk)
      throw OdError(res);

    OdGeExtents3d extTransformed;
    res = pEnt->getGeomExtents(extTransformed);
    if (res != eOk)
      throw OdError(res);

    OdGeExtents3d extTmp(extInitial);
    extTmp.transformBy(mTrans);

    if (!extEqual(extTransformed,  extTmp))
      throw OdError(eInvalidExtents);

    pDb->undo();

    OdGeExtents3d extNew;
    res = pEnt->getGeomExtents(extNew);
    if (res != eOk)
      throw OdError(res);

    if (!extEqual(extInitial, extNew))
      throw OdError(eInvalidExtents);
  }
  return -1;
}
#if 0

///////////////////////////////////////////////////////////////////////
// Round-trip test
int roundTripTest(OdDbObject* pObj)
{
  OdDbDatabase* pDb = pObj->database();
  if (!pDb)
    throw OdError (eNoDatabase);

  OdDbHostAppServices* pHostApp = pDb->appServices();
  OdMemoryStreamPtr pStream = OdMemoryStream::createNew();
  pDb->writeFile(pStream, OdDb::kDwg, OdDb::vAC15);
  pStream->seek(0, OdDb::kSeekFromStart);
    
  OdDbDatabasePtr pDbCopy = pHostApp->readFile(pStream);
  OdDbObjectPtr pCopy = pDbCopy->getOdDbObjectId(pObj->objectId().getHandle()).safeOpenObject();

  OdResBufPtr pRb1 = oddbEntGet(pObj, L"*");
  OdResBufPtr pRb2 = oddbEntGet(pCopy, L"*");
  // Skip ent name
  pRb1 = pRb1->next();
  pRb2 = pRb2->next();

  return compareResBufs(pRb1, pRb2);
}
#endif
