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

#include "OdaCommon.h"
#include "RxSysRegistry.h"
#include "RxDynamicModule.h"
#include "ExLispModule.h"
#include "ecl/ecl.h"
#include "Ed/EdCommandContext.h"
#include "Ed/EdUserIO.h"
#include "DbCommandContext.h"

ODRX_DEFINE_DYNAMIC_MODULE(ExLispModule);

// We are using global context pointer for simplicity. Don't do that in production code, use e.g. lisp environment.
OdDbCommandContextPtr gpCtx;

// these 2 functions illustrate how you may redirect ECL IO streams
// by overloading (gray:stream-write-string) and (gray:stream-write-char) for your custom stream
cl_object console_string_out(cl_object str, cl_object start, cl_object end)
{
  if (!ECL_STRINGP(str))
  {
    // signal error here
    return str;
  }
  int s = fixint(start);
  int e = ECL_FIXNUMP(end) ? fixint(end) : str->string.dim;
  if (!gpCtx.isNull())
    gpCtx->userIO()->putString(OdString(((wchar_t*)str->string.self) + s, e));
  return str;
}

cl_object console_char_out(cl_object str)
{
  wchar_t c = ECL_CHAR_CODE(str);
  if (!gpCtx.isNull())
    gpCtx->userIO()->putString(OdString(c, 1));
  return str;
}

// this is am example of some random function (actually present in AutoLisp), accessing database code
cl_object handent(cl_object str)
{
  if (!ECL_STRINGP(str))
  {
    // signal error here
    return ecl_make_int64_t(0);
  }
  OdString handle((wchar_t*)str->string.self, str->string.dim);
  OdDbObjectId id = gpCtx->database()->getOdDbObjectId(OdDbHandle(handle));
  return ecl_make_int64_t((OdUInt64)(OdDbStub*)id);
}

void ExLispModule::initApp()
{
  char* ecl = "ecl";
  // Initialize ECL
  cl_boot(1, &ecl);

  // redirect IO to command console via gray streams
  cl_eval(c_string_to_object("(import 'gray)"));
  cl_eval(c_string_to_object("(defclass my-stream (gray:fundamental-character-output-stream) ())"));
  cl_def_c_function(c_string_to_object("c-my-string-out"), (cl_objectfn_fixed)console_string_out, 3);
  cl_def_c_function(c_string_to_object("c-my-char-out"), (cl_objectfn_fixed)console_char_out, 1);
  cl_eval(c_string_to_object("(defmethod gray:stream-write-char ((stream my-stream) character) (c-my-char-out character))"));
  cl_eval(c_string_to_object("(defmethod gray:stream-write-string ((stream my-stream) string &optional (start 0) end) (c-my-string-out string start end))"));
  cl_eval(c_string_to_object("(setf *standard-output* (make-instance 'my-stream))"));

  // install callbacks to database code
  cl_def_c_function(c_string_to_object("handent"), (cl_objectfn_fixed)handent, 1);
  // ... other callbacks here
}

void ExLispModule::uninitApp()
{
  cl_shutdown();
}

struct ExLispEngine : OdStaticRxObject<OdEdLispEngine>
{

  virtual void addReactor(OdEdLispEngineReactor* pReactor) {}
  virtual void removeReactor(OdEdLispEngineReactor* pReactor){}
  virtual void execute(OdEdCommandContext* pCmdCtx, const OdString& firstLine)
  {
    gpCtx = pCmdCtx;
    cl_eval(c_string_to_object((const char*)firstLine));
    gpCtx = 0;
  }
};

OdEdLispEnginePtr ExLispModule::createLispEngine()
{
  static ExLispEngine engine;
  return &engine;
}
