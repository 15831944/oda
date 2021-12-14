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
#ifndef FMSLICECONTOURBUILDER_H_INCLUDED
#define FMSLICECONTOURBUILDER_H_INCLUDED

#include "FMProfile3D.h"
#include "FMGeometry.h"
#include "FMGeometryDebug.h"
#include "Contours/FMEdgeGraph.h"

#include "Ge/GePlane.h"
#include "Ge/GeLineSeg3d.h"
#include <UInt32Array.h>
#include "Si/SiSpatialIndex.h"
#include "Si/SiShapePlane.h"
#include "Ge/GeLine3d.h"
#include "FMDebugDraw.h"

namespace FacetModeler
{
  //////////////////////////////////////////////////////////////////////////
  // special container type used to keep sequenced data, but has ability fast search for value
  //////////////////////////////////////////////////////////////////////////
  template<typename T>
  class TListSet
  {
    std::list<T> m_lstValues; // keeps sequence
    std::set<T> m_setValues; // used for fast search

  public:
    TListSet() {};
    ~TListSet() {};

    const std::list<T>& get_list() const
    {
      return m_lstValues;
    }

    const T& front() const
    {
      return m_lstValues.front();
    }

    const T& back() const
    {
      return m_lstValues.back();
    }

    void push_back(const T& aVal)
    {
      m_lstValues.push_back(aVal);
      m_setValues.insert(aVal);
    }

    size_t size() const
    {
      return m_lstValues.size();
    }

    void clear()
    {
      m_lstValues.clear();
      m_setValues.clear();
    }

    void pop_back()
    {
      if(size() == 0)
        return;

      const T& aVal = m_lstValues.back();
      m_setValues.erase(aVal);
      m_lstValues.pop_back();
    }

    bool has_value(const T& aVal) const
    {
      return (m_setValues.find(aVal) != m_setValues.end());
    }

    bool is_equal(const TListSet<T>& t2) const
    {
      return (m_lstValues == t2.m_lstValues);
    }
  };

  //////////////////////////////////////////////////////////////////////////
  // A sequence of edges indices in EdgeGraph object. The sequence represents closed contour of graph edges
  //////////////////////////////////////////////////////////////////////////
  class SliceContour
  {
    TListSet<OdUInt32> aContour;
    OdUInt32 m_hash;

  public:
    typedef std::list<OdUInt32> TContour;

  public:
    SliceContour() : m_hash(0) {};
    ~SliceContour() {};

    OdUInt32 GetHash() const;    

    const TContour& GetContour() const;
    void AddEdge(OdUInt32 iEdge);

    OdUInt32 GetLastEdge() const;
    OdUInt32 GetFirstEdge() const;

    size_t Size() const;
    void Clear();

    void RemoveLastEdge();

    bool HasEdge(OdUInt32 iEdge) const;

    friend bool operator == (const SliceContour& c1, const SliceContour& c2);
  };

  bool operator == (const SliceContour& c1, const SliceContour& c2);
  

  //////////////////////////////////////////////////////////////////////////
  // A map of SliceContours sorted by it's hash key value. This is used for 
  // storing SliceContours and not let the container to have contours duplicates
  //////////////////////////////////////////////////////////////////////////
  typedef std::multimap<OdUInt32, SliceContour> SliceContoursMap;

  class SliceContourCollection
  {    
    SliceContoursMap m_slices;

  public:
    bool AddContour(const SliceContour& aContour);
    bool HasContour( const SliceContour& aContour ) const;

    SliceContoursMap& GetContours() { return m_slices; }
    const SliceContoursMap& GetContours() const { return m_slices; }
  };
  
  //////////////////////////////////////////////////////////////////////////
  // Encapsulates algorithm for building slice contours, based on EdgeGraph
  //////////////////////////////////////////////////////////////////////////
  //typedef std::vector<AECGe::Contour2D> ContoursVector;
  typedef Profile2D ContoursVector;
  typedef std::vector<OdUInt32> SliceNodes;

  class EdgeGraph2d;

  class FMGEOMETRY_API SliceContourBuilder
  {
    EdgeGraph& m_graph;  

    typedef std::set<OdUInt32> AssignedEdges;
    AssignedEdges m_assignedEdges;

    double m_extLeft;
    double m_extRight;
    double m_extTop;
    double m_extBottom;

  public:
    OdArray<const Edge*>* m_sourceEdges;

  private:
    bool IsEdgeAssigned(OdUInt32 edgeIdx) { return m_assignedEdges.find(edgeIdx) != m_assignedEdges.end(); }
    void SetEdgeAssigned(OdUInt32 edgeIdx) { m_assignedEdges.insert(edgeIdx); }

    void InitializeExtents(const OdGePlane& rCutPlane, const SliceContoursMap& aSlices);
    void AddPoint2dToExtents(const OdGePoint2d& ptVertex2d);
    void RemoveOverlappingContours( ContoursVector& rContours2d );

  private:
    bool RecursiveFindCycle( SliceContour& aContour, SliceContourCollection& aSlices );
    void MarkDuplicatedEdges();
    void SplitSliceToContours2d( const OdGePlane& rCutPlane, const SliceContourCollection& aSliceContours, ContoursVector& rContours2d );

    SliceNodes GetSliceContourNodes( const SliceContour& aSlice, OdArray<const Edge*>* aTag = NULL);
    void MakeClosedContour( Contour2D& rResult );
    void MakeClosedContour2dFromSliceNodes( const OdGePlane& rCutPlane, const SliceNodes& rSliceNodes, Contour2D& rResult, const OdArray<const Edge*>& aTag);

    void AddSurfacesToProfile( const ContoursVector& rContours2d, Profile2D& rResult );
    void CutHolesFromProfile( const ContoursVector& rContours2d, Profile2D& rResult );
    void BuildResultProfile( ContoursVector& rContours2d, Profile2D& rResult );

  public:
    SliceContourBuilder(EdgeGraph& aGraph) : m_graph (aGraph), m_sourceEdges(NULL)
    {
      m_extLeft = 0.0;
      m_extRight = 0.0;
      m_extTop = 0.0;
      m_extBottom = 0.0;
    };
    ~SliceContourBuilder() {};

    bool IsContourClosed( const SliceContour& aContour );

    bool FindContour( std::vector<OdUInt32>& aContour );
    
    void FindSliceContours( SliceContourCollection& aSliceContours );

  public:
    void BuildSliceProfile( const OdGePlane& rCutPlane, Profile2D& rResult );

  private:    
    OdGePlane m_cutPlane;

  public:
    void BuildSliceProfile2( const OdGePlane& rCutPlane, Profile2D& rResult );

  private:
    void FindSliceContours2( SliceContourCollection& aSliceContours ) ;


  private:
    // DEBUG FUNCTIONS
    typedef std::vector<SliceEdge> Edges;

    void DebugDrawEdgeGraph();
    void DebugDrawEdge(OdUInt32 uEdgeIdx, Edges& drawnEdges, OdInt16 edgeBaseColor = 0);
    void DebugDrawEdge(OdUInt32 uEdgeIdx, OdInt16 edgeBaseColor = 0);
    bool HasCoincidentEdge(OdUInt32 uEdgeIdx, const Edges& drawnEdges);
    bool HasCoincidentOppositeEdge(OdUInt32 uEdgeIdx, const Edges& drawnEdges);
    void DebugDrawSliceContour(SliceContour& contour);
    void DebugDrawSliceContourCollection(SliceContourCollection& contoursColl);
    void DebugCheckForDuplicatedNodes();
  };
}

#endif //FMSLICECONTOURBUILDER_H_INCLUDED
