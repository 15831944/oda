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


#ifndef OD_GE_POINT_ENT_2D_H
#define OD_GE_POINT_ENT_2D_H /*!DOM*/


#include "Ge/GeEntity2d.h"
#include "Ge/GePoint2d.h"

#include "TD_PackPush.h"

/** \details
    This class is the base class for all OdGe 2D point classes.

    Corresponding C++ library: TD_Ge

    <group OdGe_Classes> 
*/
class GE_TOOLKIT_EXPORT OdGePointEnt2d : public OdGeEntity2d
{
public:

  /** \details
    Returns this object as an OdGePoint2d.
  */
  OdGePoint2d point2d() const;

  /** \details
    Converts this object to an OdGePoint2d object.
  */
  operator OdGePoint2d() const;
  
  /** \details
    Assignment operator for the OdGePointEnt2d object.

    \param point [in]  Point entity to assign.
    
    \returns
    Reference to this OdGePointEnt2d object.
  */
  OdGePointEnt2d& operator =(
    const OdGePointEnt2d& point);

  //////////////////////////////////////////////////////////////////////////
//DOM-IGNORE-BEGIN
protected:
  /** \param source [in]  Object to be cloned.
  */
  OdGePointEnt2d();
  OdGePointEnt2d(
    const OdGePointEnt2d& source);
//DOM-IGNORE-END
};

#include "TD_PackPop.h"

#endif // OD_GE_POINT_ENT_2D_H

