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

#ifndef OD_GELINE3D_H
#define OD_GELINE3D_H /*!DOM*/

class OdGeLine2d;
#include "Ge/GeLinearEnt3d.h"

#include "TD_PackPush.h"

/** \details
    This class represents infinite lines in 3D space.

    Corresponding C++ library: TD_Ge

    <group OdGe_Classes>

    \sa
    <link ge_OdGeLine, Working with Lines>
*/
class GE_TOOLKIT_EXPORT OdGeLine3d : public OdGeLinearEnt3d
{
public:


  /** \details
    Default constructor for the OdGeLine3d class. Constructs an infinite line coincident with the X-axis.
  */
  OdGeLine3d();
  
  /** \details
    Copy constructor for the OdGeLine3d class.
    
    \param source [in]  A 3D line to be copied.
  */
  OdGeLine3d(
    const OdGeLine3d& source);
  
  /** \details
    Constructor for the OdGeLine3d class.
    
    \param point [in]  A 3D point on a line.
    \param vect [in]  Defines the direction of the line.
    
    \remarks
    Constructs an infinite line coincident with the passed vector direction.
  */
  OdGeLine3d(
    const OdGePoint3d& point, 
    const OdGeVector3d& vect);
  
  /** \details
    Constructor for the OdGeLine3d class.
    
    \param point1 [in]  First point of the 3D line.
    \param point2 [in]  Second point of the 3D line.
    
    \remarks
    Constructs an infinite line that passes through the first and second points.
    The passed points should not be coincident.
  */
  OdGeLine3d(
    const OdGePoint3d& point1, 
    const OdGePoint3d& point2);

  GE_STATIC_EXPORT static const OdGeLine3d kXAxis; // X-axis *line*.
  GE_STATIC_EXPORT static const OdGeLine3d kYAxis; // Y-axis *line*.
  GE_STATIC_EXPORT static const OdGeLine3d kZAxis; // Z-axis *line*.

  /** \details
    Sets the parameters for this line according to the arguments, and returns a reference to this line.

    \param point [in]  A 3D point on a line.
    \param vect [in]  Any 3D vector.

    \remarks
    point and vect construct an infinite line passing through point with 
    a direction of vect. vect cannot have a zero length.
  */
   OdGeLine3d& set(
    const OdGePoint3d& point, 
    const OdGeVector3d& vect);

  /** \details
    Sets the parameters for this line according to the arguments, and returns a reference to this line.

    \param point1 [in]  First point of the 3D line.
    \param point2 [in]  Second point of the 3D line.

    \remarks
    point1 and point2 construct an infinite line passing through both points. The 
    points should no be coincident.
  */
  OdGeLine3d& set(
    const OdGePoint3d& point1, 
    const OdGePoint3d& point2);

  /** \details
    Assignment operator for the OdGeLine3d object.

    \param line [in]  Line to assign.
    
    \returns
    A reference to this OdGeLine3d object.
  */
  OdGeLine3d& operator =(
    const OdGeLine3d& line);
//DOM-IGNORE-BEGIN
private:
  OdGeLine3d(OdGeEntity3dImpl*);
//DOM-IGNORE-END
};

#include "TD_PackPop.h"

#endif // OD_GELINE3D_H
