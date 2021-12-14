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
// ConsoleCommands.h
//
// list of commands via ODRX_CMD_ENTRY macro
//
// used as:
//   #define ODRX_CMD_ENTRY(cmdName, name, impl) odedRegCmds()->addCommand(&m_cmd##name); // etc
//   #include "ConsoleCommands.h"
//

#if !defined(_TEST_COMMAND) && defined(_DEBUG)
//#define _TEST_COMMAND
#endif

#ifdef ODRX_CMD_ENTRY

  ODRX_CMD_ENTRY(new, _New, NO_UNDO_IMPL 
                            virtual bool skipTempleteDialogByNonEmptyQNewTemplate() const { return false; }
                )                                   // menu : File / New...         (Ctrl+N)
 #undef ODRX_CMD_ENTRY_BASE_CLASS
 #define ODRX_CMD_ENTRY_BASE_CLASS OdqCmd_New
  ODRX_CMD_ENTRY(qnew, _QNew, NO_UNDO_IMPL
                              virtual bool skipTempleteDialogByNonEmptyQNewTemplate() const { return true; }
                )                                   // toolbar : Standard / QNew..
 // restore default
 #undef ODRX_CMD_ENTRY_BASE_CLASS
 #define ODRX_CMD_ENTRY_BASE_CLASS OdEdCommand
  ODRX_CMD_ENTRY(open, _Open, NO_UNDO_IMPL)         // menu : File / Open...        (Ctrl+O)
  ODRX_CMD_ENTRY(recover, _Recover, NO_UNDO_IMPL)         
  ODRX_CMD_ENTRY(audit, _Audit, NO_UNDO_IMPL)         
  ODRX_CMD_ENTRY(saveas, _SaveAs, NO_UNDO_IMPL)     // menu : File / Save ~As...    (Ctrl+Shift+S)
  ODRX_CMD_ENTRY(qsave, _Save, NO_UNDO_IMPL)        // menu : File / ~Save          (Ctrl+S)
  ODRX_CMD_ENTRY(closeall, _CloseAll, NO_UNDO_IMPL) // app menu (click on A) : ...
  ODRX_CMD_ENTRY(close, _Close, NO_UNDO_IMPL)       // menu : File / Close
  ODRX_CMD_ENTRY(export, _Export, NO_UNDO_IMPL)     // menu : File / Export...
  ODRX_CMD_ENTRY(import, _Import, )
  ODRX_CMD_ENTRY(quit, _Quit, NO_UNDO_IMPL)         // menu : File / E~xit          (Ctrl+Q)

  ODRX_CMD_ENTRY(script, _Script, NO_UNDO_IMPL)     // menu : Tools / ~Run Script...
 #if !(defined(ANDROID) || defined(__IPHONE_OS_VERSION_MIN_REQUIRED))
  ODRX_CMD_ENTRY(appload, _AppLoad, NO_UNDO_IMPL)   // menu : Tools / ~Load Application
 #endif
  // (see execute method in ConsoleReactors.cpp)
  ODRX_CMD_ENTRY(setvar, _SetVar, NO_UNDO_IMPL)     // menu : Tools / Inquiry / Set Variable
  ODRX_CMD_ENTRY(_setvar, _SetVarOvr, NO_UNDO_IMPL) // overridden entry

  // modify UI of console palette (special)
                                                    // menu : Tools / Command Line (Ctrl+9)
  ODRX_CMD_ENTRY(commandline, _UiConsoleShow, NO_UNDO_IMPL)
  ODRX_CMD_ENTRY(commandlinehide, _UiConsoleHide, NO_UNDO_IMPL)

  ODRX_CMD_ENTRY(commandlinetab, _UiConsoleTab, NO_UNDO_IMPL)

  ODRX_CMD_ENTRY(diesel, _Diesel, NO_UNDO_IMPL)

  ODRX_CMD_ENTRY(select, _Select, NO_UNDO_IMPL)
  ODRX_CMD_ENTRY(pselect, _PSelect, NO_UNDO_IMPL) // undocumented in ACAD but used in toolbar of property palette
  ODRX_CMD_ENTRY(ai_selall, _SelectAll, NO_UNDO_IMPL)
  ODRX_CMD_ENTRY(ai_deselect, _DeselectAll, NO_UNDO_IMPL)
  ODRX_CMD_ENTRY(handle, _Handle, NO_UNDO_IMPL)
  ODRX_CMD_ENTRY(extract, _Extract, NO_UNDO_IMPL)

  // _ to prevent overriding via ExCustObjs.tx loading 
  // (see with _STRETCH_func of DrxDebugCmds.cpp)
  // TODO support command of ExCustObjs.tx
  ODRX_CMD_ENTRY(_stretch, _Stretch, )
  ODRX_CMD_ENTRY(_erase, _Erase, )
  ODRX_CMD_ENTRY(erase, _EraseOvr, ) // overridden entry

  ODRX_CMD_ENTRY(dropgeom, _DropGeom, )
  //ODRX_CMD_ENTRY(copyclip, _Copyclip, NO_UNDO_IMPL)
  //ODRX_CMD_ENTRY(pasteclip, _Pasteclip, )

  ODRX_CMD_ENTRY(_u, _Undo, NO_UNDO_IMPL)         // menu : Edit / Undo     CTRL+Z
  ODRX_CMD_ENTRY(_mredo, _Redo, NO_UNDO_IMPL)         // menu : Edit / Redo     CTRL+Y

  ODRX_CMD_ENTRY(osnap, _OSnap, )

#ifdef _TEST_COMMAND
  ODRX_CMD_ENTRY(test, _Test, NO_UNDO_IMPL)
#endif
#if defined(_TST_COMMAND) || defined(_DEBUG)
  ODRX_CMD_ENTRY(tst, _Tst, )
#endif

  #undef ODRX_CMD_ENTRY
#endif // ODRX_CMD_ENTRY
