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
#include "DbPolyline.h"
#include "OdDbAssocPolylinePersSubentIdPE.h"
#include "Ge/GeCircArc3d.h"
#include "Ge/GeLineSeg3d.h"
#include "DbAssocEdgePersSubentId.h"

ODRX_NO_CONS_DEFINE_MEMBERS_ALTNAME(OdDbAssocPolylinePersSubentIdPE, OdDbAssocCurvePersSubentIdPE, L"AcDbAssocPolylinePersSubentIdPE");

OdDbAssocPersSubentIdPtr OdDbAssocPolylinePersSubentIdPE::createNewPersSubent(OdDbEntity* pEntity,
                                                                              const class OdDbCompoundObjectId& compId,
                                                                              const OdDbSubentId& subentId)
{
  OdDbAssocEdgePersSubentIdPtr res;
  if ( !isTypeEqual(pEntity, OdDbPolyline::desc()) || !checkSubentId(pEntity, subentId, OdDb::kEdgeSubentType) )
    res.attach((OdDbAssocEdgePersSubentId*)NULL);
  else
  {
    OdDbPolylinePtr pPolyLine = OdDbPolyline::cast(pEntity);
    int ptQty = pPolyLine->numVerts();
    int maxIdentifier = 0;
    for (int i = 0; i < ptQty; i++)
    {
        if(pPolyLine->getVertexIdentifierAt(i) > maxIdentifier)
            maxIdentifier = pPolyLine->getVertexIdentifierAt(i);
    }
    
    int startVertexIdx = (int)subentId.index() - 1;
    int endVertexIdx = startVertexIdx + 1;
    if (endVertexIdx >= ptQty)
      endVertexIdx = 0;

    if ( pPolyLine->getVertexIdentifierAt(startVertexIdx) == 0 )
    {
      pPolyLine->setVertexIdentifierAt(startVertexIdx, ++maxIdentifier);
    }

    if ( pPolyLine->getVertexIdentifierAt(endVertexIdx) == 0 )
    {
      pPolyLine->setVertexIdentifierAt(endVertexIdx, ++maxIdentifier);
    }

    res = OdDbAssocEdgePersSubentId::createObject();
    res->init(pPolyLine->getVertexIdentifierAt(startVertexIdx), pPolyLine->getVertexIdentifierAt(endVertexIdx));
  }

  return res;
}

bool isEdgeStartValid(const OdDbPolyline& pline, OdGsMarker start)
{
  const OdGsMarker minStart = 0;
  const OdGsMarker maxStart = pline.numVerts() - (pline.isClosed() ? 1 : 2);
  return start >= minStart && start <= maxStart;
}

bool isEdgeEndValid(const OdDbPolyline& pline, OdGsMarker end)
{
  const OdGsMarker minEnd = pline.isClosed() ? 0 : 1;
  const OdGsMarker maxEnd = pline.numVerts() - 1;
  return end >= minEnd && end <= maxEnd;
}

OdResult OdDbAssocPolylinePersSubentIdPE::getTransientSubentIds(const OdDbEntity* pEntity, 
                                                                const OdDbAssocPersSubentId* pPerSubentId,
                                                                OdArray<OdDbSubentId>& subents) const
{
  const OdDbPolylinePtr pPolyline = OdDbPolyline::cast(pEntity);
  const OdDbAssocEdgePersSubentIdPtr pEdgePerSubentId = OdDbAssocEdgePersSubentId::cast(pPerSubentId);
  if (pPolyline.isNull() || pEdgePerSubentId.isNull())
    return eWrongObjectType;

  subents.clear();

  const OdGsMarker startId = pEdgePerSubentId->index1();
  const OdGsMarker endId = pEdgePerSubentId->index2();

  OdGsMarker startIdx = -1;
  OdGsMarker endIdx = -1;

  const unsigned numVerts = pPolyline->numVerts();
  for (unsigned i = 0; i < numVerts; ++i)
  {
    const OdInt32 vertId = pPolyline->getVertexIdentifierAt(i);
    if (vertId == startId)
      startIdx = i;
    if (vertId == endId)
      endIdx = i;

    if (startIdx >= 0 && endIdx >= 0)
      break;
  }

  if (isEdgeStartValid(*pPolyline, startIdx))
  {
    const OdGsMarker edgeMarker = startIdx + 1;
    OdDbSubentId edge(OdDb::kEdgeSubentType, edgeMarker);
    subents.append(edge);
    return eOk;
  }

  if (isEdgeEndValid(*pPolyline, endIdx))
  {
    const OdGsMarker edgeMarker = endIdx == 0 ? numVerts : endIdx;
    OdDbSubentId edge(OdDb::kEdgeSubentType, edgeMarker);
    subents.append(edge);
    return eOk;
  }

  return eInvalidInput;
}

OdResult OdDbAssocPolylinePersSubentIdPE::getAllSubentities(const OdDbEntity* pEntity,
                                                            OdDb::SubentType subentType,
                                                            OdArray<OdDbSubentId>& allSubentIds)
{
  if ( !isTypeEqual(pEntity, OdDbPolyline::desc()) )
    return eBadObjType;

  OdDbPolylinePtr pPolyline = OdDbPolyline::cast(pEntity);

  if ( subentType == OdDb::kEdgeSubentType )
  {
    for ( unsigned int i = 1; i < pPolyline->numVerts(); i++ )
      allSubentIds.append(OdDbSubentId(OdDb::kEdgeSubentType, i));

    return eOk;
  }
  else if ( subentType == OdDb::kVertexSubentType )
  {
    for ( unsigned int i = 1; i <= pPolyline->numVerts(); i++ )
    {
      allSubentIds.append(OdDbSubentId(OdDb::kVertexSubentType, i));
      if (pPolyline->segType(i - 1) == OdDbPolyline::kArc)
        allSubentIds.append(OdDbSubentId(OdDb::kVertexSubentType, kCenterPLinePrefix + i));
      if (i < pPolyline->numVerts() || pPolyline->isClosed())
        allSubentIds.append(OdDbSubentId(OdDb::kVertexSubentType, kMiddlePLinePrefix + i));
    }

    return eOk;
  }
  else
    return eBadObjType;
}

OdResult OdDbAssocPolylinePersSubentIdPE::getEdgeVertexSubentities(const OdDbEntity*      pEntity,
                                                                   const OdDbSubentId&    edgeSubentId,
                                                                   OdDbSubentId&          startVertexSubentId,
                                                                   OdDbSubentId&          endVertexSubentId,
                                                                   OdArray<OdDbSubentId>& otherVertexSubentIds)
{
  OdDbAssocCurvePersSubentIdPE::getEdgeVertexSubentities(pEntity, edgeSubentId, startVertexSubentId, endVertexSubentId, otherVertexSubentIds);
  return otherVertexSubentIds.length() == 0 ? eBadObjType : eOk;
}

OdResult OdDbAssocPolylinePersSubentIdPE::getEdgeSubentityGeometry(const OdDbEntity*   pEntity,
                                                                   const OdDbSubentId& edgeSubentId,
                                                                   OdGeCurve3d*&       pEdgeCurve)
{
  OdDbPolylinePtr pPolyline = OdDbPolyline::cast(pEntity);
  unsigned int segIdx;
  bool bLastSegment = false;

  segIdx = (unsigned int) edgeSubentId.index() - 1;

  if (!pPolyline->isClosed() && segIdx == pPolyline->numVerts() - 1)
  {
    segIdx--;
    bLastSegment = true;
  }

  int idxEnd = pPolyline->isClosed() ? 1 : 2;
  if (segIdx > pPolyline->numVerts() - idxEnd)
    return eInvalidInput;

  OdDbPolyline::SegType segType = pPolyline->segType(segIdx);
  switch (segType)
  {
    case OdDbPolyline::kLine:
    case OdDbPolyline::kCoincident:
    {
      OdGeLineSeg3d* pLineSeg = new OdGeLineSeg3d();
      pPolyline->getLineSegAt(segIdx, *pLineSeg);

      if (bLastSegment)
      {
        OdGePoint3d point1 = pLineSeg->startPoint();
        OdGePoint3d point2 = pLineSeg->endPoint();

        pLineSeg->set(point2, point1);
      }

      pEdgeCurve = pLineSeg;
      break;
    }
    case OdDbPolyline::kArc:
    {
      OdGeCircArc3d* pArcSeg = new OdGeCircArc3d();
      pPolyline->getArcSegAt(segIdx, *pArcSeg);
      pEdgeCurve = pArcSeg;
      break;
    }
    default:
      return eInvalidInput;
  }
  return eOk;
}

OdResult OdDbAssocPolylinePersSubentIdPE::setEdgeSubentityGeometry(OdDbEntity*         pEntity,
                                                                   const OdDbSubentId& edgeSubentId,
                                                                   const OdGeCurve3d*  pNewEdgeCurve)
{
  if ( checkSubentId(pEntity, edgeSubentId, OdDb::kEdgeSubentType) )
  {
    // TODO
    return eNotImplementedYet;
  }
  else
    return eBadObjType;
}

OdResult OdDbAssocPolylinePersSubentIdPE::getVertexSubentityGeometry(const OdDbEntity*   pEntity,
                                                                     const OdDbSubentId& vertexSubentId,
                                                                     OdGePoint3d&        vertexPosition)
{
  if ( checkSubentId(pEntity, vertexSubentId, OdDb::kVertexSubentType) )
  {
    OdDbPolylinePtr pPolyline = OdDbPolyline::cast(pEntity);
    unsigned int idx = (unsigned int) vertexSubentId.index() - 1;
    if (idx >= kMiddlePLinePrefix)
    {
      idx -= kMiddlePLinePrefix;
      OdDbSubentId edgeSubentId(OdDb::kEdgeSubentType, idx + 1);
      OdGeCurve3d *pCurve;
      OdResult res = getEdgeSubentityGeometry(pEntity, edgeSubentId, pCurve);
      if (res != eOk)
        return res;
      OdGeInterval interval;
      pCurve->getInterval(interval);
      double param = 0.5*(interval.lowerBound() + interval.upperBound());
      vertexPosition = pCurve->evalPoint(param);
      delete pCurve;
      return eOk;
    }
    if (idx >= kCenterPLinePrefix)
    {
      idx -= kCenterPLinePrefix;
      OdGeCircArc3d arc3d;
      pPolyline->getArcSegAt(idx, arc3d);
      vertexPosition = arc3d.center();
      return eOk;
    }
    pPolyline->getPointAt(idx, vertexPosition);
    return eOk;
  }

  return isTypeEqual(pEntity, OdDbPolyline::desc()) ? eInvalidIndex : eBadObjType;
}

OdResult OdDbAssocPolylinePersSubentIdPE::setVertexSubentityGeometry(OdDbEntity*         pEntity,
                                                                     const OdDbSubentId& vertexSubentId,
                                                                     const OdGePoint3d&  newVertexPosition)
{
  if ( checkSubentId(pEntity, vertexSubentId, OdDb::kVertexSubentType) )
  {
    // TODO
    return eNotImplementedYet;
  }
  else
    return eBadObjType;
}


OdResult OdDbAssocPolylinePersSubentIdPE::setSubentityGeometry(OdDbEntity*         pEntity,
                                                               const OdDbSubentId& subentId,
                                                               const OdDbEntity*   pNewSubentityGeometry)
{
  if ( !isTypeEqual(pEntity, OdDbPolyline::desc()) || !isTypeEqual(pNewSubentityGeometry, OdDbPolyline::desc()) )
    return eBadObjType;

  if ( checkSubentId(pEntity, subentId, OdDb::kEdgeSubentType) )
  {
    OdGeCurve3d* pCurve = NULL;
    if ( getEdgeSubentityGeometry(pNewSubentityGeometry, subentId, pCurve) != eOk )
    {
      delete pCurve;
      return eBadObjType;
    }
    else
    {
      OdResult res = setEdgeSubentityGeometry(pEntity, subentId, pCurve);
      delete pCurve;
      return res;
    }
  }
  else if ( checkSubentId(pEntity, subentId, OdDb::kVertexSubentType) )
  {
    OdGePoint3d newPt;
    if ( getVertexSubentityGeometry(pNewSubentityGeometry, subentId, newPt) != eOk )
      return eBadObjType;

    setVertexSubentityGeometry(pEntity, subentId, newPt);
    return eOk;
  }
  else
    return eBadObjType;
}

void OdDbAssocPolylinePersSubentIdPE::fillStartVertexSubentId(const OdDbEntity*      pEntity,
                                                              const OdDbSubentId&    edgeSubentId,
                                                              OdDbSubentId& startVertexSubentId)
{
  OdDbAssocCurvePersSubentIdPE::fillStartVertexSubentId(pEntity, edgeSubentId, startVertexSubentId);
  startVertexSubentId.setIndex(kStart);
}

void OdDbAssocPolylinePersSubentIdPE::fillEndVertexSubentId(const OdDbEntity*      pEntity,
                                                            const OdDbSubentId&    edgeSubentId,
                                                            OdDbSubentId& endVertexSubentId)
{
  OdDbAssocCurvePersSubentIdPE::fillEndVertexSubentId(pEntity, edgeSubentId, endVertexSubentId);
  endVertexSubentId.setIndex(kEnd);
}

void OdDbAssocPolylinePersSubentIdPE::getOtherVertexSubentIds(const OdDbEntity*      pEntity,
                                                              const OdDbSubentId&    edgeSubentId,
                                                              OdArray<OdDbSubentId>& otherVertexSubentIds)
{
  OdDbAssocCurvePersSubentIdPE::getOtherVertexSubentIds(pEntity, edgeSubentId, otherVertexSubentIds);
  if ( checkSubentId(pEntity, edgeSubentId, OdDb::kEdgeSubentType ) )
  {
    // TODO
    OdDbPolylinePtr pPolyline = OdDbPolyline::cast(pEntity);
    for ( unsigned int i = 0; i < pPolyline->numVerts(); i++ )
      otherVertexSubentIds.append(OdDbSubentId(OdDb::kVertexSubentType, i));
  }
}

bool OdDbAssocPolylinePersSubentIdPE::checkSubentId(const OdDbEntity* pEntity, const OdDbSubentId& subentId, const OdDb::SubentType type) const
{
  if ( !isTypeEqual(pEntity, OdDbPolyline::desc()) )
    return false;
  OdDbPolylinePtr pPolyline = OdDbPolyline::cast(pEntity);
  unsigned int idx = (unsigned int) subentId.index();
  unsigned int numVerts = pPolyline->numVerts();
  if ( type == OdDb::kEdgeSubentType )
  {
    unsigned int maxSeg = pPolyline->isClosed() ? numVerts : numVerts - 1;
    return (idx >= 1 && idx <= maxSeg);
  }
  else if ( type == OdDb::kVertexSubentType )
  {
    if (idx > kMiddlePLinePrefix)
      idx -= kMiddlePLinePrefix;
    if (idx > kCenterPLinePrefix)
      idx -= kCenterPLinePrefix;
    bool result = (idx >= 1 && idx <= numVerts);
    if (result &&
        unsigned(subentId.index()) > kCenterPLinePrefix &&
        unsigned(subentId.index()) <= kMiddlePLinePrefix)
    {
      result &= pPolyline->segType(idx - 1) == OdDbPolyline::kArc;
    }
    return result;
  }
  else
    return false;
}
