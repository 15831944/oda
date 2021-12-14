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
/* ExDcObject.cpp: implementation of the ExDcObject class               */
/************************************************************************/
#include "OdaCommon.h"
#include "ExDcObject.h"


ODRX_DXF_DEFINE_MEMBERS(ExDcObject,                                                      // ClassName
                        ExCustObject,                                                    // ParentClass
                        DBOBJECT_CONSTR,                                                 // DOCREATE
                        OdDb::vAC15,                                                     // DwgVer
                        OdDb::kMRelease0,                                                // MaintVer
                        0,                                                               // nProxyFlags
                        EXDCOBJECT,                                                      // DxfName
                        ExDeepCloning|Description: Teigha Run-time Extension Example) // AppName


ExDcObject::ExDcObject()
{
}

ExDcObject::~ExDcObject()
{
}


/************************************************************************/
/* Overridden deep cloning methods.                                     */
/************************************************************************/
OdDbObjectPtr ExDcObject::subDeepClone(OdDbIdMapping& idMap, OdDbObject* owner, bool bPrimary) const
{
  // Use inherited implementation
  // See ExDcEntity overridden method for custom implementation.
  return ExCustObject::subDeepClone(idMap, owner, bPrimary);
}

OdDbObjectPtr ExDcObject::subWblockClone(OdDbIdMapping& idMap, OdDbObject* owner, bool bPrimary) const
{
  // Use inherited implementation
  // See ExDcEntity overridden method for custom implementation.
  return ExCustObject::subWblockClone(idMap, owner, bPrimary);
}
