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

#ifndef ODDB_DBSHPRIMITIVE_H
#define ODDB_DBSHPRIMITIVE_H

#include "TD_PackPush.h"

#include "DbShHistoryNode.h"

/** \details

        <group OdDbSh_Classes> 
*/
class DB3DSOLIDHISTORY_EXPORT OdDbShPrimitive : public OdDbShHistoryNode
{
public:
  ODDB_DECLARE_MEMBERS(OdDbShPrimitive);
  OdDbShPrimitive();

  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler);  
  virtual void dwgOutFields(OdDbDwgFiler* pFiler) const;  
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);  
  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;
  /** \details
  Creates a viewport-independent geometric representation of this object.

  \param pWd [in]  Pointer to the OdGiWorldDraw interface.
  Returns true if and only if the geometric representation can be generated in a
  viewport-independent manner.
  */
  virtual bool subWorldDraw(OdGiWorldDraw* pWd) const;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDbShPrimitive object pointers.
*/
typedef OdSmartPtr<OdDbShPrimitive> OdDbShPrimitivePtr;

#include "TD_PackPop.h"

#endif

