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

/************************************************************************/
/* ExEmbeddedSolid.h: interface for the EmbeddedSolidWrapper class,     */
/* representing a custom entity with embedded 3dSolid entity            */
/************************************************************************/
#if !defined(_EXEMBEDDEDSOLID_INCLUDED_)
#define _EXEMBEDDEDSOLID_INCLUDED_

#include "ExCustObjExport.h"
#include "DbEntity.h"
#include "Db3dSolid.h"


class EmbeddedSolidWrapper : public OdDbEntity
{
  static const int lastKnownVersion;
public:
  ODDB_DECLARE_MEMBERS(EmbeddedSolidWrapper);

	EmbeddedSolidWrapper();
	virtual ~EmbeddedSolidWrapper();

  bool subWorldDraw(OdGiWorldDraw* pWd) const;
  OdResult dwgInFields(OdDbDwgFiler* pFiler);
  void dwgOutFields(OdDbDwgFiler* pFiler) const;
  OdResult dxfInFields(OdDbDxfFiler* pFiler);
  void dxfOutFields(OdDbDxfFiler* pFiler) const;

  void initSolid(OdDbDatabase* pDb);

private:
  OdDb3dSolidPtr m_pSolid;
};

typedef OdSmartPtr<EmbeddedSolidWrapper> EmbeddedSolidWrapperPtr;


#endif
