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

#ifndef __FMPROFILE2DBOOL_H__
#define __FMPROFILE2DBOOL_H__

#include "FMProfile2D.h"

namespace FacetModelerProfile2DBool
{
    class Intersector;
};

namespace FacetModeler
{

/** \details
   This class contains methods performing Boolean operations
   on 2-D regions and lines
  
   \remarks
   Regions are given by bounding contours. Contours must be closed and should not 
   be self-intersecting.
   CCW contour represents region inside it, and CW contour - outside.
   Lines are given by contours too, but the contours can be closed or not and can 
   intersect each other.
*/
class FMGEOMETRY_API Profile2DBool
{
    public:
        /** \details
           Operation type.
        */
        enum Operations
        {
            eoUnknown = 0,
            eoOR, // union
            eoAND, // intersection
            eoSUB, // difference
            eoXOR, // exclusive or = union minus intersection
            eoLastOper
        };

        /** \details
           Argument stub.
          
           \remarks
           Used because we do not want duplicate
           similar methods for Contour2D and Profile2D objects.
        */
        class ArgStub
        {
            public:
                ArgStub( const Contour2D  & rC2D, OdUInt32 uPriority = 0 )
                    : m_pProfile(0), m_pContour(&rC2D.impl()), m_uPriority(uPriority) { }
                ArgStub( const IContour2D & rIC2D, OdUInt32 uPriority = 0 ) 
                    : m_pProfile(0), m_pContour(&rIC2D), m_uPriority(uPriority) { }
                ArgStub( const Profile2D  & rP2D, OdUInt32 uPriority = 0  ) 
                    : m_pProfile(&rP2D), m_pContour(0), m_uPriority(uPriority) { }
        
                inline const IContour2D * contour() const { return m_pContour; }
                inline const Profile2D  * profile() const { return m_pProfile; }
                inline OdUInt32          priority() const { return m_uPriority; }

                ArgStub& setPriority(OdUInt32 uNewP) { m_uPriority = uNewP; return *this; }

                bool isEmpty() const;

                bool isSame (const ArgStub& rOther) const;

                void copyTo (Profile2D& rResult) const;

                OdUInt32 numContours() const;

            private:
                const IContour2D * m_pContour;
                const Profile2D  * m_pProfile;
                OdUInt32           m_uPriority;
        };

        public:
            /** \details
               Constructor.
            */
            Profile2DBool( const OdGeTol & gTol = FMGeGbl::gTol );

            /** \details
               Destructor.
            */
            ~Profile2DBool();

            /** \details
               Resets the intersector to default values and updates tolerance
            */
            Profile2DBool& reset( const OdGeTol & gTol = FMGeGbl::gTol );


            /** \details
               Performs Boolean operation on 2 regions
              
               \param eOper [in]  operation code. One of: eoOR, eoAND, eoSUB, eoXOR
               \param rArg1 [in]  contours of the first region
               \param rArg2 [in]  contours of the second region
               \param rResult [out]  resulting region (modified only if operation succeeds)
              
               \returns
               returns result of operation.
              
               \remarks
               rResult can coincide/overlap rArg1 or rArg2
            */
            Result evalRegions(
                Operations eOper, 
                ArgStub rArg1, 
                ArgStub rArg2, 
                Profile2D& rResult );

            /** \details
               Cuts lines (rLinesArg) by a region (rArg2) and returns
              
               \param eOper [in]  operation code. One of: eoAND, eoSUB
               \param rLinesArg [in]  contours, which should be cut
               \param rRegionArg [in]  contours of the cutting region
               \param rResult [out]  resulting contours (modified only if operation succeeds)
               \param bRegionIsOpen [in]  if true, region boundary points are not included in the cutting region
              
               \returns
               returns result of operation.
              
               \remarks
               rResult can coincide/overlap rLinesArg or rRegionArg
               rResult can contain closed or open contours
            */
            Result evalLinesRegion( 
                Operations eOper,
                ArgStub rLinesArg,
                ArgStub rRegionArg,
                Profile2D& rResult,
                bool bRegionIsOpen = false );

            /** \details
               Perform Boolean operation on lines
              
               \param eOper [in]  operation code. One of: eoOR, eoAND, eoSUB, eoXOR
               \param rArg1 [in]  first group of lines
               \param rArg1 [in]  second group of lines
               \param rResult [out]  resulting lines (modified only if operation succeeds)
              
               \returns
               returns result of operation.
              
               \remarks
               rResult can coincide/overlap rArg1 or rArg2
               rResult can contain closed or open contours
            */
            Result evalLines( 
                Operations eOper,
                ArgStub rArg1,
                ArgStub rArg2,
                Profile2D& rResult );

            /** \details
               Store a reference to perform some operation(s) later
              
               \param rArg [in]  region (closed contours) to store
               
               \remarks
               If error is returned, the operation should not be continued
               and this->reset should be called.
               reset() should be called to start new sequence of arguments
            */
            Result pushRegion( ArgStub rArg, bool bSaveCopy = true );

            /** \details
               Performs Boolean operation on multiple regions
              
               \param eOper [in]  operation code. One of: eoOR, eoAND, eoSUB, eoXOR
               \param rResult [out]  resulting region (modified only if operation succeeds)
              
               \remarks
               This method can be called with different eOper and rResult values
               on the same set of input arguments. For example, you can call pushRegions
               few times to add arguments and then call evalPushedRegions twice to get
               union (eoOR) and intersection (eoAND) of these arguments.
            */
            Result evalPushedRegions( Operations eOper, Profile2D& rResult );

            /** \details
               Intersects rSource with rStencilRegion and stores modified source contours
               with updated metadata  in rResult
              
               \param rSource [in]  sorce contours (open, closed, any )
               \param rStencilRegion [in]  contours of the stencil region
               \param rResult [out]  resulting contours (modified only if operation succeeds)
               \param uMask   [in] flags to modify. use OdUInt32(-1) to modify all flags
               \param uInBits [in] set these bits on segments inside rStencilRegion
               \param uOutBits [in] set these bits on segments outside rStencilRegion
               \param uBoundaryBits [in] set these bits on boundary segments
              
               \returns
               returns result of operation.
              
               \remarks
               rResult can coincide/overlap rSource or rStencilRegion
                                   
               Example:
               What stencilFlags( cSrc, cStencil, cSrc, 0x1F, 0x1, 0x0, 0x3 ) does ?
               It splits cSrc segments by cStencil (so that every resulting segment 
               lays either inside or outside or on boundary of cStencil region).
               Then it updates metadata() values of the resulting segments. 
               It clears 5 lower bits (0x1F) of all metadata and then sets 
               bit 0 (0x1) on inside segments and bits 0 and 1 (0x3) on boundary ones.
            */
            Result stencilFlags( 
                ArgStub rSource, 
                ArgStub rStencilRegion, 
                Profile2D& rResult,
                OdUInt32 uMask,
                OdUInt32 uInBits,
                OdUInt32 uOutBits = 0,
                OdUInt32 uBoundaryBits = 0
                );

        private:
            OdGeTol m_gTol;
            FacetModelerProfile2DBool::Intersector * m_pIntersector;
    };
};

#endif  //__FMPROFILE2DBOOL_H__
