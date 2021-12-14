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

#ifndef _EXDCENTITY_INCLUDED_
#define _EXDCENTITY_INCLUDED_

#include "ExDeepCloningExport.h"
#include "DbCircle.h"
#include "IdArrays.h"


class EXDEEPCLONING_EXPORT ExDcEntity : public OdDbCircle
{
public:
  ODDB_DECLARE_MEMBERS(ExDcEntity);

  ExDcEntity();
  virtual ~ExDcEntity();

  // Override cloning methods
  virtual OdDbObjectPtr subDeepClone(OdDbIdMapping& idMap, OdDbObject*, bool bPrimary) const ODRX_OVERRIDE;
  virtual OdDbObjectPtr subWblockClone(OdDbIdMapping& idMap, OdDbObject*, bool bPrimary) const ODRX_OVERRIDE;

  // Override IO methods
  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler) ODRX_OVERRIDE;
  virtual void dwgOutFields(OdDbDwgFiler* pFiler) const ODRX_OVERRIDE;
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler) ODRX_OVERRIDE;
  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const ODRX_OVERRIDE;

  // Property accessors
  OdDbObjectIdArray getOwnedIds() const;
  void setOwnedIds(const OdDbObjectIdArray& ownedIds);
  void addOwnedId(const OdDbObjectId& ownedId);

  OdDbObjectId getRefId() const;
  void setRefId(const OdDbObjectId& refId);

private:
  static const OdInt16 CurrentVersion;

  OdDbObjectIdArray m_ownedIds;
  OdDbObjectId      m_refId;
};

typedef OdSmartPtr<ExDcEntity> ExDcEntityPtr;

#endif // _EXDCENTITY_INCLUDED_
