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




#ifndef OD_GEGBLGE_H
#define OD_GEGBLGE_H /*!DOM*/

#include "TD_PackPush.h"

#include "Ge/GeExport.h"
#include "Ge/GeLibVersion.h"

/** \details
  The OdGe classes are used to carry out general 2D and 3D geometric operations. 

  Corresponding C++ library: TD_Ge
  
  <group OdGe_Classes>
*/
struct OdGe
{
  /** \details
    Version of the OdGe library being used
  */
  static const OdGeLibVersion gLibVersion;

  //unused
  //DOM-IGNORE-BEGIN
  enum 
  { 
    eGood, 
    eBad 
  };
  //DOM-IGNORE-END

  /** \details
    Represents type of a geometrical entity.
  */
  enum EntityId 
  {
    /** Entity 2d.*/
    kEntity2d,
    /** Entity 3d.*/
    kEntity3d,
    /** Point entity 2d.*/
    kPointEnt2d,
    /** Point entity 3d.*/
    kPointEnt3d,
    /** Position 2d.*/
    kPosition2d,
    /** Position 3d.*/
    kPosition3d,
    /** Point on curve 2d.*/
    kPointOnCurve2d,
    /** Point on curve 3d.*/
    kPointOnCurve3d,
    /** Point on surface.*/
    kPointOnSurface,
    /** Bounded plane.*/
    kBoundedPlane,
    /** Circular arc 2d.*/
    kCircArc2d,
    /** Circular arc 3d.*/
    kCircArc3d,
    /** Conic 2d.*/
    kConic2d,
    /** Conic 3d.*/
    kConic3d,
    /** Curve 2d.*/
    kCurve2d,
    /** Curve 3d.*/
    kCurve3d,
    /** Elliptical arc 2d.*/
    kEllipArc2d,
    /** Elliptical arc 3d.*/
    kEllipArc3d,
    /** Line 2d.*/
    kLine2d,
    /** Line 3d.*/
    kLine3d,
    /** Linear entity 2d.*/
    kLinearEnt2d,
    /** Linear entity 3d.*/
    kLinearEnt3d,
    /** Line segment 2d.*/
    kLineSeg2d,
    /** Line segment 3d.*/
    kLineSeg3d,
    /** Planar entity.*/
    kPlanarEnt,
    /** Plane.*/
    kPlane,
    /** Ray 2d.*/
    kRay2d,
    /** Ray 3d.*/
    kRay3d,
    /** Surface.*/
    kSurface,
    /** Sphere.*/
    kSphere,
    /** Cylinder.*/
    kCylinder,
    /** Torus.*/
    kTorus,
    /** Cone.*/
    kCone,
    /** Spline entity 2d.*/
    kSplineEnt2d,
    /** Polyline 2d.*/
    kPolyline2d,
    /** Augmented polyline 2d.*/
    kAugPolyline2d,
    /** Nurb curve 2d.*/
    kNurbCurve2d,
    /** DSpline 2d.*/
    kDSpline2d,
    /** Cubic spline curve 2d.*/
    kCubicSplineCurve2d,
    /** Spline entity 3d.*/
    kSplineEnt3d,
    /** Polyline 3d.*/
    kPolyline3d,
    /** Augmented polyline 3d.*/
    kAugPolyline3d,
    /** NurbCurve3d.*/
    kNurbCurve3d,
    /** DSpline 3d.*/
    kDSpline3d,
    /** Cubic spline curve 3d.*/
    kCubicSplineCurve3d,
    /** Trimmed Curve 2d.*/
    kTrimmedCrv2d,
    /** Composite curve 2d.*/
    kCompositeCrv2d,
    /** Composite curve 3d.*/
    kCompositeCrv3d,
    /** External Surface.*/
    kExternalSurface,
    /** Nurb surface.*/
    kNurbSurface,
    /** Trimmed surface.*/
    kTrimmedSurface,
    /** Offset surface.*/
    kOffsetSurface,
    /** Envelope 2d.*/
    kEnvelope2d,
    /** Curve bounded surface.*/
    kCurveBoundedSurface,
    /** External curve 3d.*/
    kExternalCurve3d,
    /** External curve 2d.*/
    kExternalCurve2d,
    /** Surface curve 2d to 3d.*/
    kSurfaceCurve2dTo3d,
    /** Surface curve 3d to 2d.*/
    kSurfaceCurve3dTo2d,
    /** External bounded surface.*/
    kExternalBoundedSurface,
    /** Curve-curve intersector 2d.*/
    kCurveCurveInt2d,
    /** Curve-curve intersector 3d.*/
    kCurveCurveInt3d,
    /** Bounding block 2d.*/
    kBoundBlock2d,
    /** Bounding block 3d.*/
    kBoundBlock3d,
    /** Offset curve 2d.*/
    kOffsetCurve2d,
    /** Offset curve 3d.*/
    kOffsetCurve3d,
    /** Polynomial curve 3d.*/
    kPolynomCurve3d,
    /** Bezier curve 3d.*/
    kBezierCurve3d,
    /** Object.*/
    kObject,
    /** Fit data 3d.*/
    kFitData3d,
    /** Hatch.*/
    kHatch,
    /** Trimmed curve 2d.*/
    kTrimmedCurve2d,
    /** Trimmed curve 3d.*/
    kTrimmedCurve3d,
    /** Curve sample data.*/
    kCurveSampleData,
    /** Elliptical cone.*/
    kEllipCone,
    /** Elliptical cylinder.*/
    kEllipCylinder,
    /** Interval bounding block.*/
    kIntervalBoundBlock,
    /** Clip boundary 2d.*/
    kClipBoundary2d,
    /** External object.*/
    kExternalObject,
    /** Curve-surface intersector.*/
    kCurveSurfaceInt,
    /** Surface-surface intersector.*/
    kSurfaceSurfaceInt,
    /** Helix.*/
    kHelix,
    /** Spun surface.*/
    kSpunSurface,
    /** Segment chain 2d.*/
    kSegmentChain2d,

    /** Revolved surface.*/
    kRevolvedSurface,
    /** Ruled surface.*/
    kRuled,
    /** Edge curve (for internal use only).*/
    kEdgeCurve,
    /** Dummy value 1.*/
    kDummy1,
    /** Dummy value 2.*/
    kDummy2,
    /** Dummy value 3.*/
    kDummy3,
    /** Dummy value 4.*/
    kDummy4,
    /** Dummy value 5.*/
    kDummy5,
    /** Dummy value 6.*/
    kDummy6,
    /** Dummy value 7.*/
    kDummy7,
    /** Dummy value 8.*/
    kDummy8,
    /** Dummy value 9.*/
    kDummy9
  };

  /** \details
    Represents type of an external entity.
  */
  enum ExternalEntityKind 
  {
    /**External Entity is an ACIS entity.*/
    kAcisEntity              = 0,
    /**External Entity is an OdGeCurve3d* entity.*/
    kGe3dCurveEntity         = 1,
    /**External Entity is an OdGeSurface3d* entity.*/
    kGe3dSurfaceEntity       = 2,
    /**External Entity is undefined.*/
    kExternalEntityUndefined,
    /**External Entity is a BimRv entity.*/
    kBimEntity
  };
  
  /** \details
    Represents point containmant in the boundary.
  */
  enum PointContainment 
  {
    /**Point is inside the boundary.*/
    kInside,
    /**Point is outside the boundary.*/
    kOutside,
    /**Point is the boundary.*/
    kOnBoundary
  };
  
  /** \details
    Represents properties for the U and V parameters in a nurb surface.
  */
  enum NurbSurfaceProperties 
  {
    /**Nurb surface is uninitialized.*/
    kUninit     = 0x00,
    /**Nurb surface is open.*/
    kOpen       = 0x01,
    /**Nurb surface is closed.*/
    kClosed     = 0x02,
    /**Nurb surface is periodic.*/
    kPeriodic   = 0x04,
    /**Nurb surface is rational.*/
    kRational   = 0x08,
    /**Nurb surface has no poles.*/
    kNoPoles    = 0x10,
    /**Nurb surface has a pole at the lower bound.*/
    kPoleAtMin  = 0x20,
    /**Nurb surface has a pole at the upper bound.*/
    kPoleAtMax  = 0x40,
    /**Nurb surface has poles at the lower and upper bounds.*/
    kPoleAtBoth = 0x80
  };

  /** \details
    Represents offset curve extension type.
  */
  enum OffsetCrvExtType {
    /**Fillet.*/
    kFillet,
    /**Chamfer.*/
    kChamfer,
    /**Extend.*/
    kExtend
  };

  /** \details
    Represents configuration of first curve at the intersection point with respect to second curve
    and with respect to the specified plane normal.
  */
  enum OdGeXConfig 
  {
    /**Configuration is not defined.*/
    kNotDefined      = 1 << 0,
    /**Unknown configuration.*/
    kUnknown         = 1 << 1,
    /**First curve crosses the second curve from left to right.*/
    kLeftRight       = 1 << 2,
    /**First curve crosses the second curve from right to left.*/
    kRightLeft       = 1 << 3,
    /**First curve touches the second curve from the left.*/
    kLeftLeft        = 1 << 4,
    /**First curve touches the second curve from the right.*/
    kRightRight      = 1 << 5,
    /**First curve starts or ends in the intersection point and is to the left from the second curve.*/
    kPointLeft       = 1 << 6,
    /**First curve starts or ends in the intersection point and is to the right from the second curve.*/
    kPointRight      = 1 << 7,
    /**First curve is to the left from the second curve before the intersection point and curves are overlapping after the intersection point.*/
    kLeftOverlap     = 1 << 8,
    /**Curves are overlapping before the intersection point and first curve is to the left from the second curve after the intersection point.*/
    kOverlapLeft     = 1 << 9,
    /**First curve is to the right from the second curve before the intersection point and curves are overlapping after the intersection point.*/
    kRightOverlap    = 1 << 10,
    /**Curves are overlapping before the intersection point and first curve is to the right from the second curve after the intersection point.*/
    kOverlapRight    = 1 << 11,
    /**First curve starts in the intersection point and is overlapping with the second curve nearby.*/
    kOverlapStart    = 1 << 12,
    /**First curve ends in the intersection point and is overlapping with the second curve nearby.*/
    kOverlapEnd      = 1 << 13,
    /**Curves are overlapping before and after the intersection point.*/
    kOverlapOverlap  = 1 << 14
  };

  /** \details
    Represents type of Boolean operation.
  */
  enum BooleanType 
  {
    /**Boolean union.*/
    kUnion,
    /**Boolean subtraction.*/
    kSubtract,
    /**Boolean intersection.*/
    kCommon
  };

  /** \details
    Represents clip error.
  */
  enum ClipError 
  {
    /**No error.*/
    eOk,
    /**Invalid clip boundary.*/
    eInvalidClipBoundary,
    /**Clip boundary was not initialized.*/
    eNotInitialized
  };
  
  /** \details
    Represents clip condition.
  */
  enum ClipCondition 
  {
    /**An error occurred, probably due to invalid initialization of clipping object.*/
    kInvalid,
    /**All segments are inside the clip boundary.*/
    kAllSegmentsInside,
    /**At least one segment crosses the clip boundary.*/
    kSegmentsIntersect,
    /**The clipped polygon is outside the clip region, and the clip region is not encircled by the clipped polygon.*/
    kAllSegmentsOutsideZeroWinds,
    /**The clipped polygon is outside the clip region, and the clip region is encircled by the clipped polygon an odd number of times.
    This normally indicates that the clip region is inside the region enclosed by the input polygon.*/
    kAllSegmentsOutsideOddWinds,
    /**The clipped polygon is outside the clip region, and the clip region is encircled by the clipped polygon an even number of times.*/
    kAllSegmentsOutsideEvenWinds
  };

  /** \details 
    Represents error codes that methods receive during execution.
  */
  enum ErrorCondition 
  {
    /**No error.*/
    kOk,
    /**This object is 0.*/
    k0This,
    /**Argument 1 is 0.*/
    k0Arg1,
    /**Argument 2 is 0.*/
    k0Arg2,
    /**Arguments 1 and 2 are perpendicular to each other.*/
    kPerpendicularArg1Arg2,
    /**Arguments 1 and 2 are equal.*/
    kEqualArg1Arg2,
    /**Arguments 1 and 3 are equal.*/
    kEqualArg1Arg3,
    /**Arguments 2 and 3 are equal.*/
    kEqualArg2Arg3,
    /**Arguments 1, 2, and 3 are linearly dependent.*/
    kLinearlyDependentArg1Arg2Arg3,
    /**Argument 1 is too big.*/
    kArg1TooBig,
    /**Argument 1 is on this object.*/
    kArg1OnThis,
    /**Argument 1 is inside this object.*/
    kArg1InsideThis,
    /**Geometry is not coplanar.*/
    kNonCoplanarGeometry,
    /**Geometry is degenerate.*/
    kDegenerateGeometry,
    /**Geometry is one point.*/
    kSingularPoint
  };

  /** \details
    Represents configuration details about a curve-surface intersection.
  */
  enum csiConfig 
  {
    /**Unknown configuration.*/
    kXUnknown,
    /**Transverse -- Curve neighborhood is outside this surface.*/
    kXOut,
    /**Transverse -- Curve neighborhood is inside this surface.*/
    kXIn,
    /**Tangent -- Curve neighborhood is outside this surface.*/
    kXTanOut,
    /**Tangent -- Curve neighborhood is inside this surface.*/
    kXTanIn,
    /**Non-zero length -- Point is on the intersection boundary.*/
    kXCoincident,
    /**Non-zero length -- Point is on an arbitrary *point* on an unbounded intersection.*/
    kXCoincidentUnbounded
  };

  /** \details
    Represents types of a surface-surface intersection.
  */
  enum ssiType 
  {
    /**Non-tangent intersection.*/
    kSSITransverse,
    /**Tangent intersection with surface normals codirectional at any point within the component.*/
    kSSITangent,
    /**Tangent intersection with surface normals antidirectional at any point within the component.*/
    kSSIAntiTangent
  };

  /** \details
    Represents configuration details about a surface-surface intersection.
  */
  enum ssiConfig 
  {
    /**Unknown configuration.*/
    kSSIUnknown,
    /**Neighborhood is outside this surface.*/
    kSSIOut,
    /**Neighborhood is inside this surface.*/
    kSSIIn,
    /**Non-zero area intersection.*/
    kSSICoincident
  };

  /** \details
    Represents status of an intersection operation.
  */
  enum OdGeIntersectError 
  {
    /**No error.*/
    kXXOk,
    /**Index out of range.*/
    kXXIndexOutOfRange,
    /**Wrong dimension at index.*/
    kXXWrongDimensionAtIndex,
    /**Unknown error.*/
    kXXUnknown
  };

  /** \details
    Represents knot parameterization type.
  */
  enum OdGeKnotParameterization {
    /**Chord parametrization.*/
    kChord                  = 0x0,
    /**Chord square root parametrization.*/
    kSqrtChord              = 0x1,
    /**Uniform parametrization.*/
    kUniform                = 0x2,
    /**Custom parametrization.*/
    kCustomParameterization = 0xF,
    /**Knot parametrization isn't defined.*/
    kNotDefinedKnotParam    = 0x10
  };
};

/** \details
    Defines a synonym of the fully qualified name.

    \remarks
    May be used only if there are no global name conflicts.
*/
typedef OdGe::ErrorCondition OdGeError;

/** \details
    Defines a synonym of the fully qualified name.

    \remarks
    May be used only if there are no global name conflicts.
*/
typedef OdGe::OdGeIntersectError OdGeIntersectError;
typedef OdGe::OdGeKnotParameterization OdGeKnotParameterization;

#define DECLARE_GE_RTTI() \
  virtual bool isKindOf (OdGe::EntityId entType) const;\
  virtual OdGe::EntityId type() const

#define DEFINE_GE_RTTI(Class,Parent,TYPE) \
  bool Class::isKindOf(OdGe::EntityId entType) const\
  {\
    if ( entType == OdGe::TYPE ) return true;\
    return Parent::isKindOf( entType );\
  }\
  OdGe::EntityId Class::type() const { return OdGe::TYPE; }

/** \details
    Clears Ge heap.
*/
GE_TOOLKIT_EXPORT void odgeHeapCleanup();

#include "TD_PackPop.h"

#endif  // AC_GEGBLGE_H

