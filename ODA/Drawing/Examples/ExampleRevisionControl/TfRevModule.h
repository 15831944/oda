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

#ifndef _TfRevModule_h_Included_
#define _TfRevModule_h_Included_


#include "OdaCommon.h"
#include "Ed/EdCommandStack.h"
#include "RxModule.h"
#include "StaticRxObject.h"
#include "DbDatabase.h"
#define STL_USING_MAP
#define STL_USING_VECTOR
#include "OdaSTL.h"
#include "Tf/TfRevisionControl.h"

#include "RxDynamicModule.h"
#include "DbCommandContext.h"
#include "DbHistoryManager.h"
#include "FlatMemStream.h"
//#include "TfDwgFiler.h"
//#include "../../Drawing/Source/Tf/TfModule.h"
#include "StaticRxObject.h"
#include "ExSystemServices.h"
#include "OdFileBuf.h"
#include "ExHostAppServices.h"

#ifdef OD_HAVE_CONSOLE_H_FILE
#include <console.h>
#endif
#include "DebugStuff.h"
#include "MemoryStream.h"
#include "../ThirdParty/zmq/include/zmq.h"
#define SHA_SIZE 20

#define SERVER_CONF "tcp://192.168.2.56:4040"


OD_DLL_EXPORT void executePush(void* pCmdCtx, OdDbDatabase* m_pDb);
OD_DLL_EXPORT void executePull(void* pCmdCtx, OdDbDatabase* m_pDb);
OD_DLL_EXPORT void executeCommit(void* pCmdCtx, OdDbDatabase* m_pDb);
OD_DLL_EXPORT void executeStop(void* pCmdCtx, OdDbDatabase* m_pDb);

class OdRcModule;

enum OdRcCodes
{
  kOdRcSha = 1,
  kOdRcPull,
  kOdRcPush,
  kOdRcClone,
  kOdRcStop,
  kOdRcOk,
  kOdRcNotOk,
  kRCDoNothink,
  kOdRcNeedPull,
  kOdRcReset,
};

struct OdRcCommit : OdEdCommand
{
  const OdString groupName() const { return L"Revision control git"; }
  const OdString globalName() const { return L"Commit"; }
  void execute(OdEdCommandContext* pCmdCtx);
  virtual OdInt32 flags() const
  {
    return OdEdCommand::flags() | OdEdCommand::kNoUndoMarker;
  }
  OdRcModule* _module;
  OdDbDatabase* m_pDb;
};

struct OdRcPush : OdEdCommand
{
  const OdString groupName() const { return L"Revision control git"; }
  const OdString globalName() const { return L"Push"; }
  void execute(OdEdCommandContext* pCmdCtx);

  virtual OdInt32 flags() const
  {
    return OdEdCommand::flags() | OdEdCommand::kNoUndoMarker;
  }
  OdRcModule* _module;
  OdDbDatabase* m_pDb;
};

struct OdRcPull : OdEdCommand
{
  const OdString groupName() const { return L"Revision control git"; }
  const OdString globalName() const { return L"Pull"; }
  void execute(OdEdCommandContext* pCmdCtx);
  virtual OdInt32 flags() const
  {
    return OdEdCommand::flags() | OdEdCommand::kNoUndoMarker;
  }
  OdRcModule* _module;
  OdDbDatabase* m_pDb;
};

struct OdRcStop : OdEdCommand
{
  const OdString groupName() const { return L"Revision control git"; }
  const OdString globalName() const { return L"Stop_server"; }
  void execute(OdEdCommandContext* pCmdCtx);
  virtual OdInt32 flags() const
  {
    return OdEdCommand::flags() | OdEdCommand::kNoUndoMarker;
  }
  OdRcModule* _module;
  OdDbDatabase* m_pDb;
};

void initZMQ(void* &context, void* &socket)
{
  context = zmq_ctx_new();
  if (!context)
    throw OdException(L"Can't get context");
  socket = zmq_socket(context, ZMQ_REQ);
  if (!socket)
    throw OdException(L"Can't get socket");
  int conn = zmq_connect(socket, SERVER_CONF);
  if (conn != 0)
    throw OdException(OdString(L"Can't connect ") + SERVER_CONF);
}

template <typename T>
void send(void* const socket, const T data, const OdInt8 &flag = 0, char *buf = 0, OdUInt16 countBytes = 0)
{
  countBytes = countBytes ? countBytes : sizeof(T);
  zmq_msg_t message;
  OdInt8 res = zmq_msg_init_size(&message, countBytes);
  ODA_ASSERT(res == 0);
  if(buf)
    memcpy(zmq_msg_data(&message), buf, countBytes);
  else
    memcpy(zmq_msg_data(&message), &data, countBytes);
  OdInt32 countSendBytes = zmq_msg_send(&message, socket, flag);
  ODA_ASSERT(countSendBytes == countBytes);
  res = zmq_msg_close(&message);
  ODA_ASSERT(res == 0);
}

template <typename T>
bool receive(void* const socket, T& data, const OdUInt16 countBytes = 0, const OdInt8 &flag = 0)
{
  zmq_msg_t reply;
  OdInt8 res = zmq_msg_init(&reply);
  ODA_ASSERT(res == 0);
  int len = zmq_msg_recv(&reply, socket, flag);
  ODA_ASSERT(len != -1);
  size_t length = zmq_msg_size(&reply);
  ODA_ASSERT(len == length);
  if (length != len)
  {
    odPrintConsoleString(L"Wrong command\n");
    zmq_msg_close(&reply);
    return false;
  }
  memcpy(&data, zmq_msg_data(&reply), length);
  res = zmq_msg_close(&reply);
  ODA_ASSERT(res == 0);
  return true;
}

void sendBigData(void* const socket, OdStreamBufPtr &buf)
{
  OdUInt64 N = buf->length();
  buf->rewind();
  send(socket, N, ZMQ_SNDMORE);
  for (OdUInt64 n = 0; ;)
  {
    char subBuf[128];
    if (n + 128 < N)
    {
      buf->getBytes(subBuf, 128);
      send(socket, 0, ZMQ_SNDMORE, subBuf, 128);
      n += 128;
    }
    else
    {
      buf->getBytes(subBuf, N - n);
      send(socket, 0, 0, subBuf, N - n);
      break;
    }
  }
}

bool reciveBigData(void* socket, OdStreamBufPtr& sBuf)
{
  OdUInt64 N = 0;
  receive(socket, N);
  sBuf = OdFlatMemStreamManaged::createNew(N);
  size_t length;
  zmq_msg_t reply;
  for (OdUInt32 n = 0; N - n;)
  {
    zmq_msg_init(&reply);
    zmq_msg_recv(&reply, socket, 0);
    length = zmq_msg_size(&reply);
    if (length < 1)
    {
      odPrintConsoleString(L"Something wrong on receive bigData\n");
      ODA_ASSERT(length);
      zmq_msg_close(&reply);
      return false;
    }
    sBuf->putBytes(zmq_msg_data(&reply), (OdUInt32)length);
    n += (OdUInt32)length;
  }
  zmq_msg_close(&reply);
  sBuf->rewind();
  return true;
}

void addCommitRecursive(OdTfRepository* pRep, std::set<OdTfDigest>& baseCommits, std::set<OdTfDigest>& newCommits, const OdTfDigest& to)
{
  if (baseCommits.find(to) != baseCommits.end())
    return;

  OdTfCommitInfo c;
  c = pRep->getCommitInfo(to);
  c.hash = to;
  newCommits.insert(c.hash);

  for (unsigned j = 0; j < c.parents.size(); ++j)
    addCommitRecursive(pRep, baseCommits, newCommits, c.parents[j]);
}

void addRevisionRecursive(OdTfRepository* pRep, std::set<OdTfDigest>& baseCommits, std::set<OdTfDigest>& newRevisions, const OdTfDigest& to)
{
  if (baseCommits.find(to) != baseCommits.end())
    return;

  OdTfCommitInfo c;
  c = pRep->getCommitInfo(to);
  c.hash = to;
  newRevisions.insert(c.revision);

  for (unsigned j = 0; j < c.parents.size(); ++j)
    addRevisionRecursive(pRep, baseCommits, newRevisions, c.parents[j]);
}

//send sha to other side, if commit with this sha has other side, other side return code kOdRcOk(we this sha write to array 'from'
//if return not kOdRcOk, need recursive going by parents while will not find general ancestor that will be added to array 'from'
void findAllFromSha(void* socket, OdTfDigestArray& from, OdTfRepositoryPtr& rep, OdTfDigest sha)
{
  send(socket, kOdRcSha, ZMQ_SNDMORE);
  send(socket, sha, 0, sha.data, sizeof(sha.data));
  OdRcCodes code;
  receive(socket, code);
  if (code == kOdRcOk)
  {
    from.append(sha);
    return;
  }
  OdTfCommitInfo commitInfo = rep->getCommitInfo(sha);
  for (unsigned i = 0; i < commitInfo.parents.size(); ++i)
  {
    findAllFromSha(socket, from, rep, commitInfo.parents[i]);
  }
}
//OdTfRepository write OdTfCommitInfo objects to patch, but OdTfStorage not include it.
//serializeCommit and deserializationCommit used in clone process
void serializeCommit(OdTfCommitInfo& c, OdStreamBufPtr s)
{
  c.hash.write(s);
  c.revision.write(s);
  OdPlatformStreamer::wrInt32(*s, c.parents.size());
  for (unsigned i = 0; i < c.parents.size(); ++i)
    c.parents[i].write(s);
  OdPlatformStreamer::wrString(*s, c.annotation.author);
  OdPlatformStreamer::wrInt64(*s, c.annotation.date.packedValue());
  OdPlatformStreamer::wrString(*s, c.annotation.message);
  if (!c.annotation.userData.isNull())
  {
    OdPlatformStreamer::wrInt32(*s, (OdInt32)c.annotation.userData->length());
    c.annotation.userData->copyDataTo(s);
  }
  else
    OdPlatformStreamer::wrInt32(*s, 0);
}

//OdTfRepository write OdTfCommitInfo objects to patch, but OdTfStorage not include it.
//serializeCommit and deserializationCommit used in clone process
OdTfCommitInfo deserializationCommit(OdStreamBufPtr & s)
{
  if (s.isNull())
    return OdTfCommitInfo();
  OdTfCommitInfo c;
  c.hash.read(s);
  c.revision.read(s);
  OdUInt32 N = OdPlatformStreamer::rdInt32(*s);
  if (N > 1000)
    throw OdError("Invalid commit");
  c.parents.resize(N);
  for (unsigned i = 0; i < N; ++i)
    c.parents[i].read(s);
  c.annotation.author = OdPlatformStreamer::rdString(*s);
  c.annotation.date.setPackedValue(OdPlatformStreamer::rdInt64(*s));
  c.annotation.message = OdPlatformStreamer::rdString(*s);
  N = OdPlatformStreamer::rdInt32(*s);
  if (N > 0)
  {
    c.annotation.userData = OdFlatMemStreamManaged::createNew(N);
    s->copyDataTo(c.annotation.userData, s->tell(), s->length());
  }
  return c;
}

struct GitLikeServer
{
  OdString _path;
  OdDbDatabasePtr _db;
  OdTfRepositoryPtr _rep;
  OdDbHostAppServices* _appServices;
  OdTfDigest _rootShaRevision;
  OdTfDigest _rootShaCommit;
  void handler()
  {
    //internalRemove(L"repository_s.dsf");
    ::odrxDynamicLinker()->loadModule(TfModuleName, false);
    odTfInitialize(_appServices);
    if (_path.isEmpty() || _path.right(4).iCompare(L".dwg") == 0)
    {//create new sqlite base
      _db = _path.isEmpty() ? _appServices->createDatabase(true) : _db = _appServices->readFile(_path);
      //_rep = OdTfRepository::createObject(odrxSystemServices()->createFile(L"repository_s.dsf", Oda::FileAccessMode(Oda::kFileWrite | Oda::kFileRead), Oda::kShareDenyWrite, Oda::kCreateAlways), _appServices);
      OdStreamBufPtr sFile = OdMemoryStream::createNew();
      _rep = OdTfRepository::createObject(sFile, _db->appServices());
      _rootShaCommit = _rep->commit(_db, OdTfDigestArray());
      OdTfCommitInfo commitInfo = _rep->getCommitInfo(_rootShaCommit);
      _rootShaRevision = commitInfo.revision;
      _rep->setReference(L"master", commitInfo.hash);
    }
    else if(_path.right(4).iCompare(L".dsf") == 0)
    {//open created
      OdStreamBufPtr s = odrxSystemServices()->createFile(_path, Oda::FileAccessMode(Oda::kFileWrite | Oda::kFileRead), Oda::kShareDenyWrite, Oda::kOpenAlways);
      _rep = OdTfRepository::createObject(s, _appServices);
      _db = _rep->checkout(L"master");
      _rep->getReference(L"master", _rootShaCommit);
      OdTfCommitInfo commit =_rep->getCommitInfo(_rootShaCommit);
      while (!commit.parents.isEmpty())
        commit = _rep->getCommitInfo(commit.parents[0]);
      _rootShaRevision = commit.revision;
      _rootShaCommit = commit.hash;
    }
    else
    {
      throw OdError(L"Can't create/open repository");
    }
    
    void* context = zmq_ctx_new();
    if (!context)
      throw OdException(L"Can't get context");
    void* socket = zmq_socket(context, ZMQ_REP);
    if (!socket)
      throw OdException(L"Can't get socket");
    OdInt8 conn = zmq_bind(socket, SERVER_CONF);
    if (conn != 0)
      throw OdException(OdString(L"Can't bind to ") + SERVER_CONF);
    bool bStop = false;
    odPrintConsoleString(L"Start server\n");
    while (true)
    {
      if (bStop)
        break;
      
      OdRcCodes code;
      if(!receive(socket, code))
        continue;

      switch (code)
      {
      case kOdRcStop:
      {
        bStop = true;
        break;
      }
      case kOdRcSha:
      {
        ODA_FAIL();
        break;
      }
      case kOdRcPush:
      {
        push(socket, code, _rep);

        break;
      }
      case kOdRcPull:
      {
        doPull(socket);
        break;
      }
      case kOdRcClone:
      {
        //*  first attemp to make clone via patch(do not recive all .dsf file)
        odPrintConsoleString(L"Start clone\n");
        OdTfRevisionControl::ReferenceList refList;
        _rep->getReferences(refList);
        OdTfDigestArray shaTo, shaFrom;
        OdTfRevisionControl::ReferenceList::const_iterator it = refList.begin();
        while (it != refList.end())
        {//clone all branches, get heads all branches
          shaTo.push_back(it->second);
          ++it;
        }
        std::set<OdTfDigest> baseCommits, newRevisions, newCommits;
        baseCommits.insert(_rootShaCommit);
        newCommits.insert(_rootShaCommit);
        newRevisions.insert(_rootShaRevision);
        for (unsigned i = 0; i < shaTo.size(); i++)
          addRevisionRecursive(_rep, baseCommits, newRevisions, shaTo[i]);//get all revisions from root to all branches heads
        for (unsigned i = 0; i < shaTo.size(); i++)
          addCommitRecursive(_rep, baseCommits, newCommits, shaTo[i]);//get all commits from root to all branches heads
        OdStreamBufPtr pCommitInfoBuf = OdMemoryStream::createNew();
        std::set<OdTfDigest>::const_iterator itCommits = newCommits.begin();
        while (itCommits != newCommits.end())
        {
          OdTfCommitInfo commitInfo = _rep->getCommitInfo(*itCommits);
          serializeCommit(commitInfo, pCommitInfoBuf);//write all commite. Because makePatch write only revisions
          ++itCommits;
        }
        shaTo.clear();
        for (std::set<OdTfDigest>::iterator it = newRevisions.begin(); it != newRevisions.end(); ++it)
          shaTo.append(*it);
        OdStreamBufPtr sBuf;
        try
        {
          sBuf = ((OdTfStorage*)_rep)->makePatch(shaFrom, shaTo, refList);
          OdRcCodes code = kOdRcOk;
          send(socket, code, ZMQ_SNDMORE);
          sendBigData(socket, sBuf);
          OdRcCodes answer;
          receive(socket, answer);
          if(answer == kOdRcOk)
            sendBigData(socket, pCommitInfoBuf);
        }
        catch (const OdError& exp)
        {
          OdRcCodes code = kOdRcNotOk;
          send(socket, code);
          odPrintConsoleString(L"%s\n", exp.description());
        }
        odPrintConsoleString(L"End clone\n\n");
        break;
        //*/
      }
      }
    }
    zmq_close(socket);
    zmq_ctx_destroy(context);
    //OdTfRevisionControl::switchBranch(_db, L"master");
    //_db->writeFile(L"D:/Bugs/NewExampleRC/6.dwg", OdDb::kDwg, OdDb::kDHL_CURRENT);
  };

  void showLogRecursive(OdTfCommitInfo commitInfo, const OdString& s)
  {
    OdString sHash;
    sHash.format(L"%c%c%c%c%c%c", commitInfo.hash.data[0], commitInfo.hash.data[1], commitInfo.hash.data[2], commitInfo.hash.data[3], commitInfo.hash.data[4], commitInfo.hash.data[5]);
    odPrintConsoleString(L"Name: %10ls P: %d Hash: %s\n", s.c_str(), commitInfo.parents.size(), sHash.c_str());
    for (unsigned i = 0; i < commitInfo.parents.size(); ++i)
    {
      if (commitInfo.parents.size() > 1)
        odPrintConsoleString(L"startParent %d\n", i);
      OdTfCommitInfo commitInfo2 = _rep->getCommitInfo(commitInfo.parents[i]);
      showLogRecursive(commitInfo2, s);
      if (commitInfo.parents.size() > 1)
        odPrintConsoleString(L"startParent %d\n", i);
    }
  }

  void showLog(OdTfRevisionControl::ReferenceList refs)
  {
    OdTfRevisionControl::ReferenceList::iterator it = refs.begin();
    OdTfCommitInfo commitInfo;
    while (it != refs.end())
    {
      commitInfo = _rep->getCommitInfo(it->second);
      showLogRecursive(commitInfo, it->first);
      ++it;
    }
  };

  void doPull(void* socket)
  {
    odPrintConsoleString(L"Start pull\n");

    OdStreamBufPtr sBuf;
    reciveBigData(socket, sBuf);
    OdString branch = OdPlatformStreamer::rdString(*sBuf);
    OdTfRevisionControl::ReferenceList referenceList;
    _rep->getReferences(referenceList);
    bool bFind = referenceList.find(branch) != referenceList.end();
    if (!bFind)
    {
      send(socket, kOdRcNotOk);
      return;
    }
    send(socket, kOdRcOk, ZMQ_SNDMORE);
    send(socket, referenceList.find(branch)->second.data, 0, referenceList.find(branch)->second.data, sizeof(referenceList.find(branch)->second.data));
    OdRcCodes answer;
    receive(socket, answer);
    if (answer == kOdRcNotOk)//no need pull
      return;
    //check need merge after pull or not
    OdTfDigest clientTopBranchSha;
    receive(socket, clientTopBranchSha.data, sizeof(clientTopBranchSha.data));
    if (_rep->getCommitInfo(clientTopBranchSha).revision.isNull())
      send(socket, OdRcCodes::kOdRcNotOk, ZMQ_SNDMORE);//client must be make merge after pull
    else
      send(socket, OdRcCodes::kOdRcOk, ZMQ_SNDMORE);

    OdTfDigestArray shaFrom, shaTo;
    findAllFromSha(socket, shaFrom, _rep, referenceList.find(branch)->second);
    send(socket, OdRcCodes::kOdRcOk, ZMQ_SNDMORE);
    shaTo.push_back(referenceList.find(branch)->second);

    OdTfRevisionControl::ReferenceList refs;
    _rep->getReferences(refs);
    try
    {
      sBuf = _rep->makePatch(shaFrom, shaTo, refs);
      send(socket, kOdRcOk, ZMQ_SNDMORE);
      sendBigData(socket, sBuf);
    }
    catch (const OdError& err)
    {
      odPrintConsoleString(L"%s\n", err.description());
      send(socket, kOdRcNotOk);
      odPrintConsoleString(L"End pull\n\n");
      return;
    }
    odPrintConsoleString(L"End pull\n\n");
  }

  private:

    void push(void* socket, OdRcCodes code, OdTfRepositoryPtr _rep)
    {
      odPrintConsoleString(L"Start push\n");
      OdTfDigest shaFrom, branchSha;
      while (true)
      {
        receive(socket, code);
        if (code != kOdRcSha)
          break;
        receive(socket, shaFrom.data, sizeof(shaFrom.data));
        OdTfCommitInfo commitInfo = _rep->getCommitInfo(shaFrom);
        if (commitInfo.revision.isNull())
          send(socket, kOdRcNotOk);
        else
          send(socket, kOdRcOk);
      }
      OdStreamBufPtr sBuf;
      reciveBigData(socket, sBuf);
      if (_rep->getReference(OdPlatformStreamer::rdString(*sBuf), branchSha))
      {
        send(socket, branchSha.data, 0, branchSha.data, sizeof(branchSha.data));
        receive(socket, code);
        if (code == kOdRcNeedPull)
        {
          send(socket, kRCDoNothink);
          odPrintConsoleString(L"End push\n");
          return;
        }
      }
      else
      {
        send(socket, OdTfDigest::kNull().data, 0, OdTfDigest::kNull().data, sizeof(OdTfDigest::kNull().data));
        receive(socket, code);
        ODA_ASSERT(code == kOdRcOk);
      }
      reciveBigData(socket, sBuf);//getPatch
      OdTfDigestArray roots;
      OdTfRevisionControl::ReferenceList refs;
      try
      {
        _rep->applyPatch(sBuf, roots, refs);
        OdTfRevisionControl::ReferenceList::iterator it = refs.begin();
        while (it != refs.end())
        {
          OdTfCommitInfo commitInfo = _rep->getCommitInfo(it->second);
          if (!commitInfo.revision.isNull())
            _rep->setReference(it->first, it->second);
          ++it;
        }
        send(socket, kOdRcOk);
      }
      catch (const OdError& err)
      {
        odPrintConsoleString(L"%s\n", err.description());
        send(socket, kOdRcNotOk);
      }
      refs.clear();
      _rep->getReferences(refs);
      showLog(refs);
      odPrintConsoleString(L"End push\n\n");
    }
};

namespace Git {
  static void startServer(const OdString& path, OdDbHostAppServices* appServ)
  {
    GitLikeServer serv;
    serv._path = path;
    serv._appServices = appServ;
    serv.handler();
  };
}



class OdRcModule : public OdRxModule
{
  OdStaticRxObject<OdRcPush> m_RCPush;
  OdStaticRxObject<OdRcPull> m_RCUpdate;
  OdStaticRxObject<OdRcStop> m_RCStop; 
  OdStaticRxObject<OdRcCommit> m_RCCommit;
public:
  void initApp();
  void uninitApp();
};

#endif //_TfRevModule_h_Included_
