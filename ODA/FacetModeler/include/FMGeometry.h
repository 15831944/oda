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

#ifndef __FMGEOMETRY_H__
#define __FMGEOMETRY_H__

#include <OdaCommon.h>
#include "FMBuildSettings.h"
#include "FMSTLHelper.h"
#include <Ge/GeTol.h>
#include <Ge/GePlane.h>
#include <Ge/GeMatrix2d.h>
#include <Ge/GeLine2d.h>

#define FMGE_ANGLE_TO_MINUSPI_PI(x) ( (x>OdaPI)?(x-Oda2PI):(x) ) 

namespace FacetModeler
{
  /** \details
     Represents deviation parameters.
  */
  struct FMGEOMETRY_API DeviationParams
  {
    /** \details
       Default constructor. Creates a DeviationParams object with default parameters.
    */
    DeviationParams();

    /** \details
       Constructor. Creates a DeviationParams object with the specified parameters.
      
       \param deviation    [in] Maximum deviation value.
       \param maxpercircle [in] Maximum facets per circle.
       \param minpercircle [in] Minimum facets per circle.
    */
    DeviationParams(double deviation, OdUInt16 maxpercircle, OdUInt16 minpercircle);

    /** Maximal allowed deviation. */
    double Deviation;
    /** Maximum facets per full circle */
    OdUInt16 MaxPerCircle;
    /** Minimum facets per full circle */
    OdUInt16 MinPerCircle;
  };

  /** \details
     Defines types of boolean operations.
  */
  enum BooleanOperation
  {
    /** Union operation. */
    eUnion = 0,

    /** Intersection operation. */
    eIntersection = 1,

    /** Difference operation. */
    eDifference = 2,

    /** XOR operation. */
    eXOR = 3
  };

  /** \details 
    Returns a name by the specified type of boolean operations.
  
    \param eOperation [in] Boolean operation type from BooleanOperation enumeration.
    \returns Name of operation as OdString object.
  */
  FMGEOMETRY_API OdString GetBoolOperationName(
    BooleanOperation eOperation);

  /** \details
     Defines classification results.
  */
  enum Classification
  {
    /** Unspecified. */
    eNo = 0,

    /** Lies on. */
    eOn = 1,

    /** Inner position. */
    eInner = 2,

    /** Outer position. */
    eOuter = 3,

    /** Spanning. */
    eSpanning = 4
  };

  /** \details
     Defines face orientations.

     \remarks
     The default orientation is treated as efoFront.
     Should not be used.
  */
  enum FaceOrientation
  {
    /** Default orientation. */
    efoDefault = 0x00,

    /** Placed on the top side. */
    efoTop = 0x01,

    /** Placed on the bottom side. */
    efoBottom = 0x02,

    /** Placed on the left side. */
    efoLeft = 0x04,

    /** Placed on the right side. */
    efoRight = 0x08,

    /** Placed on the front side. */
    efoFront = 0x10,

    /** Placed on the back side. */
    efoBack = 0x20,

    /** Discarded orientation. */
    efoDiscard = 0x80
  };

  /** \details
     Represents the Facet Modeler global object and methods.
  */
  struct FMGEOMETRY_API FMGeGbl
  {
    /** \details
       Default deviation parameters.

       \remarks
       By default the deviation is 0.5,
       maximum facets per full circle is 128, and
       minimum facets per full circle is 8.
    */
    FMGEOMETRY_API_STATIC static DeviationParams gDefDev;

    /** \details
       Tolerance used in mesh operations.

       \remarks
       The default value for the tolerance is 1E-6.
    */
    FMGEOMETRY_API_STATIC static OdGeTol gTol;

    /** \details
       Returns the number of linear segments for an exploded arc,
       specified by a sweep angle and radius.
          
       \param dAngle       [in] Sweep angle.
       \param dRadius      [in] Circle radius.
       \param devDeviation [in] Deviation parameters.
       \returns The number of linear segments as OdUInt16.
    */
    FMGEOMETRY_API_STATIC static OdUInt16 GetSegmentCount(double dAngle, double dRadius,
      const DeviationParams& devDeviation);

    /** \details
       Returns a plane orientation by the specified plane normal.
      
       \param normal [in] Plane normal.
       \returns Plane orientation as FaceOrientation.
    */
    static FaceOrientation PlaneOrientation(const OdGeVector3d& normal);

    /** \details
       Inverts the specified orientation.
      
       \param eOri [in] Orientation to invert.
       \returns Inverted orientation as FaceOrientation. 
    */
    FMGEOMETRY_API_STATIC static FaceOrientation
      GetReversedOrientation(FaceOrientation eOri);

  };

  inline FaceOrientation FMGeGbl::PlaneOrientation(const OdGeVector3d& normal) {
    if (normal.isParallelTo(OdGeVector3d::kXAxis)) {
      return normal.x > 0. ? efoRight : efoLeft;
    }
    else if (normal.isParallelTo(OdGeVector3d::kYAxis)) {
      return normal.y > 0. ? efoBack : efoFront;
    }
    else if (normal.isParallelTo(OdGeVector3d::kZAxis)) {
      return normal.z > 0. ? efoTop : efoBottom;
    }
    return efoDefault;
  }

}

#endif  //__FMGEOMETRY_H__
