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

#ifndef EXSYSTEMSERVICES_DEFINED
#define EXSYSTEMSERVICES_DEFINED

#include "TD_PackPush.h"

#include "RxSystemServicesImpl.h"

#include "RxInit.h"

/** \details
  <group ExServices_Classes>
*/
class ExSystemServices : public RxSystemServicesImpl
{
public:
  /** \details
    Default constructor.
  */
  ExSystemServices()
  {
    static const char* ActInfo[] = {
#ifdef TEIGHA_TRIAL
      "", ""
#else
      //"UserInfo", "UserSignature" 

// Before compiling, an ODA activation file should be placed in a location that a compiler can access, 
// otherwise you get a compiler error such as "Kernel/Extensions/ExServices/ExSystemServices.h:43:10: fatal error: 'OdActivationInfo' file not found". 
// To learn about ODA software activation, see the activation guide at https://docs.opendesign.com/tkernel/oda_activation_cpp.html    
#include "OdActivationInfo"
#endif
    };

    odActivate(ActInfo[0], ActInfo[1]);
  }
  
  /** \details
    Virtual destructor.
  */
  virtual ~ExSystemServices()
  {
    odCleanUpStaticData();
  }
};

#include "TD_PackPop.h"

#endif// EXSYSTEMSERVICES_DEFINED
