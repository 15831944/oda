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

#include "OdaCommon.h"
#include "DbProxyEntity.h"
#include "Ge/GeCircArc3d.h"
#include "Gi/GiWorldDraw.h"		// need for OdGiWorldDraw class
#include "DbFiler.h"			// need for OdDbDwgFiler class and OdDbDxfFiler class
#include "SmileyDb.h"
#include "DbAudit.h"
//
// Step 1
// We are implementing 'worldDraw' function to draw the smiley object,
// 'dwgOutFields' function to save the smiley object in DWG file,
// 'dwgInFields' function to load the smiley object from DWG file.
// These functions must be implemented for a minimal Entity object.
//
const double kCurrentVersionNumber = 1.0;

#ifdef SMILEY_WITH_TEXT
const OdChar AsdkSmiley::mTextFieldValue[] = { OD_T("Text Sample") };
#endif

bool AsdkSmiley::subWorldDraw(OdGiWorldDraw *wd) const			// Drawing function
{
    assertReadEnabled();
    OdGeVector3d offset(0,0,0);
    // If dragging, don't fill the smiley
    if( wd->isDragging() ){
		wd->subEntityTraits().setTrueColor(entityColor());  // Returns a 'True' true color.
        wd->subEntityTraits().setFillType( kOdGiFillNever );
    }
    else
        wd->subEntityTraits().setFillType( kOdGiFillAlways );

    // Drawing circle of face
    wd->subEntityTraits().setSelectionMarker( kFaceMark ); // 1 == kFaceMark
    wd->geometry().circle( center(), radius(), mnormal );

    if( !wd->isDragging() )
		wd->subEntityTraits().setTrueColor(mbackcolor);		// Set the background true color.

    // Drawing circle of left eye
    wd->subEntityTraits().setSelectionMarker( kLeftEyeMark ); // 2 == kLeftEyeMark
	wd->geometry().circle( leftEyeCenter(), meyesize, mnormal );

    // Drawing circle of right eye
    wd->subEntityTraits().setSelectionMarker( kRightEyeMark ); // 3 == kRightEyeMark
	wd->geometry().circle( rightEyeCenter(), meyesize, mnormal );

    // Drawing arc of mouth
	OdGePoint3d smilecen( mouthCenter() + offset ),
                startpt( mouthLeft() + offset ),
                endpt( mouthRight() + offset );
    OdGeVector3d startvec = startpt - smilecen,
                 endvec = endpt - smilecen;
    double mouthangle = startvec.angleTo( endvec );
															// Give the eyes GS markers of 4.
    wd->subEntityTraits().setSelectionMarker( kMouthMark ); // 4 == kMouthMark
    wd->geometry().circularArc( smilecen, mouthRadius(), mnormal, startvec, mouthangle, kOdGiArcChord );

#ifdef SMILEY_WITH_TEXT
    wd->subEntityTraits().setSelectionMarker( 5 );
    wd->subEntityTraits().setTrueColor(OdCmEntityColor(255,0,0));// Set the background true color.
    wd->geometry().text(smilecen+startvec, mnormal, -startvec, mouthRadius()/2., mouthRadius()/5., 0, mTextField);
#endif
 
    return true;
}
// Function of the saving in DWG file
//
void AsdkSmiley::dwgOutFields(OdDbDwgFiler* filer) const
{
  assertReadEnabled();
  OdDbEntity::dwgOutFields( filer );			// Method of base class must be perfomed first

  filer->wrDouble( kCurrentVersionNumber );
  filer->wrPoint3d( center() );
  filer->wrDouble( radius() );
  filer->wrVector3d( normal() );
  filer->wrDouble( eyesApart() );
  filer->wrDouble( eyesHeight() );
  filer->wrDouble( eyeSize() );

#ifdef SMILEY_WITH_TEXT
  filer->wrString(mTextField);
#endif

  filer->wrPoint3d( mouthLeft() );
  filer->wrPoint3d( mouthBottom() );
  filer->wrPoint3d( mouthRight() );
  filer->wrInt32( backColor().color() );
}
// Function of the loading from DWG file
//
OdResult AsdkSmiley::dwgInFields(OdDbDwgFiler* filer)
{
    assertWriteEnabled();
    OdDbEntity::dwgInFields( filer );			// Method of base class must be perfomed first

    // Read version number
    if ( filer->rdDouble() > kCurrentVersionNumber ) return eMakeMeProxy;
	// Read face data
    setCenter( filer->rdPoint3d() );
    setRadius( filer->rdDouble() );
    setNormal( filer->rdVector3d() );

	// Read eyes data
    setEyesApart( filer->rdDouble() );
    setEyesHeight( filer->rdDouble() );
    setEyeSize( filer->rdDouble() );

  // Read text data
#ifdef SMILEY_WITH_TEXT
    mTextField = filer->rdString();
#endif
    
  // Read mouth data
	OdGePoint3d mouthleftpt=filer->rdPoint3d(),
                mouthbottompt=filer->rdPoint3d(),
                mouthrightpt=filer->rdPoint3d();
	
	setMouth( mouthleftpt, mouthbottompt, mouthrightpt );
	// Read color data
	mbackcolor.setColor(filer->rdInt32());
    recordGraphicsModified();    	
    return eOk;
}
//
// Step 2
// We are implementing 'dxfOutFields' function to save the smiley object in DXF file,
// and 'dxfInFields' function to load the smiley object from DXF file.
// It is desirable to implement these function for Entity object.
//
void AsdkSmiley::dxfOutFields(OdDbDxfFiler *filer) const
{
    assertReadEnabled();
    OdDbEntity::dxfOutFields( filer );			// Method of base class must be perfomed first

	filer->wrSubclassMarker( desc()->name() );
    filer->wrDouble( OdResBuf::kDxfReal, kCurrentVersionNumber );
    filer->wrPoint3d( OdResBuf::kDxfXCoord, center() );
    filer->wrDouble( OdResBuf::kDxfReal+1, radius() );
    if( filer->includesDefaultValues()
        || normal() != OdGeVector3d( 0, 0, 1 ))
    {
        filer->wrVector3d( OdResBuf::kDxfNormalX, normal() );
    }
    filer->wrDouble( OdResBuf::kDxfReal+2, eyesApart() );
    filer->wrDouble( OdResBuf::kDxfReal+3, eyesHeight() );
    filer->wrDouble( OdResBuf::kDxfReal+4, eyeSize() );

#ifdef SMILEY_WITH_TEXT
    filer->wrString( OdResBuf::kDxfText, mTextField);
#endif

    filer->wrPoint3d( OdResBuf::kDxfXCoord+1, mouthLeft() );
    filer->wrPoint3d( OdResBuf::kDxfXCoord+2, mouthBottom() );
    filer->wrPoint3d( OdResBuf::kDxfXCoord+3, mouthRight() );
}

OdResult AsdkSmiley::dxfInFields(OdDbDxfFiler *filer)
{
    assertWriteEnabled();
	OdResult es;
	OdGePoint3d center, mouthleftpt, mouthbottompt, mouthrightpt;

	if( eOk != (es = OdDbEntity::dxfInFields( filer )) ) return es;

    // Check that we are at the correct subclass data
    if( !filer->atSubclassData( desc()->name() )) return eBadDxfSequence;
    // Read version number (must be first)
    if( filer->nextItem() != OdResBuf::kDxfReal ) return eMakeMeProxy;
    if( filer->rdDouble() > kCurrentVersionNumber ) return eMakeMeProxy;
	// Read of smiley data
    while( !filer->atEOF() ) {
      switch( filer->nextItem() ) {
       case OdResBuf::kDxfXCoord:
	        filer->rdPoint3d( center );
			setCenter( center );
            break;
       case OdResBuf::kDxfReal+1:
            setRadius( filer->rdDouble() );
            break;
       case OdResBuf::kDxfNormalX:
            filer->rdVector3d( mnormal );
	 	    break;
       case OdResBuf::kDxfReal+2:
            setEyesApart( filer->rdDouble() );
            break;
       case OdResBuf::kDxfReal+3:
            setEyesHeight( filer->rdDouble() );
            break;
       case OdResBuf::kDxfReal+4:
            setEyeSize( filer->rdDouble() );
            break;
       case OdResBuf::kDxfXCoord+1:
			filer->rdPoint3d( mouthleftpt );
            break;
       case OdResBuf::kDxfXCoord+2:
            filer->rdPoint3d( mouthbottompt );
            break;
       case OdResBuf::kDxfXCoord+3:
            filer->rdPoint3d( mouthrightpt );
            break;
#ifdef SMILEY_WITH_TEXT
       case OdResBuf::kDxfText:
            filer->rdString( mTextField );
            break;
#endif
      }
    }
	setMouth( mouthleftpt, mouthbottompt, mouthrightpt );
    recordGraphicsModified();
	return eOk;
}

void AsdkSmiley::composeForLoad(OdDb::SaveType format, OdDb::DwgVersion version, OdDbAuditInfo* pAuditInfo)
{
  OdDbEntity::composeForLoad(format, version, pAuditInfo);
  if (pAuditInfo)
  {
    pAuditInfo->printError(this, L"Am I alive?", L"I think I'm OK", L"This should be the case");  // Just for testing
  }
}
