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

#ifndef DbAssocNetworkDeepCloneAux_INCLUDED_
#define DbAssocNetworkDeepCloneAux_INCLUDED_ /*!DOM*/

#include "TD_PackPush.h"

#define STL_USING_MAP
#define STL_USING_SET
#include "OdaSTL.h"

#include "DbIdMapping.h"
#include "DbObjectId.h"
#include "OdLinkedArray.h"
#include "DbObjectIterator.h"
#include "DbObjectReactor.h"
#include "OdHashSet.h"
#include "OdHashMap.h"
#include "OdPlatformSettings.h"
#include "DbBlockTableRecord.h"


class OdDbAssocAction;
class OdDbAssocNetwork;
class OdDbAssocDependency;
class OdDbAssocVariable;
typedef OdLinkedArray<OdDbObjectId> OdDbObjectIdLinkedArray;
typedef std::map<OdString, OdDbObjectId> OdString2IdMap;
typedef std::map<OdString, OdString> OdString2OdStringMap;

//DOM-IGNORE-BEGIN

class OdDbAssocVarCloneCtx
{
public:
  struct DataEntry
  {
    OdString m_srcName;
    OdString m_dstName;
    bool m_isCloned;
  };

  OdDbAssocVarCloneCtx() {}
  void addDestVariables(OdDbAssocNetwork* network);
  void addSourceVariables(OdDbAssocVariable* variable);

  bool addSourceVariable(const OdString& name);
  bool addDestVariable(const OdString& name);
  void recordVariableClone(const OdString& srcName, const OdString& dstName);

  const DataEntry* getEntryBySourceName(const OdString& srcName) const;
  const DataEntry* getEntryByDestName(const OdString& dstName) const;

  int destVariableCount() const;
  int clonedVariableCount() const;

private:
  typedef OdArray<DataEntry> Data;
  typedef std::map<OdString, Data::size_type> OdString2IdxMap;

  Data m_data;
  OdString2IdxMap m_forwardIndex;
  OdString2IdxMap m_invertedIndex;
};

//DOM-IGNORE-END

typedef OdHashSet<OdDbStub*, OdHashSet_PtrHasher<OdDbStub> > OdDbStubPtrHashSet;
typedef OdHashMap<OdDbStub*, OdDbStubPtrHashSet, OdHashSet_PtrHasher<OdDbStub> > OdDbEvalGraphParamOfAction;

/** \details
  This class implements the context object for assoc networks deep clone operations.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class OdDbAssocNetCloneCtx : public OdDbObjectReactor
{
protected:
  typedef OdHashMap<OdDbAssocNetwork*, OdDbAssocVarCloneCtx, OdHashSet_PtrHasher<OdDbAssocNetwork> > NetworkVariableIndex;
  OdDbIdMappingPtr idMap;
  OdDbStubPtrHashSet dependentOnObjectsCloned;
  OdDbStubPtrHashSet actionsToClone;
  OdDbObjectIdLinkedArray actionsToAppend;
  OdDbObjectIdLinkedArray actionsCloned;
  OdDbEvalGraphParamOfAction evalGraphParams;
  NetworkVariableIndex variableIndices;
  OdDbObjectId explodingBlockReferenceId;
  mutable OdGeMatrix3d actionXform;
  mutable bool bApplyTransform;
  OdDbAssocNetCloneCtx() { bApplyTransform = false; }
public:
  ODRX_DECLARE_MEMBERS(OdDbAssocNetCloneCtx);

  /** \details
    Registries the deep clone reactor.
  */
  static void createAssocNetworkCloneReactors();

  /** \details
    Unregisters the deep clone reactor.
  */
  static void deleteAssocNetworkCloneReactors();

  /** \details
    Attaches new context data associating it with given OdDbDatabase
  */
  static void attach(OdDbIdMapping& idMap);

  /** \details
    Gets internal data associated with given database
  */
  static OdSmartPtr<OdDbAssocNetCloneCtx> fromDatabase(OdDbDatabase* destDb);

  /** \details
    Gets internal data associated with given database
  */
  static OdSmartPtr<OdDbAssocNetCloneCtx> internal_fromDb(OdDbDatabase* destDb);

  /** \details
    Detaches this context data associated with given database
  */
  static bool detach(OdDbDatabase* destDb);

  /** \details
    Gets IdMapping associated with current clone operation
  */
  OdDbIdMapping& idMapping();

  OdDbObjectIdLinkedArray& clonedActions();

  OdDbAssocVarCloneCtx& varCloneContext(OdDbAssocNetwork* destNetwork);

  void onBlockCloned(const OdDbBlockTableRecord& originalBlock);
  void onDependentObjectCloned(const OdDbObject& originalObject);

  /** \details
    Remembers an action from source database that was cloned.
  */
  void onActionCloned(const OdDbAssocAction* action);

  virtual void setActionTransform(const OdGeMatrix3d& transform);

  bool doApplyTransform() const { return bApplyTransform; }

  const OdGeMatrix3d& actionTransform() const;

  /** Description
    Calls addMoreObjectToDeepClone() for actions whose dependent-on objects were cloned .
  */
  void addMoreObjectsToDeepClone();

  static bool isOwnedByEvalGraph(OdDbObjectId id);

  /** Description
    Remembers dependent-on object which is evaluation graph's parameter to reattach after cloning.
    \param action [in] object id of cloning action that depends on evaluation graph's child object (e.g. its parameter).
    \param dependentOnEvalGraphParam [in] evaluation graph's child object id the given action depends on.

    \remarks
    Corresponding dependency to given dependent-on object of given action will be automatically mapped
    to eval graph's child object in dynamic block definition in target database at the end of successful
    database clone operation before calling postProcessAfterDeepClone().

    \sa
    OdDbAssocAction::postProcessAfterDeepClone
  */
  void trackEvalGraphParam(OdDbObjectId action, OdDbObjectId dependentOnEvalGraphParam);

  /** Description
    Relinks dependency lists on cloned DB-objects that is be broken by deep clone operation when not all actions
    depended on a object were cloned.
  */
  void relinkDependenciesOnObjects();

  /** Description
    Appends cloned actions to its owners.
  */
  void appendActionsToOwners();

  /** Description
    Calls one of OdDbObject's methods on 'original', either deepClone() or wblockClone() depending on destination dabase value.
    if it's equal to original then deepClone() is callen and if it's not then calls wblockClone().
  */
  static bool cloneObject(OdDbIdMapping& idMap, const OdDbObject* original, OdDbObject* destOwner, OdDbObjectPtr* pClone = 0);

  static void lookupDestinationBlock(OdDbIdMapping& idMap, OdDbAssocAction* action, OdDbBlockTableRecordPtr& destBlock);

  static OdDbObjectPtr lookupActionCloneOwner(OdDbIdMapping& idMap, OdDbEntity* destActionEntity);

  static OdDbObjectPtr lookupActionCloneOwner(OdDbIdMapping& idMap, OdDbBlockTableRecord* destBlock);

  static OdDbObjectPtr lookupActionCloneOwner(OdDbIdMapping& idMap, OdDbAssocAction* action);
};

typedef OdSmartPtr<OdDbAssocNetCloneCtx> OdDbAssocNetCloneCtxPtr;


inline OdDbIdMapping& OdDbAssocNetCloneCtx::idMapping() { return *idMap; }

inline OdDbObjectIdLinkedArray& OdDbAssocNetCloneCtx::clonedActions() { return actionsCloned; }

inline OdDbAssocNetCloneCtxPtr OdDbAssocNetCloneCtx::fromDatabase(OdDbDatabase* destDb) {
  return static_cast<OdDbAssocNetCloneCtx*>(OdDbObjectReactor::findReactor(destDb, ::odrxGetClassDesc(L"OdDbAssocNetCloneCtx")));
}

inline OdDbAssocNetCloneCtxPtr OdDbAssocNetCloneCtx::internal_fromDb(OdDbDatabase* destDb) {
  return static_cast<OdDbAssocNetCloneCtx*>(OdDbObjectReactor::findReactor(destDb, OdDbAssocNetCloneCtx::desc()));
}

inline const OdGeMatrix3d& OdDbAssocNetCloneCtx::actionTransform() const {
  return actionXform;
}

inline void OdDbAssocNetCloneCtx::setActionTransform(const OdGeMatrix3d& xform) {
  actionXform = xform;
  bApplyTransform = !xform.isEqualTo(OdGeMatrix3d::kIdentity, OdGeTol(OdGeContext::gZeroTol));
}


inline OdDbObjectId oddbTranslate(OdDbObjectId id, OdDbIdMapping &idMap) {
  OdDbIdPair idPair(id);
  if(idMap.compute(idPair))
    return idPair.value();
  return OdDbObjectId::kNull;
}

inline OdDbObjectId oddbTranslateCloned(OdDbObjectId id, OdDbIdMapping &idMap) {
  OdDbIdPair idPair(id);
  if (idMap.compute(idPair) && idPair.isCloned())
    return idPair.value();
  return OdDbObjectId::kNull;
}


#include "TD_PackPop.h"

#endif
