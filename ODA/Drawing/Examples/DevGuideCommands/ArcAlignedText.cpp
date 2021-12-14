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
/* ARCALLIGNEDTEXT command demonstates the OdArcAlignedText entity      */
/* functionality and populates database with ArcAlignedText entities.   */
/************************************************************************/

#include "StdAfx.h"
#include "DbArc.h"
#include "DbArcAlignedText.h"

/********************************************************************/
/* Print specific Arc Aligned Text properties.	                    */
/********************************************************************/
void printArcAlignedTextProperties(OdDbArcAlignedText* pArcText, OdDbUserIO* pIO)
{
  OdString strMessage;
  strMessage.format(L"\n  Text String = %s", pArcText->textString().c_str());
  pIO->putString(strMessage);

  strMessage.format(L"  xScale = %1.2f", pArcText->xScale());
  pIO->putString(strMessage);

  strMessage.format(L"  Text Size = %1.2f", pArcText->textSize());
  pIO->putString(strMessage);

  strMessage.format(L"  Offset from Arc = %1.2f", pArcText->offsetFromArc());
  pIO->putString(strMessage);

  strMessage.format(L"  Offset from Arc = %1.2f", pArcText->offsetFromArc());
  pIO->putString(strMessage);

  strMessage.format(L"  Char Spacing = %1.2f", pArcText->charSpacing());
  pIO->putString(strMessage);

	OdGePoint3d center = pArcText->center();
  strMessage.format(L"  Center = (%g,%g,%g)", center.x, center.y, center.z);
  pIO->putString(strMessage);

  strMessage.format(L"  Radius = %1.2f", pArcText->radius());
  pIO->putString(strMessage);

  strMessage.format(L"  Start Angle = %1.2f", OdaToDegree(pArcText->startAngle()));
  pIO->putString(strMessage);

  strMessage.format(L"  End Angle = %1.2f", OdaToDegree(pArcText->endAngle()));
  pIO->putString(strMessage);

  if(pArcText->isUnderlined())
		pIO->putString(L"  Text Underlined = true");
	else
		pIO->putString(L"  Text Underlined = false");

	int alignment = pArcText->alignment();
	switch (alignment)
	{
	  case kRight:
		  pIO->putString(L"  Text Alignment = Right");
		  break;
	  case kCenter:
		  pIO->putString(L"  Text Alignment = Center");
		  break;
	  case kFit:
		  pIO->putString(L"  Text Alignment = Fit");
		  break;
	  case kLeft:
		  pIO->putString(L"  Text Alignment = Left");
		  break;
	}

	int position = pArcText->textPosition();
	if (position == kOnConcaveSide)
		pIO->putString(L"  Text Position = On Concave Side");
	else
		pIO->putString(L"  Text Position = On Convex Side");

	int direction = pArcText->textDirection();
	if(direction == kInwardToTheCenter)
		pIO->putString(L"  Text Direction = To the Center");
	else
		pIO->putString(L"  Text Direction = From Center");

	if(pArcText->isReversedCharOrder())
		pIO->putString(L"  Current char order mode = reversed");
	else
		pIO->putString(L"  Current char order mode = not reversed");

	OdCmColor color = pArcText->color();
  strMessage.format(L"  Text Color in RGB Format = %s", color.colorNameForDisplay().c_str());
  pIO->putString(strMessage);

	OdString typeFace;
	bool bold, italic;
	int charset, pitchAndFamily;
	pArcText->font(typeFace, bold, italic, charset, pitchAndFamily);
  strMessage.format(L"  Text Type Face = %s", typeFace.c_str());
  pIO->putString(strMessage);

	if(bold)
		pIO->putString(L"  Text Bold = true");
	else
		pIO->putString(L"  Text Bold = false");
	if(italic)
		pIO->putString(L"  Text Italic = true");
	else
		pIO->putString(L"  Text Italic =  false");

  strMessage.format(L"  Text Charset = %d", charset);
  pIO->putString(strMessage);

  strMessage.format(L"  Text Pitch and Family = %d \n", pitchAndFamily);
  pIO->putString(strMessage);
}


void _ArcAlignedText_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();

  /**********************************************************************/
  /* Create the pointer to current active space                         */
  /**********************************************************************/
  OdDbBlockTableRecordPtr pBTR = DevGuideCommandsUtils::activeBlockTableRecord(pDb).safeOpenObject(OdDb::kForWrite);


  /**********************************************************************/
	/* Add an Arc                                                         */
	/**********************************************************************/
	// Create Arc Object
	OdDbArcPtr pArc = OdDbArc::createObject();
	//Applies the default properties of the specified database to Arc Object.
	pArc->setDatabaseDefaults(pDb);
	pBTR->appendOdDbEntity(pArc);
	
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
	// Add Arc Text entity to current space. Arc Aligned Text must be added 
	// to a database before connecting with Arc object.
	pBTR->appendOdDbEntity(pArcText);
	// Applies the default properties of the specified database to Arc Text Object
	pArcText->setDatabaseDefaults(pDb);
	// Connect Arc Text Object with Arc Object
	pArcText->setArcId(pArc->objectId());
	// Set text string
	pArcText->setTextString(L"Arc text");

	/**********************************************************************/
	/* Add an Arc with changed position to demonstrate                    */
	/* the changed properties of Arc Aligned Text           						  */	
	/**********************************************************************/
	// Create Arc Object
	OdDbArcPtr pArc2 = OdDbArc::createObject();
	// Add Arc Object to current space
	pBTR->appendOdDbEntity(pArc2);
	//Applies the default properties of the specified database to Arc Object.
	pArc2->setDatabaseDefaults(pDb);
	
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
	pBTR->appendOdDbEntity(pArcText2);
	// Applies the default properties of the specified database to Arc Text Object
	pArcText2->setDatabaseDefaults(pDb);
	// Connect Arc Text Object with Arc Object
	pArcText2->setArcId(pArc2->objectId());
	// Set text string
	pArcText2->setTextString(L"Text");
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
	// Set the underlined
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
  	 
	pIO->putString(L"\n  ------ Default Values of Specific Arc-Aligned Text Properties");
	printArcAlignedTextProperties(pArcText, pIO);
	
	/**********************************************************************/
	/* Show changed values of specific properties of Arc Text Object      */
	/**********************************************************************/
	pIO->putString(L"\n  ------ Specific Arc-Aligned Text Properties");
	printArcAlignedTextProperties(pArcText2, pIO);
}
