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

#define STL_USING_IOSTREAM
#include "OdaSTL.h"
#define  STD(a)  std:: a

#include "DbFDumper.h"
#include "toString.h"

#define _T(x) L##x

void DbFDumper::tokenize(OdDbObjectIdArray& outarr, const OdString& input, const OdChar& delim, bool remap) {
  if (input.isEmpty())
    return;

  const OdChar *pbuf = input.c_str();
  int npos(0), nprevpos(0), nlen = input.getLength();

  while (npos <= nlen) {
    if (pbuf[npos] == delim || pbuf[npos] == 0) {
      if (npos != nprevpos) { // skip empty strings
        OdString sHandle(OdString(pbuf + nprevpos, npos - nprevpos));
        OdDbHandle h(sHandle);
        if(h.isNull()) {
          if(remap)
            remapHandle(h);
          outarr.push_back(m_pDb->objectId());
        }
        else {
          OdDbObjectId id = m_pDb->getOdDbObjectId(h);
          if(id.isNull())
            odPrintConsoleString(L"Invalid handle specified in option: %s\n", sHandle.c_str());
          else {
            if(remap)
              remapHandle(h);
            outarr.push_back(id);
          }
        }
      }
      nprevpos = npos + 1;
    }
    ++npos;
  }
}


OdString DbFDumper::objectIdAndClassName(const OdDbObject* pObj) {
  if (pObj) {
    OdDbHandle h = pObj->objectId().getHandle();
    return OdString().format(_T("%02I64X : < %ls >"),
      OdUInt64(remapHandle(h)),
      pObj->isA()->name().c_str()
    );
  }
  return OdString(_T("00 : < >"));
}


OdString DbFDumper::objectIdAndClassName(OdDbObjectId id) {
  return objectIdAndClassName(id.openObject());
}


DbFDumper::DbFDumper()
  : indent(0)
  , nEmbeddedSubobjectFollowState(0)
  , hasSchema(false)
  , dumpRefs(false)
  , m_savedAssertFunc(0)
{
}

void DbFDumper::fdump(OdDbDatabase* pDb, const OdString& sset, const OdString& blist, bool dumpRefs, bool remap) {
  m_pDb = pDb;
  numDumpedObjects = 0;
  remaphash.clear();
  remaphash.insert(std::pair<OdUInt64, OdUInt64>(0,0));
  this->remap = remap;
  OdDbObjectIdArray blstids;
  tokenize(blstids, blist, L',', false);
  for (OdUInt32 i = 0; i < blstids.size(); ++i) {
    this->blist.insert(blstids[i]);
  }

  if(sset.isEmpty()) { // dump all
    this->dumpRefs = true; // forced
    ::odPrintConsoleString(L"Dump in ODA format all objects in drawing.\n\n");
    fdumpObject(pDb->objectId());
    dumpReferences();
  }
  else { // dump specified only
    this->dumpRefs = dumpRefs;
    ::odPrintConsoleString(L"Dump in ODA format selected objects: %s\n\n", sset.c_str());
    OdDbObjectIdArray ssids;
    tokenize(ssids, sset, L',', true);
    for(OdUInt32 i=0; i<ssids.size(); ++i) {
      fdumpObject(ssids[i]);
      dumpReferences();
    }
  }
  if(remap) {
    ::odPrintConsoleString(L"Handle mapping:\n");
    OdArray<OdDbHandle> handles(OdUInt32(remaphash.size()), 1);
    handles.resize(OdUInt32(remaphash.size()));
    OdUInt64HashMap::const_iterator it = remaphash.begin(), end = remaphash.end();
    for(; it!=end; ++it) {
      handles[it->second] = it->first;
    }
    for(OdUInt32 i=1; i<handles.size(); ++i) {
      ::odPrintConsoleString(L"%8X <= %8ls  :  < %s >\n", i, handles[i].ascii().c_str(),
        database()->getOdDbObjectId(handles[i]).safeOpenObject(OdDb::kForRead, true)->isA()->name().c_str());
    }
    ::odPrintConsoleString(L"\n");
  }
  if(numDumpedObjects==1)
    ::odPrintConsoleString(L"1 object was dumped in total\n\n");
  else
    ::odPrintConsoleString(L"%d objects were dumped in total\n\n", numDumpedObjects);
}

void DbFDumper::dumpReferences() {
  while (!refQueue.empty()) {
    fdumpObject(refQueue.front());
    refQueue.pop();
  }
}

void DbFDumper::listDwgFields(const OdDbObject* obj) {
  seekObjectSchema(obj);
   // substituteAssertFunc();
  try {
    obj->dwgOutFields(this);
    // restoreAssertFunc();
  }
  catch (const OdError& e) {
    ::odPrintConsoleString(e.description());
    ::odPrintConsoleString(L"\n");
  }
/*
  catch (...) {
    restoreAssertFunc();
    throw;
  }
*/
}


void DbFDumper::fdumpObject(const OdDbObjectId& id, int indent) {
  if(!blist.empty() && blist.find(id)!=blist.end()) // check black list
    return;

  m_pDb = id.database();

  /**********************************************************************/
  /* Get a SmartPointer to the object                                   */
  /**********************************************************************/
  OdDbObjectPtr pObject = id.openObject();

  if (pObject.isNull()) {
    writeLine(indent, id.getHandle().ascii(), L"*Erased*");
    return;
  }
  ++numDumpedObjects;

  this->indent = indent = 0;

  /**********************************************************************/
  /* Dump object's handle                                               */
  /**********************************************************************/
  writeLine(5, _T("HANDLE"), remapHandle(id.getHandle()).ascii());

  /**********************************************************************/
  /* Dump object's class name                                           */
  /**********************************************************************/
  writeLine(5, _T("CLASS"),
    OdString().format(_T("< %ls >"), pObject->isA()->name().c_str())
  );

  /********************************************************************/
  /* Dump the object                                                  */
  /********************************************************************/
  listDwgFields(pObject);
  /********************************************************************/
  /* Dump Xrecord Data expanded to set of group-codes                 */
  /********************************************************************/
  if (pObject->isKindOf(OdDbXrecord::desc())) {
    OdDbXrecordPtr pXRec = pObject;
    dumpResBuf(pXRec->rbChain(), L"XRecord Data", indent);
  }
  /********************************************************************/
  /* Dump Object's XData                                              */
  /********************************************************************/
  dumpXdata(pObject->xData(), indent);
  writeLine();
}

void DbFDumper::insertSubclassMarkers(OdString& curFieldName) {
  OdString curSubclass = m_pSchemaIterator->currentSubClass();
  OdString curSubobject = m_pSchemaIterator->currentSubObject();
  OdString curSubobjectName = m_pSchemaIterator->currentSubObjectName();
  bool atSubclass = false;
  if (subclass != curSubclass) {
    atSubclass = true;
    subclass = curSubclass;
    writeLine(indent, L"........  SUBCLASS", "< " + subclass + L" >");
  }
  if (subobject != curSubobject || subobjectName != curSubobjectName) {
    if (subobject.isEmpty() || subobjectName != curSubobjectName) {
      subobjectName = curSubobjectName;
      writeLine(indent, L". . . . . " + curSubobjectName, L"{ " + curSubobject + L" }");
    }
    else if (!atSubclass && curSubobject.isEmpty() && !subobject.isEmpty()) {
      writeLine(indent, OdString::kEmpty, L".");
    }
    subobject = curSubobject;
  }
//   if (!subobject.isEmpty())
//     curFieldName = OdString("-> ") + curFieldName;
}


OdDbHandle DbFDumper::remapHandle(OdDbHandle h) {
  if(!remap || h.isNull()) return h;
  OdUInt64HashMap::const_iterator f = remaphash.find(h);
  if(f!=remaphash.end())
    return f->second;
  OdDbHandle m = (OdUInt64)remaphash.size();
  remaphash.insert(std::pair<OdUInt64,OdUInt64>(h, m));
  return m;
}

void DbFDumper::wrBool(bool b) {
  writeLine(indent, _T("Bool      ") + seekFieldGetName((OdInt64)b), b ? _T("true") : _T("false"));
}

void DbFDumper::wrString(const OdString &string) {
  writeLine(indent, _T("String    ") + seekFieldGetName(string), OdString().format(_T("\"%s\""), string.c_str()));
}

void DbFDumper::wrBytes(const void* /*buffer*/, OdUInt32 nLen) {
  OdString str;
  str.format(_T("(%i) { ... }"), nLen);
  writeLine(indent, _T("Binary    ") + seekFieldGetName_onBinaryChunk(), str);
}

void DbFDumper::wrInt8(OdInt8 val) {
  OdString str;
  if(val >= 0 && val < 10)
    str.format(_T("%i"), int(val));
  else
    str.format(_T("%i : %02X"), int(val), int(val));
  writeLine(indent, _T("Int8      ") + seekFieldGetName((OdInt64)val), str);
}

void DbFDumper::wrUInt8(OdUInt8 val) {
  OdString str;
  if(val<10)
    str.format(_T("%u"), int(val));
  else
    str.format(_T("%u : %02X"), int(val), int(val));
  writeLine(indent, _T("UInt8     ") + seekFieldGetName((OdInt64)val), str);
}

void DbFDumper::wrInt16(OdInt16 val) {
  OdString str;
  if (val >= 0 && val < 10)
    str.format(_T("%i"), int(val));
  else
    str.format(_T("%i : %04X"), int(val), int(OdUInt16(val)));
  writeLine(indent, _T("Int16     ") + seekFieldGetName((OdInt64)val), str);
}

void DbFDumper::wrInt32(OdInt32 val) {
  OdString str;
  if (val >= 0 && val < 10)
    str.format(_T("%li"), val);
  else
    str.format(_T("%li : %08X"), val, val);
  writeLine(indent, _T("Int32     ") + seekFieldGetName((OdInt64)val), str);
}

void DbFDumper::wrInt64(OdInt64 val) {
  OdString str;
  if (val >= 0 && val < 10)
    str.format(_T("%I64i"), val);
  else
    str.format(_T("%I64i : %02I64X"), val, val);
  writeLine(indent, _T("Int64     ") + seekFieldGetName(val), str);
}

void DbFDumper::wrDouble(double val) {
  OdString str;
  str.format(_T("%g"), val);
  if(str.getLength()==1)
    str.format(_T("%.1f"), val);
  writeLine(indent, _T("double    ") + seekFieldGetName(val), str);
}

void DbFDumper::wrDbHandle(const OdDbHandle& val) {
  writeLine(indent, _T("Handle    ") + seekFieldGetName((OdInt64)val), val.ascii());
}

void DbFDumper::wrSoftOwnershipId(const OdDbObjectId& id) {
  writeLine(indent, _T("SoftOwn   ") + seekFieldGetName(((OdInt64)id.getHandle())), objectIdAndClassName(id));
  if (dumpRefs && !id.isErased() && dumped.find(id) == dumped.end()) {
    refQueue.push(id); dumped.insert(id);
  }
}

void DbFDumper::wrHardOwnershipId(const OdDbObjectId& id) {
  writeLine(indent, _T("HardOwn   ") + seekFieldGetName(((OdInt64)id.getHandle())), objectIdAndClassName(id));
  if (dumpRefs && !id.isErased() && dumped.find(id) == dumped.end()) {
    refQueue.push(id); dumped.insert(id);
  }
}

void DbFDumper::wrSoftPointerId(const OdDbObjectId& id) {
  writeLine(indent, _T("SoftPtr   ") + seekFieldGetName(((OdInt64)id.getHandle())), objectIdAndClassName(id));
/*
  if (dumpRefs && !id.isErased() && dumped.find(id) == dumped.end()) {
    refQueue.push(id); dumped.insert(id);
  }
*/
}

void DbFDumper::wrHardPointerId(const OdDbObjectId& id) {
  writeLine(indent, _T("HardPtr   ") + seekFieldGetName(((OdInt64)id.getHandle())), objectIdAndClassName(id));
  if (dumpRefs && !id.isErased() && dumped.find(id) == dumped.end()) {
    refQueue.push(id); dumped.insert(id);
  }
}

void DbFDumper::wrPoint2d(const OdGePoint2d& pt) {
  OdString str, format, format1;
  format = _T("%g");
  format1 = "(" + format + _T(", ") + format + ")";
  str.format(format1.c_str(), pt.x, pt.y);
  writeLine(indent, _T("Point2d   ") + seekFieldGetName(pt.x, pt.y), str);
}

void DbFDumper::wrPoint3d(const OdGePoint3d& pt) {
  OdString str, format, format1;
  format = _T("%g");
  format1 = "(" + format + _T(", ") + format + _T(", ") + format + ")";
  str.format(format1.c_str(), pt.x, pt.y, pt.z);
  writeLine(indent, _T("Point3d   ") + seekFieldGetName(pt.x, pt.y, pt.z), str);
}

void DbFDumper::wrVector2d(const OdGeVector2d& vec) {
  OdString str, format, format1;
  format = _T("%g");
  format1 = "(" + format + _T(", ") + format + ")";
  str.format(format1.c_str(), vec.x, vec.y);
  writeLine(indent, _T("Vector2d  ") + seekFieldGetName(vec.x, vec.y), str);
}

void DbFDumper::wrVector3d(const OdGeVector3d& vec) {
  OdString str, format, format1;
  format = _T("%g");
  format1 = "(" + format + _T(", ") + format + _T(", ") + format + ")";
  str.format(format1.c_str(), vec.x, vec.y, vec.z);
  writeLine(indent, _T("Vector3d  ") + seekFieldGetName(vec.x, vec.y, vec.z), str);
}

void DbFDumper::wrScale3d(const OdGeScale3d& point) {
  OdString str, format, format1;
  format = _T("%g");
  format1 = "(" + format + _T(", ") + format + _T(", ") + format + ")";
  str.format(format1.c_str(), point.sx, point.sy, point.sz);
  writeLine(indent, _T("Scale3d   ") + seekFieldGetName(point.sx, point.sy, point.sz), str);
}

bool DbFDumper::seekObjectSchema(const OdDbObject* pObj) {
  if (pObj) {
    if (m_pSchemaIterator.isNull())
      m_pSchemaIterator = ::odrxSafeCreateObject<OdDbObjectSchemaIterator>(L"OdDbObjectSchemaIterator", L"TD_Tf");
    const OdRxClass* c = pObj->isA();
    const OdRxClass* d = OdGiDrawable::desc();
    do {
      hasSchema = m_pSchemaIterator->seekClass(c, m_pDb->appServices());
      c = c->myParent();
    }
    while (!hasSchema && c != d);
  }
  else {
    hasSchema = false;
  }
  subclass.empty();
  subobject.empty();
  return hasSchema;
}

void DbFDumper::wrAddress(const void* val) {
  ODA_FAIL();
}

OdString DbFDumper::seekFieldGetName() {
  if (hasSchema) {
    OdString fieldName = m_pSchemaIterator->seekFieldGetName();
    insertSubclassMarkers(fieldName);
    return fieldName;
  }
  return OdString::kEmpty;
}

OdString DbFDumper::seekFieldGetName(const OdString& sValue) {
  if (hasSchema) {
    OdString fieldName = m_pSchemaIterator->seekFieldGetName(sValue);
    insertSubclassMarkers(fieldName);
    return fieldName;
  }
  return OdString::kEmpty;
}

OdString DbFDumper::seekFieldGetName(const OdInt64& data) {
  if (hasSchema) {
    OdString fieldName = m_pSchemaIterator->seekFieldGetName(data);
    insertSubclassMarkers(fieldName);
    return fieldName;
  }
  return OdString::kEmpty;
}

OdString DbFDumper::seekFieldGetName(const double& value) {
  if (hasSchema) {
    OdString fieldName = m_pSchemaIterator->seekFieldGetName(value);
    insertSubclassMarkers(fieldName);
    return fieldName;
  }
  return OdString::kEmpty;
}

OdString DbFDumper::seekFieldGetName(const double& valueX, const double& valueY) {
  if (hasSchema) {
    OdString fieldName = m_pSchemaIterator->seekFieldGetName(valueX, valueY);
    insertSubclassMarkers(fieldName);
    return fieldName;
  }
  return OdString::kEmpty;
}

OdString DbFDumper::seekFieldGetName(const double& valueX, const double& valueY, const double& valueZ) {
  if (hasSchema) {
    OdString fieldName = m_pSchemaIterator->seekFieldGetName(valueX, valueY, valueZ);
    insertSubclassMarkers(fieldName);
    return fieldName;
  }
  return OdString::kEmpty;
}

OdString DbFDumper::seekFieldGetName_onBinaryChunk() {
  if (hasSchema) {
    OdString fieldName = m_pSchemaIterator->seekFieldGetName_onBinaryChunk();
    insertSubclassMarkers(fieldName);
    return fieldName;
  }
  return OdString::kEmpty;
}

OdUInt64 DbFDumper::tell() const {
  return 0;
}

OdAssertFunc DbFDumper::g_savedAssertFunc = 0;

void DbFDumper::substituteAssertFunc() {
  ODA_ASSERT_ONCE(!m_savedAssertFunc);
  if(!m_savedAssertFunc)
    g_savedAssertFunc = m_savedAssertFunc = ::odSetAssertFunc(&DbFDumper::myAssert);
}

void DbFDumper::restoreAssertFunc() {
  ODA_ASSERT_ONCE(m_savedAssertFunc);
  if (m_savedAssertFunc)
    ::odSetAssertFunc(m_savedAssertFunc);
  g_savedAssertFunc = m_savedAssertFunc = 0;
}

void DbFDumper::myAssert(const char* expr, const char* fileName, int nLine) {
  if(OdString(fileName).right(23)==L"TfSchemaManagerImpl.cpp" && nLine==237) {
    ; // suppress assertion dialog box
  }
  else {
    g_savedAssertFunc(expr, fileName, nLine);
  }
}

void DbFDumper::wrThickness(double val) {
  if (dwgVersion() > OdDb::vAC14) {
    if (OdNonZero(val)) {
      wrBool(false);
      wrDouble(val);
    }
    else
      wrBool(true);
  }
  else
    wrDouble(val);
}

void DbFDumper::wrExtrusion(const OdGeVector3d& v) {
  bool defaultExtrusion = (v.x == 0.0 && v.y == 0.0 && v.z == 1.0);
  wrBool(defaultExtrusion);
  if (!defaultExtrusion)
    wrVector3d(v);
}

