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

#include "OdaCommon.h"

#include "BrepBuilderInitialParams.h"
#include "ExtDbUtils.h"


namespace
{
  OdBrepBuilderFillerParams::BrepType defaultBrepType(OdExtDbUtils::DbType dbType)
  {
    switch (dbType) {
    case OdExtDbUtils::kDbDwg: return OdBrepBuilderFillerParams::kBrepAcisDwg;
    case OdExtDbUtils::kDbDgn: return OdBrepBuilderFillerParams::kBrepAcisDgn;// default for dgn because of bb
    case OdExtDbUtils::kDbVis: return OdBrepBuilderFillerParams::kBrepVisualize;
    case OdExtDbUtils::kDbPrc: return OdBrepBuilderFillerParams::kBrepPrc;
    case OdExtDbUtils::kDbIfc: return OdBrepBuilderFillerParams::kBrepIfc;
    case OdExtDbUtils::kDbBim: return OdBrepBuilderFillerParams::kBrepBimRv;
    }
    return OdBrepBuilderFillerParams::kBrepUnknown;
  }

  OdBrepBuilderFillerParams::BrepType defaultSourceBrepType(OdDbBaseDatabase* pSourceDb)
  {
    if (!pSourceDb) {
      return OdBrepBuilderFillerParams::kBrepUnknown;
    }
    OdExtDbUtils::DbType sourceDbType = OdExtDbUtils::getProduct(pSourceDb);
    if (OdExtDbUtils::kDbDgn == sourceDbType) {
      // can't determine: kBrepAcisDgn/kBrepPS
      return OdBrepBuilderFillerParams::kBrepUnknown;
    }
    return defaultBrepType(sourceDbType);
  }

  OdBrepBuilderFillerParams::BrepType defaultDestinationBrepType(OdDbBaseDatabase* pDestinationDb)
  {
    if (!pDestinationDb) {
      return OdBrepBuilderFillerParams::kBrepUnknown;
    }
    OdExtDbUtils::DbType destinationDbType = OdExtDbUtils::getProduct(pDestinationDb);
    return defaultBrepType(destinationDbType);
  }
}

void OdBrepBuilderFillerParams::resetOptions()
{
  m_options = optsDefault;
  switch (m_destinationBrepType)
  {
  case kBrepMd:
    m_options |= optsGenerateVertices;
    if (kBrepAcisDwg == m_sourceBrepType)
      m_options |= optsMake2dIntervalInclude3d;// TODO recheck acis 2d <-> 3d interval requirements
    break;
  case kBrepPrc: m_options |= optsSkipCoedge2dCurve; break;
  case kBrepAcisDwg:
  case kBrepAcisDgn:
  case kBrepVisualize: m_options |= optsMakeEllipMajorGreaterMinor | optsMake2dIntervalInclude3d; break;
  case kBrepBimRv: m_options |= optsIgnoreComplexShell; break;
  }
}

OdBrepBuilderFillerParams& OdBrepBuilderFillerParams::setupFor(OdDbBaseDatabase* pSourceDb, OdDbBaseDatabase* pDestinationDb)
{
  m_pSourceDb = pSourceDb;
  m_pDestinationDb = pDestinationDb;
  m_sourceBrepType = defaultSourceBrepType(pSourceDb);
  m_destinationBrepType = defaultDestinationBrepType(pDestinationDb);
  resetOptions();
  return *this;
}

OdBrepBuilderFillerParams& OdBrepBuilderFillerParams::setupFor(BrepType sourceBrepType, OdDbBaseDatabase* pSourceDb, OdDbBaseDatabase* pDestinationDb)
{
  m_pSourceDb = pSourceDb;
  m_pDestinationDb = pDestinationDb;
  m_sourceBrepType = sourceBrepType;
  m_destinationBrepType = defaultDestinationBrepType(pDestinationDb);
  resetOptions();
  return *this;
}

OdBrepBuilderFillerParams& OdBrepBuilderFillerParams::setupFor(BrepType sourceBrepType, OdDbBaseDatabase* pDestinationDb)
{
  m_pSourceDb = NULL;
  m_pDestinationDb = pDestinationDb;
  m_sourceBrepType = sourceBrepType;
  m_destinationBrepType = defaultDestinationBrepType(pDestinationDb);
  resetOptions();
  return *this;
}

OdBrepBuilderFillerParams& OdBrepBuilderFillerParams::setupFor(OdDbBaseDatabase* pSourceDb, BrepType destinationBrepType)
{
  m_pSourceDb = pSourceDb;
  m_pDestinationDb = NULL;
  m_sourceBrepType = defaultSourceBrepType(pSourceDb);
  m_destinationBrepType = destinationBrepType;
  resetOptions();
  return *this;
}

OdBrepBuilderFillerParams& OdBrepBuilderFillerParams::setupFor(BrepType sourceBrepType, BrepType destinationBrepType)
{
  m_pSourceDb = NULL;
  m_pDestinationDb = NULL;
  m_sourceBrepType = sourceBrepType;
  m_destinationBrepType = destinationBrepType;
  resetOptions();
  return *this;
}
