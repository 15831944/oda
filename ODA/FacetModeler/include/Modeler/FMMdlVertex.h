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

#ifndef __FMMDL_VERTEX_H__
#define __FMMDL_VERTEX_H__

#include "Modeler/FMMdlEntity.h"

namespace FacetModeler
{

  class VertexSurfData;

  /** \details
     Represents facet body vertex objects.
  */
  class FMGEOMETRY_API Vertex : public Entity
  {
  public:
    /** \details
       Constructor. Creates a vertex with the specified point position and owner body.

       \param ptPoint [in] Vertex point coordinates.
       \param pBody   [in] Pointer to the body to which the vertex will be added.
    */
    Vertex(const OdGePoint3d& ptPoint, Body* pBody);

    /** \details
       Gets point coordinates of the vertex.

       \returns Vertex position as an OdGePoint3d object.
    */
    const OdGePoint3d& point() const;

    /** \details
       Gets the previous vertex in the body.

       \returns The previous vertex in the body as an OdGePoint3d object.
    */
    Vertex* prev() const;

    /** \details
       Gets the next vertex in the body.

       \returns The next vertex in the body as an OdGePoint3d object.
    */
    Vertex* next() const;

    /** \details
       Sets the vertex position to the specified point.

       \param ptPoint [in] New vertex position to set.
    */
    void setPoint(const OdGePoint3d& ptPoint);

    /** \details
       Adds a new vertex to the bidirectional list.

       \param pNext [in/out] Vertex to add.
    */
    void addNext(Vertex* pNext);

    /** \details
       Sets pointers to the previous and next vertices.

       \param pPrev [in] Previous vertex to set.
       \param pNext [in] Next vertex to set.
    */
    void set(Vertex* pPrev, Vertex* pNext);

    /** \details
       Sets the pointer to the next vertex.

       \param pNext [in] Next vertex to set.
    */
    void setNext(Vertex* pNext);

    /** \details
       Sets the pointer to the previous vertex.

       \param pPrev [in] Next vertex to set.
    */
    void setPrev(Vertex* pPrev);

    /** \details
       Checks whether the specified surface owns this vertex.

       \param pSurface [in] Surface for checking.
       \returns true if the specified surface owns this vertex, false otherwise.
    */
    bool at(const Surface* pSurface) const;

    /** \details
       Gets the normal vector at this vertex on the specified face.

       \param atF     [in] Face for normal calculation.
       \param normal [out] Resulting normal as OdGeVector3d.
    */
    void normal(const Face& atF, OdGeVector3d& normal) const;

    /** \details
       Adds new surface data to this vertex object.

       \param pSurface [in] The surface data to add.
    */
    void addSurfaceData(Surface* pSurface);

    /** \details
       Removes existing surface data.

       \param pSurface [in] The surface data to remove.
    */
    void removeSurfaceData(Surface* pSurface);
    
    /** \details 
      Destructor.
    */
    ~Vertex();

  private:
    /** \details 
      Constructor.
    */
    Vertex();

    /** \details 
      Copy constructor.
    */
    Vertex(const Vertex&);

    /** \details 
      Copies data from specified vertex.
    
      \param other [in] Vertex to copy data from.
    
      \returns Reference to this object.
    */
    Vertex& operator=(const Vertex& other);

    /** \details 
      Clones vertex to the specified Body object.
    
      \param to [in/out] Body to which to add a vertex.
    
      \returns Pointer to the cloned vertex.
    */
    Vertex* clone(Body& to) const;

    OdGePoint3d m_pt; // Point coordinates.

    Vertex* m_pPrev;  // Previous vertex pointer.
    Vertex* m_pNext;  // Next vertex pointer.
    VertexSurfData* m_pSurfData; // Vertex Surface data.

    //DOM-IGNORE-BEGIN
    friend class Body;
    friend class BodyImpl;
    friend class BodyCustom;
    //DOM-IGNORE-END
  };

}

#endif //__FMMDL_VERTEX_H__
