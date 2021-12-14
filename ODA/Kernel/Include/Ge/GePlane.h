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

#ifndef OD_GEPLANE_H
#define OD_GEPLANE_H /*!DOM*/

#include "Ge/GePlanarEnt.h"
#include "TD_PackPush.h"

class OdGeBoundedPlane;
class OdGeLine3d;
class OdGeLineSeg3d;

/** \details
    This class represents infinite planes in 3D space.

    Corresponding C++ library: TD_Ge

    <group OdGe_Classes> 

    \sa
    <link ge_OdGePlane, Working with Planes>
    \sa
    <link OdGePlanarEnt, OdGePlanarEnt Class>
*/
class GE_TOOLKIT_EXPORT OdGePlane : public OdGePlanarEnt
{
public:


  GE_STATIC_EXPORT static const OdGePlane kXYPlane; // XY *plane*.
  GE_STATIC_EXPORT static const OdGePlane kYZPlane; // YZ *plane*.
  GE_STATIC_EXPORT static const OdGePlane kZXPlane; // ZY *plane*.
  
  /** \details
    Default constructor for the OdGePlane class. Constructs an infinite plane coincident with the XY plane.
  */
  OdGePlane();
  
  /** \details
    Copy constructor for the OdGePlane class.
    
    \param plane [in]  Plane to be copied.
    
    \remarks
    Currently not implemented.
  */
  OdGePlane(
    const OdGePlane& plane);
  
  /** \details
    Constructor for the OdGePlane class.
    
    \param origin [in]   Origin of plane.
    \param normal [in]   The normal to the plane.
    
    \remarks
    
    Constructs the plane with the specified origin, uAxis= normal.perpVector(), and vAxis=normal.crossProduct(uAxis).
  */
  OdGePlane(
    const OdGePoint3d& origin, 
    const OdGeVector3d& normal);
  
  /** \details
    Constructor for the OdGePlane class.
    
    \param uPnt [in]  A point at the end of the U-axis.
    \param origin [in]   Origin of plane.
    \param vPnt [in]  A point at the end of the V-axis.
    
    \remarks
    
    Constructs the plane with origin=or, uAxis=v1, and vAxis=v2. 
    Vectors v1=pntU-or and v2=pntV-or are non-collinear. 
  */
  OdGePlane(
    const OdGePoint3d& uPnt, 
    const OdGePoint3d& origin, 
    const OdGePoint3d& vPnt);
  
  /** \details
    Constructor for the OdGePlane class.
    
    \param origin [in]   Origin of plane.
    \param uAxis [in]  The U-axis.
    \param vAxis [in]  The V-axis.
    
    \remarks
    
    Constructs the plane with origin, U axis uAxis, and V axis vAxis. 
    Vectors uAxis and vAxis are non-collinear.
  */
  OdGePlane(
    const OdGePoint3d& origin, 
    const OdGeVector3d& uAxis, 
    const OdGeVector3d& vAxis);
  
  /** \details
    Constructor for the OdGePlane class.
    
    \param a [in]  Coefficient a.
    \param b [in]  Coefficient b.
    \param c [in]  Coefficient c.
    \param d [in]  Coefficient d.
  */
  OdGePlane(
    double a, 
    double b, 
    double c, 
    double d);

  //DOM-IGNORE-BEGIN
  TD_USING(OdGePlanarEnt::intersectWith);
  //DOM-IGNORE-END
   
   /** \details
    Returns true and the intersection line, if and only 
    if the specified plane intersects with this plane.

    \param plane [in]  Any plane with which the intersection is computed.
    \param intLine [out]  Receives the intersection line. 
    \param tol [in]  Geometric tolerance.
  */
   bool intersectWith(
    const OdGePlane& plane, 
    OdGeLine3d& intLine,
    const OdGeTol& tol = OdGeContext::gTol) const;
   
   /** \details
    Returns true and the intersection line segment, if and only 
    if the specified bounded plane intersects with this plane.

    \param plane [in]  Any bounded plane with which the intersection is computed.
    \param intLine [out]  Receives the intersection line segment. 
    \param tol [in]  Geometric tolerance.
  */
   bool intersectWith(
    const OdGeBoundedPlane& plane, 
    OdGeLineSeg3d& intLine,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns the signed distance to (elevation of) the specified point.

    \param point [in]  Any 3D point.
    
    \remarks
    Returns the signed distance from the specified point to this plane. The sign of the result is the same as the sign of the dot product of the vector: point - originOfPlane and the normal to the plane.
  */
   double signedDistanceTo(
    const OdGePoint3d& point) const;

   /** \details
    Sets the parameters for this plane according to the arguments. 

    \param point [in]   Plane origin.
    \param normal [in]  The normal to the plane.
  
    \returns
    Reference to this plane.
   */
   OdGePlane& set(
    const OdGePoint3d& point, 
    const OdGeVector3d& normal);
 
   /** \details
    Sets the parameters for this plane according to the arguments. 

    \param origin [in]   Origin of plane.
    \param uPnt [in]  A point at the end of the U-axis.
    \param vPnt [in]  A point at the end of the V-axis.
    
    \returns
    Reference to this plane. 
   */
  OdGePlane& set(
    const OdGePoint3d& uPnt, 
    const OdGePoint3d& origin, 
    const OdGePoint3d& vPnt);

   /** \details
    Sets the parameters for this plane according to the arguments. 

    \param a [in]  Coefficient a.
    \param b [in]  Coefficient b.
    \param c [in]  Coefficient c.
    \param d [in]  Coefficient d.

    \returns
    Reference to this plane.
   */
  OdGePlane& set(
    double a, 
    double b, 
    double c, 
    double d);

   /** \details
    Sets the parameters for this plane according to the arguments. 

    \param origin [in]   Origin of plane.
    \param uAxis [in]  The U-axis.
    \param vAxis [in]  The V-axis.

    \returns
    Reference to this plane. 
   */
  OdGePlane& set(
    const OdGePoint3d& origin, 
    const OdGeVector3d& uAxis, 
    const OdGeVector3d& vAxis);

  /** \details
    Assignment operator for the OdGePlane object.
    
    \param plane [in]  Plane to assign.
    
    \returns
    Reference to this OdGePlane object.
  */
  OdGePlane& operator =(
    const OdGePlane& plane);
//DOM-IGNORE-BEGIN
private:
  OdGePlane(OdGeEntity3dImpl*);
//DOM-IGNORE-END
};

#include "TD_PackPop.h"

#endif // OD_GEPLANE_H

