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
// RevisionController.cpp
//
#include "OdaCommon.h"
#include "RevisionController.h"

#include "DbHostAppServices.h"
#include "SharedPtr.h"
#define STL_USING_ALGORITHM
#include "OdaSTL.h"

OdqRevisionController::OdqRevisionController()
{
  //m_chPrefixDelimiter = L'\0'; // is off
  m_isInitialized = true;
  if (//::odrxDynamicLinker()->loadApp(OdUndoHistory).isNull() || ::odrxDynamicLinker()->loadApp(TfCoreModuleName).isNull() ||
      ::odrxDynamicLinker()->loadApp(TfModuleName).isNull()) // L"TD_Tf" // odTfInitialize will be call later via OdTigDbModuleImpl::setBaseHostAppServices
  {
    ODA_FAIL_ONCE();
    m_isInitialized = false;
    return;
  }
  m_pStorages = new TMapStorages();
}

bool OdqRevisionController::isValid() const
{
  return m_isInitialized;
}

OdqRevisionController::~OdqRevisionController()
{
  closeAllStorages();
}

//void OdqRevisionController::setPrefixDelimiter(OdChar prefixDelimiter) // L'\0' is off
//{
//  // unfortunately there is no other way to get branche in correct order & and real branch of commit
//  // except prefix part in branch label & commit description
//  m_chPrefixDelimiter = prefixDelimiter;
//}

void OdqRevisionController::closeAllStorages()
{
  m_pStorages = NULL;
}

void OdqRevisionController::closeStorage(OdRxObject* pRxDb)
{
  if (isResponsible(pRxDb))
  {
    //OdDbDatabasePtr pDb = OdDbDatabasePtr(pRxDb);
    m_pStorages->erase(m_pStorages->find(pRxDb));
    //OdTfRevisionControl::compressRevisionData(pDb.get());
  }
}

bool OdqRevisionController::isResponsible(OdRxObject* pRxDb)
{
  if (!pRxDb || !pRxDb->isKindOf(OdDbDatabase::desc()))
    return false;
  if (m_pStorages->find(pRxDb) == m_pStorages->end())
    return false; // unfortunately it is possible via close db after execute command saveastig
  return true;
}

OdRxObjectPtr OdqRevisionController::open(OdDbHostAppServices* pHostApp, const OdString& sPath)
{
  // it works without schema registration (+ conversion of TD.txds schema is broken now)
  ////bool isSchemaMissing = ::odrxGetSchemaManager()->getSchemaPath().isEmpty();
  //bool isSchemaMissing = true;
  //OdRxObjectPtr pSchemeService = ::odrxServiceDictionary()->getAt(L"ODRX_SCHEMA_SERVICES");
  //class OdExSchemaManager : public OdTfSchemaManager
  //{
  //public: 
  //  bool isRegisteredSchema(const OdString& schemeName = OdString::kEmpty)
  //  {
  //    if (schemeName.isEmpty())
  //      return m_setNames.size() != 0;
  //    return m_setNames.find(schemeName) != m_setNames.end(); 
  //  }
  //};
  //if (pSchemeService.get() && ((OdExSchemaManager*) pSchemeService.get())->isRegisteredSchema())
  //  isSchemaMissing = false;
  //if (isSchemaMissing)
  //{
  //  OdString sPath = pHostApp->findFile(L"TD.txds");
  //  if (sPath.isEmpty())
  //    sPath = pHostApp->findFile(L"TD.tsbf"); // "TD.oda");
  //  if (sPath.isEmpty() || !odrxRegisterSchema(sPath)) // ::odrxGetSchemaManager()->setSchemaPath(sPath);
  //  {
  //    ODA_FAIL_ONCE(); // ODA_ASSERT_ONCE(!sPath.isEmpty());  // is requared for merge but missing
  //  }
  //}

  OdRxObjectPtr pRxDb;
  try
  {
    OdDbDatabasePtr pDb;
    OdStreamBufPtr pFileBuf = odSystemServices()->createFile(sPath, Oda::FileAccessMode(Oda::kFileWrite | Oda::kFileRead), Oda::kShareDenyNo);
    if (!pFileBuf.isNull())
    {
      pDb = pHostApp->createDatabase(false);
      OdTfRepositoryPtr r = OdTfRepository::createObject(pFileBuf, pHostApp);
      pFileBuf = 0;
      pDb = r->checkout(OdString(L"master"), OdString(L"master"));
    }

    pDb->disableUndoRecording(false); // workaround for problem with undo
    pRxDb = OdRxObject::cast(pDb);
    m_pStorages->insert(pRxDb);

    OdStringArray allBranchesName;
    OdCommitDescriptionList lstRevisions;
    OdqRevisionController::loadBranches(OdRxObject::cast(pDb), allBranchesName, lstRevisions);
    // initially current in revision control is latest record
    if (lstRevisions.size())
    {
      OdCommitDescription& revLast = lstRevisions[0];
      if (!revLast.branches.contains(L"master"))
      {
        if (!loadDatabaseAtRevision(pRxDb, revLast))
        {
          ODA_FAIL_ONCE();
        }
      }
    }
  }
  catch (OdError&)
  {
    ODA_FAIL_ONCE();
    pRxDb = NULL;
  }
  return pRxDb;
}

bool OdqRevisionController::loadDatabase(OdRxObjectPtr pRxDb, const OdString& sPath)
{
  OdStreamBufPtr pFileBuf = odSystemServices()->createFile(sPath, Oda::FileAccessMode(Oda::kFileWrite | Oda::kFileRead), Oda::kShareDenyNo);
  OdDbDatabasePtr pDb = OdDbDatabase::cast(pRxDb);
  OdTfRepositoryPtr r = OdTfRepository::createObject(pFileBuf, pDb->appServices());
  pFileBuf = 0;
  pDb = r->checkout(OdString(L"master"), OdString(L"master"));
  m_pStorages->insert(pDb);
  return true;
}

bool OdqRevisionController::loadDatabaseAtRevision(OdRxObjectPtr pRxDb, OdCommitDescription& info)
{
  OdDbDatabasePtr pDb = OdDbDatabase::cast(pRxDb);
  try
  {
    OdString sBranch = currentBranchName(pRxDb);
    OdTfDigest branchTopSha;
    OdTfRevisionControl::getRepository(pDb)->getReference(sBranch, branchTopSha);
    
    if (branchTopSha == info.commit.hash)//if (!info.branches.contains(sBranch))
    {
      sBranch = info.branches[0];
      OdTfRevisionControl::switchBranch(pDb, sBranch, true);
    }
    else
    {
      OdTfRevisionControl::getRepository(pDb)->setReference(sBranch, info.commit.hash);
      OdTfRevisionControl::switchBranch(pDb, sBranch, true);
      OdTfRevisionControl::getRepository(pDb)->setReference(sBranch, branchTopSha);
    }
    pDb->disableUndoRecording(false); // workaround for problem with undo
  }
  catch (OdError&)
  {
    ODA_FAIL_ONCE();
    return false;
  }
  return true;
}

OdRxObjectPtr OdqRevisionController::checkoutDatabase(OdRxObjectPtr pRxDb, OdCommitDescription& info, OdDbHostAppServices* pHostApp)
{
  OdDbDatabasePtr pDb = OdDbDatabase::cast(pRxDb);
  try
  {
    OdTfRepository* pRep = OdTfRevisionControl::getRepository(pDb);
    ODA_ASSERT_ONCE(pRep);
    pDb = pRep->checkout(OdString(L"master"), OdString(L"master"));//todo some strange
    OdTfRevisionControl::switchBranch(pDb, info.branches[0], true);
  }
  catch (OdError&)
  {
    ODA_FAIL_ONCE();
    return false;
  }

  return OdRxObject::cast(pDb);
}

// TODO: wtf is that?
OdRxObjectPtr OdqRevisionController::checkoutDatabaseFix(OdRxObjectPtr pRxDb, OdCommitDescription& info, OdCommitDescription& infoInitial, OdDbHostAppServices* pHostApp)
{
  OdDbDatabasePtr pDb = OdDbDatabase::cast(pRxDb);
  try
  {
    OdTfRepository* pRep = OdTfRevisionControl::getRepository(pDb);
    ODA_ASSERT_ONCE(pRep);
    pDb = pRep->checkout(OdString(L"master"), OdString(L"master"));//todo some MORE strange
    OdTfRevisionControl::switchBranch(pDb, infoInitial.branches[0], true);
    OdTfRevisionControl::switchBranch(pDb, info.branches[0], true);
  }
  catch (OdError&)
  {
    ODA_FAIL_ONCE();
  }

  return OdRxObject::cast(pDb);
}

OdString OdqRevisionController::currentBranchName(OdRxObject* pRxDb)
{
  OdDbDatabasePtr pDb = OdDbDatabase::cast(pRxDb);
  if (pDb.isNull())
    return OdString::kEmpty;

  //int pos = -1;
  //if (m_chPrefixDelimiter && (pos = currentBranch.reverseFind(m_chPrefixDelimiter)) >= 0)
  //  currentBranch = currentBranch.mid(++pos);
  return OdTfRevisionControl::getLocalBranch(pDb);
}

OdCommitDescription OdqRevisionController::getDescriptionWithBranch(OdDbDatabase* pStorage, OdTfDigest *digest, OdTfCommitInfo& commit,
                                                                    const OdString& branch, 
                                                                    bool isHead) // = false
{
  OdCommitDescription desc;
  OdString time;
  commit.annotation.date.strftime("%m/%d/%Y %X", time);

  if (digest) {
    desc.id = OdString(digest->data, sizeof(digest->data) / sizeof(digest->data[0]));
    desc.sha = *digest;
    desc.isHead = isHead;
  }

  desc.author = commit.annotation.author;
  
  desc.msg = commit.annotation.message;
  //int pos = -1;
  //if (m_chPrefixDelimiter && (pos = desc.msg.reverseFind(m_chPrefixDelimiter)) >= 0)
  //{
  //  desc.branchByPrefix = desc.msg.left(pos);
  //  desc.msg = desc.msg.mid(++pos);
  //}
  desc.date = time;
  desc.branches.push_back(branch);

  OdTfDigest tmp;
  OdTfRevisionControl::ChangeList changeList;
  for (unsigned i = 0; i < commit.parents.size(); ++i) {
    tmp = commit.parents[i];
    desc.parents.push_back(OdString(tmp.data, sizeof(tmp.data) / sizeof(tmp.data[0])));
    OdTfCommitInfo com1 = OdTfRevisionControl::getRepository(pStorage)->getCommitInfo(*digest);
    OdTfCommitInfo com2 = OdTfRevisionControl::getRepository(pStorage)->getCommitInfo(tmp);
    OdTfRevisionControl::getRepository(pStorage)->getRevisionDifference(com1.revision, com2.revision, changeList);
  }
  desc.changeList = changeList;

  desc.commit = commit;

  return desc;
}

static OdCommitDescription* FindDesc(const OdTfDigest& key,
                                     OdCommitDescriptionList& lstRevisions,
                                     std::set<OdTfDigest>& added)
{
  if (added.find(key) == added.end())
    return NULL;
  for (OdUInt32 idx = 0, sz = lstRevisions.size(); idx < sz; idx++)
  {
    OdCommitDescription& desc = lstRevisions[idx];
    if (desc.sha == key)
      return &desc;
  }
  ODA_FAIL_ONCE(); // invalid set added
  return NULL;
}

void OdqRevisionController::addCommitParentsWithBranch(OdDbDatabase* pStorage,
                                                       OdCommitDescriptionList& lstRevisions,
                                                       OdTfCommitInfo& c,
                                                       std::set<OdTfDigest>& added, 
                                                       const OdString& branch) 
{
  for (unsigned i = 0; i < c.parents.size(); ++i)
  {
    //if (added.find(c.parents[i]) != added.end())
    //  continue;
    OdCommitDescription* pDesc = FindDesc(c.parents[i], lstRevisions, added);
    if (!pDesc)
    {
      added.insert(c.parents[i]);
      OdTfCommitInfo cp = OdTfRevisionControl::getRepository(pStorage)->getCommitInfo(c.parents[i]);
      OdCommitDescription info = getDescriptionWithBranch(pStorage, &c.parents[i], cp, branch);
      lstRevisions.append(info);
      addCommitParentsWithBranch(pStorage, lstRevisions, cp, added, branch);
    }
    else if (!pDesc->branches.contains(branch))
    {
      pDesc->branches.push_back(branch);
      addCommitParentsWithBranch(pStorage, lstRevisions, pDesc->commit, added, branch);
    }
  }
}

//struct DigestComparator {
//  bool operator()(const OdTfDigest &obj1, const OdTfDigest &obj2) const {
//    return obj1 < obj2;
//  }
//};

struct RevListComparator {
  bool operator() (const OdCommitDescription &obj1, const OdCommitDescription &obj2) const {
    //if (obj1.commit.date == obj2.commit.date)
    //  return c1.branch == L"master";
    //return obj1.commit.date > obj2.commit.date; // return obj1.date > obj2.date;
    return obj1.commit.annotation.date.packedValue() >  obj2.commit.annotation.date.packedValue();
  }
} revListComparator;


void OdqRevisionController::loadBranches(OdRxObject* pRxDb, OdStringArray& allBranchesName, 
                                         OdCommitDescriptionList& lstRevisions)
{
  allBranchesName.clear();
  lstRevisions.clear();
  OdDbDatabasePtr pDb = OdDbDatabase::cast(pRxDb);
  if (pDb.isNull())
    return;

  // see also cloud: void GetBranches(CMDCTX *ctx)
  
  OdTfRevisionControl::ReferenceList mapBranches;
  OdTfRevisionControl::getRepository(pDb)->getReferences(mapBranches);

  OdStringArray lstBranches;
  for (std::map<OdString, OdTfDigest>::iterator itr = mapBranches.begin(); itr != mapBranches.end(); itr++)
    lstBranches.push_back(itr->first);
  //if (m_chPrefixDelimiter)
  //{
  //  // do it ordered via delimiter
  //}

  std::set<OdTfDigest> added;

  for (OdUInt32 idx = 0, sz = lstBranches.size(); idx < sz; idx++)
  {
    const OdString& branch = lstBranches[idx];
    OdTfDigest top = mapBranches[branch];

    //int pos = -1;
    //if (m_chPrefixDelimiter && (pos = branch.reverseFind(m_chPrefixDelimiter)) >= 0)
    //  allBranchesName.append(branch.mid(++pos));
    //else
      allBranchesName.append(branch);

    OdTfCommitInfo cp = OdTfRevisionControl::getRepository(pDb)->getCommitInfo(top);

    ////OdArray<OdTfDigest> digest = cp.parents;
    //if (added.find(top) == added.end()) 
    OdCommitDescription* pDesc = FindDesc(top, lstRevisions, added);
    if (!pDesc)
    {
      added.insert(top);
      OdCommitDescription info = getDescriptionWithBranch(pDb, &top, cp, branch, true);
      lstRevisions.append(info);
    }
    else if (!pDesc->branches.contains(branch))
    {
      pDesc->branches.push_back(branch);
      //addCommitParentsWithBranch(pStorage, lstRevisions, pDesc->commit, added, branch);
    }

    //lstRevisions.append(getDescriptionWithBranch(pStorage, &top, cp, branch)); // currentBranch));

    addCommitParentsWithBranch(pDb, lstRevisions, cp, added, branch);
  }
//#if _MSC_VER > 1600
//  std::sort(lstRevisions.begin(), lstRevisions.end(), 
//            [](const OdCommitDescription& c1, const OdCommitDescription& c2) -> bool { 
//                 //if (c1.commit.date == c2.commit.date)
//                 //  return c1.branch == L"master";
//                 return c1.commit.date > c2.commit.date;
//              });
//#endif
  std::sort(lstRevisions.begin(), lstRevisions.end(), revListComparator);
}

// see also cloud  // GetRevDiff(CMDCTX *ctx) 
void OdqRevisionController::getDifferences(OdRxObject* pRxDb, // GetRevDiff(CMDCTX *ctx) 
                                           OdCommitDescription& infoFirst, OdCommitDescription& infoSecond,
                                           OdStringArray& entities, OdStringArray& changeTypes,
                                           OdStringArray* pHandles, // = NULL
                                           OdDbObjectPtr* ppRxFirstDb, // = NULL, 
                                           OdDbObjectPtr* ppRxSecondDb) // = NULL
{
  entities.clear();
  changeTypes.clear();
  if (pHandles)
    pHandles->clear();

  OdDbObjectPtr pRxFirst, pRxSecond;
  OdDbObjectPtr& pRxFirstDb = ppRxFirstDb ? *ppRxFirstDb : pRxFirst;
  OdDbObjectPtr& pRxSecondDb = ppRxSecondDb ? *ppRxSecondDb : pRxSecond;

  OdDbDatabasePtr pDb = OdDbDatabase::cast(pRxDb);
  if (pDb.isNull())
    return;


  //OdString revFirst = ctx->userIO()->getString("Enter revision");
  //OdString revSecond = ctx->userIO()->getString("Enter second");
  //OdTfDigest digestFirst;
  //OdTfDigest digestSecond;
  //if (revFirst.isEmpty() || revSecond.isEmpty())
  //  throw OdError("Revision id is cannot be empty");
  //OdAnsiString res = decodeBase64(OdAnsiString((const char*)revFirst));
  //memcpy(digestFirst.data, res.c_str(), sizeof(digestFirst.data) / sizeof(digestFirst.data[0]));
  //res = decodeBase64(OdAnsiString((const char*)revSecond));
  //memcpy(digestSecond.data, res.c_str(), sizeof(digestSecond.data) / sizeof(digestSecond.data[0]));

  OdTfRevisionControl::ChangeList changeList;
  OdTfCommitInfo com1 = OdTfRevisionControl::getRepository(pDb)->getCommitInfo(infoSecond.sha);
  OdTfCommitInfo com2 = OdTfRevisionControl::getRepository(pDb)->getCommitInfo(infoFirst.sha);
  OdTfRevisionControl::getRepository(pDb)->getRevisionDifference(com1.revision, com2.revision, changeList);

  for (OdTfRevisionControl::ChangeList::iterator itr = changeList.begin();
       itr != changeList.end(); itr++)
  {
    // "handle"
    if (pHandles)
      pHandles->push_back(itr->first.ascii());


    // "type" // "Entity"
    OdString sEntity;

    OdDbObjectPtr pObj = pDb->getOdDbObjectId(itr->first).openObject(OdDb::kForRead, true);
    if (pObj.isNull())
    {
      if (pRxFirstDb.isNull())
        pRxFirstDb = checkoutDatabase(pRxDb, infoFirst, pDb->appServices());
      if (pRxFirstDb.get())
        pObj = OdDbDatabase::cast(pRxFirstDb)->getOdDbObjectId(itr->first).openObject(OdDb::kForRead, true);
      if (pObj.isNull())
      {
        if (pRxSecondDb.isNull())
          pRxSecondDb = checkoutDatabase(pRxDb, infoSecond, pDb->appServices());
        if (pRxSecondDb.get())
          pObj = OdDbDatabase::cast(pRxSecondDb)->getOdDbObjectId(itr->first).openObject(OdDb::kForRead, true);
      }
    }
    if (pObj.get())
      sEntity = pObj->isA()->name();
    else
      sEntity = L"Undefined";
    if (!sEntity.find(L"AcDb"))
      sEntity = sEntity.mid(4);
    entities.push_back(sEntity);

    // "diff" // "Change type"
    OdString sType;
    if (itr->second == OdTfRevisionControl::kObjectAdded)
      sType = L"Added";
    else if (itr->second == OdTfRevisionControl::kObjectModified)
      sType = L"Modified";
    else
      sType = L"Deleted";
    changeTypes.push_back(sType);
  }
}

void OdqRevisionController::loadCommitDifference(OdRxObject* pRxDb, OdDbHandleList& list, 
                                                 OdTfDigest sha1, OdTfDigest sha2)
{
  list.clear();
  OdDbDatabasePtr pDb = OdDbDatabase::cast(pRxDb);
  if (pDb.isNull())
    return;

  OdString currentBranch = OdTfRevisionControl::getLocalBranch(pDb);
  if (currentBranch.isEmpty())
    return;
  OdTfRevisionControl::ChangeList changeList;
  OdTfCommitInfo com1 = OdTfRevisionControl::getRepository(pDb)->getCommitInfo(sha1);
  OdTfCommitInfo com2 = OdTfRevisionControl::getRepository(pDb)->getCommitInfo(sha2);
  OdTfRevisionControl::getRepository(pDb)->getRevisionDifference(com1.revision, com2.revision, changeList);
  auto it = changeList.begin();
  while( it != changeList.end() )
  {
    list.push_back( it->first );
    ++it;
  }
}

bool OdqRevisionController::commit(OdRxObject* pRxDb, const OdString& author, const OdString& message, 
                                   OdCommitDescription* pInfoFrom, // = NULL // for parent & branch
                                   OdString* psError) // = NULL
{
  OdString sUnused;
  OdString& sError = psError ? *psError : sUnused;
  sError.empty();

  OdDbDatabasePtr pDb = OdDbDatabase::cast(pRxDb);
  if (pDb.isNull())
    return false;

  OdString currentBranch = OdTfRevisionControl::getLocalBranch(pDb);
  if (currentBranch.isEmpty())
  {
    sError = L"Database is not under revision control";
    return false;
  }

  OdTimeStamp date(OdTimeStamp::kInitUniversalTime);
  try
  {
    if (pInfoFrom)
    {
      ODA_ASSERT_ONCE(pInfoFrom->branches.size() == 1);

      OdString branch = pInfoFrom->branches[0];
      ODA_ASSERT_ONCE(!branch.isEmpty() && branch != currentBranch);
      OdTfDigest* otherParent = &pInfoFrom->sha;
      OdTfRevisionControl::commit(pDb, OdTfCommitAnnotation(author, message, date));
    }
    else
      //OdTfRevisionControl::commit(pDb, author, message, date);
      OdTfRevisionControl::commit(pDb, OdTfCommitAnnotation(author, message, date));
  }
  catch (OdError& err)
  {
    ODA_ASSERT_ONCE(psError);
    sError = err.description();
    return false;
  }
  return true;
}

bool OdqRevisionController::createBranch(OdRxObject* pRxDb, const OdString& branch,
                                         OdCommitDescription* pInfoAt, // = NULL
                                         OdString* psError) // = NULL
{
  OdString sUnused;
  OdString& sError = psError ? *psError : sUnused;
  sError.empty();

  OdDbDatabasePtr pDb = OdDbDatabase::cast(pRxDb);
  if (pDb.isNull() || branch.isEmpty())
    return false;

  OdTfDigest sha;
  OdString currentBranch = OdTfRevisionControl::getLocalBranch(pDb, &sha);
  if (currentBranch.isEmpty())
    return false;

  ODA_ASSERT_ONCE(!pInfoAt || pInfoAt->branches.contains(currentBranch));

  //if (m_chPrefixDelimiter)
  //{
  //}

  OdTfDigest shaNew;
  if (OdTfRevisionControl::getRepository(pDb)->getReference(branch, shaNew))
  {
    sError = L"Branch already exists.";
    return false;
  }
  ODA_ASSERT_ONCE(currentBranch != branch);

  if (pInfoAt && sha != pInfoAt->sha)
    sha = pInfoAt->sha;

  OdTfRevisionControl::getRepository(pDb)->setReference(branch, sha);// create new branch pointing at he same revision

  try
  {
    OdTfRevisionControl::switchBranch(pDb, branch, true);
  }
  catch (OdError&)
  {
    ODA_ASSERT_ONCE(psError);
    return false;
  }

  ODA_ASSERT_ONCE(OdTfRevisionControl::getLocalBranch(pDb) == branch);
  return true;
}

bool OdqRevisionController::switchToBranch(OdRxObject* pRxDb, const OdString& branch, OdString* psError) // = NULL
{
  OdString sUnused;
  OdString& sError = psError ? *psError : sUnused;
  sError.empty();

  OdDbDatabasePtr pDb = OdDbDatabase::cast(pRxDb);
  if (pDb.isNull() || branch.isEmpty())
    return false;

  OdString currentBranch = OdTfRevisionControl::getLocalBranch(pDb);
  if (currentBranch.isEmpty())
    return false;
  if (currentBranch == branch)
  {
    sError = L"There is nothing to switch";
    return false;
  }

  try
  {
    OdTfRevisionControl::switchBranch(pDb, branch, true);
    ODA_ASSERT_ONCE(OdTfRevisionControl::getLocalBranch(pDb) == branch);
  }
  catch (OdError& err)
  {
    ODA_ASSERT_ONCE(psError);
    sError = err.description();
    return false;
  }
  return true;
}

bool OdqRevisionController::merge(OdRxObject* pRxDb, const OdString& branch, 
                                  bool bNoFastForward, // = false
                                  OdString* psError) // = NULL
{
  OdString sUnused;
  OdString& sError = psError ? *psError : sUnused;
  sError.empty();

  OdDbDatabasePtr pDb = OdDbDatabase::cast(pRxDb);
  if (pDb.isNull() || branch.isEmpty())
    return false;

  OdString currentBranch = OdTfRevisionControl::getLocalBranch(pDb);
  if (currentBranch.isEmpty())
    return false;
  if (currentBranch == branch)
  {
    sError = L"There is nothing to merge";
    return false;
  }

  OdHandleArray conflicts;
  bool bRes = false;
  try
  {
    bRes = OdTfRevisionControl::merge(pDb, branch, OdTfFixedMergeHandler<>(), bNoFastForward);
  }
  catch (OdError& err)
  {
    ODA_ASSERT_ONCE(psError);
    sError = err.description();
  }
  catch (...)
  {
    ODA_ASSERT_ONCE(psError);
    sError = L"Unknown exception";
  }
  return bRes;
}

bool OdqRevisionController::import(OdRxObject* pRxDb, const OdString& sTigFileName,
                                   OdString* psError) // = NULL
{
  OdString sUnused;
  OdString& sError = psError ? *psError : sUnused;
  sError.empty();

  OdDbDatabasePtr pDb = OdDbDatabase::cast(pRxDb);
  if (pDb.isNull())
    return false;

  OdString currentBranch = OdTfRevisionControl::getLocalBranch(pDb);
  if (!currentBranch.isEmpty())
  {
    sError = L"Database is already under revision control";
    return false;
  }
  OdStreamBufPtr buff = odSystemServices()->createFile( sTigFileName, Oda::FileAccessMode(Oda::kFileWrite | Oda::kFileRead), Oda::kShareDenyWrite, Oda::kCreateAlways);
  OdTfRepositoryPtr pRep = OdTfRepository::createObject(buff, pDb->appServices());
  OdTfDigest sha = pRep->commit(pDb, OdTfDigestArray());
  pRep->setReference(L"master", sha);
  return true;
}

bool OdqRevisionController::exportToDwg(OdRxObject* pRxDb, const OdString& sToFileName,
                                        OdDb::DwgVersion fileVersion, // = OdDb::kDHL_MC0_0 // by content
                                        OdString* psError) // = NULL
{
  OdString sUnused;
  OdString& sError = psError ? *psError : sUnused;
  sError.empty();

  OdDbDatabasePtr pDb = OdDbDatabase::cast(pRxDb);
  if (pDb.isNull() || sToFileName.isEmpty())
    return false;

  OdString currentBranch = OdTfRevisionControl::getLocalBranch(pDb);
  if (currentBranch.isEmpty())
  {
    sError = L"Database is not under revision control";
    return false;
  }

  OdStreamBufPtr pStreamBuf = ::odrxSystemServices()->createFile(sToFileName, Oda::kFileWrite, Oda::kShareDenyNo, Oda::kCreateAlways);
  if (!fileVersion)
    fileVersion = pDb->version();

  pDb->writeFile(pStreamBuf, OdDb::kDwg, fileVersion);
  return true;
}
