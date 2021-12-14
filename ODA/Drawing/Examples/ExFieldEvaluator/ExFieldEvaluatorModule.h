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

/************************************************************************/
/* Main header file for the DLL                                         */
/*                                                                      */
/* Dimplements a field evaluator                                        */
/************************************************************************/
#if !defined(AFX_CODA1_14EXAMPLESEXFIELDEVALUATOREXFIELDEVALUATORMODULE_H__15891676_A277_4316_881A_7C417635DBBA__INCLUDED_)
#define AFX_CODA1_14EXAMPLESEXFIELDEVALUATOREXFIELDEVALUATORMODULE_H__15891676_A277_4316_881A_7C417635DBBA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RxModule.h"
#include "ExFieldEvaluator.h"

class ExFieldEvaluatorModule : public OdRxModule
{
  OdSmartPtr<OdExEvaluatorLoader> m_pEvaluatorLoader;
  OdRxModulePtr m_pDynBlocks;
protected:
	ExFieldEvaluatorModule();

  void initApp();

  void uninitApp();

public:
	~ExFieldEvaluatorModule();
};

typedef OdSmartPtr<ExFieldEvaluatorModule> ExFieldEvaluatorModulePtr;


#endif // !defined(AFX_CODA1_14EXAMPLESEXFIELDEVALUATOREXFIELDEVALUATORMODULE_H__15891676_A277_4316_881A_7C417635DBBA__INCLUDED_)
