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
using System.IO;
using Teigha.TD;
using System.Threading;
using System.Linq;

namespace OdExMultiThreadSwigExamples
{
  public class RenderDbToImageContext
  {
    public String m_pRenderDeviceName;
    public long m_picWidth, m_picHeight;
    public OdDbHostAppServices m_pServices;
    public RenderDbToImageContext setup(String pRenderDevice, long picWidth, long picHeight, OdDbHostAppServices pServices)
    {
      m_pRenderDeviceName = pRenderDevice; m_picWidth = picWidth; m_picHeight = picHeight; m_pServices = pServices;
      return this;
    }
  };

  public class OdExMT_LowLevelApiSample
  {
    /// <summary>
    /// Sample about using Multithreaded Low Level API from article: https://www.opendesign.com/blog/2018/january/teigha-multithreading-low-level-api-part-1-4
    /// </summary>
    public static void Run(String folderPath)
    {
      MemoryTransaction mTr = MemoryManager.GetMemoryManager().StartTransaction();

      try
      {
        var inDwgFiles = Directory.EnumerateFiles(folderPath, "*.dwg");

        OdExHostAppServices hostApp = new OdExHostAppServices();
        hostApp.disableOutput(true);

        RenderDbToImageContext renderDbContext = new RenderDbToImageContext().setup("WinGLES2.txv", 1024, 768, hostApp);

        OdExSimpleWinNetThreadsPool winThreadPool = new OdExSimpleWinNetThreadsPool();

        Action<String> generatedFunc = new Action<String>((result) => { Console.WriteLine(result); });

        // Run loading and rendering process
        foreach (String inputFilePath in inDwgFiles)
        {
          RenderDbToImageCaller caller = new RenderDbToImageCaller();
          caller.GeneratedResult += generatedFunc;
          caller.setup(inputFilePath, renderDbContext).run(winThreadPool);
        }

        // Wait threads completion
        winThreadPool.Wait();
      }
      finally
      {
        MemoryManager.GetMemoryManager().StopTransaction(mTr);
      }
    }

    private class RenderDbToImageCaller
    {
      private String m_inputFile;
      private OdGiRasterImage m_pOutputImage;
      private RenderDbToImageContext m_pThreadCtx;

      public event Action<String> GeneratedResult;

      public RenderDbToImageCaller setup(String inputFile, RenderDbToImageContext pThreadCtx)
      {
        m_inputFile = inputFile; m_pThreadCtx = pThreadCtx;
        return this;
      }

      static void entryPoint(object pArg)
      {
        MemoryTransaction mTr = MemoryManager.GetMemoryManager().StartTransaction();
        try
        {
          Globals.odThreadsCounter().startThread();
          RenderDbToImageCaller pCaller = (RenderDbToImageCaller)pArg;
          OdDbDatabase pDb = pCaller.m_pThreadCtx.m_pServices.readFile(pCaller.m_inputFile);
          if (pDb == null)
          {
            Globals.odThreadsCounter().stopThread();
            return;
          }

          pCaller.m_pOutputImage = OdExSampleFunctionality.renderDbToImage(pDb, pCaller.m_pThreadCtx.m_pRenderDeviceName, pCaller.m_pThreadCtx.m_picWidth, pCaller.m_pThreadCtx.m_picHeight);

          bool successSave = OdExSampleFunctionality.SaveRasterImage(pCaller.m_pOutputImage, pCaller.m_inputFile);

          if (pCaller.GeneratedResult != null)
          {
            if (successSave)
              pCaller.GeneratedResult(String.Format("Success for: {0}", pCaller.m_inputFile));
            else
              pCaller.GeneratedResult(String.Format("Not generated for: {0}", pCaller.m_inputFile));
          }

          Globals.odThreadsCounter().stopThread();
        }
        finally
        {
          MemoryManager.GetMemoryManager().StopTransaction(mTr);
        }
        return;
      }

      public RenderDbToImageCaller run(OdExSimpleWinNetThreadsPool threadPool)
      {
        threadPool.RunNewThread(entryPoint, this, (uint)(ThreadsCounter.ThreadAttributes.kMtLoadingAttributes | ThreadsCounter.ThreadAttributes.kMtRegenAttributes));
        return this;
      }
    };
  }

  public class OdExMT_HighLevelApiSample
  {
    /// <summary>
    /// Sample about using Multithreaded High Level API from article: https://www.opendesign.com/blog/2017/december/teigha-multithreading-high-level-api-part-1-3
    /// </summary>
    public static void Run(String folderPath)
    {
      MemoryTransaction mTr = MemoryManager.GetMemoryManager().StartTransaction();

      try
      {
        var inDwgFiles = Directory.EnumerateFiles(folderPath, "*.dwg");

        OdExHostAppServices hostApp = new OdExHostAppServices();
        hostApp.disableOutput(true);

        RenderDbToImageContext renderDbContext = new RenderDbToImageContext().setup("WinGLES2.txv", 1024, 768, hostApp);

        // Load thread pool module
        OdRxThreadPoolService pThreadPool = (OdRxThreadPoolService)Globals.odrxDynamicLinker().loadApp("ThreadPool");
        if (pThreadPool == null)
          throw new OdError(OdResult.eNullPtr); // ThreadPool.tx not found.

        OdApcQueue pMTQueue = pThreadPool.newMTQueue((uint)(ThreadsCounter.ThreadAttributes.kMtLoadingAttributes | ThreadsCounter.ThreadAttributes.kMtRegenAttributes));

        IntPtr renderDbContextPtr = System.Runtime.InteropServices.Marshal.GetIUnknownForObject(renderDbContext);

        Action<String> generatedFunc = new Action<String>((result) => { Console.WriteLine(result); });

        // Run loading and rendering process
        foreach (String inputFile in inDwgFiles)
        {
          var caller = new RenderDbToImageCaller().setup(inputFile);
          caller.GeneratedResult += generatedFunc;
          pMTQueue.addEntryPoint(caller, renderDbContextPtr);
        }

        pMTQueue.wait();

        System.Runtime.InteropServices.Marshal.Release(renderDbContextPtr);
      }
      finally
      {
        MemoryManager.GetMemoryManager().StopTransaction(mTr);
      }

    }

    // Thread running method implementation
    private class RenderDbToImageCaller : OdApcAtom
    {
      public event Action<String> GeneratedResult;

      public RenderDbToImageCaller() : base()
      {
      }

      String m_inputFile;
      OdGiRasterImage m_pOutputImage;
      public RenderDbToImageCaller setup(String inputFile)
      {
        m_inputFile = inputFile;
        return this;
      }

      public override void apcEntryPoint(IntPtr pMessage)
      {
        MemoryTransaction mTr = MemoryManager.GetMemoryManager().StartTransaction();

        try
        {
          RenderDbToImageContext pContext = System.Runtime.InteropServices.Marshal.GetObjectForIUnknown(pMessage) as RenderDbToImageContext;
          OdDbDatabase pDb = pContext.m_pServices.readFile(m_inputFile);
          if (pDb == null)
            return;
          m_pOutputImage = OdExSampleFunctionality.renderDbToImage(pDb, pContext.m_pRenderDeviceName, pContext.m_picWidth, pContext.m_picHeight);

          bool successSave = OdExSampleFunctionality.SaveRasterImage(m_pOutputImage, m_inputFile);

          if (GeneratedResult != null)
          {
            if (successSave)
              GeneratedResult(String.Format("Success for: {0}", m_inputFile));
            else
              GeneratedResult(String.Format("Not generated for: {0}", m_inputFile));
          }
        }
        finally
        {
          MemoryManager.GetMemoryManager().StopTransaction(mTr);
        }
      }
    };
  };

  public class OdExSampleFunctionality
  {
    public static object locker_createBitmapDevice = new object();

    public static OdGiRasterImage renderDbToImage(OdDbDatabase pDb, String pRenderDeviceName, long picWidth, long picHeight)
    {
      // Create vectorization context
      OdGiContextForDbDatabase pDbCtx = OdGiContextForDbDatabase.createObject();
      // Create rendering device
      OdGsModule pGsModule = (OdGsModule)Globals.odrxDynamicLinker().loadModule(pRenderDeviceName);
      OdGsDevice pDevice = null;
      lock (locker_createBitmapDevice) { pDevice = pGsModule.createBitmapDevice(); }

      pDbCtx.setDatabase(pDb);
      // Initialize rendering device
      pDevice = TD_Db.setupActiveLayoutViews(pDevice, pDbCtx);

      uint[] CurPalette = Teigha.Core.AllPalettes.getDarkPalette();
      pDevice.setLogicalPalette(CurPalette, 256);
      pDevice.setBackgroundColor(CurPalette[0]);
      pDbCtx.setPaletteBackground(CurPalette[0]);
      // Setup size of output contents
      pDevice.onSize(new OdGsDCRect(new OdGsDCPoint(0, (int)picHeight), new OdGsDCPoint((int)picWidth, 0)));
      // Zoom into model
      OdAbstractViewPE.cast(pDevice.viewAt(0)).zoomExtents(pDevice.viewAt(0));
      // Render
      pDevice.update();

      // Create clone of rendered raster image to correctly release all vectorizer resources in current thread
      OdGiRasterImage pRaster = OdGiRasterImageHolder.createObject(OdGiRasterImage.cast(pDevice.properties().getAt("RasterImage")));
      // Return rendered raster image
      return pRaster;
    }

    public static bool SaveRasterImage(OdGiRasterImage outputImage, String inputFilePath)
    {
      OdRxRasterServices rasterServices = (OdRxRasterServices)Teigha.Core.Globals.odrxDynamicLinker().loadApp("RxRasterServices");
      if (rasterServices == null)
        throw new Exception("Failed to load RxRasterServices module");

      String saveFolderpath = Path.GetDirectoryName(inputFilePath);
      String inFileName = Path.GetFileNameWithoutExtension(inputFilePath);
      String saveFilePath = Path.Combine(saveFolderpath, String.Format("image_{0}.bmp", inFileName));

      return rasterServices.saveRasterImage(outputImage, saveFilePath);
    }
  }
}