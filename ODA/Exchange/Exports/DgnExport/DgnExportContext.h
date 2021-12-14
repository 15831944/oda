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

#ifndef _DGN_EXPORTCONTEXT_INCLUDED_ 
#define _DGN_EXPORTCONTEXT_INCLUDED_

#define STL_USING_MAP
#include <OdaSTL.h>
#include <DgDatabase.h>
#include <DbDimAssoc.h>

/** \details
  <group OdExport_Classes> 
*/
namespace TD_DGN_EXPORT 
{

class DgnExporter;

struct OdDgnExportHatchLoopAssociation
{
  OdDgElementId m_idLoopElement;
  OdDbObjectIdArray m_arrAssociationIds;
};

struct OdDgnExportDimAssoc
{
  OdUInt32             m_uPointIndex;
  OdDbOsnapPointRefPtr m_pPoint;
};

struct OdDgnExportDimAssocData
{
  OdDgElementId                 m_idDgDimension;
  OdDbObjectId                  m_idDbDimension;
  OdArray<OdDgnExportDimAssoc>  m_arrAccocPts;
  bool                          m_bDiamAssoc;
  bool                          m_b2LinesAssoc;
};

struct OdDgnExportHatchAssociation
{
  OdDgElementId m_idDgHatch;
  OdArray<OdDgnExportHatchLoopAssociation> m_arrLoopAssoc;
};

class OdDgnExportContext
{
public:
  static OdDgElementId  getElementId( const OdDbObjectId& idDwgObject );
  static void           addDgnElement(const OdDbObjectId& idDwgObject, const OdDgElementId& idElement);

  static void           restoreAfterExport();
  static void           prepareToExport( DgnExporter* pDgnExporter );
  static DgnExporter*   getDgnExporter();
  static bool           getInternalExportFlag();
  static void           setInternalExportFlag( bool bSet );

  static bool           isKeepNonDbro();
  static void           setKeepNonDbro(bool bSet);

  static bool           is3d();
  static void           set3d(bool bSet);
  static double         getMasterToUORsScale();
  static void           setMasterToUORsScale(double dScale);
  static bool           getAllowCloseCurvesFlag();
  static void           setAllowCloseCurvesFlag( bool bSet );

  static OdUInt32       getColorByBlockIndex();
  static void           setColorByBlockIndex(OdUInt32 uIndex);
  static OdUInt32       getLineStyleByBlockIndex();
  static void           setLineStyleByBlockIndex(OdUInt32 uIndex);
  static OdUInt32       getLineWeightByBlockIndex();
  static void           setLineWeightByBlockIndex(OdUInt32 uIndex);

  static void           addHatchAssociation(const OdDgnExportHatchAssociation& hatchAssoc);
  static void           clearHatchAssociations();
  static OdUInt32       getHatchAssociationsCount();
  static OdDgnExportHatchAssociation getHatchAssociation(OdUInt32 uIndex);

  static void           addProcessingBlockId(const OdDbObjectId& idBlock);
  static void           removeProcessingBlockId(const OdDbObjectId& idBlock);
  static bool           isBlockOnProcessing(const OdDbObjectId& idBlock);
  static void           removeAllProcessingBlockIds();

  static OdDgElementId  getAttrDefSetForBlock(const OdDbObjectId& idBlock);
  static void           bindAttrDefSetToBlock(const OdDgElementId& idAttrDefSet, const OdDbObjectId& idBlock);
  static void           clearAttrDefSetRecords();

  static bool           skipObjectExport(const OdDbObjectId& idObject);
  static void           addObjectToSkipExport(const OdDbObjectId& idObject);
  static void           clearAllSkippedObjects();

  static double         getLineStyleScaleCorrectionFactor();
  static void           setLineStyleScaleCorrectionFactor( double dFactor );

  static OdDgElementId  getDgnModelSpaceModelId();
  static void           setDgnModelSpaceModelId(const OdDgElementId& idModel);

  static OdDbObjectId   getViewportClipObject(const OdDbObjectId& idViewport);
  static void           addViewportClipObject(const OdDbObjectId& idViewport, const OdDbObjectId& idClipObject);
  static bool           isViewportClipObject(const OdDbObjectId& idClipObject);
  static void           clearViewportClipObjects();

  static bool           isBlockWithXRefInsert(const OdDbObjectId& idBlock);
  static void           addBlockWithXRefInsert(const OdDbObjectId& idBlock);
  static void           clearBlocksWithXRefInsert();

  static OdDgElementId  getPointBlockId();
  static void           setPointBlockId( OdDgElementId& idDgBlock );

  static ODCOLORREF     getBackgroundColor();
  static void           setBackgroundColor(ODCOLORREF bgColor);
  static bool           isDarkPalette();

  static bool           getRecomputeDimensions();
  static void           setRecomputeDimensions(bool bSet);

  static OdUInt32                 getDimensionAssocCount();
  static OdDgnExportDimAssocData  getDimensionAssoc(OdUInt32 uIndex);
  static void                     addDimensionAssoc(const OdDgnExportDimAssocData& assocData);
  static void                     clearAllDimensionAssoc();

};

}
#endif // _DGN_EXPORTCONTEXT_INCLUDED_
