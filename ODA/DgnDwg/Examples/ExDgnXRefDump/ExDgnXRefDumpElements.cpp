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
#include "ExDgnXRefDumpElements.h"
#include "DgAttributeLinkage.h"
#include "DgSummaryInfo.h"
#include "DbDatabase.h"
#include "DgTable.h"
#include "DgLevelTableRecord.h"
#include "DgCmColor.h"

/*******************************************************************************/
/* Here to include ODA Drawings SDK header files required for your sample      */
/*******************************************************************************/
#include "DbBlockTableRecord.h"
#include "DbLine.h"

//
// OdDgObject_Dumper
//


ODRX_NO_CONS_DEFINE_MEMBERS( OdDgObject_Dumper, OdRxObject )

FILE* DumpStream;


int                 OdDgObject_Dumper::m_nesting;
OdArray< OdString > OdDgObject_Dumper::m_nestedTypes;


OdDgObject_Dumper::OdDgObject_Dumper()
: m_references( 0 )
{
}


void OdDgObject_Dumper::addRef()
{
  m_references++;
}


void OdDgObject_Dumper::release()
{
  m_references--;
}


long OdDgObject_Dumper::numRefs() const
{
  return m_references;
}


void OdDgObject_Dumper::startDescription( OdRxObjectPtr object )
{
  writeShift();
  m_object = object;
  //write it name
  OdString className;
  {
    OdSmartPtr< OdDgObject_Dumper > dumper = object;

    className = dumper->getClassName();
    fwprintf( DumpStream, L"> %ls", className.c_str() );
  }

  //try to add a type of element
  {
    OdDgElementPtr element = OdDgElement::cast( m_object );

    if( !element.isNull() )
    {
      fwprintf( DumpStream, L" Element ID: %ls", element->elementId().getHandle().ascii().c_str() );
      fwprintf( DumpStream, L" Type: %d", element->getElementType() );
    }
  }

  //add the path
  {
    fwprintf( DumpStream, L" Path: */" );

    int i, j = m_nestedTypes.size();

    for( i = 0; i < j; i++ )
    {
      fwprintf( DumpStream, L"%ls/", m_nestedTypes[ i ].c_str() );
    }
  }

  //final actions
  fwprintf( DumpStream, L"\n" );
  m_nestedTypes.push_back( className );
  m_nesting++;

  // Dump linkage description
  {
    OdDgElementPtr pElm = OdDgElement::cast( m_object );
    if( pElm.get() )
    {
      OdRxObjectPtrArray linkages;
      pElm->getLinkages(linkages);
      if (linkages.size() > 0 )
      {
        writeShift();
        fwprintf( DumpStream, L"> Attribute Linkages\n" );
        for (unsigned i = 0; i < linkages.size(); ++i)
        {
          OdDgAttributeLinkagePtr pLinkage = linkages[i];
          OdBinaryData data;
          pLinkage->getData(data);

          writeShift();
          fwprintf( DumpStream, L"    Linkage ID: 0x%x", pLinkage->getPrimaryId() );
          fwprintf( DumpStream, L" Raw Data Size: %d", data.size() );
          OdDgStringLinkagePtr pStrLinkage = OdDgStringLinkage::cast( pLinkage );
          if ( pStrLinkage.get() )
          {
            fwprintf( DumpStream, L"  ( String Linkage ID: %d", pStrLinkage->getStringId() );
            fwprintf( DumpStream, L"  Value: %ls )", pStrLinkage->getString().c_str() );
          }
          fwprintf( DumpStream, L"\n" );
        }
        writeShift();
        fwprintf( DumpStream, L"< Attribute Linkages\n" );
      }
    }
  }
}


void OdDgObject_Dumper::finishDescription()
{
  m_nesting--;
 
  writeShift();
  fwprintf( DumpStream, L"< %ls\n", m_nestedTypes.last().c_str() );

  m_nestedTypes.removeLast();
}


void OdDgObject_Dumper::describeSubElements( OdDgElementIteratorPtr iterator )
{
  int           counter = 0;
  OdRxObjectPtr object;

  for( ; !iterator->done(); iterator->step(), counter++ )
  {
    writeShift();
    fwprintf( DumpStream, L"Sub element %d:\n", counter );

    object = iterator->item().openObject();

    try
    {
      OdSmartPtr< OdDgObject_Dumper > dumper = object;
      dumper->dump( object );
    }
    //i.e., it is an object without dumper
    catch( ... )
    {
      OdDgElementPtr element = object;

      writeShift();
      fwprintf( DumpStream, L"Object has not its dumper. Element type: %I32u\n", element->getElementType() );
    }
  }
}


void OdDgObject_Dumper::writeShift()
{
  int i;

  for( i = 0; i < m_nesting; i++ )
  {
    fwprintf( DumpStream, L"  " );
  }
}


void OdDgObject_Dumper::writeGraphicsElementInfo( OdDgGraphicsElementPtr element )
{
  writeShift();
  fwprintf( DumpStream, L"Common information for Graphics Elements:\n" );

  m_nesting++;

  writeFieldValue( "Color index", element->getColorIndex() );
  writeFieldValue( "Graphics group", element->getGraphicsGroupEntryId() );
  writeFieldValue( "Class", element->getClass() );
  writeFieldValue( "Line style", element->getLineStyleEntryId() );
  writeFieldValue( "Line weight", element->getLineWeight() );

  m_nesting--;
}


void OdDgObject_Dumper::writeFieldValue( const OdString& name, const OdString& value )
{
  writeFieldName( name );
  fwprintf( DumpStream, L"%ls\n", value.c_str() );
}


void OdDgObject_Dumper::writeFieldValue( const OdString& name, OdUInt16 value )
{
  writeFieldName( name );
  fwprintf( DumpStream, L"%u\n", OdUInt32( value ) );
}


void OdDgObject_Dumper::writeFieldValueHex( const OdString& name, OdUInt16 value )
{
  writeFieldName( name );
  fwprintf( DumpStream, L"%04X\n", OdUInt32( value ) );
}


void OdDgObject_Dumper::writeFieldValue( const OdString& name, OdUInt32 value )
{
  writeFieldName( name );
  fwprintf( DumpStream, L"%u\n", value );
}


void OdDgObject_Dumper::writeFieldValueHex( const OdString& name, OdUInt32 value )
{
  writeFieldName( name );
  fwprintf( DumpStream, L"%08X\n", value );
}


void OdDgObject_Dumper::writeFieldValue( const OdString& name, double value )
{
  writeFieldName( name );
  fwprintf( DumpStream, L"%g\n", value );
}


void OdDgObject_Dumper::writeFieldValue( const OdString& name, OdGePoint2d const& value )
{
  writeFieldName( name );
  fwprintf( DumpStream, L"%g; %g\n", value.x, value.y );
}


void OdDgObject_Dumper::writeFieldValue( const OdString& name, OdGePoint3d const& value )
{
  writeFieldName( name );
  fwprintf( DumpStream, L"%g; %g; %g\n", value.x, value.y, value.z );
}


void OdDgObject_Dumper::writeFieldValue( const OdString& name, OdGeVector3d const& value )
{
  writeFieldName( name );
  fwprintf( DumpStream, L"%g; %g; %g\n", value.x, value.y, value.z );
}


void OdDgObject_Dumper::writeFieldValue( const OdString& name, OdUInt64 value )
{
  writeFieldName( name );
  fwprintf( DumpStream, L"%I64u\n", value );
}


void OdDgObject_Dumper::writeFieldValue( const OdString& name, OdUInt8 value )
{
  writeFieldName( name );
  fwprintf( DumpStream, L"%u\n", unsigned( value ) );
}

  
void OdDgObject_Dumper::writeFieldValueHex( const OdString& name, OdUInt8 value )
{
  writeFieldName( name );
  fwprintf( DumpStream, L"%02X\n", unsigned( value ) );
}


void OdDgObject_Dumper::writeFieldValue( const OdString& name, bool value )
{
  writeFieldName( name );
  fwprintf( DumpStream, value ? L"true\n" : L"false\n" );
}


void OdDgObject_Dumper::writeFieldValue( const OdString& name, OdInt16 value )
{
  writeFieldName( name );
  fwprintf( DumpStream, L"%i\n", int( value ) );
}


void OdDgObject_Dumper::writeFieldValue( const OdString& name, OdInt32 value )
{
  writeFieldName( name );
  fwprintf( DumpStream, L"%I32i\n", int( value ) );
}
#if !(defined(ODA_UNIXOS) && (OD_SIZEOF_LONG == 8))
void OdDgObject_Dumper::writeFieldValue( const OdString& name, int value )
{
  writeFieldName( name );
  fwprintf( DumpStream, L"%I32i\n", value );
}
#endif

void OdDgObject_Dumper::writeFieldValue( const OdString& name, OdInt64 value )
{
  writeFieldName( name );
  fwprintf( DumpStream, L"%iI64\n", int( value ) );
}


void OdDgObject_Dumper::writeFieldValue( const OdString& name, OdGeMatrix3d const& value )
{
  writeFieldName( name );

  fwprintf( DumpStream, L"%g; %g; %g; %g; %g; %g; %g; %g; %g\n",
            value( 0, 0 ), value( 1, 0 ), value( 2, 0 ), 
            value( 0, 1 ), value( 1, 1 ), value( 2, 1 ), 
            value( 0, 2 ), value( 1, 2 ), value( 2, 2 ) );
}


void OdDgObject_Dumper::writeFieldName( const OdString& fieldName )
{
  writeShift();
  fwprintf( DumpStream, L"%-30ls = ", fieldName.c_str() );
}


//
// all other OdDgXXX_Dumper's
//

void OdDgDatabase_Dumper::dumpSummaryInfo( OdDgDatabase* pDb )
{
  fwprintf( DumpStream, L"> Summary Information\n");
  m_nesting++;

  // Summary Information
  OdDgSummaryInformationPtr pSi = oddgGetSummaryInformation(pDb);
  writeFieldValue( "Title", pSi->getTitle() );
  writeFieldValue( "Subject", pSi->getSubject() );
  writeFieldValue( "Author", pSi->getAuthor() );
  writeFieldValue( "Keywords", pSi->getKeywords() );
  writeFieldValue( "Comments", pSi->getComments() );
  writeFieldValue( "Template", pSi->getTemplate() );
  writeFieldValue( "LastSavedBy", pSi->getLastSavedBy() );
  writeFieldValue( "RevisionNumber", pSi->getRevisionNumber() );
  writeFieldValue( "ApplicationName", pSi->getApplicationName() );
  OdString timeStr;
  OdTimeStamp ts = pSi->getTotalEditingTime();
  if ( ts != OdTimeStamp(OdTimeStamp::kInitZero) )
    ts.strftime(OdString(L"%H:%M:%S"), timeStr);
  else
    timeStr = L"0";
  writeFieldValue( "TotalEditingTime", timeStr );
  ts = pSi->getLastPrintedTime();
  if ( ts != OdTimeStamp(OdTimeStamp::kInitZero) )
    ts.ctime(timeStr);
  else
    timeStr = L"0";
  writeFieldValue( "LastPrintedTime", timeStr );
  pSi->getCreatedTime().ctime(timeStr);
  writeFieldValue( "CreatedTime", timeStr );
  pSi->getLastSavedTime().ctime(timeStr);
  writeFieldValue( "LastSavedTime", timeStr );
  writeFieldValue( "Security", pSi->getSecurity() );

  // Document Summary Information
  OdDgDocumentSummaryInformationPtr pDsi = oddgGetDocumentSummaryInformation(pDb);
  writeFieldValue( "Category", pDsi->getCategory() );
  writeFieldValue( "Manager", pDsi->getManager() );
  writeFieldValue( "Company", pDsi->getCompany() );

  m_nesting--;
}


void OdDgDatabase_Dumper::dump( OdRxObjectPtr object )
{
  OdDgDatabasePtr pDb = object;

  dumpSummaryInfo( pDb );

  startDescription( object );

  //simple fields
  writeFieldValue( "Filename", pDb->getFilename() );
  writeFieldValue( "Major version", pDb->getMajorVersion() );
  writeFieldValue( "Minor version", pDb->getMinorVersion() );
  writeFieldValue( "Low", pDb->getExtMin() );
  writeFieldValue( "High", pDb->getExtMax() );
  writeFieldValue( "Is persistent", pDb->isPersistent() );

  //get all models
  {
    //writeShift();
    //fwprintf( DumpStream, L"Number of models in the database: %I32u\n", number );

    OdDgModelTablePtr pModelTable = pDb->getModelTable();
    if (!pModelTable.isNull())
    {
      OdDgElementIteratorPtr pIter = pModelTable->createIterator();
      for ( ; !pIter->done(); pIter->step() )
      {
        OdDgModelPtr pModel = OdDgModel::cast( pIter->item().openObject() );
        if ( pModel.get() )
        {
          writeShift();
          fwprintf( DumpStream, L"Model #%I32u\n", pModel->getEntryId() );        

          OdSmartPtr< OdDgModel_Dumper > pModelDumper = pModel;
          pModelDumper->dump( pModel.get() );
        }
      }
    }
  }

  finishDescription();
}


OdString OdDgDatabase_Dumper::getClassName()
{
  return "OdDgDatabase";
}


void OdDgModel_Dumper::dump( OdRxObjectPtr object )
{
  OdDgModelPtr element = object;

  startDescription( object );

  writeFieldValue( "Working unit", element->getWorkingUnit() );
 
  //describes all graphics elements
  describeSubElements( element->createGraphicsElementsIterator() );
  //describes all non-graphics elements
  describeSubElements( element->createControlElementsIterator() );

  finishDescription();
}


OdString OdDgModel_Dumper::getClassName()
{
  return "OdDgModel";
}


void OdDgReferenceHeader_Dumper::dump( OdRxObjectPtr object )
{
  OdDgReferenceAttachmentHeaderPtr  element = object;

  startDescription( object );

  writeFieldValue( "The full reference path", element->getFullFileName() );
  writeFieldValue( "The base file name", element->getFileName() );
  writeFieldValue( "ModelName", element->getModelName() );
  writeFieldValue( "LogicalName", element->getLogicalName() );
  writeFieldValue( "MasterOrigin", element->getMasterOrigin() );
  writeFieldValue( "ReferenceOrigin", element->getReferenceOrigin() );
  writeFieldValue( "Transformation", element->getTransformation() );

  writeFieldValue( "Description", element->getDescription() );
  writeFieldValue( "FileNumber", element->getFileNumber() );
  writeFieldValue( "Priority", element->getPriority() );
  writeFieldValue( "BaseNestDepth", element->getBaseNestDepth() );
  writeFieldValue( "NestDepth", element->getNestDepth() );
  writeFieldValue( "Scale", element->getScale() );
  writeFieldValue( "ZFront", element->getZFront() );
  writeFieldValue( "ZBack", element->getZBack() );
  writeFieldValue( "CameraPosition", element->getCameraPosition() );
  writeFieldValue( "CameraFocalLength", element->getCameraFocalLength() );

  writeFieldValue( "ClipPointsCount", element->getClipPointsCount() );
  char  name[ 20 ];
  for (OdUInt32 i = 0, nCount = element->getClipPointsCount(); i < nCount; i++)
  {
    writeShift();
    sprintf( name, "ClipPoint %d", i );
    writeFieldValue( name, element->getClipPoint( i ) );
  }

  writeFieldValue( "CoincidentFlag", element->getCoincidentFlag() );
  writeFieldValue( "SnapLockFlag", element->getSnapLockFlag() );
  writeFieldValue( "LocateLockFlag", element->getLocateLockFlag() );
  writeFieldValue( "CompletePathInV7Flag", element->getCompletePathInV7Flag() );
  writeFieldValue( "AnonymousFlag", element->getAnonymousFlag() );
  writeFieldValue( "InactiveFlag", element->getInactiveFlag() );
  writeFieldValue( "MissingFileFlag", element->getMissingFileFlag() );
  writeFieldValue( "LevelOverride", element->getLevelOverride() );
  writeFieldValue( "DontDetachOnAllFlag", element->getDontDetachOnAllFlag() );
  writeFieldValue( "MetadataOnlyFlag", element->getMetadataOnlyFlag() );
  writeFieldValue( "DisplayFlag", element->getDisplayFlag() );
  writeFieldValue( "LineStyleScaleFlag", element->getLineStyleScaleFlag() );
  writeFieldValue( "HiddenLineFlag", element->getHiddenLineFlag() );
  writeFieldValue( "DisplayHiddenLinesFlag", element->getDisplayHiddenLinesFlag() );
  writeFieldValue( "RotateClippingFlag", element->getRotateClippingFlag() );
  writeFieldValue( "ExtendedRefFlag", element->getExtendedRefFlag() );
  writeFieldValue( "ClipBackFlag", element->getClipBackFlag() );
  writeFieldValue( "ClipFrontFlag", element->getClipFrontFlag() );
  writeFieldValue( "CameraOnFlag", element->getCameraOnFlag() );
  writeFieldValue( "TrueScaleFlag", element->getTrueScaleFlag() );
  writeFieldValue( "DisplayBoundaryFlag", element->getDisplayBoundaryFlag() );
  writeFieldValue( "LibraryRefFlag", element->getLibraryRefFlag() );
  writeFieldValue( "DisplayRasterRefsFlag", element->getDisplayRasterRefsFlag() );
  writeFieldValue( "UseAlternateFileFlag", element->getUseAlternateFileFlag() );
  writeFieldValue( "UseLightsFlag", element->getUseLightsFlag() );
  writeFieldValue( "DoNotDisplayAsNestedFlag", element->getDoNotDisplayAsNestedFlag() );
  writeFieldValue( "ColorTableUsage", element->getColorTableUsage() );
  writeFieldValue( "ViewportFlag", element->getViewportFlag() );
  writeFieldValue( "ScaleByStorageUnitsFlag", element->getScaleByStorageUnitsFlag() );
  writeFieldValue( "PrintColorAdjustmentFlag", element->getPrintColorAdjustmentFlag() );

  // Dumps the Reference Attachment itself.
  //
  // Returns database for the reference. Use 'Model' to specify the reference in database.
  //
  // NOTE: The 'Model' value is empty for 'Default' model.
  //       The logical name helps to distinguish references from each other.

  OdRxObjectPtr pRxObject = element->getReferencedDatabase();

  OdDgDatabasePtr pDgnXRef = dynamic_cast<OdDgDatabase*>(pRxObject.get());
  if (!pDgnXRef.isNull())
  {
    fwprintf( DumpStream, L">> Received DGN Reference Attachment" );
  }
  else
  {
    OdDbDatabasePtr pTdXRef = dynamic_cast<OdDbDatabase*>(pRxObject.get());
    if (!pTdXRef.isNull())
    {
      fwprintf( DumpStream, L">> Received .dwg file Reference Attachment" );

      /********************************************************************/
      /* Fill the database                                                */
      /*                                                                  */
      /* Some custom action on file could be here                         */
      /* instead of the code below                                        */
      /********************************************************************/
      {
        OdDbObjectId msId = pTdXRef->getModelSpaceId();
        OdDbBlockTableRecordPtr pMs = msId.safeOpenObject(OdDb::kForWrite);

        // Create a new line
        OdDbLinePtr pLine = OdDbLine::createObject();
        pLine->setStartPoint( OdGePoint3d( 0., 0., 0. ) );
        pLine->setEndPoint( OdGePoint3d( 20., 20., 20. ) );
        OdDgCmColor col;
        col.setRGB(255, 255, 0);
        pLine->setColor( col );

        // Add the line to the ModelSpace
        pMs->appendOdDbEntity(pLine);
      }

      /********************************************************************/
      /* Write the database                                               */
      /********************************************************************/
      OdString sDwgXRefFileName = pTdXRef->getFilename();
      pTdXRef->writeFile(sDwgXRefFileName, OdDb::kDwg, OdDb::kDHL_CURRENT, true);

    }
    else
    {
      fwprintf( DumpStream, L">> The Reference Attachment is not found" );
    }
  }

  OdDgLevelTablePtr pLevelTable = element->getLevelTable();
  if ( pLevelTable.get() )
  {
    fwprintf( DumpStream, L"There is a level table for Xref\n" );
    for ( OdDgElementIteratorPtr iterator = pLevelTable->createIterator(); !iterator->done(); iterator->step() )
    {
      OdDgLevelTableRecordPtr pRecord = OdDgLevelTableRecord::cast( iterator->item().openObject() );
      if ( !pRecord.isNull() )
      {
        writeFieldValue( "Level Name ", pRecord->getName() );
        writeFieldValue( "Level Number", pRecord->getNumber() );
        writeFieldValue( "Level Color Index", pRecord->getElementColorIndex() );
        writeFieldValue( "Entry ID", pRecord->getEntryId() );
      }
    }
  } 
  else 
  {
    fwprintf( DumpStream, L"There is no a level table for Xref\n" );
  }

  finishDescription();
}

OdString OdDgReferenceHeader_Dumper::getClassName()
{
  return "OdDgReferenceAttachmentHeader";
}
