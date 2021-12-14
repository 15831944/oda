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

// GiWorldGeometryImpl.cpp: implementation of the OdGiWorldGeometryImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "DgStdAfx.h"
#include "GiDrawDgnElementForExplode.h"
#include "OdTextIterator.h"
#include "Gi/GiPolyline.h"
#include "Gi/GiUtils.h"
#include "Ge/GeNurbCurve3d.h"
#include "Ge/GePlane.h"
#include "OdDToStr.h"
#include "CmTransparencyArray.h"
#include "CmEntityColorArray.h"
#include "GsMarkerArray.h"

//----------------------------------------------------------
//
// OdGiDrawDgnElementForExplode
//
//----------------------------------------------------------

OdGiDrawDgnElementForExplode::OdGiDrawDgnElementForExplode() 
  : m_pEdgeData(0) 
{ 
  OdGiBaseVectorizer::m_flags |= (kDrawInvisibleEnts|kDrawLayerOff|kDrawLayerFrozen);
  setContext(this); 
  OdGiGeometrySimplifier::setDrawContext(OdGiBaseVectorizer::drawContext());
  m_pModelToEyeProc->setDrawContext(OdGiBaseVectorizer::drawContext());
  output().setDestGeometry(*this);

  m_b3dElement = true;
  m_bClipEnabled = false;
}

//----------------------------------------------------------

OdResult OdGiDrawDgnElementForExplode::explode(const OdDgElement* pEnt, OdRxObjectPtrArray& entitySet)
{
  OdUInt32 oldArraySize = entitySet.size();
  try 
  {
    m_b3dElement = false;

    if( pEnt->isKindOf( OdDgGraphicsElement::desc() ) )
    {
      CDGElementGeneral* pImpl = (CDGElementGeneral*)(OdDgElementImpl::getImpl( pEnt ));

      if( pImpl )
      {
        m_b3dElement = (pImpl->Is3D() == Dgn8::True);
      }
    }

    setDatabase(pEnt->database());
    setAttributes(pEnt); // use virtual method of OdGiBaseVectorizer
    worldDrawElement(pEnt);
    getEntityArray(entitySet);
    return (entitySet.size() > oldArraySize) ? eOk : eCannotExplodeEntity;
  }
  catch (...)
  {
    return eCannotExplodeEntity;
  }
}

void OdGiDrawDgnElementForExplode::worldDrawElement(const OdDgElement* pEnt)
{
  if ( !pEnt->worldDraw(this) )
  {
    pEnt->viewportDraw(this);
  }
}

void OdGiDrawDgnElementForExplode::addElement(OdDgElementPtr pEnt, bool needTransform)
{
  ODA_ASSERT(!pEnt.isNull());

  if( needTransform )
  {
    OdGeMatrix3d matr = getModelToWorldTransform();

    if( matr != OdGeMatrix3d::kIdentity )
    {
      if( pEnt->transformBy(matr) != eOk )
      {
        OdDgElementPtr pOldEnt(pEnt.get());

        if( pOldEnt->getTransformedCopy(matr, pEnt) != eOk )
        {
          return;
        }
      }
    }
  }

  setCurrTraitsTo(pEnt);
  m_entityList.push_back(pEnt);
}

void OdGiDrawDgnElementForExplode::pushClipBoundary(OdGiClipBoundary* pBoundary )
{
  OdGiBaseVectorizer::pushClipBoundary( pBoundary );

  m_bClipEnabled = true;
}

void OdGiDrawDgnElementForExplode::pushClipBoundary(OdGiClipBoundary* pBoundary, OdGiAbstractClipBoundary* pAbsBoundary)
{
  OdGiBaseVectorizer::pushClipBoundary( pBoundary, pAbsBoundary );

  m_bClipEnabled = true;
}

void OdGiDrawDgnElementForExplode::popClipBoundary()
{
  OdGiBaseVectorizer::popClipBoundary();

  m_bClipEnabled = false;
}

void OdGiDrawDgnElementForExplode::setCurrTraitsTo(OdDgElement* pEnt)
{
  // ! traits are set using 'doSubentities = false' because shell faces for example
  // may have different colors

  if( pEnt->isKindOf( OdDgGraphicsElement::desc()) )
  {
    OdDgGraphicsElement* pElement = (OdDgGraphicsElement*)pEnt;

    pElement->setLevelId( layer() );

    OdDgCmEntityColor textColor = trueColor();

    OdInt32 uColorIndex = 0;

    if( textColor.colorMethod() == OdDgCmEntityColor::kByColor )
    {
      OdDgDatabase* pDb = getDatabase();

      bool bFoundTrueColor = false;

      if( pDb )
      {
        uColorIndex = OdDgColorTable::getColorIndexByRGB( pDb, ODRGB( textColor.red(), textColor.green(), textColor.blue()));
      }
      else
      {
        uColorIndex = color();
      }
    }
    else if( textColor.colorMethod() == OdDgCmEntityColor::kByLayer )
    {
      uColorIndex = -1;
    }
    else if( textColor.colorMethod() == OdDgCmEntityColor::kByBlock )
    {
      uColorIndex = -2;
    }
    else
    {
      uColorIndex = color();
    }

    pElement->setColorIndex( uColorIndex );
    pElement->setLineWeight( lineWeight() );
    pElement->setLineStyleEntryId(0);

    OdUInt32 uEntryId = 0;
    
    OdDgElementId idLineStyle = lineType();

    if( !idLineStyle.isNull() )
    {
      OdDgElementPtr pLType = idLineStyle.openObject(OdDg::kForRead);

      if( !pLType.isNull() && pLType->isKindOf(OdDgLineStyleTableRecord::desc()) )
      {
        OdDgLineStyleTableRecordPtr pLineStyle = pLType;

        if( !pLineStyle.isNull() )
        {
          uEntryId = pLineStyle->getEntryId();
          pElement->setLineStyleEntryId(uEntryId);
        }
      }
    }

    double dLineScale = lineTypeScale();

    if( (uEntryId > 7) && !OdZero(dLineScale - 1.0) )
    {
      if( (pEnt->getElementType() == OdDgElement::kTypeLine) ||
          (pEnt->getElementType() == OdDgElement::kTypeLineString) ||
          (pEnt->getElementType() == OdDgElement::kTypeArc) ||
          (pEnt->getElementType() == OdDgElement::kTypeShape)
        )
      {
        OdDgLineStyleModificationLinkagePtr pLineScaleLinkage = OdDgLineStyleModificationLinkage::createObject();
        pLineScaleLinkage->setLineStyleScale( dLineScale );
        pElement->addLinkage( pLineScaleLinkage->getPrimaryId(), pLineScaleLinkage.get() );
      }
    }

    if( (pEnt->getElementType() == OdDgElement::kTypeShape) ||
        (pEnt->getElementType() == OdDgElement::kTypeMesh)  
      )
    {
      OdDgElementId idMaterial = material();

      if( !idMaterial.isNull() )
      {
        OdDgElementPtr pMat = idMaterial.openObject(OdDg::kForRead);

        if( !pMat.isNull() && pMat->isKindOf(OdDgMaterialTableRecord::desc() ) )
        {
          OdDgMaterialTableRecordPtr pMaterial = pMat;

          OdDgMaterialTablePtr pMatTable = pMaterial->database()->getMaterialTable(OdDg::kForRead);

          OdDgElementId idMatTable = pMaterial->ownerId();
          OdDgElementId idInternalMatTable = pMatTable->elementId();

          if( idMaterial == idInternalMatTable ) // Internal material
          {
            OdDgInternalMaterialLinkagePtr pMatLinkage = OdDgInternalMaterialLinkage::createObject();
            pMatLinkage->setMaterialTableRecordId((OdUInt64)(idMaterial.getHandle()));

            pElement->addLinkage( pMatLinkage->getPrimaryId(), pMatLinkage.get());
          }
          else // External material
          {
            OdDgMaterialLinkagePtr pMatLinkage = OdDgMaterialLinkage::createObject();
            pMatLinkage->setMaterialName( pMaterial->getName() );

            pElement->addLinkage( pMatLinkage->getPrimaryId(), pMatLinkage.get());
          }
        }
      }
    }

    if( (pEnt->getElementType() == OdDgElement::kTypeShape) && (drawFlags() & kDrawContourFill) )
    {
      OdDgFillColorLinkagePtr pFillColor = OdDgFillColorLinkage::createObject();

      pFillColor->setColorIndex( secondaryTrueColor().colorIndex() );
    }
  }
}

OdDgElementPtr OdGiDrawDgnElementForExplode::makeCircle(const OdGePoint3d& center,
                                                        double radius,
                                                        const OdGeVector3d& normal)
{

  if( m_b3dElement )
  {
    return makeCircle3d( center, radius, normal );
  }
  else
  {
    return makeCircle2d( center, radius, normal );
  }
}

OdDgElementPtr OdGiDrawDgnElementForExplode::makeCircle2d(const OdGePoint3d& center,
                                                          double radius,
                                                          const OdGeVector3d& normal)
{
  OdDgArc2dPtr pCircle = OdDgArc2d::createObject();
  pCircle->setOrigin(OdGePoint2d(center.x , center.y) );
  pCircle->setPrimaryAxis(radius);
  pCircle->setSecondaryAxis(radius);
  return pCircle;
}

OdDgElementPtr OdGiDrawDgnElementForExplode::makeCircle3d(const OdGePoint3d& center,
                                                          double radius,
                                                          const OdGeVector3d& normal)
{
  OdDgArc3dPtr pCircle = OdDgArc3d::createObject();
  pCircle->setOrigin(center);
  pCircle->setPrimaryAxis(radius);
  pCircle->setSecondaryAxis(radius);
  
  OdGeVector3d vrZAxis    = OdGeVector3d::kZAxis;
  OdGeVector3d vrRotation = OdGeVector3d::kXAxis;

  OdGeMatrix3d matRotation;
  matRotation = matRotation.setToPlaneToWorld(normal);

  OdGeQuaternion quatRotation;
  quatRotation.set(matRotation);

  pCircle->setRotation(quatRotation);

  return pCircle;
}

void OdGiDrawDgnElementForExplode::circleProc(const OdGePoint3d& center,
															                double radius, const OdGeVector3d& normal,
                                              const OdGeVector3d* pExtrusion)
{
  if( m_bClipEnabled )
  {
    OdGiGeometrySimplifier::circleProc( center, radius, normal, pExtrusion );
  }
  else
  {
    addElement(makeCircle(center, radius, normal));
  }
}


void OdGiDrawDgnElementForExplode::circleProc(const OdGePoint3d& p1,
															                const OdGePoint3d& p2, const OdGePoint3d& p3,
                                              const OdGeVector3d* pExtrusion)
{
  if( m_bClipEnabled )
  {
    OdGiGeometrySimplifier::circleProc( p1, p2, p3, pExtrusion );
  }
  else
  {
    OdGeCircArc3d arc(p1,p2,p3);
    addElement(makeCircle(arc.center(), arc.radius(), arc.normal()));
  }
}


void OdGiDrawDgnElementForExplode::circularArcProc(const OdGePoint3d& center,
																				           double radius,
																				           const OdGeVector3d& normal,
																				           const OdGeVector3d& startVector,
																				           double sweepAngle,
																				           OdGiArcType arcType,
                                                   const OdGeVector3d* pExtrusion)
{
  if( pExtrusion || m_bClipEnabled )
  {
    OdGiGeometrySimplifier::circularArcProc(center, radius, normal, startVector, sweepAngle, arcType, pExtrusion);
  }
  else if( m_b3dElement )
  {
    addElement(makeArc3d( center, radius, normal, startVector, sweepAngle, arcType ));
  }
  else
  {
    addElement(makeArc2d( center, radius, normal, startVector, sweepAngle, arcType ));
  }
}

OdDgElementPtr OdGiDrawDgnElementForExplode::makeArc2d(const OdGePoint3d& center, 
                                                         double radius,
                                                           const OdGeVector3d& /*normal*/,
                                                             const OdGeVector3d& startVector,
                                                               double sweepAngle, 
                                                                 OdGiArcType arcType 
                                                      )
{
  OdDgArc2dPtr pArc = OdDgArc2d::createObject();

  pArc->setOrigin( OdGePoint2d(center.x, center.y));
  pArc->setPrimaryAxis( radius );
  pArc->setSecondaryAxis( radius );
  pArc->setSweepAngle( sweepAngle );
  pArc->setStartAngle(startVector.angleTo(OdGeVector3d::kXAxis));

  return pArc;
}

OdDgElementPtr OdGiDrawDgnElementForExplode::makeArc3d(const OdGePoint3d& center, 
                                                         double radius,
                                                           const OdGeVector3d& normal,
                                                             const OdGeVector3d& startVector,
                                                               double sweepAngle, 
                                                                 OdGiArcType arcType 
                                                       )
{
  OdDgArc3dPtr pArc = OdDgArc3d::createObject();

  pArc->setOrigin( center );
  pArc->setPrimaryAxis( radius );
  pArc->setSecondaryAxis( radius );
  pArc->setSweepAngle( sweepAngle );

  OdGeMatrix3d matRotation;
  
  matRotation = matRotation.setToPlaneToWorld(normal);

  OdGeQuaternion quatRotation;
  quatRotation.set(matRotation);

  pArc->setRotation(quatRotation);

  OdGeVector3d axisX = OdGeVector3d::kXAxis;
  axisX.transformBy(matRotation);
   
  double angl = axisX.angleTo(startVector, normal);
   
  pArc->setStartAngle(angl);

  double dThickness = thickness();

  if( !OdZero(dThickness) )
  {
    EArc3D* pArcImpl = (EArc3D*)(OdDgElementImpl::getImpl(pArc));
    pArcImpl->setThickness(dThickness);
  }

  return pArc;
}

void OdGiDrawDgnElementForExplode::circularArcProc(const OdGePoint3d& start,
                                                   const OdGePoint3d& point,
                                                   const OdGePoint3d& end,
                                                   OdGiArcType arcType,
                                                   const OdGeVector3d* pExtrusion)
{
  OdGeCircArc3d arc(start, point, end);
  circularArcProc(arc.center(), arc.radius(), arc.normal(), start - arc.center(),
    arc.endAng() - arc.startAng(), arcType, pExtrusion);
}

OdDgElementPtr OdGiDrawDgnElementForExplode::makePolyline(OdInt32 nbPoints,
                                                          const OdGePoint3d* pVertexList,
                                                          const OdGeVector3d* pNormal )
{
  if (nbPoints < 2)
  {
    ODA_FAIL();
    return OdDgElementPtr();
  }
  else
  {
    if( m_b3dElement )
    {
      return makePolyline3d( nbPoints, pVertexList, pNormal );
    }
    else
    {
      return makePolyline2d( nbPoints, pVertexList, pNormal );
    }
  }
}


OdDgElementPtr OdGiDrawDgnElementForExplode::makePolyline2d(OdInt32 nbPoints,
                                                              const OdGePoint3d* pVertexList,
                                                                const OdGeVector3d* pNormal 
                                                           )
{
  OdGePoint2dArray arrVertices2d;

  for( OdInt32 i = 0; i < nbPoints; i++ )
  {
    OdGePoint2d ptVertex2d( pVertexList[i].x, pVertexList[i].y );

    arrVertices2d.push_back(ptVertex2d);
  }

  if (nbPoints == 2)
  {

    OdDgLine2dPtr pLine2d = OdDgLine2d::createObject();
    pLine2d->setStartPoint(arrVertices2d[0]);
    pLine2d->setEndPoint(arrVertices2d[1]);

    return pLine2d;
  }
  else
  {
    OdDgLineString2dPtr pLineString2d = OdDgLineString2d::createObject();

    for(OdInt32 f=0; f<nbPoints; ++f)
    {
      pLineString2d->addVertex( arrVertices2d[f] );
    }

    return pLineString2d;
  }

  return OdDgElementPtr();
}

OdDgElementPtr OdGiDrawDgnElementForExplode::makePolyline3d(OdInt32 nbPoints,
                                                              const OdGePoint3d* pVertexList,
                                                                const OdGeVector3d* pNormal 
                                                           )
{
  if (nbPoints == 2)
  {

    OdDgLine3dPtr pLine3d = OdDgLine3d::createObject();
    pLine3d->setStartPoint(pVertexList[0]);
    pLine3d->setEndPoint(pVertexList[1]);

    double dThickness = thickness();

    if( !OdZero(dThickness) )
    {
      ELine3D* pLineImpl = (ELine3D*)(OdDgElementImpl::getImpl(pLine3d));
      pLineImpl->setThickness(dThickness);
    }

    return pLine3d;
  }
  else
  {
    OdDgLineString3dPtr pLineString3d = OdDgLineString3d::createObject();

    for(OdInt32 f=0; f<nbPoints; ++f)
    {
      pLineString3d->addVertex( pVertexList[f] );
    }

    double dThickness = thickness();

    if( !OdZero(dThickness) )
    {
      ELineString3D* pLineStringImpl = (ELineString3D*)(OdDgElementImpl::getImpl(pLineString3d));
      pLineStringImpl->setThickness(dThickness);
    }

    return pLineString3d;
  }

  return OdDgElementPtr();
}

void OdGiDrawDgnElementForExplode::polyline(OdInt32 nbPoints,
																            const OdGePoint3d* pVertexList,
																            const OdGeVector3d* pNormal,
																            OdGsMarker lBaseSubEntMarker)
{
  if (nbPoints < 2)
    return;

  if( m_bClipEnabled )
  {
    m_bClipEnabled = false;

    OdGiBaseVectorizer::polyline( nbPoints, pVertexList, pNormal, lBaseSubEntMarker );

    m_bClipEnabled = true;
  }
  else
  {
    addElement(makePolyline(nbPoints, pVertexList, pNormal), true);
  }
}

void OdGiDrawDgnElementForExplode::polylineOut(OdInt32 nbPoints, const OdGePoint3d* pVertexList)
{
  addElement(makePolyline(nbPoints, pVertexList, 0));
}

OdDgElementPtr OdGiDrawDgnElementForExplode::makePolygon(OdInt32 nbPoints, const OdGePoint3d* pVertexList)
{
  if (nbPoints < 3)
  {
    ODA_FAIL();
    return OdDgElementPtr();
  }
  else
  {
    if( m_b3dElement )
    {
      return makePolygon3d( nbPoints, pVertexList );
    }
    else
    {
      return makePolygon2d( nbPoints, pVertexList );
    }
  }
}

OdDgElementPtr OdGiDrawDgnElementForExplode::makePolygon2d(OdInt32 nbPoints, const OdGePoint3d* pVertexList)
{
  OdDgShape2dPtr pShape = OdDgShape2d::createObject();

  for( OdInt32 i = 0; i < nbPoints; i++ )
  {
    pShape->addVertex( OdGePoint2d(pVertexList[i].x, pVertexList[i].y ) );
  }

  return pShape;
}

OdDgElementPtr OdGiDrawDgnElementForExplode::makePolygon3d(OdInt32 nbPoints, const OdGePoint3d* pVertexList)
{
  OdDgShape3dPtr pShape = OdDgShape3d::createObject();

  for( OdInt32 i = 0; i < nbPoints; i++ )
  {
    pShape->addVertex( pVertexList[i] );
  }

  return pShape;
}

void OdGiDrawDgnElementForExplode::polygon(OdInt32 nbPoints, const OdGePoint3d* pVertexList)
{
  if( m_bClipEnabled )
  {
    m_bClipEnabled = false;

    OdGiBaseVectorizer::polygon( nbPoints, pVertexList );

    m_bClipEnabled = true;
  }
  else
  {
    addElement(makePolygon(nbPoints, pVertexList), true);
  }
}

void OdGiDrawDgnElementForExplode::facetOut(const OdInt32* pFaceList,
                                            const OdInt32* pEdgeIndices,
                                            const OdGeVector3d* /*pNormal*/)
{
  OdDgMeshFaceLoops::VerticesArray meshFace;

  const OdUInt8* vis = 0;

  if( m_pEdgeData )
  {
    vis = m_pEdgeData->visibility();
  }

  int n = *pFaceList++;

  for( int i=0; i < n; ++i)
  {
    OdDgMeshFaceLoops::FacePoint ptFace;
    ptFace.m_pointIndex = pFaceList[i];
    ptFace.m_impliesVisibleEdge = true;

    OdInt32 nEdgeIndex = pEdgeIndices[i];

    if( nEdgeIndex >= 0 && vis && vis[nEdgeIndex] == kOdGiInvisible )
    {
      ptFace.m_impliesVisibleEdge = false;
    }
    else
    {
      bool bAdditionalEdge = true;

      OdUInt32 uStartEdge = pFaceList[i];
      OdUInt32 uEndEdge = ( i == (n-1) ) ? pFaceList[0] : pFaceList[i+1];

      for( OdUInt32 k = 0; k < m_contourPairStart.size(); k++ )
      {
        if( ((uStartEdge == m_contourPairStart[k]) && (uEndEdge == m_contourPairEnd[k])) ||
            ((uStartEdge == m_contourPairEnd[k]) && (uEndEdge == m_contourPairStart[k]))
          )
        {
          bAdditionalEdge = false;
          break;
        }
      }

      ptFace.m_impliesVisibleEdge = !bAdditionalEdge;
    }

    if( nEdgeIndex >= 0 && m_pEdgeData && m_pEdgeData->colors() )
    {
      ptFace.m_pointIndex = m_pEdgeData->colors()[nEdgeIndex];
    }
    else if( nEdgeIndex >= 0 && m_pEdgeData && m_pEdgeData->trueColors() )
    {
      ptFace.m_dColorRGB[0] = m_pEdgeData->trueColors()[nEdgeIndex].red() / 255.0;
      ptFace.m_dColorRGB[1] = m_pEdgeData->trueColors()[nEdgeIndex].green() / 255.0;
      ptFace.m_dColorRGB[2] = m_pEdgeData->trueColors()[nEdgeIndex].blue() / 255.0;
    }
    else if( m_pVertexData && m_pVertexData->trueColors() )
    {
      ptFace.m_dColorRGB[0] = m_pVertexData->trueColors()[ptFace.m_pointIndex].red() / 255.0;
      ptFace.m_dColorRGB[1] = m_pVertexData->trueColors()[ptFace.m_pointIndex].green() / 255.0;
      ptFace.m_dColorRGB[2] = m_pVertexData->trueColors()[ptFace.m_pointIndex].blue() / 255.0;
    }

    if( m_pVertexData && m_pVertexData->normals() )
    {
      ptFace.m_vrNormal = m_pVertexData->normals()[ptFace.m_pointIndex];
    }

    meshFace.push_back( ptFace );
  }

  m_pMesh->addFace(meshFace);
}

void OdGiDrawDgnElementForExplode::shellProc(OdInt32 nbVertex,
                                             const OdGePoint3d* pVertexList,
                                             OdInt32 faceListSize,
                                             const OdInt32* pFaceList,
                                             const OdGiEdgeData* pEdgeData,
                                             const OdGiFaceData* pFaceData,
                                             const OdGiVertexData* pVertexData)
{
  if( faceListSize <= 5 && pFaceData == 0 && pEdgeData == 0 )
  {
    OdGePoint3dArray arrShapePts;

    OdInt32 nPts = abs(pFaceList[0]);

    if( nPts > 2 )
    {
      for( OdInt32 i = 0; i < nPts; i++ )
      {
        arrShapePts.push_back( pVertexList[pFaceList[i+1]] );
      }

      bool bAllPtsInPlane = true;

      if( nPts > 3 )
      {
        OdGeVector3d vrNormal = Dgn8::getNormal( arrShapePts, true );

        OdGePlane plane( arrShapePts[0], vrNormal );

        OdGePoint3d ptCheckPoint = arrShapePts[3].project(plane,vrNormal);

        if( !OdZero( arrShapePts[3].distanceTo(ptCheckPoint), 1e-6) )
        {
          bAllPtsInPlane = false;
        }
      }

      if( bAllPtsInPlane )
      {
        return addElement( makePolygon(arrShapePts.size(), arrShapePts.asArrayPtr()), true);
      }
    }
  }
  else if( nbVertex < 0x7FFF )
  {
    // create mesh face loops

    OdDgMeshFaceLoopsPtr pMesh = OdDgMeshFaceLoops::createObject();

    for( OdInt32 j = 0; j < nbVertex; j++ )
    {
      pMesh->addPoint( pVertexList[j] );
    }

    m_pMesh = pMesh;

    if( pEdgeData && pEdgeData->colors() )
    {
      pMesh->setUseColorTableIndexesFlag(true);
    } else if( (pEdgeData && pEdgeData->trueColors()) || (pVertexData && pVertexData->trueColors()) )
    {
      pMesh->setUseDoubleColorsFlag(true);
    }

    if( pVertexData && pVertexData->normals() )
    {
      pMesh->setUseNormalsFlag(true);
    }

    setVertexData(nbVertex, pVertexList, pVertexData);

    m_pEdgeData   = pEdgeData;
    m_pVertexData = pVertexData;

    for( OdInt32 k = 0; k < faceListSize; k++ )
    {
      OdUInt32 nPointsInContour = abs(pFaceList[k]);

      k++;

      for( OdUInt32 l = 0; l < nPointsInContour-1; l++, k++ )
      {
        m_contourPairStart.push_back(pFaceList[k]);
        m_contourPairEnd.push_back(pFaceList[k+1]);
      }

      m_contourPairStart.push_back(pFaceList[k - nPointsInContour+1]);
      m_contourPairEnd.push_back(pFaceList[k]);

      if( OdZero( pVertexList[pFaceList[k]].distanceTo(pVertexList[pFaceList[k - nPointsInContour+1]])) )
      {
        m_contourPairStart.push_back(pFaceList[k - nPointsInContour+1]);
        m_contourPairEnd.push_back(pFaceList[k - 1]);
      }
    }

    generateShellFacets(faceListSize, pFaceList, pFaceData, 4);
    m_pEdgeData = 0;

    m_contourPairStart.clear();
    m_contourPairEnd.clear();

    addElement( pMesh, false );
  }
  else
  {
    OdInt32 startPosFc(0);
    OdInt32 endPosFc(0);
    OdInt32 startPosFcDt(0);
    OdInt32 endPosFcDt(0);
    OdInt32 startPosEdgeDt(0);
    OdInt32 endPosEdgeDt(0);
    OdInt32 sumVtx(0);
    OdInt32 curNumVtx(0);
    OdInt32 curAtt = pFaceList[startPosFc];
    OdInt32 attempts = curAtt;

    /*should not be any hole*/
    for(OdInt32 i = 0; i < faceListSize; i++)
    {
      if (0 <= curAtt)
      {
        if (0 < attempts)
        {
          attempts--;
        }
        else
        {
          if (i + 1 < faceListSize)
          {
            curAtt = pFaceList[i + 1];
            attempts = curAtt;
          }
        }
      }
      else
      {
        return;
      }
    }

    curNumVtx = pFaceList[startPosFc];
    while (endPosFc < faceListSize)
    {
      if ((sumVtx + curNumVtx < 0x7FFF) &&
          (endPosFc + curNumVtx + 1 < faceListSize))
      {
        endPosFcDt++;
        sumVtx += curNumVtx;
        endPosFc += (curNumVtx + 1);
        curNumVtx = pFaceList[endPosFc];
      }
      else
      {
        if (endPosFc + curNumVtx + 1 == faceListSize)
        {
          endPosFcDt++;
          sumVtx += curNumVtx;
          endPosFc += (curNumVtx + 1);
        }

        OdInt32 j(0);
        OdGePoint3dArray pCurVertexList;
        OdInt32Array pMapVertexList;
        OdInt32Array pCurFaceList;
        OdGiVertexData pCurVertexData;
        OdGeVector3dArray pCurVerNormals;
        OdArray<OdCmEntityColor>pCurVerTrueColors;
        OdGiFaceData pCurFaceData;
        OdGiEdgeData pCurEdgeData;
        pMapVertexList.resize(nbVertex, -1);
        curAtt = pFaceList[startPosFc];
        attempts = curAtt;
        for (OdInt32 i = startPosFc; i < endPosFc; i++)
        {
          if (attempts == curAtt)
          {
            pCurFaceList.push_back(pFaceList[i]);
            attempts--;
          }
          else if (0 < attempts)
          {
            endPosEdgeDt++;
            if (pMapVertexList[pFaceList[i]] < 0)
            {
              pCurVertexList.push_back(pVertexList[pFaceList[i]]);
              pMapVertexList[pFaceList[i]] = j++;
              if (pVertexData)
              {
                if (pVertexData->normals())
                {
                  const OdGeVector3d* pNormals = pVertexData->normals();
                  pCurVerNormals.append(pNormals[pFaceList[i]]);
                }

                if (pVertexData->trueColors())
                {
                  const OdCmEntityColor*  pTrueColors = pVertexData->trueColors();
                  pCurVerTrueColors.append(pTrueColors[pFaceList[i]]);
                }
                pCurVertexData.setOrientationFlag(pVertexData->orientationFlag());
              }
            }

            pCurFaceList.push_back(pMapVertexList[pFaceList[i]]);
            attempts--;
          }
          else
          {
            endPosEdgeDt++;
            if (pMapVertexList[pFaceList[i]] < 0)
            {
              pCurVertexList.push_back(pVertexList[pFaceList[i]]);
              pMapVertexList[pFaceList[i]] = j++;

              if (pVertexData)
              {
                if (pVertexData->normals())
                {
                  const OdGeVector3d* pNormals = pVertexData->normals();
                  pCurVerNormals.append(pNormals[pFaceList[i]]);
                }

                if (pVertexData->trueColors())
                {
                  const OdCmEntityColor*  pTrueColors = pVertexData->trueColors();
                  pCurVerTrueColors.append(pTrueColors[pFaceList[i]]);
                }
                pCurVertexData.setOrientationFlag(pVertexData->orientationFlag());
              }
            }

            pCurFaceList.push_back(pMapVertexList[pFaceList[i]]);
            if (i + 1 < endPosFc)
            {
              curAtt = pFaceList[i + 1];
              attempts = curAtt;
            }
          }
        }

        pCurVertexData.setNormals(pCurVerNormals.asArrayPtr());
        pCurVertexData.setTrueColors(pCurVerTrueColors.asArrayPtr());

        OdUInt16Array pCurFcColors;
        OdCmEntityColorArray pCurFcTrueColor;
        OdDbStubPtrArray pCurFcLayerIds;
        OdGsMarkerArray pCurFcSelectionMarkers;
        OdUInt8Array pCurFcVisibilities;
        OdGeVector3dArray pCurFcNormals;
        OdDbStubPtrArray pCurFcMaterialIds;
        OdArray<OdGiMapper>pCurFcMappers;
        OdCmTransparencyArray pCurFcTransparency;
        if (pFaceData)
        {
#ifdef _DEBUG
          OdInt32 sumFace(0);
          curAtt = pCurFaceList[0];
          attempts = curAtt;
          for (OdUInt32 i = 0;  i < pCurFaceList.size(); i++)
          {
            if (attempts == curAtt)
            {
              sumFace++;
              attempts--;
            }
            else if (0 < attempts)
            {
              attempts--;
            }
            else
            {
              if (i + 1 < pCurFaceList.size())
              {
                curAtt = pCurFaceList[i + 1];
                attempts = curAtt;
              }
            }
          }
          ODA_ASSERT(sumFace == endPosFcDt - startPosFcDt);
#endif
          if (pFaceData->colors())
          {
            const OdUInt16* pColors = pFaceData->colors();
            for (OdInt32 i = startPosFcDt; i < endPosFcDt; i++)
            {
              pCurFcColors.append(pColors[i]);
            }
            pCurFaceData.setColors(pCurFcColors.asArrayPtr());
          }

          if (pFaceData->trueColors())
          {
            const OdCmEntityColor* pTrueColor = pFaceData->trueColors();
            for (OdInt32 i = startPosFcDt; i < endPosFcDt; i++)
            {
              pCurFcTrueColor.append(pTrueColor[i]);
            }
            pCurFaceData.setTrueColors(pCurFcTrueColor.asArrayPtr());
          }

          if (pFaceData->layerIds())
          {
            OdDbStub** pLayerIds = pFaceData->layerIds();
            for (OdInt32 i = startPosFcDt; i < endPosFcDt; i++)
            {
              pCurFcLayerIds.append(pLayerIds[i]);
            }
            pCurFaceData.setLayers(pCurFcLayerIds.asArrayPtr());
          }

          if (pFaceData->selectionMarkers())
          {
            const OdGsMarker* pSelectionMarkers = pFaceData->selectionMarkers();
            for (OdInt32 i = startPosFcDt; i < endPosFcDt; i++)
            {
              pCurFcSelectionMarkers.append(pSelectionMarkers[i]);
            }
            pCurFaceData.setSelectionMarkers(pCurFcSelectionMarkers.asArrayPtr());
          }

          if (pFaceData->visibility())
          {
            const OdUInt8* pVisibilities = pFaceData->visibility();
            for (OdInt32 i = startPosFcDt; i < endPosFcDt; i++)
            {
              pCurFcVisibilities.append(pVisibilities[i]);
            }
            pCurFaceData.setVisibility(pCurFcVisibilities.asArrayPtr());
          }

          if (pFaceData->normals())
          {
            const OdGeVector3d* pNormals = pFaceData->normals();
            for (OdInt32 i = startPosFcDt; i < endPosFcDt; i++)
            {
              pCurFcNormals.append(pNormals[i]);
            }
            pCurFaceData.setNormals(pCurFcNormals.asArrayPtr());
          }

          if (pFaceData->materials())
          {
            OdDbStub** pMaterialIds = pFaceData->materials();
            for (OdInt32 i = startPosFcDt; i < endPosFcDt; i++)
            {
              pCurFcMaterialIds.append(pMaterialIds[i]);
            }
            pCurFaceData.setMaterials(pCurFcMaterialIds.asArrayPtr());
          }

          if (pFaceData->mappers())
          {
            const OdGiMapper* pMappers = pFaceData->mappers();
            for (OdInt32 i = startPosFcDt; i < endPosFcDt; i++)
            {
              pCurFcMappers.append(pMappers[i]);
            }
            pCurFaceData.setMappers(pCurFcMappers.asArrayPtr());
          }

          if (pFaceData->transparency())
          {
            const OdCmTransparency* pTransparency = pFaceData->transparency();
            for (OdInt32 i = startPosFcDt; i < endPosFcDt; i++)
            {
              pCurFcTransparency.append(pTransparency[i]);
            }
            pCurFaceData.setTransparency(pCurFcTransparency.asArrayPtr());
          }
        }

        OdUInt16Array pCurEdColors;
        OdCmEntityColorArray pCurEdTrueColors;
        OdDbStubPtrArray pCurEdLayerIds;
        OdDbStubPtrArray pCurEdLinetypeIds;
        OdGsMarkerArray pCurEdSelectionMarkers;
        OdUInt8Array pCurEdVisibilities;
        if (pEdgeData)
        {

#ifdef _DEBUG
          OdInt32 sumEdge(0);
          curAtt = pCurFaceList[0];
          attempts = curAtt;
          for (OdUInt32 i = 0;  i < pCurFaceList.size(); i++)
          {
            if (attempts == curAtt)
            {
              attempts--;
            }
            else if (0 < attempts)
            {
              sumEdge++;
              attempts--;
            }
            else
            {
              sumEdge++;
              if (i + 1 < pCurFaceList.size())
              {
                curAtt = pCurFaceList[i + 1];
                attempts = curAtt;
              }
            }
          }
          ODA_ASSERT(sumEdge == endPosEdgeDt - startPosEdgeDt);
#endif
          if (pEdgeData->colors())
          {
            const OdUInt16* pColors = pEdgeData->colors();
            for (OdInt32 i = startPosEdgeDt; i < endPosEdgeDt; i++)
            {
              pCurEdColors.append(pColors[i]);
            }
            pCurEdgeData.setColors(pCurEdColors.asArrayPtr());
          }

          if (pEdgeData->trueColors())
          {
            const OdCmEntityColor* pTrueColors = pEdgeData->trueColors();
            for (OdInt32 i = startPosEdgeDt; i < endPosEdgeDt; i++)
            {
              pCurEdTrueColors.append(pTrueColors[i]);
            }
            pCurEdgeData.setTrueColors(pCurEdTrueColors.asArrayPtr());
          }

          if (pEdgeData->layerIds())
          {
            OdDbStub** pLayerIds = pEdgeData->layerIds();
            for (OdInt32 i = startPosEdgeDt; i < endPosEdgeDt; i++)
            {
              pCurEdLayerIds.append(pLayerIds[i]);
            }
            pCurEdgeData.setLayers(pCurEdLayerIds.asArrayPtr());
          }


          if (pEdgeData->linetypeIds())
          {
            OdDbStub** pLinetypeIds = pEdgeData->linetypeIds();
            for (OdInt32 i = startPosEdgeDt; i < endPosEdgeDt; i++)
            {
              pCurEdLinetypeIds.append(pLinetypeIds[i]);
            }
            pCurEdgeData.setLinetypes(pCurEdLinetypeIds.asArrayPtr());
          }

          if (pEdgeData->selectionMarkers())
          {
            const OdGsMarker* pSelectionMarkers = pEdgeData->selectionMarkers();
            for (OdInt32 i = startPosEdgeDt; i < endPosEdgeDt; i++)
            {
              pCurEdSelectionMarkers.append(pSelectionMarkers[i]);
            }
            pCurEdgeData.setSelectionMarkers(pCurEdSelectionMarkers.asArrayPtr());
          }

          if (pEdgeData->visibility())
          {
            const OdUInt8* pVisibilities = pEdgeData->visibility();
            for (OdInt32 i = startPosEdgeDt; i < endPosEdgeDt; i++)
            {
              pCurEdVisibilities.append(pVisibilities[i]);
            }
            pCurEdgeData.setVisibility(pCurEdVisibilities.asArrayPtr());
          }
        }

#ifdef _DEBUG
        int maxVXIndx = pCurVertexList.size();
        for (OdUInt32 ii = 0; ii < pCurFaceList.size();)
        {
          for (int f = 1; f <= pCurFaceList[ii]; ++f)
          {
            ODA_ASSERT(pCurFaceList[ii+f] < maxVXIndx);
          }
          ii += pCurFaceList[ii] + 1;
        }
#endif

        OdGiDrawDgnElementForExplode::shellProc(pCurVertexList.size(),
                                                pCurVertexList.asArrayPtr(),
                                                pCurFaceList.size(),
                                                pCurFaceList.asArrayPtr(),
                                                &pCurEdgeData,
                                                &pCurFaceData,
                                                &pCurVertexData);
        sumVtx = 0;
        startPosFc = endPosFc;
        startPosFcDt = endPosFcDt;
        startPosEdgeDt = endPosEdgeDt;
      }
    }
  }
}

void OdGiDrawDgnElementForExplode::meshProc(OdInt32 rows, OdInt32 columns,
														const OdGePoint3d* pVertexList, const OdGiEdgeData* pEdgeData,
														const OdGiFaceData* pFaceData, const OdGiVertexData* pVertexData )
{
  if(rows < 0x7FFF && columns < 0x7FFF)
  {
    OdInt32 i,j,k;

    OdUInt32Array uIndexArr;

    OdDgMeshFaceLoopsPtr pMesh = OdDgMeshFaceLoops::createObject();

    for(i=0, k=0; i<columns; i++)
    {
      for(j=0; j<rows; j++)
      {
        uIndexArr.push_back(pMesh->addPoint(pVertexList[k]));
        k++;
      }
    }

    OdDgMeshFaceLoops::VerticesArray meshFace;
    OdDgMeshFaceLoops::FacePoint     ptFace0;
    OdDgMeshFaceLoops::FacePoint     ptFace1;
    OdDgMeshFaceLoops::FacePoint     ptFace2;
    OdDgMeshFaceLoops::FacePoint     ptFace3;

    for( i=1; i<rows; i++ )
    {
      for( j=1; j<columns; j++ )
      {
        meshFace.clear();

        ptFace0.m_pointIndex = uIndexArr[columns * (i-1) + j-1 ];
        ptFace1.m_pointIndex = uIndexArr[columns * (i-1) + j   ];
        ptFace2.m_pointIndex = uIndexArr[columns *  i    + j   ];
        ptFace3.m_pointIndex = uIndexArr[columns *  i    + j-1 ];

        // Add edge colors, normals and visibility support

        meshFace.push_back(ptFace0);
        meshFace.push_back(ptFace1);
        meshFace.push_back(ptFace2);
        meshFace.push_back(ptFace3);
        meshFace.push_back(ptFace0);

        pMesh->addFace(meshFace);
      }
    }

    addElement(pMesh);
    
  }
  else
  {
    ODA_FAIL();
    // should be split
  }
}

void OdGiDrawDgnElementForExplode::xline(const OdGePoint3d& first, const OdGePoint3d& second)
{
  ODA_FAIL_ONCE();
  OdGiBaseVectorizer::xline( first, second );
}

void OdGiDrawDgnElementForExplode::ray(const OdGePoint3d& first, const OdGePoint3d& second)
{
  ODA_FAIL_ONCE();
  OdGiBaseVectorizer::ray( first, second );
}

void OdGiDrawDgnElementForExplode::pline(const OdGiPolyline& lwBuf, OdUInt32 fromIndex, OdUInt32 numSegs)
{
  OdGiBaseVectorizer::pline(lwBuf, fromIndex, numSegs);
}

void OdGiDrawDgnElementForExplode::text(const OdGePoint3d& position,
                                        const OdGeVector3d& normal,
                                        const OdGeVector3d& direction,
                                        double height,
                                        double width,
                                        double oblique,
                                        const OdString& smsg)
{
  OdString msg = smsg;
  msg.trimRight();
  if(msg.isEmpty())
  {
    return;
  }

  if( m_b3dElement )
  {
    addElement(makeText3d( position, normal, direction, height, width, oblique, msg ),true);
  }
  else
  {
    addElement(makeText2d( position, normal, direction, height, width, oblique, msg ), true);
  }
}

OdDgElementPtr OdGiDrawDgnElementForExplode::makeText2d( const OdGePoint3d& position, 
                                                           const OdGeVector3d& normal, 
                                                             const OdGeVector3d& direction,
                                                               double height, 
                                                                 double width, 
                                                                   double oblique, 
                                                                     const OdString& msg
                                                       )
{
  OdDgText2dPtr pText = OdDgText2d::createObject();

  pText->setOrigin( OdGePoint2d(position.x,position.y) );
  pText->setHeightMultiplier(height);
  pText->setLengthMultiplier(width);

  if( !OdZero(oblique) )
  {
    pText->setItalics(true);
    pText->setSlant(oblique);
  }

  pText->setRotation(OdGeMatrix3d::planeToWorld(normal).getCsXAxis().angleTo(direction, normal));
  pText->setText(msg);

  return pText;
}

OdDgElementPtr OdGiDrawDgnElementForExplode::makeText3d( const OdGePoint3d& position, 
                                                           const OdGeVector3d& normal, 
                                                             const OdGeVector3d& direction,
                                                               double height, 
                                                                 double width, 
                                                                   double oblique, 
                                                                     const OdString& msg
                                                       )
{
  OdDgText3dPtr pText = OdDgText3d::createObject();

  pText->setOrigin( position );
  pText->setHeightMultiplier(height);
  pText->setLengthMultiplier(width);

  if( !OdZero(oblique) )
  {
    pText->setItalics(true);
    pText->setSlant(oblique);
  }

  OdGeMatrix3d matSetNormal;
  matSetNormal = matSetNormal.planeToWorld( normal );

  OdGeVector3d vrXOrd = matSetNormal.getCsXAxis();

  double dRotAngle = vrXOrd.angleTo(direction, normal);

  OdGeMatrix3d matRotate;
  matRotate.setToRotation(dRotAngle, normal);

  OdGeQuaternion quatRotate;
  quatRotate.set( matRotate * matSetNormal );

  pText->setRotation(quatRotate);
  pText->setText(msg);

  double dThickness = thickness();

  if( !OdZero(dThickness) )
  {
    EText3D* pTextImpl = (EText3D*)(OdDgElementImpl::getImpl(pText));
    pTextImpl->setThickness(dThickness);
  }

  return pText;
}

void OdGiDrawDgnElementForExplode::text(const OdGePoint3d& position,
                                        const OdGeVector3d& normal,
                                        const OdGeVector3d& direction,
                                        const OdChar* pMsg,
                                        OdInt32 length,
                                        bool raw,
                                        const OdGiTextStyle* pStyle)
{
  OdString msg;

  if( length==-1 )
  {
    msg = pMsg;
  }
  else
  {
    msg = OdString(pMsg, length);
  }

  msg.trimRight();

  if( msg.isEmpty() )
  {
    return;
  }

  OdGiTextStyle defTextStyle;

  if( !pStyle )
  {
    pStyle = &defTextStyle;
  }

  if( m_b3dElement )
  {
    addElement(makeText3d( position, normal, direction, raw, pStyle, msg ),true);
  }
  else
  {
    addElement(makeText2d( position, normal, direction, raw, pStyle, msg ), true);
  }
}

OdDgElementPtr OdGiDrawDgnElementForExplode::makeText2d( const OdGePoint3d& position, 
                                                           const OdGeVector3d& normal, 
                                                             const OdGeVector3d& direction,
                                                               bool raw, 
                                                                 const OdGiTextStyle* pTextStyle, 
                                                                   OdString& msg 
                                                        )
{
  OdDgText2dPtr pText = OdDgText2d::createObject();

  pText->setOrigin(OdGePoint2d(position.x,position.y));
  pText->setRotation(OdGeMatrix3d::planeToWorld(normal).getCsXAxis().angleTo(direction, normal));
  pText->setHeightMultiplier(pTextStyle->textSize());
  pText->setLengthMultiplier( pTextStyle->textSize() * pTextStyle->xScale());

  if( pTextStyle->obliquingAngle() )
  {
    pText->setItalics(true);
    pText->setSlant(pTextStyle->obliquingAngle());
  }

  pText->setBackwardsFlag(pTextStyle->isBackward());
  pText->setUpsidedownFlag(pTextStyle->isUpsideDown());

  if(raw)
  {
    msg.replace(L"%%", L"%%%%");
  }

  pText->setText(msg);

  OdFontPtr pFont = pTextStyle->getFont();

  OdString strFontName;
  OdUInt32 uFontEntryId = 0;

  OdDgDatabase* pDb = getDatabase();

  if (pFont->flags() & OdFont::kFontRsc)
  {
    strFontName = pTextStyle->ttfdescriptor().fileName();

    if( pDb )
    {
      OdDgFontTablePtr pFontTable = pDb->getFontTable(OdDg::kForRead);

      if( !pFontTable.isNull() )
      {
        OdDgFontTableRecordPtr pFontRec = pFontTable->getFont(strFontName);

        if( !pFontRec.isNull() )
        {
          uFontEntryId = pFontRec->getNumber();
        }
      }
    }
  }
  else
  {
    strFontName = pFont->getFileName();
    strFontName.replace(L'\\', L'/');

    if( strFontName.reverseFind(L'/') >= 0 )
    {
      strFontName = strFontName.right(strFontName.getLength() - 1 - strFontName.reverseFind(L'/') );
    }

    strFontName = strFontName.makeUpper();

    if( strFontName.getLength() > 4 )
    {
      if( strFontName.right(4) == L".SHX" )
      {
        strFontName = strFontName.left( strFontName.getLength() - 4);
      }

      if( strFontName.right(4) == L".TTF" )
      {
        strFontName = strFontName.left( strFontName.getLength() - 4);
      }
    }

    if( pDb )
    {
      OdDgFontTablePtr pFontTable = pDb->getFontTable(OdDg::kForRead);

      if( !pFontTable.isNull() )
      {
        OdDgElementIteratorPtr pIter = pFontTable->createIterator();

        OdString strCurFont;

        for(; !pIter->done(); pIter->step() )
        {
          OdDgFontTableRecordPtr pFontRec = pIter->item().openObject( OdDg::kForRead );

          if( !pFontRec.isNull() )
          {
            strCurFont = pFontRec->getName();

            strCurFont.makeUpper();

            if( strCurFont == strFontName )
            {
              uFontEntryId = pFontRec->getNumber();
              break;
            }
          }
        }
      }
    }
  }

  pText->setFontEntryId(uFontEntryId);

  return pText;
}

OdDgElementPtr OdGiDrawDgnElementForExplode::makeText3d( const OdGePoint3d& position, 
                                                           const OdGeVector3d& normal, 
                                                             const OdGeVector3d& direction,
                                                               bool raw, 
                                                                 const OdGiTextStyle* pTextStyle, 
                                                                   OdString& msg 
                                                       )
{
  OdDgText3dPtr pText = OdDgText3d::createObject();

  pText->setOrigin(position);

  OdGeMatrix3d matSetNormal;
  matSetNormal = matSetNormal.planeToWorld( normal );

  OdGeVector3d vrXOrd = matSetNormal.getCsXAxis();

  double dRotAngle = vrXOrd.angleTo(direction, normal);

  OdGeMatrix3d matRotate;
  matRotate.setToRotation(dRotAngle, normal);

  OdGeQuaternion quatRotate;
  quatRotate.set( matRotate * matSetNormal );

  pText->setRotation(quatRotate);
  pText->setHeightMultiplier(pTextStyle->textSize());
  pText->setLengthMultiplier(pTextStyle->xScale()*pTextStyle->textSize());

  if( pTextStyle->obliquingAngle() )
  {
    pText->setItalics(true);
    pText->setSlant(pTextStyle->obliquingAngle());
  }

  pText->setBackwardsFlag(pTextStyle->isBackward());
  pText->setUpsidedownFlag(pTextStyle->isUpsideDown());

  pText->setText(msg);

  double dThickness = thickness();

  if( !OdZero(dThickness) )
  {
    EText3D* pTextImpl = (EText3D*)(OdDgElementImpl::getImpl(pText));
    pTextImpl->setThickness(dThickness);
  }

  OdFontPtr pFont = pTextStyle->getFont();

  OdString strFontName;
  OdUInt32 uFontEntryId = 0;

  OdDgDatabase* pDb = getDatabase();

  if (pFont->flags() & OdFont::kFontRsc)
  {
    strFontName = pTextStyle->ttfdescriptor().fileName();

    if( pDb )
    {
      OdDgFontTablePtr pFontTable = pDb->getFontTable(OdDg::kForRead);

      if( !pFontTable.isNull() )
      {
        OdDgFontTableRecordPtr pFontRec = pFontTable->getFont(strFontName);

        if( !pFontRec.isNull() )
        {
          uFontEntryId = pFontRec->getNumber();
        }
      }
    }
  }
  else
  {
    strFontName = pFont->getFileName();

    if( strFontName.isEmpty() )
    {
      strFontName = pTextStyle->ttfdescriptor().typeface();
    }

    strFontName.replace(L'\\', L'/');

    if( strFontName.reverseFind(L'/') >= 0 )
    {
      strFontName = strFontName.right(strFontName.getLength() - 1 - strFontName.reverseFind(L'/') );
    }

    strFontName = strFontName.makeUpper();

    if( strFontName.getLength() > 4 )
    {
      if( strFontName.right(4) == L".SHX" )
      {
        strFontName = strFontName.left( strFontName.getLength() - 4);
      }

      if( strFontName.right(4) == L".TTF" )
      {
        strFontName = strFontName.left( strFontName.getLength() - 4);
      }
    }

    if( pDb )
    {
      OdDgFontTablePtr pFontTable = pDb->getFontTable(OdDg::kForRead);

      if( !pFontTable.isNull() )
      {
        OdDgElementIteratorPtr pIter = pFontTable->createIterator();

        OdString strCurFont;

        for(; !pIter->done(); pIter->step() )
        {
          OdDgFontTableRecordPtr pFontRec = pIter->item().openObject( OdDg::kForRead );

          if( !pFontRec.isNull() )
          {
            strCurFont = pFontRec->getName();

            strCurFont.makeUpper();

            if( strCurFont == strFontName )
            {
              uFontEntryId = pFontRec->getNumber();
              break;
            }
          }
        }
      }
    }
  }

  pText->setFontEntryId(uFontEntryId);

  return pText;
}

void OdGiDrawDgnElementForExplode::addTransformedCopy(OdDgElementPtr pEnt)
{
  m_entityList.push_back(pEnt);
}

void OdGiDrawDgnElementForExplode::draw(const OdGiDrawable* pDrawable)
{
  draw1(pDrawable, true);
}

void OdGiDrawDgnElementForExplode::draw1(const OdGiDrawable* pDrawable, bool bCheckExplodeBeforeTransform)
{
  OdDgElementPtr pEnt = OdDgElement::cast(pDrawable);

  if( pEnt.get() && !m_bClipEnabled )
  {
    OdDgElementPtr pTransEnt;

    OdResult res = pEnt->getTransformedCopy(getModelToWorldTransform(), pTransEnt);

    if( res == eOk )
    {
      addTransformedCopy(pTransEnt);
    }
    else
    {
      if (!bCheckExplodeBeforeTransform || res == eExplodeBeforeTransform)
      {
        OdRxObjectPtrArray explodedEntities;
        pEnt->explode(explodedEntities);
        unsigned int i;

        for (i = 0; i < explodedEntities.size(); i++)
        {
          OdDgElementPtr pExEnt = OdDgElement::cast(explodedEntities[i]);

          if (!pExEnt.isNull())
          {
            draw(pExEnt);
          }
        }
      }
    }
  }
  else
  {
    pDrawable->worldDraw(this);
  }
}

OdGiRegenType OdGiDrawDgnElementForExplode::regenType() const
{
  return kOdGiForExplode;
}

void OdGiDrawDgnElementForExplode::getEntityArray(OdRxObjectPtrArray& entitySet) const
{
  int nOldSize = entitySet.size();
  entitySet.resize((unsigned int)(nOldSize + m_entityList.size()));
  OdList<OdDgElementPtr>::const_iterator ppEnt = m_entityList.begin(), ppEnd = m_entityList.end();
  OdRxObjectPtrArray::iterator ppEntDest = entitySet.begin() + nOldSize;
  while(ppEnt != ppEnd)
  {
    *ppEntDest = *ppEnt;
    ++ppEnt;
    ++ppEntDest;
  }
}

//----------------------------------------------------------
//
// OdGiDrawDgnElementForExplodeGeometry
//
//----------------------------------------------------------

void OdGiDrawDgnElementForExplodeGeometry::draw(const OdGiDrawable* pDrawable)
{
  draw1(pDrawable, false);
}

//----------------------------------------------------------
//
// OdGiDrawDgnTextForExplodeGeometry
//
//----------------------------------------------------------

void OdGiDrawDgnTextForExplodeGeometry::text( const OdGePoint3d& position,
                                                const OdGeVector3d& normal, 
                                                  const OdGeVector3d& direction,
                                                    double height, 
                                                      double width, 
                                                        double oblique, 
                                                          const OdString& msg
                                            )
{
  OdGiBaseVectorizer::text( position, normal, direction, height, width, oblique, msg );
}

void OdGiDrawDgnTextForExplodeGeometry::text( const OdGePoint3d& position,
                                                const OdGeVector3d& normal, 
                                                  const OdGeVector3d& direction,
                                                    const OdChar* msg, 
                                                      OdInt32 length, 
                                                        bool raw, 
                                                          const OdGiTextStyle* pTextStyle
                                            )
{
  OdGiBaseVectorizer::text( position, normal, direction, msg, length, raw, pTextStyle );
}

void OdGiDrawDgnTextForExplodeGeometry::shellProc(OdInt32 nbVertex,
                                                    const OdGePoint3d* pVertexList,
                                                      OdInt32 faceListSize,
                                                        const OdInt32* pFaceList,
                                                          const OdGiEdgeData* pEdgeData,// = NULL
                                                            const OdGiFaceData* pFaceData,// = NULL
                                                              const OdGiVertexData* pVertexData// = NULL
                                                  )
{
  if( m_b3dElement )
  {
    createPolygon3d( nbVertex, pVertexList, faceListSize, pFaceList );
  }
  else
  {
    createPolygon2d( nbVertex, pVertexList, faceListSize, pFaceList );
  }
}

void OdGiDrawDgnTextForExplodeGeometry::createPolygon2d( OdInt32 nbVertex,
                                                           const OdGePoint3d* pVertexList, 
                                                             OdInt32 faceListSize, 
                                                               const OdInt32* pFaceList 
                                                        )
{
  bool bCreatePolygonWithHole = false;

  OdInt32 nVertices = pFaceList[0];

  if( nVertices + 1 < faceListSize )
  {
    bCreatePolygonWithHole = true;
  }

  OdDgCellHeader2dPtr pPolygonWithHole;
  OdDgElementPtr      pElementToAdd;

  if( bCreatePolygonWithHole )
  {
    pPolygonWithHole = OdDgCellHeader2d::createObject();
    pElementToAdd    = pPolygonWithHole;
  }

  for( OdInt32 i = 0; i < faceListSize; i++ )
  {
    OdDgShape2dPtr pShape = OdDgShape2d::createObject();

    OdUInt32 nVertices = abs(pFaceList[i]);

    if( pFaceList[i] < 0 )
    {
      pShape->setHbitFlag(true);
    }
    else
    {
      pShape->setHbitFlag(false);
    }

    i++;

    for( OdUInt32 j = 0; j < nVertices; j++ )
    {
      pShape->addVertex( OdGePoint2d( pVertexList[pFaceList[i]].x, pVertexList[pFaceList[i]].y ));
      i++;
    }

    i--;

    setCurrTraitsTo(pShape.get());

    OdDgFillColorLinkagePtr pFillColor = OdDgFillColorLinkage::createObject();

    OdDgCmEntityColor textColor = trueColor();

    OdInt32 uColorIndex = 0;

    if( textColor.colorMethod() == OdDgCmEntityColor::kByColor )
    {
      OdDgDatabase* pDb = getDatabase();

      bool bFoundTrueColor = false;

      if( pDb )
      {
        uColorIndex = OdDgColorTable::getColorIndexByRGB( pDb, ODRGB( textColor.red(), textColor.green(), textColor.blue()));
      }
      else
      {
        uColorIndex = color();
      }
    }
    else if( textColor.colorMethod() == OdDgCmEntityColor::kByLayer )
    {
      uColorIndex = -1;
    }
    else if( textColor.colorMethod() == OdDgCmEntityColor::kByBlock )
    {
      uColorIndex = -2;
    }
    else
    {
      uColorIndex = color();
    }

    pFillColor->setColorIndex( uColorIndex );

    pShape->addLinkage( pFillColor->getPrimaryId(), pFillColor.get());

    if( bCreatePolygonWithHole )
    {
      pPolygonWithHole->addLinkage( pFillColor->getPrimaryId(), pFillColor.get());
      pPolygonWithHole->add( pShape );
    }
    else
    {
      pElementToAdd = pShape;
    }
  }

  addElement( pElementToAdd, true );
}

void OdGiDrawDgnTextForExplodeGeometry::createPolygon3d( OdInt32 nbVertex,
                                                           const OdGePoint3d* pVertexList, 
                                                             OdInt32 faceListSize, 
                                                               const OdInt32* pFaceList 
                                                        )
{
  bool bCreatePolygonWithHole = false;

  OdInt32 nVertices = pFaceList[0];

  if( nVertices + 1 < faceListSize )
  {
    bCreatePolygonWithHole = true;
  }

  OdDgCellHeader3dPtr pPolygonWithHole;
  OdDgElementPtr      pElementToAdd;

  if( bCreatePolygonWithHole )
  {
    pPolygonWithHole = OdDgCellHeader3d::createObject();
    pElementToAdd    = pPolygonWithHole;
  }

  for( OdInt32 i = 0; i < faceListSize; i++ )
  {
    OdDgShape3dPtr pShape = OdDgShape3d::createObject();

    OdUInt32 nVertices = abs(pFaceList[i]);

    if( pFaceList[i] < 0 )
    {
      pShape->setHbitFlag(true);
    }
    else
    {
      pShape->setHbitFlag(false);
    }

    i++;

    for( OdUInt32 j = 0; j < nVertices; j++ )
    {
      pShape->addVertex( pVertexList[pFaceList[i]] );
      i++;
    }

    i--;

    setCurrTraitsTo(pShape.get());

    OdDgFillColorLinkagePtr pFillColor = OdDgFillColorLinkage::createObject();

    OdDgCmEntityColor textColor = trueColor();

    OdInt32 uColorIndex = 0;

    if( textColor.colorMethod() == OdDgCmEntityColor::kByColor )
    {
      OdDgDatabase* pDb = getDatabase();

      bool bFoundTrueColor = false;

      if( pDb )
      {
        uColorIndex = OdDgColorTable::getColorIndexByRGB( pDb, ODRGB( textColor.red(), textColor.green(), textColor.blue()));
      }
      else
      {
        uColorIndex = color();
      }
    }
    else if( textColor.colorMethod() == OdDgCmEntityColor::kByLayer )
    {
      uColorIndex = -1;
    }
    else if( textColor.colorMethod() == OdDgCmEntityColor::kByBlock )
    {
      uColorIndex = -2;
    }
    else
    {
      uColorIndex = color();
    }

    pFillColor->setColorIndex( uColorIndex );

    pShape->addLinkage( pFillColor->getPrimaryId(), pFillColor.get());

    if( bCreatePolygonWithHole )
    {
      pPolygonWithHole->addLinkage( pFillColor->getPrimaryId(), pFillColor.get());
      pPolygonWithHole->add( pShape );
    }
    else
    {
      pElementToAdd = pShape;
    }
  }

  addElement( pElementToAdd, true );
}

void OdGiDrawDgnTextForExplodeGeometry::draw(const OdGiDrawable* pDrawable)
{
  draw1(pDrawable, false);
}


//----------------------------------------------------------
//
// OdGiDrawDimensionDgnElementForExplode
//
//----------------------------------------------------------

void OdGiDrawDimensionDgnElementForExplode::setTextNode( const OdDgElementPtr& pTextNode)
{
  m_pTextNode = pTextNode;
}

void OdGiDrawDimensionDgnElementForExplode::addExternalElement(OdDgElementPtr pEnt, bool needTransform )
{
  addElement( pEnt, needTransform );
}

void OdGiDrawDimensionDgnElementForExplode::addElement(OdDgElementPtr pEnt, bool needTransform )
{
  bool bAddToTextNode   = false;

  if( (pEnt->getElementType() == OdDgElement::kTypeText) && !m_pTextNode.isNull() )
  {
    bAddToTextNode =true;
  }


  if( !bAddToTextNode )
  {
    OdGiDrawDgnElementForExplodeGeometry::addElement( pEnt, needTransform );
  }
  else
  {
    if( needTransform )
    {
      OdGeMatrix3d matr = getModelToWorldTransform();

      if( matr != OdGeMatrix3d::kIdentity )
      {
        if( pEnt->transformBy(matr) != eOk )
        {
          OdDgElementPtr pOldEnt(pEnt.get());

          if( pOldEnt->getTransformedCopy(matr, pEnt) != eOk )
          {
            return;
          }
        }
      }
    }

    setCurrTraitsTo(pEnt);

    if( bAddToTextNode )
    {
      if( m_pTextNode->isKindOf(OdDgTextNode2d::desc()) )
      {
        OdDgTextNode2dPtr pTextNode2d = m_pTextNode;

        if( !pTextNode2d.isNull() )
        {
          pTextNode2d->add(pEnt);
        }
      }
      else
      {
        OdDgTextNode3dPtr pTextNode3d = m_pTextNode;

        if( !pTextNode3d.isNull() )
        {
          pTextNode3d->add(pEnt);
        }
      }
    }
  }
}
