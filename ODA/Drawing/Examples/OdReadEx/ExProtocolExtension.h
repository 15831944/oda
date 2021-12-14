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

// ExProtocolExtension.h: interface for the ExProtocolExtension class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXPROTOCOLEXTENSION_H__A6B44366_0CE0_408A_BFDF_BAE98BD49250__INCLUDED_)
#define AFX_EXPROTOCOLEXTENSION_H__A6B44366_0CE0_408A_BFDF_BAE98BD49250__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RxObject.h"
#include "DbEntity.h"

#define STL_USING_IOSTREAM
#include "OdaSTL.h"
#define  STD(a)  std:: a

#include "DbDumper.h"

/************************************************************************/
/* This class is the base class for all entity dumpers                  */
/************************************************************************/
class OdDbEntity_Dumper : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdDbEntity_Dumper);

  virtual void dump(OdDbEntity* pEnt, int indent) const;
};

/************************************************************************/
/* This class is the base class for all object dumpers                  */
/************************************************************************/
class OdDbObject_Dumper : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdDbObject_Dumper);

  virtual void dump(OdDbObject* pObj, int indent) const;
};

class Dumpers;
  
/************************************************************************/
/* This class is the protocol extension class for all entity dumpers    */
/************************************************************************/
class ExProtocolExtension  
{
  Dumpers* m_pDumpers;
public:
  ExProtocolExtension();
  virtual ~ExProtocolExtension();
    virtual void initialize();
    virtual void uninitialize();
};

#endif // !defined(AFX_EXPROTOCOLEXTENSION_H__A6B44366_0CE0_408A_BFDF_BAE98BD49250__INCLUDED_)
