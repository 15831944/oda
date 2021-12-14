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
using Teigha.TG;
using OdKernelExamplesMgdLib.Export.Dwf;

namespace OdDgnExamplesMgdLib.Export.Dwf
{
  public class OdExDgnToDwfExportParamsFiller : OdExDwfExportParamsFiller
  {
    protected override void FillPageData(DwExportParams fillTo, OdRxObject database)
    {
      OdDgDatabase db = OdDgDatabase.cast(database);
      if (db == null)
        throw new Exception("OdDgDatabase is null");

      OdDgModel pModel = OdDgModel.cast(db.getActiveModelId().openObject());

      DwfPageData pageData = new DwfPageData();
      DwfPageDataArray pdArray = new DwfPageDataArray();
      switch (this.DwfExportType)
      {
        case OdExDwfExportType.BinaryDWF_v60:
          pageData.sLayout = pModel.getName();
          pdArray.Add(pageData);
          break;
        case OdExDwfExportType.ZippedStreamDWF_v60:
          pageData.sLayout = pModel.getName();
          pdArray.Add(pageData);
          break;
        case OdExDwfExportType.CompressedDWF_v55:
          break;
        case OdExDwfExportType.BinaryDWF_v55:
          break;
        case OdExDwfExportType.AsciiDWF_v55:
          break;
        case OdExDwfExportType.CompressedDWF_v42:
          break;
        case OdExDwfExportType.BinaryDWF_v42:
          break;
        case OdExDwfExportType.AsciiDWF_v42:
          break;
        case OdExDwfExportType.XPS:
          pageData.sLayout = pModel.getName();
          pdArray.Add(pageData);
          break;
        default:
          throw new Exception("Dwf export format & version not implemented in example");
      }
      fillTo.setPageData(pdArray);
    }
  }
}