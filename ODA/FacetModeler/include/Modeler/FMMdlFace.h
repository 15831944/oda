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

#ifndef __FMMDL_FACE_H__
#define __FMMDL_FACE_H__

#include "Modeler/FMMdlEntity.h"
#include "FMProfile3D.h"

namespace FacetModeler
{
  /** Visibility flag */
  const OdUInt32 VFF = FLAG(0);
  /** OFF flag */
  const OdUInt32 OFF = FLAG(1);
  /** Show or hide surface hatch */
  const OdUInt32 SKIP_HATCH = FLAG(18);

  /** \details
     Represents facet body face objects.
  */
  class FMGEOMETRY_API Face : public Entity
  {
  public:
    /** \details
       Default constructor.
    */
    Face();

    /** \details
       Gets a pointer to the next face.

       \returns Pointer to the next face object.
    */
    Face* next() const;

    /** \details
      Sets the pointer to the next face.

      \param pNext [in] Pointer to the next face to set.
    */
    void setNext(Face* pNext);

    /** \details
      Gets the pointer to the previous face.

      \returns Pointer to the previous face object.
    */
    Face* prev() const;

    /** \details
      Sets the pointer to the previous face object.

      \param pPrev [in] Pointer to the previous face to set.
    */
    void setPrev(Face* pPrev);

    /** \details
      Adds a face to the bidirectional list.

      \param pNext [in] Pointer to the face to add.
    */
    void addNext(Face* pNext);

    /** \details
      Sets pointers to the previous and next faces.

      \param pPrev [in] Pointer to the previous face to set.
      \param pNext [in] Pointer to the next face to set.
    */
    void set(Face* pPrev, Face* pNext);

    /** \details
      Gets the plane of this face.

      \returns The plane of this face as an OdGePlane object.
    */
    const OdGePlane& plane() const;

    /** \details
      Gets the normal of this face.

      \returns The normal of this face.
    */
    OdGeVector3d normal() const;

    /** \details
      Checks whether the face is convex.

      \returns true if this face is convex; false otherwise.

      \remarks
      Currently not implemented.
    */
    bool isConvex() const;

    /** \details
      Checks whether the face has holes.

      \returns true if the face has holes; false otherwise.
    */
    bool hasHoles() const;

    /** \details
      Gets the loop count.

      \returns Count of loops on this face.
    */
    OdUInt32 loopCount() const;

    /** \details
      Sets the loop count.

      \param n [in] New loop count to set.
    */
    void setLoopCount(OdUInt32 n);

    /** \details
      Returns the loop edge count with the specified index of a loop.

      \param iLoopIndex [in] Index of the loop on this face.
      \returns Count of edges on the specified loop.
    */
    OdUInt32 loopEdgeCount(OdUInt32 iLoopIndex = 0) const;

    /** \details
      Returns the first edge from a loop with the specified index.

      \param nLoop [in] Index of the loop.
      \returns Pointer to the first edge on the specified loop.

      \remarks
       Each face has at least one loop.
    */
    Edge* edge(OdUInt32 nLoop = 0) const;

    /** \details
      Appends an edge to the loop specified by the index.

      \param nLoop [in] Index of the loop on this face.
      \param pVertex [in] Pointer to the vertex for appending.
      \param bInverseOrder [in] true for counting from the end.
      \returns Pointer to the added edge.
    */
    Edge* addEdge(OdUInt32 nLoop, FacetModeler::Vertex* pVertex, bool bInverseOrder = false);

    /** \details
      Looks for an index of the loop that contains the specified edge on this face.

      \param pEdge [in] Pointer to the search edge.
      \returns Index of the loop if the loop is found, -1 otherwise.
    */
    OdUInt32 findLoop(const Edge* pEdge) const;

    /** \details
       Calculates the face area.

       \returns Calculated area.
    */
    double area() const;

    /** \details
       Removes plane information.
    */
    void deletePlane() const;

    /** \details
       Clears vertex tags.
    */
    void clearVertexTags();

    /** \details
       Clears edge tags.
    */
    void clearEdgeTags();

    /** \details
       Inverts the face normal.
    */
    void negate();

    /** \details
       Calculates a 3D profile with a transformation.

       \param rResult [out] Resulting three-dimensional profile.
       \param pPlane [in] Pointer to the plane on which the result is projected.
       \param bIncludeHoles [in] true for including holes in the result.
    */
    void profile(Profile3D& rResult, const OdGePlane* pPlane = NULL, bool bIncludeHoles = true) const;

    /** \details
       Calculates an orthogonal projection of the face onto a given plane.

       \param plane         [in] Plane on which the face is to be projected.
       \param result       [out] Resulting profile on the plane.
       \param bIncludeHoles [in] true to include holes in the result.
       \param pSourceEdges  [in] The array of edges that are included in the resulting projection.
    */
    void projection(const OdGePlane& plane, Profile2D& result, bool bIncludeHoles = true, OdArray<const Edge*>* pSourceEdges = NULL) const;

    /** \details
       Gets the face orientation state of this face.

       \returns The face orientation as a FaceOrientation enum value.
    */
    FaceOrientation orientation() const;

    /** \details
      Sets the face orientation.

      \param eOrientation [in] New face orientation.
    */
    void setOrientation(FaceOrientation eOrientation);

    /** \details
      Checks if the input line intersects this face and calculates the intersection point.

      \param gLine       [in] Input line to check for the intersection.
      \param pptResult  [out] (Optional) Resulting intersection point.
      \param pbCoplanar [out] (Optional) Result of the coplanar check.
      \returns true if this face and the specified line intersect, false otherwise.

      \remarks
      If the line is in the plane of this face, the method returns false and sets *pbCoplanar to true.
    */
    bool intersectLine(const OdGeLinearEnt3d& gLine, OdGePoint3d* pptResult = NULL, bool* pbCoplanar = NULL) const;

    /** \details
       Gets the pointer to the surface of this face.

       \returns Pointer to the surface of this face.
    */
    Surface* surface() const { return m_pSurface; }

    /** \details
      Sets the surface of this face.

      \param surface [in] Surface pointer to set.
    */
    void setSurface(Surface* surface);

    /** \details
       Gets the color of the current face.

       \returns The color of this face as an OdUInt32 value.
    */
    OdUInt32 color() const { return m_iColor; }

    /** \details
      Sets the color of this face.

      \param color [in] New color to set.
    */
    void setColor(const OdUInt32 color) { m_iColor = color; }

    /**
       Represents facet body edge loop objects.
    */
    struct EdgeLoop
    {

      /** \details
         Default constructor. 
      */
      EdgeLoop()
      {
        m_pFace = NULL;
        m_pFirstEdge = NULL;
        m_pNextLoop = NULL;
      }

      /** \details 
        Face owner pointer.
      */
      Face* m_pFace;
      /** \details 
        First edge pointer.
      */
      Edge* m_pFirstEdge;
      /** \details 
        Next loop pointer.
      */
      EdgeLoop* m_pNextLoop;

      /** \details
         Sets all pointers to zero.
      */
      void zero()
      {
        m_pNextLoop = NULL;
        m_pFirstEdge = NULL;
        m_pFace = NULL;
      }
    };

    /** \details
       Sets the edge loops list of this face.
      
       \param eloop [in] EdgeLoop pointer to set.
    */
    void setEdgeLoop(EdgeLoop eloop) { m_Loops = eloop; };

    /** \details
       Gets the edge loop of this face by the specified index.
      
       \param nLoop [in] Index of the edge loop on this face.
       \returns Pointer to the found loop if it is found, NULL otherwise.
    */
    EdgeLoop* loop(OdUInt32 nLoop) const;

    /** \details 
      Destructor.
    */
    ~Face();

  protected:
    /** \details 
      Copy constructor.
    
      \param face [in] Face to copy from.
    */
    Face(const Face& face);

    /** \details 
      Copies data from specified face.
    
      \param face [in] Face to copy from.
    
      \returns Reference to this object.
    */
    Face& operator=(const Face& face);

  private:
    /** \details 
      Clones face to the specified Body object.
    
      \param to [in/out] Body to which to add a face.
    
      \returns Pointer to the cloned face.
    */
    Face* clone(Body& to) const;

    /** \details 
      Removes all loops.
    */
    void clear();

    /** \details 
      Calculates a plane based on loops.
    */
    void evaluatePlane() const;

    /** \details 
      Checks the correctness of the data of the object. Used in debug mode.
    
      \param bTestPartners [in] true - to check partners, false - otherwise.
    */
    void validate(bool bTestPartners = false) const;

    /** \details 
      Sets all Edge pointers of loops to NULL.
    */
    void nullLoopEdgesRefs();

    /** \details 
      Inserts a new loop to the face at the specified index.
    
      \param nIndex     [in] Insertion position index.
      \param pFirstEdge [in] Pointer to the first edge of the added loop.
    
      \returns Pointer to the added loop.
    */
    EdgeLoop* insertLoop(OdUInt32 nIndex, Edge* pFirstEdge = NULL);

    Face*    m_pPrev;    // Previous face pointer.
    Face*    m_pNext;    // Next face pointer.
    EdgeLoop m_Loops;    // Loops list.
    Surface* m_pSurface; // Surface pointer.
    OdUInt32 m_iColor;   // Color data.
    mutable OdGePlane* m_Plane; // Calculated plane.

    //DOM-IGNORE-BEGIN
    friend class Edge;
    friend class Body;
    friend class BodyImpl;
    friend class BodyCustom;
    friend class BodyInternalConsistencyRestorer;
    //DOM-IGNORE-END
  };

  /** Array of constant Face pointers */
  typedef OdArray<const Face*, OdMemoryAllocator<const Face*> > FaceConstPtrArray;

}

#endif //__FMMDL_FACE_H__
