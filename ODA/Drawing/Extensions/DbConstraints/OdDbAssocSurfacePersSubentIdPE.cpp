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
#include "DbEntity.h"
#include "DbSurface.h"
#include "DbExtrudedSurface.h"
#include "DbPlaneSurface.h"
#include "OdDbAssocSurfacePersSubentIdPE.h"
#include <ModelerGeometry.h>
#include "DbAssocEdgePersSubentId.h"
#include "ShHistory/DbAssocPersSubentManager.h"
#include "DbImpCenterLineActionBody.h"
#include "DbImpCenterMarkActionBody.h"

ODRX_NO_CONS_DEFINE_MEMBERS_ALTNAME(OdDbAssocSurfacePersSubentIdPE, OdDbAssocCurvePersSubentIdPE, L"AcDbAssoc3dSolidPersSubentIdPE");

//OdDbAssocPersSubentIdPtr OdDbAssocSurfacePersSubentIdPE::createNewPersSubent(OdDbEntity* pEntity,
//  const class OdDbCompoundObjectId& compId,
//  const OdDbSubentId& subentId)
//{
//  OdDbAssocEdgePersSubentIdPtr res;
//  //if (!isTypeEqual(pEntity, OdDbPolyline::desc()) || !checkSubentId(pEntity, subentId, OdDb::kEdgeSubentType))
//  //  res.attach((OdDbAssocEdgePersSubentId*)NULL);
//  //else
//  //{
//  //  OdDbPolylinePtr pPolyLine = OdDbPolyline::cast(pEntity);
//  //  int ptQty = pPolyLine->numVerts();
//  //  int maxIdentifier = 0;
//  //  for (int i = 0; i < ptQty; i++)
//  //  {
//  //    if (pPolyLine->getVertexIdentifierAt(i) > maxIdentifier)
//  //      maxIdentifier = pPolyLine->getVertexIdentifierAt(i);
//  //  }
//
//  //  int startVertexIdx = (int)subentId.index() - 1;
//  //  int endVertexIdx = startVertexIdx + 1;
//  //  if (endVertexIdx >= ptQty)
//  //    endVertexIdx = 0;
//
//  //  if (pPolyLine->getVertexIdentifierAt(startVertexIdx) == 0)
//  //  {
//  //    pPolyLine->setVertexIdentifierAt(startVertexIdx, ++maxIdentifier);
//  //  }
//
//  //  if (pPolyLine->getVertexIdentifierAt(endVertexIdx) == 0)
//  //  {
//  //    pPolyLine->setVertexIdentifierAt(endVertexIdx, ++maxIdentifier);
//  //  }
//
//  //  res = OdDbAssocEdgePersSubentId::createObject();
//  //  res->init(pPolyLine->getVertexIdentifierAt(startVertexIdx), pPolyLine->getVertexIdentifierAt(endVertexIdx));
//  //}
//  ODA_ASSERT(false);
//
//  return res;
//}


OdResult OdDbAssocSurfacePersSubentIdPE::getTransientSubentIds(const OdDbEntity* pEntity,
  const OdDbAssocPersSubentId* pPerSubentId,
  OdArray<OdDbSubentId>& subents) const
{
  const OdDbSurfacePtr pSurface = OdDbSurface::cast(pEntity);
  const OdDbAssocAsmBasedEntityPersSubentIdPtr pAsmPerSubentId = OdDbAssocAsmBasedEntityPersSubentId::cast(pPerSubentId);
  const OdDbAssocIndexPersSubentIdPtr pIndexSubentId = OdDbAssocIndexPersSubentId::cast(pPerSubentId);

  if (pSurface.isNull())
    return eWrongObjectType;

  OdDbSubentId currentSubentId;

  if (!pAsmPerSubentId.isNull())
  {
    currentSubentId = pAsmPerSubentId->subentId(NULL);
  }

  if (!pIndexSubentId.isNull())
  {
    currentSubentId = pIndexSubentId->subentId(NULL);
  }

  subents.clear();

  if (checkSubentId(pEntity, currentSubentId, pPerSubentId->subentType(pEntity)))
  {
    unsigned int internalIdx = currentSubentId.index();
    subents.append(OdDbSubentId(OdDb::kEdgeSubentType, internalIdx));
  }

  return (subents.isEmpty()) ? eInvalidInput : eOk;
}

OdResult OdDbAssocSurfacePersSubentIdPE::getEdgeSubentityGeometry(const OdDbEntity*   pEntity,
  const OdDbSubentId& edgeSubentId,
  OdGeCurve3d*& pEdgeCurve)
{
  OdResult res = eInvalidFaceVertexIndex;
  const OdDbDatabase* pDb = pEntity->database();
  OdDbObjectId objId = OdDbAssocPersSubentManager::getInstanceFromDatabase(pDb);
  OdDbAssocPersSubentManagerPtr pAssocPSManager = objId.openObject(/*OdDb::kForWrite*/);

  if (checkSubentId(pEntity, edgeSubentId, OdDb::kEdgeSubentType) && !pAssocPSManager.isNull())// && isTypeEqual(pEntity, OdDbCircle::desc()))
  {
    OdUInt32 idx = (unsigned int)edgeSubentId.index();
    OdArray<OdUInt32> indexesOfEdgeCalculation = pAssocPSManager->infoAcisDataIndexes(idx);
    
    if (indexesOfEdgeCalculation.length() > 0)
    {
      OdDbSurfacePtr pSurf(pEntity); // more types here
      OdModelerGeometryPtr pModeler = getModelerGeometry(pSurf);

      if (pModeler.isNull())
        return eGeneralModelingFailure;

      pSurf->assertReadEnabled();

      res = pModeler->getEdgeSubentityGeometry(indexesOfEdgeCalculation, pEdgeCurve);
    }
  }

  return res;
}

OdResult OdDbAssocSurfacePersSubentIdPE::addSubentitySpecialSettings(OdDbEntityPtr pEntity, const OdDbSubentId subentId, OdUInt32 idx, OdDbAssocActionBodyPtr pActionBody)
{
  const OdDbDatabase* pDb = pEntity->database();
  OdDbObjectId objId = OdDbAssocPersSubentManager::getInstanceFromDatabase(pDb);
  OdDbAssocPersSubentManagerPtr pAssocPSManager = objId.openObject(OdDb::kForWrite);

  OdArray<OdUInt32> idsArray;

  //OdDbSurfacePtr pSurf(pEntity); // more types here
  OdDbSurfacePtr pSurf = OdDbSurface::cast(pEntity);

  if (pSurf.isNull())
    return eGeneralModelingFailure;

  OdModelerGeometryPtr pModeler = getModelerGeometry(pSurf);

  if (pModeler.isNull())
    return eGeneralModelingFailure;

  //pSurf->assertWriteEnabled();//TODO Is it neccessary here? This case call out() and delete m_pAcisData
  AcisDataType addedDT = kUndefinedDataType;
  AcisDataTypeArrFirst addedDTFirst = kUndefinedDataTypeArrFirst;
  OdDbCenterLineActionBodyPtr pCenterLineActionBody = OdDbCenterLineActionBody::cast(pActionBody);
  OdDbCenterMarkActionBodyPtr pCenterMarkActionBody = OdDbCenterMarkActionBody::cast(pActionBody);

  if (!pCenterLineActionBody.isNull())
  {
    //if (pSurf->isKindOf(OdDbExtrudedSurface::desc())) // TODO Default value
    {
      addedDT = kExtrudeSurface_TopOrBottom_CLine;
    }

    if (pEntity->isKindOf(OdDbPlaneSurface::desc()))
    {
      addedDT = kPlaneSurface_CLine;
      addedDTFirst = kArrSolidExtrude_CLCM;
    }
    // TODO kExtrudeSurface_Side_CLine
  }
    
  if (!pCenterMarkActionBody.isNull())
  {
    addedDT = kSurfaceExtrude_CMark;
    addedDTFirst = kArrFirstSurfaceExtrude_CMark;
  }

  OdResult ret = pModeler->addSubentitySpecialSettings(addedDT, addedDTFirst, subentId, idsArray, idx);

  if (eOk != ret && eAlreadyInDb != ret)
    return ret;

  if ((eOk == ret || eAlreadyInDb == ret) && kUndefinedDataType != addedDT)
    pAssocPSManager->addSubentRWData(addedDT, addedDTFirst, idx, idsArray);

  //if (kUndefinedDataType == addedDT)
  //  ret = eIllegalEntityType;

  return ret;
}

bool OdDbAssocSurfacePersSubentIdPE::checkSubentId(const OdDbEntity* pEntity, const OdDbSubentId& subentId, const OdDb::SubentType type) const
{
  const OdDbSurfacePtr pSurface = OdDbSurface::cast(pEntity);

  if (pSurface.isNull() || (type != OdDb::kEdgeSubentType && type != OdDb::kVertexSubentType))
    return false;

  unsigned int idx = (unsigned int)subentId.index();

  return idx > 1;
}

