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


#ifndef _TFREVISIONCONTROL_H_INCLUDED_
#define _TFREVISIONCONTROL_H_INCLUDED_

#include "TfSchemaManager.h"
#include "DbObjectIterator.h"
#include "ThreadsCounter.h"

#define STL_USING_MAP
#include "OdaSTL.h"

#include <stdexcept>

/** \details
  This structure stores hash codes for commits.
*/
struct TFDLL_EXPORT OdTfDigest
{
  /** Data array to store a hash code. */
  char data[20];

  /** \details
    Writes the hash code to a stream buffer.

    \param pBuf [in] Stream buffer to write data to.
  */
  void write(OdStreamBuf* pBuf) const { pBuf->putBytes(data, 20); }

  /** \details
    Reads a hash code from a stream buffer.

    \param pBuf [in] Stream buffer to read data from.
  */
  void read(OdStreamBuf* pBuf) { pBuf->getBytes(data, 20); }

  /** \details
    "Less than" comparison operator.

    \param other [in] Hash code to compare with this hash code.
    \returns
    True if this hash code is less than other hash code; false if this hash code
    is equal or greater than other hash code.
  */
  bool operator < (const OdTfDigest& other) const
  {
    return memcmp(data, other.data, 20) < 0;
  }

  /** \details
    Comparison operator.

    \param other [in] Hash code to compare with this hash code.
    \returns
    True if this hash code is equal to other hash code; false otherwise.
  */
  bool operator == (const OdTfDigest& other) const
  {
    return memcmp(data, other.data, 20) == 0;
  }

  /** \details
    Negative comparison operator.

    \param other [in] Hash code to compare with this hash code.
    \returns
    True if this hash code is not equal to other hash code; false otherwise.
  */
  bool operator != (const OdTfDigest& other) const
  {
    return memcmp(data, other.data, 20) != 0;
  }

  /** \details
    Creates and returns a NULL hash code.
  */
  static OdTfDigest kNull()
  {
    OdTfDigest res;
    memset(res.data, 0, 20);
    return res;
  }

  /** \details
    Checks whether this hash code is a NULL hash code.

    \returns
    True if this hash code is a NULL hash code; false otherwise.
  */
  bool isNull() const
  {
    char test[20] = { 0 };
    return memcmp(test, data, 20) == 0;
  }

  /** \details
    Default constructor for instances of the OdTfDigest structure.
    Creates a NULL hash code.
  */
  OdTfDigest()
  {
    memset(data, 0, 20);
  }
};

/** \details
  This type represents arrays of hash codes.
*/
typedef OdArray<OdTfDigest, OdMemoryAllocator<OdTfDigest> > OdTfDigestArray;

/** \details
  This namespace contains revision control specific definitions.
*/
namespace OdTfRevisionControl
{
  /** \details
    Reference list.
  */
  typedef std::map<OdString, OdTfDigest> ReferenceList;

  /** \details
    Declares change types.
  */
  enum ChangeType
  {
    /** Object not modified or not existing */
    kObjectUnmodified = 0,
    /** Object added to database */
    kObjectAdded = 1, 
    /** Object deleted from database */
    kObjectDeleted,   
    /** Object modified */
    kObjectModified    
  };

  /** \details
    Change list.
  */
  typedef std::map<OdDbHandle, ChangeType> ChangeList;

  /** \details
   Flags for defining the MergeResolution values.
   */
  enum MergeFlags {
    kMergeFlagBase = 0,
    kMergeFlagMine = 1,
    kMergeFlagTheirs = 2,
    kMergeFlagFull = 4,

    kMergeMaskMineTheirs = kMergeFlagMine | kMergeFlagTheirs,
    kMergeMaskResolution = kMergeMaskMineTheirs | kMergeFlagFull,
  };

  /** \details
    Declares merge resolutions.
  */
  enum MergeResolution {
    /** Reject changes of both sides and revert to the object revision of the base. */
    kMergeRevertFull = kMergeFlagBase | kMergeFlagFull,
    /** Resolve the conflict by preferring my changes over their changes for conflicting
        properties of the object. */
    kMergeMineConflict = kMergeFlagMine,
    /** Resolve the conflict by preserving all my changes and discard all their changes to
        the object. */
    kMergeMineFull = kMergeFlagMine | kMergeFlagFull,
    /** Resolve the conflict by preferring their changes over my changes for conflicting
        properties of the object. */
    kMergeTheirsConflict = kMergeFlagTheirs,
    /** Resolve the conflict by discarding all my changes and integrate all their changes
        to the object. */
    kMergeTheirsFull = kMergeFlagTheirs | kMergeFlagFull,

    /** Do not resolve the conflict in any way yet. The conflict should be recorded for
        later resolving. */
    kMergePostpone = kMergeMineFull,

    /** Passed in autoResolution to OdTfMergeHandler if the conflict can not be resolved
        automatically and requires an explicit decision.
        Not to be returned by OdTfMergeHandler!
    */
    kMergeNoResolution = 0,
  };

  /** \details
   Check if my and their changes of the object conflict and requires an explicit
   resolution.

   Changes conflict when both sides are modified or when one side is modified and the
   other side has been deleted.

   \param myChange    ChangeType of the object of my side.
   \param theirChange ChangeType of the object of their side.
   \returns true if ChangeType of my and their side poses a true conflict that requires
            explicit resolving.
   */
  bool inline isConflict(ChangeType myChange, ChangeType theirChange)
  {
    return myChange != kObjectUnmodified
      && theirChange != kObjectUnmodified
      && !(myChange == kObjectDeleted && theirChange == kObjectDeleted);
  }
};



/** \details
  Structure describing a commit in the revision control database.
*/
struct TFDLL_EXPORT OdTfCommitAnnotation
{
  /** Commit author. */
  OdString author;      
  /** Commit message. */
  OdString message;     
  /** Commit timestamp. */
  OdTimeStamp date;     
  /** Arbitrary user data. */
  OdStreamBufPtr userData; 

  /** \details
    Constructor for instances of the OdTfCommitAnnotation structure.

    \param author              [in] Commit author.
    \param message             [in] Commit message.
    \param date                [in] Commit timestamp.
    \param arbitraryUserData   [in] Arbitrary user data.
  */
  OdTfCommitAnnotation(
    const OdString& author = OdString::kEmpty,
    const OdString& message = OdString::kEmpty,
    const OdTimeStamp& date = OdTimeStamp(OdTimeStamp::kInitUniversalTime),
    OdStreamBuf* arbitraryUserData = 0);
};

/** \details
  Structure describing a commit relationships in the revision control database.

  \remarks
  Array of parent commits contains one record for usual commit and several for
  merge commit. In case of merge commit, the first record in the array
  corresponds to the target branch, i.e. the branch into which the merge was
  done.
*/
struct TFDLL_EXPORT OdTfCommitInfo
{
  /** Hash of the commit itself. */
  OdTfDigest hash;              
  /** Hash of the parents. */
  OdTfDigestArray parents; 
  /** Hash of the revision object. */
  OdTfDigest revision;          
  /** Describing a commit in the revision control database. */
  OdTfCommitAnnotation annotation;

  /** \details
    Constructor for instances of the OdTfCommitInfo structure.

    \param author              [in] Commit author.
    \param message             [in] Commit message.
    \param date                [in] Commit timestamp.
    \param arbitraryUserData   [in] Arbitrary user data.
  */
  OdTfCommitInfo(const OdString& author = OdString::kEmpty, const OdString& message = OdString::kEmpty, const OdTimeStamp& date = OdTimeStamp(OdTimeStamp::kInitUniversalTime), OdStreamBuf* arbitraryUserData = 0)
    :annotation(author, message, date, arbitraryUserData) {}
};

/** \details
  Merge handler interface for use with merge() and dryMerge() methods.

  This handler will be called for each change of each side encountered during a merge
  operation. It can be used to resolve the actual conflicts or to approve on any change.
  The myChange and theirChange parameter indicate the case.
  OdTfRevisionControl::isConflict() can be used to determine if the case is a conflict
  case.

  Returning a resolution preferring a side for which the object was removed, will remove
  the object from the resulting merged database.

  When returning OdTfRevisionControl::kMergePostpone it is intended that this handler
  records the conflicts for resolving them later in a manual way after the merge operation
  finished.

  \param h              [in] Handle of the (conflicting) object in my, their and
                             base database/revision.
  \param myChange       [in] Change type of the object in my database/revision compared
                             to the base revision.
  \param theirChange    [in] Change of the object in their revision compared to the base
                             revision.
  \param autoResolution [in] Proposed resolution if the conflict can be resolved
                             automatically. OdTfRevisionControl::kMergeNoResolution if the
                             conflict can not be resolved automatically and requires an
                             explicit decision.
  \returns Resolution for the conflict.

  \sa OdTfFixedMergeConflictHandler OdTfPostponeMergeConflictHandler
*/
struct OdTfMergeHandler
{
  virtual OdTfRevisionControl::MergeResolution operator()(const OdDbHandle& h,
    OdTfRevisionControl::ChangeType myChange,
    OdTfRevisionControl::ChangeType theirChange,
    OdTfRevisionControl::MergeResolution autoResolution) = 0;
  /** \details
    Interface for OdTfFixedMergeHandler.
  */
  virtual bool smartMerge() const { return false; };
  /** \details
    Interface for OdTfFixedMergeHandler.
  */
  virtual void setSmartMerge(const bool bSmartMerge) { ; };
};

/** \details
   OdTfMergeConflictHandler implementation returning a fixed conflict resolution.

   Should not be used in combination with OdTfRevisionControl::kMergePostpone for it will
   not collect the conflicts for later resolving. See OdTfPostponeMergeConflictHandler
   for that.

   \param conflictResolution The fixed conflict resolution.
*/
template<OdTfRevisionControl::MergeResolution conflictResolution = OdTfRevisionControl::kMergeTheirsConflict>
struct OdTfFixedMergeHandler : public OdTfMergeHandler
{
  /** \details
    Default constructor for instances of the OdTfFixedMergeHandler structure.
    Sets false to m_bSmartMergeCreates.
  */
  OdTfFixedMergeHandler() : m_bSmartMerge(false) {};
  /** \details
    Returns true if smart merge enable; false otherwise.
  */
  bool smartMerge() const { return m_bSmartMerge; };
  /** \details
    Set value to turn on/off smart merge.
    \param bSmartMerge [in] Set
  */
  void setSmartMerge(const bool bSmartMerge) { m_bSmartMerge = bSmartMerge; };

  OdTfRevisionControl::MergeResolution operator()(const OdDbHandle& h,
    OdTfRevisionControl::ChangeType myChange,
    OdTfRevisionControl::ChangeType theirChange,
    OdTfRevisionControl::MergeResolution autoResolution) override
  {
    return autoResolution ? autoResolution : conflictResolution;
  }
protected:
  bool m_bSmartMerge;
};

/** \details
  OdTfMergeConflictHandler implementation collecting all conflicts for resolving after the
  merge() operation finished.
*/
struct OdTfPostponeMergeHandler : public OdTfMergeHandler
{
  struct Conflict
  {
    OdDbHandle handle;
    OdTfRevisionControl::ChangeType myChange;
    OdTfRevisionControl::ChangeType theirChange;
    Conflict(const OdDbHandle& handle,
      OdTfRevisionControl::ChangeType myChange,
      OdTfRevisionControl::ChangeType theirChange)
      : handle(handle), myChange(myChange), theirChange(theirChange) {}
  };
  typedef OdArray<Conflict> Conflicts;

  Conflicts const& conflicts() const { return m_conflicts; }
  Conflicts& conflicts() { return m_conflicts; }

  OdTfRevisionControl::MergeResolution operator()(const OdDbHandle& h,
    OdTfRevisionControl::ChangeType myChange,
    OdTfRevisionControl::ChangeType theirChange,
    OdTfRevisionControl::MergeResolution autoResolution) override
  {
    if (autoResolution)
      return autoResolution;
    m_conflicts.append(Conflict(h, myChange, theirChange));
    return OdTfRevisionControl::kMergePostpone;
  }

private:
  Conflicts m_conflicts;
};

class OdTfRepository;

/** \details
  This template class is a specialization of the OdSmartPtr class for smart
  pointers to objects of the OdTfRepository class.
*/
typedef OdSmartPtr<OdTfRepository> OdTfRepositoryPtr;


struct OdTfStorage;

/** \details
  This template class is a specialization of the OdSmartPtr class for smart
  pointers to objects of the OdTfStorage structure.
*/
typedef OdSmartPtr<OdTfStorage> OdTfStoragePtr;
struct OdTfStorageImpl;

namespace OdTfRevisionControl
{
  /** \details
    Gets an existing .dsf storage.

    \param pDb [in] Database linked to the OdTfStorage.
    \returns Pointer to the OdTfRepository object. Returns a NULL pointer if the
    database is not linked to the OdTfStorage.
  */
  TFDLL_EXPORT OdTfRepository* getRepository(OdDbDatabase* pDb);

  /** \details
    Removes the link between the OdDbDatabase and the OdTfStorage.

    \param pDb [in] Database linked to the OdTfStorage.
    \returns Pointer to the OdTfRepository object. Returns a NULL pointer if the
    database is not linked to the OdTfStorage.
  */
  TFDLL_EXPORT OdTfRepositoryPtr detachRepository(OdDbDatabase* pDb);

  /** \details
    Saves changes in the database to the repository, creates a commit object and
    shifts the current branch tip if not checkout as detached. The
    "objectsToSave" optional parameter is a list of objects to save. It can be
    used to perform a partial commit.

    Includes all tracked merged commits as parents of the commit.

    \param pDb           [in] The database, which state has to be saved.
    \param message       [in] Commit message.
    \param objectsToSave [in] A list of objects to be saved. Optional. Used to
                              perform a partial commit.

    \returns Hash code of the commit.
  */
  TFDLL_EXPORT OdTfDigest commit(OdDbDatabase* pDb, const OdTfCommitAnnotation& message, const OdDbObjectIdArray* objectsToSave = 0);

  /** \details
    Dry merge operation for calculating all the changes and conflicts.

    Does not perform any real merge operation. Only useful for recording/inspecting
    callbacks to mergeHandler in order to have the full picture before performing the
    actual merge operation. The return value of the mergeHandler is ignored.

    \param pDb           [in] The database to be used as my side for the merge.
    \param sha           [in] Commit to use as their side for merge.
    \param reference     [in] Branch to use as their side for merge.
    \param mergeHandler  [in] Callback for recording/inspecting all changes.

    \returns False if merge would be a fast forward; true otherwise.
  */
  TFDLL_EXPORT bool dryMerge(const OdDbDatabase* pDb, const OdTfDigest& sha,
    OdTfMergeHandler& mergeHandler);
  TFDLL_EXPORT bool dryMerge(const OdDbDatabase* pDb, const OdString& reference,
    OdTfMergeHandler& mergeHandler);

  /** \details
    Merges changes from the repository to the database. Merge operation does not
    automatically commit changes, only runtime database is changed.

    Adds the source commit as a tracked merge parent for this drawing. This
    makes the source commit as a parent when committing the changes to the
    repository.

    \remarks If merge was fast forward, no merge commit is necessary.
      If "noFastForward" flag is true, and the remote branch is directly above
      or below this one in the commit graph, the function does nothing.

    \param pDb             [in]     A database to merge into.
    \param sha             [in]     Commit to use as their side for merge.
    \param reference       [in]     Branch to use as their side for merge.
    \param conflictHandler [in/out] Callback for handling merge conflicts.
    \param noFastForward   [in]     Flag that prohibits fast forward merging (when true).

    \returns False if merge was fast forward; true otherwise.
  */
  TFDLL_EXPORT bool merge(OdDbDatabase* pDb, const OdTfDigest& sha,
    OdTfMergeHandler& mergeHandler, bool noFastForward = false);
  TFDLL_EXPORT bool merge(OdDbDatabase* pDb, const OdString& reference,
    OdTfMergeHandler& mergeHandler, bool noFastForward = false);

  /** \details
    Returns the name of the current working branch.

    \param pDb   [in] Database to get the working branch from.
    \param head [out] Hash code of the branch head commit.

    \remarks
    If no revision control storage is associated with the database, returns
    OdString::kEmpty.

    \returns Current working branch name.
  */
  TFDLL_EXPORT OdString getLocalBranch(OdDbDatabase* pDb, OdTfDigest* head = 0);

  /** \details
    Updates the database to the revision on the tip of the specified branch and
    makes this branch default for commits. All uncommitted changes in the runtime
    database will be lost. This behavior can be modified by setting the "hard"
    flag. If "hard" is set to false and there are non-committed changes in the
    database, operation will fail. If "hard" is true (default), all
    non-committed changes will be lost.

    \param pDb        [in] Database which storage to switch the branch in.
    \param branchName [in] Name of the branch to switch to.
    \param hard       [in] Flag that defines the behavior of the function if
                           there are non-committed changes in the database.
  */
  TFDLL_EXPORT void switchBranch(OdDbDatabase* pDb, const OdString& branch, bool hard = true);

  /** \details
    Gets a list of objects that were changed since the last commit.

    \param pDb [in] Database to get the list of changes.
    \returns An array of object IDs.
  */
  TFDLL_EXPORT OdDbObjectIdArray getLocalChanges(OdDbDatabase* pDb);


  /** \details
    Creates a new OdStreamBufPtr with compressed data from the OdTfStorage
    linked to the OdDbDatabase.

    \param pDb [in] Database linked to the OdTfStorage.
    \returns New storage stream buffer.
  */
  TFDLL_EXPORT OdStreamBufPtr compressData(OdDbDatabase* pDb);

  /** \details
    Loads objects from the storage if earlier a partial checkout was performed.

    \remarks
    This function is designed to run in a separate thread.

    \param pDb      [in] Database to load objects to.
    \param pObjects [in] Iterator that contains a list of objects to be loaded.
    \param pReactor [in] Reactor for multi-threaded loading. Optional.
  */
  TFDLL_EXPORT void loadObjects(OdDbDatabase* pDb, OdDbObjectIterator* pObjects, OdMTLoadReactor* pReactor = 0);

  /** \details
    Begins partial import of the database into the revision control storage if
    the full import cannot be performed or not needed.

    \remarks
    Branch name and commit information are default.

    \param pStream [in] Stream, associated with the storage.
    \param pDb     [in] Database to be imported to the storage.
  */
  TFDLL_EXPORT void beginPartialImport(OdStreamBufPtr pStream, OdDbDatabase* pDb);

  /** \details
    Imports an object to the storage.

    \remarks
    Used only with beginPartialImport() and endPartialImport().

    \param id               [in] ID of the object to be imported to the storage.
    \param pageObjectToDisk [in] If true (default) flushes the stream buffer to
                                 the file after importing the object.
  */
  TFDLL_EXPORT void writeObjectPartial(OdDbObjectId id, bool pageObjectToDisk = true);

  /** \details
    Ends the partial import to the revision control storage creating a first
    revision.

    \param pDb [in] Database to end the partial import.
  */
  TFDLL_EXPORT void endPartialImport(OdDbDatabase* pDb);

  /** \details
    Gets a list of handles of the objects that differ between revisions.
    Revisions may actually be unrelated.

    \param pDb         [in] Database which storage to get the list of differences from.
    \param newRevision [in] Hash code of the first revision.
    \param oldRevision [in] Hash code of the second revision.
    \param result     [out] List of changes, containing IDs of objects that differ between revisions.
  */
  TFDLL_EXPORT void getRevisionDifference(OdDbDatabase* pDb, const OdTfDigest& newRevision, const OdTfDigest& oldRevision, ChangeList& result);
}

/** \details
  This structure represents the low level storage of drawing revisions.
*/
struct TFDLL_EXPORT OdTfStorage : public OdRxObject
{
/** \details
    Constructor the OdTfStorage object.
  */
  OdTfStorage();
  /** \details
    Destroys the OdTfStorage object.
  */
  ~OdTfStorage();

  /** \details
    Open an existing .dsf storage or create a new .dsf storage without history
    tracking.

    \param pStream  [in] Stream, associated with the storage.
    \param pHostApp [in] Platform specific host application services.

    \returns Pointer to the repository object corresponding to the storage.
  */
  //static OdTfStoragePtr createObject(OdStreamBuf* pStream, OdDbHostAppServices* pHostApp);

  /** \details
    Stores a .dwg database into the .dsf storage. Can be used for both the
    initial import in the repository or for integrating changes made by third
    party editors.

    The returned hash is the only way to access the stored revision of the
    drawing. There is no way to retrieve the hash of an existing revision,
    as such the hash should be stored by other external means.

    \param pDb           [in] Database to be stored in the storage.
    \param objectsToSave [in] List of objects to be saved. Optional.
                              Used to perform a partial revision.

    \returns Hash ID of the created revision in the storage.
  */
  OdTfDigest store(OdDbDatabase* pDb, const OdDbObjectIdArray* objectsToSave = 0);

  /** \details
    Retrieves a drawing database from the storage. The database is checked out
    from a specific revision from the storage. If "partial" is false, all the
    objects are loaded in memory. If they will be eventually loaded anyway, it
    is much faster not to set "partial" flag to true.

    \param sha     [in] Source revision to check out a database from.
    \param partial [in] Flag that determines whether all the objects are loaded
                        in memory (false) or not (true).

    \returns Pointer to the database corresponding to the checked out state.
  */
  OdDbDatabasePtr retrieve(const OdTfDigest& sha, bool partial = false) const;

  /** \details
    Dry merge operation for calculating all the changes and conflicts.

    Does not perform any real merge operation. Only useful for recording/inspecting
    callbacks to mergeHandler in order to have the full picture before performing the
    actual merge operation. The return value of the mergeHandler is ignored.

    \param mine         [in] Revision to be used as my side for the merge.
    \param pDb          [in] The database to be used as my side for the merge.
    \param theirs       [in] Revision to be used as their side for merge.
    \param base         [in] Common ancestor revision for my and their side.
    \param mergeHandler [in] Callback for recording/inspecting all changes.
  */
  void dryMerge(
    const OdTfDigest& mine, const OdTfDigest& theirs, const OdTfDigest& base,
    OdTfMergeHandler& mergeHandler) const;
  void dryMerge(
    const OdDbDatabase *pDb, const OdTfDigest& theirs, const OdTfDigest& base,
    OdTfMergeHandler& mergeHandler) const;

  /** \details
    Merges changes from the repository to the database. Merge operation does not
    automatically saves the changes, only runtime database is changed.

    The mine revision will be retrieve from storage and used as a target database for
    the merge. It will be returned as the result database (which is not yet stored to the
    storage).

    \param mine         [in]     Revision to be used as my side for the merge.
    \param theirs       [in]     Revision to be used as their side for the merge.
    \param base         [in]     Common ancestor revision for my and their side.
    \param mergeHandler [in/out] Callback for handling merge conflicts.
    \returns A pointer to the database containing the result of the merge of mine
             and theirs revisions.
  */
  OdDbDatabasePtr merge(
    const OdTfDigest& mine, const OdTfDigest& theirs, const OdTfDigest& base,
    OdTfMergeHandler& mergeHandler) const;

  /** \details
    Merges changes from the repository and database. Merge operation does not
    automatically saves the changes, only runtime database is changed.

    The database passes as my side will be used as the target database for the merge.
    The object changes will be calculated from this database compared to the base
    revision.

    \param pDb          [in/out] A database to merge into. This database is used as my
                                 side for the merge.
    \param theirs       [in]     Revision to be used as their side for the merge.
    \param base         [in]     Common ancestor revision for my and their side.
    \param mergeHandler [in/out] Callback for handling merge conflicts.
  */
  void merge(
    OdDbDatabase *pDb, const OdTfDigest& theirs, const OdTfDigest& base,
    OdTfMergeHandler& mergeHandler) const;

  /** \details
    Collects all the changes since the "from" set of revisions to the "to" set
    of revisions. Together with applyPatch() it is designed to transfer changes
    between storages.

    The "from" set of revisions is considered available in the remote storage.
    So, the objects reachable by these commits should not be included in the
    patch. The "to" set of revisions are the revisions that need to be made
    available on the remote. Each names a single revision to be included. So,
    if all revisions of a branch need to be included, then each revision from
    that branch needs to included in the "to" set.

    Consider following example scenarios:

    Remote:        -- C7
                 /
    C0 -- C1 -- C2 -------- C5 -- C6
           \                /     ^
             -- C3 -- C4 --     master

    Local:    master
                v
    C0 -- C1 -- C2
           \
             -- C3
                ^
             branch1

    If the local storage has revision C3 and wants to update to head of master,
    it makes a request to the remote 'update({C3}, {master})' which is
    translated on the remote to 'makePatch({C3}, {C6})'. This will only include
    the differences between revision C3 and revision C6 into the patch.

    If the local repository has two branches up to C2, C3 (C0, C1, C2, C3) and
    wants to pull the remote changes up to C6, it makes a request to the remote
    'pull({C2, C3}, master)' which is translated on the remote to
    'makePatch({C0, C1, C2, C3}, {C4, C5, C6})'. This will include all objects
    from C4, C5 and C6 in the patch not already available in any of the local
    branches.

    This can also be used for the initial clone of a repository by making a
    request to the remote 'pull({}, master)' which is thus translated to
    'makePatch({}, {C0, C1, C2, C3, C4, C5, C6})' which does not include C7 or
    'update({}, {master})' which only includes all objects for C6 in the patch.

    \param from [in] Set of revision hashes for the starting revisions.
    \param to   [in] Set of revision hashes for the ending revisions.
    \param refs [in] Set of references to be included in the patch.

    \returns Pointer to a stream buffer associated with the patch file.
  */
  OdStreamBufPtr makePatch(
    const OdTfDigestArray& from, const OdTfDigestArray& to,
    const OdTfRevisionControl::ReferenceList& refs = OdTfRevisionControl::ReferenceList()) const;

  /** \details
    Applies a patch created by makePatch() on a storage to get all the changes
    contained in the patch file transfered to the storage.

    \remarks
    applyPatch() will fail if any if the path required revisions is not
    available in the storage (makePatch() "from" parameter).

    \param s      [in] Stream buffer associated with the patch file.
    \param roots [out] Set of root objects created by the patch
                       (makePatch() "to" parameter).
    \param refs  [out] Set of references included in the patch. These won't be
                       set on the storage yet and thus require further
                       processing.
  */
  void applyPatch(OdStreamBuf* s,
    OdTfDigestArray& roots, OdTfRevisionControl::ReferenceList& refs);

  /** \details
    Gets a list of handles of the objects that differ between revisions.
    Revisions may actually be unrelated.

    \param newRevision [in] Hash code of the first revision.
    \param oldRevision [in] Hash code of the second revision.
    \param result     [out] A list of changes, containing IDs of objects that
                            differ between revisions.
  */
  void getRevisionDifference(const OdTfDigest& newRevision,
    const OdTfDigest& oldRevision, OdTfRevisionControl::ChangeList& result) const;

  /** \details
    Retrieves the stream associated with the storage.

    \returns Sream associated with the storage.
  */
  OdStreamBufPtr getStorageStream() const; 

  /** \details
    Writes a branch with its hash.

    \param refName [in] Branch name.
    \param sha     [in] Branch hash.
  */
  void setReference(const OdString& refName, const OdTfDigest& sha);

  /** \details
    Writes a set of branches with their hashes.

    \param branches [in] Set of branches with their hashes.
  */
  void setReference(const OdTfRevisionControl::ReferenceList& branches);

  /** \details
    Returns the hash of the specified branch.

    \param refName [in] Branch name.
    \param sha    [out] Hash code of the branch head commit.

    \returns False if the specified branch does not exist; true otherwise.
  */
  bool getReference(const OdString& refName, OdTfDigest& sha) const;

  /** \details
    Gets a set of branch names and their hashes from the repository.

    \param refs [out] Container with branches and their hashes.
  */
  void getReferences(OdTfRevisionControl::ReferenceList& refs) const;

protected:
  // Internal implementation not writing a header section. To be used in derived
  // makePatch() implementations.
  void makePatch(OdStreamBuf* s,
    const OdTfDigestArray& from, const OdTfDigestArray& to,
    const OdTfRevisionControl::ReferenceList& refs) const;

  protected:
    friend struct OdTfStorageImpl;
    friend void OdTfRevisionControl::beginPartialImport(OdStreamBufPtr pStream, OdDbDatabase* pDb);
    OdTfStorageImpl* m_pImpl;
};

/** \details
  Storage of drawing revisions with history tracking. Drawing databases checked
  out from this repository will have check out tracking information associated
  with it (branch tracking, parent commit, merge tracking, etc.).
*/
class TFDLL_EXPORT OdTfRepository : public OdTfStorage
{
public:
  /** \details
    Opens an existing .dsf storage or creates a new .dsf storage.

    \param pStream  [in] Stream associated with the storage.
    \param pHostApp [in] Platform specific host application services.

    \returns Pointer to the repository object corresponding to the storage.
  */
  static OdTfRepositoryPtr createObject(OdStreamBufPtr pStream, OdDbHostAppServices* pHostApp);

  /** \details
    Initializes a storage after creation by
    OdTfRepository::createObject(OdStreamBufPtr, OdDbHostAppServices*).

    \param isNewStorage [in] True to create a new storage.
  */
  void init(const bool isNewStorage);

  /** \details
    Commit a .dwg database into the .dsf repository. Can be used for both the
    initial import in the repository or for integrating changes made by third
    party editors.

    The parameter "parents" specifies the parent commits for which this database
    state is considered the next revision. "Parents" is empty for an initial
    commit, contains a single commit for a regular revision, or contains
    multiple commits for a merge revision.

    Importing does not update any references/branches so the returned hash must
    be stored as a reference, otherwise the commit will become inaccessible.

    \param pDb           [in] Database to be imported to the storage.
    \param parents       [in] Parent commits of the imported database.
    \param message       [in] Commit information.
    \param objectsToSave [in] List of objects to be saved. Optional.
                              Used to perform a partial commit.

    \returns Hash ID of the created commit in the repository.
  */
  OdTfDigest commit(OdDbDatabase* pDb, const OdTfDigestArray& parents,
    const OdTfCommitAnnotation& message = OdTfCommitAnnotation(),
    const OdDbObjectIdArray* objectsToSave = 0);

  /** \details
    The initial import to store a .dwg database into the .dsf storage. 

    \param pDb         [in] Database to be stored in the storage.
    \param localbranch [in] Branch to be set as current for commits.

    \returns Hash ID of the created commit in the repository.
  */
  OdTfDigest import(OdDbDatabase* pDb, const OdString& localbranch);

  /** \details
    Reads a database from the repository. The database is checked out from a
    specific revision from the repository. A "localBranch" sets a default branch
    for commit. If "localBranch" is an empty string, the database will be
    detached from any branch. If "partial" is false, all the objects are loaded
    in memory. If they will be eventually loaded anyway, it is much faster not
    to set "partial" flag to true.

    \param sha         [in] Source commit to check out a database from.
    \param localBranch [in] Branch to be set as current for commits.
    \param partial     [in] Flag that determines whether all the objects are
                            loaded in memory (false) or not (true).

    \returns Pointer to the database corresponding to the checked out state.
  */
  OdDbDatabasePtr checkout(const OdTfDigest& sha, const OdString& localBranch,
    bool partial = false);

  /** \details
    Convenience checkout() method using a reference as a source.

    \param reference   [in] Source branch to check out a database from.
    \param localBranch [in] Branch to be set as current for commits.
    \param partial     [in] Flag that determines whether all the objects are
                            loaded in memory (false) or not (true).

    \returns Pointer to the database corresponding to a checked out state.
  */
  OdDbDatabasePtr checkout(const OdString& reference, const OdString& localBranch, bool partial = false);

  /** \details
    Convenience checkout() method using a branch for both as source and as
    current branch for commits.

    \param branch  [in] Branch for both source and to be set as current for
                        commits.
    \param partial [in] Flag that determines whether all the objects are
                        loaded in memory (false) or not (true).

    \returns Pointer to the database corresponding to a checked out state.
  */
  OdDbDatabasePtr checkout(const OdString& branch, bool partial = false);

  /** \details
    Intermediate commits along the branch(es) are automatically included.

    Collects all the changes since the "from" set of commits to the "to" set
    of commits. Together with applyPatch() it is designed to transfer changes
    between repositories.

    The "from" set of commits is considered available in the remote repository.
    The "to" set of commits are the commits that need to be made available
    on the remote. Also "to" must contain hashes "from". Each names a single
    commit to be included. So, if all commits of a branch need to be
    included, then each commit from that branch needs to be included in the
    "to" set.

    Consider the following example scenarios:

    Remote:        -- C7
                 /
    C0 -- C1 -- C2 -------- C5 -- C6
           \                /     ^
             -- C3 -- C4 --     master

    Local:    master
                v
    C0 -- C1 -- C2
           \
             -- C3
                ^
             branch1

    If the local repository has commit C3 and wants to update to head of master,
    it makes a request to the remote 'update({C3}, {master})' which is
    translated on the remote to 'makePatch({C3}, {C6})'. This will only include
    the differences between commit C3 and commit C6 into the patch {C3, C4, C5, C6}.

    If the local repository has two branches up to C2, C3 (C0, C1, C2, C3) and
    wants to pull the remote changes up to C6, it makes a request to the remote
    'pull({C2, C3}, master)' which is translated on the remote to
    'makePatch({C0, C1, C2, C3}, {C2, C3, C4, C5, C6, C7})'. This will include
    all objects from C2, C3, C4, C5, C6 and C7 in the patch not already
    available in any of the local branches.
    C2, C3 must be included in the patch. Reason: the local repository stores C2,
    C3 without compression. But on the remote repository commits, that are not
    branch head commits, will be compressed {C0, C1, C2, C3, C4, C5}.
    Patch in the local repository replaces not compressed {C2, C3} to compressed.
    C7 must be included because it branched from C2.

    This can also be used for the initial clone of a repository by making a
    request to the remote 'pull({}, master)' which is thus translated to
    'makePatch({}, {C0, C1, C2, C3, C4, C5, C6, C7})'
    'update({}, {master})' which only includes all objects for C6 in the patch.

    \param from [in] Set of commit hashes for the starting commits.
    \param to   [in] Set of commit hashes for the ending commits.
    \param refs [in] Set of references to be included in the patch.

    \returns Pointer to the stream buffer associated with the patch file.
  */
  OdStreamBufPtr makePatch(
    const OdTfDigestArray& from, const OdTfDigestArray& to,
    const OdTfRevisionControl::ReferenceList& refs = OdTfRevisionControl::ReferenceList()) const;

  /** \details
    Intermediate commits along the branch(es) are automatically included.

    Collects all the changes since the "from" set of commits to the "to" set
    of commits. Together with applyPatch() it is designed to transfer changes
    between repositories.

    The "from" set of commits is considered available in the remote repository.
    The "to" set of commits are the commits that need to be made available
    on the remote. Also "to" must contain hashes "from". Each names a single
    commit to be included. So, if all commits of a branch need to be
    included, then each commit from that branch needs to be included in the
    "to" set.

    Consider the following example scenarios:

    Remote:        -- C7
                 /
    C0 -- C1 -- C2 -------- C5 -- C6
           \                /     ^
             -- C3 -- C4 --     master

    Local:    master
                v
    C0 -- C1 -- C2
           \
             -- C3
                ^
             branch1

    If the local repository has commit C3 and wants to update to head of master,
    it makes a request to the remote 'update({C3}, {master})' which is
    translated on the remote to 'makePatch({C3}, {C6})'. This will only include
    the differences between commit C3 and commit C6 into the patch {C3, C4, C5, C6}.

    If the local repository has two branches up to C2, C3 (C0, C1, C2, C3) and
    wants to pull the remote changes up to C6, it makes a request to the remote
    'pull({C2, C3}, master)' which is translated on the remote to
    'makePatch({C0, C1, C2, C3}, {C2, C3, C4, C5, C6, C7})'. This will include
    all objects from C2, C3, C4, C5, C6 and C7 in the patch not already
    available in any of the local branches.
    C2, C3 must be included in the patch. Reason: the local repository stores C2,
    C3 without compression. But on the remote repository commits, that are not
    branch head commits, will be compressed {C0, C1, C2, C3, C4, C5}.
    Patch in the local repository replaces not compressed {C2, C3} to compressed.
    C7 must be included because it branched from C2.

    This can also be used for the initial clone of a repository by making a
    request to the remote 'pull({}, master)' which is thus translated to
    'makePatch({}, {C0, C1, C2, C3, C4, C5, C6, C7})'
    'update({}, {master})' which only includes all objects for C6 in the patch.

    \param from [in] Set of commit hashes for the starting commits.
    \param to   [in] Commit hash of the end commit.
    \param refs [in] Set of references to be included in the patch.

    \returns Pointer to the stream buffer associated with the patch file.
  */
  OdStreamBufPtr makePatch(
    const OdTfDigestArray& from, const OdTfDigest& to,
    const OdTfRevisionControl::ReferenceList& refs = OdTfRevisionControl::ReferenceList()) const;

  /**
    Reads from the storage a structure describing relationships of a commit in
    the revision control database.

    \param sha [in] Commit hash.

    \returns Structure describing a commit relationships in the revision control
    database.
  */
  OdTfCommitInfo getCommitInfo(const OdTfDigest& sha) const;

  /**
    Writes to the storage a structure describing relationships of a commit in
    the revision control database.

    \param commitInfo [in] Structure describing relationships of a commit in the
                           revision control database.

    \returns Hash of the write commit.
    \remarks
    Also the hash of the write commit is stored in the current OdTfCommitInfo
    structure (OdTfCommitInfo::hash field in the commitInfo parameter).
  */
  OdTfDigest writeCommitInfo(OdTfCommitInfo& commitInfo);
};

#endif //_TFREVISIONCONTROL_H_INCLUDED_
