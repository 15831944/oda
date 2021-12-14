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

#ifndef DbAssocAction_INCLUDED_
#define DbAssocAction_INCLUDED_ /*!DOM*/

#include "DbConstraintsExports.h"
#include "DbAssocGlobal.h"
#include "DbEntity.h"     // Because of DragStat enum

#include "StringArray.h"
#include "DbEvalVariant.h"
#include "OdValue.h"
#include "DbIdMapping.h"
#include "DbCompoundObjectId.h"
#include "DbAssocDependency.h"

class OdDbImpAssocAction;

#include "TD_PackPush.h"

// CS - means result string after comparison

// Common
#define CS_UNDEFINED_PROPERTY L"undefinedProperty"
#define CS_NOT_CRITICAL L"notError"
#define CS_OBJECT_ERROR L"objectError"
#define CS_DIFFERENT_OBJECTS L"Objects are different"
#define CS_DIFFERENT_PARAM_CRITICAL L"Parameters are different! Comparizon is bad!"

// AssocDependency
#define CS_UNDEFINED_ANNOTATION_ACTIONBODY L"undefinedAnnotationActionBody"
#define CS_READ_DEP L"bReadDep"
#define CS_WRITE_DEP L"bWriteDep"
#define CS_OBJECT_STATE_DEPENDENT L"bObjectStateDependent"
#define CS_DELEGATING_TO_OWNING_ACTION L"bDelegatingToOwningAction"
#define CS_ORDER L"nOrder"
#define CS_PARAM_NAME L"nOrder"

#define CS_INDEX L"nIndex"

// AssocRotatedDimActionBody
#define CS_UNDEFINED_ANNOTATION_ACTION_BODY L"undefinedAnnotationActionBody"
#define CS_PARAM_BASED_ACTION_BODY L"paramBasedActionBody"

/** \details
  This class is the base class for actions which provide the associative behavior. 
  This class itself does not have virtual methods that could be overridden, therefore it is not 
  possible for external developers to derive directly from it and override its methods. 

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdDbAssocAction : public OdDbObject
{
public:     
    ODRX_DECLARE_MEMBERS(OdDbAssocAction);

    /** \details
      Constructor creates an instance of this class.
      \param createImpObj [in] tells to create implementation object in the constructor.
    */
    OdDbAssocAction(bool createImpObj = true);

    /** \details
      Destructor destroys an instance of this class.
    */
    ~OdDbAssocAction();

    /** \details
      \returns an object ID of the owned action instance. 
    */
    OdDbObjectId actionBody() const;
    
    /** \details
      Sets the owned action instance using an object ID.
      \param actionBodyId [in] to set.
    */
    OdResult setActionBody(const OdDbObjectId& actionBodyId);

    /** \details
      \returns true if action body is loaded as a proxy object.
      (If true the action is not evaluatable.)
    */
    bool isActionBodyAProxy() const;

    /** \details
      \returns the current status of this action. 
    */
    OdDbAssocStatus status() const;

    /** \details
      Sets the status for this action. 
      \param newStatus [in] to set.
      \param notifyOwningNetwork [in] tels to call owning network's ownedActionStatusChanged.
      \param setInOwnedActions [in] tels to set the same status to owned actions.
    */
    OdResult setStatus(OdDbAssocStatus newStatus, 
                       bool notifyOwningNetwork = true,
                       bool setInOwnedActions   = false);

    /** \details
      \returns the logically owning association network, or kNull if it is the topmost. 
    */
    OdDbObjectId owningNetwork() const;

    /** \details
      Sets the logically owning association network. 
      \param networkId [in] to set as logical owner of the action
      \param alsoSetAsDatabaseOwner [in] if true then networkId will be set also as database owner of the action.
    */
    OdResult setOwningNetwork(const OdDbObjectId& networkId, bool alsoSetAsDatabaseOwner);


    /** \details
      Gets the dependencies of this action. 
      \param readDependenciesWanted [in] read dependencies wanted.
      \param writeDependenciesWanted [in] write dependencies wanted.
      \param dependencyIds [out] returned dependencies.
    */
    OdResult getDependencies(bool readDependenciesWanted,
                             bool writeDependenciesWanted,
                             OdDbObjectIdArray& dependencyIds) const;

    /** \details
      Adds a dependency for this action. 
      \param dependencyId [in] ID of the dependency to add
      \param setThisActionAsOwningAction [in] tells to set this action as owning action of the dependency.
    */
    OdResult addDependency(const OdDbObjectId& dependencyId, 
                           bool setThisActionAsOwningAction = true);

    /** \details
      Creates new dependency on a given object. 
      Returns object id of new dependency.
    */
    OdDbObjectId createDependency(const OdDbObjectId& dependentOnObject,
      bool read = true, bool write = false, int order = 0, OdDbAssocDependencyPtr* newDependency = 0);

    /** \details
      Creates new dependency on a given object. 
      Returns object id of new dependency.
    */
    OdDbObjectId createDependency(const OdDbCompoundObjectId& dependentOnObject,
      bool read = true, bool write = false, int order = 0, OdDbAssocDependencyPtr* newDependency = 0);

    /** \details
      Deprecated.
    */
    OdDbObjectId createDependency(const OdDbCompoundObjectId* dependentOnObject,
      bool read = true, bool write = false, int order = 0, OdDbAssocDependencyPtr* newDependency = 0)
    {
       return createDependency(*dependentOnObject, read, write, order, newDependency);
    }

    /** \details
      Removes a dependency from this action. 
      \param dependencyId [in] ID of the dependency to remove
      \param alsoEraseIt [in] tells to erase the dependency from database.
    */
    OdResult removeDependency(const OdDbObjectId& dependencyId, 
                              bool alsoEraseIt);

    /** \details
      Clears all dependencies from this action.
      \param alsoEraseIt [in] tells to erase removed dependencies from database.
    */
    OdResult removeAllDependencies(bool alsoEraseThem);

    /** \details
      Gets an array of objects which this action depends on or modifies. 
      \param readDependenciesWanted [in] read dependencies wanted.
      \param writeDependenciesWanted [in] write dependencies wanted.
      \param objectIds [out] returned dependent objects.
    */
    OdResult getDependentObjects(bool readDependenciesWanted,
                                 bool writeDependenciesWanted,
                                 OdDbObjectIdArray& objectIds) const;
#if 0
    /** \details
      Checks whether the given dependency is owned by this action, either directly, 
      or if this action is a network. 
    */
    bool isOwnedDependency(const OdDbAssocDependency* pDependency) const;

    /** \details
      Checks whether the given dependency owned by this action, is external to this action. 
    */
    bool isExternalDependency(const OdDbAssocDependency* pDependency) const;
#endif
    /** \details
      Checks whether the object change is relevant. 
      \param pDependency [in] dependency to check.
   */
    bool isRelevantDependencyChange(const OdDbAssocDependency* pDependency) const;
#if 0
    /** \details
      Checks whether a dependency has a cached value. It is delegated from the owned 
      dependency's predicate. 
    */
    bool hasDependencyCachedValue(const OdDbAssocDependency* pDependency) const;

    /** \details
      Checks whether two depedencies depend on the same thing and allows the owning action, 
      instead of the dependency itself.
    */
    bool areDependenciesOnTheSameThing(const OdDbAssocDependency* pDependency1, 
                                       const OdDbAssocDependency* pDependency2) const;

    /** \details
      Checks whether two dependencies are equal and allows the owning action, instead of 
      the dependency itself.
    */
    bool areDependenciesEqual(const OdDbAssocDependency* pDependency1, 
                              const OdDbAssocDependency* pDependency2) const;

#endif
    /** \details
      Gets all actions depending on the given object.
      \param pObject [in] the querying object.
      \param readDependenciesWanted [in] read dependencies wanted.
      \param writeDependenciesWanted [in] write dependencies wanted.
      \param actionIds [out] array returned dependent objects.
    */
    static OdResult getActionsDependentOnObject(const OdDbObject* pObject,
                                                bool readDependenciesWanted,
                                                bool writeDependenciesWanted,                                   
                                                OdDbObjectIdArray& actionIds);

    /** \details
      Dependency notifies the owning action when dependent-on object has been cloned.
      Class overriding this callback must call this base implementation in its implementation
      for proper working of deep clone operations.
      \param pDependency [in] dependency attached to original object.
      \param pOriginal [in] original object.
      \param pClone [in] clone object.
   */
    void dependentObjectCloned(const OdDbAssocDependency* pDependency, 
                               const OdDbObject*          pDbObj, 
                               const OdDbObject*          pNewObj);

    /** \details
      Tells to action being cloned to specify related objects that should be cloned
      because this action is cloned.
      \param idMap [in] id mapping object of current cloning operation.
      \param additionalObjectsToClone [out] array to return additional objects if any.
    */
    OdResult addMoreObjectsToDeepClone(OdDbIdMapping& idMap,
                                       OdDbObjectIdArray& additionalObjectsToClone) const;

    /** \details
      Informs the source actions of the cloning object so that they can update the clones or do other
      action if it is necessary. This method is called at the end of database cloning operation.
      \param idMap [in] id mapping object of current cloning operation.
    */
    OdResult postProcessAfterDeepClone(OdDbIdMapping& idMap); 

    /** \details
      Informs the source actions so that it can obtain its clone. This method is called at the end
      of database cloning operation.
      \param idMap [in] id mapping object of current cloning operation.
    */
    OdResult postProcessAfterDeepCloneCancel(OdDbIdMapping& idMap);

    /** \details
      Checks whether the system is evaluating an action or a network of actions.
    */
    bool isActionEvaluationInProgress() const;

    /** \details
      \returns the current association evaluation callback set instance, or NULL if no action evaluation 
      is in progress.
    */
    OdDbAssocEvaluationCallback* currentEvaluationCallback() const;

    /** \details
      This method should be called from the client code evaluateOverride() method,
      usually after the client code inspected the dependencies, found which changed,
      which were on erased objects, etc. It gets all the dependencies of the action and evaluates
      those need to be evaluated ( isEvaluationRequest(pDep->status()) == true ).
    */
    OdResult evaluateDependencies();

    /** \details
      Allows the owning action, instead of the dependency itself, to be performed.
      \param pDependency [in] dependency to evaluate.
    */
    void evaluateDependency(OdDbAssocDependency* pDependency);

    /** \details
      Notifies the action owning the dependency that its status has been changed. The dependency 
      is still opened for write and already has the new status.
      \param pOwnedDependency [in] owned dependency which status has been change
      \param previousStatus [in] previous value of the dependency status
    */
    OdResult ownedDependencyStatusChanged(OdDbAssocDependency* pOwnedDependency, 
                                          OdDbAssocStatus      previousStatus);
    /** \details
      Notifies the action that its owner is transformed. For example, all entities in the block owning 
      the network that owns the action are transformed.
      \param transform [in] matrix that was used to transform
    */
    OdResult transformActionBy(const OdGeMatrix3d& transform);
#if 0

    /** \details
      Compares this action with the given action. Returns True if and only if they are equal.
    */
    bool isEqualTo(const OdDbAssocAction* pOtherAction) const;
#endif
    /** \details
      Returns the evaluation priority of this action.
    */
    OdDbAssocEvaluationPriority evaluationPriority() const;

    /** \details
      Allows the action report on all other actions that need to be evaluated.
      \param pActionsToEvaluateCallback [in] this action should specify other actions (and/or objects) that need to be evaluated
    */
    void getDependentActionsToEvaluate(OdDbActionsToEvaluateCallback* pActionsToEvaluateCallback) const;

    /** \details
      Evaluates this action. This method is called from client code to evaluate an action or 
      an associative network.
      \param pEvaluationCallback [in] evaluation callback.
    */
    void evaluate(OdDbAssocEvaluationCallback* pEvaluationCallback);

    /** \details
      Returns an ID of the object that owns the network that owns this action.
    */
    OdDbObjectId objectThatOwnsNetworkInstance() const;
#if 0
    /** \details
      Notifies the action when the action depends on objects, either directly or indirectly.
    */
    void dragStatus(const OdDb::DragStat status);
#endif

  /** \details
    Reads the .dwg file data of this object. 

    \param pFiler [in]  Filer object from which data are read.
    
    \remarks
    Returns the filer status.
  */
  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler);

  /** \details
    Writes the .dwg file data of this object. 

    \param pFiler [in]  Pointer to the filer to which data are written.
  */
  virtual void dwgOutFields(OdDbDwgFiler* pFiler) const;

  /** \details
    Reads the .dxf file data of this object. 

    \param pFiler [in]  Filer object from which data are read.
    
    \remarks
    Returns the filer status.
  */
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);

  /** \details
    Writes the .dxf file data of this object. 

    \param pFiler [in]  Pointer to the filer to which data are written.
  */
  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;

  /** \details
    After loading from file, performs necessary actions which require accessing other Database 
    objects. For example, processing round-trip data.
  */
  virtual void composeForLoad(OdDb::SaveType format, OdDb::DwgVersion version, OdDbAuditInfo* pAuditInfo);

  /** \details
    Determines the behavior for custom objects when saving to .dwg or .dxf file.
    \param ver [in]  Drawing version to save as.
    \param replaceId [out]  Object ID of the object replacing this object.
    \param exchangeXData [out]  Set to true if and only if this function did not add XData to the replacement object.
    \sa <link !!OVERLOADED_decomposeForSave_OdDbObject, OdDbObject::decomposeForSave() methods>
  */
  virtual OdDbObjectPtr decomposeForSave(OdDb::DwgVersion ver, OdDbObjectId& replaceId, bool& exchangeXData);

  /** \details
    \sa <link OdDbObject::audit@OdDbAuditInfo*, OdDbObject::audit() method.
  */
  virtual void audit(OdDbAuditInfo* pAuditInfo);

  /** \details
    Sets the index for this action.
    \param newIndex [in] to set.
  */
  OdResult setActionIndex(int newIndex);

// AC2013
// OdDbAssocActionParams stuff

  /** \details
    Returns the number of the owned action parameters.
  */
  int paramCount() const;

  /** \details
    \returns an array of the owned action parameters.
  */
  const OdDbObjectIdArray& ownedParams() const;

  /** \details
    Adds a new action parameter.
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
  */
  OdResult addParam(const OdString& paramName, OdRxClass* pParamClass, OdDbObjectId& paramId, int& paramIndex);

  /** \details
    Removes an existing action parameter and optionally erases it.
    \param paramId [in] object ID of parameter to remove.
    \param alsoEraseIt [in] tells to erase removed parameter.
  */
  OdResult removeParam(const OdDbObjectId& paramId, bool alsoEraseIt);

  /** \details
    Removes all owned action parameters and optionally erases them.
    \param bEraseThem [in] tells to erase removed parameters.
  */
  OdResult removeAllParams(bool bEraseThem);

  /** \details
    \returns an array of object IDs of the owned action parameters with specified name.
    \param paramName [in] name of parameters to return.
  */
  OdDbObjectIdArray paramsAtName(const OdString& paramName) const;

  /** \details
    \returns an owned action parameter specified by its name and optional index for parameters with the same name.
    \param paramName [in] name of parameter object to return.
    \param paramIndex [in] index of parameter object to return.
  */
  OdDbObjectId paramAtName(const OdString& paramName, int index = 0) const;

  /** \details
    \returns an owned action parameter specified by its index.
    \param paramIndex [in] index of parameter object to return.
  */
  OdDbObjectId paramAtIndex(int paramIndex) const;

  /** \details
    \returns all the names of all value action parameters.
    \param paramNames [out] string array to fill with names.
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
  OdResult getValueParam(const OdString&  paramName,
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
    \returns the unit type of a given value action parameter.
    \param paramName [in] name of parameter object to return.
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
    \param transform [in] the transformation matrix to apply.
  */
  OdResult transformAllConstantGeometryParams(const OdGeMatrix3d& transform);

  /** \details
    Scales the values of all value action parameters whose units indicate represent distances.
    \param scaleFactor [in] to apply.
  */
  OdResult scaleAllDistanceValueParams(double scaleFactor);

  /** \details
  */
  virtual OdStringArray compareWith(const OdDbObjectId bodyIDToCompare, OdDb::DwgVersion filerVersion = OdDb::kDHL_CURRENT) const;

  /** \details
    This callback appends this action to the specified owner object
    at the end of the deep and/or wblock clone operation.

    \param idPair [in]  Original-to-clone ID mapping record for this clone.
    \param pOwner [in]  Pointer to the owner object.
    \param idMap [in/out]  ID mapping of the cloning operation in progress.

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
  */
  virtual void appendToOwner(OdDbIdPair& idPair, OdDbObject* pOwner, OdDbIdMapping& idMap);

  virtual OdDbObjectPtr subDeepClone(OdDbIdMapping& idMap, OdDbObject* pOwner, bool bPrimary) const;

  virtual OdDbObjectPtr subWblockClone(OdDbIdMapping& idMap, OdDbObject* pOwner, bool bPrimary) const;

  /** \details
    \sa <link OdDbObject::subErase@bool, OdDbObject::subErase() method>
  */
  virtual OdResult subErase(bool erasing);

protected:
  /*!DOM*/
  OdDbImpAssocAction *m_pImpObj;

  /*!DOM*/
  friend class OdDbImpAssocAction;
};

/** \details
  The typified smart pointer. This template class is specialization of the OdSmartPtr class 
  for the OdDbAssocAction class.

  \sa
  <link smart_pointers, Working with Smart Pointers>
*/
typedef OdSmartPtr<OdDbAssocAction> OdDbAssocActionPtr;

#include "TD_PackPop.h"

#endif

