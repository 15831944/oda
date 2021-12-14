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
#ifndef _ODGITOOLS_INCLUDED_
#define _ODGITOOLS_INCLUDED_

#include "Gi/GiWorldDraw.h"
#include "Ge/GeCircArc2d.h"
#include "Ge/GeLineSeg2d.h"

namespace OdGiTools {

  inline void polyline2d( OdGiGeometry& geom,
    int n, const OdGePoint2d* pts, double z = 0.0 )
  {
    OdGePoint3dArray pts3d(n);
    while(n--) {
      pts3d.append()->set( pts->x, pts->y, z );
      ++pts;
    }
    geom.polyline( pts3d.size(), pts3d.getPtr() );
  }

  inline void arc2d( OdGiGeometry& geom,
    const OdGePoint2d& p1,
    const OdGePoint2d& p2,
    const OdGePoint2d& p3,
    double z = 0.0
    )
  {
    OdGePoint3d pp1, pp2, pp3;
    pp1.set( p1.x, p1.y, z );
    pp2.set( p2.x, p2.y, z );
    pp3.set( p3.x, p3.y, z );
    geom.circularArc(pp1, pp2, pp3);
  }

  inline void arc2d( OdGiGeometry& geom,
    const OdGeCircArc2d& arc,
    double z = 0.0 )
  {
    OdGePoint2dArray pts( 3 );
    arc.getSamplePoints( 3, pts );
    arc2d( geom, pts[0], pts[1], pts[2], z );
  }

  inline void lineSeg2d( OdGiGeometry& geom,
    const OdGeLineSeg2d& line,
    double z = 0.0 )
  {
    OdGePoint2d pts[] = { line.startPoint(), line.endPoint() };
    polyline2d( geom, 2, pts, z );
  }

};

#endif //_ODGITOOLS_INCLUDED_
