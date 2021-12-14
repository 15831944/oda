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
/* This console application demonstates the OdDbBlockTableRecord object */
/* functionality file and creates a .dwg file filled with               */
/* Block entities.                                                      */
/*                                                                      */
/* Calling sequence:  BlockEx <filename>		                        */
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
#include "DbBlockReference.h"
#include "DbDynBlockReference.h"
#include "DbObjPtrArray.h"
#include "IdArrays.h" 
#include "OdFileBuf.h"
#include "DbArcAlignedText.h"
#include "GeScale3d.h"
#include "DbAttribute.h"
#include "DbAttributeDefinition.h"
#include "XRefMan.h"
#include "DbTable.h"
#include "OdUtilAds.h"

#include "DbAnnotativeObjectPE.h"
#include "DbCircle.h"
#include "DbObjectContextInterface.h"
#include "DbObjectContextManager.h"
#include "DbObjectContextCollection.h"
#include "DbAudit.h"

#include <iostream>
using namespace std;

#define STL_USING_IOSTREAM
#include "OdaSTL.h"
#define  STD(a)  std:: a

#ifdef OD_HAVE_CONSOLE_H_FILE
#include <console.h>
#endif


#define BLOCKEX_STR L"BlockEx"

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
/* Add a block definition to the specified database                     */
/*                                                                      */
/* Note that the BlockTable and BlockTableRecord are implicitly closed  */
/* when before this function returns.                                   */
/************************************************************************/
OdDbObjectId addBlock(OdDbDatabase* pDb, const OdString& name)
{
	OdDbObjectId            id;
	OdDbBlockTablePtr       pTable  = pDb->getBlockTableId().safeOpenObject(OdDb::kForWrite);
	OdDbBlockTableRecordPtr pRecord = OdDbBlockTableRecord::createObject();

	/**********************************************************************/
	/* Block must have a name before adding it to the table.              */
	/**********************************************************************/
	pRecord->setName(name);
 
	/**********************************************************************/
	/* Add the record to the table.                                       */
	/**********************************************************************/
	id = pTable->add(pRecord);
	return id;
}

/************************************************************************/
/* Iterating through Records in Block Table of current Database         */
/*                                                                      */
/* Note that the BlockTable are implicitly closed when before this      */
/* function returns.                                                    */
/************************************************************************/
void printBlocks(OdDbDatabase* pDb)
{
	OdDbBlockTablePtr       pTable  = pDb->getBlockTableId().safeOpenObject(OdDb::kForRead);
	OdDbBlockTableRecordPtr pRecord = OdDbBlockTableRecord::createObject();

	/********************************************************************/
	/* Get a SmartPointer to a new ObjectIterator                       */
	/********************************************************************/
	OdDbSymbolTableIteratorPtr pEntIter = pTable->newIterator();
	/********************************************************************/
	/* Step through the BlockTableRecord                                */
	/********************************************************************/
	for (; !pEntIter->done(); pEntIter->step())
	{
    /********************************************************************/
    /* Dump the Entity                                                  */
    /********************************************************************/
		OdDbBlockTableRecordPtr pEntity = pEntIter->getRecord();
		if(!pEntity->isAnonymous())
			odPrintConsoleString(L"\nBlock name is %s", pEntity->getName().c_str());
	}
}

/********************************************************************/
/* Print Block Table Record properties			                    */
/********************************************************************/
void printBlockTableRecordProperties(const OdDbObjectId& btrId)
{
	//Open the Block Table Record
	OdDbBlockTableRecordPtr pBlockRecord;
	pBlockRecord = btrId.safeOpenObject(OdDb::kForWrite);

	//Block name
	if(!pBlockRecord->isAnonymous())
		odPrintConsoleString(L"\nBlock name is %s", pBlockRecord->getName().c_str());

	//Check Block Type
	odPrintConsoleString(L"\nBlock is %s block", pBlockRecord->isAnonymous() ? L"an anonymous" : L"not anonymous");
	odPrintConsoleString(L"\nBlock is %s block", pBlockRecord->isFromExternalReference() ? L"an external" : L"not external");
	odPrintConsoleString(L"\nBlock is %s block", pBlockRecord->isLayout() ? L"a layout" : L"not layout");
	
	//Layout ID and Layout Name
	OdDbObjectId layuotID = pBlockRecord->getLayoutId();
	odPrintConsoleString(L"\nLayout ID: %d",layuotID);
	if(layuotID)
		odPrintConsoleString(L"\nLayout name: %s", ((OdDbLayoutPtr)(pBlockRecord->getLayoutId()).safeOpenObject(OdDb::kForRead))->getLayoutName().c_str());

	//scaling
	int scaling =  pBlockRecord->blockScaling();
	switch (scaling)
	{
		case 0:
			odPrintConsoleString(L"\nBlock references can be non-uniformly scaled");
			break;
		case 1:
			odPrintConsoleString(L"\nBlock references can be scaled only uniformly");
			break;
	}

	//units
	OdDb::UnitsValue units =  pBlockRecord->blockInsertUnits();
  OdString strOut(L"\nUnits: ");
  strOut += oddbGetUnitsName(units);
	odPrintConsoleString(strOut);

	//origin
	try 
	{
		OdGePoint3d blockOrigin = pBlockRecord->origin();
		odPrintConsoleString(L"\nOrigin: x = %g, y = %g, z = %g", blockOrigin.x, blockOrigin.y, blockOrigin.z);
	}
	catch(...)
	{
		odPrintConsoleString(L"\nThe Dafault value of Origin is not set");
	}
	
	//comments
	odPrintConsoleString(L"\nComments: %s", pBlockRecord->comments().c_str());

	//explodable
	odPrintConsoleString(L"\nThe block is %s", pBlockRecord->explodable() ? (L"explodable") : (L"not explodable"));

	//attribute definitions
	odPrintConsoleString(L"\nThe block has %s", pBlockRecord->hasAttributeDefinitions() ? (L"attribute definitions") : (L"no attribute definitions"));

	//DrawOrderTable
	try
	{
		OdDbSortentsTablePtr stp = pBlockRecord->getSortentsTable();
		odPrintConsoleString(L"\nSortents Table ID: %d", stp);
	}
	catch(...) 
	{ 
		odPrintConsoleString(L"\nThere are no entities in the Current Block");	
	}
}

/********************************************************************/
/* Add an Arc to the Specified Block Table Record		            */
/********************************************************************/
void addArcToBlockAndDb(OdDbDatabase* pDb, OdDbObjectId& btrId)
{
	// Create Arc Object 
	OdDbArcPtr pArc = OdDbArc::createObject();
	//Applies the default properties of the specified database to Arc Object.
	pArc->setDatabaseDefaults(pDb);
	//Open block
	OdDbBlockTableRecordPtr pBlockRecord = btrId.safeOpenObject(OdDb::kForWrite);
	// Add Arc Object in the block
	pBlockRecord->appendOdDbEntity(pArc);
	// Set Arc Object properties    
	OdGePoint3d center = OdGePoint3d(5.0, 3.0, 0.0);
	pArc->setCenter(center);
	pArc->setRadius(5.0);
	pArc->setStartAngle(OdaToRadian(75.0));
	pArc->setEndAngle(OdaToRadian(165.0));
}

/********************************************************************/
/* Moving entities between blocks			    		            */
/********************************************************************/
void movingEntitiesBetweenBlocks(const OdDbObjectId& srcBtrId, OdDbObjectId& destBtrId)
{
	OdDbObjectIdArray entCollection;
	//Open source block
	OdDbBlockTableRecordPtr pSrcBlockRecord;
	pSrcBlockRecord = srcBtrId.safeOpenObject(OdDb::kForRead);

	OdDbObjectIteratorPtr pEntIter = pSrcBlockRecord->newIterator();
	for (; !pEntIter->done(); pEntIter->step())
	{
    /********************************************************************/
    /* Dump the Entity                                                  */
    /********************************************************************/
		entCollection.append(pEntIter->entity()->objectId());
	}
	//Open destination block
	OdDbBlockTableRecordPtr pDestBlockRecord;
	pDestBlockRecord = destBtrId.safeOpenObject(OdDb::kForWrite);
	//Move the collection of entities entCollection to block pDestBlockRecord
	pDestBlockRecord->assumeOwnershipOf(entCollection);
}

/********************************************************************/
/* Test the work with External Blocks		    		            */
/********************************************************************/
void testExternalBlock(OdDbDatabase* pDb, const OdString& fileName, const OdString& blockName)
{
	//Create an External block
	OdDbBlockTableRecordPtr pXRef = OdDbXRefManExt::addNewXRefDefBlock(pDb, fileName, blockName , false);
	//Load the database of xRef
	OdResult res = OdDbXRefMan::load(pDb, blockName);
	OdDbDatabasePtr xDb = pXRef->xrefDatabase();
	if(!xDb.isNull())
	{
		odPrintConsoleString(L"\n\nThe List of Blocks in External Database:");
		printBlocks(xDb);
		//Unload the database of xRef
		OdDbXRefMan::unload(pXRef);
		//Delete the database before uninitialization
		xDb.release();
	}
	else 
		odPrintConsoleString(L"\nThe Pointer to the External Database is NULL");
}

/************************************************************************/
/* Add a block reference to the specified BlockTableRecord              */
/************************************************************************/
OdDbObjectId addBlockReference(OdDbObjectId& bBTR,
                                 const OdDbObjectId& btrId)
{
	OdDbObjectId brefId;
	OdDbBlockTableRecordPtr pBlock = bBTR.safeOpenObject(OdDb::kForWrite);

	/**********************************************************************/
	/* Add the block reference to the BlockTableRecord                    */
	/**********************************************************************/
	OdDbBlockReferencePtr pBlkRef = OdDbBlockReference::createObject();
	pBlkRef->setDatabaseDefaults(pBlock->database());
	brefId = pBlock->appendOdDbEntity(pBlkRef);

	/**********************************************************************/
	/* Set some properties                                                */
	/**********************************************************************/
	pBlkRef->setBlockTableRecord(btrId);
 
	return brefId;
}

/********************************************************************/
/* Print Block Reference properties				                    */
/********************************************************************/
void printBlockReferenceProperties(const OdDbObjectId bRefId)
{
	//Open Block Reference Object
	OdDbBlockReferencePtr pBRef = bRefId.safeOpenObject(OdDb::kForRead);
	
	//scaleFactors
	OdGeScale3d sFactors = pBRef->scaleFactors();
	odPrintConsoleString(L"\nScale Factors: x = %1.2f, y = %1.2f, z = %1.2f", sFactors.sx, sFactors.sy, sFactors.sz);
	
	//normal
	OdGeVector3d bRefNormal = pBRef->normal();
	odPrintConsoleString(L"\nNormale: x = %1.2f, y = %1.2f, z = %1.2f", bRefNormal.x, bRefNormal.y, bRefNormal.z);

	//position
	OdGePoint3d position = pBRef->position();
	odPrintConsoleString(L"\nPosition: x = %1.2f, y = %1.2f, z = %1.2f", position.x, position.y, position.z);

	//rotation angle
	odPrintConsoleString(L"\nRotation: %1.2f", pBRef->rotation());

	//blockTransform
	OdGeMatrix3d blockTransform = pBRef->blockTransform();

	odPrintConsoleString(L"\nTransformation Matrix:");
	double i;
	for(int row = 0; row < 3; row++)
	{
		odPrintConsoleString(L"\n");
		for(int col = 0; col < 3; col++)
		{
			i = blockTransform(row, col);
			odPrintConsoleString(L"%1.2f \t", i);
		}
	}	
	//open attribute
	OdDbObjectIteratorPtr pEntIter = pBRef->attributeIterator();
	OdDbAttributePtr openAttr;
	for (; !pEntIter->done(); pEntIter->step())
	{
		/********************************************************************/
		/* Dump the Entity                                                  */
		/********************************************************************/
		openAttr = pEntIter->entity()->objectId().safeOpenObject(OdDb::kForRead);
		odPrintConsoleString(L"\nAttribute Text String: %s", openAttr->textString().c_str());
	}	
	//block extents
	OdGeExtents3d extents;
	pBRef->geomExtentsBestFit(extents, OdGeMatrix3d::kIdentity);
	OdGePoint3d minPoint = extents.minPoint();
	OdGePoint3d maxPoint = extents.maxPoint();
	odPrintConsoleString(L"\nThe extents box points are: minimum x=%g, y=%g, z=%1.3f; maximum x=%1.3f, y=%1.3f, z=%1.3f", minPoint.x,minPoint.y, minPoint.z,  maxPoint.x, maxPoint.y, maxPoint.z);

}

/********************************************************************/
/* Print Attribute Definition properties				            */
/********************************************************************/
void printAttributeDefinitionProperties(const OdDbObjectId attDefId)
{
	//Open Attribute Definition Object
	OdDbAttributeDefinitionPtr pAttrDef = attDefId.safeOpenObject(OdDb::kForRead);
	
	odPrintConsoleString(L"\nTag: %s", pAttrDef->tag().c_str());
	odPrintConsoleString(L"\nPrompt: %s", pAttrDef->prompt().c_str());
	odPrintConsoleString(L"\nThe attribute definition is: %s", pAttrDef->isConstant()? (L"constant") : (L"no constant"));
	odPrintConsoleString(L"\nText string: %s", pAttrDef->textString().c_str());
	odPrintConsoleString(L"\nField length: %d", pAttrDef->fieldLength());
	odPrintConsoleString(L"\nThe attribute definition is %s", pAttrDef->isInvisible()? (L"invisible") : (L"not invisible"));
	odPrintConsoleString(L"\nThe attribute definition is %s", pAttrDef->isMTextAttributeDefinition()? (L"a multiline attribute") : (L"not a multiline attribute"));
	
	OdDbMTextPtr mText = pAttrDef->getMTextAttributeDefinition();
	if(!mText.isNull())
		odPrintConsoleString(L"\nMultiline Text is %s", mText->contents().c_str());

	odPrintConsoleString(L"\nThe attribute position is %s", pAttrDef->lockPositionInBlock()? (L"locked") : (L"not locked"));
	odPrintConsoleString(L"\nThe attribute definition is %s", pAttrDef->isPreset()? (L"a preset") : (L"not a preset"));
	odPrintConsoleString(L"\nThe attribute is %s", pAttrDef->isVerifiable()? (L"set to verified") : (L"not set to verified"));
}

/********************************************************************/
/* Print Attribute properties							            */
/********************************************************************/
void printBlockRefAttributeProperties(const OdDbObjectId attId)
{
	//Open Attribute Object
	OdDbAttributePtr pAttr = attId.safeOpenObject(OdDb::kForRead);
	
	odPrintConsoleString(L"\nTag: %s", pAttr->tag().c_str());
	odPrintConsoleString(L"\nThe attribute is: %s", pAttr->isConstant()? (L"constant") : (L"no constant"));
	odPrintConsoleString(L"\nText string: %s", pAttr->textString().c_str());
	odPrintConsoleString(L"\nField length: %d", pAttr->fieldLength());
	odPrintConsoleString(L"\nThe attribute is %s", pAttr->isInvisible()? (L"invisible") : (L"not invisible"));
	odPrintConsoleString(L"\nThe attribute is %s", pAttr->isMTextAttribute()? (L"a multiline attribute") : (L"not a multiline attribute"));
	odPrintConsoleString(L"\nThe attribute position is %s", pAttr->lockPositionInBlock()? (L"locked") : (L"not locked"));
	odPrintConsoleString(L"\nThe attribute is %s", pAttr->isPreset()? (L"a preset") : (L"not a preset"));
	odPrintConsoleString(L"\nThe attribute is %s", pAttr->isVerifiable()? (L"set to verified") : (L"not set to verified"));
		
	OdDbMTextPtr mText = pAttr->getMTextAttribute();
	if(!mText.isNull())
		odPrintConsoleString(L"\nMultiline Text is %s", mText->contents().c_str());

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
		odPrintConsoleString(BLOCKEX_STR L" sample program. Copyright (c) 2019, Open Design Alliance\n");
		odPrintConsoleString(L"\tusage: " BLOCKEX_STR L" filename\n");
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
	OdDbDatabasePtr pDb = svcs.createDatabase(true, OdDb::kMetric);

	/**********************************************************************/
	/* Add a TTF text style                                               */
	/**********************************************************************/
	OdDbObjectId ttfStyleId = addStyle(pDb, OD_T("OdaTtfStyle"), 0.0, 1.0, 0.2, 0.0, 
      OD_T("VERDANA.TTF"), false, OD_T("Verdana"), false, false, 0, 34);

	/**********************************************************************/
	/*                Working with Block Definition    					  */ 
	/**********************************************************************/
	
	/**********************************************************************/
	/* Add a Block Definition to the specified database	                  */
	/**********************************************************************/
	OdDbObjectId blockId = addBlock(pDb, "current NewBlock");
	
	/**********************************************************************/
	/* Show Default Values of Properties of Block Definition              */
	/**********************************************************************/
	odPrintConsoleString(L"\n\tDefault Properties Values of Block Definition");
	printBlockTableRecordProperties(blockId);

	/**********************************************************************/
	/* Edit Block Definition Properties                                   */
	/**********************************************************************/
	//Open the Block Table Record 
	OdDbBlockTableRecordPtr pBlockRecord = blockId.safeOpenObject(OdDb::kForWrite);	
	//scaling
	pBlockRecord->setBlockScaling(OdDbBlockTableRecord::BlockScaling::kUniform);	
	//units
	pBlockRecord->setBlockInsertUnits(OdDb::kUnitsKilometers);	
	//origin
	pBlockRecord->setOrigin(OdGePoint3d(5.0, 3.0, 0.0));	
	//comments
	pBlockRecord->setComments("Block comments");	
	//explodable
	pBlockRecord->setExplodable(false);
	
	/**********************************************************************/
	/* Show Changed Values of Proterties of Block Definition              */
	/**********************************************************************/
	odPrintConsoleString(L"\n\n\tChanged Properties Values of Block Definition");
	printBlockTableRecordProperties(pBlockRecord->objectId());

	/**********************************************************************/
	/* Add an Arc to the DB and Block                                     */
	/**********************************************************************/
	addArcToBlockAndDb(pDb, pBlockRecord->objectId());
	
	/**********************************************************************/
	/*Moving entities between blocks	            					  */ 
	/**********************************************************************/
	movingEntitiesBetweenBlocks(pBlockRecord->objectId(), pDb->getModelSpaceId());
	
	/**********************************************************************/
	/*                Working with External Blocks     					  */ 
	/**********************************************************************/
	//Print Current Database Blocks
	odPrintConsoleString(L"\n\nThe List of Current Database Blocks:");
	printBlocks(pDb);

	testExternalBlock(pDb, OD_T("D:\\test_xref.dwg"), OD_T("XRefBlock"));
	
	//Print Current Database Blocks with External Block
	odPrintConsoleString(L"\n\nThe List of Current Database Blocks with added External Block:");
	printBlocks(pDb);

	/**********************************************************************/
	/*                Working with Block Reference						  */ 
	/**********************************************************************/
	//Create a Circle entity and add it to the Referenced Block
	OdDbCirclePtr pNewCircle = OdDbCircle::createObject();
	pNewCircle->setDatabaseDefaults(pDb);
	pNewCircle->setRadius(4.0);
	pNewCircle->setCenter(OdGePoint3d(6.5, 5.0, 0));
	//Add the Circle entity to the Block
	pBlockRecord->appendOdDbEntity(pNewCircle);
	
	//make sure that the Referenced Block is Explodable
	pBlockRecord->setExplodable(true);
	
	OdDbObjectId bRefId = addBlockReference(pDb->getModelSpaceId(), pBlockRecord->objectId());

	/**********************************************************************/
	/* Show Default Values of Block Reference Properties	              */
	/**********************************************************************/
	odPrintConsoleString(L"\n\tDefault Properties Values of Block Reference");
	printBlockReferenceProperties(bRefId);

	/**********************************************************************/
	/* Edit Block Reference Properties                                    */
	/**********************************************************************/
	//Open the Block Reference object 
	OdDbBlockReferencePtr pBRef = bRefId.safeOpenObject(OdDb::kForWrite);
	pBRef->objectId().safeOpenObject(OdDb::kForWrite);	
	//scaleFactors
	/*If the Block Scaling property of referenced BlockRecord is set to kUniform value, 
	use only the one value for X, Y and Z scale factors. In the other case the values of X, 
	Y and Z scale factors may be different*/
	pBlockRecord->setBlockScaling(OdDbBlockTableRecord::BlockScaling::kAny);
	OdGeScale3d sc = OdGeScale3d(2.0, 2.0, 2.0);
	pBRef->setScaleFactors(sc);
	//normal
	pBRef->setNormal(OdGeVector3d(2.0, 0.0, 1.0));
	//position
	pBRef->setPosition(OdGePoint3d(15, 7, 0));
	//rotation angle
	pBRef->setRotation(OdaToRadian(45.0));
	//blockTransform
	OdGeMatrix3d blockTransform = pBRef->blockTransform();
	//append attribute
	OdDbAttributePtr attr = OdDbAttribute::createObject();
	attr->setTextString("Block Reference Attribute");
	pBRef->appendAttribute(attr);

	/**********************************************************************/
	/* Show Changed Values of Properties of Block Reference               */
	/**********************************************************************/
	odPrintConsoleString(L"\n\n\tChanged Properties Values of Block Definition");
	printBlockReferenceProperties(pBRef->objectId());
	
	/**********************************************************************/
	/* Explode to Owner Space							                  */
	/**********************************************************************/
	pBRef->explodeToOwnerSpace();

	/**********************************************************************/
	/*Make Block Reference scalable										  */ 
	/**********************************************************************/
	//Make the Block itself annotative
	OdDbAnnotativeObjectPEPtr(pBlockRecord)->setAnnotative(pBlockRecord, true);
	//Make the Block Reference annotative
	OdDbAnnotativeObjectPEPtr(pBRef)->setAnnotative(pBRef, true);
	//Add supported annotation scale context to the annotative Block Reference
	OdDbObjectContextPtr ctx = pBlockRecord->database()->objectContextManager()->contextCollection("ACDB_ANNOTATIONSCALES")->getContext(OdString(L"8:1"));
	OdDbObjectContextInterfacePtr(pBRef)->addContext(pBRef, *ctx);
	ctx = pBlockRecord->database()->objectContextManager()->contextCollection("ACDB_ANNOTATIONSCALES")->getContext(OdString(L"1:2"));
	OdDbObjectContextInterfacePtr(pBRef)->addContext(pBRef, *ctx);
	ctx = pBlockRecord->database()->objectContextManager()->contextCollection("ACDB_ANNOTATIONSCALES")->getContext(OdString(L"1:20"));
	OdDbObjectContextInterfacePtr(pBRef)->addContext(pBRef, *ctx);

	
	/**********************************************************************/
	/*                Working with Attribute Definition					  */ 
	/**********************************************************************/
	//Create Attribute Definition object
    OdDbAttributeDefinitionPtr pAttrDef = OdDbAttributeDefinition::createObject();
    //Set the default database
	pAttrDef->setDatabaseDefaults(pDb);
	//Add Attribute Definition to the specified Block
    pBlockRecord->appendOdDbEntity(pAttrDef);
	
	/**********************************************************************/
	/* Show Dafault Values of Attribute Definition Proterties	          */
	/**********************************************************************/
	odPrintConsoleString(L"\n\n\tDefault Properties Values of Attribute Definition");
	printAttributeDefinitionProperties(pAttrDef->objectId());
	
	/**********************************************************************/
	/* Edit Attribute Definition Properties                               */
	/**********************************************************************/
	//tag
	pAttrDef->setTag("TYPE");	
	//prompt
	pAttrDef->setPrompt("Input type");	
	//constant
	pAttrDef->setConstant(true);	
	//textString
	pAttrDef->setTextString("DRUM");	
	//FieldLength
	pAttrDef->setFieldLength(50);	
	//invisible
	pAttrDef->setInvisible(true);	
	//MTextAttributeDefinition
	pAttrDef->convertIntoMTextAttributeDefinition(false);
	
	/**********************************************************************/
	/* Create MText				                                          */
	/**********************************************************************/
	OdDbMTextPtr mText = OdDbMText::createObject();
	mText->setDatabaseDefaults(pDb);
	pBlockRecord->appendOdDbEntity(mText);
	mText->setRotation(OdaToRadian(225));
	mText->setTextStyle(ttfStyleId);
	mText->setContents(OD_T("\\T 2; \\W 3; Attribute definition\\P " + mText->colorChange() + "2; text"));
	pAttrDef->setMTextAttributeDefinition(mText);	
	
	//lock position in block
	pAttrDef->setLockPositionInBlock(true);
	//preset status
	pAttrDef->setPreset(true);
	//verifiable status
	pAttrDef->setVerifiable(true);
	
	/**********************************************************************/
	/* Show Changed Values of Proterties of Attribute Definition          */
	/**********************************************************************/
	odPrintConsoleString(L"\n\n\tChanged Properties Values of Attribute Definition");
	printAttributeDefinitionProperties(pAttrDef->objectId());


	/**********************************************************************/
	/*            Working with Block Reference Attribute 				  */ 
	/**********************************************************************/
	//Create Attribute object
	OdDbAttributePtr pBlockRefAtt = OdDbAttribute::createObject();
    //Set the default database
	pBlockRefAtt->setDatabaseDefaults(pDb);
	//Add Attribute to the specified Block Reference
	pBRef->appendAttribute(pBlockRefAtt);
	
	/**********************************************************************/
	/* Show Dafault Values of Block Reference Attributes		          */
	/**********************************************************************/
	odPrintConsoleString(L"\n\n\tDefault Properties Values of Block Reference Attributes");
	printBlockRefAttributeProperties(pBlockRefAtt->objectId());
	/**********************************************************************/
	/* Edit Block Reference Attribute Properties                          */
	/**********************************************************************/
	//Update multiline text
	pBlockRefAtt->updateMTextAttribute();	
	//set tag
	pBlockRefAtt->setTag(L"TYPE");	
	//invisible status
	pBlockRefAtt->setInvisible(true);
	//test string
	pBlockRefAtt->setTextString("DRUM");
	//field length
	pBlockRefAtt->setFieldLength(20);
	//multiline attribute support
	pBlockRefAtt->convertIntoMTextAttribute(true);
	
	/**********************************************************************/
	/* Create MText				                                          */
	/**********************************************************************/
	OdDbMTextPtr mText2 = OdDbMText::createObject();
	mText2->setDatabaseDefaults(pDb);
	pBlockRecord->appendOdDbEntity(mText2);
	mText2->setRotation(OdaToRadian(-180));
	mText2->setHorizontalMode(OdDb::kTextCenter);
	mText2->setTextStyle(ttfStyleId);
	mText2->setContents(OD_T("\\T 3; \\W 1; Attribute\\P " + mText2->colorChange() + "1; text"));
	pBlockRefAtt->setMTextAttribute(mText2);	

	//lock position
	pBlockRefAtt->setLockPositionInBlock(true);

	/**********************************************************************/
	/* Show Changed Values of Block Reference Attributes		          */
	/**********************************************************************/
	odPrintConsoleString(L"\n\n\tDefault Properties Values of Block Reference Attributes");
	printBlockRefAttributeProperties(pBlockRefAtt->objectId());

	/**********************************************************************/
	/* Save Block's Objects in .dwg file								  */
	/**********************************************************************/
	OdDb::SaveType fileType = OdDb::kDwg;
	OdDb::DwgVersion outVer = OdDb::vAC24;
	OdString f1(argv[1]); 
    OdWrFileBuf fb1(f1.c_str());
	
	pDb->writeFile(&fb1, fileType, outVer, true /*save preview */);
    odPrintConsoleString(L"\nDatabase has been saved as %ls", pDb->getFilename().c_str());

	
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

