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

#ifndef OD_GE_RAY_2D_H
#define OD_GE_RAY_2D_H /*!DOM*/

#include "Ge/GeLinearEnt2d.h"

#include "TD_PackPush.h"

/** \details
    This class represents semi-infinite lines in 2D space.

    Corresponding C++ library: TD_Ge

    <group OdGe_Classes>

    \sa
    <link ge_OdGeRay, Working with Rays>
*/
class GE_TOOLKIT_EXPORT OdGeRay2d : public OdGeLinearEnt2d
{
public:
    
  /** \details
    Default constructor for the OdGeRay2d class. Constructs a ray from the origin oriented along the X-axis.
  */
  OdGeRay2d();
  
  /** \details
    Copy constructor for the OdGeRay2d class.
    
    \param ray [in]  Object to be copied.
  */
  OdGeRay2d(const OdGeRay2d& ray);
  
  /** \details
    Constructor for the OdGeRay2d class.
    
    \param point [in]  A 2D point from which the ray is casted.
    \param vect [in]  The direction of ray.

    \remarks
    The constructor constructs a ray that is casted from the specified point to a specified direction.
  */
  OdGeRay2d(
    const OdGePoint2d& point, 
    const OdGeVector2d& vect);
  
  /** \details
    Constructor for the OdGeRay2d class.
    
    \param point1 [in]  A 2D point from which the ray is casted.
    \param point2 [in]  A 2D point through which the ray passes.

    \remarks
    The constructor constructs a ray that is casted from the specified point, the ray passes through the point2.
  */
  OdGeRay2d(
    const OdGePoint2d& point1, 
    const OdGePoint2d& point2);
  
  /** \details
    Sets the parameters for this ray according to the arguments.

    \param point [in]  A 2D point from which the ray is casted.
    \param vect [in]  The direction of ray.

    \returns
    Reference to this ray.
    
    \remarks
    point and vect construct a semi-infinite line starting point with 
    a direction of vect. vect cannot have a zero length.
  */
  OdGeRay2d& set(
    const OdGePoint2d& point, 
    const OdGeVector2d& vect);
  
  /** \details
    Sets the parameters for this ray according to the arguments.

    \param point1 [in]  A 2D point from which the ray is casted.
    \param point2 [in]  A 2D point through which the ray passes.

    \returns
    Reference to this ray.
    
    \remarks
    point1 and point2 construct a semi-infinite line starting at point1, and passing through point2. The 
    points cannot be coincident.
  */
  OdGeRay2d& set(
    const OdGePoint2d& point1, 
    const OdGePoint2d& point2);

  /** \details
    Assignment operator for the OdGeRay2d object.

    \param ray [in]  Ray to assign.
    
    \returns
    Reference to this OdGeRay2d object.
  */
  OdGeRay2d& operator =(const OdGeRay2d& ray);

};

#include "TD_PackPop.h"

#endif // OD_GE_RAY_2D_H
