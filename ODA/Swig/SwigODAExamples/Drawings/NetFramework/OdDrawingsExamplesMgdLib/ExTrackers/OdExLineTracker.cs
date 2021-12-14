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
  public class OdExLineTracker : OdEdPointTracker
  {
    public OdDbLine TargetLine { get; private set; }
    private OdDbText m_pText { get; set; }

    public OdExLineTracker(OdDbLine pLine)
    {
      TargetLine = pLine;
      m_pText = OdDbText.createObject();
      m_pText.setDatabaseDefaults(pLine.database());
      m_pText.setNormal(pLine.normal());
      OdCmColor Color = new OdCmColor();
      Color.setColorMethod(OdCmEntityColor.Items_ColorMethod.kByColor);
      Color.setGreen(200);
      m_pText.setColor(Color);
    }

    public void update()
    {
      double distance = new OdGeVector3d(TargetLine.endPoint() - TargetLine.startPoint()).length();
      m_pText.setTextString(Math.Round(distance, 5).ToString());

      OdGeExtents3d Extents = new OdGeExtents3d();
      m_pText.getGeomExtents(Extents);
      double w = Extents.maxPoint().x - Extents.minPoint().x;
      m_pText.setPosition(TargetLine.endPoint() + OdGeVector3d.kXAxis * w / 2);
    }

    // Add the dragged circle to the view
    public override int addDrawables(OdGsView pView)
    {
      pView.add(TargetLine, null);
      pView.add(m_pText, null);

      OdGePoint2d pixelDensity = new OdGePoint2d();
      pView.getNumPixelsInUnitSquare(new OdGePoint3d(), pixelDensity);
      m_pText.setHeight(10.0 / pixelDensity.x);
      return 1;
    }

    // Remove drawables from the view
    public override void removeDrawables(OdGsView pView)
    {
      pView.erase(TargetLine);
      pView.erase(m_pText);
    }

    public override void setValue(OdGePoint3d value)
    {
      TargetLine.setEndPoint(value);
      update();
    }
  }
}
