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

#include <OdaCommon.h>
#include "DgnExportImpl.h"
#include "DgnExportContext.h"
#include "DgnExportCommon.h"
#include "DbSymUtl.h"

namespace TD_DGN_EXPORT {

class OdDgnExportContextData
{
public:
	OdDgnExportContextData() :m_bInternalExport(false), m_b3dFlag(false), m_bCloseNurbCurves(false), 
                            m_dMasterToUORsScale(1.0), m_bKeepNonDbro(false), m_uColorByBlockIndex(OdDg::kColorByCell),
                            m_uLineStyleByBlockIndex(OdDg::kLineStyleByCell), m_uLineWeightByBlockIndex(OdDg::kLineWeightByCell),
                            m_dLineStyleScaleCorrectionFactor(1.0), m_bgColor(0), m_bRecomputeDims(false)
  {

  };

public:
  DgnExporter*                                         m_pDgnExporter;
  bool                                                 m_bInternalExport;
  bool                                                 m_b3dFlag;
  bool                                                 m_bKeepNonDbro;
  bool                                                 m_bCloseNurbCurves;
  double                                               m_dMasterToUORsScale;
  double                                               m_dLineStyleScaleCorrectionFactor;
  OdUInt32                                             m_uColorByBlockIndex;
  OdUInt32                                             m_uLineStyleByBlockIndex;
  OdUInt32                                             m_uLineWeightByBlockIndex;
  OdArray<OdDgnExportHatchAssociation>                 m_arrHatchAssoc;
  std::set<OdDbObjectId>                               m_setProcessedBlockIds;
  std::set<OdDbObjectId>                               m_setSkippedObjectIds;
  std::map<OdDbObjectId, OdDgElementId>                m_mapAttrDefSet;
  std::map<OdDbObjectId, OdDbObjectId>                 m_mapViewportClip;
  OdDgElementId                                        m_idModelSpaceModel;
  std::set<OdDbObjectId>                               m_setBlocksWithXRefInserts;
  OdGePoint3d                                          m_ptBlockRefOffset;
  OdDgElementId                                        m_idPointBlock;
  ODCOLORREF                                           m_bgColor;
  bool                                                 m_bRecomputeDims;
  OdArray<OdDgnExportDimAssocData>                     m_arrDimAssoc;
};

OdArray<OdDgnExportContextData>   g_exportContextDataArr;

static OdDgnExportContextData* getCurrentContext()
{
  if( g_exportContextDataArr.size() > 0 )
  {
    return g_exportContextDataArr.asArrayPtr() + g_exportContextDataArr.size() - 1;
  }
  else
  {
    return NULL;
  }
}

//---------------------------------------------------------------------------

void OdDgnExportContext::restoreAfterExport()
{
  if( g_exportContextDataArr.size() > 0 )
  {
    g_exportContextDataArr.removeLast();
  }
}

//---------------------------------------------------------------------------

void         OdDgnExportContext::prepareToExport( DgnExporter* pDgnExporter )
{
  OdDgnExportContextData* pContextData = getCurrentContext();

  OdDgnExportContextData newData;
  newData.m_pDgnExporter = pDgnExporter;

  if (pContextData)
  {
    newData.m_b3dFlag          = pContextData->m_b3dFlag;
    newData.m_bCloseNurbCurves = pContextData->m_bCloseNurbCurves;
    newData.m_bgColor = pContextData->m_bgColor;
    newData.m_bRecomputeDims = pContextData->m_bRecomputeDims;
  }

  g_exportContextDataArr.push_back( newData );
}

//---------------------------------------------------------------------------

DgnExporter* OdDgnExportContext::getDgnExporter()
{
  DgnExporter*        pExporter = NULL;
  OdDgnExportContextData* pContextData = getCurrentContext();

  if( pContextData )
  {
    pExporter = pContextData->m_pDgnExporter;
  }

  return pExporter;
}

//---------------------------------------------------------------------------

bool OdDgnExportContext::getInternalExportFlag()
{
  bool bRet = false;

  OdDgnExportContextData* pContextData = getCurrentContext();

  if( pContextData )
  {
    bRet = pContextData->m_bInternalExport;
  }

  return bRet;
}

//---------------------------------------------------------------------------

void OdDgnExportContext::setInternalExportFlag( bool bSet )
{
  OdDgnExportContextData* pContextData = getCurrentContext();

  if( pContextData )
  {
    pContextData->m_bInternalExport = bSet;
  }
}

//---------------------------------------------------------------------------

OdDgElementId OdDgnExportContext::getElementId( const OdDbObjectId& idDwgObject )
{
  OdDgElementId retVal;

  OdDgnExportContextData* pContextData = getCurrentContext();

  if( pContextData && pContextData->m_pDgnExporter )
  {
    retVal = pContextData->m_pDgnExporter->getDgnElementId( idDwgObject );
  }

  return retVal;
}

//----------------------------------------------------------------------------

void  OdDgnExportContext::addDgnElement(const OdDbObjectId& idDwgObject, const OdDgElementId& idElement)
{
  OdDgnExportContextData* pContextData = getCurrentContext();

  if (pContextData && pContextData->m_pDgnExporter && !pContextData->m_bKeepNonDbro )
    pContextData->m_pDgnExporter->addIdPair(idDwgObject, idElement);
}

//----------------------------------------------------------------------------

bool         OdDgnExportContext::is3d()
{
  bool bRet = true;

  OdDgnExportContextData* pContextData = getCurrentContext();

  if (pContextData)
    bRet = pContextData->m_b3dFlag;

  return bRet;
}

//----------------------------------------------------------------------------

void         OdDgnExportContext::set3d(bool bSet)
{
  OdDgnExportContextData* pContextData = getCurrentContext();

  if( pContextData )
    pContextData->m_b3dFlag = bSet;
}

//----------------------------------------------------------------------------

bool           OdDgnExportContext::isKeepNonDbro()
{
  bool bRet = false;

  OdDgnExportContextData* pContextData = getCurrentContext();

  if (pContextData)
    bRet = pContextData->m_bKeepNonDbro;

  return bRet;
}

//----------------------------------------------------------------------------

void           OdDgnExportContext::setKeepNonDbro(bool bSet)
{
  OdDgnExportContextData* pContextData = getCurrentContext();

  if (pContextData)
    pContextData->m_bKeepNonDbro = bSet;
}

//----------------------------------------------------------------------------

bool           OdDgnExportContext::getAllowCloseCurvesFlag()
{
  bool bRet = false;

  OdDgnExportContextData* pContextData = getCurrentContext();

  if (pContextData)
    bRet = pContextData->m_bCloseNurbCurves;

  return bRet;
}

//----------------------------------------------------------------------------

void           OdDgnExportContext::setAllowCloseCurvesFlag(bool bSet)
{
  OdDgnExportContextData* pContextData = getCurrentContext();

  if (pContextData)
    pContextData->m_bCloseNurbCurves = bSet;
}

//----------------------------------------------------------------------------

double       OdDgnExportContext::getMasterToUORsScale()
{
  double dRet = 1.0;

  OdDgnExportContextData* pContextData = getCurrentContext();

  if (pContextData)
    dRet = pContextData->m_dMasterToUORsScale;

  return dRet;
}

//----------------------------------------------------------------------------

void         OdDgnExportContext::setMasterToUORsScale(double dScale)
{
  OdDgnExportContextData* pContextData = getCurrentContext();

  if (pContextData)
    pContextData->m_dMasterToUORsScale = dScale;
}

//----------------------------------------------------------------------------

OdUInt32       OdDgnExportContext::getColorByBlockIndex()
{
  OdUInt32 uRet = OdDg::kColorByCell;

  OdDgnExportContextData* pContextData = getCurrentContext();

  if (pContextData)
    uRet = pContextData->m_uColorByBlockIndex;

  return uRet;
}

//----------------------------------------------------------------------------

void           OdDgnExportContext::setColorByBlockIndex(OdUInt32 uIndex)
{
  OdDgnExportContextData* pContextData = getCurrentContext();

  if(pContextData)
    pContextData->m_uColorByBlockIndex = uIndex;
}

//----------------------------------------------------------------------------

OdUInt32       OdDgnExportContext::getLineStyleByBlockIndex()
{
  OdUInt32 uRet = OdDg::kLineStyleByCell;

  OdDgnExportContextData* pContextData = getCurrentContext();

  if (pContextData)
    uRet = pContextData->m_uLineStyleByBlockIndex;

  return uRet;
}

//----------------------------------------------------------------------------

void           OdDgnExportContext::setLineStyleByBlockIndex(OdUInt32 uIndex)
{
  OdDgnExportContextData* pContextData = getCurrentContext();

  if (pContextData)
    pContextData->m_uLineStyleByBlockIndex = uIndex;
}

//----------------------------------------------------------------------------

OdUInt32       OdDgnExportContext::getLineWeightByBlockIndex()
{
  OdUInt32 uRet = OdDg::kLineWeightByCell;

  OdDgnExportContextData* pContextData = getCurrentContext();

  if (pContextData)
    uRet = pContextData->m_uLineWeightByBlockIndex;

  return uRet;
}

//----------------------------------------------------------------------------

void           OdDgnExportContext::setLineWeightByBlockIndex(OdUInt32 uIndex)
{
  OdDgnExportContextData* pContextData = getCurrentContext();

  if (pContextData)
    pContextData->m_uLineWeightByBlockIndex = uIndex;
}

//----------------------------------------------------------------------------

void         OdDgnExportContext::addHatchAssociation(const OdDgnExportHatchAssociation& hatchAssoc)
{
  OdDgnExportContextData* pContextData = getCurrentContext();

  if( pContextData )
    pContextData->m_arrHatchAssoc.push_back(hatchAssoc);
}

//----------------------------------------------------------------------------

void         OdDgnExportContext::clearHatchAssociations()
{
  OdDgnExportContextData* pContextData = getCurrentContext();

  if( pContextData )
    pContextData->m_arrHatchAssoc.clear();
}

//----------------------------------------------------------------------------

OdUInt32         OdDgnExportContext::getHatchAssociationsCount()
{
  OdUInt32 uRet = 0;

  OdDgnExportContextData* pContextData = getCurrentContext();

  if( pContextData )
    uRet = pContextData->m_arrHatchAssoc.size();

  return uRet;
}

//----------------------------------------------------------------------------

OdDgnExportHatchAssociation         OdDgnExportContext::getHatchAssociation(OdUInt32 uIndex)
{
  OdDgnExportHatchAssociation retVal;

  OdDgnExportContextData* pContextData = getCurrentContext();

  if( pContextData && (uIndex < pContextData->m_arrHatchAssoc.size()) )
    retVal = pContextData->m_arrHatchAssoc[uIndex];

  return  retVal;
}

void OdDgnExportContext::addProcessingBlockId(const OdDbObjectId& idBlock)
{
  OdDgnExportContextData* pContextData = getCurrentContext();

  if (pContextData)
    pContextData->m_setProcessedBlockIds.insert(idBlock);
}

void OdDgnExportContext::removeProcessingBlockId(const OdDbObjectId& idBlock)
{
  OdDgnExportContextData* pContextData = getCurrentContext();

  if (pContextData)
    pContextData->m_setProcessedBlockIds.erase(idBlock);
}

//----------------------------------------------------------------------------

bool OdDgnExportContext::isBlockOnProcessing(const OdDbObjectId& idBlock)
{
  bool bRet = false;

  OdDgnExportContextData* pContextData = getCurrentContext();

  if( pContextData )
  {
    std::set<OdDbObjectId>::iterator pIter = pContextData->m_setProcessedBlockIds.find(idBlock);

    if( pIter != pContextData->m_setProcessedBlockIds.end() )
      bRet = true;
  }

  return bRet;
}

//----------------------------------------------------------------------------

void OdDgnExportContext::removeAllProcessingBlockIds()
{
  OdDgnExportContextData* pContextData = getCurrentContext();

  if (pContextData)
    pContextData->m_setProcessedBlockIds.clear();
}

//----------------------------------------------------------------------------

OdDgElementId  OdDgnExportContext::getAttrDefSetForBlock(const OdDbObjectId& idBlock)
{
  OdDgElementId idRet;

  OdDgnExportContextData* pContextData = getCurrentContext();

  if (pContextData)
  {
    std::map<OdDbObjectId, OdDgElementId>::iterator pIter = pContextData->m_mapAttrDefSet.find(idBlock);

    if (pIter != pContextData->m_mapAttrDefSet.end())
      idRet = pIter->second;
  }

  return idRet;
}

//----------------------------------------------------------------------------

void           OdDgnExportContext::bindAttrDefSetToBlock(const OdDgElementId& idAttrDefSet, const OdDbObjectId& idBlock)
{
  OdDgnExportContextData* pContextData = getCurrentContext();

  if (pContextData)
  {
    std::map<OdDbObjectId, OdDgElementId>::iterator pIter = pContextData->m_mapAttrDefSet.find(idBlock);

    if (pIter == pContextData->m_mapAttrDefSet.end())
      pContextData->m_mapAttrDefSet[idBlock] = idAttrDefSet;
  }
}

//----------------------------------------------------------------------------

void           OdDgnExportContext::clearAttrDefSetRecords()
{
  OdDgnExportContextData* pContextData = getCurrentContext();

  if (pContextData)
    pContextData->m_mapAttrDefSet.clear();
}

//----------------------------------------------------------------------------

bool           OdDgnExportContext::skipObjectExport(const OdDbObjectId& idObject)
{
  bool bRet = false;

  OdDgnExportContextData* pContextData = getCurrentContext();

  if( pContextData )
  {
    std::set<OdDbObjectId>::iterator pIter = pContextData->m_setSkippedObjectIds.find(idObject);

    if (pIter != pContextData->m_setSkippedObjectIds.end())
      bRet = true;
  }

  return bRet;
}

//----------------------------------------------------------------------------

void           OdDgnExportContext::addObjectToSkipExport(const OdDbObjectId& idObject)
{
  OdDgnExportContextData* pContextData = getCurrentContext();

  if (pContextData)
    pContextData->m_setSkippedObjectIds.insert(idObject);
}

//----------------------------------------------------------------------------

void           OdDgnExportContext::clearAllSkippedObjects()
{
  OdDgnExportContextData* pContextData = getCurrentContext();

  if (pContextData)
    pContextData->m_setSkippedObjectIds.clear();
}

//----------------------------------------------------------------------------

double         OdDgnExportContext::getLineStyleScaleCorrectionFactor()
{
  OdDgnExportContextData* pContextData = getCurrentContext();

  if (pContextData)
    return pContextData->m_dLineStyleScaleCorrectionFactor;

  return 1.0;
}

//----------------------------------------------------------------------------

void           OdDgnExportContext::setLineStyleScaleCorrectionFactor(double dFactor)
{
  OdDgnExportContextData* pContextData = getCurrentContext();

  if (pContextData)
    pContextData->m_dLineStyleScaleCorrectionFactor = dFactor;
}

//----------------------------------------------------------------------------

OdDgElementId  OdDgnExportContext::getDgnModelSpaceModelId()
{
  OdDgElementId idRet;

  OdDgnExportContextData* pContextData = getCurrentContext();

  if (pContextData)
    idRet = pContextData->m_idModelSpaceModel;

  return idRet;
}

//----------------------------------------------------------------------------

void           OdDgnExportContext::setDgnModelSpaceModelId(const OdDgElementId& idModel)
{
  OdDgnExportContextData* pContextData = getCurrentContext();

  if (pContextData)
    pContextData->m_idModelSpaceModel = idModel;
}

//----------------------------------------------------------------------------

OdDbObjectId   OdDgnExportContext::getViewportClipObject(const OdDbObjectId& idViewport)
{
  OdDbObjectId idRet;

  OdDgnExportContextData* pContextData = getCurrentContext();

  if( pContextData )
  {
    std::map<OdDbObjectId, OdDbObjectId>::iterator pIter = pContextData->m_mapViewportClip.begin();

    for(; pIter != pContextData->m_mapViewportClip.end(); pIter++)
    {
      if (pIter->second == idViewport)
      {
        idRet = pIter->first;
        break;
      }
    }
  }

  return idRet;
}

//----------------------------------------------------------------------------

void           OdDgnExportContext::addViewportClipObject(const OdDbObjectId& idViewport, const OdDbObjectId& idClipObject)
{
  OdDgnExportContextData* pContextData = getCurrentContext();

  if( pContextData )
    pContextData->m_mapViewportClip[idClipObject] = idViewport;
}

//----------------------------------------------------------------------------

bool           OdDgnExportContext::isViewportClipObject(const OdDbObjectId& idClipObject)
{
  bool bRet = false;

  OdDgnExportContextData* pContextData = getCurrentContext();

  if( pContextData )
  {
    std::map<OdDbObjectId, OdDbObjectId>::iterator pIter = pContextData->m_mapViewportClip.find(idClipObject);
    bRet = pIter != pContextData->m_mapViewportClip.end();
  }

  return bRet;
}

//----------------------------------------------------------------------------

void           OdDgnExportContext::clearViewportClipObjects()
{
  OdDgnExportContextData* pContextData = getCurrentContext();

  if (pContextData)
    pContextData->m_mapViewportClip.clear();
}

//----------------------------------------------------------------------------

bool           OdDgnExportContext::isBlockWithXRefInsert(const OdDbObjectId& idBlock)
{
  bool bRet = false;

  OdDgnExportContextData* pContextData = getCurrentContext();

  if (pContextData)
  {
    std::set<OdDbObjectId>::iterator pIter = pContextData->m_setBlocksWithXRefInserts.find(idBlock);

    bRet = pIter != pContextData->m_setBlocksWithXRefInserts.end();
  }

  return bRet;
}

//----------------------------------------------------------------------------

void           OdDgnExportContext::addBlockWithXRefInsert(const OdDbObjectId& idBlock)
{
  OdDgnExportContextData* pContextData = getCurrentContext();

  if (pContextData)
    pContextData->m_setBlocksWithXRefInserts.insert(idBlock);
}

//----------------------------------------------------------------------------

void           OdDgnExportContext::clearBlocksWithXRefInsert()
{
  OdDgnExportContextData* pContextData = getCurrentContext();

  if (pContextData)
    pContextData->m_setBlocksWithXRefInserts.clear();
}

//----------------------------------------------------------------------------

OdDgElementId  OdDgnExportContext::getPointBlockId()
{
  OdDgnExportContextData* pContextData = getCurrentContext();

  if (pContextData)
    return pContextData->m_idPointBlock;

  return OdDgElementId();
}

//----------------------------------------------------------------------------

void           OdDgnExportContext::setPointBlockId(OdDgElementId& idDgBlock)
{
  OdDgnExportContextData* pContextData = getCurrentContext();

  if (pContextData)
    pContextData->m_idPointBlock = idDgBlock;
}

//----------------------------------------------------------------------------

ODCOLORREF     OdDgnExportContext::getBackgroundColor()
{
  OdDgnExportContextData* pContextData = getCurrentContext();

  if (pContextData)
    return pContextData->m_bgColor;

  return 0;
}

//----------------------------------------------------------------------------

void           OdDgnExportContext::setBackgroundColor(ODCOLORREF bgColor)
{
  OdDgnExportContextData* pContextData = getCurrentContext();

  if (pContextData)
    pContextData->m_bgColor = bgColor;
}

//----------------------------------------------------------------------------

bool           OdDgnExportContext::getRecomputeDimensions()
{
  OdDgnExportContextData* pContextData = getCurrentContext();

  if (pContextData)
    return pContextData->m_bRecomputeDims;

  return false;
}

//----------------------------------------------------------------------------

bool           OdDgnExportContext::isDarkPalette()
{
  OdDgnExportContextData* pContextData = getCurrentContext();

  if( pContextData )
    return (ODGETRED(pContextData->m_bgColor) < 140) && (ODGETGREEN(pContextData->m_bgColor) < 140) && (ODGETBLUE(pContextData->m_bgColor) < 140);

  return true;
}
//----------------------------------------------------------------------------

void           OdDgnExportContext::setRecomputeDimensions(bool bSet)
{
  OdDgnExportContextData* pContextData = getCurrentContext();

  if (pContextData)
    pContextData->m_bRecomputeDims = bSet;
}

//----------------------------------------------------------------------------

OdUInt32                 OdDgnExportContext::getDimensionAssocCount()
{
  OdUInt32 uRet = 0;

  OdDgnExportContextData* pContextData = getCurrentContext();

  if (pContextData)
    uRet = pContextData->m_arrDimAssoc.size();

  return uRet;
}

//----------------------------------------------------------------------------

OdDgnExportDimAssocData  OdDgnExportContext::getDimensionAssoc(OdUInt32 uIndex)
{
  OdDgnExportDimAssocData retVal;

  OdDgnExportContextData* pContextData = getCurrentContext();

  if( pContextData && (pContextData->m_arrDimAssoc.size() > uIndex) )
    retVal = pContextData->m_arrDimAssoc[uIndex];

  return retVal;
}

//----------------------------------------------------------------------------

void                     OdDgnExportContext::addDimensionAssoc(const OdDgnExportDimAssocData& assocData)
{
  OdDgnExportContextData* pContextData = getCurrentContext();

  if( pContextData )
    pContextData->m_arrDimAssoc.push_back(assocData);
}

//----------------------------------------------------------------------------

void                     OdDgnExportContext::clearAllDimensionAssoc()
{
  OdDgnExportContextData* pContextData = getCurrentContext();

  if (pContextData)
    pContextData->m_arrDimAssoc.clear();
}

//----------------------------------------------------------------------------


}
