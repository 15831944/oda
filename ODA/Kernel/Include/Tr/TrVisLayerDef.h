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
// GLES2 device layer definition

#ifndef ODTRVISLAYERDEF
#define ODTRVISLAYERDEF

#include "TD_PackPush.h"

#include "TrVisDefs.h"
#include "OdArray.h"

/** \details
    <group ExRender_Windows_Classes> 
*/
struct OdTrVisLayerProps
{
  // Flags
  enum LayerPropsFlags
  {
    kLyVisible = (1 << 0), // Layer is visible
    kLyFaded   = (1 << 1)  // Layer is faded
  };
  // Layer flags
  OdUInt16 m_flags;

  void resetLayerFlags(OdUInt16 flags = 0) { m_flags = flags; }

  bool isLayerVisible() const { return GETBIT(m_flags, kLyVisible); }
  void setLayerVisible(bool bSet) { SETBIT(m_flags, kLyVisible, bSet); }

  bool isLayerFaded() const { return GETBIT(m_flags, kLyFaded); }
  void setLayerFaded(bool bSet) { SETBIT(m_flags, kLyFaded, bSet); }

  bool operator ==(const OdTrVisLayerProps &l2) const { return m_flags == l2.m_flags; }
  bool operator !=(const OdTrVisLayerProps &l2) const { return m_flags != l2.m_flags; }
};

/** \details
    <group ExRender_Windows_Classes> 
*/
struct OdTrVisLayerDef
{
  // Layer properties
  OdTrVisLayerProps m_props;

  void setDefault()
  {
    m_props.resetLayerFlags();
  }

  OdUInt16 diff(const OdTrVisLayerDef &l2) const
  {
    return (m_props.m_flags ^ l2.m_props.m_flags);
  }
  void propagateLayerFlag(OdTrVisLayerProps::LayerPropsFlags nFlag, bool bSet)
  {
    SETBIT(m_props.m_flags, nFlag, bSet);
  }
  void propSetVisible(bool bSet) { propagateLayerFlag(OdTrVisLayerProps::kLyVisible, bSet); }
  void propSetFaded(bool bSet)   { propagateLayerFlag(OdTrVisLayerProps::kLyFaded,   bSet); }
};

#include "TD_PackPop.h"

#endif // ODTRVISLAYERDEF
