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
#ifndef __FM_CACHEDSEG2D_H__
#define __FM_CACHEDSEG2D_H__

#include "Contours/FMImpCommonSeg2D.h"

namespace FacetModeler
{

//////////////////////////////////////////////////////////////////////////
//
//
//
//
//
//

class FMGEOMETRY_API CachedSeg2D: public CommonSeg2DImpl
{
public:
    // default constructor
    CachedSeg2D();

    // constructor:
    CachedSeg2D( const OdGePoint2d & ptA, const OdGePoint2d & ptB, double dBulge, const Attributes2D& rAttr );

    // constructor: 2 endpoints and a bulge value ( tan(a/4) )
    CachedSeg2D( const OdGePoint2d & ptA, const OdGePoint2d & ptB, double dBulge=0.0 );

    // constructor: 2 endpoints and an intermediate point
    CachedSeg2D( const OdGePoint2d & ptA, const OdGePoint2d & ptB, const OdGePoint2d & ptIntermediate );

    // slower copy constructor
    CachedSeg2D( const IBulgeSeg2D & rAnySeg );

    // virtual ~CachedSeg2D();


    // Fast copy operations (cache is copied too)
    CachedSeg2D( const CachedSeg2D& rSeg );

    Result set( const CachedSeg2D& rSeg );

    inline CachedSeg2D & operator = ( const CachedSeg2D & rSeg )
        { set( rSeg ); return *this; };


    // type info: returns esicCachedSeg2D
    virtual SegmentImplClass implClass() const;

    //////////////////////////////////////////////////////////////////////////
    // const methods

    // returns reference to the starting point
    virtual const OdGePoint2d & startPt() const;

    // returns reference to the ending point
    virtual const OdGePoint2d & endPt  () const;

    // returns bulge value
    virtual double              bulge  () const;


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

    using CommonSeg2DImpl::set;

    // Update ends and bulge
    virtual Result set( const OdGePoint2d & ptA, const OdGePoint2d & ptB, double dBulge, const Attributes2D& rAttr );

    // Copy from another segment
    virtual Result set( const IBulgeSeg2D& rSeg );

    // VC6 compatibility
    virtual Result set( const OdGePoint2d & ptA, const OdGePoint2d & ptB, double dBulge=0.0 )
    {
        return CommonSeg2DImpl::set( ptA, ptB, dBulge );
    }

    virtual Result set( const OdGePoint2d & ptA, const OdGePoint2d & ptB, const OdGePoint2d & ptIntermediate )
    {
        return CommonSeg2DImpl::set( ptA, ptB, ptIntermediate );
    }

    virtual Result set( const OdGeLineSeg2d& geLine )
    {
        return CommonSeg2DImpl::set( geLine );
    }

    virtual Result set( const OdGeCircArc2d& geArc )
    {
        return CommonSeg2DImpl::set( geArc );
    }

    inline IBulgeSeg2D& operator = ( const IBulgeSeg2D& rSeg ) { set( rSeg ); return *this; };

    // Transforms the object using the matrix
    virtual Result transformBy( const OdGeMatrix2d& mTrans );

    // inverts the segment (makes it going in opposite direction )
    virtual Result reverse();


public:

    // Get attributes (including metadata)
    virtual const Attributes2D& attributes() const;

    // Get attributes (including metadata) for update
    virtual       Attributes2D& attributes4U();

private:

    void _UpdateCache();

    inline const OdGePoint2d & _startPt() const { return m_ptStart; };
    inline const OdGePoint2d & _endPt() const { return m_ptEnd; };
    inline double              _bulge() const { return m_dBulge; };

    inline SegmentType   _type()   const { return m_eType; };
    inline double        _length() const { return m_dLength; };
    inline double        _arcAngle() const { return m_dArcInclAngle; };
    inline double        _arcRadius() const { return m_dLength / ::fabs(m_dArcInclAngle); };
    inline const OdGePoint2d & _arcCenter() const { return m_ptArcCenter; };

           double        _arcStartAngle() const;

private:

    //////////////////////////////////////////////////////////////////////////
    // mandatory data:

    OdGePoint2d m_ptStart; // Starting point

    OdGePoint2d m_ptEnd;   // Ending point

    double      m_dBulge;  // Bulge value = tan(angle/4)

    Attributes2D m_Attr;   // Metadata value


    //////////////////////////////////////////////////////////////////////////
    // Cached data:

    // Type of the segment
    SegmentType m_eType;

    // length of the segment
    double m_dLength;

    // double      m_dArcRadius; // = m_dLength / ::fabs(m_dInclAngle);
    double      m_dArcInclAngle;

    // Arc data: center, radius and included angle
    OdGePoint2d m_ptArcCenter;

    // extents ?
    //OdGeExtents2d m_Extents;
};



};



#endif  //__FM_CACHEDSEG2D_H__
