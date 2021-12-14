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
#include "BrepBuilderFillerModule.h"
#include "BrepBuilderInitialData.h"
#include "IMaterialAndColorHelper.h"
// br
#include "Br/BrBrep.h"
#include "Br/BrComplex.h"
#include "Br/BrShell.h"
#include "Br/BrFace.h"
#include "Br/BrLoop.h"
#include "Br/BrEdge.h"
#include "Br/BrBrepComplexTraverser.h"
#include "Br/BrComplexShellTraverser.h"
#include "Br/BrShellFaceTraverser.h"
#include "Br/BrFaceLoopTraverser.h"
#include "Br/BrLoopEdgeTraverser.h"
#include "Br/BrLoopVertexTraverser.h"
#include "Br/BrBrepFaceTraverser.h"
// bb
#include "BrepBuilder/BrepBuilder.h"
// ge
#include "Ge/GeGbl.h"
#include "Ge/GePoint3d.h"
#include "Ge/GePoint2d.h"
#include "Ge/GeExtents2d.h"
#include "Ge/GePoint2dArray.h"
// curve
#include "Ge/GeCurve2d.h"
#include "Ge/GeLineSeg2d.h"
#include "Ge/GeLineSeg3d.h"
#include "Ge/GeCircArc3d.h"
#include "Ge/GeEllipArc3d.h"
#include "Ge/GeNurbCurve2d.h"
#include "Ge/GeNurbCurve3d.h"
#include "Ge/GeExternalCurve3d.h"
// surf
#include "Ge/GeEllipCone.h"
#include "Ge/GeEllipCylinder.h"
#include "Ge/GeExternalSurface.h"
#include "Ge/GeExternalBoundedSurface.h"
// stl
#define STL_USING_VECTOR
#define STL_USING_ALGORITHM
#define STL_USING_UTILITY
#define STL_USING_MAP
#define STL_USING_SET
#define STL_USING_LIMITS
#include "OdaSTL.h"
//
#include "TPtr.h"
typedef TPtr< OdGeCurve3d, TObjDelete<OdGeCurve3d> > OdGeTempCurve3dPtr;
typedef TPtr< OdGeCurve2d, TObjDelete<OdGeCurve2d> > OdGeTempCurve2dPtr;
typedef TPtr< OdGeNurbCurve3d, TObjDelete<OdGeNurbCurve3d> > OdGeTempNurbCurve3dPtr;
typedef TPtr< OdGeNurbCurve2d, TObjDelete<OdGeNurbCurve2d> > OdGeTempNurbCurve2dPtr;
typedef TPtr< OdGeSurface, TObjDelete<OdGeSurface> > OdGeTempSurfacePtr;

// Helper functions
class OdBrepBuilderFillerHelper
{
  BrepBuilderInitialData& m_initialData;

  OdIMaterialAndColorHelper* m_pMaterialHelper;
  const OdBrepBuilderFillerParams& m_params;

  std::map<OdUInt64, OdUInt32> m_edges;
  std::map<OdUInt64, OdUInt32> m_vertices;

  double m_toleranceInterval;
  double m_toleranceDiffPoints;
  double m_toleranceRestore2dCurve;
  double m_toleranceCoincide;

  mutable std::set<const OdGeCurve3d*> m_edgeCurveChanged;// true if edge curve was fixed => required to update coedge curve

  // Structure for addFaceExplicitLoop
  struct EdgeCurveCompareData
  {
    const OdGeCurve3d* pCurve;
    const OdUInt32 idEdge;
    const OdGePoint3dArray aSamplePts;
    double dDist;

    EdgeCurveCompareData(const double distance = 0.0)
      : pCurve(NULL), idEdge(0), dDist(distance)
    {}

    EdgeCurveCompareData(const OdGeCurve3d* curve, const OdUInt32 id, const OdGePoint3dArray& points, const double distance)
      : pCurve(curve), idEdge(id), aSamplePts(points), dDist(distance)
    {}

    bool operator<(const EdgeCurveCompareData& other) const { return dDist < other.dDist; }
  };

  // Field for addFaceExplicitLoop
  std::multiset<EdgeCurveCompareData> m_edgesAdded;

public:
  static inline OdResult checkRet(OdResult status)
  {
#ifdef ODA_DIAGNOSTICS
    if (eOk != status) {
      return status;
    }
#endif
    return status;
  }

  explicit OdBrepBuilderFillerHelper(
    BrepBuilderInitialData& initialData,
    const OdBrepBuilderFiller& filler,
    OdIMaterialAndColorHelper* pMaterialHelper = NULL
  )
    : m_initialData(initialData)
    , m_pMaterialHelper(pMaterialHelper)
    , m_params(filler.params())
    , m_toleranceInterval(1e-9)// double d2dParamTol = 1e-9;//TODO: possible investigation needed
    , m_toleranceDiffPoints(1e-3)
    , m_toleranceRestore2dCurve(1e-6)
    , m_toleranceCoincide(1e-6)
  {}

  // Interval
  /** \details
    Test that scndInt interval is part of frstInt.
  */
  bool isIntervalContain(const OdGeInterval& frstInt, const OdGeInterval& scndInt) const;
  /** \details
    Test that scndInt interval is part of reversed frstInt.
  */
  bool isIntervalContainReversed(const OdGeInterval& frstInt, const OdGeInterval& scndInt) const;

  // Edge
  enum FixCurveStatus { eSuccess, eSkip, eFail };
  OdGeCurve3dPtr getEdgeCurve(const OdBrEdge& edge) const;
  bool fixEllipse(OdGeCurve3dPtr& pCurve, const OdBrEdge& edge) const;
  bool fixCircle(OdGeCurve3dPtr& pCurve, const OdBrEdge& edge) const;
  void fixEllipseRadiusRatio(OdGeCurve3d* pCurve) const;
  bool fixNurb(OdGeCurve3dPtr& pCurve, const OdBrEdge& edge) const;
  OdResult getEdgeCurveFixed(const OdBrEdge& edge, OdGeCurve3dPtr& pCurve) const;
  FixCurveStatus getCurveParams(const OdBrEdge& edge, const OdGeCurve3d* pCurve, OdGePoint3d& pntStart, OdGePoint3d& pntEnd, OdGeInterval& interval) const;
  void setArcInterval(OdGeCurve3d* pCurve, OdGeInterval& interval, OdGePoint3d& pntStart, OdGePoint3d& center, OdGeVector3d& major, OdGeVector3d& normal) const;

  // Coedge
  OdGeCurve2dPtr getParamCurve(const OdBrLoopEdgeTraverser& loEdTrav) const;
  OdResult checkNurb2dFor3d(const OdGeSurface* pSurf, const OdGeCurve3d* pCurve3d, OdGeCurve2d* pCurve2d) const;
  OdResult checkCurve2dFor3d(const OdGeSurface* pSurf, const OdGeCurve3d* pCurve3d, OdGeCurve2d* pCurve2d) const;
  void moveParamCurveNurbInterval(const OdGeSurface* pSurf, const OdGeCurve3d* pCurve3d, OdGeCurve2d* pCurve2d) const;
  OdResult moveParamCurveInterval(const OdGeSurface* pSurf, const OdGeCurve3d* pCurve3d, OdGeCurve2d* pCurve2d) const;
  void moveParamIntoExents(const OdGeSurface* pSurf, const OdGeCurve2d* pCurve2d, OdGePoint2d &pnt) const;
  //this routine compare a 2d curve direction with 3d curve direction
  OdResult fixParamCurve(const OdGeSurface* pSurf, const OdGeCurve3d* pCurve3d, OdGeCurve2dPtr& pCurve2d) const;
  OdResult createParamCurve(const OdGeSurface* pSurf, const OdGeCurve3d* pCurve3d, OdGeCurve2dPtr& pCurve2d) const;
  OdResult addFaceExplicitLoop(BrepBuilderInitialSurface& surfData, const OdBrFace& face);
  static bool needPCurve(const OdGeSurface* pSurf);

  // Face
  OdGeSurfacePtr checkExtSurface(const OdGeSurface* resSurf, const OdBrFace& face) const;
  OdGeSurfacePtr getFaceSurface(const OdBrFace& face) const;
  void fixEllipConeRRatio(OdGeSurface* resSurf) const;

  // Vertex
  BrepBuilderInitialEdge::VertexIndex addVertex(const OdBrVertex& vertex);

  // Loop
  OdResult performLoopWithApex(const OdBrLoop& loop, OdGeCurve3dPtr& pCurve3d, OdGeCurve2dPtr& pCurve2d, OdBrVertex* vertex = NULL) const;
  OdResult splitOuterLoops(BrepBuilderInitialSurface& surfData, BrepBuilderInitialSurfaceArray &arrSurfaces, int &nOuterLoops);

  // Topology
  OdResult performBrep(const OdBrBrep& brep);
  OdResult performComplex(const OdBrComplex& complex);
  OdResult performShell(const OdBrShell& shell, BrepBuilderShellsArray& arrShells);
  OdResult performFace(const OdBrFace& face, BrepBuilderInitialSurfaceArray &arrSurfaces);
  OdResult performLoop(const OdBrLoop& loop, BrepBuilderInitialSurface& surfData);

  static OdResult initFromImpl(OdBrepBuilderFiller& filler, OdBrepBuilder& builder, const BrepBuilderInitialData& data);

  void groupFaces(BrepBuilderInitialSurfaceArray& arrFaces, OdArray< std::set<unsigned int> >& gropedFaces);
  void findAdjacentFaces(BrepBuilderInitialSurfaceArray& arrFaces, unsigned int iCurrFace, std::set<unsigned int>& facesIndexes);


  static OdResult getDataFrom(
    BrepBuilderInitialData& data,
    const OdBrepBuilderFiller& filler,
    const OdBrBrep& brep,
    OdIMaterialAndColorHelper* materialHelper);
};

// Interval
bool OdBrepBuilderFillerHelper::isIntervalContain(const OdGeInterval& frstInt, const OdGeInterval& scndInt) const
{
  return OdLessOrEqual(frstInt.lowerBound(), scndInt.lowerBound(), m_toleranceInterval)
    && OdGreaterOrEqual(frstInt.upperBound(), scndInt.upperBound(), m_toleranceInterval);
}
bool OdBrepBuilderFillerHelper::isIntervalContainReversed(const OdGeInterval& frstInt, const OdGeInterval& scndInt) const
{
  return OdGreaterOrEqual(frstInt.lowerBound()*-1, scndInt.upperBound(), m_toleranceInterval)
    && OdLessOrEqual(frstInt.upperBound()*-1, scndInt.lowerBound(), m_toleranceInterval);
}

// Edge
OdGeCurve3dPtr OdBrepBuilderFillerHelper::getEdgeCurve(const OdBrEdge& edge) const
{
  OdGeCurve3dPtr curve3d(edge.getCurve());

  if (!curve3d)
  {
    OdGeNurbCurve3d nurbCurve3d;
    if (edge.getCurveAsNurb(nurbCurve3d)) {
      return static_cast<OdGeCurve3d*>(nurbCurve3d.copy());
    }
    return NULL;
  }

  OdGe::EntityId entType = curve3d->type();
  if (OdGe::kExternalCurve3d == entType)
  {
    OdGeCurve3d* resCurve3d = NULL;
    const OdGeExternalCurve3d* extCurve = static_cast<OdGeExternalCurve3d*>(curve3d.get());

    if (extCurve->isNativeCurve(resCurve3d))
    {
      OdGeCurve3dPtr pCurveStore(resCurve3d);

      OdGeInterval curvIntOrig, curvIntNew;
      curve3d->getInterval(curvIntOrig);
      pCurveStore->getInterval(curvIntNew);

      if (isIntervalContain(curvIntOrig, curvIntNew)
        || isIntervalContainReversed(curvIntOrig, curvIntNew)
        || m_params.isSkipCoedge2dCurve()
        || pCurveStore->type() == OdGe::kLineSeg3d)
      {
        curve3d = pCurveStore;
      }
    }
  }

  return curve3d;
}

OdBrepBuilderFillerHelper::FixCurveStatus OdBrepBuilderFillerHelper::getCurveParams(const OdBrEdge& edge, const OdGeCurve3d* pCurve,
  OdGePoint3d& pntStart, OdGePoint3d& pntEnd, OdGeInterval& interval) const
{
  OdBrVertex startVertex;
  OdBrVertex endVertex;
  if (!edge.getVertex1(startVertex) || !edge.getVertex2(endVertex)) {
    // seems only parasolid may be without vertices
    return eSkip; //can't get vertexes - skip this curve
  }

  if (edge.getOrientToCurve()) {
    pntStart = startVertex.getPoint();
    pntEnd = endVertex.getPoint();
  } else {
    pntEnd = startVertex.getPoint();
    pntStart = endVertex.getPoint();
  }

  OdGePoint3d curveStart;
  OdGePoint3d curveEnd;
  if (!pCurve->hasStartPoint(curveStart)
    || !pCurve->hasEndPoint(curveEnd))
  {
    return eFail;//Can't get vert points - this is error
  }

  if (pntStart.isEqualTo(curveStart, m_toleranceDiffPoints)) {
    return eSkip;//points are equals
  }
  pCurve->getInterval(interval);
  return eSuccess;//ok, we got params
}

void OdBrepBuilderFillerHelper::setArcInterval(OdGeCurve3d* pCurve, OdGeInterval& interval, OdGePoint3d& pntStart, OdGePoint3d& center, OdGeVector3d& major, OdGeVector3d& normal) const
{
  OdGeVector3d centerToStart = pntStart - center;
  if (major.isCodirectionalTo(centerToStart))
  {
    if (OdNegative(interval.lowerBound())) {
      pCurve->setInterval(OdGeInterval(0., interval.length()));
      m_edgeCurveChanged.insert(pCurve);
    }
  }
  else
  {
    double angle = major.angleTo(centerToStart, normal);

    if (!OdZero(angle))
    {
      pCurve->rotateBy(angle, normal, center);
      if (OdNegative(interval.lowerBound())) {
        pCurve->setInterval(OdGeInterval(0., interval.length()));
        m_edgeCurveChanged.insert(pCurve);
      }
    }
  }
#ifdef ODA_DIAGNOSTICS
  OdGePoint3d curveStart;
  pCurve->hasStartPoint(curveStart);
  ODA_ASSERT_ONCE(pCurve->hasStartPoint(curveStart));
  ODA_ASSERT_ONCE(pntStart.isEqualTo(curveStart, 1e-05));
#endif //  ODA_DIAGNOSTICS
}

bool OdBrepBuilderFillerHelper::fixEllipse(OdGeCurve3dPtr& pCurve, const OdBrEdge& edge) const
{
  OdGeEllipArc3d& geEllipse = static_cast<OdGeEllipArc3d&>(*pCurve);
  OdGePoint3d startPoint;
  OdGePoint3d endPoint;
  OdGeInterval interval;
  FixCurveStatus res = getCurveParams(edge, pCurve, startPoint, endPoint, interval);
  if (eSuccess != res) {
    return res == eSkip;
  }

  if (!geEllipse.isCircular() || !startPoint.isEqualTo(endPoint, m_toleranceDiffPoints))
  {
    double newStartParam = geEllipse.paramOf(startPoint);
    geEllipse.setInterval(OdGeInterval(newStartParam, newStartParam + interval.length()));
    pCurve = new OdGeNurbCurve3d(geEllipse);
    m_edgeCurveChanged.insert(pCurve);

    ODA_ASSERT_ONCE(static_cast<OdGeNurbCurve3d*>(pCurve.get())->startPoint().isEqualTo(startPoint, 1e-04));
    return true;
  }

  // closed circle
  OdGePoint3d center = geEllipse.center();
  OdGeVector3d major = geEllipse.majorAxis();
  OdGeVector3d normal = geEllipse.normal();

  ODA_ASSERT_ONCE(OdEqual(startPoint.distanceTo(center), geEllipse.majorRadius(), 1e-05));
  setArcInterval(pCurve, interval, startPoint, center, major, normal);
  return true;
}

bool OdBrepBuilderFillerHelper::fixCircle(OdGeCurve3dPtr& pCurve, const OdBrEdge& edge) const
{
  OdGePoint3d startPoint;
  OdGePoint3d endPoint;
  OdGeInterval interval;
  FixCurveStatus res = getCurveParams(edge, pCurve, startPoint, endPoint, interval);
  if (eSuccess != res) {
    return res == eSkip;
  }

  OdGeCircArc3d& geCircle = static_cast<OdGeCircArc3d&>(*pCurve);

  if (!startPoint.isEqualTo(endPoint, m_toleranceDiffPoints))
  {
    double newStartParam = pCurve->paramOf(startPoint);
    pCurve->setInterval(OdGeInterval(newStartParam, newStartParam + interval.length()));
    pCurve = new OdGeNurbCurve3d(geCircle);
    m_edgeCurveChanged.insert(pCurve);

    ODA_ASSERT_ONCE(static_cast<OdGeNurbCurve3d*>(pCurve.get())->startPoint().isEqualTo(startPoint, 1e-04));
    return true;
  }

  OdGePoint3d center = geCircle.center();
  OdGeVector3d major = geCircle.refVec();
  OdGeVector3d normal = geCircle.normal();

  ODA_ASSERT_ONCE(OdEqual(startPoint.distanceTo(center), geCircle.radius(), 1e-05));
  setArcInterval(pCurve, interval, startPoint, center, major, normal);
  return true;
}

void OdBrepBuilderFillerHelper::fixEllipseRadiusRatio(OdGeCurve3d* pCurve) const
{
  if (!m_params.isMakeEllipMajorGreaterMinor()) {
    return;
  }

  //major radius should be >= minor for acis
  OdGeEllipArc3d& geEllipse = static_cast<OdGeEllipArc3d&>(*pCurve);
  OdGeVector3d vecMajAxis = geEllipse.majorAxis();
  OdGeVector3d vecMinAxis = geEllipse.minorAxis();
  const double majorRadius = geEllipse.majorRadius();
  const double minorRadius = geEllipse.minorRadius();
  if (OdZero(majorRadius, m_toleranceInterval) || !vecMajAxis.isPerpendicularTo(vecMinAxis, OdGeContext::gTol)) {
    throw OdError(eNotImplementedYet);
  }
  if (!OdGreater(minorRadius / majorRadius, 1, m_toleranceInterval)) {
    return;
  }
  geEllipse.set(geEllipse.center(), vecMinAxis, vecMajAxis.negate(), minorRadius, majorRadius, geEllipse.startAng() - OdaPI2, geEllipse.endAng() - OdaPI2);
  m_edgeCurveChanged.insert(pCurve);
}

bool OdBrepBuilderFillerHelper::fixNurb(OdGeCurve3dPtr& pCurve, const OdBrEdge& edge) const
{
  OdGeNurbCurve3d& geNurb = static_cast<OdGeNurbCurve3d&>(*pCurve);
  OdGePoint3d startPoint;
  OdGePoint3d endPoint;
  OdGeInterval interval;
  FixCurveStatus res = getCurveParams(edge, pCurve, startPoint, endPoint, interval);
  if (eSuccess != res) {
    return res == eSkip;
  }
  if (startPoint.isEqualTo(endPoint, m_toleranceDiffPoints))
  {
    // Closed curve
    // isOn not implemented
    double splitParam = geNurb.paramOf(startPoint);
    OdGeInterval curve3dInt;
    geNurb.getInterval(curve3dInt);
    if (OdEqual(splitParam, curve3dInt.lowerBound(), m_toleranceDiffPoints)
      || OdEqual(splitParam, curve3dInt.upperBound(), m_toleranceDiffPoints))
    {
      // it seems that point is not on curve
      return true;
    }

    OdGeCurve3d* piece1 = NULL;
    OdGeCurve3d* piece2 = NULL;
    geNurb.getSplitCurves(splitParam, piece1, piece2);
    OdGeTempCurve3dPtr pPiece1(piece1);
    OdGeTempCurve3dPtr pPiece2(piece2);
    if (!pPiece1.isNull() && !pPiece2.isNull())
    {
      ODA_ASSERT_ONCE(OdGe::kNurbCurve3d == pPiece1->type());
      ODA_ASSERT_ONCE(OdGe::kNurbCurve3d == pPiece2->type());
      OdGeNurbCurve3d& geNurb1 = static_cast<OdGeNurbCurve3d&>(*pPiece1);
      OdGeNurbCurve3d& geNurb2 = static_cast<OdGeNurbCurve3d&>(*pPiece2);
      geNurb = geNurb2.joinWith(geNurb1);
      m_edgeCurveChanged.insert(pCurve);

      ODA_ASSERT_ONCE(geNurb.startPoint().isEqualTo(startPoint, 1e-02) && geNurb.endPoint().isEqualTo(endPoint, 1e-02));
    }
  }
  else
  {
    double startParamNew = geNurb.paramOf(startPoint);
    double endParamNew = geNurb.paramOf(endPoint);
    OdGeInterval curve3dInt;
    geNurb.getInterval(curve3dInt);
    if ((OdEqual(startParamNew, curve3dInt.lowerBound(), m_toleranceDiffPoints)
      && OdEqual(endParamNew, curve3dInt.upperBound(), m_toleranceDiffPoints)))
    {
      // it seems that point is not on curve
      return true;
    }

    double startParam = geNurb.startParam();
    double endParam = geNurb.endParam();
    if ((startParam < startParamNew && startParamNew < endParam) && (startParam < endParamNew && endParamNew < endParam))
    {
      if ((startParamNew > endParamNew) && geNurb.isClosed())
      {
        // remove middle of curve and merge second part with first
        OdGeTempNurbCurve3dPtr pNurb1(static_cast<OdGeNurbCurve3d*>(geNurb.copy()));
        OdGeTempNurbCurve3dPtr pNurb2(static_cast<OdGeNurbCurve3d*>(geNurb.copy()));
        if (!pNurb1.isNull() && !pNurb2.isNull())
        {
          pNurb1->hardTrimByParams(startParamNew, endParam);
          pNurb2->hardTrimByParams(startParam, endParamNew);
          geNurb = pNurb1->joinWith(*pNurb2);
          m_edgeCurveChanged.insert(pCurve);

          ODA_ASSERT_ONCE(geNurb.startPoint().isEqualTo(startPoint, 1e-02) && geNurb.endPoint().isEqualTo(endPoint, 1e-02));
        }
      }
      else
      {
        // trim curve
        ODA_ASSERT_ONCE(startParamNew < endParamNew);
        geNurb.hardTrimByParams(startParamNew, endParamNew);
        m_edgeCurveChanged.insert(pCurve);

        ODA_ASSERT_ONCE(geNurb.startPoint().isEqualTo(startPoint, 1e-02) && geNurb.endPoint().isEqualTo(endPoint, 1e-02));
      }
    }
  }

  return true;
}

OdResult OdBrepBuilderFillerHelper::getEdgeCurveFixed(const OdBrEdge& edge, OdGeCurve3dPtr& pCurve) const
{
  pCurve = getEdgeCurve(edge);

  if (pCurve.isNull()) {
    return checkRet(eNullEdgeCurve);
  }

  bool result = true;
  OdGe::EntityId curveType = pCurve->type();
  switch (curveType)
  {
  case OdGe::kEllipArc3d:
  {
    result = fixEllipse(pCurve, edge);
    curveType = pCurve->type(); // become useless if fixEllipse doesn't change curve type
    if (OdGe::kEllipArc3d == curveType) {
      fixEllipseRadiusRatio(pCurve);
    }
    break;
  }
  case OdGe::kCircArc3d: result = fixCircle(pCurve, edge); break;
  case OdGe::kNurbCurve3d: result = fixNurb(pCurve, edge); break;
  }

  return checkRet(result ? eOk : eCurveEndsMissed);
}

// Coedge
OdGeCurve2dPtr OdBrepBuilderFillerHelper::getParamCurve(const OdBrLoopEdgeTraverser& loEdTrav) const
{
  OdGeNurbCurve2d nurbCurve2d;
  if (odbrOK == loEdTrav.getParamCurveAsNurb(nurbCurve2d)) {
    return static_cast<OdGeCurve2d*>(nurbCurve2d.copy());
  }

  OdGeTempCurve2dPtr paramCurve(loEdTrav.getParamCurve());
  if (paramCurve.isNull()) return NULL;

  // Acis bb required nurb 2d curve. Another (bim,prc) bb doesn't use 2d curve.
  if (OdGe::kNurbCurve2d == paramCurve->type()) return paramCurve.detach();
  return OdGeNurbCurve2d::convertFrom(paramCurve.get(), m_toleranceRestore2dCurve, true);
}

OdResult OdBrepBuilderFillerHelper::checkNurb2dFor3d(const OdGeSurface* pSurf, const OdGeCurve3d* pCurve3d,
  OdGeCurve2d* pCurve2d) const
{
  OdGeNurbCurve2d& geNurb2d = static_cast<OdGeNurbCurve2d&>(*pCurve2d);

  OdGePoint3d curve3dStart;
  OdGePoint3d curve3dEnd;
  OdGePoint2d curve2dStart = geNurb2d.startPoint();
  OdGePoint2d curve2dEnd = geNurb2d.endPoint();

  if (!pCurve3d->hasStartPoint(curve3dStart)
    || !pCurve3d->hasEndPoint(curve3dEnd))
  {
    return checkRet(eCurveEndsMissed);
  }

  OdGePoint3d surf3dStart = pSurf->evalPoint(curve2dStart);
  OdGePoint3d surf3dEnd = pSurf->evalPoint(curve2dEnd);

  if ((surf3dStart.isEqualTo(curve3dStart, m_toleranceDiffPoints)
    && surf3dEnd.isEqualTo(curve3dEnd, m_toleranceDiffPoints))
    || (surf3dStart.isEqualTo(curve3dEnd, m_toleranceDiffPoints)
    && surf3dEnd.isEqualTo(curve3dStart, m_toleranceDiffPoints)))
  {
    return checkRet(eOk);
  }
  // required to fix curve
  OdGePoint2d start2dPoint;
  OdGePoint2d end2dPoint;
  OdGe::EntityId surfType = pSurf->type();
  if (OdGe::kNurbSurface == surfType)
  {
    const OdGeNurbSurface* pNurbSurf = static_cast<const OdGeNurbSurface*>(pSurf);
    start2dPoint = pNurbSurf->paramOfPrec(curve3dStart);
    end2dPoint = pNurbSurf->paramOfPrec(curve3dEnd);
  }
  else
  {
    start2dPoint = pSurf->paramOf(curve3dStart);
    end2dPoint = pSurf->paramOf(curve3dEnd);
  }

  if (!curve3dStart.isEqualTo(pSurf->evalPoint(start2dPoint), m_toleranceDiffPoints)
    || !curve3dEnd.isEqualTo(pSurf->evalPoint(end2dPoint), m_toleranceDiffPoints))
  {
    return checkRet(ePointNotOnCurve);
  }

  if (start2dPoint.isEqualTo(end2dPoint, m_toleranceDiffPoints))
  {
    if (start2dPoint.isEqualTo(curve2dStart, m_toleranceDiffPoints)
      || start2dPoint.isEqualTo(curve2dEnd, m_toleranceDiffPoints))
    {
      return checkRet(ePointNotOnCurve);
    }

    double splitParam;
    if (!geNurb2d.isOn(start2dPoint, splitParam, m_toleranceDiffPoints))
    {
      //checking for periodic surface. Parametric curves may on start/end of period
      //and paramof() can return points on another "side" of this surface
      //so we need check it at start parameter and at period
      moveParamIntoExents(pSurf, pCurve2d, start2dPoint);
      if (!geNurb2d.isOn(start2dPoint, splitParam, m_toleranceDiffPoints)) {
        return checkRet(ePointNotOnCurve);
      }
    }
    OdGeCurve2d* piece1 = NULL;
    OdGeCurve2d* piece2 = NULL;
    geNurb2d.getSplitCurves(splitParam, piece1, piece2);
    OdGeTempCurve2dPtr pPiece1(piece1);
    OdGeTempCurve2dPtr pPiece2(piece2);
    if (!pPiece1.isNull() && !pPiece2.isNull())
    {
      ODA_ASSERT_ONCE(OdGe::kNurbCurve2d == pPiece1->type());
      ODA_ASSERT_ONCE(OdGe::kNurbCurve2d == pPiece2->type());
      OdGeNurbCurve2d& geNurb1 = static_cast<OdGeNurbCurve2d&>(*pPiece1);
      OdGeNurbCurve2d& geNurb2 = static_cast<OdGeNurbCurve2d&>(*pPiece2);
      geNurb2.transformBy(OdGeMatrix2d::translation(geNurb1.startPoint() - geNurb2.endPoint()));
      geNurb2d = geNurb2.joinWith(geNurb1);

      bool res = (pSurf->evalPoint(geNurb2d.startPoint()).isEqualTo(curve3dStart, m_toleranceDiffPoints)
        && pSurf->evalPoint(geNurb2d.endPoint()).isEqualTo(curve3dEnd, m_toleranceDiffPoints));
      return checkRet(res ? eOk : ePointNotOnCurve);
    }
  }
  else
  {
    double startParamNew;
    double endParamNew;
    if (!geNurb2d.isOn(start2dPoint, startParamNew, m_toleranceDiffPoints)
      || !geNurb2d.isOn(end2dPoint, endParamNew, m_toleranceDiffPoints))
    {
      return checkRet(ePointNotOnCurve);
    }

    double startParam = geNurb2d.startParam();
    double endParam = geNurb2d.endParam();
    if ((startParam < startParamNew && startParamNew < endParam) && (startParam < endParamNew && endParamNew < endParam))
    {
      if ((startParamNew > endParamNew) && geNurb2d.isClosed())
      {
        OdGeTempNurbCurve2dPtr pNurb1(static_cast<OdGeNurbCurve2d*>(geNurb2d.copy()));
        OdGeTempNurbCurve2dPtr pNurb2(static_cast<OdGeNurbCurve2d*>(geNurb2d.copy()));
        if (!pNurb1.isNull() && !pNurb2.isNull())
        {
          pNurb1->hardTrimByParams(startParamNew, endParam);
          pNurb2->hardTrimByParams(startParam, endParamNew);
          geNurb2d = pNurb1->joinWith(*pNurb2);

          bool res = (pSurf->evalPoint(geNurb2d.startPoint()).isEqualTo(curve3dStart, m_toleranceDiffPoints)
            && pSurf->evalPoint(geNurb2d.endPoint()).isEqualTo(curve3dEnd, m_toleranceDiffPoints));
          return checkRet(res ? eOk : ePointNotOnCurve);
        }
      }
      else
      {
        ODA_ASSERT_ONCE(startParamNew < endParamNew);
        try {
          geNurb2d.hardTrimByParams(startParamNew, endParamNew);
        } catch (...) {
          ODA_FAIL_M_ONCE("Unsucessfull hardTrimByParams for nurb curve.");
        }

        bool res = (pSurf->evalPoint(geNurb2d.startPoint()).isEqualTo(curve3dStart, m_toleranceDiffPoints)
          && pSurf->evalPoint(geNurb2d.endPoint()).isEqualTo(curve3dEnd, m_toleranceDiffPoints));
        return checkRet(res ? eOk : ePointNotOnCurve);
      }
    }
  }

  return checkRet(eInvalidCurve);
}

OdResult OdBrepBuilderFillerHelper::checkCurve2dFor3d(const OdGeSurface* pSurf, const OdGeCurve3d* pCurve3d, OdGeCurve2d* pCurve2d) const
{
  if (m_edgeCurveChanged.end() == m_edgeCurveChanged.find(pCurve3d)) {
    return checkRet(eOk);
  }

  OdGe::EntityId curve2dType = pCurve2d->type();
  if (OdGe::kNurbCurve2d == curve2dType) {
    return checkRet(checkNurb2dFor3d(pSurf, pCurve3d, pCurve2d));
  }

  // TODO not nurb curve
  return checkRet(eNotImplemented);
}

void OdBrepBuilderFillerHelper::moveParamCurveNurbInterval(const OdGeSurface* pSurf, const OdGeCurve3d* pCurve3d, OdGeCurve2d* pCurve2d) const
{
  OdGeNurbCurve2d& geNurb2d = static_cast<OdGeNurbCurve2d&>(*pCurve2d);

  OdGeInterval curv3dInt;
  pCurve3d->getInterval(curv3dInt);
#ifdef ODA_DIAGNOSTICS
  OdGeInterval curv2dInt;
  geNurb2d.getInterval(curv2dInt);
  ODA_ASSERT_ONCE(OdEqual(curv2dInt.lowerBound(), geNurb2d.knotAt(0)));
  ODA_ASSERT_ONCE(OdEqual(curv2dInt.upperBound(), geNurb2d.knotAt(geNurb2d.numKnots() - 1)));
#endif // ODA_DIAGNOSTICS

  OdGeKnotVector aKt = geNurb2d.knots();
  aKt.setRange(curv3dInt.lowerBound(), curv3dInt.upperBound());
  ODA_ASSERT_ONCE(aKt.length() == geNurb2d.numKnots());

  for (int i = 0; i < aKt.length(); ++i) {
    geNurb2d.setKnotAt(i, aKt[i]);
  }
}

void OdBrepBuilderFillerHelper::moveParamIntoExents(const OdGeSurface* pSurf, const OdGeCurve2d* pCurve2d, OdGePoint2d& pnt) const
{
  bool bClosedInU = pSurf->isClosedInU();
  bool bClosedInV = pSurf->isClosedInV();
  if (!bClosedInU && !bClosedInV) {
    return;
  }

  OdGeInterval iu, iv;
  pSurf->getEnvelope(iu, iv);

  OdGeExtents2d extents;
  OdGePoint2dArray pnts;
  OdGeInterval intCur;
  pCurve2d->getInterval(intCur);
  pCurve2d->getSamplePoints(intCur.lowerBound(), intCur.upperBound(), 0., pnts);
  extents.addPoints(pnts);

  if (bClosedInU)
  {
    double dI = iu.length();
    while (pnt.x > extents.maxPoint().x + m_toleranceInterval) pnt.x -= dI;
    while (pnt.x < extents.minPoint().x - m_toleranceInterval) pnt.x += dI;

    // uv should be as close to curveExtents as possible
    if (pnt.x > extents.maxPoint().x + m_toleranceInterval && pnt.x - extents.maxPoint().x > extents.minPoint().x - (pnt.x - dI)) {
      pnt.x -= dI;
    }
  }

  if (bClosedInV)
  {
    double dI = iv.length();
    while (pnt.y > extents.maxPoint().y + m_toleranceInterval) pnt.y -= dI;
    while (pnt.y < extents.minPoint().y - m_toleranceInterval) pnt.y += dI;

    // uv should be as close to curveExtents as possible
    if (pnt.y > extents.maxPoint().y + m_toleranceInterval && pnt.y - extents.maxPoint().y > extents.minPoint().y - (pnt.y - dI)) {
      pnt.y -= dI;
    }
  }
}

OdResult OdBrepBuilderFillerHelper::moveParamCurveInterval(const OdGeSurface* pSurf, const OdGeCurve3d* pCurve3d, OdGeCurve2d* pCurve2d) const
{
  OdGe::EntityId curve2dType = pCurve2d->type();
  if (OdGe::kNurbCurve2d == curve2dType) {
    moveParamCurveNurbInterval(pSurf, pCurve3d, pCurve2d);
    return checkRet(eOk);
  }

  // TODO not nurb curve
  return checkRet(eNotImplemented);
}

static OdGeCurve2d* restoreUvCurveAsNurb(const OdGeCurve3d* pCurve3d, const OdGeSurface* pSurf, double tol)
{
  OdGeTempCurve2dPtr pProjCurve(OdGeCurve2d::restoreUvCurve(pCurve3d, pSurf, tol));
  if (pProjCurve.isNull())
    return NULL;
  if (OdGe::kNurbCurve2d == pProjCurve->type())
    return pProjCurve.detach();
  return OdGeNurbCurve2d::convertFrom(pProjCurve.get(), OdGeTol(tol, OdGeContext::gTol.equalVector()));
}

OdResult OdBrepBuilderFillerHelper::createParamCurve(const OdGeSurface* pSurf, const OdGeCurve3d* pCurve3d, OdGeCurve2dPtr& pCurve2d) const
{
  ODA_ASSERT(pSurf && pCurve3d);
  //estimate tolerance for creating param curve
  OdGePoint3dArray arrCrvSamplePnts(21);
  pCurve3d->getSamplePoints(21, arrCrvSamplePnts);
  double dTol = 0.;
  for (OdGePoint3dArray::size_type iPoint = 0; iPoint < arrCrvSamplePnts.length(); ++iPoint)
  {
    OdGePoint3d p3dCurve = arrCrvSamplePnts[iPoint];
    OdGePoint2d p2d;
    pSurf->type() == OdGe::kNurbSurface ? p2d = static_cast<const OdGeNurbSurface*>(pSurf)->paramOfPrec(p3dCurve) : p2d = pSurf->paramOf(p3dCurve);
    OdGePoint3d p3dSurf = pSurf->evalPoint(p2d);
    double dDistance = p3dSurf.distanceTo(p3dCurve);
    if (dTol < dDistance) {
      dTol = dDistance;
    }
  }
  dTol = dTol + dTol * 0.1;
  pCurve2d = restoreUvCurveAsNurb(pCurve3d, pSurf, dTol > m_toleranceRestore2dCurve ? dTol : m_toleranceRestore2dCurve);
  if (!pCurve2d) {
    return checkRet(eCreateFailed);
  }
  return checkRet(eOk);
}
OdResult OdBrepBuilderFillerHelper::fixParamCurve(const OdGeSurface* pSurf, const OdGeCurve3d* pCurve3d, OdGeCurve2dPtr& pCurve2d) const
{
  if (!pCurve2d) {
    return checkRet(eOk);
  }

  // First update 2d curve if 3d was fixed
  OdResult status = checkCurve2dFor3d(pSurf, pCurve3d, pCurve2d);
  if (eOk != status) {
    return checkRet(status);
  }

  OdGeInterval curv3dInt, curv2dInt;
  if (m_params.isMake2dIntervalInclude3d())
  {
    pCurve3d->getInterval(curv3dInt);
    pCurve2d->getInterval(curv2dInt);

    if (!isIntervalContain(curv2dInt, curv3dInt)
      && !isIntervalContainReversed(curv2dInt, curv3dInt))
    {
      //2d and 3d intervals are different.
      status = moveParamCurveInterval(pSurf, pCurve3d, pCurve2d);
      if (eOk != status) {
        return checkRet(status);
      }
      pCurve2d->getInterval(curv2dInt);
    }
    else
    {
      //Brep Builder waits for int3d == int2d. They should be equal because tolerance for tedges (and tvertex) computed with
      //parametric curves and if spatr parameter of parcur != start parameter of edge curvre invalid tol valued will be computed.
      if (!(OdEqual(curv2dInt.lowerBound(), curv3dInt.lowerBound(), m_toleranceInterval) && OdEqual(curv2dInt.upperBound(), curv3dInt.upperBound(), m_toleranceInterval)))
      {
        if (needPCurve(pSurf)) {
          return checkRet(createParamCurve(pSurf, pCurve3d, pCurve2d));
        }
        return checkRet(eCreateFailed);
      }
    }
  }

  bool bCoincide(true);
  if (geIsDir2dOnSurfCoincide3d(pSurf, pCurve3d, pCurve2d, bCoincide, m_toleranceCoincide) == eOk)
  {
    if (!bCoincide) {
      pCurve2d->reverseParam();
      pCurve2d->getInterval(curv2dInt);
    }
  }
  else
  {
    pCurve2d = restoreUvCurveAsNurb(pCurve3d, pSurf, m_toleranceRestore2dCurve);
    if (!pCurve2d) {
      return checkRet(eCreateFailed);
    }
    pCurve2d->getInterval(curv2dInt);
  }

  if (m_params.isMake2dIntervalInclude3d())
  {
    if (isIntervalContain(curv2dInt, curv3dInt)) {
      return checkRet(eOk);
    }

    //should never be here
    ODA_FAIL_M_ONCE("Probably invalid 2d and 3d intervals");
    return checkRet(moveParamCurveInterval(pSurf, pCurve3d, pCurve2d));
  }

  return checkRet(eOk);
}

/** \details
   Compares two points arrays with tolerance specified and gets maximal distance between correspond points.

   \param pts1   [in] First array.
   \param pts2   [in] Second array.
   \param dDist [out] Maximal distance.
   \param tol    [in] Tolerance.
   \returns true if arrays are equal, false - otherwise.
*/
static bool arePointsEqual(const OdGePoint3dArray& pts1, const OdGePoint3dArray& pts2, double& dDist, const OdGeTol& tol = OdGeContext::gTol)
{
  dDist = 0.0;
  if (pts1.length() != pts2.length()) {
    return false;
  }

  for (unsigned int k = 0; k < pts1.length(); k++)
  {
    double dCurDist = (pts1[k] - pts2[k]).lengthSqrd();
    if (dCurDist > dDist) {
      dDist = dCurDist;
    }
    if (dCurDist > tol.equalPoint() * tol.equalPoint()) {
      return false;
    }
  }
  return true;
}

OdResult OdBrepBuilderFillerHelper::addFaceExplicitLoop(BrepBuilderInitialSurface& surfData, const OdBrFace& face)
{
  OdGeInterval ivU, ivV;
  surfData.pSurf->getEnvelope(ivU, ivV);

  if (surfData.pSurf->isKindOf(OdGe::kSphere)
    && surfData.pSurf->isClosedInU(m_toleranceInterval)
    && surfData.pSurf->isClosedInV(m_toleranceInterval))
  {
    // Do nothing for closed sphere
    return checkRet(eOk);
  }
  else if (surfData.pSurf->isKindOf(OdGe::kTorus)
    && surfData.pSurf->isClosedInU(m_toleranceInterval)
    && surfData.pSurf->isClosedInV(m_toleranceInterval))
  {
    // Do nothing for closed torus
    return checkRet(eOk);
  }
  else if (!surfData.pSurf->isNormalReversed()
    && ivU.isBounded() && ivV.isBounded()) // Add new loop by envelope borders
  {
    const unsigned int iBorders = 4;
    const unsigned int iPtsCnt = 7;
    OdGePoint2dArray aParams(iBorders);
    OdGeCurve3dPtrArray aIsoparamCurves(iBorders);
    OdGeCurve2dPtrArray aParamCurves(iBorders);
    OdBrepBuilder::EntityDirection aDirs[iBorders] = {
      OdBrepBuilder::kForward, OdBrepBuilder::kForward,
      OdBrepBuilder::kReversed, OdBrepBuilder::kReversed };

    aParams.resize(iBorders);
    aParamCurves.resize(iBorders);
    aIsoparamCurves.resize(iBorders);

    // Create borders curves
    aIsoparamCurves[0] = surfData.pSurf->makeIsoparamCurve(false, ivV.lowerBound());
    aIsoparamCurves[1] = surfData.pSurf->makeIsoparamCurve(true, ivU.upperBound());
    aIsoparamCurves[2] = surfData.pSurf->makeIsoparamCurve(false, ivV.upperBound());
    aIsoparamCurves[3] = surfData.pSurf->makeIsoparamCurve(true, ivU.lowerBound());

    // Check makeIsoparamCurve results (can be NULL)
    if (aIsoparamCurves[0].isNull() || aIsoparamCurves[1].isNull()
      || aIsoparamCurves[2].isNull() || aIsoparamCurves[3].isNull())
    {
      return checkRet(eNullEdgeCurve);
    }

    // Change loop direction for reversed case
    if (surfData.direction == OdBrepBuilder::kReversed)
    {
      aIsoparamCurves.reverse();
      aParamCurves.reverse();
      ODA_FAIL_M_ONCE("Reversed face without loops.");
    }

    // Create corner parametric points
    aParams[0] = OdGePoint2d(ivU.lowerBound(), ivV.lowerBound());
    aParams[1] = OdGePoint2d(ivU.upperBound(), ivV.lowerBound());
    aParams[2] = OdGePoint2d(ivU.upperBound(), ivV.upperBound());
    aParams[3] = OdGePoint2d(ivU.lowerBound(), ivV.upperBound());

    if (needPCurve(surfData.pSurf))
    {
      for (unsigned int i = 0; i < iBorders; i++)
      {
        aParamCurves[i] = new OdGeNurbCurve2d(OdGeLineSeg2d(aParams[i], aParams[(i + 1) % iBorders]));
        OdResult eStatus = fixParamCurve(surfData.pSurf, aIsoparamCurves[i], aParamCurves[i]);
        if (eStatus != eOk) {
          return checkRet(eStatus);
        }
      }
    }

    // Create new loop
    BrepBuilderInitialLoop loopData;
    for (unsigned int i = 0; i < iBorders; i++)
    {
      // Degenerate edge
      OdGe::EntityId iType;
      if (aIsoparamCurves[i]->isDegenerate(iType, 1.e-10/*, m_toleranceCoincide*/)) {
        continue;
      }

      // Create coedge
      BrepBuilderInitialCoedge& coedgeData = *loopData.coedges.append();
      // Reverse direction of two coedges
      coedgeData.direction = aDirs[i];
      coedgeData.curve = aParamCurves[i];

      // Create sample points array
      OdGePoint3dArray aSmplPts;
      aIsoparamCurves[i]->getSamplePoints(iPtsCnt, aSmplPts);

      // Calculate distance from first to last points.
      OdGeInterval iv;
      aIsoparamCurves[i]->getInterval(iv);
      double dIvLength = iv.length();

      std::multiset<EdgeCurveCompareData>::iterator start = m_edgesAdded.lower_bound(EdgeCurveCompareData(dIvLength - m_toleranceInterval));
      std::multiset<EdgeCurveCompareData>::iterator end = m_edgesAdded.upper_bound(EdgeCurveCompareData(dIvLength + m_toleranceInterval));

      if (start != end)
      {
        double dMinDist = std::numeric_limits<double>::max(), dCurDist;
        std::multiset<EdgeCurveCompareData>::iterator thebest;
        bool bFound = false;
        for (std::multiset<EdgeCurveCompareData>::iterator it = start; it != end; ++it)
        {
          if (arePointsEqual(aSmplPts, it->aSamplePts, dCurDist, m_toleranceInterval) && dCurDist < dMinDist)
          {
            dMinDist = dCurDist;
            thebest = it;
            bFound = true;
          }
        }
        if (bFound) {
          coedgeData.edgeIndex = thebest->idEdge;
          continue;
        }
      }

      // Create edge
      BrepBuilderInitialEdge& edgeData = *m_initialData.edges.append();
      edgeData.curve = aIsoparamCurves[i];

      // Fill edge visual
      edgeData.hasColor = surfData.hasColor;
      if (edgeData.hasColor)
        edgeData.color = surfData.color;

      // Here we can't get OdGsMarker. There is no edge to getGsMarker.

      coedgeData.edgeIndex = m_initialData.edges.size() - 1;
      m_edges[(OdUInt64)edgeData.curve.get()] = coedgeData.edgeIndex;
      m_edgesAdded.insert(EdgeCurveCompareData(edgeData.curve.get(), coedgeData.edgeIndex, aSmplPts, dIvLength));
    }

    surfData.loops.append(loopData);
  }
  else
  {
    return checkRet(eNotApplicable);
  }
  return checkRet(eOk);
}

bool OdBrepBuilderFillerHelper::needPCurve(const OdGeSurface* pSurf)
{
  OdGe::EntityId sType = pSurf->type();
  return !(sType == OdGe::kPlane         || sType == OdGe::kSphere    || sType == OdGe::kCylinder || sType == OdGe::kCone ||
           sType == OdGe::kEllipCylinder || sType == OdGe::kEllipCone || sType == OdGe::kTorus);
}

// Face
OdGeSurfacePtr OdBrepBuilderFillerHelper::checkExtSurface(const OdGeSurface* resSurf, const OdBrFace &face) const
{
  const OdGeExternalSurface* extSurf = static_cast<const OdGeExternalSurface*>(resSurf);
  OdGeSurface* pSurf = NULL;
  if (extSurf->isNativeSurface(pSurf))
    return pSurf;

  OdGeNurbSurface nurbFace;
  OdBrErrorStatus err = face.getSurfaceAsNurb(nurbFace);
  if (odbrOK != err) {
    return NULL;
  }
  return new OdGeNurbSurface(nurbFace);
}

OdGeSurfacePtr OdBrepBuilderFillerHelper::getFaceSurface(const OdBrFace& face) const
{
  OdGeSurfacePtr surf(face.getSurface());
  if (!surf)
  {
    OdGeNurbSurface nurbSurf;
    try
    {
      if (odbrOK == face.getSurfaceAsNurb(nurbSurf)) {
        return static_cast<OdGeSurface*>(nurbSurf.copy());
      }
    }
    catch (const OdError& err)
    {
      if (err.code() != eNotImplemented) {
        throw err;
      }
    }

    return NULL;
  }

  OdGe::EntityId entType = surf->type();
  if (OdGe::kExternalBoundedSurface == entType)
  {
    OdGeSurface* tResSurf = NULL;
    const OdGeExternalBoundedSurface* extSurf = static_cast<OdGeExternalBoundedSurface*>(surf.get());
    extSurf->getBaseSurface(tResSurf);
    OdGeTempSurfacePtr resSurf(tResSurf);
    if (!resSurf.isNull() && resSurf->type() != OdGe::kExternalSurface)
      surf = resSurf.detach();
    else if (!resSurf.isNull() && resSurf->type() == OdGe::kExternalSurface)
      surf = checkExtSurface(resSurf, face);
  }
  else if (OdGe::kExternalSurface == entType)
    surf = checkExtSurface(surf, face);

  return surf;
}

void OdBrepBuilderFillerHelper::fixEllipConeRRatio(OdGeSurface* pSurf) const
{
  if (!m_params.isMakeEllipMajorGreaterMinor()) {
    return;
  }

  double dStartAngle, dEndAngle;
  OdGe::EntityId sType = pSurf->type();
  if (sType == OdGe::kEllipCone)
  {
    OdGeEllipCone *pEllipCone = static_cast<OdGeEllipCone*>(pSurf);
    OdGeVector3d vecMajAxis = pEllipCone->majorAxis();
    OdGeVector3d vecMinAxis = pEllipCone->minorAxis();
    const double majorRadius = pEllipCone->majorRadius();
    const double minorRadius = pEllipCone->minorRadius();
    if (OdZero(majorRadius, m_toleranceInterval) || !vecMajAxis.isPerpendicularTo(vecMinAxis, OdGeContext::gTol)) {
      throw OdError(eNotImplementedYet);
    }
    double dRaduisRatio = minorRadius / majorRadius;
    if (OdGreater(dRaduisRatio, 1, m_toleranceDiffPoints))
    {
      double dSinAngle, dCosAngle;
      pEllipCone->getHalfAngle(dCosAngle, dSinAngle);
      //this code need for determine sign of sin
      if (!(pEllipCone->isNormalReversed() ^ pEllipCone->isOuterNormal())) {
        dCosAngle *= -1;
      }
      if (!(pEllipCone->baseCenter() - pEllipCone->axisOfSymmetry() * (majorRadius * dCosAngle / dSinAngle)).isEqualTo(pEllipCone->apex())) {
        dSinAngle *= -1;
      }
      pEllipCone->getAngles(dStartAngle, dEndAngle);
      dStartAngle -= OdaPI2;
      dEndAngle -= OdaPI2;
      OdGeInterval height;
      pEllipCone->getHeight(height);
      pEllipCone->set(dCosAngle, dSinAngle, pEllipCone->baseCenter(), minorRadius, majorRadius, pEllipCone->axisOfSymmetry(), vecMinAxis, height, dStartAngle, dEndAngle);
    }
  }
  else if (sType == OdGe::kEllipCylinder)
  {
    OdGeEllipCylinder *pEllipCylinder = static_cast<OdGeEllipCylinder*>(pSurf);
    OdGeVector3d vecMajAxis = pEllipCylinder->majorAxis();
    OdGeVector3d vecMinAxis = pEllipCylinder->minorAxis();
    const double majorRadius = pEllipCylinder->majorRadius();
    const double minorRadius = pEllipCylinder->minorRadius();
    if (OdZero(majorRadius, m_toleranceInterval) || !vecMajAxis.isPerpendicularTo(vecMinAxis, OdGeContext::gTol)) {
      throw OdError(eNotImplementedYet);
    }
    double dRaduisRatio = minorRadius / majorRadius;
    if (OdGreater(dRaduisRatio, 1, m_toleranceDiffPoints))
    {
      pEllipCylinder->getAngles(dStartAngle, dEndAngle);
      dStartAngle -= OdaPI2;
      dEndAngle -= OdaPI2;
      OdGeInterval height;
      pEllipCylinder->getHeight(height);
      pEllipCylinder->set(majorRadius, minorRadius, pEllipCylinder->origin(), pEllipCylinder->axisOfSymmetry(), vecMinAxis.negate(), height, dStartAngle, dEndAngle);
    }
  }
}

//Vertex
BrepBuilderInitialEdge::VertexIndex OdBrepBuilderFillerHelper::addVertex(const OdBrVertex& vertex)
{
  class OdBrVertexEx : public OdBrVertex
  {
  public:
    OdUInt64 id() const {
      return (OdUInt64)m_pImp;
    }
  };

  std::map<OdUInt64, OdUInt32>::const_iterator pIt = m_vertices.find(((OdBrVertexEx*)(&vertex))->id());
  if (pIt != m_vertices.end()) {
    return pIt->second;
  }
  BrepBuilderInitialVertex& initVertex = *m_initialData.vertices.append();
  initVertex.point = vertex.getPoint();
  BrepBuilderInitialEdge::VertexIndex vertexIndex = m_initialData.vertices.size() - 1;
  m_vertices[((const OdBrVertexEx&)(vertex)).id()] = vertexIndex;
  return vertexIndex;
}

// Loop
OdResult OdBrepBuilderFillerHelper::performLoopWithApex(const OdBrLoop& loop,
  OdGeCurve3dPtr& pCurve3d, OdGeCurve2dPtr& pCurve2d, OdBrVertex* vertex) const
{
  OdBrLoopVertexTraverser loopVertTrav;
  if (odbrOK != loopVertTrav.setLoop(loop)) {
    return checkRet(eBadApexLoop);
  }

  if (vertex)
    *vertex = loopVertTrav.getVertex();
  OdGePoint3d point = vertex ? vertex->getPoint() : loopVertTrav.getVertex().getPoint();

  if ((odbrOK != loopVertTrav.next()) || !loopVertTrav.done())
  {
    // unexpected: more then one point
    return checkRet(eBadApexLoop);
  }

  pCurve3d = new OdGeLineSeg3d(point, point);
  pCurve2d = NULL;

  return checkRet(eOk);
}

OdResult OdBrepBuilderFillerHelper::splitOuterLoops(BrepBuilderInitialSurface& surfData, BrepBuilderInitialSurfaceArray& arrSurfaces, int& nOuterLoops)
{
  OdResult eStatus = eOk;
  if (m_params.isSkipCheckLoopType()) {
    return checkRet(eStatus);
  }
  OdArray<std::pair<const OdGeCurve2d*, bool> > arrCoedges;
  OdArray<const OdGeCurve3d*> m_arrEdges;
  for (OdArray<BRepBuilderGeometryId>::size_type k = 0; k < surfData.loops.last().coedges.size(); k++)
  {
    OdGeCurve3dPtr pCurve = m_initialData.edges[surfData.loops.last().coedges[k].edgeIndex].curve;
    OdGeCurve2dPtr pParamCurve = surfData.loops.last().coedges[k].curve;
    arrCoedges.append(std::make_pair(pParamCurve, surfData.loops.last().coedges[k].direction == OdBrepBuilder::kReversed));
    m_arrEdges.append(pCurve);
  }

  OdGeLoopCtx loopCtx(surfData.pSurf.get(), arrCoedges, m_arrEdges, surfData.direction == OdBrepBuilder::kReversed);
  if (loopCtx.getLoopType(m_toleranceCoincide) == OdGeLoopCtx::kLOuter) {
    nOuterLoops++;
  }
  if (nOuterLoops > 1)
  {
    //create a new face and move loop into to it
    BrepBuilderInitialSurface surfDataNew;
    surfDataNew.copyFaceExceptLoops(surfData);
    surfDataNew.loops.append(surfData.loops.last());
    surfData.loops.removeLast();
    arrSurfaces.append(surfDataNew);
  }
  return checkRet(eStatus);
}

// Topology
OdResult OdBrepBuilderFillerHelper::performBrep(const OdBrBrep& brep)
{
  ODA_ASSERT_ONCE(brep.isValid());

  OdBrBrepComplexTraverser complxTrav;
  OdBrErrorStatus errStatus = complxTrav.setBrep(brep);
  if (odbrOK != errStatus) {
    return checkRet(eBrComplexMissed);
  }

  while (!complxTrav.done())
  {
    OdBrComplex complex = complxTrav.getComplex();

    OdResult res = performComplex(complex);
    if (eOk != res) {
      return checkRet(res);
    }

    if (odbrOK != complxTrav.next()) {
      return checkRet(eNullIterator);
    }
  }

  return checkRet(eOk);
}

OdResult OdBrepBuilderFillerHelper::performComplex(const OdBrComplex& complex)
{
  OdBrComplexShellTraverser complxShellTrav;
  OdBrErrorStatus errStatus = complxShellTrav.setComplex(complex);
  if (odbrUnsuitableTopology == errStatus) {
    return checkRet(eOk);
  }
  if (odbrOK != errStatus) {
    return checkRet(eBrComplexMissed);
  }

  BrepBuilderShellsArray shells;
  while (!complxShellTrav.done())
  {
    OdBrShell shell = complxShellTrav.getShell();

    OdResult res = performShell(shell, shells);
    if (eOk != res) {
      return checkRet(res);
    }

    if (odbrOK != complxShellTrav.next()) {
      return checkRet(eNullIterator);
    }
  }
  m_initialData.complexes.append(shells);

  return checkRet(eOk);
}

OdResult OdBrepBuilderFillerHelper::performShell(const OdBrShell& shell, BrepBuilderShellsArray& arrShells)
{
  OdBrShellFaceTraverser shellFaceTrav;
  OdBrErrorStatus errStatus = shellFaceTrav.setShell(shell);
  if (odbrUnsuitableTopology == errStatus) {
    return checkRet(eOk);
  }
  if (odbrOK != errStatus) {
    return checkRet(eBrShellMissed);
  }

  bool bCheckFaces = false;
  m_edgesAdded.clear();
  BrepBuilderInitialSurfaceArray arrSurfaces;
  while (!shellFaceTrav.done())
  {
    OdBrFace face = shellFaceTrav.getFace();
    unsigned int nFacesCreated = arrSurfaces.length();
    OdResult res = performFace(face, arrSurfaces);
    if (eOk != res) {
      return checkRet(res);
    }

    nFacesCreated = arrSurfaces.length() - nFacesCreated;
    if (nFacesCreated > 1)//additional face has been created
    {                     //this face(s) should be in another shell. So after processing all faces we should check it
      bCheckFaces = true;
    }

    if (odbrOK != shellFaceTrav.next()) {
      return checkRet(eNullIterator);
    }
  }
  if (bCheckFaces || !m_edgesAdded.empty())
  {
    //detect contiguous faces and move it to logical groups. Each of this group should be in separate shell
    OdArray< std::set<unsigned int> > gropedFaces;
    groupFaces(arrSurfaces, gropedFaces);
    for (unsigned int i = 0; i < gropedFaces.length(); i++)
    {
      BrepBuilderInitialSurfaceArray arrNewSurfaces;
      std::set<unsigned int> indexes = gropedFaces[i];
      for (std::set<unsigned int>::iterator it = indexes.begin(); it != indexes.end(); it++) {
        arrNewSurfaces.append(arrSurfaces[*it]);
      }
      arrShells.append(arrNewSurfaces);
    }
    return checkRet(eOk);
  }
  arrShells.append(arrSurfaces);

  return checkRet(eOk);
}

OdResult OdBrepBuilderFillerHelper::performFace(const OdBrFace& face, BrepBuilderInitialSurfaceArray &arrSurfaces)
{
  OdResult eStatus;
  BrepBuilderInitialSurface surfData;
  surfData.pSurf = getFaceSurface(face);
  if (surfData.pSurf.isNull())
  {
    // skip face without surface
    if (m_params.isSkipNullSurface()) {
      return checkRet(eOk);
    }
    return checkRet(eNullFaceSurface);
  }
  fixEllipConeRRatio(surfData.pSurf);

  // face direction
  surfData.direction = face.getOrientToSurface() ? OdBrepBuilder::kForward : OdBrepBuilder::kReversed;

  surfData.marker.first = odbrOK == face.getGsMarker(surfData.marker.second);

  // face visual
  eStatus = surfData.setupVisualInfo(face, m_pMaterialHelper);
  if (eOk != eStatus) {
    return checkRet(eStatus);
  }

  OdBrFaceLoopTraverser faceLoopTrav;
  OdBrErrorStatus err = faceLoopTrav.setFace(face);
  if (odbrUnsuitableTopology == err)
  {
    if (m_params.isGenerateExplicitLoops())
    {
      eStatus = addFaceExplicitLoop(surfData, face);
      if (eStatus != eOk) {
        ODA_FAIL_M_ONCE("Face without loops - unsupported case");
      }
    }
    // Face without loops (sphere, torus)
    arrSurfaces.append(surfData);
    return checkRet(eOk);
  }
  if (odbrOK != err) {
    return checkRet(eBrFaceMissed);
  }

  int nOuterLoops = 0;
  while (!faceLoopTrav.done())
  {
    OdBrLoop loop = faceLoopTrav.getLoop();
    eStatus = performLoop(loop, surfData);
    if (eOk != eStatus) {
      return checkRet(eStatus);
    }

    if (odbrOK != faceLoopTrav.next()) {
      return checkRet(eNullIterator);
    }
    eStatus = splitOuterLoops(surfData, arrSurfaces, nOuterLoops);
    if (eOk != eStatus) {
      return checkRet(eStatus);
    }
  }
  arrSurfaces.append(surfData);
  return checkRet(eOk);
}

OdResult OdBrepBuilderFillerHelper::performLoop(const OdBrLoop& loop,
  BrepBuilderInitialSurface& surfData)
{
  OdBrErrorStatus err;
  OdBrLoopEdgeTraverser loopEdgeTrav;
  err = loopEdgeTrav.setLoop(loop);

  // Loop with apex:
  if (odbrDegenerateTopology == err) // maybe there should be odbrUnsuitableTopology (see arx)
  {
    OdGeCurve3dPtr curve;
    OdGeCurve2dPtr paramCurve;
    OdBrVertex vertex;
    OdResult resStatus = performLoopWithApex(loop, curve, paramCurve, &vertex);
    if (eOk == resStatus)
    {
      BrepBuilderInitialEdge::VertexIndex vertexIndex = addVertex(vertex);

      m_initialData.edges.append(BrepBuilderInitialEdge(curve, vertexIndex, vertexIndex));
      surfData.loops.append(BrepBuilderInitialLoop(
        paramCurve,
        m_initialData.edges.size() - 1, // stub edge for correct index
        OdBrepBuilder::kForward
      ));

      return checkRet(eOk);
    }
    return checkRet(resStatus);
  }
  if (odbrOK != err) {
    return checkRet(eBrEmptyLoop);
  }

  // Regular loop:
  BrepBuilderInitialLoop loopData;

  class OdBrEdgeEx : public OdBrEdge
  {
  public:
    OdUInt64 id() const {
      return (OdUInt64)m_pImp;
    }
  };

  while (!loopEdgeTrav.done())
  {
    OdBrEdge edge = loopEdgeTrav.getEdge();

    BrepBuilderInitialCoedge& coedgeData = *loopData.coedges.append();

    OdGeCurve3dPtr curve(NULL);
    bool isNewEdge = true;

    std::map<OdUInt64, OdUInt32>::const_iterator pIt = m_edges.find(((OdBrEdgeEx*)(&edge))->id());
    if (pIt != m_edges.end())
    {
      isNewEdge = false;
      coedgeData.edgeIndex = pIt->second;
      curve = m_initialData.edges[coedgeData.edgeIndex].curve;
    }

    if (isNewEdge)
    {
      OdResult resStatus = getEdgeCurveFixed(edge, curve);
      if (eOk != resStatus) {
        return checkRet(resStatus);
      }

      OdBrVertex vertices[2];
      bool res[2];
      res[0] = m_params.isGenerateVertices() && edge.getVertex1(vertices[0]);
      res[1] = m_params.isGenerateVertices() && edge.getVertex2(vertices[1]);
      if (res[0] && res[1] && !edge.getOrientToCurve())
      {
        std::swap(vertices[0], vertices[1]);
      }
      BrepBuilderInitialEdge::VertexIndex vertexIndex[2];
      for (int idx = 0; idx < 2; ++idx)
      {
        vertexIndex[idx] = res[idx] ? addVertex(vertices[idx]) : OdBrepBuilder::kDefaultVertexId;
      }

      m_initialData.edges.append(BrepBuilderInitialEdge(curve, vertexIndex[0], vertexIndex[1]));
      BrepBuilderInitialEdge& edgeData = m_initialData.edges.last();
      coedgeData.edgeIndex = m_initialData.edges.size() - 1;
      m_edges[((OdBrEdgeEx*)(&edge))->id()] = coedgeData.edgeIndex;

      // edge visual
      OdResult eStatus = edgeData.setupVisualInfo(edge, m_pMaterialHelper);
      if (eOk != eStatus) {
        return checkRet(eStatus);
      }
      edgeData.marker.first = odbrOK == edge.getGsMarker(edgeData.marker.second);
    }

    if (!m_params.isSkipCoedge2dCurve())
    {
      coedgeData.curve = getParamCurve(loopEdgeTrav);
      // acis bb required 2d coedge curve for SPLINE surface(ruled, spun etc - are SPLINE surfaces)
      ODA_ASSERT_ONCE(!curve.isNull());
      ODA_ASSERT_ONCE(!surfData.pSurf.isNull());
      if (coedgeData.curve.isNull() && (needPCurve(surfData.pSurf)))
      {
        coedgeData.curve = restoreUvCurveAsNurb(curve, surfData.pSurf, m_toleranceRestore2dCurve);
        if (coedgeData.curve.isNull()) {
          return checkRet(eCreateFailed);
        }
      }

      OdResult eStatus = fixParamCurve(surfData.pSurf, curve, coedgeData.curve);
      //if surface is analytic-defined (cone, plane etc.) parametric curves may not required
      //so if we have such surface (any but not a spline) and 2d curve is "wrong" (wrong interval or direction)
      //this 2d curve may be detached and will not given to brep builder
      if (eOk != eStatus)
      {
        if (!needPCurve(surfData.pSurf)) {
          coedgeData.curve = OdGeCurve2dPtr();
        }
        else if (eCreateFailed == eStatus)
        {//already tried create parcur
          return checkRet(eStatus);
        }
        else
        {
          eStatus = createParamCurve(surfData.pSurf, curve, coedgeData.curve);
          if (eOk != eStatus)
          {
            return checkRet(eStatus);
          }
        }
      }
    }
    else if(m_params.destinationBrepType() == OdBrepBuilderFillerParams::kBrepAcisDgn || m_params.destinationBrepType() == OdBrepBuilderFillerParams::kBrepAcisDwg)
    {
      //we reject input parametric curves but if output data is a ACIS - parametric curves should used in some cases
      //so we reject input and create own
      ODA_ASSERT_ONCE(!curve.isNull());
      ODA_ASSERT_ONCE(!surfData.pSurf.isNull());
      if (needPCurve(surfData.pSurf))
      {
        OdResult eStatus = createParamCurve(surfData.pSurf, curve, coedgeData.curve);
        if (eOk != eStatus) {
          return checkRet(eStatus);
        }
      }
    }
    coedgeData.direction = (edge.getOrientToCurve() == loopEdgeTrav.getEdgeOrientToLoop())
      ? OdBrepBuilder::kForward : OdBrepBuilder::kReversed;

    if (odbrOK != loopEdgeTrav.next()) {
      return checkRet(eNullIterator);
    }
  }

  // skip loop without coedge
  if (!loopData.coedges.empty()) {
    surfData.loops.append(loopData);
  }

  return checkRet(eOk);
}

void OdBrepBuilderFillerHelper::groupFaces(BrepBuilderInitialSurfaceArray& arrFaces, OdArray< std::set<unsigned int> >& gropedFaces)
{
  std::set<unsigned int> currentGroup;
  currentGroup.insert(0);
  gropedFaces.append(currentGroup);
  for (unsigned int i = 0; i < arrFaces.length(); i++)
  {
    std::set<unsigned int> facesIndexes;
    findAdjacentFaces(arrFaces, i, facesIndexes);
    if (facesIndexes.empty()) {
      continue;
    }
    // if at least on face was found in currentGroup - all faces should belong to this group
    std::vector<unsigned int> commonFaces;
    bool bGroupFound = false;
    for (unsigned int j = 0; j < gropedFaces.length(); j++)
    {
      std::set_intersection(gropedFaces[j].begin(), gropedFaces[j].end(), facesIndexes.begin(), facesIndexes.end(), std::back_inserter(commonFaces));
      if (!commonFaces.empty())
      {
        //found common faces - so all faces in facesIndexes should belong to this group
        gropedFaces[j].insert(facesIndexes.begin(), facesIndexes.end());
        bGroupFound = true;
        break;
      }
    }
    if (!bGroupFound)
    {
      //this is new faces group
      gropedFaces.append(facesIndexes);
    }
  }
}

void OdBrepBuilderFillerHelper::findAdjacentFaces(BrepBuilderInitialSurfaceArray& arrFaces, unsigned int iCurrFace, std::set<unsigned int>& facesIndexes)
{
  BrepBuilderInitialSurface currFace = arrFaces[iCurrFace];
  //get edges for face at index iCurrFace
  OdArray<unsigned int> edgesIdx;
  for (unsigned int i = 0; i < currFace.loops.length(); i++)
  {
    BrepBuilderInitialCoedgeArray arrCurrLoopCoedges = currFace.loops[i].coedges;
    for (unsigned int j = 0; j < arrCurrLoopCoedges.length(); j++) {
      edgesIdx.append(arrCurrLoopCoedges[j].edgeIndex);
    }
  }
  if (edgesIdx.isEmpty()) {
    return;
  }
  //now we iterate over faces and get they edges. If indexes of this edges and edgesIdx are same - current face and face at iCurrFace are adjacent
  for (unsigned int i = 0; i < arrFaces.length(); i++)
  {
//     if (i == iCurrFace)
//     {
//       continue;
//     }
    currFace = arrFaces[i];
    for (unsigned int j = 0; j < currFace.loops.length(); j++)
    {
      BrepBuilderInitialCoedgeArray arrCurrLoopCoedges = currFace.loops[j].coedges;
      for (unsigned int k = 0; k < arrCurrLoopCoedges.length(); k++)
      {
        unsigned int index = 0;
        if (edgesIdx.find(arrCurrLoopCoedges[k].edgeIndex, index)) {
          facesIndexes.insert(i);
        }
      }
    }
  }
}


/*static*/ OdResult OdBrepBuilderFillerHelper::initFromImpl(OdBrepBuilderFiller& filler, OdBrepBuilder& builder, const BrepBuilderInitialData& data)
{
  OdResult status = eOk;

  OdArray<BRepBuilderGeometryId> verticesIds(data.vertices.size());
  for (BrepBuilderInitialEdge::VertexIndex i = 0; i < data.vertices.size(); ++i)
  {
    const BrepBuilderInitialVertex& vertexData = data.vertices[i];
    verticesIds.append(builder.addVertex(vertexData.point));
  }

  filler.m_edges.reserve(data.edges.size());
  OdArray<BRepBuilderGeometryId> edgeIds(data.edges.size());
  for (BrepBuilderInitialCoedge::EdgeIndex i = 0; i < data.edges.size(); ++i)
  {
    const BrepBuilderInitialEdge& edgeData = data.edges[i];
    filler.m_edges.append(edgeData.curve);
    BRepBuilderGeometryId vertexIndex[2] = { OdBrepBuilder::kDefaultVertexId, OdBrepBuilder::kDefaultVertexId };
    for (int idx = 0; idx < 2; ++idx)
    {
      if (edgeData.vertexIndex[idx] != BrepBuilderInitialEdge::kDefaultVertexId)
      {
        vertexIndex[idx] = verticesIds[edgeData.vertexIndex[idx]];
      }
    }
    edgeIds.append(builder.addEdge(edgeData.curve, vertexIndex[0], vertexIndex[1]));
    if (edgeData.marker.first) {
      ODA_ASSERT_ONCE(edgeData.marker.second >= INT_MIN && edgeData.marker.second <= INT_MAX);
      builder.setTag(edgeIds.last(), (OdUInt32)edgeData.marker.second);
    }

    if (edgeData.hasColor)
    {
      status = builder.setEdgeColor(edgeIds.last(), edgeData.color);
      if (eOk != status) {
        return checkRet(status);
      }
    }
  }

  const bool addComplexShell = !filler.m_params.isIgnoreComplexShell();
  for (BrepBuilderComplexArray::const_iterator complexIt = data.complexes.begin(); complexIt != data.complexes.end(); ++complexIt)
  {
    BRepBuilderGeometryId currentComplexId = 0;
    if (addComplexShell) {
      currentComplexId = builder.addComplex();
    }

    for (BrepBuilderShellsArray::const_iterator shellIt = complexIt->begin(); shellIt != complexIt->end(); ++shellIt)
    {
      BRepBuilderGeometryId currentShellId(OdBrepBuilder::kDefaultShellId);
      if (addComplexShell) {
        currentShellId = builder.addShell(currentComplexId);
        ODA_ASSERT_ONCE(currentShellId != OdBrepBuilder::kDefaultShellId);
      }

      for (BrepBuilderInitialSurfaceArray::const_iterator surfIt = shellIt->begin(); surfIt != shellIt->end(); ++surfIt)
      {
        filler.m_surfaces.append(surfIt->pSurf);
        BRepBuilderGeometryId currentFaceId = builder.addFace(surfIt->pSurf, surfIt->direction, currentShellId);
        if (surfIt->marker.first) {
          ODA_ASSERT_ONCE(surfIt->marker.second >= INT_MIN && surfIt->marker.second <= INT_MAX);
          builder.setTag(currentFaceId, (OdUInt32)surfIt->marker.second);
        }
        if (surfIt->material) {
          builder.setFacesMaterial(currentFaceId, *surfIt->material);
        }
        if (surfIt->hasMaterialMapping)
        {
          status = builder.setFaceMaterialMapping(currentFaceId, surfIt->materialMapper);
          if (eOk != status) {
            return checkRet(status);
          }
        }
        if (surfIt->hasColor)
        {
          status = builder.setFaceColor(currentFaceId, surfIt->color);
          if (eOk != status) {
            return checkRet(status);
          }
        }

        const BrepBuilderInitialLoopArray& loops = surfIt->loops;
        for (BrepBuilderInitialLoopArray::const_iterator loopIt = loops.begin(); loopIt != loops.end(); ++loopIt)
        {
          BRepBuilderGeometryId currentLoopId = builder.addLoop(currentFaceId);

          const BrepBuilderInitialCoedgeArray& coedges = loopIt->coedges;
          for (BrepBuilderInitialCoedgeArray::const_iterator coedgeIt = coedges.begin(); coedgeIt != coedges.end(); ++coedgeIt)
          {
            filler.m_coedges.append(coedgeIt->curve);
            builder.addCoedge(currentLoopId, edgeIds[coedgeIt->edgeIndex], coedgeIt->direction, coedgeIt->curve);
          }

          builder.finishLoop(currentLoopId);
        }

        builder.finishFace(currentFaceId);
      }

      if (addComplexShell) {
        builder.finishShell(currentShellId);
      }
    }
    if (addComplexShell) {
      builder.finishComplex(currentComplexId);
    }
  }

  return checkRet(status);
}


/*static*/  OdResult OdBrepBuilderFillerHelper::getDataFrom(
  BrepBuilderInitialData& data,
  const OdBrepBuilderFiller& filler,
  const OdBrBrep& brep,
  OdIMaterialAndColorHelper* materialHelper)
{
  if (!brep.isValid()) {
    return OdBrepBuilderFillerHelper::checkRet(eNotInitializedYet);
  }

  OdResult status = eOk;
  try {
    if (materialHelper) {
      status = materialHelper->init(filler.params());
      if (eOk != status) {
        return OdBrepBuilderFillerHelper::checkRet(status);
      }
    }

    OdBrepBuilderFillerHelper brepBuilderFillerHelper(data, filler, materialHelper);
    status = brepBuilderFillerHelper.performBrep(brep);
  }
  catch (const OdError& err) {
    return OdBrepBuilderFillerHelper::checkRet(err.code());
  }
  catch (...) {
    return OdBrepBuilderFillerHelper::checkRet(eGeneralModelingFailure);
  }
  return OdBrepBuilderFillerHelper::checkRet(status);
}

//

OdResult OdBrepBuilderFiller::initFrom(OdBrepBuilder& builder, const BrepBuilderInitialData& data)
{
  if (!builder.isValid()) {
    return OdBrepBuilderFillerHelper::checkRet(eNotInitializedYet);
  }
  // TODO check can add geom (not implemented now)
  //if (!builder.canAddGeometry()) {
  //  return OdBrepBuilderFillerHelper::checkRet(eInvalidInput);
  //}

  clearTempArrays();

  OdResult status = eOk;
  try {
    status = OdBrepBuilderFillerHelper::initFromImpl(*this, builder, data);
  }
  catch (const OdError& err) {
    return OdBrepBuilderFillerHelper::checkRet(err.code());
  }
  catch (...) {
    return OdBrepBuilderFillerHelper::checkRet(eGeneralModelingFailure);
  }

  return OdBrepBuilderFillerHelper::checkRet(status);
}

OdResult OdBrepBuilderFiller::initFrom(OdBrepBuilder& builder, const OdBrBrep& brep,
  OdIMaterialAndColorHelper* materialHelper)
{
  BrepBuilderInitialData initData;
  OdResult status = OdBrepBuilderFillerHelper::getDataFrom(initData, *this, brep, materialHelper);
  if (eOk != status) {
    return OdBrepBuilderFillerHelper::checkRet(status);
  }

  return OdBrepBuilderFillerHelper::checkRet(initFrom(builder, initData));
}


OdResult OdBrepBuilderFiller::initFromNURBSingleFace(OdBrepBuilder& builder, const OdBrBrep& brep)
{
  clearTempArrays();

  try
  {
    OdBrErrorStatus err = odbrOK;
    OdBrBrepFaceTraverser bft;
    BrepBuilderInitialData ignore;
    OdBrepBuilderFillerHelper fillerHelper(ignore, *this);
    if (bft.setBrep(brep) != odbrOK)
    {
      return eBrBrepMissed;
    }
    while (!bft.done() && (err == odbrOK))
    {
      OdBrFaceLoopTraverser faLoTrav;
      OdBrFace face = bft.getFace();
      OdGeNurbSurface *pNurbSurf = new OdGeNurbSurface;
      face.getSurfaceAsNurb(*pNurbSurf);
      m_surfaces.append(pNurbSurf);

      //add nurbs surface to BB
      bool bOrientToSurface = face.getOrientToSurface();
      BRepBuilderGeometryId complexId = builder.addComplex();
      BRepBuilderGeometryId shellId = builder.addShell(complexId);
      BRepBuilderGeometryId faceId = builder.addFace(pNurbSurf, bOrientToSurface ? OdBrepBuilder::kForward : OdBrepBuilder::kReversed, shellId);
      //iterate over trimming-loop and create data for BB
      for (faLoTrav.setFace(face); !faLoTrav.done(); faLoTrav.next())
      {
        BRepBuilderGeometryId LoopId = builder.addLoop(faceId);
        OdBrLoopEdgeTraverser loEdTrav;
        OdBrLoop loop = faLoTrav.getLoop();
        err = loEdTrav.setLoop(loop);
        if (odbrDegenerateTopology == err)
        {
          OdGeCurve3dPtr pCurve3d;
          OdGeCurve2dPtr pCurve2d;
          OdResult resStatus = fillerHelper.performLoopWithApex(loop, pCurve3d, pCurve2d);
          if (eOk == resStatus)
          {
            m_edges.append(pCurve3d);
            m_coedges.append(pCurve2d);
            BRepBuilderGeometryId edgeId = builder.addEdge(pCurve3d);
            builder.addCoedge(LoopId, edgeId, OdBrepBuilder::kForward, pCurve2d);
            err = odbrOK;
            continue;
          }
          return resStatus;
        }

        OdArray<OdBrEdge> arrBrepEdges;
        OdArray<BRepBuilderGeometryId> arrBrepEdgesID;
        for (; !loEdTrav.done(); loEdTrav.next())
        {
          OdBrEdge edge = loEdTrav.getEdge();
          unsigned int iFindIndex = 0;
          bool bFindEdge = false;
          for (unsigned int k = 0; k < arrBrepEdges.size(); k++)
          {
            if (edge.isEqualTo(&arrBrepEdges[k]))
            {
              //In case of nurbs cone we have one edge and 2 coedges, but brep returns 2 edges
              iFindIndex = k;
              bFindEdge = true;
              break;
            }
          }

          bool bEdgeForward = edge.getOrientToCurve();
          bool bCoedgeForward = loEdTrav.getEdgeOrientToLoop();

          OdGeCurve3dPtr pCurve3d;
          OdResult resStatus = fillerHelper.getEdgeCurveFixed(edge, pCurve3d);
          if (eOk != resStatus)
          {
            return resStatus;
          }
          m_edges.append(pCurve3d);
          OdGeCurve2dPtr pCurve2d = fillerHelper.getParamCurve(loEdTrav);

          OdGeSurfacePtr surface = m_surfaces.first();
          resStatus = fillerHelper.fixParamCurve(surface, pCurve3d, pCurve2d);
          //if surface is analytic-defined (cone, plane etc.) parametric curves may not required
          //so if we have such surface (any but not a spline) and 2d curve is "wrong" (wrong interval or direction)
          //this 2d curve may be detached and will not given to brep builder
          if (eOk != resStatus)
          {
            if (OdBrepBuilderFillerHelper::needPCurve(surface))
            {
              return resStatus;
            }
            else
            {
              pCurve2d = OdGeCurve2dPtr();
            }
          }
          m_coedges.append(pCurve2d);
          BRepBuilderGeometryId edgeId;
          if (!bFindEdge)
          {
            edgeId = builder.addEdge(pCurve3d);
            builder.addCoedge(LoopId, edgeId, bCoedgeForward ? OdBrepBuilder::kForward : OdBrepBuilder::kReversed, pCurve2d);
            arrBrepEdgesID.append(edgeId);
            arrBrepEdges.append(edge);
          }
          else
          {
            builder.addCoedge(LoopId, arrBrepEdgesID.at(iFindIndex), bCoedgeForward ? OdBrepBuilder::kForward : OdBrepBuilder::kReversed, pCurve2d);
          }
        }
      }
      builder.finishFace(faceId);
      builder.finishShell(shellId);
      builder.finishComplex(complexId);
      break;//Only one face
    }
  }
  catch (const OdError& err)
  {
    throw err;
  }
  catch (...)
  {
    return eInvalidInput;
  }
  return eOk;
}

void OdBrepBuilderFiller::clearTempArrays()
{
  m_edges.clear();
  m_coedges.clear();
  m_surfaces.clear();
}

// Member methods of BrepBuilderInitialData.h classes
OdResult BrepBuilderInitialSurface::setupVisualInfo(const OdBrFace& face, OdIMaterialAndColorHelper* pMaterialHelper)
{
  if (!pMaterialHelper) return eOk;
  return pMaterialHelper->getFaceVisualInfo(face, material, materialMapper, hasMaterialMapping, color, hasColor);
}

OdResult BrepBuilderInitialEdge::setupVisualInfo(const OdBrEdge& edge, OdIMaterialAndColorHelper* pMaterialHelper)
{
  if (!pMaterialHelper) return eOk;
  return pMaterialHelper->getEdgeVisualInfo(edge, color, hasColor);
}
