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
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the TADEBUGTX_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// TADEBUGTX_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifndef _FMDEBUG_
#define _FMDEBUG_

//////////////////////////////////////////////////////////////////////////
//
typedef int OPERATION_HANDLE;
#define INVALID_OPERATION_HANDLE (-1)

inline void DbgSave2dOperationInput(FacetModeler::BooleanOperation eOperation, const FacetModeler::Profile2D &rArg1, const FacetModeler::Profile2D &rArg2, const OdGeTol& tol)
{}
inline void DbgSave2dOperationResult(FacetModeler::Result eRes, const FacetModeler::Profile2D &rResult)
{}

inline OPERATION_HANDLE DbgSave3dOperationBegin(FacetModeler::BooleanOperation eOperation, const FacetModeler::Body& rOperandA, const FacetModeler::Body& rOperandB)
{ return INVALID_OPERATION_HANDLE; }

inline OPERATION_HANDLE DbgSave3dOperationBegin(FacetModeler::BooleanOperation eOperation, const FacetModeler::Body& rOperandA)
{ return INVALID_OPERATION_HANDLE; }

inline OPERATION_HANDLE DbgSave3dOperationBegin(FacetModeler::BooleanOperation eOperation)
{ return INVALID_OPERATION_HANDLE; }

inline void DbgSave3dOperationRoutedToOldBoolean(OPERATION_HANDLE hOp)
{}

inline void DbgSave3dOperationOldBooleanResult(OPERATION_HANDLE hOp, const FacetModeler::Body& rResult)
{}

inline void DbgSave3dOperationResult(OPERATION_HANDLE hOp, const FacetModeler::Body& rResult)
{}

inline void DbgSave3dOperationException(OPERATION_HANDLE hOp)
{}

inline void DbgSave3dOperationSetCaller(const char* str)
{}

inline void DbgSave3dOperationEnable(bool bEnable)
{}

#endif //_FMDEBUG_
