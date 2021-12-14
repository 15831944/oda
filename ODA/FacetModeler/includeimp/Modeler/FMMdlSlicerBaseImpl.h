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

#ifndef AECSLICERBASEIMPL_H_INCLUDED
#define AECSLICERBASEIMPL_H_INCLUDED

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

#include "Ge/GePlane.h"
#include "Ge/GeLineSeg3d.h"
#include <UInt32Array.h>
#include "Si/SiSpatialIndex.h"
#include "Si/SiShapePlane.h"
#include "Ge/GeLine3d.h"
#include "FMDebugDraw.h"
//#include "../sampleapplications/TADebugTx/Debug.h"

template<typename _Tv>
class cycled_list
{
  template<typename _Td>
  struct _Node
  {
    _Node* pNext;
    _Node* pPrev;
    _Td data;

    _Node()
    {
      pNext = NULL;
      pPrev = NULL;
    }
  };

  typedef _Node<_Tv> Node;

  Node* m_pFirstNode;
  int m_iNodeCount;

public:
  class iterator
  {
    friend class cycled_list<_Tv>;

    Node* m_pCurrentNode;

    iterator(Node* pNode)
    {
      m_pCurrentNode = pNode;
    }

  public:
    iterator(const iterator& it)
    {
      m_pCurrentNode = it.m_pCurrentNode;
    }

    void move_forward()
    {
      m_pCurrentNode = m_pCurrentNode->pNext;
    }

    void move_backward()
    {
      m_pCurrentNode = m_pCurrentNode->pPrev;
    }

    bool operator == (const iterator& it) const
    {
      return m_pCurrentNode == it.m_pCurrentNode;
    }

    bool operator != (const iterator& it) const
    {
      return m_pCurrentNode != it.m_pCurrentNode;
    }

    const _Tv& data() const
    {
      return m_pCurrentNode->data;
    }

    _Tv& data()
    {
      return m_pCurrentNode->data;
    }

    const _Tv& next_node_data()
    {
      return m_pCurrentNode->pNext->data;
    }

    const _Tv& prev_node_data()
    {
      return m_pCurrentNode->pPrev->data;
    }

    _Tv get()
    {
      return m_pCurrentNode->data;
    }

    iterator next() const
    {
      return iterator(m_pCurrentNode->pNext);
    }
  };

public:
  cycled_list()
  {
    m_iNodeCount = 0;
    m_pFirstNode = NULL;
  }

  ~cycled_list()
  {
    clear();
  }

  bool empty() const
  {
    return m_iNodeCount == 0;
  }

  void clear()
  {
    m_iNodeCount = 0;
    if(m_pFirstNode == NULL)
      return;

    Node* pDeleteNode = m_pFirstNode->pNext;
    while(pDeleteNode != m_pFirstNode)
    {
      pDeleteNode = pDeleteNode->pNext;
      delete pDeleteNode->pPrev;
    }

    delete m_pFirstNode;
    m_pFirstNode = NULL;
  }

  int size() const
  {
    return m_iNodeCount;
  }

  void push_back(const _Tv& value)
  {
    m_iNodeCount++;

    Node* pNewNode = new Node();
    pNewNode->data = value;

    if(m_pFirstNode == NULL)
    {
      m_pFirstNode = pNewNode;
      pNewNode->pPrev = m_pFirstNode;
      pNewNode->pNext = m_pFirstNode;
      return;
    }

    Node* pLastNode = m_pFirstNode->pPrev;

    pLastNode->pNext = pNewNode;
    m_pFirstNode->pPrev = pNewNode;

    pNewNode->pPrev = pLastNode;
    pNewNode->pNext = m_pFirstNode;
  }

  void delete_range(const iterator& beg, const iterator& end)
  {
    if(beg == end)
      throw OdError(OD_T("delete_range() parameters equal"));

    Node* pBeginNode = beg.m_pCurrentNode;
    Node* pEndNode = end.m_pCurrentNode;

    if(pBeginNode->pNext == pEndNode)
    {
      return; // no inner nodes
    }

    Node* pCurrentNode = pEndNode->pPrev;
    do 
    {
      m_iNodeCount--;
      if(pCurrentNode == m_pFirstNode)
        m_pFirstNode = pBeginNode;

      pCurrentNode = pCurrentNode->pPrev;
      delete pCurrentNode->pNext;
    } 
    while (pCurrentNode != pBeginNode);

    pEndNode->pPrev = pBeginNode;
    pBeginNode->pNext = pEndNode;
  }

  iterator begin()
  {
    if(empty())
      throw OdError(OD_T("begin() cannot create iterator for empty cycled list"));

    return iterator(m_pFirstNode);
  }
};


class OdGeRay3d;

namespace FacetModeler
{

  class SlicerBaseImpl;

  struct GraphEdgeData
  {
    GraphEdgeData()
    {
      pEdge = NULL;
    };

    GraphEdgeData(const OdGePoint3d& _st, const OdGePoint3d& _end, const Edge* _pEdge) :
      st(_st),
      end(_end),
      pEdge(_pEdge)
    {};

    OdGePoint3d st, end;
    const Edge* pEdge;
  };

  class SegmentMerger
  {
    struct SegmentPoint
    {
      OdGePoint3d pt;
      double mergeLineParam;
      OdUInt32 ownerSegmentId;

      SegmentPoint(): mergeLineParam(0.0), ownerSegmentId(0)
      {}

      SegmentPoint(const OdGePoint3d& point, OdUInt32 segmentId) : mergeLineParam(0.0)
      {
        pt = point;
        ownerSegmentId = segmentId;
      }

      inline bool operator < ( const SegmentPoint& segPoint ) const 
      {
        return mergeLineParam < segPoint.mergeLineParam;
      }
    };
    typedef OdArray<SegmentPoint, OdMemoryAllocator<SegmentPoint> > SegmentPointsArray;

    struct SegmentPointLess 
    {
      inline bool operator() ( const SegmentPoint& pt1, const SegmentPoint& pt2 ) const 
      {
        return pt1.mergeLineParam < pt2.mergeLineParam;
      }
    };

  private:
    SegmentPointsArray m_segmentPoints;
    OdGePoint3d m_ptPreviousPoint;
    OdUInt32Array m_inSegments; 

    OdUInt32 m_segmentsIdBase;
    OdArray<const Edge*> m_segmentTags; 

  public:
    SegmentMerger()
    {
      m_segmentPoints.reserve(1024);
      m_segmentsIdBase = 0;
    }

    void PushSegment(const OdGePoint3d& ptStart, const OdGePoint3d& ptEnd, const Edge* tag = 0)
    {
      OdUInt32 segmentId = MakeSegmentId();
      m_segmentTags.push_back(tag);
      m_segmentPoints.push_back(SegmentPoint(ptStart, segmentId));
      m_segmentPoints.push_back(SegmentPoint(ptEnd, segmentId));
    }


    void MergeSegmentsAndCollectGraphEdges(const OdGeLine3d& mergeLine, SlicerBaseImpl* pGraph, const OdGeTol& tol, OdArray<GraphEdgeData>& face_edges)
    {
      if (m_segmentPoints.empty())
        return;

#ifdef _DEBUG
      CheckIsAllSegmentsOnMergeLine(mergeLine, tol);
#endif

      CalculatePointsParamAlongMergeLine(mergeLine, tol);
      //SortSegmentPointsAlongMergeLine(); // !!!!!!
      MergeSegments(face_edges, tol);
    }


    void MergeSegmentsAndAddToEdgeGraph(const OdGeLine3d& mergeLine, SlicerBaseImpl* pGraph, const OdGeTol& tol)
    {
      if(m_segmentPoints.empty())
        return;

  #ifdef _DEBUG
      CheckIsAllSegmentsOnMergeLine(mergeLine, tol);
  #endif

      CalculatePointsParamAlongMergeLine(mergeLine, tol);
      //SortSegmentPointsAlongMergeLine(); // !!!!!!
      MergeSegments(pGraph, tol);
    }

    void Clear()
    {
      m_segmentPoints.clear();
      m_segmentsIdBase = 0;
      m_inSegments.clear();
      m_segmentTags.clear();
    }

  private:
    OdUInt32 MakeSegmentId()
    {
      return (m_segmentsIdBase++);
    }

  #ifdef _DEBUG
    void CheckIsAllSegmentsOnMergeLine(const OdGeLine3d& mergeLine, const OdGeTol& tol)
    {
      for(OdUInt32 iPoint = 0; iPoint < m_segmentPoints.size(); iPoint++)
      {
        const SegmentPoint& point = m_segmentPoints[iPoint];
        //DEBUG_DRAW(point.pt, 0);

        ODA_ASSERT_ONCE(mergeLine.isOn(point.pt, tol));
      }
    }
  #endif

    void CalculatePointsParamAlongMergeLine(const OdGeLine3d& mergeLine, const OdGeTol& tol)
    {
      const OdUInt32 pointsCount = m_segmentPoints.size();
      for(OdUInt32 iPoint = 0; iPoint < pointsCount; iPoint++)
      {
        SegmentPoint& point = m_segmentPoints[iPoint];
        point.mergeLineParam = mergeLine.paramOf(point.pt, tol);
      }
    }

    void SortSegmentPointsAlongMergeLine()
    {
      std::sort( m_segmentPoints.begin(), m_segmentPoints.end(), SegmentPointLess() );
    }

    void MergeSegments(SlicerBaseImpl* pGraph, const OdGeTol& tol);
    void MergeSegments(OdArray<GraphEdgeData>& face_edges, const OdGeTol& tol);

    bool IsOnSegment() const
    {
      return (!m_inSegments.empty());
    }

    bool CanExtractCurrentSegment(const OdGePoint3d& currentPoint, const OdGeTol& tol)
    {
      if(!IsOnSegment())
        return false;

      if(m_ptPreviousPoint.isEqualTo(currentPoint, tol))
        return false;

      return true;
    }

    void UpdateInSegmentsArray(OdUInt32 currentPtSegmentId)
    {
      if(!m_inSegments.contains(currentPtSegmentId))
      {
        m_inSegments.append(currentPtSegmentId);
      }
      else
      {
        m_inSegments.remove(currentPtSegmentId);
      }
    }
  };


  class SlicerBaseImpl : public EdgeGraph 
  {
  protected:
    struct Intersection
    {
      OdGePoint3d   m_pt;
      double        m_param;
      const Edge*   m_pEdge;
      int m_bOnIntersectionSeg;

      Intersection() : m_pEdge(NULL), m_param(0.0), m_bOnIntersectionSeg(0)
      {}

      inline bool operator < ( const Intersection& intersection2 ) const {
        return m_param < intersection2.m_param;
      }
    };
    typedef OdArray<Intersection, OdMemoryAllocator<Intersection> > IntersectionArray;

    struct ParamLess 
    {
      inline bool operator() ( const Intersection& intersection1, const Intersection& intersection2 ) const {
        return intersection1.m_param < intersection2.m_param;
      }
    };

    struct RayFaceIntersection
    {
      RayFaceIntersection() :
        m_pFace(NULL)
      {
      }
      RayFaceIntersection(const OdGePoint3d& intPt, const Face* pFace) :
        m_intPt(intPt), m_pFace(pFace)
      {
      }

      OdGePoint3d m_intPt;
      const Face* m_pFace;
    };

    enum IntersResult 
    {
      kParallelPlanes,
      kIntersectingPlanes,
      kCoincidingPlanes
    };

    enum VertexHalfPlaneSign
    {
      ePositive,
      eNegative,
      eOnIntersectionLine/*,
      eExclude*/
    };

    struct VertexInPlanePosition
    {
      VertexInPlanePosition() : m_typeExclude(0), pEdge(NULL), m_bOnIntersectionSeg(0) {}
      VertexHalfPlaneSign halfPlane;
      const Edge* pEdge;
      int m_typeExclude; // 0 - not excluded, 1 - start exclude, 2 - end exclude
      int m_bOnIntersectionSeg;
    };
    typedef cycled_list<VertexInPlanePosition> VtxInPlanePosCycledList;

  protected:
    const Face*           m_pCurrentFace;
    const OdGePlane*      m_pCutPlane;
    OdGeVector3d          m_cutPlane_normal;
    OdGeTol               m_tol;
    OdGeLine3d            m_intersectionLine;
    FaceConstPtrArray     m_coinciding;
    FaceConstPtrArray     m_opposite;

    VtxInPlanePosCycledList m_classifiedVertices;
    IntersectionArray   m_intersections;
    OdGeLineSeg3d       m_edgeSeg;
    SegmentMerger       m_segmentsMerger;

    Intersection* m_pCurIntPt;
    Intersection* m_pEndIntPt;
    Intersection* m_pCur;
    double m_eq;

    OdGeRay3d* m_pRay;
    OdArray<RayFaceIntersection> m_rayFaceInters;

    std::list<const Face*> m_collectedFaces;
    std::map<const Edge*, OdGePoint3d> m_collectedIntersectionPts;
    std::map<const Edge*, VertexInPlanePosition> m_collectedIntersectionFromVertex;
    SegmentMerger* m_pCurSegMerger;

  public:
    SlicerBaseImpl();

    void collect_face(const Face* pFace);

    void collect_all_segments();

    // searching ray (face normal) intersections with body faces
    void checkFaceByRay(const Face* pFace);

    void reserveBuffers( size_t nFaces );

    const OdGeTol& tolerance() const { return m_tol; }

    void setTolerance( const OdGeTol& tol ) { m_tol = tol; }

    void set_cut_plane( const OdGePlane& cutPlane );

    void build_results(Profile2D* pResInclBndry, 
                        Profile2D* pResExclCoBndry,
                        Profile2D* pResExclOpBndry,
                        Profile2D* pResExclBndry,
                        FaceConstPtrArray* pCoincidingFaces,
                        FaceConstPtrArray* pOppositeFaces,
                        bool bUseReverseEdgeAtMerge,
                        bool& checkFaceByRay,
                        OdArray<const Edge*>* sourceEdges = 0,
                        const OdGeExtents2d* pIncBox = 0,
                        const OdGeExtents2d* pOrigBox = 0);

    void build_profiles(Profile2D* pResInclBndry, 
                            Profile2D* pResExclCoBndry,
                            Profile2D* pResExclOpBndry,
                            Profile2D* pResExclBndry,
                            FaceConstPtrArray* pCoincidingFaces,
                            FaceConstPtrArray* pOppositeFaces,
                            OdArray<const Edge*>* sourceEdges /*= 0*/,
                            const OdGeExtents2d* pBox);

    const FaceConstPtrArray& getOpposite() const;
    const FaceConstPtrArray& getCoinciding() const;

  public:
    void AddEdgeToGraph(const OdGePoint3d& ptStart, const OdGePoint3d& ptEnd, const Edge* tag = 0);

  protected:

    // true - collecting segments
    // false - checking if face is inside or outside of body
    bool mode;

    void ClearFaceIntersectionsData();

    const OdGePlane& current_face_plane() const;
    const OdGePlane& cut_plane() const;

    IntersResult IntersectCutPlaneToCurrentFacePlane(double tol);
    IntersResult IntersectCutPlaneToFacePlane(const Face* f) const;

    void MergeSegmentsAndAddToEdgeGraph();
    void MergeSegmentsAndCollectGraphEdges(OdArray<GraphEdgeData>& face_edges);

    void initPointExtraction();

    bool extractSegment( const OdGePoint3d*& pStart, const OdGePoint3d*& pEnd, bool& bInside );

    void ExtractSegmentsByIntersections();  

    SlicerBaseImpl::Intersection* FindNextDifferentIntersection(SlicerBaseImpl::Intersection* pIntBase);

    SlicerBaseImpl::Intersection* FindIntersectionEnterInside();

    bool IntersectionsCoincident(const SlicerBaseImpl::Intersection* pInt1, const SlicerBaseImpl::Intersection* pInt2);

    void exclude_faces( Profile2D& excludedProfile, FaceConstPtrArray& excludeFaces, OdArray<const Edge*>* pSourceEdges = NULL );
    void exclude_opposite_faces( Profile2D& excludedProfile, FaceConstPtrArray& excludeFaces, OdArray<const Edge*>* pSourceEdges = NULL );

    void FindIntersectionSegmentsWithCutPlane();

    void ClassifyLoopVertices(Edge* pFirstLoopEdge);
    SlicerBaseImpl::VertexHalfPlaneSign ClassifyPointAboutIntersectionLine(const OdGePoint3d& ptToClassify, bool exact = false);

    void CollectSegmentsOnIntersectionLine();

    void CollectIntersections();

    void MergePairedIntersections();

    bool ExtractSegmentFromVertex(VtxInPlanePosCycledList::iterator& itBase);

    VtxInPlanePosCycledList::iterator FindOnSegmentStartVertex(VtxInPlanePosCycledList::iterator& itBase);

    VtxInPlanePosCycledList::iterator FindOnSegmentEndVertex(VtxInPlanePosCycledList::iterator& itBase);

    bool VertexOnIntersectionLine(const VtxInPlanePosCycledList::iterator& itVtx) const;

    const OdGePoint3d& VertexPoint(const VtxInPlanePosCycledList::iterator& itVtx) const;

    bool IsIntersectionOnVertex(VtxInPlanePosCycledList::iterator& itVtx);

    bool IsIntersectionOnEdge(VtxInPlanePosCycledList::iterator& itVtx);

    void AddIntersectionFromVertex(const VertexInPlanePosition& vtxData);

    void  AddIntersectionFromEdge(const VertexInPlanePosition& vtxData);

    void AddIntersection(const OdGePoint3d& ptIntersection, const Edge* pEdge, int bOnIntersectionSeg = 0);

    void CollectEdgeForEdgeGraph(const OdGePoint3d& ptStart, const OdGePoint3d& ptEnd, const Edge* tag = 0);

    void AddFaceEdgesAsSegmentsToEdgeGraph();

    // adding box sidex as edges to graph
    void AddScaledBoxContour(const OdGeExtents2d& bbox);
    bool CloseByBox(const OdGeExtents2d& bbox, const OdGeExtents2d& origBBox, bool& checkFaceByRay);

#ifdef _DEBUG
    void DebugDrawIntersections() const;
#endif
  };
}

#endif //AECSLICERBASEIMPL_H_INCLUDED
