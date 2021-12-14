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
/**********************************************************************************/
/* This application reads ExDgnTextFormat.dgn file that contains                  */
/* Text Elements with multibyte text, so codepage is required to                  */
/* convert it into Unicode (used everywhere within ODA Drawings API for DGN).     */
/*                                                                                */
/* This file is read twice: without mapping of fonts and with the fonts           */
/* mapped to codepages                                                            */
/**********************************************************************************/

#include "OdaCommon.h"
#include "StaticRxObject.h"
#include "DynamicLinker.h"
#include "RxInit.h"
#include "DgDatabase.h"
#include "DgModel.h"
#include "ExDgnServices.h"
#include "ExDgnHostAppServices.h"
#include "RxDynamicModule.h"
#include "DgFontTableRecord.h"
#include "OdCharMapper.h"
#include "DgText.h"

#define STL_USING_IOSTREAM
#include "OdaSTL.h"
#define  STD(a)  std:: a

#ifndef _tcout
#ifdef _UNICODE
#define _tcout wcout
#else
#define _tcout cout
#endif
#endif

#ifdef OD_HAVE_CONSOLE_H_FILE
#include <console.h>
#endif


/************************************************************************/
/* Define a Custom Services class.                                      */
/*                                                                      */
/* Combines the platform dependent functionality of                     */
/* OdExDgnSystemServices and OdExDgnHostAppServices                     */ 
/************************************************************************/
class MyServices : public OdExDgnSystemServices, public OdExDgnHostAppServices
{
protected:
  ODRX_USING_HEAP_OPERATORS(OdExDgnSystemServices);
};


#ifndef _TOOLKIT_IN_DLL_
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdDgnModule);
ODRX_BEGIN_STATIC_MODULE_MAP()
  ODRX_DEFINE_STATIC_APPLICATION(L"TG_Db", OdDgnModule)
ODRX_END_STATIC_MODULE_MAP()
#endif


void readFile( OdDgDatabasePtr database );


FILE* ProtocolStream;     //the file for output to


/************************************************************************/
/* Main                                                                 */
/************************************************************************/
#if defined(OD_USE_WMAIN)
int wmain(int argc, wchar_t* argv[])
#else
int main(int argc, char* argv[])
#endif
{
  int nRes = 0; // Return value for main

#ifndef _TOOLKIT_IN_DLL_
  ODRX_INIT_STATIC_MODULE_MAP();
#endif

  /********************************************************************/
  /* Create a custom Services instance.                               */
  /********************************************************************/
  OdStaticRxObject<MyServices> svcs;

  /**********************************************************************/
  /* Display the Product and Version that created the executable        */
  /**********************************************************************/
  STD(cout) << "\nExDgnTextFormat sample program. Copyright (c) 2019, Open Design Alliance\n";

  /**********************************************************************/
  /* Initialize Runtime Extension environment                           */
  /**********************************************************************/
  odrxInitialize( &svcs );

  /**********************************************************************/
  /* Create the protocol file                                           */
  /**********************************************************************/
  ProtocolStream = fopen( "ExDgnTextFormat.txt", "w" );
  if( !ProtocolStream )
  {
    printf( "\nCan not create the file ExDgnTextFormat.txt\n" );
    return 1;
  }

  try
  {
    /**********************************************************************/
    /* Initialize ODA Drawings API for DGN                                */
    /**********************************************************************/
    ::odrxDynamicLinker()->loadModule(L"TG_Db", false);

    fprintf
      ( ProtocolStream, 
        "There are the dumps of text strings in English, Russian and Chinese languages.\n"
        "These strings are loaded with ODA Drawings API for DGN using different settings (fonts are either mapped or not).\n"
        "Note: if a font is not mapped, then CP_UNDEFINED codepage is used, that depends on the current system settings.\n\n"
      );

    //read the file without mapped fonts
    {
      fprintf( ProtocolStream, "#1# No fonts are mapped to codepages, default CP_UNDEFINED codepage is used\n" );

      OdDgDatabasePtr database;
      try
      {
        database = svcs.readFile( "ExDgnTextFormat.dgn" );
      }
      catch (...)
      {
      }

      if( database.isNull() )
      {
        fprintf( ProtocolStream, "Error: can not open the file ExDgnTextFormat.dgn\n" );
        return 1;
      }

      readFile( database );
    }

    //read the file with mapped fonts
    {
      fprintf( ProtocolStream, "\n#2# Russian and Chinese fonts are mapped to codepages CP_ANSI_1251 & CP_GB2312\n" );

      //map V7 fonts to the names
      OdDgV7FontsMapper::addFontName( 201, "RussianFont" );
      OdDgV7FontsMapper::addFontName( 202, "ChineseFont" );

      //map font names to the codepages
      OdCharMapper::addBigFontWithCodepage( "RussianFont", CP_ANSI_1251 );
      OdCharMapper::addBigFontWithCodepage( "ChineseFont", CP_GB2312 );

      OdDgDatabasePtr database;
      try
      {
        database = svcs.readFile( "ExDgnTextFormat.dgn" );
      }
      catch (...)
      {
      }
      if( database.isNull() )
      {
        fprintf( ProtocolStream, "Error: can not open the file ExDgnTextFormat.dgn\n" );
        return 1;
      }

      readFile( database );
    }
  }
  catch (...)
  {
    STD(cout) << "\n\nUnexpected error.";
    nRes = -1;
    throw;
  }

  /**********************************************************************/
  /* Uninitialize Runtime Extension environment                         */
  /**********************************************************************/
  ::odrxUninitialize();

  if (nRes)
  {
     STD(cout) << "\n\nPress ENTER to continue...";
     STD(cin).get();
  }
  STD(cout) << "\n\n";
  return nRes;
}


/**************************************************************************/
/* dump the string                                                        */
/**************************************************************************/
void dumpString( const OdString& string )
{
  int i, j = string.getLength();

  for( i = 0; i < j; i++ )
  {
    fprintf( ProtocolStream, "0x%.4X ", string[ i ] );
  }
}


/**************************************************************************/
/* read the file ExDgnTextFormat.dgn and check some text elements from it */
/**************************************************************************/
void readFile( OdDgDatabasePtr database )
{
  OdDgText3dPtr englishText, russianText, chineseText;

  //read the file and take all necessary text elements
  {
    OdDgModelPtr model = database->getActiveModelId().openObject( OdDg::kForRead );

    if( model.isNull() )
    {
      fprintf( ProtocolStream, "Can not find any model\n" );
      return;
    }

    OdDgElementIteratorPtr iterator = model->createGraphicsElementsIterator();
    OdDgGraphicsElementPtr element;
    for( ; !iterator->done(); iterator->step() )
    {
      element = iterator->item().openObject( OdDg::kForRead );
      if( element->getElementType() == 17 )
      {
        switch( element->getLevelEntryId() )
        {
        case 2 : englishText = element; break;
        case 3 : russianText = element; break;
        case 4 : chineseText = element; break;
        }
      }
    }
  }

  if( englishText.isNull() || russianText.isNull() || chineseText.isNull() )
  {
    fprintf( ProtocolStream, "Can not find all necessary text elements\n" );
    return;
  }

  fprintf( ProtocolStream, "English string : " );
  dumpString( englishText->getText() );
  fprintf( ProtocolStream, "\nRussian string : " );
  dumpString( russianText->getText() );
  fprintf( ProtocolStream, "\nChinese string : " );
  dumpString( chineseText->getText() );
  fprintf( ProtocolStream, "\n" );
}
