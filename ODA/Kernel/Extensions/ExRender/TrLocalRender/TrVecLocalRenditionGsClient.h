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

#ifndef ODGLES2LOCALRENDITIONCLIENT
#define ODGLES2LOCALRENDITIONCLIENT

#include "TD_PackPush.h"

#include "TrRndLocalRenditionHost.h"
#include "TrRndRenderModule.h"

// Forward declarations
class OdTrVisGsBranchMarkersAccessor;

/** \details
    <group ExRender_Windows_Classes>
*/
struct OdTrVecStateBranchGsClient : public OdTrRndStateBranchClient
{
  mutable OdTrVisGsBranchMarkersAccessor *m_pUsedAccessors, *m_pFreedAccessors;
  OdTrVecStateBranchGsClient() : m_pUsedAccessors(NULL), m_pFreedAccessors(NULL) {}
  ~OdTrVecStateBranchGsClient();

  virtual const void *stateBranchGetPtr(OdTrVisStateBranchId branchId) const;
  virtual OdTrVisStateBranchId stateBranchId(const void *pStateBranch) const;
  virtual bool stateBranchIsEmpty(const void *pStateBranch) const;
  virtual bool stateBranchHasMarkers(const void *pStateBranch) const;
  virtual const void *stateBranchGetChild(const void *pStateBranch, OdTrVisOwning childId) const;
  virtual bool stateBranchHasMarker(const void *pStateBranch, OdTrVisGsMarker gsMarker) const;
  virtual OdUInt32 stateBranchStyle(const void* pStateBranch) const;

  virtual OdTrRndMarkersAccessor* stateBranchMarkers( const void* pStateBranch) const;
  virtual void releaseMarkersAccessor( OdTrRndMarkersAccessor* pAccessor ) const;

  static OdTrRndStateBranchClientPtr createObject();
};

/** \details
    <group ExRender_Windows_Classes>
*/
class OdTrVecLocalRenditionGsClient : public OdTrRndLocalRenditionClient
{
  protected:
    OdTrRndStateBranchClientPtr m_pStateBranchClient;
    OdTrRndRenderModulePtr   m_pRenderModule;
  public:
    // Setup rendering module
    OdTrRndRenderModule *renderModule() { return m_pRenderModule; }
    const OdTrRndRenderModule *renderModule() const { return m_pRenderModule; }

    // Returns local context
    virtual OdTrRndLocalContextPtr createLocalContext(OdTrVisRenderClient *pDevice);

    // Returns optional state branches interface (if true, rendition will be initialized as Gs-based)
    virtual OdTrRndStateBranchClientPtr stateBranchClient();

    // Returns optional resource sharing provider (for multiple devices session sharing)
    virtual OdTrVisSharingProviderClientPtr createSharingProvider();

    // Predefined textures generation
    virtual bool generateLinetypeTexture(OdPs::LineType ltpId, OdUInt8Array& outPixels, OdUInt8 forePixVal) const;
    virtual bool generateFilltypeTexture(OdPs::FillStyle fsId, OdUInt8Array& outPixels, OdUInt32 dimX, OdUInt32 dimY, OdUInt8 forePixVal) const;

    static OdTrRndLocalRenditionClientPtr createObject(OdTrRndRenderModule *pRenderModule);
};

#include "TD_PackPop.h"

#endif // ODGLES2LOCALRENDITIONCLIENT
