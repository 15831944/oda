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



#ifndef OD_GEREVOLVEDSURFACE_H
#define OD_GEREVOLVEDSURFACE_H /*!DOM*/

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
    A revolved surface has a parametric representation of the form:
    retPnt(u, v) = P + C(u).x * (XVec * cos(v) + YVec * sin(v)) + C(u).z * A
    where
    * C(u) is the profile curve. Curve controls only 2 parameters - the height and the distance from the axis
    * P is the origin of the surface
    * A is the axis of the surface
    * XVec, YVec are the unit vectors for the rotated curve.
    
    For internal use only.
    
    Library: TD_Ge
    
    <group OdGe_Classes> 
*/
class GE_TOOLKIT_EXPORT OdGeRevolvedSurface : public OdGeSurface
{
public:

  /** \details
    Default constructor for the OdGeRevolvedSurface class. Constructs an invalid revolved surface.
  */
  OdGeRevolvedSurface();

  /** \details
    Constructor for the OdGeRevolvedSurface class.
    
    \param pProfile [in]  A pointer to the profile curve that is revolved.
    \param pBase [in]  Origin point that along with pAxis form the axis of the revolved surface.
    \param pAxis [in]  Axis of rotation of the profile curve.
    \param pRef [in]  The unit vector for the rotated curve (incorrectly called "Radius"). Indicates the direction of the zero rotation angle.
    \param startAngle [in]  Start angle of rotation.
    \param endAngle [in]  End angle of rotation.
    \remarks
    The constructor constructs a revolved surface with specified parameters.
  */
  OdGeRevolvedSurface(
    const OdGeCurve3d &pProfile,
    const OdGePoint3d &pBase,
    const OdGeVector3d &pAxis,
    const OdGeVector3d &pRef,
    double startAngle = 0., double endAngle = Oda2PI);

  /** \details
    Copy constructor for the OdGeRevolvedSurface class.

    \param revolvedSurf [in]  Revolved surface to be copied.
  */
  OdGeRevolvedSurface(
    const OdGeRevolvedSurface& revolvedSurf);

  /** \details
    Assignment operator for the OdGeRevolvedSurface object.

    \param revolvedSurf [in]  OdGeRevolvedSurface object to assign.
    
    \returns
    Reference to this OdGeRevolvedSurface object.
  */
  OdGeRevolvedSurface& operator = (
    const OdGeRevolvedSurface& revolvedSurf);

  /** \details
    Returns a reference to the profile curve of this revolved surface.
  */
  const OdGeCurve3d& getProfile() const;

  /** \details
    Returns an origin point of this revolved surface.
  */
  OdGePoint3d getBasePoint() const;
  
  /** \details
    Returns axis of rotating around which the profile curve is revolved.
  */
  OdGeVector3d getAxis() const;
  
  /** \details
    Returns the vector that indicates the direction of the zero rotation angle.
  */
  OdGeVector3d getRef() const;
  
  /** \details
    Returns start and end angles of rotation of the profile curve.
    
    \param startAngle [out]  Start angle of rotation of this revolved surface.
    \param endAngle [out]  End angle of rotation of this revolved surface.
    
    \remarks
    The angles are specified in radians.
  */
  void getAngles(double &startAngle, double &endAngle) const;
  
  /** \details
    Sets start and end angles of rotation of the profile curve.
    
    \param startAngle [in]  Start angle of rotation of this revolved surface.
    \param endAngle [in]  End angle of rotation of this revolved surface.
    
    \remarks
    The angles are specified in radians.
  */
  OdGeRevolvedSurface& setAngles(double startAngle, double endAngle);
  
  /** \details
    Sets vector that indicates the direction of the zero rotation angle of a profile curve.
    
    \param pRef [in]  The unit vector that indicates the direction of the zero rotation angle of a profile curve.
    
    \returns
    Reference to the resulting revolved surface.
  */
  OdGeRevolvedSurface& setRef(const OdGeVector3d &pRef);

  /** \details
    Sets parameters for this revolved surface.
    
    \param pProfile [in]  A pointer to the profile curve that is revolved.
    \param pBase [in]  Origin point of the revolved surface.
    \param pAxis [in]  Axis of rotation of the profile curve.
    \param pRef [in]  The unit vector for the rotated curve (incorrectly called “Radius”). Indicates the direction of the zero rotation angle of a profile curve.
    \param startAngle [in]  Start angle of rotation.
    \param endAngle [in]  End angle of rotation.
    
    \remarks
    The angles are specified in radians.
  */
  void set(const OdGeCurve3d &pProfile, const OdGePoint3d &pBase, const OdGeVector3d &pAxis, const OdGeVector3d &pRef);

};

#include "TD_PackPop.h"

#endif // OD_GERevolvedSurface_H

