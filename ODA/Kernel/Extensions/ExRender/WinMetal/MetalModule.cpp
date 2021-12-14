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
// MetalModule.cpp : Defines the initialization routines for the DLL.

#include "OdaCommon.h"
#include "RxDispatchImpl.h"
#include "RxDynamicModule.h"
#include "DynamicLinker.h"
#include "Gs/GsBaseVectorizeDevice.h" // for OdSmartPtr<OdGsBaseVectorizeDevice>
#include "Gs/GsViewImpl.h"            // for OdSmartPtr<OdGsViewImpl>
#include "../vec/TrVecBaseModule.h"
#include "TrVecLocalRenditionGsClient.h"
#include "TrRndRenderModule.h"

// Base device for platform-dependent screen and bitmap devices
class OdMetalPlatformBaseDevice : public OdTrVectorizerModuleHost
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
    kDepthBufMergeDbg   = (1 << 20)
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
  float    m_ssaoFocus, m_ssaoPower, m_ssaoBlurPower;
  OdUInt32 m_ssaoLoops, m_ssaoBlurRadius;
  OdUInt32 m_fxaaQuality;
  OdUInt32 m_nGPUSelectionLevel;//See GPUSelection::Level
public:
  // TrGL2-specific properties
  ODRX_DECLARE_PROPERTY_TRV(VBOLevel)
  ODRX_DECLARE_PROPERTY_TRV(StaticVBOLimit)
#ifdef UNIMPLEMENTEDMETAL
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
#endif
  ODRX_DECLARE_PROPERTY_TRV(BlendingMode)
#ifdef UNIMPLEMENTEDMETAL
  ODRX_DECLARE_PROPERTY_TRV(DepthBufferMergeMode)
  ODRX_DECLARE_PROPERTY_TRV(AntiAliasLevel)
  ODRX_DECLARE_PROPERTY_TRV(AntiAliasLevelExt)
  ODRX_DECLARE_PROPERTY_TRV(AntiAliasHltLevelExt)
  ODRX_DECLARE_PROPERTY_TRV(SSAOEnable)
  ODRX_DECLARE_PROPERTY_TRV(SSAOLoops)
  ODRX_DECLARE_PROPERTY_TRV(SSAOFocus)
  ODRX_DECLARE_PROPERTY_TRV(SSAOPower)
  ODRX_DECLARE_PROPERTY_TRV(SSAOBlurRadius)
  ODRX_DECLARE_PROPERTY_TRV(SSAOBlurPower)
  ODRX_DECLARE_PROPERTY_TRV(FXAAEnable)
  ODRX_DECLARE_PROPERTY_TRV(FXAAQuality)
  ODRX_DECLARE_PROPERTY_TRV(FXAA2dEnable)
  ODRX_DECLARE_PROPERTY_TRV(GPUSelectionEnable)
  ODRX_DECLARE_PROPERTY_TRV(GPUSelectionLevel)
#endif
  // Module-specific properties
  ODRX_DECLARE_PROPERTY_TRV(RenderSettings)
  ODRX_DECLARE_PROPERTY_TRV(SaveContextData)
  ODRX_DECLARE_PROPERTY_TRV(CheckRendererVersion)

  OdMetalPlatformBaseDevice()
    : m_nCheckVersion(0)
    , m_nPropFlags(kEnableStaticVBO
#ifdef UNIMPLEMENTEDMETAL
      | kEnableLwdPointOpt | kEnableLwdCache | kLwdGeometryShader 
      | kLtpGeometryShader | kColorModifyShader | kLwdDepthOffset 
      | kEnableShadows | kEnableVSMShadows | kPointShadowCubeMap 
      | kPointShadowGS | kShadowsSmoothness
#endif
    )
    , m_nStaticVBOLimit(256 * sizeof(float) * 3) // Allocate only for large vertex arrays
    , m_nLwdCacheLimit(16)
    , m_nBlendingMode(OdTrVisRenderClient::kBlendingSinglePassOIT)
    , m_vsmShadowsAmount(0.25f)
    , m_nShadowMapSoftness(3)
    , m_nShadowMapSize(0)
    , m_aaLevel(2.0f) // @@@tmp: m_aaLevel must be 0.0f by default?
    , m_aaLevelExt(0.25f)
    , m_aaHltLevel(0.075f)
    , m_ssaoFocus(1.5f)
    , m_ssaoPower(8.0f)
    , m_ssaoBlurPower(1.0f)
    , m_ssaoLoops(32)
    , m_ssaoBlurRadius(2)
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
#ifdef UNIMPLEMENTEDMETAL
    if (bMobile)
    {
      if (!bDynamicVBO)
        pRenderClient->setProperty(OD_T("ShareableDevice"), OdRxVariantValue(false));
      pRenderClient->setProperty(OD_T("LightSourcesLimit"), OdRxVariantValue(OdInt32(4)));
    }
    else
      pRenderClient->setProperty(OD_T("CombinedShadersStrategy"), OdRxVariantValue(true));
    pRenderClient->setProperty(OD_T("UseOverlays"), OdRxVariantValue(!bOffScreen));
#endif
    if (bDynamicVBO)
    {
      pRenderClient->setProperty(OD_T("VBOLevel"), OdRxVariantValue(OdUInt32(3)));
      pRenderClient->setProperty(OD_T("StaticVBOLimit"), OdRxVariantValue(OdUInt32(0)));
      //UNIMPLEMENTEDMETAL pRenderClient->setProperty(OD_T("LineweightCacheLimit"), OdRxVariantValue(OdUInt32(0)));
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

#ifdef UNIMPLEMENTEDMETAL
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
#endif
  void setBlendingMode(OdUInt32 nBlendingMode)
  {
    m_nBlendingMode = nBlendingMode;
  }
  OdUInt32 blendingMode() const
  {
    return m_nBlendingMode;
  }
#ifdef UNIMPLEMENTEDMETAL
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

  void setSsAOEnable(bool bEnable) { SETBIT(m_nPropFlags, kEnableSSAO, bEnable); }
  bool ssaoEnabled() const { return GETBIT(m_nPropFlags, kEnableSSAO); }
  void setSsAOLoops(OdUInt32 ssaoLoops) { m_ssaoLoops = ssaoLoops; }
  OdUInt32 ssaoLoops() const { return m_ssaoLoops; }
  void setSsAOFocus(double ssaoFocus) { m_ssaoFocus = (float)ssaoFocus; }
  float ssaoFocus() const { return m_ssaoFocus; }
  void setSsAOPower(double ssaoPower) { m_ssaoPower = (float)ssaoPower; }
  float ssaoPower() const { return m_ssaoPower; }

  void setSsAOBlurRadius(OdUInt32 ssaoBlurRadius) { m_ssaoBlurRadius = ssaoBlurRadius; }
  OdUInt32 ssaoBlurRadius() const { return m_ssaoBlurRadius; }
  void setSsAOBlurPower(double ssaoBlurPower) { m_ssaoBlurPower = (float)ssaoBlurPower; }
  float ssaoBlurPower() const { return m_ssaoBlurPower; }

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
#endif
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
#ifdef UNIMPLEMENTEDMETAL
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
#endif
    ODRX_GENERATE_PROPERTY_TRV(BlendingMode)
#ifdef UNIMPLEMENTEDMETAL
    ODRX_GENERATE_PROPERTY_TRV(DepthBufferMergeMode)
    ODRX_GENERATE_PROPERTY_TRV(AntiAliasLevel)
    ODRX_GENERATE_PROPERTY_TRV(AntiAliasLevelExt)
    ODRX_GENERATE_PROPERTY_TRV(AntiAliasHltLevelExt)
    ODRX_GENERATE_PROPERTY_TRV(SSAOEnable)
    ODRX_GENERATE_PROPERTY_TRV(SSAOLoops)
    ODRX_GENERATE_PROPERTY_TRV(SSAOFocus)
    ODRX_GENERATE_PROPERTY_TRV(SSAOPower)
    ODRX_GENERATE_PROPERTY_TRV(SSAOBlurRadius)
    ODRX_GENERATE_PROPERTY_TRV(SSAOBlurPower)
    ODRX_GENERATE_PROPERTY_TRV(FXAAEnable)
    ODRX_GENERATE_PROPERTY_TRV(FXAAQuality)
    ODRX_GENERATE_PROPERTY_TRV(FXAA2dEnable)
    ODRX_GENERATE_PROPERTY_TRV(GPUSelectionEnable)
    ODRX_GENERATE_PROPERTY_TRV(GPUSelectionLevel)
#endif
    // Module-specific properties
    ODRX_GENERATE_PROPERTY_TRV(RenderSettings)
    ODRX_GENERATE_PROPERTY_TRV(SaveContextData)
    ODRX_GENERATE_PROPERTY_TRV(CheckRendererVersion)
  }
};

// TrGL2-specific properties
ODRX_DEFINE_PROPERTY_METHODS_PREFIX_TRV(OdMetalPlatformBaseDevice::, VBOLevel                 , OdMetalPlatformBaseDevice, VBOLevel                        , setVBOLevel                   , getUInt32);
ODRX_DEFINE_PROPERTY_METHODS_PREFIX_TRV(OdMetalPlatformBaseDevice::, StaticVBOLimit           , OdMetalPlatformBaseDevice, staticVBOLimit                  , setStaticVBOLimit             , getUInt32);
#ifdef UNIMPLEMENTEDMETAL
ODRX_DEFINE_PROPERTY_METHODS_PREFIX_TRV(OdMetalPlatformBaseDevice::, CombinedShadersStrategy  , OdMetalPlatformBaseDevice, isCombinedShadersStrategyEnabled, setCombinedShadersStrategy    , getBool);
ODRX_DEFINE_PROPERTY_METHODS_PREFIX_TRV(OdMetalPlatformBaseDevice::, LineweightOptimization   , OdMetalPlatformBaseDevice, lineweightOptimizationLevel     , setLineweightOptimizationLevel, getUInt32);
ODRX_DEFINE_PROPERTY_METHODS_PREFIX_TRV(OdMetalPlatformBaseDevice::, LineweightCacheLimit     , OdMetalPlatformBaseDevice, lineweightCacheLimit            , setLineweightCacheLimit       , getUInt32);
ODRX_DEFINE_PROPERTY_METHODS_PREFIX_TRV(OdMetalPlatformBaseDevice::, GeometryShaderUsage      , OdMetalPlatformBaseDevice, geometryShaderUsageLevel        , setGeometryShaderUsageLevel   , getUInt32);
ODRX_DEFINE_PROPERTY_METHODS_PREFIX_TRV(OdMetalPlatformBaseDevice::, UseColorModifiersShader  , OdMetalPlatformBaseDevice, useColorModifiersShader         , setUseColorModifiersShader    , getBool);
ODRX_DEFINE_PROPERTY_METHODS_PREFIX_TRV(OdMetalPlatformBaseDevice::, LineweightDepthOffset    , OdMetalPlatformBaseDevice, useLineweightDepthOffset        , setLineweightDepthOffset      , getBool);
ODRX_DEFINE_PROPERTY_METHODS_PREFIX_TRV(OdMetalPlatformBaseDevice::, ShadowsRenderMode        , OdMetalPlatformBaseDevice, shadowsRenderMode               , setShadowsRenderMode          , getUInt32);
ODRX_DEFINE_PROPERTY_METHODS_PREFIX_TRV(OdMetalPlatformBaseDevice::, VsmShadowsAmount         , OdMetalPlatformBaseDevice, vsmShadowsAmount                , setVsmShadowsAmount           , getDouble);
ODRX_DEFINE_PROPERTY_METHODS_PREFIX_TRV(OdMetalPlatformBaseDevice::, ShadowMapSoftnessOverride, OdMetalPlatformBaseDevice, shadowMapSoftnessOverride       , setShadowMapSoftness          , getUInt32);
ODRX_DEFINE_PROPERTY_METHODS_PREFIX_TRV(OdMetalPlatformBaseDevice::, ShadowMapSizeOverride    , OdMetalPlatformBaseDevice, shadowMapSizeOverride           , setShadowMapSize              , getUInt32);
#endif
ODRX_DEFINE_PROPERTY_METHODS_PREFIX_TRV(OdMetalPlatformBaseDevice::, BlendingMode             , OdMetalPlatformBaseDevice, blendingMode                    , setBlendingMode               , getUInt32);
#ifdef UNIMPLEMENTEDMETAL
ODRX_DEFINE_PROPERTY_METHODS_PREFIX_TRV(OdMetalPlatformBaseDevice::, DepthBufferMergeMode     , OdMetalPlatformBaseDevice, depthBufferMergeMode            , setDepthBufferMergeMode       , getUInt32);
ODRX_DEFINE_PROPERTY_METHODS_PREFIX_TRV(OdMetalPlatformBaseDevice::, AntiAliasLevel           , OdMetalPlatformBaseDevice, antiAliasLevel                  , setAntiAliasLevel             , getDouble);
ODRX_DEFINE_PROPERTY_METHODS_PREFIX_TRV(OdMetalPlatformBaseDevice::, AntiAliasLevelExt        , OdMetalPlatformBaseDevice, antiAliasLevelExt               , setAntiAliasLevelExt          , getDouble);
ODRX_DEFINE_PROPERTY_METHODS_PREFIX_TRV(OdMetalPlatformBaseDevice::, AntiAliasHltLevelExt     , OdMetalPlatformBaseDevice, antiAliasHltLevelExt            , setAntiAliasHltLevelExt       , getDouble);
ODRX_DEFINE_PROPERTY_METHODS_PREFIX_TRV(OdMetalPlatformBaseDevice::, SSAOEnable               , OdMetalPlatformBaseDevice, ssaoEnabled                     , setSsAOEnable                 , getBool);
ODRX_DEFINE_PROPERTY_METHODS_PREFIX_TRV(OdMetalPlatformBaseDevice::, SSAOLoops                , OdMetalPlatformBaseDevice, ssaoLoops                       , setSsAOLoops                  , getUInt32);
ODRX_DEFINE_PROPERTY_METHODS_PREFIX_TRV(OdMetalPlatformBaseDevice::, SSAOFocus                , OdMetalPlatformBaseDevice, ssaoFocus                       , setSsAOFocus                  , getDouble);
ODRX_DEFINE_PROPERTY_METHODS_PREFIX_TRV(OdMetalPlatformBaseDevice::, SSAOPower                , OdMetalPlatformBaseDevice, ssaoPower                       , setSsAOPower                  , getDouble);
ODRX_DEFINE_PROPERTY_METHODS_PREFIX_TRV(OdMetalPlatformBaseDevice::, SSAOBlurRadius           , OdMetalPlatformBaseDevice, ssaoBlurRadius                  , setSsAOBlurRadius             , getUInt32);
ODRX_DEFINE_PROPERTY_METHODS_PREFIX_TRV(OdMetalPlatformBaseDevice::, SSAOBlurPower            , OdMetalPlatformBaseDevice, ssaoBlurPower                   , setSsAOBlurPower              , getDouble);
ODRX_DEFINE_PROPERTY_METHODS_PREFIX_TRV(OdMetalPlatformBaseDevice::, FXAAEnable               , OdMetalPlatformBaseDevice, fxaaEnabled                     , setFxAAEnable                 , getBool);
ODRX_DEFINE_PROPERTY_METHODS_PREFIX_TRV(OdMetalPlatformBaseDevice::, FXAAQuality              , OdMetalPlatformBaseDevice, fxaaQuality                     , setFXAAQuality                , getUInt32);
ODRX_DEFINE_PROPERTY_METHODS_PREFIX_TRV(OdMetalPlatformBaseDevice::, FXAA2dEnable             , OdMetalPlatformBaseDevice, fxaa2dEnabled                   , setFxAA2dEnable               , getBool);
ODRX_DEFINE_PROPERTY_METHODS_PREFIX_TRV(OdMetalPlatformBaseDevice::, GPUSelectionEnable       , OdMetalPlatformBaseDevice, gpuSelectionEnabled             , setGPUSelectionEnable         , getBool);
ODRX_DEFINE_PROPERTY_METHODS_PREFIX_TRV(OdMetalPlatformBaseDevice::, GPUSelectionLevel        , OdMetalPlatformBaseDevice, gpuSelectionLevel               , setGPUSelectionLevel          , getUInt32);
#endif
// Module-specific properties
ODRX_DEFINE_PROPERTY_PREFIX_TRV(OdMetalPlatformBaseDevice::, RenderSettings      , OdMetalPlatformBaseDevice, getIntPtr)
ODRX_DEFINE_PROPERTY_PREFIX_TRV(OdMetalPlatformBaseDevice::, SaveContextData     , OdMetalPlatformBaseDevice, getBool)
ODRX_DEFINE_PROPERTY_PREFIX_TRV(OdMetalPlatformBaseDevice::, CheckRendererVersion, OdMetalPlatformBaseDevice, getUInt32)

// Base device for platform-dependent screen devices
class _OdPlatformMetalDevice : public OdMetalPlatformBaseDevice
{
protected:
  bool m_bCreateContext;
public:
  ODRX_DECLARE_PROPERTY_TRV(CreateContext)

  _OdPlatformMetalDevice() : m_bCreateContext(true) {
  }

  void put_CreateContext(bool bSet) { m_bCreateContext = bSet; }
  bool get_CreateContext() const { return m_bCreateContext; }

  DeviceSetupType deviceSetupOverride() const { return kDtOnScreen; }

  void generateProperties(OdRxDictionary* pInfo) const
  {
    ODRX_INHERIT_PROPERTIES_TRV(OdMetalPlatformBaseDevice);
    ODRX_GENERATE_PROPERTY_TRV(CreateContext)
  }
};

ODRX_DEFINE_PROPERTY_PREFIX_TRV(_OdPlatformMetalDevice::, CreateContext , _OdPlatformMetalDevice, getBool)

#if defined(__IPHONE_OS_VERSION_MIN_REQUIRED) || defined(TARGET_IPHONE_SIMULATOR)

/* iOS platform device implementation */

class OdPlatformMetalDevice : public _OdPlatformMetalDevice
{
protected:
  OdIntPtr m_EAGLLayer;
  OdIntPtr m_EAGLContext;
public:
  ODRX_DECLARE_PROPERTY_TRV(EAGLLayer)
  ODRX_DECLARE_PROPERTY_TRV(EAGLContext)

  OdPlatformMetalDevice() : m_EAGLLayer(NULL), m_EAGLContext(NULL) { }
  void put_EAGLLayer(OdIntPtr pLayer) { m_EAGLLayer = pLayer; }
  OdIntPtr get_EAGLLayer() const { return m_EAGLLayer; }
  void put_EAGLContext(OdIntPtr pContext) { m_EAGLContext = pContext; }
  OdIntPtr get_EAGLContext() const { return m_EAGLContext; }

  void generateProperties(OdRxDictionary* pInfo) const
  {
    ODRX_INHERIT_PROPERTIES_TRV(_OdPlatformMetalDevice);
    ODRX_GENERATE_PROPERTY_TRV(EAGLLayer)
    ODRX_GENERATE_PROPERTY_TRV(EAGLContext)
  }
};

ODRX_DEFINE_PROPERTY_PREFIX_TRV(OdPlatformMetalDevice::, EAGLLayer  , OdPlatformMetalDevice, getIntPtr)
ODRX_DEFINE_PROPERTY_PREFIX_TRV(OdPlatformMetalDevice::, EAGLContext, OdPlatformMetalDevice, getIntPtr)

#else

/* MacOS platform device implementation */

class OdPlatformMetalDevice : public _OdPlatformMetalDevice
{
protected:
  OdIntPtr m_NSView;
  OdIntPtr m_NSOpenGLContext;
public:
  ODRX_DECLARE_PROPERTY_TRV(NSView)
  ODRX_DECLARE_PROPERTY_TRV(NSOpenGLContext)

  OdPlatformMetalDevice() : m_NSView(NULL), m_NSOpenGLContext(NULL) { }
  void put_NSView(OdIntPtr pWindow) { m_NSView = pWindow; }
  OdIntPtr get_NSView() const { return m_NSView; }
  void put_NSOpenGLContext(OdIntPtr pContext) { m_NSOpenGLContext = pContext; }
  OdIntPtr get_NSOpenGLContext() const { return m_NSOpenGLContext; }

  void generateProperties(OdRxDictionary* pInfo) const
  {
    ODRX_INHERIT_PROPERTIES_TRV(_OdPlatformMetalDevice);
    ODRX_GENERATE_PROPERTY_TRV(NSView)
    ODRX_GENERATE_PROPERTY_TRV(NSOpenGLContext)
  }
};

ODRX_DEFINE_PROPERTY_PREFIX_TRV(OdPlatformMetalDevice::, NSView         , OdPlatformMetalDevice, getIntPtr)
ODRX_DEFINE_PROPERTY_PREFIX_TRV(OdPlatformMetalDevice::, NSOpenGLContext, OdPlatformMetalDevice, getIntPtr)

#endif

class OdPlatformBitmapMetalDevice : public OdMetalPlatformBaseDevice
{
  protected:
    OdUInt32 m_idExternalFBO;
    // @@@TODO: Add your Metal-related properties for bitmap device here
  public:
    ODRX_DECLARE_PROPERTY_TRV(ExternalFBO)

    OdPlatformBitmapMetalDevice() : m_idExternalFBO(0) {}
    void put_ExternalFBO(OdUInt32 idExternalFBO) { m_idExternalFBO = idExternalFBO; }
    OdUInt32 get_ExternalFBO() const { return m_idExternalFBO; }

    void generateProperties(OdRxDictionary* pInfo) const
    {
      ODRX_INHERIT_PROPERTIES_TRV(OdMetalPlatformBaseDevice);
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

ODRX_DEFINE_PROPERTY_PREFIX_TRV(OdPlatformBitmapMetalDevice::, ExternalFBO, OdPlatformBitmapMetalDevice, getUInt32)

class MetalModule : public BaseTrVecModule
{
protected:
  OdTrRndLocalRenditionClientPtr m_pRenditionClient;
  OdTrVectorizerModuleHostPtr setupModuleHost(OdTrVectorizerModuleHost *pModuleHost)
  {
    if (m_pRenditionClient.isNull())
    {
      OdTrRndRenderModulePtr pRenderModule = ::odrxDynamicLinker()->loadModule(OdTrMetalModuleName);
      if (pRenderModule.isNull())
        throw OdError(eFileNotFound);
      m_pRenditionClient = OdTrVecLocalRenditionGsClient::createObject(pRenderModule);
    }
    static_cast<OdMetalPlatformBaseDevice*>(pModuleHost)->setRenditionClient(m_pRenditionClient);
    return pModuleHost;
  }
protected:
  OdSmartPtr<OdGsBaseVectorizeDevice> createDeviceObject()
  { return createVectorizeDevice(setupModuleHost(OdRxObjectImpl<OdPlatformMetalDevice, OdTrVectorizerModuleHost>::createObject()), OdTrVectorizerModuleHost::kDtOnScreen); }
  OdSmartPtr<OdGsViewImpl> createViewObject() { return createVectorizeView(); }

  OdSmartPtr<OdGsBaseVectorizeDevice> createBitmapDeviceObject()
  { return createVectorizeDevice(setupModuleHost(OdRxObjectImpl<OdPlatformBitmapMetalDevice, OdTrVectorizerModuleHost>::createObject()), OdTrVectorizerModuleHost::kDtOffScreen); }
  OdSmartPtr<OdGsViewImpl> createBitmapViewObject() { return createVectorizeView(); }

  virtual void initApp();
  virtual void uninitApp();
};

ODRX_DEFINE_DYNAMIC_MODULE(MetalModule);

void MetalModule::initApp() {
  BaseTrVecModule::initApp();
}

void MetalModule::uninitApp() {
  BaseTrVecModule::uninitApp();
}

//
