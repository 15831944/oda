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

#ifndef __ODGIDRAWABLEPATHNODE_H_INCLUDED_
#define __ODGIDRAWABLEPATHNODE_H_INCLUDED_


#include "OdError.h"

class OdDbStub;
class OdGiDrawable;
class OdGeMatrix3d;
 
/** \details

    Corresponding C++ library: TD_Gi
    <group OdGi_Classes> 
*/
class ODRX_ABSTRACT OdGiPathNode
{
public:
  /** \details
    Retrieves a parent node.
    \returns
    pointer to the OdGiPathNode that represents a parent node.
  */
  virtual const OdGiPathNode* parent() const = 0;

  /** \details
    Retrieves a persistent drawable identifier associated with this node.
    \returns
    pointer to the OdDbStub that represents a persistent drawable identifier.
  */
  virtual OdDbStub* persistentDrawableId() const = 0;

  /** \details
    Retrieves a transient drawable associated with this node.
    \returns
    pointer to the OdGiDrawable that represents a transient drawable.
  */
  virtual const OdGiDrawable* transientDrawable() const = 0;

  /** \details
    Retrieves a model to world transform matrix.
    \returns
    pointer to the OdGeMatrix3d that a transform matrix.
    \remarks
    throws eNotApplicable in current implementation.
  */
  virtual const OdGeMatrix3d* modelToWorld() const { throw OdError(eNotApplicable); }

  /** \details
    Retrieves a selection marker.
    \returns
    pointer to the OdGeMatrix3d that a transform matrix.
  */
  virtual OdGsMarker selectionMarker() const = 0;
  
  virtual ~OdGiPathNode() {}
};

#endif // __ODGIDRAWABLEPATHNODE_H_INCLUDED_
