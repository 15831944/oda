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



#ifndef OD_GESPUNSURF_H
#define OD_GESPUNSURF_H /*!DOM*/

//
/** \details
*/

#include "Ge/GeSurface.h"
#include "Ge/GeInterval.h"
#include "OdPlatformSettings.h"

class OdGePoint3d;
class OdGeVector3d;
class OdGeCircArc3d;
class OdGeLinearEnt3d;

#include "TD_PackPush.h"

/** \details
    A spun surface has a parametric representation of the form:
    R( u, v ) = Z( u ) + ( C( u ) - Z( u ))cos( v ) + A X ( C( u ) - Z( u ) ) sin( v)
    where
    * C(u) is the profile curve
    * Z(u) is the projection of C(u) onto the spin axis
    * A is the spin axis direction (unit vector)
    * C must not be an intersection curve or a trimmed curve
    NOTE: Z(u) = P + ( ( C( u ) - P ) . A )A where P is a reference point on the axis.
    
    For internal use only.
    
    Corresponding C++ library: TD_Ge
    
    <group OdGe_Classes> 
*/
class GE_TOOLKIT_EXPORT OdGeSpunSurf : public OdGeSurface
{
public:

  /** \details 
    Default constructor for the OdGeSpunSurf class. Constructs an invalid spun surface.
  */
  OdGeSpunSurf();
  
  /** \details 
    Constructor for the OdGeSpunSurf class.
    \param pProfile [in]  Profile curve to be spun.
    \param pBase [in]  Rotational axis origin.
    \param pAxis [in]  Axis around which the surface is spun.
    
    \remarks
    Constructs a spun surface with the specified parameters, a start angle of rotation = 0,
    an end angle of rotation = Oda2PI, and refVector = (0,0,0).
  */
  OdGeSpunSurf(
  const OdGeCurve3d &pProfile, 
  const OdGePoint3d &pBase, 
  const OdGeVector3d &pAxis);

  /** \details 
    Copy constructor for the OdGeSpunSurf class.

    \param spunSurf [in]  Spun surface to be copied.
  */
  OdGeSpunSurf(
    const OdGeSpunSurf& spunSurf);

  /** \details
    Assignment operator for the OdGeSpunSurf object.
    
    \param spunSurface [in]  Spun surface to assign.
    
    \returns
    A reference to this spun surface.
  */
  OdGeSpunSurf& operator = (
    const OdGeSpunSurf& spunSurface);

  /** \details
    Returns a pointer to the profile curve that is spun to create this spun surface.
  */
  const OdGeCurve3d *getProfile() const;

  /** \details
    Returns a point that represents a rotational axis origin.
  */
  OdGePoint3d getBasePoint() const;
  
  /** \details
    Returns a vector that represents an axis around which the profile curve is spun to create this spun surface.
  */
  OdGeVector3d getAxis() const;
  
  /** \details
    Returns a vector that indicates the direction of the zero rotation angle of a profile curve.
  */
  OdGeVector3d getRef() const;
  
  /** \details
    Sets angles of rotation of the profile curve of this spun surface.
    
    \param startAngle [in]  Start angle of rotation. To create a closed spun surface, set this parameter to 0 and endAngle to Oda2PI.
    \param endAngle [in]  End angle of rotation. To create a closed spun surface, set this parameter to Oda2PI and startAngle to 0.
  */
  OdGeSpunSurf& setAngles(double startAngle, double endAngle);
  
  /** \details
    Sets a vector that indicates the direction of the zero rotation angle of a profile curve.
    
    \param pRef [in]  A vector that specifies the direction of the zero rotation angle of a profile curve.
  */
  OdGeSpunSurf& setRef(const OdGeVector3d &pRef);

  /** \details
    Sets the given parameters to this spun surface.
    
    \param pProfile [in]  Profile curve to be spun.
    \param pBase [in]  Rotational axis origin.
    \param pAxis [in]  Axis around which the surface is spun.
  */
  void set(const OdGeCurve3d &pProfile, const OdGePoint3d &pBase, const OdGeVector3d &pAxis);

};

#include "TD_PackPop.h"

#endif // OD_GESPUNSURF_H

