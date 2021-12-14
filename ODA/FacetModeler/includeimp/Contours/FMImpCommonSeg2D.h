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
#ifndef __FMIMP_COMMONSEG2D_H__
#define __FMIMP_COMMONSEG2D_H__

#include "Contours/FM_IBulgeSeg2D.h"

namespace FacetModeler
{


//////////////////////////////////////////////////////////////////////////
//
// BulgeSeg2D is a convex 2-D curve given by 2 points and a bulge value
// If the 2 points coincide, the segment has estCoincident type
// Otherwise, if bulge==0, the segment is a straight line segment
//            otherwise it is a circular arc, and bulge=tan(Angle/4)
//
// This class contains default implementation of all methods, except the first three
// (startPt, endPt, bulge).

class FMGEOMETRY_API CommonSeg2DImpl : public IBulgeSeg2D
{
public:
    virtual ~CommonSeg2DImpl() { };


    // returns esicUnknown - default value
    virtual SegmentImplClass implClass() const;


    // returns reference to the starting point
    virtual const OdGePoint2d & startPt() const = 0;

    // returns reference to the ending point
    virtual const OdGePoint2d & endPt  () const = 0;

    // returns bulge value
    virtual double              bulge  () const = 0;


    //////////////////////////////////////////////////////////////////////////
    // const methods

    // returns the type of this segment
    virtual SegmentType type() const;

    // returns signed deviation from the line [start, end]
    // the deviation has the same sign, as the bulge() value
    virtual double deviation() const;


    // returns vDir = endPt - startPt
    virtual void   getDir ( OdGeVector2d & vDir ) const;

    // returns distance btw startPt and endPt
    virtual double dirLength( ) const;

    // returns length of the segment
    virtual double length ( ) const;



    // Returns OdGeLineSeg2d corresponding to this segment, if type()!=estArc
    virtual Result getLineSeg ( OdGeLineSeg2d& geLine ) const;

    // Returns OdGeCircArc2d corresponding to this segment, if type()==estArc
    virtual Result getArcSeg  ( OdGeCircArc2d& geArc ) const;



    // returns point at the parameter [ 0, 1 ]
    virtual Result getPoint  ( double dParam, OdGePoint2d & ptPoint  ) const;

    // retruns tangent vector at the parameter [ 0, 1 ]
    virtual Result getTangent( double dParam, OdGeVector2d& vTangent ) const;


    // Adds extents of this segment (extended by dTol) to the given extents.
    virtual void addExtents( OdGeExtents2d& rExtents, double dTol = 0.0 ) const;


    // returns true, if the point lies on the segment
    // *dParam stores the parameter of the point
    virtual bool isOn( const OdGePoint2d & ptTest, double * pdParam = 0,
        const OdGeTol & gTol = FMGeGbl::gTol ) const;


    // returns parameter of a point on the curve, which is closest to the ptTest
    virtual double nearestParam( const OdGePoint2d & ptTest, OdGePoint2d * ptNearest = 0 ) const;


    // returns signed area of a contour bounded by this segment and 2 lines,
    //   connecting the ptOrigin and ends of this segment.
    virtual double integrate( const OdGePoint2d & ptOrigin = OdGePoint2d::kOrigin ) const;


    // returns subsegment given by 2 parameters
    // Note: if (dStartParam > dEndParam) the rSubSeg will have opposite direction
    virtual Result getSubSegment(double dStartParam, double dEndParam, IBulgeSeg2D & rSubSeg ) const;

    // returns parallel segment.  
    // dNormalOffset - signed offset along normal vector at each point of original segment
    // returns error, if source segment is coincident or the offset is too big to get 
    //  valid result
    virtual Result getParallelSegment( double dNormalOffset, IBulgeSeg2D & rParSeg ) const;


    // The method returns number of different intersection points of
    // the segment (including end with minimal Y) with a ray, starting at ptTest and
    // going in the X direction.
    // *pbOnSegment is set to true, if the test point lies on the segment.
    virtual OdUInt32 intersectXRay( const OdGePoint2d& ptTest, bool * pbOnSegment = 0,
        const OdGeTol & gTol = FMGeGbl::gTol ) const;


    // Returns number of intersections (0, 1 or 2) and intersection points.
    // If some parts of the segments coincide (with the given tolerance), returns UINT_MAX
    //   and ends of the coinciding part
    virtual OdUInt32 intersect( const IBulgeSeg2D & rSegB,
        OdGePoint2d * pPt1, OdGePoint2d * pPt2 = 0,
        const OdGeTol & gTol = FMGeGbl::gTol ) const;

    // Returns number of intersections (0, 1 or 2) and Intersection objects.
    // If some parts of the segments coincide (with the given tolerance), returns UINT_MAX
    //   and ends of the coinciding part
    virtual OdUInt32 intersect( const IBulgeSeg2D & rSegB,
        Intersection * pInt1 = 0, Intersection * pInt2 = 0,
        const OdGeTol & gTol = FMGeGbl::gTol ) const;

    // Intersects this segment with an infinite line
    // Returns number of intersections
    virtual OdUInt32 intersectLine( 
        const OdGePoint2d& ptLineOrigin, const OdGeVector2d& vLineDir,
        Intersection * pInt1 = 0, Intersection * pInt2 = 0,
        const OdGeTol & gTol = FMGeGbl::gTol ) const;

    virtual OdUInt32 areOn( Intersection ** ppInt, const OdGeTol & gTol = FMGeGbl::gTol ) const;

    //////////////////////////////////////////////////////////////////////////
    // Arcs and circles ( length>0 and bulge != 0 )


    // returns arc center, if type()==estArc
    virtual OdGePoint2d arcCenter() const;

    // returns arc radius, if type()==estArc
    virtual double      arcRadius( ) const;

    // returns arc radius and center point, if type()==estArc
    virtual Result getCircleData( OdGePoint2d & ptCenter, double & dRadius ) const;

    // returns signed included angle of the arc segment
    // The returned value belongs to range ( -2*Pi, 2*Pi )
    virtual double arcAngle() const;

    // returns starting angle of the arc (direction from center to startPt )
    // The returned value belongs to range [ 0, 2*Pi )
    virtual double arcStartAngle() const;

    // returns arc center point, radius and angles if type()==estArc
    virtual Result getArcData( OdGePoint2d & ptCenter, double & dRadius,
        double & dStartAngle, double & dInclAngle ) const;




    //////////////////////////////////////////////////////////////////////////
    // non-const methods

    // Update all
    virtual Result set( const OdGePoint2d & ptA, const OdGePoint2d & ptB, double dBulge, const Attributes2D& rAttr ) = 0;

    // Update ends and bulge
    virtual Result set( const OdGePoint2d & ptA, const OdGePoint2d & ptB, double dBulge=0.0 );

    // Update ends and calculate bulge using the intermediate point
    //  (some point on the curve between ptA and ptB )
    virtual Result set( const OdGePoint2d & ptA, const OdGePoint2d & ptB, const OdGePoint2d & ptIntermediate );

    // Copy from another segment with metadata
    virtual Result set( const IBulgeSeg2D& rSeg );

    // Copy from GeLineSeg2d and reset metadata.
    virtual Result set( const OdGeLineSeg2d& geLine );
    
    // Copy from GeCircArc2d and reset metadata.
    virtual Result set( const OdGeCircArc2d& geArc );


    // Transforms the object using the matrix
    virtual Result transformBy( const OdGeMatrix2d& mTrans );

    // inverts the segment (makes it going in opposite direction )
    virtual Result reverse();


    //////////////////////////////////////////////////////////////////////////
    // Metadata access
    
    // Get attributes (including metadata)
    virtual const Attributes2D& attributes() const;

    // Get attributes (including metadata) for update
    virtual       Attributes2D& attributes4U();
};



};



#endif  //__FMIMP_COMMONSEG2D_H__
