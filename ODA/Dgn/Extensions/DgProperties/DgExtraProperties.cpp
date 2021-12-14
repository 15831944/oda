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

#include "DgExtraProperties.h"
#include "DgTable.h"
#include "DgSharedCellDefinition.h"

//-----------------------------------------------------------------------------------------------------------------------
//                                          class: OdDgHatchPattern
//-----------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------
//                                          property: LineStyleScale
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgHatchPatternPatternTypeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgHatchPatternPtr pObj = OdDgHatchPattern::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  value = pObj->getType();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: Tolerance
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgHatchPatternToleranceProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgHatchPatternPtr pObj = OdDgHatchPattern::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  value = pObj->getTolerance();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgHatchPatternToleranceProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgHatchPatternPtr pObj = OdDgHatchPattern::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  const double* pValue = rxvalue_cast<double>(&value);

  if( pValue )
  {
    pObj->setTolerance(*pValue);
    return eOk;
  }

  return eNotThatKindOfClass;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: UseTolerance
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgHatchPatternUseToleranceProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgHatchPatternPtr pObj = OdDgHatchPattern::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  value = pObj->getUseToleranceFlag();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgHatchPatternUseToleranceProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgHatchPatternPtr pObj = OdDgHatchPattern::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  const bool* pValue = rxvalue_cast<bool>(&value);

  if( pValue )
  {
    pObj->setUseToleranceFlag(*pValue);
    return eOk;
  }

  return eNotThatKindOfClass;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: Rotation
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgHatchPatternRotationProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgHatchPatternPtr pObj = OdDgHatchPattern::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  value = pObj->getRotation();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgHatchPatternRotationProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgHatchPatternPtr pObj = OdDgHatchPattern::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  const OdGeQuaternion* pValue = rxvalue_cast<OdGeQuaternion>(&value);

  if( pValue )
  {
    pObj->setRotation(*pValue);
    return eOk;
  }

  return eNotThatKindOfClass;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: 2dRotation
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgHatchPattern2dRotationProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgHatchPatternPtr pObj = OdDgHatchPattern::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  OdGeMatrix2d matRotation;
  pObj->getRotation2d(matRotation);

  OdGeVector2d vrXAxis = OdGeVector2d::kXAxis;
  vrXAxis.transformBy(matRotation);

  value = OdGeVector2d::kXAxis.angleTo(vrXAxis);

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgHatchPattern2dRotationProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgHatchPatternPtr pObj = OdDgHatchPattern::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  const double* pValue = rxvalue_cast<double>(&value);

  if( pValue )
  {
    pObj->setRotation2d(OdGeMatrix2d::rotation(*pValue));
    return eOk;
  }

  return eNotThatKindOfClass;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: UseRotation
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgHatchPatternUseRotationProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgHatchPatternPtr pObj = OdDgHatchPattern::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  value = pObj->getUseRotationFlag();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgHatchPatternUseRotationProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgHatchPatternPtr pObj = OdDgHatchPattern::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  const bool* pValue = rxvalue_cast<bool>(&value);

  if( pValue )
  {
    pObj->setUseRotationFlag(*pValue);
    return eOk;
  }

  return eNotThatKindOfClass;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: LowMultilineIndex
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgHatchPatternLowMultilineIndexProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgHatchPatternPtr pObj = OdDgHatchPattern::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  value = pObj->getLowMultilineIndex();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgHatchPatternLowMultilineIndexProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgHatchPatternPtr pObj = OdDgHatchPattern::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  const OdUInt32* pValue = rxvalue_cast<OdUInt32>(&value);

  if( pValue )
  {
    pObj->setLowMultilineIndex(*pValue);
    return eOk;
  }

  return eNotThatKindOfClass;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: HighMultilineIndex
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgHatchPatternHighMultilineIndexProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgHatchPatternPtr pObj = OdDgHatchPattern::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  value = pObj->getHighMultilineIndex();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgHatchPatternHighMultilineIndexProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgHatchPatternPtr pObj = OdDgHatchPattern::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  const OdUInt32* pValue = rxvalue_cast<OdUInt32>(&value);

  if( pValue )
  {
    pObj->setHighMultilineIndex(*pValue);
    return eOk;
  }

  return eNotThatKindOfClass;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: UseMultilineIndices
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgHatchPatternUseMultilineIndicesProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgHatchPatternPtr pObj = OdDgHatchPattern::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  value = pObj->getUseMultilineIndicesFlag();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgHatchPatternUseMultilineIndicesProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgHatchPatternPtr pObj = OdDgHatchPattern::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  const bool* pValue = rxvalue_cast<bool>(&value);

  if( pValue )
  {
    pObj->setUseMultilineIndicesFlag(*pValue);
    return eOk;
  }

  return eNotThatKindOfClass;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: Offset
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgHatchPatternOffsetProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgHatchPatternPtr pObj = OdDgHatchPattern::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  OdGePoint3d ptOffset;
  pObj->getOffset(ptOffset);
  value = ptOffset;

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgHatchPatternOffsetProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgHatchPatternPtr pObj = OdDgHatchPattern::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  const OdGePoint3d* pValue = rxvalue_cast<OdGePoint3d>(&value);

  if( pValue )
  {
    pObj->setOffset(*pValue);
    return eOk;
  }

  return eNotThatKindOfClass;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: UseOffset
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgHatchPatternUseOffsetProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgHatchPatternPtr pObj = OdDgHatchPattern::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  value = pObj->getUseOffsetFlag();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgHatchPatternUseOffsetProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgHatchPatternPtr pObj = OdDgHatchPattern::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  const bool* pValue = rxvalue_cast<bool>(&value);

  if( pValue )
  {
    pObj->setUseOffsetFlag(*pValue);
    return eOk;
  }

  return eNotThatKindOfClass;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: Snappable
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgHatchPatternSnappableProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgHatchPatternPtr pObj = OdDgHatchPattern::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  value = pObj->getSnappableFlag();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgHatchPatternSnappableProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgHatchPatternPtr pObj = OdDgHatchPattern::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  const bool* pValue = rxvalue_cast<bool>(&value);

  if( pValue )
  {
    pObj->setSnappableFlag(*pValue);
    return eOk;
  }

  return eNotThatKindOfClass;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          class: OdDgLinearHatchPattern
//-----------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------
//                                          property: Space
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgLinearHatchPatternSpaceProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgLinearHatchPatternPtr pObj = OdDgLinearHatchPattern::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  value = pObj->getSpace();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgLinearHatchPatternSpaceProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgLinearHatchPatternPtr pObj = OdDgLinearHatchPattern::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  const double* pValue = rxvalue_cast<double>(&value);

  if( pValue )
  {
    pObj->setSpace(*pValue);
    return eOk;
  }

  return eNotThatKindOfClass;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: Angle
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgLinearHatchPatternAngleProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgLinearHatchPatternPtr pObj = OdDgLinearHatchPattern::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  value = pObj->getAngle();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgLinearHatchPatternAngleProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgLinearHatchPatternPtr pObj = OdDgLinearHatchPattern::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  const double* pValue = rxvalue_cast<double>(&value);

  if( pValue )
  {
    pObj->setAngle(*pValue);
    return eOk;
  }

  return eNotThatKindOfClass;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: LineStyleEntryId
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgLinearHatchPatternLineStyleEntryIdProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgLinearHatchPatternPtr pObj = OdDgLinearHatchPattern::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  if( !pObj->getUseLineStyleFlag() )
    return eNotApplicable;

  value = pObj->getLineStyleEntryId();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgLinearHatchPatternLineStyleEntryIdProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgLinearHatchPatternPtr pObj = OdDgLinearHatchPattern::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  const OdUInt32* pValue = rxvalue_cast<OdUInt32>(&value);

  if( pValue )
  {
    pObj->setLineStyleEntryId(*pValue);
    return eOk;
  }

  return eNotThatKindOfClass;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: UseLineStyle
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgLinearHatchPatternUseLineStyleProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgLinearHatchPatternPtr pObj = OdDgLinearHatchPattern::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  value = pObj->getUseLineStyleFlag();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgLinearHatchPatternUseLineStyleProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgLinearHatchPatternPtr pObj = OdDgLinearHatchPattern::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  const bool* pValue = rxvalue_cast<bool>(&value);

  if( pValue )
  {
    pObj->setUseLineStyleFlag(*pValue);
    return eOk;
  }

  return eNotThatKindOfClass;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: LineWeight
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgLinearHatchPatternLineWeightProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgLinearHatchPatternPtr pObj = OdDgLinearHatchPattern::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  if( !pObj->getUseLineWeightFlag() )
    return eNotApplicable;

  value = (OdDb::LineWeight)pObj->getLineWeight();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgLinearHatchPatternLineWeightProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgLinearHatchPatternPtr pObj = OdDgLinearHatchPattern::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  const OdDb::LineWeight* pValue = rxvalue_cast<OdDb::LineWeight>(&value);

  if( pValue )
  {
    pObj->setLineWeight(*(OdUInt32*)pValue);
    return eOk;
  }

  return eNotThatKindOfClass;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: UseLineWeight
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgLinearHatchPatternUseLineWeightProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgLinearHatchPatternPtr pObj = OdDgLinearHatchPattern::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  value = pObj->getUseLineWeightFlag();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgLinearHatchPatternUseLineWeightProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgLinearHatchPatternPtr pObj = OdDgLinearHatchPattern::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  const bool* pValue = rxvalue_cast<bool>(&value);

  if( pValue )
  {
    pObj->setUseLineWeightFlag(*pValue);
    return eOk;
  }

  return eNotThatKindOfClass;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: LineColor
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgLinearHatchPatternLineColorProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgLinearHatchPatternPtr pObj = OdDgLinearHatchPattern::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  if( !pObj->getUseLineColorFlag() )
    return eNotApplicable;

  OdCmEntityColor cmColor;
  OdUInt32 uColorIndex = pObj->getLineColorIndex();

  if( uColorIndex = OdDg::kColorByLevel )
    cmColor.setColorMethod( OdCmEntityColor::kByLayer );
  else if( uColorIndex = OdDg::kColorByCell )
    cmColor.setColorMethod( OdCmEntityColor::kByBlock );
  else
    cmColor.setColorIndex( uColorIndex );

  value = cmColor;

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgLinearHatchPatternLineColorProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgLinearHatchPatternPtr pObj = OdDgLinearHatchPattern::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  const OdCmEntityColor* pValue = rxvalue_cast<OdCmEntityColor>(&value);

  if( pValue )
  {
    OdUInt32 uIndex = 0;

    switch( pValue->colorMethod() )
    {
      case OdCmEntityColor::kByLayer:
        uIndex = OdDg::kColorByLevel; break;
      case OdCmEntityColor::kByBlock:
        uIndex = OdDg::kColorByCell; break;
      case OdCmEntityColor::kByACI:
      case OdCmEntityColor::kByDgnIndex:
      case OdCmEntityColor::kByPen:
        uIndex = pValue->colorIndex(); break;
      default:
        return eNotApplicable;
    }
    pObj->setLineColorIndex(uIndex);
    return eOk;
  }

  return eNotThatKindOfClass;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: UseLineColor
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgLinearHatchPatternUseLineColorProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgLinearHatchPatternPtr pObj = OdDgLinearHatchPattern::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  value = pObj->getUseLineColorFlag();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgLinearHatchPatternUseLineColorProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgLinearHatchPatternPtr pObj = OdDgLinearHatchPattern::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  const bool* pValue = rxvalue_cast<bool>(&value);

  if( pValue )
  {
    pObj->setUseLineColorFlag(*pValue);
    return eOk;
  }

  return eNotThatKindOfClass;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          class: OdDgCrossHatchPattern
//-----------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------
//                                          property: Space1
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgCrossHatchPatternSpace1Property::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgCrossHatchPatternPtr pObj = OdDgCrossHatchPattern::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  value = pObj->getSpace1();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgCrossHatchPatternSpace1Property::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgCrossHatchPatternPtr pObj = OdDgCrossHatchPattern::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  const double* pValue = rxvalue_cast<double>(&value);

  if( pValue )
  {
    pObj->setSpace1(*pValue);
    return eOk;
  }

  return eNotThatKindOfClass;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: Space2
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgCrossHatchPatternSpace2Property::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgCrossHatchPatternPtr pObj = OdDgCrossHatchPattern::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  value = pObj->getSpace2();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgCrossHatchPatternSpace2Property::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgCrossHatchPatternPtr pObj = OdDgCrossHatchPattern::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  const double* pValue = rxvalue_cast<double>(&value);

  if( pValue )
  {
    pObj->setSpace2(*pValue);
    return eOk;
  }

  return eNotThatKindOfClass;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: Angle1
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgCrossHatchPatternAngle1Property::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgCrossHatchPatternPtr pObj = OdDgCrossHatchPattern::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  value = pObj->getAngle1();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgCrossHatchPatternAngle1Property::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgCrossHatchPatternPtr pObj = OdDgCrossHatchPattern::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  const double* pValue = rxvalue_cast<double>(&value);

  if( pValue )
  {
    pObj->setAngle1(*pValue);
    return eOk;
  }

  return eNotThatKindOfClass;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: Angle2
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgCrossHatchPatternAngle2Property::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgCrossHatchPatternPtr pObj = OdDgCrossHatchPattern::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  value = pObj->getAngle2();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgCrossHatchPatternAngle2Property::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgCrossHatchPatternPtr pObj = OdDgCrossHatchPattern::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  const double* pValue = rxvalue_cast<double>(&value);

  if( pValue )
  {
    pObj->setAngle2(*pValue);
    return eOk;
  }

  return eNotThatKindOfClass;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: LineStyleEntryId
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgCrossHatchPatternLineStyleEntryIdProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgCrossHatchPatternPtr pObj = OdDgCrossHatchPattern::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  if( !pObj->getUseLineStyleFlag() )
    return eNotApplicable;

  value = pObj->getLineStyleEntryId();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgCrossHatchPatternLineStyleEntryIdProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgCrossHatchPatternPtr pObj = OdDgCrossHatchPattern::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  const OdUInt32* pValue = rxvalue_cast<OdUInt32>(&value);

  if( pValue )
  {
    pObj->setLineStyleEntryId(*pValue);
    return eOk;
  }

  return eNotThatKindOfClass;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: UseLineStyle
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgCrossHatchPatternUseLineStyleProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgCrossHatchPatternPtr pObj = OdDgCrossHatchPattern::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  value = pObj->getUseLineStyleFlag();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgCrossHatchPatternUseLineStyleProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgCrossHatchPatternPtr pObj = OdDgCrossHatchPattern::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  const bool* pValue = rxvalue_cast<bool>(&value);

  if( pValue )
  {
    pObj->setUseLineStyleFlag(*pValue);
    return eOk;
  }

  return eNotThatKindOfClass;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: LineWeight
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgCrossHatchPatternLineWeightProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgCrossHatchPatternPtr pObj = OdDgCrossHatchPattern::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  if( !pObj->getUseLineWeightFlag() )
    return eNotApplicable;

  value = (OdDb::LineWeight)pObj->getLineWeight();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgCrossHatchPatternLineWeightProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgCrossHatchPatternPtr pObj = OdDgCrossHatchPattern::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  const OdDb::LineWeight* pValue = rxvalue_cast<OdDb::LineWeight>(&value);

  if( pValue )
  {
    pObj->setLineWeight(*(OdUInt32*)pValue);
    return eOk;
  }

  return eNotThatKindOfClass;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: UseLineWeight
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgCrossHatchPatternUseLineWeightProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgCrossHatchPatternPtr pObj = OdDgCrossHatchPattern::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  value = pObj->getUseLineWeightFlag();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgCrossHatchPatternUseLineWeightProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgCrossHatchPatternPtr pObj = OdDgCrossHatchPattern::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  const bool* pValue = rxvalue_cast<bool>(&value);

  if( pValue )
  {
    pObj->setUseLineWeightFlag(*pValue);
    return eOk;
  }

  return eNotThatKindOfClass;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: LineColor
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgCrossHatchPatternLineColorProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgCrossHatchPatternPtr pObj = OdDgCrossHatchPattern::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  if( !pObj->getUseLineColorFlag() )
    return eNotApplicable;

  OdCmEntityColor cmColor;
  OdUInt32 uColorIndex = pObj->getLineColorIndex();

  if( uColorIndex = OdDg::kColorByLevel )
    cmColor.setColorMethod( OdCmEntityColor::kByLayer );
  else if( uColorIndex = OdDg::kColorByCell )
    cmColor.setColorMethod( OdCmEntityColor::kByBlock );
  else
    cmColor.setColorIndex( uColorIndex );

  value = cmColor;

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgCrossHatchPatternLineColorProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgCrossHatchPatternPtr pObj = OdDgCrossHatchPattern::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  const OdCmEntityColor* pValue = rxvalue_cast<OdCmEntityColor>(&value);

  if( pValue )
  {
    OdUInt32 uIndex = 0;

    switch( pValue->colorMethod() )
    {
      case OdCmEntityColor::kByLayer:
        uIndex = OdDg::kColorByLevel; break;
      case OdCmEntityColor::kByBlock:
        uIndex = OdDg::kColorByCell; break;
      case OdCmEntityColor::kByACI:
      case OdCmEntityColor::kByDgnIndex:
      case OdCmEntityColor::kByPen:
        uIndex = pValue->colorIndex(); break;
      default:
        return eNotApplicable;
    }
    pObj->setLineColorIndex(uIndex);
    return eOk;
  }

  return eNotThatKindOfClass;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: UseLineColor
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgCrossHatchPatternUseLineColorProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgCrossHatchPatternPtr pObj = OdDgCrossHatchPattern::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  value = pObj->getUseLineColorFlag();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgCrossHatchPatternUseLineColorProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgCrossHatchPatternPtr pObj = OdDgCrossHatchPattern::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  const bool* pValue = rxvalue_cast<bool>(&value);

  if( pValue )
  {
    pObj->setUseLineColorFlag(*pValue);
    return eOk;
  }

  return eNotThatKindOfClass;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          class: OdDgSymbolHatchPattern
//-----------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------
//                                          property: Space1
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgSymbolHatchPatternSpace1Property::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgSymbolHatchPatternPtr pObj = OdDgSymbolHatchPattern::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  value = pObj->getSpace1();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgSymbolHatchPatternSpace1Property::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgSymbolHatchPatternPtr pObj = OdDgSymbolHatchPattern::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  const double* pValue = rxvalue_cast<double>(&value);

  if( pValue )
  {
    pObj->setSpace1(*pValue);
    return eOk;
  }

  return eNotThatKindOfClass;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: Space2
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgSymbolHatchPatternSpace2Property::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgSymbolHatchPatternPtr pObj = OdDgSymbolHatchPattern::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  value = pObj->getSpace2();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgSymbolHatchPatternSpace2Property::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgSymbolHatchPatternPtr pObj = OdDgSymbolHatchPattern::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  const double* pValue = rxvalue_cast<double>(&value);

  if( pValue )
  {
    pObj->setSpace2(*pValue);
    return eOk;
  }

  return eNotThatKindOfClass;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: Angle1
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgSymbolHatchPatternAngle1Property::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgSymbolHatchPatternPtr pObj = OdDgSymbolHatchPattern::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  value = pObj->getAngle1();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgSymbolHatchPatternAngle1Property::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgSymbolHatchPatternPtr pObj = OdDgSymbolHatchPattern::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  const double* pValue = rxvalue_cast<double>(&value);

  if( pValue )
  {
    pObj->setAngle1(*pValue);
    return eOk;
  }

  return eNotThatKindOfClass;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: Angle2
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgSymbolHatchPatternAngle2Property::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgSymbolHatchPatternPtr pObj = OdDgSymbolHatchPattern::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  value = pObj->getAngle2();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgSymbolHatchPatternAngle2Property::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgSymbolHatchPatternPtr pObj = OdDgSymbolHatchPattern::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  const double* pValue = rxvalue_cast<double>(&value);

  if( pValue )
  {
    pObj->setAngle2(*pValue);
    return eOk;
  }

  return eNotThatKindOfClass;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: LineStyleEntryId
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgSymbolHatchPatternLineStyleEntryIdProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgSymbolHatchPatternPtr pObj = OdDgSymbolHatchPattern::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  if( !pObj->getUseLineStyleFlag() )
    return eNotApplicable;

  value = pObj->getLineStyleEntryId();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgSymbolHatchPatternLineStyleEntryIdProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgSymbolHatchPatternPtr pObj = OdDgSymbolHatchPattern::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  const OdUInt32* pValue = rxvalue_cast<OdUInt32>(&value);

  if( pValue )
  {
    pObj->setLineStyleEntryId(*pValue);
    return eOk;
  }

  return eNotThatKindOfClass;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: UseLineStyle
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgSymbolHatchPatternUseLineStyleProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgSymbolHatchPatternPtr pObj = OdDgSymbolHatchPattern::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  value = pObj->getUseLineStyleFlag();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgSymbolHatchPatternUseLineStyleProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgSymbolHatchPatternPtr pObj = OdDgSymbolHatchPattern::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  const bool* pValue = rxvalue_cast<bool>(&value);

  if( pValue )
  {
    pObj->setUseLineStyleFlag(*pValue);
    return eOk;
  }

  return eNotThatKindOfClass;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: LineWeight
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgSymbolHatchPatternLineWeightProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgSymbolHatchPatternPtr pObj = OdDgSymbolHatchPattern::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  if( !pObj->getUseLineWeightFlag() )
    return eNotApplicable;

  value = (OdDb::LineWeight)pObj->getLineWeight();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgSymbolHatchPatternLineWeightProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgSymbolHatchPatternPtr pObj = OdDgSymbolHatchPattern::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  const OdDb::LineWeight* pValue = rxvalue_cast<OdDb::LineWeight>(&value);

  if( pValue )
  {
    pObj->setLineWeight(*(OdUInt32*)pValue);
    return eOk;
  }

  return eNotThatKindOfClass;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: UseLineWeight
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgSymbolHatchPatternUseLineWeightProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgSymbolHatchPatternPtr pObj = OdDgSymbolHatchPattern::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  value = pObj->getUseLineWeightFlag();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgSymbolHatchPatternUseLineWeightProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgSymbolHatchPatternPtr pObj = OdDgSymbolHatchPattern::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  const bool* pValue = rxvalue_cast<bool>(&value);

  if( pValue )
  {
    pObj->setUseLineWeightFlag(*pValue);
    return eOk;
  }

  return eNotThatKindOfClass;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: LineColor
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgSymbolHatchPatternColorProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgSymbolHatchPatternPtr pObj = OdDgSymbolHatchPattern::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  if( !pObj->getUseColorFlag() )
    return eNotApplicable;

  OdCmEntityColor cmColor;
  OdUInt32 uColorIndex = pObj->getColorIndex();

  if( uColorIndex = OdDg::kColorByLevel )
    cmColor.setColorMethod( OdCmEntityColor::kByLayer );
  else if( uColorIndex = OdDg::kColorByCell )
    cmColor.setColorMethod( OdCmEntityColor::kByBlock );
  else
    cmColor.setColorIndex( uColorIndex );

  value = cmColor;

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgSymbolHatchPatternColorProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgSymbolHatchPatternPtr pObj = OdDgSymbolHatchPattern::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  const OdCmEntityColor* pValue = rxvalue_cast<OdCmEntityColor>(&value);

  if( pValue )
  {
    OdUInt32 uIndex = 0;

    switch( pValue->colorMethod() )
    {
      case OdCmEntityColor::kByLayer:
        uIndex = OdDg::kColorByLevel; break;
      case OdCmEntityColor::kByBlock:
        uIndex = OdDg::kColorByCell; break;
      case OdCmEntityColor::kByACI:
      case OdCmEntityColor::kByDgnIndex:
      case OdCmEntityColor::kByPen:
        uIndex = pValue->colorIndex(); break;
      default:
        return eNotApplicable;
    }
    pObj->setColorIndex(uIndex);
    return eOk;
  }

  return eNotThatKindOfClass;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: UseLineColor
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgSymbolHatchPatternUseColorProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgSymbolHatchPatternPtr pObj = OdDgSymbolHatchPattern::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  value = pObj->getUseColorFlag();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgSymbolHatchPatternUseColorProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgSymbolHatchPatternPtr pObj = OdDgSymbolHatchPattern::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  const bool* pValue = rxvalue_cast<bool>(&value);

  if( pValue )
  {
    pObj->setUseColorFlag(*pValue);
    return eOk;
  }

  return eNotThatKindOfClass;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: TrueScale
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgSymbolHatchPatternTrueScaleProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgSymbolHatchPatternPtr pObj = OdDgSymbolHatchPattern::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  value = pObj->getTrueScaleFlag();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgSymbolHatchPatternTrueScaleProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgSymbolHatchPatternPtr pObj = OdDgSymbolHatchPattern::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  const bool* pValue = rxvalue_cast<bool>(&value);

  if( pValue )
  {
    pObj->setTrueScaleFlag(*pValue);
    return eOk;
  }

  return eNotThatKindOfClass;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: SymbolId
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgSymbolHatchPatternSymbolIdProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgSymbolHatchPatternPtr pObj = OdDgSymbolHatchPattern::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  value = pObj->getSymbolId();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgSymbolHatchPatternSymbolIdProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgSymbolHatchPatternPtr pObj = OdDgSymbolHatchPattern::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  const OdUInt64* pValue = rxvalue_cast<OdUInt64>(&value);

  if( pValue )
  {
    pObj->setSymbolId(*pValue);
    return eOk;
  }

  return eNotThatKindOfClass;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          class: OdDgDWGHatchPattern
//-----------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------
//                                          property: PatternName
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgDWGHatchPatternPatternNameProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgDWGHatchPatternPtr pObj = OdDgDWGHatchPattern::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  value = pObj->getPatternName();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgDWGHatchPatternPatternNameProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgDWGHatchPatternPtr pObj = OdDgDWGHatchPattern::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  const OdString* pValue = rxvalue_cast<OdString>(&value);

  if( pValue )
  {
    pObj->setPatternName(*pValue);
    return eOk;
  }

  return eNotThatKindOfClass;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: DwgLines
//-----------------------------------------------------------------------------------------------------------------------

ODDG_DEFINE_RX_VALUE_ARRAY_ITERATOR(DWGHatchLine);

OdRxValueIteratorPtr OdDgDWGHatchPatternDwgLinesProperty::subNewValueIterator(const OdRxObject* pO) const
{
  OdRxValueIteratorPtr res;
  res = OdRxObjectImpl<OdRxDWGHatchLineValueIterator>::createObject();

  OdDgDWGHatchPatternPtr pHatch = OdDgDWGHatchPattern::cast(pO);

  if( !pHatch.isNull() )
  {
    const OdArray<DWGHatchLine> arrHatches = pHatch->getHatch();

    ((OdRxDWGHatchLineValueIterator*)res.get())->init( arrHatches, 0);
  }
    
  return res;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgDWGHatchPatternDwgLinesProperty::subTryGetCount(const OdRxObject* pO, int& count) const
{
  OdDgDWGHatchPatternPtr pHatch = OdDgDWGHatchPattern::cast(pO);

  if (pHatch.isNull())
    return eNotApplicable;

  count = pHatch->getHatch().size();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgDWGHatchPatternDwgLinesProperty::subGetValue(const OdRxObject* pO, int index, OdRxValue& value) const
{
  OdDgDWGHatchPatternPtr pHatch = OdDgDWGHatchPattern::cast(pO);

  if (pHatch.isNull())
    return eNotApplicable;

  if( index < 0 || index >= (int)pHatch->getHatch().size() )
    return eInvalidIndex;

  value = pHatch->getHatch()[index];

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgDWGHatchPatternDwgLinesProperty::subSetValue(OdRxObject* pO, int index, const OdRxValue& value) const
{
  OdDgDWGHatchPatternPtr pHatch = OdDgDWGHatchPattern::cast(pO);

  if (pHatch.isNull())
    return eNotApplicable;

  if( index < 0 || index >= (int)pHatch->getHatch().size() )
    return eInvalidIndex;

  const DWGHatchLine* pValue = rxvalue_cast<DWGHatchLine>(&value);

  if( pValue )
  {
    OdArray<DWGHatchLine> arrHatches = pHatch->getHatch();
    arrHatches[index] = *pValue;
    pHatch->setHatch( arrHatches );
    return eOk;
  }

  return eNotThatKindOfClass;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgDWGHatchPatternDwgLinesProperty::subInsertValue(OdRxObject* pO, int index, const OdRxValue& value) const
{
  OdDgDWGHatchPatternPtr pHatch = OdDgDWGHatchPattern::cast(pO);

  if (pHatch.isNull())
    return eNotApplicable;

  if( index < 0 )
		index = 0;
	if( index >= (int)pHatch->getHatch().size() )
    index = (int)pHatch->getHatch().size();

  const DWGHatchLine* pValue = rxvalue_cast<DWGHatchLine>(&value);

  if( pValue )
  {
    OdArray<DWGHatchLine> arrHatches = pHatch->getHatch();
    arrHatches.insert( arrHatches.begin() + index, *pValue );
    pHatch->setHatch( arrHatches );
    return eOk;
  }

  return eNotThatKindOfClass;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgDWGHatchPatternDwgLinesProperty::subRemoveValue(OdRxObject* pO, int index) const
{
  OdDgDWGHatchPatternPtr pHatch = OdDgDWGHatchPattern::cast(pO);

  if (pHatch.isNull())
    return eNotApplicable;

  if( index < 0 || index >= (int)pHatch->getHatch().size() )
    return eInvalidIndex;

  OdArray<DWGHatchLine> arrHatches = pHatch->getHatch();
  arrHatches.removeAt(index);
  pHatch->setHatch( arrHatches );

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------



