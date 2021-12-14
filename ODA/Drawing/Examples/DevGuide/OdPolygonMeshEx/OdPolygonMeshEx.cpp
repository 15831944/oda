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

/************************************************************************/
/* This console application demonstates the OdDbPolygonMesh object      */
/* functionality file and creates a .dwg file filled with               */
/*  Polygon mesh entities.                                              */
/*                                                                      */
/* Calling sequence:  OdPolygonMeshEx <filename>                        */
/*                                                                      */
/************************************************************************/

#include "OdaCommon.h"
#include "diagnostics.h"
#include "DbDatabase.h"
#include "ExSystemServices.h"
#include "ExHostAppServices.h"
#include "DbBlockTableRecord.h"
#include "DbPolygonMesh.h"


#define STL_USING_IOSTREAM
#include "OdaSTL.h"
#define  STD(a)  std:: a

#ifdef OD_HAVE_CONSOLE_H_FILE
#include <console.h>
#endif


#define ODPOLYGONMESHEX_STR L"OdPolygonMeshEx"


void appendPgMeshVertex(OdDbPolygonMesh* pPgMesh, OdGePoint3d pos)
{
  /**********************************************************************/
  /* Append a Vertex to the PolyFaceMesh                                */
  /**********************************************************************/
  OdDbPolygonMeshVertexPtr pVertex = OdDbPolygonMeshVertex::createObject();
  pPgMesh->appendVertex(pVertex);
  
  /**********************************************************************/
  /* Set the properties                                                 */
  /**********************************************************************/
  pVertex->setPosition(pos);
}


void fillPolygonMesh(OdDbPolygonMesh* pPgMesh, OdGePoint3d center)
{
  /**********************************************************************/
  /* Define the size of the mesh                                        */
  /**********************************************************************/
  OdInt16 mSize = 4, nSize = 4;
  pPgMesh->setMSize(mSize);
  pPgMesh->setNSize(nSize);


  /**********************************************************************/
  /* Define a profile                                                   */
  /**********************************************************************/

  double dx = 15.;
  double dy = 10.;

  OdGeVector3dArray vectors;
  vectors.setLogicalLength(nSize);

  vectors[0] = OdGeVector3d (0.,  -dy, 0.);
  vectors[1] = OdGeVector3d (dx, -dy, 0.);
  vectors[2] = OdGeVector3d (dx,  dy, 0.);
  vectors[3] = OdGeVector3d (0.,  dy, 0.);

  /**********************************************************************/
  /* Append the vertices to the mesh                                    */
  /**********************************************************************/
  for (int i = 0; i < mSize; i++)
  {
    for (int j = 0; j < nSize; j++)
    {
      appendPgMeshVertex(pPgMesh, center + vectors[j]);
      vectors[j].rotateBy(Oda2PI / mSize, OdGeVector3d::kYAxis);
    }
  }
}


void showStat(OdDbPolygonMesh* pPgMesh)
{
  OdString about;
  OdResult eRes; 
  OdGePlane plane3dPL;
  double a, b, c, d;
  OdDb::Planarity planarity;
  OdInt16 count = 0;

  odPrintConsoleString(L"\n/////////////START DISPLAYING POLYGON MESH DATA/////////////");
  // Check the polygon mesh type
  switch( pPgMesh->polyMeshType() )
  {
    case OdDb::kSimpleMesh: about = L"Simple"; break;
    case OdDb::kQuadSurfaceMesh: about = L"Quadratic B-spline"; break;
    case OdDb::kCubicSurfaceMesh: about = L"Cubic B-spline"; break;
    case OdDb::kBezierSurfaceMesh: about = L"Bezier spline"; break;
  }
  odPrintConsoleString(L"\nPolygon mesh type is %s", about.c_str());

  // Check closed status in M direction
  odPrintConsoleString(L"\nPolygon mesh is %s in M direction", ((pPgMesh->isMClosed()) ? L"closed" : L"opened"));

  // Check closed status in N direction
  odPrintConsoleString(L"\nPolygon mesh is %s in N direction", ((pPgMesh->isNClosed()) ? L"closed" : L"opened"));

  // Get plane of the polygon mesh
  eRes = pPgMesh->getPlane(plane3dPL, planarity);
  if(eRes == eOk && planarity != OdDb::kNonPlanar)
  {
    plane3dPL.getCoefficients(a, b, c, d);
    odPrintConsoleString(L"Polygon mesh plane is (%g * X + %g * Y + %g * Z + %g)", a, b, c, d);
  }

  // Get size of the polygon mesh
  // Get number of rows
  odPrintConsoleString(L"\nNumber of rows = %i", pPgMesh->mSize()); 
  // Get number of columns
  odPrintConsoleString(L"\nNumber of columns = %i", pPgMesh->nSize());

  // Get density
  // Get density of rows
  odPrintConsoleString(L"\nDensity of rows = %i", pPgMesh->mSurfaceDensity()); 

  // Get density of columns
  odPrintConsoleString(L"\nDensity of columns = %i", pPgMesh->nSurfaceDensity());

  // Get number of vertices
  OdDbObjectIteratorPtr itVertex = pPgMesh->vertexIterator();
  for(itVertex->start() ; !itVertex->done() ; itVertex->step()) count++;
  odPrintConsoleString(L"\nPolygon mesh has %d vertices", count);
  odPrintConsoleString(L"\n//////////////END DISPLAYING POLYGON MESH DATA//////////////");
  odPrintConsoleString(L"\n");
}


/************************************************************************/
/* Define a Custom Services class.                                      */
/*                                                                      */
/* Combines the platform dependent functionality of                     */
/* ExSystemServices and ExHostAppServices                               */ 
/************************************************************************/
class MyServices : public ExSystemServices, public ExHostAppServices
{
protected:
  ODRX_USING_HEAP_OPERATORS(ExSystemServices);
};


/********************************************************************************/
/* Define Assert function to not crash Debug application if assertion is fired. */
/********************************************************************************/
static void MyAssert(const char* expression, const char* fileName, int nLineNo)
{
  OdString message;
  message.format(L"\n!!! Assertion failed: \"%ls\"\n    file: %ls, line %d\n", OdString(expression).c_str(), OdString(fileName).c_str(), nLineNo);
  odPrintConsoleString(message);
}

/************************************************************************/
/* Main                                                                 */
/************************************************************************/
#if defined(OD_USE_WMAIN)
int wmain(int argc, wchar_t* argv[])
#else
int main(int argc, char* argv[])
#endif
{
#ifdef OD_HAVE_CCOMMAND_FUNC
  argc = ccommand(&argv);
#endif
  
  /**********************************************************************/
  /* Verify the argument count and display an error message as required */
  /**********************************************************************/
  if (argc < 2) 
  {
    odPrintConsoleString(ODPOLYGONMESHEX_STR L" sample program. Copyright (c) 2019, Open Design Alliance\n");
    ODA_TRACE(L"Something gona happen.\n");
    odPrintConsoleString(L"\tusage: " ODPOLYGONMESHEX_STR L" filename\n");
    odPrintConsoleString(L"\nPress ENTER to continue...\n");
    STD(cin).get();
    return 1;
  }
  /********************************************************************/
  /* Create a custom Services instance.                               */
  /********************************************************************/
  OdStaticRxObject<MyServices> svcs;
  svcs.disableOutput(true);   // Disable progress meter output
  svcs.setRecomputeDimBlocksRequired(false);  // We need not Dimension blocks recomputing
  odSetAssertFunc(MyAssert);

  /********************************************************************/
  /* Initialize the Drawings SDK                                      */
  /********************************************************************/
  odInitialize(&svcs);




  /********************************************************************/
  /* Create Database.                                                 */
  /********************************************************************/
  OdDbDatabasePtr pDb = svcs.createDatabase(true);

  /**********************************************************************/
  /* Create the pointer to a model space                                */
  /**********************************************************************/
  OdDbBlockTableRecordPtr bBTR = pDb->getModelSpaceId().safeOpenObject(OdDb::kForWrite);


  /**********************************************************************/
  /* Create polygon meshes                                              */
  /**********************************************************************/
  /**********************************************************************/
  /* Create simple polygon mesh                                         */
  /**********************************************************************/
  // Initialize polygon mesh
  OdDbPolygonMeshPtr pPolyMesh = OdDbPolygonMesh::createObject();
  pPolyMesh->setDatabaseDefaults(pDb);
  bBTR->appendOdDbEntity(pPolyMesh);
  // Define center
  OdGePoint3d center = OdGePoint3d (-40.,-20., 20.);
  // Fill polygon mesh
  fillPolygonMesh(pPolyMesh,center);
  // Make polygon mesh closed in M-direction
  pPolyMesh->makeMClosed();
  // Show statistic
  showStat(pPolyMesh);

  /**********************************************************************/
  /* Create cubic polygon mesh                                          */
  /**********************************************************************/
  // Initialize polygon mesh
  OdDbPolygonMeshPtr pPolyMesh2 = OdDbPolygonMesh::createObject();
  pPolyMesh2->setDatabaseDefaults(pDb);
  bBTR->appendOdDbEntity(pPolyMesh2);
  // Define center
  center = OdGePoint3d (0.,-20., 20.);
  // Fill polygon mesh
  fillPolygonMesh(pPolyMesh2,center);
  // Make polygon mesh closed in M-direction
  pPolyMesh2->makeMClosed();
  // Transform it to the cubic polygon mesh
  pPolyMesh2->surfaceFit(OdDb::kCubicSurfaceMesh, 8, 8);
  // Show statistic
  showStat(pPolyMesh2);

  /**********************************************************************/
  /* Create quadric polygon mesh                                        */
  /**********************************************************************/
  // Initialize polygon mesh
  OdDbPolygonMeshPtr pPolyMesh3 = OdDbPolygonMesh::createObject();
  pPolyMesh3->setDatabaseDefaults(pDb);
  bBTR->appendOdDbEntity(pPolyMesh3);
  // Define center
  center = OdGePoint3d (40.,-20., 20.);
  // Fill polygon mesh
  fillPolygonMesh(pPolyMesh3,center);
  // Make polygon mesh closed in M-direction
  pPolyMesh3->makeMClosed();
  // Transform it to the quadric polygon mesh
  pPolyMesh3->surfaceFit(OdDb::kQuadSurfaceMesh, 8, 8);
  // Show statistic
  showStat(pPolyMesh3);

  /**********************************************************************/
  /* Create Bezier polygon mesh                                         */
  /**********************************************************************/
  // Initialize polygon mesh
  OdDbPolygonMeshPtr pPolyMesh4 = OdDbPolygonMesh::createObject();
  pPolyMesh4->setDatabaseDefaults(pDb);
  bBTR->appendOdDbEntity(pPolyMesh4);
  // Define center
  center = OdGePoint3d (80.,-20., 20.);
  // Fill polygon mesh
  fillPolygonMesh(pPolyMesh4,center);
  // Make polygon mesh closed in M-direction
  pPolyMesh4->makeMClosed();
  // Transform it to the Bezier polygon mesh
  pPolyMesh4->surfaceFit(OdDb::kBezierSurfaceMesh, 8, 8);
  // Show statistic
  showStat(pPolyMesh4);


  OdDb::SaveType fileType = OdDb::kDwg;
  OdDb::DwgVersion outVer = OdDb::kDHL_CURRENT;

  bool bOk = true;
  try
  {
    pDb->writeFile(argv[1], fileType, outVer, true /* save preview */);
  }
  catch (const OdError& err)
  {
    odPrintConsoleString(L"\nwriteFile() failed: %ls", err.description().c_str());
    bOk = false;
  }

  if (bOk)
  {
    odPrintConsoleString(L"\nDatabase has been saved as %ls", pDb->getFilename().c_str());
  }
  odPrintConsoleString(L"\n");
  system("pause");
    
  
  pDb.release();
  /**********************************************************************/
  /* Uninitialize the Drawings SDK                                      */
  /**********************************************************************/
  odUninitialize();

  return bOk ? 0 : 1;
}

