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
#ifndef __FMGEOMETRY_IMPL_H__
#define __FMGEOMETRY_IMPL_H__

// This file contains common includes and definitions used 
// in implementation of AECGeometry library

#include "FMGeometry.h"

#include "FMGeometryDebug.h"
//#include "Contours/FM_ContoursDebug.h"
#include "Contours/FMContoursBaseImp.h"

#include "Ge/GeMatrix3d.h"

namespace FacetModeler
{
  enum Operation
  {
    // BooleanOperation

    eExtrusion = 4,
    eRevolution = 5,
    eInsideBody = 6,
    eInsideFrame = 7,
    eCustom = 8,
    eTemporaryExtensionByTwoContours = 9,
    eTemporaryExtensionExtrusionByTrs = 10,

    eclone = 100,
    enegate,
    emergeCoincidentVertices,
    emergeCoplanarEntities,
    ecombine,
    ebox,
    epyramid,
    eextrusion1,
    eextrusion2,
    eextrusion3,
    erevolution1,
    erevolution2,
    einsideBody,
    einsideFrame,
    ecustomInternal,
    esection,
    ecutWithExtrusion,
    ecutWithInfExtrusion,
    eextrude,
    eTemporary_Extension_byTwoContours,
    eTemporary_Extension_byTwoProfiles,
    eTemporary_Extension_extrusionByTrs
  };
}


#endif  //__FMGEOMETRY_IMPL_H__
