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

#ifndef OD_GESHAREDCURVE_2D_PRT_H
#define OD_GESHAREDCURVE_2D_PRT_H /*!DOM*/

//#include "Ge/GeCurve2d.h"
#include "SharedPtr.h"
#include "OdArray.h"

class OdGeCurve2d;

#include "TD_PackPush.h"

/** \details
    This template class is a specialization of the OdSharedPtr class template for 2D curves.
*/
typedef OdSharedPtr<OdGeCurve2d> OdGeCurve2dPtr;

/** \details
    This template class is a specialization of the OdArray class template for 2D curve shared pointers.
*/
typedef OdArray<OdGeCurve2dPtr> OdGeCurve2dPtrArray;

#include "TD_PackPop.h"

#endif

