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




// GiBaseVectorizer.h: interface for the OdGiBaseVectorizer class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _ODGITRANSFORMED_INCLUDED_
#define _ODGITRANSFORMED_INCLUDED_


#include "Gi/GiBaseVectorizer.h"
#include "OdStack.h"

#include "TD_PackPush.h"

/** \details
This class is an implementation of transformation stack.
Corresponding C++ library: TD_Gi
<group OdGi_Classes>
*/
template <class T> class OdGiTransformed : public T
{
  /** \details
    Represents geometrical transformations between world and model spaces.
  */
  struct Transforms
  {
    /** \details
      Default constructor for the Transform structure. Sets that world to model transformation is invalid.
    */
    Transforms() : bWorldToModelValid(false) {}
    /** Model to world transform matrix.*/
    OdGeMatrix3d          xModelToWorld;
    /** World to model transform matrix.*/
    mutable OdGeMatrix3d  xWorldToModel;
    /** Flag that shows whether world to model transform is valid.*/
    mutable bool          bWorldToModelValid;
  };
  OdStack<Transforms>      m_TransformStack;

  public:

    /** \details
      Pushes a model transformation matrix to the transform stack.
      \param xMat [in]   Transformation matrix.
    */
    void pushModelTransform(const OdGeMatrix3d& xMat)
    {
      const Transforms* pPrev = m_TransformStack.top();
      Transforms* pNew = m_TransformStack.push();
      if(pPrev)
      {
        pNew->xModelToWorld.setToProduct(pPrev->xModelToWorld, xMat);
      }
      else
      {
        pNew->xModelToWorld = xMat;
      }
    }
  
    /** \details
      Pushes a model transformation matrix to the transform stack by the passed vector.
      \param vNormal [in]   Nornal to the plane for which the matrix that maps the plane coordinate system to the WCS is retrieved.
    */
    void pushModelTransform(const OdGeVector3d& vNormal)
    {
      pushModelTransform(OdGeMatrix3d::planeToWorld(vNormal));
    }
  
    /** \details
      Pops a model transformation matrix from the transform stack.
    */
    void popModelTransform()
    {
      m_TransformStack.pop();
    }

    /** \details
      Retrieves the model to world transform. Returns the identity matrix If transform stack is empty.
    */
    OdGeMatrix3d getModelToWorldTransform() const
    {
      if(m_TransformStack.top())
        return m_TransformStack.top()->xModelToWorld;
      return OdGeMatrix3d::kIdentity;
    }

    /** \details
      Retrieves the world to model transform. Returns the identity matrix if transform stack is empty.
    */
    OdGeMatrix3d getWorldToModelTransform() const
    {
      const Transforms* pTop = m_TransformStack.top();
      if(pTop)
      {
        if(!pTop->bWorldToModelValid)
        {
          pTop->xWorldToModel = pTop->xModelToWorld.inverse();
          pTop->bWorldToModelValid = true;
        }
        return pTop->xWorldToModel;
      }
      return OdGeMatrix3d::kIdentity;
    }
};

#include "TD_PackPop.h"

#endif // #ifndef _ODGITRANSFORMED_INCLUDED_
