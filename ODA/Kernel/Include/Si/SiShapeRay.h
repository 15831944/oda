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

#ifndef _SiShapeRay_h_Included_
#define _SiShapeRay_h_Included_

#include "Ge/GeExtents3d.h"
#include "Ge/GeBoundBlock3d.h"
#include "Ge/GeRay3d.h"
#include "Si/SiSpatialIndex.h"

#include "TD_PackPush.h"

/** \details
    Represents ray implementation for OdSiShape interface.

    Corresponding C++ library: SpatialIndex
    <group OdSi_Classes>
*/
struct OdSiShapeRay : public OdSiShape
{
  /** Ray starting point */
  OdGePoint3d m_pointStart;

  /** Ray direction */
  OdGeVector3d m_vecDir;

  /** \details
    Creates a new instance of a shape ray.

    \param pointStart [in] A 3D point from which the ray is casted.
    \param vecDir     [in] The direction of ray.
  */
  OdSiShapeRay( const OdGePoint3d& pointStart, const OdGeVector3d& vecDir )
  {
    m_pointStart = pointStart;
    m_vecDir = vecDir;
  }

  /** \details
    Creates a new instance of a shape ray.

    \param ray [in] Ray.
  */
  OdSiShapeRay( const OdGeRay3d& ray)
  {
    OdGePoint3d startPt;
    ray.hasStartPoint(startPt);
    m_pointStart = startPt;

    m_vecDir = ray.direction();
  }

  /** \details
    Checks if this shape ray object contains the specified extents.

    \param extents [in] Any 3D extents box.
    \param planar  [in] Specifies whether a planar computation should be performed.
    \param tol     [in] Tolerance.

    \returns false.
  */
  bool contains(const OdGeExtents3d& extents, bool planar, const OdGeTol& tol) const {
    return false;
  }

  /** \details
    Checks if this shape ray object intersects the specified extents.

    \param extents [in] Any 3D extents box.
    \param planar  [in] Specifies whether a planar computation should be performed.
    \param tol     [in] Tolerance.

    \returns true if and only if this shape ray object intersects the specfied extents.
  */
  bool intersects(const OdGeExtents3d& extents, bool planar, const OdGeTol& tol) const
  {
    // fast ray box intersection
    // Graphics Gems, 1990, pp. 395-396
    const int sz = 3;
    bool inside = true;

    //-1 - LEFT
    // 0 - MIDDLE
    // 1 - RIGHT
    int quadrant[sz];
    int whichPlane;
    double maxT[sz];
    double candidatePlane[sz];

    OdGeVector3d tolEqPt(tol.equalPoint(), tol.equalPoint(), tol.equalPoint());
    OdGePoint3d minPt = extents.minPoint() - tolEqPt;
    OdGePoint3d maxPt = extents.maxPoint() + tolEqPt;

    for (int i = 0; i != sz; i++)
    {
      if (m_pointStart[i] < minPt[i])
      {
        quadrant[i] = -1;
        candidatePlane[i] = minPt[i];
        inside = false;
      }
      else if (m_pointStart[i] > maxPt[i])
      {
        quadrant[i] = 1;
        candidatePlane[i] = maxPt[i];
        inside = false;
      }
      else
      {
        quadrant[i] = 0;
      }
    }

    OdGePoint3d coord;
    if (inside)
    {
      coord = m_pointStart;
      return true;
    }

    for (int i = 0; i != sz; i++)
    {
      if (quadrant[i] != 0 && m_vecDir[i] != 0.)
        maxT[i] = (candidatePlane[i] - m_pointStart[i]) / m_vecDir[i];
      else
        maxT[i] = -1.;
    }

    whichPlane = 0;
    for (int i = 1; i != sz; i++)
    {
      if (maxT[whichPlane] < maxT[i])
        whichPlane = i;
    }

    if (maxT[whichPlane] < 0.)
    {
      return false;
    }

    for (int i = 0; i != sz; i++)
    {
      if (whichPlane != i)
      {
        coord[i] = m_pointStart[i] + maxT[whichPlane] * m_vecDir[i];
        if (coord[i] < minPt[i] || coord[i] > maxPt[i])
        {
          return false;
        }
      }
      else
      {
        coord[i] = candidatePlane[i];
      }
    }
    return true;
  }

  /** \details
    Clones this shape ray object.

    \returns a pointer to the newly created shape object.
  */
  virtual OdSiShape* clone() const { return new OdSiShapeRay(m_pointStart, m_vecDir); }

  /** \details
    Transforms this shape ray object.

    \param tf [in] Transformation matrix.
  */
  virtual void transform(const OdGeMatrix3d& tf)
  {
    m_pointStart.transformBy(tf);
    m_vecDir.transformBy(tf);
  }
};

#include "TD_PackPop.h"

#endif
