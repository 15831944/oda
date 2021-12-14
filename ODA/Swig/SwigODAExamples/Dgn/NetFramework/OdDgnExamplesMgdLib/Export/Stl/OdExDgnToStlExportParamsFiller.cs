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
using OdKernelExamplesMgdLib.Export.Stl;

namespace OdDgnExamplesMgdLib.Export.Stl
{
  public class OdExDgnToStlExportParamsFiller : OdExStlExportParamsFiller
  {
    protected override double GetDeviation(OdRxObject obj)
    {
      // !!! calling an OdGiDrawable method getGeomExrents will give a wrong result
      // !!! we work with a DGN file, thus we should cast to OdDgElement

      OdDgElement dgnObj = OdDgElement.cast(obj);
      if (dgnObj == null)
        throw new Exception("dgnObj is NULL");

      OdGeExtents3d extents = new OdGeExtents3d();
      dgnObj.getGeomExtents(extents);

      return extents.minPoint().distanceTo(extents.maxPoint()) / 100;
    }

    protected override OdGiDrawable GetDrawable(OdRxObject database)
    {
      OdDgDatabase dgDatabase = OdDgDatabase.cast(database);
      if (dgDatabase == null)
        throw new Exception("OdDgDatabase is NULL");

      OdDgElementId targetId = dgDatabase.getElementId(TargetHandle);
      if (targetId == null)
        throw new Exception(String.Format("Not found element with handle: {0}", TargetHandle.ascii()));

      OdRxObject obj = targetId.openObject(OpenMode.kForRead);
      OdGiDrawable drawable = OdGiDrawable.cast(obj);
      if (drawable == null)
        throw new Exception(String.Format("Not drawable element with handle: {0}", TargetHandle.ascii()));

      return drawable;
    }
  }

}