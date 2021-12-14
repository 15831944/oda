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
/* This console application reads, dumps, and optionally writes         */
/* a DST file.                                                          */
/* Calling sequence:                                                    */
/*                                                                      */
/*    dsttest.exe <infilename>.dst <outfilename>.dst                    */
/*                                                                      */
/************************************************************************/

/************************************************************************/
/* The next two lines define the layout to be added to the database.    */
/************************************************************************/
#define LAYOUT_NAME L"8th Floor Furniture Plan - Mono"
#define LAYOUT_FILE_NAME L"C:\\Program Files\\AutoCAD 2007\\Sample\\8th floor furniture.dwg"

#include "OdaCommon.h"
#include "RxInit.h"
#include "StaticRxObject.h"
#include "ExSystemServices.h"
#ifdef OD_HAVE_CONSOLE_H_FILE
#include <console.h>
#endif
#define STL_USING_IOSTREAM
#include "OdaSTL.h"
#include <iostream>
#include "FormatControl.h"
#include "RxDynamicModule.h"
#include "DynamicLinker.h"

#include "Sm/SmDatabase.h"
#include "Sm/SmSheetSetMgr.h"
#include "Sm/SmCalloutBlocks.h"
#include "Sm/SmSheetView.h"
#include "Sm/SmViewCategory.h"
#include "Sm/SmSheetViews.h"
#include "Sm/SmPersist.h"
#include "Sm/SmPublishOptions.h"
#include "Sm/SmResources.h"
#include "Sm/SmSheetSelSet.h"
#include "Sm/SmSheetSelSets.h"
#include "Sm/SmViewCategories.h"

void smTest( const OdString& path, const OdString& outpath );

#ifndef _TOOLKIT_IN_DLL_
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdSmModule);
ODRX_BEGIN_STATIC_MODULE_MAP()
  ODRX_DEFINE_STATIC_APPLICATION(OdSmModuleName, OdSmModule)
ODRX_END_STATIC_MODULE_MAP()
#endif

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
  if ( argc < 2 ) 
  {
    std::wcout << L"Usage: OdSheetSet.exe infilename.dst [outfilename.dst]\n";
    return 1;
  }

#ifndef _TOOLKIT_IN_DLL_
    ODRX_INIT_STATIC_MODULE_MAP();
#endif

  /**********************************************************************/
  /* Create a custom Services instance.                                 */
  /**********************************************************************/
  OdStaticRxObject<ExSystemServices> svcs;
  /**********************************************************************/
  /* Initialize the Drawings SDK.                                       */
  /**********************************************************************/
  ::odrxInitialize(&svcs);
  {
    /********************************************************************/
    /* Load SmModule                                                    */
    /********************************************************************/
    OdRxModulePtr m = ::odrxDynamicLinker()->loadModule(OdSmModuleName);
    /********************************************************************/
    /* Process the input file                                           */
    /********************************************************************/
    try
    {
      if ( argc == 2 )
        smTest( argv[1], L"" );
      else
        smTest( argv[1], argv[2] );
    }
    catch (OdError& err)
    {
      std::wcout << L"Teigha Error: " << err.description().c_str();
    }
    catch (...)
    {
      std::wcout << L"Unknown Error.\n\n";
      return 0;
    }
  }
  /**********************************************************************/
  /* Uninitialize the Drawings SDK.                                     */
  /**********************************************************************/
  odrxUninitialize();
  return 0;
}

/************************************************************************/
/* Initialize _offset and TABSIZE                                       */
/************************************************************************/
const int offset::TABSIZE = 2;
int offset::_offset = 0;

using namespace dst;

/************************************************************************/
/* Print the Variant Value                                              */
/************************************************************************/
std::wostream& operator << ( std::wostream& o, const OdVariant& val )
{
  switch( val.type() )
  {
  case OdVariant::kInt32:
    return o << offset() << val.getInt32();
  case OdVariant::kInt16:
    return o << offset() << val.getInt16();
  case OdVariant::kString:
    return o << offset() << val.getString().c_str();
  case OdVariant::kRxObjectPtr:
    {
      OdSmPersistPtr p = val.getRxObjectPtr();
      return o << offset() << p->typeName();
    }
  }
  return o;
}

/************************************************************************/
/* Print the SmFileReference                                            */
/************************************************************************/
std::wostream& operator << ( std::wostream& o, const OdSmFileReference& val )
{
  return &val ? o << const_cast<OdSmFileReference&>(val).resolveFileName() : o;
}
/************************************************************************/
/* Print the SmCustomProperties                                         */
/************************************************************************/
void printProperties( const OdSmCustomPropertyBag* bag )
{
  if ( !bag ) return;
  OdSmEnumCustomPropertyValuePtr e = bag->getPropertyEnumerator();
  OdString name;
  const OdSmCustomPropertyValue* val;
  while( e->next( name, val ) )
  {
    std::wcout << offset() << name << L" = " << val->getValue() << std::endl;
  }
}
/************************************************************************/
/* Print the SmDbBlockRecordReference                                   */
/************************************************************************/
void printBlockRecord( const OdSmDbBlockRecordReference* ref )
{
  if (!ref) return;
  OffsetShift os;
  std::wcout << offset() << L"Name:     " << ref->getName() << std::endl;
  std::wcout << offset() << L"Filename: " << ref->getFileName() << std::endl;
  std::wcout << offset() << L"Handle:   " << ref->getDbHandle() << std::endl;
}

void printBlockRecords( OdSmEnumDbBlockRecordReferencePtr e )
{
  if (e.isNull()) return;
  while ( const OdSmDbBlockRecordReference* ref = e->next() )
  {
    printBlockRecord( ref );
    std::wcout << std::endl;
  }
}
/************************************************************************/
/* Print the SmComponent                                                */
/************************************************************************/
void printSmComponent( const OdSmComponent* c )
{
  if (!c) return;
  std::wcout << offset() << c->typeName() << L" properties" << std::endl;
  OffsetShift os;
  std::wcout << offset() << L"Name: " << c->getName() << std::endl;
  std::wcout << offset() << L"Desc: " << c->getDesc() << std::endl;
  printProperties( c->customProperties() );
}

void printLayout( const OdSmDbLayoutReference* layout )
{
  if ( !layout ) return;
  OffsetShift os;
  std::wcout << offset() << L"Layout name:   " << layout->getName() << std::endl;
  std::wcout << offset() << L"Layout handle: " << layout->getDbHandle() << std::endl;
  std::wcout << offset() << L"Layout file:   " << const_cast<OdSmDbLayoutReference*>(layout)->resolveFileName() << std::endl;
}

/************************************************************************/
/* Print the Sheet Views                                                */
/************************************************************************/
void printViews( OdSmEnumSheetViewPtr e )
{
  if (e.isNull()) return;
  while ( const OdSmSheetView* v = e->next() )
  {
    OffsetShift os;
    printSmComponent( v );
    if ( v->getCategory() )
      std::wcout << offset() << L"View Category: " << v->getCategory()->getName() << std::endl;
    std::wcout << offset()   << L"View title:    " << v->getTitle() << std::endl;
    if (v->getNamedView()) 
    {
      std::wcout << offset()   << L"View filename: " << const_cast<OdSmDbViewReference*>(v->getNamedView())->resolveFileName() << std::endl;
      std::wcout << offset()   << L"View handle:   " << v->getNamedView()->getDbHandle() << std::endl;
    }
  }
}
/************************************************************************/
/* Print the Sheets                                                     */
/************************************************************************/
void printSheets( OdSmEnumComponentPtr sheets )
{
  if (sheets.isNull()) return;
  while ( const OdSmComponent* c = sheets->next() )
  {
    OffsetShift os;
    printSmComponent( c );
    OdSmSheetPtr sheet = OdSmSheet::cast( c );
    if ( !sheet.isNull() )
    {
      std::wcout << offset() << L"Sheet title:  " << sheet->getTitle() << std::endl;
      std::wcout << offset() << L"Sheet number: "<< sheet->getNumber() << std::endl;
      std::wcout << offset() << L"Sheet Layout:\n";
      printLayout( sheet->getLayout() );
      std::wcout << offset() << L"Sheet Views:\n";
      if (sheet->getSheetViews())
        printViews( sheet->getSheetViews()->getEnumerator() );
      std::wcout << std::endl;
    }
    else
    {
      OdSmSubsetPtr s = OdSmSubset::cast( c );
      if ( !s.isNull() )
      {
        std::wcout << offset() << L"Subset begin: " << s->getName() << std::endl << std::endl;
        printSheets( s->getSheetEnumerator() );
        std::wcout << offset() << L"Subset end: " << s->getName() << std::endl << std::endl;
      }
      else
      {
        std::wcout << offset() << L"Component is not a sheet or sheetset, it is a " << c->typeName() << std::endl;
      }
    }
  }
}

/************************************************************************/
/* Print the Publish Options                                            */
/************************************************************************/
void printPublishOptions( const OdSmPublishOptions* opts )
{
  if (!opts) return;
  OffsetShift os;
  std::wcout << offset() << L"DefaultOutputdir:  " << *opts->getDefaultOutputdir() << std::endl;
  std::wcout << offset() << L"DwfType:           " << opts->getDwfType() << std::endl;
  std::wcout << offset() << L"PromptForName:     " << opts->getPromptForName() << std::endl;
  std::wcout << offset() << L"PromptForPassword: " << opts->getPromptForPassword() << std::endl;
  std::wcout << offset() << L"UsePassword:       " << opts->getUsePassword() << std::endl;
  std::wcout << offset() << L"LayerInfo:         " << opts->getLayerInfo() << std::endl;
  //std::wcout << offset() << L"" << << std::endl;
}

/************************************************************************/
/* Print the Resources                                                  */
/************************************************************************/
void printResources( const OdSmResources* rr )
{
  if (!rr) return;
  OffsetShift os;
  OdSmEnumFileReferencePtr e = rr->getEnumerator();
  while ( const OdSmFileReference* ref = e->next() )
    std::wcout << offset() << *ref << std::endl;
}

/************************************************************************/
/* Print the Sheet Selection Set                                        */
/************************************************************************/
void printSheetSelSets( const OdSmSheetSelSets* ss )
{
  if (!ss) return;
  OffsetShift os;
  OdSmEnumSheetSelSetPtr e = ss->getEnumerator();
  while (const OdSmSheetSelSet* s = e->next() )
  {
    printSmComponent( s );
    printSheets( s->getEnumerator() );
  }
}

/************************************************************************/
/* Print the View Category                                              */
/************************************************************************/
void printViewCategory( const OdSmViewCategory* vc )
{
  if (!vc) return;
  printSmComponent( vc );
  if (vc->getCalloutBlocks())
  {
    std::wcout << offset() << L"CalloutBlocks:\n";
    printBlockRecords( vc->getCalloutBlocks()->getEnumerator() );
  }
  std::wcout << offset() << L"SheetViews:\n";
  printViews( vc->getSheetViewEnumerator() );
}

/************************************************************************/
/* Print the Sheet Selection Set                                        */
/************************************************************************/
void printViewCategories( const OdSmViewCategories* vcs )
{
  if ( !vcs ) return;
  OffsetShift os;
  OdSmEnumViewCategoryPtr e = vcs->getEnumerator();
  while ( const OdSmViewCategory* vc = e->next() )
  {
    printViewCategory( vc );
  }
}

/************************************************************************/
/* Print the Sheet Set                                                  */
/************************************************************************/
void printSheetSet( const OdSmSheetSet* ss )
{
  if (!ss) return;
  OffsetShift os;
  printSmComponent( ss );
  std::wcout << offset() << L"AltPageSetups: " << *ss->getAltPageSetups() << std::endl;
  if (ss->getCalloutBlocks())
  {
    std::wcout << offset() << L"CalloutBlocks:\n";
    printBlockRecords( ss->getCalloutBlocks()->getEnumerator() );
  }
  std::wcout << offset() << L"DefDwtLayout:\n";
  printLayout( ss->getDefDwtLayout() );
  std::wcout << offset() << L"DefLabelBlk:\n";
  printBlockRecord( ss->getDefLabelBlk() );
  std::wcout << offset() << L"NewSheetLocation: " << *ss->getNewSheetLocation() << std::endl;
  if (ss->getPublishOptions())
  {
    std::wcout << offset() << L"PublishOptions:\n";
    printPublishOptions( ss->getPublishOptions() );
  }
  std::wcout << offset() << L"Resources:\n";
  printResources( ss->getResources() ); 
  std::wcout << offset() << L"SheetSelSets:\n";
  printSheetSelSets( ss->getSheetSelSets() );
  std::wcout << offset() << L"ViewCategories:\n";
  printViewCategories( ss->getViewCategories() );
  std::wcout << offset() << L"Sheets:\n";
  printSheets( ss->getSheetEnumerator() );
}
/************************************************************************/
/* Print the database                                                   */
/************************************************************************/
void printDatabase( OdSmDatabase* db )
{
  /**********************************************************************/
  /* Print the Sheet Manager component                                  */
  /**********************************************************************/
  printSmComponent( db );
  /**********************************************************************/
  /* Print the Sheet Set                                                */
  /**********************************************************************/
  printSheetSet( db->sheetSet() );
}

/************************************************************************/
/* Modify the Database                                                  */
/************************************************************************/
void modifyDatabase( OdSmDatabase* db )
{
  /**********************************************************************/
  /* Add a '.'                                                          */
  /**********************************************************************/
  OdSmEnumComponentPtr e = db->sheetSet()->getSheetEnumerator();
  while ( OdSmComponent* c = e->next() )
    c->setName( c->getName() + L"." );
  /**********************************************************************/
  /* Add a new Layout to the Sheet Set                                  */
  /**********************************************************************/
  OdSmDbLayoutReferencePtr layout = OdSmDbLayoutReference::createObject();
  layout->initNew( db->sheetSet() );
  layout->setName( LAYOUT_NAME );
  layout->setDbHandle( L"11A2" );
  layout->setFileName( LAYOUT_FILE_NAME );
  db->sheetSet()->insertComponent( db->sheetSet()->importSheet( layout ) );
}

/************************************************************************/
/* Process the input file                                               */
/************************************************************************/
void smTest( const OdString& inpath, const OdString& outpath )
{
  /**********************************************************************/
  /* Create a sheet set manager                                         */
  /**********************************************************************/
  OdSmSheetSetMgrPtr mgr = OdSmSheetSetMgr::createObject();
  /**********************************************************************/
  /* Create an SmDatabase from the input file                           */
  /**********************************************************************/
  OdSmDatabase* db = mgr->openDatabase( inpath );
  /**********************************************************************/
  /* Print the database                                                 */
  /**********************************************************************/
  printDatabase( db );
  /**********************************************************************/
  /* Modify the database                                                */
  /**********************************************************************/
  modifyDatabase( db );
  /**********************************************************************/
  /* Write the modified database                                        */
  /**********************************************************************/
  if (!outpath.isEmpty())
  {
    db->setFileName( outpath );
    db->save();
  }
  /**********************************************************************/
  /* Close the database                                                  */
  /**********************************************************************/
  mgr->close( db );
}
