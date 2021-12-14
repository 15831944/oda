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
#ifndef __FMIMP_SIMPLE_CONTOUR2D_H__
#define __FMIMP_SIMPLE_CONTOUR2D_H__

#include "FMGeometryImpl.h"
#include "Contours/FMImpCommonSeg2D.h"
#include "Contours/FM_IContour2D.h"
#include "Contours/FMImpTContour2D.h"

namespace FacetModeler
{

class SimpleContour2DImplData
{
public:

    SimpleContour2DImplData();

    virtual ~SimpleContour2DImplData();


//////////////////////////////////////////////////////////////////////////
// Internal data corresponding to a single vertex (a point, a bulge and some extra values).

    class VertexData
    {
    private:
        // essential members
        OdGePoint2d m_ptStart; // Start point of the segment
        double      m_dBulge;  // Bulge value of the segment

        mutable Attributes2D m_Attr; // 

    public:
        VertexData() : m_dBulge(0) { }
        VertexData(const OdGePoint2d & rPt, double dBulge = 0.0)
            : m_ptStart(rPt), m_dBulge(dBulge) { }

        VertexData(const OdGePoint2d & rPt, double dBulge, const Attributes2D& rAttr )
            : m_ptStart(rPt), m_dBulge(dBulge), m_Attr(rAttr) { }

        inline const OdGePoint2d & point() const { return m_ptStart; };
        inline       OdGePoint2d & point() { return m_ptStart; };

        inline const double &      bulge() const { return m_dBulge; };
        inline       double &      bulge() { return m_dBulge; };

        inline Attributes2D&       attributes() const { return m_Attr; };
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
        // fast inline methods:

        inline const OdGePoint2d & _startPt() const { return m_pVertex->point(); };
        inline double              _bulge()   const { return m_pVertex->bulge(); };
        inline const OdGePoint2d & _endPt()   const { return *m_pEndPt; };
        inline       Attributes2D& _attr()    const { return m_pVertex->attributes(); };


        //////////////////////////////////////////////////////////////////////////
        // Implementation of methods not implemented in CommonSeg2DImpl

        virtual const OdGePoint2d & startPt() const { return _startPt(); };

        virtual const OdGePoint2d & endPt  () const { return _endPt(); };

        virtual double              bulge  () const { return _bulge(); };

        // Update ends and bulge
        virtual Result set( const OdGePoint2d & ptA, const OdGePoint2d & ptB, double dBulge, const Attributes2D& rAttr );


        virtual const Attributes2D& attributes() const { return _attr(); }
        virtual       Attributes2D& attributes4U()     { return _attr(); }

        //////////////////////////////////////////////////////////////////////////
        // CommonSeg2DImpl overrides ( a faster implementation of some methods )

        // To Do ...

    private:
        const VertexData  * m_pVertex;
        const OdGePoint2d * m_pEndPt;
    };


//////////////////////////////////////////////////////////////////////////
//
// Contour data
private:

    // Array of vertex data (points, bulges etc.)
    OdArray<VertexData> m_vecVertices;

    // If m_bClosed==true, the contour is closed and the last segment is
    //   [ m_vecVertices.back().point(), m_vecVertices.front().point(), m_vecVertices.back().bulge() ]
    // Otherwise the last bulge value is ignored and the last point
    //   of the contour is m_vecVertices.back().point()
    bool         m_bClosed;


//////////////////////////////////////////////////////////////////////////
//
// methods used by TContour2DImpl
//
public:
    inline ContourImplClass _implClass() const { return ecicSimple; };

    inline bool     _closed()      const { return m_bClosed; };

    inline bool     _empty()       const { return m_vecVertices.empty(); };

    inline OdUInt32 _numVerts()    const { return m_vecVertices.size(); };

    inline OdUInt32 _numSegments() const { return m_vecVertices.empty() ? 0 : ( m_vecVertices.size() - !m_bClosed ); };

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

    inline void _setClosed( bool bClosed ) { m_bClosed = bClosed; };

    // these methods should not be empty, if some data is cached
    inline void _setModifiedAll() { };
    inline void _setModifiedSegs ( OdUInt32 /*iFirst*/, OdUInt32 /*iCount*/ = 1 ) { };
    inline void _setModifiedVerts( OdUInt32 /*iFirst*/, OdUInt32 /*iCount*/ = 1 ) { };


    Result _insertVerticesAt( OdUInt32 iIndex, OdUInt32 iCount,
        const OdGePoint2d * paPoints, const double * paBulges = 0, const Attributes2D* pAttrs = 0);

    Result _removeVertices( OdUInt32 iIndex, OdUInt32 iCount = 1 );

};





class SimpleContour2DImpl : public TContour2DImpl<SimpleContour2DImplData>
{
public:
    SimpleContour2DImpl() { };

    // Implement some IContour2D methods, not implemented in TContour2DImpl
    //  or overload with better implementation


    // virtual void set( const IContour2D & rSrcCont );

    virtual IContour2D * clone( ) const;

    /*
    virtual Result transformBy(const OdGeMatrix2d& geMatrix );

    virtual void reverse();

    virtual void deleteCoincident( const OdGeTol & gTol = FMGeGbl::gTol );

    virtual void mergeSegments( bool bMergeArcsToo = true, const OdGeTol & gTol = FMGeGbl::gTol );

    virtual Result explodeTo( IContour2D & rDestCont, double dDeviation = 0.5, OdUInt16 iFacetMaximum = 128, OdUInt16 iMinPerCircle = 8 ) const;

    virtual OdUInt32 createVertexAt( double dParam, const OdGeTol & gTol = FMGeGbl::gTol );
    */
};


};


#endif  //__FMIMP_SIMPLE_CONTOUR2D_H__
