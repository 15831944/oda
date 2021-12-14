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

#include <jni.h>
#include <time.h>
#include <android/log.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <OdaCommon.h>
#include <OdAnsiString.h>
#include <OdCharMapper.h>
#include <diagnostics.h>

#include <Gs/Gs.h>
#include <DbDatabase.h>
#include <GiContextForDbDatabase.h>
#include <DbGsManager.h>
#include "AndroidAppServices.h"
#include <Gs/GsBaseVectorizer.h>
#include <RxDynamicModule.h>
#include <ColorMapping.h>
#include <AbstractViewPE.h>

#ifdef DWF_MODULES_ENABLED
// For DWFimport
#include "DwfImport.h"
using namespace TD_DWF_IMPORT;
#endif

#ifdef COLLADA_ENABLED
#include "ColladaImport.h"
using namespace TD_COLLADA_IMPORT;
#endif
#include "RxVariantValue.h"

// For rasterImport
#include "DbRasterImageDef.h"
#include "DbRasterImage.h"
#include "DbBlockTableRecord.h"
#include "DbDictionary.h"

ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(ModelerModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(ExRasterModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdRasterProcessingServicesImpl);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdOleSsItemHandlerModuleImpl);
#ifdef DWF_MODULES_ENABLED
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(Dwf7ImportModule);
#endif
#if defined(PDFIUM_MODULE_ENABLED_LOCAL)
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT  (PdfUnderlayModule);
#endif
#ifndef ANDROID_GLES2
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(WinOpenGLModule);
#else
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(GLES2Module);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdTrGL2RenderModule);
#endif
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdRecomputeDimBlockModule);
#ifdef COLLADA_ENABLED
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdColladaImportModule);
#endif


#ifdef TA_ANDROID
#include "AECAppStaticModules.h"
ODRX_DECLARE_ARCH_STATIC_MODULES_ENTRY_POINTS()
#endif

#ifdef TC_ANDROID
#include "AECCAppStaticModules.h"
ODRX_DECLARE_CIVIL_STATIC_MODULES_ENTRY_POINTS()
ODRX_DECLARE_CIVIL_UI_STATIC_MODULES_ENTRY_POINTS()
#endif

ODRX_BEGIN_STATIC_MODULE_MAP()
ODRX_DEFINE_STATIC_APPLICATION(OdModelerGeometryModuleName,     ModelerModule)
ODRX_DEFINE_STATIC_APPLICATION(RX_RASTER_SERVICES_APPNAME,      ExRasterModule)
ODRX_DEFINE_STATIC_APPLICATION(OdRasterProcessorModuleName,     OdRasterProcessingServicesImpl)
ODRX_DEFINE_STATIC_APPLICATION(OdOleSsItemHandlerModuleName,          OdOleSsItemHandlerModuleImpl    )
#ifdef DWF_MODULES_ENABLED
ODRX_DEFINE_STATIC_APPMODULE  (OdDwf7ImportModuleName,          Dwf7ImportModule)
#endif
#if defined(PDFIUM_MODULE_ENABLED_LOCAL)
ODRX_DEFINE_STATIC_APPMODULE  (OdPdfiumModuleName, PdfUnderlayModule)
#endif
#ifndef ANDROID_GLES2
ODRX_DEFINE_STATIC_APPLICATION(OdWinOpenGLModuleName,           WinOpenGLModule)
#else
ODRX_DEFINE_STATIC_APPLICATION(OdTrGL2ModuleName,            OdTrGL2RenderModule)
ODRX_DEFINE_STATIC_APPLICATION(OdWinGLES2ModuleName,            GLES2Module)
#endif
ODRX_DEFINE_STATIC_APPLICATION(OdRecomputeDimBlockModuleName,   OdRecomputeDimBlockModule)
#ifdef COLLADA_ENABLED
ODRX_DEFINE_STATIC_APPMODULE  (OdColladaImportModuleName,       OdColladaImportModule)
#endif
#ifdef TA_ANDROID
    ODRX_DEFINE_ARCH_STATIC_APPMODULES()
#endif
#ifdef TC_ANDROID
    ODRX_DEFINE_CIVIL_STATIC_APPMODULES()
    ODRX_DEFINE_CIVIL_UI_STATIC_APPMODULES()
#endif
ODRX_END_STATIC_MODULE_MAP()

#define  LOG_TAG    "TeighaDWGJni"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,  LOG_TAG, __VA_ARGS__)
#define  LOGW(...)  __android_log_print(ANDROID_LOG_WARN,  LOG_TAG, __VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

#define LOG_EXCEPTION(e) LOGE("Exception Caught: %s\n", (const char *)e.description())

static void _MyAssert(const char *expression, const char *fileName, int nLineNo)
{
  LOGW("Assertion Failed: %s, File: %s, Line: %d\n", expression, fileName, nLineNo);
}

static OdString jstringToOdString(JNIEnv *pEnv, jstring javaString)
{
  jboolean isCopy = false;
  const char *pUTF = pEnv->GetStringUTFChars(javaString, &isCopy);
  OdCharArray unicodeBuf;
  OdCharMapper::utf8ToUnicode(pUTF, strlen(pUTF), unicodeBuf);
  pEnv->ReleaseStringUTFChars(javaString, pUTF);
  return OdString(unicodeBuf.getPtr());
}

class TeighaDWGContext
{
  public:
    OdGsDevicePtr gsDevice;
    OdDbDatabasePtr pDb;
};

static TeighaDWGContext * context = NULL;

extern "C" {

JNIEXPORT jboolean JNICALL Java_com_opendesign_android_TeighaDWGJni_init
  (JNIEnv * /*env*/, jclass /*clazz*/)
{
  ODRX_INIT_STATIC_MODULE_MAP();
  odSetAssertFunc(_MyAssert);
  odInitialize(AndroidAppServices::getInstance());

  // Load adinit.dat file
  const char* path = getenv("EXTERNAL_STORAGE");
  if (path)
  {
    OdCharArray unicodeBuf;
    OdCharMapper::utf8ToUnicode(path, strlen(path), unicodeBuf);
    OdString iniFile(unicodeBuf.getPtr());
    iniFile += L"/adinit.dat";
    iniFile = AndroidAppServices::getInstance()->findFile(iniFile);

    if (!iniFile.isEmpty())
      OdCharMapper::initialize(iniFile);
// To add some specific fonts
//    OdCharMapper::addBigFontWithIndex(L"hztxt.shx", 5);
  }

  if(setenv("ACAD", "/mnt/sdcard/fonts", 0) == 0)
    LOGI("ACAD is set");

  OdRxModulePtr pOleHandler = ::odrxDynamicLinker()->loadModule(OdOleSsItemHandlerModuleName);

  LOGI("Loading Ole Handler");

  if ( pOleHandler.isNull() )
      LOGI("Failed loading OLE handler");
  else
      LOGI("Success loading OLE handler");

  return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL Java_com_opendesign_android_TeighaDWGJni_finit
  (JNIEnv *, jclass)
{
  odUninitialize();
  return JNI_TRUE;
}

#if 0
#include "RxRasterServices.h"
void bmpRenderTest()
{
  LOGI("bmpRenderTest begin\n");
  OdGsModulePtr pGs = odrxDynamicLinker()->loadModule(OdWinGLES2ModuleName, false);
  if (!pGs.get())
    return;
  OdGsDevicePtr pDevice = pGs->createBitmapDevice();
  OdGiContextForDbDatabasePtr pCtx = OdGiContextForDbDatabase::createObject();
  pCtx->setDatabase(context->pDb);
  pDevice = OdDbGsManager::setupActiveLayoutViews(pDevice, pCtx);
  int nColorBits = 24;
  int xsize = 2000;
  int ysize = 2000;
  bool plotGeneration = true;
  ODCOLORREF bg = ODRGB(0, 0, 0); // palette background
  OdRxRasterServicesPtr pRasSvcs = odrxDynamicLinker()->loadApp(RX_RASTER_SERVICES_APPNAME);
  ODA_ASSERT_ONCE(pRasSvcs.get())
  if (pRasSvcs.isNull())
    throw OdError(eNotApplicable);
  OdString m_sOutput = OD_T("/sdcard/Bitmap.bmp");
  pDevice->properties()->putAt(OD_T("BitPerPixel"), OdRxVariantValue(OdUInt32(nColorBits)));
  pDevice->setBackgroundColor(ODRGB(173,174,173)); // ACAD's color for paper bg, "device background"
  pCtx->setPlotGeneration(plotGeneration);
  pCtx->setPaletteBackground(bg);
  const ODCOLORREF* palette = odcmAcadPalette(bg);
  OdArray<ODCOLORREF> pPalCpy;
  pPalCpy.insert(pPalCpy.begin(), palette, palette+256);
  pPalCpy[0] = bg;
  pDevice->setLogicalPalette(pPalCpy.asArrayPtr(), 256);
  OdGsDCRect gsRect(0, xsize, ysize, 0 );
  pDevice->onSize(gsRect);
  if(pCtx->isPlotGeneration())
    pDevice->setBackgroundColor(bg);
  else
    pDevice->setBackgroundColor(ODRGB(173,174,173)); // ACAD's color for paper bg, "device background"
  pDevice->update();
  pRasSvcs->saveRasterImage(OdGiRasterImagePtr(pDevice->properties()->getAt(OD_T("RasterImage"))), 
                            m_sOutput);
  LOGI("bmpRenderTest end\n");
}
#endif

JNIEXPORT jboolean JNICALL Java_com_opendesign_android_TeighaDWGJni_open
  (JNIEnv * env, jclass clazz, jstring filename)
{
  LOGI("Java_com_opendesign_android_TeighaDWGJni_open\n");
  if ( context != NULL )
  {
    return JNI_FALSE;
  }
  try
  {
    context = new TeighaDWGContext();
    OdString fileStr = jstringToOdString(env, filename);
    LOGI("Reading file: %s\n", (const char*)fileStr);
    #if DWF_MODULES_ENABLED
    if (!fileStr.right(4).iCompare(OD_T(".dwf")))
    {
      LOGI("Read via DWF importer . . .\n");
      context->pDb = AndroidAppServices::getInstance()->createDatabase();
      OdRxModulePtr pModule = ::odrxDynamicLinker()->loadModule(OdDwf7ImportModuleName);
      if (pModule.isNull())
        throw OdError(OD_T("No DWF import module"));
      OdDwfImportPtr pImporter = ((OdDwfImportModule*)pModule.get())->create();
      if (pImporter.isNull())
        throw OdError(OD_T("Can't create DWF importer"));
      pImporter->properties()->putAt(OD_T("Database"), context->pDb);
      pImporter->properties()->putAt(OD_T("DwfPath"), OdRxVariantValue(AndroidAppServices::getInstance()->findFile(fileStr)));
      pImporter->properties()->putAt(OD_T("PreserveColorIndices"), OdRxVariantValue(false) );
      pImporter->properties()->putAt(OD_T("DwgPalette"), OdRxVariantValue(OdIntPtr(::odcmAcadLightPalette())) );

      try
      {
        pImporter->import();
      }
      catch(const OdError &)
      {
        throw;
      }
      catch(...)
      {
        throw OdError(OD_T("Unknown DWF import exception"));
      }
      //catch (const DWFCore::DWFException &dwfEx)
      //{
      //  OdString dwfExS;
      //  dwfExS.format(OD_T("DWFException : Type: %ls\nMessage: %ls\nFunction: %ls\nFile: %ls\nLine: %u"),
      //                dwfEx.type(), dwfEx.message(), dwfEx.function(), dwfEx.file(), dwfEx.line());
      //  throw OdError(dwfExS);
      //}
    }
    else
    #endif
    #if defined(COLLADA_ENABLED)
    if (!fileStr.right(4).iCompare(".dae"))
    {
      LOGI("Read via Collada importer . . .\n");
      context->pDb = AndroidAppServices::getInstance()->createDatabase();
      OdRxModulePtr pModule = ::odrxDynamicLinker()->loadModule(OdColladaImportModuleName);
      if (pModule.isNull())
        throw OdError(OD_T("No Collada import module"));
      OdColladaImportPtr pImporter = TD_COLLADA_IMPORT::createImporter();
      if (pImporter.isNull())
        throw OdError(OD_T("Can't create Collada importer"));
      pImporter->properties()->putAt(OD_T("Database"), context->pDb);
      pImporter->properties()->putAt(OD_T("ColladaPath"), OdRxVariantValue(AndroidAppServices::getInstance()->findFile(fileStr)));
      pImporter->properties()->putAt(OD_T("ImportTextures"), OdRxVariantValue(true));
      pImporter->properties()->putAt(OD_T("ConsoleInfo"), OdRxVariantValue(true));
      try
      {
        pImporter->import();
      }
      catch(const OdError &)
      {
        throw;
      }
      catch(...)
      {
        throw OdError(OD_T("Unknown Collada import exception"));
      }
    } else
    #endif
    if (!fileStr.right(4).iCompare(OD_T(".bmp")) ||
             !fileStr.right(4).iCompare(OD_T(".jpg")) ||
             !fileStr.right(4).iCompare(OD_T(".png")) ||
             !fileStr.right(4).iCompare(OD_T(".tif")))
    {
      LOGI("Read via raster image importer . . .\n");
      context->pDb = AndroidAppServices::getInstance()->createDatabase();
      OdDbBlockTableRecordPtr pRecord = context->pDb->getActiveLayoutBTRId().safeOpenObject(OdDb::kForWrite) ;
      OdDbObjectId imageDictId = OdDbRasterImageDef::createImageDictionary(pRecord->database());
      OdDbDictionaryPtr pImageDict = imageDictId.safeOpenObject(OdDb::kForWrite);
      // Create image definition
      OdDbObjectId defId;
      OdDbRasterImageDefPtr pDef = OdDbRasterImageDef::createObject();
      pDef->setSourceFileName(fileStr);
      defId = pImageDict->setAt(OD_T("importedRasterImage"), pDef);
      // Try to load raster image to found correct dimensions inside TD
      OdDbRasterImageDefPtr pDefLoad = OdDbRasterImageDef::cast(defId.openObject(OdDb::kForWrite));
      pDefLoad->load(false);
      // Create and attach raster image reference
      OdDbRasterImagePtr pRef = OdDbRasterImage::createObject();
      pRef->setDatabaseDefaults(context->pDb);
      pRef->setImageDefId(defId);
      pRef->setOrientation(OdGePoint3d::kOrigin, OdGeVector3d::kXAxis, OdGeVector3d::kYAxis);
      pRef->setDisplayOpt(OdDbRasterImage::kShow, true);
      pRecord->appendOdDbEntity(pRef);
    }
    else
    {
#if defined(PDFIUM_MODULE_ENABLED_LOCAL)
        OdRxModulePtr pRxModule = ::odrxDynamicLinker()->loadModule(OdPdfiumModuleName);
        if (pRxModule.isNull()) {
            OdString tmp = OdString("Missing ") + OdPdfiumModuleName + ".\n";
            odPrintConsoleString(tmp);
            throw OdError(eTxError);
        }
        odPrintConsoleString(L"PdfiumModule has been loaded\n");
#endif
      context->pDb = AndroidAppServices::getInstance()->readFile(fileStr);
    }
    LOGI("Read complete\n");
    //bmpRenderTest();
  }
  catch(OdError & e)
  {
    LOG_EXCEPTION(e);
    delete context;
    context = NULL;
    return JNI_FALSE;
  }
  return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL Java_com_opendesign_android_TeighaDWGJni_createRenderer
  (JNIEnv * env, jclass clazz , jint width, jint height)
{
  LOGI("Java_com_opendesign_android_TeighaDWGJni_createRenderer\n");
  if ( context == NULL )
  {
    LOGE("Context is null\n");
    return JNI_FALSE;
  }
  try
  {
    if (!context->gsDevice.isNull())
    {
      context->gsDevice.release();
    }
    OdGiContextForDbDatabasePtr pDwgContext = OdGiContextForDbDatabase::createObject();
#ifndef ANDROID_GLES2
    OdGsModulePtr m = ::odrxDynamicLinker()->loadModule(OdWinOpenGLModuleName);
#else
    OdGsModulePtr m = ::odrxDynamicLinker()->loadModule(OdWinGLES2ModuleName);
#endif
    if ( m.isNull() )
    {
      LOGE("Could not load OpenGL module\n");
      return JNI_FALSE;
    }
    context->gsDevice = m->createDevice();
    if ( context->gsDevice.isNull())
    {
      return JNI_FALSE;
    }
    // Avoid context creation by device, since we manage it on Java side
    if (!context->gsDevice->properties().isNull() &&
        context->gsDevice->properties()->has(OD_T("CreateContext")))
    {
      context->gsDevice->properties()->putAt(OD_T("CreateContext"), OdRxVariantValue(false));
    }
    // Set the database to be vectorized.
    pDwgContext->setDatabase(context->pDb);
    pDwgContext->enableGsModel(true);
    // Prepare the device to render the active layout in this database.
    context->gsDevice = OdDbGsManager::setupActiveLayoutViews(context->gsDevice, pDwgContext);
    // Setup device palette
    if (OdGsPaperLayoutHelper::cast(context->gsDevice).isNull())
    {
      // ModelSpace
      context->gsDevice->setLogicalPalette(::odcmAcadDarkPalette(), 256);
      context->gsDevice->setBackgroundColor(ODRGB(0, 0, 0));
      pDwgContext->setPaletteBackground(ODRGB(0, 0, 0));
    }
    else
    {
      context->gsDevice->setLogicalPalette(::odcmAcadLightPalette(), 256);
      context->gsDevice->setBackgroundColor(ODRGB(173, 174, 173));
      pDwgContext->setPaletteBackground(ODRGB(255, 255, 255));
    }

    // Set the screen size for the generated geometry.
    OdGsDCRect screenRect( OdGsDCPoint(0, height), OdGsDCPoint(width, 0) );
    context->gsDevice->onSize(screenRect);

    OdAbstractViewPEPtr(context->gsDevice->viewAt(0))->zoomExtents(context->gsDevice->viewAt(0));

  }
  catch(OdError &e)
  {
    LOG_EXCEPTION(e);
    return JNI_FALSE;
  }
  return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL Java_com_opendesign_android_TeighaDWGJni_renderFrame
  (JNIEnv *, jclass)
{
  if (context == NULL ||
      context->gsDevice.isNull())
  {
    return JNI_FALSE;
  }
  // for testing
  //glClearColor((GLfloat)rand() / 255, (GLfloat)rand() / 255, (GLfloat)rand() / 255, 0);
  //glClear(GL_COLOR_BUFFER_BIT);
  try
  {
    context->gsDevice->invalidate(); // it's required to completely redraw in each buffer
    context->gsDevice->update();
  }
  catch(OdError &e)
  {
    LOG_EXCEPTION(e);
    return JNI_FALSE;
  }
  return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL Java_com_opendesign_android_TeighaDWGJni_destroyRenderer
  (JNIEnv *, jclass)
{
  LOGI("Java_com_opendesign_android_TeighaDWGJni_destroyRenderer\n");
  if (context != NULL)
    context->gsDevice.release();
  return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL Java_com_opendesign_android_TeighaDWGJni_close
  (JNIEnv *, jclass)
{
  if ( context == NULL )
  {
    return JNI_FALSE;
  }
  if (!context->gsDevice.isNull())
  {
    context->gsDevice.release();
  }
  if (!context->pDb.isNull())
  {
    context->pDb.release();
  }
  delete context;
  context = NULL;
  return JNI_TRUE;
}

static OdGePoint3d toWCS(jfloat x, jfloat y)
{
  OdGePoint3d wcsPt((double)x, (double)y, 0.0);
  const OdGsView* pView = context->gsDevice->viewAt(0);
  if (pView->isPerspective())
  {
    wcsPt.z = pView->projectionMatrix()(2, 3);
  }
  wcsPt.transformBy((pView->screenMatrix() * pView->projectionMatrix()).inverse());
  wcsPt.z =0.;
  wcsPt.transformBy(OdAbstractViewPEPtr(pView)->eyeToWorld(pView));
  return wcsPt;
}

JNIEXPORT jboolean JNICALL Java_com_opendesign_android_TeighaDWGJni_viewTranslate
  (JNIEnv * env, jclass clazz, jfloat xAxis, jfloat yAxis)
{
  if (context == NULL)
  {
    LOGE("Context is null\n");
    return JNI_FALSE;
  }
  if (context->gsDevice.isNull())
  {
    return JNI_FALSE;
  }
  OdGsView* pView = context->gsDevice->viewAt(0);
  OdGeVector3d delta = toWCS(-xAxis, -yAxis) - toWCS(0.0f, 0.0f);
  delta.transformBy(pView->viewingMatrix());
  pView->dolly(delta.x, delta.y, delta.z);
  return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL Java_com_opendesign_android_TeighaDWGJni_viewScale
  (JNIEnv * env, jclass clazz, jfloat sCoef)
{
  if (context == NULL)
  {
    LOGE("Context is null\n");
    return JNI_FALSE;
  }
  if (context->gsDevice.isNull())
  {
    return JNI_FALSE;
  }
  OdGsView* pView = context->gsDevice->viewAt(0);
  pView->zoom((double)sCoef);
  return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL Java_com_opendesign_android_TeighaDWGJni_viewCanRotate
  (JNIEnv * env, jclass clazz)
{
  if (context == NULL)
  {
    LOGE("Context is null\n");
    return JNI_FALSE;
  }
  if (context->pDb.isNull())
  {
    return JNI_FALSE;
  }
  return (context->pDb->getTILEMODE()) ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL Java_com_opendesign_android_TeighaDWGJni_viewRotate
  (JNIEnv * env, jclass clazz, jfloat rAngle)
{
  if (context == NULL)
  {
    LOGE("Context is null\n");
    return JNI_FALSE;
  }
  if (context->gsDevice.isNull())
  {
    return JNI_FALSE;
  }
  OdGsView* pView = context->gsDevice->viewAt(0);
  pView->roll((double)rAngle);
  return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL Java_com_opendesign_android_TeighaDWGJni_viewOrbit
  (JNIEnv * env, jclass clazz, jfloat xAxis, jfloat yAxis)
{
  LOGI("Java_com_opendesign_android_TeighaDWGJni_viewOrbit\n");
  LOGI("Java_com_opendesign_android_TeighaDWGJni_viewTranslate\n");
  if (context == NULL)
  {
    LOGE("Context is null\n");
    return JNI_FALSE;
  }
  if (context->gsDevice.isNull())
  {
    return JNI_FALSE;
  }
  OdGsView* pView = context->gsDevice->viewAt(0);
  OdGeVector3d delta((double)xAxis, (double)yAxis, 0.0);
  pView->orbit(delta.y, delta.x);
  return JNI_TRUE;
}

JNIEXPORT jint JNICALL Java_com_opendesign_android_TeighaDWGJni_viewGetRenderMode
  (JNIEnv * env, jclass clazz)
{
  LOGI("Java_com_opendesign_android_TeighaDWGJni_viewGetRenderMode\n");
  if (context == NULL)
  {
    LOGE("Context is null\n");
    return JNI_FALSE;
  }
  if (context->gsDevice.isNull())
  {
    return JNI_FALSE;
  }
  OdGsView* pView = context->gsDevice->viewAt(0);
  return (jint)pView->mode();
}

JNIEXPORT jboolean JNICALL Java_com_opendesign_android_TeighaDWGJni_viewSetRenderMode
  (JNIEnv * env, jclass clazz, jint nMode)
{
  LOGI("Java_com_opendesign_android_TeighaDWGJni_viewSetRenderMode\n");
  if (context == NULL)
  {
    LOGE("Context is null\n");
    return JNI_FALSE;
  }
  if (context->gsDevice.isNull())
  {
    return JNI_FALSE;
  }

  OdGsView* pView = context->gsDevice->viewAt(0);
  pView->setMode((OdGsView::RenderMode)nMode);
  return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL Java_com_opendesign_android_TeighaDWGJni_viewRegen
  (JNIEnv * env, jclass clazz, jint nMode)
{
  LOGI("Java_com_opendesign_android_TeighaDWGJni_viewRegen\n");
  if (context == NULL)
  {
    LOGE("Context is null\n");
    return JNI_FALSE;
  }
  if (context->gsDevice.isNull())
  {
    return JNI_FALSE;
  }
  OdGsLayoutHelperPtr pDevice = context->gsDevice;
  pDevice->invalidate();
  if (pDevice->gsModel())
    pDevice->gsModel()->invalidate(OdGsModel::kInvalidateAll);
  return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL Java_com_opendesign_android_TeighaDWGJni_saveContext
  (JNIEnv * env, jclass clazz, jint saveContextValue)
{
  if (context == NULL)
  {
    LOGE("Context is null\n");
    return JNI_FALSE;
  }
  if (context->gsDevice.isNull())
  {
    LOGE("Device is null\n");
    return JNI_FALSE;
  }
  OdRxDictionaryPtr pProperties = context->gsDevice->properties();
  if (saveContextValue)
  {
    //Put OpenGL resources into system memory
    pProperties->putAt(OD_T("SaveContextData"), OdRxVariantValue(true));
  } else
  {
    //Take resources from the memory
    pProperties->putAt(OD_T("SaveContextData"), OdRxVariantValue(false));
  }
  return JNI_TRUE;
}

} // extern "C"
