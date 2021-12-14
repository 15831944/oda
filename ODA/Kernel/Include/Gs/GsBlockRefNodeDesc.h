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

#ifndef __OD_GS_BLOCK_REF_NODE_DESC__
#define __OD_GS_BLOCK_REF_NODE_DESC__

#include "TD_PackPush.h"

//OdGsBlockRefNodeDesc is implemented as a class with implementation hidden in m_imp
//and a reference counter for it.
//It allows to use lightweighted copies if OdGsBlockRefNodeDesc and store it both as a key
//in shared definitions map and as OdGsSharedRefDefinition data member.

class OdGsBlockRefNodeDescImpl;
class OdGiSubEntityTraitsData;
class OdGeScale3d;
class OdGiAnnoScaleSet;

/** \details

    Corresponding C++ library: Gs
    <group OdGs_Classes> 
*/
class GS_TOOLKIT_EXPORT OdGsBlockRefNodeDesc
{
public:
  OdGsBlockRefNodeDesc(const OdDbStub* layoutBlockId, const OdGiSubEntityTraitsData& tr,
    const OdGeScale3d& scale, bool bUsePlotStyles, const OdGiAnnoScaleSet& annoScales);
  OdGsBlockRefNodeDesc(const OdGsBlockRefNodeDesc& c);
  OdGsBlockRefNodeDesc& operator =(const OdGsBlockRefNodeDesc& c);

  OdGsBlockRefNodeDesc(): m_imp(NULL) {}
  ~OdGsBlockRefNodeDesc();
  bool operator < (const OdGsBlockRefNodeDesc& v) const;
  const OdGsBlockRefNodeDescImpl* getImpl() const { ODA_ASSERT(m_imp); return m_imp; }
  const OdGeScale3d& scale() const;
  
  void save(OdGsFiler *pFiler) const;
  void load(OdGsFiler *pFiler);

private:
  OdGsBlockRefNodeDescImpl* m_imp;
};

#include "TD_PackPop.h"

#endif // __OD_GS_BLOCK_REF_NODE_DESC__
