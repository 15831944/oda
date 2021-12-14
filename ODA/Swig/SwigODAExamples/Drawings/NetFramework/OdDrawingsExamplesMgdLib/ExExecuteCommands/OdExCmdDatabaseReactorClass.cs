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
  public class OdExCmdDatabaseReactorClass : OdRxClass
  {
    public OdExCmdDatabaseReactorClass() { }
    public override OdRxObject create()
    {
      return new OdExCmdDatabaseReactorClass();
    }
    public new String appName() { return "OdDrawingsExamplesMgdLib.ExExecuteCommands"; }
    public new String dxfName() { return String.Empty; }
    public new String name() { return this.GetType().FullName; }
    public new DwgVersion getClassVersion(out MaintReleaseVer pMaintReleaseVer)
    {
      pMaintReleaseVer = MaintReleaseVer.kMReleaseCurrent;
      return DwgVersion.kDHL_CURRENT;
    }
    public new DwgVersion getClassVersion() { return DwgVersion.kDHL_CURRENT; }
    public new UInt32 proxyFlags() { return 0; }
    public override bool isDerivedFrom(OdRxClass pClass)
    {
      if (pClass == this)
        return true;
      return OdDbDatabaseReactor.desc().isDerivedFrom(pClass);
    }
    public new OdRxClass myParent() { return OdDbDatabaseReactor.desc(); }
  }

}
