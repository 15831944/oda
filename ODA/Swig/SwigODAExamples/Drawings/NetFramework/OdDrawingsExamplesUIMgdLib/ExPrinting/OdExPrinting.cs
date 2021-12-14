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
using System.Windows.Forms;
using System.Drawing.Printing;
using Teigha.Core;
using Teigha.TD;
using System.Drawing;
using System.IO;
using OdDrawingsExamplesMgdLib;

namespace OdDrawingsExamplesUIMgdLib.ExPrinting
{
  public class OdExPrinting
  {
    OdGsLayoutHelper m_pPrinterDevice = null;
    OdDbDatabase database = null;
    OdGsView pViewDr = null;

    public static void Swap<T>(T a, T b)
    {
      T temp = a;
      a = b;
      b = temp;
    }

    #region PrintTransaction

    MemoryTransaction manTr = null;
    private void InitPrintMemoryTransaction()
    {
      manTr = MemoryManager.GetMemoryManager().StartTransaction();
    }

    private void UnInitFormMemoryTransaction()
    {
      if (manTr != null)
        MemoryManager.GetMemoryManager().StopTransaction(manTr);
    }
    #endregion

    public void Print(OdDbDatabase db, OdGsView pView, bool bPreview)
    {
      InitPrintMemoryTransaction();
      database = db;
      pViewDr = pView;
      using (OdDbBlockTableRecord btr = (OdDbBlockTableRecord)database.getActiveLayoutBTRId().safeOpenObject(OpenMode.kForRead))
      {
        using (OdDbLayout pLayout = (OdDbLayout)btr.getLayoutId().safeOpenObject(OpenMode.kForRead))
        {
          PrintDocument prDoc = new PrintDocument();
          prDoc.PrintPage += new PrintPageEventHandler(this.PrintPage);
          prDoc.OriginAtMargins = true;
          prDoc.PrinterSettings.PrinterName = pLayout.getPlotCfgName();

          PageSettings pageSetting = prDoc.DefaultPageSettings;
          Teigha.TD.OdDbPlotSettings.PlotRotation rotation = pLayout.plotRotation();
          if (rotation == Teigha.TD.OdDbPlotSettings.PlotRotation.k90degrees || rotation == Teigha.TD.OdDbPlotSettings.PlotRotation.k270degrees)
            pageSetting.Landscape = true;
          else
            pageSetting.Landscape = false;

          Double kMmPerInch = 10 / 2.54;
          Double dPaperWidth = 0;
          Double dPaperHeight = 0;
          pLayout.getPlotPaperSize(out dPaperWidth, out dPaperHeight);
          String sCanonicalMediaName = pLayout.getCanonicalMediaName();
          PaperSize ps = new PaperSize(sCanonicalMediaName, (int)(dPaperWidth * kMmPerInch), (int)(dPaperHeight * kMmPerInch));
          pageSetting.PaperSize = ps;

          //default as OdaMfc
          pageSetting.Margins.Left = 0;
          pageSetting.Margins.Right = 0;
          pageSetting.Margins.Top = 0;
          pageSetting.Margins.Bottom = 0;

          prDoc.DefaultPageSettings = pageSetting;

          PrintPreviewDialog printPreviewDialog = new PrintPreviewDialog();

          if (prDoc.PrinterSettings.IsValid)
          {
            InitBitmapGsDevice();
            if (bPreview)
            {
              printPreviewDialog.Document = prDoc;
              printPreviewDialog.ShowDialog();
            }
            else
            {
              prDoc.Print();
            }

            if (m_pPrinterDevice != null)
              m_pPrinterDevice.Dispose();
            database = null;
            pViewDr = null;
          }
        }
      }
      UnInitFormMemoryTransaction();
    }

    private void InitBitmapGsDevice()
    {
      try
      {
        // Load the vectorization module
        OdGsModule pGs = (OdGsModule)Globals.odrxDynamicLinker().loadModule("WinOpenGL.txv", false);
        // Create a new OdGsDevice object, and associate with the vectorization GsDevice
        OdGsDevice pDevice = pGs.createBitmapDevice();
        // Return a pointer to the dictionary entity containing the device properties
        OdRxDictionary Properties = pDevice.properties();
        if (Properties != null)
        {
          if (Properties.has("DoubleBufferEnabled")) // Check if property is supported
            Properties.putAt("DoubleBufferEnabled", new OdRxVariantValue(true));
          if (Properties.has("EnableSoftwareHLR")) // Check if property is supported
            Properties.putAt("EnableSoftwareHLR", new OdRxVariantValue(true));
          if (Properties.has("DiscardBackFaces")) // Check if property is supported
            Properties.putAt("DiscardBackFaces", new OdRxVariantValue(true));
        }

        OdGiContextForDbDatabase _ctx = OdGiContextForDbDatabase.createObject();

        _ctx.setPlotGeneration(true);

        _ctx.enableGsModel(true);

        _ctx.setDatabase(database);

        m_pPrinterDevice = TD_Db.setupActiveLayoutViews(pDevice, _ctx);

        // Set the device background color and palette
        uint[] CurPalette = Teigha.Core.AllPalettes.getLightPalette();
        m_pPrinterDevice.setBackgroundColor(CurPalette[0]);
        m_pPrinterDevice.setLogicalPalette(CurPalette, 256);
        _ctx.setPaletteBackground(CurPalette[0]);
        bool acceptPlotStyles = OdExAuxUtilities.preparePlotstyles(database, _ctx);

        //Set render mode
        if (!acceptPlotStyles)
          m_pPrinterDevice.activeView().setMode(OdGsView.RenderMode.kFlatShadedWithWireframe);
        else
        {
          //Plottstyles apply only to 2dWireframe, Legacy Wireframe and Legacy HiddenLine.
          m_pPrinterDevice.activeView().setMode(OdGsView.RenderMode.k2DOptimized);
        }
      }
      catch (System.Exception ex)
      {
        MessageBox.Show(ex.ToString());
      }
    }

    void PrintPage(Object sender, PrintPageEventArgs ev)
    {
      if (m_pPrinterDevice == null)
        return;

      MemoryTransaction localTrans = MemoryManager.GetMemoryManager().StartTransaction();

      using (OdDbObject pVpObj = database.activeViewportId().openObject(OpenMode.kForWrite))
      {
        using (OdDbAbstractViewportData pAV = OdDbAbstractViewportData.cast(pVpObj))
        {
          OdGsView pGSView = pAV.gsView(pVpObj);

          PrintDocument prDoc = (PrintDocument)sender;

          Double dPrinterWidth = ev.PageBounds.Width;
          Double dPrinterHeight = ev.PageBounds.Height;
          Double dLogPixelX = ev.Graphics.DpiX; //dot per inch
          Double dLogPixelY = ev.Graphics.DpiY; //dot per inch
          Double kMmPerInch = 25.4;
          Double kMmPerHInch = 0.254;

          // Get Layout info
          OdDbLayout pLayout = (OdDbLayout)m_pPrinterDevice.layoutId().openObject(OpenMode.kForRead);
          Boolean bScaledToFit = pLayout.useStandardScale() && (Teigha.TD.OdDbPlotSettings.StdScaleType.kScaleToFit == pLayout.stdScaleType());
          Boolean bCentered = pLayout.plotCentered();
          Boolean bMetric = (pLayout.plotPaperUnits() != Teigha.TD.OdDbPlotSettings.PlotPaperUnits.kInches);
          Boolean bPrintLW = pLayout.printLineweights() || pLayout.showPlotStyles();

          Double offsetX, offsetY;// in mm
          pLayout.getPlotOrigin(out offsetX, out offsetY); // in mm
          OdGePoint2d offsets = new OdGePoint2d(offsetX, offsetY);

          Double dLeftMargin, dRightMargin, dBottomMargin, dTopMargin; // in mm
          pLayout.getPlotPaperMargins(out dLeftMargin, out dBottomMargin, out dRightMargin, out dTopMargin);
          Teigha.TD.OdDbPlotSettings.PlotType plotType = pLayout.plotType();

          Teigha.TD.OdDbPlotSettings.PlotRotation plotRotation = pLayout.plotRotation();
          if (plotRotation == Teigha.TD.OdDbPlotSettings.PlotRotation.k90degrees || plotRotation == Teigha.TD.OdDbPlotSettings.PlotRotation.k270degrees)
          {
            plotRotation = (plotRotation == Teigha.TD.OdDbPlotSettings.PlotRotation.k90degrees) ? Teigha.TD.OdDbPlotSettings.PlotRotation.k270degrees : Teigha.TD.OdDbPlotSettings.PlotRotation.k90degrees;
          }

          //set LineWeight scale factor for model space
          if (bPrintLW && database.getTILEMODE())
          {
            OdGsView pTo = m_pPrinterDevice.activeView();
            pTo.setLineweightToDcScale(Math.Max(dLogPixelX, dLogPixelY) / kMmPerInch * 0.001);
          }

          // Apply paper rotation to paper parameters
          switch (plotRotation)
          {
            case Teigha.TD.OdDbPlotSettings.PlotRotation.k90degrees:
              Swap<Double>(dTopMargin, dRightMargin);
              Swap<Double>(dBottomMargin, dLeftMargin);
              Swap<Double>(dBottomMargin, dTopMargin);
              Swap<Double>(dTopMargin, dRightMargin);
              offsets = new OdGePoint2d(-offsets.x, -offsets.y);
              break;
            case Teigha.TD.OdDbPlotSettings.PlotRotation.k180degrees:
              Swap<Double>(dRightMargin, dLeftMargin);
              offsets = new OdGePoint2d(-offsets.x, -offsets.y);
              break;
            case Teigha.TD.OdDbPlotSettings.PlotRotation.k270degrees:
              Swap<Double>(dTopMargin, dRightMargin);
              Swap<Double>(dBottomMargin, dLeftMargin);
              Swap<Double>(dBottomMargin, dTopMargin);
              offsets = new OdGePoint2d(offsets.x, offsets.y);
              break;
          }

          // Get scale factor
          double dFactor;
          if (pLayout.useStandardScale())
          {
            pLayout.getStdScale(out dFactor);
          }
          else
          {
            double numerator, denominator;
            pLayout.getCustomPrintScale(out numerator, out denominator);
            dFactor = numerator / denominator;
          }

          // Calculate paper drawable area using margins from layout (in mm).
          Double drx1 = (ev.MarginBounds.Left * kMmPerHInch + dLeftMargin);                // in mm
          Double drx2 = (ev.MarginBounds.Width * kMmPerHInch - dLeftMargin - dRightMargin); // in mm
          Double dry1 = (ev.MarginBounds.Top * kMmPerHInch + dTopMargin);                // in mm
          Double dry2 = (ev.MarginBounds.Height * kMmPerHInch - dTopMargin - dBottomMargin); // in mm

          // get info from view, viewport .... etc
          Boolean bType = (plotType == Teigha.TD.OdDbPlotSettings.PlotType.kDisplay || plotType == Teigha.TD.OdDbPlotSettings.PlotType.kLayout);
          OdAbstractViewPE pAbstractViewPE = (bType) ? OdAbstractViewPE.cast(pViewDr) : OdAbstractViewPE.cast(pGSView);
          OdRxObject pVObject = (bType) ? pViewDr : pGSView;

          OdGePoint3d viewTarget = pAbstractViewPE.target(pVObject);
          OdGePoint3d viewportCenter = pAbstractViewPE.target(pVObject);       // in plotPaperUnits
          Boolean isPerspective = pAbstractViewPE.isPerspective(pVObject);
          Double viewportH = pAbstractViewPE.fieldHeight(pVObject);  // in plotPaperUnits
          Double viewportW = pAbstractViewPE.fieldWidth(pVObject);   // in plotPaperUnits
          OdGeVector3d viewDir = pAbstractViewPE.direction(pVObject);    // in plotPaperUnits
          OdGeVector3d upV = pAbstractViewPE.upVector(pVObject);     // in plotPaperUnits
          OdGeMatrix3d eyeToWorld = pAbstractViewPE.eyeToWorld(pVObject);
          OdGeMatrix3d WorldToeye = pAbstractViewPE.worldToEye(pVObject);

          Boolean isPlanView = viewDir.normal().Equals(OdGeVector3d.kZAxis);

          OdGePoint3d oldTarget = viewTarget;

          Double fieldWidth = viewportW, fieldHeight = viewportH;

          if (plotType == Teigha.TD.OdDbPlotSettings.PlotType.kWindow || (plotType == Teigha.TD.OdDbPlotSettings.PlotType.kLimits && isPlanView))
          {
            Double xmin, ymin, xmax, ymax;
            if (plotType == Teigha.TD.OdDbPlotSettings.PlotType.kWindow)
            {
              pLayout.getPlotWindowArea(out xmin, out ymin, out xmax, out ymax);
            }
            else
            {
              xmin = database.getLIMMIN().x;
              ymin = database.getLIMMIN().y;
              xmax = database.getLIMMAX().x;
              ymax = database.getLIMMAX().y;
            }

            fieldWidth = xmax - xmin;
            fieldHeight = ymax - ymin;

            OdGeVector3d tmp = viewportCenter - viewTarget;
            viewTarget.set((xmin + xmax) / 2, (ymin + ymax) / 2, 0);
            viewTarget.transformBy(eyeToWorld);
            viewTarget -= tmp;
          }
          else if (plotType == Teigha.TD.OdDbPlotSettings.PlotType.kDisplay)
          {
            viewTarget = viewportCenter;
            fieldWidth = viewportW;
            fieldHeight = viewportH;
          }
          else if (plotType == Teigha.TD.OdDbPlotSettings.PlotType.kExtents || (plotType == Teigha.TD.OdDbPlotSettings.PlotType.kLimits && !isPlanView))
          {
            OdGeBoundBlock3d extents = new OdGeBoundBlock3d();

            if (pAbstractViewPE.plotExtents(pVObject, extents)) // pIter also skip 'off layers'
            {
              extents.transformBy(eyeToWorld);
              viewTarget = (extents.minPoint() + extents.maxPoint().asVector()).Div(2.0);
              extents.transformBy(WorldToeye);

              fieldWidth = Math.Abs(extents.maxPoint().x - extents.minPoint().x);
              fieldHeight = Math.Abs(extents.maxPoint().y - extents.minPoint().y);
            }
          }
          else if (plotType == Teigha.TD.OdDbPlotSettings.PlotType.kView)
          {
            viewTarget = viewportCenter;
            fieldWidth = viewportW;
            fieldHeight = viewportH;
          }
          else if (plotType == Teigha.TD.OdDbPlotSettings.PlotType.kLimits)
          {
            fieldWidth = (drx2 - drx1) / dFactor; // drx in mm -> fieldWidth in mm
            fieldHeight = (dry2 - dry1) / dFactor;

            viewTarget = new OdGePoint3d(fieldWidth / 2.0 - offsets.x / dFactor, fieldHeight / 2.0 - offsets.y / dFactor, 0); // in mm
            if (!bMetric)
            {
              viewTarget = viewTarget.Div(kMmPerInch); // must be in plotpaper units
              fieldWidth /= kMmPerInch;
              fieldHeight /= kMmPerInch;
            }

            bCentered = bScaledToFit = false;       // kLayout doesn't support pIter.
          }

          if (plotType != Teigha.TD.OdDbPlotSettings.PlotType.kView)
            viewTarget = viewTarget.orthoProject(new OdGePlane(oldTarget, viewDir));


          pGSView.setView(viewTarget + viewDir,
                          viewTarget, upV,
                          fieldWidth, fieldHeight,
                          isPerspective ? OdGsView.Projection.kPerspective : OdGsView.Projection.kParallel);

          OdGsDCRect viewportRect;
          switch (plotRotation)
          {
            case Teigha.TD.OdDbPlotSettings.PlotRotation.k90degrees:
            case Teigha.TD.OdDbPlotSettings.PlotRotation.k180degrees:
              viewportRect = new OdGsDCRect(ev.PageBounds.Right, ev.PageBounds.Left, ev.PageBounds.Top, ev.PageBounds.Bottom);
              break;
            case Teigha.TD.OdDbPlotSettings.PlotRotation.k270degrees:
            case Teigha.TD.OdDbPlotSettings.PlotRotation.k0degrees:
            default:
              viewportRect = new OdGsDCRect(ev.PageBounds.Left, ev.PageBounds.Right, ev.PageBounds.Bottom, ev.PageBounds.Top);
              break;
          }

          if (plotType == OdDbPlotSettings.PlotType.kExtents)
          {
            OdGeBoundBlock3d bbox = new OdGeBoundBlock3d();
            bool bBboxValid = pAbstractViewPE.viewExtents((OdGsView)(pVObject), bbox);
            pAbstractViewPE.zoomExtents(pVObject, bbox);
          }

          m_pPrinterDevice.onSize(viewportRect);

          m_pPrinterDevice.update(viewportRect);

          OdGiRasterImage bitmap = null;

          m_pPrinterDevice.getSnapShot(out bitmap, viewportRect);

          MemoryStream binData = GetImageBinaryData(bitmap);

          Image img = Image.FromStream(binData);

          ev.Graphics.DrawImage(img, 0, 0, (float)img.Width, (float)img.Height);

          bitmap.Dispose();
          pAbstractViewPE.Dispose();
          pLayout.Dispose();
        }
      }

      MemoryManager.GetMemoryManager().StopTransaction(localTrans);
    }

    static MemoryStream GetImageBinaryData(OdGiRasterImage img)
    {
      // saving file as bmp BITMAPINFOHEADER
      // (let code be crossplatform)
      UInt32 imgWidth = img.pixelWidth();
      UInt32 imgHeight = img.pixelHeight();

      double xPelsPerUnut, yPelsPerUnut;
      img.defaultResolution(out xPelsPerUnut, out yPelsPerUnut);
      UInt16 colDepth = (UInt16)img.colorDepth();
      UInt32 palSz = img.paletteDataSize();
      UInt32 scLnSz = img.scanLineSize();
      UInt32 bmpScSize = OdGiRasterImage.calcBMPScanLineSize(imgWidth, colDepth);

      MemoryStream ms = new MemoryStream();
      BinaryWriter binWr = new BinaryWriter(ms);

      binWr.Write((Int16)0x4d42);

      UInt32 scanLinesPos = 14 + 40 + palSz;  // Below version is correct only on Windows
                                              //(OdUInt32)sizeof(BITMAPFILEHEADER) + (OdUInt32)sizeof(BITMAPINFOHEADER) + palSz;
      UInt32 size = scanLinesPos + bmpScSize * imgHeight;
      binWr.Write((Int32)size);
      binWr.Write((Int32)0); // reserved
      binWr.Write((Int32)scanLinesPos);// offBits
      binWr.Write((Int32)40); // not portable: sizeof(BITMAPINFOHEADER));

      // save BITMAPINFOHEADER
      binWr.Write((Int32)imgWidth);
      binWr.Write((Int32)imgHeight);
      binWr.Write((Int16)1);
      binWr.Write((Int16)colDepth);
      binWr.Write((Int32)0);
      binWr.Write((Int32)0);
      binWr.Write((Int32)xPelsPerUnut);
      binWr.Write((Int32)yPelsPerUnut);
      binWr.Write((Int32)0);
      binWr.Write((Int32)0);

      // palette
      byte[] palette = new byte[palSz];
      img.paletteData(ref palette);
      binWr.Write(palette);

      // scanLines
      byte[] pScLns = img.scanLines();
      if (pScLns.Length != 0 && (scLnSz == bmpScSize))
      {
        // we have pointer to data in bmp format
        binWr.Write(pScLns, 0, (int)(scLnSz * imgHeight));
      }
      else
      {
        byte[] scLn = new byte[bmpScSize];
        for (uint i = 0; i < imgHeight; i++)
        {
          img.scanLines(ref scLn, i);
          binWr.Write(scLn);
        }
      }

      binWr.Flush();

      return ms;
    }
  }
}
