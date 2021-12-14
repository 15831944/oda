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




#ifndef _ODDBREGAPPTABLE_INCLUDED
#define _ODDBREGAPPTABLE_INCLUDED

#include "TD_PackPush.h"

#include "DbSymbolTable.h"
#include "DwgDeclareMembers.h"

class OdDbRegAppTableRecord;


/** \details
    This class implements the RegAppTable, which represents registered application names
    for Extended Entity Data in an OdDbDatabase instance.

    
    <group OdDb_Classes>
*/
class TOOLKIT_EXPORT OdDbRegAppTable: public OdDbSymbolTable
{
public:
  DWGMAP_DECLARE_MEMBERS(OdDbRegAppTable);

  /** \details
    Client applications typically will not use this constructor, insofar as 
    this class is a base class.
  */
  OdDbRegAppTable();

  virtual OdResult subGetClassID(
    void* pClsid) const;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDbRegAppTable object pointers.
*/
typedef OdSmartPtr<OdDbRegAppTable> OdDbRegAppTablePtr;

#include "TD_PackPop.h"

#endif // _ODDBREGAPPTABLE_INCLUDED
