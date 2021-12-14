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

#ifndef _ODA_MFC_APP_LIST_FILLER_
#define _ODA_MFC_APP_LIST_FILLER_

#include "DbFiler.h"
#include "RxObjectImpl.h"
#include "Ge/GeScale3d.h"
#include "OdString.h"
#include "DbObjectId.h"
#include "Ge/GePoint2d.h"
#include "Ge/GePoint3d.h"
#include "Tf/DbObjectSchemaIterator.h"
#include "diagnostics.h"

class ListFiller : public OdRxObjectImpl<OdDbDxfFiler>
{
  CListCtrl* m_pObjList;
  OdDbDatabase* m_pDb;
  int m_nCol;

public:

  ListFiller(OdDbDatabase* pDb, CListCtrl* pObjList);

  FilerType filerType() const {return OdDbFiler::kBagFiler;}
  OdDbDatabase* database() const { return m_pDb; }
  virtual bool includesDefaultValues() const { return true; }
  
  OdDb::DwgVersion dwgVersion(OdDb::MaintReleaseVer* = NULL) const;

  // Data output functions
  void wrString(int groupCode, const OdString& string);
  void wrName(int groupCode, const OdString& string);
  void wrBool(int groupCode, bool val);
  void wrInt8(int groupCode, OdInt8 val);
  void wrUInt8(int groupCode, OdUInt8 val);
  void wrInt16(int groupCode, OdInt16 val);
  void wrUInt16(int groupCode, OdUInt16 val);
  void wrInt32(int groupCode, OdInt32 val);
  void wrUInt32(int groupCode, OdUInt32 val);
  void wrInt64(int groupCode, OdInt64 val);
  void wrUInt64(int groupCode, OdUInt64 val);
  void wrHandle(int groupCode, OdDbHandle val);
  void wrObjectId(int groupCode, OdDbObjectId val);
  void wrAngle(int groupCode, double val, int precision = kDfltPrec);
  void wrDouble(int groupCode, double val, int precision = kDfltPrec);
  void wrPoint2d(int groupCode, const OdGePoint2d& pt, int precision = kDfltPrec);
  void wrPoint3d(int groupCode, const OdGePoint3d& pt, int precision = kDfltPrec);
  void wrVector2d(int groupCode, const OdGeVector2d& pt, int precision = kDfltPrec);
  void wrVector3d(int groupCode, const OdGeVector3d& pt, int precision = kDfltPrec);
  void wrScale3d(int groupCode, const OdGeScale3d& pt, int precision = kDfltPrec);
  void wrBinaryChunk(int groupCode, const OdUInt8* pBuff, OdUInt32 nSize);

  void        rdString(OdString &string);
  bool        rdBool();
  OdInt8      rdInt8();
  OdInt16     rdInt16();
  OdInt32     rdInt32();
  OdInt64     rdInt64();
  OdUInt8     rdUInt8();
  OdUInt16    rdUInt16();
  OdUInt32    rdUInt32();
  OdUInt64    rdUInt64();
  OdDbHandle  rdHandle();
  OdDbObjectId rdObjectId();
  double      rdAngle();
  double      rdDouble();
  void        rdPoint2d(OdGePoint2d& pt);
  void        rdPoint3d(OdGePoint3d& pt);
  void        rdVector2d(OdGeVector2d& pt);
  void        rdVector3d(OdGeVector3d& pt);
  void        rdScale3d(OdGeScale3d& pt);
  void        rdBinaryChunk(OdBinaryData&);

  // Need to implement to do not force OdaMfcApp to "fly away"
  int  precision() const;
};

class ListFillerDwg : public OdRxObjectImpl<OdDbDwgFiler>
{
  CListCtrl* m_pObjList;
  OdDbDatabase* m_pDb;
  int m_nCol;
  int m_nEmbeddedSubobjectFollowState;
  bool m_bHasSchema;
  OdString subclass;
  OdString subobject;
  OdString subobjectName;
  OdDbObjectSchemaIteratorPtr m_pSchemaIterator;
  OdAssertFunc m_savedAssertFunc;
  static OdAssertFunc g_savedAssertFunc;
  static void myAssert(const char* expr, const char* fileName, int nLine);
  void substituteAssertFunc();
  void restoreAssertFunc();
public:
  ListFillerDwg(OdDbDatabase* pDb, CListCtrl* pObjList);
  void listDwgFields(const OdDbObject* obj, bool odaSchema);

  virtual FilerType filerType() const { return OdDbFiler::kUndoFiler; }
  virtual bool isPersistentMode() const { return true; }
  OdDbDatabase* database() const { return m_pDb; }

  // Data output functions
  virtual void seek(OdInt64 /*offset*/, OdDb::FilerSeekType /*whence*/) {}
  virtual OdUInt64 tell() const;

  OdTf::TypesEnum currentType();

  void skipEmptyType();

  bool seekObjectSchema(const OdDbObject* pObj);
  OdString seekFieldGetName();
  OdString seekFieldGetName(const OdString& sValue);
  OdString seekFieldGetName(const OdInt64& data);
  OdString seekFieldGetName(const double& value);
  OdString seekFieldGetName(const double& valueX, const double& valueY);
  OdString seekFieldGetName(const double& valueX, const double& valueY, const double& valueZ);
  OdString seekFieldGetName_onBinaryChunk();

  //////////////////////////////////////////////
  // reading
  virtual bool         rdBool() {return 0;}
  virtual OdString     rdString() { return _T(""); }
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
private:
  void insertSubclassMarkers(OdString& curFieldName);
};

#endif
