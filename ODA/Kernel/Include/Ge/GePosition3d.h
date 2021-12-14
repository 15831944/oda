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

#ifndef OD_GEPOS3D_H
#define OD_GEPOS3D_H /*!DOM*/

#include "Ge/GePointEnt3d.h"

#include "TD_PackPush.h"

/** \details
    This class represents points (positions) in 3D space.

    Corresponding C++ library: TD_Ge

    <group OdGe_Classes>

    \sa
    <link ge_OdGePosition, Working with Points>
*/
class GE_TOOLKIT_EXPORT OdGePosition3d : public OdGePointEnt3d
{
public:
  
  /** \details
    Default constructor for the OdGePosition3d class.
  */
  OdGePosition3d();
  
  /** \details
    Constructor for the OdGePosition3d class.
    
    \param point [in]  A point according to which position is set.

    \remarks
    The constructor constructs position object with a specified point.
  */
  OdGePosition3d(
    const OdGePoint3d& point);
  
  /** \details
    Constructor for the OdGePosition3d class.
    
    \param x [in]  The X-coordinate of the position.
    \param y [in]  The Y-coordinate of the position.
    \param z [in]  The Z-coordinate of the position.

    \remarks
    The constructor constructs position object with specified coordinate values.
  */
  OdGePosition3d(
    double x, 
    double y, 
    double z);
  
  /** \details
    Copy constructor for the OdGePosition3d class.
    
    \param source [in]  Object to be copied.
  */
  OdGePosition3d(
    const OdGePosition3d& source);

  /** \details
    Sets the coordinates of, and returns a reference
    to, this point.

    \param point [in]  A point according to which position is set.
  */
   OdGePosition3d& set(
    const OdGePoint3d& point);
  
  /** \details
    Sets the coordinates of, and returns a reference
    to, this point.

    \param x [in]  The X-coordinate of the position.
    \param y [in]  The Y-coordinate of the position.
    \param z [in]  The Z-coordinate of the position.
  */
  OdGePosition3d& set(
    double x, 
    double y, 
    double z);

  /** \details
    Assignment operator for the OdGePosition3d object.

    \param pos [in]  Position to assign.
    
    \returns
    Reference to this OdGePosition3d object.
  */
  OdGePosition3d& operator =(
    const OdGePosition3d& pos);

  //////////////////////////////////////////////////////////////////////////

};

#include "TD_PackPop.h"

#endif // OD_GEPOS3D_H

