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

#ifndef __FMSEGMENT2D_H__
#define __FMSEGMENT2D_H__

#include "FMContoursBase.h"
#include <Ge/GePoint2d.h>
#include <Ge/GeVector2d.h>

class OdGeExtents2d;
class OdGeMatrix2d;
class OdGeLine2d;
class OdGeLineSeg2d;
class OdGeCircArc2d;

namespace FacetModeler
{

  class IBulgeSeg2D;

  /** \details
     Represents 2D Segment objects.

     \remarks
     Segment2D is a convex 2D curve given by two points and a bulge value.
     If the two points coincide, the segment has the estCoincident type.
     If bulge==0, the segment is a straight line segment;
     otherwise it is a circular arc, and bulge=tan(Angle/4).
  */
  class FMGEOMETRY_API Segment2D
  {
  public:
    /** \details
       Default constructor.
    */
    Segment2D();

    /** \details
       Constructor. Creates the object with specified start point, end point, bulge, metadata and flags.

       \param ptA       [in] Start point.
       \param ptB       [in] End point.
       \param dBulge    [in] (Optional) Bulge value.
       \param uMetaData [in] (Optional) Metadata attribute.
       \param uRawFlags [in] (Optional) Flags as a raw OdUInt32 value.

       \remarks
       Bulge value = tan(angle/4).
    */
    Segment2D(const OdGePoint2d& ptA, const OdGePoint2d& ptB,
      double dBulge = 0.0, OdIntPtr uMetaData = 0, OdUInt32 uRawFlags = 0);

    /** \details
       Constructor. Creates a segment object using three points.

       \param ptA            [in] Start point.
       \param ptB            [in] End point.
       \param ptIntermediate [in] Intermediate point.
    */
    Segment2D(const OdGePoint2d& ptA,
      const OdGePoint2d& ptB, const OdGePoint2d& ptIntermediate);

    /** \details
       Copy constructor. Creates an Segment2D object and initializes it with the given object and with the specified SegmentImplClass type.

       \param rSeg       [in] Segment2D to copy from.
       \param eImplClass [in] Type of segment implementation.
    */
    Segment2D(const Segment2D & rSeg,
      SegmentImplClass eImplClass = esicBulgeSeg2D);


    /** \details
       Constructor. Creates an object with data from the specified implementation class.

       \param eImplClass [in] Implementation object to copy.
    */
    explicit Segment2D(SegmentImplClass eImplClass);


    /** \details
       Copy constructor. Creates an Segment2D object and initializes it with the given implementation object and with the specified SegmentImplClass type.

       \param rSeg       [in] Implementation object to copy from.
       \param eImplClass [in] Type of segment implementation.
    */
    explicit Segment2D(const IBulgeSeg2D & rSeg,
      SegmentImplClass eImplClass = esicBulgeSeg2D);

    /** \details
       Copy operator. Copies data from the specified Segment2D object.

       \param rSeg [in] Segment to copy.
       \returns Reference to the recomputed segment.
    */
    Segment2D& operator = (const Segment2D& rSeg);

    /** \details
       Copy operator. Copies data from the specified IBulgeSeg2D object.

       \param rSeg [in] Segment implementation to copy.
       \returns Reference to the recomputed segment.
    */
    Segment2D& operator = (const IBulgeSeg2D& rSeg);

    /** \details
       Gets the segment implementation class of this object.

       \returns Segment implementation class.
    */
    SegmentImplClass implClass() const;

    /** \details
       Gets the start point of the segment as OdGePoint2d.

       \returns The start point.
    */
    const OdGePoint2d& startPt() const;

    /** \details
       Gets the end point of the segment as OdGePoint2d.

       \returns The start point.
    */
    const OdGePoint2d& endPt() const;

    /** \details
       Gets the bulge value of the segment.

       \returns The bulge value.
    */
    double bulge() const;

    /** \details
       Gets the segment type of this object.

       \returns Segment type.
    */
    SegmentType type() const;

    /** \details
       Gets the deviation value of the segment.

       \returns The deviation value.
    */
    double deviation() const;

    /** \details
       Gets the direction vector of the segment as OdGeVector2d.

       \param vDir [out] Direction vector.
    */
    void getDir(OdGeVector2d& vDir) const;

    /** \details
       Calculates the distance between the start and end point.

       \returns The distance between the start and end point.
    */
    double dirLength() const;

    /** \details
       Returns the segment length.
    */
    /** \details
       Calculates the segment length.

       \returns Segment length value.
    */
    double length() const;

    /** \details
       Gets the line segment corresponding to this segment.

       \param rLine [out] Resulting linear segment.
       \returns eOk if segment is linear, or erWrongSegType otherwise.

       \remarks
       Only works for line segments.
    */
    Result getLineSeg(OdGeLineSeg2d& rLine) const;

    /** \details
       Gets the arc segment corresponding to this segment.

       \param rArc [out] Resulting circular arc segment.
       \returns eOk if the segment is a circular arc, or erWrongSegType otherwise.

       \remarks
       Only works for arc segments.
    */
    Result getArcSeg(OdGeCircArc2d& rArc) const;

    /** \details
       Gets the point with the specified parameter.

       \param dParam   [in] Parameter of point on the segment.
       \param ptPoint [out] Point on the segment.
       \returns eOk if the point calculated successfully, or a corresponding error type otherwise.

       \remarks
       Point parameter in the range [ 0, 1 ].
    */
    Result getPoint(double dParam,
      OdGePoint2d& ptPoint) const;

    /** \details
       Gets the tangent vector at the specified parameter.

       \param dParam    [in] Parameter for calculation.
       \param vTangent [out] Calculated tangent as OdGeVector2d.
       \returns eOk if the tangent calculated successfully, or a corresponding error type otherwise.

       \remarks
       Point parameter in the range [ 0, 1 ].
    */
    Result getTangent(double dParam,
      OdGeVector2d& vTangent) const;

    /** \details
        Adds extents of this segment (extended by dTol) to the given extents.

       \param extExtents [out] Extents to add.
       \param dTol        [in] Comparing tolerance.
    */
    void addExtents(OdGeExtents2d& extExtents,
      double dTol = 0.0) const;

    /** \details
       Checks whether the point lies within a tolerance on the segment.

       \param ptTest  [in] Point to test.
       \param pParam [out] Resulting parameter.
       \param gTol    [in] Tolerance.
       \returns true if the point lies on the segment, or false otherwise.
    */
    bool isOn(const OdGePoint2d& ptTest, double* pParam = NULL,
      const OdGeTol& gTol = FMGeGbl::gTol) const;

    /** \details
       Calculates the nearest parameter to the specified point.

       \param ptTest     [in] Point to test.
       \param ptNearest [out] Nearest point.
       \returns parameter of a point on the curve that is closest to ptTest.
    */
    double nearestParam(const OdGePoint2d& ptTest,
      OdGePoint2d* ptNearest = NULL) const;

    /** \details
       Gets the signed area of a contour bounded by this segment and two lines
       connecting ptOrigin with ends of this segment.

       \param ptOrigin [in] Point of origin.
       \returns
       2 * signed area of a contour bounded by this segment and two lines
       connecting ptOrigin and the ends of this segment.
    */
    double integrate(const OdGePoint2d& ptOrigin = OdGePoint2d::kOrigin) const;

    /** \details
       Returns the subsegment with two specified parameters.

       \param dStartParam [in] Start parameter.
       \param dEndParam   [in] End parameter.
       \param rSubSeg    [out] Resulting segment.
       \returns eOk if the subsegment calculated successfully, or a corresponding error type otherwise.
    */
    Result getSubSegment(double dStartParam, double dEndParam,
      IBulgeSeg2D & rSubSeg) const;

    /** \details
       Gets the parallel segment.

       \param dNormalOffset [in] Normal offset.
       \param rParSeg      [out] Resulting segment.
       \returns eOk if the parallel segment calculated successfully, or a corresponding error type otherwise.
    */
    Result getParallelSegment(double dNormalOffset,
      IBulgeSeg2D& rParSeg) const;

    /** \details
       Gets the number of different intersection points of the segment
       (including end points with minimal Y) with a ray, starting at ptTest and
       going in the X direction.

       \param ptTest      [in] Point to test.
       \param pOnSegment [out] The result of checking whether a point lies on the segment.
       \param gTol        [in] Tolerance.
       \returns The number of different intersection points.

       \remarks
       pOnSegment is set to true if the test point lies on the segment.
    */
    OdUInt32 intersectXRay(const OdGePoint2d& ptTest, bool* pOnSegment = NULL,
      const OdGeTol& gTol = FMGeGbl::gTol) const;

    /** \details
       Gets the number of intersections with the specified segment.

       \param rSegB [in] Segment to check for intersections with.
       \param pPt1 [out] (Optional) First intersection point.
       \param pPt2 [out] (Optional) Second intersection point.
       \param gTol  [in] Tolerance.
       \returns The number of intersection points.
    */
    OdUInt32 intersect(const IBulgeSeg2D& rSegB,
      OdGePoint2d* pPt1, OdGePoint2d* pPt2 = NULL,
      const OdGeTol& gTol = FMGeGbl::gTol) const;

    /** \details
       Gets the number of intersections with the specified segment.

       \param rSegB  [in] Segment to check for intersections with.
       \param pInt1 [out] (Optional) First intersection object.
       \param pInt2 [out] (Optional) Second intersection object.
       \param gTol   [in] Tolerance.
       \returns The number of intersection points.
    */
    OdUInt32 intersect(const IBulgeSeg2D& rSegB,
      Intersection* pInt1 = NULL, Intersection* pInt2 = NULL,
      const OdGeTol& gTol = FMGeGbl::gTol) const;

    /** \details
       Gets the number of intersections with the specified line.

       \param rGeLine [in] Line to check for intersections with.
       \param pInt1  [out] (Optional) First intersection object.
       \param pInt2  [out] (Optional) Second intersection object.
       \param gTol    [in] Tolerance.
       \returns The number of intersection points.
    */
    OdUInt32 intersectLine(
      const OdGeLine2d& rGeLine,
      Intersection* pInt1 = NULL, Intersection* pInt2 = NULL,
      const OdGeTol& gTol = FMGeGbl::gTol) const;


    /** \details
       Gets the center of the arc.

       \returns Arc center point as OdGePoint2d.

       \remarks
       Only works for arc segments.
    */
    OdGePoint2d arcCenter() const;

    /** \details
       Gets the radius of the arc.

       \returns Radius value.

       \remarks
       Only works for arc segments.
    */
    double arcRadius() const;

    /** \details
       Gets the arc radius and center point.

       \param ptCenter [out] Center of the circular arc.
       \param dRadius  [out] Radius of the circular arc.
       \returns eOk if the arc is circular, or a corresponding error type otherwise.

       \remarks
       Only works for arc segments.
    */
    Result getCircleData(OdGePoint2d& ptCenter, double& dRadius) const;

    /** \details
       Gets the signed included angle of the arc segment.

       \returns Angle value.

       \remarks
       The returned value belongs in the range (-2*Pi, 2*Pi).
    */
    double arcAngle() const;

    /** \details
       Gets the start angle of the arc (direction from center to startPt).

       \returns Angle value.

       \remarks
       The returned value belongs in the range (0, 2*Pi).
    */
    double arcStartAngle() const;

    /** \details
       Gets the arc center point, radius and angles.

       \param ptCenter    [out] Center of the circular arc.
       \param dRadius     [out] Radius of the circular arc.
       \param dStartAngle [out] Start angle of the circular arc.
       \param dInclAngle  [out] End angle of the circular arc.
       \returns eOk if the arc is circular, or a corresponding error type otherwise.
    */
    Result getArcData(OdGePoint2d& ptCenter, double& dRadius,
      double& dStartAngle, double& dInclAngle) const;

    /** \details
       Transforms the segment using the specified matrix.

       \param mTrans [in] Transformation matrix.
       \returns eOk if the transformation succeeded, or a corresponding error type otherwise.
    */
    Result transformBy(const OdGeMatrix2d& mTrans);

    /** \details
       Inverts the segment so it is in the opposite direction.

       \returns eOk if the reversal succeeded, or a corresponding error type otherwise.
    */
    Result reverse();

    /** \details
       Updates the object with the specified start point, end point, bulge, metadata and flags.

       \param ptA       [in] Start point.
       \param ptB       [in] End point.
       \param dBulge    [in] (Optional) Bulge value.
       \param uMetaData [in] (Optional) Metadata attribute.
       \param uRawFlags [in] (Optional) Flags as raw OdUInt32 value.
       \returns eOk if the update succeeded, or a corresponding error type otherwise.

       \remarks
       Bulge value = tan(angle/4).
    */
    Result set(const OdGePoint2d& ptA, const OdGePoint2d& ptB, double dBulge = 0.0,
      OdIntPtr iMetaData = 0, OdUInt32 iRawFlags = 0);

    /** \details
       Updates end points and calculates the bulge using the specified intermediate point.

       \param ptA            [in] Start point.
       \param ptB            [in] End point.
       \param ptIntermediate [in] Intermediate point.
       \returns eOk if the update succeeded, or a corresponding error type otherwise.
    */
    Result set(const OdGePoint2d& ptA, const OdGePoint2d& ptB, const OdGePoint2d& ptIntermediate);

    /** \details
       Copies data from the specified GeLineSeg2d and resets metadata.

       \param geLine [in] Linear segment to copy data from.
       \returns eOk if the update succeeded, or a corresponding error type otherwise.
    */
    Result set(const OdGeLineSeg2d& geLine);

    /** \details
       Copies data from the specified GeCircArc2d and resets metadata.

       \param geArc [in] Circular arc segment to copy data from.
       \returns eOk if the update succeeded, or a corresponding error type otherwise.
    */
    Result set(const OdGeCircArc2d& geArc);

    /** \details
       Copies data from another segment.

       \param rSeg [in] Segment to copy data from.
       \returns eOk if the update succeeded, or a corresponding error type otherwise.
    */
    Result set(const IBulgeSeg2D& rSeg);

    /** \details
       Shifts the segment in the direction specified by the angle and distance.

       \param dAngle     [in] Shear direction angle.
       \param dShiftDist [in] Shear distance.
       \returns eOk if the shift succeeded, or a corresponding error type otherwise.
    */
    Result shift(const double dAngle, const double dShiftDist);

    /** \details
       Gets metadata.

       \returns Pointer to the metadata object if it exists, or NULL otherwise.
    */
    OdIntPtr metadata() const;

    /** \details
       Sets metadata.

       \param iNewData [in] Pointer to a new metadata object.
       \returns eOk if the set succeeded, or a corresponding error type otherwise.
    */
    Result setMetadata(OdIntPtr iNewData);

    /** \details
       Gets the ecfHidden flag value.

       \returns ecfHidden flag value as bool.
    */
    bool isHidden() const;

    /** \details
       Sets or clears the ecfHidden flag.

       \param bHidden [in] New value of the ecfHidden flag.
       \returns eOk if the set succeeded, or a corresponding error type otherwise.
    */
    Result setHidden(bool bHidden = true);

    /** \details
       Gets the orientation of the segment.

       \returns Orientation as a value from the FaceOrientation enum.
    */
    FaceOrientation orientation() const;

    /** \details
       Sets the orientation of the segment.

       \param eOrientation [in] New orientation value.
       \returns eOk if the set succeeded, or a corresponding error type otherwise.
    */
    Result setOrientation(FaceOrientation eOrientation);

    /** \details
       Gets the ecfExplodedArc flag value.

       \returns ecfExplodedArc flag value as bool.
    */
    bool isExplodedArc() const;

    /** \details
       Sets or clears the ecfExplodedArc flag.

       \param bExplodedArc [in] New value of the ecfExplodedArc flag.
       \returns eOk if the set succeeded, or a corresponding error type otherwise.
    */
    Result setExplodedArc(bool bExplodedArc = true);

    /** \details
       Gets internal flags (orientation, visibility etc.).

       \returns Internal flags as OdUInt32.
    */
    OdUInt32 rawFlags() const;

    /** \details
       Sets internal flags (orientation, visibility etc.).

       \param iRawFlags [in] New value of internal flags to set.
       \returns eOk if the set succeeded, or a corresponding error type otherwise.
    */
    Result setRawFlags(OdUInt32 iRawFlags);
    
    /** \details 
      Destructor.
    */
    ~Segment2D();

  public:
    //DOM-IGNORE-BEGIN
    inline operator const IBulgeSeg2D& () const { return *m_pImpl; }
    inline operator IBulgeSeg2D& () { return *m_pImpl; }
    inline const IBulgeSeg2D& impl() const { return *m_pImpl; }
    inline IBulgeSeg2D& impl() { return *m_pImpl; }
    //DOM-IGNORE-END
  private:

    /** Pointer to the implementation object. */
    IBulgeSeg2D* m_pImpl;
  };

}; // namespace FacetModeler

#endif  //__FMSEGMENT2D_H__
