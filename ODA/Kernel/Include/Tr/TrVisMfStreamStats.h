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
// Metafile stream statistics calculation

#ifndef _EXTRVISMFSTREAMSTATS_INCLUDED_
#define _EXTRVISMFSTREAMSTATS_INCLUDED_

#include "TD_PackPush.h"

#include "TrVisMetafileStream.h"

/** \details
  Compute statistical metafile stream information.

  Library: Source code provided.

  <group ExRender_Classes>
*/
class OdTrVisMetafileStreamStats
{
  protected:
    typedef std::pair<OdUInt32, OdUInt64> DataAsB;
    DataAsB m_chunkStats[OdTrVisMetaRecType_NTypes];
    DataAsB m_arrayStats[OdTrVisArrayWrapper::Type_Invalid];
    OdUInt32 m_nMetafiles; OdTrVisMetaRecType m_lastChunk;
    const OdUInt8 *m_pPrevStreamPos;
  protected:
  public:
    OdTrVisMetafileStreamStats() { clearStats(); }

    void clearStats()
    {
      ::memset(m_chunkStats, 0, sizeof(m_chunkStats));
      ::memset(m_arrayStats, 0, sizeof(m_arrayStats));
      m_nMetafiles = 0; m_lastChunk = OdTrVisMetaRecType_NTypes; m_pPrevStreamPos = NULL;
    }

    void startMetafileAnalyse(const OdTrVisFlatMetafileContainer *pMf)
    {
      for (OdUInt32 nArray = 0; nArray < pMf->arrayElementsSize(); nArray++)
      { const OdTrVisArrayWrapper &arry = pMf->arrayElement((OdTrVisArrayId)nArray);
        m_arrayStats[arry.m_type].first++; m_arrayStats[arry.m_type].second += arry.m_uSize;
      }
      m_nMetafiles++; m_pPrevStreamPos = pMf->memoryPtr(); m_lastChunk = OdTrVisMetaRecType_NTypes;
    }

    void endMetafileAnalyse(const OdUInt8 *pEofStreamPos)
    {
      if (m_lastChunk != OdTrVisMetaRecType_NTypes)
        m_chunkStats[m_lastChunk].second += (pEofStreamPos - m_pPrevStreamPos) - 1;
    }

    void addChunk(const OdUInt8 *pCurStreamPos)
    { const OdTrVisMetaRecType curChunk = (OdTrVisMetaRecType)*pCurStreamPos;
      endMetafileAnalyse(pCurStreamPos);
      m_chunkStats[m_lastChunk = curChunk].first++; m_pPrevStreamPos = pCurStreamPos;
    }

    OdString printStats() const
    { OdString resStr; DataAsB arrSumm = DataAsB(0, 0), chunkSumm = DataAsB(0, 0);
      resStr += OdString().format(OD_T("Statistics for %u metafile streams:\n"), m_nMetafiles);
      for (OdUInt32 nArray = 0; nArray < OdTrVisArrayWrapper::Type_Invalid; nArray++)
      {
        if (m_arrayStats[nArray].first)
        {
          resStr += OdString().format(OD_T("%u arrays of type %u: %llu bytes\n"), m_arrayStats[nArray].first, nArray, m_arrayStats[nArray].second);
          arrSumm.first += m_arrayStats[nArray].first; arrSumm.second += m_arrayStats[nArray].second;
        }
      }
      resStr += OdString().format(OD_T("Summary for %u arrays: %llu bytes\n"), arrSumm.first, arrSumm.second);
      for (OdUInt32 nChunk = 0; nChunk < OdTrVisMetaRecType_NTypes; nChunk++)
      {
        if (m_chunkStats[nChunk].first)
        {
          resStr += OdString().format(OD_T("%u %ls chunks: %llu bytes\n"), m_chunkStats[nChunk].first, ::chunkDecode((OdTrVisMetaRecType)nChunk), m_chunkStats[nChunk].second);
          chunkSumm.first += m_chunkStats[nChunk].first; chunkSumm.second += m_chunkStats[nChunk].second;
        }
      }
      resStr += OdString().format(OD_T("Summary for %u chunks: %llu bytes\n"), chunkSumm.first, chunkSumm.second);
      resStr += OdString().format(OD_T("Total memory analysed: %llu bytes\n"), arrSumm.second + chunkSumm.second + chunkSumm.first);
      return resStr;
    }
};

#include "TD_PackPop.h"

#endif // _EXTRVISMFSTREAMSTATS_INCLUDED_
