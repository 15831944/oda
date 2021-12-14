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


using Teigha.Core;
using Teigha.TD;

namespace OdDrawingsExamplesMgdLib.ExSelection
{
  public class OdExViewSelectionObjectsByMouse
  {
    public enum OperationState
    {
      None,
      Finished,
      Processing
    }

    private OdDbObjectIdArray m_selected { get; set; }

    private bool m_selectByRectange = false;

    public OdExViewSelectionObjectsByMouse()
    {
      m_selectionByRectangle = new OdExViewSelectionObjectByRectangle();
      m_selectionByPoint = new OdExViewSelectionObjectByPoint();
      m_selected = new OdDbObjectIdArray();
    }

    private OdExViewSelectionObjectByRectangle m_selectionByRectangle { get; set; }
    private OdExViewSelectionObjectByPoint m_selectionByPoint { get; set; }

    public void Init(OdDbObjectId spaceId, OdGsView view, short pickBox = 1)
    {
      m_selectionByRectangle.Init(spaceId, view);
      m_selectionByPoint.Init(spaceId, view, pickBox);
    }

    public void UnInit()
    {
      m_selectionByRectangle.UnInit();
      m_selectionByPoint.UnInit();
    }

    public OdDbObjectIdArray LastSelectedIds { get { return new OdDbObjectIdArray(m_selected); } }

    public OperationState OnMouseDown(int xMouse, int yMouse)
    {
      m_selected.Clear();
      return OnMouseDown(xMouse, yMouse, m_selected);
    }

    public void OnMouseUp(int xMouse, int yMouse)
    {
      OnMouseUp(xMouse, yMouse, m_selected);
    }

    public void OnMouseMove(int xMouse, int yMouse)
    {
      if (m_selectByRectange)
      {
        if (m_selectionByRectangle.ChangeSelectionRectangle(xMouse, yMouse))
          return;
      }
    }

    private OperationState OnMouseDown(int xMouse, int yMouse, OdDbObjectIdArray selectedObjects)
    {
      if (m_selectionByPoint.Select(xMouse, yMouse, selectedObjects))
        return OperationState.Finished;
      else if (m_selectionByRectangle.StartSelect(xMouse, yMouse))
      {
        m_selectByRectange = true;
        return OperationState.Processing;
      }
      return OperationState.None;
    }

    private void OnMouseUp(int xMouse, int yMouse, OdDbObjectIdArray selectedObjects)
    {
      if (m_selectByRectange)
      {
        if (m_selectionByRectangle.FinishSelect(xMouse, yMouse, selectedObjects))
        {
          m_selectByRectange = false;
          return;
        }
      }
    }
  }

}