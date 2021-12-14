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

namespace OdKernelExamplesMgdLib.Export.Pdf
{

  public abstract class OdExPdfExportParamsFiller
  {
    protected enum OdExPdfExportConvertFromType
    {
      None,
      Dwg,
      Dgn,
    }

    protected abstract OdExPdfExportConvertFromType ConvertFromType();

    public OdExPdfExportParamsFiller()
    {
      OdGsPageParams pageParams = new OdGsPageParams();
      this.PaperH = pageParams.getPaperHeight();
      this.PaperW = pageParams.getPaperWidth();
    }

    #region public variables
    public double PaperW { get; set; }
    public double PaperH { get; set; }
    public ushort HatchDPI { get; set; }
    public ushort GeomDPI { get; set; }
    public bool EmbeddedFonts { get; set; }
    public bool SimpleGeomOpt { get; set; }
    public bool SHXAsGeometry { get; set; }
    public bool TTFAsGeometry { get; set; }
    public bool ZoomToExtents { get; set; }
    public bool UseHLR { get; set; }
    public bool EnableLayers { get; set; }
    public bool ExportOffLayers { get; set; }
    public bool Encoded { get; set; }
    public bool AllLayouts { get; set; }
    public bool ExportHyperlinks { get; set; }
    public String Title { get; set; }
    public String Author { get; set; }
    public String Subject { get; set; }
    public String Keywords { get; set; }
    public String Creator { get; set; }
    public String Producer { get; set; }
    public String PdfFilePath { get; set; }
    public bool UsePrc { get; set; }
    public bool UsePRCAsBRep { get; set; }
    public OdStringArray LayoutList { get; set; }
    public OdRxObjectPtrArray Databases { get; set; }
    public bool MultipleDatabases { get; set; }
    #endregion

    public static OdExPdfExportParamsFiller FillDefalut(OdExPdfExportParamsFiller result)
    {
      result.HatchDPI = 150;
      result.GeomDPI = 600;

      result.EmbeddedFonts = true;
      result.SHXAsGeometry = true;
      result.SimpleGeomOpt = true;
      result.ZoomToExtents = true;
      result.EnableLayers = true;
      result.ExportOffLayers = true;
      result.UseHLR = true;
      result.Encoded = true;
      result.ExportHyperlinks = true;

      result.Author = "Open Design Alliance";
      result.Subject = "ODA .NET SWIG Pdf Export Example";
      result.Keywords = "Open Design Alliance";
      result.Creator = "Open Design Alliance";
      result.Producer = "Open Design Alliance";
      result.MultipleDatabases = false;
      return result;
    }

    public PDFExportParams Generate(OdRxObject db)
    {
      PDFExportParams result = new PDFExportParams();

      FillLayoutList(result, db);

      FillPageParameters(result);

      FillVersion(result);

      FillOutput(result);

      FillExportFlags(result);

      FillPalleteAndBackground(result);

      FillPrcExport(result);

      result.setHatchDPI(HatchDPI);
      result.setGeomDPI(GeomDPI);
      result.setTitle(Title);
      result.setAuthor(Author);
      result.setSubject(Subject);
      result.setKeywords(Keywords);
      result.setCreator(Creator);
      result.setProducer(Producer);

      return result;
    }

    private void FillLayoutList(PDFExportParams fillTo, OdRxObject database)
    {
      FillLayoutList(database);

      if (database != null)
        fillTo.setDatabase(database);

      if (Databases != null)
        fillTo.databases().AddRange(Databases);
    }

    protected abstract void FillLayoutList(OdRxObject database);

    private void FillPrcExport(PDFExportParams result)
    {
      if (!UsePrc)
        return;
      OdRxModule prcModule = Globals.odrxDynamicLinker().loadModule("OdPrcModule");
      if (null == prcModule)
        throw new Exception("PDF Export, PRC not support - OdPrcModule module is missing");

      OdRxModule prcExportModule = Globals.odrxDynamicLinker().loadModule(GetPrcExportModuleName());
      if (null == prcExportModule)
        throw new Exception(String.Format("PDF Export, PRC not support - {0} module is missing", GetPrcExportModuleName()));

      result.setPRCMode(UsePRCAsBRep ? PDFExportParams.PRCSupport.kAsBrep : PDFExportParams.PRCSupport.kAsMesh);

      FillPrcContext(result);
    }

    private String GetPrcExportModuleName()
    {
      String prcExportModuleName = null;
      switch (ConvertFromType())
      {
        case OdExPdfExportConvertFromType.Dwg: prcExportModuleName = "OdDwg2PrcExport"; break;
        case OdExPdfExportConvertFromType.Dgn: prcExportModuleName = "OdDgn2PrcExport"; break;
        case OdExPdfExportConvertFromType.None:
        default:
          throw new Exception("Prc export module not defined");
      }
      return prcExportModuleName;
    }

    private void FillPrcContext(PDFExportParams result)
    {
      OdRxObject prcContext = CreatePrcContext();
      if (null == prcContext)
        throw new Exception("PDF Export, PrcContext - not created");

      OdRxClass pCls = OdRxClass.cast(prcContext);
      if (null == pCls)
        throw new Exception("PDF Export, PRC not support - OdRxClass failed");
      result.setPRCContext(pCls.create());
    }

    private OdRxObject CreatePrcContext()
    {
      OdRxObject prcContext = null;
      bool m_bUsePRCSingleViewMode = true; // provide a corresponding checkbox in Export to PDF dialog similar to one in OdaMfcApp
      if (m_bUsePRCSingleViewMode)
        prcContext = Globals.odrxClassDictionary().getAt("OdPrcContextForPdfExport_AllInSingleView");
      else
        prcContext = Globals.odrxClassDictionary().getAt("OdPrcContextForPdfExport_Default");
      return prcContext;
    }


    private void FillPageParameters(PDFExportParams fillTo)
    {
      OdGsPageParams pageParams = new OdGsPageParams();

      pageParams.set(this.PaperW, this.PaperH);

      if (!this.EnableLayers)
        this.ExportOffLayers = false;

      if (LayoutList == null)
        throw new Exception("LayoutList is null");

      fillTo.setLayouts(this.LayoutList,this.Databases);

      OdGsPageParamsArray ppArr = fillTo.pageParams();
      uint len = (uint)LayoutList.Count;
      if (len == 0) len = 1;
      ppArr.resize(len);

      for (int idx = 0; idx < ppArr.Count; idx++)
      {
        OdGsPageParams param = new OdGsPageParams(pageParams);
        GC.SuppressFinalize(param);
        ppArr[idx] = param;
      }

      fillTo.setPageParams(ppArr);
    }

    private void FillVersion(PDFExportParams fillTo)
    {
      if (!this.EnableLayers)
        this.ExportOffLayers = false;

      bool bV15 = EnableLayers || ExportOffLayers;

      fillTo.setVersion((PDFExportParams.PDFExportVersions)(bV15 ? PDFExportParams.PDFExportVersions.kPDFv1_5 : PDFExportParams.PDFExportVersions.kPDFv1_4));
    }

    private void FillExportFlags(PDFExportParams fillTo)
    {
      fillTo.setExportFlags(
                            (EmbeddedFonts ? PDFExportParams.PDFExportFlags.kEmbededTTF : 0) |
                            (SHXAsGeometry ? PDFExportParams.PDFExportFlags.kSHXTextAsGeometry : 0) |
                            (TTFAsGeometry ? PDFExportParams.PDFExportFlags.kTTFTextAsGeometry : 0) |
                            (SimpleGeomOpt ? PDFExportParams.PDFExportFlags.kSimpleGeomOptimization : 0) |
                            (ZoomToExtents ? PDFExportParams.PDFExportFlags.kZoomToExtentsMode : 0) |
                            (EnableLayers ? PDFExportParams.PDFExportFlags.kEnableLayers : 0) |
                            (ExportOffLayers ? PDFExportParams.PDFExportFlags.kIncludeOffLayers : 0) |
                            (UseHLR ? PDFExportParams.PDFExportFlags.kUseHLR : 0) |
                            (Encoded ? PDFExportParams.PDFExportFlags.kFlateCompression : 0) |
                            (Encoded ? PDFExportParams.PDFExportFlags.kASCIIHexEncoding : 0) |
                            (ExportHyperlinks ? PDFExportParams.PDFExportFlags.kExportHyperlinks : 0)
                            );
    }

    private void FillPalleteAndBackground(PDFExportParams fillTo)
    {
      UInt32[] CurPalette = Teigha.Core.AllPalettes.getLightPalette();

      fillTo.Palette = CurPalette;
      fillTo.setBackground(CurPalette[0]);
    }

    private void FillOutput(PDFExportParams fillTo)
    {
      OdStreamBuf file = Globals.odrxSystemServices().createFile(this.PdfFilePath, FileAccessMode.kFileWrite, FileShareMode.kShareDenyNo, FileCreationDisposition.kCreateAlways);

      fillTo.setOutput(file);
    }
  }
}