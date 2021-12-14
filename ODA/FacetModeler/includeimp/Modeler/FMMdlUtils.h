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
#ifndef __FMMDL_UTILS_H__
#define __FMMDL_UTILS_H__

#include "Modeler/FMMdlBase.h"
#include "Ge/GeCircArc3d.h"
#include "FMContour3D.h"
#include "FMProfile3D.h"
#include "Modeler/FMMdlFace.h"
#include "Modeler/FMMdlEdge.h"

namespace FacetModeler
{

struct PathNode {
  OdGeMatrix3d cs;
  const OdGeCircArc3d* pArcData;
  enum {
    kSmoothAtThisNode = 1,
  };
  OdUInt32 flags;
  PathNode() {
    pArcData = 0;
    flags = 0;
  }
};

enum {
  kExtrudeClosedPath          = 1,
  kExtrudeSmoothAlongPath     = 2,
  kExtrudeSmoothAlongProfile  = 4,
  kExtrudeSingleArcProfile    = 8,
};

FMGEOMETRY_API Body extrude(
    const Profile2D& base,
    const DeviationParams& deviation,
    OdUInt32 n,
    const PathNode* path,
    OdUInt32 flags = 0 );

FMGEOMETRY_API void getBaseFaces(const Profile2D& rProfile, std::vector<Profile2D>& aBaseFaces, const DeviationParams& devDeviation);

struct MdlTolerance
{
    FMGEOMETRY_API_STATIC static const double kDefaultTol;
    FMGEOMETRY_API_STATIC static const double kSplitTol;
    FMGEOMETRY_API_STATIC static const double kZeroTol;
};

struct EdgeSortForPairRegen
{
    bool operator() ( const Edge* pE1, const Edge* pE2 ) const
    {
        Vertex* pV11 = pE1->vertex();
        Vertex* pV12 = pE1->next()->vertex();

        Vertex* pV21 = pE2->vertex();
        Vertex* pV22 = pE2->next()->vertex();

        if ( pV11 > pV12 ) std::swap( pV11, pV12 );
        if ( pV21 > pV22 ) std::swap( pV21, pV22 );

        if ( pV11 == pV21 && pV12 == pV22 )
            return false;

        if ( pV11 != pV21 )
        {
            return pV11 < pV21;
        }

        return pV12 < pV22;
    }

    static bool isEqual( const Edge* pE1, const Edge* pE2 )
    {
        EdgeSortForPairRegen cComparer;
        return (!cComparer( pE1, pE2 )) && (!cComparer( pE2, pE1 ));
    }
};

}

#endif // __FMMDL_UTILS_H__
