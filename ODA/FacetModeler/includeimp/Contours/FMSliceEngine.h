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
#include "FMProfile3D.h"
#include "FMGeometry.h"
#include "FMGeometryDebug.h"
#include "Modeler/FMMdlBody.h"
#include "Modeler/FMMdlFace.h"
#include "Modeler/FMMdlEdge.h"
#include "Modeler/FMMdlIterators.h"
#include "Contours/FMEdgeGraph.h"
#include "Contours/FMEdgeGraph.h"
#include "Contours/FMSliceContourBuilder.h"
#include "Modeler/FMMdlSlicerBaseImpl.h"

#include "Ge/GePlane.h"
#include "Ge/GeLineSeg3d.h"
#include <UInt32Array.h>
#include "Si/SiSpatialIndex.h"
#include "Si/SiShapePlane.h"
#include "Si/SiShapeBoundPlane.h"
#include "Si/SiShapeRay.h"
#include "Ge/GeLine3d.h"
#include "FMDebugDraw.h"
//#include "../sampleapplications/TADebugTx/Debug.h"

namespace FacetModeler
{

struct SpatialFaceData : public OdSiEntity {
  Face*                 m_pFace;
  TagType               m_savedTag;
  mutable OdGeExtents3d m_ext3d;
  mutable OdGeExtents2d m_ext2d;
  const Face* face() const { return m_pFace; }

  void detach() {
    m_pFace->setTag( m_savedTag );
    m_pFace = 0;
  }

  inline void attach( Face* pFace ) {
    m_pFace = pFace;
    m_savedTag = pFace->tag();
    pFace->setTag( (TagType)this );
    extents( pFace, m_ext3d );
    extents( pFace, m_ext2d );
  }

  static void extents( const Face* pFace, OdGeExtents3d& extents ) {
    Edge* pEdge = pFace->edge();
    Edge* pEnd = pEdge;
    const OdGePoint3d& pt = pEdge->vertex()->point();
    extents.set( pt, pt );
    do {
      pEdge = pEdge->next();
      extents.addPoint( pEdge->vertex()->point() );
    }
    while( pEdge != pEnd );
  }

  static void extents(const Face* pFace, OdGeExtents2d& extents) {

    OdGePlane plane(pFace->plane());

    Edge* pEdge = pFace->edge();
    Edge* pEnd = pEdge;
    const OdGePoint2d& pt = plane.paramOf(pEdge->vertex()->point());
    extents.set(pt, pt);
    do {
      pEdge = pEdge->next();
      extents.addPoint(plane.paramOf(pEdge->vertex()->point()));
    } while (pEdge != pEnd);
  }

  bool extents(OdGeExtents3d& extents) const {
    extents = m_ext3d;
    return true;
  }

  bool extents(OdGeExtents2d& extents) const {
    extents = m_ext2d;
    return true;
  }

  bool contains( const OdGeExtents3d& , bool , const OdGeTol& ) const {
    return false;
  }
};


template<class TSpatFData = SpatialFaceData>
class FaceSpatialIndex {
protected:
  OdSiSpatialIndexPtr   m_pFacesIndex;
  OdArray<TSpatFData>   m_spatialDataArray;
private:
  FaceSpatialIndex& operator = (const FaceSpatialIndex& ) { return *this; }
public:
  ~FaceSpatialIndex() {
    destroy( true );
  }
  OdSiSpatialIndex& build( const Body* pBody, double eps ) {
    destroy();
    OdUInt32 n = pBody->faceCount();
    if ( n ) {
      m_pFacesIndex = OdSiSpatialIndex::createObject( false, 0, 30, 20, eps );
      m_spatialDataArray.resize( n );
      TSpatFData* pSFD = m_spatialDataArray.asArrayPtr();
      FaceIterator iter( pBody );
      while( !iter.done() ) {
        pSFD->attach( iter.get() );
        m_pFacesIndex->insert( pSFD );
        iter.next();
        ++pSFD;
      }
    }
    return *m_pFacesIndex.get();
  }
  void destroy( bool bRestoreFaceTags = true ) {
    m_pFacesIndex.release();
    if( bRestoreFaceTags ) {
      while( m_spatialDataArray.size() ) {
        m_spatialDataArray.last().detach();
        m_spatialDataArray.removeLast();
      }
    }
    else {
      m_spatialDataArray.clear();
    }
  }

  OdSiSpatialIndex& index() {
    ODA_ASSERT( !m_pFacesIndex.isNull() ); 
    return *m_pFacesIndex; 
  }

  const OdSiSpatialIndex& index() const { 
    ODA_ASSERT( !m_pFacesIndex.isNull() );
    return *m_pFacesIndex;
  }

  OdArray<TSpatFData>& faceDataArray() { return m_spatialDataArray; }
  OdUInt32 size() const { 
    if(m_pFacesIndex.isNull())
      return 0;
    return m_spatialDataArray.size(); 
  }
};

//////////////////////////////////////////////////////////////////////////
template<class T>
inline static T mymax( T v1, T v2 ) {
  return v1 > v2 ? v1 : v2;
}

typedef OdArray<Vertex*, OdMemoryAllocator<Vertex*> > VertexIndex;
typedef OdArray<Face*, OdMemoryAllocator<Face*> > FacePtrArray;
typedef OdArray<const Face*, OdMemoryAllocator<const Face*> > FaceConstPtrArray;



//typedef OdArray<const Edge*, OdMemoryAllocator<const Edge*> > EdgeConstPtrArray;

enum FaceState {
  kInitialFaceState,
  kFaceToClone,
  kFaceToRemove,
  kProcessed
};

struct FaceData : SpatialFaceData, OdSiShape
{
  bool contains( const OdGeExtents3d& , bool , const OdGeTol& ) const 
  {
    return false;
  }

  bool intersects( const OdGeExtents3d& extents, bool planar, const OdGeTol& tol ) const 
  {
    return !extents.isDisjoint( m_ext3d, tol );
  }

  FaceData() : m_state(kInitialFaceState) {}
  OdUInt32 m_state;
};
//////////////////////////////////////////////////////////////////////////


template<class TSpatialFaceData = SpatialFaceData>
class Slicer : public SlicerBaseImpl,
               protected OdSiVisitor
{
  void visit( OdSiEntity* pEntity, bool  ) {
    TSpatialFaceData* pFaceData = static_cast<TSpatialFaceData*>( pEntity );

#ifdef _DEBUG
    //IDebugAssist* pAssist = CreateDebugAssistant();
    //pAssist->DebugDrawFace( pFaceData->face(), 0 );
#endif

    if (mode)
    {
      //collect_segments(pFaceData->face());
      collect_face(pFaceData->face());
    }
    else
      checkFaceByRay(pFaceData->face());
  }
public:
  void initSlicerAndFaceIndex( const Body* pBody, FaceSpatialIndex<TSpatialFaceData>& faceSpatialIndex, double eps = OdGeTol().equalVector() ) {
    reserveBuffers( pBody->faceCount() );
    OdGeTol sitol = faceSpatialIndex.build( pBody, eps ).tolerance();
    setTolerance( OdGeTol( sitol.equalPoint() / sitol.equalVector() * eps, eps ) );
  }


  void slice( const OdGePlane& cutPlane,
              const FaceSpatialIndex<TSpatialFaceData>& faceSpatial,
              Profile2D* pResultIncludingBoundary,
              bool intOrDif = false,
              Profile2D* pResultExcludingCoincidingBoundary = 0,
              Profile2D* pResultExcludingOppositeBoundary = 0,
              Profile2D* pResultExcludingBoundary = 0,
              FaceConstPtrArray* pCoincidingFaces = 0,
              FaceConstPtrArray* pOppositeFaces = 0,
              bool bUseReverseEdgeAtMerge = false,
              OdArray<const Edge*>* sourceEdges = 0,
              const OdGeExtents3d* pCutPlaneBounds = 0,
              const OdGeExtents2d* pCutFaceBox = 0)
  {
    EdgeGraph::Clear();
    setTolerance(OdGeTol(faceSpatial.index().tolerance().equalPoint(), 1e-5));
    set_cut_plane( cutPlane );
    ReserveBuffers( faceSpatial.size() );
    m_pCurSegMerger = &m_segmentsMerger;

    /*
    if (!pCutPlaneBounds || !pCutFaceBox)
      faceSpatial.index().query( OdSiShapePlane( cutPlane ), *this );
    else
    {
      faceSpatial.index().query(OdSiShapeBoundPlane(cutPlane, *pCutPlaneBounds), *this);
    }
    */

    OdGeExtents2d incCutFaceBox;
    OdGeExtents2d* pIncCutFaceBox = NULL;

    if (!pCutPlaneBounds || !pCutFaceBox)
      faceSpatial.index().query(OdSiShapePlane(cutPlane), *this);
    else
    {
      // increasing the 2d and 3d boxes of face
      // to process the cases when slice edge lies
      // on 2d box side
      ODA_ASSERT(pCutPlaneBounds && pCutFaceBox);
      incCutFaceBox = *pCutFaceBox;
      pIncCutFaceBox = &incCutFaceBox;

      OdGePoint2d minPt = incCutFaceBox.minPoint();
      OdGePoint2d maxPt = incCutFaceBox.maxPoint();

      double origLenX = fabs(maxPt.x - minPt.x);
      double origLenY = fabs(maxPt.y - minPt.y);

      OdGeVector2d diag = incCutFaceBox.diagonal();
      double diag_len = diag.length();
      double step = 0.05;
      int step_cnt = 0;
      for (;;)
      {
        OdGePoint2d newMaxPt = maxPt + diag * step;
        OdGePoint2d newMinPt = minPt - diag * step;

        double newLenX = fabs(newMaxPt.x - newMinPt.x);
        double newLenY = fabs(newMaxPt.y - newMinPt.y);

        if ((fabs(newLenX - origLenX) > GetAddNodeTol() &&
          fabs(newLenY - origLenY) > GetAddNodeTol()) ||
          step_cnt == 9)
        {
          incCutFaceBox.set(newMinPt, newMaxPt);
          break;
        }

        step += step;
        step_cnt++;
      }

      OdGePoint2dArray box2dPts;
      box2dPts.resize(4);
      box2dPts[0] = incCutFaceBox.minPoint();
      box2dPts[2] = incCutFaceBox.maxPoint();
      box2dPts[1] = OdGePoint2d(box2dPts[2].x, box2dPts[0].y);
      box2dPts[3] = OdGePoint2d(box2dPts[0].x, box2dPts[2].y);

      OdGeExtents3d cutPlaneBounds = *pCutPlaneBounds;
      for (int i = 0; i != 4; i++)
      {
        OdGePoint3d pt = cut_plane().evalPoint(box2dPts[i]);
        cutPlaneBounds.addPoint(pt);
      }
      faceSpatial.index().query(OdSiShapeBoundPlane(cutPlane, cutPlaneBounds), *this);
    }

    collect_all_segments();

    bool checkFaceByRay = false;

    build_results( pResultIncludingBoundary, 
      pResultExcludingCoincidingBoundary,
      pResultExcludingOppositeBoundary,
      pResultExcludingBoundary,
      pCoincidingFaces, pOppositeFaces, bUseReverseEdgeAtMerge, checkFaceByRay, sourceEdges, pIncCutFaceBox, pCutFaceBox);

    // checking face is inside or outside of body in case
    // graph is empty
    if (checkFaceByRay && pCutFaceBox)
    {
      mode = false;
      OdGePoint3d rayStart = cut_plane().pointOnPlane();
      OdGeVector3d rayDir = cut_plane().normal();
      OdGeRay3d ray(rayStart, rayDir);
      m_pRay = &ray;
      faceSpatial.index().query(OdSiShapeRay(ray), *this);

      bool inside = false;
      if (m_rayFaceInters.size() % 2 == 1)
      {
        double dist = (rayStart - m_rayFaceInters.first().m_intPt).length();
        if (dist > m_tol.equalPoint())
          inside = true;
      }

      if (!inside && intOrDif)
      {
        if (!m_coinciding.isEmpty())
        {
          inside = true;
        }
      }

      if (inside)
      {
        AddScaledBoxContour(*pCutFaceBox);
      }

      m_rayFaceInters.clear();
      mode = true;

      build_profiles(pResultIncludingBoundary,
        pResultExcludingCoincidingBoundary,
        pResultExcludingOppositeBoundary,
        pResultExcludingBoundary,
        pCoincidingFaces, pOppositeFaces, sourceEdges, pCutFaceBox);
    }

    // algo should not produce duplicates, but it does
    if(pResultExcludingOppositeBoundary != NULL)
    {
      pResultExcludingOppositeBoundary->deleteCoincident(1.e-10);
    }
  }
};

}
