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
#include "DgFontTableRecord.h"
#include "DgCellHeader.h"
#include "DgComplexCurve.h"
#include "DgSharedCellDefinition.h"
#include "DgTable.h"
#include "DgnLS/DbLSXData.h"
#include "DgColorTable.h"
#include "DbLayerTableRecord.h"
#include <DgLine.h>
#include <DgArc.h>
#include <DgComplexString.h>
#include <DgLineString.h>
#include <DgShape.h>
#include <DgBSplineCurve.h>
#include <DgEllipse.h>
#include <Ge/GeNurbCurve3d.h>
#include <Ge/GeNurbCurve2d.h>
#include "ColorMapping.h"
#include <Ge/GeGbl.h>
#include <DbBlockTableRecord.h>
#include <DbSymUtl.h>
#include <DgColorTable.h>
#include <DgComplexShape.h>
#include <Gi/GiTextStyle.h>

using namespace TD_DGN_EXPORT;

namespace TD_DGN_EXPORT 
{

//------------------------------------------------------------------------------------------------------------
//                        Implementation of class OdDgnExportLineWeightMap
//------------------------------------------------------------------------------------------------------------

OdDgnExportLineWeightsMapImpl::OdDgnExportLineWeightsMapImpl()
{
  resetToDefaults();
}

//======================================================================================

OdArray< OdDb::LineWeight> OdDgnExportLineWeightsMapImpl::getOverridedDwgLineWeights() const
{
  OdArray< OdDb::LineWeight> arrRet;

  std::map< OdDb::LineWeight, OdUInt32>::const_iterator pIter = m_mapLineWeights.begin();

  for(; pIter != m_mapLineWeights.end(); pIter++)
  {
    arrRet.push_back(pIter->first);
  }

  return arrRet;
}

//======================================================================================

void OdDgnExportLineWeightsMapImpl::resetToDefaults()
{
  m_mapLineWeights.clear();
}


//======================================================================================

OdResult         OdDgnExportLineWeightsMapImpl::setDgnIndexForDwgLineWeight(OdDb::LineWeight uDwgLineWeigth, OdUInt32 uDgnIndex)
{
  m_mapLineWeights[uDwgLineWeigth] = uDgnIndex;

  return eOk;
}

//======================================================================================

void             OdDgnExportLineWeightsMapImpl::copyLineWeightsMap(const OdDgnExportLineWeightsMapPtr& pMap)
{
  resetToDefaults();

  OdArray< OdDb::LineWeight> arrOverrides = pMap->getOverridedDwgLineWeights();

  for( OdUInt32 i = 0; i < arrOverrides.size(); i++ )
    m_mapLineWeights[arrOverrides[i]] = pMap->getDgnIndexByDwgLineWeight(arrOverrides[i]);
}

//======================================================================================

OdUInt32 OdDgnExportLineWeightsMapImpl::getDgnIndexByDwgLineWeight(OdDb::LineWeight uDwgLineWeight) const
{
  std::map< OdDb::LineWeight, OdUInt32>::const_iterator pIter = m_mapLineWeights.find(uDwgLineWeight);

  if (pIter != m_mapLineWeights.end())
    return pIter->second;
  else
    return getDefaultDgnIndexByDwgLineWeight(uDwgLineWeight);
}

//======================================================================================

OdUInt32         OdDgnExportLineWeightsMapImpl::getDefaultDgnIndexByDwgLineWeight(OdDb::LineWeight uDwgLineWeight)
{
  OdUInt32 uRet = 0;

  if (uDwgLineWeight == OdDb::kLnWtByLayer)
    uRet = OdDg::kLineWeightByLevel;
  else if (uDwgLineWeight == OdDb::kLnWtByBlock)
    uRet = OdDgnExportContext::getLineWeightByBlockIndex();
  else if (uDwgLineWeight == OdDb::kLnWtByLwDefault)
    uRet = 0;
  else if (uDwgLineWeight >= OdDb::kLnWt200)
    uRet = 14;
  else if (uDwgLineWeight >= OdDb::kLnWt158)
    uRet = 12;
  else if (uDwgLineWeight >= OdDb::kLnWt140)
    uRet = 10;
  else if (uDwgLineWeight >= OdDb::kLnWt120)
    uRet = 9;
  else if (uDwgLineWeight >= OdDb::kLnWt106)
    uRet = 8;
  else if (uDwgLineWeight >= OdDb::kLnWt100)
    uRet = 7;
  else if (uDwgLineWeight >= OdDb::kLnWt080)
    uRet = 6;
  else if (uDwgLineWeight >= OdDb::kLnWt070)
    uRet = 5;
  else if (uDwgLineWeight >= OdDb::kLnWt053)
    uRet = 4;
  else if (uDwgLineWeight >= OdDb::kLnWt040)
    uRet = 3;
  else if (uDwgLineWeight >= OdDb::kLnWt035)
    uRet = 2;
  else if (uDwgLineWeight >= OdDb::kLnWt030)
    uRet = 1;
  else
    uRet = 0;

  return uRet;
}

//------------------------------------------------------------------------------------------------------------
//                                      Useful functions
//------------------------------------------------------------------------------------------------------------

OdUInt32 getDgnFontEntryId(OdFont* pFont, OdDgDatabase* pDgnDb)
{
  OdUInt32 uRet = 0;

  if (pFont)
  {
    OdString strFileName = pFont->getFileName();

    if (pFont->flags() & OdFont::kTrueType)
    {
      OdTtfDescriptor fontDesc;
      pFont->getDescriptor(fontDesc);

      if (!fontDesc.fileName().isEmpty())
        strFileName = fontDesc.fileName();
    }

    if (!strFileName.isEmpty())
    {
      strFileName.replace(L"\\", L"/");

      if (strFileName.reverseFind(L'/') != -1)
        strFileName = strFileName.right(strFileName.getLength() - strFileName.reverseFind(L'/') - 1);

      if (strFileName.getLength() > 4)
      {
        OdString strFontExt = strFileName.right(4).makeUpper();

        if ((strFontExt == L".TTF") || (strFontExt == L".TTC") || (strFontExt == L".OTF") || (strFontExt == L".SHX"))
          strFileName = strFileName.left(strFileName.getLength() - 4);
      }
    }

    if (!strFileName.isEmpty())
    {
      OdDgFontTablePtr pFontTable = pDgnDb->getFontTable(OdDg::kForWrite);

      OdDgElementId idFont = pFontTable->getAt(strFileName);

      if (!idFont.isNull())
      {
        OdDgFontTableRecordPtr pFontRecord = idFont.openObject(OdDg::kForRead, true);

        if (!pFontRecord.isNull())
          uRet = pFontRecord->getNumber();
      }
      else
      {
        OdDgFontTableRecordPtr pFontRecord = OdDgFontTableRecord::createObject();
        pFontRecord->setName(strFileName);
        pFontRecord->setType(pFont->isShxFont() ? kFontTypeShx : kFontTypeTrueType);
        pFontTable->add(pFontRecord);
        uRet = pFontRecord->getNumber();
      }
    }
  }

  return uRet;
}

//==============================================================================================================

OdUInt32 getDgnFontEntryId(const OdString& strFontName, OdDgDatabase* pDgnDb, bool bShxFont)
{
  OdUInt32 uRet = 0;

  if( !strFontName.isEmpty() )
  {
    OdString strFileName = strFontName;

    strFileName.replace(L"\\", L"/");

    if (strFileName.reverseFind(L'/') != -1)
      strFileName = strFileName.right(strFileName.getLength() - strFileName.reverseFind(L'/') - 1);

    if (strFileName.getLength() > 4)
    {
      OdString strFontExt = strFileName.right(4).makeUpper();

      if ((strFontExt == L".TTF") || (strFontExt == L".TTC") || (strFontExt == L".OTF"))
        bShxFont = false;

      if ((strFontExt == L".TTF") || (strFontExt == L".TTC") || (strFontExt == L".OTF") || (strFontExt == L".SHX"))
        strFileName = strFileName.left(strFileName.getLength() - 4);
    }
   
    if (!strFileName.isEmpty())
    {
      OdDgFontTablePtr pFontTable = pDgnDb->getFontTable(OdDg::kForWrite);

      OdDgElementId idFont = pFontTable->getAt(strFileName);

      if (!idFont.isNull())
      {
        OdDgFontTableRecordPtr pFontRecord = idFont.openObject(OdDg::kForRead, true);

        if (!pFontRecord.isNull())
          uRet = pFontRecord->getNumber();
      }
      else
      {
        OdDgFontTableRecordPtr pFontRecord = OdDgFontTableRecord::createObject();
        pFontRecord->setName(strFileName);
        pFontRecord->setType(bShxFont ? kFontTypeShx : kFontTypeTrueType);
        pFontTable->add(pFontRecord);
        uRet = pFontRecord->getNumber();
      }
    }
  }

  return uRet;
}

//==============================================================================================================

OdUInt32 odDgnExportLineweightMapping(OdDb::LineWeight mappingVal)
{
  DgnExporter* pExporter = OdDgnExportContext::getDgnExporter();

  if (pExporter)
  {
    return pExporter->getDgnIndexByLineWeight(mappingVal);
  }
  else
  {
    return OdDgnExportLineWeightsMapImpl::getDefaultDgnIndexByDwgLineWeight(mappingVal);
  }
}

//==============================================================================================================

void     setDgnLineStyleScale(OdDgGraphicsElement* pDgnElm, double dLineStyleScale)
{
  OdDgLineStyleDataElementPEPtr pLineStylePE = OdDgLineStyleDataElementPE::cast(pDgnElm);

  if (!pLineStylePE.isNull())
    pLineStylePE->setLineStyleScale(pDgnElm, dLineStyleScale * OdDgnExportContext::getLineStyleScaleCorrectionFactor() );
}

//==============================================================================================================

void     setDgnLineStyleModifiers(OdDgGraphicsElement* pDgElm, const OdDbEntity* pDbEnt)
{
  OdGiDgLinetypeModifiers lsMod;
  double dScale = 1.0;

  if( ::oddbDgnLSReadEntityXData(pDbEnt, lsMod, &dScale) )
  {
    OdDgLineStyleDataElementPEPtr pLineStylePE = OdDgLineStyleDataElementPE::cast(pDgElm);

    if (!pLineStylePE.isNull())
      pLineStylePE->setDgnLineStyleModifiers(pDgElm, lsMod);
  }
}

//==============================================================================================================

ODCOLORREF OdCmColorToRGBLayerBlock(const OdCmColor& entityColor,
                            const OdDbObjectId& idLayer,
                              OdDgDatabase* pDgDb )
{
  ODCOLORREF uRet = 0;

  switch (entityColor.colorMethod())
  {
    case OdCmEntityColor::kByDgnIndex:
    case OdCmEntityColor::kByACI:
    {
      const ODCOLORREF* pColors;

      if(!OdDgnExportContext::isDarkPalette())
        pColors = odcmAcadLightPalette();
      else
        pColors = odcmAcadDarkPalette();

      uRet = pColors[entityColor.colorIndex()];
    } break;

    case OdCmEntityColor::kByColor:
    {
      uRet = ODRGB(entityColor.red(), entityColor.green(), entityColor.blue());
    }; break;

    case OdCmEntityColor::kByLayer:
    {
      if (!idLayer.isNull())
      {
        OdDbLayerTableRecordPtr pLayer = idLayer.openObject(OdDb::kForRead);
        uRet = OdCmColorToRGB(pLayer->color());
      }
    } break;

    case OdCmEntityColor::kByBlock:
    {
      uRet = OdDgColorTable::lookupRGB(pDgDb, OdDgnExportContext::getColorByBlockIndex());
    } break;
  }

  return uRet;
}

//==============================================================================================================

ODCOLORREF OdCmEntityColorToRGB(const OdCmEntityColor& entityColor,
                                  const OdDbObjectId& idLayer )
{
  ODCOLORREF uRet = 0;

  switch (entityColor.colorMethod())
  {
    case OdCmEntityColor::kByDgnIndex:
    case OdCmEntityColor::kByACI:
    {
      const ODCOLORREF* pColors;

      if (!OdDgnExportContext::isDarkPalette())
        pColors = odcmAcadLightPalette();
      else
        pColors = odcmAcadDarkPalette();

      uRet = pColors[entityColor.colorIndex()];
    } break;

    case OdCmEntityColor::kByColor:
    {
      uRet = ODRGB(entityColor.red(), entityColor.green(), entityColor.blue());
    }; break;

    case OdCmEntityColor::kByLayer:
    {
      if( !idLayer.isNull() )
      {
        OdDbLayerTableRecordPtr pLayer = idLayer.openObject(OdDb::kForRead);
        uRet = OdCmColorToRGB(pLayer->color());
      }
    } break;
  }

  return uRet;
}

//==============================================================================================================

ODCOLORREF OdCmColorToRGB(const OdCmColor& entityColor )
{
  ODCOLORREF uRet = 0;

  switch (entityColor.colorMethod())
  {
    case OdCmEntityColor::kByDgnIndex:
    case OdCmEntityColor::kByACI:
    {
      const ODCOLORREF* pColors;

      if(!OdDgnExportContext::isDarkPalette())
        pColors = odcmAcadLightPalette();
      else
        pColors = odcmAcadDarkPalette();

      uRet = pColors[entityColor.colorIndex()];
    } break;

    case OdCmEntityColor::kByColor:
    {
      uRet = ODRGB(entityColor.red(), entityColor.green(), entityColor.blue());
    }; break;
  }

  return uRet;
}

//==============================================================================================================

void fillUnitDescription(double dUnitScale, OdDgModel::UnitDescription& descUnit)
{
  bool bCustomUnits = true;

  for (OdUInt32 i = 0; i < 16; i++)
  {
    OdDgModel::UnitDescription presetUnit;
    OdDgModel::fillUnitDescriptor((OdDgModel::UnitMeasure)(i + 1), presetUnit);

    double dPresetScale = presetUnit.m_numerator / presetUnit.m_denominator;

    if (OdEqual(dPresetScale, dUnitScale))
    {
      bCustomUnits = false;
      descUnit = presetUnit;
      break;
    }
  }

  if (bCustomUnits)
  {
    descUnit.m_numerator = dUnitScale;
    descUnit.m_denominator = 1.0;
  }
}

//==============================================================================================================

OdDgDimension::TerminatorArrowHeadType getTerminatorArrowType(OdDgnExportDimArrowheadType arrowType)
{
  OdDgDimension::TerminatorArrowHeadType aType = OdDgDimension::kOpened;

  switch (arrowType)
  {
    case acArrowDot:
    case acArrowClosed:
    case acArrowDotSmall:
    case acArrowBoxFilled:
    case acArrowDatumFilled:
    {
      aType = OdDgDimension::kFilled;
    } break;

    case acArrowClosedBlank:
    case acArrowDotBlank:
    case acArrowBoxBlank:
    case acArrowDatumBlank:
    {
      aType = OdDgDimension::kClosed;
    } break;
  }

  return aType;
}

//==============================================================================================================

double getDimDefaultArrowHeight(OdDgnExportDimArrowheadType arrowType, double dArrowSize)
{
  double dRet = dArrowSize / 2.0;

  switch( arrowType )
  {
    case acArrowDefault:
    case acArrowNone:
    case acArrowUserDefined:
    case acArrowClosedBlank:
    case acArrowClosed:
    case acArrowOpen:
      dRet = dArrowSize / 2.0; break;

    case acArrowDot:
    case acArrowOrigin:
    case acArrowOrigin2:
    case acArrowArchTick:
    case acArrowOblique:
    case acArrowDotSmall:
    case acArrowDotBlank:
    case acArrowSmall:
    case acArrowBoxBlank:
    case acArrowBoxFilled:
    case acArrowDatumBlank:
    case acArrowDatumFilled:
    case acArrowIntegral:
    case acArrowOpen90:
      dRet = dArrowSize; break;

    case acArrowOpen30:
      dRet = dArrowSize * 2.0 * tan(OdaPI / 6); break;
  }

  return dRet;
}

//==============================================================================================================

OdDgnExportDimArrowheadType getArrowheadDwgType(OdDbObjectId objId)
{
  OdString pName = OdDmUtil::arrowName(objId);

  OdDgnExportDimArrowheadType retVal = acArrowDefault;

  if (!odStrICmp(pName, OD_T("_None")) || !odStrICmp(pName, OD_T("None")))
    retVal = acArrowNone;
  else if (!odStrICmp(pName, OD_T("_Closed")) || !odStrICmp(pName, OD_T("Closed")))
    retVal = acArrowClosed;
  else if (!odStrICmp(pName, OD_T("_Dot")) || !odStrICmp(pName, OD_T("Dot")))
    retVal = acArrowDot;
  else if (!odStrICmp(pName, OD_T("_ClosedBlank")) || !odStrICmp(pName, OD_T("ClosedBlank")))
    retVal = acArrowClosedBlank;
  else if (!odStrICmp(pName, OD_T("_Oblique")) || !odStrICmp(pName, OD_T("Oblique")))
    retVal = acArrowOblique;
  else if (!odStrICmp(pName, OD_T("_ArchTick")) || !odStrICmp(pName, OD_T("ArchTick")))
    retVal = acArrowArchTick;
  else if (!odStrICmp(pName, OD_T("_Open")) || !odStrICmp(pName, OD_T("Open")))
    retVal = acArrowOpen;
  else if (!odStrICmp(pName, OD_T("_Origin")) || !odStrICmp(pName, OD_T("Origin")))
    retVal = acArrowOrigin;
  else if (!odStrICmp(pName, OD_T("_Origin2")) || !odStrICmp(pName, OD_T("Origin2")))
    retVal = acArrowOrigin2;
  else if (!odStrICmp(pName, OD_T("_Open90")) || !odStrICmp(pName, OD_T("Open90")))
    retVal = acArrowOpen90;
  else if (!odStrICmp(pName, OD_T("_Open30")) || !odStrICmp(pName, OD_T("Open30")))
    retVal = acArrowOpen30;
  else if (!odStrICmp(pName, OD_T("_DotBlank")) || !odStrICmp(pName, OD_T("DotBlank")))
    retVal = acArrowDotBlank;
  else if (!odStrICmp(pName, OD_T("_DotSmall")) || !odStrICmp(pName, OD_T("DotSmall")))
    retVal = acArrowDotSmall;
  else if (!odStrICmp(pName, OD_T("_BoxFilled")) || !odStrICmp(pName, OD_T("BoxFilled")))
    retVal = acArrowBoxFilled;
  else if (!odStrICmp(pName, OD_T("_BoxBlank")) || !odStrICmp(pName, OD_T("BoxBlank")))
    retVal = acArrowBoxBlank;
  else if (!odStrICmp(pName, OD_T("_DatumFilled")) || !odStrICmp(pName, OD_T("DatumFilled")))
    retVal = acArrowDatumFilled;
  else if (!odStrICmp(pName, OD_T("_DatumBlank")) || !odStrICmp(pName, OD_T("DatumBlank")))
    retVal = acArrowDatumBlank;
  else if (!odStrICmp(pName, OD_T("_Integral")) || !odStrICmp(pName, OD_T("Integral")))
    retVal = acArrowIntegral;
  else if (!odStrICmp(pName, OD_T("_Small")) || !odStrICmp(pName, OD_T("Small")))
    retVal = acArrowSmall;
  else if (pName.getLength() == 0)
    retVal = acArrowClosed;
  else
    retVal = acArrowUserDefined;

  return retVal;
}

//==============================================================================================================

OdUInt32 getDgnColorIndex(const OdCmColor& entityColor, OdDgDatabase* pDgnDb )
{
  OdUInt32 uRet = 0;

  switch( entityColor.colorMethod() )
  {
    case OdCmEntityColor::kByLayer: uRet = OdDg::kColorByLevel; break;
    case OdCmEntityColor::kByBlock: uRet = OdDgnExportContext::getColorByBlockIndex(); break;
    case OdCmEntityColor::kByDgnIndex: uRet = entityColor.colorIndex(); break;
    case OdCmEntityColor::kByACI:
    {
      if(entityColor.colorIndex() < 255)
        uRet = entityColor.colorIndex();
      else
      {
        const ODCOLORREF* pColors;

        if (!OdDgnExportContext::isDarkPalette())
          pColors = odcmAcadLightPalette();
        else
          pColors = odcmAcadDarkPalette();

        uRet = OdDgColorTable::getColorIndexByRGB(pDgnDb, pColors[255] );
      }
    }
    break;
    case OdCmEntityColor::kByColor:
    {
      uRet = OdDgColorTable::getColorIndexByRGB(pDgnDb, ODRGB(entityColor.red(), entityColor.green(), entityColor.blue()));
    }; break;
  }

  return uRet;
}

//==============================================================================================================

OdUInt32 getDgnEntityColorIndex(const OdCmEntityColor& entityColor, OdDgDatabase* pDgnDb )
{
  OdUInt32 uRet = 0;

  switch (entityColor.colorMethod())
  {
    case OdCmEntityColor::kByLayer: uRet = OdDg::kColorByLevel; break;
    case OdCmEntityColor::kByBlock: uRet = OdDgnExportContext::getColorByBlockIndex(); break;
    case OdCmEntityColor::kByDgnIndex: uRet = entityColor.colorIndex(); break;
    case OdCmEntityColor::kByACI:
    {
      if (entityColor.colorIndex() < 255)
        uRet = entityColor.colorIndex();
      else
      {
        const ODCOLORREF* pColors;

        if (!OdDgnExportContext::isDarkPalette())
          pColors = odcmAcadLightPalette();
        else
          pColors = odcmAcadDarkPalette();

        uRet = OdDgColorTable::getColorIndexByRGB(pDgnDb, pColors[255]);
      }
    } break;
    case OdCmEntityColor::kByColor:
    {
      uRet = OdDgColorTable::getColorIndexByRGB(pDgnDb, ODRGB(entityColor.red(), entityColor.green(), entityColor.blue()));
    }; break;
  }

  return uRet;
}

//==============================================================================================================

bool     getDgnVisibility(const OdDbEntity* pDbEnt)
{
  bool bRet = true;

  if( pDbEnt )
    bRet = pDbEnt->visibility() == OdDb::kVisible;

  return bRet;
}

//==============================================================================================================

void     addElementToDatabase(OdDgElement* pElm, const OdDgElement* pOwner)
{
  if( !OdDgnExportContext::isKeepNonDbro() )
    pOwner->database()->addOdDgElement(pElm, pOwner->elementId());
}

//==============================================================================================================

bool     isNonModelOwner(const OdDgElement* pElm)
{
  if( pElm )
  {
    if( pElm->isKindOf(OdDgModel::desc()) )
      return false;
    else
    {
      OdDgElementId idOwner = pElm->ownerId();

      if( !idOwner.isNull() )
      {
        OdDgElementPtr pOwner = idOwner.openObject(OdDg::kForRead);
        return isNonModelOwner(pOwner);
      }
    }
  }

  return true;
}

//==============================================================================================================

bool     isPlanar(const OdGePoint3d* pPts, OdUInt32 nPts, double dTolerance )
{
  OdGePoint3dArray arrPts;
  arrPts.resize(nPts);
  memcpy(arrPts.asArrayPtr(), pPts, nPts*sizeof(OdGePoint3d));
  return isPlanar(arrPts, dTolerance);
}

//==============================================================================================================

bool     isPlanar(const OdGePoint3dArray& arrPts, double dTolerance)
{
  bool bRet = true;

  if( arrPts.size() > 3 )
  {
    OdGeVector3d vrNormal;
    OdGeError retVal = geCalculateNormal(arrPts, &vrNormal, dTolerance);
    bRet = retVal == OdGe::eOk;
  }

  return bRet;
}

//==============================================================================================================

OdUInt32 getDgnTextStyleIndex(const OdDbObjectId& idDbTextStyle)
{
  OdUInt32 uRet = 0;

  if( !idDbTextStyle.isNull() )
  {
    OdDgElementId idDgTextStyle = OdDgnExportContext::getElementId(idDbTextStyle);

    if( !idDgTextStyle.isNull() )
    {
      OdDgTextStyleTableRecordPtr pTextStyle = idDgTextStyle.openObject(OdDg::kForRead);

      if (!pTextStyle.isNull())
        uRet = pTextStyle->getEntryId();
    }
  }

  return uRet;
}

//==============================================================================================================

bool     isXRef(const OdDbBlockTableRecord* pDbBlock)
{
  if (!pDbBlock || pDbBlock->isFromExternalReference())
    return true;

  OdString strModelSpaceName = OdDbSymUtil::blockModelSpaceName();
  OdString strPaperSpaceName = OdDbSymUtil::blockPaperSpaceName();
  strModelSpaceName.makeUpper();
  strPaperSpaceName.makeUpper();

  OdString strBlockName = pDbBlock->getName();
  OdString strBlockNameUpperCase = strBlockName;
  strBlockNameUpperCase.makeUpper();

  if( (strBlockNameUpperCase == strModelSpaceName) || (strBlockNameUpperCase.find(strPaperSpaceName) != -1) )
    return true;

  return false;
}

//==============================================================================================================

bool     isContinuousLineStyle(const OdDbObjectId& idDbLineStyle, const OdDbObjectId& idLayer, bool bLineWidthIsConstant)
{
  bool bRet = true;

  if( !idDbLineStyle.isNull() )
  {
    OdDbObjectId idDbLS = idDbLineStyle;

    if( (idDbLineStyle == idDbLineStyle.database()->getLinetypeByLayerId()) && !idLayer.isNull() )
    {
      OdDbLayerTableRecordPtr pLayer = idLayer.openObject(OdDb::kForRead);
      
      if (!pLayer.isNull())
        idDbLS = pLayer->linetypeObjectId();
    }

    OdDgElementId idDgLineStyle = OdDgnExportContext::getElementId(idDbLS);

    if( !idDgLineStyle.isNull() )
    {
      OdDgLineStyleTableRecordPtr pDgLineStyle = idDgLineStyle.openObject(OdDg::kForRead);

      if( !pDgLineStyle.isNull() && (pDgLineStyle->getEntryId() > 7))
      {
        if( bLineWidthIsConstant && pDgLineStyle->getRefersToElementFlag() )
        {
          OdDgElementId idLineStyleDef = pDgLineStyle->getRefersToId();

          if( !idLineStyleDef.isNull() )
          {
            OdDgLineStyleDefTableRecordPtr pLineStyleDef = idLineStyleDef.openObject(OdDg::kForRead);

            if( !pLineStyleDef.isNull() && (pLineStyleDef->getType() == OdDg::kLsTypeLineCode) )
            {
              OdDgLineCodeResourcePtr pRes = pLineStyleDef->getResource();

              if (pRes->getStrokeCount() < 2)
                bRet = false;
            }
          }
        }
        else
          bRet = false;
      }
    }
  }

  return bRet;
}

//==============================================================================================================

void     setDgnMaterial(OdDgGraphicsElement* pDgElm,
                          const OdDbDatabase* pDwgDb,
                            const OdDbObjectId& idMat, 
                              const OdDbObjectId& idLayer )
{
  if( !pDgElm || !pDwgDb )
    return;

  if (!pDgElm->isMaterialPropertyAvailble())
    return;

  OdDbObjectId idMaterial = idMat;

  if( !idMaterial.isNull() && pDwgDb)
  {
    if( idMaterial == pDwgDb->byLayerMaterialId() )
    {
      if (!idLayer.isNull())
      {
        OdDbLayerTableRecordPtr pLayer = idLayer.openObject(OdDb::kForRead, true);
        idMaterial = pLayer->materialId();
      }
      else
        idMaterial = NULL;
    }

    if( !idMaterial.isNull() && (idMaterial != pDwgDb->byBlockMaterialId()) && (idMaterial != pDwgDb->globalMaterialId()) )
    {
      OdDgElementId idDgMaterial = OdDgnExportContext::getElementId(idMaterial);

      if( !idDgMaterial.isNull() )
        pDgElm->setMaterial(idDgMaterial);
    }
  }
}

//==============================================================================================================

OdUInt32 getDgnLineStyleEntryId(const OdDbObjectId& idDbLineStyle)
{
  OdUInt32 uRet = 0;

  if (idDbLineStyle.isNull())
    return uRet;

  OdDbDatabase* pDb = idDbLineStyle.database();

  if( idDbLineStyle == pDb->getLinetypeByLayerId() )
    uRet = OdDg::kLineStyleByLevel;
  else if( idDbLineStyle == pDb->getLinetypeByBlockId() )
    uRet = OdDgnExportContext::getLineStyleByBlockIndex();
  else
  {
    OdDgElementId idDgLineStyle = OdDgnExportContext::getElementId(idDbLineStyle);

    if( !idDgLineStyle.isNull() )
    {
      OdDgElementPtr pDgLineStyleElm = idDgLineStyle.openObject(OdDg::kForRead);

      if( !pDgLineStyleElm.isNull() && pDgLineStyleElm->isKindOf(OdDgLineStyleTableRecord::desc()))
      {
        OdDgLineStyleTableRecordPtr pDgLineStyle = pDgLineStyleElm;
        uRet = pDgLineStyle->getEntryId();
      }
    }
  }

  return uRet;
}

//==============================================================================================================

void copyElementProperties(OdDgGraphicsElement* pDgElm, const OdDbEntity* pDbEnt, OdDgDatabase* pDgnDb, bool bSetLineStyleModifiers )
{
  // level

  OdDgElementId idLayer = OdDgnExportContext::getElementId(pDbEnt->layerId());

  if (!idLayer.isNull())
    pDgElm->setLevelId(idLayer);
  else
  {
    pDgElm->setLevelEntryId(0x40);
    //ODA_ASSERT_ONCE(!"DgnExport: layer id is undefined.");
  }

  // line style

  pDgElm->setLineStyleEntryId(getDgnLineStyleEntryId(pDbEnt->linetypeId()));

  // line style scale

  setDgnLineStyleScale(pDgElm, pDbEnt->linetypeScale());

  // line style modifiers

  if( bSetLineStyleModifiers )
    setDgnLineStyleModifiers(pDgElm, pDbEnt);

  // line weight

  pDgElm->setLineWeight(odDgnExportLineweightMapping(pDbEnt->lineWeight()));

  // color

  pDgElm->setColorIndex(getDgnColorIndex(pDbEnt->color(), pDgnDb));

  // transparency

  pDgElm->setTransparency(pDbEnt->transparency());

  // visibility

  pDgElm->setInvisibleFlag(!getDgnVisibility(pDbEnt));

  // material

  setDgnMaterial(pDgElm, pDbEnt->database(), pDbEnt->materialId(), pDbEnt->layerId());
}

//==============================================================================================================

void appendOdDgElementToOwner(OdDgElement* pOwner, const OdDgElement* pDgElm)
{
  if (!pOwner || !pDgElm)
    return;

  if (pOwner->isKindOf(OdDgModel::desc()))
  {
    OdDgModelPtr pModel = pOwner;
    pModel->addElement(pDgElm);
  }
  else if (pOwner->isKindOf(OdDgCellHeader2d::desc()))
  {
    OdDgCellHeader2dPtr pCell = pOwner;
    pCell->add(pDgElm);
  }
  else if (pOwner->isKindOf(OdDgCellHeader3d::desc()))
  {
    OdDgCellHeader3dPtr pCell = pOwner;
    pCell->add(pDgElm);
  }
  else if (pOwner->isKindOf(OdDgComplexCurve::desc()))
  {
    OdDgComplexCurvePtr pCplxCurve = pOwner;
    pCplxCurve->add(pDgElm);
  }
  else if (pOwner->isKindOf(OdDgSharedCellDefinition::desc()))
  {
    OdDgSharedCellDefinitionPtr pCellDef = pOwner;
    pCellDef->add(pDgElm);
  }
  else if (pOwner->isKindOf(OdDgTable::desc()))
  {
    OdDgTablePtr pTable = pOwner;
    pTable->add(pDgElm);
  }
  else if (pOwner->isKindOf(OdDgTextNode2d::desc()))
  {
    OdDgTextNode2dPtr pTextNode = pOwner;
    pTextNode->add(pDgElm);
  }
  else if (pOwner->isKindOf(OdDgTextNode3d::desc()))
  {
    OdDgTextNode3dPtr pTextNode = pOwner;
    pTextNode->add(pDgElm);
  }
  else
  {
    ODA_ASSERT_ONCE(!"DgnExport: can't add element to non complex owner!");
  }
}

//==============================================================================================================

OdDgGraphicsElementPtr createDgLine(const OdGePoint3d& ptStart, const OdGePoint3d& ptEnd, OdDgDatabase* pDb)
{
  OdDgGraphicsElementPtr pDgLine;

  if (OdDgnExportContext::is3d())
  {
    OdDgLine3dPtr pLine3d = OdDgLine3d::createObject();
    pLine3d->setDatabaseDefaults(pDb);
    pLine3d->setStartPoint(ptStart);
    pLine3d->setEndPoint(ptEnd);
    pDgLine = pLine3d;
  }
  else
  {
    OdDgLine2dPtr pLine2d = OdDgLine2d::createObject();
    pLine2d->setDatabaseDefaults(pDb);
    pLine2d->setStartPoint(ptStart.convert2d());
    pLine2d->setEndPoint(ptEnd.convert2d());
    pDgLine = pLine2d;
  }

  return pDgLine;
}

//==============================================================================================================

OdDgGraphicsElementPtr createDgCurveFromGeCurve(OdGeCurve3d* pGeCurve, OdGeVector3d& vrNormal, OdDgDatabase* pDb, bool bCloseNurbs )
{
  OdDgGraphicsElementPtr pRet;

  if (!pGeCurve)
    return pRet;

  if (OdDgnExportContext::is3d())
  {
    OdDgCurveElement3d* pDgCurve3d = NULL;

    if( bCloseNurbs && pGeCurve && pGeCurve->type() == OdGe::kNurbCurve3d )
    {
      OdGeNurbCurve3d* pGeNurbCurve = (OdGeNurbCurve3d*)(pGeCurve);

      OdDgBSplineCurve3dPtr pDgNurbCurve = OdDgBSplineCurve3d::createObject();

      if( pDgNurbCurve->setFromOdGeNurbCurve(*pGeNurbCurve, &vrNormal, OdGeContext::gTol, false) == eOk )
        pRet = pDgNurbCurve;
    }

    if( pRet.isNull() && (OdDgCurveElement3d::createFromOdGeCurve(*pGeCurve, pDgCurve3d, &vrNormal) == eOk) )
      pRet = pDgCurve3d;

    if (pDgCurve3d)
      pDgCurve3d->release();
  }
  else
  {
    OdGeCurve2d* pGeCurve2d = pGeCurve->convertTo2d();

    if (pGeCurve2d)
    {
      OdDgCurveElement2d* pDgCurve2d = NULL;

      if (bCloseNurbs && pGeCurve && pGeCurve->type() == OdGe::kNurbCurve2d)
      {
        OdGeNurbCurve2d* pGeNurbCurve = (OdGeNurbCurve2d*)(pGeCurve);

        OdDgBSplineCurve2dPtr pDgNurbCurve = OdDgBSplineCurve2d::createObject();

        if( pDgNurbCurve->setFromOdGeNurbCurve(*pGeNurbCurve, OdGeContext::gTol, false) == eOk )
          pRet = pDgNurbCurve;
      }

      if( pRet.isNull() && (OdDgCurveElement2d::createFromOdGeCurve(*pGeCurve2d, pDgCurve2d) == eOk) )
        pRet = pDgCurve2d;

      if (pDgCurve2d)
        pDgCurve2d->release();

      delete pGeCurve2d;
    }
  }

  if (!pRet.isNull())
    pRet->setDatabaseDefaults(pDb);

  return pRet;
}

//==============================================================================================================

OdDgGraphicsElementPtr createDgLineString(const OdGePoint3dArray& arrVertices, 
                                            const OdDbEntityPtr& pDbEnt,
                                              OdDgDatabase* pDb)
{
  OdDgGraphicsElementPtr pRet;

  if (arrVertices.isEmpty())
    return pRet;
  else if (arrVertices.size() == 1)
    return createDgLine(arrVertices[0], arrVertices[0], pDb);
  else if (arrVertices.size() == 2)
    return createDgLine(arrVertices[0], arrVertices[1], pDb);

  if (arrVertices.size() > 5000)
  {
    OdDgComplexStringPtr pCplxStr = OdDgComplexString::createObject();
    pCplxStr->setDatabaseDefaults(pDb);

    OdUInt32 uCurVertex = 0;

    while (uCurVertex < arrVertices.size())
    {
      OdUInt32 uStart = uCurVertex;
      OdUInt32 uEnd = uCurVertex + 5000;

      if (uEnd > arrVertices.size())
        uEnd = arrVertices.size();

      OdGePoint3dArray arrLimitPts;
      arrLimitPts.insert(arrLimitPts.begin(), arrVertices.begin() + uStart, arrVertices.begin() + uEnd);

      OdDgGraphicsElementPtr pGrElm = createDgLineString(arrLimitPts, pDbEnt, pDb);

      if (!pGrElm.isNull())
      {
        copyElementProperties(pGrElm, pDbEnt, pDb);
        pCplxStr->add(pGrElm);
      }

      uCurVertex += 4999;
    }
    pRet = pCplxStr;

    return pRet;
  }

  if (OdDgnExportContext::is3d())
  {
    OdDgLineString3dPtr pLineString3d = OdDgLineString3d::createObject();

    for (OdUInt32 i = 0; i < arrVertices.size(); i++)
      pLineString3d->addVertex(arrVertices[i]);

    pLineString3d->setDatabaseDefaults(pDb);
    pRet = pLineString3d;
  }
  else
  {
    OdDgLineString2dPtr pLineString2d = OdDgLineString2d::createObject();

    for (OdUInt32 i = 0; i < arrVertices.size(); i++)
      pLineString2d->addVertex(arrVertices[i].convert2d());

    pLineString2d->setDatabaseDefaults(pDb);
    pRet = pLineString2d;
  }

  return pRet;
}

//==============================================================================================================

OdDgGraphicsElementPtr createDgArcFromGeCircleArc3d(const OdGeCircArc3d& geArc, bool bCreateEllipse, OdDgDatabase* pDb)
{
  OdDgGraphicsElementPtr pRet;

  OdGeVector3d vrXAxis = geArc.refVec();
  OdGeVector3d vrNormal = geArc.normal();

  if( geArc.isClosed() && (bCreateEllipse || OdDgnExportContext::getAllowCloseCurvesFlag()) )
  {
    if( OdDgnExportContext::is3d() )
    {
      OdDgEllipse3dPtr pEllipse3d = OdDgEllipse3d::createObject();
      pEllipse3d->setDatabaseDefaults(pDb);
      pEllipse3d->setOrigin(geArc.center());
      pEllipse3d->setPrimaryAxis(geArc.radius());
      pEllipse3d->setSecondaryAxis(geArc.radius());

      OdGeMatrix3d matRotation;
      matRotation.setCoordSystem(OdGePoint3d::kOrigin, vrXAxis, vrNormal.crossProduct(vrXAxis), vrNormal);
      OdGeQuaternion quatRotation;
      quatRotation.set(matRotation);
      pEllipse3d->setRotation(quatRotation);
      pRet = pEllipse3d;
    }
    else
    {
      OdDgEllipse2dPtr pEllipse2d = OdDgEllipse2d::createObject();
      pEllipse2d->setDatabaseDefaults(pDb);
      pEllipse2d->setOrigin(geArc.center().convert2d());
      pEllipse2d->setPrimaryAxis(geArc.radius());
      pEllipse2d->setSecondaryAxis(geArc.radius());
      pEllipse2d->setRotationAngle(OdGeVector3d::kXAxis.angleTo(vrXAxis, vrNormal));
      pRet = pEllipse2d;
    }
  }
  else
  {
    double dStartAngle = geArc.startAng();
    double dEndAngle = geArc.endAng();

    if (dEndAngle < dStartAngle)
      dEndAngle += Oda2PI;

    if (OdDgnExportContext::is3d())
    {
      OdDgArc3dPtr pArc3d = OdDgArc3d::createObject();
      pArc3d->setDatabaseDefaults(pDb);
      pArc3d->setOrigin(geArc.center());
      pArc3d->setPrimaryAxis(geArc.radius());
      pArc3d->setSecondaryAxis(geArc.radius());

      OdGeMatrix3d matRotation;
      matRotation.setCoordSystem(OdGePoint3d::kOrigin, vrXAxis, vrNormal.crossProduct(vrXAxis), vrNormal);
      OdGeQuaternion quatRotation;
      quatRotation.set(matRotation);
      pArc3d->setRotation(quatRotation);
      pArc3d->setStartAngle(dStartAngle);
      pArc3d->setSweepAngle(dEndAngle);
      pRet = pArc3d;
    }
    else
    {
      OdDgArc2dPtr pArc2d = OdDgArc2d::createObject();
      pArc2d->setDatabaseDefaults(pDb);
      pArc2d->setOrigin(geArc.center().convert2d());
      pArc2d->setPrimaryAxis(geArc.radius());
      pArc2d->setSecondaryAxis(geArc.radius());
      pArc2d->setRotationAngle(OdGeVector3d::kXAxis.angleTo(vrXAxis, vrNormal));
      pArc2d->setStartAngle(dStartAngle);
      pArc2d->setSweepAngle(dEndAngle);
      pRet = pArc2d;
    }
  }

  return pRet;
}

//==============================================================================================================

OdDgGraphicsElementPtr createDgShape( const OdGePoint3dArray& arrVertices, 
                                        const OdDbEntityPtr& pDbEnt,
                                          OdDgDatabase* pDb)
{
  OdDgGraphicsElementPtr pRet;

  if (arrVertices.size() < 3)
    return pRet;


  if( arrVertices.size() > 5000 )
  {
    OdDgComplexShapePtr pCplxShape = OdDgComplexShape::createObject();
    pCplxShape->setDatabaseDefaults(pDb);

    OdUInt32 uCurVertex = 0;

    while( uCurVertex < arrVertices.size() )
    {
      OdUInt32 uStart = uCurVertex;
      OdUInt32 uEnd = uCurVertex + 5000;

      if (uEnd > arrVertices.size())
        uEnd = arrVertices.size();

      OdGePoint3dArray arrLimitPts;
      arrLimitPts.insert(arrLimitPts.begin(), arrVertices.begin() + uStart, arrVertices.begin() + uEnd);

      OdDgGraphicsElementPtr pGrElm = createDgLineString(arrLimitPts, pDbEnt, pDb);

      if( !pGrElm.isNull() )
      {
        copyElementProperties(pGrElm, pDbEnt, pDb);
        pCplxShape->add(pGrElm);
      }

      uCurVertex += 4999;
    }
    pRet = pCplxShape;

    return pRet;
  }

  if (OdDgnExportContext::is3d())
  {
    OdDgShape3dPtr pShape3d = OdDgShape3d::createObject();

    for (OdUInt32 i = 0; i < arrVertices.size(); i++)
      pShape3d->addVertex(arrVertices[i]);

    if( !arrVertices.first().isEqualTo(arrVertices.last()) )
      pShape3d->addVertex(pShape3d->getVertexAt(0));

    pShape3d->setDatabaseDefaults(pDb);
    pRet = pShape3d;
  }
  else
  {
    OdDgShape2dPtr pShape2d = OdDgShape2d::createObject();

    for( OdUInt32 i = 0; i < arrVertices.size(); i++ )
      pShape2d->addVertex(arrVertices[i].convert2d());

    if( !arrVertices.first().isEqualTo(arrVertices.last()) )
      pShape2d->addVertex(pShape2d->getVertexAt(0));

    pShape2d->setDatabaseDefaults(pDb);
    pRet = pShape2d;
  }

  return pRet;
}

//==============================================================================================================

bool isPointInsideOfContour(const OdGePoint2d& point, const OdGePoint2dArray& contour)
{
  int leftSegments = 0;
  OdUInt32 i, j = contour.size();

  for (i = 0; i < j; i++)
  {
    const OdGePoint2d& first = contour[i];
    const OdGePoint2d& second = contour[(i + 1) % j];

    if ((first.y >= point.y && second.y >= point.y) || (first.y < point.y && second.y < point.y))
      continue;

    if (first.y == second.y)
      continue;

    if (OdZero(first.x - second.x) && OdZero(first.x - point.x))
      continue;

    if (((point.y - first.y) * (second.x - first.x) < (point.x - first.x) * (second.y - first.y)) ^ (first.y > second.y))
      leftSegments++;
  }

  return (leftSegments % 2) > 0;
}

//==============================================================================================================

bool isExtentsInsideOfExtents(const OdGeExtents2d& ext1, const OdGeExtents2d& ext2)
{
  bool bRet = false;

  if (!OdLess(ext1.minPoint().x, ext2.minPoint().x) &&
      !OdLess(ext1.minPoint().y, ext2.minPoint().y) &&
      !OdGreater(ext1.maxPoint().x, ext2.maxPoint().x) &&
      !OdGreater(ext1.maxPoint().y, ext2.maxPoint().y)
    )
  {
    bRet = true;
  }

  return bRet;
}

//==============================================================================================================

bool isContourInsideOfContour(const OdGePoint2dArray& arrContour1, const OdGePoint2dArray& arrContour2)
{
  bool bRet = false;

  for( OdUInt32 i = 0; i < arrContour1.size()-1; i++ )
  {
    if(isPointInsideOfContour(arrContour1[i], arrContour2))
    {
      bRet = true;
      break;
    }
  }

  return bRet;
}

//==============================================================================================================

void separateContours( const OdArray<OdGePoint2dArray>& arrGeContours, 
                         const OdArray<bool>& arrOutermostLoopStatus, 
                           OdUInt32Array& arrContourLevels )
{
  arrContourLevels.resize(arrGeContours.size(), 0);
  OdArray<OdGeExtents2d> arrExtents;
  OdUInt32 i, j;
  OdArray<OdUInt32Array> arrExternalContours;
  arrExternalContours.resize(arrGeContours.size());

  for( i = 0; i < arrGeContours.size(); i++ )
  {
    OdGeExtents2d extContour;

    for (OdUInt32 j = 0; j < arrGeContours[i].size(); j++)
      extContour.addPoint(arrGeContours[i][j]);

    arrExtents.push_back(extContour);
  }

  for( i = 0; i < arrGeContours.size(); i++ )
  {
    for( j = 0; j < arrGeContours.size(); j++ )
    {
      if( i == j )
        continue;

      if( !isExtentsInsideOfExtents(arrExtents[i], arrExtents[j]))
        continue;

      if( isContourInsideOfContour( arrGeContours[i], arrGeContours[j]) )
        arrExternalContours[i].push_back(j);
    }
  }

  OdUInt32Array arrOutermostLevel;
  arrOutermostLevel.resize(arrGeContours.size(), 0);

  for( i = 0; i < arrGeContours.size(); i++ )
  {
    if( arrOutermostLoopStatus[i] )
      arrOutermostLevel[i] = arrExternalContours[i].size();
    else
      arrOutermostLevel[i] = 0;
  }

  for( i = 0; i < arrGeContours.size(); i++ )
  {
    if( arrOutermostLoopStatus[i] || (arrExternalContours[i].size() == 0) )
      arrContourLevels[i] = 0;
    else
    {
      OdUInt32 uHighestOutermostLevel = 0;

      for (j = 0; j < arrExternalContours[i].size(); j++)
      {
        if (arrOutermostLevel[i] > uHighestOutermostLevel)
          uHighestOutermostLevel = arrOutermostLevel[i];
      }

      ODA_ASSERT_ONCE(uHighestOutermostLevel < arrExternalContours[i].size());
      arrContourLevels[i] = arrExternalContours[i].size() - uHighestOutermostLevel;
    }
  }
}

//==============================================================================================================

bool isClosedCurve(OdDgGraphicsElement* pDgElm)
{
  bool bRet = false;

  if( pDgElm->isKindOf(OdDgCurveElement2d::desc()) )
  {
    OdDgCurveElement2d* pCurve = (OdDgCurveElement2d*)pDgElm;
    OdGePoint2d ptStart, ptEnd;

    if( (pCurve->getStartPoint(ptStart) == eOk) && (pCurve->getEndPoint(ptEnd) == eOk) )
      bRet = ptStart.isEqualTo(ptEnd);
  }
  else if (pDgElm->isKindOf(OdDgCurveElement3d::desc()))
  {
    OdDgCurveElement3d* pCurve = (OdDgCurveElement3d*)pDgElm;
    OdGePoint3d ptStart, ptEnd;

    if ((pCurve->getStartPoint(ptStart) == eOk) && (pCurve->getEndPoint(ptEnd) == eOk))
      bRet = ptStart.isEqualTo(ptEnd);
  }

  return bRet;
}

//===================================================================================================

void getDgnCurveStartEndPoint(const OdDgGraphicsElementPtr& pDgCurve, OdGePoint3d& ptStart, OdGePoint3d& ptEnd)
{
  ODA_ASSERT_ONCE(pDgCurve->isKindOf(OdDgCurveElement2d::desc()) || pDgCurve->isKindOf(OdDgCurveElement3d::desc()));

  if (pDgCurve->isKindOf(OdDgCurveElement2d::desc()))
  {
    OdDgCurveElement2dPtr pDgCurve2d = pDgCurve;

    OdGePoint2d pt2dStart, pt2dEnd;
    pDgCurve2d->getStartPoint(pt2dStart);
    pDgCurve2d->getEndPoint(pt2dEnd);

    ptStart.set(pt2dStart.x, pt2dStart.y, 0.0);
    ptEnd.set(pt2dEnd.x, pt2dEnd.y, 0.0);
  }
  else if (pDgCurve->isKindOf(OdDgCurveElement3d::desc()))
  {
    OdDgCurveElement3dPtr pDgCurve3d = pDgCurve;

    pDgCurve3d->getStartPoint(ptStart);
    pDgCurve3d->getEndPoint(ptEnd);
  }
}

//===================================================================================================

void reverseDgnCurve(OdDgGraphicsElementPtr& pDgCurve)
{
  ODA_ASSERT_ONCE(pDgCurve->isKindOf(OdDgCurveElement2d::desc()) || pDgCurve->isKindOf(OdDgCurveElement3d::desc()));

  if (pDgCurve->isKindOf(OdDgCurveElement2d::desc()))
  {
    OdDgCurveElement2dPtr pDgCurve2d = pDgCurve;
    pDgCurve2d->reverseCurve();
  }
  else if (pDgCurve->isKindOf(OdDgCurveElement3d::desc()))
  {
    OdDgCurveElement3dPtr pDgCurve3d = pDgCurve;
    pDgCurve3d->reverseCurve();
  }
}

//===================================================================================================

OdGePoint3dArray getShapePtsFromSetOfDgnCurves(const OdArray<OdDgGraphicsElementPtr>& arrDgnCurves)
{
  OdGePoint3dArray retVal;

  for (OdUInt32 i = 0; i < arrDgnCurves.size(); i++)
  {
    OdGePoint3dArray arrCurPts;

    if (arrDgnCurves[i]->isKindOf(OdDgLine2d::desc()))
    {
      OdDgLine2dPtr pLine2d = arrDgnCurves[i];
      OdGePoint2d ptLine;
      ptLine = pLine2d->getStartPoint();
      arrCurPts.push_back(OdGePoint3d(ptLine.x, ptLine.y, 0.0));
      ptLine = pLine2d->getEndPoint();
      arrCurPts.push_back(OdGePoint3d(ptLine.x, ptLine.y, 0.0));
    }
    else if (arrDgnCurves[i]->isKindOf(OdDgLine3d::desc()))
    {
      OdDgLine3dPtr pLine3d = arrDgnCurves[i];
      arrCurPts.push_back(pLine3d->getStartPoint());
      arrCurPts.push_back(pLine3d->getEndPoint());
    }
    else if (arrDgnCurves[i]->isKindOf(OdDgLineString2d::desc()))
    {
      OdDgLineString2dPtr pLineString2d = arrDgnCurves[i];

      for (OdUInt32 j = 0; j < pLineString2d->getVerticesCount(); j++)
      {
        OdGePoint2d ptVertex2d = pLineString2d->getVertexAt(j);
        arrCurPts.push_back(OdGePoint3d(ptVertex2d.x, ptVertex2d.y, 0.0));
      }
    }
    else if (arrDgnCurves[i]->isKindOf(OdDgLineString3d::desc()))
    {
      OdDgLineString3dPtr pLineString3d = arrDgnCurves[i];

      for (OdUInt32 j = 0; j < pLineString3d->getVerticesCount(); j++)
        arrCurPts.push_back(pLineString3d->getVertexAt(j));
    }
    else if (arrDgnCurves[i]->isKindOf(OdDgComplexCurve::desc()) )
    {
      OdArray<OdDgGraphicsElementPtr> pGrArr;
      OdDgComplexCurvePtr pCplxCurve = arrDgnCurves[i];
      OdDgElementIteratorPtr pCplxIter = pCplxCurve->createIterator();

      for(; !pCplxIter->done(); pCplxIter->step())
      {
        OdDgGraphicsElementPtr pGrItem = pCplxIter->item().openObject(OdDg::kForRead);
        pGrArr.push_back(pGrItem);
      }

      arrCurPts = getShapePtsFromSetOfDgnCurves(pGrArr);
    }

    if (arrCurPts.size() < 2)
      continue;

    if (retVal.isEmpty())
      retVal = arrCurPts;
    else
    {
      if (retVal.last().isEqualTo(arrCurPts.first()))
        retVal.removeLast();

      retVal.append(arrCurPts);
    }
  }

  return retVal;
}

//==============================================================================================================

OdDgDimTextFormat::Accuracy getExponentalAccur(double dRoundValue)
{
  OdDgDimTextFormat::Accuracy retVal = OdDgDimTextFormat::kExponential8;

  if( dRoundValue < 0.0000005 )
    retVal = OdDgDimTextFormat::kExponential7;
  else if (dRoundValue < 0.000005)
    retVal = OdDgDimTextFormat::kExponential6;
  else if (dRoundValue < 0.00005)
    retVal = OdDgDimTextFormat::kExponential5;
  else if (dRoundValue < 0.0005)
    retVal = OdDgDimTextFormat::kExponential4;
  else if (dRoundValue < 0.005)
    retVal = OdDgDimTextFormat::kExponential3;
  else if (dRoundValue < 0.05)
    retVal = OdDgDimTextFormat::kExponential2;
  else if (dRoundValue < 0.5)
    retVal = OdDgDimTextFormat::kExponential1;

  return retVal;
}

//==============================================================================================================

OdDgDimTextFormat::Accuracy getDecimalAccur(double dRoundValue)
{
  OdDgDimTextFormat::Accuracy retVal = OdDgDimTextFormat::kDecimal8;

  if (dRoundValue < 0.0000005)
    retVal = OdDgDimTextFormat::kDecimal7;
  else if (dRoundValue < 0.000005)
    retVal = OdDgDimTextFormat::kDecimal6;
  else if (dRoundValue < 0.00005)
    retVal = OdDgDimTextFormat::kDecimal5;
  else if (dRoundValue < 0.0005)
    retVal = OdDgDimTextFormat::kDecimal4;
  else if (dRoundValue < 0.005)
    retVal = OdDgDimTextFormat::kDecimal3;
  else if (dRoundValue < 0.05)
    retVal = OdDgDimTextFormat::kDecimal2;
  else if (dRoundValue < 0.5)
    retVal = OdDgDimTextFormat::kDecimal1;

  return retVal;
}

//==============================================================================================================

OdDgDimTextFormat::Accuracy convertAccuracyToDgnFormat(OdUInt8  uDimUnitFormat, 
                                                         OdUInt8  uDimPresission,
                                                           double dRoundValue)
{
  OdDgDimTextFormat::Accuracy retVal = OdDgDimTextFormat::kAccuracyNone;

  switch( uDimUnitFormat )
  {
    case 1:
    {
      switch (uDimPresission)
      {
        case 0: retVal = OdDgDimTextFormat::kAccuracyNone; break;
        case 1: retVal = OdDgDimTextFormat::kExponential1; break;
        case 2: retVal = OdDgDimTextFormat::kExponential2; break;
        case 3: retVal = OdDgDimTextFormat::kExponential3; break;
        case 4: retVal = OdDgDimTextFormat::kExponential4; break;
        case 5: retVal = OdDgDimTextFormat::kExponential5; break;
        case 6: retVal = OdDgDimTextFormat::kExponential6; break;
        case 7: retVal = OdDgDimTextFormat::kExponential7; break;
        case 8: retVal = OdDgDimTextFormat::kExponential8; break;
      }

      OdDgDimTextFormat::Accuracy roundAccur = getExponentalAccur(dRoundValue);

      if( roundAccur < retVal )
        retVal = roundAccur;

    } break;
    
    case 4:
    case 5:
    case 7:
    {
      switch (uDimPresission)
      {
        case 0: retVal = OdDgDimTextFormat::kAccuracyNone; break;
        case 1: retVal = OdDgDimTextFormat::kFractional2; break;
        case 2: retVal = OdDgDimTextFormat::kFractional4; break;
        case 3: retVal = OdDgDimTextFormat::kFractional8; break;
        case 4: retVal = OdDgDimTextFormat::kFractional16; break;
        case 5: retVal = OdDgDimTextFormat::kFractional32; break;
        case 6: retVal = OdDgDimTextFormat::kFractional64; break;
        case 7: retVal = OdDgDimTextFormat::kDecimal7; break;
        case 8: retVal = OdDgDimTextFormat::kDecimal8; break;
      }

      if( retVal >= OdDgDimTextFormat::kDecimal7 )
      {
        OdDgDimTextFormat::Accuracy roundAccur = getDecimalAccur(dRoundValue);

        if( (roundAccur < retVal) || (roundAccur == OdDgDimTextFormat::kAccuracyNone)|| (retVal == OdDgDimTextFormat::kDecimal8))
          retVal = roundAccur;
      }

    } break;

    default:
    {
      switch (uDimPresission)
      {
        case 0: retVal = OdDgDimTextFormat::kAccuracyNone; break;
        case 1: retVal = OdDgDimTextFormat::kDecimal1; break;
        case 2: retVal = OdDgDimTextFormat::kDecimal2; break;
        case 3: retVal = OdDgDimTextFormat::kDecimal3; break;
        case 4: retVal = OdDgDimTextFormat::kDecimal4; break;
        case 5: retVal = OdDgDimTextFormat::kDecimal5; break;
        case 6: retVal = OdDgDimTextFormat::kDecimal6; break;
        case 7: retVal = OdDgDimTextFormat::kDecimal7; break;
        case 8: retVal = OdDgDimTextFormat::kDecimal8; break;
      }

      OdDgDimTextFormat::Accuracy roundAccur = getDecimalAccur(dRoundValue);

      if( (roundAccur < retVal) || (roundAccur == OdDgDimTextFormat::kAccuracyNone) || (retVal == OdDgDimTextFormat::kDecimal8) )
        retVal = roundAccur;

    } break;
  }

  return retVal;
}

//==============================================================================================================

OdDg::TextJustification getDgTextJustification(OdDb::TextHorzMode horMode, OdDb::TextVertMode verMode)
{
  OdDg::TextJustification uRet = OdDg::kLeftTop;

  switch (horMode)
  {
    case OdDb::kTextLeft:
    {
      switch (verMode)
      {
        case OdDb::kTextBase: uRet = OdDg::kLeftBottom; break;
        case OdDb::kTextBottom: uRet = OdDg::kLeftDescender; break;
        case OdDb::kTextVertMid: uRet = OdDg::kLeftCenter; break;
        case OdDb::kTextTop: uRet = OdDg::kLeftTop; break;
      }
    } break;

    case OdDb::kTextCenter:
    {
      switch (verMode)
      {
        case OdDb::kTextBase: uRet = OdDg::kCenterBottom; break;
        case OdDb::kTextBottom: uRet = OdDg::kCenterDescender; break;
        case OdDb::kTextVertMid: uRet = OdDg::kCenterCenter; break;
        case OdDb::kTextTop: uRet = OdDg::kCenterTop; break;
      }
    } break;

    case OdDb::kTextRight:
    {
      switch (verMode)
      {
        case OdDb::kTextBase: uRet = OdDg::kRightBottom; break;
        case OdDb::kTextBottom: uRet = OdDg::kRightDescender; break;
        case OdDb::kTextVertMid: uRet = OdDg::kRightCenter; break;
        case OdDb::kTextTop: uRet = OdDg::kRightTop; break;
      }
    } break;

    case OdDb::kTextMid:
    {
      uRet = OdDg::kCenterCenter;
    } break;

    case OdDb::kTextAlign:
    case OdDb::kTextFit:
    {
      uRet = OdDg::kLeftBottom;
    } break;
  }

  return uRet;
}

//==============================================================================================================

OdDgElementId getOwnerModelId(const OdDgElement* pElm)
{
  OdDgElementId idRet;

  OdDgElementId idOwner = pElm->elementId();

  while (!idOwner.isNull())
  {
    OdDgElementPtr pOwner = idOwner.openObject(OdDg::kForRead);

    if (pOwner->isKindOf(OdDgModel::desc()))
    {
      idRet = pOwner->elementId();
      break;
    }

    idOwner = pOwner->ownerId();
  }

  return idRet;
}

//==============================================================================================================

}
