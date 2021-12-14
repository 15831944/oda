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
// ViewSystemModule.cpp
//

//#include "ViewSystemCommands.h"
#include "ViewSystemModule.h"
#include "OdqViewSystem.h"

// paper-drawing protocol extension
#include "DbLayout.h"
#include "DbLayoutPaperPE.h"
#include "Gi/GiWorldDraw.h"

//#if defined(_TOOLKIT_IN_DLL_) && defined(_MSC_VER)
//
//extern "C" int APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID )
//{
//  switch (dwReason)
//  {
//  case DLL_PROCESS_ATTACH:
//    // remove this if you need per-thread initialization
//    DisableThreadLibraryCalls((HMODULE) hInstance);
//    break;
//  }
//  return TRUE;
//}
//#endif //_TOOLKIT_IN_DLL_

ODRX_DEFINE_DYNAMIC_MODULE(OdqViewSystemModule)

//-----------------------------------------------------------------------

// paper-drawing protocol extension
static class OdDbLayoutPaperPEImpl : public OdStaticRxObject<OdDbLayoutPaperPE>
{
public:
  virtual bool drawPaper(const OdDbLayout* , OdGiWorldDraw* pWd, OdGePoint3d* points)
  {
    pWd->geometry().polygon(4, points);
    return true;
  }
  virtual bool drawBorder(const OdDbLayout*, OdGiWorldDraw* pWd, OdGePoint3d* points)
  {
    pWd->geometry().polygon(4, points);
    return true;
  }
  virtual bool drawMargins(const OdDbLayout*, OdGiWorldDraw* pWd, OdGePoint3d* points)
  {
    if (points[0] == points[1] || points[1] == points[2])
      return true;

    int nDashes = 15, i;
    OdGiGeometry& geom = pWd->geometry();
    OdGePoint3d dash1[2];
    OdGePoint3d dash2[2];
    OdGeVector3d step = (points[1] - points[0]) / (nDashes * 2 + 1);
    dash1[0] = points[0];
    dash2[0] = points[2];
    for (i = 0; i <= nDashes; ++i)
    {
      dash1[1] = dash1[0] + step;
      geom.polyline(2, dash1);
      dash1[0] = dash1[1] + step;
      dash2[1] = dash2[0] - step;
      geom.polyline(2, dash2);
      dash2[0] = dash2[1] - step;
    }
    nDashes = int((points[2] - points[1]).length() / step.length() - 1) / 2;
    step = (points[2] - points[1]) / (nDashes * 2 + 1);
    dash1[0] = points[1];
    dash2[0] = points[3];
    for (i = 0; i <= nDashes; ++i)
    {
      dash1[1] = dash1[0] + step;
      geom.polyline(2, dash1);
      dash1[0] = dash1[1] + step;
      dash2[1] = dash2[0] - step;
      geom.polyline(2, dash2);
      dash2[0] = dash2[1] - step;
    }
    return true;
  }
} s_PaperDrawExt;

static OdSharedPtr<OdqViewSystem> s_pVS;

// called at the start
// register custom objects and commands defined in the module

void OdqViewSystemModule::initApp()
{
  // initialize RTTI classes

  //...::rxInit(); 
  OdDbLayout::desc()->addX(OdDbLayoutPaperPE::desc(), &s_PaperDrawExt); // addPaperDrawingCustomization()

  // create single instance of view system (dispatcher of mdi windows)
  ODA_ASSERT_ONCE(s_pVS.isNull());
  s_pVS = OdqViewSystem::create(); 
  if (s_pVS->isValid())
    // register
    getIApp()->registerQObject(IqViewSystem_iid, s_pVS);

  // register commands

  OdEdCommandStackPtr pCommands = odedRegCmds();

  //pCommands->addCommand(&m_cmdOpen);
  #define ODRX_CMD_ENTRY(cmdName, name, impl) pCommands->addCommand(&m_cmd##name);
  #include "ViewSystemCommands.h"
}

// called at the end
// unregister custom objects and commands defined in the module

void OdqViewSystemModule::uninitApp()
{
  // unregister commands

  OdEdCommandStackPtr pCommands = odedRegCmds();

  //pCommands->removeCmd(&m_cmdOpen);
  #define ODRX_CMD_ENTRY(cmdName, name, impl) pCommands->removeCmd(&m_cmd##name);
  #include "ViewSystemCommands.h"

  // unregister single instance of View System and delete it
  ODA_ASSERT_ONCE(!s_pVS.isNull());
  if (s_pVS->isValid())
  {
    s_pVS->saveSettings(); // TODO // s_pVS->uninit();
    getIApp()->unregisterQObject(IqViewSystem_iid);
  }
  s_pVS = NULL;

  // uninitialize RTTI classes

  OdDbLayout::desc()->delX(OdDbLayoutPaperPE::desc()); // removePaperDrawingCustomization
  //...::rxUninit(); 
}

