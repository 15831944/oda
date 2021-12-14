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
#include <math.h>
#include <stdlib.h>
#include "AcExpr.h"

#include <OdaCommon.h>
#include <Ge/GePoint3d.h>
#include <OdRound.h>
#include <DbObjectId.h>
#include <DbHostAppServices.h>
#include "ExFieldEvaluator.h"
#include <Ge/GeLine3d.h>
#include "OdExprEval.h"
#include <DbTable.h>
#include <DbBlockTableRecord.h>
#define STL_USING_LIST
#define STL_USING_ALGORITHM
#define STL_USING_STACK
#include <OdaSTL.h>

OdDbDatabase* database(OdDbField*);
void* convertResultA(double d, AcExprEvalResult* ptr);

OdDbDatabase* database(AcExprEvalResult* ptr)
{
  OdDbField* pField = (OdDbField*)ptr->user_data;
  if (pField && pField->database())
    return pField->database();
  OdFdFieldEnginePEPtr pFieldEnginePE = OdFdFieldEngine::desc()->getX(OdFdFieldEnginePE::desc());
  if (!pFieldEnginePE.isNull())
    return pFieldEnginePE->getCurrentDb();
  return (OdDbDatabase*)ptr->database;
}


extern "C"
{
#include "scanner.h"
#include "parser.h"
  int yyparse (yyscan_t yyscanner, AcExprEvalResult* ptr);
}

ODRX_CONS_DEFINE_MEMBERS(OdExprEvaluator,OdEvaluatorBase,RXIMPL_CONSTR);

OdExprEvaluator::OdExprEvaluator() : OdEvaluatorBase(OD_T("AcExpr")) {}

const OdString OdExprEvaluator::evaluatorId(OdDbField* pField) const 
{
  if (!pField) 
    return OdString::kEmpty;
  OdString code = pField->getFieldCode(OdDbField::kFieldCode);
  if (code.find(OD_T("\\AcExpr ")) != -1)
    return OD_T("AcExpr");
  if (code.find(OD_T("\\AcExpr.16.2 ")) != -1)
    return OD_T("AcExpr.16.2");
  return OdString::kEmpty;
}
OdResult OdExprEvaluator::format(OdDbField* pField, OdString& pszValue) const
{
  OdFieldValue fv; 
  OdResult res = pField->getValue(fv);
  if (res != eOk) return res;
  OdString format = pField->getFormat();
  if (format.isEmpty())
  {
    switch (fv.dataType())
    {
    case OdValue::kDouble:
    case OdValue::k3dPoint:
      format = L"%lu6%pr6";
      break;
    }
  }
  if (fv.format(format, pszValue, database(pField)))
    return eOk;
  return eNotImplemented;
}

#ifdef _DEBUG
extern "C" int yydebug;
#endif
void odEvaluateExpr(const char* str, AcExprEvalResult& result)
{
  yyscan_t scanner;
  yylex_init(&scanner);
  result.success = 1;
  result.result = 0;
#ifdef _DEBUG
  yydebug = 0;
#endif
  YY_BUFFER_STATE buf = yy_scan_string(str, scanner);
  yyparse(scanner, &result);
  yy_delete_buffer(buf, scanner);
  yylex_destroy(scanner);
}

// 1.  Scientific      1.55E+01
// 2.  Decimal         15.50
// 3.  Engineering     1'-3.50"
// 4.  Architectural   1'-3 1/2"
// 5.  Fractional      15 1/2
OdString getFormat(OdString code)
{
  if (code.find(OdChar('E')) != -1 || code.find(OdChar('e')) != -1) 
  {
    if (code.find(OdChar('.')) != -1 || code.find(OdChar('+')) != -1 || code.find(OdChar('-')) != -1)
      return OdString(OD_T("%lu1"));
    return OdString(OD_T("%lu6"));
  }
  if (code.find(OdChar('.')) != -1)
  {
    if (code.find(OdChar('\"')) != -1 || code.find(OdChar('\'')) != -1)
      return OdString(OD_T("%lu3"));
    else
      return OdString(OD_T("%lu2"));
  }
  if (code.find(OdChar('/')) != -1)
  {
    if (code.find(OdChar('\"')) != -1 || code.find(OdChar('\'')) != -1)
      return OdString(OD_T("%lu4"));
    else if (code.find(OdChar(' ')) != -1 )
      return OdString(OD_T("%lu5"));
  }
  return OdString(OD_T("")); // "%lu6"
}

OdResult OdExprEvaluator::evaluate(OdDbField* pField, int nContext, OdDbDatabase* pDb, OdFdFieldResult* pResult) const
{
  OdResult r = eOk;
  try
  {
    OdString code = pField->getFieldCode(
      OdDbField::FieldCodeFlag(OdDbField::kStripOptions | OdDbField::kForExpression | OdDbField::kEvaluatedChildren | OdDbField::kFieldCode));
    code.makeLower();
    AcExprEvalResult result;
    result.user_data = pField;
    result.heap = new std::list<OdFieldValue>();
    result.tablestack = new std::stack<OdDbObjectId>();
    result.rbheap = new std::list<OdArray<OdFieldValue> >();
    result.srcvalues = new std::list<OdValue>();
    result.database = pDb;
    odEvaluateExpr((const char*)code, result);
    std::list<OdValue>* src = (std::list<OdValue>*)result.srcvalues;

    if (result.success != 0 && result.result != 0)
    {
      pResult->setFieldValue((OdFieldValue*)result.result);
      pResult->setEvaluationStatus(OdDbField::kSuccess);

#if 1
      OdDbField::State state = pField->state();
      OdFieldValue* tagFld = (OdFieldValue*)result.result;

      if ( pField->getFormat().isEmpty() && ( (state & OdDbField::kHasFormattedString) == 0 ) &&
        ( (state & OdDbField::kEvaluated ) == 0 ) )
      {
        if (src->size() > 0)
        {
          OdString tagFormat;
          OdString srcFormat = src->begin()->getFormat();
          OdValue::UnitType unit = src->begin()->unitType();
          OdValue::DataType data = src->begin()->dataType();
          if (unit == OdValue::kAngle)
          {
            OdDbDatabase* pdb__ = ( pDb == NULL ) ? pField->database() : pDb;
            tagFld->setUnitType(OdValue::kUnitless);

            OdString str;
            if ( srcFormat.find(L"%au4") != -1 )
            {
              OdString frmStr = L"%au5%pr";
              OdString prec;
              prec.format(L"%1d", pdb__->getLUPREC());
              frmStr += prec;
              bool b = tagFld->format(frmStr, str, pdb__);
              if (b)
              {
                tagFld->reset();
                tagFld->set(str);
                tagFormat = frmStr; //  L"%lu6";
              }
            } else
            {
              OdString frmStr = L"%au5";
              bool b = tagFld->format(frmStr, str, pdb__);
              if (b)
              {
                tagFld->reset();
                tagFld->set(str);
                tagFormat = frmStr; //  L"%lu6";
              }
            }
            pField->setFormat(tagFormat);
          } else
          {
            OdInt32 idx;
            idx = srcFormat.find(L"%ps[");
            if (idx != -1)
            {
              tagFld->setUnitType(src->begin()->unitType());
              tagFormat = srcFormat;
            }
            else if (unit == OdValue::kAngle)
            {
              tagFld->setUnitType(OdValue::kUnitless);
            }
            else
            {
              idx = srcFormat.find(L"%lu");
              if (idx != -1)
              {
                tagFormat = srcFormat.mid(idx, 4);
              }
              else if ( data == OdValue::kString )
              {
                if (tagFld->dataType() == OdValue::kDouble)
                {
                  OdString srcStr;
                  if (src->begin()->get(srcStr))
                    tagFormat = getFormat(srcStr);
                }
              }
            }
            pField->setFormat(tagFormat);
          }
        }
        else
        {
          if ( tagFld->unitType() != OdValue::kAngle )
            pField->setFormat(getFormat(code));
        }
      }
#endif
    }
    else
    {
      pResult->setEvaluationStatus(OdDbField::kOtherError);
      r = eInvalidInput;
    }
    delete (std::list<OdFieldValue>*)result.heap;
    delete (std::stack<OdDbObjectId>*)result.tablestack;
    delete (std::list<OdArray<OdFieldValue> >*)result.rbheap;
    delete (std::list<OdValue>*)result.srcvalues;
  }
  catch (const OdError& e)
  {
    pResult->setEvaluationStatus(OdDbField::kSyntaxError, e.code(), e.description());
    return e.code();
  }

  return r;
}

void* acexprSaveToHeap(const OdFieldValue& rb, AcExprEvalResult* ptr)
{
  ((std::list<OdFieldValue>*)ptr->heap)->push_back(rb);
  return &((std::list<OdFieldValue>*)ptr->heap)->back();
}

void* acexprSaveSequenceToHeap(OdArray<OdFieldValue>& rb, AcExprEvalResult* ptr)
{
  ((std::list<OdArray<OdFieldValue> >*)ptr->rbheap)->push_back(rb);
  return &((std::list<OdArray<OdFieldValue> >*)ptr->rbheap)->back();
}

void* Vec(void* p1, void* p2, AcExprEvalResult* ptr)
{
  OdGePoint3d pp1, pp2;
  if (!((OdFieldValue*)p1)->get(pp1) || !((OdFieldValue*)p2)->get(pp2))
  {
    ptr->success = 0;
    return 0;
  }
  return acexprSaveToHeap(OdFieldValue((pp2 - pp1)), ptr);
}

void* UnitVec(void* p1, void* p2, AcExprEvalResult* ptr)
{
  OdGePoint3d pp1, pp2;
  if (!((OdFieldValue*)p1)->get(pp1) || !((OdFieldValue*)p2)->get(pp2))
  {
    ptr->success = 0;
    return 0;
  }
  OdGeVector3d vec(pp2 - pp1);
  OdGe::ErrorCondition ec = OdGe::kOk;
  vec.normalize(OdGeContext::gTol, ec);
  if (ec != OdGe::kOk)
  {
    ptr->success = 0;
    return 0;
  }
  return acexprSaveToHeap(OdFieldValue(vec), ptr);
}

void* CrossProduct(void* p1, void* p2, AcExprEvalResult* ptr)
{
  ptr->success = 0;
  OdGeVector3d pp1, pp2;
  if (!((OdFieldValue*)p1)->get(pp1) || !((OdFieldValue*)p2)->get(pp2))
    return 0;
  ptr->success = 1;
  return acexprSaveToHeap(OdFieldValue(pp1.crossProduct(pp2)), ptr);
}

void* Normal(void* p1, void* p2, AcExprEvalResult* ptr)
{
  ptr->success = 0;
  try {
    OdGePoint3d pp1, pp2;
    if (!((OdFieldValue*)p1)->get(pp1) || !((OdFieldValue*)p2)->get(pp2))
      return 0;
    OdGeVector3d vec(pp2 - pp1);
    vec.perpVector();
    ptr->success = 1;
    return acexprSaveToHeap(OdFieldValue(vec), ptr);
  } catch (const OdError&) {
    ptr->success = 0;
  }
  return 0;
}

void* Ill(void* p1, void* p2, void* p3, void* p4, AcExprEvalResult* ptr)
{
  ptr->success = 0;
  try {
    OdGePoint3d pp1, pp2, pp3, pp4;
    if (!((OdFieldValue*)p1)->get(pp1) || !((OdFieldValue*)p2)->get(pp2)
    || !((OdFieldValue*)p3)->get(pp3) || !((OdFieldValue*)p4)->get(pp4))
      return 0;
    OdGeLine3d l1(pp1, pp2);
    OdGeLine3d l2(pp3, pp4);
    OdGePoint3d p;
    if (!l1.intersectWith(l2, p))
      return 0;
    ptr->success = 1;
    return acexprSaveToHeap(OdFieldValue(p), ptr);
  } catch (const OdError&) {
  }
  return 0;
}

// offset p1 towards p2 at distance p3
void* Pld(void* p1, void* p2, void* p3, AcExprEvalResult* ptr)
{
  try {
    OdGePoint3d pp1, pp2;
    if (!((OdFieldValue*)p1)->get(pp1) || !((OdFieldValue*)p2)->get(pp2))
    {
      ptr->success = 0;
      return 0;
    }
    double d = acexprConvertToDouble(p3, ptr);
    if (!ptr->success)
      return 0;
    return acexprSaveToHeap(OdFieldValue(pp1 + ((pp2 - pp1).normal() * d)), ptr);
  }
  catch (const OdError&) {
    ptr->success = 0;
  }
  return 0;
}

// calculate a point on a (p1,p2) segment with parameter p3, may be outside the segment
void* Plt(void* p1, void* p2, void* p3, AcExprEvalResult* ptr)
{
  try {
    OdGePoint3d pp1, pp2;
    if (!((OdFieldValue*)p1)->get(pp1) || !((OdFieldValue*)p2)->get(pp2))
    {
      ptr->success = 0;
      return 0;
    }
    double d = acexprConvertToDouble(p3, ptr);
    if (!ptr->success)
      return 0;
    return acexprSaveToHeap(OdFieldValue(pp1 + ((pp2 - pp1) * d)), ptr);
  }
  catch (const OdError&) {
    ptr->success = 0;
  }
  return 0;
}
// distance between point p and the line passing through points p1 and p2
void* Dpl(void* p1, void* p2, void* p3, AcExprEvalResult* ptr)
{
  try {
    OdGePoint3d pp1, pp2, pp3;
    if (!((OdFieldValue*)p1)->get(pp1) || !((OdFieldValue*)p2)->get(pp2) || !((OdFieldValue*)p3)->get(pp3))
    {
      ptr->success = 0;
      return 0;
    }
    return acexprSaveToHeap(OdFieldValue(OdGeLine3d(pp2, pp3).distanceTo(pp1)), ptr);
  }
  catch (const OdError&) {
    ptr->success = 0;
  }
  return 0;
}

// distance from a point p1 to a plane defined by three points (p2,p3,p4)
void* Dpp(void* p1, void* p2, void* p3, void* p4, AcExprEvalResult* ptr)
{
  try {
    OdGePoint3d p, origin, pu, pv;
    if (!((OdFieldValue*)p1)->get(p) || !((OdFieldValue*)p2)->get(origin)
      || !((OdFieldValue*)p3)->get(pu) || !((OdFieldValue*)p4)->get(pv))
    {
      ptr->success = 0;
      return 0;
    }
    OdGeVector3d u = (pu - origin).normal();
    OdGeVector3d v = pv - origin;
    v -= u * u.dotProduct(v);
    v.normalize();
    return acexprSaveToHeap(OdFieldValue(OdGePlane(origin, u, v).distanceTo(p)), ptr);
  }
  catch (const OdError&) {
    ptr->success = 0;
  }
  return 0;
}

// intersection point between a line (p1,p2) and a plane passing through three points (p3,p4,p5)
void* Ilp(void* p1, void* p2, void* p3, void* p4, void* p5, AcExprEvalResult* ptr)
{
  try {
    OdGePoint3d l1, l2, origin, pu, pv;
    if (!((OdFieldValue*)p1)->get(l1) || !((OdFieldValue*)p2)->get(l2)
      || !((OdFieldValue*)p3)->get(origin) || !((OdFieldValue*)p4)->get(pu) || !((OdFieldValue*)p5)->get(pv))
    {
      ptr->success = 0;
      return 0;
    }
    OdGeVector3d u = (pu - origin).normal();
    OdGeVector3d v = pv - origin;
    v -= u * u.dotProduct(v);
    v.normalize();
    OdGePoint3d res;
    if (OdGePlane(origin, u, v).intersectWith(OdGeLine3d(l1, l2), res))
      return acexprSaveToHeap(res, ptr);
  }
  catch (const OdError&) {
    ptr->success = 0;
  }
  return 0;
}

void* Rot3(void* p1, void* p2, void* p3, AcExprEvalResult* ptr)
{
  OdGePoint3d p, origin;
  if (!((OdFieldValue*)p1)->get(p) || !((OdFieldValue*)p2)->get(origin))
  {
    ptr->success = 0;
    return 0;
  }
  double ang = acexprConvertAngle(acexprConvertToDouble(p3, ptr), ptr);
  if (!ptr->success)
    return 0;
  return acexprSaveToHeap(p.rotateBy(ang, OdGeVector3d::kZAxis, origin), ptr);
}

void* Rot4(void* p1, void* p2, void* p3, void* p4, AcExprEvalResult* ptr)
{
  try {
    OdGePoint3d p, axp1, axp2;
    if (!((OdFieldValue*)p1)->get(p) || !((OdFieldValue*)p2)->get(axp1)
      || !((OdFieldValue*)p3)->get(axp2))
    {
      ptr->success = 0;
      return 0;
    }
    double ang = acexprConvertAngle(acexprConvertToDouble(p4, ptr), ptr);
    if (!ptr->success)
      return 0;
    return acexprSaveToHeap(p.rotateBy(ang, axp2 - axp1, axp1), ptr);
  }
  catch (const OdError&) {
    ptr->success = 0;
  }
  return 0;
}

void* Add(void* p1, void* p2, AcExprEvalResult* ptr)
{
  OdFieldValue *rb1 = (OdFieldValue*)p1, *rb2 = (OdFieldValue*)p2;
  if (rb1->dataType() == OdValue::k3dPoint) // both arguments must be points if the first is a point
  {
    if (rb2->dataType() == OdValue::k3dPoint)
    {
      OdGePoint3d pp1, pp2;
      ((OdFieldValue*)p1)->get(pp1);
      ((OdFieldValue*)p2)->get(pp2);
      return acexprSaveToHeap(OdFieldValue(pp2 + pp1.asVector()), ptr);
    }
  }
  // if both arguments are of integral type - preserve type (intermediate integers are all int64)
  else if (rb1->dataType() == OdValue::kLong && rb2->dataType() == OdValue::kLong)
  {
    OdFieldValue rb(((OdInt32)*rb1) + ((OdInt32)*rb2));
    if (rb1->unitType() == rb2->unitType())
      rb.setUnitType(rb1->unitType());
    return acexprSaveToHeap(rb, ptr);
  }
  else
  {
    double arg1, arg2;
    switch(rb1->dataType())
    {
      case OdValue::kLong:
        arg1 = (OdInt32)*rb1;
        break;
      case OdValue::kDouble:
        arg1 = (double)*rb1;
        break;
      default:
        ptr->success = 0;
        return 0;
    }
    switch(rb2->dataType())
    {
    case OdValue::kLong:
      arg2 = (OdInt32)*rb2;
      break;
    case OdValue::kDouble:
      arg2 = (double)*rb2;
      break;
    default:
      ptr->success = 0;
      return 0;
    }
    OdFieldValue rb(arg2 + arg1);
    if (rb1->dataType() == rb2->dataType() && rb1->unitType() == rb2->unitType())
      rb.setUnitType(rb1->unitType());
    return acexprSaveToHeap(rb, ptr);
  }
  ptr->success = 0;
  return 0;
}
void* Mul(void* p1, void* p2, AcExprEvalResult* ptr)
{
  OdFieldValue *rb1 = (OdFieldValue*)p1, *rb2 = (OdFieldValue*)p2;
  // both are integers - result is an integer
  if (rb1->dataType() == OdValue::kLong && rb2->dataType() == OdValue::kLong)
  {
    OdFieldValue rb(((OdInt32)*rb1) * ((OdInt32)*rb2));
    if (rb1->unitType() == rb2->unitType())
      rb.setUnitType(rb1->unitType());
    return acexprSaveToHeap(rb, ptr);
  }
  // both are points/vectors - result is a scalar product
  else if (rb1->dataType() == OdValue::k3dPoint && rb2->dataType() == OdValue::k3dPoint)
  {
    OdGeVector3d pp1, pp2;
    ((OdFieldValue*)p1)->get(pp1.x, pp1.y, pp1.z);
    ((OdFieldValue*)p2)->get(pp2.x, pp2.y, pp2.z);
    return acexprSaveToHeap(OdFieldValue(pp1.dotProduct(pp2)), ptr);
  }
  // one of the arguments is a point - another should be scalar
  else if (rb1->dataType() == OdValue::k3dPoint || rb2->dataType() == OdValue::k3dPoint)
  {
    if (rb2->dataType() == OdValue::k3dPoint)
      std::swap(rb1, rb2); // mul is commutative
    OdGePoint3d pp1; rb1->get(pp1);
    double arg;
    switch (rb2->dataType())
    {
    case OdValue::kDouble:
      arg = (double)*rb2;
      break;
    case OdValue::kLong:
      arg = (OdInt32)*rb2;
      break;
    default:
      ptr->success = 0;
      return 0;
    }
    OdFieldValue res(pp1 * arg);
    res.setUnitType(rb1->unitType());
    return acexprSaveToHeap(res, ptr);
  }
  else // both are scalars
  {
    double arg1, arg2;
    switch (rb1->dataType())
    {
    case OdValue::kDouble:
      arg1 = (double)*rb1;
      break;
    case OdValue::kLong:
      arg1 = (double)(OdInt32)*rb1;
      break;
    default:
      ptr->success = 0;
      return 0;
    }
    switch (rb2->dataType())
    {
    case OdValue::kDouble:
      arg2 = (double)*rb2;
      break;
    case OdValue::kLong:
      arg2 = (double)(OdInt32)*rb2;
      break;
    default:
      ptr->success = 0;
      return 0;
    }
    OdFieldValue rb(arg1 * arg2);
    if (rb1->dataType() == rb2->dataType() && rb1->unitType() == rb2->unitType())
      rb.setUnitType(rb1->unitType());
    return acexprSaveToHeap(rb, ptr);
  }
  ptr->success = 0;
  return 0;
}

void* Div(void* p1, void* p2, AcExprEvalResult* ptr)
{
  OdFieldValue *rb1 = (OdFieldValue*)p1, *rb2 = (OdFieldValue*)p2;
  if (rb1->dataType() == OdValue::k3dPoint)
  {
    OdGeVector3d pp1;
    rb1->get(pp1.x, pp1.y, pp1.z);
    double arg;
    switch (rb2->dataType())
    {
    case OdValue::kDouble:
      arg = (double)*rb2;
      break;
    case OdValue::kLong:
      arg = (double)(OdInt32)*rb2;
      break;
    default:
      ptr->success = 0;
      return 0;
    }
    if (!OdZero(arg))
    {
      OdFieldValue rb(pp1 * (1 / arg));
      rb.setUnitType(rb1->unitType());
      return acexprSaveToHeap(rb, ptr);
    }
  }
  else
  {
    double arg1, arg2;
    switch (rb1->dataType())
    {
    case OdValue::kDouble:
      arg1 = (double)*rb1;
      break;
    case OdValue::kLong:
      arg1 = (double)(OdInt32)*rb1;
      break;
    default:
      ptr->success = 0;
      return 0;
    }
    switch (rb2->dataType())
    {
    case OdValue::kDouble:
      arg2 = (double)*rb2;
      break;
    case OdValue::kLong:
      arg2 = (double)(OdInt32)*rb2;
      break;
    default:
      ptr->success = 0;
      return 0;
    }
    if (!OdZero(arg2))
    {
      OdFieldValue rb(arg1 / arg2);
      if (rb1->dataType() == OdValue::kDouble)
        rb.setUnitType(rb1->unitType());
      return acexprSaveToHeap(rb, ptr);
    }
  }
  ptr->success = 0;
  return 0;
}

void* Pow(void* x, void* y, AcExprEvalResult* ptr)
{
  OdFieldValue *rb1 = (OdFieldValue*)x, *rb2 = (OdFieldValue*)y;
  double arg1, arg2;
  switch (rb1->dataType())
  {
  case OdValue::kDouble:
    arg1 = (double)*rb1;
    break;
  case OdValue::kLong:
    arg1 = (double)(OdInt32)*rb1;
    break;
  default:
    ptr->success = 0;
    return 0;
  }
  switch (rb2->dataType())
  {
  case OdValue::kDouble:
    arg2 = (double)*rb2;
    break;
  case OdValue::kLong:
    arg2 = (double)(OdInt32)*rb2;
    break;
  default:
    ptr->success = 0;
    return 0;
  }
  OdFieldValue rb(pow(arg1, arg2));
  if (rb1->dataType() == OdValue::kDouble)
    rb.setUnitType(rb1->unitType());
  return acexprSaveToHeap(rb, ptr);
}

void* Neg(void* p1, AcExprEvalResult* ptr)
{
  OdFieldValue *rb1 = (OdFieldValue*)p1, res;
  res.setUnitType(rb1->unitType());
  switch(rb1->dataType())
  {
  case OdValue::k3dPoint:
  {
    OdGeVector3d v;
    rb1->get(v);
    res.set(-v);
    return acexprSaveToHeap(res, ptr);
  }
    break;
  case OdValue::kLong:
    res.set(-(OdInt32)*rb1);
    return acexprSaveToHeap(res, ptr);
    break;
  case OdValue::kDouble:
    res.set(-(double)*rb1);
    return acexprSaveToHeap(res, ptr);
    break;
  }
  ptr->success = 0;
  return 0;
}
void* Sub(void* p1, void* p2, AcExprEvalResult* ptr)
{
  OdFieldValue *rb1 = (OdFieldValue*)p1, *rb2 = (OdFieldValue*)p2;
  if (rb1->dataType() == OdValue::k3dPoint) // both arguments must be points if the first is a point
  {
    OdGePoint3d pp1, pp2;
    rb1->get(pp1);
    rb2->get(pp2);
    OdFieldValue res(pp1 - pp2.asVector());
    res.setUnitType(rb1->unitType());
    return acexprSaveToHeap(res, ptr);
  }
  // if both arguments are of integral type - preserve type
  else if (rb1->dataType() == OdValue::kLong && rb2->dataType() == OdValue::kLong)
  {
    OdFieldValue res((OdInt32)*rb1 - (OdInt32)*rb2);
    if (rb1->unitType() == rb2->unitType())
      res.setUnitType(rb1->unitType());
    return acexprSaveToHeap(res, ptr);
  }
  else
  {
    double arg1, arg2;
    switch (rb1->dataType())
    {
    case OdValue::kDouble:
      arg1 = (double)*rb1;
      break;
    case OdValue::kLong:
      arg1 = (double)(OdInt32)*rb1;
      break;
    default:
      ptr->success = 0;
      return 0;
    }
    switch (rb2->dataType())
    {
    case OdValue::kDouble:
      arg2 = (double)*rb2;
      break;
    case OdValue::kLong:
      arg2 = (double)(OdInt32)*rb2;
      break;
    default:
      ptr->success = 0;
      return 0;
    }
    OdFieldValue res(arg1 - arg2);
    if (rb1->unitType() == rb2->unitType())
      res.setUnitType(rb1->unitType());
    return acexprSaveToHeap(res, ptr);
  }
  ptr->success = 0;
  return 0;
}
void* Distance(void* p1, void* p2, AcExprEvalResult* ptr)
{
  OdFieldValue *rb1 = (OdFieldValue*)p1, *rb2 = (OdFieldValue*)p2;
  OdGePoint3d pp1, pp2;
  if (rb1->get(pp1) && rb2->get(pp2))
  {
    OdFieldValue res(pp1.distanceTo(pp2));
    res.setUnitType(rb1->unitType());
    return acexprSaveToHeap(res, ptr);
  }
  ptr->success = 0;
  return 0;
}

void* W2U(void* pp, AcExprEvalResult* ptr)
{
  OdGePoint3d p;
  OdDbDatabase* pDb = database(ptr);
  if (pDb && ((OdFieldValue*)pp)->get(p))
  {
    OdGeMatrix3d ucsToWcsMatrix;
    ucsToWcsMatrix.setCoordSystem(pDb->getUCSORG(), pDb->getUCSXDIR(), pDb->getUCSYDIR(), pDb->getUCSXDIR().crossProduct(pDb->getUCSYDIR()));
    p.transformBy(ucsToWcsMatrix.invert());
    return acexprSaveToHeap(p, ptr);
  }
  else
  {
    ptr->success = 0;
    return 0;
  }
}

void* U2W(void* pp, AcExprEvalResult* ptr)
{
  OdGePoint3d p;
  OdDbDatabase* pDb = database(ptr);
  if (pDb && ((OdFieldValue*)pp)->get(p))
  {
    OdGeMatrix3d ucsToWcsMatrix;
    ucsToWcsMatrix.setCoordSystem(pDb->getUCSORG(), pDb->getUCSXDIR(), pDb->getUCSYDIR(), pDb->getUCSXDIR().crossProduct(pDb->getUCSYDIR()));
    p.transformBy(ucsToWcsMatrix);
    return acexprSaveToHeap(p, ptr);
  }
  else
  {
    ptr->success = 0;
    return 0;
  }
}

void* Xzof(void* pp, AcExprEvalResult* ptr)
{
  OdGePoint3d p;
  if (((OdFieldValue*)pp)->get(p))
  {
    return acexprSaveToHeap(OdGePoint3d(p.x, 0.0, p.z), ptr);
  }
  else
  {
    ptr->success = 0;
    return 0;
  }
}
void* Xyof(void* pp, AcExprEvalResult* ptr)
{
  OdGePoint3d p;
  if (((OdFieldValue*)pp)->get(p))
  {
    return acexprSaveToHeap(OdGePoint3d(p.x, p.y, 0.0), ptr);
  }
  else
  {
    ptr->success = 0;
    return 0;
  }
}
void* Yzof(void* pp, AcExprEvalResult* ptr)
{
  OdGePoint3d p;
  if (((OdFieldValue*)pp)->get(p))
  {
    return acexprSaveToHeap(OdGePoint3d(0.0, p.y, p.z), ptr);
  }
  else
  {
    ptr->success = 0;
    return 0;
  }
}
void* Xof(void* pp, AcExprEvalResult* ptr)
{
  OdGePoint3d p;
  if (((OdFieldValue*)pp)->get(p))
  {
    return acexprSaveToHeap(OdGePoint3d(p.x, 0.0, 0.0), ptr);
  }
  else
  {
    ptr->success = 0;
    return 0;
  }
}
void* Yof(void* pp, AcExprEvalResult* ptr)
{
  OdGePoint3d p;
  if (((OdFieldValue*)pp)->get(p))
  {
    return acexprSaveToHeap(OdGePoint3d(0.0, p.y, 0.0), ptr);
  }
  else
  {
    ptr->success = 0;
    return 0;
  }
}
void* Zof(void* pp, AcExprEvalResult* ptr)
{
  OdGePoint3d p;
  if (((OdFieldValue*)pp)->get(p))
  {
    return acexprSaveToHeap(OdGePoint3d(0.0, 0.0, p.z), ptr);
  }
  else
  {
    ptr->success = 0;
    return 0;
  }
}
void* RXof(void* pp, AcExprEvalResult* ptr)
{
  OdGePoint3d p;
  if (((OdFieldValue*)pp)->get(p))
  {
    return acexprSaveToHeap(p.x, ptr);
  }
  else
  {
    ptr->success = 0;
    return 0;
  }
}
void* RYof(void* pp, AcExprEvalResult* ptr)
{
  OdGePoint3d p;
  if (((OdFieldValue*)pp)->get(p))
  {
    return acexprSaveToHeap(p.y, ptr);
  }
  else
  {
    ptr->success = 0;
    return 0;
  }
}
void* RZof(void* pp, AcExprEvalResult* ptr)
{
  OdGePoint3d p;
  if (((OdFieldValue*)pp)->get(p))
  {
    return acexprSaveToHeap(p.z, ptr);
  }
  else
  {
    ptr->success = 0;
    return 0;
  }
}

void* Functor(void* fun, void* param, AcExprEvalResult* ptr)
{
  OdString str = *(OdFieldValue*)fun;
  if (str == L"sin")
    return Sin(param, ptr);
  else if (str == L"cos")
    return Cos(param, ptr);
  else if (str == L"tang")
    return Tan(param, ptr);
  else if (str == L"asin")
    return Asin(param, ptr);
  else if (str == L"acos")
    return Acos(param, ptr);
  else if (str == L"atan")
    return Atan(param, ptr);
  else if (str == L"log")
    return Log(param, ptr);
  else if (str == L"ln")
    return Ln(param, ptr);
  else if (str == L"exp")
    return Exp(param, ptr);
  else if (str == L"sqr")
    return Sqr(param, ptr);
  else if (str == L"sqrt")
    return Sqrt(param, ptr);
  else if (str == L"r2d")
    return R2d(param, ptr);
  else if (str == L"d2r")
    return D2r(param, ptr);
  else if (str == L"abs")
    return Abs(param, ptr);
  else if (str == L"round")
    return Round(param, ptr);
  else if (str == L"trunc")
    return Trunc(param, ptr);
  else if (str == L"sum")
    return param;
  else if (str == L"average")
    return acexprCreateDouble2(acexprConvertToDouble(param, ptr), ptr);
  else if (str == L"w2u")
    return W2U(param, ptr);
  else if (str == L"u2w")
    return U2W(param, ptr);
  else if (str == L"xzof")
    return Xzof(param, ptr);
  else if (str == L"xyof")
    return Xyof(param, ptr);
  else if (str == L"yzof")
    return Yzof(param, ptr);
  else if (str == L"xof")
    return Xof(param, ptr);
  else if (str == L"yof")
    return Yof(param, ptr);
  else if (str == L"zof")
    return Zof(param, ptr);
  else if (str == L"rxof")
    return RXof(param, ptr);
  else if (str == L"ryof")
    return RYof(param, ptr);
  else if (str == L"rzof")
    return RZof(param, ptr);
  OdFdFieldEnginePEPtr pFieldEnginePE = OdFdFieldEngine::desc()->getX(OdFdFieldEnginePE::desc());
  OdFieldValue v;
  if (!pFieldEnginePE.isNull())
    pFieldEnginePE->getLispVariable(str, v);
  if (v.isValid())
    return acexprSaveToHeap(v, ptr);
  ptr->success = 0;
  return 0;
}

static bool getRowCol(const OdFieldValue* param, OdUInt32& row, OdUInt32& col)
{
  if (param->dataType() != OdValue::kString)
    return false;
  OdString str = (OdString)*param;
  str.remove('$');
  const char* index = (const char*)str;
  row = 0;
  col = 0;
  while ((*index) <= 'z' && (*index) >= 'a')
  {
    col = (col*('z' - 'a' + 1)) + (*index) - 'a' + 1;
    ++index;
  }
  if (col == 0)
    return false;
  --col; // indices are zero based
  while ((*index) <= '9' && (*index) >= '0')
  {
    row = row * 10 + (*index) - '0';
    ++index;
  }
  if (row == 0)
    return false;
  --row; // indices are zero based
  return true;
}

void* getTableCell(OdDbTable* table, const OdFieldValue* cell, AcExprEvalResult* ptr, OdInt32* status);

static OdFieldValue evalTableSum(OdDbTable* table, OdArray<OdFieldValue>* seq, AcExprEvalResult* ptr)
{
  OdFieldValue res((OdInt32)0);
  int validCount = 0;
  for (unsigned i = 0; i < seq->size(); ++i)
  {
    const OdFieldValue* rb = &seq->getAt(i);
    OdInt32 status = 1;
    if (rb->dataType() == OdValue::kString)
      rb = (const OdFieldValue*)getTableCell(table, rb, ptr, &status);
    if (!rb || !rb->isValid() || status == 0)
    {
      if (status == 0)
      {
        validCount = 0;
        ptr->success = 0;
        break;
      }
      ptr->success = 1;
      continue;
    }
    ++validCount;
    switch (rb->dataType())
    {
    case OdValue::kDouble:
      if (res.dataType() == OdValue::kLong)
        res = OdFieldValue((double)*rb + (OdInt32)res);
      else
        res.set((double)*rb + (double)res);
      if (validCount == 1)
        res.setUnitType(rb->unitType());
      else if (rb->unitType() != res.unitType())
        res.setUnitType(OdValue::kUnitless);
      break;
    case OdValue::kLong:
      if (res.dataType() == OdValue::kLong)
        res.set((OdInt32)*rb + (OdInt32)res);
      else
        res = OdFieldValue((double)res + (OdInt32)*rb);
      if (validCount == 1)
        res.setUnitType(rb->unitType());
      else if (rb->unitType() != res.unitType())
        res.setUnitType(OdValue::kUnitless);
      break;
    }
  }
  if (validCount == 0)
    ptr->success = 0;
return res;
}

static OdInt32 evalTableCount(OdDbTable* table, OdArray<OdFieldValue>* seq, AcExprEvalResult* ptr)
{
  OdInt32 res = 0;
  for (unsigned i = 0; i < seq->size(); ++i)
  {
    const OdFieldValue* rb = &seq->getAt(i);
    OdInt32 status = 1;
    if (rb->dataType() == OdValue::kString)
      rb = (const OdFieldValue*)getTableCell(table, rb, ptr, &status);
    if (status == 0)
    {
      ptr->success = 0;
      return 0;
    }
    if (rb && rb->isValid() && (rb->dataType() == OdValue::kDouble || rb->dataType() == OdValue::kLong))
      ++res;
  }
  ptr->success = 1;
  return res;
}

static bool evalTableAvg(OdDbTable* table, OdArray<OdFieldValue>* seq, double& res, AcExprEvalResult* ptr)
{
  res = 0;
  int count = 0;
  for (unsigned int i = 0; i < seq->size(); ++i)
  {
    const OdFieldValue* rb = &seq->getAt(i);
    OdInt32 status = 1;
    if (rb->dataType() == OdValue::kString)
      rb = (const OdFieldValue*)getTableCell(table, rb, ptr, &status);
    if (status == 0)
    {
      count = 0;
      break;
    }
    if (!rb || !rb->isValid())
      continue;
    switch (rb->dataType())
    {
    case OdValue::kDouble:
      res += (double)*rb;
      ++count;
      break;
    case OdValue::kLong:
      res += (OdInt32)*rb;
      ++count;
      break;
    }
  }
  if (count != 0)
  {
    res /= count;
    ptr->success = 1;
    return true;
  }
  else
    return false;
}

static void* tableEvalExplicit(OdDbTable* table, void* fun, OdArray<OdFieldValue>* range, AcExprEvalResult* ptr)
{
  if (((OdFieldValue*)fun)->dataType() != OdValue::kString)
  {
    ptr->success = 0;
    return 0;
  }
  OdString str = *((OdFieldValue*)fun);
  OdFieldValue rb;
  if (str == L"sum")
  {
    rb = evalTableSum(table, range, ptr);
  }
  else if (str == L"average")
  {
    double res;
    if (evalTableAvg(table, range, res, ptr))
      rb = OdFieldValue(res);
  }
  else if (str == L"count")
  {
    rb = OdFieldValue(evalTableCount(table, range, ptr));
  }
  if (rb.isValid())
    return acexprSaveToHeap(rb, ptr);
  else
    ptr->success = 0;
  return 0;
}

OdDbObjectId getObjectId(void* id, AcExprEvalResult* ptr)
{
  if (((OdFieldValue*)id)->dataType() == OdValue::kObjectId)
  {
    return (OdDbObjectId)*(OdFieldValue*)id;
  }
  else
  {
    ptr->success = 0;
    return OdDbObjectId::kNull;
  }
}

void* TableEval(void* id, void* fun, void* seq, AcExprEvalResult* ptr)
{
  OdDbObjectId objId;
  if (!((OdFieldValue*)id)->get(objId))
  {
    ptr->success = 0;
    return 0;
  }
    
  OdDbTablePtr table = objId.openObject();
  return tableEvalExplicit(table, fun, (OdArray<OdFieldValue>*)seq, ptr);
}
static OdDbTablePtr getOwnerTable(OdDbObjectId ownerId)
{
  if (ownerId.isNull())
    return OdDbTablePtr();
  OdDbObjectPtr obj = ownerId.openObject();
  if (obj.isNull())
    return OdDbTablePtr();
  else if (obj->isKindOf(OdDbBlockTableRecord::desc()))
    return OdDbTablePtr();
  else if (obj->isKindOf(OdDbTable::desc()))
    return OdDbTablePtr(obj);
  else
    return getOwnerTable(obj->ownerId());
}

static OdDbBlockTableRecordPtr getOwnerBlock(OdDbObjectId ownerId)
{
  if (ownerId.isNull())
    return OdDbBlockTableRecordPtr();
  OdDbObjectPtr obj = ownerId.openObject();
  if (obj.isNull())
    return OdDbBlockTableRecordPtr();
  else if (obj->isKindOf(OdDbBlockTableRecord::desc()))
    return OdDbBlockTableRecordPtr(obj);
  else 
    return getOwnerBlock(obj->ownerId());
}
static OdDbTablePtr getImplicitTable(AcExprEvalResult* ptr)
{
  if (!((std::stack<OdDbObjectId>*)ptr->tablestack)->empty())
    return OdDbTablePtr(((std::stack<OdDbObjectId>*)ptr->tablestack)->top().openObject());
  OdFdFieldEnginePEPtr pFieldEnginePE = OdFdFieldEngine::desc()->getX(OdFdFieldEnginePE::desc());
  OdDbObjectId tableId = !pFieldEnginePE.isNull() ?
    pFieldEnginePE->getOwnerTable((OdDbField*)ptr->user_data) : OdDbObjectId::kNull;
  OdDbTablePtr table = !tableId.isNull() ?
    OdDbTable::cast(tableId.openObject()) : getOwnerTable(((OdDbField*)ptr->user_data)->ownerId());
  if (!table.isNull())
    return table;
  OdDbBlockTableRecordPtr block = getOwnerBlock(((OdDbField*)ptr->user_data)->ownerId());
  if (block.isNull())
    return table;
  OdString nm = block->getName();
  if ( nm.getAt(0) == '*' && nm.getAt(1) == 'T' )
  {
    OdDbObjectIdArray ia;
    block->getBlockReferenceIds(ia);
    if (ia.isEmpty())
      return table;
    table = ia[0].openObject();
  }
  return table;
}
void* ImplicitTableEval(void* fun, void* range, AcExprEvalResult* ptr)
{
  OdDbTablePtr table = getImplicitTable(ptr);
  return tableEvalExplicit(table, fun, (OdArray<OdFieldValue>*)range, ptr);
}
// number is [[whitespace?] [sign?] [digits] [whitespace?]]
static bool isNumber(const OdChar* s)
{
  while (*s && Od_iswspace(*s))
    ++s;
  while (*s && (*s == '-' || *s == '+'))
    ++s;
  if (!*s)
    return false;
  while (*s && Od_iswdigit(*s))
    ++s;
  while (*s && Od_iswspace(*s))
    ++s;
  return *s == 0;
}


OdFieldValue acexprValueToResbuf(const OdValue& v)
{
  OdFieldValue res;
  res.setUnitType(v.unitType());
  switch (v.dataType())
  {
  case OdValue::kDouble:
    res.set((double)v);
    return res;
  case OdValue::kLong:
    res.set((OdInt32)v);
    return res;
  case OdValue::kDate:
    res.set((OdInt64)v);
    return res;
  case OdValue::kString:
    {
      OdString s = (OdString)v;
      if (isNumber(s)) 
      {
        res.set((OdInt32)odStrToD(OdCharMapper::convertAlphaNumJapanese(s)));
        return res;
      } else if ( s.trimLeft().isEmpty() )
      { 
        res.set((OdInt32)0);
        return res;
      }
      try {
        res.set(OdUnitsFormatter::unformatL(s));
        return res;
      }
      catch (const OdError&){
        return OdFieldValue();
      }
    }
  case OdValue::k3dPoint:
  {
    OdGePoint3d p;
    v.get(p.x, p.y, p.z);
    res.set(p);
    return res;
  }
  default:
    return OdFieldValue();
  }
}

void* acexprCreateTableRange(void* cell1, void* cell2, AcExprEvalResult* ptr)
{
  OdUInt32 row1, col1, row2, col2;
  ptr->success = false;
  if (!getRowCol((OdFieldValue*)cell1, row1, col1) || !getRowCol((OdFieldValue*)cell2, row2, col2))
    return 0;
  OdArray<OdFieldValue> res;
  if (row1 > row2)
    std::swap(row1, row2);
  if (col1 > col2)
    std::swap(col1, col2);
  for (OdUInt32 row = row1; row <= row2; ++row)
  {
    for (OdUInt32 col = col1; col <= col2; ++col)
    {
      OdString s;
      const int N = 'z' - 'a' + 1;
      int c = col;
      for (;;)
      {
        OdString tmp;
        tmp = (OdChar)('a' + (c % N));
        s = tmp + s;
        c /= N;
        if (c == 0)
          break;
        else --c;
      }
      
      char buf[256] = {0};
      sprintf(buf, "%d", row+1);
      s += buf;
      res.append(OdFieldValue(s));
    }
  }
  ptr->success = true;
  return acexprSaveSequenceToHeap(res, ptr);
}

void* getTableCell(OdDbTable* table, const OdFieldValue* cell, AcExprEvalResult* ptr, OdInt32* status)
{
  OdUInt32 row, col;
  OdFieldValue res;
  if (status)
    *status = 1;
  try
  {
    if (table && getRowCol(cell, row, col))
    {
      for (OdInt32 i = 0; i < table->numContents(row, col) && !res.isValid(); ++i)
      {
        OdValue v = table->value(row, col, i, OdValue::FormatOption(OdValue::kIgnoreMtextFormat | OdValue::kForExpression));
        if (v.isValid())
        {
          res = acexprValueToResbuf(v);
          ((std::list<OdValue>*)ptr->srcvalues)->push_back(v);
        }
      }
    }
  }
  catch (const OdError& )
  {
    if (status)
      *status = 0;
    ptr->success = 0;
  }
  if (res.isValid())
    return acexprSaveToHeap(res, ptr);
  else
    ptr->success = 0;
  return 0;
}

void* acexprCreateTableSequence(void* val1, void* val2, AcExprEvalResult* ptr)
{
  OdArray<OdFieldValue>* seq1 = ((OdArray<OdFieldValue>*)val1);
  OdArray<OdFieldValue>* seq2 = ((OdArray<OdFieldValue>*)val2);
  seq1->append(*seq2);
  return seq1;
}

void* ImplicitTableCell(void* cell, AcExprEvalResult* ptr)
{
  OdDbTablePtr table = getImplicitTable(ptr);
  OdInt32 status = 1;
  void* res = getTableCell(table, (const OdFieldValue*)cell, ptr, &status);
  if (ptr->success)
    return res;
  OdFdFieldEnginePEPtr pFieldEnginePE = OdFdFieldEngine::desc()->getX(OdFdFieldEnginePE::desc());
  OdString str;
  if (!pFieldEnginePE.isNull() && ((const OdFieldValue*)cell)->get(str))
  {
    OdFieldValue v;
    pFieldEnginePE->getLispVariable(str, v);
    if (v.isValid())
    {
      ptr->success = 1;
      return acexprSaveToHeap(v, ptr);
    }
  }
  return 0;
}

void* TableCell(void* id, void* cell, AcExprEvalResult* ptr)
{
  OdDbObjectId objId(getObjectId(id, ptr));
  if (!ptr->success)
    return 0;
  OdDbTablePtr table = OdDbTable::cast(objId.openObject());
  OdInt32 status = 1;
  return getTableCell(table, (const OdFieldValue*)cell, ptr, &status);
}

bool oddbGetObjectPropertyRx(OdDbObjectId id, const OdString& propName, OdFieldValue& res);

void* ObjectEval(void* id, void* fun, AcExprEvalResult* ptr)
{
  OdDbObjectId objId(getObjectId(id, ptr));
  if (!ptr->success)
    return 0;
  if (((OdFieldValue*)fun)->dataType() != OdValue::kString)
  {
    ptr->success = 0;
    return 0;
  }
  OdString str = *(OdFieldValue*)fun;
  OdFieldValue fv;
  if (oddbGetObjectPropertyRx(objId, str, fv))
    return acexprSaveToHeap(fv, ptr);
  else
    ptr->success = 0;
  return 0;
}

OdGeVector3d convertCoords(void* x, void* y, void* z, AcExprEvalResult* ptr)
{
  OdGeVector3d p;
  switch(((OdFieldValue*)x)->dataType())
  {
  case OdValue::kDouble:
    p.x = *(OdFieldValue*)x;
    break;
  case OdValue::kLong:
    p.x = (OdInt32)*((OdFieldValue*)x);
    break;
  default:
    ptr->success = 0;
    break;
  }
  switch(((OdFieldValue*)y)->dataType())
  {
  case OdValue::kDouble:
    p.y = *(OdFieldValue*)y;
    break;
  case OdValue::kLong:
    p.y = (OdInt32)*((OdFieldValue*)y);
    break;
  default:
    ptr->success = 0;
    break;
  }
  switch (((OdFieldValue*)z)->dataType())
  {
  case OdValue::kDouble:
    p.z = *((OdFieldValue*)z);
    break;
  case OdValue::kLong:
    p.z = (OdInt32)*((OdFieldValue*)z);
    break;
  default:
    ptr->success = 0;
    break;
  }
  return p;
}

#define RELATIVE_COORD 1
#define WCS_COORD 2
static void applyVectorFlags(void* rf, AcExprEvalResult* ptr, OdGeVector3d &p)
{
  OdInt32 relativeFlags = 0;
  ((OdFieldValue*)rf)->get(relativeFlags);
  /* // relative flags are useless in ordinary fields, so currently we just ignore them
  if (relativeFlags & RELATIVE_COORD)
  {
    OdGeExtents3d ext;
    ((OdDbField*)ptr->user_data)->getGeomExtents()
    res += ext.center()
  }
  */
  if (relativeFlags & WCS_COORD)
  {
    if (OdDbDatabase* pDb = database(ptr))
    {
      OdGeMatrix3d ucsToWcsMatrix;
      ucsToWcsMatrix.setCoordSystem(pDb->getUCSORG(), pDb->getUCSXDIR(), pDb->getUCSYDIR(), pDb->getUCSXDIR().crossProduct(pDb->getUCSYDIR()));
      p.transformBy(ucsToWcsMatrix.inverse());
    }
  }
}

void* Vector(void* x, void* y, void* z, void* relativeFlags, AcExprEvalResult* ptr)
{
  OdGeVector3d p = convertCoords(x, y, z, ptr);
  if (ptr->success)
  {
    applyVectorFlags(relativeFlags, ptr, p);
    return acexprSaveToHeap(p, ptr);
  }
  return 0;
}

void* PolarVector(void* x, void* a, void* z, void* relativeFlags, AcExprEvalResult* ptr)
{
  OdGeVector3d p = convertCoords(x, a, z, ptr);
  if (ptr->success)
  {
    double angle = acexprConvertAngle(p.y, ptr);
    p = OdGeVector3d(p.x*cos(angle), p.x*sin(angle), p.z);
    applyVectorFlags(relativeFlags, ptr, p);
    return acexprSaveToHeap(p, ptr);
  }
  return 0;
}
void* SphereVector(void* r, void* phi, void* theta, void* relativeFlags, AcExprEvalResult* ptr)
{
  OdGeVector3d p = convertCoords(r, phi, theta, ptr);
  if (ptr->success)
  {
    p = OdGeVector3d(
      p.x*cos(OdaToRadian(p.z))*cos(OdaToRadian(p.y)),
      p.x*cos(OdaToRadian(p.z))*sin(OdaToRadian(p.y)),
      p.x*sin(OdaToRadian(p.z)));
    applyVectorFlags(relativeFlags, ptr, p);
    return acexprSaveToHeap(p, ptr);
  }
  return 0;
}

void* Round(void* d, AcExprEvalResult* ptr)
{
  double arg;
  switch (((OdFieldValue*)d)->dataType())
  {
  case OdValue::kDouble:
    arg = *((OdFieldValue*)d);
    break;
  case OdValue::kLong:
    arg = (OdInt32)*((OdFieldValue*)d);
    break;
  default:
    ptr->success = 0;
    break;
  }
  if (ptr->success)
    return acexprSaveToHeap(OdRound(arg), ptr);
  return 0;
}

void* R2d(void* d, AcExprEvalResult* ptr)
{
  double arg;
  switch(((OdFieldValue*)d)->dataType())
  {
  case OdValue::kDouble:
    arg = *((OdFieldValue*)d);
    break;
  case OdValue::kLong:
    arg = (OdInt32)*((OdFieldValue*)d);
    break;
  default:
    ptr->success = 0;
    break;
  }
  if (ptr->success)
    return acexprSaveToHeap(OdaToDegree(arg), ptr);
  return 0;
}

void* D2r(void* d, AcExprEvalResult* ptr)
{
  double arg;
  switch (((OdFieldValue*)d)->dataType())
  {
  case OdValue::kDouble:
    arg = *((OdFieldValue*)d);
    break;
  case OdValue::kLong:
    arg = (OdInt32)*((OdFieldValue*)d);
    break;
  default:
    ptr->success = 0;
    break;
  }
  if (ptr->success)
    return acexprSaveToHeap(OdaToRadian(arg), ptr);
  return 0;
}

void* Angle1(void* p, AcExprEvalResult* ptr)
{
  OdGeVector3d v;
  if (((OdFieldValue*)p)->get(v))
  {
    return convertResultA(OdGeVector3d::kXAxis.angleTo(v), ptr);
  }
  else
  {
    ptr->success = 0;
    return 0;
  }
}
void* Angle2(void* p1, void* p2, AcExprEvalResult* ptr)
{
  OdFieldValue* rb1 = (OdFieldValue*)p1, *rb2 = (OdFieldValue*)p2;
  OdGePoint3d pp1, pp2;
  if (rb1->get(pp1) && rb2->get(pp2))
  {
    return convertResultA(OdGeVector3d::kXAxis.angleTo(pp2 - pp1), ptr);
  }
  else
  {
    ptr->success = 0;
    return 0;
  }
}

void* Angle3(void* apex, void* p1, void* p2, AcExprEvalResult* ptr)
{
  OdFieldValue* rb1 = (OdFieldValue*)p1, *rb2 = (OdFieldValue*)p2, *rb3 = (OdFieldValue*)apex;
  OdGePoint3d pp1, pp2, papex;
  if (rb1->get(pp1) && rb2->get(pp2) && rb3->get(papex))
  {
    return convertResultA((pp1-papex).angleTo(pp2 - papex, OdGeVector3d::kZAxis), ptr);
  }
  else
  {
    ptr->success = 0;
    return 0;
  }
}

void* Angle4(void* apex, void* p1, void* p2, void* p, AcExprEvalResult* ptr)
{
  OdFieldValue* rb1 = (OdFieldValue*)p1, *rb2 = (OdFieldValue*)p2, *rb3 = (OdFieldValue*)apex, *rb4 = (OdFieldValue*)p;
  OdGePoint3d pp1, pp2, papex, refp;
  if (rb1->get(pp1) && rb2->get(pp2) && rb3->get(papex) && rb4->get(refp))
  {
    return convertResultA((pp1 - papex).angleTo(pp2 - papex, refp - papex), ptr);
  }
  else
  {
    ptr->success = 0;
    return 0;
  }
}

void* acexprParseDouble(const char* text, size_t len, AcExprEvalResult* ptr)
{
  OdAnsiString aStr(text);
  return acexprSaveToHeap(odStrToD(OdCharMapper::convertAlphaNumJapanese(aStr)), ptr);
}

void* acexprParseLiteral(const char* text, size_t len, AcExprEvalResult* ptr)
{
  OdString s(text, (int)len);
  return acexprSaveToHeap(s, ptr);
}

void* acexprParseInteger(const char* text, size_t len, AcExprEvalResult* ptr)
{
  OdInt32 val;
  sscanf(text, "%d", &val);
  return acexprSaveToHeap(val, ptr);
}

void* acexprCreateDouble2(double d, AcExprEvalResult* ptr)
{
  return acexprSaveToHeap(d, ptr);
}

void* acexprCreateInteger2(int i, AcExprEvalResult* ptr)
{
  return acexprSaveToHeap((OdInt32)i, ptr);
}

void* acexprCopyValue(void* p, AcExprEvalResult* ptr)
{
  if (p == 0)
    return 0;
  return acexprSaveToHeap(*(OdFieldValue*)p, ptr);
}

double acexprConvertToDouble(void* d, AcExprEvalResult* ptr)
{
  double arg = 0.0;
  switch (((OdFieldValue*)d)->dataType())
  {
  case OdValue::kDouble:
    arg = *(OdFieldValue*)d;
    break;
  case OdValue::kLong:
    arg = (OdInt32)*(OdFieldValue*)d;
    break;
  default:
    ptr->success = 0;
    break;
  }
  return arg;
}
// the input value is converted from current AUnits to radians
double convertArgumentA(void* d, AcExprEvalResult* ptr)
{
  double arg = acexprConvertToDouble(d, ptr);
  if (ptr->success)
  {
    OdInt16 aunits = 0;
    if (OdDbDatabase* pDb = database(ptr))
      aunits = pDb->getAUNITS();
    switch(aunits)
    {
    case 0: // decimal degrees
    case 1: // d m s
    case 4: // Surveyors
      arg *= OdaPI / 180.0;
      break;
    case 2: // grad
      arg *= OdaPI / 200.0;
      break;
    }
  }
  return arg;
}


// the result value is converted to current AUnits
void* convertResultA(double d, AcExprEvalResult* ptr)
{
  OdInt16 aunits = 0;
  if (OdDbDatabase* pDb = database(ptr))
    aunits = pDb->getAUNITS();
  switch(aunits)
  {
  case 0: // decimal degrees
  case 1: // d m s
  case 4: // Surveyors
    d *= 180.0 / OdaPI;
    break;
  case 2: // grad
    d *= 200.0 / OdaPI;
    break;
  }
  OdFieldValue* fv = (OdFieldValue*)acexprCreateDouble2(d, ptr);
  fv->setUnitType(OdValue::kAngle);
  return fv;
}

void* Sin(void* d, AcExprEvalResult* ptr)
{
  double res = convertArgumentA(d, ptr);
  if (ptr->success)
    return acexprCreateDouble2(sin(res), ptr);
  return 0;  
}
void* Cos(void* d, AcExprEvalResult* ptr)
{
  double res = convertArgumentA(d, ptr);
  if (ptr->success)
    return acexprCreateDouble2(cos(res), ptr);
  return 0;  
}
void* Tan(void* d, AcExprEvalResult* ptr)
{
  double res = convertArgumentA(d, ptr);
  if (ptr->success)
    return acexprCreateDouble2(tan(res), ptr);
  return 0;  
}
void* Asin(void* d, AcExprEvalResult* ptr)
{
  double res = acexprConvertToDouble(d, ptr);
  if (ptr->success)
    return convertResultA(OD_ASIN(res), ptr);
  return 0;  
}
void* Acos(void* d, AcExprEvalResult* ptr)
{
  double res = acexprConvertToDouble(d, ptr);
  if (ptr->success)
    return convertResultA(OD_ACOS(res), ptr);
  return 0;  
}
void* Atan(void* d, AcExprEvalResult* ptr)
{
  double res = acexprConvertToDouble(d, ptr);
  if (ptr->success)
    return convertResultA(atan(res), ptr);
  return 0;  
}
void* Log(void* d, AcExprEvalResult* ptr)
{
  double res = acexprConvertToDouble(d, ptr);
  if (ptr->success)
    return acexprCreateDouble2(log10(res), ptr);
  return 0;  
}
void* Ln(void* d, AcExprEvalResult* ptr)
{
  double res = acexprConvertToDouble(d, ptr);
  if (ptr->success)
    return acexprCreateDouble2(log(res), ptr);
  return 0;  
}
void* Exp(void* d, AcExprEvalResult* ptr)
{
  double res = acexprConvertToDouble(d, ptr);
  if (ptr->success)
    return acexprCreateDouble2(exp(res), ptr);
  return 0;  
}
void* Sqr(void* d, AcExprEvalResult* ptr)
{
  double res = acexprConvertToDouble(d, ptr);
  if (ptr->success)
    return acexprCreateDouble2(res*res, ptr);
  return 0;  
}
void* Sqrt(void* d, AcExprEvalResult* ptr)
{
  double res = acexprConvertToDouble(d, ptr);
  if (ptr->success)
    return acexprCreateDouble2(sqrt(res), ptr);
  return 0;  
}
void* Abs(void* d, AcExprEvalResult* ptr)
{
  if (((OdFieldValue*)d)->dataType() == OdValue::k3dPoint)
  {
    OdGePoint3d p;
    ((OdFieldValue*)d)->get(p);
    return acexprCreateDouble2(p.asVector().length(), ptr);
  }
  else
  {
    double res = acexprConvertToDouble(d, ptr);
    if (ptr->success)
      return acexprCreateDouble2(fabs(res), ptr);
  }
  return 0;  
}
void* Trunc(void* d, AcExprEvalResult* ptr)
{
  double res = acexprConvertToDouble(d, ptr);
  if (ptr->success)
    return acexprCreateDouble2(floor(res), ptr);
  return 0;  
}

// convert from AUnits to radians
double acexprConvertAngle(double a, AcExprEvalResult* ptr)
{
  OdInt16 aunits = 0;
  if (OdDbDatabase* pDb = database(ptr))
    aunits = pDb->getAUNITS();
  switch (aunits)
  {
  case 0: // decimal degrees
  case 1: // d m s
  case 4: // Surveyors
    a *= (OdaPI / 180);
    break;
  case 3:
    break;
  case 2: // grad
    a *= (OdaPI / 200);
    break;
  }
  return a;
}

void* acexprParseAngle(const char* text, size_t len, AcExprEvalResult* ptr)
{
  OdString s(text, (int)len);
  double d = OdUnitsFormatter::unformatA(s);
  // angle values coming w/o format are treated by trigonometric functions as being in current AUnits
  // but unformatA() always returns radians so we convert the result to AUnits as well
  OdInt16 aunits = 0;
  if (OdDbDatabase* pDb = database(ptr))
    aunits = pDb->getAUNITS();
  switch(aunits)
  {
  case 0: // decimal degrees
  case 1: // d m s
  case 4: // Surveyors
    d *= (180/OdaPI);
    break;
  case 3:
    break;
  case 2: // grad
    d *= (200/OdaPI);
    break;
  }
  OdFieldValue* fv = (OdFieldValue*)acexprCreateDouble2(d, ptr);
  fv->setUnitType(OdValue::kAngle);
  return fv;
}

void acexprPushImplicitTable(void* id, AcExprEvalResult* ptr)
{
  ((std::stack<OdDbObjectId>*)ptr->tablestack)->push(getObjectId(id, ptr));
}

void acexprPopImplicitTable(AcExprEvalResult * ptr)
{
  ((std::stack<OdDbObjectId>*)ptr->tablestack)->pop();
}

void* acexprParseArch(const char* text, size_t len, AcExprEvalResult* ptr)
{
  OdString s(text, (int)len);
  return acexprSaveToHeap(OdFieldValue(OdUnitsFormatter::unformatL(s)), ptr);
}

void* acexprParseObjRef(const char* text, size_t len, AcExprEvalResult* ptr)
{
  OdString s(text, (int)len);
  int i = s.find(L"\\_objid");
  if (i != -1)
  {
    s = s.mid(i + 7);
    s = s.left(s.getLength() - 2);
    s.trimLeft();
    s.trimRight();
    OdInt64 val;
    sscanf(s, "%" PRId64, &val);
    return acexprSaveToHeap(OdFieldValue(OdDbObjectId((OdDbStub*)val)), ptr);
  }
  ptr->success = 0;
  return 0;
}

void* acexprConvertToRange(void* v, AcExprEvalResult* ptr)
{
  OdArray<OdFieldValue> res;
  res.append(*(OdFieldValue*)v);
  return acexprSaveSequenceToHeap(res, ptr);
}

void* Exp10(void* d, AcExprEvalResult* ptr)
{
  double res = acexprConvertToDouble(d, ptr);
  if (ptr->success)
    return acexprCreateDouble2(pow(10.0, res), ptr);
  return 0;
}
