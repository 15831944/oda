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
// RevisionController.h
// 

#ifndef ODA_REVISION_CONTROLLER_H
#define ODA_REVISION_CONTROLLER_H

#include "Tf/TfRevisionControl.h"
#include "Tf/TfSchemaManager.h"

#include "StringArray.h"
#include "SharedPtr.h"
#define STL_USING_SET
#define STL_USING_MAP
#include "OdaSTL.h"

typedef std::set<OdRxObjectPtr> TMapStorages;

struct OdCommitDescription {
  OdString id; // is used for cloud only
  OdTfDigest sha; // is used for TV only
  OdTfCommitInfo commit;

  OdString author,
           msg,
           date;
  OdStringArray branches,
                parents;
  OdString branchByPrefix; // is prefix delimeter is on only

  OdTfRevisionControl::ChangeList changeList;
  bool isHead;

  OdCommitDescription() {
    isHead = false;
  }
} ; // CommitDescription in cloud

typedef OdArray<OdCommitDescription> OdCommitDescriptionList;

typedef OdArray<OdDbHandle> OdDbHandleList;

class OdDbDatabase;
class OdqRevisionController
{
public:
  OdqRevisionController();
  ~OdqRevisionController();

  bool isValid() const;
  void closeAllStorages();
  void closeStorage(OdRxObject* pRxDb);

  bool isResponsible(OdRxObject* pRxDb);
  //void setPrefixDelimiter(OdChar prefixDelimiter); // should be before open if it is used

  OdRxObjectPtr open(OdDbHostAppServices* pHostApp, const OdString& path);

  bool loadDatabase(OdRxObjectPtr pRxDb, const OdString& path);
  bool loadDatabaseAtRevision(OdRxObjectPtr pRxDb, OdCommitDescription& info);

  // used for diffs only // TODO check to redesign it
  OdRxObjectPtr checkoutDatabase(OdRxObjectPtr pRxDb, OdCommitDescription& info, OdDbHostAppServices* pHostApp);
  //temporary fix for incorrect model space checkout in case on removing objects
  OdRxObjectPtr checkoutDatabaseFix(OdRxObjectPtr pRxDb, OdCommitDescription& info, OdCommitDescription& infoInitial, OdDbHostAppServices* pHostApp);

  OdString currentBranchName(OdRxObject* pRxDb); // returns branch name of last checkout for this database
  void loadBranches(OdRxObject* pRxDb, OdStringArray& lstNames, OdCommitDescriptionList& list);

  // see also cloud  // GetRevDiff(CMDCTX *ctx) 
  void getDifferences(OdRxObject* pRxDb,
                      OdCommitDescription& infoFirst, OdCommitDescription& infoSecond,
                      OdStringArray& entities, OdStringArray& changeTypes, OdStringArray* pHandles, // out
                      OdDbObjectPtr* ppRxMainDb = NULL, OdDbObjectPtr* ppRxSecondDb = NULL);

  void loadCommitDifference(OdRxObject* pRxDb, OdDbHandleList& list, OdTfDigest sha1, OdTfDigest sha2);

  bool commit(OdRxObject* pRxDb, const OdString& author, const OdString& message, 
              OdCommitDescription* pInfoFrom = NULL, // for parent & branch
              OdString* psError = NULL);
  bool createBranch(OdRxObject* pRxDb, const OdString& branch, OdCommitDescription* pInfoAt = NULL, OdString* psError = NULL);
  bool switchToBranch(OdRxObject* pRxDb, const OdString& branch, OdString* psError = NULL);
  bool merge(OdRxObject* pRxDb, const OdString& branch, bool bNoFastForward = false, OdString* psError = NULL);

  bool import(OdRxObject* pRxDb, const OdString& sTigFileName, OdString* psError = NULL);
  bool exportToDwg(OdRxObject* pRxDb, const OdString& sToFileName, 
                   OdDb::DwgVersion fileVersion = OdDb::kDHL_MC0_0, // by content
                   OdString* psError = NULL);

protected:
  OdCommitDescription getDescriptionWithBranch(OdDbDatabase* pStorage, OdTfDigest *digest, OdTfCommitInfo& commit,
                                               const OdString& branch, bool isHead = false);
  void addCommitParentsWithBranch(OdDbDatabase* pStorage, OdCommitDescriptionList& lstRevisions,
                                  OdTfCommitInfo& c, std::set<OdTfDigest>& added, const OdString& branch);

  bool m_isInitialized;
  OdSharedPtr<TMapStorages> m_pStorages;
  //OdChar m_chPrefixDelimiter;
};

#endif // ODA_REVISION_CONTROLLER_H
