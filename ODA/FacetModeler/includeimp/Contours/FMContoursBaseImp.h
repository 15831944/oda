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
#ifndef __FMIMPCONTOURSBASE_H__
#define __FMIMPCONTOURSBASE_H__

#include "FMContoursBase.h"

namespace FacetModeler
{

    // Returns first error code (if any), or first warning (if no errors)
    inline Result worstResult( Result eRes1, Result eRes2 )
    { return odmax( eRes1, eRes2 ); }

    // The old code cuts warnings of eRes2 if eRes1 is OK
//    { return (!isError(eRes2) || isError( eRes1 ) ) ? eRes1 : eRes2; };

    // Maybe better, but slower solution looks like this one:
//    inline Result worstResult( Result eRes1, Result eRes2 )
//    {
//        int severity1 = 2*isError(eRes1) + isWarning(eRes1);
//        int severity2 = 2*isError(eRes2) + isWarning(eRes2);
//        return ( severity1 >= severity2 ? eRes1 : eRes2 );
//    }


    class FMGEOMETRY_API Attributes2D
    {
        OdIntPtr m_uMetadata; //value which is able to store a pointer;  size depends on platform type 32\64

        union
        {
            OdUInt32 m_uAllFlags;
            struct
            {
#ifdef ODA_BIGENDIAN
                OdUInt32 bSmooth        : 1;
                OdUInt32 bExplodedArc   : 1;
                OdUInt32 bHidden        : 1;
                OdUInt32 uOrientation   : 8;
#else
                OdUInt32 uOrientation   : 8;
                OdUInt32 bHidden        : 1;
                OdUInt32 bExplodedArc   : 1;
                OdUInt32 bSmooth        : 1;
#endif
            } m_Flags;
        };
    public:
        Attributes2D() : m_uMetadata(0), m_uAllFlags(0) { }
        explicit Attributes2D( OdIntPtr uMeta ) : m_uMetadata(uMeta), m_uAllFlags(0) { }
        explicit Attributes2D( OdIntPtr uMeta, OdUInt32 uRawFlags ) : m_uMetadata(uMeta), m_uAllFlags(uRawFlags) { }

        inline Attributes2D clone() const { return *this; }

        inline Attributes2D& operator = (const Attributes2D& rA )
        {
            m_uMetadata = rA.m_uMetadata;
            m_uAllFlags = rA.m_uAllFlags;
            return *this;
        }

        inline bool operator == (const Attributes2D& rA ) const
        {
            return m_uMetadata == rA.m_uMetadata && m_uAllFlags == rA.m_uAllFlags;
        }

        inline bool operator != (const Attributes2D& rA ) const
        {
            return !( *this == rA );
        }

        inline bool isNull() const
        {
            return 0==m_uMetadata && 0==m_uAllFlags;
        }

        Attributes2D& reverse();
        inline Attributes2D getReversed() const { return Attributes2D(*this).reverse(); }

        inline OdIntPtr     metadata()  const   { return m_uMetadata; }
        inline OdIntPtr&    metadata()          { return m_uMetadata; }

    public:

        inline OdUInt32     flags()     const           { return m_uAllFlags; }
        inline Attributes2D& setFlags( OdUInt32 uF )    { m_uAllFlags = uF; return *this; }
        
        inline FaceOrientation orientation() const 
        {
            return FaceOrientation( m_Flags.uOrientation ); 
        }
        inline Attributes2D& setOrientation( FaceOrientation eOri )
        {
            m_Flags.uOrientation = OdUInt32(eOri);
            return *this;
        }

        inline bool isHidden()  const       
        { 
            return m_Flags.bHidden; 
        }
        inline Attributes2D& setHidden(bool bH )     
        { 
            m_Flags.bHidden = bH;   
            return *this;
        }

        inline bool isSmooth() const { 
          return m_Flags.bSmooth;
        }
        inline Attributes2D& setSmooth(bool bSmooth ) { 
          m_Flags.bSmooth = bSmooth;
          return *this;
        }

        inline bool isExplodedArc()  const       
        { 
            return m_Flags.bExplodedArc; 
        }
        inline Attributes2D& setExplodedArc(bool bExploded )     
        { 
            m_Flags.bExplodedArc = bExploded;
            return *this;
        }

    public:
        FMGEOMETRY_API_STATIC static const Attributes2D kNull;
    };

    FMGEOMETRY_API OdGePoint2d FindNearestPoint(const OdGePoint2d& ptOrigin, const std::vector< Intersection >& vecPoints);
    FMGEOMETRY_API OdGePoint3d FindNearestPoint(const OdGePoint3d& ptOrigin, const std::vector<OdGePoint3d>& vecPoints);

    FMGEOMETRY_API OdGePoint2d FindFarthestPoint(const OdGePoint2d& ptOrigin, const std::vector< Intersection >& vecPoints);
};

#endif  //__FMIMPCONTOURSBASE_H__
