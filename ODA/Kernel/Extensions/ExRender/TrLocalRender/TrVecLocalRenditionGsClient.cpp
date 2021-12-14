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
// TrGL2 renderer local client

#include "OdaCommon.h"
#include "Gi/GiPlotStyleDefs.h"
#include "Gs/GsHighlightData.h"
#include "TrVecLocalSharingProvider.h"
#include "TrVecLocalRenditionGsClient.h"

// OdTrVecStateBranchGsClient

const void *OdTrVecStateBranchGsClient::stateBranchGetPtr(OdTrVisStateBranchId branchId) const
{
  return OdTrVisIdToPtr(void, branchId);
}

OdTrVisStateBranchId OdTrVecStateBranchGsClient::stateBranchId(const void *pStateBranch) const
{
  return (OdTrVisStateBranchId)OdTrVisPtrToId(pStateBranch);
}

bool OdTrVecStateBranchGsClient::stateBranchIsEmpty(const void *pStateBranchIn) const
{ const OdGsStateBranch *pStateBranch = reinterpret_cast<const OdGsStateBranch*>(pStateBranchIn);
  return pStateBranch->isEmpty();
}

bool OdTrVecStateBranchGsClient::stateBranchHasMarkers(const void *pStateBranchIn) const
{ const OdGsStateBranch *pStateBranch = reinterpret_cast<const OdGsStateBranch*>(pStateBranchIn);
  return !pStateBranch->markersEmpty();
}

const void *OdTrVecStateBranchGsClient::stateBranchGetChild(const void *pStateBranchIn, OdTrVisOwning childId) const
{ const OdGsStateBranch *pStateBranch = reinterpret_cast<const OdGsStateBranch*>(pStateBranchIn);
  return const_cast<OdGsStateBranch*>(pStateBranch)->findChild(OdTrVisIdToPtr(OdDbStub, childId));
}

bool OdTrVecStateBranchGsClient::stateBranchHasMarker(const void *pStateBranchIn, OdTrVisGsMarker gsMarker) const
{ const OdGsStateBranch *pStateBranch = reinterpret_cast<const OdGsStateBranch*>(pStateBranchIn);
  return pStateBranch->hasMarker((OdGsMarker)gsMarker);
}

OdUInt32 OdTrVecStateBranchGsClient::stateBranchStyle(const void* pStateBranchIn) const
{ const OdGsStateBranch *pStateBranch = reinterpret_cast<const OdGsStateBranch*>(pStateBranchIn);
  return pStateBranch->style();
}

class OdTrVisGsBranchMarkersAccessor : public OdTrRndMarkersAccessor
{
protected:
  const OdGsMarkerSet* m_pSet;
  OdTrVisGsBranchMarkersAccessor *m_pNext;
public:
  OdTrVisGsBranchMarkersAccessor( const OdGsMarkerSet* pSet ) : m_pSet( pSet ), m_pNext(NULL) {}
  ~OdTrVisGsBranchMarkersAccessor() { }

  inline void setNext(OdTrVisGsBranchMarkersAccessor *pNext) { m_pNext = pNext; }
  inline OdTrVisGsBranchMarkersAccessor *getNext() const { return m_pNext; }

  inline void setMarkersSet(const OdGsMarkerSet *pSet) { m_pSet = pSet; }
  const OdGsMarkerSet *markersSet() const { return m_pSet; }

  virtual bool empty() const
  {
    if( !m_pSet ) return true;
    return m_pSet->empty();
  }
  virtual bool hasMarker( OdGsMarker marker ) const
  {
    if( !m_pSet ) return false;
    return m_pSet->find( marker ) != m_pSet->end();
  }
  virtual bool interval( OdGsMarker& minMarker, OdGsMarker& maxMarker ) const
  {
    if( !m_pSet || m_pSet->empty() ) return false;
    minMarker = *m_pSet->begin();
    maxMarker = *m_pSet->rbegin();
    return true;
  }
};

OdTrVecStateBranchGsClient::~OdTrVecStateBranchGsClient()
{
  while (m_pFreedAccessors)
  { OdTrVisGsBranchMarkersAccessor *pAccessor = m_pFreedAccessors;
    m_pFreedAccessors = pAccessor->getNext();
    delete pAccessor;
  }
  ODA_ASSERT(!m_pUsedAccessors);
}

OdTrRndMarkersAccessor* OdTrVecStateBranchGsClient::stateBranchMarkers( const void* pHlBranchIn ) const
{
  const OdGsStateBranch* pHlBranch = reinterpret_cast<const OdGsStateBranch*>( pHlBranchIn );
  if( !pHlBranch ) return NULL;
  OdTrVisGsBranchMarkersAccessor *pAccessor;
  if (m_pFreedAccessors)
    pAccessor = m_pFreedAccessors,
    m_pFreedAccessors = pAccessor->getNext(),
    pAccessor->setMarkersSet(&pHlBranch->markers());
  else
    pAccessor = new OdTrVisGsBranchMarkersAccessor(&pHlBranch->markers());
  pAccessor->setNext(m_pUsedAccessors);
  m_pUsedAccessors = pAccessor;
  return pAccessor;
}

void OdTrVecStateBranchGsClient::releaseMarkersAccessor( OdTrRndMarkersAccessor* pAccessor ) const
{
  OdTrVisGsBranchMarkersAccessor* pA = static_cast<OdTrVisGsBranchMarkersAccessor*>( pAccessor );
  OdTrVisGsBranchMarkersAccessor *pPrevAccessor = NULL, *pCurAccessor = m_pUsedAccessors;
  while (pCurAccessor)
  {
    if (pCurAccessor == pA)
      break;
    pPrevAccessor = pCurAccessor;
    pCurAccessor = pCurAccessor->getNext();
  }
  ODA_ASSERT(pCurAccessor != NULL);
  if (pCurAccessor)
  {
    if (pPrevAccessor)
      pPrevAccessor->setNext(pCurAccessor->getNext());
    else
      m_pUsedAccessors = pCurAccessor->getNext();
    pCurAccessor->setNext(m_pFreedAccessors);
    m_pFreedAccessors = pCurAccessor;
  }
  else
    delete pA;
}

OdTrRndStateBranchClientPtr OdTrVecStateBranchGsClient::createObject()
{
  return OdRxObjectImpl<OdTrVecStateBranchGsClient, OdTrRndStateBranchClient>::createObject();
}

// OdTrVecLocalRenditionGsClient

OdTrRndLocalContextPtr OdTrVecLocalRenditionGsClient::createLocalContext(OdTrVisRenderClient *pDevice)
{
  return OdTrRndLocalContextPtr();
}

OdTrRndStateBranchClientPtr OdTrVecLocalRenditionGsClient::stateBranchClient()
{
  if (m_pStateBranchClient.isNull())
    m_pStateBranchClient = OdTrVecStateBranchGsClient::createObject();
  return m_pStateBranchClient;
}

OdTrVisSharingProviderClientPtr OdTrVecLocalRenditionGsClient::createSharingProvider()
{
  return OdTrVecLocalSharingProvider::createObject(renderModule());
}

bool OdTrVecLocalRenditionGsClient::generateLinetypeTexture(OdPs::LineType ltpId, OdUInt8Array& outPixels, OdUInt8 forePixVal) const
{
  outPixels = ::odgiGetPsLinetypesManager().rasterizeLinetype(ltpId, 2, forePixVal);
  return true;
}

bool OdTrVecLocalRenditionGsClient::generateFilltypeTexture(OdPs::FillStyle fsId, OdUInt8Array& outPixels, OdUInt32 dimX, OdUInt32 dimY, OdUInt8 forePixVal) const
{
  outPixels = ::odgiGetPsFillstylesManager().rasterizeFillstyle(fsId, 0, 0, dimX, dimY, forePixVal);
  return true;
}

OdTrRndLocalRenditionClientPtr OdTrVecLocalRenditionGsClient::createObject(OdTrRndRenderModule *pRenderModule)
{
  OdSmartPtr<OdTrVecLocalRenditionGsClient> pLRC = OdRxObjectImpl<OdTrVecLocalRenditionGsClient>::createObject();
  pLRC->m_pRenderModule = pRenderModule;
  return pLRC;
}

//
