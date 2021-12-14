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
/* This console application demonstrates the use of the oddbEntGet()    */
/* function.  See dumpObject()                                          */
/*                                                                      */
/* Calling sequence:                                                    */
/*                                                                      */
/*    OdBagFilerEx  <filename>                                          */
/*                                                                      */
/************************************************************************/

#include "OdaCommon.h"
#include "DbDatabase.h"
#include "DbEntity.h"
#include "RxObjectImpl.h"
#include "DbSymbolTable.h"
#include "DbBlockTable.h"
#include "DbBlockTableRecord.h"
#include "DbDictionary.h"
#include "DbMlineStyle.h"
#include "DbFiler.h"
#include "RxDynamicModule.h"
#include "DbUnitsFormatter.h"

#include "ExSystemServices.h"
#include "ExHostAppServices.h"

#define STL_USING_IOSTREAM
#include "OdaSTL.h"
#include <fstream>
#define  STD(a)  std:: a

#ifdef OD_HAVE_CONSOLE_H_FILE
#include <console.h>
#endif



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

/************************************************************************/
/* Format the value from the specified ResBuf                           */
/************************************************************************/
static OdString FormatValue( const OdResBuf* rb )
{
  OdString s;

  if (rb->restype() == OdResBuf::kRtEntName || rb->restype() == OdResBuf::kDxfEnd)
  {
    OdDbObjectId id = rb->getObjectId(0);
    s = id.getHandle().ascii();
    return s;
  }

  switch ( rb->restype() )
  {
  case OdResBuf::kRtColor:
    s = OdDbUnitsFormatter::formatColor( rb->getColor() );
    break;
  default:
  switch ( OdDxfCode::_getType( rb->restype() ) )
  {
  case OdDxfCode::Name:
  case OdDxfCode::String:
  case OdDxfCode::Handle:
  case OdDxfCode::LayerName:
    s = rb->getString();
    break;
  case OdDxfCode::Bool:
    s.format(OD_T("%d"), rb->getBool());
    break;
  case OdDxfCode::Integer8:
    s.format(OD_T("%d"), rb->getInt8());
    break;
  case OdDxfCode::Integer16:
    s.format(OD_T("%d"), rb->getInt16());
    break;
  case OdDxfCode::Integer32:
    s.format(OD_T("%d"), rb->getInt32());
    break;
  case OdDxfCode::Integer64:
    s.format(L"%" PRId64W, rb->getInt64());
    break;
  case OdDxfCode::Double:
  case OdDxfCode::Angle:
    s.format(OD_T("%g"), rb->getDouble());
    break;
  case OdDxfCode::Point:
    s.format(OD_T("(%g, %g, %g)"), rb->getPoint3d().x,rb->getPoint3d().y, rb->getPoint3d().z );
    break;
  case OdDxfCode::ObjectId:
  case OdDxfCode::SoftPointerId:
  case OdDxfCode::HardPointerId:
  case OdDxfCode::SoftOwnershipId:
  case OdDxfCode::HardOwnershipId:
    s = rb->getHandle().ascii();
    break;
  default:
    break;
  }
  }
  return s;
}

/************************************************************************/
/* Format the code from the specified ResBuf                            */
/************************************************************************/
static OdString FormatCode( const OdResBuf* rb )
{
  OdString s;
  s.format( OD_T("%5d"), rb->restype() );
  return s;
}

/************************************************************************/
/* Dump the specified object object to the specified output stream      */
/************************************************************************/
void dumpObject(OdDbObjectId id)
{
  odPrintConsoleString(L"--------------------\n");
  for ( OdResBufPtr rb = oddbEntGet(id); !rb.isNull(); rb = rb->next() )
  {
    odPrintConsoleString(L"%ls\n", FormatCode( rb ).c_str());
    odPrintConsoleString(L"%ls\n", FormatValue( rb ).c_str());
  }
}

/************************************************************************/
/* Dump the line types table from the specified database to the specified    */
/* output stream                                                        */
/************************************************************************/
void dumpLTypes(OdDbDatabase* pDb)
{
  OdDbSymbolTablePtr pLTypes = pDb->getLinetypeTableId().safeOpenObject();

  odPrintConsoleString(L"\nLine Types: \n");
  OdDbSymbolTableIteratorPtr pLTypesIter = pLTypes->newIterator();

  for (pLTypesIter->start(); ! pLTypesIter->done(); pLTypesIter->step())
  {
    OdDbSymbolTableRecordPtr pLType = pLTypesIter->getRecordId().safeOpenObject();

    dumpObject(pLType->objectId());
    odPrintConsoleString(L"\n");
  }
} 
/************************************************************************/
/* Dump the block table from the specified database to the specified    */
/* output stream                                                        */
/************************************************************************/
void dumpBlocks(OdDbDatabase* pDb)
{
  OdDbBlockTablePtr pBlocks = pDb->getBlockTableId().safeOpenObject();

  odPrintConsoleString(L"\nBlocks: \n");
  OdDbSymbolTableIteratorPtr pBlkIter = pBlocks->newIterator();

  for (pBlkIter->start(); ! pBlkIter->done(); pBlkIter->step())
  {
    OdDbBlockTableRecordPtr pBlock = pBlkIter->getRecordId().safeOpenObject();

    dumpObject(pBlock->objectId());
    odPrintConsoleString(L"\n");

    OdDbObjectIteratorPtr pEntIter = pBlock->newIterator();

    for (; !pEntIter->done(); pEntIter->step())
    {
      dumpObject(pEntIter->objectId());
    }
  }
} 
/************************************************************************/
/* Dump the specified database to the specified output stream           */
/************************************************************************/
void dump(OdDbDatabase* pDb, OdDbHostAppServices *pHostApp )
{
  dumpLTypes(pDb);
  dumpBlocks(pDb);
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

  if (argc < 2) 
  {
    odPrintConsoleString(L"usage: OdBagFilerEx <filename>\n");
  }
  else
  {
    OdStaticRxObject<MyServices> svcs;
    svcs.setRecomputeDimBlocksRequired(false);
    odInitialize(&svcs);

    odPrintConsoleString(L"Developed using %ls, %ls\n", svcs.product().c_str(), svcs.versionString().c_str());

    try 
    {
      OdDbDatabasePtr pDb;
      OdString f(argv[1]);
      pDb = svcs.readFile(f.c_str(), false, false, Oda::kShareDenyNo); 

      if (!pDb.isNull())
      {
        dump(pDb, &svcs);
      }
    }
    catch (OdError& e)
    {
      odPrintConsoleString(L"Teigha Error: %ls\n\n", e.description().c_str());
      return 0;
    }
    catch (...)
    {
      odPrintConsoleString(L"Unknown Error.\n\n");
      return 0;
    }

    odUninitialize();
  }
  odPrintConsoleString(L"Finished OK.\n\n");
  return 0;
}
