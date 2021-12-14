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

namespace ExDgnDumpSwigMgd
{
  class OdDgReferenceHeader_Dumper : OdRxObject_Dumper
  {
    public override void dump(OdRxObject pObject)
    {
      MemoryTransaction mTr = MemoryManager.GetMemoryManager().StartTransaction();
      OdDgReferenceAttachmentHeader element = (OdDgReferenceAttachmentHeader)pObject;
      startDescription(element);
      writeFieldValue("The full reference path", element.getFullFileName());
      writeFieldValue("The base file name", element.getFileName());
      writeFieldValue("ModelName", element.getModelName());
      writeFieldValue("LogicalName", element.getLogicalName());
      writeFieldValue("MasterOrigin", element.getMasterOrigin());
      writeFieldValue("ReferenceOrigin", element.getReferenceOrigin());
      writeFieldValue("Transformation", element.getTransformation());

      writeFieldValue("Description", element.getDescription());
      writeFieldValue("FileNumber", element.getFileNumber());
      writeFieldValue("Priority", element.getPriority());
      writeFieldValue("BaseNestDepth", element.getBaseNestDepth());
      writeFieldValue("NestDepth", element.getNestDepth());
      writeFieldValue("Scale", element.getScale());
      //writeFieldValue( "Entire Scale", element.getEntireScale() );
      writeFieldValue("Foreign Unit ", (UInt16)(element.getForeignUnits()));
      writeFieldValue("ZFront", element.getZFront());
      writeFieldValue("ZBack", element.getZBack());
      writeFieldValue("CameraPosition", element.getCameraPosition());
      writeFieldValue("CameraFocalLength", element.getCameraFocalLength());

      writeFieldValue("ClipPointsCount", element.getClipPointsCount());
      for (UInt32 i = 0, nCount = element.getClipPointsCount(); i < nCount; i++)
      {
        writeShift();
        writeFieldValue(string.Format("ClipPoint {0}", i), element.getClipPoint(i));
      }

      writeFieldValue("CoincidentFlag", element.getCoincidentFlag());
      writeFieldValue("SnapLockFlag", element.getSnapLockFlag());
      writeFieldValue("LocateLockFlag", element.getLocateLockFlag());
      writeFieldValue("CompletePathInV7Flag", element.getCompletePathInV7Flag());
      writeFieldValue("AnonymousFlag", element.getAnonymousFlag());
      writeFieldValue("InactiveFlag", element.getInactiveFlag());
      writeFieldValue("MissingFileFlag", element.getMissingFileFlag());
      writeFieldValue("LevelOverride", element.getLevelOverride());
      writeFieldValue("DontDetachOnAllFlag", element.getDontDetachOnAllFlag());
      writeFieldValue("MetadataOnlyFlag", element.getMetadataOnlyFlag());
      writeFieldValue("DisplayFlag", element.getDisplayFlag());
      writeFieldValue("LineStyleScaleFlag", element.getLineStyleScaleFlag());
      writeFieldValue("HiddenLineFlag", element.getHiddenLineFlag());
      writeFieldValue("DisplayHiddenLinesFlag", element.getDisplayHiddenLinesFlag());
      writeFieldValue("RotateClippingFlag", element.getRotateClippingFlag());
      writeFieldValue("ExtendedRefFlag", element.getExtendedRefFlag());
      writeFieldValue("ClipBackFlag", element.getClipBackFlag());
      writeFieldValue("ClipFrontFlag", element.getClipFrontFlag());
      writeFieldValue("CameraOnFlag", element.getCameraOnFlag());
      writeFieldValue("TrueScaleFlag", element.getTrueScaleFlag());
      writeFieldValue("DisplayBoundaryFlag", element.getDisplayBoundaryFlag());
      writeFieldValue("LibraryRefFlag", element.getLibraryRefFlag());
      writeFieldValue("DisplayRasterRefsFlag", element.getDisplayRasterRefsFlag());
      writeFieldValue("UseAlternateFileFlag", element.getUseAlternateFileFlag());
      writeFieldValue("UseLightsFlag", element.getUseLightsFlag());
      writeFieldValue("DoNotDisplayAsNestedFlag", element.getDoNotDisplayAsNestedFlag());
      writeFieldValue("ColorTableUsage", element.getColorTableUsage());
      writeFieldValue("ViewportFlag", element.getViewportFlag());
      writeFieldValue("ScaleByStorageUnitsFlag", element.getScaleByStorageUnitsFlag());
      writeFieldValue("PrintColorAdjustmentFlag", element.getPrintColorAdjustmentFlag());
      finishDescription();
      MemoryManager.GetMemoryManager().StopTransaction(mTr);
    }
    public override String getClassName()
    {
      return "OdDgReferenceAttachmentHeader";
    }
  }
}