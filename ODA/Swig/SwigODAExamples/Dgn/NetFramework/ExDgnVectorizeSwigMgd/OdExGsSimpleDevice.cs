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

namespace ExDgnVectorizeMgd
{
  class OdExGsSimpleDevice : OdGsBaseVectorizeDevice
  {
    private OdGiConveyorGeometryDumper m_pDestGeometry;
    private OdExGiDumper m_pDumper = new OdExGiDumperImpl();
    private List<OdExSimpleView> views = new List<OdExSimpleView>();
    public OdExGsSimpleDevice()
    {
      m_pDestGeometry = new OdGiConveyorGeometryDumper(m_pDumper);
    }

    public override OdGsView createView(OdGsClientViewInfo pInfo, bool bEnableLayerVisibilityPerView)
    {
      OdExSimpleVectorizer pVect = new OdExSimpleVectorizer(this);
      OdExSimpleView pView = new OdExSimpleView(this);
      pVect._view = pView;
      pVect.setUp(pView);
      // save reference, so that GC would not reclaim it
      views.Add(pView);

      pView.init(this, pInfo, bEnableLayerVisibilityPerView);

      /**********************************************************************/
      /* Directs the output geometry for the view to the                    */
      /* destination geometry object                                        */
      /**********************************************************************/

      pVect.output().setDestGeometry(m_pDestGeometry);

      return pVect.gsView();
    }

    public OdGiConveyorGeometry destGeometry { get { return m_pDestGeometry; } }
    public OdExGiDumper dumper { get { return m_pDumper; } }

    /************************************************************************/
    /* Called by the ODA Drawings.NET vectorization framework to update     */
    /* the GUI window for this Device object                                */
    /*                                                                      */
    /* pUpdatedRect specifies a rectangle to receive the region updated     */
    /* by this function.                                                    */
    /*                                                                      */
    /* The override should call the parent version of this function,        */
    /* OdGsBaseVectorizeDevice::update().                                   */
    /************************************************************************/
    /*    public override void update(OdGsDCRect pUpdatedRect)
        {
          base.update(pUpdatedRect);
        }*/
    UInt32 LOBYTE(UInt32 w) { return (w & 0xff); }
    UInt32 GetRValue(UInt32 rgb) { return (LOBYTE(rgb)); }
    UInt32 GetGValue(UInt32 rgb) { return (LOBYTE(((rgb)) >> 8)); }
    UInt32 GetBValue(UInt32 rgb) { return (LOBYTE((rgb) >> 16)); }

    /************************************************************************/
    /* Called by each associated view object to set the current RGB draw    */
    /* color.                                                               */
    /************************************************************************/
    public void draw_color(UInt32 color)
    {
      String retVal = "r" + GetRValue(color).ToString() + ":g"
        + GetGValue(color).ToString() + ":b" + GetBValue(color).ToString();
      dumper.output("draw_color", retVal);
    }

    /************************************************************************/
    /* Called by each associated view object to set the current RGB draw    */
    /* color.                                                               */
    /************************************************************************/
    public void draw_color(OdCmEntityColor color)
    {
      String retVal = "r" + color.red().ToString() + ":g"
        + color.green().ToString() + ":b" + color.blue().ToString();
      dumper.output("draw_color", retVal);
    }

    /************************************************************************/
    /* Called by each associated view object to set the current ACI draw    */
    /* color.                                                               */
    /************************************************************************/
    public void draw_color_index(UInt16 colorIndex)
    {
      dumper.output("draw_color_index", colorIndex.ToString());
    }

    /************************************************************************/
    /* Called by each associated view object to set the current draw        */
    /* lineweight and pixel width                                           */
    /************************************************************************/
    public void draw_lineWidth(LineWeight weight, int pixelLineWidth)
    {
      dumper.output("draw_lineWidth");
      dumper.pushIndent();
      dumper.output("weight", weight.ToString());
      dumper.output("pixelLineWidth", pixelLineWidth.ToString());
      dumper.popIndent();
    }

    /************************************************************************/
    /* Called by each associated view object to set the current Fill Mode   */
    /************************************************************************/
    public void draw_fill_mode(OdGiFillType fillMode)
    {
      switch (fillMode)
      {
        case OdGiFillType.kOdGiFillAlways:
          dumper.output("draw_fill_mode", "FillAlways");
          break;
        case OdGiFillType.kOdGiFillNever:
          dumper.output("draw_fill_mode", "FillNever");
          break;
      }
    }
    public void setupSimplifier(OdGiDeviation pDeviation)
    {
      m_pDestGeometry.setDeviation(pDeviation);
    }
  }
}