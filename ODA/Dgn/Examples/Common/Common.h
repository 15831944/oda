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
#ifndef _EX_COMMON_H_
#define _EX_COMMON_H_


#include "OdaCommon.h"
#include "OdTimeStamp.h"
#include "DgExport.h"


extern TG_EXPORT_STATIC void ( *g_pLocalTimeFunc )( OdTimeStamp &localTime ); //replaceable time function


//that function allows to save to equal files because of zeroing any time field inside of it
static void FictiousGetLocalTime( OdTimeStamp &localTime )
{
  localTime.setToZero();
}


static void SetFictiousTimeFunction()
{
#ifdef USE_FICTIOUS_TIME
  g_pLocalTimeFunc = FictiousGetLocalTime;
#else
  //doing nothing
#endif
}


#endif //_EX_COMMON_H_
