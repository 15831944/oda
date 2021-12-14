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
#include "ExDgnDumpElements.h"
#include "DgAttributeLinkage.h"
#include "DgSummaryInfo.h"
#include "RxDictionary.h"
#include "DgLevelMask.h"
#include "DgLevelFilterTableRecord.h"

//
// OdRxObject_Dumper
//


ODRX_NO_CONS_DEFINE_MEMBERS( OdRxObject_Dumper, OdRxObject )

FILE* DumpStream;


int                 OdRxObject_Dumper::m_nesting;
OdArray< OdString > OdRxObject_Dumper::m_nestedTypes;


OdRxObject_Dumper::OdRxObject_Dumper()
: m_references( 0 )
{
}


void OdRxObject_Dumper::startDescription( OdRxObjectPtr object )
{
  writeShift();
  m_object = object;
  //write it name
  OdString className;
  {
    OdSmartPtr< OdRxObject_Dumper > dumper = object;

    className = dumper->getClassName();
    fwprintf( DumpStream, L"> %ls", className.c_str() );
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

  //dump specific information
  writeLinkagesInfo( m_object );
  writeElementInfo( m_object );
  writeGraphicsElementInfo( m_object );
}


void OdRxObject_Dumper::finishDescription()
{
  m_nesting--;
 
  writeShift();
  fwprintf( DumpStream, L"< %ls\n", m_nestedTypes.last().c_str() );

  m_nestedTypes.removeLast();
}


void OdRxObject_Dumper::describeSubElements( OdDgElementIteratorPtr iterator )
{
  if( iterator.isNull() )
    return;

  int           counter = 0;
  OdRxObjectPtr object;

  for( ; !iterator->done(); iterator->step(), counter++ )
  {
    writeShift();
    fwprintf( DumpStream, L"Sub element %d:\n", counter );
    describeSubElement( iterator->item().openObject( OdDg::kForRead ) );
  }
}

void OdRxObject_Dumper::describeSubElement( OdRxObjectPtr someObject )
{
  OdSmartPtr<OdRxObject_Dumper> pDumper = OdRxObject_Dumper::cast( someObject );
  if ( !pDumper.isNull() )
  {
    pDumper->dump( someObject );
  }
  else
  {
    m_nesting++;
    fwprintf( DumpStream, L"Object does not have its dumper\n" );
    writeElementInfo( someObject );
    m_nesting--;
  }
}

void OdRxObject_Dumper::writeShift()
{
  int i;

  for( i = 0; i < m_nesting; i++ )
  {
    fwprintf( DumpStream, L"  " );
  }
}

void OdRxObject_Dumper::writeElementInfo( OdRxObjectPtr object )
{
  OdDgElementPtr element = OdDgElement::cast( object );
  if( element.isNull() )
  {
    return;
  }

  writeShift();
  fwprintf( DumpStream, L"Common information for DGN elements:\n" );
  m_nesting++;

  writeFieldValue( "ID", element->elementId().getHandle().ascii() );
  writeFieldValue( "Type", int( element->getElementType() ) );
  if (element->isDBRO())
  {
    writeFieldValue( "Is Locked", element->getLockedFlag() );
  }

  m_nesting--;
}

void OdRxObject_Dumper::writeGraphicsElementInfo( OdRxObjectPtr object )
{
  OdDgGraphicsElementPtr element = OdDgGraphicsElement::cast( object );
  if( element.isNull() )
  {
    return;
  }

  writeShift();
  fwprintf( DumpStream, L"Common information for Graphics Elements:\n" );
  m_nesting++;

  OdUInt32 level = element->getLevelEntryId();
  OdDgElementId id = element->database()->getLevelTable()->getAt( level );
  writeShift();
  if (id.isNull())
  {
    fwprintf( DumpStream, L"level id is incorrect\n" );
  }
  else
  {
    fwprintf( DumpStream, L"level id is correct\n" );
  }

  writeFieldValue( "Level ID", element->getLevelEntryId() );

  OdUInt32 uColorIndex = element->getColorIndex();

  if( (uColorIndex != OdDg::kColorByLevel) && (uColorIndex != OdDg::kColorByCell) )
  {
    writeFieldValue( "Color", element->getColor());
    writeFieldValueColorref( "Color", element->getColor());
  }

  writeFieldValue( "Color index", uColorIndex );

  switch( uColorIndex )
  {
    case OdDg::kColorByLevel : fwprintf( DumpStream, L"Color is defined by level\n" ); break;
    case OdDg::kColorByCell: fwprintf( DumpStream, L"Color is defined by cell\n" ); break;
  }

  writeFieldValue( "Graphics group", element->getGraphicsGroupEntryId() );
  writeFieldValue( "Class", element->getClass() );
  writeFieldValue( "Line style", element->getLineStyleEntryId() );
  if (element->getLineStyleEntryId() == OdDg::kLineStyleByLevel)
  {
    fwprintf( DumpStream, L"Element has LineStyleByLevel.\n" );
  }
  writeFieldValue( "Line weight", element->getLineWeight() );
  if (element->getLineWeight() == OdDg::kLineWeightByLevel)
  {
    fwprintf( DumpStream, L"Element has WeightByLevel.\n" );
  }

  writeFieldValue( "Is Invisible", element->getInvisibleFlag() );
  writeFieldValue( "Is 3D Format Element", element->get3dFormatFlag() );
  writeFieldValue( "View Independent", element->getViewIndependentFlag() );
  writeFieldValue( "Non Planar", element->getNonPlanarFlag() );
  writeFieldValue( "Not Snappable", element->getNotSnappableFlag() );

  // OdDgGraphicsElement::getHbitFlag() method replaced OdDgGraphicsElement::getHoleFlag(). 
  //  It is because an interpretation of H-bit value depends upon element type.
  //
  //  H-bit indicates whether the element is a solid or a hole for closed element types (
  //  shape, complex shape, ellipse, cone, B-spline surface header, and closed B-spline curve header):
  //    • 0 = Solid
  //    • 1 = Hole
  //  So there are OdDgBSplineSurface::getHoleFlag() and OdDgCone::getHoleFlag().
  //
  //  For a cell header if the H-bit is:
  //    • 0 = Header for a cell
  //    • 1 = Header for an orphan cell (created by group command or application) 
  //
  //  For a point string element (OdDgPointString2d/OdDgPointString3d) if the H-bit is:
  //    • 0 = Continuous
  //    • 1 = Disjoint
  //  So there are OdDgPointString2d::getContinuousFlag() and OdDgPointString3d::getContinuousFlag().
  //
  //  The H-bit has no meaning in other elements.
  //
  writeFieldValue( "Hbit", element->getHbitFlag() );

  //show the extents: recalculated one & saved one
  {
    OdGeExtents3d recalculatedExtent, savedExtent;

    //recalculated
    {
      if( element->getGeomExtents( recalculatedExtent ) == eOk )
      {
        writeFieldValue( "Result of getGeomExtents()", recalculatedExtent );
      }
      else
      {
        writeFieldValue( "Result of getGeomExtents()", "invalid value" );
      }
    }
    //saved
    OdDgGraphicsElementPEPtr pElementPE = OdDgGraphicsElementPEPtr(OdRxObjectPtr(element));
    if( !pElementPE.isNull() )
    {
      if( pElementPE->getRange( element, savedExtent ) == eOk )
      {
        writeFieldValue( "Saved extents", savedExtent );
      }
      else
      {
        writeFieldValue( "Saved extents", "invalid value" );
      }
    }
  }

  m_nesting--;
}


void OdRxObject_Dumper::writeLinkagesInfo( OdRxObjectPtr object )
{
  OdDgElementPtr element = OdDgElement::cast( object );

  if( element.isNull() )
  {
    return;
  }

  //take all linkages
  OdRxObjectPtrArray linkages;
  element->getLinkages( linkages );
  int linkagesNumber = linkages.size();

  if( linkagesNumber )
  {
    writeShift();
    fwprintf( DumpStream, L"> Attribute Linkages (%d items)\n", linkagesNumber );
    m_nesting++;

    for ( int i = 0; i < linkagesNumber; ++i)
    {
      OdDgAttributeLinkagePtr pLinkage = linkages[i];
	 
      OdBinaryData data;
      pLinkage->getData(data);
      writeShift(); 
      fwprintf( DumpStream, L"Primary ID = 0x%x, data size = %d", pLinkage->getPrimaryId(), data.size() );	  

      //additionary info depending on the type
		  switch( pLinkage->getPrimaryId() )
      {
        case OdDgAttributeLinkage::kHatch:
        {
          OdDgPatternLinkagePtr pPatternLinkage = OdDgPatternLinkage::cast( pLinkage );
          if ( !pPatternLinkage.isNull() )
          {
            OdDgHatchPatternPtr pHatchPattern = pPatternLinkage->getHatchPattern();

            if( !pHatchPattern.isNull() )
            {
              OdString namedType;

              switch( pHatchPattern->getType() )
              {
                case OdDgHatchPattern::kLinearPattern : namedType = "LinearPattern"; break;
                case OdDgHatchPattern::kCrossPattern : namedType = "CrossPattern"; break;
                case OdDgHatchPattern::kSymbolPattern : namedType = "SymbolPattern"; break;
                case OdDgHatchPattern::kDWGPattern : namedType = "DWGPattern"; break;
                default : namedType = "Unknown"; break;
              }

              fwprintf( DumpStream, L" ( Pattern type = %ls", namedType.c_str());

              if( pHatchPattern->getUseOffsetFlag() )
              {
                OdGePoint3d offset;
                pHatchPattern->getOffset( offset );
                fwprintf( DumpStream, L"; offset = (%g %g %g)", offset.x, offset.y, offset.z );
              }

              fwprintf( DumpStream, L")" );
            }
          }
        }
        break;
      case OdDgAttributeLinkage::kThickness:
        {
          OdDgThicknessLinkagePtr pThicknessLinkage = OdDgThicknessLinkage::cast( pLinkage );

          if ( !pThicknessLinkage.isNull() )
          {
            fwprintf( DumpStream, L" ( Thickness Linkage, thickness = %f )", pThicknessLinkage->getThickness() );
          }
        }
        break;
      case OdDgAttributeLinkage::kInfiniteLine:
        {
          OdDgInfiniteLineLinkagePtr pInfiniteLineLinkage = OdDgInfiniteLineLinkage::cast( pLinkage );

          if ( !pInfiniteLineLinkage.isNull() )
          {
            fwprintf( DumpStream, L" ( Infinite Line type, value = %d )", (OdUInt32)pInfiniteLineLinkage->getInfiniteLineType() );
          }
        }
        break;
      case OdDgAttributeLinkage::kTextAnnotation:
        {
          OdDgTextAnnotationLinkagePtr pTextAnnotationLinkage = OdDgTextAnnotationLinkage::cast( pLinkage );

          if ( !pTextAnnotationLinkage.isNull() )
          {
            fwprintf( DumpStream, L" ( Text annotation scale = %f )", pTextAnnotationLinkage->getAnnotationScale() );
          }
        }
        break;
      case OdDgAttributeLinkage::kString:
        {
          OdDgStringLinkagePtr pStrLinkage = OdDgStringLinkage::cast( pLinkage );

          if ( !pStrLinkage.isNull() )
          {
            fwprintf( DumpStream, L" ( String Linkage, ID = %d; value = \"%ls\")", pStrLinkage->getStringId(), pStrLinkage->getString().c_str());
          }
        }
        break;
      case OdDgAttributeLinkage::kDMRS:
        {
          OdDgDMRSLinkagePtr DMRSLinkage = OdDgDMRSLinkage::cast( pLinkage );
          if( !DMRSLinkage.isNull() )
          {
            fwprintf(
              DumpStream, L" ( DMRS Linkage, tableId = %d, MSLink = %d, type = %d )",
              int( DMRSLinkage->getTableId() ), int( DMRSLinkage->getMSLink() ), int( DMRSLinkage->getType() ) );
          }        
        }
        break;
      case 0x56D5:
        {
          OdDgProxyLinkagePtr linkage = OdDgProxyLinkage::cast( pLinkage );
          if( !linkage.isNull() )
          {
            OdBinaryData data;
            linkage->getData( data );
            OdUInt32 i, j = data.size();
            fwprintf( DumpStream, L" ( Proxy linkage )\n" );
            m_nesting++;
            writeFieldValue( "Size of the proxy linkage", j );
            for( i = 0; i < j; i++ )
            {
              if( !( i % 16 ) )
              {
                if( i )
                {
                  fwprintf( DumpStream, L"\n" );
                }
                writeShift();
                fwprintf( DumpStream, L"%.4X: ", i );
              }
              fwprintf( DumpStream, L"%.2X ", data[ i ] );
            }
            m_nesting--;
          }
        }
        break;
      case OdDgAttributeLinkage::kFRAMME    : // DB Linkage - FRAMME tag data signature
      case OdDgAttributeLinkage::kBSI       : // DB Linkage - secondary id link (BSI radix 50)
      case OdDgAttributeLinkage::kXBASE     : // DB Linkage - XBase (DBase)
      case OdDgAttributeLinkage::kINFORMIX  : // DB Linkage - Informix
      case OdDgAttributeLinkage::kINGRES    : // DB Linkage - INGRES
      case OdDgAttributeLinkage::kSYBASE    : // DB Linkage - Sybase
      case OdDgAttributeLinkage::kODBC      : // DB Linkage - ODBC
      case OdDgAttributeLinkage::kOLEDB     : // DB Linkage - OLEDB
      case OdDgAttributeLinkage::kORACLE    : // DB Linkage - Oracle
      case OdDgAttributeLinkage::kRIS       : // DB Linkage - RIS
        {
          OdDgDBLinkagePtr dbLinkage = OdDgDBLinkage::cast( pLinkage );
          if( !dbLinkage.isNull() )
          {
            OdString namedType;

            switch( dbLinkage->getDBType() )
            {
            case OdDgDBLinkage::kBSI : namedType = "BSI"; break;
            case OdDgDBLinkage::kFRAMME : namedType = "FRAMME"; break;
            case OdDgDBLinkage::kInformix : namedType = "Informix"; break;
            case OdDgDBLinkage::kIngres : namedType = "Ingres"; break;
            case OdDgDBLinkage::kODBC : namedType = "ODBC"; break;
            case OdDgDBLinkage::kOLEDB : namedType = "OLE DB"; break;
            case OdDgDBLinkage::kOracle : namedType = "Oracle"; break;
            case OdDgDBLinkage::kRIS : namedType = "RIS"; break;
            case OdDgDBLinkage::kSybase : namedType = "Sybase"; break;
            case OdDgDBLinkage::kXbase : namedType = "xBase"; break;
            default : namedType = "Unknown"; break;
            }

            fwprintf(
              DumpStream, L" ( DB Linkage, tableId = %d, MSLink = %d, type = %ls )",
              int( dbLinkage->getTableEntityId() ), int( dbLinkage->getMSLink() ), namedType.c_str() );
          }
        }
        break;
      case OdDgAttributeLinkage::kDimension       : // Dimension Linkage
        {
          OdDgDimensionLinkagePtr dimLinkage = OdDgDimensionLinkage::cast( pLinkage );
          if( !dimLinkage.isNull() )
          {
            OdString sDimType;
            switch( dimLinkage->getType() )
            {
              case OdDgDimensionLinkage::kOverall : sDimType = "Overall"; break;
              case OdDgDimensionLinkage::kSegment : sDimType = "Segment"; break;
              case OdDgDimensionLinkage::kPoint : sDimType = "Point"; break;
              case OdDgDimensionLinkage::kSegmentFlags : sDimType = "SegmentFlags"; break;
              case OdDgDimensionLinkage::kDimensionInfo : sDimType = "DimensionInfo"; break;
              default: sDimType = "Unkown"; break;
            }
            fwprintf( DumpStream, L" ( Dimension Linkage, type = %ls )", sDimType.c_str() );
  
            switch( dimLinkage->getType() )
            {
            case OdDgDimensionLinkage::kOverall: 
              break;
            case OdDgDimensionLinkage::kSegment: 
              break;
            case OdDgDimensionLinkage::kPoint: 
              break;
            case OdDgDimensionLinkage::kSegmentFlags: 
              break;
            case OdDgDimensionLinkage::kDimensionInfo: 
            {
               OdDgDimensionInfoLinkagePtr pDimInfoLinkage = pLinkage;

               if( pDimInfoLinkage->getUseAnnotationScaleValue() )
                  writeFieldValue( "  Annotation Scale", pDimInfoLinkage->getAnnotationScale() );

               if( pDimInfoLinkage->getUseDatumValue() )
               {
                 double dDatumValue = pDimInfoLinkage->getDatumValue();

                 OdDgDatabase* pDb = element->database();
                 OdDgElementId idModel;

                 if( pDb )
                   idModel = pDb->getActiveModelId();

                 if( !idModel.isNull() )
                 {
                   OdDgModelPtr pModel = OdDgModel::cast( idModel.openObject() );

                   if( !pModel.isNull() )
                   {
                     dDatumValue = pModel->convertUORsToWorkingUnits( dDatumValue );
                   }
                 }
                 else
                 {
                   dDatumValue /= DDLL(10000000000); // Storage units default factor
                 }

                 writeFieldValue( "  Datum Value", dDatumValue );
               }

               if( pDimInfoLinkage->getUseRetainFractionalAccuracy() )
               {
                 writeFieldValue( "  Detriment in reverse direction", pDimInfoLinkage->getUseDecrimentInReverceDirection() );
                 writeFieldValue( "  Primary retain fractional accuracy", pDimInfoLinkage->getPrimaryRetainFractionalAccuracy() );
                 writeFieldValue( "  Secondary retain fractional accuracy", pDimInfoLinkage->getSecondaryRetainFractionalAccuracy() );
                 writeFieldValue( "  Primary alt format retain fractional accuracy", pDimInfoLinkage->getPrimaryAltFormatRetainFractionalAccuracy() );
                 writeFieldValue( "  Secondary alt format retain fractional accuracy", pDimInfoLinkage->getSecondaryAltFormatRetainFractionalAccuracy() );
                 writeFieldValue( "  Primary tolerance retain fractional accuracy", pDimInfoLinkage->getPrimaryTolerRetainFractionalAccuracy() );
                 writeFieldValue( "  Secondary tolerance retain fractional accuracy", pDimInfoLinkage->getSecondaryTolerRetainFractionalAccuracy() );
                 writeFieldValue( "  Label line mode", pDimInfoLinkage->getLabelLineDimensionMode());
               }

               if( pDimInfoLinkage->getUseFitOptionsFlag() )
               {
                 writeFieldValue( "  Suppress unfit terminators", pDimInfoLinkage->getUseSuppressUnfitTerm() );
                 writeFieldValue( "  Use inline leader length", pDimInfoLinkage->getUseInlineLeaderLength() );
                 writeFieldValue( "  Text above optimal fit", pDimInfoLinkage->getUseTextAboveOptimalFit() );
                 writeFieldValue( "  Narrow font optimal fit", pDimInfoLinkage->getUseNarrowFontOptimalFit() );
                 writeFieldValue( "  Use Min Leader Terminator Length", pDimInfoLinkage->getUseMinLeaderTermLength() );
                 writeFieldValue( "  Use auto mode for dimension leader", pDimInfoLinkage->getUseAutoLeaderMode() );
                 writeFieldValue( "  Fit Options ", pDimInfoLinkage->getFitOptions() );
               }

               if( pDimInfoLinkage->getUseTextLocation() )
               {
                 writeFieldValue( "  Free location of text", pDimInfoLinkage->getUseFreeLocationText() ); 
                 writeFieldValue( "  Note spline fit", pDimInfoLinkage->getUseNoteSplineFit() );
                 writeFieldValue( "  Text location ", pDimInfoLinkage->getTextLocation() );
               }

               if( pDimInfoLinkage->getUseInlineLeaderLengthValue() )
               {
                 double dLengthValue = pDimInfoLinkage->getInlineLeaderLength();

                 OdDgDatabase* pDb = element->database();
                 OdDgElementId idModel;

                 if( pDb )
                   idModel = pDb->getActiveModelId();

                 if( !idModel.isNull() )
                 {
                   OdDgModelPtr pModel = OdDgModel::cast( idModel.openObject() );

                   if( !pModel.isNull() )
                   {
                     dLengthValue = pModel->convertUORsToWorkingUnits( dLengthValue );
                   }
                 }
                 else
                 {
                   dLengthValue /= DDLL(10000000000); // Storage units default factor
                 }

                 writeFieldValue( "  Inline leader length value", dLengthValue );
               }

               if( pDimInfoLinkage->getUseInlineTextLift() )
                 writeFieldValue( "  Inline text lift", pDimInfoLinkage->getInlineTextLift() );

               if( pDimInfoLinkage->getUseNoteFrameScale() )
                 writeFieldValue( "  Note frame scale", pDimInfoLinkage->getUseNoteFrameScale() );

               if( pDimInfoLinkage->getUseNoteLeaderLength() )
                 writeFieldValue( "  Note leader length", pDimInfoLinkage->getNoteLeaderLength() );

               if( pDimInfoLinkage->getUseNoteLeftMargin() )
                 writeFieldValue( "  Note left margin", pDimInfoLinkage->getUseNoteLeftMargin() );

               if( pDimInfoLinkage->getUseNoteLowerMargin() )
                 writeFieldValue( "  Note lower margin", pDimInfoLinkage->getUseNoteLowerMargin() );

               if( pDimInfoLinkage->getUsePrimaryToleranceAccuracy() )
                 writeFieldValue( "  Primary tolerance accuracy", pDimInfoLinkage->getPrimaryToleranceAccuracy() );

               if( pDimInfoLinkage->getUseSecondaryToleranceAccuracy() )
                 writeFieldValue( "  Secondary tolerance accuracy", pDimInfoLinkage->getSecondaryToleranceAccuracy() );

               if( pDimInfoLinkage->getUseStackedFractionScale() )
                 writeFieldValue( "  Stacked fraction scale", pDimInfoLinkage->getStackedFractionScale() );
             } break;
            default:
              break;
            }
          }
          break;
        }
        break;
      case OdDgAttributeLinkage::kFilterMember:
      {
        OdDgFilterMemberLinkagePtr pFilterLinkage = pLinkage;
        writeFieldValue( "  Member Id", pFilterLinkage->getMemberId() );
        writeFieldValue( "  Member Type", pFilterLinkage->getMemberType() );
        writeFieldValue( "  Name String", pFilterLinkage->getNameString() );
        writeFieldValue( "  Expression String", pFilterLinkage->getExpressionString() );
      } break;
      case OdDgAttributeLinkage::kDependency : 
        {
          OdDgDependencyLinkagePtr dependencyLinkage = OdDgDependencyLinkage::cast( pLinkage );
          if( !dependencyLinkage.isNull() )
          {
            fwprintf( DumpStream, L"( Root type = %d; App ID = %d; App Value = %d )",
              OdUInt16( dependencyLinkage->getRootDataType() ),
              dependencyLinkage->getAppValue(),
              dependencyLinkage->getAppValue() );

            //some additional information
            m_nesting++;
            switch( dependencyLinkage->getRootDataType() ) 
            {
            case OdDgDependencyLinkage::kElementId :
              OdDgDepLinkageElementIdPtr elementIdLinkage = OdDgDepLinkageElementId::cast( dependencyLinkage );
              if( !elementIdLinkage.isNull() )
              {
                OdUInt32 i, j = elementIdLinkage->getCount();
                fwprintf( DumpStream, L"\n" );
                m_nesting++;
                writeShift();
                fwprintf( DumpStream, L"Number of IDs: %d; They are:", int( j ) );
                for( i = 0; i < j; i++ )
                {
                  fwprintf( DumpStream, L" %08llX", elementIdLinkage->getAt( i ) );
                }
                m_nesting--;
              }
              break;
            }
            m_nesting--;
          }
        }
        break;
      }
      fwprintf( DumpStream, L"\n" );
    }

    m_nesting--;
    writeShift();
    fwprintf( DumpStream, L"< Attribute Linkages\n" );
  }
}

void OdRxObject_Dumper::writeFieldValueColorref( const OdString & name, ODCOLORREF value )
{
  writeFieldName( name );
  fwprintf( DumpStream, L"Red: %u Green: %u Blue: %u\n", ODGETRED(value), ODGETGREEN(value), ODGETBLUE(value));
}


void OdRxObject_Dumper::writeFieldValue( const OdString& name, const OdString& value )
{
  writeFieldName( name );
  fwprintf( DumpStream, L"%ls\n", value.c_str() );
}


void OdRxObject_Dumper::writeFieldValue( const OdString& name, OdUInt16 value )
{
  writeFieldName( name );
  fwprintf( DumpStream, L"%u\n", OdUInt32( value ) );
}


void OdRxObject_Dumper::writeFieldValueHex( const OdString& name, OdUInt16 value )
{
  writeFieldName( name );
  fwprintf( DumpStream, L"%04X\n", OdUInt32( value ) );
}


void OdRxObject_Dumper::writeFieldValue( const OdString& name, OdUInt32 value )
{
  writeFieldName( name );
  fwprintf( DumpStream, L"%u\n", value );
}

void OdRxObject_Dumper::writeFieldValue( const OdString& name, OdDgLevelFilterTable::OdDgFilterMemberType value )
{
  OdString strValue;

  switch( value )
  {
    case OdDgLevelFilterTable::kMemberTypeShort :
    {
      strValue = OD_T("kTypeShort");
    } break;

    case OdDgLevelFilterTable::kMemberTypeInt :
    {
      strValue = OD_T("kTypeInt");
    } break;

    case OdDgLevelFilterTable::kMemberTypeBool :
    {
      strValue = OD_T("kTypeBool");
    } break;

    case OdDgLevelFilterTable::kMemberTypeChar :
    {
      strValue = OD_T("kTypeChar");
    } break;

    case OdDgLevelFilterTable::kMemberTypeWChar :
    {
      strValue = OD_T("kTypeWChar");
    } break;

    case OdDgLevelFilterTable::kMemberTypeDouble :
    {
      strValue = OD_T("kTypeDouble");
    } break;

    case OdDgLevelFilterTable::kMemberTypeTime :
    {
      strValue = OD_T("kTypeTime");
    } break;

    default:
    {
      strValue = OD_T("kTypeNull");
    } break;
  }

  writeFieldValue(name, strValue);
}

void OdRxObject_Dumper::writeFieldValueHex( const OdString& name, OdUInt32 value )
{
  writeFieldName( name );
  fwprintf( DumpStream, L"%08X\n", value );
}


void OdRxObject_Dumper::writeFieldValue( const OdString& name, double value )
{
  writeFieldName( name );
  fwprintf( DumpStream, L"%f\n", value );
}


void OdRxObject_Dumper::writeFieldValue( const OdString& name, OdGePoint2d const& value )
{
  writeFieldName( name );
  fwprintf( DumpStream, L"%g; %g\n", value.x, value.y );
}


void OdRxObject_Dumper::writeFieldValue( const OdString& name, OdGePoint3d const& value )
{
  writeFieldName( name );
  fwprintf( DumpStream, L"%g; %g; %g\n", value.x, value.y, value.z );
}

void OdRxObject_Dumper::writeFieldValue( const OdString& name, OdGeVector2d const& value )
{
  writeFieldName( name );
  fwprintf( DumpStream, L"%g; %g\n", value.x, value.y );
}

void OdRxObject_Dumper::writeFieldValue( const OdString& name, OdGeVector3d const& value )
{
  writeFieldName( name );
  fwprintf( DumpStream, L"%g; %g; %g\n", value.x, value.y, value.z );
}


void OdRxObject_Dumper::writeFieldValue( const OdString& name, OdCmEntityColor const& value )
{
  writeFieldName( name );
  fwprintf( DumpStream, L"R: %u G: %u B: %u\n",
    unsigned( value.red() ), unsigned( value.green() ), unsigned( value.blue() ) );
}


void OdRxObject_Dumper::writeFieldValue( const OdString& name, OdDgLineStyleInfo const& value )
{
  writeFieldName( name );
  fwprintf( DumpStream, L"Modifiers: %08X Scale: %g Shift: %g Start width: %g End width: %g\n",
    value.getModifiers(), value.getScale(), value.getShift(), value.getStartWidth(), value.getEndWidth() );
}


void OdRxObject_Dumper::writeFieldValue( const OdString& name, OdUInt64 value )
{
  writeFieldName( name );
  fwprintf( DumpStream, L"%llu\n", value );
}


void OdRxObject_Dumper::writeFieldValue( const OdString& name, OdUInt8 value )
{
  writeFieldName( name );
  fwprintf( DumpStream, L"%u\n", unsigned( value ) );
}

  
void OdRxObject_Dumper::writeFieldValueHex( const OdString& name, OdUInt8 value )
{
  writeFieldName( name );
  fwprintf( DumpStream, L"%02X\n", unsigned( value ) );
}


void OdRxObject_Dumper::writeFieldValue( const OdString& name, OdDgLightColor const& value )
{
  writeFieldName( name );
  fwprintf( DumpStream, L"R: %g G: %g B: %g Intensity: %g\n",
            value.getRed(), value.getGreen(), value.getBlue(), value.getIntensityScale() );
}


void OdRxObject_Dumper::writeFieldValue( const OdString& name, bool value )
{
  writeFieldName( name );
  fwprintf( DumpStream, value ? L"true\n" : L"false\n" );
}


void OdRxObject_Dumper::writeFieldValue( const OdString& name, OdAngleCoordinate const& value )
{
  writeFieldName( name );
  fwprintf(  DumpStream, L"%d%ls %u' %u\"\n",
            unsigned( value.getDegrees() ), (OdString("°")).c_str(), unsigned( value.getMinutes() ), unsigned( value.getSeconds() ) );
}


void OdRxObject_Dumper::writeFieldValue( const OdString& name, OdInt16 value )
{
  writeFieldName( name );
  fwprintf( DumpStream, L"%i\n", int( value ) );
}


void OdRxObject_Dumper::writeFieldValue( const OdString& name, OdInt32 value )
{
  writeFieldName( name );
  fwprintf( DumpStream, L"%I32i\n", int( value ) );
}

#if !(defined(ODA_UNIXOS) && (OD_SIZEOF_LONG == 8))
void OdRxObject_Dumper::writeFieldValue( const OdString& name, int value )
{
  writeFieldName( name );
  fwprintf( DumpStream, L"%I32i\n", value );
}
#endif

void OdRxObject_Dumper::writeFieldValue( const OdString& name, OdInt64 value )
{
  writeFieldName( name );
  fwprintf( DumpStream, L"%iI64\n", int( value ) );
}


void OdRxObject_Dumper::writeFieldValue( const OdString& name, OdDgModel::WorkingUnit value )
{
  writeFieldName( name );
  
  switch( value )
  {
  case OdDgModel::kWuUnitOfResolution : fwprintf( DumpStream, L"unit of resolution\n" ); break;
  case OdDgModel::kWuStorageUnit : fwprintf( DumpStream, L"storage unit\n" ); break;
  case OdDgModel::kWuWorldUnit : fwprintf( DumpStream, L"world unit\n" ); break;
  case OdDgModel::kWuMasterUnit : fwprintf( DumpStream, L"master unit\n" ); break;
  case OdDgModel::kWuSubUnit : fwprintf( DumpStream, L"sub unit\n" ); break;
  }
}


void OdRxObject_Dumper::writeFieldValue( const OdString& name, OdDg::TextJustification value )
{
  writeFieldName( name );

  switch( value )
  {
  case OdDg::kLeftTop          : fwprintf( DumpStream, L"left top\n" ); break;
  case OdDg::kLeftCenter       : fwprintf( DumpStream, L"left center\n" ); break;
  case OdDg::kLeftBottom       : fwprintf( DumpStream, L"left bottom\n" ); break;
  case OdDg::kLeftMarginTop    : fwprintf( DumpStream, L"left margin top\n" ); break;
  case OdDg::kLeftMarginCenter : fwprintf( DumpStream, L"left margin center\n" ); break;
  case OdDg::kLeftMarginBottom : fwprintf( DumpStream, L"left margin bottom\n" ); break;
  case OdDg::kCenterTop        : fwprintf( DumpStream, L"center top\n" ); break;
  case OdDg::kCenterCenter     : fwprintf( DumpStream, L"center center\n" ); break;
  case OdDg::kCenterBottom     : fwprintf( DumpStream, L"center bottom\n" ); break;
  case OdDg::kRightMarginTop   : fwprintf( DumpStream, L"right margin top\n" ); break;
  case OdDg::kRightMarginCenter : fwprintf( DumpStream, L"right margin center\n" ); break;
  case OdDg::kRightMarginBottom : fwprintf( DumpStream, L"right margin bottom\n" ); break;
  case OdDg::kRightTop          : fwprintf( DumpStream, L"right top\n" ); break;
  case OdDg::kRightCenter       : fwprintf( DumpStream, L"right center\n" ); break;
  case OdDg::kRightBottom       : fwprintf( DumpStream, L"right bottom\n" ); break;
  }
}


void OdRxObject_Dumper::writeFieldValue( const OdString& name, OdGeQuaternion const& value )
{
  writeFieldName( name );

  fwprintf( DumpStream, L"%g; %g; %g; %g\n", value.w, value.x, value.y, value.z );
}


#pragma MARKMESSAGE("TODO: Examples. TextAttributes interface was modified.");
//void OdRxObject_Dumper::writeFieldValue( const OdString& name, TextAttributes const& value )
//{
//  writeShift();
//  fwprintf( DumpStream, L"%ls:\n", name.c_str() );
//
//  m_nesting++;
//
//  writeFieldValue( "Is interchar", value.isInterchar );
//  writeFieldValue( "Interchar spacing", value.intercharSpacing );
//  writeFieldValue( "Is fixed width spacing", value.isFixedWidthSpacing );
//  writeFieldValue( "Is underlined", value.isUnderlined );
//  writeFieldValue( "Underline offset", value.underlineOffset );
//  writeFieldValue( "Is overlined", value.isOverlined );
//  writeFieldValue( "Overline offset", value.overlineOffset );
//  writeFieldValue( "Is slant", value.isSlant );
//  writeFieldValue( "Slang angle", value.slantAngle );
//  writeFieldValue( "Is vertical", value.isVertical );
//  writeFieldValue( "Is right-to-left", value.isRightToLeft );
//  writeFieldValue( "Codepage presents", value.isCodepagePresented );
//  writeFieldValue( "Codepage", value.codePage );
//  writeFieldValue( "Background presents", value.isBackgroundPresented );
//  writeFieldValue( "Background fill color", value.backgroundFillColor );
//  writeFieldValue( "Background border color", value.backgroundBorderColor );
//  writeFieldValue( "Background border style", value.backgroundBorderStyle );
//  writeFieldValue( "Background border weight", value.backgroundBorderWeight );
//  writeFieldValue( "Background border X", value.backgroundBorderX );
//  writeFieldValue( "Background border Y", value.backgroundBorderY );
//  writeFieldValue( "Is subscript", value.isSubscript );
//  writeFieldValue( "Is superscript", value.isSuperscript );
//  writeFieldValue( "Has text style", value.hasTextStyle );
//  writeFieldValue( "Text style", value.textStyle );
//  writeFieldValue( "Count of line breaks", value.lineBreakCount );
//  writeFieldValue( "Is bold", value.isBold );
//  writeFieldValue( "Has underline style", value.hasUnderlineStyle );
//  writeFieldValue( "Underline color", value.underlineColor );
//  writeFieldValue( "Underline style", value.underlineStyle );
//  writeFieldValue( "Underline weight", value.underlineWeight );
//  writeFieldValue( "Has overline style", value.hasOverlineStyle );
//  writeFieldValue( "Overline color", value.overlineColor );
//  writeFieldValue( "Overline style", value.overlineStyle );
//  writeFieldValue( "Overline weight", value.overlineWeight );
//  writeFieldValue( "Is word-wrap text node", value.isWordWrapTextNode );
//  writeFieldValue( "Is full justification", value.isFullJustification );
//  writeFieldValue( "Has color", value.hasColor );
//  writeFieldValue( "Color", value.color );
//  writeFieldValue( "Is ACAD interchar spacing", value.isAcadIntercharSpacing );
//  writeFieldValue( "Is backwards", value.isBackwards );
//  writeFieldValue( "Is upside-down", value.isUpsideDown );
//
//  m_nesting--;
//}


void OdRxObject_Dumper::writeFieldValue( const OdString& name, OdDgGraphicsElement::Class value )
{
  writeFieldName( name );

  switch( value )
  {
  case OdDgGraphicsElement::kClassPrimary          : fwprintf( DumpStream, L"Primary\n" ); break;
  case OdDgGraphicsElement::kClassPatternComponent : fwprintf( DumpStream, L"Pattern component\n" ); break;
  case OdDgGraphicsElement::kClassConstruction     : fwprintf( DumpStream, L"Construction\n" ); break;
  case OdDgGraphicsElement::kClassDimension        : fwprintf( DumpStream, L"Dimension\n" ); break;
  case OdDgGraphicsElement::kClassPrimaryRule      : fwprintf( DumpStream, L"Primary rule\n" ); break;
  case OdDgGraphicsElement::kClassLinearPatterned  : fwprintf( DumpStream, L"Linear patterned\n" ); break;
  case OdDgGraphicsElement::kClassConstructionRule : fwprintf( DumpStream, L"Construction rule\n" ); break;
  }
}


void OdRxObject_Dumper::writeFieldValue( const OdString& name, OdGeMatrix2d const& value )
{
  writeFieldName( name );

  fwprintf( DumpStream, L"%g; %g; %g; %g\n", value[ 0 ][ 0 ], value[ 1 ][ 0 ], value[ 0 ][ 1 ], value[ 1 ][ 1 ] );
}


void OdRxObject_Dumper::writeFieldValue( OdString const& name, OdDgDimension::ToolType value )
{
  writeFieldName( name );
  OdDgDimensionPtr pDim = OdDgDimension::cast( m_object );
  switch( value )
  {
  case OdDgDimension::kToolTypeInvalid             : fwprintf( DumpStream, L"Invalid\n" ); break;
  case OdDgDimension::kToolTypeSizeArrow           : fwprintf( DumpStream, L"Size arrow\n" ); break;
    //            m_nesting++;
    //pDim->getDimensionTool( tool );
    //if(static_cast<OdDgDimToolSizeArrow*>( &tool )->getArcSymbolFlag())
    //  fwprintf( DumpStream, L"Tool Arc Symbol Flag set\n" );
    //fwprintf( DumpStream, L"Tool Datum Value: %d\n", static_cast<OdDgDimToolOrdinate*>( &tool )->getDatumValue() );
    //if(static_cast<OdDgDimToolSizeArrow*>( &tool )->getStackExtLinesFlag())
    //  fwprintf( DumpStream, L"Tool StackExtLines Flag set\n" );
    //if(static_cast<OdDgDimToolSizeArrow*>( &tool )->getLeftExtLinesPresentFlag())
    //  fwprintf( DumpStream, L"Tool LeftExtLinesPresent Flag set\n" );
    //if(static_cast<OdDgDimToolSizeArrow*>( &tool )->getRightExtLinesPresentFlag())
    //  fwprintf( DumpStream, L"Tool RightExtLinesPresent Flag set\n" );
    //static_cast<OdDgDimToolSizeArrow*>( &tool )->getFirstTerminator();
    //switch(static_cast<OdDgDimToolSizeArrow*>( &tool )->getFirstTerminator())
    //{      //OdDgDimTool::TerminatorType     kTtNone           = 0,  kTtArrow          = 1,  kTtStroke         = 2,  kTtCircle         = 3,  kTtFilledCircle   = 4,
    //case OdDgDimTool::kTtNone:
    //  fprintf(DumpStream, "Tool First Terminator Type: kTtNone\n");
    //  break;
    //case OdDgDimTool::kTtArrow:
    //  fprintf(DumpStream, "Tool First Terminator Type: kTtArrow\n");
    //  break;
    //case OdDgDimTool::kTtStroke:
    //  fprintf(DumpStream, "Tool First Terminator Type: kTtStroke\n");
    //  break;
    //case OdDgDimTool::kTtCircle:
    //  fprintf(DumpStream, "Tool First Terminator Type: kTtCircle\n");
    //  break;
    //case OdDgDimTool::kTtFilledCircle:
    //  fprintf(DumpStream, "Tool First Terminator Type: kTtFilledCircle\n");
    //  break;
    //}
    ////static_cast<OdDgDimToolSizeArrow*>( &tool )->getJointTerminator();
    //switch(static_cast<OdDgDimToolSizeArrow*>( &tool )->getJointTerminator())
    //{      //OdDgDimTool::TerminatorType     kTtNone           = 0,  kTtArrow          = 1,  kTtStroke         = 2,  kTtCircle         = 3,  kTtFilledCircle   = 4,
    //case OdDgDimTool::kTtNone:
    //  fprintf(DumpStream, "Tool Joint Terminator Type: kTtNone\n");
    //  break;
    //case OdDgDimTool::kTtArrow:
    //  fprintf(DumpStream, "Tool Joint Terminator Type: kTtArrow\n");
    //  break;
    //case OdDgDimTool::kTtStroke:
    //  fprintf(DumpStream, "Tool Joint Terminator Type: kTtStroke\n");
    //  break;
    //case OdDgDimTool::kTtCircle:
    //  fprintf(DumpStream, "Tool Joint Terminator Type: kTtCircle\n");
    //  break;
    //case OdDgDimTool::kTtFilledCircle:
    //  fprintf(DumpStream, "Tool Joint Terminator Type: kTtFilledCircle\n");
    //  break;
    //}
    ////static_cast<OdDgDimToolSizeArrow*>( &tool )->getLeftTerminator();
    //switch(static_cast<OdDgDimToolSizeArrow*>( &tool )->getLeftTerminator())
    //{      //OdDgDimTool::TerminatorType     kTtNone           = 0,  kTtArrow          = 1,  kTtStroke         = 2,  kTtCircle         = 3,  kTtFilledCircle   = 4,
    //case OdDgDimTool::kTtNone:
    //  fprintf(DumpStream, "Tool Left Terminator Type: kTtNone\n");
    //  break;
    //case OdDgDimTool::kTtArrow:
    //  fprintf(DumpStream, "Tool Left Terminator Type: kTtArrow\n");
    //  break;
    //case OdDgDimTool::kTtStroke:
    //  fprintf(DumpStream, "Tool Left Terminator Type: kTtStroke\n");
    //  break;
    //case OdDgDimTool::kTtCircle:
    //  fprintf(DumpStream, "Tool Left Terminator Type: kTtCircle\n");
    //  break;
    //case OdDgDimTool::kTtFilledCircle:
    //  fprintf(DumpStream, "Tool Left Terminator Type: kTtFilledCircle\n");
    //  break;
    //}
    ////static_cast<OdDgDimToolSizeArrow*>( &tool )->getRightTerminator();
    //switch(static_cast<OdDgDimToolSizeArrow*>( &tool )->getRightTerminator())
    //{      //OdDgDimTool::TerminatorType     kTtNone           = 0,  kTtArrow          = 1,  kTtStroke         = 2,  kTtCircle         = 3,  kTtFilledCircle   = 4,
    //case OdDgDimTool::kTtNone:
    //  fprintf(DumpStream, "Tool Right Terminator Type: kTtNone\n");
    //  break;
    //case OdDgDimTool::kTtArrow:
    //  fprintf(DumpStream, "Tool Right Terminator Type: kTtArrow\n");
    //  break;
    //case OdDgDimTool::kTtStroke:
    //  fprintf(DumpStream, "Tool Right Terminator Type: kTtStroke\n");
    //  break;
    //case OdDgDimTool::kTtCircle:
    //  fprintf(DumpStream, "Tool Right Terminator Type: kTtCircle\n");
    //  break;
    //case OdDgDimTool::kTtFilledCircle:
    //  fprintf(DumpStream, "Tool Right Terminator Type: kTtFilledCircle\n");
    //  break;
    //}
    ////static_cast<OdDgDimToolSizeArrow*>( &tool )->getPrefix();
    //switch(static_cast<OdDgDimToolSizeArrow*>( &tool )->getPrefix())
    //{      //OdDgDimTool::CustomSymbol       kCsNone         = 0    , kCsDiameter     = 1    , kCsRadius       = 2    , kCsSquare       = 3    , kCsSR           = 4    , kCsSDiameter    = 5
    //case OdDgDimTool::kCsNone:
    //  fprintf(DumpStream, "Tool Prefix: kCsNone\n");
    //  break;
    //case OdDgDimTool::kCsDiameter:
    //  fprintf(DumpStream, "Tool Prefix: kCsDiameter\n");
    //  break;
    //case OdDgDimTool::kCsRadius:
    //  fprintf(DumpStream, "Tool Prefix: kCsRadius\n");
    //  break;
    //case OdDgDimTool::kCsSquare:
    //  fprintf(DumpStream, "Tool Prefix: kCsSquare\n");
    //  break;
    //case OdDgDimTool::kCsSR:
    //  fprintf(DumpStream, "Tool Prefix: kCsSR\n");
    //  break;
    //case OdDgDimTool::kCsSDiameter:
    //  fprintf(DumpStream, "Tool Prefix: kCsSDiameter\n");
    //  break;
    //}
    ////static_cast<OdDgDimToolSizeArrow*>( &tool )->getSuffix();
    //switch(static_cast<OdDgDimToolSizeArrow*>( &tool )->getSuffix())
    //{      //OdDgDimTool::CustomSymbol       kCsNone         = 0    , kCsDiameter     = 1    , kCsRadius       = 2    , kCsSquare       = 3    , kCsSR           = 4    , kCsSDiameter    = 5
    //case OdDgDimTool::kCsNone:
    //  fprintf(DumpStream, "Tool Suffix: kCsNone\n");
    //  break;
    //case OdDgDimTool::kCsDiameter:
    //  fprintf(DumpStream, "Tool Suffix: kCsDiameter\n");
    //  break;
    //case OdDgDimTool::kCsRadius:
    //  fprintf(DumpStream, "Tool Suffix: kCsRadius\n");
    //  break;
    //case OdDgDimTool::kCsSquare:
    //  fprintf(DumpStream, "Tool Suffix: kCsSquare\n");
    //  break;
    //case OdDgDimTool::kCsSR:
    //  fprintf(DumpStream, "Tool Suffix: kCsSR\n");
    //  break;
    //case OdDgDimTool::kCsSDiameter:
    //  fprintf(DumpStream, "Tool Suffix: kCsSDiameter\n");
    //  break;
    //}
    ////static_cast<OdDgDimToolSizeArrow*>( &tool )->getTextType();
    //switch(static_cast<OdDgDimToolSizeArrow*>( &tool )->getTextType())
    //{      //kStandard = 0    , kVertical = 1    , kMixed    = 2
    //case OdDgDimTool::kStandard:
    //  fprintf(DumpStream, "Tool Text Type: kStandard\n");
    //  break;
    //case OdDgDimTool::kVertical:
    //  fprintf(DumpStream, "Tool Text Type: kVertical\n");
    //  break;
    //case OdDgDimTool::kMixed:
    //  fprintf(DumpStream, "Tool Text Type: kMixed\n");
    //  break;
    //}
    //            m_nesting--;
    //break;
  case OdDgDimension::kToolTypeSizeStroke         : fwprintf( DumpStream, L"Size stroke\n" ); break;
  case OdDgDimension::kToolTypeLocateSingle       : fwprintf( DumpStream, L"Locate single\n" ); break;
  case OdDgDimension::kToolTypeLocateStacked      : fwprintf( DumpStream, L"Locate stacked\n" ); break;
  case OdDgDimension::kToolTypeAngleSize          : fwprintf( DumpStream, L"Angle size\n" ); break;
  case OdDgDimension::kToolTypeArcSize            : fwprintf( DumpStream, L"Arc size\n" ); break;
  case OdDgDimension::kToolTypeAngleLocation      : fwprintf( DumpStream, L"Angle location\n" ); break;
  case OdDgDimension::kToolTypeArcLocation        : fwprintf( DumpStream, L"Arc location\n" ); break;
  case OdDgDimension::kToolTypeAngleLines         : fwprintf( DumpStream, L"Angle lines\n" ); break;
  case OdDgDimension::kToolTypeAngleAxis          : fwprintf( DumpStream, L"Angle axis\n" ); break;
  case OdDgDimension::kToolTypeRadius             : fwprintf( DumpStream, L"Radius\n" ); break;
  case OdDgDimension::kToolTypeDiameter           : fwprintf( DumpStream, L"Diameter\n" ); break;
  case OdDgDimension::kToolTypeDiameterPara       : fwprintf( DumpStream, L"Diameter para\n" ); break;
  case OdDgDimension::kToolTypeDiameterPerp       : fwprintf( DumpStream, L"Diameter perp\n" ); break;
  case OdDgDimension::kToolTypeCustomLinear       : fwprintf( DumpStream, L"Custom linear\n" ); break;
  case OdDgDimension::kToolTypeOrdinate           : 
  {
    fwprintf( DumpStream, L"Ordinate\n" ); 

    OdDgDimOrdinatePtr pOrdinateDim = OdDgDimOrdinate::cast(pDim);

    if( !pOrdinateDim.isNull() )
    {
      m_nesting++;

      if(pOrdinateDim->getArcSymbolFlag())
        fwprintf( DumpStream, L"Tool Arc Symbol Flag set\n" );

      fwprintf( DumpStream, L"Tool Datum Value: %f\n", pOrdinateDim->getDatumValue() );

      if( pOrdinateDim->getDecrementInReverseDirectionFlag() )
        fwprintf( DumpStream, L"Tool DecrementInReverse Direction Flag set\n" );

      if( pOrdinateDim->getFreeLocationOfTxtFlag() )
        fwprintf( DumpStream, L"Tool FreeLocationOfTxt Flag set\n" );

      if( pOrdinateDim->getStackExtLinesFlag() )
        fwprintf( DumpStream, L"Tool StackExtLines Flag set\n" );

      m_nesting--;
    }
  } break;
  case OdDgDimension::kToolTypeRadiusExtended     : fwprintf( DumpStream, L"Radius extended\n" ); break;
  case OdDgDimension::kToolTypeDiameterExtended   : fwprintf( DumpStream, L"Diameter extended\n" ); break;
  case OdDgDimension::kToolTypeCenter              : fwprintf( DumpStream, L"Center\n" ); break;
  }
}


void OdRxObject_Dumper::writeFieldValue( const OdString& name, OdDgDimPoint& value )
{
  writeShift();
  fwprintf( DumpStream, L"%ls:\n", name.c_str() );

  m_nesting++;

  writeFieldValue( "  Point", value.getPoint() );
  writeFieldValue( "  Offset to dimension line", value.getOffsetToDimLine() );
  writeFieldValue( "  Offset Y", value.getOffsetY() );

  switch( value.getJustification() )
  {
    case OdDgDimTextInfo::kLeftText:
    {
      writeFieldValue("  Text alignment", L"kLeftText" );
    } break;

    case OdDgDimTextInfo::kCenterLeftText:
    {
      writeFieldValue("  Text alignment", OdString(L"kCenterLeftText") );
    } break;

    case OdDgDimTextInfo::kCenterRightText:
    {
      writeFieldValue("  Text alignment", OdString(L"kCenterRightText") );
    } break;

    case OdDgDimTextInfo::kRightText:
    {
      writeFieldValue("  Text alignment", OdString(L"kRightText") );
    } break;

    case OdDgDimTextInfo::kManualText:
    {
      writeFieldValue("  Text alignment", OdString(L"kManualText") );
    } break;
  }

  writeFieldName("  Flags:");
  writeFieldValue( "    Associative", value.getAssociativeFlag() );
  writeFieldValue( "    Relative", value.getRelative() != 0 );
  writeFieldValue( "    WitnessControlLocal", value.getWitnessControlLocalFlag() );
  writeFieldValue( "    NoWitnessLine", value.getNoWitnessLineFlag() );
  writeFieldValue( "    UseAltSymbology", value.getUseAltSymbologyFlag() );

  if( value.getPrimaryTextFlag() )
  {
    writeFieldValue( "  Primary text", value.getPrimaryText() );
  }

  if( value.getPrimaryTopToleranceTextFlag() )
  {
    writeFieldValue( "  Primary Top text", value.getPrimaryTopToleranceText() );
  }

  if( value.getPrimaryBottomToleranceTextFlag() )
  {
    writeFieldValue( "  Primary Bottom text", value.getPrimaryBottomToleranceText() );
  }

  if( value.getSecondaryTextFlag() )
  {
    writeFieldValue( "  Secondary text", value.getSecondaryText() );
  }

  if( value.getSecondaryTopToleranceTextFlag() )
  {
    writeFieldValue( "  Secondary Top text", value.getSecondaryTopToleranceText() );
  }

  if( value.getSecondaryBottomToleranceTextFlag() )
  {
    writeFieldValue( "  Secondary Bottom text", value.getSecondaryBottomToleranceText() );
  }

  m_nesting--;
}


void OdRxObject_Dumper::writeFieldValue( const OdString& name, OdDgDimTextInfo const& value )
{
  writeShift();
  fwprintf( DumpStream, L"%ls:\n", name.c_str() );

  m_nesting++;

  writeFieldValue( "  Width", value.getWidth() );
  writeFieldValue( "  Height", value.getHeight() );
  writeFieldValue( "  Font ID", value.getFontEntryId() );
  writeFieldValue( "  Color", value.getColorIndex() );
  writeFieldValue( "  Weight", value.getWeight() );

  switch( value.getStackedFractionType() )
  {
    case OdDgDimTextInfo::kFractionFromText :
    {
      writeFieldValue( "  Stacked Fraction Type", OdString(L"kFractionFromText") );
    } break;

    case OdDgDimTextInfo::kFractionHorizontal :
    {
      writeFieldValue( "  Stacked Fraction Type", OdString(L"kFractionHorizontal") );
    } break;

    case OdDgDimTextInfo::kFractionDiagonal :
    {
      writeFieldValue( "  Stacked Fraction Type", OdString(L"kFractionDiagonal") );
    } break;
  }

  switch( value.getStackFractAlignment() )
  {
    case OdDgDimTextInfo::kFractAlignmentTop :
    {
      writeFieldValue( "  Stacked Fraction Alignment", OdString(L"kFractAlignmentTop") );
    } break;

    case OdDgDimTextInfo::kFractAlignmentCenter :
    {
      writeFieldValue( "  Stacked Fraction Alignment", OdString(L"kFractAlignmentCenter") );
    } break;

    case OdDgDimTextInfo::kFractAlignmentBottom :
    {
      writeFieldValue( "  Stacked Fraction Alignment", OdString(L"kFractAlignmentBottom") );
    } break;
  }

  writeFieldName("  Text Flags:");
  writeFieldValue( "    Use text color", value.getUseColorFlag() );
  writeFieldValue( "    Use weight ", value.getUseWeightFlag() );
  writeFieldValue( "    Show primary master units ", !value.getPrimaryNoMasterUnitsFlag());
  writeFieldValue( "    Has primary alt format ", !value.getHasAltFormatFlag() );
  writeFieldValue( "    Show secondary master units ", !value.getSecNoMasterUnitsFlag());
  writeFieldValue( "    Has secondary alt format ", !value.getHasSecAltFormatFlag() );

  m_nesting--;
}


void OdRxObject_Dumper::writeFieldValue( const OdString& name, OdDgDimTextFormat const& value )
{
  writeShift();
  fwprintf( DumpStream, L"%ls:\n", name.c_str() );

  m_nesting++;

  writeFieldValue( "  Primary accuracy", value.getPrimaryAccuracy() );
  writeFieldValue( "  Secondary accuracy", value.getSecondaryAccuracy() );

  switch( value.getAngleMode() )
  { 
    case OdDgDimTextFormat::kAngle_D:
    {
      writeFieldValue( "  Angle display mode", OdString(L"kAngle_D") );
    } break;

    case OdDgDimTextFormat::kAngle_DM:
    {
      writeFieldValue( "  Angle display mode", OdString(L"kAngle_DM") );
    } break;

    case OdDgDimTextFormat::kAngle_DMS:
    {
      writeFieldValue( "  Angle display mode", OdString(L"kAngle_DMS") );
    } break;

    case OdDgDimTextFormat::kAngle_C:
    {
      writeFieldValue( "  Angle display mode", OdString(L"kAngle_C") );
    } break;

    case OdDgDimTextFormat::kAngle_Radian:
    {
      writeFieldValue( "  Angle display mode", OdString(L"kAngle_Radian") );
    } break;
  }

  writeFieldName("  Text Format Flags:");
  writeFieldValue( "    AngleMeasure", value.getAngleMeasureFlag() );
  writeFieldValue( "    AngleFormat", value.getAngleFormatFlag() );
  writeFieldValue( "    PrimarySubUnits", value.getPrimarySubUnitsFlag() );
  writeFieldValue( "    PrimaryLabels", value.getPrimaryLabelsFlag() );
  writeFieldValue( "    PrimaryDelimiter", value.getPrimaryDelimiterFlag() );
  writeFieldValue( "    DecimalComma", value.getDecimalCommaFlag() );
  writeFieldValue( "    SuperScriptLSD", value.getSuperScriptLSDFlag() );
  writeFieldValue( "    RoundLSD", value.getRoundLSDFlag() );
  writeFieldValue( "    OmitLeadDelimiter", value.getOmitLeadDelimiterFlag() );
  writeFieldValue( "    LocalFileUnits", value.getLocalFileUnitsFlag() );
  writeFieldValue( "    UnusedDeprecated", value.getUnusedDeprecatedFlag() );
  writeFieldValue( "    ThousandSeparator", value.getThousandSepFlag() );
  writeFieldValue( "    MetricSpace", value.getMetricSpaceFlag() );
  writeFieldValue( "    SecondarySubUnits", value.getSecondarySubUnitsFlag() );
  writeFieldValue( "    SecondaryLabels", value.getSecondaryLabelsFlag() );
  writeFieldValue( "    SecondaryDelimiter", value.getSecondaryDelimiterFlag() );
  writeFieldValue( "    Radians", value.getRadiansFlag() );
  writeFieldValue( "    Show primary master units if zero", value.getPriAllowZeroMastFlag() );
  writeFieldValue( "    Show secondary master units if zero", value.getSecAllowZeroMastFlag() );
  writeFieldValue( "    Show primary sub units if zero", !value.getPriSubForbidZeroMastFlag() );
  writeFieldValue( "    Show secondary sub units if zero", !value.getSecSubForbidZeroMastFlag() );
  writeFieldValue( "    HideAngleSeconds", !value.getHideAngleSecondsFlag() );
  writeFieldValue( "    SkipNonStackedFractionSpace", !value.getSkipNonStackedFractionSpaceFlag() );
  
  m_nesting--;
}


void OdRxObject_Dumper::writeFieldValue( OdString const& name, OdDgDimTextFormat::Accuracy value )
{
  writeFieldName( name );

  switch( value )
  {
  case OdDgDimTextFormat::kAccuracyNone              : fwprintf( DumpStream, L"1 digit\n" ); break;

  case OdDgDimTextFormat::kDecimal1              : fwprintf( DumpStream, L"Decimal, 2 digit\n" ); break;
  case OdDgDimTextFormat::kDecimal2             : fwprintf( DumpStream, L"Decimal, 3 digit\n" ); break;
  case OdDgDimTextFormat::kDecimal3       : fwprintf( DumpStream, L"Decimal, 4 digit\n" ); break;
  case OdDgDimTextFormat::kDecimal4       : fwprintf( DumpStream, L"Decimal, 5 digit\n" ); break;
  case OdDgDimTextFormat::kDecimal5       : fwprintf( DumpStream, L"Decimal, 6 digit\n" ); break;
  case OdDgDimTextFormat::kDecimal6       : fwprintf( DumpStream, L"Decimal, 7 digit\n" ); break;
  case OdDgDimTextFormat::kDecimal7       : fwprintf( DumpStream, L"Decimal, 8 digit\n" ); break;
  case OdDgDimTextFormat::kDecimal8       : fwprintf( DumpStream, L"Decimal, 9 digit\n" ); break;

  case OdDgDimTextFormat::kFractional2          : fwprintf( DumpStream, L"Fractional, 2-th\n" ); break;
  case OdDgDimTextFormat::kFractional4          : fwprintf( DumpStream, L"Fractional, 4-th\n" ); break;
  case OdDgDimTextFormat::kFractional8          : fwprintf( DumpStream, L"Fractional, 8-th\n" ); break;
  case OdDgDimTextFormat::kFractional16         : fwprintf( DumpStream, L"Fractional, 16-th\n" ); break;
  case OdDgDimTextFormat::kFractional32         : fwprintf( DumpStream, L"Fractional, 32-th\n" ); break;
  case OdDgDimTextFormat::kFractional64         : fwprintf( DumpStream, L"Fractional, 64-th\n" ); break;

  case OdDgDimTextFormat::kExponential1 : fwprintf( DumpStream, L"Exponential, 1 digit for mantissa\n" ); break;
  case OdDgDimTextFormat::kExponential2 : fwprintf( DumpStream, L"Exponential, 2 digit for mantissa\n" ); break;
  case OdDgDimTextFormat::kExponential3 : fwprintf( DumpStream, L"Exponential, 3 digit for mantissa\n" ); break;
  case OdDgDimTextFormat::kExponential4 : fwprintf( DumpStream, L"Exponential, 4 digit for mantissa\n" ); break;
  case OdDgDimTextFormat::kExponential5 : fwprintf( DumpStream, L"Exponential, 5 digit for mantissa\n" ); break;
  case OdDgDimTextFormat::kExponential6 : fwprintf( DumpStream, L"Exponential, 6 digit for mantissa\n" ); break;
  case OdDgDimTextFormat::kExponential7 : fwprintf( DumpStream, L"Exponential, 7 digit for mantissa\n" ); break;
  case OdDgDimTextFormat::kExponential8 : fwprintf( DumpStream, L"Exponential, 8 digit for mantissa\n" ); break;
  }
}


void OdRxObject_Dumper::writeFieldValue( const OdString& name, OdDgDimGeometry const& value )
{
  writeShift();
  fwprintf( DumpStream, L"%ls:\n", name.c_str() );

  m_nesting++;

  writeFieldValue( "  Witness line offset", value.getWitnessLineOffset() );
  writeFieldValue( "  Witness line extend", value.getWitnessLineExtend() );
  writeFieldValue( "  Text lift", value.getTextLift() );
  writeFieldValue( "  Text margin", value.getTextMargin() );
  writeFieldValue( "  Terminator width", value.getTerminatorWidth() );
  writeFieldValue( "  Terminator height", value.getTerminatorHeight() );
  writeFieldValue( "  Stack offset", value.getStackOffset() );
  writeFieldValue( "  Center size", value.getCenterSize() );

  if( value.getUseMargin() )
    writeFieldValue( "  Min leader", value.getMargin() );
  else
    writeFieldValue( "  Min leader", value.getTerminatorWidth()*2.0 );
  
  m_nesting--;
}


void OdRxObject_Dumper::writeFieldValue( const OdString& name, OdDgDimOptionPtr& value )
{
  writeShift();
  fwprintf( DumpStream, L"%ls:\n", name.c_str() );

  m_nesting++;

  writeShift();
  if(!value.isNull())
  {
    switch( value->getType() )
    {
      case OdDgDimOption::kNone                             : 
      {
        writeFieldValue( "  Type", OdString(OD_T("kNone")) ); 
      } break;

      case OdDgDimOption::kTolerance                        :
      {
        OdDgDimOptionTolerancePtr pTolerOptions = value;
        writeFieldValue("", pTolerOptions);
      } break;

      case OdDgDimOption::kTerminators                      :
      {
        OdDgDimOptionTerminatorsPtr pTermOptions = value;
        writeFieldValue("", pTermOptions);
      } break;

      case OdDgDimOption::kPrefixSymbol                     :
      {
        OdDgDimOptionSymbolPtr pSymbolOptions = value;
        writeFieldValue("  Type", OdString(OD_T("kPrefixSymbol")) );
        writeFieldValue("", pSymbolOptions );
      } break;

      case OdDgDimOption::kSuffixSymbol                     :
      {
        OdDgDimOptionSymbolPtr pSymbolOptions = value;
        writeFieldValue("  Type", OdString(OD_T("kSuffixSymbol")) );
        writeFieldValue("", pSymbolOptions );
      } break;

      case OdDgDimOption::kDiameterSymbol                   :
      {
        OdDgDimOptionSymbolPtr pSymbolOptions = value;
        writeFieldValue("  Type", OdString(OD_T("kDiameterSymbol")) );
        writeFieldValue("", pSymbolOptions );
      } break;

      case OdDgDimOption::kPrefixSuffix                     :
      {
        OdDimOptionPrefixSuffixPtr pPrefixSuffixOptions = value;
        writeFieldValue("", pPrefixSuffixOptions);
      } break;

      case OdDgDimOption::kPrimaryUnits                     :
      {
        OdDgDimOptionUnitsPtr pUnitsOptions = value;
        writeFieldValue("  Type", OdString(OD_T("kPrimaryUnits")) );
        writeFieldValue("", pUnitsOptions );
      } break;

      case OdDgDimOption::kSecondaryUnits                   :
      {
        OdDgDimOptionUnitsPtr pUnitsOptions = value;
        writeFieldValue("  Type", OdString(OD_T("kSecondaryUnits")) );
        writeFieldValue("", pUnitsOptions );
      } break;

      case OdDgDimOption::kTerminatorSymbology              :
      {
        OdDgDimOptionTerminatorSymbologyPtr pTermSymbolOptions = value;
        writeFieldValue("", pTermSymbolOptions );
      } break;

      case OdDgDimOption::kView                             :
      {
        OdDgDimOptionViewPtr pViewOptions = value;
        writeFieldValue("  Type", OdString(OD_T("kView")) );

        if( !pViewOptions.isNull() )
          writeFieldValue("  Rotation", pViewOptions->getQuaternion() );
      } break;

      case OdDgDimOption::kAlternatePrimaryUnits            :
      {
        OdDgDimOptionAltFormatPtr pAltOptions = value;
        writeFieldValue("  Type", OdString(OD_T("kAlternativePrimaryUnits")) );
        writeFieldValue("", pAltOptions );
      } break;

      case OdDgDimOption::kOffset                           :
      {
        OdDgDimOptionOffsetPtr pOffsetOptions = value;
        writeFieldValue("", pOffsetOptions );
      } break;

      case OdDgDimOption::kAlternateSecondaryUnits          :
      {
        OdDgDimOptionAltFormatPtr pAltOptions = value;
        writeFieldValue("  Type", OdString(OD_T("kAlternativeSecondaryUnits")) );
        writeFieldValue("", pAltOptions );
      } break;

      case OdDgDimOption::kAlternatePrefixSymbol            :
      {
        OdDgDimOptionSymbolPtr pSymbolOptions = value;
        writeFieldValue("  Type", OdString(OD_T("kAlternatePrefixSymbol")) );
        writeFieldValue("", pSymbolOptions );
      } break;

      case OdDgDimOption::kAlternateSuffixSymbol            :
      {
        OdDgDimOptionSymbolPtr pSymbolOptions = value;
        writeFieldValue("  Type", OdString(OD_T("kAlternateSuffixSymbol")) );
        writeFieldValue("", pSymbolOptions );
      } break;

      case OdDgDimOption::kProxyCell                        :
      {
        OdDgDimOptionProxyCellPtr pCellOptions = value;
        writeFieldValue("", pCellOptions );
      } break;
    }
  }
  else
  {
    writeFieldValue( "  Type", OdString(OD_T("[value unknown]")) );
  }

  m_nesting--;
}

void OdRxObject_Dumper::writeFieldValue( const OdString& name, OdDgDimLabelLine::LabelLineDimensionMode value )
{
  OdString val;

  switch( value )
  {
  case OdDgDimLabelLine::kAngleLength :
    val = OD_T("Angle/Length"); break;
  case OdDgDimLabelLine::kLengthAbove :
    val = OD_T("Length above"); break;
  case OdDgDimLabelLine::kAngleAbove :
    val = OD_T("Angle above"); break;
  case OdDgDimLabelLine::kLengthBelow :
    val = OD_T("Length below"); break;
  case OdDgDimLabelLine::kAngleBelow :
    val = OD_T("Angle below"); break;
  case OdDgDimLabelLine::kLengthAngleAbove :
    val = OD_T("Length Angle above"); break;
  case OdDgDimLabelLine::kLengthAngleBelow :
    val = OD_T("Length Angle below"); break;
  default :
    val = OD_T("Length/Angle"); break;
  }

  writeFieldValue( name, val );
}

void OdRxObject_Dumper::writeFieldValue( const OdString& name, OdDgDimTextInfo::FitOptions value )
{
  OdString strVal;

  switch( value )
  {
  case OdDgDimTextInfo::kTermMovesFirst: strVal = "kTermMovesFirst"; break;
  case OdDgDimTextInfo::kTermReversed: strVal = "kTermReversed"; break;
  case OdDgDimTextInfo::kTermInside: strVal = "kTermInside"; break;
  case OdDgDimTextInfo::kTermOutside: strVal = "kTermOutside"; break;
  case OdDgDimTextInfo::kTextInside: strVal = "kTextInside"; break;
  case OdDgDimTextInfo::kTextMovesFirst: strVal = "kTextMovesFirst"; break;
  case OdDgDimTextInfo::kBothMoves: strVal = "kBothMoves"; break;
  case OdDgDimTextInfo::kSmallestMoves: strVal = "kSmallestMoves"; break;
  }

  writeFieldValue( name, strVal );
}

void OdRxObject_Dumper::writeFieldValue( const OdString& name, OdDgDimTextInfo::TextLocation value )
{
  OdString strVal;

  switch( value )
  {
  case OdDgDimTextInfo::kTextInline: strVal = "kTextInline"; break;
  case OdDgDimTextInfo::kTextAbove: strVal  = "kTextAbove"; break;
  case OdDgDimTextInfo::kTextOutside: strVal = "kTextOutside"; break;
  default: strVal = "kTextTopLeft"; break;
  }

  writeFieldValue( name, strVal );
}

void OdRxObject_Dumper::writeFieldValue( const OdString&, OdDgDimOptionTolerancePtr& value )
{
  writeFieldValue("  Type", OdString(OD_T("kTolerance")) );

  if( value.isNull() )
    return;

  writeFieldValue("  Upper value", value->getToleranceUpper() );
  writeFieldValue("  Lower value", value->getToleranceLower() );
  writeFieldValue("  Stack if equal", value->getStackEqualFlag() );
  writeFieldValue("  Show sign for zero", value->getShowSignForZeroFlag() );
  writeFieldValue("  Left margin", value->getToleranceHorizSep() );
  writeFieldValue("  Separator margin", value->getToleranceVertSep() );
  writeFieldValue("  Font entry Id", value->getFontEntryId() );
  writeFieldValue("  Text Width", value->getToleranceTextWidth() );
  writeFieldValue("  Text Height", value->getToleranceTextHeight() );

  if( value->getTolerancePlusMinusSymbol() )
    writeFieldValue("  Plus/Minus symbol", value->getTolerancePlusMinusSymbol() );

  if( value->getTolerancePrefixSymbol() )
    writeFieldValue("  Prefix symbol", value->getTolerancePrefixSymbol() );

  if( value->getToleranceSuffixSymbol() )
    writeFieldValue("  Suffix symbol", value->getToleranceSuffixSymbol() );

  writeFieldValue("  Stack align", value->getStackAlign() );
}

void OdRxObject_Dumper::writeFieldValue( const OdString&, OdDgDimOptionTerminatorsPtr& value )
{
  writeFieldValue("  Type", OdString(OD_T("kTerminators")) );

  if( value.isNull() )
    return;

  if( value->getArrowTermStyle() == OdDgDimOptionTerminators::kTermDefault )
  {
    writeFieldValue("  Arrow style", OdString(OD_T("kTermDefault")) );
  }
  else if( value->getArrowTermStyle() == OdDgDimOptionTerminators::kTermSymbol )
  {
    writeFieldValue("  Arrow style", OdString(OD_T("kTermSymbol")) );
    writeFieldValue("  Arrow Font entry Id", value->getArrowFontID() );
    writeFieldValue("  Arrow Symbol code", value->getArrowSymbol() );
  }
  else if( value->getArrowTermStyle() == OdDgDimOptionTerminators::kTermCell )
  {
    writeFieldValue("  Arrow style", OdString(OD_T("kTermCell")) );
    writeFieldValue("  Arrow Cell Id", value->getArrowCellID() );
  }
  else
  {
    writeFieldValue("  Arrow style", OdString(OD_T("kTermScaledCell")) );
    writeFieldValue("  Arrow Cell Id", value->getArrowCellID() );
    writeFieldValue("  Arrow Cell scale", value->getSharedCellScale() );
  }

  if( value->getStrokeTermStyle() == OdDgDimOptionTerminators::kTermDefault )
  {
    writeFieldValue("  Stroke style", OdString(OD_T("kTermDefault")) );
  }
  else if( value->getStrokeTermStyle() == OdDgDimOptionTerminators::kTermSymbol )
  {
    writeFieldValue("  Stroke style", OdString(OD_T("kTermSymbol")) );
    writeFieldValue("  Stroke Font entry Id", value->getStrokeFontID() );
    writeFieldValue("  Stroke Symbol code", value->getStrokeSymbol() );
  }
  else if( value->getStrokeTermStyle() == OdDgDimOptionTerminators::kTermCell )
  {
    writeFieldValue("  Stroke style", OdString(OD_T("kTermCell")) );
    writeFieldValue("  Stroke Cell Id", value->getStrokeCellID() );
  }
  else
  {
    writeFieldValue("  Stroke style", OdString(OD_T("kTermScaledCell")) );
    writeFieldValue("  Stroke Cell Id", value->getStrokeCellID() );
    writeFieldValue("  Stroke Cell scale", value->getSharedCellScale() );
  }

  if( value->getOriginTermStyle() == OdDgDimOptionTerminators::kTermDefault )
  {
    writeFieldValue("  Origin style", OdString(OD_T("kTermDefault")) );
  }
  else if( value->getOriginTermStyle() == OdDgDimOptionTerminators::kTermSymbol )
  {
    writeFieldValue("  Origin style", OdString(OD_T("kTermSymbol")) );
    writeFieldValue("  Origin Font entry Id", value->getOriginFontID() );
    writeFieldValue("  Origin Symbol code", value->getOriginSymbol() );
  }
  else if( value->getOriginTermStyle() == OdDgDimOptionTerminators::kTermCell )
  {
    writeFieldValue("  Origin style", OdString(OD_T("kTermCell")) );
    writeFieldValue("  Origin Cell Id", value->getOriginCellID() );
  }
  else
  {
    writeFieldValue("  Origin style", OdString(OD_T("kTermScaledCell")) );
    writeFieldValue("  Origin Cell Id", value->getOriginCellID() );
    writeFieldValue("  Origin Cell scale", value->getSharedCellScale() );
  }

  if( value->getDotTermStyle() == OdDgDimOptionTerminators::kTermDefault )
  {
    writeFieldValue("  Dot style", OdString(OD_T("kTermDefault")) );
  }
  else if( value->getDotTermStyle() == OdDgDimOptionTerminators::kTermSymbol )
  {
    writeFieldValue("  Dot style", OdString(OD_T("kTermSymbol")) );
    writeFieldValue("  Dot Font entry Id", value->getDotFontID() );
    writeFieldValue("  Dot Symbol code", value->getDotSymbol() );
  }
  else if( value->getDotTermStyle() == OdDgDimOptionTerminators::kTermCell )
  {
    writeFieldValue("  Dot style", OdString(OD_T("kTermCell")) );
    writeFieldValue("  Dot Cell Id", value->getDotCellID() );
  }
  else
  {
    writeFieldValue("  Dot style", OdString(OD_T("kTermScaledCell")) );
    writeFieldValue("  Dot Cell Id", value->getDotCellID() );
    writeFieldValue("  Dot Cell scale", value->getSharedCellScale() );
  }
}

void OdRxObject_Dumper::writeFieldValue( const OdString&, OdDgDimOptionTerminatorSymbologyPtr& value )
{
  writeFieldValue("  Type", OdString(OD_T("kTerminatorSymbology")) );

  if( value.isNull() )
    return;

  writeFieldValue("  Use Line type", value->getStyleFlag());
  writeFieldValue("  Use Line weight", value->getWeightFlag());
  writeFieldValue("  Use Color", value->getColorFlag());

  if( value->getStyleFlag() )
    writeFieldValue("  Line Type entry Id", value->getStyle());

  if( value->getWeightFlag() )
    writeFieldValue("  Line Weight", value->getWeight());

  if( value->getColorFlag() )
    writeFieldValue("  Color", value->getColor());
}

void OdRxObject_Dumper::writeFieldValue( const OdString&, OdDgDimOptionSymbolPtr& value )
{
  if( value.isNull() )
    return;

  writeFieldValue("  Font entry Id", value->getSymbolFont() );
  writeFieldValue("  Symbol code", value->getSymbolChar() );
}

void OdRxObject_Dumper::writeFieldValue( const OdString&, OdDimOptionPrefixSuffixPtr& value )
{
  writeFieldValue("  Type", OdString(OD_T("kPrefixSuffix")) );

  if( value.isNull() )
    return;

  if( value->getMainPrefix() )
  {
    writeFieldValue("  Main prefix", value->getMainPrefix() );
  }

  if( value->getMainSuffix() )
  {
    writeFieldValue("  Main suffix", value->getMainSuffix() );
  }

  if( value->getUpperPrefix() )
  {
    writeFieldValue("  Upper prefix", value->getUpperPrefix() );
  }

  if( value->getUpperSuffix() )
  {
    writeFieldValue("  Upper suffix", value->getUpperSuffix() );
  }

  if( value->getLowerPrefix() )
  {
    writeFieldValue("  Lower prefix", value->getLowerPrefix() );
  }

  if( value->getLowerSuffix() )
  {
    writeFieldValue("  Lower suffix", value->getLowerSuffix() );
  }
}

void OdRxObject_Dumper::writeFieldValue( const OdString& name, OdDgModel::UnitBase& value )
{
  if( value == OdDgModel::kNone )
  {
    writeFieldValue( name, OdString(OD_T("kNone")) );
  }
  else if( value == OdDgModel::kMeter )
  {
    writeFieldValue( name, OdString(OD_T("kMeter")) );
  }
  else
  {
    writeFieldValue( name, OdString(OD_T("Unknown")) );
  }
}

void OdRxObject_Dumper::writeFieldValue( const OdString& name, OdDgModel::UnitSystem& value )
{
  if( value == OdDgModel::kCustom )
  {
    writeFieldValue( name, OdString(OD_T("kCustom")) );
  }
  else if( value == OdDgModel::kMetric )
  {
    writeFieldValue( name, OdString(OD_T("kMetric")) );
  }
  else if( value == OdDgModel::kEnglish )
  {
    writeFieldValue( name, OdString(OD_T("kEnglish")) );
  }
  else
  {
    writeFieldValue( name, OdString(OD_T("Unknown")) );
  }
}

void OdRxObject_Dumper::writeFieldValue( const OdString&, OdDgDimOptionUnitsPtr& value )
{
  if( value.isNull() )
    return;

  OdDgModel::UnitDescription descr;
  value->getMasterUnit( descr );
  writeFieldName( "  Master units:" );
  writeFieldValue( "    Unit base", descr.m_base );
  writeFieldValue( "    Unit system", descr.m_system );
  writeFieldValue( "    Denominator", descr.m_denominator );
  writeFieldValue( "    Numerator", descr.m_numerator );
  writeFieldValue( "    Name", descr.m_name );
  value->getSubUnit( descr );
  writeFieldName( "  Sub units:" );
  writeFieldValue( "    Unit base", descr.m_base );
  writeFieldValue( "    Unit system", descr.m_system );
  writeFieldValue( "    Denominator", descr.m_denominator );
  writeFieldValue( "    Numerator", descr.m_numerator );
  writeFieldValue( "    Name", descr.m_name );
}

void OdRxObject_Dumper::writeFieldValue( const OdString&, OdDgDimOptionAltFormatPtr& value )
{
  if( value.isNull() )
    return;

  writeFieldValue("  Accuracy", value->getAccuracy() );
  writeFieldValue("  Show sub units", value->getSubUnits() );
  writeFieldValue("  Show unit labels", value->getLabel() );
  writeFieldValue("  Show delimiter", value->getDelimiter() );
  writeFieldValue("  Show sub units only", value->getNoMasterUnits() );
  writeFieldValue("  Allow zero master units", value->getAllowZeroMasterUnits() );

  if( value->getMoreThanThreshold() )
  {
    if( value->getEqualToThreshold() )
    {
      writeFieldValue("  Condition", OdString(OD_T(">=")) );
    }
    else
    {
      writeFieldValue("  Condition", OdString(OD_T(">")) );
    }
  }
  else
  {
    if( value->getEqualToThreshold() )
    {
      writeFieldValue("  Condition", OdString(OD_T("<=")) );
    }
    else
    {
      writeFieldValue("  Condition", OdString(OD_T("<")) );
    }
  }

  writeFieldValue("  Threshold", value->getThreshold() );
}

void OdRxObject_Dumper::writeFieldValue( const OdString& name, OdDgDimOptionOffset::ChainType value )
{
  OdString strValue;

  switch( value )
  {
    case OdDgDimOptionOffset::kNone:
    {
      strValue = OD_T("kNone");
    } break;

    case OdDgDimOptionOffset::kLine:
    {
      strValue = OD_T("kLine");
    } break;

    case OdDgDimOptionOffset::kArc:
    {
      strValue = OD_T("kArc");
    } break;

    case OdDgDimOptionOffset::kBSpline:
    {
      strValue = OD_T("kBSpline");
    } break;
  }

  writeFieldValue(name, strValue);
}

void OdRxObject_Dumper::writeFieldValue( const OdString& name, OdDgDimOptionOffset::LeaderAlignment value )
{
  OdString strValue;

  switch( value )
  {
    case OdDgDimOptionOffset::kAutoAlignment:
    {
      strValue = OD_T("kAutoAlignment");
    } break;

    case OdDgDimOptionOffset::kLeftAlignment:
    {
      strValue = OD_T("kLeftAlignment");
    } break;

    case OdDgDimOptionOffset::kRightAlignment:
    {
      strValue = OD_T("kRightAlignment");
    } break;
  }

  writeFieldValue(name, strValue);
}


void OdRxObject_Dumper::writeFieldValue( const OdString&, OdDgDimOptionOffsetPtr& value )
{
  writeFieldValue("  Type", OdString(OD_T("kOffset")) );

  if( value.isNull() )
    return;

  writeFieldValue("  Terminator", value->getTerminator() );
  writeFieldValue("  Chain type", value->getChainType() );
  writeFieldValue("  Elbow", value->getElbowFlag() );
  writeFieldValue("  Alignment", value->getAlignment() );
  writeFieldValue("  No dock on dim line", value->getNoDockOnDimLineFlag() );
}

void OdRxObject_Dumper::writeFieldValue( const OdString&, OdDgDimOptionProxyCellPtr& value )
{
  writeFieldValue("  Type", OdString(OD_T("kProxyCell")) );

  if( value.isNull() )
    return;

  writeFieldValue("  Origin", value->getOrigin() );
  writeFieldValue("  Rotation Matrix", value->getRotScale() );
  writeFieldValue("  Check Sum", value->getCheckSum() );
}


void OdRxObject_Dumper::writeFieldValue( const OdString& name, OdDgMultilineSymbology const& value )
{
  writeShift();
  fwprintf( DumpStream, L"%ls:\n", name.c_str() );

  m_nesting++;

  writeFieldValue( "Style", value.getLineStyleEntryId() );
  writeFieldValue( "Weight", value.getLineWeight() );
  writeFieldValue( "Color", value.getColorIndex() );
  writeFieldValue( "Use style", value.getUseStyleFlag() );
  writeFieldValue( "Use weight", value.getUseWeightFlag() );
  writeFieldValue( "Use color", value.getUseColorFlag() );
  writeFieldValue( "Use class", value.getUseClassFlag() );
  writeFieldValue( "Inside arc", value.getCapInArcFlag() );
  writeFieldValue( "Outside arc", value.getCapOutArcFlag() );
  writeFieldValue( "Cap line", value.getCapLineFlag() );
  writeFieldValue( "Custom style", value.getCustomStyleFlag() );
  writeFieldValue( "Cap color from segment", value.getCapColorFromSegmentFlag() );
  writeFieldValue( "Construction class", value.getConstructionClassFlag() );

  m_nesting--;
}


void OdRxObject_Dumper::writeFieldValue( const OdString& name, OdDgMultilinePoint const& value )
{
  writeShift();
  fwprintf( DumpStream, L"%ls:\n", name.c_str() );

  m_nesting++;

  {
    OdGePoint3d point;

    value.getPoint( point );
    writeFieldValue( "Point", point );
  }

  {
    OdUInt32            i, j = value.getBreaksCount();
    OdDgMultilineBreak  break_;
    char                fieldName[ 20 ];

    writeShift();
    fwprintf( DumpStream, L"Number of breaks: %d\n", j  );

    for( i = 0; i < j; i++ )
    {
      sprintf( fieldName, "Break %d", i );
      value.getBreak( i, break_ );
      writeFieldValue( fieldName, break_ );
    }
  }

  m_nesting--;
}


void OdRxObject_Dumper::writeFieldValue( const OdString& name, OdDgMultilineBreak const& value )
{
  writeShift();
  fwprintf( DumpStream, L"%ls:\n", name.c_str() );

  m_nesting++;

  writeFieldValue( "Lines mask", value.getLinesMask() );
  writeFieldValue( "Offset", value.getOffset() );
  writeFieldValue( "Length", value.getLength() );

  {
    OdString flagValue;

    switch( value.getFlags() )
    {
    case OdDgMultilineBreak::kStandardByDistance  : flagValue = "Standard by distance"; break;
    case OdDgMultilineBreak::kFromJoint           : flagValue = "from joint"; break;
    case OdDgMultilineBreak::kToJoint             : flagValue = "to joing"; break;
    }

    writeFieldValue( "Flag", flagValue );
  }

  m_nesting--;
}


void OdRxObject_Dumper::writeFieldValue( const OdString& name, OdDgMultilineProfile const& value )
{
  writeShift();
  fwprintf( DumpStream, L"%ls:\n", name.c_str() );

  m_nesting++;

  writeFieldValue( "Distance", value.getDistance() );

  {
    OdDgMultilineSymbology symbology;

    value.getSymbology( symbology );
    writeFieldValue( "Symbology", symbology );
  }

  m_nesting--;
}


void OdRxObject_Dumper::writeFieldValue( const OdString& name, OdDgTextExtendedProperties::LineSpacingType value )
{
  writeFieldName( name );

  switch( value )
  {
  case OdDgTextExtendedProperties::kExact      : fwprintf( DumpStream, L"Exact\n" ); break;
  case OdDgTextExtendedProperties::kAutomatic  : fwprintf( DumpStream, L"Automatic\n" ); break;
  case OdDgTextExtendedProperties::kFromLineTop: fwprintf( DumpStream, L"FromLineTop\n" ); break;
  case OdDgTextExtendedProperties::kAtLeast    : fwprintf( DumpStream, L"AtLeast\n" ); break;
  }
}

void OdRxObject_Dumper::writeFieldValue( const OdString& name, OdDgRaster::RasterFormat value )
{
  writeFieldName( name );

  switch( value )
  {
  case OdDgRaster::kBitmap     : fwprintf( DumpStream, L"Bitmap raster\n" ); break;
  case OdDgRaster::kByteData  : fwprintf( DumpStream, L"Byte data raster\n" ); break;
  case OdDgRaster::kBinaryRLE : fwprintf( DumpStream, L"Binary RLE raster\n" ); break;
  case OdDgRaster::kByteRLE   : fwprintf( DumpStream, L"Byte RLE raster\n" ); break;
  }
}

void OdRxObject_Dumper::writeFieldValue( const OdString& name, OdDgTagDefinition::Type value )
{
  writeFieldName( name );

  switch( value )
  {
  case OdDgTagDefinition::kChar    : fwprintf( DumpStream, L"Char\n" ); break;
  case OdDgTagDefinition::kInt16    : fwprintf( DumpStream, L"Short int\n" ); break;
  case OdDgTagDefinition::kInt32    : fwprintf( DumpStream, L"Long int\n" ); break;
  case OdDgTagDefinition::kDouble  : fwprintf( DumpStream, L"Double\n" ); break;
  case OdDgTagDefinition::kBinary  : fwprintf( DumpStream, L"Binary\n" ); break;
  }
}


void OdRxObject_Dumper::writeFieldValue( const OdString& name, OdDg::TextDirection value )
{
  writeFieldName( name );

  switch( value )
  {
  case OdDg::kHorizontal  : fwprintf( DumpStream, L"Horizontal\n" ); break;
  case OdDg::kVertical    : fwprintf( DumpStream, L"Vertical\n" ); break;
  case OdDg::kJapanese    : fwprintf( DumpStream, L"Japanese\n" ); break;
  case OdDg::kRightToLeft : fwprintf( DumpStream, L"Right-to-left\n" ); break;
  }
}


void OdRxObject_Dumper::writeFieldValue( const OdString& name, OdGeMatrix3d const& value )
{
  writeFieldName( name );

  fwprintf(  DumpStream, L"%g; %g; %g; %g; %g; %g; %g; %g; %g\n",
            value( 0, 0 ), value( 1, 0 ), value( 2, 0 ), 
            value( 0, 1 ), value( 1, 1 ), value( 2, 1 ), 
            value( 0, 2 ), value( 1, 2 ), value( 2, 2 ) );
}


void OdRxObject_Dumper::writeFieldValue( const OdString& name, OdGsDCRect const& value )
{
  writeFieldName( name );

  fwprintf( DumpStream, L"( 0x%X; 0x%X ) - ( 0x%X; 0x%X )\n", value.m_min.x, value.m_min.y, value.m_max.x, value.m_max.y );
}


void OdRxObject_Dumper::writeFieldValue( const OdString& name, OdDgElementId const& value )
{
  writeFieldName( name );

  fwprintf( DumpStream, L"0x%ls\n", (value.getHandle().ascii()).c_str() );
}

void OdRxObject_Dumper::writeFieldValue( const OdString& name, const OdVariant& val )
{
  if( val.isArray() && (val.type() != OdVariant::kInt8) )
    return;

  switch( val.type() )
  {
    case OdVariant::kBool:
    {
      if( val.isArray() )
      {
        OdBoolArray arrData = val.getBoolArray();

        OdString strData;

        for(OdUInt32 i = 0; i < arrData.size(); i++ )
        {
          OdString strItem;

          if( i < arrData.size() - 1 )
          {
            if( arrData[i] )
              strItem += L"true,";
            else
              strItem += L"false,";
          }
          else
          {
            if( arrData[i] )
              strItem += L"true";
            else
              strItem += L"false";
          }

          strData += strItem;
        }

        writeFieldValue( name, strData );
      }
      else
        writeFieldValue( name, val.getBool() );
    }
    break;

    case OdVariant::kInt8:
    {
      if( val.isArray() )
      {
        OdInt8Array arrData = val.getInt8Array();

        OdString strData;

        for(OdUInt32 i = 0; i < arrData.size(); i++ )
        {
          OdString strItem;

          if( i < arrData.size() - 1 )
            strItem.format(L"%02x, ", arrData[i] );
          else
            strItem.format(L"%02x", arrData[i] );

          strData += strItem;
        }

        writeFieldValue( name, strData );
      }
      else
        writeFieldValue( name, val.getInt8() );
    }
    break;

    case OdVariant::kInt16:
    {
      if( val.isArray() )
      {
        OdInt16Array arrData = val.getInt16Array();

        OdString strData;

        for(OdUInt32 i = 0; i < arrData.size(); i++ )
        {
          OdString strItem;

          if( i < arrData.size() - 1 )
            strItem.format(L"%d, ", arrData[i] );
          else
            strItem.format(L"%d", arrData[i] );

          strData += strItem;
        }

        writeFieldValue( name, strData );
      }
      else
        writeFieldValue( name, val.getInt16() );
    }
    break;

    case OdVariant::kInt32:
    {
      if( val.isArray() )
      {
        OdInt32Array arrData = val.getInt32Array();

        OdString strData;

        for(OdUInt32 i = 0; i < arrData.size(); i++ )
        {
          OdString strItem;

          if( i < arrData.size() - 1 )
            strItem.format(L"%d, ", arrData[i] );
          else
            strItem.format(L"%d", arrData[i] );

          strData += strItem;
        }

        writeFieldValue( name, strData );
      }
      else
        writeFieldValue( name, val.getInt32() );
    }
    break;

    case OdVariant::kUInt64:
    {
      if( val.isArray() )
      {
        OdInt64Array arrData = val.getInt64Array();

        OdString strData;

        for(OdUInt32 i = 0; i < arrData.size(); i++ )
        {
          OdString strItem;

          if( i < arrData.size() - 1 )
            strItem.format(L"%ll, ", arrData[i] );
          else
            strItem.format(L"%ll", arrData[i] );

          strData += strItem;
        }

        writeFieldValue( name, strData );
      }
      else
        writeFieldValue( name, val.getUInt64() );
    }
    break;

    case OdVariant::kDouble:
    {
      if( val.isArray() )
      {
        OdDoubleArray arrData = val.getDoubleArray();

        OdString strData;

        for(OdUInt32 i = 0; i < arrData.size(); i++ )
        {
          OdString strItem;

          if( i < arrData.size() - 1 )
            strItem.format(L"%f, ", arrData[i] );
          else
            strItem.format(L"%f", arrData[i] );

          strData += strItem;
        }

        writeFieldValue( name, strData );
      }
      else
        writeFieldValue( name, val.getDouble() );
    }
    break;

    case OdVariant::kString:
    {
      if( val.isArray() )
      {
        OdStringArray arrData = val.getStringArray();

        OdString strData;

        for(OdUInt32 i = 0; i < arrData.size(); i++ )
        {
          if( i < arrData.size() - 1 )
            strData += arrData[i] + L",";
          else
            strData += arrData[i];
        }

        writeFieldValue( name, strData );
      }
      else
        writeFieldValue( name, val.getString() );
    }
    break;

    case OdVariant::kAnsiString:
    {
      if( val.isArray() )
      {
        OdAnsiStringArray arrData = val.getAnsiStringArray();

        OdString strData;

        for(OdUInt32 i = 0; i < arrData.size(); i++ )
        {
          OdString strItem(arrData[i]);

          if( i < arrData.size() - 1 )
            strData += strItem + L",";
          else
            strData += strItem;
        }

        writeFieldValue( name, strData );
      }
      else
        writeFieldValue( name, OdString(val.getAnsiString()) );
    }
    break;
  }
}

void OdRxObject_Dumper::writeFieldValue( OdString const& name, OdGeExtents2d const& value )
{
  writeFieldName( name );

  OdGePoint2d min = value.minPoint(), max = value.maxPoint();

  fwprintf( DumpStream, L"Low point: %g; %g;  High point: %g; %g\n",
    min.x, min.y,
    max.x, max.y
    );
}

void OdRxObject_Dumper::writeFieldValue( OdString const& name, OdGeExtents3d const& value )
{
  writeFieldName( name );

  OdGePoint3d min = value.minPoint(), max = value.maxPoint();

  fwprintf( DumpStream, L"Low point: %g; %g; %g;  High point: %g; %g; %g\n",
    min.x, min.y, min.z,
    max.x, max.y, max.z
    );
}

void OdRxObject_Dumper::writeFieldName( const OdString& fieldName )
{
  writeShift();
  fwprintf( DumpStream, L"%-30ls = ", fieldName.c_str() );
}

void OdRxObject_Dumper::writeFieldValue( const OdString& name, OdDgDimTool::TerminatorType iType )
{
  switch( iType )
  {
    case OdDgDimTool::kTtNone:
    {
      writeFieldValue(name, OdString(OD_T("kTtNone")) );
    } break;

    case OdDgDimTool::kTtArrow:
    {
      writeFieldValue(name, OdString(OD_T("kTtArrow")) );
    } break;

    case OdDgDimTool::kTtStroke:
    {
      writeFieldValue(name, OdString(OD_T("kTtStroke")) );
    } break;

    case OdDgDimTool::kTtCircle:
    {
      writeFieldValue(name, OdString(OD_T("kTtCircle")) );
    } break;

    case OdDgDimTool::kTtFilledCircle:
    {
      writeFieldValue(name, OdString(OD_T("kTtFilledCircle")) );
    } break;
  }
}

void OdRxObject_Dumper::writeFieldValue( const OdString& name, OdDgDimTool::TextType iType )
{
  switch( iType )
  {
    case OdDgDimTool::kStandard:
    {
      writeFieldValue(name, OdString(OD_T("kStandard")) );
    } break;

    case OdDgDimTool::kVertical:
    {
      writeFieldValue(name, OdString(OD_T("kVertical")) );
    } break;

    case OdDgDimTool::kMixed:
    {
      writeFieldValue(name, OdString(OD_T("kMixed")) );
    } break;
  }
}

void OdRxObject_Dumper::writeFieldValue( const OdString& name, OdDgDimTool::CustomSymbol iSymbol )
{
  switch( iSymbol )
  {
    case OdDgDimTool::kCsNone:
    {
      writeFieldValue(name, OdString(OD_T("kCsNone")) );
    } break;

    case OdDgDimTool::kCsDiameter:
    {
      writeFieldValue(name, OdString(OD_T("kCsDiameter")) );
    } break;

    case OdDgDimTool::kCsRadius:
    {
      writeFieldValue(name, OdString(OD_T("kCsRadius")) );
    } break;

    case OdDgDimTool::kCsSquare:
    {
      writeFieldValue(name, OdString(OD_T("kCsSquare")) );
    } break;

    case OdDgDimTool::kCsSR:
    {
      writeFieldValue(name, OdString(OD_T("kCsSR")) );
    } break;

    case OdDgDimTool::kCsSDiameter:
    {
      writeFieldValue(name, OdString(OD_T("kCsSDiameter")) );
    } break;
  }
}

void OdRxObject_Dumper::writeFieldValue( const OdString& name, OdDgDimTool::Leader iLeader )
{
  switch( iLeader )
  {
    case OdDgDimTool::kRadius:
    {
      writeFieldValue(name, OdString(OD_T("kRadius")) );
    } break;

    case OdDgDimTool::kRadiusExt1:
    {
      writeFieldValue(name, OdString(OD_T("kRadiusExt1")) );
    } break;

    case OdDgDimTool::kRadiusExt2:
    {
      writeFieldValue(name, OdString(OD_T("kRadiusExt2")) );
    } break;

    case OdDgDimTool::kDiameter:
    {
      writeFieldValue(name, OdString(OD_T("kDiameter")) );
    } break;
  }
}

void OdRxObject_Dumper  ::writeFieldValue( const OdString& name, OdDgDimensionPtr pElement )
{
  switch( pElement->getDimensionType() )
  {
    case OdDgDimension::kToolTypeSizeArrow:
    {
      OdDgDimSizeArrowPtr pDimSize = pElement;

      if( !pDimSize.isNull() )
        writeFieldValue( name, pDimSize );
    } break;

    case OdDgDimension::kToolTypeSizeStroke:
    {
      OdDgDimSizeStrokePtr pDimSize = pElement;

      if( !pDimSize.isNull() )
        writeFieldValue( name, pDimSize );
    } break;

    case OdDgDimension::kToolTypeLocateSingle:
    {
      OdDgDimSingleLocationPtr pDimSize = pElement;

      if( !pDimSize.isNull() )
        writeFieldValue( name, pDimSize );
    } break;

    case OdDgDimension::kToolTypeLocateStacked:
    {
      OdDgDimStackedLocationPtr pDimSize = pElement;

      if( !pDimSize.isNull() )
        writeFieldValue( name, pDimSize );
    } break;

    case OdDgDimension::kToolTypeCustomLinear:
    {
      OdDgDimCustomLinearPtr pDimSize = pElement;

      if( !pDimSize.isNull() )
        writeFieldValue( name, pDimSize );
    } break;

    case OdDgDimension::kToolTypeAngleSize:
    {
      OdDgDimAngleSizePtr pDimSize = pElement;

      if( !pDimSize.isNull() )
        writeFieldValue( name, pDimSize );
    } break;

    case OdDgDimension::kToolTypeAngleLines:
    {
      OdDgDimAngleLinesPtr pDimSize = pElement;

      if( !pDimSize.isNull() )
        writeFieldValue( name, pDimSize );
    } break;

    case OdDgDimension::kToolTypeAngleLocation:
    {
      OdDgDimAngleLocationPtr pDimSize = pElement;

      if( !pDimSize.isNull() )
        writeFieldValue( name, pDimSize );
    } break;

    case OdDgDimension::kToolTypeArcLocation:
    {
      OdDgDimArcLocationPtr pDimSize = pElement;

      if( !pDimSize.isNull() )
        writeFieldValue( name, pDimSize );
    } break;

    case OdDgDimension::kToolTypeAngleAxisX:
    {
      OdDgDimAngleAxisXPtr pDimSize = pElement;

      if( !pDimSize.isNull() )
        writeFieldValue( name, pDimSize );
    } break;

    case OdDgDimension::kToolTypeAngleAxisY:
    {
      OdDgDimAngleAxisYPtr pDimSize = pElement;

      if( !pDimSize.isNull() )
        writeFieldValue( name, pDimSize );
    } break;

    case OdDgDimension::kToolTypeArcSize:
    {
      OdDgDimArcSizePtr pDimSize = pElement;

      if( !pDimSize.isNull() )
        writeFieldValue( name, pDimSize );
    } break;

    case OdDgDimension::kToolTypeRadius:
    {
      OdDgDimRadiusPtr pDimSize = pElement;

      if( !pDimSize.isNull() )
        writeFieldValue( name, pDimSize );
    } break;

    case OdDgDimension::kToolTypeRadiusExtended:
    {
      OdDgDimRadiusExtendedPtr pDimSize = pElement;

      if( !pDimSize.isNull() )
        writeFieldValue( name, pDimSize );
    } break;

    case OdDgDimension::kToolTypeDiameter:
    {
      OdDgDimDiameterPtr pDimSize = pElement;

      if( !pDimSize.isNull() )
        writeFieldValue( name, pDimSize );
    } break;

    case OdDgDimension::kToolTypeDiameterExtended:
    {
      OdDgDimDiameterExtendedPtr pDimSize = pElement;

      if( !pDimSize.isNull() )
        writeFieldValue( name, pDimSize );
    } break;

    case OdDgDimension::kToolTypeDiameterPara:
    {
      OdDgDimDiameterParallelPtr pDimSize = pElement;

      if( !pDimSize.isNull() )
        writeFieldValue( name, pDimSize );
    } break;

    case OdDgDimension::kToolTypeDiameterPerp:
    {
      OdDgDimDiameterPerpendicularPtr pDimSize = pElement;

      if( !pDimSize.isNull() )
        writeFieldValue( name, pDimSize );
    } break;

    case OdDgDimension::kToolTypeOrdinate:
    {
      OdDgDimOrdinatePtr pDimSize = pElement;

      if( !pDimSize.isNull() )
        writeFieldValue( name, pDimSize );
    } break;

    case OdDgDimension::kToolTypeCenter:
    {
      OdDgDimCenterPtr pDimSize = pElement;

      if( !pDimSize.isNull() )
        writeFieldValue( name, pDimSize );
    } break;
  }
}

#undef  OUT_LINEAR_DIM_TOOL_FIELDS
#define OUT_LINEAR_DIM_TOOL_FIELDS( type ) \
  void OdRxObject_Dumper::writeFieldValue( const OdString& name, type pElement ) \
{ \
  writeFieldName( name ); \
  writeFieldValue("  StackExtLinesFlag", pElement->getStackExtLinesFlag() );  \
  writeFieldValue("  LeftExtLinesPresentFlag", pElement->getLeftExtLinesPresentFlag() );  \
  writeFieldValue("  RightExtLinesPresentFlag", pElement->getRightExtLinesPresentFlag() );  \
  writeFieldValue("  ArcSymbolFlag", pElement->getArcSymbolFlag() );  \
  writeFieldValue("  FirstTerminator", pElement->getFirstTerminator() );  \
  writeFieldValue("  LeftTerminator", pElement->getLeftTerminator() );  \
  writeFieldValue("  RightTerminator", pElement->getRightTerminator() );  \
  writeFieldValue("  JointTerminator", pElement->getJointTerminator() );  \
  writeFieldValue("  TextType", pElement->getTextType() );  \
  writeFieldValue("  Prefix", pElement->getPrefix() );  \
  writeFieldValue("  Suffix", pElement->getSuffix() );  \
}

OUT_LINEAR_DIM_TOOL_FIELDS( OdDgDimSizeArrowPtr )
OUT_LINEAR_DIM_TOOL_FIELDS( OdDgDimSizeStrokePtr )
OUT_LINEAR_DIM_TOOL_FIELDS( OdDgDimSingleLocationPtr )
OUT_LINEAR_DIM_TOOL_FIELDS( OdDgDimStackedLocationPtr )
OUT_LINEAR_DIM_TOOL_FIELDS( OdDgDimCustomLinearPtr )

#undef  OUT_ANGULAR_SHORT_DIM_TOOL_FIELDS
#define OUT_ANGULAR_SHORT_DIM_TOOL_FIELDS( type ) \
void OdRxObject_Dumper::writeFieldValue( const OdString& name, type pElement ) \
{ \
  writeFieldName( name ); \
  writeFieldValue("  StackExtLinesFlag", pElement->getStackExtLinesFlag() );  \
  writeFieldValue("  LeftExtLinesPresentFlag", pElement->getLeftExtLinesPresentFlag() );  \
  writeFieldValue("  RightExtLinesPresentFlag", pElement->getRightExtLinesPresentFlag() ); \
  writeFieldValue("  ArcSymbolFlag", pElement->getArcSymbolFlag() );  \
  writeFieldValue("  LeftTerminator", pElement->getLeftTerminator() ); \
  writeFieldValue("  RightTerminator", pElement->getRightTerminator() ); \
  writeFieldValue("  Prefix", pElement->getPrefix() );  \
  writeFieldValue("  Suffix", pElement->getSuffix() );  \
}

OUT_ANGULAR_SHORT_DIM_TOOL_FIELDS( OdDgDimAngleSizePtr )
OUT_ANGULAR_SHORT_DIM_TOOL_FIELDS( OdDgDimAngleLinesPtr )
OUT_ANGULAR_SHORT_DIM_TOOL_FIELDS( OdDgDimDiameterParallelPtr )
OUT_ANGULAR_SHORT_DIM_TOOL_FIELDS( OdDgDimDiameterPerpendicularPtr )

#undef  OUT_ANGULAR_DIM_TOOL_FIELDS
#define OUT_ANGULAR_DIM_TOOL_FIELDS( type ) \
void OdRxObject_Dumper::writeFieldValue( const OdString& name, type pElement ) \
{ \
  writeFieldName( name ); \
  writeFieldValue("  StackExtLinesFlag", pElement->getStackExtLinesFlag() );  \
  writeFieldValue("  LeftExtLinesPresentFlag", pElement->getLeftExtLinesPresentFlag() );  \
  writeFieldValue("  RightExtLinesPresentFlag", pElement->getRightExtLinesPresentFlag() ); \
  writeFieldValue("  ArcSymbolFlag", pElement->getArcSymbolFlag() );  \
  writeFieldValue("  FirstTerminator", pElement->getFirstTerminator() );  \
  writeFieldValue("  LeftTerminator", pElement->getLeftTerminator() ); \
  writeFieldValue("  RightTerminator", pElement->getRightTerminator() ); \
  writeFieldValue("  Prefix", pElement->getPrefix() );  \
  writeFieldValue("  Suffix", pElement->getSuffix() );  \
}

OUT_ANGULAR_DIM_TOOL_FIELDS( OdDgDimAngleLocationPtr )
OUT_ANGULAR_DIM_TOOL_FIELDS( OdDgDimArcLocationPtr )
OUT_ANGULAR_DIM_TOOL_FIELDS( OdDgDimAngleAxisXPtr )
OUT_ANGULAR_DIM_TOOL_FIELDS( OdDgDimAngleAxisYPtr )

#undef  OUT_RADIAL_DIM_TOOL_FIELDS
#define OUT_RADIAL_DIM_TOOL_FIELDS( type ) \
void OdRxObject_Dumper::writeFieldValue( const OdString& name, type pElement ) \
{ \
  writeFieldName( name ); \
  writeFieldValue("  StackExtLinesFlag", pElement->getStackExtLinesFlag() );  \
  writeFieldValue("  CenterMarkFlag", pElement->getCenterMarkFlag() );  \
  writeFieldValue("  CenterMarkLeftExtendFlag", pElement->getCenterMarkLeftExtendFlag() );  \
  writeFieldValue("  CenterMarkRightExtendFlag", pElement->getCenterMarkRightExtendFlag() );  \
  writeFieldValue("  CenterMarkTopExtendFlag", pElement->getCenterMarkTopExtendFlag() );  \
  writeFieldValue("  CenterMarkBottomExtendFlag", pElement->getCenterMarkBottomExtendFlag() );  \
  writeFieldValue("  ArcSymbolFlag", pElement->getArcSymbolFlag() );  \
  writeFieldValue("  RightTerminator", pElement->getRightTerminator() ); \
  writeFieldValue("  Prefix", pElement->getPrefix() );  \
  writeFieldValue("  Suffix", pElement->getSuffix() );  \
  writeFieldValue("  Leader", pElement->getLeader() );  \
}

OUT_RADIAL_DIM_TOOL_FIELDS( OdDgDimRadiusPtr )
OUT_RADIAL_DIM_TOOL_FIELDS( OdDgDimRadiusExtendedPtr )
OUT_RADIAL_DIM_TOOL_FIELDS( OdDgDimDiameterPtr )
OUT_RADIAL_DIM_TOOL_FIELDS( OdDgDimDiameterExtendedPtr )

void OdRxObject_Dumper::writeFieldValue( const OdString& name, OdDgDimArcSizePtr pElement ) 
{ 
  writeFieldName( name );
  writeFieldValue("  StackExtLinesFlag", pElement->getStackExtLinesFlag() );
  writeFieldValue("  LeftExtLinesPresentFlag", pElement->getLeftExtLinesPresentFlag() );
  writeFieldValue("  RightExtLinesPresentFlag", pElement->getRightExtLinesPresentFlag() );
  writeFieldValue("  ArcSymbolFlag", pElement->getArcSymbolFlag() );
  writeFieldValue("  ChordAlignFlag", pElement->getChordAlignFlag() );
  writeFieldValue("  LeftTerminator", pElement->getLeftTerminator() );
  writeFieldValue("  RightTerminator", pElement->getRightTerminator() );
  writeFieldValue("  Prefix", pElement->getPrefix() );
  writeFieldValue("  Suffix", pElement->getSuffix() );
}

void OdRxObject_Dumper::writeFieldValue( const OdString& name, OdDgDimOrdinatePtr pElement ) 
{ 
  writeFieldName( name ); 
  writeFieldValue("  StackExtLinesFlag", pElement->getStackExtLinesFlag() );   
  writeFieldValue("  ArcSymbolFlag", pElement->getArcSymbolFlag() );     
  writeFieldValue("  DecrementInReverseDirectionFlag", pElement->getDecrementInReverseDirectionFlag() );  
  writeFieldValue("  FreeLocationOfTxtFlag", pElement->getFreeLocationOfTxtFlag() );    
  writeFieldValue("  Datum value", pElement->getDatumValue() ); 
}

void OdRxObject_Dumper::writeFieldValue( const OdString& name, OdDgDimCenterPtr pElement ) 
{ 
  writeFieldName( name );
  writeFieldValue("  CenterMarkLeftExtendFlag", pElement->getCenterMarkLeftExtendFlag() );
  writeFieldValue("  CenterMarkRightExtendFlag", pElement->getCenterMarkRightExtendFlag() );
  writeFieldValue("  CenterMarkTopExtendFlag", pElement->getCenterMarkTopExtendFlag() );
  writeFieldValue("  CenterMarkBottomExtendFlag", pElement->getCenterMarkBottomExtendFlag() );
}

void OdRxObject_Dumper::writeFieldValue( const OdString & name, OdDgSurface::Type value )
{
  switch( value )
  {
  case OdDgSurface::kSurfaceProjection              : fwprintf( DumpStream, L"Projection" ); break;
  case OdDgSurface::kSurfaceBoundedPlane            : fwprintf( DumpStream, L"Bounded plane" ); break;
  case OdDgSurface::kSurfaceUnboundedPlane          : fwprintf( DumpStream, L"Unbounded plane" ); break;
  case OdDgSurface::kSurfaceRight_CIRCULAR_Cylinder : fwprintf( DumpStream, L"Right circular cylinder" ); break;
  case OdDgSurface::kSurfaceRight_CIRCULAR_Cone     : fwprintf( DumpStream, L"Right circular cone" ); break;
  case OdDgSurface::kSurfaceTabulatedCylinder       : fwprintf( DumpStream, L"Tabulated cylinder" ); break;
  case OdDgSurface::kSurfaceTabulatedCone           : fwprintf( DumpStream, L"Tabulated cone" ); break;
  case OdDgSurface::kSurfaceConvolute               : fwprintf( DumpStream, L"Convolute" ); break;
  case OdDgSurface::kSurfaceRevolution              : fwprintf( DumpStream, L"Revolution" ); break;
  case OdDgSurface::kSurfaceWarped                  : fwprintf( DumpStream, L"Warped" ); break;
  }
}

void OdRxObject_Dumper::writeFieldValue( const OdString & name, OdDgSolid::Type value )
{
  switch( value )
  {
  case OdDgSolid::kSolidProjection : fwprintf( DumpStream, L"Projection" ); break;
  case OdDgSolid::kSolidRevolution : fwprintf( DumpStream, L"Revolution" ); break;
  case OdDgSolid::kSolidBoundary   : fwprintf( DumpStream, L"Boundary" ); break;
  }
}

void OdRxObject_Dumper::writeFieldValue( const OdString& name, OdDgTextExtendedProperties::Spacing value )
{
  writeFieldName( name );
  
  switch( value )
  {
  case OdDgTextExtendedProperties::sNone                : fwprintf( DumpStream, L"None\n" ); break;
  case OdDgTextExtendedProperties::sInterCharSpacing    : fwprintf( DumpStream, L"InterCharSpacing\n" ); break;
  case OdDgTextExtendedProperties::sFixedWidthSpacing   : fwprintf( DumpStream, L"FixedWidthSpacing\n" ); break;
  case OdDgTextExtendedProperties::sAcadInterCharSpacing: fwprintf( DumpStream, L"AcadInterCharSpacing\n" ); break;
  }
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
  //writeFieldValue( "Comments", pSi->getComments() ); // PIDSI_COMMENTS == 6
  OdDgPropertyValuePtr pPropComments = pSi->getProperty( 6 );
  if (!pPropComments.isNull())
  {
    writeFieldValue( "Comments", pPropComments->getValue() );
  }
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
  OdBinaryData dibData;
  pSi->getThumbnailBitmap(dibData);
  if ( dibData.size() )
  {
    writeFieldValue( "ThumbnailBitmap Size", (OdUInt32)dibData.size() );
  }

  // Document Summary Information
  OdDgDocumentSummaryInformationPtr pDsi = oddgGetDocumentSummaryInformation(pDb);
  writeFieldValue( "Category", pDsi->getCategory() );
  writeFieldValue( "Manager", pDsi->getManager() );
  //writeFieldValue( "Company", pDsi->getCompany() ); // PIDDSI_COMPANY == 0x0000000F
  OdDgPropertyValuePtr pPropCompany = pDsi->getProperty( 0x0F );
  writeFieldValue( "Company", pPropCompany->getValue());

  // UserDefined Properties
  OdRxDictionaryPtr pPropDic = pDsi->getCustomProperties();
  if ( pPropDic.get() )
  {
    OdRxDictionaryIteratorPtr pIt = pPropDic->newIterator();
    for ( ; !pIt->done(); pIt->next() )
    {
      OdString propName = pIt->getKey();
      OdDgPropertyValuePtr pPropValue = pIt->object();
      writeFieldValue( propName, pPropValue->getValue() );
    }
  }
  m_nesting--;
}


void OdDgDatabase_Dumper::dump( OdRxObjectPtr object )
{
  OdDgDatabasePtr pDb = object;

  dumpSummaryInfo( pDb );

  startDescription( object );

  //simple fields
  writeFieldValue( "Filename", pDb->getFilename() );
  writeFieldValue( "Original version", pDb->getOriginalFileVersion() );
  writeFieldValue( "Major version", pDb->getMajorVersion() );
  writeFieldValue( "Minor version", pDb->getMinorVersion() );
  writeFieldValue( "Control", pDb->getControlFlags() );
  writeFieldValue( "Control 1", pDb->getControl1Flags() );
  writeFieldValue( "Fbfdcn", pDb->getLockFlags() );
  writeFieldValue( "Ext locks", pDb->getExtLockFlags() );
  writeFieldValue( "Active angle", pDb->getActAngle() );
  writeFieldValue( "Angle round", pDb->getAngleRnd() );
  writeFieldValue( "X active scale", pDb->getXActScale() );
  writeFieldValue( "Y active scale", pDb->getYActScale() );
  writeFieldValue( "Z active scale", pDb->getZActScale() );
  writeFieldValue( "Round scale", pDb->getRoundScale() );
  writeFieldValue( "Azumuth", pDb->getAzimuth() );
  writeFieldValue( "Low", pDb->getExtMin() );
  writeFieldValue( "High", pDb->getExtMax() );
  writeFieldValue( "Active level", pDb->getActiveLevelEntryId() );
  writeFieldValue( "Active line style", pDb->getActiveLineStyleEntryId() );
  writeFieldValue( "Active line weight", pDb->getActiveLineWeight() );
  writeFieldValue( "Active color index", pDb->getActiveColorIndex() );
  writeFieldValue( "Fill color", pDb->getFillColorIndex() );
  writeFieldValue( "Active props", pDb->getActiveProps() );
  writeFieldValue( "Line style", pDb->getLineStyle() );
  writeFieldValue( "Line style scale", pDb->getLineStyleScale() );
  writeFieldValueHex( "Multiline flags", pDb->getMultiLineFlags() );
  writeFieldValue( "Text style ID", pDb->getActiveTextStyleEntryId() );
  writeFieldValue( "Text scale lock", pDb->getTextScaleLock() );
  writeFieldValue( "Active view group ID", pDb->getActiveViewGroupId().getHandle().ascii() );
  writeFieldValue( "Active model ID", pDb->getActiveModelId().getHandle().ascii() );
  writeFieldValue( "Angle format", pDb->getAngleFormat() );
  writeFieldValue( "Angle readout precision", pDb->getAngleReadoutPrec() );
  writeFieldValue( "Tentative mode", pDb->getTentativeMode() );
  writeFieldValue( "Tentative sub-mode", pDb->getTentativeSubMode() );
  writeFieldValue( "Keypoint divident", pDb->getKeyPointDividend() );
  writeFieldValue( "Default snap mode", pDb->getDefaultSnapMode() );
  writeFieldValue( "System class", pDb->getSystemClass() );
  writeFieldValueHex( "DMRSF flag", pDb->getDMRSFlag() );
  writeFieldValue( "DMRS linkage generation mode", pDb->getDMRSLinkageGenerationMode() );
  writeFieldValueHex( "Auto dimenstions flag", pDb->getAutoDimFlags() );
  writeFieldValueHex( "Associative lock mask", pDb->getAssocLockMask() );
  writeFieldValue( "Active cell", pDb->getActiveCell() );
  writeFieldValue( "Active term cell", pDb->getActiveTermCell() );
  writeFieldValue( "Active term scale", pDb->getActiveTermScale() );
  writeFieldValue( "Active pattern cell", pDb->getActivePatternCell() );
  writeFieldValue( "Active pattern scale", pDb->getActivePatternScale() );
  writeFieldValue( "Active pattern angle", pDb->getActivePatternAngle() );
  writeFieldValue( "Active pattern angle 2", pDb->getActivePatternAngle2() );
  writeFieldValue( "Active pattern row spacing", pDb->getActivePatternRowSpacing() );
  writeFieldValue( "Active pattern column spacing", pDb->getActivePatternColumnSpacing() );
  writeFieldValue( "Pattern tolerance", pDb->getPatternTolerance() );
  writeFieldValue( "Active point type", pDb->getActivePointType() );
  writeFieldValue( "Active point symbol", pDb->getActivePointSymbol() );
  writeFieldValue( "Active point cell", pDb->getActivePointCell() );
  writeFieldValue( "Area pattern angle", pDb->getAreaPatternAngle() );
  writeFieldValue( "Area pattern row spacing", pDb->getAreaPatternRowSpacing() );
  writeFieldValue( "Area pattern column spacing", pDb->getAreaPatternColumnSpacing() );
  writeFieldValue( "Reserved cell", pDb->getReservedCell() );
  writeFieldValue( "Z range 2D low", pDb->getZRange2dLow() );
  writeFieldValue( "Z range 2D high", pDb->getZRange2dHigh() );
  writeFieldValue( "Stream delta", pDb->getStreamDelta() );
  writeFieldValue( "Stream tolerance", pDb->getStreamTolerance() );
  writeFieldValue( "Angle tolerance", pDb->getAngleTolerance() );
  writeFieldValue( "Area tolerance", pDb->getAreaTolerance() );
  writeFieldValue( "Highlight color", pDb->getHighlightColorIndex() );
  writeFieldValue( "XOR color", pDb->getXorColorIndex() );
  writeFieldValue( "Axis lock angle", pDb->getAxisLockAngle() );
  writeFieldValue( "Axis lock origin", pDb->getAxisLockOrigin() );
  writeFieldValue( "Chamfer distance 1", pDb->getChamferDist1() );
  writeFieldValue( "Chamfer distance 2", pDb->getChamferDist2() );
  writeFieldValue( "Autochain tolerance", pDb->getAutochainTolerance() );
  writeFieldValue( "Consline distance", pDb->getConslineDistance() );
  writeFieldValue( "Arc radius", pDb->getArcRadius() );
  writeFieldValue( "Arc length", pDb->getArcLength() );
  writeFieldValue( "Cone radius 1", pDb->getConeRadius1() );
  writeFieldValue( "Cone radius 2", pDb->getConeRadius2() );
  writeFieldValue( "Polygon radius", pDb->getPolygonRadius() );
  writeFieldValue( "Surrev angle", pDb->getSurrevAngle() );
  writeFieldValue( "Extend distance", pDb->getExtendDistance() );
  writeFieldValue( "Fillet radius", pDb->getFilletRadius() );
  writeFieldValue( "Coppar distance", pDb->getCopparDistance() );
  writeFieldValue( "Array row distance", pDb->getArrayRowDistance() );
  writeFieldValue( "Array column distance", pDb->getArrayColumnDistance() );
  writeFieldValue( "Array fill angle", pDb->getArrayFillAngle() );
  writeFieldValue( "Point distance", pDb->getPointDistance() );
  writeFieldValue( "Polygon edges", pDb->getPolygonEdges() );
  writeFieldValue( "Points between", pDb->getPointsBetween() );
  writeFieldValue( "Array num of items", pDb->getArrayNumItems() );
  writeFieldValue( "Array num of rows", pDb->getArrayNumRows() );
  writeFieldValue( "Array num of columns", pDb->getArrayNumCols() );
  writeFieldValue( "Array rotate", pDb->getArrayRotate() );
  writeFieldValue( "B-spline order", pDb->getBSplineOrder() );
  writeFieldValue( "Display attribute type", pDb->getDispAttrType() );
//  writeFieldValueHex( "Render flags", pDb->getRenderFlags() );
  writeFieldValue( "Latitude", pDb->getLatitude() );
  writeFieldValue( "Longitude", pDb->getLongitude() );
  writeFieldValue( "Solar time", pDb->getSolarTime() );
  writeFieldValue( "Solar year", pDb->getSolarYear() );
  writeFieldValue( "GMT offset", pDb->getGMTOffset() );
  writeFieldValue( "Solar direction", pDb->getSolarDirection() );
  writeFieldValue( "Solar vector override", pDb->getSolarVectorOverride() );
  writeFieldValue( "Solar intensity", pDb->getSolarIntensity() );
  writeFieldValue( "Ambient intensity", pDb->getAmbientIntensity() );
  writeFieldValue( "Flash intensity", pDb->getFlashIntensity() );
  writeFieldValue( "Near depth density", pDb->getNearDepthDensity() );
  writeFieldValue( "Far depth density", pDb->getFarDepthDensity() );
  writeFieldValue( "Near depth distance", pDb->getNearDepthDistance() );
  writeFieldValue( "Haze color", pDb->getHazeColor() );
  writeFieldValue( "Shadow tolerance", pDb->getShadowTolerance() );
  writeFieldValue( "Stroke tolerance", pDb->getStrokeTolerance() );
  writeFieldValue( "Max polygon size", pDb->getMaxPolygonSize() );
  writeFieldValue( "Arc minimum", pDb->getArcMinimum() );
  writeFieldValue( "Exact Hline accuracy", pDb->getExactHLineAccuracy() );
  writeFieldValue( "Exact Hline tolerance", pDb->getExactHLineTolerance() );
  writeFieldValue( "Selection Highlight override", pDb->getSelectionHighlightOverride() );
  writeFieldValue( "Selection Highlight color", pDb->getSelectionHighlightColor() );
  writeFieldValue( "Cell filename", pDb->getCellFileName() );
  writeFieldValue( "Background file", pDb->getBackgroundFile() );
  //writeFieldValue( "Default model is 3D", pDb->getDefaultModelIs3D() );
  //writeFieldValue( "Version", pDb->getVersion() );
  //writeFieldValue( "Sub version", pDb->getSubVersion() );
  //writeFieldValue( "Format", pDb->getFormat() );
  //writeFieldValue( "Highest model ID", pDb->getHighestModelID() );
  writeFieldValue( "Handseed", pDb->getHandseed() );
  writeFieldValue( "Last saved time", pDb->getLastSaveTime() );
  //writeFieldValue( "Next graphics group", pDb->getNextGraphicGroup() );
  //writeFieldValue( "Next text node", pDb->getNextTextNode() );
  //writeFieldValue( "Original format", pDb->getOriginalFormat() );
  //writeFieldValue( "Number of model specific digital signatures", pDb->getModelSpecificDigitalSignatures() );
  //writeFieldValue( "Number of file-wide digital signatures", pDb->getFileWideDigitalSignatures() );
  //writeFieldValue( "Primary application ID", pDb->getPrimaryApplicationID() );
  writeFieldValue( "Is persistent", pDb->isPersistent() );

  OdDgElementId modelId = pDb->getDefaultModelId();
  if (!modelId.isNull())
  {
    OdDgModelPtr pModel = modelId.safeOpenObject();
    if (!pModel.isNull())
    {
      writeFieldValue( "Default Model Name", pModel->getName() );
    }
  }

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
          fwprintf( DumpStream, L"Model #%lu\n", pModel->getEntryId() );

          OdSmartPtr< OdDgModel_Dumper > pModelDumper = pModel;
          pModelDumper->dump( pModel.get() );
        }
      }
    }
  }

  //look through the tables
  {
    OdRxObjectPtr                   object;
    OdSmartPtr< OdRxObject_Dumper > dumper;

    //table of levels
    object = pDb->getLevelTable();
    if( !object.isNull() )
    {
      dumper = object;
      dumper->dump( object );
    }

    //table of level filters
    object = pDb->getLevelFilterTable();
    if( !object.isNull() )
    {
      dumper = object;
      dumper->dump( object );
    }

    //table of fonts
    object = pDb->getFontTable();
    if( !object.isNull() )
    {
      dumper = object;
      dumper->dump( object );
    }

    OdDgTextStyleTablePtr pTextStyleTbl = pDb->getTextStyleTable();
    if (!pTextStyleTbl.isNull())
    {
      fwprintf( DumpStream, L">>>>> Dumps Default Text Style\n" );
      object = pTextStyleTbl->getDefaultData();
      dumper = object;
      dumper->dump( object );
    }

    //table of text styles
    object = pDb->getTextStyleTable();
    if( !object.isNull() )
    {
      dumper = object;
      dumper->dump( object );
    }

    //table of dimension styles
    object = pDb->getDimStyleTable();
    if( !object.isNull() )
    {
      dumper = object;
      dumper->dump( object );
    }

    //table of multiline styles
    object = pDb->getMultilineStyleTable();
    if( !object.isNull() )
    {
      dumper = object;
      dumper->dump( object );
    }

    //table of line styles
    object = pDb->getLineStyleTable();
    if( !object.isNull() )
    {
      dumper = object;
      dumper->dump( object );
    }

    //table of registered applications
    object = pDb->getRegAppTable();
    if( !object.isNull() )
    {
      dumper = object;
      dumper->dump( object );
    }

    //table of views
    object = pDb->getViewGroupTable();
    if( !object.isNull() )
    {
      dumper = object;
      dumper->dump( object );
    }

    //table of named views
    object = pDb->getNamedViewTable();
    if( !object.isNull() )
    {
      dumper = object;
      dumper->dump( object );
    }

    //table of shared cell definitions
    object = pDb->getSharedCellDefinitionTable();
    if( !object.isNull() )
    {
      dumper = object;
      dumper->dump( object );
    }

    //table of tag set definitions
    object = pDb->getTagDefinitionSetTable();
    if( !object.isNull() )
    {
      dumper = object;
      dumper->dump( object );
    }

    //table of colors
    object = pDb->getColorTable();
    if( !object.isNull() )
    {
      dumper = object;
      dumper->dump( object );
    }
    else
    {
      writeShift();
      fwprintf( DumpStream, L"No color table\n" );
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

  //storage unit
  {
    OdDgModel::StorageUnitDescription description;
    element->getStorageUnit( description );

    writeShift();
    fwprintf( DumpStream, L"Storage unit:\n" );
    m_nesting++;

    writeFieldValue( "Base", description.m_base );
    writeFieldValue( "System", description.m_system );
    writeFieldValue( "Numerator", description.m_numerator );
    writeFieldValue( "Denominator", description.m_denominator );
    writeFieldValue( "Uors per Storage", description.m_uorPerStorageUnit );

    m_nesting--;
  }

  //master unit
  {
    OdDgModel::UnitDescription description;
    element->getMasterUnit( description );

    writeShift();
    fwprintf( DumpStream, L"Master unit:\n" );
    m_nesting++;

    writeFieldValue( "Base", description.m_base );
    writeFieldValue( "System", description.m_system );
    writeFieldValue( "Numerator", description.m_numerator );
    writeFieldValue( "Denominator", description.m_denominator );
    writeFieldValue( "Name", description.m_name );

    m_nesting--;
  }

  //sub unit
  {
    OdDgModel::UnitDescription description;
    element->getSubUnit( description );

    writeShift();
    fwprintf( DumpStream, L"Sub unit:\n" );
    m_nesting++;

    writeFieldValue( "Base", description.m_base );
    writeFieldValue( "System", description.m_system );
    writeFieldValue( "Numerator", description.m_numerator );
    writeFieldValue( "Denominator", description.m_denominator );
    writeFieldValue( "Name", description.m_name );

    m_nesting--;
  }

  writeFieldValue( "Working unit", element->getWorkingUnit() );
  writeFieldValue( "Global Origin", element->getGlobalOrigin() );

  OdDgModel::WorkingUnit workingUnit = element->getWorkingUnit();
  double scaleFactor = element->getMeasuresConversion(workingUnit, OdDgModel::kWuWorldUnit);
  writeFieldValue( "scaleFactor", scaleFactor);
 

  {
    OdGeExtents3d extent;

    if( element->getGeomExtents( extent ) == eOk )
    {
      writeFieldValue( "Extent", extent );
    }
    else
    {
      writeFieldValue( "Extent", L"invalid value" );
    }
  }

  //describes all graphics elements
  writeShift();
  fwprintf( DumpStream, L"All graphics sub-elements:\n" );
  describeSubElements( element->createGraphicsElementsIterator() );
  //describes all non-graphics elements
  writeShift();
  fwprintf( DumpStream, L"All control sub-elements:\n" );
  describeSubElements( element->createControlElementsIterator() );

  finishDescription();
}


OdString OdDgModel_Dumper::getClassName()
{
  return "OdDgModel";
}


void OdDgLine2d_Dumper::dump( OdRxObjectPtr object )
{
  OdDgLine2dPtr element = object;
  OdGePoint2d   point;

  startDescription( object );

  point = element->getStartPoint( );
  writeFieldValue( "Vertex 1", point );
  point = element->getEndPoint( );
  writeFieldValue( "Vertex 2", point );

  finishDescription();
}


OdString OdDgLine2d_Dumper::getClassName()
{
  return "OdDgLine2d";
}


void OdDgLine3d_Dumper::dump( OdRxObjectPtr object )
{
  OdDgLine3dPtr element = object;
  OdGePoint3d   point;

  startDescription( object );

  point = element->getStartPoint( );
  writeFieldValue( "Vertex 1", point );
  point = element->getEndPoint( );
  writeFieldValue( "Vertex 2", point );

  finishDescription();
}


OdString OdDgLine3d_Dumper::getClassName()
{
  return "OdDgLine3d";
}


void OdDgLineString2d_Dumper::dump( OdRxObjectPtr object )
{
  OdDgLineString2dPtr element = object;
  OdGePoint2d         point;
  int                 i, j = element->getVerticesCount();
  char                fieldName[ 20 ];

  startDescription( object );

  writeShift();
  fwprintf( DumpStream, L"Number of vertices: %d\n", j );
  
  for( i = 0; i < j; i++ )
  {
    sprintf( fieldName, "Vertex %d", i );
    point = element->getVertexAt( i );
    writeFieldValue( fieldName, point );
  }

  finishDescription();
}


OdString OdDgLineString2d_Dumper::getClassName()
{
  return "OdDgLineString2d";
}


void OdDgLineString3d_Dumper::dump( OdRxObjectPtr object )
{
  OdDgLineString3dPtr element = object;
  OdGePoint3d         point;
  int                 i, j = element->getVerticesCount();
  char                fieldName[ 20 ];

  startDescription( object );

  writeShift();
  fwprintf( DumpStream, L"Number of vertices: %d\n", j );
  
  for( i = 0; i < j; i++ )
  {
    sprintf( fieldName, "Vertex %d", i );
    point = element->getVertexAt( i );
    writeFieldValue( fieldName, point );
  }

  finishDescription();
}


OdString OdDgLineString3d_Dumper::getClassName()
{
  return "OdDgLineString3d";
}


void OdDgText2d_Dumper::dump( OdRxObjectPtr object )
{
  OdDgText2dPtr element = object;

  startDescription( object );

  writeFieldValue( "Text", element->getText() );
  writeFieldValue( "Font ID", element->getFontEntryId() );
  // Gets Font name
  OdDgFontTablePtr pFontTable = element->database()->getFontTable();
  OdDgFontTableRecordPtr pFont = pFontTable->getFont(element->getFontEntryId());
  if (!pFont.isNull())
  {
    writeFieldValue( "Font Name", pFont->getName() );
  }

  writeFieldValue( "Justification", element->getJustification() );
  //writeFieldValue( "Length", element->getLength() );
  //writeFieldValue( "Height", element->getHeight() );
  writeFieldValue( "Length multiplier", element->getLengthMultiplier() );
  writeFieldValue( "Height multiplier", element->getHeightMultiplier() );
  writeFieldValue( "Rotation", element->getRotation() );
  writeFieldValue( "Origin", element->getOrigin() );
  writeFieldValue( "TextColor", element->getColorIndex() );
  writeFieldValue( "Slant", element->getSlant() );
  writeFieldValue( "ItalicsOverrideFlag", element->getItalicsOverrideFlag() );
  writeFieldValue( "SlantFlag", element->getSlantFlag() );
  writeFieldValue( "SlantOverrideFlag", element->getSlantOverrideFlag() );

  writeFieldValue( "UnderlineOverrideFlag", element->getUnderlineOverrideFlag());
  writeFieldValue( "UnderlineFlag", element->getUnderlineFlag());

  writeFieldValue( "HeightOverrideFlag", element->getHeightOverrideFlag() );

  writeFieldValue( "TextStyle bit", element->getTextStyleFlag() );
  writeFieldValue( "TextStyle ID", element->getTextStyleEntryId() );

  OdDgTextStyleTablePtr pTextStyleTable = element->database()->getTextStyleTable();
  if (!pTextStyleTable.isNull())
  {
    OdDgElementId id = pTextStyleTable->getAt(element->getTextStyleEntryId());
    if (!id.isNull())
    {
      OdDgTextStyleTableRecordPtr pTextStyle = id.safeOpenObject();
      writeFieldValue( "TextStyle Name", pTextStyle->getName() );
      writeFieldValue( "Height from TextStyle", pTextStyle->getTextHeight() );
    }
  }
  writeFieldValue( "StyleOverridesFlag", element->getStyleOverridesFlag() );

  double dSpacing;
  OdDgTextExtendedProperties::Spacing spacing;
  element->getSpacing( dSpacing, spacing );
  writeFieldValue( "Spacing value", dSpacing );
  writeFieldValue( "Spacing type", spacing );

  OdInt16 nCount = element->getTextEditFieldCount(); 
  writeFieldValue( "The number of enter data fields in the text element is ", nCount );
  writeShift();
  for (OdInt16 i = 0; i < nCount; i++)
  {
    OdDgTextEditField textEditField = element->getTextEditFieldAt(i);

    writeFieldValue( "StartPosition", element->getHeight() );
    writeFieldValue( "Length", element->getRotation() );
    writeFieldValue( "Justification", element->getOrigin() );
  }

  finishDescription();
}


OdString OdDgText2d_Dumper::getClassName()
{
  return "OdDgText2d";
}


void OdDgText3d_Dumper::dump( OdRxObjectPtr object )
{
  OdDgText3dPtr element = object;

  startDescription( object );

  writeFieldValue( "Text", element->getText() );
  writeFieldValue( "Font ID", element->getFontEntryId() );
  // Gets Font name
  OdDgFontTablePtr pFontTable = element->database()->getFontTable();
  OdDgFontTableRecordPtr pFont = pFontTable->getFont(element->getFontEntryId());
  if (!pFont.isNull())
  {
    writeFieldValue( "Font Name", pFont->getName() );
    writeFieldValue( "Font number", pFont->getNumber() );
    writeFieldValue( "Font type", pFont->getType() );
  }

  writeFieldValue( "Justification", element->getJustification() );
  //writeFieldValue( "Length", element->getLength() );
  //writeFieldValue( "Height", element->getHeight() );
  writeFieldValue( "Length multiplier", element->getLengthMultiplier() );
  writeFieldValue( "Height multiplier", element->getHeightMultiplier() );
  writeFieldValue( "Rotation", element->getRotation() );
  writeFieldValue( "Origin", element->getOrigin() );
  writeFieldValue( "TextColor", element->getColorIndex() );
  writeFieldValue( "Slant", element->getSlant() );
  writeFieldValue( "ItalicsOverrideFlag", element->getItalicsOverrideFlag() );
  writeFieldValue( "SlantFlag", element->getSlantFlag() );
  writeFieldValue( "SlantOverrideFlag", element->getSlantOverrideFlag() );
  writeFieldValue( "SuperscriptFlag", element->getSuperscriptFlag() );
  writeFieldValue( "SuperscriptOverrideFlag", element->getSuperscriptOverrideFlag() );


  writeFieldValue( "HeightOverrideFlag", element->getHeightOverrideFlag() );

  writeFieldValue( "UnderlineOverrideFlag", element->getUnderlineOverrideFlag());
  writeFieldValue( "UnderlineFlag", element->getUnderlineFlag());

  writeFieldValue( "TextStyle bit", element->getTextStyleFlag() );
  writeFieldValue( "TextStyle ID", element->getTextStyleEntryId() );

  OdDgTextStyleTablePtr pTextStyleTable = element->database()->getTextStyleTable();
  if( !pTextStyleTable.isNull() )
  {
    OdDgElementId id = pTextStyleTable->getAt(element->getTextStyleEntryId());
    if (!id.isNull())
    {
      OdDgTextStyleTableRecordPtr pTextStyle = id.safeOpenObject();
      writeFieldValue( "TextStyle Name", pTextStyle->getName() );
      writeFieldValue( "Height from TextStyle", pTextStyle->getTextHeight() );
    }
  }
  writeFieldValue( "StyleOverridesFlag", element->getStyleOverridesFlag() );

  double dSpacing;
  OdDgTextExtendedProperties::Spacing spacing;
  element->getSpacing( dSpacing, spacing );
  writeFieldValue( "Spacing value", dSpacing );
  writeFieldValue( "Spacing type", spacing );

  OdInt16 nCount = element->getTextEditFieldCount(); 
  writeFieldValue( "The number of enter data fields in the text element is ", nCount );
  writeShift();
  for (OdInt16 i = 0; i < nCount; i++)
  {
    OdDgTextEditField textEditField = element->getTextEditFieldAt(i);

    writeFieldValue( "StartPosition", element->getHeight() );
    writeFieldValue( "Length", element->getRotation() );
    writeFieldValue( "Justification", element->getOrigin() );
  }

  finishDescription();
}


OdString OdDgText3d_Dumper::getClassName()
{
  return "OdDgText3d";
}


void OdDgTextNode2d_Dumper::dump( OdRxObjectPtr object )
{
  OdDgTextNode2dPtr element = object;

//#pragma MARKMESSAGE("TODO: Examples. OdDgTextNode2d interface was modified.");
  startDescription( object );

  writeFieldValue( "Line spacing", element->getLineSpacing() );
  writeFieldValue( "Font ID", element->getFontEntryId() );
  writeFieldValue( "Max length", element->getMaxLength() );
  writeFieldValue( "Justification", element->getJustification() );
  writeFieldValue( "Length", element->getLengthMultiplier() );
  writeFieldValue( "Height", element->getHeightMultiplier() );
  /*
  {
    TextAttributes attributes;

    element->getTextAttributes( attributes );
    writeFieldValue( "Text attributes", attributes );
  }
  */
  writeFieldValue( "Rotation", element->getRotation() );
  writeFieldValue( "Origin", element->getOrigin() );

  double dSpacing;
  OdDgTextExtendedProperties::Spacing spacing;
  element->getSpacing( dSpacing, spacing );
  writeFieldValue( "Spacing value", dSpacing );
  writeFieldValue( "Spacing type", spacing );


  describeSubElements( element->createIterator() );

  finishDescription();
}


OdString OdDgTextNode2d_Dumper::getClassName()
{
  return "OdDgTextNode2d";
}


void OdDgTextNode3d_Dumper::dump( OdRxObjectPtr object )
{
  OdDgTextNode3dPtr element = object;

//#pragma MARKMESSAGE("TODO: Examples. OdDgTextNode3d interface was modified.");
  startDescription( object );

  writeFieldValue( "Line spacing", element->getLineSpacing() );
  writeFieldValue( "Font ID", element->getFontEntryId() );
  writeFieldValue( "Max length", element->getMaxLength() );
  writeFieldValue( "Justification", element->getJustification() );
  writeFieldValue( "Length", element->getLengthMultiplier() );
  writeFieldValue( "Height", element->getHeightMultiplier() );
  /*
  {
    TextAttributes attributes;

    element->getTextAttributes( attributes );
    writeFieldValue( "Text attributes", attributes );
  }
  */
  writeFieldValue( "Rotation", element->getRotation() );
  writeFieldValue( "Origin", element->getOrigin() );

  double dSpacing;
  OdDgTextExtendedProperties::Spacing spacing;
  element->getSpacing( dSpacing, spacing );
  writeFieldValue( "Spacing value", dSpacing );
  writeFieldValue( "Spacing type", spacing );

  describeSubElements( element->createIterator() );

  finishDescription();
}


OdString OdDgTextNode3d_Dumper::getClassName()
{
  return "OdDgTextNode3d";
}


void OdDgShape2d_Dumper::dump( OdRxObjectPtr object )
{
  OdDgShape2dPtr element = object;
  OdGePoint2d    point;
  int            i, j = element->getVerticesCount();
  char           fieldName[ 20 ];

  startDescription( object );

  writeShift();
  fwprintf( DumpStream, L"Number of vertices: %d\n", j );
  
  for( i = 0; i < j; i++ )
  {
    sprintf( fieldName, "Vertex %d", i );
    point = element->getVertexAt( i );
    writeFieldValue( fieldName, point );
  }

  finishDescription();
}


OdString OdDgShape2d_Dumper::getClassName()
{
  return "OdDgShape2d";
}


void OdDgShape3d_Dumper::dump( OdRxObjectPtr object )
{
  OdDgShape3dPtr element = object;
  OdGePoint3d    point;
  int            i, j = element->getVerticesCount();
  char           fieldName[ 20 ];

  startDescription( object );

  writeShift();
  fwprintf( DumpStream, L"Number of vertices: %d\n", j );
  
  for( i = 0; i < j; i++ )
  {
    sprintf( fieldName, "Vertex %d", i );
    point = element->getVertexAt( i );
    writeFieldValue( fieldName, point );
  }

  finishDescription();
}


OdString OdDgShape3d_Dumper::getClassName()
{
  return "OdDgShape3d";
}


void OdDgCurve2d_Dumper::dump( OdRxObjectPtr object )
{
  OdDgCurve2dPtr element = object;
  OdGePoint2d    point;
  int            i, j = element->getVerticesCount();
  char           fieldName[ 20 ];

  startDescription( object );

  writeShift();
  fwprintf( DumpStream, L"Number of vertices: %d\n", j );
  
  for( i = 0; i < j; i++ )
  {
    sprintf( fieldName, "Vertex %d", i );
    point = element->getVertexAt( i );
    writeFieldValue( fieldName, point );
  }

  finishDescription();
}


OdString OdDgCurve2d_Dumper::getClassName()
{
  return "OdDgCurve2d";
}


void OdDgCurve3d_Dumper::dump( OdRxObjectPtr object )
{
  OdDgCurve3dPtr element = object;
  OdGePoint3d    point;
  int            i, j = element->getVerticesCount();
  char           fieldName[ 20 ];

  startDescription( object );

  writeShift();
  fwprintf( DumpStream, L"Number of vertices: %d\n", j );
  
  for( i = 0; i < j; i++ )
  {
    sprintf( fieldName, "Vertex %d", i );
    point = element->getVertexAt( i );
    writeFieldValue( fieldName, point );
  }

  finishDescription();
}


OdString OdDgCurve3d_Dumper::getClassName()
{
  return "OdDgCurve3d";
}


void OdDgEllipse2d_Dumper::dump( OdRxObjectPtr object )
{
  OdDgEllipse2dPtr element = object;

  startDescription( object );

  writeFieldValue( "Primary axis", element->getPrimaryAxis() );
  writeFieldValue( "Secondary axis", element->getSecondaryAxis() );
  writeFieldValue( "Rotation", element->getRotationAngle() );
  {
    OdGePoint2d point;

    point = element->getOrigin();
    writeFieldValue( "Origin", point );
  }
  
  finishDescription();
}


OdString OdDgEllipse2d_Dumper::getClassName()
{
  return "OdDgEllipse2d";
}


void OdDgEllipse3d_Dumper::dump( OdRxObjectPtr object )
{
  OdDgEllipse3dPtr element = object;

  startDescription( object );

  writeFieldValue( "Primary axis", element->getPrimaryAxis() );
  writeFieldValue( "Secondary axis", element->getSecondaryAxis() );
  writeFieldValue( "Rotation", element->getRotation() );
  writeFieldValue( "Origin", element->origin() );
  
  finishDescription();
}


OdString OdDgEllipse3d_Dumper::getClassName()
{
  return "OdDgEllipse3d";
}


void OdDgArc2d_Dumper::dump( OdRxObjectPtr object )
{
  OdDgArc2dPtr element = object;

  startDescription( object );

  writeFieldValue( "Primary axis", element->getPrimaryAxis() );
  writeFieldValue( "Secondary axis", element->getSecondaryAxis() );
  writeFieldValue( "Rotation", element->getRotationAngle() );
  {
    OdGePoint2d point;

    point = element->getOrigin();
    writeFieldValue( "Origin", point );
  }
  writeFieldValue( "Start angle", element->getStartAngle() );
  writeFieldValue( "Sweep angle", element->getSweepAngle() );

  OdGeEllipArc2d pArc = element->getEllipArc();
  writeFieldValue( "isClockWise", pArc.isClockWise() );
  
  finishDescription();
}


OdString OdDgArc2d_Dumper::getClassName()
{
  return "OdDgArc2d";
}


void OdDgArc3d_Dumper::dump( OdRxObjectPtr object )
{
  OdDgArc3dPtr element = object;

  startDescription( object );

  writeFieldValue( "Primary axis", element->getPrimaryAxis() );
  writeFieldValue( "Secondary axis", element->getSecondaryAxis() );
  writeFieldValue( "Rotation", element->getRotation() );
  writeFieldValue( "Origin", element->getOrigin() );
  writeFieldValue( "Start angle", element->getStartAngle() );
  writeFieldValue( "Sweep angle", element->getSweepAngle() );

  OdGeEllipArc3d pArc = element->getEllipArc();
  writeFieldValue( "Start angle (OdGeEllipArc3d)", pArc.startAng() );
  writeFieldValue( "End angle (OdGeEllipArc3d)", pArc.endAng() );
  
  finishDescription();
}


OdString OdDgArc3d_Dumper::getClassName()
{
  return "OdDgArc3d";
}


void OdDgCone_Dumper::dump( OdRxObjectPtr object )
{
  OdDgConePtr element = object;

  startDescription( object );

  {
    OdGePoint3d point = element->getCenter1();
    writeFieldValue( "Center 1", point );
    point = element->getCenter2();
    writeFieldValue( "Center 2", point );
  }
  writeFieldValue( "Radius 1", element->getRadius1() );
  writeFieldValue( "Radius 2", element->getRadius2() );
  writeFieldValue( "Rotation", element->getRotation() );
  writeFieldValue( "Is solid", element->isSolid() );
  writeFieldValue( "Hole", element->getHoleFlag() );

  finishDescription();
}


OdString OdDgCone_Dumper::getClassName()
{
  return "OdDgCone";
}


void OdDgComplexString_Dumper::dump( OdRxObjectPtr object )
{
  OdDgComplexStringPtr element = object;

  startDescription( object );

  describeSubElements( element->createIterator() );

  finishDescription();
}


OdString OdDgComplexString_Dumper::getClassName()
{
  return "OdDgComplexString";
}


void OdDgComplexShape_Dumper::dump( OdRxObjectPtr object )
{
  OdDgComplexShapePtr element = object;

  startDescription( object );

  describeSubElements( element->createIterator() );

  finishDescription();
}


OdString OdDgComplexShape_Dumper::getClassName()
{
  return "OdDgComplexShape";
}


void OdDgPointString2d_Dumper::dump( OdRxObjectPtr object )
{
  OdDgPointString2dPtr element = object;
  OdGePoint2d         point;
  int                 i, j = element->getVerticesCount();
  char                fieldName[ 20 ];

  startDescription( object );

  writeShift();
  fwprintf( DumpStream, L"Number of vertices: %d\n", j );

  writeFieldValue( "Continuous", element->getContinuousFlag() );
  
  for( i = 0; i < j; i++ )
  {
    sprintf( fieldName, "Vertex %d", i );
    point = element->getVertexAt( i );
    writeFieldValue( fieldName, point );

    sprintf( fieldName, "Rotation %d", i );
    writeFieldValue( fieldName, element->getRotationAt( i ) );
  }

  finishDescription();
}


OdString OdDgPointString2d_Dumper::getClassName()
{
  return "OdDgPointString2d";
}


void OdDgPointString3d_Dumper::dump( OdRxObjectPtr object )
{
  OdDgPointString3dPtr element = object;
  OdGePoint3d         point;
  int                 i, j = element->getVerticesCount();
  char                fieldName[ 20 ];

  startDescription( object );

  writeShift();
  fwprintf( DumpStream, L"Number of vertices: %d\n", j );
  
  writeFieldValue( "Continuous", element->getContinuousFlag() );

  for( i = 0; i < j; i++ )
  {
    sprintf( fieldName, "Vertex %d", i );
    point = element->getVertexAt( i );
    writeFieldValue( fieldName, point );

    sprintf( fieldName, "Rotation %d", i );
    writeFieldValue( fieldName, element->getRotationAt( i ) );
  }

  finishDescription();
}


OdString OdDgPointString3d_Dumper::getClassName()
{
  return "OdDgPointString3d";
}


void OdDgDimension_Dumper::dump( OdRxObjectPtr object )
{
  OdDgDimensionPtr element = object;

  startDescription( object );

  //simple fields
  writeFieldValue( "Tool type", element->getDimensionType() );
  writeFieldValue( "Scale", element->getScaleFactor() );
  writeFieldValueHex( "Flags", element->getFlags() );

  if( element->get3dFormatFlag() )
  {
    OdGeQuaternion  quaternion;
    element->getRotation( quaternion );
    writeFieldValue( "3D rotation", quaternion );
  }
  else
  {
    OdGeMatrix2d    matrix;
    element->getRotation( matrix );
    writeFieldValue( "2D rotation", matrix );
  }

  {
    writeShift();
    switch(element->getAlignment())
    {
    case OdDgDimension::kPaArbitrary:
      fwprintf( DumpStream, L"Placement Alignment: kPaArbitrary\n" );
      break;
    case OdDgDimension::kPaDrawing:
      fwprintf( DumpStream, L"Placement Alignment: kPaDrawing\n" );
      break;
    case OdDgDimension::kPaTrue:
      fwprintf( DumpStream, L"Placement Alignment: kPaTrue\n" );
      break;
    case OdDgDimension::kPaView:
      fwprintf( DumpStream, L"Placement Alignment: kPaView\n" );
      writeShift();
      fwprintf( DumpStream, L"Alignment View = %d\n", element->getAlignmentView() );
      break;
    };
    writeShift();
    switch(element->getArrowHead())
    {
    case OdDgDimension::kOpened:
      fwprintf( DumpStream, L"Arrow Head Type: kOpened\n" );
      break;
    case OdDgDimension::kClosed:
      fwprintf( DumpStream, L"Arrow Head Type: kClosed\n" );
      break;
    case OdDgDimension::kFilled:
      fwprintf( DumpStream, L"Arrow Head Type: kFilled\n" );
      break;
    };
    writeShift();
    if(element->Is3D())
    {
      OdGeQuaternion q;
      element->getRotation(q);
      fwprintf( DumpStream, L"Quaternion: [ %g %g %g %g ]\n", q.w, q.x, q.y, q.z );
    }
    else
    {
      OdGeMatrix2d q;
      element->getRotation(q);
      fwprintf( DumpStream, L"Matrix2d: [[ %g %g ] [ %g %g ]]\n", q[0][0], q[0][1], q[1][0], q[1][1] );
    }

    writeShift();
    fwprintf( DumpStream, L"Flags:\n" );
    m_nesting++;
#undef DUMPDIMFLAG
#define DUMPDIMFLAG(flg) \
  if(element->get##flg##Flag()) \
    {\
      writeShift();\
      fwprintf( DumpStream, L"%ls\n", (OdString(#flg)).c_str() );\
    }
    DUMPDIMFLAG(BoxText)
    DUMPDIMFLAG(CapsuleText)
    DUMPDIMFLAG(Centesimal)
    DUMPDIMFLAG(CrossCenter)
    DUMPDIMFLAG(Dual)
    DUMPDIMFLAG(Embed)
    DUMPDIMFLAG(Horizontal)
    DUMPDIMFLAG(Joiner)
    DUMPDIMFLAG(LeadingZero2)
    DUMPDIMFLAG(LeadingZeros)
    DUMPDIMFLAG(NoAutoTextLift)
    DUMPDIMFLAG(NoLevelSymb)
    DUMPDIMFLAG(RelDimLine)
    DUMPDIMFLAG(RelStat)
    DUMPDIMFLAG(StackFract)
    DUMPDIMFLAG(TextHeapPad)
    DUMPDIMFLAG(ThousandComma)
    DUMPDIMFLAG(Tolerance)
    DUMPDIMFLAG(Tolmode)
    DUMPDIMFLAG(TrailingZeros)
    DUMPDIMFLAG(TrailingZeros2)
    DUMPDIMFLAG(UnderlineText)

    m_nesting--;

  }
  //
  //composite fields
  //

  //points
  {
    OdUInt32      i, j = element->getPointsCount();
    OdDgDimPoint  point;
    char          fieldName[ 20 ];

    writeShift();
    fwprintf( DumpStream, L"Number of points: %d\n", j  );

    for( i = 0; i < j; i++ )
    {
      sprintf( fieldName, "Point %d", i );
      point = element->getPoint( i );
      writeFieldValue( fieldName, point );
    }
  }

  //text info
  {
    OdDgDimTextInfo textInfo;

    element->getDimTextInfo( textInfo );
    writeFieldValue( "Text info", textInfo );
  }

  //text format
  {
    OdDgDimTextFormat textFormat;

    element->getDimTextFormat( textFormat );
    writeFieldValue( "Text format", textFormat );
  }

  //geometry
  {
    OdDgDimGeometry geometry;

    element->getDimGeometry( geometry );
    writeFieldValue( "Geometry", geometry );
  }

  // Symbology
  {
    OdInt32 altLineStyle = element->getAltLineStyleEntryId();
    OdUInt32 altLineWeight = element->getAltLineWeight();
    OdUInt32 altColorIndex = element->getAltColorIndex();

    writeFieldValue( "Alternative LineStyle", altLineStyle );
    writeFieldValue( "Alternative LineWeight", altLineWeight );
    writeFieldValue( "Alternative ColorIndex", altColorIndex );
  }

  // tools

  writeFieldValue("Tools:", element );

  //options
  {
    OdUInt32 iOptionsCount = 0;

#undef  INC_DIM_OPTIONS_COUNT
#define INC_DIM_OPTIONS_COUNT(name)                      \
  if( !element->getOption(OdDgDimOption::k##name).isNull() ) \
  iOptionsCount++;

    INC_DIM_OPTIONS_COUNT(Tolerance)
      INC_DIM_OPTIONS_COUNT(Terminators)
      INC_DIM_OPTIONS_COUNT(PrefixSymbol)
      INC_DIM_OPTIONS_COUNT(SuffixSymbol)
      INC_DIM_OPTIONS_COUNT(DiameterSymbol)
      INC_DIM_OPTIONS_COUNT(PrefixSuffix)
      INC_DIM_OPTIONS_COUNT(PrimaryUnits)
      INC_DIM_OPTIONS_COUNT(SecondaryUnits)
      INC_DIM_OPTIONS_COUNT(TerminatorSymbology)
      INC_DIM_OPTIONS_COUNT(View)
      INC_DIM_OPTIONS_COUNT(AlternatePrimaryUnits)
      INC_DIM_OPTIONS_COUNT(Offset)
      INC_DIM_OPTIONS_COUNT(AlternateSecondaryUnits)
      INC_DIM_OPTIONS_COUNT(AlternatePrefixSymbol)
      INC_DIM_OPTIONS_COUNT(AlternateSuffixSymbol)
      INC_DIM_OPTIONS_COUNT(ProxyCell)

      writeShift();
      writeFieldValue( "Number of options", iOptionsCount );

    OdDgDimOptionPtr pDimOptions;
    iOptionsCount = 0;

#undef  DUMP_DIM_OPTIONS
#define DUMP_DIM_OPTIONS(name)                                 \
  pDimOptions = element->getOption(OdDgDimOption::k##name);  \
  if( !pDimOptions.isNull() )                                \
    {                                                          \
    OdString strOptionNum;                                   \
    writeShift();                                            \
    strOptionNum.format(L"Dimension option %d", iOptionsCount ); \
    writeFieldValue( strOptionNum, pDimOptions ); \
    iOptionsCount++; \
    }

      DUMP_DIM_OPTIONS(Tolerance)
      DUMP_DIM_OPTIONS(Terminators)
      DUMP_DIM_OPTIONS(PrefixSymbol)
      DUMP_DIM_OPTIONS(SuffixSymbol)
      DUMP_DIM_OPTIONS(DiameterSymbol)
      DUMP_DIM_OPTIONS(PrefixSuffix)
      DUMP_DIM_OPTIONS(PrimaryUnits)
      DUMP_DIM_OPTIONS(SecondaryUnits)
      DUMP_DIM_OPTIONS(TerminatorSymbology)
      DUMP_DIM_OPTIONS(View)
      DUMP_DIM_OPTIONS(AlternatePrimaryUnits)
      DUMP_DIM_OPTIONS(Offset)
      DUMP_DIM_OPTIONS(AlternateSecondaryUnits)
      DUMP_DIM_OPTIONS(AlternatePrefixSymbol)
      DUMP_DIM_OPTIONS(AlternateSuffixSymbol)
      DUMP_DIM_OPTIONS(ProxyCell)
  }

  finishDescription();
}


OdString OdDgDimension_Dumper::getClassName()
{
  return "OdDgDimension";
}


void OdDgMultiline_Dumper::dump( OdRxObjectPtr object )
{
  OdDgMultilinePtr element = object;

  startDescription( object );

  //simple fields
  writeFieldValue( "Origin cap angle", element->getOriginCapAngle() );
  writeFieldValue( "End cap angle", element->getEndCapAngle() );
  {
    OdGeVector3d vector;

    vector = element->getZVector();
    writeFieldValue( "Z vector", vector );
  }
  writeFieldValue( "Is closed", element->getClosedFlag() );

  //symbologies
  {
    OdDgMultilineSymbology symbology;

    element->getOriginCap( symbology );
    writeFieldValue( "Origin cap", symbology );
    element->getMiddleCap( symbology );
    writeFieldValue( "Joint cap", symbology );
    element->getEndCap( symbology );
    writeFieldValue( "End cap", symbology );
  }

  //points
  {
    OdUInt32            i, j = element->getPointsCount();
    OdDgMultilinePoint  point;
    char                fieldName[ 20 ];

    writeShift();
    fwprintf( DumpStream, L"Number of points: %d\n", j  );

    for( i = 0; i < j; i++ )
    {
      sprintf( fieldName, "Point %d", i );
      element->getPoint( i, point );
      writeFieldValue( fieldName, point );
    }
  }

  //profiles
  {
    OdUInt32              i, j = element->getProfilesCount();
    OdDgMultilineProfile  profile;
    char                  fieldName[ 20 ];

    writeShift();
    fwprintf( DumpStream, L"Number of profiles: %d\n", j  );

    for( i = 0; i < j; i++ )
    {
      sprintf( fieldName, "Profile %d", i );
      element->getProfile( i, profile );
      writeFieldValue( fieldName, profile );
    }
  }

  finishDescription();
}


OdString OdDgMultiline_Dumper::getClassName()
{
  return "OdDgMultiline";
}


void OdDgBSplineCurve2d_Dumper::dump( OdRxObjectPtr object )
{
  OdDgBSplineCurve2dPtr element = object;

  startDescription( object );

  writeFieldValue( "Order", element->getOrder() );
  writeFieldValue( "Display curve", element->getCurveDisplayFlag() );
  writeFieldValue( "Display polygon", element->getPolygonDisplayFlag() );
  writeFieldValue( "Closed", element->getClosedFlag() );

  if( element->hasFitData() )
  {
    writeFieldValue( "Construction type", OdString(L"kDefinedByFitPoints" ));
    writeFieldValue( "Natural tangents", element->getNaturalTangentsFlag() );
    writeFieldValue( "Chord length tangents", element->getChordLenTangentsFlag() );
    writeFieldValue( "Collinear tangents", element->getColinearTangentsFlag() );

    OdGePoint2dArray fitPoints;
    OdUInt32 uOrder;
    bool bTangentExists;
    OdGeVector2d vrStartTangent;
    OdGeVector2d vrEndTangent;
    bool bUniformKnots;
    element->getFitData( fitPoints, uOrder, bTangentExists, vrStartTangent, vrEndTangent, bUniformKnots );

    OdGeKnotVector vrKnots = element->getKnots();

    writeFieldValue(L"Num Fit Points", (OdUInt32)fitPoints.size() );

    for( OdUInt32 i = 0; i < fitPoints.size(); i++ )
    {
      OdString strFitPtsName;
      strFitPtsName.format(L"  Point %d", i);
      writeFieldValue( strFitPtsName, fitPoints[i] );
    }

    writeFieldValue(L"Start Tangent", vrStartTangent );
    writeFieldValue(L"End Tangent", vrEndTangent );
    writeFieldValue(L"Uniform Knots Flag", bUniformKnots );

    writeFieldValue(L"Num Knots", (OdUInt32)vrKnots.length());

    for( OdInt32 j = 0; j < vrKnots.length(); j++ )
    {
      OdString strKnotName;
      strKnotName.format(L"  Knot %d", j);
      writeFieldValue( strKnotName, vrKnots[j] );
    }
  }
  else
  {
    writeFieldValue( "Construction type", OdString(L"kDefinedByNurbsData" ));
    writeFieldValue( "Rational", element->isRational() );

    OdGePoint2dArray arrCtrlPts;
    OdGeKnotVector   vrKnots;
    OdGeDoubleArray  arrWeights;
    OdUInt32 uOrder;
    bool bClosed;
    bool bRational;

    element->getNurbsData( uOrder, bRational, bClosed, arrCtrlPts, vrKnots, arrWeights );

    writeFieldValue(L"Num Control Points", (OdUInt32)arrCtrlPts.size());

    for( OdUInt32 i = 0; i < arrCtrlPts.size(); i++ )
    {
      OdString strCtrlPtsName;
      strCtrlPtsName.format(L"  Point %d", i);
      writeFieldValue( strCtrlPtsName, arrCtrlPts[i] );
    }

    writeFieldValue(L"Num Knots", (OdUInt32)vrKnots.length() );

    for( OdInt32 j = 0; j < vrKnots.length(); j++ )
    {
      OdString strKnotName;
      strKnotName.format(L"  Knot %d", j);
      writeFieldValue( strKnotName, vrKnots[j] );
    }

    if( bRational )
    {
      writeFieldValue(L"Num Weights", (OdUInt32)arrWeights.size() );

      for( OdUInt32 k = 0; k < arrWeights.length(); k++ )
      {
        OdString strWeightName;
        strWeightName.format(L"  Weight %d", k);
        writeFieldValue( strWeightName, arrWeights[k] );
      }
    }
  }

  finishDescription();
}


OdString OdDgBSplineCurve2d_Dumper::getClassName()
{
  return "OdDgBSplineCurve2d";
}


void OdDgBSplineCurve3d_Dumper::dump( OdRxObjectPtr object )
{
  OdDgBSplineCurve3dPtr element = object;

  startDescription( object );

  //simple fields
  writeFieldValue( "Order", element->getOrder() );
  writeFieldValue( "Display curve", element->getCurveDisplayFlag() );
  writeFieldValue( "Display polygon", element->getPolygonDisplayFlag() );
  writeFieldValue( "Closed", element->getClosedFlag() );

  if( element->hasFitData() )
  {
    writeFieldValue( "Construction type", OdString(L"kDefinedByFitPoints" ));
    writeFieldValue( "Natural tangents", element->getNaturalTangentsFlag() );
    writeFieldValue( "Chord length tangents", element->getChordLenTangentsFlag() );
    writeFieldValue( "Collinear tangents", element->getColinearTangentsFlag() );

    OdGePoint3dArray fitPoints;
    OdUInt32 uOrder;
    bool bTangentExists;
    OdGeVector3d vrStartTangent;
    OdGeVector3d vrEndTangent;
    bool bUniformKnots;
    element->getFitData( fitPoints, uOrder, bTangentExists, vrStartTangent, vrEndTangent, bUniformKnots );

    OdGeKnotVector vrKnots = element->getKnots();

    writeFieldValue(L"Num Fit Points", (OdUInt32)fitPoints.size() );

    for( OdUInt32 i = 0; i < fitPoints.size(); i++ )
    {
      OdString strFitPtsName;
      strFitPtsName.format(L"  Point %d", i);
      writeFieldValue( strFitPtsName, fitPoints[i] );
    }

    writeFieldValue(L"Start Tangent", vrStartTangent );
    writeFieldValue(L"End Tangent", vrEndTangent );
    writeFieldValue(L"Uniform Knots Flag", bUniformKnots );

    writeFieldValue(L"Num Knots", (OdUInt32)vrKnots.length());

    for( OdInt32 j = 0; j < vrKnots.length(); j++ )
    {
      OdString strKnotName;
      strKnotName.format(L"  Knot %d", j);
      writeFieldValue( strKnotName, vrKnots[j] );
    }
  }
  else
  {
    writeFieldValue( "Construction type", OdString(L"kDefinedByNurbsData" ));
    writeFieldValue( "Rational", element->isRational() );

    OdGePoint3dArray arrCtrlPts;
    OdGeKnotVector   vrKnots;
    OdGeDoubleArray  arrWeights;
    OdUInt32 uOrder;
    bool bClosed;
    bool bRational;

    element->getNurbsData( uOrder, bRational, bClosed, arrCtrlPts, vrKnots, arrWeights );

    writeFieldValue(L"Num Control Points", (OdUInt32)arrCtrlPts.size());

    for( OdUInt32 i = 0; i < arrCtrlPts.size(); i++ )
    {
      OdString strCtrlPtsName;
      strCtrlPtsName.format(L"  Point %d", i);
      writeFieldValue( strCtrlPtsName, arrCtrlPts[i] );
    }

    writeFieldValue(L"Num Knots", (OdUInt32)vrKnots.length() );

    for( OdInt32 j = 0; j < vrKnots.length(); j++ )
    {
      OdString strKnotName;
      strKnotName.format(L"  Knot %d", j);
      writeFieldValue( strKnotName, vrKnots[j] );
    }

    if( bRational )
    {
      writeFieldValue(L"Num Weights", (OdUInt32)arrWeights.size() );

      for( OdUInt32 k = 0; k < arrWeights.length(); k++ )
      {
        OdString strWeightName;
        strWeightName.format(L"  Weight %d", k);
        writeFieldValue( strWeightName, arrWeights[k] );
      }
    }
  }

  finishDescription();
}


OdString OdDgBSplineCurve3d_Dumper::getClassName()
{
  return "OdDgBSplineCurve3d";
}


void OdDgSurface_Dumper::dump( OdRxObjectPtr object )
{
  OdDgSurfacePtr element = object;

  startDescription( object );

  //common information
  writeFieldValue( "Type of surface", element->getType() );

  //extract the structure
  OdDg3dObjectHelper helper( element );
  OdDg3dObjectHelper::EntireObject structure;
  helper.extractInformation( structure );
  
  OdDg3dObjectHelper::EntireObject::size_type currentBoundaryIndex = 0, boundariesAtAll = structure.size();
  writeFieldValue( "Number of boundaries", OdUInt32( boundariesAtAll ) );

  m_nesting++;

  //iterate through boundaries
  for( ; currentBoundaryIndex < boundariesAtAll; currentBoundaryIndex++ )
  {
    OdDg3dObjectHelper::OdDgGraphicsElementPtrArray
      &boundaries = structure[ currentBoundaryIndex ].m_boundary,
      &rules = structure[ currentBoundaryIndex ].m_rules;

    OdDg3dObjectHelper::OdDgGraphicsElementPtrArray::size_type currentIndex, topIndex;

    writeShift();
    fwprintf( DumpStream, L"Boundary # %d\n", int( currentBoundaryIndex ) );
    m_nesting++;
    topIndex = boundaries.size();
    for( currentIndex = 0; currentIndex < topIndex; currentIndex++ )
    {
      describeSubElement( boundaries[ currentIndex ].get() );
    }
    m_nesting--;

    writeShift();
    fwprintf( DumpStream, L"Rules\n" );
    m_nesting++;
    topIndex = rules.size();
    for( currentIndex = 0; currentIndex < topIndex; currentIndex++ )
    {
      describeSubElement( rules[ currentIndex ].get() );
    }
    m_nesting--;
  }

  m_nesting--;

  finishDescription();
}


OdString OdDgSurface_Dumper::getClassName()
{
  return "OdDgSurface";
}


void OdDgSolid_Dumper::dump( OdRxObjectPtr object )
{
  OdDgSolidPtr element = object;

  startDescription( object );

  //common information
  writeFieldValue( "Type of solid", element->getType() );

  //extract the structure
  OdDg3dObjectHelper helper( element );
  OdDg3dObjectHelper::EntireObject structure;
  helper.extractInformation( structure );
  
  OdDg3dObjectHelper::EntireObject::size_type currentBoundaryIndex = 0, boundariesAtAll = structure.size();
  writeFieldValue( "Number of boundaries", OdUInt32( boundariesAtAll ) );

  m_nesting++;

  //iterate through boundaries
  for( ; currentBoundaryIndex < boundariesAtAll; currentBoundaryIndex++ )
  {
    OdDg3dObjectHelper::OdDgGraphicsElementPtrArray
      &boundaries = structure[ currentBoundaryIndex ].m_boundary,
      &rules = structure[ currentBoundaryIndex ].m_rules;

    OdDg3dObjectHelper::OdDgGraphicsElementPtrArray::size_type currentIndex, topIndex;

    writeShift();
    fwprintf( DumpStream, L"Boundary # %d\n", int( currentBoundaryIndex ) );
    m_nesting++;
    topIndex = boundaries.size();
    for( currentIndex = 0; currentIndex < topIndex; currentIndex++ )
    {
      describeSubElement( boundaries[ currentIndex ].get() );
    }
    m_nesting--;

    writeShift();
    fwprintf( DumpStream, L"Rules\n" );
    m_nesting++;
    topIndex = rules.size();
    for( currentIndex = 0; currentIndex < topIndex; currentIndex++ )
    {
      describeSubElement( rules[ currentIndex ].get() );
    }
    m_nesting--;
  }

  m_nesting--;

  finishDescription();
}


OdString OdDgSolid_Dumper::getClassName()
{
  return "OdDgSolid";
}

void OdDgRasterAttachmentHeader_Dumper::dump( OdRxObjectPtr object )
{
  OdDgRasterAttachmentHeaderPtr element = object;

  startDescription( object );

  //simple fields
  {
    OdGeExtents3d extents;
    element->getGeomExtents( extents );
    writeFieldValue( "The result of getGeomExtents()", extents );
  }
  {
    OdGePoint3d origin;
    OdGeVector3d u, v;    //these values are logged later with OdDgRasterAttachmentComponentGeo object

    element->getOrientation( origin, u, v );
    writeFieldValue( "Origin", origin );
  }
  writeFieldValue( "Extent", element->getExtent() );
  writeFieldValue( "Display gamma", element->getDisplayGamma() );
  writeFieldValue( "Plot gamma", element->getPlotGamma() );
  writeFieldValue( "Apply rotation", element->getApplyRotationFlag() );
  writeFieldValue( "Clipping", element->getClippingFlag() );
  writeFieldValue( "Plot", element->getPlotFlag() );
  writeFieldValue( "Invert", element->getInvertFlag() );
  {
    int   i;
    char  name[ 20 ];

    for( i = 1; i <= 8; i++ )
    {
      sprintf( name, "View %d", i );
      writeFieldValue( name, element->getViewFlag( i ) );
    }
  }

  finishDescription();
}


OdString OdDgRasterAttachmentHeader_Dumper::getClassName()
{
  return "OdDgRasterAttachmentHeader";
}


void OdDgRasterHeader2d_Dumper::dump( OdRxObjectPtr object )
{
  OdDgRasterHeader2dPtr element = object;

  startDescription( object );

  //simple fields
  writeFieldValue( "Right justified", element->getRightJustifiedFlag() );
  writeFieldValue( "Lower justified", element->getLowerJustifiedFlag() );
  writeFieldValue( "Horizontal data", element->getHorizontalDataFlag() );
  writeFieldValue( "Color", element->getColorFlag() );
  writeFieldValue( "Transparent", element->getTransparentFlag() );
  writeFieldValue( "Positive", element->getPositiveFlag() );
  writeFieldValue( "Raster format", element->getFormat() );
  writeFieldValue( "Foreground", element->getForeground() );
  writeFieldValue( "Background", element->getBackground() );
  writeFieldValue( "X extent", element->getXExtent() );
  writeFieldValue( "Y extent", element->getYExtent() );
  writeFieldValue( "Scale", element->getScale() );
  {
    OdGePoint3d origin = element->getOrigin();
    writeFieldValue( "Origin", origin );
  }

  //all parts
  describeSubElements( element->createIterator() );

  finishDescription();
}


OdString OdDgRasterHeader2d_Dumper::getClassName()
{
  return "OdDgRasterHeader2d";
}


void OdDgRasterHeader3d_Dumper::dump( OdRxObjectPtr object )
{
  OdDgRasterHeader3dPtr element = object;

  startDescription( object );

  //simple fields
  writeFieldValue( "Right justified", element->getRightJustifiedFlag() );
  writeFieldValue( "Lower justified", element->getLowerJustifiedFlag() );
  writeFieldValue( "Horizontal data", element->getHorizontalDataFlag() );
  writeFieldValue( "Color", element->getColorFlag() );
  writeFieldValue( "Transparent", element->getTransparentFlag() );
  writeFieldValue( "Positive", element->getPositiveFlag() );
  writeFieldValue( "Raster format", element->getFormat() );
  writeFieldValue( "Foreground", element->getForeground() );
  writeFieldValue( "Background", element->getBackground() );
  writeFieldValue( "X extent", element->getXExtent() );
  writeFieldValue( "Y extent", element->getYExtent() );
  writeFieldValue( "Scale", element->getScale() );
  {
    OdGePoint3d origin = element->getOrigin();
    writeFieldValue( "Origin", origin );
  }

  //all parts
  describeSubElements( element->createIterator() );

  finishDescription();
}


OdString OdDgRasterHeader3d_Dumper::getClassName()
{
  return "OdDgRasterHeader3d";
}


void OdDgRasterComponent_Dumper::dump( OdRxObjectPtr object )
{
  OdDgRasterComponentPtr element = object;

  startDescription( object );

  //simple fields
  writeFieldValue( "Right justified", element->getRightJustifiedFlag() );
  writeFieldValue( "Lower justified", element->getLowerJustifiedFlag() );
  writeFieldValue( "Horizontal data", element->getHorizontalDataFlag() );
  writeFieldValue( "Color", element->getColorFlag() );
  writeFieldValue( "Transparent", element->getTransparentFlag() );
  writeFieldValue( "Positive", element->getPositiveFlag() );
  writeFieldValue( "Raster format", element->getFormat() );
  writeFieldValue( "Foreground", element->getForeground() );
  writeFieldValue( "Background", element->getBackground() );
  writeFieldValue( "Offset X", element->getOffsetX() );
  writeFieldValue( "Offset Y", element->getOffsetY() );
  writeFieldValue( "Number of pixels", element->getNumberOfPixels() );

  finishDescription();
}


OdString OdDgRasterComponent_Dumper::getClassName()
{
  return "OdDgRasterComponent";
}


void OdDgTagElement_Dumper::dump( OdRxObjectPtr object )
{
  OdDgTagElementPtr element = object;

  startDescription( object );

  //simple fields
  writeFieldValue( "Origin", element->getOrigin() );
  writeFieldValue( "Offset", element->getOffset() );
  writeFieldValue( "Rotation (3d)", element->getRotation3d() );
  writeFieldValue( "Associated", element->getAssociatedFlag() );
  writeFieldValue( "Offset is used", element->getOffsetUsedFlag() );
  {
    OdDgElementId setId;
    OdUInt16      definitionId;

    element->getTagDefinitionId( setId, definitionId );
    if( !setId.isNull() )
    {
      writeFieldValue( "Id of the set", setId );
      writeFieldValue( "Entry Id of the definition", definitionId );
      OdDgTagDefinitionSetPtr pTagSet =  setId.safeOpenObject();
      if (!pTagSet.isNull())
      {
        writeFieldValue( "TagDefinitionsSet Name", pTagSet->getName() );
        OdDgTagDefinitionPtr pTagDefinition = pTagSet->getByEntryId( definitionId );
        if (!pTagDefinition.isNull())
        {
          writeFieldValue( "TagDefinition Name", pTagDefinition->getName());
          writeFieldValue( "TagDefinition Type", pTagDefinition->getType());
        }
      }
    }
    else
    {
      fwprintf( DumpStream, L"Can't find TagDefinition\n" );
    }

  }
  writeFieldValue( "Id of the associated element", element->getAssociationId() );
  writeFieldValue( "Freeze group", element->getFreezeGroup() );

  switch ( element->getDataType() )
  {
  case OdDgTagDefinition::kChar:
    writeFieldValue( "Type", OdString( "char" ) );
    writeFieldValue( "Value", element->getString() );
    break;
  case OdDgTagDefinition::kInt16:
    writeFieldValue( "Type", OdString( "int16" ) );
    writeFieldValue( "Value", element->getInt16() );
    break;
  case OdDgTagDefinition::kInt32:
    writeFieldValue( "Type", OdString( "int32" ) );
    writeFieldValue( "Value", element->getInt32() );
    break;
  case OdDgTagDefinition::kDouble:
    writeFieldValue( "Type", OdString( "int16" ) );
    writeFieldValue( "Value", element->getDouble() );
    break;
  case OdDgTagDefinition::kBinary:
    writeFieldValue( "Type", OdString( "int16" ) );
    writeFieldValue( "Size", element->getBinarySize() );
    break;
  default:
    ODA_FAIL();
    break;
  }

  writeFieldValue( "Use interChar spacing", element->getUseInterCharSpacingFlag() );
  writeFieldValue( "Fixed width spacing", element->getFixedWidthSpacingFlag() );
  writeFieldValue( "Underlined", element->getUnderlineFlag() );
  writeFieldValue( "Use slant", element->getUseSlantFlag() );
  writeFieldValue( "Vertical", element->getVerticalFlag() );
  writeFieldValue( "Right-to-left", element->getRightToLeftFlag() );
  writeFieldValue( "Reverse MLine", element->getReverseMlineFlag() );
  writeFieldValue( "Kerning", element->getKerningFlag() );
  writeFieldValue( "Use codepage", element->getUseCodepageFlag() );
  writeFieldValue( "Use SHX big font", element->getUseShxBigFontFlag() );
  writeFieldValue( "Subscript", element->getSubscriptFlag() );
  writeFieldValue( "Superscript", element->getSuperscriptFlag() );
  writeFieldValue( "Use text style", element->getUseTextStyleFlag() );
  writeFieldValue( "Overlined", element->getOverlineFlag() );
  writeFieldValue( "Bold", element->getBoldFlag() );
  writeFieldValue( "Full justification", element->getFullJustificationFlag() );
  writeFieldValue( "ACAD interChar spacing", element->getAcadInterCharSpacingFlag() );
  writeFieldValue( "Backwards", element->getBackwardsFlag() );
  writeFieldValue( "Upside down", element->getUpsideDownFlag() );
  writeFieldValue( "ACAD fitted text", element->getAcadFittedTextFlag() );

  writeFieldValue( "Slant", element->getSlant() );
  writeFieldValue( "Character spacing", element->getCharacterSpacing() );
  writeFieldValue( "Underline spacing", element->getUnderlineSpacing() );
  writeFieldValue( "Length multiplier", element->getLengthMultiplier() );
  writeFieldValue( "Height multiplier", element->getHeightMultiplier() );

  writeFieldValue( "Text style ID", element->getTextStyleEntryId() );
  writeFieldValue( "SHX big font", element->getShxBigFont() );
  writeFieldValue( "Font ID", element->getFontEntryId() );
  writeFieldValue( "Justification", element->getJustification() );
  writeFieldValue( "Codepage", element->getCodepage() );
 
  finishDescription();
}


OdString OdDgTagElement_Dumper::getClassName()
{
  return "OdDgTagElement";
}


void OdDgCellHeader2d_Dumper::dump( OdRxObjectPtr object )
{
  OdDgCellHeader2dPtr element = object;

  startDescription( object );

  describeSubElements( element->createIterator() );

  finishDescription();
}


OdString OdDgCellHeader2d_Dumper::getClassName()
{
  return "OdDgCellHeader2d";
}


void OdDgCellHeader3d_Dumper::dump( OdRxObjectPtr object )
{
  OdDgCellHeader3dPtr element = object;

  startDescription( object );

  describeSubElements( element->createIterator() );

  finishDescription();
}


OdString OdDgCellHeader3d_Dumper::getClassName()
{
  return "OdDgCellHeader3d";
}


void OdDgBSplineSurface_Dumper::dump( OdRxObjectPtr object )
{
  OdDgBSplineSurfacePtr element = object;

  startDescription( object );

  OdUInt8 uOrderU, uOrderV;
  bool    bRational, bClosedInU, bClosedInV;
  int     nCtrlPtsInU, nCtrlPtsInV;
  OdGePoint3dArray arrCtrlPts;
  OdGeDoubleArray  arrWeights;
  OdGeKnotVector   arrKnotsU, arrKnotsV;
  OdUInt32 nRulesU, nRulesV;

  element->get( uOrderU, uOrderV, bRational, bClosedInU, bClosedInV, nCtrlPtsInU, nCtrlPtsInV, arrCtrlPts,
                arrWeights, arrKnotsU, arrKnotsV );
  element->getNumberOfSpansInU( nRulesU );
  element->getNumberOfSpansInV( nRulesV );

  //simple fields
  writeFieldValue( "Order U",  uOrderU );
  writeFieldValue( "Order V",  uOrderV );
  writeFieldValue( "Closed U", bClosedInU );
  writeFieldValue( "Closed V", bClosedInV );
  writeFieldValue( "Display curve", element->getSurfaceDisplayFlag() );
  writeFieldValue( "Display polygon", element->getControlNetDisplayFlag() );
  writeFieldValue( "Rational", bRational );
  writeFieldValue( "Number of rules U", nRulesU );
  writeFieldValue( "Number of rules V", nRulesV );
  writeFieldValue( "Number of poles U", nCtrlPtsInU );
  writeFieldValue( "Number of poles V", nCtrlPtsInV );
  writeFieldValue( "Number of knots U", arrKnotsU.length() );
  writeFieldValue( "Number of knots V", arrKnotsV.length() );
  writeFieldValue( "Number of boundaries", element->getBoundariesCount() );
  writeFieldValue( "Hole", element->getHoleFlag() );

  for (OdUInt32 nBoundariesCount = element->getBoundariesCount(), i = 0;
       i < nBoundariesCount;
       i++
      )
  {
    writeShift();
    fwprintf( DumpStream, L"Boundary %d:\n", i );
    OdGePoint2dArray arrBoundaryPts;
    element->getBoundary(i, arrBoundaryPts);
    writeFieldValue( "Number of boundary vertices", (OdUInt32)arrBoundaryPts.size() );
    for (OdUInt32 BoundaryVerticesCount = arrBoundaryPts.size(), j = 0;
         j < BoundaryVerticesCount;
         j++
         )
    {
      writeFieldValue( "Vertex", arrBoundaryPts[j] );
    }
  }

  finishDescription();
}


OdString OdDgBSplineSurface_Dumper::getClassName()
{
  return "OdDgBSplineSurface";
}


void OdDgLevelTable_Dumper::dump( OdRxObjectPtr object )
{
  OdDgLevelTablePtr element = object;
  element->upgradeOpen();
  element->generateUsageData();
  element->downgradeOpen();

  startDescription( object );

  describeSubElements( element->createIterator() );  

  finishDescription();
}

OdString OdDgLevelTable_Dumper::getClassName()
{
  return "OdDgLevelTable";
}

OdString OdDgLevelFilterTable_Dumper::getClassName()
{
  return "OdDgLevelFilterTable";
}

void OdDgLevelFilterTable_Dumper::dump( OdRxObjectPtr object )
{
  OdDgLevelFilterTablePtr element = object;

  startDescription( object );

  writeFieldValue( "Active Filter ID", (OdInt32)(element->getActiveFilterId()) );

  writeShift();

  for( OdUInt32 i = 0; i < element->getFilterMemberCount(); i++ )
  {
    OdString strMemberName;
    OdDgLevelFilterTable::OdDgFilterMemberType iMemberType;

    if( element->getFilterMember(i, strMemberName, iMemberType ) )
    {
      strMemberName = OD_T("  ") + strMemberName;
      writeFieldValue( strMemberName, iMemberType );
    }
  }

  describeSubElements( element->createIterator() );  

  finishDescription();
}

void OdDgLevelTableRecord_Dumper::dump( OdRxObjectPtr object )
{
  OdDgLevelTableRecordPtr pLevel = object;

  startDescription( object );

  writeFieldValue( "Level Name ", pLevel->getName() );
  writeFieldValue( "Level Number", pLevel->getNumber() );

  writeFieldValue( "Entry ID", pLevel->getEntryId() );
  writeFieldValue( "Code", pLevel->getNumber() );

  writeFieldValue( "Element Color (byLevel)", pLevel->getElementColorIndex() );
  OdDgCmColor colorBase;
  pLevel->getElementColor(colorBase);
  OdCmEntityColor eColor = colorBase.entityColor();
  writeFieldValue( "Element Color (colorBase)", eColor );
  writeFieldValue( "Element LineStyle (byLevel)", pLevel->getElementLineStyleEntryId() );
  writeFieldValue( "Element LineWeight (byLevel)", pLevel->getElementLineWeight() );

  writeFieldValue( "Override Color", pLevel->getOverrideColorIndex() );
  writeFieldValue( "Override LineStyle", pLevel->getOverrideLineStyleEntryId() );
  writeFieldValue( "Override LineWeight", pLevel->getOverrideLineWeight() );

  writeFieldValue( "Use override color", pLevel->getUseOverrideColorFlag() );
  writeFieldValue( "Use override line style", pLevel->getUseOverrideLineStyleFlag() );
  writeFieldValue( "Use override line weight", pLevel->getUseOverrideLineWeightFlag() );

  writeFieldValue( "Displayed", pLevel->getIsDisplayedFlag() );
  writeFieldValue( "Can be Plotted", pLevel->getIsPlotFlag() );
  writeFieldValue( "Derived from a library level ", pLevel->getIsExternalFlag() );
  writeFieldValue( "Can be snapped ", pLevel->getIsSnapFlag() );
  writeFieldValue( "ReadOnly", pLevel->getIsReadOnlyFlag() );
  writeFieldValue( "Hidden", pLevel->getIsHiddenFlag() );
  writeFieldValue( "Frozen", pLevel->getIsFrozenFlag() );
  writeFieldValue( "CustomStyleFromMaster", pLevel->getIsCustomStyleFromMasterFlag() );
  writeFieldValue( "Displayed", pLevel->getIsDisplayedFlag() );
  writeFieldValue( "isInUse", pLevel->isInUse() );

  //writeFieldValueHex( "Element access flags", pLevel->getElementAccessFlags() );
  OdDgLevelTableRecord::ElementAccess elementAccess = pLevel->getElementAccess();
  writeShift();
  fwprintf( DumpStream, L"Element access" );
  switch( elementAccess )
  {
  case OdDgLevelTableRecord::kAccessAll:      fwprintf( DumpStream, L"All\n" ); break;
  case OdDgLevelTableRecord::kAccessLocked:   fwprintf( DumpStream, L"Locked\n" ); break;
  case OdDgLevelTableRecord::kAccessReadOnly: fwprintf( DumpStream, L"ReadOnly\n" ); break;
  case OdDgLevelTableRecord::kAccessViewOnly: fwprintf( DumpStream, L"ViewOnly\n" ); break;
  }

  writeFieldValue( "Description", pLevel->getDescription() );
  writeFieldValue( "Priority", pLevel->getPriority() );
  writeFieldValue( "Transparency", pLevel->getTransparency().alphaPercent() );

  //OdBinaryData dataT;
  //dataT = pLevel->getTextStyleEntryId();
  //writeShift(); 
  //fwprintf( DumpStream, L"TextStyle data size = %d", dataT.size() );	  
  //OdUInt32 nSize1 = dataT.size();
  //for (OdUInt32 i = 0; i < nSize1 ; i++)
  //{
  //  fwprintf( DumpStream, L" TextStyle data %d = %d \n", i, dataT.getAt( i ) );	  
  //}

  //OdBinaryData data;
  //data = pLevel->getUnknownData();
  //writeShift(); 
  //fwprintf( DumpStream, L"Unkown Level data size = %d", data.size() );	  
  //OdUInt32 nSize = data.size();
  //for (OdUInt32 i = 0; i < nSize ; i++)
  //{
  //  fwprintf( DumpStream, L" Unkown %d = %d \n", i, data.getAt( i ) );	  
  //}

  finishDescription();
}

OdString OdDgLevelTableRecord_Dumper::getClassName()
{
  return "OdDgLevelTableRecord";
}


void OdDgLevelFilterTableRecord_Dumper::dump( OdRxObjectPtr object )
{
  OdDgLevelFilterTableRecordPtr pFilter = object;

  startDescription( object );

  writeFieldValue( "Filter Name ", pFilter->getName() );
  writeFieldValue( "Entry ID", pFilter->getEntryId() );
  writeFieldValue( "Parent ID", (OdInt32)(pFilter->getParentId()) );
  writeFieldValue( "Filter Type", pFilter->getFilterType() );
  writeFieldValue( "Compose Or Flag", pFilter->getComposeOrFlag() );

  writeShift();

  bool bAddData = false;

  for( OdUInt32 i = 0 ; i < pFilter->getFilterMemberCount(); i++ )
  {
    OdString strMemberName       = pFilter->getFilterMemberName(i);
    OdString strMemberExpression = pFilter->getFilterMemberExpression(i);

    if( !strMemberExpression.isEmpty() )
    {
      strMemberName = OD_T("  ") + strMemberName;

      writeFieldValue( strMemberName, strMemberExpression );
      bAddData = true;
    }
  }

  finishDescription();
}


OdString OdDgLevelFilterTableRecord_Dumper::getClassName()
{
  return "OdDgLevelFilterTableRecord";
}

void OdDgFontTable_Dumper::dump( OdRxObjectPtr object )
{
  OdDgFontTablePtr element = object;

  startDescription( object );

  describeSubElements( element->createIterator() );  

  finishDescription();
}


OdString OdDgFontTable_Dumper::getClassName()
{
  return "OdDgFontTable";
}


void OdDgFontTableRecord_Dumper::dump( OdRxObjectPtr object )
{
  OdDgFontTableRecordPtr pFont = object;

  startDescription( object );

  writeFieldValue( "Font name", pFont->getName() );
  writeFieldValue( "Font number", pFont->getNumber() );
  writeFieldValue( "Entry ID", pFont->getNumber() );
  writeFieldValue( "Alternate Font name", pFont->getAlternateName() );
  writeFieldValue( "Font type", pFont->getType() );

  finishDescription();
}


OdString OdDgFontTableRecord_Dumper::getClassName()
{
  return "OdDgFontTableRecord";
}


void OdDgTextStyleTable_Dumper::dump( OdRxObjectPtr object )
{
  OdDgTextStyleTablePtr element = object;

  startDescription( object );

  describeSubElements( element->createIterator() );  

  finishDescription();
}


OdString OdDgTextStyleTable_Dumper::getClassName()
{
  return "OdDgTextStyleTable";
}


void OdDgTextStyleTableRecord_Dumper::dump( OdRxObjectPtr object )
{
  OdDgTextStyleTableRecordPtr element = object;

  startDescription( object );

  writeFieldValue( "Text Style Name", element->getName() );
  writeFieldValue( "Entry ID", element->getEntryId() );

  writeFieldValue( "Font number", element->getFontEntryId() );
  // Gets Font name
  OdDgFontTablePtr pFontTable = element->database()->getFontTable();
  OdDgFontTableRecordPtr pFont = pFontTable->getFont(element->getFontEntryId());
  if (!pFont.isNull())
  {
    writeFieldValue( "Font Name", pFont->getName() );
  }

  writeFieldValue( "Justification", element->getJustification() );
  writeFieldValue( "Line length", element->getLineLength() );
  writeFieldValue( "Line offset", element->getLineOffset() );
  writeFieldValue( "Line spacing", element->getLineSpacing() );
  writeFieldValue( "Font number", element->getFontEntryId() );
  writeFieldValue( "Interchar spacing", element->getInterCharacter() );
  writeFieldValue( "Parent text style ID", element->getParentTextStyleEntryId() );
  writeFieldValue( "ShxBigFontId", element->getShxBigFontId() );
  writeFieldValue( "Slant", element->getSlant() );
  writeFieldValue( "Direction", element->getTextDirection() );
  writeFieldValue( "Height", element->getTextHeight() );
  writeFieldValue( "Width", element->getTextWidth() );
  writeFieldValue( "Node justification", element->getTextNodeJustification() );
  //writeFieldValue( "Is from table", element->getIsFromTableFlag() );

  writeFieldValue( "Underline", element->getUnderlineFlag() );
  writeFieldValue( "Overline", element->getOverlineFlag() );
  writeFieldValue( "Italic", element->getItalicsFlag() );
  writeFieldValue( "Bold", element->getBoldFlag() );
  writeFieldValue( "Superscript", element->getSuperscriptFlag() );
  writeFieldValue( "Subscript", element->getSubscriptFlag() );
  writeFieldValue( "Background flag", element->getBackgroundFlag() );
  writeFieldValue( "OverlineStyle", element->getOverlineStyleFlag() );
  writeFieldValue( "UnderlineStyle", element->getUnderlineStyleFlag() );
  writeFieldValue( "Fixed spacing", element->getFixedSpacingFlag() );
  writeFieldValue( "Fractions", element->getFractionsFlag() );
  writeFieldValue( "Color flag", element->getColorFlag() );
  writeFieldValue( "AcadIntercharSpacingFlag", element->getAcadIntercharSpacingFlag() );
  writeFieldValue( "FullJustificationFlag", element->getFullJustificationFlag() );
  writeFieldValue( "AcadShapeFileFlag", element->getAcadShapeFileFlag() );

  writeFieldValue( "Background border", element->getBackgroundBorder() );
  writeFieldValue( "Background fill color index", element->getBackgroundFillColorIndex() );
  writeFieldValue( "Background line color index", element->getBackgroundColorIndex() );
  writeFieldValue( "Background line style entry id", element->getBackgroundLineStyleEntryId() );
  writeFieldValue( "Background line weight", element->getBackgroundLineWeight() );
  writeFieldValue( "Overline line color", element->getOverlineColorIndex() );
  writeFieldValue( "Overline line style ID", element->getOverlineLineStyleEntryId() );
  writeFieldValue( "Overline line weight", element->getOverlineLineWeight() );
  writeFieldValue( "Overline offset", element->getOverlineOffset() );
  writeFieldValue( "Underline line color", element->getUnderlineColorIndex() );
  writeFieldValue( "Underline line style ID", element->getUnderlineLineStyleEntryId() );
  writeFieldValue( "Underline line weight", element->getUnderlineLineWeight() );
  writeFieldValue( "Underline offset", element->getUnderlineOffset() );
  writeFieldValue( "LineSpacinType offset", element->getLineSpacingType() );

  writeFieldValue( "Color index", element->getColorIndex() );
  if (element->getColorIndex() == OdDg::kColorByLevel)
  {
    fwprintf( DumpStream, L"TextStyle has ColorByLevel.\n" );
  }

  writeFieldValue( "ColorFlagOverrideFlag", element->getColorFlagOverrideFlag() );
  writeFieldValue( "TextColorOverrideFlag", element->getColorOverrideFlag() );

  finishDescription();
}


OdString OdDgTextStyleTableRecord_Dumper::getClassName()
{
  return "OdDgTextStyleTableRecord";
}


void OdDgDimStyleTable_Dumper::dump( OdRxObjectPtr object )
{
  OdDgDimStyleTablePtr element = object;

  startDescription( object );

  describeSubElements( element->createIterator() );  

  finishDescription();
}


OdString OdDgDimStyleTable_Dumper::getClassName()
{
  return "OdDgDimStyleTable";
}


void OdDgDimStyleTableRecord_Dumper::dump( OdRxObjectPtr object )
{
  OdDgDimStyleTableRecordPtr element = object;

  startDescription( object );

  writeFieldValue( "Show dimension line", element->getShowDimLineFlag() );
  writeFieldValue( "Show ext line", element->getShowExtLineFlag() );
  writeFieldValue( "Show tolerance", element->getShowToleranceFlag() );
  writeFieldValue( "Main prefix", element->getMainPrefix() );
  writeFieldValue( "Main suffix", element->getMainSuffix() );
  writeFieldValue( "Tolerance prefix", element->getTolerancePrefix() );
  writeFieldValue( "Tolerance suffix", element->getToleranceSuffix() );
  writeFieldValue( "Upper prefix", element->getUpperPrefix() );
  writeFieldValue( "Upper suffix", element->getUpperSuffix() );
  writeFieldValue( "Lower prefix", element->getLowerPrefix() );
  writeFieldValue( "Lower suffix", element->getLowerSuffix() );

  finishDescription();
}


OdString OdDgDimStyleTableRecord_Dumper::getClassName()
{
  return "OdDgDimStyleTableRecord";
}


void OdDgMultilineStyleTable_Dumper::dump( OdRxObjectPtr object )
{
  OdDgMultilineStyleTablePtr element = object;

  startDescription( object );

  describeSubElements( element->createIterator() );  

  finishDescription();
}


OdString OdDgMultilineStyleTable_Dumper::getClassName()
{
  return "OdDgMultilineStyleTable";
}


void OdDgMultilineStyleTableRecord_Dumper::dump( OdRxObjectPtr object )
{
  OdDgMultilineStyleTableRecordPtr element = object;

  startDescription( object );

  writeFieldValue( "Uses fill color", element->getUseFillColorFlag() );
  writeFieldValue( "Fill color", element->getFillColorIndex() );
  writeFieldValue( "Origin cap angle", element->getOriginCapAngle() );
  writeFieldValue( "End cap angle", element->getEndCapAngle() );
  {
    OdDgMultilineSymbology symbology;

    element->getOriginCap( symbology );
    writeFieldValue( "Origin cap", symbology );
    element->getMiddleCap( symbology );
    writeFieldValue( "End cap", symbology );
    element->getEndCap( symbology );
    writeFieldValue( "Middle cap", symbology );
  }

  {
    OdDgMultilineProfile  profile;
    int                   i, j = element->getProfilesCount();
    char                    fieldName[ 20 ];

    writeShift();
    fwprintf( DumpStream, L"Number of profiles: %d\n", j );

    for( i = 0; i < j; i++ )
    {
      sprintf( fieldName, "Profile %d", i );
      element->getProfile( i, profile );
      writeFieldValue( fieldName, profile );
    }
  }

  finishDescription();
}


OdString OdDgMultilineStyleTableRecord_Dumper::getClassName()
{
  return "OdDgMultilineStyleTableRecord";
}


void OdDgLineStyleTable_Dumper::dump( OdRxObjectPtr object )
{
  OdDgLineStyleTablePtr element = object;

  startDescription( object );

  describeSubElements( element->createIterator() );  

  finishDescription();
}


OdString OdDgLineStyleTable_Dumper::getClassName()
{
  return "OdDgLineStyleTable";
}


void OdDgLineStyleTableRecord_Dumper::dump( OdRxObjectPtr object )
{
  OdDgLineStyleTableRecordPtr element = object;

  startDescription( object );

  writeFieldValue( "LineStyle name", element->getName() );
  writeFieldValue( "EntryId", element->getEntryId() );

  finishDescription();
}


OdString OdDgLineStyleTableRecord_Dumper::getClassName()
{
  return "OdDgLineStyleTableRecord";
}


void OdDgRegAppTable_Dumper::dump( OdRxObjectPtr object )
{
  OdDgRegAppTablePtr element = object;

  startDescription( object );

  describeSubElements( element->createIterator() );  

  finishDescription();
}


OdString OdDgRegAppTable_Dumper::getClassName()
{
  return "OdDgRegAppTable";
}


void OdDgRegAppTableRecord_Dumper::dump( OdRxObjectPtr object )
{
  OdDgRegAppTableRecordPtr element = object;

  startDescription( object );
  finishDescription();
}


OdString OdDgRegAppTableRecord_Dumper::getClassName()
{
  return "OdDgRegAppTableRecord";
}


void OdDgViewGroupTable_Dumper::dump( OdRxObjectPtr object )
{
  OdDgViewGroupTablePtr element = object;

  startDescription( object );

  describeSubElements( element->createIterator() );  

  finishDescription();
}


OdString OdDgViewGroupTable_Dumper::getClassName()
{
  return "OdDgViewGroupTable";
}


void OdDgViewGroup_Dumper::dump( OdRxObjectPtr object )
{
  OdDgViewGroupPtr element = object;

  startDescription( object );
  
  writeFieldValue( "Name", element->getName() );

  describeSubElements( element->createIterator() );  

  finishDescription();
}


OdString OdDgViewGroup_Dumper::getClassName()
{
  return "OdDgViewGroup";
}


void OdDgView_Dumper::dump( OdRxObjectPtr object )
{
  OdDgViewPtr element = object;

  startDescription( object );

  writeFieldValue( "Model ID", element->getModelId() );
 
  writeFieldValue( "Is named", element->isNamed() );
  writeFieldValue( "Name", element->getName() );

  writeFieldValue( "Index", element->getIndex() );

  writeFieldValue( "View rectangle", element->getViewRectangle() );

  writeFieldValue( "Is visible", element->getVisibleFlag() );
  writeFieldValue( "Fast curves", element->getFastCurveFlag() );
  writeFieldValue( "Hide texts", element->getHideTextsFlag() );
  writeFieldValue( "High quality fonts", element->getHighQualityFontsFlag() );
  writeFieldValue( "Show line weights", element->getShowLineWeightsFlag() );
  writeFieldValue( "Show patterns", element->getShowPatternsFlag() );
  writeFieldValue( "Show text nodes", element->getShowTextNodesFlag() );
  writeFieldValue( "Show data fields", element->getShowDataFieldsFlag() );
  writeFieldValue( "Show grid", element->getShowGridFlag() );
  writeFieldValue( "Show level symbology", element->getShowLevelSymbologyFlag() );
  writeFieldValue( "Show construction", element->getShowConstructionFlag() );
  writeFieldValue( "Show dimensions", element->getShowDimensionsFlag() );
  writeFieldValue( "Fast cells", element->getFastCellsFlag() );
  writeFieldValue( "Show fills", element->getShowFillsFlag() );
  writeFieldValue( "Show axis triad", element->getShowAxisTriadFlag() );
  writeFieldValue( "Show background", element->getShowBackgroundFlag() );
  writeFieldValue( "Show boundary", element->getShowBoundaryFlag() );

  if( element->getUseCameraFlag() )
  {
    writeShift();
    fwprintf( DumpStream, L"Specific parameters for perspective camera:\n" );
    m_nesting++;

    OdGeMatrix3d rotation;
    OdGePoint3d position;
    OdGeExtents2d rectangle;
    element->getCameraRotation( rotation );
    element->getCameraPosition( position );
    element->getCameraVisibleRectangle( rectangle );

    writeFieldValue( "Position", position );
    writeFieldValue( "Rotation", rotation );
    writeFieldValue( "Focal length", element->getCameraFocalLength() );
    writeFieldValue( "Visible rectangle", rectangle );
    writeFieldValue( "Front clipping", element->getCameraFrontClippingDistance() );
    writeFieldValue( "Back clipping", element->getCameraBackClippingDistance() );

    m_nesting--;
  }
  else
  {
    writeShift();
    fwprintf( DumpStream, L"Specific parameters for orthography:\n" );
    m_nesting++;

    OdGeMatrix3d rotation;
    OdGeExtents3d box;
    element->getOrthographyRotation( rotation );
    element->getOrthographyVisibleBox( box );

    writeFieldValue( "Rotation", rotation );
    writeFieldValue( "Visible box", box );

    m_nesting--;
  }

  OdDgElementIteratorPtr pIter = element->createIterator();
  for(pIter->start(); !pIter->done(); pIter->step())
  {
    OdDgElementPtr SubElement = pIter->item().openObject();
    if(SubElement->isKindOf(OdDgLevelMask::desc()))
    {
      OdDgLevelMaskPtr levelmask = SubElement;
      char  sLevelMask[ 200 ];
      OdUInt32 maxLevelEntryId = levelmask->getMaxLevelEntryId();

      OdDgElementIteratorPtr pIt = element->database()->getLevelTable()->createIterator();
      for( ; !pIt->done(); pIt->step() )
      {
        OdDgLevelTableRecordPtr pLevel = pIt->item().safeOpenObject();
        OdUInt32 levelEntryId = pLevel->getEntryId();
        sprintf( sLevelMask, "LevelMask %d %ls", levelEntryId, pLevel->getName().c_str() );
        bool levelIsVisible = true;
        if ( levelEntryId <= maxLevelEntryId || levelEntryId == 64 )
        {
          levelIsVisible = levelmask->getLevelIsVisible( levelEntryId );
          writeFieldValue( sLevelMask, levelIsVisible );
        }
        else
        {
          writeFieldValue( sLevelMask, "Unexpected" );
        }
      }
    }
  }

  finishDescription();
}


OdString OdDgView_Dumper::getClassName()
{
  return "OdDgView";
}


void OdDgNamedViewTable_Dumper::dump( OdRxObjectPtr object )
{
  OdDgNamedViewTablePtr element = object;

  startDescription( object );

  describeSubElements( element->createIterator() );  

  finishDescription();
}


OdString OdDgNamedViewTable_Dumper::getClassName()
{
  return "OdDgNamedViewTable";
}


void OdDgSharedCellDefinitionTable_Dumper::dump( OdRxObjectPtr object )
{
  OdDgSharedCellDefinitionTablePtr element = object;

  startDescription( object );
  
  describeSubElements( element->createIterator() );  

  finishDescription();
}


OdString OdDgSharedCellDefinitionTable_Dumper::getClassName()
{
  return "OdDgSharedCellDefinitionTable";
}


void OdDgSharedCellDefinition_Dumper::dump( OdRxObjectPtr object )
{
  OdDgSharedCellDefinitionPtr element = object;

  startDescription( object );

  writeFieldValue( "Name", element->getName() );
  writeFieldValue( "Description", element->getDescription() );

  writeFieldValue( "Origin", element->getOrigin() );
  
  describeSubElements( element->createIterator() );  

  finishDescription();
}


OdString OdDgSharedCellDefinition_Dumper::getClassName()
{
  return "OdDgSharedCellDefinition";
}


void OdDgSharedCellReference_Dumper::dump( OdRxObjectPtr object )
{
  OdDgSharedCellReferencePtr element = object;

  startDescription( object );

  writeFieldValue( "Name of definition", element->getDefinitionName() );

  writeFieldValue( "Transformation", element->getTransformation() );
  writeFieldValue( "Origin", element->getOrigin() );

  writeFieldValueHex( "Class map", element->getClassMap() );

  writeFieldValue( "Override level", element->getLevelOverrideFlag() );
  writeFieldValue( "Override relative", element->getRelativeOverrideFlag() );
  writeFieldValue( "Override class", element->getClassOverrideFlag() );
  writeFieldValue( "Override color", element->getColorOverrideFlag() );
  writeFieldValue( "Override weight", element->getWeightOverrideFlag() );
  writeFieldValue( "Override style", element->getStyleOverrideFlag() );
  writeFieldValue( "Override associative point", element->getAssociativePointOverrideFlag() );

  writeFieldValue( "ColorIndex", element->getColorIndex() );
  writeFieldValue( "LineWeight", element->getLineWeight() );
  writeFieldValue( "LevelEntryId", element->getLevelEntryId() );
  OdUInt32 level = element->getLevelEntryId();
  OdDgElementId idLevel = element->database()->getLevelTable()->getAt( level );
  if (idLevel.isNull())
  {
    fwprintf( DumpStream, L"level id is incorrect\n" );
  }
  else
  {
    OdDgLevelTableRecordPtr pLevel = idLevel.safeOpenObject();
    writeFieldValue( "Level Name ", pLevel->getName() );
  }

  finishDescription();
}


OdString OdDgSharedCellReference_Dumper::getClassName()
{
  return "OdDgSharedCellReference";
}


void OdDgTagDefinitionSetTable_Dumper::dump( OdRxObjectPtr object )
{
  OdDgTagDefinitionSetTablePtr element = object;

  startDescription( object );

  describeSubElements( element->createIterator() );  

  finishDescription();
}


OdString OdDgTagDefinitionSetTable_Dumper::getClassName()
{
  return "OdDgTagDefinitionSetTable";
}


void OdDgTagDefinitionSet_Dumper::dump( OdRxObjectPtr object )
{
  OdDgTagDefinitionSetPtr pTagDefinitionsSet = object;

  startDescription( object );

  writeFieldValue( "Name", pTagDefinitionsSet->getName() );
  {
    OdUInt32                        i, j = pTagDefinitionsSet->getCount();
    OdRxObjectPtr                   object;
    OdSmartPtr< OdRxObject_Dumper > dumper;

    writeFieldValue( "Number of definitions", j );
    for( i = 0; i < j; i++ )
    {
      object = pTagDefinitionsSet->getByIndex( i );

      //MUshakov, 26/05/2010.
      //At the moment, tag definition sets can wrap some information for SmartSolid objects.
      //In this case, they collect OdDgTagDefinitionDgnStore elements.
      //The latters have no dumpers, so let us avoid it without exceptions 'no dumper'.
      dumper = OdRxObject_Dumper::cast( object );
      if( !dumper.isNull() )
      {
        dumper->dump( object );
      }
      else
      {
        m_nesting++;
        fwprintf( DumpStream, L"Object does not have its dumper\n" );
        m_nesting--;
      }
    }

    if( pTagDefinitionsSet->getProxyTagDefinitionSetFlag() )
    {
      fwprintf( DumpStream, L"      Tag definition set contains proxy data:\n");

      OdBinaryData binProxyData;
      pTagDefinitionsSet->getBinaryData( binProxyData );
      writeFieldValue( "Number proxy bytes", (OdUInt32)(binProxyData.size()) );

      OdUInt32                        i, j = binProxyData.size();

      for( i = 0; i < j; i++ )
      {
        if( !( i % 16 ) )
        {
          if( i )
          {
            fwprintf( DumpStream, L"\n" );
          }
          writeShift();
          fwprintf( DumpStream, L"%.4X: ", i );
        }
        fwprintf( DumpStream, L"%.2X ", binProxyData[ i ] );
      }
      fwprintf( DumpStream, L"\n" );
    }
  }

  finishDescription();
}


OdString OdDgTagDefinitionSet_Dumper::getClassName()
{
  return "OdDgTagDefinitionSet";
}


void OdDgTagDefinition_Dumper::dump( OdRxObjectPtr object )
{
  OdDgTagDefinitionPtr  element = object;

  startDescription( object );

  writeFieldValue( "Type", element->getType() );
  writeFieldValue( "ID", element->getEntryId() );
  writeFieldValue( "Name", element->getName() );
  writeFieldValue( "Prompt", element->getPrompt() );
  switch( element->getType() )
  {
  case OdDgTagDefinition::kChar:
    writeFieldValue( "Default char value", element->getString() );
    break;
  case OdDgTagDefinition::kInt16 :
    writeFieldValue( "Default int16 value", element->getInt16() );
    break;
  case OdDgTagDefinition::kInt32 :
    writeFieldValue( "Default int32 value", element->getInt32() );
    break;
  case OdDgTagDefinition::kDouble:
    writeFieldValue( "Default double value", element->getDouble() );
    break;
  case OdDgTagDefinition::kBinary:
    writeFieldValue( "Default binary data (size)", element->getBinarySize() );
    break;
  default:
    ODA_FAIL();
  }
 
  finishDescription();
}


OdString OdDgTagDefinition_Dumper::getClassName()
{
  return "OdDgTagDefinition";
}


void OdDgColorTable_Dumper::dump( OdRxObjectPtr object )
{
  OdDgColorTablePtr element = object;
  OdInt32               i;
  char              name[ 200 ];
 

  startDescription( object );

  for( i = 0; i < 256; i++ )
  {
    ODCOLORREF color = element->lookupRGB(i);
    OdInt32 red = ODGETRED(color);
    OdInt32 green = ODGETGREEN(color);
    OdInt32 blue = ODGETBLUE(color);

    sprintf( name, "Color %i - %i, %i, %i", i, red, green, blue );
    writeFieldValue( name, element->lookupRGB(i) /*OdUInt32( i )*/ );
  }

  OdDgDatabase* pDb = element->database();
  bool bExtendedColorTableTitle(false);
  for( i = 1; i < 64; i++ )
  {
    OdUInt32 colorIndex = i << 8;
    if (OdDgColorTable::isCorrectIndex( pDb, colorIndex ))
    {
      if (!bExtendedColorTableTitle)
      {
        fwprintf( DumpStream, L"  Extended Color Table:\n" );
        bExtendedColorTableTitle = true;
      }

      OdDgCmEntityColor trueColor = OdDgColorTable::lookupTrueColor( pDb, colorIndex );
      writeFieldValue( " True Color Index", colorIndex );
      writeFieldValueColorref( "True Color", OdDgColorTable::lookupRGB( pDb, colorIndex ) );
    }
  }

  finishDescription();
}


OdString OdDgColorTable_Dumper::getClassName()
{
  return "OdDgColorTable";
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
  //writeFieldValue( "Entire Scale", element->getEntireScale() );
  writeFieldValue( "Foreign Unit ", (OdUInt16)(element->getForeignUnits()));
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

  finishDescription();
}

OdString OdDgReferenceHeader_Dumper::getClassName()
{
  return "OdDgReferenceAttachmentHeader";
}

void OdDgMeshFaceLoops_Dumper::dump( OdRxObjectPtr object )
{
  OdDgMeshFaceLoopsPtr element = object;

  startDescription( object );

  writeFieldValue( "Number of faces", element->getFacesNumber() );
  writeFieldValue( "Number of points", element->getPointsNumber() );

  finishDescription();
}

OdString OdDgMeshFaceLoops_Dumper::getClassName()
{
  return "OdDgMeshFaceLoops";
}

void OdDgMeshPointCloud_Dumper::dump( OdRxObjectPtr object )
{
  OdDgMeshPointCloudPtr  element = object;

  startDescription( object );

  writeFieldValue( "Number of points", element->getPointsNumber() );

  finishDescription();
}

OdString OdDgMeshPointCloud_Dumper::getClassName()
{
  return "OdDgMeshPointCloud";
}

void OdDgMeshTriangleList_Dumper::dump( OdRxObjectPtr object )
{
  OdDgMeshTriangleListPtr  element = object;

  startDescription( object );

  writeFieldValue( "Number of triangles", element->getTrianglesNumber() );

  finishDescription();
}

OdString OdDgMeshTriangleList_Dumper::getClassName()
{
  return "OdDgMeshTriangleList";
}

void OdDgMeshQuadList_Dumper::dump( OdRxObjectPtr object )
{
  OdDgMeshQuadListPtr  element = object;

  startDescription( object );

  writeFieldValue( "Number of quads", element->getQuadsNumber() );

  finishDescription();
}

OdString OdDgMeshQuadList_Dumper::getClassName()
{
  return "OdDgMeshQuadList";
}

void OdDgMeshTriangleGrid_Dumper::dump( OdRxObjectPtr object )
{
  OdDgMeshTriangleGridPtr  element = object;

  startDescription( object );

  writeFieldValue( "Number of rows", element->getRowsNumber() );
  writeFieldValue( "Size of row", element->getColumnsNumber() );

  finishDescription();
}

OdString OdDgMeshTriangleGrid_Dumper::getClassName()
{
  return "OdDgMeshTriangleGrid";
}

void OdDgMeshQuadGrid_Dumper::dump( OdRxObjectPtr object )
{
  OdDgMeshQuadGridPtr  element = object;

  startDescription( object );

  writeFieldValue( "Number of rows", element->getRowsNumber() );
  writeFieldValue( "Size of row", element->getColumnsNumber() );

  finishDescription();
}

OdString OdDgMeshQuadGrid_Dumper::getClassName()
{
  return "OdDgMeshQuadGrid";
}

void OdDgProxyElement_Dumper::dump( OdRxObjectPtr object )
{
  OdDgProxyElementPtr  element = object;

  startDescription( object );

  OdBinaryData data;
  element->getData(data);

  OdUInt32 i, j = data.size();
  writeFieldValue( "Data size", j );
  for( i = 0; i < j; i++ )
  {
    if( !( i % 16 ) )
    {
      if( i )
      {
        fwprintf( DumpStream, L"\n" );
      }
      writeShift();
      fwprintf( DumpStream, L"%.4X: ", i );
    }
    fwprintf( DumpStream, L"%.2X ", data[ i ] );
  }
  fwprintf( DumpStream, L"\n" );

  finishDescription();
}

OdString OdDgProxyElement_Dumper::getClassName()
{
  return "OdDgProxyElement";
}

void OdDgApplicationData_Dumper::dump( OdRxObjectPtr object )
{
  OdDgApplicationDataPtr  element = object;

  startDescription( object );

  writeFieldValue( "Signature", element->getSignature() );

  OdBinaryData data;
  element->getData(data);

  OdUInt32 i, j = data.size();
  writeFieldValue( "Data size", j );
  for( i = 0; i < j; i++ )
  {
    if( !( i % 16 ) )
    {
      if( i )
      {
        fwprintf( DumpStream, L"\n" );
      }
      writeShift();
      fwprintf( DumpStream, L"%.4X: ", i );
    }
    fwprintf( DumpStream, L"%.2X ", data[ i ] );
  }
  fwprintf( DumpStream, L"\n" );

  finishDescription();
}

OdString OdDgApplicationData_Dumper::getClassName()
{
  return "OdDgApplicationData";
}
