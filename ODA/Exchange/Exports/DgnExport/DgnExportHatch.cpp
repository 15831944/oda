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
#include "DgnExportHatch.h"
#include "DgnExportCurve.h"
#include "DgnExportCommon.h"
#include "DgnExportContext.h"
#include <DbHatch.h>
#include <DgShape.h>
#include <DgLineString.h>
#include <DbPolyline.h>
#include <DgCellHeader.h>
#include <DgComplexShape.h>
#include <DbSolid.h>
#include <DgBSplineCurve.h>
#include <Ge/GeCircArc2d.h>
#include <Ge/GeLineSeg3d.h>
#include <Ge/GeLineSeg2d.h>
#include <DbMPolygon.h>

namespace TD_DGN_EXPORT {

  //===================================================================================================

  struct OdDgElementPtrHatchLoopAssoc
  {
    OdDgElement*      m_pElm;
    OdDbObjectIdArray m_arrAssociatedObjects;
  };

  //===================================================================================================

  bool getLoopIsPolyline(const EdgeArray& arrCurvePtrs)
  {
    bool bRet = true;

    for( OdUInt32 i = 0; i < arrCurvePtrs.size(); i++ )
    {
      if( arrCurvePtrs[i]->type() == OdGe::kLineSeg2d )
        continue;

      bRet = false;
      break;
    }

    return bRet;
  }

  //===================================================================================================

  OdDbPolylinePtr createDbPolylineFromHatchPolylineLoop(const OdGePoint2dArray& arrPLinePoints,
                                                          const OdGeDoubleArray& arrPLineBulges,
                                                            const OdGeVector3d& vrNormal,
                                                              double dElevation,
                                                                bool bCloseSegment = true
                                                       )
  {
    OdDbPolylinePtr pPolyline = OdDbPolyline::createObject();

    pPolyline->setNormal(vrNormal);
    pPolyline->setElevation(dElevation);
    OdUInt32 iVertex = 0;

    for (OdUInt32 i = 0; i < arrPLinePoints.size(); i++)
    {
      double dBulge = 0.0;

      if (i < arrPLineBulges.size())
        dBulge = arrPLineBulges[i];

      pPolyline->addVertexAt(i, arrPLinePoints[i], dBulge, 0.0, 0.0);
    }

    pPolyline->setClosed(bCloseSegment);

    return pPolyline;
  }

  //===================================================================================================

  OdDgGraphicsElementPtr createDgElementFromHatchLoopPolylineData( OdDbHatch* pDbHatch,
                                                                     const OdGePoint2dArray& arrPLinePoints, 
                                                                       const OdGeDoubleArray& arrPLineBulges,
                                                                         OdDgDatabase* pDb )
  {
    OdDgGraphicsElementPtr pRet;
    
    OdDbPolylinePtr pPolyline = createDbPolylineFromHatchPolylineLoop(arrPLinePoints, arrPLineBulges, pDbHatch->normal(), pDbHatch->elevation());

    bool bSetLineStyleModifiers = false;
    bool bNegativeThickness = false;
    pRet = exportDbPolyline(pPolyline, pDbHatch, pDb, false, true, bSetLineStyleModifiers, bNegativeThickness);

    return pRet;
  }

  //===================================================================================================

  OdDgGraphicsElementPtr createDgShapeFromHatchLoopCurves(OdDbHatch* pDbHatch, EdgeArray& arrLoopCurves2d, OdDgDatabase* pDb)
  {
    OdDgGraphicsElementPtr pRet;

    if( arrLoopCurves2d.isEmpty() )
      return pRet;

    OdGeMatrix3d matTransform = pDbHatch->getEcs();

    OdGePoint3dArray arrShapePts;

    double dZCoord = pDbHatch->elevation();

    for( OdUInt32 i = 0; i < arrLoopCurves2d.size(); i++ )
    {
      OdGePoint2d ptStart;

      if( arrLoopCurves2d[i]->hasStartPoint(ptStart) )
      {
        OdGePoint3d pt3d(ptStart.x, ptStart.y, dZCoord);
        pt3d.transformBy(matTransform);
        arrShapePts.push_back(pt3d);
      }
    }

    OdGePoint2d ptEnd;

    if (arrLoopCurves2d.last()->hasEndPoint(ptEnd))
    {
      OdGePoint3d pt3d(ptEnd.x, ptEnd.y, dZCoord);
      pt3d.transformBy(matTransform);
      arrShapePts.push_back(pt3d);
    }

    pRet = createDgShape(arrShapePts, pDbHatch, pDb);

    return pRet;
  }

  //===================================================================================================

  bool makeClosedDgCurve(const OdDgGraphicsElement* pGrElm)
  {
    if (!pGrElm)
      return false;

    if( pGrElm->isKindOf(OdDgCurveElement2d::desc()) )
    {
      OdDgCurveElement2dPtr pCurve2d = pGrElm;
      return pCurve2d->isClosed();
    }
    else if (pGrElm->isKindOf(OdDgCurveElement3d::desc()))
    {
      OdDgCurveElement3dPtr pCurve3d = pGrElm;
      return pCurve3d->isClosed();
    }

    return false;
  }

  //===================================================================================================

  OdDgGraphicsElementPtr craeteDgComplexShapeFromHatchLoopCurves(OdDbHatch* pDbHatch, OdDbEntity* pDbBaseEnt, EdgeArray& arrLoopCurves2d, OdDgDatabase* pDb)
  {
    OdDgGraphicsElementPtr pRet;

    OdGeMatrix3d matTransform = pDbHatch->getEcs();
    matTransform.preMultBy(OdGeMatrix3d::translation(OdGeVector3d::kZAxis*pDbHatch->elevation()));

    if( arrLoopCurves2d.size() == 1 )
    {
      OdGeCurve3d* pCurve3d = arrLoopCurves2d[0]->convertTo3d();

      if( pCurve3d )
      {
        OdGeVector3d vrCurveHatchNormal = pDbHatch->normal();
        pCurve3d->transformBy(matTransform);
        pRet = createDgCurveFromGeCurve(pCurve3d, vrCurveHatchNormal, pDb, true);

        if( !makeClosedDgCurve(pRet) )
          pRet = NULL;

        delete pCurve3d;
      }

      if( !pRet.isNull() )
        return pRet;
    }

    OdDgComplexShapePtr pCplxShape = OdDgComplexShape::createObject();
    OdUInt32 nItems = 0;

    for( OdUInt32 i = 0; i < arrLoopCurves2d.size(); i++ )
    {
      OdGeCurve3d* pCurve3d = arrLoopCurves2d[i]->convertTo3d();

      if( pCurve3d )
      {
        OdGeVector3d vrCurveHatchNormal = pDbHatch->normal();
        pCurve3d->transformBy(matTransform);
        OdDgGraphicsElementPtr pShapeItem = createDgCurveFromGeCurve(pCurve3d, vrCurveHatchNormal, pDb);

        if( !pShapeItem.isNull() )
        {
          copyElementProperties(pShapeItem, pDbBaseEnt, pDb);
          pCplxShape->add(pShapeItem);
          nItems++;
        }

        delete pCurve3d;
      }
    }

    if( nItems > 0 )
      pRet = pCplxShape;

    return pRet;
  }

  //===================================================================================================

  OdDgGraphicsElementPtr createHatchLoop(OdDbHatch* pDbHatch, OdDbEntity* pDbBaseEnt, OdInt32 iLoopIndex, OdDgDatabase* pDb, OdDgElementPtrHatchLoopAssoc& loopAssoc )
  {
    OdDgGraphicsElementPtr pRet;

    if( !pDbHatch || iLoopIndex >= pDbHatch->numLoops() )
      return pRet;

    OdInt32 iLoopType         = pDbHatch->loopTypeAt(iLoopIndex);
    pDbHatch->getAssocObjIdsAt(iLoopIndex, loopAssoc.m_arrAssociatedObjects);

    if( iLoopType & OdDbHatch::kNotClosed )
      return pRet;

    if( iLoopType & OdDbHatch::kPolyline )
    {
      OdGePoint2dArray arrPLinePoints;
      OdGeDoubleArray  arrPLineBulges;
      pDbHatch->getLoopAt(iLoopIndex, arrPLinePoints, arrPLineBulges);
      pRet = createDgElementFromHatchLoopPolylineData(pDbHatch, arrPLinePoints, arrPLineBulges, pDb);
    }
    else
    {
      EdgeArray arrLoopCurves2d;
      pDbHatch->getLoopAt(iLoopIndex, arrLoopCurves2d);

      bool bLoopIsPolyline = getLoopIsPolyline(arrLoopCurves2d);

      if (bLoopIsPolyline)
        pRet = createDgShapeFromHatchLoopCurves(pDbHatch, arrLoopCurves2d, pDb);
      else
        pRet = craeteDgComplexShapeFromHatchLoopCurves(pDbHatch, pDbBaseEnt, arrLoopCurves2d, pDb);
    }

    if( !pRet.isNull() )
      loopAssoc.m_pElm = pRet;

    return pRet;
  }

  //===================================================================================================

  OdGePoint2dArray getHatchLoopPts(OdDbHatch* pDbHatch, OdInt32 iLoopIndex)
  {
    OdGePoint2dArray arrRet;

    OdInt32 iLoopType = pDbHatch->loopTypeAt(iLoopIndex);

    if (iLoopType & OdDbHatch::kPolyline)
    {
      OdGePoint2dArray arrPLinePoints;
      OdGeDoubleArray  arrPLineBulges;
      pDbHatch->getLoopAt(iLoopIndex, arrPLinePoints, arrPLineBulges);

      if (arrPLineBulges.isEmpty())
      {
        arrRet = arrPLinePoints;
      }
      else
      {
        OdDbPolylinePtr pPolyline = createDbPolylineFromHatchPolylineLoop(arrPLinePoints, arrPLineBulges, pDbHatch->normal(), pDbHatch->elevation());
        
        for( OdUInt32 i = 0; i < pPolyline->numVerts() - 1; i++ )
        {
          OdGePoint2dArray arrPts;

          if( pPolyline->getBulgeAt(i) )
          {
            OdGeCircArc2d arcSeg;
            pPolyline->getArcSegAt(i, arcSeg);
            arcSeg.getSamplePoints(15, arrPts);
          }
          else
          {
            arrPts.push_back(arrPLinePoints[i]);
            arrPts.push_back(arrPLinePoints[i + 1]);
          }

          if( arrRet.isEmpty() )
            arrRet = arrPts;
          else
          {
            if (arrRet.last().isEqualTo(arrPts.first()))
              arrRet.removeLast();

            arrRet.append(arrPts);
          }
        }
      }
    }
    else
    {
      EdgeArray arrLoopCurves2d;
      pDbHatch->getLoopAt(iLoopIndex, arrLoopCurves2d);

      for (OdUInt32 i = 0; i < arrLoopCurves2d.size(); i++)
      {
        if( arrLoopCurves2d[i] )
        {
          OdGePoint2dArray arrPts;

          if( arrLoopCurves2d[i]->type() == OdGe::kLineSeg2d )
          {
            OdGePoint2d ptStart, ptEnd;
            arrLoopCurves2d[i]->hasStartPoint(ptStart);
            arrLoopCurves2d[i]->hasEndPoint(ptEnd);
            arrPts.push_back(ptStart);
            arrPts.push_back(ptEnd);
          }
          else if( arrLoopCurves2d.size() == 1 )
            arrLoopCurves2d[i]->getSamplePoints(30, arrPts);
          else
            arrLoopCurves2d[i]->getSamplePoints(20, arrPts);

          if (arrRet.isEmpty())
            arrRet = arrPts;
          else
          {
            if( arrRet.last().isEqualTo(arrPts.first()) )
              arrRet.removeLast();

            arrRet.append(arrPts);
          }
        }
      }
    }

    return arrRet;
  }

  //===================================================================================================

  void reduceShapePts( OdGePoint3dArray& arrPts )
  {
    OdGePoint3dArray arrResultPts;
    arrResultPts.push_back(arrPts[0]);
    arrResultPts.push_back(arrPts[1]);

    for( OdUInt32 i = 2; i < arrPts.size(); i++ )
    {
      OdGeVector3d vrPrev = arrResultPts.last() - arrResultPts[arrResultPts.size() - 2];
      OdGeVector3d vrCur = arrPts[i] - arrPts[i-1];

      if (vrPrev.isCodirectionalTo(vrCur))
        arrResultPts[arrResultPts.size() - 1] = arrPts[i];
      else
        arrResultPts.push_back(arrPts[i]);
    }

    arrPts = arrResultPts;
  }

  //===================================================================================================

  OdDgGraphicsElementPtr createHatchGeometryThroughExplode(OdDbHatch* pDbHatch, OdDbEntity* pBaseEnt, OdDgDatabase* pDb)
  {
    OdDgGraphicsElementPtr pRet;

    // 1. Create solid hatch clone and explode it to set of DbSolid objects.

    OdDbHatchPtr pHatchClone = pDbHatch->clone();
    pHatchClone->setPattern(OdDbHatch::kPreDefined, L"SOLID");
    OdRxObjectPtrArray arrExplodeSet;
    pHatchClone->explodeGeometry(arrExplodeSet);

    // 2. Create array of line segments.

    OdArray<OdGeLineSeg3d> arrLineSegments;
    OdArray<bool>          arrSegmentUsage;
    arrLineSegments.reserve(arrExplodeSet.size() * 4);
    arrSegmentUsage.reserve(arrExplodeSet.size() * 4);
    OdUInt32 i, j;

    for( i = 0; i < arrExplodeSet.size(); i++ )
    {
      if (!arrExplodeSet[i].isNull() && arrExplodeSet[i]->isKindOf(OdDbSolid::desc()))
      {
        OdDbSolidPtr pSolid = arrExplodeSet[i];
        OdGePoint3dArray arrSolidPts;

        for( j = 0; j < 4; j++ )
        {
          OdGePoint3d ptSolid;
          pSolid->getPointAt(j, ptSolid);

          bool bAddPoint = true;

          for( OdInt32 l = arrSolidPts.size()-1; l >= 0 ; l-- )
          {
            if (arrSolidPts[l].isEqualTo(ptSolid))
            {
              bAddPoint = false;
              break;
            }
          }

          if( bAddPoint )
            arrSolidPts.push_back(ptSolid);
        }

        if( arrSolidPts.size() < 3 )
          continue;

        for( j = 0; j < arrSolidPts.size(); j++ )
        {
          if( j == arrSolidPts.size() - 1 )
            arrLineSegments.push_back(OdGeLineSeg3d(arrSolidPts[j], arrSolidPts[0]));
          else
            arrLineSegments.push_back(OdGeLineSeg3d(arrSolidPts[j], arrSolidPts[j+1]));

          arrSegmentUsage.push_back(false);
        }
      }
    }

    // 3. remove line segments clone.

    for( i = 0; i < arrLineSegments.size(); i++ )
    {
      if( arrSegmentUsage[i] )
        continue;

      OdGePoint3d ptStart = arrLineSegments[i].startPoint();
      OdGePoint3d ptEnd   = arrLineSegments[i].endPoint();

      for( OdUInt32 j = i + 1; j < arrLineSegments.size(); j++ )
      {
        if( arrSegmentUsage[j] )
          continue;

        OdGePoint3d ptCurStart = arrLineSegments[j].startPoint();
        OdGePoint3d ptCurEnd   = arrLineSegments[j].endPoint();

        if( (ptStart.isEqualTo(ptCurStart) && ptEnd.isEqualTo(ptCurEnd)) || (ptStart.isEqualTo(ptCurEnd) && ptEnd.isEqualTo(ptCurStart)) )
        {
          arrSegmentUsage[i] = true;
          arrSegmentUsage[j] = true;
          break;
        }
      }
    }

    OdArray<OdGeLineSeg3d> arrTmp;

    for( i = 0; i < arrLineSegments.size(); i++)
    {
      if (arrSegmentUsage[i])
        continue;

      arrTmp.push_back(arrLineSegments[i]);
    }

    arrLineSegments = arrTmp;
    arrSegmentUsage.clear();
    arrSegmentUsage.resize(arrTmp.size(), false);

    OdArray<OdGePoint3dArray> arrLoops;

    bool bCreateNextLoop = true;

    while( bCreateNextLoop )
    {
      OdGePoint3dArray arrLoopPts;

      bCreateNextLoop = false;

      for(i = 0; i < arrLineSegments.size(); i++)
      {
        if (arrSegmentUsage[i])
          continue;

        arrLoopPts.push_back(arrLineSegments[i].startPoint());
        arrLoopPts.push_back(arrLineSegments[i].endPoint());
        arrSegmentUsage[i] = true;

        bool bAddNextPoint = true;

        while( bAddNextPoint )
        {
          bAddNextPoint = false;

          for( OdUInt32 j = 0; j < arrLineSegments.size(); j++ )
          {
            if( arrSegmentUsage[j] )
              continue;

            if( arrLoopPts.last().isEqualTo(arrLineSegments[j].startPoint()))
            {
              arrLoopPts.push_back(arrLineSegments[j].endPoint());
              arrSegmentUsage[j] = true;
              bAddNextPoint = true;
              break;
            }
            else if (arrLoopPts.last().isEqualTo(arrLineSegments[j].endPoint()))
            {
              arrLoopPts.push_back(arrLineSegments[j].startPoint());
              arrSegmentUsage[j] = true;
              bAddNextPoint = true;
              break;
            }
          }

          if( bAddNextPoint && arrLoopPts.last().isEqualTo(arrLoopPts.first()) )
          {
            bAddNextPoint = false;

            if( arrLoopPts.size() > 2 )
              arrLoops.push_back(arrLoopPts);
          }
        }

        bCreateNextLoop = true;
        break;
      }
    }

    if( !arrLoops.isEmpty() )
    {
      OdArray<OdDgGraphicsElementPtr> arrShapes;

      for (i = 0; i < arrLoops.size(); i++)
      {
        reduceShapePts(arrLoops[i]);
        OdDgGraphicsElementPtr pShape = createDgShape(arrLoops[i], pDbHatch, pDb);

        if (!pShape.isNull())
        {
          copyElementProperties(pShape, pBaseEnt, pDb, false);
          pShape->setHbitFlag(!arrShapes.isEmpty());
          arrShapes.push_back(pShape);
        }
      }

      if( !arrShapes.isEmpty() )
      {
        if( arrShapes.size() == 1 )
          pRet = arrShapes[0];
        else
        {
          if (OdDgnExportContext::is3d())
            pRet = OdDgCellHeader3d::createObject();
          else
            pRet = OdDgCellHeader2d::createObject();

          pRet->setDatabaseDefaults(pDb);
          pRet->setLevelEntryId(0);

          for (OdUInt32 i = 0; i < arrShapes.size(); i++)
            appendOdDgElementToOwner(pRet, arrShapes[i]);
        }
      }
    }

    return pRet;
  }

  //===================================================================================================

  bool isNonClosedHatch(OdDbHatch* pDbHatch)
  {
    bool bRet = true;

    for( OdInt32 i = 0; i < pDbHatch->numLoops(); i++ )
    {
      if( !(pDbHatch->loopTypeAt(i) & OdDbHatch::kNotClosed) )
      {
        bRet = false;
        break;
      }
    }

    return bRet;
  }

  //===================================================================================================

  bool findNearbyGeCurve( OdArray<bool>& arrUsedCurves,
                            OdArray<OdGeCurve2d*> arrHatchLoopCurves,
                              bool& bExtendStart,
                                bool& bExtendEnd,
                                  OdInt32 iCurCurve,
                                    OdInt32& iNextCurve,
                                      bool& bExtendedStart)
  {
    bool bRet = false;

    double dDistanceToStart = 1e10;
    double dDistanceToEnd   = 1e10;

    OdInt32 iBestStartCurveNumber = -1;
    OdInt32 iBestEndCurveNumber   = -1;
    bool    bStartConnectToStart  = false;
    bool    bEndConnectToStart    = false;

    OdGePoint2d ptBaseStart, ptBaseEnd;
    OdGePoint2d ptCurveStart, ptCurveEnd;

    if( bExtendStart )
      arrHatchLoopCurves[iCurCurve]->hasStartPoint(ptBaseStart);

    if( bExtendEnd )
      arrHatchLoopCurves[iCurCurve]->hasEndPoint(ptBaseEnd);

    for( OdUInt32 i = 0; i < arrHatchLoopCurves.size(); i++ )
    {
      if( arrUsedCurves[i] )
        continue;

      arrHatchLoopCurves[i]->hasStartPoint(ptCurveStart);
      arrHatchLoopCurves[i]->hasEndPoint(ptCurveEnd);

      double dDistanceToCurveStart = 0;
      double dDistanceToCurveEnd = 0;
      double dDistanceToCurve = 0;
      bool   bMinDistanceToStart = false;

      if( bExtendStart )
      {
        dDistanceToCurveStart = ptBaseStart.distanceTo(ptCurveStart);
        dDistanceToCurveEnd   = ptBaseStart.distanceTo(ptCurveEnd);

        if( dDistanceToCurveStart < dDistanceToCurveEnd )
        {
          dDistanceToCurve = dDistanceToCurveStart;
          bMinDistanceToStart = true;
        }
        else
        {
          dDistanceToCurve = dDistanceToCurveEnd;
          bMinDistanceToStart = false;
        }

        if( dDistanceToCurve < dDistanceToStart )
        {
          dDistanceToStart = dDistanceToCurve;
          iBestStartCurveNumber = i;
          bStartConnectToStart = bMinDistanceToStart;
          bRet = true;
        }
      }

      if( bExtendEnd )
      {
        dDistanceToCurveStart = ptBaseEnd.distanceTo(ptCurveStart);
        dDistanceToCurveEnd = ptBaseEnd.distanceTo(ptCurveEnd);

        if (dDistanceToCurveStart < dDistanceToCurveEnd)
        {
          dDistanceToCurve = dDistanceToCurveStart;
          bMinDistanceToStart = true;
        }
        else
        {
          dDistanceToCurve = dDistanceToCurveEnd;
          bMinDistanceToStart = false;
        }

        if (dDistanceToCurve < dDistanceToEnd)
        {
          dDistanceToEnd = dDistanceToCurve;
          iBestEndCurveNumber = i;
          bEndConnectToStart = bMinDistanceToStart;
          bRet = true;
        }
      }
    }

    if( bRet )
    {
      if( bExtendStart && bExtendEnd )
      {
        if( dDistanceToStart < dDistanceToEnd )
        {
          iNextCurve      = iBestStartCurveNumber;
          bExtendedStart = true;
          bExtendStart = !bStartConnectToStart;
          bExtendEnd = bStartConnectToStart;
        }
        else
        {
          iNextCurve = iBestEndCurveNumber;
          bExtendedStart = false;
          bExtendStart = !bEndConnectToStart;
          bExtendEnd = bEndConnectToStart;
        }
      }
      else if( bExtendStart )
      {
        iNextCurve = iBestStartCurveNumber;
        bExtendedStart = true;
        bExtendStart = !bStartConnectToStart;
        bExtendEnd = bStartConnectToStart;
      }
      else
      {
        iNextCurve = iBestEndCurveNumber;
        bExtendedStart = false;
        bExtendStart = !bEndConnectToStart;
        bExtendEnd = bEndConnectToStart;
      }
    }

    return bRet;
  }

  //===================================================================================================

  bool getDwgCurveStartEndPoint(const OdDbObjectId& idCurve, OdGePoint3d& ptObjStart, OdGePoint3d& ptObjEnd)
  {
    bool bRet = false;

    if( !idCurve.isNull() )
    {
      OdDbObjectPtr pObject = idCurve.openObject();

      if( !pObject.isNull() && pObject->isKindOf(OdDbCurve::desc()) )
      {
        OdDbCurvePtr pCurve = pObject;

        if( (pCurve->getStartPoint(ptObjStart) == eOk) && (pCurve->getEndPoint(ptObjEnd) == eOk) )
          bRet = true;
      }
    }

    return bRet;
  }

  //===================================================================================================

  double getTotalDistanceStartEnd(const OdGePoint3d& ptStart,
                                    const OdGePoint3d& ptEnd,
                                      const OdGePoint3d& ptObjStart,
                                        const OdGePoint3d& ptObjEnd)
  {
    double dStartStart  = ptStart.distanceTo(ptObjStart);
    double dStartEnd    = ptStart.distanceTo(ptObjEnd);
    double dEndStart    = ptEnd.distanceTo(ptObjStart);
    double dEndEnd      = ptEnd.distanceTo(ptObjEnd);

    double dBestStart = (dStartStart < dStartEnd) ? dStartStart : dStartEnd;
    double dBestEnd   = (dEndStart < dEndEnd) ? dEndStart : dEndEnd;

    return dBestStart + dBestEnd;
  }

  //===================================================================================================

  OdDgGraphicsElementPtr createNonClosedHatchLoop(  OdDbHatch* pDbHatch, 
                                                      OdDgDatabase* pDb, 
                                                        OdArray<OdDgElementPtrHatchLoopAssoc>& arrDgLoopAssoc,
                                                          bool& bBreakAssociation )
  {
    OdDgGraphicsElementPtr pDgHatch;
    OdArray<OdGeCurve2d*>   arrCreatedCurves;
    OdArray<OdGeCurve2d*>   arrHatchLoopCurves;

    // Create all curves to create contour

    std::set<OdDbObjectId> setHatchAssoc;

    for( OdInt32 i = 0; i < pDbHatch->numLoops(); i++ )
    {
      if( pDbHatch->loopTypeAt(i) & OdDbHatch::kTextbox )
        continue;

      if( !bBreakAssociation )
      {
        OdDbObjectIdArray arrLoopAssoc;
        pDbHatch->getAssocObjIdsAt(i, arrLoopAssoc);

        if( !arrLoopAssoc.isEmpty() )
        {
          for (OdUInt32 j = 0; j < arrLoopAssoc.size(); j++)
            setHatchAssoc.insert(arrLoopAssoc[j]);
        }
      }

      if( pDbHatch->loopTypeAt(i) & OdDbHatch::kPolyline )
      {
        OdGePoint2dArray arrPLinePoints;
        OdGeDoubleArray  arrPLineBulges;
        pDbHatch->getLoopAt(i, arrPLinePoints, arrPLineBulges);
        OdDbPolylinePtr pPolyline = createDbPolylineFromHatchPolylineLoop(arrPLinePoints, arrPLineBulges, pDbHatch->normal(), pDbHatch->elevation(), false);

        for( OdUInt32 j = 0; j < pPolyline->numVerts() - 1; j++)
        {
          if (pPolyline->getBulgeAt(i))
          {
            OdGeCircArc2d* pArcSeg = new OdGeCircArc2d();
            pPolyline->getArcSegAt(i, *pArcSeg);
            arrCreatedCurves.push_back(pArcSeg);
            arrHatchLoopCurves.push_back(pArcSeg);
          }
          else
          {
            OdGeLineSeg2d* pLineSeg = new OdGeLineSeg2d(arrPLinePoints[i], arrPLinePoints[i + 1]);
            arrCreatedCurves.push_back(pLineSeg);
            arrHatchLoopCurves.push_back(pLineSeg);
          }
        }
      }
      else
      {
        EdgeArray arrLoopEdges;
        pDbHatch->getLoopAt(i, arrLoopEdges);

        for (OdUInt32 j = 0; j < arrLoopEdges.size(); j++)
          arrHatchLoopCurves.push_back(arrLoopEdges[j]);
      }
    }

    // Connect hatch loop curves to create closed loop

    OdArray<OdGeCurve2d*> arrGeLoopCurves;

    if( arrHatchLoopCurves.isEmpty() )
      return pDgHatch;
    else if( arrHatchLoopCurves.size() == 1 )
    {
      arrGeLoopCurves.push_back(arrHatchLoopCurves[0]);

      OdGePoint2d ptStart;
      OdGePoint2d ptEnd;
      arrHatchLoopCurves[0]->hasStartPoint(ptStart);
      arrHatchLoopCurves[0]->hasEndPoint(ptEnd);

      if( !ptStart.isEqualTo(ptEnd) )
      {
        OdGeLineSeg2d* pLineSeg = new OdGeLineSeg2d(ptEnd, ptStart);
        arrCreatedCurves.push_back(pLineSeg);
        arrGeLoopCurves.push_back(pLineSeg);
        bBreakAssociation = true;
      }
    }
    else
    {
      OdArray<bool> arrUsedCurves;
      arrUsedCurves.resize(arrHatchLoopCurves.size(), false);

      OdGePoint2d ptLoopStart, ptLoopEnd;

      OdInt32 iCurCurve = 0;
      bool bExtendStart = true;
      bool bExtendEnd = true;
      bool bExtendedStart = false;
      OdInt32 iNextCurve = -1;
      arrGeLoopCurves.push_back(arrHatchLoopCurves[0]);
      arrUsedCurves[0] = true;

      while( findNearbyGeCurve(arrUsedCurves, arrHatchLoopCurves, bExtendStart, bExtendEnd, iCurCurve, iNextCurve, bExtendedStart) )
      {
        OdGePoint2d ptConnectionSegmentStart, ptConnectionSegmentEnd;

        if (bExtendedStart)
          arrHatchLoopCurves[iCurCurve]->hasStartPoint(ptConnectionSegmentStart);
        else
          arrHatchLoopCurves[iCurCurve]->hasEndPoint(ptConnectionSegmentStart);

        if( bExtendStart )
        {
          arrHatchLoopCurves[iNextCurve]->hasEndPoint(ptConnectionSegmentEnd);
          arrHatchLoopCurves[iNextCurve]->hasStartPoint(ptLoopEnd);
        }
        else
        {
          arrHatchLoopCurves[iNextCurve]->hasStartPoint(ptConnectionSegmentEnd);
          arrHatchLoopCurves[iNextCurve]->hasEndPoint(ptLoopEnd);
        }

        if( iCurCurve == 0 )
        {
          if( bExtendedStart )
            arrHatchLoopCurves[iCurCurve]->hasEndPoint(ptLoopStart);
          else
            arrHatchLoopCurves[iCurCurve]->hasStartPoint(ptLoopStart);
        }

        if( !ptConnectionSegmentStart.isEqualTo(ptConnectionSegmentEnd) )
        {
          OdGeLineSeg2d* pLineSeg = new OdGeLineSeg2d(ptConnectionSegmentStart, ptConnectionSegmentEnd);
          arrCreatedCurves.push_back(pLineSeg);
          arrGeLoopCurves.push_back(pLineSeg);
          bBreakAssociation = true;
        }

        arrGeLoopCurves.push_back(arrHatchLoopCurves[iNextCurve]);
        arrUsedCurves[iNextCurve] = true;

        iCurCurve = iNextCurve;
      }

      if( !ptLoopStart.isEqualTo(ptLoopEnd) )
      {
        OdGeLineSeg2d* pLineSeg = new OdGeLineSeg2d(ptLoopEnd, ptLoopStart);
        arrCreatedCurves.push_back(pLineSeg);
        arrGeLoopCurves.push_back(pLineSeg);
        bBreakAssociation = true;
      }
    }

    // Convert closed loop ge curves to dgn curves

    OdArray<OdDgGraphicsElementPtr> arrDgnCurves;

    OdGeMatrix3d matTransform = pDbHatch->getEcs();
    matTransform.preMultBy(OdGeMatrix3d::translation(OdGeVector3d::kZAxis*pDbHatch->elevation()));

    bool bAllCurvesAreLinesOrPolylines = true;

    OdUInt32 k;

    for( k = 0; k < arrGeLoopCurves.size(); k++ )
    {
      OdGeCurve3d* p3dCurve = arrGeLoopCurves[k]->convertTo3d();

      if( p3dCurve )
      {
        OdGeVector3d vrCurveHatchNormal = pDbHatch->normal();
        p3dCurve->transformBy(matTransform);
        OdDgGraphicsElementPtr pDgnCurve = createDgCurveFromGeCurve(p3dCurve, vrCurveHatchNormal, pDb, true);

        if( !pDgnCurve.isNull() )
        {
          if ((pDgnCurve->getElementType() != OdDgElement::kTypeLine) && (pDgnCurve->getElementType() != OdDgElement::kTypeLineString))
            bAllCurvesAreLinesOrPolylines = false;

          arrDgnCurves.push_back(pDgnCurve);
        }

        delete p3dCurve;
      }
    }

    // Correct dgn curve directions

    OdGePoint3d ptLast;

    for( k = 0; k < arrDgnCurves.size() - 1; k++ )
    {
      if( k == 0 )
      {
        OdGePoint3d ptStart1, ptEnd1;
        OdGePoint3d ptStart2, ptEnd2;

        getDgnCurveStartEndPoint(arrDgnCurves[k], ptStart1, ptEnd1);
        getDgnCurveStartEndPoint(arrDgnCurves[k+1], ptStart2, ptEnd2);

        double dDistanceStart1Start2 = ptStart1.distanceTo(ptStart2);
        double dDistanceStart1End2   = ptStart1.distanceTo(ptEnd2);
        double dDistanceEnd1Start2   = ptEnd1.distanceTo(ptStart2);
        double dDistanceEnd1End2     = ptEnd1.distanceTo(ptEnd2);
        bool bReverseCurve2Start     = true;
        bool bReverseCurve2End       = true;

        double dMinDistanceToStart   = 0.0;
        double dMinDistanceToEnd     = 0.0;

        if( dDistanceStart1Start2 < dDistanceStart1End2 )
        {
          dMinDistanceToStart = dDistanceStart1Start2;
          bReverseCurve2Start = false;
        }
        else
        {
          dMinDistanceToStart = dDistanceStart1End2;
          bReverseCurve2Start = true;
        }

        if (dDistanceEnd1Start2 < dDistanceEnd1End2)
        {
          dMinDistanceToEnd = dDistanceEnd1Start2;
          bReverseCurve2End = false;
        }
        else
        {
          dMinDistanceToEnd = dDistanceStart1End2;
          bReverseCurve2End = true;
        }

        if( dMinDistanceToStart > dMinDistanceToEnd )
        {
          reverseDgnCurve( arrDgnCurves[k] );

          if( bReverseCurve2End )
            reverseDgnCurve(arrDgnCurves[k + 1]);

          ptLast = ptStart1;
        }
        else
        {
          if( bReverseCurve2Start )
            reverseDgnCurve(arrDgnCurves[k + 1]);

          ptLast = ptEnd1;
        }
      }
      else
      {
        OdGePoint3d ptStart, ptEnd;
        getDgnCurveStartEndPoint(arrDgnCurves[k + 1], ptStart, ptEnd);

        double dDistanceStart = ptLast.distanceTo(ptStart);
        double dDistanceEnd   = ptLast.distanceTo(ptEnd);

        if( dDistanceStart > dDistanceEnd )
        {
          reverseDgnCurve(arrDgnCurves[k + 1]);
          ptLast = ptStart;
        }
        else
        {
          ptLast = ptEnd;
        }
      }
    }

    // Create loop association with element

    OdDgElementPtrHatchLoopAssoc assocLoop;

    if( !bBreakAssociation )
    {
      for( OdUInt32 k = 0; k < arrDgnCurves.size(); k++ )
      {
        OdGePoint3d ptSegmentStart, ptSegmentEnd;

        getDgnCurveStartEndPoint(arrDgnCurves[k], ptSegmentStart, ptSegmentEnd);

        std::set<OdDbObjectId>::iterator pObjSetIter = setHatchAssoc.begin();

        OdDbObjectId idBestAssoc;

        if( pObjSetIter == setHatchAssoc.end() )
          break;

        double dBestDistance = 1e10;

        for(; pObjSetIter != setHatchAssoc.end(); pObjSetIter++)
        {
          OdGePoint3d ptObjStart, ptObjEnd;

          if( getDwgCurveStartEndPoint(*pObjSetIter, ptObjStart, ptObjEnd) )
          {
            double dCurDistance = getTotalDistanceStartEnd(ptSegmentStart, ptSegmentEnd, ptObjStart, ptObjEnd);

            if( dCurDistance < dBestDistance )
            {
              dBestDistance = dCurDistance;
              idBestAssoc = *pObjSetIter;
            }
          }
        }

        if( !idBestAssoc.isNull() )
        {
          assocLoop.m_arrAssociatedObjects.push_back(idBestAssoc);
          setHatchAssoc.erase(idBestAssoc);
        }
      }
    }

    // Create shape or complex shape

    if( bAllCurvesAreLinesOrPolylines )
    {
      OdGePoint3dArray arrShapePts = getShapePtsFromSetOfDgnCurves(arrDgnCurves);
      pDgHatch = createDgShape(arrShapePts, pDbHatch, pDb);
    }
    else
    {
      pDgHatch = OdDgComplexShape::createObject();
      pDgHatch->setDatabaseDefaults(pDb);
      pDgHatch->setLevelEntryId(0);

      for (OdUInt32 l = 0; l < arrDgnCurves.size(); l++)
        appendOdDgElementToOwner(pDgHatch, arrDgnCurves[l]);
    }

    // save hatch loop association

    if( !bBreakAssociation && !pDgHatch.isNull() && !assocLoop.m_arrAssociatedObjects.isEmpty() )
    {
      assocLoop.m_pElm = pDgHatch;
      arrDgLoopAssoc.push_back(assocLoop);
    }

    // Free created curves

    for (OdUInt32 j = 0; j < arrCreatedCurves.size(); j++)
      delete arrCreatedCurves[j];

    arrCreatedCurves.clear();

    return pDgHatch;
  }

  //===================================================================================================

  OdDgGraphicsElementPtr createHatchGeometry(OdDbHatch* pDbHatch,
                                               OdDbEntity* pDbBaseEnt,
                                                 OdDgDatabase* pDb, 
                                                   OdArray<OdDgElementPtrHatchLoopAssoc>& arrDgLoopAssoc, 
                                                     bool& bBreakAssociation)
  {
    if( isNonClosedHatch(pDbHatch) )
      return createNonClosedHatchLoop(pDbHatch, pDb, arrDgLoopAssoc, bBreakAssociation);

    OdDgGraphicsElementPtr pDgHatch;

    if (pDbHatch->numLoops() == 1) // simple hatch, create only 1 non-closed loop.
    {
      OdDgElementPtrHatchLoopAssoc loopAssoc;

      pDgHatch = createHatchLoop(pDbHatch, pDbBaseEnt, 0, pDb, loopAssoc);

      if( !bBreakAssociation && loopAssoc.m_pElm && !loopAssoc.m_arrAssociatedObjects.isEmpty() )
        arrDgLoopAssoc.push_back(loopAssoc);
    }
    else // complex hatch, create set of non-closed loop, all non-closed loops are ignored.
    {
      OdArray<bool> arrOutermostLoopStatus;
      OdUInt32Array arrContourLoopNumbers;
      OdArray<OdDgGraphicsElementPtr> arrDgLoopElements;
      OdArray<OdGePoint2dArray> arrGeContours;
      bool bTextBoundaryPresents = false;

      // create dgn closed curves as loops

      for (OdInt32 i = 0; i < pDbHatch->numLoops(); i++)
      {
        if (pDbHatch->loopTypeAt(i) & OdDbHatch::kTextbox)
        {
          bTextBoundaryPresents = true;
          break;
        }

        OdDgElementPtrHatchLoopAssoc loopAssoc;

        OdDgGraphicsElementPtr pDgLoop = createHatchLoop(pDbHatch, pDbBaseEnt, i, pDb, loopAssoc);

        if (!pDgLoop.isNull())
        {
          copyElementProperties(pDgLoop, pDbBaseEnt, pDb, false);

          arrContourLoopNumbers.push_back(i);

          if (!bBreakAssociation )
            arrDgLoopAssoc.push_back(loopAssoc);

          // get simple ge contour points to calculate order of loops

          OdGePoint2dArray arrGeContour = getHatchLoopPts(pDbHatch, i);
          arrGeContours.push_back(arrGeContour);

          // Get outerMost loop status for differend hatch modes

          bool bLoopIsOutermost = (pDbHatch->loopTypeAt(i) & OdDbHatch::kOutermost) ? true : false;
          bool bLoopIsExternal = (pDbHatch->loopTypeAt(i) & OdDbHatch::kExternal) ? true : false;

          if (pDbHatch->hatchStyle() == OdDbHatch::kOuter)
            arrOutermostLoopStatus.push_back(bLoopIsOutermost);
          else if (pDbHatch->hatchStyle() == OdDbHatch::kIgnore)
            arrOutermostLoopStatus.push_back(bLoopIsOutermost && bLoopIsExternal);
          else
            arrOutermostLoopStatus.push_back(false);

          arrDgLoopElements.push_back(pDgLoop);
        }
      }

      // remove unused loops and calculate loop status ( solid or hole ) for hatches with text boundaries

      if( !bTextBoundaryPresents && arrDgLoopElements.size() > 1)
      {
        OdUInt32Array arrContourLevels;
        separateContours(arrGeContours, arrOutermostLoopStatus, arrContourLevels);

        OdUInt32 iMainContourIndex = 0;
        bool bMainContourFound = false;

        for (OdUInt32 i = 0; i < arrContourLevels.size(); i++)
        {
          if ((pDbHatch->hatchStyle() == OdDbHatch::kIgnore && arrContourLevels[i] > 0) ||
              (pDbHatch->hatchStyle() == OdDbHatch::kOuter  && arrContourLevels[i] > 1)
            )
          {
            arrContourLevels.removeAt(i);
            arrDgLoopElements.removeAt(i);
            arrContourLoopNumbers.removeAt(i);
            arrGeContours.removeAt(i);

            if (!bBreakAssociation)
              arrDgLoopAssoc.removeAt(i);

            i--;
            continue;
          }

          if (!bMainContourFound && (arrContourLevels[i] == 0))
          {
            iMainContourIndex = i;
            bMainContourFound = true;
            arrDgLoopElements[i]->setHbitFlag(false);
          }
          else
            arrDgLoopElements[i]->setHbitFlag(true);
        }

        if (arrDgLoopElements.size() > 1 && (iMainContourIndex != 0))
        {
          OdDgGraphicsElementPtr pTmp = arrDgLoopElements[iMainContourIndex];
          arrDgLoopElements[iMainContourIndex] = arrDgLoopElements[0];
          arrDgLoopElements[0] = pTmp;

          OdUInt32 iTmp = arrContourLoopNumbers[iMainContourIndex];
          arrContourLoopNumbers[iMainContourIndex] = arrContourLoopNumbers[0];
          arrContourLoopNumbers[0] = iTmp;

          OdGePoint2dArray arrTmp = arrGeContours[iMainContourIndex];
          arrGeContours[iMainContourIndex] = arrGeContours[0];
          arrGeContours[0] = arrTmp;
        }
      }

      // for hatches with text boundaries. We must explode them and create loops from solid peaces.

      if( bTextBoundaryPresents )
      {
        bBreakAssociation = true;
        arrDgLoopAssoc.clear();

        return createHatchGeometryThroughExplode(pDbHatch, pDbBaseEnt, pDb);
      }

      // create dgn element for hatch

      if (arrDgLoopElements.size() == 1)
        pDgHatch = arrDgLoopElements[0];
      else
      {
        if (OdDgnExportContext::is3d())
          pDgHatch = OdDgCellHeader3d::createObject();
        else
          pDgHatch = OdDgCellHeader2d::createObject();

        pDgHatch->setDatabaseDefaults(pDb);
        pDgHatch->setLevelEntryId(0);

        for( OdUInt32 i = 0; i < arrDgLoopElements.size(); i++ )
          appendOdDgElementToOwner(pDgHatch, arrDgLoopElements[i]);
      }
    }

    return pDgHatch;
  }

  //===================================================================================================

  OdDgGraphicsElementPtr createAssociveRegion(OdDbHatch* pDbHatch, const OdDgGraphicsElementPtr& pDgHatchGeometry, OdDgDatabase* pDb)
  {
    OdDgGraphicsElementPtr pRetAssoc;

    if( OdDgnExportContext::is3d() )
    {
      OdDgAssocRegion3dPtr pAsscoRgn3d = OdDgAssocRegion3d::createObject();
      pAsscoRgn3d->setDatabaseDefaults(pDb);

      OdGeMatrix3d matTransform = pDbHatch->getEcs();
      matTransform.preMultBy(OdGeMatrix3d::translation(OdGeVector3d::kZAxis*pDbHatch->elevation()));

      OdGePoint3d ptOrigin( pDbHatch->originPoint().x, pDbHatch->originPoint().y, 0.0);
      ptOrigin.transformBy(matTransform);

      pAsscoRgn3d->setOrigin(ptOrigin);
      pAsscoRgn3d->setTransformation(matTransform);

      if( pDbHatch->numSeedPoints() > 0 )
        pAsscoRgn3d->setRegionType(OdDgAssocRegion::kHatchFill);
      else
        pAsscoRgn3d->setRegionType(OdDgAssocRegion::kFromElements);

      OdGePoint3dArray arrSeedPts;

      for (OdInt32 i = 0; i < pDbHatch->numSeedPoints(); i++)
      {
        OdGePoint2d ptSeed = pDbHatch->getSeedPointAt(i);
        OdGePoint3d ptSeed3d(ptSeed.x, ptSeed.y, 0.0);
        ptSeed3d.transformBy(matTransform);
        arrSeedPts.push_back(ptSeed3d);
      }

      if (!arrSeedPts.isEmpty())
        pAsscoRgn3d->setSeedPoints(arrSeedPts);

      pAsscoRgn3d->add(pDgHatchGeometry);
      pRetAssoc = pAsscoRgn3d;
    }
    else
    {
      OdDgAssocRegion2dPtr pAsscoRgn2d = OdDgAssocRegion2d::createObject();
      pAsscoRgn2d->setDatabaseDefaults(pDb);

      pAsscoRgn2d->setOrigin(pDbHatch->originPoint());

      if (pDbHatch->numSeedPoints() > 0)
        pAsscoRgn2d->setRegionType(OdDgAssocRegion::kHatchFill);
      else
        pAsscoRgn2d->setRegionType(OdDgAssocRegion::kFromElements);

      OdGePoint2dArray arrSeedPts;

      for (OdInt32 i = 0; i < pDbHatch->numSeedPoints(); i++)
        arrSeedPts.push_back(pDbHatch->getSeedPointAt(i));

      if( !arrSeedPts.isEmpty() )
        pAsscoRgn2d->setSeedPoints(arrSeedPts);

      pAsscoRgn2d->add(pDgHatchGeometry);
      pRetAssoc = pAsscoRgn2d;
    }

    if( !pRetAssoc.isNull() && (pDbHatch->isSolidFill() || pDbHatch->isGradient()))
      pRetAssoc->setStringLinkage(0x0026, pDbHatch->patternName());

    return pRetAssoc;
  }

  //===================================================================================================

  OdDgGradientFill::OdDgGradientType convertGradientNameToDgGradientType(const OdString& strGradientName, bool& bInvert)
  {
    bInvert = false;
    OdDgGradientFill::OdDgGradientType retType = OdDgGradientFill::kLinear;

    if (strGradientName == L"SPHERICAL")
      retType = OdDgGradientFill::kSpherical;
    else if (strGradientName == L"HEMISPHERICAL")
      retType = OdDgGradientFill::kHemispherical;
    else if (strGradientName == L"CURVED")
      retType = OdDgGradientFill::kCurved;
    else if (strGradientName == L"CYLINDER")
      retType = OdDgGradientFill::kCylindrical;
    else if (strGradientName == L"INVSPHERICAL")
    {
      retType = OdDgGradientFill::kSpherical;
      bInvert = true;
    }
    else if (strGradientName == L"INVHEMISPHERICAL")
    {
      retType = OdDgGradientFill::kHemispherical;
      bInvert = true;
    }
    else if (strGradientName == L"INVCURVED")
    {
      retType = OdDgGradientFill::kCurved;
      bInvert = true;
    }
    else if (strGradientName == L"INVLINEAR")
    {
      bInvert = true;
    }
    else if (strGradientName == L"INVCYLINDER")
    {
      retType = OdDgGradientFill::kCylindrical;
      bInvert = true;
    }

    return retType;
  }

  //===================================================================================================

  OdDgGradientFill getGradientFill( OdDbHatch* pDbHatch )
  {
    OdDgGradientFill retGradient;
    bool bInvGradient = false;
    retGradient.setGradientType(convertGradientNameToDgGradientType(pDbHatch->gradientName().makeUpper(), bInvGradient));
    retGradient.setAngle(pDbHatch->gradientAngle());
    retGradient.setInvertFlag(bInvGradient);
    retGradient.setShift(pDbHatch->gradientShift());
    retGradient.setWhiteIntensity(pDbHatch->getShadeTintValue());
    
    OdCmColorArray  arrColors;
    OdGeDoubleArray arrValues;

    pDbHatch->getGradientColors(arrColors, arrValues);

    OdArray<OdDgGradientKey> arrKeys;

    for( OdUInt32 i = 0; i < arrValues.size(); i++ )
    {
      OdDgGradientKey curKey;
      curKey.dKeyPosition = arrValues[i];
      curKey.clrKeyColor = OdCmColorToRGB(arrColors[i]);
      arrKeys.push_back(curKey);
    }

    retGradient.setKeys(arrKeys);

    return retGradient;
  }

  //===================================================================================================

  OdDgDWGHatchPatternPtr getDwgHatchPattern( OdDbHatch* pDbHatch )
  {
    OdDgDWGHatchPatternPtr pRet = OdDgDWGHatchPattern::createObject();
    pRet->setPatternName(pDbHatch->patternName());

    OdDgDWGHatchPattern::DWGHatch hatchDef;

    for( OdInt32 i = 0; i < pDbHatch->numPatternDefinitions(); i++ )
    {
      double dAngle;
      OdGePoint2d ptBase, ptOffset;
      OdGeDoubleArray arrDashes;
      pDbHatch->getPatternDefinitionAt(i, dAngle, ptBase.x, ptBase.y, ptOffset.x, ptOffset.y, arrDashes);

      DWGHatchLine curLine;
      curLine.m_angle = dAngle;
      curLine.m_offset = ptOffset;
      curLine.m_throughPoint = ptBase;
      curLine.m_dashes = arrDashes;

      hatchDef.push_back(curLine);
    }

    pRet->setHatch(hatchDef);

    return pRet;
  }

  //===================================================================================================

  void applyFillColor(OdDgGraphicsElementPtr& pGrElm, OdUInt32 uFillColorIndex)
  {
    OdDgGraphicsElementPtr pElmToApply = pGrElm;

    OdDgElementIteratorPtr pIter;

    if (pGrElm->isKindOf(OdDgCellHeader3d::desc()))
    {
      OdDgCellHeader3dPtr pCellHeader = pGrElm;
      pIter = pCellHeader->createIterator();
    }
    else if (pGrElm->isKindOf(OdDgCellHeader3d::desc()))
    {
      OdDgCellHeader2dPtr pCellHeader = pGrElm;
      pIter = pCellHeader->createIterator();
    }

    if (!pIter.isNull() && !pIter->done())
    {
      for (; !pIter->done(); pIter->step())
      {
        OdDgElementPtr pItem = pIter->item().openObject(OdDg::kForWrite, true);

        if (pItem->isKindOf(OdDgGraphicsElement::desc()))
        {
          pElmToApply = pItem;
          break;
        }
      }
    }

    pElmToApply->setFillColor(uFillColorIndex);
  }

  //===================================================================================================

  void applyGradientFill(OdDgGraphicsElementPtr& pGrElm, const OdDgGradientFill& hatchGradient)
  {
    OdDgGraphicsElementPtr pElmToApply = pGrElm;

    OdDgElementIteratorPtr pIter;

    if( pGrElm->isKindOf(OdDgCellHeader3d::desc()) )
    {
      OdDgCellHeader3dPtr pCellHeader = pGrElm;
      pIter = pCellHeader->createIterator();
    }
    else if (pGrElm->isKindOf(OdDgCellHeader3d::desc()))
    {
      OdDgCellHeader2dPtr pCellHeader = pGrElm;
      pIter = pCellHeader->createIterator();
    }

    if( !pIter.isNull() && !pIter->done() )
    {
      for (; !pIter->done(); pIter->step())
      {
        OdDgElementPtr pItem = pIter->item().openObject(OdDg::kForWrite, true);

        if (pItem->isKindOf(OdDgGraphicsElement::desc()))
        {
          pElmToApply = pItem;
          break;
        }
      }
    }

    pElmToApply->setGradientFill(hatchGradient);
  }

  //===================================================================================================

  OdDgGraphicsElementPtr exportHatch( OdDbHatch* pDbHatch, OdDbEntity* pBaseEnt, OdDgElement* owner, bool bKeepAssoc = true)
  {
    OdDgGraphicsElementPtr pRet;

    if (pDbHatch->numLoops() == 0)
      return pRet;

    bool bBraekAssociation = !pDbHatch->associative() || !bKeepAssoc;

    OdArray<OdDgElementPtrHatchLoopAssoc> arrDgLoopAssoc;

    OdDgGraphicsElementPtr pDgHatch = createHatchGeometry(pDbHatch, pBaseEnt, owner->database(), arrDgLoopAssoc, bBraekAssociation);

    if (pDgHatch.isNull())
      return pRet;

    OdDgGraphicsElementPtr pAssocRegion;

    if (!bBraekAssociation)
    {
      copyElementProperties(pDgHatch, pBaseEnt, owner->database());
      pAssocRegion = createAssociveRegion(pDbHatch, pDgHatch, owner->database());
    }
    else
      pAssocRegion = pDgHatch;

    if (!pAssocRegion.isNull())
    {
      addElementToDatabase(pAssocRegion, owner);
      copyElementProperties(pAssocRegion, pBaseEnt, owner->database());
      appendOdDgElementToOwner(owner, pAssocRegion);

      if( !pDbHatch->objectId().isNull() )
        OdDgnExportContext::addDgnElement(pDbHatch->objectId(), pAssocRegion->elementId());
    }

    if (pDbHatch->isGradient())
    {
      OdDgGradientFill hatchGrad = getGradientFill(pDbHatch);
      applyGradientFill(pDgHatch, hatchGrad);
    }
    else if (pDbHatch->isSolidFill())
    {
      applyFillColor(pDgHatch, pDgHatch->getColorIndex());
    }
    else
    {
      OdDgDWGHatchPatternPtr pDgDwgHatchPattern = getDwgHatchPattern(pDbHatch);
      pAssocRegion->setHatchPattern(pDgDwgHatchPattern);

      if (!bBraekAssociation)
        pDgHatch->setInvisibleFlag(true, true);
    }

    if (!bBraekAssociation)
    {
      OdDgnExportHatchAssociation assocRecord;
      assocRecord.m_idDgHatch = pAssocRegion->elementId();

      for (OdUInt32 j = 0; j < arrDgLoopAssoc.size(); j++)
      {
        OdDgnExportHatchLoopAssociation loopAssoc;
        loopAssoc.m_idLoopElement = arrDgLoopAssoc[j].m_pElm->elementId();
        loopAssoc.m_arrAssociationIds = arrDgLoopAssoc[j].m_arrAssociatedObjects;
        assocRecord.m_arrLoopAssoc.push_back(loopAssoc);
      }

      OdDgnExportContext::addHatchAssociation(assocRecord);
    }

    return pAssocRegion;
  }

  //===================================================================================================

  void OdDbHatchExportPE::subExportElement(OdDbEntity* pDbEntity, OdDgElement* owner)
  {
    OdDbHatch* pDbHatch = (OdDbHatch*)(pDbEntity);

    exportHatch(pDbHatch, pDbHatch, owner);
  }

  //===================================================================================================

  bool wasElementIdAdded(const OdDgElementIdArray& arrAssocElementIds, const OdDgElementId& idAssoc)
  {
    unsigned int uIndex;
    return arrAssocElementIds.find(idAssoc, uIndex);
  }

  //===================================================================================================

  void restoreHatchAssociations()
  {
    for( OdUInt32 i = 0; i < OdDgnExportContext::getHatchAssociationsCount(); i++ )
    {
      OdDgnExportHatchAssociation hatchAssoc = OdDgnExportContext::getHatchAssociation(i);

      OdDgElementId idAssocRgn = hatchAssoc.m_idDgHatch;

      if( idAssocRgn.isNull() )
        continue;

      OdDgElementPtr pAssocRgnElm = idAssocRgn.openObject(OdDg::kForWrite);

      if( pAssocRgnElm.isNull() )
        continue;

      OdDgElementIdArray arrAssocElementIds;

      for( OdUInt32 j = 0; j < hatchAssoc.m_arrLoopAssoc.size(); j++ )
      {
        OdDgElementId idDgLoop = hatchAssoc.m_arrLoopAssoc[j].m_idLoopElement;

        if( idDgLoop.isNull() )
          continue;

        OdDgElementPtr pDgLoop = idDgLoop.openObject(OdDg::kForWrite);

        if (pDgLoop.isNull())
          continue;

        OdDgDepLinkageElementIdPtr pDepLinkage = OdDgDepLinkageElementId::createObject();
        pDepLinkage->setAppId(10002);
        pDepLinkage->setAppValue(1);

        for( OdUInt32 k = 0; k < hatchAssoc.m_arrLoopAssoc[j].m_arrAssociationIds.size(); k++ )
        {
          OdDgElementId idAssoc = OdDgnExportContext::getElementId(hatchAssoc.m_arrLoopAssoc[j].m_arrAssociationIds[k]);

          if (!idAssoc.isNull())
          {
            if( !wasElementIdAdded(arrAssocElementIds, idAssoc) )
              arrAssocElementIds.push_back(idAssoc);

            pDepLinkage->add(idAssoc.getHandle());
          }
        }

        if( pDepLinkage->getCount() )
          pDgLoop->addLinkage(pDepLinkage->getPrimaryId(), pDepLinkage.get());
      }

      if( !arrAssocElementIds.isEmpty() )
      {
        if( pAssocRgnElm->isKindOf(OdDgAssocRegion2d::desc()) )
        {
          OdDgAssocRegion2dPtr pAssoc2d = pAssocRgnElm;
          pAssoc2d->setDependedElements(arrAssocElementIds);
        }
        else if (pAssocRgnElm->isKindOf(OdDgAssocRegion3d::desc()))
        {
          OdDgAssocRegion3dPtr pAssoc3d = pAssocRgnElm;
          pAssoc3d->setDependedElements(arrAssocElementIds);
        }
      }
    }

    OdDgnExportContext::clearHatchAssociations();
  }

  //===================================================================================================

  void OdDbMPolygonExportPE::subExportElement(OdDbEntity* pDbEntity, OdDgElement* owner)
  {
    OdDbMPolygon* pDbMPolygonEnt = (OdDbMPolygon*)(pDbEntity);

    if (!pDbMPolygonEnt)
      return;

    const OdDbMPolygon* pDbMPolygon = const_cast<OdDbMPolygon*>(pDbMPolygonEnt);

    OdDbHatch* pDbHatch = pDbMPolygon->hatch();

    if (!pDbHatch)
      return;

    OdDgGraphicsElementPtr pDgHatch = exportHatch(pDbHatch, pDbMPolygonEnt, owner, false);

    if (!pDgHatch.isNull())
      OdDgnExportContext::addDgnElement(pDbMPolygon->objectId(), pDgHatch->elementId());
  }
}
