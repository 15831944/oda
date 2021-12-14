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
// Helper for metafile and selection markers iteration during metafile stream processing.

#ifndef ODTRVISMARKERSITERATOR
#define ODTRVISMARKERSITERATOR

#include "TD_PackPush.h"

#include <limits.h>
#include "TrVisMetafileStream.h"

/** \details
    <group ExRender_Classes>
*/
template <typename InprocRegisters, typename IntType = OdInt32>
class OdTrVisMarkersIterator
{
  OdUInt8 m_hasMarkers, m_nMinMark, m_nMaxMark, m_actMarkers;
  public:
    OdTrVisMarkersIterator() {}
    OD_FORCEINLINE OdUInt8 init(const InprocRegisters &inprocCtx, const IntType first, const IntType count)
    { m_hasMarkers = m_actMarkers = 0;
      if (inprocCtx.bMarksActivity)
      { m_nMinMark = 255;
        for (OdUInt8 nMark = 0; nMark < OdTrVisMetaRecMarkerType_NTypes; nMark++)
        {
          if (GETBIT(inprocCtx.bMarksActivity, 1 << nMark))
          {
            SETBIT(m_hasMarkers, 1 << nMark, inprocCtx.nMarks[nMark] && (inprocCtx.pMarks[nMark]->m_uFrom < OdUInt32(count))
                                                   /* #VIS-1776 : */ && (inprocCtx.pMarks[nMark]->m_uFrom >= OdUInt32(first)));
            if (GETBIT(m_hasMarkers, 1 << nMark))
            {
              if (m_nMinMark == 255)
                m_nMinMark = m_nMaxMark = nMark;
              else
                m_nMaxMark = nMark;
            }
          }
        }
      }
      return m_hasMarkers;
    }
    OD_FORCEINLINE IntType diff(const InprocRegisters &inprocCtx, const IntType first)
    { IntType ctDiff = INT_MAX;
      for (OdUInt8 nMark = m_nMinMark; nMark <= m_nMaxMark; nMark++)
      { const IntType curDiff = inprocCtx.pMarks[nMark]->m_uFrom - first;
        if (/*nMark == m_nMinMark ||*/ curDiff < ctDiff)
          ctDiff = curDiff, m_actMarkers = 1 << nMark;
        else if (curDiff == ctDiff)
          SETBIT_1(m_actMarkers, 1 << nMark);
      }
      return ctDiff;
    }
    OdUInt8 activeMarks() const { return m_actMarkers; }
    bool isActiveMark(OdTrVisMetaRecMarkerType nMark) const { return GETBIT(m_actMarkers, 1 << nMark); }
    OdUInt8 minimalMark() const { return m_nMinMark; }
    OdUInt8 maximalMark() const { return m_nMaxMark; }
    OD_FORCEINLINE void inc(InprocRegisters &inprocCtx)
    {
      for (OdUInt8 nMark = m_nMinMark; nMark <= m_nMaxMark; nMark++)
      {
        if (GETBIT(m_actMarkers, 1 << nMark))
          inprocCtx.pMarks[nMark]++, inprocCtx.nMarks[nMark]--;
      }
    }
};

#include "TD_PackPop.h"

#endif // ODTRVISMARKERSITERATOR
