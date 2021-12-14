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
#include "DbEntity.h"
#include "DbValueTypes.h"
#include "DbAnnotativeObjectPE.h"
#include "DbSymUtl.h"
#include "DbObjectContextPE.h"

/* OdDbEntityODAUniqueIDProperty */

struct OdDbEntityODAUniqueIDProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbEntityODAUniqueIDProperty>::createObject();
    ((OdDbEntityODAUniqueIDProperty*)res.get())->init(__OD_T("ODAUniqueID"), &OdRxValueType::Desc<OdUInt64>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbEntityODAUniqueIDProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  if (pO == NULL)
    return eNullPtr;

  OdDbEntityPtr pObj = OdDbEntity::cast(pO);
  if (pObj.isNull())
    return eNotThatKindOfClass;

  OdUInt64 h = (OdUInt64)(pObj->objectId().getHandle());
  value = h;
  return eOk;
}

/* OdDbEntityNameProperty */

struct OdDbEntityNameProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbEntityNameProperty>::createObject();
    ((OdDbEntityNameProperty*)res.get())->init(__OD_T("Name"), &OdRxValueType::Desc<OdString>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbEntityNameProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  if (pO == NULL)
    return eNullPtr;

  OdDbEntityPtr pObj = OdDbEntity::cast(pO);
  if (pObj.isNull())
    return eNotThatKindOfClass;

  OdString name = pObj->isA()->name();
  if (name.find(L"AcDb") == 0)
    name.deleteChars(0, 4);
  value = name;

  return eOk;
}

/* OdDbEntityBlockIdProperty */

struct OdDbEntityBlockIdProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbEntityBlockIdProperty>::createObject();
    ((OdDbEntityBlockIdProperty*)res.get())->init(__OD_T("BlockId"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbEntityBlockIdProperty*)res.get())->attributes().add(OdRxRefersToAttribute::createObject(L"/BlockTableId/Items"));
    ((OdDbEntityBlockIdProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbEntityBlockIdProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbEntityPtr pObj = OdDbEntity::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbObjectId>(pObj->blockId());
  return eOk;
}

/* OdDbEntityLinetypeIdProperty */

struct OdDbEntityLinetypeIdProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbEntityLinetypeIdProperty>::createObject();
    ((OdDbEntityLinetypeIdProperty*)res.get())->init(__OD_T("LinetypeId"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbEntityLinetypeIdProperty*)res.get())->attributes().add(OdRxRefersToAttribute::createObject(L"/LinetypeTableId/Items"));
    ((OdDbEntityLinetypeIdProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbEntityLinetypeIdProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Linetype"));
    ((OdDbEntityLinetypeIdProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the current linetype of the object"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"General"))
      OdRxCategory::createObject(L"General", OdRxCategory::rootCategory());
    ((OdDbEntityLinetypeIdProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"General", 3));
    ((OdDbEntityLinetypeIdProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"Linetype"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbEntityLinetypeIdProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbEntityPtr pObj = OdDbEntity::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbObjectId>(pObj->linetypeId());
  return eOk;
}

OdResult OdDbEntityLinetypeIdProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbEntityPtr pObj = OdDbEntity::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbObjectId val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  OdResult rc = pObj->setLinetype(val);
  return rc;
}

/* OdDbEntityLayerIdProperty */

struct OdDbEntityLayerIdProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbEntityLayerIdProperty>::createObject();
    ((OdDbEntityLayerIdProperty*)res.get())->init(__OD_T("LayerId"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbEntityLayerIdProperty*)res.get())->attributes().add(OdRxRefersToAttribute::createObject(L"/LayerTableId/Items"));
    ((OdDbEntityLayerIdProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbEntityLayerIdProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Layer"));
    ((OdDbEntityLayerIdProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the current layer of the object"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"General"))
      OdRxCategory::createObject(L"General", OdRxCategory::rootCategory());
    ((OdDbEntityLayerIdProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"General", 2));
    ((OdDbEntityLayerIdProperty*)res.get())->attributes().add(OdRxLMVAttribute::createObject());
    ((OdDbEntityLayerIdProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"Layer"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbEntityLayerIdProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbEntityPtr pObj = OdDbEntity::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbObjectId>(pObj->layerId());
  return eOk;
}

OdResult OdDbEntityLayerIdProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbEntityPtr pObj = OdDbEntity::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbObjectId val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  OdResult rc = pObj->setLayer(val);
  return rc;
}

/* OdDbEntityLinetypeScaleProperty */

struct OdDbEntityLinetypeScaleProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbEntityLinetypeScaleProperty>::createObject();
    ((OdDbEntityLinetypeScaleProperty*)res.get())->init(__OD_T("LinetypeScale"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbEntityLinetypeScaleProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(0)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbEntityLinetypeScaleProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Linetype scale"));
    ((OdDbEntityLinetypeScaleProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the linetype scale factor of the object"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"General"))
      OdRxCategory::createObject(L"General", OdRxCategory::rootCategory());
    ((OdDbEntityLinetypeScaleProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"General", 4));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbEntityLinetypeScaleProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbEntityPtr pObj = OdDbEntity::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->linetypeScale());
  return eOk;
}

OdResult OdDbEntityLinetypeScaleProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbEntityPtr pObj = OdDbEntity::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  OdResult rc = pObj->setLinetypeScale(val);
  return rc;
}

/* OdDbEntityLineWeightProperty */

struct OdDbEntityLineWeightProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbEntityLineWeightProperty>::createObject();
    ((OdDbEntityLineWeightProperty*)res.get())->init(__OD_T("LineWeight"), &OdRxValueType::Desc<OdDb::LineWeight>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbEntityLineWeightProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Lineweight"));
    ((OdDbEntityLineWeightProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the lineweight for the object"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"General"))
      OdRxCategory::createObject(L"General", OdRxCategory::rootCategory());
    ((OdDbEntityLineWeightProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"General", 6));
    ((OdDbEntityLineWeightProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"Lineweight"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbEntityLineWeightProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbEntityPtr pObj = OdDbEntity::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDb::LineWeight>(pObj->lineWeight());
  return eOk;
}

OdResult OdDbEntityLineWeightProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbEntityPtr pObj = OdDbEntity::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDb::LineWeight val;
  if (!(value >> val))
    return eInvalidInput;
  OdResult rc = pObj->setLineWeight(val);
  return rc;
}

/* OdDbEntityVisibleProperty */

struct OdDbEntityVisibleProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbEntityVisibleProperty>::createObject();
    ((OdDbEntityVisibleProperty*)res.get())->init(__OD_T("Visible"), &OdRxValueType::Desc<OdDb::Visibility>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbEntityVisibleProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbEntityPtr pObj = OdDbEntity::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<OdDb::Visibility>(pObj->visibility());
  return eOk;
}

OdResult OdDbEntityVisibleProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbEntityPtr pObj = OdDbEntity::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDb::Visibility val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  OdResult rc = pObj->setVisibility(val);
  return rc;
}

/* OdDbEntityColorProperty */

struct OdDbEntityColorProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbEntityColorProperty>::createObject();
    ((OdDbEntityColorProperty*)res.get())->init(__OD_T("Color"), &OdRxValueType::Desc<OdCmColor>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbEntityColorProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Color"));
    ((OdDbEntityColorProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Returns the true color of the object."));
    if (!OdRxCategory::rootCategory()->findDescendant(L"General"))
      OdRxCategory::createObject(L"General", OdRxCategory::rootCategory());
    ((OdDbEntityColorProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"General", 1));
    ((OdDbEntityColorProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"TrueColor"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbEntityColorProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbEntityPtr pObj = OdDbEntity::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdCmColor>(pObj->color());
  return eOk;
}

OdResult OdDbEntityColorProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbEntityPtr pObj = OdDbEntity::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdCmColor val;
  if (!(value >> val))
    return eInvalidInput;
  OdResult rc = pObj->setColor(val);
  return rc;
}

/* OdDbEntityMaterialIdProperty */

struct OdDbEntityMaterialIdProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbEntityMaterialIdProperty>::createObject();
    ((OdDbEntityMaterialIdProperty*)res.get())->init(__OD_T("MaterialId"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbEntityMaterialIdProperty*)res.get())->attributes().add(OdRxRefersToAttribute::createObject(L"/MaterialDictionaryId/Items"));
    ((OdDbEntityMaterialIdProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbEntityMaterialIdProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Material"));
    ((OdDbEntityMaterialIdProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the material"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"3D Visualization"))
      OdRxCategory::createObject(L"3D Visualization", OdRxCategory::rootCategory());
    ((OdDbEntityMaterialIdProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"3D Visualization", 0));
    ((OdDbEntityMaterialIdProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"Material"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbEntityMaterialIdProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbEntityPtr pObj = OdDbEntity::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbObjectId>(pObj->materialId());
  return eOk;
}

OdResult OdDbEntityMaterialIdProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbEntityPtr pObj = OdDbEntity::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbObjectId val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  OdResult rc = pObj->setMaterial(val);
  return rc;
}

/* OdDbEntityTransparencyProperty */

struct OdDbEntityTransparencyProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbEntityTransparencyProperty>::createObject();
    ((OdDbEntityTransparencyProperty*)res.get())->init(__OD_T("Transparency"), &OdRxValueType::Desc<OdCmTransparency>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbEntityTransparencyProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Transparency"));
    ((OdDbEntityTransparencyProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the transparency of the object"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"General"))
      OdRxCategory::createObject(L"General", OdRxCategory::rootCategory());
    ((OdDbEntityTransparencyProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"General", 7));
    ((OdDbEntityTransparencyProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"EntityTransparency"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbEntityTransparencyProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbEntityPtr pObj = OdDbEntity::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdCmTransparency>(pObj->transparency());
  return eOk;
}

OdResult OdDbEntityTransparencyProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbEntityPtr pObj = OdDbEntity::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdCmTransparency val;
  if (!(value >> val))
    return eInvalidInput;
  OdResult rc = pObj->setTransparency(val);
  return rc;
}

/* OdDbEntityIsPlanarProperty */

struct OdDbEntityIsPlanarProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbEntityIsPlanarProperty>::createObject();
    ((OdDbEntityIsPlanarProperty*)res.get())->init(__OD_T("IsPlanar"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbEntityIsPlanarProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbEntityPtr pObj = OdDbEntity::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->isPlanar());
  return eOk;
}

/* OdDbEntityCollisionTypeProperty */

struct OdDbEntityCollisionTypeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbEntityCollisionTypeProperty>::createObject();
    ((OdDbEntityCollisionTypeProperty*)res.get())->init(__OD_T("CollisionType"), &OdRxValueType::Desc<OdDb::CollisionType>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbEntityCollisionTypeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbEntityPtr pObj = OdDbEntity::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDb::CollisionType>(pObj->collisionType());
  return eOk;
}

/* OdDbEntityCastShadowsProperty */

struct OdDbEntityCastShadowsProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbEntityCastShadowsProperty>::createObject();
    ((OdDbEntityCastShadowsProperty*)res.get())->init(__OD_T("CastShadows"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbEntityCastShadowsProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbEntityPtr pObj = OdDbEntity::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->castShadows());
  return eOk;
}

OdResult OdDbEntityCastShadowsProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbEntityPtr pObj = OdDbEntity::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  pObj->setCastShadows(val);
  return eOk;
}

/* OdDbEntityReceiveShadowsProperty */

struct OdDbEntityReceiveShadowsProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbEntityReceiveShadowsProperty>::createObject();
    ((OdDbEntityReceiveShadowsProperty*)res.get())->init(__OD_T("ReceiveShadows"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbEntityReceiveShadowsProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbEntityPtr pObj = OdDbEntity::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->receiveShadows());
  return eOk;
}

OdResult OdDbEntityReceiveShadowsProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbEntityPtr pObj = OdDbEntity::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  pObj->setReceiveShadows(val);
  return eOk;
}

/* OdDbEntityPlotStyleNameProperty */

struct OdDbEntityPlotStyleNameProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbEntityPlotStyleNameProperty>::createObject();
    ((OdDbEntityPlotStyleNameProperty*)res.get())->init(__OD_T("PlotStyleName"), &OdRxValueType::Desc<OdString>::value(), owner);
    ((OdDbEntityPlotStyleNameProperty*)res.get())->attributes().add(OdRxRefersToAttribute::createObject(L"/PlotStyleNameDictionaryId/Items"));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbEntityPlotStyleNameProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Plot style"));
    ((OdDbEntityPlotStyleNameProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the plotstyle name for the object"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"General"))
      OdRxCategory::createObject(L"General", OdRxCategory::rootCategory());
    ((OdDbEntityPlotStyleNameProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"General", 5));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbEntityPlotStyleNameProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbEntityPtr pObj = OdDbEntity::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdString>(pObj->plotStyleName());
  return eOk;
}

OdResult OdDbEntityPlotStyleNameProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbEntityPtr pObj = OdDbEntity::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdString val;
  if (!(value >> val))
    return eInvalidInput;
  OdResult rc = pObj->setPlotStyleName(val);
  return rc;
}

/* OdDbEntityAnnotativeScaleProperty */

struct OdDbEntityAnnotativeScaleProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbEntityAnnotativeScaleProperty>::createObject();
    ((OdDbEntityAnnotativeScaleProperty*)res.get())->init(__OD_T("AnnotativeScale"), &OdRxValueType::Desc<OdString>::value(), owner);
    if (!OdRxCategory::rootCategory()->findDescendant(L"AnnotationScaling"))
      OdRxCategory::createObject(L"AnnotationScaling", OdRxCategory::rootCategory());
    ((OdDbEntityAnnotativeScaleProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"AnnotationScaling", 3));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbEntityAnnotativeScaleProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Annotative scale"));
    ((OdDbEntityAnnotativeScaleProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the annotative scale"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbEntityAnnotativeScaleProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  //>>REPLACE_ALL
  OdDbEntityPtr pObj = OdDbEntity::cast(pO);
  OdDbDatabasePtr pDb = pObj->database();
  OdDbObjectContextPEPtr a = OdDbObjectContextPE::cast(pO);
  if (pObj.isNull() || a.isNull() || pDb.isNull())
    return eNotApplicable;
  OdDbObjectContextDataPtr pCtx = a->getContextData(pObj, *(pDb->getCANNOSCALE()));
  if (pCtx.isNull())
    pCtx = a->getDefaultContextData(pObj, ODDB_ANNOTATIONSCALES_COLLECTION);

  const OdDbObjectContext* ctx = pCtx->context();
  value = ctx->getName();

  return eOk;
  //<<REPLACE_ALL
}

void createOdDbEntityProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDbEntityODAUniqueIDProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbEntityNameProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbEntityBlockIdProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbEntityLinetypeIdProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbEntityLayerIdProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbEntityLinetypeScaleProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbEntityLineWeightProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbEntityVisibleProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbEntityColorProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbEntityMaterialIdProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbEntityTransparencyProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbEntityIsPlanarProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbEntityCollisionTypeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbEntityCastShadowsProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbEntityReceiveShadowsProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbEntityPlotStyleNameProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbEntityAnnotativeScaleProperty::createObject(b.owner()));
  b.add(properties.last());
}
