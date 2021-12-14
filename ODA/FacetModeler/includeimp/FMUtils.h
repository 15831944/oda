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

#ifndef __FMUTILS_H__
#define __FMUTILS_H__

#include "FMContour2D.h"
#include "FMContour3D.h"
#include "FMProfile2D.h"

namespace FacetModeler
{

FMGEOMETRY_API void ExtendContourStart(Contour2D& rContour);
FMGEOMETRY_API void ExtendContourEnd(Contour2D& rContour);

FMGEOMETRY_API void ExtendContour(Contour2D& rContour);

FMGEOMETRY_API double FindMinimumParamA(std::vector< Intersection >& vecPoints);
FMGEOMETRY_API double FindMaximumParamA(std::vector< Intersection >& vecPoints);

FMGEOMETRY_API bool FindClosestIntToBaseStart(const Contour2D& cBase, const Contour2D& cCutter, Intersection& intClosest);
FMGEOMETRY_API bool FindClosestIntToBaseEnd(const Contour2D& cBase, const Contour2D& cCutter, Intersection& intClosest);

FMGEOMETRY_API bool FindFarthestIntFromBasePt(const Contour2D& cContour, const OdGeLine2d& lnCutter, const OdGePoint2d& ptBase, Intersection& outInt);
FMGEOMETRY_API bool FindFarthestIntFromBasePt(const Contour2D& cContour, const Contour2D& cCutter, const OdGePoint2d& ptBase, Intersection& outInt);
FMGEOMETRY_API Intersection FindFarthestIntFromPt(const OdGePoint2d& ptBase, const std::vector< Intersection >& vecPoints);

FMGEOMETRY_API Intersection FindIntClosestToPt(const OdGePoint2d& ptBase, const std::vector< Intersection >& vecPoints);
FMGEOMETRY_API bool IsIntersectionCloserToPt(const OdGePoint2d& ptBase, const Intersection& intCurrent, const Intersection& intToCheck);

}; // namespace FacetModeler

#endif  //__FMUTILS_H__
