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
// ViewSystemCommands.h
//
// list of commands via ODRX_CMD_ENTRY macro
//
// used as:
//   #define ODRX_CMD_ENTRY(cmdName, name, impl) odedRegCmds()->addCommand(&m_cmd##name); // etc
//   #include "ViewSystemCommands.h"
//

#ifdef ODRX_CMD_ENTRY

  ODRX_CMD_ENTRY(qvlayout, _QVLayout, NO_UNDO_IMPL)
  ODRX_CMD_ENTRY(model, _Model, NO_UNDO_IMPL)
  ODRX_CMD_ENTRY(layout, _Layout, NO_UNDO_IMPL)

  ODRX_CMD_ENTRY(zoom, _Zoom, NO_UNDO_IMPL)         // menu : View / Zoom / Realtime              // move the wheel of the mouse
  ODRX_CMD_ENTRY(pan, _Pan, NO_UNDO_IMPL)           // menu : View / Pan / Realtime               // click the wheel of the mouse
  
  ODRX_CMD_ENTRY(3dorbit, _3dConstrainedOrbit, NO_UNDO_IMPL)  // menu : View / Orbit / Constrained Orbit // shift + click the wheel of the mouse 
  ODRX_CMD_ENTRY(3dforbit, _3dFreeOrbit, NO_UNDO_IMPL)        // menu : View / Orbit / Free Orbit        // shift + control + click the wheel of the mouse 
  ODRX_CMD_ENTRY(3dcorbit, _3dContinuousOrbit, NO_UNDO_IMPL)  // menu : View / Orbit / Continuous Orbit

  // should be -view but it will be conflicted with our -view command of DbCommand.tx
  ODRX_CMD_ENTRY(view, _View, NO_UNDO_IMPL)  // menu : View / 3D Views ...

  ODRX_CMD_ENTRY(vscurrent, _VSCurrent, NO_UNDO_IMPL)  // menu : View / Visual Styles / ...

  ODRX_CMD_ENTRY(regenall, _RegenAll, NO_UNDO_IMPL)
  ODRX_CMD_ENTRY(regen, _Regen, NO_UNDO_IMPL) // for active (currently zoomed) viewport only
#if defined(ODA_WINDOWS) && defined(_TOOLKIT_IN_DLL_)
  ODRX_CMD_ENTRY(dlggles, _GlesDlg, NO_UNDO_IMPL)
#endif

  ODRX_CMD_ENTRY(syswindows, _SysWindows, NO_UNDO_IMPL)  // menu : Window / Cascade ... Arange ...

  // { Secret }
  ODRX_CMD_ENTRY(bclrtoapp, _BackgroundToApp, NO_UNDO_IMPL)
  ODRX_CMD_ENTRY(apptobclr, _AppToBackground, NO_UNDO_IMPL)
  ODRX_CMD_ENTRY(syswindow, _SetActiveWindowByTitle, NO_UNDO_IMPL)

  #undef ODRX_CMD_ENTRY
#endif // ODRX_CMD_ENTRY
