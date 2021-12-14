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

#include "stdafx.h"
#include "ListFiller.h"
#include "TreeFiller.h"
#include "DbObject.h"
#include "OdBinaryData.h"
#include "OdaMfcApp.h"

ListFiller::ListFiller(OdDbDatabase* pDb, CListCtrl* pObjList)
  : m_pObjList(pObjList)
  , m_pDb(pDb)
  , m_nCol(0)
{
}

OdDb::DwgVersion ListFiller::dwgVersion(OdDb::MaintReleaseVer* pMVer) const
{
  if(pMVer) *pMVer = OdDb::kMRelease0;
  return OdDb::kDHL_CURRENT;
}

void ListFiller::wrString(int groupCode, const OdString& string)
{
  CString str;
  str.Format(_T("%i"), groupCode);
  m_pObjList->InsertItem(m_nCol, str);
  m_pObjList->SetItemText(m_nCol, 1, _T("String"));
  m_pObjList->SetItemText(m_nCol++, 2, OdString(string));
}

void ListFiller::wrName(int groupCode, const OdString& string)
{
  CString str;
  str.Format(_T("%i"), groupCode);
  m_pObjList->InsertItem(m_nCol, str);
  m_pObjList->SetItemText(m_nCol, 1, _T("Name"));
  m_pObjList->SetItemText(m_nCol++, 2, OdString(string));
}

void ListFiller::wrBool(int groupCode, bool val)
{
  CString str;
  str.Format(_T("%i"), groupCode);
  m_pObjList->InsertItem(m_nCol, str);
  m_pObjList->SetItemText(m_nCol, 1, _T("Boolean"));
  str = val == 0 ? _T("false") : _T("true");
  m_pObjList->SetItemText(m_nCol++, 2, str);
}

void ListFiller::wrInt8(int groupCode, OdInt8 val)
{
  CString str;
  str.Format(_T("%i"), groupCode);
  m_pObjList->InsertItem(m_nCol, str);
  m_pObjList->SetItemText(m_nCol, 1, _T("Int8"));
  str.Format(_T("%i"), int(val));
  m_pObjList->SetItemText(m_nCol++, 2, str);
}

void ListFiller::wrUInt8(int groupCode, OdUInt8 val)
{
  CString str;
  str.Format(_T("%i"), groupCode);
  m_pObjList->InsertItem(m_nCol, str);
  m_pObjList->SetItemText(m_nCol, 1, _T("UInt8"));
  str.Format(_T("%u"), int(val));
  m_pObjList->SetItemText(m_nCol++, 2, str);
}

void ListFiller::wrInt16(int groupCode, OdInt16 val)
{
  CString str;
  str.Format(_T("%i"), groupCode);
  m_pObjList->InsertItem(m_nCol, str);
  m_pObjList->SetItemText(m_nCol, 1, _T("Int16"));
  str.Format(_T("%i"), int(val));
  m_pObjList->SetItemText(m_nCol++, 2, str);
}

void ListFiller::wrUInt16(int groupCode, OdUInt16 val)
{
  CString str;
  str.Format(_T("%i"), groupCode);
  m_pObjList->InsertItem(m_nCol, str);
  m_pObjList->SetItemText(m_nCol, 1, _T("UInt16"));
  str.Format(_T("%u"), int(val));
  m_pObjList->SetItemText(m_nCol++, 2, str);
}

void ListFiller::wrInt32(int groupCode, OdInt32 val)
{
  CString str;
  str.Format(_T("%i"), groupCode);
  m_pObjList->InsertItem(m_nCol, str);
  m_pObjList->SetItemText(m_nCol, 1, _T("Int32"));
  str.Format(_T("%li"), val);
  m_pObjList->SetItemText(m_nCol++, 2, str);
}

void ListFiller::wrUInt32(int groupCode, OdUInt32 val)
{
  CString str;
  str.Format(_T("%i"), groupCode);
  m_pObjList->InsertItem(m_nCol, str);
  m_pObjList->SetItemText(m_nCol, 1, _T("UInt32"));
  str.Format(_T("%li"), val);
  m_pObjList->SetItemText(m_nCol++, 2, str);
}

void ListFiller::wrInt64(int groupCode, OdInt64 val)
{
  CString str;
  str.Format(_T("%i"), groupCode);
  m_pObjList->InsertItem(m_nCol, str);
  m_pObjList->SetItemText(m_nCol, 1, _T("Int64"));
  str.Format(_T("%li"), val);
  m_pObjList->SetItemText(m_nCol++, 2, str);
}

void ListFiller::wrUInt64(int groupCode, OdUInt64 val)
{
  CString str;
  str.Format(_T("%i"), groupCode);
  m_pObjList->InsertItem(m_nCol, str);
  m_pObjList->SetItemText(m_nCol, 1, _T("UInt64"));
  str.Format(_T("%li"), val);
  m_pObjList->SetItemText(m_nCol++, 2, str);
}

void ListFiller::wrHandle(int groupCode, OdDbHandle val)
{
  CString str;
  str.Format(_T("%i"), groupCode);
  m_pObjList->InsertItem(m_nCol, str);
  m_pObjList->SetItemText(m_nCol, 1, _T("Handle"));
  m_pObjList->SetItemText(m_nCol++, 2, val.ascii());
}

void ListFiller::wrObjectId(int groupCode, OdDbObjectId val)
{
  CString str;
  str.Format(_T("%i"), groupCode);
  m_pObjList->InsertItem(m_nCol, str);
  switch(OdDxfCode::_getType(groupCode))
  {
  case OdDxfCode::SoftPointerId:
    str = _T("SoftPointerRef");
    break;
  case OdDxfCode::HardPointerId:
    str = _T("HardPointerRef");
    break;
  case OdDxfCode::SoftOwnershipId:
    str = _T("SoftOwnershipRef");
    break;
  case OdDxfCode::HardOwnershipId:
    str = _T("HardOwnershipRef");
    break;
  default:
    str = _T("ArbitraryId");
    break;
  }
  m_pObjList->SetItemText(m_nCol,   1, str);
  m_pObjList->SetItemText(m_nCol++, 2, COdaMfcAppApp::objectIdAndClassName(val));
}

void ListFiller::wrAngle(int groupCode, double val, int precision)
{
  CString str, format;
  str.Format(_T("%i"), groupCode);
  m_pObjList->InsertItem(m_nCol, str);
  m_pObjList->SetItemText(m_nCol, 1, _T("Angle"));
  if (precision == kDfltPrec)
    format = _T("%g");
  else
    format.Format(_T("%%.%ilf"), precision);
  str.Format(format, val);
  m_pObjList->SetItemText(m_nCol++, 2, str);
}

void ListFiller::wrDouble(int groupCode, double val, int precision)
{
  int type = OdDxfCode::_getType(groupCode);
  ODA_ASSERT(type == OdDxfCode::Double
    || type == OdDxfCode::Angle);
  CString str, format;
  str.Format(_T("%i"), groupCode);
  m_pObjList->InsertItem(m_nCol, str);
  m_pObjList->SetItemText(m_nCol, 1, _T("Double"));
  if (precision == kDfltPrec)
    format = _T("%g");
  else
    format.Format(_T("%%.%ilf"), precision);
  str.Format(format, val);
  m_pObjList->SetItemText(m_nCol++, 2, str);
}

void ListFiller::wrPoint2d(int groupCode, const OdGePoint2d& pt, int precision)
{
  CString str, format, format1;
  str.Format(_T("%i"), groupCode);
  m_pObjList->InsertItem(m_nCol, str);
  m_pObjList->SetItemText(m_nCol, 1, _T("Point2d"));
  if (precision == kDfltPrec)
    format = _T("%g");
  else
    format.Format(_T("%%.%ilf"), precision);

  format1 = '(' + format + _T(", ") + format + ')';
  str.Format(format1, pt.x, pt.y);
  m_pObjList->SetItemText(m_nCol++, 2, str);
}

void ListFiller::wrPoint3d(int groupCode, const OdGePoint3d& pt, int precision)
{
  CString str, format, format1;
  str.Format(_T("%i"), groupCode);
  m_pObjList->InsertItem(m_nCol, str);
  m_pObjList->SetItemText(m_nCol, 1, _T("Point3d"));
  if (precision == kDfltPrec)
    format = _T("%g");
  else
    format.Format(_T("%%.%ilf"), precision);

  format1 = '(' + format + _T(", ") + format + _T(", ") + format + ')';
  str.Format(format1, pt.x, pt.y, pt.z);
  m_pObjList->SetItemText(m_nCol++, 2, str);
}

void ListFiller::wrVector2d(int groupCode, const OdGeVector2d& pt,
                            int precision)
{
  CString str, format, format1;
  str.Format(_T("%i"), groupCode);
  m_pObjList->InsertItem(m_nCol, str);
  m_pObjList->SetItemText(m_nCol, 1, _T("Vector2d"));
  if (precision == kDfltPrec)
    format = _T("%g");
  else
    format.Format(_T("%%.%ilf"), precision);

  format1 = '(' + format + _T(", ") + format + ')';
  str.Format(format1, pt.x, pt.y);
  m_pObjList->SetItemText(m_nCol++, 2, str);
}

void ListFiller::wrVector3d(int groupCode, const OdGeVector3d& pt,
                            int precision)
{
  CString str, format, format1;
  str.Format(_T("%i"), groupCode);
  m_pObjList->InsertItem(m_nCol, str);
  m_pObjList->SetItemText(m_nCol, 1, _T("Vector3d"));
  if (precision == kDfltPrec)
    format = _T("%g");
  else
    format.Format(_T("%%.%ilf"), precision);

  format1 = '(' + format + _T(", ") + format + _T(", ") + format + ')';
  str.Format(format1, pt.x, pt.y, pt.z);
  m_pObjList->SetItemText(m_nCol++, 2, str);
}

void ListFiller::wrScale3d(int groupCode, const OdGeScale3d& pt,
                           int precision)
{
  CString str, format, format1;
  str.Format(_T("%i"), groupCode);
  m_pObjList->InsertItem(m_nCol, str);
  m_pObjList->SetItemText(m_nCol, 1, _T("Scale3d"));
  if (precision == kDfltPrec)
    format = _T("%g");
  else
    format.Format(_T("%%.%ilf"), precision);

  format1 = '(' + format + _T(", ") + format + _T(", ") + format + ')';
  str.Format(format1, pt.sx, pt.sy, pt.sz);
  m_pObjList->SetItemText(m_nCol++, 2, str);
}

void ListFiller::wrBinaryChunk(int groupCode, const OdUInt8* /*pBuff*/, OdUInt32 nSize)
{
  CString str;
  str.Format(_T("%i"), groupCode);
  m_pObjList->InsertItem(m_nCol, str);
  m_pObjList->SetItemText(m_nCol, 1, _T("Binary"));
  str.Format(_T("(%i) { ... }"), nSize);
  m_pObjList->SetItemText(m_nCol++, 2, str);
}

void ListFiller::rdString(OdString&)
{
  ODA_FAIL();
  throw OdError(eNotApplicable);
}
bool ListFiller::rdBool()
{
  ODA_FAIL();
  throw OdError(eNotApplicable);
  //return false;
}
OdInt8 ListFiller::rdInt8()
{
  ODA_FAIL();
  throw OdError(eNotApplicable);
  //return 0;
}
OdInt16 ListFiller::rdInt16()
{
  ODA_FAIL();
  throw OdError(eNotApplicable);
  //return 0;
}
OdInt32 ListFiller::rdInt32()
{
  ODA_FAIL();
  throw OdError(eNotApplicable);
  //return 0;
}
OdInt64 ListFiller::rdInt64()
{
  ODA_FAIL();
  throw OdError(eNotApplicable);
  //return 0;
}
OdUInt8 ListFiller::rdUInt8()
{
  ODA_FAIL();
  throw OdError(eNotApplicable);
  //return 0;
}
OdUInt16 ListFiller::rdUInt16()
{
  ODA_FAIL();
  throw OdError(eNotApplicable);
  //return 0;
}
OdUInt32 ListFiller::rdUInt32()
{
  ODA_FAIL();
  throw OdError(eNotApplicable);
  //return 0;
}
OdUInt64 ListFiller::rdUInt64()
{
  ODA_FAIL();
  throw OdError(eNotApplicable);
  //return 0;
}
OdDbHandle ListFiller::rdHandle()
{
  ODA_FAIL();
  throw OdError(eNotApplicable);
  //return 0;
}
OdDbObjectId ListFiller::rdObjectId()
{
  ODA_FAIL();
  throw OdError(eNotApplicable);
  //return OdDbObjectId();
}
double  ListFiller::rdAngle()
{
  ODA_FAIL();
  throw OdError(eNotApplicable);
  //return 0;
}
double ListFiller::rdDouble()
{
  ODA_FAIL();
  throw OdError(eNotApplicable);
  //return 0;
}
void ListFiller::rdPoint2d(OdGePoint2d& )
{
  ODA_FAIL();
  throw OdError(eNotApplicable);
}
void ListFiller::rdPoint3d(OdGePoint3d& )
{

  ODA_FAIL();
  throw OdError(eNotApplicable);
}
void ListFiller::rdVector2d(OdGeVector2d& )
{
  ODA_FAIL();
  throw OdError(eNotApplicable);
}
void ListFiller::rdVector3d(OdGeVector3d& )
{
  ODA_FAIL();
  throw OdError(eNotApplicable);
}
void ListFiller::rdScale3d(OdGeScale3d& )
{
  ODA_FAIL();
  throw OdError(eNotApplicable);
}
void ListFiller::rdBinaryChunk(OdBinaryData&)
{
  ODA_FAIL();
  throw OdError(eNotApplicable);
}

void ListFillerDwg::listDwgFields(const OdDbObject* obj, bool odaSchema) {
  seekObjectSchema(odaSchema ? obj : 0);
  substituteAssertFunc();
  try {
    obj->dwgOutFields(this);
    restoreAssertFunc();
  }
  catch (...) {
    restoreAssertFunc();
    throw;
  }
}

void ListFillerDwg::insertSubclassMarkers(OdString& curFieldName) {
  OdString curSubclass = m_pSchemaIterator->currentSubClass();
  OdString curSubobject = m_pSchemaIterator->currentSubObject();
  OdString curSubobjectName = m_pSchemaIterator->currentSubObjectName();
  bool atSubclass = false;
  if (subclass != curSubclass) {
    atSubclass = true;
    subclass = curSubclass;
    m_pObjList->InsertItem(m_nCol, _T("........................................................"));
    m_pObjList->SetItemText(m_nCol, 1, L"< "+subclass+L" >");
    m_pObjList->SetItemText(m_nCol++, 2, _T("........................................................"));
  }
  if (subobject != curSubobject || subobjectName != curSubobjectName) {
    if (subobject.isEmpty() || subobjectName != curSubobjectName) {
      subobjectName = curSubobjectName;
      m_pObjList->InsertItem(m_nCol, curSubobjectName + _T("  ........................................................"));
      m_pObjList->SetItemText(m_nCol, 1, L"{ "+curSubobject+L" }");
      m_pObjList->SetItemText(m_nCol++, 2, _T("........................................................"));
    }
    else if (!atSubclass && curSubobject.isEmpty() && !subobject.isEmpty()) {
      m_pObjList->InsertItem(m_nCol, _T("........................................................"));
      m_pObjList->SetItemText(m_nCol, 1,  _T(".............."));
      m_pObjList->SetItemText(m_nCol++, 2, _T("........................................................"));
    }
    subobject = curSubobject;
  }
  if (!subobject.isEmpty())
    curFieldName = OdString(". ") + curFieldName;
}

void ListFillerDwg::wrThickness(double val) {
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

void ListFillerDwg::wrExtrusion(const OdGeVector3d& v) {
  bool defaultExtrusion = (v.x == 0.0 && v.y == 0.0 && v.z == 1.0);
  wrBool(defaultExtrusion);
  if (!defaultExtrusion)
    wrVector3d(v);
}

void ListFillerDwg::wrBool(bool b)
{
  m_pObjList->InsertItem(m_nCol, seekFieldGetName((OdInt64)b));
  m_pObjList->SetItemText(m_nCol, 1, _T("Bool"));
  m_pObjList->SetItemText(m_nCol++, 2, b ? _T("true") : _T("false"));
}

void ListFillerDwg::wrString(const OdString &string)
{
  m_pObjList->InsertItem(m_nCol, seekFieldGetName(string));
  m_pObjList->SetItemText(m_nCol, 1, OdString(_T("String")));
  m_pObjList->SetItemText(m_nCol++, 2, OdString().format(_T("\"%s\""), string.c_str()));
}
void ListFillerDwg::wrBytes(const void* /*buffer*/, OdUInt32 nLen)
{
  if (m_bHasSchema && currentType() == OdTf::Empty)
    skipEmptyType();
  if (m_bHasSchema && currentType() != OdTf::HexBinary)
    return;
  CString str;
  m_pObjList->InsertItem(m_nCol, seekFieldGetName_onBinaryChunk());
  m_pObjList->SetItemText(m_nCol, 1, _T("Binary"));
  str.Format(_T("(%i) { ... }"), nLen);
  m_pObjList->SetItemText(m_nCol++, 2, str);
}
void ListFillerDwg::wrInt8(OdInt8 val)
{
  CString str;
  m_pObjList->InsertItem(m_nCol, seekFieldGetName((OdInt64)val));
  m_pObjList->SetItemText(m_nCol, 1, _T("Int8"));
  if(val >= 0 && val < 10)
    str.Format(_T("%i"), int(val));
  else
    str.Format(_T("%i : %02X"), int(val), int(val));
  m_pObjList->SetItemText(m_nCol++, 2, str);
}
void ListFillerDwg::wrUInt8(OdUInt8 val)
{
  CString str;
  m_pObjList->InsertItem(m_nCol, seekFieldGetName((OdInt64)val));
  m_pObjList->SetItemText(m_nCol, 1, _T("UInt8"));
  if(val<10)
    str.Format(_T("%u"), int(val));
  else
    str.Format(_T("%u : %02X"), int(val), int(val));
  m_pObjList->SetItemText(m_nCol++, 2, str);
}
void ListFillerDwg::wrInt16(OdInt16 val)
{
  CString str;
  m_pObjList->InsertItem(m_nCol, seekFieldGetName((OdInt64)val));
  m_pObjList->SetItemText(m_nCol, 1, _T("Int16"));
  if (val >= 0 && val < 10)
    str.Format(_T("%i"), int(val));
  else
    str.Format(_T("%i : %04X"), int(val), int(OdUInt16(val)));
  m_pObjList->SetItemText(m_nCol++, 2, str);
}
void ListFillerDwg::wrInt32(OdInt32 val)
{
  CString str;
  m_pObjList->InsertItem(m_nCol, seekFieldGetName((OdInt64)val));
  m_pObjList->SetItemText(m_nCol, 1, _T("Int32"));
  if (val >= 0 && val < 10)
    str.Format(_T("%li"), val);
  else
    str.Format(_T("%li : %08X"), val, val);
  m_pObjList->SetItemText(m_nCol++, 2, str);
}
void ListFillerDwg::wrInt64(OdInt64 val)
{
  CString str;
  m_pObjList->InsertItem(m_nCol, seekFieldGetName(val));
  m_pObjList->SetItemText(m_nCol, 1, _T("Int64"));
  if (val >= 0 && val < 10)
    str.Format(_T("%I64i"), val);
  else
    str.Format(_T("%I64i : %02I64X"), val, val);
  m_pObjList->SetItemText(m_nCol++, 2, str);
}
void ListFillerDwg::wrDouble(double val)
{
  CString str;
  m_pObjList->InsertItem(m_nCol, seekFieldGetName(val));
  m_pObjList->SetItemText(m_nCol, 1, _T("double"));
  str.Format(_T("%g"), val);
  if(str.GetLength()==1)
    str.Format(_T("%.1f"), val);
  m_pObjList->SetItemText(m_nCol++, 2, str);
}
void ListFillerDwg::wrDbHandle(const OdDbHandle& val)
{
  CString str;
  m_pObjList->InsertItem(m_nCol, seekFieldGetName((OdInt64)val));
  m_pObjList->SetItemText(m_nCol, 1, _T("Handle"));
  m_pObjList->SetItemText(m_nCol++, 2, val.ascii());
}
void ListFillerDwg::wrSoftOwnershipId(const OdDbObjectId& id)
{
  CString str;
  m_pObjList->InsertItem(m_nCol, seekFieldGetName(((OdInt64)id.getHandle())));
  str = _T("SoftOwnershipRef");
  m_pObjList->SetItemText(m_nCol, 1, str);
  m_pObjList->SetItemText(m_nCol++, 2, COdaMfcAppApp::objectIdAndClassName(id));
}
void ListFillerDwg::wrHardOwnershipId(const OdDbObjectId& id)
{
  CString str;
  m_pObjList->InsertItem(m_nCol, seekFieldGetName(((OdInt64)id.getHandle())));
  str = _T("HardOwnershipRef");
  m_pObjList->SetItemText(m_nCol, 1, str);
  m_pObjList->SetItemText(m_nCol++, 2, COdaMfcAppApp::objectIdAndClassName(id));
}
void ListFillerDwg::wrSoftPointerId(const OdDbObjectId& id)
{
  CString str;
  m_pObjList->InsertItem(m_nCol, seekFieldGetName(((OdInt64)id.getHandle())));
  str = _T("SoftPointer");
  m_pObjList->SetItemText(m_nCol, 1, str);
  m_pObjList->SetItemText(m_nCol++, 2, COdaMfcAppApp::objectIdAndClassName(id));
}
void ListFillerDwg::wrHardPointerId(const OdDbObjectId& id)
{
  CString str;
  m_pObjList->InsertItem(m_nCol, seekFieldGetName(((OdInt64)id.getHandle())));
  str = _T("HardPointer");
  m_pObjList->SetItemText(m_nCol, 1, str);
  m_pObjList->SetItemText(m_nCol++, 2, COdaMfcAppApp::objectIdAndClassName(id));
}
void ListFillerDwg::wrPoint2d(const OdGePoint2d& pt)
{
  if (m_bHasSchema && currentType() == OdTf::Empty)
    skipEmptyType();
  if (m_bHasSchema && currentType() == OdTf::DoubleWithDefault)
  {
    bool bFound = false;
    m_pSchemaIterator->skipDefDouble();
    seekFieldGetName(.0);
    m_pSchemaIterator->skipDefDouble();
    seekFieldGetName(.0);
    m_pObjList->InsertItem(m_nCol, OdString(L"point"));
  }
  else
  {
    m_pObjList->InsertItem(m_nCol, seekFieldGetName(pt.x, pt.y));
  }
  CString str, format, format1;
  m_pObjList->SetItemText(m_nCol, 1, _T("Point2d"));
  format = _T("%g");
  format1 = '(' + format + _T(", ") + format + ')';
  str.Format(format1, pt.x, pt.y);
  m_pObjList->SetItemText(m_nCol++, 2, str);
}
void ListFillerDwg::wrPoint3d(const OdGePoint3d& pt)
{
  CString str, format, format1;
  m_pObjList->InsertItem(m_nCol, seekFieldGetName(pt.x, pt.y));
  m_pObjList->SetItemText(m_nCol, 1, _T("Point3d"));
  format = _T("%g");
  format1 = '(' + format + _T(", ") + format + _T(", ") + format + ')';
  str.Format(format1, pt.x, pt.y, pt.z);
  m_pObjList->SetItemText(m_nCol++, 2, str);
}
void ListFillerDwg::wrVector2d(const OdGeVector2d& vec)
{
  CString str, format, format1;
  m_pObjList->InsertItem(m_nCol, seekFieldGetName(vec.x, vec.y));
  m_pObjList->SetItemText(m_nCol, 1, _T("Vector2d"));
  format = _T("%g");
  format1 = '(' + format + _T(", ") + format + ')';
  str.Format(format1, vec.x, vec.y);
  m_pObjList->SetItemText(m_nCol++, 2, str);
}
void ListFillerDwg::wrVector3d(const OdGeVector3d& vec)
{
  CString str, format, format1;
  m_pObjList->InsertItem(m_nCol, seekFieldGetName(vec.x, vec.y, vec.z));
  m_pObjList->SetItemText(m_nCol, 1, _T("Vector3d"));
  format = _T("%g");
  format1 = '(' + format + _T(", ") + format + _T(", ") + format + ')';
  str.Format(format1, vec.x, vec.y, vec.z);
  m_pObjList->SetItemText(m_nCol++, 2, str);
}
void ListFillerDwg::wrScale3d(const OdGeScale3d& point)
{
  CString str, format, format1;
  m_pObjList->InsertItem(m_nCol, seekFieldGetName(point.sx, point.sy, point.sz));
  m_pObjList->SetItemText(m_nCol, 1, _T("Scale3d"));
  format = _T("%g");
  format1 = '(' + format + _T(", ") + format + _T(", ") + format + ')';
  str.Format(format1, point.sx, point.sy, point.sz);
  m_pObjList->SetItemText(m_nCol++, 2, str);
}

// Need to implement to do not force OdaMfcApp to "fly away"
int ListFiller::precision() const
{
  return 8;
}

ListFillerDwg::ListFillerDwg(OdDbDatabase* pDb, CListCtrl* pObjList)
  : m_pObjList(pObjList)
  , m_pDb(pDb)
  , m_nCol(0)
  , m_nEmbeddedSubobjectFollowState(0)
  , m_bHasSchema(false)
  , m_savedAssertFunc(0)
{
}

bool ListFillerDwg::seekObjectSchema(const OdDbObject* pObj) {
  if (pObj) {
    if (m_pSchemaIterator.isNull())
      m_pSchemaIterator = OdDbObjectSchemaIterator::createObject();
    const OdRxClass* c = pObj->isA();
    const OdRxClass* d = OdGiDrawable::desc();
    do {
      m_bHasSchema = m_pSchemaIterator->seekClass(c, m_pDb->appServices());
      c = c->myParent();
    }
    while (!m_bHasSchema && c != d);
  }
  else {
    m_bHasSchema = false;
  }
  subclass.empty();
  subobject.empty();
  return m_bHasSchema;
}

#include "SaveState.h"
extern AECOwnershipTreeExpandPE* g_pTreeExpandPE;

void ListFillerDwg::wrAddress(const void* val) {
  switch( m_nEmbeddedSubobjectFollowState ) {
    case 0:
      // "this" pointer used as an identifier that embedded subobject follows
      // Caller calls: pFiler->wrAddress( pFiler ) and then immediately - pFiler->wrAddress( pEmbeddedSubobject )
      if( val == this )
        m_nEmbeddedSubobjectFollowState = 1;
      break;
    case 1:
      {
      const OdRxObject* pObj = (const OdRxObject*)val;
      if( pObj ) {
        OdString sName;
        sName.format(OD_T("{ Subobject begin: 0x%08X <%s> }"), pObj, pObj->isA()->name().c_str() );
        wrString( sName );
        /*
        sName.format(OD_T("{ Subobject begin: 0x%08X <%s>"), pObj, pObj->isA()->name().c_str() );
        wrString( sName );
        if( g_pTreeExpandPE ) {
          OdSaveState<int> ss_m_nEmbeddedSubobjectFollowState(m_nEmbeddedSubobjectFollowState, 0);
          g_pTreeExpandPE->outFields( pObj, this );
        }
        wrString(OD_T("Subobject end }"));
        */
      }
      else {
        wrString( OD_T("{ Subobject 0x00000000 }") );
      }
      m_nEmbeddedSubobjectFollowState = 2;
      }
      break;
    case 2:
      *((bool*)val) = *((bool*)val) = (g_pTreeExpandPE!=0); // message to caller that filer has processed trick
      m_nEmbeddedSubobjectFollowState = 0;
      break;
  }
}

OdString ListFillerDwg::seekFieldGetName() {
  if (m_bHasSchema) {
    OdString fieldName = m_pSchemaIterator->seekFieldGetName();
    insertSubclassMarkers(fieldName);
    return fieldName;
  }
  return OdString::kEmpty;
}
OdString ListFillerDwg::seekFieldGetName(const OdString& sValue) {
  if (m_bHasSchema) {
    OdString fieldName = m_pSchemaIterator->seekFieldGetName(sValue);
    insertSubclassMarkers(fieldName);
    return fieldName;
  }
  return OdString::kEmpty;
}
OdString ListFillerDwg::seekFieldGetName(const OdInt64& data) {
  if (m_bHasSchema) {
    OdString fieldName = m_pSchemaIterator->seekFieldGetName(data);
    insertSubclassMarkers(fieldName);
    return fieldName;
  }
  return OdString::kEmpty;
}
OdString ListFillerDwg::seekFieldGetName(const double& value) {
  if (m_bHasSchema) {
    OdString fieldName = m_pSchemaIterator->seekFieldGetName(value);
    insertSubclassMarkers(fieldName);
    return fieldName;
  }
  return OdString::kEmpty;
}
OdString ListFillerDwg::seekFieldGetName(const double& valueX, const double& valueY) {
  if (m_bHasSchema) {
    OdString fieldName = m_pSchemaIterator->seekFieldGetName(valueX, valueY);
    insertSubclassMarkers(fieldName);
    return fieldName;
  }
  return OdString::kEmpty;
}
OdString ListFillerDwg::seekFieldGetName(const double& valueX, const double& valueY, const double& valueZ) {
  if (m_bHasSchema) {
    OdString fieldName = m_pSchemaIterator->seekFieldGetName(valueX, valueY, valueZ);
    insertSubclassMarkers(fieldName);
    return fieldName;
  }
  return OdString::kEmpty;
}
OdString ListFillerDwg::seekFieldGetName_onBinaryChunk() {
  if (m_bHasSchema) {
    OdString fieldName = m_pSchemaIterator->seekFieldGetName_onBinaryChunk();
    insertSubclassMarkers(fieldName);
    return fieldName;
  }
  return OdString::kEmpty;
}

OdUInt64 ListFillerDwg::tell() const {
  return 0;
}

OdTf::TypesEnum ListFillerDwg::currentType()
{
  return m_pSchemaIterator->currentType();
}

void ListFillerDwg::skipEmptyType()
{
  while (m_pSchemaIterator->currentType() == OdTf::Empty) {
    m_pSchemaIterator->step();
  }
}

OdAssertFunc ListFillerDwg::g_savedAssertFunc = 0;

void ListFillerDwg::substituteAssertFunc() {
  ODA_ASSERT_ONCE(!m_savedAssertFunc);
  if(!m_savedAssertFunc)
    g_savedAssertFunc = m_savedAssertFunc = ::odSetAssertFunc(&ListFillerDwg::myAssert);
}

void ListFillerDwg::restoreAssertFunc() {
  ODA_ASSERT_ONCE(m_savedAssertFunc);
  if (m_savedAssertFunc)
    ::odSetAssertFunc(m_savedAssertFunc);
  g_savedAssertFunc = m_savedAssertFunc = 0;
}

void ListFillerDwg::myAssert(const char* expr, const char* fileName, int nLine) {
  if(OdString(fileName).right(23)==L"TfSchemaManagerImpl.cpp" && nLine==237) {
    ; // suppress assertion dialog box
  }
  else {
    g_savedAssertFunc(expr, fileName, nLine);
  }
}

