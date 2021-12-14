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
/* ExSphere.h: interface for the ExSphere class.                        */
/************************************************************************/
#if !defined(AFX_EXSPHERE_H__335BDC0F_7675_4177_841E_EB995F14FD37__INCLUDED_)
#define AFX_EXSPHERE_H__335BDC0F_7675_4177_841E_EB995F14FD37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "ExCustObjExport.h"
#include "DbEntity.h"
#include "Ge/GeSphere.h"

class OdEdCommandContext;

class ExSphere : public OdDbEntity
{
  static const int lastKnownVersion;
public:
  ODDB_DECLARE_MEMBERS(ExSphere);

	ExSphere();
	virtual ~ExSphere();
  static int getVersion();

  OdGePoint3d center() const;
	void setCenter(const OdGePoint3d& centre);
	double radius() const;
	void setRadius(double rad);

  bool subWorldDraw(OdGiWorldDraw* pWd) const;
  OdResult dwgInFields(OdDbDwgFiler* pFiler);
  void dwgOutFields(OdDbDwgFiler* pFiler) const;
  OdResult dxfInFields(OdDbDxfFiler* pFiler);
  void dxfOutFields(OdDbDxfFiler* pFiler) const;

private:
  OdGeSphere m_impl;
};

typedef OdSmartPtr<ExSphere> ExSpherePtr;

inline int ExSphere::getVersion()
{
  return lastKnownVersion;
}


#endif // !defined(AFX_EXSPHERE_H__335BDC0F_7675_4177_841E_EB995F14FD37__INCLUDED_)
