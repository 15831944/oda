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

#ifndef __FMMDL_EDGE_H__
#define __FMMDL_EDGE_H__

#include "Modeler/FMMdlVertex.h"
#include "Modeler/FMMdlFace.h"

namespace FacetModeler
{
  /** Silhouette edge flag */
  const OdUInt32 AEF = FLAG(0);
  /** Invisible edge flag */
  const OdUInt32 VEF = FLAG(1);

  /** \details
     Represents facet body edge objects.
  */
  class FMGEOMETRY_API Edge : public Entity
  {
  public:
    /** \details
       Default constructor.
    */
    Edge();

    /** \details
       Creates an edge for a given face with specified start vertex, previous edge and edge partner (superposed edge).
      
       \param pVertex   [in] Start vertex for edge.
       \param pFace     [in] Owner face.
       \param pPrevEdge [in] Previous edge.
       \param pPartner  [in] Edge partner.
    */
    Edge(Vertex* pVertex, Face* pFace, Edge* pPrevEdge, Edge* pPartner);

    /** \details
      Gets the loop that owns the edge.

      \returns Pointer to the loop that owns the edge.
    */
    Face::EdgeLoop* loop() const;

    /** \details
      Gets the face that owns the edge.

      \returns Pointer to the face that owns the edge.
    */
    Face* face() const;

    /** \details
      Gets the first vertex on the edge.

      \returns Pointer to the first vertex on the edge.
    */
    Vertex* vertex() const;

    /** \details
       Gets the end vertex.

       \returns Pointer to the last vertex on the edge.
    */
    Vertex* endVertex() const;

    /** \details
      Gets the start point of the edge as OdGePoint3d.

      \returns The start point.
    */
    const OdGePoint3d& startPoint() const;

    /** \details
       Gets the end point of the edge as OdGePoint3d.

       \returns The end point.
    */
    const OdGePoint3d& endPoint() const;

    /** \details
       Calculates the length of the edge.

       \return The length of the edge as a double value.
    */
    double length() const;

    /** \details
       Gets a vector representation of the edge.

       \returns This edge as an OdGeVector3d object with direction from the start point to the end point.
    */
    OdGeVector3d vector() const;

    /** \details
       Gets the next edge.

       \returns Pointer to the next connected edge.
    */
    Edge* next() const;

    /** \details
       Gets the previous edge.

       \returns Pointer to the previous connected edge.
    */
    Edge* prev() const;

    /** \details
       Gets the manifold pair edge.

       \returns Pointer to the manifold pair edge.

       \remarks
       An epsilon-region between this->face() and pair()->face()
       must contain only INNER points of a valid 3D solid. 
       The adjacent solid angle may contain inner and outer points.
    */
    Edge* pair() const;

    /** \details
       Gets the next edge in a single-linked loop of superposed edges.

       \returns Pointer to the next superposed edge.
    */
    Edge* nextPartner() const;

    /** \details
       Checks if the specified edge is a partner edge (superposed edge) to this edge.

       \returns true if the edges are partners (superposed), false otherwise.
    */
    bool isPartner(const Edge* pEdge) const;

    /** \details
       Checks whether the edge is manifold.

       \returns true if the edge is between exactly two faces, false otherwise.
    */
    bool isManifold() const;

    /** \details
       Calculates a planar angle between this edge and the previous one.

       \returns A planar angle (in radians) between this edge and the previous one as a double value.

       \remarks
       Angle is measured in CCW direction with face normal as a reference vector.
       The value is in the range [ 0 .. Oda2PI ].
    */
    double angleBetweenEdges() const;

    /** \details
       Calculates an inner angle between the face that owns this edge and the neighboring face.

       \returns An inner angle (in radians) between the face that owns this edge and the neighboring face as a double value.

       \remarks
       The value is in the range [ 0 .. Oda2PI ]
    */
    double angleBetweenFaces() const;

    /** \details
       Calculates the unit normal of the edge on the face.

       \returns The unit normal vector to the edge.

       \remarks
       The vector is on the owner face.
    */
    OdGeVector3d normal() const;

    /** \details
       Calculates the direction vector from the start of this edge to the start of the next one.

       \returns Edge direction vector as OdGeVector3d.
    */
    OdGeVector3d direction() const;

    /** \details
       Calculates the loop normal.

       \returns Edge loop normal multiplied by the area of the edge loop as OdGeVector3d.
    */
    OdGeVector3d calculateLoopNormal() const;

    /** \details
      Adds a next edge to the bidirectional list.

      \param pNext [in] Pointer to the edge to add.
    */
    void addNext(Edge* pNext);

    /** \details
      Sets pointers to the previous and next edges for this edge.

      \param pPrev [in] Previous edge pointer to set.
      \param pNext [in] Next edge pointer to set.

      \remarks
      For internal use only.
    */
    void set(Edge* pPrev, Edge* pNext);

    /** \details
      Sets a next edge.

      \param pNext [in] Next edge pointer to set.

      \remarks
      For internal use only.
    */
    void setNext(Edge* pNext);

    /** \details
      Sets a previous edge.

      \param pNext [in] Previous edge pointer to set.

      \remarks
      For internal use only.
    */
    void setPrev(Edge* pPrev);


    /** \details
      Sets a pair edge.

      \param pPair [in] Pair edge pointer to set.

      \remarks
      For internal use only.
    */
    void setPair(Edge* pPair);

    /** \details
       Sets a vertex of this edge.

       \param pVertex [in] Vertex pointer to set.

       \remarks
       For internal use only.
    */
    void setVertex(Vertex* pVertex);

    /** \details
       Sets the owner face of this edge.

       \param pFace [in] Pointer to the owner face to set.

       \remarks
       For internal use only.
    */
    void setFace(Face* pFace);

    /** \details
       Sets the owner loop.

       \param pLoop [in] Loop pointer to set.

       \remarks
       For internal use only.
    */
    void setLoop(Face::EdgeLoop* pLoop);

    /** \details
       Sets a partner edge.

       \param pPartner [in] Partner edge pointer to set.

       \remarks
       For internal use only.
    */
    void setPartner(Edge* pPartner) { m_pNextPartner = pPartner; }

    /** \details
       Adds a partner edge.

       \param pPartner [in] Partner edge pointer to add.

       \remarks
       For internal use only.
    */
    void addPartner(Edge* pPartner);

    /** \details
       Removes the partner edge.

       \remarks
       For internal use only.
    */
    void removePartner();

    /** \details
       Gets the color of this edge.

       \returns The color of this edge as an OdUInt32 value.
    */
    OdUInt32 color() const;

    /** \details
      Sets the color of this edge.

      \param color [in] New color to set.
    */
    void setColor(OdUInt32 iColor);

    /** \details
       Sets all pointers to NULL.
    */
    void zero();

    /** \details 
      Destructor.
    */
    ~Edge();

    /** \details
      Gets the mapping coordinate of start vertex of this edge.

      \param isInitialized [out] Determines if the mapping coordinate value was initialized.
      \returns The mapping coordinate of start vertex of this edge.
    */
    const OdGePoint2d& mappingCoord(bool& isInitialized) const;

    /** \details
      Sets the mapping coordinate of start vertex of this edge.

      \param coord [in] New mapping coordinate to set.
    */
    void setMappingCoord(const OdGePoint2d& coord);

  private:

    /** \details 
      Updates partner pairs.
    */
    void updatePartnerPairs();

    /** \details 
      Checks the correctness of the data of the object. Used in debug mode.
    
      \param bTestPartners [in] true - to check partners, false - otherwise.
    */
    void validate(bool bTestPartners = true) const;

    /** \details 
      Checks the correctness of the loop in which the edge is stored. Used in debug mode.
    
      \param bTestPartners [in] true - to check partners, false - otherwise.
    */
    void validateLoop(bool bTestPartners = true) const;

    /** \details 
      Adds specified edge to the specified loop.
    
      \param loop      [in/out] Loop object to add to.
      \param pEdge         [in] Pointer to the edge to add.
      \param bInverseOrder [in] true - for adding edge in inverted loop, false - for adding edge in normal loop.
    */
    static void addEdge(Face::EdgeLoop& loop, Edge* pEdge, bool bInverseOrder);

    /** \details 
      Copy constructor.
    */
    Edge(const Edge&);


    /** \details 
      Copies data from specified edge.
    
      \param edge [in] Edge to copy from.
    
      \returns Reference to this object.
    */
    Edge& operator=(const Edge& edge);

    Vertex* m_pVertex;    // Start vertex.
    Edge* m_pPrevEdge;    // Previous edge.
    Edge* m_pNextEdge;    // Next edge.
    Edge* m_pPairEdge;    // Overlapping edge from neighboring face.
    Edge* m_pNextPartner; // Next edge in a single-linked loop of overlapping edges.
    Face* m_pFace;        // Owner face.
    Face::EdgeLoop* m_pLoop; // Owner loop.
    OdUInt32 m_iColor;    // Edge color.
    OdGePoint2d m_mappingCoord; // Start vertex mapping coordinate.

    //DOM-IGNORE-BEGIN
    friend class Face;
    friend class Body;
    friend class BodyImpl;
    //DOM-IGNORE-END
  };

  // Inline alias method for the statement: this->next()->vertex();
  inline Vertex* Edge::endVertex() const {
    return next()->vertex();
  }

  inline Face::EdgeLoop* Edge::loop() const {
    return m_pLoop;
  }

  inline const OdGePoint3d& Edge::startPoint() const {
    return vertex()->point();
  }

  inline const OdGePoint3d& Edge::endPoint() const {
    return endVertex()->point();
  }

  inline double Edge::length() const {
    return startPoint().distanceTo(endPoint());
  }

  inline OdGeVector3d Edge::vector() const {
    return endPoint() - startPoint();
  }

  inline void Edge::setLoop(Face::EdgeLoop* pLoop) {
    m_pLoop = pLoop;
  }
}

#endif //__FMMDL_EDGE_H__
