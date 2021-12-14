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

#include "OdaCommon.h"

#include "DbDataLinkWin.h"
#include "RxDictionary.h"
#include "DbDataAdapter.h"

ODRX_DEFINE_DYNAMIC_MODULE(DbDataLinkWin);

void DbDataLinkWin::initApp()
{
  OdDbDataLink::desc()->addX(OdDbDataLinkUpdatePE::desc(), &m_link);
}

void DbDataLinkWin::uninitApp()
{
  OdDbDataLink::desc()->delX(OdDbDataLinkUpdatePE::desc());
}

OdError OdDbDataLinkUpdatePEImpl::dataLinkUpdate(OdDbDataLink* pDataLink, OdDb::UpdateDirection nDir, OdDb::UpdateOption nOption)
{
  DbDataAdapter adapter;
  return adapter.updateLink(pDataLink, nDir, nOption);
}

OdError  OdDbDataLinkUpdatePEImpl::repathSourceFiles(OdString& sBasePath, OdDb::PathOption nOption, OdString& path)
{
  DbDataAdapter adapter;
  return adapter.repathSourceFiles(sBasePath, nOption, path);
}
