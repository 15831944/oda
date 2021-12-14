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
#include "DgnExportPolygon.h"
#include "DgnExportCurve.h"
#include "DgnExportCommon.h"
#include "DgnExportContext.h"
#include <DbSolid.h>
#include <DbRegion.h>
#include <DgShape.h>
#include <DgArc.h>
#include <DgEllipse.h>
#include <DgLineString.h>
#include <DbPolyline.h>
#include <DgCellHeader.h>
#include <DgComplexShape.h>
#include <DgComplexString.h>
#include <DbSolid.h>
#include <DbFace.h>
#include <DgBSplineCurve.h>
#include <Ge/GeCircArc2d.h>
#include <Ge/GeLineSeg3d.h>
#include <Ge/GeLineSeg2d.h>
#include <Ge/GePolyline3d.h>
#include <Ge/GeCompositeCurve3d.h>
#include <Br/BrBrep.h>
#include <Br/BrBrepFaceTraverser.h>
#include <Br/BrFaceLoopTraverser.h>
#include <Br/BrBrepEdgeTraverser.h>
#include <Br/BrLoopEdgeTraverser.h>

namespace TD_DGN_EXPORT {

  //===================================================================================================
  
  void OdDbSolidExportPE::subExportElement(OdDbEntity* pDbEntity, OdDgElement* owner)
  {
    OdDbSolid* pDbSolid = (OdDbSolid*)(pDbEntity);

    OdGePoint3dArray arrPts;
    OdGePoint3d ptCur;
    pDbSolid->getPointAt(0, ptCur);
    arrPts.push_back(ptCur);

    pDbSolid->getPointAt(1, ptCur);

    if (!ptCur.isEqualTo(arrPts.last()))
      arrPts.push_back(ptCur);

    pDbSolid->getPointAt(3, ptCur);

    if (!ptCur.isEqualTo(arrPts.last()))
      arrPts.push_back(ptCur);

    pDbSolid->getPointAt(2, ptCur);

    if (!ptCur.isEqualTo(arrPts.last()))
      arrPts.push_back(ptCur);

    if( arrPts.size() > 2 )
    {
      OdDgGraphicsElementPtr pDgShape = createDgShape(arrPts, pDbEntity, owner->database());

      if( !pDgShape.isNull() )
      {
        pDgShape->setThickness(pDbSolid->thickness());
        addElementToDatabase(pDgShape, owner);
        copyElementProperties(pDgShape, pDbSolid, owner->database());
        appendOdDgElementToOwner(owner, pDgShape);
        pDgShape->setFillColor(pDgShape->getColorIndex());

        OdDgnExportContext::addDgnElement(pDbSolid->objectId(), pDgShape->elementId());
      }
    }
  }

  //===================================================================================================

  void OdDbFaceExportPE::subExportElement(OdDbEntity* pDbEntity, OdDgElement* owner)
  {
    OdDbFace* pDbFace = (OdDbFace*)(pDbEntity);

    OdGePoint3dArray arrPts;
    OdArray<bool>    arrVisibility;
    OdGePoint3d ptCur;
    pDbFace->getVertexAt(0, ptCur);
    arrPts.push_back(ptCur);
    arrVisibility.push_back(pDbFace->isEdgeVisibleAt(0));

    bool bCreateMesh = !arrVisibility.last();

    for (OdUInt32 i = 1; i < 4; i++)
    {
      pDbFace->getVertexAt(i, ptCur);

      arrVisibility.push_back(pDbFace->isEdgeVisibleAt(i));

      if (!ptCur.isEqualTo(arrPts.last()))
      {
        arrPts.push_back(ptCur);

        if (!arrVisibility.last())
          bCreateMesh = true;
      }
    }

    if (arrPts.size() > 2)
    {
      OdDgGraphicsElementPtr pDgShape;

      if( bCreateMesh )
      {
        if( OdDgnExportContext::is3d() )
        {
          OdDgMeshFaceLoopsPtr pMesh = OdDgMeshFaceLoops::createObject();
          pMesh->setDatabaseDefaults(owner->database());

          OdDgMeshFaceLoops::VerticesArray faceData;

          for (OdUInt32 i = 0; i < arrPts.size(); i++)
          {
            OdDgMeshFaceLoops::FacePoint curPoint;
            curPoint.m_pointIndex = i;
            curPoint.m_impliesVisibleEdge = arrVisibility[i];
            faceData.push_back(curPoint);

            pMesh->addPoint(arrPts[i]);
          }

          pMesh->addFace(faceData);
          pDgShape = pMesh;
        }
        else
        {
          OdDgLineString2dPtr pLineString = OdDgLineString2d::createObject();
          pLineString->setDatabaseDefaults(owner->database());

          bool bLastIsBreak = true;

          arrPts.push_back(arrPts.first());

          for( OdUInt32 i = 0; i < arrPts.size()-1; i++ )
          {
            if( !arrVisibility[i] )
            {
              if (bLastIsBreak)
                continue;
              else if( i != arrPts.size() - 2 )
                pLineString->addVertex(OdDgLineString2d::getBreakLineValue());

              bLastIsBreak = true;
            }
            else
            {
              if (bLastIsBreak)
              {
                pLineString->addVertex(arrPts[i].convert2d());
                pLineString->addVertex(arrPts[i + 1].convert2d());
              }
              else
                pLineString->addVertex(arrPts[i + 1].convert2d());

              bLastIsBreak = false;
            }
          }

          pDgShape = pLineString;
        }
      }
      else
      {
        pDgShape = createDgShape(arrPts, pDbEntity, owner->database());
      }

      if (!pDgShape.isNull())
      {
        addElementToDatabase(pDgShape, owner);
        copyElementProperties(pDgShape, pDbFace, owner->database());
        appendOdDgElementToOwner(owner, pDgShape);

        OdDgnExportContext::addDgnElement(pDbFace->objectId(), pDgShape->elementId());
      }
    }
  }

  //===================================================================================================

  bool allowToCreateSimpleCurve(OdArray<OdDgGraphicsElementPtr>& arrCurveItems)
  {
    bool bRet = true;

    for( OdUInt32 i = 0; i < arrCurveItems.size(); i++ )
    {
      if( arrCurveItems[i].isNull() )
        continue;

      if( arrCurveItems[i]->getElementType() == OdDgElement::kTypeLine )
        continue;

      if( arrCurveItems[i]->getElementType() == OdDgElement::kTypeLineString )
        continue;

      if( arrCurveItems[i]->isKindOf(OdDgComplexCurve::desc()) )
      {
        OdDgComplexCurvePtr pCplxCurve = arrCurveItems[i];

        OdArray<OdDgGraphicsElementPtr> arrSubItems;

        OdDgElementIteratorPtr pIter = pCplxCurve->createIterator();

        for(; !pIter->done(); pIter->step())
        {
          OdDgElementPtr pItem = pIter->item().openObject(OdDg::kForRead);

          if( pItem->isKindOf(OdDgGraphicsElement::desc()) )
            arrSubItems.push_back(pItem);
        }

        if( allowToCreateSimpleCurve(arrSubItems) )
          continue;
      }

      bRet = false;
      break;
    }

    return bRet;
  }

  //===================================================================================================

  OdDgGraphicsElementPtr createSimpleCurve(OdArray<OdDgGraphicsElementPtr>& arrCurveItems, 
                                             OdDgDatabase* pDb, 
                                               const OdDbEntityPtr& pEntityToInit, 
                                                 bool bForceClose)
  {
    OdDgGraphicsElementPtr pRet;

    OdGePoint3dArray arrSimplePts = getShapePtsFromSetOfDgnCurves(arrCurveItems);

    if( arrSimplePts.size() > 1 )
    {
      if (bForceClose && arrSimplePts.first().isEqualTo(arrSimplePts.last()))
        arrSimplePts.push_back(arrSimplePts.first());

      if (bForceClose)
        pRet = createDgShape(arrSimplePts, pEntityToInit, pDb);
      else
        pRet = createDgLineString(arrSimplePts, pEntityToInit, pDb);
    }

    return pRet;
  }

  //===================================================================================================

  void appendCurveToLoop(OdArray<OdDgGraphicsElementPtr>& arrLoopItems, OdDgGraphicsElementPtr& pCurve)
  {
    if( arrLoopItems.isEmpty() )
      arrLoopItems.push_back(pCurve);
    else if (arrLoopItems.size() == 1)
    {
      OdGePoint3d ptStart1, ptEnd1, ptStart2, ptEnd2;
      getDgnCurveStartEndPoint(arrLoopItems[0], ptStart1, ptEnd1);
      getDgnCurveStartEndPoint(pCurve, ptStart2, ptEnd2);

      double dStart1End2 = ptStart1.distanceTo(ptEnd2);
      double dStart1Start2 = ptStart1.distanceTo(ptStart2);

      double dEnd1End2 = ptEnd1.distanceTo(ptEnd2);
      double dEnd1Start2 = ptEnd1.distanceTo(ptStart2);

      double dStartDistance = dStart1Start2 > dStart1End2 ? dStart1End2 : dStart1Start2;
      double dEndDistance = dEnd1Start2 > dEnd1End2 ? dEnd1End2 : dEnd1Start2;

      if (dStartDistance < dEndDistance)
      {
        reverseDgnCurve(arrLoopItems[0]);

        if (dStart1Start2 > dStart1End2)
          reverseDgnCurve(pCurve);

        arrLoopItems.push_back(pCurve);
      }
      else
      {
        if (dEnd1Start2 > dEnd1End2)
          reverseDgnCurve(pCurve);

        arrLoopItems.push_back(pCurve);
      }
    }
    else
    {
      OdGePoint3d ptStart1, ptEnd1, ptStart2, ptEnd2;
      getDgnCurveStartEndPoint(arrLoopItems.last(), ptStart1, ptEnd1);
      getDgnCurveStartEndPoint(pCurve, ptStart2, ptEnd2);

      double dEnd1End2 = ptEnd1.distanceTo(ptEnd2);
      double dEnd1Start2 = ptEnd1.distanceTo(ptStart2);

      if (dEnd1Start2 > dEnd1End2)
        reverseDgnCurve(pCurve);

      arrLoopItems.push_back(pCurve);
    }
  }

  //===================================================================================================

  void appendComplexCurveToLoop(OdArray<OdDgGraphicsElementPtr>& arrLoopItems, OdDgComplexCurvePtr& pCurve)
  {
    OdDgElementIteratorPtr pIter = pCurve->createIterator();

    for(; !pIter->done(); pIter->step() )
    {
      OdDgGraphicsElementPtr pGrItem = pIter->item().openObject(OdDg::kForWrite);

      if (pGrItem.isNull())
        continue;

      if (pGrItem->isKindOf(OdDgComplexCurve::desc()))
      {
        OdDgComplexCurvePtr pCplxCurve = pGrItem;
        appendComplexCurveToLoop(arrLoopItems, pCplxCurve);
      }
      else
        appendCurveToLoop(arrLoopItems, pGrItem);
    }
  }

  //===================================================================================================

  bool getDgArcData(const OdDgGraphicsElementPtr& pCurve , 
                      OdGePoint3d& ptCenter, 
                        double& dRadius, 
                          OdGePoint3d& ptStart, 
                            OdGePoint3d& ptEnd, 
                              double& dSweepAngle)
  {
    bool bRet = false;

    if( pCurve->isKindOf(OdDgArc2d::desc()) )
    {
      OdDgArc2dPtr pArc = pCurve;

      if( OdEqual(pArc->getPrimaryAxis(), pArc->getSecondaryAxis()) )
      {
        bRet = true;
        OdGePoint2d ptCenter2d = pArc->getOrigin();
        OdGePoint2d ptStart2d, ptEnd2d;
        pArc->getStartPoint(ptStart2d);
        pArc->getEndPoint(ptEnd2d);
        ptCenter.set(ptCenter2d.x, ptCenter2d.y, 0.0);
        ptStart.set(ptStart2d.x, ptStart2d.y, 0.0);
        ptEnd.set(ptEnd2d.x, ptEnd2d.y, 0.0);
        dRadius = pArc->getPrimaryAxis();
        dSweepAngle = pArc->getSweepAngle();
      }
    }
    else if (pCurve->isKindOf(OdDgArc3d::desc()))
    {
      OdDgArc3dPtr pArc = pCurve;

      if (OdEqual(pArc->getPrimaryAxis(), pArc->getSecondaryAxis()))
      {
        bRet = true;
        ptCenter = pArc->getOrigin();
        pArc->getStartPoint(ptStart);
        pArc->getEndPoint(ptEnd);
        dRadius = pArc->getPrimaryAxis();
        dSweepAngle = pArc->getSweepAngle();
      }
    }

    return bRet;
  }

  //===================================================================================================

  void connectEdgeArcs(OdArray<OdDgGraphicsElementPtr>& arrCurveItems)
  {
    bool bPreviousIsArc = false;
    OdGePoint3d ptPrevCenter;
    double      dPrevRadius;
    double      dPrevSweepAngle;
    OdGePoint3d ptPrevStart;
    OdGePoint3d ptPrevEnd;

    for( OdUInt32 i = 0; i < arrCurveItems.size(); i++ )
    {
      if( arrCurveItems[i]->getElementType() == OdDgElement::kTypeArc )
      {
        if( bPreviousIsArc )
        {
          OdGePoint3d ptCurCenter;
          double      dCurRadius;
          double      dCurSweepAngle;
          OdGePoint3d ptCurStart;
          OdGePoint3d ptCurEnd;

          bool bCurIsArc = getDgArcData(arrCurveItems[i], ptCurCenter, dCurRadius, ptCurStart, ptCurEnd, dCurSweepAngle);

          if( bCurIsArc )
          {
            if( ptPrevCenter.isEqualTo(ptCurCenter) && (ptPrevEnd.isEqualTo(ptCurStart) || ptPrevEnd.isEqualTo(ptCurEnd)) )
            {
              double dNewAngle = dPrevSweepAngle + dCurSweepAngle;

              if( OdEqual(dNewAngle, Oda2PI) )
              {
                if (arrCurveItems[i - 1]->isKindOf(OdDgArc2d::desc()))
                {
                  OdDgArc2dPtr pArc2d = arrCurveItems[i - 1];
                  OdDgEllipse2dPtr pEllipse = OdDgEllipse2d::createObject();
                  pEllipse->setOrigin(pArc2d->getOrigin());
                  pEllipse->setRotationAngle(pArc2d->getRotationAngle());
                  pEllipse->setPrimaryAxis(pArc2d->getPrimaryAxis());
                  pEllipse->setSecondaryAxis(pArc2d->getSecondaryAxis());
                  pEllipse->setPropertiesFrom(pArc2d);
                  arrCurveItems[i - 1] = pEllipse;
                }
                else if (arrCurveItems[i - 1]->isKindOf(OdDgArc3d::desc()))
                {
                  OdDgArc3dPtr pArc3d = arrCurveItems[i - 1];
                  OdDgEllipse3dPtr pEllipse = OdDgEllipse3d::createObject();
                  pEllipse->setOrigin(pArc3d->getOrigin());
                  pEllipse->setRotation(pArc3d->getRotation());
                  pEllipse->setPrimaryAxis(pArc3d->getPrimaryAxis());
                  pEllipse->setSecondaryAxis(pArc3d->getSecondaryAxis());
                  pEllipse->setPropertiesFrom(pArc3d);
                  arrCurveItems[i - 1] = pEllipse;
                }

                bPreviousIsArc = false;
              }
              else
              {
                if( arrCurveItems[i-1]->isKindOf(OdDgArc2d::desc()) )
                {
                  OdDgArc2dPtr pArc2d = arrCurveItems[i - 1];
                  pArc2d->setSweepAngle(dNewAngle);
                }
                else if (arrCurveItems[i - 1]->isKindOf(OdDgArc3d::desc()))
                {
                  OdDgArc3dPtr pArc3d = arrCurveItems[i - 1];
                  pArc3d->setSweepAngle(dNewAngle);
                }
              }

              arrCurveItems.removeAt(i);
              i--;
            }
          }
          else
            bPreviousIsArc = false;
        }
        else
        {
          bPreviousIsArc = getDgArcData(arrCurveItems[i], ptPrevCenter, dPrevRadius, ptPrevStart, ptPrevEnd, dPrevSweepAngle);
        }
      }
      else
        bPreviousIsArc = false;
    }
  }

  //===================================================================================================

  OdDgGraphicsElementPtr createComplexCurve(OdArray<OdDgGraphicsElementPtr>& arrCurveItems, 
                                              OdDgDatabase* pDb,
                                                OdDbEntity* pEntityToInit,
                                                  bool bInitCurve,
                                                    bool bForceClose)
  {
    OdDgGraphicsElementPtr pRet;

    OdArray<OdDgGraphicsElementPtr> arrLoopItems;

    for( OdUInt32 i = 0; i < arrCurveItems.size(); i++ )
    {
      if (arrCurveItems[i]->isKindOf(OdDgComplexCurve::desc()))
      {
        OdDgComplexCurvePtr pCplxCurve = arrCurveItems[i];
        appendComplexCurveToLoop(arrLoopItems, pCplxCurve);
      }
      else
        appendCurveToLoop(arrLoopItems, arrCurveItems[i]);
    }

    if( bForceClose )
    {
      OdGePoint3d ptStart1, ptEnd1, ptStart2, ptEnd2;
      getDgnCurveStartEndPoint(arrLoopItems.first(), ptStart1, ptEnd1);
      getDgnCurveStartEndPoint(arrLoopItems.last(), ptStart2, ptEnd2);

      if( !ptStart1.isEqualTo(ptEnd2) )
      {
        OdDgGraphicsElementPtr pDgLine = createDgLine(ptEnd2, ptStart1, pDb);
        arrLoopItems.push_back(pDgLine);
      }
    }

    OdDgComplexCurvePtr pCplxRet;

    if( bForceClose )
      pCplxRet = OdDgComplexShape::createObject();
    else
      pCplxRet = OdDgComplexString::createObject();

    pCplxRet->setDatabaseDefaults(pDb);

    for (OdUInt32 j = 0; j < arrLoopItems.size(); j++)
    {
      if (bInitCurve)
        copyElementProperties(arrLoopItems[j], pEntityToInit, pDb);

      pCplxRet->add(arrLoopItems[j]);
    }

    pRet = pCplxRet;

    return pRet;
  }

  //===================================================================================================

  OdDgGraphicsElementPtr connectSubCurves(OdArray<OdDgGraphicsElementPtr>& arrCurveItems, 
                                            OdDgDatabase* pDb, 
                                              OdDbEntity* pEntityToInit,
                                                bool bInitCurve, 
                                                  bool bForceClose)
  {
    OdDgGraphicsElementPtr pRet;

    connectEdgeArcs(arrCurveItems);

    if (arrCurveItems.size() == 1)
    {
      if( bForceClose )
      {
        if( !isClosedCurve(arrCurveItems[0]))
        {
          OdGePoint3d ptStart, ptEnd;
          getDgnCurveStartEndPoint(arrCurveItems[0], ptStart, ptEnd);
          OdDgGraphicsElementPtr pLine = createDgLine(ptStart, ptEnd, pDb);
          arrCurveItems.push_back(pLine);
        }
        else
          pRet = arrCurveItems[0];
      }
      else
        pRet = arrCurveItems[0];
    }
    
    if(arrCurveItems.size() > 1 )
    {
      if (allowToCreateSimpleCurve(arrCurveItems))
        pRet = createSimpleCurve(arrCurveItems, pDb, pEntityToInit, bForceClose);
      else
        pRet = createComplexCurve(arrCurveItems, pDb, pEntityToInit, bInitCurve, bForceClose);
    }

    return pRet;
  }

  //===================================================================================================

  OdDgGraphicsElementPtr createDgEdgeFromGeCurve( OdGeCurve3d* pGeCurve, 
                                                    OdGeVector3d& vrNormal, 
                                                      OdDgDatabase* pDb,
                                                        OdDbEntity* pEntityToInit,
                                                          bool bInitCurve)
  {
    OdDgGraphicsElementPtr pRet;

    if( !pGeCurve )
      return pRet;

    switch( pGeCurve->type() )
    {
      case OdGe::kLineSeg3d:
      case OdGe::kCircArc3d:
      case OdGe::kEllipArc3d:
      case OdGe::kNurbCurve3d:
      {
        pRet = createDgCurveFromGeCurve(pGeCurve, vrNormal, pDb, true);
      } break;

      case OdGe::kPolyline3d:
      case OdGe::kAugPolyline3d:
      {
        OdGePolyline3d* pGePolyline = (OdGePolyline3d*)(pGeCurve);

        OdGePoint3dArray arrPolylinePts;

        if( pGePolyline->hasFitData() )
        {
          for (OdInt32 i = 0; i < pGePolyline->numFitPoints(); i++)
          {
            OdGePoint3d curPt = pGePolyline->fitPointAt(i);

            if( arrPolylinePts.isEmpty() || !arrPolylinePts.last().isEqualTo(curPt) )
              arrPolylinePts.push_back( curPt );
          }
        }
        else
        {
          for (OdInt32 i = 0; i < pGePolyline->numControlPoints(); i++)
          {
            OdGePoint3d curPt = pGePolyline->controlPointAt(i);

            if (arrPolylinePts.isEmpty() || !arrPolylinePts.last().isEqualTo(curPt))
              arrPolylinePts.push_back(curPt);
          }
        }

        if (pGePolyline->isClosed() && !arrPolylinePts.first().isEqualTo(arrPolylinePts.last()))
          arrPolylinePts.push_back(arrPolylinePts.first());

        if (arrPolylinePts.size() > 1)
        {
          if (arrPolylinePts.first().isEqualTo(arrPolylinePts.last()))
            pRet = createDgShape(arrPolylinePts, pEntityToInit, pDb);
          else
            pRet = createDgLineString(arrPolylinePts, pEntityToInit, pDb);
        }
      } break;

      case OdGe::kCompositeCrv3d:
      {
        OdArray<OdDgGraphicsElementPtr> arrCurveItems;
        OdGeCompositeCurve3d* pGeCompositeCurve = (OdGeCompositeCurve3d*)(pGeCurve);

        OdGeCurve3dPtrArray arrSubCurves = pGeCompositeCurve->getCurveList();

        bool bCreateCurve = true;

        for( OdUInt32 i = 0; i < arrSubCurves.size(); i++ )
        {
          OdDgGraphicsElementPtr pDgItem = createDgEdgeFromGeCurve(arrSubCurves[i].get(), vrNormal, pDb, pEntityToInit, true);

          if (pDgItem.isNull())
          {
            bCreateCurve = false;
            break;
          }
          else
            arrCurveItems.push_back(pDgItem);
        }

        if( bCreateCurve )
          pRet = connectSubCurves(arrCurveItems, pDb, pEntityToInit, bInitCurve, false);
      } break;
    }

    if (!pRet.isNull() && bInitCurve)
      copyElementProperties(pRet, pEntityToInit, pDb);

    return pRet;
  }

  //===================================================================================================

  OdDgGraphicsElementPtr createEdgePolylineFromGeCurve(OdGeCurve3d* pGeCurve, 
                                                         OdGeVector3d& vrNormal, 
                                                           OdDbEntity* pEntityToInit,
                                                             OdDgDatabase* pDb)
  {
    OdDgGraphicsElementPtr pRet;

    if( !pGeCurve )
      return pRet;

    OdGePoint3dArray arrLengthPts;
    pGeCurve->getSamplePoints(20, arrLengthPts);

    double dLength = 0.0;

    for (OdUInt32 i = 0; i < arrLengthPts.size() - 1; i++)
      dLength += arrLengthPts[i].distanceTo(arrLengthPts[i + 1]);

    double dTolerance = dLength / 1000.0;

    OdGeInterval curveInterval;
    pGeCurve->getInterval(curveInterval);
    OdGePoint3dArray arrPolylinePts;
    pGeCurve->getSamplePoints(&curveInterval, dTolerance, arrPolylinePts);

    if(arrPolylinePts.first().isEqualTo(arrPolylinePts.last()) )
      pRet = createDgShape(arrPolylinePts, pEntityToInit, pDb);
    else
      pRet = createDgLineString(arrPolylinePts, pEntityToInit, pDb);

    return pRet;
  }

  //===================================================================================================

  OdDgGraphicsElementPtr createPlanarBrepGeometry(OdBrBrep& brRegion, 
                                                    OdGeVector3d& vrNormal, 
                                                      OdDgDatabase* pDb,
                                                        OdDbEntity* pBaseElm )
  {
    OdDgGraphicsElementPtr pDgRegion;

    OdGeMatrix3d matTransform;

    if (!brRegion.getTransformation(matTransform))
      matTransform = OdGeMatrix3d::kIdentity;

    OdBrBrepFaceTraverser brFaceTraverser;
    OdBrErrorStatus err = brFaceTraverser.setBrep(brRegion);

    if( err != odbrOK )
      return pDgRegion;

    OdArray<OdDgGraphicsElementPtr> arrDgLoops;

    for (; !brFaceTraverser.done(); brFaceTraverser.next())
    {
      // calc bbox
      OdBrFace brFace = brFaceTraverser.getFace();

      OdBrFaceLoopTraverser brLoopTraverser;

      err = brLoopTraverser.setFace(brFace);

      if( err != odbrOK )
        continue;

      for(; !brLoopTraverser.done(); brLoopTraverser.next() )
      {
        OdBrLoop brLoop = brLoopTraverser.getLoop();

        OdBrLoopEdgeTraverser brLoopEdgeTraverser;

        if( brLoopEdgeTraverser.setLoop(brLoop) != odbrOK)
          continue;

        OdArray<OdDgGraphicsElementPtr> arrDgLoop;

        bool bBreakLoop = false;

        while( !brLoopEdgeTraverser.done() )
        {
          OdBrEdge brEdge = brLoopEdgeTraverser.getEdge();

          OdDgGraphicsElementPtr pDgEdgeCurve;

          OdGeCurve3d* pGeCurve = brLoopEdgeTraverser.getOrientedCurve();

          // try to convert ge curve to nurb curve

          if( pGeCurve )
            pDgEdgeCurve = createDgEdgeFromGeCurve(pGeCurve, vrNormal, pDb, pBaseElm, false);

          // if nurb curve is "exotic" then try to convert it to nurb curve

          if( pDgEdgeCurve.isNull() )
          {
            OdGeNurbCurve3d geNurbCurve;

            if( brEdge.getCurveAsNurb(geNurbCurve) )
              pDgEdgeCurve = createDgCurveFromGeCurve(&geNurbCurve, vrNormal, pDb, true);
          }

          // if ge curve can't be converted to nurb curve then try to get polyline

          if( pDgEdgeCurve.isNull() && pGeCurve )
            pDgEdgeCurve = createEdgePolylineFromGeCurve(pGeCurve, vrNormal, pBaseElm, pDb);

          if( pGeCurve )
          {
            delete pGeCurve;
            pGeCurve = NULL;
          }

          // if get curve still not created then break the loop.

          if( pDgEdgeCurve.isNull() )
          {
            bBreakLoop = true;
            break;
          }

          arrDgLoop.push_back(pDgEdgeCurve);

          brLoopEdgeTraverser.next();
        }

        if( !arrDgLoop.isEmpty() )
        {
          OdDgGraphicsElementPtr pDgLoop = connectSubCurves(arrDgLoop, pDb, pBaseElm, true, true);

          if (!pDgLoop.isNull())
            arrDgLoops.push_back(pDgLoop);
        }
      }
    }

    if (!arrDgLoops.isEmpty())
    {
      if (arrDgLoops.size() == 1)
        pDgRegion = arrDgLoops[0];
      else
      {
        if (OdDgnExportContext::is3d())
          pDgRegion = OdDgCellHeader3d::createObject();
        else
          pDgRegion = OdDgCellHeader2d::createObject();

        pDgRegion->setDatabaseDefaults(pDb);
        pDgRegion->setLevelEntryId(0);

        for (OdUInt32 i = 0; i < arrDgLoops.size(); i++)
        {
          arrDgLoops[i]->setHbitFlag(i != 0);
          copyElementProperties(arrDgLoops[i], pBaseElm, pDb);
          appendOdDgElementToOwner(pDgRegion, arrDgLoops[i]);
        }
      }
    }

    if( !pDgRegion.isNull() && matTransform != OdGeMatrix3d::kIdentity )
    {
      pDgRegion->transformBy(matTransform);
    }

    return pDgRegion;
  }

  //===================================================================================================

  void OdDbRegionExportPE::subExportElement(OdDbEntity* pDbEntity, OdDgElement* owner)
  {
    OdDbRegion* pDbRegion = (OdDbRegion*)(pDbEntity);

    OdBrBrep brRegion;
    pDbRegion->brep(brRegion);

    OdGeVector3d vrNormal;

    if( pDbRegion->getNormal(vrNormal) != eOk)
      vrNormal = OdGeVector3d::kZAxis;

    OdDgGraphicsElementPtr pDgRegion = createPlanarBrepGeometry(brRegion, vrNormal, owner->database(), pDbEntity);

    if (!pDgRegion.isNull())
    {
      addElementToDatabase(pDgRegion, owner);
      copyElementProperties(pDgRegion, pDbRegion, owner->database());
      appendOdDgElementToOwner(owner, pDgRegion);
      OdDgnExportContext::addDgnElement(pDbRegion->objectId(), pDgRegion->elementId());
    }
  }

  //===================================================================================================

}
