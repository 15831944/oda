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

#include "OdaCommon.h"
#include "GiDrawObjectsForIntersect.h"
#include "Ge/GeCircArc3d.h"
#include "Ge/GeEllipArc3d.h"
#include "Ge/GePolyline3d.h"
#include "Ge/GeNurbCurve3d.h"
#include "Ge/GeLineSeg3d.h"
#include "Ge/GeLine3d.h"
#include "Ge/GeCurveCurveInt3d.h"

//----------------------------------------------------------
//
// OdGiDrawObjectsForIntersect
//
//----------------------------------------------------------

OdGiDrawObjectsForIntersect::OdGiDrawObjectsForIntersect() 
{ 
  OdGiBaseVectorizer::m_flags |= (kDrawLayerOff|kDrawLayerFrozen);
  OdGiGeometrySimplifier::setDrawContext(OdGiBaseVectorizer::drawContext());
  m_pModelToEyeProc->setDrawContext(OdGiBaseVectorizer::drawContext());
  output().setDestGeometry(*this);

  m_bClipEnabled     = false;
  m_bDrawFirstObject = true;
}

//=====================================================================================================

 OdGiDrawObjectsForIntersect::~OdGiDrawObjectsForIntersect()
 {
   clearCurveArray(m_firstCurves);
   clearCurveArray(m_secondCurves);
 }

//=====================================================================================================

OdResult OdGiDrawObjectsForIntersect::intersect( const OdGiDrawable* pObj1, 
                                                   const OdGiDrawable* pObj2, 
                                                     OdGiIntersectType intType, 
                                                       OdGePoint3dArray& arrIsectPts 
                                               )
{
  prepareToIntersect();

  if( pObj1 == pObj2 )
  {
    worldDrawObject( pObj1, true );
    getSelfIntersectPts( intType );
  }
  else
  {
    worldDrawObject( pObj1, true );
    worldDrawObject( pObj2, false );
    getIntersectPts( intType );

  }

  arrIsectPts = m_arrIntersectPts;

  clearCurveArray(m_firstCurves);
  clearCurveArray(m_secondCurves);

  return eOk;
}

//=====================================================================================================

void OdGiDrawObjectsForIntersect::worldDrawObject( const OdGiDrawable* pObj, bool bFirstObject )
{
  m_bDrawFirstObject = bFirstObject;

  if( !pObj->worldDraw(this) )
  {
    pObj->viewportDraw(this);
  }
}

//=====================================================================================================

void OdGiDrawObjectsForIntersect::prepareToIntersect()
{
  m_arrIntersectPts.clear();
  clearCurveArray(m_firstCurves);
  clearCurveArray(m_secondCurves);
  m_bDrawFirstObject = true;
  m_bClipEnabled = false;
}

//=====================================================================================================

void OdGiDrawObjectsForIntersect::clearCurveArray( OdArray<OdGeCurve3d*>& arrCurves )
{
  for( OdUInt32 i = 0; i < arrCurves.size(); i++ )
  {
    delete arrCurves[i];
    arrCurves[i] = 0;
  }

  arrCurves.clear();
}

//=====================================================================================================

bool OdGiDrawObjectsForIntersect::canExtendCurve( const OdArray<OdGeCurve3d*>& arrCurves )
{
  bool bRet = true;

  if( arrCurves.size() > 1 )
  {
    OdGePoint3d ptLastEnd;
    OdGePoint3d ptCurStart;
    OdGePoint3d ptStart;
    arrCurves[0]->hasEndPoint( ptLastEnd );

    for( OdUInt32 i = 1; i < arrCurves.size(); i++ )
    {
      arrCurves[i]->hasStartPoint(ptCurStart);

      if( !ptLastEnd.isEqualTo(ptCurStart) )
      {
        bRet = false;
        break;
      }

      arrCurves[i]->hasEndPoint( ptLastEnd );
    }

    if( ptLastEnd.isEqualTo(ptStart) )
    {
      bRet = false;
    }
  }
  else if( arrCurves.size() > 0 )
  {
    bRet = !arrCurves[0]->isClosed();
  }

  return bRet;
}

//=====================================================================================================

bool OdGiDrawObjectsForIntersect::isCorrectIntersect( const OdGeLineSeg3d* pLine, 
                                                        const OdGePoint3d& ptIsect, 
                                                          bool bExtendStart, 
                                                            bool bExtendEnd 
                                                     )
{
  bool bRet = false;

  double dStartIsect = pLine->startPoint().distanceTo( ptIsect );
  double dEndIsect   = pLine->endPoint().distanceTo( ptIsect );
  double dStartEnd   = pLine->length();

  if( OdZero(dStartEnd - dStartIsect - dEndIsect) )
  {
    bRet = true;
  }
  else if( bExtendStart && (dStartIsect < dEndIsect) )
  {
    bRet = true;
  }
  else if( bExtendEnd && (dStartIsect > dEndIsect) )
  {
    bRet = true;
  }

  return bRet;
}

//=====================================================================================================

void OdGiDrawObjectsForIntersect::getIntersectPts( OdGiDrawObjectsForIntersect::OdGiIntersectType intType )
{
  bool bFirstCurveExtendEnabled  = canExtendCurve( m_firstCurves );
  bool bSecondCurveExtendEnabled = canExtendCurve( m_secondCurves );

  for( OdUInt32 i = 0; i < m_firstCurves.size(); i++ )
  {
    bool bExtendStartFirst = (i == 0);
    bool bExtendEndFirst   = (i == m_firstCurves.size() - 1);

    if( !bFirstCurveExtendEnabled )
    {
      bExtendStartFirst = false;
      bExtendEndFirst   = false;
    }
    else if( intType == OdGiDrawObjectsForIntersect::kExtendArg || intType == OdGiDrawObjectsForIntersect::kOnBothOperands )
    {
      bExtendStartFirst = false;
      bExtendEndFirst   = false;
    }

    for( OdUInt32 j = 0; j < m_secondCurves.size(); j++ )
    {
      bool bExtendStartSecond = (j == 0);
      bool bExtendEndSecond   = (j == m_secondCurves.size() - 1);

      if( !bSecondCurveExtendEnabled )
      {
        bExtendStartSecond = false;
        bExtendEndSecond   = false;
      }
      else if( intType == OdGiDrawObjectsForIntersect::kExtendThis || intType == OdGiDrawObjectsForIntersect::kOnBothOperands )
      {
        bExtendStartSecond = false;
        bExtendEndSecond   = false;
      }

      intersectCurveCurve( m_firstCurves[i], m_secondCurves[j], bExtendStartFirst, bExtendEndFirst, bExtendStartSecond, bExtendEndSecond, m_arrIntersectPts );
    }
  }
}

//=====================================================================================================

void OdGiDrawObjectsForIntersect::addIsectPt( OdGePoint3dArray& arrIsectPts, 
                                                const OdGePoint3d& ptToAdd 
                                            )
{
  bool bAddPt = true;

  for( OdUInt32 i = 0; i < arrIsectPts.size(); i++ )
  {
    if( arrIsectPts[i].isEqualTo(ptToAdd) )
    {
      bAddPt = false;
      break;
    }
  }

  if( bAddPt )
  {
    arrIsectPts.push_back( ptToAdd );
  }
}

//=====================================================================================================

void OdGiDrawObjectsForIntersect::intersectCurveCurve( const OdGeCurve3d* pFirstCurve, 
                                                         const OdGeCurve3d* pSecondCurve, 
                                                           bool bExtendStartFirst, 
                                                             bool bExtendEndFirst, 
                                                               bool bExtendStartSecond, 
                                                                 bool bExtendEndSecond, 
                                                                   OdGePoint3dArray& arrIntersectPts 
                                                      )
{
  if( !pFirstCurve || !pSecondCurve )
  {
    return;
  }

  switch( pFirstCurve->type() )
  {
    case OdGe::kLineSeg3d:
    {
      switch( pSecondCurve->type() )
      {
        case OdGe::kLineSeg3d:
        {
          intersectLineLine( (OdGeLineSeg3d*)(pFirstCurve), (OdGeLineSeg3d*)(pSecondCurve), 
            bExtendStartFirst, bExtendEndFirst, bExtendStartSecond, bExtendEndSecond, arrIntersectPts );
        } break;

        case OdGe::kPolyline3d:
        {
          intersectLinePolyline( (OdGeLineSeg3d*)(pFirstCurve), (OdGePolyline3d*)(pSecondCurve), 
            bExtendStartFirst, bExtendEndFirst, bExtendStartSecond, bExtendEndSecond, arrIntersectPts );
        } break;

        case OdGe::kNurbCurve3d:
        {
          intersectLineNurbs( (OdGeLineSeg3d*)(pFirstCurve), (OdGeNurbCurve3d*)(pSecondCurve), 
            bExtendStartFirst, bExtendEndFirst, bExtendStartSecond, bExtendEndSecond, arrIntersectPts );
        } break;

        case OdGe::kCircArc3d:
        {
          intersectLineCircle( (OdGeLineSeg3d*)(pFirstCurve), (OdGeCircArc3d*)(pSecondCurve), 
            bExtendStartFirst, bExtendEndFirst, bExtendStartSecond, bExtendEndSecond, arrIntersectPts, true );
        } break;

        case OdGe::kEllipArc3d:
        {
          intersectLineEllipse( (OdGeLineSeg3d*)(pFirstCurve), (OdGeEllipArc3d*)(pSecondCurve), 
            bExtendStartFirst, bExtendEndFirst, bExtendStartSecond, bExtendEndSecond, arrIntersectPts, true );
        } break;
      }
    } break;

    case OdGe::kPolyline3d:
    {
      switch( pSecondCurve->type() )
      {
        case OdGe::kLineSeg3d:
        {
          intersectLinePolyline( (OdGeLineSeg3d*)(pSecondCurve), (OdGePolyline3d*)(pFirstCurve), 
            bExtendStartSecond, bExtendEndSecond, bExtendStartFirst, bExtendEndFirst, arrIntersectPts );
        } break;

        case OdGe::kPolyline3d:
        {
          intersectPolylinePolyline( (OdGePolyline3d*)(pFirstCurve), (OdGePolyline3d*)(pSecondCurve), 
            bExtendStartFirst, bExtendEndFirst, bExtendStartSecond, bExtendEndSecond, arrIntersectPts );
        } break;

        case OdGe::kNurbCurve3d:
        {
          intersectPolylineNurbs( (OdGePolyline3d*)(pFirstCurve), (OdGeNurbCurve3d*)(pSecondCurve), 
            bExtendStartFirst, bExtendEndFirst, bExtendStartSecond, bExtendEndSecond, arrIntersectPts );
        } break;

        case OdGe::kCircArc3d:
        {
          intersectPolylineCircle( (OdGePolyline3d*)(pFirstCurve), (OdGeCircArc3d*)(pSecondCurve), 
            bExtendStartFirst, bExtendEndFirst, bExtendStartSecond, bExtendEndSecond, arrIntersectPts );
        } break;

        case OdGe::kEllipArc3d:
        {
          intersectPolylineEllipse( (OdGePolyline3d*)(pFirstCurve), (OdGeEllipArc3d*)(pSecondCurve), 
            bExtendStartFirst, bExtendEndFirst, bExtendStartSecond, bExtendEndSecond, arrIntersectPts );
        } break;
      }
    } break;

    case OdGe::kNurbCurve3d:
    {
      switch( pSecondCurve->type() )
      {
        case OdGe::kLineSeg3d:
        {
          intersectLineNurbs( (OdGeLineSeg3d*)(pSecondCurve), (OdGeNurbCurve3d*)(pFirstCurve), 
            bExtendStartSecond, bExtendEndSecond, bExtendStartFirst, bExtendEndFirst, arrIntersectPts );
        } break;

        case OdGe::kPolyline3d:
        {
          intersectPolylineNurbs( (OdGePolyline3d*)(pSecondCurve), (OdGeNurbCurve3d*)(pFirstCurve), 
            bExtendStartSecond, bExtendEndSecond, bExtendStartFirst, bExtendEndFirst, arrIntersectPts );
        } break;

        case OdGe::kNurbCurve3d:
        {
          intersectNurbsNurbs( (OdGeNurbCurve3d*)(pFirstCurve), (OdGeNurbCurve3d*)(pSecondCurve), 
            bExtendStartFirst, bExtendEndFirst, bExtendStartSecond, bExtendEndSecond, arrIntersectPts );
        } break;

        case OdGe::kCircArc3d:
        {
          intersectNurbsCircle( (OdGeNurbCurve3d*)(pFirstCurve), (OdGeCircArc3d*)(pSecondCurve), 
            bExtendStartFirst, bExtendEndFirst, bExtendStartSecond, bExtendEndSecond, arrIntersectPts );
        } break;

        case OdGe::kEllipArc3d:
        {
          intersectNurbsEllipse( (OdGeNurbCurve3d*)(pFirstCurve), (OdGeEllipArc3d*)(pSecondCurve), 
            bExtendStartFirst, bExtendEndFirst, bExtendStartSecond, bExtendEndSecond, arrIntersectPts );
        } break;
      }
    } break;

    case OdGe::kCircArc3d:
    {
      switch( pSecondCurve->type() )
      {
        case OdGe::kLineSeg3d:
        {
          intersectLineCircle( (OdGeLineSeg3d*)(pSecondCurve), (OdGeCircArc3d*)(pFirstCurve), 
            bExtendStartSecond, bExtendEndSecond, bExtendStartFirst, bExtendEndFirst, arrIntersectPts, false );
        } break;

        case OdGe::kPolyline3d:
        {
          intersectPolylineCircle( (OdGePolyline3d*)(pSecondCurve), (OdGeCircArc3d*)(pFirstCurve), 
            bExtendStartSecond, bExtendEndSecond, bExtendStartFirst, bExtendEndFirst, arrIntersectPts );
        } break;

        case OdGe::kNurbCurve3d:
        {
          intersectNurbsCircle( (OdGeNurbCurve3d*)(pSecondCurve), (OdGeCircArc3d*)(pFirstCurve), 
            bExtendStartSecond, bExtendEndSecond, bExtendStartFirst, bExtendEndFirst, arrIntersectPts );
        } break;

        case OdGe::kCircArc3d:
        {
          intersectCircleCircle( (OdGeCircArc3d*)(pFirstCurve), (OdGeCircArc3d*)(pSecondCurve), 
            bExtendStartFirst, bExtendEndFirst, bExtendStartSecond, bExtendEndSecond, arrIntersectPts );
        } break;

        case OdGe::kEllipArc3d:
        {
          intersectCircleEllipse( (OdGeCircArc3d*)(pFirstCurve), (OdGeEllipArc3d*)(pSecondCurve), 
            bExtendStartFirst, bExtendEndFirst, bExtendStartSecond, bExtendEndSecond, arrIntersectPts );
        } break;
      }
    } break;

    case OdGe::kEllipArc3d:
    {
      switch( pSecondCurve->type() )
      {
        case OdGe::kLineSeg3d:
        {
          intersectLineEllipse( (OdGeLineSeg3d*)(pSecondCurve), (OdGeEllipArc3d*)(pFirstCurve), 
            bExtendStartSecond, bExtendEndSecond, bExtendStartFirst, bExtendEndFirst, arrIntersectPts, false );
        } break;

        case OdGe::kPolyline3d:
        {
          intersectPolylineEllipse( (OdGePolyline3d*)(pSecondCurve), (OdGeEllipArc3d*)(pFirstCurve), 
            bExtendStartSecond, bExtendEndSecond, bExtendStartFirst, bExtendEndFirst, arrIntersectPts );
        } break;

        case OdGe::kNurbCurve3d:
        {
          intersectNurbsEllipse( (OdGeNurbCurve3d*)(pSecondCurve), (OdGeEllipArc3d*)(pFirstCurve), 
            bExtendStartSecond, bExtendEndSecond, bExtendStartFirst, bExtendEndFirst, arrIntersectPts );
        } break;

        case OdGe::kCircArc3d:
        {
          intersectCircleEllipse( (OdGeCircArc3d*)(pSecondCurve), (OdGeEllipArc3d*)(pFirstCurve), 
            bExtendStartSecond, bExtendEndSecond, bExtendStartFirst, bExtendEndFirst, arrIntersectPts );
        } break;

        case OdGe::kEllipArc3d:
        {
          intersectEllipseEllipse( (OdGeEllipArc3d*)(pFirstCurve), (OdGeEllipArc3d*)(pSecondCurve), 
            bExtendStartFirst, bExtendEndFirst, bExtendStartSecond, bExtendEndSecond, arrIntersectPts );
        } break;
      }
    } break;
  }
}

//=====================================================================================================

void OdGiDrawObjectsForIntersect::getSelfIntersectPts( OdGiDrawObjectsForIntersect::OdGiIntersectType intType )
{
  for( OdUInt32 i = 0; i < m_firstCurves.size(); i++ )
  {
    if( m_firstCurves[i]->type() == OdGe::kPolyline3d )
    {
      selfIntersectPolyline( (OdGePolyline3d*)m_firstCurves[i], false, false, m_arrIntersectPts );
    }
    else if( m_firstCurves[i]->type() == OdGe::kNurbCurve3d )
    {
      selfIntersectNurbs( (OdGeNurbCurve3d*)m_firstCurves[i], false, false, m_arrIntersectPts );
    }

    for( OdUInt32 j = i+1; j < m_firstCurves.size(); j++ )
    {
      if( j == i+1 )
      {
        OdGePoint3dArray arrCurIsectPts;
        intersectCurveCurve( m_firstCurves[i], m_firstCurves[j], false, false, false, false, arrCurIsectPts );

        OdGePoint3d ptFirstEnd;
        m_firstCurves[i]->hasEndPoint( ptFirstEnd );

        for( OdUInt32 k = 0; k < arrCurIsectPts.size(); k++ )
        {
          if( !ptFirstEnd.isEqualTo(arrCurIsectPts[k]) )
          {
            addIsectPt( m_arrIntersectPts, arrCurIsectPts[k] );
          }
        }
      }
      else
      {
        intersectCurveCurve( m_firstCurves[i], m_firstCurves[j], false, false, false, false, m_arrIntersectPts );
      }
    }
  }
}

//=====================================================================================================

void OdGiDrawObjectsForIntersect::addGePolyline( OdUInt32 nPts, const OdGePoint3d* pPtsArr )
{
  if( nPts == 2 )
  {
    OdGeLineSeg3d* pLineSeg = new OdGeLineSeg3d( pPtsArr[0], pPtsArr[1] );

    if( m_bDrawFirstObject )
    {
      m_firstCurves.push_back( pLineSeg );
    }
    else
    {
      m_secondCurves.push_back( pLineSeg );
    }
  }
  else
  {
    OdGePolyline3d* pPolyLine3d = new OdGePolyline3d( nPts, pPtsArr );

    if( m_bDrawFirstObject )
    {
      m_firstCurves.push_back( pPolyLine3d );
    }
    else
    {
      m_secondCurves.push_back( pPolyLine3d );
    }
  }
}

//=====================================================================================================

void OdGiDrawObjectsForIntersect::addGePolyline( const OdGePoint3dArray& arrPts )
{
  addGePolyline( arrPts.size(), arrPts.asArrayPtr() );
}

//=====================================================================================================

void OdGiDrawObjectsForIntersect::pushClipBoundary(OdGiClipBoundary* pBoundary )
{
  OdGiBaseVectorizer::pushClipBoundary( pBoundary );

  m_bClipEnabled = true;
}

//=====================================================================================================

void OdGiDrawObjectsForIntersect::pushClipBoundary(OdGiClipBoundary* pBoundary, OdGiAbstractClipBoundary* pAbsBoundary)
{
  OdGiBaseVectorizer::pushClipBoundary( pBoundary, pAbsBoundary );

  m_bClipEnabled = true;
}

//=====================================================================================================

void OdGiDrawObjectsForIntersect::popClipBoundary()
{
  OdGiBaseVectorizer::popClipBoundary();

  m_bClipEnabled = false;
}

//=====================================================================================================

void OdGiDrawObjectsForIntersect::circleProc(const OdGePoint3d& center,
															                 double radius, 
                                                 const OdGeVector3d& normal,
                                                   const OdGeVector3d* pExtrusion
                                             )
{
  if( m_bClipEnabled )
  {
    OdGiGeometrySimplifier::circleProc( center, radius, normal, pExtrusion );
  }
  else
  {
    OdGeCircArc3d* pCircArc = new OdGeCircArc3d( center, normal, radius );

    if( m_bDrawFirstObject )
    {
      m_firstCurves.push_back( pCircArc );
    }
    else
    {
      m_secondCurves.push_back( pCircArc );
    }
  }
}

//=====================================================================================================

void OdGiDrawObjectsForIntersect::circleProc(const OdGePoint3d& p1,
															                 const OdGePoint3d& p2, 
                                                 const OdGePoint3d& p3,
                                                   const OdGeVector3d* pExtrusion)
{
  if( m_bClipEnabled )
  {
    OdGiGeometrySimplifier::circleProc( p1, p2, p3, pExtrusion );
  }
  else
  {
    OdGeCircArc3d* pArc = new OdGeCircArc3d(p1,p2,p3);
    pArc->setAngles(0, Oda2PI);

    if( m_bDrawFirstObject )
    {
      m_firstCurves.push_back( pArc );
    }
    else
    {
      m_secondCurves.push_back( pArc );
    }
  }
}

//=====================================================================================================

void OdGiDrawObjectsForIntersect::circularArcProc(const OdGePoint3d& center,
																				            double radius,
																				              const OdGeVector3d& normal,
																				                const OdGeVector3d& startVector,
																				                  double sweepAngle,
																				                    OdGiArcType arcType,
                                                              const OdGeVector3d* pExtrusion)
{
  if( m_bClipEnabled )
  {
    OdGiGeometrySimplifier::circularArcProc(center, radius, normal, startVector, sweepAngle, arcType, pExtrusion);
  }
  else
  {
    OdGeCircArc3d* pArc = new OdGeCircArc3d(center, normal, startVector, radius, 0, sweepAngle);

    if( m_bDrawFirstObject )
    {
      m_firstCurves.push_back( pArc );
    }
    else
    {
      m_secondCurves.push_back( pArc );
    }
  }
}

//=====================================================================================================

void OdGiDrawObjectsForIntersect::circularArcProc(const OdGePoint3d& start,
                                                    const OdGePoint3d& point,
                                                      const OdGePoint3d& end,
                                                        OdGiArcType arcType,
                                                          const OdGeVector3d* pExtrusion)
{
  if( m_bClipEnabled )
  {
    OdGiGeometrySimplifier::circularArcProc(start, point, end, arcType, pExtrusion);
  }
  else
  {
    OdGeCircArc3d* pArc = new OdGeCircArc3d(start, point, end);

    if( m_bDrawFirstObject )
    {
      m_firstCurves.push_back( pArc );
    }
    else
    {
      m_secondCurves.push_back( pArc );
    }
  }
}

//=====================================================================================================

void OdGiDrawObjectsForIntersect::polyline(OdInt32 nbPoints,
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
    addGePolyline(nbPoints, pVertexList);
  }
}

//=====================================================================================================

void OdGiDrawObjectsForIntersect::polylineOut(OdInt32 nbPoints, const OdGePoint3d* pVertexList)
{
  addGePolyline(nbPoints, pVertexList);
}

//=====================================================================================================

void OdGiDrawObjectsForIntersect::polygon(OdInt32 nbPoints, const OdGePoint3d* pVertexList)
{
  if( m_bClipEnabled )
  {
    m_bClipEnabled = false;

    OdGiBaseVectorizer::polygon( nbPoints, pVertexList );

    m_bClipEnabled = true;
  }
  else
  {
    addGePolyline(nbPoints, pVertexList);
  }
}

//=====================================================================================================

void OdGiDrawObjectsForIntersect::shellProc(OdInt32 nbVertex,
                                              const OdGePoint3d* pVertexList,
                                                OdInt32 faceListSize,
                                                  const OdInt32* pFaceList,
                                                    const OdGiEdgeData* pEdgeData,
                                                      const OdGiFaceData* pFaceData,
                                                        const OdGiVertexData* pVertexData)
{
  OdUInt32 uEdgeNumber = 0;
  const OdUInt8* pVisibility = NULL;
  
  if( pEdgeData )
    pVisibility = pEdgeData->visibility();

  for( OdInt32 i = 0; i < faceListSize; i++ )
  {
    OdInt32 nVertices = abs(pFaceList[i]);
    i++;

    OdGePoint3dArray arrLoopPts;
    for( OdInt32 j = 0; j < nVertices; j++, i++ )
    {
      if( pVisibility && j > 0 )
      {
        if( pVisibility[uEdgeNumber] == 0 )
        {
          if( arrLoopPts.size() > 1 )
          {
            addGePolyline(arrLoopPts);
          }

          arrLoopPts.clear();
        }
        else
        {
           arrLoopPts.push_back( pVertexList[pFaceList[i]] );
        }

        uEdgeNumber++;
      }
      else
      {
        arrLoopPts.push_back( pVertexList[pFaceList[i]] );
      }
    }

    i--;

    if( arrLoopPts.size() > 1 )
    {
      addGePolyline(arrLoopPts);
    }
  }
}

//=====================================================================================================

void OdGiDrawObjectsForIntersect::meshProc( OdInt32 rows, 
                                              OdInt32 columns,
														                    const OdGePoint3d* pVertexList, 
                                                  const OdGiEdgeData* pEdgeData,
														                        const OdGiFaceData* pFaceData, 
                                                      const OdGiVertexData* pVertexData 
                                           )
{
  if(rows < 0x7FFF && columns < 0x7FFF)
  {
    OdInt32 i,j;

    for( i = 0; i < columns; i++ )
    {
      OdGePoint3dArray arrRowPts;

      for( j = 0; j < rows; j++ )
      {
        arrRowPts.push_back( pVertexList[i*rows + j] );
      }

      addGePolyline(arrRowPts);
    }

    for( j = 0; j < rows; j++ )
    {
      OdGePoint3dArray arrColPts;

      for( i = 0; i < columns; i++ )
      {
        arrColPts.push_back( pVertexList[i*rows + j] );
      }

      addGePolyline(arrColPts);
    }
  }
  else
  {
    ODA_FAIL();
    // should be split
  }
}

//=====================================================================================================

void OdGiDrawObjectsForIntersect::xline(const OdGePoint3d& first, const OdGePoint3d& second)
{
  ODA_FAIL_ONCE();
  OdGiBaseVectorizer::xline( first, second );
}

//=====================================================================================================

void OdGiDrawObjectsForIntersect::ray(const OdGePoint3d& first, const OdGePoint3d& second)
{
  ODA_FAIL_ONCE();
  OdGiBaseVectorizer::ray( first, second );
}

//=====================================================================================================

void OdGiDrawObjectsForIntersect::pline(const OdGiPolyline& lwBuf, OdUInt32 fromIndex, OdUInt32 numSegs)
{
  OdGiBaseVectorizer::pline(lwBuf, fromIndex, numSegs);
}

//=====================================================================================================

void OdGiDrawObjectsForIntersect::text(const OdGePoint3d& position,
                                         const OdGeVector3d& normal,
                                           const OdGeVector3d& direction,
                                             double height,
                                               double width,
                                                 double oblique,
                                                   const OdString& smsg)
{
  // skip texts.
}

//=====================================================================================================

void OdGiDrawObjectsForIntersect::text(const OdGePoint3d& position,
                                         const OdGeVector3d& normal,
                                           const OdGeVector3d& direction,
                                             const OdChar* pMsg,
                                               OdInt32 length,
                                                 bool raw,
                                                   const OdGiTextStyle* pStyle)
{
  // skip texts.
}

//=====================================================================================================

void OdGiDrawObjectsForIntersect::nurbsProc(const OdGeNurbCurve3d& nurbsCurve)
{
  if( m_bDrawFirstObject )
  {
    m_firstCurves.push_back( new OdGeNurbCurve3d(nurbsCurve) );
  }
  else
  {
    m_secondCurves.push_back( new OdGeNurbCurve3d(nurbsCurve) );
  }
}

//=====================================================================================================

void OdGiDrawObjectsForIntersect::ellipArcProc( const OdGeEllipArc3d& ellipArc,
                                                  const OdGePoint3d* endPointOverrides,
                                                    OdGiArcType arcType,
                                                      const OdGeVector3d* pExtrusion )
{
  if( m_bDrawFirstObject )
  {
    m_firstCurves.push_back( new OdGeEllipArc3d(ellipArc) );
  }
  else
  {
    m_secondCurves.push_back( new OdGeEllipArc3d(ellipArc) );
  }
}

//=====================================================================================================

bool OdGiDrawObjectsForIntersect::ellipArcProc(const OdGeEllipArc3d& ellipArc, double width)
{
  if( m_bDrawFirstObject )
  {
    m_firstCurves.push_back( new OdGeEllipArc3d(ellipArc) );
  }
  else
  {
    m_secondCurves.push_back( new OdGeEllipArc3d(ellipArc) );
  }

  return true;
}

//=====================================================================================================

void OdGiDrawObjectsForIntersect::rasterImageProc(const OdGePoint3d& origin,
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
  OdGePoint3dArray arrPts;
  arrPts.push_back( origin );
  arrPts.push_back( origin + u );
  arrPts.push_back( origin + u + v );
  arrPts.push_back( origin + v );
  arrPts.push_back( origin );

  addGePolyline( arrPts );
}

//=====================================================================================================

OdGiRegenType OdGiDrawObjectsForIntersect::regenType() const
{
  return kOdGiForExplode;
}

//=====================================================================================================

void OdGiDrawObjectsForIntersect::intersectLineLine( const OdGeLineSeg3d* pFirstLine, 
                                                       const OdGeLineSeg3d* pSecondLine, 
                                                         bool bExtendStartFirst,  
                                                           bool bExtendEndFirst, 
                                                             bool bExtendStartSecond, 
                                                               bool bExtendEndSecond, 
                                                                 OdGePoint3dArray& arrIntersectPts 
                                                   )
{
  if( OdZero(pFirstLine->length()) || OdZero(pSecondLine->length()) )
  {
    return;
  }

  if( !bExtendEndFirst && !bExtendEndSecond && !bExtendStartFirst && !bExtendStartSecond )
  {
    OdGePoint3d ptIsect;

    if( pFirstLine->intersectWith( *pSecondLine, ptIsect) )
    {
      addIsectPt( arrIntersectPts, ptIsect );
    }

    return;
  }

  OdGeLine3d firstLine( pFirstLine->startPoint(), pFirstLine->endPoint() );
  OdGeLine3d secondLine( pSecondLine->startPoint(), pSecondLine->endPoint() );

  OdGePoint3d ptIsect;

  if( firstLine.intersectWith( secondLine, ptIsect ) )
  {
    bool bFirstIsOk  = isCorrectIntersect(pFirstLine, ptIsect, bExtendStartFirst, bExtendEndFirst);
    bool bSecondIsOk = false;
    
    if( bFirstIsOk )
    {
      bSecondIsOk = isCorrectIntersect(pSecondLine, ptIsect, bExtendStartSecond, bExtendEndSecond);
    }

    if( bFirstIsOk && bSecondIsOk )
    {
      addIsectPt( arrIntersectPts, ptIsect );
    }
  }
}

//=====================================================================================================

void OdGiDrawObjectsForIntersect::intersectLinePolyline( const OdGeLineSeg3d* pFirstLine, 
                                                           const OdGePolyline3d* pSecondPolyline, 
                                                             bool bExtendStartFirst,  
                                                               bool bExtendEndFirst, 
                                                                 bool bExtendStartSecond, 
                                                                   bool bExtendEndSecond, 
                                                                     OdGePoint3dArray& arrIntersectPts 
                                                        )
{
  if( pSecondPolyline->numFitPoints() == 2 )
  {
    OdGeLineSeg3d lineSeg( pSecondPolyline->fitPointAt(0), pSecondPolyline->fitPointAt(1) );

    intersectLineLine( pFirstLine, &lineSeg, bExtendStartFirst, bExtendEndFirst, bExtendStartSecond, bExtendEndSecond, arrIntersectPts );
  }
  else
  {
    if( !bExtendStartFirst && !bExtendEndFirst && !bExtendStartSecond && !bExtendEndSecond )
    {
      intersectLinePolylineNoExtend( pFirstLine, pSecondPolyline, arrIntersectPts );
      return;
    }

    for( OdInt32 i = 0; i < pSecondPolyline->numFitPoints()-1; i++ )
    {
      OdGeLineSeg3d lineSeg( pSecondPolyline->fitPointAt(i), pSecondPolyline->fitPointAt(i+1) );

      if( i == 0 )
      {
        intersectLineLine( pFirstLine, &lineSeg, bExtendStartFirst, bExtendEndFirst, bExtendStartSecond, false, arrIntersectPts );
      }
      else if( i == pSecondPolyline->numFitPoints() - 2 )
      {
        intersectLineLine( pFirstLine, &lineSeg, bExtendStartFirst, bExtendEndFirst, false, bExtendEndSecond, arrIntersectPts );
      }
      else
      {
        intersectLineLine( pFirstLine, &lineSeg, bExtendStartFirst, bExtendEndFirst, false, false, arrIntersectPts );
      }
    }
  }
}

//=====================================================================================================

void OdGiDrawObjectsForIntersect::intersectLineNurbs( const OdGeLineSeg3d* pFirstLine, 
                                                        const OdGeNurbCurve3d* pSecondNurbCurve, 
                                                          bool bExtendStartFirst,  
                                                            bool bExtendEndFirst, 
                                                              bool bExtendStartSecond, 
                                                                bool bExtendEndSecond, 
                                                                  OdGePoint3dArray& arrIntersectPts 
                                                     )
{
  if( OdZero(pFirstLine->length()))
  {
    return;
  }

  OdGeLineSeg3d lineSeg( *pFirstLine );

  if( bExtendStartFirst || bExtendEndFirst )
  {
    OdGePoint3d ptNurbStart = pSecondNurbCurve->startPoint();

    if( bExtendStartFirst )
    {
      double dExtendStart = ptNurbStart.distanceTo( pFirstLine->startPoint() ) + pSecondNurbCurve->length();

      OdGeVector3d vrExtend = pFirstLine->startPoint() - pFirstLine->endPoint();

      vrExtend.normalize();

      lineSeg.set(lineSeg.startPoint() + vrExtend * dExtendStart, lineSeg.endPoint());
    }

    if( bExtendEndFirst )
    {
      double dExtendEnd   = ptNurbStart.distanceTo( pFirstLine->endPoint() ) + pSecondNurbCurve->length();

      OdGeVector3d vrExtend = pFirstLine->endPoint() - pFirstLine->startPoint();

      vrExtend.normalize();

      lineSeg.set(lineSeg.startPoint(), lineSeg.endPoint() + vrExtend * dExtendEnd );
    }
  }

  OdGeCurveCurveInt3d curveIntersect( lineSeg, *pSecondNurbCurve );

  for( OdInt32 j = 0; j < curveIntersect.numIntPoints(); j++ )
  {
    addIsectPt( arrIntersectPts, curveIntersect.intPoint(j) );
  }
}

//=====================================================================================================

void OdGiDrawObjectsForIntersect::intersectLineCircle( const OdGeLineSeg3d* pFirstLine, 
                                                         const OdGeCircArc3d* pSecondCircArcCurve, 
                                                           bool bExtendStartFirst,  
                                                             bool bExtendEndFirst, 
                                                               bool bExtendStartSecond, 
                                                                 bool bExtendEndSecond, 
                                                                   OdGePoint3dArray& arrIntersectPts, 
                                                                     bool bUseLineDistance
                                                     )
{
  if( OdZero(pFirstLine->length()))
  {
    return;
  }

  OdGeCircArc3d circArc( *pSecondCircArcCurve );

  if( bExtendStartSecond || bExtendEndSecond )
  {
    circArc.setAngles(0, Oda2PI );
  }

  if( !bExtendStartFirst && !bExtendEndFirst )
  {
    OdGePoint3d ptIsect1, ptIsect2;
    int nIsects = 0;

    if( circArc.intersectWith( *pFirstLine, nIsects, ptIsect1, ptIsect2) )
    {
      if( nIsects == 1 )
      {
        addIsectPt( arrIntersectPts, ptIsect1);
      }
      else if( nIsects == 2 )
      {
        addIsectPt( arrIntersectPts, ptIsect1);
        addIsectPt( arrIntersectPts, ptIsect2);
      }
    }
  }
  else
  {
    OdGeLine3d line3d( pFirstLine->startPoint(), pFirstLine->endPoint() );

    OdGePoint3d ptIsect1, ptIsect2;
    int nIsects = 0;

    if( circArc.intersectWith( line3d, nIsects, ptIsect1, ptIsect2) )
    {
      if( nIsects == 1 )
      {
        if( isCorrectIntersect(pFirstLine, ptIsect1, bExtendStartFirst, bExtendEndFirst) )
        {
          addIsectPt( arrIntersectPts, ptIsect1 );
        }
      }
      else if( nIsects == 2 )
      {
        OdGePoint3d ptLineStart = pFirstLine->startPoint();

        if( bUseLineDistance && ptIsect1.distanceTo(ptLineStart) > ptIsect2.distanceTo(ptLineStart) )
        {
          OdGePoint3d ptExchange = ptIsect1;
          ptIsect1 = ptIsect2;
          ptIsect2 = ptExchange;
        }

        if( isCorrectIntersect(pFirstLine, ptIsect1, bExtendStartFirst, bExtendEndFirst) )
        {
          addIsectPt( arrIntersectPts, ptIsect1 );
        }

        if( isCorrectIntersect(pFirstLine, ptIsect2, bExtendStartFirst, bExtendEndFirst) )
        {
          addIsectPt( arrIntersectPts, ptIsect2 );
        }
      }
    }
  }
}

//=====================================================================================================

void OdGiDrawObjectsForIntersect::intersectLineEllipse( const OdGeLineSeg3d* pFirstLine, 
                                                          const OdGeEllipArc3d* pSecondEllipArcCurve, 
                                                            bool bExtendStartFirst,  
                                                              bool bExtendEndFirst, 
                                                                bool bExtendStartSecond, 
                                                                  bool bExtendEndSecond, 
                                                                    OdGePoint3dArray& arrIntersectPts, 
                                                                      bool bUseLineDistance
                                                       )
{
  if( OdZero(pFirstLine->length()))
  {
    return;
  }

  OdGeEllipArc3d ellipArc( *pSecondEllipArcCurve );

  if( bExtendStartSecond || bExtendEndSecond )
  {
    ellipArc.setAngles(0, Oda2PI );
  }

  if( !bExtendStartFirst && !bExtendEndFirst )
  {
    OdGePoint3d ptIsect1, ptIsect2;
    int nIsects = 0;

    if( ellipArc.intersectWith( *pFirstLine, nIsects, ptIsect1, ptIsect2) )
    {
      if( nIsects == 1 )
      {
        addIsectPt( arrIntersectPts, ptIsect1 );
      }
      else if( nIsects == 2 )
      {
        addIsectPt( arrIntersectPts, ptIsect1 );
        addIsectPt( arrIntersectPts, ptIsect2 );
      }
    }
  }
  else
  {
    OdGeLine3d line3d( pFirstLine->startPoint(), pFirstLine->endPoint() );

    OdGePoint3d ptIsect1, ptIsect2;
    int nIsects = 0;

    if( ellipArc.intersectWith( line3d, nIsects, ptIsect1, ptIsect2) )
    {
      if( nIsects == 1 )
      {
        if( isCorrectIntersect(pFirstLine, ptIsect1, bExtendStartFirst, bExtendEndFirst) )
        {
          addIsectPt( arrIntersectPts, ptIsect1 );
        }
      }
      else if( nIsects == 2 )
      {
        OdGePoint3d ptLineStart = pFirstLine->startPoint();

        if( bUseLineDistance && ptIsect1.distanceTo(ptLineStart) > ptIsect2.distanceTo(ptLineStart))
        {
          OdGePoint3d ptExchange = ptIsect1;
          ptIsect1 = ptIsect2;
          ptIsect2 = ptExchange;
        }

        if( isCorrectIntersect(pFirstLine, ptIsect1, bExtendStartFirst, bExtendEndFirst) )
        {
          addIsectPt( arrIntersectPts, ptIsect1 );
        }

        if( isCorrectIntersect(pFirstLine, ptIsect2, bExtendStartFirst, bExtendEndFirst) )
        {
          addIsectPt( arrIntersectPts, ptIsect2 );
        }
      }
    }
  }
}

//=====================================================================================================

void OdGiDrawObjectsForIntersect::intersectPolylinePolyline( const OdGePolyline3d* pFirstPolyline, 
                                                               const OdGePolyline3d* pSecondPolyline, 
                                                                 bool bExtendStartFirst,  
                                                                   bool bExtendEndFirst, 
                                                                     bool bExtendStartSecond, 
                                                                       bool bExtendEndSecond, 
                                                                         OdGePoint3dArray& arrIntersectPts 
                                                           )
{
  if( pFirstPolyline->numFitPoints() == 2 )
  {
    OdGeLineSeg3d lineSeg( pFirstPolyline->fitPointAt(0),pFirstPolyline->fitPointAt(1));
    intersectLinePolyline( &lineSeg, pSecondPolyline, bExtendStartFirst, bExtendEndFirst, 
                           bExtendStartSecond, bExtendEndSecond, arrIntersectPts );
    return;
  }

  for( OdInt32  i = 0; i < pFirstPolyline->numFitPoints()-1; i++ )
  {
    OdGeLineSeg3d lineSeg( pFirstPolyline->fitPointAt(i),pFirstPolyline->fitPointAt(i+1));

    if( i == 0 )
    {
      intersectLinePolyline( &lineSeg, pSecondPolyline, bExtendStartFirst, false, bExtendStartSecond, 
                             bExtendEndSecond, arrIntersectPts );
    }
    else if( i == pFirstPolyline->numFitPoints() - 2 )
    {     
      intersectLinePolyline( &lineSeg, pSecondPolyline, false, bExtendEndFirst, bExtendStartSecond, 
                             bExtendEndSecond, arrIntersectPts );
    }
    else
    {   
      intersectLinePolyline( &lineSeg, pSecondPolyline, false, false, bExtendStartSecond, 
                             bExtendEndSecond, arrIntersectPts );
    }
  }
}

//=====================================================================================================

void OdGiDrawObjectsForIntersect::intersectPolylineNurbs( const OdGePolyline3d* pFirstPolyline, 
                                                            const OdGeNurbCurve3d* pSecondNurbCurve, 
                                                              bool bExtendStartFirst,  
                                                                bool bExtendEndFirst, 
                                                                  bool bExtendStartSecond, 
                                                                    bool bExtendEndSecond, 
                                                                      OdGePoint3dArray& arrIntersectPts )
{
  if( pFirstPolyline->numFitPoints() == 2 )
  {
    OdGeLineSeg3d lineSeg( pFirstPolyline->fitPointAt(0),pFirstPolyline->fitPointAt(1));
    intersectLineNurbs( &lineSeg, pSecondNurbCurve, bExtendStartFirst, bExtendEndFirst, 
                        bExtendStartSecond, bExtendEndSecond, arrIntersectPts );
    return;
  }

  for( OdInt32  i = 0; i < pFirstPolyline->numFitPoints()-1; i++ )
  {
    OdGeLineSeg3d lineSeg( pFirstPolyline->fitPointAt(i),pFirstPolyline->fitPointAt(i+1));

    if( i == 0 )
    {
      intersectLineNurbs( &lineSeg, pSecondNurbCurve, bExtendStartFirst, false, bExtendStartSecond, 
                          bExtendEndSecond, arrIntersectPts );
    }
    else if( i == pFirstPolyline->numFitPoints() - 2 )
    {     
      intersectLineNurbs( &lineSeg, pSecondNurbCurve, false, bExtendEndFirst, bExtendStartSecond, 
                          bExtendEndSecond, arrIntersectPts );
    }
    else
    {   
      intersectLineNurbs( &lineSeg, pSecondNurbCurve, false, false, bExtendStartSecond, 
                          bExtendEndSecond, arrIntersectPts );
    }
  }
}

//=====================================================================================================

void OdGiDrawObjectsForIntersect::intersectPolylineCircle( const OdGePolyline3d* pFirstPolyline, 
                                                             const OdGeCircArc3d* pSecondCircArc, 
                                                               bool bExtendStartFirst,  
                                                                 bool bExtendEndFirst, 
                                                                   bool bExtendStartSecond, 
                                                                     bool bExtendEndSecond, 
                                                                       OdGePoint3dArray& arrIntersectPts )
{
  if( pFirstPolyline->numFitPoints() == 2 )
  {
    OdGeLineSeg3d lineSeg( pFirstPolyline->fitPointAt(0),pFirstPolyline->fitPointAt(1));
    intersectLineCircle( &lineSeg, pSecondCircArc, bExtendStartFirst, bExtendEndFirst, 
                         bExtendStartSecond, bExtendEndSecond, arrIntersectPts );
    return;
  }

  for( OdInt32  i = 0; i < pFirstPolyline->numFitPoints()-1; i++ )
  {
    OdGeLineSeg3d lineSeg( pFirstPolyline->fitPointAt(i),pFirstPolyline->fitPointAt(i+1));

    if( i == 0 )
    {
      intersectLineCircle( &lineSeg, pSecondCircArc, bExtendStartFirst, false, bExtendStartSecond, 
                            bExtendEndSecond, arrIntersectPts );
    }
    else if( i == pFirstPolyline->numFitPoints() - 2 )
    {     
      intersectLineCircle( &lineSeg, pSecondCircArc, false, bExtendEndFirst, bExtendStartSecond, 
                            bExtendEndSecond, arrIntersectPts );
    }
    else
    {   
      intersectLineCircle( &lineSeg, pSecondCircArc, false, false, bExtendStartSecond, 
                            bExtendEndSecond, arrIntersectPts );
    }
  }
}

//=====================================================================================================

void OdGiDrawObjectsForIntersect::intersectPolylineEllipse( const OdGePolyline3d* pFirstPolyline, 
                                                              const OdGeEllipArc3d* pSecondEllipArc, 
                                                                bool bExtendStartFirst, 
                                                                  bool bExtendEndFirst, 
                                                                    bool bExtendStartSecond, 
                                                                      bool bExtendEndSecond, 
                                                                        OdGePoint3dArray& arrIntersectPts )
{
  if( pFirstPolyline->numFitPoints() == 2 )
  {
    OdGeLineSeg3d lineSeg( pFirstPolyline->fitPointAt(0),pFirstPolyline->fitPointAt(1));
    intersectLineEllipse( &lineSeg, pSecondEllipArc, bExtendStartFirst, bExtendEndFirst, 
                          bExtendStartSecond, bExtendEndSecond, arrIntersectPts );
    return;
  }

  for( OdInt32  i = 0; i < pFirstPolyline->numFitPoints()-1; i++ )
  {
    OdGeLineSeg3d lineSeg( pFirstPolyline->fitPointAt(i),pFirstPolyline->fitPointAt(i+1));

    if( i == 0 )
    {
      intersectLineEllipse( &lineSeg, pSecondEllipArc, bExtendStartFirst, false, bExtendStartSecond, 
                            bExtendEndSecond, arrIntersectPts );
    }
    else if( i == pFirstPolyline->numFitPoints() - 2 )
    {     
      intersectLineEllipse( &lineSeg, pSecondEllipArc, false, bExtendEndFirst, bExtendStartSecond, 
                            bExtendEndSecond, arrIntersectPts );
    }
    else
    {   
      intersectLineEllipse( &lineSeg, pSecondEllipArc, false, false, bExtendStartSecond, 
                            bExtendEndSecond, arrIntersectPts );
    }
  }
}

//=====================================================================================================

void OdGiDrawObjectsForIntersect::intersectNurbsNurbs( const OdGeNurbCurve3d* pFirstNurbCurve, 
                                                         const OdGeNurbCurve3d* pSecondNurbCurve, 
                                                           bool bExtendStartFirst,  
                                                             bool bExtendEndFirst, 
                                                               bool bExtendStartSecond, 
                                                                 bool bExtendEndSecond, 
                                                                   OdGePoint3dArray& arrIntersectPts 
                                                      )
{
  OdGeCurveCurveInt3d curveIntersect( *pFirstNurbCurve, *pSecondNurbCurve );

  for( OdInt32 j = 0; j < curveIntersect.numIntPoints(); j++ )
  {
    addIsectPt( arrIntersectPts, curveIntersect.intPoint(j) );
  }
}

//=====================================================================================================

void OdGiDrawObjectsForIntersect::intersectNurbsCircle( const OdGeNurbCurve3d* pFirstNurbCurve, 
                                                          const OdGeCircArc3d* pSecondCircArc, 
                                                            bool bExtendStartFirst,  
                                                              bool bExtendEndFirst, 
                                                                bool bExtendStartSecond, 
                                                                  bool bExtendEndSecond, 
                                                                    OdGePoint3dArray& arrIntersectPts )
{
  OdGeCircArc3d circArc( *pSecondCircArc );

  if( bExtendStartSecond || bExtendEndSecond )
  {
    circArc.setAngles(0, Oda2PI);
  }

  OdGeCurveCurveInt3d curveIntersect( *pFirstNurbCurve, circArc );

  for( OdInt32 j = 0; j < curveIntersect.numIntPoints(); j++ )
  {
    addIsectPt( arrIntersectPts, curveIntersect.intPoint(j) );
  }
}

//=====================================================================================================

void OdGiDrawObjectsForIntersect::intersectNurbsEllipse( const OdGeNurbCurve3d* pFirstNurbCurve, 
                                                           const OdGeEllipArc3d* pSecondEllipArc, 
                                                             bool bExtendStartFirst,  
                                                               bool bExtendEndFirst, 
                                                                 bool bExtendStartSecond, 
                                                                   bool bExtendEndSecond, 
                                                                     OdGePoint3dArray& arrIntersectPts )
{
  OdGeEllipArc3d ellipArc( * pSecondEllipArc );

  if( bExtendStartSecond || bExtendEndSecond )
  {
    ellipArc.setAngles(0, Oda2PI);
  }

  OdGeCurveCurveInt3d curveIntersect( *pFirstNurbCurve, ellipArc );

  for( OdInt32 j = 0; j < curveIntersect.numIntPoints(); j++ )
  {
    addIsectPt( arrIntersectPts, curveIntersect.intPoint(j) );
  }
}

//=====================================================================================================

void OdGiDrawObjectsForIntersect::intersectCircleCircle( const OdGeCircArc3d* pFirstCircArc, 
                                                           const OdGeCircArc3d* pSecondCircArc, 
                                                             bool bExtendStartFirst,  
                                                               bool bExtendEndFirst, 
                                                                 bool bExtendStartSecond, 
                                                                   bool bExtendEndSecond, 
                                                                     OdGePoint3dArray& arrIntersectPts 
                                                        )
{
  OdGeCircArc3d circArc1( *pFirstCircArc );
  OdGeCircArc3d circArc2( *pSecondCircArc );

  if( bExtendStartFirst || bExtendEndFirst )
  {
    circArc1.setAngles(0, Oda2PI);
  }

  if( bExtendStartSecond || bExtendEndSecond )
  {
    circArc2.setAngles(0, Oda2PI);
  }

  OdGePoint3d ptIsect1, ptIsect2;
  int         nIsects;

  if( circArc1.intersectWith( circArc2, nIsects, ptIsect1, ptIsect2 ) )
  {
    if( nIsects == 1 )
    {
      addIsectPt( arrIntersectPts, ptIsect1 );
    }
    else if( nIsects == 2 )
    {
      addIsectPt( arrIntersectPts, ptIsect1 );
      addIsectPt( arrIntersectPts, ptIsect2 );
    }
  }
}

//=====================================================================================================

void OdGiDrawObjectsForIntersect::intersectCircleEllipse( const OdGeCircArc3d* pFirstCircArc, 
                                                            const OdGeEllipArc3d* pSecondEllipArc, 
                                                              bool bExtendStartFirst,  
                                                                bool bExtendEndFirst, 
                                                                  bool bExtendStartSecond, 
                                                                    bool bExtendEndSecond, 
                                                                      OdGePoint3dArray& arrIntersectPts )
{
  OdGeCircArc3d circArc1( *pFirstCircArc );

  if( !OdZero(pSecondEllipArc->majorRadius() - pSecondEllipArc->minorRadius()) )
  {
    OdGeEllipArc3d ellipArc2( *pSecondEllipArc );

    if( bExtendStartFirst || bExtendEndFirst )
    {
      circArc1.setAngles(0, Oda2PI);
    }

    if( bExtendStartSecond || bExtendEndSecond )
    {
      ellipArc2.setAngles(0, Oda2PI);
    }

    OdGeNurbCurve3d ellipNurbs( ellipArc2 );

    intersectNurbsCircle( &ellipNurbs, &circArc1, false, false, false, false, arrIntersectPts );
  }
  else
  {
    OdGeCircArc3d circArc2( pSecondEllipArc->center(), pSecondEllipArc->normal(), pSecondEllipArc->majorAxis(), 
                            pSecondEllipArc->majorRadius(), pSecondEllipArc->startAng(), pSecondEllipArc->endAng() );

    if( bExtendStartSecond || bExtendEndSecond )
    {
      circArc2.setAngles(0, Oda2PI);
    }

    intersectCircleCircle( &circArc1, &circArc2, false, false, false, false, arrIntersectPts );
  }
}

//=====================================================================================================

void OdGiDrawObjectsForIntersect::intersectEllipseEllipse( const OdGeEllipArc3d* pFirstEllipArc, 
                                                             const OdGeEllipArc3d* pSecondEllipArc, 
                                                               bool bExtendStartFirst,  
                                                                 bool bExtendEndFirst, 
                                                                   bool bExtendStartSecond, 
                                                                     bool bExtendEndSecond, 
                                                                       OdGePoint3dArray& arrIntersectPts )
{
  if( pFirstEllipArc->isCircular() && pSecondEllipArc->isCircular() )
  {
    OdGeCircArc3d circArc1( pFirstEllipArc->center(), pFirstEllipArc->normal(), pFirstEllipArc->majorAxis(), 
                            pFirstEllipArc->majorRadius(), pFirstEllipArc->startAng(), pFirstEllipArc->endAng() );

    OdGeCircArc3d circArc2( pSecondEllipArc->center(), pSecondEllipArc->normal(), pSecondEllipArc->majorAxis(), 
                            pSecondEllipArc->majorRadius(), pSecondEllipArc->startAng(), pSecondEllipArc->endAng() );

    intersectCircleCircle( &circArc1, &circArc2, bExtendStartFirst, bExtendEndFirst, bExtendStartSecond, 
                           bExtendEndSecond, arrIntersectPts );
  }
  else if( pFirstEllipArc->isCircular() )
  {
    OdGeCircArc3d circArc1( pFirstEllipArc->center(), pFirstEllipArc->normal(), pFirstEllipArc->majorAxis(), 
                            pFirstEllipArc->majorRadius(), pFirstEllipArc->startAng(), pFirstEllipArc->endAng() );

    intersectCircleEllipse( &circArc1, pSecondEllipArc, bExtendStartFirst, bExtendEndFirst, bExtendStartSecond, 
                            bExtendEndSecond, arrIntersectPts );
  }
  else if( pSecondEllipArc->isCircular() )
  {
    OdGeCircArc3d circArc2( pSecondEllipArc->center(), pSecondEllipArc->normal(), pSecondEllipArc->majorAxis(), 
                            pSecondEllipArc->majorRadius(), pSecondEllipArc->startAng(), pSecondEllipArc->endAng() );

    intersectCircleEllipse( &circArc2, pFirstEllipArc, bExtendStartSecond, bExtendEndSecond, bExtendStartFirst, 
                            bExtendEndFirst, arrIntersectPts );
  }
  else
  {
    OdGeEllipArc3d ellipArc1( *pFirstEllipArc );

    if( bExtendStartFirst || bExtendEndFirst )
    {
      ellipArc1.setAngles(0, Oda2PI);
    }

    OdGeNurbCurve3d ellipNurbs( ellipArc1 );

    intersectNurbsEllipse( &ellipNurbs, pSecondEllipArc, false, false, bExtendStartSecond, bExtendEndSecond, arrIntersectPts );
  }
}

//=====================================================================================================

void OdGiDrawObjectsForIntersect::selfIntersectPolyline( const OdGePolyline3d* pPolyline, 
                                                           bool bExtendStart, 
                                                             bool bExtendEnd, 
                                                               OdGePoint3dArray& arrIntersectPts )
{
  for( OdInt32 i = 0; i < pPolyline->numFitPoints() - 2; i++ )
  {
    OdGeLineSeg3d lineSeg( pPolyline->fitPointAt(i), pPolyline->fitPointAt(i+1) );

    OdGePoint3dArray arrPolylineEndPts;

    for( OdInt32 j = i+2; j < pPolyline->numFitPoints(); j++ )
    {
      arrPolylineEndPts.push_back(pPolyline->fitPointAt(j));
    }

    bool bExtendStartCur = bExtendStart;

    if( i > 0 )
    {
      bExtendStartCur = false;
    }

    if( arrPolylineEndPts.size() > 1 )
    {
      OdGePolyline3d polyLineEnd( arrPolylineEndPts );
      intersectLinePolyline( &lineSeg, &polyLineEnd, bExtendStartCur, false, false, bExtendEnd, arrIntersectPts );
    }

    if( i == 0 )
    {
      if( (arrIntersectPts.size() > 0) && (arrIntersectPts.last().isEqualTo( pPolyline->fitPointAt(0))) )
      {
        arrIntersectPts.removeLast();
      }
    }
  }
}

//=====================================================================================================

void OdGiDrawObjectsForIntersect::selfIntersectNurbs( const OdGeNurbCurve3d* pNurbCurve, 
                                                        bool bExtendStart, 
                                                          bool bExtendEnd, 
                                                            OdGePoint3dArray& arrIntersectPts )
{
  double dTolerance = pNurbCurve->length()/1e4;
  OdGePoint3dArray arrSamplePts;
  pNurbCurve->getSamplePoints( pNurbCurve->startParam(), pNurbCurve->endParam(), dTolerance, arrSamplePts );
  OdGePolyline3d polyLine( arrSamplePts );
  selfIntersectPolyline( &polyLine, false, false, arrIntersectPts );
}

//=====================================================================================================

void OdGiDrawObjectsForIntersect::intersectLinePolylineNoExtend( const OdGeLineSeg3d* pFirstLine, 
                                                                   const OdGePolyline3d* pSecondPolyline,
                                                                     OdGePoint3dArray& arrIntersectPts )
{
  OdGeVector3d vrRef = pFirstLine->endPoint() - pFirstLine->startPoint();

  if( vrRef.isZeroLength() )
  {
    return;
  }

  vrRef.normalize();

  OdGeVector3d vrNormal = vrRef.perpVector();

  OdGeMatrix3d matTransform;
  matTransform.setCoordSystem( pFirstLine->startPoint(), vrRef, vrRef.crossProduct(vrNormal), vrNormal );
  OdGeMatrix3d matTransformBack = matTransform;
  matTransformBack = matTransformBack.invert();

  OdGeLineSeg3d  firstLine( *pFirstLine );
  firstLine.transformBy( matTransformBack );

  OdGePoint3dArray arrFitPts;

  for( OdInt32 i = 0; i < pSecondPolyline->numFitPoints(); i++ )
  {
    arrFitPts.push_back( pSecondPolyline->fitPointAt(i).transformBy( matTransformBack ) );
  }

  double dMaxX = firstLine.endPoint().x;
  double dMinX = firstLine.startPoint().x;

  for( OdUInt32 j = 0; j < arrFitPts.size()-1; j++ )
  {
    if( arrFitPts[j].x < dMinX && arrFitPts[j+1].x < dMinX )
    {
      continue;
    }

    if( arrFitPts[j].x > dMaxX && arrFitPts[j+1].x > dMaxX )
    {
      continue;
    }

    if( arrFitPts[j].y < 0 && arrFitPts[j+1].y < 0 )
    {
      continue;
    }

    if( arrFitPts[j].y > 0 && arrFitPts[j+1].y > 0 )
    {
      continue;
    }

    if( arrFitPts[j].z < 0 && arrFitPts[j+1].z < 0 )
    {
      continue;
    }

    if( arrFitPts[j].z > 0 && arrFitPts[j+1].z > 0 )
    {
      continue;
    }

    OdGeLineSeg3d polylineSeg( arrFitPts[j], arrFitPts[j+1] );

    OdGePoint3d ptIsect;

    if( firstLine.intersectWith( polylineSeg, ptIsect ) )
    {
      ptIsect = ptIsect.transformBy( matTransform );
      addIsectPt( arrIntersectPts, ptIsect );
    }
  }
}

//=====================================================================================================
