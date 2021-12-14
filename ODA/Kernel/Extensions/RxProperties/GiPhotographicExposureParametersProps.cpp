///////////////////////////////////////////////////////////////////////////////
// THIS FILE IS MACHINE GENERATED!!!!                                        //
///////////////////////////////////////////////////////////////////////////////

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
#include "RxProperty.h"
#include "RxObjectImpl.h"
#include "RxAttribute.h"
#include "RxCategory.h"
#include "RxValueTypeUtil.h"
#include "DbRootValueTypes.h"
#include "Gi/GiViewportTraits.h"

/* OdGiPhotographicExposureParametersExposureProperty */

struct OdGiPhotographicExposureParametersExposureProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdGiPhotographicExposureParametersExposureProperty>::createObject();
    ((OdGiPhotographicExposureParametersExposureProperty*)res.get())->init(__OD_T("Exposure"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdGiPhotographicExposureParametersExposureProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(0)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdGiPhotographicExposureParametersExposureProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Exposure"));
    ((OdGiPhotographicExposureParametersExposureProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Adjust the exposure value"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Render"))
      OdRxCategory::createObject(L"Render", OdRxCategory::rootCategory());
    ((OdGiPhotographicExposureParametersExposureProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Render", 1));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdGiPhotographicExposureParametersExposureProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdGiPhotographicExposureParametersPtr pObj = OdGiPhotographicExposureParameters::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->exposure());
  return eOk;
}

OdResult OdGiPhotographicExposureParametersExposureProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdGiPhotographicExposureParametersPtr pObj = OdGiPhotographicExposureParameters::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setExposure(val);
  return eOk;
}

/* OdGiPhotographicExposureParametersWhitePointProperty */

struct OdGiPhotographicExposureParametersWhitePointProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdGiPhotographicExposureParametersWhitePointProperty>::createObject();
    ((OdGiPhotographicExposureParametersWhitePointProperty*)res.get())->init(__OD_T("WhitePoint"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdGiPhotographicExposureParametersWhitePointProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(0)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdGiPhotographicExposureParametersWhitePointProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"White Point"));
    ((OdGiPhotographicExposureParametersWhitePointProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Adjust the white point value"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Render"))
      OdRxCategory::createObject(L"Render", OdRxCategory::rootCategory());
    ((OdGiPhotographicExposureParametersWhitePointProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Render", 2));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdGiPhotographicExposureParametersWhitePointProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdGiPhotographicExposureParametersPtr pObj = OdGiPhotographicExposureParameters::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->whitePoint());
  return eOk;
}

OdResult OdGiPhotographicExposureParametersWhitePointProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdGiPhotographicExposureParametersPtr pObj = OdGiPhotographicExposureParameters::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setWhitePoint(val);
  return eOk;
}

void createOdGiPhotographicExposureParametersProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdGiPhotographicExposureParametersExposureProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdGiPhotographicExposureParametersWhitePointProperty::createObject(b.owner()));
  b.add(properties.last());
}
