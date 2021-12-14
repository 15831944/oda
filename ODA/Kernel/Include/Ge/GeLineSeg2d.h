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

#ifndef OD_GE_LINE_SEG_2D_H
#define OD_GE_LINE_SEG_2D_H /*!DOM*/

#include "Ge/GeLinearEnt2d.h"

#include "TD_PackPush.h"

/** \details
  This class represents line segments in 2D space. 2D line segment can be
  defined with a start and end points or with a point and a non-zero vector that
  specifies segment length and direction. By default its start parameter equals
  to 0.0 and end parameter equals to 1.0.

  Corresponding C++ library: TD_Ge

  <group OdGe_Classes>

  \sa
  <link ge_OdGeLineSeg, Working with Line Segments>
*/
class GE_TOOLKIT_EXPORT OdGeLineSeg2d : public OdGeLinearEnt2d
{
public:

  /** \details
    Default constructor for objects of the OdGeLineSeg2d class. Constructs a
    line segment between the points (0.0, 0.0) and (1.0, 0.0).
  */
  OdGeLineSeg2d();

  /** \details
    Copy constructor for objects of the OdGeLineSeg2d class.

    \param source [in]  A 2D line segment to be copied.
  */
  OdGeLineSeg2d(
    const OdGeLineSeg2d& source);

  /** \details
    Constructor for objects of the OdGeLineSeg2d class.

    \param point1 [in]  First 2D point that represents the start of the segment.
    \param point2 [in]  Second 2D point that represents the end of the segment.
  */
  OdGeLineSeg2d(
    const OdGePoint2d& point1,
    const OdGePoint2d& point2);

  /** \details
    Constructor for objects of the OdGeLineSeg2d class.

    \param point [in]  A 2D point that represents the start of the segment.
    \param vect  [in]  Defines the direction of the line and the second point of the line.
  */
  OdGeLineSeg2d(
    const OdGePoint2d& point,
    const OdGeVector2d& vect);

  /** \details
    Sets the parameters for this line according to the arguments and returns a
    reference to this line.

    \param point [in]  A 2D point that represents the start of the segment.
    \param vect  [in]  Defines the direction of the line and the second point of the line.

    \returns
    Reference to this OdGeLineSeg2d object.

    \remarks
    Constructs a line segment between points point and point + vect. The vect
    parameter cannot have a zero length.
  */
  OdGeLineSeg2d& set(
    const OdGePoint2d& point,
    const OdGeVector2d& vect);

  /** \details
    Sets the parameters for this line according to the arguments and returns a
    reference to this line.

    \param point1 [in]  First 2D point that represents the start of the segment.
    \param point2 [in]  Second 2D point that represents the end of the segment.

    \returns
    Reference to this OdGeLineSeg2d object.

    \remarks
    Constructs a line segment between points point1 and point2. The points
    cannot coincide.
  */
  OdGeLineSeg2d& set(
    const OdGePoint2d& point1,
    const OdGePoint2d& point2);

  /** \details
    Sets the parameters for this line according to the arguments and returns a
    reference to this line.

    \param curve1   [in]  First 2D curve to which a constructed line segment must be tangent.
    \param curve2   [in]  Second 2D curve to which a constructed line segment must be tangent.
    \param param1   [in]  Point on curve1 where this line segment is tangent to curve1.
    \param param2   [in]  Point on curve2 where this line segment is tangent to curve2.
    \param success [out]  Receives true if and only if the tangent was constructed.
                          If false, this line segment is unchanged.

    \returns
    Reference to this OdGeLineSeg2d object.

    \remarks
    Constructs a tangent line segment between curve1 and curve2. Points param1
    and param2 are the approximate tangent points on curve1 and curve2
    respectively.
  */
  OdGeLineSeg2d& set(
    const OdGeCurve2d& curve1,
    const OdGeCurve2d& curve2,
    double& param1,
    double& param2,
    bool& success);

  /** \details
    Sets the parameters for this line according to the arguments and returns a
    reference to this line.

    \param point    [in]  2D point at which the constructed line segment is tangent to the specified curve.
    \param curve    [in]  2D curve to which the constructed line segment must be tangent.
    \param param    [in]  Parameter of the point on curve where this line segment is tangent to the curve.
    \param success [out]  Receives true if and only if the tangent was constructed.
                          If false, this line segment is unchanged.

    \returns
    Reference to this OdGeLineSeg2d object.

    \remarks
    Constructs a line segment starting at point, and tangent to curve.
    The param value is the approximate tangent point on curve.
  */
  OdGeLineSeg2d& set(
    const OdGeCurve2d& curve,
    const OdGePoint2d& point,
    double& param,
    bool& success);

  /** \details
    Gets the infinite perpendicular bisector of this line segment.

    \param line [out]  Receives the perpendicular bisector.
  */
  void getBisector(
    OdGeLine2d& line) const;

  /** \details
    Returns the weighted average of the start point and end point of this line
    segment.

    \param blendCoeff [in]  Blend coefficient.

    \returns
    An instance of the OdGePoint2d class.

    \remarks
    <table>
    blendCoeff         Returns
    0                  Start point
    1                  End point
    0 to 1             Point on this line segment
    &lt; 0 or &gt; 1   Point not on this line segment, but collinear with it
    </table>
  */
  OdGePoint2d baryComb(
    double blendCoeff) const;

  /** \details
    Returns the start point of this line segment.

    \returns
    An instance of the OdGePoint2d class.
  */
  OdGePoint2d startPoint() const;

  /** \details
    Returns the end point of this line segment.

    \returns
    An instance of the OdGePoint2d class.
  */
  OdGePoint2d endPoint() const;

  /** \details
    Assignment operator for the OdGeLinearEnt2d object.

    \param line [in]  Linear segment to assign.
    
    \returns
    A reference to this OdGeLinearEnt2d object.
  */
  OdGeLineSeg2d& operator =(const OdGeLineSeg2d& line);
};

#include "TD_PackPop.h"

#endif // OD_GE_LINE_SEG_2D_H
