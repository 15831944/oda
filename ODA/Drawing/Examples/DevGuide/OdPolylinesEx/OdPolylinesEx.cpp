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
/* This console application demonstates the OdDb2dPolyline and          */
/* OdDb3dPolyline objectfunctionality file and creates a .dwg file      */
/* filled with Polyline entities.                                       */
/*                                                                      */
/* Calling sequence:  OdPolylinesEx <filename>                          */
/*                                                                      */
/************************************************************************/

#include "OdaCommon.h"
#include "diagnostics.h"
#include "DbDatabase.h"
#include "ExSystemServices.h"
#include "ExHostAppServices.h"


#include "DbText.h"
#include "DbTextStyleTable.h"
#include "DbTextStyleTableRecord.h"
#include "DbArc.h"
#include "DbBlockTable.h"
#include "DbBlockTableRecord.h"
#include "OdFileBuf.h"
#include "DbArcAlignedText.h"
#include "DbTable.h"
#include "DbTableContent.h"
#include "DbTableStyle.h"
#include "DbBlockTable.h"
#include "DbAttributeDefinition.h"


#include "Ge/GeLinearEnt2d.h"
#include "Ge/GeCircArc2d.h"
#include "DbEllipse.h"
#include "DbCircle.h"

#include "HatchPatternManager.h"
#include "HatchPattern.h"

#include "Ge/GeCircArc2d.h"
#include "Ge/GeScale3d.h"
#include "Ge/GeExtents3d.h"
#include "Gi/GiMaterial.h"
#include "Gi/GiRasterWrappers.h"


#include "OdFileBuf.h"
#include <math.h>

#include "DbSymUtl.h"
#include "DbHostAppServices.h"
#include "HatchPatternManager.h"
#include "DbAnnotativeObjectPE.h"
#include "DbObjectContextCollection.h"
#include "DbObjectContextManager.h"
#include "DbObjectContextInterface.h"


#include "Db3dPolyline.h"
#include "Db3dPolylineVertex.h"

#include "Db2dPolyline.h"
#include "Db2dVertex.h"


#define STL_USING_IOSTREAM
#include "OdaSTL.h"
#define  STD(a)  std:: a

#ifdef OD_HAVE_CONSOLE_H_FILE
#include <console.h>
#endif


#define ODPOLYLINESEX_STR L"OdPolylinesEx"


OdDbObjectId addBlockDef(OdDbDatabase* pDb, const OdString& name)
{
  /**********************************************************************/
  /* Open the block table                                               */
  /**********************************************************************/
  OdDbBlockTablePtr       pBlocks  = pDb->getBlockTableId().safeOpenObject(OdDb::kForWrite);
  
  /**********************************************************************/
  /* Create a BlockTableRecord                                          */
  /**********************************************************************/
  OdDbBlockTableRecordPtr bBTR = OdDbBlockTableRecord::createObject();
  
  /**********************************************************************/
  /* Block must have a name before adding it to the table.              */
  /**********************************************************************/
  bBTR->setName(name);
  
  /**********************************************************************/
  /* Add the record to the table.                                       */
  /**********************************************************************/
  OdDbObjectId btrId = pBlocks->add(bBTR);

  /**********************************************************************/
  /* Add an Attdef                                                      */
  /**********************************************************************/
  OdDbAttributeDefinitionPtr pAttDef = OdDbAttributeDefinition::createObject();
  pAttDef->setDatabaseDefaults(pDb);
  bBTR->appendOdDbEntity(pAttDef);

  pAttDef->setPrompt(OD_T("Enter ODT_ATT: "));
  pAttDef->setTag(OD_T("Oda_ATT"));
  pAttDef->setHorizontalMode(OdDb::kTextCenter);
  pAttDef->setHeight(0.1);
  pAttDef->setTextString(OD_T("Default"));

  /**********************************************************************/
  /* Return the ObjectId of the BlockTableRecord                        */
  /**********************************************************************/
  return btrId;
}


void fillPolyline2d(OdDb2dPolyline* p2dPl, double x, double y, double width1, double width2, OdDbBlockTableRecord* bBTR)
{
	double deltaX = 10;
	double deltaY = 10;
  for (int i=0; i<7; i++)
  {
    double buf;
    OdGePoint3d pos;
	pos.x = x;
	pos.y = y;
	OdDb2dVertexPtr pVertex2d = OdDb2dVertex::createObject();
    pVertex2d->setDatabaseDefaults(bBTR->database());
	pVertex2d->setPosition(pos);
	pVertex2d->setStartWidth(width1);
	pVertex2d->setEndWidth(width2);
	p2dPl->appendVertex(pVertex2d);
	x +=deltaX;
	y +=deltaY;
	deltaY *=(-1);
	buf = width1; width1 = width2; width2 = buf;
  }

}

void fillPolyline3d(OdDb3dPolyline* p3dPl, double x, double y, double z, OdDbBlockTableRecord* bBTR)
{
	double deltaX = 10;
	double deltaY = 10;
	double deltaZ = 10;
  for (int i=0; i<7; i++)
  {
    OdGePoint3d pos;
	pos.x = x;
	pos.y = y;
	pos.z = z;
	OdDb3dPolylineVertexPtr pVertex3d = OdDb3dPolylineVertex::createObject();
    pVertex3d->setDatabaseDefaults(bBTR->database());
	pVertex3d->setPosition(pos);
	p3dPl->appendVertex(pVertex3d);
	x +=deltaX;
	y +=deltaY;;
	z +=deltaZ;
	deltaY *=(-1);
	deltaZ *=(-1);
  }
}


void showStat2d(OdDb2dPolyline* p2dPL)
{
  OdString about;
  OdInt32 numV = 0;
  OdResult eRes; 
  OdGePlane plane2dPL;
  double a, b, c, d, vArea, vParam, vLength, vStWidth, vEndWidth, bulge;
  OdDb::Planarity planarity;
  OdDbObjectIteratorPtr pVertIt;
  OdGePoint3d point;

  odPrintConsoleString(L"\n //////////////// 2D POLYLINE DATA ////////////////");

  // Check 'open' status
  odPrintConsoleString(L"\n2D-Polyline is %s", ((p2dPL->isClosed()) ? L"closed" : L"opened"));

  // Check 'periodic' status
  odPrintConsoleString(L"\n2D-Polyline is %s", ((p2dPL->isPeriodic()) ? L"periodic" : L"unperiodic"));

  // Get Polyline type
  switch( p2dPL->polyType() )
  {
    case OdDb::k2dSimplePoly: about = L"simple 2D polyline"; break;
    case OdDb::k2dFitCurvePoly: about = L"2D arc-wavy curve"; break;
    case OdDb::k2dQuadSplinePoly: about = L"2D quadratic spline"; break;
    case OdDb::k2dCubicSplinePoly: about = L"2D cubic spline"; break;
  }
  odPrintConsoleString(L"\n2D-Polyline curve type is %s", about.c_str());

  // Get Plane
  eRes = p2dPL->getPlane(plane2dPL, planarity);
  if(eRes == eOk && planarity != OdDb::kNonPlanar)
  {
    plane2dPL.getCoefficients(a, b, c, d);
    odPrintConsoleString(L"\n2D-Polyline plane is (%g * X + %g * Y + %g * Z + %g)", a, b, c, d);
  }

  // Get Normal
  odPrintConsoleString(L"\nNormal = (%g,%g,%g)", p2dPL->normal().x, p2dPL->normal().y, p2dPL->normal().z); 


  // Get elevation
  odPrintConsoleString(L"\nElevation = %g", p2dPL->elevation()); 


  // Get thickness
  odPrintConsoleString(L"\nPolyline thickness = %g", p2dPL->thickness()); 


  eRes = p2dPL->getArea(vArea);
  if(eRes == eOk)
    odPrintConsoleString(L"\nPolyline area = %g", vArea);


  // Get number of vertices
  pVertIt = p2dPL->vertexIterator();
  for (pVertIt->start(); !pVertIt->done(); pVertIt->step())
  {
	  numV++;
  }
  odPrintConsoleString(L"\nNumber of vertices: %i", numV);

  // Get vertex data
  odPrintConsoleString(L"\nVetices: ");
  for (pVertIt->start(), vParam = 0.0, numV=0; !pVertIt->done(); pVertIt->step())
  {
    if((eRes = p2dPL->getPointAtParam(vParam , point)) == eOk)
	{
	  eRes = p2dPL->getDistAtParam(vParam, vLength);
      if(eRes == eOk)
	  {
        OdDb2dVertexPtr pVertex = (OdDb2dVertexPtr)pVertIt->entity();
        switch( pVertex->vertexType() )
        {
          case OdDb::k2dVertex: about = L"no-fit"; break;
          case OdDb::k2dSplineCtlVertex: about = L"control"; break;
          case OdDb::k2dSplineFitVertex: about = L"spline-fit"; break;
          case OdDb::k2dCurveFitVertex: about = L"curve-fit"; break;
        }
        odPrintConsoleString(L"\nVertex %i, (%g,%g,%g), type: %s, length: %g", numV, point.x, point.y, point.z, about.c_str(), vLength);
	  }
	}
    vParam +=1.0;
	numV++;
  }

  // Get segment data
  odPrintConsoleString(L"\nSegments: ");
  for (pVertIt->start(), numV=0; !pVertIt->done(); pVertIt->step())
  {
    vStWidth = ((OdDb2dVertexPtr)pVertIt->entity())->startWidth();
	vEndWidth = ((OdDb2dVertexPtr)pVertIt->entity())->endWidth();
	bulge = ((OdDb2dVertexPtr)pVertIt->entity())->bulge();
	odPrintConsoleString(L"\nSegment %i, start width = %g, end width = %g, bulge = %g", numV, vStWidth, vEndWidth, bulge);
	numV++;
  }

  odPrintConsoleString(L"\n //////////////// END OF DISPLAYING 2D POLYLINE DATA ////////////////");
  odPrintConsoleString(L"\n");
}



void showStat3d(OdDb3dPolyline* p3dPL)
{
  OdString about;
  OdInt32 numV = 0;
  OdResult eRes; 
  OdGePlane plane3dPL;
  double a, b, c, d, vArea, vParam, vLength;
  OdDb::Planarity planarity;
  OdDbObjectIteratorPtr pVertIt;
  OdGePoint3d point;

  odPrintConsoleString(L"\n //////////////// 3D POLYLINE DATA ////////////////");

  // Check 'open' status
  odPrintConsoleString(L"\n3D-Polyline is %s", ((p3dPL->isClosed()) ? L"closed" : L"opened"));

  // Check 'periodic' status
  odPrintConsoleString(L"\n3D-Polyline is %s", ((p3dPL->isPeriodic()) ? L"periodic" : L"unperiodic"));

  // Get Polyline type
  switch( p3dPL->polyType() )
  {
    case OdDb::k3dSimplePoly: about = L"simple 3D polyline"; break;
    case OdDb::k3dQuadSplinePoly: about = L"3D quadratic spline"; break;
    case OdDb::k3dCubicSplinePoly: about = L"3D cubic spline"; break;
  }
  odPrintConsoleString(L"\n3D-Polyline curve type is %s", about.c_str());

  // Get Plane
  eRes = p3dPL->getPlane(plane3dPL, planarity);
  if(eRes == eOk && planarity != OdDb::kNonPlanar)
  {
    plane3dPL.getCoefficients(a, b, c, d);
    odPrintConsoleString(L"\n3D-Polyline plane is (%g * X + %g * Y + %g * Z + %g)", a, b, c, d);
  }

  eRes = p3dPL->getArea(vArea);
  if(eRes == eOk)
    odPrintConsoleString(L"\nPolyline area = %g", vArea);

  // Get number of vertices
  pVertIt = p3dPL->vertexIterator();
  for (pVertIt->start(); !pVertIt->done(); pVertIt->step())
  {
	  numV++;
  }
  odPrintConsoleString(L"\nNumber of vertices: %i", numV);

  // Get vertex data
  odPrintConsoleString(L"\nVetices: ");
  for (pVertIt->start(), vParam = 0.0, numV=0; !pVertIt->done(); pVertIt->step())
  {
    if((eRes = p3dPL->getPointAtParam(vParam , point)) == eOk)
	{
	  eRes = p3dPL->getDistAtParam(vParam, vLength);
      if(eRes == eOk)
	  {
        OdDb3dPolylineVertexPtr pVertex = (OdDb3dPolylineVertexPtr)pVertIt->entity();
        switch( pVertex->vertexType() )
        {
          case OdDb::k3dSimpleVertex: about = L"no-fit"; break;
          case OdDb::k3dControlVertex: about = L"control"; break;
          case OdDb::k3dFitVertex: about = L"spline-fit"; break;
        }
        odPrintConsoleString(L"\nVertex %i, (%g,%g,%g), type: %s, length: %g", numV, point.x, point.y, point.z, about.c_str(), vLength);
	  }
	}
    vParam +=1.0;
	numV++;
  }

  odPrintConsoleString(L"\n //////////////// END OF DISPLAYING 3D POLYLINE DATA ////////////////");
  odPrintConsoleString(L"\n");
}

/************************************************************************/
/* Add a Text Style to the specified database                           */
/*                                                                      */
/* The symbol table and symbol table record are implicitly closed when  */
/* this function returns.                                               */
/************************************************************************/
OdDbObjectId addStyle(OdDbDatabase* pDb,
                      const OdString& styleName,
                      double textSize,
                      double xScale,
                      double priorSize,
                      double obliquing,
                      const OdString& fileName,
                      bool isShapeFile,
                      const OdString& ttFaceName,
                      bool bold,
                      bool italic,
                      int charset,
                      int pitchAndFamily)
{
  OdDbObjectId styleId;

  OdDbTextStyleTablePtr pStyles = pDb->getTextStyleTableId().safeOpenObject(OdDb::kForWrite);
  OdDbTextStyleTableRecordPtr pStyle = OdDbTextStyleTableRecord::createObject();

  // Name must be set before a table object is added to a table.  The
  // isShapeFile flag must also be set (if true) before adding the object
  // to the database.
  pStyle->setName(styleName);
  pStyle->setIsShapeFile(isShapeFile);

  // Add the object to the table.
  styleId = pStyles->add(pStyle);

  // Set the remaining properties.
  pStyle->setTextSize(textSize);
  pStyle->setXScale(xScale);
  pStyle->setPriorSize(priorSize);
  pStyle->setObliquingAngle(obliquing);
  pStyle->setFileName(fileName);
  if (isShapeFile)
  {
    pStyle->setPriorSize(22.45);
  }
  if (!ttFaceName.isEmpty())
  {
    pStyle->setFont(ttFaceName, bold, italic, charset, pitchAndFamily);
  }

  return styleId;
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
    odPrintConsoleString(ODPOLYLINESEX_STR L" sample program. Copyright (c) 2019, Open Design Alliance\n");
    ODA_TRACE(L"Something gona happen.\n");
    odPrintConsoleString(L"\tusage: " ODPOLYLINESEX_STR L" filename\n");
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
  /* Add a TTF text style                                               */
  /**********************************************************************/
  OdDbObjectId ttfStyleId = addStyle(pDb, OD_T("OdaTtfStyle"), 0.0, 1.0, 0.2, 0.0, 
      OD_T("VERDANA.TTF"), false, OD_T("Verdana"), false, false, 0, 34);

  /**********************************************************************/
  /* Define the fifth data row                                          */
  /**********************************************************************/
  OdDbObjectId odaBlock1Id = addBlockDef(pDb, OD_T("OdaBlock1")); 

  /**********************************************************************/
  /* Create the pointer to a model space                                */
  /**********************************************************************/
  OdDbBlockTableRecordPtr pModelSpace = pDb->getModelSpaceId().safeOpenObject(OdDb::kForWrite);
  


  /**********************************************************************/
  /* Create the Polylines                                               */
  /**********************************************************************/

  OdDbBlockTableRecordPtr bBTR = pDb->getModelSpaceId().safeOpenObject(OdDb::kForWrite);

  /**********************************************************************/
  /* Create 2D Polylines                                                */
  /**********************************************************************/
  // Create simple 2D polyline
  OdDb2dPolylinePtr pPline2dSimple = OdDb2dPolyline::createObject();
  pPline2dSimple->setDatabaseDefaults(bBTR->database());
  bBTR->appendOdDbEntity(pPline2dSimple);
  fillPolyline2d(pPline2dSimple, 0, 40, 0.2, 1.0, bBTR);
  // Add arc to the second segment
  OdDbObjectIteratorPtr pVertIt = pPline2dSimple->vertexIterator();
  pVertIt->start(); pVertIt->step();
  pPline2dSimple->openVertex(pVertIt->objectId(), OdDb::kForWrite)->setBulge(0.5);
  // Make Polyline closed
  pPline2dSimple->makeClosed();
  // Set thickness
  pPline2dSimple->setThickness(2);
  // Show statistic
  showStat2d(pPline2dSimple);


  // Create cubic spile-fit 2D polyline
  OdDb2dPolylinePtr pPline2dCubic = OdDb2dPolyline::createObject();
  pPline2dCubic->setDatabaseDefaults(bBTR->database());
  bBTR->appendOdDbEntity(pPline2dCubic);
  fillPolyline2d(pPline2dCubic, 0, 20, 0.2, 1.0, bBTR);
  // Make Polyline closed
  pPline2dCubic->makeClosed();
  // Set thickness
  pPline2dSimple->setThickness(5);
  // Make spline-fit polyline
  pPline2dCubic->splineFit();
  // Show statistic
  showStat2d(pPline2dCubic);

  // Create quadric spile-fit 2D polyline
  OdDb2dPolylinePtr pPline2dQuadric = OdDb2dPolyline::createObject();
  pPline2dQuadric->setDatabaseDefaults(bBTR->database());
  bBTR->appendOdDbEntity(pPline2dQuadric);
  fillPolyline2d(pPline2dQuadric, 0, 0, 0.2, 1.0, bBTR);
  // Make Polyline closed
  pPline2dQuadric->makeClosed();
  // Set thickness
  pPline2dSimple->setThickness(1);
  // Make spline-fit polyline
  pPline2dQuadric->splineFit();
  // Make quadric spline-fit polyline
  pPline2dQuadric->convertToPolyType(OdDb::k2dQuadSplinePoly);
  // Show statistic
  showStat2d(pPline2dQuadric);

  // Create curve-fit 2D polyline
  OdDb2dPolylinePtr pPline2dCurve = OdDb2dPolyline::createObject();
  pPline2dCurve->setDatabaseDefaults(bBTR->database());
  bBTR->appendOdDbEntity(pPline2dCurve);
  fillPolyline2d(pPline2dCurve, 0, -20, 0.2, 1.0, bBTR);
  // Make Polyline closed
  pPline2dCurve->makeClosed();
  // Make curve-fit polyline
  pPline2dCurve->curveFit();
  // Set thickness
  pPline2dSimple->setThickness(4);
  // Show statistic
  showStat2d(pPline2dCurve);


  /**********************************************************************/
  /* Create 3D Polylines                                                */
  /**********************************************************************/
  // Create simple 3D polyline
  OdDb3dPolylinePtr pPline3dSimple = OdDb3dPolyline::createObject();
  pPline3dSimple->setDatabaseDefaults(bBTR->database());
  bBTR->appendOdDbEntity(pPline3dSimple);
  fillPolyline3d(pPline3dSimple, 80, 40, 5.0, bBTR);
  // Make Polyline closed
  pPline3dSimple->makeClosed();
  showStat3d(pPline3dSimple);


  // Create cubic spile-fit 3D polyline
  OdDb3dPolylinePtr pPline3dCubic = OdDb3dPolyline::createObject();
  pPline3dCubic->setDatabaseDefaults(bBTR->database());
  bBTR->appendOdDbEntity(pPline3dCubic);
  fillPolyline3d(pPline3dCubic, 80, 20, 5.0, bBTR);
  // Make Polyline closed
  pPline3dCubic->makeClosed();
  // Make spline-fit polyline
  pPline3dCubic->splineFit();
  showStat3d(pPline3dSimple);

  // Create quadric spile-fit 3D polyline
  OdDb3dPolylinePtr pPline3dQuadric = OdDb3dPolyline::createObject();
  pPline3dQuadric->setDatabaseDefaults(bBTR->database());
  bBTR->appendOdDbEntity(pPline3dQuadric);
  fillPolyline3d(pPline3dQuadric, 80, 0, 5.0, bBTR);
  // Make Polyline closed
  pPline3dQuadric->makeClosed();
  // Make spline-fit polyline
  pPline3dQuadric->splineFit();
  // Make quadric spline-fit polyline
  pPline3dQuadric->convertToPolyType(OdDb::k3dQuadSplinePoly);
  showStat3d(pPline3dSimple);

  OdDb::SaveType fileType = OdDb::kDwg;
  OdDb::DwgVersion outVer = OdDb::vAC24;
  OdString f(argv[1]); 
    OdWrFileBuf fb(f.c_str());
  pDb->writeFile(&fb, fileType, outVer, true /* save preview */);


  odPrintConsoleString(L"\nDatabase has been saved as %ls", pDb->getFilename().c_str());
  odPrintConsoleString(L"\n");
  system("pause");
    
  
  pDb.release();
  /**********************************************************************/
  /* Uninitialize the Drawings SDK                                      */
  /**********************************************************************/
  odUninitialize();

  return 1;
}

