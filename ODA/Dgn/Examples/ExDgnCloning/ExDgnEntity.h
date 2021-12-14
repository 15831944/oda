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

#ifndef _EXDGNENTITY_INCLUDED_
#define _EXDGNENTITY_INCLUDED_

#include "ExDgnCloningExport.h"
#include "DgEllipse.h"
//#include "IdArrays.h"


class EXDGNCLONING_EXPORT ExDgnEntity : public OdDgEllipse3d
{
public:
  ODRX_DECLARE_MEMBERS(ExDgnEntity);

  ExDgnEntity();
  virtual ~ExDgnEntity();

  // Override cloning methods
  virtual OdDgElementPtr subDeepClone(OdDgIdMapping& idMap) const;
  //virtual OdDgElementPtr subWblockClone(OdDgIdMapping& idMap) const;

  // Override IO methods
  virtual OdResult dgnInFields(OdDgFiler* pFiler);
  virtual void dgnOutFields(OdDgFiler* pFiler) const;

  // Property accessors
  OdDgElementIdArray getOwnedIds() const;
  void setOwnedIds(const OdDgElementIdArray& ownedIds);
  void addOwnedId(const OdDgElementId& ownedId);

  OdDgElementId getRefId() const;
  void setRefId(const OdDgElementId& refId);

private:
  //static const OdInt16 CurrentVersion;

  OdDgElementIdArray  m_ownedIds;
  OdDgElementId       m_refId;
};

typedef OdSmartPtr<ExDgnEntity> ExDgnEntityPtr;

#endif // _EXDGNENTITY_INCLUDED_
