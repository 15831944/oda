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
using OdDrawingsExamplesMgdLib.ExGripPoints;
using OdDrawingsExamplesMgdLib.ExSelection;
using OdKernelExamplesMgdLib;
using OdKernelExamplesMgdLib.ExView;
using System.Linq;

namespace OdDrawingsExamplesMgdLib.ExEditors
{
  public class OdExDrawingEditor : OdExViewPaintEventsInterafce, OdExViewEnableFunctionalityInterface
  {
    private enum OdExMouseMode
    {
      Quiescent,
      Selection,
      DragDrop,
    }

    private OdExMouseMode m_mouseMode;

    private OdExDrawingGripManager m_gripManager;
    private OdExViewSelectionObjectsByMouse m_mouseObjectSelection;

    private OdGsLayoutHelper m_GsLayoutHelper { get; set; }
    private OdDbDatabase m_targetDatabase { get; set; }

    private bool m_IsEnable = false;
    public bool IsEnable
    {
      get { return m_IsEnable; }
      set
      {
        m_IsEnable = value;
        ResetGripPointsSelection();
      }
    }

    public event OdExViewDelegates.PostPaintDelegate PostPaintEvent;
    public event OdExViewDelegates.PrePaintDelegate PrePaintEvent;

    public void Init(OdDbDatabase database, OdGsLayoutHelper layoutHelper)
    {
      m_GsLayoutHelper = layoutHelper;
      m_targetDatabase = database;

      m_mouseMode = OdExMouseMode.Quiescent;

      InitSelection();
      InitGripPoints();
    }

    public void UnInit()
    {
      UnInitGripPoints();
      UnInitSelection();

      m_GsLayoutHelper = null;
      m_targetDatabase = null;
    }

    #region Initialization/UnInitialization
    private void InitSelection()
    {
      m_mouseObjectSelection = new OdExViewSelectionObjectsByMouse();

      OdDbObjectId activeLayoutId = m_targetDatabase.getActiveLayoutBTRId();
      OdGsView activeView = m_GsLayoutHelper.activeView();
      short pickBox = m_targetDatabase.getSysVar("PICKBOX").getInt16();

      m_mouseObjectSelection.Init(activeLayoutId, activeView, pickBox);
    }
    private void UnInitSelection()
    {
      m_mouseObjectSelection.UnInit();
      m_mouseObjectSelection = null;
    }

    private void InitGripPoints()
    {
      m_gripManager = new OdExDrawingGripManager();
      m_gripManager.Init(m_GsLayoutHelper, m_GsLayoutHelper.gsModel(), m_targetDatabase);
    }

    private void UnInitGripPoints()
    {
      m_gripManager.UnInit();
      m_gripManager = null;
    }
    #endregion

    public void OnMouseDown(int mouseX, int mouseY, bool bShift)
    {
      if (!IsEnable)
        return;
      switch (m_mouseMode)
      {
        case OdExMouseMode.Quiescent:
          {
            bool isTriggerGripProcessing;
            bool isDragGripProcessing;

            if (m_gripManager.OnMouseDown(mouseX, mouseY, bShift, out isTriggerGripProcessing, out isDragGripProcessing))
            {
              if (isDragGripProcessing)
                m_mouseMode = OdExMouseMode.DragDrop;
              else if (isTriggerGripProcessing)
              {
                OdExHelpers.SafeCallDelegate(PrePaintEvent);
                OdExHelpers.SafeCallDelegate(PostPaintEvent);
              }
            }
            else
            {
              if (IsNeedMoveSelectedEntities(mouseX, mouseY) && bShift)
              {
                OdExHelpers.SafeCallDelegate(PrePaintEvent);
                if (m_gripManager.MoveAllSelected(mouseX, mouseY))
                  m_mouseMode = OdExMouseMode.DragDrop;
                OdExHelpers.SafeCallDelegate(PostPaintEvent);
              }
              else
              {
                switch (m_mouseObjectSelection.OnMouseDown(mouseX, mouseY))
                {
                  case OdExViewSelectionObjectsByMouse.OperationState.Processing:
                    m_mouseMode = OdExMouseMode.Selection;
                    OdExHelpers.SafeCallDelegate(PrePaintEvent);
                    OdExHelpers.SafeCallDelegate(PostPaintEvent);
                    break;
                  case OdExViewSelectionObjectsByMouse.OperationState.Finished:
                    OdExHelpers.SafeCallDelegate(PrePaintEvent);
                    ResetGripPointsSelection();
                    UpdateGripPointSelection();
                    OdExHelpers.SafeCallDelegate(PostPaintEvent);
                    m_mouseMode = OdExMouseMode.Quiescent; break;
                  default:
                    m_mouseMode = OdExMouseMode.Quiescent; break;
                }
              }
            }
            break;
          }
        case OdExMouseMode.DragDrop:
          {
            OdExHelpers.SafeCallDelegate(PrePaintEvent);
            m_gripManager.DragFinal(OdExGsDeviceHelpers.ScreenPointToWorld(m_GsLayoutHelper.activeView(), mouseX, mouseY), true);
            OdExHelpers.SafeCallDelegate(PostPaintEvent);
            m_mouseMode = OdExMouseMode.Quiescent;
            break;
          }
        default:
          break;
      }
    }

    public void OnMouseMove(int mouseX, int mouseY, bool bShift)
    {
      if (!IsEnable)
        return;
      switch (m_mouseMode)
      {
        case OdExMouseMode.Quiescent:
          {
            if (m_gripManager.OnMouseMove(mouseX, mouseY, bShift))
            {
              OdExHelpers.SafeCallDelegate(PrePaintEvent);
              OdExHelpers.SafeCallDelegate(PostPaintEvent);
            }
            break;
          }
        case OdExMouseMode.Selection:
          {
            OdExHelpers.SafeCallDelegate(PrePaintEvent);
            m_mouseObjectSelection.OnMouseMove(mouseX, mouseY);
            OdExHelpers.SafeCallDelegate(PostPaintEvent);
          }
          break;
        case OdExMouseMode.DragDrop:
          {
            OdExHelpers.SafeCallDelegate(PrePaintEvent);
            m_gripManager.SetValue(OdExGsDeviceHelpers.ScreenPointToWorld(m_GsLayoutHelper.activeView(), mouseX, mouseY));
            OdExHelpers.SafeCallDelegate(PostPaintEvent);
            break;
          }
      }
    }

    public void OnMouseUp(int mouseX, int mouseY)
    {
      if (!IsEnable)
        return;
      switch (m_mouseMode)
      {
        case OdExMouseMode.Selection:
          {
            OdExHelpers.SafeCallDelegate(PrePaintEvent);
            m_mouseObjectSelection.OnMouseUp(mouseX, mouseY);
            ResetGripPointsSelection();
            UpdateGripPointSelection();
            OdExHelpers.SafeCallDelegate(PostPaintEvent);
            m_mouseMode = OdExMouseMode.Quiescent;
            break;
          }
        //case OdExMouseMode.DragDrop:
        //  {
        //    var pView = m_GsLayoutHelper.activeView();
        //    {
        //      OdExHelpers.SafeCallDelegate(PrePaintEvent);
        //      m_gripManager.DragFinal(OdExGsDeviceHelpers.ScreenPointToWorld(pView, mouseX, mouseY), true);
        //      m_GsLayoutHelper.gsModel().invalidate(OdGsModel.InvalidationHint.kInvalidateAll);
        //      OdExHelpers.SafeCallDelegate(PostPaintEvent);
        //    }
        //    m_mouseMode = OdExMouseMode.Quiescent;
        //    break;
        //  }
        default:
          break;
      }
    }

    /// <summary>
    /// If mouse point contains in extents from overall entities method return true; otherwise false;
    /// </summary>
    /// <param name="xMouse"></param>
    /// <param name="yMouse"></param>
    /// <returns></returns>
    private bool IsNeedMoveSelectedEntities(int xMouse, int yMouse)
    {
      OdDbEntity[] selectedEntities = m_mouseObjectSelection.LastSelectedIds.Select(x => OdDbEntity.cast(x.openObject(OpenMode.kForRead))).Where(x => x != null).ToArray();
      OdGeExtents3d extents = OdExAuxUtilities.CalcExtents(selectedEntities);
      OdGePoint3d mouseLocation = OdExGsDeviceHelpers.GetWorldCoordinateByScreen(m_GsLayoutHelper.activeView(), xMouse, yMouse);
      bool needMove = extents.isValidExtents() ? extents.contains(mouseLocation, new OdGeTol(10E-6)) : false;
      return needMove;
    }

    private void ResetGripPointsSelection()
    {
      if (m_gripManager == null)
        return;

      m_gripManager.ResetSelection();
    }

    private void UpdateGripPointSelection()
    {
      m_gripManager.UpdateSelection(m_mouseObjectSelection.LastSelectedIds);
    }

  }

}