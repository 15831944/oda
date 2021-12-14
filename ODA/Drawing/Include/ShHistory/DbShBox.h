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

#ifndef ODDB_DBSHBOX_H
#define ODDB_DBSHBOX_H

#include "TD_PackPush.h"
#include "DbShPrimitive.h"

/** \details

        <group OdDbSh_Classes> 
*/
class DB3DSOLIDHISTORY_EXPORT OdDbShBox : public OdDbShPrimitive
{
public:
  ODDB_DECLARE_MEMBERS(OdDbShBox);
  OdDbShBox();

  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler);  
  virtual void dwgOutFields(OdDbDwgFiler* pFiler) const;  
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);  
  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;

  double length() const;
  double width() const;
  double height() const;
  OdResult setLength(double dLenght);
  OdResult setWidth(double dWidth);
  OdResult setHeight(double dHeight);
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDbShBox object pointers.
*/
typedef OdSmartPtr<OdDbShBox> OdDbShBoxPtr;

#include "TD_PackPop.h"

#endif

