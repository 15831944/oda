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

#ifndef __FMMDL_VERTEXSURFDATA_H__
#define __FMMDL_VERTEXSURFDATA_H__

#include "Modeler/FMMdlSurface.h"
#include "Modeler/FMMdlFace.h"

namespace FacetModeler
{
  /**
    Represents facet body vertex surface data objects.
  */
  class VertexSurfData : public Entity {
  public:
    /** \details
       Default constructor.
    */
    VertexSurfData();

    /** \details
       Constructor. Creates new vertex surface data and appends it to the list after the specified one.

       \param pVSD [in] The object from which to copy data.
    */
    VertexSurfData(VertexSurfData* pVSD);

    /** \details
       Constructor. Creates vertex surface data with the specified surface and appends it to the list after the specified one.

       \param pSurface      [in] The surface for which the object is created.
       \param pNextSurfData [in] Pointer to the next object.
    */
    VertexSurfData(Surface* pSurface, VertexSurfData* pNextSurfData);

    /** \details
       Creates a new VertexSurfData object from this object.

       \returns Pointer to newly cloned vertex surface data.
    */
    VertexSurfData* clone() const;

    /** \details
       Gets the assigned surface pointer.

       \returns Assigned surface pointer.
    */
    Surface* surface() const;

    /** \details
       Gets the next VertexSurfData pointer.

       \returns Pointer to the next VertexSurfData.
    */
    VertexSurfData* next() const;

    /** \details
       Gets the normal vector of this VertexSurfData.

       \returns Normal vector as an OdGeVector3d object.
    */
    const OdGeVector3d normal() const;

    /** \details
    Sets the normal vector of this VertexSurfData.

    \param n [in] data normal.
    */
    void setNormal(const OdGeVector3d& n);

    /** \details
       Gets the normal at the specified vertex on the specified face.
    
       \param atV [in] The vertex at which the normal is computed.
       \param atF [in] The face for which the normal is computed.
       \returns Normal at the specified vertex on the specified face.
    */
    OdGeVector3d normal(const Vertex& atV, const Face& atF) const;

    /** \details
       Checks if there is any data in the linked list that refers to the specified surface.

       \param pSurface [in] Surface for checking.
       \returns true if there is at least one VertexSurfData object that relates to the specified surface, false otherwise.
    */
    bool at(const Surface* pSurface) const;

    /** \details
       Sets the next surface data pointer.

       \param pVsd [in] Next surface data pointer to set.
    */
    void setNext(VertexSurfData* pVsd);

    /** \details
       Sets the surface.

       \param s [in] Surface pointer to set.
    */
    void setSurface(Surface* s);
    
    /** \details 
      Destructor.
    */
    ~VertexSurfData();

  private:
    Surface*  m_pSurface; // Surface pointer.
    mutable OdGeVector3d m_normal;   // Normal vector.
    VertexSurfData* m_pNextSurfData; // Next Surface data pointer.
  };

} // namespace FacetModeler

#endif //__FMMDL_VERTEXSURFDATA_H__
