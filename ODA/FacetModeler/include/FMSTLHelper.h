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

#ifndef __FMSTLHELPER_H__
#define __FMSTLHELPER_H__


// Vector section.

#ifdef _MSC_VER

#include <yvals.h>              // warning numbers get enabled in yvals.h; we must include it 
                                // before disabling any warnings, because if we do not include
                                // it here, it will be included from within STL files and the
                                // warnings will be reset back to default states.

#pragma warning( push )
#pragma warning(disable: 4018)  // signed/unsigned mismatch
#pragma warning(disable: 4100)  // unreferenced formal parameter
#pragma warning(disable: 4245)  // conversion from 'type1' to 'type2', 
                                // signed/unsigned mismatch
#pragma warning(disable: 4663)  // C++ language change: to explicitly specialize 
                                // class template 'vector'
#pragma warning(disable: 4702)  // unreachable code
#pragma warning(disable: 4710)  // 'function' : function not inlined
#pragma warning(disable: 4786)  // identifier was truncated to 'number' characters
                                // in the debug information

#endif // _MSC_VER

#include <vector>


// Map section.

#ifdef _MSC_VER
#pragma warning(disable: 4018)  // signed/unsigned mismatch
#pragma warning(disable: 4100)  // unreferenced formal parameter
#pragma warning(disable: 4245)  // conversion from 'type1' to 'type2', 
                                // signed/unsigned mismatch
#pragma warning(disable: 4284)  // iterator::operator-> not a UDT or reference to UDT.
#pragma warning(disable: 4512)  // 'class' : assignment operator could not be generated
#pragma warning(disable: 4663)  // C++ language change: to explicitly specialize 
                                // class template 'vector'
#pragma warning(disable: 4710)  // 'function' : function not inlined
#pragma warning(disable: 4786)  // identifier was truncated to 'number' characters in 
                                // the debug information

// BUG: C4786 Warning Is Not Disabled with #pragma Warning
// STATUS: Microsoft has confirmed this to be a bug in the Microsoft product. 
// This warning can be ignored. This occured only in the <map> container.

#endif // _MSC_VER

#include <map>


// List section.

#ifdef _MSC_VER

#pragma warning(disable: 4100)  // unreferenced formal parameter
#pragma warning(disable: 4284)  // return type for 'identifier::operator ->' 
                                // is not a UDT or reference 
                                // to a UDT. Will produce errors if applied using 
                                // infix notation
#pragma warning(disable: 4710)  // 'function' : function not inlined
#pragma warning(disable: 4786)  // identifier was truncated to 'number' characters 
                                // in the debug information
#endif // _MSC_VER

#include <list>

// Stack section.

#ifdef _MSC_VER

#pragma warning(disable: 4018)  // signed/unsigned mismatch
#pragma warning(disable: 4100)  // unreferenced formal parameter
#pragma warning(disable: 4245)  // conversion from 'type1' to 'type2', 
                                // signed/unsigned mismatch
#pragma warning(disable: 4710)  // 'function' : function not inlined
#pragma warning(disable: 4786)  // identifier was truncated to 'number' 
                                // characters in the debug information
#pragma warning(disable: 4146)  // unary minus operator applied to unsigned type ( <deque> )
#endif // _MSC_VER

#include <stack>


// Algorithm section.

#include <algorithm>

// Functional section.

#include <functional>

// Set section.

#include <set>


#ifdef _MSC_VER

#pragma warning( pop )

#if _MSC_VER <= 1300
#pragma warning(disable: 4097) // typedef-name ... used as synonym for class-name
#endif

#endif // _MSC_VER

#endif  //__FMSTLHELPER_H__
