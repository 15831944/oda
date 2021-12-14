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
#ifndef __FMIMP_CACHED_CONTOUR2D_H__
#define __FMIMP_CACHED_CONTOUR2D_H__

#include "FMGeometry.h"
#include "Contours/FMImpCommonSeg2D.h"
#include "Contours/FM_IContour2D.h"
#include "Contours/FMImpTContour2D.h"

#include "Ge/GeExtents2d.h"

namespace FacetModeler
{

class CachedContour2DImplData
{
public:

    CachedContour2DImplData();

    virtual ~CachedContour2DImplData();

//////////////////////////////////////////////////////////////////////////
// Internal segment cache data

    struct SegmentCacheData
    {
        SegmentCacheData() : m_bValid(false), 
            m_eType(estUnknown), m_dLength(0), m_dArcInclAngle(0) { };
        
        // data members
        bool m_bValid; // Is the cache valid ?
        
        SegmentType m_eType; // type of the segment
        double      m_dLength; // Length of the segment
        
        // Arc/Circle data:
        double      m_dArcInclAngle; // signed included angle of the arc
        OdGePoint2d m_ptArcCenter;   // center of the arc
    };


//////////////////////////////////////////////////////////////////////////
// Internal data corresponding to a single vertex (a point, a bulge and some extra values).



    class VertexData
    {
    private:
        // essential members
        OdGePoint2d m_ptStart; // Start point of the segment
        double      m_dBulge;  // Bulge value of the segment

        mutable SegmentCacheData m_Cache; // cached data

        // Start length of this segment in the contour
        // This value is not included in m_Cache, because it depends on another segments
        mutable double m_dStartLength;

        mutable Attributes2D m_Attr;

    public:
        VertexData() 
            : m_dBulge(0), m_dStartLength(0) { }

        VertexData(const OdGePoint2d & rPt, double dBulge = 0.0 )
            : m_ptStart(rPt), m_dBulge(dBulge), m_dStartLength(0) { }

        VertexData(const OdGePoint2d & rPt, double dBulge, const Attributes2D& rAttr )
            : m_ptStart(rPt), m_dBulge(dBulge), m_dStartLength(0), m_Attr(rAttr) { }

        inline const OdGePoint2d & point() const { return m_ptStart; };
        inline       OdGePoint2d & point() { return m_ptStart; };

        inline const double &      bulge() const { return m_dBulge; };
        inline       double &      bulge() { return m_dBulge; };

        inline Attributes2D&       attributes() const { return m_Attr; };

        inline SegmentCacheData &  cache() const { return m_Cache; };
        inline       double &      startLength() const { return m_dStartLength; };

        inline void  resetCache() const { m_Cache.m_bValid = false; /* m_dStartLength = 0; */ };
    };


//////////////////////////////////////////////////////////////////////////
// ImplSeg2D class is used internally in implementation of Contour methods

    class ImplSeg2D: public CommonSeg2DImpl
    {
    public:
        ImplSeg2D( ) : m_pVertex(0), m_pEndPt(0) { };

        ImplSeg2D( const VertexData & rVData, const OdGePoint2d & rPt )
            : m_pVertex(&rVData), m_pEndPt( &rPt ) { };

        void set( const VertexData & rVData, const OdGePoint2d & rPt )
            { m_pVertex = &rVData; m_pEndPt = &rPt; };

        // ToDo: another constructors...

        //////////////////////////////////////////////////////////////////////////

        inline SegmentCacheData & _cache() const { return m_pVertex->cache(); };
        inline double             _startLength() const { return m_pVertex->startLength(); };
        
        // Updates cached values in VertexData::m_pCache
        void _UpdateCache() const;


        //////////////////////////////////////////////////////////////////////////
        // fast inline methods:

        inline const OdGePoint2d & _startPt() const { return m_pVertex->point(); }
        inline double              _bulge()   const { return m_pVertex->bulge(); }
        inline const OdGePoint2d & _endPt()   const { return *m_pEndPt; }
        //inline OdUInt32            _metadata()const { return m_pVertex->metadata(); }
        inline       Attributes2D& _attr()    const { return m_pVertex->attributes(); }

        // These methods work fine only after _UpdateCache is called 
        inline SegmentType   _type()   const { return _cache().m_eType; };
        inline double        _length() const { return _cache().m_dLength; };
        inline double        _arcAngle() const { return _cache().m_dArcInclAngle; };
        inline double        _arcRadius() const { return _cache().m_dLength / ::fabs(_cache().m_dArcInclAngle); };
        inline const OdGePoint2d & _arcCenter() const { return _cache().m_ptArcCenter; };
               double        _arcStartAngle() const;


        //////////////////////////////////////////////////////////////////////////
        // Implementation of methods not implemented in CommonSeg2DImpl

        // Update ends and bulge ( always fails )
        virtual Result set( const OdGePoint2d & ptA, const OdGePoint2d & ptB, double dBulge, const Attributes2D& rAttr );


        //////////////////////////////////////////////////////////////////////////

        virtual SegmentImplClass  implClass() const;

        virtual const OdGePoint2d & startPt() const;

        virtual const OdGePoint2d & endPt  () const;

        virtual double              bulge  () const;

        virtual const Attributes2D& attributes() const  { return _attr(); }
        virtual       Attributes2D& attributes4U()      { return _attr(); }


        //////////////////////////////////////////////////////////////////////////

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

    private:
        const VertexData  * m_pVertex;
        const OdGePoint2d * m_pEndPt;
    };


//////////////////////////////////////////////////////////////////////////
//
// Contour data
private:

    // Array of vertex data (points, bulges etc.)
    //std::vector<VertexData> m_vecVertices;
    OdArray<VertexData> m_vecVertices;

    // If m_bClosed==true, the contour is closed and the last segment is
    //   [ m_vecVertices.back().point(), m_vecVertices.front().point(), m_vecVertices.back().bulge() ]
    // Otherwise the last bulge value is ignored and the last point
    //   of the contour is m_vecVertices.back().point()
    bool         m_bClosed;


    //////////////////////////////////////////////////////////////////////////
    // Cached contour data:

    OdUInt32 m_iNumSegments; // should be valid always
    OdUInt32 _updateNumSegments();

public:

    enum CacheStates
    {
        ecsNotValid = 0, // All cached info is not valid
        ecsLengthsValid = 0x01, // m_dLength and lengths of all segments are valid
        ecsAreaValid = 0x02, // m_dSignedArea is valid
        ecsExtentsValid = 0x04, // m_Extents is valid
        ecsConvexValid = 0x08 // m_bConvex is Valid

    };

    mutable OdUInt32 m_eCacheState; 

    mutable double m_dLength;
    mutable double m_dSignedArea;
    mutable OdGeExtents2d m_Extents;
    mutable bool m_bConvex;

    // These methods update (if not valid) and return cached values
    double _updateCacheLengths() const;
    //double _updateCacheArea() const;
    //const OdGeExtents2d& _updateCacheExtents() const;

    // reset contour`s and segments` cache.
    // use iNumVerts=0 to keep segments` cache intact.
    // if bStepBack is true, then iFirstVertex is the first modified vertex,
    //   so the segment (iFirstVertex-1) is updated too!
    void _resetCache(OdUInt32 iFirstVertex=0, OdUInt32 iNumVerts = OdUInt32(-1), bool bStepBack = true );

//////////////////////////////////////////////////////////////////////////
//
// methods used by TContour2DImpl
//
public:
    inline ContourImplClass _implClass() const { return ecicCached; };

    inline bool     _closed()      const { return m_bClosed; };

    inline bool     _empty()       const { return m_vecVertices.empty(); };

    inline OdUInt32 _numVerts()    const { return m_vecVertices.size(); };

    inline OdUInt32 _numSegments() const { return m_iNumSegments; };

    Result _getSegment( OdUInt32 iIndex, ImplSeg2D & rSeg ) const;

    Result _normalizeIndex( OdUInt32 & iVertIndex ) const;


    inline const VertexData& _vertex( OdUInt32 iIndex ) const
    {
        FMGE_ASSERT( iIndex < _numVerts() );
        return m_vecVertices[iIndex];
    };

    //////
    // Non-const methods

    inline VertexData& _vertex( OdUInt32 iIndex )
    {
        FMGE_ASSERT( iIndex < _numVerts() );
        return m_vecVertices[iIndex];
    };


    void _reset( OdUInt32 iVertexCount = 0, bool bClosed = false );

    void _resize( OdUInt32 iNewVertNum );

    void _reserveVertices( OdUInt32 iReservedSize );

    void _setClosed( bool bClosed );

    // These methods update cached values or reset cache flags
    void _setModifiedAll();
    void _setModifiedSegs ( OdUInt32 iFirst, OdUInt32 iCount = 1 );
    void _setModifiedVerts( OdUInt32 iFirst, OdUInt32 iCount = 1 );


    Result _insertVerticesAt( OdUInt32 iIndex, OdUInt32 iCount,
        const OdGePoint2d * paPoints, const double * paBulges = 0, const Attributes2D* pAttrs = 0);

    Result _removeVertices( OdUInt32 iIndex, OdUInt32 iCount = 1 );

};





class CachedContour2DImpl : public TContour2DImpl<CachedContour2DImplData>
{
public:
    CachedContour2DImpl() { };

    // Implement some IContour2D methods, not implemented in TContour2DImpl
    //  or overload with better implementation

    virtual IContour2D * clone( ) const;


    virtual double length() const;

    virtual Result getDistAtParam (double dParam, double& dDist ) const;
    virtual Result getParamAtDist (double dDist,  double& dParam ) const;

    virtual Result addExtents(OdGeExtents2d& geExtents ) const;

    virtual double signedArea() const;

    virtual bool isConvex() const;

    // ToDo: override intersect,isOn etc. to check extents first, if available

    virtual bool contains( const OdGePoint2d &rPoint, bool * pbOnBorder = 0,
        const OdGeTol & gTol = FMGeGbl::gTol) const;


private:

    // These methods update (if not valid) and return cached values
    double _updateCacheArea() const;
    const OdGeExtents2d& _updateCacheExtents() const;
    bool _updateCacheConvex() const;
};


};


#endif  //__FMIMP_CACHED_CONTOUR2D_H__
