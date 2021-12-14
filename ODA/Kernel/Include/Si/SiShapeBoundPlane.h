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

#ifndef _SiShapeBoundedPlane_h_Included_
#define _SiShapeBoundedPlane_h_Included_

#include "Ge/GePlane.h"
#include "Ge/GeExtents3d.h"
#include "Ge/GeBoundBlock3d.h"
#include "Si/SiSpatialIndex.h"
#include "Si/SiShapePlane.h"

#include "TD_PackPush.h"

/** \details
    Represents bounded plane implementation for OdSiShape interface.

    Corresponding C++ library: SpatialIndex
    <group OdSi_Classes>
*/
struct OdSiShapeBoundPlane : public OdSiShapePlane
{
  /** Box bounding the plane. */
  OdGeExtents3d m_bounds;

  /** \details
    Creates a new instance of a shape bounded plane.

    \param origin       [in] Origin of plane.
    \param planeNormal  [in] The normal to the plane.
    \param bounds       [in] Box bounding the plane.
  */
  OdSiShapeBoundPlane( const OdGePoint3d& origin, const OdGeVector3d& planeNormal, const OdGeExtents3d& bounds ) :
    OdSiShapePlane(origin, planeNormal)
  {
    m_bounds = bounds;
  }

  /** \details
    Creates a new instance of a shape bounded plane.

    \param plane  [in] Plane.
    \param bounds [in] Box bounding the plane.
  */
  OdSiShapeBoundPlane( const OdGePlane& plane, const OdGeExtents3d& bounds) :
    OdSiShapePlane(plane)
  {
    m_bounds = bounds;
  }

  /** \details
    Checks if this shape bounded plane object contains the specified extents.

    \param extents [in] Any 3D extents box.
    \param planar  [in] Specifies whether a planar computation should be performed.
    \param tol     [in] Tolerance.

    \returns false.
  */
  bool contains(const OdGeExtents3d& extents, bool planar, const OdGeTol& tol) const
  {
    return false;
  }

  /** \details
    Checks if this shape bounded plane object intersects the specified extents.

    \param extents [in] Any 3D extents box.
    \param planar  [in] Specifies whether a planar computation should be performed.
    \param tol     [in] Tolerance.

    \returns true if and only if this shape bounded plane object intersects the specfied extents.
  */
  bool intersects(const OdGeExtents3d& extents, bool planar, const OdGeTol& tol) const
  {
    if (m_bounds.isDisjoint(extents, tol))
      return false;

    if (!OdSiShapePlane::intersects(extents, planar, tol))
      return false;

    return true;
  }

  /** \details
    Clones this shape bounded plane object.

    \returns a pointer to the newly created shape object.
  */
  virtual OdSiShape* clone() const { return new OdSiShapeBoundPlane(m_pointOnPlane, m_planeNormal, m_bounds); }

  /** \details
    Transforms this shape bounded plane object.

    \param tf [in] Transformation matrix.
  */
  virtual void transform(const OdGeMatrix3d& tf)
  {
    OdSiShapePlane::transform(tf);
    m_bounds.transformBy(tf);
  }
};

#include "TD_PackPop.h"

#endif
