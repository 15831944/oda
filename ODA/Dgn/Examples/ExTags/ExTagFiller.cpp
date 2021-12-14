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

#include "CmColorBase.h"

#include "DgDatabase.h"
#include "DgText.h"
#include "DgTagElement.h"
#include "DgTagSetDefinition.h"
#include "DgLevelTableRecord.h"
#include "DgHostAppServices.h"
#include "DgFontTableRecord.h"

#include "ExTagFiller.h"

#define STL_USING_IOSTREAM
#include "OdaSTL.h"
#define  STD(a)  std:: a
#ifndef _tcout
#ifdef TD_UNICODE
#define _tcout wcout
#else
#define _tcout cout
#endif
#endif


OdExTagFiller::OdExTagFiller()
  : m_pModel3d(0)
  , m_ParentElementID(0)
{
}

void OdExTagFiller::addFontTableRecord()
{
  OdDgFontTableRecordPtr pFont = OdDgFontTableRecord::createObject();
  pFont->setName("GothicE");
  pFont->setType(kFontTypeTrueType);

  OdDgFontTablePtr pFontTable = m_pModel3d->database()->getFontTable(OdDg::kForWrite);
  pFontTable->add(pFont);

  pFont = OdDgFontTableRecord::createObject();
  pFont->setName("Verdana");
  pFont->setType(kFontTypeTrueType);

  pFontTable->add(pFont);

  pFont = OdDgFontTableRecord::createObject();
  pFont->setName("Italic");
  pFont->setType(kFontTypeTrueType);

  pFontTable->add(pFont);

  pFont = OdDgFontTableRecord::createObject();
  pFont->setName("Arial");
  pFont->setType(kFontTypeTrueType);

  pFontTable->add(pFont);

  pFont = OdDgFontTableRecord::createObject();
  pFont->setName("ROMANS");
  pFont->setType(kFontTypeShx);

  pFontTable->add(pFont);

  // there is no necessity to add RSC (MicroStation Recource file) font
  try 
  {
    pFont = OdDgFontTableRecord::createObject();
    pFont->setName("ENGINEERING");
    pFont->setType(kFontTypeRsc);

    pFontTable->add(pFont);
  }
  catch (OdError err)
  {
    ODA_ASSERT(err.code() == eAlreadyInDb);
  }
}


void OdExTagFiller::fillModel(OdDgModel* pModel)
{
  m_pModel3d = pModel;

  //create table of levels : it is the necessary minimum
  {
    OdDgLevelTableRecordPtr pLevel = OdDgLevelTableRecord::createObject();
    pLevel->setName("ExTags level 1");

    OdDgLevelTablePtr pLevelTable = m_pModel3d->database()->getLevelTable(OdDg::kForWrite);
    pLevelTable->add(pLevel);
  }

  /**********************************************************************/
  /* Create new tag definitions                                         */
  /**********************************************************************/
  addFontTableRecord();

  FillTable();

  /**********************************************************************/
  /* Add Text                                                           */
  /**********************************************************************/
  CreateTextElementAsParent();

  /**********************************************************************/
  /* Add tags                                                           */
  /**********************************************************************/
  createTags_offset();
  createTags_rotation();
  createTags_associationFlag();
  createTags_types();
  createTags_formattingFlags();
  createTags_slant();
  createTags_characterSpacing();
  createTags_underlineSpacing();
  createTags_sizeMultiplier();
  createTags_textStyle();
  createTags_font();
  createTags_justification();
}


void OdExTagFiller::CreateTextElementAsParent()
{
  // Create Text Element
  OdDgText3dPtr pText3d = OdDgText3d::createObject();

  pText3d->setJustification( OdDg::kCenterCenter );
  pText3d->setText( OdString( "Bearing element" ) );
  pText3d->setRotation( OdGeQuaternion( 1., 0., 0., 0. ) );
  pText3d->setOrigin( OdGePoint3d( 0., 0., 0. ) );
  //pText3d->setLength( 1. * pText3d->getText().getLength());
  //pText3d->setHeight( 1. );
  pText3d->setLengthMultiplier( 1. );
  pText3d->setHeightMultiplier( 1. );

  m_ParentElementID = m_pModel3d->addElement( pText3d );
}


OdDgTagElementPtr OdExTagFiller::CreateTag( OdUInt16 definitionID )
{
  OdDgTagElementPtr tag;

  //create the element and add it to the model
  tag = OdDgTagElement::createObject();
  m_pModel3d->addElement( tag );

  //bind to the parent element
  tag->setAssociationId( m_ParentElementID );

  //bind to the tag definition
  tag->setTagDefinitionId( m_DefinitionSetID, definitionID );

  //set very common attributes
  //tag->setSizeMultiplier( OdGePoint2d( 100., 100. ) );
  tag->setLengthMultiplier( 0.6 );
  tag->setHeightMultiplier( 0.6 );
  tag->setAssociatedFlag( true );

  return tag;
}

void OdExTagFiller::FillTable()
{
  OdDgTagDefinitionSetPtr definitionSet = OdDgTagDefinitionSet::createObject();
  if ( definitionSet.isNull() )
  {
    throw OdError( eInvalidInput );
  }
  definitionSet->setName( "Set of definitions" );

  OdDgTagDefinitionSetTablePtr pTagTable = m_pModel3d->database()->getTagDefinitionSetTable(OdDg::kForWrite);
  if ( pTagTable.isNull() )
  {
    throw OdError( eInvalidInput );
  }

  pTagTable->add( definitionSet );
  m_DefinitionSetID = definitionSet->elementId();
 
  //add definition for CHAR
  OdDgTagDefinitionPtr pDefinition = definitionSet->addDefinition( 
    OdDgTagDefinition::kChar, OdString( "Name for CHAR" ) );
  if ( pDefinition.get() )
  {
    pDefinition->setPrompt( OdString( "Prompt for CHAR" ));
    pDefinition->setString( "Have a nice day!" );
    m_stringDefinitionId = pDefinition->getEntryId();
  }

  //add definition for INT16
  pDefinition = definitionSet->addDefinition( 
                          OdDgTagDefinition::kInt16, OdString( "Name for INT16" ) );
  if ( pDefinition.get() )
  {
    pDefinition->setPrompt( OdString( "Prompt for INT16" ));
    pDefinition->setInt16( 16 );
    m_int16DefinitionId = pDefinition->getEntryId();
  }

  //add definition for INT32
  pDefinition = definitionSet->addDefinition( 
                          OdDgTagDefinition::kInt32, OdString( "Name for INT32" ) );
  if ( pDefinition.get() )
  {
    pDefinition->setPrompt( OdString( "Prompt for INT32" ));
    pDefinition->setInt32( 32 );
    m_int32DefinitionId = pDefinition->getEntryId();
  }

  //add definition for DOUBLE
  pDefinition = definitionSet->addDefinition( 
                          OdDgTagDefinition::kDouble, OdString( "Name for DOUBLE" ) );
  if ( pDefinition.get() )
  {
    pDefinition->setPrompt( OdString( "Prompt for DOUBLE" ));
    pDefinition->setDouble(1.5);
    m_doubleDefinitionId = pDefinition->getEntryId();
  }

  //add definition for BINARY
  pDefinition = definitionSet->addDefinition( OdDgTagDefinition::kBinary, OdString( "Name for BINARY" ) );
  if ( pDefinition.get() )
  {
    pDefinition->setPrompt( OdString( "Prompt for BINARY" ));
    m_binaryDefinitionId = pDefinition->getEntryId();
  }
}


void OdExTagFiller::createTags_offset()
{
  OdDgTagElementPtr tag;
  int i;
  OdString tagValue;
  
  for( i = 0; i < 4; i++ )
  {
    tag = CreateTag( m_stringDefinitionId );    

    tagValue.format( L"Offset #%d", i );
    tag->setString( tagValue );
    tag->setOrigin( OdGePoint3d( -10., 10., 0. ) );

    tag->setOffset( OdGeVector3d( 0., i + 1, 0. ) );
    tag->setOffsetUsedFlag( i != 4 ); //last element should do not use offset
  }
}


void OdExTagFiller::createTags_rotation()
{
  OdDgTagElementPtr tag;
  int i;
  OdString tagValue;

  for( i = 0; i < 3; i++ )
  {
    tag = CreateTag( m_stringDefinitionId );    

    tagValue.format( L"Rotation #%d", i );
    tag->setString( tagValue );
    tag->setOrigin( OdGePoint3d( -2., 15., 0. ) );
    tag->setColorIndex( 1 );

    tag->setRotation3d( OdGeQuaternion( cos( i * .2 ), 0., 0., -sin( i * .2 ) ) );
  }
}


void OdExTagFiller::createTags_associationFlag()
{
  OdDgTagElementPtr tag;
  int i;
  OdString tagValue;

  for( i = 0; i < 2; i++ )
  {
    tag = CreateTag( m_stringDefinitionId );    

    tagValue.format( L"Association flag #%d", i );
    tag->setString( tagValue );
    tag->setOrigin( OdGePoint3d( 5., 15. + i, 0. ) );
    tag->setColorIndex( 2 );

    tag->setAssociatedFlag( i > 0 );
  }
}


void OdExTagFiller::createTags_types()
{
  OdDgTagElementPtr tag;
  int i;
  OdUInt16 id(0);                                                             // MKU 23/12/09 - ('warning' issue)

  for( i = 0; i < 5; i++ )
  {
    switch( i )
    {
    case 0 : 
      id = m_stringDefinitionId;
      break;
    case 1 : 
      id = m_int16DefinitionId;
      break;
    case 2 : 
      id = m_int32DefinitionId;
      break;
    case 3 : 
      id = m_doubleDefinitionId;
      break;
    case 4 : 
      id = m_binaryDefinitionId;
      break;
    }
    tag = CreateTag( id );

    tag->setOrigin( OdGePoint3d( 16., 15. + i, 0. ) );
    tag->setColorIndex( 3 );
  }
}


void OdExTagFiller::createTags_formattingFlags()
{
  OdDgTagElementPtr tag;
  int i;

  for( i = 0; i < 16; i++ )
  {
    tag = CreateTag( m_stringDefinitionId );    

    tag->setOrigin( OdGePoint3d( 30., 5. + i, 0. ) );
    tag->setColorIndex( 4 );

    switch( i )
    {
    case 0 :
      tag->setUseInterCharSpacingFlag( true );
      tag->setCharacterSpacing( 0.5 );
      tag->setString( "InterChar spacing" );
      break;
    case 1 :
      tag->setUseInterCharSpacingFlag( true );
      tag->setFixedWidthSpacingFlag( true );
      tag->setCharacterSpacing( 0.5 );
      tag->setString( "Fixed width spacing" );
      break;
    case 2 :
      tag->setUnderlineFlag( true );
      tag->setString( "Underline" );
      break;
    case 3 :
      tag->setVerticalFlag( true );
      tag->setString( "Vertical" );
      tag->setOrigin( OdGePoint3d( 40., 8. + i, 0. ) );
      break;
    case 4 :
      tag->setRightToLeftFlag( true );
      tag->setString( "Right-to-left" );
      break;
    case 5 :
      tag->setReverseMlineFlag( true );
      tag->setString( "Reserve MLine" );
      break;
    case 6 :
      tag->setKerningFlag( true );
      tag->setString( "Kerning" );
      break;
    case 7 :
      tag->setSubscriptFlag( true );
      tag->setString( "Subscript" );
      break;
    case 8 :
      tag->setSuperscriptFlag( true );
      tag->setString( "Superscript" );
      break;
    case 9 :
      tag->setOverlineFlag( true );
      tag->setString( "Overline" );
      break;
    case 10 :
      tag->setBoldFlag( true );
      tag->setString( "Bold" );
      break;
    case 11 :
      tag->setFullJustificationFlag( true );
      tag->setString( "Full justification" );
      break;
    case 12 :
      tag->setAcadInterCharSpacingFlag( true );
      tag->setString( "ACAD interChar spacing" );
      break;
    case 13 :
      tag->setBackwardsFlag( true );
      tag->setString( "Backwards" );
      break;
    case 14 :
      tag->setUpsideDownFlag( true );
      tag->setString( "Upside-down" );
      break;
    case 15 :
      tag->setAcadFittedTextFlag( true );
      tag->setString( "ACAD fitted text" );
      break;
    }
  }
}


void OdExTagFiller::createTags_slant()
{
  OdDgTagElementPtr tag;
  int i;
  OdString tagValue;

  for( i = 0; i < 6; i++ )
  {
    tag = CreateTag( m_stringDefinitionId );    

    tagValue.format( L"Slant #%d", i );
    tag->setString( tagValue );
    tag->setOrigin( OdGePoint3d( 30., -10. + i, 0. ) );
    tag->setColorIndex( 5 );

    tag->setSlant( OdaPI/16.0*i );
    tag->setUseSlantFlag( true );
  }
}


void OdExTagFiller::createTags_characterSpacing()
{
  OdDgTagElementPtr tag;
  int i;
  OdString tagValue;

  for( i = 0; i < 5; i++ )
  {
    tag = CreateTag( m_stringDefinitionId );    

    tagValue.format( L"Character spacing #%d", i );
    tag->setString( tagValue );
    tag->setOrigin( OdGePoint3d( 0., -10. + i, 0. ) );
    tag->setColorIndex( 6 );

    tag->setCharacterSpacing( i * 0.2 );
    tag->setUseInterCharSpacingFlag( true );
  }
}


void OdExTagFiller::createTags_underlineSpacing()
{
  OdDgTagElementPtr tag;
  OdString fullName( "Underline spacing" ), shortName( " " );
  int i, j = fullName.getLength();

  for( i = 0; i < j; i++ )
  {
    tag = CreateTag( m_stringDefinitionId );

    shortName.setAt( 0, fullName[ i ] );
    tag->setString( shortName );
    tag->setOrigin( OdGePoint3d( -15. + i / 2., -10., 0. ) );
    tag->setColorIndex( 7 );

    tag->setUnderlineSpacing( i * 0.1 );
    tag->setUnderlineFlag( true );
  }
}


void OdExTagFiller::createTags_sizeMultiplier()
{
  OdDgTagElementPtr tag;
  int i;
  OdString tagValue;

  for( i = 0; i < 5; i++ )
  {
    tag = CreateTag( m_stringDefinitionId );    

    tagValue.format( L"Multiplier #%d", i );
    tag->setString( tagValue );
    tag->setOrigin( OdGePoint3d( -30., -10. + i, 0. ) );
    tag->setColorIndex( 9 );

    //tag->setSizeMultiplier( OdGePoint2d( 30. + i * 10., 30. + i * 10. ) );
    tag->setLengthMultiplier( (30. + i * 10.) * 0.006 );
    tag->setHeightMultiplier( (30. + i * 10.) * 0.006 );
  }
}


void OdExTagFiller::createTags_textStyle()
{
  OdDgTagElementPtr tag;
  int i;
  OdString tagValue;

  for( i = 0; i < 5; i++ )
  {
    tag = CreateTag( m_stringDefinitionId );    

    tagValue.format( L"Text style #%d", i );
    tag->setString( tagValue );
    tag->setOrigin( OdGePoint3d( -30., 0. + i, 0. ) );
    tag->setColorIndex( 10 );

    tag->setTextStyleEntryId( i );
    tag->setUseTextStyleFlag( true );
  }
}


void OdExTagFiller::createTags_font()
{
#pragma MARKMESSAGE("TODO: Create font table and enable method createTags_font()")
  OdDgTagElementPtr tag;
  int i;
  OdString tagValue;

  OdDgFontTablePtr pFontTable   = m_pModel3d->database()->getFontTable();
  OdUInt32 nAlternateFontNumber = pFontTable->getFont(m_pModel3d->database()->appServices()->getAlternateFontName())->getNumber();

  for( i = 0; i < 5; i++ )
  {
    tag = CreateTag( 0 );    

    tagValue.format( L"Text font #%d", i );
    tag->setString( tagValue );
    tag->setOrigin( OdGePoint3d( -15., 0. + i, 0. ) );
    tag->setColorIndex( 11 );

    switch( i )
    {
      case 0:
      {
        tag->setFontEntryId((!(pFontTable->getFont("Verdana")).isNull()) ? pFontTable->getFont("Verdana")->getNumber() : nAlternateFontNumber);
      } break;

      case 1:
      {
        tag->setFontEntryId( (!(pFontTable->getFont("GothicE")).isNull()) ? pFontTable->getFont("GothicE")->getNumber() : nAlternateFontNumber );
      } break;

      case 2:
      {
        tag->setFontEntryId((!(pFontTable->getFont("ENGINEERING")).isNull()) ? pFontTable->getFont("ENGINEERING")->getNumber() : nAlternateFontNumber); // set 'Architectural' font
      } break;

      case 3:
      {
        tag->setFontEntryId( (!(pFontTable->getFont("Arial")).isNull()) ? pFontTable->getFont("Arial")->getNumber() : nAlternateFontNumber );
      } break;

      case 4:
      {
        tag->setFontEntryId( nAlternateFontNumber );
      } break;
    }
  }
}


void OdExTagFiller::createTags_justification()
{
  OdDgTagElementPtr tag;
  int i;

  for( i = 0; i < 15; i++ )
  {
    tag = CreateTag( m_stringDefinitionId );    

    tag->setOrigin( OdGePoint3d( -30., 10. + i, 0. ) );
    tag->setColorIndex( 12 );

    switch( i )
    {
    case 0 :
      tag->setJustification( OdDg::kLeftTop );
      tag->setString( "Left-top" );
      break;
    case 1 :
      tag->setJustification( OdDg::kLeftCenter );
      tag->setString( "Left-center" );
      break;
    case 2 :
      tag->setJustification( OdDg::kLeftBottom );
      tag->setString( "Left-bottom" );
      break;
    case 3 :
      tag->setJustification( OdDg::kLeftMarginTop );
      tag->setString( "Left-margin-top" );
      break;
    case 4 :
      tag->setJustification( OdDg::kLeftMarginCenter );
      tag->setString( "Left-margin-center" );
      break;
    case 5 :
      tag->setJustification( OdDg::kLeftMarginBottom );
      tag->setString( "Left-margin-bottom" );
      break;
    case 6 :
      tag->setJustification( OdDg::kCenterTop );
      tag->setString( "Center-top" );
      break;
    case 7 :
      tag->setJustification( OdDg::kCenterCenter );
      tag->setString( "Center-center" );
      break;
    case 8 :
      tag->setJustification( OdDg::kCenterBottom );
      tag->setString( "Center-bottom" );
      break;
    case 9 :
      tag->setJustification( OdDg::kRightMarginTop );
      tag->setString( "Right-margin-top" );
      break;
    case 10 :
      tag->setJustification( OdDg::kRightMarginCenter );
      tag->setString( "Right-margin-center" );
      break;
    case 11 :
      tag->setJustification( OdDg::kRightMarginBottom );
      tag->setString( "Right-margin-bottom" );
      break;
    case 12 :
      tag->setJustification( OdDg::kRightTop );
      tag->setString( "Right-top" );
      break;
    case 13 :
      tag->setJustification( OdDg::kRightCenter );
      tag->setString( "Right-center" );
      break;
    case 14 :
      tag->setJustification( OdDg::kRightBottom );
      tag->setString( "Right-bottom" );
      break;
    }
  }
}
