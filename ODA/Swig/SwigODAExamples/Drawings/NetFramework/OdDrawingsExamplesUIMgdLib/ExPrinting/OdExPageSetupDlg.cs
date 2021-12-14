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
using System.Text;
using System.Windows.Forms;
using Teigha.Core;
using Teigha.TD;

namespace OdDrawingsExamplesUIMgdLib.ExPrinting
{
  public partial class OdExPageSetupDlg : Form
  {
    OdDbPlotSettings m_params;
    OdDbPlotSettingsValidator m_pValidator;
    double offsetX;
    double offsetY;

    public OdExPageSetupDlg(OdDbPlotSettings inParams)
    {
      m_params = inParams;
      InitializeComponent();
      initValues();
      OnInitDialog();
    }

    #region Plot Scale Setting
    struct PlotScaleSetting
    {
      public String m_Str;
      public double m_dRealWorldUnits;
      public double m_dDrawingUnits;
      public int m_ScaleType;
      public double m_dScaleFactor;

      public PlotScaleSetting(String str, double RWU, double DrU, int scaleType, double scaleFactor)
      {
        m_Str = str;
        m_dRealWorldUnits = RWU;
        m_dDrawingUnits = DrU;
        m_ScaleType = scaleType;
        m_dScaleFactor = scaleFactor;
      }
    }

    static PlotScaleSetting[] plotScaleSetting = new PlotScaleSetting[]
    {
                new PlotScaleSetting(("Custom"),      1,     1,    0,  1),                   //    ("Scaled to Fit"),  //    kScaleToFit
				 new PlotScaleSetting(("1/128\" = 1'"),1,     1536, 1,  0.0006510416666667),  //    ("1/128\" = 1'"),   //    k1_128in_1ft
				 new PlotScaleSetting(("1/64\" = 1'"), 1,     768,  2,  0.0013020833333333),  //    ("1/64\" = 1'"),    //    k1_64in_1ft
				 new PlotScaleSetting(("1/32\" = 1'"), 1,     384,  3,  0.0026041666666667),  //    ("1/32\" = 1'"),    //    k1_32in_1ft
				 new PlotScaleSetting(("1/16\" = 1'"), 1,     192,  4,  0.0052083333333333),  //    ("1/16\" = 1'"),    //    k1_16in_1ft
				 new PlotScaleSetting(("3/32\" = 1'"), 1,     128,  5,  0.0078125),           //    ("3/32\" = 1'"),    //    k3_32in_1ft
				 new PlotScaleSetting(("1/8\" = 1'"),  1,     96,   6,  0.0104166666666667),  //    ("1/8\" = 1'"),     //    k1_8in_1ft,
				 new PlotScaleSetting(("3/16\" = 1'"), 1,     64,   7,  0.015625),            //    ("3/16\" = 1'"),    //    k3_16in_1ft
				 new PlotScaleSetting(("1/4\" = 1'"),  1,     48,   8,  0.0208333333333333),   //   ("1/4\" = 1'"),     //    k1_4in_1ft,
				 new PlotScaleSetting(("3/8\" = 1'"),  1,     32,   9,  0.03125),             //    ("3/8\" = 1'"),     //    k3_8in_1ft,
				 new PlotScaleSetting(("1/2\" = 1'"),  1,     24,   10, 0.0416666666666667),  //    ("1/2\" = 1'"),     //    k1_2in_1ft,
				 new PlotScaleSetting(("3/4\" = 1'"),  1,     16,   11, 0.0625),              //    ("3/4\" = 1'"),     //    k3_4in_1ft,
				 new PlotScaleSetting(("1\" = 1'"),    1,     12,   12, 0.0833333333333333),  //    ("1\" = 1'"),       //    k1in_1ft,
				 new PlotScaleSetting(("3\" = 1'"),    1,     4,    13, 0.25),                //    ("3\" = 1'"),       //    k3in_1ft,
				 new PlotScaleSetting(("6\" = 1'"),    1,     2,    14, 0.5),                 //    ("6\" = 1'"),       //    k6in_1ft,
				 new PlotScaleSetting(("1' = 1'"),     1,     1,    15, 1),                   //    ("1' = 1'"),        //    k1ft_1ft,
				 new PlotScaleSetting(("1:1"),         1,     1,    16, 1),                   //    ("1:1"),            //    k1_1,
				 new PlotScaleSetting(("1:2"),         1,     2,    17, 0.5),                 //    ("1:2"),            //    k1_2,
				 new PlotScaleSetting(("1:4"),         1,     4,    18, 0.25),                //    ("1:4"),            //    k1_4,
				 new PlotScaleSetting(("1:5"),         1,     5,    19, 0.20),                //    ("1:5"),            //    k1_5,
				 new PlotScaleSetting(("1:8"),         1,     8,    20, 0.125),               //    ("1:8"),            //    k1_8,
				 new PlotScaleSetting(("1:10"),        1,     10,   21, 0.1),                 //    ("1:10"),           //    k1_10, 
				 new PlotScaleSetting(("1:16"),        1,     16,   22, 0.0625),              //    ("1:16"),           //    k1_16, 
				 new PlotScaleSetting(("1:20"),        1,     20,   23, 0.05),                //    ("1:20"),           //    k1_20, 
				 new PlotScaleSetting(("1:30"),        1,     30,   24, 0.03333333333333),    //    ("1:30"),           //    k1_30, 
				 new PlotScaleSetting(("1:40"),        1,     40,   25, 0.025),               //    ("1:40"),           //    k1_40, 
				 new PlotScaleSetting(("1:50"),        1,     50,   26, 0.2),                 //    ("1:50"),           //    k1_50, 
				 new PlotScaleSetting(("1:100"),       1,     100,  27, 0.01),                //    ("1:100"),          //    k1_100,
				 new PlotScaleSetting(("2:1"),         2,     1,    28, 2),                   //    ("2:1"),            //    k2_1,
				 new PlotScaleSetting(("4:1"),         4,     1,    29, 4),                   //    ("4:1"),            //    k4_1,
				 new PlotScaleSetting(("8:1"),         8,     1,    30, 8),                   //    ("8:1"),            //    k8_1,
				 new PlotScaleSetting(("10:1"),        10,    1,    31, 10),                  //    ("10:1"),           //    k10_1, 
				 new PlotScaleSetting(("100:1"),       100,   1,    32, 100),                 //    ("100:1"),          //    k100_1,
				 new PlotScaleSetting(("1000:1"),      1000,  1,    33, 1000),                //    ("1000:1")          //    k1000_1
				 new PlotScaleSetting(("1.5\"= 1'"),   1,     8,    34, 0.125)                //    ("1.5\"= 1'")       //    k1and1_2in_1ft
    };
    #endregion

    #region Units Info
    struct unitsInfo
    {
      public double dScale;
      public String pName1;
      public String pName2;

      public unitsInfo(double scale, String name1, String name2)
      {
        dScale = scale;
        pName1 = name1;
        pName2 = name2;
      }

      public static String getTextByValue(double dValue, unitsInfo pInfo)
      {
        if (Math.Abs(dValue) <= 1.0)
        {
          return pInfo.pName1;
        }

        return pInfo.pName2;
      }
    };

    static unitsInfo[] unitsScale = new unitsInfo[]
    {
            new unitsInfo(Teigha.Core.Globals.kMmPerInch, ("inch"), ("inches")),
            new unitsInfo(1, ("mm"), ("mm")) ,
            new unitsInfo(1 , ("pixel"), ("pixels")),
            new unitsInfo(1 , ("unit"), ("units"))
    };
    #endregion


    private void initValues()
    {
      this.textDPI.Text = "0";
      this.checkCenterPlot.Checked = false;
      this.textOffsetX.Text = "0";
      offsetX = 0;
      this.textOffsetY.Text = "0";
      offsetY = 0;
      this.radioPortrait.Checked = true;
      this.textPaperUnit.Text = "0";
      this.textDrawingUnit.Text = "0";
      this.checkFit2Paper.Checked = false;
      this.textMarginLeft.Text = "0";
      this.textMarginRight.Text = "0";
      this.textMarginTop.Text = "0";
      this.textMarginBottom.Text = "0";
      this.checkDisplayPlotStyles.Checked = false;
      this.checkUpsideDown.Checked = false;
      this.checkScaleLW.Checked = false;
      this.checkPlotLineweights.Checked = false;
      this.checkPlotStyles.Checked = false;
      this.checkPlotPaperLast.Checked = false;
      this.checkHidePaperObj.Checked = false;
      this.checkPlotTransparencies.Checked = false;
      this.textMinX.Text = "0";
      this.textMinY.Text = "0";
      this.textMaxX.Text = "0";
      this.textMaxY.Text = "0";
    }

    private bool OnInitDialog()
    {
      this.labelName.Text = "";
      this.labelPlotter.Text = "";
      this.labelWhere.Text = "";
      this.labelDescription.Text = "";

      if (m_params.database() == null || m_params.database().appServices() == null)
        return false;

      m_pValidator = m_params.database().appServices().plotSettingsValidator();
      if (m_pValidator == null)
        return false;

      // fill device list
      m_pValidator.refreshLists(m_params);

      // is stored device name available in system ?
      String deviceName = m_params.getPlotCfgName();
      String mediaName = m_params.getCanonicalMediaName();

      setPlotCfgName2Validator(deviceName, mediaName, false);

      // fill device combo
      if (!FillDeviceCombo())
        return false;

      FillScaleValues(true);
      FillPlotAreaCombo(true);
      FillPlotOffset();

      FillPaperOrientation();
      FillPlotStyles();
      FillShadePlotQualityDPI(true);
      FillPlotStyleCombo(true);
      FillViewCombo(true);
      FillWindowArea();

      // select device in combo list.
      this.comboPlotDeviceName.SelectedIndex = this.comboPlotDeviceName.Items.IndexOf(deviceName);

      // select active paper from plot settings
      // au doesn't use media name stored in dxf, possible au
      // look for name by paper parameters.
      OnSelchangeDeviceList();

      return true;
    }

    private void setPlotCfgName2Validator(String pDeviceName, String canonicalMediaName, bool bValidNames)
    {
      String currentDevice = m_params.getPlotCfgName();
      String currentMedia = m_params.getCanonicalMediaName();

      if (bValidNames && pDeviceName == currentDevice && currentMedia == canonicalMediaName)
        return;

      double dW, dH;
      m_params.getPlotPaperSize(out dW, out dH);
      OdDbPlotSettings.PlotPaperUnits units = m_params.plotPaperUnits();

      //if (m_pValidator.setPlotCfgName(m_params, pDeviceName, canonicalMediaName) != eOk)   // good device, but wrong paper
      {
        if (m_pValidator.setPlotCfgName(m_params, pDeviceName) != OdResult.eOk) // wrong device
        {
          // if not - select None.
          pDeviceName = "None";
          m_pValidator.setPlotCfgName(m_params, pDeviceName, "none_user_media");
        }
        else if (bValidNames)
        {
          if (m_pValidator.setClosestMediaName(m_params, dW, dH, units, true) != OdResult.eOk)
          {
            pDeviceName = "None";
            m_pValidator.setPlotCfgName(m_params, pDeviceName, "none_user_media");
          }
        }
      }
    }

    #region Fill Dialog Forms
    private void FillMMInches()
    {
      this.comboMMInches.Items.Clear();
      OdDbPlotSettings.PlotPaperUnits ppu = m_params.plotPaperUnits();
      if (ppu == OdDbPlotSettings.PlotPaperUnits.kPixels)
      {
        this.comboMMInches.Items.Add("pixels");
        this.comboMMInches.Enabled = false;
        this.comboMMInches.SelectedIndex = this.comboMMInches.Items.IndexOf("pixels");
      }
      else
      {
        this.comboMMInches.Items.Add("inches");
        this.comboMMInches.Items.Add("mm");
        this.comboMMInches.Enabled = true;
        this.comboMMInches.SelectedIndex =
            (ppu == OdDbPlotSettings.PlotPaperUnits.kMillimeters)
            ? this.comboMMInches.FindString("mm")
            : this.comboMMInches.FindString("inches");
      }
    }

    //haven't tested
    private void FillViewCombo(bool bFillCombo)
    {
      if (bFillCombo)
      {
        this.comboViews.Items.Clear();
        OdDbViewTable pViewTable = (OdDbViewTable)m_params.database().getViewTableId().safeOpenObject();
        OdDbSymbolTableIterator pIt = pViewTable.newIterator();
        while (!pIt.done())
        {
          OdDbViewTableRecord pView = (OdDbViewTableRecord)pIt.getRecord();
          if (pView.isPaperspaceView() != isModelSpacePageSetup())
          {
            this.comboViews.Items.Add(pView.getName());
          }
          pIt.step();
        }
      }

      this.comboViews.Enabled = this.comboViews.Items.Count != 0;
      if (this.comboViews.Items.Count != 0)
      {
        this.comboViews.SelectedItem = m_params.getPlotViewName();
      }
    }

    private void FillShadePlotQualityDPI(bool bFillCombo)
    {
      if (bFillCombo)
      {
        this.comboQuality.Items.Clear();

        this.comboQuality.Items.Add("Draft");
        this.comboQuality.Items.Add("Preview");
        this.comboQuality.Items.Add("Normal");
        this.comboQuality.Items.Add("Presentation");
        this.comboQuality.Items.Add("Maximum");
        this.comboQuality.Items.Add("Custom");

        this.comboShadePlot.Items.Clear();

        this.comboShadePlot.Items.Add("As displayed");
        this.comboShadePlot.Items.Add("Wireframe");
        this.comboShadePlot.Items.Add("Hidden");
        this.comboShadePlot.Items.Add("Rendered");
      }

      Teigha.TD.OdDbPlotSettings.ShadePlotType pShadePlot = m_params.shadePlot();
      this.comboShadePlot.SelectedIndex = (int)pShadePlot;

      Teigha.TD.OdDbPlotSettings.ShadePlotResLevel spr = m_params.shadePlotResLevel();
      this.comboQuality.SelectedIndex = (int)spr;

      if (spr == OdDbPlotSettings.ShadePlotResLevel.kCustom)
        this.textDPI.Text = m_params.shadePlotCustomDPI().ToString();

      if (isModelSpacePageSetup())
      {
        bool bEnableWindows = pShadePlot == Teigha.TD.OdDbPlotSettings.ShadePlotType.kAsDisplayed || pShadePlot == OdDbPlotSettings.ShadePlotType.kRendered;
        this.comboQuality.Enabled = bEnableWindows;
        this.textDPI.Enabled = spr == Teigha.TD.OdDbPlotSettings.ShadePlotResLevel.kCustom && bEnableWindows;
      }
      else
      {
        this.comboShadePlot.Enabled = false;
        this.comboQuality.Enabled = true;
        this.textDPI.Enabled = spr == OdDbPlotSettings.ShadePlotResLevel.kCustom;
      }
    }

    private void FillPlotStyles()
    {
      this.checkPlotLineweights.Checked = m_params.printLineweights();
      this.checkPlotStyles.Checked = m_params.plotPlotStyles();
      this.checkPlotPaperLast.Checked = m_params.drawViewportsFirst();
      this.checkHidePaperObj.Checked = m_params.plotHidden();
      this.checkPlotTransparencies.Checked = m_params.plotTransparency();

      this.checkPlotPaperLast.Enabled = !isModelSpacePageSetup();
      this.checkHidePaperObj.Enabled = !isModelSpacePageSetup();
      this.checkPlotLineweights.Enabled = !m_params.plotPlotStyles();

      if (m_params.plotPlotStyles())
        checkPlotLineweights.Checked = true;
    }

    private void FillPaperOrientation()
    {
      Teigha.TD.OdDbPlotSettings.PlotRotation pr = m_params.plotRotation();

      if (!isPaperWidthLessHeight())
      {
        radioPortrait.Checked = Convert.ToBoolean((int)pr & 1);
        radioLandscape.Checked = !radioPortrait.Checked;
      }
      else
      {
        radioLandscape.Checked = Convert.ToBoolean((int)pr & 1);
        radioPortrait.Checked = !radioLandscape.Checked;
      }
      checkUpsideDown.Checked = Convert.ToBoolean(((int)pr & 2) / 2);

    }

    private bool FillDeviceCombo()
    {
      OdStringArray devices = new OdStringArray();
      m_pValidator.plotDeviceList(devices);

      this.comboPlotDeviceName.Items.Clear();

      foreach (String device in devices)
        this.comboPlotDeviceName.Items.Add(device);

      return true;
    }

    private bool FillPaperSizes()
    {
      OdStringArray canonicalMediaNames = new OdStringArray();
      m_pValidator.canonicalMediaNameList(m_params, canonicalMediaNames);

      this.comboPaperSize.Items.Clear();

      foreach (String name in canonicalMediaNames)
        this.comboPaperSize.Items.Add(m_pValidator.getLocaleMediaName(m_params, name));

      return true;
    }

    private void FillScaleValues(bool bFillCombo)
    {
      if (bFillCombo)
      {
        this.comboScaleValues.Items.Clear();

        foreach (PlotScaleSetting setting in plotScaleSetting)
          this.comboScaleValues.Items.Add(setting.m_Str);
      }

      Teigha.TD.OdDbPlotSettings.StdScaleType sst = m_params.stdScaleType();
      if (m_params.useStandardScale()
          && sst != OdDbPlotSettings.StdScaleType.kScaleToFit
          && sst >= 0
          && sst <= OdDbPlotSettings.StdScaleType.k1and1_2in_1ft)
      {
        this.comboScaleValues.SelectedIndex = (int)sst;
      }
      else
      {
        this.comboScaleValues.SelectedIndex = this.comboScaleValues.Items.IndexOf("Custom");
      }

      bool isModel = isModelSpacePageSetup();
      bool isLayoutMode = m_params.plotType() == OdDbPlotSettings.PlotType.kLayout;

      this.checkFit2Paper.Checked = m_params.useStandardScale()
          && !isLayoutMode
          && (sst == OdDbPlotSettings.StdScaleType.kScaleToFit);
      this.checkScaleLW.Checked = m_params.scaleLineweights();

      if (isLayoutMode)
      {
        this.checkFit2Paper.Checked = this.checkCenterPlot.Checked = false;
      }

      this.checkScaleLW.Enabled = !isModel;
      this.checkFit2Paper.Enabled = !isLayoutMode;
      this.checkCenterPlot.Enabled = !isLayoutMode;

      this.comboScaleValues.Enabled = !this.checkFit2Paper.Checked;
      this.textPaperUnit.Enabled = !this.checkFit2Paper.Checked;
      this.textDrawingUnit.Enabled = !this.checkFit2Paper.Checked;

      double m_dPaperScaleUnit = 0;
      double m_dDrawingScaleUnit = 0;

      if (m_params.useStandardScale() && !this.checkFit2Paper.Checked)
      {
        m_dPaperScaleUnit = plotScaleSetting[(int)sst].m_dRealWorldUnits;
        m_dDrawingScaleUnit = plotScaleSetting[(int)sst].m_dDrawingUnits;
      }
      else
      {
        m_params.getCustomPrintScale(out m_dPaperScaleUnit, out m_dDrawingScaleUnit);
      }

      FillMMInches();

      this.textPaperUnit.Text = m_dPaperScaleUnit.ToString();
      this.textDrawingUnit.Text = m_dDrawingScaleUnit.ToString();
    }

    //don't know when/where it is used
    private bool FillArrayByPatternFile(OdStringArray arrFiles, String sPattern)
    {
      //WIN32_FIND_DATA lpFindFileData;

      //memset(&lpFindFileData, 0, sizeof(WIN32_FIND_DATA));
      //String sFolder = sPattern.Left(sPattern.ReverseFind(_T('\\')) + 1);
      //HANDLE hFindFile = FindFirstFile(sPattern, &lpFindFileData);
      //CString sFile;

      //bool bFind = true;
      //bool bIsFind = false;
      //do
      //{
      //	if (lpFindFileData.dwFileAttributes & ~FILE_ATTRIBUTE_DIRECTORY)
      //	{
      //		sFile = sFolder + lpFindFileData.cFileName;
      //		arrFiles.append(sFile);
      //		bIsFind = true;
      //	}

      //	bFind = FindNextFile(hFindFile, &lpFindFileData);

      //} while (bFind && bFind != ERROR_NO_MORE_FILES);

      //FindClose(hFindFile);
      //return bIsFind;
      return true;
    }

    private void FillPlotStyleCombo(bool bFillCombo)
    {
      if (bFillCombo)
      {
        OdStringArray PSSlist = new OdStringArray();
        m_pValidator.plotStyleSheetList(PSSlist);

        this.comboPlotStyleFiles.Items.Add("None");

        foreach (String plotStyleName in PSSlist)
        {
          this.comboPlotStyleFiles.Items.Add(plotStyleName);
        }
      }

      int indx = 0;
      String curSS = m_params.getCurrentStyleSheet();
      if (curSS != "")
      {
        indx = this.comboPlotStyleFiles.Items.IndexOf(curSS);
        if (indx == -1)
          indx = 0;
      }
      this.comboPlotStyleFiles.SelectedIndex = indx;
      OnSelChangePlotStyleFiles();

      this.checkDisplayPlotStyles.Enabled = !isModelSpacePageSetup();
      this.checkDisplayPlotStyles.Checked = m_params.showPlotStyles();
    }

    private void FillPlotAreaCombo(bool bFillCombo)
    {
      if (bFillCombo)
      {
        this.comboAreaType.Items.Clear();

        if (isModelSpacePageSetup())
        {
          this.comboAreaType.Items.Add("Display");
          this.comboAreaType.Items.Add("Extents");
          this.comboAreaType.Items.Add("Limits");

          if (viewsExists())
            this.comboAreaType.Items.Add("View");

          this.comboAreaType.Items.Add("Window");
        }
        else
        {
          this.comboAreaType.Items.Add("Display");
          this.comboAreaType.Items.Add("Extents");
          this.comboAreaType.Items.Add("Layout");

          if (viewsExists())
            this.comboAreaType.Items.Add("View");

          this.comboAreaType.Items.Add("Window");
        }
      }

      OdDbPlotSettings.PlotType pt = m_params.plotType();

      this.comboViews.Visible = pt == OdDbPlotSettings.PlotType.kView;
      this.buttonWindow.Visible = pt == OdDbPlotSettings.PlotType.kWindow;

      switch (pt)
      {
        case OdDbPlotSettings.PlotType.kDisplay:
          {
            this.comboAreaType.Text = "Display";
          }
          break;
        case OdDbPlotSettings.PlotType.kExtents:
          {
            this.comboAreaType.Text = "Extents";
          }
          break;
        case OdDbPlotSettings.PlotType.kLimits:
          {
            this.comboAreaType.Text = "Limits";
          }
          break;
        case OdDbPlotSettings.PlotType.kView:
          {
            this.comboAreaType.Text = "View";
          }
          break;
        case OdDbPlotSettings.PlotType.kWindow:
          {
            this.comboAreaType.Text = "Window";
          }
          break;
        case OdDbPlotSettings.PlotType.kLayout:
          {
            this.comboAreaType.Text = "Layout";
          }
          break;
      };

      //UpdateData(false);
    }

    private void FillWindowArea()
    {
      double xmin, ymin, xmax, ymax;
      m_params.getPlotWindowArea(out xmin, out ymin, out xmax, out ymax);

      this.textMinX.Text = String.Format("{0:F6}", xmin);
      this.textMinY.Text = String.Format("{0:F6}", ymin);
      this.textMaxX.Text = String.Format("{0:F6}", xmax);
      this.textMaxY.Text = String.Format("{0:F6}", ymax);
    }

    private void FillPlotOffset()
    {
      this.checkCenterPlot.Checked = m_params.plotCentered();

      this.textOffsetX.Enabled = !this.checkCenterPlot.Checked;
      this.textOffsetY.Enabled = !this.checkCenterPlot.Checked;

      if (isWHSwap())
        m_params.getPlotOrigin(out offsetY, out offsetX);
      else
        m_params.getPlotOrigin(out offsetX, out offsetY);

      OdDbPlotSettings.PlotPaperUnits ppu = m_params.plotPaperUnits();
      if (ppu == OdDbPlotSettings.PlotPaperUnits.kInches)
      {
        offsetX /= unitsScale[(int)ppu].dScale;
        offsetY /= unitsScale[(int)ppu].dScale;
      }

      // it doesn't changed with isWHSwap
      this.textPIOX.Text = String.Format("{0:F6}", m_params.getPaperImageOrigin().x);
      this.textPIOY.Text = String.Format("{0:F6}", m_params.getPaperImageOrigin().y);

      this.textMarginLeft.Text = String.Format("{0:F6}", m_params.getLeftMargin());
      this.textMarginRight.Text = String.Format("{0:F6}", m_params.getRightMargin());
      this.textMarginTop.Text = String.Format("{0:F6}", m_params.getTopMargin());
      this.textMarginBottom.Text = String.Format("{0:F6}", m_params.getBottomMargin());

      this.textCanonicalName.Text = m_params.getCanonicalMediaName();

      this.labelOffsetX.Text = unitsInfo.getTextByValue(offsetX, unitsScale[(int)ppu]);
      this.labelOffsetY.Text = unitsInfo.getTextByValue(offsetY, unitsScale[(int)ppu]);
      this.textOffsetX.Text = offsetX.ToString();
      this.textOffsetY.Text = offsetY.ToString();
    }

    #endregion

    private void OnCheckPlotTransparencies(object sender, EventArgs e)
    {
      m_params.setPlotTransparency(this.checkPlotTransparencies.Checked);
    }

    private void OnClickPortraitLandscape()
    {
      OdDbPlotSettings.PlotRotation pr;

      if (isPaperWidthLessHeight())
        pr = (Teigha.TD.OdDbPlotSettings.PlotRotation)(Convert.ToInt32(radioLandscape.Checked) + Convert.ToInt32(checkUpsideDown.Checked) * 2);
      else
        pr = (Teigha.TD.OdDbPlotSettings.PlotRotation)(Convert.ToInt32(!radioLandscape.Checked) + Convert.ToInt32(checkUpsideDown.Checked) * 2);

      m_pValidator.setPlotRotation(m_params, pr);

      FillPaperOrientation();
      FillScaleValues(false);
      FillPlotOffset();
    }

    private void OnChangeEditScaleUnit()
    {
      double dOldPaperScaleUnit = Double.Parse(this.textPaperUnit.Text);
      double dOldDrawingScaleUnit = Double.Parse(this.textDrawingUnit.Text);

      if (dOldPaperScaleUnit != 0 || dOldDrawingScaleUnit != 0)
      {
        if (m_pValidator.setCustomPrintScale(m_params, dOldPaperScaleUnit, dOldDrawingScaleUnit) != OdResult.eOk)
        {
          MessageBox.Show("No");
          return;
        }

        FillPaperOrientation();
        FillScaleValues(false);
        FillPlotOffset();
      }
    }

    private void checkFit2Paper_CheckedChanged(object sender, EventArgs e)
    {
      OnCheckFitToPaper();
    }

    private void OnCheckFitToPaper()
    {
      if (this.checkFit2Paper.Checked)
      {
        m_pValidator.setStdScaleType(m_params, OdDbPlotSettings.StdScaleType.kScaleToFit);
      }
      else
      {
        m_pValidator.setUseStandardScale(m_params, false);
      }
      FillPaperOrientation();
      FillScaleValues(false);
      FillPlotOffset();
    }

    private void OnCheckScaleLW(object sender, EventArgs e)
    {
      m_params.setScaleLineweights(this.checkScaleLW.Checked);

      FillPaperOrientation();
      FillScaleValues(false);
      FillPlotOffset();
    }

    private void OnSelchangeScaleValues(object sender, EventArgs e)
    {
      int i = this.comboScaleValues.SelectedIndex;
      this.comboScaleValues.Text = this.comboScaleValues.Items[i].ToString();
      if (i != 0) // skip Custom
        m_pValidator.setStdScaleType(m_params, (OdDbPlotSettings.StdScaleType)(plotScaleSetting[i].m_ScaleType));

      FillPaperOrientation();
      FillScaleValues(false);
      FillPlotOffset();
    }

    private OdDbPlotSettings.PlotPaperUnits getMediaNativePPU()
    {
      OdDbPlotSettings parameters = OdDbPlotSettings.createObject();
      m_pValidator.setPlotCfgName(parameters, m_params.getPlotCfgName(), m_params.getCanonicalMediaName());

      return parameters.plotPaperUnits();
    }

    private String getCanonicalByLocaleMediaName(String localeMediaName)
    {
      OdStringArray mediaNames = new OdStringArray();
      m_pValidator.canonicalMediaNameList(m_params, mediaNames);

      for (int i = 0; i < mediaNames.Count; i++)
      {
        if (m_pValidator.getLocaleMediaName(m_params, mediaNames[i]) == localeMediaName)
        {
          return mediaNames[i];
        }
      }

      return mediaNames[0];
    }

    private void comboPlotDeviceName_SelectedIndexChanged(object sender, EventArgs e)
    {
      OnSelchangeDeviceList();
    }

    private void OnSelchangeDeviceList()
    {
      String newDeviceName;
      int i = this.comboPlotDeviceName.SelectedIndex;
      if (i == -1)
        return;
      newDeviceName = this.comboPlotDeviceName.Items[i].ToString();

      String canonicalMediaName = m_params.getCanonicalMediaName();

      String strDeviceName = (newDeviceName);
      setPlotCfgName2Validator(strDeviceName, canonicalMediaName, true);

      if (strDeviceName == ("Custom Device"))
        this.buttonAddMedia.Visible = true;
      else
        this.buttonAddMedia.Visible = false;

      this.comboPlotDeviceName.Text = strDeviceName;

      // fill paper sizes combo box
      if (!FillPaperSizes())
      {
        return /*FALSE*/;
      }

      String localeMediaName = m_pValidator.getLocaleMediaName(m_params, m_params.getCanonicalMediaName());

      // select active media
      this.comboPaperSize.SelectedIndex = this.comboPaperSize.FindStringExact(localeMediaName);
      if (this.comboPaperSize.SelectedIndex == -1)
      {
        String csLocaleMediaName = "";
        csLocaleMediaName = this.comboPaperSize.GetItemText(0);
        if (csLocaleMediaName == (""))
        {
          return /*FALSE*/;
        }

        canonicalMediaName = getCanonicalByLocaleMediaName(csLocaleMediaName);
        m_pValidator.setCanonicalMediaName(m_params, canonicalMediaName);

        this.comboPaperSize.SelectedIndex = this.comboPaperSize.FindStringExact(csLocaleMediaName);
      }

      FillPlotAreaCombo(false);
      FillPlotOffset();

      FillScaleValues(false);
      FillPaperOrientation();
      FillPlotStyles();
    }

    private bool isWHSwap()
    {

      OdDbPlotSettings.PlotRotation pr = m_params.plotRotation();
      return pr == OdDbPlotSettings.PlotRotation.k90degrees || pr == OdDbPlotSettings.PlotRotation.k270degrees;
    }

    private void OnChangeEditOffsetXY(object sender, EventArgs e)
    {
      try // try is used for incorrect input
      {
        if (((TextBox)sender).ContainsFocus)
        {
          offsetX = Double.Parse(this.textOffsetX.Text);
          offsetY = Double.Parse(this.textOffsetY.Text);
          OdDbPlotSettings.PlotPaperUnits ppu = m_params.plotPaperUnits();
          if (ppu == OdDbPlotSettings.PlotPaperUnits.kInches)
          {
            offsetX *= unitsScale[(int)ppu].dScale;
            offsetY *= unitsScale[(int)ppu].dScale;
          }

          if (isWHSwap())
            m_pValidator.setPlotOrigin(m_params, offsetY, offsetX);
          else
            m_pValidator.setPlotOrigin(m_params, offsetX, offsetY);

          FillPaperOrientation();
          FillPlotOffset(); // possibly offset was changed in validator
          FillScaleValues(false);
        }
      }
      catch (Exception ex) { };
    }

    private void OnCheckCenterThePlot(object sender, EventArgs e)
    {
      m_pValidator.setPlotCentered(m_params, this.checkCenterPlot.Checked);

      FillPaperOrientation();
      FillPlotOffset();
      FillScaleValues(false);
    }

    private void OnSelChangePlotAreaType(object sender, EventArgs e)
    {
      String newViewType = "";
      int i = this.comboAreaType.SelectedIndex;
      newViewType = this.comboAreaType.Items[i].ToString();

      OdDbPlotSettings.PlotType pt = OdDbPlotSettings.PlotType.kDisplay;
      if (newViewType == "Display")
      {
        pt = OdDbPlotSettings.PlotType.kDisplay;
      }
      else if (newViewType == "Limits")
      {
        pt = OdDbPlotSettings.PlotType.kLimits;
      }
      else if (newViewType == "View")
      {
        pt = OdDbPlotSettings.PlotType.kView;
      }
      else if (newViewType == "Window")
      {
        pt = OdDbPlotSettings.PlotType.kWindow;
        OnClickWindowButton();
      }
      else if (newViewType == "Extents")
      {
        pt = OdDbPlotSettings.PlotType.kExtents;
      }
      else if (newViewType == "Layout")
      {
        pt = OdDbPlotSettings.PlotType.kLayout;
      }

      m_pValidator.setPlotType(m_params, pt);

      FillPlotAreaCombo(false);

      if (pt == OdDbPlotSettings.PlotType.kLayout)
      {
        // This is differences between dialog and validator. Validator doesn't
        // change UseStandardScale to false. Example is kExtents, kFit2Paper . kLayout .kExtents
        // Dialog has kFit2Paper disabled, but validator don't clear kFit2Paper flag.
        // Validator also don't change PlotOrigin to 0,0, if plotsenteres was true, but it change scale to 1:1 if fittopaper was true

        if (this.checkCenterPlot.Checked)
        {
          m_pValidator.setPlotOrigin(m_params, 0, 0);
        }
        if (this.checkFit2Paper.Checked)
        {
          m_pValidator.setUseStandardScale(m_params, false);
        }
      }

      FillPaperOrientation();
      FillScaleValues(false);
      FillPlotOffset();
    }

    private bool isPaperWidthLessHeight()
    {
      double paperWidth, paperHeight;
      m_params.getPlotPaperSize(out paperWidth, out paperHeight);
      return paperWidth < paperHeight;
    }

    private bool viewsExists()
    {
      OdDbViewTable pViewTable = (OdDbViewTable)m_params.database().getViewTableId().safeOpenObject();
      OdDbSymbolTableIterator pIt = pViewTable.newIterator();
      while (!pIt.done())
      {
        OdDbViewTableRecord pView = (OdDbViewTableRecord)pIt.getRecord();
        if (pView.isPaperspaceView() != isModelSpacePageSetup())
        {
          return true;
        }

        pIt.step();
      }
      return false;
    }

    private bool isModelSpacePageSetup()
    {
      return m_params.modelType();
    }

    private void buttonPlotStyleFiles_Click(object sender, EventArgs e)
    {
      OnClickPlotStyleFilesBtn();
    }

    private void OnClickPlotStyleFilesBtn()
    {
      int idx = this.comboPlotStyleFiles.SelectedIndex;

      String tmp;
      tmp = this.comboPlotStyleFiles.Text;

      String sPath = tmp;

      try
      {
        OdStreamBuf pFileBuf = new Teigha.Core.OdStreamBuf();
        bool bSucc = false;
        OdRxSystemServices pSs = Teigha.Core.Globals.odrxSystemServices();
        sPath = m_params.database().appServices().findFile(sPath);
        if (sPath == "")
          return;
        unchecked
        {
          if (pSs.accessFile(sPath, (int)FileAccessMode.kFileRead))
          {
            bSucc = true;
            pFileBuf = pSs.createFile(sPath);
          }
        }
        if (!bSucc) return;
        OdPsPlotStyleTable pPlotStyleTable;
        if (pFileBuf != null)
        {
          OdPsPlotStyleServices pPSS = (OdPsPlotStyleServices)Teigha.Core.Globals.odrxDynamicLinker().loadApp("PlotStyleServices");
          if (pPSS != null)
          {
            pPlotStyleTable = pPSS.loadPlotStyleTable(pFileBuf);
          }
        }

        OdExPsTableEditorDlg PsTableEditorDlg = new OdExPsTableEditorDlg();
        //PsTableEditorDlg.SetFileBufPath(sPath);
        //PsTableEditorDlg.SetPlotStyleTable(pPlotStyleTable);
        if (PsTableEditorDlg.ShowDialog() == DialogResult.OK)
        {
          return;
          //pPlotStyleTable.copyFrom(PsTableEditorDlg.GetPlotStyleTable());
        }
      }

      catch (Exception er)
      {
        return;
      }
    }

    private void OnSelChangeViewsList(object sender, EventArgs e)
    {
      String viewName;
      int idx = this.comboViews.SelectedIndex;
      viewName = this.comboViews.Text;

      m_pValidator.setPlotViewName(m_params, viewName);

      FillViewCombo(false);
    }

    private void unitsConverted(OdDbPlotSettings.PlotPaperUnits prevUnits, OdDbPlotSettings.PlotPaperUnits plotPaperUnits)
    {
      double dCoeff = 0;
      if (plotPaperUnits == OdDbPlotSettings.PlotPaperUnits.kMillimeters
          && prevUnits == OdDbPlotSettings.PlotPaperUnits.kInches)
      {
        dCoeff = 25.4;
      }
      else if (plotPaperUnits == OdDbPlotSettings.PlotPaperUnits.kInches
          && prevUnits == OdDbPlotSettings.PlotPaperUnits.kMillimeters)
      {
        dCoeff = 1 / 25.4;
      }
      else
      {
        return;
      }

      OdGePoint2d paperImageOrigin = m_params.getPaperImageOrigin();
      paperImageOrigin.x /= dCoeff;
      paperImageOrigin.y /= dCoeff;
      m_params.setPaperImageOrigin(paperImageOrigin);

      bool bStandardScale = m_params.useStandardScale();
      if (bStandardScale)
      {
        double dStandardScale;
        m_params.getStdScale(out dStandardScale);
        if (dStandardScale != 0) // skip Fit to paper
          m_pValidator.setCustomPrintScale(m_params, dStandardScale, 1 / dCoeff);
      }
      else
      {
        double numerator, denominator;
        m_params.getCustomPrintScale(out numerator, out denominator);
        m_pValidator.setCustomPrintScale(m_params, numerator, denominator / dCoeff);
      }
    }

    private void comboMMInches_SelectedIndexChanged(object sender, EventArgs e)
    {
      OnSelChangeMMInchesList();
    }

    private void OnSelChangeMMInchesList()
    {
      String unitsStr;
      int idx = this.comboMMInches.SelectedIndex;
      unitsStr = this.comboMMInches.Text;

      OdDbPlotSettings.PlotPaperUnits plotPaperUnits = OdDbPlotSettings.PlotPaperUnits.kPixels;
      if (unitsStr == "mm") plotPaperUnits = OdDbPlotSettings.PlotPaperUnits.kMillimeters;
      else if (unitsStr == "inches") plotPaperUnits = OdDbPlotSettings.PlotPaperUnits.kInches;

      OdDbPlotSettings.PlotPaperUnits prevUnits = m_params.plotPaperUnits();
      if (prevUnits != plotPaperUnits)
      {
        if (m_pValidator.setPlotPaperUnits(m_params, plotPaperUnits) != OdResult.eOk)
        {
          MessageBox.Show("OdaAssert Failed!");
        }

        unitsConverted(prevUnits, plotPaperUnits);

        FillScaleValues(false);
        FillPlotOffset();
      }
    }

    private void OnClickAddMediaButton(object sender, EventArgs e)
    {
      OdDbPlotSettingsValidatorPE.psvPaperInfo paperInfo = new OdDbPlotSettingsValidatorPE.psvPaperInfo();
      OdExCustomMediaDlg dlgCM = new OdExCustomMediaDlg(paperInfo);
      if (dlgCM.ShowDialog() == DialogResult.OK)
      {
        OdDbPlotSettingsValidatorCustomMediaPE pPE = OdDbPlotSettingsValidatorPE.desc().getX(OdDbPlotSettingsValidatorCustomMediaPE.desc()) as OdDbPlotSettingsValidatorCustomMediaPE;
        if (pPE != null)
        {
          pPE.addMedia(paperInfo);

          m_pValidator.setCanonicalMediaName(m_params, "none_user_media");
          FillPaperSizes();
          m_pValidator.setCanonicalMediaName(m_params, paperInfo.canonicalName);
          OnSelchangeDeviceList();
        }
      }
    }

    private void buttonWindow_Click(object sender, EventArgs e)
    {
      OnClickWindowButton();
    }

    private void OnClickWindowButton()
    {
      OdExPsWindowPointsDlg wp = new OdExPsWindowPointsDlg();
      if (wp.ShowDialog() == DialogResult.OK)
      {
        OdGePoint3d pt1, pt2;
        wp.getPoints(out pt1, out pt2);

        m_pValidator.setPlotWindowArea(m_params, pt1.x, pt1.y, pt2.x, pt2.y);

        FillWindowArea();
      }
    }

    private void OnChangeEditDPI(object sender, EventArgs e)
    {
      try
      {
        m_params.setShadePlotCustomDPI(Int16.Parse(this.textDPI.Text));

        FillShadePlotQualityDPI(false);
      }
      catch (Exception ex) { }
    }

    private void OnSelChangeQualityList(object sender, EventArgs e)
    {
      int idx = this.comboQuality.SelectedIndex;
      m_params.setShadePlotResLevel((OdDbPlotSettings.ShadePlotResLevel)idx);

      FillShadePlotQualityDPI(false);
    }

    private void OnSelChangeShadePlotList(object sender, EventArgs e)
    {
      int idx = this.comboShadePlot.SelectedIndex;

      m_params.setShadePlot((OdDbPlotSettings.ShadePlotType)idx);

      FillShadePlotQualityDPI(false);
    }

    private void OnCheckDisplayPlotStyles(object sender, EventArgs e)
    {
      m_params.setShowPlotStyles(this.checkDisplayPlotStyles.Checked);

      FillPlotStyleCombo(false);
    }

    private void comboPlotStyleFiles_SelectedIndexChanged(object sender, EventArgs e)
    {
      OnSelChangePlotStyleFiles();
    }

    private void OnSelChangePlotStyleFiles()
    {
      int idx = this.comboPlotStyleFiles.SelectedIndex;
      this.buttonPlotStyleFiles.Enabled = this.comboPlotStyleFiles.Text != "None";

      if (idx != -1)
      {
        String styleSheetName = this.comboPlotStyleFiles.SelectedItem as String;
        m_pValidator.setCurrentStyleSheet(m_params, styleSheetName);
      }
      else
      {
        m_pValidator.setCurrentStyleSheet(m_params, String.Empty);
      }
    }

    private void OnSelChangeMediaList(object sender, EventArgs e)
    {
      String newLocaleMediaName = "";
      int i = this.comboPaperSize.SelectedIndex;
      newLocaleMediaName = this.comboPaperSize.Items[i].ToString();
      if (newLocaleMediaName == "")
        return;
      String newCanonicalMediaName = getCanonicalByLocaleMediaName(newLocaleMediaName);

      m_pValidator.setCanonicalMediaName(m_params, newCanonicalMediaName);
      OdDbPlotSettings.PlotPaperUnits nativeUnits = getMediaNativePPU();

      FillPaperSizes();
      this.comboPaperSize.SelectedIndex = comboPaperSize.FindStringExact(newLocaleMediaName);

      // change paper orientation to dialog values
      OnClickPortraitLandscape();

      // and reset units to paper native
      if (nativeUnits == OdDbPlotSettings.PlotPaperUnits.kInches
          || nativeUnits == OdDbPlotSettings.PlotPaperUnits.kMillimeters)
      {
        this.comboMMInches.SelectedIndex = (nativeUnits == OdDbPlotSettings.PlotPaperUnits.kMillimeters) ? 1 : 0;
        OnSelChangeMMInchesList();
      }
    }

    private void checkPlotStyles_CheckedChanged(object sender, EventArgs e)
    {
      m_params.setPlotPlotStyles(this.checkPlotStyles.Checked);
      FillPlotStyles();
    }

    private void checkPlotPaperLast_CheckedChanged(object sender, EventArgs e)
    {
      m_params.setDrawViewportsFirst(this.checkPlotPaperLast.Checked);
      FillPlotStyles();
    }

    private void checkHidePaperObj_CheckedChanged(object sender, EventArgs e)
    {
      m_params.setPlotHidden(this.checkHidePaperObj.Checked);
      FillPlotStyles();
    }

    private void checkPlotLineweights_CheckedChanged(object sender, EventArgs e)
    {
      m_params.setPrintLineweights(this.checkPlotLineweights.Checked);
      FillPlotStyles();
    }

    private void OnRadioPortraitChecked(object sender, MouseEventArgs e)
    {
      this.radioPortrait.Checked = true;
      OnClickPortraitLandscape();
    }

    private void OnRadioLandscapeChecked(object sender, MouseEventArgs e)
    {
      this.radioLandscape.Checked = true;
      OnClickPortraitLandscape();
    }
  }
}
