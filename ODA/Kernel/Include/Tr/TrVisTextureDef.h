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
// Rendition texture definition

#ifndef ODTRVISTEXTUREDEF
#define ODTRVISTEXTUREDEF

#include "TD_PackPush.h"

#include "TrVisTexture.h"

/** \details
    <group ExRender_Windows_Classes> 
*/
class OdTrVisTextureResolver : public OdRxObject
{
  public:
    virtual OdTrVisTexturePtr extractUnderlyingTexture() = 0;

    // Constructors
    static OdSmartPtr<OdTrVisTextureResolver> createSimplePreserver(const OdTrVisTexture *pTexture);
    static OdSmartPtr<OdTrVisTextureResolver> createOnceTexturePreserver(const OdTrVisTexture *pTexture, const OdTrVisTextureResolver *pExtraResolver);
};

typedef OdSmartPtr<OdTrVisTextureResolver> OdTrVisTextureResolverPtr;

/** \details
    <group ExRender_Windows_Classes> 
*/
struct OdTrVisTextureDef
{
  // Flags
  enum Flags
  {
    // Enable bilinear texture filtration.
    kSmoothFilter  = (1 << 0),
    // Enable texture color with vertex color modulation.
    kModulateColor = (1 << 1),
    // Intensity map or height map. Actual for opacity maps. Means that alpha must be computed from RGB intensity even if alpha channel available.
    kIntensityMap  = (1 << 2),
    // Image contain negative RGBA components, which is must be inverted before use.
    kNegative      = (1 << 3),
    // Means that texture provide precalculated bump(normal)-map, so calculation for this channel isn't required.
    kNormalMap     = (1 << 4),
    // Means that texture provide spherical environment map. If set together with cube map, spherical map must be calculated from cube map.
    kSphereMap     = (1 << 5),
    // Means that texture provide cube environment map. If set together with spherical map, spherical map must be calculated and used instead of cube map as is.
    kCubeMap       = (1 << 6),
    // Means that image is already grayscale (height or intensity map), so RGB intensity conversion isn't required, we can use any color channel instead as is.
    kGrayscale     = (1 << 7)
  };
  // Texture flags
  OdUInt16 m_flags;
  // Texture
  OdTrVisTexturePtr m_pTexture;
  // Texture resolver
  OdTrVisTextureResolverPtr m_pResolver;

  void resetFlags(OdUInt16 nFlags = 0)
  {
    m_flags = nFlags;
  }
  void setDefault()
  {
    resetFlags();
    m_pTexture = NULL;
  }

  OdTrVisTexturePtr texture() const
  {
    if (!m_pTexture.isNull())
      return m_pTexture;
    if (!m_pResolver.isNull())
      return OdTrVisTextureResolverPtr(m_pResolver)->extractUnderlyingTexture();
    return OdTrVisTexturePtr();
  }
  OdTrVisTexturePtr setTexture(const OdTrVisTexture *pTexture)
  {
    m_pTexture = pTexture;
    return m_pTexture;
  }

  void setSmoothFilter(bool bSet) { SETBIT(m_flags, kSmoothFilter, bSet); }
  bool smoothFilterEnabled() const { return GETBIT(m_flags, kSmoothFilter); }

  void setColorModulation(bool bSet) { SETBIT(m_flags, kModulateColor, bSet); }
  bool colorModulationEnabled() const { return GETBIT(m_flags, kModulateColor); }

  void setIntensityMap(bool bSet) { SETBIT(m_flags, kIntensityMap, bSet); }
  bool isIntensityMap() const { return GETBIT(m_flags, kIntensityMap); }

  void setNegativeImage(bool bSet) { SETBIT(m_flags, kNegative, bSet); }
  bool isNegativeImage() const { return GETBIT(m_flags, kNegative); }

  void setNormalMap(bool bSet) { SETBIT(m_flags, kNormalMap, bSet); }
  bool isNormalMap() const { return GETBIT(m_flags, kNormalMap); }

  void setSphericalMap(bool bSet) { SETBIT(m_flags, kSphereMap, bSet); }
  bool isSphericalMap() const { return GETBIT(m_flags, kSphereMap); }

  void setCubeMap(bool bSet) { SETBIT(m_flags, kCubeMap, bSet); }
  bool isCubeMap() const { return GETBIT(m_flags, kCubeMap); }

  void setGrayscaleImage(bool bSet) { SETBIT(m_flags, kGrayscale, bSet); }
  bool isGrayscaleImage() const { return GETBIT(m_flags, kGrayscale); }
};

#include "TD_PackPop.h"

#endif // ODTRVISTEXTUREDEF
