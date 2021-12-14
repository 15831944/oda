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




#ifdef _MSC_VER
#define _WIN32_WINNT 0x0400 // to enable Windows Cryptographic API
#endif

#include "OdaCommon.h"
#if defined _WIN32
#include <winsock2.h>
#endif
#include "ExSystemServices.h"
#include "ExHostAppServices.h"
//#include "OdFileBuf.h"
#include "RxDynamicModule.h"

//#define STL_USING_IOSTREAM
//#define OD_HAVE_FSTREAM_FILE
//#include "OdaSTL.h"
#define  STD(a)  std:: a

#ifdef OD_HAVE_CONSOLE_H_FILE
#include <console.h>
#endif

#include "../TfRevModule.h"

class MyServices : public ExSystemServices, public ExHostAppServices
{
protected:
  ODRX_USING_HEAP_OPERATORS(ExSystemServices);
public:
  OdDbHostAppProgressMeter* newProgressMeter() { return NULL; }
};

#if defined(OD_USE_WMAIN)
int wmain(int argc, wchar_t* argv[])
#else
int main(int argc, char* argv[])
#endif
{
  odPrintConsoleString(L"TfRevisionControler server example. Copyright (c) 2019, Open Design Alliance\n");
  
  // Create a custom Services object.
  OdRxObjectImpl<MyServices> svcs;
  odInitialize(&svcs);
  OdString path;
  if (argc == 2)
    path = argv[1];
  else
    odPrintConsoleString(L"Create new repository\n");

  ::odrxDynamicLinker()->loadModule(L"TD_DynBlocks", false);
  ::odrxDynamicLinker()->loadModule(L"TD_3DSolidHistory", false);
  ::odrxDynamicLinker()->loadModule(L"DbConstraints", false);
  //::odrxDynamicLinker()->loadModule(L"OdTfRevisionControl", false);//TODO remove?
  ::odrxDynamicLinker()->loadModule(L"TD_Tf", false);
  try
  {
    odPrintConsoleString(L"Start server\n");
    Git::startServer(path, &svcs);
  }
  catch(std::exception e)
  {
    OdString s = e.what();
    odPrintConsoleString(L"Stop server. Error %s\n", e.what());
  }
  catch (OdError e)
  {
    OdString s = e.description();
    odPrintConsoleString(L"Stop server. Error %s\n", s.c_str());
  }
  catch (...)
  {
    odPrintConsoleString(L"Stop server. Unknown error\n");
  }
  ::odrxDynamicLinker()->unloadModule(L"TD_DynBlocks");
  ::odrxDynamicLinker()->unloadModule(L"TD_3DSolidHistory");
  ::odrxDynamicLinker()->unloadModule(L"DbConstraints");
  //::odrxDynamicLinker()->unloadModule(L"OdTfRevisionControl");//TODO remove
  ::odrxDynamicLinker()->unloadModule(L"TD_Tf");
  odUninitialize();
}
