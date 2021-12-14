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

namespace OdDrawingsExamplesMgdLib.ExSelection
{
  public class OdExViewSelectionObjectByPoint
  {
    private OdGsView m_targetView { get; set; }
    private OdDbObjectId m_spaceId { get; set; }
    private short m_pickBox { get; set; }

    public OdExViewSelectionObjectByPoint()
    {
      m_pickBox = 1;
    }

    public void Init(OdDbObjectId spaceId, OdGsView view, short pickBox = 1)
    {
      m_targetView = view;
      m_spaceId = spaceId;
      m_pickBox = pickBox;
    }

    public void UnInit()
    {
      m_targetView = null;
      m_spaceId = null;
    }

    public bool Select(int xMouse, int yMouse, OdDbObjectIdArray selectedObjects)
    {
      CheckInitialization();

      OdGsDCPoint[] findPoints = new OdGsDCPoint[]
      {
            new OdGsDCPoint(xMouse - m_pickBox, yMouse - m_pickBox),
            new OdGsDCPoint(xMouse + m_pickBox, yMouse + m_pickBox)
      };

      OdExTopLevelSelectionReactor selectionReactor = new OdExTopLevelSelectionReactor(selectedObjects, m_spaceId);
      m_targetView.select(findPoints, selectionReactor, OdGsView.SelectionMode.kCrossing);

      if (selectedObjects.Count == 0)
        return false;
      return true;
    }

    private void CheckInitialization()
    {
      if (m_targetView == null || m_spaceId == null)
        throw new Exception("OdExViewSelectionObjectsByMouse not initialized");
    }
  }

}