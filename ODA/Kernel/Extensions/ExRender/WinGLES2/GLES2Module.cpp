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
// GLES2Module.cpp : Defines the initialization routines for the DLL.

#include "OdaCommon.h"
#include "RxDispatchImpl.h"
#include "RxDynamicModule.h"
#include "DynamicLinker.h"
#include "Gs/GsBaseVectorizeDevice.h" // for OdSmartPtr<OdGsBaseVectorizeDevice>
#include "Gs/GsViewImpl.h"            // for OdSmartPtr<OdGsViewImpl>
#include "../vec/TrVecBaseModule.h"
#include "TrVecLocalRenditionGsClient.h"
#include "TrRndRenderModule.h"
#include "gl2/GLES2Include.h"

#if defined(_TOOLKIT_IN_DLL_) && defined(_MSC_VER)

extern "C" int APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID )
{
  switch ( dwReason )
  {
  case DLL_PROCESS_ATTACH:
    // remove this if you need per-thread initialization
    DisableThreadLibraryCalls( (HMODULE)hInstance );
    break;
  }
  return TRUE;
}

#define OD_GLES2_WIN_RESOURCES

#endif //_TOOLKIT_IN_DLL_

#ifdef OD_GLES2_WIN_RESOURCES
#include "win/ExGsGLES2RSTestDialog.h"
#endif // OD_GLES2_WIN_RESOURCES

// Base device for platform-dependent screen and bitmap devices
class OdGLES2PlatformBaseDevice : public OdTrVectorizerModuleHost
{
  protected:
    enum PropsFlags
    {
      kEnableDynamicVBO   = (1 << 0),
      kEnableStaticVBO    = (1 << 1),
      kCmbShaderStrategy  = (1 << 2),
      kEnableLwdPointOpt  = (1 << 3),
      kEnableLwdLineOpt   = (1 << 4),
      kEnableLwdCache     = (1 << 5),
      kLwdCachePriorGS    = (1 << 6),
      kLwdGeometryShader  = (1 << 7),
      kLtpGeometryShader  = (1 << 8),
      kColorModifyShader  = (1 << 9),
      kLwdDepthOffset     = (1 << 10),
      kEnableShadows      = (1 << 11),
      kEnableVSMShadows   = (1 << 12),
      kPointShadowCubeMap = (1 << 13),
      kPointShadowGS      = (1 << 14),
      kShadowsSmoothness  = (1 << 15),
      kEnableSSAO         = (1 << 16),
      kEnableFXAA         = (1 << 17),
      kEnableFXAA2d       = (1 << 18),
      kDepthBufferMerge   = (1 << 19),
      kDepthBufMergeDbg   = (1 << 20),
      kSSAODynamicRadius  = (1 << 21)
    };
  protected:
    mutable OdTrRndLocalRenditionClientPtr m_pRenditionClient;
    OdUInt32 m_nCheckVersion;
    OdUInt32 m_nPropFlags;
    OdUInt32 m_nStaticVBOLimit;
    OdUInt32 m_nLwdCacheLimit;
    OdUInt32 m_nBlendingMode;
    float    m_vsmShadowsAmount;
    OdUInt32 m_nShadowMapSoftness;
    OdUInt32 m_nShadowMapSize;
    float    m_aaLevel, m_aaLevelExt, m_aaHltLevel;
    float    m_ssaoFocus, m_ssaoPower;
    OdUInt32 m_ssaoLoops, m_ssaoBlurRadius;
    OdUInt32 m_fxaaQuality;
    OdUInt32 m_nGPUSelectionLevel;//See GPUSelection::Level
  public:
    // TrGL2-specific properties
    ODRX_DECLARE_PROPERTY_TRV(VBOLevel)
    ODRX_DECLARE_PROPERTY_TRV(StaticVBOLimit)
    ODRX_DECLARE_PROPERTY_TRV(CombinedShadersStrategy)
    ODRX_DECLARE_PROPERTY_TRV(LineweightOptimization)
    ODRX_DECLARE_PROPERTY_TRV(LineweightCacheLimit)
    ODRX_DECLARE_PROPERTY_TRV(GeometryShaderUsage)
    ODRX_DECLARE_PROPERTY_TRV(UseColorModifiersShader)
    ODRX_DECLARE_PROPERTY_TRV(LineweightDepthOffset)
    ODRX_DECLARE_PROPERTY_TRV(ShadowsRenderMode)
    ODRX_DECLARE_PROPERTY_TRV(VsmShadowsAmount)
    ODRX_DECLARE_PROPERTY_TRV(ShadowMapSoftnessOverride)
    ODRX_DECLARE_PROPERTY_TRV(ShadowMapSizeOverride)
    ODRX_DECLARE_PROPERTY_TRV(BlendingMode)
    ODRX_DECLARE_PROPERTY_TRV(DepthBufferMergeMode)
    ODRX_DECLARE_PROPERTY_TRV(AntiAliasLevel)
    ODRX_DECLARE_PROPERTY_TRV(AntiAliasLevelExt)
    ODRX_DECLARE_PROPERTY_TRV(AntiAliasHltLevelExt)
    ODRX_DECLARE_PROPERTY_TRV(SSAOEnable)
    ODRX_DECLARE_PROPERTY_TRV(SSAOLoops)
    ODRX_DECLARE_PROPERTY_TRV(SSAOFocus)
    ODRX_DECLARE_PROPERTY_TRV(SSAOPower)
    ODRX_DECLARE_PROPERTY_TRV(SSAOBlurRadius)
    ODRX_DECLARE_PROPERTY_TRV(SSAODynamicRadius)
    ODRX_DECLARE_PROPERTY_TRV(FXAAEnable)
    ODRX_DECLARE_PROPERTY_TRV(FXAAQuality)
    ODRX_DECLARE_PROPERTY_TRV(FXAA2dEnable)
    ODRX_DECLARE_PROPERTY_TRV(GPUSelectionEnable)
    ODRX_DECLARE_PROPERTY_TRV(GPUSelectionLevel)
    // Module-specific properties
    ODRX_DECLARE_PROPERTY_TRV(RenderSettings)
    ODRX_DECLARE_PROPERTY_TRV(SaveContextData)
    ODRX_DECLARE_PROPERTY_TRV(CheckRendererVersion)

    OdGLES2PlatformBaseDevice()
      : m_nCheckVersion(0)
      , m_nPropFlags(kEnableStaticVBO | kEnableLwdPointOpt | kEnableLwdCache | kLwdGeometryShader |
                     kLtpGeometryShader | kColorModifyShader | kLwdDepthOffset |
                     kEnableShadows | kEnableVSMShadows | kPointShadowCubeMap | kPointShadowGS | kShadowsSmoothness | 
                     kSSAODynamicRadius)
      , m_nStaticVBOLimit(256 * sizeof(float) * 3) // Allocate only for large vertex arrays
      , m_nLwdCacheLimit(16)
      , m_nBlendingMode(OdTrVisRenderClient::kBlendingSinglePassOIT)
      , m_vsmShadowsAmount(0.25f)
      , m_nShadowMapSoftness(3)
      , m_nShadowMapSize(0)
      , m_aaLevel(2.0f) // @@@tmp: m_aaLevel must be 0.0f by default?
      , m_aaLevelExt(0.25f)
      , m_aaHltLevel(0.075f)
      , m_ssaoFocus(1.0f)
      , m_ssaoPower(2.0f)
      , m_ssaoLoops(32)
      , m_ssaoBlurRadius(6)
      , m_fxaaQuality(4)
      , m_nGPUSelectionLevel(0)
    { }

    void setRenditionClient(OdTrRndLocalRenditionClient *pRenditionClient) { m_pRenditionClient = pRenditionClient; }
    OdTrVecLocalRenditionGsClient *renditionClient() const { return static_cast<OdTrVecLocalRenditionGsClient*>(m_pRenditionClient.get()); }

    OdTrVisRenditionPtr createRendition() { return renditionClient()->renderModule()->createRendition(m_pRenditionClient); }
    OdTrRndLocalRenditionHostPtr renditionHost() const
    { return renditionClient()->renderModule()->createRenditionHost(reinterpret_cast<OdTrVisRendition*>(renderClient()->getProperty(OD_T("Rendition"))->getIntPtr())); }

    virtual void configureForPlatform(OdTrVisRenderClient *pRenderClient, bool bOffScreen = false, bool bMobile = false, bool bDynamicVBO = false)
    {
      if (bMobile)
      {
        if (!bDynamicVBO)
          pRenderClient->setProperty(OD_T("ShareableDevice"), OdRxVariantValue(false));
        pRenderClient->setProperty(OD_T("LightSourcesLimit"), OdRxVariantValue(OdInt32(4)));
      }
      else
        pRenderClient->setProperty(OD_T("CombinedShadersStrategy"), OdRxVariantValue(true));
      pRenderClient->setProperty(OD_T("UseOverlays"), OdRxVariantValue(!bOffScreen));
      if (bDynamicVBO)
      {
        pRenderClient->setProperty(OD_T("VBOLevel"), OdRxVariantValue(OdUInt32(3)));
        pRenderClient->setProperty(OD_T("StaticVBOLimit"), OdRxVariantValue(OdUInt32(0)));
        pRenderClient->setProperty(OD_T("LineweightCacheLimit"), OdRxVariantValue(OdUInt32(0)));
      }
    }
    void configureForPlatform(OdTrVisRenderClient *pRenderClient, DeviceSetupType devType)
    { configureForPlatform(pRenderClient, devType == kDtOffScreen); }

    void setVBOLevel(OdUInt32 nLevel)
    {
      SETBIT(m_nPropFlags, kEnableDynamicVBO, GETBIT(nLevel, 1));
      SETBIT(m_nPropFlags, kEnableStaticVBO, GETBIT(nLevel, 2));
    }
    OdUInt32 VBOLevel() const
    {
      return (GETBIT(m_nPropFlags, kEnableDynamicVBO) ? 1 : 0) | (GETBIT(m_nPropFlags, kEnableStaticVBO) ? 2 : 0);
    }
    void setStaticVBOLimit(OdUInt32 nLimit)
    {
      m_nStaticVBOLimit = nLimit;
    }
    OdUInt32 staticVBOLimit() const
    {
      return m_nStaticVBOLimit;
    }

    void setCombinedShadersStrategy(bool bSet)
    {
      SETBIT(m_nPropFlags, kCmbShaderStrategy, bSet);
    }
    bool isCombinedShadersStrategyEnabled() const
    {
      return GETBIT(m_nPropFlags, kCmbShaderStrategy);
    }

    void setLineweightOptimizationLevel(OdUInt32 nLevel)
    {
      SETBIT(m_nPropFlags, kEnableLwdPointOpt, GETBIT(nLevel, 1));
      SETBIT(m_nPropFlags, kEnableLwdLineOpt , GETBIT(nLevel, 2));
      SETBIT(m_nPropFlags, kEnableLwdCache   , GETBIT(nLevel, 4));
      SETBIT(m_nPropFlags, kLwdCachePriorGS  , GETBIT(nLevel, 8));
    }
    OdUInt32 lineweightOptimizationLevel() const
    {
      return (GETBIT(m_nPropFlags, kEnableLwdPointOpt) ? 1 : 0) |
             (GETBIT(m_nPropFlags, kEnableLwdLineOpt)  ? 2 : 0) |
             (GETBIT(m_nPropFlags, kEnableLwdCache)    ? 4 : 0) |
             (GETBIT(m_nPropFlags, kLwdCachePriorGS)   ? 8 : 0);
    }

    void setLineweightCacheLimit(OdUInt32 nLimit)
    {
      m_nLwdCacheLimit = nLimit;
    }
    OdUInt32 lineweightCacheLimit() const
    {
      return m_nLwdCacheLimit;
    }

    void setGeometryShaderUsageLevel(OdUInt32 nLevel)
    {
      SETBIT(m_nPropFlags, kLwdGeometryShader, GETBIT(nLevel, 1));
      SETBIT(m_nPropFlags, kLtpGeometryShader, GETBIT(nLevel, 2));
    }
    OdUInt32 geometryShaderUsageLevel() const
    {
      return (GETBIT(m_nPropFlags, kLwdGeometryShader) ? 1 : 0) | (GETBIT(m_nPropFlags, kLtpGeometryShader) ? 2 : 0);
    }

    void setUseColorModifiersShader(bool bSet)
    {
      SETBIT(m_nPropFlags, kColorModifyShader, bSet);
    }
    bool useColorModifiersShader() const
    {
      return GETBIT(m_nPropFlags, kColorModifyShader);
    }

    void setLineweightDepthOffset(bool bSet)
    {
      SETBIT(m_nPropFlags, kLwdDepthOffset, bSet);
    }
    bool useLineweightDepthOffset() const
    {
      return GETBIT(m_nPropFlags, kLwdDepthOffset);
    }

    void setShadowsRenderMode(OdUInt32 nMode)
    {
      SETBIT(m_nPropFlags, kEnableShadows     , GETBIT(nMode,  1));
      SETBIT(m_nPropFlags, kEnableVSMShadows  , GETBIT(nMode,  2));
      SETBIT(m_nPropFlags, kPointShadowCubeMap, GETBIT(nMode,  4));
      SETBIT(m_nPropFlags, kPointShadowGS     , GETBIT(nMode,  8));
      SETBIT(m_nPropFlags, kShadowsSmoothness , GETBIT(nMode, 16));
    }
    OdUInt32 shadowsRenderMode() const
    {
      return (GETBIT(m_nPropFlags, kEnableShadows)      ?  1 : 0) |
             (GETBIT(m_nPropFlags, kEnableVSMShadows)   ?  2 : 0) |
             (GETBIT(m_nPropFlags, kPointShadowCubeMap) ?  4 : 0) |
             (GETBIT(m_nPropFlags, kPointShadowGS)      ?  8 : 0) |
             (GETBIT(m_nPropFlags, kShadowsSmoothness)  ? 16 : 0);
    }

    void setVsmShadowsAmount(double fAmount)
    {
      m_vsmShadowsAmount = (float)fAmount;
    }
    double vsmShadowsAmount() const
    {
      return m_vsmShadowsAmount;
    }

    void setShadowMapSoftness(OdUInt32 nSoftness)
    {
      m_nShadowMapSoftness = nSoftness;
    }
    OdUInt32 shadowMapSoftnessOverride() const
    {
      return m_nShadowMapSoftness;
    }

    void setShadowMapSize(OdUInt32 nSize)
    {
      m_nShadowMapSize = nSize;
    }
    OdUInt32 shadowMapSizeOverride() const
    {
      return m_nShadowMapSize;
    }

    void setBlendingMode(OdUInt32 nBlendingMode)
    {
      m_nBlendingMode = nBlendingMode;
    }
    OdUInt32 blendingMode() const
    {
      return m_nBlendingMode;
    }

    void setDepthBufferMergeMode(OdUInt32 nDepthBufMergeMode)
    {
      SETBIT(m_nPropFlags, kDepthBufferMerge, GETBIT(nDepthBufMergeMode, 1));
      SETBIT(m_nPropFlags, kDepthBufMergeDbg, GETBIT(nDepthBufMergeMode, 2));
    }
    OdUInt32 depthBufferMergeMode() const
    {
      return (GETBIT(m_nPropFlags, kDepthBufMergeDbg) ? 2 : (GETBIT(m_nPropFlags, kDepthBufferMerge) ? 1 : 0));
    }

    void setAntiAliasLevel(double aaLevel) { m_aaLevel = (float)aaLevel; }
    float antiAliasLevel() const { return m_aaLevel; }
    void setAntiAliasLevelExt(double aaLevel) { m_aaLevelExt = (float)aaLevel; }
    float antiAliasLevelExt() const { return m_aaLevelExt; }
    void setAntiAliasHltLevelExt(double aaLevel) { m_aaHltLevel = (float)aaLevel; }
    float antiAliasHltLevelExt() const { return m_aaHltLevel; }

    void setSSAOEnable(bool bEnable) { SETBIT(m_nPropFlags, kEnableSSAO, bEnable); }
    bool ssaoEnabled() const { return GETBIT(m_nPropFlags, kEnableSSAO); }
    void setSSAOLoops(OdUInt32 ssaoLoops) { m_ssaoLoops = ssaoLoops; }
    OdUInt32 ssaoLoops() const { return m_ssaoLoops; }
    void setSSAOFocus(double ssaoFocus) { m_ssaoFocus = (float)ssaoFocus; }
    float ssaoFocus() const { return m_ssaoFocus; }
    void setSSAOPower(double ssaoPower) { m_ssaoPower = (float)ssaoPower; }
    float ssaoPower() const { return m_ssaoPower; }

    void setSSAOBlurRadius(OdUInt32 ssaoBlurRadius) { m_ssaoBlurRadius = ssaoBlurRadius; }
    OdUInt32 ssaoBlurRadius() const { return m_ssaoBlurRadius; }
    void setSSAODynamicRadius(bool ssaoDynamicRadius) { SETBIT(m_nPropFlags, kSSAODynamicRadius, ssaoDynamicRadius); }
    bool ssaoDynamicRadius() const { return GETBIT(m_nPropFlags, kSSAODynamicRadius); }

    void setFxAAEnable(bool bEnable) { SETBIT(m_nPropFlags, kEnableFXAA, bEnable); }
    bool fxaaEnabled() const { return GETBIT(m_nPropFlags, kEnableFXAA); }
    void setFxAA2dEnable(bool bEnable) { SETBIT(m_nPropFlags, kEnableFXAA2d, bEnable); }
    bool fxaa2dEnabled() const { return GETBIT(m_nPropFlags, kEnableFXAA2d); }

    void setFXAAQuality(OdUInt32 nQuality) { m_fxaaQuality = odmin(nQuality, 5); }
    OdUInt32 fxaaQuality() const { return m_fxaaQuality; }

    void setGPUSelectionEnable(bool bEnable) { m_nGPUSelectionLevel = bEnable ? 1 : 0; }
    bool gpuSelectionEnabled() const { return m_nGPUSelectionLevel > 0; }

    void setGPUSelectionLevel(OdUInt32 nLevel) { m_nGPUSelectionLevel = odmin(nLevel, 3); }
    OdUInt32 gpuSelectionLevel() const { return m_nGPUSelectionLevel; }

    void put_RenderSettings(OdIntPtr) { }
    OdIntPtr get_RenderSettings() const { return (OdIntPtr)renditionHost()->renderSettingsManager(); }

    void put_SaveContextData(bool bSet) { renditionHost()->processContextData(bSet); }
    bool get_SaveContextData() const { return renditionHost()->hasContextData(); }

    void put_CheckRendererVersion(OdUInt32 nCheckVersion) { m_nCheckVersion = nCheckVersion; }
    OdUInt32 get_CheckRendererVersion() const { return renditionClient()->renderModule()->checkRendererVersion(renderClient(), m_nCheckVersion); }

    void generateProperties(OdRxDictionary* pInfo) const
    {
      // TrGL2-specific properties
      ODRX_GENERATE_PROPERTY_TRV(VBOLevel)
      ODRX_GENERATE_PROPERTY_TRV(StaticVBOLimit)
      ODRX_GENERATE_PROPERTY_TRV(CombinedShadersStrategy)
      ODRX_GENERATE_PROPERTY_TRV(LineweightOptimization)
      ODRX_GENERATE_PROPERTY_TRV(LineweightCacheLimit)
      ODRX_GENERATE_PROPERTY_TRV(GeometryShaderUsage)
      ODRX_GENERATE_PROPERTY_TRV(UseColorModifiersShader)
      ODRX_GENERATE_PROPERTY_TRV(LineweightDepthOffset)
      ODRX_GENERATE_PROPERTY_TRV(ShadowsRenderMode)
      ODRX_GENERATE_PROPERTY_TRV(VsmShadowsAmount)
      ODRX_GENERATE_PROPERTY_TRV(ShadowMapSoftnessOverride)
      ODRX_GENERATE_PROPERTY_TRV(ShadowMapSizeOverride)
      ODRX_GENERATE_PROPERTY_TRV(BlendingMode)
      ODRX_GENERATE_PROPERTY_TRV(DepthBufferMergeMode)
      ODRX_GENERATE_PROPERTY_TRV(AntiAliasLevel)
      ODRX_GENERATE_PROPERTY_TRV(AntiAliasLevelExt)
      ODRX_GENERATE_PROPERTY_TRV(AntiAliasHltLevelExt)
      ODRX_GENERATE_PROPERTY_TRV(SSAOEnable)
      ODRX_GENERATE_PROPERTY_TRV(SSAOLoops)
      ODRX_GENERATE_PROPERTY_TRV(SSAOFocus)
      ODRX_GENERATE_PROPERTY_TRV(SSAOPower)
      ODRX_GENERATE_PROPERTY_TRV(SSAOBlurRadius)
      ODRX_GENERATE_PROPERTY_TRV(SSAODynamicRadius)
      ODRX_GENERATE_PROPERTY_TRV(FXAAEnable)
      ODRX_GENERATE_PROPERTY_TRV(FXAAQuality)
      ODRX_GENERATE_PROPERTY_TRV(FXAA2dEnable)
      ODRX_GENERATE_PROPERTY_TRV(GPUSelectionEnable)
      ODRX_GENERATE_PROPERTY_TRV(GPUSelectionLevel)
      // Module-specific properties
      ODRX_GENERATE_PROPERTY_TRV(RenderSettings)
      ODRX_GENERATE_PROPERTY_TRV(SaveContextData)
      ODRX_GENERATE_PROPERTY_TRV(CheckRendererVersion)
    }
};

// TrGL2-specific properties
ODRX_DEFINE_PROPERTY_METHODS_PREFIX_TRV(OdGLES2PlatformBaseDevice::, VBOLevel                 , OdGLES2PlatformBaseDevice, VBOLevel                        , setVBOLevel                   , getUInt32);
ODRX_DEFINE_PROPERTY_METHODS_PREFIX_TRV(OdGLES2PlatformBaseDevice::, StaticVBOLimit           , OdGLES2PlatformBaseDevice, staticVBOLimit                  , setStaticVBOLimit             , getUInt32);
ODRX_DEFINE_PROPERTY_METHODS_PREFIX_TRV(OdGLES2PlatformBaseDevice::, CombinedShadersStrategy  , OdGLES2PlatformBaseDevice, isCombinedShadersStrategyEnabled, setCombinedShadersStrategy    , getBool);
ODRX_DEFINE_PROPERTY_METHODS_PREFIX_TRV(OdGLES2PlatformBaseDevice::, LineweightOptimization   , OdGLES2PlatformBaseDevice, lineweightOptimizationLevel     , setLineweightOptimizationLevel, getUInt32);
ODRX_DEFINE_PROPERTY_METHODS_PREFIX_TRV(OdGLES2PlatformBaseDevice::, LineweightCacheLimit     , OdGLES2PlatformBaseDevice, lineweightCacheLimit            , setLineweightCacheLimit       , getUInt32);
ODRX_DEFINE_PROPERTY_METHODS_PREFIX_TRV(OdGLES2PlatformBaseDevice::, GeometryShaderUsage      , OdGLES2PlatformBaseDevice, geometryShaderUsageLevel        , setGeometryShaderUsageLevel   , getUInt32);
ODRX_DEFINE_PROPERTY_METHODS_PREFIX_TRV(OdGLES2PlatformBaseDevice::, UseColorModifiersShader  , OdGLES2PlatformBaseDevice, useColorModifiersShader         , setUseColorModifiersShader    , getBool);
ODRX_DEFINE_PROPERTY_METHODS_PREFIX_TRV(OdGLES2PlatformBaseDevice::, LineweightDepthOffset    , OdGLES2PlatformBaseDevice, useLineweightDepthOffset        , setLineweightDepthOffset      , getBool);
ODRX_DEFINE_PROPERTY_METHODS_PREFIX_TRV(OdGLES2PlatformBaseDevice::, ShadowsRenderMode        , OdGLES2PlatformBaseDevice, shadowsRenderMode               , setShadowsRenderMode          , getUInt32);
ODRX_DEFINE_PROPERTY_METHODS_PREFIX_TRV(OdGLES2PlatformBaseDevice::, VsmShadowsAmount         , OdGLES2PlatformBaseDevice, vsmShadowsAmount                , setVsmShadowsAmount           , getDouble);
ODRX_DEFINE_PROPERTY_METHODS_PREFIX_TRV(OdGLES2PlatformBaseDevice::, ShadowMapSoftnessOverride, OdGLES2PlatformBaseDevice, shadowMapSoftnessOverride       , setShadowMapSoftness          , getUInt32);
ODRX_DEFINE_PROPERTY_METHODS_PREFIX_TRV(OdGLES2PlatformBaseDevice::, ShadowMapSizeOverride    , OdGLES2PlatformBaseDevice, shadowMapSizeOverride           , setShadowMapSize              , getUInt32);
ODRX_DEFINE_PROPERTY_METHODS_PREFIX_TRV(OdGLES2PlatformBaseDevice::, BlendingMode             , OdGLES2PlatformBaseDevice, blendingMode                    , setBlendingMode               , getUInt32);
ODRX_DEFINE_PROPERTY_METHODS_PREFIX_TRV(OdGLES2PlatformBaseDevice::, DepthBufferMergeMode     , OdGLES2PlatformBaseDevice, depthBufferMergeMode            , setDepthBufferMergeMode       , getUInt32);
ODRX_DEFINE_PROPERTY_METHODS_PREFIX_TRV(OdGLES2PlatformBaseDevice::, AntiAliasLevel           , OdGLES2PlatformBaseDevice, antiAliasLevel                  , setAntiAliasLevel             , getDouble);
ODRX_DEFINE_PROPERTY_METHODS_PREFIX_TRV(OdGLES2PlatformBaseDevice::, AntiAliasLevelExt        , OdGLES2PlatformBaseDevice, antiAliasLevelExt               , setAntiAliasLevelExt          , getDouble);
ODRX_DEFINE_PROPERTY_METHODS_PREFIX_TRV(OdGLES2PlatformBaseDevice::, AntiAliasHltLevelExt     , OdGLES2PlatformBaseDevice, antiAliasHltLevelExt            , setAntiAliasHltLevelExt       , getDouble);
ODRX_DEFINE_PROPERTY_METHODS_PREFIX_TRV(OdGLES2PlatformBaseDevice::, SSAOEnable               , OdGLES2PlatformBaseDevice, ssaoEnabled                     , setSSAOEnable                 , getBool);
ODRX_DEFINE_PROPERTY_METHODS_PREFIX_TRV(OdGLES2PlatformBaseDevice::, SSAOLoops                , OdGLES2PlatformBaseDevice, ssaoLoops                       , setSSAOLoops                  , getUInt32);
ODRX_DEFINE_PROPERTY_METHODS_PREFIX_TRV(OdGLES2PlatformBaseDevice::, SSAOFocus                , OdGLES2PlatformBaseDevice, ssaoFocus                       , setSSAOFocus                  , getDouble);
ODRX_DEFINE_PROPERTY_METHODS_PREFIX_TRV(OdGLES2PlatformBaseDevice::, SSAOPower                , OdGLES2PlatformBaseDevice, ssaoPower                       , setSSAOPower                  , getDouble);
ODRX_DEFINE_PROPERTY_METHODS_PREFIX_TRV(OdGLES2PlatformBaseDevice::, SSAOBlurRadius           , OdGLES2PlatformBaseDevice, ssaoBlurRadius                  , setSSAOBlurRadius             , getUInt32);
ODRX_DEFINE_PROPERTY_METHODS_PREFIX_TRV(OdGLES2PlatformBaseDevice::, SSAODynamicRadius        , OdGLES2PlatformBaseDevice, ssaoDynamicRadius               , setSSAODynamicRadius          , getBool);
ODRX_DEFINE_PROPERTY_METHODS_PREFIX_TRV(OdGLES2PlatformBaseDevice::, FXAAEnable               , OdGLES2PlatformBaseDevice, fxaaEnabled                     , setFxAAEnable                 , getBool);
ODRX_DEFINE_PROPERTY_METHODS_PREFIX_TRV(OdGLES2PlatformBaseDevice::, FXAAQuality              , OdGLES2PlatformBaseDevice, fxaaQuality                     , setFXAAQuality                , getUInt32);
ODRX_DEFINE_PROPERTY_METHODS_PREFIX_TRV(OdGLES2PlatformBaseDevice::, FXAA2dEnable             , OdGLES2PlatformBaseDevice, fxaa2dEnabled                   , setFxAA2dEnable               , getBool);
ODRX_DEFINE_PROPERTY_METHODS_PREFIX_TRV(OdGLES2PlatformBaseDevice::, GPUSelectionEnable       , OdGLES2PlatformBaseDevice, gpuSelectionEnabled             , setGPUSelectionEnable         , getBool);
ODRX_DEFINE_PROPERTY_METHODS_PREFIX_TRV(OdGLES2PlatformBaseDevice::, GPUSelectionLevel        , OdGLES2PlatformBaseDevice, gpuSelectionLevel               , setGPUSelectionLevel          , getUInt32);
// Module-specific properties
ODRX_DEFINE_PROPERTY_PREFIX_TRV(OdGLES2PlatformBaseDevice::, RenderSettings      , OdGLES2PlatformBaseDevice, getIntPtr)
ODRX_DEFINE_PROPERTY_PREFIX_TRV(OdGLES2PlatformBaseDevice::, SaveContextData     , OdGLES2PlatformBaseDevice, getBool)
ODRX_DEFINE_PROPERTY_PREFIX_TRV(OdGLES2PlatformBaseDevice::, CheckRendererVersion, OdGLES2PlatformBaseDevice, getUInt32)

// Base device for platform-dependent screen devices
class _OdPlatformGLES2Device : public OdGLES2PlatformBaseDevice
{
  protected:
    bool m_bCreateContext;
  public:
    ODRX_DECLARE_PROPERTY_TRV(CreateContext)

    _OdPlatformGLES2Device() : m_bCreateContext(true) {
    }

    void put_CreateContext(bool bSet) { m_bCreateContext = bSet; }
    bool get_CreateContext() const { return m_bCreateContext; }

    DeviceSetupType deviceSetupOverride() const { return kDtOnScreen; }

    void generateProperties(OdRxDictionary* pInfo) const
    {
      ODRX_INHERIT_PROPERTIES_TRV(OdGLES2PlatformBaseDevice);
      ODRX_GENERATE_PROPERTY_TRV(CreateContext)
    }
};

ODRX_DEFINE_PROPERTY_PREFIX_TRV(_OdPlatformGLES2Device::, CreateContext , _OdPlatformGLES2Device, getBool)

#if defined(TD_USE_QT_LIB)

/* Qt-based cross-platform device implementation */

#include <QGLWidget>
#if QT_VERSION >= QT_VERSION_CHECK(5,4,0)
#include <QOpenGLWidget>
#include <QOpenGLContext>
#endif
#include <QPointer>
#include <QObject>
#include <QApplication>
 
class OdPlatformGLES2Device : public _OdPlatformGLES2Device
{
  protected:
    QPointer<QWidget> m_pQWidget; // QGLWidget or QOpenGLWidge since Qt 5.4
#ifdef OD_GLES2_WIN_RESOURCES
    OdGLES2RSTestDialog m_rsTestDlg;
#endif // OD_GLES2_WIN_RESOURCES

  public:
    ODRX_DECLARE_PROPERTY_TRV(QGLWidget)
#ifdef OD_GLES2_WIN_RESOURCES
    ODRX_DECLARE_PROPERTY_TRV(RenderSettingsDlg)
#endif // OD_GLES2_WIN_RESOURCES

    OdPlatformGLES2Device() { }
#ifdef OD_GLES2_WIN_RESOURCES
    ~OdPlatformGLES2Device()
    {
      m_rsTestDlg.destroyDialogWindow();
    }
    void put_RenderSettingsDlg(OdIntPtr hParentWnd)
    {
      if (!m_rsTestDlg.isDialogPresent())
      {
        QWidget* pWidget = qApp->activeWindow(); // QMainWindow
        HWND hwnd = (HWND) pWidget->winId();
        m_rsTestDlg.createDialogWindow((HWND)hParentWnd, (HWND) hwnd,
                                       (OdTrRndRenderSettingsManager*)get_RenderSettings());
      }
      else if (!hParentWnd)
        m_rsTestDlg.destroyDialogWindow();
    }
    OdIntPtr get_RenderSettingsDlg() const
    {
      if (m_rsTestDlg.isDialogPresent())
        return 1;
      return 0;
    }
#endif // OD_GLES2_WIN_RESOURCES
    QWidget* put_QGLWidget(QWidget* pQWidget)
    {
      if (pQWidget == m_pQWidget)
        return pQWidget;
      QWidget* pQWidgetPrev = m_pQWidget;
      m_pQWidget = pQWidget;
      return pQWidgetPrev;
    }

    OdIntPtr put_QGLWidget(OdIntPtr ptrQWidget)
    {
      QWidget* pQWidget = qobject_cast<QWidget*>((QObject*) ptrQWidget);
      return (OdIntPtr)(QObject*) put_QGLWidget(pQWidget);
    }

    OdIntPtr get_QGLWidget() const
    {
      return (OdIntPtr)(QObject*)m_pQWidget;
    }

#if QT_VERSION >= QT_VERSION_CHECK(5,4,0)
    QOpenGLWidget* getQOpenGLWidget() const
    {
      return qobject_cast<QOpenGLWidget*>(m_pQWidget);
    }
#endif
    QGLWidget* getQGLWidget() const
    {
      return qobject_cast<QGLWidget*>(m_pQWidget);
    }
    static bool makeCurrentContext(QWidget* pQWidget, bool checkIsValid = true)
    {
#if QT_VERSION >= QT_VERSION_CHECK(5,4,0)
      if (QOpenGLWidget* pQOpenGLWidget = qobject_cast<QOpenGLWidget*>(pQWidget))
      {
        if (checkIsValid && !pQOpenGLWidget->isValid())
          return false;
        pQOpenGLWidget->makeCurrent();
      }
      else
#endif
      if (QGLWidget* pQGLWidget = qobject_cast<QGLWidget*>(pQWidget))
      {
        if (checkIsValid && !pQGLWidget->isValid())
          return false;
        pQGLWidget->makeCurrent();
      }
      else
        return false;
      return true;
    }

    void generateProperties(OdRxDictionary* pInfo) const
    {
      ODRX_INHERIT_PROPERTIES_TRV(_OdPlatformGLES2Device);
      ODRX_GENERATE_PROPERTY_TRV(QGLWidget)
#ifdef OD_GLES2_WIN_RESOURCES
      ODRX_GENERATE_PROPERTY_TRV(RenderSettingsDlg)
#endif // OD_GLES2_WIN_RESOURCES
    }

    void configureForPlatform(OdTrVisRenderClient *pRenderClient, bool bOffScreen, bool bMobile, bool bDynamicVBO)
    {
#if defined(ANDROID) || defined(__IPHONE_OS_VERSION_MIN_REQUIRED)
      bMobile = true;
#endif
      _OdPlatformGLES2Device::configureForPlatform(pRenderClient, bOffScreen, bMobile, bDynamicVBO);
    }
};

ODRX_DEFINE_PROPERTY_PREFIX_TRV(OdPlatformGLES2Device::, QGLWidget, OdPlatformGLES2Device, getIntPtr)
#ifdef OD_GLES2_WIN_RESOURCES
ODRX_DEFINE_PROPERTY_PREFIX_TRV(OdPlatformGLES2Device::, RenderSettingsDlg, OdPlatformGLES2Device, getIntPtr)
#endif // OD_GLES2_WIN_RESOURCES

#if QT_VERSION >= QT_VERSION_CHECK(5,4,0)
class OdqOpenGLOffscreenGles : public QOpenGLWidget
{
  protected:
    OdTrVectorizerUpdateHost *m_pCb;
    OdGsDCRect *m_pRect;

    void paintGL()
    {
      m_pCb->hostUpdate(m_pRect);
    }

  public:
    explicit OdqOpenGLOffscreenGles(const QSurfaceFormat& format, Qt::WindowFlags f = 0)
      : QOpenGLWidget(NULL, f)
      , m_pCb(NULL)
      , m_pRect(NULL)
    {
      setFormat(format);
    }

  public:
    QPixmap renderPixmap(OdTrVectorizerUpdateHost *pCb,
                         OdTrVisRenderClient *pClient,
                         OdGsDCRect *pRect)
    {
      //m_pCb = pCb;
      //m_pRect = pRect;
      //return QOpenGLWidget::renderPixmap(pClient->outputWindowWidth(), pClient->outputWindowHeight()); //, true);
      renderToImage(pCb, pClient, pRect);
      QImage img = grabFramebuffer();
      ODA_ASSERT_VAR(int w_ = img.width();
                     int w = pClient->outputWindowWidth();
                     int h_ = img.height();
                     int h = pClient->outputWindowHeight();)
      ODA_ASSERT_ONCE(w == w_ && h == h_);
      if (false)
        img.save("e:\\_Oda\\data\\_print_plot\\SUP-926 Whiteprint\\grab.bmp", "BMP");
      return QPixmap::fromImage(img);
    }

    void renderToImage(OdTrVectorizerUpdateHost *pCb,
                       OdTrVisRenderClient *pClient,
                       OdGsDCRect *pRect)
    {
      m_pCb = pCb;
      m_pRect = pRect;

      makeCurrent();
      //QGLContext* pContext = const_cast<QGLContext*>(context());
      QOpenGLContext* pContext = context();

      ODA_ASSERT_ONCE(pContext && pContext->isValid());
      if (pContext)
        pContext->create();

      makeCurrent();
      initializeGL();
      int w = pClient->outputWindowWidth(),
          h = pClient->outputWindowHeight();
      resizeGL(w, h);
      paintGL();
//#  ifdef _DEBUG
//    QImage img = grabFrameBuffer();
//    int w_ = img.width();
//    int h_ = img.height();
//    ODA_ASSERT_ONCE(w == w_ && h == h_);
//    img.save("e:\\_Oda\\data\\_print_plot\\SUP-926 Whiteprint\\grab.bmp", "BMP");
//#  endif
    }

    virtual QSize sizeHint() const
    {
      if (m_pRect)
        return QOpenGLWidget::sizeHint();
      QSize sz(Od_abs(m_pRect->m_max.x - m_pRect->m_min.x),
               Od_abs(m_pRect->m_max.y - m_pRect->m_min.y));
      return sz;
    }
};
#endif

class OdqOffscreenGles : public QGLWidget
{
  protected:
    OdTrVectorizerUpdateHost *m_pCb;
    OdGsDCRect *m_pRect;

    void paintGL()
    {
      m_pCb->hostUpdate(m_pRect);
    }

  public:
    explicit OdqOffscreenGles(const QGLFormat& format, Qt::WindowFlags f = 0)
      : QGLWidget(format, 0, 0, f)
      , m_pCb(NULL)
      , m_pRect(NULL)
    {
    }

  public:
    QPixmap renderPixmap(OdTrVectorizerUpdateHost *pCb,
                         OdTrVisRenderClient *pClient,
                         OdGsDCRect *pRect)
    {
      m_pCb = pCb;
      m_pRect = pRect;
      return QGLWidget::renderPixmap(pClient->outputWindowWidth(), pClient->outputWindowHeight()); //, true);
    }

    void renderToImage(OdTrVectorizerUpdateHost *pCb,
                       OdTrVisRenderClient *pClient,
                       OdGsDCRect *pRect)
    {
      m_pCb = pCb;
      m_pRect = pRect;

      makeCurrent();
      QGLContext* pContext = const_cast<QGLContext*>(context());
      ODA_ASSERT_ONCE(pContext);
      if (pContext)
        pContext->create();

      makeCurrent();
      initializeGL();
      int w = pClient->outputWindowWidth(),
          h = pClient->outputWindowHeight();
      resizeGL(w, h);
      paintGL();
//#  ifdef _DEBUG
//    QImage img = grabFrameBuffer();
//    int w_ = img.width();
//    int h_ = img.height();
//    ODA_ASSERT_ONCE(w == w_ && h == h_);
//    img.save("e:\\_Oda\\data\\_print_plot\\SUP-926 Whiteprint\\grab.bmp", "BMP");
//#  endif
    }

    virtual QSize sizeHint() const
    {
      if (m_pRect)
        return QGLWidget::sizeHint();
      QSize sz(Od_abs(m_pRect->m_max.x - m_pRect->m_min.x),
               Od_abs(m_pRect->m_max.y - m_pRect->m_min.y));
      return sz;
    }
};

#elif defined(_WIN32)

/* Windows platform device implementation */

class OdPlatformGLES2Device : public _OdPlatformGLES2Device
{
  protected:
    OdIntPtr m_WindowHDC;
    OdIntPtr m_WindowHWND;
    OdIntPtr m_WGLContext;
#ifdef OD_GLES2_WIN_RESOURCES
    OdGLES2RSTestDialog m_rsTestDlg;
#endif // OD_GLES2_WIN_RESOURCES
  public:
    ODRX_DECLARE_PROPERTY_TRV(WindowHDC)
    ODRX_DECLARE_PROPERTY_TRV(WindowHWND)
    ODRX_DECLARE_PROPERTY_TRV(WGLContext)
#ifdef OD_GLES2_WIN_RESOURCES
    ODRX_DECLARE_PROPERTY_TRV(RenderSettingsDlg)
#endif // OD_GLES2_WIN_RESOURCES

    OdPlatformGLES2Device() : m_WindowHDC(NULL), m_WindowHWND(NULL), m_WGLContext(NULL) { }
    void put_WindowHDC(OdIntPtr hDc) { m_WindowHDC = hDc; }
    OdIntPtr get_WindowHDC() const { return m_WindowHDC; }
    void put_WindowHWND(OdIntPtr hWnd) { m_WindowHWND = hWnd; }
    OdIntPtr get_WindowHWND() const { return m_WindowHWND; }
    void put_WGLContext(OdIntPtr glCtx) { m_WGLContext = glCtx; }
    OdIntPtr get_WGLContext() const { return m_WGLContext; }
#ifdef OD_GLES2_WIN_RESOURCES
    ~OdPlatformGLES2Device()
    {
      m_rsTestDlg.destroyDialogWindow();
    }
    void put_RenderSettingsDlg(OdIntPtr hParentWnd)
    {
      if (!m_rsTestDlg.isDialogPresent())
        m_rsTestDlg.createDialogWindow((HWND)hParentWnd,
                                       (HWND)m_WindowHWND,
                                       (OdTrRndRenderSettingsManager*)get_RenderSettings());
    }
    OdIntPtr get_RenderSettingsDlg() const
    {
      if (m_rsTestDlg.isDialogPresent())
        return 1;
      return 0;
    }
#endif // OD_GLES2_WIN_RESOURCES

    void generateProperties(OdRxDictionary* pInfo) const
    {
      ODRX_INHERIT_PROPERTIES_TRV(_OdPlatformGLES2Device);
      ODRX_GENERATE_PROPERTY_TRV(WindowHDC)
      ODRX_GENERATE_PROPERTY_TRV(WindowHWND)
      ODRX_GENERATE_PROPERTY_TRV(WGLContext)
#ifdef OD_GLES2_WIN_RESOURCES
      ODRX_GENERATE_PROPERTY_TRV(RenderSettingsDlg)
#endif // OD_GLES2_WIN_RESOURCES
    }
};

ODRX_DEFINE_PROPERTY_PREFIX_TRV(OdPlatformGLES2Device::, WindowHDC        , OdPlatformGLES2Device, getIntPtr)
ODRX_DEFINE_PROPERTY_PREFIX_TRV(OdPlatformGLES2Device::, WindowHWND       , OdPlatformGLES2Device, getIntPtr)
ODRX_DEFINE_PROPERTY_PREFIX_TRV(OdPlatformGLES2Device::, WGLContext       , OdPlatformGLES2Device, getIntPtr)
#ifdef OD_GLES2_WIN_RESOURCES
ODRX_DEFINE_PROPERTY_PREFIX_TRV(OdPlatformGLES2Device::, RenderSettingsDlg, OdPlatformGLES2Device, getIntPtr)
#endif // OD_GLES2_WIN_RESOURCES

#elif defined(OD_TRGL2_IOS)

/* iOS platform device implementation */

class OdPlatformGLES2Device : public _OdPlatformGLES2Device
{
  protected:
    OdIntPtr m_EAGLLayer;
    OdIntPtr m_EAGLContext;
  public:
    ODRX_DECLARE_PROPERTY_TRV(EAGLLayer)
    ODRX_DECLARE_PROPERTY_TRV(EAGLContext)

    OdPlatformGLES2Device() : m_EAGLLayer(NULL), m_EAGLContext(NULL) { }
    void put_EAGLLayer(OdIntPtr pLayer) { m_EAGLLayer = pLayer; }
    OdIntPtr get_EAGLLayer() const { return m_EAGLLayer; }
    void put_EAGLContext(OdIntPtr pContext) { m_EAGLContext = pContext; }
    OdIntPtr get_EAGLContext() const { return m_EAGLContext; }

    void generateProperties(OdRxDictionary* pInfo) const
    {
      ODRX_INHERIT_PROPERTIES_TRV(_OdPlatformGLES2Device);
      ODRX_GENERATE_PROPERTY_TRV(EAGLLayer)
      ODRX_GENERATE_PROPERTY_TRV(EAGLContext)
    }

    void configureForPlatform(OdTrVisRenderClient *pRenderClient, bool bOffScreen, bool bMobile, bool bDynamicVBO)
    { bMobile = true;
      _OdPlatformGLES2Device::configureForPlatform(pRenderClient, bOffScreen, bMobile, bDynamicVBO);
    }
};

ODRX_DEFINE_PROPERTY_PREFIX_TRV(OdPlatformGLES2Device::, EAGLLayer  , OdPlatformGLES2Device, getIntPtr)
ODRX_DEFINE_PROPERTY_PREFIX_TRV(OdPlatformGLES2Device::, EAGLContext, OdPlatformGLES2Device, getIntPtr)

#elif defined(ANDROID)

/* Android platform device implementation */

class OdPlatformGLES2Device : public _OdPlatformGLES2Device
{
  protected:
    OdIntPtr m_NativeWindow;
    OdIntPtr m_WindowDisplay;
    OdIntPtr m_EGLDisplay;
    OdIntPtr m_EGLSurface;
    OdIntPtr m_EGLContext;
  public:
    ODRX_DECLARE_PROPERTY_TRV(ANativeWindow)
    ODRX_DECLARE_PROPERTY_TRV(WindowDisplay)
    ODRX_DECLARE_PROPERTY_TRV(EGLDisplay)
    ODRX_DECLARE_PROPERTY_TRV(EGLSurface)
    ODRX_DECLARE_PROPERTY_TRV(EGLContext)

    OdPlatformGLES2Device() : m_NativeWindow(NULL), m_WindowDisplay(NULL),
                              m_EGLDisplay(NULL), m_EGLSurface(NULL), m_EGLContext(NULL) { }
    void put_ANativeWindow(OdIntPtr pWindow) { m_NativeWindow = pWindow; }
    OdIntPtr get_ANativeWindow() const { return m_NativeWindow; }
    void put_WindowDisplay(OdIntPtr pDisplay) { m_WindowDisplay = pDisplay; }
    OdIntPtr get_WindowDisplay() const { return m_WindowDisplay; }
    void put_EGLDisplay(OdIntPtr pDisplay) { m_EGLDisplay = pDisplay; }
    OdIntPtr get_EGLDisplay() const { return m_EGLDisplay; }
    void put_EGLSurface(OdIntPtr pSurf) { m_EGLSurface = pSurf; }
    OdIntPtr get_EGLSurface() const { return m_EGLSurface; }
    void put_EGLContext(OdIntPtr pCtx) { m_EGLContext = pCtx; }
    OdIntPtr get_EGLContext() const { return m_EGLContext; }

    void generateProperties(OdRxDictionary* pInfo) const
    {
      ODRX_INHERIT_PROPERTIES_TRV(_OdPlatformGLES2Device);
      ODRX_GENERATE_PROPERTY_TRV(ANativeWindow)
      ODRX_GENERATE_PROPERTY_TRV(WindowDisplay)
      ODRX_GENERATE_PROPERTY_TRV(EGLDisplay)
      ODRX_GENERATE_PROPERTY_TRV(EGLSurface)
      ODRX_GENERATE_PROPERTY_TRV(EGLContext)
    }

    void configureForPlatform(OdTrVisRenderClient *pRenderClient, bool bOffScreen, bool bMobile, bool bDynamicVBO)
    { bMobile = true;
      _OdPlatformGLES2Device::configureForPlatform(pRenderClient, bOffScreen, bMobile, bDynamicVBO);
    }
};

ODRX_DEFINE_PROPERTY_PREFIX_TRV(OdPlatformGLES2Device::, ANativeWindow, OdPlatformGLES2Device, getIntPtr)
ODRX_DEFINE_PROPERTY_PREFIX_TRV(OdPlatformGLES2Device::, WindowDisplay, OdPlatformGLES2Device, getIntPtr)
ODRX_DEFINE_PROPERTY_PREFIX_TRV(OdPlatformGLES2Device::, EGLDisplay   , OdPlatformGLES2Device, getIntPtr)
ODRX_DEFINE_PROPERTY_PREFIX_TRV(OdPlatformGLES2Device::, EGLSurface   , OdPlatformGLES2Device, getIntPtr)
ODRX_DEFINE_PROPERTY_PREFIX_TRV(OdPlatformGLES2Device::, EGLContext   , OdPlatformGLES2Device, getIntPtr)

#elif defined(EMCC)

/* Emscripten cross-compiler device implementation */

class OdPlatformGLES2Device : public _OdPlatformGLES2Device {
  public:
    OdPlatformGLES2Device() {}

    DeviceSetupType deviceSetupOverride() const { return kDtOnScreen; } // Avoid partial update generation

    void configureForPlatform(OdTrVisRenderClient *pRenderClient, bool bOffScreen, bool bMobile, bool bDynamicVBO)
    { bOffScreen = false; /* To enable overlays */ bMobile = true; /* To disable sharing */ bDynamicVBO = true; /* WebGL specific */
      _OdPlatformGLES2Device::configureForPlatform(pRenderClient, bOffScreen, bMobile, bDynamicVBO);
    }
};

#elif defined(__linux__)

/* Linux platform device implementation */

OdIntPtr odGLES2X11ChooseVisual(OdIntPtr display, bool bDoubleBufferEnabled);

class OdPlatformGLES2Device : public _OdPlatformGLES2Device
{
  protected:
    OdIntPtr m_xDisplay;
    OdIntPtr m_xWindow;
    mutable OdIntPtr m_xVisual;
    OdIntPtr m_xContext;
  public:
    ODRX_DECLARE_PROPERTY_TRV(XDisplay)
    ODRX_DECLARE_PROPERTY_TRV(XWindow)
    ODRX_DECLARE_PROPERTY_TRV(XChooseVisual)
    ODRX_DECLARE_PROPERTY_TRV(GLXContext)

    OdPlatformGLES2Device()
    {
      m_xDisplay = 0;
      m_xWindow = 0;
      m_xVisual = 0;
      m_xContext = 0;
    }
    OdIntPtr getXDisplay() const { return m_xDisplay; }
    void setXDisplay(OdIntPtr dp) { m_xDisplay = dp; }
    OdIntPtr getXWindow() const { return m_xWindow; }
    void setXWindow(OdIntPtr win) { m_xWindow = win; }
    void setChooseVisual(OdIntPtr /*vinfo*/) { /* Could be answered only */ }
    OdIntPtr chooseVisual() const
    {
      if (!m_xVisual)
        m_xVisual = ::odGLES2X11ChooseVisual(m_xDisplay, m_pRenderClient->getProperty(OD_T("DoubleBufferEnabled"))->getBool());
      return m_xVisual;
    }
    OdIntPtr getXContext() const { return m_xContext; }
    void setXContext(OdIntPtr ctx) { m_xContext = ctx; }

    void generateProperties(OdRxDictionary* pInfo) const
    {
      ODRX_INHERIT_PROPERTIES_TRV(_OdPlatformGLES2Device);
      ODRX_GENERATE_PROPERTY_TRV(XDisplay)
      ODRX_GENERATE_PROPERTY_TRV(XWindow)
      ODRX_GENERATE_PROPERTY_TRV(XChooseVisual)
      ODRX_GENERATE_PROPERTY_TRV(GLXContext)
    }
};

ODRX_DEFINE_PROPERTY_METHODS_PREFIX_TRV(OdPlatformGLES2Device::, XDisplay     , OdPlatformGLES2Device, getXDisplay , setXDisplay    , getIntPtr)
ODRX_DEFINE_PROPERTY_METHODS_PREFIX_TRV(OdPlatformGLES2Device::, XWindow      , OdPlatformGLES2Device, getXWindow  , setXWindow     , getIntPtr)
ODRX_DEFINE_PROPERTY_METHODS_PREFIX_TRV(OdPlatformGLES2Device::, XChooseVisual, OdPlatformGLES2Device, chooseVisual, setChooseVisual, getIntPtr)
ODRX_DEFINE_PROPERTY_METHODS_PREFIX_TRV(OdPlatformGLES2Device::, GLXContext   , OdPlatformGLES2Device, getXContext , setXContext    , getIntPtr)

#elif defined(__APPLE__)

/* MacOS platform device implementation */

class OdPlatformGLES2Device : public _OdPlatformGLES2Device
{
  protected:
    OdIntPtr m_NSView;
    OdIntPtr m_NSOpenGLContext;
  public:
    ODRX_DECLARE_PROPERTY_TRV(NSView)
    ODRX_DECLARE_PROPERTY_TRV(NSOpenGLContext)

    OdPlatformGLES2Device() : m_NSView(NULL), m_NSOpenGLContext(NULL) { }
    void put_NSView(OdIntPtr pWindow) { m_NSView = pWindow; }
    OdIntPtr get_NSView() const { return m_NSView; }
    void put_NSOpenGLContext(OdIntPtr pContext) { m_NSOpenGLContext = pContext; }
    OdIntPtr get_NSOpenGLContext() const { return m_NSOpenGLContext; }

    void generateProperties(OdRxDictionary* pInfo) const
    {
      ODRX_INHERIT_PROPERTIES_TRV(_OdPlatformGLES2Device);
      ODRX_GENERATE_PROPERTY_TRV(NSView)
      ODRX_GENERATE_PROPERTY_TRV(NSOpenGLContext)
    }
};

ODRX_DEFINE_PROPERTY_PREFIX_TRV(OdPlatformGLES2Device::, NSView         , OdPlatformGLES2Device, getIntPtr)
ODRX_DEFINE_PROPERTY_PREFIX_TRV(OdPlatformGLES2Device::, NSOpenGLContext, OdPlatformGLES2Device, getIntPtr)

#else

#pragma MARKMESSAGE("Unknown platform.");

typedef _OdPlatformGLES2Device OdPlatformGLES2Device;

#endif

#ifndef TD_USE_QT_LIB

class OdPlatformBitmapGLES2Device : public OdGLES2PlatformBaseDevice
{
  protected:
    OdUInt32 m_idExternalFBO;
  public:
    ODRX_DECLARE_PROPERTY_TRV(ExternalFBO)

    OdPlatformBitmapGLES2Device() : m_idExternalFBO(0) {}
    void put_ExternalFBO(OdUInt32 idExternalFBO) { m_idExternalFBO = idExternalFBO; }
    OdUInt32 get_ExternalFBO() const { return m_idExternalFBO; }

    void generateProperties(OdRxDictionary* pInfo) const
    {
      ODRX_INHERIT_PROPERTIES_TRV(OdGLES2PlatformBaseDevice);
      ODRX_GENERATE_PROPERTY_TRV(ExternalFBO)
    }

    DeviceSetupType deviceSetupOverride() const
    {
      if (renderClient()->hasProperty(OD_T("ForcePartialUpdate")) &&
          renderClient()->getProperty(OD_T("ForcePartialUpdate"))->getBool())
        return kDtOnScreen;
      return kDtOffScreen;
    }
};

ODRX_DEFINE_PROPERTY_PREFIX_TRV(OdPlatformBitmapGLES2Device::, ExternalFBO, OdPlatformBitmapGLES2Device, getUInt32)

typedef OdTrVecLocalRenditionGsClient OdGLES2RenditionGsClient;

#else // TD_USE_QT_LIB

class OdPlatformBitmapGLES2Device : public OdPlatformGLES2Device
{
  ODRX_DECLARE_PROPERTY_TRV(QImageFrame)
  protected:
    QImage* m_pFrame;
#if QT_VERSION >= QT_VERSION_CHECK(5,4,0)
    OdSharedPtr<OdqOpenGLOffscreenGles> m_pOpenGlCtx;
#endif
    OdSharedPtr<OdqOffscreenGles> m_pGlCtx;
  public:
    ODRX_USING_HEAP_OPERATORS(OdPlatformGLES2Device);

    OdPlatformBitmapGLES2Device()
      : m_pFrame(NULL)
    {
    }

    void createContext()
    {
      ODA_ASSERT_ONCE(m_pQWidget.isNull());
#if QT_VERSION >= QT_VERSION_CHECK(5,4,0)
      bool useQOpenGLWidget = false; // TODO = true;
      if (m_pQWidget.data())
      {
        useQOpenGLWidget = (getQOpenGLWidget() != NULL);
        ODA_ASSERT_ONCE(useQOpenGLWidget == !getQGLWidget());
      }
      if (useQOpenGLWidget)
      {
        if (m_pOpenGlCtx.get())
          return;
        m_pRenderClient->setProperty(OD_T("DoubleBufferEnabled"), OdRxVariantValue(false));

        QSurfaceFormat fmt = QSurfaceFormat::defaultFormat();
        fmt.setProfile(QSurfaceFormat::CompatibilityProfile); // SUP-5015
        fmt.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
        m_pOpenGlCtx = new OdqOpenGLOffscreenGles(fmt);
        m_pQWidget = m_pOpenGlCtx;
        return;
      }
#endif

      if (m_pGlCtx.get())
        return;
      m_pRenderClient->setProperty(OD_T("DoubleBufferEnabled"), OdRxVariantValue(false));

      QGLFormat format = QGLFormat::defaultFormat();
      format.setProfile(QGLFormat::CompatibilityProfile); // SUP-5015
      format.setDoubleBuffer(false);
      m_pGlCtx = new OdqOffscreenGles(format);
      m_pQWidget = m_pGlCtx;
    }

    void generateProperties(OdRxDictionary* pInfo) const
    {
      if (pInfo->has("RasterImage")) pInfo->remove(OD_T("RasterImage"));
      if (pInfo->has("BitPerPixel")) pInfo->remove(OD_T("BitPerPixel"));
      ODRX_INHERIT_PROPERTIES_TRV(OdPlatformGLES2Device);
      ODRX_GENERATE_PROPERTY_TRV(QImageFrame)
    }

    bool onRenderingBegin(OdGsDCRect* pUpdatedRect = NULL, OdTrVectorizerUpdateHost* pUpdateHost = NULL)
    {
      ODA_ASSERT(m_pFrame);
      if (m_pFrame)
      {
#if QT_VERSION >= QT_VERSION_CHECK(5,4,0)
        if (m_pOpenGlCtx.isNull() && m_pGlCtx.isNull())
#else
        if (m_pGlCtx.isNull())
#endif
        createContext();

        int w = m_pRenderClient->outputWindowWidth(),
            h = m_pRenderClient->outputWindowHeight();
        if (pUpdatedRect)
        {
          w = Od_abs(pUpdatedRect->m_max.x - pUpdatedRect->m_min.x);
          h = Od_abs(pUpdatedRect->m_max.y - pUpdatedRect->m_min.y);
        }

      // resize gives us incorrect result without it
#if QT_VERSION >= QT_VERSION_CHECK(5,4,0)
        if (m_pOpenGlCtx.get())
        {
          m_pOpenGlCtx->setMaximumWidth(w);  m_pOpenGlCtx->setMinimumWidth(w);
          m_pOpenGlCtx->setMaximumHeight(h); m_pOpenGlCtx->setMinimumHeight(h);
          m_pOpenGlCtx->resize(w, h);
          ODA_ASSERT_VAR(QSize sz = m_pOpenGlCtx->size();)
          ODA_ASSERT_ONCE(w == sz.width() && h == sz.height());

          m_pOpenGlCtx->renderToImage(pUpdateHost, m_pRenderClient, pUpdatedRect);
        }
        else
#endif
        if (m_pGlCtx.get())
        {
          m_pGlCtx->setMaximumWidth(w);  m_pGlCtx->setMinimumWidth(w);
          m_pGlCtx->setMaximumHeight(h); m_pGlCtx->setMinimumHeight(h);
          m_pGlCtx->resize(w, h);
          ODA_ASSERT_VAR(QSize sz = m_pGlCtx->size();)
          ODA_ASSERT_ONCE(w == sz.width() && h == sz.height());

          m_pGlCtx->renderToImage(pUpdateHost, m_pRenderClient, pUpdatedRect);
        }
        *m_pFrame = m_pFrame->mirrored();
        // @@@test
        //if (false)
        //  m_pFrame->save("e:\\_Oda\\data\\_print_plot\\SUP-926 Whiteprint\\test.bmp", "BMP");
      }
      return false;
    }

    bool hasDirectRenderBuffer(bool* /*bTransparent*/ = NULL) const
    {
      return true;
    }
    OdUInt8 *getDirectRenderBuffer(OdUInt32* pWidth, OdUInt32* pHeight, OdUInt32* pFormat = NULL, OdUInt32* pAlignment = NULL)
    {
      OdUInt32 width = m_pRenderClient->outputWindowWidth(),
               height = m_pRenderClient->outputWindowHeight(),
               format = OdTrVisRenderClient::kRBufFmtRGB,
               alignment = 4;
      if (pWidth) *pWidth = width;
      if (pHeight) *pHeight = height;
      if (pFormat) *pFormat = format;
      if (pAlignment) *pAlignment = alignment;
      ODA_ASSERT(m_pFrame);
      if (m_pFrame)
      {
        ODA_ASSERT_ONCE(alignment == 4 || alignment == 1);

//#  if QT_VERSION < QT_VERSION_CHECK(5,0,0)
        enum QImage::Format fmt = QImage::Format_RGB888; // = QImage::Format_RGB32; 
        //format = OdTrVisRenderClient::kRBufFmtRGB;
//#  else
//    enum QImage::Format fmt = QImage::Format_RGBA8888;
//    format = OdTrVisRenderClient::kRBufFmtRGBA;
//#  endif
        if (pFormat) *pFormat = format,

        *m_pFrame = QImage(width, height, fmt);
        uchar* pBits = m_pFrame->bits();

        ODA_ASSERT_VAR(int szColors = m_pFrame->colorCount();)
        ODA_ASSERT_ONCE(!szColors);
        ODA_ASSERT_VAR(int szPerLine = m_pFrame->bytesPerLine();)
        ODA_ASSERT_ONCE(   (alignment == 4 && szPerLine == OdGiRasterImage::calcBMPScanLineSize(width, 24))
                        || (alignment == 1 && szPerLine == width * 3));
        m_pFrame->fill(Qt::white);

        return pBits;
      }
      return NULL;
    }

    void put_QImageFrame(OdIntPtr i) {
      m_pFrame = (QImage*)i;
    }
    OdIntPtr get_QImageFrame() const {
      return (OdIntPtr)m_pFrame;
    }
};

ODRX_DEFINE_PROPERTY_PREFIX_TRV(OdPlatformBitmapGLES2Device::, QImageFrame, OdPlatformBitmapGLES2Device, getIntPtr)

#include "gl2/TrGL2LocalContext.h"
class OdGLES2QtRenditionGsClient : public OdTrVecLocalRenditionGsClient, protected OdTrVisRenderClient
{
  protected:
    OdTrRndLocalContextPtr m_pExtContext;
    virtual bool hasProperty(const OdChar *pPropName) const
    {
      return !odStrCmp((const wchar_t*) pPropName, L"CreateContext");
    }
    virtual OdRxVariantValue getProperty(const OdChar *pPropName) const
    { return OdRxVariantValue(false); }
    virtual void setProperty(const OdChar * /*pPropName*/, OdRxVariantValue /*pValue*/) {}
    virtual int outputWindowWidth() const { return 0; }
    virtual int outputWindowHeight() const { return 0; }
    virtual void emitError(const char * /*pError*/) {}
    virtual void emitWarning(const char * /*pWarn*/) {}
  public:
    virtual OdTrRndLocalContextPtr createLocalContext(OdTrVisRenderClient *pDevice)
    {
      if (m_pExtContext.isNull())
      {
        m_pExtContext = renderModule()->createLocalContext(this);
        m_pExtContext->createContext(this);
      }
      return OdTrGL2LocalContext::createLocalContext(pDevice); }
    static OdTrRndLocalRenditionClientPtr createObject(OdTrRndRenderModule *pRenderModule)
    { OdSmartPtr<OdGLES2QtRenditionGsClient> pRC = OdRxObjectImpl<OdGLES2QtRenditionGsClient>::createObject();
      pRC->m_pRenderModule = pRenderModule;
      return pRC; }
};
typedef OdGLES2QtRenditionGsClient OdGLES2RenditionGsClient;

#endif // TD_USE_QT_LIB

class GLES2Module : public BaseTrVecModule
{
  protected:
    OdTrRndLocalRenditionClientPtr m_pRenditionClient;
    OdTrVectorizerModuleHostPtr setupModuleHost(OdTrVectorizerModuleHost *pModuleHost)
    {
      if (m_pRenditionClient.isNull())
      {
        OdTrRndRenderModulePtr pRenderModule = ::odrxDynamicLinker()->loadModule(OdTrGL2ModuleName, false);
        m_pRenditionClient = OdGLES2RenditionGsClient::createObject(pRenderModule);
      }
      static_cast<OdGLES2PlatformBaseDevice*>(pModuleHost)->setRenditionClient(m_pRenditionClient);
      return pModuleHost;
    }
  protected:
    OdSmartPtr<OdGsBaseVectorizeDevice> createDeviceObject()
    { return createVectorizeDevice(setupModuleHost(OdRxObjectImpl<OdPlatformGLES2Device, OdTrVectorizerModuleHost>::createObject()), OdTrVectorizerModuleHost::kDtOnScreen); }
    OdSmartPtr<OdGsViewImpl> createViewObject() { return createVectorizeView(); }

    OdSmartPtr<OdGsBaseVectorizeDevice> createBitmapDeviceObject()
    { return createVectorizeDevice(setupModuleHost(OdRxObjectImpl<OdPlatformBitmapGLES2Device, OdTrVectorizerModuleHost>::createObject()), OdTrVectorizerModuleHost::kDtOffScreen); }
    OdSmartPtr<OdGsViewImpl> createBitmapViewObject() { return createVectorizeView(); }

    virtual void initApp();
    virtual void uninitApp();
};

ODRX_DEFINE_DYNAMIC_MODULE(GLES2Module);

void GLES2Module::initApp() {
  BaseTrVecModule::initApp();
}

void GLES2Module::uninitApp() {
  BaseTrVecModule::uninitApp();
}

//
