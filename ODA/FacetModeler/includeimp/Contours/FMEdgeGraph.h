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
#ifndef AECEDGEGRAPH_H_INCLUDED
#define AECEDGEGRAPH_H_INCLUDED

#include "FMProfile3D.h"
#include "FMGeometry.h"
#include "FMGeometryDebug.h"

#include "Ge/GePlane.h"
#include "Ge/GeLineSeg3d.h"
#include <UInt32Array.h>
#include "Si/SiSpatialIndex.h"
#include "Si/SiShapePlane.h"
#include "Ge/GeLine3d.h"
#include "FMDebugDraw.h"

namespace FacetModeler
{
  struct SliceNode
  {
    SliceNode( const OdGePoint3d& point )
    {
      pt = point;
      bProcessed = false;
    }

    SliceNode()
    {
      bProcessed = false;
    }

    OdGePoint3d pt;
    OdUInt32Array aOut;
    bool bProcessed;
  };

  //////////////////////////////////////////////////////////////////////////
  //
  //////////////////////////////////////////////////////////////////////////
  struct SliceEdge
  {
    SliceEdge( OdUInt32 s, OdUInt32 e, const Edge* _tag = 0, bool _bLeaveOrder = false )
    {
      iS = s;
      iE = e;
      bProcessed = false;
      tag = _tag;
      bLeaveOrder = _bLeaveOrder;
    }

    SliceEdge()
    {
      iS = 0;
      iE = 0;
      bProcessed = false;
      tag = 0;
      bLeaveOrder = false;
    }

    bool IsOpposite(const SliceEdge& edge) const
    {
      return (iS == edge.iE && iE == edge.iS);
    }

    OdUInt32 iS;
    OdUInt32 iE;
    bool bProcessed;
    const Edge* tag;
    bool bLeaveOrder;
  };

  static bool operator == (const SliceEdge& e1, const SliceEdge& e2)
  {
    return (e1.iS == e2.iS && e1.iE == e2.iE);
  }

 
  //////////////////////////////////////////////////////////////////////////
  //
  // EdgeGraph
  //////////////////////////////////////////////////////////////////////////
  class FMGEOMETRY_API EdgeGraph
  {
  protected:
    OdArray< SliceEdge, OdMemoryAllocator<SliceEdge> > aEdges;
    OdArray< SliceNode > aNodes;

  protected:
    void ReserveBuffersIfEmpty();
    bool LeaveOrder( const OdGePoint3d& ptS, const OdGePoint3d& ptE, const OdGeVector3d& vFaceNormal, const OdGeVector3d& vCutNormal ) ;

  public:
    OdUInt32 EdgesCount() const;
    OdUInt32 NodesCount() const;

    OdUInt32 GetEdgeStartNodeIdx(OdUInt32 aEdgeIdx) const;
    OdUInt32 GetEdgeEndNodeIdx(OdUInt32 aEdgeIdx) const;

    bool IsEdgeProcessed(OdUInt32 aEdgeIdx) const;
    void SetEdgeProcessed(OdUInt32 aEdgeIdx);
    void ResetEdgeProcessed(OdUInt32 aEdgeIdx);

    const SliceNode& GetEdgeStartNode(OdUInt32 aEdgeIdx) const;
    const SliceNode& GetEdgeEndNode(OdUInt32 aEdgeIdx) const;

    SliceNode& GetEdgeStartNode(OdUInt32 aEdgeIdx);
    SliceNode& GetEdgeEndNode(OdUInt32 aEdgeIdx); 

    const OdGePoint3d& GetEdgeStartPoint(OdUInt32 aEdgeIdx) const;
    const OdGePoint3d& GetEdgeEndPoint(OdUInt32 aEdgeIdx) const;

    const SliceNode& GetNode(OdUInt32 aNodeIdx) const;
    SliceNode& GetNode(OdUInt32 aNodeIdx);

    const SliceEdge& GetEdge(OdUInt32 aEdgeIdx) const;
    SliceEdge& GetEdge(OdUInt32 aEdgeIdx);

    const OdGePoint3d& GetNodePosition(OdUInt32 aNodeIdx) const;

    // checks if the end node of the previous edge is the same as the start node of the next edge
    // if true, then the edges segments are connected end-to-start ( represent two segments of polyline )
    bool IsEdgesConcatenated(OdUInt32 iPreviousEdge, OdUInt32 iNextEdge) const;

  public:
    void AddEdge( const OdGeLineSeg3d& lSeg, const OdGePlane& pSegPlane, const OdGePlane& pCutPlane );

    void AddEdge( const OdGePoint3d& ptSProjected,
      const OdGePoint3d& ptEProjected,
      const OdGePoint3d& ptSOriginal,
      const OdGePoint3d& ptEOriginal,
      const OdGePlane& pSegPlane,
      const OdGePlane& pCutPlane );

    void AddEdge( const OdGePoint3d& ptS,
      const OdGePoint3d& ptE,
      const OdGePlane& pSegPlane,
      const OdGePlane& pCutPlane,
      const Edge* tag = 0,
      const OdGeTol tol = FMGeGbl::gTol );

    OdUInt32 AddNode( const OdGePoint3d& pt, const OdGeTol tol = FMGeGbl::gTol );

    void BuildLinks();
    void RemoveCrossLinks();
    void RemoveOppositeEdges();
    void MergeDuplicatedEdges(bool bUseReverseEdgeAtMerge);

    void ResetEdgesProcessedFlag();
    void Clear();
    void ReserveBuffers( size_t nFaces );

    void ClearLinks();
    void AddEdge(OdUInt32 iS, OdUInt32 iE);
    double GetAddNodeTol(const OdGeTol tol = FMGeGbl::gTol) const;
    void RemoveEdge(OdUInt32 idx, OdGePoint2dArray* pArr = NULL);
    void RestoreEdgeNodesIdxs(OdUInt32 idx1, OdUInt32 idx2);
    bool DoesEdgeExist(OdUInt32 iS, OdUInt32 iE, OdUInt32& res) const;

    // changing the order of exiting edges
    // according to angle
    void ReorderLinks(const OdGePlane& plane);

#ifdef _DEBUG
  public:

    struct SliceEdgeData 
    {
      SliceEdge edge;
      int overlappedCount;
      int oppositeCount;

      SliceEdgeData() : overlappedCount(0), oppositeCount(0)
      {}
    };

    void DebugDrawEdges() const;
    void DebugDrawGraph();
    void DebugDrawEdge(OdUInt32 aEdgeIdx, OdInt16 edgeBaseColor);
    void DebugDrawColoredGraph();
    void DebugDrawEdge(const SliceEdge& edge, OdInt16 edgeBaseColor);
    void DebugDrawOppositeOverlappedEdge(const SliceEdge& edge, OdInt16 color);
    void DebugDrawColoredOrderedGraph(OdInt16 edgeBaseColor = 0);

    void DebugDrawEdge2d(const SliceEdge& edge, const OdGePoint2dArray nodes2d, OdInt16 edgeBaseColor);
    void DebugDrawOppositeOverlappedEdge2d(const SliceEdge& edge, const OdGePoint2dArray& nodes2d, OdInt16 color);
    void DebugDrawColoredOrderedGraph2d(const OdGePlane& plane, OdInt16 edgeBaseColor);
#endif
  };
}

#endif //AECEDGEGRAPH_H_INCLUDED
