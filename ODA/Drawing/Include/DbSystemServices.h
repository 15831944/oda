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


#ifndef _ODDBSYSTEMSERVICES_INCLUDED_
#define _ODDBSYSTEMSERVICES_INCLUDED_

#include "RxSystemServices.h"
#include "DbExport.h"

#include "TD_PackPush.h"

/** \details
    This class is the base class for classes that provide platform-dependent file operations for
    the Drawings SDK.
*/
typedef OdRxSystemServices OdDbSystemServices;

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDbSystemServices object pointers.
*/
typedef OdSmartPtr<OdDbSystemServices> OdDbSystemServicesPtr;

TOOLKIT_EXPORT OdDbSystemServices* odSystemServices();

/** \details
    Performs Drawings core system initialization.  
    
    \remarks
    This function registers core database classes supported by the Drawings. Instances of these classes may
    be created by client applications. 
    
    It also allows the user to specify the OdDbSystemServices instance that to be used for file creation and access.  

    \param pSystemServices [in]  Pointer to SystemServices object.
	
    */
TOOLKIT_EXPORT void odInitializeDbCore(
  OdDbSystemServices* pSystemServices);

/** \details
    Performs Drawings system initialization.  
    
    \remarks
    This function registers all classes supported by the Drawings. Instances of these classes may
    Thus be created by client applications. 
    
    It also allows the user to specify the OdDbSystemServices instance that to be used for file creation and access.  
    
    \remarks
    This function should be called once per process prior to any other Drawings calls.
    
    \param pSystemServices [in]  Pointer to SystemServices object.
	
    */
DBLIB_EXPORT void odInitialize(
  OdDbSystemServices* pSystemServices);

/** \details
    Performs Drawings system uninitialization.  

    \remarks
    This function releases the dynamic class registration data and the OdDbSystemServices pointer 
    associated with the earlier call to odInitialize.  
    
    \remarks
    This function should be called once per process as the last operation performed by the Drawings.

    */
DBLIB_EXPORT void odUninitialize();

TOOLKIT_EXPORT void odUninitializeDbCore();


#include "TD_PackPop.h"

#endif // _ODDBSYSTEMSERVICES_INCLUDED_
