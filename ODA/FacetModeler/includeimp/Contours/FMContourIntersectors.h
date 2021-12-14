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
#ifndef __FMGE_CONTOUR_INTERSECTORS_H__
#define __FMGE_CONTOUR_INTERSECTORS_H__

#include "Contours/FMContoursBaseImp.h"
#include "Contours/FM_IContour2D.h"
#include "FMProfile2D.h"

namespace FacetModeler
{

    // Intersection of contours with user-defined IDs of the contours
    class FMGEOMETRY_API IntersectionWithIDs : public Intersection
    {
    public:
        IntersectionWithIDs() : uContourA(0), uContourB(0) { };

        IntersectionWithIDs( const Intersection & rI, OdUInt32 uIDA, OdUInt32 uIDB ) 
            : Intersection(rI), uContourA(uIDA), uContourB(uIDB) { };
        
        IntersectionWithIDs( const OdGePoint2d & rPt, OdUInt32 uIDA, 
            double dPA, OdUInt32 uIDB, double dPB, Types eIT=eitNone )
            : Intersection(rPt, dPA, dPB, eIT), uContourA( uIDA ), uContourB( uIDB ) { };
        
        IntersectionWithIDs & swapParams();

        static bool lessIDParamA( const IntersectionWithIDs& rI1, const IntersectionWithIDs& rI2 );
        static bool lessIDParamB( const IntersectionWithIDs& rI1, const IntersectionWithIDs& rI2 );

    public:
        OdUInt32 uContourA; // ID of the first contour
        OdUInt32 uContourB; // ID of the second contour
    };




    class Crossing
    {
    public:

        class ContourLink
        {
        public:
            ContourLink() { };
            ContourLink( OdUInt32 uID, double dP, double dD )
                : uContourID(uID), dParam( dP ), dDistance( dD ) { };
            
            inline bool operator < ( const ContourLink & rB ) const
            {
                return (uContourID < rB.uContourID)
                     ||( (uContourID == rB.uContourID) && ( dParam < rB.dParam ) );
            };

        public:
            OdUInt32 uContourID;
            double   dParam;
            double   dDistance;
        };

        OdGePoint2d ptPoint; // center of the crossing
        
        std::vector< ContourLink > vecLinks; // contour links
    };



    // This helper class is used for fast contour/profile intersections

    class FMGEOMETRY_API ContourIntersector
    {
    public:

        // some definitions

        struct SegmentUID
        {
            SegmentUID() { };
            SegmentUID( OdUInt32 uG, OdUInt32 uC, OdUInt32 uS )
                : uGroupIdx( uG ), uContourIdx( uC ), uSegmentIdx( uS ) { };

            bool operator < ( const SegmentUID & rB ) const;
            
            OdUInt32 uGroupIdx;
            OdUInt32 uContourIdx;
            OdUInt32 uSegmentIdx;
        };
        
        
        
        
        // SegmentBound1D class
        // The class represents 1-D bound of a segment. It is used in contour intersection code.
        class SegmentBound1D {
        public:
            SegmentBound1D() 
                :m_eType(eNone)
            { };
            
            enum eBoundType {
                eNone = 0, eStart, eEnd, eLastBound
            };
            
            SegmentBound1D( eBoundType eType, double dValue, OdUInt32 uG, OdUInt32 uC, OdUInt32 uS )
                :m_dValue(dValue)
                ,m_eType(eType)
                ,m_UID( uG, uC, uS )
            { };
            
            
            // comparison of the bounds. Used in bound sorting
            static bool less( const SegmentBound1D& A, const SegmentBound1D& B)
            {
                return A.value() < B.value();
            }
            
            inline double      value()   const { return m_dValue; };
            inline eBoundType  type()    const { return m_eType; };
            
            inline const SegmentUID & segmentUID() const { return m_UID; };
            
        private:
            double       m_dValue; // position
            eBoundType   m_eType;  // start/end
            
            SegmentUID   m_UID; // Group, Contour, Segment
        };

 
        // type of intersections to look for
        enum IntersectionTypes
        {
            eIntNone = 0,
            eIntMiddle2Middle = 0x01, // interior parts of some segments intersect
            eIntNode2Node     = 0x02, // boundary points of some segments intersect
            eIntNode2Middle   = 0x04, // boundary points of some segment(s) touch interior of some other

            // ToDo...
            //eIntSkipTouches   = 0x10, // reject touches in node2node and node2middle
            //eIntNode2NodeST   = eIntNode2Node|eIntSkipTouches,
            //eIntNode2MiddleST = eIntNode2Middle|eIntSkipTouches,
            
            eIntSkipZeroLength= 0x20,// reject self-intersections with zero (<tol) distance  
            eIntAny           = 0x0F // look for all crossings
        };


    public:

        ContourIntersector( const OdGeTol & gTol = FMGeGbl::gTol );

        ~ContourIntersector();

        // Clear all groups and set tolerance
        void reset( const OdGeTol & gTol = FMGeGbl::gTol );

        void reserveGroups( OdUInt32 iNumGroups );

        // Add a new group of contours and reserve memory, if uReservedContours>0
        // bSelfIntersecting defines whether self-intersections of the group are searched for or not
        // returns group index
        OdUInt32 addGroup( bool bSelfIntersecting = false, OdUInt32 uReservedContours = 0 );

        inline OdUInt32 numGroups() const { return (OdUInt32)m_vecGroups.size(); };

        // Add contour/profile to an intersection group.
        // Intersection points on the contour will have uContourA or uContourB = uID
        //   Contours from rProfile are added with auto-incremented IDs, starting from uFirstID
        // Contours from 0 group are intersected with all contours.
        // Contours from other groups are intersected with different groups only
        Result addContour( const IContour2D & rContour, OdUInt32 uID,      OdUInt32 uGroup = 0 );
        Result addProfile( const Profile2D  & rProfile, OdUInt32 uFirstID, OdUInt32 uGroup = 0 );

        // Appends intersection points to vecPoints.
        // Returns number of intersections. 
        OdUInt32 getIntersections( std::vector< IntersectionWithIDs > & vecPoints, bool bOrderByGroups = true );

        // Looks for intersections and returns true, iff the specified type of intersection
        // was found. 
        // Default: find any intersection
        bool hasIntersections( int eLookFor = eIntAny );
        
        // Appends Crossings to the vecCrossings
        // Crossings are unique and contain links to all possible segments.
        OdUInt32 getAllCrossings( std::vector< Crossing > & vecCrossings, double dMergeTol = 0 );

    private:

        struct ContourRec 
        {
            ContourRec( const IContour2D & rContour, OdUInt32 uContourID, OdUInt32 uGroupID );
            // This method should be called manually to release pContour, if necessary.
            // It is not included in destructor to fasten std::vector operations
            void   Done( );

            inline const IContour2D & contour() const { return *pContour; };

            OdUInt32 uContourID; // ID of the contour
            OdUInt32 uGroupID;   // Group ID
            const IContour2D * pContour; // Contour ( original or a caching copy of original )
            const IContour2D * pOrigContour; // Original contour
        };

        struct GroupRec 
        {
            GroupRec() : bSelfIntersecting(false) { };

            void Done( );

            bool bSelfIntersecting;
            std::vector< ContourRec > vecContours;
        };


        OdGeTol m_gTol; // tolerance of intersection

        // Vectors of contours
        std::vector< GroupRec > m_vecGroups;

        // helper class for getAllCrossings( )
        class SegmentCrossing;

        bool m_bOrderByGroup;

    private:

        void resetGroup( OdUInt32 uGroup );

        //////////////////////////////////////////////////////////////////////////
        // Stages of getIntersections:
        
        // Stage 1: find good direction for projection of segments
        OdGeVector2d findMainDirection( ) const;
        
        // Stage 2: get and sort segment bounds using selected direction
        void fillSegmentBounds( const OdGeVector2d& vMainDir, std::vector< SegmentBound1D > & vecBounds, double dAddTol = 0 ) const;
        
        // returns total number of segments
        OdUInt32     getSegmentCount( ) const;
        
        
        // Stage 2b: try to remove some useless segments from the bounds list.
        //   the bounds aren't actually removed, but its type is set to eNone.
        // returns number of useful bounds
        OdUInt32 removeUselessBounds( std::vector< SegmentBound1D > & vecBounds ) const;

        bool createCrossing( SegmentCrossing & rCross, Crossing & rDest, const double dTol ) const;


        // skips segment joints. returns true, if there are valid intersections
        // if dZeroTol >= 0 it also skips self-intersections with zero distance along contour
        bool skipJoints( const SegmentUID & rUID1, const SegmentUID & rUID2,
            IntersectionWithIDs * aIPs, OdUInt32 iXNum, const double dZeroTol = -1.0 );

        // updates contour parameters and IDs.
        // returns number of valid intersections
        OdUInt32 postprocessIntersections( const SegmentUID & rUID1, const SegmentUID & rUID2,
            IntersectionWithIDs * aIPs, OdUInt32 iXNum );
    };


};

#endif  //__FMGE_CONTOUR_INTERSECTORS_H__
