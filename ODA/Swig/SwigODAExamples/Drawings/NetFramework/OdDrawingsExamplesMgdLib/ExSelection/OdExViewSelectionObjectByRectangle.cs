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


using OdKernelExamplesMgdLib.ExSelection;
using OdKernelExamplesMgdLib.ExView;
using System;
using Teigha.Core;
using Teigha.TD;

namespace OdDrawingsExamplesMgdLib.ExSelection
{
  public class OdExViewSelectionObjectByRectangle
  {
    private OdGsView m_targetView;
    private OdDbObjectId m_spaceId;

    private OdGsDCPoint startSelectionPoint;
    private OdGsDCPoint endSelectionPoint;
    private OdExSelectionRectangleFrame selectionRectangle;

    private bool IsStarted { get { return startSelectionPoint != null; } }

    public void Init(OdDbObjectId spaceId, OdGsView view)
    {
      m_targetView = view;
      m_spaceId = spaceId;
    }

    public void UnInit()
    {
      m_targetView = null;
      m_spaceId = null;
      InterruptSelection();
    }

    public bool StartSelect(int xMouse, int yMouse)
    {
      startSelectionPoint = new OdGsDCPoint(xMouse, yMouse);
      selectionRectangle = new OdExSelectionRectangleFrame(OdExGsDeviceHelpers.GetWorldCoordinateByScreen(m_targetView, xMouse, yMouse));
      selectionRectangle.AddToView(m_targetView);
      return true;
    }

    public bool ChangeSelectionRectangle(int xMouse, int yMouse)
    {
      if (!IsStarted)
        return false;

      selectionRectangle.setValue(OdExGsDeviceHelpers.GetWorldCoordinateByScreen(m_targetView, new OdGePoint2d(xMouse, yMouse)));

      return true;
    }

    public bool FinishSelect(int xMouse, int yMouse, OdDbObjectIdArray selectedObjects)
    {
      if (!IsStarted)
        return false;

      CheckInitialization();

      endSelectionPoint = new OdGsDCPoint(xMouse, yMouse);

      OdGsDCPoint[] findPoints = new OdGsDCPoint[]
      {
          startSelectionPoint,
          endSelectionPoint
      };

      OdExTopLevelSelectionReactor selectionReactor = new OdExTopLevelSelectionReactor(selectedObjects, m_spaceId);
      m_targetView.select(findPoints, selectionReactor, startSelectionPoint.x < endSelectionPoint.x ? OdGsView.SelectionMode.kWindow : OdGsView.SelectionMode.kCrossing);

      InterruptSelection();

      return true;
    }

    private void CheckInitialization()
    {
      if (m_targetView == null || m_spaceId == null)
        throw new Exception("OdExViewSelectionObjectsByMouse not initialized");
    }

    private void InterruptSelection()
    {
      if (selectionRectangle != null && m_targetView != null)
        m_targetView.erase(selectionRectangle);

      startSelectionPoint = null;
      endSelectionPoint = null;

      selectionRectangle = null;
    }
  }

}