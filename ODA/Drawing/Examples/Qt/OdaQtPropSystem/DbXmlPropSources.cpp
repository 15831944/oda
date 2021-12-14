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
//
// DbXmlPropSources.cpp
//

#include "OdaCommon.h"
#include "DbXmlPropSources.h"
#include "OdDbStub.h"
#include "DbDictionary.h"
#include "DbTextStyleTable.h"
#include "DbScale.h"
#include "Ge/GeScale3d.h"
#include "OdDToStr.h"

#include "DbBlockReference.h"
#include "DbAnnotativeObjectPE.h"

#include "DbMLeader.h"
#include "DbSymbolTableRecord.h"
#include "DbBlockTableRecord.h"
#include "DbObjectContextManager.h"
#include "DbObjectContextCollection.h"
#include "DbSymUtl.h"
//#include "DbAnnotativeObjectPE.h"
//#include "DbObjectContextInterface.h"

#include "ModelDocObj/DbViewBorder.h"
#include "ModelDocObj/DbViewRep.h"
#include "DbViewport.h"
#include "DbAssocManager.h"
#include "DbAssocDependency.h"

#include "DbAssocArrayActionBody.h"
#include "DbAssocArrayCommonParameters.h"
#include "DbAssocArrayRectangularParameters.h"
#include "DbAssocArrayPolarParameters.h"
#include "DbAssocArrayPathParameters.h"

#include "DbSmartCenterActionBody.h" // #include "DbSmartCenterObjectsParameters.h"
#include "DbAssocArrayModifyActionBody.h"
#include "DbAssocSurfacesParameters.h"

#include "RxMember.h"
#include "RxValue.h" //#include "RxValueType.h"
#include "RxCategory.h"
#include "RxMethod.h"
#include "RxProperty.h"
#include "RxAttribute.h"
#define STL_USING_MAP
#include "OdaSTL.h"
#include "ExtDbModule.h"

///////////////////////////////////////////////////////////////////////////////

template <class TStyle>
OdString dicRecIdToStr(OdDbObjectId idStyle, const OdString& sDefVal = OdString::kEmpty)
{
  if (idStyle.isNull())
    return sDefVal;
  OdSmartPtr<TStyle> pStyle = TStyle::cast(idStyle.openObject());
  if (pStyle.isNull())
    return sDefVal;
  return pStyle->getName();
}
#define styleIdToStr dicRecIdToStr<OdDbTextStyleTableRecord>

template <class TStyle>
OdDbObjectId strToDicRecId(OdDbObjectId idDic, const OdString& sValue)
{
  if (idDic.isNull() || sValue.isEmpty())
    return OdDbObjectId();
  OdDbDictionaryPtr pDic = OdDbDictionary::cast(idDic.openObject());
  if (pDic.isNull())
    return OdDbObjectId();
  for (OdDbDictionaryIteratorPtr pItr = pDic->newIterator(); !pItr->done(); pItr->next())
  {
    OdSmartPtr<TStyle> pStyle = TStyle::cast(pItr->getObject());
    if (pStyle.isNull())
      continue;
    OdString sName = pStyle->getName();
    ODA_ASSERT_ONCE(!sName.isEmpty());
    if (sName != sValue)
      continue;
    return pStyle->objectId();
  }
  return OdDbObjectId();
}

template <class TStyle>
OdDbObjectId strToStyleId(OdDbObjectId idDic, const OdString& sValue)
{
  if (idDic.isNull() || sValue.isEmpty())
    return OdDbObjectId();
  OdDbSymbolTablePtr pTbl = OdDbSymbolTable::cast(idDic.openObject());
  if (pTbl.isNull())
    return OdDbObjectId();
  for (OdDbSymbolTableIteratorPtr pItr = pTbl->newIterator(); !pItr->done(); pItr->step())
  {
    OdSmartPtr<TStyle> pStyle = TStyle::cast(pItr->getRecord());
    if (pStyle.isNull())
      continue;
    OdString sName = pStyle->getName();
    ODA_ASSERT_ONCE(!sName.isEmpty());
    if (sName != sValue)
      continue;
    return pStyle->objectId();
  }
  return OdDbObjectId();
}

template <class TStyle>
bool getDicRecNames(OdDbObjectId idDic, OdStringArray& values)
{
  if (idDic.isNull())
    return false;
  OdDbDictionaryPtr pDic = OdDbDictionary::cast(idDic.openObject());
  if (pDic.isNull())
    return false;
  for (OdDbDictionaryIteratorPtr pItr = pDic->newIterator(); !pItr->done(); pItr->next())
  {
    OdSmartPtr<TStyle> pStyle = TStyle::cast(pItr->getObject());
    if (pStyle.isNull())
      continue;
    OdString sName = pStyle->getName();
    ODA_ASSERT_ONCE(!sName.isEmpty());
    if (sName.isEmpty())
      continue;
    values.push_back(sName);
  }
  return true;
}

template <class TStyle>
bool getStyleNames(OdDbObjectId idDic, OdStringArray& values, OdDbObjectIdArray* pIds = NULL)
{
  if (idDic.isNull())
    return false;
  OdDbSymbolTablePtr pTbl = OdDbSymbolTable::cast(idDic.openObject());
  if (pTbl.isNull())
    return false;
  for (OdDbSymbolTableIteratorPtr pItr = pTbl->newIterator(); !pItr->done(); pItr->step())
  {
    OdSmartPtr<TStyle> pStyle = TStyle::cast(pItr->getRecord());
    if (pStyle.isNull())
      continue;
    OdString sName = pStyle->getName();
    ODA_ASSERT_ONCE(!sName.isEmpty());
    if (sName.isEmpty())
      continue;
    values.push_back(sName);
    if (pIds)
      pIds->push_back(pStyle->objectId());
  }
  return true;
}

//#define arrowBlockIdToStr dicRecIdToStr<OdDbBlockTableRecord>
OdString arrowBlockIdToStr(OdDbMLeader* pLdr)
{
  OdDbObjectId idArrow = pLdr->arrowSymbolId();
  //if (idArrow.isNull())
  //{
  //  OdDbMLeaderStylePtr pStyle = OdDbMLeaderStyle::cast(pLdr->MLeaderStyle().openObject());
  //  if (pStyle.get())
  //    idArrow = pStyle->arrowSymbolId();
  //  //dimldrblk
  //}

  OdString sName = OdDbSymUtil::getSymbolName(idArrow);
  if (sName.isEmpty())
    sName = L"Closed filled"; // integrated value by NULL value
  else
  {
    if (!sName.isEmpty() && sName.getAt(0) == L'_')
      sName = sName.mid(1);
  }
  return sName;
}

bool strToArrowBlockId(OdDbMLeader* pLdr, const OdString& sValue)
{
  if (sValue == L"Closed filled") // integrated value by NULL value
  {
    OdDbObjectId idArrow;
    pLdr->setArrowSymbolId(idArrow);
    return true;
  }
  OdString sName; sName.format(L"_%ls", sValue.c_str());
  OdDbDatabase* pDb = pLdr->database();
  OdStringArray vals;
  OdDbObjectIdArray ids;
  unsigned int findIndex = -1;
  if (   !getStyleNames<OdDbBlockTableRecord>(pDb->getBlockTableId(), vals, &ids)
      || !vals.find(sName, findIndex))
  {
    // TODO create with accordant content if block is missing
    ODA_FAIL_ONCE(); // TODO
    return false;
  }
  ODA_ASSERT_ONCE(findIndex < ids.size());
  OdDbObjectId idArrow = ids[findIndex];
  if (idArrow != pLdr->arrowSymbolId())
    pLdr->setArrowSymbolId(idArrow);
  return true;
}

bool getArrowBlockNames(OdRxObject* pRxDb, OdStringArray& values)
{
  OdDbDatabase* pDb = OdDbDatabase::cast(pRxDb);
  ODA_ASSERT_ONCE(pDb);
  OdStringArray vals;
  if (!getStyleNames<OdDbBlockTableRecord>(pDb->getBlockTableId(), vals))
    return false;
  values.push_back(L"Closed filled"); // integrated value by NULL value
  // testAnnotativeML_acad_true_Dot+None.dwg :
  // It looks like ACAD add blocks with preffix _ (_Dot, _None) by set in property palette // TODO
  // next is only to see it
  for (OdUInt32 idx = 0, sz = vals.size(); idx < sz; idx++)
  {
    OdString sName = vals.getAt(idx);
    if (sName.isEmpty() || sName.getAt(0) != L'_')
      continue;
    sName = sName.mid(1);
    values.push_back(sName);
  }
  return true;
}

OdString blockIdToStr(OdDbMLeader* pLdr)
{
  OdDbObjectId idBlock = pLdr->blockContentId();
  OdString sName = OdDbSymUtil::getSymbolName(idBlock);
  if (!sName.find(L"_Tag"))
    sName = sName.mid(4);
  return sName;
}

bool strToBlockId(OdDbMLeader* pLdr, const OdString& sValue)
{
  OdString sNamePrev = blockIdToStr(pLdr);
  if (sValue == sNamePrev)
    return false;

  OdStringArray vals; OdDbObjectIdArray ids;
  if (!getStyleNames<OdDbBlockTableRecord>(pLdr->database()->getBlockTableId(), vals, &ids))
    return false;
  for (OdUInt32 idx = 0, sz = vals.size(); idx < sz; idx++)
  {
    OdDbBlockTableRecordPtr pBlk = OdDbBlockTableRecord::cast(ids[idx].openObject());
    ODA_ASSERT_ONCE(pBlk.get());
    if (pBlk.isNull() || pBlk->isLayout())
      continue;
    OdString sName = vals.getAt(idx);
    if (sName.isEmpty())
      continue;
    if (!sName.find(L"_Tag"))
      sName = sName.mid(4);
    if (sName.getAt(0) == L'_')
      continue; // skip arrows
    if (sName != sValue)
      continue;
    OdDbObjectId idBlock = ids[idx];
    pLdr->setBlockContentId(idBlock);
    return true;
  }
  // Generate _Tag block and set it
  ODA_FAIL_ONCE(); // TODO
  return false;
}

bool getBlockNames(OdRxObject* pRxDb, OdStringArray& values)
{
  OdDbDatabase* pDb = OdDbDatabase::cast(pRxDb);
  ODA_ASSERT_ONCE(pDb);
  OdStringArray vals; OdDbObjectIdArray ids;
  if (!getStyleNames<OdDbBlockTableRecord>(pDb->getBlockTableId(), vals, &ids))
    return false;
  for (OdUInt32 idx = 0, sz = vals.size(); idx < sz; idx++)
  {
    OdDbBlockTableRecordPtr pBlk = OdDbBlockTableRecord::cast(ids[idx].openObject());
    ODA_ASSERT_ONCE(pBlk.get());
    if (pBlk.isNull() || pBlk->isLayout())
      continue;
    OdString sName = vals.getAt(idx);
    if (sName.isEmpty())
      continue;
    if (!sName.find(L"_Tag"))
      sName = sName.mid(4);
    values.push_back(sName);
  }
  return true;
}

OdString leaderLineTypeToStr(OdDbMLeaderStyle::LeaderType lineType, const OdString& sDefVal = OdString::kEmpty)
{
  switch (lineType)
  {
  case OdDbMLeaderStyle::kInVisibleLeader: return L"None";
  case OdDbMLeaderStyle::kStraightLeader: return L"Straight";
  case OdDbMLeaderStyle::kSplineLeader: return L"Spline";
  }
  return sDefVal;
}

OdDbMLeaderStyle::LeaderType strToLeaderLineType(const OdString& sValue, OdDbMLeaderStyle::LeaderType typeDefVal)
{
  if (sValue == L"None")
    return OdDbMLeaderStyle::kInVisibleLeader;
  if (sValue == L"Straight")
    return OdDbMLeaderStyle::kStraightLeader;
  if (sValue == L"Spline")
    return OdDbMLeaderStyle::kSplineLeader;
  return typeDefVal;
}

bool getLeaderLineTypes(OdStringArray& values)
{
  values.push_back(L"Straight"); values.push_back(L"Spline"); values.push_back(L"None");
  return true;
}

OdString textAlignmentTypeToStr(OdDbMLeader* pLdr, bool bByText = true, const OdString& sDefVal = OdString::kEmpty)
{
  // TOCHECK  m_Justification // TextJustifycation (1 = Left, 2 = Center, 3 = Right)
  OdDbMTextPtr pMText;
  if (bByText && (pMText = pLdr->mtext()).get())
  {
    // testAnnotativeML_oda_true.dwg
    //ODA_ASSERT_VAR(OdDbMText::FlowDirection dr = pMText->flowDirection();)
    //switch (dr)
    //{
    //case OdDbMText::kLtoR: // 1 // Left to Right
    //case OdDbMText::kRtoL: // 2 // Right to Left
    //  return L"Horizontal";
    //case OdDbMText::kTtoB: // 3 // Top to Bottom
    //case OdDbMText::kBtoT: // 4 // Bottom to Top
    //  return L"Vertical";
    //}
    OdDb::TextHorzMode mode = pMText->horizontalMode();
    switch (mode)
    {
    case OdDb::kTextLeft:
      return L"Left";
    case OdDb::kTextCenter:
      return L"Center";
    case OdDb::kTextRight:
      return L"Right";
      // default:
      //OdDb::kTextAlign OdDb::kTextMid OdDb::kTextFit
    }
  }

  OdDbMLeaderStyle::TextAlignmentType textAlignmentType = pLdr->textAlignmentType();
  switch (textAlignmentType)
  {
  case OdDbMLeaderStyle::kLeftAlignment: return L"Left";
  case OdDbMLeaderStyle::kCenterAlignment: return L"Center";
  case OdDbMLeaderStyle::kRightAlignment: return L"Right";
  }
  return sDefVal;
}

OdDbMLeaderStyle::TextAlignmentType strToTextAlignmentType(const OdString& sValue, 
                                                           OdDbMLeaderStyle::TextAlignmentType typeDefVal)
{
  if (sValue == L"Left") 
    return OdDbMLeaderStyle::kLeftAlignment;
  if (sValue == L"Center")
    return OdDbMLeaderStyle::kCenterAlignment;
  if (sValue == L"Right")
    return OdDbMLeaderStyle::kRightAlignment;
  return typeDefVal;
}

bool getTextAlignmentTypes(OdStringArray& values)
{
  values.push_back(L"Left"); values.push_back(L"Center"); values.push_back(L"Right");
  return true;
}

OdString textDirectionToStr(OdDbMLeader* pMLeader)
{
  // see 85866-mleader1_acad2013_part_1.dwg of CORE-14092
  OdDbMText::FlowDirection dr = pMLeader->mtext()->flowDirection();
  switch (dr)
  {
  case OdDbMText::kLtoR: // 1 // Left to Right
  case OdDbMText::kRtoL: // 2 // Right to Left
    return L"Horizontal";
  case OdDbMText::kTtoB: // 3 // Top to Bottom
  case OdDbMText::kBtoT: // 4 // Bottom to Top
    return L"Vertical";
  }
  ODA_ASSERT_ONCE(dr == OdDbMText::kByStyle) //5 // By Style
  return L"By style";
}

OdString textAttachmentDirectionToStr(OdDbMLeader* pMLeader)
{
  //OdDbMText::FlowDirection dr = pMLeader->mtext()->flowDirection(); // (5 = ByStyle, 3 = Vertical, 1 = Horizontal)
  //if (   pMLeader->isOverride(OdDbMLeader::kTextAttachmentDirection)
  //    || dr == OdDbMText::kByStyle)
  {
    OdDbMLeaderStyle::TextAttachmentDirection textAttachmentDirection = pMLeader->textAttachmentDirection();
    switch (textAttachmentDirection)
    {
    case OdDbMLeaderStyle::kAttachmentHorizontal: 
      return L"Horizontal";
    case OdDbMLeaderStyle::kAttachmentVertical: 
      return L"Vertical";
    }
  }
  ODA_FAIL_ONCE();
  return textDirectionToStr(pMLeader);
}

bool strToTextAttachmentDirection(OdDbMLeader* pMLeader, const OdString& sValue)
{
  if (sValue == textAttachmentDirectionToStr(pMLeader))
    return false;

  bool bOver = pMLeader->isOverride(OdDbMLeader::kTextAttachmentDirection);
  if (sValue == L"Horizontal")
  {
    pMLeader->setTextAttachmentDirection(OdDbMLeaderStyle::kAttachmentHorizontal);
    ODA_ASSERT_VAR(if (pMLeader->isOverride(OdDbMLeader::kTextAttachmentDirection) != bOver))
      pMLeader->setOverride(OdDbMLeader::kTextAttachmentDirection, bOver);
    return true;
  }
  if (sValue == L"Vertical")
  {
    pMLeader->setTextAttachmentDirection(OdDbMLeaderStyle::kAttachmentVertical);
    ODA_ASSERT_VAR(if (!pMLeader->isOverride(OdDbMLeader::kTextAttachmentDirection) != bOver))
      pMLeader->setOverride(OdDbMLeader::kTextAttachmentDirection, bOver);
    return true;
  }
  if (sValue == L"By style")
  {
    OdDbMLeaderStylePtr pStyle = OdDbMLeaderStyle::cast(pMLeader->MLeaderStyle().openObject());
    if (pStyle.get() && pStyle->textAttachmentDirection() != pMLeader->textAttachmentDirection())
      pMLeader->setTextAttachmentDirection(pStyle->textAttachmentDirection());
    ODA_ASSERT_VAR(if (pMLeader->isOverride(OdDbMLeader::kTextAttachmentDirection)))
      pMLeader->setOverride(OdDbMLeader::kTextAttachmentDirection, false);
    return true;
  }
  ODA_FAIL_ONCE();
  return false;
}

bool strToTextDirection(OdDbMLeader* pMLeader, const OdString& sValue)
{
  if (sValue == textDirectionToStr(pMLeader))
    return false;

  OdDbMTextPtr pMText = pMLeader->mtext();
  //bool bOver = pMLeader->isOverride(OdDbMLeader::kTextAttachmentDirection);
  //if (!bAttachmentDir)
  //  bOver = true;
  if (sValue == L"Horizontal")
  {
    //pMLeader->setTextAttachmentDirection(OdDbMLeaderStyle::kAttachmentHorizontal);
    //ODA_ASSERT_VAR(if (pMLeader->isOverride(OdDbMLeader::kTextAttachmentDirection) != bOver))
    //  pMLeader->setOverride(OdDbMLeader::kTextAttachmentDirection, bOver);
    pMText->setFlowDirection(OdDbMText::kLtoR); // 1 // Left to Right
    pMLeader->setMText(pMText);
    return true;
  }
  if (sValue == L"Vertical")
  {
    //pMLeader->setTextAttachmentDirection(OdDbMLeaderStyle::kAttachmentVertical);
    //ODA_ASSERT_VAR(if (!pMLeader->isOverride(OdDbMLeader::kTextAttachmentDirection) != bOver))
    //  pMLeader->setOverride(OdDbMLeader::kTextAttachmentDirection, bOver);
    pMText->setFlowDirection(OdDbMText::kTtoB); // 3 // Top to Bottom
    pMLeader->setMText(pMText);
    return true;
  }
  if (sValue == L"By style")
  {
    //OdDbMLeaderStylePtr pStyle = OdDbMLeaderStyle::cast(pMLeader->MLeaderStyle().openObject());
    //if (pStyle.get() && pStyle->textAttachmentDirection() != pMLeader->textAttachmentDirection())
    //  pMLeader->setTextAttachmentDirection(pStyle->textAttachmentDirection());
    //ODA_ASSERT_VAR(if (pMLeader->isOverride(OdDbMLeader::kTextAttachmentDirection)))
    //  pMLeader->setOverride(OdDbMLeader::kTextAttachmentDirection, false);
    pMText->setFlowDirection(OdDbMText::kByStyle); //5 // By Style
    pMLeader->setMText(pMText);
    return true;
  }
  ODA_FAIL_ONCE();
  return false;
}

bool getTextAttachmentDirections(OdStringArray& values)
{
  values.push_back(L"Horizontal"); values.push_back(L"Vertical"); 
  return true;
}

bool getTextDirections(OdStringArray& values)
{
  if (getTextAttachmentDirections(values))
    values.push_back(L"By style");
  return true;
}

OdString textAttachmentTypeToStr(OdDbMLeaderStyle::TextAttachmentType typeAttach)
{
  switch (typeAttach)
  {
  case OdDbMLeaderStyle::kAttachmentTopOfTop:
    return L"Top of top line";
  case OdDbMLeaderStyle::kAttachmentMiddleOfTop:
    return L"Middle of top line";
  case OdDbMLeaderStyle::kAttachmentMiddle:
    return L"Middle of text";
  case OdDbMLeaderStyle::kAttachmentMiddleOfBottom:
    return L"Middle of bottom line";
  case OdDbMLeaderStyle::kAttachmentBottomOfBottom:
    return L"Bottom of bottom line";
  case OdDbMLeaderStyle::kAttachmentBottomLine:
    return L"Underline bottom line";
  case OdDbMLeaderStyle::kAttachmentBottomOfTopLine:
    return L"Underline top line";
  case OdDbMLeaderStyle::kAttachmentBottomOfTop:
    return L"Bottom of top line";
  case OdDbMLeaderStyle::kAttachmentAllLine:
    return L"Underline all text";
  case OdDbMLeaderStyle::kAttachmentCenter:
    return L"Center of mtext"; // for vertical attachment.
  case OdDbMLeaderStyle::kAttachmentLinedCenter:
    return L"Underline center of mtext"; // for vertical attachment
  }
  ODA_FAIL_ONCE();
  return OdString::kEmpty;
}

OdString textAttachmentTypeToStr(OdDbMLeader* pLdr, bool bLeft)
{
  OdDbMLeaderStyle::LeaderDirectionType typeDir = bLeft ? OdDbMLeaderStyle::kLeftLeader
                                                        : OdDbMLeaderStyle::kRightLeader;
  OdDbMLeaderStyle::TextAttachmentType typeAttach = pLdr->textAttachmentType(typeDir);
  return textAttachmentTypeToStr(typeAttach);
}

bool strToTextAttachmentType(OdDbMLeader* pLdr, const OdString& sValue, bool bLeft)
{
  OdString sValuePrev = textAttachmentTypeToStr(pLdr, bLeft);
  if (sValue == sValuePrev)
    return false;

  for (OdDbMLeaderStyle::TextAttachmentType typeAttach = OdDbMLeaderStyle::kAttachmentTopOfTop;
       typeAttach <= OdDbMLeaderStyle::kAttachmentLinedCenter; 
       typeAttach = OdDbMLeaderStyle::TextAttachmentType(typeAttach + 1))
  {
    if (textAttachmentTypeToStr(typeAttach) != sValue)
      continue;

    OdDbMLeaderStyle::LeaderDirectionType typeDir = bLeft ? OdDbMLeaderStyle::kLeftLeader
                                                          : OdDbMLeaderStyle::kRightLeader;
    pLdr->setTextAttachmentType(typeAttach, typeDir);
    return true;
  }
  ODA_FAIL_ONCE();
  return false;
}

bool getTextAttachmentTypes(OdDbMLeader* pLdr, OdStringArray& values)
{
  for (OdDbMLeaderStyle::TextAttachmentType typeAttach = OdDbMLeaderStyle::kAttachmentTopOfTop;
       typeAttach <= OdDbMLeaderStyle::kAttachmentLinedCenter; 
       typeAttach = OdDbMLeaderStyle::TextAttachmentType(typeAttach + 1))
  {
    OdString sValue = textAttachmentTypeToStr(typeAttach);
    if (sValue.isEmpty())
      continue;

    values.push_back(sValue);
  }
  return !values.isEmpty();
}

OdString lineSpacingStyleToStr(OdDb::LineSpacingStyle lineSpacingStyle)
{
  switch (lineSpacingStyle)
  {
  case OdDb::kAtLeast:
    return L"At least";
  case OdDb::kExactly:
    return L"Exactly";
  };
  ODA_FAIL_ONCE();
  return OdString::kEmpty;
}

OdDb::LineSpacingStyle strToLineSpacingStyle(const OdString& sValue, OdDb::LineSpacingStyle defVal)
{
  OdDb::LineSpacingStyle st = defVal;
  if (sValue == lineSpacingStyleToStr(OdDb::kAtLeast))
    st = OdDb::kAtLeast;
  else if (sValue == lineSpacingStyleToStr(OdDb::kExactly))
    st = OdDb::kExactly;
  return st;
}

bool getLineSpacingStyles(OdStringArray& values)
{
  values.push_back(lineSpacingStyleToStr(OdDb::kAtLeast));
  values.push_back(lineSpacingStyleToStr(OdDb::kExactly));
  return true;
}

OdString blockConnectionTypeToStr(OdDbMLeaderStyle::BlockConnectionType tp)
{
  switch (tp)
  {
  case OdDbMLeaderStyle::kConnectExtents:
    return L"Center extents";
  case OdDbMLeaderStyle::kConnectBase:
    return L"Insertion point";
  }
  ODA_FAIL_ONCE();
  return OdString::kEmpty;
}

OdDbMLeaderStyle::BlockConnectionType strToBlockConnectionType(const OdString& sValue, 
                                                               OdDbMLeaderStyle::BlockConnectionType typeDef)
{
  if (blockConnectionTypeToStr(OdDbMLeaderStyle::kConnectExtents) == sValue)
    return OdDbMLeaderStyle::kConnectExtents;
  if (blockConnectionTypeToStr(OdDbMLeaderStyle::kConnectBase) == sValue)
    return OdDbMLeaderStyle::kConnectBase;
  return typeDef;
}

bool getBlockConnectionTypes(OdStringArray& values)
{
  values.push_back(blockConnectionTypeToStr(OdDbMLeaderStyle::kConnectExtents));
  values.push_back(blockConnectionTypeToStr(OdDbMLeaderStyle::kConnectBase));
  return true;
}

OdString arrayPolarDirectionToStr(enum OdDbAssocArrayPolarParameters::Direction dir)
{
  switch (dir)
  {
  case OdDbAssocArrayPolarParameters::kClockwise:
    return L"Clockwise";
  case OdDbAssocArrayPolarParameters::kCounterClockwise:
    return L"Counter-clockwise";
  }
  ODA_FAIL_ONCE();
  return OdString::kEmpty;
}

enum OdDbAssocArrayPolarParameters::Direction strToArrayPolarDirection(
                                                const OdString& sValue,
                                                enum OdDbAssocArrayPolarParameters::Direction dirDef)
{
  if (arrayPolarDirectionToStr(OdDbAssocArrayPolarParameters::kClockwise) == sValue)
    return OdDbAssocArrayPolarParameters::kClockwise;
  if (arrayPolarDirectionToStr(OdDbAssocArrayPolarParameters::kCounterClockwise) == sValue)
    return OdDbAssocArrayPolarParameters::kCounterClockwise;
  return dirDef;
}

bool getArrayPolarDirections(OdStringArray& values)
{
  values.push_back(arrayPolarDirectionToStr(OdDbAssocArrayPolarParameters::kClockwise));
  values.push_back(arrayPolarDirectionToStr(OdDbAssocArrayPolarParameters::kCounterClockwise));
  return true;
}

OdString arrayPathMethodToStr(enum OdDbAssocArrayPathParameters::Method method)
{
  switch (method)
  {
  case OdDbAssocArrayPathParameters::kDivide:
    return L"Divide";
  case OdDbAssocArrayPathParameters::kMeasure:
    return L"Measure";
  }
  ODA_FAIL_ONCE();
  return OdString::kEmpty;
}

enum OdDbAssocArrayPathParameters::Method strToArrayPathMethod(
                                                const OdString& sValue,
                                                enum OdDbAssocArrayPathParameters::Method methodDef)
{
  if (arrayPathMethodToStr(OdDbAssocArrayPathParameters::kDivide) == sValue)
    return OdDbAssocArrayPathParameters::kDivide;
  if (arrayPathMethodToStr(OdDbAssocArrayPathParameters::kMeasure) == sValue)
    return OdDbAssocArrayPathParameters::kMeasure;
  return methodDef;
}

bool getArrayPathMethods(OdStringArray& values)
{
  values.push_back(arrayPathMethodToStr(OdDbAssocArrayPathParameters::kDivide));
  values.push_back(arrayPathMethodToStr(OdDbAssocArrayPathParameters::kMeasure));
  return true;
}



OdString yesNoToStr(bool bValue)
{
  return bValue ? L"Yes" : L"No";
}

bool strToYesNo(const OdString& sValue)
{
  ODA_ASSERT_ONCE(sValue == L"Yes" || sValue == L"No");
  return sValue == L"Yes";
}

bool getYesNo(OdStringArray& values)
{
  values.push_back(L"Yes");
  values.push_back(L"No");
  return true;
}

///////////////////////////////////////////////////////////////////////////////

static OdDbViewRepPtr getViewRep(OdDbViewBorderPtr pEnt, OdDb::OpenMode mode = OdDb::kForRead)
{
  return pEnt->lookupViewRepId().openObject(mode);
}

static OdString scaleToStr(OdDbObjectId idScale, const OdString& sDefVal = OdString::kEmpty)
{
  OdString sVal = sDefVal;
  OdDbScalePtr pScale = OdDbScale::cast(idScale.safeOpenObject());
  if (pScale.get())
  {
    sVal = pScale->scaleName();
    ODA_ASSERT_ONCE(!sVal.isEmpty());
  }
  return sVal;
}

static OdString scaleToStr(OdRxObject* pRxDb, double dValue, const OdString& sDefVal = OdString::kEmpty)
{
  OdDbDatabase* pDb = OdDbDatabase::cast(pRxDb).get();
  OdString sVal = sDefVal;
  if (!pDb)
    return sVal;
  OdDbDictionaryPtr pScaleDict = pDb->getScaleListDictionaryId().safeOpenObject();
  for (OdDbDictionaryIteratorPtr pItr = pScaleDict->newIterator(); !pItr->done(); pItr->next())
  {
    OdDbScalePtr pScale = pItr->getObject();
    double dVal = pScale->scale();
    if (!OdEqual(dValue, dVal))
      continue;
    sVal = pScale->scaleName();
    ODA_ASSERT_ONCE(!sVal.isEmpty());
    break;
  }
  return sVal;
}

static OdString scaleToStr(OdDbViewBorderPtr pEnt)
{
  OdDbViewRepPtr pViewRep = getViewRep(pEnt);
  if (pViewRep.get() && pViewRep->isScaleFromParent())
    return L"From parent";
  double dScale = pEnt->scale();
  OdString sScale = scaleToStr(pEnt->database(), dScale, L"Custom");
  return sScale;
}

static double strToScale(OdRxObject* pRxDb, const OdString& sName, double dDefVal = 1.0)
{
  OdDbDatabase* pDb = OdDbDatabase::cast(pRxDb).get();
  ODA_ASSERT_ONCE(pDb);
  if (!pDb)
    return dDefVal;
  OdDbDictionaryPtr pScaleDict = pDb->getScaleListDictionaryId().safeOpenObject();
  for (OdDbDictionaryIteratorPtr pItr = pScaleDict->newIterator(); !pItr->done(); pItr->next())
  {
    OdDbScalePtr pScale = pItr->getObject();
    if (pScale->scaleName() != sName)
      continue;
    double dVal = pScale->scale();
    ODA_ASSERT_ONCE(dVal > 0.0);
    return dVal;
  }
  return dDefVal;
}

static bool getAnnoscaleNames(OdRxObject* pRxDb, OdStringArray& values)
{
  OdDbDatabase* pDb = OdDbDatabase::cast(pRxDb).get();
  if (!pDb)
    return false;
  values.clear();
  OdDbDictionaryPtr pScaleDict = pDb->getScaleListDictionaryId().safeOpenObject();
  for (OdDbDictionaryIteratorPtr pItr = pScaleDict->newIterator(); !pItr->done(); pItr->next())
  {
    OdDbScalePtr pScale = pItr->getObject();
    OdString sVal = pScale->scaleName();
    ODA_ASSERT_ONCE(!sVal.isEmpty());
    values.push_back(sVal);
  }
  return !values.isEmpty();
}

//#if defined(TEST_BY_VIEWPORT) && defined(_DEBUG)
//static OdDbAssocStatus getActionStateByDep(OdDbObjectId idDep, OdDbObjectIdArray* pDepIds = NULL)
//{
//  OdDbAssocStatus st = kIsUpToDateAssocStatus;
//  OdDbAssocDependencyPtr pDep;
//  if (!idDep.isNull() && (pDep = OdDbAssocDependency::cast(idDep.openObject())).get())
//  {
//    OdDbAssocActionPtr pAction;
//    OdDbObjectId idAction = pDep->owningAction();
//    if (!idAction.isNull() && (pAction = OdDbAssocAction::cast(idAction.openObject(OdDb::kForWrite))).get())
//    {
//      if (pDepIds)
//      {
//        OdDbObjectIdArray arrPersistentReactors = pAction->getPersistentReactors();
//        for (unsigned int idx = 0, sz = arrPersistentReactors.length(); idx < sz; idx++)
//        {
//          OdDbObjectId id = arrPersistentReactors[idx];
//          if (id == pDep->objectId())
//            continue;
//          OdDbAssocDependencyPtr pDep_ = OdDbAssocDependency::cast(id.openObject());
//          if (pDep_.isNull() || !pDep_->isDelegatingToOwningAction())
//            continue;
//
//          pDepIds->push_back(id);
//        }
//      }
//
//      st = pAction->status();
//      //if (st == kIsUpToDateAssocStatus) // strange
//      //  pAction->setStatus(kChangedDirectlyAssocStatus, true);
//    }
//  }
//  return st;
//}
//static OdDbAssocStatus getActionState(OdDbViewBorderPtr pEnt, OdDbObjectIdArray* pDepIds = NULL)
//{
//  OdDbObjectId idDep = OdDbAssocDependency::getFirstDependencyOnObject(pEnt);
//  return getActionStateByDep(idDep, pDepIds);
//}
//#endif // defined(TEST_BY_VIEWPORT) && defined(_DEBUG)

static OdString viewTypeToStr(OdDbViewBorderPtr pEnt)
{
  OdString sVal = L"Base";
  OdDbViewRepPtr pViewRep = getViewRep(pEnt);
  if (pViewRep.get())
  {
    OdDbViewRep::ViewType type = pViewRep->viewType();
    switch (type)
    {
    default:
      ODA_FAIL_ONCE();
    case OdDbViewRep::kBaseFront:
    case OdDbViewRep::kBaseTop:
    case OdDbViewRep::kBaseRight:
    case OdDbViewRep::kBaseBack:
    case OdDbViewRep::kBaseBottom:
    case OdDbViewRep::kBaseLeft:
    case OdDbViewRep::kBaseNEIsometric:
    case OdDbViewRep::kBaseNWIsometric:
    case OdDbViewRep::kBaseSEIsometric:
    case OdDbViewRep::kBaseSWIsometric:
    case OdDbViewRep::kBaseCurrent:
      //sVal = L"Base"; 
      break;
    case OdDbViewRep::kDetail:
      sVal = L"Detail"; break;
    case OdDbViewRep::kSection:
      sVal = L"Section"; break;
    case OdDbViewRep::kProjected:
      sVal = L"Projected"; break;
    }
  }
  return sVal;
}

static OdString hiddenLinesToStr(OdDbViewBorderPtr pEnt)
{
  OdString sVal;
#if 1
  OdDbViewBorder::ViewStyleType type = pEnt->viewStyleType();
  switch (type)
  {
  default:
    ODA_FAIL_ONCE(); break;
  case OdDbViewBorder::kFromBase: //0
    sVal = L"From parent"; break;
  case OdDbViewBorder::kWireframeVisibleEdges: //1
    sVal = L"Visible lines"; break;
  case OdDbViewBorder::kWireframeHiddenEdges: //2
    sVal = L"Visible and hidden lines"; break;
  case OdDbViewBorder::kShadedVisibleEdges: //3
    sVal = L"Shaded visible lines"; break;
  case OdDbViewBorder::kShadedHiddenEdges: //4
    sVal = L"Shaded visible and hidden lines"; break;
  }
#else
  OdDbViewRepPtr pViewRep = getViewRep(pEnt);
  if (pViewRep.get())
  {
    OdDbViewRep::ViewStyleType type = pViewRep->viewStyleType();
    switch (type)
    {
    default:
      ODA_FAIL_ONCE(); break;
    case OdDbViewRep::kVisibleLines:
      sVal = L"Visible lines"; break;
    case OdDbViewRep::kVisibleAndHiddenLines:
      sVal = L"Visible and hidden lines"; break;
    case OdDbViewRep::kShadedVisibleLines:
      sVal = L"Shaded visible lines"; break;
    case OdDbViewRep::kShadedVisibleAndHiddenLines:
      sVal = L"Shaded visible and hidden lines"; break;
    }
  }
#endif
  return sVal;
}

static OdString sourceTypeToStr(OdDbViewBorderPtr pEnt)
{
  OdDbViewBorder::SourceType type = pEnt->sourceType();
  OdString sVal;
  switch (type)
  {
  default: //case OdDbViewBorder::kSourceNotDefined:
    ODA_FAIL_ONCE();
    sVal = L"Not Defined"; break;
  case OdDbViewBorder::kInventorSource:
    sVal = L"Inventor Model"; break;
  case OdDbViewBorder::kFusionSource:
    sVal = L"Fusion"; break;
  case OdDbViewBorder::kModelSpaceSource:
    sVal = L"Model Space"; break;
  }
  return sVal;
}

///////////////////////////////////////////////////////////////////////////////

class OdDbXmlPropBlockReference : public OdPropBase
{
public:
  virtual bool getPropertyValues(OdPropContext& ctx)
  {
    OdDbBlockReferencePtr pRef = object();
    //OdDbDatabase* pDb = pRef->database();

    OdPropContext ctxGeom = ctx.addCategory(L"Geometry");

    if (OdDbAssocArrayActionBody::isAssociativeArray(pRef))
    {
      OdDbObjectId idBody = OdDbAssocArrayActionBody::getControllingActionBody(pRef);
      OdDbAssocArrayActionBodyPtr pBody = OdDbAssocArrayActionBody::cast(idBody.openObject());
      ODA_ASSERT_ONCE(pBody.get());
      if (pBody.isNull())
        return true;

      OdDbAssocArrayParametersPtr pParams = pBody->parameters();

      OdDbVertexRef vertexRef; OdGePoint3d pt;
      OdResult es = pBody->getSourceBasePoint(vertexRef, pt);
      if (es == eOk)
      {
        ctxGeom.addProperty(L"Base X", pt.x, tDistance).comment(L"Specifies the X value for the base point of the array.");
        ctxGeom.addProperty(L"Base Y", pt.y, tDistance).comment(L"Specifies the Y value for the base point of the array.");
        ctxGeom.addProperty(L"Base Z", pt.z, tDistance).comment(L"Specifies the Z value for the base point of the array.");
      }

      OdDbAssocArrayCommonParameters* pCommon = OdDbAssocArrayCommonParameters::cast(pParams);
      if (pCommon)
      {
        OdPropContext ctxMisc = ctx.addCategory(L"Misc");

        OdDbAssocArrayRectangularParameters* pRect = OdDbAssocArrayRectangularParameters::cast(pParams);
        OdDbAssocArrayPolarParameters* pPolar = OdDbAssocArrayPolarParameters::cast(pParams);
        OdDbAssocArrayPathParameters* pPath =  OdDbAssocArrayPathParameters::cast(pParams);
        if (pRect)
        {
          ctxMisc.addProperty(L"Column", OdVariant((OdInt32)pRect->columnCount()), tInt).comment(L"Specifies number of columns in the array.");
          ctxMisc.addProperty(L"Column spacing", pRect->columnSpacing(), tDistance).comment(L"Specifies the offset spacing between columns in the array.");
        }
        else if (pPolar)
        {
          OdGePoint3d pt;
          OdDbGeomRefPtr pGeomValue; OdDbObjectId id; OdDbVertexRef* pValue = NULL;
          OdResult es = pParams->getGeomParam(L"Origin", pGeomValue, id);
          if (   es == eOk && pGeomValue.get() 
              && (pValue = OdDbVertexRef::cast(pGeomValue).get()) != NULL) // or OdDbVectorRef* pValue = OdDbVectorRef::cast(pGeomValue).get()
            pt = pValue->point();
          ctxGeom.addProperty(L"Center X", pt.x, tDistance).comment(L"Specifies the X value for the center point.");
          ctxGeom.addProperty(L"Center Y", pt.y, tDistance).comment(L"Specifies the Y value for the center point.");
          ctxGeom.addProperty(L"Center Z", pt.z, tDistance).comment(L"Specifies the Z value for the center point.");
          ctxGeom.addProperty(L"Radius", pPolar->radius(), tDistance).comment(L"Specifies radius of polar array.");

          ctxMisc.addProperty(L"Direction", arrayPolarDirectionToStr(pPolar->direction()), tCombo).comment(L"Specifies direction of arc.");
          ctxMisc.addProperty(L"Items", OdVariant((OdInt32)pPolar->itemCount()), tInt).comment(L"Specifies number of items arrayed along the path.");
          ctxMisc.addProperty(L"Angle between items", toRadian(pPolar->angleBetweenItems()), tAngle).comment(L"Specifies angle between items in array.");
          ctxMisc.addProperty(L"Fill angle", toRadian(pPolar->fillAngle()), tAngle).comment(L"Specifies angle between first and last items in array.");
        }
        else if (pPath)
        {
          OdDbEdgeRef edgeRef = pPath->path();
          const OdGeCurve3d* pCurve = edgeRef.curve();
          if (pCurve)
          {
            OdGeInterval interval; pCurve->getInterval(interval);
            double len = interval.length();
            ctxGeom.addProperty(L"Path length", len, tDistance).readOnly().comment(L"Specifies length of path.");
          }
          ctxMisc.addProperty(L"Method", arrayPathMethodToStr(pPath->method()), tCombo).comment(L"Specifies the method of arraying items along the path.");
          ctxMisc.addProperty(L"Items", OdVariant((OdInt32)pPath->itemCount()), tInt).comment(L"Specifies number of items arrayed along the path.");
          ctxMisc.addProperty(L"Item spacing", pPath->itemSpacing(), tDistance).comment(L"Specifies the spacing between items long the path.");
          ctxMisc.addProperty(L"Start offset", pPath->startOffset(), tDistance).comment(L"Specifies offset distance from start of path curve.");
          if (pCurve && !pCurve->isClosed())
            ctxMisc.addProperty(L"End offset", pPath->endOffset(), tDistance).comment(L"Specifies offset distance from end of path curve.");
        }

        ctxMisc.addProperty(L"Rows", OdVariant((OdInt32) pCommon->rowCount()), tInt).comment(L"Specifies number of rows in the array.");
        ctxMisc.addProperty(L"Row spacing", pCommon->rowSpacing(), tDistance).comment(L"Specifies the offset spacing between rows in the array.");
        ctxMisc.addProperty(L"Row elevation", pCommon->rowElevation(), tDistance).comment(L"Specifies the change in the elevation spacing between adjacent rows.");
        ctxMisc.addProperty(L"Levels", OdVariant((OdInt32)pCommon->levelCount()), tInt).comment(L"Specifies number of levels in the array.");
        ctxMisc.addProperty(L"Level spacing", pCommon->levelSpacing(), tDistance).comment(L"Specifies the offset spacing between levels in the array.");

        if (pRect)
          ctxMisc.addProperty(L"Included axis angleitems", toRadian(pRect->axesAngle()), tAngle).comment(L"Specifies the included angle between axes.");
        else if (pPolar)
          ctxMisc.addProperty(L"Rotate items", yesNoToStr(pPolar->rotateItems()), tCombo).comment(L"Rotate items as they are arrayed.");
        else if (pPath)
          ctxMisc.addProperty(L"Align items", yesNoToStr(pPath->alignItems()), tCombo).comment(L"Aligns arrayed items with path.");
      }
      return true;
    } // if (OdDbAssocArrayActionBody::isAssociativeArray(pRef))

    OdDbObjectId idBody = OdDbSmartCenterActionBody::getSmartCenterActionBody(pRef);
    if (!idBody.isNull())
    {
      OdDbAssocArrayActionBodyPtr pBody = OdDbAssocArrayActionBody::cast(idBody.openObject());
      ODA_ASSERT_ONCE(pBody.get());
      if (pBody.isNull())
        return true;

      OdDbAssocArrayParametersPtr pParams = pBody->parameters();
      ODA_ASSERT_ONCE(pParams.get());
      OdDbSmartCenterCommonParameters* pSmartCmn = OdDbSmartCenterCommonParameters::cast(pParams);
      ODA_ASSERT_ONCE(pSmartCmn);
      //if (!pSmartCmn)
      //  return true;

      // TODO
      // Geometry:   Cross size, Cross gap, Show extension
      // Misc:       Type, Rotation, Annotative
      return true;
    }
    //if (???)
    //{    
    //  if (OdDbAssocArrayModifyActionBody* pModify = OdDbAssocArrayModifyActionBody::cast(pParams))
    //  {
    //    // TODO
    //  }
    //  else if (OdDbAssocLoftParameters* pLoft = OdDbAssocLoftParameters::cast(pParams))
    //  {
    //    // TODO
    //  }
    //}

    OdGePoint3d vPos = pRef->position();
    ctxGeom.addProperty(L"Position X", vPos.x, sXCoord).comment(L"Specify X,Y,Z for insertion point of the block.");
    ctxGeom.addProperty(L"Position Y", vPos.y, sYCoord).comment(L"Specify X,Y,Z for insertion point of the block.");
    ctxGeom.addProperty(L"Position Z", vPos.z, sZCoord).comment(L"Specify X,Y,Z for insertion point of the block.");
    OdGeScale3d scale = pRef->scaleFactors();
    ctxGeom.addProperty(L"Scale X", scale.sx, sXCoord).comment(L"Specifies the effective XScale factor of the block.");
    ctxGeom.addProperty(L"Scale Y", scale.sy, sYCoord).comment(L"Specifies the effective YScale factor of the block.");
    ctxGeom.addProperty(L"Scale Z", scale.sz, sZCoord).comment(L"Specifies the effective ZScale factor of the block.");

    OdPropContext ctxMisc = ctx.addCategory(L"Misc");
    ctxMisc.addProperty(L"Name", OdDbSymUtil::getSymbolName(pRef->blockTableRecord()), tString).readOnly().comment(L"Specifies the original block name.");
    ctxMisc.addProperty(L"Rotation", pRef->rotation(), tAngle).comment(L"Specifies the rotation angle of the block."); 
    bool isAnno = false;
    if (OdDbAnnotativeObjectPE* pPe = OdDbAnnotativeObjectPE::cast(pRef).get())
      isAnno = pPe->annotative(pRef);
    ctxMisc.addProperty(L"Annotative", yesNoToStr(isAnno), tString).readOnly().comment(L"Specifies whether or not the block is annotative.");

    return true;
  }

  virtual bool setPropertyValue(const OdString& sPath, const OdString& sValue, Action& action)
  {
    OdString sName = sPath, sCat = extractFirst(sName);

    OdDbBlockReferencePtr pRef = object(); pRef->upgradeOpen();

    if (OdDbAssocArrayActionBody::isAssociativeArray(pRef))
    {
      OdDbObjectId idBody = OdDbAssocArrayActionBody::getControllingActionBody(pRef);
      OdDbAssocArrayActionBodyPtr pBody = OdDbAssocArrayActionBody::cast(idBody.openObject());
      ODA_ASSERT_ONCE(pBody.get());
      if (pBody.isNull())
        return false;
      OdDbAssocArrayParametersPtr pParams = pBody->parameters();

      OdDbAssocArrayCommonParameters* pCommon = OdDbAssocArrayCommonParameters::cast(pParams);
      OdDbAssocArrayRectangularParameters* pRect = OdDbAssocArrayRectangularParameters::cast(pParams);
      OdDbAssocArrayPolarParameters* pPolar = OdDbAssocArrayPolarParameters::cast(pParams);
      OdDbAssocArrayPathParameters* pPath = OdDbAssocArrayPathParameters::cast(pParams);

      if (sCat == L"Geometry")
      {
        if (!sName.find(L"Base "))
        {
          OdDbVertexRef vertexRef; OdGePoint3d ptBase;
          OdResult es = pBody->getSourceBasePoint(vertexRef, ptBase);
          ODA_ASSERT_ONCE(es == eOk);
          if (es != eOk)
            return false;
          OdGePoint3d pt = ptBase;
          if (sName == L"Base X")
            pt.x = toDistance(sValue);
          else if (sName == L"Base Y")
            pt.y = toDistance(sValue);
          else if (sName == L"Base Z")
            pt.z = toDistance(sValue);
          else
          {
            ODA_FAIL_ONCE();
            return false;
          }
          if (pt == ptBase)
            return true;
          vertexRef = OdDbVertexRef(pt);
          es = pBody->setSourceBasePoint(vertexRef);
          ODA_ASSERT_ONCE(es == eOk);
          return true;
        }

        if (pPolar)
        {
          if (!sName.find(L"Center "))
          {
            OdDbGeomRefPtr pGeomValue; OdDbObjectId id; OdDbVertexRef* pValue = NULL;
            OdResult es = pParams->getGeomParam(L"Origin", pGeomValue, id);
            if (   es != eOk || pGeomValue.isNull()
                || !(pValue = OdDbVertexRef::cast(pGeomValue).get()))
            {
              ODA_FAIL_ONCE();
              return false;
            }

            OdGePoint3d ptCenter = pValue->point(),
                        pt = ptCenter;
            if (sName == L"Center X")
              pt.x = toDistance(sValue);
            else if (sName == L"Center Y")
              pt.y = toDistance(sValue);
            else if (sName == L"Center Z")
              pt.z = toDistance(sValue);
            else
            {
              ODA_FAIL_ONCE();
              return false;
            }
            if (pt == ptCenter)
              return true;

            pGeomValue = OdDbVertexRef::createObject(pt);
            es = pParams->setGeomParam(L"Origin", pGeomValue, id);
            ODA_ASSERT_ONCE(es == eOk);
            return true;
          }
          if (sName == L"Radius")
          {
            double dRadius = toDistance(sValue);
            if (!OdEqual(dRadius, pPolar->radius()))
              pPolar->setRadius(dRadius);
            return true;
          }
          ODA_FAIL_ONCE();
          return false;
        }
        ODA_FAIL_ONCE();
        return false;
      }

      if (sCat == L"Misc" && pCommon)
      {
        if (sName == L"Rows")
        {
          int num = toInt(sValue);
          if (num > 0)
            pCommon->setRowCount(num);
          return true;
        }
        if (sName == L"Row spacing")
        {
          double dist = toDistance(sValue);
          if (dist >= 0)
            pCommon->setRowSpacing(dist);
          return true;
        }
        if (sName == L"Row elevation")
        {
          double dist = toDistance(sValue);
          if (dist >= 0)
            pCommon->setRowElevation(dist);
          return true;
        }
        if (sName == L"Levels")
        {
          int num = toInt(sValue);
          if (num > 0)
            pCommon->setLevelCount(num);
          return true;
        }
        if (sName == L"Level spacing")
        {
          double dist = toDistance(sValue);
          if (dist >= 0)
            pCommon->setLevelSpacing(dist);
          return true;
        }

        if (pRect)
        {
          if (sName == L"Column")
          {
            int num = toInt(sValue);
            if (num > 0)
              pRect->setColumnCount(num);
            return true;
          }
          if (sName == L"Column spacing")
          {
            double dist = toDistance(sValue);
            if (dist >= 0)
              pRect->setColumnSpacing(dist);
            return true;
          }
          if (sName == L"Included axis angle items")
          {
            double angle = toDegree(toAngle(sValue, true));
            if (angle != pRect->axesAngle())
              pRect->setAxesAngle(angle);
            return true;
          }
          ODA_FAIL_ONCE();
          return false;
        }

        if (pPolar)
        {
          if (sName == L"Direction")
          {
            enum OdDbAssocArrayPolarParameters::Direction dir = strToArrayPolarDirection(sValue, pPolar->direction());
            if (dir != pPolar->direction())
              pPolar->setDirection(dir);
            return true;
          }
          if (sName == L"Items")
          {
            int num = toInt(sValue);
            if (num > 0)
              pPolar->setItemCount(num);
            return true;
          }
          if (sName == L"Angle between items")
          {
            double dAngle = toDegree(toAngle(sValue, true));
            if (pPolar && !OdEqual(pPolar->angleBetweenItems(), dAngle))
              pPolar->setAngleBetweenItems(dAngle);
            return true;
          }
          if (sName == L"Fill angle")
          {
            double dAngle = toDegree(toAngle(sValue, true));
            if (pPolar && !OdEqual(pPolar->fillAngle(), dAngle))
              pPolar->setFillAngle(dAngle);
            return true;
          }
          if (sName == L"Rotate items")
          {
            bool val = strToYesNo(sValue);
            if (val != pPolar->rotateItems())
              pPolar->setRotateItems(val);
            return true;
          }
          ODA_FAIL_ONCE();
          return false;
        }

        if (pPath)
        {
          if (sName == L"Method")
          {
            enum OdDbAssocArrayPathParameters::Method mthd = strToArrayPathMethod(sValue, pPath->method());
            if (mthd != pPath->method())
              pPath->setMethod(mthd);
            return true;
          }
          if (sName == L"Items")
          {
            int num = toInt(sValue);
            if (num > 0)
              pPath->setItemCount(num);
            return true;
          }
          if (sName == L"Item spacing")
          {
            int num = toInt(sValue);
            if (num > 0)
              pPath->setItemSpacing(num);
            return true;
          }
          if (sName == L"Start offset")
          {
            double dist = toDistance(sValue);
            pPath->setStartOffset(dist);
            return true;
          }
          if (sName == L"End offset")
          {
            double dist = toDistance(sValue);
            pPath->setEndOffset(dist);
            return true;
          }
          if (sName == L"Align items")
          {
            pPath->setAlignItems(strToYesNo(sValue));
            return true;
          }
          ODA_FAIL_ONCE();
          return false;
        }

        ODA_FAIL_ONCE(); // TODO
        return false;
      }

      ODA_FAIL_ONCE(); // TODO
      return true;
    } // if (OdDbAssocArrayActionBody::isAssociativeArray(pRef))

    if (sCat == L"Geometry")
    {
      if (!sName.find(L"Position "))
      {
        OdGePoint3d vPos = pRef->position();
        if (sName == L"Position X")
          vPos.x = toDistance(sValue);
        else if (sName == L"Position Y")
          vPos.y = toDistance(sValue);
        else if (sName == L"Position Z")
          vPos.z = toDistance(sValue);
        else
        {
          ODA_FAIL_ONCE();
          return false;
        }
        if (vPos != pRef->position())
          pRef->setPosition(vPos);
        return true;
      }
      if (!sName.find(L"Scale "))
      {
        OdGeScale3d scale = pRef->scaleFactors();
        OdGePoint3d vPos = pRef->position();
        if (sName == L"Scale X")
          scale.sx = toScale(sValue);
        else if (sName == L"Scale Y")
          scale.sy = toScale(sValue);
        else if (sName == L"Scale Z")
          scale.sz = toScale(sValue);
        else
        {
          ODA_FAIL_ONCE();
          return false;
        }
        if (scale != pRef->scaleFactors())
          pRef->setScaleFactors(scale);
        return true;
      }
      ODA_FAIL_ONCE();
      return false;
    }

    if (sCat == L"Misc")
    {
      OdDbBlockReferencePtr pRef = object(); pRef->upgradeOpen();
      if (sName == L"Rotation")
      {
        pRef->setRotation(toAngle(sValue));
        return true;
      }
      ODA_FAIL_ONCE();
      return false;
    }

    ODA_FAIL_ONCE();
    return false;
  }

  virtual bool getPropertyPossibleValues(const OdString& sPath,
                                         OdStringArray& values,
                                         OdStringArray&) // iconPaths
  {
    OdString sName = sPath, sCat = extractFirst(sName);
    if (sCat == L"Misc")
    {
      if (sName == L"Direction")
        return getArrayPolarDirections(values);
      if (sName == L"Method")
        return getArrayPathMethods(values);
      if (sName == L"Rotate items" || sName == L"Align items")
        return getYesNo(values);
    }
    return false;
  }

  virtual bool overrideClassName(OdString& sClassName) const
  {
    OdDbBlockReferencePtr pRef = object();
    if (OdDbAssocArrayActionBody::isAssociativeArray(pRef))
    {
      OdDbObjectId idBody = OdDbAssocArrayActionBody::getControllingActionBody(pRef);
      OdDbAssocArrayActionBodyPtr pBody = OdDbAssocArrayActionBody::cast(idBody.openObject());
      ODA_ASSERT_ONCE(pBody.get());
      if (pBody.isNull())
        return false;
      OdDbAssocArrayParametersPtr pParams = pBody->parameters();
      if (OdDbAssocArrayRectangularParameters::cast(pParams).get())
        sClassName = L"Array (Rectangular)";
      else if (OdDbAssocArrayPolarParameters::cast(pParams).get())
        sClassName = L"Array (Polar)";
      else if (OdDbAssocArrayPathParameters::cast(pParams).get())
        sClassName = L"Array (Path)";
      else
      {
        ODA_FAIL_ONCE();
        return false;
      }
      return true;
    }

    OdDbObjectId idBody = OdDbSmartCenterActionBody::getSmartCenterActionBody(pRef);
    if (!idBody.isNull())
    {
      sClassName = L"Center Mark"; // like in ACAD 2020 & later
      return true;
    }
    return false;
  }
};

///////////////////////////////////////////////////////////////////////////////

class OdDbXmlPropMLeader : public OdPropBase
{
public:
  virtual bool getPropertyValues(OdPropContext& ctx)
  {
    OdDbMLeaderPtr pLdr = object();
    //OdDbDatabase* pDb = pLdr->database();

    ctx.addCategory(L"3D Visualization").skip();

    OdPropContext ctxMisc = ctx.addCategory(L"Misc");
    double dScale = pLdr->scale(), // GrCode 45 // ? = m_dScaleOverall GrCode = 40
           dLineSpaceDistanceScale = dScale;
    ctxMisc.addProperty(L"Overall scale", dScale, tDistance).comment(L"Specifies the overall scale factor of this multileader object.");
    OdString sStyle = dicRecIdToStr<OdDbMLeaderStyle>(pLdr->MLeaderStyle()); // GrCode 340
    ctxMisc.addProperty(L"Multileader style", sStyle, tCombo).comment(L"Specifies the style name of this multileader object.");
    bool isAnno = pLdr->enableAnnotationScale();
    ctxMisc.addProperty(L"Annotative", yesNoToStr(isAnno), tCombo).comment(L"Specifies whether or not the multileader is annotative.");
    if (isAnno)
    {
      OdDbObjectContextManagerPtr pMngr = (OdDbObjectContextManager*) pLdr->getContextDataManager();
      if (pMngr.get())
      {
        OdDbObjectContextCollection* pCtxCollection = pMngr->contextCollection(ODDB_ANNOTATIONSCALES_COLLECTION);
        ODA_ASSERT_ONCE(pCtxCollection);
        OdDbAnnotationScalePtr pScale = pCtxCollection->currentContext(pLdr);
        OdString sAnno = pScale->getName();
        if (pScale->getScale(dLineSpaceDistanceScale) == eOk && !OdZero(dLineSpaceDistanceScale))
          dLineSpaceDistanceScale = 1.0 / dLineSpaceDistanceScale;
        ctxMisc.addProperty(L"Annotative scale", sAnno, tCombo).comment(L"Specifies the annotative scale.");
      }
      //OdResult OdDbMLeader::getBlockAttributeValue(
      //  OdDbObjectId attdefId,
      //  OdDbAttributePtr& pAttr) const
      //  class DBENT_EXPORT OdDbAnnotativeObjectPEImpl : public OdDbAnnotativeObjectPE

      //OdDbAnnotativeObjectPEPtr(pAttrib)->setAnnotative(pAttrib, false);
      //OdDbObjectContextPEPtr ctxPE = OdDbObjectContextInterface::cast(pAttDef);
      //OdDbAnnotScaleObjectContextDataPtr defCtx = ctxPE->getDefaultContextData(pAttDef, ODDB_ANNOTATIONSCALES_COLLECTION);
    }

    OdPropContext ctxLeaders = ctx.addCategory(L"Leaders");
    ctxLeaders.addProperty(L"Leader type", leaderLineTypeToStr(pLdr->leaderLineType()), tCombo).comment(L"Specifies the leader type.");
    OdPropServicesPtr pPropServ = propServ();
    if (pPropServ.get())
    {
      OdString sColor = pPropServ->colorBaseToStr(baseDatabase(), pLdr->leaderLineColor());
      ctxLeaders.addProperty(L"Leader color", sColor, tOdColor).comment(L"Specifies the color of the leader lines.");
      OdString sLinetype = pPropServ->linetypeToStr(baseDatabase(), pLdr->leaderLineTypeId());
      ctxLeaders.addProperty(L"Leader linetype", sLinetype, tLinetype).comment(L"Specifies the linetype of the leader lines.");
      OdString sLineweight = pPropServ->lineweightToStr(baseDatabase(), pLdr->leaderLineWeight());
      ctxLeaders.addProperty(L"Leader lineweight", sLineweight, tLineweight).comment(L"Specifies the line weight of the leader lines.");
    }

    ctxLeaders.addProperty(L"Arrowhead", arrowBlockIdToStr(pLdr), tCombo).comment(L"Specifies the type of the leader arrowhead.");
    ctxLeaders.addProperty(L"Arrowhead Size", pLdr->arrowSize(), tDistance).comment(L"Specifies the size of the leader arrowhead.");
    bool bHorizontalLanding = pLdr->enableLanding(); // TODO? && pLdr->enableDogleg();
    ctxLeaders.addProperty(L"Horizontal Landing", yesNoToStr(bHorizontalLanding), tCombo).comment(L"Enable/Disable horizontal landing of multileader.");
    ctxLeaders.addProperty(L"Landing Distance", pLdr->doglegLength(), tDistance).comment(L"Specifies the landing distance.");
    ctxLeaders.addProperty(L"Leader extension", yesNoToStr(pLdr->extendLeaderToText()), tCombo).comment(L"Extends the horizontal multiline leader to the multiline text.");

    OdDbMLeaderStyle::ContentType type = pLdr->contentType();
    switch (type)
    {
    case OdDbMLeaderStyle::kMTextContent:
      {
        OdPropContext ctxText = ctx.addCategory(L"Text");
        OdDbMTextPtr pMText = pLdr->mtext();
        if (pMText.get())
        {
          OdString sText = pMText->contents();
          ctxText.addProperty(L"Contents", sText, tString).comment(L"Specifies the text string of the mtext.");
          sStyle = styleIdToStr(pMText->textStyle());
          ctxText.addProperty(L"Text style", sStyle, tCombo).comment(L"Specifies the style name of the mtext.");
        }
        ctxText.addProperty(L"Justify", textAlignmentTypeToStr(pLdr), tCombo).comment(L"Specifies the attachment point of the mtext.");
        if (pMText.get())
          ctxText.addProperty(L"Direction", textDirectionToStr(pLdr), tCombo).comment(L"Specifies the drawing direction of the mtext.");
        ctxText.addProperty(L"Width", pMText->width(), tDistance).comment(L"Specifies the of the mtext.");
        ctxText.addProperty(L"Height", pLdr->textHeight(), tDistance).comment(L"Specifies the of the mtext.");
        double dAng = pMText->rotation();
        if (dAng < 0.0 && OdZero(dAng))
          dAng = 0.0; // normalize ("0" instead "360") // mleader_transformBy1_oda_1_0.dwg mleader_transformBy1_oda_2_0.dwg of CORE-11916/CORE-15259 subtest transformBy1
        ctxText.addProperty(L"Rotation", dAng, tAngle).comment(L"Specifies the rotation angle of the mtext.");
        ctxText.addProperty(L"Line space factor", pMText->lineSpacingFactor(), tDistance).comment(L"Specifies the line spacing factor of the mtext.");
        double dLineSpaceDistance = pLdr->textHeight() * pMText->lineSpacingFactor() * (1. + 2. / 3.) * dLineSpaceDistanceScale;
        ctxText.addProperty(L"Line space distance", dLineSpaceDistance, tDistance).readOnly().comment(L"Specifies the line spacing distance of the mtext.");
        ctxText.addProperty(L"Line space style", lineSpacingStyleToStr(pMText->lineSpacingStyle()), tCombo).comment(L"Specifies the line spacing style of the mtext.");
        if (pMText.get())
          ctxText.addProperty(L"Background mask", yesNoToStr(pMText->backgroundFillOn()), tCombo).comment(L"Specifies use Background Mask.");
        ctxText.addProperty(L"Attachment type", textAttachmentDirectionToStr(pLdr), tCombo).comment(L"Specifies if leaders connect with text horizontally or vertically.");
        ctxText.addProperty(L"Left Attachment", textAttachmentTypeToStr(pLdr, true), tCombo).comment(L"Specifies how mleaders on the left side connect with the text.");
        ctxText.addProperty(L"Right Attachment", textAttachmentTypeToStr(pLdr, false), tCombo).comment(L"Specifies how mleaders on the right side connect with the text."); // ?  Get the dog leg direction of the specific leader.
        ctxText.addProperty(L"Landing gap", pLdr->landingGap(), tDistance).comment(L"Specifies the text landing gap.");
        ctxText.addProperty(L"Text frame", OdString(pLdr->enableFrameText() ? L"Yes" : L"No"), tCombo).comment(L"Display/Hide text frame of multileader content.");
        break;
      }
    case OdDbMLeaderStyle::kBlockContent:
      {
        OdPropContext ctxBlock = ctx.addCategory(L"Block");
        ctxBlock.addProperty(L"Source block", blockIdToStr(pLdr), tCombo).comment(L"Specifies the content block of multileader.");
        ctxBlock.addProperty(L"Attachment", blockConnectionTypeToStr(pLdr->blockConnectionType()), tCombo).comment(L"Specify how leaders connect with content block.");
        OdGeScale3d scale = pLdr->blockScale();
        ODA_ASSERT_ONCE(OdEqual(fabs(scale.sx), scale.sz) && OdEqual(fabs(scale.sy), scale.sz));
        ctxBlock.addProperty(L"Scale", scale.sz, tDistance).comment(L"Specify how leaders connect with content block.");
        break;
      }
    }
    return true;
  }

  virtual bool setPropertyValue(const OdString& sPath,
                                const OdString& sValue,
                                Action& action)
  {
    OdString sName = sPath, sCat = extractFirst(sName);
    bool bVal;
    if (sCat == L"Misc")
    {
      OdDbMLeaderPtr pLdr = object(); pLdr->upgradeOpen();
      if (sName == L"Overall scale")
      {
        double dScale = odStrToD(sValue);
        if (dScale > 0.0 && !OdEqual(dScale, pLdr->scale()))
          pLdr->setScale(dScale);
      }
      else if (sName == L"Multileader style")
      {
        OdDbObjectId idStylePrev = pLdr->MLeaderStyle(),
                     idStyle = strToDicRecId<OdDbMLeaderStyle>(OdDbDatabase::cast(baseDatabase())->getMLeaderStyleDictionaryId(false), sValue);
        if (idStyle.isNull() && idStyle != idStylePrev)
          pLdr->setMLeaderStyle(idStyle);
      }
      else if (sName == L"Annotative")
      {
        if ((bVal = strToYesNo(sValue)) != pLdr->enableAnnotationScale())
          pLdr->setEnableAnnotationScale(bVal);
      }
      else if (sName == L"Annotative scale")
      {
        ODA_ASSERT_ONCE(pLdr->enableAnnotationScale());
        OdDbObjectContextManagerPtr pMngr = (OdDbObjectContextManager*)pLdr->getContextDataManager();
        if (pMngr.get() && pLdr->enableAnnotationScale())
        {
          OdDbObjectContextCollection* pCtxCollection = pMngr->contextCollection(ODDB_ANNOTATIONSCALES_COLLECTION);
          ODA_ASSERT_ONCE(pCtxCollection);
          OdDbObjectContextPtr pCtxPrev = pCtxCollection->currentContext(pLdr);
          OdString sAnnoPrev = pCtxPrev->getName();
          if (!sValue.isEmpty() && sValue != sAnnoPrev)
          {
            OdString sAnnoLow(sValue); 
            if (sAnnoLow.makeLower().find(L"add") >= 0)
            {
              ODA_FAIL_ONCE(); // TODO
            }
            else if (sAnnoLow.find(L"delete") >= 0)
            {
              ODA_FAIL_ONCE(); // TODO
            }
            else
            {
              OdDbObjectContextPtr pCtx = pCtxCollection->getContext(sValue);
              ODA_ASSERT_ONCE(pCtx.get() && pCtx.get() != pCtxPrev.get());
              ODA_ASSERT_VAR(OdResult es =)
              pCtxCollection->setCurrentContext(pCtx);
              ODA_ASSERT_ONCE(es == eOk);
            }
          }
        }
      }
      else
      {
        ODA_FAIL_ONCE();
        return false;
      }
      return true;
    }

    if (sCat == L"Leaders")
    {
      OdDbMLeaderPtr pLdr = object(); pLdr->upgradeOpen();
      OdPropServicesPtr pPropServ = propServ();
      if (sName == L"Leader type")
      {
        OdDbMLeaderStyle::LeaderType tpPrev = pLdr->leaderLineType(), tp;
        if ((tp = strToLeaderLineType(sValue, tpPrev)) != tpPrev)
          pLdr->setLeaderLineType(tp);
      }
      else if (sName == L"Leader color" && pPropServ.get())
      {
        OdCmColor colorPrev = pLdr->leaderLineColor(), color;
        if (pPropServ->colorBaseByStr(baseDatabase(), sValue, color) && color != colorPrev)
          pLdr->setLeaderLineColor(color);
      }
      else if (sName == L"Leader linetype" && pPropServ.get())
      {
        OdDbObjectId idPrev = pLdr->leaderLineTypeId(); OdDbStub* id = idPrev;
        if (pPropServ->linetypeByStr(baseDatabase(), sValue, &id) && id != idPrev)
          pLdr->setLeaderLineTypeId(OdDbObjectId(id));
      }
      else if (sName == L"Leader lineweight" && pPropServ.get())
      {
        OdDb::LineWeight lwPrev = pLdr->leaderLineWeight(), lw = lwPrev;
        if (pPropServ->lineweightByStr(baseDatabase(), sValue, lw) && lw != lwPrev)
          pLdr->setLeaderLineWeight(lw);
      }
      else if (sName == L"Arrowhead")
        return strToArrowBlockId(pLdr, sValue);
      else if (sName == L"Arrowhead Size")
      {
        double dSize = toDistance(sValue);
        if (!OdEqual(dSize, pLdr->arrowSize()))
          pLdr->setArrowSize(dSize);
      }
      else if (sName == L"Horizontal Landing")
      {
        if ((bVal = strToYesNo(sValue)) != pLdr->enableLanding())
          pLdr->setEnableLanding(bVal); // TODO? && pLdr->setEnableDogleg();
      }
      else if (sName == L"Landing Distance")
      {
        double dLen = toDistance(sValue);
        if (!OdEqual(dLen, pLdr->doglegLength()))
          pLdr->setDoglegLength(dLen);
      }
      else if (sName == L"Leader extension")
      {
        if ((bVal = strToYesNo(sValue)) != pLdr->extendLeaderToText())
          pLdr->setExtendLeaderToText(bVal);
      }
      else
      {
        ODA_FAIL_ONCE();
        return false;
      }
      return true;
    }

    if (sCat == L"Text")
    {
      OdDbMLeaderPtr pLdr = object(); pLdr->upgradeOpen();
      OdDbMTextPtr pMText = pLdr->mtext();
      if (sName == L"Contents")
      {
        pMText->setContents(sValue);
        pLdr->setMText(pMText);
      }
      else if (sName == L"Text style")
      {
        OdDbObjectId idTextStyle = strToStyleId<OdDbTextStyleTableRecord>(pLdr->database()->getTextStyleTableId(), sValue);
        if (!idTextStyle.isNull() && idTextStyle != pMText->textStyle())
        {
          pMText->setTextStyle(idTextStyle);
          pLdr->setMText(pMText);
        }
      }
      else if (sName == L"Justify")
      {
        OdDbMLeaderStyle::TextAlignmentType tpDefVal = pLdr->textAlignmentType(),
                                            tp = strToTextAlignmentType(sValue, tpDefVal);
        if (tp != tpDefVal)
          pLdr->setTextAlignmentType(tp);
      }
      else if (sName == L"Direction")
        strToTextDirection(pLdr, sValue);
      else if (sName == L"Line space style")
      {
        OdDb::LineSpacingStyle stPrev = pMText->lineSpacingStyle(),
                               st = strToLineSpacingStyle(sValue, stPrev);
        if (st != stPrev)
        {
          pMText->setLineSpacingStyle(st);
          pLdr->setMText(pMText);
        }
      }
      else if (sName == L"Background mask")
      {
        if ((bVal = strToYesNo(sValue)) != pMText->backgroundFillOn())
        {
          pMText->setBackgroundFill(bVal);
          if (bVal && !pMText->useBackgroundColorOn())
            pMText->setUseBackgroundColor(true);
          pLdr->setMText(pMText);
        }
      }
      else if (sName == L"Attachment type")
        strToTextAttachmentDirection(pLdr, sValue);
      else if (sName == L"Left Attachment")
        strToTextAttachmentType(pLdr, sValue, true);
      else if (sName == L"Right Attachment")
        strToTextAttachmentType(pLdr, sValue, false);
      else if (sName == L"Width")
      {
        pMText->setWidth(toDistance(sValue));
        pLdr->setMText(pMText);
      }
      else if (sName == L"Height")
        pLdr->setTextHeight(toDistance(sValue));
      else if (sName == L"Rotation")
      {
        pMText->setRotation(toAngle(sValue));
        pLdr->setMText(pMText);
      }
      else if (sName == L"Line space factor")
      {
        pMText->setLineSpacingFactor(toDistance(sValue));
        pLdr->setMText(pMText);
      }
      else if (sName == L"Landing gap")
        pLdr->setLandingGap(toDistance(sValue));
      else if (sName == L"Text frame")
      {
        if ((bVal = strToYesNo(sValue)) != pLdr->enableFrameText())
          pLdr->setEnableFrameText(bVal);
      }
      else
      {
        ODA_FAIL_ONCE();
        return false;
      }
      return true;
    }

    if (sCat == L"Block")
    {
      OdDbMLeaderPtr pLdr = object(); pLdr->upgradeOpen();
      if (sName == L"Source block")
        strToBlockId(pLdr, sValue);
      else if (sName == L"Attachment")
      {
        OdDbMLeaderStyle::BlockConnectionType tpDef = pLdr->blockConnectionType(),
                                              tp = strToBlockConnectionType(sValue, tpDef);
        if (tp != tpDef)
          pLdr->setBlockConnectionType(tp);
      }
      else if (sName == L"Scale")
      {
        double dScale = toDistance(sValue);
        if (dScale > 0)
        {
          OdGeScale3d scaleDef = pLdr->blockScale(), scale(dScale);
          if (scaleDef.sx < 0)
            scale.sx = -scale.sx;
          if (scaleDef.sy < 0)
            scale.sy = -scale.sy;
          if (scale != scaleDef)
            pLdr->setBlockScale(scale);
        }
      }  
      else
      {
        ODA_FAIL_ONCE();
        return false;
      }
      return true;
    }

    return false;
  }

  virtual bool getPropertyPossibleValues(const OdString& sPath,
                                         OdStringArray& values,
                                         OdStringArray&) // iconPaths
  {
    OdString sName = sPath, sCat = extractFirst(sName);
    if (sCat == L"Misc")
    {
      OdDbMLeaderPtr pLdr = object();
      if (sName == L"Multileader style")
        return getDicRecNames<OdDbMLeaderStyle>(pLdr->database()->getMLeaderStyleDictionaryId(false), values);
      if (sName == L"Annotative")
        return getYesNo(values);
      if (sName == L"Annotative scale")
      {
        ODA_ASSERT_ONCE(pLdr->enableAnnotationScale());
        OdDbObjectContextManagerPtr pMngr = (OdDbObjectContextManager*)pLdr->getContextDataManager();
        if (pMngr.get() && pLdr->enableAnnotationScale())
        {
          OdDbObjectContextCollection* pCtxCollection = pMngr->contextCollection(ODDB_ANNOTATIONSCALES_COLLECTION);
          ODA_ASSERT_ONCE(pCtxCollection);
          for (OdDbObjectContextCollectionIteratorPtr pItr = pCtxCollection->newIterator();
               !pItr->done(); pItr->next())
          {
            OdDbObjectContextPtr pCtx = pItr->getContext();
            OdString sAnno = pCtx->getName();
            values.push_back(sAnno);
          }
          // TODO // values.push_back(L"add..."); values.push_back(L"delete");
        }
        return true;
      }
    }

    if (sCat == L"Leaders")
    {
      if (sName == L"Leader type")
        return getLeaderLineTypes(values);
      if (sName == L"Arrowhead")
        return getArrowBlockNames(baseDatabase(), values);
      if (sName == L"Horizontal Landing" || sName == L"Leader extension")
        return getYesNo(values);
    }

    if (sCat == L"Text")
    {
      OdDbMLeaderPtr pLdr = object();
      if (sName == L"Text style")
        return getStyleNames<OdDbTextStyleTableRecord>(pLdr->database()->getTextStyleTableId(), values);
      if (sName == L"Justify")
        return getTextAlignmentTypes(values);
      if (sName == L"Direction")
        return getTextDirections(values);
      else if (sName == L"Line space style")
        return getLineSpacingStyles(values);
      else if (sName == L"Background mask")
        return getYesNo(values);
      if (sName == L"Attachment type")
        return getTextAttachmentDirections(values);
      if (sName == L"Left Attachment" || sName == L"Right Attachment")
        return getTextAttachmentTypes(pLdr, values);
      else if (sName == L"Text frame")
        return getYesNo(values);
    }

    if (sCat == L"Block")
    {
      OdDbMLeaderPtr pLdr = object();
      if (sName == L"Source block")
        return getBlockNames(baseDatabase(), values);
      else if (sName == L"Attachment")
        return getBlockConnectionTypes(values);
    }

    return false;
  }

  // OdRxObjectImpl<OdDbXmlPropMLeader, OdPropBase>::createObject(); // is used to create it
};

class OdDbXmlPropViewBorder : public OdPropBase
{
public:
  virtual bool getPropertyValues(OdPropContext& ctx)
  {
    OdDbViewBorderPtr pEnt = object();

    OdPropContext ctxGen = ctx.addCategory(L"General");
    ctxGen.addProperty(L"Linetype").skip();
    ctxGen.addProperty(L"LinetypeScale").skip();
    ctxGen.addProperty(L"Lineweight").skip();
    ctxGen.addProperty(L"Transparency").skip();
    ctx.addCategory(L"3D Visualization").skip();

    OdPropContext ctxGeom = ctx.addCategory(L"Geometry");
    OdGePoint3d ptIns = pEnt->insertionPoint();
    ctxGeom.addProperty(L"Insertion point X", ptIns.x, tDistance).comment(L"Specifies the X coordinate of the Drawing View's center point.");
    ctxGeom.addProperty(L"Insertion point Y", ptIns.y, tDistance).comment(L"Specifies the Y coordinate of the Drawing View's center point.");
    ctxGeom.addProperty(L"Insertion point Z", ptIns.z, tDistance).readOnly().comment(L"Specifies the Z coordinate of the Drawing View's center point.");
    ctxGeom.addProperty(L"Height", pEnt->height(), tDistance).readOnly().comment(L"Specifies the height of the Drawing View.");
    ctxGeom.addProperty(L"Width", pEnt->width(), tDistance).readOnly().comment(L"Specifies the width of the Drawing View.");

    OdPropContext ctxMisc = ctx.addCategory(L"Misc");
    ctxMisc.addProperty(L"Source", sourceTypeToStr(pEnt)).readOnly().comment(L"Specifies the source of the Drawing View.");
    OdString sInventorPath = pEnt->inventorFileReference();
    if (!sInventorPath.isEmpty())
    {
      OdString sName = sInventorPath;
      sName.replace(L'\\', L'/');
      int pos = sName.reverseFind(L'/');
      if (pos > 0)
        sName = sName.mid(++pos);
      if ((pos = sName.reverseFind(L'.')) > 0)
        sName = sName.left(pos);
      ctxMisc.addProperty(L"Name", sName).readOnly().comment(L"Specifies the name of the Inventor Model file.");
      ctxMisc.addProperty(L"Path", sInventorPath).readOnly().comment(L"Specifies the path of the Inventor Model file.");
    }
    OdString sViewType = viewTypeToStr(pEnt);
    ctxMisc.addProperty(L"View type", sViewType).readOnly().comment(L"Specifies the type of the Drawing View.");
    if (pEnt->isFirstAngleProjection())
      ctxMisc.addProperty(L"Projection angle", L"First angle").readOnly().comment(L"Specifies projection type.");
    //if (sViewType == L"Projected")
    //  ctxMisc.addProperty(L"View alignment", L"Aligned" or L"Broken").readOnly().comment(L"Specifies the aligment used by the Drawing View.");
    ctxMisc.addProperty(L"Rotation", pEnt->rotationAngle(), tAngle).comment(L"Specifies the rotation angle of the Drawing View.");

    OdPropContext ctxApp = ctx.addCategory(L"Appearance");
    OdString sHiddenLines = hiddenLinesToStr(pEnt);
    ctxApp.addProperty(L"Hidden lines", sHiddenLines).readOnly().comment(L"Specifies the view style used the Drawing View.");
    ctxApp.addProperty(L"Standard scale", scaleToStr(pEnt), tCombo).comment(L"Specifies the standard scale of the Drawing View.");
    ctxApp.addProperty(L"Custom scale", pEnt->scale(), tDouble).readOnly().comment(L"Specifies the custom scale of the Drawing View.");
    OdString sDpi; sDpi.format(L"%d DPI", pEnt->shadedDPI());
    ctxApp.addProperty(L"Shade resolution", sDpi).readOnly().comment(L"Specifies the DPI for shaded Drawing Views.");
    //ctxApp.addProperty(L"Tangent edges",    // "Off"
    //ctxApp.addProperty(L"Tangent edges foreshortened", // "Off"
    //ctxApp.addProperty(L"Justification",    // "Fixed"
    return true; 
  }

  virtual bool setPropertyValue(const OdString& sPath,
                                const OdString& sValue,
                                Action& action)
  {
    OdString sName = sPath, sCat = extractFirst(sName);

    if (sCat == L"Geometry") 
    {
      OdDbViewBorderPtr pEnt = object(); pEnt->upgradeOpen();
      OdGePoint3d ptInsPrev = pEnt->insertionPoint(),
                  ptIns = ptInsPrev;
      if (sName == L"Insertion point X")
        ptIns.x = toDistance(sValue);
      else if (sName == L"Insertion point Y")
        ptIns.y = toDistance(sValue);
      else
      {
        ODA_FAIL_ONCE();
        return false;
      }
      if (ptIns != ptInsPrev)
      {
        OdGeVector3d vOffset = ptIns - ptInsPrev;
        OdGeMatrix3d mTran; mTran.setToTranslation(vOffset);

        if (pEnt->transformBy(mTran) == eOk)
        {
          ODA_ASSERT_VAR(OdGePoint3d ptInsNew = pEnt->insertionPoint();)
          ODA_ASSERT_ONCE(ptIns == ptInsNew);
          return true;
        }
      }
      return false;
    }
    if (sCat == L"Misc" && sName == L"Rotation")
    {
      double dAng = toAngle(sValue);
      OdDbViewBorderPtr pEnt = object(); pEnt->upgradeOpen();
      double dAngPrev = pEnt->rotationAngle();
      if (!OdEqual(dAng, dAngPrev))
      {
        OdGeVector3d vNormal = OdGeVector3d::kZAxis;
        OdGePoint3d ptCenter = pEnt->centerPoint3d(); // = OdGePoint3d::kOrigin; 
        OdGeMatrix3d mRot;
        mRot.setToRotation(dAng - dAngPrev, vNormal, ptCenter);
        if (pEnt->transformBy(mRot) == eOk)
        {
          ODA_ASSERT_VAR(double dAngNew = pEnt->rotationAngle();)
          ODA_ASSERT_ONCE(OdEqual(dAng, dAngNew));
          return true;
        }
      }
      return false;
    }
    if (sCat == L"Appearance" && sName == L"Standard scale")
    {
      OdDbViewBorderPtr pEnt = object(); pEnt->upgradeOpen();
      double dScale = strToScale(baseDatabase(), sValue, pEnt->scale()),
             dScalePrev = pEnt->scale();
      if (!OdEqual(dScale, dScalePrev))
      {
        OdDbViewportPtr pViewPort = OdDbViewport::cast(pEnt->viewportId().openObject(OdDb::kForWrite));
        ODA_ASSERT_ONCE(pViewPort.get() && OdEqual(dScalePrev, pViewPort->customScale()));
        pViewPort = NULL;

        if (::odrxDynamicLinker()->getModule(DbConstraintsModuleName).isNull())
          return false; // unsupported without s

        pEnt->setScale(dScale); // see also pViewRep->setScaleFromParent(false); in OdDbImpAssocViewRepActionBody::evaluateOverride
        ODA_ASSERT_ONCE(OdEqual(dScale, pEnt->scale()));
        SETBIT(*(int*)&action, OdPropSource::enAction_evaluateTopLevelNetwork, true);
        pEnt = NULL; // set state by close // but ? OdDbAssocDependency::notifyDependenciesOnObject(pEnt, kChangedDirectlyAssocStatus) contains TODO
        //if (::odrxDynamicLinker()->getModule(DbConstraintsModuleName).isNull())
        //  return true;
        //OdDbDatabase* pDb = OdDbDatabase::cast(baseDatabase()).get();
        //OdDbAssocManager::evaluateTopLevelNetwork(pDb);

//#if defined(TEST_BY_VIEWPORT) && defined(_DEBUG)
//        OdDbDatabase* pDb = OdDbDatabase::cast(baseDatabase()).get();
//
//        OdDbObjectIdArray idsDep;
//        OdDbAssocStatus st = getActionState(object(), &idsDep);
//        ODA_ASSERT_ONCE(st == kChangedDirectlyAssocStatus);
//        for (unsigned int idx = 0, sz = idsDep.size(); idx < sz; idx++)
//        {
//          OdDbObjectId idDep = idsDep[idx];
//          st = getActionStateByDep(idDep);
//          ODA_ASSERT_ONCE(st == kChangedDirectlyAssocStatus);
//        }
//        OdDbAssocManager::evaluateTopLevelNetwork(pDb);
//
//        if (pEnt.isNull())
//          pEnt = object(), pEnt->upgradeOpen();
//        if (pViewPort.isNull())
//          pViewPort = OdDbViewport::cast(pEnt->viewportId().openObject(OdDb::kForWrite));
//        if (pViewPort.get())
//        {
//          double dVpScale = pViewPort->customScale();
//          ODA_ASSERT_ONCE(OdEqual(dScale, dVpScale));
//          //if (!OdEqual(dScale, dVpScale))
//          //  pViewPort->setCustomScale(dScale);
//        }
//#endif
      }
      return true;
    }

    return false;
  }

  virtual bool getPropertyPossibleValues(const OdString& sPath,
                                         OdStringArray& values,
                                         OdStringArray&) // iconPaths
  {
    OdString sName = sPath, sCat = extractFirst(sName);
    if (   sCat == L"Appearance" && sName == L"Standard scale"
        && getAnnoscaleNames(baseDatabase(), values))
      return true;
    return false;
  }

  // OdRxObjectImpl<OdDbXmlPropViewBorder, OdPropBase>::createObject(); // is used to create it
};

///////////////////////////////////////////////////////////////////////////////

inline OdString removeQuotes(const OdString& cStr)
{
  OdString str(cStr);
  if (str.trimLeft().trimRight().getAt(0) != L'\"')
    return str;
  int pos = (str = str.mid(1)).reverseFind(L'\"');
  ODA_ASSERT_ONCE(pos >= 0);
  if (pos >= 0)
    str = str.left(pos);
  return str;
}

inline OdString toUI(const OdString& cStr)
{
  if (cStr.findOneOf(L"\"&\'\r") < 0)
    return cStr;
  OdString str(removeQuotes(cStr));
  str.replace("&", "&amp;"); str.replace("\"", "&quot;");
  str.replace("\'", "&apos;"); str.replace("\r", "&#x0D;");
  str.replace("<", "&lt;"); str.replace(">", "&gt;"); str.replace("\n", "&#x0A;");
  return str;
}

static 
OdRxCollectionProperty* getCollectionPropObj(OdRxObject* pRxDb, OdRxMember* pMember, // in
                                             OdRxObjectPtr& pCollectionPropObj) // out
{
  ODA_ASSERT_ONCE(pRxDb && pMember);
  pCollectionPropObj = pRxDb;

  OdRxCollectionProperty* pCollection = OdRxCollectionProperty::cast(pMember).get();
  if (pCollection)
    return pCollection;

  OdRxRefersToAttribute* pRefers = OdRxRefersToAttribute::cast(pMember->attributes().get(OdRxRefersToAttribute::desc())).get();
  if (!pRefers)
    return pCollection;

  OdString sPath = removeQuotes(pRefers->path());
  if (sPath.isEmpty())
    return pCollection;
  // possible TODO via OdRxRefersToAttribute::parseReference(sPath, pRxObj) when it will be implemented

  OdStringArray pathNames;
  while (!sPath.isEmpty())
    pathNames.push_back(OdPropSource::extractFirst(sPath));
  if (pathNames.size() && pathNames.first().isEmpty())
    pathNames.removeFirst();
  int szNames = (int) pathNames.size();

  if (szNames <= 1)
  {
    if (szNames == 1)
      pCollection = OdRxCollectionProperty::cast(OdRxMemberQueryEngine::theEngine()->find(pRxDb, pathNames[0])).get();
    return pCollection;
  }

  int idx = 0, sz = (int)pathNames.size();
  for (; idx < sz; idx++)
  {
    OdRxMemberPtr pCurProp = OdRxMemberQueryEngine::theEngine()->find(pRxDb, pathNames[idx]);
    if (pCurProp.isNull())
      continue;
    OdRxPropertyPtr pIdProp = OdRxProperty::cast(pCurProp);
    if (pIdProp.isNull())
      continue;

    OdRxValue valId;
    if (pIdProp->getValue(pCollectionPropObj, valId) == eOk)
    {
      if (valId.type().reference()) // for ids
      {
        pCollectionPropObj = valId.type().reference()->dereference(valId, IOdRxReferenceType::kForRead);
        pCurProp = OdRxMemberQueryEngine::theEngine()->find(pCollectionPropObj, pathNames[++idx]);
      }
    }

    pCollection = OdRxCollectionProperty::cast(pCurProp).get();
    if (pCollection)
      break;
  }

  return pCollection;
}

static
bool getCollectionData(OdRxObjectPtr pCollectionPropObj, OdRxCollectionProperty* pCollection,
                       OdRxValue value, OdString& sValue,
                       OdStringArray* pStrValues = NULL, 
                       std::map<OdString, OdRxValue>* pMapToValue = NULL)
                       //,OdArray<OdRxValue>* pValues = NULL
                       //,std::map<OdRxValue, OdString>* pMapToStr = NULL)
{
  ODA_ASSERT(pCollection && pCollectionPropObj.get());
  if (!pCollection || pCollectionPropObj.isNull())
    return false;

  bool bRes = false;
  OdRxValueIteratorPtr pValIter = pCollection->newValueIterator(pCollectionPropObj);
  for (; !pValIter->done(); pValIter->next())
  {
    OdRxValue iterValue = pValIter->current();

    const IOdRxReferenceType* pRefType = iterValue.type().reference();
    if (!pRefType)
    {
      OdString sVal = iterValue.toString();
      ODA_ASSERT_ONCE(!sVal.isEmpty()); // test of existance

      if (value == iterValue)
      {
        bRes = true;
        sValue = sVal;
      }
      if (pStrValues)
      {
        if (!pStrValues->contains(sVal))
          pStrValues->push_back(sVal);
        ODA_ASSERT_VAR(else)
          ODA_ASSERT(true); // DGN color problem
      }
      if (pMapToValue)
        (*pMapToValue)[sVal] = iterValue;
      continue;
    }

    // for ids

    OdRxObjectPtr pItemObj = pRefType->dereference(iterValue, IOdRxReferenceType::kForRead);

    OdString strTotal;
    if (pItemObj.isNull())
      ODA_ASSERT(true); // brk //strTotal = L"NULL";

    OdRxPropertyPtr pNameProp = OdRxProperty::cast(OdRxMemberQueryEngine::theEngine()->find(pItemObj, OdString(L"Name")));
    if (pNameProp.get())
    {
      OdRxValue valName;
      if (pNameProp->getValue(pItemObj, valName) == eOk)
      {
        if (OdString* pName = rxvalue_cast<OdString>(&valName))
          strTotal = *pName;
        else if (const OdChar** pName = rxvalue_cast<const OdChar*>(&valName))
          strTotal = *pName;
      }
    }
    if (strTotal.isEmpty())
    {
      OdRxPropertyPtr pIndexProp = OdRxProperty::cast(OdRxMemberQueryEngine::theEngine()->find(pItemObj, OdString(L"Index")));
      if (pIndexProp.get())
      {
        OdRxValue valIndex;
        if (pIndexProp->getValue(pItemObj, valIndex) == eOk)
        {
          OdUInt32* pIndex = rxvalue_cast<OdUInt32>(&valIndex);
          if (pIndex)
            strTotal.format(L"%d", *pIndex);
        }
      }
    }

    if (value == iterValue)
    {
      bRes = true;
      sValue = strTotal;
    }
    if (pStrValues)
    {
      ODA_ASSERT_ONCE(!pStrValues->contains(strTotal));
      pStrValues->push_back(strTotal);
    }
    if (pMapToValue)
      (*pMapToValue)[strTotal] = iterValue;
  } // for

  return bRes;
}

static
bool getEnumerationData(OdRxMember* pMember,
                        OdRxValue value, OdString& sValue,
                        OdStringArray* pStrValues = NULL,
                        std::map<OdString, OdRxValue>* pMapToValue = NULL,
                        bool bRemoveCommonNamePreffix = true)
{
  const IOdRxEnumeration* pEnum = value.type().enumeration();
  if (!pEnum)
    return false;
  bool bRes = false;
  int idxEn = 0, szEn = pEnum->count();
  OdString sVal, sPreffix;
  for (; idxEn < szEn; idxEn++)
  {
    const OdRxEnumTag& tag = pEnum->getAt(idxEn);

    OdString sVal = toUI(tag.name()),
             sVisVal = toUI(tag.localName());
    if (sVisVal.isEmpty())
      sVisVal = toUI(tag.alternateLocalName());
    if (!sVisVal.isEmpty())
      sVal = sVisVal;
    if (sVal.isEmpty())
      sVal = toUI(value.toString());
    ODA_ASSERT_ONCE(!sVal.isEmpty());

    if (bRemoveCommonNamePreffix)
    {
      if (!idxEn)
        sPreffix = sVal;
      else if (!sPreffix.isEmpty() && sVal.find(sPreffix))
      {
        int idx = 0, sz = sPreffix.getLength(), szVal = sVal.getLength();
        if (sz > szVal)
          sz = szVal;
        for (; idx < sz; idx++)
        {
          if (sVal.getAt(idx) != sPreffix.getAt(idx))
            break;
        }
        sPreffix = sPreffix.left(idx);
      }
    }

    if (pStrValues)
      pStrValues->push_back(sVal);
    if (pMapToValue)
      (*pMapToValue)[sVal] = tag.value();

    if (value != tag.value())
      continue;

    bRes = true;
    sValue = sVal;
    if (!pStrValues && !pMapToValue && sPreffix.isEmpty())
      break;
  }

  int lenPrf = sPreffix.getLength();
  if (!bRes || !lenPrf)
    return bRes;

  sValue = sValue.mid(lenPrf);
  if (pStrValues)
  {
    for (int idx = 0, sz = pStrValues->size(); idx < sz; idx++)
    {
      OdString& sVal = (*pStrValues)[idx];
      sVal = sVal.mid(lenPrf);
    }
  }
  if (pMapToValue)
  {
    std::map<OdString, OdRxValue> map;
    for (std::map<OdString, OdRxValue>::iterator pItr = pMapToValue->begin(); 
         pItr != pMapToValue->end(); pItr++)
    {
      OdString sVal = pItr->first.mid(lenPrf);
      map[sVal] = pItr->second;
    }
    (*pMapToValue) = map;
  }
  return bRes;
}

static 
bool getCollectionData(OdRxObject* pRxDb, OdRxMember* pMember, OdRxValue value, // in
                       OdString& sValue,
                       OdStringArray* pStrValues = NULL,
                       std::map<OdString, OdRxValue>* pMapToValue = NULL)
                       //,OdArray<OdRxValue>* pValues = NULL
                       //,std::map<OdRxValue, OdString>* pMapToStr = NULL)
{
  OdString sVal;
  OdRxObjectPtr pCollectionPropObj;
  OdRxCollectionProperty* pCollection = getCollectionPropObj(pRxDb, pMember,
                                                             pCollectionPropObj); // out
  if (!pCollection || pCollectionPropObj.isNull())
  {
    if (getEnumerationData(pMember, value, sValue, pStrValues, pMapToValue))
      return true;
    return false;
  }

  int count = 0;
  OdResult res = pCollection->tryGetCount(pCollectionPropObj, count);
  ODA_ASSERT_ONCE(res == eOk && count);
  if (!count)
    return false;

  if (!getCollectionData(pCollectionPropObj, pCollection, value, 
                         sVal, pStrValues, pMapToValue)) //, pValues, pMapToStr))
    return false;

  sValue = sVal;
  return true;
}

static OdGePoint3d odStrToPoint(const OdString& csValue, OdResult* pRes = NULL)
{
  if (pRes)
    *pRes = eInvalidInput;
  OdGePoint3d pt;
  OdString sX, sY, sZ = csValue;
  if (   (sX = OdPropSource::extractFirst(sZ, L',', true)).isEmpty()
      || (sY = OdPropSource::extractFirst(sZ, L',', true)).isEmpty())
  {
    return pt;
  }
  OdChar* pEnd = NULL;
  if (!sZ.isEmpty())
  {
    pt.z = odStrToD(sZ, &pEnd);
    if (pEnd && *pEnd)
      return pt;
  }
  pt.y = odStrToD(sY, &pEnd);
  if (pEnd && *pEnd)
    return pt;
  pt.x = odStrToD(sX, &pEnd);
  if (pEnd && *pEnd)
    return pt;
  if (pRes)
    *pRes = eOk;
  return pt;
}

class OdDbXmlPropCdaBased : public OdPropBase
{
  bool getCategories(OdStringArray* pCategories = NULL, 
                     std::map<OdString, OdArray<OdRxMemberPtr> >* pMapCatMembers = NULL,
                     std::map<OdString, OdRxMemberPtr>* pMapPathToMember = NULL)
  {
    OdRxObjectPtr pRxObj = object();
    OdRxMemberIteratorPtr pItr = OdRxMemberQueryEngine::theEngine()->newMemberIterator(pRxObj);
    bool bRes = false;
    for (; !pItr->done(); pItr->next())
    {
      OdRxMember* pMember = pItr->current();
      ODA_ASSERT_ONCE(pMember);
      if (!pMember)
        continue;
      OdString sName = toUI(pMember->name());
      ODA_ASSERT_ONCE(!sName.isEmpty());
      if (sName.isEmpty())
        continue;

      if (OdRxProperty* pProperty = OdRxProperty::cast(pMember).get())
      {
        OdRxValue value;
        if (pProperty->getValue(pRxObj, value) != eOk)
          continue; // unfortunately it maybe so for members of AnnotationScaling category
      }

      OdString sCat = toUI(OdRxUiPlacementAttribute::getCategory(pMember));
      if (sCat.isEmpty() && isPropSrcFlagsOn(enPropSrcBased_CdaSkipEmptyCategory))
        continue;

      propServ()->overrideCategoryName(baseDatabase(), sCat, sName, propSrcFlags());
      propServ()->overridePropertyName(baseDatabase(), sCat, sName);

      if (sCat.isEmpty() || sName.isEmpty())
        continue;

      bRes = true;

      float weight = 0.0;
      if (pMapCatMembers)
      {
        OdArray<OdRxMemberPtr>& members = (*pMapCatMembers)[sCat];
        int sz = members.size();
        weight = OdRxUiPlacementAttribute::getWeight(pMember);
        if (weight == 0.0f || !sz) 
          members.push_back(pMember);
        else
        {
          // add with sorting by weight
          int idx = sz - 1;
          float wt = 0.0;
          for (; idx >= 0; idx--)
          {
            if (weight >= (wt = OdRxUiPlacementAttribute::getWeight(members[idx])))
              break;
          }
          members.insertAt(++idx, pMember); // insert after idx
        }
      }
      if (pCategories && !pCategories->contains(sCat))
      {
        int sz = pCategories->size();
        propServ()->overrideCategoryWeight(baseDatabase(), sCat, weight);
        if (weight == 0.0f || !sz)
          pCategories->push_back(sCat);
        else
        {
          // add with sorting by weight
          ODA_ASSERT_ONCE(pMapCatMembers);
          int idx = sz - 1;
          for (; idx >= 0; idx--)
          {
            const OdString& csCat = pCategories->getAt(idx);
            OdArray<OdRxMemberPtr>& members = (*pMapCatMembers)[csCat];
            ODA_ASSERT_ONCE(members.size());
            float wt = OdRxUiPlacementAttribute::getWeight(members[0]);
            propServ()->overrideCategoryWeight(baseDatabase(), csCat, wt);
            if (weight >= wt)
              break;
          }
          pCategories->insertAt(++idx, sCat); // insert after idx
        }
      }
      if (pMapPathToMember)
      {
        OdString sPath = sCat + L"/" + sName;
        (*pMapPathToMember)[sPath] = pMember;
      }
    }
    return bRes;
  }

  OdRxMemberPtr getMember(const OdString& sPath)
  {
    OdRxMemberPtr pMember;
    std::map<OdString, OdRxMemberPtr> mapPathToMember;
    if (!getCategories(NULL, NULL, &mapPathToMember))
      return pMember;
    std::map<OdString, OdRxMemberPtr>::iterator pItr = mapPathToMember.find(sPath);
    ODA_ASSERT_ONCE(   pItr != mapPathToMember.end() 
                    || isPropSrcFlagsOn(enPropSrcBased_XmlPattern)
                    || isPropSrcFlagsOn(enPropSrcBased_RegistredXmlSrc));
    if (pItr == mapPathToMember.end())
      return pMember;
    pMember = pItr->second.get();
    ODA_ASSERT_ONCE(pMember.get());
    return pMember;
  }

public:
  virtual bool getPropertyValues(OdPropContext& ctx)
  {
    OdRxObjectPtr pRxObj = object();

    OdStringArray categories;
    std::map<OdString, OdArray<OdRxMemberPtr> > mapCatMembers;
    if (!getCategories(&categories, &mapCatMembers))
      return false;

    for (int idxCat = 0, szCat = (int) categories.size(); idxCat < szCat; idxCat++)
    {
      const OdString sCat = categories[idxCat];
      ODA_ASSERT_ONCE(!sCat.isEmpty());
      if (sCat.isEmpty())
        continue;
      OdArray<OdRxMemberPtr>& members = mapCatMembers[sCat];
      int szMemb = (int) members.size();
      ODA_ASSERT_ONCE(szMemb);
      if (!szMemb)
        continue;

      OdString sCatVisName = sCat;
      //sCatVisName.empty();
      //if (!propServ()->overridePropertyName(baseDatabase(), sCat, sCatVisName))
      //  sCatVisName = sCat;
      //ODA_ASSERT_ONCE(!sCatVisName.isEmpty());
      //sCatVisName = toUI(sCatVisName);

      if (isPropSrcFlagsOn(enPropSrcBased_ShowWeightAsNamePrefix))
      {
        float weight = OdRxUiPlacementAttribute::getWeight(members[0]);
        propServ()->overrideCategoryWeight(baseDatabase(), sCat, weight);
        sCatVisName = OdString().format(L"%g: %ls", weight, 
                                        sCatVisName.isEmpty() ? sCat.c_str()
                                                              : sCatVisName.c_str());
      }

      OdPropContext ctxCat = ctx.addCategory(sCat);
      if (!sCatVisName.isEmpty() && sCatVisName != sCat)
        ctxCat.visName(sCatVisName);

      for (int idxMemb = 0; idxMemb < szMemb; idxMemb++)
      {
        OdRxMember* pMember = members[idxMemb].get();
        ODA_ASSERT_ONCE(pMember);
        if (!pMember)
          continue;

        OdString sName = toUI(pMember->name());
        ODA_ASSERT_ONCE(!sName.isEmpty());
        if (sName.isEmpty())
          continue;

        ODA_ASSERT_VAR(static char nameToCatch[16] = "\0";)
        ODA_ASSERT_VAR(if (nameToCatch[0] && !OdString(sName).makeLower().find(OdString(nameToCatch).makeLower())))
          ODA_ASSERT(true); // brk

        OdString sVisName = pMember->localName();
        if (sVisName.isEmpty())
          sVisName = OdRxLocalizedNameAttribute::getLocalizedName(pRxObj);

        propServ()->overridePropertyName(baseDatabase(), sCat, sName, &sVisName);

        if (sVisName.isEmpty())
          sVisName = sName;
        sVisName = toUI(sVisName);
        if (isPropSrcFlagsOn(enPropSrcBased_ShowWeightAsNamePrefix))
        {
          float weight = OdRxUiPlacementAttribute::getWeight(pMember);
          sVisName = OdString().format(L"%g: %ls", weight, sVisName.isEmpty() ? sName.c_str() 
                                                                              : sVisName.c_str());
        }

        OdString sDescription = toUI(OdRxDescriptionAttribute::getDescription(pRxObj));

        OdRxProperty* pProperty = OdRxProperty::cast(pMember).get();
        //ODA_ASSERT_ONCE(pProperty);
        if (!pProperty)
        {
#ifdef _DEBUG
          OdRxMethod* pMethod = OdRxMethod::cast(pMember).get();
          if (pMethod)
            ODA_ASSERT(true); // brk // TODO
          OdRxEnumTag* pEnum = OdRxEnumTag::cast(pMember).get();
          if (pEnum)
            ODA_ASSERT(true); // brk // TODO ? pEnum->value() ? pEnum->alternateLocalName()

          const OdArray<OdRxMemberPtr>* pKids = pMember->children();
          if (pKids && pKids->size())
          {
            const OdRxValueType& tp = pMember->type();
            for (int idxKid = 0, szKid = (int)pKids->size(); idxKid < szKid; idxKid++)
            {
              OdString sVal = tp.toString((*pKids)[idxKid]);
              ODA_ASSERT_VAR(if (sVal != L"Not implemented yet"))
                ODA_ASSERT(true); // TODO
            }
          }
#endif
          continue;
        }

        bool bReadOnly = pProperty->isReadOnly(pMember);
        OdResult res;
        // TODO DGN property code invokes assertWriteEnabled in any case for setValue
        // thefore next ACAD way to check readonly is useless here
        //ODA_ASSERT_VAR(res = pProperty->setValue(pRxObj, value);)
        //ODA_ASSERT_ONCE(bReadOnly == (res == eNotApplicable));

        OdRxValue value;
        res = pProperty->getValue(pRxObj, value);
        ODA_ASSERT_ONCE(   res == eOk 
                        || (OdDbDatabase::cast(baseDatabase()).get() && res == eNotApplicable));
        if (res != eOk)
          continue;

        const char* tp = tString;
        OdString sValue;
        if (OdRxDisplayAsAttribute* dsa = (OdRxDisplayAsAttribute*)pProperty->attributes().get(OdRxDisplayAsAttribute::desc()))
          sValue = toUI(dsa->getDisplayValue(value));
        else
          sValue = toUI(value.toString());
        if (sValue == L"NULL")
          sValue.empty(); // NULL is correct for UI

        OdPropServicesPtr pPropServices = propServ();
        ODA_ASSERT_ONCE(pPropServices.get());

        if (bool* pBool = rxvalue_cast<bool>(&value))
        {
          tp = tBool;
        }
        else if (OdCmColor* pColor = rxvalue_cast<OdCmColor>(&value))
        {
          tp = tOdColor;
          if (pPropServices.get())
            sValue = pPropServices->colorBaseToStr(baseDatabase(), *pColor);
        }
        else if (OdCmEntityColor* pEntColor = rxvalue_cast<OdCmEntityColor>(&value))
        {
          tp = tOdColor;
          if (pPropServices.get())
          {
            OdCmColor clr;
            clr.setColor(pEntColor->color());
            sValue = pPropServices->colorBaseToStr(baseDatabase(), clr);
          }
        }
        //else  if (OdCmColorBase* pColorBase = rxvalue_cast<OdCmColorBase>(&value))
        //{
        //  tp = tOdColor;
        //  if (pPropServices.get())
        //    sValue = pPropServices->colorBaseToStr(baseDatabase(), *pColorBase);
        //}
        // tBool tLinetype tLineweight
        //sValue = pPropServices->linetypeToStr(baseDatabase(), OdDbStub* id);
        else if (OdDb::LineWeight* pLineWeight = rxvalue_cast<OdDb::LineWeight>(&value))
        {
          tp = tLineweight;
          if (pPropServices.get())
            sValue = pPropServices->lineweightToStr(baseDatabase(), *pLineWeight); // , bool bLineWeightAsIndex = false);
        }
        else if (OdCmTransparency* pTransp = rxvalue_cast<OdCmTransparency>(&value))
        {
          tp = tTransparency;
          if (pPropServices.get())
            sValue = pPropServices->transparencyToStr(baseDatabase(), *pTransp);
        }
        //const OdRxValueType& rxType = value.type();
        else
        {
          OdString sVal;
          if (getCollectionData(baseDatabase(), pMember, value, sVal))
          {
            tp = tCombo;
            sValue = sVal;
          }
          ODA_ASSERT_VAR(else)
            ODA_ASSERT_ONCE(true); // DGN color  etc
        }

        OdString sType = tp;
        propServ()->overridePropertyType(baseDatabase(), sCat, sName, sType);

        OdPropContext ctxProp = ctxCat.addProperty(sName.c_str(), sValue, sType); // tp);
        if (!sVisName.isEmpty() && sVisName != sName)
          ctxProp.visName(sVisName);
        if (bReadOnly)
          ctxProp.readOnly();
        if (!sDescription.isEmpty())
          ctxProp.comment(sDescription);

      } // for idxMemb

    } // for idxCat

    return true;
  }

  virtual bool getPropertyPossibleValues(const OdString& csPath,
                                         OdStringArray& values,
                                         OdStringArray&) // iconPaths
  {
    OdString sPath = toUI(csPath);
    OdRxMemberPtr pMember = getMember(sPath);
    OdRxProperty* pProperty = OdRxProperty::cast(pMember);
    ODA_ASSERT_ONCE(   pProperty
                    || isPropSrcFlagsOn(enPropSrcBased_XmlPattern)
                    || isPropSrcFlagsOn(enPropSrcBased_RegistredXmlSrc));
    if (!pProperty)
      return false;
    ODA_ASSERT_ONCE(pMember.get());

    OdRxObjectPtr pRxObj = object();
    OdRxValue value;
    OdResult res = pProperty->getValue(pRxObj, value);
    ODA_ASSERT_ONCE(res == eOk)
    if (res != eOk)
      return false;

    OdString sVal;
    if (!getCollectionData(baseDatabase(), pMember, value, // in
                           sVal, &values)) 
      return false;
    return true;
  }

  virtual bool setPropertyValue(const OdString& csPath, const OdString& sValue, Action& action)
  {
    OdString sPath = toUI(csPath);
    OdRxMemberPtr pMember = getMember(sPath);
    OdRxProperty* pProperty = OdRxProperty::cast(pMember);
    ODA_ASSERT_ONCE(pMember.isNull() == (pProperty == NULL));
    if (!pProperty)
      return false;

    OdPropServicesPtr pPropServices = propServ();
    if (pPropServices.isNull())
      return false;

    OdRxObjectPtr pRxObj = object(true);
    OdRxValue value;
    OdResult res = pProperty->getValue(pRxObj, value);
    ODA_ASSERT_ONCE(res == eOk);
    if (res != eOk)
      return false;
    OdRxValue prevValue(value);
    ODA_ASSERT_ONCE(value == prevValue);

    OdString sPrevVal = toUI(value.toString());
    if (OdRxDisplayAsAttribute* dsa = (OdRxDisplayAsAttribute*)pProperty->attributes().get(OdRxDisplayAsAttribute::desc()))
      sPrevVal = toUI(dsa->getDisplayValue(value));
    else
      sPrevVal = toUI(value.toString());       
    if (bool* pBool = rxvalue_cast<bool>(&value))
    {
      if (sValue == sPrevVal)
        return true;
      OdString sValLower = sValue;  sValLower.makeLower();
      bool bValue = (sValue != L"0" && sValLower != L"false");
      ODA_ASSERT_ONCE(bValue != *pBool);
      *pBool = bValue;
    }
    else if (OdCmColor* pColor = rxvalue_cast<OdCmColor>(&value))
    {
      sPrevVal = pPropServices->colorBaseToStr(baseDatabase(), *pColor);
      if (sValue == sPrevVal)
        return true;
      OdCmColor clr;
      if (!pPropServices->colorBaseByStr(baseDatabase(), sValue, clr))
      {
        action = enAction_FailedToSet;
        return false;
      }
      pColor->setColor(clr.color());
    }
    else if (OdCmEntityColor* pEntColor = rxvalue_cast<OdCmEntityColor>(&value))
    {
      OdCmColor clr;
      clr.setColor(pEntColor->color());
      sPrevVal = pPropServices->colorBaseToStr(baseDatabase(), clr);
      if (sValue == sPrevVal)
        return true;
      if (!pPropServices->colorBaseByStr(baseDatabase(), sValue, clr))
      {
        action = enAction_FailedToSet;
        return false;
      }
      pEntColor->setColor(clr.color());
    }
    //sValue = pPropServices->linetypeToStr(baseDatabase(), OdDbStub* id);
    else if (OdDb::LineWeight* pLineWeight = rxvalue_cast<OdDb::LineWeight>(&value))
    {
      sPrevVal = pPropServices->lineweightToStr(baseDatabase(), *pLineWeight); // , bool bLineWeightAsIndex = false);
      if (sValue == sPrevVal)
        return true;
      OdDb::LineWeight lw;
      if (!pPropServices->lineweightByStr(baseDatabase(), sValue, lw))
      {
        action = enAction_FailedToSet;
        return false;
      }
      *pLineWeight = lw;
    }
    else if (OdCmTransparency* pTransp = rxvalue_cast<OdCmTransparency>(&value))
    {
      sPrevVal  = pPropServices->transparencyToStr(baseDatabase(), *pTransp);
      if (sValue == sPrevVal)
        return true;
      OdCmTransparency transp;
      if (!pPropServices->transparencyByStr(baseDatabase(), sValue, transp))
      {
        action = enAction_FailedToSet;
        return false;
      }
      *pTransp = transp;
    }
    else 
    {
      std::map<OdString, OdRxValue> mapToValue;
      OdString sVal;
      if (getCollectionData(baseDatabase(), pMember, value, // in
                            sVal, NULL, &mapToValue))
      {
        if (sValue == sVal)
          return true;
        std::map<OdString, OdRxValue>::iterator pItr = mapToValue.find(sValue);
        if (pItr == mapToValue.end())
          return false;

        value = pItr->second;
      }
      else
      {
        if (sValue == sPrevVal)
          return true;

        OdResult es = eOk;
        if (char* pChar = rxvalue_cast<char>(&value))
          *pChar = (char)odStrToInt(sValue);
        else if (unsigned char* pUChar = rxvalue_cast<unsigned char>(&value))
          *pUChar = (unsigned char)odStrToInt(sValue);
        else if (short* pShort = rxvalue_cast<short>(&value))
          *pShort = (short)odStrToInt(sValue);
        else if (int* pInt = rxvalue_cast<int>(&value))
          *pInt = odStrToInt(sValue);
        else if (double* pDouble = rxvalue_cast<double>(&value))
          *pDouble = odStrToD(sValue);
        else if (OdGePoint3d* pPt = rxvalue_cast<OdGePoint3d>(&value))
          *pPt = odStrToPoint(sValue, &es);
        else if (OdGeVector3d* pVect = rxvalue_cast<OdGeVector3d>(&value))
          *pVect = odStrToPoint(sValue, &es).asVector();
        else if (OdString* pStr = rxvalue_cast<OdString>(&value))
          *pStr = sValue;
        else if (OdDbObjectId* pObjId = rxvalue_cast<OdDbObjectId>(&value))
        {
          OdDbObjectId id;
          if (!sValue.isEmpty() && sValue != L"0")
          {
            OdDbHandle handle(sValue);
            if (OdDbDatabase* pDb = OdDbDatabase::cast(baseDatabase()))
              id = pDb->getOdDbObjectId(handle);
          }
          *pObjId = id;
        }
        else
        {
          ODA_FAIL_ONCE(); // TODO
          return false;
        }
        if (es != eOk)
        {
          action = enAction_FailedToSet;
          return false;
        }
      }
    }
    if (value == prevValue)
      return true;

    res = pProperty->setValue(pRxObj, value);
    if (res != eOk)
    {
      action = enAction_FailedToSet;
      return false;
    }
    return true;
  }
};

///////////////////////////////////////////////////////////////////////////////

void registerOdDbXmlPropSources(OdPropServices* pPropSrv)
{
  ODA_ASSERT_ONCE(pPropSrv);
  if (!pPropSrv)
    return;
  pPropSrv->registerXmlProp(OdDbBlockReference::desc()->name(),
                            &OdRxObjectImpl<OdDbXmlPropBlockReference, OdPropBase>::createObject);
  pPropSrv->registerXmlProp(OdDbMLeader::desc()->name(),
                            &OdRxObjectImpl<OdDbXmlPropMLeader, OdPropBase>::createObject);
  //if (::odrxDynamicLinker()->loadModule(OdModelDocObjModuleName).get())
    pPropSrv->registerXmlProp(L"AcDbViewBorder", // OdDbViewBorder::desc()->name()
                              &OdRxObjectImpl<OdDbXmlPropViewBorder, OdPropBase>::createObject);
  pPropSrv->registerXmlProp(L"CdaBased",
                            &OdRxObjectImpl<OdDbXmlPropCdaBased, OdPropBase>::createObject);
}

void unregisterOdDbXmlPropSources(OdPropServices* pPropSrv)
{
  ODA_ASSERT_ONCE(pPropSrv);
  if (!pPropSrv)
    return;
  pPropSrv->unregisterXmlProp(L"CdaBased");
  //if (::odrxDynamicLinker()->loadModule(OdModelDocObjModuleName).get())
    pPropSrv->unregisterXmlProp(L"AcDbViewBorder"); // (OdDbViewBorder::desc()->name());
  pPropSrv->unregisterXmlProp(OdDbMLeader::desc()->name());
  pPropSrv->unregisterXmlProp(OdDbBlockReference::desc()->name());
}

///////////////////////////////////////////////////////////////////////////////
