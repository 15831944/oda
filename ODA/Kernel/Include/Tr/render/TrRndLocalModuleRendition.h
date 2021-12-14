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
// Basic interface local module rendition's

#ifndef ODTRRNDLOCALMODULERENDITION
#define ODTRRNDLOCALMODULERENDITION

#include "TD_PackPush.h"

#include "TrRndLocalRenditionHost.h"
#include "TrRndRenderModule.h"

/** \details
    <group ExRender_Windows_Classes> 
*/
class OdTrRndLocalModuleRendition : public OdTrVisRenditionDef, public OdTrRndLocalRenditionHost
{
  protected:
    class LocalRenditionClient
    {
      protected:
        OdTrRndLocalRenditionClientPtr m_pClientContext;
        OdTrRndRenderModulePtr m_pRenderModule;
      public:
        LocalRenditionClient() {}
        void setup(OdTrRndLocalRenditionClient *pClientCtx, OdTrRndRenderModule *pRndModule)
        { m_pClientContext = pClientCtx; m_pRenderModule = pRndModule; }

        OdTrRndLocalRenditionClient *clientContext() { return m_pClientContext; }
        const OdTrRndLocalRenditionClient *clientContext() const { return m_pClientContext; }

        OdTrRndRenderModule *renderModule() { return m_pRenderModule; }
        const OdTrRndRenderModule *renderModule() const { return m_pRenderModule; }

        OdTrRndLocalContextPtr createLocalContext(OdTrVisRenderClient *pDevice)
        { OdTrRndLocalContextPtr pLC;
          if (!m_pClientContext.isNull())
            pLC = m_pClientContext->createLocalContext(pDevice);
          if (pLC.isNull() && !m_pRenderModule.isNull())
            pLC = m_pRenderModule->createLocalContext(pDevice);
          return pLC;
        }
    } m_clientContext;
  public:
    OdTrRndLocalModuleRendition() {}
    ~OdTrRndLocalModuleRendition() {}

    virtual void setupClientContext(OdTrRndLocalRenditionClient *pClient, OdTrRndRenderModule *pModule)
    {
      m_clientContext.setup(pClient, pModule);
    }
    OdTrRndLocalRenditionClient *clientContext() { return m_clientContext.clientContext(); }
    const OdTrRndLocalRenditionClient *clientContext() const { return m_clientContext.clientContext(); }

    // OdTrRndLocalRenditionHost interface

    // Returns managed rendition
    virtual OdTrVisRendition *rendition() { return this; }
    virtual const OdTrVisRendition *rendition() const { return this; }
  protected: // Stub other OdTrRndLocalRenditionHost methods
    // Returns render settings manager
    virtual OdTrRndRenderSettingsManager* renderSettingsManager() { ODA_FAIL(); return NULL; }
    virtual const OdTrRndRenderSettingsManager* renderSettingsManager() const { ODA_FAIL(); return NULL; }
    // Context data storage management
    virtual bool hasContextData() const { ODA_FAIL(); return false; }
    virtual void processContextData(bool /*bStore*/) { ODA_FAIL(); }
    // Resource accessors
    virtual bool getMetafileDef(OdTrVisMetafileId /*metafileId*/, OdTrVisMetafileDef & /*mfDef*/) const { ODA_FAIL(); return false; }
    virtual bool getMaterialDef(OdTrVisMaterialId /*materialId*/, OdTrVisMaterialDef & /*mtDef*/) const { ODA_FAIL(); return false; }
    // Property accessors
    virtual OdUInt32 hasProperties(OdTrVisRendition::DataType /*dataType*/, OdTrVisId /*resId*/) const { ODA_FAIL(); return 0; }
    virtual bool getPropertyDef(OdTrVisRendition::DataType /*dataType*/, OdTrVisId /*resId*/, OdTrVisPropertyDef::PropertyType /*propType*/,
                                OdTrVisPropertyDef *& /*pPropDef*/) const { ODA_FAIL(); return false; }
    // Ability to make shareable properties clone
    virtual bool supportShallowPropertiesClone(OdTrVisMetafileContainer * /*pMfStream*/) const { return false; }
    virtual bool shallowPropertiesClone(OdTrVisMetafileContainer * /*pMfStream*/) { ODA_FAIL(); return false; }
  protected:
    virtual bool generateLinetypeTexture(OdPs::LineType ltpId, OdUInt8Array &outPixels, OdUInt8 forePixVal) const
    {
      if (clientContext())
        return clientContext()->generateLinetypeTexture(ltpId, outPixels, forePixVal);
      return false;
    }
    virtual bool generateFilltypeTexture(OdPs::FillStyle fsId, OdUInt8Array &outPixels, OdUInt32 dimX, OdUInt32 dimY, OdUInt8 forePixVal) const
    {
      if (clientContext())
        return clientContext()->generateFilltypeTexture(fsId, outPixels, dimX, dimY, forePixVal);
      return false;
    }
};

#include "TD_PackPop.h"

#endif // ODTRRNDLOCALMODULERENDITION
