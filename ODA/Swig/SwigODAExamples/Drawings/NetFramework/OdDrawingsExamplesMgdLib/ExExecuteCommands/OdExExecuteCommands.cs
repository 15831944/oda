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
  public class OdExExecuteCommands
  {
    public static OdEdBaseIO CreateExStringIO(String strIO)
    {
      return ExStringIO.create(strIO);
    }

    public static OdDbCommandContext CreateDbCommandContext(String strIO, OdDbDatabase db)
    {
      return ExDbCommandContext.createObject(CreateExStringIO(strIO), db);
    }

    private String RecentCmdName { get; set; }

    public OdExExecuteCommands()
    {
      RecentCmdName = String.Empty;
    }

    public void ExecuteCommand(OdDbDatabase db, String sCmd, bool bEcho)
    {
      OdDbCommandContext pExCmdCtx = CreateDbCommandContext(sCmd, db);
      using (OdExCommandReactor commandReactor = new OdExCommandReactor(pExCmdCtx))
      {
        commandReactor.ChangedRecentCmdNameEvent += CommandReactor_ChangedRecentCmdNameEvent;
        try
        {
          OdEdCommandStack pCommands = Globals.odedRegCmds();
          {
            String s = sCmd.Substring(0, sCmd.IndexOf(" "));
            if (s.Length == sCmd.Length)
            {
              s = s.ToUpper();
              commandReactor.setLastInput(s); // cmd_reactor
              pCommands.executeCommand(s, pExCmdCtx);
            }
            else
            {
              ExStringIO m_pMacro = ExStringIO.create(sCmd);
              while (!m_pMacro.isEof())
              {
                try
                {
                  s = pExCmdCtx.userIO().getString("Command:");//(commandPrompt());
                  s = s.ToUpper();
                  commandReactor.setLastInput(s); // cmd_reactor
                }
                catch (OdEdEmptyInput)
                {
                  s = RecentCmdName;
                }
                pCommands.executeCommand(s, pExCmdCtx);
              }
            }
          }
        }
        catch (OdEdEmptyInput eEmptyInput)
        {
        }
        catch (OdEdCancel eCanc)
        {
        }
        catch (OdError err)
        {

        }
      }
    }

    private void CommandReactor_ChangedRecentCmdNameEvent(string recentCmd)
    {
      this.RecentCmdName = recentCmd;
    }
  }
}