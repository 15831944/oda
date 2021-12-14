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

#ifndef __FMMDL_ITERATORS_H__
#define __FMMDL_ITERATORS_H__

#include "Modeler/FMMdlBody.h"
#include "Modeler/FMMdlVertex.h"
#include "Modeler/FMMdlEdge.h"
#include "Modeler/FMMdlFace.h"

namespace FacetModeler
{

  /** \details
     Body vertex iterator.
  */
  class FMGEOMETRY_API VertexIterator
  {
  public:
    /** \details
       Constructor. Creates a vertex iterator for the given body.
      
       \param pBody [in] Pointer to the body to be iterated.
    */
    VertexIterator(const Body* pBody);

    /** \details
       Checks whether the iteration is finished.

       \returns true if the iteration is finished, false otherwise.
    */
    bool done();

    /** \details
       Sets the iterator to the next item.
    */
    void next();

    /** \details
       Gets the current item.

       \returns Pointer to the current item.
    */
    Vertex* get();

  private:
    Body* m_pOwner;     // Owner Body pointer.
    Vertex* m_pCurrent; // Current vertex pointer.
    Vertex* m_pNext;    // Next vertex pointer.
    Vertex* m_pFirst;   // First vertex pointer.
  };

  /** \details
     Body edge iterator.
  */
  class FMGEOMETRY_API EdgeBodyIterator
  {
  public:
    /** \details
       Constructor. Creates an edge iterator for the given body.

       \param pBody [in] Pointer to the body to be iterated.
    */
    explicit EdgeBodyIterator(const Body* pBody);

    /** \details
       Checks whether the iteration is finished.

       \returns true if the iteration is finished, false otherwise.
    */
    bool done();

    /** \details
       Sets the iterator to the next item.

       \param bNextLoop [out] If this parameter is passed, it's set to true if the pointer to the current loop has been changed to the next one; false otherwise.
    */
    void next(bool* bNextLoop = NULL);

    /** \details
       Gets the current item.

       \returns Pointer to the current item.
    */
    Edge* get();

  private:
    Body* m_pOwner;   // Owner Body pointer.
    Face* m_pCurrentFace; // Current face pointer.
    Face* m_pFirstFace; // First face pointer.
    OdUInt32 m_iCurrentLoop; // Current loop index.
    Edge* m_pFirst;   // First edge pointer.
    Edge* m_pCurrent; // Current edge pointer.
    Edge* m_pNext;    // Next edge pointer.
  };

  /** \details
     Face edge iterator.
  */
  class FMGEOMETRY_API EdgeFaceIterator
  {
  public:
    /** \details
       Constructor. Creates an edge iterator for the given face.

       \param pFace [in] Pointer to the face to be iterated.
    */
    explicit EdgeFaceIterator(const Face* pFace);

    /** \details
       Checks whether the iteration is finished.

       \returns true if the iteration is finished, false otherwise.
    */
    bool done();

    /** \details
       Sets the iterator to the next item.

       \param bNextLoop [out] If this parameter is passed, it's set to true if the pointer to the current loop has been changed to the next one; false otherwise.
    */
    void next(bool* bNextLoop = NULL);

    /** \details
       Gets the current item.

       \returns Pointer to the current item.
    */
    Edge* get();

  private:
    Face* m_pFace; // Owner face pointer.
    OdUInt32 m_iCurrentLoop; // Current loop index.
    Edge* m_pFirst;   // First edge pointer.
    Edge* m_pCurrent; // Current edge pointer.
    Edge* m_pNext;    // Next edge pointer.
  };

  /** \details
     Body face iterator.
  */
  class FMGEOMETRY_API FaceIterator
  {
  public:
    /** \details
       Constructor. Creates a face iterator for the given body.

       \param pBody [in] Pointer to the body to be iterated.
    */
    explicit FaceIterator(const Body* pBody);

    /** \details
       Checks whether the iteration is finished.

       \returns true if the iteration is finished, false otherwise.
    */
    bool done();

    /** \details
       Sets the iterator to the next item.
    */
    void next();

    /** \details
       Gets the current item.

       \returns Pointer to the current item.
    */
    Face* get();

  private:
    Body* m_pOwner;   // Owner body pointer.
    Face* m_pFirst;   // First face pointer.
    Face* m_pCurrent; // Current face pointer.
    Face* m_pNext;    // Next face pointer.
  };

}

#endif //__FMMDL_ITERATORS_H__
