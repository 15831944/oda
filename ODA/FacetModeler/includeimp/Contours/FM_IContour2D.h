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
#ifndef __FM_ICONTOUR2D_H__
#define __FM_ICONTOUR2D_H__

#include "FMGeometry.h"
#include "Contours/FM_IBulgeSeg2D.h"

class OdGeLineSeg2d;
class OdGeCircArc2d;


namespace FacetModeler
{


class IContour2D
{
public:

    virtual ~IContour2D() { };


    // Copy this contour from rSrcCont
    virtual void set( const IContour2D & rSrcCont ) = 0;

    // Create a new copy of this contour.
    // Call delete to release memory
    virtual IContour2D * clone() const = 0;


    // returns class info at runtime
    virtual ContourImplClass implClass() const = 0;

    // Create a contour of given class (ecicSimple or ecicCached ) and copy pSrcContour to it
    static IContour2D * create( ContourImplClass eClass, const IContour2D * pSrcContour = 0 );

    //////////////////////////////////////////////////////////////////////////
    //

    // Returns true if this entity is closed, false otherwise.
    virtual bool isClosed() const = 0;

    // Makes the contour closed (open).
    // This method doesn't change vertex count.
    // In open contours the last bulge value is unused.
    virtual void setClosed(bool bClosed = true) = 0;

    // Removes the last vertex (point, bulge and metadata) and makes contour closed 
    // if the last and the first points are equal. Does nothing otherwise.
    // Returns true, if the vertex was removed and the contour was closed
    virtual bool setClosedIfEndsAreEqual( const OdGeTol& gTol = FMGeGbl::gTol ) = 0;

    // returns true, if there is no vertices
    virtual bool isEmpty() const = 0;

    // returns number of vertices
    virtual OdUInt32 numVerts() const = 0;

    // returns number of segments
    // It is less than or equal (if the contour is closed) to the number of vertices.
    virtual OdUInt32 numSegments() const = 0;


    // Returns true, if the contour contains at least one arc
    virtual bool hasArcs() const = 0;




    //////////////////////////////////////////////////////////////////////////
    //
    // 1. Simple Segment and Vertex operations
    //



    // Returns the type of the segment that begins at the specified vertex.
    // If the index is out of range estUnknown is returned
    // Otherwise one of estArc, estLine, estCoincident is returned
    virtual SegmentType segmentType (OdUInt32 iIndex ) const = 0;


    // returns IBulgeSeg2D
    virtual Result getSegmentAt (OdUInt32 iIndex, IBulgeSeg2D & rSegment ) const = 0;

    // Returns coordinates of segment ends and a bulge value
    virtual Result getSegmentAt (OdUInt32 iIndex,
            OdGePoint2d & ptStart,
            OdGePoint2d & ptEnd,
            double & dBulge ) const = 0;

    // Returns OdGeLineSeg2d corresponding to the segment given by iIndex
    // Error code will be returned, if the segment's type is estArc
    virtual Result getLineSegAt (OdUInt32 iIndex, OdGeLineSeg2d& geLine ) const = 0;

    // Returns OdGeCircArc2d corresponding to the segment given by iIndex
    // Error code will be returned, if the segment's type is not estArc
    virtual Result getArcSegAt  (OdUInt32 iIndex, OdGeCircArc2d& geArc  ) const = 0;


    // Returns a point given by iIndex
    virtual Result getPointAt (OdUInt32 iIndex, OdGePoint2d & ptPoint ) const = 0;

    // Returns a bulge value of the segment given by the iIndex
    virtual Result getBulgeAt (OdUInt32 iIndex, double & dBulge ) const = 0;

    // Returns vertex data: starting point, bulge and metadata of corresponding segment
    virtual Result getVertexAt(OdUInt32 iIndex, OdGePoint2d * pptPoint, double * pdBulge = 0, Attributes2D * pAttr = 0 ) const = 0;

    // Returns internal angle of vertex vectors, given by the iIndex
    virtual Result getInternalAngleAt( OdUInt32 iIndex, double & dAngle ) const = 0;

    // Updates existing point coordinates
    virtual Result setPointAt (OdUInt32 iIndex, const OdGePoint2d & ptPoint ) = 0;

    // Updates existing bulge value
    virtual Result setBulgeAt (OdUInt32 iIndex, double dBulge ) = 0;

    // Updates existing point and bulge values
    virtual Result setVertexAt (OdUInt32 iIndex, const OdGePoint2d & ptPoint, double dBulge ) = 0;

    // Reset the contour
    virtual void   reset() = 0;

    // Makes capacity of the vertex vector >= iReservedSize
    // Number of vertices is not changed
    virtual void reserveVertices( OdUInt32 iReservedSize ) = 0;

    // Returns attributes for given segment
    virtual const Attributes2D& attributesAt(OdUInt32 iIndex ) const = 0;

    // Returns attributes for given segment for update
    virtual       Attributes2D& attributes4UAt(OdUInt32 iIndex ) = 0;

    // Updates attributes for given segment
    virtual Result setAttributesAt(OdUInt32 iIndex, const Attributes2D& rAttr ) = 0;

    // Returns metadata for given segment or 0
    virtual OdIntPtr metadataAt (OdUInt32 iIndex ) const = 0;
    
    // Updates metadata for given segment
    virtual Result setMetadataAt (OdUInt32 iIndex, OdIntPtr uNewData ) = 0;

    // Adds a vertex at the specified index.
    // The index should be <= numVerts()
    // If it is equal to numVerts(), a new vertex will be appended to the array,
    // otherwise it'll be inserted before the old iIndex vertex.
    virtual Result addVertexAt    (OdUInt32 iIndex, const OdGePoint2d& ptStart, double dBulge = 0.0, const Attributes2D& rAttr = Attributes2D::kNull ) = 0;

    // Appends a new starting point and a bulge value.
    // The bulge value relates to the segment starting at the point.
    virtual Result appendVertex   (const OdGePoint2d& ptStart, double dBulge = 0.0, const Attributes2D& rAttr = Attributes2D::kNull ) = 0;

    // Appends vertex data from vecSource to this contour
    virtual Result appendVertices ( const OdGePoint2dArray & vecSource ) = 0;

    // Appends vertex data from vecSource and bulgeSource with some size to this contour
    virtual Result appendVertices(OdUInt32 size, const OdGePoint2d* vecSource, const double* bulgeSource = NULL) = 0;

    // Appends rSeg to this contour. 
    // Returns erPointNotOnThis if rSeg.startPt() is not equal to the end of this (open) contour
    // if bShiftToHit is true, the rSeg is shifted and the return code is erWarnPointNotOnThis
    // Note: if the contour is closed, the last (closing) segment is wiped out
    virtual Result appendSegment  ( const IBulgeSeg2D & rSeg, bool bShiftToHit = false, const OdGeTol& gTol = FMGeGbl::gTol ) = 0;

    // Appends new segment to this contour ( updates bulge and metadata at the last vertex and 
    // appends new vertex with zero bulge and data)
    // Returns erEmptyContour if there are no vertices in the contour
    // Note: if the contour is closed, the last (closing) segment is wiped out
    virtual Result appendSegment  (double dBulge, const OdGePoint2d& ptNewEnd, const Attributes2D& rAttr = Attributes2D::kNull ) = 0;

    // Appends rCont to this contour ( "closed" flags are ignored )
    // Returns erPointNotOnThis if rCont.startPoint is not equal to the end of this (open) contour
    // If bCloseGap is true, a segment, connecting the points is inserted and
    //   the return code is erWarnPointNotOnThis
    virtual Result appendContour  ( const IContour2D & rCont, bool bCloseGap = false, double dMaxGap = 1e99 ) = 0;

    // Removes a vertex (and a corresponding segment).
    // Bulge values of the other segments are left intact.
    virtual Result removeVertexAt (OdUInt32 iIndex ) = 0;

    //////////////////////////////////////////////////////////////////////////
    //
    // 2. Parameterization and arbitrary point access
    //


    // There are 3 ways to pick some (non-vertex) point on the contour:
    // a) use an integer index to get a segment and then
    //    use parameter on the segment (GeLine or GeArc).
    // b) use a distance along the curve from the starting point.
    // c) use a "parameter" value.
    //
    // The last way is the fastest (and preferred in most cases) because of
    // the "parameter" structure:
    //   dParam = startParam() + iIndex * (dOffsetOnSegment/dSegmentLength)
    //
    // So the point given by parameter value can be found at constant time.

    inline double startParam() const { return 0.0; };

    inline double endParam  () const { return numSegments(); };

    // Returns starting point of the contour, if the contour is not empty
    virtual Result getStartPoint(OdGePoint2d & ptPoint ) const = 0;

    // Returns ending point of the contour, if the contour is not empty.
    // The ptPoint is equal to the starting point, if the contour is closed.
    virtual Result getEndPoint  (OdGePoint2d & ptPoint ) const = 0;


    // Returns the length of the contour
    virtual double length() const = 0;


    // returns tangent vector at the parameter
    virtual Result getTangent     (double dParam, OdGeVector2d& vTangent ) const = 0;

    // Returns a point given by a parameter value
    virtual Result getPoint       (double dParam, OdGePoint2d& ptPoint ) const = 0;

    // Returns a point given by a distance along the curve from the starting point
    virtual Result getPointAtDist (double dDist,  OdGePoint2d& ptPoint ) const = 0;

    // Converts parameter value to the distance from start along the curve
    virtual Result getDistAtParam (double dParam, double& dDist ) const = 0;

    // Converts distance from start to parameter value
    virtual Result getParamAtDist (double dDist,  double& dParam ) const = 0;


    //virtual Result getParamAtPoint(const OdGePoint2d& ptPoint, double& dParam) const = 0;
    //virtual Result getDistAtPoint (const OdGePoint2d& ptPoint, double& dDist) const = 0;


    //////////////////////////////////////////////////////////////////////////
    // Distance along the contour and closed contours

    // Convert dParam to [0, endParam() ) range (on closed contours)
    // and returns erOk, if the dParam  is valid
    virtual Result getNormalizedParam( double & dParam ) const = 0;

    // Convert dDist to [0, length() ) range (on closed contours)
    // and returns erOk, if the dDist is valid
    virtual Result getNormalizedDist( double & dDist ) const = 0;


    // Returns true, if the parameters are valid and equal (after normalization) 
    //   The result is useless, if some parameter is not valid
    virtual bool areEqualParams( double dParam1, double dParam2, const double dParamTol = 1e-10 ) const = 0;

    // Returns true, if the distances are equal (after normalization) with given tolerance
    //   The result is useless, if some parameter is not valid
    virtual bool areEqualDists ( double dDist1, double dDist2, const double dTol = FMGeGbl::gTol.equalPoint() ) const = 0;


    // Get signed length of the shortest way btw the points 1 and 2.
    // The points are given by distances along the curve
    virtual double signedMinDist        ( double dDist1, double dDist2 ) const = 0;

    // Get signed length of the shortest way btw the points 1 and 2.
    // The points are given by parameters on the curve
    virtual double signedMinDistByParams( double dParam1, double dParam2 ) const = 0;



    //////////////////////////////////////////////////////////////////////////
    //
    // 3. Geometrical Queries
    //


    // Adds geometrical extents of the contour to geExtents
    virtual Result addExtents(OdGeExtents2d& geExtents ) const = 0;


    // Returns signed area of the contour. (positive for CCW contours)
    // In open contours the first and last vertices are considered
    // to be connected with a straight line.
    virtual double signedArea() const = 0;

    virtual bool isConvex() const = 0;

    inline double area() const { return ::fabs( signedArea() ); };

    inline bool isCCW() const { return ( signedArea() >= 0.0 ); };

    // returns internal point of the closed contour (contour orientation is ignored)
    // If succeeded this->contains( rPoint, gTol ) will return true
    virtual Result getInternalPoint( OdGePoint2d& rPoint, const OdGeTol& gTol = FMGeGbl::gTol ) const = 0;

    // returns true, if the point lies on the contour
    // *dParam stores the parameter of the point
    virtual bool isOn( const OdGePoint2d & ptTest, double * pdParam = 0,
        const OdGeTol & gTol = FMGeGbl::gTol ) const = 0;

    // returns parameter of a point on the curve, which is closest to the ptTest
    virtual double nearestParam( const OdGePoint2d & ptTest, OdGePoint2d * ptNearest = 0 ) const = 0;


    // Returns true if the point lies inside the contour or on it's border.
    // Contour must be closed.
    // pbOnBorder is true, if the point lies on the border of the contour
    // Note: the contour is considered to be non-oriented.
    virtual bool contains( const OdGePoint2d &rPoint, bool * pbOnBorder = 0,
        const OdGeTol & gTol = FMGeGbl::gTol) const = 0;


    // Returns whether the contour intersects itself.
    // Works O( N log N ) at the average
    virtual bool isSelfIntersecting(const OdGeTol& gTol = FMGeGbl::gTol, bool bExcludeTouch = false  ) const = 0;

    // Returns whether this contour is closed and coincides with a directional 
    // boundary of some open 2D-region.
    virtual bool isValidRegion( const OdGeTol& gTol = FMGeGbl::gTol ) const = 0;

    virtual bool isInsideContour(const IContour2D& c2dOuter, bool fAllowBordersTouch = false, const OdGeTol& gTol = FMGeGbl::gTol) const = 0;


    // Returns number of intersections
    // and appends Intersection data to the vecPoints vector
    // Note: some intersection points (esp. on segment bounds) can coincide
    virtual OdUInt32 intersect( const IBulgeSeg2D & rSegB,
        std::vector< Intersection > & vecPoints,
        const OdGeTol & gTol = FMGeGbl::gTol ) const = 0;

    // Returns number of intersections
    // and appends Intersection data to the vecPoints vector
    // Note: some intersection points (esp. on segment bounds) can coincide
    virtual OdUInt32 intersect( const IContour2D& rContB,
        std::vector< Intersection > & vecPoints,
        const OdGeTol & gTol = FMGeGbl::gTol ) const = 0;

    // Intersects this contour with an infinite line
    // Returns number of intersections
    // and APPENDS Intersection data to the vecPoints vector
    // Note: some intersection points (esp. on segment bounds) can coincide
    virtual OdUInt32 intersectLine( 
        const OdGePoint2d& ptLineOrigin, 
        const OdGeVector2d& vLineDir,
        std::vector< Intersection > & vecPoints,
        const OdGeTol & gTol = FMGeGbl::gTol ) const = 0;


    //////////////////////////////////////////////////////////////////////////
    //
    // 4. Transformations
    //

    // Transforms the contour
    virtual Result transformBy(const OdGeMatrix2d& geMatrix,
        const DeviationParams& devDeviation = FMGeGbl::gDefDev ) = 0;

    // Reverses the contour.
    virtual void reverse() = 0;

    // Deletes redundant segments that have length = 0.
    virtual void deleteCoincident( const OdGeTol & gTol = FMGeGbl::gTol ) = 0;

    // Merges adjacent segments that lie on the same line or arc
    virtual void mergeSegments( int iMergeFlags = 0, const OdGeTol & gTol = FMGeGbl::gTol  ) = 0;

    // Explodes all arc segments to lines.
    // Note: &rDestCont == this IS OK!
    virtual Result explodeTo( IContour2D & rDestCont, const DeviationParams& devDeviation, 
        OdIntPtr uArcMetadata = 0 ) const = 0;


    //////////////////////////////////////////////////////////////////////////


    // Adds a vertex corresponding to the specified parameter into the contour, if
    // the vertex does not exist.
    // Returns index of the created/existing vertex.
    // Note: insertion/deletion of a vertex at dParam shifts all parameters > dParam by 1.0
    virtual OdUInt32 createVertexAt( double dParam, const OdGeTol & gTol = FMGeGbl::gTol ) = 0;

    // Adds vertices at the specified parameters into the contour, if
    // the vertices do not exist.
    virtual Result createVerticesAt( const std::vector<double> & vecParams, const OdGeTol & gTol = FMGeGbl::gTol ) = 0;

    virtual Result createVerticesAt( OdUInt32 size, const double* vecParams, const OdGeTol & gTol = FMGeGbl::gTol ) = 0;

    // returns sub-contour given by 2 parameters
    // Note: if (dStartParam > dEndParam) the rSubContour will have opposite direction
    virtual Result getSubContour(double dStartParam, double dEndParam,
        IContour2D & rSubContour, const OdGeTol & gTol = FMGeGbl::gTol ) const = 0;

    // appends sub-contour to rSubContour, if possible
    virtual Result appendSubContourTo(double dStartParam, double dEndParam,
        IContour2D & rSubContour, const OdGeTol & gTol = FMGeGbl::gTol ) const = 0;

    // Divides this contour into sets of contours that lie to the left of the given line and to the right.
    // Note: if OdGeLinearEnd2d can be an infinite of a finite line, depending
    //   on it`s class: OdGeLine2d or OdGeLineSeg2d
    //virtual Result splitByLine( const OdGeLinearEnt2d& rLine,
    //    AECProfile& rLeftSide, AECProfile& rRightSide,
    //    const OdGeTol & gTol = FMGeGbl::gTol ) const = 0;


    // replaces part of this contour with the given contour
    // rSubContour should be an open curve.
    // Directions are significant! I.e. if this one is closed, the remaining half is choosen
    // using rSubContour direction.
    virtual Result replaceSubContourTo( IContour2D& rDest, const IContour2D & rSubContour, const OdGeTol & gTol = FMGeGbl::gTol ) const = 0;

};





};


#endif  //__FM_ICONTOUR2D_H__
