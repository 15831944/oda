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
// TrGL2 renderer local host

#ifndef ODTRRNDLOCALRENDITIONHOST
#define ODTRRNDLOCALRENDITIONHOST

#include "TD_PackPush.h"

#include "TPtr.h"
#include "../TrVisRendition.h"
#include "TrRndLocalContext.h"
#include "TrRndStateBranchClient.h"

// Forward declaration
class OdTrRndRenderSettingsManager;
class OdTrRndLocalRenderer;

/** \details
    <group ExRender_Windows_Classes> 
*/
class OdTrRndLocalRenditionHost
{
  public:
    // Returns managed rendition
    virtual OdTrVisRendition *rendition() = 0;
    virtual const OdTrVisRendition *rendition() const = 0;

    // Returns render settings manager
    virtual OdTrRndRenderSettingsManager* renderSettingsManager() = 0;
    virtual const OdTrRndRenderSettingsManager* renderSettingsManager() const = 0;

    // Context data storage management
    virtual bool hasContextData() const = 0;
    virtual void processContextData(bool bStore) = 0;

    // Resource accessors
    virtual bool getMetafileDef(OdTrVisMetafileId metafileId, OdTrVisMetafileDef &mfDef) const = 0;
    virtual bool getMaterialDef(OdTrVisMaterialId materialId, OdTrVisMaterialDef &mtDef) const = 0;

    // Property accessors
    virtual OdUInt32 hasProperties(OdTrVisRendition::DataType dataType, OdTrVisId resId) const = 0;
    virtual bool getPropertyDef(OdTrVisRendition::DataType dataType, OdTrVisId resId, OdTrVisPropertyDef::PropertyType propType,
                                OdTrVisPropertyDef *&pPropDef) const = 0;
    // Ability to make shareable properties clone
    virtual bool supportShallowPropertiesClone(OdTrVisMetafileContainer *pMfStream) const = 0;
    virtual bool shallowPropertiesClone(OdTrVisMetafileContainer *pMfStream) = 0;
};

/** \details
    <group ExRender_Windows_Classes> 
*/
class OdTrRndLocalRenditionClient : public OdRxObject
{
  public:
    // Returns optional local context
    virtual OdTrRndLocalContextPtr createLocalContext(OdTrVisRenderClient * /*pDevice*/) { return OdTrRndLocalContextPtr(); }
    // Returns optional resource sharing provider (for multiple devices session sharing)
    virtual OdTrVisSharingProviderClientPtr createSharingProvider() { return OdTrVisSharingProviderClientPtr(); }
    // Returns optional highlighting and visibility branches interface (if true, rendition will be initialized as Gs-based)
    virtual OdTrRndStateBranchClientPtr stateBranchClient() { return OdTrRndStateBranchClientPtr(); }

    // Predefined textures generation
    virtual bool generateLinetypeTexture(OdPs::LineType /*ltpId*/, OdUInt8Array& /*outPixels*/, OdUInt8 /*forePixVal*/) const { return false; }
    virtual bool generateFilltypeTexture(OdPs::FillStyle /*fsId*/, OdUInt8Array& /*outPixels*/, OdUInt32 /*dimX*/, OdUInt32 /*dimY*/, OdUInt8 /*forePixVal*/) const { return false; }
};

/** \details
    <group ExRender_Windows_Classes>
*/
typedef OdSmartPtr<OdTrRndLocalRenditionClient> OdTrRndLocalRenditionClientPtr;

/** \details
    <group ExRender_Windows_Classes>
*/
class OdTrRndLocalRenditionHostTPtrAdaptor
{
  public:
    static void addRef(OdTrRndLocalRenditionHost* pObj)  { if (pObj) pObj->rendition()->addRef(); }
    static void release(OdTrRndLocalRenditionHost* pObj) { if (pObj) pObj->rendition()->release(); }
};

/** \details
    <group ExRender_Windows_Classes>
*/
class OdTrRndLocalRenditionHostPtr : public TPtr<OdTrRndLocalRenditionHost, OdTrRndLocalRenditionHostTPtrAdaptor>
{
  public:
    OdTrRndLocalRenditionHostPtr()
      : TPtr<OdTrRndLocalRenditionHost, OdTrRndLocalRenditionHostTPtrAdaptor>() {}
    OdTrRndLocalRenditionHostPtr(const OdTrRndLocalRenditionHostPtr& ref)
      : TPtr<OdTrRndLocalRenditionHost, OdTrRndLocalRenditionHostTPtrAdaptor>(ref) {}
    OdTrRndLocalRenditionHostPtr(const OdTrRndLocalRenditionHost* pObj, bool bAttach = false)
      : TPtr<OdTrRndLocalRenditionHost, OdTrRndLocalRenditionHostTPtrAdaptor>(pObj, bAttach) {}
    OdTrRndLocalRenditionHostPtr &operator =(OdTrRndLocalRenditionHost *pObj)
    { return static_cast<OdTrRndLocalRenditionHostPtr&>(TPtr<OdTrRndLocalRenditionHost, OdTrRndLocalRenditionHostTPtrAdaptor>::operator =(pObj)); }
    OdTrRndLocalRenditionHostPtr &operator =(const OdTrRndLocalRenditionHostPtr& ref)
    { return static_cast<OdTrRndLocalRenditionHostPtr&>(TPtr<OdTrRndLocalRenditionHost, OdTrRndLocalRenditionHostTPtrAdaptor>::operator =(ref)); }
    void release() { *this = TPtr<OdTrRndLocalRenditionHost, OdTrRndLocalRenditionHostTPtrAdaptor>::operator =(NULL); }
};

#include "TD_PackPop.h"

#endif // ODTRRNDLOCALRENDITIONHOST
