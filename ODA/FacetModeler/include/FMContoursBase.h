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

#ifndef __FMCONTOURSBASE_H__
#define __FMCONTOURSBASE_H__

#include "FMGeometry.h"
#include <Ge/GePoint2d.h>
#include "DoubleArray.h"

namespace FacetModeler
{
  /** \details
     Defines possible result codes used in Contour2D and Profile2D classes.

     \remarks
     Warnings are in the range [erWarnings.. erErrors).
     Errors are greater than erErrors.
  */
  enum Result
  {
    /** No error */
    erOk = 0,

    /** Warning */
    erWarnings,

    /** Parameter on contour is not valid */
    erWarnParamBounds,

    /** Input point or vector is not on this entity, but it should be */
    erWarnPointNotOnThis,

    /** Contour is empty */
    erWarnEmptyContour,

    /** Contour is too small */
    erWarnSmallContour,

    /** Region/Profile contours are not valid */
    erWarnImproperAreas,

    /** A contour is not closed, but it should be */
    erWarnContourNotClosed,

    /** Error */
    erErrors = 0x1000,

    /** Not implemented */
    erNotImplementedYet,

    /** Array/Segment index is out of range */
    erInvalidIndex,

    /** Parameter value is out of range */
    erParamBounds,

    /** Input point or vector is not on this entity */
    erPointNotOnThis,

    /** Invalid (singular) transformation matrix */
    erInvalidTransform,

    /** Wrong segment type, another type expected */
    erWrongSegType,

    /** Contour is empty */
    erEmptyContour,

    /** Some points are invalid */
    erSingularPoints,

    /** Metadata is not available and/or can`t be updated */
    erNoMetadata,

    /** Some input parameters are completely invalid */
    erInvalidArgs,

    /** Contour is too small */
    erContourIsTooSmall,

    /** Contour or profile is self-intersecting */
    erSelfIntersecting,

    /** Contour orientation is not applicable */
    erInvalidDirection,

    /** A contour is not closed, but it should be */
    erContourNotClosed,

    /** Operation is not applicable */
    erOperationNotApplicable,

    /** Invalid result in area operation */
    erFailedToCloseContours,

    /** Region/Profile is not valid in a regional operation */
    erImproperRegionBounds,

    /** Region/Profile is not valid in a regional operation */
    erImproperRegionAreas,

    /** Tolerance is too small, or input contours are singular */
    erToleranceViolation,

    /** Unknown error */
    erFail,

    /** Last error */
    erLastError
  };

  /** \details
     Checks whether the result is an error.
    
     \param eRes [in] Value to check.
     \returns true if the value is not an error, or false otherwise.
  */
  inline bool isOk(Result eRes)
  {
    return eRes < erErrors;
  };

  /** \details
     Checks whether the result is a warning.
    
     \param eRes [in] Value to check.
     \returns true if the value is a warning, or false otherwise.
  */
  inline bool isWarning(Result eRes)
  {
    return eRes < erErrors && eRes >= erWarnings;
  };

  /** \details
     Checks whether the result is an error.
    
     \param eRes [in] Value to check.
     \returns true if the value is an error, or false otherwise.
  */
  inline bool isError(Result eRes)
  {
    return eRes >= erErrors;
  };

  /** \details
     Defines types of segments.
  */
  enum SegmentType
  {
    /** Unknown type */
    estUnknown = 0,

    /** Straight segment with length greater than 0 */
    estLine,

    /** Arc segment with length greater than 0 */
    estArc,

    /** Segment with 0 length */
    estCoincident
  };

  /** \details
     Defines possible object placement with respect to another object.
  */
  enum Locations
  {
    /** Unknown */
    elUnknown = 0,

    /** Inside */
    elInside,

    /** Outside */
    elOutside,

    /** On bound */
    elOnBound
  };

  /** \details
     Defines flags for the mergeSegments method.
  */
  enum MergeFlags
  {
    /** Merge circular segments */
    emfMergeArcs = 1,

    /** Merge segments with different metadata */
    emfIgnoreMetadata = 2,

    /** Merge first and last segments of a closed contour */
    emfMergeOrigin = 4
  };

  /** \details
     Defines gap filling strategy types for offset curve generation.
  */
  enum FilletType
  {
    /** Close gaps with straight line segments */
    eftChamfer,

    /** Extend segments with tangential rays */
    eftTangents,

    /** Extend segments with tangential rays or circular arcs, 
       depending on segment types */
    eftExtend,

    /** Close gaps with circular arcs */
    eftCircular
  };

  //DOM-IGNORE-BEGIN
  enum ContourImplClass
  {
    ecicUnknown = 0,
    ecicSimple,
    ecicCached,
    ecicLast
  };

  enum SegmentImplClass
  {
    esicUnknown = 0,
    esicBulgeSeg2D,
    esicCachedSeg2D,
    esicLast
  };
  //DOM-IGNORE-END

  /** \details
     Describes an intersection between two curves/segments.
  */
  class FMGEOMETRY_API Intersection
  {
  public:
    /** \details
       Defines intersection types.
    */
    enum Types
    {
      /** No intersection */
      eitNone = 0,

      /** Intersection of any type (the type was not defined) */
      eitAny,

      /** Curves cross each other at a point */
      eitCrossing,

      /** Curves touch each other at a point */
      eitTouch
    };

    /** \details
       Default constructor.
    */
    Intersection() : dParamA(0), dParamB(0), eType(eitNone)
    { }

    /** \details
       Constructor. Creates an object with specified intersection point, parameters and intersection type.
    */
    Intersection(const OdGePoint2d& rPt, double dPA = 0, double dPB = 0, Types eIT = eitNone)
      : ptPoint(rPt), dParamA(dPA), dParamB(dPB), eType(eIT)
    { }

    /** \details
       Swaps parameters of the intersection.
      
       \returns Reference to this object.
    */
    inline Intersection & swapParams()
    {
      std::swap(dParamA, dParamB);
      return *this;
    };

    /** \details
       Checks whether parameter A in the 1st intersection object is less than parameter A in the 2nd intersection object.
      
       \param rI1 [in] First intersection object to compare.
       \param rI2 [in] Second intersection object to compare.
       \returns true if parameter A in the 1st intersection object is less than parameter A in the 2nd intersection object, or false otherwise.
    */
    inline static bool lessParamA(const Intersection& rI1, const Intersection& rI2)
    {
      return rI1.dParamA < rI2.dParamA;
    };

    /** \details
       Checks whether parameter B in the 1st intersection object is less than parameter B in the 2nd intersection object.

       \param rI1 [in] First intersection object to compare.
       \param rI2 [in] Second intersection object to compare.
       \returns true if parameter B in the 1st intersection object is less than parameter B in the 2nd intersection object, or false otherwise.
    */
    inline static bool lessParamB(const Intersection& rI1, const Intersection& rI2)
    {
      return rI1.dParamB < rI2.dParamB;
    };

  public:
    /** Intersection point */
    OdGePoint2d ptPoint;

    /** Parameter on the curve A */
    OdDouble dParamA;

    /** Parameter on the curve B */
    OdDouble dParamB;

    /** Type of intersection */
    Types eType;
  };
};

#endif  //__FMCONTOURSBASE_H__
