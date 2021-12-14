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
using System.IO;
using Teigha.Core;

namespace OdKernelExamplesMgdLib.Export.Dwf
{
  public abstract class OdExDwfExportParamsFiller
  {
    public String OutDwfFilepath { get; set; }
    public bool InkedArea { get; set; }
    public bool ColorMapOptimize { get; set; }
    public bool ExportInvisibleText { get; set; }
    public OdExDwfExportType DwfExportType { get; set; }

    public OdExDwfExportParamsFiller()
    {
      InkedArea = false;
      ColorMapOptimize = false;
      ExportInvisibleText = true;
      DwfExportType = OdExDwfExportType.BinaryDWF_v60;
    }

    public DwExportParams Generate(OdRxObject database)
    {
      DwExportParams result = new DwExportParams();

      result.setDatabase(database);
      result.setDwfFileName(this.OutDwfFilepath);
      result.setInkedArea(this.InkedArea);
      result.setColorMapOptimize(this.ColorMapOptimize);
      result.setExportInvisibleText(this.ExportInvisibleText);

      FillPalleteAndBackground(result);

      FillPageData(result, database);

      FillVersionAndFormat(result);

      return result;
    }

    protected abstract void FillPageData(DwExportParams fillTo, OdRxObject db);

    private void FillVersionAndFormat(DwExportParams fillTo)
    {
      switch (this.DwfExportType)
      {
        case OdExDwfExportType.BinaryDWF_v60:
          fillTo.setFormat(DW_FORMAT.DW_UNCOMPRESSED_BINARY);
          fillTo.setVersion(DwfVersion.nDwf_v60);
          break;
        case OdExDwfExportType.ZippedStreamDWF_v60:
          fillTo.setFormat(DW_FORMAT.DW_ASCII);
          fillTo.setVersion(DwfVersion.nDwf_v60);
          break;
        case OdExDwfExportType.CompressedDWF_v55:
          fillTo.setFormat(DW_FORMAT.DW_COMPRESSED_BINARY);
          fillTo.setVersion(DwfVersion.nDwf_v55);
          break;
        case OdExDwfExportType.BinaryDWF_v55:
          fillTo.setFormat(DW_FORMAT.DW_UNCOMPRESSED_BINARY);
          fillTo.setVersion(DwfVersion.nDwf_v55);
          break;
        case OdExDwfExportType.AsciiDWF_v55:
          fillTo.setFormat(DW_FORMAT.DW_ASCII);
          fillTo.setVersion(DwfVersion.nDwf_v55);
          break;
        case OdExDwfExportType.CompressedDWF_v42:
          fillTo.setFormat(DW_FORMAT.DW_COMPRESSED_BINARY);
          fillTo.setVersion(DwfVersion.nDwf_v42);
          break;
        case OdExDwfExportType.BinaryDWF_v42:
          fillTo.setFormat(DW_FORMAT.DW_UNCOMPRESSED_BINARY);
          fillTo.setVersion(DwfVersion.nDwf_v42);
          break;
        case OdExDwfExportType.AsciiDWF_v42:
          fillTo.setFormat(DW_FORMAT.DW_ASCII);
          fillTo.setVersion(DwfVersion.nDwf_v42);
          break;
        case OdExDwfExportType.XPS:
          fillTo.setFormat(DW_FORMAT.DW_XPS);
          fillTo.setVersion(DwfVersion.nDwf_v6_unknown);
          break;
        default:
          throw new Exception("Dwf export format & version not implemented in example");
      }
    }

    private void FillPalleteAndBackground(DwExportParams fillTo)
    {
      UInt32[] CurPalette = Teigha.Core.AllPalettes.getLightPalette();

      fillTo.Palette = CurPalette;
      fillTo.setBackground(CurPalette[0]);
    }
  }

}