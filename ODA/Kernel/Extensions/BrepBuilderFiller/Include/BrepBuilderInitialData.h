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

#include "BrepBuilder/BrepBuilder.h"
#include "Ge/GeCurve2dPtrArray.h"
#include "Ge/GeCurve3dPtrArray.h"
#include "CmColorBase.h"
#include "Gi/GiMaterial.h"

class OdBrEdge;
class OdBrFace;
class OdIMaterialAndColorHelper;


/** \details
  Helper structure for initialize OdBrepBuilder.
  Corresponding C++ library: TD_BrepBuilderFiller
  <group !!RECORDS_tkernel_apiref>
*/
struct BrepBuilderInitialCoedge
{
  typedef OdUInt32 EdgeIndex;
  EdgeIndex edgeIndex;
  OdBrepBuilder::EntityDirection direction;
  OdGeCurve2dPtr curve;

  /** \details
    Creates a new instance of coedge data with default parameters.
  */
  BrepBuilderInitialCoedge()
    : curve(NULL)
    , direction(OdBrepBuilder::kForward)
    , edgeIndex(0)
  {}
  /** \details
    Creates a new instance of coedge data.

    \param curve     [in] Parametric curve.
    \param direction [in] Direction relative edge.
    \param edgeIndex [in] Edge index.
  */
  BrepBuilderInitialCoedge(
    OdGeCurve2dPtr curve,
    OdBrepBuilder::EntityDirection direction,
    BrepBuilderInitialCoedge::EdgeIndex edgeIndex
  )
    : curve(curve)
    , direction(direction)
    , edgeIndex(edgeIndex)
  {}
};
typedef OdArray<BrepBuilderInitialCoedge> BrepBuilderInitialCoedgeArray;

/** \details
  Helper structure for initialize OdBrepBuilder.
  Corresponding C++ library: TD_BrepBuilderFiller
  <group !!RECORDS_tkernel_apiref>
*/
struct BrepBuilderInitialLoop
{
  BrepBuilderInitialCoedgeArray coedges;

  /** \details
    Creates a new instance of loop data with default parameters.
  */
  BrepBuilderInitialLoop() {}
  /** \details
    Creates a new instance of loop data with single coedge.

    \param curve     [in] Parametric curve.
    \param edgeIndex [in] Edge index.
    \param direction [in] Direction relative edge.
  */
  BrepBuilderInitialLoop(OdGeCurve2dPtr curve,
    BrepBuilderInitialCoedge::EdgeIndex edgeIndex,
    OdBrepBuilder::EntityDirection direction
  )
    : coedges(1)
  {
    coedges.append(BrepBuilderInitialCoedge(curve, direction, edgeIndex));
  }
};
typedef OdArray<BrepBuilderInitialLoop> BrepBuilderInitialLoopArray;

/** \details
  Helper structure for initialize OdBrepBuilder.
  Corresponding C++ library: TD_BrepBuilderFiller
  <group !!RECORDS_tkernel_apiref>
*/
struct BrepBuilderInitialSurface
{
  OdGeSurfacePtr pSurf;
  OdBrepBuilder::EntityDirection direction;
  BrepBuilderInitialLoopArray loops;

  OdDbStub* material;
  bool hasMaterialMapping;
  OdGiMapper materialMapper;

  bool hasColor;
  OdCmEntityColor color;

  std::pair<bool, OdGsMarker> marker;

  /** \details
    Creates a new instance of face data with default parameters.
  */
  BrepBuilderInitialSurface()
    : pSurf(NULL)
    , direction(OdBrepBuilder::kForward)
    , material(NULL)
    , hasMaterialMapping(false)
    , hasColor(false)
    , color(OdCmEntityColor::kNone)
    , marker(false,0)
  {}

  /** \details
    Copy face data except loops.

    \param other [in] Face to copy.
  */
  void copyFaceExceptLoops(BrepBuilderInitialSurface& other)
  {
    pSurf = other.pSurf;
    direction = other.direction;
    material = other.material;
    hasMaterialMapping = other.hasMaterialMapping;
    materialMapper = other.materialMapper;
    hasColor = other.hasColor;
    color = other.color;
    marker.first = false; // empty marker
  }

  //DOM-IGNORE-BEGIN
  OdResult setupVisualInfo(const OdBrFace& face, OdIMaterialAndColorHelper* pMaterialHelper);
  //DOM-IGNORE-END
};
typedef OdArray<BrepBuilderInitialSurface> BrepBuilderInitialSurfaceArray;

typedef OdArray<BrepBuilderInitialSurfaceArray> BrepBuilderShellsArray;

typedef OdArray<BrepBuilderShellsArray> BrepBuilderComplexArray;

/** \details
  Helper structure for initialize OdBrepBuilder.
  Corresponding C++ library: TD_BrepBuilderFiller
  <group !!RECORDS_tkernel_apiref>
*/
struct BrepBuilderInitialVertex
{
  OdGePoint3d point;
};
typedef OdArray<BrepBuilderInitialVertex> BrepBuilderInitialVertexArray;

/** \details
  Helper structure for initialize OdBrepBuilder.
  Corresponding C++ library: TD_BrepBuilderFiller
  <group !!RECORDS_tkernel_apiref>
*/
struct BrepBuilderInitialEdge
{
  OdGeCurve3dPtr curve;
  typedef OdUInt32 VertexIndex;
  static const VertexIndex kDefaultVertexId = ~4;
  VertexIndex vertexIndex[2];
  std::pair<bool, OdGsMarker> marker;
  bool hasColor;
  OdCmEntityColor color;
  BrepBuilderInitialEdge(OdGeCurve3dPtr edgeCurve = NULL, BrepBuilderInitialEdge::VertexIndex vertex1Index = kDefaultVertexId, BrepBuilderInitialEdge::VertexIndex vertex2Index = kDefaultVertexId)
    : curve(edgeCurve)
    , hasColor(false)
    , color(OdCmEntityColor::kNone)
    , marker(false, 0)
  {
    vertexIndex[0] = vertex1Index,
      vertexIndex[1] = vertex2Index;
  }

  //DOM-IGNORE-BEGIN
  OdResult setupVisualInfo(const OdBrEdge& edge, OdIMaterialAndColorHelper* pMaterialHelper);
  //DOM-IGNORE-END
};
typedef OdArray<BrepBuilderInitialEdge> BrepBuilderInitialEdgeArray;

/** \details
  Helper structure for initialize OdBrepBuilder.
  Corresponding C++ library: TD_BrepBuilderFiller
  <group !!RECORDS_tkernel_apiref>
*/
struct BrepBuilderInitialData
{
  BrepBuilderInitialVertexArray vertices;
  BrepBuilderInitialEdgeArray edges;
  BrepBuilderComplexArray complexes;
};
