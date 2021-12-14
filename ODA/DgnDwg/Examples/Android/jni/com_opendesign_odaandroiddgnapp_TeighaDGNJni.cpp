/////////////////////////////////////////////////////////////////////////////// 
// Copyright (C) 2002-2020, Open Design Alliance (the "Alliance"). 
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
//   Open Design Alliance Copyright (C) 2002-2020 by Open Design Alliance. 
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
#include <diagnostics.h>

#include <Gs/Gs.h>
#include <DgDatabase.h>
#include <GiContextForDbDatabase.h>
#include <DbGsManager.h>
#include "AndroidAppServices.h"
#include <Gs/GsBaseVectorizer.h>
#include <RxDynamicModule.h>
#include <ColorMapping.h>
#include <AbstractViewPE.h>
#include <DgGiContext.h>
#include "RxVariantValue.h"

// For rasterImport
#include "DbRasterImageDef.h"
#include "DbRasterImage.h"
#include "DbBlockTableRecord.h"
#include "DbDictionary.h"

#include "DgGsManager.h"
#include "DgGiContext.h"

ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(ModelerModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(ExRasterModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdRasterProcessingServicesImpl);
#ifndef ANDROID_GLES2
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(WinOpenGLModule);
#else
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdTrGL2RenderModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(GLES2Module);
#endif
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdRecomputeDimBlockModule);
//Dgn Import
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdDgnModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdDgn7IOModuleImpl);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(DgModelerModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdDwgModule);

ODRX_BEGIN_STATIC_MODULE_MAP()
  ODRX_DEFINE_STATIC_APPLICATION(OdModelerGeometryModuleName,     ModelerModule)
  ODRX_DEFINE_STATIC_APPLICATION(RX_RASTER_SERVICES_APPNAME,      ExRasterModule)
  ODRX_DEFINE_STATIC_APPLICATION(OdRasterProcessorModuleName,     OdRasterProcessingServicesImpl)
#ifndef ANDROID_GLES2
ODRX_DEFINE_STATIC_APPLICATION(OdWinOpenGLModuleName,           WinOpenGLModule)
#else
ODRX_DEFINE_STATIC_APPLICATION(OdTrGL2ModuleName,               OdTrGL2RenderModule)
ODRX_DEFINE_STATIC_APPLICATION(OdWinGLES2ModuleName,            GLES2Module)
#endif
  ODRX_DEFINE_STATIC_APPLICATION(OdRecomputeDimBlockModuleName,   OdRecomputeDimBlockModule)
  //Dgn
  ODRX_DEFINE_STATIC_APPLICATION(L"TG_Db", OdDgnModule)
  ODRX_DEFINE_STATIC_APPLICATION(L"TG_Dgn7IO", OdDgn7IOModuleImpl)
  ODRX_DEFINE_STATIC_APPMODULE(L"TG_ModelerGeometry",   DgModelerModule)
  ODRX_DEFINE_STATIC_APPMODULE(OdDbModuleName,          OdDwgModule)
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

class TeighaDGNContext
{
  public:
    OdGsDevicePtr gsDevice;
    OdDgDatabasePtr pDb;
};

static TeighaDGNContext * context = NULL;

extern "C" {

JNIEXPORT jboolean JNICALL Java_com_opendesign_odaandroiddgnapp_TeighaDGNJni_init
  (JNIEnv * /*env*/, jclass /*clazz*/)
{
  LOGI("Java_com_opendesign_odaandroiddgnapp_TeighaDGNJni_open\n");
  ODRX_INIT_STATIC_MODULE_MAP();
  odSetAssertFunc(_MyAssert);
  odInitialize(AndroidAppServices::getInstance());
  LOGI("Java_com_opendesign_odaandroiddgnapp_TeighaDGNJni_open: load TG_Db\n");
  ::odrxDynamicLinker()->loadModule(L"TG_Db", false);
  return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL Java_com_opendesign_odaandroiddgnapp_TeighaDGNJni_finit
  (JNIEnv *, jclass)
{
  odUninitialize();
  return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL Java_com_opendesign_odaandroiddgnapp_TeighaDGNJni_open
  (JNIEnv * env, jclass clazz, jstring filename)
{
  LOGI("Java_com_opendesign_odaandroiddgnapp_TeighaDGNJni_open\n");
  if ( context != NULL )
  {
    return JNI_FALSE;
  }
  try
  {
    context = new TeighaDGNContext();
    jboolean b;
    const char *cfn = env->GetStringUTFChars( filename, &b );
    LOGI("Reading file %s\n", cfn);
    OdAnsiString fileStr(cfn);
    {
      context->pDb = AndroidAppServices::getInstance()->readFile(cfn);
    }
    LOGI("Read complete\n");
    env->ReleaseStringUTFChars( filename, cfn );
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

JNIEXPORT jboolean JNICALL Java_com_opendesign_odaandroiddgnapp_TeighaDGNJni_createRenderer
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
      //extended check for bad files
    OdDgModelPtr pModel;
    {
      OdDgElementId activeModelId = context->pDb->getActiveModelId();

      if( activeModelId.isNull() )
      {
        // MKU 24\11\2010 #8852 (case: /dgn/V7_V8legacy/Regression/out/ExLoadToModel_dll/Building/Dgn/BSI300AE9-Atrium.dgn
        //  If Model is is invalid let's try to get & use a Default.
        activeModelId = context->pDb->getDefaultModelId();
        context->pDb->setActiveModelId (activeModelId);
      }

      if( activeModelId.isNull() )
      {
        LOGE("Incorrect Active Model Id\n");
        return JNI_FALSE;
      }

      pModel = OdDgModel::cast( activeModelId.openObject( OdDg::kForRead ) );
      if( pModel.isNull() )
      {
        LOGE("Can not open Active Model\n");
        return JNI_FALSE;
      }
    }
    if (!context->gsDevice.isNull())
    {
      context->gsDevice.release();
    }

#ifndef ANDROID_GLES2
    OdGsModulePtr m = ::odrxDynamicLinker()->loadModule(OdWinOpenGLModuleName);
#else
    OdGsModulePtr m = ::odrxDynamicLinker()->loadModule(OdWinGLES2ModuleName);
#endif
    if ( m.isNull() )
    {
      LOGE("Could not load GenericOpenGLModule\n");
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

    OdGiContextForDgDatabasePtr pDgnContext = OdGiContextForDgDatabase::createObject();

    pDgnContext->enableGsModel(true);
    
    pDgnContext->setDatabase(context->pDb);
    OdDgElementId vectorizedViewId;
    OdDgViewGroupPtr pViewGroup = context->pDb->getActiveViewGroupId().openObject();
    if( pViewGroup.isNull() )
    {
      //  Some files can have invalid id for View Group. Try to get & use a valid (recommended) View Group object.
      pViewGroup = context->pDb->recommendActiveViewGroupId().openObject();
      if( pViewGroup.isNull() )
      {
        // Add View group
        pModel->createViewGroup();
        pModel->fitToView();
        pViewGroup = context->pDb->recommendActiveViewGroupId().openObject();
      }
    }
    if( !pViewGroup.isNull() )
    {
      OdDgElementIteratorPtr pIt = pViewGroup->createIterator();
      for( ; !pIt->done(); pIt->step() )
      {
        OdDgViewPtr pView = OdDgView::cast( pIt->item().openObject() );
        if( pView.get() && pView->getVisibleFlag() )
        {
          vectorizedViewId = pIt->item();
          break;
        }
      }
    }

    if( vectorizedViewId.isNull() )
    {
        LOGE("Can not find an active view group or all its views are disabled\n");
        return JNI_FALSE;
    }
    pDgnContext->setView( vectorizedViewId.openObject( OdDg::kForRead ) );

    context->gsDevice = OdGsDeviceForDgModel::setupModelView(context->pDb->getActiveModelId(), vectorizedViewId, context->gsDevice, pDgnContext);

    /*
    // Set the database to be vectorized.
    pDgnContext->setDatabase(context->pDb);
    pDgnContext->setPlotGeneration(true);
    // pDgnContext->enableGsModel(true);

    // Prepare the device to render the active layout in this database.
    OdDbBaseDatabasePEPtr pDbPE = context->pDb ;
    context->gsDevice = pDbPE->setupActiveLayoutViews( context->gsDevice, pDgnContext);
    */
    //context->gsDevice = OdDbGsManager::setupActiveLayoutViews(context->gsDevice, pDgnContext);
    // Setup device palette
    if (OdGsPaperLayoutHelper::cast(context->gsDevice).isNull())
    {
      // ModelSpace
      context->gsDevice->setLogicalPalette(::odcmAcadDarkPalette(), 256);
      context->gsDevice->setBackgroundColor(ODRGB(0, 0, 0));
      pDgnContext->setPaletteBackground(ODRGB(0, 0, 0));
    }
    else
    {
      context->gsDevice->setLogicalPalette(::odcmAcadLightPalette(), 256);
      context->gsDevice->setBackgroundColor(ODRGB(173, 174, 173));
      pDgnContext->setPaletteBackground(ODRGB(255, 255, 255));
    }
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

JNIEXPORT jboolean JNICALL Java_com_opendesign_odaandroiddgnapp_TeighaDGNJni_renderFrame
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

JNIEXPORT jboolean JNICALL Java_com_opendesign_odaandroiddgnapp_TeighaDGNJni_destroyRenderer
  (JNIEnv *, jclass)
{
  LOGI("Java_com_opendesign_android_TeighaDWGJni_destroyRenderer\n");
  if (context != NULL)
    context->gsDevice.release();
  return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL Java_com_opendesign_odaandroiddgnapp_TeighaDGNJni_close
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

} // extern "C"

