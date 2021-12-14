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
// Direct rendering accessor for external data

#ifndef ODTRVISRENDITIONPU
#define ODTRVISRENDITIONPU

#include "TD_PackPush.h"

#include "RxObjectImpl.h"
#include "StaticRxObject.h"
#include "TrVisDefs.h"
#include "TrVisPropertyDef.h"

// Forward declarations
class OdPerfTimerBase;

/** \details
    <group ExRender_Windows_Classes> 
*/
class OdTrVisRenderDataInvokeHid
{
  public:
    virtual ~OdTrVisRenderDataInvokeHid() { }
};

/** \details
    <group ExRender_Windows_Classes> 
*/
class OdTrVisRenderDataInvoke : public OdTrVisRenderDataInvokeHid
{
  public:
    bool incomeDataStream(const OdTrVisDisplayId *pList, OdUInt32 nEntries);

    virtual bool incomeMetafile(OdTrVisDisplayId mfId) = 0;

    virtual bool incomeHighlight(bool /*bHighlight*/) { return true; }
    virtual bool incomeMatrix(const OdGeMatrix3d* /*pXfm*/) { return true; }
    virtual bool incomeFade(long /*nEntry*/, bool /*bFade*/) { return true; }
    /** \details
      Processes highlighting ot visibilities markers.
      \param brType [in] Type of markers branch.
      \param branchId [in] Id of branch.
      \returns true if next data should be processed too.
    */
    virtual bool incomeMarkers(OdTrVisStateBranchPropertyDef::BranchType /*brType*/, OdTrVisStateBranchId /*branchId*/) { return true; }
    virtual bool incomeDrawOrder(const double* /*pDrawOrder*/) { return true; }
    virtual bool incomeExploded(bool /*bExploded*/) { return true; }
    virtual bool incomeSelectionStyle(OdUInt32 /*nStyle*/) { return true; }
    
    /** \details
    Sets processing object id. Used for selection

    \param id [in] object id
    \returns true if next data should be processed too
    */
    virtual bool incomeSelectionID(OdTrVisId /*id*/) { return true; }
};

/** \details
    <group ExRender_Windows_Classes> 
*/
class OdTrVisRenderDataInvokeRedirect : public OdTrVisRenderDataInvoke
{
  protected:
    OdTrVisRenderDataInvoke *m_pRedirectTo;
    virtual bool defRv() const { return false; }
  public:
    OdTrVisRenderDataInvokeRedirect() : m_pRedirectTo(NULL) { }

    OdTrVisRenderDataInvoke *getRedirection() const { return m_pRedirectTo; }
    bool hasRedirection() const { return m_pRedirectTo != NULL; }
    void setRedirection(OdTrVisRenderDataInvoke *pRedirection) { m_pRedirectTo = pRedirection; }
    void resetRedirection() { m_pRedirectTo = NULL; }

    virtual bool incomeMetafile(OdTrVisDisplayId mfId)
    {
      if (hasRedirection())
        return getRedirection()->incomeMetafile(mfId);
      return defRv();
    }

    virtual bool incomeHighlight(bool bHighlight)
    {
      if (hasRedirection())
        return getRedirection()->incomeHighlight(bHighlight);
      return defRv();
    }
    virtual bool incomeMatrix(const OdGeMatrix3d *pXfm)
    {
      if (hasRedirection())
        return getRedirection()->incomeMatrix(pXfm);
      return defRv();
    }
    virtual bool incomeFade(long nEntry, bool bFade)
    {
      if (hasRedirection())
        return getRedirection()->incomeFade(nEntry, bFade);
      return defRv();
    }
    virtual bool incomeMarkers(OdTrVisStateBranchPropertyDef::BranchType brType, OdTrVisStateBranchId branchId)
    {
      if (hasRedirection())
        return getRedirection()->incomeMarkers(brType, branchId);
      return defRv();
    }
    virtual bool incomeDrawOrder(const double* pDrawOrder)
    {
      if (hasRedirection())
        return getRedirection()->incomeDrawOrder(pDrawOrder);
      return defRv();
    }
    virtual bool incomeExploded(bool bExploded)
    {
      if (hasRedirection())
        return getRedirection()->incomeExploded(bExploded);
      return defRv();
    }
    virtual bool incomeSelectionStyle(OdUInt32 nStyle)
    {
      if (hasRedirection())
        return getRedirection()->incomeSelectionStyle(nStyle);
      return defRv();
    }
    virtual bool incomeSelectionID(OdTrVisId id)
    {
      if (hasRedirection())
        return getRedirection()->incomeSelectionID(id);
      return defRv();
    }
};

/** \details
    <group ExRender_Windows_Classes> 
*/
class OdTrVisRenderDataInvokeBranch : public OdTrVisRenderDataInvoke
{
  protected:
    OdTrVisRenderDataInvoke *m_pRedirectA, *m_pRedirectB;
    virtual bool defRv() const { return false; }
    struct RvFlag
    {
      bool m_bRv, m_bSet;
      RvFlag() : m_bRv(true), m_bSet(false) {}
      void add(bool bSet) { m_bSet = true; m_bRv = bSet; }
      bool state() const { return m_bRv; }
      bool generateRv() const { return m_bSet; }
    };
  public:
    OdTrVisRenderDataInvokeBranch() : m_pRedirectA(NULL), m_pRedirectB(NULL) { }

    OdTrVisRenderDataInvoke *getRedirectionA() const { return m_pRedirectA; }
    bool hasRedirectionA() const { return m_pRedirectA != NULL; }
    void setRedirectionA(OdTrVisRenderDataInvoke *pRedirection) { m_pRedirectA = pRedirection; }
    void resetRedirectionA() { m_pRedirectA = NULL; }

    OdTrVisRenderDataInvoke *getRedirectionB() const { return m_pRedirectB; }
    bool hasRedirectionB() const { return m_pRedirectB != NULL; }
    void setRedirectionB(OdTrVisRenderDataInvoke *pRedirection) { m_pRedirectB = pRedirection; }
    void resetRedirectionB() { m_pRedirectB = NULL; }

    virtual bool incomeMetafile(OdTrVisDisplayId mfId)
    { RvFlag rv;
      if (hasRedirectionA())
        rv.add(getRedirectionA()->incomeMetafile(mfId));
      if (hasRedirectionB() && rv.state())
        rv.add(getRedirectionB()->incomeMetafile(mfId));
      return rv.generateRv() ? rv.state() : defRv();
    }

    virtual bool incomeHighlight(bool bHighlight)
    { RvFlag rv;
      if (hasRedirectionA())
        rv.add(getRedirectionA()->incomeHighlight(bHighlight));
      if (hasRedirectionB() && rv.state())
        rv.add(getRedirectionB()->incomeHighlight(bHighlight));
      return rv.generateRv() ? rv.state() : defRv();
    }
    virtual bool incomeMatrix(const OdGeMatrix3d *pXfm)
    { RvFlag rv;
      if (hasRedirectionA())
        rv.add(getRedirectionA()->incomeMatrix(pXfm));
      if (hasRedirectionB() && rv.state())
        rv.add(getRedirectionB()->incomeMatrix(pXfm));
      return rv.generateRv() ? rv.state() : defRv();
    }
    virtual bool incomeFade(long nEntry, bool bFade)
    { RvFlag rv;
      if (hasRedirectionA())
        rv.add(getRedirectionA()->incomeFade(nEntry, bFade));
      if (hasRedirectionB() && rv.state())
        rv.add(getRedirectionB()->incomeFade(nEntry, bFade));
      return rv.generateRv() ? rv.state() : defRv();
    }
    virtual bool incomeMarkers(OdTrVisStateBranchPropertyDef::BranchType brType, OdTrVisStateBranchId branchId)
    { RvFlag rv;
      if (hasRedirectionA())
        rv.add(getRedirectionA()->incomeMarkers(brType, branchId));
      if (hasRedirectionB() && rv.state())
        rv.add(getRedirectionB()->incomeMarkers(brType, branchId));
      return rv.generateRv() ? rv.state() : defRv();
    }
    virtual bool incomeDrawOrder(const double* pDrawOrder)
    { RvFlag rv;
      if (hasRedirectionA())
        rv.add(getRedirectionA()->incomeDrawOrder(pDrawOrder));
      if (hasRedirectionB() && rv.state())
        rv.add(getRedirectionB()->incomeDrawOrder(pDrawOrder));
      return rv.generateRv() ? rv.state() : defRv();
    }
    virtual bool incomeExploded(bool bExploded)
    { RvFlag rv;
      if (hasRedirectionA())
        rv.add(getRedirectionA()->incomeExploded(bExploded));
      if (hasRedirectionB() && rv.state())
        rv.add(getRedirectionB()->incomeExploded(bExploded));
      return rv.generateRv() ? rv.state() : defRv();
    }
    virtual bool incomeSelectionStyle(OdUInt32 nStyle)
    { RvFlag rv;
      if (hasRedirectionA())
        rv.add(getRedirectionA()->incomeSelectionStyle(nStyle));
      if (hasRedirectionB() && rv.state())
        rv.add(getRedirectionB()->incomeSelectionStyle(nStyle));
      return rv.generateRv() ? rv.state() : defRv();
    }
    virtual bool incomeSelectionID(OdTrVisId id)
    { RvFlag rv;
      if (hasRedirectionA())
        rv.add(getRedirectionA()->incomeSelectionID(id));
      if (hasRedirectionB() && rv.state())
        rv.add(getRedirectionB()->incomeSelectionID(id));
      return rv.generateRv() ? rv.state() : defRv();
    }
};

/** \details
    <group ExRender_Windows_Classes> 
*/
class OdTrVisDirectRenderPartialUpdateInvocation : public OdRxObject
{
  protected:
    OdTrVisViewportId m_viewportId;
    OdTrVisOverlayId  m_overlayId;
  public:
    OdTrVisDirectRenderPartialUpdateInvocation()
      : m_viewportId(kTrVisNullId)
      , m_overlayId(kTrVisNullId)
    { }

    static OdSmartPtr<OdTrVisDirectRenderPartialUpdateInvocation> createObject()
    { return OdRxObjectImpl<OdTrVisDirectRenderPartialUpdateInvocation>::createObject(); }
    static OdSmartPtr<OdTrVisDirectRenderPartialUpdateInvocation> createObject(OdTrVisViewportId viewportId, OdTrVisOverlayId overlayId)
    { OdSmartPtr<OdTrVisDirectRenderPartialUpdateInvocation> pObj = OdRxObjectImpl<OdTrVisDirectRenderPartialUpdateInvocation>::createObject();
      pObj->setup(viewportId, overlayId);
      return pObj;
    }

    void setup(OdTrVisViewportId viewportId, OdTrVisOverlayId overlayId)
    { m_viewportId = viewportId; m_overlayId = overlayId; }
    void setup(const OdTrVisDirectRenderPartialUpdateInvocation *pInvocation)
    { if (pInvocation) { setup(pInvocation->viewportId(), pInvocation->overlayId()); } }

    // Adds ability to customize OdTrVisRenderDataInvoke interface by invocation.
    virtual OdTrVisRenderDataInvoke *wrapInvoke(OdTrVisRenderDataInvokeHid *pInvoke) const
    { return static_cast<OdTrVisRenderDataInvoke*>(pInvoke); }
    virtual const OdTrVisDirectRenderPartialUpdateInvocation *basicInvocation() const { return this; }

    OdTrVisViewportId viewportId() const { return m_viewportId; }
    OdTrVisOverlayId  overlayId()  const { return m_overlayId; }
};

typedef OdSmartPtr<OdTrVisDirectRenderPartialUpdateInvocation> OdTrVisDirectRenderPartialUpdateInvocationPtr;

/** \details
    <group ExRender_Windows_Classes> 
*/
class OdTrVisStaticDirectRenderPartialUpdateInvocation : public OdStaticRxObject<OdTrVisDirectRenderPartialUpdateInvocation>
{
  public:
    OdTrVisStaticDirectRenderPartialUpdateInvocation() {}
    OdTrVisStaticDirectRenderPartialUpdateInvocation(OdTrVisViewportId viewportId, OdTrVisOverlayId overlayId)
    { setup(viewportId, overlayId); }
    const OdTrVisDirectRenderPartialUpdateInvocation *getPtr() const { return this; }
    OdTrVisDirectRenderPartialUpdateInvocation *getPtr() { return this; }
};

/** \details
    <group ExRender_Windows_Classes> 
*/
class OdTrVisRenderDataDirectAccessor /*: public OdRxObject*/
{
  public:
    virtual bool collectRenderData(const OdTrVisDirectRenderPartialUpdateInvocation *pInvocation, OdTrVisRenderDataInvokeHid *pInvoke) = 0;
};

//typedef OdSmartPtr<OdTrVisRenderDataDirectAccessor> OdTrVisRenderDataDirectAccessorPtr;

#include "TD_PackPop.h"

#endif // ODTRVISRENDITIONPU
