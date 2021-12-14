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
//
// DwfDbCommands.cpp
//

#include "OdaCommon.h"
#include "DwfDbModuleImpl.h"
#include "DbHostAppServices.h"
#include "Ed/EdCommandContext.h"
#include "Ed/EdUserIO.h"
#include "PropServices.h"
#include "RxVariantValue.h"
#include "DwfDatabase.h"

//////////////////////////////////////////////////////////////////////////
//
// Cloud keep database via command context implementation - strange solution
// (see addref at init and release in destructor of class DwgEdContext for example)
//

class ExDbContextDbHolder : public ExDbCommandContext
{
public:
  void init(OdEdBaseIO* pBaseIO, OdRxObject* pRxDb)
  {
    ExDbCommandContext::reset(pBaseIO, pRxDb);
    ODA_ASSERT_ONCE(baseDatabase());
    if (!baseDatabase())
      throw OdError(eNoDatabase);
    baseDatabase()->addRef();
  }

  ~ExDbContextDbHolder()
  {
    if (baseDatabase())
    {
      // OdDwfGsLayoutHelperInt should be destroyed earlier than Dwf database
      // (contains removeReactor but does not hold database - like Bim)
      setArbitraryData(L"Initialized GS", NULL); 

      baseDatabase()->release();
    }
  }
};

//////////////////////////////////////////////////////////////////////////
// Cloud support (instead Cloud/Source/DwfCtx)

void OdqCmd_OpenDbFunc::execute(OdEdCommandContext* pCtx)
{ 
  // 0:edBaseIO, 1:path, 2:partial, 3:password
  OdRxObjectPtrArray args = OdRxVariantValue(pCtx->funcIO()->param());
# define ARG(i) OdRxVariantValue(args[i])
  OdSmartPtr<OdEdBaseIO> pBaseIO = ARG(0);
  ODA_ASSERT_VAR(ExDbCommandContext* pDbCtx = reinterpret_cast<ExDbCommandContext*>(pCtx);)
  ODA_ASSERT_VAR(OdEdBaseIO* pBaseIO_ = pDbCtx->baseIO();)
  ODA_ASSERT_ONCE(pBaseIO.get() == pBaseIO_);
  OdString path = ARG(1);
  bool partialLoad = ARG(2);
  OdPassword sPassword = ARG(3);
  //OdDbBaseHostAppServices* pBaseAppServ = NULL; // TODO
  //OdExtDbModule_cast(::odrxDynamicLinker()->loadModule(L"TD_DwfDb.tx"))->setBaseHostAppServices(pBaseAppServ);

  OdDwfDatabasePtr pDwfDb = appDwfServices()->readFile(path, partialLoad, sPassword);

  //OdDbCommandContextPtr pDwfCtx = ExDbCommandContext::createObject(pBaseIO, pDwfDb);
  OdEdCommandContextPtr pCmdCtx = OdRxObjectImpl<ExDbContextDbHolder, OdEdCommandContext>::createObject();
  ExDbContextDbHolder* pDwfCtx = static_cast<ExDbContextDbHolder*>(pCmdCtx.get()); // OdDbCommandContext::cast(pCmdCtx).get());
  pDwfCtx->init(pBaseIO, pDwfDb);

  pCtx->funcIO()->setResult(pCmdCtx);
}
