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
/* This console application demonstates the OdArcAlignedText object     */
/* functionality file and creates a .dwg file filled with               */
/* ArcAlignedText entities.                                             */
/*                                                                      */
/* Calling sequence:  ArcAlignedTextEx <filename>                       */
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

#include <iostream>
using namespace std;

#define STL_USING_IOSTREAM
#include "OdaSTL.h"
#define  STD(a)  std:: a

#ifdef OD_HAVE_CONSOLE_H_FILE
#include <console.h>
#endif


#define ARCALIGNEDTEXTEX_STR L"ArcAlignedTextEx"

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

/********************************************************************/
/* Print specific Arc Aligned Text properties.	                    */
/********************************************************************/
void printArcAlignedTextProperties(OdDbArcAlignedText* pArcText)
{
	wcout<<L"\n  Text String = "<< pArcText->textString().c_str();
	odPrintConsoleString(L"\n  xScale = %1.2f", pArcText->xScale());
	odPrintConsoleString(L"\n  Text Size = %1.2f", pArcText->textSize());
	odPrintConsoleString(L"\n  Offset from Arc = %1.2f", pArcText->offsetFromArc());
	odPrintConsoleString(L"\n  Char Spacing = %1.2f", pArcText->charSpacing());

	OdGePoint3d center = pArcText->center();
	odPrintConsoleString(L"\n  Center = (%g,%g,%g)", center.x, center.y, center.z); 
	odPrintConsoleString(L"\n  Radius = %1.2f", pArcText->radius());
	odPrintConsoleString(L"\n  Start Angle = %1.2f", OdaToDegree(pArcText->startAngle()));
	odPrintConsoleString(L"\n  End Angle = %1.2f", OdaToDegree(pArcText->endAngle()));
		
	if(pArcText->isUnderlined())
		odPrintConsoleString(L"\n  Text Underlined = true");
	else
		odPrintConsoleString(L"\n  Text Underlined = false");

	int alignment = pArcText->alignment();
	switch (alignment)
	{
	  case kRight:
		odPrintConsoleString(L"\n  Text Alignment = Right");
		break;
	  case kCenter:
		odPrintConsoleString(L"\n  Text Alignment = Center");
		break;
	  case kFit:
		odPrintConsoleString(L"\n  Text Alignment = Fit");
		break;
	  case kLeft:
		odPrintConsoleString(L"\n  Text Alignment = Left");
		break;
	}

	int position = pArcText->textPosition();
	if (position == kOnConcaveSide)
		odPrintConsoleString(L"\n  Text Position = On Concave Side");
	else
		odPrintConsoleString(L"\n  Text Position = On Convex Side");

	int direction = pArcText->textDirection();
	if(direction == kInwardToTheCenter)
		odPrintConsoleString(L"\n  Text Direction = To the Center");
	else
		odPrintConsoleString(L"\n  Text Direction = From Center");

	if(pArcText->isReversedCharOrder())
		odPrintConsoleString(L"\n  Current char order mode = reversed");
	else
		odPrintConsoleString(L"\n  Current char order mode = not reversed");

	OdCmColor color = pArcText->color();
	odPrintConsoleString(L"\n  Text Color in RGB Format = %s", color.colorNameForDisplay().c_str());
	
	OdString typeFace;
	bool bold, italic;
	int charset, pitchAndFamily;
	pArcText->font(typeFace, bold, italic, charset, pitchAndFamily);
	odPrintConsoleString(L"\n  Text Type Face = %s", typeFace.c_str());
	if(bold)
		odPrintConsoleString(L"\n  Text Bold = true");
	else
		odPrintConsoleString(L"\n  Text Bold = false");
	if(italic)
		odPrintConsoleString(L"\n  Text Italic = true");
	else
		odPrintConsoleString(L"\n  Text Italic =  false");
	odPrintConsoleString(L"\n  Text Charset = %d", charset);
	odPrintConsoleString(L"\n  Text Pitch and Family = %d \n", pitchAndFamily);
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
		odPrintConsoleString(ARCALIGNEDTEXTEX_STR L" sample program. Copyright (c) 2019, Open Design Alliance\n");
		odPrintConsoleString(L"\tusage: " ARCALIGNEDTEXTEX_STR L" filename\n");
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
	/* Initialize the Drawings SDK.                                     */
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
	/* Add an Arc                                                         */
	/**********************************************************************/
	// Create Arc Object
	OdDbArcPtr pArc = OdDbArc::createObject();
	// Add Arc Object in the model space
	OdDbBlockTableRecordPtr pModelSpace = pDb->getModelSpaceId().safeOpenObject(OdDb::kForWrite);
	pModelSpace->appendOdDbEntity(pArc);
	//Applies the default properties of the specified database to Arc Object.
	pArc->setDatabaseDefaults(pModelSpace->database());
	
	// Set Arc Object properties    
	OdGePoint3d center = OdGePoint3d(5.0, 3.0, 0.0);
	pArc->setCenter(center);
	pArc->setRadius(5.0);
	pArc->setStartAngle(OdaToRadian(75.0));
	pArc->setEndAngle(OdaToRadian(165.0));
	
	/**********************************************************************/
	/* Add an Arc Aligned Text with default properties                    */
	/**********************************************************************/
	// Create Arc Text Object
	OdDbArcAlignedTextPtr pArcText = OdDbArcAlignedText::createObject();
	// Add Arc Text Object in the model space. Arc Aligned Text must be added 
	// to a database before connecting with Arc object.
	pModelSpace->appendOdDbEntity(pArcText);
	// Applies the default properties of the specified database to Arc Text Object
	pArcText->setDatabaseDefaults(pModelSpace->database());
	// Connect Arc Text Object with Arc Object
	pArcText->setArcId(pArc->objectId());
	// Set text style for Arc Text Object 
	pArcText->setTextStyle(ttfStyleId);
	// Set text string
	pArcText->setTextString("Arc text");

	/**********************************************************************/
	/* Add an Arc with changed position to demonstrate                    */
	/* the changed properties of Arc Aligned Text						  */	
	/**********************************************************************/
	// Create Arc Object
	OdDbArcPtr pArc2 = OdDbArc::createObject();
	// Add Arc Object in the model space
	pModelSpace->appendOdDbEntity(pArc2);
	//Applies the default properties of the specified database to Arc Object.
	pArc2->setDatabaseDefaults(pModelSpace->database());
	
	// Set Arc Object properties 
	center = OdGePoint3d(12.0, 3.0, 0.0);// move the center coordinate of Arc Object along the X-axis
	pArc2->setCenter(center);
	pArc2->setRadius(5.0);
	pArc2->setStartAngle(OdaToRadian(75.0));
	pArc2->setEndAngle(OdaToRadian(165.0));
	
	/**********************************************************************/
	/* Add an Arc Aligned Text with changed properties                    */
	/**********************************************************************/
	// Create Arc Text Object
	OdDbArcAlignedTextPtr pArcText2 = OdDbArcAlignedText::createObject();
	// Add Arc Text Object in the model space
	pModelSpace->appendOdDbEntity(pArcText2);
	// Applies the default properties of the specified database to Arc Text Object
	pArcText2->setDatabaseDefaults(pModelSpace->database());
	// Connect Arc Text Object with Arc Object
	pArcText2->setArcId(pArc2->objectId());
	
	// Set text style for Arc Text Object 
	pArcText2->setTextStyle(ttfStyleId);
	// Set text string
	pArcText2->setTextString("Text");
	// Set the xScale (xScale >= 0)
	pArcText2->setXScale(2.0);
	// Set the text position
	pArcText2->setTextPosition(kOnConcaveSide);
	// Set the text size (size > 0)
	pArcText2->setTextSize(0.5);
	// Set the text direction
	pArcText2->setTextDirection(kInwardToTheCenter);
	// Set the offset
	pArcText2->setOffsetFromArc(0.6);
	// Set the undetlined
	pArcText2->setUnderlined(true);
	// Set the text alignment
	pArcText2->setAlignment(kCenter);
	// Set the text color 
	OdCmColor color;
	color.setRGB(255, 0, 0); //every number must be between 0 and 255
	pArcText2->setColor(color);
	// Set the char spacing (char spacing is a double value)
	pArcText2->setCharSpacing(0.5);
	// Set the char order mode
	pArcText2->reverseCharOrder(true);
	//Set the text font
	//	- Text Type Face = Times New Roman
	//	- Text Bold = true
	//	- Text Italic = true
	//	- Text Charset = 1251
	//	- Text Pitch And Family = 0
	 pArcText2->setFont("Times New Roman", true, true, 1251, 0);
	 
	/**********************************************************************/
	/* Save Arc Text with changed properties in .dwg file                 */
	/**********************************************************************/
	OdDb::SaveType fileType = OdDb::kDwg;
	OdDb::DwgVersion outVer = OdDb::vAC24;
	OdString f1(argv[1]); 
    OdWrFileBuf fb1(f1.c_str());
	// Save Arc Text
	pDb->writeFile(&fb1, fileType, outVer, true /*save preview */);
    odPrintConsoleString(L"\nDatabase has been saved as %ls", pDb->getFilename().c_str());

	/**********************************************************************/
	/* Show default values of specific properties of Arc Text Object      */
	/**********************************************************************/
	odPrintConsoleString(L"\n  ------ Default Values of Specific Arc-Aligned Text Properties");
	printArcAlignedTextProperties(pArcText);
	
	/**********************************************************************/
	/* Show changed values of specific properties of Arc Text Object      */
	/**********************************************************************/
	odPrintConsoleString(L"\n  ------ Specific Arc-Aligned Text Properties");
	printArcAlignedTextProperties(pArcText2);
	system("pause");

	/**********************************************************************/
	/* Delete the database before uninitialization						  */
	/**********************************************************************/
	pDb.release();

	/**********************************************************************/
	/* Uninitialize the Drawings SDK                                      */
	/**********************************************************************/
	odUninitialize();

	return 1;
}
