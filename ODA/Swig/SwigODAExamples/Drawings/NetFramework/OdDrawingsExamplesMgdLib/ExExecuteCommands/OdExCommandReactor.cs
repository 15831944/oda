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

using System;
using Teigha.Core;
using Teigha.TD;

namespace OdDrawingsExamplesMgdLib.ExExecuteCommands
{
  public class OdExCommandReactor : OdEdCommandStackReactor
  {
    private String m_sLastInput = String.Empty;

    private OdExCmdDatabaseReactor dbReactor = null;

    public OdExCommandReactor(OdDbCommandContext pCmdCtx)
    {
      dbReactor = new OdExCmdDatabaseReactor(pCmdCtx);
      Globals.odedRegCmds().addReactor(this);
    }

    public override void Dispose()
    {
      dbReactor.Dispose();
      Globals.odedRegCmds().removeReactor(this);
      base.Dispose();
    }

    public void setLastInput(String sLastInput)
    {
      m_sLastInput = sLastInput;
    }

    public String lastInput()
    {
      return m_sLastInput;
    }

    public bool isDatabaseModified() { return dbReactor.isDatabaseModified(); }

    public override OdEdCommand unknownCommand(String sCmdName, OdEdCommandContext pCmdCtx)
    {
      String sMsg = String.Format("Unknown command {0}.", sCmdName);
      dbReactor.setUserIOString(sMsg);
      return new OdEdCommand();
    }

    public delegate void RecentCmdNameEventHandler(string message);

    public event RecentCmdNameEventHandler ChangedRecentCmdNameEvent;

    public override void commandWillStart(OdEdCommand pCmd, OdEdCommandContext pCmdCtx)
    {
      String lastInput = m_sLastInput.ToUpper();
      if ((pCmd.flags() & OdEdCommand.kNoHistory) != 0)
      {
        if (ChangedRecentCmdNameEvent != null)
          ChangedRecentCmdNameEvent(m_sLastInput);
      }

      if ((pCmd.flags() & OdEdCommand.kNoUndoMarker) != 0)
      {
        dbReactor.getCtx().database().startUndoRecord();
      }
    }

    public override void commandCancelled(OdEdCommand pCmd, OdEdCommandContext pCmdCtx)
    {
      undoCmd();
    }
    public override void commandFailed(OdEdCommand pCmd, OdEdCommandContext pCmdCtx)
    {
      undoCmd();
    }

    private void undoCmd()
    {
      OdDbDatabase pDb = dbReactor.getCtx().database();
      try
      {
        pDb.disableUndoRecording(true);
        pDb.undo();
        pDb.disableUndoRecording(false);
      }
      catch (OdError err)
      {
        throw new Exception("Can't repair database: " + err.Message);
      }
    }
  }

}
