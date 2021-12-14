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
// Viewport clipping and sectioning definition

#ifndef ODTRVISSECTIONDEF
#define ODTRVISSECTIONDEF

#include "TD_PackPush.h"

#include "TrVisDefs.h"
#include "Ps/PlotStyles.h"

/** \details
    <group ExRender_Windows_Classes> 
*/
struct OdTrVisSectionDef
{
  // Flags
  enum Flags
  {
    // Enable sections filling.
    kEnableFilling   = (1 << 0),
    // Enable sections stippling pattern.
    kEnableStippling = (1 << 1)
  };
  // Sectioning flags
  OdUInt16 m_flags;
  // Array of section planes
  OdTrVisSectionPlaneArray m_planes;
  // Sections filling color
  ODCOLORREF m_color;
  // Sections filling pattern color
  ODCOLORREF m_stippleColor;
  // Sections filling pattern
  OdUInt8 m_stipplePattern;

  void resetFlags(OdUInt16 nFlags = 0)
  {
    m_flags = nFlags;
  }
  void setDefault()
  {
    resetFlags();
    m_planes.setPhysicalLength(0);
    m_color = 0xFFFFFFFF;
    m_stippleColor = 0xFF000000;
    m_stipplePattern = 0;
  }

  void enableFilling(bool bSet) { SETBIT(m_flags, kEnableFilling, bSet); }
  bool isFillingEnabled() const { return GETBIT(m_flags, kEnableFilling); }

  void enableStippling(bool bSet) { SETBIT(m_flags, kEnableStippling, bSet); }
  bool isStipplingEnabled() const { return GETBIT(m_flags, kEnableStippling); }

  void setSectionPlanes(const OdTrVisSectionPlaneArray &aPlanes) { m_planes = aPlanes; }
  const OdTrVisSectionPlaneArray &sectionPlanes() const { return m_planes; }

  void appendSectionPlane(const OdTrVisSectionPlane &pPlane) { m_planes.push_back(pPlane); }
  void clearSectionPlanes() { m_planes.clear(); }

  bool hasSectionPlanes() const { return !m_planes.isEmpty(); }
  bool isClippingEnabled() const { return hasSectionPlanes(); }

  OdUInt32 numSectionPlanes() const { return (OdUInt32)m_planes.size(); }
  const OdTrVisSectionPlane &sectionPlane(OdUInt32 nPlane) const { return m_planes.getPtr()[nPlane]; }

  void setFillColor(ODCOLORREF color) { m_color = color; }
  ODCOLORREF fillColor() const { return m_color; }

  bool fillColorTransparent() const { return ODGETALPHA(m_color) < 0xFF; }
  bool fillColorInvisible() const { return ODGETALPHA(m_color) == 0; }

  void setStippleColor(ODCOLORREF color) { m_stippleColor = color; }
  ODCOLORREF stippleColor() const { return m_stippleColor; }

  void setStipplePattern(OdUInt8 pattern) { m_stipplePattern = pattern; }
  OdUInt8 stipplePattern() const { return m_stipplePattern; }

  void setStipplePattern(OdPs::FillStyle fillStyle) { m_stipplePattern = (OdUInt8)fillStyle - OdPs::kFsSolid; }
  void setStipplePattern(OdPs::FillStyle fillStyle, ODCOLORREF color) { setStipplePattern(fillStyle); m_stippleColor = color; }
};

#include "TD_PackPop.h"

#endif // ODTRVISSECTIONDEF
