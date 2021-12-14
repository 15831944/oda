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
// TigDbCommands.h
//
// list of commands via ODRX_CMD_ENTRY macro
//
// used as:
//   #define ODRX_CMD_ENTRY(cmdName, name, impl) odedRegCmds()->addCommand(&m_cmd##name); // etc
//   #include "TigCommands.h"
//

#ifdef ODRX_CMD_ENTRY

  ODRX_CMD_ENTRY(revisions, _UiRevisionShow, NO_UNDO_IMPL) // toolbar : Revision Control 
  ODRX_CMD_ENTRY(revisionsclose, _UiRevisionHide, NO_UNDO_IMPL)
  ODRX_CMD_ENTRY(revisionsrefresh, _UIRevisionsRefresh, NO_UNDO_IMPL)

  ODRX_CMD_ENTRY(commit, _Commit, NO_UNDO_IMPL)
  ODRX_CMD_ENTRY(createbranch, _CreateBranch, NO_UNDO_IMPL)
  ODRX_CMD_ENTRY(merge, _Merge, NO_UNDO_IMPL)
  ODRX_CMD_ENTRY(bswitch, _Switch, NO_UNDO_IMPL)
  ODRX_CMD_ENTRY(saveastig, _SaveAsTig, NO_UNDO_IMPL)
  ODRX_CMD_ENTRY(saveasdsf, _SaveAsDsf, NO_UNDO_IMPL)
  ODRX_CMD_ENTRY(exporttodwg, _ExportToDwg, NO_UNDO_IMPL)

  #undef ODRX_CMD_ENTRY
#endif // ODRX_CMD_ENTRY
