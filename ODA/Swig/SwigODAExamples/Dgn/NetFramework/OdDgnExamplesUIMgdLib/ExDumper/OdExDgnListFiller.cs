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
using System.Collections.Generic;
using System.Text;
using Teigha.Core;
using Teigha.TG;
using System.Windows.Forms;
using OdDgnExamplesMgdLib.ExDumper;

namespace OdDgnExamplesUIMgdLib.ExDumper
{
  public class OdExDgnListFiller : OdExDgnDumper
  {
    private ListView m_ObjList;
    private OdDgDatabase m_Database;

    public OdExDgnListFiller(OdDgDatabase pDb, ListView pObjList)
    {
      m_ObjList = pObjList;
      m_Database = pDb;
    }
    public override void dumpFieldName(string fieldName)
    {
      m_ObjList.Items.Add(fieldName);
    }
    public override void dumpFieldValue(string fieldValue)
    {
      m_ObjList.Items[m_ObjList.Items.Count - 1].SubItems.Add(fieldValue);
    }
    public void dump(OdRxObject pObj)
    {
      OdDgElementDumperPE pDumper = OdDgRxObjectDumperPE.getDumper(pObj.isA()) as OdDgElementDumperPE;
      if (pDumper != null)
        pDumper.dump(pObj, this);
    }
  };

}