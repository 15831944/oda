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
#ifndef __FMGEOMETRYDEBUG_H__
#define __FMGEOMETRYDEBUG_H__

#include "FMBuildSettings.h"
#include <OdaCommon.h>

typedef void (*FMGeAssertFuncPtr) (
    const OdChar* strFile,
    int iLine,
    const OdChar* strExpr,
    const OdChar* strComment,
    bool* pIgnoreThis 
    );

typedef void (*FMGeTraceFuncPtr) (
    const OdChar* strExpr
    );

FMGeAssertFuncPtr FMGEOMETRY_API FMGeSetAssertFunc( FMGeAssertFuncPtr ptrAssert );
FMGeTraceFuncPtr  FMGEOMETRY_API FMGeSetTraceFunc ( FMGeTraceFuncPtr  ptrTrace  );

void FMGEOMETRY_API FMGeAssert(
    const OdChar* strFile,
    int iLine,
    const OdChar* strExpr,
    const OdChar* strComment,
    bool* pIgnoreThis
);

void FMGEOMETRY_API FMGeTrace( const OdChar* strFormat, ... );


#ifndef FMGE_DEBUG
    #ifdef _DEBUG
      #define FMGE_DEBUG
    #endif
#endif

#ifdef FMGE_DEBUG
    
#ifdef _MSC_VER
    #pragma warning( disable : 4127 )
#endif // _MSC_VER

#define FMGE_ASSERT( x )         \
    {                             \
        static bool bIgn = false; \
                                  \
        if ( ! (x) )              \
        {                         \
            FMGeAssert(          \
                OD_T(__FILE__),   \
                __LINE__,         \
                OD_T(#x),         \
                0,                \
                &bIgn             \
            );                    \
        }                         \
    }

#define FMGE_ASSERTMSG( x, c )   \
    {                             \
        static bool bIgn = false; \
                                  \
        if ( ! (x) )              \
        {                         \
            FMGeAssert(          \
                OD_T(__FILE__),   \
                __LINE__,         \
                OD_T(#x),         \
                OdString(c),      \
                &bIgn             \
            );                    \
        }                         \
    }

#define FMGE_VERIFY( x )         \
    FMGE_ASSERT( x )

#define FMGE_VERIFYMSG( x, c )   \
    FMGE_ASSERTMSG( x, c )

#define FMGE_FAULT( c )          \
    {                             \
        static bool bIgn = false; \
                                  \
        FMGeAssert(              \
            OD_T(__FILE__),       \
            __LINE__,             \
            0,                    \
            OdString(c),          \
            &bIgn                 \
        );                        \
    }

#define FMGE_MUSTNOTEXECUTE                \
    FMGE_FAULT( OD_T("Must not execute!") )

#define FMGE_TRACE( x )                    \
    (FMGeTrace x)

#else  //FMGE_DEBUG
    #define FMGE_ASSERT(x)      ((void)0)
    #define FMGE_ASSERTMSG(x,c) ((void)0)
    #define FMGE_VERIFY(x)      (x)
    #define FMGE_VERIFYMSG(x, c)(x)
    #define FMGE_FAULT(c)       ((void)0)
    #define FMGE_MUSTNOTEXECUTE ((void)0)
    #define FMGE_TRACE(x)       ((void)0)
#endif //FMGE_DEBUG

#endif  //__FMGEOMETRYDEBUG_H__
