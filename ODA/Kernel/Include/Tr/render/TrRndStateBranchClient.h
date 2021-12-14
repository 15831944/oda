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
// State branch access clients.

#ifndef ODTRRNDSTATEBRANCHCLIENT
#define ODTRRNDSTATEBRANCHCLIENT

#include "TD_PackPush.h"

#include "RxObject.h"
#include "../TrVisDefs.h"

/** \details
  This is an interface for markers access. It should be faster then accessing through rendition.
*/
class OdTrRndMarkersAccessor
{
public:
  /** \details
  Checks that markers conteiner is not empty
  \returns Returns true if and only if markers empty
  */
  virtual bool empty() const = 0;
  /** \details
  Checks that marker is presented in container
  \param marker [in] Marker to be checked
  \returns Returns true if and only if markers is presented in container
  */
  virtual bool hasMarker( OdGsMarker marker ) const = 0;
  /** \details
  Retrives markers container boundaries
  \param minMarker [out] Minimum marker in container
  \param maxMarker [out] Maximum marker in container
  \returns Returns true if container supports boundaries detection; otherwise returns false.
  \remarks
  Implementation of OdTrRndMarkersAccessor may not support fast boundaries detection.
  */
  virtual bool interval( OdGsMarker& minMarker, OdGsMarker& maxMarker ) const { return false; }
};

/** \details
    <group ExRender_Windows_Classes> 
*/
struct OdTrRndStateBranchClientInterface
{
  virtual const void *stateBranchGetPtr(OdTrVisStateBranchId stateBranchId) const = 0;
  virtual OdTrVisStateBranchId stateBranchId(const void *pStateBranch) const = 0;
  virtual bool stateBranchIsEmpty(const void *pStateBranch) const = 0;
  virtual bool stateBranchHasMarkers(const void *pStateBranch) const = 0;
  virtual const void *stateBranchGetChild(const void *pStateBranch, OdTrVisOwning childId) const = 0;
  virtual bool stateBranchHasMarker(const void *pStateBranch, OdTrVisGsMarker gsMarker) const = 0;
  virtual OdUInt32 stateBranchStyle(const void* pStateBranch) const = 0;
  /** \details
  Returns state branch markers as OdTrRndMarkersAccessor
  \param pStateBranch [in] Branch identificator.
  \returns Returns instance of OdTrRndMarkersAccessor or NULL if presentation is not supported.
  */
  virtual OdTrRndMarkersAccessor* stateBranchMarkers( const void* pStateBranch ) const = 0;
  /** \details
  Releases OdTrRndMarkersAccessor
  \param pAccessor [in] Pointer to the OdTrRndMarkersAccessor
  */
  virtual void releaseMarkersAccessor( OdTrRndMarkersAccessor* pAccessor ) const = 0;
};

/** \details
    <group ExRender_Windows_Classes> 
*/
class OdTrRndStateBranchClient : public OdRxObject, public OdTrRndStateBranchClientInterface
{
};

/** \details
    <group ExRender_Windows_Classes>
*/
typedef OdSmartPtr<OdTrRndStateBranchClient> OdTrRndStateBranchClientPtr;

#include "TD_PackPop.h"

#endif // ODTRRNDSTATEBRANCHCLIENT
