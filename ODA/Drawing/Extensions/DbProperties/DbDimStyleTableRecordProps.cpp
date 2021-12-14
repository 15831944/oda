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
#include "DbDimStyleTableRecord.h"
#include "DbValueTypes.h"

/* OdDbDimStyleTableRecordIsModifiedForRecomputeProperty */

struct OdDbDimStyleTableRecordIsModifiedForRecomputeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimStyleTableRecordIsModifiedForRecomputeProperty>::createObject();
    ((OdDbDimStyleTableRecordIsModifiedForRecomputeProperty*)res.get())->init(__OD_T("IsModifiedForRecompute"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbDimStyleTableRecordIsModifiedForRecomputeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->isModifiedForRecompute());
  return eOk;
}

/* OdDbDimStyleTableRecordDimfracProperty */

struct OdDbDimStyleTableRecordDimfracProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimStyleTableRecordDimfracProperty>::createObject();
    ((OdDbDimStyleTableRecordDimfracProperty*)res.get())->init(__OD_T("Dimfrac"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimStyleTableRecordDimfracProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<int>(pObj->dimfrac());
  return eOk;
}

OdResult OdDbDimStyleTableRecordDimfracProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimfrac(val);
  return eOk;
}

/* OdDbDimStyleTableRecordDimexoProperty */

struct OdDbDimStyleTableRecordDimexoProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimStyleTableRecordDimexoProperty>::createObject();
    ((OdDbDimStyleTableRecordDimexoProperty*)res.get())->init(__OD_T("Dimexo"), &OdRxValueType::Desc<double>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimStyleTableRecordDimexoProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->dimexo());
  return eOk;
}

OdResult OdDbDimStyleTableRecordDimexoProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimexo(val);
  return eOk;
}

/* OdDbDimStyleTableRecordDimjogangProperty */

struct OdDbDimStyleTableRecordDimjogangProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimStyleTableRecordDimjogangProperty>::createObject();
    ((OdDbDimStyleTableRecordDimjogangProperty*)res.get())->init(__OD_T("Dimjogang"), &OdRxValueType::Desc<double>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimStyleTableRecordDimjogangProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->dimjogang());
  return eOk;
}

OdResult OdDbDimStyleTableRecordDimjogangProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimjogang(val);
  return eOk;
}

/* OdDbDimStyleTableRecordDimgapProperty */

struct OdDbDimStyleTableRecordDimgapProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimStyleTableRecordDimgapProperty>::createObject();
    ((OdDbDimStyleTableRecordDimgapProperty*)res.get())->init(__OD_T("Dimgap"), &OdRxValueType::Desc<double>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimStyleTableRecordDimgapProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->dimgap());
  return eOk;
}

OdResult OdDbDimStyleTableRecordDimgapProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimgap(val);
  return eOk;
}

/* OdDbDimStyleTableRecordDimexeProperty */

struct OdDbDimStyleTableRecordDimexeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimStyleTableRecordDimexeProperty>::createObject();
    ((OdDbDimStyleTableRecordDimexeProperty*)res.get())->init(__OD_T("Dimexe"), &OdRxValueType::Desc<double>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimStyleTableRecordDimexeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->dimexe());
  return eOk;
}

OdResult OdDbDimStyleTableRecordDimexeProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimexe(val);
  return eOk;
}

/* OdDbDimStyleTableRecordDimdleProperty */

struct OdDbDimStyleTableRecordDimdleProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimStyleTableRecordDimdleProperty>::createObject();
    ((OdDbDimStyleTableRecordDimdleProperty*)res.get())->init(__OD_T("Dimdle"), &OdRxValueType::Desc<double>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimStyleTableRecordDimdleProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->dimdle());
  return eOk;
}

OdResult OdDbDimStyleTableRecordDimdleProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimdle(val);
  return eOk;
}

/* OdDbDimStyleTableRecordDimdecProperty */

struct OdDbDimStyleTableRecordDimdecProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimStyleTableRecordDimdecProperty>::createObject();
    ((OdDbDimStyleTableRecordDimdecProperty*)res.get())->init(__OD_T("Dimdec"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimStyleTableRecordDimdecProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<int>(pObj->dimdec());
  return eOk;
}

OdResult OdDbDimStyleTableRecordDimdecProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimdec(val);
  return eOk;
}

/* OdDbDimStyleTableRecordDimdsepProperty */

struct OdDbDimStyleTableRecordDimdsepProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimStyleTableRecordDimdsepProperty>::createObject();
    ((OdDbDimStyleTableRecordDimdsepProperty*)res.get())->init(__OD_T("Dimdsep"), &OdRxValueType::Desc<OdInt16>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimStyleTableRecordDimdsepProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdInt16>(pObj->dimdsep());
  return eOk;
}

OdResult OdDbDimStyleTableRecordDimdsepProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdInt16 val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimdsep(val);
  return eOk;
}

/* OdDbDimStyleTableRecordDimdliProperty */

struct OdDbDimStyleTableRecordDimdliProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimStyleTableRecordDimdliProperty>::createObject();
    ((OdDbDimStyleTableRecordDimdliProperty*)res.get())->init(__OD_T("Dimdli"), &OdRxValueType::Desc<double>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimStyleTableRecordDimdliProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->dimdli());
  return eOk;
}

OdResult OdDbDimStyleTableRecordDimdliProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimdli(val);
  return eOk;
}

/* OdDbDimStyleTableRecordDimltypeProperty */

struct OdDbDimStyleTableRecordDimltypeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimStyleTableRecordDimltypeProperty>::createObject();
    ((OdDbDimStyleTableRecordDimltypeProperty*)res.get())->init(__OD_T("Dimltype"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbDimStyleTableRecordDimltypeProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimStyleTableRecordDimltypeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbObjectId>(pObj->dimltype());
  return eOk;
}

OdResult OdDbDimStyleTableRecordDimltypeProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbObjectId val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimltype(val);
  return eOk;
}

/* OdDbDimStyleTableRecordDimltex2Property */

struct OdDbDimStyleTableRecordDimltex2Property : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimStyleTableRecordDimltex2Property>::createObject();
    ((OdDbDimStyleTableRecordDimltex2Property*)res.get())->init(__OD_T("Dimltex2"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbDimStyleTableRecordDimltex2Property*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimStyleTableRecordDimltex2Property::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbObjectId>(pObj->dimltex2());
  return eOk;
}

OdResult OdDbDimStyleTableRecordDimltex2Property::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbObjectId val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimltex2(val);
  return eOk;
}

/* OdDbDimStyleTableRecordDimlwdProperty */

struct OdDbDimStyleTableRecordDimlwdProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimStyleTableRecordDimlwdProperty>::createObject();
    ((OdDbDimStyleTableRecordDimlwdProperty*)res.get())->init(__OD_T("Dimlwd"), &OdRxValueType::Desc<OdDb::LineWeight>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimStyleTableRecordDimlwdProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDb::LineWeight>(pObj->dimlwd());
  return eOk;
}

OdResult OdDbDimStyleTableRecordDimlwdProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDb::LineWeight val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimlwd(val);
  return eOk;
}

/* OdDbDimStyleTableRecordDimlunitProperty */

struct OdDbDimStyleTableRecordDimlunitProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimStyleTableRecordDimlunitProperty>::createObject();
    ((OdDbDimStyleTableRecordDimlunitProperty*)res.get())->init(__OD_T("Dimlunit"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimStyleTableRecordDimlunitProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<int>(pObj->dimlunit());
  return eOk;
}

OdResult OdDbDimStyleTableRecordDimlunitProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimlunit(val);
  return eOk;
}

/* OdDbDimStyleTableRecordDimltex1Property */

struct OdDbDimStyleTableRecordDimltex1Property : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimStyleTableRecordDimltex1Property>::createObject();
    ((OdDbDimStyleTableRecordDimltex1Property*)res.get())->init(__OD_T("Dimltex1"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbDimStyleTableRecordDimltex1Property*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimStyleTableRecordDimltex1Property::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbObjectId>(pObj->dimltex1());
  return eOk;
}

OdResult OdDbDimStyleTableRecordDimltex1Property::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbObjectId val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimltex1(val);
  return eOk;
}

/* OdDbDimStyleTableRecordDimldrblkProperty */

struct OdDbDimStyleTableRecordDimldrblkProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimStyleTableRecordDimldrblkProperty>::createObject();
    ((OdDbDimStyleTableRecordDimldrblkProperty*)res.get())->init(__OD_T("Dimldrblk"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbDimStyleTableRecordDimldrblkProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimStyleTableRecordDimldrblkProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbObjectId>(pObj->dimldrblk());
  return eOk;
}

OdResult OdDbDimStyleTableRecordDimldrblkProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbObjectId val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimldrblk(val);
  return eOk;
}

/* OdDbDimStyleTableRecordDimjustProperty */

struct OdDbDimStyleTableRecordDimjustProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimStyleTableRecordDimjustProperty>::createObject();
    ((OdDbDimStyleTableRecordDimjustProperty*)res.get())->init(__OD_T("Dimjust"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimStyleTableRecordDimjustProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<int>(pObj->dimjust());
  return eOk;
}

OdResult OdDbDimStyleTableRecordDimjustProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimjust(val);
  return eOk;
}

/* OdDbDimStyleTableRecordDimlimProperty */

struct OdDbDimStyleTableRecordDimlimProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimStyleTableRecordDimlimProperty>::createObject();
    ((OdDbDimStyleTableRecordDimlimProperty*)res.get())->init(__OD_T("Dimlim"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimStyleTableRecordDimlimProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->dimlim());
  return eOk;
}

OdResult OdDbDimStyleTableRecordDimlimProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimlim(val);
  return eOk;
}

/* OdDbDimStyleTableRecordDimlfacProperty */

struct OdDbDimStyleTableRecordDimlfacProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimStyleTableRecordDimlfacProperty>::createObject();
    ((OdDbDimStyleTableRecordDimlfacProperty*)res.get())->init(__OD_T("Dimlfac"), &OdRxValueType::Desc<double>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimStyleTableRecordDimlfacProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->dimlfac());
  return eOk;
}

OdResult OdDbDimStyleTableRecordDimlfacProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimlfac(val);
  return eOk;
}

/* OdDbDimStyleTableRecordDimclrtProperty */

struct OdDbDimStyleTableRecordDimclrtProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimStyleTableRecordDimclrtProperty>::createObject();
    ((OdDbDimStyleTableRecordDimclrtProperty*)res.get())->init(__OD_T("Dimclrt"), &OdRxValueType::Desc<OdCmColor>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimStyleTableRecordDimclrtProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdCmColor>(pObj->dimclrt());
  return eOk;
}

OdResult OdDbDimStyleTableRecordDimclrtProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdCmColor val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimclrt(val);
  return eOk;
}

/* OdDbDimStyleTableRecordDimalttdProperty */

struct OdDbDimStyleTableRecordDimalttdProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimStyleTableRecordDimalttdProperty>::createObject();
    ((OdDbDimStyleTableRecordDimalttdProperty*)res.get())->init(__OD_T("Dimalttd"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimStyleTableRecordDimalttdProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<int>(pObj->dimalttd());
  return eOk;
}

OdResult OdDbDimStyleTableRecordDimalttdProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimalttd(val);
  return eOk;
}

/* OdDbDimStyleTableRecordDimaltrndProperty */

struct OdDbDimStyleTableRecordDimaltrndProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimStyleTableRecordDimaltrndProperty>::createObject();
    ((OdDbDimStyleTableRecordDimaltrndProperty*)res.get())->init(__OD_T("Dimaltrnd"), &OdRxValueType::Desc<double>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimStyleTableRecordDimaltrndProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->dimaltrnd());
  return eOk;
}

OdResult OdDbDimStyleTableRecordDimaltrndProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimaltrnd(val);
  return eOk;
}

/* OdDbDimStyleTableRecordDimaltuProperty */

struct OdDbDimStyleTableRecordDimaltuProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimStyleTableRecordDimaltuProperty>::createObject();
    ((OdDbDimStyleTableRecordDimaltuProperty*)res.get())->init(__OD_T("Dimaltu"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimStyleTableRecordDimaltuProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<int>(pObj->dimaltu());
  return eOk;
}

OdResult OdDbDimStyleTableRecordDimaltuProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimaltu(val);
  return eOk;
}

/* OdDbDimStyleTableRecordDimalttzProperty */

struct OdDbDimStyleTableRecordDimalttzProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimStyleTableRecordDimalttzProperty>::createObject();
    ((OdDbDimStyleTableRecordDimalttzProperty*)res.get())->init(__OD_T("Dimalttz"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimStyleTableRecordDimalttzProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<int>(pObj->dimalttz());
  return eOk;
}

OdResult OdDbDimStyleTableRecordDimalttzProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimalttz(val);
  return eOk;
}

/* OdDbDimStyleTableRecordDimaltfProperty */

struct OdDbDimStyleTableRecordDimaltfProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimStyleTableRecordDimaltfProperty>::createObject();
    ((OdDbDimStyleTableRecordDimaltfProperty*)res.get())->init(__OD_T("Dimaltf"), &OdRxValueType::Desc<double>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimStyleTableRecordDimaltfProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->dimaltf());
  return eOk;
}

OdResult OdDbDimStyleTableRecordDimaltfProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimaltf(val);
  return eOk;
}

/* OdDbDimStyleTableRecordDimadecProperty */

struct OdDbDimStyleTableRecordDimadecProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimStyleTableRecordDimadecProperty>::createObject();
    ((OdDbDimStyleTableRecordDimadecProperty*)res.get())->init(__OD_T("Dimadec"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimStyleTableRecordDimadecProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<int>(pObj->dimadec());
  return eOk;
}

OdResult OdDbDimStyleTableRecordDimadecProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimadec(val);
  return eOk;
}

/* OdDbDimStyleTableRecordDimblk2Property */

struct OdDbDimStyleTableRecordDimblk2Property : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimStyleTableRecordDimblk2Property>::createObject();
    ((OdDbDimStyleTableRecordDimblk2Property*)res.get())->init(__OD_T("Dimblk2"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbDimStyleTableRecordDimblk2Property*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimStyleTableRecordDimblk2Property::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbObjectId>(pObj->dimblk2());
  return eOk;
}

OdResult OdDbDimStyleTableRecordDimblk2Property::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbObjectId val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimblk2(val);
  return eOk;
}

/* OdDbDimStyleTableRecordDimaltdProperty */

struct OdDbDimStyleTableRecordDimaltdProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimStyleTableRecordDimaltdProperty>::createObject();
    ((OdDbDimStyleTableRecordDimaltdProperty*)res.get())->init(__OD_T("Dimaltd"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimStyleTableRecordDimaltdProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<int>(pObj->dimaltd());
  return eOk;
}

OdResult OdDbDimStyleTableRecordDimaltdProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimaltd(val);
  return eOk;
}

/* OdDbDimStyleTableRecordDimaltProperty */

struct OdDbDimStyleTableRecordDimaltProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimStyleTableRecordDimaltProperty>::createObject();
    ((OdDbDimStyleTableRecordDimaltProperty*)res.get())->init(__OD_T("Dimalt"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimStyleTableRecordDimaltProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->dimalt());
  return eOk;
}

OdResult OdDbDimStyleTableRecordDimaltProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimalt(val);
  return eOk;
}

/* OdDbDimStyleTableRecordDimcenProperty */

struct OdDbDimStyleTableRecordDimcenProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimStyleTableRecordDimcenProperty>::createObject();
    ((OdDbDimStyleTableRecordDimcenProperty*)res.get())->init(__OD_T("Dimcen"), &OdRxValueType::Desc<double>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimStyleTableRecordDimcenProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->dimcen());
  return eOk;
}

OdResult OdDbDimStyleTableRecordDimcenProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimcen(val);
  return eOk;
}

/* OdDbDimStyleTableRecordDimazinProperty */

struct OdDbDimStyleTableRecordDimazinProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimStyleTableRecordDimazinProperty>::createObject();
    ((OdDbDimStyleTableRecordDimazinProperty*)res.get())->init(__OD_T("Dimazin"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimStyleTableRecordDimazinProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<int>(pObj->dimazin());
  return eOk;
}

OdResult OdDbDimStyleTableRecordDimazinProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimazin(val);
  return eOk;
}

/* OdDbDimStyleTableRecordDimclreProperty */

struct OdDbDimStyleTableRecordDimclreProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimStyleTableRecordDimclreProperty>::createObject();
    ((OdDbDimStyleTableRecordDimclreProperty*)res.get())->init(__OD_T("Dimclre"), &OdRxValueType::Desc<OdCmColor>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimStyleTableRecordDimclreProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdCmColor>(pObj->dimclre());
  return eOk;
}

OdResult OdDbDimStyleTableRecordDimclreProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdCmColor val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimclre(val);
  return eOk;
}

/* OdDbDimStyleTableRecordDimclrdProperty */

struct OdDbDimStyleTableRecordDimclrdProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimStyleTableRecordDimclrdProperty>::createObject();
    ((OdDbDimStyleTableRecordDimclrdProperty*)res.get())->init(__OD_T("Dimclrd"), &OdRxValueType::Desc<OdCmColor>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimStyleTableRecordDimclrdProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdCmColor>(pObj->dimclrd());
  return eOk;
}

OdResult OdDbDimStyleTableRecordDimclrdProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdCmColor val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimclrd(val);
  return eOk;
}

/* OdDbDimStyleTableRecordDimaunitProperty */

struct OdDbDimStyleTableRecordDimaunitProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimStyleTableRecordDimaunitProperty>::createObject();
    ((OdDbDimStyleTableRecordDimaunitProperty*)res.get())->init(__OD_T("Dimaunit"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimStyleTableRecordDimaunitProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<int>(pObj->dimaunit());
  return eOk;
}

OdResult OdDbDimStyleTableRecordDimaunitProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimaunit(val);
  return eOk;
}

/* OdDbDimStyleTableRecordDimarcsymProperty */

struct OdDbDimStyleTableRecordDimarcsymProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimStyleTableRecordDimarcsymProperty>::createObject();
    ((OdDbDimStyleTableRecordDimarcsymProperty*)res.get())->init(__OD_T("Dimarcsym"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimStyleTableRecordDimarcsymProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<int>(pObj->dimarcsym());
  return eOk;
}

OdResult OdDbDimStyleTableRecordDimarcsymProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimarcsym(val);
  return eOk;
}

/* OdDbDimStyleTableRecordDimaltzProperty */

struct OdDbDimStyleTableRecordDimaltzProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimStyleTableRecordDimaltzProperty>::createObject();
    ((OdDbDimStyleTableRecordDimaltzProperty*)res.get())->init(__OD_T("Dimaltz"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimStyleTableRecordDimaltzProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<int>(pObj->dimaltz());
  return eOk;
}

OdResult OdDbDimStyleTableRecordDimaltzProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimaltz(val);
  return eOk;
}

/* OdDbDimStyleTableRecordDimatfitProperty */

struct OdDbDimStyleTableRecordDimatfitProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimStyleTableRecordDimatfitProperty>::createObject();
    ((OdDbDimStyleTableRecordDimatfitProperty*)res.get())->init(__OD_T("Dimatfit"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimStyleTableRecordDimatfitProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<int>(pObj->dimatfit());
  return eOk;
}

OdResult OdDbDimStyleTableRecordDimatfitProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimatfit(val);
  return eOk;
}

/* OdDbDimStyleTableRecordDimaszProperty */

struct OdDbDimStyleTableRecordDimaszProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimStyleTableRecordDimaszProperty>::createObject();
    ((OdDbDimStyleTableRecordDimaszProperty*)res.get())->init(__OD_T("Dimasz"), &OdRxValueType::Desc<double>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimStyleTableRecordDimaszProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->dimasz());
  return eOk;
}

OdResult OdDbDimStyleTableRecordDimaszProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimasz(val);
  return eOk;
}

/* OdDbDimStyleTableRecordDimtxstyProperty */

struct OdDbDimStyleTableRecordDimtxstyProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimStyleTableRecordDimtxstyProperty>::createObject();
    ((OdDbDimStyleTableRecordDimtxstyProperty*)res.get())->init(__OD_T("Dimtxsty"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbDimStyleTableRecordDimtxstyProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimStyleTableRecordDimtxstyProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbObjectId>(pObj->dimtxsty());
  return eOk;
}

OdResult OdDbDimStyleTableRecordDimtxstyProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbObjectId val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimtxsty(val);
  return eOk;
}

/* OdDbDimStyleTableRecordDimtvpProperty */

struct OdDbDimStyleTableRecordDimtvpProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimStyleTableRecordDimtvpProperty>::createObject();
    ((OdDbDimStyleTableRecordDimtvpProperty*)res.get())->init(__OD_T("Dimtvp"), &OdRxValueType::Desc<double>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimStyleTableRecordDimtvpProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->dimtvp());
  return eOk;
}

OdResult OdDbDimStyleTableRecordDimtvpProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimtvp(val);
  return eOk;
}

/* OdDbDimStyleTableRecordDimtxtdirectionProperty */

struct OdDbDimStyleTableRecordDimtxtdirectionProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimStyleTableRecordDimtxtdirectionProperty>::createObject();
    ((OdDbDimStyleTableRecordDimtxtdirectionProperty*)res.get())->init(__OD_T("Dimtxtdirection"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimStyleTableRecordDimtxtdirectionProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->dimtxtdirection());
  return eOk;
}

OdResult OdDbDimStyleTableRecordDimtxtdirectionProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimtxtdirection(val);
  return eOk;
}

/* OdDbDimStyleTableRecordDimtxtProperty */

struct OdDbDimStyleTableRecordDimtxtProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimStyleTableRecordDimtxtProperty>::createObject();
    ((OdDbDimStyleTableRecordDimtxtProperty*)res.get())->init(__OD_T("Dimtxt"), &OdRxValueType::Desc<double>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimStyleTableRecordDimtxtProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->dimtxt());
  return eOk;
}

OdResult OdDbDimStyleTableRecordDimtxtProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimtxt(val);
  return eOk;
}

/* OdDbDimStyleTableRecordDimtszProperty */

struct OdDbDimStyleTableRecordDimtszProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimStyleTableRecordDimtszProperty>::createObject();
    ((OdDbDimStyleTableRecordDimtszProperty*)res.get())->init(__OD_T("Dimtsz"), &OdRxValueType::Desc<double>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimStyleTableRecordDimtszProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->dimtsz());
  return eOk;
}

OdResult OdDbDimStyleTableRecordDimtszProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimtsz(val);
  return eOk;
}

/* OdDbDimStyleTableRecordDimtolProperty */

struct OdDbDimStyleTableRecordDimtolProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimStyleTableRecordDimtolProperty>::createObject();
    ((OdDbDimStyleTableRecordDimtolProperty*)res.get())->init(__OD_T("Dimtol"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimStyleTableRecordDimtolProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->dimtol());
  return eOk;
}

OdResult OdDbDimStyleTableRecordDimtolProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimtol(val);
  return eOk;
}

/* OdDbDimStyleTableRecordDimtohProperty */

struct OdDbDimStyleTableRecordDimtohProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimStyleTableRecordDimtohProperty>::createObject();
    ((OdDbDimStyleTableRecordDimtohProperty*)res.get())->init(__OD_T("Dimtoh"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimStyleTableRecordDimtohProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->dimtoh());
  return eOk;
}

OdResult OdDbDimStyleTableRecordDimtohProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimtoh(val);
  return eOk;
}

/* OdDbDimStyleTableRecordDimtpProperty */

struct OdDbDimStyleTableRecordDimtpProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimStyleTableRecordDimtpProperty>::createObject();
    ((OdDbDimStyleTableRecordDimtpProperty*)res.get())->init(__OD_T("Dimtp"), &OdRxValueType::Desc<double>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimStyleTableRecordDimtpProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->dimtp());
  return eOk;
}

OdResult OdDbDimStyleTableRecordDimtpProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimtp(val);
  return eOk;
}

/* OdDbDimStyleTableRecordDimtoljProperty */

struct OdDbDimStyleTableRecordDimtoljProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimStyleTableRecordDimtoljProperty>::createObject();
    ((OdDbDimStyleTableRecordDimtoljProperty*)res.get())->init(__OD_T("Dimtolj"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimStyleTableRecordDimtoljProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<int>(pObj->dimtolj());
  return eOk;
}

OdResult OdDbDimStyleTableRecordDimtoljProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimtolj(val);
  return eOk;
}

/* OdDbDimStyleTableRecordDimfxlenProperty */

struct OdDbDimStyleTableRecordDimfxlenProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimStyleTableRecordDimfxlenProperty>::createObject();
    ((OdDbDimStyleTableRecordDimfxlenProperty*)res.get())->init(__OD_T("Dimfxlen"), &OdRxValueType::Desc<double>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimStyleTableRecordDimfxlenProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<double>(pObj->dimfxl());
  return eOk;
}

OdResult OdDbDimStyleTableRecordDimfxlenProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_FNAME
  pObj->setDimfxl(val);
  return eOk;
}

/* OdDbDimStyleTableRecordDimfxlenOnProperty */

struct OdDbDimStyleTableRecordDimfxlenOnProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimStyleTableRecordDimfxlenOnProperty>::createObject();
    ((OdDbDimStyleTableRecordDimfxlenOnProperty*)res.get())->init(__OD_T("DimfxlenOn"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimStyleTableRecordDimfxlenOnProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<bool>(pObj->dimfxlon());
  return eOk;
}

OdResult OdDbDimStyleTableRecordDimfxlenOnProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_FNAME
  pObj->setDimfxlon(val);
  return eOk;
}

/* OdDbDimStyleTableRecordDimblk1Property */

struct OdDbDimStyleTableRecordDimblk1Property : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimStyleTableRecordDimblk1Property>::createObject();
    ((OdDbDimStyleTableRecordDimblk1Property*)res.get())->init(__OD_T("Dimblk1"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbDimStyleTableRecordDimblk1Property*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimStyleTableRecordDimblk1Property::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbObjectId>(pObj->dimblk1());
  return eOk;
}

OdResult OdDbDimStyleTableRecordDimblk1Property::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbObjectId val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimblk1(val);
  return eOk;
}

/* OdDbDimStyleTableRecordDimblkProperty */

struct OdDbDimStyleTableRecordDimblkProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimStyleTableRecordDimblkProperty>::createObject();
    ((OdDbDimStyleTableRecordDimblkProperty*)res.get())->init(__OD_T("Dimblk"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbDimStyleTableRecordDimblkProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimStyleTableRecordDimblkProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbObjectId>(pObj->dimblk());
  return eOk;
}

OdResult OdDbDimStyleTableRecordDimblkProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbObjectId val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimblk(val);
  return eOk;
}

/* OdDbDimStyleTableRecordDimpostProperty */

struct OdDbDimStyleTableRecordDimpostProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimStyleTableRecordDimpostProperty>::createObject();
    ((OdDbDimStyleTableRecordDimpostProperty*)res.get())->init(__OD_T("Dimpost"), &OdRxValueType::Desc<OdString>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimStyleTableRecordDimpostProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdString>(pObj->dimpost());
  return eOk;
}

OdResult OdDbDimStyleTableRecordDimpostProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdString val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimpost(val);
  return eOk;
}

/* OdDbDimStyleTableRecordDimuptProperty */

struct OdDbDimStyleTableRecordDimuptProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimStyleTableRecordDimuptProperty>::createObject();
    ((OdDbDimStyleTableRecordDimuptProperty*)res.get())->init(__OD_T("Dimupt"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimStyleTableRecordDimuptProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->dimupt());
  return eOk;
}

OdResult OdDbDimStyleTableRecordDimuptProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimupt(val);
  return eOk;
}

/* OdDbDimStyleTableRecordDimtzinProperty */

struct OdDbDimStyleTableRecordDimtzinProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimStyleTableRecordDimtzinProperty>::createObject();
    ((OdDbDimStyleTableRecordDimtzinProperty*)res.get())->init(__OD_T("Dimtzin"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimStyleTableRecordDimtzinProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<int>(pObj->dimtzin());
  return eOk;
}

OdResult OdDbDimStyleTableRecordDimtzinProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimtzin(val);
  return eOk;
}

/* OdDbDimStyleTableRecordDimapostProperty */

struct OdDbDimStyleTableRecordDimapostProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimStyleTableRecordDimapostProperty>::createObject();
    ((OdDbDimStyleTableRecordDimapostProperty*)res.get())->init(__OD_T("Dimapost"), &OdRxValueType::Desc<OdString>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimStyleTableRecordDimapostProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdString>(pObj->dimapost());
  return eOk;
}

OdResult OdDbDimStyleTableRecordDimapostProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdString val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimapost(val);
  return eOk;
}

/* OdDbDimStyleTableRecordDimzinProperty */

struct OdDbDimStyleTableRecordDimzinProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimStyleTableRecordDimzinProperty>::createObject();
    ((OdDbDimStyleTableRecordDimzinProperty*)res.get())->init(__OD_T("Dimzin"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimStyleTableRecordDimzinProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<int>(pObj->dimzin());
  return eOk;
}

OdResult OdDbDimStyleTableRecordDimzinProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimzin(val);
  return eOk;
}

/* OdDbDimStyleTableRecordDimtoflProperty */

struct OdDbDimStyleTableRecordDimtoflProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimStyleTableRecordDimtoflProperty>::createObject();
    ((OdDbDimStyleTableRecordDimtoflProperty*)res.get())->init(__OD_T("Dimtofl"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimStyleTableRecordDimtoflProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->dimtofl());
  return eOk;
}

OdResult OdDbDimStyleTableRecordDimtoflProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimtofl(val);
  return eOk;
}

/* OdDbDimStyleTableRecordDimse1Property */

struct OdDbDimStyleTableRecordDimse1Property : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimStyleTableRecordDimse1Property>::createObject();
    ((OdDbDimStyleTableRecordDimse1Property*)res.get())->init(__OD_T("Dimse1"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimStyleTableRecordDimse1Property::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->dimse1());
  return eOk;
}

OdResult OdDbDimStyleTableRecordDimse1Property::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimse1(val);
  return eOk;
}

/* OdDbDimStyleTableRecordDimsd2Property */

struct OdDbDimStyleTableRecordDimsd2Property : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimStyleTableRecordDimsd2Property>::createObject();
    ((OdDbDimStyleTableRecordDimsd2Property*)res.get())->init(__OD_T("Dimsd2"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimStyleTableRecordDimsd2Property::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->dimsd2());
  return eOk;
}

OdResult OdDbDimStyleTableRecordDimsd2Property::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimsd2(val);
  return eOk;
}

/* OdDbDimStyleTableRecordDimsoxdProperty */

struct OdDbDimStyleTableRecordDimsoxdProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimStyleTableRecordDimsoxdProperty>::createObject();
    ((OdDbDimStyleTableRecordDimsoxdProperty*)res.get())->init(__OD_T("Dimsoxd"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimStyleTableRecordDimsoxdProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->dimsoxd());
  return eOk;
}

OdResult OdDbDimStyleTableRecordDimsoxdProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimsoxd(val);
  return eOk;
}

/* OdDbDimStyleTableRecordDimse2Property */

struct OdDbDimStyleTableRecordDimse2Property : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimStyleTableRecordDimse2Property>::createObject();
    ((OdDbDimStyleTableRecordDimse2Property*)res.get())->init(__OD_T("Dimse2"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimStyleTableRecordDimse2Property::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->dimse2());
  return eOk;
}

OdResult OdDbDimStyleTableRecordDimse2Property::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimse2(val);
  return eOk;
}

/* OdDbDimStyleTableRecordDimsd1Property */

struct OdDbDimStyleTableRecordDimsd1Property : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimStyleTableRecordDimsd1Property>::createObject();
    ((OdDbDimStyleTableRecordDimsd1Property*)res.get())->init(__OD_T("Dimsd1"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimStyleTableRecordDimsd1Property::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->dimsd1());
  return eOk;
}

OdResult OdDbDimStyleTableRecordDimsd1Property::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimsd1(val);
  return eOk;
}

/* OdDbDimStyleTableRecordDimrndProperty */

struct OdDbDimStyleTableRecordDimrndProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimStyleTableRecordDimrndProperty>::createObject();
    ((OdDbDimStyleTableRecordDimrndProperty*)res.get())->init(__OD_T("Dimrnd"), &OdRxValueType::Desc<double>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimStyleTableRecordDimrndProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->dimrnd());
  return eOk;
}

OdResult OdDbDimStyleTableRecordDimrndProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimrnd(val);
  return eOk;
}

/* OdDbDimStyleTableRecordDimlweProperty */

struct OdDbDimStyleTableRecordDimlweProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimStyleTableRecordDimlweProperty>::createObject();
    ((OdDbDimStyleTableRecordDimlweProperty*)res.get())->init(__OD_T("Dimlwe"), &OdRxValueType::Desc<OdDb::LineWeight>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimStyleTableRecordDimlweProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDb::LineWeight>(pObj->dimlwe());
  return eOk;
}

OdResult OdDbDimStyleTableRecordDimlweProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDb::LineWeight val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimlwe(val);
  return eOk;
}

/* OdDbDimStyleTableRecordDimscaleProperty */

struct OdDbDimStyleTableRecordDimscaleProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimStyleTableRecordDimscaleProperty>::createObject();
    ((OdDbDimStyleTableRecordDimscaleProperty*)res.get())->init(__OD_T("Dimscale"), &OdRxValueType::Desc<double>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimStyleTableRecordDimscaleProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->dimscale());
  return eOk;
}

OdResult OdDbDimStyleTableRecordDimscaleProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimscale(val);
  return eOk;
}

/* OdDbDimStyleTableRecordDimsahProperty */

struct OdDbDimStyleTableRecordDimsahProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimStyleTableRecordDimsahProperty>::createObject();
    ((OdDbDimStyleTableRecordDimsahProperty*)res.get())->init(__OD_T("Dimsah"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimStyleTableRecordDimsahProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->dimsah());
  return eOk;
}

OdResult OdDbDimStyleTableRecordDimsahProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimsah(val);
  return eOk;
}

/* OdDbDimStyleTableRecordDimtixProperty */

struct OdDbDimStyleTableRecordDimtixProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimStyleTableRecordDimtixProperty>::createObject();
    ((OdDbDimStyleTableRecordDimtixProperty*)res.get())->init(__OD_T("Dimtix"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimStyleTableRecordDimtixProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->dimtix());
  return eOk;
}

OdResult OdDbDimStyleTableRecordDimtixProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimtix(val);
  return eOk;
}

/* OdDbDimStyleTableRecordDimtihProperty */

struct OdDbDimStyleTableRecordDimtihProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimStyleTableRecordDimtihProperty>::createObject();
    ((OdDbDimStyleTableRecordDimtihProperty*)res.get())->init(__OD_T("Dimtih"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimStyleTableRecordDimtihProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->dimtih());
  return eOk;
}

OdResult OdDbDimStyleTableRecordDimtihProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimtih(val);
  return eOk;
}

/* OdDbDimStyleTableRecordDimtmoveProperty */

struct OdDbDimStyleTableRecordDimtmoveProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimStyleTableRecordDimtmoveProperty>::createObject();
    ((OdDbDimStyleTableRecordDimtmoveProperty*)res.get())->init(__OD_T("Dimtmove"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimStyleTableRecordDimtmoveProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<int>(pObj->dimtmove());
  return eOk;
}

OdResult OdDbDimStyleTableRecordDimtmoveProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimtmove(val);
  return eOk;
}

/* OdDbDimStyleTableRecordDimtmProperty */

struct OdDbDimStyleTableRecordDimtmProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimStyleTableRecordDimtmProperty>::createObject();
    ((OdDbDimStyleTableRecordDimtmProperty*)res.get())->init(__OD_T("Dimtm"), &OdRxValueType::Desc<double>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimStyleTableRecordDimtmProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->dimtm());
  return eOk;
}

OdResult OdDbDimStyleTableRecordDimtmProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimtm(val);
  return eOk;
}

/* OdDbDimStyleTableRecordDimtfillclrProperty */

struct OdDbDimStyleTableRecordDimtfillclrProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimStyleTableRecordDimtfillclrProperty>::createObject();
    ((OdDbDimStyleTableRecordDimtfillclrProperty*)res.get())->init(__OD_T("Dimtfillclr"), &OdRxValueType::Desc<OdCmColor>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimStyleTableRecordDimtfillclrProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdCmColor>(pObj->dimtfillclr());
  return eOk;
}

OdResult OdDbDimStyleTableRecordDimtfillclrProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdCmColor val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimtfillclr(val);
  return eOk;
}

/* OdDbDimStyleTableRecordDimtdecProperty */

struct OdDbDimStyleTableRecordDimtdecProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimStyleTableRecordDimtdecProperty>::createObject();
    ((OdDbDimStyleTableRecordDimtdecProperty*)res.get())->init(__OD_T("Dimtdec"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimStyleTableRecordDimtdecProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<int>(pObj->dimtdec());
  return eOk;
}

OdResult OdDbDimStyleTableRecordDimtdecProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimtdec(val);
  return eOk;
}

/* OdDbDimStyleTableRecordDimtadProperty */

struct OdDbDimStyleTableRecordDimtadProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimStyleTableRecordDimtadProperty>::createObject();
    ((OdDbDimStyleTableRecordDimtadProperty*)res.get())->init(__OD_T("Dimtad"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimStyleTableRecordDimtadProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<int>(pObj->dimtad());
  return eOk;
}

OdResult OdDbDimStyleTableRecordDimtadProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimtad(val);
  return eOk;
}

/* OdDbDimStyleTableRecordDimtfillProperty */

struct OdDbDimStyleTableRecordDimtfillProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimStyleTableRecordDimtfillProperty>::createObject();
    ((OdDbDimStyleTableRecordDimtfillProperty*)res.get())->init(__OD_T("Dimtfill"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimStyleTableRecordDimtfillProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<int>(pObj->dimtfill());
  return eOk;
}

OdResult OdDbDimStyleTableRecordDimtfillProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimtfill(val);
  return eOk;
}

/* OdDbDimStyleTableRecordDimtfacProperty */

struct OdDbDimStyleTableRecordDimtfacProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimStyleTableRecordDimtfacProperty>::createObject();
    ((OdDbDimStyleTableRecordDimtfacProperty*)res.get())->init(__OD_T("Dimtfac"), &OdRxValueType::Desc<double>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimStyleTableRecordDimtfacProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->dimtfac());
  return eOk;
}

OdResult OdDbDimStyleTableRecordDimtfacProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimStyleTableRecordPtr pObj = OdDbDimStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimtfac(val);
  return eOk;
}

void createOdDbDimStyleTableRecordProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDbDimStyleTableRecordIsModifiedForRecomputeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimStyleTableRecordDimfracProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimStyleTableRecordDimexoProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimStyleTableRecordDimjogangProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimStyleTableRecordDimgapProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimStyleTableRecordDimexeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimStyleTableRecordDimdleProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimStyleTableRecordDimdecProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimStyleTableRecordDimdsepProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimStyleTableRecordDimdliProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimStyleTableRecordDimltypeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimStyleTableRecordDimltex2Property::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimStyleTableRecordDimlwdProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimStyleTableRecordDimlunitProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimStyleTableRecordDimltex1Property::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimStyleTableRecordDimldrblkProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimStyleTableRecordDimjustProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimStyleTableRecordDimlimProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimStyleTableRecordDimlfacProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimStyleTableRecordDimclrtProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimStyleTableRecordDimalttdProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimStyleTableRecordDimaltrndProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimStyleTableRecordDimaltuProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimStyleTableRecordDimalttzProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimStyleTableRecordDimaltfProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimStyleTableRecordDimadecProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimStyleTableRecordDimblk2Property::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimStyleTableRecordDimaltdProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimStyleTableRecordDimaltProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimStyleTableRecordDimcenProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimStyleTableRecordDimazinProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimStyleTableRecordDimclreProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimStyleTableRecordDimclrdProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimStyleTableRecordDimaunitProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimStyleTableRecordDimarcsymProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimStyleTableRecordDimaltzProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimStyleTableRecordDimatfitProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimStyleTableRecordDimaszProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimStyleTableRecordDimtxstyProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimStyleTableRecordDimtvpProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimStyleTableRecordDimtxtdirectionProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimStyleTableRecordDimtxtProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimStyleTableRecordDimtszProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimStyleTableRecordDimtolProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimStyleTableRecordDimtohProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimStyleTableRecordDimtpProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimStyleTableRecordDimtoljProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimStyleTableRecordDimfxlenProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimStyleTableRecordDimfxlenOnProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimStyleTableRecordDimblk1Property::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimStyleTableRecordDimblkProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimStyleTableRecordDimpostProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimStyleTableRecordDimuptProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimStyleTableRecordDimtzinProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimStyleTableRecordDimapostProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimStyleTableRecordDimzinProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimStyleTableRecordDimtoflProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimStyleTableRecordDimse1Property::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimStyleTableRecordDimsd2Property::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimStyleTableRecordDimsoxdProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimStyleTableRecordDimse2Property::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimStyleTableRecordDimsd1Property::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimStyleTableRecordDimrndProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimStyleTableRecordDimlweProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimStyleTableRecordDimscaleProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimStyleTableRecordDimsahProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimStyleTableRecordDimtixProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimStyleTableRecordDimtihProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimStyleTableRecordDimtmoveProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimStyleTableRecordDimtmProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimStyleTableRecordDimtfillclrProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimStyleTableRecordDimtdecProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimStyleTableRecordDimtadProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimStyleTableRecordDimtfillProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimStyleTableRecordDimtfacProperty::createObject(b.owner()));
  b.add(properties.last());
}
