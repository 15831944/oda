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

// OdCurveFunctions.h : main header file for the ODCURVEFUNCTIONS DLL
//

#if !defined(AFX_CODA1_14EXAMPLESODCURVEFUNCTIONSODCURVEFUNCTIONSMODULE_H__33B36B73_634C_4B19_AF0D_29BF75626E8C__INCLUDED_)
#define AFX_CODA1_14EXAMPLESODCURVEFUNCTIONSODCURVEFUNCTIONSMODULE_H__33B36B73_634C_4B19_AF0D_29BF75626E8C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RxModule.h"
#include "OdCurveFunctions.h"
#include "StaticRxObject.h"
/////////////////////////////////////////////////////////////////////////////
// OdCurveFunctionsModule
// See OdCurveFunctions.cpp for the implementation of this class
//

class OdCurveFunctionsModule : public OdRxModule
{
protected:
	OdCurveFunctionsModule();
  OdStaticRxObject<OdDbCurvePE_Line> m_pLinePE;
  OdStaticRxObject<OdDbCurvePE_Arc> m_pArcPE;

  void initApp();

  void uninitApp();

public:
	~OdCurveFunctionsModule();
};

typedef OdSmartPtr<OdCurveFunctionsModule> OdCurveFunctionsModulePtr;


#endif // !defined(AFX_CODA1_14EXAMPLESODCURVEFUNCTIONSODCURVEFUNCTIONSMODULE_H__33B36B73_634C_4B19_AF0D_29BF75626E8C__INCLUDED_)
