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

#ifndef DbAssocGlobal_INCLUDED_
#define DbAssocGlobal_INCLUDED_ /*!DOM*/

#include "DbConstraintsExports.h"
#include "DbObject.h"
#include "Ge/GeTol.h"

class OdDbAssocAction;
class OdDbAssocActionBody;
class OdDbAssocNetwork;
class OdDbAssocDependency;
class OdDbAssocDependencyBody;
class OdDbAssocGeomDependency;
class OdDbAssoc2dConstraintGroup;
class OdDbAssocPersSubentId;
class OdDbAssocSimplePersSubentId;
class OdDbAssocSingleEdgePersSubentId;
class OdDbAssocPersSubentIdPE;
class OdDbAssocVariable;
class OdDbAssocValueDependency;
class OdDbAssocValueProviderPE;
class OdDbEvalContext;

#include "TD_PackPush.h"

/** \details
  Declares the error status.
*/
enum ErrorStatus
{
        kSuccess,       // Success
        kOutOfRange,    // Outs the range
        kInvalidInput   // Incorrect data
};

/** \details
  Declares the associative status.
*/
enum OdDbAssocStatus
{
  kIsUpToDateAssocStatus = 0,        // Everything is in sync
  kChangedDirectlyAssocStatus,       // Explicitly changed
  kChangedTransitivelyAssocStatus,   // Changed indirectly due to changes
  kChangedNoDifferenceAssocStatus,   // No real change (only forced to evaluate)
  kFailedToEvaluateAssocStatus,      // Action failed to evaluate but evaluation continues
  kErasedAssocStatus,                // Dependent-on object erased, or action is to be erased
  kSuppressedAssocStatus,            // Action evaluation suppressed, treat as if evaluated
  kUnresolvedAssocStatus,            // Dependent-on object is unresolved (e.g the xref is unloaded)
};

/** \details
  Global function evaluates the request severity level.
*/
inline int evaluationRequestSeverityLevel(OdDbAssocStatus status)
{
    switch (status)
    {
    case kChangedDirectlyAssocStatus:
        return 3;
    case kChangedTransitivelyAssocStatus:
        return 2;
    case kChangedNoDifferenceAssocStatus:
        return 1;
    default:
        return 0; // Not a request to evaluate
    }
}

/** \details
  Global function checks the evaluation request.
*/
inline bool isEvaluationRequest(OdDbAssocStatus status)
{
    return evaluationRequestSeverityLevel(status) > 0;
}

inline bool isToBeSkipped(OdDbAssocStatus status)
{
    return status == kErasedAssocStatus || status == kSuppressedAssocStatus;
}

/** \details
  Declares the evaluation priority.
*/
enum OdDbAssocEvaluationPriority
{
  kCannotBeEvaluatedAssocEvaluationPriority = -1000,
  kCannotDermineAssocEvaluationPriority = 0,
  kCanBeEvaluatedAssocEvaluationPriority = 1000,
};

/** \details
  Declares the evaluation mode.
*/
enum OdDbAssocEvaluationMode
{
    kModifyObjectsAssocEvaluationMode,       // The mode in which the action depends-on via write-dependencies are modified
    kModifyActionAssocEvaluationMode,        // The mode in which the action is modified and it satisfies the objects depended on action
};

/** \details
  Declares the dragging state.
*/
enum OdDbAssocDraggingState
{
    kNotDraggingAssocDraggingState,          // Outside the dragging loop
    kFirstSampleAssocDraggingState,          // The first sample of the dragging loop
    kIntermediateSampleAssocDraggingState,   // An intermediate sample of the dragging loop
    kLastSampleAssocDraggingState            // The last sample of the dragging loop
};

/** \details
  Declares the dragging state.
*/
enum OdDbAssocTransformationType
{
    kNotSpecified,                           // Not specified
    kStretch,                                // Strech command changes the geometries
    kRotate,                                 // Rotate command changes the geometries
    kMove,                                   // Move command changes the geometries
};

/** \details
  This class implements the association evaluation callback set.
  Actions provide a mechanism to notify about the progress of evaluation, notify about which 
  objects the action is going to use and modify, to allow the evaluation to be cancelled, etc.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdDbAssocEvaluationCallback
{
public:
    /** \details
      Constructor creates an instance of this class.
    */
    OdDbAssocEvaluationCallback() {}

    /** \details
      Destructor destroys an instance of this class.
    */
    virtual ~OdDbAssocEvaluationCallback() {}

#   if 0

    /** \details
      Returns the evaluation mode.
    */
    virtual OdDbAssocEvaluationMode evaluationMode() const { return kModifyObjectsAssocEvaluationMode; }

    /** \details
      Informs that the action starts its evaluation.
      The action issues this callback just before opening the object for reading or writing.
    */
    virtual void beginActionEvaluation(OdDbAssocAction* pAction) = 0;

    /** \details
      Informs that the action has finished its evaluation.
      This callback is issued at the end of this call.
    */
    virtual void endActionEvaluation(OdDbAssocAction* pAction) = 0;

    /** \details
      Informs that an error occurred during its evaluation and may also inform about which object 
      caused the failure, giving its id, pointer or both.
    */
    virtual void setActionEvaluationErrorStatus(OdDbAssocAction*    pAction,
                                                ErrorStatus   errorStatus,
                                                const OdDbObjectId& objectId   = OdDbObjectId::kNull,
                                                OdDbObject*         pObject    = NULL,
                                                void*               pErrorInfo = NULL) = 0;

    /** \details
      Informs that the action is going to use or modify.
      The action issues this callback just before opening the object for reading or writing.
    */
    virtual void 
    beginActionEvaluationUsingObject(OdDbAssocAction*    pAction, 
                                     const OdDbObjectId& objectId, 
                                     bool                objectIsGoingToBeUsed,
                                     bool                objectIsGoingToBeModified,
                                     OdDbObject*&        pSubstituteObject) = 0;

    /** \details
      Informs that the action is done with using or modifying.
      The action issues this callback just before it closes the object.
    */
    virtual void endActionEvaluationUsingObject(OdDbAssocAction*    pAction, 
                                                const OdDbObjectId& objectId, 
                                                OdDbObject*         pObject) = 0;

    /** \details
      Returns the dragging state.
  
      \remarks
      The action may inquire from the client code whether the evaluation is happening  
      from inside of the dragging loop and at which stage the dragging is at. 
    */
    virtual OdDbAssocDraggingState draggingState() const { return kNotDraggingAssocDraggingState; }

#   endif

    /** \details
      Returns True when the evaluation to be canceled.
    */
    virtual bool cancelActionEvaluation() = 0;

    /** \details
      Allows the evaluation callback to pass arbitrary data for actions when they are evaluated.
    */
    virtual OdDbEvalContext* getAdditionalData() const { return NULL; }

    /** \details
      Allows the evaluation callback to pass information about what transformation type   
      has been performed.
    */
    virtual OdDbAssocTransformationType getTransformationType() const { return kNotSpecified; }
};

#if 0

inline bool isDraggingProvidingSubstituteObjects(const OdDbAssocEvaluationCallback* pEvaluationCallback)
{
    if (pEvaluationCallback == NULL)
        return false;

    const OdDbAssocDraggingState draggingState = pEvaluationCallback->draggingState();

    return draggingState == kFirstSampleAssocDraggingState || 
           draggingState == kIntermediateSampleAssocDraggingState;
}

#endif

/** \details
  This class implements the callback that informs the caller about other actions, dependencies or 
  objects which should be evaluated because they depend on the given action.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdDbActionsToEvaluateCallback
{
public:
  /** \details
    Constructor creates an instance of this class.
  */
  OdDbActionsToEvaluateCallback() {}

  /** \details
    Destructor destroys an instance of this class.
  */
  virtual ~OdDbActionsToEvaluateCallback() {}

  /** \details
    Informs the caller that a dependency needs to be evaluated and requires a new status for it.
  */
  virtual void dependencyNeedsToEvaluate(OdDbAssocDependency* dependency, OdDbAssocStatus newStatus) = 0;

  /** \details
    Informs the caller that an action needs to be evaluated and requires a new status for it.
  */
  virtual void actionNeedsToEvaluate(OdDbAssocAction* action, OdDbAssocStatus newStatus) = 0;

  /** \details
    Informs the caller that an object needs to be evaluated and requires a new status for it.
  */
  virtual void objectNeedsToEvaluate(const OdDbObject* object, OdDbAssocStatus newStatus) = 0;

  /** \details
    Informs the caller that an action, a dependency or an object needs to be evaluated and requires a new status for it.
  */
  virtual void needsToEvaluate(const OdDbObjectId &objectId, OdDbAssocStatus newStatus = kChangedDirectlyAssocStatus, bool ownedActionsAlso = true) = 0;
};

// Internal use only

/*!DOM*/
enum OdDbAssocCreateImpObject 
{
    /*!DOM*/
    kAcDbAssocCreateImpObject = 0,
    /*!DOM*/
    kAcDbAssocDoNotCreateImpObject = 1,
};

#include "TD_PackPop.h"

#endif

