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
#include "DgnExportCurve.h"
#include "DgnExportCommon.h"
#include "DgnExportContext.h"
#include <DgLine.h>
#include <DbLine.h>
#include <DbEllipse.h>
#include <DbCircle.h>
#include <DgEllipse.h>
#include <DbArc.h>
#include <DgArc.h>
#include <DgComplexString.h>
#include <DgComplexShape.h>
#include <DbPolyline.h>
#include <DgLineString.h>
#include <Db3dPolyline.h>
#include <Db2dPolyline.h>
#include <Db3dPolylineVertex.h>
#include <DbSpline.h>
#include <DgBSplineCurve.h>
#include <DbRay.h>
#include <DbXline.h>
#include <DbMline.h>
#include <DbMlineStyle.h>
#include <DgMultiline.h>
#include <DgSharedCellReference.h>
#include <DbPoint.h>
#include "Ge/GeCircArc3d.h"
#include "Ge/GeNurbCurve3d.h"
#include "Ge/GeLineSeg3d.h"

namespace TD_DGN_EXPORT {

//---------------------------------------------------------------------------------------------------

double getMaxLineWidth(OdDgGraphicsElement* pDgCurve)
{
  double dRet = -1;

  if (!pDgCurve)
    return dRet;

  if( pDgCurve->isKindOf(OdDgArc2d::desc()))
  {
    OdDgArc2dPtr pArc = pDgCurve;
    dRet = pArc->getPrimaryAxis() * 2.0;
  }
  else if (pDgCurve->isKindOf(OdDgArc3d::desc()))
  {
    OdDgArc3dPtr pArc = pDgCurve;
    dRet = pArc->getPrimaryAxis() * 2.0;
  }
  else if (pDgCurve->isKindOf(OdDgComplexCurve::desc()))
  {
    OdDgComplexCurvePtr pCplxCurve = pDgCurve;
    OdDgElementIteratorPtr pIter = pCplxCurve->createIterator();

    for(; !pIter->done(); pIter->step() )
    {
      OdDgElementPtr pItemElm = pIter->item().openObject(OdDg::kForRead);

      if( !pItemElm.isNull() && pItemElm->isKindOf(OdDgGraphicsElement::desc()) )
      {
        OdDgGraphicsElementPtr pGrItem = pItemElm;

        double dMaxItemLendth = getMaxLineWidth(pGrItem) * 2.0;

        if( dMaxItemLendth > 0 )
        {
          if( (dRet < 0) || (dRet > dMaxItemLendth) )
            dRet = dMaxItemLendth;
        }
      }
    }
  }

  return dRet;
}

//---------------------------------------------------------------------------------------------------

void applyLineStyleModifiers(OdDgGraphicsElement* pDgCurve, OdGiDgLinetypeModifiers& lsMod)
{
  if( !pDgCurve )
    return;

  double dMaxLineWidth = getMaxLineWidth(pDgCurve);

  if( dMaxLineWidth > 0 )
  {
    if( (lsMod.getWidthMode() == OdGiDgLinetypeModifiers::kLsWidthConstant) && (lsMod.getWidth() > dMaxLineWidth) )
      lsMod.setWidth(dMaxLineWidth);
    else if (lsMod.getWidthMode() == OdGiDgLinetypeModifiers::kLsWidthTapered)
    {
      if(lsMod.getWidth() > dMaxLineWidth)
        lsMod.setWidth(dMaxLineWidth);

      if (lsMod.getEndWidth() > dMaxLineWidth)
        lsMod.setEndWidth(dMaxLineWidth);
    }
  }

  OdDgLineStyleDataElementPEPtr pLineStyleDataPE = OdDgLineStyleDataElementPE::cast(pDgCurve);

  if (!pLineStyleDataPE.isNull())
    pLineStyleDataPE->setDgnLineStyleModifiers(pDgCurve, lsMod);
}

//---------------------------------------------------------------------------------------------------

bool applyStartEndWidths(OdDgGraphicsElement* pDgCurve, double dStartWidth, double dEndWidth)
{
  bool bRet = false;

  if (OdNonZero(dStartWidth) || OdNonZero(dEndWidth))
  {
    OdGiDgLinetypeModifiers lsMod;

    if (OdEqual(dStartWidth, dEndWidth))
    {
      lsMod.setTrueWidthFlag(true);
      lsMod.setWidthMode(OdGiDgLinetypeModifiers::kLsWidthConstant);
      lsMod.setWidth(dStartWidth);
    }
    else
    {
      lsMod.setTrueWidthFlag(true);
      lsMod.setWidthMode(OdGiDgLinetypeModifiers::kLsWidthTapered);
      lsMod.setWidth(dStartWidth);
      lsMod.setEndWidth(dEndWidth);
    }

    applyLineStyleModifiers(pDgCurve, lsMod);

    bRet = true;
  }

  return bRet;
}

//---------------------------------------------------------------------------------------------------

void OdDbLineExportPE::subExportElement(OdDbEntity* pDbEntity, OdDgElement* owner)
{
  OdDbLine* pDbLine = (OdDbLine*)(pDbEntity);

  if( OdZero(pDbLine->thickness()) )
  {
    OdGePoint3d p1, p2;
    p1 = pDbLine->startPoint();
    p2 = pDbLine->endPoint();

    OdDgGraphicsElementPtr pDgLine = createDgLine(p1, p2, owner->database());

    pDgLine->setThickness(pDbLine->thickness());
    addElementToDatabase(pDgLine, owner);
    copyElementProperties(pDgLine, pDbLine, owner->database());
    appendOdDgElementToOwner(owner, pDgLine);

    OdDgnExportContext::addDgnElement(pDbLine->objectId(), pDgLine->elementId());
  }
  else
  {
    OdGePoint3dArray arrShapePts;
    arrShapePts.push_back(pDbLine->startPoint());
    arrShapePts.push_back(pDbLine->endPoint());
    arrShapePts.push_back(pDbLine->endPoint() + pDbLine->normal() * pDbLine->thickness());
    arrShapePts.push_back(pDbLine->startPoint() + pDbLine->normal() * pDbLine->thickness());
    arrShapePts.push_back(pDbLine->startPoint());
    
    OdDgGraphicsElementPtr pDgLine = createDgShape(arrShapePts, pDbLine, owner->database());

    pDgLine->setThickness(0.0);
    addElementToDatabase(pDgLine, owner);
    copyElementProperties(pDgLine, pDbLine, owner->database());
    appendOdDgElementToOwner(owner, pDgLine);

    OdDgnExportContext::addDgnElement(pDbLine->objectId(), pDgLine->elementId());
  }
}

//---------------------------------------------------------------------------------------------------

void OdDbCircleExportPE::subExportElement(OdDbEntity* pDbEntity, OdDgElement* owner)
{
  OdDbCircle* pDbCircle = (OdDbCircle*)(pDbEntity);

  OdGePoint3d  ptCenter = pDbCircle->center();
  double       dRadius  = pDbCircle->radius();
  OdGeVector3d vrNormal = pDbCircle->normal();
  OdGePoint3d  ptStart;
  pDbCircle->getStartPoint(ptStart);
  OdGeVector3d vrXAxis = ptStart - ptCenter;

  if (vrXAxis.isZeroLength())
    vrXAxis = vrNormal.perpVector();
  else
    vrXAxis.normalize();

  OdDgGraphicsElementPtr pDgEllipse;

  if( OdDgnExportContext::getAllowCloseCurvesFlag() || OdDgnExportContext::isViewportClipObject(pDbEntity->id()) )
  {
    if (OdDgnExportContext::is3d())
    {
      OdDgEllipse3dPtr pEllipse3d = OdDgEllipse3d::createObject();
      pEllipse3d->setDatabaseDefaults(owner->database());
      pEllipse3d->setOrigin(ptCenter);
      pEllipse3d->setPrimaryAxis(dRadius);
      pEllipse3d->setSecondaryAxis(dRadius);

      OdGeMatrix3d matRotation;
      matRotation.setCoordSystem(OdGePoint3d::kOrigin, vrXAxis, vrNormal.crossProduct(vrXAxis), vrNormal);
      OdGeQuaternion quatRotation;
      quatRotation.set(matRotation);
      pEllipse3d->setRotation(quatRotation);
      pDgEllipse = pEllipse3d;
    }
    else
    {
      OdDgEllipse2dPtr pEllipse2d = OdDgEllipse2d::createObject();
      pEllipse2d->setDatabaseDefaults(owner->database());
      pEllipse2d->setOrigin(ptCenter.convert2d());
      pEllipse2d->setPrimaryAxis(dRadius);
      pEllipse2d->setSecondaryAxis(dRadius);
      pEllipse2d->setRotationAngle(OdGeVector3d::kXAxis.angleTo(vrXAxis, vrNormal));
      pDgEllipse = pEllipse2d;
    }
  }
  else
  {
    if (OdDgnExportContext::is3d())
    {
      OdDgArc3dPtr pArc3d = OdDgArc3d::createObject();
      pArc3d->setDatabaseDefaults(owner->database());
      pArc3d->setOrigin(ptCenter);
      pArc3d->setPrimaryAxis(dRadius);
      pArc3d->setSecondaryAxis(dRadius);

      OdGeMatrix3d matRotation;
      matRotation.setCoordSystem(OdGePoint3d::kOrigin, vrXAxis, vrNormal.crossProduct(vrXAxis), vrNormal);
      OdGeQuaternion quatRotation;
      quatRotation.set(matRotation);
      pArc3d->setRotation(quatRotation);
      pArc3d->setStartAngle(0);
      pArc3d->setSweepAngle(Oda2PI);
      pDgEllipse = pArc3d;
    }
    else
    {
      OdDgArc2dPtr pArc2d = OdDgArc2d::createObject();
      pArc2d->setDatabaseDefaults(owner->database());
      pArc2d->setOrigin(ptCenter.convert2d());
      pArc2d->setPrimaryAxis(dRadius);
      pArc2d->setSecondaryAxis(dRadius);
      pArc2d->setRotationAngle(OdGeVector3d::kXAxis.angleTo(vrXAxis, vrNormal));
      pArc2d->setStartAngle(0);
      pArc2d->setSweepAngle(Oda2PI);
      pDgEllipse = pArc2d;
    }
  }

  pDgEllipse->setThickness(pDbCircle->thickness());
  addElementToDatabase(pDgEllipse, owner);
  copyElementProperties(pDgEllipse, pDbCircle, owner->database());
  appendOdDgElementToOwner(owner, pDgEllipse);

  OdDgnExportContext::addDgnElement(pDbCircle->objectId(), pDgEllipse->elementId());
}

//---------------------------------------------------------------------------------------------------

void OdDbEllipseExportPE::subExportElement(OdDbEntity* pDbEntity, OdDgElement* owner)
{
  OdDbEllipse* pDbEllipse = (OdDbEllipse*)(pDbEntity);

  OdGePoint3d  ptCenter = pDbEllipse->center();
  OdGeVector3d vrMajor = pDbEllipse->majorAxis();
  OdGeVector3d vrMinor = pDbEllipse->minorAxis();
  OdGeVector3d vrNormal = pDbEllipse->normal();
  double dStartAngle = pDbEllipse->startAngle();
  double dEndAngle   = pDbEllipse->endAngle();

  if (dEndAngle < dStartAngle)
    dEndAngle += Oda2PI;

  OdDgGraphicsElementPtr pDgEllipse;

  if( vrMajor.isPerpendicularTo(vrMinor) && vrMajor.isPerpendicularTo(vrNormal) )
  {
    if( pDbEllipse->isClosed() && (OdDgnExportContext::getAllowCloseCurvesFlag() || OdDgnExportContext::isViewportClipObject(pDbEntity->id())))
    {
      if (OdDgnExportContext::is3d())
      {
        OdDgEllipse3dPtr pEllipse3d = OdDgEllipse3d::createObject();
        pEllipse3d->setDatabaseDefaults(owner->database());
        pEllipse3d->setOrigin(ptCenter);
        pEllipse3d->setPrimaryAxis(vrMajor.length());
        pEllipse3d->setSecondaryAxis(vrMinor.length());

        OdGeMatrix3d matRotation;
        matRotation.setCoordSystem(OdGePoint3d::kOrigin, vrMajor.normal(), vrMinor.normal(), vrNormal);
        OdGeQuaternion quatRotation;
        quatRotation.set(matRotation);
        pEllipse3d->setRotation(quatRotation);
        pDgEllipse = pEllipse3d;
      }
      else
      {
        OdDgEllipse2dPtr pEllipse2d = OdDgEllipse2d::createObject();
        pEllipse2d->setDatabaseDefaults(owner->database());
        pEllipse2d->setOrigin(ptCenter.convert2d());
        pEllipse2d->setPrimaryAxis(vrMajor.length());
        pEllipse2d->setSecondaryAxis(vrMinor.length());
        pEllipse2d->setRotationAngle(OdGeVector3d::kXAxis.angleTo(vrMajor.normal(), vrNormal));
        pDgEllipse = pEllipse2d;
      }
    }
    else
    {
      if( OdDgnExportContext::is3d() )
      {
        OdGeCurve3d* p3dCurve;

        if (pDbEllipse->getOdGeCurve(p3dCurve) == eOk)
        {
          OdDgArc3dPtr pArc3d = OdDgArc3d::createObject();
          pArc3d->setDatabaseDefaults(owner->database());
          OdGeVector3d vrNormal = pDbEllipse->normal();
          pArc3d->setFromOdGeCurve(*p3dCurve, &vrNormal);
          delete p3dCurve;

          pDgEllipse = pArc3d;
        }
      }
      else
      {
        OdGeCurve3d* p3dCurve;

        if (pDbEllipse->getOdGeCurve(p3dCurve) == eOk)
        {
          OdDgArc3dPtr pArc3d = OdDgArc3d::createObject();
          pArc3d->setDatabaseDefaults(owner->database());
          OdGeVector3d vrNormal = pDbEllipse->normal();
          pArc3d->setFromOdGeCurve(*p3dCurve, &vrNormal);
          delete p3dCurve;

          OdDg2d3dConversionGraphicsElementPEPtr pConvertTo2dPE = OdDg2d3dConversionGraphicsElementPE::cast(pArc3d);

          if (!pConvertTo2dPE.isNull())
            pDgEllipse = pConvertTo2dPE->convertTo2d(pArc3d);
        }
      }
    }
  }
  else
  {
    OdGeCurve3d* pEllipseCurve;

    if( pDbEllipse->getOdGeCurve(pEllipseCurve) == eOk )
    {
      pDgEllipse = createDgCurveFromGeCurve(pEllipseCurve, vrNormal, owner->database());
      delete pEllipseCurve;
    }
  }

  ODA_ASSERT_ONCE(!pDgEllipse.isNull());

  if( !pDgEllipse.isNull() )
  {
    addElementToDatabase(pDgEllipse, owner);
    copyElementProperties(pDgEllipse, pDbEllipse, owner->database());
    appendOdDgElementToOwner(owner, pDgEllipse);

    OdDgnExportContext::addDgnElement(pDbEllipse->objectId(), pDgEllipse->elementId());
  }
}

//---------------------------------------------------------------------------------------------------

void OdDbArcExportPE::subExportElement(OdDbEntity* pDbEntity, OdDgElement* owner)
{
  OdDbArc* pDbArc = (OdDbArc*)(pDbEntity);

  OdGePoint3d  ptCenter = pDbArc->center();
  double       dRadius = pDbArc->radius();
  OdGeVector3d vrNormal = pDbArc->normal();
  OdGeMatrix3d matEsc = pDbArc->getEcs();
  OdGeVector3d vrXAxis = OdGeVector3d::kXAxis;
  vrXAxis.transformBy(matEsc);

  if (vrXAxis.isZeroLength())
    vrXAxis = vrNormal.perpVector();
  else
    vrXAxis.normalize();

  double dStartAngle = pDbArc->startAngle();
  double dEndAngle = pDbArc->endAngle();

  if( dEndAngle < dStartAngle )
    dEndAngle += Oda2PI;

  OdDgGraphicsElementPtr pDgArc;

  if (OdDgnExportContext::is3d())
  {
    OdDgArc3dPtr pArc3d = OdDgArc3d::createObject();
    pArc3d->setDatabaseDefaults(owner->database());
    pArc3d->setOrigin(ptCenter);
    pArc3d->setPrimaryAxis(dRadius);
    pArc3d->setSecondaryAxis(dRadius);

    OdGeMatrix3d matRotation;
    matRotation.setCoordSystem(OdGePoint3d::kOrigin, vrXAxis, vrNormal.crossProduct(vrXAxis), vrNormal);
    OdGeQuaternion quatRotation;
    quatRotation.set(matRotation);
    pArc3d->setRotation(quatRotation);
    pArc3d->setStartAngle(dStartAngle);
    pArc3d->setSweepAngle(dEndAngle - dStartAngle);
    pDgArc = pArc3d;
  }
  else
  {
    OdDgArc2dPtr pArc2d = OdDgArc2d::createObject();
    pArc2d->setDatabaseDefaults(owner->database());
    pArc2d->setOrigin(ptCenter.convert2d());
    pArc2d->setPrimaryAxis(dRadius);
    pArc2d->setSecondaryAxis(dRadius);
    pArc2d->setRotationAngle(OdGeVector3d::kXAxis.angleTo(vrXAxis, vrNormal));
    pArc2d->setStartAngle(dStartAngle);
    pArc2d->setSweepAngle(dEndAngle - dStartAngle);
    pDgArc = pArc2d;
  }

  pDgArc->setThickness(pDbArc->thickness());
  addElementToDatabase(pDgArc, owner);
  copyElementProperties(pDgArc, pDbArc, owner->database());
  appendOdDgElementToOwner(owner, pDgArc);

  OdDgnExportContext::addDgnElement(pDbArc->objectId(), pDgArc->elementId());
}

//---------------------------------------------------------------------------------------------------

bool createConstantWidthLineStyleModifiers(OdDbPolyline* pDbPolyLine, OdGiDgLinetypeModifiers& lsModConstWidth)
{
  bool bRet = false;

  double dConstantWidth = pDbPolyLine->getConstantWidth();

  if( !pDbPolyLine->hasWidth() && OdNonZero(dConstantWidth) )
  {
    lsModConstWidth.setTrueWidthFlag(true);
    lsModConstWidth.setWidthMode(OdGiDgLinetypeModifiers::kLsWidthConstant);
    lsModConstWidth.setWidth(dConstantWidth);
    bRet = true;
  }

  return bRet;
}

//---------------------------------------------------------------------------------------------------

bool createLineStyleModifiersForPolylineSegment(OdDbPolyline* pDbPolyLine, OdUInt32 uSegment, OdGiDgLinetypeModifiers& lsMod)
{
  bool bRet = createConstantWidthLineStyleModifiers(pDbPolyLine, lsMod);

  if( !bRet )
  {
    double dStartWidth = 0;
    double dEndWidth = 0;
    pDbPolyLine->getWidthsAt(uSegment, dStartWidth, dEndWidth);

    if( OdEqual(dStartWidth, dEndWidth) )
    {
      if( OdNonZero(dStartWidth) )
      {
        lsMod.setTrueWidthFlag(true);
        lsMod.setWidthMode(OdGiDgLinetypeModifiers::kLsWidthConstant);
        lsMod.setWidth(dStartWidth);
        bRet = true;
      }
    }
    else
    {
      lsMod.setTrueWidthFlag(true);
      lsMod.setWidthMode(OdGiDgLinetypeModifiers::kLsWidthTapered);
      lsMod.setWidth(dStartWidth);
      lsMod.setEndWidth(dEndWidth);
      bRet = true;
    }
  }

  return bRet;
}

OdDgGraphicsElementPtr exportDbPolyline(  OdDbPolyline* pDbPolyLine, 
                                            OdDbEntity* pDbBaseEntity, 
                                              OdDgDatabase* pDb, 
                                                bool bPLineGenerateOff,
                                                  bool bCreateShapes,
                                                    bool& bSetLineStyleModifiers,
                                                      bool& bNegativeThickness )
{
  bNegativeThickness = false;
  OdDgGraphicsElementPtr pDgPolyline;
  bSetLineStyleModifiers = true;

  if (OdDgnExportContext::isViewportClipObject(pDbBaseEntity->id()))
    bCreateShapes = true;

  // 1. Skip polyline if it is empty

  if (pDbPolyLine->segType(0) == OdDbPolyline::kEmpty)
    return pDgPolyline;

  // 2. If polyline contains only 1 line, then create point (line with zero length).

  if( pDbPolyLine->segType(0) == OdDbPolyline::kPoint) // polyline contains only 1 point
  {
    if( bCreateShapes )
      return pDgPolyline;

    OdGePoint3d ptOrigin;
    pDbPolyLine->getPointAt(0, ptOrigin);
    pDgPolyline = createDgLine(ptOrigin, ptOrigin, pDb);
  }
  // 3. If polyline contains only 1 segment, then create DgLine or DgArc
  else if (pDbPolyLine->numVerts() == 2 && !pDbPolyLine->isClosed())
  {
    if (!OdZero(pDbPolyLine->getBulgeAt(0)))
    {
      OdGeCircArc3d arcSeg;
      pDbPolyLine->getArcSegAt(0, arcSeg);

      OdGeVector3d vrPLineNormal = pDbPolyLine->normal();

      if( bCreateShapes && !arcSeg.isClosed() )
      {
        OdDgComplexShapePtr pCplxElm = OdDgComplexShape::createObject();
        OdDgGraphicsElementPtr pDgArc  = createDgCurveFromGeCurve(&arcSeg, vrPLineNormal, pDb);
        OdDgGraphicsElementPtr pDgLine = createDgLine(arcSeg.endPoint(), arcSeg.startPoint(), pDb);
        copyElementProperties(pDgArc, pDbBaseEntity, pDb, true);
        copyElementProperties(pDgLine, pDbBaseEntity, pDb, true);
        pCplxElm->add(pDgArc);
        pCplxElm->add(pDgLine);
        pDgPolyline = pCplxElm;
      }
      else
        pDgPolyline = createDgCurveFromGeCurve(&arcSeg, vrPLineNormal, pDb);
    }

    OdGiDgLinetypeModifiers lsModSegment;
    bool bHasModifiers = createLineStyleModifiersForPolylineSegment(pDbPolyLine, 0, lsModSegment);

    if( pDgPolyline.isNull() )
    {
      bool bCraeteLine = true;

      OdGePoint3d ptStartVertex, ptEndVertex;

      pDbPolyLine->getPointAt(0, ptStartVertex);
      pDbPolyLine->getPointAt(1, ptEndVertex);

      OdGeVector3d vrStartEnd = ptEndVertex - ptStartVertex;

      if( bHasModifiers && !vrStartEnd.isZeroLength() )
      {
        OdGeVector3d vrPerp = vrStartEnd.rotateBy(OdaPI2, pDbPolyLine->normal());
        vrPerp.normalize();

        if( lsModSegment.getWidthMode() == OdGiDgLinetypeModifiers::kLsWidthConstant )
        {
          double dWidth = lsModSegment.getWidth();

          if( !OdZero(dWidth) )
          {
            OdGePoint3dArray arrShapePts;
            arrShapePts.push_back(ptStartVertex + vrPerp * dWidth / 2.0);
            arrShapePts.push_back(ptEndVertex + vrPerp * dWidth / 2.0);
            arrShapePts.push_back(ptEndVertex - vrPerp * dWidth / 2.0);
            arrShapePts.push_back(ptStartVertex - vrPerp * dWidth / 2.0);
            arrShapePts.push_back(arrShapePts.first());

            pDgPolyline = createDgShape(arrShapePts, pDbPolyLine, pDb);
            bCraeteLine = false;
            bHasModifiers = false;
          }
        }
        else if( lsModSegment.getWidthMode() == OdGiDgLinetypeModifiers::kLsWidthTapered )
        {
          double dStartWidth = lsModSegment.getWidth();
          double dEndWidth   = lsModSegment.getEndWidth();

          if( !OdZero(dStartWidth) || !OdZero(dEndWidth) )
          {
            OdGePoint3dArray arrShapePts;
            arrShapePts.push_back(ptStartVertex + vrPerp * dStartWidth / 2.0);
            arrShapePts.push_back(ptEndVertex + vrPerp * dEndWidth / 2.0);

            if( !OdZero(dEndWidth) )
              arrShapePts.push_back(ptEndVertex - vrPerp * dEndWidth / 2.0);

            if( !OdZero( dStartWidth) )
            arrShapePts.push_back(ptStartVertex - vrPerp * dStartWidth / 2.0);

            arrShapePts.push_back(arrShapePts.first());

            pDgPolyline = createDgShape(arrShapePts, pDbPolyLine, pDb);
            bCraeteLine = false;
            bHasModifiers = false;
          }
        }
      }

      if(bCraeteLine && !bCreateShapes )
      {
        pDgPolyline = createDgLine(ptStartVertex, ptEndVertex, pDb);
      }
    }

    if( bHasModifiers )
    {
      applyLineStyleModifiers(pDgPolyline, lsModSegment);
      bSetLineStyleModifiers = false;
    }
  }
  // 4. If polyline contains 2 arc segment, then create DgLine or DgArc
  else if( pDbPolyLine->numVerts() == 2 && pDbPolyLine->isClosed() && pDbPolyLine->hasBulges() && 
           OdNonZero(pDbPolyLine->getBulgeAt(0)) && OdNonZero(pDbPolyLine->getBulgeAt(1)) && !pDbPolyLine->hasWidth() )
  {
    OdGeCircArc3d arcSeg1, arcSeg2;

    pDbPolyLine->getArcSegAt(0, arcSeg1);
    pDbPolyLine->getArcSegAt(1, arcSeg2);

    if( arcSeg1.normal().isEqualTo(arcSeg2.normal()) && arcSeg1.center().isEqualTo(arcSeg2.center()) && OdEqual(arcSeg1.radius(), arcSeg2.radius()) )
    {
      arcSeg1.setAngles(arcSeg1.startAng(), arcSeg1.endAng() + arcSeg2.endAng() - arcSeg2.startAng());

      pDgPolyline = createDgArcFromGeCircleArc3d(arcSeg1, bCreateShapes, pDb);

      OdGiDgLinetypeModifiers lsModSegment;

      if (createLineStyleModifiersForPolylineSegment(pDbPolyLine, 0, lsModSegment))
      {
        applyLineStyleModifiers(pDgPolyline, lsModSegment);
        bSetLineStyleModifiers = false;
      }
    }
  }

  // 5. Create complex curve if db polyline has bulges, non constant widths or non-continuous line type with off plinegen flag.
  if( pDgPolyline.isNull() && (pDbPolyLine->hasBulges() || pDbPolyLine->hasWidth() || bPLineGenerateOff ))
  {
    bool bWidthApplied = false;

    OdDgComplexCurvePtr pCplxElm;
    
    if( bCreateShapes )
      pCplxElm = OdDgComplexShape::createObject();
    else
      pCplxElm = OdDgComplexString::createObject();

    for (OdUInt32 i = 0; i < pDbPolyLine->numVerts(); i++)
    {
      if ((i == pDbPolyLine->numVerts() - 1) && !pDbPolyLine->isClosed())
        continue;

      OdDgGraphicsElementPtr pDgCurve;

      if (OdNonZero(pDbPolyLine->getBulgeAt(i)))
      {
        OdGeCircArc3d arcSeg;
        try
        {
          pDbPolyLine->getArcSegAt(i, arcSeg);
          OdGeVector3d vrPLineNormal = pDbPolyLine->normal();
          pDgCurve = createDgCurveFromGeCurve(&arcSeg, vrPLineNormal, pDb);
        }
        catch (...)
        {
          pDgCurve = NULL;
        }
      }

      if (pDgCurve.isNull())
      {
        OdGePoint3d ptStartVertex, ptEndVertex;

        pDbPolyLine->getPointAt(i, ptStartVertex);

        if (i == pDbPolyLine->numVerts() - 1)
          pDbPolyLine->getPointAt(0, ptEndVertex);
        else
          pDbPolyLine->getPointAt(i + 1, ptEndVertex);

        pDgCurve = createDgLine(ptStartVertex, ptEndVertex, pDb);
      }

      OdGiDgLinetypeModifiers lsMod;

      bool bSetSegmentModifiers = true;

      if (createLineStyleModifiersForPolylineSegment(pDbPolyLine, i, lsMod))
      {
        applyLineStyleModifiers(pDgCurve, lsMod);
        bSetSegmentModifiers = false;
      }

      if (!pDgCurve.isNull())
      {
        pCplxElm->add(pDgCurve);
        copyElementProperties(pDgCurve, pDbBaseEntity, pDb, bSetSegmentModifiers);
      }
    }

    OdGiDgLinetypeModifiers lsModConst;

    if (createConstantWidthLineStyleModifiers(pDbPolyLine, lsModConst))
    {
      pCplxElm->setLineStyleModifiers(lsModConst);
      bSetLineStyleModifiers = false;
    }

    pDgPolyline = pCplxElm;
  }
  // 6. Create dgn line string element.
  else if( pDgPolyline.isNull() )
  {
    OdGePoint3dArray arrVertices;

    for (OdUInt32 i = 0; i < pDbPolyLine->numVerts(); i++)
    {
      OdGePoint3d ptVertex;
      pDbPolyLine->getPointAt(i, ptVertex);
      arrVertices.push_back(ptVertex);
    }

    if (pDbPolyLine->isClosed() && !arrVertices.first().isEqualTo(arrVertices.last()))
      arrVertices.push_back(arrVertices.first());

    if( pDbPolyLine->normal().z < 0 )
      bNegativeThickness = true;
    else if( OdZero(pDbPolyLine->normal().z) )
    {
      OdGeVector3d vrCalcNormal = pDbPolyLine->normal();

      if( geCalculateNormal(arrVertices, &vrCalcNormal) == eOk )
      {
        if (!vrCalcNormal.isCodirectionalTo(pDbPolyLine->normal()))
          arrVertices.reverse();
      }
    }

    if( bCreateShapes )
      pDgPolyline = createDgShape(arrVertices, pDbPolyLine, pDb);
    else
      pDgPolyline = createDgLineString(arrVertices, pDbPolyLine, pDb);

    OdGiDgLinetypeModifiers lsModConst;

    if (createConstantWidthLineStyleModifiers(pDbPolyLine, lsModConst))
    {
      applyLineStyleModifiers(pDgPolyline, lsModConst);
      bSetLineStyleModifiers = false;
    }
  }

  return pDgPolyline;
}

//---------------------------------------------------------------------------------------------------

void OdDbPolylineExportPE::subExportElement(OdDbEntity* pDbEntity, OdDgElement* owner)
{
  OdDbPolyline* pDbPolyLine = (OdDbPolyline*)(pDbEntity);

  OdDgGraphicsElementPtr pDgPolyline;
  bool bPLineGenerateOff = !pDbPolyLine->hasPlinegen() && isContinuousLineStyle(pDbPolyLine->linetypeId(), pDbPolyLine->layerId(), true);
  bool bSetLineStyleModifiers = true;
  bool bNegativeThickness     = false;

  pDgPolyline = exportDbPolyline(pDbPolyLine, pDbPolyLine, owner->database(), bPLineGenerateOff, false, bSetLineStyleModifiers, bNegativeThickness);

  if( !pDgPolyline.isNull() )
  {
    if( bNegativeThickness )
      pDgPolyline->setThickness(-pDbPolyLine->thickness(),true);
    else
      pDgPolyline->setThickness(pDbPolyLine->thickness(), true);

    addElementToDatabase(pDgPolyline, owner);
    copyElementProperties(pDgPolyline, pDbPolyLine, owner->database(), bSetLineStyleModifiers);
    appendOdDgElementToOwner(owner, pDgPolyline);
    OdDgnExportContext::addDgnElement(pDbPolyLine->objectId(), pDgPolyline->elementId());
  }
}

//---------------------------------------------------------------------------------------------------

OdResult convertPolylineToGeCurve(OdGePoint3dArray& arrCtrlPts, int nDegree, bool bClosed, OdGeCurve3d*& pGeCurve)
{
  OdGeKnotVector knots;
  int i = 0;
  int j = 0;
  int nCpLength = 0;
  double dKnota = 0;
  double dKnotb = 1;

  nCpLength = arrCtrlPts.logicalLength();

  j = nCpLength - nDegree - 1;

  if (nCpLength < 2)
    return eNotApplicable;
  else if (nCpLength == 2)
  {
    pGeCurve = new OdGeLineSeg3d(arrCtrlPts[0], arrCtrlPts[1]);
    return eOk;
  }

  if( !bClosed )
  { //open curve
    for (i = 0; i <= nDegree; i++)
    {
      knots.append(0);
    }
    if (j > 0)
    {
      dKnota = dKnotb / (j + 1);
      for (i = 0; i < j; i++)
      {
        knots.append(dKnota*(i + 1));
      }
    }
    else
    {
      if (j)
      { //quadratic curve with 3 control points: adding the fourth cp
        arrCtrlPts.insertAt(1, arrCtrlPts.at(1));
        j++;
        nCpLength++;
      }
    }
    for (i = 0; i <= nDegree; i++)
    {
      knots.append(dKnotb);
    }
  }
  else
  { //closed curve
    arrCtrlPts.append(arrCtrlPts.at(0));

    for (i = 1; i < nDegree; i++)
      arrCtrlPts.append(arrCtrlPts.at(i));


    j = nCpLength + 2 * nDegree + 1;
    dKnota = dKnotb / (j);
    for (i = 0; i < j; i++)
    {
      knots.append(dKnota*(i));
    }
  }

  pGeCurve = new OdGeNurbCurve3d(nDegree, knots, arrCtrlPts, bClosed);

  return eOk;
}

//---------------------------------------------------------------------------------------------------

OdResult convert3dPolylineToGeCurve(OdDb3dPolyline* pDb3dPolyLine, OdGeCurve3d*& pGeCurve)
{
  if( !pDb3dPolyLine )
    return eNullObjectPointer;

  OdGePoint3dArray controlPoints;

  OdDb3dPolylineVertexPtr pVert;
  OdDbObjectIteratorPtr pIter = pDb3dPolyLine->vertexIterator();

  for(; !pIter->done(); pIter->step())
  {
    pVert = pIter->entity(OdDb::kForRead);
    ODA_ASSERT(!pVert.isNull());

    if( pVert->vertexType() != OdDb::k3dControlVertex )
      continue;

    controlPoints.append(pVert->position());
  }

  int nDegree;

  switch (pDb3dPolyLine->polyType())
  {
    case OdDb::k3dQuadSplinePoly:  nDegree = 2; break;
    case OdDb::k3dCubicSplinePoly: nDegree = 3; break;
    default: return eInvalidInput;
  }

  return convertPolylineToGeCurve(controlPoints, nDegree, pDb3dPolyLine->isClosed(), pGeCurve);
}

//---------------------------------------------------------------------------------------------------

OdResult convert2dPolylineToGeCurve(OdDb2dPolyline* pDb2dPolyLine, OdGeCurve3d*& pGeCurve)
{
  if (!pDb2dPolyLine)
    return eNullObjectPointer;

  OdGePoint3dArray controlPoints;

  OdDb2dVertexPtr pVert;
  OdDbObjectIteratorPtr pIter = pDb2dPolyLine->vertexIterator();

  for (; !pIter->done(); pIter->step())
  {
    pVert = pIter->entity(OdDb::kForRead);
    ODA_ASSERT(!pVert.isNull());

    if (pVert->vertexType() != OdDb::k2dSplineCtlVertex)
      continue;

    controlPoints.append( pDb2dPolyLine->vertexPosition(*pVert) );
  }

  int nDegree;

  switch (pDb2dPolyLine->polyType())
  {
    case OdDb::k2dQuadSplinePoly:  nDegree = 2; break;
    case OdDb::k2dCubicSplinePoly: nDegree = 3; break;
    default: return eInvalidInput;
  }

  return convertPolylineToGeCurve(controlPoints, nDegree, pDb2dPolyLine->isClosed(), pGeCurve);
}

//---------------------------------------------------------------------------------------------------

void OdDb3dPolylineExportPE::subExportElement(OdDbEntity* pDbEntity, OdDgElement* owner)
{
  OdDb3dPolyline* pDb3dPolyLine = (OdDb3dPolyline*)(pDbEntity);

  OdDgGraphicsElementPtr pDgPolyline;

  if( pDb3dPolyLine->polyType() == OdDb::k2dSimplePoly )
  {
    OdGePoint3dArray arrPts;

    OdDbObjectIteratorPtr pVertexIter = pDb3dPolyLine->vertexIterator();

    for(; !pVertexIter->done(); pVertexIter->step() )
    {
      OdDb3dPolylineVertexPtr pVertex = pVertexIter->entity();
      arrPts.push_back(pVertex->position());
    }

    if (pDb3dPolyLine->isClosed() && (arrPts.size() > 0) && !arrPts.first().isEqualTo(arrPts.last()))
      arrPts.append(arrPts.first());

    pDgPolyline = createDgLineString(arrPts, pDb3dPolyLine, owner->database());
  }
  else
  {
    OdGeCurve3d* pGeCurve = NULL;

    if( convert3dPolylineToGeCurve(pDb3dPolyLine, pGeCurve) == eOk )
    {
      OdGeVector3d vrNormal = OdGeVector3d::kZAxis;
      pDgPolyline = createDgCurveFromGeCurve(pGeCurve, vrNormal, owner->database());

      delete pGeCurve;
    }
  }

  if (!pDgPolyline.isNull())
  {
    addElementToDatabase(pDgPolyline, owner);
    copyElementProperties(pDgPolyline, pDb3dPolyLine, owner->database(), true);
    appendOdDgElementToOwner(owner, pDgPolyline);
    OdDgnExportContext::addDgnElement(pDb3dPolyLine->objectId(), pDgPolyline->elementId());
  }
}

//---------------------------------------------------------------------------------------------------

void OdDb2dPolylineExportPE::subExportElement(OdDbEntity* pDbEntity, OdDgElement* owner)
{
  OdDb2dPolyline* pDb2dPolyLine = (OdDb2dPolyline*)(pDbEntity);

  OdDgGraphicsElementPtr pDgPolyline;
  bool bSetLineStyleModifiers = true;

  // try to get constant width

  bool    bHasWidths  = false;
  double  dConstWidth = 0.0;

  OdDbObjectIteratorPtr pVertexIter = pDb2dPolyLine->vertexIterator();

  if( !pVertexIter.isNull() )
  {
    bool bInitWidth = false;

    for(; !pVertexIter->done(); pVertexIter->step() )
    {
      OdDb2dVertexPtr p2dVertex = pVertexIter->entity(OdDb::kForRead);

      if( !bInitWidth )
      {
        bHasWidths  = !OdEqual(p2dVertex->startWidth(), p2dVertex->endWidth());
        dConstWidth = p2dVertex->startWidth();
        bInitWidth = true;
      }
      else
      {
        if (!OdEqual(p2dVertex->startWidth(), dConstWidth) || !OdEqual(p2dVertex->endWidth(), dConstWidth))
          bHasWidths = true;
      }

      if( bHasWidths )
        break;
    }
  }

  // Convert 2d polyline to dgn curve

  bool bNegativeThickness = false;

  switch( pDb2dPolyLine->polyType() )
  {
    case OdDb::k2dQuadSplinePoly:
    case OdDb::k2dCubicSplinePoly:
    {
      OdGeCurve3d* pGeCurve = NULL;

      if( convert2dPolylineToGeCurve(pDb2dPolyLine, pGeCurve) == eOk )
      {
        OdGeVector3d vrNormal = pDb2dPolyLine->normal();
        pDgPolyline = createDgCurveFromGeCurve(pGeCurve, vrNormal, owner->database());

        delete pGeCurve;

        if( !bHasWidths && !OdZero(dConstWidth) )
        {
          bSetLineStyleModifiers = !applyStartEndWidths(pDgPolyline, dConstWidth, dConstWidth);
        }
        else
        {
          double dStartWidth = pDb2dPolyLine->defaultStartWidth();
          double dEndWidth = pDb2dPolyLine->defaultEndWidth();
          bSetLineStyleModifiers = !applyStartEndWidths(pDgPolyline, dStartWidth, dEndWidth);
        }        
      }
    } break;

    default:
    {
      pVertexIter = pDb2dPolyLine->vertexIterator();

      if( !pVertexIter.isNull() )
      {
        OdGeMatrix3d matEcs = pDb2dPolyLine->getEcs();
        matEcs.inverse();

        OdDbPolylinePtr pPolyline = OdDbPolyline::createObject();
        pPolyline->setNormal(pDb2dPolyLine->normal());
		    OdUInt32 iVertex = 0;
        pPolyline->setElevation(pDb2dPolyLine->elevation());

        for(; !pVertexIter->done(); pVertexIter->step())
        {
          OdDb2dVertexPtr p2dVertex = pVertexIter->entity(OdDb::kForRead);

          if( (p2dVertex->vertexType() == OdDb::k2dSplineCtlVertex) || (p2dVertex->vertexType() == OdDb::k2dSplineFitVertex) )
            continue;

          if( !p2dVertex.isNull() )
          {
            OdGePoint3d ptPos3d = p2dVertex->position();
			      pPolyline->addVertexAt(iVertex, ptPos3d.convert2d(), p2dVertex->bulge(), p2dVertex->startWidth(), p2dVertex->endWidth());
			      iVertex++;
          }
        }

        pPolyline->setClosed(pDb2dPolyLine->isClosed());

        if( !bHasWidths && OdNonZero(dConstWidth) )
          pPolyline->setConstantWidth(dConstWidth);

        bool bPLineGenerateOff = !pDb2dPolyLine->isLinetypeGenerationOn() && isContinuousLineStyle(pDb2dPolyLine->linetypeId(), pDb2dPolyLine->layerId(), true);

        pDgPolyline = exportDbPolyline(pPolyline, pDb2dPolyLine, owner->database(), bPLineGenerateOff, false, bSetLineStyleModifiers, bNegativeThickness);
      }
    }
  }

  if (!pDgPolyline.isNull())
  {
    if (bNegativeThickness)
      pDgPolyline->setThickness(-pDb2dPolyLine->thickness(), true);
    else
      pDgPolyline->setThickness(pDb2dPolyLine->thickness(), true);
    addElementToDatabase(pDgPolyline, owner);
    copyElementProperties(pDgPolyline, pDb2dPolyLine, owner->database(), bSetLineStyleModifiers);
    appendOdDgElementToOwner(owner, pDgPolyline);
    OdDgnExportContext::addDgnElement(pDb2dPolyLine->objectId(), pDgPolyline->elementId());
  }
}

//---------------------------------------------------------------------------------------------------

void OdDbBSplineCurveExportPE::subExportElement(OdDbEntity* pDbEntity, OdDgElement* owner)
{
  OdDbSpline* pDbSpline = (OdDbSpline*)(pDbEntity);

  OdDgGraphicsElementPtr pDgPolyline;

  if( OdDgnExportContext::is3d() )
  {
    OdDgBSplineCurve3dPtr pDgCurve = OdDgBSplineCurve3d::createObject();

    bool bCreateNurbsData = true;

    if( !pDbSpline->isClosed() && pDbSpline->hasFitData() )
    {
      OdGePoint3dArray arrFitPts;
      int iDegree;
      double dFitTolerance;
      bool bTangents;
      OdGeVector3d vrStartTangent = OdGeVector3d::kIdentity;
      OdGeVector3d vrEndTangent = OdGeVector3d::kIdentity;
      OdGe::OdGeKnotParameterization knotParams;

      if( pDbSpline->getFitData(arrFitPts, iDegree, dFitTolerance, bTangents, vrStartTangent, vrEndTangent, knotParams) == eOk )
      {
        if( pDgCurve->setFitData(arrFitPts, vrStartTangent, vrEndTangent, false, knotParams == OdGe::kUniform) == eOk )
          bCreateNurbsData = false;
      }
    }

    if( bCreateNurbsData )
    {
      bool bCreateOpenCurve = true;

      if( pDbSpline->isClosed() )
      {
        OdGeCurve3d* pGeCurve;

        if( (pDbSpline->getOdGeCurve(pGeCurve) == eOk) && pGeCurve && (pGeCurve->type() == OdGe::kNurbCurve3d) )
        {
          OdGeNurbCurve3d* pNurbCurve = (OdGeNurbCurve3d*)(pGeCurve);
          OdGeVector3d vrNormal = OdGeVector3d::kZAxis;

          if( pDgCurve->setFromOdGeNurbCurve(*pNurbCurve, &vrNormal, OdGeContext::gTol, OdDgnExportContext::getAllowCloseCurvesFlag()) == eOk )
          {
            pDgPolyline = pDgCurve;
            bCreateOpenCurve = false;
          }

          delete pGeCurve;
        }
      }

      if( bCreateOpenCurve )
      {
        int iDegree;
        double dCtrlPtsTolerance = 1e-10;
        bool bRational, bClosed, bPeriodic;
        OdGePoint3dArray arrCtrlPts;
        OdGeKnotVector vrKnots;
        OdGeDoubleArray arrWeights;

        pDbSpline->getNurbsData(iDegree, bRational, bClosed, bPeriodic, arrCtrlPts, vrKnots, arrWeights, dCtrlPtsTolerance);

        vrKnots.setRange(0.0, 1.0);

        if( pDgCurve->setNurbsData(iDegree + 1, bRational, false, arrCtrlPts, vrKnots, arrWeights) != eOk )
          pDgCurve = NULL;
      }
    }

    if (!pDgCurve.isNull())
      pDgCurve->setPolygonDisplayFlag(pDbSpline->cvFrame());

    pDgPolyline = pDgCurve;
  }
  else
  {
    OdDgBSplineCurve2dPtr pDgCurve = OdDgBSplineCurve2d::createObject();

    bool bCreateNurbsData = true;

    if( !pDbSpline->isClosed() && pDbSpline->hasFitData() )
    {
      OdGePoint3dArray arrFitPts;
      int iDegree;
      double dFitTolerance;
      bool bTangents;
      OdGeVector3d vrStartTangent = OdGeVector3d::kIdentity;
      OdGeVector3d vrEndTangent = OdGeVector3d::kIdentity;
      OdGe::OdGeKnotParameterization knotParams;

      if (pDbSpline->getFitData(arrFitPts, iDegree, dFitTolerance, bTangents, vrStartTangent, vrEndTangent, knotParams) == eOk )
      {
        OdGePoint2dArray arrFitPts2d;

        for (OdUInt32 i = 0; i < arrFitPts.size(); i++)
          arrFitPts2d.push_back(arrFitPts[i].convert2d());

        if(pDgCurve->setFitData(arrFitPts2d, vrStartTangent.convert2d(), vrEndTangent.convert2d(), pDbSpline->isClosed(), knotParams == OdGe::kUniform) == eOk )
          bCreateNurbsData = false;
      }
    }

    if (bCreateNurbsData)
    {
      bool bCreateOpenCurve = true;

      if (pDbSpline->isClosed())
      {
        OdGeCurve3d* pGeCurve;

        if ((pDbSpline->getOdGeCurve(pGeCurve) == eOk) && pGeCurve && (pGeCurve->type() == OdGe::kNurbCurve3d))
        {
          OdGeCurve2d* pGeCurve2d = pGeCurve->convertTo2d();

          if( pGeCurve2d && (pGeCurve2d->type() == OdGe::kNurbCurve2d) )
          {
            OdGeNurbCurve2d* pNurbCurve2d = (OdGeNurbCurve2d*)(pGeCurve2d);

            if( pDgCurve->setFromOdGeNurbCurve(*pNurbCurve2d, OdGeContext::gTol, OdDgnExportContext::getAllowCloseCurvesFlag() ) == eOk )
            {
              pDgPolyline = pDgCurve;
              bCreateOpenCurve = false;
            }

            delete pGeCurve2d;
          }

          delete pGeCurve;
        }
      }

      if( bCreateOpenCurve )
      {
        int iDegree;
        double dCtrlPtsTolerance;
        bool bRational, bClosed, bPeriodic;
        OdGePoint3dArray arrCtrlPts;
        OdGeKnotVector vrKnots;
        OdGeDoubleArray arrWeights;

        pDbSpline->getNurbsData(iDegree, bRational, bClosed, bPeriodic, arrCtrlPts, vrKnots, arrWeights, dCtrlPtsTolerance);

        OdGePoint2dArray arrCtrlPts2d;

        for (OdUInt32 i = 0; i < arrCtrlPts.size(); i++)
          arrCtrlPts2d.push_back(arrCtrlPts[i].convert2d());

        vrKnots.setRange(0.0, 1.0);

        if( pDgCurve->setNurbsData(iDegree + 1, bRational, false, arrCtrlPts2d, vrKnots, arrWeights) != eOk )
          pDgCurve = NULL;
      }
    }

    if (!pDgCurve.isNull())
      pDgCurve->setPolygonDisplayFlag(pDbSpline->cvFrame());

    pDgPolyline = pDgCurve;
  }

  if (!pDgPolyline.isNull())
  {
    addElementToDatabase(pDgPolyline, owner);
    copyElementProperties(pDgPolyline, pDbSpline, owner->database(), true);
    appendOdDgElementToOwner(owner, pDgPolyline);
    OdDgnExportContext::addDgnElement(pDbSpline->objectId(), pDgPolyline->elementId());
  }
}

//---------------------------------------------------------------------------------------------------

void OdDbRayExportPE::subExportElement(OdDbEntity* pDbEntity, OdDgElement* owner)
{
  OdDbRay* pDbRay = (OdDbRay*)(pDbEntity);

  OdGePoint3d ptStart = pDbRay->basePoint();
  OdGePoint3d ptEnd = ptStart + pDbRay->unitDir()*10000.0;

  OdDgGraphicsElementPtr pDgPolyline;

  if (OdDgnExportContext::is3d())
  {
    OdDgLine3dPtr pLine3d = OdDgLine3d::createObject();
    pLine3d->setDatabaseDefaults(owner->database());
    pLine3d->setStartPoint(ptStart);
    pLine3d->setEndPoint(ptEnd);
    pLine3d->setInfiniteLineType(OdDg::kFromEndPoint);
    pDgPolyline = pLine3d;
  }
  else
  {
    OdDgLine2dPtr pLine2d = OdDgLine2d::createObject();
    pLine2d->setDatabaseDefaults(owner->database());
    pLine2d->setStartPoint(ptStart.convert2d());
    pLine2d->setEndPoint(ptEnd.convert2d());
    pLine2d->setInfiniteLineType(OdDg::kFromEndPoint);
    pDgPolyline = pLine2d;
  }

  if (!pDgPolyline.isNull())
  {
    addElementToDatabase(pDgPolyline, owner);
    copyElementProperties(pDgPolyline, pDbRay, owner->database(), true);
    appendOdDgElementToOwner(owner, pDgPolyline);
    OdDgnExportContext::addDgnElement(pDbRay->objectId(), pDgPolyline->elementId());
  }
}

//---------------------------------------------------------------------------------------------------

void OdDbXLineExportPE::subExportElement(OdDbEntity* pDbEntity, OdDgElement* owner)
{
  OdDbXline* pDbXLine = (OdDbXline*)(pDbEntity);

  OdGePoint3d ptStart = pDbXLine->basePoint();
  OdGePoint3d ptEnd = ptStart + pDbXLine->unitDir()*10000.0;

  OdDgGraphicsElementPtr pDgPolyline;

  if (OdDgnExportContext::is3d())
  {
    OdDgLine3dPtr pLine3d = OdDgLine3d::createObject();
    pLine3d->setDatabaseDefaults(owner->database());
    pLine3d->setStartPoint(ptStart);
    pLine3d->setEndPoint(ptEnd);
    pLine3d->setInfiniteLineType(OdDg::kInfiniteLine);
    pDgPolyline = pLine3d;
  }
  else
  {
    OdDgLine2dPtr pLine2d = OdDgLine2d::createObject();
    pLine2d->setDatabaseDefaults(owner->database());
    pLine2d->setStartPoint(ptStart.convert2d());
    pLine2d->setEndPoint(ptEnd.convert2d());
    pLine2d->setInfiniteLineType(OdDg::kInfiniteLine);
    pDgPolyline = pLine2d;
  }

  if (!pDgPolyline.isNull())
  {
    addElementToDatabase(pDgPolyline, owner);
    copyElementProperties(pDgPolyline, pDbXLine, owner->database(), true);
    appendOdDgElementToOwner(owner, pDgPolyline);
    OdDgnExportContext::addDgnElement(pDbXLine->objectId(), pDgPolyline->elementId());
  }
}

//---------------------------------------------------------------------------------------------------

void getDbMLineSegmentLengthsAndOffsets(const OdDbMline* pDbMLine, 
                                          OdUInt32 uSegmentIndex, 
                                            OdArray<double>& arrSegmentOffsets, 
                                              OdArray<double>& arrSegmentLengths)
{
  arrSegmentOffsets.clear();
  arrSegmentLengths.clear();

  OdUInt32 uEndSegmentIndex = uSegmentIndex + 1;

  if (uEndSegmentIndex >= (OdUInt32)pDbMLine->numVertices())
    uEndSegmentIndex = 0;

  OdGeVector3d vrDir = pDbMLine->axisAt(uSegmentIndex);

  if (!vrDir.isZeroLength())
    vrDir.normalize();

  OdGeVector3d vrPerp = vrDir;
  vrPerp.rotateBy(OdaPI2, pDbMLine->normal());

  OdMLSegmentArray arrStartParams;
  OdMLSegmentArray arrEndParams;

  pDbMLine->getParametersAt(uSegmentIndex, arrStartParams);
  pDbMLine->getParametersAt(uEndSegmentIndex, arrEndParams);

  OdGeVector3d vrStartMiter = pDbMLine->miterAt(uSegmentIndex);
  OdGeVector3d vrEndMiter   = pDbMLine->miterAt(uEndSegmentIndex);
  OdGePoint3d  ptBaseStart  = pDbMLine->vertexAt(uSegmentIndex);
  OdGePoint3d  ptBaseEnd    = pDbMLine->vertexAt(uEndSegmentIndex);

  for( OdUInt32 i = 0; i < arrStartParams.size(); i++ )
  {
    OdGePoint3d ptSegStart = ptBaseStart + vrStartMiter * arrStartParams[i].m_SegParams[0];
    OdGePoint3d ptSegEnd   = ptBaseEnd + vrEndMiter * arrEndParams[i].m_SegParams[0];
    OdGePoint3d ptSegBase;

    double dSegLength = ptSegStart.distanceTo(ptSegEnd);
    double dSegOffset = 0.0;

    if (!vrPerp.isZeroLength())
    {
      OdGeLine3d lineSeg(ptSegStart, ptSegEnd);
      OdGeLine3d linePerp(ptBaseStart, vrPerp);

      if (!lineSeg.intersectWith(linePerp, ptSegBase))
        ptSegBase = ptSegStart;

      dSegOffset = ptSegStart.distanceTo(ptSegBase);

      if (!OdZero(dSegOffset))
      {
        OdGeVector3d vrOffsetDir = ptSegStart - ptSegBase;

        if (!vrOffsetDir.isCodirectionalTo(vrDir))
          dSegOffset *= -1;
      }
    }

    arrSegmentOffsets.push_back(dSegOffset);
    arrSegmentLengths.push_back(dSegLength);
  }
}

//---------------------------------------------------------------------------------------------------

void OdDbMLineExportPE::subExportElement(OdDbEntity* pDbEntity, OdDgElement* owner)
{
  OdDbMline* pDbMLine = (OdDbMline*)(pDbEntity);
  OdDbObjectId idMLineStyle = pDbMLine->style();

  if( idMLineStyle.isNull() )
  {
    OdDgnExportPE::subExportElement(pDbEntity, owner);
    return;
  }

  OdDbMlineStylePtr pDbMLineStyle = idMLineStyle.openObject(OdDb::kForRead);

  OdDgMultilinePtr pDgMLine = OdDgMultiline::createObject();

  if( pDbMLineStyle->filled() )
    pDgMLine->setFillColor(getDgnColorIndex(pDbMLineStyle->fillColor(), owner->database()));

  double    dOffset;
  OdCmColor clrColor;
  OdDbObjectId idDbLineType;

  for (OdInt32 i = 0; i < pDbMLineStyle->numElements(); i++)
  {
    pDbMLineStyle->getElementAt(i, dOffset, clrColor, idDbLineType);

    OdDgMultilineProfile   dgMLineProfile;
    OdDgMultilineSymbology dgMLineSymbology;
    dgMLineSymbology.setColorIndex(getDgnColorIndex(clrColor, owner->database()));
    dgMLineSymbology.setLineStyleEntryId(getDgnLineStyleEntryId(idDbLineType));
    dgMLineSymbology.setUseColorFlag(true);
    dgMLineSymbology.setUseStyleFlag(true);

    dgMLineProfile.setDistance(dOffset);
    dgMLineProfile.setSymbology(dgMLineSymbology);

    pDgMLine->addProfile(dgMLineProfile);
  }

  pDgMLine->setOriginCapAngle(pDbMLineStyle->startAngle());
  pDgMLine->setEndCapAngle(pDbMLineStyle->endAngle());

  OdDgMultilineSymbology symbStartCap;
  symbStartCap.setCapColorFromSegmentFlag(true);

  if( !pDbMLine->supressStartCaps() )
  {
    symbStartCap.setCapLineFlag(pDbMLineStyle->startSquareCap());
    symbStartCap.setCapOutArcFlag(pDbMLineStyle->startRoundCap());
    symbStartCap.setCapInArcFlag(pDbMLineStyle->startInnerArcs());
  }
  else
  {
    symbStartCap.setCapLineFlag(false);
    symbStartCap.setCapOutArcFlag(false);
    symbStartCap.setCapInArcFlag(false);
  }

  pDgMLine->setOriginCap(symbStartCap);

  OdDgMultilineSymbology symbEndCap;
  symbEndCap.setCapColorFromSegmentFlag(true);

  if (!pDbMLine->supressEndCaps())
  {
    symbEndCap.setCapLineFlag(pDbMLineStyle->endSquareCap());
    symbEndCap.setCapOutArcFlag(pDbMLineStyle->endRoundCap());
    symbEndCap.setCapInArcFlag(pDbMLineStyle->endInnerArcs());
  }
  else
  {
    symbEndCap.setCapLineFlag(false);
    symbEndCap.setCapOutArcFlag(false);
    symbEndCap.setCapInArcFlag(false);
  }

  pDgMLine->setEndCap(symbEndCap);

  OdDgMultilineSymbology symbMidCap;
  symbMidCap.setCapColorFromSegmentFlag(true);
  symbMidCap.setCapLineFlag(pDbMLineStyle->showMiters());

  pDgMLine->setMiddleCap(symbMidCap);

  if (pDbMLine->justification() == Mline::kZero)
    pDgMLine->setOffsetMode(OdDgMultiline::kByCenter);
  else if (pDbMLine->justification() == Mline::kTop)
    pDgMLine->setOffsetMode(OdDgMultiline::kByMaximum);
  else
    pDgMLine->setOffsetMode(OdDgMultiline::kByMinimum);

  pDgMLine->setOffsetIsValidFlag(true);

  OdArray<double> arrSegmentOffsets;
  OdArray<double> arrSegmentLengths;

  for (OdInt32 i = 0; i < pDbMLine->numVertices(); i++)
  {
    OdMLSegmentArray arrSegments;
    pDbMLine->getParametersAt(i, arrSegments);

    bool bHasBreaks = false;

    if( (i != pDbMLine->numVertices() - 1) || pDbMLine->closedMline() )
    {
      for (OdUInt32 k = 0; k < arrSegments.size(); k++)
      {
        if( (arrSegments[k].m_SegParams.size() > 2) || !OdZero(arrSegments[k].m_SegParams[1]) )
        {
          bHasBreaks = true;
          break;
        }
      }
    }

    OdDgMultilinePoint ptMLine;
    ptMLine.setPoint(pDbMLine->vertexAt(i));

    if( bHasBreaks )
    {
      getDbMLineSegmentLengthsAndOffsets(pDbMLine, i, arrSegmentOffsets, arrSegmentLengths);

      for( OdUInt32 k = 0; k < arrSegments.size(); k++ )
      {
        if( (arrSegments[k].m_SegParams.size() > 2) || !OdZero(arrSegments[k].m_SegParams[1]) )
        {
          OdUInt32 uMask = 1;
          uMask = uMask << k;

          if( !OdZero(arrSegments[k].m_SegParams[1]) )
          {
            double dBreakLength = arrSegments[k].m_SegParams[1] + arrSegmentOffsets[k];

            OdDgMultilineBreak curBreak;
            curBreak.setLinesMask(uMask);
            curBreak.setFlags(OdDgMultilineBreak::kFromJoint);
            curBreak.setOffset(0);
            curBreak.setLength(dBreakLength);
            ptMLine.addBreak(curBreak);
          }

          for( OdUInt32 j = 2; j < arrSegments[k].m_SegParams.size(); j += 2 )
          {
            if( j < arrSegments[k].m_SegParams.size() - 1)
            {
              OdDgMultilineBreak curBreak;
              curBreak.setLinesMask(uMask);

              double dBreakLength = arrSegments[k].m_SegParams[j + 1] - arrSegments[k].m_SegParams[j];

              if (OdZero(dBreakLength))
                continue;
              
              if( OdZero(arrSegments[k].m_SegParams[j]) )
              {
                curBreak.setFlags(OdDgMultilineBreak::kFromJoint);
                curBreak.setLength(dBreakLength);
              }
              else if (OdEqual(arrSegments[k].m_SegParams[j + 1], arrSegmentLengths[k]))
              {
                curBreak.setFlags(OdDgMultilineBreak::kToJoint);
                curBreak.setLength(dBreakLength);
              }
              else
              {
                curBreak.setFlags(OdDgMultilineBreak::kStandardByDistance);
                curBreak.setOffset(arrSegments[k].m_SegParams[j] + arrSegmentOffsets[k]);
                curBreak.setLength(dBreakLength);
              }

              ptMLine.addBreak(curBreak);
            }
          }

          if( arrSegments[k].m_SegParams.size() % 2 != 0 )
          {
            double dBreakLength = arrSegmentLengths[k] - arrSegments[k].m_SegParams.last();

            if( !OdZero(dBreakLength) )
            {
              OdDgMultilineBreak curBreak;
              curBreak.setLinesMask(uMask);
              curBreak.setFlags(OdDgMultilineBreak::kToJoint);
              curBreak.setOffset(arrSegments[k].m_SegParams.last() + arrSegmentOffsets[k]);
              curBreak.setLength(dBreakLength);
              ptMLine.addBreak(curBreak);
            }
          }
        }
      }
    }

    pDgMLine->addPoint(ptMLine);
  }

  if( pDbMLine->closedMline() )
  {
    if( !pDbMLine->vertexAt(0).isEqualTo(pDbMLine->vertexAt(pDbMLine->numVertices() - 1)))
    {
      OdDgMultilinePoint ptMLine;
      ptMLine.setPoint(pDbMLine->vertexAt(0));
      pDgMLine->addPoint(ptMLine);
    }
  }

  pDgMLine->setClosedFlag(pDbMLine->closedMline());

  if (!pDgMLine.isNull())
  {
    addElementToDatabase(pDgMLine, owner);
    copyElementProperties(pDgMLine, pDbMLine, owner->database(), true);
    appendOdDgElementToOwner(owner, pDgMLine);
    OdDgnExportContext::addDgnElement(pDbMLine->objectId(), pDgMLine->elementId());
  }
}

//---------------------------------------------------------------------------------------------------

OdDgGraphicsElementPtr getPointBlockRef(OdDbPoint* pDbPoint, OdDgDatabase* pDb)
{
  OdDgElementId idPointBlockDef = OdDgnExportContext::getPointBlockId();

  if( idPointBlockDef.isNull() )
  {
    OdDgSharedCellDefinitionTablePtr pSharedCellDefTable = pDb->getSharedCellDefinitionTable(OdDg::kForWrite);

    OdString strPointDefName = L"POINT_DISPLAY";

    if( !pSharedCellDefTable->getAt(strPointDefName).isNull() )
    {
      idPointBlockDef = pSharedCellDefTable->getAt(strPointDefName);
    }
    else
    {
      short   mode = pDbPoint->database()->getPDMODE();
      double  size = pDbPoint->database()->getPDSIZE();

      OdDgSharedCellDefinitionPtr pDgPointDef = OdDgSharedCellDefinition::createObject();
      pDgPointDef->setDatabaseDefaults(pDb);

      OdGePoint3d verts[5];

      if (mode >= 64)
      {
        OdDgLineString2dPtr pSquare = OdDgLineString2d::createObject();
        pSquare->setDatabaseDefaults(pDb);
        pSquare->addVertex(OdGePoint2d(-size / 2.0, -size / 2.0));
        pSquare->addVertex(OdGePoint2d(-size / 2.0, size / 2.0));
        pSquare->addVertex(OdGePoint2d(size / 2.0, size / 2.0));
        pSquare->addVertex(OdGePoint2d(size / 2.0, -size / 2.0));
        pSquare->addVertex(OdGePoint2d(-size / 2.0, -size / 2.0));
        pSquare->setLineStyleEntryId(0);
        pSquare->setColorIndex(OdDg::kColorByCell);
        pSquare->setLineWeight(OdDg::kLineWeightByCell);
        pSquare->setLevelEntryId(0);

        pDgPointDef->add(pSquare);

        mode -= 64;
      }

      if (mode >= 32)
      {
        OdDgArc2dPtr pArc = OdDgArc2d::createObject();
        pArc->setDatabaseDefaults(pDb);
        pArc->setOrigin(OdGePoint2d::kOrigin);
        pArc->setPrimaryAxis(size / 2.0);
        pArc->setSecondaryAxis(size / 2.0);
        pArc->setRotationAngle(0.0);
        pArc->setStartAngle(0.0);
        pArc->setSweepAngle(Oda2PI);
        pArc->setLineStyleEntryId(0);
        pArc->setColorIndex(OdDg::kColorByCell);
        pArc->setLineWeight(OdDg::kLineWeightByCell);
        pArc->setLevelEntryId(0);

        pDgPointDef->add(pArc);

        mode -= 32;
      }

      switch (mode)
      {
        case 0: // point
        {
          OdDgLine2dPtr pLine = OdDgLine2d::createObject();
          pLine->setDatabaseDefaults(pDb);
          pLine->setStartPoint(OdGePoint2d::kOrigin);
          pLine->setEndPoint(OdGePoint2d::kOrigin);
          pLine->setLineStyleEntryId(0);
          pLine->setColorIndex(OdDg::kColorByCell);
          pLine->setLineWeight(OdDg::kLineWeightByCell);
          pLine->setLevelEntryId(0);

          pDgPointDef->add(pLine);
        }
        break;

        case 2: // vertical cross
        {
          OdDgLine2dPtr pLine = OdDgLine2d::createObject();
          pLine->setDatabaseDefaults(pDb);
          pLine->setStartPoint(OdGePoint2d(-size, 0));
          pLine->setEndPoint(OdGePoint2d(size, 0));
          pLine->setLineStyleEntryId(0);
          pLine->setColorIndex(OdDg::kColorByCell);
          pLine->setLineWeight(OdDg::kLineWeightByCell);
          pLine->setLevelEntryId(0);

          pDgPointDef->add(pLine);

          pLine = OdDgLine2d::createObject();
          pLine->setDatabaseDefaults(pDb);
          pLine->setStartPoint(OdGePoint2d(0, -size));
          pLine->setEndPoint(OdGePoint2d(0, size));
          pLine->setLineStyleEntryId(0);
          pLine->setColorIndex(OdDg::kColorByCell);
          pLine->setLineWeight(OdDg::kLineWeightByCell);
          pLine->setLevelEntryId(0);

          pDgPointDef->add(pLine);
        }
        break;

        case 3: // diagonal cross
        {
          double sqrt2 = sqrt(2.0);

          OdDgLine2dPtr pLine = OdDgLine2d::createObject();
          pLine->setDatabaseDefaults(pDb);
          pLine->setStartPoint(OdGePoint2d(-size * sqrt2 / 2.0, -size * sqrt2 / 2.0));
          pLine->setEndPoint(OdGePoint2d(size*sqrt2 / 2.0, size*sqrt2 / 2.0));
          pLine->setLineStyleEntryId(0);
          pLine->setColorIndex(OdDg::kColorByCell);
          pLine->setLineWeight(OdDg::kLineWeightByCell);
          pLine->setLevelEntryId(0);

          pDgPointDef->add(pLine);

          pLine = OdDgLine2d::createObject();
          pLine->setDatabaseDefaults(pDb);
          pLine->setStartPoint(OdGePoint2d(-size * sqrt2 / 2.0, size*sqrt2 / 2.0));
          pLine->setEndPoint(OdGePoint2d(size*sqrt2 / 2.0, -size * sqrt2 / 2.0));
          pLine->setLineStyleEntryId(0);
          pLine->setColorIndex(OdDg::kColorByCell);
          pLine->setLineWeight(OdDg::kLineWeightByCell);
          pLine->setLevelEntryId(0);

          pDgPointDef->add(pLine);
        }
        break;

        case 4: // small vertical line
        {
          OdDgLine2dPtr pLine = OdDgLine2d::createObject();
          pLine->setDatabaseDefaults(pDb);
          pLine->setStartPoint(OdGePoint2d::kOrigin);
          pLine->setEndPoint(OdGePoint2d(0.0, size / 2.0));
          pLine->setLineStyleEntryId(0);
          pLine->setColorIndex(OdDg::kColorByCell);
          pLine->setLineWeight(OdDg::kLineWeightByCell);
          pLine->setLevelEntryId(0);

          pDgPointDef->add(pLine);
        }
        break;
      }

      pDgPointDef->transformBy(OdGeMatrix3d::scaling(OdDgnExportContext::getMasterToUORsScale()));

      pDgPointDef->setName(strPointDefName);
      pSharedCellDefTable->add(pDgPointDef);

      idPointBlockDef = pDgPointDef->elementId();
    }

    OdDgnExportContext::setPointBlockId(idPointBlockDef);
  }

  OdDgGraphicsElementPtr pDgPoint;

  if( !idPointBlockDef.isNull() )
  {
    OdDgSharedCellReferencePtr pDgPointRef = OdDgSharedCellReference::createObject();
    pDgPointRef->setDefinitionName(L"POINT_DISPLAY");
    pDgPointRef->setColorOverrideFlag(true);
    pDgPointRef->setWeightOverrideFlag(true);
    pDgPointRef->setLevelOverrideFlag(true);
    pDgPointRef->setOrigin(pDbPoint->position());

    OdGeMatrix3d ocs2wcs(OdGeMatrix3d::planeToWorld(pDbPoint->normal()));
    OdGeMatrix3d rot(OdGeMatrix3d::rotation(-pDbPoint->ecsRotation(), OdGeVector3d::kZAxis));

    OdGeMatrix3d matTransform = ocs2wcs * rot * OdGeMatrix3d::scaling(1.0 / OdDgnExportContext::getMasterToUORsScale());

    pDgPointRef->setTransformation(matTransform);

    pDgPoint = pDgPointRef;
  }

  return pDgPoint;
}

//---------------------------------------------------------------------------------------------------

void OdDbPointExportPE::subExportElement(OdDbEntity* pDbEntity, OdDgElement* owner)
{
  OdDbPoint* pDbPoint = (OdDbPoint*)(pDbEntity);

  short pdmode = pDbEntity->database()->getPDMODE();

  if (pDbEntity->layerId() == pDbEntity->database()->getLayerDefpointsId())
    pdmode = 0;

  OdDgGraphicsElementPtr pDgPoint;

  if( pdmode == 0 )
  {
    OdGePoint3d p1, p2;
    p1 = pDbPoint->position();

    if (OdZero(pDbPoint->thickness()))
      p2 = p1 + pDbPoint->normal()*pDbPoint->thickness();
    else
      p2 = p1;

    pDgPoint = createDgLine(p1, p2, owner->database());
  }
  else
  {
    pDgPoint = getPointBlockRef(pDbPoint, owner->database());
  }

  if( !pDgPoint.isNull() )
  {
    addElementToDatabase(pDgPoint, owner);
    copyElementProperties(pDgPoint, pDbPoint, owner->database());
    appendOdDgElementToOwner(owner, pDgPoint);
  }

  OdDgnExportContext::addDgnElement(pDbPoint->objectId(), pDgPoint->elementId());
}

//---------------------------------------------------------------------------------------------------

}
