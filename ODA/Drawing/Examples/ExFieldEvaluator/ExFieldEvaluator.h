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

#ifndef _EXFIELDEVALUATOR_H_INCLUDED_
#define _EXFIELDEVALUATOR_H_INCLUDED_
/************************************************************************/
/* Defines notification functions for the evaluation of OdDbField       */ 
/* objects                                                              */
/************************************************************************/
#include "FdField.h"
#include "SmartPtr.h"
#define STL_USING_MAP
#include "OdaSTL.h"

class OdEvaluatorBase : public OdFdFieldEvaluator
{
  ODRX_DECLARE_MEMBERS(OdEvaluatorBase);
  OdEvaluatorBase(const OdString&);
  virtual const OdString evaluatorId() const;
  virtual OdResult initialize(OdDbField* pField) const; 
  virtual OdResult compile(OdDbField* pField, OdDbDatabase* pDb, OdFdFieldResult* pResult) const;
protected:
  OdString m_sId;
  OdString getFieldName(OdDbField* pField) const;
};
class OdTextEvaluator : public OdEvaluatorBase 
{
public:
  ODRX_DECLARE_MEMBERS(OdTextEvaluator);
  OdTextEvaluator();
  virtual const OdString evaluatorId(OdDbField* pField) const;
  virtual OdResult evaluate(OdDbField* pField, int nContext, OdDbDatabase* pDb, OdFdFieldResult* pResult) const;
  virtual OdResult format(OdDbField* pField, OdString& pszValue) const;
};

class OdVarEvaluator : public OdEvaluatorBase 
{
public:
  ODRX_DECLARE_MEMBERS(OdVarEvaluator);
  OdVarEvaluator();
  virtual const OdString evaluatorId(OdDbField* pField) const;
  virtual OdResult evaluate(OdDbField* pField, int nContext, OdDbDatabase* pDb, OdFdFieldResult* pResult) const;
  virtual OdResult format(OdDbField* pField, OdString& pszValue) const;
  virtual OdResult compile(OdDbField* pField, OdDbDatabase* pDb, OdFdFieldResult* pResult) const;
};

class OdDieselEvaluator : public OdEvaluatorBase 
{
public:
  ODRX_DECLARE_MEMBERS(OdDieselEvaluator);
  OdDieselEvaluator();
  virtual const OdString evaluatorId(OdDbField* pField) const;
  virtual OdResult evaluate(OdDbField* pField, int nContext, OdDbDatabase* pDb, OdFdFieldResult* pResult) const;
  virtual OdResult format(OdDbField* pField, OdString& pszValue) const;
  virtual OdResult compile(OdDbField* pField, OdDbDatabase* pDb, OdFdFieldResult* pResult) const;
};

class OdSheetSetEvaluator : public OdEvaluatorBase
{
public:
  ODRX_DECLARE_MEMBERS(OdSheetSetEvaluator);
  OdSheetSetEvaluator();
  virtual const OdString evaluatorId(OdDbField* pField) const;
  virtual OdResult evaluate(OdDbField* pField, int nContext, OdDbDatabase* pDb, OdFdFieldResult* pResult) const;
  virtual OdResult format(OdDbField* pField, OdString& value) const;
  virtual OdResult compile(OdDbField* pField, OdDbDatabase* pDb, OdFdFieldResult* pResult) const;
};

class OdExEvaluatorLoader : public OdFdFieldEvaluatorLoader
{
  typedef std::map<OdString,OdFdFieldEvaluatorPtr, OdString::lessnocase> EvaluatorMap;
  EvaluatorMap m_pEvaluators;
public:
  ODRX_DECLARE_MEMBERS(OdExEvaluatorLoader);
  OdExEvaluatorLoader();
  virtual OdFdFieldEvaluator* getEvaluator(
    const OdString& pszEvalId);
  virtual OdFdFieldEvaluator * findEvaluator(
    OdDbField* pField, 
    OdString& pszEvalId);
};

#endif //_EXFIELDEVALUATOR_H_INCLUDED_
