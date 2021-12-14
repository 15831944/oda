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

// GiDrawDgnElementForIntersect.h: interface for the OdGiDrawDgnElementForIntersect class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_GIDRAWOBJECTSFORINTERSECT_INCLUDED_)
#define _GIDRAWOBJECTSFORINTERSECT_INCLUDED_

#include "Gi/GiBaseVectorizer.h"
#include "StaticRxObject.h"
#include "Gi/GiGeometrySimplifier.h"
#include "Ge/GeCurve3dPtrArray.h"
#include "Ge/GePolyline3d.h"
#include "Ge/GeLine3d.h"

class OdGiDrawObjectsForIntersect : public OdGiBaseVectorizer
                                  , public OdGiGeometrySimplifier
{
public:

  enum OdGiIntersectType
  {
    kOnBothOperands    = 0,
    kExtendThis        = 1,
    kExtendArg         = 2,
    kExtendBoth        = 3
  };

public:

  OdGiDrawObjectsForIntersect();
  ~OdGiDrawObjectsForIntersect();
  OdResult intersect(const OdGiDrawable* pObj1, const OdGiDrawable* pObj2, OdGiIntersectType intType, 
                     OdGePoint3dArray& arrIsectPts);
  virtual void prepareToIntersect();
  virtual void worldDrawObject(const OdGiDrawable* pObj, bool bFirstObject);
  virtual void getIntersectPts( OdGiIntersectType intType );
  virtual void getSelfIntersectPts( OdGiIntersectType intType );

  // OdGiBaseVectorizer overrides
  //
  OdGiRegenType regenType() const;

  virtual void polyline(OdInt32 nbPoints,
    const OdGePoint3d* pVertexList,
    const OdGeVector3d* pNormal = NULL,
    OdGsMarker lBaseSubEntMarker = -1);
  virtual void polygon(OdInt32 nbPoints, const OdGePoint3d* pVertexList);
  virtual void pline(const OdGiPolyline& lwBuf, OdUInt32 fromIndex = 0, OdUInt32 numSegs = 0);
  virtual void xline(const OdGePoint3d&,  const OdGePoint3d&);
  virtual void ray(const OdGePoint3d&, const OdGePoint3d&);
  virtual void text(
    const OdGePoint3d& position,
    const OdGeVector3d& normal, 
    const OdGeVector3d& direction,
    double height, 
    double width, 
    double oblique, 
    const OdString& msg);
  virtual void text(
    const OdGePoint3d& position,
    const OdGeVector3d& normal, 
    const OdGeVector3d& direction,
    const OdChar* msg, 
    OdInt32 length, 
    bool raw, 
    const OdGiTextStyle* pTextStyle);

  // OdGiGeometrySimplifier overrides
  //
  virtual void circleProc(
    const OdGePoint3d& center, double radius, const OdGeVector3d& normal, const OdGeVector3d* pExtrusion = 0);

  virtual void circleProc(
    const OdGePoint3d&, const OdGePoint3d&, const OdGePoint3d&, const OdGeVector3d* pExtrusion = 0);

  virtual void circularArcProc(
    const OdGePoint3d& center,
    double radius,
    const OdGeVector3d& normal,
    const OdGeVector3d& startVector,
    double sweepAngle,
    OdGiArcType arcType = kOdGiArcSimple, const OdGeVector3d* pExtrusion = 0);

  virtual void circularArcProc(
    const OdGePoint3d& start,
    const OdGePoint3d& point,
    const OdGePoint3d& end,
    OdGiArcType arcType = kOdGiArcSimple, const OdGeVector3d* pExtrusion = 0);

  virtual void meshProc(OdInt32 rows,
                OdInt32 columns,
                const OdGePoint3d* pVertexList,
                const OdGiEdgeData* pEdgeData = NULL,
                const OdGiFaceData* pFaceData = NULL,
                const OdGiVertexData* pVertexData = NULL);

  virtual void shellProc(OdInt32 nbVertex,
          const OdGePoint3d* pVertexList,
          OdInt32 faceListSize,
          const OdInt32* pFaceList,
          const OdGiEdgeData* pEdgeData = NULL,
          const OdGiFaceData* pFaceData = NULL,
          const OdGiVertexData* pVertexData = NULL);

  virtual void polylineOut(OdInt32 nbPoints, const OdGePoint3d* pVertexList);

  virtual void nurbsProc(const OdGeNurbCurve3d& nurbsCurve);

  virtual void ellipArcProc(
    const OdGeEllipArc3d& ellipArc,
    const OdGePoint3d* endPointOverrides = 0,
    OdGiArcType arcType = kOdGiArcSimple,
    const OdGeVector3d* pExtrusion = 0);

  virtual bool ellipArcProc(const OdGeEllipArc3d& ellipArc, double width);

  virtual void rasterImageProc(const OdGePoint3d& origin,
    const OdGeVector3d& u,
    const OdGeVector3d& v,
    const OdGiRasterImage* pImage, 
    const OdGePoint2d* uvBoundary, 
    OdUInt32 numBoundPts,
    bool transparency = false,
    double brightness = 50.0,
    double contrast = 50.0,
    double fade = 0.0);

  virtual void pushClipBoundary(OdGiClipBoundary* pBoundary);
  virtual void pushClipBoundary(OdGiClipBoundary* pBoundary, OdGiAbstractClipBoundary* pClipInfo);
  virtual void popClipBoundary();

protected:

  void addGePolyline( const OdGePoint3dArray& arrPts );
  void addGePolyline( OdUInt32 nPts, const OdGePoint3d* pPtsArr );

  static void addIsectPt( OdGePoint3dArray& arrIsectPts, const OdGePoint3d& ptToAdd );

  static bool canExtendCurve( const OdArray<OdGeCurve3d*>& arrCurves );
  static bool isCorrectIntersect( const OdGeLineSeg3d* pLine, const OdGePoint3d& ptIsect, bool bExtendStart, bool bExtendEnd );
  static void clearCurveArray( OdArray<OdGeCurve3d*>& arrCurves );

  void intersectCurveCurve( const OdGeCurve3d* pFirstCurve, const OdGeCurve3d* pSecondCurve, 
                            bool bExtendStartFirst,  bool bExtendEndFirst, bool bExtendStartSecond, 
                            bool bExtendEndSecond, OdGePoint3dArray& arrIntersectPts );

  void intersectLineLine( const OdGeLineSeg3d* pFirstLine, const OdGeLineSeg3d* pSecondLine, 
                          bool bExtendStartFirst,  bool bExtendEndFirst, bool bExtendStartSecond, 
                          bool bExtendEndSecond, OdGePoint3dArray& arrIntersectPts );
  void intersectLinePolyline( const OdGeLineSeg3d* pFirstLine, const OdGePolyline3d* pSecondPolyline, 
                          bool bExtendStartFirst,  bool bExtendEndFirst, bool bExtendStartSecond, 
                          bool bExtendEndSecond, OdGePoint3dArray& arrIntersectPts );
  void intersectLineNurbs( const OdGeLineSeg3d* pFirstLine, const OdGeNurbCurve3d* pSecondNurbCurve, 
                          bool bExtendStartFirst,  bool bExtendEndFirst, bool bExtendStartSecond, 
                          bool bExtendEndSecond, OdGePoint3dArray& arrIntersectPts );
  void intersectLineCircle( const OdGeLineSeg3d* pFirstLine, const OdGeCircArc3d* pSecondCircArcCurve, 
                            bool bExtendStartFirst,  bool bExtendEndFirst, bool bExtendStartSecond, 
                            bool bExtendEndSecond, OdGePoint3dArray& arrIntersectPts, bool bUseLineDistance = true );
  void intersectLineEllipse( const OdGeLineSeg3d* pFirstLine, const OdGeEllipArc3d* pSecondEllipArcCurve, 
                            bool bExtendStartFirst,  bool bExtendEndFirst, bool bExtendStartSecond, 
                            bool bExtendEndSecond, OdGePoint3dArray& arrIntersectPts, bool bUseLineDistance = true);
  void intersectPolylinePolyline( const OdGePolyline3d* pFirstPolyline, const OdGePolyline3d* pSecondPolyline, 
                                  bool bExtendStartFirst,  bool bExtendEndFirst, bool bExtendStartSecond, 
                                  bool bExtendEndSecond, OdGePoint3dArray& arrIntersectPts );
  void intersectPolylineNurbs( const OdGePolyline3d* pFirstPolyline, const OdGeNurbCurve3d* pSecondNurbCurve, 
                               bool bExtendStartFirst,  bool bExtendEndFirst, bool bExtendStartSecond, 
                               bool bExtendEndSecond, OdGePoint3dArray& arrIntersectPts );
  void intersectPolylineCircle( const OdGePolyline3d* pFirstPolyline, const OdGeCircArc3d* pSecondCircArc, 
                                bool bExtendStartFirst,  bool bExtendEndFirst, bool bExtendStartSecond, 
                                bool bExtendEndSecond, OdGePoint3dArray& arrIntersectPts );
  void intersectPolylineEllipse( const OdGePolyline3d* pFirstPolyline, const OdGeEllipArc3d* pSecondEllipArc, 
                                bool bExtendStartFirst,  bool bExtendEndFirst, bool bExtendStartSecond, 
                                bool bExtendEndSecond, OdGePoint3dArray& arrIntersectPts );
  void intersectNurbsNurbs( const OdGeNurbCurve3d* pFirstNurbCurve, const OdGeNurbCurve3d* pSecondNurbCurve, 
                            bool bExtendStartFirst,  bool bExtendEndFirst, bool bExtendStartSecond, 
                            bool bExtendEndSecond, OdGePoint3dArray& arrIntersectPts );
  void intersectNurbsCircle( const OdGeNurbCurve3d* pFirstNurbCurve, const OdGeCircArc3d* pSecondCircArc, 
                             bool bExtendStartFirst,  bool bExtendEndFirst, bool bExtendStartSecond, 
                             bool bExtendEndSecond, OdGePoint3dArray& arrIntersectPts );
  void intersectNurbsEllipse( const OdGeNurbCurve3d* pFirstNurbCurve, const OdGeEllipArc3d* pSecondEllipArc, 
                              bool bExtendStartFirst,  bool bExtendEndFirst, bool bExtendStartSecond, 
                              bool bExtendEndSecond, OdGePoint3dArray& arrIntersectPts );
  void intersectCircleCircle( const OdGeCircArc3d* pFirstCircArc, const OdGeCircArc3d* pSecondCircArc, 
                              bool bExtendStartFirst,  bool bExtendEndFirst, bool bExtendStartSecond, 
                              bool bExtendEndSecond, OdGePoint3dArray& arrIntersectPts );
  void intersectCircleEllipse( const OdGeCircArc3d* pFirstCircArc, const OdGeEllipArc3d* pSecondEllipArc, 
                               bool bExtendStartFirst,  bool bExtendEndFirst, bool bExtendStartSecond, 
                               bool bExtendEndSecond, OdGePoint3dArray& arrIntersectPts );
  void intersectEllipseEllipse( const OdGeEllipArc3d* pFirstEllipArc, const OdGeEllipArc3d* pSecondEllipArc, 
                               bool bExtendStartFirst,  bool bExtendEndFirst, bool bExtendStartSecond, 
                               bool bExtendEndSecond, OdGePoint3dArray& arrIntersectPts );

  void selfIntersectPolyline( const OdGePolyline3d* pPolyline, bool bExtendStart, bool bExtendEnd, 
                              OdGePoint3dArray& arrIntersectPts );
  void selfIntersectNurbs( const OdGeNurbCurve3d* pNurbCurve, bool bExtendStart, bool bExtendEnd, 
                           OdGePoint3dArray& arrIntersectPts );

  void intersectLinePolylineNoExtend( const OdGeLineSeg3d* pFirstLine, const OdGePolyline3d* pSecondPolyline,
                                      OdGePoint3dArray& arrIntersectPts
                                    );

  bool                    m_bClipEnabled;
  bool                    m_bDrawFirstObject;
  OdArray<OdGeCurve3d*>   m_firstCurves;
  OdArray<OdGeCurve3d*>   m_secondCurves;
  OdGePoint3dArray        m_arrIntersectPts;
};


#endif // !defined(_GIDRAWOBJECTSFORINTERSECT_INCLUDED_)
