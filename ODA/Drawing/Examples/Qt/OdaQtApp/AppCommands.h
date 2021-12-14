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
// AppCommands.h
//
// list of commands via ODRX_CMD_ENTRY macro
//
// used as:
//   #define ODRX_CMD_ENTRY(cmdName, name, impl) odedRegCmds()->addCommand(&m_cmd##name); // etc
//   #include "ConsoleCommands.h"
//

#ifdef ODRX_CMD_ENTRY

  ODRX_CMD_ENTRY(menu, _Menu, NO_UNDO_IMPL)         // load cui-file
  ODRX_CMD_ENTRY(cuiload, _CuiLoad, NO_UNDO_IMPL)
  ODRX_CMD_ENTRY(cuiunload, _CuiUnload, NO_UNDO_IMPL)

 #if !(defined(ANDROID) || defined(__IPHONE_OS_VERSION_MIN_REQUIRED))
  // printing (is temporary here after using the class OdqPropertyDialog)
  // (see execute methods in AppPlotCommands.cpp)
  ODRX_CMD_ENTRY(print, _Print, )
  ODRX_CMD_ENTRY(plot, _Plot, )                     // menu : File / Plot...
  // TODO
  ODRX_CMD_ENTRY(pagesetup, _PageSetup, NO_UNDO_IMPL) // menu : File / Page Setup Manager...
  ODRX_CMD_ENTRY(preview, _PlotPreview, NO_UNDO_IMPL) // menu : File / Plot Preview
 #endif

  ODRX_CMD_ENTRY(layer, _Layer, )                   // menu : Format / Layer...

  //ODRX_CMD_ENTRY(options, _Options, NO_UNDO_IMPL)   // menu : Tools / Options...
  ODRX_CMD_ENTRY(resetvars, _ResetVars, NO_UNDO_IMPL) // (Console only) : restore default values of group registry variables [ALL/DWG/DGN/APP]<APP>

  ODRX_CMD_ENTRY(about, _About, NO_UNDO_IMPL)       // menu : Help / About...
  ODRX_CMD_ENTRY(help, _Help, NO_UNDO_IMPL)       // menu : Help / Help

  #undef ODRX_CMD_ENTRY
#endif // ODRX_CMD_ENTRY
