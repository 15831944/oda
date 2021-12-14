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
// Material color definition

#ifndef ODTRVISMATERIALCOLOR
#define ODTRVISMATERIALCOLOR

#include "TD_PackPush.h"

#include "TrVisDefs.h"

/** \details
    <group ExRender_Windows_Classes> 
*/
struct OdTrVisMaterialColor
{
  protected: float m_r, m_g, m_b, m_a;
  float clamp(float f) const { return OdTrVisClamp(f, 0.0f, 1.0f); }
  OdTrVisMaterialColor(float r, float g, float b, float a) : m_r(r), m_g(g), m_b(b), m_a(a) {}
  public: OdTrVisMaterialColor() : m_r(0.0f), m_g(0.0f), m_b(0.0f), m_a(1.0f) {}
  OdTrVisMaterialColor(float r, float g, float b) : m_r(r), m_g(g), m_b(b), m_a(1.0f) {}
  float red() const { return m_r; } float green() const { return m_g; } float blue() const { return m_b; }
  void setRed(float r) { m_r = r; } void setGreen(float g) { m_g = g; } void setBlue(float b) { m_b = b; }
  float alpha() const { return m_a; } void setAlpha(float a) { m_a = a; }
  void set(float r, float g, float b) { m_r = r; m_g = g; m_b = b; }
  ODCOLORREF toCref() const { return ODRGBA(clamp(m_r) * 255, clamp(m_g) * 255, clamp(m_b) * 255, clamp(m_a) * 255); }
  void fromCref(ODCOLORREF cRef) { m_r = float(ODGETRED(cRef) / 255); m_g = float(ODGETGREEN(cRef) / 255); m_b = float(ODGETBLUE(cRef) / 255); }
  float *asArray() { return &m_r; } const float *asArray() const { return &m_r; }
  void validateRange() { m_r = clamp(m_r); m_g = clamp(m_g); m_b = clamp(m_b); m_a = clamp(m_a); }
  // Old-school interface
  void operator *=(const OdTrVisMaterialColor &clr2) { m_r *= clr2.m_r; m_g *= clr2.m_g; m_b *= clr2.m_b; m_a *= clr2.m_a; }
  OdTrVisMaterialColor operator *(const OdTrVisMaterialColor &clr2) const
  { return OdTrVisMaterialColor(m_r * clr2.m_r, m_g * clr2.m_g, m_b * clr2.m_b, m_a * clr2.m_a); }
  void operator *=(float mlfc) { m_r *= mlfc; m_g *= mlfc; m_b *= mlfc; /*m_a *= mlfc;*/ }
  OdTrVisMaterialColor operator *(float mlfc) const
  { return OdTrVisMaterialColor(m_r * mlfc, m_g * mlfc, m_b * mlfc, m_a /* mlfc*/); }
  bool operator ==(const OdTrVisMaterialColor &clr2) const
  { return OdTrVisFPEqual(m_r, clr2.m_r) && OdTrVisFPEqual(m_g, clr2.m_g) && OdTrVisFPEqual(m_b, clr2.m_b) && OdTrVisFPEqual(m_a, clr2.m_a); }
  bool operator !=(const OdTrVisMaterialColor &clr2) const
  { return !OdTrVisFPEqual(m_r, clr2.m_r) || !OdTrVisFPEqual(m_g, clr2.m_g) || !OdTrVisFPEqual(m_b, clr2.m_b) || !OdTrVisFPEqual(m_a, clr2.m_a); }
  float operator [](int nElem) const { switch (nElem) { case 0: return m_r; case 1: return m_g; case 2: return m_b; default: return m_a; }}
  float &operator [](int nElem) { switch (nElem) { case 0: return m_r; case 1: return m_g; case 2: return m_b; default: return m_a; } }
};

#include "TD_PackPop.h"

#endif // ODTRVISMATERIALCOLOR
