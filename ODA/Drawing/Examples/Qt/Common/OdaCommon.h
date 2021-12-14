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
//
// redirect file for temporal compatibility with v3.2

#include "../../../../Kernel/Include/OdaCommon.h"

#ifdef _MSC_VER
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif
#endif

///////////////////////////////////////////////////////////////////////////////

#ifndef OD_T

#define OD_T DD_T
#define __OD_T __DD_T

#endif

#include "OdModuleNames.h"

///////////////////////////////////////////////////////////////////////////////
//
// workaround for problem with catching exceptions (OdEdOtherInput and OdEdKeyword) 
// generated in other module on Linux
// (are really caught via base class OdEdException only on linux)
//

#ifndef FIX_LINUX_EXCEPTION_BEGIN

  #if defined(_TOOLKIT_IN_DLL_) && defined(__linux__) || defined(ANDROID)

    #define FIX_LINUX_EXCEPTION_BEGIN \
      try {

    #define FIX_LINUX_EXCEPTION_END \
      } catch (const OdError& err) { \
        if (err.description().find(OD_T("Exception: OdEdEmptyInput")) >= 0) \
          throw OdEdEmptyInput(); \
        else if (err.description().find(OD_T("Exception: OdEdOtherInput")) >= 0) \
          throw OdEdOtherInput(((const OdEdOtherInput*) &err)->string()); \
        else if (err.description().find(OD_T("Exception: OdEdKeyword")) >= 0) \
          throw OdEdKeyword(((const OdEdKeyword*) &err)->keywordIndex(), \
                            ((const OdEdKeyword*) &err)->keyword()); \
        else if (err.description().find(sidCommandCanceled) >= 0) /* OD_T("Command canceled.") */ \
          throw OdEdCancel(); \
        else \
          throw; \
      }

  #else

    #define FIX_LINUX_EXCEPTION_BEGIN
    #define FIX_LINUX_EXCEPTION_END

  #endif

  // see also macro FIX_MAC_APP_TROW

#endif

///////////////////////////////////////////////////////////////////////////////
