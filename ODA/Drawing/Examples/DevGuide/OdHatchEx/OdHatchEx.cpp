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
/* This console application demonstates the OdDbHatch object            */
/* functionality file and creates a .dwg file filled with               */
/* Hatch entities.                                                      */
/*                                                                      */
/* Calling sequence:  OdHatchEx <filename>                              */
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

#define STL_USING_IOSTREAM
#include "OdaSTL.h"
#define  STD(a)  std:: a

#ifdef OD_HAVE_CONSOLE_H_FILE
#include <console.h>
#endif


#define ODHATCHEX_STR L"OdHatchEx"


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



void showStat(OdDbHatch* pHatch)
{
  odPrintConsoleString(L"\n-----------");
  odPrintConsoleString(L"\nSTART DISPLAYING HATCH DATA");
  odPrintConsoleString(L"\n-----------");
 
  odPrintConsoleString(L"\npNumber of loops %i", pHatch->numLoops());
  if (pHatch->hatchObjectType() == 0)
    odPrintConsoleString(L"\npHatch is classic hatch object");
  else odPrintConsoleString(L"\npHatch is gradient object");

  OdString str;
  OdInt8 nHatchStyle;
  nHatchStyle = pHatch->hatchStyle();
  switch(nHatchStyle)
  {
    case 0:
      str = "'normal'";
      break;
    case 1:
      str = "'outer'";
      break;
    case 2:
      str = "'ignore'";
      break;
  }
  odPrintConsoleString(L"\nHatch style is %s", str.c_str());

  if (pHatch->associative())
    odPrintConsoleString(L"\nHatch is associative");
  else odPrintConsoleString(L"\nHatch is not associative");

  double area;
  pHatch->getArea(area);
  odPrintConsoleString(L"\nArea of Hatch is %f", area);

  OdGePoint2d orPoint = pHatch->originPoint();
  odPrintConsoleString(L"\nOrigin x= %f y= %f", orPoint.x, orPoint.y);

  OdGeVector3d normal = pHatch->normal();
  odPrintConsoleString(L"\nNormal x= %f y= %f z= %f", normal.x, normal.y, normal.z);

  odPrintConsoleString(L"\nElevation of Hatch %f", pHatch->elevation());

  if (pHatch->hatchObjectType() == 0)
  {
    OdInt8 nHatchType = pHatch->patternType();
    switch(nHatchType)
    {
      case 0:
      str = "user defined";
      break;
    case 1:
      str = "predefined";
      break;
    case 2:
      str = "custom defined";
      break;
    }
    odPrintConsoleString(L"\nHatch is %s", str.c_str());

    str = pHatch->patternName();
    odPrintConsoleString(L"\nName of hatch pattern is %s", str.c_str());

    double dAngle = pHatch->patternAngle();
    odPrintConsoleString(L"\nAngle of hatch pattern is %f radians", dAngle);

    double dScale = pHatch->patternScale();
    odPrintConsoleString(L"\nPattern scale is %f", dScale);

    if (pHatch->patternDouble())
      odPrintConsoleString(L"\nPattern is double-hatched");
    else odPrintConsoleString(L"\nPattern single-hatched");
  }
  else
  {
    OdInt8 nHatchType = pHatch->gradientType();
    switch(nHatchType)
    {
      case 0:
        str = "predefined";
        break;
      case 1:
        str = "user defined";
        break;
    }
    odPrintConsoleString(L"\nGradient is %s", str.c_str());

    OdString gradName = pHatch->gradientName();
    odPrintConsoleString(L"\nName of gradient is %s", gradName.c_str());

    double dAngle = pHatch->gradientAngle();
    odPrintConsoleString(L"\nGradient angle is %f radians", dAngle);

    double dShift = pHatch->gradientShift();
    odPrintConsoleString(L"\nGradient shift is %f", dShift);
  }



  odPrintConsoleString(L"\n-----------");
  odPrintConsoleString(L"\nEND DISPLAYING HATCH DATA");
  odPrintConsoleString(L"\n-----------");
  odPrintConsoleString(L"\n\n\n");
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
    odPrintConsoleString(ODHATCHEX_STR L" sample program. Copyright (c) 2019, Open Design Alliance\n");
    ODA_TRACE(L"Something gona happen.\n");
    odPrintConsoleString(L"\tusage: " ODHATCHEX_STR L" filename\n");
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
  /* Create the hatches                                                 */
  /**********************************************************************/
  OdDbBlockTableRecordPtr bBTR = pDb->getModelSpaceId().safeOpenObject(OdDb::kForWrite);


  /**********************************************************************/
  /* Create the first hatch, that describes the simple solid pattern    */
  /**********************************************************************/
  
  // Create object
  OdDbHatchPtr pHatch = OdDbHatch::createObject();

  // Add object into the model space
  pModelSpace = pDb->getModelSpaceId().safeOpenObject(OdDb::kForWrite);
  pModelSpace->appendOdDbEntity(pHatch);

  //Applies the default properties of the specified database to hatch Object.
  pHatch->setDatabaseDefaults(pModelSpace->database());

  /**********************************************************************/
  /* Set the parameters                                                 */
  /**********************************************************************/
  pHatch->setAssociative(false);
  pHatch->setPattern(OdDbHatch::kPreDefined, OD_T("SOLID"));
  pHatch->setHatchStyle(OdDbHatch::kNormal);

  // Define the hatch color
  OdCmColor col;
  col.setRGB(0, 150, 255);
  pHatch->setColor(col);
  
  // Define the loop of hatch (2D circle)
  OdGeCircArc2d *cirArc = new OdGeCircArc2d();
  // Define the center of the loop
  OdGePoint3d point;
  point.x = -5;
  point.y = 5;
  OdGePoint2d cenPt(point.x, point.y);
  // Set porpeties to circle
  cirArc->setCenter(cenPt);
  cirArc->setRadius(2);
  cirArc->setAngles(0.0, Oda2PI);
  // Define the array of edges and apply them to hatch
  EdgeArray edgePtrs;
  edgePtrs.append(cirArc);
  // Append loop to the hatch
  pHatch->appendLoop(OdDbHatch::kDefault, edgePtrs);

  showStat(pHatch);



  /******************************************************************************/
  /* Create the second hatch, that describes the simple user predefined pattern */
  /******************************************************************************/
  OdDbHatchPtr pHatch2 = OdDbHatch::createObject();
  // Add object into the model space
  pModelSpace = pDb->getModelSpaceId().safeOpenObject(OdDb::kForWrite);
  pModelSpace->appendOdDbEntity(pHatch2);
  //Applies the default properties of the specified database to Arc Object.
  pHatch2->setDatabaseDefaults(pModelSpace->database());
  pHatch2->setDatabaseDefaults(pDb);
  OdDbObjectId hatchId = bBTR->appendOdDbEntity(pHatch2);

  /**********************************************************************/
  /* Set some properties                                                */
  /**********************************************************************/
  pHatch2->setAssociative(true);
  pHatch2->setDatabaseDefaults(pDb); // make hatch aware of DB for the next call
  pHatch2->setPattern(OdDbHatch::kUserDefined, OD_T("_USER"));
  // Set pattern scale and pattern angle
  pHatch2->setPatternSpace(0.5);
  pHatch2->setPatternAngle(OdaToRadian(30.0));
  // Set PatternDouble flag
  pHatch2->setPatternDouble(true);
  pHatch2->setHatchStyle(OdDbHatch::kNormal);
 
  /**********************************************************************/
  /* Define the loops                                                   */
  /**********************************************************************/
  
  // Define loop as array of object IDs
  OdDbObjectIdArray loopIds;
  // Define ellipse
  OdDbEllipsePtr pEllipse = OdDbEllipse::createObject();
  pEllipse->setDatabaseDefaults(pDb);
  loopIds.push_back(bBTR->appendOdDbEntity(pEllipse));
  // Set  properties of ellipse
  OdGePoint3d centerPt ;
  centerPt.x = 4;
  centerPt.y = 5;
  pEllipse->set(centerPt, OdGeVector3d::kZAxis, OdGeVector3d(3, 0., 0.), 0.8);

  // Append loop to the hatch
  pHatch2->appendLoop(OdDbHatch::kDefault, loopIds);
  
  showStat(pHatch2);


  /**********************************************************************/
  /* Create the third hatch, that describes hatch with a circular hole */
  /**********************************************************************/
  OdDbHatchPtr pHatch3 = OdDbHatch::createObject();
  pHatch3->setDatabaseDefaults(pDb);
  OdDbObjectId whiteHatchId = bBTR->appendOdDbEntity(pHatch3);

  /**********************************************************************/
  /* Set some properties                                                */
  /**********************************************************************/

  pHatch3->setAssociative(false);
  pHatch3->setPattern(OdDbHatch::kPreDefined, OD_T("SOLID"));
  pHatch3->setHatchStyle(OdDbHatch::kNormal);

  /**********************************************************************/
  /* Define the outer loop with an OdGePolyline2d                       */
  /**********************************************************************/

  OdGePoint2dArray  vertexPts;
  OdGeDoubleArray   vertexBulges;
  vertexPts.resize(4);
  double delta = 2;
  point.x = 10;
  point.y = 10;
  vertexPts[0].set(point.x + delta,   point.y - delta);
  vertexPts[1].set(point.x + delta*5, point.y - delta);
  vertexPts[2].set(point.x + delta*5, point.y - delta*5);
  vertexPts[3].set(point.x + delta,   point.y - delta*5);
  // Append loop to the hatch
  pHatch3->appendLoop(OdDbHatch::kExternal | OdDbHatch::kPolyline, 
    vertexPts, vertexBulges);
  
  // Define the circle inner hole
  OdGeCircArc2d *cirArc10 = new OdGeCircArc2d();
  // Set some properties
  OdGePoint2d cenPt10(point.x + delta*3, point.y - delta*3);
  cirArc10->setCenter(cenPt10);
  cirArc10->setRadius(delta);
  cirArc10->setAngles(0.0, Oda2PI);

  EdgeArray edgePtrsIn;
  
  edgePtrsIn.append(cirArc10);
  // Append loop to the hatch
  pHatch3->insertLoopAt(1, OdDbHatch::kDefault, edgePtrsIn);

  showStat(pHatch3);


  /**********************************************************************/
  /* Create fourth hatch, that describes an associative predefined hatch */
  /**********************************************************************/

  try
  {
	OdDbHatchPtr pHatch4 = OdDbHatch::createObject();
    pHatch4->setDatabaseDefaults(pDb);
    OdDbObjectId hatchId = bBTR->appendOdDbEntity(pHatch4);

    /********************************************************************/
    /* Set some properties                                              */
    /********************************************************************/

    pHatch4->setAssociative(true);
    pHatch4->setDatabaseDefaults(pDb);// make hatch aware of DB for the next call
    pHatch4->setPattern(OdDbHatch::kPreDefined, OD_T("BOX"));
    pHatch4->setPatternScale(2);
	pHatch4->setPatternAngle(OdaToRadian(45.0));
    pHatch4->setHatchStyle(OdDbHatch::kNormal);

	/********************************************************************/
    /* Define the loops                                                 */
    /********************************************************************/

    loopIds.clear();
    OdDbCirclePtr pCircle = OdDbCircle::createObject();
    pCircle->setDatabaseDefaults(pDb);
    loopIds.push_back(bBTR->appendOdDbEntity(pCircle));
    centerPt.x += delta*2.0;
    centerPt.y -= delta*2.5;
    pCircle->setCenter(centerPt);
    pCircle->setRadius(delta*1.5);  
    // Append loop to the hatch
    pHatch4->appendLoop(OdDbHatch::kDefault, loopIds);

    pHatch4->evaluateHatch();

	showStat(pHatch4);
  }
  catch (const OdError& e)
  {
    odPrintConsoleString(L"\n\nException occurred: %ls\n", e.description().c_str());
    odPrintConsoleString(L"\nHatch with predefined pattern \"ANGLE\" was not added.\n");
    odPrintConsoleString(L"\nMake sure PAT file with pattern definition is available to Teigha.");
    odPrintConsoleString(L"\n\nPress ENTER to continue...");
    STD(cin).get();
  }

  /**********************************************************************/
  /* Define a custom hatch pattern OD_T("MY_TRIANGLE")                           */
  /**********************************************************************/

  OdHatchPattern     triangles;
  OdHatchPatternLine line;

  // Define first line
  line.m_dLineAngle = 0.0;
  line.m_patternOffset = OdGeVector2d(0, 0.2);
  line.m_dashes.push_back(-0.1);
  line.m_dashes.push_back(0.1);

  // Add line to pattern
  triangles.push_back(line);

  // Define second line
  line.m_dLineAngle = 1.57;
  line.m_patternOffset = OdGeVector2d(0, 0.2);
  line.m_dashes.clear();
  line.m_dashes.push_back(0.1);
  line.m_dashes.push_back(-0.1);

  // Add line to pattern
  triangles.push_back(line);

  // Define third line
  line.m_dLineAngle = 0.785;
  line.m_basePoint = OdGePoint2d(-0.1, 0);
  line.m_dashes.clear();
  line.m_dashes.push_back(0.1414);
  line.m_dashes.push_back(-0.1414);
  line.m_patternOffset = OdGeVector2d(-0.1414, 0.1414);

  // Add line to pattern
  triangles.push_back(line);

  /**********************************************************************/
  /* Register the pattern                                                */
  /**********************************************************************/
  pDb->appServices()->patternManager()->appendPattern(OdDbHatch::kCustomDefined, 
    OD_T("MY_TRIANGLE"), triangles);

  /**********************************************************************/
  /* Create fifth hatch as an associative custom defined hatch          */
  /**********************************************************************/

  OdDbHatchPtr pHatch5 = OdDbHatch::createObject();
  pHatch5->setDatabaseDefaults(pDb);
  OdDbObjectId hatchId5 = bBTR->appendOdDbEntity(pHatch5);

  /**********************************************************************/
  /* Set some properties                                                */
  /**********************************************************************/

  pHatch5->setAssociative(true);
  pHatch5->setDatabaseDefaults(pDb); // make hatch aware of DB for the next call
  pHatch5->setPattern(OdDbHatch::kCustomDefined, OD_T("MY_TRIANGLE"));
  pHatch5->setPatternScale(0.5);
  pHatch5->setPatternSpace(2);
  pHatch5->setHatchStyle(OdDbHatch::kNormal);

  /**********************************************************************/
  /* Define the loops                                                   */
  /**********************************************************************/

  loopIds.clear();
  OdDbCirclePtr pCircle2 = OdDbCircle::createObject();
  pCircle2->setDatabaseDefaults(pDb);
  loopIds.push_back(bBTR->appendOdDbEntity(pCircle2));

  centerPt.x = 0 ;
  centerPt.y = 0;
  pCircle2->setCenter(centerPt);
  pCircle2->setRadius(delta*1.5); 

  // Append the loop to the hatch 
  pHatch5->appendLoop(OdDbHatch::kDefault, loopIds);

  showStat(pHatch5);

  /**********************************************************************/
  /* Create the sixth hatch, that describes gradient hatch              */
  /**********************************************************************/
  

  OdDbHatchPtr pHatch6 = OdDbHatch::createObject();
  // Add object into the model space
  pModelSpace = pDb->getModelSpaceId().safeOpenObject(OdDb::kForWrite);
  pModelSpace->appendOdDbEntity(pHatch6);
  //Applies the default properties of the specified database to Arc Object.
  pHatch6->setDatabaseDefaults(pModelSpace->database());

  /**********************************************************************/
  /* Set the parameters                                                 */
  /**********************************************************************/
  pHatch6->setAssociative(false);
  pHatch6->setHatchObjectType(OdDbHatch::HatchObjectType::kGradientObject);
  pHatch6->setGradient(OdDbHatch::GradientPatternType::kPreDefinedGradient, OD_T("SPHERICAL"));

  // Define colors for gradient
  OdCmColor col1;
  col1.setRGB(0, 255, 50);

  OdCmColor col2;
  col2.setRGB(255, 30, 0);
  
  OdCmColor colors[2] = {col1, col2};
  double gradientValues[2] = {0.0, 1.0};
  // Set colors
  pHatch6->setGradientColors(2, colors, gradientValues);
  // Shift the gradient
  pHatch6->setGradientShift(-0.5);

  /**********************************************************************/
  /* Define the loops                                                   */
  /**********************************************************************/
  OdGeCircArc2d *cirArc6 = new OdGeCircArc2d();
  OdGePoint3d point6;
  point6.x = 4;
  point6.y = -5;
  OdGePoint2d cenPt6(point6.x, point6.y);
  cirArc6->setCenter(cenPt6);
  cirArc6->setRadius(3);
  cirArc6->setAngles(0.0, Oda2PI);
  EdgeArray edgePtrs6;
  edgePtrs6.append(cirArc6);
  // Append the loop
  pHatch6->appendLoop(OdDbHatch::kDefault, edgePtrs6);

  showStat(pHatch6);


  OdDb::SaveType fileType = OdDb::kDwg;
  OdDb::DwgVersion outVer = OdDb::vAC24;
  OdString f(argv[1]); 
    OdWrFileBuf fb(f.c_str());
  pDb->writeFile(&fb, fileType, outVer, true /* save preview */);


  
  


    odPrintConsoleString(L"\nDatabase has been saved as %ls", pDb->getFilename().c_str());
	odPrintConsoleString(L"\n");
  //odPrintConsoleString(L"\n Object %s was loaded", pTable->handle().ascii().c_str());
  //showStat(pTable);
  system("pause");
    
  
  pDb.release();
  /**********************************************************************/
  /* Uninitialize the Drawings SDK                                      */
  /**********************************************************************/
  odUninitialize();

  return 1;
}

