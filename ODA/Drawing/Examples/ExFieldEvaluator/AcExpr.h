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
#ifndef _AcExpr_h_Included_
#define _AcExpr_h_Included_

#include "OdPlatformSettings.h"

typedef struct AcExprEvalResult
{
  void* result;
  int success;
  void* user_data;
  void* heap;
  void* tablestack;
  void* rbheap;
  void* database;
  void* srcvalues;
}
AcExprEvalResult;

#if defined __cplusplus
extern "C" {
#endif
void* acexprCopyValue(void* p, AcExprEvalResult* ptr);
void* acexprParseDouble(const char* text, size_t len, AcExprEvalResult* ptr);
void* acexprParseAngle(const char* text, size_t len, AcExprEvalResult* ptr);
void* acexprCreateDouble2(double d, AcExprEvalResult* ptr);
void* acexprParseLiteral(const char* text, size_t len, AcExprEvalResult* ptr);
void* acexprParseObjRef(const char* text, size_t len, AcExprEvalResult* ptr);
void* acexprParseInteger(const char* text, size_t len, AcExprEvalResult* ptr);
void* acexprParseArch(const char* text, size_t len, AcExprEvalResult* ptr);
void* acexprCreateInteger2(int i, AcExprEvalResult* ptr);
double acexprConvertToDouble(void* d, AcExprEvalResult* ptr);
void* acexprConvertToRange(void* v, AcExprEvalResult* ptr);
double acexprConvertAngle(double a, AcExprEvalResult* ptr);
void* Sub(void* x, void* y, AcExprEvalResult* ptr);
void* Add(void* x, void* y, AcExprEvalResult* ptr);
void* Div(void* x, void* y, AcExprEvalResult* ptr);
void* Mul(void* x, void* y, AcExprEvalResult* ptr);
void* Pow(void* x, void* y, AcExprEvalResult* ptr);
void* Neg(void* x, AcExprEvalResult* ptr);
void* TableEval(void* id, void* fun, void* range, AcExprEvalResult* ptr);
void* ImplicitTableEval(void* fun, void* range, AcExprEvalResult* ptr);
void* acexprCreateTableSequence(void* val1, void* val2, AcExprEvalResult* ptr);
void* acexprCreateTableRange(void* cell1, void* cell2, AcExprEvalResult* ptr);
void  acexprPushImplicitTable(void* id, AcExprEvalResult* ptr);
void  acexprPopImplicitTable(AcExprEvalResult* ptr);
void* TableCell(void* id, void* cell, AcExprEvalResult* ptr);
void* ImplicitTableCell(void* cell, AcExprEvalResult* ptr);
void* ObjectEval(void* id, void* fun, AcExprEvalResult* ptr);
void* Vector(void* x, void* y, void* z, void* relativeFlags, AcExprEvalResult* ptr);
void* PolarVector(void* x, void* a, void* z, void* relativeFlags, AcExprEvalResult* ptr);
void* SphereVector(void* x, void* p, void* r, void* relativeFlags, AcExprEvalResult* ptr);
void* Vec(void* p1, void* p2, AcExprEvalResult* ptr);
void* UnitVec(void* p1, void* p2, AcExprEvalResult* ptr);
void* Normal(void* p1, void* p2, AcExprEvalResult* ptr);
void* Ill(void* p1, void* p2, void* p3, void* p4, AcExprEvalResult* ptr); 
void* Ilp(void* p1, void* p2, void* p3, void* p4, void* p5, AcExprEvalResult* ptr); 
void* Distance(void* x, void* y, AcExprEvalResult* ptr);
void* Angle1(void* p, AcExprEvalResult* ptr);
void* Angle2(void* p1, void* p2, AcExprEvalResult* ptr);
void* Angle3(void* apex, void* p1, void* p2, AcExprEvalResult* ptr);
void* Angle4(void* apex, void* p1, void* p2, void* p, AcExprEvalResult* ptr);
void* Round(void* d, AcExprEvalResult* ptr);
void* R2d(void* d, AcExprEvalResult* ptr);
void* D2r(void* d, AcExprEvalResult* ptr);
void* Pld(void* p1, void* p2, void* p3, AcExprEvalResult* ptr); 
void* Plt(void* p1, void* p2, void* p3, AcExprEvalResult* ptr); 
void* Dpl(void* p1, void* p2, void* p3, AcExprEvalResult* ptr); 
void* Dpp(void* p1, void* p2, void* p3, void* p4, AcExprEvalResult* ptr); 
void* Rot3(void* p1, void* p2, void* p3, AcExprEvalResult* ptr);
void* Rot4(void* p1, void* p2, void* p3, void* p4, AcExprEvalResult* ptr);
void* CrossProduct(void* p1, void* p2, AcExprEvalResult* ptr);

void* Sin(void* d, AcExprEvalResult* ptr);
void* Cos(void* d, AcExprEvalResult* ptr);
void* Tan(void* d, AcExprEvalResult* ptr);
void* Asin(void* d, AcExprEvalResult* ptr);
void* Acos(void* d, AcExprEvalResult* ptr);
void* Atan(void* d, AcExprEvalResult* ptr);
void* Log(void* d, AcExprEvalResult* ptr);
void* Ln(void* d, AcExprEvalResult* ptr);
void* Exp(void* d, AcExprEvalResult* ptr);
void* Sqr(void* d, AcExprEvalResult* ptr);
void* Sqrt(void* d, AcExprEvalResult* ptr);
void* Abs(void* d, AcExprEvalResult* ptr);
void* Trunc(void* d, AcExprEvalResult* ptr);
void* Exp10(void* d, AcExprEvalResult* ptr);

void* Functor(void* fun, void* param, AcExprEvalResult* ptr);

#if defined __cplusplus
}
#endif

#define YYSTYPE void*
#define YY_NO_UNISTD_H
#define YY_DECL \
       int yylex(YYSTYPE * yylval, YYLTYPE* loc, yyscan_t yyscanner, AcExprEvalResult* ptr )
#endif
