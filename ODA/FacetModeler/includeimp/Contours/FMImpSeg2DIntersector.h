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
#ifndef __FMIMP_SEG2DINTERSECTOR_H__
#define __FMIMP_SEG2DINTERSECTOR_H__

#include "Contours/FM_IBulgeSeg2D.h"

namespace FacetModeler
{

//////////////////////////////////////////////////////////////////////////
// Intersection helpers

FMGEOMETRY_API OdUInt32 intersect_LineLine( 
    const IBulgeSeg2D & rLine1, const IBulgeSeg2D & rLine2,
    Intersection & rInt1, Intersection & rInt2,
    const OdGeTol & gTol = FMGeGbl::gTol );

FMGEOMETRY_API OdUInt32 intersect_ArcLine( 
    const IBulgeSeg2D & rArc, const IBulgeSeg2D & rLine,
    Intersection & rInt1, Intersection & rInt2,
    const OdGeTol & gTol = FMGeGbl::gTol );

FMGEOMETRY_API OdUInt32 intersect_ArcArc ( 
    const IBulgeSeg2D & rArc1, const IBulgeSeg2D & rArc2,
    Intersection & rInt1, Intersection & rInt2,
    const OdGeTol & gTol = FMGeGbl::gTol );


// intersections with infinite lines
FMGEOMETRY_API OdUInt32 intersect_PointInfLine( 
    const OdGePoint2d& rPoint, 
    const OdGePoint2d& ptLineOrigin, const OdGeVector2d& vLineDir, 
    Intersection & rInt1,
    const OdGeTol & gTol = FMGeGbl::gTol );

FMGEOMETRY_API OdUInt32 intersect_LineInfLine( 
    const IBulgeSeg2D & rLineSeg, 
    const OdGePoint2d& ptLineOrigin, const OdGeVector2d& vLineDir, 
    Intersection & rInt1, Intersection & rInt2,
    const OdGeTol & gTol = FMGeGbl::gTol );

FMGEOMETRY_API OdUInt32 intersect_ArcInfLine( 
    const IBulgeSeg2D & rArcSeg, 
    const OdGePoint2d& ptLineOrigin, const OdGeVector2d& vLineDir, 
    Intersection & rInt1, Intersection & rInt2,
    const OdGeTol & gTol = FMGeGbl::gTol );

};



#endif  //__FMIMP_SEG2DINTERSECTOR_H__
