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

namespace OdDrawingsExamplesMgdLib.ExTrackers
{
  public class OdExStringTracker : OdEdStringTracker
  {
    public OdDbText TargetText { get; private set; }
    private OdDbText m_pCursor { get; set; }
    private bool m_bCursor { get; set; }

    public OdExStringTracker(OdDbText pText) : base(IntPtr.Zero, false)
    {
      TargetText = pText;
      m_bCursor = false;
      m_pCursor = pText.clone() as OdDbText;
      m_pCursor.setHorizontalMode(TextHorzMode.kTextLeft);
      m_pCursor.setVerticalMode(TextVertMode.kTextBottom);
    }

    public override void setValue(String value)
    {
      TargetText.setTextString(value);
      TargetText.adjustAlignment();

      m_pCursor.setTextString(m_bCursor ? "I" : String.Empty);

      if (m_bCursor)
      {
        OdGePoint3dArray bounds = new OdGePoint3dArray();
        TargetText.getBoundingPoints(bounds);
        m_pCursor.setPosition(bounds[3]);
      }
      m_bCursor = !m_bCursor;
    }

    public override void setCursor(bool bShowCursor)
    {
      m_pCursor.setTextString(bShowCursor ? "I" : String.Empty);
      if (m_bCursor = bShowCursor)
      {
        OdGePoint3dArray bounds = new OdGePoint3dArray();
        TargetText.getBoundingPoints(bounds);
        m_pCursor.setHeight(TargetText.height());
        m_pCursor.setPosition(bounds[3]);
      }
    }

    public override int addDrawables(OdGsView pView)
    {
      pView.add(TargetText, null);
      pView.add(m_pCursor, null);
      return 1;
    }

    public override void removeDrawables(OdGsView pView)
    {
      pView.erase(m_pCursor);
      pView.erase(TargetText);
    }
  }
}
