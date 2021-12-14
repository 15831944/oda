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
#ifndef __FM_IBULGESEG2D_H__
#define __FM_IBULGESEG2D_H__

#include "FMGeometry.h"
#include "Contours/FMContoursBaseImp.h"

#include "Ge/GePoint2d.h"
#include "Ge/GeVector2d.h"

class OdGeExtents2d;
class OdGeMatrix2d;
class OdGeLineSeg2d;
class OdGeCircArc2d;


namespace FacetModeler
{


//////////////////////////////////////////////////////////////////////////
// IBulgeSeg2D is a convex 2-D curve given by 2 points and a bulge value
// If the 2 points coincide, the segment has estCoincident type
// Otherwise, if bulge==0, the segment is a straight line segment
//            otherwise it is a circular arc, and bulge=tan(Angle/4)
//

class FMGEOMETRY_API IBulgeSeg2D
{
public:
    virtual ~IBulgeSeg2D() { };
                 
    // Create a segment of given class (esicBulgeSeg2D  or esicCachedSeg2D ) and copy pSrcSeg to it
    static IBulgeSeg2D * create( SegmentImplClass eClass, const IBulgeSeg2D * pSrcSeg = 0 );

    // returns implementation class information
    virtual SegmentImplClass implClass() const = 0;

    //////////////////////////////////////////////////////////////////////////
    // const methods

    // returns reference to the starting point
    virtual const OdGePoint2d & startPt() const = 0;

    // returns reference to the ending point
    virtual const OdGePoint2d & endPt  () const = 0;

    // returns bulge value
    virtual double              bulge  () const = 0;


    //////////////////////////////////////////////////////////////////////////
    // const methods

    // returns the type of this segment
    virtual SegmentType type() const = 0;

    // returns signed deviation from the line [start, end]
    // the deviation has the same sign, as the bulge() value
    virtual double deviation() const = 0;


    // returns vDir = endPt - startPt
    virtual void   getDir ( OdGeVector2d & vDir ) const = 0;

    // returns distance btw startPt and endPt
    virtual double dirLength( ) const = 0;

    // returns length of the segment
    virtual double length ( ) const = 0;



    // Returns OdGeLineSeg2d corresponding to this segment, if type()!=estArc
    virtual Result getLineSeg ( OdGeLineSeg2d& geLine ) const = 0;

    // Returns OdGeCircArc2d corresponding to this segment, if type()==estArc
    virtual Result getArcSeg  ( OdGeCircArc2d& geArc ) const = 0;



    // returns point at the parameter [ 0, 1 ]
    virtual Result getPoint  ( double dParam, OdGePoint2d & ptPoint  ) const = 0;

    // returns tangent vector at the parameter [ 0, 1 ]
    virtual Result getTangent( double dParam, OdGeVector2d& vTangent ) const = 0;


    // Adds extents of this segment (extended by dTol) to the given extents.
    virtual void addExtents( OdGeExtents2d& rExtents, double dTol = 0.0 ) const = 0;


    // returns true, if the point lies on the segment
    // *dParam stores the parameter of the point
    virtual bool isOn( const OdGePoint2d & ptTest, double * pdParam = 0,
        const OdGeTol & gTol = FMGeGbl::gTol ) const = 0;


    // returns parameter of a point on the curve, which is closest to the ptTest
    virtual double nearestParam( const OdGePoint2d & ptTest, OdGePoint2d * ptNearest = 0 ) const = 0;


    // returns signed area of a contour bounded by this segment and 2 lines,
    //   connecting the ptOrigin and ends of this segment.
    virtual double integrate( const OdGePoint2d & ptOrigin = OdGePoint2d::kOrigin ) const = 0;


    // returns subsegment given by 2 parameters
    // Note: if (dStartParam > dEndParam) the rSubSeg will have opposite direction
    virtual Result getSubSegment(double dStartParam, double dEndParam, IBulgeSeg2D & rSubSeg ) const = 0;


    // returns parallel segment.  
    // dNormalOffset - signed offset along normal vector at each point of original segment
    // returns error, if source segment is coincident or the offset is too big to get 
    //  valid result
    virtual Result getParallelSegment( double dNormalOffset, IBulgeSeg2D & rParSeg ) const = 0;

    // The method returns number of different intersection points of
    // the segment (including end with minimal Y) with a ray, starting at ptTest and
    // going in the X direction.
    // *pbOnSegment is set to true, if the test point lies on the segment.
    virtual OdUInt32 intersectXRay( const OdGePoint2d& ptTest, bool * pbOnSegment = 0,
        const OdGeTol & gTol = FMGeGbl::gTol ) const = 0;


    // Returns number of intersections (0, 1 or 2) and intersection points.
    // ToDo: comments on endpoints
    virtual OdUInt32 intersect( const IBulgeSeg2D & rSegB,
        OdGePoint2d * pPt1, OdGePoint2d * pPt2 = 0,
        const OdGeTol & gTol = FMGeGbl::gTol ) const = 0;

    // Returns number of intersections (0, 1 or 2) and Intersection objects.
    // ToDo: comments on endpoints
    virtual OdUInt32 intersect( const IBulgeSeg2D & rSegB,
        Intersection * pInt1 = 0, Intersection * pInt2 = 0,
        const OdGeTol & gTol = FMGeGbl::gTol ) const = 0;

    // Intersects this segment with an infinite line
    // Returns number of intersections
    virtual OdUInt32 intersectLine( 
        const OdGePoint2d& ptLineOrigin, const OdGeVector2d& vLineDir,
        Intersection * pInt1 = 0, Intersection * pInt2 = 0,
        const OdGeTol & gTol = FMGeGbl::gTol ) const = 0;

    // This method is used internally in intersect, but it can be used separately 
    //   to perform a fast check of multiple points.
    // ppInt must be a null-terminated array of pointers to Intersection.
    // The function fills dParamA fields and updates eType field
    // Returns number of points, lying on this segment
    virtual OdUInt32 areOn( Intersection ** ppInt, const OdGeTol & gTol = FMGeGbl::gTol ) const = 0;

    //////////////////////////////////////////////////////////////////////////
    // Arcs and circles ( length>0 and bulge != 0 )


    // returns arc center, if type()==estArc
    virtual OdGePoint2d arcCenter() const = 0;

    // returns arc radius, if type()==estArc
    virtual double      arcRadius( ) const = 0;

    // returns arc radius and center point, if type()==estArc
    virtual Result getCircleData( OdGePoint2d & ptCenter, double & dRadius ) const = 0;

    // returns signed included angle of the arc segment
    // The returned value belongs to range ( -2*Pi, 2*Pi )
    virtual double arcAngle() const = 0;

    // returns starting angle of the arc (direction from center to startPt )
    // The returned value belongs to range [ 0, 2*Pi )
    virtual double arcStartAngle() const = 0;

    // returns arc center point, radius and angles if type()==estArc
    virtual Result getArcData( OdGePoint2d & ptCenter, double & dRadius,
        double & dStartAngle, double & dInclAngle ) const = 0;



    //////////////////////////////////////////////////////////////////////////
    // non-const methods

    // Update all
    virtual Result set( const OdGePoint2d & ptA, const OdGePoint2d & ptB, double dBulge, const Attributes2D& rAttr ) = 0;

    // Update ends and bulge, reset attributes
    virtual Result set( const OdGePoint2d & ptA, const OdGePoint2d & ptB, double dBulge=0.0 ) = 0;

    // Update ends and calculate bulge using the intermediate point
    //  (some point on the curve between ptA and ptB )
    virtual Result set( const OdGePoint2d & ptA, const OdGePoint2d & ptB, const OdGePoint2d & ptIntermediate ) = 0;

    // Copy from GeLineSeg2d and reset metadata.
    virtual Result set( const OdGeLineSeg2d& geLine ) = 0;

    // Copy from GeCircArc2d and reset metadata.
    virtual Result set( const OdGeCircArc2d& geArc ) = 0;


    // Copy from another segment with metadata
    virtual Result set( const IBulgeSeg2D& rSeg ) = 0;

    inline IBulgeSeg2D& operator = ( const IBulgeSeg2D& rSeg ) { set( rSeg ); return *this; };

    // Transforms the object using the matrix
    virtual Result transformBy( const OdGeMatrix2d& mTrans ) = 0;

    // inverts the segment (makes it going in opposite direction )
    virtual Result reverse() = 0;


    //////////////////////////////////////////////////////////////////////////
    // Metadata access
    // Contours and segments can hold some additional information: metadata.

    // Get attributes (including metadata)
    virtual const Attributes2D& attributes() const = 0;

    // Get attributes (including metadata) for update
    virtual       Attributes2D& attributes4U() = 0;

    // Set attributes (including metadata)
    inline void    setAttributes( const Attributes2D& rNewAttr ) { attributes4U() = rNewAttr; }

    // Get metadata. Default value : 0
    inline OdIntPtr metadata  () const { return attributes().metadata(); }

    // Set metadata.
    inline void     setMetadata  (OdIntPtr uNewData) { attributes4U().metadata() = uNewData; }

};


};



#endif  //__FM_IBULGESEG2D_H__
