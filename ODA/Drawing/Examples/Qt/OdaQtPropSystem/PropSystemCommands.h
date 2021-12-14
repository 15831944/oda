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
// PropSystemCommands.h
//
// list of commands via ODRX_CMD_ENTRY macro
//
// used as:
//   #define ODRX_CMD_ENTRY(cmdName, name, impl) odedRegCmds()->addCommand(&m_cmd##name); // etc
//   #include "PropSystemCommands.h"
//

#ifdef ODRX_CMD_ENTRY

  ODRX_CMD_ENTRY(properties, _UiPropertyShow, NO_UNDO_IMPL) // menu : Modify / properties 
  ODRX_CMD_ENTRY(propertiesclose, _UiPropertyHide, NO_UNDO_IMPL)

  /* {Secret} */
  // internal - is used created undo with set value vis property palette
  ODRX_CMD_ENTRY(setprop, _SetFilteredSelectionPropValue,) 

  ODRX_CMD_ENTRY(options, _Options, NO_UNDO_IMPL)
  ODRX_CMD_ENTRY(proppal, _PropPal, NO_UNDO_IMPL)
  ODRX_CMD_ENTRY(propdlg, _PropDlg, NO_UNDO_IMPL)
  ODRX_CMD_ENTRY(cda, _Cda, NO_UNDO_IMPL)

 #ifdef _DEBUG // QT_DEBUG
  /* {Secret} */
  ODRX_CMD_ENTRY(propertiestab, _UiPropertyTab, NO_UNDO_IMPL)
 #endif

  #undef ODRX_CMD_ENTRY
#endif // ODRX_CMD_ENTRY
