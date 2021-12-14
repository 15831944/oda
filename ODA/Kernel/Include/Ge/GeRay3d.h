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

#ifndef OD_GERAY3D_H
#define OD_GERAY3D_H /*!DOM*/

#include "Ge/GeLinearEnt3d.h"

#include "TD_PackPush.h"

/** \details
    This class represents semi-infinite lines in 3D space.

    Corresponding C++ library: TD_Ge

    <group OdGe_Classes>

    \sa
    <link ge_OdGeRay, Working with Rays>
*/
class GE_TOOLKIT_EXPORT OdGeRay3d : public OdGeLinearEnt3d
{
public:
  
  /** \details
    Default constructor for the OdGeRay3d class. Constructs a ray from the origin oriented along the X-axis.
  */
  OdGeRay3d();
  
  /** \details
    Copy constructor for the OdGeRay3d class.
    
    \param line [in]  Object to be copied.
  */
  OdGeRay3d(
    const OdGeRay3d& line);
  
  /** \details
    Constructor for the OdGeRay3d class.
    
    \param point [in]  A 3D point from which the ray is casted.
    \param vect [in]  The direction of ray.

    \remarks
    The constructor constructs a ray that is casted from the specified point to a specified direction.
  */
  OdGeRay3d(
    const OdGePoint3d& point, 
    const OdGeVector3d& vect);
  
  /** \details
    Constructor for the OdGeRay3d class.
    
    \param point1 [in]  A 3D point from which the ray is casted.
    \param point2 [in]  A 3D point through which the ray passes.

    \remarks
    The constructor constructs a ray that is casted from the specified point, the ray passes through the point2.
  */
  OdGeRay3d(const OdGePoint3d& point1, 
    const OdGePoint3d& point2);

  /** \details
    Sets the parameters for this ray according to the arguments.

    \param point [in]  A 3D point from which the ray is casted.
    \param vect [in]  The direction of ray.

    \returns
    Reference to this ray.
    
    \remarks
    point and vect construct a semi-infinite line starting point with 
    a direction of vect. vect cannot have a zero length.
  */
   OdGeRay3d& set(
    const OdGePoint3d& point, 
    const OdGeVector3d& vect);

  /** \details
    Sets the parameters for this ray according to the arguments.

    \param point1 [in]  A 3D point from which the ray is casted.
    \param point2 [in]  A 3D point through which the ray passes.

    \returns
    Reference to this ray.
    
    \remarks
    point1 and point2 construct a semi-infinite line starting at point1, and passing through point2. The 
    points cannot be coincident.
  */
    OdGeRay3d& set(
    const OdGePoint3d& point1, 
    const OdGePoint3d& point2);

  /** \details
    Assignment operator for the OdGeRay3d object.

    \param line [in]  Ray to assign.
    
    \returns
    Reference to this OdGeRay3d object.
  */
  OdGeRay3d& operator =(
    const OdGeRay3d& line);
};

#include "TD_PackPop.h"

#endif

