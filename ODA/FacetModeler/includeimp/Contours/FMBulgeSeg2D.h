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
#ifndef __FM_BULGESEG2D_H__
#define __FM_BULGESEG2D_H__

#include "Contours/FMImpCommonSeg2D.h"

namespace FacetModeler
{

//////////////////////////////////////////////////////////////////////////
//
// BulgeSeg2D is a convex 2-D curve given by 2 points and a bulge value
// If the 2 points coincide, the segment has estCoincident type
// Otherwise, if bulge==0, the segment is a straight line segment
//            otherwise it is a circular arc, and bulge=tan(Angle/4)
//

class FMGEOMETRY_API BulgeSeg2D : public CommonSeg2DImpl
{
public:
    //////////////////////////////////////////////////////////////////////////
    // Constructors ...

    // default constructor
    BulgeSeg2D() : m_dBulge(0) { };

    // constructor: 2 endpoints and a bulge value ( tan(a/4) )
    BulgeSeg2D( const OdGePoint2d & ptA, const OdGePoint2d & ptB, double dBulge=0.0 )
        : m_ptStart(ptA)
        , m_ptEnd(ptB)
        , m_dBulge(dBulge)
        { };

    BulgeSeg2D( const OdGePoint2d & ptA, const OdGePoint2d & ptB, double dBulge, const Attributes2D& rAttr )
        : m_ptStart(ptA)
        , m_ptEnd(ptB)
        , m_dBulge(dBulge)
        , m_Attr(rAttr)
        { };

    // constructor: 2 endpoints and an intermediate point
    BulgeSeg2D( const OdGePoint2d & ptA, const OdGePoint2d & ptB, const OdGePoint2d & ptIntermediate );

    // fast copy constructor
    BulgeSeg2D( const BulgeSeg2D  & rSeg )
        : m_ptStart( rSeg.m_ptStart )
        , m_ptEnd( rSeg.m_ptEnd )
        , m_dBulge( rSeg.m_dBulge )
        , m_Attr( rSeg.m_Attr )
        { };

    // slower copy constructor
    BulgeSeg2D( const IBulgeSeg2D & rAnySeg );


    // type info: returns esicBulgeSeg2D
    virtual SegmentImplClass implClass( ) const;

    //////////////////////////////////////////////////////////////////////////
    // Implementation of data getters, not implemented in CommonSeg2DImpl

    // returns reference to the starting point
    virtual const OdGePoint2d & startPt() const;

    // returns reference to the ending point
    virtual const OdGePoint2d & endPt  () const;

    // returns bulge value
    virtual double              bulge  () const;


    //////////////////////////////////////////////////////////////////////////
    // Implementation of non-const methods, not implemented in CommonSeg2DImpl

    using CommonSeg2DImpl::set;

    // Update ends and bulge
    virtual Result set( const OdGePoint2d & ptA, const OdGePoint2d & ptB, double dBulge, const Attributes2D& rAttr );

    // VC6 compatibility
    virtual Result set( const OdGePoint2d & ptA, const OdGePoint2d & ptB, double dBulge=0.0 )
    {
        return CommonSeg2DImpl::set( ptA, ptB, dBulge );
    }

    virtual Result set( const OdGePoint2d & ptA, const OdGePoint2d & ptB, const OdGePoint2d & ptIntermediate )
    {
        return CommonSeg2DImpl::set( ptA, ptB, ptIntermediate );
    }

    virtual Result set( const IBulgeSeg2D& rSeg )
    {
        return CommonSeg2DImpl::set( rSeg );
    }

    virtual Result set( const OdGeLineSeg2d& geLine )
    {
        return CommonSeg2DImpl::set( geLine );
    }

    virtual Result set( const OdGeCircArc2d& geArc )
    {
        return CommonSeg2DImpl::set( geArc );
    }

    // Get attributes (including metadata)
    virtual const Attributes2D& attributes() const;

    // Get attributes (including metadata) for update
    virtual       Attributes2D& attributes4U();

    //////////////////////////////////////////////////////////////////////////
    //  overrides ( a faster implementation of some methods )


    // returns the type of this segment
    virtual SegmentType type() const;



    // To Do ...


protected:

    //////////////////////////////////////////////////////////////////////////
    // necessary data members

    OdGePoint2d m_ptStart; // Starting point

    OdGePoint2d m_ptEnd;   // Ending point

    double      m_dBulge;  // Bulge value = tan(angle/4)

    Attributes2D m_Attr;   // 
};



};


#endif  //__FM_BULGESEG2D_H__
