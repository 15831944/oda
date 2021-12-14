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


using OdDrawingsExamplesMgdLib.ExJigs;
using OdDrawingsExamplesMgdLib.ExTrackers;
using System.Windows.Controls;
using Teigha.Core;
using Teigha.TD;

namespace OdDrawingsExamplesUIMgdLib.ExJigs.Wpf
{
  public class OdExCircleRadiusJigWpf : OdExJigWpf
  {
    public OdExCircleRadiusJigWpf(UserControl targetControl, OdExCircleRadiusJig jig)
      : base(targetControl, jig)
    { }

    protected override void Init()
    {
      this.m_targetControl.MouseDown += OnMouseDown;
      this.m_targetControl.MouseMove += OnMouseMove;
    }

    protected override void UnInit()
    {
      this.m_targetControl.MouseDown -= OnMouseDown;
      this.m_targetControl.MouseMove -= OnMouseMove;
    }
  }

}