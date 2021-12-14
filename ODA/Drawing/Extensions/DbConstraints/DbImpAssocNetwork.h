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

#ifndef DbImpAssocNetwork_INCLUDED_
#define DbImpAssocNetwork_INCLUDED_ /*!DOM*/

#include "DbConstraintsExports.h"
#include "DbImpAssocAction.h"

#include "TD_PackPush.h"

/** \details
  This class represents the custom implementation of the network of associated actions.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdDbImpAssocNetwork : public OdDbImpAssocAction
{
public: 
  /** \details
    Constructor creates an instance of this class.
  */
  explicit OdDbImpAssocNetwork();

  /** \details
    \returns array of object IDs of owned by this network.
  */
  OdDbObjectIdArray getActions() const;

  /** \details
    \returns array of the actions marked to be evaluated.
  */
  OdDbObjectIdArray getActionsToEvaluate() const;

  /** \details
    Reports the actions, dependencies and objects that need to be evaluated.
    \param pThisNetwork [in] associative network object owning this implementation object.
    \param pActionsToEvaluateCallback [in,callback] callback interface to report objects that need to be evaluated.
  */
  virtual void getDependentActionsToEvaluate(const OdDbAssocAction* pThisNetwork, OdDbActionsToEvaluateCallback* pActionsToEvaluateCallback) const;

  /** \details
    Adds the associated action to be owned by this network using its ID.
    \param thisNetwork [in] associative network object owning this implementation object.
    \param actionId [in] object ID of the action to add to network object.
    \param alsoSetAsDatabaseOwner [in] if true this network object will be set also as database owner of the action.
  */
  OdResult addAction(OdDbAssocNetwork* thisNetwork, const OdDbObjectId& actionId, bool alsoSetAsDatabaseOwner);

  /** \details
    Removes the associated action from this network using its ID.
    \param thisNetwork [in] associative network object owning this implementation object.
    \param actionId [in] object ID of the action to bee removed.
    \param alsoEraseIt [in] if true the action will be erased.
  */
  OdResult removeAction(OdDbAssocNetwork* thisNetwork, const OdDbObjectId& actionId, bool alsoEraseIt);

#if 0 // TODO: Nothing complex but needs time

  /** \details
    Adds an array of associated actions to be owned by this network.
  */
  OdResult addActions(const OdDbObjectIdArray& actionIds, bool alsoSetAsDatabaseOwner);

  /** \details
    Removes an array of associated actions from this network.
  */
  OdResult removeAllActions(bool alsoEraseThem);
#endif

  /** \details
    This method is called from the setStatus(..., true) method of the associated action and 
    notifies the network owing the action that the status has just been changed.
    \param pThisNetwork [in] associative network object owning this implementation object.
    \param pOwnedAction [in] the action owned by the network object which status is Changed.
    \param previousStatus [in] previous value of dependency status
  */
  OdResult ownedActionStatusChanged(OdDbObject* pThisNetwork, OdDbAssocAction* pOwnedAction, OdDbAssocStatus previousStatus);

  /** \details
    Reads the .dwg file data of this object. 

    \param pThisNetwork [in] associative network object owning this implementation object.
    \param pFiler [in]  Filer object from which data are read.
    
    \remarks
    Returns the filer status.
  */
  OdResult dwgInFields(OdDbAssocAction *pThisNetwork, OdDbDwgFiler* pFiler);

  /** \details
    Writes the .dwg file data of this object. 

    \param pFiler [in]  Pointer to the filer to which data are written.
    \param thisNetworkObjectId [in] object ID of associative network owning this implementation object
  */
  virtual void dwgOutFields(OdDbDwgFiler* pFiler, OdDbObjectId thisNetworkObjectId) const;

  /** \details
    Reads the .dxf file data of this object. 

    \param pFiler [in]  Filer object from which data are read.
    \param thisNetworkObjectId [in] object ID of associative network owning this implementation object
    
    \remarks
    Returns the filer status.
  */
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler, OdDbObjectId thisNetworkObjectId);

  /** \details
    Writes the .dxf file data of this object. 

    \param pFiler [in]  Pointer to the filer to which data are written.
    \param thisNetworkObjectId [in] object ID of associative network owning this implementation object
  */
  virtual void dxfOutFields(OdDbDxfFiler* pFiler, OdDbObjectId thisNetworkObjectId) const;

  /** \details
    After loading from file, performs necessary actions which require accessing other Database objects.
    For example, processing round-trip data.

    \param format [in]  File type.
    \param version [in]  Drawing version of file loaded.
    \param pAuditInfo [in]  Pointer to an OdDbAuditInfo object. Can be Null. Not Null if drawing is being loaded in Recover mode.
    If so, audit() will be called later.
  */
  virtual void composeForLoad(OdDbObject* pObj, OdDb::SaveType format, OdDb::DwgVersion version, OdDbAuditInfo* pAuditInfo);

  /** \details
    Sets the status of the network of associated actions.
    \param thisNetwork [in] associative network object owning this implementation object.
    \param newStatus [in] new value of assoc.status to set.
    \param notifyOwningNetwork [in] tels to call owning network's ownedActionStatusChanged.
    \param setInOwnedActions [in] tels to set the same status to owned actions if any.
  */
  OdResult setStatus(OdDbObject* thisNetwork,
                     OdDbAssocStatus newStatus,
                     bool notifyOwningNetwork,
                     bool setInOwnedActions);

  /** \details
    Evaluates the network of associated actions.
    \param pThisNetwork [in] associative network owning this implementation object
    \param pEvaluationCallback [in,callback] evaluation callback.
  */
  virtual void evaluate(OdDbAssocAction *pThisNetwork, OdDbAssocEvaluationCallback* pEvaluationCallback);

  /** \details
    \param pThisNetwork [in] action owning this implementation object
    \param pAuditInfo [in] context object for current audit operation.
    \sa
    * OdDbObject::audit()
  */
  virtual void audit(OdDbObject* pThisNetwork, OdDbAuditInfo* pAuditInfo);

  /** \details
    This callback appends this action to the specified owner object
    at the end of the deep and/or wblock clone operation.

    \param pThisNetwork [in] associative network owning this implementation object
    \param idPair [in]  Original-to-clone ID mapping record for this clone.
    \param pOwner [in]  Pointer to the owner object.
    \param idMap [in] id mapping object being used in of current cloning operation.

    \sa
    * OdDbAssocAction::appendToOwner()
  */
  virtual void appendToOwner(OdDbAssocAction* pThisNetwork, OdDbIdPair& idPair, OdDbObject* owner, OdDbIdMapping& idMap);

  /** \details
    This override checks if owning block was successfully cloned and if not then erases this network object.
  */
  OdResult postProcessAfterDeepClone(OdDbAssocAction *pAction, OdDbIdMapping& idMap);

  /** \details
    \returns the evaluation priority of this associative network object.
  */
  virtual OdDbAssocEvaluationPriority evaluationPriority() const;
private:
  /*!DOM*/
  void auditEvaluationQueue(OdDbObject* pObj, OdDbAuditInfo* pAuditInfo);
  /*!DOM*/
  OdDbObjectIdArray       m_actions;
  /*!DOM*/
  OdArray<bool>           m_isOwnedAction;
protected:
  /*!DOM*/
  OdUInt32 m_maxChildActionIdx;
  /*!DOM*/
  virtual void fixStatusConsistency(OdDbAssocAction* self);
  /*!DOM*/
  bool m_fixStatusConsistency;

  /*!DOM*/
  friend void odaaFixStatusConsistencyOnce(OdDbAssocNetwork* action);
  /*!DOM*/
  friend void odaaEnableStatusConsistencyCheck(OdDbAssocNetwork* network, bool doIt);
private:
  /*!DOM*/
  OdDbObjectIdArray       m_actionsToEvaluate;

};

/*!DOM*/
void odaaEnableStatusConsistencyCheck(OdDbAssocNetwork* network, bool doIt = true);
/*!DOM*/
void odaaFixStatusConsistencyOnce(OdDbAssocNetwork* action);

#include "TD_PackPop.h"

#endif
