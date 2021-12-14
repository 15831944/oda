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
// GLES2 device definitions

#ifndef ODBASETRVECDEVICEDEFINITIONS
#define ODBASETRVECDEVICEDEFINITIONS

#include "../TrVisDefs.h"
#include "../TrVisLightDef.h"

/** \details
    <group ExRender_Windows_Classes> 
*/
struct OdTrVecLightDef : public OdTrVisLightDef
{ OdUInt32 m_nDuplicate;
  enum { kOverrideDuplicate = kLastOverride << 1 };
  void setDefault()
  { OdTrVisLightDef::setDefault();
    m_nDuplicate = 0; }
  OdTrVecLightDef &apply(OdUInt32 nMod, const OdTrVecLightDef &l)
  { if (nMod != 0xFFFFFFFF)
    { OdTrVisLightDef::apply(nMod, l);
      if (GETBIT(nMod, kOverrideDuplicate))
        m_nDuplicate = l.m_nDuplicate;
    } else
      *this = l;
    return *this; }
  OdUInt32 diff(const OdTrVecLightDef &dl2, bool bFinFirstDiff = false) const
  { const OdUInt32 axxum = OdTrVisLightDef::diff(dl2, bFinFirstDiff);
    if (axxum != 0xFFFFFFFF)
    { if (m_nDuplicate != dl2.m_nDuplicate)
        return (bFinFirstDiff) ? 0xFFFFFFFF : (axxum | kOverrideDuplicate);
    } return axxum; }
  bool operator ==(const OdTrVecLightDef &dl2) const
  { return diff(dl2, true) == 0; }
  bool operator !=(const OdTrVecLightDef &dl2) const
  { return diff(dl2, true) != 0; }
};

#endif // ODBASETRVECDEVICEDEFINITIONS
