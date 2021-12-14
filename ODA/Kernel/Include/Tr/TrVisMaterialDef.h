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
// GLES2 device material definition

#ifndef ODTRVISMATERIALDEF
#define ODTRVISMATERIALDEF

#include "TD_PackPush.h"

#include "TrVisMaterialColor.h"
#include "TrVisTextureDef.h"

/** \details
    <group ExRender_Windows_Classes> 
*/
struct OdTrVisMaterialChannelComponent
{
  // Main material channel color.
  OdTrVisMaterialColor m_channelColor;
  // Factor to blend between material channel color and vertex color [0 - full material color, 1 - full vertex color]
  float m_blendFactor;
  // Material transparency [0 - completely transparent, 1 - opaque]
  float m_opacityLevel; // [Have sense for diffuse channel only]
  // Channel coefficient [have different meaning for different channels]. Must be non-zero to make channel active.
  // For specular channel: specular color (shininnes) component.
  // For bump and normal map channels: bump (normal) map scale.
  // For reflection and refracion channel: reflectance (refraction) coefficient.
  // For other channels it is can provide color intensity or blending opacity multiplier and typically set as 1.
  float m_channelValue;
  // Factor to blend between texture and channel color [0 - full channel color, 1 - full texture color]
  float m_textureBlend;
  // Texture ID
  OdTrVisTextureId m_textureId;
  // Texture tiling (wrapping) method
  enum WrapType
  {
    kWrapRepeat = 0,
    kWrapClamp,
    kWrapCrop,
    kWrapMirror
  } m_uWrap, m_vWrap;

  // Setup channel defaults
  void setDefault(float channelColor = 0.0f, float blendFactor = 1.0f, float channelValue = 0.0f)
  {
    m_channelColor[0] = m_channelColor[1] = m_channelColor[2] = channelColor;
    m_blendFactor = blendFactor;
    m_opacityLevel = 1.0f;
    m_channelValue = channelValue;
    m_textureBlend = 0.0f;
    m_textureId = kTrVisNullId;
    m_uWrap = m_vWrap = kWrapRepeat;
  }
  // Check that channel enabled
  bool isEnabled() const { return !OdTrVisFPZero(m_channelValue); }
};

/** \details
    <group ExRender_Windows_Classes>
*/
struct OdTrVisMaterialChannelDef
{
  // Realistic material channel component
  OdTrVisMaterialChannelComponent m_realisticComponent;
  // Material channel component for non-realistic modes
  OdTrVisMaterialChannelComponent m_shadingComponent;

  // Setup channel defaults
  void setDefault(float realisticColor = 0.0f, float shadingColor = 0.0f, float blendFactor = 1.0f, float channelValue = 0.0f)
  {
    m_realisticComponent.setDefault(realisticColor, blendFactor, channelValue);
    m_shadingComponent.setDefault(shadingColor, blendFactor, channelValue);
  }
  // Copy realistic material component to shading material component
  void shadingFromRealistic() { m_shadingComponent = m_realisticComponent; }
  // Copy shading material component to realistic material component
  void realisticFromShading() { m_realisticComponent = m_shadingComponent; }
  // Check that channel enabled
  bool isEnabled() const { return m_realisticComponent.isEnabled(); }
  // Check that shading component enabled
  bool hasShadingComponent() const { return m_shadingComponent.isEnabled(); }
};

/** \details
    <group ExRender_Windows_Classes> 
*/
struct OdTrVisMaterialDef
{
  // Ambient material channel
  OdTrVisMaterialChannelDef m_ambientChannel;
  // Diffuse material channel
  OdTrVisMaterialChannelDef m_diffuseChannel;
  // Specular material channel
  OdTrVisMaterialChannelDef m_specularChannel;
  // Emission material channel
  OdTrVisMaterialChannelDef m_emissionChannel;
  // Opacity material channel
  OdTrVisMaterialChannelDef m_opacityChannel;
  // Bump material channel
  OdTrVisMaterialChannelDef m_bumpMapChannel;
  // Reflection material channel
  OdTrVisMaterialChannelDef m_reflectChannel;
  // Refraction material channel
  OdTrVisMaterialChannelDef m_refractChannel;
  // Normal map material channel
  OdTrVisMaterialChannelDef m_normMapChannel;
  // Tint material channel
  OdTrVisMaterialChannelDef m_tintChannel;

  // Setup material defaults
  void setDefault()
  {
    m_ambientChannel.setDefault(0.0f, 0.0f, 0.0f, 1.0f);
    m_diffuseChannel.setDefault(0.0f, 0.0f, 1.0f, 1.0f);
    m_specularChannel.setDefault(1.0f, 0.0f, 0.0f, 50.0f);
    m_emissionChannel.setDefault();
    m_opacityChannel.setDefault();
    m_bumpMapChannel.setDefault();
    m_reflectChannel.setDefault();
    m_refractChannel.setDefault();
    m_normMapChannel.setDefault();
    m_tintChannel.setDefault();
  }
  // Copy realistic material component to shading material component
  void shadingFromRealistic()
  {
    m_ambientChannel.shadingFromRealistic();
    m_diffuseChannel.shadingFromRealistic();
    m_specularChannel.shadingFromRealistic();
    m_emissionChannel.shadingFromRealistic();
    m_opacityChannel.shadingFromRealistic();
    m_bumpMapChannel.shadingFromRealistic();
    m_reflectChannel.shadingFromRealistic();
    m_refractChannel.shadingFromRealistic();
    m_normMapChannel.shadingFromRealistic();
    m_tintChannel.shadingFromRealistic();
  }
  // Copy shading material component to realistic material component
  void realisticFromShading()
  {
    m_ambientChannel.realisticFromShading();
    m_diffuseChannel.realisticFromShading();
    m_specularChannel.realisticFromShading();
    m_emissionChannel.realisticFromShading();
    m_opacityChannel.realisticFromShading();
    m_bumpMapChannel.realisticFromShading();
    m_reflectChannel.realisticFromShading();
    m_refractChannel.realisticFromShading();
    m_normMapChannel.realisticFromShading();
    m_tintChannel.realisticFromShading();
  }
};

#include "TD_PackPop.h"

#endif // ODTRVISMATERIALDEF
