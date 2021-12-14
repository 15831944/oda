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



#ifndef OD_GERULED_H
#define OD_GERULED_H /*!DOM*/

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
    If both curves are valid, the parametric equation for this surface is :
    <pre>
    P(u, v) = C1(u) + v(C2(u) - C1(u))
    </pre>
    If one of the curves is replaced with a point, the equations simplify to one of :
    
    <pre>
    P(u, v) = P1 + v(C2(u) - P1)
    P(u, v) = C1(u) + v(P2 - C1(u))
    </pre>
    A ruled face with no curves and two points is degenerate and will not be returned.
    
    For internal use only.

    Library: TD_Ge

   <group OdGe_Classes> 
*/
class GE_TOOLKIT_EXPORT OdGeRuled : public OdGeSurface
{
public:

  /** \details
    Default constructor for the OdGeRuled class. Constructs an invalid ruled surface.
  */
  OdGeRuled();
  
  /** \details
    Constructor for the OdGeRuled class.
    
    \param pProfileCurve1 [in]  First profile curve that is used to construct the ruled surface. 
    \param pProfileCurve2 [in]  Second profile curve that is used to construct the ruled surface. 

    \remarks
    The constructor constructs a ruled surface by sweeping a line between 2 specified profile curves.
  */
  OdGeRuled(
    const OdGeCurve3d &pProfileCurve1,
    const OdGeCurve3d &pProfileCurve2);
  
  /** \details
    Constructor for the OdGeRuled class.
    
    \param pProfileCurve1 [in]  A profile curve that is used to construct the ruled surface. 
    \param pPoint2 [in]  A point that is used to construct the ruled surface. 

    \remarks
    The constructor constructs a ruled surface by sweeping a line between specified profile curve and point.
  */
  OdGeRuled(
    const OdGeCurve3d &pProfileCurve1,
    const OdGePoint3d &pPoint2);
  
  /** \details
    Constructor for the OdGeRuled class.
    
    \param pPoint1 [in]  A point that is used to construct the ruled surface.
    \param pProfileCurve2 [in]  A profile curve that is used to construct the ruled surface.

    \remarks
    The constructor constructs a ruled surface by sweeping a line between specified profile curve and point.
  */
  OdGeRuled(
    const OdGePoint3d &pPoint1,
    const OdGeCurve3d &pProfileCurve2);
  
  /** \details
    Copy constructor for the OdGeRuled class.

    \param ruled [in]  Ruled surface to copy.
  */
  OdGeRuled(
    const OdGeRuled& ruled);

  /** \details
    Sets the data to create a ruled surface.

    \param pProfileCurve1 [in]  First profile curve that is used to construct the ruled surface. 
    \param pProfileCurve2 [in]  Second profile curve that is used to construct the ruled surface. 
  */
  void set(const OdGeCurve3d& pProfileCurve1, const OdGeCurve3d& pProfileCurve2);

  /** \details
    Sets the data to create a ruled surface.

    \param pProfileCurve1 [in]  A profile curve that is used to construct the ruled surface. 
    \param pPoint2 [in]  A point that is used to construct the ruled surface. 
  */
  void set(const OdGeCurve3d& pProfileCurve1, const OdGePoint3d& pPoint2);

  /** \details
    Sets data to create a surface.

    \param pPoint1 [in]  A point that is used to construct the ruled surface.
    \param pProfileCurve2 [in]  A profile curve that is used to construct the ruled surface.

  */
  void set(const OdGePoint3d &pPoint1, const OdGeCurve3d& pProfileCurve2);

  /** \details
    Assignment operator for the OdGeRuled object.

    \param extSurf [in]  OdGeRuled object to assign.
    
    \returns
    Reference to this OdGeRevolvedSurface object.
  */
  OdGeRuled& operator = (
    const OdGeRuled& extSurf);

  /** \details
    Returns the profile curve.

    \param iIndex [in] Input index that specifies which curve should be retrieved.
    \param pProfileCurve [out] Output profile curve.

    \returns
    eOk if successful or an appropriate error code in the other case.
  */
  OdResult getProfileCurve(const OdUInt8 iIndex, OdGeCurve3d*& pProfileCurve) const;

  /** \details
    Returns the point.

    \param iIndex [in] Input index that specifies which point should be retrieved.
    \param point [out] Output point.

    \returns
    eOk if successful or an appropriate error code in the other case.
  */
  OdResult getPoint(const OdUInt8 iIndex, OdGePoint3d& point) const;

  /** \details
    Sets the envelope of GeRuled.

    \param intrvlU [in] u-interval.
    \param intrvlV [in] v-interval.
  */
  bool setEnvelope(const OdGeInterval& intrvlU, const OdGeInterval& intrvlV);

  /** \details
    Gets the envelope of GeRuled.

    \param intrvlU [out] u-interval.
    \param intrvlV [out] v-interval.
  */
  void getEnvelope(OdGeInterval& intrvlU, OdGeInterval& intrvlV) const;

  /** \details
    Checks if a point was used to define the first profile.
  */
  bool hasFirstProfilePoint() const;

  /** \details
    Checks if a point was used to define the second profile.
  */
  bool hasSecondProfilePoint() const;
};

#include "TD_PackPop.h"

#endif // OD_GERULED_H

