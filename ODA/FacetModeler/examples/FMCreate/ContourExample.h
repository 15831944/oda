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

#pragma once

#include "FMProfile2D.h"

using namespace FacetModeler;

/** \details
   Example of creating a square.
  
   \param side [in] The length of the side of the square.
   \returns Resulting Profile2D.
*/
Profile2D createSquare(const double side = 5.0);

/** \details
   Example of creating a circle.
  
   \param radius [in] Radius.
   \returns Resulting Profile2D.
*/
Profile2D createCircle(const double radius = 5.0);

/** \details
   Polygon creation example.
  
   \param radius   [in] The radius of the circle in which the polygon fits.
   \param sidesCnt [in] Count of polygon sides.
   \returns Resulting Profile2D.
*/
Profile2D createPolygon(double radius = 4.0, unsigned int sidesCnt = 6);

/** \details
   Example of creating a nut profile using a boolean operation.
  
   \param majorRadius [in] The radius of the circle in which the hexagon fits.
   \param minorRadius [in] The radius of the inner circle.
   \returns Resulting Profile2D.

   \remarks
   You can learn more about examples of boolean operations with CreateBodyExample.
*/
Profile2D createNutProfile(double majorRadius = 4.0, double minorRadius = 3.0);

