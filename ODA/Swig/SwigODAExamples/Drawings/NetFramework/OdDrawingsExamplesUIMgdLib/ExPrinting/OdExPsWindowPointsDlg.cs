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
using System.ComponentModel;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Teigha.Core;

namespace OdDrawingsExamplesUIMgdLib.ExPrinting
{
  public partial class OdExPsWindowPointsDlg : Form
  {
    OdGePoint3d pt1, pt2;

    public OdExPsWindowPointsDlg()
    {
      InitializeComponent();
      pt1 = new OdGePoint3d();
      pt2 = new OdGePoint3d();
    }

    private void buttonOK_Click(object sender, EventArgs e)
    {
      try
      {
        string[] args = this.textPoint1.Text.Split(',', ' ', ';', '.');
        pt1.x = Double.Parse(args[0]);
        pt1.y = Double.Parse(args[1]);
        if (args.Length > 2)
          pt1.z = Double.Parse(args[2]);

        string[] args2 = this.textPoint2.Text.Split(',', ' ', ';', '.');
        pt2.x = Double.Parse(args2[0]);
        pt2.y = Double.Parse(args2[1]);
        if (args2.Length > 2)
          pt2.z = Double.Parse(args2[2]);
      }
      catch (Exception ex)
      {
        this.buttonCancel.Enabled = true;
      }
    }

    public void getPoints(out OdGePoint3d point1, out OdGePoint3d point2)
    {
      point1 = pt1;
      point2 = pt2;
    }
  }
}
