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
#ifndef __FM_P2DB_EDGE_FLAGS_H__
#define __FM_P2DB_EDGE_FLAGS_H__

#include "FMContoursBase.h"
#include "FMProfile2DBool.h"


namespace FacetModelerProfile2DBool
{
    using FacetModeler::Locations;

    // IEdgeInfo callback interface is used by IFlagSetter implementations 
    // to obtain edge information and set appropriate flags
    // The interface is overriden and implemented by Intersector class
    class IEdgeInfo
    {
    public:
        virtual ~IEdgeInfo( ) { };

        // returns group/argument index of this edge. Usually 0, because
        // Intersector handles edges from the first argument only (at present: 2005-09-09)
        virtual OdUInt32  ownerGroup() const = 0;

        // returns true, if the edge is paired with some other edge (from any group)
        virtual bool      isPaired( ) const = 0;

        // returns true if the edge is paired with some edge of another group
        virtual bool      isBoundary( ) const = 0;

        // returns edge location WRT some group/argument of Intersector
        virtual Locations location( OdUInt32 uGroup = 1 ) const = 0;
    };



    // IFlagSetter is a callback used by Intersector::evaluateFlags.
    // Override get() method to modify flags using IEdgeInfo argument.
    class IFlagSetter 
    {
    public:
        virtual ~IFlagSetter( ) { };

        virtual OdUInt32 get( OdUInt32 uSourceFlags, const IEdgeInfo& ) const { return uSourceFlags; };
    };


    class StdFlagSetter: public IFlagSetter
    {
    public:
        StdFlagSetter( ) : m_uMask(0), m_uFlagsIn(0), m_uFlagsOut(0), m_uFlagsBnd(0) { };
        StdFlagSetter( OdUInt32 uMask, OdUInt32 uIn, OdUInt32 uOut = 0, OdUInt32 uBnd = 0); 
       
        StdFlagSetter& setFlags( OdUInt32 uMask, OdUInt32 uIn, OdUInt32 uOut = 0, OdUInt32 uBnd = 0 );

        virtual OdUInt32 get( OdUInt32 uSourceFlags, const IEdgeInfo& rEdgeInfo ) const;

    private:
        OdUInt32 m_uMask;
        OdUInt32 m_uFlagsIn;
        OdUInt32 m_uFlagsOut;
        OdUInt32 m_uFlagsBnd;
    };

};

#endif  //__FM_P2DB_EDGE_FLAGS_H__
