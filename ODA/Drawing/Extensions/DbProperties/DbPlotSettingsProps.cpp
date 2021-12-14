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
#include "DbPlotSettings.h"
#include "DbValueTypes.h"

/* OdDbPlotSettingsPlotRotationProperty */

struct OdDbPlotSettingsPlotRotationProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbPlotSettingsPlotRotationProperty>::createObject();
    ((OdDbPlotSettingsPlotRotationProperty*)res.get())->init(__OD_T("PlotRotation"), &OdRxValueType::Desc<OdDbPlotSettings::PlotRotation>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbPlotSettingsPlotRotationProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbPlotSettingsPtr pObj = OdDbPlotSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbPlotSettings::PlotRotation>(pObj->plotRotation());
  return eOk;
}

/* OdDbPlotSettingsShowPlotStylesProperty */

struct OdDbPlotSettingsShowPlotStylesProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbPlotSettingsShowPlotStylesProperty>::createObject();
    ((OdDbPlotSettingsShowPlotStylesProperty*)res.get())->init(__OD_T("ShowPlotStyles"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbPlotSettingsShowPlotStylesProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbPlotSettingsPtr pObj = OdDbPlotSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->showPlotStyles());
  return eOk;
}

OdResult OdDbPlotSettingsShowPlotStylesProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbPlotSettingsPtr pObj = OdDbPlotSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setShowPlotStyles(val);
  return eOk;
}

/* OdDbPlotSettingsPlotPlotStylesProperty */

struct OdDbPlotSettingsPlotPlotStylesProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbPlotSettingsPlotPlotStylesProperty>::createObject();
    ((OdDbPlotSettingsPlotPlotStylesProperty*)res.get())->init(__OD_T("PlotPlotStyles"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbPlotSettingsPlotPlotStylesProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbPlotSettingsPtr pObj = OdDbPlotSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->plotPlotStyles());
  return eOk;
}

OdResult OdDbPlotSettingsPlotPlotStylesProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbPlotSettingsPtr pObj = OdDbPlotSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setPlotPlotStyles(val);
  return eOk;
}

/* OdDbPlotSettingsPlotCenteredProperty */

struct OdDbPlotSettingsPlotCenteredProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbPlotSettingsPlotCenteredProperty>::createObject();
    ((OdDbPlotSettingsPlotCenteredProperty*)res.get())->init(__OD_T("PlotCentered"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbPlotSettingsPlotCenteredProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbPlotSettingsPtr pObj = OdDbPlotSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->plotCentered());
  return eOk;
}

/* OdDbPlotSettingsShadePlotResLevelProperty */

struct OdDbPlotSettingsShadePlotResLevelProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbPlotSettingsShadePlotResLevelProperty>::createObject();
    ((OdDbPlotSettingsShadePlotResLevelProperty*)res.get())->init(__OD_T("ShadePlotResLevel"), &OdRxValueType::Desc<OdDbPlotSettings::ShadePlotResLevel>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbPlotSettingsShadePlotResLevelProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbPlotSettingsPtr pObj = OdDbPlotSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbPlotSettings::ShadePlotResLevel>(pObj->shadePlotResLevel());
  return eOk;
}

OdResult OdDbPlotSettingsShadePlotResLevelProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbPlotSettingsPtr pObj = OdDbPlotSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbPlotSettings::ShadePlotResLevel val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setShadePlotResLevel(val);
  return eOk;
}

/* OdDbPlotSettingsShadePlotProperty */

struct OdDbPlotSettingsShadePlotProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbPlotSettingsShadePlotProperty>::createObject();
    ((OdDbPlotSettingsShadePlotProperty*)res.get())->init(__OD_T("ShadePlot"), &OdRxValueType::Desc<OdDbPlotSettings::ShadePlotType>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbPlotSettingsShadePlotProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbPlotSettingsPtr pObj = OdDbPlotSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbPlotSettings::ShadePlotType>(pObj->shadePlot());
  return eOk;
}

OdResult OdDbPlotSettingsShadePlotProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbPlotSettingsPtr pObj = OdDbPlotSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbPlotSettings::ShadePlotType val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  OdResult rc = pObj->setShadePlot(val);
  return rc;
}

/* OdDbPlotSettingsPlotHiddenProperty */

struct OdDbPlotSettingsPlotHiddenProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbPlotSettingsPlotHiddenProperty>::createObject();
    ((OdDbPlotSettingsPlotHiddenProperty*)res.get())->init(__OD_T("PlotHidden"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbPlotSettingsPlotHiddenProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbPlotSettingsPtr pObj = OdDbPlotSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->plotHidden());
  return eOk;
}

OdResult OdDbPlotSettingsPlotHiddenProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbPlotSettingsPtr pObj = OdDbPlotSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setPlotHidden(val);
  return eOk;
}

/* OdDbPlotSettingsPlotTransparencyProperty */

struct OdDbPlotSettingsPlotTransparencyProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbPlotSettingsPlotTransparencyProperty>::createObject();
    ((OdDbPlotSettingsPlotTransparencyProperty*)res.get())->init(__OD_T("PlotTransparency"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbPlotSettingsPlotTransparencyProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbPlotSettingsPtr pObj = OdDbPlotSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->plotTransparency());
  return eOk;
}

OdResult OdDbPlotSettingsPlotTransparencyProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbPlotSettingsPtr pObj = OdDbPlotSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setPlotTransparency(val);
  return eOk;
}

/* OdDbPlotSettingsPlotPaperSizeProperty */

struct OdDbPlotSettingsPlotPaperSizeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbPlotSettingsPlotPaperSizeProperty>::createObject();
    ((OdDbPlotSettingsPlotPaperSizeProperty*)res.get())->init(__OD_T("PlotPaperSize"), &OdRxValueType::Desc<OdGePoint2d>::value(), owner);
    return res;
  }
  virtual OdResult subChildren(OdArray<OdRxMemberPtr>& children) const
  {
    children.append(OdRxSubProperty::createObject(L"X", OdRxValueType::Desc<double>::value(), this));
    children.append(OdRxSubProperty::createObject(L"Y", OdRxValueType::Desc<double>::value(), this));
    return eOk;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbPlotSettingsPlotPaperSizeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbPlotSettingsPtr pObj = OdDbPlotSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_LINE
  double dX = 0, dY = 0; pObj->getPlotPaperSize(dX, dY); OdGePoint2d paperSize(dX, dY); value = paperSize;
  return eOk;
}

/* OdDbPlotSettingsPlotConfigurationNameProperty */

struct OdDbPlotSettingsPlotConfigurationNameProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbPlotSettingsPlotConfigurationNameProperty>::createObject();
    ((OdDbPlotSettingsPlotConfigurationNameProperty*)res.get())->init(__OD_T("PlotConfigurationName"), &OdRxValueType::Desc<OdString>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbPlotSettingsPlotConfigurationNameProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbPlotSettingsPtr pObj = OdDbPlotSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<OdString>(pObj->getPlotCfgName());
  return eOk;
}

/* OdDbPlotSettingsPlotSettingsNameProperty */

struct OdDbPlotSettingsPlotSettingsNameProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbPlotSettingsPlotSettingsNameProperty>::createObject();
    ((OdDbPlotSettingsPlotSettingsNameProperty*)res.get())->init(__OD_T("PlotSettingsName"), &OdRxValueType::Desc<OdString>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbPlotSettingsPlotSettingsNameProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbPlotSettingsPtr pObj = OdDbPlotSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<OdString>(pObj->getPlotSettingsName());
  return eOk;
}

OdResult OdDbPlotSettingsPlotSettingsNameProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbPlotSettingsPtr pObj = OdDbPlotSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdString val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setPlotSettingsName(val);
  return eOk;
}

/* OdDbPlotSettingsCanonicalMediaNameProperty */

struct OdDbPlotSettingsCanonicalMediaNameProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbPlotSettingsCanonicalMediaNameProperty>::createObject();
    ((OdDbPlotSettingsCanonicalMediaNameProperty*)res.get())->init(__OD_T("CanonicalMediaName"), &OdRxValueType::Desc<OdString>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbPlotSettingsCanonicalMediaNameProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbPlotSettingsPtr pObj = OdDbPlotSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<OdString>(pObj->getCanonicalMediaName());
  return eOk;
}

/* OdDbPlotSettingsPlotViewportBordersProperty */

struct OdDbPlotSettingsPlotViewportBordersProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbPlotSettingsPlotViewportBordersProperty>::createObject();
    ((OdDbPlotSettingsPlotViewportBordersProperty*)res.get())->init(__OD_T("PlotViewportBorders"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbPlotSettingsPlotViewportBordersProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbPlotSettingsPtr pObj = OdDbPlotSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->plotViewportBorders());
  return eOk;
}

OdResult OdDbPlotSettingsPlotViewportBordersProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbPlotSettingsPtr pObj = OdDbPlotSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setPlotViewportBorders(val);
  return eOk;
}

/* OdDbPlotSettingsPlotPaperUnitsProperty */

struct OdDbPlotSettingsPlotPaperUnitsProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbPlotSettingsPlotPaperUnitsProperty>::createObject();
    ((OdDbPlotSettingsPlotPaperUnitsProperty*)res.get())->init(__OD_T("PlotPaperUnits"), &OdRxValueType::Desc<OdDbPlotSettings::PlotPaperUnits>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbPlotSettingsPlotPaperUnitsProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbPlotSettingsPtr pObj = OdDbPlotSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbPlotSettings::PlotPaperUnits>(pObj->plotPaperUnits());
  return eOk;
}

/* OdDbPlotSettingsPlotOriginProperty */

struct OdDbPlotSettingsPlotOriginProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbPlotSettingsPlotOriginProperty>::createObject();
    ((OdDbPlotSettingsPlotOriginProperty*)res.get())->init(__OD_T("PlotOrigin"), &OdRxValueType::Desc<OdGePoint2d>::value(), owner);
    return res;
  }
  virtual OdResult subChildren(OdArray<OdRxMemberPtr>& children) const
  {
    children.append(OdRxSubProperty::createObject(L"X", OdRxValueType::Desc<double>::value(), this));
    children.append(OdRxSubProperty::createObject(L"Y", OdRxValueType::Desc<double>::value(), this));
    return eOk;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbPlotSettingsPlotOriginProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbPlotSettingsPtr pObj = OdDbPlotSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_LINE
  double dX = 0, dY = 0; pObj->getPlotOrigin(dX, dY); value = OdGePoint2d(dX, dY);
  return eOk;
}

/* OdDbPlotSettingsShadePlotCustomDpiProperty */

struct OdDbPlotSettingsShadePlotCustomDpiProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbPlotSettingsShadePlotCustomDpiProperty>::createObject();
    ((OdDbPlotSettingsShadePlotCustomDpiProperty*)res.get())->init(__OD_T("ShadePlotCustomDpi"), &OdRxValueType::Desc<short>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbPlotSettingsShadePlotCustomDpiProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbPlotSettingsPtr pObj = OdDbPlotSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<short>(pObj->shadePlotCustomDPI());
  return eOk;
}

OdResult OdDbPlotSettingsShadePlotCustomDpiProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbPlotSettingsPtr pObj = OdDbPlotSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  short val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_FNAME
  pObj->setShadePlotCustomDPI(val);
  return eOk;
}

/* OdDbPlotSettingsDrawViewportsFirstProperty */

struct OdDbPlotSettingsDrawViewportsFirstProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbPlotSettingsDrawViewportsFirstProperty>::createObject();
    ((OdDbPlotSettingsDrawViewportsFirstProperty*)res.get())->init(__OD_T("DrawViewportsFirst"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbPlotSettingsDrawViewportsFirstProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbPlotSettingsPtr pObj = OdDbPlotSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->drawViewportsFirst());
  return eOk;
}

OdResult OdDbPlotSettingsDrawViewportsFirstProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbPlotSettingsPtr pObj = OdDbPlotSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDrawViewportsFirst(val);
  return eOk;
}

/* OdDbPlotSettingsPrintLineweightsProperty */

struct OdDbPlotSettingsPrintLineweightsProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbPlotSettingsPrintLineweightsProperty>::createObject();
    ((OdDbPlotSettingsPrintLineweightsProperty*)res.get())->init(__OD_T("PrintLineweights"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbPlotSettingsPrintLineweightsProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbPlotSettingsPtr pObj = OdDbPlotSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->printLineweights());
  return eOk;
}

OdResult OdDbPlotSettingsPrintLineweightsProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbPlotSettingsPtr pObj = OdDbPlotSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setPrintLineweights(val);
  return eOk;
}

/* OdDbPlotSettingsScaleLineweightsProperty */

struct OdDbPlotSettingsScaleLineweightsProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbPlotSettingsScaleLineweightsProperty>::createObject();
    ((OdDbPlotSettingsScaleLineweightsProperty*)res.get())->init(__OD_T("ScaleLineweights"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbPlotSettingsScaleLineweightsProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbPlotSettingsPtr pObj = OdDbPlotSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->scaleLineweights());
  return eOk;
}

OdResult OdDbPlotSettingsScaleLineweightsProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbPlotSettingsPtr pObj = OdDbPlotSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setScaleLineweights(val);
  return eOk;
}

/* OdDbPlotSettingsModelTypeProperty */

struct OdDbPlotSettingsModelTypeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbPlotSettingsModelTypeProperty>::createObject();
    ((OdDbPlotSettingsModelTypeProperty*)res.get())->init(__OD_T("ModelType"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbPlotSettingsModelTypeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbPlotSettingsPtr pObj = OdDbPlotSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->modelType());
  return eOk;
}

/* OdDbPlotSettingsPlotAsRasterProperty */

struct OdDbPlotSettingsPlotAsRasterProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbPlotSettingsPlotAsRasterProperty>::createObject();
    ((OdDbPlotSettingsPlotAsRasterProperty*)res.get())->init(__OD_T("PlotAsRaster"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbPlotSettingsPlotAsRasterProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbPlotSettingsPtr pObj = OdDbPlotSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->plotAsRaster());
  return eOk;
}

/* OdDbPlotSettingsPlotWireframeProperty */

struct OdDbPlotSettingsPlotWireframeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbPlotSettingsPlotWireframeProperty>::createObject();
    ((OdDbPlotSettingsPlotWireframeProperty*)res.get())->init(__OD_T("PlotWireframe"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbPlotSettingsPlotWireframeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbPlotSettingsPtr pObj = OdDbPlotSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->plotWireframe());
  return eOk;
}

/* OdDbPlotSettingsShadePlotIdProperty */

struct OdDbPlotSettingsShadePlotIdProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbPlotSettingsShadePlotIdProperty>::createObject();
    ((OdDbPlotSettingsShadePlotIdProperty*)res.get())->init(__OD_T("ShadePlotId"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbPlotSettingsShadePlotIdProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbPlotSettingsShadePlotIdProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbPlotSettingsPtr pObj = OdDbPlotSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbObjectId>(pObj->shadePlotId());
  return eOk;
}

/* OdDbPlotSettingsStdScaleProperty */

struct OdDbPlotSettingsStdScaleProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbPlotSettingsStdScaleProperty>::createObject();
    ((OdDbPlotSettingsStdScaleProperty*)res.get())->init(__OD_T("StdScale"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbPlotSettingsStdScaleProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(0)));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbPlotSettingsStdScaleProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbPlotSettingsPtr pObj = OdDbPlotSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_LINE
  double dScale; pObj->getStdScale(dScale); value = dScale;
  return eOk;
}

/* OdDbPlotSettingsUseStandardScaleProperty */

struct OdDbPlotSettingsUseStandardScaleProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbPlotSettingsUseStandardScaleProperty>::createObject();
    ((OdDbPlotSettingsUseStandardScaleProperty*)res.get())->init(__OD_T("UseStandardScale"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbPlotSettingsUseStandardScaleProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbPlotSettingsPtr pObj = OdDbPlotSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->useStandardScale());
  return eOk;
}

/* OdDbPlotSettingsPlotViewNameProperty */

struct OdDbPlotSettingsPlotViewNameProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbPlotSettingsPlotViewNameProperty>::createObject();
    ((OdDbPlotSettingsPlotViewNameProperty*)res.get())->init(__OD_T("PlotViewName"), &OdRxValueType::Desc<OdString>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbPlotSettingsPlotViewNameProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbPlotSettingsPtr pObj = OdDbPlotSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<OdString>(pObj->getPlotViewName());
  return eOk;
}

/* OdDbPlotSettingsPlotTypeProperty */

struct OdDbPlotSettingsPlotTypeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbPlotSettingsPlotTypeProperty>::createObject();
    ((OdDbPlotSettingsPlotTypeProperty*)res.get())->init(__OD_T("PlotType"), &OdRxValueType::Desc<OdDbPlotSettings::PlotType>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbPlotSettingsPlotTypeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbPlotSettingsPtr pObj = OdDbPlotSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbPlotSettings::PlotType>(pObj->plotType());
  return eOk;
}

/* OdDbPlotSettingsCurrentStyleSheetProperty */

struct OdDbPlotSettingsCurrentStyleSheetProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbPlotSettingsCurrentStyleSheetProperty>::createObject();
    ((OdDbPlotSettingsCurrentStyleSheetProperty*)res.get())->init(__OD_T("CurrentStyleSheet"), &OdRxValueType::Desc<OdString>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbPlotSettingsCurrentStyleSheetProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbPlotSettingsPtr pObj = OdDbPlotSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<OdString>(pObj->getCurrentStyleSheet());
  return eOk;
}

/* OdDbPlotSettingsStdScaleTypeProperty */

struct OdDbPlotSettingsStdScaleTypeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbPlotSettingsStdScaleTypeProperty>::createObject();
    ((OdDbPlotSettingsStdScaleTypeProperty*)res.get())->init(__OD_T("StdScaleType"), &OdRxValueType::Desc<OdDbPlotSettings::StdScaleType>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbPlotSettingsStdScaleTypeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbPlotSettingsPtr pObj = OdDbPlotSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbPlotSettings::StdScaleType>(pObj->stdScaleType());
  return eOk;
}

void createOdDbPlotSettingsProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDbPlotSettingsPlotRotationProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbPlotSettingsShowPlotStylesProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbPlotSettingsPlotPlotStylesProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbPlotSettingsPlotCenteredProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbPlotSettingsShadePlotResLevelProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbPlotSettingsShadePlotProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbPlotSettingsPlotHiddenProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbPlotSettingsPlotTransparencyProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbPlotSettingsPlotPaperSizeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbPlotSettingsPlotConfigurationNameProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbPlotSettingsPlotSettingsNameProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbPlotSettingsCanonicalMediaNameProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbPlotSettingsPlotViewportBordersProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbPlotSettingsPlotPaperUnitsProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbPlotSettingsPlotOriginProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbPlotSettingsShadePlotCustomDpiProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbPlotSettingsDrawViewportsFirstProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbPlotSettingsPrintLineweightsProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbPlotSettingsScaleLineweightsProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbPlotSettingsModelTypeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbPlotSettingsPlotAsRasterProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbPlotSettingsPlotWireframeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbPlotSettingsShadePlotIdProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbPlotSettingsStdScaleProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbPlotSettingsUseStandardScaleProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbPlotSettingsPlotViewNameProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbPlotSettingsPlotTypeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbPlotSettingsCurrentStyleSheetProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbPlotSettingsStdScaleTypeProperty::createObject(b.owner()));
  b.add(properties.last());
}
