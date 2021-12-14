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

#include "DgnExportPE.h"
#include "DgnExportImpl.h"
#include "DgnExportCommon.h"
#include "DgnExportContext.h"
#include "DgnExportGeneric.h"
#include <RxDispatchImpl.h>
#include <DbMline.h>
#include <DbMText.h>
#include <DgLevelTableRecord.h>
#include <DgTextStyleTableRecord.h>
#include <DgMaterialTableRecord.h>
#include <DbLinetypeTableRecord.h>
#include <DgArc.h>
#include <DgLine.h>
#include <DgLineString.h>
#include <DgShape.h>
#include <DgCellHeader.h>
#include <DgComplexString.h>
#include <DgComplexShape.h>
#include <DgEllipse.h>
#include <DgText.h>
#include <DbBlockTableRecord.h>
#include <CmTransparencyArray.h>
#include <DgFontTableRecord.h>
#include <DgRaster.h>
#include <DbLight.h>

using namespace TD_DGN_EXPORT;

ODRX_NO_CONS_DEFINE_MEMBERS(OdDgnExportPE, OdRxObject);

//----------------------------------------------------------------------------------------

namespace TD_DGN_EXPORT 
{

bool OdDgnExportPE::allowExportElement(OdDbEntity* e)
{
  bool bRet = !OdDgnExportContext::skipObjectExport(e->id());

  return bRet;
}

//----------------------------------------------------------------------------------------

void OdDgnExportPE::exportElement(OdDbEntity* e, OdDgElement* owner)
{
  if (allowExportElement(e))
  {
    try
    {
      subExportElement(e, owner);
    }
    catch(...)
    {
      ODA_ASSERT_ONCE(!"Error of entity conversion.");
    }
  }
}

//----------------------------------------------------------------------------------------

void OdDgnExportPE::subExportElement(OdDbEntity* e, OdDgElement* owner)
{
  OdStaticRxObject<OdExportDbObjectThroughDraw> drawObject;
  try
  {
    OdRxObjectPtrArray entitySet;
    drawObject.explode(e, owner->database(), e->database(), entitySet);

    for (unsigned int i = 0; i < entitySet.size(); ++i)
    {
      OdDgElementPtr elm = entitySet[i];
      addElementToDatabase(elm, owner);
      appendOdDgElementToOwner(owner, elm);
    }
  }
  catch (...) 
  {
    ODA_FAIL_ONCE();
  }
}

//----------------------------------------------------------------------------------------
//                    Implementation of OdExportDbObjectThroughDraw
//----------------------------------------------------------------------------------------

OdExportDbObjectThroughDraw::OdExportDbObjectThroughDraw()
{
  OdGiBaseVectorizer::m_flags |= (kDrawInvisibleEnts | kDrawLayerOff | kDrawLayerFrozen);
  setContext(this);
  OdGiGeometrySimplifier::setDrawContext(OdGiBaseVectorizer::drawContext());
  m_pModelToEyeProc->setDrawContext(OdGiBaseVectorizer::drawContext());
  output().setDestGeometry(*this);
  m_b3dElement = true;
  m_bClipEnabled = false;
  m_dLSTransform = 1.0;
  m_uRegenType = kOdGiForExplode;
}

//----------------------------------------------------------------------------------------

OdResult OdExportDbObjectThroughDraw::explode(const OdDbEntity* pEnt, OdDgDatabase* pDb, OdDbDatabase* db, OdRxObjectPtrArray& entitySet)
{
  OdUInt32 oldArraySize = entitySet.size();
  setDatabase(db);
  m_pDgDb = pDb;
  m_b3dElement = OdDgnExportContext::is3d();
  try
  {
    m_nDrawableAttributes = setAttributes(pEnt); // use virtual method of OdGiBaseVectorizer

    worldDrawEntity(pEnt);
    getEntityArray(entitySet);

    return (entitySet.size() > oldArraySize) ? eOk : eCannotExplodeEntity;
  }
  catch (...)
  {
    return eCannotExplodeEntity;
  }
}

//----------------------------------------------------------------------------------------

void OdExportDbObjectThroughDraw::worldDrawEntity(const OdGiDrawable* pEnt)
{
  prepareToDraw(pEnt);

  if(!pEnt->worldDraw(this))
    pEnt->viewportDraw(this);

  restoreAfterDraw();
}

//----------------------------------------------------------------------------------------

void OdExportDbObjectThroughDraw::prepareToDraw(const OdGiDrawable* pDrawable)
{
  m_uStoredRegenType = m_uRegenType;

  if (pDrawable->isKindOf(OdDbMText::desc()) || 
      pDrawable->isKindOf(OdDbMline::desc())
     )
    m_uRegenType = kOdGiStandardDisplay;
}

//----------------------------------------------------------------------------------------

void OdExportDbObjectThroughDraw::restoreAfterDraw()
{
  m_uRegenType = m_uStoredRegenType;
}

//----------------------------------------------------------------------------------------

void OdExportDbObjectThroughDraw::setCurrTraitsTo(OdDgElement* pDgElm)
{
  OdDgGraphicsElement* pElm;
  
  if( pDgElm->isKindOf(OdDgGraphicsElement::desc()) )
    pElm = (OdDgGraphicsElement*)(pDgElm);

  if (!pElm)
    return;

  // Level

  OdDbObjectId  idLayer = OdDbObjectId(effectiveTraits().layer());
  OdDgElementId idLevel = OdDgnExportContext::getElementId(idLayer);

  if( idLevel.isNull() || OdDgLevelTableRecord::cast(idLevel.openObject()).isNull() )
    pElm->setLevelEntryId(0);
  else
    pElm->setLevelId(idLevel);

  // Color

  OdCmEntityColor curColor = effectiveTraits().trueColor();
  pElm->setColorIndex( getDgnEntityColorIndex(curColor, getDgDatabase() ) );

  // Line Type

  OdDbObjectId  idLineType = OdDbObjectId(effectiveTraits().lineType());
  pElm->setLineStyleEntryId(getDgnLineStyleEntryId(idLineType));

  // line style scale

  setDgnLineStyleScale(pElm, effectiveTraits().lineTypeScale());

  // line style modifiers

  if( effectiveTraits().lineStyleModifiers() && (effectiveTraits().lineStyleModifiers()->m_uFlags != 0) )
  {
    OdDgLineStyleDataElementPEPtr pLineStylePE = OdDgLineStyleDataElementPE::cast(pElm);

    if (!pLineStylePE.isNull())
      pLineStylePE->setDgnLineStyleModifiers(pElm, *effectiveTraits().lineStyleModifiers() );
  }

  // line weight

  pElm->setLineWeight(odDgnExportLineweightMapping(effectiveTraits().lineWeight()));

  // material

  if (material())
  {
    OdDbObjectId materialId = OdDbObjectId(effectiveTraits().material());

    if( !materialId.isNull() )
      setDgnMaterial(pElm, getDatabase(), materialId, idLayer);
  }

  // visibility

  pElm->setInvisibleFlag((m_nDrawableAttributes & OdGiDrawable::kDrawableIsInvisible) ? true: false);

  // transparency

  pElm->setTransparency(effectiveTraits().transparency());
}

//----------------------------------------------------------------------------------------

void OdExportDbObjectThroughDraw::addElement(OdDgElementPtr pEnt, bool needTransform)
{
  ODA_ASSERT(!pEnt.isNull());

  if (pEnt.isNull())
    return;

  if (needTransform)
  {
    OdGeMatrix3d matr = getModelToWorldTransform();

    if (matr != OdGeMatrix3d::kIdentity)
    {
      if (pEnt->transformBy(matr) != eOk)
      {
        OdDgElementPtr pOldEnt(pEnt.get());

        if (pOldEnt->getTransformedCopy(matr, pEnt) != eOk)
          return;
      }
    }
  }

  if (pEnt->getElementType() == OdDgElement::kTypeCellHeader)
  {
    OdDgElementIteratorPtr pIter;

    if (pEnt->isKindOf(OdDgCellHeader2d::desc()))
    {
      OdDgCellHeader2dPtr pCell = pEnt;
      pIter = pCell->createIterator();
    }
    else
    {
      OdDgCellHeader3dPtr pCell = pEnt;
      pIter = pCell->createIterator();
    }

    setCurrTraitsTo(pEnt);

    for (; !pIter->done(); pIter->step())
    {
      OdDgGraphicsElementPtr pGrElm = pIter->item().openObject(OdDg::kForWrite);
      setCurrTraitsTo(pGrElm);

      if (pGrElm->getHbitFlag())
        pGrElm->setLineStyleEntryId(0);
    }
  }
  else
    setCurrTraitsTo(pEnt);

  m_entityList.push_back(pEnt);
}

//----------------------------------------------------------------------------------------

void OdExportDbObjectThroughDraw::pushClipBoundary(OdGiClipBoundary* pBoundary)
{
  OdUInt32 uOldFlags = m_flags;
  OdGiBaseVectorizer::m_flags &= ~kDrawInvisibleEnts;

  OdGiBaseVectorizer::pushClipBoundary(pBoundary);

  OdGiBaseVectorizer::m_flags = uOldFlags;

  m_bClipEnabled = true;
}

//----------------------------------------------------------------------------------------

void OdExportDbObjectThroughDraw::pushClipBoundary(OdGiClipBoundary* pBoundary, OdGiAbstractClipBoundary* pAbsBoundary)
{
  OdGiBaseVectorizer::pushClipBoundary(pBoundary, pAbsBoundary);

  m_bClipEnabled = true;
}

//----------------------------------------------------------------------------------------

void OdExportDbObjectThroughDraw::popClipBoundary()
{
  OdGiBaseVectorizer::popClipBoundary();

  m_bClipEnabled = false;
}

//----------------------------------------------------------------------------------------

OdDgElementPtr OdExportDbObjectThroughDraw::makeCircle(const OdGePoint3d& center, double radius, const OdGeVector3d& normal)
{
  if( m_b3dElement )
    return makeCircle3d(center, radius, normal);
  else
    return makeCircle2d(center, radius, normal);
}

//----------------------------------------------------------------------------------------

OdDgElementPtr OdExportDbObjectThroughDraw::makeCircle2d(const OdGePoint3d& center, double radius, const OdGeVector3d& normal)
{
  if (normal.isCodirectionalTo(OdGeVector3d::kZAxis))
  {
    OdDgEllipse2dPtr pCircle = OdDgEllipse2d::createObject();
    pCircle->setDatabaseDefaults(getDgDatabase());
    pCircle->setOrigin(OdGePoint2d(center.x, center.y));
    pCircle->setPrimaryAxis(radius);
    pCircle->setSecondaryAxis(radius);

    if (fillType() == kOdGiFillAlways)
    {
      OdCmEntityColor curColor = effectiveTraits().trueColor();
      pCircle->setFillColor(getDgnEntityColorIndex(curColor, getDgDatabase()));
    }

    return pCircle;
  }
  else
  {
    OdDgElementPtr pRet = makeCircle3d(center, radius, normal);

    if (!pRet.isNull())
    {
      OdDg2d3dConversionGraphicsElementPEPtr pConvertTo2dPE = OdDg2d3dConversionGraphicsElementPE::cast(pRet);

      if (!pConvertTo2dPE.isNull())
      {
        pRet = pConvertTo2dPE->convertTo2d(pRet);
      }
    }

    return pRet;
  }
}

//----------------------------------------------------------------------------------------

OdDgElementPtr OdExportDbObjectThroughDraw::makeCircle3d(const OdGePoint3d& center, double radius, const OdGeVector3d& normal)
{
  OdDgEllipse3dPtr pCircle = OdDgEllipse3d::createObject();
  pCircle->setDatabaseDefaults(getDgDatabase());
  pCircle->setOrigin(center);
  pCircle->setPrimaryAxis(radius);
  pCircle->setSecondaryAxis(radius);

  OdGeVector3d vrZAxis = OdGeVector3d::kZAxis;
  OdGeVector3d vrRotation = OdGeVector3d::kXAxis;

  OdGeMatrix3d matRotation;
  matRotation = matRotation.setToPlaneToWorld(normal);

  OdGeQuaternion quatRotation;
  quatRotation.set(matRotation);

  pCircle->setRotation(quatRotation);

  if (fillType() == kOdGiFillAlways)
  {
    OdCmEntityColor curColor = effectiveTraits().trueColor();
    pCircle->setFillColor(getDgnEntityColorIndex(curColor, getDgDatabase()));
  }

  return pCircle;
}

//----------------------------------------------------------------------------------------

void OdExportDbObjectThroughDraw::circleProc(const OdGePoint3d& center,
                                               double radius, 
                                                 const OdGeVector3d& normal,
                                                   const OdGeVector3d* pExtrusion)
{
  if (m_bClipEnabled)
  {
    OdGiGeometrySimplifier::circleProc(center, radius, normal, pExtrusion);
  }
  else
  {
    addElement(makeCircle(center, radius, normal));
  }
}

//----------------------------------------------------------------------------------------

void OdExportDbObjectThroughDraw::circleProc( const OdGePoint3d& p1,
                                                const OdGePoint3d& p2, 
                                                  const OdGePoint3d& p3,
                                                    const OdGeVector3d* pExtrusion)
{
  if (m_bClipEnabled)
  {
    OdGiGeometrySimplifier::circleProc(p1, p2, p3, pExtrusion);
  }
  else
  {
    OdGeCircArc3d arc(p1, p2, p3);
    addElement(makeCircle(arc.center(), arc.radius(), arc.normal()));
  }
}

//----------------------------------------------------------------------------------------

void OdExportDbObjectThroughDraw::circularArcProc(const OdGePoint3d& center,
                                                    double radius,
                                                      const OdGeVector3d& normal,
                                                        const OdGeVector3d& startVector,
                                                          double sweepAngle,
                                                            OdGiArcType arcType,
                                                              const OdGeVector3d* pExtrusion)
{
  if (pExtrusion || m_bClipEnabled)
  {
    OdGiGeometrySimplifier::circularArcProc(center, radius, normal, startVector, sweepAngle, arcType, pExtrusion);
  }
  else if (m_b3dElement)
  {
    addElement(makeArc3d(center, radius, normal, startVector, sweepAngle, arcType));
  }
  else
  {
    addElement(makeArc2d(center, radius, normal, startVector, sweepAngle, arcType));
  }
}

//----------------------------------------------------------------------------------------

OdDgElementPtr OdExportDbObjectThroughDraw::makeArc2d(const OdGePoint3d& center,
                                                        double radius,
                                                          const OdGeVector3d& normal,
                                                            const OdGeVector3d& startVector,
                                                              double sweepAngle,
                                                                OdGiArcType arcType
                                                      )
{
  if( normal.isCodirectionalTo(OdGeVector3d::kZAxis) )
  {
    OdDgArc2dPtr pArc = OdDgArc2d::createObject();

    pArc->setDatabaseDefaults(getDgDatabase());
    pArc->setOrigin(OdGePoint2d(center.x, center.y));
    pArc->setPrimaryAxis(radius);
    pArc->setSecondaryAxis(radius);
    pArc->setSweepAngle(sweepAngle);
    pArc->setStartAngle(OdGeVector3d::kXAxis.angleTo(startVector, OdGeVector3d::kZAxis));

    if (arcType > kOdGiArcSimple)
    {
      OdDgComplexShapePtr pCplxShape = OdDgComplexShape::createObject();
      pCplxShape->setDatabaseDefaults(getDgDatabase());

      OdGePoint2d ptArcStart, ptArcEnd;
      OdDgLineString2dPtr pLineString = OdDgLineString2d::createObject();
      pLineString->setDatabaseDefaults(getDgDatabase());
      pArc->getStartPoint(ptArcStart);
      pArc->getEndPoint(ptArcEnd);
      pLineString->addVertex(ptArcEnd);
      pLineString->addVertex(OdGePoint2d(center.x, center.y));
      pLineString->addVertex(ptArcStart);

      setCurrTraitsTo(pArc);
      setCurrTraitsTo(pLineString);

      pCplxShape->add(pArc);
      pCplxShape->add(pLineString);

      if (fillType() == kOdGiFillAlways)
      {
        OdCmEntityColor curColor = effectiveTraits().trueColor();
        pCplxShape->setFillColor(getDgnEntityColorIndex(curColor, getDgDatabase()));
      }

      return pCplxShape;
    }

    return pArc;
  }
  else
  {
    OdDgElementPtr pRet = makeArc3d(center, radius, normal, startVector, sweepAngle, arcType);

    if( !pRet.isNull() )
    {
      OdDg2d3dConversionGraphicsElementPEPtr pConvertTo2dPE = OdDg2d3dConversionGraphicsElementPE::cast(pRet);

      if( !pConvertTo2dPE.isNull() )
      {
        pRet = pConvertTo2dPE->convertTo2d(pRet);
      }
    }

    return pRet;
  }
}

//----------------------------------------------------------------------------------------

OdDgElementPtr OdExportDbObjectThroughDraw::makeArc3d(  const OdGePoint3d& center,
                                                          double radius,
                                                            const OdGeVector3d& normal,
                                                              const OdGeVector3d& startVector,
                                                                double sweepAngle,
                                                                  OdGiArcType arcType
                                                      )
{
  OdDgArc3dPtr pArc = OdDgArc3d::createObject();

  pArc->setDatabaseDefaults(getDgDatabase());
  pArc->setOrigin(center);
  pArc->setPrimaryAxis(radius);
  pArc->setSecondaryAxis(radius);
  pArc->setSweepAngle(sweepAngle);

  OdGeMatrix3d matRotation;

  matRotation = matRotation.setToPlaneToWorld(normal);

  OdGeQuaternion quatRotation;
  quatRotation.set(matRotation);

  pArc->setRotation(quatRotation);

  OdGeVector3d axisX = OdGeVector3d::kXAxis;
  axisX.transformBy(matRotation);

  double angl = axisX.angleTo(startVector, normal);

  pArc->setStartAngle(angl);

  if( arcType > kOdGiArcSimple )
  {
    OdDgComplexShapePtr pCplxShape = OdDgComplexShape::createObject();
    pCplxShape->setDatabaseDefaults(getDgDatabase());

    OdGePoint3d ptArcStart, ptArcEnd;
    OdDgLineString3dPtr pLineString = OdDgLineString3d::createObject();
    pLineString->setDatabaseDefaults(getDgDatabase());
    pArc->getStartPoint(ptArcStart);
    pArc->getEndPoint(ptArcEnd);
    pLineString->addVertex(ptArcEnd);
    pLineString->addVertex(center);
    pLineString->addVertex(ptArcStart);

    setCurrTraitsTo(pArc);
    setCurrTraitsTo(pLineString);

    pCplxShape->add(pArc);
    pCplxShape->add(pLineString);

    if( fillType() == kOdGiFillAlways )
    {
      OdCmEntityColor curColor = effectiveTraits().trueColor();
      pCplxShape->setFillColor(getDgnEntityColorIndex(curColor, getDgDatabase()));
    }

    return pCplxShape;
  }

  double dThickness = thickness();

  if (!OdZero(dThickness))
    pArc->setThickness(dThickness);

  return pArc;
}

//----------------------------------------------------------------------------------------

void OdExportDbObjectThroughDraw::circularArcProc(const OdGePoint3d& start,
                                                    const OdGePoint3d& point,
                                                      const OdGePoint3d& end,
                                                        OdGiArcType arcType,
                                                          const OdGeVector3d* pExtrusion)
{
  OdGeCircArc3d arc(start, point, end);

  circularArcProc(arc.center(), arc.radius(), arc.normal(), start - arc.center(),
    arc.endAng() - arc.startAng(), arcType, pExtrusion);
}

//----------------------------------------------------------------------------------------

OdDgElementPtr OdExportDbObjectThroughDraw::makePolyline(OdInt32 nbPoints,
                                                           const OdGePoint3d* pVertexList,
                                                             const OdGeVector3d* pNormal)
{
  if (nbPoints < 2)
  {
    ODA_FAIL();
    return OdDgElementPtr();
  }
  else
  {
    if( m_b3dElement )
      return makePolyline3d(nbPoints, pVertexList, pNormal);
    else
      return makePolyline2d(nbPoints, pVertexList, pNormal);
  }
}

//----------------------------------------------------------------------------------------

OdDgElementPtr OdExportDbObjectThroughDraw::makePolyline2d(OdInt32 nbPoints,
                                                             const OdGePoint3d* pVertexList,
                                                               const OdGeVector3d* pNormal
                                                          )
{
  OdGePoint2dArray arrVertices2d;

  for (OdInt32 i = 0; i < nbPoints; i++)
  {
    OdGePoint2d ptVertex2d(pVertexList[i].x, pVertexList[i].y);

    arrVertices2d.push_back(ptVertex2d);
  }

  if (nbPoints == 2)
  {

    OdDgLine2dPtr pLine2d = OdDgLine2d::createObject();
    pLine2d->setDatabaseDefaults(getDgDatabase());
    pLine2d->setStartPoint(arrVertices2d[0]);
    pLine2d->setEndPoint(arrVertices2d[1]);

    return pLine2d;
  }
  else
  {
    OdDgLineString2dPtr pLineString2d = OdDgLineString2d::createObject();
    pLineString2d->setDatabaseDefaults(getDgDatabase());

    for (OdInt32 f = 0; f < nbPoints; ++f)
    {
      pLineString2d->addVertex(arrVertices2d[f]);
    }

    return pLineString2d;
  }

  return OdDgElementPtr();
}

//----------------------------------------------------------------------------------------

OdDgElementPtr OdExportDbObjectThroughDraw::makePolyline3d( OdInt32 nbPoints,
                                                              const OdGePoint3d* pVertexList,
                                                                const OdGeVector3d* pNormal
                                                          )
{
  if (nbPoints == 2)
  {

    OdDgLine3dPtr pLine3d = OdDgLine3d::createObject();
    pLine3d->setDatabaseDefaults(getDgDatabase());
    pLine3d->setStartPoint(pVertexList[0]);
    pLine3d->setEndPoint(pVertexList[1]);

    double dThickness = thickness();

    if (!OdZero(dThickness))
      pLine3d->setThickness(dThickness);

    return pLine3d;
  }
  else
  {
    OdDgLineString3dPtr pLineString3d = OdDgLineString3d::createObject();
    pLineString3d->setDatabaseDefaults(getDgDatabase());

    for (OdInt32 f = 0; f < nbPoints; ++f)
    {
      pLineString3d->addVertex(pVertexList[f]);
    }

    double dThickness = thickness();

    if (!OdZero(dThickness))
      pLineString3d->setThickness(dThickness);

    return pLineString3d;
  }

  return OdDgElementPtr();
}

//----------------------------------------------------------------------------------------

void OdExportDbObjectThroughDraw::polyline( OdInt32 nbPoints,
                                              const OdGePoint3d* pVertexList,
                                                const OdGeVector3d* pNormal,
                                                  OdGsMarker lBaseSubEntMarker)
{
  if (nbPoints < 2)
    return;

  if (m_bClipEnabled)
  {
    m_bClipEnabled = false;

    OdGiBaseVectorizer::polyline(nbPoints, pVertexList, pNormal, lBaseSubEntMarker);

    m_bClipEnabled = true;
  }
  else
  {
    addElement(makePolyline(nbPoints, pVertexList, pNormal), true);
  }
}

//----------------------------------------------------------------------------------------

void OdExportDbObjectThroughDraw::polylineOut(OdInt32 nbPoints, const OdGePoint3d* pVertexList)
{
  addElement(makePolyline(nbPoints, pVertexList, 0));
}

//----------------------------------------------------------------------------------------

OdDgElementPtr OdExportDbObjectThroughDraw::makeGroupedHole2d( const OdArray<OdGePoint3dArray>& arrFaces,
                                                                 const OdArray<bool>& arrHoleStatus)
{
  OdDgElementPtr pRet;

  OdArray<OdGePoint3dArray> arrSetFaces;
  OdArray<OdGePoint3dArray> arrSetHoles;

  OdUInt32 i = 0;
  for (i = 0; i < arrHoleStatus.size(); i++)
  {
    if (arrHoleStatus[i])
      arrSetHoles.push_back(arrFaces[i]);
    else
      arrSetFaces.push_back(arrFaces[i]);
  }

  OdDgCellHeader2dPtr pCellHeader = OdDgCellHeader2d::createObject();

  for (i = 0; i < arrSetFaces.size(); i++)
  {
    OdDgShape2dPtr pShape = makePolygon2d(arrSetFaces[i].size(), arrSetFaces[i].asArrayPtr());
    pShape->setDatabaseDefaults(getDgDatabase());
    pCellHeader->add(pShape);
    pShape->setHoleFlag(false);
  }

  for (i = 0; i < arrSetHoles.size(); i++)
  {
    OdDgShape2dPtr pShape = makePolygon2d(arrSetHoles[i].size(), arrSetHoles[i].asArrayPtr());
    pShape->setDatabaseDefaults(getDgDatabase());
    pCellHeader->add(pShape);
    pShape->setHoleFlag(true);
  }

  pRet = pCellHeader;

  return pRet;
}

//----------------------------------------------------------------------------------------

OdDgElementPtr OdExportDbObjectThroughDraw::makeGroupedHole3d( const OdArray<OdGePoint3dArray>& arrFaces,
                                                                 const OdArray<bool>& arrHoleStatus)
{
  OdDgElementPtr pRet;

  OdArray<OdGePoint3dArray> arrSetFaces;
  OdArray<OdGePoint3dArray> arrSetHoles;

  OdUInt32 i = 0;
  for (i = 0; i < arrHoleStatus.size(); i++)
  {
    if (arrHoleStatus[i])
      arrSetHoles.push_back(arrFaces[i]);
    else
      arrSetFaces.push_back(arrFaces[i]);
  }

  OdDgCellHeader3dPtr pCellHeader = OdDgCellHeader3d::createObject();

  for (i = 0; i < arrSetFaces.size(); i++)
  {
    OdDgShape3dPtr pShape = makePolygon3d(arrSetFaces[i].size(), arrSetFaces[i].asArrayPtr());
    pShape->setDatabaseDefaults(getDgDatabase());
    pCellHeader->add(pShape);
    pShape->setHoleFlag(false);
  }

  for (i = 0; i < arrSetHoles.size(); i++)
  {
    OdDgShape3dPtr pShape = makePolygon3d(arrSetHoles[i].size(), arrSetHoles[i].asArrayPtr());
    pShape->setDatabaseDefaults(getDgDatabase());
    pCellHeader->add(pShape);
    pShape->setHoleFlag(true);
  }

  pRet = pCellHeader;

  return pRet;
}

//----------------------------------------------------------------------------------------

OdDgElementPtr OdExportDbObjectThroughDraw::makePolygonOrGroupedHole(OdInt32 nbPoints,
                                                                       const OdGePoint3d* pVertexList,
                                                                         OdInt32 faceListSize,
                                                                           const OdInt32* pFaceList)
{
  OdDgElementPtr pRet;

  if ((nbPoints > 2) && (faceListSize > 3))
  {
    OdArray<OdGePoint3dArray> arrFaces;
    OdArray<bool>             arrHoleStatus;

    for (OdInt32 i = 0; i < faceListSize; i++)
    {
      OdInt32 nIndexesInLoop = abs(pFaceList[i]);
      arrHoleStatus.push_back(pFaceList[i] < 0);
      i++;

      OdGePoint3dArray arrFacePts;

      for (OdInt32 j = 0; j < nIndexesInLoop; j++, i++)
      {
        arrFacePts.push_back(pVertexList[pFaceList[i]]);
      }

      i--;

      if (arrFacePts.size() > 2)
        arrFaces.push_back(arrFacePts);
    }

    if (arrFaces.size() == 1)
    {
      if (m_b3dElement)
        pRet = makePolygon3d(arrFaces[0].size(), arrFaces[0].asArrayPtr());
      else
        pRet = makePolygon2d(arrFaces[0].size(), arrFaces[0].asArrayPtr());
    }
    else
    {
      if (m_b3dElement)
        pRet = makeGroupedHole3d(arrFaces, arrHoleStatus);
      else
        pRet = makeGroupedHole2d(arrFaces, arrHoleStatus);
    }
  }

  return pRet;
}

//----------------------------------------------------------------------------------------

OdDgElementPtr OdExportDbObjectThroughDraw::makePolygon(OdInt32 nbPoints, const OdGePoint3d* pVertexList)
{
  if (nbPoints < 3)
  {
    ODA_FAIL();
    return OdDgElementPtr();
  }
  else
  {
    if (m_b3dElement)
    {
      return makePolygon3d(nbPoints, pVertexList);
    }
    else
    {
      return makePolygon2d(nbPoints, pVertexList);
    }
  }
}

//----------------------------------------------------------------------------------------

OdDgElementPtr OdExportDbObjectThroughDraw::makePolygon2d(OdInt32 nbPoints, const OdGePoint3d* pVertexList)
{
  OdDgShape2dPtr pShape = OdDgShape2d::createObject();
  pShape->setDatabaseDefaults(getDgDatabase());

  for (OdInt32 i = 0; i < nbPoints; i++)
  {
    pShape->addVertex(OdGePoint2d(pVertexList[i].x, pVertexList[i].y));
  }

  if( fillType() == kOdGiFillAlways )
  {
    OdCmEntityColor curColor = effectiveTraits().trueColor();
    pShape->setFillColor(getDgnEntityColorIndex(curColor, getDgDatabase()));
  }

  return pShape;
}

//----------------------------------------------------------------------------------------

OdDgElementPtr OdExportDbObjectThroughDraw::makePolygon3d(OdInt32 nbPoints, const OdGePoint3d* pVertexList)
{
  OdDgShape3dPtr pShape = OdDgShape3d::createObject();
  pShape->setDatabaseDefaults(getDgDatabase());

  for (OdInt32 i = 0; i < nbPoints; i++)
  {
    pShape->addVertex(pVertexList[i]);
  }

  if( fillType() == kOdGiFillAlways )
  {
    OdCmEntityColor curColor = effectiveTraits().trueColor();
    pShape->setFillColor(getDgnEntityColorIndex(curColor, getDgDatabase()));
  }

  double dThickness = thickness();

  if (!OdZero(dThickness))
    pShape->setThickness(dThickness);

  return pShape;
}

//----------------------------------------------------------------------------------------

void OdExportDbObjectThroughDraw::polygon(OdInt32 nbPoints, const OdGePoint3d* pVertexList)
{
  if (m_bClipEnabled)
  {
    m_bClipEnabled = false;

    OdGiBaseVectorizer::polygon(nbPoints, pVertexList);

    m_bClipEnabled = true;
  }
  else
  {
    addElement(makePolygon(nbPoints, pVertexList), true);
  }
}

//----------------------------------------------------------------------------------------

void OdExportDbObjectThroughDraw::facetOut( const OdInt32* pFaceList,
                                              const OdInt32* pEdgeIndices,
                                                const OdGeVector3d* /*pNormal*/)
{
  OdDgMeshFaceLoops::VerticesArray meshFace;

  const OdUInt8* vis = 0;

  if (m_pEdgeData)
  {
    vis = m_pEdgeData->visibility();
  }

  int n = *pFaceList++;

  for (int i = 0; i < n; ++i)
  {
    OdDgMeshFaceLoops::FacePoint ptFace;
    ptFace.m_pointIndex = pFaceList[i];
    ptFace.m_impliesVisibleEdge = true;

    OdInt32 nEdgeIndex = pEdgeIndices[i];

    if (nEdgeIndex >= 0 && vis && vis[nEdgeIndex] == kOdGiInvisible)
    {
      ptFace.m_impliesVisibleEdge = false;
    }
    else
    {
      bool bAdditionalEdge = true;

      OdUInt32 uStartEdge = pFaceList[i];
      OdUInt32 uEndEdge = (i == (n - 1)) ? pFaceList[0] : pFaceList[i + 1];

      OdUInt64 uStartEndEdgeKey = uStartEdge;
      uStartEndEdgeKey = (OdUInt64)(uStartEndEdgeKey << 32) + uEndEdge;
      OdUInt64 uEndStartEdgeKey = uEndEdge;
      uEndStartEdgeKey = (OdUInt64)(uEndStartEdgeKey << 32) + uStartEdge;

      std::set<OdUInt64>::iterator pStartEndIter = m_contourPairStartEnd.find(uStartEndEdgeKey);
      std::set<OdUInt64>::iterator pEndStartIter = m_contourPairStartEnd.find(uEndStartEdgeKey);

      if ((pStartEndIter != m_contourPairStartEnd.end()) || (pEndStartIter != m_contourPairStartEnd.end()))
        bAdditionalEdge = false;

      ptFace.m_impliesVisibleEdge = !bAdditionalEdge;
    }

    if (nEdgeIndex >= 0 && m_pEdgeData && m_pEdgeData->colors())
    {
      ptFace.m_colorIndex = m_pEdgeData->colors()[nEdgeIndex];
    }
    else if (nEdgeIndex >= 0 && m_pEdgeData && m_pEdgeData->trueColors())
    {
      ptFace.m_dColorRGB[0] = m_pEdgeData->trueColors()[nEdgeIndex].red() / 255.0;
      ptFace.m_dColorRGB[1] = m_pEdgeData->trueColors()[nEdgeIndex].green() / 255.0;
      ptFace.m_dColorRGB[2] = m_pEdgeData->trueColors()[nEdgeIndex].blue() / 255.0;
    }
    else if (m_pVertexData && m_pVertexData->trueColors())
    {
      ptFace.m_dColorRGB[0] = m_pVertexData->trueColors()[ptFace.m_pointIndex].red() / 255.0;
      ptFace.m_dColorRGB[1] = m_pVertexData->trueColors()[ptFace.m_pointIndex].green() / 255.0;
      ptFace.m_dColorRGB[2] = m_pVertexData->trueColors()[ptFace.m_pointIndex].blue() / 255.0;
    }
    else if (m_pMesh->getUseColorTableIndexesFlag())
    {
      ptFace.m_colorIndex = subEntityTraits().color();
    }
    else if (m_pMesh->getUseDoubleColorsFlag() )
    {
      OdCmEntityColor entColor    = subEntityTraits().trueColor();
      ODCOLORREF      curRGBColor = OdCmEntityColorToRGB(entColor, subEntityTraits().layer());
      ptFace.m_dColorRGB[0] = ODGETRED(curRGBColor) / 255.0;
      ptFace.m_dColorRGB[1] = ODGETGREEN(curRGBColor) / 255.0;
      ptFace.m_dColorRGB[2] = ODGETBLUE(curRGBColor) / 255.0;
    }

    if (m_pVertexData && m_pVertexData->normals())
    {
      ptFace.m_vrNormal = m_pVertexData->normals()[ptFace.m_pointIndex];
    }

    meshFace.push_back(ptFace);
  }

  m_pMesh->addFace(meshFace);
}

//----------------------------------------------------------------------------------------

void OdExportDbObjectThroughDraw::shellProc(OdInt32 nbVertex,
                                              const OdGePoint3d* pVertexList,
                                                OdInt32 faceListSize,
                                                  const OdInt32* pFaceList,
                                                    const OdGiEdgeData* pEdgeData,
                                                      const OdGiFaceData* pFaceData,
                                                        const OdGiVertexData* pVertexData)
{
  if( isPlanar(pVertexList, nbVertex) )
  {
    OdDgElementPtr pShell = makePolygonOrGroupedHole(nbVertex, pVertexList, faceListSize, pFaceList);

    if (!pShell.isNull())
    {
      addElement(pShell);
      return;
    }
  }

  if (faceListSize <= 5 && pFaceData == 0 && pEdgeData == 0)
  {
    OdGePoint3dArray arrShapePts;

    OdInt32 nPts = abs(pFaceList[0]);

    if (nPts > 2)
    {
      for( OdInt32 i = 0; i < nPts; i++ )
        arrShapePts.push_back(pVertexList[pFaceList[i + 1]]);

      if( isPlanar(arrShapePts, 1e-6) )
        return addElement(makePolygon(arrShapePts.size(), arrShapePts.asArrayPtr()), true);
    }
  }
  else if( nbVertex < 0x7FFF )
  {
    // create mesh face loops

    OdDgMeshFaceLoopsPtr pMesh = OdDgMeshFaceLoops::createObject();
    pMesh->setDatabaseDefaults(getDgDatabase());

    for (OdInt32 j = 0; j < nbVertex; j++)
      pMesh->addPoint(pVertexList[j]);

    m_pMesh = pMesh;

    if( (pFaceData && pFaceData->colors()) || (pEdgeData && pEdgeData->colors()) )
    {
      pMesh->setUseColorTableIndexesFlag(true);
    }
    else if ((pFaceData && pFaceData->trueColors()) || 
             (pVertexData && pVertexData->trueColors()) || 
             (pFaceData && pFaceData->trueColors()))
    {
      pMesh->setUseDoubleColorsFlag(true);
    }

    if (pVertexData && pVertexData->normals())
    {
      pMesh->setUseNormalsFlag(true);
    }

    setVertexData(nbVertex, pVertexList, pVertexData);

    m_pEdgeData = pEdgeData;
    m_pVertexData = pVertexData;

    for (OdInt32 k = 0; k < faceListSize; k++)
    {
      OdUInt32 nPointsInContour = abs(pFaceList[k]);

      k++;

      for (OdUInt32 l = 0; l < nPointsInContour - 1; l++, k++)
      {
        OdUInt64 uStartEndKey1 = pFaceList[k];
        uStartEndKey1 = (OdUInt64)(uStartEndKey1 << 32) + pFaceList[k + 1];
        m_contourPairStartEnd.insert(uStartEndKey1);
      }

      OdUInt64 uStartEndKey2 = pFaceList[k - nPointsInContour + 1];
      uStartEndKey2 = (OdUInt64)(uStartEndKey2 << 32) + pFaceList[k];
      m_contourPairStartEnd.insert(uStartEndKey2);

      if (OdZero(pVertexList[pFaceList[k]].distanceTo(pVertexList[pFaceList[k - nPointsInContour + 1]])))
      {
        OdUInt64 uStartEndKey3 = pFaceList[k - nPointsInContour + 1];
        uStartEndKey3 = (OdUInt64)(uStartEndKey3 << 32) + pFaceList[k - 1];
        m_contourPairStartEnd.insert(uStartEndKey3);
      }
    }

    generateShellFacets(faceListSize, pFaceList, pFaceData, 4);
    m_pEdgeData = 0;

    m_contourPairStartEnd.clear();

    addElement(pMesh, false);
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
    for (OdInt32 i = 0; i < faceListSize; i++)
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
          for (OdUInt32 i = 0; i < pCurFaceList.size(); i++)
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
          for (OdUInt32 i = 0; i < pCurFaceList.size(); i++)
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
            ODA_ASSERT(pCurFaceList[ii + f] < maxVXIndx);
          }
          ii += pCurFaceList[ii] + 1;
        }
#endif

        OdExportDbObjectThroughDraw::shellProc(pCurVertexList.size(),
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

//----------------------------------------------------------------------------------------

void OdExportDbObjectThroughDraw::meshProc( OdInt32 rows, 
                                              OdInt32 columns,
                                                const OdGePoint3d* pVertexList, 
                                                  const OdGiEdgeData* pEdgeData,
                                                    const OdGiFaceData* pFaceData, 
                                                      const OdGiVertexData* pVertexData)
{
  if (rows < 0x7FFF && columns < 0x7FFF)
  {
    OdInt32 i, j, k;

    OdUInt32Array uIndexArr;

    OdDgMeshFaceLoopsPtr pMesh = OdDgMeshFaceLoops::createObject();
    pMesh->setDatabaseDefaults(getDgDatabase());

    for (i = 0, k = 0; i < columns; i++)
    {
      for (j = 0; j < rows; j++)
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

    for (i = 1; i < rows; i++)
    {
      for (j = 1; j < columns; j++)
      {
        meshFace.clear();

        ptFace0.m_pointIndex = uIndexArr[columns * (i - 1) + j - 1];
        ptFace1.m_pointIndex = uIndexArr[columns * (i - 1) + j];
        ptFace2.m_pointIndex = uIndexArr[columns *  i + j];
        ptFace3.m_pointIndex = uIndexArr[columns *  i + j - 1];

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

//----------------------------------------------------------------------------------------

void OdExportDbObjectThroughDraw::xline(const OdGePoint3d& first, const OdGePoint3d& second)
{
  OdDgGraphicsElementPtr pDgPolyline;

  if (m_b3dElement)
  {
    OdDgLine3dPtr pLine3d = OdDgLine3d::createObject();
    pLine3d->setDatabaseDefaults(getDgDatabase());
    pLine3d->setStartPoint(first);
    pLine3d->setEndPoint(second);
    pLine3d->setInfiniteLineType(OdDg::kInfiniteLine);
    pDgPolyline = pLine3d;
  }
  else
  {
    OdDgLine2dPtr pLine2d = OdDgLine2d::createObject();
    pLine2d->setDatabaseDefaults(getDgDatabase());
    pLine2d->setStartPoint(first.convert2d());
    pLine2d->setEndPoint(second.convert2d());
    pLine2d->setInfiniteLineType(OdDg::kInfiniteLine);
    pDgPolyline = pLine2d;
  }

  if( !pDgPolyline.isNull() )
    addElement(pDgPolyline);
}

//----------------------------------------------------------------------------------------

void OdExportDbObjectThroughDraw::ray(const OdGePoint3d& first, const OdGePoint3d& second)
{
  OdDgGraphicsElementPtr pDgPolyline;

  if( m_b3dElement )
  {
    OdDgLine3dPtr pLine3d = OdDgLine3d::createObject();
    pLine3d->setDatabaseDefaults(getDgDatabase());
    pLine3d->setStartPoint(first);
    pLine3d->setEndPoint(second);
    pLine3d->setInfiniteLineType(OdDg::kFromEndPoint);
    pDgPolyline = pLine3d;
  }
  else
  {
    OdDgLine2dPtr pLine2d = OdDgLine2d::createObject();
    pLine2d->setDatabaseDefaults(getDgDatabase());
    pLine2d->setStartPoint(first.convert2d());
    pLine2d->setEndPoint(second.convert2d());
    pLine2d->setInfiniteLineType(OdDg::kFromEndPoint);
    pDgPolyline = pLine2d;
  }

  if( !pDgPolyline.isNull() )
    addElement(pDgPolyline);
}

//----------------------------------------------------------------------------------------

void OdExportDbObjectThroughDraw::pline(const OdGiPolyline& lwBuf, OdUInt32 fromIndex, OdUInt32 numSegs)
{
  OdGiBaseVectorizer::pline(lwBuf, fromIndex, numSegs);
}

//----------------------------------------------------------------------------------------

void OdExportDbObjectThroughDraw::text( const OdGePoint3d& position,
                                          const OdGeVector3d& normal,
                                            const OdGeVector3d& direction,
                                              double height,
                                                double width,
                                                  double oblique,
                                                    const OdString& smsg)
{
  OdString msg = smsg;
  msg.trimRight();
  if (msg.isEmpty())
  {
    return;
  }

  if (m_b3dElement)
  {
    addElement(makeText3d(position, normal, direction, height, width, oblique, msg), true);
  }
  else
  {
    addElement(makeText2d(position, normal, direction, height, width, oblique, msg), true);
  }
}

//----------------------------------------------------------------------------------------

OdDgElementPtr OdExportDbObjectThroughDraw::makeText2d( const OdGePoint3d& position,
                                                          const OdGeVector3d& normal,
                                                            const OdGeVector3d& direction,
                                                              double height,
                                                                double width,
                                                                  double oblique,
                                                                    const OdString& msg
                                                      )
{
  OdDgText2dPtr pText = OdDgText2d::createObject();
  pText->setDatabaseDefaults(getDgDatabase());

  pText->setOrigin(OdGePoint2d(position.x, position.y));
  pText->setHeightMultiplier(height);
  pText->setLengthMultiplier(width);

  if (!OdZero(oblique))
  {
    pText->setItalics(true);
    pText->setSlant(oblique);
  }

  pText->setRotation(OdGeMatrix3d::planeToWorld(normal).getCsXAxis().angleTo(direction, normal));
  pText->setText(msg);

  return pText;
}

//----------------------------------------------------------------------------------------

OdDgElementPtr OdExportDbObjectThroughDraw::makeText3d( const OdGePoint3d& position,
                                                          const OdGeVector3d& normal,
                                                            const OdGeVector3d& direction,
                                                              double height,
                                                                double width,
                                                                  double oblique,
                                                                    const OdString& msg
                                                      )
{
  OdDgText3dPtr pText = OdDgText3d::createObject();
  pText->setDatabaseDefaults(getDgDatabase());

  OdGiTextStyle curTextStyle;
  context()->getDefaultTextStyle(curTextStyle);

  pText->setOrigin(position);
  pText->setHeightMultiplier(height);
  pText->setLengthMultiplier(height * curTextStyle.xScale() );

  if (!OdZero(oblique))
  {
    pText->setItalics(true);
    pText->setSlant(oblique);
  }

  OdGeMatrix3d matSetNormal;
  matSetNormal = matSetNormal.planeToWorld(normal);

  OdGeVector3d vrXOrd = matSetNormal.getCsXAxis();

  double dRotAngle = vrXOrd.angleTo(direction, normal);

  OdGeMatrix3d matRotate;
  matRotate.setToRotation(dRotAngle, normal);

  OdGeQuaternion quatRotate;
  quatRotate.set(matRotate * matSetNormal);

  pText->setRotation(quatRotate);
  pText->setText(msg);

  double dThickness = thickness();

  if (!OdZero(dThickness))
    pText->setThickness(dThickness);

  return pText;
}

//----------------------------------------------------------------------------------------

void OdExportDbObjectThroughDraw::text( const OdGePoint3d& position,
                                          const OdGeVector3d& normal,
                                            const OdGeVector3d& direction,
                                              const OdChar* pMsg,
                                                OdInt32 length,
                                                  bool raw,
                                                    const OdGiTextStyle* pStyle)
{
  OdString msg;

  if (length == -1)
  {
    msg = pMsg;
  }
  else
  {
    msg = OdString(pMsg, length);
  }

  msg.trimRight();

  if (msg.isEmpty())
  {
    return;
  }

  OdGiTextStyle defTextStyle;

  if (!pStyle)
  {
    pStyle = &defTextStyle;
  }

  if (m_b3dElement)
  {
    addElement(makeText3d(position, normal, direction, raw, pStyle, msg), true);
  }
  else
  {
    addElement(makeText2d(position, normal, direction, raw, pStyle, msg), true);
  }
}

//----------------------------------------------------------------------------------------

OdDgElementPtr OdExportDbObjectThroughDraw::makeText2d( const OdGePoint3d& position,
                                                          const OdGeVector3d& normal,
                                                            const OdGeVector3d& direction,
                                                              bool raw,
                                                                const OdGiTextStyle* pTextStyle,
                                                                  OdString& msg
                                                      )
{
  OdDgText2dPtr pText = OdDgText2d::createObject();
  pText->setDatabaseDefaults(getDgDatabase());

  pText->setOrigin(OdGePoint2d(position.x, position.y));
  pText->setRotation(OdGeMatrix3d::planeToWorld(normal).getCsXAxis().angleTo(direction, normal));
  pText->setHeightMultiplier(pTextStyle->textSize());
  pText->setLengthMultiplier(pTextStyle->textSize() * pTextStyle->xScale());

  if (pTextStyle->obliquingAngle())
  {
    pText->setItalics(true);
    pText->setSlant(pTextStyle->obliquingAngle());
  }

  pText->setBackwardsFlag(pTextStyle->isBackward());
  pText->setUpsidedownFlag(pTextStyle->isUpsideDown());

  if (raw)
  {
    msg.replace(L"%%", L"%%%%");
  }

  pText->setText(msg);

  OdFontPtr pFont = pTextStyle->getFont();

  OdString strFontName;
  OdUInt32 uFontEntryId = 0;

  OdDgDatabase* pDb = getDgDatabase();

  if (pFont->flags() & OdFont::kFontRsc)
  {
    strFontName = pTextStyle->ttfdescriptor().fileName();

    if (pDb)
    {
      OdDgFontTablePtr pFontTable = pDb->getFontTable(OdDg::kForRead);

      if (!pFontTable.isNull())
      {
        OdDgFontTableRecordPtr pFontRec = pFontTable->getFont(strFontName);

        if (!pFontRec.isNull())
        {
          uFontEntryId = pFontRec->getNumber();
        }
      }
    }
  }
  else
  {
    strFontName = pFont->getFileName();

    if (strFontName.isEmpty())
    {
      strFontName = pTextStyle->ttfdescriptor().typeface();

      if (strFontName.isEmpty())
        strFontName = pTextStyle->ttfdescriptor().fileName();
    }

    strFontName.replace(L'\\', L'/');

    if (strFontName.reverseFind(L'/') >= 0)
    {
      strFontName = strFontName.right(strFontName.getLength() - 1 - strFontName.reverseFind(L'/'));
    }

    strFontName = strFontName.makeUpper();

    if (strFontName.getLength() > 4)
    {
      if (strFontName.right(4) == L".SHX")
      {
        strFontName = strFontName.left(strFontName.getLength() - 4);
      }

      if( pFont->flags() & OdFont::kTrueType )
      {
        OdTtfDescriptor fontDesc;
        pFont->getDescriptor(fontDesc);

        if (!fontDesc.fileName().isEmpty())
          strFontName = fontDesc.fileName();
      }

      if (strFontName.right(4) == L".TTF")
      {
        strFontName = strFontName.left(strFontName.getLength() - 4);
      }
    }

    uFontEntryId = getDgnFontEntryId(strFontName, pDb, !(pFont->flags() & OdFont::kTrueType) );
  }

  pText->setFontEntryId(uFontEntryId);

  return pText;
}

//----------------------------------------------------------------------------------------

OdDgElementPtr OdExportDbObjectThroughDraw::makeText3d( const OdGePoint3d& position,
                                                          const OdGeVector3d& normal,
                                                            const OdGeVector3d& direction,
                                                              bool raw,
                                                                const OdGiTextStyle* pTextStyle,
                                                                  OdString& msg
                                                      )
{
  OdDgText3dPtr pText = OdDgText3d::createObject();
  pText->setDatabaseDefaults(getDgDatabase());

  pText->setOrigin(position);

  OdGeMatrix3d matSetNormal;
  matSetNormal = matSetNormal.planeToWorld(normal);

  OdGeVector3d vrXOrd = matSetNormal.getCsXAxis();

  double dRotAngle = vrXOrd.angleTo(direction, normal);

  OdGeMatrix3d matRotate;
  matRotate.setToRotation(dRotAngle, normal);

  OdGeQuaternion quatRotate;
  quatRotate.set(matRotate * matSetNormal);

  pText->setRotation(quatRotate);
  pText->setHeightMultiplier(pTextStyle->textSize());
  pText->setLengthMultiplier(pTextStyle->xScale()*pTextStyle->textSize());

  if (pTextStyle->obliquingAngle())
  {
    pText->setItalics(true);
    pText->setSlant(pTextStyle->obliquingAngle());
  }

  pText->setBackwardsFlag(pTextStyle->isBackward());
  pText->setUpsidedownFlag(pTextStyle->isUpsideDown());

  pText->setText(msg);

  double dThickness = thickness();

  if (!OdZero(dThickness))
    pText->setThickness(dThickness);

  OdFontPtr pFont = pTextStyle->getFont();

  OdString strFontName;
  OdUInt32 uFontEntryId = 0;

  OdDgDatabase* pDb = getDgDatabase();

  if (pFont->flags() & OdFont::kFontRsc)
  {
    strFontName = pTextStyle->ttfdescriptor().fileName();

    if (pDb)
    {
      OdDgFontTablePtr pFontTable = pDb->getFontTable(OdDg::kForRead);

      if (!pFontTable.isNull())
      {
        OdDgFontTableRecordPtr pFontRec = pFontTable->getFont(strFontName);

        if (!pFontRec.isNull())
        {
          uFontEntryId = pFontRec->getNumber();
        }
      }
    }
  }
  else
  {
    strFontName = pFont->getFileName();

    if (strFontName.isEmpty())
    {
      strFontName = pTextStyle->ttfdescriptor().typeface();

      if (strFontName.isEmpty())
        strFontName = pTextStyle->ttfdescriptor().fileName();
    }

    strFontName.replace(L'\\', L'/');

    if (strFontName.reverseFind(L'/') >= 0)
    {
      strFontName = strFontName.right(strFontName.getLength() - 1 - strFontName.reverseFind(L'/'));
    }

    strFontName = strFontName.makeUpper();

    if (strFontName.getLength() > 4)
    {
      if (strFontName.right(4) == L".SHX")
      {
        strFontName = strFontName.left(strFontName.getLength() - 4);
      }

      if (pFont->flags() & OdFont::kTrueType)
      {
        OdTtfDescriptor fontDesc;
        pFont->getDescriptor(fontDesc);

        if (!fontDesc.fileName().isEmpty())
          strFontName = fontDesc.fileName();
      }

      if (strFontName.right(4) == L".TTF")
      {
        strFontName = strFontName.left(strFontName.getLength() - 4);
      }
    }

    uFontEntryId = getDgnFontEntryId(strFontName, pDb, !(pFont->flags() & OdFont::kTrueType) );
  }

  pText->setFontEntryId(uFontEntryId);

  return pText;
}

//----------------------------------------------------------------------------------------

void OdExportDbObjectThroughDraw::addTransformedCopy(OdDgElementPtr pEnt)
{
  m_entityList.push_back(pEnt);
}

//----------------------------------------------------------------------------------------

bool OdExportDbObjectThroughDraw::doDraw(OdUInt32 drawableFlags, const OdGiDrawable* pDrawable)
{
  draw1(pDrawable, true);

  return true;
}

//----------------------------------------------------------------------------------------

void OdExportDbObjectThroughDraw::draw1(const OdGiDrawable* pDrawable, bool bCheckExplodeBeforeTransform)
{
  OdDbEntityPtr pEnt = OdDbEntity::cast(pDrawable);

  if( pEnt.get() && !m_bClipEnabled )
  {
    bool bOldNonDbroState = OdDgnExportContext::isKeepNonDbro();
    try
    {
      OdDgnExportContext::setKeepNonDbro(true);
      OdDgCellHeader3dPtr pContainer = OdDgCellHeader3d::createObject();
      pContainer->setDatabaseDefaults(getDgDatabase());

      OdDgnExportPE* pExportPE = (OdDgnExportPE*)pEnt->queryX(OdDgnExportPE::desc());

      if( pExportPE )
      {
        pExportPE->exportElement(pEnt, pContainer);

        OdDgElementIteratorPtr pIter = pContainer->createIterator();

        for(; !pIter->done(); pIter->step())
        {
          OdDgElementPtr pItem = pIter->item().openObject(OdDg::kForRead);

          if( !pItem.isNull() )
          {
            OdDgElementPtr pItemClone = pItem->clone();

            if( !pItemClone.isNull() )
              m_entityList.push_back(pItemClone);
          }
        }
      }
      else
      {
        worldDrawEntity(pEnt);
      }
    }
    catch (...)
    {
      OdDgnExportContext::setKeepNonDbro(bOldNonDbroState);
    }

    OdDgnExportContext::setKeepNonDbro(bOldNonDbroState);
  }
  else
  {
    bool bRestoreByBlockProperties = false;

    OdUInt32 uColorByBlock, uLineStyleByBlock, uLineWeightByBlock;

    if( pDrawable->isKindOf(OdDbBlockTableRecord::desc()) )
    {
      bRestoreByBlockProperties = true;
      uColorByBlock = OdDgnExportContext::getColorByBlockIndex();
      uLineStyleByBlock = OdDgnExportContext::getLineStyleByBlockIndex();
      uLineWeightByBlock = OdDgnExportContext::getLineWeightByBlockIndex();

      OdCmEntityColor curColor = effectiveTraits().trueColor();
      OdDbObjectId  idLineType = OdDbObjectId(effectiveTraits().lineType());

      OdDgnExportContext::setColorByBlockIndex(getDgnEntityColorIndex(curColor, getDgDatabase()));
      OdDgnExportContext::setLineStyleByBlockIndex(getDgnLineStyleEntryId(idLineType));
      OdDgnExportContext::setLineWeightByBlockIndex(odDgnExportLineweightMapping(effectiveTraits().lineWeight()));
    }

    worldDrawEntity(pDrawable);

    if( bRestoreByBlockProperties )
    {
      OdDgnExportContext::setColorByBlockIndex(uColorByBlock);
      OdDgnExportContext::setLineStyleByBlockIndex(uLineStyleByBlock);
      OdDgnExportContext::setLineWeightByBlockIndex(uLineWeightByBlock);
    }
  }
}
//----------------------------------------------------------------------------------------

OdGiRegenType OdExportDbObjectThroughDraw::regenType() const
{
  return m_uRegenType;
}

//----------------------------------------------------------------------------------------

void OdExportDbObjectThroughDraw::getEntityArray(OdRxObjectPtrArray& entitySet) const
{
  int nOldSize = entitySet.size();
  entitySet.resize((unsigned int)(nOldSize + m_entityList.size()));
  OdList<OdDgGraphicsElementPtr>::const_iterator ppEnt = m_entityList.begin(), ppEnd = m_entityList.end();
  OdRxObjectPtrArray::iterator ppEntDest = entitySet.begin() + nOldSize;
  while (ppEnt != ppEnd)
  {
    *ppEntDest = *ppEnt;
    ++ppEnt;
    ++ppEntDest;
  }
}

//----------------------------------------------------------------------------------------

void OdExportDbObjectThroughDraw::rasterImageProc(const OdGePoint3d& origin,
                                                    const OdGeVector3d& u,
                                                      const OdGeVector3d& v,
                                                        const OdGiRasterImage* pImage,
                                                          const OdGePoint2d* uvBoundary,
                                                            OdUInt32 numBoundPts,
                                                              bool transparency,
                                                                double brightness,
                                                                  double contrast,
                                                                    double fade
                                                 )
{
  if (!pImage)
    return;

  OdDgGraphicsElementPtr pGrImage;

  if( m_b3dElement )
  {
    OdDgRasterHeader3dPtr pRaster = OdDgRasterHeader3d::createObject();
    pRaster->setDatabaseDefaults(getDgDatabase());

    if( numBoundPts > 0 )
    {
      OdGePoint2dArray arrBoundary;

      for (OdUInt32 i = 0; i < numBoundPts; i++)
        arrBoundary.push_back(uvBoundary[i]);

      pRaster->setBoundary(arrBoundary);
    }
    pRaster->setTransparentFlag(transparency);
    pRaster->initFromGiRasterImage(pImage);

    pRaster->setOrigin(origin);
    pRaster->setScale(u.length());
    pRaster->setLowerJustifiedFlag(true);

    pGrImage = pRaster;
  }
  else
  {
    OdDgRasterHeader2dPtr pRaster = OdDgRasterHeader2d::createObject();
    pRaster->setDatabaseDefaults(getDgDatabase());

    if (numBoundPts > 0)
    {
      OdGePoint2dArray arrBoundary;

      for (OdUInt32 i = 0; i < numBoundPts; i++)
        arrBoundary.push_back(uvBoundary[i]);

      pRaster->setBoundary(arrBoundary);
    }
    pRaster->setTransparentFlag(transparency);
    pRaster->initFromGiRasterImage(pImage);

    pRaster->setOrigin(origin);
    pRaster->setScale(u.length());
    pRaster->setLowerJustifiedFlag(true);

    pGrImage = pRaster;
  }

  if (!pGrImage.isNull())
    addElement(pGrImage);
}

//----------------------------------------------------------------------------------------

}
