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

#include "StdAfx.h"
#include "DbPolygonMesh.h"


/************************************************************************/
/* POLYGONMESH command demonstates the OdDbPolygonMesh entity           */
/* functionality  and pupulates database with Polygon Mesh entities.    */
/************************************************************************/
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


void showStat(OdDbPolygonMesh* pPgMesh, OdDbUserIO* pIO)
{
  OdString about;
  OdResult eRes; 
  OdGePlane plane3dPL;
  double a, b, c, d;
  OdDb::Planarity planarity;
  OdInt16 count = 0;

  pIO->putString(L"\n/////////////START DISPLAYING POLYGON MESH DATA/////////////");
  // Check the polygon mesh type
  switch( pPgMesh->polyMeshType() )
  {
    case OdDb::kSimpleMesh: about = L"Simple"; break;
    case OdDb::kQuadSurfaceMesh: about = L"Quadratic B-spline"; break;
    case OdDb::kCubicSurfaceMesh: about = L"Cubic B-spline"; break;
    case OdDb::kBezierSurfaceMesh: about = L"Bezier spline"; break;
  }
  OdString message;
  message.format(L"Polygon mesh type is %s", about.c_str());
  pIO->putString(message);

  // Check closed status in M direction
  message.format(L"Polygon mesh is %s in M direction", ((pPgMesh->isMClosed()) ? L"closed" : L"opened"));
  pIO->putString(message);

  // Check closed status in N direction
  message.format(L"Polygon mesh is %s in N direction", ((pPgMesh->isNClosed()) ? L"closed" : L"opened"));
  pIO->putString(message);

  // Get plane of the polygon mesh
  eRes = pPgMesh->getPlane(plane3dPL, planarity);
  if(eRes == eOk && planarity != OdDb::kNonPlanar)
  {
    plane3dPL.getCoefficients(a, b, c, d);
    message.format(L"Polygon mesh plane is (%g * X + %g * Y + %g * Z + %g)", a, b, c, d);
    pIO->putString(message);
  }

  // Get size of the polygon mesh
  // Get number of rows
  message.format(L"Number of rows = %i", pPgMesh->mSize());
  pIO->putString(message); 
  // Get number of columns
  message.format(L"Number of columns = %i", pPgMesh->nSize());
  pIO->putString(message);

  // Get density
  // Get density of rows
  message.format(L"Density of rows = %i", pPgMesh->mSurfaceDensity());
  pIO->putString(message); 

  // Get density of columns
  message.format(L"Density of columns = %i", pPgMesh->nSurfaceDensity());
  pIO->putString(message);

  // Get number of vertices
  OdDbObjectIteratorPtr itVertex = pPgMesh->vertexIterator();
  for(itVertex->start() ; !itVertex->done() ; itVertex->step()) count++;
  message.format(L"Polygon mesh has %d vertices", count);
  pIO->putString(message);
  pIO->putString(L"//////////////END DISPLAYING POLYGON MESH DATA//////////////\n");
}


void _PolygonMesh_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();

  /**********************************************************************/
  /* Create the pointer to current active space                         */
  /**********************************************************************/
  OdDbBlockTableRecordPtr bBTR = DevGuideCommandsUtils::activeBlockTableRecord(pDb).safeOpenObject(OdDb::kForWrite);


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
  showStat(pPolyMesh, pIO);

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
  showStat(pPolyMesh2, pIO);

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
  showStat(pPolyMesh3, pIO);

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
  showStat(pPolyMesh4, pIO);
}
