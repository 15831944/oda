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
using System.Windows.Forms;
using Teigha.Core;
using Teigha.TD;
using OdDrawingsExamplesUIMgdLib;
using OdKernelExamplesMgdLib;

namespace OdaDwgAppSwigMgd
{
  public class OdExHostApplicationServices : ExHostAppServices
  {
    private bool m_bPartial;
    private bool m_bRecover;
    public void setPartialOption(bool val) { m_bPartial = val; }
    public void setRecoverOption(bool val) { m_bRecover = val; }
    public bool isRecoverOption() { return m_bRecover; }
    public bool isPartial() { return m_bPartial; }

    #region Audit
    public override void auditPrintReport(OdAuditInfo pAuditInfo, String strLine, int nPrintDest)
    {
      if (m_pAuditDlg != null)
        m_pAuditDlg.printReport((OdDbAuditInfo)pAuditInfo);
    }

    private OdExAuditInfoDlg m_pAuditDlg = null;
    public OdExAuditInfoDlg getAuditInfoDlg()
    {
      return m_pAuditDlg;
    }

    public void InitAuditInfoDlg()
    {
      m_pAuditDlg = new OdExAuditInfoDlg();
    }
    #endregion

    // override this method for some purpose as
    //    to be able to perform DWF export of rasters
    //    to be able to export shaded viewports using OpenGL device
    //    and so on..
    public override OdGsDevice gsBitmapDevice(OdRxObject pViewObj, OdRxObject pDb, uint flags)
    {
      OdGsModule pGsModule = null;
      OdGsDevice gsDevice = null;
      ExHostAppServices.GsBitmapDeviceFlags deviceFlags = (ExHostAppServices.GsBitmapDeviceFlags)flags;

      try
      {
        ExHostAppServices.GsBitmapDeviceFlags aaa = (deviceFlags & ExHostAppServices.GsBitmapDeviceFlags.kFor2dExportRender);
        ExHostAppServices.GsBitmapDeviceFlags bbb = (deviceFlags & ExHostAppServices.GsBitmapDeviceFlags.kFor2dExportRenderHLR);
        if ((deviceFlags & ExHostAppServices.GsBitmapDeviceFlags.kFor2dExportRender) != 0)
        {
          // Don't export HiddenLine viewports as bitmap in Pdf/Dwf/Svg exports.
          if ((deviceFlags & ExHostAppServices.GsBitmapDeviceFlags.kFor2dExportRenderHLR) == 0)
          {
            // Try to export shaded viewports using OpenGL device.
            pGsModule = (OdGsModule)Globals.odrxDynamicLinker().loadModule("WinOpenGL.txv");
          }
        }
        if (pGsModule != null)
        {
          gsDevice = pGsModule.createBitmapDevice();
        }
      }
      catch (OdError err)
      {
      }
      return gsDevice;
    }

    public override string fileDialog(int flags, string dialogCaption, string defExt, string defFilename, string filter)
    {
      SaveFileDialog dlg = new SaveFileDialog();
      dlg.DefaultExt = defExt;
      dlg.Title = dialogCaption;
      dlg.FileName = defFilename;
      dlg.Filter = filter;
      if (dlg.ShowDialog() == DialogResult.OK)
        return dlg.FileName;

      return String.Empty;
    }

    public override String findFile(String fileName, OdRxObject pDb, OdDbBaseHostAppServices.FindFileHint hint)
    {
      String sFile = base.findFile(fileName, pDb, hint);
      if (sFile.Length > 0)
        return sFile;

      String strFileName = fileName;
      String ext;
      if (strFileName.Length > 3)
        ext = strFileName.Substring(strFileName.Length - 4, 4).ToUpper();
      else
        ext = fileName.ToUpper();
      if (ext == String.Format(".PC3"))
        return OdExAppRegistryFindConfigHelpers.FindConfigFile(String.Format("PrinterConfigDir"), fileName);
      if (ext == String.Format(".STB") || ext == String.Format(".CTB"))
        return OdExAppRegistryFindConfigHelpers.FindConfigFile(String.Format("PrinterStyleSheetDir"), fileName);
      //return System.IO.Path.Combine(Environment.GetEnvironmentVariable("DDPLOTSTYLEPATHS"), fileName);
      if (ext == String.Format(".PMP"))
        return OdExAppRegistryFindConfigHelpers.FindConfigFile(String.Format("PrinterDescDir"), fileName);

      switch (hint)
      {
        case FindFileHint.kFontFile:
        case FindFileHint.kCompiledShapeFile:
        case FindFileHint.kTrueTypeFontFile:
        case FindFileHint.kPatternFile:
        case FindFileHint.kFontMapFile:
        case FindFileHint.kTextureMapFile:
          break;
        default:
          return sFile;
      }

      if (hint != FindFileHint.kTextureMapFile && ext != String.Format(".SHX") && ext != String.Format(".PAT") && ext != String.Format(".TTF") && ext != String.Format(".TTC"))
      {
        strFileName += String.Format(".shx");
      }
      else if (hint == FindFileHint.kTextureMapFile)
      {
        strFileName.Replace(String.Format("/"), String.Format("\\"));
        int last = strFileName.LastIndexOf("\\");
        if (last == -1)
          strFileName = "";
        else
          strFileName = strFileName.Substring(0, last);
      }


      sFile = (hint != FindFileHint.kTextureMapFile) ? OdExAppRegistryFindConfigHelpers.GetRegistryACADFromProfile() : OdExAppRegistryFindConfigHelpers.GetRegistryAVEMAPSFromProfile();
      while (sFile.Length > 0)
      {
        int nFindStr = sFile.IndexOf(";");
        String sPath;
        if (-1 == nFindStr)
        {
          sPath = sFile;
          sFile = String.Format("");
        }
        else
        {
          sPath = String.Format("{0}\\{1}", sFile.Substring(0, nFindStr), strFileName);
          if (System.IO.File.Exists(sPath))
          {
            return sPath;
          }
          sFile = sFile.Substring(nFindStr + 1, sFile.Length - nFindStr - 1);
        }
      }

      if (hint == FindFileHint.kTextureMapFile)
      {
        return sFile;
      }

      if (sFile.Length <= 0)
      {
        String sAcadLocation = OdExAppRegistryFindConfigHelpers.GetRegistryAcadLocation();
        if (sAcadLocation.Length > 0)
        {
          sFile = String.Format("{0}\\Fonts\\{1}", sAcadLocation, strFileName);
          if (System.IO.File.Exists(sFile))
          {
            sFile = String.Format("{0}\\Support\\{1}", sAcadLocation, strFileName);
            if (System.IO.File.Exists(sFile))
            {
              sFile = String.Format("");
            }
          }
        }
      }
      return sFile;
    }

    #region Undo
    private OdStreamBuf undoStr = null;
    private bool undoType = false;
    public void setUndoType(bool value) { undoType = value; }
    public bool getUndoType() { return undoType; }
    public override OdDbUndoController newUndoController()
    {
      if (null == undoStr)
      {
        undoStr = TD_Db.newUndoStream(this);
      }
      return TD_Db.newUndoController(undoType, undoStr);
    }
    public override OdStreamBuf newUndoStream()
    {
      undoStr = TD_Db.newUndoStream(this);
      return undoStr;
    }
    #endregion

  }
}
