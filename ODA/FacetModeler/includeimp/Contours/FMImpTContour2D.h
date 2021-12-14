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
#ifndef __FM_T_CONTOUR2D_IMPL_H__
#define __FM_T_CONTOUR2D_IMPL_H__

#include "FMGeometry.h"

#include "Contours/FM_IContour2D.h"
#include "Contours/FMBulgeSeg2D.h"
#include "Contours/FMCachedSeg2D.h"

#include "Ge/GeMatrix2d.h"
#include "Ge/GeExtents2d.h"
#include "Ge/GeLineSeg2d.h"
#include "Ge/GeCircArc2d.h"

// To Do: replace DBL_EPSILON in calculations with an appropriate constant
#include <limits.h>
#include <float.h>
//

namespace FacetModeler
{


extern Result fast_isSelfIntersecting(const IContour2D & rContour, const OdGeTol & gTol, 
                                      bool & bIntersects );

extern Result fast_isValidRegion     (const IContour2D & rContour, const OdGeTol & gTol, 
                                      bool & bValidRegion );

extern Result fast_intersect(const IContour2D& rContA, const IContour2D& rContB, 
                             std::vector< Intersection > & vecPoints, const OdGeTol & gTol );

extern Result fast_getInternalPoint( const IContour2D& rContour, OdGePoint2d& rPoint, const OdGeTol & gTol );

extern void   contour2d_mergeSegments( IContour2D& rContour, int iMergeFlags = 0, const OdGeTol & gTol = FMGeGbl::gTol );

extern Attributes2D& nullAttributesForUpdate();

//////////////////////////////////////////////////////////////////////////
//
// This template implements IContour2D methods
//

template < class TContourData >
class TContour2DImpl :
    public IContour2D,
    protected TContourData
{
public:
    typedef typename TContourData::ImplSeg2D  TImplSeg2D;
    typedef typename TContourData::VertexData TVertexData;

public:
    TContour2DImpl() { };

    TContour2DImpl(const TContour2DImpl& rSrcCont) : TContourData( rSrcCont ) { };

//////////////////////////////////////////////////////////////////////////
//
// IContour2D methods
//

    // Copy this contour from rSrcCont
    virtual void set( const IContour2D & rSrcCont );

    // Create a new copy of this contour.
    // Call delete to release memory
    virtual IContour2D * clone( ) const;


    // returns class info at runtime
    virtual ContourImplClass implClass() const
    {
        return this->_implClass();
    }

    //////////////////////////////////////////////////////////////////////////
    //

    // Returns true if this entity is closed, false otherwise.
    virtual bool isClosed() const
    {
        return this->_closed();
    }

    // Makes the contour closed (open).
    // This method doesn't change vertex count.
    // In open contours the last bulge value is unused.
    virtual void setClosed(bool bClosed = true)
    {
        if ( this->_closed() != bClosed )
            this->_setClosed( bClosed );
    }

    // Removes the last vertex and makes contour closed if
    // the last and the first points are equal. 
    // Returns true, if the vertex was removed and the contour was closed
    virtual bool setClosedIfEndsAreEqual( const OdGeTol& gTol = FMGeGbl::gTol );


    // returns true, if there is no vertices
    virtual bool isEmpty() const
    {
        return this->_empty();
    }

    // returns number of vertices
    virtual OdUInt32 numVerts() const
    {
        return this->_numVerts();
    }

    // returns number of segments
    // It is less than or equal (if the contour is closed) to the number of vertices.
    virtual OdUInt32 numSegments() const
    {
        return this->_numSegments();
    }


    // Returns true, if the contour contains at least one arc
    virtual bool hasArcs() const;



    //////////////////////////////////////////////////////////////////////////
    //
    // 1. Simple Segment and Vertex operations
    //



    // Returns the type of the segment that begins at the specified vertex.
    // If the index is out of range estUnknown is returned
    // Otherwise one of estArc, estLine, estCoincident is returned
    virtual SegmentType segmentType (OdUInt32 iIndex ) const;


    // returns IBulgeSeg2D
    virtual Result getSegmentAt (OdUInt32 iIndex, IBulgeSeg2D & rSegment ) const;

    // Returns coordinates of segment ends and a bulge value
    virtual Result getSegmentAt (OdUInt32 iIndex,
            OdGePoint2d & ptStart,
            OdGePoint2d & ptEnd,
            double & dBulge ) const;

    // Returns OdGeLineSeg2d corresponding to the segment given by iIndex
    // Error code will be returned, if the segment's type is estArc
    virtual Result getLineSegAt (OdUInt32 iIndex, OdGeLineSeg2d& geLine ) const;

    // Returns OdGeCircArc2d corresponding to the segment given by iIndex
    // Error code will be returned, if the segment's type is not estArc
    virtual Result getArcSegAt  (OdUInt32 iIndex, OdGeCircArc2d& geArc  ) const;


    // Returns a point given by iIndex
    virtual Result getPointAt (OdUInt32 iIndex, OdGePoint2d & ptPoint ) const;

    // Returns a bulge value of the segment given by the iIndex
    virtual Result getBulgeAt (OdUInt32 iIndex, double & dBulge ) const;

    // Returns vertex data: starting point, bulge and metadata of corresponding segment
    virtual Result getVertexAt(OdUInt32 iIndex, OdGePoint2d * pptPoint, double * pdBulge = 0, Attributes2D* pAttr = 0 ) const;

    // Returns internal angle of vertex vectors, given by the iIndex
    virtual Result getInternalAngleAt( OdUInt32 iIndex, double & dAngle ) const;

    // Updates existing point coordinates
    virtual Result setPointAt (OdUInt32 iIndex, const OdGePoint2d & ptPoint );

    // Updates existing bulge value
    virtual Result setBulgeAt (OdUInt32 iIndex, double dBulge );

    // Updates existing point and bulge values
    virtual Result setVertexAt (OdUInt32 iIndex, const OdGePoint2d & ptPoint, double dBulge );

    // Reset the contour
    virtual void   reset();

    // Makes capacity of the vertex vector >= iReservedSize
    // Number of vertices is not changed
    virtual void reserveVertices( OdUInt32 iReservedSize );

    // Returns attributes for given segment
    virtual const Attributes2D& attributesAt(OdUInt32 iIndex ) const;

    // Returns attributes for given segment for update
    virtual       Attributes2D& attributes4UAt(OdUInt32 iIndex );

    // Updates attributes for given segment
    virtual Result setAttributesAt(OdUInt32 iIndex, const Attributes2D& rAttr );

    // Returns metadata for given segment or 0
    virtual OdIntPtr metadataAt (OdUInt32 iIndex ) const;
    
    // Updates metadata for given segment
    virtual Result setMetadataAt (OdUInt32 iIndex, OdIntPtr uNewData );

    // Adds a vertex at the specified index.
    // The index should be <= numVerts()
    // If it is equal to numVerts(), a new vertex will be appended to the array,
    // otherwise it'll be inserted before the old iIndex vertex.
    virtual Result addVertexAt    (OdUInt32 iIndex, const OdGePoint2d& ptStart, double dBulge = 0.0, const Attributes2D& rAttr = Attributes2D::kNull );

    // Appends a new starting point and a bulge value.
    // The bulge value relates to the segment starting at the point.
    virtual Result appendVertex   (const OdGePoint2d& ptStart, double dBulge = 0.0, const Attributes2D& rAttr = Attributes2D::kNull );

    // Appends vertex data from vecSource to this contour
    virtual Result appendVertices ( const OdGePoint2dArray & vecSource );

    virtual Result appendVertices(OdUInt32 size, const OdGePoint2d* vecSource, const double* bulgeSource);

    // Appends rSeg to this contour. 
    // Returns erPointNotOnThis if rSeg.startPt() is not equal to this->endPoint().
    // if bShiftToHit is true, the rSeg is shifted and the operation succeedes
    virtual Result appendSegment  ( const IBulgeSeg2D & rSeg, bool bShiftToHit = false, const OdGeTol& gTol = FMGeGbl::gTol  );

    // Appends new segment to this contour ( updates bulge and metadata at the last vertex and 
    // appends new vertex with zero bulge and data)
    // Returns erEmptyContour if there are no vertices in the contour
    // Note: if the contour is closed, the last (closing) segment is wiped out
    virtual Result appendSegment  (double dBulge, const OdGePoint2d& ptNewEnd, const Attributes2D& rAttr = Attributes2D::kNull );


    // Appends rCont to this contour ( "closed" flags are ignored )
    // Returns erPointNotOnThis if rCont.startPoint is not equal to the end of this (open) contour
    // If bCloseGap is true, a segment, connecting the points is inserted and
    //   the return code is erWarnPointNotOnThis
    virtual Result appendContour  ( const IContour2D & rCont, bool bCloseGap = false, double dMaxGap = 1e99 );


    // Removes a vertex (and a corresponding segment).
    // Bulge values of the other segments are left intact.
    virtual Result removeVertexAt (OdUInt32 iIndex );
    


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


    // Returns starting point of the contour, if the contour is not empty
    virtual Result getStartPoint(OdGePoint2d & ptPoint ) const;

    // Returns ending point of the contour, if the contour is not empty.
    // The ptPoint is equal to the starting point, if the contour is closed.
    virtual Result getEndPoint  (OdGePoint2d & ptPoint ) const;


    // Returns the length of the contour
    virtual double length() const;


    // returns tangent vector at the parameter
    virtual Result getTangent     (double dParam, OdGeVector2d& vTangent ) const;

    // Returns a point given by a parameter value
    virtual Result getPoint       (double dParam, OdGePoint2d& ptPoint ) const;

    // Returns a point given by a distance along the curve from the starting point
    virtual Result getPointAtDist (double dDist,  OdGePoint2d& ptPoint ) const;

    // Converts parameter value to the distance from start along the curve
    virtual Result getDistAtParam (double dParam, double& dDist ) const;

    // Converts distance from start to parameter value
    virtual Result getParamAtDist (double dDist,  double& dParam ) const;


    //virtual Result getParamAtPoint(const OdGePoint2d& ptPoint, double& dParam) const;
    //virtual Result getDistAtPoint (const OdGePoint2d& ptPoint, double& dDist) const;


    //////////////////////////////////////////////////////////////////////////
    // Distance along the contour and closed contours
    
    // Convert dParam to [0, endParam() ) range (on closed contours)
    // and returns erOk, if the dParam  is valid
    virtual Result getNormalizedParam( double & dParam ) const;
    
    // Convert dDist to [0, length() ) range (on closed contours)
    // and returns erOk, if the dDist is valid
    virtual Result getNormalizedDist( double & dDist ) const;
    
    // Returns true, if the parameters are valid and equal (after normalization) with given tolerance
    //   should not be called on invalid parameters !
    // ToDo: dParam tolerance!!!
    virtual bool areEqualParams( double dParam1, double dParam2, const double dTol = 1e-10 ) const;
    
    // Returns true, if the distances are valid and equal (after normalization) with given tolerance
    //   returns false, if the params are not equal or not valid
    virtual bool areEqualDists ( double dDist1, double dDist2, const double dTol = FMGeGbl::gTol.equalPoint() ) const;
    
    
    // Get signed length of the shortest way btw the points 1 and 2.
    // The points are given by distances along the curve
    virtual double signedMinDist        ( double dDist1, double dDist2 ) const;
    
    // Get signed length of the shortest way btw the points 1 and 2.
    // The points are given by parameters on the curve
    virtual double signedMinDistByParams( double dParam1, double dParam2 ) const;



    //////////////////////////////////////////////////////////////////////////
    //
    // 3. Geometrical Queries
    //


    // Adds geometrical extents of the contour to geExtents
    virtual Result addExtents(OdGeExtents2d& geExtents ) const;


    // Returns signed area of the contour. (positive for CCW contours)
    // In open contours the first and last vertices are considered
    // to be connected with a straight line.
    virtual double signedArea() const;

    // returns true, if the contour is convex
    // end false if otherwise
    virtual bool isConvex() const;

    // returns internal point of the closed contour (contour orientation is ignored)
    // If succeeded this->contains( rPoint, gTol ) will return true
    virtual Result getInternalPoint( OdGePoint2d& rPoint, const OdGeTol& gTol = FMGeGbl::gTol ) const;

    // returns true, if the point lies on the contour
    // *dParam stores the parameter of the point
    virtual bool isOn( const OdGePoint2d & ptTest, double * pdParam = 0,
        const OdGeTol & gTol = FMGeGbl::gTol ) const;

    // returns parameter of a point on the curve, which is closest to the ptTest
    virtual double nearestParam( const OdGePoint2d & ptTest, OdGePoint2d * ptNearest = 0 ) const;


    // Returns true if the point lies inside the contour or on it's border.
    // Contour must be closed.
    // pbOnBorder is true, if the point lies on the border of the contour
    // Note: the contour is considered to be non-oriented.
    virtual bool contains( const OdGePoint2d &rPoint, bool * pbOnBorder = 0,
        const OdGeTol & gTol = FMGeGbl::gTol) const;


    // Returns whether the contour intersects itself.
    // Works O( N log N ) at the average
    virtual bool isSelfIntersecting(const OdGeTol& gTol = FMGeGbl::gTol, bool bExcludeTouch = false ) const;

    // Returns whether this contour is closed and coincides with a directional 
    // boundary of some open 2D-region.
    virtual bool isValidRegion( const OdGeTol& gTol = FMGeGbl::gTol ) const;

    virtual bool isInsideContour(const IContour2D& c2dOuter, bool fAllowBordersTouch = false, const OdGeTol& gTol = FMGeGbl::gTol) const;

    // Returns number of intersections
    // and appends Intersection data to the vecPoints vector
    // Note: some intersection points (esp. on segment bounds) can coincide
    virtual OdUInt32 intersect( const IBulgeSeg2D & rSegB,
        std::vector< Intersection > & vecPoints,
        const OdGeTol & gTol = FMGeGbl::gTol ) const;

    // Returns number of intersections
    // and appends Intersection data to the vecPoints vector
    // Note: some intersection points (esp. on segment bounds) can coincide
    virtual OdUInt32 intersect( const IContour2D& rContB,
        std::vector< Intersection > & vecPoints,
        const OdGeTol & gTol = FMGeGbl::gTol ) const;

    // Intersects this contour with an infinite line
    // Returns number of intersections
    // and APPENDS Intersection data to the vecPoints vector
    // Note: some intersection points (esp. on segment bounds) can coincide
    virtual OdUInt32 intersectLine( 
        const OdGePoint2d& ptLineOrigin, 
        const OdGeVector2d& vLineDir,
        std::vector< Intersection > & vecPoints,
        const OdGeTol & gTol = FMGeGbl::gTol ) const;


    //////////////////////////////////////////////////////////////////////////
    //
    // 4. Transformations
    //

    // Transforms the contour
    virtual Result transformBy(const OdGeMatrix2d& geMatrix,
        const DeviationParams& devDeviation = FMGeGbl::gDefDev );

    // Reverses the contour.
    virtual void reverse();

    // Deletes redundant segments that have length = 0.
    virtual void deleteCoincident( const OdGeTol & gTol = FMGeGbl::gTol );

    // Merges adjacent segments that lie on the same line or arc
    //   emfMergeArcs       - Merge circular segments
    //   emfIgnoreMetadata  - Merge segments with different metadata
    //   emfMergeOrigin     - Merge first and last segments of a closed contour
    virtual void mergeSegments( int iMergeFlags = 0, const OdGeTol & gTol = FMGeGbl::gTol );

    // Explodes all arc segments to lines.
    // Facet deviation defines the maximum distance from the chord to the arc.
    virtual Result explodeTo( IContour2D & rDestCont,
        const DeviationParams& devDeviation = FMGeGbl::gDefDev, 
        OdIntPtr uArcMetadata = 0 ) const;


    //////////////////////////////////////////////////////////////////////////


    // Adds a vertex corresponding to the specified parameter into the contour, if
    // the vertex does not exist.
    // Returns index of the created/existing vertex or OdUInt32(-1), if the dParam is not valid.
    // Note: insertion/deletion of a vertex at dParam invalidates all parameters > dParam
    virtual OdUInt32 createVertexAt( double dParam, const OdGeTol & gTol = FMGeGbl::gTol );

    // Adds vertices at the specified parameters into the contour, if
    // the vertices do not exist.
    virtual Result createVerticesAt( const std::vector<double> & vecParams, const OdGeTol & gTol = FMGeGbl::gTol );

    virtual Result createVerticesAt( OdUInt32 size, const double* vecParams, const OdGeTol & gTol = FMGeGbl::gTol );


    // returns sub-contour given by 2 parameters
    // Note: if (dStartParam > dEndParam) the rSubContour will have opposite direction
    virtual Result getSubContour(double dStartParam, double dEndParam,
        IContour2D & rSubContour, const OdGeTol & gTol = FMGeGbl::gTol ) const;

    // appends sub-contour to rSubContour, if possible
    virtual Result appendSubContourTo(double dStartParam, double dEndParam,
        IContour2D & rSubContour, const OdGeTol & gTol = FMGeGbl::gTol ) const;

    // Divides this contour into sets of contours that lie to the left of the given line and to the right.
    // Note: if OdGeLinearEnd2d can be an infinite of a finite line, depending
    //   on it`s class: OdGeLine2d or OdGeLineSeg2d
    //virtual Result splitByLine( const OdGeLinearEnt2d& rLine,
    //    AECProfile& rLeftSide, AECProfile& rRightSide,
    //    const OdGeTol & gTol = FMGeGbl::gTol ) const;


    virtual Result replaceSubContourTo( IContour2D& rDest, const IContour2D & rSubContour, const OdGeTol & gTol = FMGeGbl::gTol ) const;

//////////////////////////////////////////////////////////////////////////
//
// Internal implementation methods
//
protected:

    Result _param2IdxParam( double dParam, OdUInt32 & iIndex, double & dSegParam ) const;
    Result _dist2IdxParam ( double dDist,  OdUInt32 & iIndex, double & dSegParam ) const;
    Result _param2dist    ( double dParam, double & dDist ) const;

    Result _normalizeParam( double & dParam ) const;
    
    Result _createVerticesAt( OdUInt32 uCount, const double * pdParams, const OdGeTol & gTol );

    Result _getSubSegment ( OdUInt32 iIndex, double dStartOffs, double dEndOffs, IBulgeSeg2D & rDestSeg ) const; 

    Result _paramRange2IdxParams( const double dParamA, const double dParamB, 
        OdUInt32& uIdxA, double& dSegParamA, OdUInt32& uIdxB, double& dSegParamB,
        OdUInt32& iNumPeriodsAB, const double dParamTol = DBL_EPSILON ) const;

    Result _appendSubContourTo(double dStartParam, double dEndParam, IContour2D & rSubContour, const OdGeTol & gTol ) const;


    inline bool _isEqualToThis( const IContour2D & rC ) const
    {
        return ((const IContour2D *)this) == &rC;
    }
};



//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//
//
// IMPLEMENTATiON



template < class TContourData >
void TContour2DImpl<TContourData>::set( const IContour2D & rSrcCont )
{
    if ( this == &rSrcCont )
        return;

    OdUInt32 iNumVerts = rSrcCont.numVerts();
    bool bClosed = rSrcCont.isClosed();

    this->_reset( iNumVerts, bClosed );

    if ( iNumVerts > 0 )
    {
        for (OdUInt32 iVert = 0; iVert < iNumVerts; iVert++ )
        {
            TVertexData & rVert = this->_vertex( iVert );

            Result eRes = erOk;
            eRes = rSrcCont.getVertexAt( iVert, & rVert.point(), & rVert.bulge(), & rVert.attributes() );
            FMGE_ASSERT( isOk(eRes) );
        }
    }

    this->_setModifiedAll();
}


template < class TContourData >
IContour2D * TContour2DImpl<TContourData>::clone( ) const
{
    FMGE_FAULT("Not Implemented");
    return 0;
}


template < class TContourData >
bool TContour2DImpl<TContourData>::setClosedIfEndsAreEqual( const OdGeTol& gTol )
{
    OdUInt32 uNumVerts = this->_numVerts();

    if ( uNumVerts < 2 )
        return false;

    if ( ! this->_vertex( 0 ).point().isEqualTo( this->_vertex( uNumVerts - 1 ).point(), gTol ) )
        return false;

    this->_removeVertices( uNumVerts - 1 );
    this->_setClosed( true );

    return true;
}


template < class TContourData >
bool TContour2DImpl<TContourData>::hasArcs() const
{
    OdUInt32 iNumSeg = this->_numSegments();

    TImplSeg2D ImplSeg;
    for (OdUInt32 iSeg = 0; iSeg<iNumSeg; iSeg++ )
    {
        this->_getSegment( iSeg, ImplSeg );
        if ( ImplSeg.type() == estArc )
            return true;
    }

    return false;
}




template < class TContourData >
SegmentType TContour2DImpl<TContourData>::segmentType (OdUInt32 iIndex ) const
{
    TImplSeg2D Seg;
    if ( isError( this->_getSegment( iIndex, Seg ) ) )
        return estUnknown;

    return Seg.type();
}


template < class TContourData >
const Attributes2D& TContour2DImpl<TContourData>::attributesAt(OdUInt32 iIndex ) const
{
    if ( iIndex < this->_numVerts() )
        return this->_vertex( iIndex ).attributes();

    return Attributes2D::kNull;
}

template < class TContourData >
Attributes2D& TContour2DImpl<TContourData>::attributes4UAt(OdUInt32 iIndex )
{
    if ( iIndex < this->_numVerts() )
        return this->_vertex( iIndex ).attributes();

    return nullAttributesForUpdate();
}

template < class TContourData >
Result TContour2DImpl<TContourData>::setAttributesAt(OdUInt32 iIndex, const Attributes2D& rAttr )
{
    if ( iIndex < this->_numVerts() )
    {
        this->_vertex( iIndex ).attributes() = rAttr;
        return erOk;
    }

    return erInvalidIndex;
}

template < class TContourData >
OdIntPtr TContour2DImpl<TContourData>::metadataAt (OdUInt32 iIndex ) const
{
    OdUInt32 uData = 0;

    if ( iIndex < this->_numVerts() )
        uData = this->_vertex( iIndex ).attributes().metadata();

    return uData;
}

template < class TContourData >
Result TContour2DImpl<TContourData>::setMetadataAt (OdUInt32 iIndex, OdIntPtr uNewData )
{
    Result eRes = erOk;

    if ( iIndex < this->_numVerts() )
        this->_vertex( iIndex ).attributes().metadata() = uNewData;
    else
        eRes = erInvalidIndex;

    return eRes;
}


//template < class TContourData >
//Result TContour2DImpl<TContourData>::setAllMetadata (OdUInt32 uNewData, OdUInt32 uBitsToModify )
//{
//    OdUInt32 iNumVerts = this->_numVerts();
//    if ( iNumVerts <= 0 )
//        return erOk;
//
//    if ( 0 == uBitsToModify )
//        return erOk;
//
//    OdUInt32 uAndMask = ~uBitsToModify;
//    OdUInt32 uOrMask  =  (uNewData & uBitsToModify);
//
//    for (OdUInt32 uVert = 0; uVert < iNumVerts; ++uVert )
//    {
//        OdUInt32 & rData = this->_vertex(uVert).attributes().metadata();
//        rData &= uAndMask;
//        rData |= uOrMask;
//    }
//
//    return erOk;
//}

template < class TContourData >
Result TContour2DImpl<TContourData>::getSegmentAt (OdUInt32 iIndex, IBulgeSeg2D & rSegment ) const
{
    TImplSeg2D ImplSeg;
    Result eRes = this->_getSegment( iIndex, ImplSeg );
    if ( isError( eRes ) )
        return eRes;

    return worstResult( eRes, rSegment.set( ImplSeg ) );
}



template < class TContourData >
Result TContour2DImpl<TContourData>::getSegmentAt (OdUInt32 iIndex,
            OdGePoint2d & ptStart,
            OdGePoint2d & ptEnd,
            double & dBulge ) const
{
    TImplSeg2D ImplSeg;
    Result eRes = this->_getSegment( iIndex, ImplSeg );
    if ( isError( eRes ) )
        return eRes;

    ptStart = ImplSeg._startPt();
    ptEnd   = ImplSeg._endPt();
    dBulge  = ImplSeg._bulge();  
    //uMetadata = ImplSeg._metadata();

    return eRes;
}



template < class TContourData >
Result TContour2DImpl<TContourData>::getLineSegAt (OdUInt32 iIndex, OdGeLineSeg2d& geLine ) const
{
    TImplSeg2D ImplSeg;
    Result eRes = this->_getSegment( iIndex, ImplSeg );
    if ( isError( eRes ) )
        return eRes;

    return worstResult( eRes, ImplSeg.getLineSeg( geLine ) );
}



template < class TContourData >
Result TContour2DImpl<TContourData>::getArcSegAt  (OdUInt32 iIndex, OdGeCircArc2d& geArc  ) const
{
    TImplSeg2D ImplSeg;
    Result eRes = this->_getSegment( iIndex, ImplSeg );
    if ( isError( eRes ) )
        return eRes;

    return worstResult( eRes, ImplSeg.getArcSeg( geArc ) );
}



template < class TContourData >
Result TContour2DImpl<TContourData>::getPointAt (OdUInt32 iIndex, OdGePoint2d & ptPoint ) const
{
    Result eRes = this->_normalizeIndex( iIndex );
    if ( isError( eRes ) )
        return eRes;

    ptPoint = this->_vertex(iIndex).point();
    return eRes;
}



template < class TContourData >
Result TContour2DImpl<TContourData>::getBulgeAt (OdUInt32 iIndex, double & dBulge ) const
{
    Result eRes = this->_normalizeIndex( iIndex );
    if ( isError( eRes ) )
        return eRes;

    dBulge = this->_vertex(iIndex).bulge();
    return eRes;
}


template < class TContourData >
Result TContour2DImpl<TContourData>::getVertexAt(OdUInt32 iIndex, OdGePoint2d * pptPoint, double * pdBulge, Attributes2D * pAttr ) const
{
    Result eRes = this->_normalizeIndex( iIndex );
    if ( isError( eRes ) )
        return eRes;

    const TVertexData & rVert = this->_vertex(iIndex);

    if ( pptPoint ) 
        *pptPoint = rVert.point();

    if ( pdBulge ) 
        *pdBulge = rVert.bulge();

    if ( pAttr ) 
        *pAttr = rVert.attributes();

    return eRes;
}


template < class TContourData >
Result TContour2DImpl<TContourData>::getInternalAngleAt( OdUInt32 iIndex, double & dAngle ) const
{
    Result eRes = this->_normalizeIndex( iIndex );
    if ( isError( eRes ) )
        return eRes;

    OdUInt32 iSize = this->numSegments();
    if( !this->_closed() && ( iIndex == 0 || iIndex == iSize-1 ) )
    {
      dAngle = 0.0;
      return eRes;
    }

    TImplSeg2D rSeg;
    OdGeVector2d vA;
    OdGeVector2d vB;

    this->_getSegment( iIndex, rSeg );
    rSeg.getTangent( 0.0, vA );

    if( iIndex == 0 )
      this->_getSegment( iSize-1, rSeg );
    else
       this->_getSegment( iIndex-1, rSeg );

    rSeg.getTangent( 1.0, vB );

    dAngle = vA.angleToCCW( vB.negate() );
    if( dAngle < 0 )
      dAngle = dAngle + Oda2PI;

    return eRes;
}



template < class TContourData >
void TContour2DImpl<TContourData>::reset()
{
    this->_reset();
}



template < class TContourData >
void TContour2DImpl<TContourData>::reserveVertices( OdUInt32 iReservedSize )
{
    this->_reserveVertices( iReservedSize );
}



template < class TContourData >
Result TContour2DImpl<TContourData>::setPointAt (OdUInt32 iIndex, const OdGePoint2d & ptPoint )
{
    Result eRes = this->_normalizeIndex( iIndex );
    if ( isError( eRes ) )
        return eRes;

    this->_vertex( iIndex ).point() = ptPoint;
    this->_setModifiedVerts( iIndex );

    return eRes;
}



template < class TContourData >
Result TContour2DImpl<TContourData>::setBulgeAt (OdUInt32 iIndex, double dBulge )
{
    Result eRes = this->_normalizeIndex( iIndex );
    if ( isError( eRes ) )
        return eRes;

    this->_vertex( iIndex ).bulge() = dBulge;
    this->_setModifiedSegs( iIndex );

    return eRes;
}


template < class TContourData >
Result TContour2DImpl<TContourData>::setVertexAt (OdUInt32 iIndex, const OdGePoint2d & ptPoint, double dBulge )
{
    Result eRes = this->_normalizeIndex( iIndex );
    if ( isError( eRes ) )
        return eRes;
    
    TVertexData & rVert = this->_vertex( iIndex );
    rVert.point() = ptPoint;
    rVert.bulge() = dBulge;
    this->_setModifiedVerts( iIndex );
    
    return eRes;
}



template < class TContourData >
Result TContour2DImpl<TContourData>::addVertexAt(OdUInt32 iIndex, const OdGePoint2d& ptStart, double dBulge, const Attributes2D& rAttr )
{
    return this->_insertVerticesAt( iIndex, 1, &ptStart, &dBulge, &rAttr );
}



template < class TContourData >
Result TContour2DImpl<TContourData>::appendVertex   (const OdGePoint2d& ptStart, double dBulge, const Attributes2D& rAttr)
{
    return this->_insertVerticesAt( this->_numVerts(), 1, &ptStart, &dBulge, &rAttr );
}





template < class TContourData >
Result TContour2DImpl<TContourData>::appendVertices ( const OdGePoint2dArray & vecSource )
{
    if ( vecSource.empty() )
        return erInvalidIndex;

    return this->_insertVerticesAt( this->_numVerts(), vecSource.size(), &(vecSource[0]) );
}

template < class TContourData >
Result TContour2DImpl<TContourData>::appendVertices(OdUInt32 size, const OdGePoint2d* vecSource, const double* bulgeSource)
{
  if ( size == 0 )
        return erInvalidIndex;

  return this->_insertVerticesAt( this->_numVerts(), size, vecSource, bulgeSource );
}


template < class TContourData >
Result TContour2DImpl<TContourData>::appendSegment  ( const IBulgeSeg2D & rSeg, bool bShiftToHit, const OdGeTol& gTol )
{
    if ( this->_empty() )
    {
        OdGePoint2d vPoints[2] = { rSeg.startPt(), rSeg.endPt() };
        double      vBulges[2] = { rSeg.bulge(),   0.0 };

        Result eRes = this->_insertVerticesAt( 0, 2, vPoints, vBulges );
        if ( isOk(eRes) )
            this->_vertex( 0 ).attributes() = rSeg.attributes();

        return eRes;
    }

    // Contour is not empty
    OdUInt32 iLastVert = this->_numVerts() - 1;

    TVertexData & rLastVert = this->_vertex( iLastVert );
    const OdGePoint2d & ptEnd = rLastVert.point();

    const OdGePoint2d & rSegStart = rSeg.startPt();
    OdGePoint2d ptSegEnd = rSeg.endPt();

    Result eRes = erOk;

    if ( !ptEnd.isEqualTo( rSegStart, gTol ) )
    {
        if ( !bShiftToHit )
            return erPointNotOnThis;

        eRes = erWarnPointNotOnThis;

        // shift the end
        ptSegEnd.x += ptEnd.x - rSegStart.x;
        ptSegEnd.y += ptEnd.y - rSegStart.y;
    }

    double dBulge = rSeg.bulge();
    if ( dBulge != rLastVert.bulge() )
    {
        rLastVert.bulge() = dBulge;
        this->_setModifiedVerts( iLastVert );
    }

    rLastVert.attributes() = rSeg.attributes();
    eRes = worstResult( eRes, this->_insertVerticesAt( iLastVert+1, 1, &ptSegEnd ) );

    return eRes;
}



template < class TContourData >
Result TContour2DImpl<TContourData>::appendSegment(double dBulge, const OdGePoint2d& ptNewEnd, const Attributes2D& rAttr )
{
    if ( this->_empty() )
        return erEmptyContour;

    // Contour is not empty
    OdUInt32 iLastVert = this->_numVerts() - 1;
    TVertexData & rLastVert = this->_vertex( iLastVert );

    // update bulge if different
    if ( dBulge != rLastVert.bulge() )
    {
        rLastVert.bulge() = dBulge;
        this->_setModifiedVerts( iLastVert );
    }

    // update metadata
    rLastVert.attributes() = rAttr;

    // append the new vertex
    return this->_insertVerticesAt( iLastVert+1, 1, &ptNewEnd );
}


template < class TContourData >
Result TContour2DImpl<TContourData>::appendContour  ( const IContour2D & rCont, bool bCloseGap, double dMaxGap )
{
    FMGE_ASSERT( !_isEqualToThis( rCont ) );
    if ( _isEqualToThis( rCont ) )
        return erInvalidArgs;

    if ( rCont.isEmpty() )
        return erOk;
    
    if ( this->_empty() )
    {
        set( rCont );
        return erOk;
    }

    Result eBigRes = erOk;

    OdGePoint2d ptContStart;
    Result eRes = rCont.getStartPoint( ptContStart );
    if ( isError( eRes ) )
        return eRes;

    // this contour is not empty
    OdUInt32 iLastVert = this->_numVerts() - 1;
    
    TVertexData & rLastVert = this->_vertex( iLastVert );
    const OdGePoint2d & ptEnd = rLastVert.point();

    if ( ! ptEnd.isEqualTo( ptContStart, FMGeGbl::gTol ) )
    {
        if ( !bCloseGap || dMaxGap < ptEnd.distanceTo( ptContStart ) )
            return erPointNotOnThis;

        eBigRes = erWarnPointNotOnThis;

        // close the gap with a linear segment

        if ( rLastVert.bulge() != 0.0 )
        {
            rLastVert.bulge() = 0.0;
            this->_setModifiedVerts( iLastVert );
        }

        ++iLastVert; // start appending from the next vertex
    }

    OdUInt32 iNumContVerts = rCont.numVerts();
    FMGE_ASSERT( iNumContVerts > 0 );

    OdUInt32 iModifiedVert = iLastVert;
    this->_resize( iLastVert + iNumContVerts );

    for (OdUInt32 iContVert = 0 ; iContVert < iNumContVerts; ++iContVert, ++iLastVert )
    {
        TVertexData & rVert = this->_vertex( iLastVert );
        
        eRes = rCont.getVertexAt( iContVert, & rVert.point(), & rVert.bulge(), & rVert.attributes() ); 
        FMGE_ASSERT( isOk(eRes) );
        eBigRes = worstResult( eBigRes, eRes );
    }
    this->_setModifiedVerts( iModifiedVert, iNumContVerts );
    
    return eBigRes;
}





template < class TContourData >
Result TContour2DImpl<TContourData>::removeVertexAt (OdUInt32 iIndex )
{
    return this->_removeVertices( iIndex );
}




template < class TContourData >
Result TContour2DImpl<TContourData>::getStartPoint(OdGePoint2d & ptPoint ) const
{
    if ( this->_empty() )
        return erInvalidIndex;

    ptPoint = this->_vertex(0).point();
    return erOk;
}




template < class TContourData >
Result TContour2DImpl<TContourData>::getEndPoint  (OdGePoint2d & ptPoint ) const
{
    if ( this->_empty() )
        return erInvalidIndex;

    OdUInt32 iIndex = this->_closed() ? 0 : (this->_numVerts()-1);

    ptPoint = this->_vertex(iIndex).point();
    return erOk;
}





template < class TContourData >
double TContour2DImpl<TContourData>::length() const
{
    OdUInt32 iNumSeg = this->_numSegments();

    double dL = 0;

    TImplSeg2D ImplSeg;
    for (OdUInt32 iSeg = 0; iSeg<iNumSeg; iSeg++ )
    {
        this->_getSegment( iSeg, ImplSeg );
        dL += ImplSeg.length();
    }

    return dL;
}





template < class TContourData >
Result TContour2DImpl<TContourData>::_param2IdxParam(double dParam, OdUInt32 & iIndex, double & dSegParam ) const
{
    long iNumSegs = this->_numSegments();
    if ( iNumSegs <= 0 )
    {
        iIndex = 0;
        dSegParam = 0;
        return erEmptyContour;
    }

    const double dParamTol = 4*DBL_EPSILON;

    double dIdx;
    double dNormalizedParam = ::modf( dParam, &dIdx );
    long iNormalizedIdx = long( ::floor( dIdx + 0.5 ) );
    if ( dNormalizedParam < 0 )
    {
        dNormalizedParam += 1;
        iNormalizedIdx   -= 1;
    }

    Result eRes = erOk;

    if ( iNormalizedIdx >= long(iNumSegs) )
    {
        if ( iNormalizedIdx==iNumSegs && dNormalizedParam <= dParamTol )
        {
            iNormalizedIdx--;
            dNormalizedParam = 1.0;
        }
        else if ( this->_closed() )
        {
            iNormalizedIdx = ::ldiv( iNormalizedIdx, iNumSegs ).rem;
            FMGE_ASSERT( OdUInt32(iNormalizedIdx) < OdUInt32(iNumSegs) );
        }
        else
            eRes = erParamBounds;
    }
    else if ( iNormalizedIdx < 0 )
    {
        if ( iNormalizedIdx== -1 && dNormalizedParam >= (1.0-dParamTol) )
        {
            iNormalizedIdx++;
            dNormalizedParam = 0.0;
        }
        else if ( this->_closed() )
        {
            iNormalizedIdx = ::ldiv( iNormalizedIdx, iNumSegs ).rem + iNumSegs;
            FMGE_ASSERT( OdUInt32(iNormalizedIdx) < OdUInt32(iNumSegs) );
        }
        else
            eRes = erParamBounds;
    }

    dSegParam = dNormalizedParam;
    iIndex    = OdUInt32( iNormalizedIdx );

    FMGE_ASSERT( dSegParam >= -DBL_EPSILON &&  dSegParam <= (1.0+DBL_EPSILON) );

    return eRes;
}




template < class TContourData >
Result TContour2DImpl<TContourData>::_dist2IdxParam ( double dDist,  OdUInt32 & iIndex, double & dSegParam ) const
{
    iIndex = 0;
    dSegParam = 0;

    OdUInt32 iNumSegs = this->_numSegments();
    if ( iNumSegs <= 0 )
        return erEmptyContour;

    const double dTol = FMGeGbl::gTol.equalVector();

    bool bClosed = this->_closed();

    bool bDistNormalized = false;

    if ( dDist < 0.0 )
    {
        if ( !bClosed )
            return ( dDist >= -dTol ) ? erOk : erParamBounds;

        double dLength = ((IContour2D*)this)->length(); // virtual method
        if ( dLength <= dTol )
            return ( dDist >= -dTol ) ? erOk : erParamBounds; // zero-length contour

        // in closed contours we should convert dDist to range [0, dLength)
        bDistNormalized = true;
        dDist -= ::floor( dDist/dLength ) * dLength;
        if (dDist < 0.0 || dDist >= dLength )
            return erOk; // dDist = 0.0;
    }

    for(;;)
    {
        double dCurDist = 0;
        OdUInt32 iSeg = 0;

        TImplSeg2D ImplSeg;
        do
        {
            this->_getSegment( iSeg, ImplSeg );
            double dL = ImplSeg.length();
            if ( (dCurDist + dL) > dDist )
            {
                // We've just found the segment
                iIndex = iSeg;
                dSegParam = (dDist - dCurDist)/dL;
                return erOk;
            }

            dCurDist += dL;

        } while ( ++iSeg < iNumSegs );

        // Segment was not found
        if ( bDistNormalized )
        {
            FMGE_FAULT( "It seems that contour::length() returned a wrong value!" );
            return erParamBounds;
        }

        // dCurDist == length
        double dLength = dCurDist;

        if ( !this->_closed() || dLength <= dTol )
        {
            iIndex = iNumSegs - 1; // Last segment
            dSegParam = 1.0; // Last point
            return ( dDist - dLength ) <= dTol ? erOk : erParamBounds;
        }

        // convert dDist to range [0, dLength)
        bDistNormalized = true;
        dDist -= ::floor( dDist/dLength ) * dLength;
        if (dDist < 0.0 || dDist >= dLength )
            return erOk; // dDist = 0.0;

    } // try again with normalized dDist
}



template < class TContourData >
Result TContour2DImpl<TContourData>::_param2dist    ( double dParam, double & dDist ) const
{
    OdUInt32 iIndex;
    double dSegParam;
    Result eRes = _param2IdxParam( dParam, iIndex, dSegParam );
    if ( isError(eRes) )
        return eRes;

    double dSumLen = 0;

    TImplSeg2D ImplSeg;

    // Add offset from the iIndex segment
    if ( dSegParam!= 0.0 )
    {
        this->_getSegment( iIndex, ImplSeg );
        dSumLen += ImplSeg.length()*dSegParam;
    }

    // Add lengths of all segments < iIndex
    for (OdUInt32 iSeg = 0; iSeg<iIndex; iSeg++ )
    {
        this->_getSegment( iSeg, ImplSeg );
        dSumLen += ImplSeg.length();
    }

    dDist = dSumLen;
    return eRes;
}



template < class TContourData >
Result TContour2DImpl<TContourData>::getTangent     (double dParam, OdGeVector2d& vTangent ) const
{
    OdUInt32 iIndex;
    double dSegParam;
    TImplSeg2D ImplSeg;

    Result eRes = _param2IdxParam( dParam, iIndex, dSegParam );
    if (   isError( eRes ) 
        || isError( eRes = worstResult( eRes, this->_getSegment( iIndex, ImplSeg ) ) ) 
       )
    {
        vTangent = OdGeVector2d::kIdentity;
        return eRes;
    }

    return worstResult( eRes, ImplSeg.getTangent( dSegParam, vTangent ) );
}




template < class TContourData >
Result TContour2DImpl<TContourData>::getPoint       (double dParam, OdGePoint2d& ptPoint ) const
{
    // Handle empty and 1-point contours
    if ( this->_numVerts()<=1 )
    {
        if ( !this->_empty() && OdZero(dParam) )
        {
            ptPoint = this->_vertex(0).point();
            return erOk;
        }
        ptPoint = OdGePoint2d::kOrigin;
        return this->_empty() ? erEmptyContour : erParamBounds;
    }

    OdUInt32 iIndex;
    double dSegParam;
    TImplSeg2D ImplSeg;

    Result eRes = _param2IdxParam( dParam, iIndex, dSegParam );
    if (   isError( eRes )
        || isError( eRes = worstResult( eRes, this->_getSegment( iIndex, ImplSeg ) ) )
        )
    {
        ptPoint = OdGePoint2d::kOrigin;
        return eRes;
    }

    return worstResult( eRes, ImplSeg.getPoint( dSegParam, ptPoint ) );
}




template < class TContourData >
Result TContour2DImpl<TContourData>::getPointAtDist (double dDist,  OdGePoint2d& ptPoint ) const
{
    ptPoint = OdGePoint2d::kOrigin;

    // Handle empty and 1-point contours
    if ( this->_numVerts() <= 1 )
    {
        if ( !this->_empty() && OdZero( dDist, FMGeGbl::gTol.equalVector() ) )
        {
            ptPoint = this->_vertex(0).point();
            return erOk;
        }
        return this->_empty() ? erEmptyContour : erParamBounds;
    }

    double dParam = 0;
    Result eRes = ((IContour2D*)this)->getParamAtDist( dDist, dParam ); // virtual
    if ( isError(eRes) )
        return eRes;

    OdUInt32 iNumSegs = this->_numSegments();
    if ( dParam < 0.0 || dParam > iNumSegs )
    {
        FMGE_FAULT( "Contour::getParamAtDist returned an invalid parameter value." );
        return erParamBounds;
    }

    OdUInt32 iIndex = OdUInt32( ::floor(dParam) );
    if ( iIndex == iNumSegs )
        --iIndex;

    TImplSeg2D ImplSeg;
    eRes = worstResult( eRes, this->_getSegment( iIndex, ImplSeg ) );
    if ( isError( eRes ) )
        return eRes;

    return worstResult( eRes, ImplSeg.getPoint( dParam - iIndex, ptPoint ) );
}




template < class TContourData >
Result TContour2DImpl<TContourData>::getDistAtParam (double dParam, double& dDist ) const
{
    return _param2dist( dParam, dDist );
}



template < class TContourData >
Result TContour2DImpl<TContourData>::getParamAtDist (double dDist,  double& dParam ) const
{
    OdUInt32 iIndex;
    double dSegParam;

    Result eRes = _dist2IdxParam( dDist, iIndex, dSegParam );
    if ( isError( eRes ) )
    {
        dParam = 0;
        return eRes;
    }

    dParam = iIndex + dSegParam;

    return eRes;
}





template < class TContourData >
Result TContour2DImpl<TContourData>::_normalizeParam( double & dParam ) const
{
    const double dParamEps = 1e-10; // ToDo: tolerance !!!
    
    int iNumSegs = this->_numSegments();
    if ( iNumSegs <= 0 )
    {
        if ( this->_empty() )
            return erEmptyContour;
        
        if ( ::fabs( dParam ) > dParamEps )
            return erParamBounds;
        
        dParam = 0;
        return erOk;
    }
    
    Result eRes = erOk;
    
    // In closed contours parameter is periodic
    if ( this->_closed() )
    {
        if ( dParam < 0.0 || dParam >= iNumSegs )
        {
            dParam -= ::floor(dParam/iNumSegs)*iNumSegs;
            if ( dParam < 0.0 || dParam >=iNumSegs )
                dParam = 0.0;
        }
    }
    else
    {
        if ( dParam < 0 )
        {
            // eRes = erWarnParamBounds;
            if ( dParam >= -dParamEps )
                dParam = 0.0;
            else
                eRes = erParamBounds;
        }
        else if ( dParam > iNumSegs )
        {
            // eRes = erWarnParamBounds;
            if ( dParam < (iNumSegs+dParamEps) )
                dParam = iNumSegs;
            else
                eRes = erParamBounds;
        }
    }
    
    return eRes;
}


template < class TContourData >
Result TContour2DImpl<TContourData>::getNormalizedParam( double & dParam ) const
{
    return _normalizeParam( dParam );
}



template < class TContourData >
Result TContour2DImpl<TContourData>::getNormalizedDist( double & dDist ) const
{
    Result eRes = erOk;

    double dLength = length();

    const double dTol = FMGeGbl::gTol.equalVector();

    if ( this->_closed() )
    {
        if ( dDist < 0.0 || dDist >= dLength )
        {
            if ( dLength > dTol )
            {
                dDist -= ::floor(dDist/dLength)*dLength;
                if ( dDist < 0.0 || dDist > dLength )
                    dDist = 0;
            }
            else 
                dDist = 0; // empty contour ???
        }
    }
    else // open
    {
        if ( dDist < 0 )
        {
            // eRes = erWarnParamBounds;
            if ( dDist >= -dTol )
                dDist = 0.0;
            else
                eRes = erParamBounds;
        }
        else if ( dDist > dLength )
        {
            // eRes = erWarnParamBounds;
            if ( dDist < (dLength + dTol) )
                dDist = dLength;
            else
                eRes = erParamBounds;
        }
    }

    return eRes;
}


template < class TContourData >
bool TContour2DImpl<TContourData>::areEqualParams( double dParam1, double dParam2, const double dTol ) const
{
    double dDelta = ::fabs( dParam2 - dParam1 );

    if ( this->_closed() )
    {
        OdUInt32 iNumSegs = this->_numSegments();

        if ( iNumSegs > 0 )
        {
            if ( dDelta > iNumSegs )
                dDelta -= ::floor(dDelta/iNumSegs)*iNumSegs;

            if ( 2.0*dDelta > iNumSegs )
                dDelta = iNumSegs - dDelta;
        }
    }

    return ( dDelta <= dTol );
}


template < class TContourData >
bool TContour2DImpl<TContourData>::areEqualDists ( double dDist1, double dDist2, const double dTol ) const
{
    double dDelta = ::fabs( dDist2 - dDist1 );

    if ( this->_closed() )
    {
        if ( dDelta <= dTol )
            return true; // do not evaluate length, if possible

        double dLength = length();
        
        if ( dLength > dTol )
        {
            if ( dDelta > dLength )
                dDelta -= ::floor(dDelta/dLength)*dLength;
            
            if ( 2.0*dDelta > dLength )
                dDelta = dLength - dDelta;
        }
    }
    
    return ( dDelta <= dTol );
}


template < class TContourData >
double TContour2DImpl<TContourData>::signedMinDist( double dDist1, double dDist2 ) const
{
    if ( ! this->_closed() )
    {
        // open contour
        return dDist2 - dDist1;
    }

    // closed contour

    const double dTol = odmin( 1e-10, FMGeGbl::gTol.equalPoint() );

    if ( OdEqual( dDist1, dDist2, dTol ) )
        return 0;

    double dLength = length();

    if ( dLength < dTol )
        return 0;

    if ( dDist1 < 0.0 || dDist1 >= dLength )
        dDist1 -= ::floor(dDist1/dLength)*dLength;

    if ( dDist2 < 0.0 || dDist2 >= dLength )
        dDist2 -= ::floor(dDist2/dLength)*dLength;

    bool bSwap = ( dDist1 > dDist2 );
    if (bSwap)
        std::swap( dDist1, dDist2 );
    
    double dDelta = dDist2 - dDist1;
    
    if ( dDelta > dLength/2 ) 
        dDelta = dLength - dDelta;
    
    return bSwap ? -dDelta : dDelta;
}


template < class TContourData >
double TContour2DImpl<TContourData>::signedMinDistByParams( double dParam1, double dParam2 ) const
{
    if ( dParam1 == dParam2 )
        return 0;

    double dDist1, dDist2;
    Result eRes = worstResult(
        getDistAtParam ( dParam1, dDist1 ),
        getDistAtParam ( dParam2, dDist2 ) );
    if ( isError( eRes ) )
        return DBL_MAX; // ???

    return signedMinDist( dDist1, dDist2 );
}












template < class TContourData >
Result TContour2DImpl<TContourData>::addExtents(OdGeExtents2d& geExtents ) const
{
    if ( this->_empty() )
        return erOk;

    OdGeExtents2d myExts;

    OdUInt32 iNumSeg = this->_numSegments();
    if ( iNumSeg > 0 )
    {
        // Iterate through segments

        TImplSeg2D ImplSeg;
        for (OdUInt32 iSeg = 0; iSeg<iNumSeg; iSeg++ )
        {
            this->_getSegment( iSeg, ImplSeg );
            ImplSeg.addExtents( myExts );
        }
    }
    else
    {
        myExts.addPoint( this->_vertex(0).point() );
    }

    // extend myExts with a tolerance ?

    geExtents.addExt( myExts );

    return erOk;
}




template < class TContourData >
double TContour2DImpl<TContourData>::signedArea() const
{
    // Sum of area of all the segments in contour.
    double dSum = 0;

    OdUInt32 iNumSeg = this->_numSegments();
    if ( iNumSeg > 0 )
    {
        // Iterate through segments and sum doubled areas

        TImplSeg2D ImplSeg;
        for (OdUInt32 iSeg = 0; iSeg<iNumSeg; iSeg++ )
        {
            this->_getSegment( iSeg, ImplSeg );
            dSum += ImplSeg.integrate();
        }

        if ( !this->_closed() )
        {
            // Connect last and first point with a straight line and get integral value
            dSum += BulgeSeg2D( this->_vertex(iNumSeg).point(), this->_vertex(0).point() ).integrate();
        }

        // "integrate" returns doubled area...
        dSum *= 0.5;
    }

    return dSum;
}

template < class TContourData >
bool TContour2DImpl<TContourData>::isConvex() const
{
  bool bResult = false;

  OdUInt32 iSize = this->numSegments();
  if( iSize > 1 && this->_closed() )
  {
    TImplSeg2D rSeg;
    this->_getSegment( iSize-1, rSeg );

    OdGeVector2d vA;
    OdGeVector2d vB;
    rSeg.getTangent( 1.0, vB );

    double dCrossProduct;
    OdUInt32 iFlag = 0;

    // vector product calculation for each vertexes
    for( OdUInt32 i = 0; i < iSize; i++ )
    {

      this->_getSegment( i, rSeg );
      rSeg.getTangent( 0.0, vA );

      /*
        cross (vector) product between two tangent lines (handling line and arc types)
        vectors A=(ax ay), B=(bx by)
        matrix [ ax ay;
                 bx by ];
        determinant of this matrix = vector product = ax*by-ay*bx = |a|*|b|*sin(alfa).
        If sin( alfa ) < 0 ( inner angle alfa > 180 ) then det < 0.
      */
      dCrossProduct = vA.crossProduct( vB.negate() );

      // convex contour not have different signs of product
      if( OdGreaterOrEqual( dCrossProduct, 0.0 ) )
        iFlag |= 1;
      else
        iFlag |= 2;

      // non convex (concave) case
      if( iFlag == 3 )
        return false;

      if( i < iSize-1 ) // updating segments
        vB = vA;
    }
    if( iFlag != 0 )
      bResult = true;
  }
  return bResult;
}


template < class TContourData >
Result TContour2DImpl<TContourData>::getInternalPoint( OdGePoint2d& rPoint, const OdGeTol& gTol ) const
{
    return fast_getInternalPoint( *(const IContour2D*)this, rPoint, gTol );
}



template < class TContourData >
bool TContour2DImpl<TContourData>::isOn( const OdGePoint2d & ptTest, double * pdParam, const OdGeTol & gTol) const
{
    bool bOn = false;
    double dParam = 0;

    OdUInt32 iNumSeg = this->_numSegments();
    if ( iNumSeg > 0 )
    {
        // Iterate through segments and find the first match

        TImplSeg2D ImplSeg;
        for (OdUInt32 iSeg = 0; iSeg<iNumSeg; iSeg++ )
        {
            this->_getSegment( iSeg, ImplSeg );

            double dSegParam;
            bOn = ImplSeg.isOn( ptTest, &dSegParam, gTol );
            if (bOn)
            {
                dParam = iSeg + dSegParam;
                break;
            }
        }
    }
    else if ( !this->_empty() )
    {
        // single point contour
        bOn = BulgeSeg2D( this->_vertex(0).point(), this->_vertex(0).point() ).isOn( ptTest, &dParam, gTol);
    }

    if (pdParam)
        *pdParam = dParam;

    return bOn;
}



template < class TContourData >
double TContour2DImpl<TContourData>::nearestParam( const OdGePoint2d & ptTest, OdGePoint2d * pptNearest) const
{
    if ( this->_empty() )
    {
        if (pptNearest)
            *pptNearest = ptTest; // What should I return here ?
        return -1.0;
    }

    // Nearest point and it`s parameter
    double dNearestParam = 0;
    OdGePoint2d ptNearest = this->_vertex(0).point();

    // Minimal distance
    double dMinDist = ptNearest.distanceTo( ptTest );

    const double dZeroDist = 1e-15; // stop search, if distance is <= this value

    OdUInt32 iNumSeg = this->_numSegments();
    if ( ( iNumSeg > 0 ) && (dMinDist > dZeroDist) )
    {
        // Iterate through segments and find the best match

        OdGePoint2d ptTmp;
        TImplSeg2D ImplSeg;

        for (OdUInt32 iSeg = 0; iSeg<iNumSeg; iSeg++ )
        {
            this->_getSegment( iSeg, ImplSeg );

            double dSegParam = ImplSeg.nearestParam( ptTest, &ptTmp );

            double dDist = ptTmp.distanceTo( ptTest );

            if ( dDist < dMinDist )
            {
                // This segment is more close
                dMinDist = dDist;
                dNearestParam = iSeg + dSegParam;
                ptNearest = ptTmp;

                if ( dMinDist <= dZeroDist )
                    break;
            }
        }
    }

    if (pptNearest)
        *pptNearest = ptNearest;

    return dNearestParam;
}



template < class TContourData >
bool TContour2DImpl<TContourData>::contains( const OdGePoint2d &rPoint, bool * pbOnBorder, const OdGeTol & gTol) const
{
    if (pbOnBorder)
        *pbOnBorder = false;

    if ( this->_empty() )
        return false;

    if ( !this->_closed() )
        return false; // or close it?

    bool bInside = false;


    OdUInt32 iNumSeg = this->_numSegments();
    if ( iNumSeg > 0 )
    {
        // Iterate through segments and find the first match

        TImplSeg2D ImplSeg;
        for (OdUInt32 iSeg = 0; iSeg<iNumSeg; iSeg++ )
        {
            this->_getSegment( iSeg, ImplSeg );

            bool bOnSegment = false;
            OdUInt32 iCount = ImplSeg.intersectXRay( rPoint, &bOnSegment, gTol );
            if (bOnSegment)
            {
                if (pbOnBorder)
                    *pbOnBorder = true;
                bInside = true;
                break;
            }
            if ( iCount&1 )
                bInside = !bInside;
        }
    }
    else if ( !this->_empty() )
    {
        // single point contour
        bInside = BulgeSeg2D( this->_vertex(0).point(), this->_vertex(0).point() ).isOn( rPoint, 0, gTol);
        if (pbOnBorder)
            *pbOnBorder = bInside;
    }

    return bInside;
}





template < class TContourData >
bool TContour2DImpl<TContourData>::isSelfIntersecting(const OdGeTol & gTol, bool bExcludeTouch /*= false*/ ) const
{
    OdUInt32 iNumSegs = this->_numSegments();

    if ( iNumSegs <= 1 )
        return false;

    // use fast ( N log N ) check if iNumSegs is big
    // ToDo: profile the code and find good condition
    if ( iNumSegs > 16 && !bExcludeTouch )
    {
        bool bIntersects = false;
        Result eRes = fast_isSelfIntersecting( *(const IContour2D*)(this), gTol, bIntersects );
        if ( isOk(eRes) )
        {
            return bIntersects;
        }
        // else => try the default method
    }

    Intersection XPt[2];

    TImplSeg2D ImplSegA; // ToDo: use cached segment here instead of the ImplSeg
    TImplSeg2D ImplSegB;

    for (OdUInt32 iSegA = 0; iSegA < iNumSegs; iSegA++ )
    {
        this->_getSegment( iSegA, ImplSegA );

        for (OdUInt32 iSegB = 0; iSegB < iSegA; iSegB++ )
        {
            this->_getSegment( iSegB, ImplSegB );

            OdUInt32 iCrosses = ImplSegA.intersect( ImplSegB, &(XPt[0]), &(XPt[1]), gTol );
            if ( iCrosses > 2 )
                iCrosses = 2;

            // do not count self-touch as self-intersection
#if 1
            if(bExcludeTouch && (iSegA != iNumSegs-1 || iSegB != 0) && iCrosses == 1 && Intersection::eitTouch == XPt[0].eType && !this->areEqualParams( iSegB + XPt[0].dParamB, iSegA + XPt[0].dParamA, 1e-14 ))
            {
              const bool bAisTouchEnd = OdEqual(XPt[0].dParamA, 1., 1e-10);
              const bool bAisTouchBeg = OdEqual(XPt[0].dParamA, 0., 1e-10);
              ODA_ASSERT(!bAisTouchEnd || !bAisTouchBeg);
              const bool bBisTouchEnd = OdEqual(XPt[0].dParamB, 1., 1e-10);
              const bool bBisTouchBeg = OdEqual(XPt[0].dParamB, 0., 1e-10);
              ODA_ASSERT(!bBisTouchEnd || !bBisTouchBeg);
              const bool bAisTouch = bAisTouchEnd || bAisTouchBeg;
              const bool bBisTouch = bBisTouchEnd || bBisTouchBeg;
              if(bAisTouch && bBisTouch)
              {
                const OdGeVector2d vA1 = bAisTouchEnd ? ImplSegA.startPt() - ImplSegA.endPt() : ImplSegA.endPt() - ImplSegA.startPt();
                int iNext = ((iSegA + (bAisTouchEnd ? 1 : -1)) + iNumSegs) % iNumSegs;
                TImplSeg2D ImplNext;
                this->_getSegment( iNext, ImplNext );
                const OdGeVector2d vA2 = bAisTouchEnd ? ImplNext.endPt() - ImplSegA.endPt() : ImplNext.startPt() - ImplSegA.startPt();

                const OdGeVector2d vB1 = bBisTouchEnd ? ImplSegB.startPt() - ImplSegB.endPt() : ImplSegB.endPt() - ImplSegB.startPt();
                iNext = ((iSegB + (bBisTouchEnd ? 1 : -1)) + iNumSegs) % iNumSegs;
                this->_getSegment( iNext, ImplNext );
                const OdGeVector2d vB2 = bBisTouchEnd ? ImplNext.endPt() - ImplSegB.endPt() : ImplNext.startPt() - ImplSegB.startPt();

                double angA = vA1.angleToCCW(vA2);
                if(angA < 0)
                {
                  angA += Oda2PI;
                }
                double angB1 = vA1.angleToCCW(vB1);
                if(angB1 < 0)
                {
                  angB1 += Oda2PI;
                }
                double angB2 = vA1.angleToCCW(vB2);
                if(angB2 < 0)
                {
                  angB2 += Oda2PI;
                }
                if((angB1 < angA && angB2 < angA) || (angB1 > angA && angB2 > angA))
                {
                  continue;
                }
              }
              else if(bAisTouch)
              {
                int iNext = ((iSegA + (bAisTouchEnd ? 1 : -1)) + iNumSegs) % iNumSegs;
                TImplSeg2D ImplNext;
                this->_getSegment( iNext, ImplNext );
                const OdGePoint2d pt1 = bAisTouchEnd ? ImplNext.endPt() : ImplNext.startPt();
                const OdGePoint2d pt2 = bAisTouchEnd ? ImplSegA.startPt() : ImplSegA.endPt();

                OdGeVector2d vLine = ImplSegB.endPt() - ImplSegB.startPt();
                const double sign1 = vLine.crossProduct(pt1 - ImplSegB.startPt());
                const double sign2 = vLine.crossProduct(pt2 - ImplSegB.startPt());
                if(sign1*sign2 > -1e-14)
                {
                  continue;
                }
              }
              else if(bBisTouch)
              {
                int iNext = ((iSegB + (bBisTouchEnd ? 1 : -1)) + iNumSegs) % iNumSegs;
                TImplSeg2D ImplNext;
                this->_getSegment( iNext, ImplNext );
                const OdGePoint2d pt1 = bBisTouchEnd ? ImplNext.endPt() : ImplNext.startPt();
                const OdGePoint2d pt2 = bBisTouchEnd ? ImplSegB.startPt() : ImplSegB.endPt();

                OdGeVector2d vLine = ImplSegA.endPt() - ImplSegA.startPt();
                const double sign1 = vLine.crossProduct(pt1 - ImplSegA.startPt());
                const double sign2 = vLine.crossProduct(pt2 - ImplSegA.startPt());
                if(sign1*sign2 > -1e-14)
                {
                  continue;
                }
              }
            }
#endif

            // Handle crossings
            for ( OdUInt32 iC = 0; iC < iCrosses; iC++ )
            {
                if ( !XPt[iC].eType )
                    continue;

                // Note: this way will not count intersections on segment bounds
                if ( Intersection::eitCrossing == XPt[iC].eType )
                    return true;
                /*if (bExcludeTouch && Intersection::eitTouch == XPt[iC].eType )
                    continue;*/

                if ( this->areEqualParams( iSegB + XPt[iC].dParamB, iSegA + XPt[iC].dParamA, 1e-14 ) )
                    continue; // equal parameters => no self-intersection

                double dDist = this->signedMinDistByParams( iSegB + XPt[iC].dParamB, iSegA + XPt[iC].dParamA );
                if ( OdZero( dDist, gTol.equalPoint() ) )
                    continue; // equal distances => no self-intersection

                return true;
            }
        }
    }

    return false;
}


template < class TContourData >
bool TContour2DImpl<TContourData>::isValidRegion( const OdGeTol& gTol ) const
{
    bool bValidRegion = false;

    fast_isValidRegion( *(const IContour2D*)(this), gTol, bValidRegion );

    return bValidRegion;
}

template < class TContourData >
bool TContour2DImpl<TContourData>::isInsideContour(const IContour2D& c2dOuter, bool fAllowBordersTouch, const OdGeTol& gTol) const
{
  // this function is valid for contours with straight segments only
  ODA_ASSERT(!this->hasArcs());
  ODA_ASSERT(!c2dOuter.hasArcs());

  const OdUInt32 numVertices = this->numVerts();
  OdGePoint2d ptVertex;
  for(OdUInt32 uVertexIdx = 0; uVertexIdx < numVertices; uVertexIdx++)
  {
    this->getVertexAt(uVertexIdx, &ptVertex);
   
    bool fOnBorder = false;
    if(!c2dOuter.contains(ptVertex, &fOnBorder))
      return false;

    if(!fAllowBordersTouch && fOnBorder)
      return false;
  }

  return true;
}


template < class TContourData >
OdUInt32 TContour2DImpl<TContourData>::intersect( const IBulgeSeg2D & rSegB,
                           std::vector< Intersection > & vecPoints,
                           const OdGeTol & gTol) const
{
    if ( this->_empty() )
        return 0;

    // ToDo: use cached segment here instead of rSegB

    OdUInt32 iOrigCrossCount = (OdUInt32)vecPoints.size();

    OdUInt32 iNumSegs = this->_numSegments();

    Intersection XPt[2];

    if ( iNumSegs > 0 )
    {
        TImplSeg2D ImplSeg;
        // ToDo: use cached segment here instead of the ImplSeg

        for (OdUInt32 iSeg = 0; iSeg < iNumSegs; iSeg++ )
        {
            this->_getSegment( iSeg, ImplSeg );

            OdUInt32 iCrosses = ImplSeg.intersect( rSegB, &(XPt[0]), &(XPt[1]), gTol );
            if ( iCrosses > 2 ) // overlapping seb-segments
                iCrosses = 2; // use only bounds

            for ( OdUInt32 iC = 0; iC < iCrosses; iC++ )
                if ( XPt[iC].eType != Intersection::eitNone )
                {
                    if ( XPt[iC].eType != Intersection::eitCrossing )
                        XPt[iC].eType = Intersection::eitAny; // To Do: update intersection type

                    XPt[iC].dParamA += iSeg;
                    vecPoints.push_back( XPt[iC] );
                }
        }
    }
    else if ( this->_numVerts() > 0 ) // Handle 1-point contour
    {
        const OdGePoint2d & ptVert = this->_vertex(0).point();

        double dBParam = 0;
        if ( rSegB.isOn( ptVert, &dBParam, gTol ) )
        {
            XPt[0].ptPoint = ptVert;
            XPt[0].dParamA = 0;
            XPt[0].dParamB = dBParam;
            XPt[0].eType = Intersection::eitTouch;

            vecPoints.push_back( XPt[0] );
        }
    }

    return (OdUInt32)vecPoints.size() - iOrigCrossCount;
}




template < class TContourData >
OdUInt32 TContour2DImpl<TContourData>::intersect( const IContour2D& rContB,
                           std::vector< Intersection > & vecPoints,
                           const OdGeTol & gTol ) const
{
    if ( this->_empty() || rContB.isEmpty() )
        return 0;

    OdUInt32 iNumSegs = this->_numSegments();
    //OdUInt32 iNumSegsB= rContB.numSegments();

    OdUInt32 iOrigCrossCount = (OdUInt32)vecPoints.size();
    OdUInt32 iCrossCount = iOrigCrossCount;

    // use fast ( NLogN ) intersection method and caching in big contours
    // ToDo: profile the code and find good condition
    if ( iNumSegs > 16 || rContB.numSegments() > 16 )
    {
        Result eRes = fast_intersect( *(const IContour2D*)(this), rContB, vecPoints, gTol );
        if ( isOk(eRes) )
        {
            return (OdUInt32)vecPoints.size() - iOrigCrossCount;
        }
        // else => try the default method
    }

    if ( iNumSegs > 0 )
    {
        TImplSeg2D ImplSeg;
        // ToDo: use cached segment here instead of the ImplSeg

        for (OdUInt32 iSeg = 0; iSeg < iNumSegs; iSeg++ )
        {
            this->_getSegment( iSeg, ImplSeg );

            //OdUInt32 iCrosses =
            rContB.intersect( ImplSeg, vecPoints, gTol );

            //FMGE_ASSERT( (vecPoints.size() - iCrossCount) == iCrosses );

            while ( iCrossCount < vecPoints.size() )
            {
                Intersection & rCross = vecPoints[iCrossCount];

                double dBParam = rCross.dParamA;
                rCross.dParamA = iSeg + rCross.dParamB;
                rCross.dParamB = dBParam;

                // To Do: check and remove duplicates
                // To Do: update intersection type
                rCross.eType = Intersection::eitAny;

                ++iCrossCount;
            }
        }
    }
    else if ( this->_numVerts() > 0 ) // Handle 1-point contour
    {

        BulgeSeg2D ASeg( this->_vertex(0).point(), this->_vertex(0).point() );

        //OdUInt32 iCrosses =
        rContB.intersect( ASeg, vecPoints, gTol );

        while ( iCrossCount < vecPoints.size() )
        {
            Intersection & rCross = vecPoints[iCrossCount];
            rCross.dParamB = rCross.dParamA;
            rCross.dParamA = 0.0;
            rCross.eType = Intersection::eitTouch;
            ++iCrossCount;
        }
    }

    return iCrossCount - iOrigCrossCount;
}


template < class TContourData >
OdUInt32 TContour2DImpl<TContourData>::intersectLine( 
    const OdGePoint2d& ptLineOrigin, 
    const OdGeVector2d& vLineDir,
    std::vector< Intersection > & vecPoints,
    const OdGeTol & gTol) const
{
    if ( this->_empty() || vLineDir.isZeroLength( gTol ) )
        return 0;

    OdUInt32 iNumSegs = this->_numSegments();

    OdUInt32 iOrigCrossCount = (OdUInt32)vecPoints.size();

    // ToDo: use fast ( NLogN ) intersection method and caching in big contours
//    if ( iNumSegs > 16 || rContB.numSegments() > 16 )
//    {
//        Result eRes = fast_intersectLine( *(const IContour2D*)(this), rContB, vecPoints, gTol );
//        if ( isOk(eRes) )
//        {
//            return vecPoints.size() - iOrigCrossCount;
//        }
//        // else => try the default method
//    }

    Intersection XPt[2];

    if ( iNumSegs > 0 )
    {
        TImplSeg2D ImplSeg;

        for (OdUInt32 iSeg = 0; iSeg < iNumSegs; iSeg++ )
        {
            this->_getSegment( iSeg, ImplSeg );

            OdUInt32 iCrosses = ImplSeg.intersectLine( ptLineOrigin, vLineDir, &(XPt[0]), &(XPt[1]), gTol );
            if ( iCrosses > 0 )
            {
                if ( iCrosses > 2 )
                    iCrosses = 2;

                for ( OdUInt32 iC = 0; iC < iCrosses; iC++ )
                    if ( XPt[iC].eType != Intersection::eitNone )
                    {
                        if ( XPt[iC].eType != Intersection::eitCrossing )
                            XPt[iC].eType = Intersection::eitAny; // To Do: update intersection type

                        XPt[iC].dParamA += iSeg;
                        vecPoints.push_back( XPt[iC] );
                    }
            }
        }
    }
    else if ( this->_numVerts() > 0 ) // Handle 1-point contour
    {
        BulgeSeg2D ASeg( this->_vertex(0).point(), this->_vertex(0).point() );

        OdUInt32 iCrosses = ASeg.intersectLine( ptLineOrigin, vLineDir, &(XPt[0]), 0, gTol );

        if ( iCrosses > 0 && XPt[0].eType != Intersection::eitNone )
        {
            XPt[0].eType = Intersection::eitTouch;
            XPt[0].dParamA = 0.0;
            vecPoints.push_back( XPt[0] );
        }
    }

    return (OdUInt32)vecPoints.size() - iOrigCrossCount;
}


template < class TContourData >
Result TContour2DImpl<TContourData>::transformBy(const OdGeMatrix2d& geMatrix,
    const DeviationParams& devDeviation )
{
    // isScaledOrtho undefined!!!
    //if ( !geMatrix.isScaledOrtho( FMGeGbl::gTol ) )
    //{
    //    FMGE_ASSERT( 0 );
    //    return erFail;
    //}

    bool bExplodeRequired = false;
    {
        OdGePoint2d origin;
        OdGeVector2d x, y;
        geMatrix.getCoordSystem(origin, x, y);

        if ( !OdEqual( x.length(), y.length(), FMGeGbl::gTol.equalPoint() ) )
        {
            bExplodeRequired = true;
        }
    }

    if ( bExplodeRequired )
    {
        // Initial version.
        // 
        this->explodeTo( *this, devDeviation );
    }

    OdUInt32 iNumVerts = this->_numVerts();
    if ( this->_numVerts() <= 0 )
        return erOk;

    bool bInverse = ( geMatrix.det() < 0 );

    for ( OdUInt32 iVert = 0; iVert<iNumVerts; iVert++ )
    {
        TVertexData & rVert = this->_vertex(iVert);
        rVert.point().transformBy( geMatrix );
        if ( bInverse )
            rVert.bulge() = -rVert.bulge();
    }

    this->_setModifiedAll();

    return erOk;
}



template < class TContourData >
void TContour2DImpl<TContourData>::reverse()
{
    OdUInt32 iNumVerts = this->_numVerts();
    if ( this->_numVerts() <= 0 )
        return;// erOk;

    if ( ! this->_closed() )
    {
        // Open contour -- swap all vertices except the odd middle one
        OdUInt32 iSwapNum = iNumVerts/2;

        for ( OdUInt32 iS = 0; iS < iSwapNum; iS++ )
        {
            TVertexData & rVertA = this->_vertex(iS);
            OdGePoint2d& rPtB   = this->_vertex(iNumVerts - 1 - iS).point();

            std::swap( rVertA.point(), rPtB );

            TVertexData & rVertB = this->_vertex(iNumVerts - 2 - iS);

            std::swap( rVertA.bulge(), rVertB.bulge() );
            std::swap( rVertA.attributes(), rVertB.attributes() );
        }
    }
    else
    {
        // Closed contour -- swap all except the first vertex and the even middle one

        // swap points
        {
            OdUInt32 iPSwapNum = (iNumVerts-1)/2;
            for ( OdUInt32 iS = 0; iS < iPSwapNum; iS++ )
            {
                OdGePoint2d& rPtA   = this->_vertex(iS+1).point();
                OdGePoint2d& rPtB   = this->_vertex(iNumVerts - iS - 1).point();
                std::swap( rPtA, rPtB );
            }
        }

        // Swap bulges and metadata
        {
            OdUInt32 iBSwapNum = iNumVerts/2;
            for ( OdUInt32 iS = 0; iS < iBSwapNum; iS++ )
            {
                TVertexData & rVertA = this->_vertex(iS);
                TVertexData & rVertB = this->_vertex(iNumVerts - iS - 1);

                std::swap( rVertA.bulge(), rVertB.bulge() );
                std::swap( rVertA.attributes(), rVertB.attributes() );
            }
        }
    }

    // reverse bulges and attributes
    {
        for (OdUInt32 i = 0, iSize = numVerts(); i < iSize; i++ )
        {
            TVertexData & rVert = this->_vertex(i);
            rVert.bulge() = -rVert.bulge();
            rVert.attributes().reverse();
        }
    }

    this->_setModifiedAll();

    //return erOk;
}



template < class TContourData >
void TContour2DImpl<TContourData>::deleteCoincident( const OdGeTol & gTol )
{
    OdUInt32 iNumVerts = this->_numVerts();
    if ( this->_numVerts() <= 1 )
        return;// erOk;

    TVertexData * pPrevVert = & this->_vertex(0);

    // number of ready points
    OdUInt32 iNumHandled = 1;

    for ( OdUInt32 iVert = 1; iVert<iNumVerts; iVert++)
    {
        TVertexData& rThisVert = this->_vertex(iVert);
        if ( rThisVert.point().isEqualTo( pPrevVert->point(), gTol ) )
        {
            // equal points, override metadata of zero-length segment
            pPrevVert->bulge() = rThisVert.bulge();
            pPrevVert->attributes() = rThisVert.attributes();
        }
        else // different points
        {
            pPrevVert = & this->_vertex(iNumHandled);
            if ( iNumHandled < iVert )
                *pPrevVert = rThisVert;

            iNumHandled++;
        }
    }

    if ( this->_closed() )
    {
        if ( this->_vertex(0).point().isEqualTo( pPrevVert->point(), gTol ) )
        {
            iNumHandled--; // remove the duplicate endpoint
        }
    }
    else
    {
        // ensure the last vertex and bulge of an open contour are preserved
        if ( iNumHandled < (iNumVerts-1) )
            this->_vertex(iNumHandled) = this->_vertex( iNumVerts-1 );
    }

    if ( iNumHandled < iNumVerts )
    {
         // resize and reset cache
        this->_resize( iNumHandled );
        this->_setModifiedAll();
    }
}


template < class TContourData >
void TContour2DImpl<TContourData>::mergeSegments( int iMergeFlags, const OdGeTol& gTol )
{
    contour2d_mergeSegments( *static_cast<IContour2D*>(this), iMergeFlags, gTol );
}


template < class TContourData >
Result TContour2DImpl<TContourData>::explodeTo( IContour2D & rDestCont, 
    const DeviationParams& devDeviation, 
    OdIntPtr uArcMetadata ) const
{
    // aliasing check:
    bool bThisDest = ( ((const IContour2D*)this) == &rDestCont );
    
    bool bClosed = this->_closed();

    OdUInt32 iNumVerts = this->_numVerts();
    OdUInt32 iNumSegs = this->_numSegments();

    Result eRes = erOk;

    if ( iNumSegs < 1 ) // 0 or 1 point(s)
    {
        if ( !bThisDest )
            rDestCont.set( *this );

        return erOk;
    }


    //////////////////////////////////////////////////////////////////////////
    // first pass: get points and store metadata

    OdGePoint2dArray vecPoints;

    typedef std::pair< OdUInt32, Attributes2D > SegDataPair;
    std::vector< SegDataPair > vecAttributes;

    vecAttributes.reserve( iNumVerts + 1);
    vecPoints.reserve( iNumVerts );
    
    TImplSeg2D ImplSeg;
    CachedSeg2D arcSeg;

    bool bHasArcs = false;

    for ( OdUInt32 iSeg = 0; iSeg < iNumSegs; iSeg++ )
    {
        this->_getSegment( iSeg, ImplSeg );

        Attributes2D Attr = ImplSeg.attributes();
        vecAttributes.push_back( SegDataPair( vecPoints.size(), Attr ) );

        vecPoints.push_back( ImplSeg.startPt() );

        if ( ImplSeg.type() == estArc )
        {
            bHasArcs = true;
            vecAttributes.back().second.setExplodedArc(true).metadata() |= uArcMetadata; // ATTRTODO

            eRes = arcSeg.set( ImplSeg );
            FMGE_ASSERT( erOk == eRes );
            if ( isError(eRes) )
                return eRes;
            
            OdUInt32 iSegmentCount = FMGeGbl::GetSegmentCount(
                ::fabs( arcSeg.arcAngle() ), arcSeg.arcRadius(), devDeviation );

            // vecPoints.reserve( ... )

            // insert intermediate points
            OdGePoint2d ptVertex;
            for ( OdUInt32 iPt = 1; iPt<iSegmentCount; iPt++ )
            {
                eRes = arcSeg.getPoint( double(iPt)/iSegmentCount, ptVertex );
                if ( isError(eRes) )
                    return eRes;
                vecPoints.push_back( ptVertex );
            }
        }
    }
    
    // append the last vertex for non-closed contours
    // ( in closed contours it coincides with the first one )
    if ( ! bClosed )
    {
        const TVertexData & rLast = this->_vertex( iNumVerts-1 );
        FMGE_ASSERT( ImplSeg.endPt().isEqualTo( rLast.point() ) );

        vecAttributes.push_back( SegDataPair( vecPoints.size(), rLast.attributes() ) );
        vecPoints.push_back( rLast.point() );
    }

    vecAttributes.push_back( SegDataPair( vecPoints.size(), Attributes2D::kNull ) ); // terminating record

    //////////////////////////////////////////////////////////////////////////
    // second pass: set points and metadata

    if ( !bHasArcs ) // No arcs => no action
    {
        if ( !bThisDest ) // copy, if necessary
            rDestCont.set( *this );

        return erOk;
    } 

    {
        rDestCont.reset();
        rDestCont.setClosed( bClosed );

        eRes = rDestCont.appendVertices( vecPoints );
        if ( isOk( eRes ) )
        {
            // update metadata (slower)
            for ( OdUInt32 uI = 1; uI < vecAttributes.size(); ++uI )
            {
                const Attributes2D& rAttr = vecAttributes[ uI-1 ].second;
                if ( !rAttr.isNull() )
                {
                    OdUInt32 uVert = vecAttributes[ uI-1 ].first;
                    OdUInt32 uLastVert = vecAttributes[ uI ].first;
                    
                    for ( ;  uVert < uLastVert; ++uVert )
                        rDestCont.setAttributesAt( uVert, rAttr );
                }
            }
        }
    }
    
    return eRes;
}



template < class TContourData >
OdUInt32 TContour2DImpl<TContourData>::createVertexAt( double dParam, const OdGeTol & gTol )
{
    OdUInt32 iIndex;
    double dSegParam;
    Result eRes = _param2IdxParam( dParam, iIndex, dSegParam );
    if ( isError(eRes) )
        return OdUInt32(-1);

    TImplSeg2D ImplSeg;
    eRes = this->_getSegment( iIndex, ImplSeg );
    FMGE_ASSERT( erOk==eRes );
    if ( isError(eRes) )
        return OdUInt32(-1);

    // Segment`s length is used to compare with tolerance
    double dLen = ImplSeg.length();

    const double dTol = gTol.equalVector();

    // Check if the vertex already exists

    if ( dSegParam*dLen <= dTol )
    {
        FMGE_ASSERT( dSegParam*dLen >= -dTol );
        return iIndex; // The vertex exists
    }
    else if ( (1.0 - dSegParam)*dLen <= dTol )
    {
        FMGE_ASSERT( (1.0 - dSegParam)*dLen >= -dTol );
        return (iIndex+1) % this->_numVerts(); // The vertex exists
    }

    // The vertex should be created
    OdUInt32 iNewIndex = (iIndex+1);

    // Get the new point
    OdGePoint2d ptSplit;
    eRes = ImplSeg.getPoint( dSegParam, ptSplit);
    FMGE_ASSERT( erOk==eRes );
    if ( isError(eRes) )
        return OdUInt32(-1);

    Attributes2D Attr = ImplSeg.attributes();

    if ( ImplSeg.type()==estArc )
    {
        // calculate new bulges
        double dAngle = ImplSeg.arcAngle();
        double dBulge1 = ::tan( 0.25 * dAngle * dSegParam );
        double dBulge2 = ::tan( 0.25 * dAngle * (1.0 - dSegParam) );

        // ImplSeg can become invalid, if reallocation happens

        this->_vertex( iIndex ).bulge() = dBulge1;
        this->_setModifiedSegs( iIndex );
        eRes = this->_insertVerticesAt(iNewIndex, 1, &ptSplit, &dBulge2, &Attr );
    }
    else
    {
        // No bulges
        if ( ImplSeg._bulge() != 0.0 )
        {
            this->_vertex( iIndex ).bulge() = 0.0;
            this->_setModifiedSegs( iIndex );
        }

        eRes = this->_insertVerticesAt(iNewIndex, 1, &ptSplit, 0, &Attr );
    }

    FMGE_ASSERT( erOk==eRes );
    if ( isError( eRes ) )
        return OdUInt32(-1);

    return iNewIndex;
}


template < class TContourData >
Result TContour2DImpl<TContourData>::_createVerticesAt( OdUInt32 uCount, const double * pdParams, const OdGeTol & gTol )
{
    FMGE_ASSERT( 0!= pdParams );
    if ( uCount<1 || !pdParams )
        return erOk;

    typedef std::pair< OdUInt32, double > SegOffsPair;
    std::vector< SegOffsPair > vecParams;
    vecParams.reserve( uCount );

    Result eBigRes = erOk;

    // convert contour parameters to segment/segParam pairs and sort
    {
        SegOffsPair currSO;

        for ( OdUInt32 uP = 0; uP < uCount; ++uP )
        {
            Result eRes = _param2IdxParam( pdParams[uP], currSO.first, currSO.second );
            eBigRes = worstResult( eBigRes, eRes );
            if ( isOk( eRes ) )
                vecParams.push_back( currSO );
        }

        std::sort( vecParams.begin(), vecParams.end() );
    }


    if ( !vecParams.empty() )
    {
        TImplSeg2D ImplSeg;
        CachedSeg2D currSeg;
        Result eRes = erOk;

        std::vector< double > vecSegParams;
        std::vector< OdGePoint2d > vecSegPoints;
        std::vector< double > vecSegBulges;
        std::vector< Attributes2D > vecSegAttributes;
        //vecSegParams.reserve( vecParams.size() + 1);

        const double dTol = gTol.equalVector();

        // Walk through the params from end to start, to avoid unnecessary recalculation
        OdUInt32 uSEnd = (OdUInt32)vecParams.size();
        while ( uSEnd > 0 )
        {
            OdInt32 uSBegin = uSEnd - 1;
            OdUInt32 uSeg = vecParams[uSBegin].first;
            while ( uSBegin > 0 && ( uSeg == vecParams[uSBegin-1].first ) )
                --uSBegin;

            // handle points [ uSBegin .. uSEnd ) lying on uSeg
            eRes = this->_getSegment( uSeg, ImplSeg );
            FMGE_ASSERT( erOk==eRes );
            currSeg.set( ImplSeg );
            
            // Segment`s length is used to compare with tolerance
            double dLen = currSeg.length();

            // add new parameters to vecSegParams
            {
                vecSegParams.resize( 0 );

                for ( OdUInt32 uP = uSBegin; uP < uSEnd; ++uP )
                {
                    double dSegParam = vecParams[uP].second;
                    if ( dSegParam*dLen > dTol && (1.0 - dSegParam)*dLen > dTol )
                    {
                        // point lies inside the segment
                        if (    vecSegParams.empty() 
                            || !OdEqual( vecSegParams.back(), dSegParam, dTol ) )
                        {
                            // add it, skipping duplicates
                            vecSegParams.push_back( dSegParam );
                        }
                    }
                }
            }

            if ( !vecSegParams.empty() )
            {
                OdUInt32 uNewNum = (OdUInt32)vecSegParams.size();

                // fill points
                vecSegPoints.resize( uNewNum );
                {
                    for ( OdUInt32 uP = 0 ; uP < uNewNum; ++uP )
                    {
                        eRes = currSeg.getPoint( vecSegParams[uP], vecSegPoints[uP] );
                        FMGE_ASSERT( isOk(eRes) );
                    }
                }

                // fill metadata, if used
                const Attributes2D * pAttributes = 0;
                vecSegAttributes.resize( 0 );
                if ( !currSeg.attributes().isNull() )
                {
                    vecSegAttributes.resize( uNewNum, currSeg.attributes() );
                    pAttributes = &( vecSegAttributes.front() );
                }

                if ( currSeg.type() == estArc )
                {
                    // recalculate bulges
                    {
                        vecSegBulges.resize( uNewNum+1 );
                        vecSegParams.push_back( 1.0 );

                        double dAngle4 = currSeg.arcAngle()/4.0;

                        double dPrevParam = 0;
                        for ( OdUInt32 uP = 0; uP <= uNewNum; ++uP )
                        {
                            vecSegBulges[ uP ] = ::tan( dAngle4 * ( vecSegParams[uP] - dPrevParam ) );
                            dPrevParam = vecSegParams[uP];
                        }
                    }

                    this->_vertex( uSeg ).bulge() = vecSegBulges[0];
                    this->_setModifiedSegs( uSeg );

                    // add vertices with bulges and metadata
                    eRes = this->_insertVerticesAt( uSeg + 1, uNewNum, 
                        & ( vecSegPoints[0] ), 
                        & ( vecSegBulges[1] ), 
                        pAttributes );
                    
                    eBigRes = worstResult( eBigRes, eRes );
                }
                else
                {
                    // linear segment

                    // reset bulge, if it was not exactly zero
                    if ( ImplSeg._bulge() != 0.0 )
                    {
                        this->_vertex( uSeg ).bulge() = 0.0;
                        this->_setModifiedSegs( uSeg );
                    }

                    // add vertices with 0 bulges and metadata
                    eRes = this->_insertVerticesAt( uSeg + 1, uNewNum, &( vecSegPoints.front() ), 0, pAttributes );

                    eBigRes = worstResult( eBigRes, eRes );
                }
            }

            uSEnd = uSBegin; 
        }

    }

    return eBigRes;
}


template < class TContourData >
Result TContour2DImpl<TContourData>::createVerticesAt( const std::vector<double> & vecParams, const OdGeTol & gTol )
{
    return _createVerticesAt( (OdUInt32)vecParams.size(), &( vecParams.front() ), gTol );
}

template <class TContourData>
Result TContour2DImpl<TContourData>::createVerticesAt(OdUInt32 size, const double* vecParams, const OdGeTol& gTol)
{
    return _createVerticesAt( size, vecParams, gTol );
}


template < class TContourData >
Result TContour2DImpl<TContourData>::_paramRange2IdxParams( 
    const double dParamA,                   const double dParamB, 
    OdUInt32& uIdxA, double& dSegParamA,    OdUInt32& uIdxB, double& dSegParamB,
    OdUInt32& iNumPeriodsAB, const double dParamTol ) const
{
    iNumPeriodsAB = 0;

    OdUInt32 iNumSegs = this->_numSegments();
    if ( iNumSegs <= 0 )
    {
        uIdxA = uIdxB = 0;
        dSegParamA = dSegParamB = 0;
        if ( this->_numVerts() > 0 && OdZero(dParamA, dParamTol) && OdZero(dParamB, dParamTol) )
            return erWarnEmptyContour;

        return erInvalidIndex;
    }

    bool bReverse = (dParamA > dParamB);

    // split into segment index and fractional segment parameter
    long   aiIdx[2];
    double adSegParams[2];
    {
        for (int i = 0; i<2; ++i )
        {
            double dIdx;
            adSegParams[i] = ::modf( i==int(bReverse) ? dParamA : dParamB, &dIdx );
            aiIdx[i] = long( ::floor( dIdx + 0.5 ) );
            if ( adSegParams[i] < 0 )
            {
                adSegParams[i] += 1;
                aiIdx[i]       -= 1;
            }
        }
    }

    // adjust upper/lower bounds
    {
        if ( adSegParams[0] >= 1.0-dParamTol )
        {
            adSegParams[0] = 0.0;
            aiIdx[0] += 1;
        } 
        else if ( adSegParams[0] <= dParamTol )
        {
            adSegParams[0] = 0.0;
        }

        if ( adSegParams[1] <= dParamTol )
        {
            adSegParams[1] = 1.0;
            aiIdx[1] -= 1;
        }
        else if ( adSegParams[1] >= 1.0-dParamTol )
        {
            adSegParams[1] = 1.0;
        }
    }

    Result eRes = erOk;

    if ( aiIdx[0] > aiIdx[1] || ( aiIdx[0] == aiIdx[1] && adSegParams[0] >= adSegParams[1] ) )
    {
        FMGE_ASSERTMSG( OdZero( (aiIdx[0]+adSegParams[0]) - (aiIdx[1]+adSegParams[1]), 2*dParamTol ), 
            "Critical floating-point inaccuracy!" );

        aiIdx[1] = aiIdx[0];
        adSegParams[1] = adSegParams[0];

        eRes = erWarnEmptyContour;
    }


    if ( this->_closed() )
    {
        // contour is closed => remove periods

        ldiv_t aPeriodsIdxs[2];
        for (int i=0; i<2; ++i )
        {
            ldiv_t& rDest = aPeriodsIdxs[i];
            rDest = ::ldiv( aiIdx[i], long(iNumSegs) );
            if ( rDest.rem < 0 )
            {
                rDest.rem += long(iNumSegs);
                rDest.quot--;
            }
        }

        iNumPeriodsAB = aPeriodsIdxs[ !bReverse ].quot - aPeriodsIdxs[ bReverse ].quot;
        uIdxA = OdUInt32( aPeriodsIdxs[ bReverse ].rem );
        uIdxB = OdUInt32( aPeriodsIdxs[!bReverse ].rem );
        dSegParamA = adSegParams[ bReverse ];
        dSegParamB = adSegParams[!bReverse ];
    }
    else
    {   
        // contour is not closed

        // validate bounds
        for (int i=0; i<2; ++i )
        {
            if ( aiIdx[i] < 0 )
            {
                if ( aiIdx[i] == -1 && adSegParams[i] >= (1.0-dParamTol) )
                {
                    aiIdx[i] = 0;
                    adSegParams[i] = 0.0;
                }
                else
                    return erInvalidIndex;
            }
            else if ( aiIdx[i] >= long(iNumSegs) )
            {
                if ( aiIdx[i] == long(iNumSegs) && adSegParams[i] <= dParamTol )
                {
                    aiIdx[i] = long(iNumSegs)-1;
                    adSegParams[i] = 1.0;
                }
                else
                    return erInvalidIndex;
            }
        }

        iNumPeriodsAB = 0;
        uIdxA = OdUInt32( aiIdx[ bReverse ] );
        uIdxB = OdUInt32( aiIdx[!bReverse ] );
        dSegParamA = adSegParams[ bReverse ];
        dSegParamB = adSegParams[!bReverse ];
    }

    return eRes;
}



template < class TContourData >
Result TContour2DImpl<TContourData>::_getSubSegment ( OdUInt32 iIndex, double dStartOffs, double dEndOffs, IBulgeSeg2D & rDestSeg ) const
{
    TImplSeg2D srcSeg;
    Result eRes = this->_getSegment( iIndex, srcSeg );
    if ( isError(eRes) )
        return eRes;
    
    return worstResult( eRes, srcSeg.getSubSegment( dStartOffs, dEndOffs, rDestSeg ) );
}


template < class TContourData >
Result TContour2DImpl<TContourData>::_appendSubContourTo(double dStartParam, double dEndParam, IContour2D & rSubContour, const OdGeTol & gTol ) const
{
    Result eRes = erOk;
    OdUInt32 iNumSegs = this->_numSegments();

    // TODO: skip zero-length starting/ending sub-segments

    {
        OdUInt32 iNumPeriods = 0;
        double adSegParams[2];
        OdUInt32 auSegIdxs[2];

        bool bReverse = ( dStartParam > dEndParam );

        eRes = _paramRange2IdxParams( 
            bReverse ? dEndParam   : dStartParam, 
            bReverse ? dStartParam : dEndParam, 
            auSegIdxs[0], adSegParams[0], auSegIdxs[1], adSegParams[1], iNumPeriods, 1e-14 );
        if ( isError(eRes) )
            return eRes;

#ifdef _DEBUG
        if ( iNumSegs > 0 )
        {
            double dOldLen = ( bReverse ? -1 : 1)*(dEndParam - dStartParam);
            double dNewLen = (auSegIdxs[1]+adSegParams[1]) - (auSegIdxs[0]+adSegParams[0]) + iNumPeriods*double(iNumSegs);
            FMGE_ASSERT( OdEqual( dOldLen , dNewLen, 1e-10 ) );

            FMGE_ASSERT( auSegIdxs[0] < OdUInt32(iNumSegs) && auSegIdxs[1] < OdUInt32(iNumSegs) );
            FMGE_ASSERT( this->_closed() || 0==iNumPeriods );
        }
#endif //_DEBUG

        if ( iNumPeriods < 0 || ( iNumPeriods==0 && auSegIdxs[0] > auSegIdxs[1] ) )
        {
            FMGE_FAULT("Illegal values caused by buggy code or FP-inaccuracy!");
            return erFail;
        }

        // Case 0: Append one point
        if ( erWarnEmptyContour==eRes )
        {
            TImplSeg2D srcSeg;
            Result tmpRes = this->_getSegment( auSegIdxs[bReverse], srcSeg );
            if ( isError(tmpRes) )
                return tmpRes;

            OdGePoint2d ptPoint;
            tmpRes = srcSeg.getPoint( adSegParams[bReverse], ptPoint );
            if ( isError(tmpRes) )
                return tmpRes;

            eRes = worstResult( eRes, rSubContour.appendSegment( 
                BulgeSeg2D( ptPoint, ptPoint, 0.0, srcSeg.attributes() ),
                false, gTol ) );

            return eRes;
        }

        // Case 1: Append one (sub-)segment
        if ( auSegIdxs[0]==auSegIdxs[1] && 0==iNumPeriods )
        {
            BulgeSeg2D gSingleSeg;
            eRes = worstResult( eRes, _getSubSegment( 
                auSegIdxs[0], adSegParams[ bReverse ], adSegParams[ !bReverse ], gSingleSeg ) );
            if ( isError(eRes) )
                return eRes;

            eRes = worstResult( eRes, rSubContour.appendSegment( gSingleSeg, false, gTol ) );
            return eRes;
        }

        //////////
        // default case: Append multiple segments
        if ( iNumSegs <= 0 )
        {
            FMGE_MUSTNOTEXECUTE;
            return erFail;
        }

        // get first/last (sub-)segments
        BulgeSeg2D gBoundarySegs[2];
        eRes = worstResult( eRes, _getSubSegment( auSegIdxs[0], adSegParams[0], 1.0, gBoundarySegs[0] ) );
        eRes = worstResult( eRes, _getSubSegment( auSegIdxs[1], 0.0, adSegParams[1], gBoundarySegs[1] ) );
        if ( isError(eRes) )
            return eRes;

        // first/last unbroken segments (should be copied as is)
        OdUInt32 aiUnbrokenSegs[2];
        aiUnbrokenSegs[0] = auSegIdxs[0] + 1;
        aiUnbrokenSegs[1] = auSegIdxs[1] - 1 + iNumPeriods*OdUInt32(iNumSegs);

        OdUInt32 iNumUnbroken = ( aiUnbrokenSegs[1] - aiUnbrokenSegs[0] + 1 );
        FMGE_ASSERT( iNumUnbroken >= 0 );

        OdUInt32 iDestVerts = rSubContour.numVerts();
        rSubContour.reserveVertices( iDestVerts + iNumUnbroken + 2 + 1 );

        if ( bReverse )
        {
            gBoundarySegs[0].reverse();
            gBoundarySegs[1].reverse();
        }

        // append the first segment. Fail if there is a big gap
        eRes = worstResult( eRes, rSubContour.appendSegment( gBoundarySegs[ bReverse ], false, gTol ) );
        if ( isError(eRes) )
            return eRes;


        // append middle segments
        if ( !bReverse )
        {
            // append in normal direction
            FMGE_ASSERT( aiUnbrokenSegs[0] >= 0 );
            TImplSeg2D srcSeg;
            for (OdUInt32 iSeg = aiUnbrokenSegs[0]; iSeg <= aiUnbrokenSegs[1]; ++iSeg )
            {
                OdUInt32 iIndex = OdUInt32( iSeg % OdUInt32(iNumSegs) );
                Result tmpRes = this->_getSegment( iIndex, srcSeg );

                FMGE_ASSERT( erOk==tmpRes );
                if ( isError(tmpRes) )
                    return tmpRes;

                tmpRes = rSubContour.appendSegment( srcSeg._bulge(), srcSeg._endPt(), srcSeg._attr() );
                if ( isError(tmpRes) )
                    return tmpRes;
            }
        }
        else
        {
            // append in reverse direction
            FMGE_ASSERT( aiUnbrokenSegs[0] >= 0 );
            for (OdUInt32 iSeg = aiUnbrokenSegs[1]; iSeg >= aiUnbrokenSegs[0]; --iSeg )
            {
                OdUInt32 iIndex = OdUInt32( iSeg % OdUInt32(iNumSegs) );
                const TVertexData & rVert = this->_vertex( iIndex );

                // append reversed segment
                Result tmpRes = rSubContour.appendSegment( -rVert.bulge(), rVert.point(), rVert.attributes().getReversed() );
                if ( isError(tmpRes) )
                    return tmpRes;
            }
        }

        eRes = worstResult( eRes, rSubContour.appendSegment( gBoundarySegs[ !bReverse ], false, gTol ) );
        FMGE_ASSERT( isOk(eRes) ); // fails iff this code is buggy ?
        if ( isError(eRes) )
            return eRes;
    }

    return eRes;
}



template < class TContourData >
Result TContour2DImpl<TContourData>::getSubContour(
    double dStartParam, double dEndParam, IContour2D & rSubContour, const OdGeTol & gTol ) const
{
    FMGE_ASSERT( ! _isEqualToThis(rSubContour) );
    if ( _isEqualToThis(rSubContour) )
        return erInvalidArgs;

    rSubContour.reset();
    return _appendSubContourTo( dStartParam, dEndParam, rSubContour, gTol );
}

template < class TContourData >
Result TContour2DImpl<TContourData>::appendSubContourTo(
    double dStartParam, double dEndParam, IContour2D & rSubContour, const OdGeTol & gTol ) const
{
    FMGE_ASSERT( ! _isEqualToThis(rSubContour) );
    if ( _isEqualToThis(rSubContour) )
        return erInvalidArgs;

    return _appendSubContourTo( dStartParam, dEndParam, rSubContour, gTol );
}



template < class TContourData >
Result TContour2DImpl<TContourData>::replaceSubContourTo( IContour2D& rDest, const IContour2D & rSubContour, const OdGeTol & gTol ) const
{
    FMGE_ASSERT( !_isEqualToThis(rDest) && (&rDest != &rSubContour) );
    if ( _isEqualToThis(rDest) || &rDest == &rSubContour )
        return erInvalidArgs;

    if ( rSubContour.isEmpty() )
    {
        rDest.set( *this );
        return erWarnEmptyContour;
    }

    Result eRes = erOk;

    OdGePoint2d aSubEnds[2];
    if (   erOk != ( eRes = rSubContour.getStartPoint( aSubEnds[0] ) )
        || erOk != ( eRes = rSubContour.getEndPoint( aSubEnds[1] ) ) )
        return eRes;

    double adParams[2];
    if (  !isOn( aSubEnds[0], &(adParams[0]), gTol )
        ||!isOn( aSubEnds[1], &(adParams[1]), gTol ) )
        return erPointNotOnThis;

    // now we have 2 points, 2 parameters and should replace 
    // part of this contour from adParams[0] to adParams[1] with rSubContour

    bool bEqualEnds = aSubEnds[0].isEqualTo( aSubEnds[1], gTol );
    double dParamLength = double( this->_numSegments() );

    if ( this->_closed() )
    {     
        rDest.reset();

        double dLoopStart = adParams[1];
        double dLoopEnd   = adParams[0];

        if ( bEqualEnds )
            dLoopEnd = dLoopStart + dParamLength;
        else if ( dLoopEnd < dLoopStart )
            dLoopEnd += dParamLength;

        eRes = _appendSubContourTo( dLoopStart, dLoopEnd, rDest, gTol );
        if ( isError(eRes) )
            return eRes;

        eRes = rDest.appendContour( rSubContour );
        if ( isOk(eRes) )
        {
            // remove duplicate vertex and close the contour
            rDest.setClosedIfEndsAreEqual( gTol );
            rDest.setClosed( true );
        }
    }
    else
    {
        // Open contour
        if ( bEqualEnds )
            adParams[1] = adParams[0];
        else if ( adParams[0] > adParams[1] )
            return erInvalidDirection;

        rDest.reset();

        if ( adParams[0] > 0 )
            eRes = _appendSubContourTo( 0.0, adParams[0], rDest, gTol );

        if ( isOk(eRes) )
            eRes = rDest.appendContour( rSubContour );

        if ( isOk(eRes) && ( adParams[1] < dParamLength ) )
            eRes = _appendSubContourTo( adParams[1], dParamLength, rDest, gTol );
    }

    return eRes;
}




}; // namespace FacetModeler



#endif  //__FM_T_CONTOUR2D_IMPL_H__





















