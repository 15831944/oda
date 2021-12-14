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
// Viewport fog definition

#ifndef ODTRVISFOGDEF
#define ODTRVISFOGDEF

#include "TD_PackPush.h"

/** \details
    <group ExRender_Windows_Classes> 
*/
struct OdTrVisFogDef
{
  // Flags
  enum Flags
  {
    // Enable fog.
    kEnableFog     = (1 << 0),
    // Display fog on background.
    kFogBackground = (1 << 1)
  };
  // Fog flags
  OdUInt16 m_flags;
  // Fog color
  ODCOLORREF m_color;
  // Fog distances (in world coordinates)
  double m_fDistNear, m_fDistFar;
  // Fog densities (in 0-1 range)
  double m_fDensNear, m_fDensFar;

  void resetFlags(OdUInt16 nFlags = 0)
  {
    m_flags = nFlags;
  }
  void setDefault()
  {
    resetFlags();
    m_color = 0xFFFFFFFF;
    m_fDistNear = m_fDistFar = 0.0;
    m_fDensNear = 0.0; m_fDensFar = 1.0;
  }

  void enableFog(bool bSet) { SETBIT(m_flags, kEnableFog, bSet); }
  bool isFogEnabled() const { return GETBIT(m_flags, kEnableFog); }

  void enableFogAtBackground(bool bSet) { SETBIT(m_flags, kFogBackground, bSet); }
  bool isFogAtBackgroundEnabled() const { return GETBIT(m_flags, kFogBackground); }

  void setColor(ODCOLORREF color) { m_color = color; }
  ODCOLORREF color() const { return m_color; }

  void setDistances(double fNear, double fFar) { m_fDistNear = fNear; m_fDistFar = fFar; }
  double nearDistance() const { return m_fDistNear; }
  double farDistance() const { return m_fDistFar; }
  double depthRange() const { return m_fDistFar - m_fDistNear; }

  void setDensities(double fNear, double fFar) { m_fDensNear = fNear; m_fDensFar = fFar; }
  double nearDensity() const { return m_fDensNear; }
  double farDensity() const { return m_fDensFar; }
  double densityRange() const { return m_fDensFar - m_fDensNear; }

  bool operator ==(const OdTrVisFogDef &fog) const
  {
    if (isFogEnabled() != fog.isFogEnabled())
      return false;
    if (!isFogEnabled())
      return true;
    return (isFogAtBackgroundEnabled() == fog.isFogAtBackgroundEnabled()) &&
      (color() == fog.color()) && OdEqual(nearDistance(), fog.nearDistance()) &&
      OdEqual(farDistance(), fog.farDistance()) && OdEqual(nearDensity(), fog.nearDensity()) &&
      OdEqual(farDensity(), fog.farDensity());
  }
  bool operator !=(const OdTrVisFogDef &fog) const
  {
    if (isFogEnabled() != fog.isFogEnabled())
      return true;
    if (!isFogEnabled())
      return false;
    return (isFogAtBackgroundEnabled() != fog.isFogAtBackgroundEnabled()) ||
      (color() != fog.color()) || !OdEqual(nearDistance(), fog.nearDistance()) ||
      !OdEqual(farDistance(), fog.farDistance()) || !OdEqual(nearDensity(), fog.nearDensity()) ||
      !OdEqual(farDensity(), fog.farDensity());
  }
};

#include "TD_PackPop.h"

#endif // ODTRVISFOGDEF
