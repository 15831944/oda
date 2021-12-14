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
// MetafileId, DisplayId and MetafilePtr converters

#ifndef ODVISDISPLAYIDCONVERTER
#define ODVISDISPLAYIDCONVERTER

#include "TrVisDefs.h"

#include "TD_PackPush.h"

// Forward declarations
struct OdTrVisMetafileContainer;

/** \details
    <group ExRender_Windows_Classes>
*/
struct OdTrVisMetafileDisplayIdConverter
{
  // MetafileId converters
  virtual OdTrVisDisplayId metafileIdToDisplayId(OdTrVisMetafileId metafileId) const = 0;
  virtual OdTrVisMetafileContainer *metafileIdToMetafilePtr(OdTrVisMetafileId metafileId) const = 0;
  // DisplayId converters
  virtual OdTrVisMetafileId displayIdToMetafileId(OdTrVisDisplayId displayId) const = 0;
  virtual OdTrVisMetafileContainer *displayIdToMetafilePtr(OdTrVisDisplayId displayId) const = 0;
  // MetafilePtr converters
  virtual OdTrVisMetafileId metafilePtrToMetafileId(const OdTrVisMetafileContainer *pMf) const = 0;
  virtual OdTrVisDisplayId metafilePtrToDisplayId(const OdTrVisMetafileContainer *pMf) const = 0;
};

/** \details
    Interpret MetafileId, DisplayId and MetafilePtr as same primitives.
    <group ExRender_Windows_Classes>
*/
struct OdTrVisMetafileDisplayIdConverter_direct : public OdTrVisMetafileDisplayIdConverter
{
  static OdTrVisMetafileDisplayIdConverter_direct g_converterStub;
  // MetafileId converters
  virtual OdTrVisDisplayId metafileIdToDisplayId(OdTrVisMetafileId metafileId) const { return (OdTrVisDisplayId)metafileId; }
  virtual OdTrVisMetafileContainer *metafileIdToMetafilePtr(OdTrVisMetafileId metafileId) const { return OdTrVisIdToPtr(OdTrVisMetafileContainer, metafileId); }
  // DisplayId converters
  virtual OdTrVisMetafileId displayIdToMetafileId(OdTrVisDisplayId displayId) const { return (OdTrVisMetafileId)displayId; }
  virtual OdTrVisMetafileContainer *displayIdToMetafilePtr(OdTrVisDisplayId displayId) const { return OdTrVisIdToPtr(OdTrVisMetafileContainer, displayId); }
  // MetafilePtr converters
  virtual OdTrVisMetafileId metafilePtrToMetafileId(const OdTrVisMetafileContainer *pMf) const { return (OdTrVisMetafileId)OdTrVisPtrToId(pMf); }
  virtual OdTrVisDisplayId metafilePtrToDisplayId(const OdTrVisMetafileContainer *pMf) const { return (OdTrVisDisplayId)OdTrVisPtrToId(pMf); }
};

#include "TD_PackPop.h"

#endif // ODVISDISPLAYIDCONVERTER
