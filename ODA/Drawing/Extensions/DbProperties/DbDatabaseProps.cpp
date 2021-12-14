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
#include "DbDatabase.h"
#include "DbValueTypes.h"
#include "DbLine.h"

/* OdDbDatabaseCelweightProperty */

struct OdDbDatabaseCelweightProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseCelweightProperty>::createObject();
    ((OdDbDatabaseCelweightProperty*)res.get())->init(__OD_T("Celweight"), &OdRxValueType::Desc<OdDb::LineWeight>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseCelweightProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<OdDb::LineWeight>(pObj->getCELWEIGHT());
  return eOk;
}

OdResult OdDbDatabaseCelweightProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDb::LineWeight val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setCELWEIGHT(val);
  return eOk;
}

/* OdDbDatabaseThicknessProperty */

struct OdDbDatabaseThicknessProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseThicknessProperty>::createObject();
    ((OdDbDatabaseThicknessProperty*)res.get())->init(__OD_T("Thickness"), &OdRxValueType::Desc<double>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseThicknessProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<double>(pObj->getTHICKNESS());
  return eOk;
}

OdResult OdDbDatabaseThicknessProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setTHICKNESS(val);
  return eOk;
}

/* OdDbDatabaseJoinStyleProperty */

struct OdDbDatabaseJoinStyleProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseJoinStyleProperty>::createObject();
    ((OdDbDatabaseJoinStyleProperty*)res.get())->init(__OD_T("JoinStyle"), &OdRxValueType::Desc<OdDb::JoinStyle>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseJoinStyleProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<OdDb::JoinStyle>(pObj->getJOINSTYLE());
  return eOk;
}

OdResult OdDbDatabaseJoinStyleProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDb::JoinStyle val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setJOINSTYLE(val);
  return eOk;
}

/* OdDbDatabaseEndCapsProperty */

struct OdDbDatabaseEndCapsProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseEndCapsProperty>::createObject();
    ((OdDbDatabaseEndCapsProperty*)res.get())->init(__OD_T("EndCaps"), &OdRxValueType::Desc<OdDb::EndCaps>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseEndCapsProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<OdDb::EndCaps>(pObj->getENDCAPS());
  return eOk;
}

OdResult OdDbDatabaseEndCapsProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDb::EndCaps val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setENDCAPS(val);
  return eOk;
}

/* OdDbDatabaseFilletradProperty */

struct OdDbDatabaseFilletradProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseFilletradProperty>::createObject();
    ((OdDbDatabaseFilletradProperty*)res.get())->init(__OD_T("Filletrad"), &OdRxValueType::Desc<double>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseFilletradProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<double>(pObj->getFILLETRAD());
  return eOk;
}

OdResult OdDbDatabaseFilletradProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setFILLETRAD(val);
  return eOk;
}

/* OdDbDatabaseTextsizeProperty */

struct OdDbDatabaseTextsizeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseTextsizeProperty>::createObject();
    ((OdDbDatabaseTextsizeProperty*)res.get())->init(__OD_T("Textsize"), &OdRxValueType::Desc<double>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseTextsizeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<double>(pObj->getTEXTSIZE());
  return eOk;
}

OdResult OdDbDatabaseTextsizeProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setTEXTSIZE(val);
  return eOk;
}

/* OdDbDatabaseLtscaleProperty */

struct OdDbDatabaseLtscaleProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseLtscaleProperty>::createObject();
    ((OdDbDatabaseLtscaleProperty*)res.get())->init(__OD_T("Ltscale"), &OdRxValueType::Desc<double>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseLtscaleProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<double>(pObj->getLTSCALE());
  return eOk;
}

OdResult OdDbDatabaseLtscaleProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setLTSCALE(val);
  return eOk;
}

/* OdDbDatabaseSketchincProperty */

struct OdDbDatabaseSketchincProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseSketchincProperty>::createObject();
    ((OdDbDatabaseSketchincProperty*)res.get())->init(__OD_T("Sketchinc"), &OdRxValueType::Desc<double>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseSketchincProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<double>(pObj->getSKETCHINC());
  return eOk;
}

OdResult OdDbDatabaseSketchincProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setSKETCHINC(val);
  return eOk;
}

/* OdDbDatabaseTracewidProperty */

struct OdDbDatabaseTracewidProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseTracewidProperty>::createObject();
    ((OdDbDatabaseTracewidProperty*)res.get())->init(__OD_T("Tracewid"), &OdRxValueType::Desc<double>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseTracewidProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<double>(pObj->getTRACEWID());
  return eOk;
}

OdResult OdDbDatabaseTracewidProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setTRACEWID(val);
  return eOk;
}

/* OdDbDatabaseLineWeightDisplayProperty */

struct OdDbDatabaseLineWeightDisplayProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseLineWeightDisplayProperty>::createObject();
    ((OdDbDatabaseLineWeightDisplayProperty*)res.get())->init(__OD_T("LineWeightDisplay"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseLineWeightDisplayProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<bool>(pObj->getLWDISPLAY());
  return eOk;
}

OdResult OdDbDatabaseLineWeightDisplayProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setLWDISPLAY(val);
  return eOk;
}

/* OdDbDatabaseUserr2Property */

struct OdDbDatabaseUserr2Property : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseUserr2Property>::createObject();
    ((OdDbDatabaseUserr2Property*)res.get())->init(__OD_T("Userr2"), &OdRxValueType::Desc<double>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseUserr2Property::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<double>(pObj->getUSERR2());
  return eOk;
}

OdResult OdDbDatabaseUserr2Property::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setUSERR2(val);
  return eOk;
}

/* OdDbDatabaseUserr1Property */

struct OdDbDatabaseUserr1Property : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseUserr1Property>::createObject();
    ((OdDbDatabaseUserr1Property*)res.get())->init(__OD_T("Userr1"), &OdRxValueType::Desc<double>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseUserr1Property::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<double>(pObj->getUSERR1());
  return eOk;
}

OdResult OdDbDatabaseUserr1Property::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setUSERR1(val);
  return eOk;
}

/* OdDbDatabaseUserr4Property */

struct OdDbDatabaseUserr4Property : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseUserr4Property>::createObject();
    ((OdDbDatabaseUserr4Property*)res.get())->init(__OD_T("Userr4"), &OdRxValueType::Desc<double>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseUserr4Property::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<double>(pObj->getUSERR4());
  return eOk;
}

OdResult OdDbDatabaseUserr4Property::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setUSERR4(val);
  return eOk;
}

/* OdDbDatabaseUserr3Property */

struct OdDbDatabaseUserr3Property : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseUserr3Property>::createObject();
    ((OdDbDatabaseUserr3Property*)res.get())->init(__OD_T("Userr3"), &OdRxValueType::Desc<double>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseUserr3Property::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<double>(pObj->getUSERR3());
  return eOk;
}

OdResult OdDbDatabaseUserr3Property::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setUSERR3(val);
  return eOk;
}

/* OdDbDatabasePlinewidProperty */

struct OdDbDatabasePlinewidProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabasePlinewidProperty>::createObject();
    ((OdDbDatabasePlinewidProperty*)res.get())->init(__OD_T("Plinewid"), &OdRxValueType::Desc<double>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabasePlinewidProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<double>(pObj->getPLINEWID());
  return eOk;
}

OdResult OdDbDatabasePlinewidProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setPLINEWID(val);
  return eOk;
}

/* OdDbDatabaseOleStartUpProperty */

struct OdDbDatabaseOleStartUpProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseOleStartUpProperty>::createObject();
    ((OdDbDatabaseOleStartUpProperty*)res.get())->init(__OD_T("OleStartUp"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseOleStartUpProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<bool>(pObj->getOLESTARTUP());
  return eOk;
}

OdResult OdDbDatabaseOleStartUpProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setOLESTARTUP(val);
  return eOk;
}

/* OdDbDatabaseXrefEditEnabledProperty */

struct OdDbDatabaseXrefEditEnabledProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseXrefEditEnabledProperty>::createObject();
    ((OdDbDatabaseXrefEditEnabledProperty*)res.get())->init(__OD_T("XrefEditEnabled"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseXrefEditEnabledProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<bool>(pObj->getXEDIT());
  return eOk;
}

OdResult OdDbDatabaseXrefEditEnabledProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setXEDIT(val);
  return eOk;
}

/* OdDbDatabasePdsizeProperty */

struct OdDbDatabasePdsizeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabasePdsizeProperty>::createObject();
    ((OdDbDatabasePdsizeProperty*)res.get())->init(__OD_T("Pdsize"), &OdRxValueType::Desc<double>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabasePdsizeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<double>(pObj->getPDSIZE());
  return eOk;
}

OdResult OdDbDatabasePdsizeProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setPDSIZE(val);
  return eOk;
}

/* OdDbDatabaseAngbaseProperty */

struct OdDbDatabaseAngbaseProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseAngbaseProperty>::createObject();
    ((OdDbDatabaseAngbaseProperty*)res.get())->init(__OD_T("Angbase"), &OdRxValueType::Desc<double>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseAngbaseProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<double>(pObj->getANGBASE());
  return eOk;
}

OdResult OdDbDatabaseAngbaseProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setANGBASE(val);
  return eOk;
}

/* OdDbDatabaseSplinesegsProperty */

struct OdDbDatabaseSplinesegsProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseSplinesegsProperty>::createObject();
    ((OdDbDatabaseSplinesegsProperty*)res.get())->init(__OD_T("Splinesegs"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseSplinesegsProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<int>(pObj->getSPLINESEGS());
  return eOk;
}

OdResult OdDbDatabaseSplinesegsProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setSPLINESEGS(val);
  return eOk;
}

/* OdDbDatabaseUseri5Property */

struct OdDbDatabaseUseri5Property : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseUseri5Property>::createObject();
    ((OdDbDatabaseUseri5Property*)res.get())->init(__OD_T("Useri5"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseUseri5Property::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<int>(pObj->getUSERI5());
  return eOk;
}

OdResult OdDbDatabaseUseri5Property::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setUSERI5(val);
  return eOk;
}

/* OdDbDatabaseSurfvProperty */

struct OdDbDatabaseSurfvProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseSurfvProperty>::createObject();
    ((OdDbDatabaseSurfvProperty*)res.get())->init(__OD_T("Surfv"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseSurfvProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<int>(pObj->getSURFV());
  return eOk;
}

OdResult OdDbDatabaseSurfvProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setSURFV(val);
  return eOk;
}

/* OdDbDatabaseSurfuProperty */

struct OdDbDatabaseSurfuProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseSurfuProperty>::createObject();
    ((OdDbDatabaseSurfuProperty*)res.get())->init(__OD_T("Surfu"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseSurfuProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<int>(pObj->getSURFU());
  return eOk;
}

OdResult OdDbDatabaseSurfuProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setSURFU(val);
  return eOk;
}

/* OdDbDatabaseUseri4Property */

struct OdDbDatabaseUseri4Property : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseUseri4Property>::createObject();
    ((OdDbDatabaseUseri4Property*)res.get())->init(__OD_T("Useri4"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseUseri4Property::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<int>(pObj->getUSERI4());
  return eOk;
}

OdResult OdDbDatabaseUseri4Property::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setUSERI4(val);
  return eOk;
}

/* OdDbDatabaseUseri1Property */

struct OdDbDatabaseUseri1Property : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseUseri1Property>::createObject();
    ((OdDbDatabaseUseri1Property*)res.get())->init(__OD_T("Useri1"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseUseri1Property::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<int>(pObj->getUSERI1());
  return eOk;
}

OdResult OdDbDatabaseUseri1Property::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setUSERI1(val);
  return eOk;
}

/* OdDbDatabasePdmodeProperty */

struct OdDbDatabasePdmodeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabasePdmodeProperty>::createObject();
    ((OdDbDatabasePdmodeProperty*)res.get())->init(__OD_T("Pdmode"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabasePdmodeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<int>(pObj->getPDMODE());
  return eOk;
}

OdResult OdDbDatabasePdmodeProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setPDMODE(val);
  return eOk;
}

/* OdDbDatabaseUseri3Property */

struct OdDbDatabaseUseri3Property : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseUseri3Property>::createObject();
    ((OdDbDatabaseUseri3Property*)res.get())->init(__OD_T("Useri3"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseUseri3Property::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<int>(pObj->getUSERI3());
  return eOk;
}

OdResult OdDbDatabaseUseri3Property::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setUSERI3(val);
  return eOk;
}

/* OdDbDatabaseUseri2Property */

struct OdDbDatabaseUseri2Property : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseUseri2Property>::createObject();
    ((OdDbDatabaseUseri2Property*)res.get())->init(__OD_T("Useri2"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseUseri2Property::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<int>(pObj->getUSERI2());
  return eOk;
}

OdResult OdDbDatabaseUseri2Property::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setUSERI2(val);
  return eOk;
}

/* OdDbDatabaseSurftypeProperty */

struct OdDbDatabaseSurftypeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseSurftypeProperty>::createObject();
    ((OdDbDatabaseSurftypeProperty*)res.get())->init(__OD_T("Surftype"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseSurftypeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<int>(pObj->getSURFTYPE());
  return eOk;
}

OdResult OdDbDatabaseSurftypeProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setSURFTYPE(val);
  return eOk;
}

/* OdDbDatabaseUnitmodeProperty */

struct OdDbDatabaseUnitmodeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseUnitmodeProperty>::createObject();
    ((OdDbDatabaseUnitmodeProperty*)res.get())->init(__OD_T("Unitmode"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseUnitmodeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<int>(pObj->getUNITMODE());
  return eOk;
}

OdResult OdDbDatabaseUnitmodeProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setUNITMODE(val);
  return eOk;
}

/* OdDbDatabaseMeasurementProperty */

struct OdDbDatabaseMeasurementProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseMeasurementProperty>::createObject();
    ((OdDbDatabaseMeasurementProperty*)res.get())->init(__OD_T("Measurement"), &OdRxValueType::Desc<OdDb::MeasurementValue>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseMeasurementProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<OdDb::MeasurementValue>(pObj->getMEASUREMENT());
  return eOk;
}

OdResult OdDbDatabaseMeasurementProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDb::MeasurementValue val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setMEASUREMENT(val);
  return eOk;
}

/* OdDbDatabaseIsolinesProperty */

struct OdDbDatabaseIsolinesProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseIsolinesProperty>::createObject();
    ((OdDbDatabaseIsolinesProperty*)res.get())->init(__OD_T("Isolines"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseIsolinesProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<int>(pObj->getISOLINES());
  return eOk;
}

OdResult OdDbDatabaseIsolinesProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setISOLINES(val);
  return eOk;
}

/* OdDbDatabaseMaxactvpProperty */

struct OdDbDatabaseMaxactvpProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseMaxactvpProperty>::createObject();
    ((OdDbDatabaseMaxactvpProperty*)res.get())->init(__OD_T("Maxactvp"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseMaxactvpProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<int>(pObj->getMAXACTVP());
  return eOk;
}

OdResult OdDbDatabaseMaxactvpProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setMAXACTVP(val);
  return eOk;
}

/* OdDbDatabaseShadedifProperty */

struct OdDbDatabaseShadedifProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseShadedifProperty>::createObject();
    ((OdDbDatabaseShadedifProperty*)res.get())->init(__OD_T("Shadedif"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseShadedifProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<int>(pObj->getSHADEDIF());
  return eOk;
}

OdResult OdDbDatabaseShadedifProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setSHADEDIF(val);
  return eOk;
}

/* OdDbDatabaseSurftab2Property */

struct OdDbDatabaseSurftab2Property : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseSurftab2Property>::createObject();
    ((OdDbDatabaseSurftab2Property*)res.get())->init(__OD_T("Surftab2"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseSurftab2Property::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<int>(pObj->getSURFTAB2());
  return eOk;
}

OdResult OdDbDatabaseSurftab2Property::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setSURFTAB2(val);
  return eOk;
}

/* OdDbDatabaseSurftab1Property */

struct OdDbDatabaseSurftab1Property : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseSurftab1Property>::createObject();
    ((OdDbDatabaseSurftab1Property*)res.get())->init(__OD_T("Surftab1"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseSurftab1Property::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<int>(pObj->getSURFTAB1());
  return eOk;
}

OdResult OdDbDatabaseSurftab1Property::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setSURFTAB1(val);
  return eOk;
}

/* OdDbDatabaseShadedgeProperty */

struct OdDbDatabaseShadedgeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseShadedgeProperty>::createObject();
    ((OdDbDatabaseShadedgeProperty*)res.get())->init(__OD_T("Shadedge"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseShadedgeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<int>(pObj->getSHADEDGE());
  return eOk;
}

OdResult OdDbDatabaseShadedgeProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setSHADEDGE(val);
  return eOk;
}

/* OdDbDatabaseSplinetypeProperty */

struct OdDbDatabaseSplinetypeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseSplinetypeProperty>::createObject();
    ((OdDbDatabaseSplinetypeProperty*)res.get())->init(__OD_T("Splinetype"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseSplinetypeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<int>(pObj->getSPLINETYPE());
  return eOk;
}

OdResult OdDbDatabaseSplinetypeProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setSPLINETYPE(val);
  return eOk;
}

/* OdDbDatabasePextminProperty */

struct OdDbDatabasePextminProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabasePextminProperty>::createObject();
    ((OdDbDatabasePextminProperty*)res.get())->init(__OD_T("Pextmin"), &OdRxValueType::Desc<OdGePoint3d>::value(), owner);
    return res;
  }
  virtual OdResult subChildren(OdArray<OdRxMemberPtr>& children) const
  {
    children.append(OdRxSubProperty::createObject(L"X", OdRxValueType::Desc<double>::value(), this));
    children.append(OdRxSubProperty::createObject(L"Y", OdRxValueType::Desc<double>::value(), this));
    children.append(OdRxSubProperty::createObject(L"Z", OdRxValueType::Desc<double>::value(), this));
    return eOk;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabasePextminProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<OdGePoint3d>(pObj->getPEXTMIN());
  return eOk;
}

OdResult OdDbDatabasePextminProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint3d val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setPEXTMIN(val);
  return eOk;
}

/* OdDbDatabasePinsbaseProperty */

struct OdDbDatabasePinsbaseProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabasePinsbaseProperty>::createObject();
    ((OdDbDatabasePinsbaseProperty*)res.get())->init(__OD_T("Pinsbase"), &OdRxValueType::Desc<OdGePoint3d>::value(), owner);
    return res;
  }
  virtual OdResult subChildren(OdArray<OdRxMemberPtr>& children) const
  {
    children.append(OdRxSubProperty::createObject(L"X", OdRxValueType::Desc<double>::value(), this));
    children.append(OdRxSubProperty::createObject(L"Y", OdRxValueType::Desc<double>::value(), this));
    children.append(OdRxSubProperty::createObject(L"Z", OdRxValueType::Desc<double>::value(), this));
    return eOk;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabasePinsbaseProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<OdGePoint3d>(pObj->getPINSBASE());
  return eOk;
}

OdResult OdDbDatabasePinsbaseProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint3d val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setPINSBASE(val);
  return eOk;
}

/* OdDbDatabasePlimminProperty */

struct OdDbDatabasePlimminProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabasePlimminProperty>::createObject();
    ((OdDbDatabasePlimminProperty*)res.get())->init(__OD_T("Plimmin"), &OdRxValueType::Desc<OdGePoint2d>::value(), owner);
    return res;
  }
  virtual OdResult subChildren(OdArray<OdRxMemberPtr>& children) const
  {
    children.append(OdRxSubProperty::createObject(L"X", OdRxValueType::Desc<double>::value(), this));
    children.append(OdRxSubProperty::createObject(L"Y", OdRxValueType::Desc<double>::value(), this));
    return eOk;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabasePlimminProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<OdGePoint2d>(pObj->getPLIMMIN());
  return eOk;
}

OdResult OdDbDatabasePlimminProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint2d val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setPLIMMIN(val);
  return eOk;
}

/* OdDbDatabasePextmaxProperty */

struct OdDbDatabasePextmaxProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabasePextmaxProperty>::createObject();
    ((OdDbDatabasePextmaxProperty*)res.get())->init(__OD_T("Pextmax"), &OdRxValueType::Desc<OdGePoint3d>::value(), owner);
    return res;
  }
  virtual OdResult subChildren(OdArray<OdRxMemberPtr>& children) const
  {
    children.append(OdRxSubProperty::createObject(L"X", OdRxValueType::Desc<double>::value(), this));
    children.append(OdRxSubProperty::createObject(L"Y", OdRxValueType::Desc<double>::value(), this));
    children.append(OdRxSubProperty::createObject(L"Z", OdRxValueType::Desc<double>::value(), this));
    return eOk;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabasePextmaxProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<OdGePoint3d>(pObj->getPEXTMAX());
  return eOk;
}

OdResult OdDbDatabasePextmaxProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint3d val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setPEXTMAX(val);
  return eOk;
}

/* OdDbDatabaseCeltscaleProperty */

struct OdDbDatabaseCeltscaleProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseCeltscaleProperty>::createObject();
    ((OdDbDatabaseCeltscaleProperty*)res.get())->init(__OD_T("Celtscale"), &OdRxValueType::Desc<double>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseCeltscaleProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<double>(pObj->getCELTSCALE());
  return eOk;
}

OdResult OdDbDatabaseCeltscaleProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setCELTSCALE(val);
  return eOk;
}

/* OdDbDatabaseCmlstyleIDProperty */

struct OdDbDatabaseCmlstyleIDProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseCmlstyleIDProperty>::createObject();
    ((OdDbDatabaseCmlstyleIDProperty*)res.get())->init(__OD_T("CmlstyleID"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbDatabaseCmlstyleIDProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseCmlstyleIDProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<OdDbObjectId>(pObj->getCMLSTYLE());
  return eOk;
}

OdResult OdDbDatabaseCmlstyleIDProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbObjectId val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setCMLSTYLE(val);
  return eOk;
}

/* OdDbDatabaseInterferecolorProperty */

struct OdDbDatabaseInterferecolorProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseInterferecolorProperty>::createObject();
    ((OdDbDatabaseInterferecolorProperty*)res.get())->init(__OD_T("Interferecolor"), &OdRxValueType::Desc<OdCmColor>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseInterferecolorProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<OdCmColor>(pObj->getINTERFERECOLOR());
  return eOk;
}

OdResult OdDbDatabaseInterferecolorProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdCmColor val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setINTERFERECOLOR(val);
  return eOk;
}

/* OdDbDatabaseCmlscaleProperty */

struct OdDbDatabaseCmlscaleProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseCmlscaleProperty>::createObject();
    ((OdDbDatabaseCmlscaleProperty*)res.get())->init(__OD_T("Cmlscale"), &OdRxValueType::Desc<double>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseCmlscaleProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<double>(pObj->getCMLSCALE());
  return eOk;
}

OdResult OdDbDatabaseCmlscaleProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setCMLSCALE(val);
  return eOk;
}

/* OdDbDatabaseCmljustProperty */

struct OdDbDatabaseCmljustProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseCmljustProperty>::createObject();
    ((OdDbDatabaseCmljustProperty*)res.get())->init(__OD_T("Cmljust"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseCmljustProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<int>(pObj->getCMLJUST());
  return eOk;
}

OdResult OdDbDatabaseCmljustProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setCMLJUST(val);
  return eOk;
}

/* OdDbDatabasePlimmaxProperty */

struct OdDbDatabasePlimmaxProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabasePlimmaxProperty>::createObject();
    ((OdDbDatabasePlimmaxProperty*)res.get())->init(__OD_T("Plimmax"), &OdRxValueType::Desc<OdGePoint2d>::value(), owner);
    return res;
  }
  virtual OdResult subChildren(OdArray<OdRxMemberPtr>& children) const
  {
    children.append(OdRxSubProperty::createObject(L"X", OdRxValueType::Desc<double>::value(), this));
    children.append(OdRxSubProperty::createObject(L"Y", OdRxValueType::Desc<double>::value(), this));
    return eOk;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabasePlimmaxProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<OdGePoint2d>(pObj->getPLIMMAX());
  return eOk;
}

OdResult OdDbDatabasePlimmaxProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint2d val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setPLIMMAX(val);
  return eOk;
}

/* OdDbDatabaseLayerTableIdProperty */

struct OdDbDatabaseLayerTableIdProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseLayerTableIdProperty>::createObject();
    ((OdDbDatabaseLayerTableIdProperty*)res.get())->init(__OD_T("LayerTableId"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbDatabaseLayerTableIdProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbDatabaseLayerTableIdProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbObjectId>(pObj->getLayerTableId());
  return eOk;
}

/* OdDbDatabaseExtminProperty */

struct OdDbDatabaseExtminProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseExtminProperty>::createObject();
    ((OdDbDatabaseExtminProperty*)res.get())->init(__OD_T("Extmin"), &OdRxValueType::Desc<OdGePoint3d>::value(), owner);
    return res;
  }
  virtual OdResult subChildren(OdArray<OdRxMemberPtr>& children) const
  {
    children.append(OdRxSubProperty::createObject(L"X", OdRxValueType::Desc<double>::value(), this));
    children.append(OdRxSubProperty::createObject(L"Y", OdRxValueType::Desc<double>::value(), this));
    children.append(OdRxSubProperty::createObject(L"Z", OdRxValueType::Desc<double>::value(), this));
    return eOk;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseExtminProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<OdGePoint3d>(pObj->getEXTMIN());
  return eOk;
}

OdResult OdDbDatabaseExtminProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint3d val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setEXTMIN(val);
  return eOk;
}

/* OdDbDatabaseInsbaseProperty */

struct OdDbDatabaseInsbaseProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseInsbaseProperty>::createObject();
    ((OdDbDatabaseInsbaseProperty*)res.get())->init(__OD_T("Insbase"), &OdRxValueType::Desc<OdGePoint3d>::value(), owner);
    return res;
  }
  virtual OdResult subChildren(OdArray<OdRxMemberPtr>& children) const
  {
    children.append(OdRxSubProperty::createObject(L"X", OdRxValueType::Desc<double>::value(), this));
    children.append(OdRxSubProperty::createObject(L"Y", OdRxValueType::Desc<double>::value(), this));
    children.append(OdRxSubProperty::createObject(L"Z", OdRxValueType::Desc<double>::value(), this));
    return eOk;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseInsbaseProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<OdGePoint3d>(pObj->getINSBASE());
  return eOk;
}

OdResult OdDbDatabaseInsbaseProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint3d val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setINSBASE(val);
  return eOk;
}

/* OdDbDatabasePucsnameProperty */

//COMMENT: TODO: READONLY????? setPUCSNAME()
struct OdDbDatabasePucsnameProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabasePucsnameProperty>::createObject();
    ((OdDbDatabasePucsnameProperty*)res.get())->init(__OD_T("Pucsname"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbDatabasePucsnameProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbDatabasePucsnameProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<OdDbObjectId>(pObj->getPUCSNAME());
  return eOk;
}

/* OdDbDatabasePucsorgProperty */

struct OdDbDatabasePucsorgProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabasePucsorgProperty>::createObject();
    ((OdDbDatabasePucsorgProperty*)res.get())->init(__OD_T("Pucsorg"), &OdRxValueType::Desc<OdGePoint3d>::value(), owner);
    return res;
  }
  virtual OdResult subChildren(OdArray<OdRxMemberPtr>& children) const
  {
    children.append(OdRxSubProperty::createObject(L"X", OdRxValueType::Desc<double>::value(), this));
    children.append(OdRxSubProperty::createObject(L"Y", OdRxValueType::Desc<double>::value(), this));
    children.append(OdRxSubProperty::createObject(L"Z", OdRxValueType::Desc<double>::value(), this));
    return eOk;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbDatabasePucsorgProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<OdGePoint3d>(pObj->getPUCSORG());
  return eOk;
}

/* OdDbDatabasePelevationProperty */

struct OdDbDatabasePelevationProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabasePelevationProperty>::createObject();
    ((OdDbDatabasePelevationProperty*)res.get())->init(__OD_T("Pelevation"), &OdRxValueType::Desc<double>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabasePelevationProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<double>(pObj->getPELEVATION());
  return eOk;
}

OdResult OdDbDatabasePelevationProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setPELEVATION(val);
  return eOk;
}

/* OdDbDatabasePucsydirProperty */

struct OdDbDatabasePucsydirProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabasePucsydirProperty>::createObject();
    ((OdDbDatabasePucsydirProperty*)res.get())->init(__OD_T("Pucsydir"), &OdRxValueType::Desc<OdGeVector3d>::value(), owner);
    return res;
  }
  virtual OdResult subChildren(OdArray<OdRxMemberPtr>& children) const
  {
    children.append(OdRxSubProperty::createObject(L"X", OdRxValueType::Desc<double>::value(), this));
    children.append(OdRxSubProperty::createObject(L"Y", OdRxValueType::Desc<double>::value(), this));
    children.append(OdRxSubProperty::createObject(L"Z", OdRxValueType::Desc<double>::value(), this));
    return eOk;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbDatabasePucsydirProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<OdGeVector3d>(pObj->getPUCSYDIR());
  return eOk;
}

/* OdDbDatabasePucsxdirProperty */

struct OdDbDatabasePucsxdirProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabasePucsxdirProperty>::createObject();
    ((OdDbDatabasePucsxdirProperty*)res.get())->init(__OD_T("Pucsxdir"), &OdRxValueType::Desc<OdGeVector3d>::value(), owner);
    return res;
  }
  virtual OdResult subChildren(OdArray<OdRxMemberPtr>& children) const
  {
    children.append(OdRxSubProperty::createObject(L"X", OdRxValueType::Desc<double>::value(), this));
    children.append(OdRxSubProperty::createObject(L"Y", OdRxValueType::Desc<double>::value(), this));
    children.append(OdRxSubProperty::createObject(L"Z", OdRxValueType::Desc<double>::value(), this));
    return eOk;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbDatabasePucsxdirProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<OdGeVector3d>(pObj->getPUCSXDIR());
  return eOk;
}

/* OdDbDatabaseFacetresProperty */

struct OdDbDatabaseFacetresProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseFacetresProperty>::createObject();
    ((OdDbDatabaseFacetresProperty*)res.get())->init(__OD_T("Facetres"), &OdRxValueType::Desc<double>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseFacetresProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<double>(pObj->getFACETRES());
  return eOk;
}

OdResult OdDbDatabaseFacetresProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setFACETRES(val);
  return eOk;
}

/* OdDbDatabaseHpOriginProperty */

struct OdDbDatabaseHpOriginProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseHpOriginProperty>::createObject();
    ((OdDbDatabaseHpOriginProperty*)res.get())->init(__OD_T("HpOrigin"), &OdRxValueType::Desc<OdGePoint2d>::value(), owner);
    return res;
  }
  virtual OdResult subChildren(OdArray<OdRxMemberPtr>& children) const
  {
    children.append(OdRxSubProperty::createObject(L"X", OdRxValueType::Desc<double>::value(), this));
    children.append(OdRxSubProperty::createObject(L"Y", OdRxValueType::Desc<double>::value(), this));
    return eOk;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseHpOriginProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<OdGePoint2d>(pObj->getHPORIGIN());
  return eOk;
}

OdResult OdDbDatabaseHpOriginProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint2d val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setHPORIGIN(val);
  return eOk;
}

/* OdDbDatabaseHpInheritProperty */

struct OdDbDatabaseHpInheritProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseHpInheritProperty>::createObject();
    ((OdDbDatabaseHpInheritProperty*)res.get())->init(__OD_T("HpInherit"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseHpInheritProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<bool>(pObj->getHPINHERIT());
  return eOk;
}

OdResult OdDbDatabaseHpInheritProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_FNAME
  pObj->setHPINHERIT(val);
  return eOk;
}

/* OdDbDatabaseChamferdProperty */

struct OdDbDatabaseChamferdProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseChamferdProperty>::createObject();
    ((OdDbDatabaseChamferdProperty*)res.get())->init(__OD_T("Chamferd"), &OdRxValueType::Desc<double>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseChamferdProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<double>(pObj->getCHAMFERD());
  return eOk;
}

OdResult OdDbDatabaseChamferdProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setCHAMFERD(val);
  return eOk;
}

/* OdDbDatabaseChamferaProperty */

struct OdDbDatabaseChamferaProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseChamferaProperty>::createObject();
    ((OdDbDatabaseChamferaProperty*)res.get())->init(__OD_T("Chamfera"), &OdRxValueType::Desc<double>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseChamferaProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<double>(pObj->getCHAMFERA());
  return eOk;
}

OdResult OdDbDatabaseChamferaProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setCHAMFERA(val);
  return eOk;
}

/* OdDbDatabaseUserr5Property */

struct OdDbDatabaseUserr5Property : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseUserr5Property>::createObject();
    ((OdDbDatabaseUserr5Property*)res.get())->init(__OD_T("Userr5"), &OdRxValueType::Desc<double>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseUserr5Property::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<double>(pObj->getUSERR5());
  return eOk;
}

OdResult OdDbDatabaseUserr5Property::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setUSERR5(val);
  return eOk;
}

/* OdDbDatabaseChamfercProperty */

struct OdDbDatabaseChamfercProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseChamfercProperty>::createObject();
    ((OdDbDatabaseChamfercProperty*)res.get())->init(__OD_T("Chamferc"), &OdRxValueType::Desc<double>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseChamfercProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<double>(pObj->getCHAMFERC());
  return eOk;
}

OdResult OdDbDatabaseChamfercProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setCHAMFERC(val);
  return eOk;
}

/* OdDbDatabaseChamferbProperty */

struct OdDbDatabaseChamferbProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseChamferbProperty>::createObject();
    ((OdDbDatabaseChamferbProperty*)res.get())->init(__OD_T("Chamferb"), &OdRxValueType::Desc<double>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseChamferbProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<double>(pObj->getCHAMFERB());
  return eOk;
}

OdResult OdDbDatabaseChamferbProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setCHAMFERB(val);
  return eOk;
}

/* OdDbDatabaseCecolorProperty */

struct OdDbDatabaseCecolorProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseCecolorProperty>::createObject();
    ((OdDbDatabaseCecolorProperty*)res.get())->init(__OD_T("Cecolor"), &OdRxValueType::Desc<OdCmColor>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseCecolorProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<OdCmColor>(pObj->getCECOLOR());
  return eOk;
}

OdResult OdDbDatabaseCecolorProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdCmColor val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setCECOLOR(val);
  return eOk;
}

/* OdDbDatabaseMLeaderstyleProperty */

struct OdDbDatabaseMLeaderstyleProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseMLeaderstyleProperty>::createObject();
    ((OdDbDatabaseMLeaderstyleProperty*)res.get())->init(__OD_T("MLeaderstyle"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbDatabaseMLeaderstyleProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseMLeaderstyleProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<OdDbObjectId>(pObj->mleaderstyle());
  return eOk;
}

OdResult OdDbDatabaseMLeaderstyleProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbObjectId val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setMLeaderstyle(val);
  return eOk;
}

/* OdDbDatabaseTablestyleProperty */

struct OdDbDatabaseTablestyleProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseTablestyleProperty>::createObject();
    ((OdDbDatabaseTablestyleProperty*)res.get())->init(__OD_T("Tablestyle"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbDatabaseTablestyleProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseTablestyleProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<OdDbObjectId>(pObj->tablestyle());
  return eOk;
}

OdResult OdDbDatabaseTablestyleProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbObjectId val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setTablestyle(val);
  return eOk;
}

/* OdDbDatabaseInterferevpvsProperty */

struct OdDbDatabaseInterferevpvsProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseInterferevpvsProperty>::createObject();
    ((OdDbDatabaseInterferevpvsProperty*)res.get())->init(__OD_T("Interferevpvs"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbDatabaseInterferevpvsProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseInterferevpvsProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<OdDbObjectId>(pObj->getINTERFEREVPVS());
  return eOk;
}

OdResult OdDbDatabaseInterferevpvsProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbObjectId val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setINTERFEREVPVS(val);
  return eOk;
}

/* OdDbDatabaseInterfereobjvsProperty */

struct OdDbDatabaseInterfereobjvsProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseInterfereobjvsProperty>::createObject();
    ((OdDbDatabaseInterfereobjvsProperty*)res.get())->init(__OD_T("Interfereobjvs"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbDatabaseInterfereobjvsProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseInterfereobjvsProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<OdDbObjectId>(pObj->getINTERFEREOBJVS());
  return eOk;
}

OdResult OdDbDatabaseInterfereobjvsProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbObjectId val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setINTERFEREOBJVS(val);
  return eOk;
}

/* OdDbDatabaseDimstyleProperty */

struct OdDbDatabaseDimstyleProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseDimstyleProperty>::createObject();
    ((OdDbDatabaseDimstyleProperty*)res.get())->init(__OD_T("Dimstyle"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbDatabaseDimstyleProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseDimstyleProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<OdDbObjectId>(pObj->getDIMSTYLE());
  return eOk;
}

OdResult OdDbDatabaseDimstyleProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbObjectId val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setDIMSTYLE(val);
  return eOk;
}

/* OdDbDatabaseHandseedProperty */

//COMMENT: TODO: setHANDSEED() ??????
struct OdDbDatabaseHandseedProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseHandseedProperty>::createObject();
    ((OdDbDatabaseHandseedProperty*)res.get())->init(__OD_T("Handseed"), &OdRxValueType::Desc<OdDbHandle>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseHandseedProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<OdDbHandle>(pObj->handseed());
  return eOk;
}

OdResult OdDbDatabaseHandseedProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  //>>REPLACE_ALL
//TODO: OdDbDatabaseHandseedProperty r/o
//  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
//  if (pObj.isNull())
//    return eNotApplicable;
//  
//#if defined(PROTECTED_DLL_BUILDING) || !defined(PROTECTED_DLL_ENABLED)
//  pObj->setHANDSEED(*rxvalue_cast<OdDbHandle>(&value));
//  return eOk;
//#endif
  return eNotApplicable;
  //<<REPLACE_ALL
}

/* OdDbDatabaseCetransparencyProperty */

struct OdDbDatabaseCetransparencyProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseCetransparencyProperty>::createObject();
    ((OdDbDatabaseCetransparencyProperty*)res.get())->init(__OD_T("Cetransparency"), &OdRxValueType::Desc<OdCmTransparency>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseCetransparencyProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<OdCmTransparency>(pObj->getCETRANSPARENCY());
  return eOk;
}

OdResult OdDbDatabaseCetransparencyProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdCmTransparency val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setCETRANSPARENCY(val);
  return eOk;
}

/* OdDbDatabaseCeltypeProperty */

struct OdDbDatabaseCeltypeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseCeltypeProperty>::createObject();
    ((OdDbDatabaseCeltypeProperty*)res.get())->init(__OD_T("Celtype"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbDatabaseCeltypeProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseCeltypeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<OdDbObjectId>(pObj->getCELTYPE());
  return eOk;
}

OdResult OdDbDatabaseCeltypeProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbObjectId val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setCELTYPE(val);
  return eOk;
}

/* OdDbDatabaseTextstyleProperty */

struct OdDbDatabaseTextstyleProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseTextstyleProperty>::createObject();
    ((OdDbDatabaseTextstyleProperty*)res.get())->init(__OD_T("Textstyle"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbDatabaseTextstyleProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseTextstyleProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<OdDbObjectId>(pObj->getTEXTSTYLE());
  return eOk;
}

OdResult OdDbDatabaseTextstyleProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbObjectId val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setTEXTSTYLE(val);
  return eOk;
}

/* OdDbDatabaseByBlockLinetypeProperty */

struct OdDbDatabaseByBlockLinetypeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseByBlockLinetypeProperty>::createObject();
    ((OdDbDatabaseByBlockLinetypeProperty*)res.get())->init(__OD_T("ByBlockLinetype"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbDatabaseByBlockLinetypeProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbDatabaseByBlockLinetypeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<OdDbObjectId>(pObj->getLinetypeByBlockId());
  return eOk;
}

/* OdDbDatabaseByLayerLinetypeProperty */

struct OdDbDatabaseByLayerLinetypeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseByLayerLinetypeProperty>::createObject();
    ((OdDbDatabaseByLayerLinetypeProperty*)res.get())->init(__OD_T("ByLayerLinetype"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbDatabaseByLayerLinetypeProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbDatabaseByLayerLinetypeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<OdDbObjectId>(pObj->getLinetypeByLayerId());
  return eOk;
}

/* OdDbDatabaseCurrentSpaceIdProperty */

//COMMENT: TODO: excluded in Database.h
struct OdDbDatabaseCurrentSpaceIdProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseCurrentSpaceIdProperty>::createObject();
    ((OdDbDatabaseCurrentSpaceIdProperty*)res.get())->init(__OD_T("CurrentSpaceId"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbDatabaseCurrentSpaceIdProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

//COMMENT: excluded in Database.h
OdResult OdDbDatabaseCurrentSpaceIdProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  //>>REPLACE_ALL
  //TODO: Needed programmer check
  return eNotApplicable;
//  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
//  if (pObj.isNull())
//    return eNotApplicable;
//  // value = pObj->currentSpaceId();
//  return eOk;
  //<<REPLACE_ALL
}

/* OdDbDatabasePaperSpaceVportIdProperty */

struct OdDbDatabasePaperSpaceVportIdProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabasePaperSpaceVportIdProperty>::createObject();
    ((OdDbDatabasePaperSpaceVportIdProperty*)res.get())->init(__OD_T("PaperSpaceVportId"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbDatabasePaperSpaceVportIdProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbDatabasePaperSpaceVportIdProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<OdDbObjectId>(pObj->getPaperSpaceId());
  return eOk;
}

/* OdDbDatabaseContinuousLinetypeProperty */

struct OdDbDatabaseContinuousLinetypeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseContinuousLinetypeProperty>::createObject();
    ((OdDbDatabaseContinuousLinetypeProperty*)res.get())->init(__OD_T("ContinuousLinetype"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbDatabaseContinuousLinetypeProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbDatabaseContinuousLinetypeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<OdDbObjectId>(pObj->getLinetypeContinuousId());
  return eOk;
}

/* OdDbDatabaseNamedObjectsDictionaryIdProperty */

struct OdDbDatabaseNamedObjectsDictionaryIdProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseNamedObjectsDictionaryIdProperty>::createObject();
    ((OdDbDatabaseNamedObjectsDictionaryIdProperty*)res.get())->init(__OD_T("NamedObjectsDictionaryId"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbDatabaseNamedObjectsDictionaryIdProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbDatabaseNamedObjectsDictionaryIdProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbObjectId>(pObj->getNamedObjectsDictionaryId());
  return eOk;
}

/* OdDbDatabaseColorDictionaryIdProperty */

struct OdDbDatabaseColorDictionaryIdProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseColorDictionaryIdProperty>::createObject();
    ((OdDbDatabaseColorDictionaryIdProperty*)res.get())->init(__OD_T("ColorDictionaryId"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbDatabaseColorDictionaryIdProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbDatabaseColorDictionaryIdProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbObjectId>(pObj->getColorDictionaryId());
  return eOk;
}

/* OdDbDatabaseLayerZeroProperty */

struct OdDbDatabaseLayerZeroProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseLayerZeroProperty>::createObject();
    ((OdDbDatabaseLayerZeroProperty*)res.get())->init(__OD_T("LayerZero"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbDatabaseLayerZeroProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbDatabaseLayerZeroProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<OdDbObjectId>(pObj->getLayerZeroId());
  return eOk;
}

/* OdDbDatabasePlotSettingsDictionaryIdProperty */

struct OdDbDatabasePlotSettingsDictionaryIdProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabasePlotSettingsDictionaryIdProperty>::createObject();
    ((OdDbDatabasePlotSettingsDictionaryIdProperty*)res.get())->init(__OD_T("PlotSettingsDictionaryId"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbDatabasePlotSettingsDictionaryIdProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbDatabasePlotSettingsDictionaryIdProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbObjectId>(pObj->getPlotSettingsDictionaryId());
  return eOk;
}

/* OdDbDatabaseTableStyleDictionaryIdProperty */

struct OdDbDatabaseTableStyleDictionaryIdProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseTableStyleDictionaryIdProperty>::createObject();
    ((OdDbDatabaseTableStyleDictionaryIdProperty*)res.get())->init(__OD_T("TableStyleDictionaryId"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbDatabaseTableStyleDictionaryIdProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbDatabaseTableStyleDictionaryIdProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbObjectId>(pObj->getTableStyleDictionaryId());
  return eOk;
}

/* OdDbDatabaseOriginalFileNameProperty */

struct OdDbDatabaseOriginalFileNameProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseOriginalFileNameProperty>::createObject();
    ((OdDbDatabaseOriginalFileNameProperty*)res.get())->init(__OD_T("OriginalFileName"), &OdRxValueType::Desc<OdString>::value(), owner);
    ((OdDbDatabaseOriginalFileNameProperty*)res.get())->attributes().add(OdRxFilePathAttribute::createObject());
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbDatabaseOriginalFileNameProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<OdString>(pObj->originalFilename());
  return eOk;
}

/* OdDbDatabaseNumberOfSavesProperty */

struct OdDbDatabaseNumberOfSavesProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseNumberOfSavesProperty>::createObject();
    ((OdDbDatabaseNumberOfSavesProperty*)res.get())->init(__OD_T("NumberOfSaves"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbDatabaseNumberOfSavesProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<int>(pObj->numberOfSaves());
  return eOk;
}

/* OdDbDatabaseOriginalFileVersionProperty */

struct OdDbDatabaseOriginalFileVersionProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseOriginalFileVersionProperty>::createObject();
    ((OdDbDatabaseOriginalFileVersionProperty*)res.get())->init(__OD_T("OriginalFileVersion"), &OdRxValueType::Desc<OdDb::DwgVersion>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbDatabaseOriginalFileVersionProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<OdDb::DwgVersion>(pObj->originalFileVersion());
  return eOk;
}

/* OdDbDatabaseLastSavedAsVersionProperty */

struct OdDbDatabaseLastSavedAsVersionProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseLastSavedAsVersionProperty>::createObject();
    ((OdDbDatabaseLastSavedAsVersionProperty*)res.get())->init(__OD_T("LastSavedAsVersion"), &OdRxValueType::Desc<OdDb::DwgVersion>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbDatabaseLastSavedAsVersionProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<OdDb::DwgVersion>(pObj->lastSavedAsVersion());
  return eOk;
}

/* OdDbDatabaseApproxNumObjectsProperty */

struct OdDbDatabaseApproxNumObjectsProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseApproxNumObjectsProperty>::createObject();
    ((OdDbDatabaseApproxNumObjectsProperty*)res.get())->init(__OD_T("ApproxNumObjects"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbDatabaseApproxNumObjectsProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<int>(pObj->approxNumObjects());
  return eOk;
}

/* OdDbDatabaseSectionManagerIdProperty */

struct OdDbDatabaseSectionManagerIdProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseSectionManagerIdProperty>::createObject();
    ((OdDbDatabaseSectionManagerIdProperty*)res.get())->init(__OD_T("SectionManagerId"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbDatabaseSectionManagerIdProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbDatabaseSectionManagerIdProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<OdDbObjectId>(pObj->getSectionManager());
  return eOk;
}

/* OdDbDatabaseMLeaderStyleDictionaryIdProperty */

struct OdDbDatabaseMLeaderStyleDictionaryIdProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseMLeaderStyleDictionaryIdProperty>::createObject();
    ((OdDbDatabaseMLeaderStyleDictionaryIdProperty*)res.get())->init(__OD_T("MLeaderStyleDictionaryId"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbDatabaseMLeaderStyleDictionaryIdProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbDatabaseMLeaderStyleDictionaryIdProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbObjectId>(pObj->getMLeaderStyleDictionaryId());
  return eOk;
}

/* OdDbDatabaseSectionViewStyleDictionaryIdProperty */

struct OdDbDatabaseSectionViewStyleDictionaryIdProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseSectionViewStyleDictionaryIdProperty>::createObject();
    ((OdDbDatabaseSectionViewStyleDictionaryIdProperty*)res.get())->init(__OD_T("SectionViewStyleDictionaryId"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbDatabaseSectionViewStyleDictionaryIdProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbDatabaseSectionViewStyleDictionaryIdProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbObjectId>(pObj->getSectionViewStyleDictionaryId());
  return eOk;
}

/* OdDbDatabaseDetailViewStyleDictionaryIdProperty */

struct OdDbDatabaseDetailViewStyleDictionaryIdProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseDetailViewStyleDictionaryIdProperty>::createObject();
    ((OdDbDatabaseDetailViewStyleDictionaryIdProperty*)res.get())->init(__OD_T("DetailViewStyleDictionaryId"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbDatabaseDetailViewStyleDictionaryIdProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbDatabaseDetailViewStyleDictionaryIdProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbObjectId>(pObj->getDetailViewStyleDictionaryId());
  return eOk;
}

/* OdDbDatabaseDataLinkDictionaryIdProperty */

struct OdDbDatabaseDataLinkDictionaryIdProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseDataLinkDictionaryIdProperty>::createObject();
    ((OdDbDatabaseDataLinkDictionaryIdProperty*)res.get())->init(__OD_T("DataLinkDictionaryId"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbDatabaseDataLinkDictionaryIdProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbDatabaseDataLinkDictionaryIdProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<OdDbObjectId>(pObj->dataLinkDictionaryId());
  return eOk;
}

/* OdDbDatabaseBlockTableIdProperty */

struct OdDbDatabaseBlockTableIdProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseBlockTableIdProperty>::createObject();
    ((OdDbDatabaseBlockTableIdProperty*)res.get())->init(__OD_T("BlockTableId"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbDatabaseBlockTableIdProperty*)res.get())->attributes().add(OdRxHierarchyLevelAttribute::createObject(L"Block"));
    ((OdDbDatabaseBlockTableIdProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbDatabaseBlockTableIdProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbObjectId>(pObj->getBlockTableId());
  return eOk;
}

/* OdDbDatabaseClayerProperty */

struct OdDbDatabaseClayerProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseClayerProperty>::createObject();
    ((OdDbDatabaseClayerProperty*)res.get())->init(__OD_T("Clayer"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbDatabaseClayerProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseClayerProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<OdDbObjectId>(pObj->getCLAYER());
  return eOk;
}

OdResult OdDbDatabaseClayerProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbObjectId val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setCLAYER(val);
  return eOk;
}

/* OdDbDatabaseViewTableIdProperty */

struct OdDbDatabaseViewTableIdProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseViewTableIdProperty>::createObject();
    ((OdDbDatabaseViewTableIdProperty*)res.get())->init(__OD_T("ViewTableId"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbDatabaseViewTableIdProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbDatabaseViewTableIdProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbObjectId>(pObj->getViewTableId());
  return eOk;
}

/* OdDbDatabaseTextStyleTableIdProperty */

struct OdDbDatabaseTextStyleTableIdProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseTextStyleTableIdProperty>::createObject();
    ((OdDbDatabaseTextStyleTableIdProperty*)res.get())->init(__OD_T("TextStyleTableId"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbDatabaseTextStyleTableIdProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbDatabaseTextStyleTableIdProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbObjectId>(pObj->getTextStyleTableId());
  return eOk;
}

/* OdDbDatabaseTileModeProperty */

struct OdDbDatabaseTileModeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseTileModeProperty>::createObject();
    ((OdDbDatabaseTileModeProperty*)res.get())->init(__OD_T("TileMode"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseTileModeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<bool>(pObj->getTILEMODE());
  return eOk;
}

OdResult OdDbDatabaseTileModeProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setTILEMODE(val);
  return eOk;
}

/* OdDbDatabaseLinetypeTableIdProperty */

struct OdDbDatabaseLinetypeTableIdProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseLinetypeTableIdProperty>::createObject();
    ((OdDbDatabaseLinetypeTableIdProperty*)res.get())->init(__OD_T("LinetypeTableId"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbDatabaseLinetypeTableIdProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbDatabaseLinetypeTableIdProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbObjectId>(pObj->getLinetypeTableId());
  return eOk;
}

/* OdDbDatabaseUcsTableIdProperty */

struct OdDbDatabaseUcsTableIdProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseUcsTableIdProperty>::createObject();
    ((OdDbDatabaseUcsTableIdProperty*)res.get())->init(__OD_T("UcsTableId"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbDatabaseUcsTableIdProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbDatabaseUcsTableIdProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<OdDbObjectId>(pObj->getUCSTableId());
  return eOk;
}

/* OdDbDatabasePlotStyleNameDictionaryIdProperty */

struct OdDbDatabasePlotStyleNameDictionaryIdProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabasePlotStyleNameDictionaryIdProperty>::createObject();
    ((OdDbDatabasePlotStyleNameDictionaryIdProperty*)res.get())->init(__OD_T("PlotStyleNameDictionaryId"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbDatabasePlotStyleNameDictionaryIdProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbDatabasePlotStyleNameDictionaryIdProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbObjectId>(pObj->getPlotStyleNameDictionaryId());
  return eOk;
}

/* OdDbDatabaseLayoutDictionaryIdProperty */

struct OdDbDatabaseLayoutDictionaryIdProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseLayoutDictionaryIdProperty>::createObject();
    ((OdDbDatabaseLayoutDictionaryIdProperty*)res.get())->init(__OD_T("LayoutDictionaryId"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbDatabaseLayoutDictionaryIdProperty*)res.get())->attributes().add(OdRxHierarchyLevelAttribute::createObject(L"Model"));
    ((OdDbDatabaseLayoutDictionaryIdProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbDatabaseLayoutDictionaryIdProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbObjectId>(pObj->getLayoutDictionaryId());
  return eOk;
}

/* OdDbDatabaseVisualStyleDictionaryIdProperty */

struct OdDbDatabaseVisualStyleDictionaryIdProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseVisualStyleDictionaryIdProperty>::createObject();
    ((OdDbDatabaseVisualStyleDictionaryIdProperty*)res.get())->init(__OD_T("VisualStyleDictionaryId"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbDatabaseVisualStyleDictionaryIdProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbDatabaseVisualStyleDictionaryIdProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbObjectId>(pObj->getVisualStyleDictionaryId());
  return eOk;
}

/* OdDbDatabaseMaterialDictionaryIdProperty */

struct OdDbDatabaseMaterialDictionaryIdProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseMaterialDictionaryIdProperty>::createObject();
    ((OdDbDatabaseMaterialDictionaryIdProperty*)res.get())->init(__OD_T("MaterialDictionaryId"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbDatabaseMaterialDictionaryIdProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbDatabaseMaterialDictionaryIdProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbObjectId>(pObj->getMaterialDictionaryId());
  return eOk;
}

/* OdDbDatabaseGroupDictionaryIdProperty */

struct OdDbDatabaseGroupDictionaryIdProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseGroupDictionaryIdProperty>::createObject();
    ((OdDbDatabaseGroupDictionaryIdProperty*)res.get())->init(__OD_T("GroupDictionaryId"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbDatabaseGroupDictionaryIdProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbDatabaseGroupDictionaryIdProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbObjectId>(pObj->getGroupDictionaryId());
  return eOk;
}

/* OdDbDatabaseRegAppTableIdProperty */

struct OdDbDatabaseRegAppTableIdProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseRegAppTableIdProperty>::createObject();
    ((OdDbDatabaseRegAppTableIdProperty*)res.get())->init(__OD_T("RegAppTableId"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbDatabaseRegAppTableIdProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbDatabaseRegAppTableIdProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbObjectId>(pObj->getRegAppTableId());
  return eOk;
}

/* OdDbDatabaseViewportTableIdProperty */

struct OdDbDatabaseViewportTableIdProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseViewportTableIdProperty>::createObject();
    ((OdDbDatabaseViewportTableIdProperty*)res.get())->init(__OD_T("ViewportTableId"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbDatabaseViewportTableIdProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbDatabaseViewportTableIdProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbObjectId>(pObj->getViewportTableId());
  return eOk;
}

/* OdDbDatabaseMLStyleDictionaryIdProperty */

struct OdDbDatabaseMLStyleDictionaryIdProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseMLStyleDictionaryIdProperty>::createObject();
    ((OdDbDatabaseMLStyleDictionaryIdProperty*)res.get())->init(__OD_T("MLStyleDictionaryId"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbDatabaseMLStyleDictionaryIdProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbDatabaseMLStyleDictionaryIdProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbObjectId>(pObj->getMLStyleDictionaryId());
  return eOk;
}

/* OdDbDatabaseDimStyleTableIdProperty */

struct OdDbDatabaseDimStyleTableIdProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseDimStyleTableIdProperty>::createObject();
    ((OdDbDatabaseDimStyleTableIdProperty*)res.get())->init(__OD_T("DimStyleTableId"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbDatabaseDimStyleTableIdProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbDatabaseDimStyleTableIdProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbObjectId>(pObj->getDimStyleTableId());
  return eOk;
}

/* OdDbDatabaseAngdirProperty */

struct OdDbDatabaseAngdirProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseAngdirProperty>::createObject();
    ((OdDbDatabaseAngdirProperty*)res.get())->init(__OD_T("Angdir"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseAngdirProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<bool>(pObj->getANGDIR());
  return eOk;
}

OdResult OdDbDatabaseAngdirProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setANGDIR(val);
  return eOk;
}

/* OdDbDatabaseSkpolyProperty */

struct OdDbDatabaseSkpolyProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseSkpolyProperty>::createObject();
    ((OdDbDatabaseSkpolyProperty*)res.get())->init(__OD_T("Skpoly"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseSkpolyProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<bool>(pObj->getSKPOLY());
  return eOk;
}

OdResult OdDbDatabaseSkpolyProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setSKPOLY(val);
  return eOk;
}

/* OdDbDatabaseXclipFrameProperty */

struct OdDbDatabaseXclipFrameProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseXclipFrameProperty>::createObject();
    ((OdDbDatabaseXclipFrameProperty*)res.get())->init(__OD_T("XclipFrame"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseXclipFrameProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<int>(pObj->getXCLIPFRAME());
  return eOk;
}

OdResult OdDbDatabaseXclipFrameProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setXCLIPFRAME(val);
  return eOk;
}

/* OdDbDatabaseSplframeProperty */

struct OdDbDatabaseSplframeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseSplframeProperty>::createObject();
    ((OdDbDatabaseSplframeProperty*)res.get())->init(__OD_T("Splframe"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseSplframeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<bool>(pObj->getSPLFRAME());
  return eOk;
}

OdResult OdDbDatabaseSplframeProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setSPLFRAME(val);
  return eOk;
}

/* OdDbDatabaseUsrtimerProperty */

struct OdDbDatabaseUsrtimerProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseUsrtimerProperty>::createObject();
    ((OdDbDatabaseUsrtimerProperty*)res.get())->init(__OD_T("Usrtimer"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseUsrtimerProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<bool>(pObj->getUSRTIMER());
  return eOk;
}

OdResult OdDbDatabaseUsrtimerProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setUSRTIMER(val);
  return eOk;
}

/* OdDbDatabaseSaveproxygraphicsProperty */

struct OdDbDatabaseSaveproxygraphicsProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseSaveproxygraphicsProperty>::createObject();
    ((OdDbDatabaseSaveproxygraphicsProperty*)res.get())->init(__OD_T("Saveproxygraphics"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseSaveproxygraphicsProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<int>(pObj->getPROXYGRAPHICS());
  return eOk;
}

OdResult OdDbDatabaseSaveproxygraphicsProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setPROXYGRAPHICS(val);
  return eOk;
}

/* OdDbDatabaseLimcheckProperty */

struct OdDbDatabaseLimcheckProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseLimcheckProperty>::createObject();
    ((OdDbDatabaseLimcheckProperty*)res.get())->init(__OD_T("Limcheck"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseLimcheckProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<bool>(pObj->getLIMCHECK());
  return eOk;
}

OdResult OdDbDatabaseLimcheckProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setLIMCHECK(val);
  return eOk;
}

/* OdDbDatabasePlineEllipseProperty */

struct OdDbDatabasePlineEllipseProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabasePlineEllipseProperty>::createObject();
    ((OdDbDatabasePlineEllipseProperty*)res.get())->init(__OD_T("PlineEllipse"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabasePlineEllipseProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<bool>(pObj->getPELLIPSE());
  return eOk;
}

OdResult OdDbDatabasePlineEllipseProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setPELLIPSE(val);
  return eOk;
}

/* OdDbDatabaseDispSilhProperty */

struct OdDbDatabaseDispSilhProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseDispSilhProperty>::createObject();
    ((OdDbDatabaseDispSilhProperty*)res.get())->init(__OD_T("DispSilh"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseDispSilhProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<bool>(pObj->getDISPSILH());
  return eOk;
}

OdResult OdDbDatabaseDispSilhProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setDISPSILH(val);
  return eOk;
}

/* OdDbDatabaseMirrtextProperty */

struct OdDbDatabaseMirrtextProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseMirrtextProperty>::createObject();
    ((OdDbDatabaseMirrtextProperty*)res.get())->init(__OD_T("Mirrtext"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseMirrtextProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<bool>(pObj->getMIRRTEXT());
  return eOk;
}

OdResult OdDbDatabaseMirrtextProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setMIRRTEXT(val);
  return eOk;
}

/* OdDbDatabaseAunitsProperty */

struct OdDbDatabaseAunitsProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseAunitsProperty>::createObject();
    ((OdDbDatabaseAunitsProperty*)res.get())->init(__OD_T("Aunits"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseAunitsProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<int>(pObj->getAUNITS());
  return eOk;
}

OdResult OdDbDatabaseAunitsProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setAUNITS(val);
  return eOk;
}

/* OdDbDatabaseLuprecProperty */

struct OdDbDatabaseLuprecProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseLuprecProperty>::createObject();
    ((OdDbDatabaseLuprecProperty*)res.get())->init(__OD_T("Luprec"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseLuprecProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<int>(pObj->getLUPREC());
  return eOk;
}

OdResult OdDbDatabaseLuprecProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setLUPREC(val);
  return eOk;
}

/* OdDbDatabaseAttmodeProperty */

struct OdDbDatabaseAttmodeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseAttmodeProperty>::createObject();
    ((OdDbDatabaseAttmodeProperty*)res.get())->init(__OD_T("Attmode"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseAttmodeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<int>(pObj->getATTMODE());
  return eOk;
}

OdResult OdDbDatabaseAttmodeProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setATTMODE(val);
  return eOk;
}

/* OdDbDatabaseAuprecProperty */

struct OdDbDatabaseAuprecProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseAuprecProperty>::createObject();
    ((OdDbDatabaseAuprecProperty*)res.get())->init(__OD_T("Auprec"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseAuprecProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<int>(pObj->getAUPREC());
  return eOk;
}

OdResult OdDbDatabaseAuprecProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setAUPREC(val);
  return eOk;
}

/* OdDbDatabaseLunitsProperty */

struct OdDbDatabaseLunitsProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseLunitsProperty>::createObject();
    ((OdDbDatabaseLunitsProperty*)res.get())->init(__OD_T("Lunits"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseLunitsProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<int>(pObj->getLUNITS());
  return eOk;
}

OdResult OdDbDatabaseLunitsProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setLUNITS(val);
  return eOk;
}

/* OdDbDatabasePlimcheckProperty */

struct OdDbDatabasePlimcheckProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabasePlimcheckProperty>::createObject();
    ((OdDbDatabasePlimcheckProperty*)res.get())->init(__OD_T("Plimcheck"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabasePlimcheckProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<bool>(pObj->getPLIMCHECK());
  return eOk;
}

OdResult OdDbDatabasePlimcheckProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setPLIMCHECK(val);
  return eOk;
}

/* OdDbDatabaseWorldviewProperty */

struct OdDbDatabaseWorldviewProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseWorldviewProperty>::createObject();
    ((OdDbDatabaseWorldviewProperty*)res.get())->init(__OD_T("Worldview"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseWorldviewProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<bool>(pObj->getWORLDVIEW());
  return eOk;
}

OdResult OdDbDatabaseWorldviewProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setWORLDVIEW(val);
  return eOk;
}

/* OdDbDatabaseTreedepthProperty */

struct OdDbDatabaseTreedepthProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseTreedepthProperty>::createObject();
    ((OdDbDatabaseTreedepthProperty*)res.get())->init(__OD_T("Treedepth"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseTreedepthProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<int>(pObj->getTREEDEPTH());
  return eOk;
}

OdResult OdDbDatabaseTreedepthProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setTREEDEPTH(val);
  return eOk;
}

/* OdDbDatabaseVisretainProperty */

struct OdDbDatabaseVisretainProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseVisretainProperty>::createObject();
    ((OdDbDatabaseVisretainProperty*)res.get())->init(__OD_T("Visretain"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseVisretainProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<bool>(pObj->getVISRETAIN());
  return eOk;
}

OdResult OdDbDatabaseVisretainProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setVISRETAIN(val);
  return eOk;
}

/* OdDbDatabaseMaintenanceReleaseVersionProperty */

struct OdDbDatabaseMaintenanceReleaseVersionProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseMaintenanceReleaseVersionProperty>::createObject();
    ((OdDbDatabaseMaintenanceReleaseVersionProperty*)res.get())->init(__OD_T("MaintenanceReleaseVersion"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

//COMMENT: need to check
OdResult OdDbDatabaseMaintenanceReleaseVersionProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_LINE
  OdDb::MaintReleaseVer maintVer; pObj->originalFileSavedByVersion(&maintVer); value = static_cast<int>(maintVer);
  return eOk;
}

/* OdDbDatabaseRetainOriginalThumbnailBitmapProperty */

struct OdDbDatabaseRetainOriginalThumbnailBitmapProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseRetainOriginalThumbnailBitmapProperty>::createObject();
    ((OdDbDatabaseRetainOriginalThumbnailBitmapProperty*)res.get())->init(__OD_T("RetainOriginalThumbnailBitmap"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseRetainOriginalThumbnailBitmapProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<bool>(pObj->retainOriginalThumbnailBitmap());
  return eOk;
}

OdResult OdDbDatabaseRetainOriginalThumbnailBitmapProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setRetainOriginalThumbnailBitmap(val);
  return eOk;
}

/* OdDbDatabaseXrefBlockIdProperty */

struct OdDbDatabaseXrefBlockIdProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseXrefBlockIdProperty>::createObject();
    ((OdDbDatabaseXrefBlockIdProperty*)res.get())->init(__OD_T("XrefBlockId"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbDatabaseXrefBlockIdProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbDatabaseXrefBlockIdProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<OdDbObjectId>(pObj->xrefBlockId());
  return eOk;
}

/* OdDbDatabaseLastSavedAsMaintenanceVersionProperty */

//COMMENT: TODO: WARNING! need to check
struct OdDbDatabaseLastSavedAsMaintenanceVersionProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseLastSavedAsMaintenanceVersionProperty>::createObject();
    ((OdDbDatabaseLastSavedAsMaintenanceVersionProperty*)res.get())->init(__OD_T("LastSavedAsMaintenanceVersion"), &OdRxValueType::Desc<OdDb::MaintReleaseVer>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbDatabaseLastSavedAsMaintenanceVersionProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_LINE
  OdDb::MaintReleaseVer maintVer; pObj->lastSavedAsVersion(&maintVer); value = maintVer;
  return eOk;
}

/* OdDbDatabaseOriginalFileSavedByVersionProperty */

struct OdDbDatabaseOriginalFileSavedByVersionProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseOriginalFileSavedByVersionProperty>::createObject();
    ((OdDbDatabaseOriginalFileSavedByVersionProperty*)res.get())->init(__OD_T("OriginalFileSavedByVersion"), &OdRxValueType::Desc<OdDb::DwgVersion>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbDatabaseOriginalFileSavedByVersionProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<OdDb::DwgVersion>(pObj->originalFileSavedByVersion());
  return eOk;
}

/* OdDbDatabaseOriginalFileSavedByMaintenanceVersionProperty */

//COMMENT: TODO: need to check
struct OdDbDatabaseOriginalFileSavedByMaintenanceVersionProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseOriginalFileSavedByMaintenanceVersionProperty>::createObject();
    ((OdDbDatabaseOriginalFileSavedByMaintenanceVersionProperty*)res.get())->init(__OD_T("OriginalFileSavedByMaintenanceVersion"), &OdRxValueType::Desc<OdDb::MaintReleaseVer>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbDatabaseOriginalFileSavedByMaintenanceVersionProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_LINE
  OdDb::MaintReleaseVer maintVer; pObj->originalFileSavedByVersion(&maintVer); value = maintVer;
  return eOk;
}

/* OdDbDatabaseOriginalFileMaintenanceVersionProperty */

//COMMENT: TODO: need to check
struct OdDbDatabaseOriginalFileMaintenanceVersionProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseOriginalFileMaintenanceVersionProperty>::createObject();
    ((OdDbDatabaseOriginalFileMaintenanceVersionProperty*)res.get())->init(__OD_T("OriginalFileMaintenanceVersion"), &OdRxValueType::Desc<OdDb::MaintReleaseVer>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbDatabaseOriginalFileMaintenanceVersionProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_LINE
  OdDb::MaintReleaseVer maintVer; pObj->originalFileVersion(&maintVer); value = maintVer;
  return eOk;
}

/* OdDbDatabaseIsEmrProperty */

struct OdDbDatabaseIsEmrProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseIsEmrProperty>::createObject();
    ((OdDbDatabaseIsEmrProperty*)res.get())->init(__OD_T("IsEmr"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbDatabaseIsEmrProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<bool>(pObj->isEMR());
  return eOk;
}

/* OdDbDatabaseFillmodeProperty */

struct OdDbDatabaseFillmodeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseFillmodeProperty>::createObject();
    ((OdDbDatabaseFillmodeProperty*)res.get())->init(__OD_T("Fillmode"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseFillmodeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<bool>(pObj->getFILLMODE());
  return eOk;
}

OdResult OdDbDatabaseFillmodeProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setFILLMODE(val);
  return eOk;
}

/* OdDbDatabaseRegenmodeProperty */

struct OdDbDatabaseRegenmodeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseRegenmodeProperty>::createObject();
    ((OdDbDatabaseRegenmodeProperty*)res.get())->init(__OD_T("Regenmode"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseRegenmodeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<bool>(pObj->getREGENMODE());
  return eOk;
}

OdResult OdDbDatabaseRegenmodeProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setREGENMODE(val);
  return eOk;
}

/* OdDbDatabasePsltscaleProperty */

struct OdDbDatabasePsltscaleProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabasePsltscaleProperty>::createObject();
    ((OdDbDatabasePsltscaleProperty*)res.get())->init(__OD_T("Psltscale"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabasePsltscaleProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<bool>(pObj->getPSLTSCALE());
  return eOk;
}

OdResult OdDbDatabasePsltscaleProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setPSLTSCALE(val);
  return eOk;
}

/* OdDbDatabaseQtextmodeProperty */

struct OdDbDatabaseQtextmodeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseQtextmodeProperty>::createObject();
    ((OdDbDatabaseQtextmodeProperty*)res.get())->init(__OD_T("Qtextmode"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseQtextmodeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<bool>(pObj->getQTEXTMODE());
  return eOk;
}

OdResult OdDbDatabaseQtextmodeProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setQTEXTMODE(val);
  return eOk;
}

/* OdDbDatabaseOrthomodeProperty */

struct OdDbDatabaseOrthomodeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseOrthomodeProperty>::createObject();
    ((OdDbDatabaseOrthomodeProperty*)res.get())->init(__OD_T("Orthomode"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseOrthomodeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<bool>(pObj->getORTHOMODE());
  return eOk;
}

OdResult OdDbDatabaseOrthomodeProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setORTHOMODE(val);
  return eOk;
}

/* OdDbDatabaseDimasoProperty */

struct OdDbDatabaseDimasoProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseDimasoProperty>::createObject();
    ((OdDbDatabaseDimasoProperty*)res.get())->init(__OD_T("Dimaso"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseDimasoProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<bool>(pObj->getDIMASO());
  return eOk;
}

OdResult OdDbDatabaseDimasoProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setDIMASO(val);
  return eOk;
}

/* OdDbDatabasePlinegenProperty */

struct OdDbDatabasePlinegenProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabasePlinegenProperty>::createObject();
    ((OdDbDatabasePlinegenProperty*)res.get())->init(__OD_T("Plinegen"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabasePlinegenProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<bool>(pObj->getPLINEGEN());
  return eOk;
}

OdResult OdDbDatabasePlinegenProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setPLINEGEN(val);
  return eOk;
}

/* OdDbDatabaseDimshoProperty */

struct OdDbDatabaseDimshoProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseDimshoProperty>::createObject();
    ((OdDbDatabaseDimshoProperty*)res.get())->init(__OD_T("Dimsho"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseDimshoProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<bool>(pObj->getDIMSHO());
  return eOk;
}

OdResult OdDbDatabaseDimshoProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setDIMSHO(val);
  return eOk;
}

/* OdDbDatabaseExtmaxProperty */

struct OdDbDatabaseExtmaxProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseExtmaxProperty>::createObject();
    ((OdDbDatabaseExtmaxProperty*)res.get())->init(__OD_T("Extmax"), &OdRxValueType::Desc<OdGePoint3d>::value(), owner);
    return res;
  }
  virtual OdResult subChildren(OdArray<OdRxMemberPtr>& children) const
  {
    children.append(OdRxSubProperty::createObject(L"X", OdRxValueType::Desc<double>::value(), this));
    children.append(OdRxSubProperty::createObject(L"Y", OdRxValueType::Desc<double>::value(), this));
    children.append(OdRxSubProperty::createObject(L"Z", OdRxValueType::Desc<double>::value(), this));
    return eOk;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseExtmaxProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<OdGePoint3d>(pObj->getEXTMAX());
  return eOk;
}

OdResult OdDbDatabaseExtmaxProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint3d val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setEXTMAX(val);
  return eOk;
}

/* OdDbDatabaseTileModeLightSynchProperty */

struct OdDbDatabaseTileModeLightSynchProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseTileModeLightSynchProperty>::createObject();
    ((OdDbDatabaseTileModeLightSynchProperty*)res.get())->init(__OD_T("TileModeLightSynch"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseTileModeLightSynchProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<int>(pObj->getTILEMODELIGHTSYNCH());
  return eOk;
}

OdResult OdDbDatabaseTileModeLightSynchProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setTILEMODELIGHTSYNCH(val);
  return eOk;
}

/* OdDbDatabaseAnnotativeDwgProperty */

struct OdDbDatabaseAnnotativeDwgProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseAnnotativeDwgProperty>::createObject();
    ((OdDbDatabaseAnnotativeDwgProperty*)res.get())->init(__OD_T("AnnotativeDwg"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseAnnotativeDwgProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<bool>(pObj->getANNOTATIVEDWG());
  return eOk;
}

OdResult OdDbDatabaseAnnotativeDwgProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setANNOTATIVEDWG(val);
  return eOk;
}

/* OdDbDatabaseNorthDirectionProperty */

struct OdDbDatabaseNorthDirectionProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseNorthDirectionProperty>::createObject();
    ((OdDbDatabaseNorthDirectionProperty*)res.get())->init(__OD_T("NorthDirection"), &OdRxValueType::Desc<double>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseNorthDirectionProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<double>(pObj->getNORTHDIRECTION());
  return eOk;
}

OdResult OdDbDatabaseNorthDirectionProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setNORTHDIRECTION(val);
  return eOk;
}

/* OdDbDatabaseLightGlyphDisplayProperty */

struct OdDbDatabaseLightGlyphDisplayProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseLightGlyphDisplayProperty>::createObject();
    ((OdDbDatabaseLightGlyphDisplayProperty*)res.get())->init(__OD_T("LightGlyphDisplay"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseLightGlyphDisplayProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<int>(pObj->getLIGHTGLYPHDISPLAY());
  return eOk;
}

OdResult OdDbDatabaseLightGlyphDisplayProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setLIGHTGLYPHDISPLAY(val);
  return eOk;
}

/* OdDbDatabaseLightsInBlocksProperty */

struct OdDbDatabaseLightsInBlocksProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseLightsInBlocksProperty>::createObject();
    ((OdDbDatabaseLightsInBlocksProperty*)res.get())->init(__OD_T("LightsInBlocks"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseLightsInBlocksProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<bool>(pObj->getLIGHTSINBLOCKS());
  return eOk;
}

OdResult OdDbDatabaseLightsInBlocksProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setLIGHTSINBLOCKS(val);
  return eOk;
}

/* OdDbDatabaseAnnoAllVisibleProperty */

struct OdDbDatabaseAnnoAllVisibleProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseAnnoAllVisibleProperty>::createObject();
    ((OdDbDatabaseAnnoAllVisibleProperty*)res.get())->init(__OD_T("AnnoAllVisible"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseAnnoAllVisibleProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<bool>(pObj->getANNOALLVISIBLE());
  return eOk;
}

OdResult OdDbDatabaseAnnoAllVisibleProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setANNOALLVISIBLE(val);
  return eOk;
}

/* OdDbDatabaseShadowPlaneLocationProperty */

struct OdDbDatabaseShadowPlaneLocationProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseShadowPlaneLocationProperty>::createObject();
    ((OdDbDatabaseShadowPlaneLocationProperty*)res.get())->init(__OD_T("ShadowPlaneLocation"), &OdRxValueType::Desc<double>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseShadowPlaneLocationProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<double>(pObj->getSHADOWPLANELOCATION());
  return eOk;
}

OdResult OdDbDatabaseShadowPlaneLocationProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setSHADOWPLANELOCATION(val);
  return eOk;
}

/* OdDbDatabasedragvsProperty */

struct OdDbDatabasedragvsProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabasedragvsProperty>::createObject();
    ((OdDbDatabasedragvsProperty*)res.get())->init(__OD_T("dragvs"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbDatabasedragvsProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabasedragvsProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<OdDbObjectId>(pObj->getDRAGVS());
  return eOk;
}

OdResult OdDbDatabasedragvsProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbObjectId val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setDRAGVS(val);
  return eOk;
}

/* OdDbDatabaseObscuredColorProperty */

struct OdDbDatabaseObscuredColorProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseObscuredColorProperty>::createObject();
    ((OdDbDatabaseObscuredColorProperty*)res.get())->init(__OD_T("ObscuredColor"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseObscuredColorProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<int>(pObj->getOBSCUREDCOLOR());
  return eOk;
}

OdResult OdDbDatabaseObscuredColorProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setOBSCUREDCOLOR(val);
  return eOk;
}

/* OdDbDatabaseObscuredLineTypeProperty */

struct OdDbDatabaseObscuredLineTypeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseObscuredLineTypeProperty>::createObject();
    ((OdDbDatabaseObscuredLineTypeProperty*)res.get())->init(__OD_T("ObscuredLineType"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseObscuredLineTypeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<int>(pObj->getOBSCUREDLTYPE());
  return eOk;
}

OdResult OdDbDatabaseObscuredLineTypeProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setOBSCUREDLTYPE(val);
  return eOk;
}

/* OdDbDatabaseHideTextProperty */

struct OdDbDatabaseHideTextProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseHideTextProperty>::createObject();
    ((OdDbDatabaseHideTextProperty*)res.get())->init(__OD_T("HideText"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseHideTextProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<int>(pObj->getHIDETEXT());
  return eOk;
}

OdResult OdDbDatabaseHideTextProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setHIDETEXT(val);
  return eOk;
}

/* OdDbDatabaseHaloGapProperty */

struct OdDbDatabaseHaloGapProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseHaloGapProperty>::createObject();
    ((OdDbDatabaseHaloGapProperty*)res.get())->init(__OD_T("HaloGap"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseHaloGapProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<int>(pObj->getHALOGAP());
  return eOk;
}

OdResult OdDbDatabaseHaloGapProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setHALOGAP(val);
  return eOk;
}

/* OdDbDatabaseIntersectDisplayProperty */

struct OdDbDatabaseIntersectDisplayProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseIntersectDisplayProperty>::createObject();
    ((OdDbDatabaseIntersectDisplayProperty*)res.get())->init(__OD_T("IntersectDisplay"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseIntersectDisplayProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<int>(pObj->getINTERSECTIONDISPLAY());
  return eOk;
}

OdResult OdDbDatabaseIntersectDisplayProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setINTERSECTIONDISPLAY(val);
  return eOk;
}

/* OdDbDatabaseLatitudeProperty */

struct OdDbDatabaseLatitudeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseLatitudeProperty>::createObject();
    ((OdDbDatabaseLatitudeProperty*)res.get())->init(__OD_T("Latitude"), &OdRxValueType::Desc<double>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseLatitudeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<double>(pObj->getLATITUDE());
  return eOk;
}

OdResult OdDbDatabaseLatitudeProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setLATITUDE(val);
  return eOk;
}

/* OdDbDatabaseLongitudeProperty */

struct OdDbDatabaseLongitudeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseLongitudeProperty>::createObject();
    ((OdDbDatabaseLongitudeProperty*)res.get())->init(__OD_T("Longitude"), &OdRxValueType::Desc<double>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseLongitudeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<double>(pObj->getLONGITUDE());
  return eOk;
}

OdResult OdDbDatabaseLongitudeProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setLONGITUDE(val);
  return eOk;
}

/* OdDbDatabaseIntersectColorProperty */

struct OdDbDatabaseIntersectColorProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseIntersectColorProperty>::createObject();
    ((OdDbDatabaseIntersectColorProperty*)res.get())->init(__OD_T("IntersectColor"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseIntersectColorProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<int>(pObj->getINTERSECTIONCOLOR());
  return eOk;
}

OdResult OdDbDatabaseIntersectColorProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setINTERSECTIONCOLOR(val);
  return eOk;
}

/* OdDbDatabaseMsLtScaleProperty */

struct OdDbDatabaseMsLtScaleProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseMsLtScaleProperty>::createObject();
    ((OdDbDatabaseMsLtScaleProperty*)res.get())->init(__OD_T("MsLtScale"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseMsLtScaleProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<bool>(pObj->getMSLTSCALE());
  return eOk;
}

OdResult OdDbDatabaseMsLtScaleProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setMSLTSCALE(val);
  return eOk;
}

/* OdDbDatabaseSolidHistProperty */

struct OdDbDatabaseSolidHistProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseSolidHistProperty>::createObject();
    ((OdDbDatabaseSolidHistProperty*)res.get())->init(__OD_T("SolidHist"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseSolidHistProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<int>(pObj->getSOLIDHIST());
  return eOk;
}

OdResult OdDbDatabaseSolidHistProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setSOLIDHIST(val);
  return eOk;
}

/* OdDbDatabaseDxEvalProperty */

struct OdDbDatabaseDxEvalProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseDxEvalProperty>::createObject();
    ((OdDbDatabaseDxEvalProperty*)res.get())->init(__OD_T("DxEval"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseDxEvalProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<int>(pObj->getDXEVAL());
  return eOk;
}

OdResult OdDbDatabaseDxEvalProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setDXEVAL(val);
  return eOk;
}

/* OdDbDatabasePsolWidthProperty */

struct OdDbDatabasePsolWidthProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabasePsolWidthProperty>::createObject();
    ((OdDbDatabasePsolWidthProperty*)res.get())->init(__OD_T("PsolWidth"), &OdRxValueType::Desc<double>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabasePsolWidthProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<double>(pObj->getPSOLWIDTH());
  return eOk;
}

OdResult OdDbDatabasePsolWidthProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setPSOLWIDTH(val);
  return eOk;
}

/* OdDbDatabaseShowHistProperty */

struct OdDbDatabaseShowHistProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseShowHistProperty>::createObject();
    ((OdDbDatabaseShowHistProperty*)res.get())->init(__OD_T("ShowHist"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseShowHistProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<int>(pObj->getSHOWHIST());
  return eOk;
}

OdResult OdDbDatabaseShowHistProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setSHOWHIST(val);
  return eOk;
}

/* OdDbDatabaseStepSizeProperty */

struct OdDbDatabaseStepSizeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseStepSizeProperty>::createObject();
    ((OdDbDatabaseStepSizeProperty*)res.get())->init(__OD_T("StepSize"), &OdRxValueType::Desc<double>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseStepSizeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<double>(pObj->getSTEPSIZE());
  return eOk;
}

OdResult OdDbDatabaseStepSizeProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setSTEPSIZE(val);
  return eOk;
}

/* OdDbDatabaseLensLengthProperty */

struct OdDbDatabaseLensLengthProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseLensLengthProperty>::createObject();
    ((OdDbDatabaseLensLengthProperty*)res.get())->init(__OD_T("LensLength"), &OdRxValueType::Desc<double>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseLensLengthProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<double>(pObj->getLENSLENGTH());
  return eOk;
}

OdResult OdDbDatabaseLensLengthProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setLENSLENGTH(val);
  return eOk;
}

/* OdDbDatabaseCameraHeightProperty */

struct OdDbDatabaseCameraHeightProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseCameraHeightProperty>::createObject();
    ((OdDbDatabaseCameraHeightProperty*)res.get())->init(__OD_T("CameraHeight"), &OdRxValueType::Desc<double>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseCameraHeightProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<double>(pObj->getCAMERAHEIGHT());
  return eOk;
}

OdResult OdDbDatabaseCameraHeightProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setCAMERAHEIGHT(val);
  return eOk;
}

/* OdDbDatabaseStepsPerSecProperty */

struct OdDbDatabaseStepsPerSecProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseStepsPerSecProperty>::createObject();
    ((OdDbDatabaseStepsPerSecProperty*)res.get())->init(__OD_T("StepsPerSec"), &OdRxValueType::Desc<double>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseStepsPerSecProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<double>(pObj->getSTEPSPERSEC());
  return eOk;
}

OdResult OdDbDatabaseStepsPerSecProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setSTEPSPERSEC(val);
  return eOk;
}

/* OdDbDatabaseCameraDisplayProperty */

struct OdDbDatabaseCameraDisplayProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseCameraDisplayProperty>::createObject();
    ((OdDbDatabaseCameraDisplayProperty*)res.get())->init(__OD_T("CameraDisplay"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseCameraDisplayProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<bool>(pObj->getCAMERADISPLAY());
  return eOk;
}

OdResult OdDbDatabaseCameraDisplayProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setCAMERADISPLAY(val);
  return eOk;
}

/* OdDbDatabaseLoftMag2Property */

struct OdDbDatabaseLoftMag2Property : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseLoftMag2Property>::createObject();
    ((OdDbDatabaseLoftMag2Property*)res.get())->init(__OD_T("LoftMag2"), &OdRxValueType::Desc<double>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseLoftMag2Property::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<double>(pObj->getLOFTMAG2());
  return eOk;
}

OdResult OdDbDatabaseLoftMag2Property::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setLOFTMAG2(val);
  return eOk;
}

/* OdDbDatabaseLoftMag1Property */

struct OdDbDatabaseLoftMag1Property : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseLoftMag1Property>::createObject();
    ((OdDbDatabaseLoftMag1Property*)res.get())->init(__OD_T("LoftMag1"), &OdRxValueType::Desc<double>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseLoftMag1Property::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<double>(pObj->getLOFTMAG1());
  return eOk;
}

OdResult OdDbDatabaseLoftMag1Property::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setLOFTMAG1(val);
  return eOk;
}

/* OdDbDatabaseCmaterialProperty */

struct OdDbDatabaseCmaterialProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseCmaterialProperty>::createObject();
    ((OdDbDatabaseCmaterialProperty*)res.get())->init(__OD_T("Cmaterial"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbDatabaseCmaterialProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseCmaterialProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<OdDbObjectId>(pObj->getCMATERIAL());
  return eOk;
}

OdResult OdDbDatabaseCmaterialProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbObjectId val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setCMATERIAL(val);
  return eOk;
}

/* OdDbDatabaseCshadowProperty */

struct OdDbDatabaseCshadowProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseCshadowProperty>::createObject();
    ((OdDbDatabaseCshadowProperty*)res.get())->init(__OD_T("Cshadow"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseCshadowProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<int>(pObj->getCSHADOW());
  return eOk;
}

OdResult OdDbDatabaseCshadowProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setCSHADOW(static_cast<OdGiSubEntityTraits::ShadowFlags>(val));
  return eOk;
}

/* OdDbDatabaseLoftAng2Property */

struct OdDbDatabaseLoftAng2Property : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseLoftAng2Property>::createObject();
    ((OdDbDatabaseLoftAng2Property*)res.get())->init(__OD_T("LoftAng2"), &OdRxValueType::Desc<double>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseLoftAng2Property::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<double>(pObj->getLOFTANG2());
  return eOk;
}

OdResult OdDbDatabaseLoftAng2Property::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setLOFTANG2(val);
  return eOk;
}

/* OdDbDatabaseLoftParamProperty */

struct OdDbDatabaseLoftParamProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseLoftParamProperty>::createObject();
    ((OdDbDatabaseLoftParamProperty*)res.get())->init(__OD_T("LoftParam"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseLoftParamProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<int>(pObj->getLOFTPARAM());
  return eOk;
}

OdResult OdDbDatabaseLoftParamProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setLOFTPARAM(static_cast<OdDb::LoftParamType>(val));
  return eOk;
}

/* OdDbDatabasePsolHeightProperty */

struct OdDbDatabasePsolHeightProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabasePsolHeightProperty>::createObject();
    ((OdDbDatabasePsolHeightProperty*)res.get())->init(__OD_T("PsolHeight"), &OdRxValueType::Desc<double>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabasePsolHeightProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<double>(pObj->getPSOLHEIGHT());
  return eOk;
}

OdResult OdDbDatabasePsolHeightProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setPSOLHEIGHT(val);
  return eOk;
}

/* OdDbDatabaseLoftAng1Property */

struct OdDbDatabaseLoftAng1Property : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseLoftAng1Property>::createObject();
    ((OdDbDatabaseLoftAng1Property*)res.get())->init(__OD_T("LoftAng1"), &OdRxValueType::Desc<double>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseLoftAng1Property::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<double>(pObj->getLOFTANG1());
  return eOk;
}

OdResult OdDbDatabaseLoftAng1Property::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setLOFTANG1(val);
  return eOk;
}

/* OdDbDatabaseLoftNormalsProperty */

struct OdDbDatabaseLoftNormalsProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseLoftNormalsProperty>::createObject();
    ((OdDbDatabaseLoftNormalsProperty*)res.get())->init(__OD_T("LoftNormals"), &OdRxValueType::Desc<OdDb::LoftNormalsType>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseLoftNormalsProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<OdDb::LoftNormalsType>(pObj->getLOFTNORMALS());
  return eOk;
}

OdResult OdDbDatabaseLoftNormalsProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDb::LoftNormalsType val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setLOFTNORMALS(val);
  return eOk;
}

/* OdDbDatabasePucsBaseProperty */

struct OdDbDatabasePucsBaseProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabasePucsBaseProperty>::createObject();
    ((OdDbDatabasePucsBaseProperty*)res.get())->init(__OD_T("PucsBase"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbDatabasePucsBaseProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabasePucsBaseProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<OdDbObjectId>(pObj->getPUCSBASE());
  return eOk;
}

OdResult OdDbDatabasePucsBaseProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbObjectId val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setPUCSBASE(val);
  return eOk;
}

/* OdDbDatabaseUpdateThumbnailProperty */

struct OdDbDatabaseUpdateThumbnailProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseUpdateThumbnailProperty>::createObject();
    ((OdDbDatabaseUpdateThumbnailProperty*)res.get())->init(__OD_T("UpdateThumbnail"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseUpdateThumbnailProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<int>(pObj->getUPDATETHUMBNAIL());
  return eOk;
}

OdResult OdDbDatabaseUpdateThumbnailProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setUPDATETHUMBNAIL(val);
  return eOk;
}

/* OdDbDatabaseIsPartiallyOpenedProperty */

struct OdDbDatabaseIsPartiallyOpenedProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseIsPartiallyOpenedProperty>::createObject();
    ((OdDbDatabaseIsPartiallyOpenedProperty*)res.get())->init(__OD_T("IsPartiallyOpened"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbDatabaseIsPartiallyOpenedProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<bool>(pObj->isPartiallyOpened());
  return eOk;
}

/* OdDbDatabaseElevationProperty */

struct OdDbDatabaseElevationProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseElevationProperty>::createObject();
    ((OdDbDatabaseElevationProperty*)res.get())->init(__OD_T("Elevation"), &OdRxValueType::Desc<double>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseElevationProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<double>(pObj->getELEVATION());
  return eOk;
}

OdResult OdDbDatabaseElevationProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setELEVATION(val);
  return eOk;
}

/* OdDbDatabaseUcsorgProperty */

struct OdDbDatabaseUcsorgProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseUcsorgProperty>::createObject();
    ((OdDbDatabaseUcsorgProperty*)res.get())->init(__OD_T("Ucsorg"), &OdRxValueType::Desc<OdGePoint3d>::value(), owner);
    return res;
  }
  virtual OdResult subChildren(OdArray<OdRxMemberPtr>& children) const
  {
    children.append(OdRxSubProperty::createObject(L"X", OdRxValueType::Desc<double>::value(), this));
    children.append(OdRxSubProperty::createObject(L"Y", OdRxValueType::Desc<double>::value(), this));
    children.append(OdRxSubProperty::createObject(L"Z", OdRxValueType::Desc<double>::value(), this));
    return eOk;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbDatabaseUcsorgProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<OdGePoint3d>(pObj->getUCSORG());
  return eOk;
}

/* OdDbDatabaseLimminProperty */

struct OdDbDatabaseLimminProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseLimminProperty>::createObject();
    ((OdDbDatabaseLimminProperty*)res.get())->init(__OD_T("Limmin"), &OdRxValueType::Desc<OdGePoint2d>::value(), owner);
    return res;
  }
  virtual OdResult subChildren(OdArray<OdRxMemberPtr>& children) const
  {
    children.append(OdRxSubProperty::createObject(L"X", OdRxValueType::Desc<double>::value(), this));
    children.append(OdRxSubProperty::createObject(L"Y", OdRxValueType::Desc<double>::value(), this));
    return eOk;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseLimminProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<OdGePoint2d>(pObj->getLIMMIN());
  return eOk;
}

OdResult OdDbDatabaseLimminProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint2d val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setLIMMIN(val);
  return eOk;
}

/* OdDbDatabaseLimmaxProperty */

struct OdDbDatabaseLimmaxProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseLimmaxProperty>::createObject();
    ((OdDbDatabaseLimmaxProperty*)res.get())->init(__OD_T("Limmax"), &OdRxValueType::Desc<OdGePoint2d>::value(), owner);
    return res;
  }
  virtual OdResult subChildren(OdArray<OdRxMemberPtr>& children) const
  {
    children.append(OdRxSubProperty::createObject(L"X", OdRxValueType::Desc<double>::value(), this));
    children.append(OdRxSubProperty::createObject(L"Y", OdRxValueType::Desc<double>::value(), this));
    return eOk;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseLimmaxProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<OdGePoint2d>(pObj->getLIMMAX());
  return eOk;
}

OdResult OdDbDatabaseLimmaxProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint2d val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setLIMMAX(val);
  return eOk;
}

/* OdDbDatabaseUcsnameProperty */

struct OdDbDatabaseUcsnameProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseUcsnameProperty>::createObject();
    ((OdDbDatabaseUcsnameProperty*)res.get())->init(__OD_T("Ucsname"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbDatabaseUcsnameProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbDatabaseUcsnameProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<OdDbObjectId>(pObj->getUCSNAME());
  return eOk;
}

/* OdDbDatabaseUcsBaseProperty */

struct OdDbDatabaseUcsBaseProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseUcsBaseProperty>::createObject();
    ((OdDbDatabaseUcsBaseProperty*)res.get())->init(__OD_T("UcsBase"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbDatabaseUcsBaseProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseUcsBaseProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<OdDbObjectId>(pObj->getUCSBASE());
  return eOk;
}

OdResult OdDbDatabaseUcsBaseProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbObjectId val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setUCSBASE(val);
  return eOk;
}

/* OdDbDatabaseUcsxdirProperty */

struct OdDbDatabaseUcsxdirProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseUcsxdirProperty>::createObject();
    ((OdDbDatabaseUcsxdirProperty*)res.get())->init(__OD_T("Ucsxdir"), &OdRxValueType::Desc<OdGeVector3d>::value(), owner);
    return res;
  }
  virtual OdResult subChildren(OdArray<OdRxMemberPtr>& children) const
  {
    children.append(OdRxSubProperty::createObject(L"X", OdRxValueType::Desc<double>::value(), this));
    children.append(OdRxSubProperty::createObject(L"Y", OdRxValueType::Desc<double>::value(), this));
    children.append(OdRxSubProperty::createObject(L"Z", OdRxValueType::Desc<double>::value(), this));
    return eOk;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbDatabaseUcsxdirProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<OdGeVector3d>(pObj->getUCSXDIR());
  return eOk;
}

/* OdDbDatabaseUcsydirProperty */

struct OdDbDatabaseUcsydirProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseUcsydirProperty>::createObject();
    ((OdDbDatabaseUcsydirProperty*)res.get())->init(__OD_T("Ucsydir"), &OdRxValueType::Desc<OdGeVector3d>::value(), owner);
    return res;
  }
  virtual OdResult subChildren(OdArray<OdRxMemberPtr>& children) const
  {
    children.append(OdRxSubProperty::createObject(L"X", OdRxValueType::Desc<double>::value(), this));
    children.append(OdRxSubProperty::createObject(L"Y", OdRxValueType::Desc<double>::value(), this));
    children.append(OdRxSubProperty::createObject(L"Z", OdRxValueType::Desc<double>::value(), this));
    return eOk;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbDatabaseUcsydirProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<OdGeVector3d>(pObj->getUCSYDIR());
  return eOk;
}

/* OdDbDatabaseDimAssocProperty */

struct OdDbDatabaseDimAssocProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseDimAssocProperty>::createObject();
    ((OdDbDatabaseDimAssocProperty*)res.get())->init(__OD_T("DimAssoc"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseDimAssocProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<int>(pObj->getDIMASSOC());
  return eOk;
}

OdResult OdDbDatabaseDimAssocProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setDIMASSOC(val);
  return eOk;
}

/* OdDbDatabaseLayerNotifyProperty */

struct OdDbDatabaseLayerNotifyProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseLayerNotifyProperty>::createObject();
    ((OdDbDatabaseLayerNotifyProperty*)res.get())->init(__OD_T("LayerNotify"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseLayerNotifyProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<int>(pObj->getLAYERNOTIFY());
  return eOk;
}

OdResult OdDbDatabaseLayerNotifyProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setLAYERNOTIFY(val);
  return eOk;
}

/* OdDbDatabaseSortEntsProperty */

struct OdDbDatabaseSortEntsProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseSortEntsProperty>::createObject();
    ((OdDbDatabaseSortEntsProperty*)res.get())->init(__OD_T("SortEnts"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseSortEntsProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<int>(pObj->getSORTENTS());
  return eOk;
}

OdResult OdDbDatabaseSortEntsProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setSORTENTS(val);
  return eOk;
}

/* OdDbDatabaseLayerEvalProperty */

struct OdDbDatabaseLayerEvalProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseLayerEvalProperty>::createObject();
    ((OdDbDatabaseLayerEvalProperty*)res.get())->init(__OD_T("LayerEval"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseLayerEvalProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<int>(pObj->getLAYEREVAL());
  return eOk;
}

OdResult OdDbDatabaseLayerEvalProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setLAYEREVAL(val);
  return eOk;
}

/* OdDbDatabaseDgnFrameProperty */

struct OdDbDatabaseDgnFrameProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseDgnFrameProperty>::createObject();
    ((OdDbDatabaseDgnFrameProperty*)res.get())->init(__OD_T("DgnFrame"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseDgnFrameProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<int>(pObj->getDGNFRAME());
  return eOk;
}

OdResult OdDbDatabaseDgnFrameProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setDGNFRAME(val);
  return eOk;
}

/* OdDbDatabaseDwfFrameProperty */

struct OdDbDatabaseDwfFrameProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseDwfFrameProperty>::createObject();
    ((OdDbDatabaseDwfFrameProperty*)res.get())->init(__OD_T("DwfFrame"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseDwfFrameProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<int>(pObj->getDWFFRAME());
  return eOk;
}

OdResult OdDbDatabaseDwfFrameProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setDWFFRAME(val);
  return eOk;
}

/* OdDbDatabaseIndexctlProperty */

struct OdDbDatabaseIndexctlProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseIndexctlProperty>::createObject();
    ((OdDbDatabaseIndexctlProperty*)res.get())->init(__OD_T("Indexctl"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseIndexctlProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<int>(pObj->getINDEXCTL());
  return eOk;
}

OdResult OdDbDatabaseIndexctlProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setINDEXCTL(val);
  return eOk;
}

/* OdDbDatabasePdfFrameProperty */

struct OdDbDatabasePdfFrameProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabasePdfFrameProperty>::createObject();
    ((OdDbDatabasePdfFrameProperty*)res.get())->init(__OD_T("PdfFrame"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabasePdfFrameProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<int>(pObj->getPDFFRAME());
  return eOk;
}

OdResult OdDbDatabasePdfFrameProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setPDFFRAME(val);
  return eOk;
}

/* OdDbDatabaseMsOleScaleProperty */

struct OdDbDatabaseMsOleScaleProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseMsOleScaleProperty>::createObject();
    ((OdDbDatabaseMsOleScaleProperty*)res.get())->init(__OD_T("MsOleScale"), &OdRxValueType::Desc<double>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseMsOleScaleProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<double>(pObj->getMSOLESCALE());
  return eOk;
}

OdResult OdDbDatabaseMsOleScaleProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setMSOLESCALE(val);
  return eOk;
}

/* OdDbDatabaseInsunitsProperty */

struct OdDbDatabaseInsunitsProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseInsunitsProperty>::createObject();
    ((OdDbDatabaseInsunitsProperty*)res.get())->init(__OD_T("Insunits"), &OdRxValueType::Desc<OdDb::UnitsValue>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseInsunitsProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<OdDb::UnitsValue>(pObj->getINSUNITS());
  return eOk;
}

OdResult OdDbDatabaseInsunitsProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDb::UnitsValue val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setINSUNITS(val);
  return eOk;
}

/* OdDbDatabasePlotStyleModeProperty */

struct OdDbDatabasePlotStyleModeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabasePlotStyleModeProperty>::createObject();
    ((OdDbDatabasePlotStyleModeProperty*)res.get())->init(__OD_T("PlotStyleMode"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbDatabasePlotStyleModeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<bool>(pObj->getPSTYLEMODE());
  return eOk;
}

/* OdDbDatabaseTStackAlignProperty */

struct OdDbDatabaseTStackAlignProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseTStackAlignProperty>::createObject();
    ((OdDbDatabaseTStackAlignProperty*)res.get())->init(__OD_T("TStackAlign"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseTStackAlignProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<int>(pObj->getTSTACKALIGN());
  return eOk;
}

OdResult OdDbDatabaseTStackAlignProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setTSTACKALIGN(val);
  return eOk;
}

/* OdDbDatabaseViewportScaleDefaultProperty */

struct OdDbDatabaseViewportScaleDefaultProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseViewportScaleDefaultProperty>::createObject();
    ((OdDbDatabaseViewportScaleDefaultProperty*)res.get())->init(__OD_T("ViewportScaleDefault"), &OdRxValueType::Desc<double>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseViewportScaleDefaultProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<double>(pObj->getPSVPSCALE());
  return eOk;
}

OdResult OdDbDatabaseViewportScaleDefaultProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setPSVPSCALE(val);
  return eOk;
}

/* OdDbDatabaseTstackSizeProperty */

struct OdDbDatabaseTstackSizeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseTstackSizeProperty>::createObject();
    ((OdDbDatabaseTstackSizeProperty*)res.get())->init(__OD_T("TstackSize"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDatabaseTstackSizeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<int>(pObj->getTSTACKSIZE());
  return eOk;
}

OdResult OdDbDatabaseTstackSizeProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setTSTACKSIZE(val);
  return eOk;
}

/* OdDbDatabaseFileNameProperty */

struct OdDbDatabaseFileNameProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDatabaseFileNameProperty>::createObject();
    ((OdDbDatabaseFileNameProperty*)res.get())->init(__OD_T("FileName"), &OdRxValueType::Desc<OdString>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbDatabaseFileNameProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDatabasePtr pObj = OdDbDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<OdString>(pObj->getFilename());
  return eOk;
}

void createOdDbDatabaseProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDbDatabaseCelweightProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseThicknessProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseJoinStyleProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseEndCapsProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseFilletradProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseTextsizeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseLtscaleProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseSketchincProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseTracewidProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseLineWeightDisplayProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseUserr2Property::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseUserr1Property::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseUserr4Property::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseUserr3Property::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabasePlinewidProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseOleStartUpProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseXrefEditEnabledProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabasePdsizeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseAngbaseProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseSplinesegsProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseUseri5Property::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseSurfvProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseSurfuProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseUseri4Property::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseUseri1Property::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabasePdmodeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseUseri3Property::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseUseri2Property::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseSurftypeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseUnitmodeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseMeasurementProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseIsolinesProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseMaxactvpProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseShadedifProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseSurftab2Property::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseSurftab1Property::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseShadedgeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseSplinetypeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabasePextminProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabasePinsbaseProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabasePlimminProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabasePextmaxProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseCeltscaleProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseCmlstyleIDProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseInterferecolorProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseCmlscaleProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseCmljustProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabasePlimmaxProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseLayerTableIdProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseExtminProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseInsbaseProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabasePucsnameProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabasePucsorgProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabasePelevationProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabasePucsydirProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabasePucsxdirProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseFacetresProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseHpOriginProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseHpInheritProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseChamferdProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseChamferaProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseUserr5Property::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseChamfercProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseChamferbProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseCecolorProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseMLeaderstyleProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseTablestyleProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseInterferevpvsProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseInterfereobjvsProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseDimstyleProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseHandseedProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseCetransparencyProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseCeltypeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseTextstyleProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseByBlockLinetypeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseByLayerLinetypeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseCurrentSpaceIdProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabasePaperSpaceVportIdProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseContinuousLinetypeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseNamedObjectsDictionaryIdProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseColorDictionaryIdProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseLayerZeroProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabasePlotSettingsDictionaryIdProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseTableStyleDictionaryIdProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseOriginalFileNameProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseNumberOfSavesProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseOriginalFileVersionProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseLastSavedAsVersionProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseApproxNumObjectsProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseSectionManagerIdProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseMLeaderStyleDictionaryIdProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseSectionViewStyleDictionaryIdProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseDetailViewStyleDictionaryIdProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseDataLinkDictionaryIdProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseBlockTableIdProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseClayerProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseViewTableIdProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseTextStyleTableIdProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseTileModeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseLinetypeTableIdProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseUcsTableIdProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabasePlotStyleNameDictionaryIdProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseLayoutDictionaryIdProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseVisualStyleDictionaryIdProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseMaterialDictionaryIdProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseGroupDictionaryIdProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseRegAppTableIdProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseViewportTableIdProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseMLStyleDictionaryIdProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseDimStyleTableIdProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseAngdirProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseSkpolyProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseXclipFrameProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseSplframeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseUsrtimerProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseSaveproxygraphicsProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseLimcheckProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabasePlineEllipseProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseDispSilhProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseMirrtextProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseAunitsProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseLuprecProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseAttmodeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseAuprecProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseLunitsProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabasePlimcheckProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseWorldviewProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseTreedepthProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseVisretainProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseMaintenanceReleaseVersionProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseRetainOriginalThumbnailBitmapProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseXrefBlockIdProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseLastSavedAsMaintenanceVersionProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseOriginalFileSavedByVersionProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseOriginalFileSavedByMaintenanceVersionProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseOriginalFileMaintenanceVersionProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseIsEmrProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseFillmodeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseRegenmodeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabasePsltscaleProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseQtextmodeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseOrthomodeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseDimasoProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabasePlinegenProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseDimshoProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseExtmaxProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseTileModeLightSynchProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseAnnotativeDwgProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseNorthDirectionProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseLightGlyphDisplayProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseLightsInBlocksProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseAnnoAllVisibleProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseShadowPlaneLocationProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabasedragvsProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseObscuredColorProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseObscuredLineTypeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseHideTextProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseHaloGapProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseIntersectDisplayProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseLatitudeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseLongitudeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseIntersectColorProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseMsLtScaleProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseSolidHistProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseDxEvalProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabasePsolWidthProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseShowHistProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseStepSizeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseLensLengthProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseCameraHeightProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseStepsPerSecProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseCameraDisplayProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseLoftMag2Property::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseLoftMag1Property::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseCmaterialProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseCshadowProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseLoftAng2Property::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseLoftParamProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabasePsolHeightProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseLoftAng1Property::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseLoftNormalsProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabasePucsBaseProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseUpdateThumbnailProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseIsPartiallyOpenedProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseElevationProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseUcsorgProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseLimminProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseLimmaxProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseUcsnameProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseUcsBaseProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseUcsxdirProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseUcsydirProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseDimAssocProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseLayerNotifyProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseSortEntsProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseLayerEvalProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseDgnFrameProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseDwfFrameProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseIndexctlProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabasePdfFrameProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseMsOleScaleProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseInsunitsProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabasePlotStyleModeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseTStackAlignProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseViewportScaleDefaultProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseTstackSizeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDatabaseFileNameProperty::createObject(b.owner()));
  b.add(properties.last());
}
