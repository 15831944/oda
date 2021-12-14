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

#ifndef _DB_FDUMPER_H
#define _DB_FDUMPER_H

#include "OdaCommon.h"
#include "DbDumper.h"

#include "DbDatabase.h"
#include "ExPrintConsole.h"
#include "Tf/DbObjectSchemaIterator.h"
#include "diagnostics.h"
#include "OdHashSet.h"
#define STL_USING_QUEUE
#include "OdaSTL.h"
#include "OdHashMap.h"

class OdDbStub;


class DbFDumper : public OdRxObjectImpl<OdIdFiler>, public DbDumper {
  OdDbDatabase* m_pDb;
  int indent;
  int numDumpedObjects;
  int nEmbeddedSubobjectFollowState;
  bool hasSchema;
  OdString subclass;
  OdString subobject;
  OdString subobjectName;
  OdDbObjectSchemaIteratorPtr m_pSchemaIterator;
  OdAssertFunc m_savedAssertFunc;
  static OdAssertFunc g_savedAssertFunc;
  static void myAssert(const char* expr, const char* fileName, int nLine);
  void substituteAssertFunc();
  void restoreAssertFunc();
  typedef OdHashSet<OdDbStub*, OdHashSet_PtrHasher<OdDbStub> > OdDbStubPtrHashSet;
  typedef OdHashMap<OdUInt64, OdUInt64> OdUInt64HashMap;
  OdDbStubPtrHashSet dumped;
  OdDbStubPtrHashSet blist;
  OdUInt64HashMap remaphash;
  std::queue<OdDbStub*> refQueue;
  bool dumpRefs;
  bool remap;
  OdDbHandle remapHandle(OdDbHandle id);
  void tokenize(OdDbObjectIdArray& outarr, const OdString& input, const OdChar& delim, bool remap);
public:
  DbFDumper();
  void listDwgFields(const OdDbObject* obj);

  virtual FilerType filerType() const { return OdDbFiler::kUndoFiler; }
  virtual bool isPersistentMode() const { return true; }
  OdDbDatabase* database() const { return m_pDb; }

  // Data output functions
  virtual void seek(OdInt64 /*offset*/, OdDb::FilerSeekType /*whence*/) {}
  virtual OdUInt64 tell() const;

  bool seekObjectSchema(const OdDbObject* pObj);
  OdString seekFieldGetName();
  OdString seekFieldGetName(const OdString& sValue);
  OdString seekFieldGetName(const OdInt64& data);
  OdString seekFieldGetName(const double& value);
  OdString seekFieldGetName(const double& valueX, const double& valueY);
  OdString seekFieldGetName(const double& valueX, const double& valueY, const double& valueZ);
  OdString seekFieldGetName_onBinaryChunk();
  OdString objectIdAndClassName(const OdDbObject* pObj);
  OdString objectIdAndClassName(OdDbObjectId id);

  //////////////////////////////////////////////
  // reading
  virtual bool         rdBool() {return 0;}
  virtual OdString     rdString() { return OdString::kEmpty; }
  virtual void         rdBytes(void* /*buffer*/, OdUInt32 /*nLen*/) {}

  virtual OdInt8       rdInt8() {return 0;}
  virtual OdUInt8      rdUInt8() {return 0;}

  virtual OdInt16      rdInt16() {return 0;}
  virtual OdInt32      rdInt32() {return 0;}
  virtual OdInt64      rdInt64() {return 0;}
  virtual double       rdDouble() {return 0;}
  virtual OdDbHandle   rdDbHandle() {return 0;}

  virtual OdDbObjectId rdSoftOwnershipId() { return OdDbObjectId::kNull; }
  virtual OdDbObjectId rdHardOwnershipId() { return OdDbObjectId::kNull; }
  virtual OdDbObjectId rdHardPointerId() { return OdDbObjectId::kNull; }
  virtual OdDbObjectId rdSoftPointerId() { return OdDbObjectId::kNull; }

  virtual OdGePoint2d rdPoint2d() { return OdGePoint2d::kOrigin; }
  virtual OdGePoint3d rdPoint3d() { return OdGePoint3d::kOrigin; }
  virtual OdGeVector3d rdVector3d() { return OdGeVector3d::kIdentity; }
  virtual OdGeVector2d rdVector2d() { return OdGeVector2d::kIdentity; }
  virtual OdGeScale3d rdScale3d() { return OdGeScale3d::kIdentity; }

  //////////////////////////////////////////////
  // writing
  virtual void wrBool(bool) ;
  virtual void wrString(const OdString &string) ;
  virtual void wrBytes(const void* buffer, OdUInt32 nLen) ;

  virtual void wrInt8(OdInt8 val) ;
  virtual void wrUInt8(OdUInt8 val) ;

  virtual void wrInt16(OdInt16 val) ;
  virtual void wrInt32(OdInt32 val) ;
  virtual void wrInt64(OdInt64 val) ;
  virtual void wrDouble(double val) ;
  virtual void wrDbHandle(const OdDbHandle& val) ;

  virtual void wrSoftOwnershipId(const OdDbObjectId& id) ;
  virtual void wrHardOwnershipId(const OdDbObjectId& id) ;
  virtual void wrSoftPointerId(const OdDbObjectId& id) ;
  virtual void wrHardPointerId(const OdDbObjectId& id) ;

  virtual void wrPoint2d(const OdGePoint2d& pt) ;
  virtual void wrPoint3d(const OdGePoint3d& pt) ;
  virtual void wrVector2d(const OdGeVector2d& vec) ;
  virtual void wrVector3d(const OdGeVector3d& vec) ;
  virtual void wrScale3d(const OdGeScale3d& point) ;
  virtual void wrAddress(const void* val);

  virtual void wrThickness(double value);
  virtual void wrExtrusion(const OdGeVector3d& value);

  virtual void fdump(OdDbDatabase* pDb, const OdString& sset, const OdString& blist, bool dumpRefs, bool remap);

  void dumpReferences();

  virtual void fdumpObject(const OdDbObjectId& id, int indent = 0);
private:
  void insertSubclassMarkers(OdString& curFieldName);
};

#endif
