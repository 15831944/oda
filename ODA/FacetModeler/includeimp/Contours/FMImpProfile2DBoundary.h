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
#ifndef __FMIMPPROFILE2DBOUNDARY_H__
#define __FMIMPPROFILE2DBOUNDARY_H__

#include "Contours/FMImpSegmentNet2D.h"

namespace FacetModeler
{

class Profile2DBoundaryImpl
{
    public:
        Profile2DBoundaryImpl( const OdGeTol& tol )
        {
            m_cCore.reset( tol );
            m_bCachedRawData = false;
            m_aGeoS.clear();
            m_aGeoC.clear();
            m_aGeoP.clear();
        }

        const OdGeTol& tol() const
        {
            return m_cCore.getTol();
        }

        void setTol( const OdGeTol& tTol )
        {
            invalidateGeometry();
            m_cCore.reset( tTol );
        }

        inline void invalidateGeometry()
        {
            m_bCachedRawData = false;
        }

        inline void clearGeometry()
        {
            invalidateGeometry();

            m_aGeoS.clear();
            m_aGeoC.clear();
            m_aGeoP.clear();
        }

        void updateGeometry()
        {
            if ( m_bCachedRawData )
                return;

            m_cCore.reset( m_cCore.getTol() );

            size_t i, iSize = m_aGeoS.size();
            for ( i = 0; i < iSize; i++ )
                m_cCore.addSegment( m_aGeoS[ i ] );

            iSize = m_aGeoC.size();
            for ( i = 0; i < iSize; i++ )
                m_cCore.addSegments( m_aGeoC[ i ] );

            iSize = m_aGeoP.size();
            for ( i = 0; i < iSize; i++ )
                m_cCore.addSegments( m_aGeoP[ i ] );

            m_bCachedRawData = true;
        }

        bool m_bCachedRawData;
        std::vector<Segment2D> m_aGeoS;
        std::vector<Contour2D> m_aGeoC;
        std::vector<Profile2D> m_aGeoP;

        FMSegmentNet2D::SegNetProcessor m_cCore;
};

}

#endif // __FMIMPPROFILE2DBOUNDARY_H__
