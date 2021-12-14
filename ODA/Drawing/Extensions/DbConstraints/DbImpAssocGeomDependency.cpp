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
#include "DbObject.h"
#include "DbImpAssocGeomDependency.h"
#include "DbFiler.h"
#include "DbEntity.h"
#include "DbPolyline.h"
#include "DbExtrudedSurface.h"
#include "OdDbAssocPersSubentIdPE.h"
#include "DbAssoc2dConstraintGroup.h"
#include "OdConstrainedGeometryImpl.h"
#include "DbAssocPersSubentId.h"
#include "OdDbAssocPolylinePersSubentIdPE.h"
#include "DbAssocNetwork.h"
#include "DbImpSmartCenterActionBody.h"
#include "ShHistory/DbAssocPersSubentManager.h"

OdDbImpAssocGeomDependency::OdDbImpAssocGeomDependency() : OdDbImpAssocDependency()
  ,m_bCachingGeometry(false)
  ,m_bPolylineGeometry(true)
{
  m_pPersSubent = NULL;
  m_pSubentCurve = NULL;
}

OdDbImpAssocGeomDependency::~OdDbImpAssocGeomDependency()
{
  m_pPersSubent.release();
  delete m_pSubentCurve;
}

OdResult OdDbImpAssocGeomDependency::dwgInFields(OdDbDwgFiler* pFiler)
{
  OdResult es = OdDbImpAssocDependency::dwgInFields(pFiler);
  if (es != eOk)
    return es;

  if (pFiler->rdInt16() != 0) // Version?
  {
    ODA_FAIL();
    return eMakeMeProxy;
  }
  if (pFiler->rdBool() != true) // Object to follow?
  {
    ODA_FAIL();
    return eMakeMeProxy;
  }
  return OdDbAssocPersSubentId::createObjectAndDwgInFields(pFiler, m_pPersSubent);
}


void OdDbImpAssocGeomDependency::dwgOutFields(OdDbDwgFiler* pFiler) const
{
  OdDbImpAssocDependency::dwgOutFields(pFiler);

  pFiler->wrInt16(0);   // Version?
  pFiler->wrBool(true); // Object to follow?

  if (!m_pPersSubent.isNull())
  {
    m_pPersSubent->dwgOutFields(pFiler);
  }
  else
    ODA_FAIL();
}

#define NEXT_CODE(code) \
if (pFiler->nextItem() != code) \
{                               \
  ODA_FAIL();                   \
  return eBadDxfSequence;       \
}

OdResult OdDbImpAssocGeomDependency::dxfInFields(OdDbDxfFiler* pFiler)
{
  OdResult es = OdDbImpAssocDependency::dxfInFields(pFiler);
  if (es != eOk)
    return es;

  // Check that we are at the correct subclass data
  if( !pFiler->atSubclassData( OdDbAssocGeomDependency::desc()->name() ))
  {
    return eBadDxfSequence;
  }
  
  // Maybe it's version number?
  if( pFiler->nextItem() != 90 || pFiler->rdInt32() != 0)
    return eMakeMeProxy;

  NEXT_CODE(290)
  if (pFiler->rdBool() != true) // Flag indicating object to follow?
  {
    ODA_FAIL();
    return eMakeMeProxy;
  }
  return OdDbAssocPersSubentId::createObjectAndDxfInFields(pFiler, m_pPersSubent);
}

#undef NEXT_CODE

void OdDbImpAssocGeomDependency::dxfOutFields(OdDbDxfFiler* pFiler) const
{
  OdDbImpAssocDependency::dxfOutFields(pFiler);
  pFiler->wrSubclassMarker(OdDbAssocGeomDependency::desc()->name());

  pFiler->wrInt32(90, 0);    // Version?
  pFiler->wrBool(290, true); // Flag indicating presence of following object? 

  if(!m_pPersSubent.isNull())
  {
    m_pPersSubent->dxfOutFields(pFiler);
  }
  else
    ODA_FAIL();
}

OdDbAssocPersSubentIdPtr OdDbImpAssocGeomDependency::persistentSubentId() const
{
  return m_pPersSubent;
}

OdResult OdDbImpAssocGeomDependency::getTransientSubentIds(OdArray<OdDbSubentId>& transientSubentIds) const
{
  if (!m_pPersSubent.isNull())
  {
    OdDbObjectId idDependentOn = geomObject();
    if (!idDependentOn.isNull())
    {
      OdDbEntityPtr pEnt = idDependentOn.openObject();
      return m_pPersSubent->getTransientSubentIds(pEnt, transientSubentIds);
    }
  }
  return eNullObjectPointer;
}

OdResult OdDbImpAssocGeomDependency::setSubentity(const OdDbSubentId& transientSubentId)
{
  if ( !isAttachedToObject() )
    return eNullObjectPointer;

  OdDbObjectId objId = geomObject();
  OdDbObjectPtr pObj = objId.openObject();
  if ( pObj.isNull() )
    return eNullObjectPointer;

  if ( !pObj->isKindOf(OdDbEntity::desc()) )
    return eNullObjectPointer;

  OdSmartPtr<OdDbAssocPersSubentIdPE> pPE = pObj->isA()->getX(OdDbAssocPersSubentIdPE::desc());
  if ( pPE.isNull() )
    return eNullObjectPointer;

  //pPE->setAssocGeomDependency(this); // TODO ABT 09222017

  OdDbEntityPtr pEntity = OdDbEntity::cast(pObj);
  pEntity->upgradeOpen();

  
  m_pPersSubent = pPE->createNewPersSubent(pEntity, m_refPath, transientSubentId);
  if ( m_pPersSubent.isNull() )
    return eNullObjectPointer;

  switch ( transientSubentId.type() )
  {
    case OdDb::kVertexSubentType:
    {
      OdGePoint3d vertex;
      if ( pPE->getVertexSubentityGeometry(pEntity, transientSubentId, vertex) != eOk )
        return eBadObjType;
      OdGePoint3dArray arrPt;
      arrPt.append(vertex);
      return setVertexSubentityGeometry(arrPt);
    }
    break;
    case OdDb::kEdgeSubentType:
    {
      OdGeCurve3d *pCurve;
      if ( pPE->getEdgeSubentityGeometry(pEntity, transientSubentId, pCurve) != eOk )
        return eBadObjType;

      OdArray<const OdGeCurve3d *> arrEdge;
      arrEdge.append(pCurve);
      OdResult res = setEdgeSubentityGeometry(arrEdge);
      delete pCurve;
      return res;
    }
    break;
    default:
      ODA_FAIL_ONCE();
  }

  //pPE->setAssocGeomDependency(NULL);// TODO ABT 09222017

  return eBadObjType;
}

OdDb::SubentType OdDbImpAssocGeomDependency::subentType() const
{
  if (!m_pPersSubent.isNull())
  {
    OdDbObjectId idDependentOn = geomObject();
    if (!idDependentOn.isNull())
    {
      OdDbEntityPtr pEnt = idDependentOn.openObject();
      return m_pPersSubent->subentType(pEnt);
    }
  }
  return OdDb::kNullSubentType;
}

OdInt32 OdDbImpAssocGeomDependency::transientSubentCount() const
{
  if (!m_pPersSubent.isNull())
  {
    OdDbObjectId idDependentOn = geomObject();
    if (!idDependentOn.isNull())
    {
      OdDbEntityPtr pEnt = idDependentOn.openObject();
      return m_pPersSubent->transientSubentCount(pEnt);
    }
  }
  return 0;
}


bool OdDbImpAssocGeomDependency::isCachingSubentityGeometry() const
{
  return m_bCachingGeometry;
}


void OdDbImpAssocGeomDependency::setCachingSubentityGeometry(bool bCaching)
{
  m_bCachingGeometry = bCaching;
}


OdResult OdDbImpAssocGeomDependency::getVertexSubentityGeometry(OdGePoint3dArray& vertexPositions) const
{
  if ( subentType() != OdDb::kVertexSubentType )
    return eBadObjType;
  
  OdGePoint3d vertex = m_subentPt;
  vertexPositions.append(vertex);

  return eOk;
}


OdResult OdDbImpAssocGeomDependency::getVertexSubentityGeometryFromPersSubentiy(OdGeCurve3d* & geometry, OdGePoint3dArray& vertexPositions) const
{
  OdArray<OdGeCurve3d*> arrCurve;
  
  //OdResult res = getBaseEdgeSubentityGeometry(arrCurve);
  OdResult res = getEdgeSubentityGeometry(arrCurve);

  if (res != eOk)
    return res;

  ODA_ASSERT(arrCurve.length() == 1);
  geometry = arrCurve[0];

  OdGePoint3d vertex;
  OdGePoint3d startPoint;
  OdGePoint3d endPoint;
  OdDbAssocSimplePersSubentIdPtr pSimplePersSubentId = OdDbAssocSimplePersSubentId::cast(m_pPersSubent);
  OdDbAssocSingleEdgePersSubentIdPtr pSinglePersSubentId = OdDbAssocSingleEdgePersSubentId::cast(m_pPersSubent);
  OdDbAssocEdgePersSubentIdPtr pEdgePersSubentId = OdDbAssocEdgePersSubentId::cast(m_pPersSubent);

  if (arrCurve[0]->isKindOf(OdGe::kCircArc3d))
  {
    OdGeCircArc3d refGeArc = *(OdGeCircArc3d*)geometry;
    vertex = refGeArc.center();
    startPoint = refGeArc.startPoint();
    endPoint = refGeArc.endPoint();
  }
  else
  {
    OdGeLineSeg3d refLineSegment = *((OdGeLineSeg3d*)geometry);

    startPoint = refLineSegment.startPoint();
    endPoint = refLineSegment.endPoint();
  }
  
  if (!pSinglePersSubentId.isNull())
  {
    vertex = m_subentPt;
  }

  if (!pEdgePersSubentId.isNull())
  {
    vertex = m_subentPt;
  }

  if (!pSimplePersSubentId.isNull())
  {
    OdDbSubentId idPers = pSimplePersSubentId->subentId(NULL);
    OdGsMarker indMark = idPers.index();

    if (indMark == -1)
      vertex = startPoint;
    else if (indMark == -3)
      vertex = endPoint;
    else
    {
      ODA_ASSERT(false);
      //return eInvalidIndex;
    }
  }

  vertexPositions.append(vertex);

  return eOk;
}

OdResult OdDbImpAssocGeomDependency::getBaseEdgeSubentityGeometry(OdArray<OdGeCurve3d*>& edgeCurves) const
{
  OdDbEntityPtr pEnt;
  OdDbObjectIdArray path;
  m_refPath.getFullPath(path);
  int len = path.length();

  if (len > 0)
  {
      OdDbObjectId tempObjId = path.getAt(len - 1);
      pEnt = OdDbEntity::cast(tempObjId.openObject());
  }
  else
    pEnt = OdDbEntity::cast(m_dependentOnObjectId.openObject());

  if (pEnt.isNull())
    return eNullObjectId;

  OdDbAssocPersSubentIdPEPtr pPE = pEnt->isA()->getX(OdDbAssocPersSubentIdPE::desc());
  if (pPE.isNull())
  {
    ODA_ASSERT_ONCE(!"OdDbAssocPersSubentIdPE is null");
    return eNullObjectPointer;
  }
  OdGeCurve3d* pCurve = NULL;
  OdArray<OdDbSubentId> arrIds;

  OdDbAssocPolylinePersSubentIdPEPtr polylinePersSubentPE = OdDbAssocPolylinePersSubentIdPE::cast(pPE);
  OdDbAssocEdgePersSubentIdPtr polylinePersSubent = OdDbAssocEdgePersSubentId::cast(m_pPersSubent);

  if (!m_bPolylineGeometry && !polylinePersSubentPE.isNull() && polylinePersSubent.isNull())
    return eNullObjectPointer;
    
  if (m_pPersSubent.isNull())
    return eNullObjectPointer;

  OdResult res = m_pPersSubent->getTransientSubentIds(pEnt, arrIds);
  if (res != eOk)
    return res;
  
  //pPE->setAssocGeomDependency(this);

  for (OdUInt32 i = 0; i < arrIds.length(); ++i)
  {
    res = pPE->getEdgeSubentityGeometry(pEnt, arrIds[i], this, pCurve);
    if (res != eOk)
      return res;
    edgeCurves.append(pCurve);
  }

  //pPE->setAssocGeomDependency(NULL);// TODO ABT 09222017

  return eOk;
}

OdResult OdDbImpAssocGeomDependency::getEdgeSubentityGeometry(OdArray<OdGeCurve3d*>& edgeCurves) const
{
  OdDbCompoundObjectId compoundObjectId;
  getDependentOnCompoundObject(compoundObjectId);
  if (compoundObjectId.leafId().isErased())
    return eWasErased;

  OdResult res = eBadObjType;

  if ( subentType() != OdDb::kEdgeSubentType )
    return eBadObjType;

  OdDbEntityPtr pEnt = compoundObjectId.leafId().openObject();

  if (pEnt.isNull())
  {
    ODA_ASSERT(!L"CompoundObject not completed yet");
    return eNullObjectId;
  }

  OdDbAssocPersSubentIdPEPtr pPE = pEnt->isA()->getX(OdDbAssocPersSubentIdPE::desc());
  if (pPE.isNull())
  {
    ODA_ASSERT(!L"OdDbAssocPersSubentIdPEPtr is Null");
    return eNullObjectId;
  }

  OdArray<OdDbSubentId> arrIds;
  res = pPE->getTransientSubentIds(pEnt, persistentSubentId(), arrIds);
  
  if (res != eOk || 0 == arrIds.length())
    return res;

  OdGeCurve3d* pCurve = NULL;

  res = pPE->getEdgeSubentityGeometry(pEnt, arrIds[0], pCurve);
  
  if (res != eOk)
    return res;

  if (!compoundObjectId.isSimpleObjectId())
  {
    OdGeMatrix3d mTrans;
    res = compoundObjectId.getTransform(mTrans);

    if (res != eOk)
      return res;
    
    pCurve->transformBy(mTrans);
  }
  
  edgeCurves.append(pCurve);

  return res;
}
  
OdResult OdDbImpAssocGeomDependency::getFaceSubentityGeometry(OdArray<OdGeSurface*>& faceSurfaces) const
{
  return eNotImplementedYet;
}

OdResult OdDbImpAssocGeomDependency::setVertexSubentityGeometry(const OdGePoint3dArray& newVertexPositions)
{
  if ( subentType() != OdDb::kVertexSubentType )
    return eBadObjType;
  
  if ( newVertexPositions.length() == 0 )
    return eBadObjType;

  m_subentPt = newVertexPositions[0];

  return eOk;
}

OdResult OdDbImpAssocGeomDependency::setEdgeSubentityGeometry(const OdArray<const OdGeCurve3d*>& newEdgeCurves)
{
  if ( subentType() != OdDb::kEdgeSubentType )
    return eBadObjType;

  if ( newEdgeCurves.length() == 0 )
    return eBadObjType;

  delete m_pSubentCurve;

#if 0 // when copy will create for all gepm object remove this precompiled directive
  m_pSubentCurve = (OdGeCurve3d*)newEdgeCurves[0]->copy();
#else
  if ( newEdgeCurves[0]->isKindOf(OdGe::kCircArc3d) )
  {
    OdGeCircArc3d *pCircArc = (OdGeCircArc3d*)newEdgeCurves[0];

    m_pSubentCurve = new OdGeCircArc3d(pCircArc->center(), pCircArc->normal(),
                    pCircArc->refVec(), pCircArc->radius(),
                    pCircArc->startAng(), pCircArc->endAng());
  }
  else if ( newEdgeCurves[0]->isKindOf(OdGe::kLineSeg3d) )
  {
    OdGeLineSeg3d *pLineSeg = (OdGeLineSeg3d*)newEdgeCurves[0];

    m_pSubentCurve = new OdGeLineSeg3d(pLineSeg->startPoint(), pLineSeg->endPoint());
  }
  else if ( newEdgeCurves[0]->isKindOf(OdGe::kRay3d) )
  {
    OdGeRay3d *pRay = (OdGeRay3d*)newEdgeCurves[0];
    m_pSubentCurve = new OdGeRay3d(pRay->pointOnLine(), pRay->direction());
  }
  else if ( newEdgeCurves[0]->isKindOf(OdGe::kLine3d) )
  {
    OdGeLine3d *pLine = (OdGeLine3d*)newEdgeCurves[0];
    m_pSubentCurve = new OdGeLine3d(pLine->pointOnLine(), pLine->direction());
  }
  else if ( newEdgeCurves[0]->isKindOf(OdGe::kEllipArc3d) )
  { 
    OdGeEllipArc3d *pEllipArc = ( OdGeEllipArc3d*)newEdgeCurves[0];
    m_pSubentCurve = new OdGeEllipArc3d(pEllipArc->center(), pEllipArc->majorAxis(), pEllipArc->minorAxis(),
                pEllipArc->majorRadius(), pEllipArc->minorRadius(),
                pEllipArc->startAng(), pEllipArc->endAng());
  }
  else if ( newEdgeCurves[0]->isKindOf(OdGe::kNurbCurve3d) )
  {
    OdGeNurbCurve3d *pNurb = (OdGeNurbCurve3d*)newEdgeCurves[0];
    OdGeDoubleArray arrWeights;
    OdGePoint3dArray arrCtrlPt;

    OdInt32 i;
    for ( i = 0; i < pNurb->numWeights(); i++ )
      arrWeights.append(pNurb->weightAt(i));

    for ( i = 0; i < pNurb->numControlPoints(); i++ )
      arrCtrlPt.append(pNurb->controlPointAt(i));

    double period;
    m_pSubentCurve = new OdGeNurbCurve3d(pNurb->degree(), pNurb->knots(), arrCtrlPt, arrWeights, pNurb->isPeriodic(period));
  }
#endif 
  return eOk;
}

OdResult OdDbImpAssocGeomDependency::setFaceSubentityGeometry(const OdArray<const OdGeSurface*>& newFaceSurfaces)
{
  return eNotImplementedYet;
}

OdResult OdDbImpAssocGeomDependency::dependentOnObjectMirrored()
{
  if (!m_pPersSubent.isNull())
  {
    OdDbObjectId idDependentOn = dependentOnObject();
    if (!idDependentOn.isNull())
    {
      OdDbEntityPtr pEnt = idDependentOn.openObject();
      return m_pPersSubent->mirror(pEnt);
    }
  }
  return eNullObjectPointer;
}

OdResult OdDbImpAssocGeomDependency::setOwningAction(OdDbObjectId actionId, OdDbAssocDependency *pDep)
{
  if ( m_pPersSubent.isNull() )
    return eBadObjType;

  return OdDbImpAssocDependency::setOwningAction(actionId, pDep);
}

OdResult OdDbImpAssocGeomDependency::updateDependentOnObject(OdDbAssocDependency * pDep)
{
  return eOk;
}

OdResult OdDbImpAssocGeomDependency::createNewEdgePersSubent(OdDbEntity* pEntity, OdDbAssocAction* pAction, OdUInt32& subentIndex)//, bool bNewPersSubent)//, bool bPolylineGeometry)
{
  //if ( pEntity->isKindOf(OdDbBlockReference::desc()) || pEntity->isKindOf(OdDbPoint::desc()))
  //{
  //  OdDbAssocSimplePersSubentIdPtr res = OdDbAssocSimplePersSubentId::createObject();
  //  res->init(subentId);
  //  return res;
  //}

  if (pEntity->isKindOf(OdDbSurface::desc()) || pEntity->isKindOf(OdDb3dSolid::desc()))
  {
    OdDbAssocNetworkPtr pNetOwner = OdDbAssocNetwork::cast(pAction->owningNetwork().openObject(OdDb::kForRead));
    OdDbObjectIdArray arrActions = pNetOwner->getActions();
    //find maximum index
    subentIndex = 0;

    //if (bNewPersSubent)
    {
      for (OdUInt32 counter = 0; counter < arrActions.length(); counter++)
      {
        OdDbAssocActionPtr pTempAction = OdDbAssocAction::cast(arrActions[counter].openObject(OdDb::kForRead));

        if (!pTempAction.isNull())
        {
          OdDbObjectIdArray DepIDs;
          pTempAction->getDependencies(true, true, DepIDs);

          for (OdUInt32 numDep = 0; numDep < DepIDs.length(); numDep++)
          {
            OdDbAssocGeomDependencyPtr pGeomDep0 = getAssocGeomDependencyPtr(pTempAction, OdDb::kForRead, numDep);
            
            if (!pGeomDep0.isNull())
            {
              OdDbAssocPersSubentIdPtr perssubEntId = pGeomDep0->persistentSubentId();

              //OdUInt32 ind = perssubEntId->subentId(NULL).index();

              OdDbAssocAsmBasedEntityPersSubentIdPtr pPersSubent = OdDbAssocAsmBasedEntityPersSubentId::cast(perssubEntId);
              OdDbAssocIndexPersSubentIdPtr  pPersSubentInd = OdDbAssocIndexPersSubentId::cast(perssubEntId);
              OdDbAssocSimplePersSubentIdPtr pPersSubentSimple = OdDbAssocSimplePersSubentId::cast(perssubEntId);
              OdDbAssocEdgePersSubentIdPtr pPersSubentEdge = OdDbAssocEdgePersSubentId::cast(perssubEntId);
              ODA_ASSERT(pPersSubentEdge.isNull());
              ODA_ASSERT(pPersSubentSimple.isNull());

              OdUInt32 ind = 0;

              if (!pPersSubent.isNull())
              {
                ind = pPersSubent->subentId(NULL).index();
              }

              if (!pPersSubentInd.isNull())
              {
                ind = pPersSubentInd->subentId(NULL).index();
              }

              if (!pPersSubentSimple.isNull())
              {
                ind = pPersSubentSimple->subentId(NULL).index();
              }

              if (ind > subentIndex)
                subentIndex = ind;
            }
          }
        }
      }
      subentIndex = subentIndex == 0 ? 2 : subentIndex + 1;

      OdDbObjectId objId = OdDbAssocPersSubentManager::getInstanceFromDatabase(pEntity->database());
      ODA_ASSERT(!objId.isNull());
      OdDbAssocPersSubentManagerPtr pAssocPSManager = objId.openObject(/*OdDb::kForWrite*/);
      OdUInt32 maxSubentIdIndex = pAssocPSManager->maxSubentIdIndex();

      if (maxSubentIdIndex >= subentIndex)
        subentIndex = maxSubentIdIndex + 1;
    }

    m_pPersSubent = OdDbAssocAsmBasedEntityPersSubentId::createObject();
    ((OdDbAssocAsmBasedEntityPersSubentIdPtr)m_pPersSubent)->init(OdDbSubentId(OdDb::kEdgeSubentType, subentIndex));

    return eOk;
  }
  else
  {
    ODA_ASSERT(pEntity->isKindOf(OdDbPolyline::desc()));

    if (pEntity->isKindOf(OdDbPolyline::desc()))
    {
      //find maximum index
      OdDbPolylinePtr pPolyLine = OdDbPolyline::cast(pEntity);
      OdUInt32 ptQty = pPolyLine->numVerts();
      OdInt32 maxIdentifier = 0;
      for (OdUInt32 i = 0; i < ptQty; i++)
        maxIdentifier = odmax(pPolyLine->getVertexIdentifierAt(i), maxIdentifier);

      OdInt32 startVertexIdx = subentIndex - 1;
      //OdInt32 startVertexIdx = subentIndex;
      OdInt32 endVertexIdx = pPolyLine->isClosed() && startVertexIdx == pPolyLine->numVerts() - 1 ? 0 : startVertexIdx + 1;

      if (pPolyLine->getVertexIdentifierAt(startVertexIdx) == 0)
      {
        pPolyLine->setVertexIdentifierAt(startVertexIdx, ++maxIdentifier);
      }

      if (pPolyLine->getVertexIdentifierAt(endVertexIdx) == 0)
      {
        pPolyLine->setVertexIdentifierAt(endVertexIdx, ++maxIdentifier);
      }

      m_pPersSubent = OdDbAssocEdgePersSubentId::createObject();
      ((OdDbAssocEdgePersSubentIdPtr)m_pPersSubent)->init(pPolyLine->getVertexIdentifierAt(startVertexIdx), pPolyLine->getVertexIdentifierAt(endVertexIdx));

      return eOk;
    }
    else
    {
      m_bPolylineGeometry = false;
    }

    return eInvalidDrawing;
  }
}


OdResult OdDbImpAssocGeomDependency::createNewSingleEdgePersSubent(const OdDbObjectId lineID)
{
  OdDbEntityPtr pEntity = OdDbEntity::cast(lineID.openObject());

  m_bPolylineGeometry = pEntity->isKindOf(OdDbPolyline::desc());

  if (m_bPolylineGeometry)
    m_pPersSubent = OdDbAssocEdgePersSubentId::createObject();
  else
    m_pPersSubent = OdDbAssocSingleEdgePersSubentId::createObject();

  //m_pPersSubent = m_bPolylineGeometry ? OdDbAssocEdgePersSubentId::createObject() : OdDbAssocSingleEdgePersSubentId::createObject();

  if (m_pPersSubent.isNull())
    return eNullObjectPointer;


  return eOk;
}

OdResult OdDbImpAssocGeomDependency::createNewAssocSimplePersSubent(OdDbObjectId lineID, OdDb::SubentType type, PointPositionOnBlockLine isStartPoint)
{
  OdDbEntityPtr pEntity = OdDbEntity::cast(lineID.openObject());
  
  m_bPolylineGeometry = pEntity->isKindOf(OdDbPolyline::desc());

  OdGsMarker index = 0; 

  if (isStartPoint == kStartPoint)
    index = -1;

  if (isStartPoint == kEndPoint)
    index = -3;

  if (isStartPoint == kStartPoint || isStartPoint == kEndPoint)
  {
    OdDbSubentId subentId(type, index);

    m_pPersSubent = OdDbAssocSimplePersSubentId::createObject();
    ((OdDbAssocSimplePersSubentIdPtr)m_pPersSubent)->init(subentId);
  }
    
  if (isStartPoint == kOnLine)
  {
    if (m_bPolylineGeometry)
      m_pPersSubent = OdDbAssocEdgePersSubentId::createObject();
    else
      m_pPersSubent = OdDbAssocSingleEdgePersSubentId::createObject();
  }

  if (m_pPersSubent.isNull())
    return eNullObjectPointer;

  return eOk;
}

OdStringArray OdDbImpAssocGeomDependency::compareWith(const OdDbImpAssocDependency *depIDToCompare, OdDb::DwgVersion filerVersion) const
{
	OdStringArray retArray = OdDbImpAssocDependency::compareWith(depIDToCompare, filerVersion);

	OdDbImpAssocGeomDependency* comparedDep = (OdDbImpAssocGeomDependency *)(depIDToCompare);

	return retArray;
}

OdDbObjectId OdDbImpAssocGeomDependency::geomObject() const
{
  OdDbCompoundObjectId compId;
  getDependentOnCompoundObject(compId);
  return !compId.isEmpty() ? compId.leafId() : OdDbObjectId::kNull;
}

