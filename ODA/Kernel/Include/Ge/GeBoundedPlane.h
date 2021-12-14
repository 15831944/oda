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

#ifndef OD_GEBNDPLN_H
#define OD_GEBNDPLN_H  /*!DOM*/

#include "Ge/GePlanarEnt.h"
#include "Ge/GePlane.h"
#include "Ge/GeVector3d.h"

class OdGePlane;
class OdGeVector3d;
class OdGePoint3d;
class OdGePoint2d;
class OdGeLineSeg3d;

#include "TD_PackPush.h"

/** \details
    This class represents bounded planes in 3D space.

    Corresponding C++ library: TD_Ge

    <group OdGe_Classes> 

    \sa
    <link ge_OdGeBoundedPlane, Working with Bounded Planes>
    \sa
    <link OdGePlanarEnt, OdGePlanarEnt Class>
*/
class GE_TOOLKIT_EXPORT OdGeBoundedPlane : public OdGePlanarEnt
{
public:

  /** \details 
  The default constructor for the OdGeBoundedPlane class. Constructs an infinite plane coinciding with the XY plane.
  */
  OdGeBoundedPlane();
  
  /** 
    \details Constructor for the OdGeBoundedPlane class.
    \param plane [in]  A plane to be cloned.

    \remarks
    Copies the input OdGeBoundedPlane object.
  */
  OdGeBoundedPlane(const OdGeBoundedPlane& plane);
  
  /** 
    \details Constructor for the OdGeBoundedPlane class.
    \param origin [in]  Origin of the bounded plane. 
    \param uAxis [in]  U-axis.
    \param vAxis [in]  V-axis.

    \remarks
    Constructs a plane with the specified parameters.
  */
  OdGeBoundedPlane(
    const OdGePoint3d& origin,
    const OdGeVector3d& uAxis,
    const OdGeVector3d& vAxis);
  
  /** 
    \details Constructor for the OdGeBoundedPlane class.
    \param uPnt [in]  A point at the end of the U-axis.
    \param origin [in]  Origin of the bounded plane.  
    \param vPnt [in]  A point at the end of the V-axis.

    \remarks
    Constructs a plane with the specified parameters.
  */
  OdGeBoundedPlane(
    const OdGePoint3d& uPnt,
    const OdGePoint3d& origin,
    const OdGePoint3d& vPnt);

  TD_USING(OdGePlanarEnt::intersectWith);
   
   /** 
    \details Checks for an intersection with a plane.
    \param plane    [in]  Input plane that is tested for an intersection. 
    \param intLine  [out]  A computed line segment of an intersection.
    \param tol      [in]  Geometric tolerance.
    
    \returns
    true if this plane intersects with an input plane, otherwise it returns false.
  */
   bool intersectWith(
    const OdGePlane& plane, 
    OdGeLineSeg3d& intLine,
    const OdGeTol& tol = OdGeContext::gTol) const;
   
   /** 
    \details Checks for an intersection with a plane.
    \param plane    [in]  Input plane that is tested for an intersection. 
    \param intLine  [out]  A computed line segment of an intersection.
    \param tol      [in]  Geometric tolerance.
    
    \returns
    Returns a bool value that indicates if an intersection occurred.
    \remarks
    If this plane intersects with an input plane, the method returns true, otherwise it returns false.
  */
   bool intersectWith(
    const OdGeBoundedPlane& plane, 
    OdGeLineSeg3d& intLine,
    const OdGeTol& tol = OdGeContext::gTol) const;
  
  /** \details
    Sets parameters for this plane according to the arguments, and returns a reference to this plane.

    \param origin [in]  The origin of the bounded plane.
    \param uAxis [in]  The U-axis of the plane.
    \param vAxis [in]  The V-axis of the plane.
   
    \returns
    Reference to this plane.
  */
   OdGeBoundedPlane& set(
    const OdGePoint3d& origin, 
    const OdGeVector3d& uAxis, 
    const OdGeVector3d& vAxis);
    
  /** \details
    Sets parameters for this plane according to the arguments, and returns a reference to this plane.

    \param origin [in]  The origin of the bounded plane.
    \param uPnt [in]  A point at the end of the U-axis.
    \param vPnt [in]  A point at the end of the V-axis.
   
    \returns
    Reference to this plane.
  */
   OdGeBoundedPlane& set(
    const OdGePoint3d& uPnt,
    const OdGePoint3d& origin,
    const OdGePoint3d& vPnt);

  /** \details
    The assignment operator for the OdGeBoundedPlane class.

    \param plane [in]  Input OdGeBoundedPlane object.

    \returns
    A reference to this OdGeBoundedPlane object.
  */
  OdGeBoundedPlane& operator =(const OdGeBoundedPlane& plane);
};

#include "TD_PackPop.h"

#endif // OD_GEBNDPLN_H

