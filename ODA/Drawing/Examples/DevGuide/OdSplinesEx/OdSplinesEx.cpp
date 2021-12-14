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
/* This console application demonstates the OdDbSpline object           */
/* functionality and creates a .dwg file filled with                    */
/*  Spline entities.                                                    */
/*                                                                      */
/* Calling sequence:  OdSplinesEx <filename>                            */
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

#include "DbPolygonMesh.h"

#include "DbSpline.h"


#define STL_USING_IOSTREAM
#include "OdaSTL.h"
#define  STD(a)  std:: a

#ifdef OD_HAVE_CONSOLE_H_FILE
#include <console.h>
#endif


#define ODSPLINESEX_STR L"OdSplinesEx"


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




void showStat(OdDbSpline* pSpline)
{
  OdString about;
  OdResult eRes; 
  OdGePlane plane3d;
  double a, b, c, d;
  OdDb::Planarity planarity;
  double vArea;
  double vStartParam;
  double vEndParam;
  OdGePoint3d start;
  OdGePoint3d end;
  OdGeVector3d vecFirst;
  OdGeVector3d vecSecond;
  double vTotalLength;
  int degree;
  bool rational;
  bool closed;
  bool periodic;
  OdGePoint3dArray controlPoints;
  OdGeDoubleArray knots;
  OdGeDoubleArray weights;
  double controlPtTol;
  double knotTol;
  OdGePoint3dArray fitPoints;
  double fitTolerance;
  bool tangentsExist;
  OdGeVector3d pStartTan;
  OdGeVector3d pEndTan;

  odPrintConsoleString(L"\n/////////////START DISPLAYING SPLINE DATA/////////////");
  //Spline type
  switch(pSpline->type())
  {
    case OdDbSpline::kControlPoints: about = L"'by control points'"; break;
    case OdDbSpline::kFitPoints: about = L"'by fit points'"; break;
  }
  odPrintConsoleString(L"\nSpline type is %s", about.c_str());

  // 'Closed' status
  odPrintConsoleString(L"\nSpline is %s", ((pSpline->isClosed()) ? L"closed" : L"opened"));

  // 'Periodic' status
  odPrintConsoleString(L"\nSpline is %s", ((pSpline->isPeriodic()) ? L"periodic" : L"aperiodic"));

  // Plane
  odPrintConsoleString(L"\nSpline is %s", ((pSpline->isPlanar()) ? L"planar" : L"non-planar"));
  eRes = pSpline->getPlane(plane3d, planarity);
  if(eRes == eOk && planarity != OdDb::kNonPlanar) 
  {
    plane3d.getCoefficients(a, b, c, d);
    odPrintConsoleString(L"\nSpline plane is (%g * X + %g * Y + %g * Z + %g)", a, b, c, d);
  }

  // Area
  eRes = pSpline->getArea(vArea);
  if(eRes == eOk)
    odPrintConsoleString(L"\nspline area = %g", vArea);

  // Start parameter
  eRes = pSpline->getStartParam(vStartParam);
  if(eRes == eOk)
    odPrintConsoleString(L"\nStart parameter = %g", vStartParam);

  // End parameter
  eRes = pSpline->getEndParam(vEndParam);
  if(eRes == eOk)
    odPrintConsoleString(L"\nEnd parameter = %g", vEndParam);

  // Start point
  eRes = pSpline->getStartPoint(start);
  if(eRes == eOk)
    odPrintConsoleString(L"\nStart point = (%g,%g,%g)", start.x, start.y, start.z);

  // First and second derivatives of the first point
  eRes = pSpline->getFirstDeriv(vStartParam, vecFirst);
  if(eRes == eOk) 
    odPrintConsoleString(L"\nFirst derivative vector of the first point is (%g,%g,%g)", vecFirst.x, vecFirst.y, vecFirst.z);
  eRes = pSpline->getSecondDeriv(vStartParam, vecSecond);
  if(eRes == eOk)
    odPrintConsoleString(L"\nSecond derivative vector of the first point is (%g,%g,%g)", vecSecond.x, vecSecond.y, vecSecond.z);

  // End point
  eRes = pSpline->getEndPoint(end); 
  if(eRes == eOk)
    odPrintConsoleString(L"\nEnd point = (%g,%g,%g)", end.x, end.y, end.z);
  
  // First and second derivatives of the last point
  eRes = pSpline->getFirstDeriv(vEndParam, vecFirst);
  if(eRes == eOk) 
    odPrintConsoleString(L"\nFirst derivative vector of the last point is (%g,%g,%g)", vecFirst.x, vecFirst.y, vecFirst.z);
  eRes = pSpline->getSecondDeriv(vEndParam, vecSecond);
  if(eRes == eOk)
    odPrintConsoleString(L"\nSecond derivative vector of the last point is (%g,%g,%g)", vecSecond.x, vecSecond.y, vecSecond.z);

  // Total lenght of the spline
  eRes= pSpline->getDistAtParam(vEndParam, vTotalLength);
  if(eRes == eOk)
    odPrintConsoleString(L"\nSpline length = %g", vTotalLength);

  // Get NURBS data
  pSpline->getNurbsData(degree, rational, closed, periodic, controlPoints, knots, weights, controlPtTol, knotTol);

  // Degree
  odPrintConsoleString(L"\nDegree = %d", degree);

  // 'Rational' status
  odPrintConsoleString(L"\nSpline is %s", ((rational) ? L"rational" : L"nonrational"));

  // Control point tolerance
  odPrintConsoleString(L"\nControl point tolerance: %f", controlPtTol);

  // Knot tolerance
  odPrintConsoleString(L"\nKnot point tolerance: %f", knotTol);

  // Control points
  for (int i = 0; i < controlPoints.size(); i++)
  {
    odPrintConsoleString(L"\nControl point %d coordinates: %f %f %f", i, controlPoints[i].x, controlPoints[i].y, controlPoints[i].z);
  }

  // Knots
  for (int i = 0; i < knots.size(); i++)
  {
    odPrintConsoleString(L"\nKnot %d is %f", i, knots[i]);
  }

  // Get fit data
  pSpline->getFitData(fitPoints, degree, fitTolerance, tangentsExist, pStartTan, pEndTan);
  if (pSpline->hasFitData())
  {
    // Fit points
    for (int i = 0; i < fitPoints.size(); i++)
      odPrintConsoleString(L"\nFit point %d coordinates: %f %f %f", i, fitPoints[i].x, fitPoints[i].y, fitPoints[i].z);
  
    // Fit tolerance
    odPrintConsoleString(L"\nSpline fit tolerance is %f", fitTolerance);

    // Tangent vectors
    if (tangentsExist)
    {
      odPrintConsoleString(L"\nStart tangent vector: %f %f %f", pStartTan.x, pStartTan.y, pStartTan.z);
      odPrintConsoleString(L"\nEnd tangent vector: %f %f %f", pEndTan.x, pEndTan.y, pEndTan.z);
    }
  }
  
  odPrintConsoleString(L"\n//////////////END DISPLAYING SPLINE DATA//////////////");
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
#if (defined(WIN32) || defined(WIN64))
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
    odPrintConsoleString(ODSPLINESEX_STR L" sample program. Copyright (c) 2019, Open Design Alliance\n");
    ODA_TRACE(L"Something gona happen.\n");
    odPrintConsoleString(L"\tusage: " ODSPLINESEX_STR L" filename\n");
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

  OdDbBlockTableRecordPtr bBTR = pDb->getModelSpaceId().safeOpenObject(OdDb::kForWrite);


  /**********************************************************************/
  /* Create Splines                                                     */
  /**********************************************************************/
  /**********************************************************************/
  /* Create first Spline via the fit data                               */
  /**********************************************************************/
  // Create and initialize the spline
  OdDbSplinePtr pSpline = OdDbSpline::createObject();
  pSpline->setDatabaseDefaults(bBTR->database());
  bBTR->appendOdDbEntity(pSpline);

  // Declare an array of fit points
  OdGePoint3d point = OdGePoint3d (0,0,0);
  double dx   = 8.0;
  double dy   = 8.0;

  OdGePoint3dArray fitPoints;
  fitPoints.push_back(point + OdGeVector3d(1.0 * dx, 1.0 * dy, 0.0));
  fitPoints.push_back(point + OdGeVector3d(3.0 * dx, 6.0 * dy, 0.0));
  fitPoints.push_back(point + OdGeVector3d(4.0 * dx, 2.0 * dy, 0.0));
  fitPoints.push_back(point + OdGeVector3d(7.0 * dx, 7.0 * dy, 0.0));
  
  // Declare degree
  int degree = 3;

  // Declare fit tolerance
  double fitTolerance = 0.0;

  // Declare start tangent
  OdGeVector3d pStartTan = OdGeVector3d(0.0, 0.0, 0.0);
  OdGeVector3d pEndTan = OdGeVector3d(1.0, 0.0, 0.0);
  
  // Set fit data 
  pSpline->setFitData(fitPoints, degree, fitTolerance, pStartTan, pEndTan);
  
  // Show spline data
  showStat(pSpline);


  /**********************************************************************/
  /* Create second Spline via the NURBS data                            */
  /**********************************************************************/
  // Create and initialize the spline
  OdDbSplinePtr pSpline2 = OdDbSpline::createObject();
  pSpline2->setDatabaseDefaults(bBTR->database());
  bBTR->appendOdDbEntity(pSpline2);

  // Get NURBS data from the first spline to make tha same spline
  bool rational;
  bool closed;
  bool periodic;
  OdGePoint3dArray controlPoints;
  OdGeDoubleArray knots;
  OdGeDoubleArray weights;
  double controlPtTol;
  double knotTol;
  pSpline->getNurbsData(degree, rational, closed, periodic, controlPoints, knots, weights, controlPtTol, knotTol);
  
  //Shift all points by 100 on X axis to make second spline visible under the first spline
  for (int i = 0; i <controlPoints.size(); i++)
	  controlPoints[i].x +=100;
  
  // Set NURBS data to the second spline
  pSpline2->setNurbsData(degree, rational, closed, periodic, controlPoints, knots, weights, controlPtTol, knotTol);
  
  // Show spline data
  showStat(pSpline2);


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

