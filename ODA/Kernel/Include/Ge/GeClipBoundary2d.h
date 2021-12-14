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

#ifndef OD_GECLIPBOUNDARY2D_H
#define OD_GECLIPBOUNDARY2D_H /*!DOM*/

#include "Ge/GeEntity2d.h"
#include "Ge/GePoint2dArray.h"
#include "Ge/GeIntArray.h"

#include "TD_PackPush.h"

/** \details
    This class defines a planar clipping object for clipping arbitrary
    2D polygons or polylines to 2D convex open or closed polyline curves
    or rectangles.

    Corresponding C++ library: TD_Ge

   <group OdGe_Classes> 
*/
class GE_TOOLKIT_EXPORT OdGeClipBoundary2d : public OdGeEntity2d
{
public:
  

  /** \details
    Default constructor for the OdGeClipBoundary2d class. Constructs an empty 2D clip boundary.
    Currently not implemented.
  */
  OdGeClipBoundary2d();
  
  /** \details
    Constructor for the OdGeClipBoundary2d class. Constructs a 2D clip boundary limited by the specified points. 
    Currently not implemented.
    
    \param cornerA [in]  First corner to define a clip boundary.
    \param cornerB [in]  Second corner to define a clip boundary.
  */
  OdGeClipBoundary2d(
    const OdGePoint2d& cornerA, 
    const OdGePoint2d& cornerB);
  
  /** \details
    Constructor for the OdGeClipBoundary2d class. Constructs a 2D clip boundary from the passed array of points. 
    
    \param clipBoundary [in]  Array of points to define a clip boundary.
  */
  OdGeClipBoundary2d(
    const OdGePoint2dArray& clipBoundary);
  
  /** \details
    Copy constructor for the OdGeClipBoundary2d class. Copies the specified clip boundary. 
    
    \param src [in]  Clip boundary to copy.
  */
  OdGeClipBoundary2d(
    const OdGeClipBoundary2d& src);

  /** \details
    Initializes ortho-aligned rectangular clip boundary. 
    Currently not implemented.
    
    \param cornerA [in]  First corner to define a clip boundary.
    \param cornerB [in]  Second corner to define a clip boundary.
    \returns
    eOk if initialized successfully, or an appropriate error code otherwise.
  */
  OdGe::ClipError set(
    const OdGePoint2d& cornerA, 
    const OdGePoint2d& cornerB);
    
  /** \details
    Initializes convex polyline / polygon clip boundary.

    \param clipBoundary [in]  Array of points to define a clip boundary.
    \returns
    eOk if initialized successfully, or an appropriate error code otherwise.
  */
  OdGe::ClipError set(
    const OdGePoint2dArray& clipBoundary);

  /** \details
    Clips a closed polygon, creating a second closed polygon. 
    Currently not implemented.

    \param rawVertices                [in]  Array of 2D points that define the input polygon. Self-intersecting and winding polygons are allowed.
    \param clippedVertices            [out] Receives an array of 2D points that define the clipped resulting closed polygon.
    \param clipCondition              [out] Receives the information about the spatial relationship between the clip boundary and the input clipped polygon.
    \param pClippedSegmentSourceLabel [in]  Optional segment label information identifying the source of each segment in the clipped polygon.
    \returns
    eOk if clipped successfully, or an appropriate error code otherwise.
  */
  OdGe::ClipError clipPolygon(
    const OdGePoint2dArray& rawVertices, 
    OdGePoint2dArray& clippedVertices,
    OdGe::ClipCondition& clipCondition,
    OdGeIntArray* pClippedSegmentSourceLabel = NULL) const;

  /** \details
    Clips a polyline, creating a second polyline. 
    Currently not implemented.

    \param rawVertices                [in]  Array of 2D points that define the input polyline. Self-intersecting and winding polylines are allowed.
    \param clippedVertices            [out] Receives an array of 2D points that define the clipped resulting polyline.
    \param clipCondition              [out] Receives the information about the spatial relationship between the clip boundary and the input clipped polyline.
    \param pClippedSegmentSourceLabel [in]  Optional segment label information identifying the source of each segment in the clipped polyline.
    \returns
    eOk if clipped successfully, or an appropriate error code otherwise.
  */
  OdGe::ClipError clipPolyline(
    const OdGePoint2dArray& rawVertices, 
    OdGePoint2dArray& clippedVertices,
    OdGe::ClipCondition& clipCondition,
    OdGeIntArray* pClippedSegmentSourceLabel = NULL) const;

  /** \details
    Assignment operator for the OdGeClipBoundary2d class. Assigns properties of the specified clip boundary.

    \param src   [in]  Clip boundary to assign.
  */
  OdGeClipBoundary2d& operator =(const OdGeClipBoundary2d& src);

};

#include "TD_PackPop.h"

#endif
