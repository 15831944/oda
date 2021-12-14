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

#ifndef __ODGITESSELATOR_2_H_INCLUDED__
#define __ODGITESSELATOR_2_H_INCLUDED__

#include "TD_PackPush.h"

#include "OdList.h"
#include "OdArray.h"

#define ODGT_NAMESPACE OdGeTess2
#define ODGT_PTRTYPE Contour*
#define ODGT_PARENT

#define ODGT_MEMBERS2 \
public:\
static void *create(OdUInt32 num)\
{\
  return new Vertex[num];\
}

#define ODGT_MEMBERS \
friend class OptimizedHolder;\
 MemoryHolder *m_pHolder;\
public:\
static void *create(OdUInt32 num)\
{\
  return new Contour[num];\
}

#define ODGT_HOLDER , MemoryHolder &pHolder
#define ODGT_HOLDER2 , pHolder

/** \details

  <group TD_Namespaces>
*/
namespace ODGT_NAMESPACE
{
  class Vertex;
  class Contour;

  class GE_TOOLKIT_EXPORT MemoryHolder
  {
  public:
    virtual Vertex *newVertex() = 0;
    virtual void deleteVertex(Vertex *) = 0;

    virtual Contour *newContour() = 0;
    virtual void deleteContour(Contour *) = 0;

    virtual ~MemoryHolder() {}
  };
};


#include "GeTess.h"

#define HOLDER_(A, B) struct A\
{\
  bool         m_First;\
  B*           m_FirstPage;\
  OdList< B* > m_Buf;\
  OdUInt32     m_pageSize;\
  OdUInt32     m_pos;\
\
  A(OdUInt32 pageSize)\
    : m_First(true)\
    , m_FirstPage(0)\
    , m_pageSize(pageSize)\
    , m_pos(0)\
  {\
    m_FirstPage = (B*)B::create(m_pageSize);\
    /*m_Buf.resize(1, (B*)B::create(m_pageSize) );*/\
  }\
\
  void freeMem()\
  {\
    delete []m_FirstPage; m_FirstPage = 0;\
    OdList< B* >::iterator pIt = m_Buf.begin();\
    OdList< B* >::iterator pItEnd = m_Buf.end();\
\
    while(pIt != pItEnd)\
    {\
      delete[] *pIt;\
      ++pIt;\
    }\
  }\
\
  ~A()\
  {\
    freeMem();\
  }\
\
  B* newOne()\
  {\
    if (m_pos == m_pageSize)\
    {\
      m_First = false;\
      m_Buf.push_front( (B*)B::create(m_pageSize) );\
      m_pos = 0;\
    }\
    if (m_First)\
      return &m_FirstPage[m_pos++];\
    return &(*m_Buf.begin())[m_pos++];\
  }\
};

/** \details

  <group TD_Namespaces>
*/
namespace OdGeTess2
{
  /** \details
    This class is an optimized memory holder for management of contours and
    vertex memory.

    \remarks
    This class represents a memory manager for elements that take part in the
    tessellation process. It allocates memory for growing arrays in blocks and
    does not relocate memory as std::vector, for example. This approach
    increases performance and reduces the time required to tessellate contours.

    FOR INTERNAL USE.

    Corresponding C++ library: TD_Ge

    <group OdGe_Classes>
  */
  class GE_TOOLKIT_EXPORT OptimizedHolder : public MemoryHolder
  {
    HOLDER_(Holder_Contour, Contour)
    HOLDER_(Holder_Vertex, Vertex)

    Holder_Vertex  m_Vxs; // Array of vertices.
    Holder_Contour m_Ctrs; // Array of contours.

  public:
    /** \details
      Constructor for objects of the OptimizedHolder class.

      \param nPreAllocVertexes [in]  Number of vertices for which memory needs to be allocated.
      \param nPreAllocContours [in]  Number of contours for which memory needs to be allocated.

      \remarks
      Constructs an object with the defined number of elements for which memory
      should be allocated.
    */
    OptimizedHolder(OdUInt32 nPreAllocVertexes, OdUInt32 nPreAllocContours);

    /** \details
      Allocates memory for a new vertex.
    */
    virtual Vertex *newVertex() ODRX_OVERRIDE;

    /** \details
      Frees memory of a specified vertex.

      \param pVx [in]  A pointer to a vertex that should be deleted.

      \remarks
      Currently not implemented.
    */
    virtual void deleteVertex(Vertex * pVx) ODRX_OVERRIDE;

    /** \details
      Allocates memory for a new contour.
    */
    virtual Contour *newContour() ODRX_OVERRIDE;

    /** \details
      Frees memory of a specified contour.

      \remarks
      Currently not implemented.
    */
    virtual void deleteContour(Contour *) ODRX_OVERRIDE;

    /** \details
      Destructor for objects of the OptimizedHolder class.
    */
    ~OptimizedHolder();
  };
};

#undef ODGT_NAMESPACE
#undef ODGT_PTRTYPE
#undef ODGT_PARENT
#undef ODGT_MEMBERS
#undef ODGT_HOLDER
#undef ODGT_HOLDER2

#include "TD_PackPop.h"

#endif // __ODGITESSELATOR_2_H_INCLUDED__
