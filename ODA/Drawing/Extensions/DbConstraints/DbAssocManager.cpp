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
#include "DbAssocManager.h"
#include "DbAssocNetwork.h"
#include "DbFiler.h"
#include "DbDictionary.h"
#include "DbAssocDependency.h"
#include "OdHashMap.h"
#include "DbAssocNetCloneCtx.h"
#include "DbImpAssocNetwork.h"
#include "DbAssocActionModule.h"


/*
ODRX_DEFINE_MEMBERS_EX(OdDbAssocManager,     // ClassName
  OdDbObject,           // ParentClass
  DBOBJECT_CONSTR,      // DOCREATE
  OdDb::kDHL_1021,      // DwgVer
  OdDb::kMRelease6,     // MaintVer
  1025,                 // nProxyFlags (kEraseAllowed | kDisableProxyWarning)
  L"AcDbAssocManager",  // DWG class name
  L"ACDBASSOCMANAGER",  // DxfName
  L"ObjectDBX Classes", // AppName
  OdRx::kMTLoading | OdRx::kHistoryAware);
*/

OdDbAssocManager::OdDbAssocManager()
{
   
}

/* Sets action status to 'changed directly' if the action has 'up-to-date' status and has one or more dirty dependencies.
   network stores a flag to run this only once after drawing was loaded */
void odaaFixStatusConsistencyOnce(OdDbAssocNetwork* network) {
  OdDbImpAssocNetwork* netImp = static_cast<OdDbImpAssocNetwork*>(OdDbImpAssocAction::getImpObject(network));
  if(netImp->m_fixStatusConsistency)
    netImp->fixStatusConsistency(network);
}

/* Sets action status to 'changed directly' if the action has 'up-to-date' status and has one or more dirty dependencies. */
void odaaFixStatusConsistency(OdDbAssocAction* action) {
  OdDbImpAssocAction::getImpObject(action)->fixStatusConsistency(action);
}

bool OdDbAssocManager::evaluateTopLevelNetwork( OdDbDatabase *pDb, 
                                                OdDbAssocEvaluationCallback *pCallback /*= NULL*/, 
                                                int callbackOrder /*= 0*/ )
{
  OdDbObjectId networkId = OdDbAssocNetwork::getInstanceFromDatabase( pDb, false);
  OdDbAssocNetworkPtr pNetwork = OdDbAssocNetwork::cast(networkId.openObject(OdDb::kForWrite));
  OdDbAssocStatus status;
  if ( pNetwork.get() ) {
    ::odaaFixStatusConsistencyOnce(pNetwork);
    pDb->flushAllTransactedChanges();
    if ( isEvaluationRequest(status = pNetwork->status()) || status == kErasedAssocStatus ) {
      pNetwork->evaluate(pCallback);
      if (pNetwork->status() == kErasedAssocStatus)
        pNetwork->erase();
      return true;
    }
  }
  return false;
}

bool OdDbAssocManager::isActionEvaluationInProgress(const OdDbDatabase* db) {
  OdDbObjectId netid = OdDbAssocNetwork::getInstanceFromDatabase(const_cast<OdDbDatabase*>(db), false);
  OdDbAssocNetworkPtr net = netid.openObject();
  if(net.get())
    return net->isActionEvaluationInProgress();
  return false;
}

void OdDbAssocManager::enableAutoEvaluation(bool doIt) {
  OdDbAssocActionModulePtr aam = ::odrxSafeLoadApp(DbAssocActionModuleName);
  aam->enableAutoEvaluation(doIt);
}

bool OdDbAssocManager::autoEvaluationEnabled() {
  OdDbAssocActionModulePtr aam = ::odrxSafeLoadApp(DbAssocActionModuleName);
  return aam->autoEvaluationEnabled();
}
