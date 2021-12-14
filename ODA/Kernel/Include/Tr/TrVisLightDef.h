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
// GLES2 device light definition

#ifndef ODTRVISLIGHTDEF
#define ODTRVISLIGHTDEF

#include "TD_PackPush.h"

#include "TrVisMaterialColor.h"
#include "Ge/GeVector3d.h"
#include "Ge/GePoint3d.h"
#include "Gs/GsFiler.h"

/** \details
    <group ExRender_Windows_Classes> 
*/
struct OdTrVisLightDef
{
  // Flags
  enum LightDefFlags
  {
    kLightEnabled = (1 << 0), // Light is on
    kLightUseLims = (1 << 1), // Enable attenuation limits
    kLightShadows = (1 << 2)  // Shadows casting enabled
  };
  // Overriders
  enum Overrider
  {
    // Never override type and 'default light' state
    kOverrideVisibility    = (1 << 0),
    kOverrideOrientation   = (1 << 1),
    kOverrideColor         = (1 << 2),
    kOverrideAttenuation   = (1 << 3),
    kOverrideAngles        = (1 << 4),
    kOverrideSpecualFactor = (1 << 5),
    kOverrideShadow        = (1 << 6),
    kLastOverride          = kOverrideShadow
  };
  // Type of light
  enum LightType
  {
    kDistant = 0,
    kPoint,
    kSpot
  } m_type;
  // Light position
  OdGePoint3d m_position;
  // Light direction vector
  OdGeVector3d m_direction;
  // Light color
  OdTrVisMaterialColor m_color;
  // Attenuations
  float m_constantAttenuation;
  float m_linearAttenuation;
  float m_quadraticAttenuation;
  // Attenuation limits
  float m_startAttenuationLimit;
  float m_endAttenuationLimit;
  // Spot light parameters
  float m_spotCutoff;
  float m_spotExponent;
  // Factor for specular highlighting
  float m_specularFactor;
  // Shadow map parameters
  OdUInt16 m_shadowMapSize;
  OdUInt16 m_shadowSoftness;
  OdUInt16 m_shadowSamples;
  // Flags
  OdUInt16 m_lightFlags;

  void setLightOn(bool bSet) { SETBIT(m_lightFlags, kLightEnabled, bSet); }
  bool isLightOn() const { return GETBIT(m_lightFlags, kLightEnabled); }

  void enableAttenuationLimits(bool bSet) { SETBIT(m_lightFlags, kLightUseLims, bSet); }
  bool attenuationLimitsEnabled() const { return GETBIT(m_lightFlags, kLightUseLims); }

  void enableShadows(bool bSet) { SETBIT(m_lightFlags, kLightShadows, bSet); }
  bool shadowsEnabled() const { return GETBIT(m_lightFlags, kLightShadows); }

  // Setup light defaults
  void setDefault()
  {
    m_type = kDistant;
    m_position = OdGePoint3d::kOrigin;
    m_direction = OdGeVector3d::kZAxis;
    m_color.set(0.0f, 0.0f, 0.0f);
    m_constantAttenuation = 1.0f;
    m_linearAttenuation = 0.0f;
    m_quadraticAttenuation = 0.0f;
    m_startAttenuationLimit = 0.0f;
    m_endAttenuationLimit = 1.0f;
    m_spotCutoff = m_spotExponent = 0.0f;
    m_specularFactor = 0.0f;
    m_shadowMapSize = 256;
    m_shadowSoftness = 1;
    m_shadowSamples = 16;
    m_lightFlags = 0;
  }

  // Partial data apply
  OdTrVisLightDef &apply(OdUInt32 nMod, const OdTrVisLightDef &l)
  {
    if (nMod != 0xFFFFFFFF)
    {
      if (GETBIT(nMod, kOverrideVisibility))
        setLightOn(l.isLightOn());
      if (GETBIT(nMod, kOverrideOrientation))
        m_position = l.m_position,
        m_direction = l.m_direction;
      if (GETBIT(nMod, kOverrideColor))
        m_color = l.m_color;
      if (GETBIT(nMod, kOverrideAttenuation))
        m_constantAttenuation = l.m_constantAttenuation,
        m_linearAttenuation = l.m_linearAttenuation,
        m_quadraticAttenuation = l.m_quadraticAttenuation,
        enableAttenuationLimits(l.attenuationLimitsEnabled()),
        m_startAttenuationLimit = l.m_startAttenuationLimit,
        m_endAttenuationLimit = l.m_endAttenuationLimit;
      if (GETBIT(nMod, kOverrideAngles))
        m_spotCutoff = l.m_spotCutoff,
        m_spotExponent = l.m_spotExponent;
      if (GETBIT(nMod, kOverrideSpecualFactor))
        m_specularFactor = l.m_specularFactor;
      if (GETBIT(nMod, kOverrideShadow))
        enableShadows(l.shadowsEnabled()),
        m_shadowMapSize = l.m_shadowMapSize,
        m_shadowSoftness = l.m_shadowSoftness,
        m_shadowSamples = l.m_shadowSamples;
    }
    else
      *this = l;
    return *this;
  }

  OdUInt32 diff(const OdTrVisLightDef &dl2, bool bFinFirstDiff = false) const
  {
    if (m_type != dl2.m_type)
      return 0xFFFFFFFF;
    OdUInt32 axxum = 0;
    if (isLightOn() != dl2.isLightOn())
    { if (bFinFirstDiff)
        return 0xFFFFFFFF;
      SETBIT_1(axxum, kOverrideVisibility);
    }
    if (!m_position.isEqualTo(dl2.m_position) || !m_direction.isEqualTo(dl2.m_direction))
    { if (bFinFirstDiff)
        return 0xFFFFFFFF;
      SETBIT_1(axxum, kOverrideOrientation);
    }
    if (m_color != dl2.m_color)
    { if (bFinFirstDiff)
        return 0xFFFFFFFF;
      SETBIT_1(axxum, kOverrideColor);
    }
    if (!OdTrVisFPEqual(m_constantAttenuation, dl2.m_constantAttenuation) ||
        !OdTrVisFPEqual(m_linearAttenuation, dl2.m_linearAttenuation) ||
        !OdTrVisFPEqual(m_quadraticAttenuation, dl2.m_quadraticAttenuation) ||
        (attenuationLimitsEnabled() != dl2.attenuationLimitsEnabled()) ||
        !OdTrVisFPEqual(m_startAttenuationLimit, dl2.m_startAttenuationLimit) ||
        !OdTrVisFPEqual(m_endAttenuationLimit, dl2.m_endAttenuationLimit))
    { if (bFinFirstDiff)
        return 0xFFFFFFFF;
      SETBIT_1(axxum, kOverrideAttenuation);
    }
    if (!OdTrVisFPEqual(m_spotCutoff, dl2.m_spotCutoff) || !OdTrVisFPEqual(m_spotExponent, dl2.m_spotExponent))
    { if (bFinFirstDiff)
        return 0xFFFFFFFF;
      SETBIT_1(axxum, kOverrideAngles);
    }
    if (!OdTrVisFPEqual(m_specularFactor, dl2.m_specularFactor))
    { if (bFinFirstDiff)
        return 0xFFFFFFFF;
      SETBIT_1(axxum, kOverrideSpecualFactor);
    }
    if ((shadowsEnabled() != dl2.shadowsEnabled()) || (m_shadowMapSize != dl2.m_shadowMapSize) ||
        (m_shadowSoftness != dl2.m_shadowSoftness) || (m_shadowSamples != dl2.m_shadowSamples))
    { if (bFinFirstDiff)
        return 0xFFFFFFFF;
      SETBIT_1(axxum, kOverrideShadow);
    }
    return axxum;
  }

  bool operator ==(const OdTrVisLightDef &dl2) const
  { return diff(dl2, true) == 0; }
  bool operator !=(const OdTrVisLightDef &dl2) const
  { return diff(dl2, true) != 0; }

  void save(OdGsFiler *pFiler) const;
  void load(OdGsFiler *pFiler);
};

inline void OdTrVisLightDef::save(OdGsFiler *pFiler) const {
  pFiler->wrUInt8   ((OdUInt8)m_type        );
  pFiler->wrPoint3d (m_position             );
  pFiler->wrVector3d(m_direction            );
  for (int nColorElem = 0; nColorElem < 4; nColorElem++)
    pFiler->wrFloat (m_color[nColorElem]    );
  pFiler->wrFloat   (m_constantAttenuation  );
  pFiler->wrFloat   (m_linearAttenuation    );
  pFiler->wrFloat   (m_quadraticAttenuation );
  pFiler->wrFloat   (m_startAttenuationLimit);
  pFiler->wrFloat   (m_endAttenuationLimit  );
  pFiler->wrFloat   (m_spotCutoff           );
  pFiler->wrFloat   (m_spotExponent         );
  pFiler->wrFloat   (m_specularFactor       );
  pFiler->wrUInt16  (m_shadowMapSize        );
  pFiler->wrUInt16  (m_shadowSoftness       );
  pFiler->wrUInt16  (m_shadowSamples        );
  pFiler->wrUInt16  (m_lightFlags           );
}

inline void OdTrVisLightDef::load(OdGsFiler *pFiler) {
  m_type = (LightType)pFiler->rdUInt8();
  pFiler->rdPoint3d(m_position);
  pFiler->rdVector3d(m_direction);
  for (int nColorElem = 0; nColorElem < 4; nColorElem++)
    m_color[nColorElem]   = pFiler->rdFloat();
  m_constantAttenuation   = pFiler->rdFloat();
  m_linearAttenuation     = pFiler->rdFloat();
  m_quadraticAttenuation  = pFiler->rdFloat();
  m_startAttenuationLimit = pFiler->rdFloat();
  m_endAttenuationLimit   = pFiler->rdFloat();
  m_spotCutoff            = pFiler->rdFloat();
  m_spotExponent          = pFiler->rdFloat();
  m_specularFactor        = pFiler->rdFloat();
  m_shadowMapSize         = pFiler->rdUInt16();
  m_shadowSoftness        = pFiler->rdUInt16();
  m_shadowSamples         = pFiler->rdUInt16();
  m_lightFlags            = pFiler->rdUInt16();
}

#include "TD_PackPop.h"

#endif // ODTRVISLIGHTDEF
