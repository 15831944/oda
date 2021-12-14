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
/* This console application demonstates the OdDbTable object            */
/* functionality file and creates a .dwg file filled with               */
/* Table entities.                                                      */
/*                                                                      */
/* Calling sequence:  OdTableEx <filename>                              */
/*                                                                      */
/************************************************************************/

#include "OdaCommon.h"
#include "diagnostics.h"
#include "DbDatabase.h"
#include "ExSystemServices.h"
#include "ExHostAppServices.h"
#include "DbTextStyleTable.h"
#include "DbTextStyleTableRecord.h"
#include "DbArc.h"
#include "DbBlockTable.h"
#include "DbBlockTableRecord.h"
#include "OdFileBuf.h"
#include "DbTable.h"
#include "DbAttributeDefinition.h"
#include "DbDictionary.h"

#define STL_USING_IOSTREAM
#include "OdaSTL.h"
#define  STD(a)  std:: a

#ifdef OD_HAVE_CONSOLE_H_FILE
#include <console.h>
#endif

#define ODTABLEEX_STR L"OdTableEx"

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
	/* Add an Arc                                                         */
	/**********************************************************************/
	OdDbArcPtr pArc = OdDbArc::createObject();
	pArc->setDatabaseDefaults(pDb);
	bBTR->appendOdDbEntity(pArc);
	pArc->setRadius(3.0);
	
	OdGePoint3d center = OdGePoint3d(5.0, 3.0, 0.0);
	center.y -= pArc->radius() / 2.0;

	pArc->setCenter(center);
	pArc->setStartAngle(OdaToRadian(0.));
	pArc->setEndAngle(OdaToRadian(180.));

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

void showStat(OdDbTable* pTable)
{
	odPrintConsoleString(L"\n-----------");
	odPrintConsoleString(L"\nTABLE DATA");
	odPrintConsoleString(L"\n-----------");
	odPrintConsoleString(L"\nMINIMUM TABLE SIZE");
	odPrintConsoleString(L"\nMinimum table width: %f", pTable->minimumTableWidth());
	odPrintConsoleString(L"\nMinimum table height: %f", pTable->minimumTableHeight());
	odPrintConsoleString(L"\n-----------");
	odPrintConsoleString(L"\nTABLE SIZE");
	odPrintConsoleString(L"\nTable width: %f", pTable->width());
	odPrintConsoleString(L"\nTable height: %f", pTable->height());
	odPrintConsoleString(L"\nNumber of rows: %i", pTable->numRows());
	odPrintConsoleString(L"\nNumber of columns: %i", pTable->numColumns());
	odPrintConsoleString(L"\n-----------");
	odPrintConsoleString(L"\nMINIMUM ROW HEIGHT");
	for (int row = 0; row < (int) pTable->numRows(); row ++)
	{
		odPrintConsoleString(L"\nRow %i = %f", row+1, pTable->minimumRowHeight(row));
	}
	odPrintConsoleString(L"\n-----------");
	odPrintConsoleString(L"\nMINIMUM COLUMN WIDTH");
	for (int col = 0; col < (int) pTable->numColumns(); col ++)
	{
		odPrintConsoleString(L"\nRow %i = %f", col+1, pTable->minimumColumnWidth(col));
	}
	odPrintConsoleString(L"\n-----------");
	odPrintConsoleString(L"\nROW HEIGHT");
	for (int row = 0; row < (int) pTable->numRows(); row ++)
	{
		odPrintConsoleString(L"\nRow %i = %f", row+1, pTable->rowHeight(row));
	}
	odPrintConsoleString(L"\n-----------");
	odPrintConsoleString(L"\nCOLUMN WIDTH");
	for (int col = 0; col < (int) pTable->numColumns(); col ++)
	{
		odPrintConsoleString(L"\nRow %i = %f", col+1, pTable->columnWidth(col));
	} 
	odPrintConsoleString(L"\n-----------");
	odPrintConsoleString(L"\n");
	odPrintConsoleString(L"\n-----------");
	odPrintConsoleString(L"\nCell styles\n");
	for (int row = 0; row < (int) pTable->numRows(); row++)
	{
		for (int col = 0; col < (int) pTable->numColumns(); col ++)
		{
			odPrintConsoleString(L"\t\t %s", pTable->cellStyle(row,col).c_str());
		} 
		odPrintConsoleString(L"\n");
	}
	odPrintConsoleString(L"\n-----------");
	odPrintConsoleString(L"\n");
}

/************************************************************************/
/* Show properties of breaking table.		                            */
/************************************************************************/
void showBreakingTableProp(const OdDbTable * pTable)
{
	odPrintConsoleString(L"\n-------------------------");
	odPrintConsoleString(L"\nBREAKING TABLE PROPERTIES");
	odPrintConsoleString(L"\n-------------------------");

	if(pTable->isBreakEnabled())
	{
		odPrintConsoleString(L"\nBreaking mode is enabled.");

		// Get break option
		switch(pTable->breakOption())
		{
			case(OdDb::kTableBreakNone): 
				odPrintConsoleString(L"\nBreaking option is kTableBreakNone."); break;

			case(OdDb::kTableBreakEnableBreaking): 
				odPrintConsoleString(L"\nBreaking option is kTableBreakEnableBreaking."); break;
			case(OdDb::kTableBreakRepeatTopLabels): 
				odPrintConsoleString(L"\nBreaking option is kTableBreakRepeatTopLabels."); break;
			case(OdDb::kTableBreakRepeatBottomLabels): 
				odPrintConsoleString(L"\nBreaking option is kTableBreakRepeatBottomLabels."); break;
			case(OdDb::kTableBreakAllowManualPositions): 
				odPrintConsoleString(L"\nBreaking option is kTableBreakAllowManualPositions."); break;
			case(OdDb::kTableBreakAllowManualHeights): 
				odPrintConsoleString(L"\nBreaking option is kTableBreakAllowManualHeights."); break;		
			case(OdDb::kTableBreakEnableBreaking|OdDb::kTableBreakRepeatTopLabels):
				odPrintConsoleString(L"\nBreaking options are: kTableBreakEnableBreaking and kTableBreakRepeatTopLabels."); break;
			case(OdDb::kTableBreakEnableBreaking|OdDb::kTableBreakRepeatBottomLabels):
				odPrintConsoleString(L"\nBreaking options are: kTableBreakEnableBreaking and kTableBreakRepeatBottomLabels."); break;
			case(OdDb::kTableBreakEnableBreaking|OdDb::kTableBreakAllowManualPositions):
				odPrintConsoleString(L"\nBreaking options are: kTableBreakEnableBreaking and kTableBreakAllowManualPositions."); break;
			case(OdDb::kTableBreakEnableBreaking|OdDb::kTableBreakAllowManualHeights):
				odPrintConsoleString(L"\nBreaking options are: kTableBreakEnableBreaking and kTableBreakAllowManualHeights."); break;

			case(OdDb::kTableBreakEnableBreaking|OdDb::kTableBreakRepeatTopLabels|OdDb::kTableBreakRepeatBottomLabels):
				odPrintConsoleString(L"\nBreaking options are: kTableBreakEnableBreaking, kTableBreakRepeatTopLabels and kTableBreakRepeatBottomLabels."); break;
			case(OdDb::kTableBreakEnableBreaking|OdDb::kTableBreakRepeatTopLabels|OdDb::kTableBreakAllowManualPositions):
				odPrintConsoleString(L"\nBreaking options are: kTableBreakEnableBreaking, kTableBreakRepeatTopLabels and kTableBreakAllowManualPositions."); break;
			case(OdDb::kTableBreakEnableBreaking|OdDb::kTableBreakRepeatTopLabels|OdDb::kTableBreakAllowManualHeights):
				odPrintConsoleString(L"\nBreaking options are: kTableBreakEnableBreaking, kTableBreakRepeatTopLabels and kTableBreakAllowManualHeights."); break;

			case(OdDb::kTableBreakEnableBreaking|OdDb::kTableBreakRepeatBottomLabels|OdDb::kTableBreakAllowManualPositions):
				odPrintConsoleString(L"\nBreaking options are: kTableBreakEnableBreaking, kTableBreakRepeatBottomLabels and kTableBreakAllowManualPositions."); break;
			case(OdDb::kTableBreakEnableBreaking|OdDb::kTableBreakRepeatBottomLabels|OdDb::kTableBreakAllowManualHeights):
				odPrintConsoleString(L"\nBreaking options are: kTableBreakEnableBreaking, kTableBreakRepeatBottomLabels and kTableBreakAllowManualHeights."); break;

			case(OdDb::kTableBreakEnableBreaking|OdDb::kTableBreakAllowManualPositions|OdDb::kTableBreakAllowManualHeights):
				odPrintConsoleString(L"\nBreaking options are: kTableBreakEnableBreaking, kTableBreakAllowManualPositions and kTableBreakAllowManualHeights."); break;

			case(OdDb::kTableBreakEnableBreaking|OdDb::kTableBreakRepeatTopLabels|OdDb::kTableBreakRepeatBottomLabels|OdDb::kTableBreakAllowManualPositions):
				odPrintConsoleString(L"\nBreaking options are: kTableBreakEnableBreaking, kTableBreakRepeatTopLabels, kTableBreakRepeatBottomLabels and kTableBreakAllowManualPositions."); break;
			case(OdDb::kTableBreakEnableBreaking|OdDb::kTableBreakRepeatTopLabels|OdDb::kTableBreakRepeatBottomLabels|OdDb::kTableBreakAllowManualHeights):
				odPrintConsoleString(L"\nBreaking options are: kTableBreakEnableBreaking, kTableBreakRepeatTopLabels, kTableBreakRepeatBottomLabels and kTableBreakAllowManualHeights."); break;
			case(OdDb::kTableBreakEnableBreaking|OdDb::kTableBreakRepeatBottomLabels|OdDb::kTableBreakAllowManualPositions|OdDb::kTableBreakAllowManualHeights):
				odPrintConsoleString(L"\nBreaking options are: kTableBreakEnableBreaking, kTableBreakRepeatBottomLabels, kTableBreakAllowManualPositions and kTableBreakAllowManualHeights."); break;

			case(OdDb::kTableBreakEnableBreaking|OdDb::kTableBreakRepeatTopLabels|OdDb::kTableBreakRepeatBottomLabels|OdDb::kTableBreakAllowManualPositions|OdDb::kTableBreakAllowManualHeights):
				odPrintConsoleString(L"\nBreaking options are: kTableBreakEnableBreaking, kTableBreakRepeatTopLabels, kTableBreakRepeatBottomLabels, kTableBreakAllowManualPositions and kTableBreakAllowManualHeights."); break;
	
			case(OdDb::kTableBreakRepeatTopLabels|OdDb::kTableBreakRepeatBottomLabels):
				odPrintConsoleString(L"\nBreaking options are: kTableBreakRepeatTopLabels and kTableBreakRepeatBottomLabels."); break;
			case(OdDb::kTableBreakRepeatTopLabels|OdDb::kTableBreakAllowManualPositions):
				odPrintConsoleString(L"\nBreaking options are: kTableBreakRepeatTopLabels and kTableBreakAllowManualPositions."); break;
			case(OdDb::kTableBreakRepeatTopLabels|OdDb::kTableBreakAllowManualHeights):
				odPrintConsoleString(L"\nBreaking options are: kTableBreakRepeatTopLabels and kTableBreakAllowManualHeights."); break;
			case(OdDb::kTableBreakRepeatTopLabels|OdDb::kTableBreakRepeatBottomLabels|OdDb::kTableBreakAllowManualPositions):
				odPrintConsoleString(L"\nBreaking options are: kTableBreakRepeatTopLabels, kTableBreakRepeatBottomLabels and kTableBreakAllowManualPositions."); break;
			case(OdDb::kTableBreakRepeatTopLabels|OdDb::kTableBreakAllowManualPositions|OdDb::kTableBreakAllowManualHeights):
				odPrintConsoleString(L"\nBreaking options are: kTableBreakRepeatTopLabels, kTableBreakAllowManualPositions and kTableBreakAllowManualHeights."); break;
			case(OdDb::kTableBreakRepeatTopLabels|OdDb::kTableBreakRepeatBottomLabels|OdDb::kTableBreakAllowManualPositions|OdDb::kTableBreakAllowManualHeights):
				odPrintConsoleString(L"\nBreaking options are: kTableBreakRepeatTopLabels, kTableBreakRepeatBottomLabels, kTableBreakAllowManualPositions and kTableBreakAllowManualHeights."); break;

			case(OdDb::kTableBreakRepeatBottomLabels|OdDb::kTableBreakAllowManualPositions):
				odPrintConsoleString(L"\nBreaking options are: kTableBreakRepeatBottomLabels and kTableBreakAllowManualPositions."); break;
			case(OdDb::kTableBreakRepeatBottomLabels|OdDb::kTableBreakAllowManualHeights):
				odPrintConsoleString(L"\nBreaking options are: kTableBreakRepeatBottomLabels and kTableBreakAllowManualHeights."); break;
			case(OdDb::kTableBreakRepeatBottomLabels|OdDb::kTableBreakAllowManualPositions|OdDb::kTableBreakAllowManualHeights):
				odPrintConsoleString(L"\nBreaking options are: kTableBreakRepeatBottomLabels, kTableBreakAllowManualPositions and kTableBreakAllowManualHeights."); break;
	
			case(OdDb::kTableBreakAllowManualPositions|OdDb::kTableBreakAllowManualHeights):
				odPrintConsoleString(L"\nBreaking options are: kTableBreakAllowManualPositions and kTableBreakAllowManualHeights."); break;
		}

		// Get break flow direction
		switch(pTable->breakFlowDirection())
		{
			case(OdDb::kTableBreakFlowRight): odPrintConsoleString(L"\nBreak flow direction is kTableBreakFlowRight."); break;
			case(OdDb::kTableBreakFlowDownOrUp): odPrintConsoleString(L"\nBreak flow direction is kTableBreakFlowDownOrUp."); break;
			case(OdDb::kTableBreakFlowLeft): odPrintConsoleString(L"\nBreak flow direction is kTableBreakFlowLeft."); break;
		}

		// Get break spacing
		odPrintConsoleString(L"\nBreak spacing is: %1.2f", pTable->breakSpacing());

		// Get breaks height and offset
		OdGeVector3d offset;
		int cntSubTables = 0;
		while (!OdZero(pTable->breakHeight(cntSubTables))) 
		{
			odPrintConsoleString(L"\nBreak height of %d sub-table is %1.2f", cntSubTables, pTable->breakHeight(cntSubTables));
			cntSubTables++;
		}
		for (int i = 1; i < cntSubTables; i++)
		{
			offset = pTable->breakOffset(i);
			odPrintConsoleString(L"\nBreak offset of %d sub-table is: x = %1.2f, y = %1.2f, z = %1.2f", i, offset.x, offset.y, offset.z);
		}
	}
	else
	{
		odPrintConsoleString(L"\nBreaking mode is not enabled.");
	}
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
		odPrintConsoleString(ODTABLEEX_STR L" sample program. Copyright (c) 2019, Open Design Alliance\n");
		odPrintConsoleString(L"\tusage: " ODTABLEEX_STR L" filename\n");
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
	/* Create the Table                                                   */
	/**********************************************************************/
	// Create object
	OdDbTablePtr pTable = OdDbTable::createObject();
	// Add object into the model space
	OdDbBlockTableRecordPtr pModelSpace = pDb->getModelSpaceId().safeOpenObject(OdDb::kForWrite);
	pModelSpace->appendOdDbEntity(pTable);
	//Applies the default properties of the specified database to Table Object.
	pTable->setDatabaseDefaults(pModelSpace->database());
	
	/**********************************************************************/
	/* Set the parameters                                                 */
	/**********************************************************************/
	// Set the style of the table
	OdDbDictionaryPtr pDict = pDb->getNamedObjectsDictionaryId().safeOpenObject();
    OdDbObjectId tblId = pDict->getAt(OdString(OD_T("ACAD_TABLESTYLE")));
    OdDbDictionaryPtr pTblSt =  tblId.safeOpenObject();
    OdDbObjectId stId = pTblSt->getAt(OD_T("Standard"));
	pTable->setTableStyle(stId);
	// Set 3 columns
	pTable->setNumColumns(3);
	// Set 11 rows
	pTable->setNumRows(11);
	pTable->setCellStyle(2, 2, L"_DATA");
	pTable->generateLayout();
	// Set width of all columns
	pTable->setColumnWidth(3.); 
	// Set height of all rows
	pTable->setRowHeight(1.);
	// Set text style
	pTable->setTextStyle(ttfStyleId);
	// Set text height for all row types in table
	pTable->setTextHeight(0.500*pTable->rowHeight(0), OdDb::kTitleRow);
	pTable->setTextHeight(0.300*pTable->rowHeight(1), OdDb::kHeaderRow);
	pTable->setTextHeight(0.250*pTable->rowHeight(2), OdDb::kDataRow);
	/**********************************************************************/
	/* Set the alignments                                                 */
	/**********************************************************************/
	for (int row = 1; row < (int) pTable->numRows(); row ++)
	{
		for (int col = 0; col < (int) pTable->numColumns(); col ++)
		{
			pTable->setAlignment(row, col, OdDb::kMiddleCenter);
		}
	}
	/**********************************************************************/
	/* Define the title row                                               */
	/**********************************************************************/
	pTable->setCellStyle(0, 0, L"_TITLE");
	pTable->setCellStyle(0, 1, L"_TITLE");
	pTable->setCellStyle(0, 2, L"_TITLE");
	// Merge all cells in first row
	pTable->mergeCells(0, 0, 0, pTable->numColumns() - 1);
	// Set text of the title
	pTable->setTextString(0, 0, OD_T("Title of Table"));
	/**********************************************************************/
	/* Define the header row                                              */
	/**********************************************************************/ 
	pTable->setCellStyle(1, 0, L"_HEADER");
	pTable->setCellStyle(1, 1, L"_HEADER");
	pTable->setCellStyle(1, 2, L"_HEADER");
	pTable->setTextString(1, 0, OD_T("Header0"));
	pTable->setTextString(1, 1, OD_T("Header1"));
	pTable->setTextString(1, 2, OD_T("Header2"));
	/**********************************************************************/
	/* Define data rows													  */
	/**********************************************************************/
	for (int i=2; i<11; i++)
	{
		for (int j=0; j<3; j++)
		{
			pTable->setCellStyle(i,j, L"_DATA");
		}
	}
	/**********************************************************************/
	/* Define the first data row                                          */
	/**********************************************************************/
	pTable->setTextString(2, 0, OD_T("Data"));
	pTable->setTextString(2, 1, OD_T("WIDE COLUMN"));
	pTable->setColumnWidth(1, 5);
	pTable->setTextString(2, 2, OD_T("Data"));
	/**********************************************************************/
	/* Define the second data row to show auto-scaled block references    */
	/**********************************************************************/
	// Insert block reference
	pTable->setBlockTableRecordId(3, 0, odaBlock1Id);
	// Set auto scale
	pTable->setAutoScale(3, 0, true);

	pTable->setTextString(3, 1, OD_T("<-Auto scaled Block Cells->"));

	// Insert block reference
	pTable->setBlockTableRecordId(3, 2, odaBlock1Id);
	// Set auto scale
	pTable->setAutoScale(3, 2, true);
	// Rotate block reference
	pTable->setRotation(3, 2, 0, OdaToRadian(30.0));
	/**********************************************************************/
	/* Define the third data row to show text size                        */
	/**********************************************************************/
	pTable->setTextString(4, 0, OD_T("Small text"));
	pTable-> setTextHeight (4, 0, 0.1);
	pTable->setTextString(4, 1, OD_T("Medium text"));
	pTable-> setTextHeight (4, 1, 0.2);
	pTable->setTextString(4, 2, OD_T("BIG TEXT"));
	pTable-> setTextHeight (4, 2, 0.3);
	/**********************************************************************/
	/* Define the fourth data row to show higher row                      */
	/**********************************************************************/
	// Set new height of row
	pTable->setRowHeight(5, 2.);
	pTable->setTextString(5, 0, OD_T("This row"));
	pTable->setTextString(5, 1, OD_T("is higher"));
	pTable->setTextString(5, 2, OD_T("then other"));
	/**********************************************************************/
	/* Define the fifth data row to show the rotation of text             */
	/**********************************************************************/
	pTable->setTextString(6, 0, OD_T("90 deg."));
	pTable->setRotation(6, 0, 0, OdaToRadian(90.0));
	pTable->setTextString(6, 1, OD_T("180 deg."));
	pTable->setRotation(6, 1, 0, OdaToRadian(180.0));
	pTable->setTextString(6, 2, OD_T("270 deg."));
	pTable->setRotation(6, 2, 0, OdaToRadian(270.0));
	/**********************************************************************/
	/* Define the sixth data row to show scaled bloack cells              */
	/**********************************************************************/
	pTable->setBlockTableRecordId(7, 0, odaBlock1Id);
	pTable->setAutoScale(7, 0, false);
	pTable-> setBlockScale(7, 0, 0.1);
	pTable->setBlockRotation(7, 0, 0.0);

	pTable->setTextString(7, 1, OD_T("<-Scaled Block Cells->"));

	pTable->setBlockTableRecordId(7, 2, odaBlock1Id, true);
	pTable->setAutoScale(7, 2, false);
	pTable-> setBlockScale(7, 2, 0.05);
	pTable->setRotation(7, 2, 0, OdaToRadian(135.0));
	/**********************************************************************/
	/* Define the seventh data row to show alignments                     */
	/**********************************************************************/
	// Set new alignments
	pTable->setAlignment(8, 0, OdDb::kMiddleLeft);
	pTable->setAlignment(8, 1, OdDb::kTopCenter);
	pTable->setAlignment(8, 2, OdDb::kBottomRight);
	// Set text to them
	pTable->setTextString(8, 0, OD_T("Middle left"));
	pTable->setTextString(8, 1, OD_T("Top Center"));
	pTable->setTextString(8, 2, OD_T("Bottom right"));
	/**********************************************************************/
	/* Define the sixth data row to show multiple content cells           */
	/**********************************************************************/
	pTable->setCellType(9,0,OdDb::kMultipleContentCell);
	pTable->setBlockTableRecordId(9, 0, 0, odaBlock1Id, true);
	pTable->setAutoScale(9, 0, 0, true);
	pTable->setTextString(9, 0, 1, OD_T("text"));
	pTable->setTextString(9, 1, OD_T("<-Multiple content->"));
	pTable->setCellType(9,2,OdDb::kMultipleContentCell);
	pTable->setBlockTableRecordId(9, 2, 0, odaBlock1Id, true);
	pTable->setAutoScale(9, 2, 0, false);
	pTable-> setBlockScale(9, 2, 0.05);
	pTable->setTextString(9, 2, 1, OD_T("text"));
	pTable->setRotation(9, 2, 0, OdaToRadian(135.0));
	pTable->setRotation(9, 2, 1, OdaToRadian(25.0));
	/**********************************************************************/
	/* Define the nineth data row  to show operations with gtid lines     */
	/**********************************************************************/
	// Create cell width default grid lines
	pTable->setTextString(10, 0, OD_T("Simple cell"));
	// Create cell with invisible grid lines
	pTable->setGridVisibility(10, 1, OdDb::kHorzBottom, OdDb::kInvisible);
	pTable->setGridVisibility(10, 1, OdDb::kHorzTop, OdDb::kInvisible);
	pTable->setTextString(10, 1, OD_T("Invisible lines"));
	// Create cell with double lines
	pTable->setGridLineStyle(10, 2, OdDb::kHorzBottom, OdDb::kGridLineStyleDouble);
	pTable->setGridLineStyle(10, 2, OdDb::kHorzTop, OdDb::kGridLineStyleDouble);
	pTable->setGridLineStyle(10, 2, OdDb::kVertRight, OdDb::kGridLineStyleDouble);
	// Set spacing of the top grid line
	pTable->setGridDoubleLineSpacing(10, 2, OdDb::kHorzTop, 0.2);
	pTable->setTextString(10, 2, OD_T("Double lines"));

	// Show table properties 
	showStat(pTable);

	/**********************************************************************/
	/*					Work with breaking table						  */
	/**********************************************************************/
	/**********************************************************************/
	/* Add Table Entity													  */
	/**********************************************************************/  
	OdDbTablePtr pBrTable = OdDbTable::createObject();
	// Add table into the model space
	pModelSpace->appendOdDbEntity(pBrTable);
	// Applies the default properties of the specified database to the table
	pBrTable->setDatabaseDefaults(pDb);

	//Set Breaking Table properties
	pBrTable->setTableStyle(stId);
	pBrTable->setNumColumns(2);
	pBrTable->setNumRows(4);
	pBrTable->setColumnWidth(3.0);
	pBrTable->setRowHeight(0.5);
	pBrTable->setPosition(OdGePoint3d(2.0, 5.0, 0.0));
	pBrTable->setTextStyle(ttfStyleId);
	/**********************************************************************/
	/* Define the title row                                               */
	/**********************************************************************/
	pBrTable->setCellStyle(0, 0, L"_TITLE");
	pBrTable->setCellStyle(0, 1, L"_TITLE");
	// Merge all cells in first row
	pBrTable->mergeCells(0, 0, 0, pBrTable->numColumns() - 1);
	// Set text of the title
	pBrTable->setTextString(0, 0, OD_T("Title of Breaking Table"));
	/**********************************************************************/
	/* Set the alignments and cell type                                   */
	/**********************************************************************/
	for (int row = 1; row < (int) pBrTable->numRows(); row ++)
	{
		for (int col = 0; col < (int) pBrTable->numColumns(); col ++)
		{
			pBrTable->setAlignment(row, col, OdDb::kMiddleCenter);
			pBrTable->setCellStyle(row, col, L"_DATA");
		}
	}
	/**********************************************************************/
	/* Set text string					                                  */
	/**********************************************************************/
	pBrTable->setTextString(1, 0, L"DATA 1");
	pBrTable->setTextString(2, 0, L"DATA 2");
	pBrTable->setTextString(3, 0, L"DATA 3");

	// Show default breaking table properties
	showBreakingTableProp(pBrTable);

	/**********************************************************************/
	/* Change breaking properties		                                  */
	/**********************************************************************/
	// Table enables breaks and allows manual position and height for breaks
	pBrTable->enableBreak(true); 
	pBrTable->setBreakOption((OdDb::TableBreakOption(pBrTable->breakOption()|OdDb::kTableBreakAllowManualPositions|OdDb::kTableBreakAllowManualHeights)));
	// Set vertical break flow direction
	pBrTable->setBreakFlowDirection(OdDb::kTableBreakFlowDownOrUp);
	// Set break spacing
	pBrTable->setBreakSpacing(0.75);
	// Set the height of first and second sub-tables
	pBrTable->setBreakHeight(0, 0.5);
	pBrTable->setBreakHeight(1, 1.0);
	pBrTable->setBreakHeight(2, 0.5);
	pBrTable->setBreakHeight(3, 0.5);
	// Change the offset of second and third sub-tables
	pBrTable->setBreakOffset(1, OdGeVector3d(2.0, -1.2, 0.0)); 
	pBrTable->setBreakOffset(2, OdGeVector3d(1.5, -3.0, 0.0)); 

	pBrTable->recomputeTableBlock();
	
	// showBreakingTableProp(pBrTable);
	showBreakingTableProp(pBrTable);

	
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

	if(bOk)
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

