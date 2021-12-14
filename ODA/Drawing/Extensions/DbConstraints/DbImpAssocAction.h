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

#ifndef DbImpAssocAction_INCLUDED_
#define DbImpAssocAction_INCLUDED_ /*!DOM*/

#include "DbConstraintsExports.h"
#include "DbAssocGlobal.h"
#include "DbEntity.h"     // Because of DragStat enum
#include "DbImpAssocActionBody.h"
#include "DbAssocActionBody.h"

#include "TD_PackPush.h"


/** \details
  This class represents the custom implementation of an associated actions.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdDbImpAssocAction
{
public:     
    /** \details
      Constructor creates an instance of this class.
    */
    OdDbImpAssocAction();

    /** \details
      Destructor destroys an instance of this class.
    */
    virtual ~OdDbImpAssocAction();

    /** \details
      \param pAction [in] the action object to query implementation of
      \returns a pointer to the implementation object associated with action object.
    */
    static OdDbImpAssocAction* getImpObject(OdDbAssocAction *pAction);

    /** \details
      Returns an object ID of the owned action body instance. 
    */
    virtual OdDbObjectId actionBody() const;
    
    /** \details
      Sets the owned action body instance using an object ID.
      \param thisActionId [in] object ID of action owning this implementation object.
      \param actionBodyId [in] to set.
    */
    virtual OdResult setActionBody(OdDbObjectId& thisActionId, const OdDbObjectId& actionBodyId);

    /** \details
      \returns the current status of this action. 
    */
    virtual OdDbAssocStatus status() const;

    /** \details
      Sets the status for this action. 
      \param pThisAction [in] action owning this implementation object
      \param newStatus [in] to set.
      \param notifyOwningNetwork [in] tels to call owning network's ownedActionStatusChanged.
      \param setInOwnedActions [in] tels to set the same status to owned actions.
   */
    virtual OdResult setStatus(OdDbObject* pThisAction,
                               OdDbAssocStatus newStatus, 
                               bool notifyOwningNetwork = true,
                               bool setInOwnedActions   = false);

    /** \details
      \returns the logically owning association network, or kNull if it is the topmost. 
    */
    virtual OdDbObjectId owningNetwork() const;

    /** \details
      Sets the logically owning association network. 
      \param pThisAction [in] action owning this implementation object
      \param networkId [in] to set as logical owner of the action
      \param alsoSetAsDatabaseOwner [in] if true then the network object will be set also as database owner of the action.
    */
    virtual OdResult setOwningNetwork(OdDbAssocAction *pThisAction,
                                      const OdDbObjectId& networkId,
                                      bool alsoSetAsDatabaseOwner);

    /** \details
      Gets the dependencies of this action. 
      \param readDependenciesWanted [in] read dependencies wanted.
      \param writeDependenciesWanted [in] write dependencies wanted.
      \param dependencyIds [out] array will be filled with returning dependency IDs.
   */
    virtual OdResult getDependencies(bool readDependenciesWanted,
                                     bool writeDependenciesWanted,
                                     OdDbObjectIdArray& dependencyIds) const;

    /** \details
      Adds a dependency for this action. 
      \param dependencyId [in] ID of the dependency to add
      \param setThisActionAsOwningAction [in] tells to set this action as owning action of the dependency.
      \param pThisAction [in] action owning this implementation object
    */
    virtual OdResult addDependency(const OdDbObjectId& dependencyId, 
                                   bool setThisActionAsOwningAction,
                                   OdDbAssocAction *pThisAction);

    /** \details
      Removes a dependency from this action. 
      \param dependencyId [in] ID of the dependency to remove
      \param alsoEraseIt [in] tells to erase the dependency from database.
    */
    virtual OdResult removeDependency(const OdDbObjectId& dependencyId, 
                                      bool alsoEraseIt);

    /** \details
      Clears all dependencies from this action. 
      \param alsoEraseIt [in] tells to erase removed dependencies from database.
    */
    virtual OdResult removeAllDependencies(bool alsoEraseThem);

    /** \details
      Gets an array of objects which this action depends on or modifies. 
      \param readDependenciesWanted [in] read dependencies wanted.
      \param writeDependenciesWanted [in] write dependencies wanted.
      \param objectIds [out] returned dependent objects.
    */
    virtual OdResult getDependentObjects(bool readDependenciesWanted,
                                 bool writeDependenciesWanted,
                                 OdDbObjectIdArray& objectIds) const;
#if 0
    /** \details
      Checks whether the given dependency is owned by this action, either directly, 
      or if this action is a network. 
    */
    virtual bool isOwnedDependency(const OdDbAssocDependency* pDependency) const;

    /** \details
      Checks whether the given dependency owned by this action, is external to this action. 
    */
    virtual bool isExternalDependency(const OdDbAssocDependency* pDependency) const;
#endif
    /** \details
      Checks whether the dependent object change is relevant. 
      \param pDependency [in] dependency to check.
    */
    virtual bool isRelevantDependencyChange(const OdDbAssocDependency* pDependency) const;
#if 0
    /** \details
      Checks whether a dependency has a cached value. It is delegated from the owned 
      dependency's predicate. 
    */
    virtual bool hasDependencyCachedValue(const OdDbAssocDependency* pDependency) const;

    /** \details
      Checks whether two depedencies depend on the same thing and allows the owning action, 
      instead of the dependency itself.
    */
    virtual bool areDependenciesOnTheSameThing(const OdDbAssocDependency* pDependency1, 
                                               const OdDbAssocDependency* pDependency2) const;

    /** \details
      Checks whether two dependencies are equal and allows the owning action, instead of 
      the dependency itself.
    */
    virtual bool areDependenciesEqual(const OdDbAssocDependency* pDependency1, 
                                      const OdDbAssocDependency* pDependency2) const;

#endif
    /** \details
      Dependency notifies the owning action when dependent-on object has been cloned.
      Class overriding this callback must call this base implementation in its implementation
      for proper working of deep clone operations.
      \param pDependency [in] dependency attached to original object.
      \param pOriginal [in] original object.
      \param pClone [in] clone object.
    */
    virtual void dependentObjectCloned(const OdDbAssocDependency* pDependency, 
                                       const OdDbObject* pOriginal, 
                                       const OdDbObject* pClone);

    /** \details
      Tells to action being cloned to specify related objects that should be cloned
      because this action is cloned.
      \param pThisAction [in] action owning this implementation object
      \param idMap [in] id mapping object being used in of current cloning operation.
      \param additionalObjectsToClone [out] array to return additional objects if any.
    */
    virtual OdResult addMoreObjectsToDeepClone(const OdDbAssocAction* pThisAction, OdDbIdMapping& idMap, 
                                       OdDbObjectIdArray& additionalObjectsToClone) const;


    /** \details
      Informs the source actions of the cloning object so that they can update the clones or do other 
      action if it is necessary. This method is called from reactor notification.
      \param pThisAction [in] action owning this implementation object
      \param idMap [in] id mapping object being used in of current cloning operation.
    */
    virtual OdResult postProcessAfterDeepClone(OdDbAssocAction *pThisAction, OdDbIdMapping& idMap); 

    /** \details
      Informs the source actions so that it can obtain its clone. This method is called from reactor 
      notification.
      \param pThisAction [in] action owning this implementation object
      \param idMap [in] id mapping object being used in of current cloning operation.
    */
    virtual OdResult postProcessAfterDeepCloneCancel(OdDbAssocAction *pThisAction, OdDbIdMapping& idMap);

    /** \details
      Checks whether the system is evaluating an action or a network of actions.
    */
    virtual bool isActionEvaluationInProgress() const;

    /** \details
      \returns the current association evaluation callback set instance, or NULL if no action evaluation 
      is in progress.
    */
    virtual OdDbAssocEvaluationCallback* currentEvaluationCallback() const;

    /** \details
      Aggregates all dependencies of the action evaluates the ones the need to be evaluated.
    */
    virtual OdResult evaluateDependencies();

    /** \details
      Allows the owning action, instead of the dependency itself, to be performed.
      \param pDependency [in] to evaluate
    */
    virtual void evaluateDependency(OdDbAssocDependency* pDependency);

    /** \details
      Notifies the action owning the dependency that its status has been changed. The dependency 
      is still opened for write and already has the new status.
      \param pThisAction [in] action owning this implementation object
      \param pOwnedDependency [in] owned dependency which status has been change
      \param previousStatus [in] previous value of dependency status
    */
    virtual OdResult ownedDependencyStatusChanged(OdDbAssocAction* pThisAction,
                                                  OdDbAssocDependency* pOwnedDependency,
                                                  OdDbAssocStatus      previousStatus);

    /** \details
      Notifies the action that its owner is transformed. For example, all entities in the block owning 
      the network that owns the action are transformed.
      \param thisAction [in] action owning this implementation object
      \param transform [in] matrix that was used to transform
    */
    virtual OdResult transformActionBy(OdDbAssocAction* thisAction, const OdGeMatrix3d& transform);
#if 0
    /** \details
      Compares this action with the given action. Returns True if and only if they are equal.
    */
    virtual bool isEqualTo(const OdDbAssocAction* pOtherAction) const;
#endif
    /** \details
      \returns the evaluation priority of this action.
    */
    virtual OdDbAssocEvaluationPriority evaluationPriority() const;

    /** \details
      Asks the action to specify all other actions that need to be evaluated because this action is evaluated.
      \param pThisAction [in] action owning this implementation object
      \param pActionsToEvaluateCallback [in] this action should specify other actions (and/or objects) that need to be evaluated
    */
    virtual void getDependentActionsToEvaluate(const OdDbAssocAction* pThisAction, OdDbActionsToEvaluateCallback* pActionsToEvaluateCallback) const;

   /** \details
      Check every dependency if its change is relevant and call evaluate() on it if not.
      \returns true if action has any dependency with relevant change.
    */
    bool filterChanges_hasRelevantDependencyChange();

    /** \details
      Evaluates this action. This method is called from client code to evaluate an action or 
      an associative network.
      \param pThisAction [in] action owning this implementation object
      \param pEvaluationCallback [in] evaluation callback.
    */
    virtual void evaluate(OdDbAssocAction *pThisAction, OdDbAssocEvaluationCallback* pEvaluationCallback);

    /** \details
      \param pThisAction [in] action owning this implementation object
      \returns an ID of the object that owns the network that owns this action.
    */
    OdDbObjectId objectThatOwnsNetworkInstance(const OdDbAssocAction *pThisAction) const;
#if 0
    /** \details
      Notifies the action when the action depends on objects, either directly or indirectly.
    */
    virtual void dragStatus(const OdDb::DragStat status);
#endif

  /** \details
    Reads the .dwg file data of this object. 

    \param pFiler [in]  Filer object from which data are read.
    \returns the filer status.
  */
  virtual OdResult dwgInFields(OdDbAssocAction *self, OdDbDwgFiler* filer);

  /** \details
    Writes the .dwg file data of this object. 

    \param pFiler [in]  Pointer to the filer to which data are written.
  */
  virtual void dwgOutFields(OdDbDwgFiler* pFiler, OdDbObjectId objectId) const;

  /** \details
    Reads the .dxf file data of this object. 
    \param pFiler [in]  Filer object from which data are read.
    \returns the filer status.
  */
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler, OdDbObjectId objectId);

  /** \details
    Writes the .dxf file data of this object. 

    \param pFiler [in]  Pointer to the filer to which data are written.
  */
  virtual void dxfOutFields(OdDbDxfFiler* pFiler, OdDbObjectId objectId) const;

  /** \details
    After loading from file, performs necessary actions which require accessing other Database 
    objects. For example, processing round-trip data.
    \param pThisAction [in] action owning this implementation object
    \param format [in] of drawing object is loaded from.
    \param version [in] of drawing object is loaded from.
    \param pAuditInfo [in] audit context object.
    \sa
    * OdDbObject::composeForLoad()
 */
  virtual void composeForLoad(OdDbObject* pThisAction, OdDb::SaveType format, OdDb::DwgVersion version, OdDbAuditInfo* pAuditInfo );

  /** \details
    Determines the behavior for custom objects when saving to an earlier version of .dwg or .dxf file.
    \param pThisAction [in] action owning this implementation object
    \param format [in] of drawing object is going to be saved to.
    \param version [in] of drawing object is going to be saved to.
    \sa
    * OdDbObject::decomposeForSave()
  */
  virtual void decomposeForSave(OdDbObject* pThisAction, OdDb::SaveType format, OdDb::DwgVersion version);

  /** \details
    \param pThisAction [in] action owning this implementation object
    \param pAuditInfo [in] audit context object.
    \sa
    * OdDbObject::audit()
  */
  virtual void audit(OdDbObject* pThisAction, OdDbAuditInfo* pAuditInfo);

  /** \details
    This callback appends this action to the specified owner object
    at the end of the deep and/or wblock clone operation.

    \param pThisAction [in] action owning this implementation object
    \param idPair [in]  Original-to-clone ID mapping record for this clone.
    \param pOwner [in]  Pointer to the owner object.
    \param idMap [in] id mapping object being used in of current cloning operation.

    \remarks
    Unlike generic cloning process, for assoc. actions this callback is called
    just before postProcessAfterDeepClone() callback when IDs are alredy translated
    in the target database and associative data is audited so it's safe
    to use any hierarchy traversing methods in the target database.

    The default implementation appends this action to OdDbAssocNetwork
    but the OdDbBlockTableRecord object can be provided as the owner parameter.
    The implementation then just calls OdDbAssocNetwork::getInstanceFromObject()
    to obtain the network object to append this action to.
    If the owner object is neither OdDbAssocNetwork nor OdDbBlockTableRecord
    then OdDbObject::appendToOwner() is called.

    \sa
    * OdDbObject::appendToOwner()
  */
  virtual void appendToOwner(OdDbAssocAction* pThisAction, OdDbIdPair& idPair, OdDbObject* owner, OdDbIdMapping& idMap);

  /** \details
    Sets the index for this action.
    \param newIndex [in] new index to set.
  */
  OdResult setActionIndex(int newIndex);

  /** \details
    Gets the maximal index of the associated dependency.
  */
  int getMaximumAssocDepIndex();

  /** \details
    Sets the maximal index of the associated dependency for this action.
    \param index [in] maximal index to set.
  */
  void setMaximumAssocDepIndex(int index);

// AC2013
// OdDbAssocActionParams stuff

  /** \details
    Removes all owned action parameters and optionally erases them.
    \param bEraseThem [in] tells to erase removed parameters.
  */
  OdResult removeAllParams(bool bEraseThem);

  /** \details
    Returns the number of the owned action parameters.
  */
  int paramCount() const;

  /** \details
    \returns an array of the owned action parameters.
  */
  const OdDbObjectIdArray& ownedParams() const;

  /** \details
    Adds the action parameter.
    \param paramId [in] object ID of parameter object to add to action.
    \param paramIndex [out] returned index of added parameter.
  */
  OdResult addParam(const OdDbObjectId& paramId, int& paramIndex);

  /** \details
    Creates and adds a new action parameter.
    \param paramName [in] name for new parameter object.
    \param pParamClass [in] RX class of new parameter object to create.
    \param paramId [out] returned object ID of created and added parameter.
    \param paramIndex [out] returned index of added parameter.
    \param pThisAction [in] action owning this implementation object
  */
  OdResult addParam(const OdString& paramName, OdRxClass* pParamClass, 
                    OdDbObjectId& paramId, int& paramIndex, OdDbAssocAction *pThisAction);

//#if defined(DB_ASSOC_ACTION_PARAM_METHOGS_DEFINED)

//#endif

  /** \details
    Removes an existing action parameter and optionally erases it.
    \param paramId [in] object ID of parameter to remove.
    \param alsoEraseIt [in] tells to erase removed parameter.
  */
  OdResult removeParam(const OdDbObjectId& paramId, bool alsoEraseIt);

//#if defined(DB_ASSOC_ACTION_PARAM_METHOGS_DEFINED)

  /** \details
    \returns an array of object IDs of the owned action parameters with specified name.
    \param paramName [in] name of parameters to return.
  */
  OdDbObjectIdArray paramsAtName(const OdString& paramName) const;

  /** \details
    \param paramName [in] name of parameter object to return.
    \param paramIndex [in] index of parameter object to return.
    \returns an owned action parameter specified by its name and optional index for parameters with the same name.
  */
  OdDbObjectId paramAtName(const OdString& paramName, int index = 0) const;

//#endif

  /** \details
    \param paramIndex [in] index of parameter object to return.
    \returns an owned action parameter specified by its index.
  */
  OdDbObjectId paramAtIndex(int paramIndex) const;

//#if defined(DB_ASSOC_ACTION_PARAM_METHOGS_DEFINED)

  /** \details
    \param paramNames [out] string array to fill with names.
    \returns all the names of all value action parameters.
  */
  void ownedValueParamNames(OdStringArray& paramNames) const;

  /** \details
    Gets an array of values, array of expressions and array of evaluatorIds of a given value action parameter.
    \param paramName [in] name of parameter object to return.
    \param values [out] array will be filled with returning values.
    \param expressions [out] array will be filled with returning expressions.
    \param evaluatorIds [out] array will be filled with evaluator IDs.
  */
  OdResult getValueParamArray(const OdString& paramName,
                              OdArray<OdDbEvalVariantPtr>& values,
                              OdStringArray& expressions,
                              OdStringArray& evaluatorIds) const;

  /** \details
    Gets a value, expression and evaluatorIds of a given value action parameter.
    \param value [out] will be filled with returning value.
    \param expression [out] will be filled with returning expressions.
    \param evaluatorId [out] will be filled with evaluator ID.
    \param valueIndex [in] value index.
  */
  OdResult getValueParam(const OdString& paramName,
                         OdDbEvalVariant& value,
                         OdString& expression,
                         OdString& evaluatorId,
                         int valueIndex = 0) const;

  /** \details
    Sets an array of values, array of expressions and array of evaluatorIds for a given value action parameter.
    \param paramName [in] name of parameter object to return.
    \param values [in] array of values to set.
    \param expressions [in] array of expressions to set.
    \param evaluatorIds [in] array of evaluator IDs to set.
    \param errorMessages [out] array of error strings to return in case of errors.
    \param silentMode [in] tells to do not return error messages in case of errors.
  */
  OdResult setValueParamArray(const OdString& paramName,
                              const OdArray<OdDbEvalVariantPtr>& values,
                              const OdStringArray& expressions,
                              const OdStringArray& evaluatorIds,
                              OdStringArray& errorMessages,
                              bool silentMode = false);

  /** \details
    Sets a value, expression and evaluatorIds for a given value action parameter.
    \param paramName [in] name of parameter object to return.
    \param value [in] to set.
    \param expression [in] to set.
    \param evaluatorId [in] to set.
    \param errorMessage [out] returned in case of error.
    \param silentMode [in] tells to do not return error message in case of error.
    \param valueIndex [in] description to come.
  */
  OdResult setValueParam(const OdString& paramName,
                         const OdDbEvalVariant& value,
                         const OdString& expression,
                         const OdString& evaluatorId,
                         OdString& errorMessage,
                         bool silentMode = false,
                         int valueIndex = 0);

  /** \details
    \param paramName [in] name of parameter object to return.
    \returns the unit type of a given value action parameter.
  */
  OdValue::UnitType valueParamUnitType(const OdString& paramName) const;

  /** \details
    Sets the unit type for a given value action parameter.
    \param paramName [in] name of parameter object to return.
    \param unitType [in] to set.
  */
  OdResult setValueParamUnitType(const OdString& paramName, OdValue::UnitType unitType);

  /** \details
    Removes the value action parameter using its name.
    \param paramName [in] name of parameter object to remove.
  */
  OdResult removeValueParam(const OdString& paramName);

  /** \details
    An each value action parameter may either be defined by a constant, or an expression. 
    When it is an expression, the parameter stores an object ID of a read-only associated
    value dependency on an associated variable that stores the expression and provides the 
    input value. 
    \param paramName [in] name of parameter object to remove.
    \param variableIds [out] array of returned variable object IDs.
  */
  OdResult valueParamInputVariables(const OdString& paramName, OdDbObjectIdArray& variableIds) const;

  /** \details
    The value action parameter can store an object ID of an "output" dependency on an associated 
    object. The parameter then becomes "linked" with the named "property" of the associated object.
    \param paramName [in] name of parameter object to remove.
    \param controlledObjectDepId [out] dependency object ID to set.
  */
  OdResult setValueParamControlledObjectDep(const OdString& paramName, const OdDbObjectId& controlledObjectDepId);

  /** \details
    Updates the "property" of the associated object from the current value of a value action parameter.
    \param paramName [in] name of parameter object to remove.
  */
  OdResult updateValueParamControlledObject(const OdString& paramName) const;

  /** \details
    Updates the value action parameter from the current value of a named "property" of the associated object.
    \param paramName [in] name of parameter object to remove.
  */
  OdResult updateValueParamFromControlledObject(const OdString& paramName);

  /** \details
    Updates the named properties of all objects linked with this value action parameters.
  */
  OdResult updateAllObjectsControlledByValueParams() const;

  /** \details
    Applies the transformation matrix to all constant geometries in all owned associated parameters.
    \param transform [in] to apply.
  */
  OdResult transformAllConstantGeometryParams(const OdGeMatrix3d& transform);

  /** \details
    Scales the values of all value action parameters whose units indicate represent distances.
    \param scaleFactor [in] to apply.
  */
  OdResult scaleAllDistanceValueParams(double scaleFactor);

  /** \details
    Updates all value parameters taking actual values from associated variables
    \param pThisAction [in] action owning this implementation object
  */
  OdResult updateValueParams(OdDbAssocAction* pThisAction);

  /** \details
    Adds value parameters which need to be cloned along with this action.
    \param pThisAction [in] action owning this implementation object
    \param valueParamsToClone [out] array to return additional objects if any.
  */
  void addValueParamsToDeepClone(const OdDbAssocAction* pThisAction, OdDbObjectIdArray& valueParamsToClone) const;

//#endif
  /** \details
  */
  virtual OdStringArray compareWith(const OdDbImpAssocAction *bodyIDToCompare, OdDb::DwgVersion filerVersion = OdDb::kDHL_CURRENT) const;

  /*!DOM*/
  OdResult checkGetBody(OdDbAssocActionBodyPtr& out, OdDb::OpenMode mode = OdDb::kForWrite) const;

  /*!DOM*/
  static bool doesObjectHaveActiveActions(const OdDbObject* obj, const OdDbObjectId& exceptAction);
protected:
  friend class OdDbAssocAction;
  /*!DOM*/
  inline void writeStatus(OdDbObject* pDbObject, OdDbAssocStatus newStatus);
  /*!DOM*/
  inline void postpone_or_writeStatus(OdDbObject* pThisAction, OdDbAssocStatus newStatus, bool setInOwnedActions);
  /*!DOM*/
  int findDependenciesArrayIndex(OdDbObjectId objectId);

  /*!DOM*/
  OdArray<bool> m_isOwnedDependency;
  /*!DOM*/
  OdDbObjectIdArray m_arrDependencies;
  /*!DOM*/
  OdDbObjectId m_idActionBody;
  /*!DOM*/
  OdDbObjectId m_idOwningNetwork;
  /*!DOM*/
  OdDbAssocStatus m_status; //??
  /*!DOM*/
  OdInt32 m_evalPriority; //??
  /*!DOM*/
  OdUInt32 m_actionIndex;
  /*!DOM*/
  OdUInt32 m_maxAssocDepIdx;
  // see also OdDbImpAssocParamBasedActionBody (R24)
  // R27:
  /*!DOM*/
  OdDbObjectIdArray m_arrParams;
  /*!DOM*/
  OdArray<OdDbImpValueParamPtr> m_arrValueParams;
  /*!DOM*/
  bool m_isBase;

  bool m_EvaluationInProgress;
  /*!DOM*/
  OdInt16 m_version;

  /*!DOM*/
  virtual void fixStatusConsistency(OdDbAssocAction* action);

  friend void odaaFixStatusConsistency(OdDbAssocAction* action);
};

/*!DOM*/
void odaaFixStatusConsistency(OdDbAssocAction* action);

#include "TD_PackPop.h"

#endif

