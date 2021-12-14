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
/* ExCustObj.h: interface for the ExCustObj class                       */
/************************************************************************/
#if !defined(AFX_EXAMPLECUSTOBJECT_H__B6D0433A_D5F7_447A_95E1_F6EAC04777CA__INCLUDED_)
#define AFX_EXAMPLECUSTOBJECT_H__B6D0433A_D5F7_447A_95E1_F6EAC04777CA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DbObject.h"
#include "ExCustObjExport.h"


class EXCUSTOBJEXPORT ExCustObject : public OdDbObject
{
  static const int lastKnownVersion;
  int m_nCustValue;
  OdString m_string;

public:
  // Macro to declare
  ODDB_DECLARE_MEMBERS(ExCustObject);

  ExCustObject();
  virtual ~ExCustObject();

  static int getVersion();

  void setValue(int n);
  int getValue() const;

  void setString(const OdString&);
  OdString getString() const;

  // Methods to override
  OdResult dwgInFields(OdDbDwgFiler* pFiler);
  void dwgOutFields(OdDbDwgFiler* pFiler) const;
  OdResult dxfInFields(OdDbDxfFiler* pFiler);
  void dxfOutFields(OdDbDxfFiler* pFiler) const;
};

typedef OdSmartPtr<ExCustObject> ExCustObjectPtr;

#endif // !defined(AFX_EXAMPLECUSTOBJECT_H__B6D0433A_D5F7_447A_95E1_F6EAC04777CA__INCLUDED_)
