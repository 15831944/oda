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

#include "TfRevModule.h"
#include "DbCircle.h"
#include "DbBlockTableRecord.h"
#include "DebugStuff.h"
#include "MemoryStream.h"


ODRX_DEFINE_DYNAMIC_MODULE(OdRcModule);


void executeCommit(void* pCtx, OdDbDatabase* m_pDb)
{
  OdEdCommandContext* pCmdCtx = (OdEdCommandContext*)pCtx;
  ::odrxDynamicLinker()->loadModule(TfModuleName, false);
  OdDbDatabase* pDb = 0;
  if (pCmdCtx)
    pDb = OdDbCommandContextPtr(pCmdCtx)->database();
  else
    pDb = m_pDb;
  odTfInitialize(pDb->appServices());
  OdString msg, author;
  if (pCmdCtx)
    author = pCmdCtx->userIO()->getString(L"Enter author", OdEd::kGstAllowSpaces);
  if (pCmdCtx)
    msg = pCmdCtx->userIO()->getString(L"Enter commit message", OdEd::kGstAllowSpaces);
  OdTfRepository* pRep = OdTfRevisionControl::getRepository(pDb);
  OdTfDigestArray parents;
  OdTfRevisionControl::ReferenceList refList;
  pRep->getReferences(refList);
  OdString localBranch = OdTfRevisionControl::getLocalBranch(pDb);
  parents.append(refList.find(localBranch)->second);
  OdTfDigest sha = pRep->commit(pDb, parents, OdTfCommitAnnotation(author, msg));
  pRep->setReference(localBranch, sha);
}

void OdRcCommit::execute(OdEdCommandContext* pCmdCtx)
{
  executeCommit(pCmdCtx, m_pDb);
}

/** \details
Send code to stop server.
*/

void executeStop(void* pCtx, OdDbDatabase* m_pDb)
{
  OdEdCommandContext* pCmdCtx = (OdEdCommandContext*)pCtx;
  ::odrxDynamicLinker()->loadModule(TfModuleName, false);
  OdDbDatabase* pDb = 0;
  if (pCmdCtx)
    pDb = OdDbCommandContextPtr(pCmdCtx)->database();
  else
    pDb = m_pDb;
  odTfInitialize(pDb->appServices());
  void* context = 0;
  void* socket = 0;
  try
  {
    initZMQ(context, socket);
    send(socket, kOdRcStop, 0);
  }
  catch (OdException& e)
  {
    if (pCmdCtx)
      pCmdCtx->userIO()->putString(e.description());
    else
      odPrintConsoleString(L"%s\n", e.description());
  }
  catch (...)
  {
    if (pCmdCtx)
      pCmdCtx->userIO()->putString(L"unknown connection error");
    else
      odPrintConsoleString(L"unknown connection error");
  }
  zmq_close(socket);
  zmq_ctx_destroy(context);
}

void OdRcStop::execute(OdEdCommandContext* pCmdCtx)
{
  executeStop(pCmdCtx, m_pDb);
}



/** \details
Send set of commits to server (like git push)
*/

void executePush(void* pCtx, OdDbDatabase* m_pDb)
{
  OdEdCommandContext* pCmdCtx = (OdEdCommandContext*)pCtx;
  ::odrxDynamicLinker()->loadModule(TfModuleName, false);
  OdDbDatabase* pDb = 0;
  if (pCmdCtx)
    pDb = OdDbCommandContextPtr(pCmdCtx)->database();
  else
    pDb = m_pDb;
  odTfInitialize(pDb->appServices());
  OdTfRepositoryPtr pRep = OdTfRevisionControl::getRepository(pDb);
  void* context = 0;
  void* socket = 0;
  try
  {
    OdRcCodes code;
    initZMQ(context, socket);
    send(socket, kOdRcPush, ZMQ_SNDMORE);
    OdTfDigestArray shaTo, shaFrom;
    OdTfDigest serverBranchSha;
    OdTfRevisionControl::ReferenceList refList;
    pRep->getReferences(refList);
    OdString s = OdTfRevisionControl::getLocalBranch(pDb);
    shaTo.append(refList.find(s)->second);
    findAllFromSha(socket, shaFrom, pRep, shaTo.first());
    send(socket, kOdRcOk, ZMQ_SNDMORE);
    OdStreamBufPtr sBuf = OdMemoryStream::createNew();
    OdPlatformStreamer::wrString(*sBuf, s);
    sendBigData(socket, sBuf);
    receive(socket, serverBranchSha.data, sizeof(serverBranchSha.data));
    if (pRep->getCommitInfo(serverBranchSha).revision.isNull() && !serverBranchSha.isNull())
    {
      send(socket, OdRcCodes::kOdRcNeedPull);
      receive(socket, code);
      ODA_ASSERT(code == kRCDoNothink);
      throw OdError(L"Server branch ahead from local. Need pull\n");
    }
    send(socket, OdRcCodes::kOdRcOk, ZMQ_SNDMORE);

    sBuf = pRep->makePatch(shaFrom, shaTo, refList);
    sendBigData(socket, sBuf);
    receive(socket, code);
    if (code != kOdRcOk)
    {
      ODA_FAIL();
    }
  }
  catch (OdException& e)
  {
    if (pCmdCtx)
      pCmdCtx->userIO()->putString(e.description());
    else
      odPrintConsoleString(L"%s\n", e.description());
  }
  catch (...)
  {
    if (pCmdCtx)
      pCmdCtx->userIO()->putString(L"unknown connection error");
    else
      odPrintConsoleString(L"unknown connection error\n");

  }
  zmq_close(socket);
  zmq_ctx_destroy(context);
}

void OdRcPush::execute(OdEdCommandContext* pCmdCtx)
{
  executePush(pCmdCtx, m_pDb);
}




void pull(OdDbDatabase* pDb, void* socket, void* context)
{
  OdTfRepositoryPtr pRep = OdTfRevisionControl::getRepository(pDb);
  send(socket, kOdRcPull, ZMQ_SNDMORE);//TODO need check, that database has one parent
  OdTfDigest shaLocalTop;
  OdString localBranch = OdTfRevisionControl::getLocalBranch(pDb);
  pRep->getReference(localBranch, shaLocalTop);
  OdStreamBufPtr res = OdMemoryStream::createNew();
  OdPlatformStreamer::wrString(*res, localBranch);
  sendBigData(socket, res);
  res = 0;
  OdRcCodes answer;
  receive(socket, answer);
  if (answer != kOdRcOk)
  {
    odPrintConsoleString(L"Remote branch not exist\n");
    zmq_close(socket);
    zmq_ctx_destroy(context);
    return;
  }
  OdTfDigest servrTopSha;
  receive(socket, servrTopSha);
  OdTfCommitInfo commit = pRep->getCommitInfo(servrTopSha);
  if (!commit.revision.isNull())
  {
    send(socket, kOdRcNotOk, ZMQ_SNDMORE);
    odPrintConsoleString(L"No need pull\n");
    zmq_close(socket);
    zmq_ctx_destroy(context);
    return;
  }
  send(socket, kOdRcOk, ZMQ_SNDMORE);
  //check need merge after pull or not
  send(socket, shaLocalTop, 0, shaLocalTop.data, sizeof(shaLocalTop.data));
  receive(socket, answer);
  const bool bNeedMerge = answer == kOdRcNotOk;
  //search sha from for server
  OdTfDigest shaFrom;

  while (true)
  {
    receive(socket, answer);
    if (answer != kOdRcSha)
      break;
    receive(socket, shaFrom.data, sizeof(shaFrom.data));
    OdTfCommitInfo commitInfo = pRep->getCommitInfo(shaFrom);
    if (commitInfo.revision.isNull())
      send(socket, kOdRcNotOk);
    else
      send(socket, kOdRcOk);
  }
  receive(socket, answer);
  if (answer != kOdRcOk)
  {
    odPrintConsoleString(L"Server can't create a patch");
    zmq_close(socket);
    zmq_ctx_destroy(context);
    return;
  }
  OdStreamBufPtr sBuf;
  reciveBigData(socket, sBuf);
  try
  {
    OdTfDigestArray roots;
    OdTfRevisionControl::ReferenceList refList;
    pRep->applyPatch(sBuf, roots, refList);
    if (!bNeedMerge)
      pRep->setReference(localBranch, refList.find(localBranch)->second);
    else
    {
      if (OdTfRevisionControl::merge(pDb, refList.find(localBranch)->second, OdTfFixedMergeHandler<OdTfRevisionControl::kMergeMineFull>()))
      {
        OdTfDigestArray parents;
        parents.append(refList.find(localBranch)->second);
        parents.append(shaLocalTop);
        OdTfDigest shaLocalTop = pRep->commit(pDb, parents, OdTfCommitAnnotation(L"client", L"mergeCommit"));
        pRep->setReference(localBranch, shaLocalTop);
      }
    }
    OdTfRevisionControl::switchBranch(pDb, localBranch);
  }
  catch (const OdError err)
  {
    send(socket, kOdRcNotOk);
    throw err;
  }
  catch (...)
  {
    send(socket, kOdRcNotOk);
    throw;
  }
}

namespace ExTfClone{
  void clone(OdEdCommandContext* pCmdCtx, void* socket, void* context, OdDbDatabase* pDb)
  {
    //*  make clone via patch(do not receive all .dsf file)
    if (pCmdCtx)
      pCmdCtx->userIO()->putString(L"Not found repository. Executed clone\n");
    else
      odPrintConsoleString(L"Not found repository. Executed clone\n");
    OdRcCodes code = kOdRcClone;
    send(socket, code, 0);
    receive(socket, code);
    if (code != kOdRcOk)
    {
      odPrintConsoleString(L"Server can't create a patch on clone");
      zmq_close(socket);
      zmq_ctx_destroy(context);
      return;
    }
    OdStreamBufPtr sBuf;
    reciveBigData(socket, sBuf);
    //remove("D:/Bugs/NewExampleRC/repC.dsf");
    //OdString s = L"D:/Bugs/NewExampleRC/repC.dsf";
    //if(odrxSystemServices()->accessFile("D:/Bugs/NewExampleRC/repC.dsf", 0))
      //s = L"D:/Bugs/NewExampleRC/repC2.dsf";
    //OdTfRepositoryPtr _rep = OdTfRepository::createObject(odrxSystemServices()->createFile(s, Oda::FileAccessMode(Oda::kFileWrite | Oda::kFileRead), Oda::kShareDenyWrite, Oda::kCreateAlways), pDb->appServices());
    OdStreamBufPtr sFile = OdMemoryStream::createNew();
    OdTfRepositoryPtr _rep = OdTfRepository::createObject(sFile, pDb->appServices());
    _rep->commit(pDb, OdTfDigestArray());
    OdTfDigestArray roots;
    OdTfRevisionControl::ReferenceList refList;
    try
    {
      _rep->applyPatch(sBuf, roots, refList);
      send(socket, kOdRcOk, 0);
      OdStreamBufPtr sBuf;
      reciveBigData(socket, sBuf);
      sBuf->rewind();
      while (!sBuf->isEof())
      {
        OdTfCommitInfo commitInfo = deserializationCommit(sBuf);
        ODA_ASSERT(!commitInfo.hash.isNull());
        _rep->writeCommitInfo(commitInfo);
      }
      OdTfRevisionControl::ReferenceList::const_iterator it = refList.begin();
      while (it != refList.end())
      {
        _rep->setReference(it->first, it->second);
        ++it;
      }
      OdTfRevisionControl::switchBranch(pDb, L"master");
    }
    catch (const OdError& err)
    {
      if (pCmdCtx)
        pCmdCtx->userIO()->putString(err.description());
      else
        odPrintConsoleString(L"%s\n", err.description());
    }
  }
}


void executePull(void * pCtx, OdDbDatabase* m_pDb)
{
  OdEdCommandContext* pCmdCtx = (OdEdCommandContext*)pCtx;
  ::odrxDynamicLinker()->loadModule(TfModuleName, false);
  OdDbDatabase* pDb = 0;
  if (pCmdCtx)
    pDb = OdDbCommandContextPtr(pCmdCtx)->database();
  else
    pDb = m_pDb;
  odTfInitialize(pDb->appServices());
  void* context = 0;
  void* socket = 0;
  try
  {
    context = zmq_ctx_new();
    initZMQ(context, socket);

    if (!OdTfRevisionControl::getLocalBranch(pDb).isEmpty())//if found name - do pull, else do clone
      pull(pDb, socket, context);
    else//clone
      ExTfClone::clone(pCmdCtx, socket, context, pDb);
  }
  catch (OdException& e)
  {
    if (pCmdCtx)
      pCmdCtx->userIO()->putString(e.description());
    else
      odPrintConsoleString(L"%s\n", e.description());
  }
  catch (...)
  {
    if (pCmdCtx)
      pCmdCtx->userIO()->putString(L"unknown connection error");
    else
      odPrintConsoleString(L"unknown connection error");
  }
  zmq_close(socket);
  zmq_ctx_destroy(context);
}

void OdRcPull::execute(OdEdCommandContext* pCmdCtx)
{
  executePull(pCmdCtx, m_pDb);
}



void OdRcModule::initApp()
{
  OdEdCommandStackPtr pCommands = odedRegCmds();
  pCommands->addCommand(&m_RCCommit);
  pCommands->addCommand(&m_RCPush);
  pCommands->addCommand(&m_RCUpdate);
  pCommands->addCommand(&m_RCStop);
  m_RCCommit._module = this;
  m_RCUpdate._module = this;
  m_RCStop._module = this;
  m_RCPush._module = this;
}

void OdRcModule::uninitApp()
{
  OdEdCommandStackPtr pCommands = odedRegCmds();
  pCommands->removeGroup(L"Revision control git");
}


