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

// Buttons :
//    'q', 'Q' - Quit (must use this rather than closing the window, to ensure proper termination)
//    'Y', 'U' - Orbit around X axis
//    'H', 'J' - Orbit around Y axis
//    'N', 'M' - Orbit around Z axis

//    'I' - zoom in
//    'O' - zoom out

//    'S' - move camara left
//    'F' - move camera right
//    'E' - move camera up
//    'C' - move camera down

//    'D' - zoom to extents

#include <stdlib.h> 
#include <stdio.h>

#include <string.h> 
#include <fcntl.h>


#ifdef OD_HAVE_CONSOLE_H_FILE
#include <console.h>
#endif

#include "OdaCommon.h"
#include "DbDatabase.h"
#include "DbBlockTableRecord.h"
#include "DbViewportTable.h"
#include "DbViewportTableRecord.h"
#include "Db3dSolid.h"
#include "DbGsManager.h"
#include "DbBody.h"
#include "DbRegion.h"
#include "Db3dSolid.h"

#include "RxObjectImpl.h"

#include "ExSystemServices.h"
#include "ExHostAppServices.h"
#include "GiContextForDbDatabase.h"

#include "ExGsOpenGLGenericDevice.h"
#include "ExGsGenericScreenDevice.h"

#include "OdFileBuf.h"

#include "RxDynamicModule.h"

#ifndef _TOOLKIT_IN_DLL_

ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(ModelerModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(GenericOpenGLModule);

ODRX_BEGIN_STATIC_MODULE_MAP()
  ODRX_DEFINE_STATIC_APPLICATION(OdModelerGeometryModuleName,     ModelerModule)
  ODRX_DEFINE_STATIC_APPLICATION(OD_T("GenericOpenGLModule"), GenericOpenGLModule)
ODRX_END_STATIC_MODULE_MAP()

#endif

#define GLUT_DISABLE_ATEXIT_HACK // Disable warning in GLUT 3.7 for Windows

#define TD_USING_GLUT
#include "TD_GL.h"

class GenericOpenGLModule : public OdGsBaseModule
{
protected:
  OdSmartPtr<OdGsBaseVectorizeDevice> createDeviceObject()
  {
    return ExGsGenericScreenDevice<ExGsOpenGLGenericDevice>::createObject();
  }

  OdSmartPtr<OdGsViewImpl> createViewObject()
  {
    return OdRxObjectImpl<ExGsOpenGLGenericView, OdGsViewImpl>::createObject();
  }

  OdSmartPtr<OdGsBaseVectorizeDevice> createBitmapDeviceObject()
  {
    return OdSmartPtr<OdGsBaseVectorizeDevice>();
  }

  OdSmartPtr<OdGsViewImpl> createBitmapViewObject()
  {
    return OdRxObjectImpl<ExGsOpenGLGenericView, OdGsViewImpl>::createObject();
  }
};

ODRX_DEFINE_STATIC_MODULE(GenericOpenGLModule);

OdGsDevicePtr g_pDevice ;
OdDbDatabasePtr g_pDb;

class MyServices : public ExSystemServices, public ExHostAppServices
{
protected:
  ODRX_USING_HEAP_OPERATORS(ExSystemServices);
};

#include "diagnostics.h"

static void MyAssert(const char* expresssion, const char* fileName, int nLineNo)
{
  printf("\nAssertion Failed: %ls\n"
         "file: %ls\n"
         "line: %d\n",
         expresssion, fileName, nLineNo);      
}

static GLfloat DrawMatrix[16] = {
  1, 0, 0, 0,
  0, 1, 0, 0,
  0, 0, 1, 0,
  0, 0, 0, 1
};

void MyDisplay(void)
{
  glMatrixMode(GL_MODELVIEW);
  glLoadMatrixf(DrawMatrix);

  g_pDevice->update();

  glFinish();
}

void MyReshape(int width, int height)
{
  OdGsDCRect screenRect(OdGsDCPoint(0, height), OdGsDCPoint(width, 0));
  g_pDevice->onSize(screenRect);
}

void MyKey(unsigned char key, int x, int y)
{
  OdGsView* pView = g_pDevice->viewAt(0);
  key = toupper(key);
  switch (key)
  {
    case 'Q':
      //odUninitialize();
      throw 0;
      //exit(0);
      break;
   
    case 'Y':
      pView->orbit(0.3927, 0.0);
      glutPostRedisplay();
      break;

    case 'U':
      pView->orbit(-0.3927, 0.0);
      glutPostRedisplay();
      break;

    case 'H':
      pView->orbit(0.0, 0.3927);
      glutPostRedisplay();
      break;

    case 'J':
      pView->orbit(0.0, -0.3927);
      glutPostRedisplay();
      break;

    case 'N':
      pView->roll(0.3927);
      glutPostRedisplay();
      break;

    case 'M':
      pView->roll(-0.3927);
      glutPostRedisplay();
      break;

    case 'O':
      pView->zoom(0.75);
      glutPostRedisplay();
      break;

    case 'I':
      pView->zoom(1.5);
      glutPostRedisplay();
      break;

    case 'S':
      {
        OdGeVector3d vec(-20.0, 0.0, 0.0);
        vec.transformBy((pView->screenMatrix() * pView->projectionMatrix()).inverse());
        pView->dolly(vec);
        glutPostRedisplay();
      }
      break;

    case 'F':
      {
        OdGeVector3d vec(20.0, 0.0, 0.0);
        vec.transformBy((pView->screenMatrix() * pView->projectionMatrix()).inverse());
        pView->dolly(vec);
        glutPostRedisplay();
      }
      break;

    case 'E':
      {
        OdGeVector3d vec(0.0, 20.0, 0.0);
        vec.transformBy((pView->screenMatrix() * pView->projectionMatrix()).inverse());
        pView->dolly(vec);
        glutPostRedisplay();
      }
      break;
      
    case 'C':
      {
        OdGeVector3d vec(0.0, -20.0, 0.0);
        vec.transformBy((pView->screenMatrix() * pView->projectionMatrix()).inverse());
        pView->dolly(vec);
        glutPostRedisplay();
      }
      break;

    case 'D':
      {
        if(OdAbstractViewPEPtr(pView)->zoomExtents(pView))
        {
          glutPostRedisplay();
        }
      }
      break;
  }
}

int main(int argc, char* argv[])
{
#ifdef OD_HAVE_CCOMMAND_FUNC
  argc = ccommand(&argv);
#endif

  if (argc < 2) 
  {
    printf("usage: OdOpenGL <file> [-rm:N]\n");
    printf("  -rm sets render mode to N, one of:\n");
    printf("    0 - OdGsView::k2DOptimized\n");
    printf("    1 - OdGsView::kWireframe\n");
    printf("    2 - OdGsView::kHiddenLine\n");
    printf("    3 - OdGsView::kFlatShaded\n");
    printf("    4 - OdGsView::kGouraudShaded\n");
    printf("    5 - OdGsView::kFlatShadedWithWireframe\n");
    printf("    6 - OdGsView::kGouraudShadedWithWireframe\n");
  }
  else
  {
#ifndef _TOOLKIT_IN_DLL_
    ODRX_INIT_STATIC_MODULE_MAP();
#endif

    // Create a custom Services object.
    OdStaticRxObject<MyServices> svcs;
    odInitialize(&svcs);
    odgsInitialize();
    odSetAssertFunc(MyAssert);

    printf("Developed using %ls, %ls\n", svcs.product().c_str(), svcs.versionString().c_str());

    try 
    {
      // Create a database and load the drawing into it.      
      OdString file = argv[1];
      OdString ext = file.right(3);
      ext.makeUpper();
      if (ext == "SAT")
      {
        OdDbEntityPtrArray ptr;
        if (OdDbBody::acisIn(file, ptr) == eOk)
        {
          g_pDb = svcs.createDatabase();
          OdDbBlockTableRecordPtr pBlock = g_pDb->getModelSpaceId().safeOpenObject(OdDb::kForWrite);

          for ( unsigned int i = 0; i < ptr.size(); ++i )
          {
            if (!OdDb3dSolidPtr(ptr[i]).isNull())
              OdDb3dSolidPtr(ptr[i])->setColorIndex(7);
            else if (!OdDbBodyPtr(ptr[i]).isNull())
              OdDbBodyPtr(ptr[i])->setColorIndex(7);
            else if (!OdDbRegionPtr(ptr[i]).isNull())
              OdDbRegionPtr(ptr[i])->setColorIndex(7);

            pBlock->appendOdDbEntity( ptr[i] );
          }

          OdDbViewportTablePtr pVPTbl = g_pDb->getViewportTableId().safeOpenObject();
          OdDbObjectId id = pVPTbl->getActiveViewportId();
          if (id)
          {
            OdDbViewportTableRecordPtr pRec = id.safeOpenObject(OdDb::kForWrite);
            pRec->zoomExtents();
          }
        }
      }
      else
      {
        g_pDb = svcs.readFile(argv[1], false, false, Oda::kShareDenyNo); 
      }

      if (!g_pDb.isNull())
      {
        // Create the rendering context.
        OdGiContextForDbDatabasePtr pDwgContext = OdGiContextForDbDatabase::createObject();

        // Create the custom rendering device and set the output stream for the device.
#ifdef _TOOLKIT_IN_DLL_
        // This initialization works fine with both dll and static configurations.
        OdGsModulePtr m = ODRX_STATIC_MODULE_ENTRY_POINT(GenericOpenGLModule)(OD_T("GenericOpenGLModule"));
#else
        OdGsModulePtr m = ::odrxDynamicLinker()->loadModule(OD_T("GenericOpenGLModule"));
#endif
        g_pDevice = m->createDevice();

        // Set the database to be vectorized.
        pDwgContext->setDatabase(g_pDb);

        // Prepare the device to render the active layout in this database.
        g_pDevice = OdDbGsManager::setupActiveLayoutViews(g_pDevice, pDwgContext);

        if (argc > 2) 
        {
          if (odStrnICmpA(argv[2], "-rm:", 4) == 0)
          {
            int choice = argv[2][4] - '0';
            if (choice >= 0 && choice <= 6)
            {
              OdGsView::RenderMode rm = 
                OdGsView::RenderMode(OdGsView::k2DOptimized + choice);
              OdGsViewPtr pView = g_pDevice->viewAt(0);
              pView->setMode(rm);
            }
          }
        }

        // Set the screen size for the generated geometry.
        OdGsDCRect screenRect(OdGsDCPoint(0, 600), OdGsDCPoint(800, 0));
        g_pDevice->onSize(screenRect);

        // Initialize GLUT state
        glutInit(&argc, argv);

        // Set window size and location
        glutInitWindowSize(800, 600);
        glutInitWindowPosition(0, 0);

        // Select type of Display mode: Single buffer & RGBA color
        glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);

        // Open a window
        glutCreateWindow("OpenGL Viewer");

        // Register Callback Functions
        glutReshapeFunc(MyReshape);
        glutDisplayFunc(MyDisplay);
        glutKeyboardFunc(MyKey);

        MyReshape(800, 600);

        // Start Event Processing Engine
        glutMainLoop();
      }
      printf("\nFinished OK.\n");
    }
    catch (int)
    {
      printf("\nNormal termination");
    }
    catch (const OdError& e)
    {
      printf("\nException Caught: %ls\n", e.description().c_str());      
    }
    catch (...)
    {
      printf("\nUnknown Exception Caught\n");
    }

    g_pDevice.release();
    g_pDb.release();

    odgsUninitialize();
    odUninitialize();
  }

  return 0;
}

