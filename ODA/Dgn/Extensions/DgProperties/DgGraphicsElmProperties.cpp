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

#include "DgGraphicsElmProperties.h"
#include "DgCmColor.h"
#include "DgLevelTableRecord.h"
#include "DgTable.h"
#include "DgDatabase.h"
#include "DgDisplayStyle.h"

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: ColorIndex
//-----------------------------------------------------------------------------------------------------------------------
/*
OdResult OdDgGraphicsElementColorIndexProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgGraphicsElementPtr pObj = OdDgGraphicsElement::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  value = pObj->getColorIndex();

  return eOk;
}
*/
//-----------------------------------------------------------------------------------------------------------------------
/*
OdResult OdDgGraphicsElementColorIndexProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgGraphicsElementPtr pObj = OdDgGraphicsElement::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  pObj->setColorIndex(*rxvalue_cast<OdUInt32>(&value));

  return eOk;
}
*/
//-----------------------------------------------------------------------------------------------------------------------
//                                          property: Color
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgGraphicsElementColorProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgGraphicsElementPtr pObj = OdDgGraphicsElement::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  OdCmEntityColor cmColor;

  OdUInt32 uIndex = pObj->getColorIndex();

  if( uIndex == OdDg::kColorByLevel )
    cmColor.setColorMethod( OdCmEntityColor::kByLayer );
  else if( uIndex == OdDg::kColorByCell )
    cmColor.setColorMethod( OdCmEntityColor::kByBlock );
  else if( uIndex < 256 )
  {
    OdCmEntityColor::setDgnColorIndex(&uIndex, uIndex);
    cmColor.setColor( uIndex );
  }
  else
  {
    ODCOLORREF rgbColor = pObj->getColor();
    cmColor.setColorMethod( OdCmEntityColor::kByColor );
    cmColor.setRed( ODGETRED(rgbColor) );
    cmColor.setGreen( ODGETGREEN(rgbColor) );
    cmColor.setBlue( ODGETBLUE(rgbColor) );
  }

  value = cmColor;

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgGraphicsElementColorProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgGraphicsElementPtr pObj = OdDgGraphicsElement::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  const OdCmEntityColor* pColor = rxvalue_cast<OdCmEntityColor>(&value);

  switch( pColor->colorMethod() )
  {
    case OdCmEntityColor::kByLayer:
      pObj->setColorIndex( OdDg::kColorByLevel ); break;
    case OdCmEntityColor::kByBlock:
      pObj->setColorIndex( OdDg::kColorByCell ); break;
    case OdCmEntityColor::kByACI:
      pObj->setColorIndex( pColor->colorIndex() ); break; //???
    case OdCmEntityColor::kByDgnIndex:
      pObj->setColorIndex( pColor->colorIndex() ); break;
    case OdCmEntityColor::kByPen:
      pObj->setColorIndex( pColor->colorIndex() ); break; //???
    case OdCmEntityColor::kByColor:
    {
      ODCOLORREF curColor = ODRGB( pColor->red(), pColor->green(), pColor->blue());
      pObj->setColor(curColor);
    }
  }

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: GraphicsGroupIndex
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgGraphicsElementGraphicsGroupIndexProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgGraphicsElementPtr pObj = OdDgGraphicsElement::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  value = pObj->getGraphicsGroupEntryId();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgGraphicsElementGraphicsGroupIndexProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgGraphicsElementPtr pObj = OdDgGraphicsElement::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  pObj->setGraphicsGroupEntryId(*rxvalue_cast<OdUInt32>(&value));

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: Class
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgGraphicsElementClassProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgGraphicsElementPtr pObj = OdDgGraphicsElement::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  value = pObj->getClass();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgGraphicsElementClassProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgGraphicsElementPtr pObj = OdDgGraphicsElement::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  pObj->setClass(*rxvalue_cast<OdDgGraphicsElement::Class>(&value));

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: LevelIndex
//-----------------------------------------------------------------------------------------------------------------------
/*
OdResult OdDgGraphicsElementLevelIndexProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgGraphicsElementPtr pObj = OdDgGraphicsElement::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  value = pObj->getLevelEntryId();

  return eOk;
}
*/
//-----------------------------------------------------------------------------------------------------------------------
/*
OdResult OdDgGraphicsElementLevelIndexProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgGraphicsElementPtr pObj = OdDgGraphicsElement::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  pObj->setLevelEntryId(*rxvalue_cast<OdUInt32>(&value));
 
  return eOk;
}
*/
//-----------------------------------------------------------------------------------------------------------------------
//                                          property: Level
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgGraphicsElementLevelProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgGraphicsElementPtr pObj = OdDgGraphicsElement::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  value = pObj->getLevelId();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgGraphicsElementLevelProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgGraphicsElementPtr pObj = OdDgGraphicsElement::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  pObj->setLevelId(*rxvalue_cast<OdDgElementId>(&value));
 
  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: LevelName
//-----------------------------------------------------------------------------------------------------------------------
/*
OdResult OdDgGraphicsElementLevelNameProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgGraphicsElementPtr pObj = OdDgGraphicsElement::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  OdString      strRet  = OdString::kEmpty;
  OdDgElementId idLevel = pObj->getLevelId();

  if( !idLevel.isNull() )
  {
    OdDgLevelTableRecordPtr pLevel = idLevel.openObject(OdDg::kForRead);
    strRet = pLevel->getName();
  }

  value = strRet;

  return eOk;
}
*/
//-----------------------------------------------------------------------------------------------------------------------
/*
OdResult OdDgGraphicsElementLevelNameProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgGraphicsElementPtr pObj = OdDgGraphicsElement::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  OdString strLevelName = *rxvalue_cast<OdString>(&value);

  OdDgDatabase* pDb = pObj->database();

  if( !pDb )
    return eNotApplicable;

  OdDgLevelTablePtr pLevelTable = pDb->getLevelTable(OdDg::kForRead);

  OdDgElementId idLevel = pLevelTable->getAt(strLevelName);

  if( !idLevel.isNull() )
    pObj->setLevelId(idLevel);
  else
    return eNotApplicable;
 
  return eOk;
}
*/
//-----------------------------------------------------------------------------------------------------------------------
//                                          property: LineStyleIndex
//-----------------------------------------------------------------------------------------------------------------------
/*
OdResult OdDgGraphicsElementLineStyleIndexProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgGraphicsElementPtr pObj = OdDgGraphicsElement::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  value = pObj->getLineStyleEntryId();

  return eOk;
}
*/
//-----------------------------------------------------------------------------------------------------------------------
/*
OdResult OdDgGraphicsElementLineStyleIndexProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgGraphicsElementPtr pObj = OdDgGraphicsElement::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  pObj->setLineStyleEntryId(*rxvalue_cast<OdUInt32>(&value));
 
  return eOk;
}
*/
//-----------------------------------------------------------------------------------------------------------------------
//                                          property: LineStyle
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgGraphicsElementLineStyleProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgGraphicsElementPtr pObj = OdDgGraphicsElement::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  value = pObj->getLineStyleId();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgGraphicsElementLineStyleProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgGraphicsElementPtr pObj = OdDgGraphicsElement::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  pObj->setLineStyleId(*rxvalue_cast<OdDgElementId>(&value));
 
  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: LineStyleName
//-----------------------------------------------------------------------------------------------------------------------
/*
OdResult OdDgGraphicsElementLineStyleNameProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgGraphicsElementPtr pObj = OdDgGraphicsElement::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  OdString      strRet  = OdString::kEmpty;
  OdDgElementId idLineStyle = pObj->getLineStyleId();

  if( !idLineStyle.isNull() )
  {
    OdDgLineStyleTableRecordPtr pLineStyle = idLineStyle.openObject(OdDg::kForRead);
    strRet = pLineStyle->getName();
  }

  value = strRet;

  return eOk;
}
*/
//-----------------------------------------------------------------------------------------------------------------------
/*
OdResult OdDgGraphicsElementLineStyleNameProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgGraphicsElementPtr pObj = OdDgGraphicsElement::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  OdString strLineStyleName = *rxvalue_cast<OdString>(&value);

  OdDgDatabase* pDb = pObj->database();

  if( !pDb )
    return eNotApplicable;

  OdDgLineStyleTablePtr pLineStyleTable = pDb->getLineStyleTable(OdDg::kForRead);

  OdDgElementId idLineStyle = pLineStyleTable->getAt(strLineStyleName);

  if( !idLineStyle.isNull() )
    pObj->setLineStyleId(idLineStyle);
  else
    return eNotApplicable;
 
  return eOk;
}
*/
//-----------------------------------------------------------------------------------------------------------------------
//                                          property: LineWeight
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgGraphicsElementLineWeightProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgGraphicsElementPtr pObj = OdDgGraphicsElement::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  value = (OdDb::LineWeight)(pObj->getLineWeight());

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgGraphicsElementLineWeightProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgGraphicsElementPtr pObj = OdDgGraphicsElement::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  pObj->setLineWeight((OdUInt32)(*rxvalue_cast<OdDb::LineWeight>(&value)));
 
  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: Thickness
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgGraphicsElementThicknessProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgGraphicsElementPtr pObj = OdDgGraphicsElement::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  value = pObj->getThickness();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgGraphicsElementThicknessProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgGraphicsElementPtr pObj = OdDgGraphicsElement::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  pObj->setThickness(*rxvalue_cast<double>(&value));
 
  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: Priority
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgGraphicsElementPriorityProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgGraphicsElementPtr pObj = OdDgGraphicsElement::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  value = pObj->getPriority();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgGraphicsElementPriorityProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgGraphicsElementPtr pObj = OdDgGraphicsElement::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  pObj->setPriority(*rxvalue_cast<OdUInt32>(&value));
 
  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: Invisible
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgGraphicsElementInvisibleProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgGraphicsElementPtr pObj = OdDgGraphicsElement::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  value = pObj->getInvisibleFlag();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgGraphicsElementInvisibleProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgGraphicsElementPtr pObj = OdDgGraphicsElement::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  pObj->setInvisibleFlag(*rxvalue_cast<bool>(&value));
 
  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: 3D
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgGraphicsElement3DProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgGraphicsElementPtr pObj = OdDgGraphicsElement::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  value = pObj->get3dFormatFlag();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: ViewIndependent
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgGraphicsElementViewIndependentProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgGraphicsElementPtr pObj = OdDgGraphicsElement::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  value = pObj->getViewIndependentFlag();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgGraphicsElementViewIndependentProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgGraphicsElementPtr pObj = OdDgGraphicsElement::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  pObj->setViewIndependentFlag(*rxvalue_cast<bool>(&value));
 
  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: NonPlanar
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgGraphicsElementNonPlanarProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgGraphicsElementPtr pObj = OdDgGraphicsElement::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  value = pObj->getNonPlanarFlag();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: NotSnappable
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgGraphicsElementNotSnappableProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgGraphicsElementPtr pObj = OdDgGraphicsElement::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  value = pObj->getNotSnappableFlag();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgGraphicsElementNotSnappableProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgGraphicsElementPtr pObj = OdDgGraphicsElement::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  pObj->setNotSnappableFlag(*rxvalue_cast<bool>(&value));
 
  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: HBit
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgGraphicsElementHBitProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgGraphicsElementPtr pObj = OdDgGraphicsElement::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  value = pObj->getHbitFlag();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgGraphicsElementHBitProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgGraphicsElementPtr pObj = OdDgGraphicsElement::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  pObj->setHbitFlag(*rxvalue_cast<bool>(&value));
 
  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: Transparency
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgGraphicsElementTransparencyProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgGraphicsElementPtr pObj = OdDgGraphicsElement::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  value = pObj->getTransparency();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgGraphicsElementTransparencyProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgGraphicsElementPtr pObj = OdDgGraphicsElement::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  pObj->setTransparency(*rxvalue_cast<OdCmTransparency>(&value));
 
  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: Extents
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgGraphicsElementExtentsProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgGraphicsElementPtr pObj = OdDgGraphicsElement::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  OdGeExtents3d extData;
  pObj->getGeomExtents(extData);
  value = extData;

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: DisplayStyle
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgGraphicsElementDisplayStyleProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgGraphicsElementPtr pObj = OdDgGraphicsElement::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  OdDgDisplayStyleGraphicsElementPEPtr pElementPE = OdDgDisplayStyleGraphicsElementPEPtr(OdRxObjectPtr(pObj));
  OdDgElementId idDisplayStyle;

  if( !pElementPE.isNull() )
  {
    OdDgDisplayStyleTableRecordPtr pDisplayStyle = pElementPE->getDispalyStyle(pObj);

    if( !pDisplayStyle.isNull() )
      idDisplayStyle = pDisplayStyle->elementId();
  }

  value = idDisplayStyle;

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgGraphicsElementDisplayStyleProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgGraphicsElementPtr pObj = OdDgGraphicsElement::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  OdDgElementId idDisplayStyle = *rxvalue_cast<OdDgElementId>(&value);
  OdDgDisplayStyleGraphicsElementPEPtr pElementPE = OdDgDisplayStyleGraphicsElementPEPtr(OdRxObjectPtr(pObj));

  if( idDisplayStyle.isNull() )
  {
    pElementPE->setDispalyStyle(pObj, OdDgDisplayStyleTableRecordPtr() );
  }
  else
  {
    OdDgElementPtr pElm = idDisplayStyle.openObject(OdDg::kForRead);

    if( !pElm.isNull() && pElm->isKindOf(OdDgDisplayStyleTableRecord::desc()) )
    {
      OdDgDisplayStyleTableRecordPtr pDisplayStyle = pElm;
      pElementPE->setDispalyStyle(pObj, pDisplayStyle);
    }
  }
 
  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: DisplayStyleName
//-----------------------------------------------------------------------------------------------------------------------
/*
OdResult OdDgGraphicsElementDisplayStyleNameProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgGraphicsElementPtr pObj = OdDgGraphicsElement::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  OdString      strRet  = OdString::kEmpty;

  OdDgDisplayStyleGraphicsElementPEPtr pElementPE = OdDgDisplayStyleGraphicsElementPEPtr(OdRxObjectPtr(pObj));
  OdDgElementId idDisplayStyle;

  if( !pElementPE.isNull() )
  {
    OdDgDisplayStyleTableRecordPtr pDisplayStyle = pElementPE->getDispalyStyle(pObj);

    if( !pDisplayStyle.isNull() )
      strRet = pDisplayStyle->getName();
  }

  value = strRet;

  return eOk;
}
*/
//-----------------------------------------------------------------------------------------------------------------------
/*
OdResult OdDgGraphicsElementDisplayStyleNameProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgGraphicsElementPtr pObj = OdDgGraphicsElement::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  OdString strDisplayStyleName = *rxvalue_cast<OdString>(&value);

  OdDgDatabase* pDb = pObj->database();

  if( !pDb )
    return eNotApplicable;

  OdDgDisplayStyleGraphicsElementPEPtr pElementPE = OdDgDisplayStyleGraphicsElementPEPtr(OdRxObjectPtr(pObj));
  OdDgDisplayStyleTablePtr pDisplayStyleTable = pDb->getDisplayStyleTable(OdDg::kForRead);

  if( strDisplayStyleName.isEmpty() )
  {
    pElementPE->setDispalyStyle(pObj, OdDgDisplayStyleTableRecordPtr() );
    return eOk;
  }

  OdDgElementId idDisplayStyle = pDisplayStyleTable->getAt(strDisplayStyleName);

  if( idDisplayStyle.isNull() )
    return eNotApplicable;

  OdDgElementPtr pElm = idDisplayStyle.openObject(OdDg::kForRead);

  if( !pElm.isNull() && pElm->isKindOf(OdDgDisplayStyleTableRecord::desc()) )
  {
    OdDgDisplayStyleTableRecordPtr pDisplayStyle = pElm;
    pElementPE->setDispalyStyle(pObj, pDisplayStyle);
  }
 
  return eOk;
}
*/
//-----------------------------------------------------------------------------------------------------------------------
//                                          property: ItemTypeInstances
//-----------------------------------------------------------------------------------------------------------------------

ODDG_DEFINE_RX_VALUE_ARRAY_ITERATOR(OdDgECPropertyInstanceFullPath);

OdRxValueIteratorPtr OdDgGraphicsElementItemTypeInstancesProperty::subNewValueIterator(const OdRxObject* pO) const
{
	OdRxValueIteratorPtr res;

	OdDgGraphicsElementPtr pObj = OdDgGraphicsElement::cast(pO);

	if (pObj.isNull())
		return res;

  OdDgECClassInstanceElementPEPtr pItemTypePE = OdDgECClassInstanceElementPE::cast(pObj);

	if (pItemTypePE.isNull())
		return res;

	res = OdRxObjectImpl<OdRxOdDgECPropertyInstanceFullPathValueIterator>::createObject();

	if (!pObj.isNull())
	{
		OdArray<OdDgECPropertyInstanceFullPath> arrItemTypes;

		for (OdUInt32 i = 0; i < pItemTypePE->getECClassInstanceCount(pObj); i++)
		{
			OdDgECClassInstancePtr pCurInstance = pItemTypePE->getECClassInstance(pObj, i);

      if( pCurInstance.isNull() )
        continue;

			for (OdUInt32 j = 0; j < pCurInstance->getPropertyInstanceCount(); j++)
			{
				OdDgECPropertyInstance curProperty = pCurInstance->getPropertyInstance(j);

				if( (curProperty.getType() != OdDgECPropertyInstance::kECClassInstance) &&
					  (curProperty.getType() != OdDgECPropertyInstance::kECClassInstanceArray)
					)
				{ 
          OdDgECPropertyInstanceFullPath tmpData;
					tmpData.setECSchemaName(pCurInstance->getECClassReference().getSchemaName());
					tmpData.setECClassName(pCurInstance->getECClassReference().getClassName());
					tmpData.setPropertyInstance(curProperty);
					arrItemTypes.push_back(tmpData);
				}
			}
		}
		((OdRxOdDgECPropertyInstanceFullPathValueIterator*)res.get())->init(arrItemTypes, 0);
	}

	return res;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgGraphicsElementItemTypeInstancesProperty::subTryGetCount(const OdRxObject* pO, int& count) const
{
	OdDgGraphicsElementPtr pObj = OdDgGraphicsElement::cast(pO);

	if (pObj.isNull())
		return eNotApplicable;

  OdDgECClassInstanceElementPEPtr pItemTypePE = OdDgECClassInstanceElementPE::cast(pObj);

	if (pItemTypePE.isNull())
		return eNotApplicable;

	count = 0;

  OdArray<OdDgECPropertyInstanceFullPath> arrItemTypes;

	for (OdUInt32 i = 0; i < pItemTypePE->getECClassInstanceCount(pObj); i++)
	{
		OdDgECClassInstancePtr pCurInstance = pItemTypePE->getECClassInstance(pObj, i);

    if( pCurInstance.isNull() )
      continue;

		for (OdUInt32 j = 0; j < pCurInstance->getPropertyInstanceCount(); j++)
		{
			OdDgECPropertyInstance curProperty = pCurInstance->getPropertyInstance(j);

			if ((curProperty.getType() != OdDgECPropertyInstance::kECClassInstance) &&
				  (curProperty.getType() != OdDgECPropertyInstance::kECClassInstanceArray)
				)
			{
				count++;
			}
		}
	}

	return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgGraphicsElementItemTypeInstancesProperty::subGetValue(const OdRxObject* pO, int index, OdRxValue& value) const
{
	OdDgGraphicsElementPtr pObj = OdDgGraphicsElement::cast(pO);

	if (pObj.isNull())
		return eNotApplicable;

  OdDgECClassInstanceElementPEPtr pItemTypePE = OdDgECClassInstanceElementPE::cast(pObj);

	if (pItemTypePE.isNull())
		return eNotApplicable;

	if( index < 0 )
		return eInvalidIndex;

	int			 iCurCount = 0;
	bool		 bSetValue = false;

	for (OdUInt32 i = 0; i < pItemTypePE->getECClassInstanceCount(pObj); i++)
	{
    OdDgECClassInstancePtr pCurInstance = pItemTypePE->getECClassInstance(pObj, i);

    if( pCurInstance.isNull() )
      continue;

		for (OdUInt32 j = 0; j < pCurInstance->getPropertyInstanceCount(); j++)
		{
			OdDgECPropertyInstance curProperty = pCurInstance->getPropertyInstance(j);

			if((curProperty.getType() != OdDgECPropertyInstance::kECClassInstance) &&
				 (curProperty.getType() != OdDgECPropertyInstance::kECClassInstanceArray)
				)
			{
				if (iCurCount == index)
				{
          OdDgECPropertyInstanceFullPath tmpData;
					tmpData.setECSchemaName(pCurInstance->getECClassReference().getSchemaName());
					tmpData.setECClassName(pCurInstance->getECClassReference().getClassName());
					tmpData.setPropertyInstance(curProperty);
					value = tmpData;
					bSetValue = true;
					break;
				}
				iCurCount++;
			}
		}

		if (bSetValue)
			break;
	}

	return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgGraphicsElementItemTypeInstancesProperty::subChildren(OdArray<OdRxMemberPtr>& children) const
{
	// as far as I understand, subproperties are generated if value type has children and OdRxTypePromotionAttribute
	// if that is true, generic version of this code may be moved to OdRxProperty::subChildren or even straight to children()
	children.append(OdRxSubPropertyOfIndexedProperty::createObject(L"ECSchemaName", OdRxValueType::Desc<OdString>::value(), this));
	children.append(OdRxSubPropertyOfIndexedProperty::createObject(L"ECClassName", OdRxValueType::Desc<OdString>::value(), this));
	children.append(OdRxSubPropertyOfIndexedProperty::createObject(L"ECPropertyName", OdRxValueType::Desc<OdString>::value(), this));
	children.append(OdRxSubPropertyOfIndexedProperty::createObject(L"Value", OdRxValueType::Desc<void>::value(), this));
	return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgGraphicsElementItemTypeInstancesProperty::subSetValue(OdRxObject* pO, int index, const OdRxValue& value) const
{
	OdDgGraphicsElementPtr pObj = OdDgGraphicsElement::cast(pO);

	if (pObj.isNull())
		return eNotApplicable;

  OdDgECClassInstanceElementPEPtr pItemTypePE = OdDgECClassInstanceElementPE::cast(pObj);

	if (pItemTypePE.isNull())
		return eNotApplicable;

	const OdDgECPropertyInstanceFullPath* pValue = rxvalue_cast<OdDgECPropertyInstanceFullPath>(&value);

	if(pValue)
	{
		int			 iCurCount = 0;
		bool		 bSetValue = false;

		for (OdUInt32 i = 0; i < pItemTypePE->getECClassInstanceCount(pObj); i++)
		{
      OdDgECClassInstancePtr pCurInstance = pItemTypePE->getECClassInstance(pObj, i);

      if( pCurInstance.isNull() )
        continue;

			for (OdUInt32 j = 0; j < pCurInstance->getPropertyInstanceCount(); j++)
			{
        OdDgECPropertyInstance curProperty = pCurInstance->getPropertyInstance(j);

        if((curProperty.getType() != OdDgECPropertyInstance::kECClassInstance) &&
           (curProperty.getType() != OdDgECPropertyInstance::kECClassInstanceArray)
          )
        {
					if (iCurCount == index)
					{
            pCurInstance->setPropertyInstance(j, pValue->getPropertyInstance());
						pItemTypePE->setECClassInstance(pObj, i, pCurInstance);
						bSetValue = true;
						break;
					}
					iCurCount++;
				}
			}

			if (bSetValue)
				break;
		}

		if (bSetValue)
			return eOk;
		else
			return eInvalidIndex;
	}

	return eNotThatKindOfClass;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgGraphicsElementItemTypeInstancesProperty::subInsertValue(OdRxObject* pO, int index, const OdRxValue& value) const
{
	OdDgGraphicsElementPtr pObj = OdDgGraphicsElement::cast(pO);

	if (pObj.isNull())
		return eNotApplicable;

  OdDgECClassInstanceElementPEPtr pItemTypePE = OdDgECClassInstanceElementPE::cast(pObj);

	if (pItemTypePE.isNull())
		return eNotApplicable;

	const OdDgECPropertyInstanceFullPath* pValue = rxvalue_cast<OdDgECPropertyInstanceFullPath>(&value);

	if (pValue)
	{
		bool bAddItemTypeInstance = true;
		bool bAddPropertyInstance = true;

		for (OdUInt32 i = 0; i < pItemTypePE->getECClassInstanceCount(pObj); i++)
		{
      OdDgECClassInstancePtr pCurInstance = pItemTypePE->getECClassInstance(pObj, i);

      if(pCurInstance.isNull() )
        continue;

			if ((pCurInstance->getECClassReference().getSchemaName() == pValue->getECSchemaName()) &&
					(pCurInstance->getECClassReference().getClassName() == pValue->getECClassName())
				)
			{
				for (OdUInt32 j = 0; j < pCurInstance->getPropertyInstanceCount(); j++)
				{
					OdDgECPropertyInstance curProperty = pCurInstance->getPropertyInstance(j);

					if( curProperty.getName() == pValue->getPropertyInstance().getName() )
					{
            pCurInstance->setPropertyInstance(j, pValue->getPropertyInstance());
						pItemTypePE->setECClassInstance(pObj, i, pCurInstance);
						bAddPropertyInstance = false;
					}
				}

				if (bAddPropertyInstance)
				{
          pCurInstance->addPropertyInstance(pValue->getPropertyInstance());
					pItemTypePE->setECClassInstance(pObj, i, pCurInstance);
					bAddPropertyInstance = false;
				}

				bAddItemTypeInstance = false;
				break;
			}
		}

		if (bAddItemTypeInstance)
		{
      OdDgECSchemaReference schemaRef = OdDgECSchema::getECSchemaReferenceByName(pValue->getECSchemaName(), pObj->database());
      OdDgECClassReference  classRef( pValue->getECClassName(), schemaRef);
      OdDgECClassInstancePtr pNewInstance = OdDgECClassInstance::createObject(classRef, pObj->database(), false);
      pNewInstance->addPropertyInstance(pValue->getPropertyInstance());
			pItemTypePE->addECClassInstance(pObj, pNewInstance);
		}

		return eOk;
	}

	return eNotThatKindOfClass;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgGraphicsElementItemTypeInstancesProperty::subRemoveValue(OdRxObject* pO, int index) const
{
	OdDgGraphicsElementPtr pObj = OdDgGraphicsElement::cast(pO);

	if (pObj.isNull())
		return eNotApplicable;

  OdDgECClassInstanceElementPEPtr pItemTypePE = OdDgECClassInstanceElementPE::cast(pObj);

	if (pItemTypePE.isNull())
		return eNotApplicable;

	int			 iCurCount = 0;
	bool		 bSetValue = false;

	for (OdUInt32 i = 0; i < pItemTypePE->getECClassInstanceCount(pObj); i++)
	{
    OdDgECClassInstancePtr pCurInstance = pItemTypePE->getECClassInstance(pObj, i);

    if( pCurInstance.isNull() )
      continue;

		for (OdUInt32 j = 0; j < pCurInstance->getPropertyInstanceCount(); j++)
		{
			OdDgECPropertyInstance curProperty = pCurInstance->getPropertyInstance(j);

			if ((curProperty.getType() != OdDgECPropertyInstance::kECClassInstance) &&
					(curProperty.getType() != OdDgECPropertyInstance::kECClassInstanceArray)
				)
			{
				if (iCurCount == index)
				{
          pCurInstance->deletePropertyInstance(j);

					if (pCurInstance->getPropertyInstanceCount() == 0)
						pItemTypePE->deleteECClassInstance(pObj, i);
					else
						pItemTypePE->setECClassInstance(pObj, i, pCurInstance);

					break;
				}
				iCurCount++;
			}
		}

		if (bSetValue)
			break;
	}

	if (bSetValue)
		return eOk;
	else
		return eInvalidIndex;
}

//-----------------------------------------------------------------------------------------------------------------------



