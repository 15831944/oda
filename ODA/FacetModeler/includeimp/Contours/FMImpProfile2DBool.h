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
#ifndef __FM_IMP_PROFILE2D_BOOL_H__
#define __FM_IMP_PROFILE2D_BOOL_H__


#include "FMProfile2DBool.h"

#include "Contours/FMContoursBaseImp.h"

#include "Contours/FM_IBulgeSeg2D.h"
#include "Contours/FM_IContour2D.h"

#include "Contours/FMContourIntersectors.h"

#include "FMContour2D.h"
#include "FMProfile2D.h"

#include "Contours/FMP2DBEdgeFlags.h"

using namespace FacetModeler;

namespace FacetModelerProfile2DBool
{
    class Intersector;
    class Node;
    class Edge;
    class EdgeGroup;

    enum OperationsExt
    {
        eoUnknown    = Profile2DBool::eoUnknown,
        eoOR         = Profile2DBool::eoOR,
        eoAND        = Profile2DBool::eoAND,
        eoSUB        = Profile2DBool::eoSUB,
        eoXOR        = Profile2DBool::eoXOR,

        eoRegularize = (Profile2DBool::eoLastOper+1), // regularization of a single argument
        //eoRegularizeTest, // regularization for the testing purposes

        eoFlagSetting, // set flags on the first argument WRT another args

        eoOpsMask    = 0xFF,
            
        // Argument types
        eoLinear   = 0x100,
        eoRegional = 0x200,
        eoLinReg   = eoLinear | eoRegional,
        eoArgMask  = 0xF00,

        // additional stuff
        eoLinRegNoBoundary = 0x1000 // region boundary is not included in cutting region
    };


    enum InfinityLocation
    {
        eilOutside = 0,
        eilInside,
        eilByBiggestArea
        //eilBySummaryArea
    };

    enum RegularityFlags
    {
        erfDefault          = 0,
        erfIntersections    = 0x01, // some bounds touch each other at least at one point
        erfSticking         = 0x02, // some bounds have coincident parts
        erfCounterSticking  = 0x04, // some bounds have coincident parts with opposite directions
        erfWrongBounds      = 0x08, // some bounds are not included in regularized profile
        erfWrongNesting     = 0x10, // some sub-regions have incorrect orientation
        erfUnknownError     = 0x100,// some error happened in regularization
        erfAllFlags         = 0xFFF
    };

#undef min
    template< class T >
    inline const T& min(const T& x, const T& y)
    {
        return (x<=y ? x : y );
    }

#undef max
    template< class T >
    inline const T& max(const T& x, const T& y)
    {
        return (x>=y ? x : y );
    }
    


    class Contour
    {
    public:
        Contour( const IContour2D & rC, OdUInt32 uCID, OdUInt32 uGID, bool bReversed = false, bool bForceCopy = false );
        void Done( );

        // reset nodes and flags. Keep source contours
        void resetNodes();
        
        inline const IContour2D & contour2D() const { return *pContour; };
        inline OdUInt32 id()      const { return uContourID; };
        inline OdUInt32 groupId() const { return uGroupID; };
        
        inline bool isReversed () const { return m_bReversed; };

        void addNode ( Node * pNode, double dParam );
        Result createEdges( Intersector & rAI, bool bStoreEdges = false );

        int getSign() const;

        Locations getLocation( const OdGePoint2d & ptTest, const OdGeTol gTol ) const;

        inline const std::vector< Edge* >& getEdges() const { return m_vecEdges; };

    private:
        OdUInt32 uContourID; // ID of the contour
        OdUInt32 uGroupID;   // Group ID
        const IContour2D * pContour; // Contour ( original or a caching copy of original )
        const IContour2D * pOrigContour; // Original contour
        bool m_bReversed;

        std::vector< std::pair< double, Node * > > m_vecNodes;

        // this member is not empty, if bStoreEdges was true in createEdges call
        std::vector< Edge* > m_vecEdges;
    };

    class Group
    {
    public:
        enum GroupTypes { 
            egtUnknown = 0,
            egtLines = 0x10,
            egtLinesWithDirection = egtLines | 0x01,
            egtRegion = 0x20,
            egtRegionNormal = egtRegion,
            egtRegionPositive = egtRegion | 0x01,
            egtRegionOrdered = egtRegion | 0x02,
            //egtNegative = 0x80,
            egtLastType
        };

        
        Group( Intersector & rAI, GroupTypes eT, OdUInt32 uPri, OdUInt32 uFirst = 0, OdUInt32 uSize = 0 );
        
        inline GroupTypes type()     const { return m_eType; };
        inline OdUInt32 priority() const { return m_uPriority; };
        
        inline OdUInt32 size()     const { return m_uSize; };
        inline OdUInt32 firstIdx() const { return m_uFirst; };
        inline OdUInt32 nextIdx()   const { return m_uFirst + m_uSize; };
        
        const IContour2D & contour( OdUInt32 uIndex ) const;
        
        inline Intersector & intersector() const { return *m_pAI; };
        
        void resize( OdUInt32 uNewSize );
        
        // validate contours and initialize some cached values
        Result validateGroup();

        Locations getLocation( const OdGePoint2d & ptTest, const OdGeTol gTol ) const;
                       
    private:           
        OdUInt32 m_uFirst;
        OdUInt32 m_uSize;
        
        OdUInt32 m_uPriority;
        GroupTypes m_eType;

        // elInside or elOutside - location of point which is not contained by the contours 
        // (i.e. "infinity" point)
        Locations m_eInfinityLocation; 

        Intersector * m_pAI;
    };
    
    
    
    class Node 
    {
    public:
        Node( ) : m_uHotID(0) { };
        Node( const OdGePoint2d & ptC )
            : m_ptCenter( ptC ), m_uHotID(0) { };
        
        enum EdgeDirection {
            eOutEdge = 0,
            eInEdge  = 1,
            eEdgeDirs 
        };

        void addEdge( int eDir, Edge * pEdge );

        void updateEdgeIndices( );
        void createEdgeGroups( Intersector & rAI );

        void updateFinalEdges( bool bReorderByAngle, double dTgStep );

        typedef std::pair< EdgeDirection, Edge * > EdgeLink;

        inline OdUInt32 edgeCount( ) const { return (OdUInt32)m_vecEdges.size(); };

        inline const EdgeLink & edgeLink ( OdUInt32 iIndex ) const { return m_vecEdges[iIndex]; };

        // HotIDs are used during export to simplify check, whether we`ve just passed the node
        inline OdUInt32 getHotID() const { return m_uHotID; };
        inline OdUInt32 setHotID(OdUInt32 uID) { return m_uHotID = uID; };

        //Edge * findNeighbour( const Edge * pEdge, EdgeDirection eEdgeDir, EdgeDirection eNeighbourDir, bool bCCW = false ) const;

        Edge * findFinalRegionalNeighbour( const Edge * pEdge, EdgeDirection eEdgeDir, EdgeDirection eNeighbourDir, bool bCCW = false ) const;
        Edge * findFinalEdge( EdgeDirection eEdgeDir ) const;

        void reverseFinalEdge( OdUInt32 iIndex );

    private:
        OdGePoint2d m_ptCenter;

        OdUInt32 m_uHotID;

        std::vector< EdgeLink > m_vecEdges;
    };
    


    class Edge
    {
    public:
        Edge( const Contour & rC, double dStartP, double dEndP ); 
        
        enum NodePosition {
            eStartNode = 0,
            eEndNode   = 1,
            eNodePositions
        };

        inline const Contour &  contour( ) const { return *m_pContour; };
        inline OdUInt32 regionId() const { return m_pContour->groupId(); };

        Edge & setNode( OdUInt32 ePos, Node * pNode, OdUInt32 iIndex );

        inline double param( int ePos ) const { return m_aParams[ePos]; };
        inline Node * node ( int ePos ) const { return m_aNodes[ePos]; };
        inline Node * anotherNode( int ePos ) const { return m_aNodes[ ePos^1 ]; };

        double tangentAt( int ePos, double dStep ) const;

        void reverseFinalEdge();

        inline OdUInt32 indexInNode( int ePos ) const { return m_aIndices[ePos]; };

        const OdGePoint2d& middlePoint() const;

        inline EdgeGroup * edgeGroup() const { return m_pEdgeGroup; };
        Edge & setEdgeGroup( EdgeGroup * pGroup, OdUInt32 iIndex );

        static bool areCoincident( const Edge & rEdgeA, const Edge & rEdgeB, bool * pbCoDirectional = 0, const OdGeTol & gTol = FMGeGbl::gTol );
        static EdgeGroup * GroupEdges( Intersector & rAI, Edge & rEdgeA, Edge & rEdgeB, bool bCoDirectional );


        enum Flags {
            efSourceReversed = 0x01,
            efChecked = 0x02, // finality was checked
            efHot = 0x04, // the edge is a candidate for export
            //efDone = 0x04,
            efFinal = 0x10,
            efFinalReversed = 0x20,
            efFinalDone = 0x40
        };

        inline bool isFinal() const { return 0 != (m_uFlags & efFinal); };
        
        inline OdUInt32 setFlag( OdUInt32 uFlag ) { return m_uFlags |= uFlag; };
        inline OdUInt32 unsetFlag( OdUInt32 uFlag ) { return m_uFlags &= ~uFlag; };
        inline OdUInt32 getFlag( OdUInt32 uFlag ) const { return m_uFlags & uFlag; };

        Edge * getNextFinalRegional( bool bContoursCCW = true ) const;
        Edge * getPrevFinalRegional( bool bContoursCCW = true ) const;

        Edge * getNextFinalLinear( bool bStopAtJunctions = false ) const;

    private:
        Node    * m_aNodes[ eNodePositions ];
        OdUInt32  m_aIndices[ eNodePositions ];

        double    m_aParams[ eNodePositions ];
        const Contour * m_pContour;

        mutable OdGePoint2d m_ptMiddle;

        EdgeGroup * m_pEdgeGroup;

        OdUInt32 m_uFlags;
    };


    class EdgeGroup
    {
    public:
        EdgeGroup( Edge * pFirstEdge, Edge * pSecondEdge = 0 );

        EdgeGroup & addEdge( Edge * pEdge );
        EdgeGroup & merge( EdgeGroup * pGroup2 );

        inline OdUInt32 size() const { return (OdUInt32)m_vecEdges.size(); };
        inline Edge * edge( OdUInt32 iIndex ) { return m_vecEdges[iIndex]; };

        EdgeGroup & setFlags( OdUInt32 uFlags, OdUInt32 uUnsetFirst = 0 );

        inline bool isHandled() const { return m_bHandled; };

        const OdGePoint2d& middlePoint() const;

        //EdgeGroup & sortByRegionNode();

    private:
        std::vector< Edge * > m_vecEdges;
        bool m_bHandled;
    };



    // This interface updates final/checked flags in edges and edge groups.
    class IEdgeChecker 
    {
    public:
        virtual ~IEdgeChecker() { };

        // this static method creates a new EdgeChecker, corresponding to eOp
        // or returns 0, if the operation is not valid
        static IEdgeChecker * Create( OperationsExt eOp, Intersector & rAI );

        virtual Result CheckFinality( Edge & ) const = 0;

        virtual Result CheckFinality( EdgeGroup & ) const = 0;
    };


    class EdgeInfoImpl;
    

    class Intersector
    {
    public:
        Intersector();
        ~Intersector();
        
        //////////////////////////////////////////////////////////////////////////
        // 
        // Some simple user-friendly methods:
        //

        Result Regularize( const Profile2D& rSource, Profile2D& rDest, 
            bool bValidateNesting = false,
            InfinityLocation eIL = eilOutside,
            const OdGeTol & gTol = FMGeGbl::gTol );

        // fills in riRegularityFlags
        Result TestRegularity( const Profile2D& rSource,
            int& riRegularityFlags,
            int  iInterestingFlags = erfAllFlags,
            InfinityLocation eIL = eilByBiggestArea,
            const OdGeTol & gTol = FMGeGbl::gTol );

        Result PerformOperation( BooleanOperation eType, 
            const Profile2D &rArg1,
            const Profile2D &rArg2,
            Profile2D &rResult,
            const OdGeTol & gTol = FMGeGbl::gTol );
        
        static bool TryFastPerformOperation( BooleanOperation eType, 
            const Profile2D& rArg1,
            const Profile2D& rArg2,
            Profile2D& rResult );

        //////////////////////////////////////////////////////////////////////////
        
        //Result InitOperation( OperationsExt eOper, const OdGeTol & gTol = FMGeGbl::gTol );



        //////////////////////////////////////////////////////////////////////////
        // 
        // Low-level methods. Do not use if not sure WYAD.


        //////////////////////////////////////////////////////////////////////////
        // initialization

        void reset( const OdGeTol & gTol = FMGeGbl::gTol );

        void reserve( OdUInt32 uGroups, OdUInt32 uContours );

        // returns group index or OdUInt(-1)
        OdUInt32 addGroup( Group::GroupTypes eT, OdUInt32 uPriority = 0, OdUInt32 uReservedSize = 0 );

        // add contours to some group ( Fails, if uGroup != last addGroup result !!! )
        Result addContour( OdUInt32 uGroup, const IContour2D & rContour, bool bReversed = false, bool bForceCopy = false );

        // add profile to some group ( Fails, if uGroup != last addGroup result !!! )
        Result addProfile( OdUInt32 uGroup, const Profile2D  & rProfile, bool bReversed = false, bool bForceCopy = false );

        // tries to perform operation on existing groups/contours
        // result is stored in member variable and can be obtained via getResult();
        Result evaluate( OperationsExt eOp );


        // Flag-setting operations.
        // The result of this operation contains contours of the first group with
        // some additional vertices and updated metadata.
        // ToDo: more comments
        Result evaluateFlags( const IFlagSetter& rFlagSetter );


        // returns resulting profile from the last "evaluate"
        // use moveResultTo instead to avoid contour copying
        inline const Profile2D& getResult( ) const { return m_cDestProfile; };
        inline void moveResultTo( Profile2D & rDest ) { rDest.clear(); m_cDestProfile.swap( rDest ); };

        //////////////////////////////////////////////////////////////////////////

        inline OdUInt32 numContours() const { return (OdUInt32)m_vecContours.size(); };
        const Contour & getContour( OdUInt32 uContourID ) const;
        
        inline OdUInt32 numGroups() const { return (OdUInt32)m_vecGroups.size(); };
        const Group &   getGroup( OdUInt32 uGroupIdx ) const;

        //  returns group indexes ordered by priority
        inline OdUInt32 getGroupIdxByPriority( OdUInt32 iIdx ) const
        { return iIdx; };

        inline const OdGeTol & getTol() const { return m_gTol; };
        inline const OdGeTol & getPairTol() const { return m_gPairTol; };
        inline const OdGeTol & getMergeTol() const { return m_gMergeTol; };
        inline const OdGeTol & getExportTol() const { return m_gEdgeExportTol; };

        inline const IEdgeChecker  & getChecker() const { return *m_pEdgeChecker; };

        //////////////////////////////////////////////////////////////////////////

        // adds node to Intersector`s node list 
        Node * addNode( Node * pNode );

        // adds edge to Intersector`s edge list 
        Edge * addEdge( Edge * pEdge );

        // adds edgegroup to Intersector`s edgegroup list 
        EdgeGroup * addEdgeGroup( EdgeGroup * pEdgeGroup );

        inline void setRegularityFlags( int iFlags ) { m_iRegularityFlags|=iFlags; };

    private:

        std::vector< Contour > m_vecContours;
        std::vector< Group   > m_vecGroups;

        std::vector< Node * > m_vecNodes;
        std::vector< Edge * > m_vecEdges;
        std::vector< EdgeGroup * > m_vecEdgeGroups;

        int m_iRegularityFlags;

        //////////////////////////////////////////////////////////////////////////
        //
        OperationsExt m_eCurrOper;

        //
        Profile2D  m_cDestProfile;

        IEdgeChecker * m_pEdgeChecker;


        // tolerances
        OdGeTol m_gTol;
        OdGeTol m_gPairTol;
        OdGeTol m_gMergeTol;
        double  m_dTgStep;
        OdGeTol m_gEdgeExportTol;
        void initTolerance ( const OdGeTol & gTol );

        // Reset all intermediate data. Keep source groups and contours
        void resetIntermediate();

        // Reset resulting profile and operation code
        void resetResult();

        // validates operation and groups/contours. 
        // Returns error, if the operation is not valid
        Result validateArgs( OperationsExt eOper );

        //////////////////////////////////////////////////////////////////////////
    private:

        // Stage 1: find all crossings using ContourIntersector
        Result FindCrossings( std::vector< Crossing > & vecCrossings );

        // Stage 2: convert crossings to nodes and arrange the nodes on contours
        Result CreateNodes( const std::vector< Crossing > & vecCrossings );
        
        // Stage 3: split contours into edges, connecting the nodes
        Result CreateEdges( bool bStoreEdges = false, bool bStoreFirstGroupOnly = false );

        // Stage 4: find coincident edges and create groups
        Result CreateEdgeGroups( );

        // Stage 5: find and verify final edges, depending on operation type
        Result FindFinalEdges( );

        // Stage 6: create final contours from final edges
        Result CreateFinalContours();

        Result CreateFinalContoursClosed( Profile2D & rDest, OdUInt32 & uHotID );
        Result CreateFinalContoursOpen  ( Profile2D & rDest, OdUInt32 & uHotID );

        Result GenerateFinalContour( const std::vector<Edge*> & vecEdges, IContour2D & rDestC2D, bool bClosed );

        // Stage 5F: create final contours with modified metadata
        Result CreateFlaggedResult( const IFlagSetter& rFlagSetter );

        // remove empty/collapsed contours and improperly nested ones
        // returns erOk or erWarn*, if some contours in the source profile should be removed
        // pvecValid contains indexes of valid contours in the source profile
        //      the indexes are ordered by contour area (the first is the biggest)
        // pvecResults contains status for each contour ( erOk for valid ones )
        // Note: contours MUST NOT intersect. However, the contours can have common bounds
        Result ValidateNesting( const Profile2D & rSource, 
            std::vector<OdUInt32> * pvecValid   = 0,
            std::vector<Result> * pvecResults = 0,
            InfinityLocation eIL = eilOutside
            ) const;

        // remove empty/collapsed contours and improperly nested ones
        // Note: contours MUST NOT intersect. However, the contours can have common bounds
        // Note: rSource can be equal to *pDest
        // Returns erOk, erWarnImproperAreas, erWarnSmallContour, erWarnEmptyContour or some 
        //   error code.
        Result RegularizeNesting( const Profile2D & rSource, Profile2D * pDest = 0, Profile2D * pRemoved = 0, InfinityLocation eIL = eilOutside );

    }; // class Intersector
        

  
  
}; // namespace FacetModelerProfile2DBool

#endif  //__FM_IMP_PROFILE2D_BOOL_H__
