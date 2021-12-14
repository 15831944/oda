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
/* Sample implementation of the OdGeCurveElement2dPE and                */
/* OdGeCurveElement3dPE interfaces.                                     */
/*                                                                      */
/************************************************************************/
#if !defined(DGCURVEFUNCTIONSMODULE__INCLUDED_)
#define DGCURVEFUNCTIONSMODULE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RxModule.h"
#include "StaticRxObject.h"
#include "DgLineCurvePEImpl.h"
#include "DgArcCurvePEImpl.h"
#include "DgEllipseCurvePEImpl.h"

/************************************************************************/
/* OdDgCurveFunctionsModule                                             */
/* See DgCurveFunctionsModule.cpp for the implementation of this class  */
/************************************************************************/
class OdDgCurveFunctionsModule : public OdRxModule
{
  OdStaticRxObject<OdDgLineElement2dCurvePE>       _line2dCurvePE;
  OdStaticRxObject<OdDgLineElement3dCurvePE>       _line3dCurvePE;
  OdStaticRxObject<OdDgArcElement2dCurvePE>        _arc2dCurvePE;
  OdStaticRxObject<OdDgArcElement3dCurvePE>        _arc3dCurvePE;
  OdStaticRxObject<OdDgEllipseElement2dCurvePE>    _ellipse2dCurvePE;
  OdStaticRxObject<OdDgEllipseElement3dCurvePE>    _ellipse3dCurvePE;
protected:
	OdDgCurveFunctionsModule();
  void initApp();
  void uninitApp();
public:
	~OdDgCurveFunctionsModule();
};

typedef OdSmartPtr< OdDgCurveFunctionsModule > OdDgCurveFunctionsModulePtr;


#endif // !defined(DGCURVEFUNCTIONSMODULE__INCLUDED_)
