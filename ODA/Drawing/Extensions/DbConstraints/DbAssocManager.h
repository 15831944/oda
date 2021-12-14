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

#ifndef DbAssocManager_INCLUDED_
#define DbAssocManager_INCLUDED_ 

#include "DbConstraintsExports.h"
#include "DbAssocGlobal.h"
#include "DbObject.h"

#include "TD_PackPush.h"

/** \details
  This class represents the association manager.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdDbAssocManager : public OdDbObject
{
public: 
    // ODRX_DECLARE_MEMBERS(OdDbAssocManager);

    /** \details
      Constructor creates an instance of this class.
    */
    explicit OdDbAssocManager();

    /** \details
      Evaluates the top level for network.
    */
    static bool evaluateTopLevelNetwork( OdDbDatabase *pDb, 
                                         OdDbAssocEvaluationCallback *pCallback = NULL, 
                                         int callbackOrder = 0);
    
    /** \details
      Checks whether the system is evaluating an action or a network of actions.
    */
    static bool isActionEvaluationInProgress(const OdDbDatabase* db);

    /** \details
      Request to evaluate action, dependency or arbitrary object (all dependencies on it).
    */
    static OdResult requestToEvaluate( const OdDbObjectId& objectId,
      OdDbAssocStatus newStatus = kChangedDirectlyAssocStatus, bool ownedActionsAlso = true);

    /** \details
      Enables or disables evaluation of top level network on editor reactor events.
    */
    static void enableAutoEvaluation(bool doIt = true);

    /** \details
      \returns true if evaluation of top level network on editor reactor events is enabled.
    */
    static bool autoEvaluationEnabled();
}; 
#include "TD_PackPop.h"

#endif
