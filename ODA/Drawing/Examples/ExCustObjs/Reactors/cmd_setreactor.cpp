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
/* cmd_setreactor.cpp -- Adds or removes the specified debug reactors   */
/************************************************************************/
#include "OdaCommon.h"
#include "Ed/EdUserIO.h"
#include "DbCommandContext.h"
#include "DbSSet.h"
#include "DbDatabase.h"
#include "cmd_setreactor.h"

/************************************************************************/
/* Create Set Reactor command                                           */
/************************************************************************/
void _setreactor_cmd::execute(OdEdCommandContext* pCmdCtx)
{
  /**********************************************************************/
  /* Establish CommandContext, Database, and HostAppServices, & UserIO */
  /**********************************************************************/
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbHostAppServices* pHostApp = pDb->appServices();
  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();

  /**********************************************************************/
  /* Prompt for reactor type                                            */
  /**********************************************************************/
  OdUInt32 nReactorType = pIO->getKeyword(L"Enter type of record [Object/Database/Transaction/RxEvent/Editor/LongTansaction]:", 
    L"Object Database Transaction RxEvent Editor LongTansaction");

  /**********************************************************************/
  /* Prompt for Add/Remove                                              */
  /**********************************************************************/
  bool bAdd = pIO->getKeyword(OD_T("Add/Remove <Add>:"), OD_T("Add Remove"), 0) == 0;
  switch (nReactorType)
  {
  case 0: //Object
    {
      /******************************************************************/
      /* Initialize the DbgObjectReactor                                */
      /******************************************************************/
      m_ObjReactor.init(pHostApp);
      
      /******************************************************************/
      /* Interactively create a selection set and return an iterator    */
      /******************************************************************/
      OdDbSelectionSetIteratorPtr pIter =
        pIO->select(OD_T("Select objects <done>:"), OdEd::kSelAllowInactSpaces|OdEd::kSelAllowObjects|OdEd::kSelAllowPSVP)->newIterator();
      
      /******************************************************************/
      /* Remove any reactors from objects in the selection set and add  */
      /* new ones as required.                                          */
      /******************************************************************/
      while (!pIter->done())
      {
        OdDbObjectId objId = pIter->objectId();
        OdDbObjectPtr pObj = objId.openObject();
        if (pObj.get())
        {
          pObj->removeReactor(&m_ObjReactor);
          if (bAdd) pObj->addReactor(&m_ObjReactor);
        }
        pIter->next();
      }
    }
    break;
  case 1: //Database
    /********************************************************************/
    /* Initialize the DbgDatabaseReactor                                */
    /********************************************************************/
    m_DbReactor.init(pHostApp);

    /********************************************************************/
    /* Remove any reactor from the database, add a new one as required  */
    /********************************************************************/
    pDb->removeReactor(&m_DbReactor);
    if (bAdd) pDb->addReactor(&m_DbReactor);
    break;
  case 2: //Transaction

    /********************************************************************/
    /* Initialize the DbgTransactionReactor                             */
    /********************************************************************/
    m_TransReactor.init(pHostApp);

    /********************************************************************/
    /* Remove any transaction reactor from the database, and add a      */
    /* new one as required                                              */
    /********************************************************************/
    pDb->removeTransactionReactor(&m_TransReactor);
    if (bAdd) pDb->addTransactionReactor(&m_TransReactor);
    break;
  case 3: //rxEvent
    /********************************************************************/
    /* Initialize the DbgEventReactor                                   */
    /********************************************************************/
    m_EditorReactor.init(pHostApp);
    /********************************************************************/
    /* Remove any event reactor from the database, and add a            */
    /* ones as required                                                 */
    /********************************************************************/
    odrxEvent()->removeReactor(&m_EditorReactor);
    if (bAdd) odrxEvent()->addReactor(&m_EditorReactor);
    break;
  case 4: //Editor
    /********************************************************************/
    /* Initialize the DbgEventReactor                                   */
    /********************************************************************/
    m_EditorReactor.init(pHostApp);
    /********************************************************************/
    /* Remove any event reactor from the database, and add a            */
    /* ones as required                                                 */
    /********************************************************************/
    odedEditor()->removeReactor(&m_EditorReactor);
    if (bAdd) odedEditor()->addReactor(&m_EditorReactor);
    break;
  case 5:
    m_LongTransactionReactor.init(pIO);
    odapLongTransactionManager()->removeReactor(&m_LongTransactionReactor);
    if (bAdd) 
      odapLongTransactionManager()->addReactor(&m_LongTransactionReactor);
    break;
  }
}

/************************************************************************/
/* Destructor -- Remove event reactor                                   */
/************************************************************************/
_setreactor_cmd::~_setreactor_cmd()
{
  odapLongTransactionManager()->removeReactor(&m_LongTransactionReactor);
  odrxEvent()->removeReactor(&m_EditorReactor);
  odedEditor()->removeReactor(&m_EditorReactor);
}
