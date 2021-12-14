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

#ifndef OD_GELNSG3D_H
#define OD_GELNSG3D_H /*!DOM*/

class OdGeLineSeg2d;
#include "Ge/GeLinearEnt3d.h"
#include "Ge/GeInterval.h"

#include "TD_PackPush.h"

/** \details
  This class represents line segments in 3D space. 3D line segment can be
  defined with a start and end points or with a point and a non-zero vector that
  specifies segment length and direction.
  By default its start parameter equals to 0.0 and end parameter equals to 1.0.

  Corresponding C++ library: TD_Ge

  <group OdGe_Classes>

  \sa
  <link ge_OdGeLineSeg, Working with Line Segments>
*/
class GE_TOOLKIT_EXPORT OdGeLineSeg3d : public OdGeLinearEnt3d
{
public:

  /** \details
    Default constructor for objects of the OdGeLineSeg3d class. Constructs a
    line segment between the points (0.0, 0.0, 0.0) and (1.0, 0.0, 0.0).
  */
  OdGeLineSeg3d();

  /** \details
    Constructor for objects of the OdGeLineSeg3d class.

    \param source [in]  Object to be copied.
  */
  OdGeLineSeg3d(
    const OdGeLineSeg3d& source);

  /** \details
    Constructor for objects of the OdGeLineSeg3d class.

    \param point [in]  First point of the line segment.
    \param vect  [in]  A 3D vector that defines the line direction and second point.

    \remarks
    Constructs a line segment between the points: point and point + vect.
    The vect vector can have a zero length.
  */
  OdGeLineSeg3d(
    const OdGePoint3d& point,
    const OdGeVector3d& vect);

  /** \details
    Constructor for objects of the OdGeLineSeg3d class.

    \param point1 [in]  Start point of the line segment.
    \param point2 [in]  Second point of the line segment.

    \remarks
    Points point1 and point2 can be coincident. Constructs a line segment
    between the specified points.
  */
  OdGeLineSeg3d(
    const OdGePoint3d& point1,
    const OdGePoint3d& point2);

  /** \details
    Gets the unbounded perpendicular bisecting plane of this line segment.

    \param plane [out]  Receives the bisecting plane.
  */
  void getBisector(OdGePlane& plane) const;

  /** \details
    Returns the weighted average of the start point and end point of this line
    segment.

    \param blendCoeff [in]  Blend coefficient.

    \returns
    An instance of the OdGePoint3d class.

    \remarks
    <table>
    blendCoeff         Returns
    0                  Start point
    1                  End point
    0 to 1             Point on this line segment
    &lt; 0 or &gt; 1   Point not on this line segment, but collinear with it
    </table>
  */
  OdGePoint3d baryComb(double blendCoeff) const;

  /** \details
    Returns the start point of this line segment.

    \returns
    An instance of the OdGePoint3d class.
  */
  OdGePoint3d startPoint() const;

  /** \details
    Returns the end point of this line segment.

    \returns
    An instance of the OdGePoint3d class.
  */
  OdGePoint3d endPoint() const;

  /** \details
    Sets the parameters for this line according to the arguments and returns a
    reference to this line.

    \param point [in]  Start point of the line segment.
    \param vect  [in]  A 3D vector that defines the line direction and second point.

    \returns
    Reference to this OdGeLineSeg3d object.

    \remarks
    Constructs a line segment between the points: point and point + vect.
    The vect vector can have a zero length.
  */
  OdGeLineSeg3d& set(
    const OdGePoint3d& point,
    const OdGeVector3d& vect);

  /** \details
    Sets the parameters for this line according to the arguments and returns a
    reference to this line.

    \param point1 [in]  First point of the line segment.
    \param point2 [in]  Second point of the line segment.

    \returns
    Reference to this OdGeLineSeg3d object.

    \remarks
    Constructs a line segment between points point1 and point2. The points can
    be coincident.
  */
  OdGeLineSeg3d& set(
    const OdGePoint3d& point1,
    const OdGePoint3d& point2);

  /** \details
    Sets the parameters for this line segment according to the arguments and
    returns a reference to this line segment.

    \param curve1   [in]  First 3D curve to which a line segment must be tangent.
    \param curve2   [in]  Second 3D curve to which a line segment must be tangent.
    \param param1   [in]  Point on curve1 where line segment is tangent to curve.
    \param param2   [in]  Point on curve2 where line segment is tangent to curve.
    \param success [out]  Indicates that tangent line was computed successfully.
                          If false, the line segment does not change.

    \returns
    Reference to this OdGeLineSeg3d object.
  */
  OdGeLineSeg3d& set(
    const OdGeCurve3d& curve1,
    const OdGeCurve3d& curve2,
    double& param1, double& param2,
    bool& success);

  /** \details
    Sets the parameters for this line segment according to the arguments and
    returns a reference to this line segment.

    \param curve    [in]  A curve to which a line segment will be tangent.
    \param point    [in]  A 3D point through which a line segment passes.
    \param param    [in]  A point on curve where line segment is tangent to curve.
    \param success [out]  Indicates that tangent line was computed successfully.
                          If false, the line segment does not change.


    \returns
    Reference to this OdGeLineSeg3d object.

    \remarks
    The point must lie on the plane of the curve and the curve must be planar.
  */
  OdGeLineSeg3d& set(
    const OdGeCurve3d& curve,
    const OdGePoint3d& point,
    double& param,
    bool& success);

  /** \details
    Assignment operator for the OdGeLineSeg3d object.

    \param line [in]  Linear segment to assign.
    \returns
    A reference to this OdGeLinearEnt3d object.
  */ 
  OdGeLineSeg3d& operator =(
    const OdGeLineSeg3d& line);

  /** \details
    Returns the distance to a vector.

    \param point [in]  A 3D point from which the distance is calculated.
    \param tol   [in]  Tolerance.
  */
  double getDistanceToVector(const OdGePoint3d& point, const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Attaches to the end of itself another curve.

    \param curve      [in]  Link to the join *curve*.
    \param iTolerance [in]  Geometric tolerance.

    \remarks
    The end of the current *curve* should coincide with the beginning of the
    next *curve*.
  */
  OdGeLineSeg3d& joinWith(
    const OdGeLineSeg3d& curve, const OdGeTol &iTolerance = OdGeContext::gTol);
};

#include "TD_PackPop.h"

#endif
