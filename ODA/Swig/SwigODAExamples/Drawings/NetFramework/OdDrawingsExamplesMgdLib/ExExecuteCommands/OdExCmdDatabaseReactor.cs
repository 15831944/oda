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
  public class OdExCmdDatabaseReactor : OdDbDatabaseReactor
  {
    #region Standard Part OdRxClass-related
    private static OdExCmdDatabaseReactorClass g_pDesc = new OdExCmdDatabaseReactorClass();
    public static new OdRxClass desc() { return g_pDesc; }
    public override OdRxClass isA() { return g_pDesc; }
    public override OdRxObject queryX(OdRxClass pClass)
    {
      if (desc().isDerivedFrom(pClass))
        return this;
      else
        return null;
    }
    #endregion

    private OdDbCommandContext m_Ctx = null;
    private bool m_bModified = false;

    public OdExCmdDatabaseReactor(OdDbCommandContext pCtx)
    {
      m_Ctx = pCtx;
      m_bModified = false;
      m_Ctx.database().addReactor(this);
    }

    public override void Dispose()
    {
      if (!m_bModified)
      {
        m_Ctx.database().removeReactor(this);
      }
      base.Dispose();
    }

    public void setUserIOString(String str)
    {
      m_Ctx.userIO().putString(str);
    }

    public OdDbCommandContext getCtx()
    {
      return m_Ctx;
    }

    public override void headerSysVarWillChange(OdDbDatabase db, String str)
    {
      setModified();
    }

    public override void objectOpenedForModify(OdDbDatabase db, OdDbObject obj)
    {
      setModified();
    }

    public bool isDatabaseModified() { return m_bModified; }

    private void setModified()
    {
      m_bModified = true;
      m_Ctx.database().removeReactor(this); // here we should inherit OdDbDatabaseReactor to match the argument
    }
  }

}
