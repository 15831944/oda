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

#pragma once

#include "OdaCommon.h"
#include "Gi/GiDrawable.h"
#include "RxVariant.h"
#include "DgDatabase.h"
#include "DgLine.h"
#include "DgLineString.h"
#include "DgText.h"
#include "DgTextNode.h"
#include "DgShape.h"
#include "DgCurve.h"
#include "DgEllipse.h"
#include "DgArc.h"
#include "DgCone.h"
#include "DgComplexString.h"
#include "DgComplexShape.h"
#include "DgPointString.h"
#include "DgDimension.h"
#include "DgMultiline.h"
#include "DgBSplineCurve.h"
#include "Dg3DObject.h"
#include "DgRasterAttach.h"
#include "DgRaster.h"
#include "DgTagElement.h"
#include "DgCellHeader.h"
#include "DgBSplineSurface.h"
#include "DgLevelTableRecord.h"
#include "DgFontTableRecord.h"
#include "DgTextStyleTableRecord.h"
#include "DgDimStyleTableRecord.h"
#include "DgMultilineStyleTableRecord.h"
#include "DgLineStyleTableRecord.h"
#include "DgRegAppTableRecord.h"
#include "DgView.h"
#include "DgSharedCellDefinition.h"
#include "DgSharedCellReference.h"
#include "DgTagSetDefinition.h"
#include "DgReferenceAttach.h"
#include "Ge/GeExtents3d.h"
#include "RxObjectImpl.h"
#include "DgMesh.h"
#include "DgProxyElement.h"
#include "DgApplicationData.h"

extern FILE* DumpStream;     //the file for output to

/************************************************************************/
/* This class is the base class for dumpers of all objects.             */
/*                                                                      */
/* Every object class has this extension.                               */
/* Also, dumper is available (OdDgDumper) for all such dumpers.         */
/************************************************************************/


class OdRxObject_Dumper : public OdRxObjectImpl< OdRxObject >
{

  ODRX_DECLARE_MEMBERS( OdRxObject_Dumper );
public:

  OdRxObject_Dumper();

  virtual void      dump( OdRxObjectPtr object ) = 0; //each object class has to describe itself via that method
  virtual OdString  getClassName() = 0;               //each object class has to define its own name

  //some useful methods for the other dump()'s
  void startDescription( OdRxObjectPtr object );
  void finishDescription();
  void describeSubElements( OdDgElementIteratorPtr iterator );
  void describeSubElement( OdRxObjectPtr someObject );
  void writeShift();
  void writeElementInfo( OdRxObjectPtr object );
  void writeGraphicsElementInfo( OdRxObjectPtr object );
  void writeLinkagesInfo( OdRxObjectPtr object );

  //output for different types
  void writeFieldValue( const OdString & name, OdString const& value );
  void writeFieldValue( const OdString & name, OdUInt16 value );
  void writeFieldValueHex( const OdString & name, OdUInt16 value );
  void writeFieldValue( const OdString & name, OdUInt32 value );
  void writeFieldValue( const OdString& name, OdDgLevelFilterTable::OdDgFilterMemberType value );
  void writeFieldValueHex( const OdString & name, OdUInt32 value );
  void writeFieldValue( const OdString & name, double value );
  void writeFieldValue( const OdString & name, OdGePoint2d const& value );
  void writeFieldValue( const OdString & name, OdGePoint3d const& value );
  void writeFieldValue( const OdString & name, OdGeVector2d const& value );
  void writeFieldValue( const OdString & name, OdGeVector3d const& value );
  void writeFieldValue( const OdString & name, OdCmEntityColor const& value );
  void writeFieldValue( const OdString & name, OdDgLineStyleInfo const& value );
  void writeFieldValue( const OdString & name, OdUInt64 value );
  void writeFieldValue( const OdString & name, OdUInt8 value );
  void writeFieldValueHex( const OdString & name, OdUInt8 value );
  void writeFieldValue( const OdString & name, OdDgLightColor const& value );
  void writeFieldValue( const OdString & name, bool value );
  void writeFieldValue( const OdString & name, OdAngleCoordinate const& value );
  void writeFieldValue( const OdString & name, OdInt16 value );
  void writeFieldValue( const OdString & name, OdInt32 value );
#if !(defined(ODA_UNIXOS) && (OD_SIZEOF_LONG == 8))
  void writeFieldValue( const OdString & name, int value );
#endif
  void writeFieldValue( const OdString & name, OdInt64 value );
  void writeFieldValue( const OdString & name, OdDgModel::WorkingUnit value );
  void writeFieldValue( const OdString & name, OdDg::TextJustification value );
  void writeFieldValue( const OdString & name, OdGeQuaternion const& value );
  //void writeFieldValue( OdString const& name, TextAttributes const& value );
  void writeFieldValue( const OdString & name, OdDgGraphicsElement::Class value );
  void writeFieldValue( const OdString & name, OdGeMatrix2d const& value );
  void writeFieldValue( const OdString & name, OdDgDimension::ToolType value );
  void writeFieldValue( const OdString & name, OdDgDimPoint& value );
  void writeFieldValue( const OdString & name, OdDgDimTextInfo const& value );
  void writeFieldValue( const OdString & name, OdDgDimTextFormat const& value );
  void writeFieldValue( const OdString & name, OdDgDimTextFormat::Accuracy value );
  void writeFieldValue( const OdString & name, OdDgDimGeometry const& value );
  void writeFieldValue( const OdString & name, OdDgDimOptionPtr& value );
  void writeFieldValue( const OdString & name, OdDgDimOptionSymbolPtr& value );
  void writeFieldValue( const OdString & name, OdDgDimOptionTerminatorSymbologyPtr& value );
  void writeFieldValue( const OdString & name, OdDgDimOptionTolerancePtr& value );
  void writeFieldValue( const OdString & name, OdDgDimOptionTerminatorsPtr& value );
  void writeFieldValue( const OdString & name, OdDimOptionPrefixSuffixPtr& value );
  void writeFieldValue( const OdString & name, OdDgModel::UnitSystem& value );
  void writeFieldValue( const OdString & name, OdDgModel::UnitBase& value );
  void writeFieldValue( const OdString & name, OdDgDimOptionUnitsPtr& value );
  void writeFieldValue( const OdString & name, OdDgDimOptionAltFormatPtr& value );
  void writeFieldValue( const OdString & name, OdDgDimOptionOffsetPtr& value );
  void writeFieldValue( const OdString & name, OdDgDimOptionProxyCellPtr& value );
  void writeFieldValue( const OdString & name, OdDgMultilineSymbology const& value );
  void writeFieldValue( const OdString & name, OdDgMultilinePoint const& value );
  void writeFieldValue( const OdString & name, OdDgMultilineBreak const& value );
  void writeFieldValue( const OdString & name, OdDgMultilineProfile const& value );
  void writeFieldValue( const OdString & name, OdDgRaster::RasterFormat value );
  void writeFieldValue( const OdString & name, OdDgTagDefinition::Type value );
  void writeFieldValue( const OdString & name, OdDg::TextDirection value );
  void writeFieldValue( const OdString & name, OdGeMatrix3d const& value );
  void writeFieldValue( const OdString & name, OdGsDCRect const& value );
  void writeFieldValue( const OdString & name, OdDgElementId const& value );
  void writeFieldValue( const OdString & name, const OdVariant& value );
  void writeFieldValue( const OdString & name, OdGeExtents2d const& value );
  void writeFieldValue( const OdString & name, OdGeExtents3d const& value );
  void writeFieldValue( const OdString & name, OdDgTextExtendedProperties::LineSpacingType value );
  void writeFieldValue( const OdString & name, OdDgDimLabelLine::LabelLineDimensionMode value );
  void writeFieldValue( const OdString & name, OdDgDimTextInfo::FitOptions value );
  void writeFieldValue( const OdString & name, OdDgDimTextInfo::TextLocation value );
  void writeFieldValue( const OdString & name, OdDgDimTool::TerminatorType iType );
  void writeFieldValue( const OdString & name, OdDgDimTool::TextType iType );
  void writeFieldValue( const OdString & name, OdDgDimTool::CustomSymbol iSymbol );
  void writeFieldValue( const OdString & name, OdDgDimTool::Leader iLeader );
  void writeFieldValue( const OdString & name, OdDgDimensionPtr pElement );
  void writeFieldValue( const OdString & name, OdDgDimSizeArrowPtr pElement );
  void writeFieldValue( const OdString & name, OdDgDimSizeStrokePtr pElement );
  void writeFieldValue( const OdString & name, OdDgDimSingleLocationPtr pElement );
  void writeFieldValue( const OdString & name, OdDgDimStackedLocationPtr pElement );
  void writeFieldValue( const OdString & name, OdDgDimCustomLinearPtr pElement );
  void writeFieldValue( const OdString & name, OdDgDimAngleSizePtr pElement );
  void writeFieldValue( const OdString & name, OdDgDimArcSizePtr pElement );
  void writeFieldValue( const OdString & name, OdDgDimAngleLinesPtr pElement );
  void writeFieldValue( const OdString & name, OdDgDimAngleLocationPtr pElement );
  void writeFieldValue( const OdString & name, OdDgDimArcLocationPtr pElement );
  void writeFieldValue( const OdString & name, OdDgDimAngleAxisXPtr pElement );
  void writeFieldValue( const OdString & name, OdDgDimAngleAxisYPtr pElement );
  void writeFieldValue( const OdString & name, OdDgDimRadiusPtr pElement );
  void writeFieldValue( const OdString & name, OdDgDimRadiusExtendedPtr pElement );
  void writeFieldValue( const OdString & name, OdDgDimDiameterPtr pElement );
  void writeFieldValue( const OdString & name, OdDgDimDiameterExtendedPtr pElement );
  void writeFieldValue( const OdString & name, OdDgDimDiameterParallelPtr pElement );
  void writeFieldValue( const OdString & name, OdDgDimDiameterPerpendicularPtr pElement );
  void writeFieldValue( const OdString & name, OdDgDimOrdinatePtr pElement );
  void writeFieldValue( const OdString & name, OdDgDimCenterPtr pElement );
  void writeFieldValue( const OdString & name, OdDgDimOptionOffset::ChainType value );
  void writeFieldValue( const OdString & name, OdDgDimOptionOffset::LeaderAlignment value );
  void writeFieldValue( const OdString & name, OdDgSurface::Type value );
  void writeFieldValue( const OdString & name, OdDgSolid::Type value );
  void writeFieldValue( const OdString& name, OdDgTextExtendedProperties::Spacing value );
  void writeFieldValueColorref( const OdString & name, ODCOLORREF value );

  OdRxObjectPtr getObject(){ return m_object; }
  //it is common for all dumpers
  static int                  m_nesting;
private:

  OdRxObjectPtr m_object;
  //methods used only by this class
  void writeFieldName( const OdString& fieldName );

  long        m_references;     //used by each object separately

  //it is common for all dumpers
  static OdArray< OdString >  m_nestedTypes;

};


/************************************************************************/
/* Dumpers for all objects                                              */
/************************************************************************/


class OdDgDatabase_Dumper : public OdRxObject_Dumper
{

public:

  virtual void      dump( OdRxObjectPtr object );
  virtual OdString  getClassName();

private:
  void dumpSummaryInfo( OdDgDatabase* pDb );
};


class OdDgModel_Dumper : public OdRxObject_Dumper
{

public:

  virtual void      dump( OdRxObjectPtr object );
  virtual OdString  getClassName();

};


class OdDgLine2d_Dumper : public OdRxObject_Dumper
{

public:

  virtual void      dump( OdRxObjectPtr object );
  virtual OdString  getClassName();

};


class OdDgLine3d_Dumper : public OdRxObject_Dumper
{

public:

  virtual void      dump( OdRxObjectPtr object );
  virtual OdString  getClassName();

};


class OdDgLineString2d_Dumper : public OdRxObject_Dumper
{

public:

  virtual void      dump( OdRxObjectPtr object );
  virtual OdString  getClassName();

};


class OdDgLineString3d_Dumper : public OdRxObject_Dumper
{

public:

  virtual void      dump( OdRxObjectPtr object );
  virtual OdString  getClassName();

};


class OdDgText2d_Dumper : public OdRxObject_Dumper
{

public:

  virtual void      dump( OdRxObjectPtr object );
  virtual OdString  getClassName();

};


class OdDgText3d_Dumper : public OdRxObject_Dumper
{

public:

  virtual void      dump( OdRxObjectPtr object );
  virtual OdString  getClassName();

};


class OdDgTextNode2d_Dumper : public OdRxObject_Dumper
{

public:

  virtual void      dump( OdRxObjectPtr object );
  virtual OdString  getClassName();

};


class OdDgTextNode3d_Dumper : public OdRxObject_Dumper
{

public:

  virtual void      dump( OdRxObjectPtr object );
  virtual OdString  getClassName();

};


class OdDgShape2d_Dumper : public OdRxObject_Dumper
{

public:

  virtual void      dump( OdRxObjectPtr object );
  virtual OdString  getClassName();

};


class OdDgShape3d_Dumper : public OdRxObject_Dumper
{

public:

  virtual void      dump( OdRxObjectPtr object );
  virtual OdString  getClassName();

};


class OdDgCurve2d_Dumper : public OdRxObject_Dumper
{

public:

  virtual void      dump( OdRxObjectPtr object );
  virtual OdString  getClassName();

};


class OdDgCurve3d_Dumper : public OdRxObject_Dumper
{

public:

  virtual void      dump( OdRxObjectPtr object );
  virtual OdString  getClassName();

};


class OdDgEllipse2d_Dumper : public OdRxObject_Dumper
{

public:

  virtual void      dump( OdRxObjectPtr object );
  virtual OdString  getClassName();

};


class OdDgEllipse3d_Dumper : public OdRxObject_Dumper
{

public:

  virtual void      dump( OdRxObjectPtr object );
  virtual OdString  getClassName();

};


class OdDgArc2d_Dumper : public OdRxObject_Dumper
{

public:

  virtual void      dump( OdRxObjectPtr object );
  virtual OdString  getClassName();

};


class OdDgArc3d_Dumper : public OdRxObject_Dumper
{

public:

  virtual void      dump( OdRxObjectPtr object );
  virtual OdString  getClassName();

};


class OdDgCone_Dumper : public OdRxObject_Dumper
{

public:

  virtual void      dump( OdRxObjectPtr object );
  virtual OdString  getClassName();

};


class OdDgComplexString_Dumper : public OdRxObject_Dumper
{

public:

  virtual void      dump( OdRxObjectPtr object );
  virtual OdString  getClassName();

};


class OdDgComplexShape_Dumper : public OdRxObject_Dumper
{

public:

  virtual void      dump( OdRxObjectPtr object );
  virtual OdString  getClassName();

};


class OdDgPointString2d_Dumper : public OdRxObject_Dumper
{

public:

  virtual void      dump( OdRxObjectPtr object );
  virtual OdString  getClassName();

};


class OdDgPointString3d_Dumper : public OdRxObject_Dumper
{

public:

  virtual void      dump( OdRxObjectPtr object );
  virtual OdString  getClassName();

};


class OdDgDimension_Dumper : public OdRxObject_Dumper
{

public:

  virtual void      dump( OdRxObjectPtr object );
  virtual OdString  getClassName();

};


class OdDgMultiline_Dumper : public OdRxObject_Dumper
{

public:

  virtual void      dump( OdRxObjectPtr object );
  virtual OdString  getClassName();

};


class OdDgBSplineCurve3d_Dumper : public OdRxObject_Dumper
{

public:

  virtual void      dump( OdRxObjectPtr object );
  virtual OdString  getClassName();

};


class OdDgBSplineCurve2d_Dumper : public OdRxObject_Dumper
{

public:

  virtual void      dump( OdRxObjectPtr object );
  virtual OdString  getClassName();

};


class OdDgSurface_Dumper : public OdRxObject_Dumper
{

public:

  virtual void      dump( OdRxObjectPtr object );
  virtual OdString  getClassName();

};


class OdDgSolid_Dumper : public OdRxObject_Dumper
{

public:

  virtual void      dump( OdRxObjectPtr object );
  virtual OdString  getClassName();

};


class OdDgRasterAttachmentHeader_Dumper : public OdRxObject_Dumper
{

public:

  virtual void      dump( OdRxObjectPtr object );
  virtual OdString  getClassName();

};


class OdDgRasterHeader2d_Dumper : public OdRxObject_Dumper
{

public:

  virtual void      dump( OdRxObjectPtr object );
  virtual OdString  getClassName();

};


class OdDgRasterHeader3d_Dumper : public OdRxObject_Dumper
{

public:

  virtual void      dump( OdRxObjectPtr object );
  virtual OdString  getClassName();

};


class OdDgRasterComponent_Dumper : public OdRxObject_Dumper
{

public:

  virtual void      dump( OdRxObjectPtr object );
  virtual OdString  getClassName();

};


class OdDgTagElement_Dumper : public OdRxObject_Dumper
{

public:

  virtual void      dump( OdRxObjectPtr object );
  virtual OdString  getClassName();

};


class OdDgCellHeader2d_Dumper : public OdRxObject_Dumper
{

public:

  virtual void      dump( OdRxObjectPtr object );
  virtual OdString  getClassName();

};


class OdDgCellHeader3d_Dumper : public OdRxObject_Dumper
{

public:

  virtual void      dump( OdRxObjectPtr object );
  virtual OdString  getClassName();

};


class OdDgBSplineSurface_Dumper : public OdRxObject_Dumper
{

public:

  virtual void      dump( OdRxObjectPtr object );
  virtual OdString  getClassName();

};


class OdDgLevelTable_Dumper : public OdRxObject_Dumper
{

public:

  virtual void      dump( OdRxObjectPtr object );
  virtual OdString  getClassName();

};

class OdDgLevelFilterTable_Dumper : public OdRxObject_Dumper
{

public:

  virtual void      dump( OdRxObjectPtr object );
  virtual OdString  getClassName();

};


class OdDgLevelTableRecord_Dumper : public OdRxObject_Dumper
{

public:

  virtual void      dump( OdRxObjectPtr object );
  virtual OdString  getClassName();

};

class OdDgLevelFilterTableRecord_Dumper : public OdRxObject_Dumper
{

public:

  virtual void      dump( OdRxObjectPtr object );
  virtual OdString  getClassName();

};



class OdDgFontTable_Dumper : public OdRxObject_Dumper
{

public:

  virtual void      dump( OdRxObjectPtr object );
  virtual OdString  getClassName();

};


class OdDgFontTableRecord_Dumper : public OdRxObject_Dumper
{

public:

  virtual void      dump( OdRxObjectPtr object );
  virtual OdString  getClassName();

};


class OdDgTextStyleTable_Dumper : public OdRxObject_Dumper
{

public:

  virtual void      dump( OdRxObjectPtr object );
  virtual OdString  getClassName();

};


class OdDgTextStyleTableRecord_Dumper : public OdRxObject_Dumper
{

public:

  virtual void      dump( OdRxObjectPtr object );
  virtual OdString  getClassName();

};


class OdDgDimStyleTable_Dumper : public OdRxObject_Dumper
{

public:

  virtual void      dump( OdRxObjectPtr object );
  virtual OdString  getClassName();

};


class OdDgDimStyleTableRecord_Dumper : public OdRxObject_Dumper
{

public:

  virtual void      dump( OdRxObjectPtr object );
  virtual OdString  getClassName();

};


class OdDgMultilineStyleTable_Dumper : public OdRxObject_Dumper
{

public:

  virtual void      dump( OdRxObjectPtr object );
  virtual OdString  getClassName();

};


class OdDgMultilineStyleTableRecord_Dumper : public OdRxObject_Dumper
{

public:

  virtual void      dump( OdRxObjectPtr object );
  virtual OdString  getClassName();

};


class OdDgLineStyleTable_Dumper : public OdRxObject_Dumper
{

public:

  virtual void      dump( OdRxObjectPtr object );
  virtual OdString  getClassName();

};


class OdDgLineStyleTableRecord_Dumper : public OdRxObject_Dumper
{

public:

  virtual void      dump( OdRxObjectPtr object );
  virtual OdString  getClassName();

};


class OdDgRegAppTable_Dumper : public OdRxObject_Dumper
{

public:

  virtual void      dump( OdRxObjectPtr object );
  virtual OdString  getClassName();

};


class OdDgRegAppTableRecord_Dumper : public OdRxObject_Dumper
{

public:

  virtual void      dump( OdRxObjectPtr object );
  virtual OdString  getClassName();

};


class OdDgViewGroupTable_Dumper : public OdRxObject_Dumper
{

public:

  virtual void      dump( OdRxObjectPtr object );
  virtual OdString  getClassName();

};


class OdDgViewGroup_Dumper : public OdRxObject_Dumper
{

public:

  virtual void      dump( OdRxObjectPtr object );
  virtual OdString  getClassName();

};


class OdDgView_Dumper : public OdRxObject_Dumper
{

public:

  virtual void      dump( OdRxObjectPtr object );
  virtual OdString  getClassName();

};


class OdDgNamedViewTable_Dumper : public OdRxObject_Dumper
{

public:

  virtual void      dump( OdRxObjectPtr object );
  virtual OdString  getClassName();

};


class OdDgSharedCellDefinitionTable_Dumper : public OdRxObject_Dumper
{

public:

  virtual void      dump( OdRxObjectPtr object );
  virtual OdString  getClassName();

};


class OdDgSharedCellDefinition_Dumper : public OdRxObject_Dumper
{

public:

  virtual void      dump( OdRxObjectPtr object );
  virtual OdString  getClassName();

};


class OdDgSharedCellReference_Dumper : public OdRxObject_Dumper
{

public:

  virtual void      dump( OdRxObjectPtr object );
  virtual OdString  getClassName();

};


class OdDgTagDefinitionSetTable_Dumper : public OdRxObject_Dumper
{

public:

  virtual void      dump( OdRxObjectPtr object );
  virtual OdString  getClassName();

};


class OdDgTagDefinitionSet_Dumper : public OdRxObject_Dumper
{

public:

  virtual void      dump( OdRxObjectPtr object );
  virtual OdString  getClassName();

};


class OdDgTagDefinition_Dumper : public OdRxObject_Dumper
{

public:

  virtual void      dump( OdRxObjectPtr object );
  virtual OdString  getClassName();

};


class OdDgColorTable_Dumper : public OdRxObject_Dumper
{

public:

  virtual void      dump( OdRxObjectPtr object );
  virtual OdString  getClassName();

};


class OdDgReferenceHeader_Dumper : public OdRxObject_Dumper
{

public:

  virtual void      dump( OdRxObjectPtr object );
  virtual OdString  getClassName();

};

class OdDgMeshFaceLoops_Dumper : public OdRxObject_Dumper
{

public:

  virtual void      dump( OdRxObjectPtr object );
  virtual OdString  getClassName();

};

class OdDgMeshPointCloud_Dumper : public OdRxObject_Dumper
{

public:

  virtual void      dump( OdRxObjectPtr object );
  virtual OdString  getClassName();

};

class OdDgMeshTriangleList_Dumper : public OdRxObject_Dumper
{

public:

  virtual void      dump( OdRxObjectPtr object );
  virtual OdString  getClassName();

};

class OdDgMeshQuadList_Dumper : public OdRxObject_Dumper
{

public:

  virtual void      dump( OdRxObjectPtr object );
  virtual OdString  getClassName();

};

class OdDgMeshTriangleGrid_Dumper : public OdRxObject_Dumper
{

public:

  virtual void      dump( OdRxObjectPtr object );
  virtual OdString  getClassName();

};

class OdDgMeshQuadGrid_Dumper : public OdRxObject_Dumper
{

public:

  virtual void      dump( OdRxObjectPtr object );
  virtual OdString  getClassName();

};

class OdDgProxyElement_Dumper : public OdRxObject_Dumper
{

public:

  virtual void      dump( OdRxObjectPtr object );
  virtual OdString  getClassName();

};

class OdDgApplicationData_Dumper : public OdRxObject_Dumper
{

public:

  virtual void      dump( OdRxObjectPtr object );
  virtual OdString  getClassName();

};

/************************************************************************/
/* This manages all the dumpers                                         */
/************************************************************************/


class OdDgDumper
{

public:

  OdDgDumper();
  virtual ~OdDgDumper();

  void rootDump( OdRxObjectPtr database ) const;  //root dumper

private:

  OdDgDatabase_Dumper                                 m_databaseDumper;
  OdDgModel_Dumper                                    m_modelDumper;
  OdDgLine2d_Dumper                                   m_line2dDumper;
  OdDgLine3d_Dumper                                   m_line3dDumper;
  OdDgLineString2d_Dumper                             m_lineString2dDumper;
  OdDgLineString3d_Dumper                             m_lineString3dDumper;
  OdDgText2d_Dumper                                   m_text2dDumper;
  OdDgText3d_Dumper                                   m_text3dDumper;
  OdDgTextNode2d_Dumper                               m_textNode2dDumper;
  OdDgTextNode3d_Dumper                               m_textNode3dDumper;
  OdDgShape2d_Dumper                                  m_shape2dDumper;
  OdDgShape3d_Dumper                                  m_shape3dDumper;
  OdDgCurve2d_Dumper                                  m_curve2dDumper;
  OdDgCurve3d_Dumper                                  m_curve3dDumper;
  OdDgEllipse2d_Dumper                                m_ellipse2dDumper;
  OdDgEllipse3d_Dumper                                m_ellipse3dDumper;
  OdDgArc2d_Dumper                                    m_arc2dDumper;
  OdDgArc3d_Dumper                                    m_arc3dDumper;
  OdDgCone_Dumper                                     m_coneDumper;
  OdDgComplexString_Dumper                            m_complexStringDumper;
  OdDgComplexShape_Dumper                             m_complexShapeDumper;
  OdDgPointString2d_Dumper                            m_pointString2dDumper;
  OdDgPointString3d_Dumper                            m_pointString3dDumper;
  OdDgDimension_Dumper                                m_dimensionDumper;
  OdDgMultiline_Dumper                                m_multilineDumper;
  OdDgBSplineCurve2d_Dumper                           m_bSplineCurve2dDumper;
  OdDgBSplineCurve3d_Dumper                           m_bSplineCurve3dDumper;
  OdDgSurface_Dumper                                  m_surfaceDumper;
  OdDgSolid_Dumper                                    m_solidDumper;
  OdDgRasterAttachmentHeader_Dumper                   m_rasterAttachmentHeaderDumper;
  OdDgRasterHeader2d_Dumper                           m_rasterHeader2dDumper;
  OdDgRasterHeader3d_Dumper                           m_rasterHeader3dDumper;
  OdDgRasterComponent_Dumper                          m_rasterComponentDumper;
  OdDgTagElement_Dumper                               m_tagElementDumper;
  OdDgCellHeader2d_Dumper                             m_cellHeader2dDumper;
  OdDgCellHeader3d_Dumper                             m_cellHeader3dDumper;
  OdDgBSplineSurface_Dumper                           m_bSplineSurfaceDumper;
  OdDgLevelTable_Dumper                               m_levelTableDumper;
  OdDgLevelTableRecord_Dumper                         m_levelTableRecordDumper;
  OdDgLevelFilterTable_Dumper                         m_levelFilterTableDumper;
  OdDgLevelFilterTableRecord_Dumper                   m_levelFilterTableRecordDumper;
  OdDgFontTable_Dumper                                m_fontTableDumper;
  OdDgFontTableRecord_Dumper                          m_fontTableRecordDumper;
  OdDgTextStyleTable_Dumper                           m_textStyleTableDumper;
  OdDgTextStyleTableRecord_Dumper                     m_textStyleTableRecordDumper;
  OdDgDimStyleTable_Dumper                            m_dimStyleTableDumper;
  OdDgDimStyleTableRecord_Dumper                      m_dimStyleTableRecordDumper;
  OdDgMultilineStyleTable_Dumper                      m_multilineStyleTableDumper;
  OdDgMultilineStyleTableRecord_Dumper                m_multilineStyleTableRecordDumper;
  OdDgLineStyleTable_Dumper                           m_lineStyleTableDumper;
  OdDgLineStyleTableRecord_Dumper                     m_lineStyleTableRecordDumper;
  OdDgRegAppTable_Dumper                              m_regAppTableDumper;
  OdDgRegAppTableRecord_Dumper                        m_regAppTableRecordDumper;
  OdDgViewGroupTable_Dumper                           m_viewGroupTableDumper;
  OdDgViewGroup_Dumper                                m_viewGroupDumper;
  OdDgView_Dumper                                     m_viewDumper;
  OdDgNamedViewTable_Dumper                           m_namedViewTableDumper;
  OdDgSharedCellDefinitionTable_Dumper                m_sharedCellDefinitionTableDumper;
  OdDgSharedCellDefinition_Dumper                     m_sharedCellDefinitionDumper;
  OdDgSharedCellReference_Dumper                      m_sharedCellReferenceDumper;
  OdDgTagDefinitionSetTable_Dumper                    m_tagSetDefinitionTableDumper;
  OdDgTagDefinitionSet_Dumper                         m_tagSetDefinitionDumper;
  OdDgTagDefinition_Dumper                            m_tagDefinitionDumper;
  OdDgColorTable_Dumper                               m_colorTableDumper;
  OdDgReferenceHeader_Dumper                          m_referenceAttachmentDumper;
  OdDgMeshFaceLoops_Dumper                            m_meshFaceLoopsDumper;
  OdDgMeshPointCloud_Dumper                           m_meshPointCloudDumper;
  OdDgMeshTriangleList_Dumper                         m_meshTriangleListDumper;
  OdDgMeshQuadList_Dumper                             m_meshQuadListDumper;
  OdDgMeshTriangleGrid_Dumper                         m_meshTriangleGridDumper;
  OdDgMeshQuadGrid_Dumper                             m_meshQuadGridDumper;
  OdDgProxyElement_Dumper                             m_proxyDumper;
  OdDgApplicationData_Dumper                          m_applicationDataDumper;

};
